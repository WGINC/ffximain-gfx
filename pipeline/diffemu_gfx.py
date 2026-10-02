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
# Live mode (FFXI_LIVE=live.json from live_state.py): inputs come from a memory dump of the running game.
LIVE_CFG = json.load(open(os.environ['FFXI_LIVE'])) if os.environ.get('FFXI_LIVE') else None
if LIVE_CFG:
    G.SANDBOX = int(LIVE_CFG['sandbox'], 16); G.POOL = G.SANDBOX + 0x00800000   # a gap the dump does not use
from unicorn import UC_HOOK_CODE, UC_PROT_ALL, UC_HOOK_MEM_READ_UNMAPPED, UC_HOOK_MEM_WRITE_UNMAPPED
import hashlib
import pefile
from unicorn import UC_MEM_READ_UNMAPPED, UC_MEM_WRITE_UNMAPPED, UC_MEM_FETCH_UNMAPPED, UC_MEM_FETCH_PROT, UC_PROT_READ, UC_PROT_WRITE
from unicorn.x86_const import UC_X86_REG_EDX
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX

COM_BASE = 0x6E000000          # fake objects + vtables
COM_TRAP = 0x6E800000          # one 4-byte trap per (interface, slot)
FP_TRAMP = COM_BASE + 0xF0000  # stub return through the x87 stack (inside the COM_BASE mapping)
FP_SCRATCH = COM_BASE + 0xF0100
PREFER = ['IDirect3DTexture8', 'IDirect3DSurface8', 'IDirect3DVertexBuffer8', 'IDirect3DIndexBuffer8',
          'IDirect3DCubeTexture8', 'IDirect3DVolumeTexture8', 'IDirect3DSwapChain8', 'IDirect3DVolume8',
          'IDirect3DDevice8', 'IDirect3D8']
ALLOC_BASE, ALLOC_SIZE = 0x6F000000, 0x800000   # blocks returned by allocator stubs (data/allocators.json)
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

def _bound(v, sh, mk, lo, hi):
    f = (v >> sh) & mk
    return (v & ~(mk << sh) & 0xFFFFFFFF) | (((lo + f % (hi - lo + 1)) & mk) << sh)

