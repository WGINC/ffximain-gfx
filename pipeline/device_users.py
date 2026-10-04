"""device_users.py -- functions outside the exported graphics set that use the Direct3D device, from the code.

The graphics set (ida/ida_gfx.py select) seeds on the device holders g_pD3DHolder, g_pCDx, g_pDancerDevice
and 0x10669418. A fourth holder is proven since (data/com_fields.json device_via_1047BFA8): the CMoDX object
at 0x1047BFA8 keeps the device at +0xEBC. Its users were never exported. A function is listed here when,
outside the current export and outside the statically linked D3DX code,
  holder   an instruction refers to one of the holder globals (0x1047BFA8 among them);
  com      it holds a D3D8 method call com_calls.py proves (data/com_calls.json);
  draw     it calls one of CDx's draw wrappers (CDx_DrawPrimitive, CDx_DrawIndexedPrimitive, CDx_DrawPrimitiveUP).
These are facts about the code (an address operand, a proven call, a direct call); none is a guess about
what the function does. make_ida_fix.py adds the list to the functions the IDA pass exports.

    python3 device_users.py data/gfx_device_users.json        uses $FFXI_DLL
"""
import collections, json, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sp_solve as S
from capstone.x86 import X86_OP_IMM, X86_OP_MEM

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
HOLDERS = {0x103D3CBC: 'g_pD3DHolder', 0x1045666C: 'g_pCDx', 0x1066941C: 'g_pDancerDevice', 0x10669418: '0x10669418',
           0x1047BFA8: 'CMoDX object 0x1047BFA8 (com_fields device_via_1047BFA8)'}
DRAW = {0x1000CB20: 'CDx_DrawPrimitive', 0x1000CC00: 'CDx_DrawIndexedPrimitive', 0x1000CD00: 'CDx_DrawPrimitiveUP'}


def main(out):
    im = S.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    have = {json.loads(l)['ea'] for l in open(os.path.join(D, 'exports', 'gfx_export.jsonl'))}
    d3dx = {int(k, 16) for k in json.load(open(os.path.join(D, 'd3dx', 'd3dx81_manifest.json')))}
    com = collections.defaultdict(list)
    for k, v in json.load(open(os.path.join(D, 'com_calls.json'))).items(): com[int(v['func'], 16)].append(k)
    res = {}
    for k, e in sorted(ext.items()):
        f = int(k, 16)
        if f in have or f in d3dx: continue
        why = collections.defaultdict(set)
        for a, i in im.code(e).items():
            for o in i.operands:
                v = o.imm if o.type == X86_OP_IMM else o.mem.disp if o.type == X86_OP_MEM and not o.mem.base and not o.mem.index else None
                if v is not None and (v & 0xFFFFFFFF) in HOLDERS: why['holder'].add(HOLDERS[v & 0xFFFFFFFF])
            if i.mnemonic == 'call' and i.operands and i.operands[0].type == X86_OP_IMM and i.operands[0].imm in DRAW:
                why['draw'].add(DRAW[i.operands[0].imm])
        if f in com: why['com'] = set(com[f])
        if why: res['0x%08X' % f] = {k_: sorted(v) for k_, v in sorted(why.items())}
    stats = collections.Counter(k_ for v in res.values() for k_ in v)
    json.dump({'_doc': __doc__.strip().split('\n')[0], '_stats': dict(stats, functions=len(res)), 'functions': res},
              open(out, 'w'), indent=1, sort_keys=True)
    print('%d functions outside the export use the device: %s' % (len(res), dict(stats)))


if __name__ == '__main__':
    main(sys.argv[1])
