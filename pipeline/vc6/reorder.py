"""reorder.py -- search the order of statements in a marked region of a hand-written source for the game's bytes.

Most of what is left in the hand-matched functions (recompile/d3d_round_120.md) is scheduling: which register a value
lands in and where a load or store is placed follow from the order of independent statements. The order is searched,
not reasoned about: the lines between `//@reorder` and `//@end` in SRC (one statement per line) are permuted by
single swaps and moves, each candidate compiled with the game's compiler and scored by mdiff.py's count of differing
instruction lines against the game's function, a candidate kept when it is not worse (hill climbing from the
written order, with restarts). A line marked `//@fixed` stays where it is.

What it finds is only a source order that compiles to the game's bytes: a candidate is written out only where
match.py says IDENTICAL, which also means it computes what the game's code computes (the same instructions). Nothing
about the original source's order follows from it.

    FFXI_DLL=... VC6=... python3 pipeline/vc6/reorder.py SRC OUT [--tries 300] [--jobs 2] [--seed 1]
OUT is written only when a candidate is IDENTICAL; the best score is printed either way."""
import argparse, concurrent.futures as CF, os, random, re, subprocess, sys, tempfile
P = os.path.dirname(os.path.abspath(__file__))


def regions(lines):
    out, start = [], None
    for k, l in enumerate(lines):
        if l.strip() == '//@reorder': start = k + 1
        elif l.strip() == '//@end' and start is not None: out.append((start, k)); start = None
    return out


def score(text, src_dir):
    fd, path = tempfile.mkstemp(suffix=os.path.splitext(SRC)[1], dir=src_dir)   # beside SRC: its includes resolve
    os.write(fd, text.encode()); os.close(fd)
    try:
        r = subprocess.run([sys.executable, os.path.join(P, 'mdiff.py'), path, '-q'], capture_output=True, text=True)
        m = re.search(r': (\d+) lines differ', r.stdout)
        return int(m.group(1)) if m else 10 ** 6
    finally:
        os.remove(path)


def mutate(order, fixed, rnd):
    o = list(order)
    free = [k for k in range(len(o)) if o[k] not in fixed]
    if len(free) < 2: return o
    i, j = rnd.sample(free, 2)
    if rnd.random() < 0.5: o[i], o[j] = o[j], o[i]
    else:
        x = o.pop(i); o.insert(j, x)
        if any(o[k] in fixed and order[k] != o[k] for k in range(len(o))): return list(order)
    return o


def main():
    global SRC
    ap = argparse.ArgumentParser(); ap.add_argument('src'); ap.add_argument('out')
    ap.add_argument('--tries', type=int, default=300); ap.add_argument('--jobs', type=int, default=2); ap.add_argument('--seed', type=int, default=1)
    o = ap.parse_args(); SRC = os.path.abspath(o.src)
    lines = open(SRC).read().split('\n')
    regs = regions(lines)
    if not regs: sys.exit('no //@reorder ... //@end region in ' + o.src)
    rnd = random.Random(o.seed)
    body = [list(range(a, b)) for a, b in regs]
    fixed = {k for a, b in regs for k in range(a, b) if '//@fixed' in lines[k]}
    def text(orders):
        out = list(lines)
        for (a, b), ordr in zip(regs, orders): out[a:b] = [lines[k] for k in ordr]
        return '\n'.join(out)
    best = body; best_s = score(text(best), os.path.dirname(SRC))
    print('written order: %d lines differ' % best_s, flush=True)
    tried = 0
    with CF.ThreadPoolExecutor(o.jobs) as ex:
        while tried < o.tries and best_s > 0:
            cands = []
            for _ in range(o.jobs):
                r = rnd.randrange(len(regs)); c = [list(x) for x in best]; c[r] = mutate(c[r], fixed, rnd); cands.append(c)
            scores = list(ex.map(lambda c: score(text(c), os.path.dirname(SRC)), cands)); tried += len(cands)
            for c, s in zip(cands, scores):
                if s <= best_s:
                    if s < best_s: print('try %d: %d lines' % (tried, s), flush=True)
                    best, best_s = c, s
    print('best: %d lines after %d tries' % (best_s, tried))
    if best_s == 0:
        t = text(best); fd, path = tempfile.mkstemp(suffix=os.path.splitext(SRC)[1], dir=os.path.dirname(SRC))
        os.write(fd, t.encode()); os.close(fd)
        r = subprocess.run([sys.executable, os.path.join(P, 'match.py'), path], capture_output=True, text=True); os.remove(path)
        print(r.stdout.strip())
        if 'DIFFERS' not in r.stdout and 'MISSING' not in r.stdout and 'compile failed' not in r.stdout:
            open(o.out, 'w').write(t); print('written', o.out)
    else:
        open(o.out + '.best', 'w').write(text(best)); print('closest order written to', o.out + '.best')


if __name__ == '__main__':
    main()
