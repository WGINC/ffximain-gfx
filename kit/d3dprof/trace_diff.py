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
import argparse, bisect, collections, json, os, re, sys
H = os.path.dirname(os.path.abspath(__file__))


# parameters that are addresses of the data the hash covers: a different run of the same call has a different address
# (stack, heap), so they are left out of the token whenever the hash stands for what they point at
PTR_ARG = {'SetTransform': (1,), 'GetTransform': (1,), 'MultiplyTransform': (1,), 'SetViewport': (0,), 'SetMaterial': (0,), 'SetLight': (1,),
           'SetClipPlane': (1,), 'SetVertexShaderConstant': (1,), 'SetPixelShaderConstant': (1,),
           'DrawPrimitiveUP': (2,), 'DrawIndexedPrimitiveUP': (4,)}


BYTES = {}                  # token -> the bytes the call read (traces from 6 Oct on: matrices, small vertex lists)


def floats(hexdata, per_row):
    import struct
    b = bytes.fromhex(hexdata); n = len(b) // 4
    v = struct.unpack('<%dI' % n, b[:4 * n])
    def one(w):
        f = struct.unpack('<f', struct.pack('<I', w))[0]
        return '%08X' % w if (w >> 23) & 0xFF in (0, 0xFF) and w not in (0, 0x80000000) or abs(f) > 1e7 else ('%.5g' % f if w != 0x80000000 else '-0')
    return [' '.join(one(w) for w in v[i:i + per_row]) for i in range(0, n, per_row)]


def parse(path, by_site=False):
    frames = collections.defaultdict(list)          # (label, frame) -> [token]
    meta = []; bufs = []
    for l in open(path):
        if l.startswith('#'): meta.append(l.rstrip()); continue
        raw = l.split(); dat = next((x[2:] for x in raw if x.startswith('d=')), None)
        p = [x for x in raw if not x.startswith('d=')]       # (the recorded bytes: kept per token in BYTES, shown below)
        if len(p) < 7: continue
        label, frame, slot, name, site, np_ = int(p[0]), int(p[1]), int(p[2]), p[3], int(p[4], 16), int(p[5])
        args = tuple(p[6:6 + min(np_, 6)]); h = p[6 + min(np_, 6)]
        if name in ('VB_Unlock', 'IB_Unlock'):       # buffer contents: compared by the function that filled them (below)
            bufs.append((label, frame, name, site, tuple(int(x, 16) for x in args), h)); continue
        args = tuple(a for i, a in enumerate(args) if i not in PTR_ARG.get(name, ()))
        tok = (name, site if by_site else 0, args, h)
        if dat: BYTES.setdefault(tok, dat)
        frames[(label, frame)].append(tok)
    return frames, meta, bufs


def names(manifest):
    if not manifest: return []
    out = []
    for l in open(manifest):
        if l[0] in '#!' or not l.strip(): continue
        p = l.split()
        out.append((0x10000000 + int(p[0], 16), p[5]))          # (column 6: NAME; T= may follow)
    return sorted(out)


def owner(tab, site):
    best = None
    for a, n in tab:
        if a <= site: best = (a, n)
        else: break
    return '%s+0x%X' % (best[1], site - best[0]) if best else ''


class Owners:
    """the function a call site is in: a site in the game's DLL by data/func_extents.json; a site in the reconstruction
    (its load address is in the trace's header) by the nearest export at or below it in the reconstruction DLL, named
    as the manifest names that export, so both halves name the same function the same way. The export rule is a
    naming aid: code the compiler placed between exports would be named after the export before it."""
    def __init__(self, meta, recon_dll, manifest, extents):
        self.man = {}; self.exp_name = {}
        for l in open(manifest):
            if l[0] in '#!' or not l.strip(): continue
            p = l.split(); a = 0x10000000 + int(p[0], 16)
            self.man[a] = p[5]; self.exp_name[p[2]] = (a, p[5])       # (column 6: NAME; T= may follow)
        ex = json.load(open(extents))
        self.pieces = sorted((lo, hi, int(k, 16)) for k, v in ex.items() for lo, hi in v)
        self.starts = [x[0] for x in self.pieces]
        m = next((re.search(r'reconstruction at 0x([0-9A-Fa-f]+)', x) for x in meta if 'reconstruction at' in x), None)
        self.rbase = int(m.group(1), 16) if m else None
        self.exports = []
        if self.rbase and os.path.exists(recon_dll):
            import pefile
            pe = pefile.PE(recon_dll, fast_load=True); pe.parse_data_directories()
            self.rsize = pe.OPTIONAL_HEADER.SizeOfImage
            self.exports = sorted((e.address, (e.name or b'').decode()) for e in pe.DIRECTORY_ENTRY_EXPORT.symbols)
            self.eaddr = [x[0] for x in self.exports]

    def __call__(self, site):
        if 0x10000000 <= site < 0x11000000:
            i = bisect.bisect_right(self.starts, site) - 1
            while i >= 0 and self.pieces[i][0] > site - 0x100000:
                lo, hi, f = self.pieces[i]
                if lo <= site < hi: return '%s 0x%08X' % (self.man.get(f, 'fn'), f)
                i -= 1
            return 'game 0x%08X' % site
        if self.exports and self.rbase <= site < self.rbase + self.rsize:
            i = bisect.bisect_right(self.eaddr, site - self.rbase) - 1
            if i >= 0:
                a, n = self.exp_name.get(self.exports[i][1], (None, self.exports[i][1]))
                return '%s 0x%08X' % (n, a) if a else 'reconstruction %s' % n
        return 'other 0x%08X' % site


