"""hx2cpp.py -- turn Hex-Rays pseudocode (exported by ida_gfx.py) into compilable C++ translation
units for the equivalence gate.

Every referenced game object is replaced by an address-bound token:
  G_<ADDR>  a global / string / table, as an lvalue of its IDA type at its real address
  F_<ADDR>  a game function, as a typed function pointer to its real address
so a reconstruction reads the same memory and calls the same original code as the function it
replaces. The function itself is exported as xi_<ADDR> with the original calling convention.
`this` becomes `self` (it is a keyword in C++ and these are free functions at this stage).

Usage: python3 hx2cpp.py gfx_export2.jsonl outdir/
"""
import json, os, re, struct, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from msvc_demangle import demangle
import hx_patches

OPAQUE = {'Iostream_init', 'exception', 'type_info', 'codecvt_base'}
CC_RE = r'(__cdecl|__stdcall|__thiscall|__fastcall)'

SPOILS = re.compile(r'\s*__spoils<[^>]*>')
def ptr_type(ftype):
    """'R __cc(args)' -> 'R (__cc *)(args)'; None if not representable.
    __spoils<..> (callee preserves a scratch register) is dropped: it lets Hex-Rays see a value survive
    the call; the C++ caller re-supplies it explicitly, and GCC assumes eax/ecx/edx clobbered anyway."""
    ftype = SPOILS.sub('', ftype)
    ftype = re.sub(r'^([^()]*)\(\s*(__\w+\s*)?\*\s*\)\s*\(', r'\1 \2(', ftype)   # IDA's `R (*)(args)`: the function type
    ftype = re.sub(r'\b__noreturn\b', '', ftype)
    m = re.match(r'^(.*?)\s*' + CC_RE + r'\s*\((.*)\)\s*$', ftype, re.S)
    if m:
        ret, cc, args = m.groups()
    else:
        m = re.match(r'^(.*?)\s*\((.*)\)\s*$', ftype, re.S)
        if not m: return None
        ret, args = m.groups(); cc = '__cdecl'
    if re.search(r'__usercall|__userpurge|@<', ftype): return None
    args = re.sub(r'\bthis\b', 'self', args)
    # pointer parameters become void *: same ABI, and any pointer the decompiled caller holds
    # converts implicitly (C++ rejects e.g. void*** -> _DWORD** that C accepted)
    parts, depth, cur = [], 0, ''
    for ch in args:
        if ch == ',' and depth == 0: parts.append(cur); cur = ''; continue
        depth += (ch == '(') - (ch == ')'); cur += ch
    if cur.strip(): parts.append(cur)
    # Windows pointer typedefs (LPCWSTR, LPSYSTEMTIME, ...) are pointers too
    parts = ['void *' if '*' in p or '[' in p or re.match(r'^(const\s+)?LP[A-Z]\w*\b', p.strip()) else p.strip() for p in parts]
    ret = re.sub(CC_RE, '', ret)          # a calling convention left in the return type (`void __cdecl __noreturn`)
    return '%s (%s *)(%s)' % (ret.strip() or 'int', cc, ', '.join(parts))

def typedef_for(name, t):
    """typedef declaring `name` as type string t (handles arrays and function pointers)."""
    t = t.strip()
    if not t or t in ('_UNKNOWN', 'void'): return 'typedef _BYTE %s;' % name
    m = re.match(r'^(.*?)((?:\[\d*\])+)$', t)            # arrays: T[n][m]
    if m: return 'typedef %s %s%s;' % (m.group(1).strip(), name, m.group(2))
    m = re.match(r'^(.*?\(\s*' + CC_RE + r'?\s*\*+)(\s*\).*)$', t, re.S)   # function pointers
    if m: return 'typedef %s%s%s;' % (m.group(1), name, m.group(3))
    if re.search(CC_RE + r'\s*\(', t) or re.match(r'^[^(]*\(.*\)$', t):   # a function type itself
        return 'typedef _BYTE %s;' % name
    return 'typedef %s %s;' % (t, name)

def token_re(p):
    if re.match(r'^[A-Za-z_][\w:~<>,\s*&]*$', p) and not p.startswith('"'):
        return re.compile(r'(?<![\w:])' + re.escape(p) + r'(?![\w])')
    return re.compile(re.escape(p))

