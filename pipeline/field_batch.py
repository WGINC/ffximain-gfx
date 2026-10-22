"""field_batch.py -- the fields of each object the tree's code works on, with every access, for naming sessions.

A field is named from all its uses at once: what writes it, what reads it, what it is compared with, what it is
passed to. A unit is one kind of object:
  * a class: the tree's placement (TREE/placement.json) or class_flow.py (data/class_flow.json); and
  * a function without a class whose object-sharing neighbours (it passes its own `this` unchanged to them, or they
    to it: data/obj_flow.json `own`) that have a class all have one, repeated: it works on that class's object. A
    helper shared by several classes takes none, so no two classes are merged; and
  * functions with no class around them, grouped with each other the same way: anonymous objects: a function passes its own `this`
    unchanged to another (data/obj_flow.json `own`), so both see one object with one layout. Units joined this way
    with no class are anonymous (named by their lowest function's address).
For each field (an offset from `this`) the dossier lists each access in the tree's verified code, as the line that
holds it, with the function's current name; the type the code reads it as; and the names the IDB already gives the
class's members (data/idb_members.json), marked as the IDB's.

    python3 field_batch.py TREE OUT_DIR [MIN_FUNCS]
Writes OUT_DIR/NNN_<unit>.md and OUT_DIR/index.json; units of at least MIN_FUNCS functions (default 2) that touch
at least one field. Naming sessions write data/fields/NNN_<unit>.json (batches/README.md, Fields)."""
import collections, json, os, re, sys
P = os.path.dirname(os.path.abspath(__file__)); D = os.path.join(P, '..', 'data'); sys.path.insert(0, P)
import batch as B
import hx2clean as H

ACC = re.compile(r'\*\s*\(\s*([\w\s\*]+?)\s*\*\s*\)\s*\(\s*\(\s*(?:int|uint32_t|char \*|uint8_t \*)\s*\)\s*(?:self_?|this)\s*\+\s*(0x[0-9a-fA-F]+|\d+)\s*\)'
                 r'|(?:\bthis|\bself_?)->(field_([0-9A-Fa-f]+)|\w+)\b')
LINES = 10


