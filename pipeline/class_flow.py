"""class_flow.py -- the class of the object each function works on (its `this`), where the code settles it.

Starts from what the binary states and carries it along the calls (data/obj_flow.json), to a fixpoint:

  * seeds: a function held in a class's vtable (data/class_vtables.json), and a function that stores a class's
    vtable into its own object (`const` at own+0: a constructor or destructor of that class);
  * a function every one of whose direct call sites passes an object of one class, and whose address is used
    nowhere but in direct calls (a vtable slot, a table, a callback argument would hand it other objects), has that
    class. The object a call site passes is known when it is:
      - the caller's own `this` (`own`), the caller's class known;
      - a global whose class is known (data/global_objects.json);
      - the pointer in a field of the caller's own object (`field own+K`), that field's class known;
  * a field's class: every value that methods of one class store into their object at K (`stores`) is an object of
    one class (the caller's own `this`, a global, a field of its own, all as above), and there is at least one.
A function reached with objects of two classes, or from a call site whose object is not known, is not settled.
`own+K` (a sub-object) is not followed: its class is not known from the outer one.

    FFXI_DLL=... python3 class_flow.py data/class_flow.json
Output {"functions": {"0xF": {"class": C, "how": "vtable" | "stores the vtable" | "callers: ..."}},
        "fields": {"C": {"0xK": D}}, "_counts": {...}}"""
import collections, json, os, re, struct, sys
from capstone.x86 import X86_OP_IMM, X86_OP_MEM
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import sp_solve as S
D = os.path.join(P, '..', 'data')
TOP = 'TOP'


def main(out):
    ld = lambda f: json.load(open(os.path.join(D, f)))
    im = S.Image(os.environ['FFXI_DLL']); ext = ld('func_extents.json')
    flow = ld('obj_flow.json'); cv = ld('class_vtables.json')
    starts = {int(k, 16) for k in ext if not k.startswith('_')}
    vt_class = {int(v['vtable'], 16): c for c, v in cv.items()}
    slot_of = collections.defaultdict(set)                    # function -> classes whose vtable holds it
    slot_words = set()                                        # image addresses of vtable slots
    for c, v in cv.items():
        for k, s in enumerate(v['slots']):
            slot_of[int(s, 16)].add(c); slot_words.add(int(v['vtable'], 16) + 4 * k)
    # globals: name -> address (the IDB export's data objects), then their known classes
    gaddr = {}
    for l in open(os.path.join(D, 'exports', 'gfx_export.jsonl')):
        for nm, o in (json.loads(l).get('objs') or {}).items():
            if not o.get('func'): gaddr.setdefault(nm, o['ea'])
    gcls = {}
    for nm, v in ld('global_objects.json').items():
        a = gaddr.get(nm) or (int(nm.split('_')[-1], 16) if re.match(r'^(dword|off|unk)_[0-9A-F]{8}$', nm) else None)
        if a is not None: gcls[a] = v['class']
    # every direct call site of each function, and every other use of its address
    sites = collections.Counter(); taken = set()
    for k, e in ext.items():
        if k.startswith('_'): continue
        for a, i in im.code(e).items():
            for n_, op in enumerate(i.operands):
                if op.type == X86_OP_IMM and op.imm in starts:
                    if i.mnemonic in ('call', 'jmp') and n_ == 0:
                        if i.mnemonic == 'call': sites[op.imm] += 1
                        else: taken.add(op.imm)                    # a tail jump: its caller's callers reach it
                    else: taken.add(op.imm)
                elif op.type == X86_OP_MEM and not op.mem.base and not op.mem.index and (op.mem.disp & 0xFFFFFFFF) in starts:
                    taken.add(op.mem.disp & 0xFFFFFFFF)
    img = im.img
    for o in range(0, len(img) - 3, 4):
        w = struct.unpack_from('<I', img, o)[0]
        if w in starts and im.base + o not in slot_words: taken.add(w)
    # seeds
    seed = {f: set(cs) for f, cs in slot_of.items()}
    stores = {int(f, 16): v for f, v in flow['stores'].items()}
    for f, L in stores.items():
        vs = {vt_class[int(d.split()[1], 16)] for _, off, d in L if off == '0x0' and d.startswith('const ') and int(d.split()[1], 16) in vt_class}
        if vs: seed.setdefault(f, set()).update(vs)
    calls = {int(t, 16): v for t, v in flow['calls'].items()}
    cls = {f: frozenset(v) for f, v in seed.items()}
    how = {f: 'vtable' if f in slot_of else 'stores the vtable' for f in seed}
    fcls = {}

    def obj_class(d, f):
        """the class of the object a value names inside function f, or None (not known)"""
        c = cls.get(f)
        if d == 'own': return c if c and len(c) == 1 else None
        if d.startswith('global '): g = gcls.get(int(d.split()[1], 16)); return frozenset([g]) if g else None
        if d.startswith('field own+') and c and len(c) == 1:
            x = fcls.get((next(iter(c)), int(d.split('+')[1], 16)))
            return frozenset([x]) if x and x != TOP else None
        return None

    rounds = 0
    while True:
        rounds += 1; changed = 0
        # fields: what the methods of a settled class store into their object
        nf = {}
        for f, L in stores.items():
            c = cls.get(f)
            if not c or len(c) != 1: continue
            for _, off, d in L:
                if d.startswith('const'): continue
                oc = obj_class(d, f); key = (next(iter(c)), int(off, 16))
                if oc is None or len(oc) != 1: continue
                x = next(iter(oc))
                nf[key] = x if nf.get(key, x) == x else TOP
        if nf != fcls: fcls = nf; changed += 1
        for t, L in calls.items():
            if t in seed or t in taken: continue
            if len(L) != sites.get(t, 0): continue                     # a call site whose object is not known
            cs = set()
            for _, caller, d in L:
                oc = obj_class(d, int(caller, 16))
                if oc is None: cs = None; break
                cs |= oc
            if not cs or len(cs) != 1: continue
            new = frozenset(cs)
            if cls.get(t) != new:
                cls[t] = new; changed += 1
                how[t] = 'callers: ' + ', '.join(sorted({d for _, _, d in L}))
        if not changed: break
    res = {'functions': {'0x%08X' % f: {'class': next(iter(c)), 'how': how[f]} for f, c in sorted(cls.items()) if len(c) == 1},
           'fields': {}}
    for (c, k), x in sorted(fcls.items()):
        if x != TOP: res['fields'].setdefault(c, {})['0x%X' % k] = x
    n = collections.Counter(v['how'].split(':')[0] for v in res['functions'].values())
    res['_counts'] = {'settled': len(res['functions']), 'by': dict(n), 'fields typed': sum(len(v) for v in res['fields'].values()),
                      'rounds': rounds}
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print(res['_counts'])


if __name__ == '__main__':
    main(sys.argv[1])
