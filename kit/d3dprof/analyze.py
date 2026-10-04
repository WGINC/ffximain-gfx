"""analyze.py -- a d3dprof CSV joined with the code: each call site checked and named.

    python3 analyze.py d3dprof_<pid>.csv [OUT.json]        uses $FFXI_DLL

A row's call site is checked against the analysed DLL. The instruction ending at the return address must
be one of two calls (slot order from the D3D8 header, data/com_spec.json):
  * an indirect call through the method's own vtable slot (`call [reg + 4*slot]`);
  * a direct call to a function that leaves through `jmp [reg + 4*slot]` (a tail call: the method returns
    to that function's caller).
Only then is the row attributed to FFXiMain. Rows that fail stay unattributed.

The first d3dprof build marked other-module callers by setting bit 0 of the return address. That bit is
not free: a return address can be odd. So in those CSVs, a FFXiMain site with an odd return address
appears as `other module` with bit 0 cleared. For such a row both v and v|1 are tried, and the slot check
decides. The process also holds polcore.dll at 0x10010000..0x10461000, inside the analysed image's range,
which is why an address in the range is not enough on its own.
"""
import collections, csv, json, os, sys
HERE = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.join(HERE, '..', '..')
sys.path.insert(0, os.path.join(ROOT, 'pipeline'))
import sp_solve as S
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM

D = os.path.join(ROOT, 'data')
SLOT = {m['name']: k for k, m in enumerate(json.load(open(os.path.join(D, 'com_spec.json')))['IDirect3DDevice8'])}


