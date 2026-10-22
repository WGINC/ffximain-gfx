"""msvc_tu.py -- a tree function as a translation unit of its own for Visual C++ 6.0, and whether it compiles to the
game's bytes (msvc_match.py).

The tree is written for MinGW (tree/build.sh: C++17, GCC attributes, `undefined4` a struct). This writes, for one
function, the unit MSVC6 can compile with the same meaning:
  * the integer types as MSVC6 spells them (it has no <stdint.h>); `undefined4` and the other Ghidra types as plain
    unsigned integers of their size. The unit is for functions with no floating-point code, where the struct's
    bit-copying from a float never arises;
  * `nullptr` as 0 (MSVC6 predates it); Hex-Rays' `qmemcpy` (an inline `rep movs`) as the memcpy intrinsic;
  * an untyped argument to a game function (XI_ANY: the argument's bytes in a zeroed dword) as `xi_any(arg)`, which
    gives the same dword: an overload for each integer type (zero-extended below 4 bytes) and for pointers. Its type
    is deduced from the argument, which MSVC6 does correctly; an argument of any other type is a compile error;
  * `at<T>(p, off)` written out as `*(T *)((uint8_t *)(p) + (off))`: MSVC6 does not take a function template's
    argument when no parameter names it, so the template is not used;
  * `this->field` of the class (the tree's header: `TYPE name;` and its `static_assert(offsetof(...) == OFF)`) as
    the same cast at that offset, so no layout is redeclared;
  * a `__thiscall` free function (MSVC6 takes `__thiscall` on members only) as a member of a struct of its own,
    `this` in ECX as before, its first parameter's name bound to `this`;
  * a member function as a member of a class declared with that one function (`static` as the header declares it).

Declared from the tree's own headers: each global the body names (`gfx_globals.hpp`: its typedef, `extern`, and its
address for the match), and each game function it calls (`gfx_orig.hpp`: the wrapper's cast type and address; a
`__thiscall` callee as a member of a struct `XI_C_<name>` called through its first argument). A type name those
declarations use that the unit does not know is declared as an incomplete class.

What is left out, each with its reason (the unit is not written): a function the gate did not pass; one that takes
an address-taken frame (`xi_frame`), calls through a pointer, reads its entry EAX (`xi_entry_eax`), uses floating
point (or calls a game function that does), uses a GCC attribute or a raw game address (`XI_EA`, `XI_GAME`,
`XI_CODE`), or calls a game function whose wrapper is not typed or returns XI_ANY; a `static __thiscall` member; a
`this->` name the header gives no offset for. Nothing is guessed: a name the unit does not declare is a compile
error, reported as such.

    MSVC6_PP=... FFXI_DLL=... python3 msvc_tu.py TREE OUT.json [ADDR ...]       (no ADDR: every function)
OUT: {addr: {"result": "MATCH" | "DIFFER" | "COMPILE" | "LEFT OUT", "why": ...}}; with MSVC_TU_KEEP=DIR each unit is
also written to DIR/<addr>.cpp."""
import glob, json, os, re, sys
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)