_FPC_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'fp_consts.json')
FP_CONSTS = json.load(open(_FPC_PATH)) if os.path.exists(_FPC_PATH) else {}
FP_LIT = re.compile(r'(?<![\w.])(\d+\.\d*(?:[eE][+-]?\d+)?)(?![\w.])')

def exact_fp_literals(ea, body):
    # Hex-Rays prints a float constant as its shortest decimal; as a C double that is a different
    # number. Write it as the exact value the original's x87 instruction loads (fpconst.py).
    f32 = [float.fromhex(v) for k, v in FP_CONSTS.get('0x%08X' % ea, {}).values() if k == 'f32']
    if not f32: return body
    def fix(m):
        d = float(m.group(1))
        r = struct.unpack('<f', struct.pack('<f', d))[0] if abs(d) < 3.4e38 else d
        return r.hex() if r != d and r in f32 else m.group(1)
    return FP_LIT.sub(fix, body)

_FPS_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'fp_callsites.json')
FP_SITES = json.load(open(_FPS_PATH)) if os.path.exists(_FPS_PATH) else None
FPRET_ALL = json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'fpret.json')))
FPRET_FNS = {'0x%08X' % int(k, 16) for k, v in FPRET_ALL.items() if v in ('float', 'double')}

def wrap_calls(body, name, fmt):
    # replace each call `name(...)` (balanced parentheses) by fmt % call
    out, pos = [], 0
    for m in re.finditer(r'(?<![\w.>])%s\s*\(' % re.escape(name), body):
        if m.start() < pos: continue
        depth, i = 1, m.end()
        while i < len(body) and depth:
            depth += {'(': 1, ')': -1}.get(body[i], 0); i += 1
        out.append(body[pos:m.start()]); out.append(fmt % body[m.start():i]); pos = i
    out.append(body[pos:])
    return ''.join(out)

def unconsumed_fp_calls(ea, syms, body):
    # A callee typed `float`/`double` makes GCC pop its x87 result even when the result is unused.
    # Where the original caller never reads ST0 after the call (fp_callsites.py), the callee is
    # declared `void` instead, so neither side touches the x87 stack after it. A callee proven to
    # return nothing on the x87 stack (fpret `none`) is always void; where Hex-Rays uses its "result",
    # the call becomes `(call, 0.0)`: the value never existed, and the original never reads it.
    used = set(FP_SITES.get('0x%08X' % ea, ())) if FP_SITES is not None else set()
    out = []
    for s in syms:
        m = re.match(r'#define (F_([0-9A-F]{8})) \(reinterpret_cast<(float|double) (\(__\w+ \*\)\(.*\))>\(0x[0-9A-F]+u\)\)$', s)
        if m:
            none = FPRET_ALL.get('0x%x' % int(m.group(2), 16)) == 'none'
            if none or (FP_SITES is not None and '0x' + m.group(2) in FPRET_FNS and '0x' + m.group(2) not in used):
                calls = re.findall(r'(?m)^(\s*)%s\(' % m.group(1), body)
                stmt_only = len(calls) == len(re.findall(r'\b%s\b' % m.group(1), body))
                if stmt_only or none:
                    if not stmt_only:
                        body = wrap_calls(body, m.group(1), '(%s, 0.0)')
                    s = s.replace('<%s %s>' % (m.group(3), m.group(4)), '<void %s>' % m.group(4))
        out.append(s)
    return out, body

BYREF_DECL = re.compile(r'^(\s+)([^;\n]*?)\s+(\**)(v\d+)(\[\d+\])?; // \[esp\+([0-9A-F]+)h\](?: \[ebp-[0-9A-F]+h\])? BYREF$', re.M)

