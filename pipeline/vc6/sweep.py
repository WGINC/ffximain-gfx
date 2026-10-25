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

    FFXI_DLL=... VC6=... python3 pipeline/vc6/sweep.py OUT.json [--flags "/O2"] [--keep DIR] [--only 0xADDR,...] [--jobs N] [--cache DIR]
OUT.json: {"0xADDR": {"verdict": IDENTICAL | IDENTICAL_UNCOMPARED (equal bytes, but a relocation's target could not be
compared: its symbol names no address) | DIFFERS | NO_COMPILE | LEFT_OUT (not converted, with why), "why", "conv"}}."""
import argparse, collections, glob, hashlib, json, os, re, shutil, subprocess, sys, tempfile
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
#define XI_GAME(ea) (ea)
/* XI_CALL_ST0(0x10311C2C)(x): 0x10311C2C is the CRT's __ftol, byte for byte LIBC.LIB's (sweep.py FTOL), which the
   compiler calls for a floating-point to integer conversion */
#define XI_FTOL(x) ((__int64)(x))
typedef unsigned char XI_PT1; typedef unsigned short XI_PT2; typedef unsigned int XI_PT4; typedef unsigned __int64 XI_PT8;
typedef signed char XI_PS1; typedef short XI_PS2; typedef int XI_PS4; typedef __int64 XI_PS8;
#define XI_PIECE(x, o, s) (*(XI_PT##s *)((char *)&(x) + (o)))
#define XI_PIECE_S(x, o, s) (*(XI_PS##s *)((char *)&(x) + (o)))
/* a 3-byte piece has no type: its value, read from the 4-byte word (sweep.py macros rewrites XI_PIECE(x, o, 3)) */
#define XI_PIECE3(x, o) ((((uint32_t)(x)) >> (8 * (o))) & 0xFFFFFFu)
#define CONCAT13(h, l) ((uint32_t)(((uint32_t)(uint8_t)(h) << 24) | ((uint32_t)(l) & 0xFFFFFFu)))
void *__cdecl memcpy(void *, const void *, size_t);
#pragma intrinsic(memcpy)
#define qmemcpy memcpy
__declspec(dllimport) DWORD __stdcall timeGetTime(void);
#ifdef __cplusplus
#define nullptr 0
/* xi_castbits<To>(x) (tree/include/xi_ghidra.hpp): the bits of x as a To; To deduced from a null pointer, which
   Visual C++ 6.0 handles where an explicit template argument it does not (sweep.py macros) */
template <class To, class From> inline To xi_castbits_(From f, To *) { To t; memcpy(&t, &f, sizeof t); return t; }
#endif
'''
SIG = re.compile(r'^XI_API\s+(?P<ret>.+?)\s+(?P<cc>__cdecl|__stdcall|__thiscall|__fastcall)\s+(?P<name>\w+)\((?P<args>[^)]*)\)\s*$', re.M)
# the class-method form (Hex-Rays route): `RET [CC] Class::Method(ARGS)`; without CC a member (this in ECX)
MSIG = re.compile(r'^(?P<ret>[^\n/#]+?)\s+(?:(?P<cc>__cdecl|__stdcall|__thiscall|__fastcall)\s+)?(?P<cls>\w+)::(?P<name>\w+)\((?P<args>[^)]*)\)\s*$', re.M)


ORIG = re.compile(r'^XI_ORIG (?P<ret>.+?) (?P<name>\w+)\((?P<params>[^)]*)\) \{ (?:return )?reinterpret_cast<(?P<rt>.+?) '
                  r'\((?P<cc>__\w+) \*\)\((?P<types>[^)]*)\)>\(XI_GAME\((?P<addr>0x[0-9A-Fa-f]+)u\)\)', re.M)
# a variadic wrapper: `template <class... R> XI_ORIG RET NAME(fixed..., R... rest) { ... (CC *)(...)>(XI_GAME(ADDR)) ...`
VORIG = re.compile(r'^template <class\.\.\. R> XI_ORIG (?P<ret>.+?) (?P<name>\w+)\((?P<params>[^)]*?),?\s*R\.\.\. rest\) \{ (?:return )?'
                   r'reinterpret_cast<(?P<rt>.+?) \((?P<cc>__\w+) \*\)\((?P<types>[^)]*?), decltype[^>]*>\(XI_GAME\((?P<addr>0x[0-9A-Fa-f]+)u\)\)', re.M)
GLOB = re.compile(r'^typedef (?P<type>.+?) (?P<name>\w+)_t(?P<arr>\[\d*\])?;\s*\n#define (?P=name) XI_EA\((?P=name)_t, (?P<addr>0x[0-9A-Fa-f]+)u\)', re.M)
BASIC = {'void', 'char', 'short', 'int', 'long', 'float', 'double', 'unsigned', 'signed', 'uint8_t', 'int8_t', 'uint16_t',
         'int16_t', 'uint32_t', 'int32_t', 'uint64_t', 'int64_t', 'undefined', 'undefined1', 'undefined2', 'undefined4',
         'undefined8', 'byte', 'uchar', 'ushort', 'word', 'uint', 'dword', 'ulong', 'size_t', 'BOOL', 'bool', 'code',
         'LPVOID', 'DWORD', '_DWORD', '_WORD', '_BYTE'}


def plain(ty):
    """the type is made of shim types and pointers only"""
    return all(w in BASIC for w in re.findall(r'[A-Za-z_]\w*', ty.replace('const', '')))


WINT = {'FARPROC': 'typedef int (__stdcall *FARPROC)();', 'HMODULE': 'typedef void *HMODULE;', 'CHAR': 'typedef char CHAR;',
        'HWND': 'typedef void *HWND;', 'HANDLE': 'typedef void *HANDLE;', 'xi_codefn': 'typedef code xi_codefn;',
        '_UNKNOWN': 'typedef unsigned char _UNKNOWN;', 'FILE': 'struct _iobuf; typedef struct _iobuf FILE;'}


def type_decls(ty):
    """the declarations a global's type needs beyond the shim: a Windows typedef, a forward declaration for a pointer
    to a named type, a one-int placeholder for one held by value (its fields and size are not known: code reading
    them fails to compile, code using its size compiles to other bytes, neither can match by chance)"""
    out = []
    for w in re.findall(r'[A-Za-z_]\w*', ty.replace('const', '')):
        if w in BASIC or w in ('__int16', '__int8', '__int32', '__int64'): continue
        if w in WINT: out.append(WINT[w]); continue
        if re.search(r'\b%s\s*\*' % w, ty): out.append('struct %s;' % w)
        else: out.append('struct %s { int xi_unknown_layout; };' % w)
    return out


def externs():
    src = open(os.path.join(ROOT, 'tree', 'include', 'gfx_orig.hpp')).read()
    o = {m.group('name'): m.groupdict() for m in ORIG.finditer(src)}
    for m in VORIG.finditer(src): o[m.group('name')] = dict(m.groupdict(), variadic=True)
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
        if w.get('variadic') and len(args) < len(types): return None, 'variadic arity'
        sym = 'FUN_%08X' % int(w['addr'], 16)
        if w['cc'] == '__thiscall':
            if not types or len(args) != len(types): return None, 'thiscall arity'
            sname = 'FUNt_%08X' % int(w['addr'], 16)
            decls[sname] = 'struct %s { %s%s f(%s); };' % (sname, tp, w['rt'], ', '.join(types[1:]) or 'void')
            out += body[i:k] + '((%s *)(%s))->f(%s)' % (sname, args[0], ', '.join(args[1:]))
        else:
            if w.get('variadic'):       # a caller-cleanup variadic call: its pushes are the call site's (promoted, as the game's)
                if tpl: return None, 'variadic with XI_ANY in C++'
                decls[sym] = '%s%s %s %s(%s);' % ('' if c_mode else 'extern "C" ', w['rt'], w['cc'], sym, ', '.join(types + ['...']))
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
            if '__attribute__' in src.split('{', 1)[0]: continue          # a register convention VC6 cannot declare
            if not calls and ('orig::' in src or re.search(r'\b(DAT|g|dword|byte|word|flt|dbl|PTR|off|unk|stru|s|u|_)_[0-9A-Za-z_]*\b', src.split('{', 1)[1])):
                continue
            out.append((ea, m, src))
    return out


_CLASSES = None


def class_fields(cls):
    """the field lines of tree/include's `class XI_API cls { ... };` (its methods and static_asserts left out), the
    other class names its fields point to, or None when there is no such class or a field is of a type the shim lacks"""
    global _CLASSES
    if _CLASSES is None:
        _CLASSES = {}
        for f in glob.glob(os.path.join(ROOT, 'tree', 'include', '*.hpp')):
            for m in re.finditer(r'(?ms)^class XI_API (\w+) \{\n(.*?)^\};', open(f).read()):
                _CLASSES[m.group(1)] = m.group(2)
    body = _CLASSES.get(cls)
    if body is None: return None
    lines, ptrs = [], set()
    for l in body.splitlines():
        s = l.split('//')[0].strip()
        if not s or s.endswith(':') or '(' in s: continue           # public:, a method
        decl = re.sub(r'\[[^\]]*\]', '', s.rstrip(';')).rsplit(None, 1)[0]
        for w in re.findall(r'[A-Za-z_]\w*', decl):
            if w in BASIC or w in ('__int16', '__int8', '__int32', '__int64', 'const'): continue
            if re.search(r'\b%s\s*\*' % w, decl): ptrs.add(w); continue
            return None
        lines.append('  ' + s)
    return lines, ptrs


FTOL = 0x10311C2C          # LIBC.LIB's __ftol, the same 39 bytes at this address (checked by ftol_check below)
_ADDRS = None


def ftol_check(img):
    """FTOL holds LIBC.LIB's __ftol: the library member's code against the DLL's bytes (else the mapping is not used)"""
    lib = open(os.path.join(os.environ['VC6'], 'LIB', 'LIBC.LIB'), 'rb').read(); p = 8
    while p < len(lib):
        size = int(lib[p + 48:p + 58].decode().strip()); data = lib[p + 60:p + 60 + size]
        if data[:2] == b'\x4c\x01' and b'__ftol' in data:
            f = tempfile.mktemp(suffix='.obj'); open(f, 'wb').write(data)
            try: funcs, _ = M.coff(f)
            except Exception: funcs = {}
            os.remove(f)
            if '__ftol' in funcs:
                code, rels = funcs['__ftol']
                return not rels and bytes(img[FTOL - 0x10000000:FTOL - 0x10000000 + len(code)]) == code
        p += 60 + size + (size & 1)
    return False


