"""gfx_coverage.py -- does the graphics set cover every function that draws? An audit from the DLL.

The graphics set (ida_gfx.select) is: functions that reference a device global, plus name families,
plus IDA types mentioning D3D. This re-derives, from the code alone, every function that
  device   reads or writes a Direct3D device holder (g_pD3DHolder, g_pCDx, g_pDancerDevice, 0x10669418)
  d3dx     calls statically linked D3DX code (data/d3dx/d3dx81_manifest.json)
  calls    calls a function of the graphics set
and reports those outside the set, grouped by how they relate to it.

    python3 gfx_coverage.py FFXiMain_unpacked.dll EXPORT.jsonl OUT.json
"""
import json, os, sys, collections
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_REG_INVALID

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
DEVICE = {0x103D3CBC: 'g_pD3DHolder', 0x1045666C: 'g_pCDx', 0x1066941C: 'g_pDancerDevice', 0x10669418: 'dancer device holder'}


def main(dll, export, out):
    pe = pefile.PE(dll, fast_load=True)
    base = pe.OPTIONAL_HEADER.ImageBase; img = pe.get_memory_mapped_image()
    ext = {int(k, 16): v for k, v in json.load(open(os.path.join(D, 'func_extents.json'))).items()}
    d3dx = {int(k, 16) for k in json.load(open(os.path.join(D, 'd3dx', 'd3dx81_manifest.json')))}
    gfx = {json.loads(l)['ea']: json.loads(l)['name'] for l in open(export)}
    md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True
    rows = {}
    for f, ranges in ext.items():
        if f in d3dx: continue
        dev, dx, calls = set(), set(), set()
        for a, b in ranges:
            for i in md.disasm(img[a - base:b - base], a):
                for op in i.operands:
                    v = op.mem.disp & 0xFFFFFFFF if op.type == X86_OP_MEM and op.mem.base == X86_REG_INVALID else \
                        op.imm & 0xFFFFFFFF if op.type == X86_OP_IMM else None
                    if v in DEVICE: dev.add(DEVICE[v])
                if i.mnemonic in ('call', 'jmp') and i.operands and i.operands[0].type == X86_OP_IMM:
                    t = i.operands[0].imm & 0xFFFFFFFF
                    if t in d3dx: dx.add(t)
                    if t in gfx and t != f: calls.add(t)
        if dev or dx or calls:
            rows[f] = {'device': sorted(dev), 'd3dx': len(dx), 'calls_gfx': len(calls), 'in_set': f in gfx}
    outside = {f: r for f, r in rows.items() if not r['in_set']}
    summary = collections.Counter()
    for r in outside.values():
        summary['device' if r['device'] else 'd3dx' if r['d3dx'] else 'calls graphics only'] += 1
    json.dump({'summary': dict(summary), 'set_size': len(gfx),
               'outside': {'0x%08X' % f: r for f, r in sorted(outside.items())}}, open(out, 'w'), indent=0)
    print('graphics set', len(gfx), '| outside the set:', dict(summary))


if __name__ == '__main__':
    main(*sys.argv[1:4])
