"""dispatch_slots.py -- the functions a `jmp dword ptr [slot]` of the game's D3DX dispatch table can reach.

The statically linked D3DX picks its maths routines by CPU at run time. Each routine is a thunk `jmp dword ptr [slot]`
(0x102D6F03 for slot 0x103CC41C, ...), the slots form one table, and one function fills it, 0x102E7650 (read from the
game's code, 1 Nov):
  * it copies the default table (TABLE_DEFAULT, 0x39 dwords) over the slots (`rep movsd`), then
  * calls the fillers with the table's address, each after a CPU-feature test, and each filler stores constants
    into the table (`mov dword ptr [reg + off], imm`): the routines for that feature.
Before the first fill a slot holds its static value: a stub that calls the fill and jumps through the slot again.
So the targets of a slot are its static value, the default table's dword at the same offset, and what each filler
stores at that offset. That this is all the code that writes the table is read from the relocation table (the
loader rebases every absolute address the code holds): the only references into the table are each slot's own
thunks and the fill's two references to the table's base, and the fill's own calls are the fillers' only callers.
The script checks each of these and stops if one does not hold.

    FFXI_DLL=... python3 dispatch_slots.py OUT.json
Output {"0xINSN": {"slot": "0x...", "kind": "jmp" | "call", "targets": ["0x...", ...]}} for each jmp or call through a slot."""
import json, os, struct, sys
import capstone, pefile
from capstone.x86 import X86_OP_IMM, X86_OP_MEM
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dump_verify as D_V

FILL = 0x102E7650
TABLE, TABLE_DEFAULT, N = 0x103CC410, 0x103CC4F8, 0x39
D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')


def main(out):
    pe = pefile.PE(os.environ['FFXI_DLL'], fast_load=True); img = pe.get_memory_mapped_image(); B = pe.OPTIONAL_HEADER.ImageBase
    dw = lambda a: struct.unpack_from('<I', img, a - B)[0]
    blocks, _ = D_V.reloc_blocks(pe, img)
    rel = sorted(B + p + (e & 0xFFF) for p, es in blocks for e in es if e >> 12 == 3)
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    cs = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32); cs.detail = True
    def code(f):
        for a, b in ext['0x%x' % f]:
            yield from cs.disasm(bytes(img[a - B:b - B]), a)
    fill = list(code(FILL))
    # the fill: its references to the two tables, and the fillers it calls with the table's address
    reps = [i for i in fill if i.mnemonic.startswith('rep movs')]
    if not reps: sys.exit('dispatch_slots: 0x%08X has no rep movs' % FILL)
    # a filler: a call whose last push before it is EBX, which holds the table (`mov ebx, TABLE`, and nothing else in
    # the fill writes EBX but the push/pop that saves it)
    if [i.op_str for i in fill if 'ebx' in i.op_str.split(',')[0] and i.mnemonic not in ('push', 'pop')] != ['ebx, 0x%x' % TABLE]:
        sys.exit('dispatch_slots: EBX in the fill is not only the table')
    fillers, last_push = [], None
    for i in fill:
        if i.mnemonic == 'push': last_push = i.op_str
        elif i.mnemonic == 'call':
            if last_push == 'ebx' and i.operands[0].type == X86_OP_IMM: fillers.append(i.operands[0].imm)
            last_push = None
    # every relocated dword pointing into either table: where it is
    refs = [(r, dw(r)) for r in rel if TABLE <= dw(r) < TABLE + 4 * N or TABLE_DEFAULT <= dw(r) < TABLE_DEFAULT + 4 * N]
    fill_lo, fill_hi = ext['0x%x' % FILL][0]
    slot_refs = {}
    for r, v in refs:
        if fill_lo <= r < fill_hi:
            if v not in (TABLE, TABLE_DEFAULT): sys.exit('dispatch_slots: the fill refers to 0x%08X' % v)
            continue
        if not TABLE <= v < TABLE + 4 * N: sys.exit('dispatch_slots: 0x%08X refers to the default table' % r)
        slot_refs.setdefault(v, []).append(r)
    # each slot reference must be a `jmp dword ptr [slot]` or `call dword ptr [slot]` (the thunk and the stub): reads
    jmps = {}
    for v, rs in slot_refs.items():
        for r in rs:
            i = next(cs.disasm(bytes(img[r - 2 - B:r + 4 - B]), r - 2), None)
            if not (i and i.mnemonic in ('jmp', 'call') and i.operands[0].type == X86_OP_MEM and i.size == 6):
                sys.exit('dispatch_slots: the reference at 0x%08X to slot 0x%08X is not a jmp or call through it' % (r, v))
            jmps[r - 2] = (v, i.mnemonic)
    # the fillers are called only by the fill
    text = [s for s in pe.sections if s.Name.rstrip(b'\0') == b'.text'][0]
    t0 = B + text.VirtualAddress; t = bytes(img[text.VirtualAddress:text.VirtualAddress + text.Misc_VirtualSize])
    for f in fillers:
        callers = [t0 + k for k in range(len(t) - 5) if t[k] == 0xE8 and (t0 + k + 5 + struct.unpack_from('<i', t, k + 1)[0]) & 0xFFFFFFFF == f]
        if any(not fill_lo <= c < fill_hi for c in callers): sys.exit('dispatch_slots: filler 0x%08X has another caller' % f)
    stores = {}
    for f in fillers:
        for i in code(f):
            if i.mnemonic == 'mov' and len(i.operands) == 2 and i.operands[0].type == X86_OP_MEM and i.operands[1].type == X86_OP_IMM \
                    and i.operands[0].mem.base and not i.operands[0].mem.index and i.operands[0].size == 4:
                stores.setdefault(i.operands[0].mem.disp, set()).add(i.operands[1].imm & 0xFFFFFFFF)
            elif i.operands and i.operands[0].type == X86_OP_MEM and i.mnemonic not in ('cmp', 'test', 'push') \
                    and i.operands[0].mem.base and i.reg_name(i.operands[0].mem.base) not in ('ebp', 'esp'):
                # any other write through a pointer (the table's) leaves the slot's value unknown here
                sys.exit('dispatch_slots: filler 0x%08X writes through 0x%08X %s %s' % (f, i.address, i.mnemonic, i.op_str))
    res = {}
    for j, (v, kind) in sorted(jmps.items()):
        off = v - TABLE
        ts = {dw(v), dw(TABLE_DEFAULT + off)} | stores.get(off, set())
        res['0x%08X' % j] = {'slot': '0x%08X' % v, 'kind': kind, 'targets': ['0x%08X' % x for x in sorted(ts)]}
    res['_doc'] = __doc__.split('\n\n')[0]
    res['_fill'] = {'fill': '0x%08X' % FILL, 'fillers': ['0x%08X' % f for f in fillers], 'table': '0x%08X' % TABLE,
                    'default': '0x%08X' % TABLE_DEFAULT, 'slots': N}
    json.dump(res, open(out, 'w'), indent=1, sort_keys=True)
    print('%d jumps and calls through %d slots' % (len(jmps), len({v for v, _ in jmps.values()})))


if __name__ == '__main__':
    main(sys.argv[1])
