"""idb_access.py -- turn Hex-Rays' accesses to members of the IDB's structs back into the offsets they access, for
the structs the clean tree defines itself.

The IDB now types many objects with its own structs (`CDx *`, `CTsZoneMap *`), so Hex-Rays prints
`g_pCDx->_pad_10_1F4[1849]` or `this->base.base._pad_104` where it used to print `*((_BYTE *)g_pCDx + 2349)`.
The clean tree replaces those structs with its own class layouts, which have other members. Each access is
rewritten to the offset of the member in the IDB struct (data/idb_members.json, offsets from the compiler),
which is the offset the code accesses: Hex-Rays chose the member the struct places there.

Only member steps on a *replaced* struct are rewritten: one the tree defines, or an IDB struct that holds
one by value (its own layout then depends on the tree's class). The root of a chain must have a declared
type: the function's parameters (the export's `args`), its locals (Hex-Rays' declarations), the globals
(`objs`), or a cast `((S *)x)`. A chain with an index that is not a literal, or into a multi-dimensional
array, keeps its form and is counted in `left`.

For the method's own `this` (when the function is placed in a class) the access is written in the forms
hx2clean.find_accesses reads, so the class layout learns the field and the access becomes `this->field`:
  *(T *)(this + OFF)          T an integer or float type (hx2clean.WIDTH), OFF in bytes
  *((B **)this + OFF/4)       T = `B *` with B one of those types, OFF a multiple of 4
Any other access is `(*(T *)((char *)X + 0xOFF))`, the same bytes read at the same width."""
import json, os, re

MEM = json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'idb_members.json')))

def struct_of(t):
    """'CDx *' -> ('CDx', 1); 'CDx' -> ('CDx', 0); anything else -> (None, 0)"""
    t = re.sub(r'\b(struct|class|const|volatile)\b', '', t).strip()
    m = re.match(r'^(\w+)\s*(\**)$', t)
    if not m or m.group(1) not in MEM: return None, 0
    return m.group(1), len(m.group(2))

def replaced_set(tree_classes):
    rep = {c for c in tree_classes if c in MEM}
    while True:
        more = {s for s, d in MEM.items() if s not in rep and any(not dims and struct_of(t) in [(r, 0) for r in rep]
                                                                  for _, _, _, t, dims in d['members'])}
        if not more: return rep
        rep |= more

def _member(s, name):
    for n, off, size, t, dims in MEM[s]['members']:
        if n == name: return off, size, t, dims
    return None

def _ptr_to(t):
    """the type `pointer to t` written as a cast target"""
    if re.search(r'\(\s*(__\w+\s*)?\*\s*\)', t):            # function pointer: one more star inside
        return re.sub(r'\(\s*((?:__\w+\s*)?)\*\s*\)', r'(\1**)', t, count=1)
    return t.rstrip() + ' *'

def _literal(s):
    s = s.strip()
    m = re.fullmatch(r'(0x[0-9A-Fa-f]+|\d+)[uUlL]*', s)
    return int(m.group(1), 0) if m else None

def _bracket(text, p):
    """index of the ']' matching the '[' at p, or -1"""
    depth = 0
    for i in range(p, len(text)):
        if text[i] == '[': depth += 1
        elif text[i] == ']':
            depth -= 1
            if depth == 0: return i
    return -1

def var_types(r, body):
    """name -> declared type for the function's parameters, locals and globals"""
    vt = {}
    for a in r.get('args') or []:
        if a and a[0]: vt[a[0]] = a[1]
    decls = body.split('\n\n', 1)[0]
    for m in re.finditer(r'^\s+([^;\n]*?[\w*])\s*\b(\w+)((?:\[\d+\])*);', decls, re.M):
        if not m.group(3): vt[m.group(2)] = m.group(1)
    for nm, o in (r.get('objs') or {}).items():
        if o.get('func'): continue
        for p in set(o.get('printed', [nm])) | {nm}: vt[p] = o.get('type', '')
    return vt

def rewrite(r, text, replaced, this_name, this_forms, width, shadow=frozenset()):
    """text with every chain through a replaced struct written as an offset access; (text, rewritten, left)"""
    vt = var_types(r, text.split('\n{', 1)[-1])
    shadow = set(shadow) | {nm for nm, o in (r.get('objs') or {}).items() if not o.get('func')}
    root_rx = re.compile(r'\(\((?:struct\s+)?(\w+)\s*\*\)\s*(\w+)\)(?=->)|(?<![\w.>])\b(\w+)\b(?=->)')
    out, pos, done, left = [], 0, 0, 0
    for m in root_rx.finditer(text):
        if m.start() < pos: continue
        if m.group(1):
            ty, base = m.group(1) + ' *', m.group(0); is_this = False
        else:
            ty, base = vt.get(m.group(3), ''), m.group(3); is_this = this_forms and m.group(3) == this_name
        s, depth = struct_of(ty)
        if s not in replaced or depth != 1: continue
        p = m.end(); expr = None; ok = True; any_step = False
        while True:
            s, depth = struct_of(ty)
            if not (text.startswith('->', p) and depth == 1 and s in replaced): break
            mm = re.match(r'->(\w+)', text[p:]); mem = _member(s, mm.group(1)) if mm else None
            if not mem: ok = False; break
            off, size, t, dims = mem; q = p + mm.end()
            while True:
                if text.startswith('.', q) and not dims:
                    s2, d2 = struct_of(t); mm2 = re.match(r'\.(\w+)', text[q:])
                    sub = _member(s2, mm2.group(1)) if (s2 and d2 == 0 and mm2) else None
                    if not sub: break
                    off += sub[0]; size, t, dims = sub[1], sub[2], sub[3]; q += mm2.end(); continue
                if text.startswith('[', q) and dims:
                    e = _bracket(text, q); idx = _literal(text[q + 1:e]) if e > 0 else None
                    if idx is None or len(dims) > 1: break
                    elem = size // dims[0]; off += idx * elem; size, dims = elem, []; q = e + 1; continue
                break
            if dims:
                if len(dims) > 1: ok = False; break
                expr = '((%s)((char *)%s + 0x%X))' % (_ptr_to(t), base, off)    # the array as a pointer to its first element
            elif is_this and expr is None and t.strip() in width:
                expr = '*(%s *)(%s + %d)' % (t.strip(), base, off)
            elif is_this and expr is None and t.strip().endswith('*') and t.strip()[:-1].strip() in width and off % 4 == 0:
                expr = '*((%s **)%s + %d)' % (t.strip()[:-1].strip(), base, off // 4)
            else:
                tt = t
                # a pointer to a struct that has the same name as one of the function's globals (the IDB names a
                # vtable's type and the vtable itself alike; the tree binds every global as a macro): used only as
                # a value (assigned, its address taken) it is written as `void *`, the same 4 bytes
                bn = struct_of(t)[0] or re.sub(r'[\s*]', '', t)
                if t.strip().endswith('*') and bn in shadow and not re.match(r'\s*(->|\.|\[)', text[q:]): tt = 'void *'
                expr = '(*(%s)((char *)%s + 0x%X))' % (_ptr_to(tt), base, off)
            any_step = True; p = q; ty = t if not dims else ''; base = '(%s)' % expr if not expr.startswith('(') else expr
            is_this = False
        if not ok or not any_step:
            left += not ok
            continue
        out.append(text[pos:m.start()]); out.append(expr); pos = p; done += 1
    out.append(text[pos:])
    return ''.join(out), done, left
