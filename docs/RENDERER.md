# The renderer: what it calls, where, and what that means for the goals

The goals are a performance patch, a UI overhaul (the owner's wording, 6 Oct; first written here as "a possible
graphical overhaul"), and a renderer port to D3D11. docs/ROADMAP.md charts them. This
document maps the parts of the renderer they depend on. Each fact names its source. A fact rests on one
of three kinds of evidence, and the text says which:

- **proven**: the code alone settles it (com_calls.py, purges, the relocation table);
- **accepted**: rests on the owner's accepted declaration that CDx+0xC is the device (data/com_fields.json);
- **observed**: seen in the gate's runs of the original code (diffemu_gfx.py, fuzz and four memory dumps).
  An observation shows what those inputs reached, not what every execution does. The runs' inputs are
  partly fuzzed, so an observed argument value can come from fuzz.

Data: `data/render_map.json` (pipeline/render_map.py, from `FFXI_COM_SITES=1` gate runs plus
data/com_calls.json).

## 1. Direct3D call sites

1,633 call sites in the code have a known Direct3D 8 method:

| Evidence | Sites |
|---|---|
| proven | 217 |
| accepted (CDx device) | 151 |
| observed only | 1,265 |

Where both a proof and an observation exist for a site, they never disagree (`conflicts: []`). 19 further
sites reached a real object of a dump whose interface is not known here. That is fuzz reaching dump
memory; no method is claimed for them.

Of the 2,662 indirect calls in the exported graphics functions:
- 1,506 are Direct3D calls with a known method;
- 139 are resolved game virtual calls (data/vcalls.json);
- 1,017 are neither. Those are function pointers, unresolved virtual calls, and Direct3D calls no run reached.

By method (sites): SetRenderState 636, SetTextureStageState 600, SetTexture 87, SetVertexShader 63,
LightEnable 47, SetStreamSource 22, CreateVertexBuffer 17, SetTransform 16, and fewer for the rest.

### Draws are few and funnelled

Only 12 call sites draw:

| Site | Function | Method | Evidence |
|---|---|---|---|
| 0x1000CB6E, 0x1000CBAC, 0x1000CBED | CDx_DrawPrimitive | DrawPrimitive | accepted |
| 0x1000CC58, 0x1000CCA0, 0x1000CCEB | CDx_DrawIndexedPrimitive | DrawIndexedPrimitive | accepted |
| 0x1000CD53, 0x1000CD96, 0x1000CDDC | CDx_DrawPrimitiveUP | DrawPrimitiveUP | accepted |
| 0x1029BB0E, 0x1029BB42 | sqgxDlistDraw | DrawIndexedPrimitive (strip, list) | observed |
| 0x1028AFA4 | sub_1028AB50 | DrawPrimitive | observed |

Each CDx wrapper has three sites. One is the plain draw. The other two draw once per eye when
CDx_GetStereoSecondEyeTarget returns a second-eye target (tree/src/CDx.cpp, CDx::DrawPrimitive).
Callers of the wrappers across the whole binary (direct calls):
- CDx_DrawPrimitiveUP: 50;
- CDx_DrawPrimitive: 8;
- CDx_DrawIndexedPrimitive: 4.

So two layers issue the draws:
- the CDx wrappers;
- the `sqgx` display-list layer (`sqgxDlistDraw`).

A D3D11 backend, and per-draw profiling, need these few places, not 1,600.

### State the game sets

- **Render states**: 56 render states are set somewhere. Most frequent by site count: ALPHABLENDENABLE,
  ZWRITEENABLE, TEXTUREFACTOR, ZBIAS, ZENABLE, ALPHAREF, CULLMODE, SRC/DESTBLEND, LIGHTING,
  ALPHATESTENABLE, FOG*.
  - The stencil states appear too: `sqsnShadowVolumeSetPass` (0x10297D38 to 0x10297EB2, constants in
    its code) sets stencil-always with STENCILPASS DECR and CULLMODE CCW in one pass, and INCR with CW in
    the other. That is a two-pass stencil shadow volume.
- **Texture-stage states**: 15 types. COLOROP/ALPHAOP and their ARG0/1/2 dominate (the fixed-function
  combiners), then filtering and addressing, TEXCOORDINDEX, TEXTURETRANSFORMFLAGS and RESULTARG.
  - A D3D11 port has to express these combiners in shaders.
- **Vertex shaders and FVF codes**: CreateVertexShader is called at 7 sites, so programmable vertex
  shaders exist alongside FVF draws. Of the 7, one site makes all 5 programmable vertex shaders (0x10178250, the
  zone map's; with 1 pixel shader), 4 make fixed-function shaders from a declaration only, and 2 are in functions
  nothing reaches (docs/KIT.md, the static shader catalogue).
  - Values observed at SetVertexShader sites that decode as FVF codes (D3D8 SDK d3d8types.h; bit 0 clear):
    0x144, 0x142, 0x44, 0x42, 0x152, 0x2, 0x6.
  - In decoded form: XYZRHW|DIFFUSE|TEX1, XYZ|DIFFUSE|TEX1, XYZRHW|DIFFUSE, XYZ|DIFFUSE,
    XYZ|NORMAL|DIFFUSE|TEX1, XYZ, XYZB1.
  - Other observed values are fuzz or shader handles. Which is which is not settled here.

## 2. The frame (Frame_Render, 0x10011A00)

App_RenderFrameMaskedFPU (0x10001B50) masks FPU exceptions (`_controlfp(0x8001F, 0x8001F)`) and calls
Frame_Render with `this = [0x104568FC]` (the IDB calls that global g_pCamera; it holds the 0x33C-byte
object CApp_RunMainLoop constructs). In code order:

1. TestCooperativeLevel (on failure: error code, return), BeginScene, CDx_SetDefaultRenderState,
   texture upload pump (TexMgr_UploadPumpEntry).
2. Stereo mode: push the second-eye target. Then push the main scene target and clear it
   (CDx_PushRenderTargetRet1, sub_1000A390).
3. The game-state machine. In the world: the zone and actor update/draw calls (sub_1018B130,
   sub_1018C2B0, StDancer_Update, sub_1018B860(g_pXiZone), ...). These are outside the exported set and
   reach the device through CMoDX (section 4).
4. Post effects: depth blur (Post_DepthBlurComposite), Post_Afterimage, Post_FlashAndZoomFeedback.
5. CDx_SetViewport, CDx_CompositeSceneToBackBuffer.
6. UI: menus (Gfx_SetDefault2DStates, the menu manager), cursor, fade quad, movie frame, text
   (Font_DrawPass), stereo overlay.
7. EndScene, Present (CDx_Present).
8. Frame pacing (section 3), then Camera_ApplyViewProjection and GammaRamp_ApplyIfChanged.

## 3. Frame pacing: the game's own limit is 30 fps (15 in some states)

**The wait loop.** From the code (0x10012B05 to 0x10012BC6) and the timer object at `[this+0x1C]`
(vtable 0x1032A118):
- Timer method 9 (0x10019550) returns `1000.0 / (timeGetTime() - [t+0xC] - [t+0x1C])`, with a 0 difference
  counted as 1. Method 12 (0x10019230) returns the float at `[t+8]`.
- After Present, the frame computes `v = 60 / (method12() * method9())` and stores it at `this+0x28` and
  `this+0x2C`. With `[t+8] = 1.0`, `v` is the elapsed time in 1/60-second ticks.
- While `v < (int)[this+0x30]` it calls `Sleep(1)` and `pause`, then measures again (0x10012B12 to 0x10012B64).
- So `[this+0x30]` is the minimum number of 1/60 s ticks per frame. 2 is 30 fps, 1 is 60 fps.

**Who sets the minimum.** The game sets it in one function, 0x1008A0E0, which stores its argument in
`this+0x30` and in the global 0x1035B5E8, and prints the debug line `FPS制限[%d]` ("FPS limit"). It is
called only with constants:
- `2` at 0x1008A0D2 (in 0x1008A0D0), 0x10088A55 and 0x10088AC7 (in 0x100883E0), and 0x1009AAB9;
- `4` at 0x100884B3 (in 0x100883E0).

The global's initial value in the binary is 2. So the game's own code allows 30 fps, or 15 fps in the
state 0x100884B3 handles. That state is not identified here.

**In the four dumps:**
- the global 0x1035B5E8 is 2, but `[this+0x30]` is 1. Something other than the game's setter wrote 1 into
  `this+0x30` (an FPS unlock outside the game's code; which tool did it is not known here);
- `[t+8] = 1.0`;
- `[this+0x28]` is 2.0 in the two town dumps and 1.0 in combat and zonetoinn;
- `[t+4]` holds `1000/elapsed`, the last measured rate: 24.39 in the town dump.

What follows:
- **With the minimum at 1, the loop holds frames to 60 fps.** A pacer in front of Present (speedfix_hook's
  `frame_cap_fps`) cannot get more than 60 while that is so.
- **A minimum of 0 removes the wait** (`v < 0` never holds). Section 3a has what the frame then does.
- **What the dumps show:** the town dumps were taken at about 24 fps (41 ms per frame), below either limit.

### 3a. How the frame turns elapsed time into the game's time step (0x10012BC6 to 0x10012CB0)

After the wait, the frame keeps two values. Both come from the code; 0x10311C2C is MSVC's `_ftol`, which
sets the x87 rounding to truncate before `fistp`.

1. **Whole ticks, with the fraction carried:**
   - computed as `x = 60 / timer.method9() + [this+0x3C]`, then `[this+0x38] = trunc(x)` and
     `[this+0x3C] = x - trunc(x)`;
   - over many frames these whole ticks add up to the real elapsed time;
   - read through 0x10014D60 by 8 functions: 0x10088BD0, 0x10089850, 0x100B0910, 0x1012C410, 0x102095B0,
     0x10222DF0, 0x10226540, 0x1022B290.
2. **The frame step:**
   - the measured `v` of the last 4 frames goes into a ring (0x10456964..0x10456973), and the ring is
     averaged (`* 0.25`);
   - `[this+0x2C]` ends as this frame's own `v`, capped at 20 (0x10012C7E to 0x10012C95). The value is kept
     on the x87 stack from before the averaging;
   - `[this+0x28] = min(max(trunc(average), [this+0x30]), 20)`, a whole number of ticks;
   - 0x10014CF0, which the IDB names `Camera_GetFrameScale`, returns `max([this+0x28], 1.0)`;
   - 112 functions call it. The exported ones use it as the time to advance: a countdown `x -= step`
     (CXiSkeletonActor_OnMove, Actor_SelectNearestLights), an angle `angle += (int64)step`
     (Post_DrawRotatingGhostLayers), `step * 0.03125` in movement (CXiSkeletonActor_OnMove);
   - 0x10456960 is set to `60 / [this+0x28]`.

What follows from the code:
- **Below 60 fps, the frame step under-counts real time** whenever the average is not a whole number of
  ticks, because it is truncated. The dumps show it:

  | Dump | Ring (last 4 frames, ticks) | Average | Step (`[this+0x28]`) | Step / average |
  |---|---|---|---|---|
  | town | 2.46, 1.68, 1.80, 2.16 | 2.025 | 2.0 | 99% |
  | town2 | 3.18, 2.70, 3.12, 2.82 | 2.955 | 2.0 | 68% |
  | combat | 1.56, 1.56, 1.56, 1.74 | 1.605 | 1.0 | 62% |
  | zonetoinn | 1.02, 1.08, 1.02, 1.02 | 1.035 | 1.0 | 97% |

  In every dump the step is the truncated ring average, as the code says. In each of these states,
  everything advanced by the step was running at that fraction of real time.
  The whole-tick counter (1.) does not lose time.
- **Above 60 fps (minimum 0):**
  - the average falls below 1, its truncation is 0, so `[this+0x28]` becomes 0;
  - `Camera_GetFrameScale` then returns 1.0 every frame, so the 112 users advance one full tick per frame.
    By this reading, at 120 fps what they drive runs at twice real time. Not observed in the running game (17 Oct):
    players run with the tick count at 0, and what they see has not been measured here;
  - 0x10456960 becomes `60 / 0`.

  Going past 60 therefore needs the step changed. Some users convert it to an integer (`(int64)step`) and
  would stop advancing on a fractional step, so it cannot simply be made fractional. The options are
  stepping in whole ticks only on frames where one has passed (the counter in 1. already does that), or
  interpolating between ticks. Both change what those 112 functions see.

**What speedfix does with it (18 Oct, `uncap` and `frame_step`, kit/speedfix/README.md).**
- Both are edits to verified code: speedfix runs gated forks of Frame_Render, 0x10014CF0 and 0x10014D20 made from
  the tree (kit/speedfix/fork/; with the switches off each passed the gate as the game's function, with them on
  every difference is a declared one). No game byte is patched for them.
- `uncap=1`: the fork reads `[this+0x30]` (the minimum; the game's own setting, RENDERER 3) as 0, so the wait
  ends at once. The field itself is not written.
- `frame_step=1` replaces the truncation (the `_ftol` call at 0x10012C3C): from the same ring average, with the
  game's floor `[this+0x30]` and cap 20, the fork keeps a carry, as 1. above does, and the getters 0x10014CF0,
  0x10014D20 and Frame_Render's copy (0x10011A24..0x10011A3E) answer `trunc(carry + average)`. Over many frames those
  whole ticks add up to the ring averages, so to real time, at any frame rate. Above 60 most frames answer 0.
- `frame_step=2` is that, plus the untruncated average at 14 call sites in 13 functions. Those are the sites where
  every path of the returned value, followed through the caller's code, is compared, dropped or added into an
  accumulator (`pipeline/frame_step_users.py`, `data/frame_step_users.json`, `kit/speedfix/sf_step_sites.h`).
  The other 204 sites get whole ticks.
- `[this+0x28]` itself is still the game's (the truncation's result goes there as before), so 0x10456960 and
  anything reading `+0x28` directly is unchanged. Readers of the field that are not one of the getters or the
  inline copy have not been enumerated here.
- Not known:
  - code that moves a fixed amount per frame without reading the step. One such function is 0x1024DE20; how many
    there are is not known, and above 60 they run faster than real time whatever the step;
  - what the value 0 (a frame with no tick) does at each of the 204 sites: the game's own code never produced it
    (`max(step, 1)`). `frame_step_users.py` found no site where the step is a divisor;
  - below 60 fps, `frame_step` 1 and 2 remove the under-count of the table above, so what it drives moves faster
    than it does now (as fast as real time).

### 3c. The tick loops: why characters stopped and flickered above 60 with frame_step=1 (6 Nov)

The owner saw characters, the auto-running player most, flicker between two poses while moving in the 5 Nov run
(frame_step=1 above 60). From the game's instructions (6AC37846, pipeline/disasm.py; data/frame_step_loops.json):
- **Five getter calls are loop counts.** pipeline/frame_step_loops.py finds every call of the step getter whose code
  is exactly `call getter; fcom [0.0]; jne EXIT; BODY ...; fsub [1.0]; fcomp [0.0]; je BODY`: the body runs ceil(s)
  times for s > 0, not at all for s <= 0. The game never answers below 1 (`max(step, 1)`); frame_step=1 answers 0 on
  the frames above 60 that complete no tick. The five whose body is a chase toward a value fixed before the loop
  (x += (target - x) k a pass; read from each listing, recorded in the script's WHY):
  - YmCameraManager_Follow 0x100205B1: the camera's gap B, 0.25 a pass (`fld [0x10329CE4]`);
  - CXiSkeletonActor_OnMove 0x100C65BA: the model's position toward `[esi+0x5C4]`, 0.125 a pass (`fmul [0x1032A3BC]`);
  - OnMove 0x100C6882 and 0x100C6C64: the model's angles `[esi+0x61C..]` toward a target, wrapped to (-pi, pi] each
    pass, k (0.125, or a speed times 0.00125) through 0x100272B0;
  - 0x100C47F0's 0x100C4878: angles `[esi+0x744..]`, 0.125 through 0x100272B0.
  Three more loops (0x1002AD55, 0x100AAB78, 0x100AADF5) are not chases (a step and a normalisation; a velocity added
  after a test) and are left as the game has them.
- **On a frame of 0 ticks the model stops.** OnMove's movement this frame is then exactly 0 (the chase did not run);
  its four compares of that movement with 0.01 (`[0x10329A18]`, 0x100C6BBC..0x100C7100) take the standing path, and the
  movement is stored at `[esi+0x6A8]`, which slot 0x1D0 of the skeleton actors' vtables (0x100D49D0) makes a
  translation (called from Actor_SetCameraFromModelPoints and 0x1003BD90). The game's position of the character (what
  frames_player reads) moved on every frame of the 5 Nov run; the model following it did not. Run on the game's
  instructions (kit/speedfix/test/test_steploops.py, unicorn) at 144 fps with frame_step=1, the model did not move on
  42 of 72 frames. Which code chooses the pose from the standing path is not traced; the stops themselves are
  established.
- **frame_step=3** (speedfix): as 2, and these five calls get the fraction s, so the body runs n = ceil(s) passes, with
  k' = 1 - (1 - k)^(s/n): the distance left after the frame is (1 - k)^s, what s ticks leave. k' = k when s is whole,
  so at 60 fps it is the game's code bit for bit. A factor read from a constant has that instruction's operand pointed
  at speedfix's float (the constant is read by other code: 21 and 53 instructions); a factor passed to 0x100272B0 goes
  through sf_scale_k. k' is computed when the loop's own getter call is answered (the getter's fork calls speedfix with
  that s), so it is the s the loop counts with; computing it at Present, as first written, would have used the frame
  before's step (found while writing the host test, fixed before any run). Below one tick a frame, the 0.01 compares use 0.01 s (the same movement per tick). Checked on
  the game's instructions: identical at s = 1; at 75, 100, 144 and 240 fps the camera's gap, the model's position and
  angle after the same real time agree with the game's at 60 to float rounding, with no frame without movement.

- **The walk/run rate (found after the 6 Nov run, where frame_step=3 gave "moonwalking": slow walk cycles at full
  speed, blurry).** OnMove calls 0x100C83C0 (0x100C7256, 0x100C759A) with the frame's movement vector (`[esp+0x18]`,
  the chase loops' result). It calls 0x100C8930 for a rate and 0x100C8BB0, which picks the motion (idle, walk, run) and
  its playback rate from it. 0x100C8930: dist = the vector's length; dist < 0.0001 (`[0x1032A1A8]`) -> rate 0;
  otherwise rate = dist / (step x speed (vtable +0x1A8) x 1/60), the step from the getter at 0x100C8AC8 or 0x100C8AE7
  (classed whole by frame_step_users.py: the step is in a divisor through a product, which that script does not
  follow); then rate = 0.25 rate + 0.75 `[esi+0x824]` once a call, i.e. once a frame. Run on those instructions
  (test_steploops.py, the actor's calls stubbed), a model moving its speed at 144 fps: the game at 60 gives 1.000;
  frame_step=1 swings 0 to 0.25 (a frame of 0 ticks: dist 0, rate 0; the next: 0.25 x 1) -- the 5 Nov flicker; frame_step=3
  with only the loops 0.174 (dist s of a tick against a whole-tick expectation) -- the 6 Nov moonwalk. frame_step=3 now
  also gives these two calls the fraction, and below one tick a frame makes the smoothing per tick (weights 1 - 0.75^s
  and 0.75^s) and the standing test 0.0001 s: 1.000 every frame. Whether 0x100C8BB0's choice or the blur has more to it
  is not traced; the rate is established.

- **Two more chase loops, counted in integers (6 Nov, after the moonwalk).** A scan of every whole-tick site for
  `call _ftol; test eax, eax; jle; ... dec r; jne` (the first scan looked only for float counts) found two in
  YmCameraManager_Follow: 0x1001F5A2 (the camera toward its target, 0.25 a pass) and 0x10020148 (0.125). Under the first
  frame_step=3 build they still counted whole ticks: on the game's instructions at 144 fps the camera did not move on 42
  of 72 frames. frame_step=3 answers them ceil(s) (_ftol truncates, so the fraction would give no pass) with k' through
  sf_scale_k: the camera position after the same real time equals the game's at 60 (75 and 144 fps, largest difference
  9.5e-07), every frame moving. The scan's third hit (0x102160A3) counts calls of 0x10310A3C, not a chase.
- **Divisions by the step, audited.** The whole-tick sites with a division within 0x200 bytes after the call were
  read (0x1001BC9C, 0x1001F01F, 0x10035944, 0x100893CB, 0x1008D34B, 0x100B578B, 0x1011966C, 0x1019EAF2, 0x101EF282,
  0x1021F263, 0x1021F2C0, 0x10220998, 0x10220B14): in each the divisor is not the step (a timer's remaining over its
  length, a distance, a remaining tick count, a constant). The walk/run rate is the one found where the step is the
  divisor.
- **The camera's linear moves.** YmCameraManager_Follow's 0x1001F01F (step x input x 0.0279 into 0x1001EBB0, the yaw:
  the current angle around the target, fpatan, plus this, then the position at that angle) and 0x1001F0F2 (step x
  input x 0.1067 added to `[edi+0x48]`) are applied once a frame and add up: frame_step=3 gives them the fraction, so the
  camera turns and rises a little every frame instead of a tick's worth on tick frames only (read from the listing; not
  run). The zoom sites (0x1001F82F..0x1001F91D: `+0x2F8 = +0x2F4 +- 6 step`, clamped 242..900, also stored as an integer
  setting) are left whole: how +0x2F4 follows +0x2F8 is not traced.
- **The camera's follow (found after the 6 Nov run: the camera jittered while running, "it made me sick").** The
  6 Nov build listed 0x1001FA38 and 0x1001FE7C as "two integer loops with collision tests, left whole". Read through
  (pipeline/frame_step_loops.py FOLLOW_*), they are the two getter calls of one loop, `for (i = 0; i < ftol(s); i++)`,
  and it is the camera following the player, once a whole tick: a hold timer `[0x10456D7C]` -= 1.0 (set to 20 by a wall
  push; nothing more while above 0); d = |player - camera|; with setting 0x91 the turn behind the player (a counter
  `[0x10456D8C]` of 4 passes after the stick is let go, then the camera += (the point d behind the player - camera) x
  N x 0.0005 in x z, 0x100272E0); d > 100: camera += normalised(v)(d - 100)0.5; 6 <= d <= 100: camera += v(d - 6)0.012
  (one 0x100272B0 call, 0x1001FD5A); closer than 3 by a wall: camera += v(3 - d)(-0.125) and a horizontal push out of
  (3 - h)0.125. The loop 0x1001F66B/0x1001F711 brings the camera back along its path, 0.05 of the way a pass. So the
  characters moved by the fraction each frame while the camera followed by 0, 1, 0, 1 ... passes: on the game's
  instructions at 144 fps (test_steploops.py section 6), the camera's distance to a player running at 5 a second swung
  by 0.05 from frame to frame, against 0.0001 at 60 fps. Under frame_step=3 now: ceil(s) passes, every chase's factor k'
  for f = s/ceil(s) of a tick (the far form's (k/0.5)(1 - 0.5^f): the excess over 100 halves a tick; sf_follow_k tells
  the forms apart by |v|, 1 or at least 6, which the code makes so); the timer takes f a pass; the counter starts at
  round(4/f) passes (a call in place of its `mov`). Measured: the frame-to-frame swing 0.0001 at 144 fps; s = 1 bit for
  bit the game's code; after the same real time the camera within one tick's movement of the game's at 60, at 75, 120
  and 144 fps (0.02 of 6.4 standing after 0.3 s, 0.045 of 6.9 running after 1 s: the follow's factor changes with the
  distance each pass, so finer passes follow the same rule along a slightly different path; this is not removed by any
  per-pass factor, only by running the game's logic at 60 and interpolating). The counter `[0x10456D80]` (60, taken 1
  off each camera update, i.e. each frame) counts frames in the game too and is left as it is.

### 3d. Liminality: where update and drawing are in the frame (traced 10 Nov)

From the owner's runs (task_draws, 9 Nov) and the game's code:
- **The task pass** (CMoTaskMng 0x10074C30) calls each task's CMoTask_Dispatch (0x10075000): vtable +0x20, then +0x24 if
  that returned 0. Every draw of the pass (~893 a frame in the plaza) is made by three tasks: CXiActorDraw (vtable
  0x10329C44, +0x20 0x100817E0), the zone (0x10336748, 0x10178B30) and CXiActorNameDraw (0x10329C18, 0x100817F0). The
  actors' own tasks (CXiSkeletonActor, +0x20 = OnMove 0x100C63D0, +0x24 an empty `ret`) draw nothing.
- **The camera** is updated from the player's OnMove: YmCameraManager_Follow (0x1001EE60) is called from 0x100A5E10,
  which CXiSkeletonActor_OnMove calls at 0x100C81F0 (and 0x1008CD90 at 0x1008D2B5). So the camera moves when the
  movement tasks run.
- **The actors' drawing:** CXiActorDraw's +0x20 calls CXiActorDraw_UpdateDrawList (0x10082D70): it walks the draw list
  (g_pActorDrawListHead, linked at +0x54 / +0x58), keeps it sorted by the float at +0x8C, and for each actor with
  [+0xB2] == 0 and [+0x8C] >= 0 calls its vtable +0x288 -- for CXiSkeletonActor RunStaggeredUpdates (0x100CBD60) ->
  PerActor_FourBucketStaggeredUpdate (0x100CBDE0). That function, every frame, per actor: the occlusion probe
  (Occlusion_QueryScreenRect, every 4th frame by GetFrameCounter() % 4 and the actor's bucket), the head look
  (CXiSkeletonActor_UpdateHeadLook), the nearest lights and the shadow's light direction (Actor_SelectNearestLights,
  Shadow_SmoothLightDirection: both read the step getter), the animation (CXiSkeletonActor_AnimateModels ->
  Model_AnimateAndPose -> 0x1002AC60, whose loop reads the step at 0x1002AD55; and 0x10035390 at 0x10035944), and the
  world matrix at +0x998: scale from vtable +0x110, rotation from vtable +0x1C0 (CXiSkeletonActor_GetDir: +0x61C) plus
  +0x62C, position from vtable +0x1BC (CXiSkeletonActor_GetPos: +0x5FC) plus +0x60C; then CXiSkeletonActor_DrawModel.
  +0x5FC and +0x61C are the vectors OnMove's chase loops move (RENDERER 3c: "p = the model position (vtable +0x1BC)",
  "the angles [esi+0x61C..0x624]").
- **So under Liminality stage 1** (frames of 0 ticks: only the three drawing tasks run, every step getter answers 0):
  nothing moves, the camera with it; the drawing path's step readers (the animation loop, the shadow and lights) get
  0, so the pose holds too. The occlusion probes and the staggered buckets count frames (GetFrameCounter), not ticks,
  as in the game at any frame rate.
- **Stage 1's flashing (10 Nov run), cause from the code:** OnMove and the drawing path hand each other four fields.
  OnMove's start clears +0xB2 bit 0 (0x100C641C) and masks +0x6A4 to bit 0x10 (0x100C640E); it sets +0x8E8 = 1
  (0x100C7F4C). The drawing path sets +0xB2 bit 0 and +0xB0 = 1 when the occlusion count +0x9F4 is 0
  (0x100CC69B..0x100CC6AE), and resets +0x8E8 (0x100CBD7B). A frame without OnMove drew with the drawing's own marks of
  the frame before. speedfix now keeps the four fields after each OnMove and restores them on a skipped frame.
- **Stage 2 (built 11 Nov, speedfix sf_main.cpp):** +0x5FC and +0x61C are kept for each actor's last two ticks. Every
  frame the actor is drawn at their blend by carry / (ticks its last OnMove covered), angles the short way round, no
  blend across a move of 50 or more. The tick's values go back before OnMove.
  The camera's view is built in Camera_ApplyViewProjection (0x10015510, Frame_Render's end) from the camera at
  [g_pCamera+0x50]: eye +0x44, target +0x50, roll +0x5C, up +0xA8 (builder FUN_10021C10). FUN_10021B70 caches +0x60 /
  +0x6C for change detection. Camera_UpdateController (0x10021900) snaps +0x44 / +0x50 from +0x8C / +0x98 on the
  requests +0x88 / +0x89. So the eye and target are blended around that call (the Frame_Render fork, `xi_lim_cam`)
  and put back after; not with a snap pending.
  What is drawn is up to one tick behind the game's state.
