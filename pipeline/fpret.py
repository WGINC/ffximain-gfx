"""fpret.py -- which functions return a value in x87 st(0)?

Tracks the x87 stack depth along every path from entry (MSVC6 code enters with an empty x87
stack). A function returns a float when every reachable `ret` is reached with depth exactly 1.
Calls to functions already known to return a float push one; the set is grown to a fixpoint.
Output: {hex(ea): "float"} for float-returning functions.
Usage: python3 fpret.py game.dll ida_functions.json out.json
"""
import json, sys
import capstone, pefile
from capstone.x86 import X86_OP_IMM, X86_OP_MEM

PUSH = {'fld', 'fild', 'fldz', 'fld1', 'fldpi', 'fldl2e', 'fldln2', 'fldlg2', 'fldl2t', 'fbld'}
POP1 = {'fstp', 'fistp', 'faddp', 'fsubp', 'fsubrp', 'fmulp', 'fdivp', 'fdivrp', 'fcomp', 'fucomp',
        'fbstp', 'fcomip', 'fucomip', 'fisttp'}
POP2 = {'fcompp', 'fucompp'}

def main(game, idaf, out):
    pe = pefile.PE(game); img = bytes(pe.get_memory_mapped_image()); B = pe.OPTIONAL_HEADER.ImageBase
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32); md.detail = True
    F = {r['ea']: r['chunks'] for r in json.load(open(idaf))}
    cache = {}
    def insn(a):
        if a not in cache:
            cache[a] = next(md.disasm(img[a - B:a - B + 16], a, 1), None)
        return cache[a]
    def inside(f, a): return any(s <= a < e for s, e in F[f])
    fl = set()
    def analyse(f):
        work = [(f, 0)]; seen = set(); rets = set(); ok = True
        while work:
            a, d = work.pop()
            if (a, d) in seen or len(seen) > 20000: continue
            seen.add((a, d))
            i = insn(a)
            if i is None or not inside(f, a): ok = False; continue
            m = i.mnemonic; nd = d
            if m in PUSH: nd = d + 1
            elif m in POP1: nd = d - 1
            elif m in POP2: nd = d - 2
            elif m == 'fxch' or m.startswith('f') and m not in ('fnstsw', 'fstsw', 'fnstcw', 'fldcw', 'fwait', 'fnclex', 'fninit', 'femms'):
                pass
            if m == 'call':
                op = i.operands[0]
                if op.type == X86_OP_IMM and (op.imm & 0xFFFFFFFF) in fl: nd = d + 1
            if nd < 0 or nd > 8: ok = False; continue
            if m in ('ret', 'retn'):
                rets.add(nd); continue
            nxt = []
            if m == 'jmp':
                op = i.operands[0]
                if op.type == X86_OP_IMM:
                    t = op.imm & 0xFFFFFFFF
                    if inside(f, t): nxt = [t]
                    elif t in fl: rets.add(nd + 1)     # tail call to a float-returning function
                    elif t in F: rets.add(nd)
            elif m.startswith('j') or m.startswith('loop'):
                op = i.operands[0]
                nxt = [a + i.size] + ([op.imm & 0xFFFFFFFF] if op.type == X86_OP_IMM else [])
            elif m in ('int3', 'hlt'):
                pass
            else:
                nxt = [a + i.size]
            for n in nxt: work.append((n, nd))
        return ok and rets == {1}
    changed = True; rounds = 0
    while changed and rounds < 6:
        changed = False; rounds += 1
        for f in F:
            if f in fl: continue
            if analyse(f): fl.add(f); changed = True
    json.dump({hex(f): 'float' for f in sorted(fl)}, open(out, 'w'))
    print('float-returning functions:', len(fl), 'rounds', rounds)

if __name__ == '__main__':
    main(*sys.argv[1:4])
