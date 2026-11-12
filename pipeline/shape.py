"""shape.py -- source-shaping passes over the clean tree's text: rewrites that change how the source reads and not
what it compiles to.

hx2clean.py calls shape_source() on every source file it writes, so the tree regenerates with the passes applied.
The proof that a pass changes nothing is mechanical: `python3 pipeline/shape.py check` compiles every file of tree/src
as committed and with the passes undone/applied and compares the objects' code and data byte for byte
(--selftest of each pass is that comparison). A pass that changes one byte of one object is a defect in the pass.

Pass 1, d3d_values: the value of a Direct3D render state or texture stage state, written as the SDK's name.
  Source of each name and value: the Direct3D 8 SDK's d3d8types.h, as the tree declares them in xi_d3d8.hpp.
  Source of which enumeration a state takes: the Direct3D 8 SDK's documentation of D3DRENDERSTATETYPE and
  D3DTEXTURESTAGESTATETYPE (each member's entry names its value type), written out by hand as STATE_KIND below; it
  is not machine-read from a file in this repository. A wrong row there would give a misleading name, never different
  code: the name stands for the same integer, and `shape.py check` proves the objects are the same.
  Rewritten only when the call names the state literally (D3DRS_x / D3DTSS_x) and the value is an integer literal
  that the enumeration has a name for; anything else is left as it is.
"""
import os, re, subprocess, sys

H = os.path.dirname(os.path.abspath(__file__))

# state -> the kind of its value (the SDK documentation's entry for each state; see the docstring)
STATE_KIND = {
    'D3DRS_ZENABLE': 'D3DZB_', 'D3DRS_FILLMODE': 'D3DFILL_', 'D3DRS_SHADEMODE': 'D3DSHADE_',
    'D3DRS_SRCBLEND': 'D3DBLEND_', 'D3DRS_DESTBLEND': 'D3DBLEND_', 'D3DRS_CULLMODE': 'D3DCULL_',
    'D3DRS_ZFUNC': 'D3DCMP_', 'D3DRS_ALPHAFUNC': 'D3DCMP_', 'D3DRS_STENCILFUNC': 'D3DCMP_',
    'D3DRS_FOGTABLEMODE': 'D3DFOG_', 'D3DRS_FOGVERTEXMODE': 'D3DFOG_', 'D3DRS_BLENDOP': 'D3DBLENDOP_',
    'D3DRS_STENCILFAIL': 'D3DSTENCILOP_', 'D3DRS_STENCILZFAIL': 'D3DSTENCILOP_', 'D3DRS_STENCILPASS': 'D3DSTENCILOP_',
    'D3DRS_DIFFUSEMATERIALSOURCE': 'D3DMCS_', 'D3DRS_SPECULARMATERIALSOURCE': 'D3DMCS_',
    'D3DRS_AMBIENTMATERIALSOURCE': 'D3DMCS_', 'D3DRS_EMISSIVEMATERIALSOURCE': 'D3DMCS_',
    'D3DTSS_COLOROP': 'D3DTOP_', 'D3DTSS_ALPHAOP': 'D3DTOP_',
    'D3DTSS_COLORARG0': 'TA', 'D3DTSS_COLORARG1': 'TA', 'D3DTSS_COLORARG2': 'TA', 'D3DTSS_ALPHAARG0': 'TA',
    'D3DTSS_ALPHAARG1': 'TA', 'D3DTSS_ALPHAARG2': 'TA', 'D3DTSS_RESULTARG': 'TA',
    'D3DTSS_ADDRESSU': 'D3DTADDRESS_', 'D3DTSS_ADDRESSV': 'D3DTADDRESS_', 'D3DTSS_ADDRESSW': 'D3DTADDRESS_',
    'D3DTSS_MAGFILTER': 'D3DTEXF_', 'D3DTSS_MINFILTER': 'D3DTEXF_', 'D3DTSS_MIPFILTER': 'D3DTEXF_',
    'D3DTSS_TEXTURETRANSFORMFLAGS': 'TTFF', 'D3DTSS_TEXCOORDINDEX': 'TCI',
}
for _s in ('ZWRITEENABLE', 'ALPHATESTENABLE', 'LASTPIXEL', 'DITHERENABLE', 'ALPHABLENDENABLE', 'FOGENABLE', 'SPECULARENABLE',
           'RANGEFOGENABLE', 'STENCILENABLE', 'CLIPPING', 'LIGHTING', 'COLORVERTEX', 'LOCALVIEWER', 'NORMALIZENORMALS',
           'EDGEANTIALIAS', 'SOFTWAREVERTEXPROCESSING', 'POINTSPRITEENABLE', 'POINTSCALEENABLE', 'MULTISAMPLEANTIALIAS',
           'INDEXEDVERTEXBLENDENABLE'):
    STATE_KIND['D3DRS_' + _s] = 'BOOL'

