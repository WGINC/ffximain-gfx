"""caller_reads.py -- which bytes of a function's EAX result its callers read, from the callers' code.

A function that ends with `mov al, 1` or `setne al` hands back in the rest of EAX whatever was there: what an
earlier instruction or a callee left. The C++ reconstruction leaves different bytes there. If no caller ever
reads them, no code can tell the two apart. The gate then compares only the bytes the callers read.

For each function, every place that reaches it is examined, and the function is listed only when all of them are
known:
  * a `call F`, or a `jmp F` that ends its caller (a tail call), is the only kind of reference. A function whose
    address appears anywhere else is left out: as an immediate, an absolute memory operand, or a dword anywhere in
    the image (a vtable, a table, a callback argument). Its callers are not all known.
  * From the return address, the caller's control flow is followed (vcall_resolve.succs) with the set of EAX
    bytes that still hold F's result. An instruction that reads any of them (capstone's register accesses,
    implicit ones included) adds them to the result. A write removes the bytes it writes; `xor`/`sub` of a
    register with itself is a write only. A `ret` passes the bytes still held to the caller's own callers (this
    fact for the caller, to a fixpoint).
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

    FFXI_DLL=... python3 caller_reads.py [--reg edx] OUT.json [ENTRY.json [INPUTS.json SITES.json]]

--reg edx: the same facts for EDX (a function returning EDX:EAX, or one that leaves EDX as its caller had it):
OUT says which bytes of the EDX it returns its callers read, INPUTS and SITES which functions take their entry
EDX as an input and where callers read it back. Not used as facts: EDX is written by few functions, so most of
what this finds "read" is a call through a pointer or a jump the walk does not follow, counted as reading
(14,269 functions; 21 Oct).
Output {"0xFUNC": 0 | 1 | 2}: no caller reads the result / only AL / only AX. A function whose callers read
all of EAX, or are not all known, is not listed.

ENTRY.json (data/eax_entry_unseen.json): the functions in which the value EAX holds at entry is seen by no code:
the function and its callees read no byte of it (the walk from the entry above), and the bytes it can hand back
unchanged at a `ret` are bytes no caller reads (this fact). {"0xFUNC": [bytes it can hand back unchanged]}. The
reconstruction may start such a function with any value in EAX.

INPUTS.json (data/eax_inputs.json): the functions in which the value EAX holds at entry IS seen, the opposite
case: {"0xFUNC": {"reads": [entry bytes the function or its callees read], "hands_back": [bytes it can return
unchanged that a caller reads; all it can return unchanged when its callers are not all known], "callers_known":
bool, "reads_seen": [the bytes of "reads" an instruction examines (COPIES, UPWARD, SHIFTS below), in the function or a
direct callee]}}. "reads" also counts code the walk does not follow as reading (a call through a pointer, a jump it
cannot bound, and where a copy of the bytes goes: `push eax` after `mov ax, ...` hands a short argument on with
two bytes no code is shown to look at): the safe side for what a reconstruction must pass, but no evidence of a
read; "reads_seen" is that evidence.
That value is an input of the function as much as a parameter is: its reconstruction takes it.

SITES.json (data/eax_kept_sites.json): the direct calls after which the caller reads a byte the callee may hand
back unchanged from its own entry EAX: {"0xCALL": {"caller", "callee", "bytes"}}. At such a site the caller's EAX
before the call is part of what it reads afterwards, so a translation must pass it.
"""
import json, os, struct, sys
from capstone.x86 import X86_OP_IMM, X86_OP_MEM
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import sp_solve as S
import vcall_resolve as V

