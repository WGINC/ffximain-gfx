"""manifest_for_build.py -- the kit's manifest for another build of FFXiMain.dll (build translation, docs/KIT.md).

    python3 manifest_for_build.py dist/xikit_manifest.txt builds/<stamp>.json OTHER_unpacked.dll dist/xikit_manifest_<stamp>.txt

The kit keeps the analysed build's numbering everywhere. For another build it needs, per function, the bytes its entry
holds there (to check before it writes a jmp) and that build's relocated dwords in that window, and the code ranges
that moved (!shift lines), which it passes to the reconstruction and the mods. builds/<stamp>.json comes from
pipeline/build_translate.py, which proved every manifest function the same code at its range's shift and every
address the reconstruction and speedfix name; this refuses a build file that does not say so.

Per line: RVA, flags, export, name and trampoline (T=) stay; EXPECTED is the other build's 20 bytes from entry-4 at
the translated address (image at 0x10000000, so relocated dwords hold their preferred-base values, as the kit
compares them); RELOCS is the other build's relocation table over that window. A function shorter than the 5-byte
jmp keeps only padding (int3/nop) after it in the other build too, or it is left out and named."""
import json, os, sys
import pefile
HERE = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.join(HERE, '..', '..')
sys.path.insert(0, os.path.join(ROOT, 'pipeline'))
from dump_verify import reloc_blocks
BASE = 0x10000000


def main(man_p, build_p, dll_p, out_p):
    b = json.load(open(build_p))
    if b['failures']['manifest'] or b['failures']['addresses'] or b['manifest_proven'] != b['manifest_functions']:
        sys.exit('manifest_for_build: %s does not settle every function and address' % build_p)
    ranges = [(int(lo, 16), int(hi, 16), d) for lo, hi, d in b['ranges']]
    live = lambda ea: ea + next((d for lo, hi, d in ranges if lo <= ea < hi), 0)
    pe = pefile.PE(dll_p, fast_load=True); img = pe.get_memory_mapped_image()
    if '%08X' % pe.FILE_HEADER.TimeDateStamp != b['timestamp']: sys.exit('manifest_for_build: the DLL is not build %s' % b['timestamp'])
    relocs = set()
    blocks, _ = reloc_blocks(pe, img)
    for page, ents in blocks:
        for e in ents:
            if e >> 12 == 3: relocs.add(BASE + page + (e & 0xFFF))
    ext = json.load(open(os.path.join(ROOT, 'data', 'func_extents.json')))
    out, left = [], []
    for l in open(man_p):
        if l.startswith('#'):
            out.append(l.replace('FFXiMain.dll TimeDateStamp', 'FFXiMain.dll TimeDateStamp %s (translated from' % b['timestamp'], 1) if 'TimeDateStamp' in l else l)
            continue
        if l.startswith('!game_timestamp'):
            out.append('!game_timestamp %s\n!analysed_timestamp %s\n' % (b['timestamp'], b['analysed_timestamp']))
            out += ['!shift %08X %08X %d\n' % r for r in ranges if r[2]]
            continue
        if l.startswith('!') or not l.strip(): out.append(l); continue
        f = l.split(' ')
        ea = BASE + int(f[0], 16); nea = live(ea)
        rs = ext.get('0x%x' % ea, [])
        end = max((e for s, e in rs if s <= ea < e), default=ea + 5)
        if end < ea + 5 and not all(x in (0xCC, 0x90) for x in img[end - ea + nea - BASE:nea + 5 - BASE]):
            left.append('%08X: shorter than the jmp, and not padding after it in this build' % ea); continue
        raw = bytes(img[nea - 4 - BASE:nea + 16 - BASE])
        rel = [r - (nea - 4) for r in sorted(relocs) if nea - 7 <= r <= nea + 15]
        f[3] = raw.hex().upper(); f[4] = ','.join(map(str, rel)) if rel else '-'
        out.append(' '.join(f))
    open(out_p, 'w').write(''.join(out))
    n = sum(1 for x in out if x[:1] not in '#!' and x.strip())
    print('%s: %d functions for build %s (%d code ranges moved)%s' % (out_p, n, b['timestamp'], sum(1 for r in ranges if r[2]),
          '; left out: ' + '; '.join(left) if left else ''))


if __name__ == '__main__':
    if len(sys.argv) != 5: sys.exit(__doc__)
    main(*sys.argv[1:5])
