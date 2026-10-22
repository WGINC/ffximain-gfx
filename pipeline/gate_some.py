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
SLICE_SECONDS = 20          # a case expected to take longer is split into pieces of about this length


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


def merge_pieces(res, trials):
    """one record per function from the pieces of a split case (gate_some cuts slow cases into trial ranges):
    outcomes, mismatches, D3D calls and unresolved/fault counts added; the earliest mismatch; coverage: an
    instruction is covered if any piece covered it. The verdict as run_case gives it for the whole: FAIL if any
    trial differed, UNRESOLVED if any trial differed only in bytes assumed to matter (seen_unproven), PASS if some
    trial returned and none differed, else UNRESOLVED if any trial was, else INCONCLUSIVE. A function missing a piece is reported INCOMPLETE, never PASS."""
    by = collections.OrderedDict()
    for r in res: by.setdefault(norm(r['original']), []).append(r)
    out = []
    for k, rs in by.items():
        if len(rs) == 1 and '_trial_first' not in rs[0]: out.append(rs[0]); continue
        rs.sort(key=lambda r: r.get('_trial_first', 0))
        m = {kk: v for kk, v in rs[0].items() if kk != '_trial_first'}
        add = lambda key: sum(r.get(key, 0) for r in rs)
        m['trials'] = add('trials'); m['mismatches'] = add('mismatches'); m['d3d_calls_checked'] = add('d3d_calls_checked')
        m['x87_rounded_trials'] = add('x87_rounded_trials'); m['seen_unproven'] = add('seen_unproven')
        for key in ('original_outcomes', 'unresolved', 'original_faults'):
            c = collections.Counter()
            for r in rs: c.update(r.get(key, {}))
            if c or key != 'original_faults': m[key] = dict(c)
        m['first_mismatch'] = next((r['first_mismatch'] for r in rs if r.get('first_mismatch')), None)
        if all('coverage' in r for r in rs):
            unc = set.intersection(*(set(r['coverage']['uncovered']) for r in rs))
            cu = set.intersection(*(set(r['coverage']['calls_uncovered']) for r in rs))
            c0 = rs[0]['coverage']
            m['coverage'] = {'insns': c0['insns'], 'covered': c0['insns'] - len(unc), 'uncovered': sorted(unc),
                             'calls': c0['calls'], 'calls_uncovered': sorted(cu)}
        if m['trials'] != trials or any(r['verdict'] == 'ERROR' for r in rs): m['verdict'] = 'INCOMPLETE'
        elif m['mismatches']: m['verdict'] = 'FAIL'
        elif m['seen_unproven']: m['verdict'] = 'UNRESOLVED'
        elif m['original_outcomes'].get('return', 0): m['verdict'] = 'PASS'
        else: m['verdict'] = 'UNRESOLVED' if m['unresolved'] else 'INCONCLUSIVE'
        m['pieces'] = len(rs)
        out.append(m)
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('run'); ap.add_argument('select', nargs='+')
    ap.add_argument('--jobs', type=int, default=os.cpu_count()); ap.add_argument('--trials', type=int, default=100)
    ap.add_argument('--out', default='gate_some'); ap.add_argument('--chunk', type=int, default=1)
    ap.add_argument('--dll', help='DLL to gate (default RUN/raw/gfx_raw.dll)')
    ap.add_argument('--cases', help='cases file (default RUN/cases.json)')
    ap.add_argument('--no-cache', dest='cache', action='store_false',
                    help='run every case even where gate_cache.py shows the run would be identical to a stored one')
    a = ap.parse_args()
    run = os.path.join(ROOT, 'runs', a.run) if not os.path.isdir(a.run) else a.run
    CASES[0] = a.cases
    want = select(run, a.select)
    casesf = a.cases or os.path.join(run, 'cases.json')
    cases = [c for c in json.load(open(casesf)) if norm(c['original']) in want]
    print('%d selected, %d gateable' % (len(want), len(cases)))
    dll = os.environ['FFXI_DLL']; base = os.path.join(run, a.out)
    # one case per chunk, slowest first (times from earlier runs, runs/gate_times.json; a case never timed is
    # taken to be slow), handed to whichever worker is free: the run ends close to total time / workers
    # instead of waiting on a slow case that happened to come last. Order changes nothing a case sees.
    tkey = 'live:' + os.path.basename(os.environ['FFXI_LIVE']) if os.environ.get('FFXI_LIVE') else 'fuzz'
    tpath = os.path.join(ROOT, 'runs', 'gate_times.json')
    try: times = json.load(open(tpath))
    except (OSError, ValueError): times = {}
    known = times.get(tkey, {})
    # a case known to take long is cut into pieces of trials (diffemu_gfx.run_case: _trial_first/_trial_count;
    # each trial's inputs depend only on its number), run on any free worker and merged below, so one slow
    # function no longer holds the run while the other workers sit idle
    pieces = []
    for c in cases:
        n = min(a.trials // 5, int(known.get(norm(c['original']), 0) // SLICE_SECONDS) + 1) if a.trials >= 10 else 1
        if n <= 1: pieces.append(c); continue
        step = -(-a.trials // n)
        for f in range(0, a.trials, step):
            pieces.append(dict(c, _trial_first=f, _trial_count=min(step, a.trials - f)))
    cases = pieces
    cases.sort(key=lambda c: -known.get(norm(c['original']), 1e9) / max(1, a.trials // c.get('_trial_count', a.trials)))
    chunks = [cases[i:i + a.chunk] for i in range(0, len(cases), a.chunk)]
    for f in os.listdir(run):
        if f.startswith(os.path.basename(base) + '_') and f.endswith(('.json', '.jsonl', '.log')): os.remove(os.path.join(run, f))
    # long-lived workers (diffemu_gfx.py --serve), each handed the next chunk as soon as it finishes one:
    # the start-up (imports, PE parses, opening the dump) is paid once per worker, not once per chunk
    import selectors
    cmd = [sys.executable, os.path.join(P, 'diffemu_gfx.py'), dll, a.dll or os.path.join(run, 'raw', 'gfx_raw.dll'),
           '-', '-', '--trials', str(a.trials), '--seed', '11', '--serve'] + (['--cache'] if a.cache else [])
    sel = selectors.DefaultSelector(); workers = []
    for _ in range(min(a.jobs, len(chunks)) or 0):
        w = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True, bufsize=1)
        sel.register(w.stdout, selectors.EVENT_READ, w); workers.append(w)
    k, res, busy = 0, [], {}
    def feed(w):
        global_k = feed.k
        if global_k >= len(chunks): return False
        cf, rf = '%s_c%d.json' % (base, global_k), '%s_%d.jsonl' % (base, global_k)
        json.dump(chunks[global_k], open(cf, 'w'))
        w.stdin.write('%s %s\n' % (cf, rf)); w.stdin.flush(); busy[w] = rf; feed.k += 1
        return True
    feed.k = 0
    for w in workers: feed(w)
    while busy:
        for key, _ in sel.select():
            w = key.data; line = w.stdout.readline()
            if not line:                                   # the worker died: its chunk has no results
                sel.unregister(w.stdout); rf = busy.pop(w, None)
                if rf: print('worker exited during %s' % rf)
                continue
            if not line.startswith('done '): continue
            rf = busy.pop(w)
            if os.path.exists(rf): res += [json.loads(l) for l in open(rf)]
            if not feed(w): w.stdin.close()
    for w in workers:
        try: w.stdin.close()
        except Exception: pass
        w.wait()
    run_t = collections.defaultdict(float)             # a split case's time is the sum of its pieces
    for i in range(len(chunks)):
        tf = '%s_%d.jsonl.times' % (base, i)
        if os.path.exists(tf):
            for l in open(tf):
                o_, t_ = l.split(); run_t[norm(o_)] += float(t_)
    known.update(run_t)
    times[tkey] = known
    try:
        json.dump(times, open(tpath + '.tmp', 'w'), indent=0, sort_keys=True); os.replace(tpath + '.tmp', tpath)
    except OSError: pass
    if feed.k < len(chunks) or len(res) < len(cases):
        print('INCOMPLETE: %d of %d cases have results' % (len(res), len(cases)))
    res = merge_pieces(res, a.trials)
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
