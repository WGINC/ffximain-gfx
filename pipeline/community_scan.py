"""community_scan.py -- the FFXI community's names for places in FFXiMain.dll, located exactly in the analysed build.

The community tools that read the live client (Ashita v4, Windower 5) find what they use by byte patterns, and
name it: a function, or the global that holds an object, with the object's fields at offsets. A pattern that
matches the analysed DLL exactly once ties the community's name to one address; that match is a fact, the name is
the community's knowledge, kept with its source. Read here (each repository checked out locally, never run):

  * Ashita config/ashita/ashita.pointers.ini: [name] module = FFXiMain.dll, pattern (?? wildcards), offset;
  * Ashita addons: ashita.memory.find(0, 0, 'pattern', offset, count) with the name in a trailing `-- comment`;
  * Windower 5 libraries/memory/types.lua: types.X = struct({signature = '...', offsets = {...}}, ...)
    (Windower's scanner returns the address just after the match, dereferenced once; `&` marks a position returned
    as it is, `*` one dereferenced).

At the returned position R (Ashita: match + offset): a function start is that function; otherwise the dword at R,
where it is an address in the image, is the function or global it names (the instruction's operand).

    FFXI_DLL=... python3 community_scan.py ASHITA_DIR WINDOWER5_DIR OUT.json
Output {"functions": {"0xF": [{name, source, file}]}, "globals": {"0xG": [...]}, "unmatched": [...], "ambiguous": [...]}"""
import configparser, glob, json, os, re, struct, sys
P = os.path.dirname(os.path.abspath(__file__)); D = os.path.join(P, '..', 'data'); sys.path.insert(0, P)
import sp_solve as S


def compile_pat(p):
    """(regex over bytes, offset of the & marker or None, offset of the * marker or None, pattern length)"""
    p = re.sub(r'\s+', '', p)
    amp = star = None; out = b''; i = 0; n = 0
    while i < len(p):
        c = p[i]
        if c == '&': amp = n; i += 1; continue
        if c == '*': star = n; i += 1; continue
        if p[i:i + 2] == '??': out += b'.'; i += 2; n += 1; continue
        if c == '?': out += b'.'; i += 1; n += 1; continue          # a single ? as one wildcard byte
        out += re.escape(bytes([int(p[i:i + 2], 16)])); i += 2; n += 1
    return re.compile(out, re.S), amp, star, n


def entries(ashita, w5):
    c = configparser.ConfigParser(inline_comment_prefixes=(';',), strict=False)
    c.read(os.path.join(ashita, 'config', 'ashita', 'ashita.pointers.ini'))
    for s in c.sections():
        if 'ffximain' in c[s].get('module', '').lower():
            yield {'name': s, 'pattern': c[s]['pattern'], 'offset': int(c[s].get('offset', '0'), 0),
                   'count': int(c[s].get('count', '0'), 0), 'source': 'ashita pointers.ini', 'file': 'config/ashita/ashita.pointers.ini'}
    rx = re.compile(r"ashita\.memory\.find\(\s*0\s*,\s*0\s*,\s*'([0-9A-Fa-f?]+)'\s*,\s*(-?(?:0x)?[0-9A-Fa-f]+)\s*,\s*(\d+)\s*\)[^\n]*?--\s*(.+)$")
    for f in sorted(glob.glob(os.path.join(ashita, 'addons', '**', '*.lua'), recursive=True)):
        if '/annotations/' in f: continue
        for l in open(f, errors='replace'):
            m = rx.search(l)
            if m:
                yield {'name': m.group(4).strip(), 'pattern': m.group(1), 'offset': int(m.group(2), 0), 'count': int(m.group(3)),
                       'source': 'ashita addon', 'file': os.path.relpath(f, ashita)}
    tl = os.path.join(w5, 'libraries', 'memory', 'types.lua')
    for m in re.finditer(r"types\.(\w+)\s*=\s*\w+\(\{\s*signature\s*=\s*'([^']+)'(?:\s*,\s*offsets\s*=\s*\{([^}]*)\})?(?:\s*,\s*module\s*=\s*'([^']+)')?",
                         open(tl).read()):
        if m.group(4) and 'ffximain' not in m.group(4).lower(): continue
        yield {'name': 'windower5 types.' + m.group(1), 'pattern': m.group(2), 'offset': 0, 'count': 0,
               'offsets': [int(x, 0) for x in (m.group(3) or '').split(',') if x.strip()],
               'source': 'windower5 types.lua', 'file': 'libraries/memory/types.lua'}


def main(ashita, w5, out):
    im = S.Image(os.environ['FFXI_DLL']); ext = json.load(open(os.path.join(D, 'func_extents.json')))
    starts = {int(k, 16) for k in ext if not k.startswith('_')}
    img = bytes(im.img); base = im.base
    res = {'functions': {}, 'globals': {}, 'unmatched': [], 'ambiguous': []}
    for e in entries(ashita, w5):
        rx, amp, star, plen = compile_pat(e['pattern'])
        hits = [m.start() for m in rx.finditer(img)]
        if not hits: res['unmatched'].append(e); continue
        if len(hits) > 1 and not e['count']: res['ambiguous'].append(dict(e, matches=len(hits))); continue
        h = hits[min(e['count'], len(hits) - 1)]
        rec = {'name': e['name'], 'source': e['source'], 'file': e['file'], 'match': '0x%08X' % (base + h)}
        if e.get('offsets'): rec['offsets'] = ['0x%X' % x for x in e['offsets']]
        if e['source'].startswith('windower'):
            # Windower's scanner: the address at `&` (not dereferenced), at `*` (dereferenced), else just after the
            # match, dereferenced once (Windower/packages wiki, scanner)
            if amp is not None:
                r = base + h + amp
                if r in starts: res['functions'].setdefault('0x%08X' % r, []).append(rec); continue
            else:
                r = base + h + (star if star is not None else plen)
            v = struct.unpack_from('<I', img, r - base)[0] if base <= r < base + len(img) - 4 else None
            if v in starts: res['functions'].setdefault('0x%08X' % v, []).append(dict(rec, via='dereferenced'))
            elif v is not None and base <= v < base + len(img): res['globals'].setdefault('0x%08X' % v, []).append(rec)
            else: res['unmatched'].append(dict(e, why='matched at 0x%08X; its position holds no address in the image' % (base + h)))
            continue
        r = base + h + e['offset']                       # Ashita: the match plus the offset, as an address
        if r in starts:
            res['functions'].setdefault('0x%08X' % r, []).append(rec)
            continue
        v = struct.unpack_from('<I', img, r - base)[0] if base <= r < base + len(img) - 4 else None
        if v is not None and base <= v < base + len(img):
            if v in starts: res['functions'].setdefault('0x%08X' % v, []).append(dict(rec, via='operand'))
            else: res['globals'].setdefault('0x%08X' % v, []).append(rec)
        else:
            rel = struct.unpack_from('<i', img, r - base + 1)[0] if img[r - base] == 0xE8 else None
            t = r + 5 + rel if rel is not None else None
            if t in starts: res['functions'].setdefault('0x%08X' % t, []).append(dict(rec, via='call at the match'))
            else: res['unmatched'].append(dict(e, why='matched at 0x%08X, but the position names no function or global' % r))
    json.dump(res, open(out, 'w'), indent=1, sort_keys=True)
    print('functions %d, globals %d, unmatched %d, ambiguous %d' % (len(res['functions']), len(res['globals']),
                                                                  len(res['unmatched']), len(res['ambiguous'])))


if __name__ == '__main__':
    main(*sys.argv[1:4])
