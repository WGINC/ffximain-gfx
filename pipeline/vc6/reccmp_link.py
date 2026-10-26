"""reccmp_link.py -- link recompile/ into DLLs with PDBs and compare them with reccmp (isledecomp's recompilation compare).

reccmp compares a linked binary plus its PDB with the original; match.py compares each COFF object with the original.
This builds what reccmp needs from the same sources and compiler flags as match.py (cl /nologo /c /Gy + --flags,
default /O2), plus /Z7 (CodeView in the object; `match.py --all --flags "/O2 /Z7"` is IDENTICAL for every function,
so the debug information changes no code byte), and links with Visual C++ 6.0's link.exe:

    link /NOLOGO /DLL /NOENTRY /NODEFAULTLIB /DEBUG /INCREMENTAL:NO /OPT:NOREF /OUT:FFXIMAIN.DLL /PDB:FFXIMAIN.pdb *.obj stubs.obj

Two DLLs, one per language: recompile/*.c and recompile/*.cpp. small_math.cpp and two perm_*.c files define the same
extern "C" function at the same address (0x10026E90 Vec4_Copy, 0x100272B0 Vec3_ScaleInPlace), so one DLL would hold
a duplicate symbol. Each group's annotated addresses are checked to be unique.

Unresolved externals (functions and globals the sources only declare) are given stub definitions, generated from the
linker's own LNK2001 list into stubs.c / stubs.cpp, each annotated so reccmp can map it to the game's address:
  * `_FUN_XXXXXXXX` (C, a name carrying an address as match.py's ADDR_RE reads it) -> `// STUB: FFXIMAIN 0x...`
    and an empty function (a STUB is matched by address and never compared);
  * `_DAT_XXXXXXXX` (C) or `?DAT_xxxxxxxx@@3PAU<T>@@A` (C++, a pointer to struct T) -> `// GLOBAL: FFXIMAIN 0x...`
    and a definition of that type.
Any other unresolved symbol stops the script (no /FORCE: a forced link leaves a reference pointing nowhere). The
`_DAT_` rule has no instance in recompile/ yet (untested); the other two are used.
No library is linked (/NODEFAULTLIB): in recompile/ as it is, the only CRT symbol the objects reference is __fltused
(the compiler's marker that floating point is used), and LIBC.LIB's __fltused pulls in its floating-point start-up and
then crt0.obj, which needs a main. It is defined in the stubs instead (an int, not annotated: it has no game address).
/INCREMENTAL:NO: /DEBUG alone links incrementally, and every call then goes through a `jmp` thunk, which reccmp shows
as `<OFFSETn>` where the game has the callee's name (the 11 functions that call a stub were under 100% without this flag).

    FFXI_DLL=... VC6=... WINEPREFIX=... python3 pipeline/vc6/reccmp_link.py OUT_DIR [--flags "/O2"] \
        [--reccmp DIR_WITH_reccmp-reccmp] [--match] [--sizes] [--skip-build]

OUT_DIR (outside the repository): OUT_DIR/orig/FFXIMAIN.dll is a symlink to $FFXI_DLL (reccmp takes the target name
from the original's file name); OUT_DIR/<c|cpp>/{src,build}. With --reccmp, runs
`reccmp-reccmp --paths ORIG DLL PDB SRC --json OUT_DIR/<group>/reccmp.json` per group, or with --sizes
`reccmp-reccmp --target FFXIMAIN` from generated project files whose CSV data source gives each annotated function's
size in the game (reccmp_sizes.json). With --match also match.py --json per source file. Then one line per annotated
address: match.py's verdict, reccmp's accuracy (OUT_DIR/compare[_sizes].json). docs/RECCMP.md has the results.
"""
import argparse, glob, json, os, re, shutil, subprocess, sys
P = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.normpath(os.path.join(P, '..', '..'))
sys.path.insert(0, P)
from match import FUNC_RE, ADDR_RE   # the same marker and address-name rules as match.py


def win(path):
    return 'Z:' + os.path.abspath(path).replace('/', '\\')


def env():
    vc6 = os.environ['VC6']
    return dict(os.environ, WINEDEBUG='-all', INCLUDE=win(os.path.join(vc6, 'INCLUDE')), LIB=win(os.path.join(vc6, 'LIB')))


def tool(name, args, cwd):
    exe = os.path.join(os.environ['VC6'], os.environ.get('VC6_BIN', 'BIN'), name)
    r = subprocess.run(['wine', exe] + args, cwd=cwd, env=env(), capture_output=True, text=True, errors='replace')
    return r.returncode, (r.stdout + r.stderr).replace('\r', '')


def compile_one(src, build, flags):
    obj = os.path.splitext(os.path.basename(src))[0] + '.obj'
    rc, out = tool('cl.exe', ['/nologo', '/c', '/Gy'] + flags + ['/Z7', '/Fo' + obj, win(src)], build)
    if rc: sys.exit('compile failed: %s\n%s' % (src, out))
    return obj


