"""reloc_literals.py -- the addresses each graphics function's own code holds as relocated immediates.

The relocation table lists every place where the loader rebases an absolute address. That table is how
dump_verify.py relocates the code, and the result matches the running game byte for byte. So an
instruction operand covered by a relocation entry holds an address, whatever Hex-Rays prints for it. Where
the pseudocode shows such a value as a plain integer (`(int)v32 < 271942420`, a loop bound compared as an
integer), hx2clean.py writes it as `(int)XI_GAME(0x10358314u)`. That compiles to the same constant in the
verified build and is rebased in the in-game build.

A value is listed for a function only when every immediate or displacement in the function with that
value is relocated. If one occurrence is relocated and another is not, the plain integer could be either,
and the value is left out (counted as `ambiguous`).

    python3 reloc_literals.py data/reloc_literals.json        uses $FFXI_DLL
Output: {"0xFUNC": ["0x10358314", ...], "_stats": {...}}
"""
import collections, json, os, sys
import pefile
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sp_solve as S
import dump_verify as D_V

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')


def main(out):
    dll = os.environ['FFXI_DLL']
    pe = pefile.PE(dll, fast_load=True); base = pe.OPTIONAL_HEADER.ImageBase
    # the whole chain of the .reloc section (dump_verify.reloc_blocks: the header's directory covers only its
    # end, not .text; the chain is the one dump_verify checks byte for byte against the running game)
    blocks, _ = D_V.reloc_blocks(pe, pe.get_memory_mapped_image())
    rel = {base + page + (e & 0xFFF) for page, ents in blocks for e in ents if e >> 12 == 3}     # HIGHLOW
    im = S.Image(dll)
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    want = {json.loads(l)['ea'] for l in open(os.path.join(D, 'exports', 'gfx_export.jsonl'))}
    want |= {int(k, 16) for k in json.load(open(os.path.join(D, 'gfx_device_users.json')))['functions']}
    stats = collections.Counter(); res = {}
    for k, e in ext.items():
        f = int(k, 16)
        if f not in want: continue
        seen = collections.defaultdict(set)          # value -> {relocated?}
        for a, i in im.code(e).items():
            b = bytes(im.img[a - im.base:a - im.base + i.size])
            # every 4-byte field of the instruction: an immediate or a displacement (capstone gives their offsets)
            for off, size in ((i.disp_offset, i.disp_size), (i.imm_offset, i.imm_size)):
                if size != 4 or not off: continue
                v = int.from_bytes(b[off:off + 4], 'little')
                seen[v].add(a + off in rel)
        vals = sorted(v for v, r in seen.items() if r == {True})
        stats['ambiguous'] += sum(1 for r in seen.values() if r == {True, False})
        if vals: res['0x%08X' % f] = ['0x%08X' % v for v in vals]; stats['values'] += len(vals)
    res['_stats'] = dict(stats, functions=len(res))
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print(res['_stats'])


if __name__ == '__main__':
    main(sys.argv[1])
