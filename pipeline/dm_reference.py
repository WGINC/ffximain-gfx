"""dm_reference.py -- one reference entry per function of the verified tree, for DancingMad's documentation.

Every line of an entry comes from a committed source here, and says which; nothing is written that a file does
not hold. Where a statement is an interpretation (a summary of what a function is for), it is labelled so, with
the confidence its source gave it, and kept apart from the facts:

  * the tree (tree/src): the function's signature as the verified C++ declares it, its gate line (trials,
    outcomes, Direct3D calls compared, and each live dump it was gated on), its class or source unit;
  * the code (the DLL, data/func_extents.json): the functions it calls directly and the globals it reads and
    writes (absolute memory operands, by operand position);
  * facts files: `ret N` (purges.json, tail_purges.json), ECX/EDX use, argument slots (cdecl_args.json), the
    return's width (ret_width.json, ret_kinds.json), registers left untouched (reg_keeps.json), Direct3D 8
    methods called on a proven device (com_calls.json), whether a frame runs it (frame_set.json);
  * names: this repository's (names_pending_idb.json, tree/placement.json, the IDB in the committed export),
    DancingMad's (tools/ffxi_symbols_ida.py, with the confidence its comment states), and the summary written
    when the function was named (docs/NAMES.md), which is an inference from reading the code.

    FFXI_DLL=... python3 dm_reference.py DANCINGMAD_DIR CROSSCHECK.json OUT_DIR"""
import ast, collections, json, os, re, sys
from capstone.x86 import X86_OP_IMM, X86_OP_MEM
P = os.path.dirname(os.path.abspath(__file__)); D = os.path.join(P, '..', 'data'); ROOT = os.path.join(P, '..')
sys.path.insert(0, P)
import sp_solve as S

ld = lambda f: json.load(open(os.path.join(D, f)))


def tree_functions():
    """{addr: (unit file, signature, gate line, function line)} from tree/src"""
    out = {}
    src = os.path.join(ROOT, 'tree', 'src')
    for fn in sorted(os.listdir(src)):
        lines = open(os.path.join(src, fn)).read().split('\n')
        for k, l in enumerate(lines):
            m = re.match(r'// FUNCTION: FFXIMAIN (0x[0-9A-F]+)\s*(.*)', l)
            if not m: continue
            gate, sig = '', ''
            for x in lines[k + 1:k + 12]:
                if x.startswith('// GATE:'): gate = x[len('// GATE:'):].strip()
                elif x.startswith('//') or not x.strip(): continue
                else:
                    sig = x.strip(); break
            out[int(m.group(1), 16)] = (fn, re.sub(r'\s+', ' ', sig), gate, m.group(2))
    return out


def names_md():
    out = {}
    for l in open(os.path.join(ROOT, 'docs', 'NAMES.md')):
        m = re.match(r'\| `(0x[0-9A-F]+)` \| `([^`]+)` \| (\w+) \|[^|]*\| (.*) \|\s*$', l)
        if m: out[int(m.group(1), 16)] = (m.group(2), m.group(3), m.group(4))
    return out


def dm_names(dm):
    out = {}
    for node in ast.walk(ast.parse(open(os.path.join(dm, 'tools', 'ffxi_symbols_ida.py')).read())):
        if isinstance(node, ast.Call) and getattr(node.func, 'id', None) == 'apply_name' and len(node.args) >= 4:
            try:
                a, n, c, f = (ast.literal_eval(x) for x in node.args[:4])
                out[a] = (n, c)
            except ValueError: pass
    return out


