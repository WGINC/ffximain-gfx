"""mutate.py -- a source search for C++ functions (decomp-permuter takes C only): random rewrites of a function's text,
each compiled by the game's compiler and scored against the game's bytes, kept where not worse (hill climbing).

decomp-permuter (permute.py) parses C with pycparser, so the game's __thiscall methods, written as C++, are out of its
reach. This applies, at the level of the text, the kinds of change that decided the hand matches
(recompile/d3d_round_118.md to _120.md):
  * swap  -- two statements of a block (lines ending in `;`, the same indentation) exchange places (rule 14);
  * move  -- a statement moves to another place in its block;
  * chain -- `a = b = x;` becomes `b = a = x;` (the right-hand target is stored first);
  * arms  -- `if (C) {A} else {B}` becomes `if (!(C)) {B} else {A}` (the if body falls through: rule 5);
  * decl  -- two declarations of locals exchange places (the order VC6 allocates registers in);
  * sign  -- a local declared `int` becomes `unsigned int`, or back;
  * cmp   -- `X == 0` becomes `!X`, `X != 0` becomes `X`, `a < b` becomes `b > a`, and back;
  * incr  -- `i++` becomes `++i` or `i += 1`.
Any rewrite may change what the code computes; a result counts only where match.py says IDENTICAL, and then it computes
what the game's code computes (the same instructions). Nothing about the original source follows from it.

    FFXI_DLL=... VC6=... python3 pipeline/vc6/mutate.py SRC OUT [--tries 400] [--jobs 2] [--seed 1]
SRC is a self-contained source with one `// FUNCTION: FFXIMAIN 0x...` mark (a recompile/ file, or a sweep source with
the mark added, as reorder_batch.py does). OUT is written only when a candidate is IDENTICAL; else OUT.best holds the
closest text.
"""
import argparse, concurrent.futures as CF, os, random, re, subprocess, sys, tempfile
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import reorder as RO

STMT = re.compile(r'^([ \t]+)(?!return\b|break\b|continue\b|case\b|default\b|goto\b|if\b|while\b|for\b|do\b|else\b)[^{}:\n]*;\s*$')
DECL = re.compile(r'^([ \t]+)((?:unsigned |signed |const )*[A-Za-z_]\w*)(\s*\**\s*)(\w+)(\[[^\]]*\])?;\s*$')
KW = {'return', 'goto', 'delete', 'else'}


def body_range(lines):
    """the lines of the marked function's body: after the `{` that follows the mark, to its matching `}`"""
    m = next(i for i, l in enumerate(lines) if l.startswith('// FUNCTION: FFXIMAIN'))
    a = next(i for i in range(m, len(lines)) if lines[i].strip() == '{' or lines[i].rstrip().endswith('{'))
    depth = 0
    for i in range(a, len(lines)):
        depth += lines[i].count('{') - lines[i].count('}')
        if depth == 0: return a + 1, i
    return a + 1, len(lines)


def _stmt_runs(lines, a, b):
    """indices of plain statements, grouped by run (adjacent, same indentation)"""
    runs, cur, ind = [], [], None
    for i in range(a, b):
        m = STMT.match(lines[i])
        if m and not DECL.match(lines[i]) and not lines[i].lstrip().startswith('//'):
            if cur and m.group(1) != ind: runs.append(cur); cur = []
            ind = m.group(1); cur.append(i)
        else:
            if cur: runs.append(cur)
            cur, ind = [], None
    if cur: runs.append(cur)
    return [r for r in runs if len(r) >= 2]


def m_swap(lines, a, b, rnd):
    runs = _stmt_runs(lines, a, b)
    if not runs: return None
    r = rnd.choice(runs); i, j = rnd.sample(r, 2); out = list(lines); out[i], out[j] = out[j], out[i]; return out


def m_move(lines, a, b, rnd):
    runs = _stmt_runs(lines, a, b)
    if not runs: return None
    r = rnd.choice(runs); i, j = rnd.sample(r, 2); out = list(lines); x = out.pop(i); out.insert(j, x); return out


def m_chain(lines, a, b, rnd):
    sites = [i for i in range(a, b) if re.match(r'^\s*[^=;]+?\s=\s[^=;]+?\s=\s[^=]', lines[i]) and '==' not in lines[i]]
    if not sites: return None
    i = rnd.choice(sites); m = re.match(r'^(\s*)([^=;]+?)\s=\s([^=;]+?)\s=\s(.*)$', lines[i])
    if not m: return None
    out = list(lines); out[i] = '%s%s = %s = %s' % (m.group(1), m.group(3), m.group(2), m.group(4)); return out


def _block_end(lines, i):
    """the index of the line closing the block whose `{` is on line i"""
    depth = 0
    for k in range(i, len(lines)):
        depth += lines[k].count('{') - lines[k].count('}')
        if depth == 0: return k
    return None