def code_addrs():
    """{name: address} of the tree's game-function addresses (gfx_orig.hpp `constexpr uint32_t NAME = 0x...u;`)"""
    global _ADDRS
    if _ADDRS is None:
        _ADDRS = {m.group(1): int(m.group(2), 16) for m in re.finditer(r'constexpr uint32_t (\w+) = (0x[0-9A-Fa-f]+)u;',
                                                                       open(os.path.join(ROOT, 'tree', 'include', 'gfx_orig.hpp')).read())}
    return _ADDRS


def _at_calls(body):
    """`at<T>(obj, off)` (tree/include/gfx_common.hpp: the field of type T at byte offset off) written out as the
    expression it returns, `(*(T *)((char *)(obj) + (off)))`"""
    out, i = [], 0
    while True:
        j = body.find('at<', i)
        while j > 0 and (body[j - 1].isalnum() or body[j - 1] == '_'): j = body.find('at<', j + 3)
        if j < 0: out.append(body[i:]); return ''.join(out)
        k, depth = j + 3, 1
        while k < len(body) and depth:
            depth += {'<': 1, '>': -1}.get(body[k], 0); k += 1
        if depth or k >= len(body) or body[k] != '(': return None
        T = body[j + 3:k - 1]
        e = _group(body, k)
        if e < 0: return None
        inner, d, cut = body[k + 1:e - 1], 0, None
        for q, ch in enumerate(inner):
            d += {'(': 1, ')': -1, '<': 0}.get(ch, 0)
            if ch == ',' and d == 0: cut = q; break
        if cut is None: return None
        obj, off = inner[:cut].strip(), inner[cut + 1:].strip()
        inner_c = _at_calls(obj)
        if inner_c is None: return None
        out.append(body[i:j] + '(*(%s *)((char *)(%s) + (%s)))' % (T, inner_c, off)); i = e


