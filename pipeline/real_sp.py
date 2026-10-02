"""real_sp.py -- the real stack depth at each instruction of a function, observed by running it, and how
much each call really changes it.

stack_depth.py derives the depth statically and has to guess how much an indirect call pops (a COM
method, a virtual call through a vtable it cannot see). Here the original runs in the gate's emulator
over the gate's own trials (fuzzed, or live with FFXI_LIVE set), and ESP is read at every executed
instruction of the function: depth = bytes below ESP at entry (the return address excluded). Paths that
never execute have no entry. A conflict (one address seen at two depths) is reported, not hidden.

    python3 real_sp.py RUNDIR OUT.json ADDR [ADDR ...] [--trials N]      uses $FFXI_DLL
OUT.json: {"0xFUNC": {"depth": {"0xADDR": depth}, "calls": {"0xCALL": bytes popped}, "tdepth": {...}}}.
"tdepth" is the depth only on paths where every call before it was real code: where IDA's SP may be synced to it.
"calls" holds only calls whose effect comes from real code: game code that ran, or a stub whose pop count
is the callee's own `ret N` (purges.json). A call answered by a fake COM object or a stub typed from IDA's
prototype is left out: its pop is the model's, not the game's (the fake for an untyped COM pointer is a
generic IDirect3DResource8, whose slot 8 GetPriority pops 4 where the real GetDesc pops 8).
ida/ffxi_ida_fix.py corrects IDA's SP at the listed calls only.
"""
import argparse, json, os, random, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import diffemu_gfx as DG
G = DG.G
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EAX, UC_X86_REG_EBX, UC_X86_REG_ECX, UC_X86_REG_EDX, \
    UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP
import struct
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG
REGS = {'eax': UC_X86_REG_EAX, 'ebx': UC_X86_REG_EBX, 'ecx': UC_X86_REG_ECX, 'edx': UC_X86_REG_EDX,
        'esi': UC_X86_REG_ESI, 'edi': UC_X86_REG_EDI, 'ebp': UC_X86_REG_EBP, 'esp': UC_X86_REG_ESP}
PURGES = json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'purges.json')))
IMAGE = (0x10000000, 0x10BE2000)


def call_insns(m, ext):
    md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True; out = {}
    for a, b in ext:
        for i in md.disasm(bytes(m.uc.mem_read(a, b - a)), a):
            if i.mnemonic == 'call': out[i.address] = i
    return out


def call_target(uc, i):
    op = i.operands[0]
    try:
        if op.type == X86_OP_IMM: return op.imm
        if op.type == X86_OP_REG: return uc.reg_read(REGS[i.reg_name(op.reg)])
        if op.type == X86_OP_MEM and not op.mem.index:
            a = (uc.reg_read(REGS[i.reg_name(op.mem.base)]) if op.mem.base else 0) + op.mem.disp
            return struct.unpack('<I', uc.mem_read(a & 0xFFFFFFFF, 4))[0]
    except Exception:
        pass
    return None


def pushed_since_call(calls, ext, m):
    """{call: bytes pushed since the previous call or stack adjustment, in straight-line code}"""
    md = Cs(CS_ARCH_X86, CS_MODE_32); out = {}
    for a, b in ext:
        n = 0
        for i in md.disasm(bytes(m.uc.mem_read(a, b - a)), a):
            if i.mnemonic == 'push': n += 4
            elif i.mnemonic == 'call': out[i.address] = n; n = 0
            elif i.mnemonic.startswith(('j', 'ret')) or (i.mnemonic in ('add', 'sub', 'pop') and 'esp' in i.op_str) \
                    or i.mnemonic == 'pop': n = 0
    return out


RETS = {'hresult': 'HRESULT', 'ptr': 'void *', 'ulong': 'unsigned int'}


def com_method(m, t):
    """(interface, method spec) when t is a fake COM method whose interface the gate knows for certain;
    IDirect3DResource8 is also the fallback for an untyped COM pointer, so its methods are not trusted"""
    if t not in getattr(m, 'trap', {}): return None
    name, slot = m.trap[t]
    if name in ('IDirect3DResource8', '*wild'): return None      # *wild: input repair's trap (a guessed pop)
    return name, DG.SPEC[name][slot]


def com_decl(name, spec):
    ret = RETS.get(spec['ret'], 'int')
    return '%s __stdcall %s_%s(void *self%s);' % (ret, name, spec['name'], ''.join(', int a%d' % i for i in range(spec['nparams'])))


def trusted(m, t):
    """does a call to t change ESP as the game does?"""
    if com_method(m, t): return True                                 # a fake of a known interface: its spec's pop
    if t is None or not IMAGE[0] <= t < IMAGE[1]: return False      # untyped fake COM, foreign module, trap
    spec = m.stubs.get(t)
    if spec is None: return True                                     # game code that runs for real
    real = PURGES.get('0x%08X' % t)
    return real is not None and spec.get('pops', 0) * 4 == real


