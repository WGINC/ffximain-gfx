"""make_manifest.py -- xikit_manifest.txt: the functions the kit may swap, and what it must find at each entry.

A function is listed only if (docs/KIT.md):
  * it is verified: PASS in the clean tree as built (verification/gate_clean_final.jsonl: fuzz verdicts only since
    the 22 Oct regeneration, 8ace87a; the four dumps' gates last run on the 4 Oct tree, docs/PROBLEMS.md P1);
  * the in-game reconstruction DLL exports it under the symbol the gate called (map_clean_cases.py);
  * its first 5 bytes are inside the function (data/func_extents.json), or the function is shorter and the rest
    is the int3/nop alignment padding after it (no function starts there);
  * no branch or call anywhere in the game's code targets entry+1 .. entry+4 (every function in
    func_extents.json decoded): such a jump would land inside the kit's `jmp`;
  * its in-game code holds, or reaches by direct calls, no game address left unrebased (pipeline/raw_game_addrs.py
    on the probe build, RAW_JSON: such an address is right in the gate, which loads the game at 0x10000000, and wrong
    in the game; 31 Oct). Without RAW_JSON this check is not made, and the report says so.
Flags, which leave a function out by default:
  1  SEH: the original installs an exception frame (reads fs:[0] in its first 64 bytes); the reconstruction has none
  2  speedfix: speedfix_hook patches an instruction inside it (its documented sites)
  8  register input: the original reads EDX at its entry (data/edx_use.json), takes its entry EAX as an input
     (data/eax_inputs.json: reads it, or hands it back to a caller that reads it), or ECX while the tree declares it
     neither __thiscall nor __fastcall (data/ecx_use.json): a value its declaration does not carry. Swapping is
     unaffected (the reconstruction was gated as it is); a mod may not hook it (its replacement could not see the
     register, nor pass it on to the previous code)
  4  untested: the gate's compared trials did not reach all of the original's instructions
     (verification/coverage_clean.json, pipeline/combine_coverage.py); a PASS says nothing about that code.
     Every function is flagged when the coverage file is missing. Also set for a PASS on fewer than THIN (10)
     compared trials (verification/gate_clean_final.jsonl: trials in which the original returned, over all gates).
Each line: RVA FLAGS EXPORT EXPECTED RELOCS NAME T=TRAMPOLINE
  RVA       entry - 0x10000000
  EXPECTED  20 bytes of the analysed image from entry-4 to entry+16, as hex
  RELOCS    the offsets (relative to the window's start, -3 .. 19) of the relocated dwords touching the window,
            comma-separated, or '-'. The kit subtracts the load delta from each such dword lying wholly inside the
            window before comparing; the bytes of one that straddles the window's edge are not compared (their
            value depends on the load address and the rest of the dword is outside the window).
  T=        how a mod's hook reaches the game's own code (trampoline() below): N bytes of whole instructions
            copied from the entry, ':o,o' rel32 displacements to re-aim, or '-' (no trampoline; the hook's
            'previous' is then the reconstruction's export). Older kits read the first six columns only.
Usage: python3 make_manifest.py RECON_INGAME_DLL OUT_MANIFEST [OUT_REPORT.json] [RAW_JSON]     (uses $FFXI_DLL)"""
import bisect, json, os, re, struct, subprocess, sys
HERE = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.join(HERE, '..', '..')
sys.path.insert(0, os.path.join(ROOT, 'pipeline'))
import pefile
import sp_solve as S
from dump_verify import reloc_blocks
from capstone.x86 import X86_OP_IMM

THIN = 10
SPEEDFIX_SITES = [0x1006C4D7, 0x100CC688, 0x100CD011, 0x1006CDDC]   # speedfix_hook.cpp: kReadbackCall, kActorQueryCalls, cpu tier
BASE = 0x10000000


EAX_INPUTS = json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'data', 'eax_inputs.json')))


def reg_input(ea, place, edx_use, ecx_use, free_conv):
    k = '0x%08X' % ea; p = place.get('0x%x' % ea, {})
    if edx_use.get(k) not in (None, False): return True
    if k in EAX_INPUTS: return True
    if ecx_use.get(k) in (None, False): return False
    if p.get('class'): return False                                     # a member: __thiscall, ECX is `this`
    return free_conv.get(p.get('symbol')) not in ('__thiscall', '__fastcall')


