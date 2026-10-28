"""genoxd_scan.py -- two community libraries' names and layouts for FFXiMain.dll, located in the analysed build.

docs/COMMUNITY.md's rules, for two more sources (each checked out locally, never run; read as data):

  * ffxi_hide_ui (github.com/genoxd/ffxi_hide_ui, 0BSD): engine/signatures.h, `const char kSigX[] = "8b 44 ?? ..."`
    with the comment above each; engine/game.h, the menu object's fields `const size_t kMenuX = 0x..; // comment`;
  * ffxi_world_draw (github.com/genoxd/ffxi_world_draw, 0BSD): ffxi_world_draw.h, the renderer and camera patterns
    (`renderer_pattern_` / `camera_pattern_` with their masks and the operand positions `*_at_`), and the fields it
    reads, `renderer_*_offset_` / `camera_*_offset_`.

Located (fact): a pattern matching the DLL exactly once. Its start, where it is a function start (data/func_extents.json),
is that function; elsewhere it is a site inside the function holding it. A world_draw pattern names a global by the
operand at its stated position. Patterns matching more than once, or not at all, are listed and set aside.

Used (fact): a field offset a source claims is checked against the game's code -- whether an instruction with that
displacement ([reg + offset]) occurs in the functions the source ties to the object: the menu, in the functions
ffxi_hide_ui's signatures locate (its own routines on the menu); the renderer, in CDx's methods
(tree/include/CDx.hpp, and DancingMad's CDx_ names in data/dm_names.json); the camera, in the functions that load its global. That the offset is used there is a fact;
what the field means is the source's knowledge, kept with it. Where data/field_names.json names the same offset, both
are listed.

    FFXI_DLL=... python3 pipeline/genoxd_scan.py HIDE_UI_DIR WORLD_DRAW_DIR data/genoxd_scan.json
"""
import json, os, re, subprocess, sys
import capstone, pefile

P = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.join(P, '..'); D = os.path.join(ROOT, 'data')


def commit(d):
    try: return subprocess.run(['git', '-C', d, 'rev-parse', '--short', 'HEAD'], capture_output=True, text=True).stdout.strip()
    except OSError: return ''


def hideui_sigs(d):
    """[(name, tokens, comment)] from engine/signatures.h"""
    src = open(os.path.join(d, 'engine', 'signatures.h'), errors='replace').read()
    out = []
    for m in re.finditer(r'((?:^[ \t]*//[^\n]*\n)*)[ \t]*const char (kSig\w+)\[\]\s*=\s*((?:\s*"[^"]*")+)\s*;', src, re.M):
        note = ' '.join(l.strip()[2:].strip() for l in m.group(1).splitlines() if l.strip())
        out.append((m.group(2), ' '.join(re.findall(r'"([^"]*)"', m.group(3))).split(), note))
    return out


def hideui_fields(d):
    """{offset: [(name, comment)]} from engine/game.h's kMenu constants"""
    out = {}
    for l in open(os.path.join(d, 'engine', 'game.h'), errors='replace'):
        m = re.match(r'\s*const size_t (kMenu\w+)\s*=\s*(0x[0-9A-Fa-f]+);\s*(?://\s*(.*))?$', l)
        if m and m.group(1) != 'kMenuBytes': out.setdefault(int(m.group(2), 16), []).append((m.group(1), (m.group(3) or '').strip()))
    return out


def worlddraw(d):
    h = open(os.path.join(d, 'ffxi_world_draw.h'), errors='replace').read()
    def arr(n):
        m = re.search(r'%s\[\d+\]\s*=\s*\{([^}]*)\}' % n, h); return [int(x, 16) for x in re.findall(r'0x[0-9A-Fa-f]+', m.group(1))]
    def mask(n): return re.search(r'%s\[\d+\]\s*=\s*"([^"]*)"' % n, h).group(1)
    def const(n): return int(re.search(r'%s\s*=\s*(0x[0-9A-Fa-f]+|\d+);' % n, h).group(1), 0)
    pats = {'renderer': (arr('renderer_pattern_'), mask('renderer_mask_'), {'renderer global': 2}),
            'camera': (arr('camera_pattern_'), mask('camera_mask_'),
                       {'camera global': const('camera_pattern_global_at_'), 'renderer global': const('camera_pattern_renderer_at_')})}
    fields = {}
    for m in re.finditer(r'((?:^[ \t]*//[^\n]*\n)*)[ \t]*static constexpr std::uintptr_t ((renderer|camera)_\w+_offset_)\s*=\s*(0x[0-9A-Fa-f]+);', h, re.M):
        note = ' '.join(l.strip()[2:].strip() for l in m.group(1).splitlines() if l.strip())
        fields.setdefault(m.group(3), {}).setdefault(int(m.group(4), 16), []).append((m.group(2), note))
    return pats, fields


