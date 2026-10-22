"""dm_crosscheck.py -- DancingMad's named addresses checked against this repository's binary, names and tree.

DancingMad (the owner's research repository) names FFXiMain.dll addresses in tools/ffxi_symbols_ida.py and
tools/ffxi_symbols_ghidra.py (`apply_name(addr, name, comment, is_func)`), and cites addresses throughout
docs/FINDINGS.md and docs/GRAPHICS_PIPELINE.md. Its scripts are read with `ast`, never run. Each named address
is checked against what this repository holds, every check from a committed source:

  * the binary: a function name is at a function's first instruction (data/func_extents.json), or else the
    function holding it is named; a data name is outside every function;
  * the name this repository uses: data/names_pending_idb.json (named since the export), tree/placement.json
    (the verified tree), else the IDB name in the committed export (data/exports/gfx_export.jsonl);
  * the tree: whether the function is in it, and its gate verdict (verification/gate_clean_final.jsonl);
  * DancingMad's own confidence, as its comment states it (PS2-confirmed, AUTHOR-INFERRED, Candidate, ...).

A different name is not by itself an error on either side; the row says which name each side uses and on what.
The docs' addresses are listed with the line that cites them, and each is checked against function starts and
the names above.

    python3 dm_crosscheck.py DANCINGMAD_DIR OUT.json"""
import ast, bisect, json, os, re, sys
P = os.path.dirname(os.path.abspath(__file__)); D = os.path.join(P, '..', 'data'); ROOT = os.path.join(P, '..')


def calls(path):
    out = []
    for node in ast.walk(ast.parse(open(path).read())):
        if isinstance(node, ast.Call) and getattr(node.func, 'id', None) == 'apply_name' and len(node.args) >= 4:
            try: out.append(tuple(ast.literal_eval(a) for a in node.args[:4]) + (node.lineno,))
            except ValueError: pass
    return out


def confidence(comment):
    c = comment.upper()
    for k, v in (('AUTHOR-INFERRED', 'author-inferred'), ('_CANDIDATE', 'candidate'), ('CANDIDATE', 'candidate'),
                 ('PS2-CONFIRMED', 'PS2-confirmed'), ('PS2 CONFIRMED', 'PS2-confirmed'), ('CONFIRMED', 'confirmed'),
                 ('MEDIUM CONFIDENCE', 'medium'), ('LOW CONFIDENCE', 'low'), ('HIGH CONFIDENCE', 'high')):
        if k in c: return v
    return 'not stated'


def main(dm, out):
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    starts = {int(k, 16) for k in ext}
    pieces = sorted((a, b, int(k, 16)) for k, rs in ext.items() for a, b in rs); ps = [p[0] for p in pieces]
    def holder(a):
        j = bisect.bisect_right(ps, a) - 1
        return pieces[j][2] if j >= 0 and pieces[j][0] <= a < pieces[j][1] else None
    ida = {}
    for l in open(os.path.join(D, 'exports', 'gfx_export.jsonl')):
        r = json.loads(l); ida.setdefault(r['ea'], r['name'])
        for n, o in (r.get('objs') or {}).items(): ida.setdefault(o['ea'], n)
    pending = {int(k, 16): v['name'] for k, v in json.load(open(os.path.join(D, 'names_pending_idb.json'))).items() if not k.startswith('_')}
    tree = {int(k, 16): v for k, v in json.load(open(os.path.join(ROOT, 'tree', 'placement.json'))).items()}
    verdict = {int(json.loads(l)['original'], 16): json.loads(l)['verdict'] for l in open(os.path.join(ROOT, 'verification', 'gate_clean_final.jsonl'))}
    ours = lambda a: (pending[a], 'names_pending_idb.json') if a in pending else (tree[a]['symbol'].replace('::', '_'), 'tree') \
        if a in tree else (ida[a], 'IDB (export)') if a in ida else (None, None)
    norm = lambda n: re.sub(r'[^0-9a-z]', '', (n or '').lower())

    rows, seen = [], {}
    for script in ('ffxi_symbols_ida.py', 'ffxi_symbols_ghidra.py'):
        for addr, name, comment, is_func, line in calls(os.path.join(dm, 'tools', script)):
            key = (addr, name)
            if key in seen: seen[key]['scripts'].append(script); continue
            on, src = ours(addr)
            h = holder(addr)
            r = {'addr': '0x%08X' % addr, 'dm_name': name, 'dm_kind': 'function' if is_func else 'data',
                 'dm_confidence': confidence(comment), 'dm_comment': comment, 'scripts': [script], 'line': line,
                 'our_name': on, 'our_source': src,
                 'in_tree': addr in tree, 'verdict': verdict.get(addr)}
            if is_func:
                r['binary'] = 'function start' if addr in starts else ('inside 0x%08X' % h if h else 'not in any function')
            else:
                r['binary'] = 'data (outside every function)' if h is None else 'inside function 0x%08X' % h
            if on is None: r['names'] = 'only DancingMad names it'
            elif norm(on) == norm(name) or norm(on).endswith(norm(name)) or norm(name).endswith(norm(on)): r['names'] = 'same'
            elif re.match(r'(sub|fn|unk|dword|byte|word|off|flt|dbl|stru|asc|loc)_[0-9A-Fa-f]+$', on): r['names'] = 'unnamed here'
            else: r['names'] = 'differ'
            rows.append(r); seen[key] = r

    cites = []
    for doc in ('FINDINGS.md', 'GRAPHICS_PIPELINE.md', 'ROADMAP.md'):
        for n, line in enumerate(open(os.path.join(dm, 'docs', doc)), 1):
            for m in re.finditer(r'\b0x(10[0-9A-Fa-f]{6})\b', line):
                a = int(m.group(1), 16); h = holder(a); on, src = ours(a)
                cites.append({'doc': doc, 'line': n, 'addr': '0x%08X' % a,
                              'binary': 'function start' if a in starts else ('inside 0x%08X' % h if h else 'not in any function'),
                              'our_name': on, 'in_tree': a in tree, 'verdict': verdict.get(a), 'text': line.strip()[:240]})
    json.dump({'names': rows, 'doc_addresses': cites}, open(out, 'w'), indent=1)
    import collections
    print('%d names (%s)' % (len(rows), dict(collections.Counter(r['names'] for r in rows))))
    print('  binary: %s' % dict(collections.Counter(r['binary'].split(' 0x')[0] for r in rows)))
    print('  in the tree: %d (%s)' % (sum(r['in_tree'] for r in rows), dict(collections.Counter(r['verdict'] for r in rows if r['in_tree']))))
    print('  DancingMad confidence: %s' % dict(collections.Counter(r['dm_confidence'] for r in rows)))
    print('%d addresses cited in the docs (%s)' % (len(cites), dict(collections.Counter(c['binary'].split(' 0x')[0] for c in cites))))


if __name__ == '__main__':
    main(*sys.argv[1:3])
