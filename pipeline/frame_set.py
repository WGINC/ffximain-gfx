"""frame_set.py -- the functions a frame runs: the tree's selection by the frame, not by the device.

The graphics set (ida/ida_gfx.py select) is seeded on the Direct3D device: functions that refer to its holders, a
name family, and D3D-typed functions. A change to how a frame behaves (frame_step: the game's time step, made in
Frame_Render and read by 218 call sites in 113 functions) reaches far past that set. This lists every function the
frame is known to run, each with how it is known; nothing is added on a guess.

    python3 frame_set.py [--root 0x10001520 ...] [--samples xikit_samples_<pid>.csv ...] [--out data/frame_set.json]
                                                                                                    (uses $FFXI_DLL)
Evidence, kept apart (a function may have several):
  static    reached from a root by direct calls, direct tail jumps (jmp to another function's entry) and the
            indirect calls and jumps whose complete target set the code proves (data/indirect_targets.json `static`:
            vcall_resolve.py's). A Direct3D 8 method call on a proven object (`com`, `com_accepted`) goes into the
            Direct3D implementation, not game code. Any other indirect call is not followed; each function's count of
            them is listed (`unresolved`; jumps apart, `unresolved_jumps`), so the static set is a lower bound of what
            the frame can run.
  recorded  reached only when the targets the running game was seen calling at those sites (indirect_targets.json
            `recorded`, game functions only) are followed too, from the static set and from what they reach. Observed,
            not proven complete: these functions ran in a session; others the same sites can reach are not shown.
  self      the kit's sampler (xikit samples, sample=1) found the render thread inside the function's code
            (`self,game` rows): it ran on the render thread in that run. On ARM64 Windows under x86 emulation the
            sampled EIP is not a position (docs/HISTORY.md, 18 Oct: 0x00010002), so such runs give few of these.
  stack     an `incl,game` address (a value on the render thread's stack that points into game code) is a return
            address of a call in the function: the instruction before it is a call. A stack value can be stale, so
            this says the function called something at some point before the sample, not that the frame runs it.
Roots: CApp::RunMainLoop (0x10001520) by default: every frame goes through it (its inclusive share in the samples
is 100%).
The output lists, per function: the evidence, whether it is in tree/ (placement.json), and whether it is the
statically linked D3DX 8.1 (data/d3dx/d3dx81_manifest.json: not game code)."""
import argparse, bisect, collections, csv, json, os, sys
P = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.join(P, '..'); D = os.path.join(ROOT, 'data')
BASE = 0x10000000


GAME = {}                                     # call site: the targets the running game called (indirect_targets `recorded`)
UNRES = collections.defaultdict(list)         # each function's unresolved indirect call sites
UNRES_J = collections.defaultdict(list)       # and jumps
REC = collections.defaultdict(set)            # function: game functions its unresolved sites were seen calling


def load_code():
    import pefile, capstone
    from capstone.x86 import X86_OP_IMM, X86_OP_MEM
    pe = pefile.PE(os.environ['FFXI_DLL'], fast_load=True)
    img = pe.get_memory_mapped_image()
    ext = {int(k, 16): [(s, e) for s, e in v] for k, v in json.load(open(os.path.join(D, 'func_extents.json'))).items()}
    it = {int(k, 16): v for k, v in json.load(open(os.path.join(D, 'indirect_targets.json')))['sites'].items()}
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32); md.detail = True
    calls, unresolved, ends = {}, {}, {}
    for f, pieces in ext.items():
        out, n, e2 = set(), 0, {}
        for s, e in pieces:
            for i in md.disasm(img[s - BASE:e - BASE], s):
                e2[i.address + i.size] = i.mnemonic
                if i.mnemonic not in ('call', 'jmp') or not i.operands: continue
                o = i.operands[0]
                if o.type == X86_OP_IMM:
                    if i.mnemonic == 'call' or (o.imm in ext and o.imm != f): out.add(o.imm)
                    continue
                r = it.get(i.address)
                if r is None:
                    # indirect_targets.py leaves out only import slots ([abs]) and bounded switches ([idx*4 + T]),
                    # both with no base register; any other site missing means the file is stale
                    if o.type != X86_OP_MEM or o.mem.base:
                        sys.exit('0x%08X %s %s: not in data/indirect_targets.json (regenerate it)' % (i.address, i.mnemonic, i.op_str))
                    continue
                if r['status'] == 'static': out |= {int(t, 16) for t in r['static']['targets']}
                elif r['status'] in ('com', 'com_accepted'): pass      # into the Direct3D implementation, not game code
                else:
                    (UNRES if i.mnemonic == 'call' else UNRES_J)[f].append(i.address)
                    if i.mnemonic == 'call': n += 1
                    seen = [t for t in (r.get('recorded') or {}).get('targets', ()) if t.startswith('0x')]
                    if r.get('recorded'): GAME[i.address] = r['recorded']['targets']
                    REC[f] |= {int(t, 16) for t in seen if int(t, 16) in ext and int(t, 16) != f}
        calls[f] = {c for c in out if c in ext}; unresolved[f] = n; ends[f] = e2
    return ext, calls, unresolved, ends


