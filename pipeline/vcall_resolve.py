"""vcall_resolve.py -- virtual calls through `this` whose callee is provable from the code, and what they pop.

The DLL has no RTTI for its game classes, and a class tree rebuilt from constructors is not complete (an
inlined base constructor's vtable store is dead and the compiler drops it). So no tree is used. Instead:

  * vtables: every constant stored to [obj+0] or [obj+N] whose first dword is a function; a vtable runs
    while its dwords are functions, up to the next known vtable;
  * classes(F), the vtables an object can have while method F runs on it as `this`:
      - every vtable that holds F (F was reached by dispatch through it), plus
      - for each direct `call F` / `jmp F`: the caller's own classes if it passes its `this` unchanged in
        ECX, or the exact vtable it has just stored into its `this`;
      - TOP (anything) if F's address is taken any other way: an absolute reference in code or data that is
        not a vtable slot, a call from code no function extent covers, or a caller passing another object;
    computed as a fixpoint over the whole DLL;
  * a call `call [vt+4k]` where vt was loaded from [this+0] resolves to {X[k] for X in classes(F)}, or to
    the exact vtable stored into `this` earlier on every path (reset at any call). It is settled only when
    every candidate exists and all agree on `ret N` (data/purges.json); otherwise it is left out.

    python3 vcall_resolve.py data/vcalls.json                 uses $FFXI_DLL
Output: {"0xCALL": {"func", "slot", "targets": [...], "pops"}} for settled calls; "_unsettled": {reason: n}.
"""
import bisect, json, os, sys, struct, collections
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sp_solve as S

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
TOP = 'TOP'
R32 = {'al': 'eax', 'ah': 'eax', 'ax': 'eax', 'bl': 'ebx', 'bh': 'ebx', 'bx': 'ebx', 'cl': 'ecx', 'ch': 'ecx', 'cx': 'ecx',
       'dl': 'edx', 'dh': 'edx', 'dx': 'edx', 'si': 'esi', 'di': 'edi', 'bp': 'ebp', 'sp': 'esp'}
r32 = lambda r: R32.get(r, r)


def succs(im, ins, a, i):
    m = i.mnemonic; ops = i.operands
    if m.startswith('ret') or m in ('int3', 'hlt', 'ud2'): return []
    if m == 'jmp':
        if ops[0].type == X86_OP_IMM: return [ops[0].imm] if ops[0].imm in ins else []
        return None                                         # a switch or a tail jump through memory: see caller
    if m.startswith(('j', 'loop')): return [a + i.size] + ([ops[0].imm] if ops[0].type == X86_OP_IMM else [])
    return [a + i.size]


def written(i):
    """registers the instruction writes (32-bit names), conservatively"""
    m = i.mnemonic; out = set()
    if m in ('cmp', 'test', 'push', 'jmp', 'call') or m.startswith(('j', 'ret')): pass
    elif i.operands and i.operands[0].type == X86_OP_REG: out.add(r32(i.reg_name(i.operands[0].reg)))
    if m in ('xchg', 'xadd', 'cmpxchg') and len(i.operands) > 1 and i.operands[1].type == X86_OP_REG:
        out.add(r32(i.reg_name(i.operands[1].reg)))
    if m == 'pop' and i.operands[0].type == X86_OP_REG: out.add(r32(i.reg_name(i.operands[0].reg)))
    if m in ('cdq', 'mul', 'imul', 'div', 'idiv') or m.startswith(('rep', 'movs', 'stos', 'lods', 'scas', 'cmps')):
        out |= {'eax', 'edx', 'ecx', 'esi', 'edi'}
    if m in ('call',): out |= {'eax', 'ecx', 'edx'}
    if m in ('pushal', 'popal', 'leave', 'cpuid', 'rdtsc'): out |= {'eax', 'ebx', 'ecx', 'edx', 'esi', 'edi', 'ebp'}
    return out


