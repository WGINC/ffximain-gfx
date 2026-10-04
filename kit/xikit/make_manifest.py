"""make_manifest.py -- xikit_manifest.txt: the functions the kit may swap, and what it must find at each entry.

A function is listed only if (docs/KIT.md):
  * it is verified: PASS on fuzz and the four dumps in the clean tree as built (verification/gate_clean_final.jsonl);
  * the in-game reconstruction DLL exports it under the symbol the gate called (map_clean_cases.py);
  * its first 5 bytes are inside the function (data/func_extents.json), or the function is shorter and the rest
    is the int3/nop alignment padding after it (no function starts there);
  * no branch or call anywhere in the game's code targets entry+1 .. entry+4 (every function in
    func_extents.json decoded): such a jump would land inside the kit's `jmp`.
Flags, which leave a function out by default:
  1  SEH: the original installs an exception frame (reads fs:[0] in its first 64 bytes); the reconstruction has none
  2  speedfix: speedfix_hook patches an instruction inside it (its documented sites)
Each line: RVA FLAGS EXPORT EXPECTED RELOCS NAME
  RVA       entry - 0x10000000
  EXPECTED  20 bytes of the analysed image from entry-4 to entry+16, as hex
  RELOCS    the offsets (relative to the window's start, -3 .. 19) of the relocated dwords touching the window,
            comma-separated, or '-'. The kit subtracts the load delta from each such dword lying wholly inside the
            window before comparing; the bytes of one that straddles the window's edge are not compared (their
            value depends on the load address and the rest of the dword is outside the window).
Usage: python3 make_manifest.py RECON_INGAME_DLL OUT_MANIFEST [OUT_REPORT.json]     (uses $FFXI_DLL)"""
import bisect, json, os, struct, subprocess, sys
HERE = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.join(HERE, '..', '..')
sys.path.insert(0, os.path.join(ROOT, 'pipeline'))
import pefile
import sp_solve as S
from dump_verify import reloc_blocks
from capstone.x86 import X86_OP_IMM

SPEEDFIX_SITES = [0x1006C4D7, 0x100CC688, 0x100CD011, 0x1006CDDC]   # speedfix_hook.cpp: kReadbackCall, kActorQueryCalls, cpu tier
BASE = 0x10000000


def main(recon, out, report=None):
    D = os.path.join(ROOT, 'data'); V = os.path.join(ROOT, 'verification')
    tmp = out + '.cases.json'
    subprocess.run([sys.executable, os.path.join(ROOT, 'pipeline', 'map_clean_cases.py'), recon,
                    os.path.join(ROOT, 'tree', 'placement.json'), os.path.join(V, 'cases_raw_final.json'), tmp],
                   check=True, capture_output=True)
    cases = {c['original']: c for c in json.load(open(tmp))}; os.remove(tmp)
    passed = {json.loads(l)['original'] for l in open(os.path.join(V, 'gate_clean_final.jsonl')) if json.loads(l)['verdict'] == 'PASS'}
    place = json.load(open(os.path.join(ROOT, 'tree', 'placement.json')))
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
    lines, rep = [], {'listed': 0, 'refused': {}}
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
        if why: rep['refused'][key] = why; continue
        raw = bytes(img[ea - 4 - BASE:ea + 16 - BASE])            # a 20-byte window: 4 before the entry, 16 from it
        rel = [r - (ea - 4) for r in sorted(relocs) if ea - 7 <= r <= ea + 15]   # relocated dwords touching the window
        flags = 0
        head = bytes(img[ea - BASE:ea - BASE + 64])
        if b'\x64\xA1\x00\x00\x00\x00' in head or b'\x64\xFF\x35\x00\x00\x00\x00' in head: flags |= 1
        if ea in speedfix: flags |= 2
        name = place.get('0x%x' % ea, {}).get('symbol') or c['name'].split()[0]
        lines.append('%08X %d %s %s %s %s' % (ea - BASE, flags, c['candidate'], raw.hex().upper(),
                                              ','.join(map(str, rel)) or '-', name.replace(' ', '_')))
    rd = open(recon, 'rb').read(); import zlib
    with open(out, 'w', newline='\n') as f:
        f.write('# xikit manifest: RVA FLAGS EXPORT EXPECTED RELOCS NAME (kit/xikit/make_manifest.py; flags 1=SEH 2=speedfix site)\n')
        f.write('# FFXiMain.dll TimeDateStamp 0x%08X\n' % pe.FILE_HEADER.TimeDateStamp)
        f.write('!game_timestamp %08X\n!recon_size %d\n!recon_crc32 %08X\n' % (pe.FILE_HEADER.TimeDateStamp, len(rd), zlib.crc32(rd) & 0xFFFFFFFF))
        f.write('\n'.join(lines) + '\n')
    rep['listed'] = len(lines)
    rep['flags'] = {'seh': sum(1 for l in lines if int(l.split()[1]) & 1), 'speedfix': sum(1 for l in lines if int(l.split()[1]) & 2)}
    if report: json.dump(rep, open(report, 'w'), indent=1)
    print('%d listed (%s); %d refused' % (len(lines), rep['flags'], len(rep['refused'])))
    for k, v in sorted(rep['refused'].items()): print('  refused %s: %s' % (k, v))


if __name__ == '__main__':
    main(*sys.argv[1:4])
