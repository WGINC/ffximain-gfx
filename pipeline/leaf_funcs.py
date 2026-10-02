"""leaf_funcs.py -- small leaf functions the gate runs for real instead of stubbing.

A stub logs a call and returns a fuzzed value, but it cannot do what the callee does to memory. For
a trivial accessor such as `*out = this->field_B4; ret 4` (CXiSkeletonActor vtable slot 0x1E0) that
loses the only effect the caller depends on. A leaf -- one contiguous chunk of at most MAX bytes,
no call, no jump outside itself, no indirect jump, no segment-register or privileged instruction,
ending in ret -- is plain game code with no further dependencies: running it is exact and identical
on both sides, and its writes are compared like the caller's own.

    python3 leaf_funcs.py FFXiMain_unpacked.dll data/leaf_funcs.json
Output: {"0xADDR": ["0xGLOBAL", ...]} -- each leaf with the .data globals it reads or writes by
absolute address. A case that runs a leaf fuzzes those globals as well (gfx_cases.py): the leaf now
reads them, where its stub did not (a camera getter reading g_pCamera).
"""
import json, os, sys
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
MAX = 64
BANNED = ('call', 'int', 'int3', 'into', 'hlt', 'in', 'out', 'ins', 'outs', 'cli', 'sti', 'lcall', 'ljmp',
          'sysenter', 'syscall', 'rdtsc', 'cpuid', 'lock')


DATA = (0x1034F000, 0x109FA000)


def globals_of(md, img, base, ranges):
    a, b = ranges[0]; out = set()
    for i in md.disasm(img[a - base:b - base], a):
        for op in i.operands:
            if op.type == 3 and not op.mem.base and not op.mem.index and DATA[0] <= op.mem.disp < DATA[1]:
                out.add('0x%08X' % op.mem.disp)
    return sorted(out)


def is_leaf(md, img, base, ranges):
    if len(ranges) != 1: return False
    a, b = ranges[0]
    if b - a > MAX: return False
    ins = list(md.disasm(img[a - base:b - base], a))
    if not ins or sum(i.size for i in ins) != b - a or not ins[-1].mnemonic.startswith('ret'): return False
    for i in ins:
        m = i.mnemonic
        if m in BANNED or m.startswith('rep') and 'stos' not in m and 'movs' not in m: return False
        if any(r in i.op_str for r in ('fs:', 'gs:', 'cr', 'dr')): return False
        if m.startswith('j') or m.startswith('loop'):
            op = i.operands[0] if i.operands else None
            if op is None or op.type != X86_OP_IMM or not (a <= op.imm < b): return False
    return True


def main(dll, out):
    pe = pefile.PE(dll, fast_load=True); base = pe.OPTIONAL_HEADER.ImageBase; img = pe.get_memory_mapped_image()
    md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True
    res = {'0x%08X' % int(k, 16): globals_of(md, img, base, r)
           for k, r in json.load(open(os.path.join(D, 'func_extents.json'))).items() if is_leaf(md, img, base, r)}
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print(len(res), 'leaf functions')


if __name__ == '__main__':
    main(*sys.argv[1:3])
