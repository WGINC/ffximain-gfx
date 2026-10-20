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
import bisect, json, os, random, re, struct, sys, time
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__))))
import diffemu_fixed as G
# Live mode (FFXI_LIVE=live.json from live_state.py): inputs come from a memory dump of the running game.
LIVE_CFG = json.load(open(os.environ['FFXI_LIVE'])) if os.environ.get('FFXI_LIVE') else None
if LIVE_CFG:
    G.SANDBOX = int(LIVE_CFG['sandbox'], 16); G.POOL = G.SANDBOX + 0x00800000   # a gap the dump does not use
from unicorn.x86_const import UC_X86_REG_FS, UC_X86_REG_GDTR, UC_X86_REG_CS, UC_X86_REG_SS, UC_X86_REG_DS, UC_X86_REG_ES, UC_X86_REG_GS
from unicorn import UC_HOOK_CODE, UC_HOOK_MEM_READ, UC_HOOK_MEM_WRITE, UC_PROT_ALL, UC_HOOK_MEM_READ_UNMAPPED, UC_HOOK_MEM_WRITE_UNMAPPED
import hashlib
import pefile
from unicorn import UC_MEM_READ_UNMAPPED, UC_MEM_WRITE_UNMAPPED, UC_MEM_FETCH_UNMAPPED, UC_MEM_FETCH_PROT, UC_PROT_READ, UC_PROT_WRITE
from unicorn.x86_const import UC_X86_REG_EDX
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX

COM_BASE = 0x6E000000          # fake objects + vtables
COM_TRAP = 0x6E800000          # one 4-byte trap per (interface, slot)
WILD_TRAP = COM_TRAP + 0x80000  # input repair: one trap per popped-argument count
FP_TRAMP = COM_BASE + 0xF0000  # stub return through the x87 stack (inside the COM_BASE mapping)
FP_SCRATCH = COM_BASE + 0xF0100
# D3D8 getters that fill a struct: method -> (parameter index, size in bytes, 'f' floats / 'i' integers)
STRUCT_OUT = {'GetTransform': (1, 64, 'f'), 'GetMaterial': (0, 68, 'f'), 'GetLight': (1, 104, 'f'),
              'GetClipPlane': (1, 16, 'f'), 'GetViewport': (0, 24, 'i'), 'GetRenderState': (1, 4, 'i'),
              'GetTextureStageState': (2, 4, 'i'), 'GetLightEnable': (1, 4, 'i'), 'GetVertexShader': (0, 4, 'i'),
              'GetPixelShader': (0, 4, 'i'), 'GetDisplayMode': (0, 16, 'i'), 'GetCreationParameters': (0, 16, 'i'),
              'GetRasterStatus': (0, 8, 'i'), 'GetClipStatus': (0, 8, 'i'), 'GetCurrentTexturePalette': (0, 4, 'i'),
              'GetDesc': (0, 32, 'i'), 'GetLevelDesc': (1, 32, 'i')}
# LockRect's output (D3DLOCKED_RECT *): (interface, method) -> its parameter index after `this` (d3d8.h prototypes:
# IDirect3DSurface8::LockRect(pLockedRect, ...), IDirect3DTexture8::LockRect(Level, pLockedRect, ...),
# IDirect3DCubeTexture8::LockRect(FaceType, Level, pLockedRect, ...))
LOCKED_RECT_OUT = {('IDirect3DSurface8', 'LockRect'): 0, ('IDirect3DTexture8', 'LockRect'): 1,
                   ('IDirect3DCubeTexture8', 'LockRect'): 2}
# D3D8 methods that read data through a pointer parameter (the D3D8 SDK header): method -> function of the parameters
# giving (parameter index, bytes read). The bytes are compared, hashed, with the call: before 6 Oct only the pointer was
# (as "a stack address"), so a reconstruction that never wrote the data it passes compared equal
def _prim_verts(t, n):
    return {1: n, 2: 2 * n, 3: n + 1, 4: 3 * n, 5: n + 2, 6: n + 2}.get(t, 0)
IN_DATA = {'SetTransform': lambda a: [(1, 64)], 'MultiplyTransform': lambda a: [(1, 64)], 'SetViewport': lambda a: [(0, 24)],
           'SetMaterial': lambda a: [(0, 68)], 'SetLight': lambda a: [(1, 104)], 'SetClipPlane': lambda a: [(1, 16)],
           'SetVertexShaderConstant': lambda a: [(1, 16 * a[2])], 'SetPixelShaderConstant': lambda a: [(1, 16 * a[2])],
           'DrawPrimitiveUP': lambda a: [(2, _prim_verts(a[0], a[1]) * a[3])],
           'DrawIndexedPrimitiveUP': lambda a: [(4, _prim_verts(a[0], a[3]) * (2 if a[5] == 101 else 4)), (6, (a[1] + a[2]) * a[7])]}
# game functions stubbed in the gate that pass a pointer parameter on to one of those methods unchanged: callee -> the same
# rule over the stub's own arguments. CDx::DrawPrimitiveUP 0x1000CD00 pushes its 4 arguments to
# IDirect3DDevice8::DrawPrimitiveUP (vtable +0x120) at 0x1000CD53 and 0x1000CD96 (its own code)
STUB_DATA = {0x1000CD00: lambda a: [(2, _prim_verts(a[0], a[1]) * a[3])]}


def _data_hash(uc, p, n):
    """the bytes a callee reads through a pointer, hashed for the call log; -1 when they cannot be read"""
    import zlib
    if not p or n <= 0: return 0
    if n > 0x100000: return -2
    try: return zlib.crc32(bytes(uc.mem_read(p, n)))
    except Exception: return -1


PREFER = ['IDirect3DTexture8', 'IDirect3DSurface8', 'IDirect3DVertexBuffer8', 'IDirect3DIndexBuffer8',
          'IDirect3DCubeTexture8', 'IDirect3DVolumeTexture8', 'IDirect3DSwapChain8', 'IDirect3DVolume8',
          'IDirect3DDevice8', 'IDirect3D8']
ALLOC_BASE, ALLOC_SIZE = 0x6F000000, 0x800000   # blocks returned by allocator stubs (data/allocators.json)
SPEC = json.load(open(os.path.join(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data'), 'com_spec.json')))
import com_vtables as COMVT
_COM_VTABLES = json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'com_vtables.json'))) \
    if os.path.exists(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'com_vtables.json')) else {}
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

_EXEC_RANGES, _LIVE = {}, []


def _exec_ranges(path):
    """the executable sections of a PE file as address ranges, read once per process (per file version)"""
    st = os.stat(path); key = (os.path.abspath(path), st.st_size, st.st_mtime_ns)
    if key not in _EXEC_RANGES:
        pe = pefile.PE(path, fast_load=True); r = []
        for sec in pe.sections:
            if sec.Characteristics & 0x20000000:          # IMAGE_SCN_MEM_EXECUTE
                a = pe.OPTIONAL_HEADER.ImageBase + sec.VirtualAddress
                r.append((a, a + max(sec.Misc_VirtualSize, sec.SizeOfRawData)))
        _EXEC_RANGES[key] = r
    return list(_EXEC_RANGES[key])


