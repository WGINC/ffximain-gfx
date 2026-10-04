"""artifact_count.py -- how much decompiler residue is left: phase 0 of docs/DEARTIFACT.md.

Counts each kind of artifact in an IDA export (the pseudocode text) and, optionally, in a built tree
(src/*.cpp), so every de-artifacting round can show what it removed and that nothing came back:

  cast_this / cast_local / cast_arg / cast_global / cast_other
                 `*(_DWORD *)...`-style casts (_BYTE/_WORD/_DWORD/_QWORD), by the expression they start from
  untyped_vcall  a call through a cast vtable slot `(__thiscall **)` (any calling convention)
  partial_reg    LOBYTE / HIBYTE / LOWORD / HIWORD / LODWORD / HIDWORD / BYTEn / WORDn / SLOBYTE ...
  sub_name       uses of an address-only function name sub_XXXXXXXX
  data_name      uses of an address-only data name dword_/unk_/off_/byte_/word_/qword_/flt_/dbl_/stru_XXXXXXXX
  anon_var       uses of Hex-Rays' numbered locals and arguments vN / aN

    python3 artifact_count.py EXPORT.jsonl [TREE_DIR] [--out verification/artifacts.json] [--label NAME]
With --out the counts are added under the label (default: the export's sha1) and the file is rewritten.
"""
import argparse, collections, glob, hashlib, json, os, re

CAST = re.compile(r'\*\s*\(\s*\(?\s*_(?:BYTE|WORD|DWORD|QWORD)\s*\*\s*\)\s*\(?\s*&?\s*\(?\s*(\w+)')
PATTERNS = {
    'untyped_vcall': r'\(__\w+ \*\*\)',
    'partial_reg': r'\b(?:S?LO|S?HI)(?:BYTE|WORD|DWORD)\s*\(|\bS?(?:BYTE|WORD)\d\s*\(',
    'sub_name': r'\bsub_[0-9A-F]{6,8}\b',
    'data_name': r'\b(?:dword|unk|off|byte|word|qword|flt|dbl|stru)_[0-9A-F]{6,8}\b',
    'anon_var': r'\b[va]\d+\b',
}


def count(texts):
    c = collections.Counter()
    for t in texts:
        for m in CAST.finditer(t):
            b = m.group(1)
            c['cast_' + ('this' if b == 'this' else 'arg' if re.fullmatch(r'a\d+', b) else 'local' if re.fullmatch(r'v\d+', b)
                         else 'global' if re.match(r'(?:g_|dword_|off_|unk_|byte_|word_|qword_|flt_|dbl_|stru_)', b) else 'other')] += 1
        for k, p in PATTERNS.items(): c[k] += len(re.findall(p, t))
    c['casts'] = sum(v for k, v in c.items() if k.startswith('cast_'))
    return dict(sorted(c.items()))


def main():
    ap = argparse.ArgumentParser(); ap.add_argument('export'); ap.add_argument('tree', nargs='?')
    ap.add_argument('--out'); ap.add_argument('--label')
    a = ap.parse_args()
    recs = [json.loads(l) for l in open(a.export)]
    res = {'export': count(r.get('text', '') for r in recs), 'functions': len(recs),
           'export_sha1': hashlib.sha1(open(a.export, 'rb').read()).hexdigest()}
    if a.tree:
        res['tree'] = count(open(f, encoding='utf-8', errors='replace').read() for f in sorted(glob.glob(os.path.join(a.tree, 'src', '*.cpp'))))
    for k in ('export', 'tree'):
        if k in res: print('%-6s %s' % (k, ' '.join('%s=%d' % x for x in res[k].items())))
    if a.out:
        try: allr = json.load(open(a.out))
        except (OSError, ValueError): allr = {'_doc': __doc__.split('\n\n')[0]}
        allr[a.label or res['export_sha1']] = res
        json.dump(allr, open(a.out, 'w'), indent=1, sort_keys=True)


if __name__ == '__main__':
    main()
