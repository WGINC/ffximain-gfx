#!/usr/bin/env python3
"""make_pdb.py -- FFXiMain.pdb: the reconstruction's names and types as a debugger's symbol file.

    FFXI_DLL=/path/FFXiMain.dll python3 sdk/pdb/make_pdb.py OUT_DIR [kit/xikit/builds/<stamp>.json]
                                                           (needs clang, lld-link, llvm-pdbutil; the json for a build
                                                           other than the analysed one, with FFXI_DLL that build's DLL)

Writes OUT_DIR/FFXiMain.pdb and OUT_DIR/FFXiMain.pdb.json (every symbol with the source of its name). Nothing of
the game's code or data is in either: the PDB holds names, addresses, sizes and the tree's type declarations.

How it is made (no PDB is written by hand):
  1. layout.s: an assembly file with the game's three sections that hold symbols (.text, .rdata, .data), each exactly
     the size the game's section table gives, empty, with one label at each function's and each named global's
     offset; and a CodeView symbol subsection (.debug$S) with an S_GPROC32 record per function (its size from
     data/func_extents.json) and an S_GDATA32 record per global.
  2. types.cpp: includes every header of tree/include; compiled with CodeView debug info and
     -fno-eliminate-unused-debug-types, so every class of the reconstruction (fields at their offsets, which the
     headers static_assert) is in the PDB's type stream.
  3. lld-link links both at the game's image base and section alignment into a throwaway DLL and writes the PDB.
  4. Checked: the throwaway DLL's section table equals the game's for those sections (address and size), and every
     symbol read back from the PDB (llvm-pdbutil) is at the address it was given.

Names, in this order (the source is recorded per symbol):
  tree      the kit's manifest (kit/xikit/dist/xikit_manifest.txt): a function in the tree, named as the tree names it
  facts     ghidra/ffxi_facts.json "names"
  evidence  data/evidence_names.json (a vtable slot)
  dm        data/dm_names.json entries marked true
  none      sub_<address>: a function start from data/func_extents.json with no name
  globals   tree/include/gfx_globals.hpp (#define NAME XI_EA(type, address)), those inside the image's data sections

The game's DLL carries no usable CodeView record (its debug directory entry is type 2 with 80 bytes that are not an
NB10 or RSDS record), so no debugger can match this PDB to it by signature: it has to be loaded by hand
(sdk/pdb/README.md says how in each debugger, and what is not known about Visual Studio).
"""
import json, os, re, struct, subprocess, sys

H = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.join(H, '..', '..')
BASE = 0x10000000


def sections(dll):
    d = open(dll, 'rb').read(); pe = struct.unpack_from('<I', d, 0x3C)[0]
    n = struct.unpack_from('<H', d, pe + 6)[0]; opt = struct.unpack_from('<H', d, pe + 20)[0]
    assert struct.unpack_from('<I', d, pe + 24 + 28)[0] == BASE, 'image base'
    assert struct.unpack_from('<I', d, pe + 24 + 32)[0] == 0x1000, 'section alignment'
    out = []
    for i in range(n):
        o = pe + 24 + opt + 40 * i
        out.append((d[o:o + 8].rstrip(b'\0').decode(), struct.unpack_from('<I', d, o + 12)[0], struct.unpack_from('<I', d, o + 8)[0]))
    return out, struct.unpack_from('<I', d, pe + 8)[0]


def names():
    src = {}
    for l in open(os.path.join(ROOT, 'kit/xikit/dist/xikit_manifest.txt')):
        if l[0] in '#!' or not l.strip(): continue
        p = l.split()
        src[BASE + int(p[0], 16)] = (p[5], 'tree')
    f = json.load(open(os.path.join(ROOT, 'ghidra/ffxi_facts.json')))['names']
    for a, n in f.items(): src.setdefault(int(a, 16), (n, 'facts'))
    for a, e in json.load(open(os.path.join(ROOT, 'data/evidence_names.json'))).items():
        if a.startswith('0x') and isinstance(e, dict) and e.get('name'): src.setdefault(int(a, 16), (e['name'], 'evidence'))
    for a, e in json.load(open(os.path.join(ROOT, 'data/dm_names.json'))).items():
        if isinstance(e, list) and len(e) >= 3 and e[2] is True: src.setdefault(int(a, 16), (e[0], 'dm'))
    return src