def trampoline(ea, rs, img, targets):
    """the whole instructions from the entry that a trampoline copies (>= 5 bytes: what the kit's jmp overwrites),
    as 'N' or 'N:o,o' (o: offset of a rel32 call/jmp displacement the kit re-aims), or '-' and why not: an
    instruction that ends the function or branches relative other than rel32 call/jmp, bytes past the function's
    extent, or a jump from anywhere in the game's code into the copied bytes after the entry."""
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True       # (operands below need the detail mode)
    n = 0; fix = []
    code = bytes(img[ea - BASE:ea - BASE + 32])
    for i in md.disasm(code, ea):
        if n >= 5: break
        b = i.bytes
        if b[0] in (0xE8, 0xE9) and i.size == 5: fix.append(n + 1)
        elif i.mnemonic in ('ret', 'retn', 'iret', 'int3', 'jmp') or i.mnemonic.startswith('j') or i.mnemonic.startswith('loop') \
                or i.mnemonic == 'call' and i.operands and i.operands[0].type == X86_OP_IMM:
            return '-', '%s at +%d' % (i.mnemonic, n)
        n += i.size
    if n < 5: return '-', 'undecodable'
    if n > 16: return '-', 'the copied %d bytes pass the 16 after the entry the kit compares' % n
    if not any(a <= ea and ea + n <= b for a, b in rs): return '-', 'the copied %d bytes pass the end of the function' % n
    hit = [t for t in range(ea + 1, ea + n) if t in targets]
    if hit: return '-', 'a jump into the copied bytes (0x%08X targets 0x%08X)' % (targets[hit[0]], hit[0])
    return ('%d:%s' % (n, ','.join(map(str, fix))) if fix else '%d' % n), None


