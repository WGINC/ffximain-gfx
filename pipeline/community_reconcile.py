"""community_reconcile.py -- the community's names set against ours: what is adopted, what is the same, what to measure.

The owner's rule (22 Oct): where the community has already worked something out, trust it; where it is not
documented, or conflicts with our own, measure it. Inputs: community_scan.py's located names, community_structs.py's
layouts, data/community_objects.json (which of our objects each community struct is, with the evidence), and our
names: DancingMad's (data/dm_names.json), the IDB's and the Ghidra project's, the naming sessions' (data/names/).

  * a function: the community's name is adopted where ours is a placeholder or a naming session's reading (any
    confidence); where ours is DancingMad's or the IDB's and says another thing, it is a conflict (both kept, to
    measure). A name that is a symbol (`ntGameMonthGet`, `FsMacroContainer::setPage`) is taken as written; an Ashita
    pointer label (`chat.addchatmessage`) is not a symbol and is kept as an alias only.
  * a global: only Windower 5's struct locations name the global itself (the struct is laid over the object the
    global points to, or over the globals there); an Ashita pointer often names what is reached through it with
    offsets (ashita.offsets.ini), so it is kept as an alias.
  * a field of a mapped object: each source's name at the offset; where both sources name it they must agree, and
    where we name it (DancingMad's field names, the IDB's members) ours must agree, else it is a conflict.
"Agree" here is spelt the same once case and separators are dropped; anything else that differs is listed for
review or measurement, never decided by likeness.

    python3 community_reconcile.py SCAN.json STRUCTS.json OUT.json
Output {"functions": {...}, "globals": {...}, "fields": {...}, "measure": [...]} with each entry's sources and status."""
import json, os, re, sys
P = os.path.dirname(os.path.abspath(__file__)); D = os.path.join(P, '..', 'data'); sys.path.insert(0, P)
import hx2clean as H

PLACE = re.compile(r'^(?:sub_|fn_|FUN_|nullsub)')
norm = lambda s: re.sub(r'[^a-z0-9]', '', s.lower().split('::')[-1] if '::' in s else s.lower())


def main(scan, structs, out):
    sc = json.load(open(scan)); st = json.load(open(structs))
    objs = json.load(open(os.path.join(D, 'community_objects.json')))
    judged = json.load(open(os.path.join(D, 'community_judged.json'))) if os.path.exists(os.path.join(D, 'community_judged.json')) else {}
    dm = {int(k, 16): v[0] for k, v in json.load(open(os.path.join(D, 'dm_names.json'))).items() if not k.startswith('_')}
    rv = {int(k, 16): v for k, v in H.reviewed_names().items()}
    gpath = os.path.join(D, '..', 'runs', 'ghidra', 'names.json')
    gh = {int(k, 16): v for k, v in json.load(open(gpath)).items()} if os.path.exists(gpath) else {}
    res = {'functions': {}, 'globals': {}, 'fields': {}, 'measure': []}
    for k, L in sorted(sc['functions'].items()):
        a = int(k, 16)
        syms = sorted({x['name'] for x in L if x['source'] == 'ashita addon'})
        labels = sorted({x['name'] for x in L if x['source'] != 'ashita addon'})
        ours, whose = (dm[a], 'DancingMad') if a in dm else (rv[a]['name'], 'naming session (%s)' % rv[a]['confidence']) if a in rv \
            else (gh.get(a), 'IDB/Ghidra') if gh.get(a) and not PLACE.match(gh[a]) else (None, None)
        e = {'community': syms, 'aliases': labels, 'ours': ours, 'ours_from': whose, 'sources': sorted({x['source'] + ': ' + x['file'] for x in L})}
        if not syms: e['status'] = 'alias only (a pointer label, not a symbol)'
        elif ours is None or whose.startswith('naming session'): e['status'] = 'adopted'; e['name'] = syms[0]
        elif any(norm(s) == norm(ours) for s in syms): e['status'] = 'the same'
        else:
            e['status'] = 'conflict: measure'
            res['measure'].append({'kind': 'function', 'addr': k, 'community': syms, 'ours': ours, 'ours_from': whose})
        res['functions'][k] = e
    for k, L in sorted(sc['globals'].items()):
        res['globals'][k] = {'windower5': sorted({x['name'] for x in L if x['source'].startswith('windower')}),
                             'ashita_alias': sorted({x['name'] for x in L if not x['source'].startswith('windower')})}
    # fields of the objects data/community_objects.json maps
    fn = {}
    for c, v in json.load(open(os.path.join(D, 'field_names.json'))).items():
        for off, x in v.items(): fn[(c, int(off, 16))] = x['name']
    for c, v in json.load(open(os.path.join(D, 'idb_members.json'))).items():
        for m in v.get('members', []):
            if not m[0].startswith(('_pad', 'field_', 'base')): fn.setdefault((c, m[1]), m[0])
    for oname, o in objs.items():
        if oname.startswith('_'): continue
        rows = {}; spans = []                                      # spans: (start, end, src, name) a member covers
        def add(src, sname, base, prefix, depth=0):
            for off, x in (st[src].get(sname) or {}).items():
                nm = x if isinstance(x, str) else x['name']
                if nm.startswith(('_unknown', '_unkonwn', 'padding', 'unknown')): continue
                at = int(off, 16) + base
                ty = (x.get('type') or '') if isinstance(x, dict) else ''
                sub = re.sub(r'^(?:Ashita::FFXI::)?(\w+)$', r'\1', ty)
                if src == 'ashita' and sub in st['ashita'] and depth < 4:      # a nested struct: its members, flattened
                    add(src, sub, at, prefix + nm + '.', depth + 1)
                rows.setdefault(at, {}).setdefault(src, prefix + nm)
                if isinstance(x, dict) and x.get('size'): spans.append((at, at + x['size'], src, prefix + nm))
        for src, sname, base in o['structs']: add(src, sname, base, '')
        for off, r in sorted(rows.items()):
            ours = fn.get((o['class'], off))
            names = set(r.values())
            e = dict(r, ours=ours)
            jd = judged.get(oname, {})
            if len({norm(n) for n in names}) > 1 and '0x%X' % off in jd.get('same meaning', []):
                e['status'] = 'adopted (the sources word it differently; judged the same, data/community_judged.json)'
            elif len({norm(n) for n in names}) > 1:
                e['status'] = 'sources differ: measure'
                if '0x%X' % off in jd.get('different: measure', {}): e['why'] = jd['different: measure']['0x%X' % off]
            elif ours and norm(ours) != norm(next(iter(names))):
                e['status'] = 'conflicts with ours: measure'
            else: e['status'] = 'adopted' if not ours else 'the same'
            if e['status'].endswith('measure'):
                res['measure'].append({'kind': 'field', 'object': oname, 'class': o['class'], 'offset': '0x%X' % off, **r, 'ours': ours})
            res['fields'].setdefault(oname, {})['0x%X' % off] = e
        res.setdefault('spans', {})[oname] = sorted(spans)
    json.dump(res, open(out, 'w'), indent=1, sort_keys=True)
    import collections
    print('functions', dict(collections.Counter(v['status'] for v in res['functions'].values())))
    print('fields', {o: dict(collections.Counter(e['status'] for e in v.values())) for o, v in res['fields'].items()})
    print('to measure:', len(res['measure']))


if __name__ == '__main__':
    main(*sys.argv[1:4])
