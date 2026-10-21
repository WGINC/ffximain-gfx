"""msvc_match.py -- does this source compile, with the game's own compiler, to the game's own bytes?

FFXiMain.dll's Rich header names the compiler of the game's objects: prodid 48/49 (C/C++), build 9044, which is
Visual C++ 6.0's Processor Pack code generator (c2.dll 13.0.9044; a test compile stamps exactly that @comp.id). The
compiler is never in this repository: MSVC6_PP names a local copy's bin directory (CL.EXE, C1.DLL, C1XX.DLL,
c2.dll), run under wine.

A function's source is compiled to an object (one COMDAT per function, /Gy), and the function's bytes are compared
with the DLL's bytes at its address. A field the object relocates (an address: a global, a called function, a jump
table) holds the linker's value in the DLL, so it is compared by its target, not its bytes: the caller supplies
{symbol: address} for the symbols the source names, and each relocation's target must be that address
(REL32: the DLL's field is target - (field + 4); DIR32: the DLL's field is the target itself). A compiler-made
constant (`__real@...`, a floating literal) needs no address: the bytes the object defines for it must be the
bytes at the address the DLL's field holds.

    MSVC6_PP=... FFXI_DLL=... python3 msvc_match.py SOURCE.cpp SYMBOL 0xADDR [SYMS.json] [-- CL options]
prints MATCH, or the first differing byte and both listings."""
import json, os, re, struct, subprocess, sys, tempfile
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)

DEFAULT_OPTS = ['/O2']


def compile_obj(src_text, opts, workdir=None, name='m'):
    d = workdir or tempfile.mkdtemp(prefix='msvc_')
    src = os.path.join(d, name + ('.c' if opts and '/Tc' in opts else '.cpp'))
    open(src, 'w').write(src_text)
    env = dict(os.environ, WINEDEBUG='-all')
    env.setdefault('WINEPREFIX', os.path.join(os.path.dirname(os.environ['MSVC6_PP'].rstrip('/')), '..', 'wineprefix'))
    r = subprocess.run(['wine', os.path.join(os.environ['MSVC6_PP'], 'CL.EXE'), '/nologo', '/c', '/Gy'] + opts
                       + ['/Fo' + name + '.obj', os.path.basename(src)], cwd=d, capture_output=True, text=True, env=env)
    obj = os.path.join(d, name + '.obj')
    return (obj if os.path.exists(obj) else None), r.stdout + r.stderr


def coff_function(obj, symbol):
    """(bytes, [(offset, kind, target symbol)]) of the COMDAT section defining symbol (an external, as the compiler
    decorates it, or its undecorated prefix)"""
    b = open(obj, 'rb').read()
    nsec = struct.unpack_from('<H', b, 2)[0]; symptr, nsym = struct.unpack_from('<II', b, 8)
    opt = struct.unpack_from('<H', b, 16)[0]
    secs = [struct.unpack_from('<8sIIIIIIHHI', b, 20 + opt + 40 * i) for i in range(nsec)]
    strtab = symptr + 18 * nsym
    def sname(e):
        if e[:4] == b'\0\0\0\0':
            o = struct.unpack_from('<I', e, 4)[0]; return b[strtab + o:b.index(b'\0', strtab + o)].decode()
        return e[:8].split(b'\0')[0].decode()
    syms, i = [], 0
    while i < nsym:
        e = b[symptr + 18 * i:symptr + 18 * i + 18]
        val, sec, typ, cls, naux = struct.unpack_from('<IhHBB', e, 8)
        syms.append((sname(e), val, sec, cls)); syms += [None] * naux; i += 1 + naux
    hit = [s for s in syms if s and (s[0] == symbol or s[0].startswith(symbol)) and s[2] > 0 and s[3] == 2]
    if not hit: return None
    name, val, sec, _ = hit[0]
    s = secs[sec - 1]; size, rawptr, relptr, nrel = s[3], s[4], s[5], s[7]
    data = b[rawptr:rawptr + size]
    rels = []
    for k in range(nrel):
        off, si, typ = struct.unpack_from('<IIH', b, relptr + 10 * k)
        rels.append((off, {0x14: 'rel32', 0x06: 'dir32'}.get(typ, 'type%d' % typ), syms[si][0] if syms[si] else '?'))
    consts = {}
    for t in {r[2] for r in rels}:
        if t.startswith('__real@'):
            for sy in syms:
                if sy and sy[0] == t and sy[2] > 0:
                    cs = secs[sy[2] - 1]; consts[t] = b[cs[4] + sy[1]:cs[4] + cs[3]]
    coff_function.consts = consts
    return data[val:], rels


def match(src_text, symbol, ea, symaddr, opts=None):
    """(True, '') or (False, why)"""
    import sp_solve as S
    obj, log = compile_obj(src_text, opts or DEFAULT_OPTS)
    if not obj: return False, 'compile failed:\n' + log
    f = coff_function(obj, symbol)
    if not f: return False, 'symbol %s not in the object' % symbol
    data, rels = f
    im = S.Image(os.environ['FFXI_DLL'])
    orig = bytes(im.img[ea - im.base:ea - im.base + len(data)])
    relat = {o: (k, t) for o, k, t in rels}
    i = 0
    while i < len(data):
        if i in relat:
            k, t = relat[i]
            got = struct.unpack_from('<I', orig, i)[0]
            cv = coff_function.consts.get(t)
            if cv is not None and k == 'dir32':
                if bytes(im.img[got - im.base:got - im.base + len(cv)]) != cv:
                    return False, 'byte %d: constant %s is not the DLL\'s bytes at 0x%08X' % (i, t, got)
                i += 4; continue
            if t not in symaddr: return False, 'byte %d: relocation to %s, whose address is not given' % (i, t)
            want = symaddr[t] if k == 'dir32' else (symaddr[t] - (ea + i + 4)) & 0xFFFFFFFF
            if k not in ('rel32', 'dir32') or got != want:
                return False, 'byte %d: %s to %s: DLL holds 0x%08X, expected 0x%08X' % (i, k, t, got, want)
            i += 4; continue
        if data[i] != orig[i]:
            return False, 'byte %d differs: compiled %s / DLL %s' % (i, data[i:i + 8].hex(), orig[i:i + 8].hex())
        i += 1
    return True, '%d bytes, %d relocations' % (len(data), len(rels))


if __name__ == '__main__':
    a = sys.argv[1:]
    opts = a[a.index('--') + 1:] if '--' in a else None
    a = a[:a.index('--')] if '--' in a else a
    syms = {k: int(v, 16) for k, v in json.load(open(a[3])).items()} if len(a) > 3 else {}
    ok, why = match(open(a[0]).read(), a[1], int(a[2], 16), syms, opts)
    print('MATCH' if ok else 'DIFFER', why)
