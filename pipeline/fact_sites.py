"""fact_sites.py -- the call sites whose stack effect the gate cannot settle, prepared for measuring in the game
(xikit facts=1, docs/KIT.md), and the measurements turned into data/game_sp.json.

A call the gate reaches with no definitive pop count (an UNRESOLVED verdict names it: "call 0x...: pop count
unknown") is measured where it is certain: the game's own code, running. The kit patches the call so that it runs
from a copy that records ESP just before the call and just after it returns, and where it went.

    python3 fact_sites.py sites OUT.txt [--from-gates] [--from-file data/facts_wanted.json] [ADDR ...]   (uses $FFXI_DLL)
        the sites, from the committed gate records' UNRESOLVED calls (--from-gates), facts_wanted.py's list
        (--from-file) and/or given addresses, with
        the patch region of each; written for the kit (kit/xikit/dist/xikit_facts.txt)
        --out ADDR,...     these call sites also record their output slots (kind `out`); --out-fails: every call
                           in the functions whose raw gate verdict is FAIL and that the tree does not hold
        --jmp ADDR,...     these switch jumps (kind `jmp`); --jmp-unbounded: every `jmp [reg*4 + table]` in the DLL
                           whose targets sp_solve.switch_targets does not settle
    python3 fact_sites.py ingest xikit_facts_<pid>.csv [...] [--out data/game_sp.json] [--switch data/game_switch.json]
                                [--outs data/game_outs.json]
        observations to facts: a site's pop count is recorded only when every observed call agrees; a switch's
        targets seen (only ever a subset of what it can reach: what the game did, never what it cannot do); per output
        slot and class of returned EAX, the calls and how many changed the dword ("written on every observed call"
        only where all of them did: an unchanged dword may still have been written with the value it held)

The patch region of a site (every condition from the analysed binary; a site that fails one is listed as refused,
with why):
  * it ends at the call's return address, so a thread inside the callee when the kit patches returns to the
    game's own bytes after the region;
  * it is the call, and when that is shorter than the kit's 5-byte jmp, the whole instructions just before it,
    back to 5 bytes or more (at most 16);
  * those earlier instructions are neither branches, calls nor returns, and are in the same function piece;
  * nothing in the game's code jumps to an address inside the region after its first byte (every function in
    data/func_extents.json decoded).
A switch jump's region is the jump instruction itself (5 bytes or more, nothing jumping inside it).
Each line: SITE START LEN CALLOFF FIX EXPECTED RELOCS FUNC KIND
  SITE     the call instruction's address (analysed)
  START    the region's first byte; LEN its length; CALLOFF the call's offset in it
  FIX      the offset of a rel32 displacement to re-aim (a direct call), or -1
  EXPECTED the region's analysed bytes, hex; RELOCS offsets in it of relocated dwords, or '-'
  FUNC     the function the site is in (analysed address)
  KIND     call (pops and target), out (also the output slots), jmp (a switch jump: where it went)"""
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
    from capstone.x86 import X86_OP_IMM, X86_OP_MEM
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
    if o.from_file:                                 # data/facts_wanted.json (facts_wanted.py): {site: why}
        for a, why in json.load(open(o.from_file)).items(): want.setdefault(int(a, 16), why)
    outs = {int(a, 16) for a in (o.out_sites or '').split(',') if a}
    if o.out_fails:
        for f in fail_funcs():
            v = ext.get('0x%08X' % f) or ext.get('0x%08x' % f)
            if not v: continue
            for a, i in im.code([tuple(x) for x in v]).items():
                if i.mnemonic == 'call': outs.add(a)
    for a in outs: want.setdefault(a, 'output slots')
    jmps = {int(a, 16): 'given' for a in (o.jmp or '').split(',') if a}
    if o.jmp_unbounded:
        for k, v in ext.items():
            ins = im.code([tuple(x) for x in v]); order = sorted(ins); labels = S.labels_of(ins)
            for n, a in enumerate(order):
                i = ins[a]; op = i.operands[0] if i.operands else None
                if i.mnemonic != 'jmp' or op is None or op.type != X86_OP_MEM or op.mem.scale != 4 or not op.mem.index or op.mem.base: continue
                prev = [ins[x] for x in order[max(0, n - getattr(S, 'SWITCH_RUN', 5)):n]]
                if S.switch_targets(im, prev, i, labels) is None: jmps.setdefault(a, 'unbounded switch')
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    lines, refused = [], {}
    for site, why in sorted(jmps.items()):
        j = bisect.bisect_right(starts, site) - 1
        piece = next((p for p in pieces[max(0, j - 40):j + 1][::-1] if p[0] <= site < p[1]), None)
        if not piece: refused[site] = 'no function piece holds it'; want.setdefault(site, why); continue
        i = next(md.disasm(bytes(img[site - BASE:site - BASE + 16]), site), None)
        if i is None or i.mnemonic != 'jmp' or i.size < 5: refused[site] = 'no jump of 5 bytes or more decodes at it'; want.setdefault(site, why); continue
        hit = [t_ for t_ in range(site + 1, site + i.size) if t_ in targets]
        if hit: refused[site] = 'a jump into it (to 0x%08X)' % hit[0]; want.setdefault(site, why); continue
        rel = [r - site for r in sorted(relocs) if site - 3 <= r < site + i.size]
        if any(r < 0 or r + 4 > i.size for r in rel): refused[site] = 'a relocated dword straddles it'; want.setdefault(site, why); continue
        lines.append('%08X %08X %d 0 -1 %s %s %08X jmp' % (site, site, i.size, bytes(img[site - BASE:site - BASE + i.size]).hex().upper(),
                                                          ','.join(map(str, rel)) or '-', piece[2]))
    for site, why_from in sorted(want.items()):
        if site in jmps: continue
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
        lines.append('%08X %08X %d %d %d %s %s %08X %s' % (site, start, n, site - start, fix, raw.hex().upper(),
                                                           ','.join(map(str, rel)) or '-', fn, 'out' if site in outs else 'call'))
    with open(o.out, 'w', newline='\n') as f:
        f.write('# xikit facts: SITE START LEN CALLOFF FIX EXPECTED RELOCS FUNC KIND (pipeline/fact_sites.py)\n')
        f.write('\n'.join(lines) + '\n')
    kinds = collections.Counter(l.rsplit(' ', 1)[1] for l in lines)
    print('%d sites written (%s), %d refused' % (len(lines), ', '.join('%d %s' % (n, k) for k, n in sorted(kinds.items())), len(refused)))
    for k, v in sorted(refused.items()): print('  refused 0x%08X (%s): %s' % (k, want[k], v))


