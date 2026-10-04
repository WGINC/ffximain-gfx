"""
diffemu.py -- gap 3: an equivalence gate for reconstructed functions.

Runs an original FFXiMain.dll function and a reconstructed one side by side in Unicorn, from
byte-identical starting states, over many fuzzed inputs, and compares everything a caller could
observe:
  * every byte either side changed outside its own stack frame (object fields, globals,
    buffers reached through pointers),
  * EAX/EDX, and the x87 stack top and ST0 when a function returns a float,
  * ESP after return (so a wrong `ret N` / calling convention is caught),
  * EBX/ESI/EDI/EBP preserved,
  * how the run ended: normal return, memory fault, import call (with its arguments), or
    instruction limit.

Both images share one address space, so a reconstruction can read the game's globals at their
real addresses and call not-yet-reconstructed original functions through plain function
pointers -- the same way a hybrid build injected through the proxy DLL would.

The reconstruction is a 32-bit PE DLL (built with MinGW or clang; see recon/build.sh) loaded at
its preferred base with no imports; functions are found through its export table.

Usage:
  python3 diffemu.py FFXiMain_unpacked.dll recon.dll cases.json [--trials N] [--seed S]
                     [--json out] [--overlay dump.bin@0xADDR ...]

A case may stub callees it is not testing ("stubs": {"0x10055570": {"returns": "this",
"pops": 0}}). "pops" is how many stack arguments the callee removes (thiscall/stdcall);
"args" is how many to log and compare (defaults to "pops"; set it for cdecl callees);
"conv" is the callee's convention ("thiscall" default, so ECX is compared). Stubs
apply to both runs, and every stubbed call is logged and compared.
"""
from __future__ import annotations

import argparse
import json
import array, os, random
import struct
import sys
from dataclasses import dataclass, field

import pefile
try:
    import numpy as _np      # optional: only speeds up Fuzzer.reset
except ImportError:
    _np = None
from unicorn import (Uc, UcError, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE, UC_HOOK_MEM_WRITE,
                     UC_HOOK_MEM_INVALID, UC_PROT_ALL)
from unicorn.x86_const import (UC_X86_REG_EAX, UC_X86_REG_EBP, UC_X86_REG_EBX, UC_X86_REG_ECX,
                               UC_X86_REG_EDI, UC_X86_REG_EDX, UC_X86_REG_EIP, UC_X86_REG_ESI,
                               UC_X86_REG_ESP, UC_X86_REG_FP0, UC_X86_REG_FPSW)

PAGE = 0x1000
TRAP_BASE = 0x70000000       # one 16-byte slot per imported function
RET_SENTINEL = 0x7F800000    # functions return here; emulation stops
STACK_TOP = 0x7F000000
STACK_SIZE = 0x100000
SANDBOX = 0x30000000         # fuzzed objects, buffers and the pointee pool
SANDBOX_SIZE = 0x01000000
POOL = SANDBOX + 0x00800000  # second half: random bytes that fuzzed pointers aim into
POOL_SIZE = 0x00800000
CANARY = {UC_X86_REG_EBX: 0xB0B0B0B0, UC_X86_REG_ESI: 0x51515151,
          UC_X86_REG_EDI: 0xD1D1D1D1, UC_X86_REG_EBP: 0xBEBEBEBE}


def align_up(v, a=PAGE):
    return (v + a - 1) & ~(a - 1)


@dataclass
class RunResult:
    status: str                       # "return", "fault", "import:<name>", "limit"
    detail: str = ""
    eax: int = 0
    edx: int = 0
    esp_delta: int = 0
    saved_ok: bool = True
    fp_top: int = 0
    st0: tuple | None = None
    changes: dict = field(default_factory=dict)   # addr -> final byte (outside the stack)
    stop_eip: int = 0
    calls: list = field(default_factory=list)       # stubbed callees, in call order


_F2S = struct.Struct('<f'); _I32 = struct.Struct('<I')
_F2I = lambda x: _I32.unpack(_F2S.pack(x))[0]          # a float's single-precision bits, as struct does it
_PE_LAYOUTS = {}


