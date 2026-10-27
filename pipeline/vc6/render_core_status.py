"""render_core_status.py -- where docs/PLAN.md step 20 stands: the Direct3D-calling functions and their direct callers.

The two sets, as recompile/d3d_round_118.md defines them:
  * #118: every function a site in data/com_calls.json names (`func`), proven or accepted;
  * #119: every function (data/func_extents.json) holding a `call rel32` to one of those, read from the game's bytes,
    less the ones in the first set.
For each: in the frame set (data/frame_set.json), in the tree (tree/placement.json), and at the game's bytes -- by the
sweep as written or by a proven rule (verification/vc6_sweep.json), or by a hand-written source
(verification/recompile_match.json) -- with the source of that verdict.

    FFXI_DLL=... python3 pipeline/vc6/render_core_status.py verification/render_core_status.json"""
import json, os, struct, sys
P = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.join(P, '..', '..')
J = lambda *p: json.load(open(os.path.join(ROOT, *p)))
BASE = 0x10000000


def main(out):
    import pefile
    img = pefile.PE(os.environ['FFXI_DLL'], fast_load=True).get_memory_mapped_image()
    n = lambda a: '0x%08X' % (a if isinstance(a, int) else int(a, 16))
    dev = sorted({n(v['func']) for k, v in J('data', 'com_calls.json').items() if not k.startswith('_') and isinstance(v, dict) and v.get('func')})
    ext = {n(k): v for k, v in J('data', 'func_extents.json').items() if not k.startswith('_')}
    devset = {int(a, 16) for a in dev}
    callers = set()
    for f, pieces in ext.items():
        for s, e in pieces:
            b = img[s - BASE:e - BASE]
            for k in range(len(b) - 4):
                if b[k] == 0xE8 and (s + k + 5 + struct.unpack_from('<i', b, k + 1)[0]) & 0xFFFFFFFF in devset:
                    callers.add(f); break
    callers = sorted(callers - set(dev))
    fs = J('data', 'frame_set.json'); fset = {n(k) for k in (fs.get('functions') or fs) if str(k).startswith('0x')}
    place = {n(k) for k in J('tree', 'placement.json') if k.startswith('0x')}
    sweep = {n(k): v for k, v in J('verification', 'vc6_sweep.json').items() if k.startswith('0x')}
    hand = {n(k): v for k, v in J('verification', 'recompile_match.json').items()}
    def row(a):
        r = {'frame_set': a in fset, 'in_tree': a in place}
        sw = sweep.get(a, {})
        if hand.get(a, {}).get('verdict') == 'IDENTICAL': r['bytes'] = 'IDENTICAL'; r['by'] = hand[a]['file']
        elif sw.get('verdict') in ('IDENTICAL', 'IDENTICAL_UNCOMPARED') or sw.get('rule'):
            r['bytes'] = 'IDENTICAL'; r['by'] = 'vc6_sweep.json' + (' rule ' + sw['rule'] if sw.get('rule') else '')
        else: r['bytes'] = sw.get('verdict') or ('not in the tree' if a not in place else 'no sweep verdict')
        return r
    res = {'_doc': __doc__.split('\n\n')[0], '118': {a: row(a) for a in dev}, '119': {a: row(a) for a in callers}}
    json.dump(res, open(out, 'w'), indent=1, sort_keys=True)
    for t in ('118', '119'):
        rows = res[t].values(); done = sum(r['bytes'] == 'IDENTICAL' for r in rows)
        print('#%s: %d functions, %d at the game\'s bytes (%d in the frame set, %d of them at the bytes)' % (
            t, len(rows), done, sum(r['frame_set'] for r in rows), sum(r['frame_set'] and r['bytes'] == 'IDENTICAL' for r in rows)))


if __name__ == '__main__':
    main(sys.argv[1])
