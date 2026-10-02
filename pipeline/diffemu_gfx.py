"""diffemu_gfx.py -- the equivalence gate (diffemu_fixed.py) extended for graphics code.

Additions, all applied identically to the original and the reconstruction:
  * stub "returns": "fuzz" -- each stubbed call returns the next value of a per-trial random
    sequence (restarted for each run), so both branches after a call get exercised;
  * fake Direct3D 8 objects -- every interface gets a vtable of trap slots. A call is logged
    (interface, method, `this`, arguments) and compared like a stubbed call; it returns a fuzzed
    HRESULT / count and pops its arguments (stdcall). Methods that return an interface through
    an out-pointer write a fake object of that interface there on success, so code that goes on
    to use the texture / surface / buffer keeps being checked;
  * case "com_globals": {"0xADDR": "IDirect3DDevice8"} puts a fake object pointer in a global;
    case "struct_globals": {"0xADDR": {"size": "0x478", "com": {"8": "IDirect3DDevice8"}}}
    puts a fuzzed struct there whose listed fields are fake objects (the D3D holder, CDx).
"""
import json, os, random, struct, sys
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__))))
import diffemu_fixed as G
from unicorn import UC_HOOK_CODE, UC_PROT_ALL, UC_HOOK_MEM_READ_UNMAPPED, UC_HOOK_MEM_WRITE_UNMAPPED
import hashlib
from unicorn import UC_MEM_READ_UNMAPPED, UC_MEM_WRITE_UNMAPPED, UC_MEM_FETCH_UNMAPPED, UC_MEM_FETCH_PROT, UC_PROT_READ, UC_PROT_WRITE
from unicorn.x86_const import UC_X86_REG_EDX
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX

