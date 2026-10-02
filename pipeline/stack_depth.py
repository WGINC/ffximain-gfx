"""stack_depth.py -- the real stack depth at every instruction of a function, from its code.

Follows the control-flow graph from the entry with the depth (bytes pushed since entry, return
address excluded). A call pops what its callee's `ret N` pops (data/purges.json); a virtual call
`call [reg+slot]` resolves through the given vtable. Hex-Rays names stack slots by an offset from
one frame base; where IDA's own depth tracking slips (a call it thinks pops more or less), every
name after it is one slot off. Comparing this depth with the offsets Hex-Rays implies finds that
point.

    python3 stack_depth.py ADDR [VTABLE] [ADDR_TO_PRINT ...]     uses $FFXI_DLL
Prints the depth at each listed address (or at every instruction if none is listed) and every
address where two paths arrive with different depths.
"""
import json, os, sys, struct
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')


def depths(ea, vtable=None):
    pe = pefile.PE(os.environ['FFXI_DLL'], fast_load=True); base = pe.OPTIONAL_HEADER.ImageBase
    img = pe.get_memory_mapped_image(); md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True
    purges = json.load(open(os.path.join(D, 'purges.json')))
    ext = json.load(open(os.path.join(D, 'func_extents.json')))['0x%x' % ea]
    ins = {}
    for a, b in ext:
        for i in md.disasm(img[a - base:b - base], a): ins[i.address] = i
    unresolved = set()      # calls whose pop count is not known (no `ret N` found, indirect without vtable)
    def purge_of(i):
        op = i.operands[0]
        k = None
        if op.type == X86_OP_IMM: k = purges.get('0x%08X' % op.imm)
        elif op.type == X86_OP_MEM and vtable and op.mem.base and op.mem.index == 0:
            t = struct.unpack('<I', img[vtable + op.mem.disp - base:vtable + op.mem.disp - base + 4])[0]
            k = purges.get('0x%08X' % t)
        if k is None: unresolved.add(i.address); return 0
        return k
    depth, conflicts, work = {ea: 0}, [], [ea]
    while work:
        a = work.pop(); d = depth[a]
        while a in ins:
            i = ins[a]; m = i.mnemonic; ops = i.operands; nd = d
            if m == 'push': nd = d + 4
            elif m == 'pop': nd = d - 4
            elif m in ('sub', 'add') and ops[0].type == X86_OP_REG and i.reg_name(ops[0].reg) == 'esp' and ops[1].type == X86_OP_IMM:
                nd = d + ops[1].imm if m == 'sub' else d - ops[1].imm
            elif m == 'call': nd = d - purge_of(i)
            nxt = []
            if m.startswith('ret'): nxt = []
            elif m == 'jmp':
                nxt = [ops[0].imm] if ops[0].type == X86_OP_IMM else []
            elif m.startswith('j') or m.startswith('loop'):
                nxt = [a + i.size] + ([ops[0].imm] if ops[0].type == X86_OP_IMM else [])
            else:
                nxt = [a + i.size]
            last = None
            for n in nxt:
                if n not in ins: continue
                if n in depth:
                    if depth[n] != nd: conflicts.append((n, depth[n], nd, a))
                    continue
                depth[n] = nd
                if n == a + i.size and m not in ('jmp',) and not m.startswith('j'): last = n
                else: work.append(n)
            if last is None: break
            a, d = last, nd
    depths.unresolved = unresolved
    return ins, depth, conflicts


def main(ea, vt=None, *show):
    ins, depth, conflicts = depths(int(ea, 16), int(vt, 16) if vt and vt != '-' else None)
    for a in (int(x, 16) for x in show) if show else sorted(depth):
        print('%08X depth 0x%X  %s %s' % (a, depth.get(a, -1), ins[a].mnemonic if a in ins else '?', ins[a].op_str if a in ins else ''))
    for n, d1, d2, frm in conflicts:
        print('CONFLICT at %08X: 0x%X vs 0x%X (from %08X)' % (n, d1, d2, frm))


if __name__ == '__main__':
    main(*sys.argv[1:])
