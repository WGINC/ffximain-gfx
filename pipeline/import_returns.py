"""import_returns.py -- each import's declared return type, from the prototypes the exports carry: the Windows SDK's
(Ghidra applies windows_vs12_32.gdt by name, GfxExport `import_proto`; IDA its SDK type library). Only an import
every record types alike is listed.

A caller's EAX after an import call is the import's result when the SDK declares a 4-byte one: caller_reads.py
then knows the call writes all of EAX. The 4-byte and pointer types below are the SDK's own typedefs
(windef.h, winnt.h, basetsd.h); a return of any other type is listed with its size unknown (null), and the call is
then treated as one that may leave EAX as it was. An import no record types is taken from Microsoft's headers
(data/sdk_import_returns.json, sdk_protos.py), marked "src": "sdk".

    FFXI_DLL=... python3 import_returns.py data/import_returns.json
Output {"NAME": {"ret": "BOOL", "size": 4 | null}} for the imports in the DLL's import table."""
import json, os, re, sys
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import sp_solve as S
D = os.path.join(P, '..', 'data')
FOUR = {'BOOL', 'DWORD', 'UINT', 'INT', 'int', 'LONG', 'ULONG', 'HRESULT', 'LRESULT', 'WPARAM', 'LPARAM', 'MMRESULT',
        'SIZE_T', 'DWORD_PTR', 'UINT_PTR', 'INT_PTR', 'LONG_PTR', 'ULONG_PTR', 'COLORREF', 'LCID', 'HANDLE', 'FARPROC',
        'unsigned int', 'uint', 'long', 'unsigned long', 'SOCKET', 'u_long', 'LPSTR', 'LPCSTR', 'LPWSTR', 'LPVOID',
        'PVOID', 'LPTSTR', 'HKL', 'HIMC'}


def size_of(t):
    t = t.strip()
    if t == 'void': return 0
    if t.endswith('*') or t in FOUR or re.match(r'^H[A-Z]+$', t): return 4      # pointers; DECLARE_HANDLE types
    return None


def main(out):
    im = S.Image(os.environ['FFXI_DLL'])
    names = set(S.imports(im, os.environ['FFXI_DLL']).values())
    seen = {}
    for f in ('gfx_export.jsonl', 'ghidra_export.jsonl', 'ghidra_frameset.jsonl'):
        for l in open(os.path.join(D, 'exports', f)):
            for nm, o in (json.loads(l).get('objs') or {}).items():
                if not o.get('import') or o.get('import_proto') is False: continue
                m = re.match(r'^(.*?)\s*(__\w+)?\s*\(', o.get('type') or '')
                if m: seen.setdefault(nm.lstrip('_').replace('imp_', '') if nm.startswith('__imp_') else nm, set()).add(m.group(1).strip())
    res = {n: {'ret': next(iter(t)), 'size': size_of(next(iter(t)))} for n, t in sorted(seen.items()) if n in names and len(t) == 1}
    # the imports no record types: their declarations in Microsoft's headers (sdk_protos.py), with "src": "sdk".
    # Where both type an import, the records' type is kept; a size the headers give differently is reported
    sdk = json.load(open(os.path.join(D, 'sdk_import_returns.json')))
    for n, v in sorted(sdk.items()):
        if n not in names: continue
        if n not in res: res[n] = dict(v, src='sdk')
        elif res[n]['size'] is not None and res[n]['size'] != v['size']:
            print('%s: the records say %s, the headers %s' % (n, res[n]['ret'], v['ret']), file=sys.stderr)
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print('%d imports typed (%d of a known size), of %d in the import table' % (len(res), sum(v['size'] is not None for v in res.values()), len(names)))


if __name__ == '__main__':
    main(sys.argv[1])