SHIM = r'''typedef signed char int8_t; typedef unsigned char uint8_t; typedef short int16_t; typedef unsigned short uint16_t;
typedef int int32_t; typedef unsigned int uint32_t; typedef __int64 int64_t; typedef unsigned __int64 uint64_t;
typedef unsigned int uintptr_t;
typedef unsigned char undefined; typedef unsigned char undefined1; typedef unsigned short undefined2;
typedef unsigned int undefined3; typedef unsigned int undefined4; typedef unsigned __int64 undefined8;
typedef unsigned char byte; typedef unsigned int uint; typedef unsigned short ushort; typedef unsigned int ulong;
typedef int8_t _BYTE_S; typedef uint8_t _BYTE; typedef uint16_t _WORD; typedef uint32_t _DWORD; typedef uint64_t _QWORD;
typedef int8_t _BOOL1; typedef int32_t _BOOL4; typedef uint8_t _UNKNOWN;
#define XI_API
#define nullptr 0
extern "C" void * __cdecl memcpy(void *, const void *, unsigned int);
#pragma intrinsic(memcpy)
#define qmemcpy memcpy
inline unsigned int xi_any(char v) { return (unsigned char)v; }
inline unsigned int xi_any(signed char v) { return (unsigned char)v; }
inline unsigned int xi_any(unsigned char v) { return v; }
inline unsigned int xi_any(bool v) { return (unsigned char)v; }
inline unsigned int xi_any(short v) { return (unsigned short)v; }
inline unsigned int xi_any(unsigned short v) { return v; }
inline unsigned int xi_any(int v) { return (unsigned int)v; }
inline unsigned int xi_any(unsigned int v) { return v; }
inline unsigned int xi_any(long v) { return (unsigned int)v; }
inline unsigned int xi_any(unsigned long v) { return v; }
template <class T> inline unsigned int xi_any(T *v) { return (unsigned int)v; }
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
#define WORD1(x) WORDn(x, 1)
#define SLOBYTE(x) (*((int8_t *)&(x)))
#define SLOWORD(x) (*((int16_t *)&(x)))
#define SHIWORD(x) (*((int16_t *)&(x) + 1))
#define SHIBYTE(x) (*((int8_t *)&(x) + sizeof(x) - 1))
#define SLODWORD(x) (*((int32_t *)&(x)))
#define SHIDWORD(x) (*((int32_t *)&(x) + 1))
'''

LEAVE_OUT = [('orig_void::', 'calls a game function whose float result it drops (orig_void::)'), ('xi_frame', 'address-taken frame (xi_frame)'),
             ('xi_entry_eax', 'reads its entry EAX'), ('__attribute__', 'a GCC attribute'),
             ('XI_EA', 'names a game global'), ('XI_GAME', 'names a game address'), ('XI_CODE', 'names a game function')]
FLOAT = re.compile(r'\bfloat\b|\bdouble\b|long double|\bfloat10\b|\bxi_k[48]\b')
INDIRECT = re.compile(r'\(\*\*?\(code|__thiscall \*|__stdcall \*|__cdecl \*|__fastcall \*')


def functions(tree):
    """{addr: (file, verified, definition text)} for every `// FUNCTION:` of the tree"""
    res = {}
    for p in sorted(glob.glob(os.path.join(tree, 'src', '*.cpp'))):
        for s in open(p).read().split('// FUNCTION: FFXIMAIN ')[1:]:
            a = int(s[:10], 16); lines = s.split('\n')
            k = 1
            while k < len(lines) and lines[k].startswith('//'): k += 1
            text = '\n'.join(lines[k:]); end = text.find('\n}\n')
            res[a] = (p, any(l.startswith('// GATE: PASS') for l in lines[1:k]), text[:end + 2] if end >= 0 else text)
    return res


def header_info(tree):
    """({addr: declaration line}, {class: {member: (type, offset)}}) from the tree's headers"""
    decl, fields = {}, {}
    for p in glob.glob(os.path.join(tree, 'include', '*.hpp')):
        t = open(p, errors='replace').read()
        for line in t.split('\n'):
            m = re.search(r'\);\s*//\s*0x([0-9A-Fa-f]{8})\b', line)
            if m: decl[int(m.group(1), 16)] = line.strip()
        for m in re.finditer(r'static_assert\(\s*(?:XI_OFFSETOF|offsetof|__builtin_offsetof)\(\s*(\w+)\s*,\s*(\w+)\s*\)\s*==\s*(0x[0-9A-Fa-f]+|\d+)', t):
            cls, mem, off = m.group(1), m.group(2), int(m.group(3), 0)
            d = re.search(r'^\s*([\w:<> ]+?[\s*&]+)' + re.escape(mem) + r'\s*;', t, re.M)
            if d: fields.setdefault(cls, {})[mem] = (d.group(1).strip(), off)
    return decl, fields


