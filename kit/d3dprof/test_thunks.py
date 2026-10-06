"""test_thunks.py -- runs d3dprof.dll's thunks in Unicorn (no Windows needed): a call through the hooked
vtable must reach the original method with the caller's stack exactly as the caller built it, return to the
caller with the method's own pop, and be counted (with a repeated SetRenderState counted as redundant).

    python3 test_thunks.py            (after build.sh)
"""
import struct, subprocess, sys, os
import pefile
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_EAX, UC_X86_REG_ECX

H = os.path.dirname(os.path.abspath(__file__)); DLL = os.path.join(H, 'd3dprof.dll')
sym = {}
for l in subprocess.run(['i686-w64-mingw32-nm', DLL], capture_output=True, text=True).stdout.split('\n'):
    p = l.split()
    if len(p) == 3: sym[p[2]] = int(p[0], 16)
pe = pefile.PE(DLL); base = pe.OPTIONAL_HEADER.ImageBase; img = pe.get_memory_mapped_image()
u = Uc(UC_ARCH_X86, UC_MODE_32)
u.mem_map(base, (len(img) + 0xFFF) & ~0xFFF); u.mem_write(base, img)
STUB, STACK, VT, GAME = 0x20000000, 0x30000000, 0x40000000, 0x10000000
u.mem_map(STUB, 0x10000); u.mem_write(STUB + 0xF00, struct.pack('<I', 0x70000000)); u.mem_map(STACK - 0x10000, 0x10000); u.mem_map(VT, 0x1000)
# imports the record path uses (VirtualAlloc: successive 128 MB blocks from 0x70000000): GetCurrentThreadId -> 7, QueryPerformanceCounter -> writes 0 (stdcall, 1 arg)
for e in pe.DIRECTORY_ENTRY_IMPORT:
    for imp in e.imports:
        code = {b'VirtualAlloc': b'\xA1' + struct.pack('<I', STUB + 0xF00) + b'\x81\x05' + struct.pack('<I', STUB + 0xF00) + b'\x00\x00\x00\x08\xC2\x10\x00', b'IsBadReadPtr': b'\x31\xC0\xC2\x08\x00',
                b'GetCurrentThreadId': b'\xB8\x07\x00\x00\x00\xC3',
                b'QueryPerformanceCounter': b'\x8B\x44\x24\x04\xC7\x00\x00\x00\x00\x00\xC7\x40\x04\x00\x00\x00\x00\xC2\x04\x00'}.get(imp.name)
        a = STUB + 0x100 * (imp.address & 0xFF)
        u.mem_write(a, code or b'\xF4'); u.mem_write(imp.address, struct.pack('<I', a))
# the "original" methods: slot k is `mov eax, 0x1000+k; ret 4*(1+nparams)` (the header's pops)
NP = {50: 2, 34: 0, 71: 5}
for k in range(97):
    n = 4 * (1 + NP.get(k, 0))
    u.mem_write(VT + 0x800 + 16 * k, b'\xB8' + struct.pack('<I', 0x1000 + k) + b'\xC2' + struct.pack('<H', n))
    u.mem_write(VT + 4 * k, struct.pack('<I', VT + 0x800 + 16 * k))
u.mem_write(sym['_g_orig'], struct.pack('<I', VT))
u.mem_write(sym['_g_next'], bytes(u.mem_read(VT, 4 * 97)))     # (d3dprof_attach: set_next, the thunks' onward table; no wrappers here)
u.mem_write(sym['_g_lo'], struct.pack('<I', GAME)); u.mem_write(sym['_g_hi'], struct.pack('<I', GAME + 0x1000000))
u.mem_write(sym['_g_rs'], b'\xFF' * 1024)
RET = GAME + 0x9736   # a "call site" in the game's DLL (as loaded at its analysed base: g_rel = 0)
u.mem_map(GAME, 0x10000); u.mem_write(RET, b'\xF4')


