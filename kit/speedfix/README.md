# speedfix (a mod of the in-game kit)

speedfix_hook rewritten as a mod of xikit, from the owner's `speedfix_hook.cpp` (GPL-3.0-only; these files keep that
licence). It does the same work through the kit's device hooks and function hooks. Each address it relies on was
checked against the analysed build, and each change to the game's code checks the bytes it replaces.

## Install

1. Build: `kit/speedfix/build.sh` (MinGW) writes `kit/speedfix/dist/speedfix.dll`.
2. Put `speedfix.dll` next to `xikit.dll` and add it to `xikit.ini`: `mods=speedfix.dll`.
3. Do not load `speedfix_hook.dll` as well. The mod checks for it and does nothing if it is there.
4. The first start writes `speedfix.ini` next to the mod, with the defaults. It is re-read within 60 frames of being
   saved, and what changed goes to `xikit.log` (lines starting `speedfix:`), along with a summary every minute.

## Settings (`speedfix.ini`, `[speedfix]`)

| Key | Default | What |
|---|---|---|
| `state_cache` | 1 | Skip SetRenderState / SetTextureStageState / SetTexture that repeat the value the device holds |
| `vertex_shader_cache` | 1 | Skip SetVertexShader that repeats the bound handle |
| `extended_state_cache` | 0 | Also SetTransform / SetViewport / SetLight / LightEnable / SetStreamSource / SetIndices |
| `state_cache_reset_each_scene` | 1 | Forget the cache at each BeginScene / EndScene |
| `batch_up_draws` | 0 | Merge back-to-back DrawPrimitiveUP calls of one stride into one list |
| `batch_up_max_prims`, `batch_up_max_bytes` | 0 | Caps on one merged draw (0: the device's MaxPrimitiveCount, the 1 MB buffer) |
| `frame_cap_fps` | 0 | Even frame pacing; at most 60, and 60/30/20/15 keep the game's time exact (below) |
| `occlusion_stagger` | 8 | Each actor's visibility re-tested every 4 (the game's), 8 or 16 frames; live both ways |
| `skip_actor_occlusion` | 0 | A/B: every actor counts as visible without its occlusion query |
| `skip_all_occlusion_readbacks` | 0 | A/B: every occlusion query counts as visible without the GPU-to-CPU readback |
| `texture_compression`, `texture_compression_value` | 1, 15 | The DXT threshold for textures loaded from now on |
| `cpu_tier` | 1 | The SSE math tier on any CPU with SSE (only before the game has chosen its tier) |
| `lazy_state` | 0 | Lever: hold SetRenderState / SetTextureStageState / SetTexture until a method that reads them; a change undone before then never reaches the device (below) |
| `vs_dedupe` | 0 | Lever: a vertex shader of a declaration already made (no shader code) is made once; its handle is given out again (below) |
| `sse_skinning` | 0 | Lever: the game's SSE skinning loops on any CPU that reports SSE (below) |
| `merge_scenes` | 0 | Lever: one BeginScene/EndScene pair a frame (below) |
| `skip_shadow_pass` | 0 | Lever: one actor's shadow-texture pass (Model_BeginOffscreenPass) not drawn; its state calls still sent (below) |
| `motion_stats` | 0 | At the start: each statistics line also splits frame time and per-frame work by camera still / moving (below) |
| `stats_every` | 60 | Seconds between the statistics lines in xikit.log |
| `ab_levers`, `ab_after`, `ab_phase`, `ab_rounds`, `ab_settle` | empty, 60, 30, 4, 60 | The lever A/B, no keys: the listed settings off (A) against as set (B) (below) |

## The levers (measured, not yet decided)

What the owner's two sessions (the profiles, resource logs and trace of runs 10016 and 6080; docs/HISTORY.md) show
and what each lever does about it. Each is off by default; the lever A/B measures it in the game.

- **lazy_state: frame time.** `Cell_DrawQuadBlended` (the UI and text quads) sets ALPHABLENDENABLE 1 before each quad
  and 0 after it (0x1011F962): 540 calls a frame of the 0 alone, 3.6 ms a frame in the baseline (6.7 us each), and
  the state cache cannot skip them because each one changes the value. Direct3D 8 reads that state when it draws, so
  speedfix holds a change until something reads it: a draw, Clear, a Get, a state block, BeginScene, EndScene, Present
  (every method except those that set or get other state or make resources). A change undone before then (the 0
  after one quad and the 1 before the next) is never sent. With batching on as well, quads with the same texture
  then merge. A held texture is AddRef'd until it is sent or undone. A held call returns D3D_OK to the game; a
  failure when it is sent is not reported back (it leaves that state unknown to the cache). It needs every
  state-reading method hooked (about 60 more than the defaults), so it takes effect at the next start when it was
  off at this one; with batching or the extended cache on, every method is hooked anyway and it switches live.
  Not known: how much the extra hooks cost on each call against what the skipped calls save. That is what its A/B
  measures.