def globals_table(tree):
    """{name: (typedef line, address)} from include/gfx_globals.hpp (`typedef T NAME_t;` and
    `#define NAME XI_EA(NAME_t, 0xADDRu)`)"""
    t = open(os.path.join(tree, 'include', 'gfx_globals.hpp'), errors='replace').read()
    tds = {}
    for m in re.finditer(r'^typedef\s.*?\b(\w+)_t\b[^;]*;$', t, re.M): tds[m.group(1)] = m.group(0)
    res = {}
    for m in re.finditer(r'^#define (\w+) XI_EA\(\1_t, (0x[0-9A-Fa-f]+)u\)$', t, re.M):
        if m.group(1) in tds: res[m.group(1)] = (tds[m.group(1)], int(m.group(2), 16))
    return res


KNOWN = set('''void char short int long unsigned signed bool float double __int64 const volatile struct class union enum
int8_t uint8_t int16_t uint16_t int32_t uint32_t int64_t uint64_t uintptr_t undefined undefined1 undefined2 undefined3
undefined4 undefined8 byte uint ushort ulong _BYTE _WORD _DWORD _QWORD _BYTE_S _BOOL1 _BOOL4 _UNKNOWN typedef'''.split())
CONVS = {'__cdecl', '__stdcall', '__fastcall', '__thiscall'}


def global_decls(body, globs):
    """(declarations, {name: address}) for the globals the body names; a type name the unit does not know is
    declared as an incomplete class, so using its members is a compile error, not a guess"""
    used = sorted(n for n in set(re.findall(r'\b[A-Za-z_]\w*\b', body)) if n in globs)
    fwd, lines, addr = set(), [], {}
    for n in used:
        td, a = globs[n]
        for w in re.findall(r'\b[A-Za-z_]\w*\b', td.split(n + '_t')[0]):
            if w not in KNOWN: fwd.add(w)
        lines.append(td); lines.append('extern %s_t %s;' % (n, n)); addr[n] = a
    return ''.join('class %s;\n' % w for w in sorted(fwd)) + '\n'.join(lines) + ('\n' if lines else ''), addr


def orig_table(tree):
    """{name: (return type, convention, [parameter types], address)} from include/gfx_orig.hpp's wrappers
    `XI_ORIG ... NAME(...) { ... reinterpret_cast<RET (CONV *)(TYPES)>(XI_GAME(0xADDRu))(...); }`"""
    res = {}
    for line in open(os.path.join(tree, 'include', 'gfx_orig.hpp'), errors='replace'):
        m = re.match(r'^XI_ORIG .*?\b(\w+)\(.*reinterpret_cast<(.*?) \((__\w+) \*\)\((.*?)\)>\(XI_GAME\((0x[0-9A-Fa-f]+)u\)\)', line)
        if m:
            ps = [x.strip() for x in m.group(4).split(',')] if m.group(4).strip() not in ('', 'void') else []
            res[m.group(1)] = (m.group(2).strip(), m.group(3), ps, int(m.group(5), 16))
    return res


def call_args(text, i):
    """(arguments, end) of the call whose `(` is at text[i]"""
    depth, args, start = 0, [], i + 1
    for j in range(i, len(text)):
        ch = text[j]
        if ch == '(': depth += 1
        elif ch == ')':
            depth -= 1
            if depth == 0:
                if text[start:j].strip(): args.append(text[start:j].strip())
                return args, j + 1
        elif ch == ',' and depth == 1: args.append(text[start:j].strip()); start = j + 1
    raise ValueError('unbalanced call')


