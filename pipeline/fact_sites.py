"""fact_sites.py -- the call sites whose stack effect the gate cannot settle, prepared for measuring in the game
(xikit facts=1, docs/KIT.md), and the measurements turned into data/game_sp.json.

A call the gate reaches with no definitive pop count (an UNRESOLVED verdict names it: "call 0x...: pop count
unknown") is measured where it is certain: the game's own code, running. The kit patches the call so that it runs
from a copy that records ESP just before the call and just after it returns, and where it went.

    python3 fact_sites.py sites OUT.txt [--from-gates] [ADDR ...]       (uses $FFXI_DLL)
        the sites, from the committed gate records' UNRESOLVED calls (--from-gates) and/or given addresses, with
        the patch region of each; written for the kit (kit/xikit/dist/xikit_facts.txt)
    python3 fact_sites.py ingest xikit_facts_<pid>.csv [...] [--out data/game_sp.json]
        observations to facts: a site's pop count is recorded only when every observed call agrees

The patch region of a site (every condition from the analysed binary; a site that fails one is listed as refused,
with why):
  * it ends at the call's return address, so a thread inside the callee when the kit patches returns to the
    game's own bytes after the region;
  * it is the call, and when that is shorter than the kit's 5-byte jmp, the whole instructions just before it,
    back to 5 bytes or more (at most 16);
  * those earlier instructions are neither branches, calls nor returns, and are in the same function piece;
  * nothing in the game's code jumps to an address inside the region after its first byte (every function in
    data/func_extents.json decoded).
Each line: SITE START LEN CALLOFF FIX EXPECTED RELOCS FUNC
  SITE     the call instruction's address (analysed)
  START    the region's first byte; LEN its length; CALLOFF the call's offset in it
  FIX      the offset of a rel32 displacement to re-aim (a direct call), or -1
  EXPECTED the region's analysed bytes, hex; RELOCS offsets in it of relocated dwords, or '-'
  FUNC     the function the site is in (analysed address)"""
import argparse, bisect, collections, glob, json, os, re, sys
P = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.join(P, '..'); sys.path.insert(0, P)
BASE = 0x10000000


def gate_sites():
    out = {}
    for f in ('gate_raw_combined.jsonl', 'gate_clean_final.jsonl'):
        for l in open(os.path.join(ROOT, 'verification', f)):
            r = json.loads(l)
            for k in (r.get('unresolved') or {}):
                m = re.match(r'call (0x[0-9A-Fa-f]+)', k)
                if m: out.setdefault(int(m.group(1), 16), r['original'])
    return out


