"""fork.py -- edit a reconstructed function as a diff against its gate-verified source, compile it into a mod, and
gate the edit against the game's own code (docs/SDK.md, "Forks").

    python3 fork.py new NAME [DIR]       NAME: as the tree names it (CDx::DrawFadeQuad, Gfx_IsGammaSupported) or its
                                         analysed address (0x1000A820). Writes DIR/<Name>.fork.cpp (default: .): the
                                         function's block from the tree's source, verbatim, with that file's includes
    python3 fork.py diff FILE.fork.cpp   the fork against the tree's source of the same function (unified diff)
    python3 fork.py gate FILE.fork.cpp [--trials N] [--live LIVE.json] [--work DIR]
                                         (in the repository, $FFXI_DLL) the gate on the fork: the in-game build of the
                                         tree with this one function replaced by the fork, compiled with the flags a mod
                                         build gives it, run against the game's original function (diffemu_gfx.py,
                                         with coverage). An unedited fork must PASS as the tree does; an edited one
                                         shows where it now differs from the game

What is a fact here and what is not:
  * `new` copies the gate-verified source text. It does not claim the copy compiles to the same machine code in a
    mod: the code of a function can depend on the rest of its source file (GCC inlines and allocates registers
    across the functions it can see). `gate` is what verifies the fork as compiled.
  * The mod compiles the fork with the tree's flags (fork_flags.txt, generated from tree/build.sh by gen_sdk.py) and
    -DXI_INGAME, as `gate` does; the fork's calls to other reconstructed functions go to gfx_recon_ingame.dll in the
    mod and to the same build's code in `gate`. The C library functions the code calls (memcpy, ...) are the mod's
    (msvcrt) in the game and the tree's rt.c in the gate."""
import argparse, difflib, hashlib, json, os, re, shutil, subprocess, sys
H = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.join(H, '..')
SRC = next((d for d in (os.path.join(ROOT, 'tree', 'src'), os.path.join(H, 'tree_src')) if os.path.isdir(d)), None)
FUNC = re.compile(r'^// FUNCTION: FFXIMAIN (0x[0-9A-Fa-f]+)')
HEAD = '// FORK OF: %s %s (from %s)\n'


def blocks():
    """every function block of the tree's source: (address, file, includes, text)"""
    for fn in sorted(os.listdir(SRC)):
        if not fn.endswith('.cpp'): continue
        lines = open(os.path.join(SRC, fn)).read().split('\n')
        inc = [l for l in lines if l.startswith('#include')]
        starts = [i for i, l in enumerate(lines) if FUNC.match(l)]
        for k, i in enumerate(starts):
            j = starts[k + 1] if k + 1 < len(starts) else len(lines)
            body = lines[i:j]
            while body and not body[-1].strip(): body.pop()
            yield int(FUNC.match(lines[i]).group(1), 16), fn, inc, '\n'.join(body) + '\n'


def placement():
    for p in (os.path.join(ROOT, 'tree', 'placement.json'), os.path.join(H, 'tree_placement.json')):
        if os.path.exists(p): return {int(a, 16): v['symbol'] for a, v in json.load(open(p)).items()}
    sys.exit('fork.py: no tree/placement.json')


def find(name):
    sym = placement()
    addr = int(name, 16) if re.fullmatch(r'0x[0-9A-Fa-f]+', name) else next((a for a, s in sym.items() if s == name), None)
    if addr is None: sys.exit('fork.py: %s is not a function of the tree (tree/placement.json)' % name)
    b = next((x for x in blocks() if x[0] == addr), None)
    if b is None: sys.exit('fork.py: no source block for 0x%08X in %s' % (addr, SRC))
    return addr, sym.get(addr, name), b


def ident(sym): return re.sub(r'\W', '_', sym.replace('::', '__'))


def read_fork(path):
    t = open(path).read()
    m = re.match(r'// FORK OF: (\S+) (0x[0-9A-Fa-f]+) \(from (\S+)\)\n', t)
    if not m: sys.exit('fork.py: %s does not start with the line fork.py new writes (// FORK OF: ...)' % path)
    return m.group(1), int(m.group(2), 16), m.group(3), t