def game_calls(body, origs):
    """(body with each orig:: call written for MSVC6, declarations, {symbol: address}) or (None, reason, None).
    A __thiscall callee is a member of a struct of its own, called through its first argument."""
    decls, addr, done = [], {}, set()
    while True:
        m = re.search(r'\borig::(\w+)\s*\(', body)
        if not m: return body, decls, addr
        n = m.group(1); o = origs.get(n)
        if o is None: return None, 'calls %s, whose wrapper is not typed' % n, None
        ret, conv, ps, a = o
        if 'XI_ANY' in ret: return None, 'calls %s, whose return is untyped (XI_ANY)' % n, None
        if FLOAT.search(ret) or any(FLOAT.search(x) for x in ps): return None, 'calls %s, which takes or returns floating point' % n, None
        try: args, end = call_args(body, m.end() - 1)
        except ValueError as e: return None, str(e), None
        if len(args) != len(ps): return None, 'calls %s with %d arguments, its wrapper takes %d' % (n, len(args), len(ps)), None
        args = ['xi_any(%s)' % x if p_ == 'XI_ANY' else x for x, p_ in zip(args, ps)]
        ps = ['unsigned int' if p_ == 'XI_ANY' else p_ for p_ in ps]
        if conv == '__thiscall':
            if not ps or not args: return None, 'calls %s (__thiscall) with no this' % n, None
            st = 'XI_C_' + n
            if n not in done:
                decls.append('struct %s { %s %s(%s); };' % (st, ret, n, ', '.join(ps[1:])))
                addr['%s::%s' % (st, n)] = a; done.add(n)
            new = '((%s *)(%s))->%s(%s)' % (st, args[0], n, ', '.join(args[1:]))
        else:
            if n not in done:
                decls.append('%s %s %s(%s);' % (ret, conv, n, ', '.join(ps)))
                addr[n] = a; done.add(n)
            new = '%s(%s)' % (n, ', '.join(args))
        body = body[:m.start()] + new + body[end:]


def expand_at(text):
    """`at<T>(p, off)` -> `(*(T *)((uint8_t *)(p) + (off)))`, innermost first"""
    while True:
        m = re.search(r'\bat<([^<>]+(?:<[^<>]*>)?)>\(', text)
        if not m: return text
        i = m.end(); depth = 1; comma = None
        for j in range(i, len(text)):
            ch = text[j]
            if ch == '(': depth += 1
            elif ch == ')':
                depth -= 1
                if depth == 0: break
            elif ch == ',' and depth == 1 and comma is None: comma = j
        if comma is None: raise ValueError('at<> without two arguments')
        new = '(*(%s *)((uint8_t *)(%s) + (%s)))' % (m.group(1).strip(), text[i:comma].strip(), text[comma + 1:j].strip())
        text = text[:m.start()] + new + text[j + 1:]


