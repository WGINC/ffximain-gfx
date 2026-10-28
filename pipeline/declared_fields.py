"""declared_fields.py -- the typing batches' fields that carry a definitive type, as data/declared_fields.json.

Reads batches/r6_*.json (WORKERS.md typing batches: per class, per offset, a name, a type and its evidence, each with a
confidence). A field is declared only where the batch gives it confidence `high`, a name, and a type the tree's headers
declare as written (TYPES: the D3D8 interfaces and structs of pipeline/xi_d3d8.hpp, the fixed-width integers, float,
HCURSOR); its evidence is kept with it. hx2clean.py takes these as it takes the IDB's named fields (KNOWN_FIELDS):
the type and the name in the class's declaration, no inferred field overlapping it. A field the IDB already declares
(data/ida_named_fields.json) keeps the IDB's declaration; a batch that disagrees with it is printed, not applied.
Medium-confidence names (`name_confidence` or `confidence` medium, a name given) go to data/field_names.json, which
names an inferred field without changing its type -- only where that file has no name for the field yet.

    python3 pipeline/declared_fields.py            (writes data/declared_fields.json, updates data/field_names.json)
"""
import glob, json, os, re
ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')
TYPES = {'float': 'float', 'int16_t': 'int16_t', 'int32_t': 'int32_t', 'uint8_t': 'uint8_t', 'DWORD': 'uint32_t',
         'IDirect3D8 *': 'IDirect3D8 *', 'IDirect3DDevice8 *': 'IDirect3DDevice8 *',
         'IDirect3DTexture8 *': 'IDirect3DTexture8 *', 'IDirect3DSurface8 *': 'IDirect3DSurface8 *',
         'D3DMATRIX': 'D3DMATRIX', 'D3DVIEWPORT8': 'D3DVIEWPORT8', 'D3DCAPS8': 'D3DCAPS8', 'HCURSOR': 'HCURSOR'}
SIZE = {'float': 4, 'int16_t': 2, 'int32_t': 4, 'uint8_t': 1, 'uint32_t': 4, 'D3DMATRIX': 64, 'D3DVIEWPORT8': 24,
        'D3DCAPS8': 212, 'HCURSOR': 4}
IDENT = re.compile(r'^[A-Za-z_]\w*$')


def main():
    idb = json.load(open(os.path.join(ROOT, 'data', 'ida_named_fields.json')))
    idb_at = {(c, off): (n, t) for c, d in idb.items() for off, n, t, w in d['members']}
    declared, names, skipped = {}, {}, []
    for f in sorted(glob.glob(os.path.join(ROOT, 'batches', 'r6_*.json'))):
        batch = os.path.relpath(f, ROOT)
        for cls, fields in json.load(open(f)).items():
            if cls.startswith('_'): continue
            for o, e in fields.items():
                if not isinstance(e, dict): continue
                off = int(o, 16); nm = e.get('name')
                if not nm or not IDENT.match(nm) or off == 0: continue
                ty = TYPES.get((e.get('type') or '').strip())
                if e.get('confidence') == 'high' and ty:
                    w = 4 if ty.endswith('*') else SIZE[ty]
                    if e.get('width') not in (None, w):
                        skipped.append('%s+0x%X: width %s, type %s is %d' % (cls, off, e.get('width'), ty, w)); continue
                    if (cls, off) in idb_at:
                        if idb_at[(cls, off)] != (nm, ty): skipped.append('%s+0x%X: the IDB declares %s; %s %s not applied' % (cls, off, idb_at[(cls, off)], ty, nm))
                        continue
                    declared.setdefault(cls, {})['0x%X' % off] = {'name': nm, 'type': ty, 'width': w, 'evidence': e.get('evidence'), 'batch': batch}
                elif 'medium' in (e.get('confidence'), e.get('name_confidence')):
                    names.setdefault(cls, {})['0x%X' % off] = {'name': nm, 'confidence': 'medium',
                        'summary': e.get('summary') or '', 'evidence': e.get('name_evidence') or e.get('evidence') or '', 'batch': batch}
    json.dump({'_doc': __doc__.split('\n\n')[0], **{c: dict(sorted(d.items(), key=lambda kv: int(kv[0], 16))) for c, d in sorted(declared.items())}},
              open(os.path.join(ROOT, 'data', 'declared_fields.json'), 'w'), indent=1)
    fp = os.path.join(ROOT, 'data', 'field_names.json'); fn = json.load(open(fp)); added = 0
    for c, d in names.items():
        for o, v in d.items():
            if (c in declared and o in declared[c]) or o.lower() in {k.lower() for k in fn.get(c, {})}: continue
            fn.setdefault(c, {})[o] = v; added += 1
    json.dump(fn, open(fp, 'w'), indent=1)
    print('declared %d fields in %d classes; %d medium names added to field_names.json; %d skipped' %
          (sum(len(d) for d in declared.values()), len(declared), added, len(skipped)))
    for s in skipped: print('  skipped', s)


if __name__ == '__main__':
    main()