def link(objs, build):
    return tool('link.exe', ['/NOLOGO', '/DLL', '/NOENTRY', '/NODEFAULTLIB', '/DEBUG', '/INCREMENTAL:NO', '/OPT:NOREF', '/OUT:FFXIMAIN.DLL',
                             '/PDB:FFXIMAIN.pdb'] + objs, build)


def addr_of(name):
    m = ADDR_RE.search(name)
    return int(m.group(1), 16) if m else None


def stub_for(sym):
    """(address, annotation, definition) for an unresolved symbol, or None"""
    if sym == '__fltused':   # the CRT's marker that floating point is used; no game address, not annotated
        return -1, None, 'int _fltused;'
    m = re.fullmatch(r'_((?:FUN|sub|fn)_[0-9A-Fa-f]{8})', sym)
    if m: return addr_of(m.group(1)), 'STUB', 'void %s(void) {}' % m.group(1)
    m = re.fullmatch(r'_((?:FUN|sub|fn)_[0-9A-Fa-f]{8})@(\d+)', sym)
    if m:
        n = int(m.group(2)) // 4
        return addr_of(m.group(1)), 'STUB', 'void __stdcall %s(%s) {}' % (m.group(1), ', '.join('int a%d' % i for i in range(n)) or 'void')
    m = re.fullmatch(r'_(DAT_[0-9A-Fa-f]{8})', sym)
    if m: return addr_of(m.group(1)), 'GLOBAL', 'int %s;' % m.group(1)
    m = re.fullmatch(r'\?(DAT_[0-9A-Fa-f]{8})@@3PAU(\w+)@@A', sym)
    # the struct's declaration goes on the line before the marker: reccmp names a GLOBAL from the line after it
    if m: return addr_of(m.group(1)), 'GLOBAL', 'struct %s;\n%s *%s;' % (m.group(2), m.group(2), m.group(1))
    return None


def marks(path):
    return [int(m.group(1), 16) for l in open(path, errors='replace') for m in [FUNC_RE.search(l)] if m]


def build_group(out, group, files, flags):
    src, build = os.path.join(out, group, 'src'), os.path.join(out, group, 'build')
    for d in (src, build): shutil.rmtree(d, ignore_errors=True); os.makedirs(d)
    seen = {}
    for f in files:
        for a in marks(f):
            if a in seen: sys.exit('0x%08X annotated in both %s and %s' % (a, seen[a], f))
            seen[a] = f
        shutil.copy(f, src)
    objs = [compile_one(os.path.join(src, os.path.basename(f)), build, flags) for f in files]
    rc, out_ = link(objs, build)
    # C++ symbols are listed undecorated and then decorated in parentheses: "struct Cam * X" (?X@@3PAUCam@@A)
    unresolved = sorted(set(a or b for a, b in re.findall(r'unresolved external symbol (?:"[^"]*" \((\S+)\)|(\S+))', out_)))
    if unresolved:
        stubs = []
        for s in unresolved:
            st = stub_for(s)
            if st is None or st[0] is None: sys.exit('no stub rule for unresolved %s\n%s' % (s, out_))
            stubs.append(st)
        ext = '.c' if group == 'c' else '.cpp'
        path = os.path.join(src, 'stubs' + ext)
        with open(path, 'w') as fh:
            fh.write('/* reccmp_link.py: definitions for the externals the sources only declare (from the linker\'s\n'
                     '   unresolved list), annotated with the game address their name carries. Not compared. */\n')
            for a, kind, d in sorted(stubs):
                if ext == '.cpp' and kind != 'GLOBAL': d = 'extern "C" { %s }' % d    # a definition with C linkage
                pre, _, d = d.rpartition('\n')
                fh.write((pre + '\n' if pre else '') + ('// %s: FFXIMAIN 0x%08X\n' % (kind, a) if kind else '') + d + '\n')
        objs.append(compile_one(path, build, flags))
        rc, out_ = link(objs, build)
    if rc: sys.exit('link failed (%s):\n%s' % (group, out_))
    print('%s: %d sources, %d stubs -> %s' % (group, len(files), len(unresolved), os.path.join(build, 'FFXIMAIN.DLL')))
    if out_.strip(): print('  link: ' + out_.strip().replace('\n', '\n  link: '))
    return src, build, seen