def unit(a, defn, decl, fields, globs=None, origs=None):
    """(source, symbol prefix) or (None, reason)"""
    for k, why in LEAVE_OUT:
        if k in defn: return None, why
    if FLOAT.search(defn): return None, 'floating point'
    if INDIRECT.search(defn): return None, 'calls through a pointer'
    head, body = defn.split('\n{', 1) if '\n{' in defn else (None, None)
    if head is None: return None, 'definition not found'
    head = ' '.join(head.split())
    m = re.match(r'^(?:XI_API\s+)?(.*?)\s*\b((?:\w+::)?\w+)\s*\((.*)\)\s*$', head)
    if not m: return None, 'signature not parsed: ' + head
    ret, name, params = m.group(1), m.group(2), m.group(3)
    conv = re.search(r'\b(__cdecl|__stdcall|__fastcall|__thiscall)\b', ret)
    ret = re.sub(r'\b(__cdecl|__stdcall|__fastcall|__thiscall)\b', '', ret).strip()
    conv = conv.group(1) if conv else None
    body = '{' + body
    body, gdecl, gaddr = game_calls(body, origs or {})
    if body is None: return None, gdecl
    if '::' in name:
        cls, meth = name.split('::')
        hd = decl.get(a, '')
        static = bool(re.match(r'static\b', hd))
        if static and conv == '__thiscall': return None, 'a static __thiscall member'
        fl = fields.get(cls, {})
        def repl(mm):
            f = fl.get(mm.group(1))
            if not f: raise KeyError(mm.group(1))
            return 'at<%s>(this, 0x%X)' % f
        try: body = re.sub(r'\bthis->(\w+)\b(?!\s*\()', repl, body)
        except KeyError as e: return None, 'this->%s: no offset in the header' % e.args[0]
        cv = conv + ' ' if conv and conv != '__thiscall' else ''
        src = 'class %s { public: %s%s %s%s(%s); };\n%s %s%s(%s)\n%s\n' % (
            cls, 'static ' if static else '', ret, cv, meth, params, ret, cv, name, params, body)
        sym = '?%s@%s@@' % (meth, cls)
    elif conv == '__thiscall':
        ps = [x.strip() for x in params.split(',')] if params.strip() not in ('', 'void') else []
        if not ps: return None, '__thiscall with no parameter'
        pm = re.match(r'^(.*?)(\w+)$', ps[0])
        if not pm: return None, 'first parameter not parsed: ' + ps[0]
        rest = ', '.join(ps[1:])
        st = 'XI_%08X' % a
        src = 'struct %s { %s %s(%s); };\n%s %s::%s(%s)\n{\n  %s%s = (%s)this;\n%s\n}\n' % (
            st, ret, name, rest, ret, st, name, rest, pm.group(1), pm.group(2), pm.group(1).strip(), body)
        sym = '?%s@%s@@' % (name, st)
    else:
        src = '%s %s%s(%s)\n%s\n' % (ret, conv + ' ' if conv else '', name, params, body)
        sym = '?%s@@' % name
    try: src = expand_at(src)
    except ValueError as e: return None, str(e)
    g, unit.addr = global_decls(defn, globs or {})
    unit.addr.update(gaddr)
    callees = {k.split('::')[-1] for k in gaddr}
    fwd = sorted({w for d in gdecl for w in re.findall(r'\b[A-Za-z_]\w*\b', d)
                  if w not in KNOWN and w not in CONVS and w not in callees and not w.startswith('XI_C_')}
                 - set(re.findall(r'class (\w+);', g)))
    return SHIM + g + ''.join('class %s;\n' % w for w in fwd) + ''.join(d + '\n' for d in gdecl) + src, sym


def main(tree, out, *addrs):
    import msvc_match as M
    fn = functions(tree); decl, fields = header_info(tree); globs = globals_table(tree); origs = orig_table(tree)
    want = [int(x, 16) for x in addrs] or sorted(fn)
    keep = os.environ.get('MSVC_TU_KEEP'); res = {}
    if keep: os.makedirs(keep, exist_ok=True)
    for a in want:
        if a not in fn: res['0x%08X' % a] = {'result': 'LEFT OUT', 'why': 'not in the tree'}; continue
        p, ok, defn = fn[a]
        if not ok: res['0x%08X' % a] = {'result': 'LEFT OUT', 'why': 'not verified by the gate'}; continue
        src, sym = unit(a, defn, decl, fields, globs, origs)
        if src is None: res['0x%08X' % a] = {'result': 'LEFT OUT', 'why': sym}; continue
        if keep: open(os.path.join(keep, '%08X.cpp' % a), 'w').write(src)
        good, why = M.match(src, sym, a, unit.addr)
        r = 'MATCH' if good else ('COMPILE' if why.startswith('compile failed') or why.startswith('symbol') else 'DIFFER')
        res['0x%08X' % a] = {'result': r, 'why': why[-600:]}
        print('0x%08X %s %s' % (a, r, why.splitlines()[-1][:120] if why else ''), flush=True)
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    import collections
    print(collections.Counter(v['result'] for v in res.values()))


if __name__ == '__main__':
    main(*sys.argv[1:])
