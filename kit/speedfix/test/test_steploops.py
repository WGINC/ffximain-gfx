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
# 4. the walk/run rate (0x100C8930's computation, 0x100C8A2C..0x100C8B4A, the game's instructions): a character moving
#    at a steady speed S (vtable +0x1A8; its model moving S/60 a tick, so the game's rate is 1 at 60 fps). Stubs answer
#    the actor's calls (vtable +0x19C 0: the 3D length; +0x1A0 1.0; the model tests 0x10084390/0x10084350/0x10084370 0)
#    and the getter (ST0 = the step as frame_step answers it); the two model tests pop their argument (ret 4) as the game's do. The movement vector is the frame's ([esp+0x10] points at
#    it), the rate after the smoothing is [esp+4], stored at [esi+0x824] as the game's tail does (clamped to 3).
#    Compared: the game at 60 fps; frame_step=1 at 144 fps (movement and step whole ticks: frames of 0); frame_step=3 at
#    144 fps with the getter whole and the game's weights (the 6 Nov run); frame_step=3 with the rate's fix (the getter
#    the fraction, weights 1 - 0.75^s and 0.75^s, the test 0.0001 s).
STUB = SCR + 0x800
def rate_machine():
    m = machine()
    code = {0: b'\x31\xC0\xC3',                                   # xor eax, eax; ret
            1: b'\xD9\x05' + struct.pack('<I', SCR + 0x900) + b'\xC3',   # fld [speed]; ret
            2: b'\xD9\xE8\xC3',                                   # fld1; ret
            3: b'\xD9\x05' + struct.pack('<I', SCR + 0x904) + b'\xC3',   # fld [step]; ret
            4: b'\x31\xC0\xC2\x04\x00'}                           # xor eax, eax; ret 4 (0x10084350, 0x10084370 pop 4: data/purges.json)
    for k, c in code.items(): m.mem_write(STUB + 0x20 * k, c)
    vt = HEAP + 0x8000
    for slot, k in ((0x19C, 0), (0x1A8, 1), (0x1A0, 2)): m.mem_write(vt + slot, struct.pack('<I', STUB + 0x20 * k))
    m.mem_write(HEAP, struct.pack('<I', vt))
    for at, k in ((0x100C8A90, 0), (0x100C8A98, 4), (0x100C8AA3, 0), (0x100C8AAB, 4), (0x100C8AC8, 3), (0x100C8AE7, 3)):
        m.mem_write(at, b'\xE8' + struct.pack('<i', (STUB + 0x20 * k) - (at + 5)))
    return m
def rate_frame(m, s_answer, move, kw):
    """one call: ST0 of the getter s_answer; the frame's movement `move` (a length along x); kw: (new, old, thr) or None"""
    if kw:
        for at, v, off in ((0x100C8B3E, kw[0], 0x910), (0x100C8B36, kw[1], 0x914), (0x100C8A74, kw[2], 0x918)):
            m.mem_write(SCR + off, f2b(v)); m.mem_write(at + 2, struct.pack('<I', SCR + off))
    else:
        for at, c in ((0x100C8B3E, 0x10329CE4), (0x100C8B36, 0x10329D34), (0x100C8A74, 0x1032A1A8)): m.mem_write(at + 2, struct.pack('<I', c))
    m.mem_write(SCR + 0x904, f2b(s_answer)); m.mem_write(SCR + 0x940, b''.join(map(f2b, (move, 0.0, 0.0))))
    esp = STACK + 0x80000; m.reg_write(UC_X86_REG_ESP, esp); m.reg_write(UC_X86_REG_ESI, HEAP)
    m.mem_write(esp + 0x10, struct.pack('<I', SCR + 0x940))
    h = m.hook_add(UC_HOOK_CODE, lambda uc, a, sz, u: uc.emu_stop(), begin=0x100C8B4A, end=0x100C8B4A)
    m.emu_start(0x100C8A2C, 0, count=100000); m.hook_del(h)
    r = min(b2f(m.mem_read(esp + 4, 4)), 3.0); m.mem_write(HEAP + 0x824, f2b(r)); return r
SPEED = 5.0
def rate_run(fps, mode, sec=1.0):
    m = rate_machine(); m.mem_write(SCR + 0x900, f2b(SPEED)); m.mem_write(HEAP + 0x824, f2b(1.0))
    out = []; carry = 0.0; s = 60.0 / fps; tick = SPEED / 60.0
    for _ in range(int(round(fps * sec))):
        if mode == 'game': out.append(rate_frame(m, 1.0, tick, None)); continue
        carry += s; w = math.floor(carry + 1e-9); carry -= w
        if mode == 'fs1': out.append(rate_frame(m, float(w), tick * w, None))              # the model moves whole ticks
        elif mode == 'fs3_loops': out.append(rate_frame(m, float(w), tick * s, None))      # moves s (the loops), getter whole
        else:
            keep = 0.75 ** s if s < 1 else 0.75
            out.append(rate_frame(m, s, tick * s, (b2f(f2b(1 - keep)), b2f(f2b(keep)), b2f(f2b(0.0001 * s)) if s < 1 else 0.0001)))
    return out
g = rate_run(60, 'game'); a = rate_run(144, 'fs1'); b = rate_run(144, 'fs3_loops'); c = rate_run(144, 'fs3_rate')
st = lambda v: (sum(v[len(v) // 2:]) / (len(v) - len(v) // 2), min(v[len(v) // 2:]), max(v[len(v) // 2:]))
print('    walk/run rate, the last half second: (mean, min, max)  game at 60: %.3f %.3f %.3f;  144 fps frame_step=1: %.3f %.3f %.3f;  '
      'frame_step=3, loops only (6 Nov): %.3f %.3f %.3f;  frame_step=3 with the rate fix: %.3f %.3f %.3f' % (st(g) + st(a) + st(b) + st(c)))
say(abs(st(g)[0] - 1.0) < 1e-3, 'the game at 60 fps: rate 1 for a model moving its speed')
say(st(a)[1] == 0.0 and 0.2 < st(a)[2] <= 0.25 + 1e-6, 'frame_step=1 at 144 fps: the rate swings between 0 (a frame of 0 ticks: standing) and 0.25 (the next frame: 0.25 x 1 + 0.75 x 0) (the 5 Nov flicker)')
say(st(b)[0] < 0.6, 'frame_step=3 with the loops only at 144 fps: the rate well under 1 (the 6 Nov moonwalk): %.3f' % st(b)[0])
say(abs(st(c)[0] - 1.0) < 1e-3 and st(c)[1] > 0.999, 'frame_step=3 with the rate fix at 144 fps: rate 1 every frame, as the game at 60')
print('ALL OK' if not bad else '%d FAILED' % bad)
sys.exit(1 if bad else 0)