- **vs_dedupe: loading and stutter.** The sessions made about 1,450 vertex shaders each (CreateVertexShader at about
  269 us each, on the render thread), and d3dprof's shader catalogue found 3 distinct ones: declarations with no
  shader code. A vertex shader with no code is the fixed-function pipeline fed by its declaration (DirectX 8 SDK,
  CreateVertexShader), so one made from the same declaration and usage is the same shader. speedfix makes it once and
  gives its handle out again; DeleteVertexShader deletes it when the last holder does. The declaration is walked by
  its tokens (a constant's data may hold 0xFFFFFFFF). Live both ways. Not known: whether the game compares two
  handles anywhere (no such use has been seen), and whether Reset destroys vertex shaders on the owner's driver (the
  table is kept across Reset). Measure it by the creations it saves (`xikit.log`: `vs_dedupe:` each minute) and by
  the spike log (`spike_ms=50` in xikit.ini), not by the frame-time A/B: it acts while loading.
- **sse_skinning: frame time on CPUs the game does not call Intel.** On the owner's M5 MacBook Pro (Windows on ARM in
  Parallels, the game x86-emulated) the render thread spent 13.3% of its samples in the x87 skinning loop of
  0x1002F1D0, and none in its SSE loop (docs/HISTORY.md, run 6156).
  - How the game chooses: the loop is picked on every call by the SSE flag of the game's CPU object, `[0x104566E8]+9`,
    read through 0x1000D680 (or `+0xC`, which nothing writes). Its detection (0x1000D400) sets that flag from
    CPUID.1:EDX bit 25 only when the vendor is "GenuineTMx86" or contains "Intel" from its first "I"; an
    "AuthenticAMD" CPU gets the 3DNow flag instead.
  - What the lever does: it writes the CPU's own SSE bit there while it is on and the kit's hooks are live, and the
    game's value back otherwise, on the render thread.
  - Who reads the flag: only 0x1002F1D0 and 0x100311A0, the latter reached through a pointer, reading the flag
    afresh on each call.
  - Why switching mid-session is safe for the skinning: both loops read the same per-call inputs, the model fields
    and the bone matrices 0x10457550 that SkinAndDrawV2 sets on every call, and write the vertex buffers it locks
    every frame.
  - Not established:
    - whether 0x100311A0 leaves anything flag-dependent behind after a call;
    - how far the SSE loop's single-precision results differ from the x87 loop's (it is the game's own code for
      Intel CPUs);
    - what the emulator's CPUID vendor is.
- **merge_scenes: frame time.** The profiles show several BeginScene/EndScene pairs a frame. The owner's Bedau run
  made 7.7 a frame, its EndScene calls taking 4.6 ms a frame in all, about 0.6 ms each. Under dgVoodoo that may be
  where queued work is submitted.
  - What it does: the game's EndScene inside a frame is held, and the BeginScene after it is dropped, so the frame
    is one scene. The held EndScene goes out before Present, Reset, CopyRects, UpdateTexture and GetFrontBuffer;
    those three always pass through a flushing stub, so the minimal start has 3 stubs.
  - Not known: whether the cost moves to Present instead of going away (the A/B answers that), and whether any
    driver treats a render-target switch inside one long scene differently.
