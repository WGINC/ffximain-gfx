"""com_vtables.py -- which D3D8 interface a vtable is, from the objects the device's Create methods returned in the
running game (xikit profile=1 or trace=1: xikit_vtables_<pid>.csv, kit/d3dprof/d3dprof.c vt_note).

The D3D8 header declares the interface of each Create method's out parameter (CreateVertexBuffer returns an
IDirect3DVertexBuffer8, ...). The kit records, the first time it sees it, the vtable of each object a Create method
returned, with the vtable's entries (as many as data/com_spec.json gives the interface), each as module+offset.

    python3 com_vtables.py ingest xikit_vtables_<pid>.csv [...] [--out data/com_vtables.json]
        a vtable is recorded with its interface and its entries; two records of one vtable must agree on both,
        else the vtable is recorded as a conflict and never used
    python3 com_vtables.py --selftest

The gate (diffemu_gfx.py, _com_target) uses a record only when the captured vtable's entries are exactly the
recorded ones (the same implementation, module-relative), so a record taken with one build of a Direct3D 8
implementation never names a vtable of another."""
import argparse, json, os, sys
P = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.join(P, '..')
SPEC = json.load(open(os.path.join(ROOT, 'data', 'com_spec.json')))


def parse(lines):
    for l in lines:
        l = l.strip()
        if not l or l.startswith('#'): continue
        f = l.split(',')
        iface, vt, entries = f[0], f[1], f[2:]
        if iface not in SPEC: raise ValueError('not an interface com_spec.json has: %s' % iface)
        if len(entries) != len(SPEC[iface]): raise ValueError('%s: %d entries, the header gives %d' % (vt, len(entries), len(SPEC[iface])))
        yield iface, vt.lower(), [e.lower() for e in entries]


def ingest(files, old=None):
    out = dict(old or {})
    for path in files:
        for iface, vt, entries in parse(open(path)):
            r = out.get(vt)
            if r is None:
                out[vt] = {'interface': iface, 'entries': entries,
                           'source': "an object the device's Create method returned (the D3D8 header's out-parameter "
                                     "interface) had this vtable, in the running game (xikit vt_note): %s" % os.path.basename(path)}
            elif r.get('conflict') or r['interface'] != iface or r['entries'] != entries:
                out[vt] = {'conflict': sorted({r.get('interface', '?'), iface}),
                           'source': 'two records of this vtable disagree; never used'}
    return out


def lookup(table, vt_name, entries):
    """The interface of the vtable named vt_name (module+offset, any case) whose entries (module+offset) are
    `entries`, or None: no record, a conflicting one, or entries that differ from the recorded ones."""
    r = table.get(vt_name.lower())
    if not r or r.get('conflict'): return None
    if [e.lower() for e in entries[:len(r['entries'])]] != r['entries']: return None
    return r['interface']


def selftest():
    a = ['# vtables ...', 'IDirect3DVertexBuffer8,d3d8.dll+0x12A654,' + ','.join('d3d8.dll+0x%X' % (0xC0000 + 16 * k) for k in range(14))]
    b = ['IDirect3DSurface8,d3d8.dll+0x12A654,' + ','.join('d3d8.dll+0x%X' % (0xC0000 + 16 * k) for k in range(11))]
    import tempfile
    d = tempfile.mkdtemp(); pa = os.path.join(d, 'a.csv'); pb = os.path.join(d, 'b.csv')
    open(pa, 'w').write('\n'.join(a) + '\n'); open(pb, 'w').write('\n'.join(b) + '\n')
    t = ingest([pa]); ent = ['D3D8.dll+0x%X' % (0xC0000 + 16 * k) for k in range(14)]
    ok = lookup(t, 'D3D8.DLL+0x12a654', ent) == 'IDirect3DVertexBuffer8'
    ok &= lookup(t, 'd3d8.dll+0x12A654', ent[:13] + ['d3d8.dll+0x1']) is None        # another build's entries
    ok &= lookup(t, 'd3d8.dll+0x12A658', ent) is None                                 # no record
    t2 = ingest([pa, pb]); ok &= lookup(t2, 'd3d8.dll+0x12A654', ent) is None and t2['d3d8.dll+0x12a654'].get('conflict') is not None
    try: list(parse(['IDirect3DVertexBuffer8,x.dll+0x0,x.dll+0x1'])); ok = False
    except ValueError: pass
    print('%s: selftest (a record names its vtable; other entries, no record or a conflict name nothing; a short line is refused)' % ('ok' if ok else 'FAIL'))
    return ok


def main():
    if '--selftest' in sys.argv: sys.exit(0 if selftest() else 1)
    ap = argparse.ArgumentParser(); sp = ap.add_subparsers(dest='cmd', required=True)
    a = sp.add_parser('ingest'); a.add_argument('files', nargs='+'); a.add_argument('--out', default=os.path.join(ROOT, 'data', 'com_vtables.json'))
    o = ap.parse_args()
    old = json.load(open(o.out)) if os.path.exists(o.out) else {}
    t = ingest(o.files, old)
    json.dump(t, open(o.out, 'w'), indent=1, sort_keys=True)
    for k, v in sorted(t.items()): print('%s: %s' % (k, v.get('interface') or 'CONFLICT %s' % v['conflict']))


if __name__ == '__main__':
    main()
