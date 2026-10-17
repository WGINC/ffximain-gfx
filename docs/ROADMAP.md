# Roadmap: the three goals

The owner's goals, as stated on 6 Oct:
1. a **performance patch**;
2. a **renderer port to Direct3D 11**;
3. a **UI overhaul**.

docs/RENDERER.md, written earlier, phrased the third as "a possible graphical overhaul".

This document charts the performance patch step by step, and lists for each goal what it needs and what exists
already. Every number names its source. Where something has not been measured or established, the text says so.

## 1. What exists (shared by all three)

| Piece | State | Where |
|---|---|---|
| Reconstructed graphics code | 777 functions, gate: 774 PASS, 2 INCONCLUSIVE, 1 FAIL; 620 fully covered | `tree/`, `verification/` |
| In-game kit | Swaps the reconstruction in function by function, with checks, quarantine, bisect, per-call-site profiler (d3dprof) and alternating A/B trace | `kit/`, docs/KIT.md |
| SDK (mods) | A mod hooks any of 774 functions with a type-checked replacement, calls the game's own version through a trampoline (721 of them; the reconstruction for 53), and draws each frame | `sdk/`, docs/SDK.md |
| Renderer map | 1,633 Direct3D call sites with known method, the draw funnel (12 draw sites in 2 layers), frame structure, pacing | docs/RENDERER.md, `data/render_map.json` |
| UI layer facts | 11 TkMenu functions matched and checked against the owner's declarations; the menu manager object | docs/UI.md |
| In-game evidence | 8 profiles, alternating traces 11000_1..4 (no difference with swaps on beyond what the scene does), screenshots | docs/HISTORY.md |

**What the in-game swaps have reached.** In the owner's session 11000 (25 minutes), 236 of the 619 swapped
functions ran at all (`xikit_calls_11000.csv`). The other 383, along with the 155 not swapped by default, have
only the gate's evidence.

## 2. What the profiles say (d3dprof v2, 600 frames each, the owner's machine)

The owner's setup: Direct3D 8 runs through dgVoodoo on D3D11 under Parallels (RENDERER.md section 5). The same
person and machine took every capture; the scenes differ and were not always stated. The four marked
"broken-picture period" were taken while 7 swapped quad functions drew from stack garbage (fixed since), which may
have changed what was drawn.

| Capture | Swaps | Frame ms (avg) | D3D calls / frame | Draws | Primitives | Redundant sets |
|---|---|---|---|---|---|---|
| 9176 | on | 18.0 | 6,131 | 926 | 20,637 | 2,435 |
| 1368 | (before the kit) | 21.7 | 8,892 | 1,181 | 23,977 | (v1) |
| 2788 | (before the kit) | 26.2 | 14,492 | 1,839 | 445,652 | 4,098 |
| 2836 | on (broken-picture period) | 30.1 | 13,220 | 1,359 | 95,449 | 3,998 |
| 11000 | on | 46.5 | 23,861 | 2,318 | 170,445 | 8,100 |
| 7428 | on (broken-picture period) | 49.1 | 24,402 | 2,075 | 119,459 | 7,923 |
| 8608 | on (broken-picture period) | 54.0 | 26,304 | 2,922 | 297,205 | 8,727 |
| 6956 | on (broken-picture period) | 59.1 | 32,413 | 3,164 | 289,335 | 9,914 |

**Observed: frame time follows the call count.** Over these eight, the fit is frame ms ≈ 6.6 + 0.00168 ×
calls, with r = 0.988. Primitives do not follow it: 2788 drew 445,652 primitives per frame in 26.2 ms.

This is a correlation across different scenes, not a measurement of cost. A busier scene also has more draws,
more geometry and more game logic. It is consistent with a per-call cost of about 1.7 µs through
dgVoodoo/Parallels, but that has to be measured (P0 below) before the plan relies on it.

**Redundant sets are about a third of all calls:** 8,100 of 23,861 per frame in 11000. Each one sets a state to
the value it already has.

**Who makes the calls in the busiest well-attributed capture (11000).** Calls from swapped functions are named
through the reconstruction's exports (`kit/d3dprof/trace_diff.py` Owners). Draws are counted at the CDx wrappers,
not at the functions that asked for them.

