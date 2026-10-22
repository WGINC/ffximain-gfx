# ida_user_types.py -- every function whose type the IDB flags as user-set, for the owner to mark which they typed
# (run inside IDA; read-only: the IDB is not changed). docs/PROBLEMS.md P13, requests/02_user_set_types.md.
#
# IDA flags a type user-set (ida_nalt.is_userti) whoever set it: the owner, or an earlier session's script. It does not
# record which. This script lists them with what the repo knows about each, so the owner can mark the origin.
#
# Input: ida/ffxi_ida_fix.py (its DATA["types"], parsed from the file, never imported: importing runs its IDB
#        changes), data/ida_proto_origin.json, tree/src/*.cpp.
# Output: data/ida_user_types.json, one entry per function start ea with is_userti:
#   {"name": IDA's name, "type": the prototype as IDA prints it (as in data/ida_proto_origin.json),
#    "comment": the function comment, regular then repeatable (joined by a newline when both exist), or null,
#    "session_note": a comment containing "[type " or "[name " (written by session scripts),
#    "fix_script": the address is a key of DATA["types"] in ida/ffxi_ida_fix.py,
#    "gate_sites": entries of data/ida_proto_origin.json whose "callee" is this function,
#    "in_tree": a "// FUNCTION: FFXIMAIN 0x..." line for it in tree/src/*.cpp}
#   plus "_doc": IDA version, IDB file name, date, counts.
# And requests/review/02_user_types.md: the entries with gate_sites > 0 or in_tree, and neither session_note nor
# fix_script, sorted by gate_sites (highest first, then address), with an empty Origin column for the owner.
#
# Run: File > Script file... > ida_user_types.py  (IDA 9.x)
import datetime, glob, json, os, re
import idaapi, idautils, idc, ida_funcs, ida_kernwin, ida_nalt, ida_name, ida_typeinf

try:
    REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
except NameError:
    REPO = globals().get('REPO') or os.getcwd()
FIX = os.path.join(REPO, 'ida', 'ffxi_ida_fix.py')
ORIGIN = os.path.join(REPO, 'data', 'ida_proto_origin.json')
TREE = os.path.join(REPO, 'tree', 'src', '*.cpp')
OUT = os.path.join(REPO, 'data', 'ida_user_types.json')
REVIEW = os.path.join(REPO, 'requests', 'review', '02_user_types.md')


def hx(ea):
    return None if ea is None or ea == idaapi.BADADDR else '0x%08X' % ea


def pr(tif, name=''):
    s = ida_typeinf.print_tinfo('', 0, 0, ida_typeinf.PRTYPE_1LINE, tif, name, '')
    return s if s is not None else str(tif)


def fix_types():
    # the keys of DATA["types"], from the json literal the script embeds (DATA = json.loads(r'''...'''))
    s = open(FIX, encoding='utf-8').read()
    m = re.search(r"^DATA = json\.loads\(r'''(.*?)'''\)", s, re.S | re.M)
    return {int(k, 16) for k in json.loads(m.group(1))['types']}


def gate_counts():
    n = {}
    for k, r in json.load(open(ORIGIN)).items():
        if k != '_doc' and r.get('callee'):
            ea = int(r['callee'], 16); n[ea] = n.get(ea, 0) + 1
    return n


def tree_funcs():
    got = set()
    for p in glob.glob(TREE):
        for m in re.finditer(r'// FUNCTION: FFXIMAIN (0x[0-9A-Fa-f]+)', open(p, encoding='utf-8', errors='replace').read()):
            got.add(int(m.group(1), 16))
    return got


def comment(f):
    c = [x for x in (ida_funcs.get_func_cmt(f, 0), ida_funcs.get_func_cmt(f, 1)) if x]
    return '\n'.join(c) if c else None


def cell(s):
    return (s or '').replace('|', '\\|').replace('\n', ' ')


def main():
    fix = fix_types(); gate = gate_counts(); tree = tree_funcs()
    out = {}
    for ea in idautils.Functions():
        if not ida_nalt.is_userti(ea): continue
        f = ida_funcs.get_func(ea)
        t = ida_typeinf.tinfo_t()
        c = comment(f)
        out[hx(ea)] = {'name': ida_name.get_name(ea), 'type': pr(t) if ida_nalt.get_tinfo(t, ea) else None,
                       'comment': c, 'session_note': bool(c and ('[type ' in c or '[name ' in c)),
                       'fix_script': ea in fix, 'gate_sites': gate.get(ea, 0), 'in_tree': ea in tree}
    review = sorted((k for k, r in out.items() if (r['gate_sites'] > 0 or r['in_tree'])
                     and not r['session_note'] and not r['fix_script']),
                    key=lambda k: (-out[k]['gate_sites'], k))
    cnt = lambda key: sum(1 for r in out.values() if r[key])
    doc = {'ida_version': ida_kernwin.get_kernel_version(), 'idb': os.path.basename(idc.get_idb_path()),
           'date': datetime.date.today().isoformat(), 'functions': ida_funcs.get_func_qty(), 'user_set': len(out),
           'session_note': cnt('session_note'), 'fix_script': cnt('fix_script'), 'with_gate_sites': cnt('gate_sites'),
           'in_tree': cnt('in_tree'), 'review': len(review), 'fix_script_types': len(fix),
           'fix_script_types_not_user_set': sorted(hx(e) for e in fix if hx(e) not in out),
           'comment': 'regular, then repeatable, joined by a newline when both exist',
           'source': 'ida/ida_user_types.py'}
    res = {'_doc': doc}
    res.update(sorted(out.items()))
    with open(OUT, 'w') as fh:
        json.dump(res, fh, indent=0, sort_keys=False)
    os.makedirs(os.path.dirname(REVIEW), exist_ok=True)
    with open(REVIEW, 'w', encoding='utf-8', newline='\n') as fh:
        fh.write('# User-set function types to mark (request 02)\n\n')
        fh.write('%d functions; from `data/ida_user_types.json` (%s, %s). ' % (len(review), doc['idb'], doc['date']))
        fh.write('Fill **Origin** with `mine` (you set this type), `session` (an earlier session set it) or `unsure`.\n\n')
        fh.write('| address | name | prototype | gate sites | in tree | Origin |\n|---|---|---|---|---|---|\n')
        for k in review:
            r = out[k]
            fh.write('| %s | %s | `%s` | %d | %s | |\n' % (k, cell(r['name']), cell(r['type']), r['gate_sites'],
                                                         'yes' if r['in_tree'] else 'no'))
    print('%d user-set of %d functions -> %s; %d to review -> %s' % (len(out), doc['functions'], OUT, len(review), REVIEW))
    return doc


if __name__ == '__main__':
    main()
