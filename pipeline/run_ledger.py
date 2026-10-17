#!/usr/bin/env python3
"""run_ledger.py: every frame-time figure in the owner's raw xikit logs, cited by file and line.

Usage: run_ledger.py DIR [DIR ...] > ledger.md

Finds every xikit.log under the given folders (the folders the owner sent; they never enter the repo) and,
for each, the xikit_timing_*.csv, xikit_spikes_*.csv and xikit_ab_*.csv beside it. Nothing is inferred:
each figure printed is a value read from one line, and the line is named as <run folder>/<file>:<line>.
A run folder is the folder holding xikit.log (xikit names it <date>_<time>_<pid>).

What is read, and from which line (formats as printed by kit/xikit/xikit.c and kit/speedfix/sf_main.cpp):
- "xikit started in process N"                     -> the pid
- "kit build: xikit kit, source H, built D"         -> the kit build
- "machine: ..."                                    -> the machine the process ran on
- "graphics module: d3d8.dll at A, version V, ..."  -> the d3d8.dll's file version, as printed ("?" = none)
- "speedfix: settings (start|changed): ..."         -> the levers
- "speedfix: frames N, mean X ms, sd S, max M"      -> speedfix's per-minute frame statistics
- "speedfix: ab: group N (levers): ..."             -> an A/B group starting
- "[speedfix: ]ab: phase N (A|B): F frames, mean X ms, median Y ms[, p95 ...]" -> one phase
- "speedfix: ab N (levers): A (off) X ms, B (on) Y ms: B - A D ms ..."           -> a speedfix A/B summary
- "ab: done. A (off) X ms, B (on) Y ms: B - A D ms ..."                         -> an xikit A/B summary
- "speedfix: occlusion readbacks: N run, M skipped" -> readback counts
- "speedfix: null_draws: N draw calls not sent"     -> the null_draws count
- xikit_timing_*.csv rows (one per 600 frames)       -> frame_ms_avg and frame_ms_p50 per row
- xikit_spikes_*.csv rows                            -> frames slower than spike_ms
- d3d8on11.log (our d3d8.dll's log; in the run folder or the folder above it, rewritten at each start and
  carrying no pid, so which start wrote it is stated as unknown unless one start in the folder used ours):
  "executor ab: phase N, filter=F, K frames: the game's frame X ms on average" -> one executor A/B phase,
  "executor stats, 600 frames: busy X ms a frame"                             -> the executor's load
"""
import csv, os, re, statistics, sys

R = {
    'pid': re.compile(r'xikit started in process (\d+)'),
    'build': re.compile(r'kit build: xikit kit, source (\S+), built ([0-9-]+ [0-9:]+ UTC)'),
    'machine': re.compile(r'machine: (.*)'),
    'gfx': re.compile(r'graphics module: d3d8\.dll at \S+, version ([^,]+),'),
    'settings': re.compile(r'speedfix: settings \((start|changed)\): (.*)'),
    'frames': re.compile(r'speedfix: frames (\d+), mean ([\d.]+) ms, sd ([\d.]+), max ([\d.]+)'),
    'group': re.compile(r'speedfix: ab: group (\d+) \(\s*([^)]*)\): starts in (\d+) s: (\d+) phases of (\d+) s'),
    'phase': re.compile(r'ab: phase (\d+) \(([AB])\): (\d+) frames, mean ([\d.]+) ms, median ([\d.]+) ms'
                        r'(?:, p95 ([\d.]+) ms, max ([\d.]+) ms)?'),
    'sum_sf': re.compile(r'speedfix: ab (\d+) \(\s*([^)]*)\): A \(off\) ([\d.]+) ms, B \(on\) ([\d.]+) ms: '
                         r'B - A ([-+][\d.]+) ms \(([-+][\d.]+)%\), medians ([-+][\d.]+) ms'
                         r'(?:, p95 ([-+][\d.]+) ms)?; phases of one label differ by up to ([\d.]+) ms(.*)'),
    'sum_xk': re.compile(r'\d ab: done\. A \(off\) ([\d.]+) ms, B \(on\) ([\d.]+) ms: B - A ([-+][\d.]+) ms '
                         r'\(([-+][\d.]+)%\), medians ([-+][\d.]+) ms; phases of one label differ by up to ([\d.]+) ms(.*)'),
    'readback': re.compile(r'speedfix: occlusion readbacks: (\d+) run, (\d+) skipped'),
    'nulldraw': re.compile(r'speedfix: null_draws: (\d+) draw calls'),
}
EX_PHASE = re.compile(r"executor ab: phase (\d+), filter=(\d+), (\d+) frames: the game's frame ([\d.]+) ms on average")
EX_BUSY = re.compile(r'executor stats, 600 frames: busy ([\d.]+) ms a frame')