def sites(o):
    import pefile
    import sp_solve as S
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    from capstone.x86 import X86_OP_IMM
    from dump_verify import reloc_blocks
    dll = os.environ['FFXI_DLL']
    pe = pefile.PE(dll, fast_load=True); img = pe.get_memory_mapped_image()
    relocs = set()
    blocks, _ = reloc_blocks(pe, img)
    for page, ents in blocks:
        for e in ents:
            if e >> 12 == 3: relocs.add(BASE + page + (e & 0xFFF))
    ext = json.load(open(os.path.join(ROOT, 'data', 'func_extents.json')))
    im = S.Image(dll)
    targets = set(); pieces = []
    for k, rs in ext.items():
        for a, b in rs: pieces.append((a, b, int(k, 16)))
        for a, i in im.code([tuple(x) for x in rs]).items():
            if (i.mnemonic in ('call', 'jmp') or i.mnemonic.startswith('j') or i.mnemonic.startswith('loop')) \
                    and i.operands and i.operands[0].type == X86_OP_IMM:
                targets.add(i.operands[0].imm)
    pieces.sort(); starts = [p[0] for p in pieces]
    want = gate_sites() if o.from_gates else {}
    for a in o.addrs: want.setdefault(int(a, 16), 'given')
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    lines, refused = [], {}
    for site, why_from in sorted(want.items()):
        j = bisect.bisect_right(starts, site) - 1
        piece = next((p for p in pieces[max(0, j - 40):j + 1][::-1] if p[0] <= site < p[1]), None)
        if not piece: refused[site] = 'no function piece holds it'; continue
        a, b, fn = piece
        ins = list(md.disasm(bytes(img[a - BASE:b - BASE]), a))
        k = next((n for n, i in enumerate(ins) if i.address == site), None)
        if k is None or ins[k].mnemonic != 'call': refused[site] = 'no call instruction decodes at it'; continue
        ret = site + ins[k].size; s = k
        while ret - ins[s].address < 5:
            s -= 1
            if s < 0: break
            i = ins[s]
            if i.mnemonic in ('call', 'jmp', 'ret', 'retn', 'int3') or i.mnemonic.startswith('j') or i.mnemonic.startswith('loop'):
                s = -1; break
        if s < 0: refused[site] = 'fewer than 5 bytes of plain instructions end at its return address'; continue
        start = ins[s].address; n = ret - start
        if n > 16: refused[site] = 'the region would be %d bytes' % n; continue
        hit = [t for t in range(start + 1, ret) if t in targets]
        if hit: refused[site] = 'a jump into the region (to 0x%08X)' % hit[0]; continue
        fix = (site - start + 1) if img[site - BASE] == 0xE8 else -1
        raw = bytes(img[start - BASE:ret - BASE])
        rel = [r - start for r in sorted(relocs) if start - 3 <= r < ret]
        if any(r < 0 or r + 4 > n for r in rel): refused[site] = 'a relocated dword straddles the region'; continue
        lines.append('%08X %08X %d %d %d %s %s %08X' % (site, start, n, site - start, fix, raw.hex().upper(),
                                                        ','.join(map(str, rel)) or '-', fn))
    with open(o.out, 'w', newline='\n') as f:
        f.write('# xikit facts: SITE START LEN CALLOFF FIX EXPECTED RELOCS FUNC (pipeline/fact_sites.py)\n')
        f.write('\n'.join(lines) + '\n')
    print('%d sites written, %d refused' % (len(lines), len(refused)))
    for k, v in sorted(refused.items()): print('  refused 0x%08X (%s): %s' % (k, want[k], v))


def ingest(o):
    obs = collections.defaultdict(collections.Counter); tg = collections.defaultdict(collections.Counter)
    for path in o.files:
        for l in open(path):
            if l.startswith('#') or l.startswith('site,'): continue
            p = l.rstrip('\n').split(',')
            site, pops, n = p[0], int(p[1]), int(p[2])
            obs[site][pops] += n; tg[site][p[3]] += n
    old = json.load(open(o.out)) if os.path.exists(o.out) else {}
    new = dict(old)
    for site, c in sorted(obs.items()):
        if len(c) != 1:
            print('%s: observed pop counts disagree %s: not recorded' % (site, dict(c))); continue
        pops = next(iter(c)); prev = old.get(site)
        if prev and prev['pops'] != pops:
            print('%s: %d now, %d recorded before: not recorded' % (site, pops, prev['pops'])); continue
        new[site] = {'pops': pops, 'observed_calls': sum(c.values()) + (prev['observed_calls'] if prev else 0),
                     'targets': sorted(set(tg[site]) | set(prev['targets'] if prev else [])),
                     'source': 'measured in the running game (xikit facts=1): ESP after the call minus ESP before it'}
        print('%s: pops %d (%d calls; targets %s)' % (site, pops, sum(c.values()), ', '.join(tg[site])))
    json.dump(new, open(o.out, 'w'), indent=1, sort_keys=True)


def main():
    ap = argparse.ArgumentParser(); sp = ap.add_subparsers(dest='cmd', required=True)
    a = sp.add_parser('sites'); a.add_argument('out'); a.add_argument('addrs', nargs='*'); a.add_argument('--from-gates', action='store_true')
    b = sp.add_parser('ingest'); b.add_argument('files', nargs='+'); b.add_argument('--out', default=os.path.join(ROOT, 'data', 'game_sp.json'))
    o = ap.parse_args()
    sites(o) if o.cmd == 'sites' else ingest(o)


if __name__ == '__main__':
    main()