- **The animation's step reads in the drawing path:**
  - 0x1002AC60's loop (0x1002AD55) is a direction chase kept at +0xB0, v = normalize(31/32 v + 1/32 T) a tick. It is
    unchanged on a frame of 0 ticks, as the game at 60 fps.
  - 0x10035944 (0x10035940, reached through the vtable at 0x1032A924) is a timed value: [+0x38] -= step, then the
    vtable +0x18 callback at or below 0, else lerp(+0x40, +0x44) by 1 - rem / len. frame_step_users.py: whole (an
    indirect jmp it does not model).
  - A walk by direct calls from the three drawing tasks finds 8 step reads (939 indirect calls not followed). One is
    a fraction site: the zone's 0x10178BD0, a 0-to-1 progress g += step / (60 x seconds) (0x10177650 / 0x101776C0 set
    the rate). Under Liminality it now gets the frame's fraction (it runs every frame).
  - **The motion clock (found 11 Nov) is advanced in OnMove, not in the drawing.** The chain:
    - CXiSkeletonActor_OnMove calls CYyModel_ForEachPart130 (0x1002C2A0) at 0x100C7FA7 with the model at +0x674 and
      the actor.
    - ForEachPart130 calls, for each part 0x1002B5A0 lists, 0x1002A130. That function does `add ecx, 0x24` and
      jumps to MotionQueList_ProcessAll (0x1001A3E0, the jmp at 0x1002A133; it has no other caller).
    - ProcessAll goes over 7 queues (+0x78, then every 0x14 down). Each queue gets CYyMotionQue_Update (0x1001B2E0),
      then KzMotionQueList_NotifyAll (0x1001B1F0).
    - Update calls 0x1001B5D0 for each element: a blend weight +0x10 and timers +0x0C / +0x14, by step (read at
      0x1001B5D4).
    - NotifyAll calls each element's vtable +0x34 with 2 arguments. In CYyMotionQue's vtable (0x1032A1C0), slot 13 is
      CYyMotionQue_ComputeFrameAdvance (0x1001A8E0, `ret 8`): frame +0x24 += step x ([res+0x36] x 0.5) x speed
      +0x28 (step read at 0x1001A9AB). The duration is [+0x38] if above 0, else the resource's word +0x34.
    - Looping: a looping motion wraps by subtracting the duration while the frame is at or past it (adding it while
      at or below 0, played backwards). +0x2C counts the loops left. At the end the frame is held, +0x28 = 0, and
      vtable +0x1FC of the actor is called.
    - Both step reads are fraction sites (frame_step_users.py).
    The drawing path only samples the pose at that time: Model_AnimateAndPose -> MotionQueue_UpdateAllChannels
    (0x1001A420, called at 0x1002A27B) -> CYyMotionQue_SampleInto (0x1001B230), each element's vtable +0x38. That is
    why the poses change at 60 Hz under Liminality.
  - **The limbs, built 12 Nov (speedfix lim_limbs, default on under Liminality):**
    - After each OnMove, every CYyMotionQue element of the actor's model is kept: its frame +0x24 and weight +0x10.
      The elements are found at model +0x674+0x44 -> parts (linked at +4) -> part +0x24 -> 7 queues -> list at
      queue +4 (linked at +4), and only those whose vtable is CYyMotionQue's, checked live.
    - A frame is blended only when its change in that OnMove is exactly ComputeFrameAdvance's advance, wrapped once
      by the duration or not.
    - A weight is blended only in fade state 3 at both ticks, when it changed by exactly step x [+0x18].
    - Blended before CXiActorDraw's +0x20 for the draw list's actors ([0x1047D578], next +0x54), from their live
      lists, where they still hold the tick's values; put back right after.
    - test_steploops section 7 runs the game's 0x1001A8E0 from its entry on 12 cases (forwards, backwards, wraps,
      +0x38, the duration-2 +1 case). speedfix's prediction matches every one.

