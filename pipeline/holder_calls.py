"""holder_calls.py -- Direct3D 8 calls through a proven holder field written as typed method calls (task #124).

Hex-Rays prints a call through a field it has no type for as a cast vtable slot:
    (*(R (__stdcall **)(P...))(**(_DWORD **)(G + OFF) + SLOT))(*(_DWORD *)(G + OFF), a, b)
Where data/com_fields.json proves what G + OFF holds (an access path [global, offset] and its interface, with the
proof com_calls.py re-checks against the binary), the call is that interface's method number SLOT / 4 in the D3D8
header's order (data/com_spec.json) and is written as the SDK's C++ call (tree/include/xi_d3d8.hpp's wrapper)
    (*(IDirect3DDevice8 **)(G + OFF))->SetRenderState(a, b)
For Visual C++ 6.0, pipeline/vc6/sweep.py writes such a call back as the slot call (_d3d_slot_calls): the game was
built against the SDK's virtual interfaces, whose calls VC6 orders as a slot call, not as the tree's inline wrapper. Each argument whose parameter the header types other than
as a 4-byte integer is cast to that type (a literal of an enum parameter by its enumerator's name where the header has
one): the value passed is the same 4 bytes, now typed. A call through a holder the proofs do not cover, through a
local copy of the pointer, or whose slot or argument count the header does not give, is left as it is.

typed_self_calls does the same for a method's own field that the IDB declares as a Direct3D interface pointer (hx2clean's
KNOWN_FIELDS, e.g. CDx +0xC `IDirect3DDevice8 *pDevice`): `(**((_DWORD **)self + 3) + SLOT))(*((_DWORD *)self + 3), ...)`.
"""
import json, os, re

P = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.join(P, '..')
SPEC = json.load(open(os.path.join(ROOT, 'data', 'com_spec.json')))
_HDR = open(os.path.join(ROOT, 'tree', 'include', 'xi_d3d8.hpp')).read()
INT4 = {'unsigned int', 'int', 'unsigned long', 'long', 'DWORD', 'UINT'}


def _enums():
    out = {}
    for m in re.finditer(r'enum (\w+) \{(.*?)\};', _HDR, re.S):
        vals = {}
        for n, v in re.findall(r'(\w+) = (-?(?:0x[0-9A-Fa-f]+|\d+))', m.group(2)):
            vals.setdefault(int(v, 0), n)
        out[m.group(1)] = vals
    return out


def _wrappers():
    """{iface: {method: [param types]}} from the header's C++ wrappers (`int M(T a, U b) { return lpVtbl->M(...`)"""
    out = {}
    for m in re.finditer(r'struct (\w+) \{\s*\n\s*struct \1Vtbl \*lpVtbl;(.*?)\n\};', _HDR, re.S):
        meths = {}
        for w in re.finditer(r'\n\s+\w[\w ]*?\s(\w+)\(([^()]*)\) \{ return lpVtbl->', m.group(2)):
            ps = [p.strip() for p in w.group(2).split(',') if p.strip()]
            meths[w.group(1)] = [re.sub(r'\s*\b\w+$', '', p).strip() for p in ps]
        out[m.group(1)] = meths
    return out


ENUMS, WRAP = _enums(), _wrappers()


def _holders():
    """[(global address, offset, iface)] from data/com_fields.json: two-step paths [global, offset]"""
    out = []
    for k, v in json.load(open(os.path.join(ROOT, 'data', 'com_fields.json'))).items():
        if k.startswith('_') or not isinstance(v, dict): continue
        p = v.get('path') or []
        if len(p) == 2 and re.fullmatch(r'0x[0-9A-Fa-f]+', p[0]) and v.get('iface') in WRAP: out.append((int(p[0], 16), int(p[1], 16), v['iface']))
    return out


def _global_names():
    """{address: [tree global names]} from tree/include/gfx_globals.hpp's XI_EA bindings"""
    out = {}
    for n, a in re.findall(r'#define (\w+) XI_EA\(\w+, (0x[0-9A-Fa-f]+)u\)', open(os.path.join(ROOT, 'tree', 'include', 'gfx_globals.hpp')).read()):
        out.setdefault(int(a, 16), []).append(n)
    return out