def main(tree, outdir, min_funcs='2'):
    min_funcs = int(min_funcs); os.makedirs(outdir, exist_ok=True)
    tf = B.tree_functions(tree); pl = json.load(open(os.path.join(tree, 'placement.json')))
    flow = json.load(open(os.path.join(D, 'obj_flow.json')))
    cf = json.load(open(os.path.join(D, 'class_flow.json')))['functions']
    idbm = json.load(open(os.path.join(D, 'idb_members.json')))
    dm = {int(k, 16): v[0] for k, v in json.load(open(os.path.join(D, 'dm_names.json'))).items() if not k.startswith('_')}
    rv = {int(k, 16): v for k, v in H.reviewed_names().items()}
    name = lambda a: dm.get(a) or (rv[a]['name'] if a in rv else tf[a][1] if a in tf else 'sub_%X' % a)
    # functions that share one object: a call passing the caller's own `this` unchanged (obj_flow `own`)
    nb = collections.defaultdict(set)
    for t, L in flow['calls'].items():
        for _, c, d in L:
            if d == 'own' and int(t, 16) != int(c, 16): nb[int(t, 16)].add(int(c, 16)); nb[int(c, 16)].add(int(t, 16))
    klass = {}
    for a in tf:
        c = (pl.get('0x%08x' % a) or pl.get(hex(a)) or {}).get('class') or (cf.get('0x%08X' % a) or {}).get('class')
        if c: klass[a] = c
    # a function with no class takes its neighbours' class where every neighbour with a class has that one (repeated;
    # a base-class helper called from several classes takes none, so classes are never merged through it)
    while True:
        new = {}
        for a in tf:
            if a in klass: continue
            cs = {klass[x] for x in nb.get(a, ()) if x in klass}
            if len(cs) == 1: new[a] = cs.pop()
        if not new: break
        klass.update(new)
    # the rest: grouped with the neighbours that have no class either (anonymous objects)
    par = {}
    def find(x):
        while par.get(x, x) != x: par[x] = par.get(par[x], par[x]); x = par[x]
        return x
    for a in tf:
        if a in klass: continue
        for x in nb.get(a, ()):
            if x in tf and x not in klass and not any(klass.get(y) for y in nb.get(x, ())) and not any(klass.get(y) for y in nb.get(a, ())):
                par[find(a)] = find(x)
    units = collections.defaultdict(set)
    for a in tf: units['class:' + klass[a] if a in klass else find(a)].add(a)
    out = []
    for root, fns in units.items():
        cnames = sorted({klass[a] for a in fns if a in klass})
        fields = collections.defaultdict(lambda: {'lines': [], 'types': collections.Counter(), 'named': set()})
        for a in sorted(fns):
            text = tf[a][2]
            for line in text.split('\n'):
                for m in ACC.finditer(line):
                    if m.group(2): off = int(m.group(2), 0); fields[off]['types'][m.group(1).strip()] += 1
                    elif m.group(4): off = int(m.group(4), 16)
                    else: continue                         # a member the tree already names: listed from the IDB below
                    f = fields[off]
                    if len(f['lines']) < LINES * 3: f['lines'].append((name(a), line.strip()[:220]))
        if not fields or len(fns) < min_funcs: continue
        unit = cnames[0] if cnames else 'obj_%08X' % min(fns)
        out.append((unit, fns, fields))
    out.sort(key=lambda u: (-len(u[2]), u[0]))
    index = []
    for n, (unit, fns, fields) in enumerate(out, 1):
        bid = '%03d_%s' % (n, re.sub(r'\W', '_', unit)[:40])
        L = ['# Fields %s: %d fields of `%s`, used by %d functions' % (bid, len(fields), unit, len(fns)), '',
             'Name these fields: see `batches/README.md` (Fields). Write `data/fields/%s.json`.' % bid, '']
        im = {m[1]: m[0] for m in (idbm.get(unit) or {}).get('members', []) if not m[0].startswith(('_pad', 'field_', 'base'))}
        if im: L += ['The IDB already names these members of `%s` (offset: name; the IDB\'s, from earlier sessions or the '
                     'owner, not proven): %s' % (unit, ', '.join('+0x%X: `%s`' % (k, v) for k, v in sorted(im.items()))), '']
        L += ['Functions of this object: ' + ', '.join('`%s`' % name(a) for a in sorted(fns)[:60])
              + (' and %d more' % (len(fns) - 60) if len(fns) > 60 else ''), '']
        for off in sorted(fields):
            f = fields[off]
            t = ', '.join('%s x%d' % (k, v) for k, v in f['types'].most_common(3))
            L += ['## `+0x%X`%s' % (off, (' (IDB: `%s`)' % im[off]) if off in im else ''), '',
                  '- read/written as: %s; %d accesses shown' % (t or 'named member', min(len(f['lines']), LINES)), '']
            for fn, line in f['lines'][:LINES]: L.append('  - in `%s`: `%s`' % (fn, line.replace('`', "'")))
            L.append('')
        open(os.path.join(outdir, bid + '.md'), 'w').write('\n'.join(L))
        index.append({'batch': bid, 'unit': unit, 'functions': ['0x%08X' % a for a in sorted(fns)],
                      'fields': ['0x%X' % o for o in sorted(fields)]})
    json.dump({'_doc': {'tree': tree, 'units': len(index), 'fields': sum(len(u['fields']) for u in index)}, 'batches': index},
              open(os.path.join(outdir, 'index.json'), 'w'), indent=1)
    print('%d units, %d fields, %d functions' % (len(index), sum(len(u['fields']) for u in index),
                                                 sum(len(u['functions']) for u in index)))


if __name__ == '__main__':
    main(*sys.argv[1:4])
