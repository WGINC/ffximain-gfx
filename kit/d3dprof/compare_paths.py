"""compare_paths.py -- two d3dprof profiles (profile=1 call_time=1) of the same place in the game, taken through two
Direct3D 8 paths (dgVoodoo with one output API, another, Windows' own d3d8.dll, ...), side by side.

    python3 compare_paths.py A.csv B.csv [--names A_name B_name]

Per method: calls a frame and the time inside each call (the layers below the profiler: the Direct3D 8
implementation, the runtime and driver under it, and the GPU work a call waits for), and the time a frame in all.
The frame time of each window is in the profile's header. What decides is the frame time; the per-call times say
where a path is cheaper or dearer. Two windows of different scenes do not compare: take both standing at the same spot,
same time of day if the zone has one, after the frame time has settled. The calls a frame should then agree too (the
same game, the same scene); the table shows them for that check."""
import argparse, collections, csv, re, sys


def load(path):
    head = next(l for l in open(path) if l.startswith('# d3dprof'))
    m = re.search(r'frame ms avg ([\d.]+)', head)
    by = collections.defaultdict(lambda: [0.0, 0.0])
    for r in csv.DictReader(l for l in open(path) if not l.startswith('#')):
        if r.get('caller') == 'caller of CDx draw wrapper': continue          # (the same draws again, by their caller)
        c = float(r['calls_per_frame'] or 0); t = float(r.get('us_per_frame') or 0)
        by[r['method']][0] += c; by[r['method']][1] += t
    return float(m.group(1)) if m else None, by


def main():
    ap = argparse.ArgumentParser(); ap.add_argument('a'); ap.add_argument('b'); ap.add_argument('--names', nargs=2, default=['A', 'B'])
    o = ap.parse_args()
    fa, A = load(o.a); fb, B = load(o.b)
    na, nb = o.names
    print('frame time: %s %.2f ms, %s %.2f ms (%+.1f%%)' % (na, fa, nb, fb, 100 * (fb - fa) / fa if fa else 0))
    ta = sum(v[1] for v in A.values()) / 1000; tb = sum(v[1] for v in B.values()) / 1000
    print('time inside Direct3D calls a frame: %s %.2f ms, %s %.2f ms' % (na, ta, nb, tb))
    print('%-26s %10s %10s %9s %9s %9s %9s' % ('method', 'calls ' + na, 'calls ' + nb, 'us ' + na, 'us ' + nb, 'ms/f ' + na, 'ms/f ' + nb))
    for m in sorted(set(A) | set(B), key=lambda k: -max(A[k][1], B[k][1])):
        (ca, ua), (cb, ub) = A[m], B[m]
        if max(ua, ub) < 50: continue                        # under 0.05 ms a frame on both
        print('%-26s %10.1f %10.1f %9.3f %9.3f %9.3f %9.3f' % (m, ca, cb, ua / ca if ca else 0, ub / cb if cb else 0, ua / 1000, ub / 1000))


if __name__ == '__main__':
    main()
