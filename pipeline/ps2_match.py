"""ps2_match.py -- PC functions matched to the PS2 build's named functions, each match with the evidence it rests on.

The PS2 executable (PS2_ELF, ps2_elf.py) names every function. The PC DLL (FFXI_DLL) names few. A PC function is
matched to a PS2 one only on evidence both binaries hold, never on likeness of shape:

  * `string`: a string literal (its exact bytes, at least MINLEN of them) that exactly one PS2 function and exactly
    one PC function refer to. Two compilers of the same source keep a literal's bytes; a literal unique to one
    function on both sides ties those functions. Where one function pair shares several such literals, each is
    listed. A pair one of whose functions also has a unique literal pointing at a different partner is a
    conflict, and neither is kept.
  * `strings`: a function's whole set of literals (two or more), held by exactly one function on each side.
  * `floats`: float constants (their 4 or 8 bytes) that exactly one PS2 function and one PC function use, at least
    two of them shared by the pair. PC: an x87/SSE instruction's memory operand into the image, read at the
    operand's size; PS2: a literal-pool load (R_MIPS_LITERAL, $gp-relative) of a `lwc1`/`ldc1`, or a `lui`/`ori`
    value moved to the FPU (ps2_elf.float_immediates). A value is compared as the float it is (a double that is
    a float exactly compares with that float).
  * `calls`: within a matched pair, the direct callees taken in the order the code calls them. Where both
    sides call the same number of distinct functions and the matched ones line up, an unmatched callee that is
    the only one at its position on both sides is matched too. Its evidence names the matched pair it came
    from. This is propagation from a `string` match, a weaker kind, and every such match says so.

The inferred kinds (`floats`, `calls`, `neighbours`) never pair library code: the two C runtimes are different
code (MSVC's on PC, the PS2 SDK's), sharing names only. So a PS2 partner must be a mangled C++ symbol of the
game's own (`name__F...` or `name__<n><Class>...`), and a PC function must be a function of the Ghidra project
(runs/ghidra/names.json) whose name is not a C runtime one (a leading underscore and lower case: `_strncpy`).

A PC function's references are the immediates and absolute memory operands of its instructions
(data/func_extents.json's code) that point at a NUL-terminated string in the image; a PS2 function's are its
HI16/LO16 address pairs (ps2_elf.py). Strings are compared as bytes.

    FFXI_DLL=... PS2_ELF=... python3 ps2_match.py OUT.json
Output {"0xPC": {"ps2": "mangled name", "ps2_addr": "0x...", "kind": "string" | "calls", "evidence": [...]}},
and the conflicts under "_conflicts"."""
import collections, json, os, re, sys
from capstone.x86 import X86_OP_IMM, X86_OP_MEM
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import sp_solve as S
import ps2_elf
D = os.path.join(P, '..', 'data')
MINLEN = 6
PRINT = re.compile(rb'^[\x09\x0a\x0d\x20-\x7e\x80-\xfe]+$')


OPS = {'__ct': 'Constructor', '__dt': 'Destructor', '__as': 'operator_assign', '__eq': 'operator_eq',
       '__ne': 'operator_ne', '__vc': 'operator_index', '__cl': 'operator_call', '__nw': 'operator_new',
       '__dl': 'operator_delete', '__pl': 'operator_plus', '__mi': 'operator_minus', '__ml': 'operator_mul'}


def demangle(m):
    """(class or None, method) from a CodeWarrior (cfront-style) name: `Method__<n>Class...`, `name__Q<k><n>A<n>B...`,
    `name__F...` (a free function), `__ct__<n>Class...`; None when the name is not mangled"""
    k = m.find('__', 1 if not m.startswith('__') else 2)
    while k > 0 and not re.match(r'__(\d|Q\d|F|C)', m[k:]): k = m.find('__', k + 1)
    if k <= 0: return None
    name, rest = m[:k], m[k + 2:]
    name = OPS.get(name, name)
    if rest.startswith('F') or rest.startswith('CF'): return None, name
    parts = []
    mq = re.match(r'Q(\d)', rest)
    count = int(mq.group(1)) if mq else 1
    rest = rest[2:] if mq else rest
    for _ in range(count):
        mm = re.match(r'(\d+)', rest)
        if not mm: return None
        n = int(mm.group(1)); parts.append(rest[len(mm.group(1)):len(mm.group(1)) + n]); rest = rest[len(mm.group(1)) + n:]
    return '::'.join(parts), name


