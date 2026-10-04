"""ida_sp_check.py -- where IDA's stack pointer is wrong, and the one correction that explains it.

Hex-Rays names every stack slot from IDA's SP; where IDA's SP is off, the names after that point are off by
the same amount (one slot with two names, a local read at the return address: `retaddr`). The exact depth
at each block head comes from sp_solve.head_depths (the caller's stack balance with every call effect from a
definitive source; after `and esp, -N` counted from the aligned point, as IDA counts). IDA's SP comes from
the export (`spd`: its change points; between them the SP holds). Walking the function in address order,
each point where the two first part is explained by the instruction in front of it:
  * a call whose pop is known (the callee's `ret N`, a dispatch thunk's default, sp_solve, vcall_resolve,
    com_calls): IDA's change there is wrong;
  * an unconditional `jmp`/`ret`: the block after it is reached only by jumps, and IDA carried the SP over
    from the jump in address order instead of taking it from the jump sources.
The correction is applied (shifting IDA's SP after it) and the walk goes on; any difference no such
instruction explains is reported, not corrected.

    python3 ida_sp_check.py EXPORT.jsonl OUT.json        uses $FFXI_DLL
OUT.json: {"0xFUNC": {"fixes": [{"at", "change", "why"}], "unexplained": [...]}} for functions IDA has wrong.
"change" is the SP change IDA should have at "at" (positive: the stack shrinks), as ida/ffxi_ida_fix.py sets it.
"""
import bisect, json, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sp_solve as S
from capstone.x86 import X86_OP_IMM

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')


def main(exp, out):
    dll = os.environ['FFXI_DLL']; im = S.Image(dll)
    ext = json.load(open(os.path.join(D, 'func_extents.json'))); purges = json.load(open(os.path.join(D, 'purges.json')))
    known_srcs = [json.load(open(os.path.join(D, f))) for f in ('sp_solved.json', 'vcalls.json', 'com_calls.json')]
    starts = {int(k, 16) for k in ext}; iat = S.imports(im, dll); nr = S.noreturn_funcs(im, ext, iat)

    def pop(i):
        o = i.operands[0]
        if o.type == X86_OP_IMM: return purges.get('0x%08X' % o.imm), "the callee's own ret N"
        for name, src in zip(('sp_solve', 'vcall_resolve', 'com_calls'), known_srcs):
            if '0x%08X' % i.address in src: return src['0x%08X' % i.address]['pops'], name
        return None, None

    res = {}; checked = 0
    for l in open(exp):
        r = json.loads(l); f = r['ea']; e = ext.get('0x%x' % f)
        if not e or not r.get('spd'): continue
        pts = sorted((a, v) for a, v in r['spd']); keys = [a for a, _ in pts]
        ida = lambda a: -pts[bisect.bisect_right(keys, a) - 1][1] if bisect.bisect_right(keys, a) else 0
        hd = S.head_depths(im, f, e, purges, starts, nr, iat)
        if not hd: continue
        checked += 1
        ins = im.code(e); order = sorted(ins); adj = 0; fixes = []; bad = []
        for k, a in enumerate(order):
            if a not in hd: continue
            cur = ida(a) + adj
            if cur == hd[a]: continue
            p = ins[order[k - 1]] if k else None
            if p is not None and (p.mnemonic == 'jmp' or p.mnemonic.startswith('ret')):
                fixes.append({'at': '0x%08X' % p.address, 'change': (ida(p.address) - ida(a)) + (cur - hd[a]),
                              'why': 'the block at 0x%08X is reached only by jumps, at depth %d (IDA carried %d over from this jump)' % (a, hd[a], cur)})
            elif p is not None and p.mnemonic == 'call' and pop(p)[0] is not None:
                n, src = pop(p)
                fixes.append({'at': '0x%08X' % p.address, 'change': n, 'why': 'the call pops %d (%s)' % (n, src)})
            else:
                bad.append({'at': '0x%08X' % a, 'depth': hd[a], 'ida': cur}); continue
            adj += hd[a] - cur
        if fixes or bad: res['0x%08X' % f] = {'name': r['name'], 'fixes': fixes, 'unexplained': bad}
    json.dump(res, open(out, 'w'), indent=1)
    print('%d functions with exact depths checked; IDA wrong in %d: %d corrections, %d unexplained differences' % (
        checked, len(res), sum(len(v['fixes']) for v in res.values()), sum(len(v['unexplained']) for v in res.values())))


if __name__ == '__main__':
    main(*sys.argv[1:])
