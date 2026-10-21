"""reg_preserve.py -- which of EAX/ECX/EDX each function leaves untouched, read from the code.

Hex-Rays and the ECX/EDX-use analyses assume a call clobbers EAX, ECX and EDX (the ABI allows it). The compiler
did not always rely on that: where it knew a callee leaves a register alone it kept a value there across the call
(reg_kept_audit.py), and a small function may read its entry ECX only after such a call (DynVB_Reserve 0x10044010:
`push edi; call 0x10043de0; ... add edx, ecx`). Treating the call as a write hides that read.

A function leaves a register untouched when none of its instructions writes any part of it
(vcall_resolve.written_exact) and every function it calls or tail-jumps to leaves it untouched too (a fixpoint);
an indirect call or jump makes it unknown, except a jump through a table whose index the instructions before it
bound (`cmp r, N` / `ja`, falling through, no jump landing between them; or MSVC's two-level switch, where the bound
index first selects a byte from an index table) with every entry inside the function (table()).

    preserved(im, code) -> {function: set of 'eax'/'ecx'/'edx' it leaves untouched}"""
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from vcall_resolve import written_exact, r32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG

VOL = ('eax', 'ecx', 'edx')


def table(im, ins, a, order):
    """the targets of `jmp [r*4+T]` when its index is bound by the instructions before it, else None. Two forms:
      cmp r, N; ja X; jmp [r*4+T]                                   targets T[0..N]
      cmp i, N; ja X; xor r, r; mov rl, [i+B]; jmp [r*4+T]          (MSVC's two-level switch) targets T[B[0..N]]
    each instruction directly after the one before, and no jump lands on any of them but the `cmp`"""
    i = ins[a]; o = i.operands[0] if i.operands else None
    if i.mnemonic != 'jmp' or o is None or o.type != X86_OP_MEM or o.mem.base or not o.mem.index or o.mem.scale != 4:
        return None
    k = order.index(a)
    landing = {x.operands[0].imm for x in ins.values()
               if x.mnemonic.startswith('j') and x.operands and x.operands[0].type == X86_OP_IMM}
    def bound(k, reg):          # cmp reg, N; ja directly before order[k]: N
        if k < 2: return None
        c, j = ins[order[k - 2]], ins[order[k - 1]]
        if c.mnemonic != 'cmp' or j.mnemonic != 'ja' or c.address + c.size != j.address or j.address + j.size != order[k]: return None
        if c.operands[0].type != X86_OP_REG or c.operands[1].type != X86_OP_IMM or c.operands[0].reg != reg: return None
        if j.address in landing or order[k] in landing or not 0 <= c.operands[1].imm <= 4096: return None
        return c.operands[1].imm
    n = bound(k, o.mem.index)
    if n is not None:
        return [im.u32((o.mem.disp & 0xFFFFFFFF) + 4 * m) for m in range(n + 1)]
    if k < 2: return None
    x, mv = ins[order[k - 2]], ins[order[k - 1]]
    if x.mnemonic != 'xor' or mv.mnemonic != 'mov' or x.address + x.size != mv.address or mv.address + mv.size != a: return None
    if x.address in landing or mv.address in landing or a in landing: return None
    xo, mo = x.operands, mv.operands
    if len(xo) != 2 or xo[0].type != X86_OP_REG or xo[1].type != X86_OP_REG or xo[0].reg != xo[1].reg or xo[0].reg != o.mem.index: return None
    if len(mo) != 2 or mo[0].type != X86_OP_REG or mo[0].size != 1 or mo[1].type != X86_OP_MEM or mo[1].size != 1: return None
    if r32(mv.reg_name(mo[0].reg)) != r32(i.reg_name(o.mem.index)) or mv.reg_name(mo[0].reg)[-1] != 'l': return None
    m = mo[1].mem
    if m.base == 0 or m.index != 0 or m.segment != 0: return None
    n = bound(k - 2, m.base)
    if n is None: return None
    return [im.u32((o.mem.disp & 0xFFFFFFFF) + 4 * im.img[(m.disp & 0xFFFFFFFF) + q - im.base]) for q in range(n + 1)]


