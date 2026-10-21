"""vtable_bounds.py -- ends each class's slot list in data/class_vtables.json where the code shows the vtable ends.

The slot lists came from a scan that read on while the dwords looked like code addresses, up to a cap of 400;
124 of the 226 ran into the next object. A list now ends at the first of:

  * an address some instruction refers to (an immediate or an absolute memory operand, data/func_extents.json's
    code): that is another object, most often the next vtable, which its constructor stores;
  * a dword that is not a function's first instruction (data/func_extents.json): no slot can hold it.

Each slot kept is a function start that lies before any address the code names; whether a class uses its last
kept slots is not shown by this. The vtable address itself is not changed.

    FFXI_DLL=... python3 vtable_bounds.py [--check]"""
import bisect, json, os, sys
from capstone.x86 import X86_OP_IMM, X86_OP_MEM
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import sp_solve as S
D = os.path.join(P, '..', 'data')


def main(check=False):
    im = S.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    starts = {int(k, 16) for k in ext}
    refs = set()
    for e in ext.values():
        for i in im.code(e).values():
            for o in i.operands:
                if o.type == X86_OP_IMM: refs.add(o.imm & 0xFFFFFFFF)
                elif o.type == X86_OP_MEM and not o.mem.base and not o.mem.index: refs.add(o.mem.disp & 0xFFFFFFFF)
    R = sorted(refs)
    path = os.path.join(D, 'class_vtables.json'); cv = json.load(open(path))
    changed = 0
    for n, c in cv.items():
        vt = int(c['vtable'], 16)
        j = bisect.bisect_right(R, vt); end = R[j] if j < len(R) else 1 << 32
        k = 0
        while k < len(c['slots']) and vt + 4 * k < end and im.u32(vt + 4 * k) in starts:
            k += 1
        if [int(s, 16) for s in c['slots'][:k]] != [im.u32(vt + 4 * i) for i in range(k)]:
            sys.exit('%s: slot list does not match the image' % n)
        if k != len(c['slots']):
            changed += 1; print('%-30s %s: %d -> %d slots' % (n, c['vtable'], len(c['slots']), k))
            c['slots'] = c['slots'][:k]
    print('%d of %d slot lists end earlier' % (changed, len(cv)))
    if check: sys.exit(1 if changed else 0)
    json.dump(cv, open(path, 'w'))


if __name__ == '__main__':
    main('--check' in sys.argv)
