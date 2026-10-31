"""frame_diff.py -- the whole-frame differential: one entry point run on the live dump with the original code
only, then again with every swapped function's entry redirected to its reconstruction (what the in-game kit
does), and the two runs compared.

The per-function gate (diffemu_gfx.py) runs one function at a time and answers its callees as stubs, so it cannot
see a fault in how two replaced functions hand a value to each other. Here nothing is stubbed: callees run as
the game's own code or as the reconstruction, whichever the swap set says; only calls into other modules of the
dump (D3D8, polcore, Windows) are answered as in the gate (fake COM objects, pops from the call site).

    FFXI_DLL=... FFXI_LIVE=live_town.json python3 frame_diff.py RECON.dll MANIFEST.txt [--entry 0x10011A00]
        [--include-untested] [--only EXPORT,...] [--limit N] [--out report.json]

The swap set is the kit's manifest (flag 4 = untested is left out unless --include-untested), the entry the
redirect applies to is each function's original address (RVA + 0x10000000). Compared: the order, callee,
ECX and arguments of every foreign / Direct3D call; every byte changed outside the stack; EAX; ESP. The first
difference is reported with the call before it. Nothing here is a verdict about a function; it says where two
whole runs of the same input first differ."""
import json, os, sys, struct, time
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import diffemu_gfx as X
import diffemu_fixed as G
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EIP


class _FirstDraw:
    """a random stream whose next random() is 0.0 (D3D_OK in the gate's COM answer), the rest the stream's own"""
    def __init__(self, r): self.r = r; self.used = False
    def random(self):
        if self.used: return self.r.random()
        self.used = True; return 0.0
    def __getattr__(self, k): return getattr(self.r, k)


