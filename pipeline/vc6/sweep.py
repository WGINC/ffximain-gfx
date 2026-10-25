"""sweep.py -- which of the tree's own functions the game's compiler turns into the game's bytes, as they are.

Takes tree/src's functions, writes each into its own source for Visual C++ 6.0 (setup_vc6.sh) with only mechanical
changes, compiles them under Wine, and compares each with the game's bytes (match.py's comparison: every byte
outside a relocation, each call's target by the address its callee's name carries, the length up to padding):
  * the types the tree uses (uint32_t, undefined4, ...) from a shim header (vc6_shim.h, written next to them);
  * `XI_API` dropped (a class method `RET [CC] Class::Method(...)` is taken the same way: without CC a member, its
    `void * self = this` line dropped; only where it does not use the class's layout, `this->`); a `__cdecl` / `__stdcall` function is compiled as C (the decompiler's int/pointer mixing is a
    C warning, not an error), a `__thiscall` one as a member function of an empty struct with `self_` bound to
    `this` (C++: Visual C++ 6.0 takes `__thiscall` only on members).
  * a global (tree/include/gfx_globals.hpp, `XI_EA(type, address)`) becomes `(*(type *)address)`: the compiler
    puts the absolute address in the instruction, which is the game's own relocated value at its preferred base;
  * a call to the game's code (`orig::Name`, tree/include/gfx_orig.hpp) becomes a direct call of an external
    function named `FUN_<address>` with the wrapper's convention and parameter types (a `__thiscall` one a member of
    a struct of that name, `((FUN_<address> *)a0)->f(...)`), so the comparison checks its target. A wrapper with an
    `XI_ANY` parameter (any 4 bytes: its conversion is the wrapper's, not the source's) or a type the shim does not
    define leaves the function out, counted by why.
Nothing else is changed: the point is what the tree's code compiles to, not a source tuned to match.

    FFXI_DLL=... VC6=... python3 pipeline/vc6/sweep.py OUT.json [--flags "/O2"] [--keep DIR] [--only 0xADDR,...]
OUT.json: {"0xADDR": {"verdict": IDENTICAL | IDENTICAL_UNCOMPARED (equal bytes, but a relocation's target could not be
compared: its symbol names no address) | DIFFERS | NO_COMPILE | LEFT_OUT (not converted, with why), "why", "conv"}}."""
import argparse, glob, json, os, re, shutil, subprocess, sys, tempfile
P = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.join(P, '..', '..'); sys.path.insert(0, P)
import match as M

SHIM = r'''/* vc6_shim.h: the tree's type names for Visual C++ 6.0 (pipeline/vc6/sweep.py) */
typedef unsigned char uint8_t; typedef signed char int8_t; typedef unsigned short uint16_t; typedef short int16_t;
typedef unsigned int uint32_t; typedef int int32_t; typedef unsigned __int64 uint64_t; typedef __int64 int64_t;
typedef unsigned char undefined, undefined1, byte, uchar; typedef unsigned short undefined2, ushort, word;
typedef unsigned int undefined4, uint, dword; typedef unsigned __int64 undefined8; typedef unsigned long ulong;
typedef unsigned int size_t; typedef int BOOL;
typedef void *LPVOID; typedef unsigned long DWORD; typedef unsigned int _DWORD; typedef unsigned short _WORD;
typedef unsigned char _BYTE;
#ifndef __cplusplus
typedef unsigned char bool;
#define true 1
#define false 0
typedef int code();              /* Ghidra's code: a function of any arguments */
#else
typedef int code(...);
#endif
/* the decompiler's helpers, as tree/include/xi_ghidra.hpp defines them */
#define NAN(x) ((x) != (x))
#define CONCAT31(h, l) ((uint32_t)(((uint32_t)(h) << 8) | (uint8_t)(l)))
#define xi_sw(x) ((uint32_t)(x))
double __cdecl sqrt(double);
#define SQRT(x) sqrt(x)
'''
SIG = re.compile(r'^XI_API\s+(?P<ret>.+?)\s+(?P<cc>__cdecl|__stdcall|__thiscall|__fastcall)\s+(?P<name>\w+)\((?P<args>[^)]*)\)\s*$', re.M)
# the class-method form (Hex-Rays route): `RET [CC] Class::Method(ARGS)`; without CC a member (this in ECX)
MSIG = re.compile(r'^(?P<ret>[^\n/#]+?)\s+(?:(?P<cc>__cdecl|__stdcall|__thiscall|__fastcall)\s+)?(?P<cls>\w+)::(?P<name>\w+)\((?P<args>[^)]*)\)\s*$', re.M)


ORIG = re.compile(r'^XI_ORIG (?P<ret>.+?) (?P<name>\w+)\((?P<params>[^)]*)\) \{ (?:return )?reinterpret_cast<(?P<rt>.+?) '
                  r'\((?P<cc>__\w+) \*\)\((?P<types>[^)]*)\)>\(XI_GAME\((?P<addr>0x[0-9A-Fa-f]+)u\)\)', re.M)