def exact_frame(body):
    # Hex-Rays sometimes splits one stack array into several variables (`char v70[8]; char v71;
    # char v72;` ...) and the code then writes through a pointer to one of them across the others.
    # GCC places such variables anywhere, so the writes land on unrelated locals. When a function has
    # an address-taken single `char` (the sign of a split array), every address-taken local is
    # placed at its original stack offset inside one buffer, so overlapping accesses behave as in
    # the original.
    decls = list(BYREF_DECL.finditer(body))
    if not any(m.group(2).strip() == 'char' and not m.group(3) and not m.group(5) for m in decls): return body
    top = max(int(m.group(6), 16) for m in decls) + 0x400
    out, pos, first = [], 0, True
    for m in decls:
        indent, t, stars, name, arr, off = m.group(1), m.group(2).strip(), m.group(3), m.group(4), m.group(5), int(m.group(6), 16)
        out.append(body[pos:m.start()])
        if first:
            out.append('%salignas(16) unsigned char xi_frame[0x%X];  // address-taken locals at their original offsets\n' % (indent, top))
            first = False
        t = t + ' ' + stars if stars else t
        if arr:
            out.append('%s%s (&%s)%s = *reinterpret_cast<%s (*)%s>(xi_frame + 0x%X);' % (indent, t, name, arr, t, arr, off))
        else:
            out.append('%s%s &%s = *reinterpret_cast<%s *>(xi_frame + 0x%X);' % (indent, t, name, t, off))
        pos = m.end()
    out.append(body[pos:])
    return ''.join(out)

SIGNED_CMP = re.compile(r'\*\(_DWORD \*\)(\([^;()]*\)) (<=|>=|<|>) (?=\(__int16\)|SHIWORD\(|SLOWORD\(|\(int\)|\(char\)|\(__int8\)|\(signed)')
SIGNED_CMP_LIT = re.compile(r'\*\(_DWORD \*\)(\([^;()]*\))((?: [+-] \d+)?) (<=|>=|<|>) (-?\d+)(?![\w.])')
SIGNED_CMP_INT = re.compile(r'(\(int\)[^;<>=()]*? (?:<=|>=|<|>) )\*\(_DWORD \*\)')
SIGNED_DIV = re.compile(r'\*\(_DWORD \*\)(\([^;()]*\)) ([/%]) (\d+)(?![\w.])')

def signed_dword_compares(body):
    # `_DWORD` has no signedness in Hex-Rays output: a comparison or division takes it from the other
    # operand, and an unsigned one is written with a `u` literal (`<= 2u`). In C++ `_DWORD` is
    # uint32_t, which would make `*(_DWORD *)p + 1 > 0` or `*(_DWORD *)p / 2` unsigned: read it as int.
    body = SIGNED_CMP.sub(r'*(int *)\1 \2 ', body)
    body = SIGNED_CMP_LIT.sub(r'*(int *)\1\2 \3 \4', body)
    body = SIGNED_CMP_INT.sub(r'\1*(int *)', body)
    return SIGNED_DIV.sub(r'*(int *)\1 \2 \3', body)

def inplace_buffers(body):
    # A global the IDB types as a pointer but that is really a buffer stored in place: the same
    # function copies into `&G` and then copies out of `G`. The copy out must read from `&G`.
    for g in set(re.findall(r'qmemcpy\(&([A-Za-z_]\w*),', body)):
        body = re.sub(r'(qmemcpy\([^,;]+, )%s(, )' % re.escape(g), r'\1&%s\2' % g, body)
    return body

def body_rules(ea, body):
    # the text-level repairs, shared by the raw translation (hx2cpp) and the clean tree (hx2clean)
    body = exact_fp_literals(ea, body)
    body = fpu_status_words(body)
    body = signed_dword_compares(body)
    body = exact_frame(body)
    return inplace_buffers(body)

def drop_float_return(ea, ret, body):
    # IDA typed it as returning a float, but the code returns with an empty x87 stack (fpret.py): the
    # value Hex-Rays returns is computed and dropped, as the original does (e.g. through __ftol).
    # A call it returns cannot return on the x87 stack either, so such casts are retyped void too.
    if ret.strip() not in ('float', 'double', 'long double') or FPRET_ALL.get('0x%x' % ea) != 'none':
        return ret, body
    return 'void', re.sub(r'\breturn ([^;]+);', lambda m: '(void)(%s); return;' % re.sub(
        r'\((?:float|double|long double) \((__\w+) \*\)', r'(void (\1 *)', m.group(1)), body)

