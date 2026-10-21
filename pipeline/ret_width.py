"""ret_width.py -- which bytes of EAX each function's own code sets before it returns, from the code.

A function that sets only AL (`mov al, [esi+0xA]`) or AX, or returns what such a callee left, returns in the
rest of EAX whatever was there before it: its caller's leftover. The gate compares the function's EAX with the
original's; the leftover bytes are the caller's, which a C++ function cannot have, so only the bytes the code
sets are its result. This follows each function's control flow from the entry with, at every instruction, the
bytes of EAX written since the entry on every path that reaches it (an intersection at joins):

  * a write of AL, AH, AX or EAX sets those bytes (mov, movzx/movsx, lea, pop, setcc, lods, in, the one-operand
    mul/imul/div/idiv); `xor`/`sub` of a register with itself sets it all; `and` with an immediate sets the bytes
    the immediate clears; any other instruction that writes EAX from its old value sets nothing new;
  * a direct call sets what the callee's result sets (this fact, to a fixpoint), or nothing new where the
    callee leaves EAX untouched (data/reg_keeps.json); a call through a pointer, or an indirect jump the code
    does not bound, leaves the function out (no width);
  * `ret`: the bytes set on every path to it; a tail jump: what its target sets.

    FFXI_DLL=... python3 ret_width.py data/ret_width.json
Output: {"0xFUNC": 1 | 2 (only AL / AX set on every path)}; a function whose code sets all of EAX, or that
cannot be followed, is not listed."""
import json, os, sys
from capstone.x86 import X86_OP_IMM, X86_OP_REG
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import sp_solve as S
import vcall_resolve as V

PART = {'al': {0}, 'ah': {1}, 'ax': {0, 1}, 'eax': {0, 1, 2, 3}}
ALL = frozenset({0, 1, 2, 3})
SETS = ('mov', 'movzx', 'movsx', 'lea', 'pop', 'in')       # (a cmov writes only when its condition holds)


def effect(i, width):
    """(bytes of EAX the instruction sets, it ends the path: None = cannot follow); width: {callee: set}"""
    m = i.mnemonic; o = i.operands
    if m == 'call':
        if not o or o[0].type != X86_OP_IMM: return None
        w = width.get(o[0].imm)
        return w if w is not None else None
    if m in ('mul', 'imul', 'div', 'idiv') and len(o) == 1: return ALL
    if m.startswith(('lods', 'rep lods')): return {0} if m.endswith('b') else {0, 1} if m.endswith('w') else ALL
    if not o or o[0].type != X86_OP_REG or i.reg_name(o[0].reg) not in PART:
        if m in ('xchg',) and len(o) == 2 and o[1].type == X86_OP_REG and i.reg_name(o[1].reg) in PART: return set(PART[i.reg_name(o[1].reg)])
        return set()
    r = i.reg_name(o[0].reg); part = set(PART[r])
    if m in SETS or m.startswith('set'): return part
    if m in ('xor', 'sub') and len(o) == 2 and o[1].type == X86_OP_REG and o[1].reg == o[0].reg: return part
    if m == 'and' and len(o) == 2 and o[1].type == X86_OP_IMM:
        imm = o[1].imm & 0xFFFFFFFF
        return {b for b in part if (imm >> (8 * b)) & 0xFF == 0}
    if m == 'xchg': return part
    if m in ('cmp', 'test', 'push'): return set()
    return set()


def walk(im, f, ins, width, keeps, labels):
    """the bytes set at every `ret` and tail jump; None when a path cannot be followed"""
    if f not in ins: return None
    have = {f: frozenset()}; work = [f]; out = ALL; tails = []
    while work:
        a = work.pop(); i = ins[a]; m = i.mnemonic; o = i.operands
        cur = have[a]
        if m == 'call' and o and o[0].type == X86_OP_IMM and 'eax' in keeps.get(o[0].imm, ()):
            e = set()                                          # the callee leaves EAX as it was
        else:
            e = effect(i, width)
            if e is None: return None
        cur = frozenset(cur | e)
        if m.startswith('ret'): out = out & cur; continue
        if m == 'jmp' and o and o[0].type == X86_OP_IMM and o[0].imm not in ins:
            w = width.get(o[0].imm)
            if w is None: return None
            out = out & frozenset(cur | w); continue
        nx = V.succs(im, ins, a, i, labels)
        if nx is None: return None
        for n in nx:
            if n not in ins: continue
            if n not in have: have[n] = cur; work.append(n)
            elif have[n] & cur != have[n]: have[n] = have[n] & cur; work.append(n)
    return out


def main(out):
    im = S.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(P, '..', 'data', 'func_extents.json')))
    keeps = {int(k, 16): set(v) for k, v in json.load(open(os.path.join(P, '..', 'data', 'reg_keeps.json'))).items()}
    code = {int(k, 16): im.code(e) for k, e in ext.items()}
    labels = {f: S.labels_of(ins) for f, ins in code.items()}
    width = {}
    while True:                                                # widths only grow from unknown: a fixpoint
        new = 0
        for f, ins in code.items():
            if f in width: continue
            w = walk(im, f, ins, width, keeps, labels[f])
            if w is not None: width[f] = w; new += 1
        if not new: break
    res = {}
    for f, w in sorted(width.items()):
        if w == ALL: continue
        if w == frozenset({0}): res['0x%08X' % f] = 1
        elif w == frozenset({0, 1}): res['0x%08X' % f] = 2
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print('%d functions followed; %d set only AL, %d only AX' % (len(width), sum(v == 1 for v in res.values()), sum(v == 2 for v in res.values())))


if __name__ == '__main__':
    main(*sys.argv[1:])
