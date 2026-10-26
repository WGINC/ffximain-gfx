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
  * it leaves by a jump (conditional or not) to another function at a depth that is not exact or not 0, or to a
    function that is itself left out (or reached again through such jumps: a cycle is not followed).
A jump to another function at depth 0 (ESP at the return address, as at the entry) passes the frame: the target
reads the same argument slots, so its reads count as the function's, except bytes of a slot the instructions
straight before the jump store (each ending where the next begins, no jump landing after it, no call between),
which the target reads as stored, not as passed.

    FFXI_DLL=... python3 arg_reads.py OUT.json
Output {"0xFUNC": [mask of argument 0, mask of argument 1, ...]}, up to the last argument read; listed only
where some mask is not 0xFFFFFFFF. A mask of 0 means the code never reads that argument."""
import json, os, sys
from capstone import CS_AC_READ, CS_AC_WRITE
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import sp_solve as S

NSLOT = 16


def arg_bytes(i, op, d):
    """the argument-area bytes (offsets past the return address) an ESP/EBP-based memory operand covers, or None"""
    base = i.reg_name(op.mem.base)
    if base == 'esp': off = op.mem.disp - d[0]
    else:
        if d[1] is None: return None
        off = op.mem.disp - d[1]
    return range(off - 4, off - 4 + op.size)


def stored_before(ins, labels, jmp, depths):
    """bytes of the argument area the straight-line run ending at the jump stores, nothing landing after them"""
    out = set(); x = jmp
    while True:
        if x.address in labels: break
        p = labels.before(x.address)
        if p is None or p.mnemonic == 'call' or p.mnemonic.startswith(('j', 'ret', 'loop')): break
        d = depths.get(p.address)
        if d is None: break
        for op in p.operands:
            if op.type == X86_OP_MEM and op.mem.base and not op.mem.index and p.reg_name(op.mem.base) in ('esp', 'ebp') \
                    and (op.access & CS_AC_WRITE) and not (op.access & CS_AC_READ) and p.mnemonic == 'mov':
                r = arg_bytes(p, op, d)
                if r is not None: out.update(b for b in r if b >= 0)
        x = p
    return out


def reads_of(ea, ins, depths):
    """([mask per slot], [(tail-jump target, argument bytes stored before it)]) or None"""
    masks = [0] * NSLOT; tails = []; labels = S.labels_of(ins)
    for a, i in ins.items():
        m, o = i.mnemonic, i.operands
        if (m == 'jmp' or m.startswith('j')) and o and o[0].type == X86_OP_IMM and o[0].imm not in ins:
            d = depths.get(a)
            if d is None or d[0] != 0: return None
            tails.append((o[0].imm, stored_before(ins, labels, i, depths)))
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
            r = arg_bytes(i, op, d)
            if r is None: return None
            off = r.start                                    # past the return address: argument area from 0
            if m == 'lea':
                if off >= 0: return None
                continue
            if off + op.size <= 0 or not (op.access & CS_AC_READ): continue
            for b in range(max(off, 0), off + op.size):
                k = b // 4
                if k < NSLOT: masks[k] |= 0xFF << (8 * (b % 4))
    return masks, tails


def trim(masks):
    last = max((k for k in range(NSLOT) if masks[k]), default=-1)
    return masks[:last + 1]


def main(out):
    im = S.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(P, '..', 'data', 'func_extents.json')))
    purges = json.load(open(os.path.join(P, '..', 'data', 'purges.json')))
    iat = S.imports(im, os.environ['FFXI_DLL']); nr = S.noreturn_funcs(im, ext, iat)
    starts = {int(k, 16) for k in ext}
    own = {}
    for k, e in ext.items():
        ea = int(k, 16)
        depths = S.insn_depths(im, ea, e, purges, starts, nr=nr, iat=iat)
        if not depths: continue
        r = reads_of(ea, im.code(e), depths)
        if r is not None: own[ea] = r
    # tail jumps: a function is decided once every target is (cycles stay undecided)
    done = {ea: m for ea, (m, tails) in own.items() if not tails}
    changed = True
    while changed:
        changed = False
        for ea, (m, tails) in own.items():
            if ea in done or not all(t in done for t, _ in tails): continue
            m = list(m)
            for t, stored in tails:
                for k, tm in enumerate(done[t]):
                    keep = sum(0xFF << (8 * b) for b in range(4) if 4 * k + b not in stored)
                    m[k] |= tm & keep
            done[ea] = m; changed = True
    res, n = {}, len(done)
    for ea, m in done.items():
        r = trim(m)
        if any(x != 0xFFFFFFFF for x in r): res['0x%08X' % ea] = r
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print('%d functions whose argument reads are all seen; %d read less than whole dwords of some argument' % (n, len(res)))


if __name__ == '__main__':
    main(*sys.argv[1:])