def fpu_status_words(body):
    # Hex-Rays splits `fcom; fnstsw ax` into condition variables (// c0, c2, c3) but leaves the status
    # word variable (// fps) unassigned, so `(fps & 0x100)` reads garbage. Each read of the status word
    # becomes the word fnstsw stores, built from the condition variables assigned most recently before it.
    decl = dict((v, k) for v, k in re.findall(r'\b(v\d+); // (fps|c0|c2|c3)\b', body))
    fps = {v for v, k in decl.items() if k == 'fps'}
    if not fps: return body
    cond = {v: k for v, k in decl.items() if k != 'fps'}
    tok = re.compile(r'\b(v\d+)\b(\s*=(?!=))?')
    last, out, pos = {}, [], 0
    start = body.find('\n\n')            # skip the declarations
    for m in tok.finditer(body, start if start > 0 else 0):
        v, assign = m.group(1), m.group(2)
        if v in cond and assign: last[cond[v]] = v
        elif v in fps and not assign and last:
            bits = ' | '.join('(%s ? 0x%X : 0)' % (last[k], b) for k, b in (('c0', 0x100), ('c2', 0x400), ('c3', 0x4000)) if k in last)
            out.append(body[pos:m.start()]); out.append('(__int16)(%s)' % bits); pos = m.end()
    out.append(body[pos:])
    return ''.join(out)

_PURGE_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'purges.json')
PURGES = json.load(open(_PURGE_PATH)) if os.path.exists(_PURGE_PATH) else {}

def ghidra_fix_convention(rec):
    # Ghidra's calling convention for the function itself, checked against its real `ret N`:
    # stdcall that pops nothing is cdecl; cdecl that pops is stdcall
    real = PURGES.get('0x%08X' % rec['ea'])
    t, cc = rec.get('type', ''), rec.get('cc', '__cdecl')
    if real == 0 and '__stdcall' in t: t, cc = t.replace('__stdcall', '__cdecl'), '__cdecl'
    elif real and '__cdecl' in t: t, cc = t.replace('__cdecl', '__stdcall'), '__stdcall'
    return dict(rec, type=t, cc=cc)

GHIDRA_AUTO = re.compile(r'\b((DAT|PTR|FUN|s|u|UNK|BYTE|WORD|DWORD)_(?:\w*?_)?([0-9a-f]{8}))\b')

def ghidra_auto_objs(rec, body):
    # Ghidra's automatic names carry their address (DAT_10456a2c, s_text_1034f00c, FUN_1011bba0); they
    # are not stored symbols, so the exporter cannot list them. LAB_ (goto labels) stay labels.
    objs = dict(rec.get('objs', {}))
    for m in GHIDRA_AUTO.finditer(body):
        nm, kind, ea = m.group(1), m.group(2), int(m.group(3), 16)
        if nm in objs: continue
        if kind == 'FUN':
            objs[nm] = {'ea': ea, 'func': True, 'type': 'undefined4 __cdecl(void)', 'printed': [nm], 'purged': None}
        else:
            objs[nm] = {'ea': ea, 'func': False, 'type': 'undefined4', 'printed': [nm], 'size': 4,
                        'is_str': kind in ('s', 'u')}
    return objs

def call_arg_counts(body, name):
    # number of arguments at each call `name(...)` in the body (top-level commas)
    out = []
    for m in re.finditer(r'(?<![\w.>])%s\s*\(' % re.escape(name), body):
        depth, i, n, empty = 1, m.end(), 1, True
        while i < len(body) and depth:
            c = body[i]
            if c in '([{': depth += 1
            elif c in ')]}': depth -= 1
            elif c == ',' and depth == 1: n += 1
            if depth and not c.isspace(): empty = False
            i += 1
        out.append(0 if empty else n)
    return out

def ghidra_ptr_type(name, o, body):
    # Ghidra's committed prototypes can disagree with its call sites (a callee's unused parameters are
    # dropped). What the original really pushes is fixed by the callee's purge (callee-cleanup) or by
    # the call sites (cdecl), so the call goes through XI_ANY slots of that count (xi_ghidra.hpp).
    m = re.match(r'^(.*?)\s*' + CC_RE + r'?\s*\((.*)\)\s*$', SPOILS.sub('', o['type']), re.S)
    ret = (m.group(1).strip() if m else 'int') or 'int'
    cc = (m.group(2) if m else None) or '__cdecl'
    regs = {'__thiscall': 1, '__fastcall': 2}.get(cc, 0)
    real = PURGES.get('0x%08X' % o['ea'])
    if o.get('purged') is not None and not 0 <= o['purged'] <= 0x400: o = dict(o, purged=None)   # Ghidra: unknown
    if real == 0 and cc == '__stdcall': cc = '__cdecl'      # pops nothing: the caller cleans up
    if cc in ('__stdcall', '__thiscall', '__fastcall') and (real is not None or o.get('purged') is not None):
        nstack = (real if real is not None else o['purged']) // 4
    elif real:                                       # Ghidra says cdecl, but the callee pops: stdcall
        cc, nstack = '__stdcall', real // 4
    else:
        nstack = max(call_arg_counts(body, name) or [0]) - regs
    params = ['void *'] * regs + ['XI_ANY'] * max(nstack, 0)
    return '%s (%s *)(%s)' % (ret, cc, ', '.join(params))