def main():
    out = os.path.abspath(sys.argv[1]); os.makedirs(out, exist_ok=True)
    secs, stamp = sections(os.environ['FFXI_DLL'])
    want = [s for s in secs if s[0] in ('.text', '.rdata', '.data')]
    assert [s[0] for s in want] == ['.text', '.rdata', '.data'] and [s[0] for s in secs[:3]] == ['.text', '.rdata', '.data']
    for (n, va, sz), (n2, va2, _) in zip(want, want[1:]): assert (va + sz + 0xFFF) & ~0xFFF == va2, 'sections not contiguous'
    def sec_of(a):
        for i, (n, va, sz) in enumerate(want):
            if BASE + va <= a < BASE + va + sz: return i
        return None
    nm = names(); ext = json.load(open(os.path.join(ROOT, 'data/func_extents.json')))
    syms, used = [], {}
    def label(name, a):
        name = re.sub(r'[^A-Za-z0-9_:~<>,*& @$?.]', '_', name)
        if name in used: name = '%s@%08X' % (name, a)
        used[name] = a; return name
    for a_, rs in sorted(ext.items(), key=lambda kv: int(kv[0], 16)):
        a = int(a_, 16)
        if sec_of(a) != 0: continue
        size = max(1, rs[0][1] - rs[0][0]) if rs and rs[0][0] == a else 1
        n, s = nm.get(a, ('sub_%08X' % a, 'none'))
        syms.append({'addr': a, 'size': size, 'name': label(n, a), 'kind': 'function', 'source': s})
    fstarts = {s['addr'] for s in syms}
    for a, (n, s) in sorted(nm.items()):          # named code that is not a function start of func_extents: a public only
        if a not in fstarts and sec_of(a) == 0: syms.append({'addr': a, 'size': 0, 'name': label(n, a), 'kind': 'label', 'source': s})
    g = re.findall(r'#define (\w+) XI_EA\((\w+), 0x([0-9A-Fa-f]+)u\)', open(os.path.join(ROOT, 'tree/include/gfx_globals.hpp')).read())
    seen = {s['addr'] for s in syms}
    for n, _, a_ in g:
        a = int(a_, 16)
        if sec_of(a) in (1, 2) and a not in seen:
            seen.add(a); syms.append({'addr': a, 'size': 0, 'name': label(n, a), 'kind': 'data', 'source': 'globals'})
    # ---- another build of the game (kit/xikit/builds/<stamp>.json, docs/KIT.md "build translation"): each symbol
    # moves by its range's shift; a symbol in no range of that file is left out (where it is in that build is not
    # known); a function the file lists as not shown the same keeps its name with size 1 (its length there is not known)
    if len(sys.argv) > 2:
        b = json.load(open(sys.argv[2])); assert int(b['timestamp'], 16) == stamp, 'the build file is for %s, FFXI_DLL is %08X' % (b['timestamp'], stamp)
        code = [(int(lo, 16), int(hi, 16), d) for lo, hi, d in b['ranges']]; data = [(int(lo, 16), int(hi, 16), d) for lo, hi, d in b.get('data_ranges', [])]   # (a build file without data ranges: no global is carried over)
        diff = {int(r.split('-')[0], 16) for r in b.get('functions_not_shown_same', [])}
        moved, left = [], 0
        for s_ in syms:
            r = next(((lo, hi, d) for lo, hi, d in (data if s_['kind'] == 'data' else code) if lo <= s_['addr'] < hi), None)
            if r is None: left += 1; continue
            if s_['addr'] in diff: s_['size'] = 1; s_['source'] += ' (not shown the same in this build)'
            s_['analysed'] = s_['addr']; s_['addr'] += r[2]; moved.append(s_)
        print('build %s: %d symbols carried over, %d left out (in no range of %s)' % (b['timestamp'], len(moved), left, os.path.basename(sys.argv[2])))
        syms = moved
        for s_ in syms: assert sec_of(s_['addr']) is not None, 'moved symbol outside the sections: %r' % s_
    syms.sort(key=lambda s: s['addr'])

    # ---- layout.s
    L = ['# generated by sdk/pdb/make_pdb.py: the game\'s section layout with labels; no code, no data']
    flags = {'.text': '"xr"', '.rdata': '"dr"', '.data': '"dw"'}
    for i, (n, va, sz) in enumerate(want):
        L.append('\t.section %s,%s' % (n, flags[n]))
        full = sz if i == len(want) - 1 else ((sz + 0xFFF) & ~0xFFF)      # the last one: its own size
        pos = 0
        for s in syms:
            if sec_of(s['addr']) != i: continue
            off = s['addr'] - BASE - va
            if off > pos: L.append('\t.fill %d, 1, 0' % (off - pos)); pos = off
            L.append('\t.globl "_%s"\n"_%s":' % (s['name'], s['name']))      # "_": a COFF label starting with L would be local
        if sz > pos: L.append('\t.fill %d, 1, 0' % (sz - pos))
    L.append('\t.section .debug$S,"dr"\n\t.p2align 2\n\t.long 4\n\t.long 0xF1\n\t.long Lsym_end-Lsym_begin\nLsym_begin:')
    for k, s in enumerate(syms):
        nb = s['name'].encode() + b'\0'
        if s['kind'] == 'function':
            body = 2 + 12 + 4 + 8 + 4 + 4 + 2 + 1 + len(nb); pad = (-(body + 2)) % 4
            L.append('\t.short %d\n\t.short 0x1110\n\t.long 0\n\t.long 0\n\t.long 0\n\t.long %d\n\t.long 0\n\t.long %d\n\t.long 0' % (body + pad, s['size'], s['size']))
            L.append('\t.secrel32 "_%s"\n\t.secidx "_%s"\n\t.byte 0\n\t.asciz "%s"' % (s['name'], s['name'], s['name']))
            if pad: L.append('\t.fill %d, 1, 0' % pad)
            L.append('\t.short 2\n\t.short 0x6')
        elif s['kind'] == 'data':
            body = 2 + 4 + 4 + 2 + len(nb); pad = (-(body + 2)) % 4
            L.append('\t.short %d\n\t.short 0x110d\n\t.long 0\n\t.secrel32 "_%s"\n\t.secidx "_%s"\n\t.asciz "%s"' % (body + pad, s['name'], s['name'], s['name']))
            if pad: L.append('\t.fill %d, 1, 0' % pad)
    L.append('Lsym_end:\n\t.p2align 2')
    open(os.path.join(out, 'layout.s'), 'w').write('\n'.join(L) + '\n')

    # ---- types.cpp
    inc = os.path.join(ROOT, 'tree', 'include')
    hs = sorted(f for f in os.listdir(inc) if f.endswith(('.hpp', '.h')))
    open(os.path.join(out, 'types.cpp'), 'w').write('// generated by sdk/pdb/make_pdb.py: every tree header, for its types\n' +
                                                    ''.join('#include "%s"\n' % h for h in hs))
    run = lambda c: subprocess.run(c, check=True, cwd=out)
    run(['clang', '--target=i686-pc-windows-msvc', '-c', 'layout.s', '-o', 'layout.obj'])
    tflags = [l.strip() for l in open(os.path.join(H, 'types_flags.txt')) if l.strip() and not l.startswith('#')]
    run(['clang++', '--target=i686-w64-mingw32', '-c', 'types.cpp', '-o', 'types.obj', '-I', inc, '-g', '-gcodeview',
         '-fno-eliminate-unused-debug-types', '-w'] + tflags)
    run(['lld-link', '/dll', '/noentry', '/nodefaultlib', '/machine:x86', '/base:0x%X' % BASE, '/align:4096', '/debug',
         '/opt:noref', '/opt:noicf', '/safeseh:no', '/force:unresolved', '/pdb:FFXiMain.pdb', '/out:layout_throwaway.dll', 'layout.obj', 'types.obj'])

    # ---- checks
    got, _ = sections(os.path.join(out, 'layout_throwaway.dll'))
    for (n, va, sz) in want:
        m = [g for g in got if g[0] == n]
        assert m and m[0][1] == va, 'section %s is at 0x%X in the throwaway image, 0x%X in the game' % (n, m[0][1] if m else 0, va)
    secva = {i + 1: g[1] for i, g in enumerate(got)}
    dump = subprocess.run(['llvm-pdbutil', 'dump', '--publics', 'FFXiMain.pdb'], cwd=out, capture_output=True, text=True, check=True).stdout
    pub = {}
    for m in re.finditer(r'S_PUB32 \[size = \d+\] `([^`]*)`\s*\n\s*flags = [^,]*, addr = (\d+):(\d+)', dump):
        pub[m.group(1)] = BASE + secva[int(m.group(2))] + int(m.group(3))
    bad = [s for s in syms if pub.get('_' + s['name']) != s['addr']]
    assert not bad, '%d symbols are not at their address in the PDB, first %r (pdb: %r)' % (len(bad), bad[0], pub.get('_' + bad[0]['name']))
    ty = subprocess.run(['llvm-pdbutil', 'dump', '--summary', '--types', 'FFXiMain.pdb'], cwd=out, capture_output=True, text=True, check=True).stdout
    ntypes = len(re.findall(r'LF_(?:STRUCTURE|CLASS|UNION) \[size', ty))
    json.dump({'_about': 'FFXiMain.pdb, made by sdk/pdb/make_pdb.py for the build with TimeDateStamp 0x%08X' % stamp,
               'symbols': [{'address': '0x%08X' % s['addr'], 'name': s['name'], 'kind': s['kind'], 'size': s['size'], 'source': s['source'], **({'analysed_address': '0x%08X' % s['analysed']} if 'analysed' in s else {})} for s in syms]},
              open(os.path.join(out, 'FFXiMain.pdb.json'), 'w'), indent=0)
    keep = os.environ.get('XI_PDB_KEEP') == '1'      # test_pdb.py: the throwaway image names the PDB, so dbghelp loads it
    for f in ('layout.s', 'layout.obj', 'types.obj', 'types.cpp', 'layout_throwaway.lib') + (() if keep else ('layout_throwaway.dll',)):
        if os.path.exists(os.path.join(out, f)): os.remove(os.path.join(out, f))
    import collections
    c = collections.Counter((s['kind'], s['source']) for s in syms)
    print('FFXiMain.pdb for build %08X: %d symbols, every one read back at its address; %d class/struct/union records' % (stamp, len(syms), ntypes))
    for k, v in sorted(c.items()): print('  %6d  %s, name from %s' % (v, k[0], k[1]))


if __name__ == '__main__':
    main()
