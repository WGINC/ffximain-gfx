"""combine_gates.py -- one verdict per function from the fuzz gate and the live gates (one per verified dump).

A function is PASS when at least one gate compared it and passed, and no gate failed it: a FAIL anywhere
is a FAIL. With no comparable run anywhere, the fuzz verdict stands (UNRESOLVED, naming its call site, or
INCONCLUSIVE). A fuzz UNRESOLVED on a comparison the live gates did not make (NEWER) stays UNRESOLVED whatever
they say. Each record keeps the fuzz record's fields and adds `gates`: the verdict of every gate and
its returning-trial count, so the evidence behind a combined PASS stays visible.

    python3 combine_gates.py FUZZ.jsonl OUT.jsonl LIVE_NAME=LIVE.jsonl [LIVE_NAME=LIVE.jsonl ...]
"""
import json, sys, collections

# a fuzz verdict left UNRESOLVED by a comparison the committed live gates predate: the entry EAX handed to a callee
# that may read it (eax_inputs.json, 22 Oct). Those gates never compared it, so their PASS does not answer it.
NEWER = ('entry EAX passed to', 'EAX at return differs past the declared')


def main(fuzz, out, *lives):
    gates = [('fuzz', fuzz)] + [tuple(x.split('=', 1)) for x in lives]
    recs = {g: {json.loads(l)['original']: json.loads(l) for l in open(p)} for g, p in gates}
    res = []; changed = collections.Counter()
    for k, base in recs['fuzz'].items():
        per = {g: recs[g].get(k) for g, _ in gates}
        vs = {g: r['verdict'] for g, r in per.items() if r}
        newer = [u for u in (base.get('unresolved') or {}) if u.startswith(NEWER)] if base['verdict'] == 'UNRESOLVED' else []
        if 'FAIL' in vs.values(): v = 'FAIL'
        elif newer and any(g != 'fuzz' and vs[g] == 'PASS' for g in vs): v = 'UNRESOLVED'   # see NEWER
        elif 'PASS' in vs.values(): v = 'PASS'
        else: v = base['verdict']
        r = dict(base) if v == base['verdict'] or v != 'FAIL' else dict(next(per[g] for g in vs if vs[g] == 'FAIL'))
        if v == 'PASS' and base['verdict'] != 'PASS':
            r = dict(next(per[g] for g in vs if vs[g] == 'PASS'))
        r['verdict'] = v
        r['gates'] = {g: [x['verdict'], (x.get('original_outcomes') or {}).get('return', 0)] for g, x in per.items() if x}
        if v != base['verdict']: changed['%s -> %s' % (base['verdict'], v)] += 1
        res.append(r)
    open(out, 'w').write(''.join(json.dumps(r) + '\n' for r in res))
    print('%d functions: %s; changed from the fuzz verdict: %s' % (
        len(res), dict(collections.Counter(r['verdict'] for r in res)), dict(changed)))


if __name__ == '__main__':
    main(*sys.argv[1:])