def _live_state():
    """the dump, opened once per process: LiveState only reads it (its one cache, the pops of foreign code it
    has decoded, is a function of the dump's bytes), so every case of the process shares it"""
    if not _LIVE:
        if LIVE_CFG['dump'].endswith('.xcap'):          # a call captured in the game (xikit capture=, xcap.py)
            import xcap
            _LIVE.append(xcap.CaptureState(LIVE_CFG['dump']))
        else:
            import live_state
            _LIVE.append(live_state.LiveState(LIVE_CFG['dump'], LIVE_CFG['build_map']))
    return _LIVE[0]


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
        self.com_sites = {} if os.environ.get('FFXI_COM_SITES') else None
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
        # input repair (run_original): a function pointer slot of a fuzzed object aims here, at the trap
        # for the number of argument dwords the call site's callee pops
        for k in range(64 * 33): self.trap[WILD_TRAP + 4 * k] = ('*wild', k)    # k = pops | thiscall << 5 | (logged args + 1) << 6
        # Execution of data (a call through a fuzzed function pointer lands in a fuzzed page, the pool,
        # .data or the stack) ends the run as a fault: the game never does it, and the fuzzed bytes run
        # as code would act on values that legitimately differ between the two sides (return addresses),
        # so a trial where the original does it is not compared, like any other original fault.
        code = [(COM_TRAP, COM_TRAP + 0x100000), (FP_TRAMP, FP_TRAMP + 0x10),
                (G.TRAP_BASE, G.TRAP_BASE + 0x10000), (G.RET_SENTINEL, G.RET_SENTINEL + G.PAGE)]
        for path in (game_dll, recon_dll):
            if path: code += _exec_ranges(path)
        self.foreign = []
        if LIVE_CFG:
            # live mode: the dump's other modules are not data (they are called through real objects'
            # vtables); execution there is trapped and answered like a stub (_on_foreign)
            self.live = _live_state()
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
        self.stub_n = {}; self.stub_ret = {}; self.src_log = None; self.import_n = {}; self.site_conflicts = {}; self.tls = {}; self.files = {}; self.last_error = None
        if LIVE_CFG:
            for a, hx in LIVE_CFG['data_writes']:
                u.mem_write(int(a, 16), bytes.fromhex(hx))
            self.live_globals = {int(a, 16) for a, _ in LIVE_CFG['data_writes']}
            self.live_this = {int(k, 16): (kind, int(a, 16)) for k, (kind, a) in LIVE_CFG['this_sources'].items()}
            self.live_inst = {int(k, 16): [int(x, 16) for x in v] for k, v in LIVE_CFG['instances'].items()}
            self.trial_no = 0
            self._vt_iface = {}
            from capstone import Cs, CS_ARCH_X86, CS_MODE_32
            self._md = Cs(CS_ARCH_X86, CS_MODE_32); self._md.detail = True
            self.demand_limit = 4096          # dump pages share the demand list with fuzzed ones
        # synthetic (fuzzed) pages have their own budget in both modes: a trial that wanders through thousands
        # of random pages is not comparable either way (it ended at the instruction limit after ~18 s of page
        # mapping on the dumps: 0x1006E970 spent ~1000 s per dump gate this way); the dumps' own pages keep
        # the larger demand_limit
        self.synth_limit = 96; self.synth = 0
        # a thread information block at FS, as Windows lays it out: the SEH prologue of MSVC code reads and
        # writes fs:[0] (ExceptionList; here the end-of-chain marker) and restores it before it returns
        u.mem_map(TEB, G.PAGE, UC_PROT_ALL); u.mem_map(GDT, G.PAGE, UC_PROT_ALL)
        u.mem_write(TEB, struct.pack('<7I', 0xFFFFFFFF, G.STACK_TOP, G.STACK_TOP - G.STACK_SIZE, 0, 0, 0, TEB))
        # a flat GDT as Windows has it (once a GDT exists every segment is loaded from it): 1 code and 2 data,
        # base 0, 4 GB, 32-bit; 3 the thread block for FS. All ring 0, the level the emulator runs at.
        def desc(base, lim, access, flags):
            return (lim & 0xFFFF) | ((base & 0xFFFFFF) << 16) | (access << 40) | (((lim >> 16) & 0xF) << 48) | (flags << 52) | ((base >> 24) << 56)
        u.mem_write(GDT + 8, struct.pack('<3Q', desc(0, 0xFFFFF, 0x9B, 0xC), desc(0, 0xFFFFF, 0x93, 0xC), desc(TEB, 0xFFF, 0x93, 0x4)))
        u.reg_write(UC_X86_REG_GDTR, (0, GDT, 4 * 8 - 1, 0x0))
        for reg, sel in ((UC_X86_REG_CS, 1), (UC_X86_REG_SS, 2), (UC_X86_REG_DS, 2), (UC_X86_REG_ES, 2), (UC_X86_REG_GS, 2), (UC_X86_REG_FS, 3)):
            u.reg_write(reg, sel << 3)
        self.ctx = u.context_save()

    def alloc(self, n):
        """a fresh zeroed block from the per-run allocator (the same address on both sides); 0 when exhausted"""
        n = (min(n, 0x10000) if 0 < n else 0x1000) + 0xF & ~0xF
        v = self.alloc_next; self.alloc_next += max(n, 0x40)
        if self.alloc_next > ALLOC_BASE + ALLOC_SIZE: return 0
        self.alloc_high = max(self.alloc_high, self.alloc_next)
        return v

    def call(self, *a, **k):
        # each run allocates from the same start, over zeroed memory: both sides get identical blocks
        if self.alloc_high > ALLOC_BASE:
            self.uc.mem_write(ALLOC_BASE, bytes(self.alloc_high - ALLOC_BASE))
        self.alloc_next = self.alloc_high = ALLOC_BASE
        self.stub_n = {}                  # calls made to each stub so far in this run (input repair keys)
        self.fault_ret = None             # the return address on the stack when execution left the code
        self.tls = {}                     # the emulated thread's TLS slots (TlsSetValue / TlsGetValue)
        self.files = {}                   # files the run created: handle -> [position, size]
        self.last_error = None            # the thread's last-error value once the run sets it
        self.key_n = {}                   # declared mode: identical calls so far in this run (_keyed)
        return super().call(*a, **k)

    def _keyed(self, call):
        """declared mode (declared.py): the answer to this call comes from the call itself -- the trial, its target
        and arguments, and how many identical calls came before it in this run -- so a call the change drops or adds
        does not move the answers of the calls after it. Off (the default), the run's one stream answers in order."""
        if not getattr(self, 'keyed', False): return
        h = hashlib.sha1(repr(call).encode()).digest()
        c = self.key_n.get(h, 0); self.key_n[h] = c + 1
        self.rng = random.Random(h + struct.pack('<QI', self.key_seed & 0xFFFFFFFFFFFFFFFF, c))

    def _on_data_exec(self, uc, addr, size, _):
        self.fault_ret = struct.unpack('<I', uc.mem_read(uc.reg_read(UC_X86_REG_ESP), 4))[0]
        self.stop_reason = 'fault'
        self.stop_detail = 'execution of data at 0x%08X' % addr
        uc.emu_stop()

    def _on_trap(self, uc, addr, size, _):
        name = self.trap_names.get(addr, hex(addr))
        esp = uc.reg_read(UC_X86_REG_ESP)
        n = IMPORT_NARGS.get(name.split('!')[-1])
        if n is None:                     # not in data/import_nargs.json (IDA's SDK prototypes): the call site's
            site = site_pops(self, struct.unpack('<I', uc.mem_read(esp, 4))[0])     # definitive count, which is
            if site: self.import_n[name] = site[2]          # the import's own, so the reconstruction's site reuses it
            n = site[2] if site else self.import_n.get(name)
            if n is None:                 # no definitive count anywhere: stop rather than read a guessed number
                self.stop_reason = 'unresolved'
                self.stop_detail = 'import %s: argument count unknown (not in import_nargs.json, call site not solved)' % name
                uc.emu_stop(); return
        args = struct.unpack('<%dI' % n, uc.mem_read(esp + 4, 4 * n)) if n else ()
        fake = IMPORT_FAKES.get(name.split('!')[-1])
        if fake and len(args) == fake[0]:
            # a benign query answered the same way on both sides (logged, compared like a stubbed call),
            # so a function that only asks Windows something on its way does not stop there
            self.calls.append((addr, 0, tuple(_stackref(a) for a in args))); self._keyed(self.calls[-1])
            try: v = fake[1](self, uc, args)
            except Exception: v = 0
            uc.reg_write(UC_X86_REG_EAX, v & 0xFFFFFFFF)
            ret = struct.unpack('<I', uc.mem_read(esp, 4))[0]
            uc.reg_write(UC_X86_REG_ESP, esp + 4 + 4 * fake[0]); uc.reg_write(UC_X86_REG_EIP, ret)
            return
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
        if access in (UC_MEM_FETCH_UNMAPPED, UC_MEM_FETCH_PROT):
            try: self.fault_ret = struct.unpack('<I', uc.mem_read(uc.reg_read(UC_X86_REG_ESP), 4))[0]
            except Exception: pass
        if False:   # wild-call trapping disabled (see note in __init__)
            self.wild += 1
            esp = uc.reg_read(UC_X86_REG_ESP)
            try:
                ret_addr, *args = struct.unpack('<5I', uc.mem_read(esp, 20))
            except Exception:
                return super()._on_invalid(uc, access, addr, size, value, _)
            self.calls.append((0xD0000000, addr, tuple(G_stack(a) for a in args[:4]))); self._keyed(self.calls[-1])
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
           len(getattr(self, 'demand', [])) >= getattr(self, 'demand_limit', 0) or \
           getattr(self, 'synth', 0) >= getattr(self, 'synth_limit', 1 << 30):
            return super()._on_invalid(uc, access, addr, size, value, _)
        for page in {addr & ~(G.PAGE - 1), (addr + size - 1) & ~(G.PAGE - 1)}:
            if page in self.demand: continue
            try:
                uc.mem_map(page, G.PAGE, UC_PROT_ALL)
            except Exception:
                return False
            uc.mem_write(page, self._page_bytes(page)); self.demand.append(page); self.synth += 1
        return True

    def _on_foreign(self, uc, addr, size, _):
        # a call into another module of the dump (D3D8 wrapper, polcore, Windows): logged and
        # compared like a stubbed call, answered with a fuzzed EAX, popping what the real function pops
        esp = uc.reg_read(UC_X86_REG_ESP)
        ret_addr = struct.unpack('<I', uc.mem_read(esp, 4))[0]
        self._last_com = None
        t = self._com_target(uc, addr, esp, ret_addr)
        if self.com_sites is not None and IMAGE_LO <= ret_addr < IMAGE_HI:
            # record only (render_map.py): the original's call site, the callee, and the object's vtable, slot
            # and address when the call went through one; nothing here changes the run
            vt, slot, this = self._last_com or (None, None, None)
            self._record_site((ret_addr, addr, vt, slot), this, esp + 4 if vt is not None else esp, 3)
        if t is not None:                 # a method of a real D3D8 object: the fake COM trap answers it
            uc.reg_write(UC_X86_REG_EIP, t); return
        # what the callee pops: the call site's definitive count (site_pops), as for any other call; with none,
        # the run stops as unresolved (no `ret N` read off foreign code, which would be a guess)
        site = site_pops(self, ret_addr) if IMAGE_LO <= ret_addr < IMAGE_HI else None
        if site is None:
            last = getattr(self, 'last_site', None) if IMAGE_LO <= ret_addr < IMAGE_HI else None   # set by site_pops
            self.stop_reason = 'unresolved'
            self.stop_detail = 'call %s into %s: pop count unknown%s' % (
                last or 'returning to 0x%08X' % ret_addr, self.live.module_of(addr),
                (' (sources disagree: %s)' % self.site_conflicts[last]) if last in self.site_conflicts else '')
            uc.emu_stop(); return
        n = 4 * site[0]
        if os.environ.get('FFXI_LIVE_TRACE'):
            print('foreign %08X pops %d ret %08X esp %08X' % (addr, n, ret_addr, esp), file=sys.stderr)
        args = struct.unpack('<%dI' % min(n // 4, MAX_STUB_ARGS), uc.mem_read(esp + 4, 4 * min(n // 4, MAX_STUB_ARGS))) if n else ()
        self.calls.append((addr, _stackref(uc.reg_read(UC_X86_REG_ECX)), tuple(_stackref(a) for a in args))); self._keyed(self.calls[-1])
        k = self.rng.random()
        v = 0 if k < 0.6 else 1 if k < 0.7 else self.rng.randrange(2, 64) if k < 0.85 else self.rng.getrandbits(32)
        uc.reg_write(UC_X86_REG_EAX, v)
        uc.reg_write(UC_X86_REG_ESP, esp + 4 + n)
        uc.reg_write(UC_X86_REG_EIP, ret_addr)

    def _record_site(self, k, this, argp, nargs):
        # render_map.py: per call site, how often it ran, the objects it ran on and up to 8 distinct tuples of
        # its first arguments (the dwords after `this`); read from the stack, nothing written
        r = self.com_sites.get(k)
        if r is None: r = self.com_sites[k] = [0, set(), set()]
        r[0] += 1
        if len(r[1]) < 8: r[1].add(this)
        if len(r[2]) < 8 and nargs:
            try: r[2].add(struct.unpack('<%dI' % nargs, self.uc.mem_read(argp + 4, 4 * nargs)))
            except Exception: pass

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
        def rd(a):                        # mapped memory, else the dump (no demand paging from here)
            try: return struct.unpack('<I', uc.mem_read(a, 4))[0]
            except Exception:
                b = self.live.read(a, 4)
                if b is None: raise ValueError(a)
                return self.live.translate(struct.unpack('<I', b)[0])
        try:
            this = rd(esp + 4); vt = rd(this)
            if slot is None or rd(vt + 4 * slot) != addr:
                # the call went through a register (GCC: `call eax`): the slot is where the vtable holds it
                slot = next((k for k in range(128) if rd(vt + 4 * k) == addr), None)
                if slot is None: return None
        except Exception: return None
        self._last_com = (vt, slot, this)
        if vt not in self._vt_iface:
            # recorded in the running game (com_vtables.py): an object a Create method returned, whose interface the
            # D3D8 header declares, had this vtable with exactly these entries
            named = None
            if _COM_VTABLES:
                rel = lambda a: next(('%s+0x%X' % (n, a - b) for b, sz, n in self.live.foreign if b <= a < b + sz), None)
                vname = rel(vt)
                r = _COM_VTABLES.get(vname.lower()) if vname else None
                if r and not r.get('conflict'):
                    try: ents = [rel(rd(vt + 4 * k)) or '?' for k in range(len(r['entries']))]
                    except Exception: ents = None
                    if ents is not None: named = COMVT.lookup(_COM_VTABLES, vname, ents)
            if named:
                self._vt_iface[vt] = [named]
        if vt not in self._vt_iface:
            mod = next(((a, b) for a, b in self.foreign if a <= addr < b), None)
            n = 0
            while n < 128:
                w = self.live.read(vt + 4 * n, 4)
                if w is None or not (mod[0] <= struct.unpack('<I', w)[0] < mod[1]): break
                n += 1
            # vtables sit back to back, so a run of n entries into the module can be one interface's vtable
            # followed by the next: every interface no longer than the run is a candidate
            self._vt_iface[vt] = [k for k in IFACES if len(SPEC[k]) <= n]
        cands = [k for k in self._vt_iface[vt] if slot < len(SPEC[k])]
        if not cands: return None
        if any(SPEC[k][slot] != SPEC[cands[0]][slot] for k in cands):
            return None                   # the candidates differ at this slot: not a known method (site_pops decides)
        name = cands[0]                   # all candidates have this very method: any of them answers it the same
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
        self.wild = 0; self.synth = 0
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
        nargs = spec.get('args', spec.get('pops', 0))
        k = max(4, min(nargs, MAX_STUB_ARGS))           # the return address and the arguments logged
        ret_addr, *args = struct.unpack('<%dI' % (k + 1), uc.mem_read(esp, 4 * (k + 1)))
        ecx = uc.reg_read(UC_X86_REG_ECX)
        masks = spec.get('masks') or []
        logged = tuple(a & (masks[i] if i < len(masks) else 0xFFFFFFFF) for i, a in enumerate(args[:nargs]))
        logged = tuple(_stackref(a) for a in logged)
        if addr in STUB_DATA and len(args) >= 4:
            logged += tuple(_data_hash(uc, args[i], sz) for i, sz in STUB_DATA[addr](args))
        self.calls.append((addr, _stackref(ecx) if spec.get('conv', 'thiscall') == 'thiscall' or spec.get('ecx') else 0, logged)); self._keyed(self.calls[-1])
        k = self.rng.random()
        v = 0 if k < 0.3 else 1 if k < 0.5 else self.rng.randrange(2, 64) if k < 0.7 else self.rng.getrandbits(32)
        d = getattr(self, 'dict', ())
        if d and self.rng.random() < 0.125: v = d[self.rng.randrange(len(d))]    # one of the function's constants
        if spec.get('ret') == 'alloc':         # an allocator: a fresh zeroed block, never NULL
            v = self.alloc(args[spec['size_arg']] if spec.get('size_arg') is not None else 0x1000)
        elif spec.get('ret') == 'bool':          # the callee only ever returns EAX = 0 or 1 (ret_kinds.py)
            v = v & 1
        elif spec.get('ret') == 'lowbool':     # AL = 0 or 1; the upper bits are left over from earlier
            v = (v & ~0xFF) | (v & 1)
        n = self.stub_n.get(addr, 0); self.stub_n[addr] = n + 1
        if (addr, n) in self.stub_ret: v = self.stub_ret[(addr, n)]       # input repair (run_original)
        if self.src_log is not None: self.src_log.append(('stub', (addr, n), v))
        for idx, iface in spec.get('com_out', ()):
            # an `I...**` output (D3DXAssembleShader's ID3DXBuffer **): on success the callee stores an
            # interface pointer there, which the caller then calls through -- a fake object of that kind
            if v & 0x80000000 or iface not in self.obj: continue
            try:
                p = struct.unpack('<I', uc.mem_read(esp + 4 + 4 * idx, 4))[0]
                if p:
                    self.writes.append((p, bytes(uc.mem_read(p, 4))))
                    uc.mem_write(p, struct.pack('<I', self.obj[iface]))
            except Exception:
                pass
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
        if name == '*wild':
            # a virtual call on a fuzzed object (input repair): logged with `this` (a __thiscall site) and its
            # arguments when their count is known (data/callsite_args.json, from Hex-Rays), answered with a
            # fuzzed EAX, popping what the call site's callee pops. An argument pointing into the caller's
            # stack (an output) gets a fuzzed dword, the same on both sides, so neither reads its own
            # uninitialised local.
            esp = uc.reg_read(UC_X86_REG_ESP)
            pops, this_, nlog = slot & 31, (slot >> 5) & 1, (slot >> 6) - 1
            n = max(pops, nlog, 0)
            vals = struct.unpack('<%dI' % (1 + n), uc.mem_read(esp, 4 * (1 + n)))
            ecx = _stackref(uc.reg_read(UC_X86_REG_ECX)) if this_ else 0       # a stack-`this` call leaves ECX undefined
            self.calls.append((0xD0000000 | pops, ecx, tuple(_stackref(a) for a in vals[1:1 + max(nlog, 0)]))); self._keyed(self.calls[-1])
            for a in vals[1:1 + (nlog if nlog >= 0 else pops)]:
                if esp < a < esp + 0x800:     # into the caller's own frame: an output it reads back
                    k = self.rng.random()
                    o = 0 if k < 0.2 else G.POOL + (self.rng.randrange(0, G.POOL_SIZE - 0x1000) & ~3) if k < 0.8 else self.rng.getrandbits(32)
                    self.writes.append((a, bytes(uc.mem_read(a, 4)))); uc.mem_write(a, struct.pack('<I', o))
            k = self.rng.random()
            v = 0 if k < 0.3 else 1 if k < 0.5 else self.rng.randrange(2, 64) if k < 0.7 else self.rng.getrandbits(32)
            n = self.stub_n.get(addr, 0); self.stub_n[addr] = n + 1
            v = self.stub_ret.get((addr, n), v)
            if self.src_log is not None: self.src_log.append(('stub', (addr, n), v))
            uc.reg_write(UC_X86_REG_EAX, v)
            uc.reg_write(UC_X86_REG_ESP, esp + 4 + 4 * pops)
            uc.reg_write(UC_X86_REG_EIP, vals[0])
            return
        m = SPEC[name][slot]
        n = m['nparams']
        esp = uc.reg_read(UC_X86_REG_ESP)
        vals = struct.unpack('<%dI' % (2 + n), uc.mem_read(esp, 4 * (2 + n)))
        ret_addr, this, args = vals[0], vals[1], vals[2:]
        if self.com_sites is not None and IMAGE_LO <= ret_addr < IMAGE_HI:
            self._record_site((ret_addr, 'fake', name, slot), this, esp + 4, min(n, 3))   # record only
        data = tuple(_data_hash(uc, args[i], sz) for i, sz in IN_DATA[m['name']](args)) if m['name'] in IN_DATA else ()
        self.calls.append((0xC0000000 | (IFACES.index(name) << 8) | slot, this, tuple(_stackref(a) for a in args) + data)); self._keyed(self.calls[-1])
        if m['ret'] == 'hresult':
            k = self.rng.random()
            v = 0 if k < 0.8 else 0x8876086C if k < 0.9 else 0x88760868   # D3D_OK / INVALIDCALL / DEVICELOST
        else:
            v = self.rng.randrange(0, 4)
        n_ = self.stub_n.get(addr, 0); self.stub_n[addr] = n_ + 1
        if m['ret'] != 'hresult':         # a pointer or count result: repairable like a stub's (run_original)
            v = self.stub_ret.get((addr, n_), v)
            if self.src_log is not None: self.src_log.append(('stub', (addr, n_), v))
        if m['out'] and v == 0:
            idx, iface = m['out']
            p = args[idx]
            try:
                old = bytes(uc.mem_read(p, 4))
                self.writes.append((p, old))      # so Machine.call undoes it like any other write
                uc.mem_write(p, struct.pack('<I', self.obj.get(iface, self.obj['IDirect3DResource8'])))
            except Exception:
                pass
        so = STRUCT_OUT.get(m['name'])
        if so and (m['ret'] != 'hresult' or v == 0):
            # a getter's struct output: filled from the trial's random stream (the same on both sides),
            # so neither side reads its own uninitialised stack (GetTransform's matrix)
            idx, size, kind = so
            p = args[idx] if idx < len(args) else 0
            words = []
            for _ in range(size // 4):
                if kind == 'f': words.append(struct.unpack('<I', struct.pack('<f', (0.0, 1.0, -1.0, 0.5)[self.rng.randrange(4)] if self.rng.random() < 0.5 else self.rng.uniform(-4.0, 4.0)))[0])
                else: words.append(self.rng.randrange(0, 4) if self.rng.random() < 0.7 else self.rng.getrandbits(32))
            try:
                self.writes.append((p, bytes(uc.mem_read(p, size))))
                uc.mem_write(p, struct.pack('<%dI' % (size // 4), *words))
            except Exception:
                pass
        lo = LOCKED_RECT_OUT.get((name, m['name']))
        if lo is not None and v == 0:
            # LockRect's D3DLOCKED_RECT {INT Pitch; void *pBits} (d3d8types.h): readable pool memory, so the code that
            # reads the locked pixels runs. Drawn from a stream of its own (trial seed, call number), so functions that
            # never lock see the trial stream they always did; both sides get the same values for the same call
            r = random.Random(int.from_bytes(hashlib.md5(b'lock:%d:%d' % (self.page_seed, len(self.calls))).digest()[:8], 'little'))
            pitch = 4 * r.randrange(1, 0x101)
            bits = G.POOL + (r.randrange(0, G.POOL_SIZE - 0x20 * pitch - 0x1000) & ~3)
            p = args[lo] if lo < len(args) else 0
            try:
                self.writes.append((p, bytes(uc.mem_read(p, 8))))
                uc.mem_write(p, struct.pack('<iI', pitch, bits))
            except Exception:
                pass
        uc.reg_write(UC_X86_REG_EAX, v)
        uc.reg_write(UC_X86_REG_ESP, esp + 4 + 4 * (1 + n))
        uc.reg_write(UC_X86_REG_EIP, ret_addr)


def _fake_curdir(m, uc, args):
    path = b'C:\\PlayOnline\\SquareEnix\\FINAL FANTASY XI'
    if args[0] > len(path):
        m.writes.append((args[1], bytes(uc.mem_read(args[1], len(path) + 1))))
        uc.mem_write(args[1], path + b'\0'); return len(path)
    return len(path) + 1


def _fake_tlsset(m, uc, a):
    m.tls[a[0]] = a[1]; return 1


def _fake_release_sem(m, uc, a):
    if a[2]: m.writes.append((a[2], bytes(uc.mem_read(a[2], 4)))); uc.mem_write(a[2], struct.pack('<I', m.rng.randrange(0, 4)))
    return 1 if m.rng.random() < 0.9 else 0


def _fake_writefile(m, uc, a):
    if a[3]: m.writes.append((a[3], bytes(uc.mem_read(a[3], 4)))); uc.mem_write(a[3], struct.pack('<I', a[2]))
    f = m.files.setdefault(a[0], [0, 0]); f[0] += a[2]; f[1] = max(f[1], f[0])       # [position, size]
    return 1


def _fake_setfilepointer(m, uc, a):
    # the file CreateFile made is new and empty; position and size follow the writes made to it
    f = m.files.setdefault(a[0], [0, 0]); dist = struct.unpack('<i', struct.pack('<I', a[1]))[0]
    if a[2]: return 0xFFFFFFFF                                   # a 64-bit move: not modelled, reported as failure
    f[0] = max(0, {0: 0, 1: f[0], 2: f[1]}.get(a[3], 0) + dist); return f[0]


IMPORT_FAKES = {   # name: (argument count, answer); stdcall: the fake pops the arguments
    'GetCurrentDirectoryA': (2, _fake_curdir),
    'GetTickCount': (0, lambda m, uc, a: 0x00123456),
    'timeGetTime': (0, lambda m, uc, a: 0x00123456),
    'LoadCursorFromFileA': (1, lambda m, uc, a: 0 if m.rng.random() < 0.3 else 0x00C10000 | m.rng.randrange(0x100)),
    'SetCursor': (1, lambda m, uc, a: 0),
    'ShowCursor': (1, lambda m, uc, a: 0),
    'DestroyCursor': (1, lambda m, uc, a: 1),
    # another thread's doing: an input, drawn from the values Win32 documents for it (WAIT_OBJECT_0,
    # WAIT_ABANDONED, WAIT_FAILED; WAIT_TIMEOUT only with a finite timeout), the same on both sides
    # the process heap: a block from the gate's per-run allocator, identical on both sides (its contents are
    # unspecified without HEAP_ZERO_MEMORY; zero is one instance of that); HeapFree reports success
    'HeapAlloc': (3, lambda m, uc, a: m.alloc(a[2]) if a[2] <= 0x10000 else 0),
    'HeapFree': (3, lambda m, uc, a: 1),
    # VOID Sleep(DWORD) (Windows SDK): the thread yields; nothing is returned or written. The calls are compared
    # (SkinAndDrawV2 sleeps 0 ms and retries after a failed Lock)
    'Sleep': (1, lambda m, uc, a: 0),
    # void, and in a run with one thread entering always acquires: the calls are compared, nothing returned
    'EnterCriticalSection': (1, lambda m, uc, a: 0),
    'LeaveCriticalSection': (1, lambda m, uc, a: 0),
    'InitializeCriticalSection': (1, lambda m, uc, a: 0),
    'DeleteCriticalSection': (1, lambda m, uc, a: 0),
    # BOOL results: an input drawn from the documented TRUE / FALSE, the same on both sides
    'CloseHandle': (1, lambda m, uc, a: 1 if m.rng.random() < 0.9 else 0),
    'SetEvent': (1, lambda m, uc, a: 1 if m.rng.random() < 0.9 else 0),
    'ResetEvent': (1, lambda m, uc, a: 1 if m.rng.random() < 0.9 else 0),
    # files: CreateFile either fails (INVALID_HANDLE_VALUE) or gives a handle, an input like the rest; WriteFile
    # succeeds and stores the count it was asked to write (the only success Win32 documents for a disk file)
    'CreateFileA': (7, lambda m, uc, a: 0x0000F11E if m.rng.random() < 0.5 else 0xFFFFFFFF),
    'WriteFile': (5, _fake_writefile),
    'GetFileType': (1, lambda m, uc, a: 1 if a[0] == 0x0000F11E else 0),      # FILE_TYPE_DISK for the file it opened
    # after a failure: a Win32 error code, an input drawn from the ones CreateFile documents for a path
    'GetLastError': (0, lambda m, uc, a: m.last_error if m.last_error is not None else m.rng.choice([2, 3, 5, 32, 112])),
    'SetLastError': (1, lambda m, uc, a: setattr(m, 'last_error', a[0]) or 0),
    # thread-local storage of the one emulated thread, fresh each run: unset slots read 0 (documented)
    'TlsGetValue': (1, lambda m, uc, a: m.tls.get(a[0], 0)),
    'TlsSetValue': (2, _fake_tlsset),
    'ReleaseSemaphore': (3, _fake_release_sem),
    'SetFilePointer': (4, _fake_setfilepointer),
    'CreateDirectoryA': (2, lambda m, uc, a: 1 if m.rng.random() < 0.5 else 0),   # BOOL; failure (e.g. it exists) is documented
    'GetCurrentThreadId': (0, lambda m, uc, a: 0x00000F0C),   # the one emulated thread's id (any fixed id)
    'WaitForSingleObject': (2, lambda m, uc, a: m.rng.choice([0, 0x80, 0xFFFFFFFF] + ([0x102] if a[1] != 0xFFFFFFFF else []))),
}


REPAIR_SPAN = 0x100000            # the top MB of the sandbox's object half: buffers that input repair hands out
TEB = 0x7F7F0000                # the emulated thread's information block (FS points here)
GDT = 0x7F7E0000                # a GDT holding FS's descriptor (32-bit mode takes segment bases from it)
DATA_RW = (0x1034F000, 0x109FA8E0)   # the image's writable data (.data, .data1)
IMAGE_LO, IMAGE_HI = 0x10001000, 0x10329000   # the image's code


def _repairable(m, a):
    """may an input repair overwrite the dword at a? fuzzed objects and the pool, demand (or dump)
    pages and the image's writable data -- never the stack, allocator blocks, fake COM objects or code"""
    if G.SANDBOX <= a < G.SANDBOX + G.SANDBOX_SIZE or DATA_RW[0] <= a < DATA_RW[1]: return True
    return (a & ~(G.PAGE - 1)) in m.demand


_BLOCKS = []


def site_pops(m, ret):
    """What the callee of the call returning to `ret` pops, from definitive sources only:
      data/sp_solved.json   solved from the caller's own stack balance (sp_solve.py, self-tested);
      data/real_sp.json     measured by running real game code (real_sp.py, trusted calls only);
      data/game_sp.json     measured in the running game itself (xikit facts=1, fact_sites.py ingest: ESP after
                            the call minus ESP before it, recorded only where every observed call agreed);
      data/vcalls.json      a virtual call on `this` whose every possible callee is proven (vcall_resolve.py);
      data/com_calls.json   a D3D8 method on an object proven to hold that interface (com_calls.py);
      data/callsite_args.json  the call's declared type in the IDB ('typed', not a type Hex-Rays made up).
    Returns (stack dwords popped, ECX carries `this`, dwords to compare) or None when no source has the
    call, or the sources disagree (recorded in m.site_conflicts). Nothing is guessed: a None stops the run
    as unresolved (run_original), naming the call site."""
    from capstone import CS_ARCH_X86 as _A, CS_MODE_32 as _M, Cs as _Cs
    if not _BLOCKS:
        ex = json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'func_extents.json')))
        _BLOCKS.extend(sorted((a, b, sorted(map(tuple, v))) for v in ex.values() for a, b in v))
        _STARTS.extend(x[0] for x in _BLOCKS); _MAXLEN[0] = max(b - a for a, b, _ in _BLOCKS)
    site_pops_init()
    # the function's instructions, decoded once (the image's code is never written by the gate)
    # the first piece in sorted order that contains `ret` (as a linear scan would find it): only pieces
    # starting within the longest piece's length below `ret` can contain it
    j = bisect.bisect_left(_STARTS, ret); hit = None
    while j > 0 and _STARTS[j - 1] >= ret - _MAXLEN[0]:
        j -= 1
        if _BLOCKS[j][0] < ret <= _BLOCKS[j][1]: hit = _BLOCKS[j]
    if hit is None: return None
    key = tuple(map(tuple, hit[2]))
    ends = _ENDS.get(key)
    if ends is None:
        ends = _ENDS[key] = {}
        for a, b in hit[2]:
            for i in _Cs(_A, _M).disasm(bytes(m.uc.mem_read(a, b - a)), a): ends[i.address + i.size] = i
    call = ends.get(ret)
    m.last_site = None
    if call is None or call.mnemonic != 'call': return None
    k = '0x%08X' % call.address; m.last_site = k
    if getattr(m, 'dep_sites', None) is not None: m.dep_sites.add(k)       # gate_cache: a dependency of this run
    typed = _CALLSITES['typed'].get(k)
    srcs = {}
    if k in _CALLSITES['solved']: srcs['solved'] = _CALLSITES['solved'][k]
    if k in _CALLSITES['real']: srcs['real'] = _CALLSITES['real'][k]
    if k in _CALLSITES['game']: srcs['game'] = _CALLSITES['game'][k]
    if k in _CALLSITES['vtable']: srcs['vtable'] = _CALLSITES['vtable'][k]
    if k in _CALLSITES['com']: srcs['com'] = _CALLSITES['com'][k]
    if typed: srcs['typed'] = 0 if typed['cc'] == 'cdecl' else 4 * typed['args']
    if len(set(srcs.values())) != 1:
        if srcs: m.site_conflicts[k] = srcs
        return None
    pops = next(iter(srcs.values()))
    if pops % 4: return None
    # ECX carries `this` where the IDB declares __thiscall, where the original's own code enters the callee with
    # ECX = the object whose vtable it calls through, not also pushed as an argument (data/ecx_this_sites.json),
    # or where the call is direct to a function data/type_fixes.json corrects to __thiscall (each entry names the
    # instructions that read or pass on ECX): the reconstruction must then pass that object in ECX too.
    # Otherwise ECX is not compared (no claim either way)
    direct = int(call.op_str, 16) if re.fullmatch(r'0x[0-9a-f]+', call.op_str) else None
    this = (bool(typed) and typed['cc'] == 'thiscall') or k in _CALLSITES['ecx_this'] or direct in _CALLSITES['thiscall_fix']
    return pops // 4, this, (typed['args'] if typed else pops // 4)


_CALLSITES = {}
# a stub logs at most this many stack arguments (16 dwords: more than any callee in the graphics set takes;
# it was 4 before 5 Oct, which left the fifth argument of CXiMovie_DrawFrame uncompared)
MAX_STUB_ARGS = 16


def site_pops_init():
    if not _CALLSITES:
        d = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
        ld = lambda f: json.load(open(os.path.join(d, f))) if os.path.exists(os.path.join(d, f)) else {}
        _CALLSITES['typed'] = {k: v for k, v in ld('callsite_args.json').items() if v.get('src') == 'typed'}
        _CALLSITES['ecx_this'] = {k for k, v in ld('ecx_this_sites.json').items()
                                  if not k.startswith('_') and v['ecx'] and not v['pushed']}
        _CALLSITES['thiscall_fix'] = {int(k, 16) for k, v in ld('type_fixes.json').items()
                                      if re.search(r'__thiscall\s*\(', v['type'])}
        _CALLSITES['solved'] = {k: v['pops'] for k, v in ld('sp_solved.json').items() if not k.startswith('_')}
        _CALLSITES['real'] = {c: p for r in ld('real_sp.json').values() for c, p in r.get('calls', {}).items()}
        _CALLSITES['game'] = {k: v['pops'] for k, v in ld('game_sp.json').items() if not k.startswith('_')}
        _CALLSITES['vtable'] = {k: v['pops'] for k, v in ld('vcalls.json').items() if not k.startswith('_')}
        _CALLSITES['com'] = {k: v['pops'] for k, v in ld('com_calls.json').items()}
_STARTS = []
_MAXLEN = [0]
_ENDS = {}               # function pieces -> {end address of an instruction: instruction}
_MD = []
_R32 = {'al': 'eax', 'ah': 'eax', 'ax': 'eax', 'bl': 'ebx', 'bh': 'ebx', 'bx': 'ebx', 'cl': 'ecx', 'ch': 'ecx', 'cx': 'ecx',
        'dl': 'edx', 'dh': 'edx', 'dx': 'edx', 'si': 'esi', 'di': 'edi', 'bp': 'ebp', 'sp': 'esp'}


def _insn(m, pc):
    if not _MD:
        from capstone import Cs as _Cs, CS_ARCH_X86 as _A, CS_MODE_32 as _M
        md = _Cs(_A, _M); md.detail = True; _MD.extend([md, {}])
    md, cache = _MD
    if pc not in cache:
        try: cache[pc] = next(md.disasm(bytes(m.uc.mem_read(pc, 16)), pc), None)
        except Exception: cache[pc] = None
    return cache[pc]


def _base_reg(i):
    """the base register of an instruction's memory operand"""
    from capstone.x86 import X86_OP_MEM as _MEM
    for o in i.operands:
        if o.type == _MEM and o.mem.base:
            return _R32.get(i.reg_name(o.mem.base), i.reg_name(o.mem.base))
    return None


def _tb_none(why):
    if os.environ.get('XI_REPAIR_DEBUG'): print('trace_back: gives up (%d)' % why)
    return None


def _trace_back(m, log, k, reg, bad=None):
    """the input a register's value at log position k came from, following it back through moves,
    `lea`/add/sub of the same register, stack stores and reloads, and stub returns: ('mem', addr, value)
    for a dword of input memory, ('stub', key, value) for a stub's return, or None"""
    from capstone.x86 import X86_OP_REG as _REG, X86_OP_MEM as _MEM, X86_OP_IMM as _IMM
    adjusted = False
    def near(v): return adjusted or bad is None or 0 <= bad - v < 0x10000   # the value the pointer was made from
    for _ in range(4000):
        k -= 1
        if k < 0: return _tb_none(1)
        e = log[k]
        if e[0] == 'stub' and reg == 'eax': return e if near(e[2]) else _tb_none(14)
        if e[0] != 'pc': continue
        if e[1] in m.stubs:               # a stubbed callee (its code never runs): EAX is its return
            if reg == 'eax':
                st = next((x for x in log[max(0, k - 2):k + 4] if x[0] == 'stub'), None)
                return st if st else _tb_none(12)
            if reg in ('ecx', 'edx'): return _tb_none(13)
            continue
        i = _insn(m, e[1])
        if i is None: return _tb_none(2)
        ops = i.operands
        if not ops or ops[0].type != _REG or _R32.get(i.reg_name(ops[0].reg), i.reg_name(ops[0].reg)) != reg:
            if i.mnemonic == 'call' and reg == 'eax':   # a trap's return (fake COM object, virtual-call trap)
                st = next((x for x in log[k + 1:k + 4] if x[0] == 'stub'), None)
                if st: return st
            if i.mnemonic == 'call' and reg in ('eax', 'ecx', 'edx'):
                if os.environ.get('XI_REPAIR_DEBUG'): print('trace_back: %s clobbered by call at 0x%X' % (reg, i.address))
                return _tb_none(3)                   # clobbered by a real callee
            if i.mnemonic in ('xchg', 'cdq', 'mul', 'div', 'lodsd', 'rep movsd', 'popal'): return _tb_none(4)
            continue
        mn = i.mnemonic
        if mn in ('cmp', 'test', 'push'): continue
        reads = [x for x in log[k + 1:k + 4] if x[0] == 'mem']
        if mn in ('mov', 'pop') and (mn == 'pop' or ops[1].type == _MEM):
            if not reads:
                if os.environ.get('XI_REPAIR_DEBUG'): print('trace_back: no load logged for 0x%X %s %s; next %s' % (i.address, mn, i.op_str, log[k + 1:k + 4]))
                return _tb_none(5)
            _, a, v = reads[0]
            if STACK_LO <= a < STACK_HI:       # a reload of a stack slot: who stored it
                w = next((j for j in range(k - 1, -1, -1) if log[j][0] == 'w' and log[j][1] == a), None)
                if w is None:                  # never stored: one of the function's own arguments, or uninitialised
                    i0 = (a - (G.STACK_TOP - 0x1000 - 4 * len(m.cur_args))) // 4
                    return ('arg', i0, v) if 0 <= i0 < len(m.cur_args) and a % 4 == 0 else _tb_none(10)
                p = next((j for j in range(w - 1, -1, -1) if log[j][0] == 'pc'), None)
                si = _insn(m, log[p][1]) if p is not None else None
                if si is None: return _tb_none(6)
                if si.mnemonic == 'push' and si.operands[0].type == _REG: src = si.operands[0]
                elif si.mnemonic == 'mov' and len(si.operands) == 2 and si.operands[1].type == _REG: src = si.operands[1]
                else: return _tb_none(7)
                k = p; reg = _R32.get(si.reg_name(src.reg), si.reg_name(src.reg)); continue
            if os.environ.get('XI_REPAIR_DEBUG') and not _repairable(m, a): print('trace_back: load from 0x%X not repairable' % a)
            if not near(v): return _tb_none(15)      # not the value the faulting pointer has: a step not modelled
            return ('mem', a, v) if _repairable(m, a) else _tb_none(11)
        if mn == 'mov' and ops[1].type == _REG:
            reg = _R32.get(i.reg_name(ops[1].reg), i.reg_name(ops[1].reg)); continue
        if mn == 'lea' and ops[1].type == _MEM and ops[1].mem.base and not ops[1].mem.index:
            reg = _R32.get(i.reg_name(ops[1].mem.base), i.reg_name(ops[1].mem.base)); adjusted = True; continue
        if mn in ('add', 'sub', 'and', 'or', 'shl', 'shr', 'sar', 'inc', 'dec') and (len(ops) < 2 or ops[1].type == _IMM):
            adjusted = True; continue         # the same register, adjusted
        if os.environ.get('XI_REPAIR_DEBUG'): print('trace_back: stops at 0x%X %s %s' % (i.address, mn, i.op_str))
        return _tb_none(8)
    return _tb_none(9)


def _fuzz_block(rng, size):
    data = bytearray(size)
    for off in range(0, size, 4):
        k = rng.random()
        v = 0 if k < 0.25 else rng.randrange(1, 16) if k < 0.45 else \
            G.POOL + (rng.randrange(0, G.POOL_SIZE - 0x1000) & ~3) if k < 0.7 else rng.getrandbits(32)
        struct.pack_into('<I', data, off, v)
    return bytes(data)


def run_original(m, orig, conv, this, args, limit, ts, repairs=16, debug=False):
    """The original's measurement run for one trial, with the inputs made to reach the code.
    Discovery: every page it touches is mapped first. Input repair: when the original faults through a
    pointer that came from its inputs, that input is fixed and the trial runs again:
      * a read or write near address 0 through a fuzzed null or small integer: the dword it was loaded
        from (or the stub call that returned it) is set to a fresh buffer of the same fuzzed mix;
      * a call through a fuzzed function pointer: the slot is set to a trap that logs the call and pops
        what the call site's callee pops (site_pops; the count nearest it that lets the original carry on,
        when the pushes before the call also hold the next call's arguments).
    Both sides then see the repaired inputs: the repairs stay in place until restore_inputs (after the
    reconstruction's run). Deterministic in the trial seed, and uses no fuzzer randomness (replays stay
    aligned). Returns (result, patches)."""
    import re as _re
    patches = []; rng = random.Random(ts * 7919 + 1); top = G.SANDBOX + 0x700000; tried = set()
    m.cur_args = args
    def run():
        if getattr(m, 'cov_run', None) is not None: m.cov_run.clear()    # coverage: this run's instructions only
        m.rng = random.Random(ts); m.key_seed = ts; m.wild = 0; return m.call(orig, conv, this, args, limit)
    for _ in range(3 + repairs):
        before = len(m.demand); r = run()
        if len(m.demand) != before: continue
        if r.status != 'fault' or len(patches) >= repairs: return r, patches
        f = _re.match(r'access 0x([0-9A-F]+) size \d+ type (19|20|21)$|execution of data at 0x([0-9A-F]+)$', r.detail or '')
        if not f: return r, patches
        bad = int(f.group(1) or f.group(3), 16); fetch = f.group(2) not in ('19', '20')
        site = None
        if fetch:
            ret = m.fault_ret; site = site_pops(m, ret) if ret and IMAGE_LO <= ret < IMAGE_HI else None
            if site is None or site[0] >= 32 or site[2] >= 32:
                if ret and IMAGE_LO <= ret < IMAGE_HI:    # a fuzzed method pointer, but what its callee pops is unknown
                    k = getattr(m, 'last_site', None)
                    r.status = 'unresolved'; r.detail = 'call %s: pop count unknown%s' % (
                        k or 'returning to 0x%08X' % ret, (' (sources disagree: %s)' % m.site_conflicts[k]) if k in m.site_conflicts else '')
                return r, patches
        elif bad >= 0x10000: return r, patches
        log = m.src_log = []              # the run again, logging executed instructions, dword loads, stack stores, stub returns
        def cd(uc, addr, size, _): log.append(('pc', addr))
        def rd(uc, access, addr, size, value, _):
            if size == 4: log.append(('mem', addr, struct.unpack('<I', uc.mem_read(addr, 4))[0]))
        def wr(uc, access, addr, size, value, _):
            if size == 4 and STACK_LO <= addr < STACK_HI: log.append(('w', addr, value & 0xFFFFFFFF))
        hs = [m.uc.hook_add(UC_HOOK_CODE, cd, begin=0x10000000, end=IMAGE_HI - 1),
              m.uc.hook_add(UC_HOOK_MEM_READ, rd), m.uc.hook_add(UC_HOOK_MEM_WRITE, wr)]
        run()
        for h in hs: m.uc.hook_del(h)
        m.src_log = None
        last = max((j for j, e in enumerate(log) if e[0] == 'pc'), default=None)
        src = None
        if last is not None and fetch:    # the call read its target from the slot: the load logged after it
            src = next((e for e in log[last + 1:] if e[0] == 'mem' and e[2] == bad and _repairable(m, e[1])), None)
        elif last is not None:
            reg = _base_reg(_insn(m, log[last][1])) if _insn(m, log[last][1]) else None
            src = _trace_back(m, log, last, reg, bad) if reg and reg != 'esp' else None
        if debug: print('repair: %s bad=0x%X site=%s src=%s' % (r.detail, bad, site, src))
        if src is None or (src[0], src[1]) in tried: return r, patches
        tried.add((src[0], src[1]))
        if fetch:
            if src[0] != 'mem': return r, patches
            a = src[1]; patches.append((a, bytes(m.uc.mem_read(a, 4))))
            # the pop count and the arguments to compare: definitive (site_pops), never searched for
            tag = (int(site[1]) << 5) | ((site[2] + 1) << 6)
            if debug: print('repair: trap at 0x%X pops %d, compares %d' % (a, site[0], site[2]))
            m.uc.mem_write(a, struct.pack('<I', WILD_TRAP + 4 * (site[0] | tag))); continue
        off = bad - src[2]                # the faulting offset from the pointer (unknown if adjusted on the way)
        size = ((off & ~0xFFF) + 0x1000) if 0 <= off < 0x10000 else 0x2000
        if top + size > G.SANDBOX + 0x700000 + REPAIR_SPAN: return r, patches
        buf = top; top += size
        m.uc.mem_write(buf, _fuzz_block(rng, size))
        if src[0] == 'stub':              # a stub's return value: that call (by its count) returns the buffer
            m.stub_ret[src[1]] = buf; patches.append(('stub', src[1]))
        elif src[0] == 'arg':             # an argument: both sides are called with the repaired list
            args[src[1]] = buf
        else:
            patches.append((src[1], bytes(m.uc.mem_read(src[1], 4))))
            m.uc.mem_write(src[1], struct.pack('<I', buf))
    return r, patches


def restore_inputs(m, patches):
    for a, old in reversed(patches):
        if a == 'stub': m.stub_ret.pop(old, None); continue
        try: m.uc.mem_write(a, old)
        except Exception: pass


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
        if fz.dict and n >= 1 and fz.rng.random() < 0.25:     # one of the function's constants, at an aligned offset
            at = 4 * fz.rng.randrange((n + 3) // 4); v = struct.pack('<I', fz.dict[fz.rng.randrange(len(fz.dict))])
            b = b[:at] + v[:n - at] + b[at + 4:]
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
    rp = case.get("replay")                 # a captured call (xcap.py): its own `this` and arguments, nothing drawn
    if rp:
        this = int(rp["this"], 16) if rp.get("this") else None
        if this: patches += [(this + int(off, 0), struct.pack("<I", m.obj[iface])) for off, iface in case.get("this_com", {}).items()]
    elif conv == "thiscall":
        vptr = int(case["vptr"], 16) if case.get("vptr") else None
        if live:
            inst = m.live_inst.get(vptr) if vptr else None
            src = m.live_this.get(int(case["original"], 16))
            if inst: this = inst[m.trial_no % len(inst)]
            elif src and src[0] == 'global': this = struct.unpack("<I", m.uc.mem_read(src[1], 4))[0] or None
            elif src and src[0] == 'static': this = src[1]
            elif src and src[0] == 'vtable':               # an instance of a vtable holding the method, from the dump
                objs = m.live_inst.get(src[1]) or []
                this = objs[m.trial_no % len(objs)] if objs else None
        if this:
            patches += [(this + int(off, 0), struct.pack("<I", m.obj[iface])) for off, iface in case.get("this_com", {}).items()]
        else:
            this = fz.buffer(int(str(case.get("this_size", "0x400")), 0), vptr)
            for off, iface in case.get("this_com", {}).items():
                m.uc.mem_write(this + int(off, 0), struct.pack("<I", m.obj[iface]))
    args = [int(a, 16) for a in rp["args"]] if rp else [fz.arg(s) for s in case.get("args", [])]
    trial_seed = fz.rng.getrandbits(32)
    m.page_seed = trial_seed; m.reset_demand()
    for a, b in patches: m.live_write(a, b)
    if live: m.trial_no += 1
    m.constrain([] if rp else case.get("input_ranges", []), this, args)  # (a captured call's inputs are the game's own: not narrowed)
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
    # "setup": an export of the candidate DLL run once before the trials, its writes kept (fork.py gate-hook: the
    # mod's xikit_mod_init with a gate host, which records the replacement the candidate's entry jumps to). It
    # runs before any trial, so what it writes is the same state for every trial, and outside the original's runs
    if case.get("setup") and case["setup"] not in getattr(m, "setup_done", set()):
        if case["setup"] not in m.exports:
            return {"name": case["name"], "original": case["original"], "candidate": case["candidate"],
                    "verdict": "ERROR", "reason": "setup export %s not found" % case["setup"]}
        r0 = m.call(m.exports[case["setup"]], "cdecl", None, [], 2_000_000)
        if r0.status != "return":
            return {"name": case["name"], "original": case["original"], "candidate": case["candidate"],
                    "verdict": "ERROR", "reason": "setup %s did not return: %s %s" % (case["setup"], r0.status, r0.detail)}
        for a, v in r0.changes.items(): m.uc.mem_write(a, bytes([v]))
        m.setup_done = getattr(m, "setup_done", set()) | {case["setup"]}
    fz = G.Fuzzer(m, seed)
    fz.dict = m.dict = _cmp_consts(orig)
    statuses, first_fail, fails, com_calls, rounded, unresolved, faults = {}, None, 0, 0, 0, {}, {}
    # declared differences (declared.py): a changed function against the original, every difference stated
    decl = None
    if case.get('declared') is not None:
        import declared as DC
        decl = DC.load(case['declared']); DC.bind_imports(decl, getattr(m, 'trap_names', {}))
    m.keyed = decl is not None; declared_seen, declared_trials = {}, 0
    # every global this case overwrites is restored afterwards, so cases sharing an emulator never
    # see each other's fuzzed pointers (diffemu_fixed never restored them)
    touched = [(int(g, 16), n) for g, n in case.get("fuzz_ranges", [])]
    touched += [(int(g, 16), 4) for g in list(case.get("globals", {})) + list(case.get("com_globals", {})) + list(case.get("struct_globals", {}))]
    touched += [(int(e["addr"], 16), 4) for e in case.get("input_ranges", []) if e["where"] == "global"]
    saved = [(a, bytes(m.uc.mem_read(a, n))) for a, n in touched]
    # FFXI_COVERAGE=1: which of the original's own instructions ran in the trials that were compared (the original
    # returned and the reconstruction ran). A PASS says nothing about code no compared trial reached.
    cov = cov_hooks = None
    if os.environ.get('FFXI_COVERAGE'):
        cov_ins, cov_calls = _own_insns(orig)
        m.cov_run = set(); cov = set()
        cov_hooks = [m.uc.hook_add(UC_HOOK_CODE, lambda uc, addr, size, _: m.cov_run.add(addr), begin=lo, end=hi - 1)
                     for lo, hi in _extent(orig)]
    # each trial's inputs come from (seed, trial number) alone, so a slice of the trials (gate_some splits slow
    # functions into pieces: _trial_first, _trial_count) runs exactly the trials the whole run would. In live
    # mode the trial number also picks the dump object used as `this` (it was a per-process counter, so a
    # function's inputs depended on which cases the same worker had run before it).
    first = case.get('_trial_first', 0); trials = case.get('_trial_count', trials)
    for t in range(first, first + trials):
        fz.rng = random.Random(seed * 1000003 + t)
        if getattr(m, 'live', None): m.trial_no = t
        this, args, trial_seed = setup_trial(m, case, fz)
        a, patches = run_original(m, orig, conv, this, args, limit, trial_seed)
        ran = set(m.cov_run) if cov is not None else None
        if a.status == 'unresolved': unresolved[a.detail] = unresolved.get(a.detail, 0) + 1
        if a.status == 'fault': faults[a.detail or '?'] = faults.get(a.detail or '?', 0) + 1     # where the original faulted
        if a.status in ('fault', 'limit', 'unresolved'):
            b = None            # not compared (below): the reconstruction need not run
        else:
            # the original returned within `limit`; compiled code may take more instructions for the same
            # work, so the reconstruction gets ten times that before its not returning counts as a difference
            m.rng = random.Random(trial_seed); m.key_seed = trial_seed; m.wild = 0; b = m.call(cand, conv, this, args, limit * 10)
        restore_inputs(m, patches); demand_pages = len(m.demand); m.reset_demand()
        if b is not None and b.status == 'unresolved':     # the reconstruction met a count no source gives:
            unresolved[b.detail] = unresolved.get(b.detail, 0) + 1; a.status = 'unresolved'; b = None   # not compared
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
        if a.status in ('fault', 'limit', 'unresolved'):
            why = None          # the original crashed or never finished: no defined behaviour to
                                # compare against (the compiler may drop a dead load the original
                                # faulted on, or order independent work differently before a crash).
                                # A reconstruction that crashes where the original returns stays a FAIL.
        else:
            if decl is None: why = G.compare(a, b, 'int' if ret == 'word' else ret)
            else:
                why, seen = DC.compare(a, b, 'int' if ret == 'word' else ret, decl, this, G.compare)
                for k_, v_ in seen.items(): declared_seen[k_] = declared_seen.get(k_, 0) + v_
                declared_trials += bool(seen)
        if cov is not None and b is not None and a.status not in ('fault', 'limit', 'unresolved'):
            cov |= ran
        if why:
            fails += 1
            if first_fail is None:
                first_fail = {"trial": t, "this": hex(this) if this else None, "args": [hex(x) for x in args], "why": why}
    for a, b_ in saved: m.uc.mem_write(a, b_)
    m.ctx = m.uc.context_save()
    coverage = None
    if cov is not None:
        for h in cov_hooks: m.uc.hook_del(h)
        m.cov_run = None
        coverage = {"insns": len(cov_ins), "covered": len(cov & cov_ins),
                    "uncovered": ['0x%08X' % x for x in sorted(cov_ins - cov)],
                    "calls": len(cov_calls), "calls_uncovered": ['0x%08X' % x for x in sorted(cov_calls - cov)]}
    ok_runs = statuses.get("return", 0)
    verdict = "PASS" if fails == 0 and ok_runs > 0 else ("FAIL" if fails else "UNRESOLVED" if unresolved else "INCONCLUSIVE")
    return {"name": case["name"], "original": case["original"], "candidate": case["candidate"],
            "trials": trials, **({"_trial_first": first} if '_trial_first' in case else {}), "original_outcomes": statuses, "mismatches": fails, "d3d_calls_checked": com_calls,
            "verdict": verdict, "first_mismatch": first_fail, "x87_rounded_trials": rounded,
            "unresolved": unresolved, **({"original_faults": dict(sorted(faults.items(), key=lambda x: -x[1])[:5])} if faults else {}),
            **({"coverage": coverage} if coverage is not None else {}),
            **({"declared": {"trials_with_declared_differences": declared_trials, "seen": declared_seen,
                             "never_seen": [d['text'] for d in decl['calls'] + decl['memory'] if d['text'] not in declared_seen]
                                           + (['return'] if decl['return'] and 'return' not in declared_seen else [])}}
               if decl is not None else {})}


_COV_EXT = {}


def _extent(f):
    """the function's pieces [(lo, hi)] (data/func_extents.json)"""
    if not _COV_EXT:
        _COV_EXT.update({int(k, 16): v for k, v in json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'func_extents.json'))).items()})
    return [tuple(x) for x in _COV_EXT.get(f, [])]


_OWN = {}


def _own_insns(f):
    """(instruction addresses, call instruction addresses) of the function's own pieces, decoded from the image"""
    if f not in _OWN:
        from capstone import CS_ARCH_X86 as _A, CS_MODE_32 as _M, Cs as _Cs
        import pefile
        if '_imgb' not in _OWN:
            p = pefile.PE(os.environ['FFXI_DLL']); _OWN['_imgb'] = (bytes(p.get_memory_mapped_image()), p.OPTIONAL_HEADER.ImageBase)
        img, base = _OWN['_imgb']; ins, calls = set(), set()
        for lo, hi in _extent(f):
            for i in _Cs(_A, _M).disasm(img[lo - base:hi - base], lo):
                ins.add(i.address)
                if i.mnemonic == 'call': calls.add(i.address)
        _OWN[f] = (ins, calls)
    return _OWN[f]

_CONSTS = {}


def _cmp_consts(f):
    """the values the function's own code compares its inputs against, for the fuzzer's dictionary (Fuzzer.dict):
    the immediate of each `cmp`/`test` (and that value + 1, the other side of an ordered comparison); in a chain
    of `dec r` / `sub r, N` / `add r, N` each followed by a conditional jump (only `mov`/`push`/`pop`/`lea`, which leave the flags, between; an MSVC switch lowered to
    decrements), the value of r that makes the result zero; the dword of each `fcom`/`fcomp`/`fucom`/`fucomp`
    operand at a fixed address in the image, and of each `fld dword` from one. Only the inputs a trial draws
    change; both sides get the same ones, so this decides nothing, it only reaches branches a random value
    almost never takes (a transform type of 2, 3 or 256, an alpha of 0x80, a '\\n')."""
    if f in _CONSTS: return _CONSTS[f]
    from capstone import CS_ARCH_X86 as _A, CS_MODE_32 as _M, Cs as _Cs
    from capstone.x86 import X86_OP_IMM as _I, X86_OP_MEM as _MEM, X86_OP_REG as _R
    _own_insns(f)
    img, base = _OWN['_imgb']; md = _Cs(_A, _M); md.detail = True
    out = set()
    for lo, hi in _extent(f):
        ins = list(md.disasm(img[lo - base:hi - base], lo)); off = {}
        for n, i in enumerate(ins):
            m = i.mnemonic; o = i.operands
            k = n + 1                                # the next instruction that reads or sets flags
            while k < len(ins) and ins[k].mnemonic in ('mov', 'push', 'pop', 'lea'): k += 1
            nxt = ins[k].mnemonic if k < len(ins) else ''
            if m in ('cmp', 'test') and len(o) == 2 and o[1].type == _I:
                v = o[1].imm & 0xFFFFFFFF
                r = i.reg_name(o[0].reg) if o[0].type == _R else None
                out.add((v + off.get(r, 0)) & 0xFFFFFFFF if m == 'cmp' else v)
                if m == 'cmp': out.add((v + off.get(r, 0) + 1) & 0xFFFFFFFF)
            elif m in ('fcom', 'fcomp', 'fucom', 'fucomp', 'fld') and o and o[0].type == _MEM and o[0].size == 4 \
                    and not o[0].mem.base and not o[0].mem.index and base <= o[0].mem.disp < base + len(img) - 4:
                out.add(struct.unpack_from('<I', img, o[0].mem.disp - base)[0])
            if m in ('dec', 'sub', 'add') and o and o[0].type == _R and (m == 'dec' or (len(o) == 2 and o[1].type == _I)):
                r = i.reg_name(o[0].reg)
                d = 1 if m == 'dec' else (o[1].imm if m == 'sub' else -o[1].imm)
                off[r] = (off.get(r, 0) + d) & 0xFFFFFFFF
                if nxt.startswith('j') and nxt != 'jmp': out.add(off[r])
                continue
            if m.startswith('j') or m.startswith('ret'): continue
            for r in list(off):                      # any other write to r ends its chain
                if o and o[0].type == _R and i.reg_name(o[0].reg) == r and m not in ('cmp', 'test', 'push'): del off[r]
    out.discard(0)
    _CONSTS[f] = tuple(sorted(out)); return _CONSTS[f]


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
    ap.add_argument('--trials', type=int, default=100); ap.add_argument('--seed', type=int, default=11)     # the gate's (gate_some.py), so a direct run reproduces verification/
    ap.add_argument('--only', default=None)
    ap.add_argument('--cache', action='store_true', help='reuse verdicts of provably identical runs (gate_cache.py)')
    ap.add_argument('--serve', action='store_true',
                    help='CASES and OUT are ignored; read "CASES OUT" pairs from stdin, one per line, and answer '
                         '"done OUT" for each (one process for many chunks: gate_some.py)')
    o = ap.parse_args()
    if not o.serve:
        gate_file(o, o.cases, o.out); return
    # each case runs on an emulator of its own (batches(size=1)) and nothing a case sees depends on the
    # cases run before it in the process (checked: one process for many cases gives the same bytes as one
    # process per case), so a worker only saves the start-up: imports, PE parses, the dump
    for line in sys.stdin:
        if not line.strip(): continue
        cf, of = line.split()
        with open(of + '.log', 'w') as log:
            so = sys.stdout; sys.stdout = log
            try: gate_file(o, cf, of)
            except Exception as e: print('worker error: %r' % e)
            finally: sys.stdout = so
        print('done', of, flush=True)


_GC_STATE = {}


def gate_file(o, cases_path, out_path):
    """gate every case of one cases file into out_path (resuming: cases already in out_path are skipped)"""
    cases = json.load(open(cases_path))
    if o.only: cases = [c for c in cases if c['original'].lower() in o.only.lower().split(',')]
    done = set()
    if os.path.exists(out_path):
        done = {json.loads(l)['candidate'] for l in open(out_path)}
    cases = [c for c in cases if c['candidate'] not in done]
    f = open(out_path, 'a'); k = 0
    # gate_cache: a verdict is reused only when the run would be the same run (see gate_cache.py)
    use_cache = o.cache
    if use_cache:
        import gate_cache as GC
        if not _GC_STATE:
            _GC_STATE.update(base=GC.base_key(o.game, os.environ.get('FFXI_LIVE')), images={}); site_pops_init()
        base, images = _GC_STATE['base'], _GC_STATE['images']; hits = 0
    for chunk, stubs in batches(cases):
        # RECON may be a directory of per-function DLLs (build_raw.sh: fndll/fn_<address>.dll)
        recon = os.path.join(o.recon, 'fn_%08X.dll' % int(chunk[0]['original'], 16)) if os.path.isdir(o.recon) else o.recon
        if use_cache:
            st = os.stat(recon); ik = (recon, st.st_size, st.st_mtime_ns)     # a worker outlives files: key by version
            img = images.get(ik) or images.setdefault(ik, GC.Image(recon))
            c = chunk[0]; key = GC.case_key(base, c, o.trials, o.seed)
            r = GC.lookup(key, img, _CALLSITES) if len(chunk) == 1 else None
            if r is not None:
                hits += 1; f.write(json.dumps(r) + '\n'); f.flush(); k += len(chunk)
                print('%d/%d (cached)' % (k, len(cases)), flush=True); continue
        m = GfxMachine(o.game, recon, stubs, [])
        tr = GC.Tracker(m) if use_cache else None
        t0 = time.time()
        for c in chunk:
            try:
                r = run_case(m, c, o.trials, o.seed)
            except Exception as e:
                r = {'name': c['name'], 'original': c['original'], 'candidate': c['candidate'],
                     'verdict': 'ERROR', 'reason': repr(e)[:300]}
            if tr is not None and len(chunk) == 1 and r.get('verdict') != 'ERROR':
                GC.store(key, tr.deps(img, _CALLSITES), r)
            f.write(json.dumps(r) + '\n'); f.flush()
        if m.com_sites is not None:
            with open(out_path + '.com', 'a') as cf:
                cf.write(json.dumps({'original': chunk[0]['original'], 'sites': [
                    list(k) + [r[0], sorted(r[1], key=str), sorted(r[2], key=str)]
                    for k, r in sorted(m.com_sites.items(), key=str)]}) + '\n')
        with open(out_path + '.times', 'a') as tf:
            tf.write('%s %.3f\n' % (chunk[0]['original'], (time.time() - t0) / len(chunk)))
        k += len(chunk)
        print('%d/%d' % (k, len(cases)), flush=True)
    f.close()

if __name__ == '__main__':
    main()
