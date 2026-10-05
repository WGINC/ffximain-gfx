"""combine_coverage.py -- per function, the original's instructions no compared gate trial reached, over all gates.

Each gate run with FFXI_COVERAGE=1 (diffemu_gfx.run_case) records, per function, the original's own instructions
(its extent in data/func_extents.json) that ran in the trials that were compared: the original returned and the
reconstruction ran. An instruction is covered when any trial of any gate reached it. A gate's PASS says nothing
about the code no compared trial reached: a defect there (a call that loses ECX, a wrong branch) is not seen.

    python3 combine_coverage.py OUT.json GATE.jsonl [GATE.jsonl ...]
Output {"0xFUNC": {"insns", "covered", "uncovered": [...], "calls", "calls_uncovered": [...], "verdicts": {gate: v}},
"_summary": {...}}; a function counts as fully covered when "uncovered" is empty."""
import json, os, sys


def main(out, gates):
    res = {}
    for g in gates:
        name = os.path.basename(g).rsplit('.', 1)[0]
        for l in open(g):
            r = json.loads(l); c = r.get('coverage')
            k = '0x%08X' % int(r['original'], 16)
            e = res.setdefault(k, {'name': r['name'], 'verdicts': {}, '_unc': None, '_calls': None})
            e['verdicts'][name] = r['verdict']
            if c is None:
                e['missing'] = e.get('missing', []) + [name]; continue
            e['insns'] = c['insns']; e['calls'] = c['calls']
            unc, cu = set(c['uncovered']), set(c['calls_uncovered'])
            e['_unc'] = unc if e['_unc'] is None else e['_unc'] & unc
            e['_calls'] = cu if e['_calls'] is None else e['_calls'] & cu
    full = 0
    for k, e in res.items():
        unc, cu = e.pop('_unc'), e.pop('_calls')
        if unc is None: continue
        e['uncovered'] = sorted(unc); e['calls_uncovered'] = sorted(cu)
        e['covered'] = e['insns'] - len(unc)
        full += not unc and not e.get('missing')
    res = dict(sorted(res.items()))
    res['_summary'] = {'functions': len(res), 'fully_covered': full, 'gates': [os.path.basename(g) for g in gates],
                       'instructions': sum(e.get('insns', 0) for e in res.values() if isinstance(e, dict) and 'insns' in e),
                       'covered': sum(e.get('covered', 0) for e in res.values() if isinstance(e, dict) and 'covered' in e)}
    json.dump(res, open(out, 'w'), indent=0)
    print(json.dumps(res['_summary']))


if __name__ == '__main__':
    main(sys.argv[1], sys.argv[2:])
