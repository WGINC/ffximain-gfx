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
Nothing else is changed: the point is what the tree's code compiles to, not a source tuned to match. What the tree's
GCC build takes and Visual C++ 6.0 does not is spelled the way VC6 takes it, computing the same (task #116):
  * an implicit conversion GCC's -fpermissive takes (pointer to pointer, pointer to or from a 4-byte integer, an
    integer to an enum) as the cast to the type cl names in its C2440 / C2664 (permissive, after a first compile);
  * a C source naming a Direct3D interface includes the header for C (d3d_c); a call of an interface's C++ wrapper
    (`dev->SetRenderState(...)`) makes it C++; a struct a body or a global's type names through a pointer is declared
    with a typedef (C needs one);
  * a global holding a function pointer (an import's IAT slot) is declared at its address, as the other globals;
  * the decompilers' other virtual-call spellings (`*(int *)X`, Hex-Rays' `(*(R (__thiscall **)(P))(SLOT))(...)`)
    as the stand-in class call (_vcalls); a method's own `T * self = reinterpret_cast<T *>(this);` kept as its binding;
  * the tree's helpers (CONCATnm, LODWORD and the like, SUBnm, xi_k4/xi_k8, builtin_strncpy, __builtin_isunordered),
    the CRT's memcpy / memset / strcpy with C linkage (the intrinsic pragma needs it), `ull` as `ui64`, a hexadecimal
    floating literal as a decimal the compiler is checked to read back as the same double (hexf_check);
  * the shim's own inline functions emitted beside a function are not counted as the function (SHIM_HELPER).
