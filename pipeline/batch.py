"""batch.py -- the tree's unnamed functions as batches of dossiers, for naming sessions (batches/README.md).

A function is in a batch when it is in the tree (TREE/src), its name is still a placeholder (FUN_, sub_, fn_, a vtable
slot vfNNN), and neither DancingMad (data/dm_names.json) nor this repository (data/names_pending_idb.json,
data/names/*.json) names it. Batches follow the tree's grouping, a class or a source unit (TREE/placement.json),
split to at most SIZE functions, so each batch is one area of code. They are ordered by the frame set: the batch
with the most functions sampled in a running frame comes first (data/frame_set.json evidence "self" or "stack").

Each function's dossier holds only what a committed file or the DLL says, and names the source:
  * its verified C++ (TREE/src) and gate line;
  * convention facts: ret N, ret width, ECX/EDX use (purges, ret_width, ecx_use, edx_use);
  * the class vtables that hold it, and at which slot (data/class_vtables.json);
  * whom it calls directly and who calls it directly (the DLL's code, data/func_extents.json), each by its current
    name: DancingMad's, this repository's reviewed one, the tree's, else the placeholder;
  * the globals it reads and writes (absolute operands into .data), Direct3D 8 methods (com_calls.json), imports;
  * DancingMad's comment and the PS2 build's match (data/ps2_matches.json), where there are any;
  * the strings it and its callees refer to (NUL-terminated ASCII at an operand's address, read from the DLL);
  * globals by the tree's name for them (TREE/include/gfx_globals.hpp) where it has one;
  * how its callers call it: the lines of their verified code that name it;
  * named functions two calls away (callers' callers, callees' callees), and its shortest chain of direct calls from
    the frame root (data/frame_set.json _roots);
  * for each neighbour a naming session named, that session's confidence and summary, marked as a reading, not a fact;
  * the whole body (up to 400 lines), and the bodies of short callees (up to 12 lines), with each call to a function
    that has a name now written with that name.

    FFXI_DLL=... python3 batch.py TREE OUT_DIR [SIZE] [SCOPE] [ID_PREFIX]
SCOPE is `frame` (the default: only functions in data/frame_set.json) or `all`; `frame+low` also takes again the
frame-set functions a naming session named only with `low` confidence or whose prefix reprefix_names.py replaced.
`list:FILE` takes exactly the functions FILE lists (a JSON list of addresses; pipeline/name_bench.py).
ID_PREFIX starts every batch id (a second round, `r2_`, in its own OUT_DIR, so its names files never collide).
Writes OUT_DIR/NNN_<group>.md (one per batch) and OUT_DIR/index.json."""
import collections, glob, json, os, re, sys
from capstone.x86 import X86_OP_IMM, X86_OP_MEM
P = os.path.dirname(os.path.abspath(__file__)); D = os.path.join(P, '..', 'data'); sys.path.insert(0, P)
import sp_solve as S
import hx2clean as H

ld = lambda f: json.load(open(os.path.join(D, f)))
GENERIC = re.compile(r'^(?:\w+::)?(?:fn_[0-9A-F]{8}|vf\d+|Slot\d+\w*)$|_VT_[0-9A-F]{8}::vf\d+$')
BODY_LINES = 400
SHORT_CALLEE = 12


def tree_functions(tree):
    """{addr: (file, name, definition text, gate line)}"""
    out = {}
    for p in sorted(glob.glob(os.path.join(tree, 'src', '*.cpp'))):
        for s in open(p).read().split('// FUNCTION: FFXIMAIN ')[1:]:
            a = int(s[:10], 16); lines = s.split('\n'); k = 1
            while k < len(lines) and lines[k].startswith('//'): k += 1
            gate = next((l[3:] for l in lines[1:k] if l.startswith(('// GATE', '// NOT VERIFIED'))), '')
            text = '\n'.join(lines[k:]); end = text.find('\n}\n'); text = text[:end + 2] if end >= 0 else text
            sig = re.sub(r'__attribute__\s*\(\(.*?\)\)', '', lines[k] if k < len(lines) else '')   # GCC attributes
            m = re.search(r'\b((?:\w+::)?~?\w+)\s*\(', sig)
            out[a] = (os.path.basename(p), m.group(1) if m else '?', text, gate)
    return out


def placeholder(name):
    return bool(H.PLACEHOLDER.match(name.split('::')[-1]) or GENERIC.search(name))


