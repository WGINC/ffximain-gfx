"""ret_kinds.py -- what each function in the game can return in EAX, from its code.

A gate stub answers a call with a fuzzed EAX. A callee that really returns only 0/1 then gets
values it never produces, and a reconstruction that tests `!= 0` where the original tests `al == 1`
fails on an input the game cannot produce. This runs a small forward dataflow over each function's
control flow and classifies the value at every `ret`:

  bool     EAX is exactly 0 or 1 on every path
  lowbool  AL is 0 or 1 on every path; the upper 24 bits are whatever was in EAX before
  (absent) anything else: the stub keeps fuzzing the full register

    python3 ret_kinds.py FFXiMain_unpacked.dll data/ret_kinds.json
"""
import json, os, sys
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_REG, X86_OP_IMM

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
ZERO, BOOL, LOWBOOL, TOP = 'zero', 'bool', 'lowbool', 'top'
RANK = {ZERO: 0, BOOL: 1, LOWBOOL: 2, TOP: 3}


def join(a, b):
    if a is None: return b
    if b is None: return a
    return a if RANK[a] >= RANK[b] else b


def classify(md, img, base, ranges):
    ins = []
    for a, b in ranges:
        ins += list(md.disasm(img[a - base:b - base], a))
    if not ins: return None
    at = {i.address: k for k, i in enumerate(ins)}
    state = {0: TOP}                 # EAX on entry: anything
    work, rets = [0], []
    seen = {}
    while work:
        k = work.pop()
        s = state[k]
        flag_eax = False             # last flag-setter was `test eax, eax` / `or eax, eax` / `cmp eax, 0`
        flag_al = False              # last flag-setter was `test al, al` / `cmp al, 0`: on the zero branch AL = 0
        while k < len(ins):
            i = ins[k]; m = i.mnemonic; ops = i.operands
            r0 = i.reg_name(ops[0].reg) if ops and ops[0].type == X86_OP_REG else None
            imm1 = ops[1].imm if len(ops) > 1 and ops[1].type == X86_OP_IMM else None
            r1 = i.reg_name(ops[1].reg) if len(ops) > 1 and ops[1].type == X86_OP_REG else None
            nxt = []
            if m in ('ret', 'retn'):
                rets.append(s); break
            if m == 'jmp':
                if ops[0].type == X86_OP_IMM and ops[0].imm in at: nxt = [(at[ops[0].imm], s)]
                else: return None            # tail call or jump table
                fall = False
            elif m.startswith('j'):
                if ops[0].type != X86_OP_IMM or ops[0].imm not in at: return None
                taken, through = s, s
                if flag_eax and m in ('je', 'jz'): taken = ZERO
                if flag_eax and m in ('jne', 'jnz'): through = ZERO
                al0 = ZERO if s == ZERO else LOWBOOL          # AL known 0; the upper bits as they were
                if flag_al and m in ('je', 'jz'): taken = al0
                if flag_al and m in ('jne', 'jnz'): through = al0
                nxt = [(at[ops[0].imm], taken)]; s = through; fall = True
            else:
                fall = True
                if m == 'call': s = TOP
                elif r0 == 'eax' and m == 'xor' and r1 == 'eax': s = ZERO
                elif r0 == 'eax' and m == 'mov' and imm1 in (0, 1): s = ZERO if imm1 == 0 else BOOL
                elif r0 == 'eax' and m == 'and' and imm1 == 1: s = BOOL
                elif r0 == 'al' and (m == 'mov' and imm1 in (0, 1) or m == 'xor' and r1 == 'al' or m.startswith('set')):
                    s = BOOL if s in (ZERO, BOOL) else LOWBOOL
                elif r0 == 'eax' and m == 'movzx' and r1 == 'al': s = BOOL if s in (BOOL, LOWBOOL, ZERO) else TOP
                elif r0 in ('eax', 'ax', 'al', 'ah') and m not in ('test', 'cmp', 'push'): s = TOP
                elif m in ('cdq', 'cwde', 'lodsd', 'lodsb', 'xchg', 'mul', 'div', 'imul', 'idiv', 'in', 'rdtsc'):
                    if any(o.type == X86_OP_REG and i.reg_name(o.reg) in ('eax', 'ax', 'al', 'ah') for o in ops) or m in ('cdq', 'cwde', 'lodsd', 'lodsb', 'mul', 'div', 'idiv', 'in', 'rdtsc'):
                        s = TOP
                elif m in ('popal', 'popad', 'pop') and r0 == 'eax': s = TOP
                if m in ('test', 'or') and r0 == 'eax' and r1 == 'eax' or m == 'cmp' and r0 == 'eax' and imm1 == 0:
                    flag_eax = True; flag_al = False
                elif m in ('test', 'or') and r0 == 'al' and r1 == 'al' or m == 'cmp' and r0 == 'al' and imm1 == 0:
                    flag_al = True; flag_eax = False
                elif m in ('test', 'cmp', 'or', 'and', 'xor', 'add', 'sub', 'inc', 'dec', 'neg', 'shl', 'shr', 'sar', 'sbb', 'adc'):
                    flag_eax = False; flag_al = False
            for t, st in nxt:
                new = join(state.get(t), st)
                if state.get(t) != new or t not in seen:
                    state[t] = new; seen[t] = 1; work.append(t)
            if not fall: break
            k += 1
            if k in state and k not in (x[0] for x in nxt):
                new = join(state[k], s)
                if new == state[k] and k in seen: break
                state[k] = new; seen[k] = 1; s = new
    if not rets: return None
    r = rets[0]
    for x in rets[1:]: r = join(r, x)
    return {ZERO: BOOL, BOOL: BOOL, LOWBOOL: LOWBOOL}.get(r)


def main(dll, out):
    pe = pefile.PE(dll, fast_load=True)
    base = pe.OPTIONAL_HEADER.ImageBase; img = pe.get_memory_mapped_image()
    md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True
    res = {}
    for k, ranges in json.load(open(os.path.join(D, 'func_extents.json'))).items():
        c = classify(md, img, base, ranges)
        if c: res['0x%08X' % int(k, 16)] = c
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    import collections
    print(len(res), collections.Counter(res.values()))


if __name__ == '__main__':
    main(*sys.argv[1:3])