def this_flow(im, ins, ea, vts):
    """per instruction, the state before it: (regs holding the entry ECX, regs holding its vtable pointer,
    exact vtable stored into it on every path, or None)"""
    start = (frozenset({'ecx'}), frozenset(), None)
    state = {ea: start}; work = [ea]
    def meet(x, y): return (x[0] & y[0], x[1] & y[1], x[2] if x[2] == y[2] else None)
    while work:
        a = work.pop(); i = ins[a]; Sx, V, X = state[a]; m = i.mnemonic; ops = i.operands
        S2, V2, X2 = set(Sx), set(V), X
        w = written(i)
        if m == 'mov' and len(ops) == 2 and ops[0].type == X86_OP_REG and ops[1].type == X86_OP_REG:
            d, s = r32(i.reg_name(ops[0].reg)), r32(i.reg_name(ops[1].reg))
            S2.discard(d); V2.discard(d)
            if s in Sx: S2.add(d)
            if s in V: V2.add(d)
        elif m == 'mov' and len(ops) == 2 and ops[0].type == X86_OP_REG and ops[1].type == X86_OP_MEM and ops[1].size == 4 \
                and ops[1].mem.base and not ops[1].mem.index and ops[1].mem.disp == 0 and r32(i.reg_name(ops[1].mem.base)) in Sx:
            d = r32(i.reg_name(ops[0].reg)); S2.discard(d); V2.add(d)
        else:
            for r in w: S2.discard(r); V2.discard(r)
        if m == 'mov' and len(ops) == 2 and ops[0].type == X86_OP_MEM and ops[0].mem.base and not ops[0].mem.index \
                and ops[0].mem.disp == 0 and r32(i.reg_name(ops[0].mem.base)) in Sx:
            X2 = ops[1].imm if ops[1].type == X86_OP_IMM and ops[1].imm in vts else None     # the vptr overwritten
        elif ops and ops[0].type == X86_OP_MEM and m not in ('cmp', 'test', 'push') and ops[0].mem.base \
                and r32(i.reg_name(ops[0].mem.base)) in Sx and ops[0].mem.disp == 0 and not ops[0].mem.index:
            X2 = None
        if m == 'call': X2 = None                           # any callee may rewrite the vptr
        nxt = succs(im, ins, a, i)
        if nxt is None: nxt = []
        new = (frozenset(S2), frozenset(V2), X2)
        for n in nxt:
            if n not in ins: continue
            if n not in state: state[n] = new; work.append(n)
            else:
                mm = meet(state[n], new)
                if mm != state[n]: state[n] = mm; work.append(n)
    return state