def lever_values(s):
    return dict(kv.split('=', 1) for kv in s.split() if '=' in kv)


def read_csv_rows(path):
    """(line number, row dict) for each data row; '#' comment lines are skipped but counted."""
    out, header = [], None
    with open(path, newline='', errors='replace') as f:
        for n, line in enumerate(f, 1):
            if line.startswith('#') or not line.strip():
                continue
            cells = next(csv.reader([line]))
            if header is None:
                header = cells
                continue
            out.append((n, dict(zip(header, cells))))
    return out


def read_run(log):
    d = os.path.dirname(log)
    run = {'dir': os.path.basename(d), 'log': log, 'pid': [], 'build': [], 'machine': [], 'gfx': [],
           'settings': [], 'frames': [], 'groups': [], 'phases': [], 'sums': [], 'readbacks': [],
           'nulldraws': [], 'timing': [], 'spikes': [], 'ab_csv': []}
    with open(log, errors='replace') as f:
        lines = f.read().splitlines()
    run['lines'] = len(lines)
    for n, line in enumerate(lines, 1):
        for key, rx in R.items():
            m = rx.search(line)
            if not m:
                continue
            g = m.groups()
            if key == 'pid': run['pid'].append((n, g[0]))
            elif key == 'build': run['build'].append((n, g[0], g[1]))
            elif key == 'machine': run['machine'].append((n, g[0]))
            elif key == 'gfx': run['gfx'].append((n, g[0]))
            elif key == 'settings': run['settings'].append((n, g[0], lever_values(g[1])))
            elif key == 'frames': run['frames'].append((n, int(g[0]), float(g[1]), float(g[2]), float(g[3])))
            elif key == 'group': run['groups'].append((n, int(g[0]), g[1].strip()))
            elif key == 'phase':
                run['phases'].append((n, int(g[0]), g[1], int(g[2]), float(g[3]), float(g[4]),
                                      g[5] and float(g[5]), g[6] and float(g[6])))
            elif key == 'sum_sf':
                run['sums'].append({'line': n, 'kind': 'speedfix', 'n': int(g[0]), 'levers': g[1].strip(),
                                    'a': float(g[2]), 'b': float(g[3]), 'd': float(g[4]), 'pct': float(g[5]),
                                    'dmed': float(g[6]), 'dp95': g[7] and float(g[7]), 'spread': float(g[8]),
                                    'note': g[9].strip()})
            elif key == 'sum_xk':
                run['sums'].append({'line': n, 'kind': 'xikit', 'n': 1, 'levers': 'everything the kit adds',
                                    'a': float(g[0]), 'b': float(g[1]), 'd': float(g[2]), 'pct': float(g[3]),
                                    'dmed': float(g[4]), 'dp95': None, 'spread': float(g[5]),
                                    'note': g[6].strip()})
            elif key == 'readback': run['readbacks'].append((n, int(g[0]), int(g[1])))
            elif key == 'nulldraw': run['nulldraws'].append((n, int(g[0])))
            break
    for name in sorted(os.listdir(d)):
        p = os.path.join(d, name)
        if re.fullmatch(r'xikit_timing_\d+\.csv', name):
            run['timing'].append((name, read_csv_rows(p)))
        elif re.fullmatch(r'xikit_spikes_\d+\.csv', name):
            run['spikes'].append((name, read_csv_rows(p)))
        elif re.fullmatch(r'xikit_ab_\d+_\d+\.csv', name):
            run['ab_csv'].append((name, read_csv_rows(p)))
    return run


def cite(run, line, f='xikit.log'):
    return '%s/%s:%d' % (run['dir'], f, line)


