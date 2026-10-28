"""emit_identical.py -- the sweep's sources of the functions it found at the game's bytes, as recompile/ files.

    python3 pipeline/vc6/emit_identical.py SWEEP.json KEEP_DIR OUT_DIR [--only 0xADDR,...] [--why TEXT]

SWEEP.json and KEEP_DIR are one run of pipeline/vc6/sweep.py (--keep KEEP_DIR). For each function it found IDENTICAL
-- as written, or under the rule `rule_identical` names -- the source the sweep compiled (KEEP_DIR/f_<ADDR>[_rule].c
or .cpp, with the casts its permissive rounds wrote) is copied to OUT_DIR with a header naming where it came from and
a `// FUNCTION: FFXIMAIN 0x...` mark above its definition, so pipeline/vc6/match.py checks it (match.py --all reads
recompile/sweep/). The shim headers the sources include (vc6_shim.h, xi_d3d8_vc6*.h) are copied beside them. A
function the sweep wrote as a class of its own (rule adjthunk: the thunk is VC6's, not a definition) or compiled
together with its callees (sametu) is not emitted. Nothing here decides a verdict: match.py does, on each file.
"""
import argparse, glob, json, os, re, shutil


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('sweep'); ap.add_argument('keep'); ap.add_argument('out')
    ap.add_argument('--only', default=''); ap.add_argument('--why', default='')
    o = ap.parse_args()
    res = json.load(open(o.sweep))
    only = {'0x%08X' % int(x, 16) for x in o.only.split(',') if x}
    os.makedirs(o.out, exist_ok=True)
    for h in glob.glob(os.path.join(o.keep, '*.h')): shutil.copy(h, o.out)
    n = 0
    for ea, v in sorted(res.items()):
        if only and ea not in only: continue
        rule = None
        if v.get('verdict') == 'IDENTICAL': pass
        elif v.get('rule_verdict') == 'IDENTICAL' and v.get('rule_identical'): rule = v['rule_identical']
        else: continue
        if rule and 'adjthunk' in rule: continue
        stem = 'f_%s%s' % (ea[2:], '_' + rule if rule else '')
        src = next((p for p in (os.path.join(o.keep, stem + x) for x in ('.cpp', '.c')) if os.path.exists(p)), None)
        if src is None: print('%s: no kept source %s' % (ea, stem)); continue
        lines = open(src).read().split('\n')
        # the definition: the last line followed by an unindented `{` (the function is the last thing in a sweep source;
        # Hex-Rays puts its blocks' braces on lines of their own too, indented)
        defs = [k for k in range(len(lines) - 1) if lines[k + 1].rstrip() == '{' and lines[k].strip() and not lines[k].lstrip().startswith(('struct', '#', '//'))]
        if not defs: print('%s: no definition found' % ea); continue
        k = defs[-1]
        head = ['/* %s at the game\'s bytes: pipeline/vc6/sweep.py\'s source of it%s, unchanged but for this note and the',
                '   mark below (pipeline/vc6/emit_identical.py). %s */']
        head = [head[0] % (ea, ' under rule ' + rule if rule else ' as the tree writes it'), head[1] % (o.why or 'match.py checks it.')]
        lines = head + lines[:k] + ['// FUNCTION: FFXIMAIN %s' % ea] + lines[k:]
        open(os.path.join(o.out, 'sw_%s%s' % (ea[2:], os.path.splitext(src)[1])), 'w').write('\n'.join(lines))
        n += 1
    print('%d sources written to %s' % (n, o.out))


if __name__ == '__main__':
    main()
