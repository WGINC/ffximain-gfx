"""hx_patches.py -- hand corrections to Hex-Rays output, kept as data and applied to the export.

Some functions decompile wrongly in ways no translator rule can repair (pushed arguments mistaken
for stack variables, a wrong pointer). A patch is data/hx_patches/0xADDR.json:

  {"why": "...", "evidence": "...", "text_sha1": "<sha1 of the export text it was written against>",
   "replace": [["exact text in the export", "corrected text"], ...]}

It is applied only while the export's text still has that sha1, so a re-export (for example after
the IDB was fixed) is never silently overridden: the patch is reported as stale instead.
Both hx2cpp.py (raw translation) and hx2clean.py (the clean tree) call apply().
"""
import hashlib, json, os, sys

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


def apply(rec):
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
