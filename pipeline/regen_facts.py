"""regen_facts.py -- re-derive every fact file in data/ from the DLL, in verify_all's order, until none changes.

The facts read each other (reg_saved reads cdecl_args, caller_reads reads reg_saved, x87_effects reads reg_saved,
...), and not all in list order, so one pass after a change to a tool or an input can leave a later fact stale for
an earlier one. Each pass runs verify_all.fact_commands in order, installing each result before the next runs; the
passes repeat until one changes nothing. That is the state verify_all's step 2 checks: every fact what its tool
derives from the committed inputs.

    FFXI_DLL=... python3 pipeline/regen_facts.py [--max N] [--only a.json,b.json] [--full] [--jobs N]
Prints, per pass, the facts that changed and how many top-level keys of each.

Only what changed is run again. Every tool runs under pipeline/regen_reads/sitecustomize.py, which records each
file the tool's Python processes open for reading (its imports included) and each directory they list; the
contents of all of them under the repo, and the DLL, are hashed after the run (runs/regen_facts/state.json).
A tool is run again when its command changed, when any file or listing it read differs from what it read last
time, or when data/ no longer holds what it produced. Nothing about a tool's inputs is declared by hand. A tool
that started a program (idb_members.py runs the compiler) read things the hook cannot see, so it runs every pass.
A skipped tool is taken to produce what it produced from the same inputs: the tools are deterministic in their
inputs, which is what verify_all's re-derivation already relies on. `--full` runs every tool in its first pass (the recorded reads still order them).

Tools run concurrently (--jobs, default the core count, and within 80% of the memory available at the start, by
each tool's last measured peak; a tool never measured runs alone) where the last recorded reads allow: a tool starts once
every earlier tool whose output it read last time has finished. If a run shows it read the output of an earlier
tool that was still running, its result is discarded and it runs again after that tool. Files outside the repo
(the Python installation) are not tracked.
"""
import hashlib, json, os, shutil, subprocess, sys, tempfile, threading, time
from concurrent.futures import ThreadPoolExecutor, FIRST_COMPLETED, wait
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import verify_all as VA

ROOT, D = VA.ROOT, VA.D
STATE_DIR = os.path.join(ROOT, 'runs', 'regen_facts')
STATE = os.path.join(STATE_DIR, 'state.json')
DLL = os.path.realpath(os.environ['FFXI_DLL'])
SKIP_PARTS = ('/.git/', '/__pycache__/')

_hash_cache, _hash_lock = {}, threading.Lock()
RUN = None


def fhash(p):
    """content hash of a file, or of a directory's sorted listing; None if absent"""
    try: st = os.stat(p)
    except OSError: return None
    key = (p, st.st_size, st.st_mtime_ns, st.st_ino)
    with _hash_lock:
        if key in _hash_cache: return _hash_cache[key]
    if os.path.isdir(p): h = 'dir:' + hashlib.sha256('\n'.join(sorted(os.listdir(p))).encode()).hexdigest()
    else:
        s = hashlib.sha256()
        with open(p, 'rb') as f:
            for b in iter(lambda: f.read(1 << 20), b''): s.update(b)
        h = s.hexdigest()
    with _hash_lock: _hash_cache[key] = h
    return h


def import_hash(d, names):
    """hash of the entries of directory d whose module name is one of names (and which ones are absent)"""
    try: ents = sorted(e for e in os.listdir(d) if e.split('.')[0] in names)
    except OSError: return None
    return 'imp:' + hashlib.sha256('\n'.join(ents).encode()).hexdigest()


def run_signature():
    """what every tool runs under and its hook cannot see: the hook itself (read before it is installed) and the
    interpreter (outside the repo). A change to either runs every tool again"""
    py = os.path.realpath(shutil.which('python3'))
    st = os.stat(py)
    return '%s|%s|%d|%d' % (fhash(os.path.join(P, 'regen_reads', 'sitecustomize.py')), py, st.st_size, st.st_mtime_ns)


def tracked(p):
    return (p == DLL or (p.startswith(ROOT + os.sep) or p == ROOT)) and not any(s in p + '/' for s in SKIP_PARTS)


