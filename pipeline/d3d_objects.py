"""d3d_objects.py -- Direct3D 8 objects held in stack slots, proven from the code and the D3D8 header.

A D3D8 method that creates or returns an object writes it through an out-parameter the header declares
(data/com_spec.json "out": [parameter index, interface]; CreateTexture's ppTexture is an
IDirect3DTexture8 **). Starting from the device calls com_calls.py proves (data/com_calls.json), each
function is followed with an exact model of its stack frame:

  * ESP at every instruction from sp_solve.insn_depths (only where the depth is exact); a register holding a
    frame address (lea r, [esp+N], mov ebp, esp, add r, imm) is tracked as entry ESP + offset;
  * every write into the frame is recorded: stores, pushes, and what a call may write (its own argument area
    and everything below ESP at the call);
  * a frame address that leaves the model (pushed, stored, passed in ECX/EDX, used in arithmetic not
    modelled) is an escape: the callee or code that receives it may write anything at or above it (STRICT:
    anything in the frame).

A slot S (4 bytes) is typed `iface *` when every write that can reach it is: the out-parameter of a proven
D3D8 call whose header type is that interface; a store of a register proven to hold that interface; or a
store of 0 -- and no escape reaches it. A value loaded from a typed slot is that interface, and a call
`mov r, [obj]; call [r + 4k]` with that object as its first stack argument is a proven call of the
interface's method k: its pops come from the header (and feed sp_solve's equations), its out-parameters
type more slots. Repeated to a fixpoint. A call or slot resting on an accepted (unproven) com_fields
declaration carries "accepted".

    python3 d3d_objects.py data/d3d_objects.json [--strict]        uses $FFXI_DLL
Output: {"slots": {"0xFUNC": {"off": {"iface", "accepted", "out_calls": [[call, arg index]]}}},
         "calls": {"0xCALL": {"func", "iface", "slot", "method", "pops", "accepted"}}}
"""
import json, os, sys, collections
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sp_solve as S
import com_calls as C

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
r32 = C.r32
WRITE_ONLY = {'mov', 'movzx', 'movsx', 'lea', 'pop', 'fstp', 'fst', 'fistp', 'fist', 'fnstsw', 'fnstcw', 'fstsw', 'fstcw',
              'fisttp'}


