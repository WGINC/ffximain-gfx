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

Usage: python3 hx2clean.py export.jsonl gate.jsonl manifest.json tree_include_dir out_dir
"""
import json, os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import collections
from msvc_demangle import demangle
import hx_patches
import hx2cpp

CC_RE = r'(__cdecl|__stdcall|__thiscall|__fastcall)'
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

def orig_wrapper(name, ea, ftype):
    ftype = re.sub(r'^([^()]*)\(\s*(__\w+\s*)?\*\s*\)\s*\(', r'\1 \2(', ftype)   # IDA's `R (*)(args)`: the function type
    ftype = hx2cpp.fnptr_return(ftype)
    m = re.match(r'^(.*?)\s*' + CC_RE + r'?\s*\((.*)\)\s*$', ftype, re.S)
    if not m or re.search(r'__user|@<', ftype): return None
    ret, cc, args = m.groups(); cc = cc or '__cdecl'
    if '(' in ret or ')' in ret: return None       # a return type not parsed: no wrapper rather than a wrong one
    if cc.strip() == '__cdecl' and hx2cpp.PURGES.get('0x%08X' % ea): cc = '__stdcall'   # pops (`ret N`): D3DX dispatch thunks
    ret = re.sub(r'\b__noreturn\b|' + CC_RE, '', ret).strip() or 'int'    # `void __cdecl __noreturn`
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
    if variadic:
        body = 'return reinterpret_cast<%s>(0x%08Xu)(%s);' % (fp, ea, ', '.join(names + ['rest...']))
        if ret == 'void': body = body.replace('return ', '')
        return ('template <class... R> XI_ORIG %s %s(%s) { %s }' % (
            ret, name, ', '.join(['%s %s' % (p, n) for p, n in zip(ps, names)] + ['R... rest']), body))
    body = 'return reinterpret_cast<%s>(0x%08Xu)(%s);' % (fp, ea, ', '.join(names))
    if ret == 'void': body = body.replace('return ', '')
    return ('XI_ORIG %s %s(%s) { %s }' % (ret, name, ', '.join('%s %s' % (p, n) for p, n in zip(ps, names)), body))

# ------------------------------------------------------------------ text rewriting
def strip_noise(body):
    body = re.sub(r'\s*//\s*(e?[abcd]x|e?[sd]i|e?bp|e?sp|[abcd][lh]|st\d?|\[esp[^\n]*|\[ebp[^\n]*)\s*$', '', body, flags=re.M)
    return body

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

PLACEHOLDER = re.compile(r'^(sub|dword|word|byte|flt|dbl|qword|unk|off|stru)_[0-9A-F]{8}$')


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

def canonicalise_names(R, pending):
    # A record exported before an IDB rename still prints the old name, so one address can carry
    # two names across records. Every referenced object gets one name: the reviewed name from
    # names_pending_idb.json (not yet in the IDB), else the IDB's real name, else the placeholder.
    # Only references are renamed; a record's own name drives class placement and is left alone.
    seen = collections.defaultdict(collections.Counter)
    for r in R.values():
        for nm, o in r['objs'].items():
            if not PLACEHOLDER.match(nm): seen[o['ea']][nm] += 1
    canon = {ea: c.most_common(1)[0][0] for ea, c in seen.items()}
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
            new = canon.get(o['ea'])
            if not new or new == nm or o['ea'] == r['ea']: continue
            if new in r['objs'] and r['objs'][new]['ea'] != o['ea']: continue   # would collide
            r['text'] = re.sub(r'\b%s\b' % re.escape(nm), new, r['text'])
            o['printed'] = [new if p == nm else p for p in o.get('printed', [])]
            r['objs'].setdefault(new, o); del r['objs'][nm]

# ------------------------------------------------------------------ main
def main(exp, gate, manf, incdir, outdir):
    R = {}
    _dm = json.load(open(os.path.join(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data'), 'dm_names.json')))
    for l in open(exp):
        r = json.loads(re.sub(r"\s*__spoils<[^>]*>", "", l))   # callee-preserves-register notes: Hex-Rays only
        r = hx_patches.apply(r)
        dmn = _dm.get(hex(r['ea']))
        if dmn and r['name'].startswith('sub_') and re.match(r'^[A-Za-z_]\w*$', dmn[0]):
            r['name'] = dmn[0]                       # DancingMad's newer name (Sep 2026 update)
        R[r['ea']] = r
    canonicalise_names(R, json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'names_pending_idb.json'))))
    passed = set()
    for l in open(gate):
        g = json.loads(l)
        if g.get('verdict') == 'PASS': passed.add(int(g['original'], 16))
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
        place[ea] = (cls, ident(meth) if meth else None, why)

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
    origs, globs, gtypes = {}, {}, {}
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
    def gtype_of(o):
        t = 'char[1]' if o.get('is_str') else o['type']
        if re.sub(r'^(struct\s+)?(std::)?', '', t.strip()) in ('Iostream_init', 'exception', 'type_info', 'codecvt_base'): t = 'char[1]'
        return t.strip()
    SUFFIX = {'_BYTE': 'u8', 'unsigned __int8': 'u8', 'char': 's8', '__int8': 's8', '_WORD': 'u16', 'unsigned __int16': 'u16',
              '__int16': 's16', '_DWORD': 'u32', 'unsigned int': 'u32', 'int': 's32', 'float': 'f32', 'double': 'f64'}
    for ea in passed:
        for nm, o in R[ea].get('objs', {}).items():
            if o['func']:
                w = orig_wrapper('fn_%08X' % o['ea'] if not re.match(r'^[A-Za-z_]\w*$', nm) else nm, o['ea'], o['type'])
                # keyed by (address, name): a record exported before a rename still refers to the
                # old name, so both names get a wrapper to the same address
                if w: origs[(o['ea'], nm)] = (nm, w)
                if w and re.match(r'^(float|double|long double)\s*(__\w+\s*)?\(', o['type'].strip()):   # not `float *`
                    fwrap[nm] = (o['ea'], o['type'])
            else:
                globs.setdefault(o['ea'], (nm, o))
                gtypes.setdefault(o['ea'], collections.Counter())[gtype_of(o)] += 1
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
        f.write('}  // namespace orig_addr\n#define XI_CODE(name) reinterpret_cast<void *>(orig_addr::name)\n')
    gnames, gcanon, gvariant = {}, {}, {}
    with open(os.path.join(outdir, 'include', 'gfx_globals.hpp'), 'w') as f:
        f.write('// gfx_globals.hpp -- game globals the graphics code uses, bound to their addresses.\n#pragma once\n#include "gfx_common.hpp"\n')
        for ea in sorted(globs):
            nm, o = globs[ea]
            gn = nm if re.match(r'^[A-Za-z_]\w*$', nm) else 'g_%08X' % ea
            gnames[ea] = gn; RESERVED.add(gn)
            from hx2cpp import typedef_for
            # functions disagree on a global's type when they read it at different widths (Hex-Rays
            # types each use): the most common type gets the plain name, every other gets its own
            # name at the same address (`unk_104571D8_as_u8`), so each function reads exactly as wide
            # as the original did
            cnt = gtypes[ea]
            canon = sorted(cnt, key=lambda x: (-cnt[x], -WIDTH.get(x, 4)))[0]
            gcanon[ea] = canon
            f.write(typedef_for('%s_t' % gn, canon) + '\n#define %s XI_EA(%s_t, 0x%08Xu)\n' % (gn, gn, ea))
            for k, other in enumerate(sorted(x for x in cnt if x != canon)):
                vn = '%s_as_%s' % (gn, SUFFIX.get(other, 'v%d' % k)); gvariant[(ea, other)] = vn
                f.write(typedef_for('%s_t' % vn, other) + '\n#define %s XI_EA(%s_t, 0x%08Xu)\n' % (vn, vn, ea))

    per_file = collections.defaultdict(list); decls = collections.defaultdict(list); report = {}
    for ea, (cls, meth, why) in sorted(place.items()):
        r = R[ea]; text = r['text']
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
            if o['func']:
                if o['ea'] == ea and cls: tok = 'orig::' + (nm if re.match(r'^[A-Za-z_]\w*$', nm) else 'fn_%08X' % o['ea'])
                else: tok = 'orig::' + (nm if re.match(r'^[A-Za-z_]\w*$', nm) else 'fn_%08X' % o['ea'])
            else:
                tok = gvariant.get((o['ea'], gtype_of(o)), gnames[o['ea']])
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
        m = re.match(r'^(.*?)\s*' + CC_RE + r'?\s*([~\w:?@$<>]+)\s*\((.*)\)\s*$', sig, re.S)
        ret, cc, _, params = m.groups(); cc = cc or '__cdecl'; ret = ret.strip()
        ret, body = hx2cpp.drop_float_return(ea, ret, body)
        ps = [re.sub(r'\s+', ' ', p).strip() for p in split_params(params)]
        note = '// FUNCTION: FFXIMAIN 0x%08X' % ea
        if cls and nothis.get(ea):
            mp = ', '.join(ps)
            st = nothis[ea] == 'static'
            ccs = (' ' + cc) if st else ''   # a static member keeps the original convention
            decls[cls].append('    %s%s%s %s(%s);  // 0x%08X' % ('static ' if st else '', ret, ccs, meth, mp, ea))
            code = '%s  (%s)\n%s%s %s::%s(%s)\n%s' % (note, 'placed by ' + why, ret, ccs, cls, meth, mp, body.rstrip())
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
            code = '%s  (%s)\n%s %s::%s(%s)\n%s' % (note, 'placed by ' + why, ret, cls, meth, mp, body.rstrip())
            per_file['src/%s.cpp' % cls].append((ea, code))
        else:
            fname = r['name'] if re.match(r'^[A-Za-z_]\w*$', r['name']) and not r['name'].startswith(('sub_', 'nullsub_')) else 'fn_%08X' % ea
            fname = fname if fname not in ('main',) else 'fn_%08X' % ea
            ps = [re.sub(r'\bself\b', 'self_', p) for p in ps]; body = re.sub(r'\bself\b', 'self_', body)
            u = UNITS.get(hex(ea))
            if u:
                note += '  (unit %s: %s)' % (u['unit'], u['why'])
                why = 'unit: ' + u['why']
            code = '%s\nXI_API %s %s %s(%s)\n%s' % (note, ret, cc, fname, ', '.join(ps) or 'void', body.rstrip())
            # a free function goes into the source unit the binary places it in (unit_place.py);
            # with no evidence it stays in a file named for its 64 KB slice of the image
            per_file['src/%s.cpp' % u['unit'] if u else 'src/gfx_free_%03X.cpp' % ((ea >> 16) & 0xFFF)].append((ea, code))
            decls['__free__'].append('XI_API %s %s %s(%s);  // 0x%08X' % (ret, cc, fname, ', '.join(ps) or 'void', ea))
        report[hex(ea)] = {'class': cls, 'method': meth, 'why': why,
                           'symbol': '%s::%s' % (cls, meth) if cls else fname}
        if not cls and UNITS.get(hex(ea)): report[hex(ea)]['unit'] = UNITS[hex(ea)]['unit']

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
    for path, items in per_file.items():
        cls = os.path.basename(path)[:-4]
        inc = '#include "%s.hpp"\n' % cls if cls in layouts else ''
        with open(os.path.join(outdir, path), 'w') as f:
            f.write('// %s -- generated by hx2clean.py from gate-verified Hex-Rays output.\n' % os.path.basename(path))
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
    t = open(os.path.join(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data'), 'gfx_types_game.h')).read()
    blocks = re.split(r'\n(?=/\* \d+ \*/\n)', t); keep = []
    for b in blocks:
        m = re.search(r'^struct (\w+)\s*\n\{', b, re.M)
        if m and m.group(1) in layouts: continue
        keep.append(b)
    fwd = ''.join('class %s;\n' % c for c in sorted(layouts))
    with open(os.path.join(outdir, 'include', 'gfx_types.hpp'), 'w') as f:
        f.write('// gfx_types.hpp -- game types from the IDB that this tree does not define itself.\n#pragma once\n'
                + fwd + '\n'.join(keep).replace('#pragma once', '').replace('struct CDx *', 'CDx *') + '\n')
    print('functions', len(place), 'classes', len(layouts), collections.Counter(v[2] for v in place.values()))

if __name__ == '__main__':
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    main(*sys.argv[1:6])
