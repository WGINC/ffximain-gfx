"""this_class.py -- the class of `this` in each function, where the code settles it.

vcall_resolve.analyse() computes, for every function F, the vtables an object can have while F runs on it as
`this` (classes(F)): every vtable that holds F, and through each direct call the caller's own classes where it passes
its `this` unchanged; TOP (anything) where F's address is taken otherwise or a caller passes another object. Each
vtable is named by data/class_vtables.json. F's class is settled when every vtable it can see belongs to one class
of that file; otherwise it is not (several classes: which common base holds the fields is not known from this).

    FFXI_DLL=... python3 this_class.py data/this_class.json
Output: {"0xFUNC": {"class": name | null, "vtables": ["0x..."], "classes": [names], "why": "..."}} for every function
classes(F) is not TOP for; "_doc" with counts."""
import collections, json, os, sys
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import vcall_resolve as V


def main(out):
    A = V.analyse(); classes = A['classes']
    cv = json.load(open(os.path.join(P, '..', 'data', 'class_vtables.json')))
    vt_name = {int(v['vtable'], 16): k for k, v in cv.items()}
    res, n = {}, collections.Counter()
    for f, cl in sorted(classes.items()):
        if cl == V.TOP: n['TOP'] += 1; continue
        if not cl: n['no vtable reaches it'] += 1; continue
        got = {vt_name.get(x) for x in cl}; names = sorted(x for x in got if x)
        if None in got:
            r = {'class': None, 'why': 'a vtable class_vtables.json does not name'}
        elif len(names) == 1:
            r = {'class': names[0], 'why': 'every vtable it can see is %s\'s' % names[0]}
        else:
            r = {'class': None, 'why': '%d classes' % len(names)}
        r.update(vtables=['0x%08X' % x for x in sorted(cl)], classes=names)
        res['0x%08X' % f] = r; n['settled' if r['class'] else 'not settled: ' + r['why'].split(' ')[0]] += 1
    res['_doc'] = {'source': 'vcall_resolve.analyse() classes(F), vtables named by data/class_vtables.json',
                   'counts': dict(n)}
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print(dict(n))


if __name__ == '__main__':
    main(sys.argv[1])
