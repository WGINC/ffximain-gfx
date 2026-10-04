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
u.mem_map(STUB, 0x10000); u.mem_map(STACK - 0x10000, 0x10000); u.mem_map(VT, 0x1000)
# imports the record path uses: GetCurrentThreadId -> 7, QueryPerformanceCounter -> writes 0 (stdcall, 1 arg)
for e in pe.DIRECTORY_ENTRY_IMPORT:
    for imp in e.imports:
        code = {b'GetCurrentThreadId': b'\xB8\x07\x00\x00\x00\xC3',
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
    site, slot, used, kind, calls, red, prims = struct.unpack('<IHBBIIQ', u.mem_read(sym['_g_sites'] + 24 * k, 24))
    if used: sites[(site, slot, kind)] = (calls, prims)
assert sites.get((GAME + 0x9737, 50, 0)) == (1, 0), sites
assert sites.get((GAME + 0xCDE2, 72, 0)) == (1, 2), sites
assert sites.get((caller, 72, 2)) == (1, 2), sites
print('ok: odd return address kept exact; wrapper caller recorded')
print('ok: 5 calls through the thunks reached the originals with the stack intact; 1 redundant SetRenderState, '
      '1 draw of 1 primitive counted')