def translate(rec):
    ea = rec['ea']; text = rec['text']
    head, brace, body = text.partition('\n{')
    if not brace: return None, 'no body'
    # leading comment lines of the signature are kept as comments above the function
    sig_lines = [l for l in head.splitlines() if not l.startswith('//')]
    comments = [l for l in head.splitlines() if l.startswith('//')]
    sig = ' '.join(sig_lines).strip()
    body = '{' + body
    syms = []; reps = []
    if rec.get('source') == 'ghidra': rec = dict(rec, objs=ghidra_auto_objs(rec, body))
    for nm, o in rec.get('objs', {}).items():
        oea = o['ea']
        if o['func']:
            pt = ghidra_ptr_type(nm, o, body) if rec.get('source') == 'ghidra' else ptr_type(o['type'])
            if pt is None: return None, 'callee %s has no C++ type (%s)' % (nm, o['type'])
            tok = 'F_%08X' % oea
            syms.append('#define %s (reinterpret_cast<%s>(0x%08Xu))' % (tok, pt, oea))
        else:
            tok = 'G_%08X' % oea
            ot = 'char[1]' if o.get('is_str') else o['type']   # a string object is its address
            if re.sub(r'^(struct\s+)?(std::)?', '', ot.strip()) in OPAQUE:
                ot = 'char[1]'   # a FLIRT runtime type (layout unknown) on a game object: use it as its address
            syms.append(typedef_for('T_%08X' % oea, ot))
            syms.append('#define %s XI_EA(T_%08X, 0x%08Xu)' % (tok, oea, oea))
        forms = set(o.get('printed', [nm])) | {nm}
        for p in list(forms):
            m1 = re.match(r'^_?([A-Za-z]\w*)@\d+$', p)          # _Name@N (stdcall decoration)
            if m1: forms.add(m1.group(1))
            m2 = re.match(r'^\?(\w+)@(\w+)@@', p)              # ?method@Class@@... (MSVC mangling)
            if m2: forms.add('%s::%s' % (m2.group(2), m2.group(1)))
            if p.startswith('_') and re.match(r'^_\w+$', p): forms.add(p[1:])
            dm = demangle(p)
            if dm: forms.add(dm)
        for p in forms:
            reps.append((p, tok, oea))
    # a global with the same name as a parameter (the IDB has a global named `self`): Hex-Rays writes
    # the global as `::name`, so only that form is the global
    pnames = set(re.findall(r'(\w+)\s*(?:\[[^]]*\])?\s*(?:,|$)', sig[sig.find('(') + 1:sig.rfind(')')]))
    shadow = {p for p, tok, oea in reps if p in pnames}
    for p, tok, oea in reps:
        if p in shadow: body = body.replace('::' + p, tok)
    reps = [r for r in reps if r[0] not in shadow]
    # longest printed form first so `sub_1000` never eats part of `sub_10001234`
    for p, tok, oea in sorted(reps, key=lambda r: -len(r[0])):
        if oea == ea:  # the function's own name in its signature is handled below
            body = token_re(p).sub(tok, body)
            continue
        body = token_re(p).sub(tok, body)
        sig = token_re(p).sub(tok, sig)
    m = re.match(r'^(.*?)\s*' + CC_RE + r'?\s*([~\w:?@$<>]+)\s*\((.*)\)\s*$', SPOILS.sub('', sig), re.S)
    if not m: return None, 'unparsed signature: ' + sig[:120]
    ret, cc, _name, params = m.groups()
    cc = cc or '__cdecl'
    params = re.sub(r'\bthis\b', 'self', params)
    # C++ needs a declared enum for `enum X`; the D3D enums are declared without the underscore
    body = re.sub(r'\benum\s+_?(D3D\w+)', r'\1', body); params = re.sub(r'\benum\s+_?(D3D\w+)', r'\1', params)
    ret = re.sub(r'\benum\s+_?(D3D\w+)', r'\1', ret)
    body = re.sub(r'\bthis\b', 'self', body)
    body = body_rules(ea, body)
    fname = 'xi_%08X' % ea
    out = ['// FUNCTION: FFXIMAIN 0x%08X  %s' % (ea, rec['name'])]
    out += comments
    ret, body = drop_float_return(ea, ret, body)
    syms, body = unconsumed_fp_calls(ea, syms, body)
    out.append('extern "C" __declspec(dllexport) %s %s %s(%s)' % (ret.strip(), cc, fname, params.strip() or 'void'))
    out.append(body.rstrip())
    return {'syms': syms, 'code': '\n'.join(out), 'export': fname, 'cc': cc, 'ret': ret.strip()}, None

