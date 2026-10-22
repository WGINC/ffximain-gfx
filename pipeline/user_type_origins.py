"""user_type_origins.py -- what the repos record about who set each user-set function type (P13, request 02).

IDA flags a function type user-set (ida_nalt.is_userti) whoever set it and does not record who. This reads the review
table's functions and gives each an Origin only where a source in the repos says so; nothing is inferred from a name.

    python3 user_type_origins.py [DANCINGMAD_DIR]
Reads:  data/ida_user_types.json, data/ida_proto_origin.json, data/ps2_dwarf_funcs.json, requests/review/02_user_types.md
        (its rows), ida/ffxi_ida_fix.py (DATA parsed, not imported), DancingMad/tools/ffxi_symbols_ida.py (default
        ../DancingMad; parsed with ast, not run).
Writes: data/ida_user_type_origins.json and the Origin / Evidence columns of requests/review/02_user_types.md.
Run after ida/ida_user_types.py (which writes the table without these two columns' contents).

Origin, from these rules only:
  session  the type was changed by a session's script, recorded in the script's own data: the fix script's class pass
           (DATA["classes"] methods) typed `this` as the class struct it made, and the prototype shows that struct; or
           the function carries the script's own `[conv ...]` note (made __thiscall); or DATA["types"].
  library  the prototype prints identically to the SDK type library's symbol of that name (ida_proto_origin.json "til").
  mine     never set here: only the owner can say so; a `mine` already in the table is kept.
  (blank)  nothing recorded.
The Evidence column also lists name and comment facts, marked "name:"/"comment:"; they do not decide Origin, because a
function's name or comment says nothing about who typed its prototype.
"""
import ast, json, os, re, sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
REVIEW = os.path.join(REPO, 'requests', 'review', '02_user_types.md')
OUT = os.path.join(REPO, 'data', 'ida_user_type_origins.json')
ROW = re.compile(r'^\| (0x[0-9A-F]{8}) \| (.*?) \| `(.*?)` \| (\d+) \| (yes|no) \|([^|]*)\|(?:(.*)\|)?$')


def load(p):
    return json.load(open(os.path.join(REPO, p), encoding='utf-8'))


def fix_data():
    s = open(os.path.join(REPO, 'ida', 'ffxi_ida_fix.py'), encoding='utf-8').read()
    return json.loads(re.search(r"^DATA = json\.loads\(r'''(.*?)'''\)", s, re.S | re.M).group(1))


def dancingmad_names(d):
    # apply_name(addr, 'name', 'comment', is_func) calls of the PS2 symbol script, by address
    p = os.path.join(d, 'tools', 'ffxi_symbols_ida.py')
    if not os.path.exists(p): return None
    got = {}
    for n in ast.walk(ast.parse(open(p, encoding='utf-8').read())):
        if isinstance(n, ast.Call) and getattr(n.func, 'id', '') == 'apply_name' and len(n.args) >= 3 \
                and all(isinstance(a, ast.Constant) for a in n.args[:3]):
            got['0x%08X' % n.args[0].value] = n.args[2].value
    return got


def main(dm_dir):
    u = load('data/ida_user_types.json'); o = load('data/ida_proto_origin.json')
    ps2 = load('data/ps2_dwarf_funcs.json'); D = fix_data(); dm = dancingmad_names(dm_dir)
    methods = {}
    for cn, c in D['classes'].items():
        for m in c.get('methods', []): methods.setdefault(m, []).append(cn)
    til = {r['callee']: r for k, r in o.items() if k != '_doc' and r.get('callee')}
    old = {}
    lines = open(REVIEW, encoding='utf-8').read().split('\n')
    for l in lines:
        m = ROW.match(l)
        if m: old[m.group(1)] = m.group(6).strip()
    res = {}; new = []
    for l in lines:
        m = ROW.match(l)
        if not m: new.append(l); continue
        k = m.group(1); r = u[k]; t = r['type'] or ''; c = r['comment'] or ''
        ev = []; origin = ''
        if k in D['types']:
            origin = 'session'; ev.append('type: ffxi_ida_fix.py DATA["types"]')
        for cn in methods.get(k, []):
            if re.search(r'\b%s \*' % re.escape(cn), t):
                origin = origin or 'session'; ev.append('type: `this` is %s *, the struct the class pass made' % cn)
            else:
                ev.append('type: listed in class pass (%s) but `this` is not %s *' % (cn, cn))
        if k in D['pre_class_types']: ev.append('type: a pre-class prototype is recorded in the script data (its origin is not)')
        if '[conv ' in c:
            origin = origin or 'session'; ev.append('type: `[conv ...]` note, script made it __thiscall')
        e = til.get(k)
        if e and e.get('til_same'):
            origin = origin or 'library'; ev.append('type: identical to %s symbol' % e['til_name'])
        if k in ps2: ev.append('type: PS2 DWARF params %s do not match' % [p[1] for p in ps2[k]['params']])
        if k in D['names']: ev.append('name: ffxi_ida_fix.py names (%s confidence)' % D['names'][k]['confidence'])
        if dm and k in dm:
            ev.append('name: DancingMad apply_name (%s)' % ('AUTHOR-INFERRED' if 'AUTHOR-INFERRED' in dm[k]
                      else 'PS2' if 'PS2' in dm[k] else 'evidence in its comment'))
        for tag, txt in (('[recon]', 'tree verification'), ('[gfx-recon', 'tree verification'),
                         ('FLIRT false positive', 'FLIRT name removed'), ('d3dx8.1', 'byte-identical to d3dx8.1'),
                         ('PS2 counterpart', 'PS2 counterpart note')):
            if tag in c and 'comment: ' + txt not in ev: ev.append('comment: ' + txt)
        if old.get(k) == 'mine': origin = 'mine'
        res[k] = {'origin': origin or None, 'evidence': ev}
        new.append('| %s | %s | `%s` | %s | %s | %s | %s |' % (k, m.group(2), m.group(3), m.group(4), m.group(5), origin,
                                                              '; '.join(ev).replace('|', '\\|')))
    json.dump({'_doc': {'rules': __doc__.split('Origin, from these rules only:')[1].split('The Evidence')[0].strip(),
                        'DancingMad': dm_dir if dm is not None else None, 'rows': len(res)}, **res},
              open(OUT, 'w'), indent=0)
    text = '\n'.join(new)
    text = re.sub(r'\| address \| name \| prototype \| gate sites \| in tree \| Origin \|( Evidence \|)?\n\|(---\|){6,7}',
                  '| address | name | prototype | gate sites | in tree | Origin | Evidence |\n|---|---|---|---|---|---|---|', text)
    open(REVIEW, 'w', encoding='utf-8', newline='\n').write(text)
    n = {}
    for v in res.values(): n[v['origin']] = n.get(v['origin'], 0) + 1
    print('%d rows %s -> %s' % (len(res), n, OUT))


if __name__ == '__main__':
    main(sys.argv[1] if len(sys.argv) > 1 else os.path.join(os.path.dirname(REPO), 'DancingMad'))
