"""hx2clean.py -- clean, class-shaped C++ for the gate-verified graphics functions.

Input: the Hex-Rays export, the raw gate results (only PASS functions are emitted), the tree
manifest (class/method names) and the tree headers (class sizes, bases).

Per function:
  * placement: tree manifest class > `Class_Method` IDA name with a known class > typed `this`
    (CDx *, D3DHolder *) > free function. Nothing else is guessed.
  * members: `self` becomes `this`; `*((T *)self + N)` / `*(T *)(self + N)` / `self->_pad_H[N]`
    become `this->field_XXX` (layout inferred from every member of the class). Anything left is
    reached through `self`, a byte-exact alias declared at the top of the body, so no pointer
    arithmetic ever changes meaning.
  * D3D: `X->lpVtbl->M(X, a...)` -> `X->M(a...)`; literal render-state / texture-stage values get
    their D3D enum names.
  * calls into the game: `orig::Name(...)` (always-inline wrapper around the original address);
    globals keep their IDB names, bound to their addresses in gfx_globals.hpp.
  * decompiler noise (register / stack-slot comments) removed.
  * partial-variable macros (LOBYTE, HIWORD, ...) on integer locals and parameters: exact C (partial_regs.py).

Usage: python3 hx2clean.py export.jsonl gate.jsonl manifest.json tree_include_dir out_dir
"""
import glob, json, os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import collections
from msvc_demangle import demangle
import hx_patches
import hx2cpp
import partial_regs

def _head_params(text):
    """the parameter list of a record's text, from its signature line"""
    head = text.partition('\n{')[0]
    return head[head.find('(') + 1:head.rfind(')')] if '(' in head else ''


CC_RE = r'(__cdecl|__stdcall|__thiscall|__fastcall)'
PS2_NAMES = {}
REVIEWED = {}                  # reviewed_names(): this repository's reviewed names, by address
WIDTH = {'_BYTE': 1, 'char': 1, '__int8': 1, 'unsigned __int8': 1, '_WORD': 2, '__int16': 2,
         'unsigned __int16': 2, '_DWORD': 4, 'int': 4, 'float': 4, 'unsigned int': 4, '__int32': 4,
         'double': 8, '__int64': 8, '_QWORD': 8}
FTYPE = {'_BYTE': 'uint8_t', 'char': 'int8_t', '__int8': 'int8_t', 'unsigned __int8': 'uint8_t',
         '_WORD': 'uint16_t', '__int16': 'int16_t', 'unsigned __int16': 'uint16_t', '_DWORD': 'uint32_t',
         'int': 'int32_t', 'unsigned int': 'uint32_t', '__int32': 'int32_t', 'float': 'float',
         'double': 'double', '__int64': 'int64_t', '_QWORD': 'uint64_t'}
KNOWN_FIELDS = {('CDx', 8): ('IDirect3D8 *', 'pD3D', 4, ''), ('CDx', 12): ('IDirect3DDevice8 *', 'pDevice', 4, ''),
                ('D3DHolder', 4): ('IDirect3D8 *', 'pD3D', 4, ''), ('D3DHolder', 8): ('IDirect3DDevice8 *', 'pDevice', 4, '')}