def call(slot, args, ret=RET, above=()):
    sp = STACK - 0x100
    u.mem_write(ret, b'\xF4')
    frame = [ret, 0x55AA0000] + list(args) + list(above)
    u.mem_write(sp, struct.pack('<%dI' % len(frame), *frame))
    u.reg_write(UC_X86_REG_ESP, sp)
    seen = {}
    def at_orig(uc, addr, size, _):
        if addr == VT + 0x800 + 16 * slot:
            esp = uc.reg_read(UC_X86_REG_ESP)
            seen['stack'] = struct.unpack('<%dI' % len(frame), uc.mem_read(esp, 4 * len(frame)))
            seen['esp'] = esp
    h = u.hook_add(UC_HOOK_CODE, at_orig)
    u.emu_start(sym['_d3dprof_t%d' % slot], ret)
    u.hook_del(h)
    assert seen.get('esp') == sp, 'the original method did not see the caller\'s stack pointer'
    assert list(seen['stack']) == frame, 'the stack changed on the way: %r' % (seen['stack'],)
    assert u.reg_read(UC_X86_REG_ESP) == sp + 4 + 4 * (1 + NP.get(slot, 0)), 'wrong pop'
    assert u.reg_read(UC_X86_REG_EAX) == 0x1000 + slot, 'the method\'s result was not returned'


call(50, [7, 1]); call(50, [7, 1]); call(50, [7, 0]); call(34, []); call(71, [4, 0, 3, 0, 1])
calls = struct.unpack('<Q', u.mem_read(sym['_g_calls_window'], 8))[0]
red = struct.unpack('<Q', u.mem_read(sym['_g_redundant_window'], 8))[0]
prims = struct.unpack('<Q', u.mem_read(sym['_g_prims_window'], 8))[0]
draws = struct.unpack('<Q', u.mem_read(sym['_g_draws_window'], 8))[0]
assert (calls, red, prims, draws) == (5, 1, 1, 1), (calls, red, prims, draws)
# an odd return address in the game stays exact and counts as FFXiMain; a draw from CDx_DrawPrimitiveUP's
# plain site (returns to 0x1000CDE2, wrapper's caller at [ESP+36]) also records the wrapper's caller
call(50, [7, 1], ret=GAME + 0x9737)
caller = GAME + 0x1235
call(72, [4, 2, 0x1000, 20], ret=GAME + 0xCDE2, above=[0] * 3 + [caller])   # 2 params + this + ret = 4 dwords
sites = {}
for k in range(8192):
    site, slot, used, kind, calls, red, prims, ticks, timed = struct.unpack('<IHBBIIQQI4x', u.mem_read(sym['_g_sites'] + 40 * k, 40))
    if used: sites[(site, slot, kind)] = (calls, prims)
assert sites.get((GAME + 0x9737, 50, 0)) == (1, 0), sites
assert sites.get((GAME + 0xCDE2, 72, 0)) == (1, 2), sites
assert sites.get((caller, 72, 2)) == (1, 2), sites
# the call trace: label 0 for one frame, a SetRenderState, a SetTransform (matrix hashed), a DrawPrimitiveUP (vertex data
# hashed), then Present ends the frame; label 1 starts at frame 0 again and keeps the earlier records
TR = 0x70000000; u.mem_map(TR, 0x18000000)      # three 128 MB blocks: the trace, its bytes, the state dictionary
def ccall(name, *args):
    sp = STACK - 0x400; u.mem_write(sp, struct.pack('<%dI' % (len(args) + 1), RET, *args)); u.reg_write(UC_X86_REG_ESP, sp)
    u.mem_write(RET, b'\xF4'); u.emu_start(sym[name], RET)
