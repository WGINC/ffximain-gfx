"""differs.py -- why the sweep's DIFFERS functions differ, grouped by the shape of the difference.

    FFXI_DLL=... python3 pipeline/vc6/differs.py SWEEP.json KEEP_DIR OUT.json
SWEEP.json and KEEP_DIR are sweep.py's output and its --keep folder (the objects). For each DIFFERS function both
instruction streams are disassembled (capstone, 32-bit), ours from the object and the game's from the DLL over the
function's extent (data/func_extents.json), and compared at three levels:
  * OPERANDS: the same mnemonics in the same order; only operands differ (registers, displacements, immediates);
  * ORDER: the same multiset of instructions (mnemonic and operands, with every call/jump target and relocated
    operand blanked), in another order;
  * MNEMONICS: the same mnemonics in another order, the operands also differing;
  * otherwise SHAPE, with what the game has that ours lacks and the reverse (mnemonic counts).
Each class is what the bytes show, not a cause: which source change turns one into the other is for match.py to
prove on each function. Also counted: whether the game's function has a frame (`push ebp; mov ebp, esp`) where ours
has none, and the reverse, and the length difference.
"""
import collections, json, os, re, sys
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import match as M
from sweep import SHIM_HELPER
import capstone, pefile


def insns(code, ea):
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    out = []
    for i in md.disasm(bytes(code), ea):
        ops = i.op_str
        if i.mnemonic.startswith(('call', 'j', 'loop')): ops = '<t>'
        ops = re.sub(r'0x1[0-9a-f]{7}', '<a>', ops)      # absolute addresses: relocated, compared by match.py
        out.append((i.mnemonic, ops))
    return out


def frame(ins):
    return len(ins) >= 2 and ins[0] == ('push', 'ebp') and ins[1] == ('mov', 'ebp, esp')


def main():
    sweep, keep, out = sys.argv[1:4]
    res = json.load(open(sweep))
    img = pefile.PE(os.environ['FFXI_DLL'], fast_load=True).get_memory_mapped_image()
    ext = json.load(open(os.path.join(M.ROOT, 'data', 'func_extents.json')))
    rows, cls = {}, collections.Counter()
    lacks, extra = collections.Counter(), collections.Counter()
    for k, v in sorted(res.items()):
        if v['verdict'] != 'DIFFERS': continue
        ea = int(k, 16)
        obj = [f for f in (os.path.join(keep, 'f_%08X.obj' % ea),) if os.path.exists(f)]
        e = ext.get(k) or ext.get(k.lower()) or ext.get('0x%08x' % ea)
        if not obj or not e or len(e) != 1: rows[k] = {'class': 'UNREAD'}; cls['UNREAD'] += 1; continue
        funcs, _ = M.coff(obj[0])
        funcs = {k_: v_ for k_, v_ in funcs.items() if not SHIM_HELPER.match(k_)}   # the shim's helpers beside it (sweep.py)
        if len(funcs) != 1: rows[k] = {'class': 'UNREAD'}; cls['UNREAD'] += 1; continue
        (_, (code, rels)), = funcs.items()
        n = e[0][1] - e[0][0]
        game = img[ea - M.BASE:ea - M.BASE + n]
        a, b = insns(code, ea), insns(game, ea)
        while a and a[-1][0] in ('int3', 'nop'): a.pop()
        while b and b[-1][0] in ('int3', 'nop'): b.pop()
        if [m for m, _ in a] == [m for m, _ in b]: c = 'OPERANDS'
        elif collections.Counter(a) == collections.Counter(b): c = 'ORDER'
        elif collections.Counter(m for m, _ in a) == collections.Counter(m for m, _ in b): c = 'MNEMONICS'
        else: c = 'SHAPE'
        r = {'class': c, 'ours': len(a), 'game': len(b), 'bytes_ours': len(code), 'bytes_game': n}
        fa, fb = frame(a), frame(b)
        if fa != fb: r['frame'] = 'game only' if fb else 'ours only'
        if c == 'SHAPE':
            ca, cb = collections.Counter(m for m, _ in a), collections.Counter(m for m, _ in b)
            r['game_has'] = dict(cb - ca); r['ours_has'] = dict(ca - cb)
            lacks.update((cb - ca).keys()); extra.update((ca - cb).keys())
        rows[k] = r; cls[c] += 1
    fr = collections.Counter(r.get('frame') for r in rows.values() if r.get('frame'))
    summary = {'classes': dict(cls), 'frame_mismatch': dict(fr),
               'shape_game_has_most': lacks.most_common(15), 'shape_ours_has_most': extra.most_common(15)}
    json.dump({'summary': summary, 'functions': rows}, open(out, 'w'), indent=1)
    print(json.dumps(summary, indent=1))


if __name__ == '__main__':
    main()