- **motion_stats: where stutter comes from** (speedfix_hook's still/moving split).
  - The camera counts as moving in a frame when that frame's first view transform differs from the previous
    frame's.
  - Per label, each statistics line gives:
    - frame time: mean, p50, p95;
    - per frame: draws (UP draws among them), CopyRects (each occlusion readback copies with one), textures bound
      for the first time, and resource creations.
  - It hooks the draws, SetTransform and the Create methods from the start. It is a measurement, not a lever.
- **Already there, now measurable one at a time:** `batch_up_draws` (1,031 DrawPrimitiveUP a frame through the CDx
  wrapper 0x1000CDE2), `extended_state_cache` (the zone mesh `sub_1017C9C0` sets the same light 3,925 times a frame in
  the town scene of run 6080), `occlusion_stagger`, `skip_actor_occlusion` and `skip_all_occlusion_readbacks` (readbacks
  were rare in those scenes: 0.35 a frame).
- **skip_shadow_pass: render-target switches and draws.** In the town trace of run 6080 (17 frames), each frame
  had 18 pairs of SetRenderTarget to one 256-wide target and back, with 28 draws into it. All come from
  `Model_BeginOffscreenPass` (0x10033800) and `Model_EndOffscreenPassBindTextures` (0x10033A70). When
  `g_actorShadowMode` is 1 and `g_shadowCasterDistance` is under 4 (one actor at a time, chosen in
  CXiSkeletonActor's per-actor update), CYyModelDt draws each mesh of that actor a second time, into that target, from
  the light's view (`g_lightViewMatrix`), unlit, in one constant colour. Across the 17 frames, nothing bound that
  target's texture with SetTexture or copied from it; the only render-target texture ever bound was the scene's own.
  So these frames show the pass's output unused. The code does not say what reads it, so this is not known to hold
  everywhere. speedfix hooks the two functions (the game's code runs in both) and, between them, does not send
  SetRenderTarget, Clear or the draws. Every state call still goes out, so the state after the pass is the game's.
  All four callers call End only when Begin returned non-zero (tree/src/CYyModelDt.cpp). If the texture is read
  somewhere unseen, the visible sign would be that actor's shadow going stale or missing: look at your own
  character's shadow with it on. It hooks as lazy_state does (taking effect at the next start if both were off).
- **Not made into levers yet, and why:**
  - Pooling vertex and index buffers. The sessions made about 10,000 a session, mostly DYNAMIC | WRITEONLY vertex
    buffers in the default pool in a few sizes (6,000, 4,000, 8,000 and 2,000 bytes). Pooling them needs their
    Release, which is a method of the buffer, not of the device. Most were made from a module the old resource log
    could not name (offset 0x9D8E); `module_of` names it in the next capture.
  - The zone mesh at its source (task 16) and the scene and render-target count (task 18) need the tree's code first.

## The lever A/B (`ab_levers`)

xikit's `ab=1` compares the game with and without everything the kit adds. To measure one lever, list it in
`speedfix.ini`, keep xikit.ini's `ab=0`, and stand still:

```
[speedfix]
lazy_state=1
ab_levers=lazy_state
```

`ab_levers` takes setting names (above: `lazy_state`), never "A" or "B": speedfix switches between the two by
itself. `ab_after` seconds after `ab_levers` is saved non-empty, phases of `ab_phase` seconds run in the order A B B A,
`ab_rounds` times (4: 8 phases, 4 minutes). A is the listed settings off (`occlusion_stagger` 4, the game's); B is
the settings as set. Everything else speedfix does stays as set in both. The first `ab_settle` frames of a phase are
left out, and so are frames that did not go through speedfix's hooks (F9, a trace). Results go to `xikit.log`, in
the log folder: a line per phase (frames, mean, median, p95, max) and a summary (`speedfix: ab <n> (<levers>)`:
B - A, the medians and p95, and how much the phases of one label differ among themselves, which is the noise the
difference must beat). A name that is not a lever, a lever this start's hooks cannot do, or one set to its off value
is left out and logged. Lever names: `state_cache`, `vertex_shader_cache`, `extended_state_cache`,
`state_cache_reset_each_scene`, `batch_up_draws`, `lazy_state`, `vs_dedupe`, `occlusion_stagger`,
`skip_actor_occlusion`, `skip_all_occlusion_readbacks`, `frame_cap_fps`, `sse_skinning`, `merge_scenes`. Several can be
listed together (they go off and on together). Groups separated by `;` run one after another, each its own A/B with
its own summary, 5 s apart: `ab_levers=sse_skinning; merge_scenes; batch_up_draws, lazy_state`. Empty `ab_levers`
and set it again for another run.

## What changed from speedfix_hook

- **The kit's hooks, not the driver's vtable.** d3dprof sits behind speedfix, so `profile=1 call_time=1` counts and
  times what speedfix lets through. The kit takes mod hooks off for F9 and for a trace's unswapped frames. speedfix's
  changes to the game's code follow them in those frames, so the alternating trace (`trace=1`) compares the game with
  and without everything speedfix does, and nothing else.
- **The occlusion stagger** is one re-aimed call in the game's bucket test (0x100CC58A in
  `CXiSkeletonActor::PerActor_FourBucketStaggeredUpdate`). An actor is queried when frame mod N equals its bucket plus
  4k, where k is its share of that bucket (taken from its address), so each actor is still queried only on frames the
  game would query it. A forced update (`forceUpdateFlag`, the model flags, a setting) still queries at once, as in the
  game. Unlike speedfix_hook's relocated bucket counters, N can be lowered while the game runs.
- **skip_actor_occlusion** is a function hook (`XI_HOOK`) on `Occlusion_QueryScreenRect`. It answers 256 only to the
  two actor call sites, which it tells apart by their return addresses. With its switches off it is gated: PASS against
  the game's function, 38 of 38 instructions covered (`test/test_speedfix.py`).
- **frame_cap_fps above 60 is refused** (clamped to 60). The game advances its time by `max(step, 1)` ticks of
  1/60 s per frame (`Camera_GetFrameScale`, docs/RENDERER.md 3a), so above 60 it would run faster than real time.
  Below 60 the step is truncated to whole ticks: 60, 30, 20 and 15 keep game time equal to real time, and another cap
  is used but logged.
- **Batching:** strips and fans are converted to lists only while FILLMODE is SOLID and SHADEMODE is GOURAUD (read
  from the device when not known). Under point or wireframe fill the converted list repeats shared vertices and edges.
  Under flat shading, which vertex colours each triangle depends on D3D8's numbering, which is not settled here.
  Lists are merged whatever the state.
- **Only the hooks it needs.** With batching, the extended cache and lazy_state off at the start (the defaults),
  speedfix hooks 13 device methods: the cached setters, Reset, Present, the scene and state-block methods, and
  CreateVertexShader / DeleteVertexShader (vs_dedupe). CopyRects, UpdateTexture and GetFrontBuffer pass a flushing
  stub, for merge_scenes. Every other call goes
  straight to the layer below. Each hooked method costs a pass through the kit's front layer and speedfix on every
  call, and d3dprof, which sits below them, cannot see that cost. Turning batching or the extended cache on while
  the game runs then takes effect at the next start.
- **Calls from another thread** are never cached or batched. They send what is pending and make the whole shadow
  unknown. **MultiplyTransform** makes that transform's shadow unknown; speedfix_hook's extended cache kept trusting
  it.
- **Not carried over:**
  - `occlusion_async_readback`: the owner measured it slower than the stall it removes, on dgVoodoo/D3D11/Parallels;
  - `unsafe_async_batch_submit`: every device call waits for it anyway;
  - the device-call census: d3dprof's profile gives it per call site.
- **The kit:** with the mod loaded, `respect_speedfix` applies to it as to speedfix_hook. The two functions it changes
  inside (manifest flag 2) stay the game's code, so a reconstruction cannot bypass the change.

## Tested (offline) and not

`test/test_speedfix.py`:
- **Wine host** (`sf_host.c`, the real FFXiMain.dll, the kit, a recording fake device), in three runs: the defaults (13 methods hooked, batching left for the next start), batching on from the start, and lazy_state with vs_dedupe from the start:
  - the state, shader and scene rules, and the other-thread rule;
  - batching: held, sent in order before a state change, a strip as a list with its winding, no conversion under flat
    shading;
  - the stagger's arithmetic through the re-aimed call, and the game's own test in a frame without the hooks;
  - the readback skip;
  - the `cpu_tier` and `texture_compression` writes;
  - the kit's `respect_speedfix`;
  - lazy_state: a toggle between two draws never reaches the device, the last change goes out before EndScene,
    GetRenderState and Clear get the held change first, a held texture's reference is taken and given back, with
    batching two quads around an undone toggle become one draw, turned off while running nothing stays held;
  - vs_dedupe: one shader for a repeated declaration, two for declarations that differ after constant data of
    0xFFFFFFFF, a shader with code made as asked, deletion only by the last holder;
  - the lever A/B: phases A then B, the summary, an unknown name left out;
  - sse_skinning: off leaves the game's SSE flag (0), on sets it to 1 (the CPU under Wine reports SSE), off again
    puts the game's value back;
  - merge_scenes: three scenes in a frame reach the device as one Begin and one End before Present, a CopyRects
    between scenes ends the held scene first, off again passes every scene;
  - motion_stats: frames with a fixed view and frames with a changing view are counted apart;
  - skip_shadow_pass: the game's own Begin/End run against the fake device (shadow mode 1): no SetRenderTarget, Clear
    or draw reaches it, its state calls do; turned off while running, the pass switches targets and draws.
- **The gate:** the query hook.

Not tested: the real game. That covers the real device and driver, dgVoodoo, which thread calls what, how much any
of this saves, and the A/B picture. The first run: `profile=1 call_time=1` with the defaults, then the alternating trace.
