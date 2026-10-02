# Runs inside IDA. Read-only: classifies pipeline/IDA disagreements.
import json, collections, os
D=r"C:\Users\wadeglover\Downloads\ffxi_ida_exchange"
L=json.load(open(os.path.join(D,"cmp_lists.json")))
def st(a):
    f=ida_bytes.get_flags(a)
    if ida_bytes.is_code(f): return 'code-head' if ida_bytes.is_head(f) else 'code-tail'
    if ida_bytes.is_data(f): return 'data'
    if ida_bytes.is_tail(f): return 'tail'
    return 'unexplored'
res={}
c=collections.Counter(); ex=collections.defaultdict(list)
for h in L['ponly']:
    a=int(h,16); fn=ida_funcs.get_func(a)
    s=st(a)
    xr=[x for x in idautils.XrefsTo(a)]
    kinds=set()
    for x in xr:
        seg=ida_segment.getseg(x.frm); kinds.add(('code' if seg and seg.perm&1 else 'data') if seg else '?')
    if fn:
        k='in-ida-func:'+s+(':lib-owner' if fn.flags&ida_funcs.FUNC_LIB else '')
        if fn.start_ea!=a:
            # is it a separate IDA chunk (tail)?
            ch=ida_funcs.get_fchunk(a)
            if ch and ch.start_ea==a and ch.start_ea!=fn.start_ea: k+=':is-ida-tailchunk'
    else:
        k='no-ida-func:'+s
    k+=':xref='+('+'.join(sorted(kinds)) or 'none')
    c[k]+=1; ex[k].append(h)
res['ponly']={k:(v,ex[k][:6]) for k,v in c.most_common()}
c=collections.Counter(); ex=collections.defaultdict(list)
for h in L['ionly']:
    a=int(h,16); fn=ida_funcs.get_func(a)
    k=('lib' if fn.flags&ida_funcs.FUNC_LIB else 'nonlib')+(':thunk' if fn.flags&ida_funcs.FUNC_THUNK else '')
    nx=len([x for x in idautils.XrefsTo(a) if not (fn.start_ea<=x.frm<fn.end_ea)])
    k+=':xrefs=%s'%('0' if nx==0 else '1' if nx==1 else 'many')
    c[k]+=1; ex[k].append((h,ida_name.get_name(a)))
res['ionly']={k:(v,ex[k][:6]) for k,v in c.most_common()}
# size mismatches: does IDA's func end at a lib-named function start?
c=collections.Counter(); ex=collections.defaultdict(list)
for h in L['sizemis']:
    a=int(h,16); fn=ida_funcs.get_func(a)
    pend=max(int(b,16) for _,b in L['pchunks'][h])
    iend=max(e for _,e in idautils.Chunks(a))
    nxt=ida_funcs.get_func(fn.end_ea) if fn else None
    if iend<pend:
        k='ida-shorter'
        if nxt and nxt.start_ea<pend: k+=':next-ida-func-inside' + (':lib' if nxt.flags&ida_funcs.FUNC_LIB else ':nonlib')
    else:
        k='ida-longer'
        if len(list(idautils.Chunks(a)))>len(L['pchunks'][h]): k+=':ida-more-chunks'
    c[k]+=1; ex[k].append(h)
res['sizemis']={k:(v,ex[k][:6]) for k,v in c.most_common()}
json.dump(res,open(os.path.join(D,"classify_out.json"),"w"),indent=1)
result={k:{kk:vv[0] for kk,vv in v.items()} for k,v in res.items()}