### 3b. The owner's run of 26 Oct: game speed following the frame rate under d3d8on11

The owner saw walking, camera, map movement and running animations speed up and slow down with the frame rate
in runs A and B, both under d3d8on11, and not under dgVoodoo. What is known, and from where:
- **The minimum is off.** The owner reports that Windower sets the frame divisor to 0. With `[this+0x30] = 0`
  the wait loop never waits, and above 60 fps `Camera_GetFrameScale` answers 1.0 every frame (3a). So what the
  112 users drive advances one tick (1/60 s) per frame, whatever the frame's real length.
- **The frames were short.** In run A's timing log under d3d8on11, frames ran 5 to 7 ms for long stretches
  (140 to 200 fps). By 3a, that is 2.4 to 3.3 times real time for step-driven motion, varying with the frame
  rate as the owner saw.
- **d3d8on11 does not wait for the display.** Its Present is DXGI_SWAP_EFFECT_DISCARD with `Present(0, 0)`:
  the game's presentation interval and swap effect are not honoured (kit/d3d8on11/d8_exec.cpp:455 and :844). Under dgVoodoo the
  frame is held to the display; that dgVoodoo honours the interval is inferred from the owner's observation,
  not measured here.
- **speedfix's limiter did not hold the frame either.** In run B (`precise_pacing`), the limiter's log shows
  0 ms waits after its first phase, with `[this+0x30] = 1` read. Why is not settled.

