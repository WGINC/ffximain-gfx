"""hx2cpp.py -- turn Hex-Rays pseudocode (exported by ida/ffxi_ida_fix.py or ida/ida_gfx.py) into compilable C++ translation
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
def fnptr_return(ftype):
    """a function returning a pointer to a function pointer, `R (cc1 *..*cc(args))(fargs)`: the return is a
    pointer in EAX, so `void * cc(args)` (as pointer parameters are) -- same ABI"""
    m = re.match(r'^[^()]*\(\s*(?:__\w+\s*)?\*+\s*(' + CC_RE + r')?\s*\(([^()]*)\)\s*\)\s*\([^()]*\)\s*$', ftype)
    return 'void * %s(%s)' % (m.group(1) or '', m.group(m.lastindex)) if m else ftype


def ptr_type(ftype):
    """'R __cc(args)' -> 'R (__cc *)(args)'; None if not representable.
    __spoils<..> (callee preserves a scratch register) is dropped: it lets Hex-Rays see a value survive
    the call; the C++ caller re-supplies it explicitly, and GCC assumes eax/ecx/edx clobbered anyway."""
    ftype = SPOILS.sub('', ftype)
    ftype = re.sub(r'^([^()]*)\(\s*(__\w+\s*)?\*\s*\)\s*\(', r'\1 \2(', ftype)   # IDA's `R (*)(args)`: the function type
    ftype = re.sub(r'\b__noreturn\b', '', ftype)
    ftype = fnptr_return(ftype)
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

def read_past_byref(body):
    # An address-taken local -- scalar or array -- followed directly by a local whose first use is a read:
    # the callee given the address fills a struct that runs on into it, and Hex-Rays named the struct's
    # later fields as separate locals (0x10042020: `int v22` BYREF at +0x24 handed to a virtual method that
    # fills a 12-byte vector, then `float v23` at +0x28 and `int v24` at +0x2C read without being assigned).
    # GCC places such locals anywhere, so they read garbage.
    ds = sorted(((int(m.group(6), 16), m) for m in STACK_DECL.finditer(body)), key=lambda x: x[0])
    start = body.find('\n\n'); start = start if start > 0 else 0
    for (o1, a), (o2, b) in zip(ds, ds[1:]):
        if 'BYREF' not in a.group(0) or a.group(3): continue
        el = _ELEM.get(a.group(2).strip())
        if not el or o1 + (int(a.group(5)[1:-1]) if a.group(5) else 1) * el != o2: continue
        m = re.search(r'(?<![\w.>])%s\b(\s*=(?!=))?' % re.escape(b.group(4)), body[start:])
        if m and not m.group(1): return True
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
    # A fourth: an address-taken local followed directly by a local read before it is assigned (read_past_byref).
    # A fifth (byref_blocks.py): an address-taken local whose block (to the end of the frame) holds a local the code
    # writes and never reads, or bytes no local is declared in. Its address is passed, so the callee reads what the
    # original wrote there; as separate variables GCC drops the write-only ones (CDx::DrawTexturedScreenQuad 0x1000A5A0:
    # the full-screen quad's vertices, 6 Oct). Placing the locals at their original offsets is what the original does.
    decls = list(BYREF_DECL.finditer(body))
    import byref_blocks
    if pointer_walks_locals(body) or split_array(body) or negative_index(body) or truncated_array(body) \
            or filled_past_array(body) or read_past_byref(body) or byref_blocks.blocks(body):
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

def c_signed(t):
    """the signedness C++ gives an operand of IDA type t (as xi_hexrays.hpp declares it); None if unclear"""
    t = re.sub(r'^(const |volatile )+', '', t.strip())
    if '*' in t or '[' in t: return False                                # a pointer compares unsigned
    if re.match(r'(unsigned\b|_BYTE\b|_WORD\b|_DWORD\b|_QWORD\b|bool\b|BYTE\b|WORD\b|DWORD\b|UINT\b|ULONG\b|size_t\b)', t): return False
    if re.match(r'(signed |__int8\b|__int16\b|__int32\b|__int64\b|int\b|char\b|short\b|long\b|_BYTE_S\b|INT\b|LONG\b|HRESULT\b)', t): return True
    return None

CMP_LEFT = ('( ', '(', '!', ', ', '&& ', '|| ', '; ', '? ', ': ', '= ', 'return ')
CMP_RIGHT = (' )', ')', ';', ',', ' &&', ' ||', ' ?', ' :')

def ctree_signs(rec, body):
    """Each comparison as signed or unsigned as Hex-Rays read it from the jump (`jl` vs `jb`): the export's
    rec['cmps'] = [[ea, [op, signed], left, right, size, left type, right type]] (ida/ffxi_ida_fix.py). Where
    the operand types would give C++ the other signedness, both operands are cast to the comparison's own
    width and sign. A comparison whose text is not found once and unambiguously is left as it is."""
    todo = {}; both = {}
    for ea, (op, sgn), l, r, size, lt, rt in rec.get('cmps') or ():
        both.setdefault('%s %s %s' % (l, op, r), set()).add(sgn)
    for ea, (op, sgn), l, r, size, lt, rt in rec.get('cmps') or ():
        a, b = c_signed(lt), c_signed(rt)
        if a is None or b is None or size not in (1, 2, 4, 8): continue
        if size >= 4: wrong = (not (a and b)) if sgn else (a and b)     # C++: unsigned if either is unsigned
        else: wrong = a != bool(sgn) or b != bool(sgn)                # promoted to int: the operands' own sign
        if not wrong: continue
        k = '%s %s %s' % (l, op, r)
        todo[k] = sgn if len(both[k]) == 1 else None                 # the same text read both ways: ambiguous
        todo.setdefault(('size', k), size)
    T = {(1, 1): '__int8', (2, 1): '__int16', (4, 1): 'int', (8, 1): '__int64',
         (1, 0): 'unsigned __int8', (2, 0): 'unsigned __int16', (4, 0): 'unsigned int', (8, 0): 'unsigned __int64'}
    n = 0
    for k, sgn in todo.items():
        if isinstance(k, tuple) or sgn is None: continue
        hits = []
        i = body.find(k)
        while i >= 0:
            pre, post = body[:i], body[i + len(k):]
            if pre.endswith(CMP_LEFT) and post.startswith(CMP_RIGHT): hits.append(i)
            i = body.find(k, i + 1)
        if not hits: continue
        e = next(x for x in rec['cmps'] if '%s %s %s' % (x[2], x[1][0], x[3]) == k)
        t = T[(todo[('size', k)], sgn)]
        new = '(%s)(%s) %s (%s)(%s)' % (t, e[2], e[1][0], t, e[3])
        for i in reversed(hits): body = body[:i] + new + body[i + len(k):]; n += 1
    ctree_signs.count = n
    return body

STDCALL_VMETHOD = re.compile(r'\(\*\(\((\w[\w\s\*]*?) \(__stdcall \*\*\)\(([^()]*)\)\)(\w+)->vftable \+ (\d+)\)\)\(')

def vmethod_this(body):
    # A virtual call through a C++ object's own vftable (`g_pProcessor->vftable + 33`) that Hex-Rays typed
    # __stdcall without `this`: the original loads the vtable from ECX and passes the object there
    # (`mov ecx, [g_pProcessor]; mov edx, [ecx]; call [edx+84h]`), so it is a __thiscall on the object.
    return STDCALL_VMETHOD.sub(lambda m: '(*((%s (__thiscall **)(void *, %s))%s->vftable + %s))(%s, ' % (
        m.group(1), m.group(2), m.group(3), m.group(4), m.group(3)), body)

def body_rules(ea, body, exact_signs=False):
    # the text-level repairs, shared by the raw translation (hx2cpp) and the clean tree (hx2clean).
    # exact_signs: the export carries each comparison's signedness (ctree_signs has applied it), so the
    # pattern rule signed_dword_compares, which infers it from how Hex-Rays prints, is not used
    body = exact_fp_literals(ea, body)
    body = fpu_status_words(body)
    body = negate_if_negative(body)
    body = float_args_in_casts(body)
    body = thiscall_float_this(body)
    body = deleting_dtor_this(body)
    if not exact_signs: body = signed_dword_compares(body)
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
    # three, so the word adds the unordered case. Whether the pair was unordered is recorded where the
    # comparison is made (xi_fpu_unord, set with each c0 assignment from its operands), not rebuilt at the
    # read: when branches compare different operands (`if (..) c0 = a < b; else c0 = a < c;`) the read must
    # see the comparison the path took, and the operands may have changed by then (0x1017DE20 tested `a, c`
    # on the `b` path, and missed the original's call when b was a NaN). The variables themselves keep
    # Hex-Rays' meaning: its `(c0 | c2)` for a `test ah, 5; jp` parity test relies on it.
    decl = dict((v, k) for v, k in re.findall(r'\b(v\d+); // (fps|c0|c2|c3)\b', body))
    fps = {v for v, k in decl.items() if k == 'fps'}
    if not fps: return body
    cond = {v: k for v, k in decl.items() if k != 'fps'}
    tok = re.compile(r'\b(v\d+)\b(\s*=(?!=))?')
    last, out, pos, unord = {}, [], 0, False
    start = body.find('\n\n')            # skip the declarations
    start = start if start > 0 else 0
    for m in tok.finditer(body, start):
        if m.start() < pos: continue
        v, assign = m.group(1), m.group(2)
        if v in cond and assign:
            last[cond[v]] = v
            if cond[v] == 'c0':
                rhs = _expr_at(body, m.end()); ab = _split_top(rhs, '<')
                un = '__builtin_isunordered(%s, %s)' % ab if ab else '0'
                out.append(body[pos:m.start()])
                out.append('(xi_fpu_unord = %s, %s%s)' % (un, body[m.start():m.end()], rhs))
                pos = m.end() + len(rhs); unord = True
        elif v in fps and not assign and last:
            bit = {'c0': 0x100, 'c2': 0x400, 'c3': 0x4000}
            if unord:
                bits = ' | '.join('((%s) ? 0x%X : 0)' % (' || '.join(([last[k]] if k in last and k != 'c2' else []) + ['xi_fpu_unord']), bit[k])
                                  for k in ('c0', 'c2', 'c3'))
            else:
                bits = ' | '.join('(%s ? 0x%X : 0)' % (last[k], bit[k]) for k in ('c0', 'c2', 'c3') if k in last)
            out.append(body[pos:m.start()]); out.append('(__int16)(%s)' % bits); pos = m.end()
    out.append(body[pos:])
    res = ''.join(out)
    if unord:                            # declared with the function's other locals
        res = res[:start] + '\n  bool xi_fpu_unord = false;' + res[start:]
    return res

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

def _group_end(text, i):
    # text[i] is '(': the index just past its matching ')', or -1
    d = 0
    for j in range(i, len(text)):
        d += {'(': 1, ')': -1}.get(text[j], 0)
        if d == 0: return j + 1
    return -1

def thiscall_float_this(body):
    # A __thiscall's first parameter is `this`, passed in ECX, whatever Hex-Rays typed it. Typed `float` (an
    # object pointer kept in a stack slot Hex-Rays also uses as a float), GCC pushes it instead and ECX keeps a
    # stale value (CMoSchedularTask_Interpret, 0x1005CA6E `mov ecx, ebp; call [eax+0x178]` became a call with
    # ECX = 2). In `(*(T (__thiscall **)(float, ...))(ptr))(x, ...)` the parameter is declared `_DWORD` and
    # passed x's bits: COERCE_FLOAT(y) becomes y; any other float, xi_bitcast<uint32_t>((float)(x)).
    out, pos = [], 0
    for m in re.finditer(r'\(\*\([^()]*\(__thiscall \*\*\)\((float|double)(?=[,)])', body):
        if m.start() < pos: continue
        pl = m.end() - len(m.group(1)) - 1                    # '(' of the parameter list
        pe = _group_end(body, pl)
        if pe < 0 or body[pe:pe + 2] != ')(': continue
        ee = _group_end(body, pe + 1)                          # the pointer expression
        if ee < 0 or body[ee:ee + 2] != ')(': continue
        ae = _group_end(body, ee + 1)
        if ae < 0: continue
        ps, args = _split_args(body[pl + 1:pe - 1]), _split_args(body[ee + 2:ae - 1])
        if not args or len(ps) != len(args): continue
        x = args[0]; inner = x[len('COERCE_FLOAT('):-1]
        if x.startswith('COERCE_FLOAT(') and _expr_at(x, len('COERCE_FLOAT(')) == inner: arg0 = inner
        else: arg0 = 'xi_bitcast<uint32_t>((float)(%s))' % x
        out += [body[pos:pl + 1], ', '.join(['_DWORD'] + ps[1:]), body[pe - 1:ee + 2], ', '.join([arg0] + args[1:])]
        pos = ae - 1
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
# a function with no `ret` that tail-jumps at stack depth 0 pops what its target's `ret N` pops (tail_purges.py)
_TAIL_PATH = os.path.join(os.path.dirname(_PURGE_PATH), 'tail_purges.json')
if os.path.exists(_TAIL_PATH):
    PURGES.update({k: v['pops'] for k, v in json.load(open(_TAIL_PATH)).items() if k not in PURGES})

def callee_pops_fix(o, pt):
    # a callee IDA sees as `jmp [ptr]` (the D3DX CPU-dispatch thunks, e.g. D3DXMatrixMultiply at 0x102D6F03)
    # gets purged 0 and __cdecl, but every implementation behind the pointer is __stdcall (purges.py)
    if (o.get('purged') or 0) == 0 and PURGES.get('0x%08X' % o['ea']) and '__cdecl' in pt:
        return pt.replace('__cdecl', '__stdcall', 1)
    return pt


def ghidra_prepare(rec):
    """a Ghidra record (GfxExport) made ready for translation, the same for the raw translation (here) and the clean
    tree (hx2clean.py): (rec, None, None), or (None, status, why) when it cannot be translated"""
    if GHIDRA_REG.search(rec.get('text', '').partition('\n{')[2]):
        return None, 'skipped', 'register or stack input Ghidra could not tie to a parameter: ' + \
            ', '.join(sorted(set(GHIDRA_REG.findall(rec['text'].partition('\n{')[2]))))
    rec = ghidra_fix_convention(rec)
    t, why = ghidra_icalls(rec)
    if t is None: return None, 'untranslatable', why
    rec = dict(rec, text=t)
    if rec.get('x87_locals'):
        # locals that hold an x87 register value (GfxExport.x87Locals): the precision of ST, not of float
        head, brace, rest = rec['text'].partition('\n{')
        for n in rec['x87_locals']:
            rest = re.sub(r'^(\s*)(?:float|double)(\s+%s;)' % re.escape(n), r'\1float10\2', rest, count=1, flags=re.M)
        rec = dict(rec, text=head + brace + rest)
    head, brace, rest = rec['text'].partition('\n{')
    rest, why = ghidra_consts(rest)
    if rest is None: return None, 'untranslatable', why
    rec = dict(rec, text=head + brace + ghidra_pieces(rest))
    if rec.get('type'):
        # Ghidra's text omits the calling convention in its signature; the record's type has it
        head, brace, rest = rec['text'].partition('\n{')
        rec = dict(rec, text='\n'.join(l for l in head.splitlines() if l.startswith('//')) + '\n' + rec['type'] + brace + rest)
    return rec, None, None

def ghidra_fix_convention(rec):
    # Ghidra's calling convention for the function itself, checked against its real `ret N`:
    # stdcall that pops nothing is cdecl; cdecl that pops is stdcall
    real = PURGES.get('0x%08X' % rec['ea'])
    t, cc = rec.get('type', ''), rec.get('cc', '__cdecl')
    if cc == '__thiscall' and rec.get('args') and rec['args'][0][0] == 'this' and not re.search(r'\(\s*\w[\w\s*]*\bthis\b', t):
        # a prototype string leaves out Ghidra's automatic `this` (ECX); the record's parameter list has it
        t = re.sub(r'\(\s*(void)?\s*\)\s*$', '(%s this)' % rec['args'][0][1], t) if re.search(r'\(\s*(void)?\s*\)\s*$', t) \
            else re.sub(r'\(', '(%s this, ' % rec['args'][0][1], t, count=1)
    if real == 0 and '__stdcall' in t: t, cc = t.replace('__stdcall', '__cdecl'), '__cdecl'
    elif real and '__cdecl' in t: t, cc = t.replace('__cdecl', '__stdcall'), '__stdcall'
    return dict(rec, type=t, cc=cc)

ICALL_MARK = re.compile(r'/\*XI_ICALL:(0x[0-9A-F]+)\*/')

def _balanced(text, i):
    # index just past the bracket group opening at text[i]
    depth = 0
    for j in range(i, len(text)):
        if text[j] in '([': depth += 1
        elif text[j] in ')]':
            depth -= 1
            if not depth: return j + 1
    return None

def ghidra_icalls(rec):
    # an indirect call GfxExport.java marked `/*XI_ICALL:0xCALL*/callee(args)`: called with the convention and
    # stack arguments rec['icalls'] holds (conv_facts.py: ecx_this_sites / sp_solved / vcalls / com_calls / real_sp)
    text, icalls = rec['text'], rec.get('icalls') or {}
    if rec.get('icalls_unmarked'): return None, 'indirect calls with an established convention could not be marked in the text'
    while True:
        m = ICALL_MARK.search(text)
        if not m: return text, None
        cc, n, ret = icalls.get(m.group(1), (None, 0, '-'))
        i = m.end()
        if text[i] == '(': j = _balanced(text, i)
        else:
            mm = re.match(r'[A-Za-z_]\w*', text[i:]); j = i + mm.end() if mm else None
        while j is not None and j < len(text) and text[j].isspace(): j += 1
        if j is None or text[j] != '(': return None, 'indirect call %s: callee expression not parsed' % m.group(1)
        k = _balanced(text, j)
        args, depth, cur = [], 0, ''
        for c in text[j + 1:k - 1]:
            if c in '([': depth += 1
            elif c in ')]': depth -= 1
            if c == ',' and depth == 0: args.append(cur); cur = ''
            else: cur += c
        if cur.strip(): args.append(cur)
        want = n + (1 if cc == '__thiscall' else 0)
        if cc is None or len(args) != want:
            return None, 'indirect call %s: %d arguments in the text, %s %d established' % (m.group(1), len(args), cc, want)
        params = (['void *'] if cc == '__thiscall' else []) + ['XI_ANY'] * n
        if cc == '__thiscall': args[0] = '(void *)(%s)' % args[0].strip()
        call = '((%s (%s *)(%s))(%s))(%s)' % ('float10' if ret == 'float' else 'int', cc, ', '.join(params) or 'void',
                                             text[i:j], ','.join(args))
        text = text[:m.start()] + call + text[k:]

GHIDRA_AUTO = re.compile(r'\b((DAT|PTR|FUN|s|u|UNK|BYTE|WORD|DWORD)_(?:\w*?_)?([0-9a-f]{8}))\b')

def ghidra_auto_objs(rec, body):
    # Ghidra's automatic names carry their address (DAT_10456a2c, s_text_1034f00c, FUN_1011bba0); they
    # are not stored symbols, so the exporter cannot list them. LAB_ (goto labels) stay labels.
    objs = dict(rec.get('objs', {}))
    for m in GHIDRA_AUTO.finditer(body):
        nm, kind, ea = m.group(1), m.group(2), int(m.group(3), 16)
        if nm in objs: continue
        if kind == 'FUN' and ea == rec['ea'] and rec.get('type'):
            # a call to itself: the exporter lists no object for it; the record's own prototype is the callee's
            objs[nm] = {'ea': ea, 'func': True, 'type': re.sub(r'\b%s\s*\(' % re.escape(rec['name']) if rec.get('name') else r'(?!)', '(', rec['type']),
                        'printed': [nm], 'purged': None}
        elif kind == 'FUN':
            objs[nm] = {'ea': ea, 'func': True, 'type': 'undefined4 __cdecl(void)', 'printed': [nm], 'purged': None}
        else:
            objs[nm] = {'ea': ea, 'func': False, 'type': 'undefined4', 'printed': [nm], 'size': 4,
                        'is_str': kind in ('s', 'u')}
    for m in re.finditer(r'&\s*(LAB_([0-9a-f]{8}))\b', body):    # a code address taken as a value
        if m.group(1) not in objs:
            objs[m.group(1)] = {'ea': int(m.group(2), 16), 'func': False, 'type': 'undefined1', 'printed': [m.group(1)],
                                'size': 1, 'is_str': False}
    return objs

K_MARK = re.compile(r'/\*XI_K:(4|8):([0-9A-F]+)\*/(-?(?:\d+\.\d*(?:e[+-]?\d+)?|\d+e[+-]?\d+|\d+|INFINITY|NAN))')
BARE_FLOAT = re.compile(r'(?<![\w.])\d+(?:\.\d*(?:e[+-]?\d+)?|e[+-]?\d+)(?![\w.])')

def ghidra_consts(text):
    # a floating constant GfxExport marked `/*XI_K:size:bits*/value`: its own encoding (xi_k4 / xi_k8), not the
    # double C++ reads from Ghidra's printed value; a floating literal left without its width is refused
    text = K_MARK.sub(lambda m: 'xi_k%s(0x%sull)' % (m.group(1), m.group(2)), text)
    rest = re.sub(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'', '""', text)
    m = BARE_FLOAT.search(rest)
    return (None, 'floating constant %s without its width' % m.group(0)) if m else (text, None)

def ghidra_bool(text):
    # Ghidra's `bool` is a 1-byte type and its `(bool)x` a change of type, not of value: `cmp cl, al` against a
    # sete result compares the whole byte (0x10227C09). C++'s bool would turn 2 into 1, so it is a plain byte.
    parts = re.split(r'("(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\')', text)
    return ''.join(x if i % 2 else re.sub(r'\bbool\b', 'xi_bool', x) for i, x in enumerate(parts))

def ghidra_pieces(text):
    # Ghidra's piece accessor `x._O_S_`: the S bytes at byte offset O of x's storage (little-endian x86), an lvalue
    # (xi_ghidra.hpp XI_PIECE); x is the postfix expression before the `.`
    while True:
        m = re.search(r'\._(\d+)_(\d+)_(?!\w)', text)
        if not m: return text
        i = m.start()
        while True:
            j = i
            while j > 0 and text[j - 1].isspace(): j -= 1
            if j and text[j - 1] in ')]':
                op, cl = ('(', ')') if text[j - 1] == ')' else ('[', ']')
                d, k = 0, j - 1
                while k >= 0:
                    d += (text[k] == cl) - (text[k] == op)
                    if d == 0: break
                    k -= 1
                i = k
                if text[k] == '(' and not re.search(r'[\w\])]\s*$', text[:k]): break   # a parenthesised operand
                continue
            mm = re.search(r'[A-Za-z_]\w*$', text[:j])
            if mm:
                i = mm.start()
                if i >= 1 and text[i - 1] == '.' or text[max(0, i - 2):i] == '->': i -= 1 if text[i - 1] == '.' else 2; continue
            break
        text = text[:i] + 'XI_PIECE(%s, %s, %s)' % (text[i:m.start()], m.group(1), m.group(2)) + text[m.end():]

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

IMPORT_NARGS = json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'import_nargs.json')))

def ghidra_import_type(name, o):
    # a call through an import table slot: the Windows SDK's declared prototype (GfxExport applies Ghidra's
    # windows_vs12_32.gdt by name), its argument count checked against IDA's SDK prototypes (import_nargs.json)
    if not o.get('import_proto'): return None, 'import %s: no SDK prototype' % name
    m = re.match(r'^(.*?)\s*' + CC_RE + r'?\s*\((.*)\)\s*$', SPOILS.sub('', o['type']), re.S)
    if not m: return None, 'import %s: prototype not parsed: %s' % (name, o['type'])
    ret, cc, params = m.group(1).strip() or 'int', m.group(2) or '__cdecl', m.group(3).strip()
    if '...' in params: return None, 'import %s: variadic' % name
    n = 0 if params in ('', 'void') else params.count(',') + 1
    if IMPORT_NARGS.get(name) is not None and IMPORT_NARGS[name] != n:
        return None, 'import %s: SDK prototype has %d arguments, import_nargs.json %d' % (name, n, IMPORT_NARGS[name])
    if ret != 'void': ret = 'uint32_t'           # every value returned in EAX; the caller's text casts it
    return '%s (%s *)(%s)' % (ret, cc, ', '.join(['XI_ANY'] * n)), None

CC_REGS = {'__thiscall': ['ECX'], '__fastcall': ['ECX', 'EDX']}

def extra_regs(cc, storage):
    # parameters in registers the convention itself does not pass (an EAX or EDX input GfxExport gave a
    # callee that leaves the register as it found it); "other" storage is not expressible either
    own = CC_REGS.get(cc, [])
    return [(i, r) for i, r in enumerate(storage or []) if r != 'stack' and (i >= len(own) or own[i] != r)]

def ghidra_reg_call(name, o, body, tok):
    # a call to a callee that takes a register its convention does not cover: an asm call that loads EAX, ECX,
    # EDX as the callee's storage says, pushes the stack arguments, and pops them itself when the callee does not
    # (its `ret N`, purges.json). The callee returns EDX:EAX; ESI/EDI/EBX/EBP are callee-saved in the game's code.
    m = re.match(r'^(.*?)\s*' + CC_RE + r'?\s*\((.*)\)\s*$', SPOILS.sub('', o['type']), re.S)
    ret = (m.group(1).strip() if m else 'int') or 'int'
    if re.search(r'float|double', ret): return None, 'callee %s returns in ST0 and takes a register input' % name
    st = o.get('storage') or []
    regs = [r for r in st if r != 'stack']
    if any(r not in ('EAX', 'ECX', 'EDX') for r in regs) or len(set(regs)) != len(regs):
        return None, 'callee %s takes parameters in %s' % (name, ', '.join(regs))
    real = PURGES.get('0x%08X' % o['ea'])
    purge = real if real is not None else (o['purged'] if o.get('purged') is not None and o['purged'] >= 0 else None)
    if purge is None: return None, 'callee %s takes a register input and its pop count is unknown' % name
    nstack = purge // 4 if purge else max(call_arg_counts(body, name) or [0]) - len(regs)
    n = len(regs) + max(nstack, 0)
    where = list(st) + ['stack'] * (n - len(st))
    if len(where) != n: return None, 'callee %s: %d parameters for %d arguments' % (name, len(st), n)
    fn = 'xi_cr_%08X' % o['ea']
    args = ', '.join('XI_ANY a%d' % i for i in range(n))
    stk = [i for i, w in enumerate(where) if w == 'stack']
    rv = {r: 'a%d.bits' % i for i, r in enumerate(where) if r != 'stack'}
    lines = ['static inline unsigned long long %s(%s) {' % (fn, args),
             '    uint32_t xs[%d] = {%s}; uint32_t ra = %s, rc = %s, rd = %s;' % (max(len(stk), 1), ', '.join('a%d.bits' % i for i in stk) or '0',
                 rv.get('EAX', '0'), rv.get('ECX', '0'), rv.get('EDX', '0')),
             '    __asm__ volatile(' + ''.join('"pushl %d(%%%%esi)\\n\\t"' % (4 * k) for k in reversed(range(len(stk))))
             + '"call *%%edi\\n\\t"' + ('"addl $%d, %%%%esp\\n\\t"' % (4 * len(stk)) if stk and not purge else ''),
             '        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x%08Xu) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");' % o['ea'],
             '    return ((unsigned long long)rd << 32) | ra;', '}',
             '#define %s %s' % (tok, fn)]
    return lines, None

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
    body = ctree_signs(rec, '{' + body)          # on Hex-Rays' own text, before any name is replaced
    syms = []; reps = []
    if rec.get('source') == 'ghidra': rec = dict(rec, objs=ghidra_auto_objs(rec, body))
    if rec.get('source') == 'ghidra' and extra_regs(rec.get('cc'), rec.get('storage')):
        return None, 'entry takes %s in a register its convention does not pass' % ', '.join(r for i, r in extra_regs(rec.get('cc'), rec.get('storage')))
    for nm, o in rec.get('objs', {}).items():
        oea = o['ea']
        if o['func']:
            tok = 'F_%08X' % oea
            if o.get('import'):
                pt, why = ghidra_import_type(nm, o)
                if pt is None: return None, why
                syms.append('typedef %s;' % re.sub(r'\((\w+) \*\)', r'(\1 *P_%08X)' % oea, pt, count=1))
                syms.append('#define %s (*reinterpret_cast<P_%08X *>(0x%08Xu))' % (tok, oea, oea))
            elif rec.get('source') == 'ghidra' and FPRET_ALL.get('0x%x' % oea) == 'consumes':
                # fpret.json: the callee pops a value the caller leaves in ST0 (__ftol); Ghidra passes it as its
                # one parameter (GfxExport, `st0in`); EDX:EAX come back as the callee leaves them
                syms.append('#define %s(x) xi_call_st0(0x%08Xu, (x))' % (tok, oea))
            elif rec.get('source') == 'ghidra' and extra_regs(re.search(CC_RE, o['type']).group(0) if re.search(CC_RE, o['type']) else '__cdecl', o.get('storage')):
                lines, why = ghidra_reg_call(nm, o, body, tok)
                if lines is None: return None, why
                syms.extend(lines)
            else:
                pt = ghidra_ptr_type(nm, o, body) if rec.get('source') == 'ghidra' else ptr_type(o['type'])
                if pt is None: return None, 'callee %s has no C++ type (%s)' % (nm, o['type'])
                pt = callee_pops_fix(o, pt)
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
    body = body_rules(ea, body, 'cmps' in rec)
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
CONSTRUCTS = {k: v for k, v in json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'constructs.json'))).items() if not k.startswith('_')}
D3DX81 = json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'd3dx', 'd3dx81_manifest.json')))
BAD2 = re.compile(r'__usercall|__userpurge|JUMPOUT|__asm|BUG\(\)|__halt|(?<![A-Z])_UNKNOWN\b(?!\s*\*)|MEMORY\[|'
                  r'__readfsdword|__writefsdword|__outbyte|__inbyte|__CFSHL__|__SEH_|__try|__except|\bretaddr\b')
# Ghidra's names for a value its prototype does not carry: a register read on entry (in_ECX), one a callee left
# (extraout_EDX), a register kept across the function (unaff_ESI), a stack slot past the parameters (in_stack_..).
# Compiled as written these are uninitialised locals; the translation refuses them instead (conv_facts.py sets
# the conventions the game's code establishes before decompiling, so what is left is not expressible as typed).
GHIDRA_REG = re.compile(r'\b(?:in|unaff|extraout)_\w+')
LIB_TEXT = re.compile(r'\b(CD3DX\w*|Iostream_init|type_info|std::\w+|exception)\b')

def types_header():
    """The IDB's types header as C++ accepts it: IDA names a method pointer's first parameter `this` (a C++
    keyword); it becomes `self`, as in the function bodies. Parameter names do not change any type."""
    t = open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'gfx_types_game.h')).read()
    return re.sub(r'\bthis\b', 'self', t)

def main(src, outdir):
    os.makedirs(outdir, exist_ok=True)
    # a quoted #include searches the including file's directory first, so this copy is the one compiled
    with open(os.path.join(outdir, 'gfx_types_game.h'), 'w') as f: f.write(types_header())
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
        if '0x%08X' % ea in CONSTRUCTS:          # established from the code (data/constructs.json)
            meta['%08X' % ea] = {'status': 'skipped', 'why': 'construct: ' + CONSTRUCTS['0x%08X' % ea]['construct']}; continue
        if 'skip' in rec or 'text' in rec:
            m = BAD2.search(rec.get('text', ''))
            if m or 'text' not in rec:
                meta['%08X' % ea] = {'status': 'skipped', 'why': 'unsupported construct ' + (m.group(0) if m else rec.get('skip', '?'))}; continue
        if False:
            meta['%08X' % ea] = {'status': 'library', 'why': 'D3DX / C++ runtime code, not game code'}; continue
        rec = hx_patches.apply(rec)
        if rec.get('source') == 'ghidra':
            rec, status, why = ghidra_prepare(rec)
            if rec is None:
                meta['%08X' % ea] = {'status': status, 'why': why}; continue
        rec = dict(rec, text=SPOILS.sub('', rec['text']))
        r, why = translate(rec)
        if r is None:
            meta['%08X' % ea] = {'status': 'untranslatable', 'why': why}; continue
        if rec.get('source') == 'ghidra':
            r = dict(r, syms=[ghidra_bool(x) for x in r['syms']], code=ghidra_bool(r['code']))
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