| Function (manifest flags) | Calls / frame | Redundant | Notes |
|---|---|---|---|
| CYyModelDt::SkinAndDrawV2 (4) | 5,330 | 3,830 (72%) | Character skinning and drawing |
| fn_100467E0 (4) | 2,481 | 6 | Calls CDx::SetLight (3 sites), XiZone_GetFog, XiZone_GetDirLight, XiZone_GetAmbient (its body: tree/src/CMoElem.cpp) |
| Gfx_ApplyCachedStreamSource (0) | 1,793 | 388 | |
| Cell_DrawQuadBlended (4) | 1,289 | 890 | UI quads |
| CXiSkeletonActor::DrawModel (0) | 1,244 | 297 | |
| CDx::DrawPrimitiveUP (0) | 1,090 draws | | About 2 primitives each |
| fn_1017C9C0 (4) | 1,066 | 161 | The zone mesh (4,150 calls in capture 2788) |
| Font_DrawGlyphQuad (0) + Actor_DrawWorldText (4) + ShapeMng_BindFontTexture (0) | 598 + 588 + 172 | 582 + 564 + 150 | Text |
| CDx::DrawPrimitive (0) | 530 draws | | 128,960 primitives |

All of these are in the manifest, so a mod can hook every one of them today. Most carry flag 4: the gate's trials
did not run all of their instructions. A rewrite that works from their reconstructed bodies relies on code no
trial checked (P2 below).

**Not measured yet:**
- time inside Direct3D calls;
- time spent waiting in Present (whether the GPU or the CPU sets the frame rate);
- CPU time per game function;
- which scene each capture shows.

## 3. Goal 1: the performance patch

**Form.** xikit plus one performance mod (`sdk/`), so each fix can be turned on and off live and A/B-traced. Some
fixes are better made at the device level (a wrapper over IDirect3DDevice8, as d3dprof is), some at the source (a
hooked function rewritten from its reconstructed body).

