"""disasm.py -- disassemble a game function by address, from the DLL (no IDA needed).

    python3 disasm.py ADDR [ADDR...]          uses $FFXI_DLL and data/func_extents.json (all chunks)

Each line: address, bytes, instruction. Calls and jumps to named functions show the name from the
export or from data/names_pending_idb.json when one is known.
"""
import json, os, sys
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')


def names():
    n = {}
    for line in open(os.path.join(D, 'exports', 'gfx_export.jsonl')):
        r = json.loads(line)
        n.setdefault(r['ea'], r['name'])
        for nm, o in r['objs'].items():
            if not nm.startswith(('sub_', 'dword_', 'byte_', 'flt_', 'unk_')): n.setdefault(o['ea'], nm)
    for k, v in json.load(open(os.path.join(D, 'names_pending_idb.json'))).items():
        n[int(k, 16)] = v['name']
    return n


def main(addrs):
    pe = pefile.PE(os.environ['FFXI_DLL'], fast_load=True)
    base = pe.OPTIONAL_HEADER.ImageBase; img = pe.get_memory_mapped_image()
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    md = Cs(CS_ARCH_X86, CS_MODE_32); N = names()
    for a in addrs:
        ea = int(a, 16)
        print('%08X %s' % (ea, N.get(ea, '')))
        for s, e in ext.get('0x%x' % ea, [[ea, ea + 0x100]]):
            for i in md.disasm(img[s - base:e - base], s):
                ops = i.op_str
                for tok in ops.replace(',', ' ').replace('[', ' ').replace(']', ' ').split():
                    if tok.startswith('0x') and int(tok, 16) in N: ops += '    ; ' + N[int(tok, 16)]; break
                print('  %08X  %-8s %s' % (i.address, i.mnemonic, ops))
        print()


if __name__ == '__main__':
    main(sys.argv[1:])
