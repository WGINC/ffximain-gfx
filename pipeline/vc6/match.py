"""match.py -- compile C/C++ with the game's own compiler and compare each function's bytes with FFXiMain.dll's.

The compiler is Visual C++ 6.0 SP6 with the Processor Pack's code generator, assembled by setup_vc6.sh in $VC6 (never
in the repository); it runs under Wine. Each function in the source is preceded by its address, as in tree/:

    // FUNCTION: FFXIMAIN 0x100279B0

The source is compiled to a COFF object (cl /c, the flags given; default /O2), and each function's code is read from
it: its COMDAT section (cl /Gy, added: one section per function; it changes no instruction) and that section's
relocations. Against the game's bytes at the address:
  * every byte not covered by a relocation must be equal;
  * a REL32 relocation (a call or jump to another symbol) is compared by its target: the game's target is read from
    its own bytes; when the symbol's name carries an address (FUN_/sub_/fn_ + 8 hex digits, or a name the tree's
    placement gives an address) the two must be the same, otherwise the target is reported, not compared;
  * a DIR32 relocation (an absolute address: a global, a jump table, a constant) is likewise compared when the
    symbol names an address (DAT_/g_ ... _XXXXXXXX), else reported;
  * the length must be the game function's (data/func_extents.json, a single piece), up to alignment padding (0xCC /
    0x90) after the code on either side (a /Gy section is padded to its alignment).
Verdict per function: IDENTICAL (every byte and every comparable target equal; uncompared targets are listed),
DIFFERS (the first differing offset, both instruction streams there), or MISSING (no code for it in the object).

    FFXI_DLL=... VC6=... python3 pipeline/vc6/match.py SRC.cpp [--flags "/O2 ..."] [--keep DIR]
    VC6=... python3 pipeline/vc6/match.py --compid       the @comp.id this toolchain stamps (C and C++)
    FFXI_DLL=... VC6=... python3 pipeline/vc6/match.py --all   every recompile/*.cpp; exits 1 unless all IDENTICAL
"""
import argparse, json, os, re, shutil, struct, subprocess, sys, tempfile
P = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.join(P, '..', '..'); BASE = 0x10000000
FUNC_RE = re.compile(r'//\s*FUNCTION:\s*FFXIMAIN\s+(0x[0-9A-Fa-f]+)')
ADDR_RE = re.compile(r'(?:FUN|sub|fn|DAT|LAB|g)_?[A-Za-z0-9]*?_?(1[0-9A-Fa-f]{7})\b')


def cl(src, obj, flags, cwd):
    vc6 = os.environ['VC6']
    env = dict(os.environ, WINEDEBUG='-all', INCLUDE='Z:' + os.path.join(vc6, 'INCLUDE').replace('/', '\\'))
    r = subprocess.run(['wine', os.path.join(vc6, os.environ.get('VC6_BIN', 'BIN'), 'cl.exe'), '/nologo', '/c', '/Gy'] + flags + ['/Fo' + obj, src],
                       cwd=cwd, env=env, capture_output=True, text=True, errors='replace')
    return r.returncode, (r.stdout + r.stderr).strip()