def _pe_layout(path):
    """(base, mapped size, mapped image, imports, exports) of a PE file, parsed once per process: the same
    file (path, size and modification time) always gives the same bytes, so the parse is reused"""
    st = os.stat(path); key = (os.path.abspath(path), st.st_size, st.st_mtime_ns)
    if key not in _PE_LAYOUTS:
        pe = pefile.PE(path)
        base = pe.OPTIONAL_HEADER.ImageBase
        img = pe.get_memory_mapped_image(ImageBase=base)
        size = align_up(max(pe.OPTIONAL_HEADER.SizeOfImage, len(img)))
        imports = []
        for d in getattr(pe, "DIRECTORY_ENTRY_IMPORT", []):
            for i in d.imports:
                nm = (i.name or f"ord{i.ordinal}".encode()).decode("latin-1")
                imports.append((i.address, f"{d.dll.decode('latin-1')}!{nm}"))
        exports = [(e.name.decode(), base + e.address)
                   for e in getattr(getattr(pe, "DIRECTORY_ENTRY_EXPORT", None), "symbols", []) or [] if e.name]
        _PE_LAYOUTS[key] = (base, size, bytes(img), imports, exports)
    return _PE_LAYOUTS[key]


class Machine:
    def __init__(self, game_dll: str, recon_dll: str | None, stubs: dict | None = None,
                 overlays: list[tuple[str, int]] = ()):
        self.uc = Uc(UC_ARCH_X86, UC_MODE_32)
        self.calls: list[tuple] = []
        self.trap_names: dict[int, str] = {}
        self.exports: dict[str, int] = {}
        self._map_pe(game_dll, trap_imports=True)
        if recon_dll:
            self.exports = self._map_pe(recon_dll, trap_imports=False)
        u = self.uc
        u.mem_map(TRAP_BASE, align_up(max(16 * len(self.trap_names), PAGE)), UC_PROT_ALL)
        u.mem_map(RET_SENTINEL, PAGE, UC_PROT_ALL)
        u.mem_map(STACK_TOP - STACK_SIZE, STACK_SIZE, UC_PROT_ALL)
        u.mem_map(SANDBOX, SANDBOX_SIZE, UC_PROT_ALL)
        u.mem_write(TRAP_BASE, b"\xF4" * (16 * len(self.trap_names) or 1))  # hlt
        u.mem_write(RET_SENTINEL, b"\xF4")
        self.writes: list[tuple[int, bytes]] = []
        self.stop_reason: str | None = None
        self.insn_count = 0
        self.limit = 0
        u.hook_add(UC_HOOK_MEM_WRITE, self._on_write)
        u.hook_add(UC_HOOK_CODE, self._on_trap, begin=TRAP_BASE, end=TRAP_BASE + 16 * len(self.trap_names))
        u.hook_add(UC_HOOK_CODE, self._on_ret, begin=RET_SENTINEL, end=RET_SENTINEL)
        u.hook_add(UC_HOOK_MEM_INVALID, self._on_invalid)
        # Stubbed callees: both sides call the same original address, so a stub changes both
        # runs identically. Each stubbed call is logged (callee, ECX, first 4 stack args) and
        # the logs are compared, so a reconstruction must call the same things in the same
        # order with the same arguments.
        self.stubs = {}
        for addr_s, spec in (stubs or {}).items():
            addr = int(addr_s, 16)
            self.stubs[addr] = spec
            u.hook_add(UC_HOOK_CODE, self._on_stub, begin=addr, end=addr)
        # Raw memory overlays (e.g. a .data dump from the live client) so globals the game
        # initialises at runtime hold real values instead of zero.
        for path, base in overlays:
            data = open(path, "rb").read()
            for pg in range(base & ~(PAGE - 1), align_up(base + len(data)), PAGE):
                try:
                    u.mem_map(pg, PAGE, UC_PROT_ALL)
                except UcError:
                    pass  # already mapped (inside the image)
            u.mem_write(base, data)
        self.ctx = u.context_save()

    # -- loading -------------------------------------------------------------
    def _map_pe(self, path, trap_imports):
        base, size, img, imports, export_list = _pe_layout(path)
        self.uc.mem_map(base, size, UC_PROT_ALL)
        self.uc.mem_write(base, img)
        if imports and not trap_imports:
            names = ", ".join(n for _, n in imports[:8])
            raise SystemExit(f"{path} imports functions ({names}...). Build it with -nostdlib and "
                             "link the freestanding helpers in recon/rt.c instead.")
        for slot, name in imports:
            t = TRAP_BASE + 16 * len(self.trap_names)
            self.trap_names[t] = name
            self.uc.mem_write(slot, struct.pack("<I", t))
        return dict(export_list)

    # -- hooks ---------------------------------------------------------------
    def _on_write(self, uc, access, addr, size, value, _):
        self.writes.append((addr, bytes(uc.mem_read(addr, size))))

    def _on_trap(self, uc, addr, size, _):
        esp = uc.reg_read(UC_X86_REG_ESP)
        args = struct.unpack("<4I", uc.mem_read(esp + 4, 16))
        self.stop_reason = f"import:{self.trap_names.get(addr, hex(addr))}"
        self.stop_detail = "args " + " ".join(f"{a:08X}" for a in args)
        uc.emu_stop()

    def _on_stub(self, uc, addr, size, _):
        spec = self.stubs[addr]
        esp = uc.reg_read(UC_X86_REG_ESP)
        ret_addr, *args = struct.unpack("<5I", uc.mem_read(esp, 20))
        ecx = uc.reg_read(UC_X86_REG_ECX)
        # log only the callee's real stack arguments; anything above them is the caller's frame
        nargs = spec.get("args", spec.get("pops", 0))
        # ECX is only meaningful to a thiscall callee; for cdecl/stdcall it is scratch
        logged_ecx = ecx if spec.get("conv", "thiscall") == "thiscall" else 0
        self.calls.append((addr, logged_ecx, tuple(args[:nargs])))
        r = spec.get("returns", "zero")
        uc.reg_write(UC_X86_REG_EAX, ecx if r == "this" else (0 if r == "zero" else int(str(r), 0)))
        uc.reg_write(UC_X86_REG_ESP, esp + 4 + 4 * spec.get("pops", 0))
        uc.reg_write(UC_X86_REG_EIP, ret_addr)

    def _on_ret(self, uc, addr, size, _):
        self.stop_reason = "return"
        uc.emu_stop()

    def _on_invalid(self, uc, access, addr, size, value, _):
        self.stop_reason = "fault"
        self.stop_detail = f"access 0x{addr:08X} size {size} type {access}"
        return False

    # -- running -------------------------------------------------------------
    def call(self, fn: int, conv: str, this: int | None, args: list[int], limit: int) -> RunResult:
        u = self.uc
        u.context_restore(self.ctx)
        self.writes.clear()
        self.calls = []
        self.stop_reason, self.stop_detail = None, ""
        esp = STACK_TOP - 0x1000
        frame = struct.pack("<I", RET_SENTINEL) + b"".join(struct.pack("<I", a & 0xFFFFFFFF) for a in args)
        esp -= len(frame)
        u.mem_write(esp, frame)
        u.reg_write(UC_X86_REG_ESP, esp)
        for r, v in CANARY.items():
            u.reg_write(r, v)
        u.reg_write(UC_X86_REG_EAX, 0xEEEEEEEE)
        u.reg_write(UC_X86_REG_EDX, 0xDDDDDDDD)
        u.reg_write(UC_X86_REG_ECX, this if conv == "thiscall" else 0xCCCCCCCC)
        start_esp = esp
        try:
            # no wall-clock timeout: Unicorn implements it with a timer thread per emu_start, which made
            # the whole gate ~4x slower. The two slow cases are bounded by the instruction count.
            u.emu_start(fn, RET_SENTINEL + 0x100, count=limit)
        except UcError as e:
            if not self.stop_reason:
                self.stop_reason, self.stop_detail = "fault", str(e)
        if not self.stop_reason:
            self.stop_reason = "limit"
        eip = u.reg_read(UC_X86_REG_EIP)
        r = RunResult(status=self.stop_reason, detail=self.stop_detail)
        r.eax = u.reg_read(UC_X86_REG_EAX)
        r.edx = u.reg_read(UC_X86_REG_EDX)
        # after `ret`, ESP sits just past the return address; callee-popped args move it further
        r.esp_delta = u.reg_read(UC_X86_REG_ESP) - (start_esp + 4)
        r.saved_ok = all(u.reg_read(k) == v for k, v in CANARY.items())
        fpsw = u.reg_read(UC_X86_REG_FPSW)
        r.fp_top = (fpsw >> 11) & 7
        r.st0 = u.reg_read(UC_X86_REG_FP0 + r.fp_top) if r.fp_top != 0 else None  # FPn are physical registers; st(0) is FP[TOP]
        # bytes outside the stack whose value actually changed (a store of the value already
        # there is invisible to any caller, so it must not count as a difference)
        lo, hi = STACK_TOP - STACK_SIZE, STACK_TOP
        before = {}
        for a, old in self.writes:
            for i, byte in enumerate(old):
                if not (lo <= a + i < hi):
                    before.setdefault(a + i, byte)  # first write saw the pre-run value
        for a, orig_byte in before.items():
            now = u.mem_read(a, 1)[0]
            if now != orig_byte:
                r.changes[a] = now
        r.stop_eip = eip
        r.calls = list(self.calls)
        # undo every write, newest first, so the next run starts from the same bytes
        for a, old in reversed(self.writes):
            u.mem_write(a, old)
        return r