The fix on the d3d8on11 side is to honour the game's presentation interval (a sync interval of 1 for
D3DPRESENT_INTERVAL_ONE / DEFAULT). At 60 fps, a minimum of 0 or 1 gives the game's own speed. Above 60, the
step needs speedfix's `frame_step` (3a).

## 4. Coverage gaps that matter for the goals

- **The CMoDX device users.** Before the 4 Oct export, 99 functions outside it used the device (93 through the
  CMoDX object at 0x1047BFA8, whose +0xEBC is the device, proven in com_fields.json). The 4 Oct export (867
  functions) includes them; data/gfx_device_users.json now finds none outside it. Some are not in the verified
  tree: the occlusion readback (0x1006C8B0) is UNRESOLVED at one call (docs/GAPS.md).
- **The 3D world path.** The world update/draw functions called from the state machine (section 2) are
  outside the export.
- **The x87 comparison artifacts.** The `__builtin_isunordered` chains in the tree (Frame_Render's pacing
  loop has five) are the x87 compare idiom. The next IDA export's per-comparison instructions are meant
  to remove them.

## 5. Measured costs (owner's speedfix_hook, on dgVoodoo / D3D11 / Parallels)

These numbers are the owner's measurements, recorded in speedfix_hook.cpp. They were not measured here.

