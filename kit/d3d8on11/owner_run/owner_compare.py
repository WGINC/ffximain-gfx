"""owner_compare.py FOLDER -- reads a t8run_... folder from run_t8.bat (README.md) and compares each implementation's
pictures with ours (ours_direct). The criterion is the one test_d3d8on11.py uses against Microsoft's reference device:
a pixel matches within TOL of the other's pixel or, both ways, of one of its 3x3 neighbours (a pixel centre on an edge,
or a sample on a texel boundary, is a tie each implementation's rounding decides). Prints, per implementation:
- where: the d3d8.dll loaded, its version, the adapter;
- basic: its result lines that are not "ok";
- defaults: the states whose values differ from ours;
- scenes: per scene, the pixels with no match; for alphaop_disabled_stage0/1 also what alpha the picture implies;
- random: per case file, the cases with unmatched pixels and their classes (random_cases.py: A and C read texture
  coordinates the vertices do not supply, which no documentation defines; S filters a cube texture linearly; L lights
  with unnormalized normals and NORMALIZENORMALS off; F has linear fog with FOGSTART >= FOGEND; W has table fog on pre-
  transformed vertices; D has a stage with its colour op on and its alpha op DISABLE, undefined by the DirectX 8.1 text;
  N has a stage reading a texture it was not given; P filters a texture other than by point, for which the DirectX 8.1
  text gives no precision). "-" is a case of none of these. A few unmatched pixels in a point-filtered case can be the
  reference device's point sampling at a texel boundary ("Nearest-Point Sampling", DirectX 8.1: the reference device
  "does not perform this operation with perfect accuracy"); ours' test switch D8ON11_TEST_TEXEDGE (README.md) tells
  those apart.

    python3 owner_compare.py t8run_20261015_101010"""
import os, sys
H = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, os.path.join(H, '..', 'test'))
from random_cases import classes

TOL = 3


def pictures(path):
    out = {}
    if not os.path.exists(path): return out
    for l in open(path, errors='replace'):
        if l.startswith('scene '):
            _, n, hx = l.split(' ', 2); hx = hx.strip()
            out[n] = None if hx == 'none' else [int(hx[i:i + 6], 16) for i in range(0, len(hx), 6)]
    return out


def near(x, y): return all(abs(((x >> s) & 255) - ((y >> s) & 255)) <= TOL for s in (0, 8, 16))


def unmatched(a, b):
    bad = 0
    for i in range(len(a)):
        if near(a[i], b[i]): continue
        x, y = i % 64, i // 64
        nb = [(x + dx) + 64 * (y + dy) for dx in (-1, 0, 1) for dy in (-1, 0, 1) if 0 <= x + dx < 64 and 0 <= y + dy < 64]
        if not (any(near(a[i], b[j]) for j in nb) and any(near(b[i], a[j]) for j in nb)): bad += 1
    return bad


def alpha_of(pic, x, y, src, bg=0x203040):
    """the alpha a pixel implies for SRCALPHA / INVSRCALPHA of colour src onto bg, from its red channel"""
    p, s, b = (pic[y * 64 + x] >> 16) & 255, (src >> 16) & 255, (bg >> 16) & 255
    return (p - b) / (s - b) if s != b else float('nan')