# MSVC's stack probe _chkstk (0x10312150), this exact code: it saves ECX (`push ecx`), and nothing changes ESP before
# `mov eax, esp`, so EAX is the slot's address when `mov ecx, [eax]` reloads it. Its other ECX reads come after
# `lea ecx, [esp + 8]` wrote it. So its entry ECX is not used and is left as it was. Matched by every instruction,
# not by address, so a different probe is not taken for this one.
CHKSTK_CODE = [('push', 'ecx'), ('cmp', 'eax, 0x1000'), ('lea', 'ecx, [esp + 8]'), ('jb', '0x10312170'),
               ('sub', 'ecx, 0x1000'), ('sub', 'eax, 0x1000'), ('test', 'dword ptr [ecx], eax'), ('cmp', 'eax, 0x1000'),
               ('jae', '0x1031215c'), ('sub', 'ecx, eax'), ('mov', 'eax, esp'), ('test', 'dword ptr [ecx], eax'),
               ('mov', 'esp, ecx'), ('mov', 'ecx, dword ptr [eax]'), ('mov', 'eax, dword ptr [eax + 4]'), ('push', 'eax'),
               ('ret', '')]


def saves_ecx(ins):
    """the function is _chkstk as above: ECX saved and reloaded, its entry value otherwise unused"""
    return [(ins[a].mnemonic, ins[a].op_str) for a in sorted(ins)] == CHKSTK_CODE


def preserved(im, code, orders=None):
    orders = orders or {f: sorted(ins) for f, ins in code.items()}
    own = {}
    for f, ins in code.items():
        w, callees, unknown = set(), set(), False
        for a, i in ins.items():
            m = i.mnemonic; o = i.operands
            if m == 'call' or m == 'jmp':
                if o and o[0].type == X86_OP_IMM:
                    if m == 'call' or o[0].imm not in ins: callees.add(o[0].imm)
                elif not (m == 'jmp' and (lambda t: t is not None and all(x in ins for x in t))(table(im, ins, a, orders[f]))):
                    unknown = True
                continue
            w |= {r for r in written_exact(i) if r in VOL}
        if saves_ecx(ins): w.discard('ecx')                 # reloaded from its own save (CHKSTK_CODE)
        own[f] = (w, callees, unknown)
    keeps = {f: set() if u else set(VOL) - w for f, (w, c, u) in own.items()}
    changed = True
    while changed:
        changed = False
        for f, (w, callees, u) in own.items():
            k = set(keeps[f])
            for c in callees: k &= keeps.get(c, set())
            if k != keeps[f]: keeps[f] = k; changed = True
    return keeps


def slot_kind(ins, order, a):
    """for `push REG` at a: "overwritten" when, straight after it, a 4-byte write to [esp] comes before anything
    that could read the slot (an access overlapping [esp, esp+4), another use of ESP, a call, push, pop, jump, or a
    jump landing); "address-passed" when the slot's address is taken first (`mov r, esp` / `lea r, [esp]`: MSVC's
    by-value temporary, built in place by the callee); otherwise "use" (the pushed value is an argument or is read)"""
    landing = {x.operands[0].imm for x in ins.values()
               if x.mnemonic.startswith('j') and x.operands and x.operands[0].type == X86_OP_IMM}
    k = order.index(a)
    for b in order[k + 1:k + 12]:
        i = ins[b]; m = i.mnemonic; o = i.operands
        if b in landing or m in ('call', 'push', 'pop') or m.startswith(('j', 'ret', 'loop')): return 'use'
        if m in ('mov', 'lea') and len(o) == 2 and o[0].type == X86_OP_REG and \
                ((o[1].type == X86_OP_REG and i.reg_name(o[1].reg) == 'esp') or
                 (o[1].type == X86_OP_MEM and o[1].mem.base and i.reg_name(o[1].mem.base) == 'esp' and not o[1].mem.index and o[1].mem.disp == 0)):
            return 'address-passed'
        for n, x in enumerate(o):
            if x.type == X86_OP_REG and i.reg_name(x.reg) == 'esp': return 'use'
            if x.type != X86_OP_MEM or not x.mem.base or i.reg_name(x.mem.base) != 'esp': continue
            if x.mem.index: return 'use'
            if m == 'lea':
                if 0 <= x.mem.disp < 4: return 'address-passed'
                continue                                                        # another stack address
            if not (x.mem.disp < 4 and x.mem.disp + x.size > 0): continue           # does not touch the slot
            if n == 0 and x.mem.disp == 0 and x.size == 4 and m in ('mov', 'fstp', 'fistp', 'fst', 'fist'): return 'overwritten'
            return 'use'
    return 'use'