def buffer_report(bufs, owners, top):
    """per filling function: unlocks, bytes, inf/NaN-pattern and huge-float dwords per frame, and how many of the
    contents (hashes) the swapped half wrote were also written in the unswapped half; the unswapped frames compared
    with each other give what overlap the scene itself allows"""
    if not bufs: print('\nno buffer records (trace_buffers=0, or a kit before 6 Oct)'); return
    g = collections.defaultdict(lambda: {0: collections.defaultdict(list), 1: collections.defaultdict(list)})
    for label, frame, name, site, a, h in bufs:
        g[(owners(site), name)][label][frame].append((a[1], a[2], a[3], h))
    rows = []
    for k, side in g.items():
        def per(lbl, i):
            fr = side[lbl]; n = max(len(fr), 1)
            return sum(sum(x[i] for x in v) for v in fr.values()) / n
        offh = [set(x[3] for x in v) for v in side[0].values()]; onh = set(x[3] for v in side[1].values() for x in v)
        allo = set().union(*offh) if offh else set()
        inner = (sum(len(a & b) / max(len(a | b), 1) for i, a in enumerate(offh) for b in offh[i + 1:]) /
                 max(len(offh) * (len(offh) - 1) / 2, 1)) if len(offh) > 1 else None
        cross = len(onh & allo) / max(len(onh), 1) if onh else None
        unl = [len([x for v in side[l].values() for x in v]) / max(len(side[l]), 1) for l in (0, 1)]
        stats = [(per(l, 0), per(l, 1), per(l, 2)) for l in (0, 1)]
        score = abs(stats[0][1] - stats[1][1]) + abs(stats[0][2] - stats[1][2]) + abs(unl[0] - unl[1]) * 4
        if inner is not None and cross is not None: score += max(0.0, inner - cross) * 1000
        rows.append((score, k, unl, stats, inner, cross))
    rows.sort(key=lambda r: -r[0])
    print('\nbuffer contents by the function that locked them (per frame: unlocks, bytes, inf/NaN-pattern dwords, floats >= 2^20;'
          ' "same" = share of the contents also written without swaps, against what the unswapped frames share among themselves):')
    for score, (fn, name), unl, st, inner, cross in rows[:top]:
        print('  %-9s %-52s off %5.1f x %8.0f B nan %6.0f big %6.0f | on %5.1f x %8.0f B nan %6.0f big %6.0f | same %s (scene %s)' % (
            name, fn[:52], unl[0], st[0][0], st[0][1], st[0][2], unl[1], st[1][0], st[1][1], st[1][2],
            '-' if cross is None else '%3.0f%%' % (100 * cross), '-' if inner is None else '%3.0f%%' % (100 * inner)))


def multiset(tokens):
    return collections.Counter(tokens)


def delta(a, b):
    """tokens whose counts differ: {token: (count in a, count in b)}"""
    return {t: (a.get(t, 0), b.get(t, 0)) for t in set(a) | set(b) if a.get(t, 0) != b.get(t, 0)}


# the parameters that say WHICH state a call sets (render state, stage and type, transform, light index, texture
# stage, stream): calls are paired across frames by these, the rest of the parameters and the hash are the value
SELECT = {'SetRenderState': 1, 'SetTextureStageState': 2, 'SetTransform': 1, 'MultiplyTransform': 1, 'GetTransform': 1,
          'SetLight': 1, 'LightEnable': 1, 'SetTexture': 1, 'SetStreamSource': 1, 'SetVertexShaderConstant': 1,
          'SetPixelShaderConstant': 1, 'SetClipPlane': 1}


