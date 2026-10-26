"""permute.py -- search for a source the game's compiler turns into a function's bytes (decomp-permuter), keep it
only where match.py's comparison says IDENTICAL.

The permuter (github.com/simonlindholm/decomp-permuter, not in this repo: PERMUTER points at a checkout) rewrites a
C function at random (variable splits, statement order, casts, ...) and scores each version's object against the
target's instructions. It takes C (pycparser) and ELF objects, so:
  * the function's source is the sweep's (sweep.py --keep DIR: f_<ADDR>.c with vc6_shim.h); only C functions
    (__cdecl, __stdcall) are taken. For parsing, the MSVC keywords the shim and source use are defined away
    (`-DPERMUTER`); a __stdcall function is compiled with /Gz (stdcall the default), since the permuter's output has
    lost the keyword, and `long long` (the parser's __int64) is written back as __int64;
  * Visual C++ 6.0's COFF object is converted to ELF (MinGW objcopy, leading underscore removed);
  * the target object is the game's bytes over the function's extent, built under runs/permute (never committed:
    game code).
A result the permuter scores 0 is compiled again by match.py at /O2 (with __stdcall written back) and kept only if
IDENTICAL. What it shows is that this source gives the game's bytes, nothing about the original source: the
permuter's rewrites (a `volatile` parameter, a split variable) are what made the bytes, and are left as they are.

    FFXI_DLL=... VC6=... PERMUTER=... python3 pipeline/vc6/permute.py KEEP_DIR SWEEP.json OUT_DIR ADDR [ADDR ...]
        [--seconds 120] [--jobs 2]
OUT_DIR gets perm_<ADDR>.c for every proven function, with its `// FUNCTION: FFXIMAIN` line (recompile/: checked by
match.py --all).
"""
import argparse, concurrent.futures as CF, json, os, re, shutil, subprocess, sys
P = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.join(P, '..', '..'); sys.path.insert(0, P)
import match as M

PRELUDE = '''#ifdef PERMUTER
#define __cdecl
#define __stdcall
#define __fastcall
#define __declspec(x)
#define __int64 long long
#endif
'''


def setup(keep, ea, conv, work):
    d = os.path.join(work, '%08X' % ea); shutil.rmtree(d, ignore_errors=True); os.makedirs(d)
    src = open(os.path.join(keep, 'f_%08X.c' % ea)).read()
    shim = open(os.path.join(keep, 'vc6_shim.h')).read()
    shim = re.sub(r'(?m)^#pragma.*\n', '', shim)
    open(os.path.join(d, 'base.c'), 'w').write(PRELUDE + src.replace('#include "vc6_shim.h"\n', shim, 1))
    gz = '/Gz' if conv == '__stdcall' else '/Gd'
    open(os.path.join(d, 'compile.sh'), 'w').write('''#!/bin/bash
set -e
IN="$(realpath "$1")"; OUT="$(realpath -m "$3")"; D=$(mktemp -d)
sed 's/long long/__int64/g' "$IN" > $D/x.c
cd $D && WINEDEBUG=-all INCLUDE="Z:%s" wine "%s" /nologo /c /O2 /Gy %s x.c >/dev/null 2>&1
i686-w64-mingw32-objcopy --remove-leading-char -I pe-i386 -O elf32-i386 $D/x.obj $D/x.o
# a __stdcall name carries @N: the permuter finds the function by its plain name
for s in $(i686-w64-mingw32-nm $D/x.o | awk '$3 ~ /@[0-9]+$/ {print $3}'); do
  i686-w64-mingw32-objcopy --redefine-sym "$s=${s%%@*}" $D/x.o; done
mv $D/x.o "$OUT"; rm -rf $D
''' % (os.path.join(os.environ['VC6'], 'INCLUDE').replace('/', '\\\\'), os.path.join(os.environ['VC6'], 'BIN', 'cl.exe'), gz))
    os.chmod(os.path.join(d, 'compile.sh'), 0o755)
    return d


