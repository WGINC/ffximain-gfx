"""trace_diff.py -- compare the two halves of an xikit A/B trace (xikit_trace_<pid>.txt, Ctrl+Alt+F8 in game):
the same Direct3D calls recorded with every swap off (label 0) and with the swaps on (label 1).

A call is the token (method, call site, its parameters, the hash of the structure or vertex data a pointer
parameter points at). The scene is not frozen between the two halves (animation, a camera drifting), so the two
frames of one half are compared with each other first: that difference is the scene's own and is subtracted. What
is left, per (method, call site, parameter values), is a count that changed when the swaps went on.

    python3 trace_diff.py xikit_trace_1234.txt [--manifest xikit_manifest.txt] [--top 40]

The call site is the analysed address (0x10xxxxxx) of the return address; --manifest names it with the swapped
function that contains it (RVA of the function's entry <= site, nearest below), which is a naming aid and no
proof the site is inside that function. Nothing here says which side is right: it says where the two differ."""
import argparse, collections, re, sys


# parameters that are addresses of the data the hash covers: a different run of the same call has a different address
# (stack, heap), so they are left out of the token whenever the hash stands for what they point at
PTR_ARG = {'SetTransform': (1,), 'MultiplyTransform': (1,), 'SetViewport': (0,), 'SetMaterial': (0,), 'SetLight': (1,),
           'SetClipPlane': (1,), 'SetVertexShaderConstant': (1,), 'SetPixelShaderConstant': (1,),
           'DrawPrimitiveUP': (2,), 'DrawIndexedPrimitiveUP': (4,)}


def parse(path, by_site=False):
    frames = collections.defaultdict(list)          # (label, frame) -> [token]
    meta = []
    for l in open(path):
        if l.startswith('#'): meta.append(l.rstrip()); continue
        p = l.split()
        if len(p) < 7: continue
        label, frame, slot, name, site, np_ = int(p[0]), int(p[1]), int(p[2]), p[3], int(p[4], 16), int(p[5])
        args = tuple(p[6:6 + min(np_, 6)]); h = p[6 + min(np_, 6)]
        args = tuple(a for i, a in enumerate(args) if i not in PTR_ARG.get(name, ()))
        frames[(label, frame)].append((name, site if by_site else 0, args, h))
    return frames, meta


def names(manifest):
    if not manifest: return []
    out = []
    for l in open(manifest):
        if l[0] in '#!' or not l.strip(): continue
        p = l.split()
        out.append((0x10000000 + int(p[0], 16), p[-1]))
    return sorted(out)


def owner(tab, site):
    best = None
    for a, n in tab:
        if a <= site: best = (a, n)
        else: break
    return '%s+0x%X' % (best[1], site - best[0]) if best else ''


def multiset(tokens):
    return collections.Counter(tokens)


def delta(a, b):
    """tokens whose counts differ: {token: (count in a, count in b)}"""
    return {t: (a.get(t, 0), b.get(t, 0)) for t in set(a) | set(b) if a.get(t, 0) != b.get(t, 0)}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('trace'); ap.add_argument('--manifest'); ap.add_argument('--by-site', action='store_true', help='keep the call site in the token (sites inside the reconstruction are addresses of the reconstruction, not of the game: a swapped run then differs at every swapped function)'); ap.add_argument('--top', type=int, default=40)
    o = ap.parse_args()
    frames, meta = parse(o.trace, o.by_site); tab = names(o.manifest)
    for m in meta[:3]: print(m)
    A = sorted(f for l, f in frames if l == 0); B = sorted(f for l, f in frames if l == 1)
    print('frames: %d without swaps, %d with; calls per frame %s | %s' % (
        len(A), len(B), [len(frames[(0, f)]) for f in A], [len(frames[(1, f)]) for f in B]))
    if len(A) < 2 or not B: sys.exit('need at least two frames without swaps (their difference is the scene\'s own) and one with')
    base = [multiset(frames[(0, f)]) for f in A]; swp = [multiset(frames[(1, f)]) for f in B]
    # the scene's own change: the tokens that differ between any two frames without swaps
    noise = set()
    for i in range(len(base)):
        for j in range(i + 1, len(base)): noise |= set(delta(base[i], base[j]))
    # a swapped frame is compared with every unswapped frame; a token counts as changed only if its count is outside
    # the range the unswapped frames show for it (min..max over A) in every swapped frame
    changed = {}
    for t in set().union(*[set(c) for c in base + swp]):
        lo = min(c.get(t, 0) for c in base); hi = max(c.get(t, 0) for c in base)
        outs = [c.get(t, 0) for c in swp if not lo <= c.get(t, 0) <= hi]
        if len(outs) == len(swp): changed[t] = (lo, hi, [c.get(t, 0) for c in swp])
    print('tokens: %d distinct; %d differ between frames without swaps (the scene\'s own); %d change when swaps go on\n' % (
        len(set().union(*[set(c) for c in base + swp])), len(noise), len(changed)))
    # by call site and method: how the counts moved
    by = collections.defaultdict(lambda: [0, 0, []])
    for t, (lo, hi, got) in changed.items():
        k = (t[1], t[0]); by[k][0] += (lo + hi) / 2; by[k][1] += sum(got) / len(got); by[k][2].append((t[2], t[3], lo, hi, got))
    rows = sorted(by.items(), key=lambda kv: -sum(abs(x[2][0] - (x[2][1] + x[3]) / 2) for x in kv[1][2]) if False else -len(kv[1][2]))
    print('call sites whose calls differ with swaps on (method, site, owner; per frame calls off -> on; distinct tokens changed):')
    for (site, name), (off, on, toks) in rows[:o.top]:
        print('  %-26s 0x%08X %-40s %7.1f -> %7.1f  tokens %d' % (name, site, (owner(tab, site) if site else '(content only)'), off, on, len(toks)))
        for args, h, lo, hi, got in toks[:3]:
            print('       args %s hash %s : off %d..%d, on %s' % (' '.join(args), h, lo, hi, got))
    if not changed: print('no call differs beyond what the scene itself changes: the swaps drew the same calls')


if __name__ == '__main__':
    main()
