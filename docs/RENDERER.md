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