def main(recon, out, report=None, raw_json=None):
    D = os.path.join(ROOT, 'data'); V = os.path.join(ROOT, 'verification')
    tmp = out + '.cases.json'
    subprocess.run([sys.executable, os.path.join(ROOT, 'pipeline', 'map_clean_cases.py'), recon,
                    os.path.join(ROOT, 'tree', 'placement.json'), os.path.join(V, 'cases_raw_final.json'), tmp],
                   check=True, capture_output=True)
    cases = {c['original']: c for c in json.load(open(tmp))}; os.remove(tmp)
    passed = {json.loads(l)['original'] for l in open(os.path.join(V, 'gate_clean_final.jsonl')) if json.loads(l)['verdict'] == 'PASS'}
    place = json.load(open(os.path.join(ROOT, 'tree', 'placement.json')))
    edx_use = json.load(open(os.path.join(D, 'edx_use.json'))); ecx_use = json.load(open(os.path.join(D, 'ecx_use.json')))
    free_conv = {}
    for l in open(os.path.join(ROOT, 'tree', 'include', 'gfx_free.hpp')):
        m = re.search(r'(__cdecl|__stdcall|__thiscall|__fastcall)\s+\**\s*(\w+)\(', l)
        if m: free_conv[m.group(2)] = m.group(1)
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    im = S.Image(os.environ['FFXI_DLL'])
    pe = pefile.PE(os.environ['FFXI_DLL'], fast_load=True); img = pe.get_memory_mapped_image()
    relocs = set()
    blocks, _ = reloc_blocks(pe, img)
    for page, ents in blocks:
        for e in ents:
            if e >> 12 == 3: relocs.add(BASE + page + (e & 0xFFF))
    # every branch/call target in the game's code
    targets = {}
    for k, rs in ext.items():
        for a, i in im.code([tuple(x) for x in rs]).items():
            if (i.mnemonic in ('call', 'jmp') or i.mnemonic.startswith('j') or i.mnemonic.startswith('loop')) \
                    and i.operands and i.operands[0].type == X86_OP_IMM:
                targets.setdefault(i.operands[0].imm, a)
    fstart = sorted((r[0], r[1], int(k, 16)) for k, rs in ext.items() for r in rs); fk = [x[0] for x in fstart]
    owner = lambda a: (lambda j: fstart[j][2] if j >= 0 and a < fstart[j][1] else None)(bisect.bisect_right(fk, a) - 1)
    speedfix = {owner(a) for a in SPEEDFIX_SITES}
    lines, rep = [], {'listed': 0, 'refused': {}, 'no_trampoline': {}}
    raw_fx = json.load(open(raw_json))['exports'] if raw_json else None
    rep['raw_address_check'] = 'made (%s)' % raw_json if raw_json else 'NOT made: no RAW_JSON given'
    cp = os.path.join(ROOT, 'verification', 'coverage_clean.json')
    coverage = json.load(open(cp)) if os.path.exists(cp) else {}
    # the trials in which the original returned normally (the only ones compared), over every gate of the record
    # the tree is annotated with; a PASS on fewer than THIN of them is thin evidence
    gp = os.path.join(ROOT, 'verification', 'gate_clean_final.jsonl')
    def n_returned(r):                  # summed over the gates (combine_gates.py: 'gates': {name: [verdict, n]})
        return sum(n for v, n in r['gates'].values()) if r.get('gates') else r['original_outcomes'].get('return', 0)
    returned = {'0x%08X' % int(r['original'], 16): n_returned(r) for r in map(json.loads, open(gp))} if os.path.exists(gp) else {}
    for key in sorted(passed):
        ea = int(key, 16); why = None
        c = cases.get(key)
        rs = ext.get('0x%x' % ea)
        if not c: why = 'no export for it in the reconstruction DLL'
        elif not rs or not any(a <= ea < b for a, b in rs): why = 'no extent for its entry'
        elif not any(a <= ea and ea + 5 <= b for a, b in rs):
            # shorter than the jmp: the jmp may run into the alignment padding after it, if that is only
            # int3/nop filler, no function starts there and nothing jumps there (checked below with the rest)
            end = max(b for a, b in rs if a <= ea < b)
            pad = bytes(img[end - BASE:ea + 5 - BASE])
            if not all(x in (0xCC, 0x90) for x in pad) or any(end <= f < ea + 5 for f in fk):
                why = 'the function is shorter than the 5-byte jmp at its entry, and what follows is not padding'
        if not why and c and rs:
            hit = [t for t in range(ea + 1, ea + 5) if t in targets]
            if hit: why = 'a jump into its first 5 bytes (0x%08X targets 0x%08X)' % (targets[hit[0]], hit[0])
        if not why and raw_fx is not None and c['candidate'] in raw_fx:
            why = 'its in-game code uses a game address without rebasing it (raw_game_addrs.py): %s' % raw_fx[c['candidate']][0]
        if why: rep['refused'][key] = why; continue
        raw = bytes(img[ea - 4 - BASE:ea + 16 - BASE])            # a 20-byte window: 4 before the entry, 16 from it
        rel = [r - (ea - 4) for r in sorted(relocs) if ea - 7 <= r <= ea + 15]   # relocated dwords touching the window
        flags = 0
        head = bytes(img[ea - BASE:ea - BASE + 64])
        if b'\x64\xA1\x00\x00\x00\x00' in head or b'\x64\xFF\x35\x00\x00\x00\x00' in head: flags |= 1
        if ea in speedfix: flags |= 2
        if reg_input(ea, place, edx_use, ecx_use, free_conv): flags |= 8
        cv = coverage.get('0x%08X' % ea)
        if not cv or cv.get('uncovered') is None or cv['uncovered'] or cv.get('missing'): flags |= 4
        if returned.get('0x%08X' % ea, 0) < THIN: flags |= 4             # thin evidence: too few compared trials
        name = place.get('0x%x' % ea, {}).get('symbol') or c['name'].split()[0]
        tr, tw = trampoline(ea, rs, img, targets)
        if tr == '-': rep['no_trampoline'][name] = tw
        # xikit.c reads EXPORT into 256 bytes and NAME into 96 (Fn.exp, Fn.name): a longer one would make it drop the line
        if len(c['candidate']) > 255 or len(name) > 95: sys.exit('make_manifest: %s: export or name too long for xikit.c' % name)
        lines.append('%08X %d %s %s %s %s T=%s' % (ea - BASE, flags, c['candidate'], raw.hex().upper(),
                                                   ','.join(map(str, rel)) or '-', name.replace(' ', '_'), tr))
    rd = open(recon, 'rb').read(); import zlib
    # the reconstruction DLL's code symbols (its COFF symbol table, the DLL this manifest is tied to by CRC): which
    # function a fault inside it is in. A '.cold' part (code GCC moved out of line) belongs to its function.
    rpe = pefile.PE(recon, fast_load=True); rbase = rpe.OPTIONAL_HEADER.ImageBase
    nm = subprocess.run([os.environ.get('NM', 'i686-w64-mingw32-nm'), '-n', recon], capture_output=True, text=True, check=True).stdout
    code = []
    for l in nm.splitlines():
        p = l.split()
        if len(p) == 3 and p[1] in 'Tt' and not p[2].startswith('.'):
            sym = p[2][1:] if p[2].startswith('_') else p[2]        # (the i686 C symbol underscore)
            code.append('!code %X %s' % (int(p[0], 16) - rbase, sym[:-5] if sym.endswith('.cold') else sym))
    with open(out, 'w', newline='\n') as f:
        f.write('# xikit manifest: RVA FLAGS EXPORT EXPECTED RELOCS NAME T=TRAMPOLINE (kit/xikit/make_manifest.py; flags 1=SEH 2=speedfix site 4=untested 8=register input)\n')
        f.write('# FFXiMain.dll TimeDateStamp 0x%08X\n' % pe.FILE_HEADER.TimeDateStamp)
        f.write('!game_timestamp %08X\n!recon_size %d\n!recon_crc32 %08X\n' % (pe.FILE_HEADER.TimeDateStamp, len(rd), zlib.crc32(rd) & 0xFFFFFFFF))
        f.write('\n'.join(lines) + '\n')
        f.write('\n'.join(code) + '\n')
    rep['listed'] = len(lines)
    rep['flags'] = {'seh': sum(1 for l in lines if int(l.split()[1]) & 1), 'speedfix': sum(1 for l in lines if int(l.split()[1]) & 2),
                    'untested': sum(1 for l in lines if int(l.split()[1]) & 4), 'register_input': sum(1 for l in lines if int(l.split()[1]) & 8)}
    if report: json.dump(rep, open(report, 'w'), indent=1)
    print('%d listed (%s); %d refused; %d without a trampoline' % (len(lines), rep['flags'], len(rep['refused']), len(rep['no_trampoline'])))
    for k, v in sorted(rep['refused'].items()): print('  refused %s: %s' % (k, v))


if __name__ == '__main__':
    main(*sys.argv[1:5])
