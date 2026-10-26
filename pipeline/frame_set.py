"""frame_set.py -- the functions a frame runs: the tree's selection by the frame, not by the device.

The graphics set (ida/ida_gfx.py select) is seeded on the Direct3D device: functions that refer to its holders, a
name family, and D3D-typed functions. A change to how a frame behaves (frame_step: the game's time step, made in
Frame_Render and read by 218 call sites in 113 functions) reaches far past that set. This lists every function the
frame is known to run, each with how it is known; nothing is added on a guess.

    python3 frame_set.py [--root 0x10001520 ...] [--samples xikit_samples_<pid>.csv ...] [--out data/frame_set.json]
                                                                                                    (uses $FFXI_DLL)
Evidence, kept apart (a function may have several):
  static    reached from a root by direct calls, direct tail jumps (jmp to another function's entry) and the
            virtual calls vcall_resolve.py proved (data/vcalls.json `targets`). An indirect call with no proof is
            not followed; each function's count of them is listed (`unresolved`), so the static set is a lower
            bound of what the frame can run.
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


COM = set()                                   # call sites of proven D3D8 methods (com_calls.py)
GAME = {}                                     # call site: the targets the running game called (game_sp.json)
UNRES = collections.defaultdict(list)         # each function's unresolved indirect call sites


def load_code():
    import pefile, capstone
    from capstone.x86 import X86_OP_IMM
    pe = pefile.PE(os.environ['FFXI_DLL'], fast_load=True)
    img = pe.get_memory_mapped_image()
    ext = {int(k, 16): [(s, e) for s, e in v] for k, v in json.load(open(os.path.join(D, 'func_extents.json'))).items()}
    vsite = collections.defaultdict(set)
    for k, v in json.load(open(os.path.join(D, 'vcalls.json'))).items():
        if k.startswith('_'): continue
        for t in v.get('targets') or []: vsite[int(k, 16)].add(int(t, 16))
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32); md.detail = True
    COM.update(int(k, 16) for k in json.load(open(os.path.join(D, 'com_calls.json'))))
    calls, unresolved, ends = {}, {}, {}
    for f, pieces in ext.items():
        out, n, e2 = set(), 0, {}
        for s, e in pieces:
            for i in md.disasm(img[s - BASE:e - BASE], s):
                e2[i.address + i.size] = i.mnemonic
                if i.mnemonic == 'call':
                    o = i.operands[0]
                    if o.type == X86_OP_IMM: out.add(o.imm)
                    elif i.address in vsite: out |= vsite[i.address]
                    elif i.address in COM: pass       # a D3D8 method on a proven object (com_calls.py): into d3d8, not game code
                    elif not (o.type == 3 and not o.mem.base and not o.mem.index): n += 1; UNRES[f].append(i.address)   # [abs]: an import thunk slot
                elif i.mnemonic == 'jmp' and i.operands[0].type == X86_OP_IMM and i.operands[0].imm in ext and i.operands[0].imm != f:
                    out.add(i.operands[0].imm)
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
    gp = os.path.join(D, 'game_sp.json')
    GAME.update({int(k, 16): v['targets'] for k, v in (json.load(open(gp)).items() if os.path.exists(gp) else ()) if not k.startswith('_') and v.get('targets')})
    owner = owner_index(ext)
    static = closure(calls, roots)
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
    every = static | set(self_n) | set(stack_n)
    funcs = {}
    for f in sorted(every):
        e = [k for k, ok in (('static', f in static), ('self', f in self_n), ('stack', f in stack_n)) if ok]
        r = {'evidence': e, 'tree': f in tree, 'd3dx': f in d3dx}
        if f in static and unresolved[f]:
            r['unresolved'] = unresolved[f]
            # of those, the sites the game was seen calling (xikit facts=1, data/game_sp.json `targets`): the
            # targets it called, which says what ran, not that nothing else can
            seen = {'0x%08X' % a: GAME[a] for a in UNRES[f] if a in GAME}
            if seen: r['observed_targets'] = seen
        if self_n[f]: r['self_samples'] = self_n[f]
        if stack_n[f]: r['stack_samples'] = stack_n[f]
        funcs['0x%08X' % f] = r
    game = [f for f in every if f not in d3dx]
    summary = {
        'functions': len(every), 'game_code': len(game), 'd3dx': len(every & d3dx),
        'static': len(static), 'only_observed': len(every - static),
        'in_tree': len(every & tree), 'game_code_not_in_tree': len([f for f in game if f not in tree]),
        'tree_not_in_frame_set': len(tree - every),
        'unresolved_indirect_calls': sum(unresolved[f] for f in static), 'functions_with_unresolved': sum(1 for f in static if unresolved[f]),
        'unresolved_observed_in_game': sum(1 for f in static for a in UNRES[f] if a in GAME),
    }
    json.dump({'_source': 'pipeline/frame_set.py on the analysed build (roots %s); evidence kinds in its docstring'
                          % ', '.join('0x%08X' % r for r in roots),
               '_roots': ['0x%08X' % r for r in roots], '_runs': runs, '_summary': summary, 'functions': funcs},
              open(o.out, 'w'), indent=1)
    for k, v in summary.items(): print('%-28s %d' % (k, v))


if __name__ == '__main__':
    main()
