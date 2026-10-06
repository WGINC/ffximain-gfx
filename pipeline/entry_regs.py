"""entry_regs.py -- which general registers a function reads with their entry value, from its own code.

A function whose code reads EAX, EBX, ESI, EDI or EBP before writing it takes an argument in that register (a
`__usercall`), or has a caller leave something in it. The IDB types most such functions as ordinary cdecl /
stdcall / thiscall; a reconstruction that calls them as typed sets nothing in the register (the ECX version of this
crashed the game: DynVB_Reserve 0x10044010, 6 Oct). data/ecx_use.json and data/edx_use.json cover ECX and EDX
across callees; this covers the other registers inside the function itself (not through its callees: a callee that
reads a register the function never wrote is not seen here).

Along the control flow from the entry, a register is "unwritten" until an instruction writes any part of it
(vcall_resolve.written_exact; a call writes EAX, ECX, EDX). An instruction that reads it while unwritten is a read of
the entry value, except:
  * `push r` of EBX/ESI/EDI/EBP that is a save: every path from it to a `ret` passes a `pop r` (or `leave`, for EBP: the
    register is restored; the stack balance is sp_solve's business);
  * `xor r, r`, `sub r, r`, `or r, -1`, `and r, 0` (constants, not reads: capstone lists them as reads).

    FFXI_DLL=... python3 entry_regs.py OUT.json [0xADDR ...]      (default: every function in data/func_extents.json)
Output {"0xFUNC": {"eax": "0xADDR instruction", ...}} for functions with at least one such read."""
import json, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sp_solve as S
from vcall_resolve import written_exact, r32
from reg_preserve import table
from capstone.x86 import X86_OP_IMM, X86_OP_REG

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
REGS = ('eax', 'ebx', 'esi', 'edi', 'ebp')
SAVED = ('ebx', 'esi', 'edi', 'ebp')


def analyse(im, ins):
    order = sorted(ins)

    def succ(a):
        i = ins[a]; m = i.mnemonic; o = i.operands
        if m.startswith('ret'): return []
        if m.startswith('j') or m == 'loop':
            if not (o and o[0].type == X86_OP_IMM):
                t = table(im, ins, a, order) if m == 'jmp' else None
                return [x for x in t if x in ins] if t and all(x in ins for x in t) else []
            t = [o[0].imm] if o[0].imm in ins else []
            return t if m == 'jmp' else t + ([a + i.size] if a + i.size in ins else [])
        return [a + i.size] if a + i.size in ins else []

    # saved[r][a]: no path from a reaches a `ret` (or leaves the function) without passing a `pop r`. A safety property:
    # the greatest fixpoint (a loop that exits does not spoil it), starting from "holds" everywhere and removing it
    # where an instruction that is not the pop reaches a ret, or leaves the function, or has a successor where it fails
    saved = {}
    for r in SAVED:
        st = {a: True for a in order}
        changed = True
        while changed:
            changed = False
            for a in reversed(order):
                i = ins[a]
                if (i.mnemonic == 'pop' and i.operands and i.operands[0].type == X86_OP_REG and r32(i.reg_name(i.operands[0].reg)) == r) \
                        or (i.mnemonic == 'leave' and r == 'ebp'): v = True              # `leave` pops EBP
                else:
                    n = succ(a); v = bool(n) and all(st.get(x, False) for x in n)
                if v != st[a]: st[a] = v; changed = True
        saved[r] = st
    # forward: registers still holding their entry value before each instruction
    ent = {order[0]: frozenset(REGS)} if order else {}
    work = list(ent); reads = {}
    while work:
        a = work.pop(); i = ins[a]; o = i.operands; live = ent[a]
        rd = {r32(i.reg_name(r)) for r in i.regs_access()[0]} & live
        if len(o) == 2 and o[0].type == X86_OP_REG and ((i.mnemonic in ('xor', 'sub') and o[1].type == X86_OP_REG and o[0].reg == o[1].reg)
                                                       or (i.mnemonic == 'or' and o[1].type == X86_OP_IMM and o[1].imm & 0xFFFFFFFF == 0xFFFFFFFF)
                                                       or (i.mnemonic == 'and' and o[1].type == X86_OP_IMM and o[1].imm & 0xFFFFFFFF == 0)):
            rd = set()
        if i.mnemonic == 'push' and o and o[0].type == X86_OP_REG:
            r = r32(i.reg_name(o[0].reg))
            n = succ(a)
            if r in SAVED and n and all(saved[r].get(x, False) for x in n): rd = rd - {r}
        for r in rd: reads.setdefault(r, '0x%08X %s %s' % (a, i.mnemonic, i.op_str))
        w = {r for r in written_exact(i)} | ({'eax', 'ecx', 'edx'} if i.mnemonic == 'call' else set())
        nxt = live - w
        for b in succ(a):
            old = ent.get(b); new = nxt if old is None else old | nxt        # may-analysis: still entry-valued on some path
            if new != old: ent[b] = new; work.append(b)
    return reads


def main(out, only=None):
    im = S.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    res = {}
    for k, v in ext.items():
        if only and int(k, 16) not in only: continue
        ins = im.code(v)
        if not ins: continue
        r = analyse(im, ins)
        if r: res['0x%08X' % int(k, 16)] = r
    json.dump(dict(sorted(res.items())), open(out, 'w'), indent=0)
    print('%d functions read an entry register (of EAX/EBX/ESI/EDI/EBP)' % len(res))


if __name__ == '__main__':
    main(sys.argv[1], {int(x, 16) for x in sys.argv[2:]} or None)