- **Occlusion readback** (0x1006C8B0):
  - What it does: CopyRects of a shared 16x16 target, then a read-only LockRect. The CPU waits for the GPU there.
  - Cost: about 4 to 5 ms for the first readback in a frame, about 1.2 ms for each further one; about
    2.3 ms per readback in another measurement.
  - Callers: six. Two are actor paths (0x100CC688, 0x100CD011); the others include 0x1004A960, 0x1004E6FF,
    0x10054AE2 (in 0x100548D0) and 0x100AC894.
  - Checked here: Occlusion_QueryScreenRect (0x1006C280) has those six callers and pops 36 (`ret 0x24`);
    0x1006C8B0 has the one caller 0x1006C4D7.
- **DrawPrimitiveUP:** measured at about 20% of the frame's traced stall time.
- **Redundant sets:** measured redundant fractions, by call (not yet cached by default):

  | Call | Redundant |
  |---|---|
  | SetLight | 83% |
  | SetIndices | 70% |
  | SetViewport | 62% |
  | LightEnable | 33% |
  | SetStreamSource | 20% |
  | SetTransform | 5 to 9% |

What the map adds to these: per call site attribution. kit/d3dprof counts each method per call site, and
the call sites map to the functions in data/render_map.json and the reconstructed tree.

## 6. What this means for each goal