def cmd_new(o):
    addr, sym, (_, fn, inc, text) = find(o.name)
    os.makedirs(o.dir, exist_ok=True)
    out = os.path.join(o.dir, ident(sym) + '.fork.cpp')
    if os.path.exists(out): sys.exit('fork.py: %s exists; not overwritten' % out)
    with open(out, 'w') as f:
        f.write(HEAD % (sym, '0x%08X' % addr, 'tree/src/' + fn))
        f.write('// Edit the body below; `fork.py diff` shows the edit, `fork.py gate` runs it against the game\'s code.\n')
        f.write('// Hook it from the mod\'s xikit_mod_init with XI_FORK(host, %s).\n' % sym)
        f.write('\n'.join(inc) + '\n\n' + text)
    print('wrote %s (%s, 0x%08X, from tree/src/%s)' % (out, sym, addr, fn))


def cmd_diff(o):
    sym, addr, src, t = read_fork(o.file)
    _, _, (_, fn, inc, text) = find('0x%08X' % addr)
    base = '\n'.join(inc) + '\n\n' + text
    mine = t.split('\n', 3)[3] if t.count('\n') >= 3 else ''
    d = list(difflib.unified_diff(base.splitlines(True), mine.splitlines(True), 'tree/src/' + fn, o.file))
    sys.stdout.writelines(d)
    if not d: print('(no change: the fork is the tree\'s source of %s)' % sym)


def flags():
    """the tree's compile flags, from fork_flags.txt; in the repository checked against tree/build.sh itself"""
    ff = [l for l in open(os.path.join(H, 'fork_flags.txt')).read().split('\n') if l]
    b = os.path.join(ROOT, 'tree', 'build.sh')
    if os.path.exists(b) and ff != tree_flags(b): sys.exit('fork.py: sdk/fork_flags.txt is not tree/build.sh\'s FLAGS; run sdk/gen_sdk.py')
    return ff


def tree_flags(build_sh):
    r = subprocess.run(['bash', '-c', 'eval "$(sed -n "/^FLAGS=(/,/)$/p" "$1")"; printf "%s\\n" "${FLAGS[@]}"', '_', build_sh],
                       capture_output=True, text=True, check=True)
    return [l for l in r.stdout.split('\n') if l]


def run(c, **k):
    r = subprocess.run(c, capture_output=True, text=True, **k)
    if r.returncode: sys.exit('fork.py: %s failed\n%s%s' % (c[0], r.stdout[-3000:], r.stderr[-3000:]))
    return r