def fail_funcs():
    """the functions whose raw gate verdict is FAIL (verification/gate_raw_*.jsonl) and that the tree does not hold"""
    tree = set()
    for f in glob.glob(os.path.join(ROOT, 'tree', 'src', '*.cpp')):
        tree |= {int(x, 16) for x in re.findall(r'// FUNCTION: FFXIMAIN (0x[0-9A-F]+)', open(f).read())}
    last = {}
    for f in sorted(glob.glob(os.path.join(ROOT, 'verification', 'gate_raw_*.jsonl'))):
        for l in open(f):
            r = json.loads(l); last[int(r['original'], 16)] = r['verdict']
    return sorted(f for f, v in last.items() if v == 'FAIL' and f not in tree)


def ingest(o):
    obs = collections.defaultdict(collections.Counter); tg = collections.defaultdict(collections.Counter)
    sw = collections.defaultdict(lambda: {'targets': collections.Counter(), 'func': None})
    outs = collections.defaultdict(lambda: {'calls': 0, 'changed': 0, 'func': None})
    for path in o.files:
        for l in open(path):
            if l.startswith('#') or l.startswith('site,'): continue
            p = l.rstrip('\n').split(',')
            if p[0] == 'out':                       # out,site,slot,EAX class,calls,changed,function
                k = (p[1], p[2], p[3]); outs[k]['calls'] += int(p[4]); outs[k]['changed'] += int(p[5]); outs[k]['func'] = p[6]
                continue
            if p[1] == 'jmp':                       # site,jmp,n,target,vtable,slot,function
                sw[p[0]]['targets'][p[3]] += int(p[2]); sw[p[0]]['func'] = p[6]; continue
            site, pops, n = p[0], int(p[1]), int(p[2])
            obs[site][pops] += n; tg[site][p[3]] += n
    if sw:
        old = json.load(open(o.switch)) if os.path.exists(o.switch) else {}
        for site, v in sorted(sw.items()):
            prev = old.get(site, {})
            seen = collections.Counter(prev.get('targets_seen', {})); seen.update(v['targets'])
            old[site] = {'func': v['func'], 'targets_seen': dict(sorted(seen.items())),
                         'source': 'measured in the running game (xikit facts=1, kind jmp): the targets taken; a target '
                                   'not seen is not shown unreachable'}
            print('%s: switch, targets seen %s' % (site, ', '.join('%s x%d' % kv for kv in sorted(seen.items()))))
        json.dump(old, open(o.switch, 'w'), indent=1, sort_keys=True)
    if outs:
        # which stack dwords are arguments: the kit watches 8 above ESP at every call; past the callee's own arguments
        # they are the caller's locals. The count: what the callee popped at this site (measured, when above 0: its
        # ret N), or for 0 the callee's exact cdecl count (data/cdecl_args.json); with neither the slot is kept, marked
        cdecl = json.load(open(os.path.join(ROOT, 'data', 'cdecl_args.json')))
        nargs = {}
        for site, c in obs.items():
            if len(c) != 1: continue
            pops = next(iter(c)); tgt = [x for x in tg[site] if x.startswith('game:0x')]
            if pops > 0: nargs[site] = pops // 4
            elif len(tgt) == 1 and isinstance(cdecl.get('0x' + tgt[0][7:].upper()), int): nargs[site] = cdecl['0x' + tgt[0][7:].upper()]
        old = json.load(open(o.outs)) if os.path.exists(o.outs) else {}
        for (site, slot, cls), v in sorted(outs.items()):
            if slot.startswith('arg') and site in nargs and int(slot[3:]) >= nargs[site]: continue   # a local of the caller's
            e = old.setdefault(site, {'func': v['func'], 'slots': {},
                                      'source': 'measured in the running game (xikit facts=1, kind out): the dword a slot '
                                                'points to, before the call and after; changed shows a write, unchanged does not show none'})
            c = e['slots'].setdefault(slot, {}).setdefault(cls, {'calls': 0, 'changed': 0})
            c['calls'] += v['calls']; c['changed'] += v['changed']
            c['written_on_every_observed_call'] = c['changed'] == c['calls']
            if slot.startswith('arg') and site not in nargs: e['argument_count'] = 'not known: a stack dword may be a local of the caller'
            print('%s %s, EAX %s: %d calls, %d changed it' % (site, slot, cls, c['calls'], c['changed']))
        json.dump(old, open(o.outs, 'w'), indent=1, sort_keys=True)
    if not obs: return
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
    a = sp.add_parser('sites'); a.add_argument('out'); a.add_argument('addrs', nargs='*'); a.add_argument('--from-gates', action='store_true'); a.add_argument('--from-file')
    a.add_argument('--out', dest='out_sites', default=''); a.add_argument('--out-fails', action='store_true')
    a.add_argument('--jmp', default=''); a.add_argument('--jmp-unbounded', action='store_true')
    b = sp.add_parser('ingest'); b.add_argument('files', nargs='+'); b.add_argument('--out', default=os.path.join(ROOT, 'data', 'game_sp.json'))
    b.add_argument('--switch', default=os.path.join(ROOT, 'data', 'game_switch.json')); b.add_argument('--outs', default=os.path.join(ROOT, 'data', 'game_outs.json'))
    o = ap.parse_args()
    sites(o) if o.cmd == 'sites' else ingest(o)


if __name__ == '__main__':
    main()