def target(img, ext, ea, name, d):
    e = ext.get('0x%08x' % ea) or ext.get('0x%08X' % ea)
    if not e or len(e) != 1: return False
    open(os.path.join(d, 't.bin'), 'wb').write(img[e[0][0] - M.BASE:e[0][1] - M.BASE])
    subprocess.run(['i686-w64-mingw32-objcopy', '-I', 'binary', '-O', 'elf32-i386', '-B', 'i386', '--rename-section',
                    '.data=.text,alloc,load,readonly,code,contents', '--redefine-sym', '_binary_t_bin_start=' + name,
                    '-N', '_binary_t_bin_end', '-N', '_binary_t_bin_size', 't.bin', 'target.o'], cwd=d, check=True)
    os.remove(os.path.join(d, 't.bin'))
    open(os.path.join(d, 'settings.toml'), 'w').write('func_name = "%s"\ncompiler_type = "base"\n' % name)
    return True


def one(args):
    keep, ea, conv, name, work, seconds = args
    d = os.path.join(work, '%08X' % ea)
    try:
        subprocess.run([sys.executable, os.path.join(os.environ['PERMUTER'], 'permuter.py'), d, '--stop-on-zero', '-j', '1'],
                       cwd=os.environ['PERMUTER'], capture_output=True, timeout=seconds)
    except subprocess.TimeoutExpired:
        pass
    outs = sorted(x for x in os.listdir(d) if x.startswith('output-0-'))
    if not outs: return ea, None
    src = open(os.path.join(d, outs[0], 'source.c')).read().replace('long long', '__int64')
    if conv == '__stdcall':
        src = re.sub(r'(\b%s\s*\()' % re.escape(name), r'__stdcall \1', src, count=1)
    marked = re.sub(r'(?m)^(.*\b%s\s*\()' % re.escape(name), '// FUNCTION: FFXIMAIN 0x%08X\n\\1' % ea, src, count=1)
    chk = os.path.join(d, 'check.c'); open(chk, 'w').write(marked)
    r = subprocess.run([sys.executable, os.path.join(P, 'match.py'), chk], capture_output=True, text=True)
    return ea, (marked if re.search(r'0x%08X IDENTICAL\b' % ea, r.stdout) else None)


def main():
    ap = argparse.ArgumentParser(); ap.add_argument('keep'); ap.add_argument('sweep'); ap.add_argument('out'); ap.add_argument('addrs', nargs='+')
    ap.add_argument('--seconds', type=int, default=120); ap.add_argument('--jobs', type=int, default=2)
    o = ap.parse_args()
    import pefile
    img = pefile.PE(os.environ['FFXI_DLL'], fast_load=True).get_memory_mapped_image()
    ext = json.load(open(os.path.join(ROOT, 'data', 'func_extents.json')))
    sw = json.load(open(o.sweep)); work = os.path.join(ROOT, 'runs', 'permute'); os.makedirs(work, exist_ok=True)
    jobs = []
    for a in o.addrs:
        ea = int(a, 16); v = sw.get('0x%08X' % ea) or {}
        if v.get('conv') not in ('__cdecl', '__stdcall') or not os.path.exists(os.path.join(o.keep, 'f_%08X.c' % ea)): continue
        defs = re.findall(r'(?m)^(?!#)[^\n]*?\b(\w+)\s*\([^;{]*\)\s*\n\{', open(os.path.join(o.keep, 'f_%08X.c' % ea)).read())
        if not defs: continue
        m = re.match(r'(\w+)', defs[-1])                     # the function is the file's last definition
        d = setup(o.keep, ea, v['conv'], work)
        if target(img, ext, ea, m.group(1), d): jobs.append((o.keep, ea, v['conv'], m.group(1), work, o.seconds))
    got = {}
    os.makedirs(o.out, exist_ok=True)
    with CF.ThreadPoolExecutor(max_workers=o.jobs) as ex:
        for ea, src in ex.map(one, jobs):             # each written as it is proven: a long batch can be stopped
            print('0x%08X %s' % (ea, 'IDENTICAL' if src else 'not found'), flush=True)
            if not src: continue
            got[ea] = src
            open(os.path.join(o.out, 'perm_%08X.c' % ea), 'w').write(
                '/* pipeline/vc6/permute.py: a source decomp-permuter found and match.py proved identical at /O2. It gives the\n'
                '   game\'s bytes; it is not the original source (the permuter\'s rewrites are what made it). */\n' + src)
    print('%d of %d proven' % (len(got), len(jobs)))


if __name__ == '__main__':
    main()