def _castbits(body):
    """`xi_castbits<T>(x)` as `xi_castbits_((x), (T *)0)` (the shim's: T deduced)"""
    out, i = [], 0
    while True:
        j = body.find('xi_castbits<', i)
        if j < 0: out.append(body[i:]); return ''.join(out)
        k, depth = j + len('xi_castbits<'), 1
        while k < len(body) and depth:
            depth += {'<': 1, '>': -1}.get(body[k], 0); k += 1
        if depth or k >= len(body) or body[k] != '(': return None
        T = body[j + len('xi_castbits<'):k - 1]
        e = _group(body, k)
        if e < 0: return None
        inner = _castbits(body[k + 1:e - 1])
        if inner is None: return None
        out.append(body[i:j] + 'xi_castbits_((%s), (%s *)0)' % (inner, T)); i = e


def macros(body):
    """the tree's XI_ helpers the shim cannot define as they are, rewritten; (body, None) or (None, why)"""
    body = re.sub(r'\s*__attribute__\(\(aligned\(\d+\)\)\)', '', body)
    body = re.sub(r'\balignas\(\d+\)\s*', '', body)          # the frame's alignment: the frame layout is the game's
    body = _at_calls(body)
    if body is None: return None, 'at<> not parsed'
    body = re.sub(r'XI_PIECE\(([^(),]+), (\d+), 3\)', r'XI_PIECE3(\1, \2)', body)
    body = _castbits(body)
    if body is None: return None, 'xi_castbits not parsed'
    if re.search(r'XI_CALL_ST0\((?!0x10311C2Cu\))', body): return None, 'XI_CALL_ST0 of another function'
    body = body.replace('XI_CALL_ST0(0x10311C2Cu)', 'XI_FTOL')
    def code(m):
        a = code_addrs().get(m.group(1))
        return '((void *)0x%08Xu)' % a if a is not None else m.group(0)
    body = re.sub(r'XI_CODE\((\w+)\)', code, body)
    if 'XI_CODE(' in body: return None, 'XI_CODE of an unknown name'
    if 'XI_ANY' in body: return None, 'XI_ANY in the body'
    return body, None


