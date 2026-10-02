"""purges.py -- how many argument bytes each game function pops on return (`ret N`), from its code.

Used for Ghidra decompilations: Ghidra's committed prototypes can drop parameters the callee never
reads, but a callee-cleanup function (__stdcall / __thiscall) always pops what its caller pushed, so
`ret N` fixes the number of stack arguments (hx2cpp.ghidra_ptr_type).

    python3 purges.py FFXiMain_unpacked.dll data/purges.json
Output: {"0xADDR": N} for functions whose every `ret` pops the same N bytes.
"""
import json, os, sys
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')


def main(dll, out):
    pe = pefile.PE(dll, fast_load=True)
    base = pe.OPTIONAL_HEADER.ImageBase; img = pe.get_memory_mapped_image()
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    res = {}
    for k, ranges in json.load(open(os.path.join(D, 'func_extents.json'))).items():
        pops = set()
        for a, b in ranges:
            for i in md.disasm(img[a - base:b - base], a):
                if i.mnemonic in ('ret', 'retn'):
                    pops.add(int(i.op_str, 0) if i.op_str else 0)
        if len(pops) == 1: res['0x%08X' % int(k, 16)] = pops.pop()
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print(len(res), 'functions')


if __name__ == '__main__':
    main(*sys.argv[1:3])
