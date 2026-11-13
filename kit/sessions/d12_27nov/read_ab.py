"""read_ab.py -- the 27 Nov session's A/B: each group (step_exact; pace_adaptive; both), A (off) against B (on).

    python3 read_ab.py <start folder in xikit_logs> <presentmon csv>

speedfix marks each phase "ab gG P X levers" (G the group, P the phase, X A or B) and the end "ab end" (sf_main.cpp
ab_set); the kit puts the mark's number in its frame log with the frame's qpc, PresentMon's CPUStartQPC is the same
counter. A phase runs from its mark to the next mark; its first 60 frames are left out (ab_settle). Per group and label:
  frames      mean, p95, p99, the mean change from one frame to the next (xikit frames);
  stutter     frames over 16 ms and over twice the median of the 60 around them, a minute (the 25-26 Nov measure);
  on screen   PresentMon DisplayedTime: shown frames held one refresh, two, more (the refresh: the most common shown
              time); presents never shown; shown frames a second; the mean change of on-screen time between shown frames."""
import collections, csv, glob, os, re, statistics as st, sys
SETTLE = 60


def pct(a, p):
    a = sorted(a); return a[min(len(a) - 1, int(p / 100 * len(a)))] if a else float('nan')


def main(folder, pm_path, qpf=24e6):
    log = open(os.path.join(folder, 'xikit.log'), errors='replace').read()
    label = {int(n): t.strip() for n, t in re.findall(r'measure: mark (\d+) at frame \d+ \(mark\.bat\): (.*)', log)}
    rows, hdr = [], None
    for l in open(glob.glob(os.path.join(folder, 'xikit_frames_*.csv'))[0], errors='replace'):
        if l.startswith('frame,'): hdr = l.strip().split(','); continue
        if hdr and l[:1].isdigit(): rows.append(dict(zip(hdr, l.rstrip('\r\n').split(','))))
    ft = [float(r['frame_ms']) for r in rows]; q = [int(r['qpc']) for r in rows]; mk = [int(r['mark'] or 0) for r in rows]
    spike = set(i for i in range(30, len(ft) - 30) if ft[i] > 16 and ft[i] > 2 * sorted(ft[i - 30:i + 30])[30])
    pm = list(csv.DictReader(open(pm_path, encoding='utf-8-sig', errors='replace')))
    shown = [float(r['DisplayedTime']) for r in pm if r['DisplayedTime'] not in ('NA', '')]
    refresh = collections.Counter(round(x * 4) / 4 for x in shown).most_common(1)[0][0] if shown else 8.33
    print('refresh (the most common on-screen time): %.2f ms' % refresh)
    agg = collections.defaultdict(lambda: collections.defaultdict(list)); names = {}
    for m in sorted(set(mk)):
        mm = re.match(r'ab g(\d+) (\d+) ([AB]) ?(.*)$', label.get(m, ''))
        if not mm: continue
        g, ph, lab, lev = int(mm.group(1)), int(mm.group(2)), mm.group(3), mm.group(4); names[g] = lev
        idx = [i for i in range(len(rows)) if mk[i] == m][SETTLE:]
        if len(idx) < 100: continue
        q0, q1 = q[idx[0]], q[idx[-1]]
        a = agg[(g, lab)]
        a['ft'] += [ft[i] for i in idx]; a['secs'].append((q1 - q0) / qpf); a['spikes'].append(sum(1 for i in idx if i in spike))
        pr = [r for r in pm if q0 <= int(r['CPUStartQPC']) <= q1]
        a['dt'] += [float(r['DisplayedTime']) for r in pr if r['DisplayedTime'] not in ('NA', '')]
        a['never'].append(sum(1 for r in pr if r['DisplayedTime'] in ('NA', ''))); a['pres'].append(len(pr))
        a['phases'].append(ph)
    for g in sorted(names):
        print('group %d (%s):' % (g, names[g]))
        for lab, what in (('A', 'off'), ('B', 'on ')):
            a = agg.get((g, lab))
            if not a or not a['ft']: print('  %s: no frames' % lab); continue
            f, dt = a['ft'], a['dt']; secs = sum(a['secs'])
            one = sum(1 for x in dt if x < 1.5 * refresh) / max(len(dt), 1); two = sum(1 for x in dt if 1.5 * refresh <= x < 2.5 * refresh) / max(len(dt), 1)
            print('  %s (%s, phases %s, %.0f s): frame mean %.2f ms p95 %.2f p99 %.2f change %.2f | stutter %.1f a minute | shown: one refresh %.1f%%, two %.1f%%, more %.1f%%, never shown %.1f%%, %.1f a second, change %.2f ms' % (
                lab, what, a['phases'], secs, st.mean(f), pct(f, 95), pct(f, 99), st.mean(abs(f[i] - f[i - 1]) for i in range(1, len(f))),
                60 * sum(a['spikes']) / max(secs, 1), 100 * one, 100 * two, 100 * (1 - one - two) if dt else 0,
                100 * sum(a['never']) / max(sum(a['pres']), 1), 1000 * len(dt) / max(sum(dt), 1),
                st.mean(abs(dt[i] - dt[i - 1]) for i in range(1, len(dt))) if len(dt) > 1 else 0))
    for l in re.findall(r'speedfix: ab \d+ \(.*', log): print(l[:200])
    for l in re.findall(r'speedfix: pace_adaptive: .*', log)[-3:]: print(l[:200])
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1], sys.argv[2]))
