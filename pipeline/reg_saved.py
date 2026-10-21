"""reg_saved.py -- which of EBX, ESI, EDI, EBP each function hands back to its caller as it found them, from the code.

MSVC keeps values a function needs across its calls in these registers (a method's `this` in ESI). Following such a
value across a call is exact only where the callee is shown to restore the register; that is this fact.

Over each function's control flow (vcall_resolve.succs), from the entry, every register and every stack slot holds a
symbolic value: one of the four registers' entry values, or nothing known. Stack slots are keyed by their distance
below the entry ESP, from sp_solve.insn_depths (exact: every pop solved); a `push esi` at depth d stores ESI's value
in slot d+4, the `pop esi` at depth d+4 reads it back. At a join a value is kept only where every path agrees.

  * A register is restored when at every `ret` (and every tail jump to a function that restores it) it holds its
    entry value.
  * A direct call keeps a register's value when the callee restores it (this fact, a greatest fixpoint: every
    function first assumed to restore all four, then refuted). It may change its own arguments: the slots of its
    `ret N` (purges.json) or its exact cdecl argument count (cdecl_args.json); with neither known, every slot is
    dropped.
  * An import keeps all four: Microsoft's documented x86 conventions (__stdcall, __cdecl) require every Win32
    function to preserve EBX, ESI, EDI and EBP. It may change its arguments (import_nargs.json; unknown: every slot
    dropped).
  * A function is left undecided (not listed) when any instruction's stack depth is not exact, when it takes a stack
    slot's address (`lea` from ESP or EBP), when it calls or jumps through a pointer, or when its control flow
    cannot be followed (an indirect jump the code does not bound).

    FFXI_DLL=... python3 reg_saved.py data/reg_saved.json
Output {"0xFUNC": ["ebp", "ebx", "edi", "esi"]}: the registers it restores (a function restoring none is listed with
an empty list; an undecided one is not listed)."""
import json, os, sys
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import sp_solve as S
import vcall_resolve as V
D = os.path.join(P, '..', 'data')
SAVED = ('ebx', 'esi', 'edi', 'ebp')
PART = {'bl': 'ebx', 'bh': 'ebx', 'bx': 'ebx', 'ebx': 'ebx', 'si': 'esi', 'esi': 'esi', 'di': 'edi', 'edi': 'edi',
        'bp': 'ebp', 'ebp': 'ebp'}