def convert(m, src, orig=None, glob_=None):
    """(file suffix, source) or (None, why)"""
    ret, cc, name, args = m.group('ret'), m.group('cc'), m.group('name'), m.group('args').strip()
    body = src[src.index('{'):]
    body, why = macros(body)
    if body is None: return None, why
    force_cpp = bool(re.search(r'reinterpret_cast|static_cast|&\s*\w+\s*=\s*\*', body))   # C++ references and casts
    pre = []
    if 'cls' in m.groupdict():
        if 'alignas' in body: return None, 'alignas'
        layout = None
        if re.search(r'\bthis->', body):
            if cc is not None: return None, 'this-> outside a member'
            layout = class_fields(m.group('cls'))
            if layout is None: return None, 'class layout the shim lacks'
        # class pointers in the signature: forward declarations (their layout is not needed for a pointer)
        for c in sorted(set(re.findall(r'\b([A-Z]\w*)\s*\*', ret + ' ' + args)) - BASIC - {'LPVOID'}):
            pre.append('struct %s;' % c)
        if cc == '__thiscall' and args and re.match(r'^void\s*\*\s*self$', [x.strip() for x in args.split(',')][0]):
            args = re.sub(r'^void\s*\*\s*self\b', 'void * self_', args); body = re.sub(r'\bself\b', 'self_', body)
        elif cc is None and layout is not None:
            # a member that uses its class's fields: the class's fields as a struct, the method in it
            for c in sorted(layout[1]): pre.append('struct %s;' % c)
            cls = m.group('cls')
            head = '#include "vc6_shim.h"\n' + ''.join(x + '\n' for x in pre)
            for g in sorted(set(re.findall(r'\b\w+\b', body)) & set(glob_ or ())):
                gd = glob_[g]
                if not plain(gd['type']): return None, 'global type the shim lacks'
                head += ('#define %s (*(%s (*)%s)%su)\n' % (g, gd['type'], gd['arr'], gd['addr']) if gd.get('arr')
                         else '#define %s (*(%s *)%su)\n' % (g, gd['type'], gd['addr']))
            if 'orig::' in body:
                body, d = rewrite_calls(body, orig)
                if body is None: return None, d
                head += ''.join(x + '\n' for x in d)
            return '.cpp', (head + '#pragma pack(push, 1)\nstruct %s {\n  %s %s(%s);\n%s\n};\n#pragma pack(pop)\n%s %s::%s(%s)\n%s'
                            % (cls, ret, name, args or 'void', '\n'.join(layout[0]), ret, cls, name, args or 'void', body))
        elif cc is None:
            cc = '__thiscall'; args = 'void * self_' + (', ' + args if args else '')
            body = re.sub(r'void \* self = reinterpret_cast<void \*>\(this\);[^\n]*\n', '', body)
            body = re.sub(r'\bself\b', 'self_', body)
    if orig is not None:
        for g in sorted(set(re.findall(r'\b\w+\b', body)) & set(glob_)):
            gd = glob_[g]
            d = type_decls(gd['type'])
            if d is None: return None, 'global type the shim lacks'
            pre += [x for x in d if x not in pre]
            if gd.get('arr'): pre.append('#define %s (*(%s (*)%s)%su)' % (g, gd['type'], gd['arr'], gd['addr']))
            else: pre.append('#define %s (*(%s *)%su)' % (g, gd['type'], gd['addr']))
        if 'orig::' in body:
            nb, d = rewrite_calls(body, orig, c_mode=cc != '__thiscall' and not force_cpp)
            cc_cpp = nb is None or cc == '__thiscall' or force_cpp
            if nb is None: nb, d = rewrite_calls(body, orig)
            if nb is None: return None, d
            body = nb; pre += d
        else: cc_cpp = False
    else: cc_cpp = False
    head = '#include "vc6_shim.h"\n' + ''.join(x + '\n' for x in pre)
    if cc in ('__cdecl', '__stdcall', '__fastcall'):
        if cc_cpp or force_cpp: return '.cpp', head + 'extern "C" %s %s %s(%s)\n%s' % (ret, cc, name, args or 'void', body)
        return '.c', head + '%s %s %s(%s)\n%s' % (ret, cc, name, args or 'void', body)
    a = [x.strip() for x in args.split(',')] if args else []
    if not a or not re.match(r'^void\s*\*\s*self_$', a[0]): return None, 'thiscall without self_ first'
    rest = ', '.join(a[1:]) or 'void'
    return '.cpp', (head + 'struct XS { %s %s(%s); };\n%s XS::%s(%s)\n{\n  void *self_ = this;\n%s\n}\n'
                    % (ret, name, rest, ret, name, rest, body[1:].rstrip().rstrip('}')))