def main(out):
    im = S.Image(os.environ['FFXI_DLL']); base = im.base; img = im.img
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    purges = json.load(open(os.path.join(D, 'purges.json')))
    starts = {int(k, 16) for k in ext}
    code = {f: im.code(e) for f, e in ((int(k, 16), v) for k, v in ext.items())}
    merged = []                                              # the code the function extents cover, merged
    for a, b in sorted((a, b) for e in ext.values() for a, b in e):
        if merged and a <= merged[-1][1]: merged[-1][1] = max(merged[-1][1], b)
        else: merged.append([a, b])
    mstarts = [a for a, b in merged]
    def inside(x):
        j = bisect.bisect_right(mstarts, x) - 1
        return j >= 0 and merged[j][0] <= x < merged[j][1]
    insn_at = {}
    for f, ins in code.items():
        for a in ins: insn_at[a] = f
    u32 = lambda a: struct.unpack_from('<I', img, a - base)[0]

    # vtables: constants stored to [reg+N] whose first dword is a function
    vt_set = set()
    for f, ins in code.items():
        for a, i in ins.items():
            o = i.operands
            if i.mnemonic == 'mov' and len(o) == 2 and o[0].type == X86_OP_MEM and o[0].size == 4 and o[1].type == X86_OP_IMM:
                v = o[1].imm
                if base <= v < base + len(img) - 4 and u32(v) in starts: vt_set.add(v)
    vts = sorted(vt_set); vt_slots = {}
    for n, v in enumerate(vts):
        lim = vts[n + 1] if n + 1 < len(vts) else base + len(img)
        k = 0; sl = []
        while v + 4 * k + 4 <= lim and u32(v + 4 * k) in starts: sl.append(u32(v + 4 * k)); k += 1
        vt_slots[v] = sl
    slot_bytes = {}                                         # address inside a vtable -> vtable
    for v, sl in vt_slots.items():
        for k in range(len(sl)):
            for b in range(4): slot_bytes[v + 4 * k + b] = v
    member = collections.defaultdict(set)
    for v, sl in vt_slots.items():
        for f in sl: member[f].add(v)

    # every absolute 4-byte occurrence of a function's address in the image, and every rel32 call/jmp to it
    top = set(); reason = {}
    instr_refs = collections.defaultdict(set)               # function -> instruction addresses whose operand is it
    for f, ins in code.items():
        for a, i in ins.items():
            for o in i.operands:
                if o.type == X86_OP_IMM and o.imm in starts and not (i.mnemonic in ('call', 'jmp') or i.mnemonic.startswith('j')):
                    instr_refs[o.imm].add(a)
                if o.type == X86_OP_MEM and o.mem.disp in starts: instr_refs[o.mem.disp].add(a)
    for f in instr_refs:
        top.add(f); reason[f] = 'address taken in code'
    mv = memoryview(img); n = len(img)
    for off in range(0, n - 3):
        x = mv[off] | mv[off + 1] << 8 | mv[off + 2] << 16 | mv[off + 3] << 24
        if x in starts and x not in top:
            a = base + off
            if a in slot_bytes and (a - slot_bytes[a]) % 4 == 0: continue          # a vtable slot
            # inside a decoded function: only real operands count (handled above); elsewhere: unknown use
            if inside(a): continue
            top.add(x); reason[x] = 'referenced from data/undecoded code at 0x%08X' % a
        if mv[off] in (0xE8, 0xE9) and off + 5 <= n:
            t = (base + off + 5 + struct.unpack_from('<i', img, off + 1)[0]) & 0xFFFFFFFF
            if t in starts and t not in top and base + off not in insn_at and not inside(base + off):
                top.add(t); reason[t] = 'called from undecoded code at 0x%08X' % (base + off)

    # this-flow in every function, and its direct calls/jumps
    flows = {}
    callers = collections.defaultdict(list)                 # F -> [(caller, site state ecx-is-this, exact vt)]
    for f, ins in code.items():
        if f not in ins: continue
        st = this_flow(im, ins, f, vt_set); flows[f] = st
        for a, i in ins.items():
            o = i.operands
            if a not in st:                                 # code our flow did not reach (e.g. a switch case):
                if i.mnemonic == 'call' and o and o[0].type == X86_OP_IMM and o[0].imm in starts:
                    callers[o[0].imm].append((f, False, None))      # its object is unknown
                continue
            if (i.mnemonic == 'call' or i.mnemonic == 'jmp' or i.mnemonic.startswith('j')) and o and o[0].type == X86_OP_IMM \
                    and o[0].imm in starts and o[0].imm != f and not (i.mnemonic != 'call' and o[0].imm in ins):
                Sx, V, X = st[a]
                callers[o[0].imm].append((f, 'ecx' in Sx, X))
    # functions with direct calls from code we did not decode a flow for (no extent entry) are TOP above
    classes = {f: set(member.get(f, ())) for f in starts}
    for f in top: classes[f] = TOP
    changed = True
    while changed:
        changed = False
        for f, cl in callers.items():
            if classes.get(f) == TOP: continue
            cur = classes[f]
            for g, this_passed, X in cl:
                if not this_passed: add = TOP
                elif X is not None: add = {X}
                else: add = classes.get(g, TOP)
                if add == TOP:
                    classes[f] = TOP; reason[f] = 'called with an object other than the caller\'s this (from 0x%08X)' % g; changed = True; break
                if not add <= cur: cur |= add; changed = True

    # resolve
    res = {}; why = collections.Counter()
    for f, ins in code.items():
        st = flows.get(f)
        if not st: continue
        for a, i in ins.items():
            if i.mnemonic != 'call' or a not in st: continue
            o = i.operands[0]
            if o.type != X86_OP_MEM or not o.mem.base or o.mem.index or o.mem.disp % 4: continue
            Sx, V, X = st[a]
            if r32(i.reg_name(o.mem.base)) not in V: continue
            k = o.mem.disp // 4
            cand = {X} if X is not None else classes.get(f, TOP)
            if cand == TOP: why['this may be any object'] += 1; continue
            if not cand: why['method reached by no vtable and no caller'] += 1; continue
            if any(len(vt_slots[v]) <= k for v in cand): why['slot past a vtable end'] += 1; continue
            tg = sorted({vt_slots[v][k] for v in cand}); pp = {purges.get('0x%08X' % t) for t in tg}
            if None in pp or len(pp) != 1: why['candidates disagree or pop unknown'] += 1; continue
            res['0x%08X' % a] = {'func': '0x%08X' % f, 'slot': k, 'targets': ['0x%08X' % t for t in tg], 'pops': pp.pop(),
                                 'exact': X is not None}
    res['_unsettled'] = dict(why); res['_vtables'] = len(vt_slots)
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print('%d vtables; %d virtual calls on `this` settled; unsettled: %s' % (len(vt_slots), len(res) - 2, dict(why)))


if __name__ == '__main__':
    main(sys.argv[1])
