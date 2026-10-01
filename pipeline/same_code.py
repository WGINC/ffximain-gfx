"""same_code.py -- are two builds of the clean tree the same code, function by function?

Compiles every src/*.cpp of both trees with -ffunction-sections, then compares each function's
section: bytes (relocation sites masked) plus the relocation offsets, types and target symbols.
Moving a function to another file, or reordering files, leaves its section unchanged, so gate
verdicts from one build carry over to the other for every function reported identical.

Usage: python3 same_code.py TREE_A TREE_B [out.json] [--ignore-names]
"""
import json, os, struct, subprocess, sys, tempfile, collections

FLAGS = ['-ffunction-sections', '-O2', '-mfpmath=387', '-march=i686', '-ffreestanding', '-fno-builtin',
         '-fno-asynchronous-unwind-tables', '-fexcess-precision=standard', '-fno-exceptions', '-fno-rtti',
         '-std=c++17', '-fno-strict-aliasing', '-fwrapv', '-fno-delete-null-pointer-checks', '-fpermissive', '-w',
         '-DXI_API=__attribute__((dllexport))']

def coff_sections(path):
    b = open(path, 'rb').read()
    _, nsec, _, symptr, nsym, opt, _ = struct.unpack_from('<HHIIIHH', b, 0)
    strtab = symptr + nsym * 18
    def cstr(o): return b[strtab + o:b.index(b'\0', strtab + o)].decode()
    syms, i = [], 0
    while i < nsym:
        raw = b[symptr + i * 18: symptr + i * 18 + 18]
        nm = cstr(struct.unpack_from('<I', raw, 4)[0]) if raw[:4] == b'\0\0\0\0' else raw[:8].rstrip(b'\0').decode()
        naux = raw[17]; syms.append(nm); syms += [None] * naux; i += 1 + naux
    out = {}
    for s in range(nsec):
        h = b[20 + opt + s * 40: 60 + opt + s * 40]
        nm = h[:8].rstrip(b'\0').decode()
        if nm.startswith('/'): nm = cstr(int(nm[1:]))
        _, _, sz, ptr, rptr, _, nrel, _, _ = struct.unpack_from('<IIIIIIHHI', h, 8)
        if not nm.startswith('.text') or not sz: continue
        data = bytearray(b[ptr:ptr + sz]); rels = []
        for r in range(nrel):
            a, si, t = struct.unpack_from('<IIH', b, rptr + r * 10)
            rels.append((a, t, syms[si]))
            if t in (6, 20): data[a:a + 4] = b'\0\0\0\0'
        out[nm] = (bytes(data), tuple(rels))
    return out

def build(tree, tmp):
    d = os.path.join(tmp, os.path.basename(os.path.abspath(tree)) + str(abs(hash(tree)))); os.makedirs(d)
    secs = {}
    for f in sorted(os.listdir(os.path.join(tree, 'src'))):
        o = os.path.join(d, f + '.o')
        subprocess.run(['i686-w64-mingw32-g++'] + FLAGS + ['-I', os.path.join(tree, 'include'), '-c',
                        os.path.join(tree, 'src', f), '-o', o], check=True)
        for k, v in coff_sections(o).items():
            if k in ('.text',): continue
            secs[k] = v
    return secs

def by_address(tree, secs):
    """re-key sections by the game address of the function they hold (placement.json symbols), so
    two builds compare even when functions were renamed; relocation targets compare by kind only"""
    place = json.load(open(os.path.join(tree, 'placement.json')))
    sym2ea = {v['symbol']: k for k, v in place.items() if v.get('symbol')}
    names = list(secs)
    dem = subprocess.run(['i686-w64-mingw32-c++filt', '-n'], input='\n'.join(n.split('$', 1)[-1] for n in names),
                         capture_output=True, text=True).stdout.split('\n')
    out = {}
    for n, d in zip(names, dem):
        key = d.split('(')[0].strip()
        ea = sym2ea.get(key) or sym2ea.get(key.lstrip('_').split('@')[0])
        data, rels = secs[n]
        out[ea or n] = (data, tuple((a, t_) for a, t_, _ in rels))
    return out

def main(a, b, outf=None, ignore_names=False):
    with tempfile.TemporaryDirectory() as tmp:
        A, B = build(a, tmp), build(b, tmp)
    if ignore_names:
        A, B = by_address(a, A), by_address(b, B)
    same = sorted(k for k in A.keys() & B.keys() if A[k] == B[k])
    diff = sorted(k for k in A.keys() & B.keys() if A[k] != B[k])
    res = {'same': len(same), 'different': diff, 'only_a': sorted(A.keys() - B.keys()), 'only_b': sorted(B.keys() - A.keys())}
    print('identical %d, different %d, only in A %d, only in B %d' % (len(same), len(diff), len(res['only_a']), len(res['only_b'])))
    if outf: json.dump(res, open(outf, 'w'), indent=0)
    return res

if __name__ == '__main__':
    args = [x for x in sys.argv[1:] if x != '--ignore-names']
    main(*args[:3], ignore_names='--ignore-names' in sys.argv)
