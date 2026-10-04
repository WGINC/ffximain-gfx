"""gate_some.py -- gate a subset of a run's raw translation, in parallel shards.

    python3 gate_some.py RUN SELECT [SELECT...] [--jobs N] [--trials 100] [--out NAME]

SELECT is an address (0x10009920), `all`, `fail` / `inconclusive` / `notpass` (verdicts in
verification/gate_raw_final.jsonl), or `changed:OTHER_RAW_DIR` (functions whose translated source
differs from that directory). Results go to runs/RUN/NAME.jsonl (default gate_some), one line per
function, and a verdict count is printed, with each function's change from the reference verdict.
"""
import argparse, json, os, subprocess, sys, filecmp, collections, time

P = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.join(P, '..')


def reference():
    return {norm(json.loads(l)['original']): json.loads(l)['verdict']
            for l in open(os.path.join(ROOT, 'verification', 'gate_raw_final.jsonl'))}


CASES = [None]


def norm(a): return '0x%08X' % int(a, 16)


def select(run, sels):
    ref, out = reference(), set()
    for s in sels:
        if s == 'all':
            out |= {norm(c['original']) for c in json.load(open(CASES[0] or os.path.join(run, 'cases.json')))}
        elif s in ('fail', 'inconclusive', 'unresolved', 'notpass'):
            want = {'fail': {'FAIL'}, 'inconclusive': {'INCONCLUSIVE'}, 'unresolved': {'UNRESOLVED'}}.get(s)
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
    ap.add_argument('--out', default='gate_some'); ap.add_argument('--chunk', type=int, default=4)
    ap.add_argument('--dll', help='DLL to gate (default RUN/raw/gfx_raw.dll)')
    ap.add_argument('--cases', help='cases file (default RUN/cases.json)')
    ap.add_argument('--cache', action='store_true', help='reuse verdicts of provably identical runs (gate_cache.py)')
    a = ap.parse_args()
    run = os.path.join(ROOT, 'runs', a.run) if not os.path.isdir(a.run) else a.run
    CASES[0] = a.cases
    want = select(run, a.select)
    casesf = a.cases or os.path.join(run, 'cases.json')
    cases = [c for c in json.load(open(casesf)) if norm(c['original']) in want]
    print('%d selected, %d gateable' % (len(want), len(cases)))
    dll = os.environ['FFXI_DLL']; base = os.path.join(run, a.out)
    # small chunks handed out as workers free up: a few slow cases cannot strand the rest behind them
    chunks = [cases[i:i + a.chunk] for i in range(0, len(cases), a.chunk)]
    for f in os.listdir(run):
        if f.startswith(os.path.basename(base) + '_') and f.endswith(('.json', '.jsonl', '.log')): os.remove(os.path.join(run, f))
    running, k, res = [], 0, []
    while k < len(chunks) or running:
        while k < len(chunks) and len(running) < a.jobs:
            cf, rf = '%s_c%d.json' % (base, k), '%s_%d.jsonl' % (base, k)
            json.dump(chunks[k], open(cf, 'w'))
            running.append((subprocess.Popen([sys.executable, os.path.join(P, 'diffemu_gfx.py'), dll, a.dll or os.path.join(run, 'raw', 'gfx_raw.dll'),
                                              cf, rf, '--trials', str(a.trials), '--seed', '11'] + (['--cache'] if a.cache else []),
                                             stdout=open('%s_%d.log' % (base, k), 'w'), stderr=subprocess.STDOUT), rf))
            k += 1
        time.sleep(0.5)
        for p_, rf in list(running):
            if p_.poll() is not None:
                running.remove((p_, rf))
                if os.path.exists(rf): res += [json.loads(l) for l in open(rf)]
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
