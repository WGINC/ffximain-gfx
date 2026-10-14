"""build_translate.py -- can the kit run on another build of FFXiMain.dll, and how its addresses translate.

    python3 build_translate.py ANALYSED.dll OTHER_unpacked.dll [OUT.json]
      (OTHER_unpacked.dll: pipeline/unpack_pol.py of the shipped DLL; OUT defaults to kit/xikit/builds/<stamp>.json)

The kit, the reconstruction and the mods address the game in the analysed build's numbering (XI_GAME). For another
build they need, for every address they use, where it is there. This settles that from the two images, and writes it
only when it is settled for everything they use:

1. Code ranges. pipeline/build_map.py maps each function of the analysed build to the other by its bytes (absolute
   addresses and branch displacements masked). Its matches, in address order, fall into runs of equal shift; each
   run of at least 3 functions gives a range [first function of the run, first function of the next run) moved by
   that shift.
2. The manifest's functions. Each must be PROVEN the same code at its range's shift, chunk by chunk: every instruction
   byte-identical except call/jump displacements, whose new target must be the old target's new address, and absolute
   operands, whose new value must be the old value's new address (pipeline/build_translate.py: same_code()). A chunk
   may also be placed by its proven neighbours when its bytes fit several shifts (short stubs): see same_code().
3. Every address the reconstruction and speedfix name. The XI_GAME / XI_EA / orig:: literals in tree/ and
   kit/speedfix/ and sdk/include/: a code address must lie in a range and not in a function that changed; a data
   address (at or above the analysed .rdata) must be referenced by the same instruction operand at the translated
   place in the other build, wherever the analysed build references it (the data sections are not shifted unless
   this shows it).
Any failure: no output, and the reason per address. The output: the ranges, the counts, and what changed."""
import bisect, collections, json, os, re, subprocess, sys
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM

R = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')
B = 0x10000000


def load(p):
    pe = pefile.PE(p, fast_load=True)
    return pe, pe.get_memory_mapped_image()


