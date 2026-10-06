"""samples.py -- where the render thread's time went: xikit_samples_<pid>.csv (xikit sample=1) by function.

    python3 samples.py xikit_samples_1234.csv [--manifest ../xikit/dist/xikit_manifest.txt] [--top 30]

Each sample is one look at the render thread (every sample_ms). Three views, as shares of all samples:
  self       the function the thread was in: a game function (analysed address, by data/func_extents.json),
             a reconstructed one (its symbol, named as the manifest names it), or another module (Direct3D,
             the driver, Windows), by module;
  elsewhere  for samples in another module: the game or reconstructed function that called into it (the
             innermost return address on the stack), with the call site: time spent in Direct3D and below,
             by who asked for it;
  inclusive  a function with a return address anywhere on the stack: the time under it. The stack is scanned
             for return addresses (the address after a call instruction), so a stale one left in an older
             frame's dead slot is counted too; inclusive shares are an upper estimate.
A sample is an estimate of where time goes, not a measurement of one call: with n samples a share p has a
standard error of about sqrt(p(1-p)/n)."""
import argparse, bisect, collections, json, math, os, re
H = os.path.dirname(os.path.abspath(__file__))


class Names:
    def __init__(self, manifest, extents):
        self.man = {}
        if os.path.exists(manifest):
            for l in open(manifest):
                if l[0] in '#!' or not l.strip(): continue
                p = l.split(); self.man[0x10000000 + int(p[0], 16)] = p[5]
        ex = json.load(open(extents))
        self.pieces = sorted((lo, hi, int(k, 16)) for k, v in ex.items() for lo, hi in v)
        self.starts = [x[0] for x in self.pieces]

    def game(self, a):
        i = bisect.bisect_right(self.starts, a) - 1
        while i >= 0 and self.pieces[i][0] > a - 0x100000:
            lo, hi, f = self.pieces[i]
            if lo <= a < hi: return '%s 0x%08X' % (self.man.get(f, 'fn'), f)
            i -= 1
        return 'game 0x%08X (no extent)' % a


def parse(path, names):
    meta = []; rows = collections.defaultdict(list)
    for l in open(path):
        if l.startswith('#'): meta.append(l.strip()); continue
        p = l.rstrip('\n').split(',')
        if len(p) < 4: continue
        kind, where, addr, n = p[0], p[1], ','.join(p[2:-1]), int(p[-1])
        if where == 'game':
            a = int(addr, 16); fn = names.game(a); site = '0x%08X' % a
        elif where == 'recon':
            parts = addr.split(); site = parts[0]
            fn = ('%s (reconstruction)' % parts[2]) if len(parts) > 2 else ('reconstruction %s' % (parts[1] if len(parts) > 1 else '?'))
        elif where == 'module':
            fn = addr.split('+')[0]; site = addr
        else:
            fn = 'memory of no module'; site = addr
        rows[kind].append((fn, site, n))
    return meta, rows


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('samples'); ap.add_argument('--manifest', default=os.path.join(H, '..', 'xikit', 'dist', 'xikit_manifest.txt'))
    ap.add_argument('--extents', default=os.path.join(H, '..', '..', 'data', 'func_extents.json')); ap.add_argument('--top', type=int, default=30)
    o = ap.parse_args()
    meta, rows = parse(o.samples, Names(o.manifest, o.extents))
    for m in meta[:1]: print(m)
    total = sum(n for _, _, n in rows['self'])
    if not total: print('no samples'); return
    se = lambda n: 100 * math.sqrt((n / total) * (1 - n / total) / total)
    def table(title, kind, by_site=False):
        agg = collections.Counter(); sites = collections.defaultdict(collections.Counter)
        for fn, site, n in rows[kind]: agg[fn] += n; sites[fn][site] += n
        print('\n%s (%% of %d samples, +- one standard error):' % (title, total))
        for fn, n in agg.most_common(o.top):
            extra = ''
            if by_site: extra = '  sites: ' + ', '.join('%s %d' % kv for kv in sites[fn].most_common(3))
            print('  %6.2f%% +-%.2f  %s%s' % (100 * n / total, se(n), fn, extra))
    table('self: where the render thread was', 'self')
    table('elsewhere: time in other modules (Direct3D, driver, Windows), by the game or reconstructed function that called there', 'caller', True)
    table('inclusive: time under each function (upper estimate)', 'incl')


if __name__ == '__main__':
    main()
