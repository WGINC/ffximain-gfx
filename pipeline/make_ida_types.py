"""make_ida_types.py -- class types for the IDB, from the code: so Hex-Rays prints `this->field_1C` and
`this->vftable->Release(this)` instead of `*((_DWORD *)this + 7)` and `(*(int (__thiscall **)(...))(*this + 8))`.

Only what the code proves (vcall_resolve.analyse):
  * a method's class: `this` can hold exactly one vtable (every vtable whose slots hold the method, closed
    over callers passing their own `this`); the class is that vtable's name in class_vtables.json, else
    Vt_<vtable address>;
  * the vtable: its slots (the dwords from the vtable address while they are functions);
  * the fields: every [this + d] the class's methods access (entry ECX followed through register copies),
    with the operand width. An offset accessed at more than one width, or overlapping another, gets no field
    (Hex-Rays keeps its cast there). The kind is float when every access is an x87 load/store, else an
    unsigned integer of that width -- a field's pointee is never guessed.
Only classes with a method in the graphics export are emitted. The IDA script creates a type only where the
IDB has none of that name (the project's own structs, CDx and the like, are never replaced).

    python3 make_ida_types.py data/ida_classes.json        uses $FFXI_DLL
"""
import collections, json, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import vcall_resolve as V
from capstone.x86 import X86_OP_MEM

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
INT = {1: 'unsigned __int8', 2: 'unsigned __int16', 4: 'unsigned int', 8: 'unsigned __int64'}
FLT = {4: 'float', 8: 'double'}


def main(out):
    A = V.analyse()
    cv = json.load(open(os.path.join(D, 'class_vtables.json')))
    v2c = {int(v['vtable'], 16): c for c, v in cv.items()}
    exp = {json.loads(l)['ea'] for l in open(os.path.join(D, 'exports', 'gfx_export.jsonl'))}
    one = {f: next(iter(c)) for f, c in A['classes'].items()
           if isinstance(c, (set, frozenset)) and c is not V.TOP and len(c) == 1}
    want = {v for f, v in one.items() if f in exp}
    acc = collections.defaultdict(lambda: collections.defaultdict(collections.Counter))
    for f, v in one.items():
        if v not in want or not A['flows'].get(f): continue
        ins = A['code'][f]
        for a, (Sx, _, _) in A['flows'][f].items():
            i = ins[a]
            for o in i.operands:
                if o.type == X86_OP_MEM and o.mem.base and not o.mem.index and o.mem.segment == 0 \
                        and V.r32(i.reg_name(o.mem.base)) in Sx and o.mem.disp >= 0 and i.mnemonic != 'lea':
                    acc[v][o.mem.disp][(o.size, 'f' if i.mnemonic.startswith('f') else 'i')] += 1
    classes = {}; nf = dropped = 0
    for v in sorted(want):
        name = v2c.get(v, 'Vt_%08X' % v)
        cand = {}
        for d, c in acc[v].items():
            if d == 0: continue                                  # the vtable pointer
            sizes = {s for s, _ in c}
            if len(sizes) != 1 or sizes.pop() not in INT: dropped += 1; continue
            s = next(iter(c))[0]
            kinds = {k for _, k in c}
            cand[d] = (s, FLT[s] if kinds == {'f'} and s in FLT else INT[s])
        fields, end = [], 4
        for d in sorted(cand):
            s, t = cand[d]
            if d < end or any(d < e < d + s for e in cand if e != d): dropped += 1; continue
            fields.append(['0x%X' % d, s, t]); end = d + s
        nf += len(fields)
        classes[name] = {'vtable': '0x%08X' % v, 'slots': ['0x%08X' % t for t in A['vt_slots'][v]], 'fields': fields,
                         'methods': sorted('0x%08X' % f for f, x in one.items() if x == v)}
    json.dump({'_doc': __doc__.split('\n\n')[0], 'classes': classes}, open(out, 'w'), indent=0, sort_keys=True)
    print('%d classes (%d named), %d fields, %d offsets left untyped (several widths or overlapping); %d methods typed' % (
        len(classes), sum(not c.startswith('Vt_') for c in classes), nf, dropped, sum(len(c['methods']) for c in classes.values())))


if __name__ == '__main__':
    main(sys.argv[1])
