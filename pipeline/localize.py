"""localize.py -- where does a failing reconstruction first diverge from the original?

Replays a FAIL case's first failing trial on both sides, recording every non-stack memory write
(pc, address, size, value) and every stubbed/COM call (return site, callee, args). The first event
that differs is reported with the original's game address and, for the reconstruction, the C++
source line (the function's translation unit rebuilt with -g, resolved with addr2line).
Usage: python3 localize.py GAME RAWDIR CASES GATE.jsonl OUT.json [limit]
"""
import json, os, random, struct, subprocess, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import diffemu_gfx as DG
G = DG.G
from unicorn import UC_HOOK_MEM_WRITE
from unicorn.x86_const import UC_X86_REG_EIP, UC_X86_REG_ESP

class TraceMachine(DG.GfxMachine):
    def __init__(self, *a, **k):
        super().__init__(*a, **k)
        self.trace = []
        self.uc.hook_add(UC_HOOK_MEM_WRITE, self._trace_write)
    def _trace_write(self, uc, access, addr, size, value, _):
        if G.STACK_TOP - G.STACK_SIZE <= addr < G.STACK_TOP: return
        self.trace.append(('W', uc.reg_read(UC_X86_REG_EIP), addr, size, value & ((1 << (8 * size)) - 1)))
    def _on_stub(self, uc, addr, size, _):
        ret = struct.unpack('<I', uc.mem_read(uc.reg_read(UC_X86_REG_ESP), 4))[0]
        n0 = len(self.calls); super()._on_stub(uc, addr, size, _)
        if len(self.calls) > n0: self.trace.append(('C', ret) + tuple(self.calls[-1]))
    def _on_com(self, uc, addr, size, _):
        ret = struct.unpack('<I', uc.mem_read(uc.reg_read(UC_X86_REG_ESP), 4))[0]
        n0 = len(self.calls); super()._on_com(uc, addr, size, _)
        if len(self.calls) > n0: self.trace.append(('C', ret) + tuple(self.calls[-1]))

def debug_dll(rawdir, ea_hex):
    """rebuild one function's TU with -g (same flags otherwise) into its own DLL"""
    b = 'fn_%s' % ea_hex[2:].upper().zfill(8)
    d = os.path.join(rawdir, 'dbg'); os.makedirs(d, exist_ok=True)
    o = os.path.join(d, b + '.o'); dll = os.path.join(d, b + '.dll')
    flags = ['-O2', '-g', '-mfpmath=387', '-march=i686', '-ffreestanding', '-fno-builtin', '-fno-asynchronous-unwind-tables',
             '-fexcess-precision=standard', '-fno-exceptions', '-fno-rtti', '-std=c++17', '-I' + os.path.dirname(os.path.abspath(__file__)), '-I' + os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data'),
             '-w', '-fno-strict-aliasing', '-fwrapv', '-fno-delete-null-pointer-checks', '-fpermissive']
    if subprocess.run(['i686-w64-mingw32-g++'] + flags + ['-c', os.path.join(rawdir, b + '.cpp'), '-o', o]).returncode: return None
    subprocess.run(['i686-w64-mingw32-g++', '-shared', '-nostdlib', '-o', dll, o, os.path.join(rawdir, 'obj', 'rt.o'), '-lgcc',
                    '-Wl,--image-base=0x60000000', '-Wl,-e,_DllMainCRTStartup@12', '-Wl,--disable-auto-image-base', '-Wl,--kill-at'])
    return dll

def src_line(dll, pc):
    r = subprocess.run(['i686-w64-mingw32-addr2line', '-e', dll, '%x' % pc], capture_output=True, text=True).stdout.strip()
    if ':' not in r or r.startswith('??'): return None
    f, ln = r.rsplit(':', 1)
    try:
        ln = int(ln.split()[0]); return ln, open(f).read().split('\n')[ln - 1].strip()
    except Exception:
        return r, ''