# ---------------------------------------------------------------------------
# fuzzing
# ---------------------------------------------------------------------------

class Fuzzer:
    """Deterministic value mix: zeros, small ints, floats, and pointers that land in mapped
    memory, so most runs get past their first dereference instead of faulting at once."""

    def __init__(self, m: Machine, seed: int):
        self.m = m
        self.rng = random.Random(seed)
        self.cursor = SANDBOX

    def reset(self):
        self.cursor = SANDBOX
        # drawn in bulk (randbytes): the per-byte draw was 90% of a case's time
        pool = array.array('I', self.rng.randbytes(0x10000))
        sel = self.rng.randbytes(len(pool)); aux = array.array('I', self.rng.randbytes(0x10000))
        # every dword of the pointee pool is drawn from a mix -- zero (64/256), small integer 1..15 (51/256),
        # pool pointer (64/256), or random -- so loaded indices, flags and second-level pointers are all
        # realistic, not just the first dereference
        m = 0x10000 - 64
        if _np is not None:     # the same rule, element-wise in numpy (identical bytes; about 50x faster)
            k, a, p = (_np.frombuffer(sel, _np.uint8), _np.frombuffer(aux.tobytes(), '<u4'), _np.frombuffer(pool.tobytes(), '<u4'))
            pool = _np.where(k < 64, _np.uint32(0), _np.where(k < 115, a % 15 + 1, _np.where(
                k < 179, _np.uint32(POOL) + ((a % m) & _np.uint32(0xFFFFFFFC)), p))).astype('<u4').tobytes()
        else:
            pool = array.array('I', [0 if k < 64 else a % 15 + 1 if k < 115 else POOL + ((a % m) & ~3) if k < 179 else p
                                     for k, a, p in zip(sel, aux, pool)]).tobytes()
        for i in range(0, POOL_SIZE, len(pool)):
            self.m.uc.mem_write(POOL + i, pool)

    def dword(self):
        return self.dwords(1)[0]

    def dwords(self, n):
        """n draws of the value mix: zero 25%, 1..63 20%, a float in [-1000, 1000) 20%, a pool pointer 25%, any
        dword 10%. The random calls are randrange and uniform written out as CPython 3.11 defines them
        (randrange(a, b) = a + _randbelow(b - a), _randbelow by rejection on getrandbits(bit_length);
        uniform(a, b) = a + (b - a) * random()), so the stream and every value are those of one dword() call
        after another; only the Python call overhead is gone."""
        rnd = self.rng.random; bits = self.rng.getrandbits; f2i = _F2I
        span = POOL_SIZE - 0x1000; kspan = span.bit_length(); out = []
        for _ in range(n):
            k = rnd()
            if k < 0.25: out.append(0)
            elif k < 0.45:
                r = bits(6)
                while r >= 63: r = bits(6)
                out.append(1 + r)
            elif k < 0.65: out.append(f2i(-1000.0 + 2000.0 * rnd()))
            elif k < 0.9:
                r = bits(kspan)
                while r >= span: r = bits(kspan)
                out.append(POOL + (r & ~3))
            else: out.append(bits(32))
        return out

    def buffer(self, size: int, vptr: int | None = None) -> int:
        size = max(4, align_up(size, 4))
        addr = self.cursor
        self.cursor = align_up(self.cursor + size + 64, 16)
        if self.cursor >= POOL:
            raise RuntimeError("sandbox exhausted; lower this_size/ptr sizes")
        head = min(size, 0x1000)
        data = array.array('I', self.dwords(head // 4)).tobytes()
        if size > head:  # large buffers: tile a second realistic 4 KB block (fast, same mix)
            block = array.array('I', self.dwords(0x400)).tobytes()
            reps = (size - head + len(block) - 1) // len(block)
            data += (block * reps)[:size - head]
        if vptr is not None:
            data = struct.pack("<I", vptr) + data[4:]
        self.m.uc.mem_write(addr, data)
        return addr

    def arg(self, spec):
        if spec == "int":
            return self.dword()
        if spec == "small":
            return self.rng.randrange(0, 16)
        if spec == "flags":
            return self.rng.choice([0, 1, 2, 3])
        if spec == "float":
            return struct.unpack("<I", struct.pack("<f", self.rng.uniform(-1000.0, 1000.0)))[0]
        if isinstance(spec, str) and spec.startswith("ptrmid:"):
            # pointer into the middle of a buffer, for negative displacements: ptrmid:SIZE:OFFSET
            _, size, off = spec.split(":")
            return self.buffer(int(size, 0)) + int(off, 0)
        if isinstance(spec, str) and spec.startswith("ptr:"):
            return self.buffer(int(spec[4:], 0))
        if isinstance(spec, int):
            return spec
        raise ValueError(f"unknown arg spec {spec!r}")


# ---------------------------------------------------------------------------
# comparison
# ---------------------------------------------------------------------------

def compare(a: RunResult, b: RunResult, ret: str):
    """Return None if a caller could not tell the runs apart, else a description."""
    if a.status.split(":")[0] != b.status.split(":")[0] or (a.status.startswith("import") and a.status != b.status):
        return f"ended differently: original {a.status} {a.detail} / reconstruction {b.status} {b.detail}"
    if a.status == "limit":
        return None if b.status == "limit" else "original hit the instruction limit"
    if a.status.startswith("import") and a.detail != b.detail:
        return f"same import, different arguments: {a.detail} vs {b.detail}"
    if a.calls != b.calls:
        n = next((i for i, (x, y) in enumerate(zip(a.calls, b.calls)) if x != y), min(len(a.calls), len(b.calls)))
        fmt = lambda c: f"0x{c[0]:08X}(ecx=0x{c[1]:08X}, " + ", ".join(f"0x{v:08X}" for v in c[2]) + ")"
        ca = fmt(a.calls[n]) if n < len(a.calls) else "no call"
        cb = fmt(b.calls[n]) if n < len(b.calls) else "no call"
        return f"stubbed call #{n + 1} differs: original {ca} / reconstruction {cb}"
    diff = sorted(k for k in set(a.changes) | set(b.changes) if a.changes.get(k) != b.changes.get(k))
    if diff:
        k = diff[0]
        return (f"memory differs at {len(diff)} byte(s), first 0x{k:08X}: "
                f"original {a.changes.get(k, 'unchanged')} / reconstruction {b.changes.get(k, 'unchanged')}")
    if a.status != "return":
        return None  # both faulted/stopped identically with identical side effects
    if ret == "bool" and (a.eax & 0xFF) != (b.eax & 0xFF):
        return f"AL {a.eax & 0xFF} vs {b.eax & 0xFF}"  # MSVC bool: only AL is defined
    if ret in ("int", "ptr") and a.eax != b.eax:
        return f"EAX 0x{a.eax:08X} vs 0x{b.eax:08X}"
    if ret == "int64" and (a.eax, a.edx) != (b.eax, b.edx):
        return "EDX:EAX differs"
    if ret == "float" and (a.fp_top != b.fp_top or a.st0 != b.st0):
        return f"x87 result differs: {a.st0} vs {b.st0}"
    if ret == "void" and a.fp_top != b.fp_top:
        return "x87 stack depth differs after return"
    if a.esp_delta != b.esp_delta:
        return f"stack cleanup differs: original pops {a.esp_delta}, reconstruction pops {b.esp_delta}"
    if not b.saved_ok:
        return "reconstruction clobbered EBX/ESI/EDI/EBP"
    return None


def run_case(m: Machine, case: dict, trials: int, seed: int):
    orig = int(case["original"], 16)
    if case["candidate"] not in m.exports:
        return {"name": case["name"], "verdict": "ERROR", "reason": f"export {case['candidate']} not found"}
    cand = m.exports[case["candidate"]]
    conv = case.get("conv", "thiscall")
    ret = case.get("returns", "int")
    limit = case.get("insn_limit", 200000)
    fz = Fuzzer(m, seed)
    statuses, first_fail, fails = {}, None, 0
    for t in range(trials):
        fz.reset()
        # globals the function reads get fresh random bytes every trial (same for both runs)
        for g, n in case.get("fuzz_ranges", []):
            # half the bytes zero, so flag/boolean reads see both outcomes
            m.uc.mem_write(int(g, 16), bytes(0 if fz.rng.random() < 0.5 else fz.rng.getrandbits(8) for _ in range(n)))
        # global tables of pointers: every entry gets a fresh sandbox buffer
        for g, n in case.get("fuzz_ptr_ranges", []):
            for i in range(n):
                m.uc.mem_write(int(g, 16) + 4 * i, struct.pack("<I", fz.buffer(0x100)))
        for g, spec in case.get("globals", {}).items():
            m.uc.mem_write(int(g, 16), struct.pack("<I", fz.arg(spec) & 0xFFFFFFFF))
        this = None
        if conv == "thiscall":
            vptr = int(case["vptr"], 16) if case.get("vptr") else None
            this = fz.buffer(int(str(case.get("this_size", "0x40")), 0), vptr)
        args = [fz.arg(s) for s in case.get("args", [])]
        # snapshot the fuzzed inputs so both runs start identical: Machine.call undoes its
        # own writes, and the fuzzer wrote before either run
        m.ctx = m.uc.context_save()
        a = m.call(orig, conv, this, args, limit)
        b = m.call(cand, conv, this, args, limit)
        statuses[a.status.split(":")[0]] = statuses.get(a.status.split(":")[0], 0) + 1
        why = compare(a, b, ret)
        if why:
            fails += 1
            if first_fail is None:
                first_fail = {"trial": t, "this": hex(this) if this else None,
                              "args": [hex(x) for x in args], "why": why}
        # restore the pristine globals the case overrode
    verdict = "PASS" if fails == 0 and statuses.get("return", 0) > 0 else ("FAIL" if fails else "INCONCLUSIVE")
    # "expect": "FAIL" marks a deliberately wrong reconstruction kept as a test of the gate
    # itself; the case is satisfied when the gate rejects it.
    expected = case.get("expect", "PASS")
    return {"name": case["name"], "original": case["original"], "candidate": case["candidate"],
            "expected": expected, "met_expectation": verdict == expected,
            "trials": trials, "original_outcomes": statuses, "mismatches": fails,
            "verdict": verdict, "first_mismatch": first_fail}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("game_dll")
    ap.add_argument("recon_dll")
    ap.add_argument("cases")
    ap.add_argument("--trials", type=int, default=300)
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--json")
    ap.add_argument("--overlay", action="append", default=[],
                    help="raw memory dump to map, as path@0xADDRESS (repeatable)")
    o = ap.parse_args()
    overlays = []
    for ov in o.overlay:
        path, _, addr = ov.rpartition("@")
        overlays.append((path, int(addr, 16)))
    cases = json.load(open(o.cases))
    results = []
    for case in cases:
        # a fresh machine per case keeps globals and the sandbox pristine between cases
        m = Machine(o.game_dll, o.recon_dll, case.get("stubs"), overlays)
        r = run_case(m, case, o.trials, o.seed)
        results.append(r)
        tag = "" if r.get("expected", "PASS") == "PASS" else f" (expected {r['expected']})"
        line = f"{r['verdict'] + tag:<30} {r['name']:<34} mismatches {r.get('mismatches', '-')}/{r.get('trials', '-')}"
        line += f"  original outcomes {r.get('original_outcomes')}"
        print(line)
        if r.get("first_mismatch"):
            print("   first mismatch:", r["first_mismatch"]["why"])
        if r.get("reason"):
            print("  ", r["reason"])
    if o.json:
        json.dump(results, open(o.json, "w"), indent=2)
    return 0 if all(r.get("met_expectation") for r in results) else 1


if __name__ == "__main__":
    sys.exit(main())