def timing_summary(run, name, rows):
    """frame-weighted mean of frame_ms_avg, and the range of the per-row p50, each cited."""
    rows = [(n, r) for n, r in rows if r.get('frames') and r.get('frame_ms_avg')]
    if not rows:
        return '%s: no rows' % name
    tot = sum(int(r['frames']) for _, r in rows)
    mean = sum(int(r['frames']) * float(r['frame_ms_avg']) for _, r in rows) / tot
    lo = min(rows, key=lambda x: float(x[1]['frame_ms_p50']))
    hi = max(rows, key=lambda x: float(x[1]['frame_ms_p50']))
    return ('%s rows %d-%d: %d frames, mean %.2f ms (frame-weighted, all rows); p50 per 600 frames %s ms (%s) to '
            '%s ms (%s)' % (name, rows[0][0], rows[-1][0], tot, mean, lo[1]['frame_ms_p50'],
                            cite(run, lo[0], name), hi[1]['frame_ms_p50'], cite(run, hi[0], name)))


def d3d8on11_section(path, runs_here):
    """our device's log: executor A/B phases and the executor's load, each cited by line."""
    name = '/'.join(os.path.normpath(path).split(os.sep)[-3:])
    with open(path, errors='replace') as f:
        lines = f.read().splitlines()
    print('## %s (%d lines)\n' % (name, len(lines)))
    ours = [r['dir'] for r in runs_here if r['gfx'] and r['gfx'][0][1] == '?']
    # fopen(p, "w") at each start (kit/d3d8on11/d8_common.cpp:19): what is left is the last start's log. The run
    # folders are named by their start time, so the last of ours in sorted order is that start, unless files were
    # moved between folders by hand, which the files cannot show.
    print('- the file carries no pid and is rewritten at each start (d8_common.cpp:19). Starts in this folder whose '
          'd3d8.dll has no file version: %s%s' % (', '.join(ours) or 'none',
                                                  ' (the last of them opened it last: %s)' % ours[-1] if ours else ''))
    for n, line in enumerate(lines, 1):
        if line.startswith('executor:'):
            print('- %s (:%d)' % (line, n))
            break
    phases = []
    for n, line in enumerate(lines, 1):
        m = EX_PHASE.search(line)
        if m:
            phases.append((n, int(m.group(1)), int(m.group(2)), int(m.group(3)), float(m.group(4))))
    if phases:
        print('- executor A/B phases: ' + '; '.join('%d filter=%d %d fr %.3f ms (:%d)' % (p[1], p[2], p[3], p[4], p[0])
                                                   for p in phases))
        by = {}
        for p in phases:
            by.setdefault(p[2], []).append(p)
        for f, ps in sorted(by.items()):
            ms = [p[4] for p in ps]
            lo, hi = min(ps, key=lambda p: p[4]), max(ps, key=lambda p: p[4])
            print('  - filter=%d: %d phases, mean of phase means %.2f ms (computed here, not printed by the device), '
                  'from %.3f (:%d) to %.3f (:%d), spread %.2f ms' % (f, len(ps), sum(ms) / len(ms), lo[4], lo[0],
                                                                      hi[4], hi[0], hi[4] - lo[4]))
    busy = [(n, float(m.group(1))) for n, line in enumerate(lines, 1) for m in [EX_BUSY.search(line)] if m]
    if busy:
        lo, hi = min(busy, key=lambda b: b[1]), max(busy, key=lambda b: b[1])
        print('- executor busy per 600 frames: %d lines, %.3f ms (:%d) to %.3f ms (:%d)' % (len(busy), lo[1], lo[0],
                                                                                         hi[1], hi[0]))
    print()


