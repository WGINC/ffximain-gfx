"""read_smooth.py -- the 26 Nov session: microstutter and what reaches the screen, per mark of one start.

    python3 read_smooth.py <start folder in xikit_logs> <presentmon csv>

Per mark (mark.bat; the kit's frame log has the mark number and the qpc, PresentMon's CPUStartQPC the same counter):
  frames      mean frame time, p95, p99;
  spikes      frames over twice the median of the 60 frames around them and over 16 ms, a minute (the 25 Nov measure:
              2 buffers 46-187 a minute, 3 buffers 21);
  on screen   PresentMon DisplayedTime: the share of shown frames held one refresh, two, more (the refresh taken as the
              most common shown time); presents never shown; shown frames a second."""
import collections, csv, glob, os, re, statistics as st, sys


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
    spike = [i for i in range(30, len(ft) - 30) if ft[i] > 16 and ft[i] > 2 * sorted(ft[i - 30:i + 30])[30]]
    pm = list(csv.DictReader(open(pm_path, encoding='utf-8-sig', errors='replace')))
    shown = [float(r['DisplayedTime']) for r in pm if r['DisplayedTime'] not in ('NA', '')]
    refresh = collections.Counter(round(x * 4) / 4 for x in shown).most_common(1)[0][0] if shown else 0
    print('refresh (the most common on-screen time): %.2f ms' % refresh)
    for m in sorted(set(mk)):
        idx = [i for i in range(len(rows)) if mk[i] == m]
        if len(idx) < 120: continue
        f = [ft[i] for i in idx]; q0, q1 = q[idx[0]], q[idx[-1]]; secs = (q1 - q0) / qpf
        ns = sum(1 for i in spike if idx[0] <= i <= idx[-1])
        pr = [r for r in pm if q0 <= int(r['CPUStartQPC']) <= q1]
        dt = [float(r['DisplayedTime']) for r in pr if r['DisplayedTime'] not in ('NA', '')]
        never = sum(1 for r in pr if r['DisplayedTime'] in ('NA', ''))
        one = sum(1 for x in dt if x < 1.5 * refresh) / max(len(dt), 1); two = sum(1 for x in dt if 1.5 * refresh <= x < 2.5 * refresh) / max(len(dt), 1)
        print('mark %d %-10s %4.0f s: frame mean %5.2f ms p95 %5.2f p99 %5.2f | spikes %5.1f a minute | shown: one refresh %5.1f%%, two %4.1f%%, more %4.1f%%, never shown %4.1f%%, %5.1f a second' % (
            m, (label.get(m, '(start)') or '')[:10], secs, st.mean(f), pct(f, 95), pct(f, 99), 60 * ns / max(secs, 1),
            100 * one, 100 * two, 100 * (1 - one - two) if dt else 0, 100 * never / max(len(pr), 1), 1000 * len(dt) / max(sum(dt), 1)))
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1], sys.argv[2]))
