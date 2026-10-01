"""libmatch.py -- identify statically linked library functions in FFXiMain.dll by byte comparison
against a COFF static library (.lib), with every relocated field masked.

For each function symbol defined in the library: its section bytes from the symbol's offset to the
next symbol (or section end), with relocation sites masked. A game function matches when every
unmasked byte is equal. Candidates: (1) the game function carrying the same name, (2) every game
function start (exhaustive search) for library functions not found by name.

Usage: python3 libmatch.py game.dll lib.lib ida_functions.json out.json [--min 12]
"""
import json, struct, sys, collections

def ar_members(data):
    assert data[:8] == b'!<arch>\n'
    pos = 8; longnames = b''
    while pos + 60 <= len(data):
        h = data[pos:pos + 60]; name = h[:16].decode('latin1').strip(); size = int(h[48:58].decode().strip())
        body = data[pos + 60: pos + 60 + size]
        if name == '//': longnames = body
        elif name not in ('/', ''):
            if name.startswith('/') and name[1:].isdigit():
                off = int(name[1:]); end = off
                while end < len(longnames) and longnames[end:end + 1] not in (b'\n', b'\x00'): end += 1
                name = longnames[off:end].decode('latin1').rstrip('/')
            yield name.rstrip('/'), body
        pos += 60 + size + (size & 1)

def coff_functions(obj):
    """[(symbol, bytes, mask)] for function symbols in an i386 COFF object"""
    if len(obj) < 20: return []
    machine, nsec, _, symptr, nsym, opt, _ = struct.unpack_from('<HHIIIHH', obj, 0)
    if machine != 0x14C: return []
    secs = []
    for i in range(nsec):
        o = 20 + opt + 40 * i
        name, vsize, va, rsize, rptr, relptr, _, nrel, _, ch = struct.unpack_from('<8sIIIIIIHHI', obj, o)
        secs.append(dict(name=name.rstrip(b'\0'), size=rsize, ptr=rptr, relptr=relptr, nrel=nrel, ch=ch))
    strtab = symptr + 18 * nsym
    syms = []; i = 0
    while i < nsym:
        o = symptr + 18 * i
        raw, value, secnum, typ, cls, naux = struct.unpack_from('<8sIhHBB', obj, o)
        if raw[:4] == b'\0\0\0\0':
            so = struct.unpack_from('<I', raw, 4)[0]; e = obj.index(b'\0', strtab + so); nm = obj[strtab + so:e].decode('latin1')
        else:
            nm = raw.rstrip(b'\0').decode('latin1')
        syms.append((nm, value, secnum, typ, cls))
        i += 1 + naux
    out = []
    bysec = collections.defaultdict(list)
    for nm, value, secnum, typ, cls in syms:
        if secnum > 0 and cls == 2 and (typ & 0x20 or secs[secnum - 1]['ch'] & 0x20):   # external, function / code section
            bysec[secnum].append((value, nm))
    for secnum, lst in bysec.items():
        s = secs[secnum - 1]
        if not s['ch'] & 0x20 or s['ptr'] == 0: continue
        data = obj[s['ptr']: s['ptr'] + s['size']]
        mask = bytearray(b'\1' * len(data))
        for r in range(s['nrel']):
            va, si, rt = struct.unpack_from('<IIH', obj, s['relptr'] + 10 * r)
            for k in range(4):
                if va + k < len(mask): mask[va + k] = 0
        lst.sort()
        for j, (value, nm) in enumerate(lst):
            end = lst[j + 1][0] if j + 1 < len(lst) else len(data)
            out.append((nm, data[value:end], bytes(mask[value:end])))
    return out

def trim(b, m):
    """drop trailing int3/nop padding the compiler adds after a function in its section"""
    n = len(b)
    while n and b[n - 1] in (0xCC, 0x90) and m[n - 1]: n -= 1
    return b[:n], m[:n]

def main(game, lib, idaf, out, minlen=12):
    import pefile
    pe = pefile.PE(game); img = bytes(pe.get_memory_mapped_image()); B = pe.OPTIONAL_HEADER.ImageBase
    funcs = json.load(open(idaf))
    starts = sorted(r['ea'] for r in funcs); names = {r['name']: r['ea'] for r in funcs}
    libf = []
    for mname, body in ar_members(open(lib, 'rb').read()):
        for nm, b, m in coff_functions(body):
            b, m = trim(b, m)
            if len(b) >= 4: libf.append((nm, mname, b, m))
    def match_at(ea, b, m):
        o = ea - B
        if o < 0 or o + len(b) > len(img): return False
        g = img[o:o + len(b)]
        return all(not mk or x == y for x, y, mk in zip(g, b, m))
    # index game starts by first unmasked bytes for the exhaustive search
    found = {}; byname = 0
    for nm, mem, b, m in libf:
        cand = [names[k] for k in (nm, nm.lstrip('_')) if k in names]
        hit = [ea for ea in cand if match_at(ea, b, m)]
        if hit: found[hit[0]] = (nm, mem, len(b), 'name+bytes'); byname += 1
    for nm, mem, b, m in libf:
        if len(b) < minlen or any(v[0] == nm for v in found.values()): continue
        for ea in starts:
            if ea in found: continue
            if match_at(ea, b, m):
                found[ea] = (nm, mem, len(b), 'bytes'); break
    res = {hex(ea): {'symbol': v[0], 'object': v[1], 'bytes': v[2], 'how': v[3]} for ea, v in sorted(found.items())}
    json.dump(res, open(out, 'w'), indent=0)
    print('library functions', len(libf), '| matched in game:', len(found), '(by name', byname, ')')

if __name__ == '__main__':
    a = sys.argv[1:]
    main(a[0], a[1], a[2], a[3], int(a[5]) if len(a) > 5 else 12)
