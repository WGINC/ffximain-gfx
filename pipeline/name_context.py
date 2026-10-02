"""name_context.py -- the evidence for naming what is still a placeholder, gathered in one place.

    python3 name_context.py fields CLASS [TREE]       every field_XXX of CLASS: type, and each use by function
    python3 name_context.py globals [TREE] [EXPORT]   every dword_/sub_/... placeholder in the tree: its users
                                                      across the whole export, with the lines that touch it

The output is meant to be read (by a person or a model) to propose names into data/field_names.json
or data/names_pending_idb.json, each with the evidence it rests on.
"""
import json, os, re, sys, collections

ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')
PLACEHOLDER = re.compile(r'\b(?:sub|dword|word|byte|flt|dbl|qword|unk|off|stru)_[0-9A-F]{8}\b')


def functions(src):
    """(name, body) for each function definition in a generated source file"""
    text = open(src).read()
    for m in re.finditer(r'^(?:XI_API )?[^\n;{}]*?\b([\w:]+)\(([^\n]*)\)\s*\n\{(.*?)\n\}', text, re.S | re.M):
        yield m.group(1), m.group(0)


def fields(cls, tree):
    hdr = open(os.path.join(tree, 'include', cls + '.hpp')).read()
    decl = {m.group(2): (m.group(1), m.group(3)) for m in re.finditer(r'^\s+(.+?) (field_[0-9A-F]+); +// \+(0x[0-9A-F]+)', hdr, re.M)}
    uses = collections.defaultdict(lambda: collections.defaultdict(list))
    for f in sorted(os.listdir(os.path.join(tree, 'src'))):
        for name, body in functions(os.path.join(tree, 'src', f)):
            if not name.startswith(cls + '::'): continue     # another class's field_XXX is a different field
            for line in body.split('\n'):
                for fld in set(re.findall(r'\bthis->(field_[0-9A-F]+)\b', line)):
                    if fld in decl: uses[fld][name].append(line.strip()[:160])
    for fld, (t, off) in sorted(decl.items(), key=lambda x: int(x[1][1], 16)):
        print('=== %s %s  %s  (%d functions)' % (cls, off, t, len(uses[fld])))
        for fn, lines in list(uses[fld].items())[:6]:
            print('  [%s]' % fn)
            for l in lines[:4]: print('      ' + l)


def globals_(tree, export):
    ph = set()
    for d in ('src', 'include'):
        for f in os.listdir(os.path.join(tree, d)):
            ph |= set(PLACEHOLDER.findall(open(os.path.join(tree, d, f)).read()))
    recs = [json.loads(l) for l in open(export)]
    for p in sorted(ph):
        ea = int(p.split('_')[1], 16)
        users = []
        for r in recs:
            hit = [nm for nm, o in r['objs'].items() if o['ea'] == ea]
            if hit:
                lines = [l.strip() for l in r['text'].split('\n') if not l.startswith('//') and
                         any(re.search(r'\b%s\b' % re.escape(h), l) for h in hit)]
                users.append((r['name'], lines))
        print('=== %s  users=%d' % (p, len(users)))
        for n, ls in users[:5]:
            print('  [%s]' % n)
            for l in ls[:4]: print('      ' + l[:160])


if __name__ == '__main__':
    mode = sys.argv[1]
    if mode == 'fields':
        fields(sys.argv[2], sys.argv[3] if len(sys.argv) > 3 else os.path.join(ROOT, 'tree'))
    else:
        globals_(sys.argv[2] if len(sys.argv) > 2 else os.path.join(ROOT, 'tree'),
                 sys.argv[3] if len(sys.argv) > 3 else os.path.join(ROOT, 'data', 'exports', 'gfx_export_current.jsonl'))