- **Performance patch.** The occlusion readback is a GPU-to-CPU synchronisation on every query. D3D8 has
  no asynchronous occlusion query, so speedfix staggers and skips it.
  - In a D3D11 backend it becomes an `ID3D11Query` (D3D11_QUERY_OCCLUSION) read a frame or more later,
    with no stall.
  - The reconstructed 0x1006C8B0 and its callers are where that change is made once the next export
    brings them in.
- **D3D11 port.** The draws come from two layers (CDx wrappers and sqgx display lists). The state is
  fixed-function: render states, texture-stage combiners, FVF and a few vertex shaders.
  - A backend at the CDx/sqgx layer sees each draw with its purpose, which a D3D8-to-D3D11 wrapper like
    dgVoodoo cannot.
  - The fixed-function combiner states seen in section 1 are the shader permutations it has to cover.
- **Frame rate:** the game's own setter allows 30 (or 15); 60 needs `this+0x30 = 1`, as the dumps already
  show. Above 60 depends on how the frame's tick consumers behave with no whole tick (section 3).

## 7. First in-game profile (owner's capture, d3dprof, process 1368)

Source: one CSV of 600 frames, taken without speedfix_hook (owner's statement), so the counts are the
game's own calls with no cache or batching in between. The call sites were joined with the code by
`kit/d3dprof/analyze.py`. A row counts only if the instruction ending at its return address calls through
that method's own vtable slot, or calls a wrapper that tail-jumps through it. 523 of 526 rows pass. The
other 3 are TestCooperativeLevel from outside FFXiMain (0x67CBxxxx, 7.5 calls per frame).

That CSV came from the first d3dprof build. It cleared bit 0 of FFXiMain call sites with an odd return
address and labelled them `other module` (fixed in v2). analyze.py restores them, and the slot check
decides each one.

**Frame:** average 21.7 ms (46 fps), minimum 16.25 ms, maximum 56.3 ms.

**Per frame:**
- 8,892 Direct3D calls;
- 1,181 draws, 23,977 primitives:

  | Method | Calls | Primitives | Primitives per call |
  |---|---|---|---|
  | DrawPrimitiveUP | 1,077 | 4,049 | about 3.8 |
  | DrawIndexedPrimitive | 79 | 5,734 | about 73 |
  | DrawPrimitive | 24.8 | 14,195 | about 572 |

  So 91% of the draw calls carry 17% of the primitives.
- Every draw went through the CDx wrappers' plain sites (0x1000CBED, 0x1000CCEB, 0x1000CDDC). The sqgx
  display-list draws did not occur in this capture.
- 3,558 sets repeated the value the state already had: 40% of all calls. By method:
  - SetVertexShader: 1,094 of 1,118;
  - SetRenderState: 1,526 of 2,966;
  - SetTextureStageState: 492 of 2,328;
  - SetTexture: 336 of 511.
- 3,220 calls in the whole capture (5.4 per frame) came from a thread other than the rendering thread.
  d3dprof counts them but does not record their methods.

**Where the calls come from:** nine functions named Cell_*, Font_*, Gfx_SetAdditiveBlend and
sub_1011E980 (0x1011BAE0..0x1011F9xx, plus Font_DrawPass and Font_DrawString) issue:
- 5,084 of the 8,892 calls (57%);
- 2,695 of the 3,558 redundant sets (76%).

The largest single sources:

| Function | Calls | Redundant | Notes |
|---|---|---|---|
| Cell_DrawQuadBlended | 2,548 | 1,684 | |
| sub_1011E980 | 1,556 | 271 | |
| Font_DrawGlyphQuad | 669 | 669 | all SetVertexShader with the shader already bound |
| CYyModelDt_SkinAndDrawV2 | 453 | 360 | |
| Model_EndOffscreenPassBindTextures | 342 | 124 | |

The names are the IDB's.

**What this shows for the goals:**
- **The draw-call count is in tiny DrawPrimitiveUP batches** (about 4 triangles each), and the state
  traffic around them comes mostly from the Cell/Font functions. Batching those (speedfix's
  `batch_up_draws`) and dropping the repeated sets (its `state_cache`) address exactly this.
- **In the reconstructed code it can be fixed at the source.** Set state once per run of quads instead of
  per quad. The next capture (d3dprof v2) records the caller of each CDx draw wrapper, so the 1,077 UP
  draws can be assigned to the functions that ask for them.

## 8. Second capture (d3dprof v2, process 2788): who draws

600 frames, without speedfix_hook. Of the 644 rows:
- 617 call sites pass the slot check;
- 24 draw requesters pass (each a direct call to a CDx draw wrapper ending at the recorded address);
- 3 are TestCooperativeLevel from outside FFXiMain.

**Frame:** average 26.2 ms (38 fps), minimum 18.2, maximum 44.3.

**Per frame:** 14,492 calls, 1,839 draws, 445,652 primitives, 4,098 sets that repeat the current value.

**Who asks for the draws** (the CDx wrappers' callers, per frame):

| Requester | Draws | Primitives |
|---|---|---|
| sub_1017C9C0 (a CTsZoneMap method: the zone mesh) | 781.6 DrawPrimitive + 39.4 DrawIndexedPrimitive | 389,381 + 2,412 |
| Font_DrawGlyphQuad | 543.2 DrawPrimitiveUP | 1,086 (2 per call: one quad per glyph) |
| Cell_DrawQuadBlended | 181.2 DrawPrimitiveUP | 362 |
| CYyModelDt_SkinAndDrawV2 (six sites) | 148 DrawIndexedPrimitive | 9,709 |
| CTsZoneMap_DrawProjectedShadow | 68.8 DrawPrimitiveUP | 192 |
| sub_10183760 | 9.7 DrawPrimitive | 40,261 |
| 0x10043F40 (a device user, in the new export) | 16.5 DrawPrimitive | 2,146 |

**The zone mesh function, sub_1017C9C0.**
- It issues 4,150 calls per frame, 1,034 of them redundant. Around each of its 782 draws it sets the
  stream source (0x1017DD95, 781.6), the texture (0x1017DCCB, 773.7, 476.5 redundant) and render states
  (0x1017DB60 546.7; four sites at 273 each).
- It is in the verified tree (tree/src/unit_10178790_ZoneMap.cpp, gate PASS on fuzz and all four dumps).
  That makes it the first place to change. In a D3D8 performance patch: sort and merge its draws by
  texture and state. In a D3D11 port: put the zone in a few large buffers and draw it with far fewer calls.

**The glyph quads.** Font_DrawGlyphQuad draws one quad per glyph (543 per frame here) and sets the vertex
shader before each, every time to the one already bound (543 of 543 redundant).

**Calls from another thread.** d3dprof's `other_thread_calls` is cumulative since injection, not per
window. Both captures show 3,220, so those calls were made once, early, and not during play.

**Not measured:** time inside the calls. The 38 fps here against 46 fps in the first capture goes with
18 times the primitives (445,652 against 23,977), but which calls cost the time needs timing, not counts.

## 9. Where the world pass ends (28 Oct 2026)

ffxi_world_draw (docs/COMMUNITY.md) draws its geometry at the end of the game's world pass, and finds that end in
the game's own state. What our checks show, and what is that source's statement:

- **Checked here.** CDx (`g_pCDx`, 0x1045666C) keeps a render-target stack: entries indexed by the one-byte depth
  at +0x158 (`renderTargetDepth`), the world's target at +0x194 (`sceneTargetA`), the device at +0x0C. The push
  (CDx::PushRenderTargetTexture, 0x10009F20) and the pop's worker (0x1000A2B0, from PopRenderTargetA 0x1000A200)
  both store the new depth to +0x158 (0x10009F3B, 0x1000A2BC) before their first call through vtable slot +0x7C,
  SetRenderTarget's (0x10009F57, 0x1000A2F6). Which object those calls go through is not traced here.
- **The source's statement.** A pop that lowers the depth while +0x194 is still the bound target is the end of the
  world pass; at that moment the target, its depth buffer and viewport are the world's. The camera to draw with is
  the game's camera object (`g_pCamera`, 0x104568FC: view at +0x94, projection at +0x154, both accessed by the
  functions that load it), not the device's transforms: the model pass draws with the view set to identity, and
  the game restores its view from that object after each borrower.

For the goals: this is a boundary in the game's own state between the world and what follows (UI), for the
whole-frame differential (#131), work inserted into the world pass on the D3D11 path (#143, #144), and the UI pass
(#140, #145). Before it is used as a fact, the end-of-pass rule is to be measured (a capture that marks the pop).
The measurement (31 Oct): d3dprof.c records `[g_pCDx]+0x194` as the hash of every SetRenderTarget in a trace, and
`kit/d3dprof/trace_diff.py --part world` cuts each frame at the first SetRenderTarget away from that target while it is
bound, printing the cut's call site. The rule holds in a trace where that site is the pop's in every unswapped frame
(return address 0x1000A2F9). The first trace with it (31 Oct) showed +0x194 holds a texture, not the surface bound (the value never matched a SetRenderTarget surface); the kit now records that texture's level-0 surface (HISTORY 31 Oct, evening). Not yet measured. A trace of 13 Oct (before the field) shows the pushes
(return 0x10009F5A) and pops (0x1000A2F9) in pairs within each frame.