BYTES = {'al': {0}, 'ah': {1}, 'ax': {0, 1}, 'eax': {0, 1, 2, 3}}
PARTS = {'eax': BYTES, 'edx': {'dl': {0}, 'dh': {1}, 'dx': {0, 1}, 'edx': {0, 1, 2, 3}}}
ALL = frozenset({0, 1, 2, 3})
# reads_seen: the instructions that examine the bytes they read. A copy moves them without looking at them; an
# "upward" operation (Intel SDM: add, adc, sub, sbb, inc, dec, neg, not, and, or, xor, shl, lea) makes each result
# byte from the operand bytes at or below it, so a byte reaches only the same or higher bytes of its destination:
# neither shows the byte to matter, and where the result goes is followed no further (in EAX itself: still held).
# A shift or rotate down brings upper bits into lower ones, but what it brings is often masked off next (MSVC's
# bitfield read: `mov al, [x]; shr eax, 4; and eax, 7`): whether a bit survives is a question of bits, not followed
# here, so it shows no read either. What does: comparing or testing (cmp, test, bt), multiplying or dividing, and
# using the bytes as an address (a memory operand's base or index).
COPIES = ('mov', 'movzx', 'movsx', 'push', 'pushal', 'xchg', 'stosb', 'stosw', 'stosd')
UPWARD = ('add', 'adc', 'sub', 'sbb', 'inc', 'dec', 'neg', 'not', 'and', 'or', 'xor', 'shl', 'sal', 'lea')
SHIFTS = ('shr', 'sar', 'ror', 'rol', 'rcr', 'rcl', 'shrd', 'shld')


def examined(i, r):
    """the bytes of r (EAX bytes the instruction reads) that it examines (above)"""
    addr = set()
    for o in i.operands:
        if o.type == X86_OP_MEM:
            for x in (o.mem.base, o.mem.index):
                if x and i.reg_name(x) in BYTES: addr |= BYTES[i.reg_name(x)]
    m = i.mnemonic.split()[-1]                                     # `rep stosd`: the operation is stosd
    if m == 'lea': return set()                                    # an address computed, never used as one
    if m in COPIES or m in UPWARD or m in SHIFTS: return r & addr
    return r


