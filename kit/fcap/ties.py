"""ties.py -- how much of a captured frame point-samples on a texel boundary, where D3D11 lets the driver read either texel.

    python3 ties.py CAPTURE.fcap [--eps 0.004] [--png OUT.png]     (after kit/d3d8on11/build.sh and kit/fcap/build.sh)

Replays the capture twice through kit/d3d8on11 under Wine (a virtual X server): once as it is, once with the test
switch D8ON11_TEST_TIES=eps, which draws magenta where a point-filtered 2D stage samples within eps texels of a texel
boundary (kit/d3d8on11/d8_exec.cpp gen_ps; README.md). A pixel counts when the two replays differ there: the marker
reached the finished picture. Why it matters: D3D11 converts the scaled coordinate to "at least 16.8 Fixed Point" and
takes its integer part (D3D11.3 functional spec 7.18.7), with a conversion tolerance (3.2.4.1), so within about 1/256
texel of a boundary two drivers may read different texels; on the owner's machine they did (docs/HISTORY.md, 15 Oct).

The count is an upper bound on what a driver may draw differently, not a count of differences:
- a coordinate exactly on a boundary is read the same by every driver when it arrives exact (t8 rhw_texture: all 1024
  pixels marked, none differed between the owner's GPU, WARP, Wine and the reference device);
- a marked pixel that a later draw blends over still counts, and a render target read later as a texture carries the
  marker on;
- the mark uses ours' own LOD to choose MINFILTER or MAGFILTER (the spec's isotropic formula, 7.18.11), not the
  driver's, so near LOD 0 it may test the other filter.
Prints the count, its share of the picture and its bounding box; with --png, the picture, the marked replay and the
counted pixels side by side."""
import argparse, os, shutil, subprocess, sys, tempfile
F = os.path.dirname(os.path.abspath(__file__)); K = os.path.join(F, '..')
sys.path.insert(0, F); sys.path.insert(0, os.path.join(K, 'd3dprof'))
import numpy as np
from pictures import load
from compare import png


def replay(cap, d, extra):
    os.makedirs(d)
    shutil.copy(os.path.join(F, 'dist', 'fcap_replay.exe'), d); shutil.copy(cap, d)
    shutil.copy(os.path.join(K, 'd3d8on11', 'dist', 'd3d8.dll'), d)
    env = dict(os.environ, WINEDEBUG='-all', WINEDLLOVERRIDES='d3d8=n', D8ON11_MODE='direct', **extra)
    r = subprocess.run(['xvfb-run', '-a', '-s', '-screen 0 1024x768x24', 'wine', 'fcap_replay.exe', os.path.basename(cap), 'r'],
                       cwd=d, capture_output=True, text=True, timeout=1800, env=env)
    out = '\n'.join(l for l in r.stdout.splitlines() if 'X connection' not in l)
    pic = os.path.join(d, 'r_replay.pic')
    if not os.path.exists(pic): sys.exit('replay wrote no picture:\n' + out)
    if ' 0 failures' not in out: print('note: the replay reported failed calls:\n' + out)
    return load(pic)[2]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('capture'); ap.add_argument('--eps', type=float, default=1 / 256); ap.add_argument('--png')
    a = ap.parse_args()
    w = tempfile.mkdtemp(prefix='ties_')
    try:
        off = replay(os.path.abspath(a.capture), os.path.join(w, 'off'), {})
        on = replay(os.path.abspath(a.capture), os.path.join(w, 'on'), {'D8ON11_TEST_TIES': repr(a.eps)})
    finally:
        if not os.environ.get('KEEP'): shutil.rmtree(w, ignore_errors=True)
        else: print('kept', w)
    if off.shape != on.shape: sys.exit('the two replays differ in size')
    m = (off != on).any(axis=2); n = int(m.sum())
    ys, xs = np.nonzero(m)
    box = 'x %d..%d, y %d..%d' % (xs.min(), xs.max(), ys.min(), ys.max()) if n else '-'
    print('%d of %d pixels (%.3f%%) show a point sample within %g texels of a texel boundary, at %s'
          % (n, m.size, 100 * n / m.size, a.eps, box))
    if a.png:
        mask = np.zeros_like(off); mask[..., 0] = np.where(m, 255, 0); mask[..., 2] = np.where(m, 255, 0)
        gap = np.full((off.shape[0], 4, 3), 64, dtype=off.dtype)
        png(a.png, np.concatenate([off, gap, on, gap, mask], axis=1))


if __name__ == '__main__':
    main()