ccall('_d3dprof_trace_start', 0)
MAT = 0x6A000000; u.mem_map(MAT, 0x1000); u.mem_write(MAT, bytes(range(64)))
call(50, [27, 1]); call(37, [2, MAT]); call(72, [4, 1, MAT, 12]); call(15, [0, 0, 0, 0])
ccall('_d3dprof_trace_start', 1); call(50, [27, 0]); call(44, [1, MAT])
ccall('_d3dprof_trace_stop'); call(50, [27, 5])
n = struct.unpack('<I', u.mem_read(sym['_g_ntrace'], 4))[0]
recs = [struct.unpack('<BBHII6II', u.mem_read(g + 0, 40)) for g in [TR + 40 * k for k in range(n)]]
assert n == 6, n
assert [(r[0], r[2], r[3], r[1]) for r in recs] == [(0, 50, 0, 2), (0, 37, 0, 2), (0, 72, 0, 4), (0, 15, 0, 4), (1, 50, 0, 2), (1, 44, 0, 2)], recs
assert recs[0][5:7] == (27, 1) and recs[4][5:7] == (27, 0)
fnv = lambda b: __import__('functools').reduce(lambda h, x: ((h ^ x) * 16777619) & 0xFFFFFFFF, b, 2166136261)
assert recs[1][11] == fnv(bytes(range(64))), hex(recs[1][11])
assert recs[2][11] == fnv(bytes(range(36))), hex(recs[2][11])     # type 4 (triangle list), count 1: 3 vertices of stride 12 = 36 bytes
boff = struct.unpack('<I', u.mem_read(sym['_g_boff'], 4))[0]; blen = struct.unpack('<I', u.mem_read(sym['_g_blen'], 4))[0]; gb = struct.unpack('<I', u.mem_read(sym['_g_bytes'], 4))[0]
o1 = struct.unpack('<I', u.mem_read(boff + 4, 4))[0]; l1 = u.mem_read(blen + 1, 1)[0]
assert l1 == 64 and bytes(u.mem_read(gb + o1, 64)) == bytes(range(64)), (l1, o1)
assert u.mem_read(blen + 0, 1)[0] == 0                       # SetRenderState: no bytes
# the device state the draw ran with: record 2 (DrawPrimitiveUP) has a state hash; its snapshot has render state 27 = 1
# (set by record 0) and render state 0 unknown (never set: 0xFFFFFFFF)
st = struct.unpack('<I', u.mem_read(sym['_g_st'], 4))[0]; snap = struct.unpack('<I', u.mem_read(sym['_g_snap'], 4))[0]
st2 = struct.unpack('<I', u.mem_read(st + 8, 4))[0]; nsnap = struct.unpack('<I', u.mem_read(sym['_g_nsnap'], 4))[0]
assert st2 != 0 and struct.unpack('<I', u.mem_read(st, 4))[0] == 0 and nsnap == 1, (hex(st2), nsnap)
assert struct.unpack('<I', u.mem_read(snap + 4 * 27, 4))[0] == 1 and struct.unpack('<I', u.mem_read(snap, 4))[0] == 0xFFFFFFFF
o5 = struct.unpack('<I', u.mem_read(boff + 20, 4))[0]
assert u.mem_read(blen + 5, 1)[0] == 104 and bytes(u.mem_read(gb + o5, 104)) == bytes(range(64)) + bytes(40)   # SetLight: the D3DLIGHT8

# vertex buffer contents: with buffers hooked, SetStreamSource gives the bound VB a vtable copy whose Lock/Unlock are
# wrapped; at Unlock the locked range is hashed and recorded (slot 97) with the Lock's call site
VB, VBVT, VBDATA = 0x6B000000, 0x6B000100, 0x6B001000
u.mem_map(VB, 0x10000)
# fake VB methods: Lock(this, off, size, pp, flags) stores VBDATA+off in *pp, ret 20; Unlock ret 4; GetDesc(this, d) d[4] = 64, ret 8
lock = b'\x8B\x44\x24\x10\x8B\x4C\x24\x08\x81\xC1' + struct.pack('<I', VBDATA) + b'\x89\x08\x31\xC0\xC2\x14\x00'
unlock = b'\x31\xC0\xC2\x04\x00'
desc = b'\x8B\x44\x24\x08\xC7\x40\x10\x40\x00\x00\x00\x31\xC0\xC2\x08\x00'
for k in range(14): u.mem_write(VBVT + 4 * k, struct.pack('<I', VB + 0x800 + 0x40 * k))
for k, code in ((11, lock), (12, unlock), (13, desc)): u.mem_write(VB + 0x800 + 0x40 * k, code)
u.mem_write(VB, struct.pack('<I', VBVT))
ccall('_d3dprof_trace_hookbufs', 1); ccall('_d3dprof_trace_start', 0)
call(83, [0, VB, 24])                                         # SetStreamSource(0, VB, 24)
newvt = struct.unpack('<I', u.mem_read(VB, 4))[0]
assert newvt != VBVT, 'the bound VB was not given the wrapped vtable'
def vcall(fn, args, ret):
    sp = STACK - 0x400; u.mem_write(sp, struct.pack('<%dI' % (len(args) + 1), ret, *args)); u.reg_write(UC_X86_REG_ESP, sp)
    u.mem_write(ret, b'\xF4'); u.emu_start(fn, ret); return u.reg_read(UC_X86_REG_EAX), u.reg_read(UC_X86_REG_ESP) - sp