class FrameMachine(X.GfxMachine):
    """the gate's machine, with the write log made to tolerate a first write to a page the dump holds: the gate maps
    every page in a discovery run first and runs again; one run of a whole frame cannot afford that"""
    def _on_write(self, uc, access, addr, size, value, _):
        try: old = bytes(uc.mem_read(addr, size))
        except Exception:
            for p in {addr & ~(G.PAGE - 1), (addr + size - 1) & ~(G.PAGE - 1)}:
                try: uc.mem_read(p, 1)
                except Exception:
                    if not self.live_map(p) and addr >= 0x10000:
                        uc.mem_map(p, G.PAGE, X.UC_PROT_ALL); uc.mem_write(p, self._page_bytes(p)); self.demand.append(p)
            old = bytes(uc.mem_read(addr, size))
        self.writes.append((addr, old))


    # IDirect3DVertexBuffer8 / IDirect3DIndexBuffer8::Lock(UINT OffsetToLock, UINT SizeToLock, BYTE **ppbData, DWORD Flags)
    # (d3d8.h): on D3D_OK the method stores the locked memory's address through ppbData. The gate's COM model leaves it
    # unwritten (data/com_spec.json gives Lock no output; the gate is not changed here, so its cache stands), and a
    # frame then writes its vertices wherever the slot pointed (31 Oct: DynVB_Unlock's own manager object, a fault at
    # 0x100441B3 on every dump). Here a successful Lock gets a fresh zeroed block of SizeToLock bytes (0 = the whole
    # buffer, whose size the call does not give: the allocator's largest block, 64 KB), the same on both runs.
    BUFFER_LOCK = {('IDirect3DVertexBuffer8', 'Lock'), ('IDirect3DIndexBuffer8', 'Lock')}

    def _on_com(self, uc, addr, size, _):
        t = self.trap.get(addr)
        lock = None
        if t and t[0] != '*wild' and (t[0], X.SPEC[t[0]][t[1]]['name']) in self.BUFFER_LOCK:
            esp = uc.reg_read(X.UC_X86_REG_ESP)
            _, _, _, size_, pp, _ = struct.unpack('<6I', uc.mem_read(esp, 24))     # return, this, the four parameters
            lock = (size_, pp)
        masked = None
        if t and t[0] == 'IDirect3DDevice8' and X.SPEC[t[0]][t[1]]['name'] == 'SetLight':
            # SetLight(DWORD Index, CONST D3DLIGHT8 *): for a directional light (Type 3) the SDK says Position "has no
            # meaning for directional lights and is ignored" (DirectX 8.1 reference, D3DLIGHT8) and "directional
            # lights don't use the range property" (programmer's guide, Light Position, Range, and Attenuation), so
            # those two fields (dwords 13-15 and 19) are left out of the call's data hash; every other field counts.
            esp = uc.reg_read(X.UC_X86_REG_ESP); pl = struct.unpack('<I', uc.mem_read(esp + 12, 4))[0]
            try:
                if struct.unpack('<I', uc.mem_read(pl, 4))[0] == 3:
                    masked = (pl, bytes(uc.mem_read(pl + 52, 12)), bytes(uc.mem_read(pl + 76, 4)))
                    uc.mem_write(pl + 52, bytes(12)); uc.mem_write(pl + 76, bytes(4))
            except Exception: masked = None
        if t and t[0] != '*wild' and X.SPEC[t[0]][t[1]]['ret'] == 'hresult':
            # the run's declared input: every Direct3D method returns D3D_OK (the gate draws a failure for 20% of
            # calls to test both sides' error paths; a frame of the game running normally is the input here). The
            # HRESULT is the gate's first draw from the trial stream in this call, so that one draw is answered 0.0.
            self.rng = _FirstDraw(self.rng)
        super()._on_com(uc, addr, size, _)
        if isinstance(self.rng, _FirstDraw): self.rng = self.rng.r
        if masked: uc.mem_write(masked[0] + 52, masked[1]); uc.mem_write(masked[0] + 76, masked[2])
        if lock and uc.reg_read(X.UC_X86_REG_EAX) == 0:
            blk = self.alloc(lock[0] or 0x10000)
            if blk:
                self.writes.append((lock[1], bytes(uc.mem_read(lock[1], 4))))
                uc.mem_write(lock[1], struct.pack('<I', blk))
                self.locks += 1

    def _on_foreign(self, uc, addr, size, _):
        """as the gate's: a call into another module of the dump is logged and answered. What it pops: the call site's
        definitive count (site_pops) where there is one, else the callee's own `ret N` read from its code in the dump
        (live_state.purge: every reachable ret agreeing, the walk complete). Neither: the run stops as unresolved."""
        esp = uc.reg_read(X.UC_X86_REG_ESP)
        ret_addr = struct.unpack('<I', uc.mem_read(esp, 4))[0]
        self._last_com = None
        t = self._com_target(uc, addr, esp, ret_addr)
        if t is not None:
            uc.reg_write(UC_X86_REG_EIP, t); return
        site = X.site_pops(self, ret_addr) if X.IMAGE_LO <= ret_addr < X.IMAGE_HI else None
        if site is not None: n = 4 * site[0]; src = 'site'
        else:
            p = self.live.purge(addr); n = p; src = 'callee'
            if (p is None or p % 4) and self.assume:
                # NOT a fact: no source gives this call's pop count. The dwords pushed in the straight run of
                # instructions before the call (none past an earlier call or branch) are taken as its stdcall
                # arguments, and the call is listed under 'ASSUMED' in the report; a result that depends on one is
                # conditional on it. A wrong count shows as a stack the function does not restore (esp check below).
                p = self.assumed_pops(ret_addr); n = p; src = 'ASSUMED'
                if p is not None: self.assumed.setdefault(ret_addr, [p, 0])[1] += 1
            if p is None or p % 4:
                self.stop_reason = 'unresolved'
                self.stop_detail = 'call returning to 0x%08X into %s (0x%08X): pop count unknown' % (ret_addr, self.live.module_of(addr), addr)
                uc.emu_stop(); return
        self.pop_src[src] = self.pop_src.get(src, 0) + 1
        k = min(n // 4, X.MAX_STUB_ARGS)
        args = struct.unpack('<%dI' % k, uc.mem_read(esp + 4, 4 * k)) if k else ()
        self.calls.append((addr, 0, tuple(X._stackref(a) for a in args)))     # ECX of a foreign call is leftover, not compared
        r = self.rng.random()
        v = 0 if r < 0.6 else 1 if r < 0.7 else self.rng.randrange(2, 64) if r < 0.85 else self.rng.getrandbits(32)
        uc.reg_write(X.UC_X86_REG_EAX, v); uc.reg_write(X.UC_X86_REG_ESP, esp + 4 + n); uc.reg_write(UC_X86_REG_EIP, ret_addr)


    def assumed_pops(self, ret_addr):
        if ret_addr in self.assumed_cache: return self.assumed_cache[ret_addr]
        cs = self._md; pushes = 0; a = ret_addr; res = None
        # decode backwards: try each start up to 64 bytes before the call, keep the decode whose last instruction ends at the call
        call = None
        for L in (2, 3, 6, 5, 7):
            code = bytes(self.uc.mem_read(ret_addr - L, L))
            for i in cs.disasm(code, ret_addr - L):
                if i.mnemonic == 'call' and i.address + i.size == ret_addr: call = i
            if call: break
        if call:
            window = bytes(self.uc.mem_read(call.address - 96, 96)); base = call.address - 96
            for st in range(0, 16):                      # a decode of the window that lands exactly on the call
                ins = list(cs.disasm(window[st:], base + st))
                if ins and ins[-1].address + ins[-1].size == call.address:
                    pushes = 0
                    for i in reversed(ins):
                        if i.mnemonic in ('call', 'jmp', 'ret', 'retn') or i.mnemonic.startswith('j') or i.mnemonic in ('leave',): break
                        if i.mnemonic == 'push': pushes += 1
                        elif i.mnemonic == 'sub' and i.op_str.startswith('esp') or i.mnemonic == 'add' and i.op_str.startswith('esp'): break
                    res = 4 * pushes; break
        self.assumed_cache[ret_addr] = res
        return res


def read_manifest(path):
    out = []
    for l in open(path):
        if l.startswith('#') or l.startswith('!') or not l.strip(): continue
        p = l.split()
        out.append((0x10000000 + int(p[0], 16), int(p[1]), p[2]))
    return out


def fmt(c):
    return '0x%08X(ecx=0x%08X%s)' % (c[0], c[1], ''.join(', 0x%08X' % v for v in c[2]))


POP_WORKER = 0x1000A2B0      # CDx's render-target pop worker (docs/RENDERER.md section 9)


def run_once(m, entry_fn, this, limit, redirect, seed):
    hooks = []; m.pop_src.clear(); m.assumed.clear(); m.pops = []
    def on_pop(uc, addr, size, _):
        # each pop: the call count so far, the depth it lowers (CDx +0x158) and the world target bound (+0x194); the
        # end of the world pass by ffxi_world_draw's rule is the pop from depth 1 (their statement, not checked here)
        cdx = struct.unpack('<I', uc.mem_read(0x1045666C, 4))[0]
        m.pops.append((len(m.calls), uc.mem_read(cdx + 0x158, 1)[0], struct.unpack('<I', uc.mem_read(cdx + 0x194, 4))[0]))
    hooks.append(m.uc.hook_add(UC_HOOK_CODE, on_pop, begin=POP_WORKER, end=POP_WORKER))
    if redirect:
        def on_entry(uc, addr, size, _):
            t = redirect.get(addr)
            if t is not None: uc.reg_write(UC_X86_REG_EIP, t)
        for a in redirect: hooks.append(m.uc.hook_add(UC_HOOK_CODE, on_entry, begin=a, end=a))
    m.rng = X.random.Random(seed); m.wild = 0
    t0 = time.time(); r = m.call(entry_fn, 'thiscall', this, [], limit)
    for h in hooks: m.uc.hook_del(h)
    return r, time.time() - t0


def main():
    import argparse
    ap = argparse.ArgumentParser()
    ap.add_argument('recon'); ap.add_argument('manifest')
    ap.add_argument('--entry', default='0x10011A00'); ap.add_argument('--include-untested', action='store_true')
    ap.add_argument('--this-global', default=None, help="`this` is the dword at this game address in the dump (Frame_Render's: "
                    "0x104568FC, as App_RenderFrameMaskedFPU 0x10001B50 loads it; docs/RENDERER.md section 2)")
    ap.add_argument('--only', default=None); ap.add_argument('--limit', type=int, default=200000000)
    ap.add_argument('--assume-pops', dest='assume', action='store_true', help='exploratory: unresolved call pops are ASSUMED (see _on_foreign), not facts'); ap.add_argument('--seed', type=int, default=1); ap.add_argument('--out', default=None)
    o = ap.parse_args()
    game = os.environ['FFXI_DLL']
    m = FrameMachine(game, o.recon, {}, [])
    m.pop_src = {}; m.locks = 0; m.assume = o.assume; m.assumed = {}; m.assumed_cache = {}
    m.demand_limit = 1 << 20; m.synth_limit = 96
    sw = [(a, f, n) for a, f, n in read_manifest(o.manifest) if (o.include_untested or not f & 4)]
    if o.only: keep = set(o.only.split(',')); sw = [s for s in sw if s[2] in keep]
    redirect = {a: m.exports[n] for a, f, n in sw if n in m.exports}
    entry = int(o.entry, 16)
    case = {'original': o.entry, 'candidate': None, 'conv': 'thiscall', 'vptr': None, 'this_size': '0x4000', 'args': []}
    fz = G.Fuzzer(m, o.seed); fz.rng = X.random.Random(o.seed)
    m.trial_no = 0
    this, args, tseed = X.setup_trial(m, case, fz)
    if o.this_global:
        this = struct.unpack('<I', m.uc.mem_read(int(o.this_global, 16), 4))[0]
        if not this: sys.exit('%s holds 0 in this dump' % o.this_global)
    print('entry %s this=%s, %d swapped (%d with exports)' % (o.entry, hex(this or 0), len(sw), len(redirect)), flush=True)
    a, ta = run_once(m, entry, this, o.limit, None, tseed)
    print('stop eip 0x%08X' % a.stop_eip); print('original:        %s %s  %d foreign/D3D calls, %d bytes changed, %.1f s, pops from %s' % (a.status, a.detail, len(a.calls), len(a.changes), ta, dict(m.pop_src)), flush=True); print('  assumed sites: %d' % len(m.assumed), flush=True)
    pops_a = list(m.pops); m.reset_demand()
    cand_entry = redirect.get(entry, entry)
    b, tb = run_once(m, cand_entry, this, o.limit, redirect, tseed)
    print('reconstruction:  %s %s  %d foreign/D3D calls, %d bytes changed, %.1f s' % (b.status, b.detail, len(b.calls), len(b.changes), tb), flush=True)
    rep = {'entry': o.entry, 'swapped': len(redirect), 'original': [a.status, a.detail, len(a.calls)], 'recon': [b.status, b.detail, len(b.calls)],
           'dump': os.environ.get('FFXI_LIVE'), 'recon_dll': o.recon, 'manifest': o.manifest,
           'declared_input': 'every Direct3D method returns D3D_OK; a buffer Lock returns a fresh zeroed block',
           'not_compared': 'SetLight of a directional light: Position and Range (DirectX 8.1 SDK, D3DLIGHT8)'}
    wend = next((c for c, d, w in pops_a if d == 1), None)      # the original run's pops
    rep['world_pass_end_call'] = wend
    print('world pass end (pop from depth 1): %s' % ('at call %d of %d' % (wend, len(a.calls)) if wend is not None else 'not reached'))
    why = G.compare(a, b, 'int')
    rep['difference'] = why
    n = next((i for i, (x, y) in enumerate(zip(a.calls, b.calls)) if x != y), None)
    if n is None and len(a.calls) != len(b.calls): n = min(len(a.calls), len(b.calls))
    if n is not None:
        rep['first_call_difference'] = n
        print('first differing call: #%d' % (n + 1))
        for i in range(max(0, n - 3), n + 2):
            print('  %d original %s | recon %s' % (i + 1, fmt(a.calls[i]) if i < len(a.calls) else '-', fmt(b.calls[i]) if i < len(b.calls) else '-'))
    print('compare:', why or 'identical')
    if o.out: json.dump(rep, open(o.out, 'w'), indent=1)


if __name__ == '__main__':
    main()
