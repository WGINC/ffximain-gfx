"""cmpcheck.py DIR: the D3D11 and D3D12 runs of run_check.bat compared scene by scene. A pixel that differs and is not
marked by the D3D12 ties run (magenta: a point sample within 0.01 texel of a texel border) is a real difference."""
import sys, os, glob
D = sys.argv[1]
def load(pat):
    out = {}
    for f in sorted(glob.glob(os.path.join(D, pat))):
        for l in open(f, errors='replace'):
            if l.startswith('scene '):
                p = l.split(' ', 2)
                if len(p) < 3: continue
                hx = p[2].strip()
                if hx == 'none' or any(c not in '0123456789abcdefABCDEF' for c in hx): out[p[1]] = None; continue
                out[p[1]] = [int(hx[i:i + 6], 16) for i in range(0, len(hx), 6)]
    return out
res = {}
for kind in ('scenes', 'random?'):
    a, b, t = load('out_d3d11_%s.txt' % kind), load('out_d3d12_%s.txt' % kind), load('out_d3d12_%s_ties.txt' % kind)
    same = tie = 0; bad = []
    for n in sorted(set(a) | set(b)):
        x, y, z = a.get(n), b.get(n), t.get(n)
        if x is None or y is None or len(x) != len(y): bad.append((n, 'no picture from ' + ('d3d11' if x is None else 'd3d12'), 0)); continue
        if x == y: same += 1; continue
        um = [i for i in range(len(x)) if x[i] != y[i] and not (z and z[i] == 0xFF00FF)]
        if not um: tie += 1; continue
        worst = max(max(abs(((x[i] >> s) & 255) - ((y[i] >> s) & 255)) for s in (0, 8, 16)) for i in um)
        bad.append((n, '%d pixels differ (not ties), worst channel %d' % (len(um), worst), len(um)))
    print('%s: %d compared; %d identical, %d differ only at ties, %d differ' % (kind.rstrip('?'), len(set(a) | set(b)), same, tie, len(bad)))
    for n, why, k in sorted(bad, key=lambda r: -r[2])[:40]: print('   %-34s %s' % (n, why))
