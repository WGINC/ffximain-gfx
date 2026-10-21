"""this_flow.py -- what each direct call passes in ECX, relative to the caller's own entry ECX, from the code.

A method's class can be read off its callers: where every caller hands on the object it was itself called on, the
callee works on that object. This gives, at every direct call, ECX's value as the caller's code computes it:

  * `own`: the caller's own entry ECX (its `this`), unchanged;
  * `own+K`: that plus a constant (a sub-object at offset K, `lea ecx, [esi+K]`);
  * `global 0xG`: the dword loaded from a global (`mov ecx, [G]`);
  * otherwise not listed.

Values are followed over the caller's control flow (vcall_resolve.succs) through register copies and stack slots
(keyed by sp_solve's exact depths, as reg_saved.py does). A call keeps ECX only where the callee leaves it untouched
(data/reg_keeps.json), and EBX, ESI, EDI, EBP only where the callee restores them (data/reg_saved.json); everything
else a call may change is dropped. At a join a value is kept only where every path agrees. A caller whose depths
are not exact, or whose flow cannot be followed, gives nothing.

    FFXI_DLL=... python3 this_flow.py data/this_flow.json
Output {"0xCALLEE": [["0xCALLSITE", "0xCALLER", "own" | "own+0xK" | "global 0xG"], ...]}"""
import json, os, sys
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import sp_solve as S
import vcall_resolve as V
D = os.path.join(P, '..', 'data')
REGS = ('eax', 'ecx', 'edx', 'ebx', 'esi', 'edi', 'ebp')
PART = {'al': 'eax', 'ah': 'eax', 'ax': 'eax', 'cl': 'ecx', 'ch': 'ecx', 'cx': 'ecx', 'dl': 'edx', 'dh': 'edx', 'dx': 'edx',
        'bl': 'ebx', 'bh': 'ebx', 'bx': 'ebx', 'si': 'esi', 'di': 'edi', 'bp': 'ebp'}