def closure(calls, roots):
    seen = set(roots); st = list(roots)
    while st:
        f = st.pop()
        for c in calls.get(f, ()):
            if c not in seen: seen.add(c); st.append(c)
    return seen


def owner_index(ext):
    pieces = sorted((s, e, f) for f, v in ext.items() for s, e in v)
    starts = [p[0] for p in pieces]
    def owner(a, end_inclusive=False):
        j = bisect.bisect_right(starts, a) - 1
        while j >= 0 and starts[j] > a - 0x100000:
            s, e, f = pieces[j]
            if s <= a < e or (end_inclusive and a == e): return f
            j -= 1
        return None
    return owner


def main():
    ap = argparse.ArgumentParser(); ap.add_argument('--root', action='append'); ap.add_argument('--samples', nargs='*', default=[])
    ap.add_argument('--out', default=os.path.join(D, 'frame_set.json'))
    o = ap.parse_args()
    roots = [int(r, 16) for r in (o.root or ['0x10001520'])]
    ext, calls, unresolved, ends = load_code()
    owner = owner_index(ext)
    static = closure(calls, roots)
    recorded = closure({f: calls[f] | REC[f] for f in calls}, roots) - static
    tree = {int(k, 16) for k in json.load(open(os.path.join(ROOT, 'tree', 'placement.json')))}
    d3dx = {int(k, 16) for k in json.load(open(os.path.join(D, 'd3dx', 'd3dx81_manifest.json')))}
    self_n, stack_n, runs = collections.Counter(), collections.Counter(), []
    for path in o.samples:
        head = open(path).readline().strip(); runs.append({'file': os.path.basename(path), 'header': head})
        for r in csv.reader(l for l in open(path) if not l.startswith('#')):
            if len(r) < 4 or r[1] != 'game': continue
            a = int(r[2].split()[0], 16); n = int(r[3])
            if r[0] == 'self':
                f = owner(a)
                if f: self_n[f] += n
            elif r[0] == 'incl':
                f = owner(a, end_inclusive=True)
                if f and ends[f].get(a) == 'call': stack_n[f] += n     # a return address: the instruction before is a call
    every = static | recorded | set(self_n) | set(stack_n)
    funcs = {}
    for f in sorted(every):
        e = [k for k, ok in (('static', f in static), ('recorded', f in recorded), ('self', f in self_n), ('stack', f in stack_n)) if ok]
        r = {'evidence': e, 'tree': f in tree, 'd3dx': f in d3dx}
        if (f in static or f in recorded) and (unresolved[f] or UNRES_J[f]):
            if unresolved[f]: r['unresolved'] = unresolved[f]
            if UNRES_J[f]: r['unresolved_jumps'] = len(UNRES_J[f])
            # of those, the sites the game was seen calling (xikit facts=1, indirect_targets.json `recorded`): the
            # targets it called, which says what ran, not that nothing else can
            seen = {'0x%08X' % a: GAME[a] for a in UNRES[f] + UNRES_J[f] if a in GAME}
            if seen: r['observed_targets'] = seen
        if self_n[f]: r['self_samples'] = self_n[f]
        if stack_n[f]: r['stack_samples'] = stack_n[f]
        funcs['0x%08X' % f] = r
    game = [f for f in every if f not in d3dx]
    summary = {
        'functions': len(every), 'game_code': len(game), 'd3dx': len(every & d3dx),
        'static': len(static), 'recorded': len(recorded), 'only_observed': len(every - static - recorded),
        'in_tree': len(every & tree), 'game_code_not_in_tree': len([f for f in game if f not in tree]),
        'tree_not_in_frame_set': len(tree - every),
        'unresolved_indirect_calls': sum(unresolved[f] for f in static), 'functions_with_unresolved': sum(1 for f in static if unresolved[f]),
        'unresolved_observed_in_game': sum(1 for f in static for a in UNRES[f] if a in GAME),
        'unresolved_indirect_jumps': sum(len(UNRES_J[f]) for f in static),
        'recorded_reach_unresolved_calls': sum(unresolved[f] for f in recorded),
    }
    json.dump({'_source': 'pipeline/frame_set.py on the analysed build (roots %s); evidence kinds in its docstring'
                          % ', '.join('0x%08X' % r for r in roots),
               '_roots': ['0x%08X' % r for r in roots], '_runs': runs, '_summary': summary, 'functions': funcs},
              open(o.out, 'w'), indent=1)
    for k, v in summary.items(): print('%-28s %d' % (k, v))


if __name__ == '__main__':
    main()
