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

BYREF_DECL = re.compile(r'^(\s+)([^;\n]*?)\s+(\**)([A-Za-z_]\w*)(\[\d+\])?; // \[esp\+([0-9A-F]+)h\](?: \[ebp-[0-9A-F]+h\])? BYREF$', re.M)

STACK_DECL = re.compile(r'^(\s+)([^;\n]*?)\s+(\**)([A-Za-z_]\w*)(\[\d+\])?; // \[esp\+([0-9A-F]+)h\](?: \[ebp-[0-9A-F]+h\])?(?: BYREF)?$', re.M)

def pointer_walks_locals(body):
    # p = &v (v a scalar stack local), then p -- or an alias of it (q = p) -- is moved or offset:
    # `p += 4`, `++q`, `q[k]`, `p - 1`, `(char *)p + k`
    scalars = {m.group(4) for m in STACK_DECL.finditer(body) if not m.group(5) and not m.group(3)}
    for p, v in re.findall(r'\b([A-Za-z_]\w*) = (?:\([^()]*\))?&([A-Za-z_]\w*);', body):
        if v not in scalars: continue
        ptrs, grow = {p}, True
        while grow:
            new = {q for q, r in re.findall(r'\b([A-Za-z_]\w*) = (?:\([^()]*\))?([A-Za-z_]\w*);', body) if r in ptrs} - ptrs
            grow = bool(new); ptrs |= new
        for q in ptrs:
            if re.search(r'\b%s\s*(\+=|-=|\+\+|--|\[|[-+] [\w(])|(\+\+|--)%s\b|\(char \*\)%s [-+] ' % (q, q, q), body):
                return True
    return False

_ELEM = {'float': 4, 'int': 4, '_DWORD': 4, 'unsigned int': 4, '_BYTE': 1, 'char': 1, 'unsigned __int8': 1,
         '__int16': 2, '_WORD': 2, 'unsigned __int16': 2, 'double': 8, '__int64': 8, '_QWORD': 8}

def split_array(body):
    ds = sorted(((int(m.group(6), 16), m) for m in STACK_DECL.finditer(body)), key=lambda x: x[0])
    for (o1, a), (o2, b) in zip(ds, ds[1:]):
        if 'BYREF' not in a.group(0) or a.group(3) or b.group(3): continue
        t = a.group(2).strip(); n = int(a.group(5)[1:-1]) if a.group(5) else 1
        # an address-taken array -- or scalar: a struct such as a rect passed by pointer -- followed
        # directly by a local of its element type
        if t in _ELEM and o1 + n * _ELEM[t] == o2 and b.group(2).strip() == t: return True
    return False

def filled_past_array(body):
    # An address-taken array followed directly by a local that is read but never assigned: the
    # buffer is one struct cut by Hex-Rays where the code stops naming bytes, and the callee that
    # fills it (Dancer_InitDevice: GetDesc into `_BYTE v8[24]`, then Width/Height read as v9/v10)
    # is the only writer of the locals after it.
    ds = sorted(((int(m.group(6), 16), m) for m in STACK_DECL.finditer(body)), key=lambda x: x[0])
    for (o1, a), (o2, b) in zip(ds, ds[1:]):
        if 'BYREF' not in a.group(0) or not a.group(5) or a.group(3): continue
        el = _ELEM.get(a.group(2).strip())
        if not el or o1 + int(a.group(5)[1:-1]) * el != o2: continue
        v = re.escape(b.group(4)); rest = body[b.end():]
        if not re.search(r'\b%s\b' % v, rest): continue
        if re.search(r'\b%s\s*(=(?!=)|[-+*/|&^]=|\+\+|--)|(\+\+|--|&)%s\b|\w+\(%s\) =' % (v, v, v), rest): continue
        return True
    return False

def negative_index(body):
    # `v72[v5 - 1]` with v5 from 0: an index below a stack array reaches the local before it (one array
    # split by Hex-Rays), which GCC places elsewhere
    arrays = {m.group(4) for m in STACK_DECL.finditer(body) if m.group(5)}
    return any(re.search(r'\b%s\[[^\]\[;]*- \d+\]' % re.escape(a), body) for a in arrays)