def m_arms(lines, a, b, rnd):
    """`if (C)` / `{` A `}` / `else` / `{` B `}`, each on its own line"""
    sites = []
    for i in range(a, b - 4):
        if re.match(r'^\s*if \(.*\)\s*$', lines[i]) and lines[i + 1].strip() == '{':
            e = _block_end(lines, i + 1)
            if e and e + 2 < b and lines[e + 1].strip() == 'else' and lines[e + 2].strip() == '{':
                e2 = _block_end(lines, e + 2)
                if e2: sites.append((i, e, e2))
    if not sites: return None
    i, e, e2 = rnd.choice(sites)
    m = re.match(r'^(\s*)if \((.*)\)\s*$', lines[i]); cond = m.group(2)
    neg = cond[2:-1] if cond.startswith('!(') and cond.endswith(')') else '!(%s)' % cond
    A, B = lines[i + 1:e + 1], lines[e + 3:e2 + 1]
    return lines[:i] + ['%sif (%s)' % (m.group(1), neg), lines[e + 2]] + B[:-1] + [lines[e2], lines[e + 1], lines[i + 1]] + A[:-1] + [lines[e]] + lines[e2 + 1:]


def m_decl(lines, a, b, rnd):
    ds = [i for i in range(a, b) if DECL.match(lines[i]) and DECL.match(lines[i]).group(2) not in KW]
    if len(ds) < 2: return None
    i, j = rnd.sample(ds, 2); out = list(lines); out[i], out[j] = out[j], out[i]; return out


def m_sign(lines, a, b, rnd):
    ds = [i for i in range(a, b) if re.match(r'^\s*(unsigned int|int) \w+;', lines[i])]
    if not ds: return None
    i = rnd.choice(ds); out = list(lines)
    out[i] = re.sub(r'^(\s*)unsigned int ', r'\1int ', out[i]) if 'unsigned int' in out[i] else re.sub(r'^(\s*)int ', r'\1unsigned int ', out[i])
    return out


_CMP = [(re.compile(r'\(([^()]+?) == 0\)'), r'(!\1)'), (re.compile(r'\(!(\w+)\)'), r'(\1 == 0)'),
        (re.compile(r'\(([^()]+?) != 0\)'), r'(\1)'), (re.compile(r'\((\w+) < (\w+)\)'), r'(\2 > \1)'),
        (re.compile(r'\((\w+) > (\w+)\)'), r'(\2 < \1)')]


def m_cmp(lines, a, b, rnd):
    sites = [(i, k) for i in range(a, b) for k, (rx, _) in enumerate(_CMP) if rx.search(lines[i])]
    if not sites: return None
    i, k = rnd.choice(sites); rx, rep = _CMP[k]; out = list(lines); out[i] = rx.sub(rep, out[i], count=1); return out


def m_incr(lines, a, b, rnd):
    sites = [i for i in range(a, b) if re.search(r'\b\w+\+\+|\+\+\w+\b|\w+ \+= 1\b', lines[i])]
    if not sites: return None
    i = rnd.choice(sites); out = list(lines); l = out[i]
    forms = [lambda v: v + '++', lambda v: '++' + v, lambda v: v + ' += 1']
    m = re.search(r'\b(\w+)\+\+|\+\+(\w+)\b|\b(\w+) \+= 1\b', l); v = m.group(1) or m.group(2) or m.group(3)
    out[i] = l[:m.start()] + rnd.choice(forms)(v) + l[m.end():]
    return out


MUTATIONS = [m_swap, m_move, m_chain, m_arms, m_decl, m_sign, m_cmp, m_incr]


def mutate(lines, rnd):
    a, b = body_range(lines)
    for _ in range(20):
        f = rnd.choice(MUTATIONS)
        out = f(lines, a, b, rnd)
        if out is not None and out != lines: return out, f.__name__[2:]
    return None, None


def main():
    ap = argparse.ArgumentParser(); ap.add_argument('src'); ap.add_argument('out')
    ap.add_argument('--tries', type=int, default=400); ap.add_argument('--jobs', type=int, default=2); ap.add_argument('--seed', type=int, default=1)
    o = ap.parse_args(); src = os.path.abspath(o.src); RO.SRC = src; d = os.path.dirname(src)
    rnd = random.Random(o.seed)
    best = open(src).read().split('\n'); best_s = RO.score('\n'.join(best), d)
    print('written: %d lines differ' % best_s, flush=True)
    tried = 0
    with CF.ThreadPoolExecutor(o.jobs) as ex:
        while tried < o.tries and best_s > 0:
            cands = [c for c in (mutate(best, rnd) for _ in range(o.jobs)) if c[0] is not None]
            if not cands: break
            scores = list(ex.map(lambda c: RO.score('\n'.join(c[0]), d), cands)); tried += len(cands)
            for (c, kind), s in zip(cands, scores):
                if s <= best_s:
                    if s < best_s: print('try %d: %d lines (%s)' % (tried, s, kind), flush=True)
                    best, best_s = c, s
    print('best: %d lines after %d tries' % (best_s, tried))
    t = '\n'.join(best)
    if best_s == 0:
        fd, path = tempfile.mkstemp(suffix=os.path.splitext(src)[1], dir=d); os.write(fd, t.encode()); os.close(fd)
        r = subprocess.run([sys.executable, os.path.join(P, 'match.py'), path], capture_output=True, text=True); os.remove(path)
        print(r.stdout.strip())
        if 'IDENTICAL' in r.stdout and 'DIFFERS' not in r.stdout and 'MISSING' not in r.stdout:
            open(o.out, 'w').write(t); print('written', o.out); return
    open(o.out + '.best', 'w').write(t); print('closest written to', o.out + '.best')


if __name__ == '__main__':
    main()