def project_files(out, g, src, build, seen):
    """reccmp-project.yml / reccmp-user.yml / reccmp-build.yml for target FFXIMAIN, with a CSV data source giving each
    annotated function's size in the game (data/func_extents.json, single-piece extents only). Without a size reccmp
    reads the original over the recompiled function's length (reccmp/compare/functions.py: orig_size = recomp_size)."""
    import hashlib
    ext = json.load(open(os.path.join(ROOT, 'data', 'func_extents.json')))
    proj = os.path.join(out, g, 'proj'); os.makedirs(proj, exist_ok=True)
    with open(os.path.join(proj, 'sizes.csv'), 'w') as fh:
        fh.write('address|size\n')
        for a in sorted(seen):
            e = ext.get('0x%08x' % a) or ext.get('0x%08X' % a)
            if e and len(e) == 1: fh.write('%08x|%d\n' % (a, e[0][1] - e[0][0]))
    sha = hashlib.sha256(open(os.environ['FFXI_DLL'], 'rb').read()).hexdigest()
    open(os.path.join(proj, 'reccmp-project.yml'), 'w').write(
        'targets:\n  FFXIMAIN:\n    filename: FFXIMAIN.dll\n    source-root: %s\n    hash:\n      sha256: %s\n'
        '    data-sources:\n      - sizes.csv\n' % (os.path.relpath(src, proj), sha))
    open(os.path.join(proj, 'reccmp-user.yml'), 'w').write(
        'targets:\n  FFXIMAIN:\n    path: %s\n' % os.path.relpath(os.path.join(out, 'orig', 'FFXIMAIN.dll'), proj))
    open(os.path.join(build, 'reccmp-build.yml'), 'w').write(
        'project: %s\ntargets:\n  FFXIMAIN:\n    path: FFXIMAIN.DLL\n    pdb: FFXIMAIN.pdb\n' % proj)   # absolute: reccmp resolves a relative one against the yml file's own path


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('out'); ap.add_argument('--flags', default='/O2'); ap.add_argument('--reccmp'); ap.add_argument('--match', action='store_true')
    ap.add_argument('--sizes', action='store_true', help="run reccmp from project files with the game's function sizes (CSV)")
    ap.add_argument('--skip-build', action='store_true', help='reuse OUT_DIR/<group>/{src,build} from an earlier run')
    o = ap.parse_args()
    out = os.path.abspath(o.out)
    if out.startswith(ROOT + os.sep): sys.exit('OUT_DIR must be outside the repository')
    orig = os.path.join(out, 'orig', 'FFXIMAIN.dll')
    os.makedirs(os.path.dirname(orig), exist_ok=True)
    if os.path.lexists(orig): os.remove(orig)
    os.symlink(os.path.abspath(os.environ['FFXI_DLL']), orig)
    groups = {'c': sorted(glob.glob(os.path.join(ROOT, 'recompile', '*.c'))),
              'cpp': sorted(glob.glob(os.path.join(ROOT, 'recompile', '*.cpp')))}
    rows = {}
    tag = '_sizes' if o.sizes else ''
    for g, files in groups.items():
        if o.skip_build:
            src, build = os.path.join(out, g, 'src'), os.path.join(out, g, 'build')
            seen = {a: f for f in files for a in marks(f)}
        else:
            src, build, seen = build_group(out, g, files, o.flags.split())
        rec = {}
        if o.reccmp:
            js = os.path.join(out, g, 'reccmp%s.json' % tag)
            if o.sizes:
                project_files(out, g, src, build, seen)
                args = ['--target', 'FFXIMAIN']
            else:
                args = ['--paths', orig, os.path.join(build, 'FFXIMAIN.DLL'), os.path.join(build, 'FFXIMAIN.pdb'), src]
            r = subprocess.run([os.path.join(o.reccmp, 'reccmp-reccmp')] + args + ['--json', js, '--print-rec-addr', '--no-color'],
                               cwd=build, capture_output=True, text=True, errors='replace')
            open(os.path.join(out, g, 'reccmp%s.txt' % tag), 'w').write(r.stdout + r.stderr)
            if r.returncode: sys.exit('reccmp failed (%s):\n%s' % (g, (r.stdout + r.stderr)[-3000:]))
            for e in json.load(open(js)).get('data', []):
                rec[int(e['address'], 16)] = e
        mat = {}
        if o.match:
            for f in files:
                js = os.path.join(out, g, os.path.basename(f) + '.match.json')
                subprocess.run([sys.executable, os.path.join(P, 'match.py'), f, '--flags', o.flags, '--json', js], capture_output=True)
                for k, v in json.load(open(js)).items(): mat[int(k, 16)] = v['verdict']
        for a, f in seen.items():
            e = rec.get(a)
            rv = 'NOT SEEN' if o.reccmp and e is None else ('' if e is None else
                 ('stub' if e.get('stub') else '%.2f%%%s' % (100 * e.get('matching', 0), ' (effective)' if e.get('effective') else '')))
            rows[(a, f)] = (os.path.basename(f), mat.get(a, ''), rv, (e or {}).get('name', ''))
    for (a, _), (f, m, r, n) in sorted(rows.items()):
        print('0x%08X  %-16s  %-10s  %-18s %s' % (a, f, m, r, n))
    json.dump([{'address': '0x%08X' % a, 'file': f, 'match': m, 'reccmp': r, 'name': n} for (a, _), (f, m, r, n) in sorted(rows.items())],
              open(os.path.join(out, 'compare%s.json' % tag), 'w'), indent=1)


if __name__ == '__main__':
    main()