CONF = {'string': 'confirmed', 'strings': 'confirmed', 'floats': 'candidate', 'calls': 'candidate', 'neighbours': 'candidate'}


def show(b):
    """a literal as text: Shift-JIS (cp932, the game's own encoding) where its bytes are valid, else Latin-1"""
    try: return b.decode('cp932')
    except UnicodeDecodeError: return b.decode('latin-1')


def text_ok(s):
    return s is not None and len(s) >= MINLEN and PRINT.match(s) is not None


FLOAT_OPS = re.compile(r'^(f(?!n?st[cs]w|n?stenv|ldenv|n?save|rstor|xch|n?init|n?clex|wait|free|ld[1zlpn]|ldl2|decstp|incstp)\w*|'
                       r'(mov|add|sub|mul|div|min|max|comi|ucomi|cvt\w*|sqrt)s[sd]\w*)$')


def as_float(raw):
    """a constant's value, as a float where it is one exactly (a double holding a float's value compares equal)"""
    import struct as st
    if len(raw) == 4: return st.unpack('<f', raw)[0]
    if len(raw) == 8:
        d = st.unpack('<d', raw)[0]
        try: return st.unpack('<f', st.pack('<f', d))[0] if st.unpack('<f', st.pack('<f', d))[0] == d else d
        except OverflowError: return d
    return None


def pc_side(im, ext):
    """({func: set(strings)}, {func: [direct callees in address order]}, {func: set(float constants)})"""
    lo, hi = im.base, im.base + len(im.img)
    def cstr(a):
        if not lo <= a < hi: return None
        o = a - lo; e = im.img.find(b'\0', o, o + 512)
        return im.img[o:e] if e >= 0 else None
    strs, calls, flts = {}, {}, {}
    for k, e in ext.items():
        f = int(k, 16); ins = im.code(e); ss = set(); cs = []; fl = set()
        for a in sorted(ins):
            i = ins[a]
            for o in i.operands:
                v = o.imm & 0xFFFFFFFF if o.type == X86_OP_IMM else \
                    o.mem.disp & 0xFFFFFFFF if o.type == X86_OP_MEM and not o.mem.base and not o.mem.index else None
                if v is None: continue
                if i.mnemonic == 'call' and o.type == X86_OP_IMM: cs.append(v); continue
                if o.type == X86_OP_MEM and FLOAT_OPS.match(i.mnemonic) and o.size in (4, 8) and lo <= v <= hi - o.size:
                    x = as_float(bytes(im.img[v - lo:v - lo + o.size]))
                    if x is not None and x == x: fl.add(x)
                    continue
                s = cstr(v)
                if text_ok(s): ss.add(s)
        strs[f] = ss; calls[f] = cs; flts[f] = fl
    return strs, calls, flts


def ps2_side(elf):
    fs = elf.functions(); starts = sorted(fs)
    import bisect
    owner = lambda a: (lambda j: starts[j] if j >= 0 and a < fs[starts[j]][1] else None)(bisect.bisect_right(starts, a) - 1)
    strs, calls, flts = collections.defaultdict(set), collections.defaultdict(list), collections.defaultdict(set)
    for f, (nm, end) in fs.items():
        for at, bits in elf.float_immediates(f, end): flts[f].add(as_float(bits.to_bytes(4, 'little')))
    for at, kind, t in sorted(elf.refs()):
        f = owner(at)
        if f is None: continue
        if kind == 'lit' and elf.inside(t):
            w = elf.u32(at) >> 26                                   # lwc1 0x31, ldc1 0x35 (MIPS); others skipped
            n = 4 if w == 0x31 else 8 if w == 0x35 else 0
            if n and elf.inside(t + n - 1):
                x = as_float(elf.img[t - elf.base:t - elf.base + n])
                if x is not None and x == x: flts[f].add(x)
            continue
        if kind == 'call': calls[f].append(t)
        elif kind == 'addr':
            s = elf.cstr(t)
            if text_ok(s): strs[f].add(s)
    return fs, strs, calls, flts


