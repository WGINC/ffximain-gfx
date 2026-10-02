"""hx_patches.py -- hand corrections to Hex-Rays output, kept as data and applied to the export.

Some functions decompile wrongly in ways no translator rule can repair (pushed arguments mistaken
for stack variables, a wrong pointer). A patch is data/hx_patches/0xADDR.json:

  {"why": "...", "evidence": "...", "text_sha1": "<sha1 of the export text it was written against>",
   "replace": [["exact text in the export", "corrected text"], ...]}

It is applied only while the export's text still has that sha1, so a re-export (for example after
the IDB was fixed) is never silently overridden: the patch is reported as stale instead.
Both hx2cpp.py (raw translation) and hx2clean.py (the clean tree) call apply().

Type corrections, data/type_fixes.json: {"0xADDR": {"type": ..., "why": ..., "evidence": ...}}. For a
function the type is its prototype without a name; it replaces the IDB's wherever the function is
referenced (each record's `objs`) and as the function's own signature. For data it is the object's
type (an array type also sets its size), wherever the object is referenced. Use it for a prototype that is wrong in the
IDB and has not been fixed there yet (pipeline/ida_apply_names.py pushes these to the IDB too).
"""
import hashlib, json, os, re, sys

DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'hx_patches')
_P = None


def load():
    global _P
    if _P is None:
        _P = {}
        if os.path.isdir(DIR):
            for f in os.listdir(DIR):
                if f.endswith('.json'): _P[int(f[:-5], 16)] = json.load(open(os.path.join(DIR, f)))
    return _P


def sha1(text): return hashlib.sha1(text.encode()).hexdigest()


TYPE_FIXES = os.path.join(DIR, '..', 'type_fixes.json')
_T = None


def type_fixes():
    global _T
    if _T is None:
        _T = {int(k, 16): v['type'] for k, v in json.load(open(TYPE_FIXES)).items()} if os.path.exists(TYPE_FIXES) else {}
    return _T


def fix_types(rec):
    t = type_fixes()
    objs = rec.get('objs', {})
    if not any(o.get('ea') in t for o in objs.values()) and rec['ea'] not in t: return rec
    def fixed(o):
        if o.get('ea') not in t: return o
        if o.get('func'): return dict(o, type=t[o['ea']])
        n = re.search(r'\[(\d+)\]$', t[o['ea']])          # data: an array type also gives the size
        elem = {'_BYTE': 1, 'char': 1, '_WORD': 2, '__int16': 2, '_DWORD': 4, 'int': 4, 'float': 4, '_QWORD': 8, 'double': 8}
        size = int(n.group(1)) * elem.get(t[o['ea']][:n.start()].strip(), 0) if n else o.get('size')
        return dict(o, type=t[o['ea']], size=size or o.get('size'))
    rec = dict(rec, objs={n: fixed(o) for n, o in objs.items()})
    # a callee whose corrected return is a full int: `LOWORD(x) = f(...)` (from a 16-bit IDB type) keeps
    # all 32 bits, as the original's `push eax` / `test eax, eax` after the call does
    for n, o in objs.items():
        if o.get('func') and o.get('ea') in t and re.match(r'(unsigned )?int\s', t[o['ea']]):
            rec['text'] = re.sub(r'\bLOWORD\((\w+)\) = (%s\()' % re.escape(n), r'\1 = \2', rec['text'])
    if rec['ea'] in t:
        rec['type'] = t[rec['ea']]
        if '(' in t[rec['ea']]:
            rec['ret'] = re.sub(r'\s*(__\w+)?\s*$', '', t[rec['ea']][:t[rec['ea']].index('(')]).strip() or rec.get('ret')
        new = [x.strip() for x in t[rec['ea']][t[rec['ea']].index('(') + 1:t[rec['ea']].rindex(')')].split(',')]
        new = {re.search(r'(\w+)$', x).group(1): re.sub(r'\s*\w+$', '', x) for x in new if re.search(r'\w\s+\**\w+$', x)}
        text = rec['text']; m = re.search(r'^[^\n]*\b%s\(([^)]*)\)' % re.escape(rec['name']), text, re.M)
        if m:
            params = ', '.join(re.sub(r'^(.*?)(\**\s*\b(\w+))$', lambda q: (new[q.group(3)] + ' ' + q.group(3)) if q.group(3) in new else q.group(0), x.strip())
                               for x in m.group(1).split(','))
            rec['text'] = text[:m.start(1)] + params + text[m.end(1):]
        if rec.get('args'):
            rec['args'] = [[a[0], new.get(a[0], a[1])] + list(a[2:]) for a in rec['args']]
    return rec


def apply(rec):
    return fix_types(patch_text(rec))


def patch_text(rec):
    p = load().get(rec['ea'])
    if not p: return rec
    if sha1(rec['text']) != p['text_sha1']:
        print('hx_patches: 0x%08X stale (export text changed), not applied' % rec['ea'], file=sys.stderr)
        return rec
    text = rec['text']
    for old, new in p['replace']:
        if text.count(old) != 1:
            print('hx_patches: 0x%08X snippet not found exactly once, not applied' % rec['ea'], file=sys.stderr)
            return rec
        text = text.replace(old, new)
    return dict(rec, text=text)
