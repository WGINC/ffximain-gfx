"""reloc_literals.py -- the addresses each function's own code holds as relocated immediates.

The relocation table lists every place where the loader rebases an absolute address. That table is how
dump_verify.py relocates the code, and the result matches the running game byte for byte. So an
instruction operand covered by a relocation entry holds an address, whatever Hex-Rays prints for it. Where
the pseudocode shows such a value as a plain integer (`(int)v32 < 271942420`, a loop bound compared as an
integer), hx2clean.py writes it as `(int)XI_GAME(0x10358314u)`. That compiles to the same constant in the
verified build and is rebased in the in-game build.

A call's return address is listed too: the address of the instruction after a `call` in the function's own code.
The call pushes it, Ghidra writes that push as a literal (`local_8 = 0x1026d0bb;`), and the original never holds it as
data. It is a code address, by the decoded code, so XI_GAME is right for it (1 Nov: 9 such stores, flagged by
raw_game_addrs.py on the probe build).

A value is listed for a function only when every immediate or displacement in the function with that
value is relocated (a return address: when no immediate or displacement in it has that value unrelocated). If one occurrence is relocated and another is not, the plain integer could be either,
and the value is left out (counted as `ambiguous`).

A comparison bound is a third case: the decompiler prints `x >= R` as `R - 1 < x` (and `x <= R` as `x < R + 1`), and
folds `R - n - 1` into `~n + (R - 1)`, so the literal is a relocated value of the function plus or minus one
(1 Nov: `0x1048701f < (int)puVar6` in 0x10095900, whose code compares with the relocated 0x10487020). Such a literal
L is listed in BOUNDS as L -> R when the function's code holds L nowhere and exactly one of L-1, L+1 as a relocated
value only; hx2clean.py writes it as `XI_GAME(R) - 1` (or `+ 1`), the same constant in the verified build.

    python3 reloc_literals.py data/reloc_literals.json [data/reloc_bounds.json]       uses $FFXI_DLL
Output: {"0xFUNC": ["0x10358314", ...], "_stats": {...}}; BOUNDS {"0xFUNC": {"0x1048701F": "0x10487020"}}
"""
import collections, json, os, sys
import pefile
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sp_solve as S
import dump_verify as D_V

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')


def main(out, bounds_out=None):
    dll = os.environ['FFXI_DLL']
    pe = pefile.PE(dll, fast_load=True); base = pe.OPTIONAL_HEADER.ImageBase
    # the whole chain of the .reloc section (dump_verify.reloc_blocks: the header's directory covers only its
    # end, not .text; the chain is the one dump_verify checks byte for byte against the running game)
    blocks, _ = D_V.reloc_blocks(pe, pe.get_memory_mapped_image())
    rel = {base + page + (e & 0xFFF) for page, ents in blocks for e in ents if e >> 12 == 3}     # HIGHLOW
    im = S.Image(dll)
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    # every function with an extent (until 1 Nov only the graphics export and the device users were listed: the
    # tree had grown past them, and 89 in-game functions held addresses written as plain integers, raw_game_addrs.py)
    stats = collections.Counter(); res = {}; bounds = {}
    for k, e in ext.items():
        if k.startswith('_'): continue
        f = int(k, 16)
        seen = collections.defaultdict(set)          # value -> {relocated?}
        code = im.code(e)
        rets = {a + i.size for a, i in code.items() if i.mnemonic == 'call'}
        for a, i in code.items():
            b = bytes(im.img[a - im.base:a - im.base + i.size])
            # every 4-byte field of the instruction: an immediate or a displacement (capstone gives their offsets)
            for off, size in ((i.disp_offset, i.disp_size), (i.imm_offset, i.imm_size)):
                if size != 4 or not off: continue
                v = int.from_bytes(b[off:off + 4], 'little')
                seen[v].add(a + off in rel)
        vals = {v for v, r in seen.items() if r == {True}}
        stats['ambiguous'] += sum(1 for r in seen.values() if r == {True, False})
        ra = {v for v in rets if False not in seen.get(v, ())}
        stats['return_addresses'] += len(ra - vals)
        if True:                  # counted in every run, so the stats are the same with or without BOUNDS
            b = {}
            for L in {v + d for v in vals for d in (-1, 1)} - set(seen):
                rs = [L + d for d in (-1, 1) if L + d in vals]
                if len(rs) == 1: b['0x%08X' % L] = '0x%08X' % rs[0]
                else: stats['bounds_ambiguous'] += 1
            if b: bounds['0x%08X' % f] = b; stats['bounds'] += len(b)
        vals = sorted(vals | ra)
        if vals: res['0x%08X' % f] = ['0x%08X' % v for v in vals]; stats['values'] += len(vals)
    res['_stats'] = dict(stats, functions=len(res))
    if bounds_out:
        bounds['_doc'] = 'reloc_literals.py: a literal one off a relocated value of the function (a comparison bound): literal -> that value'
        json.dump(bounds, open(bounds_out, 'w'), indent=0, sort_keys=True)
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print(res['_stats'])


if __name__ == '__main__':
    main(*sys.argv[1:3])