# fields the user named in the IDB's own structs (CMoProcessor, CXiSkeletonActor, CXiSkeletonActorRes)
_named = json.load(open(os.path.join(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data'), 'ida_named_fields.json')))
for _c, _d in _named.items():
    for _off, _n, _t, _w in _d['members']:
        if _off == 0 or _n.startswith(('pad_', 'CMoTask_base')): continue
        _m = re.match(r'^(.*?)(\[\d+\])$', _t)
        KNOWN_FIELDS[(_c, _off)] = ((_m.group(1) if _m else _t).replace('void *', 'void *'), _n, _w, _m.group(2) if _m else '')
VAL = json.load(open(os.path.join(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data'), 'd3d_values.json')))
# names for inferred fields (data/field_names.json): only the identifier changes, never the type or offset
FIELD_NAMES = {(c, int(off, 16)): v for c, d in json.load(open(os.path.join(
    os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'field_names.json'))).items() for off, v in d.items()}

def ident(s): return re.sub(r'\W', '_', s)

# ------------------------------------------------------------------ symbol bindings
def split_params(args):
    parts, depth, cur = [], 0, ''
    for ch in args:
        if ch == ',' and depth == 0: parts.append(cur); cur = ''; continue
        depth += (ch == '(') - (ch == ')'); cur += ch
    if cur.strip(): parts.append(cur.strip())
    return [] if parts == ['void'] else parts

# Ghidra's type names, as xi_ghidra.hpp defines them, spelt in plain C++ for the shared headers (gfx_globals.hpp,
# gfx_orig.hpp), which do not include xi_ghidra.hpp (`code`, `word`, `addr` could be a Hex-Rays local's name). The
# undefinedN types are not among them: xi_ghidra_shared.hpp defines them for every translation unit. A `code *` the
# text calls (a global, hx2cpp.ghidra_global_type, or a parameter) is spelt `xi_codefn *` (gfx_common.hpp), callable.
_GHIDRA_PLAIN = {'byte': 'uint8_t', 'uchar': 'uint8_t', 'ushort': 'uint16_t', 'word': 'uint16_t',
                 'uint': 'uint32_t', 'dword': 'uint32_t', 'ulong': 'uint32_t', 'longlong': 'int64_t',
                 'ulonglong': 'uint64_t', 'qword': 'uint64_t', 'float10': 'long double', 'xi_bool': 'uint8_t',
                 'int3': 'int32_t', 'uint3': 'uint32_t', 'wchar16': 'uint16_t', 'wchar32': 'uint32_t', 'code': 'void',
                 'pointer': 'void *', 'addr': 'void *'}


CXX_KEYWORDS = {'default', 'case', 'switch', 'new', 'delete', 'this', 'class', 'struct', 'union', 'enum', 'template',
                'typename', 'operator', 'friend', 'virtual', 'public', 'private', 'protected', 'namespace', 'using',
                'register', 'auto', 'static', 'const', 'volatile', 'inline', 'extern', 'signed', 'unsigned', 'return',
                'goto', 'do', 'while', 'for', 'if', 'else', 'break', 'continue', 'sizeof', 'typedef', 'try', 'catch',
                'throw', 'bool', 'true', 'false', 'char', 'short', 'int', 'long', 'float', 'double', 'void', 'asm'}


def plain_type(t):
    return re.sub(r'\b(%s)\b' % '|'.join(sorted(_GHIDRA_PLAIN, key=len, reverse=True)), lambda m: _GHIDRA_PLAIN[m.group(1)], t)


def orig_wrapper(name, ea, ftype):
    ftype = plain_type(ftype)
    ftype = re.sub(r'^([^()]*)\(\s*(__\w+\s*)?\*\s*\)\s*\(', r'\1 \2(', ftype)   # IDA's `R (*)(args)`: the function type
    ftype = hx2cpp.fnptr_return(ftype)
    m = re.match(r'^(.*?)\s*' + CC_RE + r'?\s*\((.*)\)\s*$', ftype, re.S)
    if not m or re.search(r'__user|@<', ftype): return None
    ret, cc, args = m.groups(); cc = cc or '__cdecl'
    if '(' in ret or ')' in ret: return None       # a return type not parsed: no wrapper rather than a wrong one
    if cc.strip() == '__cdecl' and hx2cpp.PURGES.get('0x%08X' % ea): cc = '__stdcall'   # pops (`ret N`): D3DX dispatch thunks
    ret = re.sub(r'\b__noreturn\b|' + CC_RE, '', ret).strip() or 'int'    # `void __cdecl __noreturn`
    # MSVC code returns a bool in AL and its callers test AL against zero; GCC assumes a `bool` is exactly 0 or 1
    # and may fold it into other logic (CXiSkeletonActor 0x100CBDE0: `ModelTest(...) || !v42` with AL = 2 went
    # the other way). The game's bool is read as the raw byte.
    if ret == 'bool': ret = 'uint8_t'
    TYPEWORDS = {'int', 'char', 'short', 'long', 'float', 'double', 'bool', 'void', 'unsigned', 'signed',
                 '__int8', '__int16', '__int32', '__int64'}
    ps = []
    for p in split_params(args):
        if '*' in p or '[' in p or re.match(r'^(const\s+)?LP[A-Z]\w*\b', p.strip()): ps.append('void *'); continue
        toks = p.split()
        if len(toks) >= 2 and toks[-2] not in ('unsigned', 'signed', 'struct', 'enum', 'const', 'class') \
           and toks[-1] not in TYPEWORDS:
            toks = toks[:-1]          # a parameter name
        ps.append(' '.join(toks) or 'int')
    variadic = bool(ps) and ps[-1].strip() == '...'
    if variadic: ps = ps[:-1]
    names = ['a%d' % i for i in range(len(ps))]
    fp = '%s (%s *)(%s)' % (ret, cc, ', '.join(ps + (['...'] if variadic else [])))
    if variadic and all(p.strip() == 'XI_ANY' for p in ps):
        # the Ghidra route's variadic callee (hx2cpp.ghidra_ptr_type): each argument one 4-byte slot, as many as the
        # call site passes (xi_ghidra.hpp xi_va_call)
        fp = '%s (%s *)(%s)' % (ret, cc, ', '.join(ps + ['decltype(XI_ANY(rest))...']))
        body = 'return reinterpret_cast<%s>(XI_GAME(0x%08Xu))(%s);' % (fp, ea, ', '.join(names + ['XI_ANY(rest)...']))
    elif variadic:
        body = 'return reinterpret_cast<%s>(XI_GAME(0x%08Xu))(%s);' % (fp, ea, ', '.join(names + ['rest...']))
    if variadic:
        if ret == 'void': body = body.replace('return ', '')
        return ('template <class... R> XI_ORIG %s %s(%s) { %s }' % (
            ret, name, ', '.join(['%s %s' % (p, n) for p, n in zip(ps, names)] + ['R... rest']), body))
    body = 'return reinterpret_cast<%s>(XI_GAME(0x%08Xu))(%s);' % (fp, ea, ', '.join(names))
    if ret == 'void': body = body.replace('return ', '')
    return ('XI_ORIG %s %s(%s) { %s }' % (ret, name, ', '.join('%s %s' % (p, n) for p, n in zip(ps, names)), body))

# ------------------------------------------------------------------ text rewriting
def strip_noise(body):
    body = re.sub(r'\s*//\s*(e?[abcd]x|e?[sd]i|e?bp|e?sp|[abcd][lh]|st\d?|\[esp[^\n]*|\[ebp[^\n]*)\s*$', '', body, flags=re.M)
    return body

_RL = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'reloc_literals.json')
RELOC_LIT = {int(k, 16): {int(v, 16) for v in vs} for k, vs in json.load(open(_RL)).items() if not k.startswith('_')} \
    if os.path.exists(_RL) else {}


def reloc_literals(ea, body):
    """an integer literal whose value the function's own code holds only as relocated immediates (an address,
    data/reloc_literals.json) -> XI_GAME(0x...u), cast back to the literal's own type (int, or unsigned int
    with a u suffix), so the verified build compiles the same constant and the in-game build rebases it"""
    vals = RELOC_LIT.get(ea)
    if not vals: return body, 0
    n = [0]
    def sub(m):
        lit, suf = m.group(1), m.group(2) or ''
        if len(lit) > 1 and lit[0] == '0' and lit[1] not in 'xX': return m.group(0)       # octal: not an address
        v = int(lit, 0)
        if v not in vals: return m.group(0)
        n[0] += 1
        return '(%s)XI_GAME(0x%08Xu)' % ('unsigned int' if suf else 'int', v)
    body = re.sub(r'(?<![\w.])(0[xX][0-9A-Fa-f]+|\d+)([uU])?(?![\w.])', sub, body)
    return body, n[0]


def name_d3d_values(body):
    def rs(m):
        state, val = m.group(2), m.group(3)
        v = int(val.rstrip('uU'), 0)
        if state in VAL['boolrs'] and v in (0, 1): nv = 'TRUE' if v else 'FALSE'
        elif state in VAL['rs']: nv = VAL['enums'][VAL['rs'][state]].get(str(v))
        else: nv = None
        return m.group(1) + state + ', ' + (nv or val) + ')'
    body = re.sub(r'(SetRenderState\()(D3DRS_\w+), (\d+u?)\)', rs, body)
    def tss(m):
        state, val = m.group(3), m.group(4)
        v = int(val.rstrip('uU'), 0)
        if state in VAL['tss']: nv = VAL['enums'][VAL['tss'][state]].get(str(v))
        elif re.match(r'D3DTSS_(COLOR|ALPHA)ARG', state) and (v & 0xF) in range(6):
            nv = VAL['ta'][str(v & 0xF)] + (' | D3DTA_COMPLEMENT' if v & 0x10 else '') + (' | D3DTA_ALPHAREPLICATE' if v & 0x20 else '')
            if v & ~0x3F: nv = None
        else: nv = None
        return m.group(1) + m.group(2) + ', ' + state + ', ' + (nv or val) + ')'
    body = re.sub(r'(SetTextureStageState\()([^,()]+), (D3DTSS_\w+), (\d+u?)\)', tss, body)
    body = re.sub(r'(Draw(?:Indexed)?Primitive(?:UP)?\()(\d+)u?,', lambda m: m.group(1) + (VAL['enums']['D3DPRIMITIVETYPE'].get(m.group(2)) or m.group(2)) + ',', body)
    return body

EXPR = r'[\w]+(?:(?:->|\.)[\w]+|\[\w+\])*'
def com_calls(body):
    return re.sub(r'(' + EXPR + r')->lpVtbl->(\w+)\(\1(?:, )?', lambda m: '%s->%s(' % (m.group(1), m.group(2)), body)

# ------------------------------------------------------------------ fields
_TY = '|'.join(map(re.escape, sorted(WIDTH, key=len, reverse=True)))
ACCESS = [
    (re.compile(r'\*\(\((' + _TY + r') \*\*+\)self \+ (\d+)\)'), 'ptr'),
    (re.compile(r'\*\(\((' + '|'.join(map(re.escape, WIDTH)) + r') \*\)self \+ (\d+)\)'), True),
    (re.compile(r'\*\((' + '|'.join(map(re.escape, WIDTH)) + r') \*\)\(self \+ (\d+)\)'), 'bytes'),
    (re.compile(r'\*\((' + '|'.join(map(re.escape, WIDTH)) + r') \*\)self(?![\w\s]*\+)'), 'zero'),
]
def self_elem(sig_self_type):
    t = sig_self_type.replace('*', '').strip()
    return WIDTH.get(t, 1) if sig_self_type.strip().endswith('*') else 1

def find_accesses(body, self_type):
    """[(match_span, offset, type)]"""
    out = []
    for rx, mode in ACCESS:
        for m in rx.finditer(body):
            t = m.group(1)
            if mode == 'ptr':
                stars = m.group(0)[1:].split(')self')[0].count('*') - 1   # the cast's stars minus the dereference
                t = t + ' ' + '*' * stars; off = int(m.group(2)) * 4
            elif mode is True: off = int(m.group(2)) * WIDTH[t]
            elif mode == 'bytes': off = int(m.group(2)) * self_elem(self_type)
            else: off = 0
            out.append((m.span(), off, t))
    # *(self + N) / self[N] with a typed self pointer
    st = self_type.replace('*', '').strip(); nst = self_type.count('*')
    if self_type.strip().endswith('*') and st in WIDTH:
        et = st if nst == 1 else st + ' ' + '*' * (nst - 1)
        ew = WIDTH[st] if nst == 1 else 4
        for m in re.finditer(r'\*\(self \+ (\d+)\)|(?<![\w.>])self\[(\d+)\]', body):
            n = int(m.group(1) or m.group(2)); out.append((m.span(), n * ew, et))
    for m in re.finditer(r'self->_pad_([0-9A-F]+)\[(\d+)\]', body):
        out.append((m.span(), int(m.group(1), 16) + int(m.group(2)), '_BYTE'))
    out.sort()
    res, last = [], -1
    for (a, b), off, t in out:           # drop overlapping matches
        if a >= last: res.append(((a, b), off, t)); last = b
    return res

class Layout:
    def __init__(self, cls, size):
        self.cls, self.size = cls, size
        self.seen = collections.defaultdict(collections.Counter)
    def add(self, off, t):
        if t.endswith('*'): WIDTH.setdefault(t, 4); FTYPE.setdefault(t, t)
        self.seen[off][t] += 1
    def fields(self):
        chosen, end = {}, -1
        for off in sorted(self.seen):
            t = self.seen[off].most_common(1)[0][0]
            w = WIDTH[t]
            if off < end or (self.size and off + w > self.size) or off < 4: continue
            chosen[off] = t; end = off + w
        known = {off: v for (c, off), v in KNOWN_FIELDS.items() if c == self.cls}
        for off in list(chosen):     # inferred fields never overlap a named one
            w = WIDTH.get(chosen[off], 4)
            if any(k <= off < k + v[2] or off <= k < off + w for k, v in known.items()): del chosen[off]
        for off, (t, n, w, arr) in known.items(): chosen[off] = ('KNOWN', t, n, w, arr)
        return chosen

def field_name(cls, off, t):
    k = KNOWN_FIELDS.get((cls, off))
    if k: return k[1]
    n = FIELD_NAMES[(cls, off)]['name'] if (cls, off) in FIELD_NAMES else None
    if n and n in RESERVED:            # a global or game function of that name is a macro/function in scope
        print('field name %s::%s collides with a global; using field_%X' % (cls, n, off), file=sys.stderr); n = None
    return n or 'field_%X' % off
RESERVED = set()
def _ptrwidth(t): return 4 if t.endswith('*') else WIDTH.get(t, 4)

def rewrite_members(body, self_type, cls, layout_fields):
    acc = find_accesses(body, self_type)
    out, pos = [], 0
    for (a, b), off, t in acc:
        f = layout_fields.get(off)
        out.append(body[pos:a])
        KIND = {'float': 'f', 'double': 'f'}
        def kind(x):
            x = x.strip()
            if x in KIND: return KIND[x]
            if x.endswith('*'):
                base = x.rstrip('* ').replace('const ', '').strip()
                # a pointer to a class is only interchangeable with the same class pointer: replacing
                # `_DWORD *` with `IDirect3DDevice8 *` changes what `*p + n` means
                return 'p' if base in WIDTH or base in ('void', 'char', 'unsigned char') else 'c:' + base
            return 'i'
        def same_kind(ft, at_):
            return kind(ft) == kind(at_)
        if isinstance(f, tuple):
            # a named field replaces an access only at the same width and kind (a float read as
            # raw bits, or one byte of a dword, stays a raw access)
            if f[4] == '' and WIDTH.get(t, 4) == f[3] and same_kind(f[1].strip(), t):
                out.append('this->' + f[2])
            else:
                out.append('at<%s>(this, 0x%X)' % (FTYPE.get(t, t), off))
        elif f is not None and f == t:
            out.append('this->' + field_name(cls, off, t))
        elif f is not None and WIDTH.get(f) == WIDTH.get(t) and FTYPE.get(f) == FTYPE.get(t):
            out.append('this->' + field_name(cls, off, t))
        else:
            out.append('at<%s>(this, 0x%X)' % (FTYPE.get(t, t), off))
        pos = b
    out.append(body[pos:])
    body = ''.join(out)
    for (c, off), v in KNOWN_FIELDS.items():
        if c == cls: body = re.sub(r'\bself->%s\b' % re.escape(v[1]), 'this->' + v[1], body)
    return body

# IDA's placeholder names, and Ghidra's (FUN_, DAT_, LAB_, PTR_, ... with lower-case hex): never the canonical name
PLACEHOLDER = re.compile(r'^(?:_?(sub|dword|word|byte|flt|dbl|qword|unk|off|stru)_[0-9A-F]{8}'
                         r'|(FUN|DAT|LAB|PTR|thunk_FUN|switchD|caseD|s|u|_DAT|BYTE|WORD|DWORD|QWORD)_[0-9a-fA-F_]+)$')


def declared_names(r):
    """the names a record declares: its parameters, and the locals Hex-Rays declares at the top of the body
    (the lines before the first blank line, each ending in its location comment: `; // eax`, `; // [esp+..]`)"""
    head, _, body = r['text'].partition('\n{')
    sig = ' '.join(l for l in head.splitlines() if not l.startswith('//'))
    params = sig[sig.find('(') + 1:sig.rfind(')')]
    names = {m.group(1) for p in params.split(',') for m in [re.search(r'(\w+)\s*(\[[^\]]*\])*\s*$', p)] if m}
    decls = body.lstrip('\n').split('\n\n', 1)[0]
    names |= set(re.findall(r'^\s+[^;=()]*?[\s\*&](\w+)(?:\[[^\]]*\])*; // ', decls, re.M))
    return names

def ps2_names():
    """{ea: name} from data/ps2_matches.json (ps2_match.py: each PC function matched to the PS2 build's named one, with its
    evidence and DancingMad's confidence word), spelt Class_Method. A PS2 class whose name with a leading C is a PC
    runtime-class descriptor's name (XiActor: the PC's CXiActor) is spelt as the PC names it."""
    path = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'ps2_matches.json')
    if not os.path.exists(path): return {}
    m = json.load(open(path)); pcls = set(m.get('_pc_classes', []))
    out = {}
    for k, v in m.items():
        if k.startswith('_'): continue
        cls = v.get('ps2_class')
        if cls and 'C' + cls in pcls: cls = 'C' + cls
        nm = ('%s_%s' % (cls.replace('::', '_'), v['ps2_method'])) if cls else v['ps2_method']
        if re.match(r'^[A-Za-z_]\w*$', nm): out[int(k, 16)] = nm
    return out


def canonicalise_names(R, pending):
    # A record exported before an IDB rename still prints the old name, so one address can carry
    # two names across records. Every referenced object gets one name: the reviewed name from
    # names_pending_idb.json (not yet in the IDB), else the IDB's real name, else the placeholder.
    # Only references are renamed; a record's own name drives class placement and is left alone.
    # the IDB's names are the IDA records' (a Ghidra record prints the names of the Ghidra project, which can lag
    # or differ): a name an IDA record uses wins, the Ghidra records' names count only where no IDA record has one
    seen, seen_ida = collections.defaultdict(collections.Counter), collections.defaultdict(collections.Counter)
    for r in R.values():
        for nm, o in r['objs'].items():
            if not PLACEHOLDER.match(nm) and re.match(r'^[A-Za-z_]\w*$', nm):     # a name C++ can print
                seen[o['ea']][nm] += 1
                if not r.get('ghidra'): seen_ida[o['ea']][nm] += 1
    # the most common name; a tie goes to the first in sort order (a Counter's order is the order records were read)
    top = lambda c: sorted(c.items(), key=lambda kv: (-kv[1], kv[0]))[0][0]
    canon = {ea: top(seen_ida.get(ea) or c) for ea, c in seen.items()}
    for ea, nm in PS2_NAMES.items():                # the PS2 build's name, where the IDB has none
        canon.setdefault(ea, nm)
    canon.update({int(k, 16): v['name'] for k, v in pending.items()})
    local = {'self', 'this'} | set().union(*(declared_names(r) for r in R.values()))
    for r in R.values():
        for nm, o in list(r['objs'].items()):
            if nm in local and not o['func']:
                # a global is a macro in the tree, so one named like a parameter or local of any function
                # (the IDB has globals named `self` and `C`; Hex-Rays writes them `::self`, `::C` where a
                # parameter shadows them) would replace that identifier everywhere: it gets a distinct name
                new = 'g_%s_%08X' % (nm, o['ea'])
                head, _, body = r['text'].partition('\n{')
                if nm in declared_names(r):                     # shadowed here: only `::name` is the global
                    r['text'] = re.sub(r'(?<![\w:])::%s\b' % re.escape(nm), new, r['text'])
                else:
                    r['text'] = head + '\n{' + re.sub(r'(?<![\w:])(?:::)?\b%s\b(?!::)' % re.escape(nm), new, body)
                o['printed'] = [new]; r['objs'][new] = r['objs'].pop(nm)
                continue
            # a recursive call names the function itself: it takes the record's own name (DancingMad's, reviewed)
            new = r['name'] if o['ea'] == r['ea'] else canon.get(o['ea'])
            if not new or new == nm or not re.match(r'^[A-Za-z_]\w*$', new): continue
            if new in r['objs'] and r['objs'][new]['ea'] != o['ea']: continue   # would collide
            # the text prints the object some other way (Hex-Rays writes `??0X@@QAE@XZ` as `X::X`): renaming it
            # would part the reference from its record; the translation names it as before
            if not re.search(r'\b%s\b' % re.escape(nm), r['text']): continue
            r['text'] = re.sub(r'\b%s\b' % re.escape(nm), lambda _m: new, r['text'])
            o['printed'] = [new if p == nm else p for p in o.get('printed', [])]
            r['objs'].setdefault(new, o); del r['objs'][nm]

def reviewed_names():
    """{address: entry} of this repository's reviewed names: data/names_pending_idb.json, and each batch's file under
    data/names/ (pipeline/batch.py; one file per batch, so parallel work does not collide). An address named twice
    differently in one round is an error to resolve, not a choice to make here; a later round's file (rN_..., the
    batches of batches/roundN/) took the earlier name and its dossier again, and its entry replaces the earlier one."""
    d = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
    out, src, rnd = {}, {}, {}
    rnd_of = lambda p: int(re.match(r'r(\d+)_', os.path.basename(p)).group(1)) if re.match(r'r\d+_', os.path.basename(p)) else 1
    for p in [os.path.join(d, 'names_pending_idb.json')] + sorted(glob.glob(os.path.join(d, 'names', '*.json')), key=lambda p: (rnd_of(p), p)):
        for k, v in json.load(open(p)).items():
            if k.startswith('_'): continue
            k = '0x%08X' % int(k, 16)
            if k in out and out[k]['name'] != v['name'] and rnd_of(p) > rnd.get(k, 1):
                out[k] = dict(v, replaces=out[k]['name']); src[k] = os.path.basename(p); rnd[k] = rnd_of(p); continue
            if k in out and out[k]['name'] != v['name']:
                raise SystemExit('%s named %r in %s and %r in %s' % (k, out[k]['name'], src[k], v['name'], p))
            out[k] = v; src[k] = os.path.basename(p); rnd[k] = rnd_of(p)
    return out


# ------------------------------------------------------------------ main
def main(exp, gate, manf, incdir, outdir):
    R = {}
    PS2_NAMES.update(ps2_names()); REVIEWED.update(reviewed_names())
    _dm = json.load(open(os.path.join(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data'), 'dm_names.json')))
    # EXP and GATE may each be a comma-separated list: the IDA export first, then Ghidra exports (GfxExport.java) of
    # functions it does not hold; a later record of an address already read is ignored
    # each export's own gate (GATE in the same order as EXP): an address held by several exports takes the record of
    # the first export whose gate passed it (else the first record), so a function passed only by the Ghidra route
    # is not built from an IDA record its gate did not pass
    gates = gate.split(',')
    passed_in = []
    for gf in gates:
        ps = set()
        for l in open(gf):
            g = json.loads(l)
            if g.get('verdict') == 'PASS': ps.add(int(g['original'], 16))
        passed_in.append(ps)
    # raw verdicts given again under the gate's current rules (XI_REGATE, tree/source.json `raw_regate`: lines with
    # `export`, the basename of the export the record comes from; the IDA export is gfx_export.jsonl): each replaces
    # that export's earlier verdict for the function, which was given under rules since changed
    if os.environ.get('XI_REGATE'):
        names = [os.path.basename(e) for e in exp.split(',')]
        for l in open(os.environ['XI_REGATE']):
            g = json.loads(l); a = int(g['original'], 16)
            k = next((n for n, b in enumerate(names) if b == g['export'] or (n == 0 and g['export'] == 'gfx_export.jsonl')), None)
            if k is None or k >= len(passed_in): continue
            if g.get('verdict') == 'PASS': passed_in[k].add(a)
            else: passed_in[k].discard(a)
    recs, first = [], {}
    for i, e in enumerate(exp.split(',')):
        for l in open(e):
            r = json.loads(re.sub(r"\s*__spoils<[^>]*>", "", l))   # callee-preserves-register notes: Hex-Rays only
            r['_exp'] = i                               # which export (its order: the IDA export first)
            recs.append(r); first.setdefault(r['ea'], (i, len(recs) - 1))
            if i < len(passed_in) and r['ea'] in passed_in[i] and not (first[r['ea']][0] < len(passed_in)
                                                                         and r['ea'] in passed_in[first[r['ea']][0]]):
                first[r['ea']] = (i, len(recs) - 1)
    chosen = {k for _, k in first.values()}
    allrecs, recs = recs, [r for k, r in enumerate(recs) if k in chosen]
    # the IDB's names of functions the IDA export holds only as callees (its records' objects)
    idb_fn = {o['ea']: nm for r in allrecs if r.get('source') != 'ghidra' for nm, o in (r.get('objs') or {}).items()
              if o.get('func') and re.match(r'^[A-Za-z_]\w*$', nm) and not nm.startswith(('sub_', 'j_sub_', 'nullsub_'))}
    for r in allrecs:                               # and of the functions it holds as records itself
        if r.get('source') != 'ghidra' and re.match(r'^[A-Za-z_]\w*$', r['name']) and not r['name'].startswith(('sub_', 'j_sub_', 'nullsub_')):
            idb_fn.setdefault(r['ea'], r['name'])
    for r in recs:
        if r['ea'] in R: continue
        if r.get('source') != 'ghidra':     # corrections to Hex-Rays' text and IDA's prototypes (hx2cpp the same)
            r = hx_patches.apply(r)
        if r.get('source') == 'ghidra':
            # the same preparation as the raw translation (hx2cpp.ghidra_prepare); its names: the IDB's, else fn_
            r = hx_patches.patch_text(r, quiet=True)          # only a patch written against this Ghidra text
            _exp = r['_exp']; r, status, why = hx2cpp.ghidra_prepare(r)
            if r is None: continue
            r = dict(r, objs=hx2cpp.ghidra_auto_objs(r, r['text'].partition('\n{')[2]),
                     name=idb_fn.get(r['ea'], 'sub_%X' % r['ea']), ghidra=True, _exp=_exp)
            r['text'] = re.sub(r'\bFUN_%08x\b' % r['ea'], r['name'], r['text'])
        _gname0 = r['name']
        dmn = _dm.get(hex(r['ea']))
        if dmn and PLACEHOLDER.match(r['name']) and re.match(r'^[A-Za-z_]\w*$', dmn[0]):
            r['name'] = dmn[0]                       # DancingMad's name (data/dm_names.json, pipeline/dm_names.py)
        rv = REVIEWED.get('0x%08X' % r['ea'])
        if rv and PLACEHOLDER.match(r['name']) and re.match(r'^[A-Za-z_]\w*$', rv['name']):
            r['name'] = rv['name']                   # this repository's reviewed name (names_pending_idb, data/names/)
        if PLACEHOLDER.match(r['name']) and r['ea'] in PS2_NAMES:
            r['name'] = PS2_NAMES[r['ea']]           # the PS2 build's name (ps2_match.py), where none other is known
        if r.get('ghidra') and r['name'] != _gname0 and re.match(r'^[A-Za-z_]\w*$', r['name']):
            # a Ghidra record's recursive calls were written with its first name (above): they take the new one
            r['text'] = re.sub(r'\b%s\b' % re.escape(_gname0), r['name'], r['text'])
        R[r['ea']] = r
    canonicalise_names(R, REVIEWED)
    passed = set().union(*passed_in)                 # passed by some export's gate (as given again, XI_REGATE)
    def reg_callee(o):
        m = re.search(CC_RE, o.get('type') or '')
        return bool(o.get('storage')) and bool(hx2cpp.extra_regs(m.group(0) if m else '__cdecl', o['storage']))
    def ftype_of(ea, o, nm=''):
        # a Ghidra record's callee: typed as the raw translation types it (hx2cpp.ghidra_ptr_type: `this` first for a
        # __thiscall, the stack slots the callee's `ret N` or the call sites give, each an XI_ANY)
        if not R[ea].get('ghidra'): return o['type'].strip()
        body = R[ea]['text'].partition('\n{')[2]
        # the call sites as the text prints them: the object's names and the callee record's own name
        names = {nm} | set(o.get('printed') or []) | ({R[o['ea']]['name']} if o['ea'] in R else set())
        pts = [hx2cpp.ghidra_ptr_type(n, o, body) for n in sorted(names)]
        pt = max(pts, key=lambda t: (t.count(','), 'XI_ANY' in t))
        pt = re.sub(r'\((__\w+) \*\)', r'\1', pt, count=1)
        if reg_callee(o):            # takes registers its convention does not pass: called as the raw translation calls it
            pt += ' /*regs:%s*/' % ','.join(o['storage'])
        return pt
    # a game function the tree calls through a wrapper (gfx_orig.hpp) gets only what the wrapper's convention passes:
    # ECX for __thiscall/__fastcall, EDX for __fastcall, nothing else. A caller of a game function whose own code reads
    # another entry register (data/ecx_use.json, edx_use.json, entry_regs.json: an instruction named, not a read
    # through a callee) hands it whatever the compiled C++ left there, which the gate does not see (it compares the
    # stubbed call's arguments): such a caller is left out, and the game's own code runs in its place (verify_all.py
    # step 2b checks the same rule on the tree). Every call goes through a wrapper (orig::, the game's code), the
    # callee's reconstruction in the tree included; a __ftol-like callee (XI_CALL_ST0) does not.
    _D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
    _eu, _du, _er = (json.load(open(os.path.join(_D, f))) for f in ('ecx_use.json', 'edx_use.json', 'entry_regs.json'))
    _ei = json.load(open(os.path.join(_D, 'eax_inputs.json')))
    _kept = {(int(v['caller'], 16), int(v['callee'], 16)) for v in json.load(open(os.path.join(_D, 'eax_kept_sites.json'))).values()}
    # a kept-EAX call the gate runs as the game does (gate_sees_eax.py) is the gate's to judge, not left out
    import gate_sees_eax
    _cases = gate_sees_eax.load_cases(os.path.join(_D, '..', 'verification', 'cases_raw_final.json'))
    def wrapper_bad(cea, fea, ft):
        """the registers game function fea needs from caller cea that a wrapper typed ft does not pass (a list,
        empty if none). Needs: its own code reads ECX / EDX / another register at entry (ecx_use, edx_use,
        entry_regs: an instruction named), reads its entry EAX (eax_inputs.json), or hands back EAX bytes this
        caller reads after the call (eax_kept_sites.json)"""
        mr = re.search(r'/\*regs:([\w,]+)\*/', ft)
        if mr: cc = re.search(CC_RE, ft).group(0) if re.search(CC_RE, ft) else '__cdecl'
        else:
            w = orig_wrapper('f', fea, ft)
            m = w and re.search(r'reinterpret_cast<[^(]*\((__\w+) \*\)', w)
            if not m: return []
            cc = m.group(1)
        given = set(hx2cpp.CC_REGS.get(cc, [])) | (set(mr.group(1).split(',')) - {'stack'} if mr else set())
        k = '0x%08X' % fea
        direct = lambda v: isinstance(v, str) and v.startswith('0x')
        bad = ['ECX' for _ in [0] if 'ECX' not in given and direct(_eu.get(k))]
        bad += ['EDX' for _ in [0] if 'EDX' not in given and direct(_du.get(k))]
        bad += [r.upper() for r, ins in (_er.get(k) or {}).items()
                if ' push ' not in ' ' + ins[11:] + ' ' and r.upper() not in given and r.upper() != 'EAX']
        if 'EAX' not in given and (_ei.get(k) or {}).get('reads'): bad.append('EAX')
        elif 'EAX' not in given and (cea, fea) in _kept and not gate_sees_eax.seen(_cases, cea, fea): bad.append('EAX')
        return bad
    left_out = {}
    # functions an earlier build of this tree could not compile (regen_clean.sh, drop_failed.py: {ea: why}): left
    # out, so the game's own code runs in their place, each with the compiler's message
    dropped = {}
    if os.environ.get('XI_DROP') and os.path.exists(os.environ['XI_DROP']):
        dropped = {int(k, 16): v for k, v in json.load(open(os.environ['XI_DROP'])).items()}
        passed -= set(dropped)
    while True:
        drop = {}
        for ea in sorted(passed):
            for nm, o in R[ea].get('objs', {}).items():
                if not o['func'] or o.get('import'): continue
                if R[ea].get('ghidra') and hx2cpp.FPRET_ALL.get('0x%x' % o['ea']) == 'consumes': continue   # XI_CALL_ST0
                bad = wrapper_bad(ea, o['ea'], ftype_of(ea, o, nm))
                if bad: drop[ea] = '0x%08X reads %s' % (o['ea'], '/'.join(bad)); break
        if not drop: break
        passed -= set(drop); left_out.update(drop)
    os.makedirs(outdir, exist_ok=True)
    lo = {'0x%08X' % ea: '%s: calls %s, which its wrapper does not pass' % (R[ea]['name'], why) for ea, why in left_out.items()}
    lo.update({'0x%08X' % ea: '%s: does not compile in this tree: %s' % (R[ea]['name'] if ea in R else '?', why) for ea, why in dropped.items()})
    json.dump(dict(sorted(lo.items())), open(os.path.join(outdir, 'left_out.json'), 'w'), indent=0)
    man = {int(k, 16): v for k, v in json.load(open(manf)).items()}
    sizes, bases = {}, {}
    for fn in os.listdir(incdir):
        s = open(os.path.join(incdir, fn)).read()
        m = re.search(r'static_assert\(sizeof\((\w+)\) == (0x[0-9A-F]+)', s)
        if m: sizes[m.group(1)] = int(m.group(2), 16)
        m = re.search(r'class XI_API (\w+) : public (\w+)', s)
        if m: bases[m.group(1)] = m.group(2)
    sizes['CDx'] = 0xA50; sizes['D3DHolder'] = 0x478
    known = sorted(sizes, key=len, reverse=True)

    # placement
    place, nothis = {}, {}
    _units_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'units.json')
    UNITS = json.load(open(_units_path)) if os.path.exists(_units_path) else {}
    _uc = os.path.join(os.path.dirname(_units_path), 'units_callers.json')
    if os.path.exists(_uc):                         # called only from one unit and adjacent to it
        for k, v in json.load(open(_uc)).items():
            if UNITS.get(k, {}).get('kind') in (None, 'gap'): UNITS[k] = v
    _votes_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'this_votes.json')
    VOTES = json.load(open(_votes_path)) if os.path.exists(_votes_path) else {}
    _dd = os.path.dirname(_votes_path)
    TSRC = json.load(open(os.path.join(_dd, 'this_sources.json'))) if os.path.exists(os.path.join(_dd, 'this_sources.json')) else {}
    GOBJ = json.load(open(os.path.join(_dd, 'global_objects.json'))) if os.path.exists(os.path.join(_dd, 'global_objects.json')) else {}
    def global_this(ea):
        """callers pass the object held in one global (whose class its creation shows) as `this`"""
        srcs = TSRC.get(hex(ea), {}).get('sources', {})
        gl = collections.Counter()
        for s, n in srcs.items():
            m = re.match(r'^(?:load|addr) (\w+)', s)
            if m: gl[m.group(1)] += n
        if len(gl) != 1: return None
        g, n = gl.most_common(1)[0]
        rest = {s: k for s, k in srcs.items() if not s.startswith(('load', 'addr'))}
        # every other call site must pass a register (a caller's own `this`), and the global
        # must account for at least two thirds of the call sites
        if any(not s.startswith('reg ') for s in rest) or n * 3 < 2 * sum(srcs.values()): return None
        if g not in GOBJ: return None
        return GOBJ[g]['class'], 'callers pass the object in %s as this (%d of %d call sites); %s' % (
            g, n, sum(srcs.values()), GOBJ[g]['why'])
    def ancestors(c):
        out = [c]
        while c in bases: c = bases[c]; out.append(c)
        return out
    def common_base(cs):
        chains = [ancestors(c) for c in cs]
        for c in chains[0]:
            if all(c in ch for ch in chains): return c if c in sizes or c in bases else None
        return None
    for ea in sorted(passed):
        r = R[ea]; nm = r['name']; sig = r['text'].split('\n{')[0]
        cls = meth = None; why = None
        self_t = r['args'][0][1] if r['cc'] == 'thiscall' and r['args'] else None
        if ea in man and man[ea]['class']:
            cls, meth, why = man[ea]['class'], man[ea]['name'], 'tree manifest'
        elif r['cc'] == 'thiscall' and hex(ea) in VOTES and VOTES[hex(ea)]['votes'] and not VOTES[hex(ea)]['other']:
            # every caller passes its own `this` unchanged: the method belongs to the callers'
            # class, or to their nearest common base when several subclasses call it
            lca = common_base(list(VOTES[hex(ea)]['votes']))
            if lca:
                cls, meth, why = lca, (nm if not nm.startswith('sub_') else 'fn_%08X' % ea), \
                    'callers pass their own this (%s)' % ', '.join(sorted(VOTES[hex(ea)]['votes']))
        else:
            for c in known:
                if nm.startswith(c + '_') and len(nm) > len(c) + 1:
                    cls, meth, why = c, nm[len(c) + 1:], 'IDB name'; break
            if cls is None and self_t and re.match(r'^(CDx|D3DHolder) \*', self_t):
                cls, meth, why = self_t.split()[0], nm if not nm.startswith('sub_') else 'fn_%08X' % ea, 'typed this'
        nothis[ea] = None
        if cls is None and r['cc'] == 'thiscall':
            gt = global_this(ea)
            if gt: cls, meth, why = gt[0], (nm if not nm.startswith('sub_') else 'fn_%08X' % ea), gt[1]
        if cls and r['cc'] != 'thiscall':
            if why == 'tree manifest' and r['cc'] == 'stdcall':
                # a method from the class's vtable whose body never reads ECX: IDA types it stdcall.
                # A thiscall member that ignores `this` pops the same stack arguments, so it is the
                # same ABI -- it stays a member (static when the class model says so)
                nothis[ea] = 'static' if man[ea].get('static') else 'member'
                why = 'tree manifest; does not use this'
            elif why == 'IDB name':
                # named for its class but takes no `this`: a static member (same ABI as a free
                # function; the class only scopes the name). Accessors like
                # CMoTask_GetRuntimeClass_variantN are inline copies emitted in other units
                nothis[ea] = 'static'
                why = 'IDB name; static (takes no this)'
            else:
                cls = None                          # free helpers stay free (placed by unit below)
        if r.get('ghidra') and hx2cpp.extra_regs(r.get('cc'), r.get('storage')):
            # its entry takes a register its convention does not pass (GfxExport's register input): a free function
            # with GCC's regparm signature (hx2cpp.regparm_entry), the original's ABI; a member function cannot be
            cls = meth = None; why = 'takes %s in a register: a free function' % ', '.join(x for _, x in hx2cpp.extra_regs(r.get('cc'), r.get('storage')))
        elif r.get('ghidra') and hx2cpp.undefined_reg_params(r.get('cc'), _head_params(r['text'])):
            # a register parameter typed undefined4 goes on the stack in GCC unless written with regparm, as the raw
            # translation does (hx2cpp.undefined_reg_params); a member function cannot be
            cls = meth = None; why = 'an undefined4 register parameter: a free function with regparm'
        place[ea] = (cls, ident(meth) if meth else None, why)

    # accesses to members of IDB structs the tree replaces -> the offsets they access (idb_access.py)
    import idb_access
    replaced = idb_access.replaced_set({c for c, _, _ in place.values() if c})
    idb_stats = collections.Counter()
    all_globals = {nm for r in R.values() for nm, o in (r.get('objs') or {}).items() if not o.get('func')}
    for ea in place:
        r = R[ea]; head, sep, body = r['text'].partition('\n{')
        # the repairs that read a vtable call through `obj->vftable + N` run first, while that text is still
        # there (both are idempotent; body_rules applies them again later)
        body = hx2cpp.float_args_in_casts(hx2cpp.vmethod_this(body))
        this_name = r['args'][0][0] if r.get('args') else None
        this_forms = bool(place[ea][0]) and not nothis.get(ea)
        body, n, nl = idb_access.rewrite(r, sep + body, replaced, this_name, this_forms, WIDTH, all_globals)
        r['text'] = head + body; idb_stats['rewritten'] += n; idb_stats['left'] += nl
    print('IDB struct accesses: %d chains on %d replaced structs rewritten to offsets, %d left' % (
        idb_stats['rewritten'], len(replaced), idb_stats['left']))
    # sizes the IDB's own named structs establish (larger than a descriptor-only record)
    size_conflicts = {}
    for c, d in _named.items():
        if sizes.get(c, 0) < d['size']:
            size_conflicts[c] = 'class descriptor says 0x%X; the IDB struct and the code use 0x%X' % (sizes.get(c, 0), d['size'])
            sizes[c] = d['size']
    # layouts
    layouts = {}
    for ea, (cls, meth, why) in place.items():
        if not cls: continue
        r = R[ea]; L = layouts.setdefault(cls, Layout(cls, sizes.get(cls)))
        if nothis.get(ea): continue                 # no `this` accesses to learn a layout from
        body = r['text'].split('\n{', 1)[1]
        body = re.sub(r'\bthis\b', 'self', body)
        p0 = r['args'][0][0] if r['args'] else 'self'
        if p0 and p0 not in ('self', 'this'): body = re.sub(r'\b%s\b' % re.escape(p0), 'self', body)
        for _, off, t in find_accesses(body, r['args'][0][1]): L.add(off, t)

    os.makedirs(os.path.join(outdir, 'include'), exist_ok=True)
    os.makedirs(os.path.join(outdir, 'src'), exist_ok=True)
    # symbol bindings for everything referenced
    origs, globs, gtypes, gtier = {}, {}, {}, {}
    _gfrom = {}
    def gset(gea, v, t):
        """a global's name: from a record of the first export that uses it (then the first name in sort order)"""
        if gea not in globs or (t, v[0]) < _gfrom[gea]: globs[gea] = v; _gfrom[gea] = (t, v[0])
    fwrap, voidv = {}, {}
    def void_float_calls(ea, body):
        # a float callee whose result this caller never reads (hx2cpp.unconsumed_fp_calls): the call
        # goes through a void-returning wrapper so neither side touches the x87 stack after it
        used = set(hx2cpp.FP_SITES.get('0x%08X' % ea, ())) if hx2cpp.FP_SITES is not None else set()
        for nm, (cea, ctype) in fwrap.items():
            k = '0x%08X' % cea
            if not ((k in hx2cpp.FPRET_FNS and k not in used) or hx2cpp.FPRET_ALL.get('0x%x' % cea) == 'none'): continue
            calls = len(re.findall(r'(?m)^\s*orig::%s\(' % re.escape(nm), body))
            total = len(re.findall(r'\borig::%s\b' % re.escape(nm), body))
            none = hx2cpp.FPRET_ALL.get('0x%x' % cea) == 'none'
            if total and (calls == total or none):
                if calls != total:          # proven none: a "result" used in an expression never existed
                    body = hx2cpp.wrap_calls(body, 'orig::' + nm, '(%s, 0.0)')
                body = re.sub(r'\borig::%s\(' % re.escape(nm), 'orig_void::%s(' % nm, body)
                voidv[nm] = (cea, ctype)
        return body
    def gtype_of(o, r, nm):
        # a Ghidra record's global as its raw translation declares it (hx2cpp.ghidra_global_type)
        t = hx2cpp.ghidra_global_type(nm, o, r['text']) if r.get('ghidra') else 'char[1]' if o.get('is_str') else o['type']
        if t.strip() == 'code *': t = 'xi_codefn *'
        if re.sub(r'^(struct\s+)?(std::)?', '', t.strip()) in ('Iostream_init', 'exception', 'type_info', 'codecvt_base'): t = 'char[1]'
        return t.strip()
    SUFFIX = {'_BYTE': 'u8', 'unsigned __int8': 'u8', 'char': 's8', '__int8': 's8', '_WORD': 'u16', 'unsigned __int16': 'u16',
              '__int16': 's16', '_DWORD': 'u32', 'unsigned int': 'u32', 'int': 's32', 'float': 'f32', 'double': 'f64'}
    # a name two game functions carry (operator_delete: 0x101907D0 and 0x10311BB0) is spelt with its address
    _name_eas = collections.defaultdict(set)
    for ea in passed:
        for nm, o in R[ea].get('objs', {}).items():
            if o['func']: _name_eas[nm].add(o['ea'])
    # records can type one callee differently (IDA's prototype in one, the Ghidra route's in another): each was gated
    # with its own, so each prototype gets its own wrapper; the most common one the plain name, the others _vN
    def obase(nm, fea):
        return 'fn_%08X' % fea if not re.match(r'^[A-Za-z_]\w*$', nm) or nm in CXX_KEYWORDS else \
            nm if len(_name_eas.get(nm, ())) <= 1 else '%s_%08X' % (nm, fea)
    _ftypes, _ftier = collections.defaultdict(collections.Counter), collections.defaultdict(dict)
    for ea in passed:
        for nm, o in R[ea].get('objs', {}).items():
            if o['func'] and not o.get('import'):
                _k, _t = (o['ea'], obase(nm, o['ea'])), ftype_of(ea, o, nm)
                _ftypes[_k][_t] += 1
                _ftier[_k][_t] = min(_ftier[_k].get(_t, 99), R[ea].get('_exp', 0))
    # numbered among the prototypes one name carries (IDA's sub_X and Ghidra's FUN_x are different names of one
    # address), those of the first export that uses it first, so a name's wrappers keep their spelling when a
    # later export's records are added
    _fvar = {}
    for kb, cnt in _ftypes.items():
        for k, t in enumerate(sorted(cnt, key=lambda t: (_ftier[kb][t], -cnt[t], t))): _fvar[kb + (t,)] = k
    def oname(nm, fea, ftype=None):
        base = obase(nm, fea)
        k = _fvar.get((fea, base, ftype.strip())) if ftype else 0
        return base if not k else '%s_v%d' % (base, k)
    for ea in passed:
        for nm, o in R[ea].get('objs', {}).items():
            if o['func'] and o.get('import'):
                # a Ghidra import: its address is the import table slot, which holds the function's address; bound
                # as a pointer at the slot (as the IDA records bind it), called through it
                pt, _why = hx2cpp.ghidra_import_type(nm, o)          # as the raw translation types it
                if pt is None: continue
                gset(o['ea'], (nm, dict(o, func=False, type=pt)), R[ea].get('_exp', 0))
                gtypes.setdefault(o['ea'], collections.Counter())[pt] += 1
                gtier.setdefault(o['ea'], {}).setdefault(R[ea].get('_exp', 0), collections.Counter())[pt] += 1
                continue
            if o['func']:
                ft = ftype_of(ea, o, nm)
                on = oname(nm, o['ea'], ft)
                if '/*regs:' in ft:
                    # hx2cpp.ghidra_reg_call: an asm call that loads the registers its storage names, pushes the
                    # stack arguments in order and pops them when the callee does not
                    lines, _why = hx2cpp.ghidra_reg_call(nm, o, R[ea]['text'].partition('\n{')[2], on)
                    if lines:
                        lines = [re.sub(r'^static inline (.*?) xi_cr_[0-9A-F]+\(', r'XI_ORIG \1 %s(' % on, lines[0])] + lines[1:-1]
                        origs[(o['ea'], on)] = (on, '\n'.join(lines))
                    continue
                w = orig_wrapper(on, o['ea'], ft)
                # keyed by (address, name): a record exported before a rename still refers to the
                # old name, so both names get a wrapper to the same address
                if w: origs[(o['ea'], on)] = (on, w)
                if w and re.match(r'^(float|double|long double)\s*(__\w+\s*)?\(', o['type'].strip()):   # not `float *`
                    fwrap[on] = (o['ea'], ft)
            else:
                if R[ea].get('ghidra') and re.search(r'\bgoto\s+%s\s*;' % re.escape(nm), R[ea]['text']):
                    continue                            # a goto target of the body (LAB_...), not a global
                if R[ea].get('ghidra') and nm in hx2cpp.CXX_KEYWORDS:
                    continue                            # the keyword in the text, not a global (hx2cpp.CXX_KEYWORDS)
                gset(o['ea'], (nm, o), R[ea].get('_exp', 0))
                gtypes.setdefault(o['ea'], collections.Counter())[gtype_of(o, R[ea], nm)] += 1
                gtier.setdefault(o['ea'], {}).setdefault(R[ea].get('_exp', 0), collections.Counter())[gtype_of(o, R[ea], nm)] += 1
    with open(os.path.join(outdir, 'include', 'gfx_orig.hpp'), 'w') as f:
        f.write('// gfx_orig.hpp -- calls into the game\'s own code (not reconstructed here), by address.\n'
                '#pragma once\n#include "gfx_common.hpp"\n#define XI_ORIG static inline __attribute__((always_inline))\nnamespace orig {\n')
        for k in sorted(origs): f.write(origs[k][1] + '\n')
        f.write('}  // namespace orig\n')
        f.write('// a game function used as a value (callback, table entry): its address in the game image\n'
                'namespace orig_addr {\n')
        _seen = set()
        for (ea, _), (nm0, _w) in sorted(origs.items()):
            an = nm0 if re.match(r'^[A-Za-z_]\w*$', nm0) else 'fn_%08X' % ea
            if an in _seen: continue
            _seen.add(an); f.write('constexpr uint32_t %s = 0x%08Xu;\n' % (an, ea))
        f.write('}  // namespace orig_addr\n#define XI_CODE(name) reinterpret_cast<void *>(XI_GAME(orig_addr::name))\n')
    gnames, gcanon, gvariant = {}, {}, {}
    fnames = {r['name'] for ea_, r in R.items() if ea_ in passed}
    with open(os.path.join(outdir, 'include', 'gfx_globals.hpp'), 'w') as f:
        f.write('// gfx_globals.hpp -- game globals the graphics code uses, bound to their addresses.\n#pragma once\n#include "gfx_common.hpp"\n')
        for ea in sorted(globs):
            nm, o = globs[ea]
            gn = nm if re.match(r'^[A-Za-z_]\w*$', nm) and nm not in CXX_KEYWORDS else 'g_%08X' % ea
            # a global (an import table slot too) named like a function of the tree: its macro would replace that
            # function's declaration in every unit (the game's 0x10009770 is named SetCursorPos, as is the slot)
            if gn in fnames: gn = 'g_%s_%08X' % (gn, ea)
            gnames[ea] = gn; RESERVED.add(gn)
            from hx2cpp import typedef_for
            # functions disagree on a global's type when they read it at different widths (Hex-Rays
            # types each use): the most common type gets the plain name, every other gets its own
            # name at the same address (`unk_104571D8_as_u8`), so each function reads exactly as wide
            # as the original did
            # the plain name's type: the most common among the records of the first export that uses the global, so
            # a later export's records (the frame set's) do not change the spelling the earlier ones were built with
            cnt = gtypes[ea]; c0 = gtier[ea][min(gtier[ea])]
            canon = sorted(c0, key=lambda x: (-c0[x], -WIDTH.get(x, 4)))[0]
            gcanon[ea] = canon
            f.write(typedef_for('%s_t' % gn, plain_type(canon)) + '\n#define %s XI_EA(%s_t, 0x%08Xu)\n' % (gn, gn, ea))
            tier = {x: min(t for t, c in gtier[ea].items() if x in c) for x in cnt}
            for k, other in enumerate(sorted((x for x in cnt if x != canon), key=lambda x: (tier[x], x))):
                vn = '%s_as_%s' % (gn, SUFFIX.get(other, 'v%d' % k)); gvariant[(ea, other)] = vn
                f.write(typedef_for('%s_t' % vn, plain_type(other)) + '\n#define %s XI_EA(%s_t, 0x%08Xu)\n' % (vn, vn, ea))

    per_file = collections.defaultdict(list); decls = collections.defaultdict(list); report = {}
    for ea, (cls, meth, why) in sorted(place.items()):
        r = R[ea]; text = r['text']
        # GfxExport `sse`: the original's float arithmetic is SSE, each operation rounded to float (as hx2cpp.py)
        sse = '__attribute__((target("sse,fpmath=sse"))) ' if r.get('sse') else ''
        head, _, body = text.partition('\n{')
        body = hx2cpp.ctree_signs(r, '{' + body)
        # the IDB's own status lines ([gfx-recon ...]) describe a previous build; don't copy them
        comments = [l for l in head.splitlines() if l.startswith('//') and '[gfx-recon' not in l]
        sig = ' '.join(l for l in head.splitlines() if not l.startswith('//'))
        # names: objects -> orig::Name / global names
        reps = []
        for nm, o in r.get('objs', {}).items():
            forms = set(o.get('printed', [nm])) | {nm}
            for p in list(forms):
                m1 = re.match(r'^_?([A-Za-z]\w*)@\d+$', p)
                if m1: forms.add(m1.group(1))
                if re.match(r'^_\w+$', p): forms.add(p[1:])   # IDA '__strlwr' is printed '_strlwr'
                dm = demangle(p)
                if dm: forms.add(dm)
            if o['func'] and o['ea'] in R and R[o['ea']].get('name'):
                forms.add(R[o['ea']]['name'])          # a callee the text names by its own record's name (sub_X)
            if o['func'] and r.get('ghidra') and hx2cpp.FPRET_ALL.get('0x%x' % o['ea']) == 'consumes':
                # fpret.json: the callee pops a value the caller leaves in ST0 (__ftol); called as the raw translation
                # calls it (hx2cpp: xi_call_st0), not through a stack-argument wrapper
                tok = 'XI_CALL_ST0(0x%08Xu)' % o['ea']
            elif o['func'] and not o.get('import'):
                tok = 'orig::' + oname(nm, o['ea'], ftype_of(ea, o, nm))
            elif o.get('import'):
                tok = gnames[o['ea']]                  # the pointer at the import table slot
            elif o['ea'] not in gnames:
                continue                               # a goto target left out of the globals above
            else:
                tok = gvariant.get((o['ea'], gtype_of(o, r, nm)), gnames[o['ea']])
            for p in forms: reps.append((p, tok))
        from hx2cpp import token_re
        for p, tok in sorted(reps, key=lambda x: -len(x[0])):
            if p == tok: continue
            if tok.startswith('orig::'):
                # called -> orig::Name(...); used as a value (callback, vtable slot) -> the game's address
                rx = token_re(p)
                def _sub(m, tok=tok):
                    rest = body_ref[0][m.end():].lstrip()
                    return tok if rest.startswith('(') else 'XI_CODE(%s)' % tok[len('orig::'):]
                body_ref = [body]; body = rx.sub(_sub, body)
                body_ref = [sig]; sig = rx.sub(_sub, sig)
            else:
                body = token_re(p).sub(tok, body); sig = token_re(p).sub(tok, sig)
        sig = re.sub(r'\bthis\b', 'self', sig); body = re.sub(r'\bthis\b', 'self', body)
        body = body.replace('->base.vftable', '->vftable')
        body = re.sub(r'\benum\s+_?(D3D\w+)', r'\1', body); sig = re.sub(r'\benum\s+_?(D3D\w+)', r'\1', sig)
        body = hx2cpp.body_rules(ea, body, 'cmps' in r)       # before strip_noise: exact_frame reads the [esp+..] comments
        body = strip_noise(body); body = com_calls(body); body = name_d3d_values(body)
        body = void_float_calls(ea, body)
        if r.get('ghidra'):
            # as the raw translation (hx2cpp.ghidra_bool): Ghidra's bool is a byte of any value; the signature is
            # spelt in plain types, since the shared headers do not include xi_ghidra.hpp
            body = hx2cpp.ghidra_bool(body)
            # a `code *` parameter the body calls, `(*p)(...)`: callable (xi_codefn, gfx_common.hpp)
            sig = re.sub(r'\bcode \*\s*(\w+)', lambda m: ('xi_codefn * ' if re.search(r'\(\s*\*\s*%s\s*\)\s*\(' % m.group(1), body) else 'code * ') + m.group(1), sig)
            sig = plain_type(re.sub(r'\bbool\b', 'unsigned char', re.sub(r'\bfloat10\b', 'long double', sig)))
        m = re.match(r'^(.*?)\s*' + CC_RE + r'?\s*([~\w:?@$<>]+)\s*\((.*)\)\s*$', sig, re.S)
        ret, cc, _, params = m.groups(); cc = cc or '__cdecl'; ret = ret.strip()
        ret, body = hx2cpp.drop_float_return(ea, ret, body)
        ps = [re.sub(r'\s+', ' ', p).strip() for p in split_params(params)]
        # LOBYTE(x), HIWORD(x), ... on integer locals and parameters: the exact C (partial_regs.py)
        body, preg = partial_regs.rewrite(body, partial_regs.var_types(ps, body))
        body, nrel = reloc_literals(ea, body)
        note = '// FUNCTION: FFXIMAIN 0x%08X' % ea
        if cls and nothis.get(ea):
            mp = ', '.join(ps)
            st = nothis[ea] == 'static'
            ccs = (' ' + cc) if st else ''   # a static member keeps the original convention
            decls[cls].append('    %s%s%s %s(%s);  // 0x%08X' % ('static ' if st else '', ret, ccs, meth, mp, ea))
            code = '%s  (%s)\n%s%s%s %s::%s(%s)\n%s' % (note, 'placed by ' + why, sse, ret, ccs, cls, meth, mp, body.rstrip())
            per_file['src/%s.cpp' % cls].append((ea, code))
        elif cls:
            pname = re.findall(r'(\w+)\s*$', ps[0])[0] if ps else 'self'
            self_t = ps[0][:ps[0].rfind(pname)].strip() if ps else 'void *'
            if pname != 'self':
                body = re.sub(r'\b%s\b' % re.escape(pname), 'self', body)
            fields = layouts[cls].fields()
            body = rewrite_members(body, self_t, cls, fields)
            if re.sub(r'\s+', '', self_t) == cls + '*':
                body = re.sub(r'\bself\b', 'this', body)          # same type: `this` is exact
            elif re.search(r'\bself\b', body):
                body = body.replace('{', '{\n  %s self = reinterpret_cast<%s>(this);  // byte-exact view for the rest' % (self_t, self_t), 1)
            mp = ', '.join(ps[1:])
            decls[cls].append('    %s %s(%s);  // 0x%08X' % (ret, meth, mp, ea))
            code = '%s  (%s)\n%s%s %s::%s(%s)\n%s' % (note, 'placed by ' + why, sse, ret, cls, meth, mp, body.rstrip())
            per_file['src/%s.cpp' % cls].append((ea, code))
        else:
            fname = r['name'] if re.match(r'^[A-Za-z_]\w*$', r['name']) and not r['name'].startswith(('sub_', 'nullsub_')) else 'fn_%08X' % ea
            fname = fname if fname not in ('main',) else 'fn_%08X' % ea
            ps = [re.sub(r'\bself\b', 'self_', p) for p in ps]; body = re.sub(r'\bself\b', 'self_', body)
            u = UNITS.get(hex(ea))
            if u:
                note += '  (unit %s: %s)' % (u['unit'], u['why'])
                why = 'unit: ' + u['why']
            if r.get('ghidra') and (hx2cpp.extra_regs(r.get('cc'), r.get('storage'))
                                    or hx2cpp.undefined_reg_params(cc, ', '.join(ps))):
                rp, _why = hx2cpp.regparm_entry(ea, cc, ', '.join(ps), r.get('storage'))
                if rp is None: raise SystemExit('0x%08X: %s (the raw translation refused it too)' % (ea, _why))
                cc, ps = rp[0], hx2cpp._params(rp[1])
            code = '%s\nXI_API %s%s %s %s(%s)\n%s' % (note, sse, ret, cc, fname, ', '.join(ps) or 'void', body.rstrip())
            # a free function goes into the source unit the binary places it in (unit_place.py);
            # with no evidence it stays in a file named for its 64 KB slice of the image
            per_file['src/%s.cpp' % u['unit'] if u else 'src/gfx_free_%03X.cpp' % ((ea >> 16) & 0xFFF)].append((ea, code))
            decls['__free__'].append('XI_API %s %s %s(%s);  // 0x%08X' % (ret, cc, fname, ', '.join(ps) or 'void', ea))
        report[hex(ea)] = {'class': cls, 'method': meth, 'why': why,
                           'symbol': '%s::%s' % (cls, meth) if cls else fname}
        if not cls and UNITS.get(hex(ea)): report[hex(ea)]['unit'] = UNITS[hex(ea)]['unit']
        if preg: report[hex(ea)]['partial_regs'] = preg
        if nrel: report[hex(ea)]['reloc_literals'] = nrel

    # headers
    for cls, L in layouts.items():
        fields = L.fields(); size = L.size
        ends = [off + (f[3] if isinstance(f, tuple) else WIDTH.get(f, 4)) for off, f in fields.items()]
        if size and ends and max(ends) > size:
            size_conflicts[cls] = 'class descriptor says 0x%X; the code reaches offset 0x%X' % (size, max(ends))
            size = None
        lines = ['// %s -- graphics members reconstructed from Hex-Rays output, gate-verified.' % cls,
                 '// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.',
                 '#pragma once', '#include "gfx_common.hpp"', '#pragma pack(push, 1)', 'class XI_API %s {' % cls, 'public:']
        lines += sorted(set(decls[cls]), key=lambda s: s.split('//')[-1])
        lines.append('')
        pos = 0
        if cls not in ('D3DHolder',):
            lines.append('    void **vftable;  // +0x0'); pos = 4
        else:
            lines.append('    void **vftable;  // +0x0'); pos = 4
        for off in sorted(fields):
            f = fields[off]
            if off < pos: continue
            if off > pos: lines.append('    uint8_t _pad_%X[0x%X];' % (pos, off - pos))
            arr = ''
            if isinstance(f, tuple):
                t, n, w, arr = f[1], f[2], f[3], f[4]
            else:
                t, n, w = FTYPE.get(f, f), field_name(cls, off, f), WIDTH.get(f, 4)
            note = FIELD_NAMES[(cls, off)]['summary'] if (cls, off) in FIELD_NAMES and not isinstance(f, tuple) else ''
            lines.append('    %s %s%s;  // +0x%X%s' % (t, n, arr, off, ' ' + note if note else '')); pos = off + w
        if size and size > pos: lines.append('    uint8_t _pad_%X[0x%X];' % (pos, size - pos))
        lines += ['};', '#pragma pack(pop)']
        if size: lines.append('static_assert(sizeof(%s) == 0x%X, "%s: sizeof");' % (cls, size, cls))
        if cls in size_conflicts: lines.insert(2, '// SIZE CONFLICT: ' + size_conflicts[cls] + '. sizeof not asserted from the descriptor.' if not size else '// NOTE: ' + size_conflicts[cls] + '.')
        for off in sorted(fields):
            f = fields[off]
            if off < 4: continue
            n = f[2] if isinstance(f, tuple) else field_name(cls, off, f)
            lines.append('static_assert(XI_OFFSETOF(%s, %s) == 0x%X, "%s: layout");' % (cls, n, off, cls))
        open(os.path.join(outdir, 'include', '%s.hpp' % cls), 'w').write('\n'.join(lines) + '\n')
    with open(os.path.join(outdir, 'include', 'gfx_free.hpp'), 'w') as f:
        f.write('// gfx_free.hpp -- graphics functions with no established class.\n#pragma once\n#include "gfx_common.hpp"\n')
        f.write('\n'.join(sorted(set(decls['__free__']), key=lambda s: s.split('//')[-1])) + '\n')
    ghidra_ea = {ea for ea, r in R.items() if r.get('ghidra')}
    for path, items in per_file.items():
        cls = os.path.basename(path)[:-4]
        inc = '#include "%s.hpp"\n' % cls if cls in layouts else ''
        gh = any(ea in ghidra_ea for ea, _ in items)
        if gh: inc += '#include "xi_ghidra.hpp"     // the Ghidra decompiler\'s types and helpers (float10, NAN, ...)\n'
        with open(os.path.join(outdir, path), 'w') as f:
            f.write('// %s -- generated by hx2clean.py from gate-verified %s output.\n' % (
                os.path.basename(path), 'Hex-Rays and Ghidra' if gh and any(ea not in ghidra_ea for ea, _ in items) else 'Ghidra' if gh else 'Hex-Rays'))
            f.write('#include "gfx_all.hpp"\n' + inc + '\n')
            f.write('\n\n'.join(c for _, c in sorted(items)) + '\n')
    with open(os.path.join(outdir, 'include', 'gfx_all.hpp'), 'w') as f:
        f.write('#pragma once\n#include "gfx_common.hpp"\n#include "gfx_globals.hpp"\n#include "gfx_orig.hpp"\n'
                '#include "gfx_orig_void.hpp"\n#include "gfx_free.hpp"\n')
    with open(os.path.join(outdir, 'include', 'gfx_orig_void.hpp'), 'w') as f:
        f.write('// gfx_orig_void.hpp -- game functions typed as returning a float, called where the original never\n'
                '// reads the x87 result: the call returns void, so neither side pops it.\n#pragma once\n#include "gfx_common.hpp"\nnamespace orig_void {\n')
        for nm in sorted(voidv):
            cea, ctype = voidv[nm]
            w = orig_wrapper(nm, cea, re.sub(r'^(float|double|long double)\b', 'void', ctype.strip()))
            if w: f.write(w + '\n')
        f.write('}  // namespace orig_void\n')
        for cls in sorted(layouts): f.write('#include "%s.hpp"\n' % cls)
    json.dump(report, open(os.path.join(outdir, 'placement.json'), 'w'), indent=0)
    json.dump(size_conflicts, open(os.path.join(outdir, 'size_conflicts.json'), 'w'), indent=0)
    t = hx2cpp.types_header()
    blocks = re.split(r'\n(?=/\* \d+ \*/\n)', t); keep = []
    for b in blocks:
        m = re.search(r'^struct (\w+)\s*\n\{', b, re.M)
        if m and (m.group(1) in layouts or m.group(1) in replaced): continue
        keep.append(b)
    fwd = ''.join('class %s;\n' % c for c in sorted(layouts)) + ''.join('struct %s;\n' % c for c in sorted(replaced - set(layouts)))
    with open(os.path.join(outdir, 'include', 'gfx_types.hpp'), 'w') as f:
        f.write('// gfx_types.hpp -- game types from the IDB that this tree does not define itself.\n#pragma once\n'
                + fwd + '\n'.join(keep).replace('#pragma once', '').replace('struct CDx *', 'CDx *') + '\n')
    print('functions', len(place), 'classes', len(layouts), collections.Counter(v[2] for v in place.values()))

if __name__ == '__main__':
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    main(*sys.argv[1:6])