def main(hd, wd, out):
    pe = pefile.PE(os.environ['FFXI_DLL'], fast_load=True); img = bytes(pe.get_memory_mapped_image()); base = 0x10000000
    ext = {int(k, 16): v for k, v in json.load(open(os.path.join(D, 'func_extents.json'))).items() if not k.startswith('_')}
    def holder(a):
        return next((f for f, spans in ext.items() for s, t in spans if s <= a < t), None)
    def find(tokens):
        rx = re.compile(b''.join(b'.' if t == '??' else re.escape(bytes([int(t, 16)])) for t in tokens), re.S)
        return [base + m.start() for m in rx.finditer(img)]
    cs = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32); cs.detail = True
    def disps(f):
        """{displacement: first instruction address} of the [reg + disp] memory operands in function f"""
        r = {}
        for s, t in ext.get(f, []):
            for i in cs.disasm(img[s - base:t - base], s):
                for op in i.operands:
                    if op.type == capstone.x86.X86_OP_MEM and op.mem.base != 0 and op.mem.disp > 0:
                        r.setdefault(op.mem.disp, '0x%08X %s %s' % (i.address, i.mnemonic, i.op_str))
        return r
    u32 = lambda a: int.from_bytes(img[a - base:a - base + 4], 'little')
    res = {'_doc': __doc__.split('\n\n')[0], 'sources': {'ffxi_hide_ui': {'commit': commit(hd), 'licence': '0BSD'},
                                                       'ffxi_world_draw': {'commit': commit(wd), 'licence': '0BSD'}},
           'functions': {}, 'sites': {}, 'globals': {}, 'unmatched': [], 'ambiguous': [], 'fields': {}}
    menu_funcs = set()
    for name, toks, note in hideui_sigs(hd):
        hits = find(toks)
        rec = {'name': name, 'source': 'ffxi_hide_ui', 'file': 'engine/signatures.h', 'note': note}
        if not hits: res['unmatched'].append(rec); continue
        if len(hits) > 1: res['ambiguous'].append(dict(rec, matches=len(hits))); continue
        a = hits[0]
        if a in ext: res['functions'].setdefault('0x%08X' % a, []).append(rec); menu_funcs.add(a)
        else:
            f = holder(a)
            res['sites'].setdefault('0x%08X' % a, []).append(dict(rec, function='0x%08X' % f if f else None))
    pats, wfields = worlddraw(wd)
    globs = {}
    for obj, (pat, mask, at) in pats.items():
        toks = ['??' if m == '?' else '%02x' % b for b, m in zip(pat, mask)]
        hits = find(toks)
        rec = {'name': obj + '_pattern_', 'source': 'ffxi_world_draw', 'file': 'ffxi_world_draw.h'}
        if len(hits) != 1: (res['unmatched'] if not hits else res['ambiguous']).append(dict(rec, matches=len(hits))); continue
        for what, k in at.items():
            g = u32(hits[0] + k)
            globs.setdefault(what, set()).add(g)
            res['globals'].setdefault('0x%08X' % g, []).append(dict(rec, name=what, match='0x%08X' % hits[0], operand_at=k))
    gn = {}
    for n, a in re.findall(r'#define (\w+) XI_EA\(\w+, (0x[0-9A-Fa-f]+)u\)', open(os.path.join(ROOT, 'tree', 'include', 'gfx_globals.hpp')).read()):
        gn.setdefault(int(a, 16), n)
    for g, recs in res['globals'].items():
        for r in recs: r['ours'] = gn.get(int(g, 16))
    # the objects' functions, for the field check
    cdx = {int(a, 16) for a in re.findall(r'//\s*(0x[0-9A-Fa-f]{8})', open(os.path.join(ROOT, 'tree', 'include', 'CDx.hpp')).read())}
    # and the CDx methods outside the tree, by DancingMad's names (data/dm_names.json: CDx_...)
    cdx |= {int(a, 16) for a, v in json.load(open(os.path.join(D, 'dm_names.json'))).items()
            if not a.startswith('_') and isinstance(v, list) and str(v[0]).startswith('CDx_')}
    cam = set()
    for g in globs.get('camera global', ()):
        for m in re.finditer(re.escape(b'\xa1' + g.to_bytes(4, 'little')) + b'|' + re.escape(b'\x8b') + b'[\x0d\x15\x1d\x35\x3d\x05]' + re.escape(g.to_bytes(4, 'little')), img):
            f = holder(base + m.start())
            if f: cam.add(f)
    fnames = json.load(open(os.path.join(D, 'field_names.json')))
    def check(obj, claims, funcs, ours_cls):
        dd = {}
        for f in sorted(funcs):
            for k, v in disps(f).items(): dd.setdefault(k, v)
        ours = fnames.get(ours_cls, {}) if ours_cls else {}
        for off, cl in sorted(claims.items()):
            e = {'claims': [{'name': n, 'note': c} for n, c in cl], 'used': dd.get(off), 'functions_read': len(funcs)}
            o = ours.get('0x%X' % off)
            if o: e['ours'] = {'name': o['name'], 'evidence': o['evidence']}
            res['fields'].setdefault(obj, {})['0x%X' % off] = e
    check('menu (ffxi_hide_ui game.h)', hideui_fields(hd), menu_funcs, None)
    check('renderer = CDx (ffxi_world_draw)', wfields.get('renderer', {}), cdx, 'CDx')
    check('camera (ffxi_world_draw)', wfields.get('camera', {}), cam, None)
    json.dump(res, open(out, 'w'), indent=1, sort_keys=True)
    used = sum(1 for o in res['fields'].values() for e in o.values() if e['used'])
    print('functions %d, sites %d, globals %d, unmatched %d, ambiguous %d; fields %d, used in the code %d' % (
        len(res['functions']), len(res['sites']), len(res['globals']), len(res['unmatched']), len(res['ambiguous']),
        sum(len(o) for o in res['fields'].values()), used))


if __name__ == '__main__':
    main(*sys.argv[1:4])