FRAME_DECL = re.compile(r'^\s+([^;\n]*?)\s+(\**)([A-Za-z_]\w*)(\[\d+\])?; // \[esp\+([0-9A-F]+)h\] \[ebp-([0-9A-F]+)h\]( BYREF)?$', re.M)

def truncated_array(body):
    # An address-taken array followed by frame space no local is declared in (4 bytes or more, up to the
    # next local or the frame top): Hex-Rays sized the array by the elements the code names, and a callee
    # filling the whole buffer overruns GCC's shorter one (Sky_DrawDome: v9[12] filled with 8 dwords).
    ds = [(int(m.group(5), 16), int(m.group(6), 16), m) for m in FRAME_DECL.finditer(body)]
    if not ds: return False
    top = max(x + y for x, y, _ in ds)
    starts = sorted({x for x, _, _ in ds})
    for x, y, m in ds:
        if not m.group(7) or m.group(2): continue
        el = _ELEM.get(m.group(1).strip())
        if not el: continue
        # an address-taken scalar counts too: a struct output whose first field alone was named
        # (Dancer_InitDevice: GetDesc into `int v8`, then 8 unnamed bytes, then the rest as v9[])
        end = x + (int(m.group(4)[1:-1]) if m.group(4) else 1) * el
        nxt = min([o for o in starts if o > x] + [top])
        if nxt - end >= 4: return True
    return False

def exact_frame(body):
    # Hex-Rays sometimes splits one stack array into several variables (`char v70[8]; char v71;
    # char v72;` ...) and the code then writes through a pointer to one of them across the others.
    # GCC places such variables anywhere, so the writes land on unrelated locals. When a function has
    # an address-taken single `char` (the sign of a split array), every address-taken local is
    # placed at its original stack offset inside one buffer, so overlapping accesses behave as in
    # the original.
    # A second sign: a pointer set to the address of a scalar local and then moved (`p = &v46; ...
    # p += 4`): it walks the locals after it, which Hex-Rays declared separately and need not mark
    # BYREF, so then every stack local is placed.
    # A third: an address-taken array followed directly by a local of its element type (`float v3[12];`
    # at +0xC, `float v4;` at +0x3C): one array (a 4x4 matrix) that Hex-Rays cut where the code stopped
    # naming elements; a callee that fills the whole array overruns GCC's shorter one.
    decls = list(BYREF_DECL.finditer(body))
    if pointer_walks_locals(body) or split_array(body) or negative_index(body) or truncated_array(body) \
            or filled_past_array(body):
        decls = list(STACK_DECL.finditer(body))
    elif not any(m.group(2).strip() == 'char' and not m.group(3) and not m.group(5) for m in decls): return body
    if not decls: return body
    # the buffer reaches past the end of the last local (its size counted), plus slack for overruns
    def _end(m):
        n = int(m.group(5)[1:-1]) if m.group(5) else 1
        el = 4 if m.group(3) else _ELEM.get(m.group(2).strip(), 16)
        return int(m.group(6), 16) + n * el
    top = max(_end(m) for m in decls) + 0x400
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
SIGNED_CONV = re.compile(r'(\((?:double|float|long double)\)\(?)\*\(_DWORD \*\)')
SIGNED_LOCAL = re.compile(r'^\s*(?:int|signed int|__int16|__int8|char|short|signed __int16|signed __int8) (\w+);', re.M)
DWORD_PTR_LOCAL = re.compile(r'^\s*_DWORD \*(\w+);', re.M)
SIGNED_DIV = re.compile(r'\*\(_DWORD \*\)(\([^;()]*\)) ([/%]) (\d+)(?![\w.])')

