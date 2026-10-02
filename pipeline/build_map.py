"""build_map.py -- map addresses between two builds of FFXiMain.dll (functions and globals).

A memory dump of the running game may come from another build than the one IDA analysed (the dump
from 7 Sep 2026 holds the 5 Aug build; the IDB is of the 3 Sep build). Most code is unchanged but
shifted. Each function of the analysed build is looked up in the other build's code by its bytes with
every absolute address and call/jump displacement masked; where the whole function matches, each
absolute operand gives a pair (address in the analysed build, address in the other). Pairs that
disagree are dropped.

    python3 build_map.py ANALYSED.dll OTHER_IMAGE.bin OTHER_BASE OUT.json
OTHER_IMAGE.bin: the other build's mapped image (dump_image.py), loaded at OTHER_BASE. Addresses in
the output are in each build's own numbering at image base 0x10000000.
Output: {"functions": {"0xANALYSED": "0xOTHER"}, "globals": {"0xANALYSED": "0xOTHER"}, "stats": {...}}
"""
import bisect, collections, json, os, re, sys
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
IMG = (0x10000000, 0x10BE2000)


def masked(md, code, ea):
    """(pattern bytes with masked positions as None, [(offset, analysed absolute value)])"""
    pat = list(code); absr = []
    for i in md.disasm(code, ea):
        o = i.address - ea
        if i.mnemonic in ('call', 'jmp') or i.mnemonic.startswith('j'):
            if i.operands and i.operands[0].type == X86_OP_IMM and i.size >= 5:
                for k in range(i.size - 4, i.size): pat[o + k] = None
        # absolute addresses: displacement / immediate inside the image
        for k in range(o, o + i.size - 3):
            v = int.from_bytes(code[k:k + 4], 'little')
            if IMG[0] <= v < IMG[1] and k + 4 <= o + i.size and pat[k] is not None:
                for j in range(4): pat[k + j] = None
                absr.append((k, v))
    return pat, absr


def main(dll, other, other_base, out):
    other_base = int(other_base, 16)
    pe = pefile.PE(dll, fast_load=True); base = pe.OPTIONAL_HEADER.ImageBase; img = pe.get_memory_mapped_image()
    oimg = open(other, 'rb').read()
    rel = other_base - 0x10000000                       # the other image is relocated by this much
    md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    funcs = sorted((int(k, 16), r) for k, r in ext.items() if len(r) == 1 and r[0][1] - r[0][0] >= 8)
    fmap, gpairs = {}, collections.defaultdict(collections.Counter)
    shift = 0                                           # last seen displacement between the builds
    stats = collections.Counter()
    for ea, ((a, b),) in funcs:
        code = img[a - base:b - base]
        pat, absr = masked(md, code, a)
        rx = re.compile(b''.join(re.escape(bytes([c])) if c is not None else b'.' for c in pat), re.S)
        found = None
        for lo, hi in ((shift - 0x400, shift + 0x400), (shift - 0x20000, shift + 0x20000), (-0x200000, 0x200000)):
            s = max(0, a - 0x10000000 + lo); e = min(len(oimg), a - 0x10000000 + hi + len(code))
            ms = [m.start() for m in rx.finditer(oimg, s, e)]
            if len(ms) == 1: found = ms[0]; break
            if len(ms) > 1:
                near = min(ms, key=lambda x: abs(x - (a - 0x10000000 + shift)))
                found = near; stats['ambiguous_nearest'] += 1; break
        if found is None: stats['unmatched'] += 1; continue
        stats['matched'] += 1
        oea = 0x10000000 + found; shift = oea - a
        fmap['0x%08X' % a] = '0x%08X' % oea
        for k, v in absr:
            ov = int.from_bytes(oimg[found + k:found + k + 4], 'little') - rel
            gpairs[v][ov] += 1
    gmap = {}
    for v, c in gpairs.items():
        (ov, n), = c.most_common(1)
        if len(c) == 1: gmap['0x%08X' % v] = '0x%08X' % ov
        else: stats['conflicting_global'] += 1
    stats['globals'] = len(gmap); stats['functions_total'] = len(funcs)
    json.dump({'functions': fmap, 'globals': gmap, 'stats': dict(stats)}, open(out, 'w'), indent=0, sort_keys=True)
    print(dict(stats))


if __name__ == '__main__':
    main(*sys.argv[1:5])