def replay(m, case, trial, seed, orig, cand):
    conv = case.get('conv', 'thiscall'); limit = case.get('insn_limit', 300000)
    fz = G.Fuzzer(m, seed)
    for t in range(trial + 1):
        fz.reset()
        for g, n in case.get('fuzz_ranges', []):
            m.uc.mem_write(int(g, 16), bytes(0 if fz.rng.random() < 0.5 else fz.rng.getrandbits(8) for _ in range(n)))
        for g, spec in case.get('globals', {}).items():
            m.uc.mem_write(int(g, 16), struct.pack('<I', fz.arg(spec) & 0xFFFFFFFF))
        for g, iface in case.get('com_globals', {}).items():
            m.uc.mem_write(int(g, 16), struct.pack('<I', m.obj[iface]))
        for g, sp in case.get('struct_globals', {}).items():
            buf = fz.buffer(int(str(sp['size']), 0))
            for off, iface in sp.get('com', {}).items(): m.uc.mem_write(buf + int(off, 0), struct.pack('<I', m.obj[iface]))
            m.uc.mem_write(int(g, 16), struct.pack('<I', buf))
        this = None
        if conv == 'thiscall':
            vptr = int(case['vptr'], 16) if case.get('vptr') else None
            this = fz.buffer(int(str(case.get('this_size', '0x400')), 0), vptr)
            for off, iface in case.get('this_com', {}).items(): m.uc.mem_write(this + int(off, 0), struct.pack('<I', m.obj[iface]))
        args = [fz.arg(s) for s in case.get('args', [])]
        ts = fz.rng.getrandbits(32)
        if t < trial: continue
        m.page_seed = ts; m.reset_demand(); m.constrain(case.get('input_ranges', []), this, args); m.ctx = m.uc.context_save()
        for _ in range(3):
            n = len(m.demand); m.rng = random.Random(ts); m.wild = 0; m.call(orig, conv, this, args, limit)
            if len(m.demand) == n: break
        out = []
        for fn in (orig, cand):
            m.trace = []; m.rng = random.Random(ts); m.wild = 0
            r = m.call(fn, conv, this, args, limit); out.append((r, list(m.trace)))
        return out

def main(game, rawdir, casesf, gatef, outf, limit=200):
    cases = {c['original']: c for c in json.load(open(casesf))}
    fails = [json.loads(l) for l in open(gatef)]
    fails = [g for g in fails if g['verdict'] == 'FAIL'][:int(limit)]
    res = {}
    for g in fails:
        c = cases[g['original']]; trial = g['first_mismatch']['trial']
        dll = debug_dll(rawdir, g['original'])
        if not dll: res[g['original']] = {'error': 'debug build failed'}; continue
        m = TraceMachine(game, dll, c['stubs'], [])
        cand = m.exports[c['candidate']]
        (a, ta), (b, tb) = replay(m, c, trial, 11, int(c['original'], 16), cand)
        key = lambda e: e[:1] + e[2:]           # compare without the pc
        i = next((k for k, (x, y) in enumerate(zip(ta, tb)) if key(x) != key(y)), min(len(ta), len(tb)))
        ev = lambda t: (None if i >= len(t) else [t[i][0], '0x%08X' % t[i][1]] + [('0x%X' % v if isinstance(v, int) else v) for v in t[i][2:]])
        e_o, e_r = ev(ta), ev(tb)
        line = src_line(dll, tb[i][1]) if i < len(tb) and tb[i][0] == 'W' else (src_line(dll, tb[i][1] - 1) if i < len(tb) else None)
        res[g['original']] = {'name': g['name'], 'why': g['first_mismatch']['why'][:160], 'event_index': i,
                              'original_event': e_o, 'recon_event': e_r, 'recon_line': line,
                              'status': [a.status, b.status], 'events': [len(ta), len(tb)]}
        if b.status in ('fault', 'limit'):     # where the reconstruction crashed or was stopped
            res[g['original']]['recon_stop'] = ['0x%08X' % b.stop_eip, src_line(dll, b.stop_eip), b.detail[:120]]
        if a.status in ('fault', 'limit'):
            res[g['original']]['original_stop'] = ['0x%08X' % a.stop_eip, a.detail[:120]]
        print(g['original'], g['name'][:30], 'event', i, '| recon line', line,
              '| recon stop', res[g['original']].get('recon_stop'), flush=True)
    json.dump(res, open(outf, 'w'), indent=1)

if __name__ == '__main__':
    main(*sys.argv[1:7])
