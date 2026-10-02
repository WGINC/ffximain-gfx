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

OPAQUE = {'Iostream_init', 'exception', 'type_info', 'codecvt_base'}
CC_RE = r'(__cdecl|__stdcall|__thiscall|__fastcall)'

SPOILS = re.compile(r'\s*__spoils<[^>]*>')
def ptr_type(ftype):
    """'R __cc(args)' -> 'R (__cc *)(args)'; None if not representable.
    __spoils<..> (callee preserves a scratch register) is dropped: it lets Hex-Rays see a value survive
    the call; the C++ caller re-supplies it explicitly, and GCC assumes eax/ecx/edx clobbered anyway."""
    ftype = SPOILS.sub('', ftype)
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
    parts = ['void *' if '*' in p or '[' in p else p.strip() for p in parts]
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
FPRET_FNS = {'0x%08X' % int(k, 16) for k, v in json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)),
             '..', 'data', 'fpret.json'))).items() if v in ('float', 'double')}

def unconsumed_fp_calls(ea, syms, body):
    # A callee typed `float`/`double` makes GCC pop its x87 result even when the result is unused.
    # Where the original caller never reads ST0 after the call (fp_callsites.py), the callee is
    # declared `void` instead, so neither side touches the x87 stack after it.
    if FP_SITES is None: return syms
    used = set(FP_SITES.get('0x%08X' % ea, ()))
    out = []
    for s in syms:
        m = re.match(r'#define (F_([0-9A-F]{8})) \(reinterpret_cast<(float|double) (\(__\w+ \*\)\(.*\))>\(0x[0-9A-F]+u\)\)$', s)
        if m and '0x' + m.group(2) in FPRET_FNS and '0x' + m.group(2) not in used:
            calls = re.findall(r'(?m)^(\s*)%s\(' % m.group(1), body)
            if len(calls) == len(re.findall(r'\b%s\b' % m.group(1), body)):
                s = s.replace('<%s %s>' % (m.group(3), m.group(4)), '<void %s>' % m.group(4))
        out.append(s)
    return out

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
    for nm, o in rec.get('objs', {}).items():
        oea = o['ea']
        if o['func']:
            pt = ptr_type(o['type'])
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
    body = exact_fp_literals(ea, body)
    body = fpu_status_words(body)
    fname = 'xi_%08X' % ea
    out = ['// FUNCTION: FFXIMAIN 0x%08X  %s' % (ea, rec['name'])]
    out += comments
    out.append('extern "C" __declspec(dllexport) %s %s %s(%s)' % (ret.strip(), cc, fname, params.strip() or 'void'))
    out.append(body.rstrip())
    syms = unconsumed_fp_calls(ea, syms, body)
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
        rec = dict(rec, text=SPOILS.sub('', rec['text']))
        r, why = translate(rec)
        if r is None:
            meta['%08X' % ea] = {'status': 'untranslatable', 'why': why}; continue
        with open(os.path.join(outdir, 'fn_%08X.cpp' % ea), 'w') as f:
            f.write(PRELUDE + '\n'.join(r['syms']) + '\n\n' + r['code'] + '\n')
        meta['%08X' % ea] = {'status': 'translated', 'export': r['export'], 'cc': r['cc'], 'ret': r['ret'],
                             'name': rec['name']}
    json.dump(meta, open(os.path.join(outdir, 'meta.json'), 'w'), indent=0)
    import collections
    print(collections.Counter(v['status'] for v in meta.values()))

if __name__ == '__main__':
    main(sys.argv[1], sys.argv[2])
