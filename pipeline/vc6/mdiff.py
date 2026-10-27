"""mdiff.py -- what is left between a hand-written source and the game's bytes: per function, a unified diff of the two
instruction streams.

Compiles SRC with the game's compiler (match.py's cl; $VC6) and disassembles each function the source marks
(`// FUNCTION: FFXIMAIN 0x...`, or the address in a sweep file's name f_<ADDR>.cpp) against the game's code over its
extent (data/func_extents.json). Call and jump targets are blanked, and so are operands that hold an address: the
game's absolute addresses and our relocated operands (match.py compares those by target; this shows only the rest).
The object function compared is the one closest to the game's (difflib ratio), so a source may hold helpers.

    FFXI_DLL=... VC6=... python3 pipeline/vc6/mdiff.py SRC [--flags "/O2"] [-q]
-q prints only the count of differing lines per function."""
import argparse, difflib, json, os, re, sys, tempfile
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import match as M, differs as D, pefile
ap = argparse.ArgumentParser(); ap.add_argument('src'); ap.add_argument('--flags', default='/O2'); ap.add_argument('-q', action='store_true')
o = ap.parse_args()
d = tempfile.mkdtemp(prefix='md_'); src = os.path.abspath(o.src)
rc, out = M.cl('Z:' + src.replace('/', '\\'), os.path.join(d, 'o.obj'), o.flags.split(), d)
if rc: print(out[-1500:]); sys.exit(1)
objs, _ = M.coff(os.path.join(d, 'o.obj'))
pe = pefile.PE(os.environ['FFXI_DLL'], fast_load=True); img = pe.get_memory_mapped_image()
ext = json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'data', 'func_extents.json')))
text = open(src).read(); addrs = [int(a, 16) for a in M.FUNC_RE.findall(text)]
if not addrs:
    m = re.search(r'f_([0-9A-Fa-f]{8})', os.path.basename(src)); addrs = [int(m.group(1), 16)] if m else []
names = list(objs)
import capstone, re as _re
def rinsns(code, rels):
    offs = {o_ for o_, _, _ in rels}
    out = []
    for i in capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32).disasm(bytes(code), 0):
        op = i.op_str
        if i.mnemonic.startswith(('call', 'j', 'loop')): op = '<t>'
        elif any(i.address <= o_ < i.address + i.size for o_ in offs):    # the relocated number (the addend) as <a>
            op = _re.sub(r'(?<![\w+*-])(0x[0-9a-f]+|\d+)(?=\]|,|$)', '<a>', op, count=1) if '[' in op or i.mnemonic == 'push' or ',' in op else '<a>'
            op = _re.sub(r'0x1[0-9a-f]{7}', '<a>', op)
        else: op = _re.sub(r'0x1[0-9a-f]{7}', '<a>', op)
        out.append((i.mnemonic, op))
    return out
for ea in addrs:
    e = ext.get('0x%08X' % ea) or ext.get('0x%08x' % ea); s, t = e[0]
    g = D.insns(img[s - M.BASE:t - M.BASE], s)
    best = None
    for nm in names:
        c = rinsns(objs[nm][0], objs[nm][1])
        r = difflib.SequenceMatcher(None, g, c).ratio()
        if best is None or r > best[0]: best = (r, nm, c)
    r, nm, c = best
    while c and c[-1][0] in ('int3', 'nop'): c.pop()
    diff = [l for l in difflib.unified_diff(['%s %s' % x for x in g], ['%s %s' % x for x in c], 'game', 'ours', lineterm='', n=2)]
    nd = sum(1 for l in diff if l[:1] in '+-' and l[:3] not in ('---', '+++'))
    print('0x%08X %s: %d lines differ (%d game, %d ours)' % (ea, nm, nd, len(g), len(c)))
    if not o.q: print('\n'.join(diff[:80]))
