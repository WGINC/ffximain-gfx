"""cpp_perm.py -- cpp_permuter (github.com/CriminalRETeam/cpp_permuter, MIT; a C++ permuter built for MSVC 6 matching,
not in this repo: CPP_PERMUTER points at its binary) with the game's compiler and the game's bytes as the target.

cpp_permuter parses one C++ function, applies its passes (statement order, declarations in and out of blocks, if/else
arms, getters, chained assignment, the `sub` vs `add -N` quirk, integer types, ...), compiles each candidate and scores
it. Here:
  * compile: the candidate with Visual C++ 6.0 under Wine, as match.py compiles (`/O2 /Gy`; VC6, setup_vc6.sh);
  * score:   the candidate's function against the game's bytes over its extent (data/func_extents.json), counted as
             mdiff.py counts (differing instruction lines, call targets and relocated operands blanked); 0 only where
             match.py's comparison (every byte outside a relocation, each relocation's target) says IDENTICAL, so
             cpp_permuter stops only on a proven source. The address is the source's `// FUNCTION: FFXIMAIN` mark.
What a match shows is that this source gives the game's bytes, nothing about the original source.

    FFXI_DLL=... VC6=... CPP_PERMUTER=.../cpp_permuter python3 pipeline/vc6/cpp_perm.py run SRC OUTDIR [cpp_permuter options]
    python3 pipeline/vc6/cpp_perm.py compile SRC OBJ          (the -c command)
    python3 pipeline/vc6/cpp_perm.py score OBJ SRC            (the --score-cmd command)
SRC is a self-contained source with one FUNCTION mark (recompile/ form). `run` names the function from the line after
the mark and passes `-s SRC -f NAME -c ... --score-cmd ...`; a match is written by cpp_permuter under OUTDIR.
"""
import difflib, json, os, re, shlex, subprocess, sys
P = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.join(P, '..', '..'); sys.path.insert(0, P)
FUNC = re.compile(r'^//\s*FUNCTION:\s*FFXIMAIN\s+(0x[0-9A-Fa-f]+)', re.M)


def compile_(src, obj):
    import match as M
    rc, out = M.cl(os.path.basename(src), os.path.abspath(obj), ['/O2'], os.path.dirname(os.path.abspath(src)))
    if rc: print(out[-2000:]); sys.exit(1)


def score(obj, src):
    import capstone, pefile
    import match as M, differs as D
    ea = int(FUNC.search(open(src, errors='replace').read()).group(1), 16)
    img = pefile.PE(os.environ['FFXI_DLL'], fast_load=True).get_memory_mapped_image()
    ext = json.load(open(os.path.join(ROOT, 'data', 'func_extents.json')))
    e = ext.get('0x%08X' % ea) or ext.get('0x%08x' % ea); s, t = e[0]
    g = D.insns(img[s - M.BASE:t - M.BASE], s)
    funcs, _ = M.coff(obj)
    def rinsns(code, rels):                    # mdiff.py's rendering of our object
        offs = {o_ for o_, _, _ in rels}; out = []
        for i in capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32).disasm(bytes(code), 0):
            op = i.op_str
            if i.mnemonic.startswith(('call', 'j', 'loop')): op = '<t>'
            elif any(i.address <= o_ < i.address + i.size for o_ in offs):
                op = re.sub(r'(?<![\w+*-])(0x[0-9a-f]+|\d+)(?=\]|,|$)', '<a>', op, count=1) if '[' in op or i.mnemonic == 'push' or ',' in op else '<a>'
                op = re.sub(r'0x1[0-9a-f]{7}', '<a>', op)
            else: op = re.sub(r'0x1[0-9a-f]{7}', '<a>', op)
            out.append((i.mnemonic, op))
        while out and out[-1][0] in ('int3', 'nop'): out.pop()
        return out
    best = None
    for nm, (code, rels) in funcs.items():
        c = rinsns(code, rels); r = difflib.SequenceMatcher(None, g, c).ratio()
        if best is None or r > best[0]: best = (r, nm, c, code, rels)
    _, nm, c, code, rels = best
    diff = difflib.unified_diff(['%s %s' % x for x in g], ['%s %s' % x for x in c], lineterm='', n=0)
    nd = sum(1 for l in diff if l[:1] in '+-' and l[:3] not in ('---', '+++'))
    if nd == 0:
        v, why, _ = M.compare(img, ext, ea, code, rels, {})
        if v != 'IDENTICAL': nd = 1; print('same instructions, but match.py: %s %s' % (v, why))
    print(nd)


def run(src, out, rest):
    t = open(src, errors='replace').read(); m = FUNC.search(t)
    sig = t[m.end():].lstrip('\n').split('\n', 1)[0]
    name = re.search(r'([A-Za-z_][\w:~]*)\s*\(', re.sub(r'\b(extern\s+"C"|__\w+)\b', '', sig)).group(1)
    me = shlex.quote(os.path.abspath(__file__))
    cmd = [os.environ['CPP_PERMUTER'], '-s', src, '-f', name,
           '-c', '%s %s compile {src} {obj}' % (shlex.quote(sys.executable), me),
           '--score-cmd', '%s %s score {obj} {src}' % (shlex.quote(sys.executable), me), '-o', out] + rest
    print(' '.join(shlex.quote(c) for c in cmd), flush=True)
    sys.exit(subprocess.call(cmd))


if __name__ == '__main__':
    a = sys.argv[1:]
    if a[:1] == ['compile']: compile_(a[1], a[2])
    elif a[:1] == ['score']: score(a[1], a[2])
    elif a[:1] == ['run']: run(a[1], a[2], a[3:])
    else: sys.exit(__doc__)