_names = None
def names():
    """prefix -> {value: name}, from the tree's own D3D header (enum members and #defines)"""
    global _names
    if _names is None:
        t = open(os.path.join(H, 'xi_d3d8.hpp')).read(); _names = {}
        for n, v in re.findall(r'^\s*(?:#define\s+)?(D3D[A-Z0-9_]+?)\s*(?:=\s*|\s+)(0x[0-9A-Fa-f]+|\d+)\s*,?\s*(?://.*|/\*.*)?$', t, re.M):
            for pre in set(STATE_KIND.values()) | {'D3DTA_', 'D3DTTFF_', 'D3DTSS_TCI_'}:
                if n.startswith(pre) and 'FORCE' not in n and 'MASK' not in n: _names.setdefault(pre, {}).setdefault(int(v, 0), n)
    return _names

def value_name(state, v):
    k = STATE_KIND.get(state); N = names()
    if k is None: return None
    if k == 'BOOL': return {0: 'FALSE', 1: 'TRUE'}.get(v)
    if k == 'TA':                                   # an argument: the selection, then the two modifier bits
        sel = N['D3DTA_'].get(v & 0xF); rest = v & ~0xF
        if sel is None or (v & 0xF) > 5 or rest & ~0x30: return None
        return ' | '.join([sel] + [N['D3DTA_'][b] for b in (0x10, 0x20) if rest & b])
    if k == 'TTFF':
        c = N.get('D3DTTFF_', {}).get(v & 0xFF)
        if c is None or v & ~0x1FF: return None
        return c + (' | D3DTTFF_PROJECTED' if v & 0x100 else '')
    if k == 'TCI':                                  # the coordinate set's index, with a generation mode in the high word
        if v < 0x10000: return None                 # a plain index stays a number
        m = N.get('D3DTSS_TCI_', {}).get(v & 0xFFFF0000)
        if m is None: return None
        return m if not v & 0xFFFF else '%s | %d' % (m, v & 0xFFFF)
    return N.get(k, {}).get(v)

_CALL = re.compile(r'(\bSet(?:RenderState|TextureStageState)\w*\((?:[^;\n]*?,\s*)?)(D3DRS_\w+|D3DTSS_\w+),\s*(0x[0-9A-Fa-f]+|\d+)u?\);')
def d3d_values(text, count=None):
    def sub(m):
        nm = value_name(m.group(2), int(m.group(3), 0))
        if nm is None: return m.group(0)
        if count is not None: count[0] += 1
        return '%s%s, %s);' % (m.group(1), m.group(2), nm)
    return _CALL.sub(sub, text)


# Pass 2, members: a read at a fixed offset from a pointer whose class the tree declares, written as the member.
#   `*(T *)((char *)B + N)` (and the `*(T *)&(*(unsigned __int8 *)((char *)B + N))` form) where B's class has a
#   member starting at +N (the class header's `// +0xN` comment, written by hx2clean.py from the layout) becomes
#   `B->m` when T is the member's declared type, and `*(T *)&B->m` otherwise: the same address, the same type read.
#   B is a global the tree declares as a class pointer (gfx_globals.hpp: `typedef C * NAME_t;`) or `this` in a
#   member function of C (the definition `... C::name(`). Any other read stays as it is.
_INC = [os.path.join(H, '..', 'tree', 'include')]
def set_include(d): _INC[0] = d; _layout.clear()
_layout = {}
def layout():
    if not _layout:
        inc = _INC[0]; mem = {}
        for f in os.listdir(inc):
            if not f.endswith('.hpp'): continue
            for ty, nm, arr, off in re.findall(r'^\s+([\w][\w \*:<>]*?)\s*\b(\w+)(\[[^\]]+\])?;\s*//\s*\+0x([0-9A-Fa-f]+)\s*$', open(os.path.join(inc, f)).read(), re.M):
                mem.setdefault(f[:-4], {})[int(off, 16)] = (ty.strip(), nm, bool(arr))
        g = {}
        gp = os.path.join(inc, 'gfx_globals.hpp')
        if os.path.exists(gp):
            for t, n in re.findall(r'typedef ([^;]+?) (\w+)_t;', open(gp).read()):
                m = re.match(r'^(?:class |struct )?(\w+) \*$', t.strip())
                if m and m.group(1) in mem: g[n] = m.group(1)
        _layout['mem'], _layout['glob'] = mem, g
    return _layout['mem'], _layout['glob']

_T = r'(?P<T>[A-Za-z_][\w ]*?(?:\s*\*)*)'
_NUM = r'(?P<N>0x[0-9A-Fa-f]+|\d+)'
_MEMBER = re.compile(r'\*\(' + _T + r' ?\*\)(?:(?P<amp>&\(\*\(unsigned __int8 \*\)\()|\()\(char \*\)(?P<B>\w+) \+ ' + _NUM + r'\)(?(amp)\)|)')
_DEF = re.compile(r'^[A-Za-z_][^\n;]*?\b(\w+)::~?\w+\(', re.M)
def members(text, count=None):
    mem, glob = layout()
    if not mem: return text
    out = []
    for chunk in re.split(r'(?=^// FUNCTION: )', text, flags=re.M):
        d = _DEF.search(chunk); this_cls = d.group(1) if d and d.group(1) in mem else None
        def sub(m):
            b = m.group('B'); c = glob.get(b) or (this_cls if b == 'this' else None)
            if c is None: return m.group(0)
            e = mem[c].get(int(m.group('N'), 0))
            if e is None: return m.group(0)
            t = re.sub(r'\s+', '', m.group('T')); ty, nm, arr = e
            if count is not None: count[0] += 1
            if not arr and t == re.sub(r'\s+', '', ty): return '%s->%s' % (b, nm)
            return '*(%s *)&%s->%s' % (m.group('T').strip(), b, nm)
        out.append(_MEMBER.sub(sub, chunk))
    return ''.join(out)

