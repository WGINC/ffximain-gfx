"""ps2_refs.py -- for every function of the PS2 executable (SCUS_972.66, not stripped), the data objects it
references, read from the executable's own relocations (.relmain, .relDancer.bin). Dancer.bin's code bytes are not in the file: there only the relocation's
symbol is known. The executable is linked, so the instructions hold final addresses: a LO16's target is its
HI16 partner's upper half plus its own signed lower half, a GPREL16's is _gp plus its immediate, an R_MIPS_32's
the stored word. The target is named by the object symbol containing it, with the offset into it. R_MIPS_26
(calls) are listed separately as callees. A relocation against a section symbol (.rodata, .data) gives an address,
named by the object symbol that contains it where one does.

Output {"FUNC": {"addr", "objects": [[name, offset, reloc_type, insn_addr]], "callees": [[name, insn_addr]]}} in address order,
plus "_objects": {name: [addr, size, binding]}.
Usage: python3 ps2_refs.py SCUS_972.66 OUT.json"""
import bisect, json, struct, sys
from elftools.elf.elffile import ELFFile


def main(path, out):
    f = open(path, 'rb'); e = ELFFile(f)
    symtab = e.get_section_by_name('.symtab')
    syms = list(symtab.iter_symbols())
    funcs = sorted((s['st_value'], s['st_value'] + s['st_size'], s.name) for s in syms
                   if s['st_info']['type'] == 'STT_FUNC' and s['st_size'] > 0)
    fstart = [a for a, _, _ in funcs]
    objs = sorted((s['st_value'], s['st_size'], s.name, s['st_info']['bind']) for s in syms
                  if s['st_info']['type'] == 'STT_OBJECT')
    ostart = [a for a, _, _, _ in objs]
    secs = {s.name: s for s in e.iter_sections()}
    main_sec = secs['main']; mbase, mdata = main_sec['sh_addr'], main_sec.data()

    def word(a):
        if mbase <= a < mbase + len(mdata) - 3: return struct.unpack_from('<I', mdata, a - mbase)[0]
        return None

    def owner_obj(a):
        k = bisect.bisect_right(ostart, a) - 1
        if k >= 0 and objs[k][0] <= a < objs[k][0] + max(objs[k][1], 1): return objs[k]
        return None

    gp = next(s['st_value'] for s in syms if s.name == '_gp')
    sx = lambda v: v - 0x10000 if v & 0x8000 else v
    res = {}
    for rs in ('.relmain', '.relDancer.bin'):
        sec = e.get_section_by_name(rs)
        hi = {}                                          # symbol index -> the last R_MIPS_HI16's upper half
        for r in sec.iter_relocations():
            t = r['r_info_type']; off = r['r_offset']
            k = bisect.bisect_right(fstart, off) - 1
            if k < 0 or off >= funcs[k][1]: continue
            fn = funcs[k][2]
            s = syms[r['r_info_sym']]
            rec = res.setdefault(fn, {'addr': '0x%08X' % funcs[k][0], 'objects': [], 'callees': []})
            if t == 4:                                   # R_MIPS_26
                rec['callees'].append([s.name, '0x%08X' % off]); continue
            w = word(off)
            if w is None:                                # Dancer.bin: no code bytes, only the symbol
                if s['st_info']['type'] == 'STT_OBJECT' and t in (2, 6, 7): rec['objects'].append([s.name, None, t, '0x%08X' % off])
                continue
            # a linked executable: the instructions hold the final address
            if t == 5: hi[r['r_info_sym']] = (w & 0xFFFF) << 16; continue
            if t == 6:
                if r['r_info_sym'] not in hi: continue
                tgt = (hi[r['r_info_sym']] + sx(w & 0xFFFF)) & 0xFFFFFFFF
            elif t == 7: tgt = (gp + sx(w & 0xFFFF)) & 0xFFFFFFFF
            elif t == 2: tgt = w
            else: continue
            o = owner_obj(tgt)
            if o: rec['objects'].append([o[2], tgt - o[0], t, '0x%08X' % off])
            else: rec['objects'].append(['@0x%08X' % tgt, 0, t, '0x%08X' % off])
    for v in res.values(): v['objects'].sort(key=lambda x: x[3]); v['callees'].sort(key=lambda x: x[1])
    res['_objects'] = {n: ['0x%08X' % a, sz, b] for a, sz, n, b in objs}
    json.dump(res, open(out, 'w'))
    print('%d functions with references, %d objects' % (len(res) - 1, len(objs)))


if __name__ == '__main__':
    main(*sys.argv[1:3])