class GfxMachine(G.Machine):
    def __init__(self, game_dll, recon_dll, stubs=None, overlays=()):
        super().__init__(game_dll, recon_dll, stubs, overlays)
        u = self.uc
        u.mem_map(COM_BASE, 0x100000, UC_PROT_ALL)
        u.mem_map(COM_TRAP, 0x100000, UC_PROT_ALL)
        u.mem_map(ALLOC_BASE, ALLOC_SIZE, UC_PROT_ALL)
        self.alloc_next = self.alloc_high = ALLOC_BASE
        if not LIVE_CFG: self.live = None
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
        # Execution of data (a call through a fuzzed function pointer lands in a fuzzed page, the pool,
        # .data or the stack) ends the run as a fault: the game never does it, and the fuzzed bytes run
        # as code would act on values that legitimately differ between the two sides (return addresses),
        # so a trial where the original does it is not compared, like any other original fault.
        code = [(COM_TRAP, COM_TRAP + 0x100000), (FP_TRAMP, FP_TRAMP + 0x10),
                (G.TRAP_BASE, G.TRAP_BASE + 0x10000), (G.RET_SENTINEL, G.RET_SENTINEL + G.PAGE)]
        for path in (game_dll, recon_dll):
            if not path: continue
            pe = pefile.PE(path, fast_load=True)
            for sec in pe.sections:
                if sec.Characteristics & 0x20000000:          # IMAGE_SCN_MEM_EXECUTE
                    a = pe.OPTIONAL_HEADER.ImageBase + sec.VirtualAddress
                    code.append((a, a + max(sec.Misc_VirtualSize, sec.SizeOfRawData)))
        self.foreign = []
        if LIVE_CFG:
            # live mode: the dump's other modules are not data (they are called through real objects'
            # vtables); execution there is trapped and answered like a stub (_on_foreign)
            import live_state
            self.live = live_state.LiveState(LIVE_CFG['dump'], LIVE_CFG['build_map'])
            clash = [(G.TRAP_BASE, G.RET_SENTINEL + G.PAGE), (COM_BASE, ALLOC_BASE + ALLOC_SIZE),
                     (G.STACK_TOP - G.STACK_SIZE, G.STACK_TOP), (0x10000000, 0x11000000), (0x60000000, 0x61000000)]
            for fb, fs, fn in self.live.foreign:
                if any(fb < e and fb + fs > s_ for s_, e in clash): continue
                self.foreign.append((fb, fb + fs)); code.append((fb, fb + fs))
                u.hook_add(UC_HOOK_CODE, self._on_foreign, begin=fb, end=fb + fs - 1)
        lo = 0
        for a, b in sorted(code):
            if a > lo: u.hook_add(UC_HOOK_CODE, self._on_data_exec, begin=lo, end=a - 1)
            lo = max(lo, b)
        u.hook_add(UC_HOOK_CODE, self._on_data_exec, begin=lo, end=0xFFFFFFFF)
        # demand paging: an access to unmapped memory above 64 KB maps a page whose bytes are a
        # pure function of (trial seed, page address) -- identical for both runs, no replay needed
        self.demand = []; self.page_seed = 0; self.demand_limit = 96; self.wild_limit = 64; self.wild = 0
        # (fuzzed data stays executable, as in diffemu_fixed: redirecting execution from Unicorn's
        #  fault hook proved non-deterministic between runs, 1 Oct 2026)
        self.rng = random.Random(0)
        if LIVE_CFG:
            for a, hx in LIVE_CFG['data_writes']:
                u.mem_write(int(a, 16), bytes.fromhex(hx))
            self.live_globals = {int(a, 16) for a, _ in LIVE_CFG['data_writes']}
            self.live_this = {int(k, 16): (kind, int(a, 16)) for k, (kind, a) in LIVE_CFG['this_sources'].items()}
            self.live_inst = {int(k, 16): [int(x, 16) for x in v] for k, v in LIVE_CFG['instances'].items()}
            self.trial_no = 0
            self._vt_iface = {}; self.com_guess = 0
            from capstone import Cs, CS_ARCH_X86, CS_MODE_32
            self._md = Cs(CS_ARCH_X86, CS_MODE_32); self._md.detail = True
            self.demand_limit = 4096          # dump pages share the demand list with fuzzed ones
        self.ctx = u.context_save()

    def call(self, *a, **k):
        # each run allocates from the same start, over zeroed memory: both sides get identical blocks
        if self.alloc_high > ALLOC_BASE:
            self.uc.mem_write(ALLOC_BASE, bytes(self.alloc_high - ALLOC_BASE))
        self.alloc_next = self.alloc_high = ALLOC_BASE
        return super().call(*a, **k)

    def _on_data_exec(self, uc, addr, size, _):
        self.stop_reason = 'fault'
        self.stop_detail = 'execution of data at 0x%08X' % addr
        uc.emu_stop()

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
        for a, sh, mk, lo, hi in getattr(self, 'ranges', ()):      # bounded fields inside this page
            if page <= a <= page + G.PAGE - 4:
                v = struct.unpack_from('<I', out, a - page)[0]
                struct.pack_into('<I', out, a - page, _bound(v, sh, mk, lo, hi))
        return bytes(out)

    def constrain(self, entries, this, args):
        # data/input_ranges.json: resolve each entry to an address for this trial; a field already in
        # mapped memory is bounded now, one in a page mapped later is bounded when the page is made
        self.ranges = []
        for e in entries:
            off = int(str(e.get('off', '0')), 0); sh = e.get('shift', 0); mk = int(str(e.get('mask', '0xFFFFFFFF')), 0)
            try:
                if e['where'] == 'global': a = int(e['addr'], 16)
                elif e['where'] == 'global_ptr': a = struct.unpack('<I', self.uc.mem_read(int(e['addr'], 16), 4))[0] + off
                elif e['where'] == 'this': a = (this or 0) + off
                elif e['where'] == 'arg': a = args[e['arg']] + off if e['arg'] < len(args) else 0
                else: continue
            except Exception:
                continue
            if a < 0x10000 or a > 0xFFFFFFF0: continue
            self.ranges.append((a, sh, mk, e['lo'], e['hi']))
            try:
                v = struct.unpack('<I', self.uc.mem_read(a, 4))[0]
                self.uc.mem_write(a, struct.pack('<I', _bound(v, sh, mk, e['lo'], e['hi'])))
            except Exception:
                pass            # not mapped yet: _page_bytes bounds it when the page is made

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
        if self.live and access == UC_MEM_FETCH_UNMAPPED and any(a <= addr < b for a, b in self.foreign):
            # a call into another module: map its page so the code hook (_on_foreign) answers it
            p = addr & ~(G.PAGE - 1)
            if p in self.demand or self.live_map(p): return True
            try: self.uc.mem_map(p, G.PAGE, UC_PROT_ALL); self.demand.append(p); return True
            except Exception: return False
        if self.live and access in (UC_MEM_READ_UNMAPPED, UC_MEM_WRITE_UNMAPPED) and addr >= 0x10000:
            pages = {addr & ~(G.PAGE - 1), (addr + size - 1) & ~(G.PAGE - 1)}
            if all(p in self.demand or self.live.read(p, G.PAGE) is not None for p in pages):
                for p in pages:
                    if p not in self.demand and not self.live_map(p): return False
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

    def _on_foreign(self, uc, addr, size, _):
        # a call into another module of the dump (D3D8 wrapper, polcore, Windows): logged and
        # compared like a stubbed call, answered with a fuzzed EAX, popping what the real function pops
        esp = uc.reg_read(UC_X86_REG_ESP)
        ret_addr = struct.unpack('<I', uc.mem_read(esp, 4))[0]
        t = self._com_target(uc, addr, esp, ret_addr)
        if t is not None:                 # a method of a real D3D8 object: the fake COM trap answers it
            uc.reg_write(UC_X86_REG_EIP, t); return
        n = self.live.pops(addr)
        if os.environ.get('FFXI_LIVE_TRACE'):
            print('foreign %08X pops %d ret %08X esp %08X' % (addr, n, ret_addr, esp), file=sys.stderr)
        args = struct.unpack('<%dI' % min(n // 4, 4), uc.mem_read(esp + 4, 4 * min(n // 4, 4))) if n else ()
        self.calls.append((addr, _stackref(uc.reg_read(UC_X86_REG_ECX)), tuple(_stackref(a) for a in args)))
        k = self.rng.random()
        v = 0 if k < 0.6 else 1 if k < 0.7 else self.rng.randrange(2, 64) if k < 0.85 else self.rng.getrandbits(32)
        uc.reg_write(UC_X86_REG_EAX, v)
        uc.reg_write(UC_X86_REG_ESP, esp + 4 + n)
        uc.reg_write(UC_X86_REG_EIP, ret_addr)

    def _com_target(self, uc, addr, esp, ret_addr):
        # A real D3D8 object's method: `call [reg+slot*4]` with `this` as the first stack argument
        # and this->vtable[slot] == the target. The interface is chosen by the vtable's length
        # (consecutive entries into the same module) among those com_spec gives; candidates of equal
        # length must agree on the slot's parameter count, else the first is taken.
        slot = None
        for L in (2, 3, 6):
            try: code = bytes(uc.mem_read(ret_addr - L, L))
            except Exception: continue
            for i in self._md.disasm(code, ret_addr - L):
                if i.mnemonic == 'call' and i.address + i.size == ret_addr and i.operands[0].type == 3 \
                        and i.operands[0].mem.base and not i.operands[0].mem.index:
                    slot = i.operands[0].mem.disp // 4
                break
            if slot is not None: break
        if slot is None: return None
        def rd(a):                        # mapped memory, else the dump (no demand paging from here)
            try: return struct.unpack('<I', uc.mem_read(a, 4))[0]
            except Exception:
                b = self.live.read(a, 4)
                if b is None: raise ValueError(a)
                return self.live.translate(struct.unpack('<I', b)[0])
        try:
            this = rd(esp + 4); vt = rd(this)
            if rd(vt + 4 * slot) != addr: return None
        except Exception: return None
        if vt not in self._vt_iface:
            mod = next(((a, b) for a, b in self.foreign if a <= addr < b), None)
            n = 0
            while n < 128:
                w = self.live.read(vt + 4 * n, 4)
                if w is None or not (mod[0] <= struct.unpack('<I', w)[0] < mod[1]): break
                n += 1
            cands = [k for k in IFACES if len(SPEC[k]) == n]
            if not cands:                 # vtables sit back to back, so the length can run on: the
                cands = PREFER              # resource interfaces in order of how often the game uses them
                self.com_guess += 1
            self._vt_iface[vt] = cands
        cands = [k for k in self._vt_iface[vt] if slot < len(SPEC[k])]
        if not cands: return None
        name = cands[0]
        return COM_TRAP + 0x1000 * IFACES.index(name) + 4 * slot

    def live_map(self, page):
        # the dump's page (pointers into its FFXiMain translated); unmapped again by reset_demand
        b = self.live.page(page)
        if b is None: return False
        try: self.uc.mem_map(page, G.PAGE, UC_PROT_ALL)
        except Exception: return False
        self.uc.mem_write(page, b); self.demand.append(page)
        return True

    def live_write(self, addr, data):
        for p in range(addr & ~(G.PAGE - 1), addr + len(data), G.PAGE):
            if p not in self.demand:
                try: self.uc.mem_read(p, 1)
                except Exception: self.live_map(p)
        self.uc.mem_write(addr, data)

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
        if spec.get('ret') == 'alloc':         # an allocator: a fresh zeroed block, never NULL
            n = args[spec['size_arg']] if spec.get('size_arg') is not None else 0x1000
            n = (min(n, 0x10000) if 0 < n else 0x1000) + 0xF & ~0xF
            v = self.alloc_next; self.alloc_next += max(n, 0x40)
            if self.alloc_next > ALLOC_BASE + ALLOC_SIZE: v = 0          # exhausted: the call fails
            self.alloc_high = max(self.alloc_high, self.alloc_next)
        elif spec.get('ret') == 'bool':          # the callee only ever returns EAX = 0 or 1 (ret_kinds.py)
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


def setup_trial(m, case, fz):
    """One trial's inputs, exactly as the gate makes them: fuzzed globals, COM objects in globals and
    in `this`, arguments, the demand-page seed and the input ranges. Returns (this, args, trial_seed).
    Diagnostics (localize, loop_bounds) call this so they replay the gate's trials.
    Live mode: globals the dump holds keep their values, struct globals and `this` are the dump's real
    objects (with the fake D3D objects patched in), and the dump's pages replace fuzzed ones."""
    conv = case.get("conv", "thiscall")
    live = getattr(m, 'live', None)
    held = (lambda a: a in m.live_globals) if live else (lambda a: False)
    fz.reset()
    for g, n in case.get("fuzz_ranges", []):
        b = bytes(0 if fz.rng.random() < 0.5 else fz.rng.getrandbits(8) for _ in range(n))
        if not held(int(g, 16)): m.uc.mem_write(int(g, 16), b)
    for g, spec in case.get("globals", {}).items():
        v = fz.arg(spec) & 0xFFFFFFFF
        if not held(int(g, 16)): m.uc.mem_write(int(g, 16), struct.pack("<I", v))
    for g, iface in case.get("com_globals", {}).items():
        m.uc.mem_write(int(g, 16), struct.pack("<I", m.obj[iface]))
    patches = []                      # live mode: writes into dump pages, made after the page reset below
    for g, sp in case.get("struct_globals", {}).items():
        obj = struct.unpack("<I", m.uc.mem_read(int(g, 16), 4))[0] if held(int(g, 16)) else 0
        if obj:
            patches += [(obj + int(off, 0), struct.pack("<I", m.obj[iface])) for off, iface in sp.get("com", {}).items()]
            continue
        buf = fz.buffer(int(str(sp["size"]), 0))
        for off, iface in sp.get("com", {}).items():
            m.uc.mem_write(buf + int(off, 0), struct.pack("<I", m.obj[iface]))
        m.uc.mem_write(int(g, 16), struct.pack("<I", buf))
    this = None
    if conv == "thiscall":
        vptr = int(case["vptr"], 16) if case.get("vptr") else None
        if live:
            inst = m.live_inst.get(vptr) if vptr else None
            src = m.live_this.get(int(case["original"], 16))
            if inst: this = inst[m.trial_no % len(inst)]
            elif src and src[0] == 'global': this = struct.unpack("<I", m.uc.mem_read(src[1], 4))[0] or None
            elif src and src[0] == 'static': this = src[1]
        if this:
            patches += [(this + int(off, 0), struct.pack("<I", m.obj[iface])) for off, iface in case.get("this_com", {}).items()]
        else:
            this = fz.buffer(int(str(case.get("this_size", "0x400")), 0), vptr)
            for off, iface in case.get("this_com", {}).items():
                m.uc.mem_write(this + int(off, 0), struct.pack("<I", m.obj[iface]))
    args = [fz.arg(s) for s in case.get("args", [])]
    trial_seed = fz.rng.getrandbits(32)
    m.page_seed = trial_seed; m.reset_demand()
    for a, b in patches: m.live_write(a, b)
    if live: m.trial_no += 1
    m.constrain(case.get("input_ranges", []), this, args)
    m.ctx = m.uc.context_save()
    return this, args, trial_seed


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
    touched += [(int(e["addr"], 16), 4) for e in case.get("input_ranges", []) if e["where"] == "global"]
    saved = [(a, bytes(m.uc.mem_read(a, n))) for a, n in touched]
    for t in range(trials):
        this, args, trial_seed = setup_trial(m, case, fz)
        a = None
        for _ in range(3):      # discovery: map every page the original touches before measuring
            before = len(m.demand)
            m.rng = random.Random(trial_seed); m.wild = 0; r_ = m.call(orig, conv, this, args, limit)
            if len(m.demand) == before:
                a = r_          # no new page: this run is the measurement run (same seed, same pages)
                break
        if a is None:
            m.rng = random.Random(trial_seed); m.wild = 0; a = m.call(orig, conv, this, args, limit)
        if a.status in ('fault', 'limit'):
            b = None            # not compared (below): the reconstruction need not run
        else:
            m.rng = random.Random(trial_seed); m.wild = 0; b = m.call(cand, conv, this, args, limit)
        demand_pages = len(m.demand); m.reset_demand()
        com_calls += sum(1 for c in a.calls if c[0] >= 0xC0000000)
        statuses[a.status.split(":")[0]] = statuses.get(a.status.split(":")[0], 0) + 1
        if ret == 'word' and b is not None:
            a.eax &= 0xFFFF; b.eax &= 0xFFFF
        fret = case.get('fret')
        if ret == 'float' and b is not None and fret in ('float', 'double') and a.st0 and b.st0 and a.fp_top == b.fp_top \
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
