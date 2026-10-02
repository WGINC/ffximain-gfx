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


def dispatch_thunks(img, base, md, res):
    """D3DX CPU dispatch: `jmp [P]` thunks through a table of function pointers that a selector fills at
    startup (`mov esi, DEFAULT; mov edi, TABLE; rep movsd`, then CPU-specific overrides). Every version of
    an entry has the same signature, so a thunk pops what the default implementation's `ret N` pops."""
    rd = lambda a: int.from_bytes(img[a - base:a - base + 4], 'little')
    lo, hi = base, base + len(img)
    n, seen = 0, {}
    for a in range(0, len(img) - 6):
        if img[a] != 0xFF or img[a + 1] != 0x25: continue
        p = int.from_bytes(img[a + 2:a + 6], 'little')
        if not lo <= p < hi - 4: continue
        t = rd(p)                               # initially the lazy selector: push 1; call SEL; jmp [P]
        if not lo <= t < hi - 16: continue
        ins = list(md.disasm(img[t - base:t - base + 16], t))
        if len(ins) < 3 or ins[0].mnemonic != 'push' or ins[1].mnemonic != 'call' or ins[2].mnemonic != 'jmp': continue
        sel = int(ins[1].op_str, 16)
        if sel not in seen:
            src = dst = None
            for i in md.disasm(img[sel - base:sel - base + 0x40], sel):
                if i.mnemonic == 'mov' and i.op_str.startswith('esi, 0x'): src = int(i.op_str[5:], 16)
                if i.mnemonic == 'mov' and i.op_str.startswith('edi, 0x'): dst = int(i.op_str[5:], 16)
                if i.mnemonic.startswith('rep'): break
            seen[sel] = (src, dst)
        src, dst = seen[sel]
        if src is None or dst is None or not 0 <= p - dst < 0x400: continue
        impl = rd(src + p - dst)
        if '0x%08X' % impl in res and '0x%08X' % (base + a) not in res:
            res['0x%08X' % (base + a)] = res['0x%08X' % impl]; n += 1
    return n


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
    n = dispatch_thunks(img, base, md, res)
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print(len(res), 'functions,', n, 'D3DX dispatch thunks')


if __name__ == '__main__':
    main(*sys.argv[1:3])
