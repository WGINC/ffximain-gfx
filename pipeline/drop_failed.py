"""drop_failed.py -- the functions of a built clean tree whose unit still fails to compile, as {ea: message}.

    python3 pipeline/drop_failed.py TREE_DIR OUT.json [PREVIOUS.json]
For each src/<unit>.cpp without an object (build/obj/<unit>.o) after build.sh and repair_compile.py, the first error
line of build/err/<unit>.txt is mapped to the function whose `// FUNCTION: FFXIMAIN` line precedes it. Each such
function is listed with the error; PREVIOUS.json's entries are kept. regen_clean.sh regenerates the tree without them
(XI_DROP), so the game's own code runs in their place. A unit whose error lies outside any function is reported, not
mapped. Exit 0 when nothing new was found."""
import json, os, re, sys


def main(d, out, prev=None):
    res = json.load(open(prev)) if prev and os.path.exists(prev) else {}
    new = 0
    for f in sorted(os.listdir(os.path.join(d, 'src'))):
        if not f.endswith('.cpp'): continue
        b = f[:-4]
        if os.path.exists(os.path.join(d, 'build', 'obj', b + '.o')): continue
        ep = os.path.join(d, 'build', 'err', b + '.txt')
        if not os.path.exists(ep): continue
        err = open(ep, encoding='utf-8', errors='replace').read().replace('‘', "'").replace('’', "'")
        lines = open(os.path.join(d, 'src', f)).read().split('\n')
        for m in re.finditer(r'src/%s\.cpp:(\d+):\d+: error: (.*)' % re.escape(b), err):
            n = int(m.group(1)); fn = None
            for k in range(n - 1, -1, -1):
                mm = re.match(r'// FUNCTION: FFXIMAIN (0x[0-9A-F]{8})', lines[k])
                if mm: fn = mm.group(1); break
            if fn is None: print('%s: error outside a function (line %d): %s' % (f, n, m.group(2)[:120])); continue
            if fn not in res: res[fn] = m.group(2)[:200]; new += 1; print('%s %s: %s' % (fn, f, m.group(2)[:120]))
    json.dump(dict(sorted(res.items())), open(out, 'w'), indent=0)
    print('%d functions do not compile (%d new)' % (len(res), new))
    sys.exit(1 if new else 0)


if __name__ == '__main__':
    main(*sys.argv[1:4])
