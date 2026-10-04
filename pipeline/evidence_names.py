"""evidence_names.py -- names for `sub_` functions that state a fact, and nothing else (de-artifacting phase 5).

The only rule that gives names today:
  * vtable slot: the function is at slot k of exactly one vtable V (vcall_resolve.vtables), at that one slot,
    V is the vtable of a named class C (data/ida_classes.json, data/class_vtables.json), and no instruction
    or dword in the image refers to any address inside V between its start and slot k, so the slot is reached
    only from V's start: name `C_vf<k>`.
Checked and giving none (counted in the output): a string in the binary naming the function. The strings the
graphics `sub_` functions use are file names, paths and UI text; none names a function.
Destructor or constructor shapes, call patterns and similar are not used (patterns, not proof).

    python3 evidence_names.py data/evidence_names.json        uses $FFXI_DLL
Output: {"0xFUNC": {"name", "vtable", "slot", "evidence"}, "_stats": {...}}; make_ida_fix.py passes the names
to the IDA pass for functions the IDB still calls `sub_`.
"""
import collections, json, os, struct, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sp_solve as S
import vcall_resolve as V
from capstone.x86 import X86_OP_IMM, X86_OP_MEM

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')


def main(out):
    im = S.Image(os.environ['FFXI_DLL']); base = im.base; img = bytes(im.img)
    ext = json.load(open(os.path.join(D, 'func_extents.json'))); starts = {int(k, 16) for k in ext}
    code = {int(k, 16): im.code(e) for k, e in ext.items()}
    vt = V.vtables(code, img, base, starts)
    subs = set()
    for l in open(os.path.join(D, 'exports', 'gfx_export.jsonl')):
        r = json.loads(l)
        if r['name'].startswith('sub_'): subs.add(r['ea'])
        subs.update(o['ea'] for n, o in r.get('objs', {}).items() if o['func'] and n.startswith('sub_'))
    cls = {int(c['vtable'], 16): n for n, c in json.load(open(os.path.join(D, 'ida_classes.json')))['classes'].items()}
    for n, c in json.load(open(os.path.join(D, 'class_vtables.json'))).items(): cls.setdefault(int(c['vtable'], 16), n)
    # every value the code or the data can use as an address: immediates, absolute memory operands, and the
    # image's dwords at every byte offset
    ref = set()
    for ins in code.values():
        for i in ins.values():
            for o in i.operands:
                if o.type == X86_OP_IMM: ref.add(o.imm & 0xFFFFFFFF)
                elif o.type == X86_OP_MEM and not o.mem.base and not o.mem.index: ref.add(o.mem.disp & 0xFFFFFFFF)
    for s in range(4): ref.update(struct.unpack_from('<%dI' % ((len(img) - s) // 4), img, s))
    member = collections.defaultdict(list)
    for v, sl in vt.items():
        for k, f in enumerate(sl): member[f].append((v, k))
    stats = collections.Counter(); res = {}
    for f in sorted(subs):
        m = member.get(f, [])
        if not m: stats['in no vtable'] += 1; continue
        if len({v for v, k in m}) > 1: stats['in several vtables'] += 1; continue
        if len(m) > 1: stats['at several slots of one vtable'] += 1; continue
        v, k = m[0]
        if any(v + 4 * j in ref for j in range(1, k + 1)): stats['its vtable is referred to between its start and the slot'] += 1; continue
        if v not in cls: stats['its vtable belongs to no named class'] += 1; continue
        stats['named (vtable slot)'] += 1
        res['0x%08X' % f] = {'name': '%s_vf%d' % (cls[v], k), 'vtable': '0x%08X' % v, 'slot': k,
                             'evidence': 'slot %d of vtable 0x%08X (%s), the only vtable and slot holding it; nothing refers '
                                         'inside the vtable before that slot' % (k, v, cls[v])}
    stats['named (string in the binary)'] = 0
    res['_stats'] = dict(stats, **{'sub_ functions': len(subs)})
    json.dump(res, open(out, 'w'), indent=1, sort_keys=True)
    print('%d sub_ functions: %s' % (len(subs), dict(stats)))


if __name__ == '__main__':
    main(sys.argv[1])
