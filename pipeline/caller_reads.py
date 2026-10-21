"""caller_reads.py -- which bytes of a function's EAX result its callers read, from the callers' code.

A function that ends with `mov al, 1` or `setne al` hands back in the rest of EAX whatever was there: what an
earlier instruction or a callee left. The C++ reconstruction leaves different bytes there. If no caller ever
reads them, no code can tell the two apart. The gate then compares only the bytes the callers read.

For each function, every place that reaches it is examined, and the function is listed only when all of them are
known:
  * a `call F`, or a `jmp F` that ends its caller (a tail call), is the only kind of reference. A function whose
    address appears anywhere else is left out: as an immediate, an absolute memory operand, or a dword anywhere in
    the image (a vtable, a table, a callback argument). Its callers are not all known.
  * From the return address, the caller's control flow is followed (vcall_resolve.succs) with F's result in EAX,
    bit by bit through the registers (byte_flow.py: copies, arithmetic, masks and shifts as the Intel SDM defines
    them; joins are unions). A byte counts as read where data made from it is examined (compared, tested, used as
    an address, multiplied, or reaches a flag read) or escapes where the walk does not follow it (stored to
    memory, pushed, an instruction byte_flow does not model, a register a callee may read). A `ret` passes what
    EAX still holds to the caller's own callers (this fact for the caller, to a fixpoint).
  * A direct call to G reads the bytes G reads of its entry EAX, and afterwards the bytes G can return unchanged
    still hold F's result. Both come from the same walk over G from its entry, through G's own calls, as a
    fixpoint growing from nothing (a may-analysis: every byte some path reads or keeps is counted).
  * A call to an import reads no EAX byte (the Windows SDK's declared prototypes: EAX is no parameter of
    stdcall or cdecl); every byte may survive it, since a void import's EAX is not declared, except where the SDK
    declares a 4-byte result (data/import_returns.json): that call writes all of EAX. So does a call to,
    or a jump to, a thunk that is one `jmp dword ptr [import slot]`, and such a jump itself (a tail call to the
    import: the bytes still held reach the function's own `ret`).
  * A virtual call vcall_resolve.py settles (data/vcalls.json) reads what any of its targets reads and keeps what
    any of them keeps (its reads seen: what every target is seen to read).
  * Any other call through a pointer, an indirect jump the code does not bound, or a jump out of the function
    counts as reading every byte still held: what it reads is not followed.

    FFXI_DLL=... python3 caller_reads.py OUT.json [ENTRY.json [INPUTS.json SITES.json]]

Output {"0xFUNC": 0 | 1 | 2}: no caller reads the result / only AL / only AX. A function whose callers read
all of EAX, or are not all known, is not listed.

ENTRY.json (data/eax_entry_unseen.json): the functions in which the value EAX holds at entry is seen by no code:
the function and its callees read no byte of it (the walk from the entry above), and the bytes it can hand back
unchanged at a `ret` are bytes no caller reads (this fact). {"0xFUNC": [bytes it can hand back unchanged]}. The
reconstruction may start such a function with any value in EAX.

INPUTS.json (data/eax_inputs.json): the functions in which the value EAX holds at entry IS seen, the opposite
case: {"0xFUNC": {"reads": [entry bytes the function or its callees may read], "reads_seen": [those an instruction
is shown to examine], "hands_back": [bytes it may return unchanged that a caller reads; all it may return unchanged
when its callers are not all known], "callers_known": bool}}. "reads" counts every escape as a read (byte_flow.py),
and a value computed from entry EAX and handed back at a `ret`: the safe side for what a reconstruction must pass.
"reads_seen" is evidence of a read: the walk counts only examinations, an and/or with a register of unknown value
keeps no dependence, and code it does not follow adds nothing. That value is an input of the function as much as a
parameter is: its reconstruction takes it.

SITES.json (data/eax_kept_sites.json): the direct calls after which the caller reads a byte the callee may hand
back unchanged from its own entry EAX: {"0xCALL": {"caller", "callee", "bytes"}}. At such a site the caller's EAX
before the call is part of what it reads afterwards, so a translation must pass it.
"""
import json, os, struct, sys
from capstone.x86 import X86_OP_IMM, X86_OP_MEM
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import sp_solve as S
import vcall_resolve as V
import byte_flow as BF

ALL = frozenset({0, 1, 2, 3})
WHY = None       # caller_reads.WHY = {} before main(): {func: {instruction: sources}} where a walk from f's entry counts bytes as read unfollowed


