"""field_types.py -- pointer types of class fields from the whole-program points-to result (points_to.py).

A field C::F (a 4-byte offset of a class with a proven vtable, data/ida_classes.json) is typed `T *` when,
in the points-to solution:
  * nothing is stored through an unbounded pointer anywhere (the "anywhere" set is empty) -- else no field
    is typed and the reason is the count;
  * in every object that holds C's vtable at offset 0 (allocation sites, data objects), no store of a value
    that is not a pointer reaches F (a store of the constant 0 is null, allowed), and no store at an unknown
    offset in that object does either;
  * every value that can reach F is a Direct3D object of one interface (T = that interface), or an object
    that holds one common class's vtable at offset 0 (T = that class); nothing else (data, API or external
    memory, code) can reach it.
Every result rests on the premises points_to.py states (P1-P3, P5-P8), listed with it.

    python3 field_types.py data/field_types.json        (reads runs/points_to_M.pkl)
"""
import collections, json, os, pickle, sys

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
R = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'runs')
PREMISES = ['P1', 'P2', 'P3', 'P5', 'P6', 'P7', 'P8']


def main(out):
    st = pickle.load(open(os.path.join(R, 'points_to_M.pkl'), 'rb'))
    M, NPW, ANYW = st['M'], st['NPW'], st['ANYW']
    classes = json.load(open(os.path.join(D, 'ida_classes.json')))['classes']
    vt2c = {int(c['vtable'], 16): n for n, c in classes.items()}
    cv = json.load(open(os.path.join(D, 'class_vtables.json')))
    for n, v in cv.items(): vt2c.setdefault(int(v['vtable'], 16), n)
    def vts(obj):
        return {off for (o, off) in M.get(obj, {}).get(0, ()) if o == ('D',) and off in vt2c}
    inst = collections.defaultdict(set)
    for obj in M:
        if obj[0] in ('H', 'DA') or obj == ('D',):
            for v in vts(obj): inst[v].add(obj)
    res = {'_doc': __doc__.split('\n\n')[0], 'premises': PREMISES, 'anywhere': len(ANYW), 'fields': {}}
    stats = collections.Counter()
    if ANYW:
        res['why_none'] = '%d targets can be written through an unbounded pointer; no field is provable' % len(ANYW)
    for cname, c in sorted(classes.items()):
        v = int(c['vtable'], 16); objs = inst.get(v, set())
        if not objs: stats['class with no instance in the solution'] += 1; continue
        for off_s, size, ty in c['fields']:
            off = int(off_s, 16)
            if size != 4 or ANYW: continue
            vals, bad = set(), None
            for o in objs:
                cells = M.get(o, {}); npw = NPW.get(o, set())
                if off in npw or None in npw: bad = 'a non-pointer value is stored'; break
                vals |= cells.get(off, set()) | cells.get(None, set())
            if bad is None and not vals: bad = 'no pointer reaches it'
            if bad is None:
                kinds = {o[0] for o, _ in vals}
                if kinds == {'X'} and len({o[1] for o, _ in vals}) == 1:
                    t = next(iter(vals))[0][1] + ' *'
                elif kinds <= {'H', 'DA', 'D'}:
                    common = None
                    for o, x in vals:
                        w = vts(o) if x in (0, None) else set()
                        common = w if common is None else common & w
                    names = sorted(vt2c[w] for w in common or ())
                    t = (names[0] + ' *') if len(names) == 1 else None
                    if t is None: bad = 'the objects reaching it share no single class (%s)' % (', '.join(names) or 'none')
                else: bad = 'reached by %s' % sorted(kinds)
            if bad: stats[bad] += 1; continue
            res['fields'].setdefault(cname, {})['0x%X' % off] = t; stats['typed'] += 1
    res['stats'] = dict(stats)
    json.dump(res, open(out, 'w'), indent=1, sort_keys=True)
    print('fields typed: %d; %s' % (stats['typed'], dict(stats)))


if __name__ == '__main__':
    main(sys.argv[1])