COM_BASE = 0x6E000000          # fake objects + vtables
COM_TRAP = 0x6E800000          # one 4-byte trap per (interface, slot)
FP_TRAMP = COM_BASE + 0xF0000  # stub return through the x87 stack (inside the COM_BASE mapping)
FP_SCRATCH = COM_BASE + 0xF0100
SPEC = json.load(open(os.path.join(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data'), 'com_spec.json')))
IFACES = sorted(SPEC)

STACK_LO, STACK_HI = G.STACK_TOP - G.STACK_SIZE, G.STACK_TOP
def G_stack(v): return _stackref(v)
def _stackref(v):
    """a pointer into the caller's own stack frame: its exact value depends on the frame layout,
    which a reconstruction may legitimately change; compare it as 'some stack address'"""
    return 0x5717C000 if STACK_LO <= v < STACK_HI else v

IMPORT_NARGS = json.load(open(os.path.join(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data'), 'import_nargs.json'))) \
    if os.path.exists(os.path.join(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data'), 'import_nargs.json')) else {}

class GfxMachine(G.Machine):
    def __init__(self, game_dll, recon_dll, stubs=None, overlays=()):
        super().__init__(game_dll, recon_dll, stubs, overlays)
        u = self.uc
        u.mem_map(COM_BASE, 0x100000, UC_PROT_ALL)
        u.mem_map(COM_TRAP, 0x100000, UC_PROT_ALL)
        u.mem_write(COM_TRAP, b'\xF4' * 0x100000)
        u.mem_write(FP_TRAMP, b'\xD9\x05' + struct.pack('<I', FP_SCRATCH) + b'\xC3')
        self.obj = {}; self.trap = {}
        for i, name in enumerate(IFACES):
            vt = COM_BASE + 0x1000 * i
            obj = COM_BASE + 0x80000 + 0x100 * i     # object: vtable pointer + zero padding
            for s in range(len(SPEC[name])):
                t = COM_TRAP + 0x1000 * i + 4 * s
                self.trap[t] = (name, s)
                u.mem_write(vt + 4 * s, struct.pack('<I', t))
            u.mem_write(obj, struct.pack('<I', vt))
            self.obj[name] = obj
        # aliases the spec uses for base interfaces
        self.obj.setdefault('IDirect3DBaseTexture8', self.obj['IDirect3DTexture8'])
        u.hook_add(UC_HOOK_CODE, self._on_com, begin=COM_TRAP, end=COM_TRAP + 0x100000 - 1)
        # demand paging: an access to unmapped memory above 64 KB maps a page whose bytes are a
        # pure function of (trial seed, page address) -- identical for both runs, no replay needed
        self.demand = []; self.page_seed = 0; self.demand_limit = 96; self.wild_limit = 64; self.wild = 0
        # (fuzzed data stays executable, as in diffemu_fixed: redirecting execution from Unicorn's
        #  fault hook proved non-deterministic between runs, 1 Oct 2026)
        self.rng = random.Random(0)
        self.ctx = u.context_save()

    def _on_trap(self, uc, addr, size, _):
        name = self.trap_names.get(addr, hex(addr))
        esp = uc.reg_read(UC_X86_REG_ESP)
        n = IMPORT_NARGS.get(name.split('!')[-1], 4)
        args = struct.unpack('<%dI' % n, uc.mem_read(esp + 4, 4 * n)) if n else ()
        self.stop_reason = 'import:' + name
        self.stop_detail = 'args ' + ' '.join('%08X' % _stackref(a) for a in args)
        uc.emu_stop()

    def _page_bytes(self, page):
        r = random.Random(int.from_bytes(hashlib.md5(b'%d:%d' % (self.page_seed, page)).digest()[:8], 'little'))
        out = bytearray(G.PAGE)
        for off in range(0, G.PAGE, 4):
            k = r.random()
            if k < 0.3: v = 0
            elif k < 0.5: v = r.randrange(1, 16)
            elif k < 0.75: v = G.POOL + (r.randrange(0, G.POOL_SIZE - 0x1000) & ~3)
            else: v = r.getrandbits(32)
            struct.pack_into('<I', out, off, v)
        return bytes(out)

    def _on_invalid(self, uc, access, addr, size, value, _):
        # overrides the base hook.
        # * execution of fuzzed data (a call through a random function pointer): treated as a call
        #   to an unknown cdecl function -- logged with its arguments, answered with a fuzzed EAX,
        #   and returned from; identical for both runs, so equivalent code stays equivalent
        # * unmapped data reads/writes above 64 KB: a demand page
        # * anything else (null-ish pointers, budgets spent): a fault
        if False:   # wild-call trapping disabled (see note in __init__)
            self.wild += 1
            esp = uc.reg_read(UC_X86_REG_ESP)
            try:
                ret_addr, *args = struct.unpack('<5I', uc.mem_read(esp, 20))
            except Exception:
                return super()._on_invalid(uc, access, addr, size, value, _)
            self.calls.append((0xD0000000, addr, tuple(G_stack(a) for a in args[:4])))
            uc.reg_write(UC_X86_REG_EAX, self.rng.getrandbits(32))
            uc.reg_write(UC_X86_REG_ESP, esp + 4)
            uc.reg_write(UC_X86_REG_EIP, ret_addr)
            return True
        if access not in (UC_MEM_READ_UNMAPPED, UC_MEM_WRITE_UNMAPPED) or addr < 0x10000 or \
           len(getattr(self, 'demand', [])) >= getattr(self, 'demand_limit', 0):
            return super()._on_invalid(uc, access, addr, size, value, _)
        for page in {addr & ~(G.PAGE - 1), (addr + size - 1) & ~(G.PAGE - 1)}:
            if page in self.demand: continue
            try:
                uc.mem_map(page, G.PAGE, UC_PROT_ALL)
            except Exception:
                return False
            uc.mem_write(page, self._page_bytes(page)); self.demand.append(page)
        return True

    def reset_demand(self):
        self.wild = 0
        for page in self.demand:
            try: self.uc.mem_unmap(page, G.PAGE)
            except Exception: pass
        self.demand = []

    def seed_run(self, seed):
        self.rng = random.Random(seed)

    def _on_stub(self, uc, addr, size, _):
        spec = self.stubs[addr]
        if spec.get('returns') != 'fuzz':
            return super()._on_stub(uc, addr, size, _)
        esp = uc.reg_read(UC_X86_REG_ESP)
        ret_addr, *args = struct.unpack('<5I', uc.mem_read(esp, 20))
        ecx = uc.reg_read(UC_X86_REG_ECX)
        nargs = spec.get('args', spec.get('pops', 0))
        masks = spec.get('masks') or []
        logged = tuple(a & (masks[i] if i < len(masks) else 0xFFFFFFFF) for i, a in enumerate(args[:nargs]))
        logged = tuple(_stackref(a) for a in logged)
        self.calls.append((addr, _stackref(ecx) if spec.get('conv', 'thiscall') == 'thiscall' else 0, logged))
        k = self.rng.random()
        v = 0 if k < 0.3 else 1 if k < 0.5 else self.rng.randrange(2, 64) if k < 0.7 else self.rng.getrandbits(32)
        if spec.get('ret') == 'bool':          # the callee only ever returns EAX = 0 or 1 (ret_kinds.py)
            v = v & 1
        elif spec.get('ret') == 'lowbool':     # AL = 0 or 1; the upper bits are left over from earlier
            v = (v & ~0xFF) | (v & 1)
        uc.reg_write(UC_X86_REG_EAX, v)
        uc.reg_write(UC_X86_REG_ESP, esp + 4 + 4 * spec.get('pops', 0))
        if spec.get('fpret'):
            # the callee returns in ST0: return through `fld dword [FP_SCRATCH]; ret` so the value
            # really lands on the x87 stack. The trampoline is written once: Unicorn caches
            # translated code, so rewriting it per call could run a stale copy.
            f = (0.0, 1.0, -1.0, 0.5, 2.0, 100.0)[self.rng.randrange(6)] if self.rng.random() < 0.6 else self.rng.uniform(-1000, 1000)
            uc.mem_write(FP_SCRATCH, struct.pack('<f', f))
            sp = esp + 4 * spec.get('pops', 0)          # the trampoline's `ret` pops ret_addr from here
            uc.mem_write(sp, struct.pack('<I', ret_addr))
            uc.reg_write(UC_X86_REG_ESP, sp)
            uc.reg_write(UC_X86_REG_EIP, FP_TRAMP)
            return
        uc.reg_write(UC_X86_REG_EIP, ret_addr)

    def _on_com(self, uc, addr, size, _):
        if addr not in self.trap:
            return
        name, slot = self.trap[addr]
        m = SPEC[name][slot]
        n = m['nparams']
        esp = uc.reg_read(UC_X86_REG_ESP)
        vals = struct.unpack('<%dI' % (2 + n), uc.mem_read(esp, 4 * (2 + n)))
        ret_addr, this, args = vals[0], vals[1], vals[2:]
        self.calls.append((0xC0000000 | (IFACES.index(name) << 8) | slot, this, tuple(_stackref(a) for a in args)))
        if m['ret'] == 'hresult':
            k = self.rng.random()
            v = 0 if k < 0.8 else 0x8876086C if k < 0.9 else 0x88760868   # D3D_OK / INVALIDCALL / DEVICELOST
        else:
            v = self.rng.randrange(0, 4)
        if m['out'] and v == 0:
            idx, iface = m['out']
            p = args[idx]
            try:
                old = bytes(uc.mem_read(p, 4))
                self.writes.append((p, old))      # so Machine.call undoes it like any other write
                uc.mem_write(p, struct.pack('<I', self.obj.get(iface, self.obj['IDirect3DResource8'])))
            except Exception:
                pass
        uc.reg_write(UC_X86_REG_EAX, v)
        uc.reg_write(UC_X86_REG_ESP, esp + 4 + 4 * (1 + n))
        uc.reg_write(UC_X86_REG_EIP, ret_addr)


def run_case(m, case, trials, seed):
    """G.run_case plus: COM placements in globals and in `this`, and one shared random stream
    for stub/COM return values per trial (restarted for each of the two runs)."""
    orig = int(case["original"], 16)
    if case["candidate"] not in m.exports:
        return {"name": case["name"], "original": case["original"], "candidate": case["candidate"],
                "verdict": "ERROR", "reason": "export not found"}
    cand = m.exports[case["candidate"]]
    conv = case.get("conv", "thiscall"); ret = case.get("returns", "int")
    limit = case.get("insn_limit", 300000)
    fz = G.Fuzzer(m, seed)
    statuses, first_fail, fails, com_calls, rounded = {}, None, 0, 0, 0
    # every global this case overwrites is restored afterwards, so cases sharing an emulator never
    # see each other's fuzzed pointers (diffemu_fixed never restored them)
    touched = [(int(g, 16), n) for g, n in case.get("fuzz_ranges", [])]
    touched += [(int(g, 16), 4) for g in list(case.get("globals", {})) + list(case.get("com_globals", {})) + list(case.get("struct_globals", {}))]
    saved = [(a, bytes(m.uc.mem_read(a, n))) for a, n in touched]
    for t in range(trials):
        fz.reset()
        for g, n in case.get("fuzz_ranges", []):
            m.uc.mem_write(int(g, 16), bytes(0 if fz.rng.random() < 0.5 else fz.rng.getrandbits(8) for _ in range(n)))
        for g, spec in case.get("globals", {}).items():
            m.uc.mem_write(int(g, 16), struct.pack("<I", fz.arg(spec) & 0xFFFFFFFF))
        for g, iface in case.get("com_globals", {}).items():
            m.uc.mem_write(int(g, 16), struct.pack("<I", m.obj[iface]))
        for g, sp in case.get("struct_globals", {}).items():
            buf = fz.buffer(int(str(sp["size"]), 0))
            for off, iface in sp.get("com", {}).items():
                m.uc.mem_write(buf + int(off, 0), struct.pack("<I", m.obj[iface]))
            m.uc.mem_write(int(g, 16), struct.pack("<I", buf))
        this = None
        if conv == "thiscall":
            vptr = int(case["vptr"], 16) if case.get("vptr") else None
            this = fz.buffer(int(str(case.get("this_size", "0x400")), 0), vptr)
            for off, iface in case.get("this_com", {}).items():
                m.uc.mem_write(this + int(off, 0), struct.pack("<I", m.obj[iface]))
        args = [fz.arg(s) for s in case.get("args", [])]
        trial_seed = fz.rng.getrandbits(32)
        m.page_seed = trial_seed; m.reset_demand()
        m.ctx = m.uc.context_save()
        for _ in range(3):      # discovery: map every page the original touches before measuring
            before = len(m.demand)
            m.rng = random.Random(trial_seed); m.wild = 0; m.call(orig, conv, this, args, limit)
            if len(m.demand) == before: break
        m.rng = random.Random(trial_seed); m.wild = 0; a = m.call(orig, conv, this, args, limit)
        m.rng = random.Random(trial_seed); m.wild = 0; b = m.call(cand, conv, this, args, limit)
        demand_pages = len(m.demand); m.reset_demand()
        com_calls += sum(1 for c in a.calls if c[0] >= 0xC0000000)
        statuses[a.status.split(":")[0]] = statuses.get(a.status.split(":")[0], 0) + 1
        if ret == 'word':
            a.eax &= 0xFFFF; b.eax &= 0xFFFF
        fret = case.get('fret')
        if ret == 'float' and fret in ('float', 'double') and a.st0 and b.st0 and a.fp_top == b.fp_top \
                and a.st0 != b.st0 and _x87_round(a.st0, fret) == _x87_round(b.st0, fret):
            # the original hands back ST0 unrounded; the reconstruction rounds to the declared return
            # type (C++ requires it). Equal once rounded to that type = equal to any caller that stores
            # the result; counted so the report can say how often only excess precision differed.
            b.st0 = a.st0; rounded += 1
        if a.status in ('fault', 'limit'):
            why = None          # the original crashed or never finished: no defined behaviour to
                                # compare against (the compiler may drop a dead load the original
                                # faulted on, or order independent work differently before a crash).
                                # A reconstruction that crashes where the original returns stays a FAIL.
        else:
            why = G.compare(a, b, 'int' if ret == 'word' else ret)
        if why:
            fails += 1
            if first_fail is None:
                first_fail = {"trial": t, "this": hex(this) if this else None, "args": [hex(x) for x in args], "why": why}
    for a, b_ in saved: m.uc.mem_write(a, b_)
    m.ctx = m.uc.context_save()
    ok_runs = statuses.get("return", 0)
    verdict = "PASS" if fails == 0 and ok_runs > 0 else ("FAIL" if fails else "INCONCLUSIVE")
    return {"name": case["name"], "original": case["original"], "candidate": case["candidate"],
            "trials": trials, "original_outcomes": statuses, "mismatches": fails, "d3d_calls_checked": com_calls,
            "verdict": verdict, "first_mismatch": first_fail, "x87_rounded_trials": rounded}

def _x87_round(st0, kind):
    """80-bit (mantissa, sign|exponent) -> the value rounded to float or double, as bytes"""
    import math
    m, se = st0; e = (se & 0x7FFF); s = -1.0 if se & 0x8000 else 1.0
    if e == 0x7FFF: return ('special', m, se)
    try:
        v = s * math.ldexp(float(m), e - 16383 - 63) if m else 0.0 * s
        return struct.pack('<f' if kind == 'float' else '<d', v)
    except (OverflowError, struct.error):
        return ('overflow', se)

_EXT = json.load(open(os.path.join(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data'), 'func_extents.json'))) \
    if os.path.exists(os.path.join(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data'), 'func_extents.json')) else {}
def _body(o_):
    return _EXT.get(o_, [[int(o_, 16), int(o_, 16) + 1]])
def _inside(addr, ranges):
    return any(s <= addr < e for s, e in ranges)
def batches(cases, size=1):
    """cases sharing an emulator: no case's stubs may land anywhere inside another case's
    function body (not only on its first byte), and stub specs for one address must agree"""
    pending = list(cases)
    while pending:
        chunk, stubs, bodies, rest = [], {}, [], []
        for c in pending:
            cs = {hex(int(a, 16)): s for a, s in (c.get('stubs') or {}).items()}
            o_ = hex(int(c['original'], 16)); mine = _body(o_)
            if len(chunk) >= size or any(_inside(int(a, 16), mine) for a in stubs) or \
               any(_inside(int(a, 16), b) for a in cs for b in bodies) or \
               any(a in stubs and stubs[a] != s for a, s in cs.items()):
                rest.append(c); continue
            chunk.append(c); bodies.append(mine); stubs.update(cs)
        yield chunk, stubs
        pending = rest

def main():
    import argparse
    ap = argparse.ArgumentParser()
    ap.add_argument('game'); ap.add_argument('recon'); ap.add_argument('cases'); ap.add_argument('out')
    ap.add_argument('--trials', type=int, default=100); ap.add_argument('--seed', type=int, default=1)
    ap.add_argument('--only', default=None)
    o = ap.parse_args()
    cases = json.load(open(o.cases))
    if o.only: cases = [c for c in cases if c['original'].lower() in o.only.lower().split(',')]
    done = set()
    if os.path.exists(o.out):
        done = {json.loads(l)['candidate'] for l in open(o.out)}
    cases = [c for c in cases if c['candidate'] not in done]
    f = open(o.out, 'a'); k = 0
    for chunk, stubs in batches(cases):
        m = GfxMachine(o.game, o.recon, stubs, [])
        for c in chunk:
            try:
                r = run_case(m, c, o.trials, o.seed)
            except Exception as e:
                r = {'name': c['name'], 'original': c['original'], 'candidate': c['candidate'],
                     'verdict': 'ERROR', 'reason': repr(e)[:300]}
            f.write(json.dumps(r) + '\n'); f.flush()
        k += len(chunk)
        print('%d/%d' % (k, len(cases)), flush=True)

if __name__ == '__main__':
    main()