def analyse(im, f, ins, depth, seeds, slots_in, spec, strict):
    """one pass over function f. seeds: {call addr: (iface, method entry, accepted)} known before the pass;
    slots_in: {off: (iface, accepted)} typed slots assumed for loads. Returns (writes, escapes, calls found,
    out-writes)."""
    labels = S.labels_of(ins)
    st0 = ({}, {})                     # (regs: reg -> value, mem: frame off -> value)
    state = {f: st0}; work = [f]
    writes = collections.defaultdict(list)      # off -> [(insn, size, value)]
    escapes = []                                # offsets whose address left the model (None: a frame address unknown)
    found = {}; outw = []
    def val_esp(a):
        return ('SA', -depth[a][0]) if a in depth else None
    while work:
        a = work.pop(); regs, mem = state[a]; regs = dict(regs); mem = dict(mem); i = ins[a]; m = i.mnemonic; ops = i.operands
        esp = val_esp(a)
        def v_of(op):
            if op.type == X86_OP_REG:
                n = r32(i.reg_name(op.reg))
                if n == 'esp': return esp
                return regs.get(n) if op.size == 4 else None
            if op.type == X86_OP_IMM: return ('I', op.imm & 0xFFFFFFFF)
            return None
        def addr_of(op):
            """frame offset of a memory operand, 'other' if not in the frame, None if unknown"""
            if op.mem.segment or op.mem.index: return None if (op.mem.base and isinstance(v_of_reg(op.mem.base), tuple) and v_of_reg(op.mem.base)[0] == 'SA') or (op.mem.index and isinstance(v_of_reg(op.mem.index), tuple) and v_of_reg(op.mem.index)[0] == 'SA') else 'other'
            if not op.mem.base: return 'other'
            b = v_of_reg(op.mem.base)
            if b is None:
                n = r32(i.reg_name(op.mem.base))
                return None if n in ('esp', 'ebp') else 'other'
            if b[0] == 'SA': return b[1] + op.mem.disp
            return 'other'
        def v_of_reg(reg):
            n = r32(i.reg_name(reg))
            return esp if n == 'esp' else regs.get(n)
        # values read by this instruction that are frame addresses escape unless the use is modelled
        modelled = False
        new_regs = dict(regs)
        for r in C.written(i): new_regs.pop(r, None)
        if m == 'push' and ops:
            v = v_of(ops[0]) if ops[0].type != X86_OP_MEM else None
            if ops[0].type == X86_OP_MEM:
                o = addr_of(ops[0])
                v = mem.get(o) if isinstance(o, int) else None
                if isinstance(o, int) and v is None and o in slots_in: v = ('O',) + slots_in[o]
            if esp is not None:
                off = esp[1] - 4; writes[off].append((a, 4, v)); mem[off] = v
                if v is None: mem.pop(off, None)
            else: escapes.append(None)
            modelled = True
        elif m == 'mov' and len(ops) == 2:
            d, s = ops
            if d.type == X86_OP_REG and d.size == 4:
                if s.type == X86_OP_MEM and s.size == 4:
                    o = addr_of(s)
                    if isinstance(o, int):
                        v = mem.get(o)
                        if v is None and o in slots_in: v = ('O',) + slots_in[o]
                        if v is not None: new_regs[r32(i.reg_name(d.reg))] = v
                    elif o == 'other' and s.mem.base and not s.mem.index and s.mem.disp == 0:
                        b = v_of_reg(s.mem.base)
                        if b and b[0] == 'O': new_regs[r32(i.reg_name(d.reg))] = ('V',) + b[1:]       # the object's vtable
                else:
                    v = v_of(s)
                    if v is not None and v[0] != 'I': new_regs[r32(i.reg_name(d.reg))] = v
                modelled = True
            elif d.type == X86_OP_MEM:
                o = addr_of(d); v = v_of(s) if s.type != X86_OP_MEM else None
                if isinstance(o, int):
                    writes[o].append((a, d.size, v if d.size == 4 else None))
                    for k in range(o - 3, o + d.size): mem.pop(k, None)
                    if d.size == 4 and v is not None: mem[o] = v
                elif o is None: escapes.append(None)
                if v is not None and v[0] == 'SA': escapes.append(v[1])          # a frame address stored
                modelled = True
        elif m == 'lea' and ops[1].type == X86_OP_MEM:
            b = ops[1].mem.base; dn = r32(i.reg_name(ops[0].reg))
            if b and not ops[1].mem.index and not ops[1].mem.segment:
                bv = v_of_reg(b)
                if bv and bv[0] == 'SA': new_regs[dn] = ('SA', bv[1] + ops[1].mem.disp)
                elif bv is None and r32(i.reg_name(b)) in ('esp', 'ebp'): escapes.append(None)
            elif (b and (v_of_reg(b) or (None,))[0] == 'SA') or (ops[1].mem.index and (v_of_reg(ops[1].mem.index) or (None,))[0] == 'SA'):
                escapes.append(None)
            modelled = True
        elif m in ('add', 'sub') and ops[0].type == X86_OP_REG and ops[1].type == X86_OP_IMM and r32(i.reg_name(ops[0].reg)) != 'esp':
            n = r32(i.reg_name(ops[0].reg)); v = regs.get(n)
            if v and v[0] == 'SA': new_regs[n] = ('SA', v[1] + (ops[1].imm if m == 'add' else -ops[1].imm))
            modelled = True
        elif m == 'call':
            o = ops[0]
            if o.type == X86_OP_MEM and o.mem.base and not o.mem.index:
                vt = v_of_reg(o.mem.base); thisv = mem.get(esp[1]) if esp else None
                if vt and vt[0] == 'V' and thisv and thisv[0] == 'O' and thisv[1] == vt[1] and o.mem.disp % 4 == 0 \
                        and o.mem.disp // 4 < len(spec[vt[1]]):
                    found[a] = (vt[1], spec[vt[1]][o.mem.disp // 4], vt[2] or thisv[2])
            if a in seeds or a in found:
                iface, meth, acc = seeds.get(a) or found[a]
                if meth.get('out') and esp is not None:
                    k, oif = meth['out']; pv = mem.get(esp[1] + 4 + 4 * k)
                    if pv and pv[0] == 'SA': outw.append((pv[1], oif, acc, a, k))
            # what a call may write: its argument area and everything below ESP; ECX/EDX frame addresses escape
            for r in ('ecx', 'edx'):
                v = regs.get(r)
                if v and v[0] == 'SA': escapes.append(v[1])
            if esp is not None:
                writes[('below', esp[1] + 64)].append((a, 0, None))        # pushed args (<= 16 dwords) and the callee's frame
                for k in [x for x in mem if x < esp[1] + 64]: mem.pop(k, None)
            else: escapes.append(None)
            modelled = True
        elif m in ('cmp', 'test') or m.startswith('j'):
            modelled = True
        if not modelled:
            # any other instruction: a frame address it reads escapes; a frame write it makes is unknown
            for k, op in enumerate(ops):
                if op.type == X86_OP_REG:
                    v = v_of(op)
                    if v and v[0] == 'SA' and not (k == 0 and m in WRITE_ONLY): escapes.append(v[1])
                elif op.type == X86_OP_MEM:
                    o = addr_of(op)
                    if k == 0 and m not in ('cmp', 'test', 'push') or m in ('fstp', 'fst', 'fistp', 'fist', 'fnstsw', 'fnstcw', 'fisttp'):
                        if isinstance(o, int):
                            writes[o].append((a, max(op.size, 1), None))
                            for x in range(o - 3, o + max(op.size, 1)): mem.pop(x, None)
                        elif o is None: escapes.append(None)
            if m.startswith(('rep', 'stos', 'movs')): escapes.append(None)
        nxt = C.successors(im, ins, a, i, labels)
        if nxt is None: return None
        for n in nxt:
            if n not in ins: continue
            ns = (new_regs, mem)
            if n not in state: state[n] = ns; work.append(n)
            else:
                orr, om = state[n]
                rr = {r: v for r, v in orr.items() if new_regs.get(r) == v}; mm = {k: v for k, v in om.items() if mem.get(k) == v}
                if rr != orr or mm != om: state[n] = (rr, mm); work.append(n)
    return writes, escapes, found, outw


def type_slots(writes, escapes, outw, strict):
    """{off: (iface, accepted, [(call, k)])} for the slots every write of which is a proven object of one interface"""
    cand = {}
    for off, oif, acc, call, k in outw:
        c = cand.setdefault(off, [oif, acc, []])
        if c[0] != oif: c[0] = None
        c[1] = c[1] or acc; c[2].append((call, k))
    out = {}
    out_insns = {}
    for off, oif, acc, call, k in outw: out_insns.setdefault(off, set()).add(call)
    for off, (oif, acc, calls) in cand.items():
        if oif is None: continue
        if None in escapes or any(e is not None and (strict or e <= off) for e in escapes): continue
        ok = True
        for wo, ws in writes.items():
            if isinstance(wo, tuple):                     # a call's argument area / callee frame: all below wo[1]
                if off < wo[1] and not all(a in out_insns.get(off, ()) for a, _, _ in ws): ok = False
                continue
            for a, size, v in ws:
                if wo < off + 4 and off < wo + max(size, 1):
                    if wo == off and size == 4 and (v == ('I', 0) or (v and v[0] == 'O' and v[1] == oif)): acc = acc or (v[0] == 'O' and v[2])
                    else: ok = False
        if ok: out[off] = (oif, acc, calls)
    return out


def main(out, strict=False):
    im = S.Image(os.environ['FFXI_DLL'])
    ext = {'0x%08X' % int(k, 16): v for k, v in json.load(open(os.path.join(D, 'func_extents.json'))).items()}
    spec = json.load(open(os.path.join(D, 'com_spec.json')))
    purges = json.load(open(os.path.join(D, 'purges.json')))
    starts = {int(k, 16) for k in ext}
    base = json.load(open(os.path.join(D, 'com_calls.json')))
    seeds = {}
    for k, v in base.items():
        meth = spec[v['iface']][v['slot']]
        seeds[int(k, 16)] = (v['iface'], meth, bool(v.get('accepted')))
    funcs = {int(v['func'], 16) for v in base.values()}
    calls = {}; slots = {}
    for rnd in range(8):
        before = (len(calls), sum(len(s) for s in slots.values()))
        for k, (iface, meth, acc) in calls.items():                      # new calls' pops into the stack solver
            S.COMCALLS['0x%08X' % k] = 4 * (1 + meth['nparams'])
        for f in sorted(funcs):
            e = ext.get('0x%08X' % f)
            if not e: continue
            ins = im.code(e)
            if f not in ins: continue
            depth = S.insn_depths(im, f, e, purges, starts)
            sl = {o: (t[0], t[1]) for o, t in slots.get(f, {}).items()}
            sd = {a: s for a, s in list(seeds.items()) + list(calls.items()) if a in ins}
            r = analyse(im, f, ins, depth, sd, sl, spec, strict)
            if r is None: continue
            writes, escapes, found, outw = r
            slots[f] = type_slots(writes, escapes, outw, strict)
            for a, x in found.items():
                if a not in seeds: calls[a] = x
        if (len(calls), sum(len(s) for s in slots.values())) == before: break
    res = {'_doc': __doc__.split('\n\n')[0], 'strict': strict,
           'slots': {'0x%08X' % f: {str(o): {'iface': t[0], 'accepted': t[1], 'out_calls': [['0x%08X' % c, k] for c, k in t[2]]}
                                    for o, t in sorted(s.items())} for f, s in sorted(slots.items()) if s},
           'calls': {'0x%08X' % a: {'func': next(('0x%08X' % f for f in funcs if a in im.code(ext['0x%08X' % f])), None),
                                    'iface': x[0], 'method': x[1]['name'], 'pops': 4 * (1 + x[1]['nparams']), 'accepted': x[2]}
                     for a, x in sorted(calls.items())}}
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print('%d slots typed in %d functions (%s); %d further D3D8 calls proven (%d resting on an accepted declaration)' % (
        sum(len(s) for s in res['slots'].values()), len(res['slots']),
        dict(collections.Counter(t['iface'] for s in res['slots'].values() for t in s.values())),
        len(res['calls']), sum(x['accepted'] for x in res['calls'].values())))


if __name__ == '__main__':
    main(sys.argv[1], '--strict' in sys.argv)
