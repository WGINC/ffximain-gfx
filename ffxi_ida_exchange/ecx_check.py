# Runs inside IDA, read-only. Path-sensitive "is ECX read before written" over IDA's flowchart.
# Rules (decided by hand on samples 0x10008870 / 0x100014A0 / 0x1000A290 / 0x10018B00):
#  * `push ecx` as a function's FIRST instruction is MSVC's 4-byte stack reservation, not a read,
#    unless the slot is popped/read back before it is written -- reported separately as 'push-only'.
#  * a call to a function whose convention (pipeline, else IDA) is thiscall while ECX is still
#    the incoming value counts as a read (pass-through `this`).
#  * a jmp to another function (tail call) likewise, if the target is thiscall.
#  * calls clobber ECX (it becomes defined).
import json, os, collections
D=r"C:\Users\wadeglover\Downloads\ffxi_ida_exchange"
PP=json.load(open(os.path.join(D,"pipeline_protos.json")))
PPi={int(k,16):v for k,v in PP.items()}
ECX_NAMES={'ecx','cx','cl','ch'}
def regs_in(op):
    s=idc.print_operand(op[0],op[1]).lower()
    return s
def callee_this(t):
    if t in PPi: return PPi[t][0]=='thiscall'
    tp=idc.get_type(t) or ''
    return '__thiscall' in tp
def analyse(fea):
    f=ida_funcs.get_func(fea)
    if not f or f.start_ea!=fea: return None
    fc=ida_gdl.FlowChart(f)
    blocks={b.start_ea:b for b in fc}
    # state: True = ECX still holds incoming value
    work=[(fea,True)]; seen=set(); reads=[]; pushfirst=False
    while work:
        bea,live=work.pop()
        if (bea,live) in seen or bea not in blocks: continue
        seen.add((bea,live)); b=blocks[bea]
        ea=b.start_ea
        while ea<b.end_ea and live:
            insn=ida_ua.insn_t(); 
            if not ida_ua.decode_insn(insn,ea): break
            mn=insn.get_canon_mnem()
            ops=[idc.print_operand(ea,i).lower() for i in range(3)]
            feat=insn.get_canon_feature()
            if mn=='push' and ops[0]=='ecx' and ea==fea:
                pushfirst=True
            elif mn in('call',):
                t=idc.get_operand_value(ea,0)
                if ida_funcs.get_func(t) and callee_this(t): reads.append(ea)
                # register-indirect call through ecx
                if 'ecx' in ops[0]: reads.append(ea)
                live=False; break
            elif mn=='jmp' and idc.get_operand_type(ea,0) in (idc.o_near,idc.o_far):
                t=idc.get_operand_value(ea,0)
                tf=ida_funcs.get_func(t)
                if tf and tf.start_ea==t and t!=fea and callee_this(t): reads.append(ea)
            else:
                # source uses: op1.. are sources for most; op0 is source if CF_USE1
                used=False
                for i,o in enumerate(ops[:3]):
                    if not o: continue
                    toks=set(o.replace('[',' ').replace(']',' ').replace('+',' ').replace('*',' ').replace(',',' ').replace('-',' ').split())
                    if not (toks & ECX_NAMES): continue
                    is_mem = '[' in o
                    if is_mem: used=True
                    elif i==0:
                        if feat & ida_idp.CF_USE1: used=True
                    else: used=True
                if mn=='xor' and ops[0]==ops[1]=='ecx': used=False
                if used: reads.append(ea); live=False; break
                # write kills
                if ops[0] in ECX_NAMES and (feat & ida_idp.CF_CHG1) and ops[0] in ('ecx',): live=False; break
            ea=idc.next_head(ea,b.end_ea)
        if reads: break
        for s in b.succs(): work.append((s.start_ea,live))
    return {'reads':[hex(x) for x in reads[:1]],'pushfirst':pushfirst}
out={}; c=collections.Counter()
I={}
for fea in idautils.Functions():
    I[fea]=idc.get_type(fea) or ''
targets=[]
for k,(conv,n) in PP.items():
    a=int(k,16)
    if a not in I: continue
    it=I[a]; ithis='__thiscall' in it
    if conv in('thiscall','stdcall','cdecl') and ((conv=='thiscall')!=ithis): targets.append(a)
for a in targets:
    r=analyse(a)
    if r is None: continue
    conv=PPi[a][0]
    reads=bool(r['reads'])
    verdict = ('pipeline' if (conv=='thiscall')==reads else 'ida')
    k=(conv, 'ida-this' if '__thiscall' in I[a] else 'ida-notthis', 'ecx-read' if reads else ('push-first-only' if r['pushfirst'] else 'no-read'), 'agrees:'+verdict)
    c[str(k)]+=1; out.setdefault(str(k),[]).append(hex(a))
json.dump(out,open(os.path.join(D,"ecx_check_out.json"),"w"),indent=0)
result=dict(c.most_common())
