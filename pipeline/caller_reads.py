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
    stdcall or cdecl); every byte may survive it, since a void import's EAX is not declared.
  * A call through a pointer, an indirect jump the code does not bound, or a jump out of the function counts as
    reading every byte still held: what it reads is not followed.

    FFXI_DLL=... python3 caller_reads.py OUT.json
Output {"0xFUNC": 0 | 1 | 2}: no caller reads the result / only AL / only AX. A function whose callers read
all of EAX, or are not all known, is not listed."""
import json, os, struct, sys
from capstone.x86 import X86_OP_IMM, X86_OP_MEM
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import sp_solve as S
import vcall_resolve as V

BYTES = {'al': {0}, 'ah': {1}, 'ax': {0, 1}, 'eax': {0, 1, 2, 3}}
ALL = frozenset({0, 1, 2, 3})


def access(i):
    """(EAX bytes read, EAX bytes written) by one instruction"""
    try: rd, wr = i.regs_access()
    except Exception: return set(ALL), set()
    rd = {i.reg_name(r) for r in rd}; wr = {i.reg_name(r) for r in wr}
    o = i.operands
    if i.mnemonic in ('xor', 'sub') and len(o) == 2 and o[0].type == o[1].type == 1 and o[0].reg == o[1].reg:
        rd = set()
    r = set().union(*[BYTES[x] for x in rd if x in BYTES]); w = set().union(*[BYTES[x] for x in wr if x in BYTES])
    return r, w


def main(out):
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
    eread = {f: frozenset() for f in starts}; keep = {f: frozenset() for f in starts}   # grow to a fixpoint

    def walk(f, a, live, at_ret):
        """(bytes of `live` read on the paths from a in f, bytes still held at a `ret`); at_ret(f, held)
        gives the bytes a `ret` counts as read"""
        ins = code[f]
        if f not in labels: labels[f] = S.labels_of(ins)
        got, kept, seen, work = set(), set(), set(), [(a, frozenset(live))]
        while work:
            x, lv = work.pop()
            if (x, lv) in seen or not lv: continue
            seen.add((x, lv))
            i = ins.get(x)
            if i is None: got |= lv; continue
            m, o = i.mnemonic, i.operands
            if m == 'call':
                t = o[0]
                if t.type == X86_OP_MEM and not t.mem.base and not t.mem.index and (t.mem.disp & 0xFFFFFFFF) in iat: pass
                elif t.type == X86_OP_IMM and t.imm in starts:
                    got |= lv & eread[t.imm]; lv = lv & keep[t.imm]
                else: got |= lv; continue
                work.append((x + i.size, lv)); continue
            r, w = access(i)
            got |= lv & r; lv = lv - w
            if not lv: continue
            if m.startswith('ret'):
                kept |= lv; got |= at_ret(f, lv); continue
            if m == 'jmp' and o and o[0].type == X86_OP_IMM and o[0].imm not in ins:
                t = o[0].imm
                if t in starts:                                    # a tail call: G's reads, then G's return is f's
                    got |= lv & eread[t]; lv = lv & keep[t]
                    kept |= lv; got |= at_ret(f, lv)
                else: got |= lv
                continue
            nx = V.succs(im, ins, x, i, labels[f])
            if nx is None: got |= lv; continue
            for n in nx:
                if n in ins: work.append((n, lv))
                else: got |= lv
        return frozenset(got), frozenset(kept)

    while True:                                                    # entry reads and kept bytes of every function
        changed = 0
        for f in starts:
            g, k = walk(f, f, ALL, lambda f_, held: set())
            if g != eread[f] or k != keep[f]: eread[f] = eread[f] | g; keep[f] = keep[f] | k; changed += 1
        print('entry/keep round: %d changed' % changed)
        if not changed: break

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
    print('%d functions with all callers known; result never read: %d, only AL: %d, only AX: %d' % (
        len(known), sum(v == 0 for v in res.values()), sum(v == 1 for v in res.values()), sum(v == 2 for v in res.values())))


if __name__ == '__main__':
    main(*sys.argv[1:])
