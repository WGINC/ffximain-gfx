"""check_names.py -- a naming session's output, checked before it is merged (batches/README.md).

    python3 check_names.py batches/index.json data/names/NNN_group.json
Checks, each a hard error:
  * every key is a function of that batch (index.json), written 0xXXXXXXXX;
  * each entry has "name", "confidence" (high | medium | low), "evidence" (non-empty: what in the dossier shows it)
    and "summary" (one sentence);
  * the name is a C identifier of 4 to 64 characters, in the form Prefix_Verb... (at least one underscore, each part
    starting with a capital letter or a digit), and not a placeholder (FUN_, sub_, fn_, vf, Slot);
  * the prefix (the part before the first underscore) is the batch's class, Unit<address> for a unit batch, or
    is shown by the batch's dossier (batches/NNN_group.md) as whole CamelCase parts of a name there (StAvatar in
    StAvatarInit, Time in Time_Normalize), not as letters inside another word: an area word the dossier does not
    show is invented. A unit batch's label (ZoneMap in unit_10178790_ZoneMap) counts as its own;
  * no name is used twice in the file, by DancingMad (data/dm_names.json), by the IDB or the Ghidra project
    (data/exports/gfx_export.jsonl, runs/ghidra/names.json), or by another reviewed name
    (data/names_pending_idb.json, the other files in data/names/) for a different address.
Prints OK, or each problem and exits 1."""
import glob, json, os, re, sys

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
NAME = re.compile(r'^[A-Z][A-Za-z0-9]*(?:_[A-Z0-9][A-Za-z0-9]*)+$')
BAD = re.compile(r'^(?:FUN|sub|fn|vf|Slot|nullsub|thunk)_?', re.I)


def index_for(path):
    """The batch index a names file belongs to: batches/index.json, or batches/round<N>/index.json for rN_ files."""
    m = re.match(r'r(\d+)_', os.path.basename(path))
    return os.path.join(D, '..', 'batches', *(['round' + m.group(1)] if m else []), 'index.json')


def main(index, path):
    idx = json.load(open(index))
    bid = os.path.basename(path)[:-5]
    batch = next((b for b in idx['batches'] if b['batch'] == bid), None)
    errs = []
    if batch is None: errs.append('%s is not a batch in %s' % (bid, index)); batch = {'functions': []}
    want = set(batch['functions'])
    got = json.load(open(path))
    taken = {}
    md = os.path.join(os.path.dirname(os.path.abspath(index)), bid + '.md')
    dossier = open(md).read() if os.path.exists(md) else ''
    group = re.sub(r'^r\d+_', '', bid); group = group.split('_', 1)[1] if '_' in group else group
    own = {re.sub(r'_VT_[0-9A-F]{8}$', '', group)} if not group.startswith('unit_') else set()
    if group.startswith('unit_'): own.add('Unit' + group[5:13]); own.update(group[14:].split('_')[:1] if group[14:] else [])
    own_lc = {o.lower() for o in own}
    for k, v in json.load(open(os.path.join(D, 'dm_names.json'))).items():
        if not k.startswith('_'): taken.setdefault(v[0], '0x%08X' % int(k, 16))
    # the project's own names: the IDB's (the committed export's function objects) and the Ghidra project's
    for l in open(os.path.join(D, 'exports', 'gfx_export.jsonl')):
        for nm_, o in (json.loads(l).get('objs') or {}).items():
            if o.get('func') and not o.get('import'): taken.setdefault(nm_, '0x%08X' % o['ea'])
    _g = os.path.join(D, '..', 'runs', 'ghidra', 'names.json')
    for k, v in (json.load(open(_g)).items() if os.path.exists(_g) else ()):
        if not BAD.match(v) and not v.startswith('j_'): taken.setdefault(v, '0x%08X' % int(k, 16))
    for p in [os.path.join(D, 'names_pending_idb.json')] + sorted(glob.glob(os.path.join(D, 'names', '*.json'))):
        if os.path.abspath(p) == os.path.abspath(path): continue
        for k, v in json.load(open(p)).items():
            if not k.startswith('_'): taken.setdefault(v['name'], '0x%08X' % int(k, 16))
    seen = {}
    for k, v in got.items():
        if k.startswith('_'): continue
        if not re.fullmatch(r'0x[0-9A-F]{8}', k): errs.append('%s: key not written 0xXXXXXXXX' % k); continue
        if k not in want: errs.append('%s: not a function of batch %s' % (k, bid))
        if not isinstance(v, dict): errs.append('%s: entry is not an object' % k); continue
        n = v.get('name', '')
        if not NAME.match(n) or not 4 <= len(n) <= 64: errs.append('%s: name %r is not Prefix_Verb... form' % (k, n))
        if BAD.match(n): errs.append('%s: name %r is a placeholder' % (k, n))
        pre = n.split('_', 1)[0]
        shown = (re.search(r'(?:(?<![A-Za-z0-9])|(?<=[a-z0-9_]))%s(?![a-z])' % re.escape(pre), dossier)
                 or re.search(r'(?<![A-Za-z0-9_])%s(?![a-z])' % re.escape(pre[0].lower() + pre[1:]), dossier))
        if pre not in own and pre.lower() not in own_lc and not shown:
            errs.append('%s: prefix %r is neither the batch\'s class, %s, nor a word the dossier shows' % (k, pre, ' / '.join(sorted(own))))
        if v.get('confidence') not in ('high', 'medium', 'low'): errs.append('%s: confidence must be high, medium or low' % k)
        if not str(v.get('evidence', '')).strip(): errs.append('%s: no evidence' % k)
        if not str(v.get('summary', '')).strip(): errs.append('%s: no summary' % k)
        if n in seen: errs.append('%s: name %r also used for %s in this file' % (k, n, seen[n]))
        if n in taken and taken[n] != k: errs.append('%s: name %r already names %s' % (k, n, taken[n]))
        seen[n] = k
    print('OK: %d names' % len(seen) if not errs else '\n'.join(errs))
    sys.exit(1 if errs else 0)


if __name__ == '__main__':
    main(*sys.argv[1:3])