def _group(s, i):
    """the end (exclusive) of the balanced parenthesis group opening at s[i]"""
    n = 0
    for j in range(i, len(s)):
        n += {'(': 1, ')': -1}.get(s[j], 0)
        if n == 0: return j + 1
    return -1


def _strip(e):
    """the expression without outer parentheses and an outer (uint32_t) cast"""
    e = e.strip()
    while True:
        if e.startswith('(uint32_t)'): e = e[len('(uint32_t)'):].strip(); continue
        if e.startswith('(') and _group(e, 0) == len(e): e = e[1:-1].strip(); continue
        return e


def _low_part(e, bits):
    """for `(HIGH << bits) | (uintN_t)(LOW)` (a decompiler's register piece: HIGH what the register held), LOW"""
    e = _strip(e)
    t = ' << %d) | (uint%d_t)(' % (bits, bits)
    i = e.find(t)
    if i < 0 or not e.startswith('('): return None
    if _group(e, 0) != i + len(' << %d)' % bits): return None          # the left side is the one group ending there
    j = i + len(t) - 1
    k = _group(e, j)
    if k != len(e): return None
    return e[j + 1:k - 1]


def _rule_ret_piece(text, ret, name, bits, ty):
    """every `return (HIGH << bits) | (uintN_t)(LOW);` becomes `return LOW;` with the function returning `ty`. What the
    decompiler wrote as HIGH is what the register held: a source returning only the low piece leaves it there"""
    out, pos, n = [], 0, 0
    for m in re.finditer(r'\breturn\b', text):
        if m.start() < pos: continue
        end = text.find(';', m.end())
        if end < 0: return None
        e = text[m.end():end]
        low = _low_part(e, bits)
        if low is not None: out.append(text[pos:m.start()] + 'return (%s)(%s)' % (ty, low)); n += 1
        else: out.append(text[pos:end])
        pos = end
    if not n: return None
    t = ''.join(out) + text[pos:]
    t2 = re.sub(r'(^|[{;]\s*|\n)' + re.escape(ret) + r'(\s+(?:__\w+\s+)?(?:XS::)?' + re.escape(name) + r'\()', lambda mm: mm.group(1) + ty + mm.group(2), t)
    return t2 if t2 != t else None


def _rule_ge0(text, ret, name):
    """`-1 < X` for an int X becomes `X >= 0`: the same comparison, written as the decompiler does not"""
    t = re.sub(r'-1 < (\(int\)\s*\w+|\(int\)\s*(?=\()|\w+\b)', lambda m: '__GE0__' + m.group(1), text)
    if t == text: return None
    out, i = [], 0
    while True:
        j = t.find('__GE0__', i)
        if j < 0: out.append(t[i:]); break
        out.append(t[i:j]); j += len('__GE0__')
        m = re.match(r'\(int\)\s*', t[j:])
        k = j + (m.end() if m else 0)
        if t[k] == '(': e = _group(t, k)
        else: e = k + re.match(r'\w+', t[k:]).end()
        if e < 0: return None
        out.append('(%s >= 0)' % t[j:e]); i = e
    return ''.join(out)