GLOB = re.compile(r'^typedef (?P<type>.+?) (?P<name>\w+)_t(?P<arr>\[\d*\])?;\s*\n#define (?P=name) XI_EA\((?P=name)_t, (?P<addr>0x[0-9A-Fa-f]+)u\)', re.M)
BASIC = {'void', 'char', 'short', 'int', 'long', 'float', 'double', 'unsigned', 'signed', 'uint8_t', 'int8_t', 'uint16_t',
         'int16_t', 'uint32_t', 'int32_t', 'uint64_t', 'int64_t', 'undefined', 'undefined1', 'undefined2', 'undefined4',
         'undefined8', 'byte', 'uchar', 'ushort', 'word', 'uint', 'dword', 'ulong', 'size_t', 'BOOL', 'bool', 'code',
         'LPVOID', 'DWORD', '_DWORD', '_WORD', '_BYTE'}


def plain(ty):
    """the type is made of shim types and pointers only"""
    return all(w in BASIC for w in re.findall(r'[A-Za-z_]\w*', ty.replace('const', '')))


def externs():
    o = {m.group('name'): m.groupdict() for m in ORIG.finditer(open(os.path.join(ROOT, 'tree', 'include', 'gfx_orig.hpp')).read())}
    g = {m.group('name'): m.groupdict() for m in GLOB.finditer(open(os.path.join(ROOT, 'tree', 'include', 'gfx_globals.hpp')).read())}
    return o, g


def split_args(s):
    out, depth, cur = [], 0, ''
    for ch in s:
        if ch in '([': depth += 1
        elif ch in ')]': depth -= 1
        if ch == ',' and depth == 0: out.append(cur.strip()); cur = ''; continue
        cur += ch
    if cur.strip(): out.append(cur.strip())
    return out


def rewrite_calls(body, orig, c_mode=False):
    """(new body, [declarations]) or (None, why). In C (c_mode) an XI_ANY parameter is declared `unsigned int`: C
    converts a pointer to it with a warning, and a wrong conversion can only make the bytes differ, never match; in
    C++ it is a template parameter deduced from the argument; a __thiscall callee needs C++"""
    decls, out, i = {}, '', 0
    while True:
        k = body.find('orig::', i)
        if k < 0: out += body[i:]; break
        m = re.match(r'orig::(\w+)\s*\(', body[k:])
        if not m: return None, 'orig:: not a call'
        w = orig.get(m.group(1))
        if w is None: return None, 'no wrapper for %s' % m.group(1)
        if c_mode and w['cc'] == '__thiscall': return None, 'thiscall callee'
        raw = [x.strip() for x in w['types'].split(',')] if w['types'].strip() else []
        # an XI_ANY parameter: unsigned int in C; in C++ a template parameter, deduced from the argument, so the call
        # passes the argument's own type (the mangled name still carries FUN_<address>)
        tpl = [] if c_mode else ['A%d' % n for n, x in enumerate(raw) if x == 'XI_ANY']
        types = [('unsigned int' if c_mode else 'A%d' % n) if x == 'XI_ANY' else x for n, x in enumerate(raw)]
        tp = 'template <%s> ' % ', '.join('class ' + x for x in tpl) if tpl else ''
        if not plain(w['rt']) or not all(plain(x) for x in types if x not in tpl): return None, 'type the shim lacks'
        # the call's argument list, balanced
        j = k + m.end(); depth = 1
        while depth and j < len(body):
            depth += {'(': 1, ')': -1}.get(body[j], 0); j += 1
        args = split_args(body[k + m.end():j - 1])
        sym = 'FUN_%08X' % int(w['addr'], 16)
        if w['cc'] == '__thiscall':
            if not types or len(args) != len(types): return None, 'thiscall arity'
            sname = 'FUNt_%08X' % int(w['addr'], 16)
            decls[sname] = 'struct %s { %s%s f(%s); };' % (sname, tp, w['rt'], ', '.join(types[1:]) or 'void')
            out += body[i:k] + '((%s *)(%s))->f(%s)' % (sname, args[0], ', '.join(args[1:]))
        else:
            decls[sym] = '%s%s %s %s(%s);' % (tp or ('' if c_mode else 'extern "C" '), w['rt'], w['cc'], sym, ', '.join(types) or 'void')
            out += body[i:k] + '%s(%s)' % (sym, ', '.join(args))
        i = j
    return out, list(decls.values())


def candidates(only=None, calls=False):
    out = []
    for f in sorted(glob.glob(os.path.join(ROOT, 'tree', 'src', '*.cpp'))):
        t = open(f).read()
        parts = re.split(r'(?m)^// FUNCTION: FFXIMAIN (0x[0-9A-F]+).*\n', t)
        for i in range(1, len(parts), 2):
            ea = int(parts[i], 16); body = parts[i + 1]
            if only and ea not in only: continue
            m = SIG.search(body) or MSIG.search(body.split('{', 1)[0] + '{')
            if not m: continue
            end = body.find('\n}\n', m.end())
            if end < 0: continue
            src = body[m.start():end + 2]
            if 'XI_' in src.replace('XI_API', '') or '__attribute__' in src: continue
            if not calls and ('orig::' in src or re.search(r'\b(DAT|g|dword|byte|word|flt|dbl|PTR|off|unk|stru|s|u|_)_[0-9A-Za-z_]*\b', src.split('{', 1)[1])):
                continue
            out.append((ea, m, src))
    return out