def parse_alt(path):
    """an alternating trace: {frame: (label, [(method, site, select, value, bytes)])}"""
    fr = {}; meta = []
    for l in open(path):
        if l.startswith('#'): meta.append(l.rstrip()); continue
        raw = l.split(); dat = next((x[2:] for x in raw if x.startswith('d=')), None)
        p = [x for x in raw if not x.startswith('d=')]
        if len(p) < 7: continue
        label, frame, name, site, np_ = int(p[0]), int(p[1]), p[3], int(p[4], 16), int(p[5])
        if name in ('VB_Unlock', 'IB_Unlock', 'Present'): continue
        args = p[6:6 + min(np_, 6)]; h = p[6 + min(np_, 6)]
        args = [a for i, a in enumerate(args) if i not in PTR_ARG.get(name, ())]
        k = SELECT.get(name, 0)
        e = fr.setdefault(frame, [label, []]); e[0] = label
        e[1].append((name, site, tuple(args[:k]), (tuple(args[k:]), h), dat))
    return fr, meta


def f32(hexdata):
    import struct
    b = bytes.fromhex(hexdata); return struct.unpack('<%df' % (len(b) // 4), b[:len(b) // 4 * 4])


def alternating(path, owners, top):
    """Each swapped frame (label 1) is compared with the unswapped frames just before and after it (label 0). Calls
    are paired by (method, the state it sets, which occurrence of that in the frame); not by call site, since the
    swapped frame's sites are in the reconstruction and its export names are only a naming aid (shown with a
    finding). A group is compared only where all three frames make the same number of such calls, so a scene that
    draws one more object in one frame does not shift the pairing. A swapped call is reported when:
      STABLE   its two neighbours made the call with the same value and the swapped frame with another one
               (what the scene animates differs between the neighbours too, so this cannot be animation);
      OUTSIDE  the call read bytes the trace kept (a matrix, a small vertex list) and a float of the swapped frame is
               outside the range its two neighbours span (more than 1e-5 relative): not between the frame before
               and the frame after.
    A group whose count differs in the swapped frame while both neighbours agree is reported as COUNT. Where the two
    neighbours differ and no bytes were kept (a light, a hash only), the call is undecided and only counted."""
    fr, meta = parse_alt(path)
    for m in meta[:3]: print(m)
    labels = ''.join({0: '0', 1: '1'}.get(fr[f][0], 'x') for f in sorted(fr))
    print('frames (0 swaps off, 1 on, x mixed: left out): %s' % labels)
    own = {}
    def group(frame):
        g = collections.defaultdict(list)
        for name, site, sel, val, dat in fr[frame][1]:
            g[(name, sel)].append((val, dat, site))
        return g
    def who(site):
        o = own.get(site)
        if o is None: o = own[site] = owners(site).rsplit(' 0x', 1)[0] if owners else '0x%08X' % site
        return o
    def test(f0, f, f1, found, stats):
        a, b, c = group(f0), group(f), group(f1)
        for k in set(a) | set(b) | set(c):
            na, nb, nc = len(a.get(k, ())), len(b.get(k, ())), len(c.get(k, ()))
            if na == nc and nb != na: found[('COUNT', '') + k].append((f, '%d calls before, %d in this frame, %d after' % (na, nb, nc))); continue
            if not (na == nb == nc): stats['groups whose count changes in the scene'] += 1; continue
            for i, ((va, da, _), (vb, db, sb), (vc, dc, _)) in enumerate(zip(a[k], b[k], c[k])):
                stats['calls compared'] += 1; kk = (who(sb),) + k
                if va == vb == vc: stats['same in all three'] += 1; continue
                if va == vc:
                    found[('STABLE',) + kk].append((f, 'call %d: %s | before and after %s' % (i, fmtval(vb, db), fmtval(va, da)))); continue
                if da and db and dc and len(da) == len(db) == len(dc):
                    x, y, z = f32(da), f32(db), f32(dc); bad = []
                    for j, (p, q, r) in enumerate(zip(x, y, z)):
                        lo, hi = min(p, r), max(p, r); tol = 1e-5 * max(1.0, abs(p), abs(r))
                        if q == q and p == p and r == r and not lo - tol <= q <= hi + tol: bad.append('[%d] %.6g not in %.6g..%.6g' % (j, q, p, r))
                        elif (q != q) != (p != p and r != r): bad.append('[%d] NaN %s' % (j, 'in this frame' if q != q else 'in the neighbours'))
                    if bad: found[('OUTSIDE',) + kk].append((f, 'call %d: %s' % (i, '; '.join(bad[:6]))))
                    else: stats['between its neighbours'] += 1
                else: stats['undecided (the neighbours differ, no bytes kept)'] += 1
    lab = lambda f: fr.get(f, [None])[0]
    found = collections.defaultdict(list); stats = collections.Counter(); triples = 0
    for f in sorted(fr):
        if lab(f) == 1 and lab(f - 1) == 0 and lab(f + 1) == 0: triples += 1; test(f - 1, f, f + 1, found, stats)
    # the control: the same test on unswapped frames between the unswapped frames two either side (twice as far apart,
    # so the scene changes more between them than between a swapped frame's neighbours): what the scene alone gives
    ctrl = collections.defaultdict(list); cstats = collections.Counter(); ctriples = 0
    for f in sorted(fr):
        if lab(f) == 0 and lab(f - 2) == 0 and lab(f + 2) == 0: ctriples += 1; test(f - 2, f, f + 2, ctrl, cstats)
    print('swapped frames compared with both neighbours: %d; %s' % (triples, ', '.join('%s %d' % kv for kv in stats.most_common())))
    if not triples: print('no swapped frame lies between two unswapped ones (a trace with trace_alternate=0? use the default comparison)'); return
    if not found: print('no call of a swapped frame differs from its neighbours beyond what they differ among themselves'); return
    print('control (unswapped frames tested the same way against the unswapped frames two either side): %d frames, %d findings' % (ctriples, len(ctrl)))
    print('\ncalls that differ (kind, function the swapped frame\'s call is in, method, the state it sets; in how many swapped frames, and in how many\n'
          'control frames the same test fires on the same calls: a finding the control shows as often is the scene\'s own):')
    rate = lambda d, k, n: len({h[0] for h in d.get(k, ())}) / max(n, 1)
    excess = lambda k: rate(found, k, triples) - rate(ctrl, k, ctriples)
    print('findings more frequent in swapped frames than in the control by more than 0.2: %d (listed first)' % sum(excess(k) > 0.2 for k in found))
    for (kind, o, name, sel), hits in sorted(found.items(), key=lambda kv: (-excess(kv[0]), -len(kv[1]), kv[0]))[:top]:
        print('  %-7s %-44s %-22s %-18s in %d of %d swapped frames; control %d of %d' % (kind, o or '-', name, ' '.join(sel),
              len({h[0] for h in hits}), triples, len({h[0] for h in ctrl.get((kind, o, name, sel), ())}), ctriples))
        for f, why in hits[:2]: print('           frame %d: %s' % (f, why))


def fmtval(v, d):
    args, h = v
    s = ' '.join(args) + (' hash ' + h if h != '00000000' else '')
    if d and len(d) <= 128: s += ' [' + ' '.join('%.5g' % x for x in f32(d)[:16]) + ']'
    return s


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('trace'); ap.add_argument('--manifest', default=os.path.join(H, '..', 'xikit', 'dist', 'xikit_manifest.txt'));
    ap.add_argument('--recon', default=os.path.join(H, '..', 'xikit', 'dist', 'gfx_recon_ingame.dll'))
    ap.add_argument('--extents', default=os.path.join(H, '..', '..', 'data', 'func_extents.json')); ap.add_argument('--by-site', action='store_true', help='keep the call site in the token (sites inside the reconstruction are addresses of the reconstruction, not of the game: a swapped run then differs at every swapped function)'); ap.add_argument('--top', type=int, default=40)
    o = ap.parse_args()
    head = open(o.trace).readline()
    if 'alternating' in head:
        meta = [head]; alternating(o.trace, Owners(meta, o.recon, o.manifest, o.extents) if os.path.exists(o.manifest) else None, o.top); return
    frames, meta, bufs = parse(o.trace, o.by_site); tab = names(o.manifest)
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
    # the bytes of changed calls, where the trace has them: the calls only the unswapped frames make, then the swapped
    shown = 0
    for name in sorted({t[0] for t in changed}):
        offs = [t for t, (lo, hi, got) in changed.items() if t[0] == name and t in BYTES and hi > 0 and max(got) == 0]
        ons = [t for t, (lo, hi, got) in changed.items() if t[0] == name and t in BYTES and hi == 0 and min(got) > 0]
        if not offs and not ons: continue
        print('\n%s: the bytes of calls made only without swaps (off) and only with them (on); %s' % (
            name, 'vertices of 28 bytes: x y z rhw colour u v' if name.endswith('UP') else '4x4 matrix rows'))
        for tag, lst in (('off', offs[:2]), ('on', ons[:2])):
            for tok in lst:
                rows = floats(BYTES[tok], 4 if 'Transform' in name else 7)
                print('  %-3s args %s' % (tag, ' '.join(tok[2])))
                for r in rows: print('        ' + r)
        shown += 1
    buffer_report(bufs, Owners(meta, o.recon, o.manifest, o.extents), o.top)


if __name__ == '__main__':
    main()