_LIT = re.compile(r"^(?:-?(?:0x[0-9a-fA-F]+|\d+)|'\\x[0-9a-fA-F]{2}'|'[^'\\]')$")


def _lit_val(t):
    if t.startswith("'"): return int(t[3:5], 16) if t.startswith("'\\x") else ord(t[1])
    return int(t, 0)


def _cmp_sites(text):
    """[(start, end, lhs, rhs, literal side)] for each parenthesised `(A < B)` with one side an integer literal"""
    out = []
    for i, ch in enumerate(text):
        if ch != '(': continue
        e = _group(text, i)
        if e < 0: continue
        inner = text[i + 1:e - 1]
        # one top-level ` < `, nothing else at top level that binds looser
        depth, cut = 0, []
        for j in range(len(inner)):
            c = inner[j]
            if c == '(': depth += 1
            elif c == ')': depth -= 1
            elif depth == 0 and inner.startswith(' < ', j): cut.append(j)
            elif depth == 0 and (inner.startswith('&&', j) or inner.startswith('||', j) or c in '?,='): cut = None; break
        if not cut or len(cut) != 1: continue
        a, b = inner[:cut[0]].strip(), inner[cut[0] + 3:].strip()
        if _LIT.match(a) and not _LIT.match(b): out.append((i, e, a, b, 'l'))
        elif _LIT.match(b) and not _LIT.match(a): out.append((i, e, a, b, 'r'))
    # nested sites overlap: keep the innermost-free ones only (no site inside another)
    return [x for x in out if not any(y is not x and y[0] < x[0] and x[1] <= y[1] for y in out)]


def _rule_cmpk(text, ret, name):
    """`(C < X)` becomes `(X >= C+1)` and `(X < C)` becomes `(X <= C-1)`: the same integer comparison, as the source
    may have written it. Each site either way, every combination up to 4 sites (else all at once); the variants are
    returned joined by a marker the caller splits"""
    sites = _cmp_sites(text)
    if not sites: return None
    def flip(t, chosen):
        for (i, e, a, b, side) in sorted(chosen, reverse=True):
            if side == 'l': new = '(%s >= %d)' % (b, _lit_val(a) + 1)
            else: new = '(%s <= %d)' % (a, _lit_val(b) - 1)
            t = t[:i] + new + t[e:]
        return t
    import itertools
    if len(sites) <= 4:
        combos = [c for r in range(1, len(sites) + 1) for c in itertools.combinations(sites, r)]
    else:
        combos = [sites]
    return [flip(text, c) for c in combos]


def _rule_retthis(text, ret, name):
    """a `__thiscall` function written as returning void whose code may return `this` (EAX = ECX at the end, as a
    setter returning *this does): `void` becomes `void *` and every return returns `this`"""
    if ret != 'void' or 'XS::' not in text: return None
    t = re.sub(r'\breturn\s*;', 'return this;', text)
    body_end = t.rstrip().rfind('}')
    t = t[:body_end] + '  return this;\n' + t[body_end:]
    t2 = re.sub(r'(^|[{;]\s*|\n)void(\s+(?:XS::)?' + re.escape(name) + r'\()', lambda m: m.group(1) + 'void *' + m.group(2), t)
    return t2 if t2 != t else None


# the rewrite rules (--rules): each a mechanical change of how the code is written, never of what it computes; a
# result counts only where match.py's comparison says IDENTICAL
RULES = [
    ('ret8', lambda t, r, n: _rule_ret_piece(t, r, n, 8, 'unsigned char')),
    ('ret16', lambda t, r, n: _rule_ret_piece(t, r, n, 16, 'unsigned short')),
    ('ge0', _rule_ge0),
    ('cmpk', _rule_cmpk),
    ('retthis', _rule_retthis),
]


def apply_rules(text, ret, name):
    """[(rule, text)] for every rule that changes the text, and every rule together"""
    out, t_all, names = [], text, []
    for rn, f in RULES:
        t = f(text, ret, name)
        if t is None: continue
        if isinstance(t, list):                       # a rule with several variants (cmpk): each on its own
            out += [('%s%d' % (rn, k), x) for k, x in enumerate(t)]
            t2 = f(t_all, ret, name)
            if t2: t_all = t2[-1]; names.append(rn)
            continue
        out.append((rn, t))
        t2 = f(t_all, ret, name)
        if t2 is not None and not isinstance(t2, list): t_all = t2; names.append(rn)
    if len(names) > 1: out.append(('_'.join(names), t_all))
    return out


