"""cmpvariants.py DIR: run_variants.bat's results: each Direct3D 12 variant against Direct3D 11, the scenes and cases
where more than 50 of the 4,096 pixels differ (the ties and the rounding seen under Wine stay under that)."""
import sys, os
D = sys.argv[1]
def load(f):
    o = {}
    if not os.path.exists(f): return o
    for l in open(f, errors='replace'):
        if l.startswith('scene '):
            p = l.split(' ', 2); hx = p[2].strip()
            if hx and all(c in '0123456789abcdefABCDEF' for c in hx): o[p[1]] = [int(hx[i:i + 6], 16) for i in range(0, len(hx), 6)]
    return o
for kind in ('scenes', 'random'):
    ref = load(os.path.join(D, 'v_d3d11_%s.txt' % kind))
    print('%s (%d drawn by d3d11):' % (kind, len(ref)))
    for v in ('base', 'sm4', 'smpdirect', 'nocheck', 'fresh', 'sync'):
        x = load(os.path.join(D, 'v_%s_%s.txt' % (v, kind)))
        bad = [n for n in ref if n not in x or sum(1 for a, b in zip(ref[n], x[n]) if a != b) > 50]
        print('   %-10s %3d drawn, %3d differ: %s' % (v, len(x), len(bad), ' '.join(bad[:12])))