def main(folder):
    impls = [d for d in ('dgv', 'win', 'ours_direct', 'ours_remote', 'ours_warp', 'ours_debug') if os.path.isdir(os.path.join(folder, d))]
    base = os.path.join(folder, 'ours_direct')
    ours_sc = pictures(os.path.join(base, 'scenes.txt'))
    ours_def = dict(l.rsplit(' ', 1) for l in open(os.path.join(base, 'defaults.txt'), errors='replace') if l[:3] in ('rs ', 'tss')) \
        if os.path.exists(os.path.join(base, 'defaults.txt')) else {}
    cases = {}
    for s in (1, 2, 3):
        p = os.path.join(base, 't8_cases%d.txt' % s)
        if os.path.exists(p):
            for blk in open(p).read().split('case ')[1:]:
                ls = ('case ' + blk).strip().splitlines(); cases[(s, ls[0].split()[1])] = ls
    for d in impls:
        p = os.path.join(folder, d)
        print('==== %s' % d)
        for f in ('where.txt',):
            if os.path.exists(os.path.join(p, f)): print(open(os.path.join(p, f), errors='replace').read().strip())
        bl = [l.strip() for l in open(os.path.join(p, 'basic.txt'), errors='replace')] if os.path.exists(os.path.join(p, 'basic.txt')) else ['(no basic.txt)']
        print('basic: %d lines, not ok: %s' % (len(bl), [l for l in bl if l and not l.startswith('ok')][:12]))
        if d == 'ours_direct': continue
        df = dict(l.rsplit(' ', 1) for l in open(os.path.join(p, 'defaults.txt'), errors='replace') if l[:3] in ('rs ', 'tss')) \
            if os.path.exists(os.path.join(p, 'defaults.txt')) else {}
        # as test_d3d8on11.py compares Microsoft's runtime: render states up to 173 (the last D3D8 one) but 153
        # (SOFTWAREVERTEXPROCESSING, a device setting), and not those an implementation answers 0xBAADCAFE (never set)
        dx8 = lambda k: not k.startswith('rs ') or (int(k.split()[1]) <= 173 and int(k.split()[1]) != 153)
        unset = [k for k in df if df[k].strip() == 'BAADCAFE']
        dd = sorted((k for k in set(df) | set(ours_def) if dx8(k) and k not in unset and df.get(k) != ours_def.get(k)),
                    key=lambda k: (k[0], [int(x) for x in k.split()[1:]]))
        fails = [k for k in dd if k not in df]; dd = [k for k in dd if k in df]
        print('defaults: %d states read, %d answered BAADCAFE (never set), %d D3D8 states not answered (ours answers them): %s' % (
            len(df), len(unset), len(fails), ' '.join(k.split()[-1] if k.startswith('rs ') else '(%s)' % k for k in fails)))
        print('   %d D3D8 states differ from ours (theirs/ours): %s' % (
            len(dd), ', '.join('%s %s/%s' % (k, df[k].strip(), ours_def.get(k, '-').strip()) for k in dd)))
        sc = pictures(os.path.join(p, 'scenes.txt'))
        print('scenes (pixels with no match in ours):')
        for n in ours_sc:
            a, b = sc.get(n), ours_sc[n]
            if not a or not b: print('   %-32s no picture from %s' % (n, d if not a else 'ours')); continue
            u = unmatched(a, b); extra = ''
            if n.startswith('alphaop_disabled'):
                src = 0xFFC200 if n.endswith('1') else 0xFF6000      # the colour the stages make where alpha 1 shows it
                extra = '  implied alpha at (20,20) %.2f, (50,50) %.2f (ours %.2f, %.2f)' % (
                    alpha_of(a, 20, 20, src), alpha_of(a, 50, 50, src), alpha_of(b, 20, 20, src), alpha_of(b, 50, 50, src))
            print('   %-32s %4d%s' % (n, u, extra))
        for s in (1, 2, 3):
            ra, rb = pictures(os.path.join(p, 'random%d.txt' % s)), pictures(os.path.join(base, 'random%d.txt' % s))
            diff = []
            for (ss, n), ls in sorted(cases.items()):
                if ss != s: continue
                a, b = ra.get(n), rb.get(n)
                if not a or not b: diff.append('%s (no picture)' % n); continue
                u = unmatched(a, b)
                if u:
                    cls = classes(ls, ref=True)
                    diff.append('%s %d px %s' % (n, u, cls or '-'))
            print('random seed %d: %d of %d cases differ from ours: %s' % (s, len(diff), sum(1 for k in cases if k[0] == s), '; '.join(diff)))


if __name__ == '__main__':
    main(sys.argv[1])