def toolchain_hash():
    """the compiler's own files (BIN's executables and DLLs): a different compiler is a different object"""
    h = hashlib.sha256()
    b = os.path.join(os.environ['VC6'], 'BIN')
    for f in sorted(os.listdir(b)):
        if f.lower().endswith(('.exe', '.dll')): h.update(f.lower().encode() + b'\0' + open(os.path.join(b, f), 'rb').read())
    return h.hexdigest()


def cache_key(src, shim, flags, tc):
    h = hashlib.sha256()
    for x in (src, shim.encode(), flags.encode(), tc.encode()): h.update(hashlib.sha256(x).digest())
    return h.hexdigest()


def main():
    ap = argparse.ArgumentParser(); ap.add_argument('out'); ap.add_argument('--flags', default='/O2'); ap.add_argument('--keep')
    ap.add_argument('--jobs', type=int, default=os.cpu_count())
    ap.add_argument('--rules', action='store_true', help='also try each DIFFERS function with the rewrite rules (RULES)')
    ap.add_argument('--cache', default=os.path.join(ROOT, 'runs', 'vc6_cache'), help="objects by source hash ('' for none)")
    ap.add_argument('--only', default=''); ap.add_argument('--calls', action='store_true', help='also functions that call the game or name its globals')
    o = ap.parse_args()
    only = {int(x, 16) for x in o.only.split(',') if x}
    import pefile
    img = pefile.PE(os.environ['FFXI_DLL'], fast_load=True).get_memory_mapped_image()
    ext = json.load(open(os.path.join(ROOT, 'data', 'func_extents.json')))
    d = o.keep or tempfile.mkdtemp(prefix='vc6sweep_'); os.makedirs(d, exist_ok=True)
    place = {'ftol': FTOL} if ftol_check(img) else {}
    print('__ftol: %s' % ('0x%08X is LIBC.LIB\'s, byte for byte' % FTOL if place else 'not established; its calls stay uncompared'))
    open(os.path.join(d, 'vc6_shim.h'), 'w').write(SHIM)
    # the Direct3D 8 interfaces as the tree declares them (tree/include/xi_d3d8.hpp, from the SDK's d3d8.h), less the
    # static_asserts Visual C++ 6.0 does not have; included by the sources that name an IDirect3D interface
    d3d = ''.join(l for l in open(os.path.join(ROOT, 'tree', 'include', 'xi_d3d8.hpp')) if 'static_assert' not in l)
    open(os.path.join(d, 'xi_d3d8_vc6.h'), 'w').write(d3d)
    res, files, sig = {}, [], {}
    orig, glob_ = externs() if o.calls else (None, None)
    for ea, m, src in candidates(only, o.calls):
        c = convert(m, src, orig, glob_)
        k = '0x%08X' % ea
        if c[0] is None: res[k] = {'verdict': 'LEFT_OUT', 'why': c[1], 'conv': m.group('cc')}; continue
        text = c[1]
        if re.search(r'\bIDirect3D\w*8\b', text):
            text = text.replace('#include "vc6_shim.h"\n', '#include "vc6_shim.h"\n#include "xi_d3d8_vc6.h"\n', 1)
        fn = 'f_%08X%s' % (ea, c[0]); open(os.path.join(d, fn), 'w').write(text); files.append((ea, fn, m.group('cc')))
        sig[ea] = (m.group('ret').strip(), m.group('name'))
    # compile in chunks (one cl run per chunk; an error stops only that file's object), --jobs chunks at a time.
    # An object is cached by the hash of what decides it: its source, the shim, the flags and the compiler's own
    # files (cache_key). The comparison with the game is redone every run: it depends on the DLL and the facts.
    cache = o.cache and os.path.abspath(o.cache)
    if cache: os.makedirs(cache, exist_ok=True)
    tc = toolchain_hash() if cache else None
    env = dict(os.environ, WINEDEBUG='-all', INCLUDE='Z:' + os.path.join(os.environ['VC6'], 'INCLUDE').replace('/', '\\'))
    import concurrent.futures as CF, time

    d3dh = open(os.path.join(d, 'xi_d3d8_vc6.h'), 'rb').read()
    def key_of(fn):
        src = open(os.path.join(d, fn), 'rb').read()
        return cache_key(src + (d3dh if b'xi_d3d8_vc6.h' in src else b''), SHIM, o.flags, tc)

    def compile_chunk(chunk):
        subprocess.run(['wine', os.path.join(os.environ['VC6'], 'BIN', 'cl.exe'), '/nologo', '/c', '/Gy'] + o.flags.split() + [f for _, f, _ in chunk],
                       cwd=d, env=env, capture_output=True, text=True, errors='replace')
        if cache:
            for _, fn, _ in chunk:
                obj, c = os.path.join(d, fn.rsplit('.', 1)[0] + '.obj'), os.path.join(cache, key_of(fn))
                if os.path.exists(obj): shutil.copyfile(obj, c + '.tmp'); os.replace(c + '.tmp', c + '.obj')
                else: open(c + '.fail', 'w').close()

    def judge(files, what):
        """compiles (or takes from the cache) every (ea, file, cc), and gives each its verdict"""
        todo, hits = [], 0
        for ea, fn, cc in files:
            obj = os.path.join(d, fn.rsplit('.', 1)[0] + '.obj')
            if os.path.exists(obj): os.remove(obj)
            c = cache and os.path.join(cache, key_of(fn))
            if c and os.path.exists(c + '.obj'): shutil.copyfile(c + '.obj', obj); hits += 1
            elif c and os.path.exists(c + '.fail'): hits += 1
            else: todo.append((ea, fn, cc))
        t0 = time.time()
        with CF.ThreadPoolExecutor(max_workers=o.jobs) as ex:
            list(ex.map(compile_chunk, [todo[k:k + 40] for k in range(0, len(todo), 40)]))
        print('%s: compiled %d sources in %.0f s (%d jobs); %d from the cache' % (what, len(todo), time.time() - t0, o.jobs, hits), flush=True)
        out = {}
        for ea, fn, cc in files:
            obj = os.path.join(d, fn.rsplit('.', 1)[0] + '.obj'); key = '0x%08X' % ea
            if not os.path.exists(obj): out[fn] = {'verdict': 'NO_COMPILE', 'why': 'cl failed', 'conv': cc}; continue
            funcs, _ = M.coff(obj)
            if len(funcs) != 1: out[fn] = {'verdict': 'NO_COMPILE', 'why': '%d functions in the object' % len(funcs), 'conv': cc}; continue
            (sym, (code, rels)), = funcs.items()
            v, why, notes = M.compare(img, ext, ea, code, rels, place)
            if v == 'IDENTICAL' and notes: v, why = 'IDENTICAL_UNCOMPARED', '; '.join(notes)   # a relocation not checked
            out[fn] = {'verdict': v, 'why': why, 'conv': cc, 'size': len(code)}
        return out

    got = judge(files, 'the tree as written')
    for ea, fn, cc in files: res['0x%08X' % ea] = got[fn]
    # --rules: each DIFFERS function again with each rewrite rule that applies (RULES); the verdict stays the tree's
    # code's, and a rule that gives IDENTICAL is recorded with it ("rule_identical"): the bytes prove that source
    if o.rules:
        var, meta = [], {}
        for ea, fn, cc in files:
            if res['0x%08X' % ea]['verdict'] != 'DIFFERS': continue
            text = open(os.path.join(d, fn)).read()
            for rn, t in apply_rules(text, *sig[ea]):
                vf = 'f_%08X_%s%s' % (ea, rn, os.path.splitext(fn)[1]); open(os.path.join(d, vf), 'w').write(t)
                var.append((ea, vf, cc)); meta[vf] = rn
        got = judge(var, 'rewrite rules')
        tried = collections.Counter(); won = collections.Counter()
        for ea, vf, cc in var:
            rn = meta[vf]; r = res['0x%08X' % ea]
            base = 'cmpk' if re.fullmatch(r'cmpk\d+', rn) else rn
            if base not in r.setdefault('rules_tried', []): r['rules_tried'].append(base); tried[base] += 1
            if got[vf]['verdict'] in ('IDENTICAL', 'IDENTICAL_UNCOMPARED') and 'rule_identical' not in r:
                r['rule_identical'] = rn; r['rule_verdict'] = got[vf]['verdict']; won[base] += 1
        for rn in sorted(tried): print('rule %s: applied to %d DIFFERS, IDENTICAL in %d' % (rn, tried[rn], won[rn]))
    json.dump(dict(sorted(res.items())), open(o.out, 'w'), indent=1)
    c = collections.Counter(v['verdict'] for v in res.values())
    print('%d functions: %s' % (len(res), ', '.join('%s %d' % kv for kv in sorted(c.items()))))
    if not o.keep: shutil.rmtree(d)


if __name__ == '__main__':
    main()