def main(path, out=None):
    im = S.Image(os.environ['FFXI_DLL']); md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    spans = sorted((a, b, int(k, 16)) for k, e in ext.items() for a, b in e)
    names = {json.loads(l)['ea']: json.loads(l)['name'] for l in open(os.path.join(D, 'exports', 'gfx_export.jsonl'))}
    users = {int(k, 16) for k in json.load(open(os.path.join(D, 'gfx_device_users.json')))['functions']}
    rmap = json.load(open(os.path.join(D, 'render_map.json')))['sites']

    def func_of(a):
        for s, e, f in spans:
            if s <= a < e: return f
        return None

    def tail_jumps_through(f, slot):
        """function f leaves through `jmp [reg + 4*slot]` (a wrapper that tail-calls the method, as 0x1006AE30
        tail-calls ApplyStateBlock): the method then returns straight to f's caller"""
        e = ext.get('0x%08x' % f)
        return e is not None and any(
            i.mnemonic == 'jmp' and i.operands[0].type == X86_OP_MEM and i.operands[0].mem.base
            and not i.operands[0].mem.index and i.operands[0].mem.disp == 4 * slot for i in im.code(e).values())

    def call_ending(ret, slot):
        """the call instruction ending at ret, if it calls through vtable slot `slot`, or calls a function
        that tail-jumps through it"""
        if not (im.base + 8 <= ret < im.base + len(im.img)): return None
        for L in (3, 6, 2, 7, 5):
            for i in md.disasm(bytes(im.img[ret - L - im.base:ret - im.base]), ret - L):
                if i.mnemonic == 'call' and i.address + i.size == ret:
                    o = i.operands[0]
                    if o.type == X86_OP_MEM and o.mem.base and not o.mem.index and o.mem.disp == 4 * slot: return i
                    if o.type == X86_OP_IMM and tail_jumps_through(o.imm, slot): return i
                break
        return None

    head = open(path).readline().strip(); v2 = head.startswith('# d3dprof v2'); wrapped = []
    WRAPPERS = {0x1000CB20: 'CDx_DrawPrimitive', 0x1000CC00: 'CDx_DrawIndexedPrimitive', 0x1000CD00: 'CDx_DrawPrimitiveUP'}
    rows = list(csv.DictReader(l for l in open(path) if not l.startswith('#')))
    res, stats = [], collections.Counter()
    for r in rows:
        v = int(r['call_site'], 16); slot = SLOT.get(r['method'])
        if r['caller'] == 'caller of CDx draw wrapper':
            # the draw's requester: a direct call to the CDx wrapper must end at v
            i = next((i for L in (5,) for i in md.disasm(bytes(im.img[v - L - im.base:v - im.base]), v - L)
                      if i.mnemonic == 'call' and i.address + i.size == v and i.operands[0].type == X86_OP_IMM
                      and i.operands[0].imm in WRAPPERS), None) if im.base + 8 <= v < im.base + len(im.img) else None
            f = func_of(i.address) if i else None
            wrapped.append({'method': r['method'], 'calls': float(r['calls_per_frame']), 'prims': float(r['primitives_per_frame']),
                            'ret': '0x%08X' % v, 'call': '0x%08X' % i.address if i else None, 'func': '0x%08X' % f if f else None,
                            'name': names.get(f) or ('(device user, next export)' if f in users else None) if f else None,
                            'checked': i is not None})
            stats['draw requester (call to a CDx wrapper checked)' if i else 'draw requester (not checked)'] += 1
            continue
        # v2 CSVs give every address exactly; the first format cleared bit 0 of FFXiMain sites it marked as
        # other-module, so there both v|1 and v are tried
        cands = [v] if r['caller'] == 'FFXiMain' else ([] if v2 else [v | 1, v])
        hit = next(((c, i) for c in cands for i in [call_ending(c, slot)] if i is not None), None)
        e = {'method': r['method'], 'calls': float(r['calls_per_frame']), 'redundant': float(r['redundant_per_frame']),
             'prims': float(r['primitives_per_frame']), 'csv_site': r['call_site'], 'csv_caller': r['caller']}
        if hit:
            c, i = hit; f = func_of(i.address)
            e.update(ret='0x%08X' % c, call='0x%08X' % i.address, func='0x%08X' % f if f else None,
                     name=names.get(f) or ('(device user, next export)' if f in users else None),
                     in_export=f in names, in_render_map='0x%08X' % i.address in rmap)
            stats['FFXiMain, slot checked' + (' (bit-0 corrected)' if c != v else '')] += 1
        else:
            stats['not attributed (no call through that slot ends there)'] += 1
        res.append(e)
    by_func = collections.defaultdict(lambda: collections.Counter())
    for e in res:
        k = e.get('func') or 'unattributed'
        by_func[k]['calls'] += e['calls']; by_func[k]['redundant'] += e['redundant']; by_func[k]['prims'] += e['prims']
        if e['method'].startswith('Draw'): by_func[k]['draws'] += e['calls']
    report = {'header': head, 'stats': dict(stats), 'rows': sorted(res, key=lambda e: -e['calls']),
              'by_function': {k: dict(v, name=next((e.get('name') for e in res if e.get('func') == k), None))
                              for k, v in sorted(by_func.items(), key=lambda kv: -kv[1]['calls'])}}
    report['draw_requesters'] = sorted(wrapped, key=lambda e: -e['calls'])
    if out: json.dump(report, open(out, 'w'), indent=1)
    print(head); print(dict(stats))
    print('\nby function (per frame): calls, redundant, draws, primitives')
    for k, v in list(report['by_function'].items())[:25]:
        print('  %-11s %-44s %8.1f %8.1f %7.1f %8.1f' % (k, (v['name'] or '')[:44], v['calls'], v['redundant'], v.get('draws', 0), v['prims']))
    if wrapped:
        print('\nwho asks for the draws (callers of the CDx draw wrappers, per frame): calls, primitives')
        for e in report['draw_requesters'][:25]:
            print('  %-22s %-11s %-40s %8.1f %8.1f%s' % (e['method'], e['func'], (e['name'] or '')[:40], e['calls'], e['prims'], '' if e['checked'] else '  (not checked)'))
    return report


if __name__ == '__main__':
    main(sys.argv[1], sys.argv[2] if len(sys.argv) > 2 else None)
