"""cdecl_args.py -- how many stack arguments each caller-cleanup (cdecl) function reads, from its code.

A gate stub logs a callee's arguments and compares them. For a cdecl callee, logging more slots
than the callee reads compares stale stack contents: MSVC often leaves an earlier call's arguments
on the stack (deferred cleanup), and the reconstruction's stale slots differ. This finds the highest
argument slot each function reads ([esp+N] beyond its own pushes, or [ebp+N] above the frame).

    python3 cdecl_args.py FFXiMain_unpacked.dll data/cdecl_args.json
Output: {"0xADDR": number of argument slots read}, for functions that pop nothing (`ret` / `ret 0`)
and whose stack depth could be followed (an `esp` change it cannot follow, a tail call or an indirect
jump drops the function).
"""
import json, os, sys
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_MEM, X86_OP_IMM, X86_OP_REG

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')


def nargs(md, img, base, ranges):
    ins = []
    for a, b in ranges:
        ins += list(md.disasm(img[a - base:b - base], a))
    ins.sort(key=lambda i: i.address)
    depth, ebp_depth, top = 0, None, 0
    inside = lambda t: any(a <= t < b for a, b in ranges)
    for i in ins:
        m = i.mnemonic; ops = i.operands
        if m == 'jmp' and ops and (ops[0].type != X86_OP_IMM or not inside(ops[0].imm)):
            return None                                   # tail call or jump table: arguments may be forwarded
        for op in ops:
            if op.type != X86_OP_MEM: continue
            b = i.reg_name(op.mem.base) if op.mem.base else None
            if b == 'esp' and op.mem.index == 0:
                slot = (op.mem.disp - depth) // 4          # 0 = return address, 1 = first argument
                if slot >= 1: top = max(top, slot)
            elif b == 'ebp' and ebp_depth is not None and op.mem.index == 0:
                slot = (op.mem.disp + ebp_depth) // 4
                if slot >= 1 and op.mem.disp > 0: top = max(top, slot)
        if m == 'push': depth += 4
        elif m == 'pop':
            depth -= 4
            if ops and ops[0].type == X86_OP_REG and i.reg_name(ops[0].reg) == 'ebp': ebp_depth = None
        elif m == 'mov' and len(ops) == 2 and ops[0].type == X86_OP_REG and ops[1].type == X86_OP_REG \
                and i.reg_name(ops[0].reg) == 'ebp' and i.reg_name(ops[1].reg) == 'esp':
            ebp_depth = depth
        elif m in ('sub', 'add') and ops and ops[0].type == X86_OP_REG and i.reg_name(ops[0].reg) == 'esp':
            if len(ops) < 2 or ops[1].type != X86_OP_IMM: return None
            depth += ops[1].imm if m == 'sub' else -ops[1].imm
        elif m in ('ret', 'retn', 'jmp') and ebp_depth is None:
            depth = 0 if m in ('ret', 'retn') else depth   # each return path starts again from entry depth 0
        elif m in ('leave',):
            ebp_depth = None
        elif ops and ops[0].type == X86_OP_REG and i.reg_name(ops[0].reg) == 'esp' and m not in ('cmp', 'test'):
            if m != 'mov' or not (len(ops) == 2 and ops[1].type == X86_OP_REG and i.reg_name(ops[1].reg) == 'ebp'):
                return None
            depth = ebp_depth if ebp_depth is not None else depth
        if depth < -8: return None
    return top


def main(dll, out):
    pe = pefile.PE(dll, fast_load=True)
    base = pe.OPTIONAL_HEADER.ImageBase; img = pe.get_memory_mapped_image()
    md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True
    purges = json.load(open(os.path.join(D, 'purges.json')))
    res = {}
    for k, ranges in json.load(open(os.path.join(D, 'func_extents.json'))).items():
        key = '0x%08X' % int(k, 16)
        if purges.get(key) != 0: continue
        n = nargs(md, img, base, ranges)
        if n is not None: res[key] = n
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print(len(res), 'cdecl functions')


if __name__ == '__main__':
    main(*sys.argv[1:3])