def run_tool(f, cmd, work, tag):
    """runs one tool into work/; returns (rc, out, reads {path: hash}, outputs [(fact, path)], started_programs)"""
    o = os.path.join(work, f)
    log = os.path.join(work, '.reads.%s.%s' % (f, tag))
    for p in (o, log):
        if os.path.exists(p): os.remove(p)
    pyc = tempfile.mkdtemp(prefix='pyc_', dir=work)
    env = dict(os.environ, REGEN_READS=log, PYTHONDONTWRITEBYTECODE='1', PYTHONPYCACHEPREFIX=pyc,
               PYTHONPATH=os.pathsep.join([os.path.join(P, 'regen_reads')] + ([os.environ['PYTHONPATH']] if os.environ.get('PYTHONPATH') else [])))
    # run through wait4 for the peak memory of the tool and everything it started (ru_maxrss, KiB on Linux)
    with tempfile.TemporaryFile() as so:
        pr = subprocess.Popen('python3 pipeline/' + cmd.format(o=o), shell=True, cwd=ROOT, env=env, stdout=so, stderr=subprocess.STDOUT)
        _, status, ru = os.wait4(pr.pid, 0)
        pr.returncode = os.waitstatus_to_exitcode(status)
        so.seek(0); text = so.read().decode('utf-8', 'replace')
    class r: returncode = pr.returncode; stdout = text; stderr = ''
    rss_mb = ru.ru_maxrss // 1024
    shutil.rmtree(pyc, ignore_errors=True)
    reads, writes, progs, ilist, names = set(), set(), [], set(), set()
    if os.path.exists(log):
        for line in open(log, encoding='utf-8', errors='surrogateescape'):
            k, _, v = line.rstrip('\n').partition('\t')
            if k == 'x': progs.append(v)
            elif k == 'w': writes.add(os.path.realpath(v))
            elif k in ('r', 'l'): reads.add(os.path.realpath(v))
            elif k == 'L': ilist.add(os.path.realpath(v))
            elif k == 'i': names.add(v.split('.')[0]); names.add(v.rsplit('.', 1)[-1])
        os.remove(log)
    # what the tool wrote itself (its own outputs and scratch) is not an input
    reads = {p: fhash(p) for p in sorted(reads - writes) if tracked(p)}
    # a directory the import system listed: only the entries an import's name could resolve to (name.py, name/,
    # name.<ext>), present or absent -- a module appearing where an import looked would change what it finds
    for d in sorted(ilist - reads.keys()):
        if tracked(d): reads['import:' + d] = import_hash(d, names)
    outs = [(f, o)] + ([('ecx_use.json', os.path.join(work, 'ecx_use.json'))] if f == 'vcalls.json' else [])
    return r.returncode, r.stdout + r.stderr, reads, outs, progs, sorted(names), rss_mb


def install(src, dst):
    tmp = dst + '.regen_tmp'
    shutil.copyfile(src, tmp); os.replace(tmp, dst)


