"""import_libs.py -- what Microsoft's import libraries say about the functions the game imports: the name of each one
imported by ordinal only, and the stack bytes each one's call pops.

An import library (the SDK's <DLL>.LIB, here Visual Studio 6's VC98/LIB, the game's compiler's) holds one short
import member per exported function (Microsoft PE/COFF specification, "Import Library Format"): the ordinal or hint,
the name type, and the symbol as the compiler decorated it from the SDK header's declaration.
  ordinals     a member whose name type is IMPORT_ORDINAL (0) carries the ordinal itself: the game's WS2_32.dll
               imports are by ordinal only, so Ghidra calls them `Ordinal_N` and finds no SDK prototype; the
               member names them (GfxExport.java applies that name's prototype). A member with a hint (name type
               1-3) says nothing about the ordinal and is not used for it.
  stack_bytes  `_Name@N` is the decoration of a __stdcall function with N argument bytes, which it pops; `_Name`
               (no `@`) of a __cdecl one, which pops 0. A C++-decorated symbol (`?...`) is not used.
               diffemu_gfx.py uses these for an import data/import_pops.json (the game's solved call sites) does not
               list; where both list an import they must agree (a disagreement is reported and the import left out).
  stdcall_args a __stdcall one's argument count, its bytes / 4 (every Win32 argument is a dword), for an import
               data/import_nargs.json (IDA's SDK prototypes) does not list; checked against it where it does.
pefile also names the WS2_32 ordinals (its own table, which sp_solve.imports uses): each name it gives is checked
against the library here and a disagreement reported.

    FFXI_DLL=... python3 import_libs.py VC98_LIB_DIR OUT.json
Output {"ordinals": {"WS2_32.dll!115": "WSAStartup"}, "stack_bytes": {"ImmGetContext": 4}, "stdcall_args": {...},
        "_unnamed": [ordinal imports no member names], "_disagree": [...], "_source": ...}."""
import json, os, re, struct, sys
import pefile


def members(path):
    """[(ordinal or hint, name type, symbol)] for the short import members of an import library"""
    d = open(path, 'rb').read()
    assert d[:8] == b'!<arch>\n', path
    out, p = [], 8
    while p + 60 <= len(d):
        size = int(d[p + 48:p + 58]); body = d[p + 60:p + 60 + size]
        if len(body) >= 20 and body[:4] == b'\x00\x00\xff\xff':
            oh, typ = struct.unpack('<HH', body[16:20])
            out.append((oh, (typ >> 2) & 7, body[20:].split(b'\0')[0].decode()))
        p += 60 + size + (size & 1)
    return out


def undecorate(sym):
    """('Name', stack bytes, __stdcall?) for `_Name@N` (__stdcall) or `_Name` (__cdecl); None for anything else"""
    m = re.match(r'^_([A-Za-z_]\w*?)(?:@(\d+))?$', sym)
    return (m.group(1), int(m.group(2)) if m.group(2) else 0, m.group(2) is not None) if m else None


def main(libdir, out):
    pe = pefile.PE(os.environ['FFXI_DLL'])
    libs = {f.upper(): os.path.join(libdir, f) for f in os.listdir(libdir)}
    pops = json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'import_pops.json')))
    nargs = json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'import_nargs.json')))
    ords, sb, na, unnamed, disagree, nolib = {}, {}, {}, [], [], []
    for e in pe.DIRECTORY_ENTRY_IMPORT:
        dll = e.dll.decode()
        lib = libs.get(os.path.splitext(dll)[0].upper() + '.LIB')
        if not lib: nolib.append(dll); continue
        ms = members(lib)
        by_ord = {oh: undecorate(s) for oh, nt, s in ms if nt == 0 and undecorate(s)}
        by_name = {}
        for oh, nt, s in ms:
            u = undecorate(s)
            if u: by_name.setdefault(u[0], set()).add(u[1:])
        for i in e.imports:
            if i.import_by_ordinal:
                if i.ordinal not in by_ord: unnamed.append('%s!%d' % (dll, i.ordinal)); continue
                nm = by_ord[i.ordinal][0]
                ords['%s!%d' % (dll, i.ordinal)] = nm
                if i.name and i.name.decode() != nm: disagree.append('%s!%d: pefile %s, library %s' % (dll, i.ordinal, i.name.decode(), nm))
            else:
                nm = i.name.decode()
            b = by_name.get(nm)
            if not b or len(b) != 1: continue
            b, std = next(iter(b))
            if nm in pops and pops[nm] != b: disagree.append('%s: import_pops.json %d, library %d' % (nm, pops[nm], b)); continue
            sb[nm] = b
            # a __stdcall one's argument count is its bytes / 4 (every Win32 argument is a dword); checked against
            # import_nargs.json (IDA's SDK prototypes) where that lists it
            if std and b % 4 == 0:
                if nargs.get(nm) is not None and nargs[nm] != b // 4: disagree.append('%s: import_nargs.json %d, library %d bytes' % (nm, nargs[nm], b)); continue
                na[nm] = b // 4
    res = {'_source': 'short import members of the import libraries in %s (Visual Studio 6 VC98/LIB)' % os.path.basename(os.path.normpath(libdir)),
           'ordinals': dict(sorted(ords.items())), 'stack_bytes': dict(sorted(sb.items())),
           'stdcall_args': dict(sorted(na.items())),
           '_unnamed': unnamed, '_disagree': disagree, '_no_library': nolib}
    json.dump(res, open(out, 'w'), indent=0)
    print('%d ordinal imports named; %d imports with stack bytes (%d not in import_pops.json); unnamed %s; '
          'disagree %s; no library %s' % (len(ords), len(sb), len(set(sb) - set(pops)), unnamed, disagree, nolib))


if __name__ == '__main__':
    main(*sys.argv[1:])
