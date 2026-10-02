"""real_sp.py -- the real stack depth at each instruction of a function, observed by running it.

stack_depth.py derives the depth statically and has to guess how much an indirect call pops (a COM
method, a virtual call through a vtable it cannot see). Here the original runs in the gate's emulator
over the gate's own trials (fuzzed, or live with FFXI_LIVE set), and ESP is read at every executed
instruction of the function: depth = bytes below ESP at entry (the return address excluded). Paths that
never execute have no entry. A conflict (one address seen at two depths) is reported, not hidden.

    python3 real_sp.py RUNDIR OUT.json ADDR [ADDR ...] [--trials N]      uses $FFXI_DLL
OUT.json: {"0xFUNC": {"0xADDR": depth, ...}}; ida/ffxi_ida_fix.py compares it with IDA's SP (get_spd)
and corrects IDA where the two part.
"""
import argparse, json, os, random, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import diffemu_gfx as DG
G = DG.G
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP


def observe(m, case, ext, trials):
    conv = case.get('conv', 'thiscall'); limit = case.get('insn_limit', 300000)
    ea = int(case['original'], 16)
    seen, conflicts, st = {}, {}, {'esp0': None}

    def hook(uc, addr, size, _):
        esp = uc.reg_read(UC_X86_REG_ESP)
        if addr == ea and st['esp0'] is None: st['esp0'] = esp + 4    # after the call pushed its return address
        if st['esp0'] is None: return
        d = st['esp0'] - 4 - esp
        if not 0 <= d < 0x10000: return                              # a recursive or nested activation
        if addr in seen and seen[addr] != d: conflicts.setdefault(addr, set()).update({seen[addr], d})
        else: seen[addr] = d
    hooks = [m.uc.hook_add(UC_HOOK_CODE, hook, begin=a, end=b - 1) for a, b in ext]
    fz = G.Fuzzer(m, 11)
    for _ in range(trials):
        this, args, ts = DG.setup_trial(m, case, fz)
        m.rng = random.Random(ts); m.wild = 0; st['esp0'] = None
        m.call(ea, conv, this, args, limit)
    for h in hooks: m.uc.hook_del(h)
    return seen, conflicts


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('run'); ap.add_argument('out'); ap.add_argument('addrs', nargs='+')
    ap.add_argument('--trials', type=int, default=100)
    a = ap.parse_args()
    cases = {c['original']: c for c in json.load(open(os.path.join(a.run, 'cases.json')))}
    exts = json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'func_extents.json')))
    res = json.load(open(a.out)) if os.path.exists(a.out) else {}
    for x in a.addrs:
        key = '0x%08X' % int(x, 16); c = cases[key]
        m = DG.GfxMachine(os.environ['FFXI_DLL'], os.path.join(a.run, 'raw', 'gfx_raw.dll'), c['stubs'], [])
        seen, conflicts = observe(m, c, exts['0x%x' % int(x, 16)], a.trials)
        res[key] = {'0x%08X' % k: v for k, v in sorted(seen.items()) if k not in conflicts}   # one depth only
        print('%s: %d instructions observed, %d conflicts %s' % (key, len(seen), len(conflicts),
              {'0x%08X' % k: sorted(v) for k, v in list(conflicts.items())[:5]}))
    json.dump(res, open(a.out, 'w'), indent=0)


if __name__ == '__main__':
    main()
