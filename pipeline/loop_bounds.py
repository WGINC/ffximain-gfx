"""loop_bounds.py -- why does an original run to the instruction limit? Find the loop and its bound.

For each function: replay gate trials of the original until some hit the instruction limit, record
where they stop, find the innermost loop around that address (a backward branch), and list what the
loop's exit test compares against and where that value was loaded from: a global (absolute address),
a field of `this`/an argument (register + displacement), or a stack local. A fuzzed count there is an
input the game cannot produce; data/input_ranges.json (globals) or the case's ranges bound it.

    python3 loop_bounds.py FFXiMain_unpacked.dll RUNDIR ADDR [ADDR...]   (RUNDIR has cases.json, raw/)
"""
import collections, json, os, random, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import diffemu_gfx as DG
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG, X86_REG_INVALID

G = DG.G
D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')


def stops(m, case, trials=40):
    fz = G.Fuzzer(m, 11); conv = case.get('conv', 'thiscall'); out = collections.Counter()
    orig = int(case['original'], 16)
    for _ in range(trials):
        fz.reset()
        for g, n in case.get('fuzz_ranges', []):
            m.uc.mem_write(int(g, 16), bytes(0 if fz.rng.random() < 0.5 else fz.rng.getrandbits(8) for _ in range(n)))
        for g, spec in case.get('globals', {}).items():
            m.uc.mem_write(int(g, 16), (fz.arg(spec) & 0xFFFFFFFF).to_bytes(4, 'little'))
        this = fz.buffer(int(str(case.get('this_size', '0x400')), 0), int(case['vptr'], 16) if case.get('vptr') else None) if conv == 'thiscall' else None
        args = [fz.arg(s) for s in case.get('args', [])]
        ts = fz.rng.getrandbits(32); m.page_seed = ts; m.reset_demand(); m.constrain(case.get('input_ranges', []), this, args); m.ctx = m.uc.context_save()
        for _ in range(3):
            n = len(m.demand); m.rng = random.Random(ts); m.wild = 0
            r = m.call(orig, conv, this, args, case.get('insn_limit', 300000))
            if len(m.demand) == n: break
        if r.status == 'limit': out[r.stop_eip] += 1
    return out


def loop_info(ins, at, eip):
    # innermost backward branch whose range [target, branch] contains eip
    best = None
    for i in ins:
        if i.mnemonic.startswith('j') or i.mnemonic.startswith('loop'):
            op = i.operands[0] if i.operands else None
            if op is not None and op.type == X86_OP_IMM and op.imm <= eip <= i.address and op.imm < i.address:
                if best is None or i.address - op.imm < best[1] - best[0]: best = (op.imm, i.address)
    if not best: return None
    body = [i for i in ins if best[0] <= i.address <= best[1]]
    tests = []
    for i in body:
        if i.mnemonic in ('cmp', 'test') or i.mnemonic.startswith('loop') or (i.mnemonic in ('dec', 'sub') and i.operands):
            tests.append('%08X %s %s' % (i.address, i.mnemonic, i.op_str))
    # where the compared registers come from: the last load of each before the loop
    regs = set()
    for i in body:
        if i.mnemonic == 'cmp':
            for op in i.operands:
                if op.type == X86_OP_REG: regs.add(i.reg_name(op.reg))
    srcs = []
    for i in ins:
        if i.address >= best[0]: break
        if i.mnemonic in ('mov', 'movzx', 'movsx') and len(i.operands) == 2 and i.operands[0].type == X86_OP_REG \
                and i.reg_name(i.operands[0].reg) in regs and i.operands[1].type == X86_OP_MEM:
            srcs.append('%08X %s %s' % (i.address, i.mnemonic, i.op_str))
    mems = ['%08X %s %s' % (i.address, i.mnemonic, i.op_str) for i in body if i.mnemonic == 'cmp' and any(o.type == X86_OP_MEM for o in i.operands)]
    return {'loop': '%08X-%08X' % best, 'tests': tests[:6], 'bound_loads': srcs[-4:], 'mem_compares': mems[:4]}


def main(dll, run, *addrs):
    cases = {c['original']: c for c in json.load(open(os.path.join(run, 'cases.json')))}
    pe = pefile.PE(dll, fast_load=True); base = pe.OPTIONAL_HEADER.ImageBase; img = pe.get_memory_mapped_image()
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True
    res = {}
    for a in addrs:
        c = cases.get('0x%08X' % int(a, 16))
        if not c: continue
        m = DG.GfxMachine(dll, os.path.join(run, 'raw', 'gfx_raw.dll'), c['stubs'], [])
        st = stops(m, c)
        ins = []
        for s, e in ext.get('0x%x' % int(a, 16), []):
            ins += list(md.disasm(img[s - base:e - base], s))
        ins.sort(key=lambda i: i.address); at = {i.address: i for i in ins}
        loops = {}
        for eip, n in st.most_common(3):
            li = loop_info(ins, at, eip)
            loops['0x%08X' % eip] = {'runs': n, 'inside_function': eip in at, **(li or {})}
        res['0x%08X' % int(a, 16)] = {'name': c['name'].split()[0], 'limit_runs_of_40': sum(st.values()), 'stops': loops}
        print(json.dumps(res['0x%08X' % int(a, 16)]), flush=True)
    return res


if __name__ == '__main__':
    main(*sys.argv[1:])
