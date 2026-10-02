"""gate_some.py -- gate a subset of a run's raw translation, in parallel shards.

    python3 gate_some.py RUN SELECT [SELECT...] [--jobs N] [--trials 100] [--out NAME]

SELECT is an address (0x10009920), `all`, `fail` / `inconclusive` / `notpass` (verdicts in
verification/gate_raw_final.jsonl), or `changed:OTHER_RAW_DIR` (functions whose translated source
differs from that directory). Results go to runs/RUN/NAME.jsonl (default gate_some), one line per
function, and a verdict count is printed, with each function's change from the reference verdict.
"""
import argparse, json, os, subprocess, sys, filecmp, collections

P = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.join(P, '..')


def reference():
    return {norm(json.loads(l)['original']): json.loads(l)['verdict']
            for l in open(os.path.join(ROOT, 'verification', 'gate_raw_final.jsonl'))}


def norm(a): return '0x%08X' % int(a, 16)


def select(run, sels):
    ref, out = reference(), set()
    for s in sels:
        if s == 'all':
            out |= {norm(c['original']) for c in json.load(open(os.path.join(run, 'cases.json')))}
        elif s in ('fail', 'inconclusive', 'notpass'):
            want = {'fail': {'FAIL'}, 'inconclusive': {'INCONCLUSIVE'}}.get(s)
            out |= {k for k, v in ref.items() if (v in want if want else v != 'PASS')}
        elif s.startswith('changed:'):
            other = s.split(':', 1)[1]
            for f in os.listdir(os.path.join(run, 'raw')):
                if f.startswith('fn_') and f.endswith('.cpp'):
                    o = os.path.join(other, f)
                    if not os.path.exists(o) or not filecmp.cmp(o, os.path.join(run, 'raw', f), shallow=False):
                        out.add(norm(f[3:11]))
        else:
            out.add(norm(s))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('run'); ap.add_argument('select', nargs='+')
    ap.add_argument('--jobs', type=int, default=os.cpu_count()); ap.add_argument('--trials', type=int, default=100)
    ap.add_argument('--out', default='gate_some')
    a = ap.parse_args()
    run = os.path.join(ROOT, 'runs', a.run) if not os.path.isdir(a.run) else a.run
    want = select(run, a.select)
    cases = [c for c in json.load(open(os.path.join(run, 'cases.json'))) if norm(c['original']) in want]
    print('%d selected, %d gateable' % (len(want), len(cases)))
    dll = os.environ['FFXI_DLL']; base = os.path.join(run, a.out)
    procs = []
    for s in range(a.jobs):
        part = cases[s::a.jobs]
        if not part: continue
        json.dump(part, open('%s_c%d.json' % (base, s), 'w'))
        if os.path.exists('%s_%d.jsonl' % (base, s)): os.remove('%s_%d.jsonl' % (base, s))
        procs.append(subprocess.Popen([sys.executable, os.path.join(P, 'diffemu_gfx.py'), dll, os.path.join(run, 'raw', 'gfx_raw.dll'),
                                       '%s_c%d.json' % (base, s), '%s_%d.jsonl' % (base, s), '--trials', str(a.trials), '--seed', '11'],
                                      stdout=open('%s_%d.log' % (base, s), 'w'), stderr=subprocess.STDOUT))
    for p in procs: p.wait()
    res = []
    for s in range(a.jobs):
        f = '%s_%d.jsonl' % (base, s)
        if os.path.exists(f): res += [json.loads(l) for l in open(f)]
    with open(base + '.jsonl', 'w') as f:
        for r in res: f.write(json.dumps(r) + '\n')
    ref = reference()
    print(collections.Counter(r['verdict'] for r in res))
    for r in sorted(res, key=lambda r: r['original']):
        old = ref.get(norm(r['original']))
        if old != r['verdict']:
            print('  %s %s: %s -> %s' % (r['original'], r['name'].split()[0], old, r['verdict']))


if __name__ == '__main__':
    main()