PP = 0x6B002000
lk = struct.unpack('<I', u.mem_read(newvt + 44, 4))[0]; ul = struct.unpack('<I', u.mem_read(newvt + 48, 4))[0]
LOCKSITE = GAME + 0x4321
eax, popped = vcall(lk, [VB, 0, 0, PP, 0], LOCKSITE)           # size 0: the whole buffer, from GetDesc
assert eax == 0 and popped == 4 + 20, (eax, popped)
assert struct.unpack('<I', u.mem_read(PP, 4))[0] == VBDATA
data = struct.pack('<4f', 1.0, 2.0, 3.0, 2e7) + struct.pack('<I', 0xFF808080) + bytes(44)
u.mem_write(VBDATA, data)
eax, popped = vcall(ul, [VB], GAME + 0x4400)
assert eax == 0 and popped == 4 + 4, (eax, popped)
n2 = struct.unpack('<I', u.mem_read(sym['_g_ntrace'], 4))[0]
r = struct.unpack('<BBHII6II', u.mem_read(TR + 40 * (n2 - 1), 40))
assert r[2] == 97 and r[4] == LOCKSITE and r[5:9] == (0, 64, 1, 1) and r[11] == fnv(data), r
# per-call time (call_time=1): the call returns through d3dprof_ret_stub, which reads the counter and goes back to
# the caller; the method sees the caller's stack with only the return address replaced, and leaves EAX and EDX as
# the caller gets them. The fake counter advances 100 per read, so one timed call is exactly 100 ticks
u.mem_write(STUB + 0xF10, struct.pack('<I', 0))
qpc = next(STUB + 0x100 * (imp.address & 0xFF) for e in pe.DIRECTORY_ENTRY_IMPORT for imp in e.imports if imp.name == b'QueryPerformanceCounter')
u.mem_write(qpc, b'\x8B\x44\x24\x04\x8B\x0D' + struct.pack('<I', STUB + 0xF10) + b'\x83\xC1\x64\x89\x0D' + struct.pack('<I', STUB + 0xF10)
            + b'\x89\x08\xC7\x40\x04\x00\x00\x00\x00\xC2\x04\x00')
u.mem_write(VT + 0x800 + 16 * 3, b'\xB8\x03\x10\x00\x00\xBA\x34\x12\x00\x00\xC2\x04\x00')    # mov eax, 0x1003; mov edx, 0x1234; ret 4
u.mem_write(sym['_d3dprof_call_time'], struct.pack('<I', 1))
TRET = GAME + 0x7777; u.mem_write(TRET, b'\xF4')
sp = STACK - 0x100; u.mem_write(sp, struct.pack('<3I', TRET, 0x55AA0000, 0xC0FFEE)); u.reg_write(UC_X86_REG_ESP, sp)
seen = {}
def at3(uc, addr, size, _):
    if addr == VT + 0x800 + 16 * 3: seen['stack'] = struct.unpack('<3I', uc.mem_read(uc.reg_read(UC_X86_REG_ESP), 12)); seen['esp'] = uc.reg_read(UC_X86_REG_ESP)
h = u.hook_add(UC_HOOK_CODE, at3); u.emu_start(sym['_d3dprof_t3'], TRET); u.hook_del(h)
from unicorn.x86_const import UC_X86_REG_EDX
assert seen['esp'] == sp and seen['stack'] == (sym['_d3dprof_ret_stub'], 0x55AA0000, 0xC0FFEE), seen
assert u.reg_read(UC_X86_REG_EIP) == TRET and u.reg_read(UC_X86_REG_ESP) == sp + 8, 'did not return to the caller with the method\'s pop'
assert u.reg_read(UC_X86_REG_EAX) == 0x1003 and u.reg_read(UC_X86_REG_EDX) == 0x1234, 'EAX/EDX not as the method left them'
assert struct.unpack('<I', u.mem_read(sym['_g_ct_depth'], 4))[0] == 0
for k in range(8192):
    site, slot, used, kind, calls, red, prims, ticks, timed = struct.unpack('<IHBBIIQQI4x', u.mem_read(sym['_g_sites'] + 40 * k, 40))
    if used and site == TRET and slot == 3: break
else: raise AssertionError('no site for the timed call')
assert (calls, timed, ticks) == (1, 1, 100), (calls, timed, ticks)
print('ok: call time: the call came back through the return stub to its caller with the method\'s pop, EAX and EDX; '
      'one interval of the counter recorded at its site')
print('ok: buffer trace: the bound VB got wrapped Lock/Unlock with the right pops; Unlock recorded its 64 bytes, '
      'hash, 1 inf/NaN-pattern dword and 1 float >= 2^20, at the Lock\'s call site')
print('ok: call trace: records, frame marks, labels, matrix and vertex-data hashes, the state a draw ran with')
print('ok: odd return address kept exact; wrapper caller recorded')
print('ok: 5 calls through the thunks reached the originals with the stack intact; 1 redundant SetRenderState, '
      '1 draw of 1 primitive counted')