def _split_args(s, i):
    """from s[i] (just inside an argument list) to its closing paren: ([args], index after the paren)"""
    depth, start, args = 0, i, []
    while i < len(s):
        c = s[i]
        if c in '([': depth += 1
        elif c in ')]':
            if depth == 0:
                if s[start:i].strip(): args.append(s[start:i].strip())
                return args, i + 1
            depth -= 1
        elif c == ',' and depth == 0: args.append(s[start:i].strip()); start = i + 1
        i += 1
    return None, i


def _typed_arg(a, t):
    t = t.strip()
    if t in INT4 or t == 'float': return a
    if t in ENUMS:
        lit = re.fullmatch(r'(0x[0-9A-Fa-f]+|\d+)[uU]?', a)
        if lit and int(lit.group(1), 0) in ENUMS[t]: return ENUMS[t][int(lit.group(1), 0)]
    return '(%s)(%s)' % (t, a)


def _rewrite(body, objrx, objtext, iface):
    """every cast-slot call whose vtable is `**(OBJ) + SLOT` and whose first argument is `*(OBJ)`, typed"""
    rx = re.compile(r'\(\*\([^()]*?\(__stdcall \*\*\)\([^()]*\)\)\(' + objrx[0] + r' \+ (\d+)\)\)\(\s*' + objrx[1])
    out, pos, n = [], 0, 0
    for m in rx.finditer(body):
        if m.start() < pos: continue
        slot = int(m.group(1))
        if slot % 4 or slot // 4 >= len(SPEC[iface]): continue
        meth = SPEC[iface][slot // 4]['name']
        ptypes = WRAP[iface].get(meth)
        rest = body[m.end():]
        if rest.lstrip().startswith(','): j = m.end() + rest.index(',') + 1
        elif rest.lstrip().startswith(')'): j = m.end() + rest.index(')')
        else: continue
        args, end = _split_args(body, j)
        if args is None or ptypes is None or len(args) != len(ptypes): continue
        out.append(body[pos:m.start()] + '%s->%s(%s)' % (objtext, meth, ', '.join(_typed_arg(a, t) for a, t in zip(args, ptypes))))
        pos = end; n += 1
    return ''.join(out) + body[pos:], n


def typed_self_calls(body, fields):
    """fields: {byte offset: iface} of the method's own class; body's `self` calls through them typed"""
    n = 0
    for off, iface in fields.items():
        if off % 4 or iface not in WRAP: continue
        k = off // 4
        objrx = (r'\*\*\(\((?:_DWORD|int) \*\*\)self \+ %d\)' % k, r'\*\(\((?:_DWORD|int) \*\)self \+ %d\)' % k)
        body, c = _rewrite(body, objrx, '((%s *)*((_DWORD *)self + %d))' % (iface, k), iface); n += c
    return body, n


def typed_holder_calls(body):
    """body with the calls through proven holders typed; and how many"""
    global _RX
    names = _global_names()
    n = 0
    for g, off, iface in _holders():
        for gname in names.get(g, []):
            obj = r'%s \+ (?:%d|0x%X|0x%x)' % (re.escape(gname), off, off, off)
            rx = re.compile(r'\(\*\([^()]*?\(__stdcall \*\*\)\([^()]*\)\)\(\*\*\((?:_DWORD|int|unsigned int) \*\*\)\((' + obj +
                            r')\) \+ (\d+)\)\)\(\s*\*\((?:_DWORD|int|unsigned int) \*\)\(\1\)')
            out, pos = [], 0
            for m in rx.finditer(body):
                if m.start() < pos: continue
                slot = int(m.group(2))
                if slot % 4 or slot // 4 >= len(SPEC[iface]): continue
                meth = SPEC[iface][slot // 4]['name']
                ptypes = WRAP[iface].get(meth)
                rest = body[m.end():]
                if rest.lstrip().startswith(','): j = m.end() + rest.index(',') + 1
                elif rest.lstrip().startswith(')'): j = m.end() + rest.index(')')
                else: continue
                args, end = _split_args(body, j)
                if args is None or ptypes is None or len(args) != len(ptypes): continue
                o = '(*(%s **)(%s + 0x%X))' % (iface, gname, off)
                call = '%s->%s(%s)' % (o, meth, ', '.join(_typed_arg(a, t) for a, t in zip(args, ptypes)))
                out.append(body[pos:m.start()] + call); pos = end; n += 1
            body = ''.join(out) + body[pos:]
    return body, n


if __name__ == '__main__':
    import sys
    t = open(sys.argv[1]).read()
    b, n = typed_holder_calls(t)
    print(b if '-p' in sys.argv else n)