def signed_dword_compares(body):
    # `_DWORD` has no signedness in Hex-Rays output: a comparison or division takes it from the other
    # operand, and an unsigned one is written with a `u` literal (`<= 2u`). In C++ `_DWORD` is
    # uint32_t, which would make `*(_DWORD *)p + 1 > 0`, `*(_DWORD *)p / 2` or `(double)(*(_DWORD *)p + 1)`
    # unsigned: read it as int.
    body = SIGNED_CMP.sub(r'*(int *)\1 \2 ', body)
    body = SIGNED_CMP_LIT.sub(r'*(int *)\1\2 \3 \4', body)
    body = SIGNED_CMP_INT.sub(r'\1*(int *)', body)
    # a signed local (`int i`) compared with a `_DWORD`: Hex-Rays writes `(unsigned int)i` when the
    # comparison is unsigned, so a bare one is signed (`for ( i = 0; i < *(_DWORD *)(a3 + 68); ++i )`, `jl`)
    for v in set(SIGNED_LOCAL.findall(body)) | set(re.findall(r'\b(?:int|__int16|char) (\w+)[,)]', body.split('\n', 1)[0])):
        body = re.sub(r'(\b%s (?:<=|>=|<|>) )\*\(_DWORD \*\)' % re.escape(v), r'\1*(int *)', body)
        body = re.sub(r'\*\(_DWORD \*\)(\([^;()]*\)) (<=|>=|<|>) (%s)(?![\w\[(.])' % re.escape(v), r'*(int *)\1 \2 \3', body)
        # ... or with a read through a `_DWORD *` local (`v41 < *(v42 - 5)`)
        for q in DWORD_PTR_LOCAL.findall(body):
            d = r'(\*\(%s [+-] \d+\)|\*%s\b(?![\[(])|%s\[[^\];]+\])' % ((re.escape(q),) * 3)
            body = re.sub(r'(\b%s (?:<=|>=|<|>) )%s' % (re.escape(v), d), r'\1(int)\2', body)
            body = re.sub(r'%s( (?:<=|>=|<|>) %s)(?![\w\[(.])' % (d, re.escape(v)), r'(int)\1\2', body)
    body = SIGNED_CONV.sub(r'\1*(int *)', body)     # x87 has only signed `fild`: an unsigned source would show its correction
    return SIGNED_DIV.sub(r'*(int *)\1 \2 \3', body)

def inplace_buffers(body):
    # A global the IDB types as a pointer but that is really a buffer stored in place: the same
    # function copies into `&G` and then copies out of `G`. The copy out must read from `&G`.
    for g in set(re.findall(r'qmemcpy\(&([A-Za-z_]\w*),', body)):
        body = re.sub(r'(qmemcpy\([^,;]+, )%s(, )' % re.escape(g), r'\1&%s\2' % g, body)
    return body

STDCALL_VMETHOD = re.compile(r'\(\*\(\((\w[\w\s\*]*?) \(__stdcall \*\*\)\(([^()]*)\)\)(\w+)->vftable \+ (\d+)\)\)\(')

def vmethod_this(body):
    # A virtual call through a C++ object's own vftable (`g_pProcessor->vftable + 33`) that Hex-Rays typed
    # __stdcall without `this`: the original loads the vtable from ECX and passes the object there
    # (`mov ecx, [g_pProcessor]; mov edx, [ecx]; call [edx+84h]`), so it is a __thiscall on the object.
    return STDCALL_VMETHOD.sub(lambda m: '(*((%s (__thiscall **)(void *, %s))%s->vftable + %s))(%s, ' % (
        m.group(1), m.group(2), m.group(3), m.group(4), m.group(3)), body)

def body_rules(ea, body):
    # the text-level repairs, shared by the raw translation (hx2cpp) and the clean tree (hx2clean)
    body = exact_fp_literals(ea, body)
    body = fpu_status_words(body)
    body = negate_if_negative(body)
    body = float_args_in_casts(body)
    body = deleting_dtor_this(body)
    body = signed_dword_compares(body)
    body = vmethod_this(body)
    body = exact_frame(body)
    return inplace_buffers(body)

def drop_float_return(ea, ret, body):
    # IDA typed it as returning a float, but the code returns with an empty x87 stack (fpret.py): the
    # value Hex-Rays returns is computed and dropped, as the original does (e.g. through __ftol).
    # A call it returns cannot return on the x87 stack either, so such casts are retyped void too.
    fix = hx_patches.type_fixes().get(ea)
    if fix and re.match(r'void\s*(__\w+\s*)?\(', fix) and ret.strip() != 'void':
        pass                              # data/type_fixes.json: the return value is unused (no caller reads EAX)
    elif ret.strip() not in ('float', 'double', 'long double') or FPRET_ALL.get('0x%x' % ea) != 'none':
        return ret, body
    return 'void', re.sub(r'\breturn ([^;]+);', lambda m: '(void)(%s); return;' % re.sub(
        r'\((?:float|double|long double) \((__\w+) \*\)', r'(void (\1 *)', m.group(1)), body)