def main():
    mx = int(sys.argv[sys.argv.index('--max') + 1]) if '--max' in sys.argv else 6
    only = set(sys.argv[sys.argv.index('--only') + 1].split(',')) if '--only' in sys.argv else None
    jobs = int(sys.argv[sys.argv.index('--jobs') + 1]) if '--jobs' in sys.argv else os.cpu_count()
    full = '--full' in sys.argv
    src = VA.load(os.path.join(ROOT, 'tree', 'source.json'))
    os.makedirs(STATE_DIR, exist_ok=True)
    work = os.path.join(STATE_DIR, 'work'); os.makedirs(work, exist_ok=True)
    # a fixed path, so that a tool's recorded read of it means the same file next time
    texp = os.path.join(work, 'tree_export.jsonl')
    VA.sh('git show %s:data/exports/gfx_export.jsonl > %s.new && cmp -s %s.new %s || mv %s.new %s; rm -f %s.new'
          % ((src['export_commit'], texp) + (texp,) * 5))
    cmds = [(f, c) for f, c in VA.fact_commands(texp, src['raw_gate']) if only is None or f in only]
    order = {f: i for i, (f, _) in enumerate(cmds)}
    data_path = {f: os.path.realpath(os.path.join(D, f)) for f, _ in cmds}
    data_path['ecx_use.json'] = os.path.realpath(os.path.join(D, 'ecx_use.json'))
    producer = {data_path[f]: f for f, _ in cmds}; producer[data_path['ecx_use.json']] = 'vcalls.json'
    state = json.load(open(STATE)) if os.path.exists(STATE) else {}
    global RUN; RUN = run_signature()
    # the memory the tools may use at once: what the system has available at the start, less a margin
    avail = next(int(l.split()[1]) for l in open('/proc/meminfo') if l.startswith('MemAvailable:')) // 1024
    budget = int(avail * 0.8)
    print('jobs %d, memory budget %d MB' % (jobs, budget), flush=True)

    def stale(f, cmd):
        s = state.get(f)
        if full or s is None or s.get('cmd') != cmd or s.get('programs') or s.get('run') != RUN: return True
        for p, h in s['reads'].items():
            if p.startswith('import:'):
                if import_hash(p[7:], s.get('names', ())) != h: return True
            elif fhash(p) != h: return True
        return any(fhash(data_path[g]) != h for g, h in s['produced'].items())

    t_all = time.time()
    for n in range(1, mx + 1):
        if n > 1: full = False   # --full: every tool in the first pass; after it, what changed
        changed, ran, t0 = [], [], time.time()
        todo = [f for f, _ in cmds]
        cmd_of = dict(cmds)
        running, done_at, started_at = {}, {}, {}
        mem_of = {f: ((state.get(f) or {}).get('rss_mb') or budget) for f in todo}
        # the earlier tools whose output a tool read last time
        def needs(f):
            s = state.get(f)
            if s is None: return [g for g in todo_all if order[g] < order[f]]   # never recorded: after all before it
            return [producer[p] for p in s['reads'] if p in producer and order[producer[p]] < order[f]]
        todo_all = list(todo)
        pending = list(todo)
        finished = set()
        failed = None
        with ThreadPoolExecutor(max_workers=jobs) as ex:
            while pending or running:
                for f in list(pending):
                    if len(running) >= jobs: break
                    # memory: a tool starts only if its last peak fits beside the running tools' peaks; a tool with no
                    # recorded peak runs alone (a tool killed for memory fails the pass, and a pass must not)
                    need = (state.get(f) or {}).get('rss_mb')
                    held = sum(mem_of[x] for x in running.values())
                    if running and (need is None or held + need > budget): continue
                    if not all(g in finished for g in needs(f)): continue
                    pending.remove(f)
                    if not stale(f, cmd_of[f]):
                        finished.add(f); done_at[f] = time.time(); continue
                    started_at[f] = time.time()
                    running[ex.submit(run_tool, f, cmd_of[f], work, 'p%d' % n)] = f
                if not running:
                    if pending: continue   # a skip freed a dependency: schedule again
                    break
                fin, _ = wait(list(running), return_when=FIRST_COMPLETED)
                for fu in fin:
                    f = running.pop(fu)
                    rc, out, reads, outs, progs, names, rss = fu.result()
                    if rc or not os.path.exists(outs[0][1]):
                        failed = (f, (out.strip().splitlines() or [rc])[-1]); continue
                    # read an earlier tool's output while that tool was still running: run again after it
                    early = [producer[p] for p in reads if p in producer and order[producer[p]] < order[f]
                             and (producer[p] not in done_at or done_at[producer[p]] > started_at[f])]
                    if early:
                        pending.insert(0, f); state.setdefault(f, {})['reads'] = reads
                        state[f].setdefault('cmd', None); continue
                    produced = {}
                    for g, og in outs:
                        dp = data_path[g]
                        d = VA.jdiff(VA.load(og), VA.load(dp)) if os.path.exists(dp) else ['(new fact file)']
                        if d:
                            install(og, dp); changed.append((g, len(d)))
                            print('pass %d: %s changed (%d keys)' % (n, g, len(d)), flush=True)
                        produced[g] = fhash(dp)
                    state[f] = {'cmd': cmd_of[f], 'reads': reads, 'produced': produced, 'programs': progs, 'names': names, 'run': RUN,
                                'seconds': round(time.time() - started_at[f], 1), 'rss_mb': rss}
                    ran.append(f); finished.add(f); done_at[f] = time.time()
                    print('  %s: %.0f s, %d MB' % (f, state[f]['seconds'], rss), flush=True)
                if failed: break
        json.dump(state, open(STATE + '.tmp', 'w'), indent=1, sort_keys=True); os.replace(STATE + '.tmp', STATE)
        if failed:
            print('pass %d: %s failed: %s' % (n, failed[0], failed[1])); return 1
        print('pass %d: ran %d of %d tools (%s) in %.0f s' % (n, len(ran), len(cmds),
              ', '.join(ran) if len(ran) <= 8 else '%d tools' % len(ran), time.time() - t0), flush=True)
        if not changed:
            print('pass %d: nothing changed (%.0f s in all)' % (n, time.time() - t_all)); return 0
    print('still changing after %d passes' % mx); return 1


if __name__ == '__main__':
    sys.exit(main())