def main(dm, crossf, outdir):
    im = S.Image(os.environ['FFXI_DLL']); ext = ld('func_extents.json')
    tree = {int(k, 16): v for k, v in json.load(open(os.path.join(ROOT, 'tree', 'placement.json'))).items()}
    tf = tree_functions(); nm = names_md(); dmn = dm_names(dm)
    cross = json.load(open(crossf)); cross_by = {int(r['addr'], 16): r for r in cross['names']}
    ida = {}
    for l in open(os.path.join(D, 'exports', 'gfx_export.jsonl')):
        r = json.loads(l); ida.setdefault(r['ea'], r['name'])
        for n, o in (r.get('objs') or {}).items(): ida.setdefault(o['ea'], n)
    pending = {int(k, 16): v['name'] for k, v in ld('names_pending_idb.json').items() if not k.startswith('_')}
    name = lambda a: pending.get(a) or (tree[a]['symbol'] if a in tree else None) or ida.get(a) or dmn.get(a, (None,))[0] or ('sub_%X' % a)
    purges, tails = ld('purges.json'), ld('tail_purges.json')
    ecx, edx, cdecl = ld('ecx_use.json'), ld('edx_use.json'), ld('cdecl_args.json')
    rw, rk, keeps, fs = ld('ret_width.json'), ld('ret_kinds.json'), ld('reg_keeps.json'), ld('frame_set.json')['functions']
    com = collections.defaultdict(list)
    for k, v in ld('com_calls.json').items():
        if not k.startswith('_') and isinstance(v, dict) and v.get('func'): com[int(v['func'], 16)].append('%s::%s' % (v.get('iface', '?'), v.get('method', '?')))
    import pefile
    pe = pefile.PE(os.environ['FFXI_DLL'], fast_load=True); base = pe.OPTIONAL_HEADER.ImageBase
    secs = [(x.Name.rstrip(b'\0').decode(), base + x.VirtualAddress, base + x.VirtualAddress + max(x.Misc_VirtualSize, x.SizeOfRawData)) for x in pe.sections]
    section = lambda a: next((n for n, lo, hi in secs if lo <= a < hi), None)
    iat = S.imports(im, os.environ['FFXI_DLL'])

    settled_sites = set()
    for f_ in ('vcalls.json', 'com_calls.json', 'sp_solved.json', 'game_sp.json'):
        settled_sites |= {'0x%08X' % int(k, 16) for k, v in ld(f_).items() if not k.startswith('_') and isinstance(v, dict) and v.get('pops') is not None}
    for r_ in ld('real_sp.json').values():
        if isinstance(r_, dict): settled_sites |= {'0x%08X' % int(k, 16) for k in r_.get('calls', {})}
    groups = collections.defaultdict(list)
    for a in sorted(tree):
        pl = tree[a]; key = '0x%08X' % a
        group = pl.get('class') or pl.get('unit') or 'free'
        e = ext.get('0x%08x' % a) or ext.get(key)
        callees, gr, gw, consts, imps = collections.Counter(), set(), set(), set(), collections.Counter()
        icalls = []
        if e:
            for x, i in im.code(e).items():
                if i.mnemonic == 'call' and i.operands and i.operands[0].type == X86_OP_IMM: callees[i.operands[0].imm] += 1
                elif i.mnemonic == 'call' and i.operands and not (i.operands[0].type == X86_OP_MEM and not i.operands[0].mem.base
                                                                   and not i.operands[0].mem.index and (i.operands[0].mem.disp & 0xFFFFFFFF) in iat):
                    icalls.append('0x%08X' % x)
                for n_, o in enumerate(i.operands):
                    if o.type == X86_OP_MEM and not o.mem.base and not o.mem.index:
                        g = o.mem.disp & 0xFFFFFFFF
                        if g in iat:
                            if i.mnemonic in ('call', 'jmp'): imps[str(iat[g]).split('!')[-1]] += 1
                            continue
                        sec = section(g)
                        if sec == '.rdata': consts.add(g); continue
                        if sec not in ('.data', '.data1'): continue
                        writes = n_ == 0 and i.mnemonic not in ('cmp', 'test', 'push', 'call', 'jmp') and not i.mnemonic.startswith(('j', 'fld', 'fild', 'fcom', 'fucom', 'ficom', 'fadd', 'fsub', 'fmul', 'fdiv'))
                        (gw if writes else gr).add(g)
        fn, sig, gate, why = tf.get(a, ('?', '', '', ''))
        L = ['### `0x%08X` %s' % (a, pl['symbol']), '']
        L.append('- **Verified C++** (`tree/src/%s`): `%s`' % (fn, sig.rstrip('{').strip()))
        L.append('- **Gate** (verification/gate_clean_final.jsonl, the live dumps): %s' % (gate or 'no gate line'))
        facts = []
        if key in purges: facts.append('`ret %d` (purges.json)' % purges[key])
        elif key in tails: facts.append('pops %d through its tail jump to %s (tail_purges.json)' % (tails[key]['pops'], ', '.join(tails[key]['via'])))
        if key in ecx: facts.append('entry ECX %s (ecx_use.json)' % ('read: `%s`' % ecx[key] if isinstance(ecx[key], str) else 'never read' if ecx[key] is False else 'not established'))
        if key in edx: facts.append('entry EDX %s (edx_use.json)' % ('read' if edx[key] else 'never read' if edx[key] is False else 'not established'))
        if key in cdecl: facts.append('reads %d argument slot(s) (cdecl_args.json)' % cdecl[key])
        if key in rw: facts.append('sets only %s of EAX before it returns (ret_width.json)' % ('AL' if rw[key] == 1 else 'AX'))
        if key in rk: facts.append('returns %s (ret_kinds.json)' % ('EAX = 0 or 1' if rk[key] == 'bool' else 'AL = 0 or 1, the rest left over' if rk[key] == 'lowbool' else rk[key]))
        if key in keeps: facts.append('leaves %s untouched (reg_keeps.json)' % ', '.join(x.upper() for x in keeps[key]))
        if facts: L.append('- **Convention facts:** ' + '; '.join(facts))
        if callees: L.append('- **Calls directly:** ' + ', '.join('`%s`%s' % (name(c), ' ×%d' % n if n > 1 else '') for c, n in sorted(callees.items())))
        if com.get(a): L.append('- **Direct3D 8 calls** (com_calls.json, proven device): ' + ', '.join('`%s`%s' % (m, ' ×%d' % n if n > 1 else '') for m, n in sorted(collections.Counter(com[a]).items())))
        if imps: L.append('- **Calls imports:** ' + ', '.join('`%s`%s' % (m, ' ×%d' % n if n > 1 else '') for m, n in sorted(imps.items())))
        if consts: L.append('- **Reads constants** (.rdata): %d' % len(consts))
        if icalls:
            settled = sum(1 for c in icalls if c in settled_sites)
            L.append('- **Indirect calls:** %d, of which %d have a pop count from a definitive source (vcalls, com_calls, sp_solved, real_sp, game_sp)' % (len(icalls), settled))
        if gr: L.append('- **Reads globals:** ' + ', '.join('`%s`' % name(g) if name(g) and not name(g).startswith('sub_') else '`0x%08X`' % g for g in sorted(gr)))
        if gw: L.append('- **Writes globals:** ' + ', '.join('`%s`' % name(g) if name(g) and not name(g).startswith('sub_') else '`0x%08X`' % g for g in sorted(gw)))
        f = fs.get(key) or fs.get('0x%08x' % a)
        L.append('- **Run by a frame** (frame_set.json): %s' % (', '.join(f['evidence']) if f else 'not shown (static reach from the main loop and the sampler did not find it)'))
        if a in dmn:
            c = cross_by.get(a, {})
            L.append('- **DancingMad name:** `%s` (%s)%s' % (dmn[a][0], c.get('dm_confidence', 'not stated'),
                     '' if c.get('names') in ('same', None) else ' — differs from this repository\'s `%s`' % c.get('our_name')))
        if a in nm:
            L.append('- **Summary** (inferred from reading the code, %s confidence; docs/NAMES.md in ffximain-gfx): %s' % (nm[a][1], nm[a][2]))
        elif a in dmn and dmn[a][1]:
            L.append('- **DancingMad note:** %s' % dmn[a][1])
        L.append('')
        groups[group].append('\n'.join(L))
    os.makedirs(outdir, exist_ok=True)
    idx = ['# Verified functions', '',
           'Every function in the verified C++ tree of the ffximain-gfx repository: %d functions, each gated against the game\'s own code. '
           'Generated by `pipeline/dm_reference.py` there; each line names the file it comes from. Statements marked *inferred* are '
           'interpretations, not facts.' % len(tree), '', '| Group | Functions |', '|---|---|']
    for g in sorted(groups):
        safe = re.sub(r'[^A-Za-z0-9_]+', '_', g)
        open(os.path.join(outdir, safe + '.md'), 'w').write('# %s\n\n' % g + '\n'.join(groups[g]))
        idx.append('| [%s](%s.md) | %d |' % (g, safe, len(groups[g])))
    open(os.path.join(outdir, 'README.md'), 'w').write('\n'.join(idx) + '\n')
    print('%d functions in %d groups -> %s' % (len(tree), len(groups), outdir))


if __name__ == '__main__':
    main(*sys.argv[1:4])
