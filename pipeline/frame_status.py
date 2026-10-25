"""frame_status.py -- one status line per frame-set function: where it stands, from the repo's own records.

    python3 pipeline/frame_status.py verification/frame_set_status.json
For each function of data/frame_set.json ("functions"), from these sources only:
  tree        in tree/placement.json (the verified tree's functions)
  gate        its verdict in verification/gate_clean_final.jsonl (PASS / FAIL / INCONCLUSIVE / UNRESOLVED), if any
  bytes       its verdict in verification/vc6_sweep.json (IDENTICAL / DIFFERS / NO_COMPILE / LEFT_OUT), and the rule
              that made it IDENTICAL if one did (rule_identical)
  d3d         Direct3D call sites in it: "proven" (com_calls.json without an accepted declaration) and "accepted"
              (resting on a declaration the owner accepted, not proven)
  unresolved  its indirect call sites frame_set.py left unresolved
  d3dx        frame_set.py's D3DX flag
Prints the counts the plan's steps are measured by (docs/PLAN.md). Nothing is inferred: a function absent from a
source has no entry for it.
"""
import collections, json, os, sys
ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')
J = lambda *p: json.load(open(os.path.join(ROOT, *p)))


def main(out):
    fs = J('data', 'frame_set.json')['functions']
    place = {k.upper().replace('0X', '0x') for k in J('tree', 'placement.json')}
    gate = {}
    for line in open(os.path.join(ROOT, 'verification', 'gate_clean_final.jsonl')):
        line = line.strip()
        if not line: continue
        r = json.loads(line)
        if r.get('original'): gate[r['original'].upper().replace('0X', '0x')] = r.get('verdict')
    sweep = J('verification', 'vc6_sweep.json')
    d3d = collections.defaultdict(lambda: {'proven': 0, 'accepted': 0})
    for site, v in J('data', 'com_calls.json').items():
        if site.startswith('_') or not isinstance(v, dict) or 'func' not in v: continue
        d3d[v['func'].upper().replace('0X', '0x')]['accepted' if 'accepted' in v else 'proven'] += 1
    res = {}
    for k, v in fs.items():
        a = k.upper().replace('0X', '0x')
        sw = sweep.get(a) or {}
        r = {'tree': a in place, 'gate': gate.get(a), 'bytes': sw.get('verdict'),
             'unresolved': v.get('unresolved', 0), 'd3dx': bool(v.get('d3dx'))}
        if sw.get('rule_identical'): r['bytes_rule'] = sw['rule_identical']
        if a in d3d: r['d3d'] = dict(d3d[a])
        res[a] = r
    json.dump(dict(sorted(res.items())), open(out, 'w'), indent=0)
    n = len(res)
    ident = sum(1 for r in res.values() if r['bytes'] in ('IDENTICAL', 'IDENTICAL_UNCOMPARED') or r.get('bytes_rule'))
    rc = [r for r in res.values() if 'd3d' in r]
    c = collections.Counter
    print('frame set: %d functions' % n)
    print('  in the tree: %d; gate %s' % (sum(r['tree'] for r in res.values()), dict(c(r['gate'] for r in res.values() if r['gate']))))
    print('  at the game\'s bytes (as written or by a rule): %d (%.1f%%)' % (ident, 100.0 * ident / n))
    print('  with unresolved indirect calls: %d functions, %d sites' % (sum(1 for r in res.values() if r['unresolved']), sum(r['unresolved'] for r in res.values())))
    print('  calling Direct3D: %d (in the tree %d, PASS %d, at the bytes %d)' % (
        len(rc), sum(r['tree'] for r in rc), sum(r['gate'] == 'PASS' for r in rc),
        sum(1 for r in rc if r['bytes'] in ('IDENTICAL', 'IDENTICAL_UNCOMPARED') or r.get('bytes_rule'))))


if __name__ == '__main__':
    main(*sys.argv[1:])