PRELUDE = '''#include "xi_hexrays.hpp"
#include "xi_d3d8.hpp"
#include "gfx_types_game.h"
#include "xi_compat_win.hpp"
'''
# Microsoft's statically linked D3DX and C++ runtime are not game code: never reconstruct them
LIBRARY = re.compile(r'^(CD3DX|D3DX|_D3DX|\?\?|\?.*@std@@|.*Iostream|.*type_info|std::)')
D3DX81 = json.load(open(os.path.join(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data'), 'd3dx81_matches.json')))
BAD2 = re.compile(r'__usercall|__userpurge|JUMPOUT|__asm|BUG\(\)|__halt|(?<![A-Z])_UNKNOWN\b(?!\s*\*)|MEMORY\[|'
                  r'__readfsdword|__writefsdword|__outbyte|__inbyte|__CFSHL__|__SEH_|__try|__except|\bretaddr\b')
LIB_TEXT = re.compile(r'\b(CD3DX\w*|Iostream_init|type_info|std::\w+|exception)\b')

def main(src, outdir):
    os.makedirs(outdir, exist_ok=True)
    meta = {}
    for line in open(src):
        rec = json.loads(line)
        ea = rec['ea']
        # library = Microsoft code: byte-identical to DirectX 8.1 d3dx8.lib (libmatch.py) or named D3DX.
        # Calling something runtime-named does NOT make a function library code (1 Oct 2026 audit).
        if ('0x%x' % ea) in D3DX81 or re.match(r'^_?(C?D3DX)', rec['name']):
            meta['%08X' % ea] = {'status': 'library', 'why': 'D3DX 8.1: %s' % D3DX81.get('0x%x' % ea, {}).get('symbol', rec['name'])}; continue
        if 'skip' in rec or 'text' in rec:
            m = BAD2.search(rec.get('text', ''))
            if m or 'text' not in rec:
                meta['%08X' % ea] = {'status': 'skipped', 'why': 'unsupported construct ' + (m.group(0) if m else rec.get('skip', '?'))}; continue
        if False:
            meta['%08X' % ea] = {'status': 'library', 'why': 'D3DX / C++ runtime code, not game code'}; continue
        rec = hx_patches.apply(rec)
        if rec.get('source') == 'ghidra':
            rec = ghidra_fix_convention(rec)
        if rec.get('source') == 'ghidra' and rec.get('type'):
            # Ghidra's text omits the calling convention in its signature; the record's type has it
            head, brace, rest = rec['text'].partition('\n{')
            rec = dict(rec, text='\n'.join(l for l in head.splitlines() if l.startswith('//')) + '\n' + rec['type'] + brace + rest)
        rec = dict(rec, text=SPOILS.sub('', rec['text']))
        r, why = translate(rec)
        if r is None:
            meta['%08X' % ea] = {'status': 'untranslatable', 'why': why}; continue
        with open(os.path.join(outdir, 'fn_%08X.cpp' % ea), 'w') as f:
            f.write(PRELUDE + ('#include "xi_ghidra.hpp"\n' if rec.get('source') == 'ghidra' else '')
                    + '\n'.join(r['syms']) + '\n\n' + r['code'] + '\n')
        meta['%08X' % ea] = {'status': 'translated', 'export': r['export'], 'cc': r['cc'], 'ret': r['ret'],
                             'name': rec['name']}
    json.dump(meta, open(os.path.join(outdir, 'meta.json'), 'w'), indent=0)
    import collections
    print(collections.Counter(v['status'] for v in meta.values()))

if __name__ == '__main__':
    main(sys.argv[1], sys.argv[2])
