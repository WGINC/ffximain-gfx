"""d3dx_data.py -- the names of the D3DX 8.1 data the statically linked D3DX functions use, from d3dx8.lib itself.

Every D3DX function in the game was byte-matched to d3dx8.lib with its relocated fields masked (libmatch.py,
data/d3dx/d3dx81_manifest.json). Each of those masked fields has a relocation in the library naming its target: a
symbol, or a section plus the offset stored in the field (IMAGE_REL_I386_DIR32, the addend is the stored word). At
the same offset in the game's copy, the relocated dword is the address the linker gave that target. So
  game address = game dword - addend, named by the library's symbol (for a section target: the symbol of that
  section that contains the offset; a static without its own symbol keeps the section's name and offset).
A target that two references place at two different game addresses is reported, not named.
Usage: python3 d3dx_data.py d3dx8.lib OUT.json     (uses $FFXI_DLL)"""
import collections, json, os, struct, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pefile
from libmatch import ar_members

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')


def parse(obj):
    machine, nsec, _, symptr, nsym, opt, _ = struct.unpack_from('<HHIIIHH', obj, 0)
    if machine != 0x14C: return None
    secs = []
    for i in range(nsec):
        o = 20 + opt + 40 * i
        name, vsize, va, rsize, rptr, relptr, _, nrel, _, ch = struct.unpack_from('<8sIIIIIIHHI', obj, o)
        secs.append(dict(name=name.rstrip(b'\0').decode('latin1'), size=rsize, ptr=rptr, relptr=relptr, nrel=nrel, ch=ch))
    strtab = symptr + 18 * nsym; syms = {}; i = 0
    while i < nsym:
        o = symptr + 18 * i
        raw, value, secnum, typ, cls, naux = struct.unpack_from('<8sIhHBB', obj, o)
        if raw[:4] == b'\0\0\0\0':
            so = struct.unpack_from('<I', raw, 4)[0]; e = obj.index(b'\0', strtab + so); nm = obj[strtab + so:e].decode('latin1')
        else: nm = raw.rstrip(b'\0').decode('latin1')
        syms[i] = (nm, value, secnum, typ, cls, naux)
        i += 1 + naux
    return secs, syms


def main(lib, out):
    pe = pefile.PE(os.environ['FFXI_DLL'], fast_load=True); base = pe.OPTIONAL_HEADER.ImageBase
    img = pe.get_memory_mapped_image()
    man = json.load(open(os.path.join(D, 'd3dx', 'd3dx81_manifest.json')))
    objs = {}
    for mname, body in ar_members(open(lib, 'rb').read()):
        p = parse(body) if len(body) > 20 else None
        if p: objs.setdefault(mname.split('\\')[-1].lower(), []).append((body, p))
    found = collections.defaultdict(set); n = 0
    for ga, m in man.items():
        ga = int(ga, 16)
        for body, (secs, syms) in objs.get((m.get('object') or '').lower(), []):
            fs = [(s, v) for s, v in syms.items() if v[0] == m['symbol'] and v[2] > 0]
            if not fs: continue
            _, (fn, fval, fsec, _, _, _) = fs[0]
            s = secs[fsec - 1]
            ends = sorted(v[1] for v in syms.values() if v[2] == fsec and v[1] > fval and v[4] in (2, 3))
            fend = ends[0] if ends else s['size']
            for r in range(s['nrel']):
                va, si, rt = struct.unpack_from('<IIH', body, s['relptr'] + 10 * r)
                if rt != 6 or not fval <= va < fend: continue
                tn, tval, tsec, ttyp, tcls, _ = syms[si]
                add = struct.unpack_from('<I', body, s['ptr'] + va)[0]
                gw = struct.unpack_from('<I', img, ga + (va - fval) - base)[0]
                if tsec > 0 and tcls == 3 and tval == 0 and tn.startswith('.'):     # a section symbol: find the static inside it
                    inside = sorted((v[1], v[0]) for v in syms.values() if v[2] == tsec and not v[0].startswith('.') and v[1] <= add)
                    nm, off = (inside[-1][1], add - inside[-1][0]) if inside else ('%s+0x%X' % (tn, add), 0)
                    found[(m['object'], nm)].add(gw - off)
                else:
                    found[(m['object'], tn)].add(gw - add)
                n += 1
    res = {}; conflicts = []
    for (o, nm), addrs in found.items():
        if len(addrs) != 1: conflicts.append([o, nm, sorted('0x%08X' % a for a in addrs)]); continue
        a = next(iter(addrs))
        if not base <= a < base + pe.OPTIONAL_HEADER.SizeOfImage: continue
        sec = next((s.Name.rstrip(b'\0').decode() for s in pe.sections if s.VirtualAddress <= a - base < s.VirtualAddress + max(s.Misc_VirtualSize, s.SizeOfRawData)), None)
        if sec == '.text': continue               # code targets are the functions themselves
        res.setdefault('0x%08X' % a, []).append({'symbol': nm, 'object': o, 'section': sec})
    json.dump({'data': res, 'conflicts': conflicts}, open(out, 'w'), indent=1, sort_keys=True)
    print('%d relocations read; %d data addresses named; %d conflicts' % (n, len(res), len(conflicts)))


if __name__ == '__main__':
    main(*sys.argv[1:3])
