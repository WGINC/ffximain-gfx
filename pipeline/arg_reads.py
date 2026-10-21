"""arg_reads.py -- which bytes of each stack argument a function's own code reads, from its code.

A caller passes a byte argument as `push eax` with AL set: the upper 24 bits are whatever EAX held. The gate
logs every stubbed call's arguments as dwords, so two callers that pass the same byte compare unequal when their
leftover bits differ. The bits the callee never reads are not part of the call, and this gives, per function,
the mask of bytes it reads in each argument slot. gfx_cases.py uses it as the stub's argument masks.

Stack depths are sp_solve.insn_depths: exact, every unknown pop solved. At an instruction with depth d, the
operand [esp + x] is entry ESP + x - d; with EBP a copy of ESP at depth e, [ebp + x] is entry ESP + x - e. Entry
ESP + 4 + 4k is argument k. A memory operand there that the instruction reads (capstone's operand access)
reads those bytes; a write alone does not.

A function is left out, because its argument reads cannot all be seen, when:
  * an instruction with an ESP- or EBP-based memory operand has no exact depth, or EBP is not a copy of ESP there;
  * an argument's address is taken (`lea` of entry ESP + 4 or above), or ESP or EBP is copied into another
    register other than `mov ebp, esp`;
  * it leaves by a jump to another function. That jump passes the frame, so the target's reads would be the
    function's; it is not followed here.

    FFXI_DLL=... python3 arg_reads.py OUT.json
Output {"0xFUNC": [mask of argument 0, mask of argument 1, ...]}, up to the last argument read; listed only
where some mask is not 0xFFFFFFFF. A mask of 0 means the code never reads that argument."""
import json, os, sys
from capstone import CS_AC_READ
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import sp_solve as S

NSLOT = 16


def reads_of(ea, ins, depths):
    """[mask per slot] or None"""
    masks = [0] * NSLOT
    for a, i in ins.items():
        m, o = i.mnemonic, i.operands
        if m == 'jmp' and o and o[0].type == X86_OP_IMM and o[0].imm not in ins: return None
        for n, op in enumerate(o):
            if op.type == X86_OP_REG and i.reg_name(op.reg) in ('esp', 'ebp') and m in ('mov', 'lea', 'xchg') and n == 1:
                if not (m == 'mov' and o[0].type == X86_OP_REG and i.reg_name(o[0].reg) == 'ebp' and i.reg_name(op.reg) == 'esp'):
                    return None
            if op.type != X86_OP_MEM or not op.mem.base: continue
            base = i.reg_name(op.mem.base)
            if base not in ('esp', 'ebp'): continue
            if op.mem.index: return None
            d = depths.get(a)
            if d is None: return None
            if base == 'esp': off = op.mem.disp - d[0]
            else:
                if d[1] is None: return None
                off = op.mem.disp - d[1]
            off -= 4                                         # past the return address: argument area from 0
            if m == 'lea':
                if off >= 0: return None
                continue
            if off + op.size <= 0 or not (op.access & CS_AC_READ): continue
            for b in range(max(off, 0), off + op.size):
                k = b // 4
                if k < NSLOT: masks[k] |= 0xFF << (8 * (b % 4))
    last = max((k for k in range(NSLOT) if masks[k]), default=-1)
    return masks[:last + 1]


def main(out):
    im = S.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(P, '..', 'data', 'func_extents.json')))
    purges = json.load(open(os.path.join(P, '..', 'data', 'purges.json')))
    iat = S.imports(im, os.environ['FFXI_DLL']); nr = S.noreturn_funcs(im, ext, iat)
    starts = {int(k, 16) for k in ext}
    res, n = {}, 0
    for k, e in ext.items():
        ea = int(k, 16)
        depths = S.insn_depths(im, ea, e, purges, starts, nr=nr, iat=iat)
        if not depths: continue
        r = reads_of(ea, im.code(e), depths)
        if r is None: continue
        n += 1
        if any(x != 0xFFFFFFFF for x in r): res['0x%08X' % ea] = r
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print('%d functions whose argument reads are all seen; %d read less than whole dwords of some argument' % (n, len(res)))


if __name__ == '__main__':
    main(*sys.argv[1:])