def main(out):
    im = S.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    purges = json.load(open(os.path.join(D, 'purges.json')))
    keeps = {int(k, 16): set(v) for k, v in json.load(open(os.path.join(D, 'reg_keeps.json'))).items()}
    saved = {int(k, 16): set(v) for k, v in json.load(open(os.path.join(D, 'reg_saved.json'))).items()}
    iat = S.imports(im, os.environ['FFXI_DLL']); nr = S.noreturn_funcs(im, ext, iat)
    starts = {int(k, 16) for k in ext}
    res = {}
    for k, e in ext.items():
        f = int(k, 16); ins = im.code(e)
        dep = S.insn_depths(im, f, e, purges, starts, nr=nr, iat=iat)
        if not dep or f not in ins: continue
        labels = S.labels_of(ins)
        state = {f: ({'ecx': ('own', 0)}, {})}; work = [f]; sites = {}
        ok = True
        while work and ok:
            a = work.pop(); i = ins.get(a)
            if i is None or a not in dep: ok = False; break
            regs, slots = dict(state[a][0]), dict(state[a][1])
            d, eb = dep[a]; m, o = i.mnemonic, i.operands
            def key(op):
                b = i.reg_name(op.mem.base) if op.mem.base else None
                if op.mem.index or b not in ('esp', 'ebp'): return None
                return d - op.mem.disp if b == 'esp' else ((eb - op.mem.disp) if eb is not None else None)
            def reg(op): return i.reg_name(op.reg) if op.type == X86_OP_REG else None
            ends = False
            if m == 'call':
                t = o[0]
                if t.type == X86_OP_IMM and t.imm in starts:
                    v = regs.get('ecx')
                    if v: sites[a] = (t.imm, v)
                    kept = keeps.get(t.imm, set()) | saved.get(t.imm, set())
                    regs = {r: x for r, x in regs.items() if r in kept}
                    slots = {kk: x for kk, x in slots.items() if kk <= d - 4 * 16}     # args a callee may change: dropped
                else:
                    regs = {r: x for r, x in regs.items() if r in ('ebx', 'esi', 'edi', 'ebp')} if (
                        t.type == X86_OP_MEM and not t.mem.base and not t.mem.index and (t.mem.disp & 0xFFFFFFFF) in iat) else {}
                    slots = {kk: x for kk, x in slots.items() if kk <= d - 4 * 16}
            elif m.startswith('ret') or (m == 'jmp' and o and o[0].type == X86_OP_IMM and o[0].imm not in ins):
                ends = True
            elif m == 'push':
                slots[d + 4] = regs.get(reg(o[0])) if reg(o[0]) in REGS else None
            elif m == 'pop':
                r = reg(o[0])
                if r in REGS: regs[r] = slots.get(d)
                elif r in PART: regs.pop(PART[r], None)
            elif m == 'mov' and reg(o[0]) in REGS and reg(o[1]) in REGS:
                regs[reg(o[0])] = regs.get(reg(o[1]))
            elif m == 'mov' and reg(o[0]) in REGS and o[1].type == X86_OP_MEM and o[1].size == 4:
                kk = key(o[1])
                if kk is not None: regs[reg(o[0])] = slots.get(kk)
                elif not o[1].mem.base and not o[1].mem.index: regs[reg(o[0])] = ('global', o[1].mem.disp & 0xFFFFFFFF)
                else: regs.pop(reg(o[0]), None)
            elif m == 'lea' and reg(o[0]) in REGS and o[1].type == X86_OP_MEM and o[1].mem.base and not o[1].mem.index \
                    and i.reg_name(o[1].mem.base) in REGS and regs.get(i.reg_name(o[1].mem.base)) and regs[i.reg_name(o[1].mem.base)][0] == 'own':
                regs[reg(o[0])] = ('own', regs[i.reg_name(o[1].mem.base)][1] + o[1].mem.disp)
            elif m in ('add', 'sub') and reg(o[0]) in REGS and o[1].type == X86_OP_IMM and regs.get(reg(o[0])) and regs[reg(o[0])][0] == 'own':
                regs[reg(o[0])] = ('own', regs[reg(o[0])][1] + (o[1].imm if m == 'add' else -o[1].imm))
            else:
                try: rd, wr = i.regs_access()
                except Exception: ok = False; break
                for w in (i.reg_name(x) for x in wr):
                    if w in REGS: regs.pop(w, None)
                    elif w in PART: regs.pop(PART[w], None)
            for op in o:                                               # a stack slot written
                if op.type == X86_OP_MEM and (op.access & 2) and m != 'push':
                    kk = key(op)
                    if kk is not None:
                        v = regs.get(reg(o[1])) if m == 'mov' and len(o) == 2 and reg(o[1]) in REGS and op.size == 4 else None
                        for x in range(kk - op.size + 1, kk + 4): slots.pop(x, None)
                        if v: slots[kk] = v
                    elif op.mem.base and i.reg_name(op.mem.base) in ('esp', 'ebp'): slots = {}
            if ends: continue
            nx = V.succs(im, ins, a, i, labels)
            if nx is None: ok = False; break
            for n in nx:
                if n not in ins: continue
                if n not in state: state[n] = (regs, slots); work.append(n); continue
                r0, s0 = state[n]
                r1 = {r: x for r, x in r0.items() if regs.get(r) == x}; s1 = {kk: x for kk, x in s0.items() if slots.get(kk) == x}
                if (r1, s1) != (r0, s0): state[n] = (r1, s1); work.append(n)
        if not ok: continue
        for a, (t, v) in sites.items():
            desc = ('own' if v[1] == 0 else 'own+0x%X' % (v[1] & 0xFFFFFFFF)) if v[0] == 'own' else 'global 0x%08X' % v[1]
            res.setdefault('0x%08X' % t, []).append(['0x%08X' % a, '0x%08X' % f, desc])
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print('%d callees with call sites whose ECX is known (%d sites)' % (len(res), sum(len(v) for v in res.values())))


if __name__ == '__main__':
    main(sys.argv[1])