Forms that were left out, converted (task #117):
  * a callee's parameter or return type the shim lacked, declared as the tree declares it (call_type_decls): VC6's
    own __int8 / __int16 / __int32 / __int64, a Windows typedef, a Direct3D type (its header), a pointer to a struct
    the tree declares (a forward declaration); a global of a Direct3D type held by value gets the header's layout;
  * a variadic wrapper whose fixed parameters are typed, `(void *, ...)`, as the caller-cleanup `...` call;
  * a `__thiscall` function whose first parameter (ECX) has another 4-byte type T: `T self_ = (T)this;`;
  * a class method using its fields that names a global of any type: the global as for the other functions.

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
#ifdef __cplusplus
extern "C"
#endif
double __cdecl sqrt(double);  /* C linkage, as <math.h> declares it */
#define SQRT(x) sqrt(x)
#define XI_GAME(ea) (ea)
/* XI_CALL_ST0(0x10311C2C)(x): 0x10311C2C is the CRT's __ftol, byte for byte LIBC.LIB's (sweep.py FTOL), which the
   compiler calls for a floating-point to integer conversion */
#define XI_FTOL(x) ((__int64)(x))
typedef unsigned char XI_PT1; typedef unsigned short XI_PT2; typedef unsigned int XI_PT4; typedef unsigned __int64 XI_PT8;
typedef signed char XI_PS1; typedef short XI_PS2; typedef int XI_PS4; typedef __int64 XI_PS8;
#define XI_PIECE(x, o, s) (*(XI_PT##s *)((char *)&(x) + (o)))
#define XI_PIECE_S(x, o, s) (*(XI_PS##s *)((char *)&(x) + (o)))
/* a piece of an array global (Visual C++ 6.0 takes no & of `*(T (*)[N])address`: C2101): from the array's first byte,
   the same address (sweep.py convert) */
#define XI_PIECEA(x, o, s) (*(XI_PT##s *)((char *)(x) + (o)))
#define XI_PIECEA_S(x, o, s) (*(XI_PS##s *)((char *)(x) + (o)))
/* a 3-byte piece has no type: its value, read from the 4-byte word (sweep.py macros rewrites XI_PIECE(x, o, 3)) */
#define XI_PIECE3(x, o) ((((uint32_t)(x)) >> (8 * (o))) & 0xFFFFFFu)
#define CONCAT13(h, l) ((uint32_t)(((uint32_t)(uint8_t)(h) << 24) | ((uint32_t)(l) & 0xFFFFFFu)))
/* the CRT's string functions (tree/include/xi_decomp.hpp: <string.h>), with C linkage in C++ as well: the intrinsic
   pragma takes only a C-linkage function (cl C4162) */
#ifdef __cplusplus
extern "C" {
#endif
void *__cdecl memcpy(void *, const void *, size_t);
void *__cdecl memset(void *, int, size_t);
char *__cdecl strcpy(char *, const char *);
/* and the others the tree's <string.h> declares that Visual C++ 6.0 has as intrinsics (its STRING.H, without _CRTIMP:
   the static CRT) */
int __cdecl memcmp(const void *, const void *, size_t);
char *__cdecl strcat(char *, const char *);
int __cdecl strcmp(const char *, const char *);
size_t __cdecl strlen(const char *);
#ifdef __cplusplus
}
#endif
#pragma intrinsic(memcpy, memset, strcpy, memcmp, strcat, strcmp, strlen)
/* __builtin_isunordered(a, b) (GCC): either is a NaN; _isnan (FLOAT.H): nonzero for a NaN */
#ifdef __cplusplus
extern "C" int __cdecl _isnan(double);
#else
int __cdecl _isnan(double);
#endif
#define __builtin_isunordered(a, b) (_isnan(a) || _isnan(b))
#define qmemcpy memcpy
/* more of the decompiler's helpers, as tree/include/xi_ghidra.hpp and xi_decomp.hpp define them */
typedef signed char sbyte; typedef __int64 longlong; typedef unsigned __int64 ulonglong; typedef unsigned __int64 qword;
typedef unsigned char xi_bool; typedef unsigned __int64 _QWORD;
#define __PAIR64__(high, low) (((uint64_t)(uint32_t)(high) << 32) | (uint32_t)(low))
#define __SPAIR64__(high, low) ((int64_t)__PAIR64__(high, low))
#define CONCAT11(h, l) ((uint16_t)(((uint16_t)(uint8_t)(h) << 8) | (uint8_t)(l)))
#define CONCAT12(h, l) ((uint32_t)(((uint32_t)(uint8_t)(h) << 16) | (uint16_t)(l)))
#define CONCAT21(h, l) ((uint32_t)(((uint32_t)(uint16_t)(h) << 8) | (uint8_t)(l)))
#define CONCAT22(h, l) ((uint32_t)(((uint32_t)(uint16_t)(h) << 16) | (uint16_t)(l)))
#define CONCAT43(h, l) ((uint64_t)(((uint64_t)(uint32_t)(h) << 24) | ((uint64_t)(l) & 0xFFFFFFui64)))
#define CONCAT44(h, l) ((uint64_t)(((uint64_t)(uint32_t)(h) << 32) | (uint32_t)(l)))
#define ZEXT14(x) ((uint32_t)(uint8_t)(x))
#define ZEXT24(x) ((uint32_t)(uint16_t)(x))
#define ZEXT48(x) ((uint64_t)(uint32_t)(x))
#define ZEXT12(x) ((uint16_t)(uint8_t)(x))
#define SEXT14(x) ((int32_t)(int8_t)(x))
#define SEXT24(x) ((int32_t)(int16_t)(x))
#define SEXT48(x) ((int64_t)(int32_t)(x))
#define SEXT12(x) ((int16_t)(int8_t)(x))
#define LAST_IND(x, part_type) (sizeof(x) / sizeof(part_type) - 1)
#define BYTEn(x, n)  (*((_BYTE *)&(x) + (n)))
#define WORDn(x, n)  (*((_WORD *)&(x) + (n)))
#define DWORDn(x, n) (*((_DWORD *)&(x) + (n)))
#define LOBYTE(x)  BYTEn(x, 0)
#define LOWORD(x)  WORDn(x, 0)
#define LODWORD(x) DWORDn(x, 0)
#define HIBYTE(x)  BYTEn(x, sizeof(x) - 1)
#define HIWORD(x)  WORDn(x, sizeof(x) / 2 - 1)
#define HIDWORD(x) DWORDn(x, 1)
#define BYTE1(x) BYTEn(x, 1)
#define BYTE2(x) BYTEn(x, 2)
#define BYTE3(x) BYTEn(x, 3)
#define BYTE4(x) BYTEn(x, 4)
#define WORD1(x) WORDn(x, 1)
#define WORD2(x) WORDn(x, 2)
#define SLOBYTE(x) (*((int8_t *)&(x)))
#define SLOWORD(x) (*((int16_t *)&(x)))
#define SHIWORD(x) (*((int16_t *)&(x) + 1))
#define SHIBYTE(x) (*((int8_t *)&(x) + sizeof(x) - 1))
#define SBYTE1(x) (*((int8_t *)&(x) + 1))
#define SLODWORD(x) (*((int32_t *)&(x)))
#define SHIDWORD(x) (*((int32_t *)&(x) + 1))
__declspec(dllimport) DWORD __stdcall timeGetTime(void);
#ifndef __cplusplus
typedef unsigned int XI_ANY;    /* any 4 bytes (tree/include/xi_ghidra_shared.hpp): in C, the word */
#define nullptr ((void *)0)
#endif
#ifdef __cplusplus
/* any 4 bytes, from an argument of any 4-byte type: passed on the stack as the word, as MSVC passes a 4-byte struct */
struct XI_ANY { unsigned int bits; XI_ANY() {} template <class T> XI_ANY(T v) : bits((unsigned int)v) {} };
#define nullptr 0
/* xi_castbits<To>(x) (tree/include/xi_ghidra.hpp): the bits of x as a To; To deduced from a null pointer, which
   Visual C++ 6.0 handles where an explicit template argument it does not (sweep.py macros) */
template <class To, class From> inline To xi_castbits_(From f, To *) { To t; memcpy(&t, &f, sizeof t); return t; }
/* COERCE_FLOAT(v) (tree/include/xi_decomp.hpp): xi_bitcast<float>((uint32_t)(v)), the word's bits as a float */
#define COERCE_FLOAT(v) xi_castbits_((uint32_t)(v), (float *)0)
#endif
'''
SIG = re.compile(r'^XI_API\s+(?P<ret>.+?)\s+(?P<cc>__cdecl|__stdcall|__thiscall|__fastcall)\s+(?P<name>\w+)\((?P<args>[^)]*)\)\s*$', re.M)
# the class-method form (Hex-Rays route): `RET [CC] Class::Method(ARGS)`; without CC a member (this in ECX)
MSIG = re.compile(r'^(?P<ret>[^\n/#]+?)\s+(?:(?P<cc>__cdecl|__stdcall|__thiscall|__fastcall)\s+)?(?P<cls>\w+)::(?P<name>\w+)\((?P<args>[^)]*)\)\s*$', re.M)


ORIG = re.compile(r'^XI_ORIG (?P<ret>.+?) (?P<name>\w+)\((?P<params>[^)]*)\) \{ (?:return )?reinterpret_cast<(?P<rt>.+?) '
                  r'\((?P<cc>__\w+) \*\)\((?P<types>[^)]*)\)>\(XI_GAME\((?P<addr>0x[0-9A-Fa-f]+)u\)\)', re.M)
# a variadic wrapper: `template <class... R> XI_ORIG RET NAME(fixed..., R... rest) { ... (CC *)(...)>(XI_GAME(ADDR)) ...`
VORIG = re.compile(r'^template <class\.\.\. R> XI_ORIG (?P<ret>.+?) (?P<name>\w+)\((?P<params>[^)]*?),?\s*R\.\.\. rest\) \{ (?:return )?'
                   r'reinterpret_cast<(?P<rt>.+?) \((?P<cc>__\w+) \*\)\((?P<types>[^)]*?), (?:decltype[^>]*|\.\.\.\))>\(XI_GAME\((?P<addr>0x[0-9A-Fa-f]+)u\)\)', re.M)
GLOB = re.compile(r'^typedef (?P<type>.+?) (?P<name>\w+)_t(?P<arr>\[\d*\])?;\s*\n#define (?P=name) XI_EA\((?P=name)_t, (?P<addr>0x[0-9A-Fa-f]+)u\)', re.M)
# a global holding a function pointer (an import's IAT slot, a vtable's first slot): `typedef R (CC *NAME_t)(P);`
FGLOB = re.compile(r'^(?P<td>typedef (?P<rt>[^()\n]+?) \((?:(?P<cc>__\w+) )?\*(?P<name>\w+)_t\)\((?P<params>[^()]*)\);)\s*\n'
                   r'#define (?P=name) XI_EA\((?P=name)_t, (?P<addr>0x[0-9A-Fa-f]+)u\)', re.M)
BASIC = {'void', 'char', 'short', 'int', 'long', 'float', 'double', 'unsigned', 'signed', 'uint8_t', 'int8_t', 'uint16_t',
         'int16_t', 'uint32_t', 'int32_t', 'uint64_t', 'int64_t', 'undefined', 'undefined1', 'undefined2', 'undefined4',
         'undefined8', 'byte', 'uchar', 'ushort', 'word', 'uint', 'dword', 'ulong', 'size_t', 'BOOL', 'bool', 'code',
         'LPVOID', 'DWORD', '_DWORD', '_WORD', '_BYTE',
         # Visual C++ 6.0's own sized integers; the tree's GCC build defines them as the same types (xi_decomp.hpp:
         # __int8 char, __int16 short, __int32 int; __int64 is GCC's own)
         '__int8', '__int16', '__int32', '__int64'}


def plain(ty):
    """the type is made of shim types and pointers only"""
    return all(w in BASIC for w in re.findall(r'[A-Za-z_]\w*', ty.replace('const', '')))


# Windows types as tree/include/xi_compat_win.hpp declares them (from the SDK headers)
WINT = {'FARPROC': 'typedef int (__stdcall *FARPROC)();', 'HMODULE': 'typedef void *HMODULE;', 'CHAR': 'typedef char CHAR;',
        'UINT': 'typedef unsigned int UINT;', 'LPSTR': 'typedef char *LPSTR;', 'LPCSTR': 'typedef const char *LPCSTR;',
        'LPSYSTEMTIME': 'struct _SYSTEMTIME; typedef struct _SYSTEMTIME *LPSYSTEMTIME;',
        'LPRECT': 'struct tagRECT; typedef struct tagRECT *LPRECT;', 'LPMSG': 'struct tagMSG; typedef struct tagMSG *LPMSG;',
        'HINSTANCE': 'struct HINSTANCE__; typedef struct HINSTANCE__ *HINSTANCE;',
        'HCURSOR': 'struct HICON__; typedef struct HICON__ *HCURSOR;', 'LRESULT': 'typedef int LRESULT;',
        'HWND': 'typedef void *HWND;', 'HANDLE': 'typedef void *HANDLE;', 'xi_codefn': 'typedef code xi_codefn;',
        '_UNKNOWN': 'typedef unsigned char _UNKNOWN;', 'FILE': 'struct _iobuf; typedef struct _iobuf FILE;',
        # xi_compat_win.hpp (imm.h DECLARE_HANDLE(HIMC); mmsystem.h; winuser.h's MSG) and gfx_types.hpp (LONG)
        'HIMC': 'struct HIMC__; typedef struct HIMC__ *HIMC;', 'MMRESULT': 'typedef unsigned int MMRESULT;',
        'MSG': 'struct tagMSG; typedef struct tagMSG MSG;', 'LONG': 'typedef int LONG;'}


def type_decls(ty):
    """the declarations a global's type needs beyond the shim: a Windows typedef, a forward declaration for a pointer
    to a named type, a one-int placeholder for one held by value (its fields and size are not known: code reading
    them fails to compile, code using its size compiles to other bytes, neither can match by chance)"""
    out = []
    for w in re.findall(r'[A-Za-z_]\w*(?:::\w+)?', ty.replace('const', '')):
        if w in BASIC or w in ('__int16', '__int8', '__int32', '__int64', 'XI_ANY'): continue
        # a type in namespace std (xi_compat_win.hpp: `namespace std { struct codecvt_base; }`), through a pointer
        if w.startswith('std::') and re.search(r'%s\s*\*' % re.escape(w), ty): out.append('namespace std { struct %s; }' % w[5:]); continue
        if w in WINT: out.append(WINT[w]); continue
        # a struct also named by a typedef, as C needs (C++ takes it too)
        if re.search(r'\b%s\s*\*' % w, ty): out.append('struct %s; typedef struct %s %s;' % (w, w, w))
        # a Direct3D type held by value: its layout is the tree's header's (xi_d3d8.hpp, from the SDK's d3d8.h)
        elif w in d3d_names(): out.append(D3D_MARK)
        else: out.append('struct %s { int xi_unknown_layout; }; typedef struct %s %s;' % (w, w, w))
    return out


def externs():
    src = open(os.path.join(ROOT, 'tree', 'include', 'gfx_orig.hpp')).read()
    o = {m.group('name'): m.groupdict() for m in ORIG.finditer(src)}
    for m in VORIG.finditer(src): o[m.group('name')] = dict(m.groupdict(), variadic=True)
    gt = open(os.path.join(ROOT, 'tree', 'include', 'gfx_globals.hpp')).read()
    g = {m.group('name'): m.groupdict() for m in GLOB.finditer(gt)}
    for m in FGLOB.finditer(gt):
        # its parameter types (a parameter's name dropped), for the declarations they need
        ps = [re.sub(r'\s*\b\w+$', '', x.strip()) if re.search(r'[\w*]\s*\b\w+$', x.strip()) and not re.match(r'^[\w\s]*\b(?:%s)$' % '|'.join(BASIC), x.strip()) else x.strip()
              for x in m.group('params').split(',') if x.strip()]
        g[m.group('name')] = dict(m.groupdict(), type=m.group('name') + '_t', arr=None, fptr=' '.join([m.group('rt')] + ps))
    return o, g


def glob_lines(g, gd):
    """the lines that declare global g at its address: `#define g (*(type *)address)`, after what its type needs"""
    if gd.get('fptr'):
        return type_decls(gd['fptr']) + [gd['td'], '#define %s (*(%s *)%su)' % (g, gd['type'], gd['addr'])]
    d = type_decls(gd['type'])
    if gd.get('arr'): return d + ['#define %s (*(%s (*)%s)%su)' % (g, gd['type'], gd['arr'], gd['addr'])]
    return d + ['#define %s (*(%s *)%su)' % (g, gd['type'], gd['addr'])]


def split_args(s):
    out, depth, cur = [], 0, ''
    for ch in s:
        if ch in '([': depth += 1
        elif ch in ')]': depth -= 1
        if ch == ',' and depth == 0: out.append(cur.strip()); cur = ''; continue
        cur += ch
    if cur.strip(): out.append(cur.strip())
    return out


_D3DN = None
_OPAQUE = None
D3D_MARK = '/* xi_d3d8 */'     # a source needing a Direct3D type (a callee's, a global's): main() includes the header


def d3d_names():
    """the struct, enum and typedef names tree/include/xi_d3d8.hpp declares"""
    global _D3DN
    if _D3DN is None:
        h = open(os.path.join(ROOT, 'tree', 'include', 'xi_d3d8.hpp')).read()
        _D3DN = set(re.findall(r'(?m)^(?:struct|enum) (\w+)\b', h)) | set(re.findall(r'(?m)^typedef [^;]*?\b(\w+);', h))
    return _D3DN


def opaque_names():
    """the opaque types tree/include/xi_compat_win.hpp declares (`struct Iostream_init; ...`, the IDB's FLIRT names,
    used only through pointers) and `namespace std { struct codecvt_base; }`"""
    global _OPAQUE
    if _OPAQUE is None:
        h = open(os.path.join(ROOT, 'tree', 'include', 'xi_compat_win.hpp')).read()
        _OPAQUE = set(re.findall(r'\bstruct (\w+);', h))
        _OPAQUE |= {'std::%s' % n for ns in re.findall(r'namespace std \{([^}]*)\}', h) for n in re.findall(r'struct (\w+);', ns)}
    return _OPAQUE


def call_type_decls(ty):
    """what a callee's parameter or return type needs beyond the shim, as the tree declares it: ([declarations],
    needs C++), or None when it is a type the sweep has no declaration for. A Windows typedef (WINT); a Direct3D
    type (its header, D3D_MARK); a pointer to a struct the tree declares (tree/include) or to one of
    xi_compat_win.hpp's opaque types: a forward declaration, which a pointer needs (passed as its 4 bytes)"""
    out, cpp = [], False
    for w in re.findall(r'[A-Za-z_]\w*(?:::\w+)?', re.sub(r'\b(?:const|volatile)\b', '', ty)):
        if w in BASIC or w == 'XI_ANY': continue
        if w in WINT: out.append(WINT[w]); continue
        if w in d3d_names(): out.append(D3D_MARK); continue
        if not re.search(r'(?<![\w:])%s\s*\*' % re.escape(w), ty): return None       # by value: its layout is needed
        if w.startswith('std::') and w in opaque_names():
            out.append('namespace std { struct %s; }' % w[5:]); cpp = True
        elif w in struct_names() or w in opaque_names(): out.append('struct %s; typedef struct %s %s;' % (w, w, w))
        else: return None
    return out, cpp


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
        tds = [call_type_decls(x) for x in [w['rt']] + [x for x in types if x not in tpl]]
        if None in tds: return None, 'type the shim lacks'
        if c_mode and any(cpp for _, cpp in tds): return None, 'a type C cannot name'
        for td, _ in tds:
            for x in td: decls.setdefault(x, x)
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
                # a template cannot be variadic in C++98: the fixed XI_ANY parameters are the shim's 4-byte XI_ANY
                vt = [('XI_ANY' if x in tpl else x) for x in types]
                decls[sym] = '%s%s %s %s(%s);' % ('' if c_mode else 'extern "C" ', w['rt'], w['cc'], sym, ', '.join(vt + ['...']))
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


# the shim's inline functions (and HELPERS), by their decorated names
SHIM_HELPER = re.compile(r'^\?\?0XI_ANY@@|^\?xi_castbits_@@|^_?(?:builtin_strncpy|xi_k4|xi_k8)$|^\?(?:builtin_strncpy|xi_k4|xi_k8|xi_b32|xi_b64)@@')
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


# the decompilers' virtual call through a __thiscall function-pointer type: Ghidra's `((R (__thiscall *)(P))(FN))(A)`
# with FN the slot's value, Hex-Rays' `(*(R (__thiscall **)(P))(SLOT))(A)` with SLOT the slot's address
_VCALL = re.compile(r'\(\((?P<ret>[^()]+?) \(__thiscall \*\)\((?P<params>[^()]*)\)\)\(')
_VCALL2 = re.compile(r'\(\*\((?P<ret>[^()]+?) \(__thiscall \*\*\)\((?P<params>[^()]*)\)\)\(')
_VCALL3 = re.compile(r'\(\*\(\((?P<ret>[^()]+?) \(__thiscall \*\*\)\((?P<params>[^()]*)\)\)\(')
_INT4 = r'(?:int|uint|uint32_t|_DWORD|undefined4|dword|DWORD|unsigned int)'
VDECLS = []      # stand-in classes the virtual calls of the last macros() body need (convert puts them at file scope)


def _strip_parens(e):
    e = e.strip()
    while e.startswith('(') and _group(e, 0) == len(e): e = e[1:-1].strip()
    return e


def _strip_ptr_cast(e):
    """the expression without outer parentheses and outer pointer casts `(T *)`"""
    while True:
        e = _strip_parens(e)
        m = re.match(r'^\((?:[A-Za-z_]\w*\s*)+\*+\s*\)\s*(.*)$', e)
        if not m or _group(e, 0) != len(e) - len(m.group(1)): return e
        e = m.group(1)


def _pure(e):
    """no side effect: no call, assignment, ++ or --"""
    return not re.search(r'\b[A-Za-z_]\w*\s*\(|\+\+|--|[^=!<>]=[^=]', e)


def _vtable_slot(addr, decls):
    """for the address of a vtable slot read from object X -- `VT + OFF` or `VT`, VT the object's first 4 bytes as an
    integer: `*(T *)X` for a 4-byte integer T, or `*X` with X a name declared (a local, a parameter, a global) a
    pointer to one, or X itself `*(T **)(E)` -- (X, OFF), X free of side effects; else None"""
    addr = _strip_parens(addr)
    m = re.match(r'^(.*?)\s*\+\s*(0x[0-9a-fA-F]+|\d+)$', addr)
    vt, off = (m.group(1), int(m.group(2), 0)) if m else (addr, 0)
    vt = _strip_parens(vt)
    x = None
    m = re.match(r'^\*\(%s \*\)\s*(.+)$' % _INT4, vt)
    if m: x = _strip_parens(m.group(1))
    else:
        m = re.match(r'^\*(\w+)$', vt)
        if m and re.search(r'(?:^|[;{(,\n])\s*%s\s*\*\s*%s\s*[;,)]' % (_INT4, re.escape(m.group(1))), decls): x = m.group(1)
        m = re.match(r'^\*(\*\(%s \*\*\)\s*\(.*\))$' % _INT4, vt)
        if m and _group(m.group(1), m.group(1).index('(', m.group(1).index('**)'))) == len(m.group(1)): x = m.group(1)
        m = re.match(r'^\*(\*\(%s \*\*\)\s*\w+)$' % _INT4, vt)                 # **(T **)v: the vtable of *(T **)v
        if m: x = m.group(1)
        m = re.match(r'^\*\(void \* \*\)\(\(char \*\)(\w+) \+ 0x0\)$', vt)   # *(void * *)((char *)X + 0x0)
        if m: x = m.group(1)
    return (x, off) if x and _pure(x) else None


def _same_obj(a, b):
    """the same object: equal once outer pointer casts are dropped, `*(T **)E` read as `*(T *)E` for a 4-byte T"""
    f = lambda e: re.sub(r'\*\(%s \*\*\)' % _INT4, lambda m: m.group(0)[:-2] + ')', _strip_ptr_cast(e))
    return f(a) == f(b)


def _vcalls(body, decls=''):
    """a virtual call the decompiler wrote through a __thiscall function-pointer type -- which Visual C++ 6.0 cannot
    spell -- as the C++ call it compiles from: a stand-in class with OFF/4 virtual slots before the method,
    `((XVk *)(X))->f(A...)`. Only where the slot is read from the vtable of the object X passed as `this` (the first
    argument), X free of side effects (the decompiled form reads it twice, the call once), the slot's offset a
    multiple of 4: Ghidra's
    `((R (__thiscall *)(void *, P...))((**(code **)(*X + OFF))))((void *)(X), A...)` and Hex-Rays'
    `(*(R (__thiscall **)(T, P...))(*(_DWORD *)X + OFF))(X, A...)`. Anything else is left as it is"""
    out, i = [], 0
    while True:
        ms = [x for x in (_VCALL.search(body, i), _VCALL2.search(body, i), _VCALL3.search(body, i)) if x]
        if not ms: out.append(body[i:]); return ''.join(out)
        m = min(ms, key=lambda x: x.start())
        p = m.start(); e1 = _group(body, p)
        if e1 < 0 or e1 >= len(body) or body[e1] != '(':
            out.append(body[i:m.end()]); i = m.end(); continue
        slot = None
        if m.re is _VCALL3:
            v1 = _group(body, m.end() - 1)                                  # the (VT) group
            mk = re.match(r'\s*\+\s*(\d+)\)\)\(', body[v1:]) if v1 > 0 else None
            if not mk:
                out.append(body[i:m.end()]); i = m.end(); continue
            e1 = v1 + mk.end() - 1
            vs = _vtable_slot(body[m.end() - 1:v1], decls + '\n' + body)
            slot = (vs[0], vs[1] + 4 * int(mk.group(1))) if vs and vs[1] == 0 else None
        elif m.re is _VCALL:
            fn = _strip_parens(body[m.end() - 1:e1 - 1])
            fm = re.match(r'^\*\*?\(code \*\*\)(.*)$', fn)
            if fm: slot = _vtable_slot(fm.group(1), decls + '\n' + body)
        else:
            slot = _vtable_slot(body[m.end() - 1:e1 - 1], decls + '\n' + body)
        e2 = _group(body, e1)
        args = split_args(body[e1 + 1:e2 - 1]) if e2 > 0 else []
        params = [x.strip() for x in m.group('params').split(',')]
        ok = slot is not None and args and params and len(args) == len(params) and slot[1] % 4 == 0 \
            and re.match(r'^(?:%s|[\w ]+\*+)$' % _INT4, params[0]) and _same_obj(args[0], slot[0])
        if not ok:
            out.append(body[i:m.end()]); i = m.end(); continue
        obj, off = slot
        k = off // 4; sig = '%s f(%s)' % (m.group('ret'), ', '.join(params[1:]) or 'void')
        name = 'XV%d_%s' % (k, hashlib.sha1(sig.encode()).hexdigest()[:6])
        VDECLS.append('struct %s { %s virtual %s; };' % (name, ' '.join('virtual void s%d();' % n for n in range(k)), sig))
        out.append(body[i:p] + '((%s *)(%s))->f(%s)' % (name, obj, ', '.join(args[1:])))
        i = e2


HEXF = {}         # decimal written for a hexadecimal floating literal: its value


def hexf_check(d):
    """{decimal: True when Visual C++ 6.0 reads it as the double HEXF gives}: a C source of `double xi_hf[] = {...}`
    compiled, its .data section read back"""
    import struct
    ks = sorted(HEXF)
    if not ks: return {}
    open(os.path.join(d, 'xi_hexf.c'), 'w').write('double xi_hf[%d] = { %s };\n' % (len(ks), ', '.join(ks)))
    rc, _ = M.cl('xi_hexf.c', 'xi_hexf.obj', ['/O2'], d)
    got = b''
    if rc == 0:
        b = open(os.path.join(d, 'xi_hexf.obj'), 'rb').read()
        _, nsec, _, _, _, opt, _ = struct.unpack_from('<HHIIIHH', b, 0)
        for i in range(nsec):
            nm, _, _, size, raw = struct.unpack_from('<8sIIII', b, 20 + opt + 40 * i)
            if nm.rstrip(b'\0') == b'.data': got = b[raw:raw + size]
    return {k: got[8 * i:8 * i + 8] == struct.pack('<d', HEXF[k]) for i, k in enumerate(ks)}


def macros(body, decls=''):
    """the tree's XI_ helpers the shim cannot define as they are, rewritten; (body, None) or (None, why). decls: the
    function's parameters and the globals it names, declared, for the types _vcalls needs"""
    body = re.sub(r'\s*__attribute__\(\(aligned\(\d+\)\)\)', '', body)
    body = re.sub(r'\balignas\(\d+\)\s*', '', body)          # the frame's alignment: the frame layout is the game's
    body = _at_calls(body)
    if body is None: return None, 'at<> not parsed'
    body = re.sub(r'XI_PIECE\(([^(),]+), (\d+), 3\)', r'XI_PIECE3(\1, \2)', body)
    # a hexadecimal floating literal (C++17; Visual C++ 6.0 has none): the decimal that reads back as the same double
    # (Python's repr), checked against the compiler itself (hexf_check) before any source using it is compiled
    def hexf(m):
        v = float.fromhex(m.group(0)); t = repr(v)
        HEXF[t] = v; return t
    body = re.sub(r'\b0[xX][0-9A-Fa-f]*\.[0-9A-Fa-f]*[pP][-+]?\d+\b(?![fFlL])', hexf, body)
    # a 64-bit literal's suffix: C++'s `ull` / `ll` is Visual C++ 6.0's `ui64` / `i64` (the same types)
    body = re.sub(r'\b(0[xX][0-9A-Fa-f]+|\d+)(?:ull|ULL|llu|LLU)\b', r'\1ui64', body)
    body = re.sub(r'\b(0[xX][0-9A-Fa-f]+|\d+)(?:ll|LL)\b', r'\1i64', body)
    body = _castbits(body)
    if body is None: return None, 'xi_castbits not parsed'
    del VDECLS[:]
    body = _vcalls(body, decls)
    if re.search(r'XI_CALL_ST0\((?!0x10311C2Cu\))', body): return None, 'XI_CALL_ST0 of another function'
    body = body.replace('XI_CALL_ST0(0x10311C2Cu)', 'XI_FTOL')
    def code(m):
        a = code_addrs().get(m.group(1))
        return '((void *)0x%08Xu)' % a if a is not None else m.group(0)
    body = re.sub(r'XI_CODE\((\w+)\)', code, body)
    if 'XI_CODE(' in body: return None, 'XI_CODE of an unknown name'
    return body, None


# helpers of the tree (tree/include/xi_ghidra.hpp) the shim defines only in a source that uses them: (definition,
# needs C++). SUB41 and the like take a float's encoding (an overload), which C cannot write
HELPERS = {
    'builtin_strncpy': ('static __inline char *builtin_strncpy(void *d, const char *s, unsigned n) {\n'
                        '    char *o = (char *)d; unsigned i = 0;\n    for (; i < n && s[i]; i++) o[i] = s[i];\n'
                        '    for (; i < n; i++) o[i] = 0;\n    return (char *)d;\n}', False),
    'xi_k4': ('static __inline float xi_k4(unsigned __int64 b) { uint32_t u = (uint32_t)b; float f; memcpy(&f, &u, 4); return f; }', False),
    'xi_k8': ('static __inline double xi_k8(unsigned __int64 b) { double d; memcpy(&d, &b, 8); return d; }', False),
    'xi_b32': ('template <class T> static inline uint32_t xi_b32(T x) { return (uint32_t)x; }\n'
               'static inline uint32_t xi_b32(float x) { uint32_t u; memcpy(&u, &x, 4); return u; }\n'
               'template <class T> static inline uint64_t xi_b64(T x) { return (uint64_t)x; }\n'
               'static inline uint64_t xi_b64(double x) { uint64_t u; memcpy(&u, &x, 8); return u; }\n'
               '#define SUB41(x, b) ((uint8_t)(xi_b32(x) >> (8 * (b))))\n#define SUB42(x, b) ((uint16_t)(xi_b32(x) >> (8 * (b))))\n'
               '#define SUB81(x, b) ((uint8_t)(xi_b64(x) >> (8 * (b))))\n#define SUB84(x, b) ((uint32_t)(xi_b64(x) >> (8 * (b))))\n'
               '#define SUB82(x, b) ((uint16_t)(xi_b64(x) >> (8 * (b))))', True),
    # the CRT's math functions (tree/include/xi_decomp.hpp: the x87 instructions /Oi gives them) from the compiler's own
    # <math.h>; ABS as tree/include/xi_ghidra.hpp defines it
    'math': ('#include <math.h>\n#define fpatan(y, x) atan2((y), (x))   /* x87 fpatan: atan(ST1 / ST0), what /Oi gives atan2 */', False),
    'ABS': ('#define ABS(x) ((x) < 0 ? -(x) : (x))', False),
}
HELPER_USE = {'builtin_strncpy': r'\bbuiltin_strncpy\b', 'xi_k4': r'\bxi_k4\b', 'xi_k8': r'\bxi_k8\b',
              'xi_b32': r'\b(?:SUB4[12]|SUB8[124]|xi_b32|xi_b64)\b',
              'math': r'\b(?:sin|cos|tan|atan|atan2|sqrt|fabs|floor|ceil|fmod|pow|exp|log|fpatan)\s*\(', 'ABS': r'\bABS\s*\('}
# Windows types a body names (tree/include/xi_compat_win.hpp and gfx_types.hpp, from the SDK headers)
BODYT = dict(WINT, LPDWORD='typedef unsigned int *LPDWORD;', PVOID='typedef void *PVOID;',
             LPCRITICAL_SECTION='struct _RTL_CRITICAL_SECTION; typedef struct _RTL_CRITICAL_SECTION *LPCRITICAL_SECTION;')
_STRUCTS = None


def struct_names():
    """the struct and class names tree/include declares (gfx_types.hpp's IDB types, the classes)"""
    global _STRUCTS
    if _STRUCTS is None:
        _STRUCTS = set()
        for f in glob.glob(os.path.join(ROOT, 'tree', 'include', '*.hpp')):
            _STRUCTS |= set(re.findall(r'(?m)^(?:struct|class(?: XI_API)?) (?:/\*\w*\*/ )?(\w+)\b', open(f).read()))
    return _STRUCTS


_D3DM = None


def d3d_methods():
    """the names of the inline wrappers tree/include/xi_d3d8.hpp gives its interfaces (`dev->SetRenderState(...)`)"""
    global _D3DM
    if _D3DM is None:
        _D3DM = set(re.findall(r'(?m)^\s+[\w\s*]+?\b(\w+)\([^)]*\) \{ return lpVtbl->', open(os.path.join(ROOT, 'tree', 'include', 'xi_d3d8.hpp')).read()))
    return _D3DM


def convert(m, src, orig=None, glob_=None):
    """(file suffix, source) or (None, why)"""
    ret, cc, name, args = m.group('ret'), m.group('cc'), m.group('name'), m.group('args').strip()
    body = src[src.index('{'):]
    gdecls = ''.join('%s %s;\n' % (glob_[g]['type'], g) for g in sorted(set(re.findall(r'\b\w+\b', body)) & set(glob_ or ()))
                     if not glob_[g].get('arr') and not glob_[g].get('fptr'))
    body, why = macros(body, '(' + args + ')\n' + gdecls)
    if body is None: return None, why
    force_cpp = bool(re.search(r'reinterpret_cast|static_cast|&\s*\w+\s*=\s*\*', body)) or bool(VDECLS)   # C++ references and casts, virtual calls
    # a Direct3D interface's inline wrapper called (`dev->SetRenderState(...)`): C++, as the tree
    force_cpp = force_cpp or any(x in d3d_methods() for x in re.findall(r'->\s*(\w+)\s*\(', body))
    pre = list(dict.fromkeys(VDECLS))
    words = set(re.findall(r'\b[A-Za-z_]\w*\b', body))
    for h, (code, cpp) in HELPERS.items():
        if re.search(HELPER_USE[h], body): pre.append(code); force_cpp = force_cpp or cpp
    # the types a body names: a Windows typedef; a struct or class of tree/include used through a pointer, declared
    # (its layout is not needed for a pointer: code reading its fields still fails to compile); a global's type by
    # its typedef's name (gfx_globals.hpp `NAME_t`)
    for w in sorted(words):
        if w in BODYT: pre.append(BODYT[w])
        elif w in struct_names() and w not in BASIC and re.search(r'\b%s\s*\*' % w, body): pre.append('struct %s; typedef struct %s %s;' % (w, w, w))
        elif w.endswith('_t') and glob_ and w[:-2] in glob_:
            gd = glob_[w[:-2]]
            pre += type_decls(gd.get('fptr') or gd['type'])
            pre.append(gd['td'] if gd.get('fptr') else 'typedef %s %s%s;' % (gd['type'], w, gd.get('arr') or ''))
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
            head = '#include "vc6_shim.h"\n' + ''.join(x + '\n' for x in dict.fromkeys(pre))
            # each global at its address, after what its type needs (glob_lines: as for the other functions)
            gl = []
            for g in sorted(set(re.findall(r'\b\w+\b', body)) & set(glob_ or ())):
                gd = glob_[g]
                if gd.get('arr'): body = re.sub(r'\bXI_PIECE(_S)?\(%s,' % g, r'XI_PIECEA\1(%s,' % g, body)
                gl += [x for x in glob_lines(g, gd) if x not in gl and x not in pre]
            head += ''.join(x + '\n' for x in gl)
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
            if gd.get('arr'): body = re.sub(r'\bXI_PIECE(_S)?\(%s,' % g, r'XI_PIECEA\1(%s,' % g, body)
            pre += [x for x in glob_lines(g, gd) if x not in pre]
        if 'orig::' in body:
            nb, d = rewrite_calls(body, orig, c_mode=cc != '__thiscall' and not force_cpp)
            cc_cpp = nb is None or cc == '__thiscall' or force_cpp
            if nb is None: nb, d = rewrite_calls(body, orig)
            if nb is None: return None, d
            body = nb; pre += d
        else: cc_cpp = False
    else: cc_cpp = False
    head = '#include "vc6_shim.h"\n' + ''.join(x + '\n' for x in dict.fromkeys(pre))
    if cc in ('__cdecl', '__stdcall', '__fastcall'):
        if cc_cpp or force_cpp: return '.cpp', head + 'extern "C" %s %s %s(%s)\n%s' % (ret, cc, name, args or 'void', body)
        return '.c', head + '%s %s %s(%s)\n%s' % (ret, cc, name, args or 'void', body)
    a = [x.strip() for x in args.split(',')] if args else []
    # the first parameter is the one in ECX, whatever the tree names it (self_, or a1 where the decompiler named it so)
    sm = a and re.match(r'^(?P<t>[\w\s]*?\w[\w\s]*?\**)\s*\b(?P<n>\w+)$', a[0])
    if not sm: return None, 'thiscall without a first parameter'
    st, pn = sm.group('t').strip(), sm.group('n')
    if pn != 'self_' and (re.search(r'\bself_\b', body) or re.match(r'^void\s*\*$', st)): return None, 'thiscall first parameter not self_'
    rest = ', '.join(a[1:]) or 'void'
    # a method whose body binds its own view of this (`T * self = reinterpret_cast<T *>(this);`) keeps that one.
    # Otherwise self_, the parameter the tree passes in ECX, is `this`: `void *self_ = this;`, or for a first
    # parameter of another 4-byte type T (a pointer or an int: ECX's value as that type) `T self_ = (T)this;`, the
    # same register's value
    if pn == 'self_' and re.search(r'(?m)^\s*[\w\s*]+?\bself_ = reinterpret_cast<[^>]+>\(this\);', body): bind = ''
    elif re.match(r'^void\s*\*$', st): bind = '  void *self_ = this;\n'
    else:
        if call_type_decls(st) is None or not (st.endswith('*') or re.match(r'^(?:%s)$' % _INT4, st)):
            return None, 'thiscall self_ of a type the shim lacks'
        bind = '  %s %s = (%s)this;\n' % (st, pn, st)
    # the signature's types, declared as the tree declares them (call_type_decls), where the sweep has a declaration
    sd = []
    for x in [ret, st] + a[1:]:
        pt = re.sub(r'\s*\b\w+$', '', x) if x not in (ret, st) and re.search(r'[\w*]\s*\b\w+$', x) else x
        ptd = call_type_decls(pt)
        if ptd is not None and not ptd[1]: sd += [y for y in ptd[0] if y not in sd]
    head += ''.join(x + '\n' for x in sd if x not in head.split('\n'))
    return '.cpp', (head + 'struct XS { %s %s(%s); };\n%s XS::%s(%s)\n{\n%s%s\n}\n'
                    % (ret, name, rest, ret, name, rest, bind, body[1:].rstrip().rstrip('}')))


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


_GLOBDEF = re.compile(r'(?m)^#define (\w+) \(\*\((.+?) (\(\*\)(\[[^\]]*\](?:\[[^\]]*\])*)\)|\*\))(0x[0-9A-Fa-f]{8})u\)$')


def _rule_globvar(text, ret, name):
    """a global read through its cast address (glob_lines) declared as a variable instead: `(*(int *)0x1047BFA8)`
    makes the compiler reload it where the declared `extern int` is loaded once (recompile/d3d_round_118.md rule 1).
    The variable's name carries the address (match.py's ADDR_RE), so its relocations are still compared"""
    def decl(m):
        g, ty, arr, ea = m.group(1), m.group(2), m.group(4) or '', m.group(5)
        v = 'xi_g_%08X' % int(ea, 16)
        return 'extern %s %s%s;\n#define %s %s' % (ty, v, arr, g, v)
    t = _GLOBDEF.sub(decl, text)
    return t if t != text else None


_FRAME_DECL = re.compile(r'(?m)^[ \t]*(?:alignas\(\d+\) )?(?:unsigned )?char xi_frame\[\w+\][^;\n]*;[^\n]*\n')
_FRAME_REF = re.compile(r'(?m)^([ \t]*)([A-Za-z_][\w \t]*?(?:\s*\*+)?)\s*(?:&(\w+)|\(&(\w+)\)\[(\w+)\]) = '
                        r'\*reinterpret_cast<[^>]*>\(xi_frame \+ (\w+)\);[ \t]*\n')
_SIZES = {'char': 1, 'unsigned char': 1, 'uint8_t': 1, 'int8_t': 1, 'bool': 1, 'short': 2, 'uint16_t': 2, 'int16_t': 2,
          'int': 4, 'unsigned int': 4, 'uint32_t': 4, 'int32_t': 4, 'float': 4, '_DWORD': 4, 'undefined4': 4,
          'uint64_t': 8, 'int64_t': 8, 'double': 8}


def _rule_frame(text, ret, name):
    """the decompiler's frame block (`char xi_frame[N]` with each local a reference into it at its offset) as plain
    locals, each declared where its reference was (recompile/d3d_round_119.md rule 12: the block gives a frame of its
    size, the plain locals the compiler's own layout). Only where nothing else names the block and no two locals
    overlap"""
    if len(_FRAME_DECL.findall(text)) != 1: return None
    spans = []
    def plain(m):
        ind, ty, ref, aref, n, off = m.groups()
        ty = ty.strip()
        sz = 4 if ty.endswith('*') else _SIZES.get(re.sub(r'\s+', ' ', ty))
        if sz is None: raise ValueError(ty)
        try: k = int(n, 0) if aref else 1; o = int(off, 0)
        except ValueError: raise ValueError(n)
        spans.append((o, o + sz * k))
        return '%s%s %s%s;\n' % (ind, ty, ref or aref, '[%s]' % n if aref else '')
    try: t = _FRAME_REF.sub(plain, _FRAME_DECL.sub('', text))
    except ValueError: return None
    if 'xi_frame' in t or not spans: return None
    spans.sort()
    if any(a[1] > b[0] for a, b in zip(spans, spans[1:])): return None
    return t


def _rule_float(text, ret, name):
    """float arithmetic in float (recompile/d3d_round_119.md rule 11): the decompiler's double locals, casts and
    constants give qword FPU operands where the game has dword ones. In the function's body only (its signature and
    the callees' declarations are the calling convention): [`double` as `float` and an unsuffixed literal with an
    `f`; the literals alone]"""
    m = re.search(r'\b%s\s*\([^;{]*\)\s*\{' % re.escape(name), text)
    if not m: return None
    head, body = text[:m.end()], text[m.end():]
    lit = lambda b: re.sub(r'(?<![\w.])(\d+\.\d*(?:[eE][-+]?\d+)?|\d+[eE][-+]?\d+)(?![\w.])', r'\1f', b)
    out = []
    if re.search(r'\bdouble\b', body): out.append(head + lit(re.sub(r'\bdouble\b', 'float', body)))
    b2 = lit(body)
    if b2 != body: out.append(head + b2)
    return out or None


_KW = {'sizeof', 'if', 'while', 'for', 'switch', 'return'}
_TYPEW = {'void', 'int', 'char', 'short', 'long', 'float', 'double', 'unsigned', 'signed', 'const', '_DWORD', '_BYTE',
          '_WORD', 'undefined4', 'bool', '__stdcall', '__thiscall', '__cdecl', '__fastcall'}   # `void (__stdcall **)(...)`


def _rule_devlocal(text, ret, name):
    """a call statement through an object a 4-byte integer global points to (`(*(...)(**(_DWORD **)(G + 3772) + S))
    (*(_DWORD *)(G + 3772), ...)`) with the global read once into a block-scoped local: the game reads the global
    afresh for each device call into the register that then holds the device (recompile/d3d_round_119.md rule 8).
    Only a statement with no other call and no assignment, so nothing in it can change the global between the reads
    it replaces"""
    ints = {m.group(1) for m in re.finditer(r'(?m)^#define (\w+) \(\*\((?:int|_DWORD|unsigned int|undefined4) \*\)0x[0-9A-Fa-f]{8}u\)$', text)}
    for m in re.finditer(r'(?m)^extern (?:int|_DWORD|unsigned int|undefined4) (xi_g_[0-9A-F]{8});\n#define (\w+) \1$', text):
        ints.add(m.group(2))
    if not ints: return None
    out, i, n = [], 0, 0
    for m in re.finditer(r'(?m)^([ \t]*)(\(\*\()', text):
        if m.start() < i: continue
        depth, j = 0, m.start(2)
        while j < len(text):
            c = text[j]
            if c == '(': depth += 1
            elif c == ')': depth -= 1
            elif c in ';{}' and depth == 0: break
            j += 1
        if j >= len(text) or text[j] != ';': continue
        st = text[m.start(2):j]
        gs = [g for g in ints if len(re.findall(r'\b%s\b' % g, st)) >= 2]
        if len(gs) != 1: continue
        k = _group(st, 0)                                    # the callee `(*(...)(...))`, then its arguments
        args = st[k:]
        if not args.startswith('(') or _group(args, 0) != len(args): continue
        if ')(' in re.sub(r'\((?:const |unsigned |signed )*[A-Za-z_]\w*(?: \*+)?\)', 'C', args) or any(w not in _KW and w not in _TYPEW for w in re.findall(r'\b([A-Za-z_]\w*)\s*\(', st)): continue   # another call
        if re.search(r'(?<![=!<>])=(?!=)|\+\+|--', st): continue                             # an assignment
        g = gs[0]
        out.append(text[i:m.start(2)])
        out.append('{ int xi_l = %s; %s; }' % (g, re.sub(r'\b%s\b' % g, 'xi_l', st)))
        i = j + 1; n += 1
    if not n: return None
    return ''.join(out) + text[i:]


# the rewrite rules (--rules): each a mechanical change of how the code is written, never of what it computes; a
# result counts only where match.py's comparison says IDENTICAL
RULES = [
    ('ret8', lambda t, r, n: _rule_ret_piece(t, r, n, 8, 'unsigned char')),
    ('ret16', lambda t, r, n: _rule_ret_piece(t, r, n, 16, 'unsigned short')),
    ('ge0', _rule_ge0),
    ('cmpk', _rule_cmpk),
    ('retthis', _rule_retthis),
    ('globvar', _rule_globvar),
    ('devlocal', _rule_devlocal),
    ('frame', _rule_frame),
    ('float', _rule_float),
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


_CONV = re.compile(r"^C2440: '(?P<op>=|return|initializing)' : cannot convert from '(?P<a>[^']+)' to '(?P<b>[^']+)'$")
_CONVP = re.compile(r"^C2664: '(?P<fn>[^']+)' : cannot convert parameter (?P<n>\d+) from '(?P<a>[^']+)' to '(?P<b>[^']+)'$")
# a call through a function-pointer cast: `(*(R (CC **)(P))(SLOT))(` or `((R (CC *)(P))(FN))(`
_FPCALL = re.compile(r'\(\*?\((?P<ret>[^()]+?) \((?:__\w+ )?\*\*?\)\((?P<params>[^()]*)\)\)')


def _conv_kind(t):
    """'ptr' for a data or function pointer type as cl names it, 'int' for a 4-byte integer, 'enum' for an enum,
    else None"""
    t = re.sub(r'\bconst\b|\bvolatile\b', '', t).strip()
    if t.endswith('*') or re.search(r'\(__\w+ \*+\s*\)\(', t): return 'ptr'
    if re.match(r'^enum \w+$', t): return 'enum'
    if t in ('int', 'unsigned int', 'long', 'unsigned long'): return 'int'
    return None


def permissive(text, errors, fn):
    """the tree is compiled by GCC with -fpermissive (tree/build.sh), which takes an implicit conversion between two
    pointer types, or between a pointer and an integer, as the conversion a cast would make; Visual C++ 6.0's C++
    refuses it (C2440). Each such conversion cl names -- at an assignment, an initialisation or a return, whole on
    one line, one per line -- is written as that cast to the type cl names, `x = (T)(e);`, which is the same value on
    this flat 32-bit target. The same for an argument passed to a parameter (C2664) of a function cl names, called
    once on the line: `f(a, (T)(b))`. (new text, number of casts)"""
    lines, done = text.split('\n'), 0
    at = collections.defaultdict(set)
    for where, ln, msg in errors:
        if where != fn: continue
        m = _CONV.match(msg) or _CONVP.match(msg)
        # a pointer to or from a pointer or an integer; an integer to an enum (C++ has no implicit one, GCC's
        # -fpermissive takes it: the enum holds the integer's value)
        k = m and (_conv_kind(m.group('a')), _conv_kind(m.group('b')))
        ok = k and None not in k and ('ptr' in k and 'enum' not in k or k == ('int', 'enum'))
        if not ok: at[ln].add(None)
        elif m.re is _CONV: at[ln].add((m.group('op'), m.group('b').strip()))
        else: at[ln].add(('arg %s %s' % (m.group('n'), m.group('fn')), m.group('b').strip()))
    for ln, what in at.items():
        if len(what) != 1 or None in what or not 0 < ln <= len(lines): continue
        (op, ty), = what
        l = lines[ln - 1]
        if op.startswith('arg '):
            _, n, callee = op.split(' ', 2)
            if not re.match(r'^\w+$', callee):           # a signature `R CC NAME(P...)`, or a type `R (P...)`
                sm = re.match(r'^.*?\b(\w+)\(.*\)$', callee)
                callee = sm.group(1) if sm else None
            if callee:
                ks = [k for k in re.finditer(r'(?<![\w.])%s\s*\(' % re.escape(callee), l)]
                if len(ks) != 1: continue
                k = ks[0].end() - 1
            else:                                         # the one call on the line through a function-pointer cast
                ks = [x for x in _FPCALL.finditer(l)]
                if len(ks) != 1: continue
                k = _group(l, ks[0].start())
                if k < 0 or k >= len(l) or l[k] != '(': continue
            e = _group(l, k)
            if e < 0: continue
            args = split_args(l[k + 1:e - 1]); n = int(n)
            if not 0 < n <= len(args): continue
            args[n - 1] = '(%s)(%s)' % (ty, args[n - 1])
            lines[ln - 1] = l[:k + 1] + ', '.join(args) + l[e - 1:]
            done += 1; continue
        if op == 'return':
            ks = [k.start() for k in re.finditer(r'\breturn\b', l)]
            if len(ks) != 1: continue
            k = ks[0] + len('return')
        else:
            # the one assignment at the line's top level (not inside parentheses, not ==, <=, >=, !=, op=)
            ks, depth = [], 0
            for j, ch in enumerate(l):
                depth += {'(': 1, ')': -1, '[': 1, ']': -1}.get(ch, 0)
                if ch == '=' and depth == 0 and l[j + 1:j + 2] != '=' and l[j - 1:j] not in ('=', '<', '>', '!', '+', '-', '*', '/', '%', '&', '|', '^'):
                    ks.append(j)
            if len(ks) != 1: continue
            k = ks[0] + 1
        # the expression: to the `;` at its own level
        depth, e = 0, None
        for j in range(k, len(l)):
            depth += {'(': 1, ')': -1, '[': 1, ']': -1}.get(l[j], 0)
            if depth < 0: break
            if l[j] == ';' and depth == 0: e = j; break
        if e is None or not l[k:e].strip(): continue
        lines[ln - 1] = l[:k] + ' (%s)(%s)' % (ty, l[k:e].strip()) + l[e:]
        done += 1
    return '\n'.join(lines), done


def d3d_c(h):
    """the tree's Direct3D 8 header (xi_d3d8.hpp, its static_asserts already left out) for a C source: each struct
    and enum also named by a typedef, as C needs; the interfaces' inline C++ wrappers and the ++/-- operators on the
    enums (C has those on an enum already) left out; its two `static const` formats as macros of the same value.
    The structs, their fields and the vtables' slots are the header's."""
    out = ['/* xi_d3d8_vc6c.h: tree/include/xi_d3d8.hpp for C (pipeline/vc6/sweep.py d3d_c) */']
    names = re.findall(r'(?m)^struct (\w+)\s*[{;]', h)
    out += ['typedef struct %s %s;' % (n, n) for n in dict.fromkeys(names)]
    skip, enum = False, None
    for l in h.splitlines():
        if l.startswith('// Decompiled loops step D3D enums'): skip = True
        if skip:
            if l.startswith('#undef XI_ENUM_STEP'): skip = False
            continue
        if re.match(r'^\s+[^/].*\)\s*\{.*\}\s*$', l): continue                    # an inline member wrapper
        m = re.match(r'^static const (\w+) (\w+) = (.*);$', l)
        if m: out.append('#define %s (%s)' % (m.group(2), m.group(3))); continue
        out.append(l)
        m = re.match(r'^enum (\w+) \{', l)
        if m: enum = m.group(1)
        if l.startswith('};') and enum: out.append('typedef enum %s %s;' % (enum, enum)); enum = None
    return '\n'.join(out) + '\n'


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
    open(os.path.join(d, 'xi_d3d8_vc6c.h'), 'w').write(d3d_c(d3d))
    res, files, sig = {}, [], {}
    orig, glob_ = externs() if o.calls else (None, None)
    conv = []
    for ea, m, src in candidates(only, o.calls):
        conv.append((ea, m, convert(m, src, orig, glob_)))
    hx = hexf_check(d)
    if hx: print('hexadecimal floating literals: %d, %d read back as written' % (len(hx), sum(hx.values())))
    for ea, m, c in conv:
        k = '0x%08X' % ea
        bad = c[0] and [x for x in hx if not hx[x] and re.search(r'(?<![\w.])%s(?![\w.])' % re.escape(x), c[1])]
        if bad: c = (None, 'hexadecimal floating literal read otherwise (%s)' % bad[0])
        if c[0] is None: res[k] = {'verdict': 'LEFT_OUT', 'why': c[1], 'conv': m.group('cc')}; continue
        text = c[1]
        if re.search(r'\bIDirect3D\w*8\b', text) or D3D_MARK in text:
            text = text.replace('#include "vc6_shim.h"\n', '#include "vc6_shim.h"\n#include "xi_d3d8_vc6%s.h"\n' % ('c' if c[0] == '.c' else ''), 1)
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

    d3dh = open(os.path.join(d, 'xi_d3d8_vc6.h'), 'rb').read(); d3dhc = open(os.path.join(d, 'xi_d3d8_vc6c.h'), 'rb').read()
    errs = {}                  # file: [(where, line, cl's message)], its compile's errors
    def key_of(fn):
        src = open(os.path.join(d, fn), 'rb').read()
        return cache_key(src + (d3dh if b'xi_d3d8_vc6.h' in src else b'') + (d3dhc if b'xi_d3d8_vc6c.h' in src else b''), SHIM, o.flags, tc)

    def compile_chunk(chunk):
        r = subprocess.run(['wine', os.path.join(os.environ['VC6'], 'BIN', 'cl.exe'), '/nologo', '/c', '/Gy'] + o.flags.split() + [f for _, f, _ in chunk],
                           cwd=d, env=env, capture_output=True, text=True, errors='replace')
        # each file's errors, for the NO_COMPILE classes and the casts (permissive below): cl prints each file's name
        # before its messages, and a message in a header the file includes names the header
        cur = None
        for l in (r.stdout + r.stderr).splitlines():
            l = l.strip()
            if re.match(r'^f_[0-9A-F]{8}\w*\.c(?:pp)?$', l): cur = l; continue
            em = re.match(r'^(\S+?)\((\d+)\) : (?:fatal )?error (C\d+: .*)$', l)
            if em and cur: errs.setdefault(cur, []).append((em.group(1), int(em.group(2)), em.group(3)))
        if cache:
            for _, fn, _ in chunk:
                obj, c = os.path.join(d, fn.rsplit('.', 1)[0] + '.obj'), os.path.join(cache, key_of(fn))
                if os.path.exists(obj): shutil.copyfile(obj, c + '.tmp'); os.replace(c + '.tmp', c + '.obj')
                else:       # the file's own name as '*' (another file of the same source shares the entry)
                    json.dump([('*' if w == fn else w, n, e) for w, n, e in errs.get(fn, [])], open(c + '.fail', 'w'))

    def judge(files, what):
        """compiles (or takes from the cache) every (ea, file, cc), and gives each its verdict"""
        todo, hits = [], 0
        for ea, fn, cc in files:
            obj = os.path.join(d, fn.rsplit('.', 1)[0] + '.obj')
            if os.path.exists(obj): os.remove(obj)
            errs.pop(fn, None)
            c = cache and os.path.join(cache, key_of(fn))
            if c and os.path.exists(c + '.obj'): shutil.copyfile(c + '.obj', obj); hits += 1
            elif c and os.path.exists(c + '.fail'):
                hits += 1
                try: errs[fn] = [(fn if w == '*' else w, n, e) for w, n, e in json.load(open(c + '.fail'))]
                except ValueError: errs[fn] = []
            else: todo.append((ea, fn, cc))
        t0 = time.time()
        with CF.ThreadPoolExecutor(max_workers=o.jobs) as ex:
            list(ex.map(compile_chunk, [todo[k:k + 40] for k in range(0, len(todo), 40)]))
        print('%s: compiled %d sources in %.0f s (%d jobs); %d from the cache' % (what, len(todo), time.time() - t0, o.jobs, hits), flush=True)
        out = {}
        for ea, fn, cc in files:
            obj = os.path.join(d, fn.rsplit('.', 1)[0] + '.obj'); key = '0x%08X' % ea
            if not os.path.exists(obj):
                e = errs.get(fn) or []
                out[fn] = {'verdict': 'NO_COMPILE', 'why': 'cl failed', 'conv': cc,
                           'error': ('%s(%d): %s' % e[0]) if e else ''}; continue
            funcs, _ = M.coff(obj)
            # the shim's own inline helpers (XI_ANY's constructor, xi_castbits_) are emitted beside the function when
            # instantiated: they are not the function; a call to one is a relocation whose symbol names no address
            own = {k: v for k, v in funcs.items() if not SHIM_HELPER.match(k)}
            if len(own) != 1: out[fn] = {'verdict': 'NO_COMPILE', 'why': '%d functions in the object' % len(own), 'conv': cc, 'funcs': sorted(funcs)}; continue
            (sym, (code, rels)), = own.items()
            v, why, notes = M.compare(img, ext, ea, code, rels, place)
            if v == 'IDENTICAL' and notes: v, why = 'IDENTICAL_UNCOMPARED', '; '.join(notes)   # a relocation not checked
            out[fn] = {'verdict': v, 'why': why, 'conv': cc, 'size': len(code)}
        return out

    got = judge(files, 'the tree as written')
    # GCC's -fpermissive conversions written as casts (permissive), as many rounds as cl finds more
    casts = collections.Counter()
    for rnd in range(1, 9):
        redo = []
        for ea, fn, cc in files:
            if got[fn]['verdict'] != 'NO_COMPILE': continue
            t, n = permissive(open(os.path.join(d, fn)).read(), errs.get(fn, []), fn)
            if n: open(os.path.join(d, fn), 'w').write(t); casts[fn] += n; redo.append((ea, fn, cc))
        if not redo: break
        got.update(judge(redo, 'implicit conversions as casts, round %d (%d sources)' % (rnd, len(redo))))
    for ea, fn, cc in files:
        res['0x%08X' % ea] = got[fn]
        if casts[fn]: got[fn]['casts'] = casts[fn]
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
            base = re.sub(r'\d+$', '', rn) if re.fullmatch(r'(cmpk|float)\d+', rn) else rn
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