def main(tree, outdir, size='40', scope='frame', idprefix=''):
    size = int(size); os.makedirs(outdir, exist_ok=True)
    im = S.Image(os.environ['FFXI_DLL']); ext = ld('func_extents.json')
    tf = tree_functions(tree); pl = json.load(open(os.path.join(tree, 'placement.json')))
    dm = {int(k, 16): v for k, v in ld('dm_names.json').items() if not k.startswith('_')}
    rv = {int(k, 16): v for k, v in H.reviewed_names().items()}
    fsj = ld('frame_set.json'); fs = fsj['functions']
    roots = [int(r, 16) for r in re.findall(r'0x[0-9A-Fa-f]+', str(fsj.get('_roots', '')))]
    gnames = {}
    for l in open(os.path.join(tree, 'include', 'gfx_globals.hpp')):
        m = re.match(r'#define (\w+) XI_EA\(\w+, (0x[0-9A-F]+)u\)', l)
        g_ = re.sub(r'_as_[a-z]?\d+$', '', m.group(1)) if m else ''
        if m and not re.match(r'_*(?:DAT|LAB|PTR|FUN|s|u|ADDR|BYTE|WORD|DWORD|QWORD|FLOAT|DOUBLE)_', g_): gnames[int(m.group(2), 16)] = g_
    def cstr(x):
        o = x - im.base
        if not 0 <= o < len(im.img): return None
        e = im.img.find(b'\0', o, o + 160)
        t = im.img[o:e] if e > o else b''
        return t.decode() if len(t) >= 4 and all(32 <= c < 127 for c in t) else None
    name = lambda a: (dm[a][0] if a in dm else rv[a]['name'] if a in rv else tf[a][1] if a in tf else 'sub_%X' % a)
    purges, rw, ecx, edx = ld('purges.json'), ld('ret_width.json'), ld('ecx_use.json'), ld('edx_use.json')
    ps2 = {int(k, 16): v for k, v in ld('ps2_matches.json').items() if not k.startswith('_') and isinstance(v, dict)}
    com = collections.defaultdict(collections.Counter)
    for k, v in ld('com_calls.json').items():
        if not k.startswith('_') and isinstance(v, dict) and v.get('func'):
            com[int(v['func'], 16)]['%s::%s' % (v.get('iface', '?'), v.get('method', '?'))] += 1
    iat = S.imports(im, os.environ['FFXI_DLL'])
    slots = collections.defaultdict(list)          # the class vtables that hold each function, and at which slot
    for cn, v in ld('class_vtables.json').items():
        for k_, s in enumerate(v['slots']): slots[int(s, 16)].append((cn, k_))
    # every direct call in the DLL's functions: callers of each target
    callers, calls, globs, imps, strs = collections.defaultdict(set), {}, {}, {}, collections.defaultdict(set)
    for k, v in ext.items():
        if k.startswith('_'): continue
        f = int(k, 16); cs, gr, gw, ip = collections.Counter(), set(), set(), collections.Counter()
        for x, i in im.code(v).items():
            if i.mnemonic in ('call', 'jmp') and i.operands and i.operands[0].type == X86_OP_IMM:
                t = i.operands[0].imm
                if i.mnemonic == 'call' or str(t) in ext or ('0x%08x' % t) in ext: callers[t].add(f)
                if i.mnemonic == 'call': cs[t] += 1
            for n_, o in enumerate(i.operands):
                sv = o.imm if o.type == X86_OP_IMM else o.mem.disp & 0xFFFFFFFF if o.type == X86_OP_MEM and not o.mem.base and not o.mem.index else None
                if sv is not None and i.mnemonic not in ('call', 'jmp') and cstr(sv): strs[f].add(cstr(sv))
                if o.type == X86_OP_MEM and not o.mem.base and not o.mem.index:
                    g = o.mem.disp & 0xFFFFFFFF
                    if g in iat:
                        if i.mnemonic in ('call', 'jmp'): ip[str(iat[g]).split('!')[-1]] += 1
                    elif 0x1034F000 <= g < 0x109FA000:
                        (gw if n_ == 0 and i.mnemonic.startswith('mov') else gr).add(g)
        calls[f], globs[f], imps[f] = cs, (gr, gw), ip
    redo = {a for a, v in rv.items() if scope == 'frame+low' and (v.get('confidence') == 'low' or 'invented_prefix_name' in v)}
    if scope.startswith('list:'):                      # exactly these functions (name_bench.py)
        todo = [int(x, 16) for x in json.load(open(scope[5:])) if int(x, 16) in tf]
    else:
        todo = [a for a in tf if (placeholder(tf[a][1]) and a not in dm and a not in rv or a in redo)
                and (scope == 'all' or '0x%08X' % a in fs)]
    # shortest chain of direct calls from the frame root
    parent, q = {r: None for r in roots}, list(roots)
    for x in q:
        for c in sorted(calls.get(x, ())):
            if c not in parent: parent[c] = x; q.append(c)
    def chain(a):
        out = []
        while a is not None and len(out) < 40: out.append(a); a = parent.get(a)
        return out[::-1]
    known = lambda a: a in dm or a in rv or (a in tf and not placeholder(tf[a][1]))
    def tag(a):
        v = rv.get(a)
        return '`%s`' % name(a) + (' (%s)' % v['confidence'] if v and a not in dm else '')
    def named_text(t):
        """the tree's code with each placeholder call (orig::FUN_x, fn_X, sub_X) written with the callee's current name"""
        def sub(m):
            x = int(m.group(2), 16)
            return name(x) if known(x) else m.group(0)
        return re.sub(r'\b(orig::)?(?:FUN|fn|sub)_([0-9A-Fa-f]{8})\b', sub, t)
    def call_lines(c, a):
        if c not in tf: return []
        keys = {tf[a][1].split('::')[-1], 'FUN_%08x' % a, 'sub_%X' % a, 'fn_%08X' % a} if a in tf else {'FUN_%08x' % a, 'sub_%X' % a}
        return [named_text(l.strip()) for l in tf[c][2].split('\n') if any(re.search(r'\b%s\b' % re.escape(k), l) for k in keys)][:2]
    groups = collections.defaultdict(list)
    for a in sorted(todo):
        p = pl.get(hex(a)) or pl.get('0x%08x' % a) or {}
        groups[p.get('class') or p.get('unit') or 'free'].append(a)
    batches = []
    for g, fns in groups.items():
        for i in range(0, len(fns), size):
            part = fns[i:i + size]
            hot = sum(1 for a in part if {'self', 'stack'} & set((fs.get('0x%08X' % a) or {}).get('evidence', [])))
            batches.append((g, part, hot))
    batches.sort(key=lambda b: (-b[2], b[0], b[1][0]))
    index = []
    for n, (g, part, hot) in enumerate(batches, 1):
        bid = idprefix + '%03d_%s' % (n, re.sub(r'\W', '_', g)[:40])
        L = ['# Batch %s: %d functions of `%s`' % (bid, len(part), g), '',
             'Name these functions: see `batches/README.md`. Write `data/names/%s.json`.' % bid,
             '%d of them were sampled in a running frame (data/frame_set.json).' % hot, '']
        for a in part:
            key = '0x%08X' % a; fn, nm, text, gate = tf[a]
            L += ['## `%s` %s' % (key, nm), '']
            facts = []
            if key in purges: facts.append('`ret %d`' % purges[key])
            if key in rw: facts.append('sets only %s of EAX' % ('AL' if rw[key] == 1 else 'AX'))
            if key in ecx: facts.append('entry ECX %s' % ('read' if isinstance(ecx[key], str) else 'never read' if ecx[key] is False else 'not established'))
            if key in edx: facts.append('entry EDX %s' % ('read' if edx[key] else 'never read'))
            ev = (fs.get(key) or {}).get('evidence')
            L.append('- **Gate:** %s' % (gate or 'none') + ('; **frame set:** %s' % ', '.join(ev) if ev else ''))
            if facts: L.append('- **Facts:** ' + '; '.join(facts) + ' (purges, ret_width, ecx_use, edx_use)')
            cs = calls.get(a, {})
            if cs: L.append('- **Calls:** ' + ', '.join('%s%s' % (tag(c), ' x%d' % k if k > 1 else '') for c, k in sorted(cs.items())))
            if slots.get(a): L.append('- **In vtables** (class_vtables.json): ' + ', '.join('`%s` slot %d (+0x%X)' % (c_, k_, 4 * k_) for c_, k_ in slots[a][:8])
                                      + (' and %d more' % (len(slots[a]) - 8) if len(slots[a]) > 8 else ''))
            cl = sorted(callers.get(a, ()))
            if cl: L.append('- **Called by:** ' + ', '.join(tag(c) for c in cl[:20]) + (' and %d more' % (len(cl) - 20) if len(cl) > 20 else ''))
            if com.get(a): L.append('- **Direct3D 8:** ' + ', '.join('`%s`%s' % (m, ' x%d' % k if k > 1 else '') for m, k in sorted(com[a].items())))
            if imps.get(a): L.append('- **Imports:** ' + ', '.join('`%s`' % m for m in sorted(imps[a])))
            gr, gw = globs.get(a, (set(), set()))
            if gw: L.append('- **Writes globals:** ' + ', '.join('`0x%08X`' % x for x in sorted(gw)[:16]))
            if gr: L.append('- **Reads globals:** ' + ', '.join('`0x%08X`' % x for x in sorted(gr)[:16]))
            if a in ps2: L.append('- **PS2 match** (%s): `%s`' % (ps2[a].get('confidence'), ps2[a].get('ps2')))
            if a in redo: L.append("- **Named before** (a naming session's reading, %s): `%s`; name it again from this dossier, or leave it out to keep that name"
                                   % (rv[a].get('confidence'), rv[a]['name']))
            gn = [gnames[x] for x in sorted(gr | gw) if x in gnames]
            if gn: L.append('- **Named globals** (TREE/include/gfx_globals.hpp): ' + ', '.join('`%s`' % x for x in gn[:16]))
            ss = sorted(strs.get(a, ())); cs_ss = sorted(set().union(*[strs.get(c, set()) for c in cs]) - set(ss)) if cs else []
            if ss: L.append('- **Strings it refers to** (DLL bytes): ' + ', '.join('`%s`' % x.replace('`', "'") for x in ss[:10]))
            if cs_ss: L.append('- **Strings its callees refer to:** ' + ', '.join('`%s`' % x.replace('`', "'") for x in cs_ss[:10]))
            ch = chain(a)
            if len(ch) > 1: L.append('- **From the frame root** (shortest chain of direct calls): ' + ' → '.join('`%s`' % name(x) for x in ch[-8:]))
            up2 = sorted({x for c in cl for x in callers.get(c, ()) if known(x)} - set(cl))
            dn2 = sorted({x for c in cs for x in calls.get(c, {}) if known(x)} - set(cs))
            if up2: L.append("- **Named callers' callers:** " + ', '.join('`%s`' % name(x) for x in up2[:12]) + (' and %d more' % (len(up2) - 12) if len(up2) > 12 else ''))
            if dn2: L.append("- **Named callees' callees:** " + ', '.join('`%s`' % name(x) for x in dn2[:12]) + (' and %d more' % (len(dn2) - 12) if len(dn2) > 12 else ''))
            uses = [(c, l) for c in cl[:6] for l in call_lines(c, a)]
            if uses:
                L.append('- **How its callers call it** (their verified code):')
                L += ['  - in %s: `%s`' % (tag(c), l.replace('`', "'")[:200]) for c, l in uses[:6]]
            rd = [x for x in sorted(set(cl) | set(cs)) if x in rv and x not in dm and x != a]
            if rd:
                L.append("- **Neighbours' readings** (naming sessions' summaries, not facts):")
                L += ['  - %s: %s' % (tag(x), rv[x].get('summary', '')[:200]) for x in rd[:10]]
            short = [c for c in sorted(cs) if c in tf and c != a and len(tf[c][2].split('\n')) <= SHORT_CALLEE][:4]
            for c in short:
                L += ['', 'Short callee %s:' % tag(c), '```cpp'] + named_text(tf[c][2]).split('\n') + ['```']
            body = named_text(text).split('\n')
            L += ['', '```cpp'] + body[:BODY_LINES] + (['// ... %d more lines (tree/src/%s)' % (len(body) - BODY_LINES, fn)] if len(body) > BODY_LINES else []) + ['```', '']
        open(os.path.join(outdir, bid + '.md'), 'w').write('\n'.join(L))
        index.append({'batch': bid, 'group': g, 'functions': ['0x%08X' % a for a in part], 'sampled': hot})
    json.dump({'_doc': {'tree': tree, 'batches': len(index), 'functions': len(todo), 'size': size, 'scope': scope}, 'batches': index},
              open(os.path.join(outdir, 'index.json'), 'w'), indent=1)
    print('%d functions in %d batches' % (len(todo), len(index)))


if __name__ == '__main__':
    main(*sys.argv[1:6])