def pc_classes(im, ext):
    """the PC's runtime-class names: each GetRuntimeClass stub (`mov eax, offset D; ret`) returns a descriptor
    {name, size, base, ...} whose first dword points at the class's name (CApp: \"CApp\", size 0x44)"""
    lo, hi = im.base, im.base + len(im.img); out = set()
    for e in ext.values():
        ins = im.code(e)
        if len(ins) != 2: continue
        a, b = [ins[x] for x in sorted(ins)]
        if a.mnemonic == 'mov' and a.op_str.startswith('eax, 0x') and b.mnemonic == 'ret' and not b.op_str:
            d = a.operands[1].imm
            if lo <= d < hi - 4 and lo <= im.u32(d) < hi:
                n = bytes(im.img[im.u32(d) - lo:im.u32(d) - lo + 64]).split(b'\0')[0]
                if re.match(rb'^[A-Za-z_]\w{1,60}$', n): out.add(n.decode())
    return sorted(out)


def main(out):
    im = S.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    elf = ps2_elf.Elf(os.environ['PS2_ELF'])
    pstr, pcalls, pflt = pc_side(im, ext)
    fs, qstr, qcalls, qflt = ps2_side(elf)
    by_pc, by_ps = collections.defaultdict(set), collections.defaultdict(set)
    for f, ss in pstr.items():
        for s in ss: by_pc[s].add(f)
    for f, ss in qstr.items():
        for s in ss: by_ps[s].add(f)
    pairs = collections.defaultdict(list)
    for s in set(by_pc) & set(by_ps):
        if len(by_pc[s]) == 1 and len(by_ps[s]) == 1:
            pairs[(next(iter(by_pc[s])), next(iter(by_ps[s])))].append(s)
    pc_to = collections.defaultdict(set); ps_to = collections.defaultdict(set)
    for a, b in pairs: pc_to[a].add(b); ps_to[b].add(a)
    res, conflicts = {}, []
    for (a, b), ss in sorted(pairs.items()):
        if len(pc_to[a]) > 1 or len(ps_to[b]) > 1:
            conflicts.append({'pc': '0x%08X' % a, 'ps2': fs[b][0], 'strings': [show(x) for x in ss]}); continue
        res[a] = {'ps2': fs[b][0], 'ps2_addr': '0x%08X' % b, 'kind': 'string',
                  'evidence': ['unique literal %r' % show(x) for x in sorted(ss)]}
    # whole literal sets, and unique float constants
    def setkey(d): 
        c = collections.Counter(frozenset(v) for v in d.values() if len(v) >= 2)
        return {f: frozenset(v) for f, v in d.items() if len(v) >= 2 and c[frozenset(v)] == 1}
    pk, qk = setkey(pstr), setkey(qstr)
    qinv = {v: f for f, v in qk.items()}
    taken0 = {int(v['ps2_addr'], 16) for v in res.values()}
    for a, k in pk.items():
        b = qinv.get(k)
        if b is None or a in res or b in taken0: continue
        res[a] = {'ps2': fs[b][0], 'ps2_addr': '0x%08X' % b, 'kind': 'strings',
                  'evidence': ['the same %d literals, a set no other function holds on either side: %s'
                               % (len(k), ', '.join(repr(show(x)) for x in sorted(k)))]}
        taken0.add(b)
    fpc, fps = collections.defaultdict(set), collections.defaultdict(set)
    for f, v in pflt.items():
        for x in v: fpc[x].add(f)
    for f, v in qflt.items():
        for x in v: fps[x].add(f)
    fpairs = collections.defaultdict(list)
    for x in set(fpc) & set(fps):
        if len(fpc[x]) == 1 and len(fps[x]) == 1: fpairs[(next(iter(fpc[x])), next(iter(fps[x])))].append(x)
    fa, fb = collections.Counter(a for a, b in fpairs), collections.Counter(b for a, b in fpairs)
    for (a, b), xs in sorted(fpairs.items()):
        if len(xs) < 2 or fa[a] > 1 or fb[b] > 1 or a in res or b in taken0: continue
        res[a] = {'ps2': fs[b][0], 'ps2_addr': '0x%08X' % b, 'kind': 'floats',
                  'evidence': ['%d float constants no other function uses on either side: %s' % (len(xs), ', '.join(repr(x) for x in sorted(xs)))]}
        taken0.add(b)
    gnames = json.load(open(os.path.join(P, '..', 'runs', 'ghidra', 'names.json'))) \
        if os.path.exists(os.path.join(P, '..', 'runs', 'ghidra', 'names.json')) else {}
    game_ps = lambda b: b in fs and re.search(r'__(F|Q?\d+[A-Za-z_])', fs[b][0]) is not None and not fs[b][0].startswith('__')
    game_pc = lambda a: ('0x%08X' % a) in gnames and not re.match(r'^_[a-z]', gnames['0x%08X' % a])
    for a in [a for a, v in res.items() if v['kind'] == 'floats' and not (game_ps(int(v['ps2_addr'], 16)) and game_pc(a))]:
        del res[a]
    nstr = len(res)
    # library anchors: a PC function the IDB names exactly as a PS2 C function (unmangled: sprintf, strcpy), the
    # C runtime both builds link; and the string matches above
    pcname = {}
    for f in ('gfx_export.jsonl',):
        for l in open(os.path.join(D, 'exports', f)):
            for nm, o in (json.loads(l).get('objs') or {}).items():
                if o.get('func') and not o.get('import'): pcname.setdefault(o['ea'], nm.lstrip('_'))
    psname = {v[0]: k for k, v in fs.items() if '__' not in v[0]}
    lib = {a: psname[n] for a, n in pcname.items() if n in psname and not n.startswith(('sub_', 'nullsub'))}
    # propagation through the call order of matched pairs, to a fixpoint: the callees of both functions in the order
    # the code first calls them, split at the callees matched on both sides (a string or library match, or an
    # earlier propagation); where the same two anchors enclose exactly one unmatched callee on each side, those match
    match = lambda x: int(res[x]['ps2_addr'], 16) if x in res else lib.get(x)
    taken_ps = {int(v['ps2_addr'], 16) for v in res.values()} | set(lib.values())
    changed = True
    while changed:
        changed = False
        for a0, v in sorted(res.items()):
            b0 = int(v['ps2_addr'], 16)
            ca = list(dict.fromkeys(pcalls.get(a0, []))); cb = list(dict.fromkeys(qcalls.get(b0, [])))
            pos_b = {y: j for j, y in enumerate(cb)}
            anchors = [(i, pos_b[match(x)]) for i, x in enumerate(ca) if match(x) in pos_b]
            if any(j2 <= j1 for (_, j1), (_, j2) in zip(anchors, anchors[1:])): continue     # anchors out of order
            bounds = [(-1, -1)] + anchors + [(len(ca), len(cb))]
            for (i1, j1), (i2, j2) in zip(bounds, bounds[1:]):
                if i2 - i1 != 2 or j2 - j1 != 2: continue
                x, y = ca[i1 + 1], cb[j1 + 1]
                if x in res or match(x) is not None or y in taken_ps or not game_ps(y) or not game_pc(x) or ('0x%08X' % x) not in ext: continue
                lo_ = 'the start' if i1 < 0 else 'callee 0x%08X' % ca[i1]; hi_ = 'the end' if i2 == len(ca) else 'callee 0x%08X' % ca[i2]
                res[x] = {'ps2': fs[y][0], 'ps2_addr': '0x%08X' % y, 'kind': 'calls',
                          'evidence': ['the only unmatched callee between %s and %s, on both sides, of the matched pair 0x%08X = %s'
                                       % (lo_, hi_, a0, v['ps2'])]}
                taken_ps.add(y); changed = True
    # neighbours: an unmatched PC function g and PS2 function G pair up when at least NEIGH of g's matched callers
    # and callees have their partners among G's callers and callees in the same direction, and each is the
    # other's only best (a tie on either side is no match). Repeated to a fixpoint, each round on the matches before it
    pcall = {f: set(c for c in cs if ('0x%08X' % c) in ext) for f, cs in pcalls.items()}
    pcaller = collections.defaultdict(set)
    for f, cs in pcall.items():
        for c in cs: pcaller[c].add(f)
    qcall = {f: set(c for c in cs if c in fs) for f, cs in qcalls.items()}
    qcaller = collections.defaultdict(set)
    for f, cs in qcall.items():
        for c in cs: qcaller[c].add(f)
    NEIGH = 2
    while True:
        img = {a: int(v['ps2_addr'], 16) for a, v in res.items()}
        img.update({a: b for a, b in lib.items() if a not in img})
        taken = set(img.values())
        score = collections.defaultdict(lambda: collections.defaultdict(list))
        for a, b in img.items():
            for g in pcaller.get(a, ()):                  # g calls a: G calls b
                if g in res or not game_pc(g): continue
                for G in qcaller.get(b, ()):
                    if not game_ps(G): continue
                    if G not in taken: score[g][G].append('calls 0x%08X = %s' % (a, fs[b][0] if b in fs else hex(b)))
            for g in pcall.get(a, ()):                     # a calls g: b calls G
                if g in res or not game_pc(g): continue
                for G in qcall.get(b, ()):
                    if not game_ps(G): continue
                    if G not in taken: score[g][G].append('called by 0x%08X = %s' % (a, fs[b][0] if b in fs else hex(b)))
        best_pc = {}
        for g, cand in score.items():
            ranked = sorted(cand.items(), key=lambda kv: -len(kv[1]))
            if len(ranked[0][1]) < NEIGH or (len(ranked) > 1 and len(ranked[1][1]) == len(ranked[0][1])): continue
            best_pc[g] = ranked[0][0]
        by_G = collections.defaultdict(list)
        for g, G in best_pc.items(): by_G[G].append((len(score[g][G]), g))
        new = 0
        for G, gs in by_G.items():
            gs.sort(reverse=True)
            if len(gs) > 1 and gs[0][0] == gs[1][0]: continue
            # mutual: no other PC function scores higher for G either
            g = gs[0][1]
            if any(len(score[h].get(G, ())) >= len(score[g][G]) for h in score if h != g): continue
            res[g] = {'ps2': fs[G][0], 'ps2_addr': '0x%08X' % G, 'kind': 'neighbours', 'evidence': sorted(set(score[g][G]))}
            new += 1
        if not new: break
    # the name, in DancingMad's spelling (Class_Method; a free function's own name): the method from the PS2 symbol,
    # the class as the PS2 symbol has it (the PC's own class names are the PC descriptors', which may differ:
    # YmSchedulerTask is CMoSchedularTask on PC; hx2clean puts a matched method in the PC class it is placed in)
    for a, v in res.items():
        d = demangle(v['ps2'])
        cls, meth = d if d else (None, v['ps2'])
        v['ps2_class'] = cls; v['ps2_method'] = meth
        v['name'] = ('%s_%s' % (cls.replace('::', '_'), meth)) if cls else meth
        v['confidence'] = CONF[v['kind']]
    out_d = {'0x%08X' % k: v for k, v in sorted(res.items())}
    out_d['_conflicts'] = conflicts
    out_d['_pc_classes'] = pc_classes(im, ext)
    json.dump(out_d, open(out, 'w'), indent=1)
    kinds = collections.Counter(v['kind'] for v in res.values())
    print('PC strings in %d functions, PS2 in %d; matched %s; %d library anchors; %d conflicts' % (
        sum(1 for v in pstr.values() if v), sum(1 for v in qstr.values() if v), dict(kinds), len(lib), len(conflicts)))


if __name__ == '__main__':
    main(sys.argv[1])