def main(old_p, new_p, out=None):
    ope, oimg = load(old_p); npe, nimg = load(new_p)
    ots, nts = ope.FILE_HEADER.TimeDateStamp, npe.FILE_HEADER.TimeDateStamp
    text = ope.sections[0]; TLO, THI = B + text.VirtualAddress, B + ope.sections[1].VirtualAddress     # .text .. .rdata
    tmp = os.path.join(os.path.dirname(os.path.abspath(new_p)), 'build_map_%08X.json' % nts)
    subprocess.run([sys.executable, os.path.join(R, 'pipeline', 'build_map.py'), old_p, '-', '0x10000000', tmp],
                   check=True, capture_output=True) if False else None
    # build_map wants the other build's flat image: write it next to the unpacked file
    flat = tmp.replace('.json', '.bin'); open(flat, 'wb').write(nimg)
    r = subprocess.run([sys.executable, os.path.join(R, 'pipeline', 'build_map.py'), old_p, flat, '0x10000000', tmp],
                       capture_output=True, text=True)
    if r.returncode: sys.exit('build_map failed: ' + r.stderr[-2000:])
    bm = json.load(open(tmp)); fmap = {int(k, 16): int(v, 16) for k, v in bm['functions'].items()}
    gmap = {int(k, 16): int(v, 16) for k, v in bm['globals'].items()}
    ext = {int(k, 16): v for k, v in json.load(open(os.path.join(R, 'data', 'func_extents.json'))).items()}
    md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True

    # 1. code ranges from runs of equal shift
    pts = sorted((a, d - a) for a, d in fmap.items() if TLO <= a < THI)
    runs = []
    for a, d in pts:
        if runs and runs[-1][2] == d: runs[-1][3] += 1
        else: runs.append([a, None, d, 1])
    runs = [x for x in runs if x[3] >= 3]
    merged = []
    for x in runs:
        if merged and merged[-1][2] == x[2]: merged[-1][3] += x[3]
        else: merged.append(x)
    ranges = []
    for i, x in enumerate(merged):
        lo = TLO if i == 0 else x[0]; hi = merged[i + 1][0] if i + 1 < len(merged) else THI
        ranges.append((lo, hi, x[2]))
    starts = [x[0] for x in ranges]

    def shift_of(a):
        i = bisect.bisect_right(starts, a) - 1
        return ranges[i][2] if i >= 0 and ranges[i][0] <= a < ranges[i][1] else None

    def tr(v):
        if TLO <= v < THI:
            d = shift_of(v); return v + d if d is not None else None
        return gmap.get(v, v)                       # data: as build_map paired it (checked in step 3 for what we use)

    # the functions whose bytes are not the same at their range's shift: what changed
    changed = []
    for a, ch in sorted(ext.items()):
        if not (TLO <= a < THI) or len(ch) != 1: continue
        d = shift_of(a)
        if d is None: continue
        if fmap.get(a) is None or fmap[a] - a != d:
            s, e = ch[0]
            if not same_code(md, oimg, nimg, s, e, d, ch, tr)[0]: changed.append((s, e))

    # 2. the manifest's functions
    man = [B + int(l.split()[0], 16) for l in open(os.path.join(R, 'kit', 'xikit', 'dist', 'xikit_manifest.txt'))
           if l[:1] not in '#!' and l.strip()]
    bad = {}
    for ea in man:
        ch = ext.get(ea)
        if not ch: bad['%08X' % ea] = 'no extents'; continue
        for s, e in ch:
            d = shift_of(s)
            if d is None: bad['%08X' % ea] = 'chunk %08X in no range' % s; break
            ok, why = same_code(md, oimg, nimg, s, e, d, ch, tr)
            if not ok: bad['%08X' % ea] = why; break

    # 3. every address the reconstruction, speedfix and the SDK name
    lits = set()
    for root in ('tree/src', 'tree/include', 'kit/speedfix', 'sdk/include'):
        for dp, _, fs in os.walk(os.path.join(R, root)):
            for f in fs:
                if not f.endswith(('.cpp', '.hpp', '.h', '.c')): continue
                t = open(os.path.join(dp, f), errors='replace').read()
                for m in re.finditer(r'XI_(?:GAME|EA)\([^,()]*?,?\s*(0x1[0-9A-Fa-f]{7})u?\)|XI_GAME\((0x1[0-9A-Fa-f]{7})u?\)|reaim_call\((0x1[0-9A-Fa-f]{7})u?', t):
                    lits.add(int(next(g for g in m.groups() if g), 16))
    chg_starts = [s for s, _ in changed]
    data_refs = collections.defaultdict(list)
    for a in sorted(ext):                               # where the analysed build's code names each data address
        for s, e in ext[a]:
            if not (TLO <= s < THI): continue
            for i in md.disasm(oimg[s - B:e - B], s):
                for op in i.operands:
                    v = op.mem.disp & 0xFFFFFFFF if op.type == X86_OP_MEM else (op.imm & 0xFFFFFFFF if op.type == X86_OP_IMM else None)
                    if v is not None and v >= THI and v < B + ope.OPTIONAL_HEADER.SizeOfImage: data_refs[v].append(i.address)
    lit_bad = {}; ndata = ncode = 0
    for v in sorted(lits):
        if TLO <= v < THI:
            ncode += 1
            if shift_of(v) is None: lit_bad['%08X' % v] = 'code address in no range'; continue
            k = bisect.bisect_right(chg_starts, v) - 1
            if k >= 0 and changed[k][0] <= v < changed[k][1]: lit_bad['%08X' % v] = 'inside a function that changed'
        else:
            ndata += 1
            refs = data_refs.get(v, [])
            for site in refs:
                ns = tr(site)
                if ns is None: lit_bad['%08X' % v] = 'referenced at %08X, which is in no range' % site; break
                ins = next(md.disasm(oimg[site - B:site - B + 16], site)); n2 = next(md.disasm(nimg[ns - B:ns - B + 16], ns), None)
                if n2 is None or n2.bytes != ins.bytes and n2.size == ins.size and v.to_bytes(4, 'little') not in n2.bytes:
                    lit_bad['%08X' % v] = 'referenced at %08X, not at %08X in the other build' % (site, ns); break
    rep = {
        'analysed_timestamp': '%08X' % ots, 'timestamp': '%08X' % nts,
        'ranges': [['%08X' % lo, '%08X' % hi, d] for lo, hi, d in ranges],
        'functions_mapped_by_build_map': len(fmap), 'manifest_functions': len(man), 'manifest_proven': len(man) - len(bad),
        'functions_not_shown_same': ['%08X-%08X' % c for c in changed],   # changed, or holding data (jump tables) the check cannot read
        'addresses_named': {'code': ncode, 'data': ndata}, 'failures': {'manifest': bad, 'addresses': lit_bad},
    }
    print(json.dumps({k: v for k, v in rep.items() if k != 'failures'}, indent=1))
    if bad or lit_bad:
        print('NOT SETTLED: %d manifest functions, %d named addresses' % (len(bad), len(lit_bad)))
        for k, v in list(bad.items())[:20] + list(lit_bad.items())[:20]: print('  ', k, v)
        sys.exit(1)
    out = out or os.path.join(R, 'kit', 'xikit', 'builds', '%08X.json' % nts)
    os.makedirs(os.path.dirname(out), exist_ok=True)
    json.dump(rep, open(out, 'w'), indent=1); open(out, 'a').write('\n')
    print('settled: wrote', out)


def same_code(md, oimg, nimg, s, e, d, chunks, tr):
    """Chunk [s, e) of a function at shift d: every instruction byte-identical, except call/jump displacements
    (the new target is the translated old target; inside the function: by the same shift) and absolute operands
    (the new value is the translated old value)."""
    code = oimg[s - B:e - B]; ncode = nimg[s + d - B:e + d - B]
    ins = list(md.disasm(code, s))
    if sum(i.size for i in ins) != e - s: return False, 'chunk %08X does not disassemble whole' % s
    for i in ins:
        o = i.address - s; ob = code[o:o + i.size]; nb = ncode[o:o + i.size]
        rel = (i.mnemonic == 'call' or i.mnemonic.startswith('j')) and i.operands and i.operands[0].type == X86_OP_IMM and i.size >= 5
        if rel and ob[:i.size - 4] == nb[:i.size - 4]:
            ot = i.operands[0].imm
            nt = s + d + o + i.size + int.from_bytes(nb[i.size - 4:], 'little', signed=True)
            exp = ot + d if any(a <= ot < b for a, b in chunks) else tr(ot)
            if exp != nt: return False, '%s at %08X: target %08X is %08X there, expected %s' % (i.mnemonic, i.address, ot, nt, '%08X' % exp if exp else 'none')
            continue
        nb2 = bytearray(nb)
        for k in range(0, i.size - 3):
            v = int.from_bytes(ob[k:k + 4], 'little')
            if B <= v < B + 0xC15000:
                t = tr(v); nv = int.from_bytes(nb[k:k + 4], 'little')
                if t is not None and nv == t: nb2[k:k + 4] = ob[k:k + 4]
        if bytes(nb2) != ob: return False, 'instruction %08X %s %s differs' % (i.address, i.mnemonic, i.op_str)
    return True, ''


if __name__ == '__main__':
    if len(sys.argv) < 3: sys.exit(__doc__)
    main(*sys.argv[1:4])
