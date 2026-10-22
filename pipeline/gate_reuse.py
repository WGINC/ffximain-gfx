"""gate_reuse.py -- which of an earlier raw gate's verdicts a changed gate would compute identically.

Used once, 22 Oct, when two gate changes reached only two code paths of diffemu_gfx:
  * site_pops' sources (P13: IDB call types only where SDK-identical; a direct callee's `ret N`), reached only by a
    run that looks up a call site;
  * ret_unproven (bytes a function's own code never sets, ret_width.json), reached only by a case whose function is
    in ret_width.json with a void, bool or word return.
The earlier run (gate_raw10) stored, per piece, the call sites site_pops looked up (runs/gate_cache: each entry's
deps beside its exact result). A function whose pieces looked up none, and whose case is not of the second kind,
ran no changed code, so its earlier verdict is the new one. Every other function is re-gated.
Checked on the IDA route, gated in full with the new code: all 671 functions this selects had identical results
(verdict, mismatches, outcomes, first mismatch, unresolved, trials, seen_unproven).

    python3 gate_reuse.py OUT.json RUN [RUN ...]       (runs/RUN/gate_raw10*.jsonl, runs/RUN/cases.json)
Output {RUN: {addr: the earlier merged record}} for the reusable functions; counts printed."""
import collections, glob, hashlib, json, sys

H = lambda r: hashlib.sha256(json.dumps(r, sort_keys=True).encode()).hexdigest()


def main(out, *runs):
    idx = {}
    for p in glob.glob('runs/gate_cache/*/*.json'):
        try: es = json.load(open(p))
        except ValueError: continue
        for e in es: idx.setdefault(H(e['result']), e['deps'])
    rw = json.load(open('data/ret_width.json'))
    res = {}
    for r in runs:
        pieces = collections.defaultdict(list)
        for p in glob.glob('runs/%s/gate_raw10_*.jsonl' % r):
            for l in open(p):
                x = json.loads(l); pieces[x['original']].append(x)
        merged = {json.loads(l)['original']: json.loads(l) for l in open('runs/%s/gate_raw10.jsonl' % r)}
        cases = {c['original']: c for c in json.load(open('runs/%s/cases.json' % r))}
        c = collections.Counter(); keep = {}
        for a, m in merged.items():
            deps = [idx.get(H(x)) for x in pieces.get(a, [])]
            if not deps or None in deps: c['no cache record: re-gate'] += 1; continue
            narrow = cases.get(a, {}).get('returns') in ('void', 'unread', 'bool', 'word') and ('0x%08X' % int(a, 16)) in rw
            if set().union(*(set(d['sites']) for d in deps)): c['looked up call sites: re-gate'] += 1
            elif narrow: c['ret_width, narrow return: re-gate'] += 1
            else: c['reuse'] += 1; keep[a] = m
        print(r, dict(c)); res[r] = keep
    json.dump(res, open(out, 'w'))


if __name__ == '__main__':
    main(*sys.argv[1:])
