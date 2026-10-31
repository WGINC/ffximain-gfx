"""hot_functions.py -- the game functions where the render thread spent its time, from the kit's sampling profiler.

Each SAMPLES file is an xikit sampler log (kit/xikit, `sample=1`: the render thread's instruction pointer every 1 ms;
rows `self,game,ADDR,N` are N samples whose instruction pointer was ADDR in FFXiMain.dll). A sample is credited to
the function whose extent (data/func_extents.json) holds its address. The hot set is the fewest functions, most
samples first, holding FRACTION of the samples credited to game functions: a measured cut over the runs given, not a
property of the game (other scenes spend their time elsewhere). The runs used are named in the output.

    python3 pipeline/hot_functions.py OUT.json FRACTION SAMPLES.csv [SAMPLES.csv ...]
"""
import bisect, collections, json, os, sys

ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')


def main(out, frac, *files):
    frac = float(frac)
    ext = {int(k, 16): v for k, v in json.load(open(os.path.join(ROOT, 'data', 'func_extents.json'))).items() if not k.startswith('_')}
    iv = sorted((s, t, f) for f, sp in ext.items() for s, t in sp); starts = [x[0] for x in iv]
    def holder(a):
        i = bisect.bisect_right(starts, a) - 1
        return iv[i][2] if i >= 0 and iv[i][0] <= a < iv[i][1] else None
    cnt = collections.Counter(); runs = []; outside = 0
    for p in files:
        head = open(p).readline().strip()
        for l in open(p):
            x = l.strip().split(',')
            if len(x) != 4 or x[:2] != ['self', 'game']: continue
            f = holder(int(x[2].split()[0], 16)); k = int(x[3])
            if f is None: outside += k
            else: cnt['0x%08X' % f] += k
        runs.append({'file': os.path.basename(p), 'header': head})
    total = sum(cnt.values()); acc = 0; hot = {}
    for f, k in cnt.most_common():
        if acc >= frac * total: break
        acc += k; hot[f] = k
    json.dump({'_doc': __doc__.split('\n\n')[0], '_runs': runs, '_fraction': frac,
               '_samples': {'in_game_functions': total, 'outside_any_extent': outside, 'in_hot_set': acc},
               'functions': hot}, open(out, 'w'), indent=1)
    print('%d samples in %d game functions; hot set %d functions, %d samples (%.1f%%)' % (
        total, len(cnt), len(hot), acc, 100.0 * acc / total))


if __name__ == '__main__':
    main(*sys.argv[1:])