def coff(path):
    """{symbol name: (bytes, [(offset, type, target symbol)])} for the code symbols, and the @comp.id"""
    b = open(path, 'rb').read()
    _, nsec, _, symptr, nsym, opt, _ = struct.unpack_from('<HHIIIHH', b, 0)
    secs = [struct.unpack_from('<8sIIIIIIHHI', b, 20 + opt + 40 * i) for i in range(nsec)]
    strtab = symptr + 18 * nsym
    def name(raw):
        if raw[:4] == b'\0\0\0\0':
            o = strtab + struct.unpack_from('<I', raw, 4)[0]; return b[o:b.index(b'\0', o)].decode()
        return raw.rstrip(b'\0').decode()
    syms, i, compid = [], 0, None
    while i < nsym:
        raw = b[symptr + 18 * i:symptr + 18 * i + 18]
        nm = name(raw[:8]); val, sec, typ, cls, naux = struct.unpack_from('<IhHBB', raw, 8)
        syms.append((nm, val, sec, typ, cls))
        if nm == '@comp.id': compid = val
        for _ in range(naux): syms.append(None)
        i += 1 + naux
    out = {}
    for nm_, val, sec, typ, cls in [s for s in syms if s]:
        if sec <= 0 or (typ >> 4) != 2 or cls not in (2, 3): continue          # a function, defined here
        s = secs[sec - 1]; size, rawptr, relptr, nrel = s[3], s[4], s[5], s[7]
        code = b[rawptr:rawptr + size]
        rels = []
        for k in range(nrel):
            off, si, rt = struct.unpack_from('<IIH', b, relptr + 10 * k)
            t = syms[si]
            rels.append((off - val, rt, t[0] if t else '?'))
        out[nm_] = (code[val:], rels)
    return out, compid


def listing_lines(cod):
    """{decorated function name: first source line} from cl's /FAc listing"""
    res, cur = {}, None
    for l in open(cod, errors='replace'):
        m = re.match(r'^(\S+)\s+PROC NEAR', l)
        if m: cur = m.group(1); continue
        m = re.match(r'^; Line (\d+)', l)
        if m and cur and cur not in res: res[cur] = int(m.group(1))
        if re.match(r'^\S+\s+ENDP', l): cur = None
    return res


def compare(img, ext, ea, code, rels, place):
    e = ext.get('0x%08X' % ea) or ext.get('0x%08x' % ea)
    n = e[0][1] - e[0][0] if e and len(e) == 1 else None
    # a COMDAT section is padded to its alignment: bytes past the game function's end that are padding are not code
    if n is not None and len(code) > n and all(x in (0xCC, 0x90) for x in code[n:]) and not any(o_ + 4 > n for o_, _, _ in rels):
        code = code[:n]
    orig = img[ea - BASE:ea - BASE + len(code)]
    covered = set(); notes = []
    for off, rt, sym in rels:
        covered.update(range(off, off + 4))
        a = None
        m = ADDR_RE.search(sym)
        if m: a = int(m.group(1), 16)
        elif sym.lstrip('_?') in place: a = place[sym.lstrip('_?')]
        if rt == 0x14:                       # IMAGE_REL_I386_REL32
            got = (ea + off + 4 + struct.unpack_from('<i', orig, off)[0]) & 0xFFFFFFFF
        elif rt == 0x06:                     # IMAGE_REL_I386_DIR32
            got = struct.unpack_from('<I', orig, off)[0] - struct.unpack_from('<I', code, off)[0]   # minus the addend
            got &= 0xFFFFFFFF
        else:
            notes.append('relocation type %d at +%d (%s): not compared' % (rt, off, sym)); continue
        if a is None: notes.append('+%d %s -> 0x%08X in the game: not compared (%s carries no address)' % (off, 'call/jump' if rt == 0x14 else 'address', got, sym))
        elif a != got: return 'DIFFERS', '+%d: %s targets %s (0x%08X), the game 0x%08X' % (off, 'REL32' if rt == 0x14 else 'DIR32', sym, a, got), notes
    for k in range(len(code)):
        if k not in covered and code[k] != orig[k]:
            return 'DIFFERS', '+%d: 0x%02X here, 0x%02X in the game' % (k, code[k], orig[k]), notes
    if n is not None:
        tail = img[ea - BASE + len(code):ea - BASE + n]
        if len(code) > n or any(x not in (0xCC, 0x90) for x in tail):
            return 'DIFFERS', 'length %d, the game function %d' % (len(code), n), notes
    return 'IDENTICAL', '', notes


