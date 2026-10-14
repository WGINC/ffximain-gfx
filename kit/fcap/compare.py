"""compare.py -- a replayed frame against the picture the capture took (kit/fcap/replay.c writes both).

    python3 compare.py A.pic B.pic [--tol 8] [--png OUT.png]

A pixel differs when a colour channel differs by more than --tol (default 8, the d3d8on11 picture test's criterion: a
choice of this test, not a fact about either device). Prints the share of pixels that differ, the worst channel
difference and the bounding box of the differing pixels; with --png, the two pictures and the difference side by side.
Exit status 0 when at most --maxfrac (default 0.01) of the pixels differ."""
import argparse, os, struct, sys, zlib
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'd3dprof'))
import numpy as np
from pictures import load


def png(path, rows):
    h, w = rows.shape[:2]
    raw = b''.join(b'\0' + rows[y].astype(np.uint8).tobytes() for y in range(h))
    def chunk(t, d): return struct.pack('>I', len(d)) + t + d + struct.pack('>I', zlib.crc32(t + d))
    open(path, 'wb').write(b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, 2, 0, 0, 0)) +
                           chunk(b'IDAT', zlib.compress(raw)) + chunk(b'IEND', b''))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('a'); ap.add_argument('b'); ap.add_argument('--tol', type=int, default=8)
    ap.add_argument('--maxfrac', type=float, default=0.01); ap.add_argument('--png')
    a = ap.parse_args()
    _, _, pa = load(a.a); _, _, pb = load(a.b)
    if pa.shape != pb.shape: print('sizes differ: %s and %s' % (pa.shape, pb.shape)); sys.exit(1)
    d = np.abs(pa.astype(np.int32) - pb.astype(np.int32)).max(axis=2)
    bad = d > a.tol; n = int(bad.sum()); frac = n / bad.size
    ys, xs = np.nonzero(bad)
    box = 'x %d..%d, y %d..%d' % (xs.min(), xs.max(), ys.min(), ys.max()) if n else '-'
    print('%d of %d pixels differ (%.3f%%), worst channel difference %d, at %s' % (n, bad.size, 100 * frac, int(d.max()), box))
    if a.png:
        diff = np.zeros_like(pa); diff[..., 0] = np.where(bad, 255, 0); diff[..., 1] = np.where(bad, 0, np.minimum(d * 8, 255))
        gap = np.full((pa.shape[0], 4, 3), 64, dtype=pa.dtype)
        png(a.png, np.concatenate([pa, gap, pb, gap, diff], axis=1))
    sys.exit(0 if frac <= a.maxfrac else 1)


if __name__ == '__main__':
    main()
