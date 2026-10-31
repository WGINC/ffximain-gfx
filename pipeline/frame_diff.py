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


def run_once(m, entry_fn, this, limit, redirect, seed):
    hooks = []; m.pop_src.clear(); m.assumed.clear()
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
    m.pop_src = {}; m.assume = o.assume; m.assumed = {}; m.assumed_cache = {}
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
    m.reset_demand()
    cand_entry = redirect.get(entry, entry)
    b, tb = run_once(m, cand_entry, this, o.limit, redirect, tseed)
    print('reconstruction:  %s %s  %d foreign/D3D calls, %d bytes changed, %.1f s' % (b.status, b.detail, len(b.calls), len(b.changes), tb), flush=True)
    rep = {'entry': o.entry, 'swapped': len(redirect), 'original': [a.status, a.detail, len(a.calls)], 'recon': [b.status, b.detail, len(b.calls)]}
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
