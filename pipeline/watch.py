"""watch.py -- who wrote a memory range, on each side of a gate trial?

Replays the case's first failing trial (as the gate makes it, live mode included when FFXI_LIVE is
set) and reports, for every byte in [ADDR, ADDR+LEN) whose final value differs between the original
and the reconstruction, the last instruction that wrote it on each side (the reconstruction's with its
source line from a -g rebuild of the function).

    python3 watch.py RUNDIR GATE.jsonl FUNC_ADDR ADDR [LEN]          uses $FFXI_DLL
"""
import json, os, random, struct, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import diffemu_gfx as DG, localize as L
from unicorn import UC_HOOK_MEM_WRITE
from unicorn.x86_const import UC_X86_REG_EIP


def main(run, gate, ea, addr, n='16'):
    ea = '0x%08X' % int(ea, 16); lo = int(addr, 16); hi = lo + int(n, 0)
    c = next(x for x in json.load(open(os.path.join(run, 'cases.json'))) if x['original'] == ea)
    g = next(json.loads(l) for l in open(gate) if json.loads(l)['original'] == ea)
    trial = g['first_mismatch']['trial']
    dll = L.debug_dll(os.path.join(run, 'raw'), ea)
    m = DG.GfxMachine(os.environ['FFXI_DLL'], dll, c['stubs'], [])
    fz = DG.G.Fuzzer(m, 11)
    for _ in range(trial + 1): this, args, ts = DG.setup_trial(m, c, fz)
    last = {}
    def wr(uc, acc, a, size, val, _):
        for k in range(size):
            if lo <= a + k < hi: last[a + k] = (uc.reg_read(UC_X86_REG_EIP), (val >> (8 * k)) & 0xFF)
    m.uc.hook_add(UC_HOOK_MEM_WRITE, wr)
    conv = c.get('conv', 'thiscall'); limit = c.get('insn_limit', 300000)
    for _ in range(3):
        k = len(m.demand); m.rng = random.Random(ts); m.call(int(ea, 16), conv, this, args, limit)
        if len(m.demand) == k: break
    last.clear(); m.rng = random.Random(ts); m.call(int(ea, 16), conv, this, args, limit); lo_ = dict(last)
    last.clear(); m.rng = random.Random(ts); m.call(m.exports[c['candidate']], conv, this, args, limit); lr = dict(last)
    print('trial %d, this %s' % (trial, hex(this) if this else None))
    for a in sorted(set(lo_) | set(lr)):
        o, r = lo_.get(a), lr.get(a)
        if (o and o[1]) != (r and r[1]):
            print('%08X  original %s  reconstruction %s %s' % (a, ('%02X by %08X' % (o[1], o[0])) if o else 'unwritten',
                  ('%02X by %08X' % (r[1], r[0])) if r else 'unwritten', L.src_line(dll, r[0]) if r and r[0] >= 0x60000000 else ''))


if __name__ == '__main__':
    main(*sys.argv[1:])