def _expr_at(text, i):
    # the expression starting at text[i]: up to `;`, or a `,` or `)` outside its own parentheses
    depth = 0
    for j in range(i, len(text)):
        ch = text[j]
        if ch == '(': depth += 1
        elif ch == ')':
            if depth == 0: return text[i:j]
            depth -= 1
        elif ch in ',;' and depth == 0: return text[i:j]
    return text[i:]

def _split_top(expr, op):
    # expr split at the one occurrence of `op` outside parentheses, or None
    depth, hits = 0, []
    for i, ch in enumerate(expr):
        depth += (ch == '(') - (ch == ')')
        if depth == 0 and expr.startswith(op, i) and expr[i - 1:i] != op[0] and expr[i + len(op):i + len(op) + 1] not in ('=', op[-1]):
            hits.append(i)
    return (expr[:hits[0]].strip(), expr[hits[0] + len(op):].strip()) if len(hits) == 1 else None

def fpu_status_words(body):
    # Hex-Rays splits `fcom; fnstsw ax` into condition variables (// c0, c2, c3) but leaves the status
    # word variable (// fps) unassigned, so `(fps & 0x100)` reads garbage. Each read of the status word
    # becomes the word fnstsw stores, built from the condition variables assigned most recently before it.
    # Hex-Rays writes them as c0 = a < b, c2 = 0, c3 = a == b; for an unordered pair (a NaN) x87 sets all
    # three, so the word adds the unordered case, from the operands of the c0 comparison. The variables
    # themselves keep Hex-Rays' meaning: its `(c0 | c2)` for a `test ah, 5; jp` parity test relies on it.
    decl = dict((v, k) for v, k in re.findall(r'\b(v\d+); // (fps|c0|c2|c3)\b', body))
    fps = {v for v, k in decl.items() if k == 'fps'}
    if not fps: return body
    cond = {v: k for v, k in decl.items() if k != 'fps'}
    tok = re.compile(r'\b(v\d+)\b(\s*=(?!=))?')
    last, out, pos, unord = {}, [], 0, None
    start = body.find('\n\n')            # skip the declarations
    for m in tok.finditer(body, start if start > 0 else 0):
        v, assign = m.group(1), m.group(2)
        if v in cond and assign:
            last[cond[v]] = v
            if cond[v] == 'c0':
                ab = _split_top(_expr_at(body, m.end()), '<')
                unord = '__builtin_isunordered(%s, %s)' % ab if ab else None
        elif v in fps and not assign and last:
            bit = {'c0': 0x100, 'c2': 0x400, 'c3': 0x4000}
            if unord:
                bits = ' | '.join('((%s) ? 0x%X : 0)' % (' || '.join(([last[k]] if k in last and k != 'c2' else []) + [unord]), bit[k])
                                  for k in ('c0', 'c2', 'c3'))
            else:
                bits = ' | '.join('(%s ? 0x%X : 0)' % (last[k], bit[k]) for k in ('c0', 'c2', 'c3') if k in last)
            out.append(body[pos:m.start()]); out.append('(__int16)(%s)' % bits); pos = m.end()
    out.append(body[pos:])
    return ''.join(out)

def negate_if_negative(body):
    # `fcom [0.0]; fnstsw ax; test ah, 5; jp skip` followed by `fchs` (x < 0 ? -x : x, the game's inline
    # abs) or by loading 0.0 (x < 0 ? 0 : x, a clamp): Hex-Rays writes the test as `(c0 | c2) != 0` with
    # c0/c2 never assigned, so it reads garbage. The parity test is true exactly when C0 alone is set:
    # x < 0, false for a NaN as in the original. x is the negated value, or for the clamp the variable
    # the statement just before assigned.
    decl = dict((v, k) for v, k in re.findall(r'\b(v\d+); // (c0|c2)\b', body))
    start = body.find('\n\n'); start = start if start > 0 else 0
    unset = {v for v in decl if not re.search(r'\b%s\s*=(?!=)' % v, body[start:])}
    def ok(a, b): return {decl.get(a), decl.get(b)} == {'c0', 'c2'} and a in unset and b in unset
    def neg(m):
        return '%s%s < 0.0%s' % (m.group(1), m.group(5), m.group(4)) if ok(m.group(2), m.group(3)) else m.group(0)
    def clamp(m):
        return '%s%s < 0.0%s' % (m.group(1), m.group(2), m.group(5)) if ok(m.group(3), m.group(4)) else m.group(0)
    rest = re.sub(r'(if \( )\((v\d+) \| (v\d+)\) != 0( \)\n\s*v\d+ = -(v\d+);)', neg, body[start:])
    rest = re.sub(r'((v\d+) = [^;\n]+;\n\s*if \( )\((v\d+) \| (v\d+)\) != 0( \)\n\s*\2 = 0\.0;)', clamp, rest)
    return body[:start] + rest

