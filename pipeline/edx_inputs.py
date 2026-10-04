"""edx_inputs.py -- which functions read their entry EDX, from the code: vcall_resolve's ECX analysis run for EDX.

A caller that hands a callee whatever EDX happens to hold (Hex-Rays: a never-assigned `// edx` variable passed
as a __fastcall's second argument) is exact only if the callee never reads its entry EDX. Same fixpoint as
vcall_resolve.ecx_inputs; the EDX-specific effects (Intel SDM): `cdq` and one-operand `mul`/`imul` write EDX,
`div`/`idiv` read it, a call clobbers it, `push edx` may save it for later (unknown).

    python3 edx_inputs.py data/edx_use.json        uses $FFXI_DLL
Output: {"0xFUNC": true (reads it, with "first_read") | false (never) | null (cannot tell)}
"""
import json, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import vcall_resolve as V
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG

PARTS = {'edx', 'dx', 'dl', 'dh'}
COVERS = {'edx': {'edx', 'dx', 'dl', 'dh'}, 'dx': {'dx', 'dl', 'dh'}, 'dl': {'dl'}, 'dh': {'dh'}}


def covered(part, written):
    have = set().union(*(COVERS[w] for w in written)) if written else set()
    if {'dl', 'dh'} <= have: have.add('dx')
    return part in have and (part != 'edx' or 'edx' in written)


def edx_effect(i):
    """(parts of EDX the instruction reads, parts it writes, may-read-unknown)"""
    m = i.mnemonic; ops = i.operands; rd, wr = set(), set()
    if m in ('pushal', 'pushad'): return rd, wr, True
    if m == 'call' or m == 'cdq': return rd, {'edx'}, False
    if m in ('mul', 'imul') and len(ops) == 1: return rd, {'edx'}, False
    if m in ('div', 'idiv'): rd.add('edx')
    if m in ('xor', 'sub') and len(ops) == 2 and ops[0].type == X86_OP_REG and ops[1].type == X86_OP_REG \
            and ops[0].reg == ops[1].reg and i.reg_name(ops[0].reg) in PARTS:
        return rd, {i.reg_name(ops[0].reg)}, False
    if m == 'push' and ops and ops[0].type == X86_OP_REG and i.reg_name(ops[0].reg) == 'edx': return rd, wr, True
    for n, o in enumerate(ops):
        if o.type == X86_OP_MEM:
            for r in (o.mem.base, o.mem.index):
                if r and i.reg_name(r) in PARTS: rd.add(i.reg_name(r))
        elif o.type == X86_OP_REG and i.reg_name(o.reg) in PARTS:
            p = i.reg_name(o.reg)
            if n == 0 and (m in V.WRITE_ONLY or m.startswith('set')): wr.add(p)
            elif n == 0 and m == 'xchg': rd.add(p); wr.add(p)
            else:
                rd.add(p)
                if n == 0 and m not in ('cmp', 'test', 'push'): wr.add(p)
    return rd, wr, False


def main(out):
    import sp_solve as S
    im = S.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(V.D, 'func_extents.json')))
    starts = {int(k, 16) for k in ext}
    code = {f: im.code(e) for f, e in ((int(k, 16), v) for k, v in ext.items())}
    V.ecx_effect, V.covered, V.FIRST_READ, V.REG = edx_effect, covered, {}, 'edx'      # the same fixpoint, for EDX
    res = V.ecx_inputs(im, code, starts)
    json.dump({'0x%08X' % f: (V.FIRST_READ.get(f) or True) if v else v for f, v in sorted(res.items())}, open(out, 'w'), indent=0)
    print('%d functions: %d read their entry EDX, %d never, %d cannot tell' % (
        len(res), sum(v is True for v in res.values()), sum(v is False for v in res.values()), sum(v is None for v in res.values())))


if __name__ == '__main__':
    main(sys.argv[1])
