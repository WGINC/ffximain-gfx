"""pictures.py -- the pictures of an alternating trace (xikit trace_pictures=K) compared, pixel by pixel.

    python3 pictures.py DIR_OR_FILES... [--tol 2] [--out PREFIX]

Each .pic is one frame's back buffer as the game finished it (xikit_trace_<pid>_<n>_f<frame>_l<label>.pic; label 0
swaps off, 1 on). A swapped frame lies between two unswapped ones a frame apart; anything the scene animates
changes between those two as well. So, per pixel and colour channel, a swapped frame's value is checked against
the range its two neighbours span (widened by --tol, default 2, for dithering and rounding):
  * outside the range  -> the swapped frame drew something its neighbours did not, on either side of it;
  * inside             -> consistent with the scene's own change.
The same test on an unswapped frame against the unswapped frames two either side (twice as far apart, so the scene
changes more) is the control: what the test reports when nothing was swapped. Per swapped frame: the share of
pixels outside, the bounding box of where they are, and (with --out) a picture: the frame, darkened, with the
pixels outside its neighbours' range in red."""
import argparse, glob, os, re, struct, sys
import numpy as np


def load(path):
    b = open(path, 'rb').read()
    magic, ver, w, h, fmt, bpp, frame, label = struct.unpack_from('<8I', b, 0)
    if magic != 0x43495058: raise ValueError('%s: not a trace picture' % path)
    raw = np.frombuffer(b, dtype=np.uint8, offset=32, count=w * h * bpp)
    if bpp == 4:
        px = raw.reshape(h, w, 4)[:, :, [2, 1, 0]].astype(np.int16)                 # B G R x -> R G B
    elif fmt == 23:                                                                   # R5G6B5
        v = raw.view('<u2').reshape(h, w).astype(np.int32)
        px = np.stack([(v >> 11 & 31) * 255 // 31, (v >> 5 & 63) * 255 // 63, (v & 31) * 255 // 31], -1).astype(np.int16)
    elif fmt in (24, 25):                                                             # X1R5G5B5, A1R5G5B5
        v = raw.view('<u2').reshape(h, w).astype(np.int32)
        px = np.stack([(v >> 10 & 31) * 255 // 31, (v >> 5 & 31) * 255 // 31, (v & 31) * 255 // 31], -1).astype(np.int16)
    elif fmt == 26:                                                                   # A4R4G4B4
        v = raw.view('<u2').reshape(h, w).astype(np.int32)
        px = np.stack([(v >> 8 & 15) * 17, (v >> 4 & 15) * 17, (v & 15) * 17], -1).astype(np.int16)
    else:
        raise ValueError('%s: format %d not read' % (path, fmt))
    return frame, label, px


def outside(p, q, r, tol):
    lo = np.minimum(p, r) - tol; hi = np.maximum(p, r) + tol
    return ((q < lo) | (q > hi)).any(axis=-1)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('paths', nargs='+'); ap.add_argument('--tol', type=int, default=2); ap.add_argument('--out', default=None)
    o = ap.parse_args()
    files = []
    for p in o.paths: files += sorted(glob.glob(os.path.join(p, '*.pic'))) if os.path.isdir(p) else [p]
    fr = {}
    for f in files:
        k, lab, px = load(f); fr[k] = (lab, px)
    if not fr: sys.exit('no pictures')
    print('frames: %s (label 0 swaps off, 1 on, 2 mixed)' % ' '.join('%d:%d' % (k, fr[k][0]) for k in sorted(fr)))
    def report(kind, a, k, b):
        m = outside(fr[a][1], fr[k][1], fr[b][1], o.tol); n = int(m.sum()); tot = m.size
        box = ''
        if n:
            ys, xs = np.nonzero(m); box = ' at x %d..%d, y %d..%d' % (xs.min(), xs.max(), ys.min(), ys.max())
        print('  %-8s frame %2d against %d and %d: %d of %d pixels (%.4f%%) outside%s' % (kind, k, a, b, n, tot, 100.0 * n / tot, box))
        if o.out and n:
            from PIL import Image
            img = (fr[k][1] // 3).astype(np.uint8); img[m] = (255, 0, 0)
            Image.fromarray(img).save('%s_%s_f%02d.png' % (o.out, kind, k))
        return n
    sw = [k for k in sorted(fr) if fr[k][0] == 1 and fr.get(k - 1, (None,))[0] == 0 and fr.get(k + 1, (None,))[0] == 0]
    ct = [k for k in sorted(fr) if fr[k][0] == 0 and fr.get(k - 2, (None,))[0] == 0 and fr.get(k + 2, (None,))[0] == 0]
    print('swapped frames between two unswapped ones:')
    s = [report('swapped', k - 1, k, k + 1) for k in sw]
    print('control (an unswapped frame against the unswapped frames two either side):')
    c = [report('control', k - 2, k, k + 2) for k in ct]
    if not sw: print('  none (no swapped frame has both neighbours among the pictures)')
    if not ct: print('  none')
    if sw and ct:
        print('verdict: %s' % ('the swapped frames show pixels outside their neighbours\' range beyond what the control shows'
                               if min(s) > max(c) else 'no swapped frame differs from its neighbours more than the control does'))


if __name__ == '__main__':
    main()
