"""sweep.py -- which of the tree's own functions the game's compiler turns into the game's bytes, as they are.

Takes tree/src's functions that call nothing (no `orig::`) and name no global, writes each into its own source for
Visual C++ 6.0 (setup_vc6.sh) with only mechanical changes, compiles them under Wine, and compares each with the
game's bytes (match.py's comparison: every byte outside a relocation, the length up to padding):
  * the types the tree uses (uint32_t, undefined4, ...) from a shim header (vc6_shim.h, written next to them);
  * `XI_API` dropped; a `__cdecl` / `__stdcall` function is compiled as C (the decompiler's int/pointer mixing is a
    C warning, not an error), a `__thiscall` one as a member function of an empty struct with `self_` bound to
    `this` (C++: Visual C++ 6.0 takes `__thiscall` only on members).
Nothing else is changed: the point is what the tree's code compiles to, not a source tuned to match.

    FFXI_DLL=... VC6=... python3 pipeline/vc6/sweep.py OUT.json [--flags "/O2"] [--keep DIR] [--only 0xADDR,...]
OUT.json: {"0xADDR": {"verdict": IDENTICAL | DIFFERS | NO_COMPILE, "why", "conv"}}."""
import argparse, glob, json, os, re, shutil, subprocess, sys, tempfile
P = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.join(P, '..', '..'); sys.path.insert(0, P)
import match as M

SHIM = r'''/* vc6_shim.h: the tree's type names for Visual C++ 6.0 (pipeline/vc6/sweep.py) */
typedef unsigned char uint8_t; typedef signed char int8_t; typedef unsigned short uint16_t; typedef short int16_t;
typedef unsigned int uint32_t; typedef int int32_t; typedef unsigned __int64 uint64_t; typedef __int64 int64_t;
typedef unsigned char undefined, undefined1, byte, uchar; typedef unsigned short undefined2, ushort, word;
typedef unsigned int undefined4, uint, dword; typedef unsigned __int64 undefined8; typedef unsigned long ulong;
typedef unsigned int size_t; typedef int BOOL;
'''
SIG = re.compile(r'^XI_API\s+(?P<ret>.+?)\s+(?P<cc>__cdecl|__stdcall|__thiscall|__fastcall)\s+(?P<name>\w+)\((?P<args>[^)]*)\)\s*$', re.M)


def candidates(only=None):
    out = []
    for f in sorted(glob.glob(os.path.join(ROOT, 'tree', 'src', '*.cpp'))):
        t = open(f).read()
        parts = re.split(r'(?m)^// FUNCTION: FFXIMAIN (0x[0-9A-F]+).*\n', t)
        for i in range(1, len(parts), 2):
            ea = int(parts[i], 16); body = parts[i + 1]
            if only and ea not in only: continue
            m = SIG.search(body)
            if not m: continue
            end = body.find('\n}\n', m.end())
            if end < 0: continue
            src = body[m.start():end + 2]
            if 'orig::' in src or 'XI_' in src.replace('XI_API', '') or '__attribute__' in src: continue
            if re.search(r'\b(DAT|g|dword|byte|word|flt|dbl|PTR|off|unk|stru|s|u|_)_[0-9A-Za-z_]*\b', src.split('{', 1)[1]): continue
            out.append((ea, m, src))
    return out


def convert(m, src):
    """(file suffix, source) or None"""
    ret, cc, name, args = m.group('ret'), m.group('cc'), m.group('name'), m.group('args').strip()
    body = src[src.index('{'):]
    if cc in ('__cdecl', '__stdcall', '__fastcall'):
        return '.c', '#include "vc6_shim.h"\n%s %s %s(%s)\n%s' % (ret, cc, name, args or 'void', body)
    a = [x.strip() for x in args.split(',')] if args else []
    if not a or not re.match(r'^void\s*\*\s*self_$', a[0]): return None
    rest = ', '.join(a[1:]) or 'void'
    return '.cpp', ('#include "vc6_shim.h"\nstruct XS { %s %s(%s); };\n%s XS::%s(%s)\n{\n  void *self_ = this;\n%s\n}\n'
                    % (ret, name, rest, ret, name, rest, body[1:].rstrip().rstrip('}')))


def main():
    ap = argparse.ArgumentParser(); ap.add_argument('out'); ap.add_argument('--flags', default='/O2'); ap.add_argument('--keep')
    ap.add_argument('--only', default=''); o = ap.parse_args()
    only = {int(x, 16) for x in o.only.split(',') if x}
    import pefile
    img = pefile.PE(os.environ['FFXI_DLL'], fast_load=True).get_memory_mapped_image()
    ext = json.load(open(os.path.join(ROOT, 'data', 'func_extents.json')))
    d = o.keep or tempfile.mkdtemp(prefix='vc6sweep_'); os.makedirs(d, exist_ok=True)
    open(os.path.join(d, 'vc6_shim.h'), 'w').write(SHIM)
    res, files = {}, []
    for ea, m, src in candidates(only):
        c = convert(m, src)
        k = '0x%08X' % ea
        if c is None: res[k] = {'verdict': 'NO_COMPILE', 'why': 'thiscall without self_ first', 'conv': m.group('cc')}; continue
        fn = 'f_%08X%s' % (ea, c[0]); open(os.path.join(d, fn), 'w').write(c[1]); files.append((ea, fn, m.group('cc')))
    # compile in chunks (one cl run per chunk; an error stops only that file's object)
    for k in range(0, len(files), 40):
        chunk = files[k:k + 40]
        env = dict(os.environ, WINEDEBUG='-all', INCLUDE='Z:' + os.path.join(os.environ['VC6'], 'INCLUDE').replace('/', '\\'))
        subprocess.run(['wine', os.path.join(os.environ['VC6'], 'BIN', 'cl.exe'), '/nologo', '/c', '/Gy'] + o.flags.split() + [f for _, f, _ in chunk],
                       cwd=d, env=env, capture_output=True, text=True, errors='replace')
        for ea, fn, cc in chunk:
            obj = os.path.join(d, fn.rsplit('.', 1)[0] + '.obj'); key = '0x%08X' % ea
            if not os.path.exists(obj): res[key] = {'verdict': 'NO_COMPILE', 'why': 'cl failed', 'conv': cc}; continue
            funcs, _ = M.coff(obj)
            if len(funcs) != 1: res[key] = {'verdict': 'NO_COMPILE', 'why': '%d functions in the object' % len(funcs), 'conv': cc}; continue
            (sym, (code, rels)), = funcs.items()
            v, why, notes = M.compare(img, ext, ea, code, rels, {})
            res[key] = {'verdict': v, 'why': why, 'conv': cc, 'size': len(code)}
    json.dump(dict(sorted(res.items())), open(o.out, 'w'), indent=1)
    import collections
    c = collections.Counter(v['verdict'] for v in res.values())
    print('%d functions: %s' % (len(res), ', '.join('%s %d' % kv for kv in sorted(c.items()))))
    if not o.keep: shutil.rmtree(d)


if __name__ == '__main__':
    main()
