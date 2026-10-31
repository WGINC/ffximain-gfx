"""raw_game_addrs.py -- the reconstructed functions whose in-game code holds a game address that is not rebased.

In the in-game build every address of FFXiMain.dll must go through XI_GAME (xi_decomp.hpp: the analysed address plus
xi_rel, the game's load delta). A literal the source writes without it (an inline-asm call target, a class
descriptor passed as an integer, a loop bound that is a table's end) is used as is: in the gate it is right (the gate
loads the game at its analysed base, 0x10000000), in the game it points at nothing (31 Oct: the owner's game faulted
executing 0x100D12D0, the target of a raw `call *%edi`).

The probe build (tree/build.sh XI_PROBE=1, gfx_recon_probe.dll, never run) moves every XI_GAME address by 0x01000000
at compile time. A constant in 0x10001000-0x10700000 left in its code (an immediate, or the displacement of a memory
operand) was therefore not written through XI_GAME. Such a constant may also be a plain integer that happens to lie in
that range; it is listed all the same (leaving a function out of the swap set is the safe side). Each is charged to
the function holding it (the DLL's symbol table: from a symbol to the next) and to every exported function that
reaches it by direct calls inside the DLL.

    python3 pipeline/raw_game_addrs.py PROBE.dll OUT.json
OUT: {"exports": {export symbol: [first finding, ...]}, "holders": {symbol: [findings]}}"""
import bisect, collections, json, subprocess, sys
import capstone, pefile
from capstone.x86 import X86_OP_IMM, X86_OP_MEM

LO, HI = 0x10001000, 0x10700000


def main(dll, out):
    pe = pefile.PE(dll); pe.parse_data_directories(); ib = pe.OPTIONAL_HEADER.ImageBase
    syms = {}
    for l in subprocess.run(['i686-w64-mingw32-nm', dll], capture_output=True, text=True, check=True).stdout.split('\n'):
        p = l.split()
        if len(p) == 3 and p[1] in 'tT' and not p[2].startswith('.'):
            syms.setdefault(int(p[0], 16), p[2][1:] if p[2].startswith('_') else p[2])   # (COFF's leading underscore)
    starts = sorted(syms); names = [syms[a] for a in starts]
    holder = lambda a: names[bisect.bisect_right(starts, a) - 1] if a >= starts[0] else None
    exports = {(e.name or b'').decode() for e in pe.DIRECTORY_ENTRY_EXPORT.symbols}
    cs = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32); cs.detail = True
    found = collections.defaultdict(list); calls = collections.defaultdict(set)
    for s in pe.sections:
        if not s.Characteristics & 0x20000000: continue
        base = ib + s.VirtualAddress; data = s.get_data(); off = 0
        while off < len(data):
            last = off
            for i in cs.disasm(data[off:], base + off):
                off = i.address - base + i.size
                h = holder(i.address)
                for op in i.operands:
                    v = op.imm if op.type == X86_OP_IMM else op.mem.disp if op.type == X86_OP_MEM else None
                    if v is not None and LO <= (v & 0xFFFFFFFF) < HI:
                        if i.mnemonic == 'call' or i.mnemonic.startswith('j'): continue      # (a target in this DLL)
                        found[h].append('0x%08X %s %s' % (i.address, i.mnemonic, i.op_str))
                if i.mnemonic in ('call', 'jmp') and i.operands and i.operands[0].type == X86_OP_IMM:
                    t = holder(i.operands[0].imm)
                    if t and t != h: calls[t].add(h)                                  # callee -> callers
            if off == last: off += 1                                                  # undecodable byte
    # each holder's findings reach every function that calls it, directly or through others
    reach = collections.defaultdict(set)
    for h in found:
        seen, todo = {h}, [h]
        while todo:
            x = todo.pop()
            for c in calls.get(x, ()):
                if c not in seen: seen.add(c); todo.append(c)
        for x in seen: reach[x].add(h)
    exp = {x: ['%s: %s' % (h, found[h][0]) for h in sorted(hs)] for x, hs in sorted(reach.items()) if x in exports}
    json.dump({'_doc': __doc__.split('\n\n')[0], 'exports': exp, 'holders': {h: v for h, v in sorted(found.items())}},
              open(out, 'w'), indent=1)
    print('%d game-range constants in %d functions; %d exported functions hold or reach one' % (
        sum(map(len, found.values())), len(found), len(exp)))


if __name__ == '__main__':
    main(*sys.argv[1:3])
