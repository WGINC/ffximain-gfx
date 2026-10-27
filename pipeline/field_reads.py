"""field_reads.py -- list the [register + displacement] memory accesses of game functions, from the code bytes.

    FFXI_DLL=... python3 pipeline/field_reads.py ADDR [ADDR...]      (extents from data/func_extents.json)
    FFXI_DLL=... python3 pipeline/field_reads.py --reg ecx ADDR      only accesses through one base register

For every instruction with a memory operand [base + disp] (no index register, or with one: shown), prints the
address, the access width in bytes, read/write (capstone's operand access flags), the base register and the
displacement. It does not follow data flow: which object a base register holds at that instruction is not
decided here; read the instruction stream (pipeline/disasm.py) for that. docs/FORMATS.md uses it to check the
offsets the decompiled text shows against the instructions the game executes.

Nothing from the game image is written to the repo; output goes to stdout.
"""
import json, os, sys
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_MEM
from capstone import CS_AC_READ, CS_AC_WRITE

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')


def main(argv):
    only = None
    if argv[:1] == ['--reg']:
        only = argv[1]; argv = argv[2:]
    pe = pefile.PE(os.environ['FFXI_DLL'], fast_load=True)
    base = pe.OPTIONAL_HEADER.ImageBase
    img = pe.get_memory_mapped_image()
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True
    for a in argv:
        ea = int(a, 16)
        chunks = ext.get('0x%x' % ea)
        if chunks is None:
            print('%08X: no extent in data/func_extents.json' % ea); continue
        print('%08X' % ea)
        for s, e in chunks:
            for i in md.disasm(img[s - base:e - base], s):
                for op in i.operands:
                    if op.type != X86_OP_MEM or op.mem.base == 0:
                        continue
                    b = i.reg_name(op.mem.base)
                    if b in ('esp', 'ebp') or (only and b != only):
                        continue
                    rw = ('r' if op.access & CS_AC_READ else '') + ('w' if op.access & CS_AC_WRITE else '')
                    if i.mnemonic == 'lea':
                        rw = 'lea'
                    idx = '+%s*%d' % (i.reg_name(op.mem.index), op.mem.scale) if op.mem.index else ''
                    print('  %08X  %-3s %d  [%s%s%+#x]  %s %s' % (i.address, rw, op.size, b, idx, op.mem.disp,
                                                               i.mnemonic, i.op_str))


if __name__ == '__main__':
    main(sys.argv[1:])