def cmd_gate(o):
    sym, addr, src, t = read_fork(o.file)
    dll = os.environ.get('FFXI_DLL') or sys.exit('fork.py gate: set FFXI_DLL to your copy of FFXiMain.dll')
    cases = [c for c in json.load(open(os.path.join(ROOT, 'verification', 'cases_clean_final.json'))) if int(c['original'], 16) == addr]
    if not cases: sys.exit('fork.py gate: no gate case for 0x%08X' % addr)
    ff = flags(); cxx = os.environ.get('CXX', 'i686-w64-mingw32-g++')
    # the in-game build of the tree (every object), once per source state
    key = hashlib.sha1(b''.join(open(os.path.join(SRC, f), 'rb').read() for f in sorted(os.listdir(SRC)))
                       + open(os.path.join(ROOT, 'tree', 'build.sh'), 'rb').read() + open(os.path.join(ROOT, 'tree', 'rt.c'), 'rb').read()).hexdigest()[:12]
    work = os.path.join(o.work, 'tree_' + key)
    if not os.path.exists(os.path.join(work, 'build', 'gfx_recon_ingame.dll')):
        shutil.rmtree(work, ignore_errors=True); shutil.copytree(os.path.join(ROOT, 'tree'), work, ignore=shutil.ignore_patterns('build'))
        print('building the tree (in-game) once into %s ...' % work, flush=True)
        run(['./build.sh'], cwd=work, env=dict(os.environ, XI_INGAME='1'))
    obj = os.path.join(work, 'build', 'obj')
    # the fork, compiled as build_mod.sh compiles it; the tree's definition of the same symbol renamed in its object
    g = os.path.join(o.work, 'fork_' + ident(sym)); shutil.rmtree(g, ignore_errors=True); os.makedirs(g)
    run([cxx] + ff + ['-DXI_INGAME', '-I' + os.path.join(ROOT, 'tree', 'include'), '-c', os.path.abspath(o.file), '-o', os.path.join(g, 'fork.o')])
    mangled = cases[0]['candidate']
    nm = run(['i686-w64-mingw32-nm', '-g', '--defined-only', os.path.join(g, 'fork.o')]).stdout
    if not re.search(r' T _?%s$' % re.escape(mangled), nm, re.M):
        sys.exit('fork.py gate: the fork does not define %s (the gate case\'s symbol); did the edit change its signature?' % mangled)
    objs = []
    for f in sorted(os.listdir(obj)):
        if not f.endswith('.o') or f == 'rt.o': continue
        p = os.path.join(obj, f)
        m = re.search(r' T (_?%s)$' % re.escape(mangled), run(['i686-w64-mingw32-nm', '-g', '--defined-only', p]).stdout, re.M)
        if m:                                       # (the COFF symbol, with i386's leading underscore)
            q = os.path.join(g, f)
            run(['i686-w64-mingw32-objcopy', '--redefine-sym', '%s=%s__tree' % (m.group(1), m.group(1)), p, q]); p = q
        objs.append(p)
    out = os.path.join(g, 'gfx_recon_fork.dll')
    run([cxx, '-shared', '-nostdlib', '-o', out, os.path.join(obj, 'rt.o')] + objs + [os.path.join(g, 'fork.o'), '-lgcc',
         '-Wl,--image-base=0x60000000', '-Wl,-e,_DllMainCRTStartup@12', '-Wl,--disable-auto-image-base', '-Wl,--kill-at'])
    run([sys.executable, os.path.join(ROOT, 'kit', 'xikit', 'close_gaps.py'), out])
    json.dump(cases, open(os.path.join(g, 'case.json'), 'w'))
    res = os.path.join(g, 'gate.jsonl')
    env = dict(os.environ, FFXI_COVERAGE='1')
    if o.live: env['FFXI_LIVE'] = os.path.abspath(o.live)
    run([sys.executable, os.path.join(ROOT, 'pipeline', 'diffemu_gfx.py'), dll, out, os.path.join(g, 'case.json'), res, '--trials', str(o.trials)], env=env)
    r = json.loads(open(res).readline())
    cov = r.get('coverage') or {}
    print('%s 0x%08X (fork %s): %s; %s; coverage %s of %s instructions' % (sym, addr, o.file, r['verdict'], r.get('original_outcomes'),
                                                                        cov.get('covered'), cov.get('insns')))
    if r.get('first_mismatch'): print('  first mismatch:', json.dumps(r['first_mismatch'])[:1500])
    if r.get('unresolved'): print('  unresolved:', json.dumps(r['unresolved'])[:800])
    print('  (gate record: %s)' % res)
    sys.exit(0 if r['verdict'] == 'PASS' else 1)


def main():
    ap = argparse.ArgumentParser(); sp = ap.add_subparsers(dest='cmd', required=True)
    a = sp.add_parser('new'); a.add_argument('name'); a.add_argument('dir', nargs='?', default='.')
    b = sp.add_parser('diff'); b.add_argument('file')
    c = sp.add_parser('gate'); c.add_argument('file'); c.add_argument('--trials', type=int, default=100); c.add_argument('--live')
    c.add_argument('--work', default=os.path.join(ROOT, 'runs', 'fork'))
    o = ap.parse_args()
    if not SRC: sys.exit('fork.py: no tree source (tree/src, or tree_src next to this script)')
    {'new': cmd_new, 'diff': cmd_diff, 'gate': cmd_gate}[o.cmd](o)


if __name__ == '__main__':
    main()