def _split_args(s):
    out, d, cur = [], 0, ''
    for ch in s:
        if ch == ',' and d == 0: out.append(cur.strip()); cur = ''; continue
        d += (ch == '(') - (ch == ')'); cur += ch
    if cur.strip(): out.append(cur.strip())
    return out

def float_args_in_casts(body):
    # A call through a cast function pointer whose parameter Hex-Rays typed `_DWORD` but whose argument is
    # `*(float *)p`: the original pushes the float's bits, C++ would convert the value to an integer
    # (757.0f -> 757). Such a parameter is declared `float`.
    out, pos = [], 0
    for m in re.finditer(r'\(__\w+ \*\*\)\(', body):
        i = m.end(); d = 1; j = i
        while d and j < len(body): d += {'(': 1, ')': -1}.get(body[j], 0); j += 1
        k = body.find(')(', j)
        if k < 0 or k - j > 120 or i < pos: continue
        a = k + 2; d = 1; b = a
        while d and b < len(body): d += {'(': 1, ')': -1}.get(body[b], 0); b += 1
        ps, args = _split_args(body[i:j - 1]), _split_args(body[a:b - 1])
        if len(ps) != len(args): continue
        new = [('float' if p in ('_DWORD', 'int', 'unsigned int') and x.startswith('*(float *)') else p) for p, x in zip(ps, args)]
        if new != ps:
            out.append(body[pos:i]); out.append(', '.join(new)); pos = j - 1
    out.append(body[pos:])
    return ''.join(out)

DTOR_NO_THIS = re.compile(r'\(\*\(void \(__stdcall \*\*\)\(int\)\)\(\*\(_DWORD \*\)([A-Za-z_]\w*) \+ 24\)\)\(1\)')

def deleting_dtor_this(body):
    # `mov eax,[ecx]; push 1; call [eax+0x18]`: the scalar deleting destructor (vtable slot 6), thiscall on
    # the object whose vtable it reads. Hex-Rays writes it `__stdcall (int)` and drops the object
    # (TexRef_EnsureRenderTarget 0x100755DD, CXiSkeletonActorRes_ProcessLoadS 0x100D70B3).
    return DTOR_NO_THIS.sub(lambda m: '(*(void (__thiscall **)(void *, int))(*(_DWORD *)%s + 24))((void *)%s, 1)' % (m.group(1), m.group(1)), body)

_PURGE_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'purges.json')
PURGES = json.load(open(_PURGE_PATH)) if os.path.exists(_PURGE_PATH) else {}

def callee_pops_fix(o, pt):
    # a callee IDA sees as `jmp [ptr]` (the D3DX CPU-dispatch thunks, e.g. D3DXMatrixMultiply at 0x102D6F03)
    # gets purged 0 and __cdecl, but every implementation behind the pointer is __stdcall (purges.py)
    if (o.get('purged') or 0) == 0 and PURGES.get('0x%08X' % o['ea']) and '__cdecl' in pt:
        return pt.replace('__cdecl', '__stdcall', 1)
    return pt

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
            pt = callee_pops_fix(o, pt)
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
        # the statically linked D3DX / runtime code lies after the game's last object (0x102D0000 on): an
        # unnamed function there (sub_102D538A, which calls _D3DXFilterTexture) is library code too
        if ('0x%x' % ea) in D3DX81 or re.match(r'^_?(C?D3DX)', rec['name']) or 0x102D0000 <= ea < 0x10330000:
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