def compid():
    d = tempfile.mkdtemp(prefix='vc6_')
    open(os.path.join(d, 't.c'), 'w').write('int f(int a){return a+1;}\n')
    open(os.path.join(d, 't.cpp'), 'w').write('struct A{int x;int g();};int A::g(){return x;}\n')
    for s in ('t.c', 't.cpp'):
        rc, out = cl(s, s + '.obj', ['/O2'], d)
        _, c = coff(os.path.join(d, s + '.obj')) if rc == 0 else ({}, None)
        print('%s: %s' % (s, 'prodid %d build %d' % (c >> 16, c & 0xFFFF) if c is not None else 'compile failed: ' + out[-300:]))
    shutil.rmtree(d)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('src', nargs='?'); ap.add_argument('--flags', default='/O2'); ap.add_argument('--keep')
    ap.add_argument('--compid', action='store_true'); ap.add_argument('--json'); ap.add_argument('--all', action='store_true')
    o = ap.parse_args()
    if o.compid: return compid()
    if o.all:
        import glob
        bad = 0
        for f in sorted(glob.glob(os.path.join(ROOT, 'recompile', '*.cpp'))):
            r = subprocess.run([sys.executable, os.path.abspath(__file__), f, '--flags', o.flags], capture_output=True, text=True)
            print(os.path.basename(f) + ': ' + (r.stdout.splitlines() or ['(no output) ' + r.stderr[-300:]])[0])
            bad += r.returncode != 0 or 'DIFFERS' in r.stdout or 'MISSING' in r.stdout
        sys.exit(1 if bad else 0)
    import pefile
    img = pefile.PE(os.environ['FFXI_DLL'], fast_load=True).get_memory_mapped_image()
    ext = json.load(open(os.path.join(ROOT, 'data', 'func_extents.json')))
    pl = os.path.join(ROOT, 'tree', 'placement.json')
    place = {}
    if os.path.exists(pl):
        for k, v in json.load(open(pl)).items():
            if isinstance(v, dict) and v.get('symbol'): place[v['symbol']] = int(k, 16)
    d = o.keep or tempfile.mkdtemp(prefix='vc6_'); os.makedirs(d, exist_ok=True)
    src = os.path.join(d, os.path.basename(o.src)); shutil.copy(o.src, src)
    rc, out = cl(os.path.basename(src), 'm.obj', o.flags.split() + ['/FAc', '/Fam.cod'], d)
    if rc: print('compile failed:\n' + out); sys.exit(2)
    funcs, c = coff(os.path.join(d, 'm.obj'))
    lines = listing_lines(os.path.join(d, 'm.cod'))
    marks = [(n + 1, int(m.group(1), 16)) for n, l in enumerate(open(src, errors='replace')) for m in [FUNC_RE.search(l)] if m]
    res = {}
    for ln, ea in marks:
        nxt = min([x for x, _ in marks if x > ln], default=10 ** 9)
        sym = next((s for s, first in sorted(lines.items(), key=lambda kv: kv[1]) if ln < first < nxt and s in funcs), None)
        if sym is None: res['0x%08X' % ea] = ('MISSING', 'no compiled function between line %d and the next mark' % ln, []); continue
        v, why, notes = compare(img, ext, ea, *funcs[sym], place)
        res['0x%08X' % ea] = (v, sym + (': ' + why if why else ''), notes)
    n = sum(v[0] == 'IDENTICAL' for v in res.values())
    print('@comp.id prodid %d build %d; %d of %d functions identical' % (c >> 16, c & 0xFFFF, n, len(res)) if c is not None else '%d of %d identical' % (n, len(res)))
    for ea, (v, why, notes) in sorted(res.items()):
        print('%s %-9s %s' % (ea, v, why))
        for x in notes: print('    ' + x)
    if o.json: json.dump({k: {'verdict': v[0], 'why': v[1], 'notes': v[2]} for k, v in res.items()}, open(o.json, 'w'), indent=1)
    if not o.keep: shutil.rmtree(d)


if __name__ == '__main__':
    main()
