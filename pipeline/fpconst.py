"""fpconst.py -- the x87 memory constants each graphics function uses, read from the game DLL.

Hex-Rays prints a single-precision constant (`fmul ds:flt_X`) as a short decimal such as 0.58823526.
Compiled as a C double that literal is a different number from the float the original multiplies by,
so the result differs in the low bits. This records, per function, every float/double constant an x87
instruction reads from the image; hx2cpp rewrites matching literals to the constant's exact value.

    python3 fpconst.py FFXiMain_unpacked.dll EXPORT.jsonl data/fp_consts.json
"""
import json, os, struct, sys
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_MEM, X86_REG_INVALID

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')


def main(dll, export, out):
    pe = pefile.PE(dll, fast_load=True)
    base = pe.OPTIONAL_HEADER.ImageBase
    img = pe.get_memory_mapped_image()
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True
    res = {}
    for line in open(export):
        ea = json.loads(line)['ea']
        consts = {}
        for a, b in ext.get('0x%x' % ea, []):
            for ins in md.disasm(img[a - base:b - base], a):
                if not ins.mnemonic.startswith('f'): continue
                for op in ins.operands:
                    if op.type != X86_OP_MEM or op.mem.base != X86_REG_INVALID or op.mem.index != X86_REG_INVALID: continue
                    addr = op.mem.disp & 0xFFFFFFFF
                    if not (base <= addr < base + len(img) - 8) or op.size not in (4, 8): continue
                    raw = img[addr - base:addr - base + op.size]
                    v = struct.unpack('<f' if op.size == 4 else '<d', raw)[0]
                    if v == v and abs(v) != float('inf'):
                        consts['0x%08X' % addr] = ['f32' if op.size == 4 else 'f64', v.hex()]
        if consts: res['0x%08X' % ea] = consts
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print('%d functions, %d constants' % (len(res), sum(len(v) for v in res.values())))


if __name__ == '__main__':
    main(*sys.argv[1:4])
