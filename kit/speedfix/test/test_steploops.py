"""test_steploops.py -- frame_step=3's chase loops, run on the game's own instructions (unicorn, $FFXI_DLL an unpacked
analysed build; pipeline/frame_step_loops.py, kit/speedfix/sf_main.cpp apply_step_loops).

Each loop is started right after its getter call with the step s in ST0 (as the getter leaves it) and run to its exit.
The factor speedfix changes is changed the way speedfix changes it: the operand of an `fld/fmul [K]` points at a float
holding k', a `call 0x100272B0` gets k' as its argument (sf_scale_k). Every other instruction is the game's. Checked:
  1. s = 1 with k' = k (frame_step 3 at 60 fps): every output bit-identical to the unchanged code;
  2. over the same real time, 60 fps with s = 1 (the game) against 144 fps and 75 fps with frame_step 3 (s = 60/fps,
     k' = 1 - (1-k)^(s/ceil(s))): the distance left agrees within float rounding;
  3. the same at 144 fps with frame_step 1 (whole ticks, frames of 0): it moves on only some frames (what frame_step 3
     removes): counted, not failed.

    FFXI_DLL=... python3 test_steploops.py"""
import math, os, struct, sys
import pefile
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE, UcError
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP

pe = pefile.PE(os.environ['FFXI_DLL'], fast_load=True); BASE = pe.OPTIONAL_HEADER.ImageBase
IMG = pe.get_memory_mapped_image()
STACK, HEAP, SCR = 0x00200000, 0x00400000, 0x00600000
f2b = lambda v: struct.pack('<f', v); b2f = lambda b: struct.unpack('<f', b)[0]
bad = 0


def say(ok, what):
    global bad
    print('%s: %s' % ('ok' if ok else 'FAIL', what)); bad += not ok


def machine():
    mu = Uc(UC_ARCH_X86, UC_MODE_32)
    size = (len(IMG) + 0xFFFF) & ~0xFFFF
    mu.mem_map(BASE, size); mu.mem_write(BASE, IMG)
    for a in (STACK, HEAP, SCR): mu.mem_map(a, 0x100000)
    return mu


def kprime(k, s):
    n = math.ceil(s)
    return k if n == s else b2f(f2b(1.0 - (1.0 - k) ** (s / n)))   # sf_main.cpp loop_k: in double, stored as float


def run(mu, site, s, exits, setup, scale_k=None):
    """start after the getter call at `site` with ST0 = s; stop at an exit; scale_k(k) -> the argument 0x100272B0 gets"""
    mu.mem_write(SCR, f2b(s))
    stub = b'\xD9\x05' + struct.pack('<I', SCR) + b'\xE9' + struct.pack('<i', (site + 5) - (SCR + 0x10 + 11))
    mu.mem_write(SCR + 0x10, stub)
    esp = STACK + 0x80000; mu.reg_write(UC_X86_REG_ESP, esp); setup(mu, esp)
    hooks = []
    if scale_k:
        def hk(uc, addr, size, u):
            if addr == 0x100272B0:
                sp = uc.reg_read(UC_X86_REG_ESP); k = b2f(uc.mem_read(sp + 8, 4)); uc.mem_write(sp + 8, f2b(scale_k(k)))
        hooks.append(mu.hook_add(UC_HOOK_CODE, hk, begin=0x100272B0, end=0x100272B0))
    def stop(uc, addr, size, u):
        if addr in exits: uc.emu_stop()
    hooks.append(mu.hook_add(UC_HOOK_CODE, stop))
    try: mu.emu_start(SCR + 0x10, 0, count=200000)
    except UcError as e: raise SystemExit('emulation error at %08X: %s' % (mu.reg_read(UC_X86_REG_EIP), e))
    for h in hooks: mu.hook_del(h)
    return esp


def point_operand(mu, at, to):
    mu.mem_write(at + 2, struct.pack('<I', to))


# ---------------------------------------------------------------- the loops
def camera(mu, s, gap, kval):
    """YmCameraManager_Follow 0x100205B1: [esp+0x10] = 0, [esp+0x20] = the gap; out: [esp+0x10], the distance moved"""
    mu.mem_write(SCR + 0x40, f2b(kval)); point_operand(mu, 0x100205CD, SCR + 0x40)
    def setup(m, esp): m.mem_write(esp + 0x10, f2b(0.0)); m.mem_write(esp + 0x20, f2b(gap))
    esp = run(mu, 0x100205B1, s, {0x100205FC}, setup)
    return b2f(mu.mem_read(esp + 0x10, 4))


def onmove_pos(mu, s, p, t, kval):
    """OnMove 0x100C65BA: p at [esp+0x3C], the target at [esp+0x4C]; out: p"""
    mu.mem_write(SCR + 0x44, f2b(kval))
    for at in (0x100C65EE, 0x100C65FF, 0x100C660D): point_operand(mu, at, SCR + 0x44)
    def setup(m, esp):
        m.mem_write(esp + 0x3C, b''.join(map(f2b, p))); m.mem_write(esp + 0x4C, b''.join(map(f2b, t)))
    esp = run(mu, 0x100C65BA, s, {0x100C665C}, setup)
    return [b2f(mu.mem_read(esp + 0x3C + 4 * i, 4)) for i in range(3)]