def main(out, entry_out=None, inputs_out=None, sites_out=None):
    im = S.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(P, '..', 'data', 'func_extents.json')))
    iat = S.imports(im, os.environ['FFXI_DLL'])
    code = {int(k, 16): im.code(e) for k, e in ext.items()}
    owner = {a: f for f, ins in code.items() for a in ins}
    starts = set(code)
    sites, taken = {}, set()
    for f, ins in code.items():
        for a, i in ins.items():
            for n, o in enumerate(i.operands):
                v = o.imm & 0xFFFFFFFF if o.type == X86_OP_IMM else \
                    o.mem.disp & 0xFFFFFFFF if o.type == X86_OP_MEM and not o.mem.base and not o.mem.index else None
                if v not in starts: continue
                if i.mnemonic == 'call' and o.type == X86_OP_IMM: sites.setdefault(v, []).append((f, a, 'call'))
                elif i.mnemonic == 'jmp' and o.type == X86_OP_IMM and v != f and v not in ins: sites.setdefault(v, []).append((f, a, 'jmp'))
                elif not (i.mnemonic.startswith('j') and o.type == X86_OP_IMM and v in ins): taken.add(v)
    img = im.img
    for v in starts:
        if img.find(struct.pack('<I', v)) != -1: taken.add(v)
    labels = {}
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True
    _thunk = {}
    IR = json.load(open(os.path.join(P, '..', 'data', 'import_returns.json')))
    def import_jmp(i):
        # the import slot a `jmp dword ptr [slot]` goes through, or None
        o = i.operands
        if i.mnemonic == 'jmp' and len(o) == 1 and o[0].type == X86_OP_MEM and not o[0].mem.base \
                and not o[0].mem.index and (o[0].mem.disp & 0xFFFFFFFF) in iat: return o[0].mem.disp & 0xFFFFFFFF
        return None
    def import_thunk(t):
        if t not in _thunk:
            try: _thunk[t] = import_jmp(next(md.disasm(bytes(img[t - im.base:t - im.base + 8]), t)))
            except StopIteration: _thunk[t] = None
        return _thunk[t]
    def after_import(slot, lv):
        # the bytes still held after a call of the import in `slot`: none where the SDK declares a 4-byte EAX result
        return frozenset() if (IR.get(iat[slot]) or {}).get('size') == 4 else lv
    eread = {f: frozenset() for f in starts}; keep = {f: frozenset() for f in starts}   # grow to a fixpoint
    VCALLS = {k: v for k, v in json.load(open(os.path.join(P, '..', 'data', 'vcalls.json'))).items() if not k.startswith('_')}
    _dj = lambda n: json.load(open(os.path.join(P, '..', 'data', n)))
    ECX_USE, EDX_USE = _dj('ecx_use.json'), _dj('edx_use.json')                 # a callee's reads of ECX / EDX
    RKEEPS = {int(k, 16): set(v) for k, v in _dj('reg_keeps.json').items()}       # registers it never writes
    RSAVED = {int(k, 16): set(v) for k, v in _dj('reg_saved.json').items()}       # callee-saved ones it restores
    EREGS = {int(k, 16): set(v) for k, v in _dj('entry_regs.json').items()}       # registers it reads at entry
    edef = {f: frozenset() for f in starts}                         # entry bytes an instruction is seen to read

    def walk(f, a, live, at_ret, seen_only=False):
        """(source bytes read on the paths from a in f, EAX bytes still held unchanged at a `ret`), following the
        value's bits through the registers (byte_flow.py). `live`: the EAX bytes holding the value at a, unchanged.
        at_ret(f, positions) gives the EAX byte positions a `ret` counts as read (f's callers). seen_only: count only
        the reads an instruction is seen to make (byte_flow's examined; a direct callee's edef); otherwise also
        every place the value escapes or goes where the walk does not follow (may-reads)."""
        ins = code[f]
        rd = edef if seen_only else eread
        entry = (a == f)
        if f not in labels: labels[f] = S.labels_of(ins)
        got, kept = set(), set()
        state_at, work = {a: BF.entry_state('eax', tuple(sorted(live)))}, [a]
        def unk(st):
            srcs = set().union(*[st.srcs(r) for r in st.regs]) if st.regs else set()
            if not seen_only and srcs and WHY is not None and entry: WHY.setdefault(f, {})['0x%08X' % x] = sorted(srcs)
            return frozenset() if seen_only else frozenset(srcs)
        def eax_srcs(st, positions):
            mask = 0
            for k in positions: mask |= 0xFF << (8 * k)
            return st.srcs('eax', mask)
        def held(st):
            """EAX byte positions that may depend on the value, and those that may hold its byte k unchanged"""
            d, p = st.d('eax'), st.p('eax')
            dep = {k for k in range(4) if any((x >> (8 * k)) & 0xFF for x in d)}
            same = {k for k in range(4) if (p[k] >> (8 * k)) & 0xFF}
            return dep, same
        def ret_here(st, keep_eax=True):
            dep, same = held(st)
            r = at_ret(f, dep)
            out = set(eax_srcs(st, r))
            if not seen_only:
                if entry: out |= {s_ for s_ in range(4) if st.c('eax')[s_]}   # a value computed from it handed back: an input
                for reg in st.regs:                              # left in another register at the return
                    if reg != 'eax': out |= st.srcs(reg)
            return out, (same if keep_eax else set())
        def after_callee(st, t, kp, imp=False):
            """the state after a call of t (None: an import, imp, or a settled virtual call), which keeps EAX bytes
            kp; and the sources the call reads. An import keeps EBX, ESI, EDI, EBP (Microsoft's x86 conventions)
            and reads no register (its arguments are on the stack)"""
            r = set()
            regs = {}
            for reg, v in st.regs.items():
                if reg == 'eax':
                    mask = 0
                    for k in kp: mask |= 0xFF << (8 * k)
                    nd = tuple(x & mask for x in v[0]); np_ = tuple(x & mask for x in v[1])
                    if any(nd): regs['eax'] = (nd, np_)
                elif reg in ('ecx', 'edx'):
                    if imp:
                        if not seen_only: r |= st.srcs(reg)      # volatile across the call: may be kept, not followed
                        continue
                    use = (ECX_USE if reg == 'ecx' else EDX_USE).get('0x%08X' % t) if t is not None else None
                    if isinstance(use, str): r |= st.srcs(reg)          # the callee reads it: seen
                    elif use is not False and not seen_only: r |= st.srcs(reg)
                    if t is not None and reg in RKEEPS.get(t, ()): regs[reg] = v
                elif imp: regs[reg] = v
                else:
                    if t is not None and reg in RSAVED.get(t, ()):
                        regs[reg] = v
                        if reg in EREGS.get(t, ()) and not seen_only: r |= st.srcs(reg)
                    elif not seen_only: r |= st.srcs(reg)
            return BF.State(regs), r
        while work:
            x = work.pop(); st = state_at[x]
            i = ins.get(x)
            if i is None: got |= unk(st); continue
            m, o = i.mnemonic, i.operands
            nst = None
            if m == 'call':
                t = o[0]
                # the bits used to reach the callee (call [eax+8]): an address, examined
                if t.type == X86_OP_MEM:
                    for r_, _ in BF.mem_regs(i): got |= st.srcs(r_)
                elif t.type == 1 and i.reg_name(t.reg) in BF.PART: got |= st.srcs(BF.PART[i.reg_name(t.reg)][0])
                if t.type == X86_OP_MEM and not t.mem.base and not t.mem.index and (t.mem.disp & 0xFFFFFFFF) in iat:
                    kp = after_import(t.mem.disp & 0xFFFFFFFF, ALL); nst, r = after_callee(st, None, kp, imp=True)
                    if not seen_only: got |= r
                elif t.type == X86_OP_IMM and t.imm in starts:
                    got |= eax_srcs(st, rd[t.imm]); nst, r = after_callee(st, t.imm, keep[t.imm]); got |= r
                elif t.type == X86_OP_IMM and import_thunk(t.imm) is not None:
                    kp = after_import(import_thunk(t.imm), ALL); nst, r = after_callee(st, None, kp, imp=True)
                    if not seen_only: got |= r
                elif '0x%08X' % x in VCALLS and all(int(v, 16) in starts for v in VCALLS['0x%08X' % x]['targets']):
                    # a virtual call vcall_resolve.py settles: one of its targets runs. May-reads: what any of
                    # them reads; reads seen: what every one of them is seen to read; kept: what any can keep
                    ts = [int(v, 16) for v in VCALLS['0x%08X' % x]['targets']]
                    pos = frozenset.intersection(*[edef[v] for v in ts]) if seen_only else frozenset().union(*[eread[v] for v in ts])
                    got |= eax_srcs(st, pos)
                    nst, r = after_callee(st, None, frozenset().union(*[keep[v] for v in ts]))
                    if not seen_only: got |= r
                else: got |= unk(st); continue
                nx = [x + i.size]
            else:
                if m.startswith('ret'):
                    g, k = ret_here(st); got |= g; kept |= k; continue
                slot = import_jmp(i)
                if slot is None and m == 'jmp' and o and o[0].type == X86_OP_IMM and o[0].imm not in ins and o[0].imm not in starts:
                    slot = import_thunk(o[0].imm)
                if slot is not None:                               # a tail call to an import
                    s2, r = after_callee(st, None, after_import(slot, ALL), imp=True)
                    if not seen_only: got |= r
                    g, k = ret_here(s2); got |= g; kept |= k; continue
                if m == 'jmp' and o and o[0].type == X86_OP_IMM and o[0].imm not in ins:
                    t = o[0].imm
                    if t in starts:                                # a tail call: G's reads, then G's return is f's
                        got |= eax_srcs(st, rd[t]); s2, r = after_callee(st, t, keep[t]); got |= r
                        g, k = ret_here(s2); got |= g; kept |= k
                    else: got |= unk(st)
                    continue
                if m.startswith('j') or m.startswith('loop'):
                    try: reads_flags = 'eflags' in {i.reg_name(r_) for r_ in i.regs_access()[0]}
                    except Exception: reads_flags = True
                    ex = set(st.flags) if reads_flags else set()       # a conditional jump on flags made from the value
                    if m in ('jecxz', 'jcxz') or m.startswith('loop'): ex |= st.srcs('ecx')
                    got |= ex
                    if o and o[0].type != X86_OP_IMM:              # an indirect jump: its target formed from the value
                        for r_, _ in BF.mem_regs(i): got |= st.srcs(r_)
                        if o[0].type == 1 and i.reg_name(o[0].reg) in BF.PART: got |= st.srcs(BF.PART[i.reg_name(o[0].reg)][0])
                    nst = st
                else:
                    nst, ex, es = BF.step(i, st, strict=seen_only)
                    got |= ex if seen_only else es
                nx = V.succs(im, ins, x, i, labels[f])
                if nx is None: got |= unk(nst); continue
            if not nst.any() and not nst.flags: continue
            for n in nx:
                if n not in ins: got |= unk(nst); continue
                old = state_at.get(n)
                newst = nst if old is None else BF.join(old, nst)
                if old is None or newst.key() != old.key(): state_at[n] = newst; work.append(n)
        return frozenset(got), frozenset(kept)

    while True:                                                    # entry reads and kept bytes of every function
        changed = 0
        for f in starts:
            g, k = walk(f, f, ALL, lambda f_, held: set())
            if g != eread[f] or k != keep[f]: eread[f] = eread[f] | g; keep[f] = keep[f] | k; changed += 1
        print('entry/keep round: %d changed' % changed)
        if not changed: break
    while True:                                                    # the reads seen, over the final keep
        changed = 0
        for f in starts:
            g = walk(f, f, ALL, lambda f_, held: set(), seen_only=True)[0]
            if not g <= edef[f]: edef[f] = edef[f] | g; changed += 1
        print('seen-read round: %d changed' % changed)
        if not changed: break
    assert all(edef[f] <= eread[f] for f in starts), 'a read seen that the may-walk does not count'

    reads = {f: frozenset() for f in sites if f not in taken}         # grows to a fixpoint
    known = set(reads)
    for f in starts:
        if f not in known: reads[f] = ALL
    at_ret = lambda f, held: held & reads.get(f, ALL)
    while True:
        changed = 0
        for f in known:
            r = set()
            for c, a, kind in sites[f]:
                if kind == 'call': r |= walk(c, a + code[c][a].size, ALL, at_ret)[0]
                else: r |= reads.get(c, ALL)
                if r == ALL: break
            r = frozenset(r)
            if r != reads[f]: reads[f] = r; changed += 1
        if not changed: break
    res = {}
    for f in sorted(known):
        r = reads[f]
        if not r: res['0x%08X' % f] = 0
        elif r == {0}: res['0x%08X' % f] = 1
        elif r <= {0, 1}: res['0x%08X' % f] = 2
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    if entry_out:
        unseen = {'0x%08X' % f: sorted(keep[f]) for f in sorted(starts)
                  if not eread[f] and f in known and not (keep[f] & reads[f])}
        json.dump(unseen, open(entry_out, 'w'), indent=0, sort_keys=True)
        print('%d functions whose entry EAX no code sees' % len(unseen))
    if inputs_out:
        inp = {}
        for f in sorted(starts):
            hb = keep[f] & (reads[f] if f in known else ALL)
            if eread[f] or hb:
                inp['0x%08X' % f] = {'reads': sorted(eread[f]), 'reads_seen': sorted(edef[f]), 'hands_back': sorted(hb),
                                     'callers_known': f in known}
        json.dump(inp, open(inputs_out, 'w'), indent=0, sort_keys=True)
        st = {}
        for f in sorted(starts):
            if not keep[f]: continue
            for c, a, kind in sites.get(f, []):
                if kind != 'call': continue
                b = walk(c, a + code[c][a].size, ALL, at_ret)[0] & keep[f]
                if b: st['0x%08X' % a] = {'caller': '0x%08X' % c, 'callee': '0x%08X' % f, 'bytes': sorted(b)}
        json.dump(st, open(sites_out, 'w'), indent=0, sort_keys=True)
        print('%d functions take their entry EAX as an input; %d call sites read bytes handed back from it' % (len(inp), len(st)))
    print('%d functions with all callers known; result never read: %d, only AL: %d, only AX: %d' % (
        len(known), sum(v == 0 for v in res.values()), sum(v == 1 for v in res.values()), sum(v == 2 for v in res.values())))


if __name__ == '__main__':
    main(*sys.argv[1:])