def main(out):
    im = S.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    purges = json.load(open(os.path.join(D, 'purges.json')))
    cargs = {k: v for k, v in json.load(open(os.path.join(D, 'cdecl_args.json'))).items() if not k.startswith('_')}
    inargs = json.load(open(os.path.join(D, 'import_nargs.json')))
    iat = S.imports(im, os.environ['FFXI_DLL']); nr = S.noreturn_funcs(im, ext, iat)
    starts = {int(k, 16) for k in ext}
    code, depths, labels = {}, {}, {}
    for k, e in ext.items():
        f = int(k, 16); code[f] = im.code(e)
        depths[f] = S.insn_depths(im, f, e, purges, starts, nr=nr, iat=iat)
        labels[f] = S.labels_of(code[f])

    def argbytes(t):
        p = purges.get('0x%08X' % t)
        if p: return p
        c = cargs.get('0x%08X' % t)
        return 4 * c if isinstance(c, int) else None

    undecided = set()
    restores = {f: set(SAVED) for f in code}

    def analyse(f):
        ins, dep = code[f], depths[f]
        if not dep or f not in ins: return None
        for a, i in ins.items():
            for o in i.operands:
                if i.mnemonic == 'lea' and o.type == X86_OP_MEM and o.mem.base and i.reg_name(o.mem.base) in ('esp', 'ebp'): return None
        entry = ({r: r for r in SAVED}, {})
        state = {f: entry}; work = [f]; at_ret = None
        def val(regs, r): return regs.get(PART.get(r, r)) if r in PART and PART[r] == r else None
        while work:
            a = work.pop(); i = ins.get(a)
            if i is None or a not in dep: return None
            regs, slots = dict(state[a][0]), dict(state[a][1])
            d, e = dep[a]; m, o = i.mnemonic, i.operands
            def slot_key(op):
                b = i.reg_name(op.mem.base) if op.mem.base else None
                if op.mem.index or b not in ('esp', 'ebp'): return None
                if b == 'esp': return d - op.mem.disp
                return (e - op.mem.disp) if e is not None else None
            ends = False
            if m == 'push':
                v = regs.get(i.reg_name(o[0].reg)) if o[0].type == X86_OP_REG and i.reg_name(o[0].reg) in SAVED else None
                slots[d + 4] = v
            elif m == 'pop':
                if o[0].type == X86_OP_REG:
                    r = PART.get(i.reg_name(o[0].reg))
                    if r and r == i.reg_name(o[0].reg): regs[r] = slots.get(d)
                    elif r: regs[r] = None
            elif m == 'call':
                t = o[0]
                if t.type == X86_OP_IMM and t.imm in code:
                    for r in SAVED:
                        if r not in restores.get(t.imm, ()): regs[r] = None
                    n = argbytes(t.imm)
                    slots = {k: v for k, v in slots.items() if n is not None and not (d - n < k <= d)} if n is not None else {}
                elif t.type == X86_OP_MEM and not t.mem.base and not t.mem.index and (t.mem.disp & 0xFFFFFFFF) in iat:
                    n = inargs.get(iat[t.mem.disp & 0xFFFFFFFF])
                    slots = {k: v for k, v in slots.items() if not (d - 4 * n < k <= d)} if isinstance(n, int) else {}
                else: return None                                   # through a pointer
            elif m.startswith('ret'):
                held = {r for r in SAVED if regs.get(r) == r}
                at_ret = held if at_ret is None else at_ret & held; ends = True
            elif m == 'jmp' and o and o[0].type == X86_OP_IMM and o[0].imm not in ins:
                if o[0].imm not in code: return None
                held = {r for r in SAVED if regs.get(r) == r and r in restores.get(o[0].imm, ())}
                at_ret = held if at_ret is None else at_ret & held; ends = True
            else:
                try: rd, wr = i.regs_access()
                except Exception: return None
                wr = {i.reg_name(x) for x in wr}
                if m == 'mov' and o[0].type == X86_OP_REG and o[1].type == X86_OP_REG and i.reg_name(o[0].reg) in SAVED:
                    regs[i.reg_name(o[0].reg)] = regs.get(i.reg_name(o[1].reg)) if i.reg_name(o[1].reg) in SAVED else None
                elif m == 'mov' and o[0].type == X86_OP_REG and o[1].type == X86_OP_MEM and i.reg_name(o[0].reg) in SAVED \
                        and slot_key(o[1]) is not None and o[1].size == 4:
                    regs[i.reg_name(o[0].reg)] = slots.get(slot_key(o[1]))
                else:
                    for w in wr:
                        if w in PART: regs[PART[w]] = None
                for op in o:                                       # a write to a stack slot
                    if op.type == X86_OP_MEM and (op.access & 2):
                        k = slot_key(op)
                        if k is None and op.mem.base and i.reg_name(op.mem.base) in ('esp', 'ebp'): return None
                        if k is not None:
                            v = regs.get(i.reg_name(o[1].reg)) if m == 'mov' and len(o) == 2 and o[1].type == X86_OP_REG \
                                and i.reg_name(o[1].reg) in SAVED and op.size == 4 else None
                            for kk in range(k - op.size + 1, k + 4): slots.pop(kk, None)
                            slots[k] = v
            if ends: continue
            nx = V.succs(im, ins, a, i, labels[f])
            if nx is None: return None
            for n in nx:
                if n not in ins: continue
                if n not in state: state[n] = (regs, slots); work.append(n); continue
                r0, s0 = state[n]
                r1 = {r: v for r, v in r0.items() if regs.get(r) == v}
                s1 = {k: v for k, v in s0.items() if slots.get(k) == v}
                if (r1, s1) != (r0, s0): state[n] = (r1, s1); work.append(n)
        return at_ret if at_ret is not None else set(SAVED)        # no return: nothing handed back

    rnd = 0
    while True:
        rnd += 1; changed = 0
        for f in code:
            if f in undecided: continue
            r = analyse(f)
            if r is None: undecided.add(f); restores[f] = set(); changed += 1; continue
            if r != restores[f]: restores[f] = r; changed += 1
        print('round %d: %d changed' % (rnd, changed), file=sys.stderr)
        if not changed: break
    res = {'0x%08X' % f: sorted(v) for f, v in sorted(restores.items()) if f not in undecided}
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    import collections
    print('%d functions decided (%d undecided); restore all four: %d' % (len(res), len(undecided), sum(len(v) == 4 for v in res.values())))


if __name__ == '__main__':
    main(sys.argv[1])