def onmove_turn(mu, s, ang, tgt, k, ks):
    """OnMove 0x100C6C64: the angles at [esi+0x61C], the target at [esp+0x54], k at [esp+0x14]; out: the angles"""
    def setup(m, esp):
        m.reg_write(UC_X86_REG_ESI, HEAP); m.mem_write(HEAP + 0x61C, b''.join(map(f2b, ang)))
        m.mem_write(esp + 0x54, b''.join(map(f2b, tgt))); m.mem_write(esp + 0x14, f2b(k))
    run(mu, 0x100C6C64, s, {0x100C6DAA}, setup, scale_k=ks)
    return [b2f(mu.mem_read(HEAP + 0x61C + 4 * i, 4)) for i in range(3)]


def actor_turn(mu, s, ang, tgt, ks):
    """0x100C47F0's loop at 0x100C4878: the angles at [esi+0x744], the target at [esp+0x24] (after its push edi)"""
    def setup(m, esp):
        m.reg_write(UC_X86_REG_ESI, HEAP); m.mem_write(HEAP + 0x744, b''.join(map(f2b, ang)))
        m.mem_write(esp - 4 + 0x24, b''.join(map(f2b, tgt)))
    run(mu, 0x100C4878, s, {0x100C4940, 0x100C493F, 0x100C493B}, setup, scale_k=ks)
    return [b2f(mu.mem_read(HEAP + 0x744 + 4 * i, 4)) for i in range(3)]


def frames(fps, seconds, mode):
    """the step each frame: mode 0 the game at 60 (1 a frame); 1 frame_step=1 (whole ticks, a carry); 3 the fraction"""
    n = int(round(fps * seconds)); s = 60.0 / fps; carry = 0.0; out = []
    for _ in range(n):
        if mode == 0: out.append(1.0)
        elif mode == 3: out.append(s)
        else: carry += s; w = math.floor(carry + 1e-9); carry -= w; out.append(float(w))
    return out


mu = machine()
K_CAM, K_POS = b2f(IMG[0x10329CE4 - BASE:0x10329CE4 - BASE + 4]), b2f(IMG[0x1032A3BC - BASE:0x1032A3BC - BASE + 4])

# 1. s = 1, k' = k: bit-identical
say(camera(mu, 1.0, 3.0, K_CAM) == camera(mu, 1.0, 3.0, kprime(K_CAM, 1.0)), 'camera, s = 1: identical')
a = onmove_pos(mu, 1.0, [0.0, 0.0, 0.0], [2.0, 0.5, -1.0], K_POS)
say(a == onmove_pos(mu, 1.0, [0.0, 0.0, 0.0], [2.0, 0.5, -1.0], kprime(K_POS, 1.0)) and a[0] != 0.0, 'OnMove position, s = 1: identical (%s)' % a)
a = onmove_turn(mu, 1.0, [0.0, 0.5, 0.0], [0.0, 2.0, 0.0], 0.125, lambda k: k)
say(a == onmove_turn(mu, 1.0, [0.0, 0.5, 0.0], [0.0, 2.0, 0.0], 0.125, lambda k: kprime(k, 1.0)), 'OnMove turning, s = 1: identical (%s)' % a)
a = actor_turn(mu, 1.0, [0.0, 0.5, 0.0], [0.0, 2.0, 0.0], lambda k: k)
say(a[1] != 0.5 and a == actor_turn(mu, 1.0, [0.0, 0.5, 0.0], [0.0, 2.0, 0.0], lambda k: kprime(k, 1.0)), '0x100C47F0 turning, s = 1: identical (%s)' % a)

# 2./3. the same real time (0.5 s) at 60 (the game), 144 and 75 (frame_step 3), 144 (frame_step 1)
def sim(fps, mode, sec=0.5):
    gap = 3.0; p = [0.0, 0.0, 0.0]; t = [4.0, 1.0, -2.0]; ang = [0.0, 0.2, 0.0]; tgt = [0.0, 2.6, 0.0]; still = 0
    for s in frames(fps, sec, mode):
        kc = kprime(K_CAM, s) if mode == 3 else K_CAM
        moved = camera(mu, s, gap, kc); gap -= moved
        q = onmove_pos(mu, s, p, t, kprime(K_POS, s) if mode == 3 else K_POS); still += q == p; p = q
        ang = onmove_turn(mu, s, ang, tgt, 0.125, (lambda k, s=s: kprime(k, s)) if mode == 3 else (lambda k: k))
    return gap, p, ang, still

for fps, sec in ((144, 0.5), (75, 0.4), (240, 0.5), (100, 0.3)):    # a whole number of ticks at each rate
    g = sim(60, 0, sec); r = sim(fps, 3, sec)
    dc = abs(r[0] - g[0]) / g[0]; dp = max(abs(r[1][i] - g[1][i]) for i in range(3)); da = abs(r[2][1] - g[2][1])
    say(dc < 1e-4 and dp < 1e-4 and da < 1e-4 and r[3] == 0,
        '%d fps with frame_step 3 against the game at 60, %.1f s: camera gap %.6f vs %.6f, position %s vs %s, '
        'angle %.6f vs %.6f; frames without movement %d' % (fps, sec, r[0], g[0], [round(x, 5) for x in r[1]],
                                                            [round(x, 5) for x in g[1]], r[2][1], g[2][1], r[3]))
g = sim(60, 0); r = sim(144, 1)
print('note: 144 fps with frame_step 1: %d of %d frames the model did not move (a step of 0); camera gap %.6f vs %.6f'
      % (r[3], len(frames(144, 0.5, 1)), r[0], g[0]))
print('ALL OK' if not bad else '%d FAILED' % bad)
sys.exit(1 if bad else 0)
