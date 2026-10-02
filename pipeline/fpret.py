"""fpret.py -- which functions return a value in x87 st(0)?

Tracks the x87 stack depth along every path from entry (MSVC6 code enters with an empty x87
stack). A function returns a float when every reachable `ret` is reached with depth exactly 1.
Calls to functions already known to return a float push one; the set is grown to a fixpoint.
Output: {hex(ea): "float"} for float-returning functions, {hex(ea): "none"} for functions proven to
return with an empty x87 stack.
Calls to functions that pop a value they were given (__ftol) pop one.
Usage: python3 fpret.py game.dll data/func_extents.json out.json [previous.json]   (or IDA's function list)
previous.json: an earlier result whose `float` verdicts are kept unless now proven `none`.
"""
import json, sys
import capstone, pefile
from capstone.x86 import X86_OP_IMM, X86_OP_MEM

PUSH = {'fld', 'fild', 'fldz', 'fld1', 'fldpi', 'fldl2e', 'fldln2', 'fldlg2', 'fldl2t', 'fbld', 'fsincos', 'fptan', 'fxtract'}
POP1 = {'fstp', 'fistp', 'faddp', 'fsubp', 'fsubrp', 'fmulp', 'fdivp', 'fdivrp', 'fcomp', 'fucomp',
        'fbstp', 'fcomip', 'fucomip', 'fisttp', 'fpatan', 'fyl2x', 'fyl2xp1'}
POP2 = {'fcompp', 'fucompp'}

def main(game, idaf, out, previous=None):
    pe = pefile.PE(game); img = bytes(pe.get_memory_mapped_image()); B = pe.OPTIONAL_HEADER.ImageBase
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32); md.detail = True
    src = json.load(open(idaf))
    if isinstance(src, dict):                       # data/func_extents.json: {"0xADDR": [[start, end], ...]}
        F = {int(k, 16): v for k, v in src.items()}
    else:
        F = {r['ea']: r['chunks'] for r in src}
    cache = {}
    def insn(a):
        if a not in cache:
            cache[a] = next(md.disasm(img[a - B:a - B + 16], a, 1), None)
        return cache[a]
    def inside(f, a): return any(s <= a < e for s, e in F[f])
    fl = set()
    pop1 = set()                                      # functions that consume a value already on the x87 stack (__ftol)
    def analyse(f, start=0):
        work = [(f, 0 if start == 'rets' else start)]; seen = set(); rets = set(); ok = True
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
                elif op.type == X86_OP_IMM and (op.imm & 0xFFFFFFFF) in pop1: nd = d - 1
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
        if start == 'rets': return ok, rets
        return (ok and rets == {1}) if start == 0 else (ok and rets == {0})
    # a consumer pops one value it was given: from an empty stack it underflows, from one value it
    # returns with none (e.g. MSVC __ftol: fistp)
    CTRL = ('fnstsw', 'fstsw', 'fnstcw', 'fstcw', 'fldcw', 'fwait', 'wait', 'fnclex', 'fninit', 'femms')
    def first_x87_pops(f):
        # the first x87 instruction on the straight-line path from entry pops a value nobody pushed
        a = f
        for _ in range(40):
            i = insn(a)
            if i is None or not inside(f, a) or i.mnemonic in ('ret', 'retn', 'call') or i.mnemonic.startswith('j'): return False
            if i.mnemonic.startswith('f') and i.mnemonic not in CTRL: return i.mnemonic in POP1
            a += i.size
        return False
    for f in F:
        if first_x87_pops(f) and analyse(f, 1): pop1.add(f)
    print('consumers (pop one value):', len(pop1), sorted(hex(x) for x in pop1)[:8])
    changed = True; rounds = 0
    while changed and rounds < 6:
        changed = False; rounds += 1
        for f in F:
            if f in fl: continue
            if analyse(f): fl.add(f); changed = True
    # `none`: proven to return with an empty x87 stack on every path (not merely unproven float)
    res = {hex(f): 'float' for f in fl}
    for f in F:
        if f in fl: continue
        ok, rets = analyse(f, 'rets')
        if ok and rets == {0}: res[hex(f)] = 'none'
    # a float function whose float comes from a virtual call is invisible here: keep an earlier
    # `float` verdict unless this analysis proves the function returns with an empty x87 stack
    if previous:
        for k, v in json.load(open(previous)).items():
            if v == 'float' and res.get(k) != 'none': res[k] = 'float'
    json.dump(dict(sorted(res.items())), open(out, 'w'))
    print('float-returning functions:', len(fl), 'proven none:', sum(v == 'none' for v in res.values()), 'rounds', rounds)

if __name__ == '__main__':
    main(*sys.argv[1:5])