def convert(m, src, orig=None, glob_=None):
    """(file suffix, source) or (None, why)"""
    ret, cc, name, args = m.group('ret'), m.group('cc'), m.group('name'), m.group('args').strip()
    body = src[src.index('{'):]
    pre = []
    if 'cls' in m.groupdict():
        if re.search(r'\bthis->', body) or 'alignas' in body: return None, 'uses the class layout or alignas'
        # class pointers in the signature: forward declarations (their layout is not needed for a pointer)
        for c in sorted(set(re.findall(r'\b([A-Z]\w*)\s*\*', ret + ' ' + args)) - BASIC - {'LPVOID'}):
            pre.append('struct %s;' % c)
        if cc == '__thiscall' and args and re.match(r'^void\s*\*\s*self$', [x.strip() for x in args.split(',')][0]):
            args = re.sub(r'^void\s*\*\s*self\b', 'void * self_', args); body = re.sub(r'\bself\b', 'self_', body)
        elif cc is None:
            cc = '__thiscall'; args = 'void * self_' + (', ' + args if args else '')
            body = re.sub(r'void \* self = reinterpret_cast<void \*>\(this\);[^\n]*\n', '', body)
            body = re.sub(r'\bself\b', 'self_', body)
    if orig is not None:
        for g in sorted(set(re.findall(r'\b\w+\b', body)) & set(glob_)):
            gd = glob_[g]
            if not plain(gd['type']): return None, 'global type the shim lacks'
            if gd.get('arr'): pre.append('#define %s (*(%s (*)%s)%su)' % (g, gd['type'], gd['arr'], gd['addr']))
            else: pre.append('#define %s (*(%s *)%su)' % (g, gd['type'], gd['addr']))
        if 'orig::' in body:
            nb, d = rewrite_calls(body, orig, c_mode=cc != '__thiscall')
            cc_cpp = nb is None or cc == '__thiscall'
            if nb is None: nb, d = rewrite_calls(body, orig)
            if nb is None: return None, d
            body = nb; pre += d
        else: cc_cpp = False
    else: cc_cpp = False
    head = '#include "vc6_shim.h"\n' + ''.join(x + '\n' for x in pre)
    if cc in ('__cdecl', '__stdcall', '__fastcall'):
        if cc_cpp: return '.cpp', head + 'extern "C" %s %s %s(%s)\n%s' % (ret, cc, name, args or 'void', body)
        return '.c', head + '%s %s %s(%s)\n%s' % (ret, cc, name, args or 'void', body)
    a = [x.strip() for x in args.split(',')] if args else []
    if not a or not re.match(r'^void\s*\*\s*self_$', a[0]): return None, 'thiscall without self_ first'
    rest = ', '.join(a[1:]) or 'void'
    return '.cpp', (head + 'struct XS { %s %s(%s); };\n%s XS::%s(%s)\n{\n  void *self_ = this;\n%s\n}\n'
                    % (ret, name, rest, ret, name, rest, body[1:].rstrip().rstrip('}')))


def main():
    ap = argparse.ArgumentParser(); ap.add_argument('out'); ap.add_argument('--flags', default='/O2'); ap.add_argument('--keep')
    ap.add_argument('--only', default=''); ap.add_argument('--calls', action='store_true', help='also functions that call the game or name its globals')
    o = ap.parse_args()
    only = {int(x, 16) for x in o.only.split(',') if x}
    import pefile
    img = pefile.PE(os.environ['FFXI_DLL'], fast_load=True).get_memory_mapped_image()
    ext = json.load(open(os.path.join(ROOT, 'data', 'func_extents.json')))
    d = o.keep or tempfile.mkdtemp(prefix='vc6sweep_'); os.makedirs(d, exist_ok=True)
    open(os.path.join(d, 'vc6_shim.h'), 'w').write(SHIM)
    res, files = {}, []
    orig, glob_ = externs() if o.calls else (None, None)
    for ea, m, src in candidates(only, o.calls):
        c = convert(m, src, orig, glob_)
        k = '0x%08X' % ea
        if c[0] is None: res[k] = {'verdict': 'LEFT_OUT', 'why': c[1], 'conv': m.group('cc')}; continue
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
            if v == 'IDENTICAL' and notes: v, why = 'IDENTICAL_UNCOMPARED', '; '.join(notes)   # a relocation not checked
            res[key] = {'verdict': v, 'why': why, 'conv': cc, 'size': len(code)}
    json.dump(dict(sorted(res.items())), open(o.out, 'w'), indent=1)
    import collections
    c = collections.Counter(v['verdict'] for v in res.values())
    print('%d functions: %s' % (len(res), ', '.join('%s %d' % kv for kv in sorted(c.items()))))
    if not o.keep: shutil.rmtree(d)


if __name__ == '__main__':
    main()