speedfix_hook (the owner's, GPL-3.0-only by its SPDX header) already does several fixes at the device level. Its
measured results and its crash-safety rules are inputs here. Whether its code may be carried into this repo is
the owner's decision. Until then the fixes are reimplemented.

**Rules for every step:**
1. Measure before and after with the same scene and settings (d3dprof, plus P0's timing).
2. Run an alternating trace with the fix on. It may show only the intended differences: fewer redundant sets,
   merged draws. Any other STABLE, OUTSIDE or COUNT finding blocks the fix.
3. A rewrite of a function starts from its reconstructed body and keeps what it computes.

### P0. Measure where the time goes (the next step; nothing below is chosen without it)

| Tool | What it answers | State |
|---|---|---|
| Present wait timer (xikit) | Time inside Present per frame, against frame time: is the GPU or the translation stack the limit? | Built: xikit_timing_<pid>.csv |
| Per-call timing (d3dprof) | Time inside each Direct3D method, per call site: the actual cost per call under dgVoodoo | Built: `call_time=1` (with `profile=1`): `us_per_call` per (method, call site) in d3dprof_<pid>.csv; also the sampler's "elsewhere" view |
| Sampling profiler of the render thread (xikit) | Exclusive CPU time by function and by module (game, reconstruction, the D3D8 runtime or wrapper, driver, Sleep in the pacing loop) | Built: `sample=1`, kit/d3dprof/samples.py |
| Fixed test scenes | Repeatable captures: a busy town, an empty zone, combat; each with speedfix off | Owner, in game |

**Output:** a table of where the 46.5 ms of capture 11000 go. It also settles whether call count is the lever
the correlation in section 2 suggests.

### P1. Remove redundant state sets (device level, low risk)

- **What:** skip a set that repeats the value currently set: SetRenderState, SetTextureStageState,
  SetVertexShader, SetTexture, SetIndices, SetStreamSource. Clear the cache when a state block is applied or the
  device is reset.
- **Expected effect:** about a third of calls gone (8,100 of 23,861 in 11000). d3dprof already counts each
  redundant set by call site.
- **Evidence it works:** speedfix's `state_cache` and `vertex_shader_cache` do this in game. What they gained is
  not recorded here.
- **Risk:** state the cache does not see. Direct3D 8 state blocks (ApplyStateBlock: 329 calls per frame in 11000)
  change states behind it, and Reset or CreateStateBlock does too. Invalidate on those, as speedfix does
  (`state_cache_reset_each_scene`).
- **Next (11 Oct, HISTORY.md):** forgetting at each ApplyStateBlock is what defeats the cache in the effect drain
  (18% of the plaza frame's calls, 544 blocks applied inside it). The game records each block once from a fixed
  table, so speedfix can keep each block's recorded values and set its shadow to them on apply. Simulated on the
  plaza trace, with only the drain's two blocks known: 20,645 to 18,423 calls a frame with lazy_state, and 24,347
  to 22,391 without it. Built as speedfix's `block_cache` (off by default). The documented behaviour is checked by
  probe8 through the game's d3d8.dll (it passes under Wine); the owner's run and an A/B are still to come.

### P2. Fix the hot functions at the source (SDK mods)

Order by P0's timing. Below is the order by calls in 11000. Before rewriting a flag-4 function, raise its gate
coverage (see the prerequisites in section 6). Its reconstructed body is the specification, and parts of it are
unchecked.

1. **CYyModelDt::SkinAndDrawV2** (5,330 calls, 72% redundant). Sets state per mesh piece that repeats.
   Rewrite: set once per model and sort pieces by state. It also writes vertex buffers every frame: 34 unlocks, 278,256 bytes
   per frame in trace 9176. The name and the contents changing every frame suggest CPU skinning; that is not
   established. Whether moving that to a vertex shader pays depends on P0.
2. **Text: Font_DrawGlyphQuad, Actor_DrawWorldText, ShapeMng_BindFontTexture.** One quad and one draw per
   glyph (RENDERER.md section 8). 582 of Font_DrawGlyphQuad's 598 calls per frame repeat the current state. Rewrite: collect glyphs
   per font texture and draw each batch once. Same for UI quads in **Cell_DrawQuadBlended** (890 of 1,289
   redundant).
3. **Small DrawPrimitiveUP calls** (1,090 per frame, about 2 primitives each). Merge consecutive ones with the
   same stride and state (speedfix `batch_up_draws`), or better, at the source in 2.
4. **The zone mesh, fn_1017C9C0** (4,150 calls in 2788, 1,066 in 11000): one draw per piece, with the stream
   source and texture set around each. Rewrite: sort by texture and state; later, a few large buffers.
5. **fn_100467E0** (2,481 calls, almost none redundant). It sets lights from the zone's fog, directional and
   ambient light each time it runs, and its sets are not repeats. Any gain would come from setting them less
   often. That is a change of method, so it comes
   after P0 shows its cost.

### P3. The occlusion readback

0x1006C8B0 is a GPU-to-CPU wait on every query. The owner measured 4–5 ms for the first query in a frame and
1.2 ms for each further one (RENDERER.md section 5).
- **Not hookable yet.** It is in the export, but the gate leaves it UNRESOLVED (docs/RESIDUE.md): its call at
  0x1006C8CD (a Direct3D call on the occlusion target, GetSurfaceLevel by its description) has no definitive
  pop count, so it is not in the tree or the manifest. Settling that one call (a proof of the object's
  interface, or the pop observed in the running game, docs/GAPS.md) brings it in.
- **Its three callers in the manifest** (Occlusion_QueryScreenRect, Occlusion_QueryScreenRectImpl,
  Occlusion_DrawQuery) can be hooked now. A mod can stagger or skip queries the way speedfix's `occlusion_stagger`
  and `skip_*` options do.
- **The real fix** (results read a frame later, no wait) needs that call settled and the function gated.
  Under D3D11 it is an asynchronous query (goal 2).

### P4. Above 60 fps

RENDERER.md section 3: the game holds at most 60 fps through `this+0x30`. Its time step truncates whole ticks,
which under-counts time below 60 fps (the dumps show 62–99% of real time). Above 60 the step reaches 0 ticks,
and 112 functions see a full tick every frame.
- **Going past 60:** the step has to change: whole ticks only on frames where one has passed, or interpolation.
  Both change what those 112 functions see, and Frame_Render is flag 4. This comes last, with its own tests.
- **The under-count below 60 is a correctness issue as well as a performance one.** Fixing it is a separate
  decision for the owner.
- **Reaching 60 itself (17 Oct):** the limiter (0x10011A00) times frames with timeGetTime's whole milliseconds.
  With a tick count of 1 and a scale of 1.0 that gives 17 ms frames, 58.8 fps. speedfix's `precise_pacing` times it
  with the performance counter instead, and `kit/sessions/perf60` measures it in the game. The owner's tick count and
  scale are not established; the lever logs them.
- **Why above 60 is not a setting (17 Oct):**
  - The step is `max(step, 1)` ticks a frame (frame_cap_fps's note in kit/speedfix/README.md).
  - Letting the limiter through faster (`[limiter+0x30]` = 0) still gives at least one tick a frame. So at 120 fps
    the game's clock, Camera_GetFrameScale's 1.0 and everything that reads them run at twice real time.
  - A higher rate needs the game's update separated from its rendering. The update would run on whole ticks and
    the rendering would run as often as the device allows, with or without interpolation. Which of the 112
    functions belong to the update is not established.
  - Hundreds of fps reported on other machines (Vulkan on Linux) are not explained by anything established here.
    Which counter measured them, and which client it was, is not known.

### P5. Startup and load time

speedfix's `cpu_tier` (the SSE math tier gate) and `texture_compression` (fewer textures CPU-encoded at load)
are startup-time fixes. `cpu_tier`'s site 0x1006CDDC is inside a manifest function (flag 2, a speedfix site);
where `texture_compression`'s registry value is read has not been located here. They stay in speedfix for now.

### Done means

The busy-town capture reaches the frame time the owner sets as the target. The alternating trace with the patch
shows only its intended differences. And the patch has run for a full session without a quarantine or a crash.

## 4. Goal 2: the D3D11 renderer

The worked-out scheme, from what is measured now (the emulated x86 setup, the plaza profile, the scope of D3D8 the
game uses, three designs and the measurement that chooses between them): docs/D3D11.md.

**What it has to replace:**
1. **Every device user.**
   - 1,633 call sites in the export use the device.
   - Every function that uses the device is in the export (data/gfx_device_users.json: 0 outside it). 90 of
     the 867 are not in the verified tree (docs/RESIDUE.md); until they are, they run as the game's code.
2. **Resources.** The game creates textures, vertex and index buffers, state blocks and vertex shaders
   (7 CreateVertexShader sites: 5 programmable shaders and 1 pixel shader from one function, 4 fixed-function, 2
   unreachable; docs/KIT.md, the static shader catalogue) on the Direct3D 8 device and keeps them.
3. **The fixed-function pipeline.**
   - 56 render states and 15 texture-stage state types are set somewhere.
   - FVF codes seen: XYZRHW|DIFFUSE|TEX1, XYZ|DIFFUSE|TEX1, XYZ|NORMAL|DIFFUSE|TEX1, XYZB1 and others.
   - Two-pass stencil shadow volumes (sqsnShadowVolumeSetPass).
   - All of these become shaders and state objects (RENDERER.md section 1).
4. **The swap chain and presentation,** and the device reset path (Gfx_CreateDevice is hookable).

**The approach this plan recommends (not decided):**
- **First, our own IDirect3DDevice8 implemented on D3D11.** It works with the code that is not reconstructed,
  which a CDx-level backend cannot. Write it for what the profiles show the game uses, not the whole interface.
- **Then move hot paths to native D3D11 through SDK hooks.** For example, the zone mesh in large buffers, and
  occlusion as asynchronous queries.

Unlike dgVoodoo, it can be told what a draw is for (the CDx and sqgx wrapper layer).

**Needed:**
- the next export, gated;
- a picture-comparison tool (the same frame through D3D8 and through the new device, compared pixel by
  pixel). It does not exist yet; the alternating trace compares calls, not pixels;
- P0's measurements, to know what the translation layer costs today.

### Stretch: modernise the renderer's methods (deferred)

The owner asked for Square's rendering code and methods to be improved losslessly with current practice (Luna, "3D
Game Programming with DirectX 11", e.g. constant buffers by update frequency, immutable state objects, instancing,
fewer state changes). Deferred until the port draws as the reference on the owner's driver (HISTORY, 14 Oct t8run):
"lossless" needs a verified baseline to compare against. Each change, when taken, is gated like the rest: same
pictures as the reference in t8 and in frame replay.

## 5. Goal 3: the UI overhaul

**Known (docs/UI.md):**
- The TkMenu layer's 11 functions are found and match the owner's declarations.
- The menu manager is the static object at 0x10621838.
- Three layout fields agree with the code.

**Not known:**
- How menus reach the draw functions. Walking direct callers up from Cell_/Font_ does not reach the menu
  functions; the path may go through vtables, which that walk does not follow.

**Possible today:**
- Hook the text and UI-quad draws (Font_DrawGlyphQuad, the Cell_ family, FontGlyphCache_*). The SDK allows
  restyling, scaling and batching them.
- Draw new UI in the frame callback.

**Needed:**
1. **The menu-to-draw path:** resolve the TkMenu primitives' virtual calls, then export and gate those
   functions. Without it, a change to a quad cannot tell which menu it belongs to.
2. **Layout and resolution:** where UI coordinates and scale come from (the 2D state and viewport functions:
   Gfx_SetDefault2DStates, Gfx_ApplyViewport1904, ...) and how the owner's CTk* layouts map onto the game's.
3. **Assets:** where fonts and UI textures are loaded and decoded. That is not in the export (the CYyTex
   functions in it are helpers).
4. **Input:** fnOnKeyDown and the menu manager, for new behaviour.

## 6. Prerequisites, in order

| # | Item | Needed for | State |
|---|---|---|---|
| 1 | P0 measurement tools (Present wait, per-call timing, sampling profiler) | Goal 1; goal 2's baseline | Built (Wine-tested); the in-game measurements are the owner's |
| 2 | Raise gate coverage of the hot flag-4 functions (SkinAndDrawV2, Cell_DrawQuadBlended, fn_1017C9C0, fn_100467E0, Actor_DrawWorldText, ...) | Goal 1 P2 | Fuzz aimed at their unrun branches, and new dumps from busy scenes. The four dumps are town, town2, combat and zonetoinn |
| 3 | The 90 exported functions outside the verified tree (8 UNRESOLVED calls, 13 FAIL, 11 constructs, 9 compile), and the world draw path outside the export (docs/GAPS.md) | Goal 1 P3; goal 2; goal 3's asset and menu paths | Open |
| 4 | Task #64: the register-preserving-callee blind spot in ecx_use/edx_use | The types mods and the gate rely on | Closed (7a925bd; checked 6 Oct: regenerated data identical) |
| 5 | The SDK's first in-game run (the example mod) | Every mod | Owner, in game |
| 6 | Picture comparison | Goal 2; checking visual changes | Built (`trace_pictures=K`, pictures.py); not run in the game yet |
| 7 | Task #65: whole-frame differential (blocked on calls into real D3D8 objects) | Confidence in the swapped set as a whole | Blocked; the alternating trace covers part of it in game |
| 8 | `Post_DrawRotatingGhostLayers` (gate FAIL, -0.0 vs +0.0) and `Gfx_DestroyD3DHolder` (crash when swapped) | Completeness | Open, causes unknown |

docs/GAPS.md measures what stands between us and changing any part of the pipeline on knowledge rather than
trial, and orders the tools that close it.

**The next step:** build P0 and take one baseline capture in a busy scene with speedfix off. Then P1, then the
first P2 mod for text.

## Measured in Bedau (the busy plaza of Port Jeuno), 6 Oct: where a frame goes and what draws it

**Render-thread time** (the owner's M5 MacBook Pro, Windows on ARM in Parallels, dgVoodoo to D3D11; about 71 ms a
frame; HISTORY.md, runs 3736 and 6156). Characters are about 26 ms of the frame; text and UI quads about 7 ms.

| Work | Share | Where |
|---|---|---|
| Occlusion readback stall | 19.5% | LockRect at 0x1006C91C |
| Character draws | 16% | every one of the 1,586 DrawIndexedPrimitive a frame comes from CYyModelDt::SkinAndDrawV2, one triangle strip of about 58 triangles each |
| x87 skinning | 13.3% | |
| Skinning buffer locks | 7.6% | |
| Text and UI quads | 10.3% | DrawPrimitiveUP, 885 a frame of 2 triangles each: glyphs 386, Cell quads 209, name plates 200 |
| Effects and zone | 3.2% | DrawPrimitive: CMoVuElem_Draw 270 a frame, the zone mesh 45 |

**Draw reduction, characters first.** The worked-out plan, with the facts it rests on: docs/SKINMERGE.md.
1. Measure how many of SkinAndDrawV2's strips run under the same state with nothing between them
   (merge_potential.py on a Bedau trace).
2. Gate the reconstruction of SkinAndDrawV2 on calls captured in Bedau. Its fuzzed gate trials covered 62 of 1,596
   instructions. With the facts of session 4, one capture PASSes 8 of 8 trials and covers 932. The other two lacked
   the model's command list (capture budget); new captures with `capture_follow` are needed for the rest.
3. A fork that draws a run of strips as one indexed list (an index list built once per model), gated against the
   game with the draw change declared.

That is the first step of the renderer port (goal 2) done in place. The D3D11 backend then draws the same merged
lists, and can skin on the GPU.

**The Direct3D 8 path on the owner's machine.** The game crashes there without dgVoodoo (owner, 6 Oct), so every
call goes through dgVoodoo's D3D8-to-D3D11 translation, as emulated x86 code. The only comparison left in place is
between dgVoodoo's own settings (compare_paths.py). Leaving the translation behind takes a renderer that issues
D3D11 itself: the D3D11 backend of goal 2. That backend is also the way to fewer and cheaper calls.

**Concluded (6 Oct, owner's sessions 3 and 4):** Windows' own d3d8.dll crashes the game on this machine; dgVoodoo
2.8.2 with D3D12 output stretches and flickers textures; dgVoodoo with D3D11 output is the one working path (D3D11 on
Parallels' prl_umdd10.dll, all x86 code emulated). No further path comparison is open; compare_paths.py stays for
comparing dgVoodoo settings if one is ever changed.
