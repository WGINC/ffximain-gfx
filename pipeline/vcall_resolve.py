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
Output: {"0xCALL": {"func", "slot", "targets": [...], "pops", "ecx", "ret"}} for settled calls ("ecx": the callees
read their entry ECX (true) or never do (false), null if that cannot be told; "ret": "eax" or "st0" from
fpret.json); "_unsettled": {reason: n}. Also ecx_use.json next to it: {"0xFUNC": "the instruction reading it" / false} for every
function whose use of its entry ECX is decided (`or ecx, -1`, `xor ecx, ecx` are writes; `push ecx`, which
MSVC also uses to reserve 4 stack bytes, leaves it undecided).
"""
import bisect, json, os, sys, struct, collections
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sp_solve as S
import com_calls as C

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
TOP = 'TOP'
R32 = {'al': 'eax', 'ah': 'eax', 'ax': 'eax', 'bl': 'ebx', 'bh': 'ebx', 'bx': 'ebx', 'cl': 'ecx', 'ch': 'ecx', 'cx': 'ecx',
       'dl': 'edx', 'dh': 'edx', 'dx': 'edx', 'si': 'esi', 'di': 'edi', 'bp': 'ebp', 'sp': 'esp'}
r32 = lambda r: R32.get(r, r)


def succs(im, ins, a, i, labels):
    """the next addresses in the function: a switch through a table only where the code bounds its index
    (com_calls.successors, sp_solve.switch_targets); None for any other indirect jump, which the callers
    treat as code they cannot follow (no must-fact holds after a join it may reach)"""
    return C.successors(im, ins, a, i, labels)


def written_exact(i):
    """registers the instruction writes (32-bit names), as the Intel manual defines each instruction: imul with
    two or three operands writes its first operand only; one-operand mul/imul/div/idiv write eax and edx; cdq
    writes edx; the string instructions write their count and pointer registers (and eax for lods)"""
    m = i.mnemonic; ops = i.operands
    base = m.split()[-1] if m.startswith('rep') else m
    string = base[:4] in ('movs', 'cmps', 'stos', 'scas', 'lods') and base[4:] in ('b', 'w', 'd', '') \
        and not any(o.type == X86_OP_REG and i.reg_name(o.reg).startswith('xmm') for o in ops)   # not SSE movsd/cmpsd
    if not string: base = ''
    if base.startswith('movs') or base.startswith('cmps'): return {'ecx', 'esi', 'edi'} if m.startswith('rep') else {'esi', 'edi'}
    if base.startswith('stos') or base.startswith('scas'): return {'ecx', 'edi'} if m.startswith('rep') else {'edi'}
    if base.startswith('lods'): return {'ecx', 'esi', 'eax'} if m.startswith('rep') else {'esi', 'eax'}
    if m == 'cdq': return {'edx'}
    if m in ('mul', 'imul', 'div', 'idiv') and len(ops) == 1: return {'eax', 'edx'}
    if m == 'imul': return {r32(i.reg_name(ops[0].reg))}
    return written(i)


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


def this_flow(im, ins, ea, vts, written=None):
    """per instruction, the state before it: (regs holding the entry ECX, regs holding its vtable pointer,
    exact vtable stored into it on every path, or None)"""
    written = written or globals()['written']
    start = (frozenset({'ecx'}), frozenset(), None)
    state = {ea: start}; work = [ea]; labels = S.labels_of(ins)
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
        nxt = succs(im, ins, a, i, labels)
        if nxt is None: return {}                         # an indirect jump not bounded: nothing is known here
        new = (frozenset(S2), frozenset(V2), X2)
        for n in nxt:
            if n not in ins: continue
            if n not in state: state[n] = new; work.append(n)
            else:
                mm = meet(state[n], new)
                if mm != state[n]: state[n] = mm; work.append(n)
    return state


ECX_PARTS = {'ecx', 'cx', 'cl', 'ch'}
REG = 'ecx'             # the register ecx_inputs follows (edx_inputs.py sets 'edx')


WRITE_ONLY = ('mov', 'movzx', 'movsx', 'lea', 'pop')
COVERS = {'ecx': {'ecx', 'cx', 'cl', 'ch'}, 'cx': {'cx', 'cl', 'ch'}, 'cl': {'cl'}, 'ch': {'ch'}}


def covered(part, written_parts):
    """is all of `part` written (so reading it is not reading the entry ECX)?"""
    have = set().union(*(COVERS[w] for w in written_parts)) if written_parts else set()
    if {'cl', 'ch'} <= have: have.add('cx')
    return part in have and (part != 'ecx' or 'ecx' in written_parts)


def masked_read(i, parts):
    """`and r32, imm` whose immediate keeps only bits 0-7, 8-15 or 0-15: the result's other bits are 0 whatever
    the register held (Intel SDM, AND), so the instruction reads only the part those bits are in and writes the
    whole register. ({the part read}, or None when the instruction is not such an `and`.) parts: the register's
    names in the order 32-bit, 16-bit, low byte, high byte"""
    ops = i.operands
    if i.mnemonic != 'and' or len(ops) != 2 or ops[0].type != X86_OP_REG or ops[1].type != X86_OP_IMM: return None
    full, x, lo, hi = parts
    if i.reg_name(ops[0].reg) != full: return None
    v = ops[1].imm & 0xFFFFFFFF
    if v & ~0xFF == 0: return {lo}
    if v & ~0xFF00 == 0: return {hi}
    if v & ~0xFFFF == 0: return {x}
    return None


def ecx_effect(i):
    """(parts of ECX the instruction reads, parts it writes, may-read-unknown)"""
    m = i.mnemonic; ops = i.operands; rd, wr, unk = set(), set(), False
    if m in ('pushal', 'pushad'): return rd, wr, True                  # saves it; whether it is used is unknown
    if m.startswith(('loop', 'rep')) or m == 'jecxz' or (i.prefix[0] in (0xF2, 0xF3) and m.startswith(('movs', 'stos', 'lods', 'scas', 'cmps'))):
        rd.add('ecx')
    if m == 'call': return rd, {'ecx'}, False                           # a call leaves ECX clobbered
    if m in ('xor', 'sub') and len(ops) == 2 and ops[0].type == X86_OP_REG and ops[1].type == X86_OP_REG \
            and ops[0].reg == ops[1].reg and i.reg_name(ops[0].reg) in ECX_PARTS:
        return rd, {i.reg_name(ops[0].reg)}, False                     # `xor ecx, ecx`: a write
    if len(ops) == 2 and ops[0].type == X86_OP_REG and i.reg_name(ops[0].reg) in ECX_PARTS and ops[1].type == X86_OP_IMM \
            and ((m == 'or' and ops[1].imm & ((1 << 8 * ops[0].size) - 1) == (1 << 8 * ops[0].size) - 1)
                 or (m == 'and' and ops[1].imm & ((1 << 8 * ops[0].size) - 1) == 0)):
        return rd, {i.reg_name(ops[0].reg)}, False                     # `or ecx, -1` / `and ecx, 0`: a constant
    if masked_read(i, ('ecx', 'cx', 'cl', 'ch')) is not None: return masked_read(i, ('ecx', 'cx', 'cl', 'ch')), {'ecx'}, False
    if m == 'push' and ops and ops[0].type == X86_OP_REG and i.reg_name(ops[0].reg) == 'ecx':
        return rd, wr, True                                             # `push ecx` also reserves 4 stack bytes
    for n, o in enumerate(ops):
        if o.type == X86_OP_MEM:
            for r in (o.mem.base, o.mem.index):
                if r and i.reg_name(r) in ECX_PARTS: rd.add(i.reg_name(r))
        elif o.type == X86_OP_REG and i.reg_name(o.reg) in ECX_PARTS:
            p = i.reg_name(o.reg)
            if n == 0 and (m in WRITE_ONLY or m.startswith('set')): wr.add(p)
            elif n == 0 and m in ('xchg',): rd.add(p); wr.add(p)
            else:
                rd.add(p)
                if n == 0 and m not in ('cmp', 'test', 'push'): wr.add(p)
    return rd, wr, unk


FIRST_READ = {}         # function -> the instruction that reads its entry ECX (the evidence)


def ecx_inputs(im, code, starts):
    """{function: True (uses its entry ECX on some path), False (never does), None (cannot tell: a `push ecx`,
    an indirect call or jump, or a callee that cannot tell, reached with ECX not wholly written)}; a fixpoint
    over direct calls and tail jumps"""
    res, pend = {}, {}
    import reg_preserve                     # (imports this module: not at the top)
    keeps = reg_preserve.preserved(im, code)
    for f, ins in code.items():
        if f not in ins: res[f] = None; continue
        wr = {f: frozenset()}; work = [f]; read = False; callees = set(); unknown = False; labels = S.labels_of(ins)
        order = sorted(ins)
        while work:
            a = work.pop(); i = ins[a]; w = wr[a]; o = i.operands
            rd, wp, unk = ecx_effect(i)
            if unk and i.mnemonic == 'push' and o and o[0].type == X86_OP_REG and r32(i.reg_name(o[0].reg)) == REG:
                # `push REG`: MSVC's stack-slot reservation or an argument. Only the exact case is decided here:
                # reg_preserve.slot_kind says "overwritten" when a 4-byte write to the slot comes straight after, before
                # anything could read it, so REG is not read. Everything else (its "use" is only "not shown to be
                # overwritten") stays undecided
                if reg_preserve.slot_kind(ins, order, a) == 'overwritten': rd, wp, unk = set(), set(), False
            # a direct call to a function that leaves the register untouched (reg_preserve.py) does not write it:
            # the entry value is still there afterwards (0x10044010 reads its ECX after `call 0x10043de0`)
            if i.mnemonic == 'call' and o and o[0].type == X86_OP_IMM and REG in keeps.get(o[0].imm, ()): wp = set()
            if any(not covered(p, w) for p in rd): read = True; FIRST_READ[f] = '0x%08X %s %s' % (a, i.mnemonic, i.op_str); break
            whole = REG in w
            if not whole:
                if unk: unknown = True
                if i.mnemonic in ('call', 'jmp') and o:
                    if o[0].type == X86_OP_IMM:
                        if o[0].imm in code and not (i.mnemonic == 'jmp' and o[0].imm in ins): callees.add(o[0].imm)
                    else: unknown = True                    # through a pointer: the callee may read it
            w2 = frozenset(w | wp)
            nx = succs(im, ins, a, i, labels)
            if nx is None: unknown = True; nx = []          # an indirect jump not bounded: its targets are unseen
            for n in nx:
                if n not in ins: continue
                if n not in wr: wr[n] = w2; work.append(n)
                else:
                    mm = wr[n] & w2
                    if mm != wr[n]: wr[n] = mm; work.append(n)
        if read: res[f] = True
        else: res[f] = False; pend[f] = (callees, unknown)
    changed = True
    while changed:
        changed = False
        for f, (callees, unknown) in pend.items():
            v = True if any(res.get(g) is True for g in callees) else \
                None if unknown or any(res.get(g, None) is None for g in callees) else False
            if v != res[f]: res[f] = v; changed = True
    return res


def vtables(code, img, base, starts):
    """{vtable: [slot functions]}: every constant stored to [reg+N] whose first dword is a function; a vtable
    runs while its dwords are functions, up to the next such constant"""
    u32 = lambda a: struct.unpack_from('<I', img, a - base)[0]
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
    return vt_slots


def analyse():
    """the whole-image analysis: {'im', 'code', 'starts', 'purges', 'flows' (per function, the state before each
    instruction: regs holding the entry ECX, regs holding its vtable, exact vtable stored), 'classes' (per
    function, the vtables its `this` can have, or TOP), 'vt_slots', 'reason'}"""
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

    vt_slots = vtables(code, img, base, starts); vts = sorted(vt_slots); vt_set = set(vt_slots)
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

    return {'im': im, 'code': code, 'starts': starts, 'purges': purges, 'flows': flows, 'classes': classes,
            'vt_slots': vt_slots, 'reason': reason}


def main(out):
    A = analyse()
    im, code, starts, purges, flows, classes, vt_slots = (A[k] for k in ('im', 'code', 'starts', 'purges', 'flows', 'classes', 'vt_slots'))
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
    # the convention, from the callees' own code: all read their entry ECX (this in ECX), or none does
    ecx = ecx_inputs(im, code, starts)
    fp = json.load(open(os.path.join(D, 'fpret.json')))
    for k, r in res.items():
        e = {ecx.get(int(t, 16)) for t in r['targets']}
        r['ecx'] = e.pop() if len(e) == 1 else None
        f = {fp.get('0x%x' % int(t, 16)) for t in r['targets']}
        r['ret'] = 'eax' if f == {'none'} else 'st0' if f == {'float'} else None
    json.dump({'0x%08X' % f: (FIRST_READ.get(f, 'through a callee') if v else False) for f, v in sorted(ecx.items()) if v is not None},
              open(os.path.join(os.path.dirname(os.path.abspath(out)), 'ecx_use.json'), 'w'), indent=0)
    res['_unsettled'] = dict(why); res['_vtables'] = len(vt_slots)
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print('%d vtables; %d virtual calls on `this` settled; unsettled: %s' % (len(vt_slots), len(res) - 2, dict(why)))


if __name__ == '__main__':
    main(sys.argv[1])
