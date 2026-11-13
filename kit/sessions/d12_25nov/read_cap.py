"""read_cap.py -- the 25 Nov session's A/B per phase: frame times (xikit frames) and what reached the screen (PresentMon).

    python3 read_cap.py <start folder in xikit_logs> <presentmon csv>

speedfix's A/B (ab.bat) writes a mark at each phase ("ab 3 B") and at the end ("ab end"): the kit logs each as
"measure: mark N at frame F (mark.bat): ab 3 B" and puts N in the frame log's mark column, with the frame's qpc.
PresentMon's CPUStartQPC is the same performance counter, so each phase's presents are those between its mark's qpc
and the next one's. The first ab_settle frames of each phase (60) are left out of both, as speedfix leaves them out.
Per phase and per label (A: the cap off, B: on):
  frame time  mean, p95, p99, and the mean change from one frame to the next (xikit frames);
  on screen   PresentMon DisplayedTime: the share of shown frames held one refresh, two, three or more (the refresh
              taken as the most common shown time), the presents never shown, shown frames a second, and the mean
              change in on-screen time from one shown frame to the next."""
import collections, csv, glob, os, re, statistics as st, sys

SETTLE = 60


def pct(a, p):
    a = sorted(a)
    return a[min(len(a) - 1, int(p / 100 * len(a)))] if a else float('nan')


def main(folder, pm_path):
    log = open(glob.glob(os.path.join(folder, 'xikit.log'))[0], errors='replace').read()
    label = {int(n): t.strip() for n, t in re.findall(r'measure: mark (\d+) at frame \d+ \(mark\.bat\): (.*)', log)}
    frames, hdr = [], None
    for l in open(glob.glob(os.path.join(folder, 'xikit_frames_*.csv'))[0], errors='replace'):
        if l.startswith('frame,'): hdr = l.strip().split(','); continue
        if hdr and l[:1].isdigit(): frames.append(dict(zip(hdr, l.rstrip('\r\n').split(','))))
    # the phases: from each 'ab N X' mark's first frame to the next mark's
    starts = []
    for r in frames:
        m = int(r['mark'] or 0)
        if m and (not starts or starts[-1][0] != m): starts.append((m, int(r['qpc'])))
    phases = []
    for i, (m, q) in enumerate(starts):
        t = label.get(m, '')
        mm = re.match(r'ab (\d+) ([AB])$', t)
        if mm: phases.append((int(mm.group(1)), mm.group(2), q, starts[i + 1][1] if i + 1 < len(starts) else 1 << 62))
    if not phases: print('no "ab N A/B" marks in %s' % folder); return 1
    pm = list(csv.DictReader(open(pm_path, encoding='utf-8-sig', errors='replace')))
    shown_all = [float(r['DisplayedTime']) for r in pm if r['DisplayedTime'] not in ('NA', '')]
    refresh = collections.Counter(round(x * 4) / 4 for x in shown_all).most_common(1)[0][0] if shown_all else 0
    print('refresh (the most common on-screen time): %.2f ms' % refresh)
    agg = {'A': collections.defaultdict(list), 'B': collections.defaultdict(list)}
    for n, lab, q0, q1 in phases:
        ft = [float(r['frame_ms']) for r in frames if q0 <= int(r['qpc']) < q1][SETTLE:]
        rows = [r for r in pm if q0 <= int(r['CPUStartQPC']) < q1][SETTLE:]
        dt = [float(r['DisplayedTime']) for r in rows if r['DisplayedTime'] not in ('NA', '')]
        never = sum(1 for r in rows if r['DisplayedTime'] in ('NA', ''))
        if len(ft) < 30 or len(dt) < 30: print('phase %d %s: too few frames' % (n, lab)); continue
        one = sum(1 for x in dt if x < 1.5 * refresh) / len(dt); two = sum(1 for x in dt if 1.5 * refresh <= x < 2.5 * refresh) / len(dt)
        more = 1 - one - two
        a = agg[lab]
        a['ft'] += ft; a['dt'] += dt; a['never'].append(never); a['rows'].append(len(rows))
        print('phase %d %s: %5d frames, mean %.2f ms p95 %.2f p99 %.2f change %.2f | shown: one refresh %.1f%%, two %.1f%%, more %.1f%%, never shown %.1f%%, %.1f a second, change %.2f ms' % (
            n, lab, len(ft), st.mean(ft), pct(ft, 95), pct(ft, 99), st.mean(abs(ft[i] - ft[i - 1]) for i in range(1, len(ft))),
            100 * one, 100 * two, 100 * more, 100 * never / len(rows), 1000 * len(dt) / sum(dt), st.mean(abs(dt[i] - dt[i - 1]) for i in range(1, len(dt)))))
    for lab, name in (('A', 'the cap off'), ('B', 'the cap on (120)')):
        a = agg[lab]
        if not a['ft']: continue
        ft, dt = a['ft'], a['dt']
        one = sum(1 for x in dt if x < 1.5 * refresh) / len(dt); two = sum(1 for x in dt if 1.5 * refresh <= x < 2.5 * refresh) / len(dt)
        print('%s (%s): frame mean %.2f ms, p95 %.2f, p99 %.2f | shown: one refresh %.1f%%, two %.1f%%, more %.1f%%, never shown %.1f%%' % (
            lab, name, st.mean(ft), pct(ft, 95), pct(ft, 99), 100 * one, 100 * two, 100 * (1 - one - two), 100 * sum(a['never']) / sum(a['rows'])))
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1], sys.argv[2]))