PASSES = [('d3d_values', d3d_values), ('members', members)]
def shape_source(text):
    for _, f in PASSES: text = f(text)
    return text


def _objbytes(cxx_flags, src_text, tree, tmp, tag):
    p = os.path.join(tmp, tag + '.cpp'); o = os.path.join(tmp, tag + '.o'); open(p, 'w').write(src_text)
    subprocess.run(['i686-w64-mingw32-g++'] + cxx_flags + ['-I', os.path.join(tree, 'include'), '-c', p, '-o', o], check=True, cwd=tree)
    d = subprocess.run(['i686-w64-mingw32-objdump', '-s', '-j', '.text', '-j', '.rdata', '-j', '.data', '-r', o], capture_output=True, text=True).stdout
    return d.split('\n', 3)[-1]      # (without the line naming the file)

def check(tree, apply):
    """compile each file of tree/src as it is and with the passes applied (apply=True: the tree is not yet shaped) or
    as it is against the same text (apply=False: just counts); report files whose objects differ"""
    import concurrent.futures, tempfile
    flags = [l.strip() for l in open(os.path.join(H, '..', 'sdk', 'fork_flags.txt')) if l.strip() and not l.startswith('-Iinclude')]
    files = sorted(f for f in os.listdir(os.path.join(tree, 'src')) if f.endswith('.cpp')); tmp = tempfile.mkdtemp(prefix='shape_')
    def one(f):
        t = open(os.path.join(tree, 'src', f)).read(); c = [0]; s = t
        set_include(os.path.join(tree, 'include')) if _INC[0] != os.path.join(tree, 'include') else None
        for _, p in PASSES: s = p(s, c)
        if s == t: return f, 0, True
        return f, c[0], _objbytes(flags, t, tree, tmp, f[:-4] + '_a') == _objbytes(flags, s, tree, tmp, f[:-4] + '_b')
    with concurrent.futures.ThreadPoolExecutor(os.cpu_count() or 4) as ex: res = list(ex.map(one, files))
    import shutil; shutil.rmtree(tmp, ignore_errors=True)
    bad = [f for f, n, ok in res if not ok]
    print('shape check: %d files, %d rewritten at %d places; %d compile to different objects%s' % (
        len(files), sum(1 for _, n, _ in res if n), sum(n for _, n, _ in res), len(bad), (': ' + ' '.join(bad[:8])) if bad else ''))
    return not bad

def same(old, new):
    """compile every file of OLD/src and NEW/src, each against its own include/, and compare the objects: a
    regenerated tree that only renames (members, globals) or reshapes must give the same code, data and relocations
    for every file. Files present in one tree only are listed."""
    import concurrent.futures, tempfile, shutil
    flags = [l.strip() for l in open(os.path.join(H, '..', 'sdk', 'fork_flags.txt')) if l.strip() and not l.startswith('-Iinclude')]
    a = set(f for f in os.listdir(os.path.join(old, 'src')) if f.endswith('.cpp')); b = set(f for f in os.listdir(os.path.join(new, 'src')) if f.endswith('.cpp'))
    tmp = tempfile.mkdtemp(prefix='shape_same_')
    def one(f):
        to, tn = open(os.path.join(old, 'src', f)).read(), open(os.path.join(new, 'src', f)).read()
        return f, _objbytes(flags, to, old, tmp, f[:-4] + '_o') == _objbytes(flags, tn, new, tmp, f[:-4] + '_n')
    with concurrent.futures.ThreadPoolExecutor(os.cpu_count() or 4) as ex: res = list(ex.map(one, sorted(a & b)))
    shutil.rmtree(tmp, ignore_errors=True)
    bad = [f for f, ok in res if not ok]
    print('shape same: %d files in both, %d compile to different objects%s; only in the old tree: %s; only in the new: %s' % (
        len(res), len(bad), (': ' + ' '.join(bad[:8])) if bad else '', sorted(a - b)[:8] or 'none', sorted(b - a)[:8] or 'none'))
    return not bad and a == b

if __name__ == '__main__':
    if sys.argv[1:2] == ['same']:
        sys.exit(0 if same(os.path.abspath(sys.argv[2]), os.path.abspath(sys.argv[3])) else 1)
    if sys.argv[1:2] == ['check']:
        sys.exit(0 if check(os.path.abspath(sys.argv[2] if len(sys.argv) > 2 else os.path.join(H, '..', 'tree')), True) else 1)
    print(__doc__)
