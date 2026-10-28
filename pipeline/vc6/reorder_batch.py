"""reorder_batch.py -- reorder.py over many of the sweep's DIFFERS functions, with the regions marked mechanically.

For each address: the sweep's source as written (sweep.py --keep DIR: f_<ADDR>.c / .cpp), with the globvar and frame rules where
they apply (sweep.py RULES), gets the `// FUNCTION: FFXIMAIN 0x<ADDR>` mark (match.py confirms reorder.py's result by
it: without one it would compare nothing) and `//@reorder` ... `//@end` around every run of two or more plain
statements (lines ending in `;`, at one indentation, no braces, labels, `return`, `break`, `case` or declarations
without an initializer: a declaration's place is not a statement's). reorder.py then permutes them; it writes OUT only
where match.py says IDENTICAL, so a function counts only as the game's bytes prove.

    FFXI_DLL=... VC6=... python3 pipeline/vc6/reorder_batch.py KEEP_DIR WORK_DIR 0xADDR[,0xADDR...] [--tries 120] [--par 3]
Prints one line per function (best score, or IDENTICAL) and writes WORK_DIR/summary.json."""
import argparse, concurrent.futures as CF, glob, json, os, re, shutil, subprocess, sys
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import sweep as S

PLAIN = re.compile(r'^([ \t]+)(?!return\b|break\b|continue\b|case\b|default\b|goto\b|if\b|while\b|for\b|do\b|else\b)[^{}:\n]*;\s*$')
REF = re.compile(r'^[ \t]+[^=;]*\(?&\w+\)?(\[\w+\])? = \*reinterpret_cast<')   # a reference into the frame block
DECL = re.compile(r'^[ \t]+[A-Za-z_][\w \t]*[\w*]\s*\**\s*\w+(\[[^\]]*\])?;\s*$')     # `T name;`: no statement


def mark(text, ea):
    lines = text.split('\n')
    k = next((i for i, l in enumerate(lines) if l == '{' and i > 0), None)
    if k is None: return None
    lines.insert(k - 1, '// FUNCTION: FFXIMAIN 0x%08X' % ea)
    out, run, ind = [], [], None
    def flush():
        if len(run) >= 2: out.extend(['//@reorder'] + run + ['//@end'])
        else: out.extend(run)
        run.clear()
    for l in lines:
        m = PLAIN.match(l)
        if m and not DECL.match(l) and not REF.match(l) and not l.lstrip().startswith('//'):
            if run and m.group(1) != ind: flush()
            ind = m.group(1); run.append(l)
        else:
            flush(); ind = None; out.append(l)
    flush()
    t = '\n'.join(out)
    return t if '//@reorder' in t else None


def one(keep, work, ea, tries):
    src = [f for f in glob.glob(os.path.join(keep, 'f_%08X.c*' % ea)) if not f.endswith('.obj')]
    if not src: return ea, 'no source'
    text = open(src[0]).read()
    text = S._rule_globvar(text, None, None) or text
    text = S._rule_frame(text, None, None) or text          # the xi_frame block as plain locals, where it applies
    t = mark(text, ea)
    if t is None: return ea, 'no region'
    d = os.path.join(work, '%08X' % ea); os.makedirs(d, exist_ok=True)
    for h in glob.glob(os.path.join(keep, '*.h')): shutil.copy(h, d)
    f = os.path.join(d, os.path.basename(src[0])); open(f, 'w').write(t)
    out = os.path.join(d, 'ident' + os.path.splitext(f)[1])
    r = subprocess.run([sys.executable, os.path.join(P, 'reorder.py'), f, out, '--tries', str(tries), '--jobs', '1'],
                       capture_output=True, text=True)
    open(os.path.join(d, 'log.txt'), 'w').write(r.stdout + r.stderr)
    if os.path.exists(out): return ea, 'IDENTICAL'
    m = re.search(r'written order: (\d+).*?best: (\d+)', r.stdout, re.S)
    return ea, ('%s -> %s lines' % m.groups()) if m else 'failed: ' + (r.stdout + r.stderr).strip()[-120:]


def main():
    ap = argparse.ArgumentParser(); ap.add_argument('keep'); ap.add_argument('work'); ap.add_argument('addrs')
    ap.add_argument('--tries', type=int, default=120); ap.add_argument('--par', type=int, default=3)
    o = ap.parse_args(); os.makedirs(o.work, exist_ok=True)
    eas = [int(a, 16) for a in o.addrs.split(',') if a]
    res = {}
    with CF.ThreadPoolExecutor(max_workers=o.par) as ex:
        for ea, v in ex.map(lambda a: one(o.keep, o.work, a, o.tries), eas):
            res['0x%08X' % ea] = v; print('0x%08X %s' % (ea, v), flush=True)
    json.dump(res, open(os.path.join(o.work, 'summary.json'), 'w'), indent=1)
    print('%d functions: %d IDENTICAL' % (len(res), sum(v == 'IDENTICAL' for v in res.values())))


if __name__ == '__main__':
    main()