def main(dirs):
    logs = []
    for top in dirs:
        for root, _, files in os.walk(top):
            if 'xikit.log' in files:
                logs.append(os.path.join(root, 'xikit.log'))
    runs = sorted((read_run(l) for l in logs), key=lambda r: r['dir'])
    print('# Run ledger: every figure read from the raw logs\n')
    print('Generated by pipeline/run_ledger.py. Each figure is cited as <run folder>/<file>:<line>.\n')
    for run in runs:
        pid = run['pid'][0][1] if run['pid'] else '?'
        print('## %s (pid %s, %d log lines)\n' % (run['dir'], pid, run['lines']))
        for n, h, when in run['build'][:1]:
            print('- kit build: source %s, built %s (%s)' % (h, when, cite(run, n)))
        for n, m in run['machine'][:1]:
            print('- machine: %s (%s)' % (m, cite(run, n)))
        if not run['machine']:
            print('- machine: not in the log')
        for n, v in run['gfx'][:1]:
            print('- d3d8.dll file version: %s (%s)' % (v, cite(run, n)))
        if not run['gfx']:
            print('- d3d8.dll: not in the log')
        if run['settings']:
            n, kind, lv = run['settings'][0]
            print('- settings (%s): %s (%s)' % (kind, ' '.join('%s=%s' % kv for kv in lv.items()), cite(run, n)))
            prev = lv
            for n, kind, lv in run['settings'][1:]:
                diff = ['%s %s->%s' % (k, prev.get(k, '-'), v) for k, v in lv.items() if prev.get(k) != v]
                print('  - settings (%s): %s (%s)' % (kind, ', '.join(diff) or 'no change', cite(run, n)))
                prev = lv
        else:
            print('- speedfix settings: not in the log (speedfix not loaded, or an older build)')
        if run['frames']:
            fr = run['frames']
            tot = sum(x[1] for x in fr)
            lo, hi = min(fr, key=lambda x: x[2]), max(fr, key=lambda x: x[2])
            print('- speedfix per-minute frames: %d minutes (%s to %s), %d frames; mean per minute %.2f ms (%s) '
                  'to %.2f ms (%s)' % (len(fr), cite(run, fr[0][0]), cite(run, fr[-1][0]), tot, lo[2],
                                       cite(run, lo[0]), hi[2], cite(run, hi[0])))
            print('  - each minute: ' + ', '.join('%.2f (:%d)' % (x[2], x[0]) for x in fr))
        for name, rows in run['timing']:
            print('- ' + timing_summary(run, name, rows))
        for name, rows in run['spikes']:
            ms = [(n, float(r['ms'])) for n, r in rows if r.get('ms')]
            if ms:
                top = max(ms, key=lambda x: x[1])
                print('- %s: %d spike frames, the longest %.1f ms (%s)' % (name, len(ms), top[1], cite(run, top[0], name)))
            else:
                print('- %s: no spike rows' % name)
        for n, ran, sk in run['readbacks']:
            print('- occlusion readbacks: %d run, %d skipped (%s)' % (ran, sk, cite(run, n)))
        for n, k in run['nulldraws']:
            print('- null_draws: %d draw calls not sent (%s)' % (k, cite(run, n)))
        for n, g, lv in run['groups']:
            print('- A/B group %d (%s) starts (%s)' % (g, lv, cite(run, n)))
        if run['phases']:
            print('- A/B phases: ' + '; '.join('%d%s %d fr mean %.2f med %.2f (:%d)' % (p[1], p[2], p[3], p[4], p[5], p[0])
                                               for p in run['phases']))
        for s in run['sums']:
            print('- **A/B %s %d (%s): A %.2f ms, B %.2f ms, B - A %+.2f ms (%+.1f%%), medians %+.2f ms%s; spread %.2f ms'
                  '%s** (%s)' % (s['kind'], s['n'], s['levers'], s['a'], s['b'], s['d'], s['pct'], s['dmed'],
                                 '' if s['dp95'] is None else ', p95 %+.2f ms' % s['dp95'], s['spread'],
                                 ' ' + s['note'] if s['note'] else '', cite(run, s['line'])))
        for name, rows in run['ab_csv']:
            print('- %s: ' % name + '; '.join('phase %s %s mean %s p50 %s (:%d)' % (r['phase'], r['label'], r['mean_ms'],
                                                                                   r['p50_ms'], n) for n, r in rows))
        print()
    seen = set()
    for run in runs:
        for d in (os.path.dirname(run['log']), os.path.dirname(os.path.dirname(run['log']))):
            p = os.path.join(d, 'd3d8on11.log')
            if os.path.isfile(p) and os.path.realpath(p) not in seen:
                seen.add(os.path.realpath(p))
                here = [r for r in runs if os.path.dirname(r['log']) == d or os.path.dirname(os.path.dirname(r['log'])) == d]
                d3d8on11_section(p, here)
    print('## Every A/B summary line\n')
    print('| Run | Levers | A ms | B ms | B - A ms | medians | p95 | spread | printed note | Source |')
    print('|---|---|---|---|---|---|---|---|---|---|')
    for run in runs:
        for s in run['sums']:
            print('| %s | %s | %.2f | %.2f | %+.2f | %+.2f | %s | %.2f | %s | %s |' % (
                run['dir'], s['levers'], s['a'], s['b'], s['d'], s['dmed'],
                '' if s['dp95'] is None else '%+.2f' % s['dp95'], s['spread'], s['note'], cite(run, s['line'])))


if __name__ == '__main__':
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    main(sys.argv[1:])