def observe(m, case, ext, trials):
    conv = case.get('conv', 'thiscall'); limit = case.get('insn_limit', 300000)
    ea = int(case['original'], 16)
    seen, conflicts, st = {}, {}, {'esp0': None, 'taint': False}
    tseen, tconf = {}, {}               # depths on paths where every call so far was real code (trusted)
    calls = call_insns(m, ext); trust = {}; targets = {}

    def hook(uc, addr, size, _):
        esp = uc.reg_read(UC_X86_REG_ESP)
        if addr == ea and st['esp0'] is None: st['esp0'] = esp + 4    # after the call pushed its return address
        if st['esp0'] is None: return
        d = st['esp0'] - 4 - esp
        if not 0 <= d < 0x10000: return                              # a recursive or nested activation
        if addr == ea and d == 0: st['taint'] = False                # each run of the trial starts clean
        if not st['taint']:
            if addr in tseen and tseen[addr] != d: tconf.setdefault(addr, set()).update({tseen[addr], d})
            else: tseen[addr] = d
        if addr in calls:
            t = call_target(uc, calls[addr]); ok = trusted(m, t)
            trust[addr] = trust.get(addr, True) and ok
            if ok: targets.setdefault(addr, set()).add(t)
            else: st['taint'] = True                                 # the depth after it is the model's
        if addr in seen and seen[addr] != d: conflicts.setdefault(addr, set()).update({seen[addr], d})
        else: seen[addr] = d
    hooks = [m.uc.hook_add(UC_HOOK_CODE, hook, begin=a, end=b - 1) for a, b in ext]
    fz = G.Fuzzer(m, 11)
    for _ in range(trials):
        this, args, ts = DG.setup_trial(m, case, fz)
        st['esp0'] = None
        _, patches = DG.run_original(m, ea, conv, this, args, limit, ts)     # the gate's inputs, repaired as the gate does
        DG.restore_inputs(m, patches); m.reset_demand()
    for h in hooks: m.uc.hook_del(h)
    observe.trusted_depth = {k: v for k, v in tseen.items() if k not in tconf}
    return seen, conflicts, calls, trust, targets


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('run'); ap.add_argument('out'); ap.add_argument('addrs', nargs='+')
    ap.add_argument('--trials', type=int, default=100)
    a = ap.parse_args()
    cases = {c['original']: c for c in json.load(open(os.path.join(a.run, 'cases.json')))}
    exts = json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'func_extents.json')))
    res = json.load(open(a.out)) if os.path.exists(a.out) else {}
    for x in a.addrs:
        key = '0x%08X' % int(x, 16); c = cases.get(key)
        if c is None: print('%s: no gate case (does not compile)' % key); continue
        m = DG.GfxMachine(os.environ['FFXI_DLL'], os.path.join(a.run, 'raw', 'gfx_raw.dll'), c['stubs'], [])
        seen, conflicts, calls, trust, targets = observe(m, c, exts['0x%x' % int(x, 16)], a.trials)
        depth = {k: v for k, v in seen.items() if k not in conflicts}                      # one depth only
        pops = {}
        for k, i in calls.items():
            n = k + i.size
            if trust.get(k) and k in depth and n in depth: pops['0x%08X' % k] = depth[k] - depth[n]
        # indirect calls where the bytes pushed since the previous call (what Hex-Rays takes as the
        # arguments when the call has no type) differ from what the callee really pops, with the callees seen
        guess = pushed_since_call(calls, exts['0x%x' % int(x, 16)], m)
        forced = {}
        for k, i in calls.items():
            ck = '0x%08X' % k
            if ck not in pops or i.operands[0].type == X86_OP_IMM or k not in guess or guess[k] == pops[ck] or k not in targets: continue
            coms = [com_method(m, t) for t in targets[k]]
            if all(coms):
                # a COM method: forced only where Hex-Rays over-assigns (more pushed than popped), since a
                # typed lpVtbl call already has the right count and would lose its names
                if guess[k] < pops[ck] or len({c[1]['name'] for c in coms}) != 1: continue
                forced[ck] = {'pops': pops[ck], 'pushed': guess[k], 'targets': [], 'decl': com_decl(*coms[0])}
            elif not any(coms):
                forced[ck] = {'pops': pops[ck], 'pushed': guess[k], 'targets': ['0x%08X' % t for t in sorted(targets[k])]}
        res[key] = {'depth': {'0x%08X' % k: v for k, v in sorted(depth.items())}, 'calls': pops, 'call_types': forced,
                    'tdepth': {'0x%08X' % k: v for k, v in sorted(observe.trusted_depth.items())}}
        print('%s: %d instructions observed, %d conflicts %s' % (key, len(seen), len(conflicts),
              {'0x%08X' % k: sorted(v) for k, v in list(conflicts.items())[:5]}))
    json.dump(res, open(a.out, 'w'), indent=0)


if __name__ == '__main__':
    main()