def access(i):
    """(EAX bytes read, EAX bytes written) by one instruction"""
    try: rd, wr = i.regs_access()
    except Exception: return set(ALL), set()
    rd = {i.reg_name(r) for r in rd}; wr = {i.reg_name(r) for r in wr}
    o = i.operands
    # `xor`/`sub r, r` give 0 and `sbb r, r` gives -CF whatever r held (Intel SDM, SBB: dest - (src + CF)): writes only
    if i.mnemonic in ('xor', 'sub', 'sbb') and len(o) == 2 and o[0].type == o[1].type == 1 and o[0].reg == o[1].reg:
        rd = set()
    # `or r, -1` / `and r, 0`: every bit of r is set or cleared whatever it held (Intel SDM, OR / AND): a write only
    if i.mnemonic in ('or', 'and') and len(o) == 2 and o[0].type == 1 and o[1].type == 2:
        full = (1 << (8 * o[0].size)) - 1
        if (o[1].imm & full) == (full if i.mnemonic == 'or' else 0): rd = {x for x in rd if x != i.reg_name(o[0].reg)}
    r = set().union(*[BYTES[x] for x in rd if x in BYTES]); w = set().union(*[BYTES[x] for x in wr if x in BYTES])
    # `and`/`test r, imm`: a byte of r whose immediate byte is 0 comes out 0 whatever it held, so only the bytes the
    # immediate keeps are read (Intel SDM, AND, TEST; flags are computed from the result)
    if i.mnemonic in ('and', 'test') and len(o) == 2 and o[0].type == 1 and o[1].type == 2 and i.reg_name(o[0].reg) in BYTES:
        bs = sorted(BYTES[i.reg_name(o[0].reg)])
        r -= {b for k, b in enumerate(bs) if not (o[1].imm >> (8 * k)) & 0xFF}
    return r, w


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
        return frozenset() if BYTES is PARTS['eax'] and (IR.get(iat[slot]) or {}).get('size') == 4 else lv
    eread = {f: frozenset() for f in starts}; keep = {f: frozenset() for f in starts}   # grow to a fixpoint
    VCALLS = {k: v for k, v in json.load(open(os.path.join(P, '..', 'data', 'vcalls.json'))).items() if not k.startswith('_')}
    edef = {f: frozenset() for f in starts}                         # entry bytes an instruction is seen to read

    def walk(f, a, live, at_ret, seen_only=False):
        """(bytes of `live` read on the paths from a in f, bytes still held at a `ret`); at_ret(f, held)
        gives the bytes a `ret` counts as read. seen_only: count only reads an instruction makes (here or in a
        direct callee, edef): code the walk does not follow (a call through a pointer, a jump it cannot bound or
        out of the function) adds nothing"""
        ins = code[f]
        rd = edef if seen_only else eread
        def unk(lv): return frozenset() if seen_only else lv
        if f not in labels: labels[f] = S.labels_of(ins)
        got, kept, seen, work = set(), set(), set(), [(a, frozenset(live))]
        while work:
            x, lv = work.pop()
            if (x, lv) in seen or not lv: continue
            seen.add((x, lv))
            i = ins.get(x)
            if i is None: got |= unk(lv); continue
            m, o = i.mnemonic, i.operands
            if m == 'call':
                t = o[0]
                if t.type == X86_OP_MEM and not t.mem.base and not t.mem.index and (t.mem.disp & 0xFFFFFFFF) in iat:
                    lv = after_import(t.mem.disp & 0xFFFFFFFF, lv)
                elif t.type == X86_OP_IMM and t.imm in starts:
                    got |= lv & rd[t.imm]; lv = lv & keep[t.imm]
                elif t.type == X86_OP_IMM and import_thunk(t.imm) is not None: lv = after_import(import_thunk(t.imm), lv)
                elif '0x%08X' % x in VCALLS and all(int(v, 16) in starts for v in VCALLS['0x%08X' % x]['targets']):
                    # a virtual call vcall_resolve.py settles: one of its targets runs. May-reads: what any of
                    # them reads; reads seen: what every one of them is seen to read; kept: what any can keep
                    ts = [int(v, 16) for v in VCALLS['0x%08X' % x]['targets']]
                    got |= lv & (frozenset.intersection(*[edef[v] for v in ts]) if seen_only else frozenset().union(*[eread[v] for v in ts]))
                    lv = lv & frozenset().union(*[keep[v] for v in ts])
                else: got |= unk(lv); continue
                work.append((x + i.size, lv)); continue
            r, w = access(i)
            if seen_only:
                if m in UPWARD and o and o[0].type == 1 and i.reg_name(o[0].reg) in BYTES:
                    w = w - r                                      # in place: a byte read and written is still held, changed
                r = examined(i, r)
            got |= lv & r; lv = lv - w
            if not lv: continue
            if m.startswith('ret'):
                kept |= lv; got |= at_ret(f, lv); continue
            slot = import_jmp(i)
            if slot is None and m == 'jmp' and o and o[0].type == X86_OP_IMM and o[0].imm not in ins and o[0].imm not in starts:
                slot = import_thunk(o[0].imm)
            if slot is not None:                                   # a tail call to an import
                lv = after_import(slot, lv); kept |= lv; got |= at_ret(f, lv); continue
            if m == 'jmp' and o and o[0].type == X86_OP_IMM and o[0].imm not in ins:
                t = o[0].imm
                if t in starts:                                    # a tail call: G's reads, then G's return is f's
                    got |= lv & rd[t]; lv = lv & keep[t]
                    kept |= lv; got |= at_ret(f, lv)
                else: got |= unk(lv)
                continue
            nx = V.succs(im, ins, x, i, labels[f])
            if nx is None: got |= unk(lv); continue
            for n in nx:
                if n in ins: work.append((n, lv))
                else: got |= unk(lv)
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
    a = sys.argv[1:]
    if a[:1] == ['--reg']:
        BYTES = PARTS[a[1]]; a = a[2:]
    main(*a)
