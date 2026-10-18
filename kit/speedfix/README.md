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
| `frame_cap_fps` | 0 | Even frame pacing; at most 60 unless `uncap=1` (then 1000); with `frame_step=0`, 60/30/20/15 keep the game's time exact (below) |
| `occlusion_stagger` | 8 | Each actor's visibility re-tested every 4 (the game's), 8 or 16 frames; live both ways |
| `skip_actor_occlusion` | 0 | A/B: every actor counts as visible without its occlusion query |
| `skip_all_occlusion_readbacks` | 0 | A/B: every occlusion query counts as visible without the GPU-to-CPU readback |
| `texture_compression`, `texture_compression_value` | 1, 15 | The DXT threshold for textures loaded from now on |
| `cpu_tier` | 1 | The SSE math tier on any CPU with SSE (only before the game has chosen its tier) |
| `lazy_state` | 0 | Lever: hold SetRenderState / SetTextureStageState / SetTexture until a method that reads them; a change undone before then never reaches the device (below) |
| `block_cache` | 0 | Lever: applying a state block the game recorded updates the cache with what the block sets, instead of forgetting the whole cache (below) |
| `vs_dedupe` | 0 | Lever: a vertex shader of a declaration already made (no shader code) is made once; its handle is given out again (below) |
| `sse_skinning` | 0 | Lever: the game's SSE skinning loops on any CPU that reports SSE (below) |
| `merge_scenes` | 0 | Lever: one BeginScene/EndScene pair a frame (below) |
| `skin_merge` | 0 | Lever: a model's runs of consecutive triangle-list records drawn as one DrawIndexedPrimitive; forks SkinAndDrawV2 when on at the start (below, docs/SKINMERGE.md) |
| `precise_pacing` | 0 | Lever: the game's own frame limiter timed with the performance counter and waited on precisely: 60.0 frames a second instead of 58.8; live both ways (below) |
| `uncap` | 0 | The game's minimum ticks a frame (`[g_pCamera+0x30]`) set to 0 each frame: no 60 fps wait; the old value restored when off; live both ways (below) |
| `frame_step` | 0 | 1 = the game's step answered in whole ticks with the fraction carried, so its time follows real time at any frame rate; 2 = that, and the untruncated step at 14 call sites that only compare or accumulate it (below) |
| `occlusion_late` | 0 | Lever: 1 = each occlusion probe answered with the object's previous probe (no wait for this frame's picture); 2 = measure only, the game's answers (below) |
| `null_draws` | 0 | A test, not a speedup: the draw calls return D3D_OK without reaching the device; nothing is drawn; live both ways (below) |
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
- **block_cache: frame time in effect-heavy scenes.** Each queued effect element applies two or three state blocks
  (YmElem_SetupDevice and YmElem_RestoreDevice; 544 inside the effect drain of the plaza trace's frame 2, which is
  7,580 of the frame's 41,093 calls). The cache forgets everything at each block, since a block may set anything,
  so the default cache skipped 46 of those 7,580 calls (docs/HISTORY.md, 11 Oct).
  - Where the rule comes from: Microsoft's reference for BeginStateBlock says the setters called between
    BeginStateBlock and EndStateBlock are recorded in the block, and that a state set twice keeps its last value. The
    game records each block once, from a fixed table (StateBlock_ApplyTSS 0x1006AF90, 0x1006AF00).
  - What speedfix does: it follows every block recorded on the render thread, whether the lever is on or off. That
    is its render and stage states with their values, the texture stages it binds, and whether it records anything
    else this layer sees (the shader; transforms, viewport, lights, streams and indices when they are hooked). With
    the lever on, applying a followed block sets its states in the shadow, makes its texture stages and those other
    kinds unknown, and keeps everything else. Any other block forgets everything, as before.
  - A block stops being followed at DeleteStateBlock, CaptureStateBlock, CreateStateBlock returning its token, or
    Reset. After a Reset the game's own blocks are therefore forgotten at each apply, as without the lever (the game
    does not record them again).
  - Live both ways.
  - Simulated on the plaza trace: 24,347 to 22,391 calls a frame with the extended cache, 20,645 to 18,423 with
    lazy_state. Only the drain's two blocks were modelled there.
  - Not known:
    - whether the game's d3d8.dll (dgVoodoo on the owner's machine) behaves as the reference says. probe8 checks it
      (`stateblock.apply` in its CSV; it passes under Wine);
    - what applying a block costs against the calls it saves. Its A/B measures that.
- **vs_dedupe: loading and stutter.** The sessions made about 1,450 vertex shaders each (CreateVertexShader at about
  269 us each, on the render thread), and d3dprof's shader catalogue found 3 distinct ones: declarations with no
  shader code. A vertex shader with no code is the fixed-function pipeline fed by its declaration (DirectX 8 SDK,
  CreateVertexShader), so one made from the same declaration and usage is the same shader. speedfix makes it once and
  gives its handle out again; DeleteVertexShader deletes it when the last holder does. The declaration is walked by
  its tokens (a constant's data may hold 0xFFFFFFFF). Live both ways. Not known: whether the game compares two
  handles anywhere (no such use has been seen), and whether Reset destroys vertex shaders on the owner's driver (the
  table is kept across Reset). Measure it by the creations it saves (`xikit.log`: `vs_dedupe:` each minute) and by
  the spike log (`spike_ms=50` in xikit.ini), not by the frame-time A/B: it acts while loading.
- **precise_pacing: 60.0 frames a second instead of 58.8.** The game paces itself (sf_main.cpp's head lists each
  instruction). Its limiter (in 0x10011A00) polls its timer's rate (0x10019550: 1000 / the ms since the last frame,
  from timeGetTime, whole ms) and sleeps `Sleep(1)` until `60 / (scale * rate)` reaches `[limiter+0x30]` ticks of
  1/60 s. With whole ms and one tick, 16 ms gives 0.96 and 17 ms gives 1.02, so each frame lasts at least 17 ms:
  58.8 frames a second. The lever answers the limiter's three calls of the rate from the performance counter and
  replaces its Sleep with a wait to the exact moment (a high-resolution timer, then a spin of about 1 ms). The game's
  time stays exact: each frame's ms are measured from one frame's last rate call to the next, so they add up to the
  counter's time (less the paused ms the timer keeps, which it still subtracts). Other callers of the rate get the
  game's function. Off, or in a frame not through the hooks, the limiter is the game's (the counter is still read).
  - Not established: the tick count `[limiter+0x30]` and the timer's scale (`+8`) in the owner's game (the 58.8
    assumes 1 and 1.0, which the code's constants do not fix; each statistics line gives both as the last wait read them); and on which thread the limiter runs (the log names
    both threads once). The limiter's position relative to Present is not known either, so the lever counts a frame
    as through the hooks when the frame now or the one just presented went through them.
  - What it does not do: go above 60. That is `uncap` and `frame_step` (below).
  - Measure: `xikit.log` each minute, `precise_pacing: N frames timed by the counter; the limiter waited X ms`; and
    the frame A/B (`ab_levers=precise_pacing`), whose mean frame should move from about 17.0 to 16.67 ms. Do not use
    it with `frame_cap_fps` (two pacers).
- **uncap and frame_step: above 60, with the game's time following real time** (docs/RENDERER.md 3a).
  - `uncap=1` writes 0 to the frame's minimum tick count `[[0x104568FC]+0x30]` each frame (2 is the game's 30 fps,
    1 its 60). The game's limiter then does not wait. The value found is logged, and written back when `uncap` goes
    to 0. With `frame_step=0` this is what Windower's `FrameRateDivisor 0` and Ashita's fps addon are documented to
    do, and the game then advances one tick every frame: at 300 fps in sf_host its step added up to 500% of real time.
  - `frame_step=1`: the step's truncation (the `_ftol` call at 0x10012C3C) goes to speedfix, which keeps the
    fraction the game drops and carries it. The getters 0x10014CF0, 0x10014D20 and Frame_Render's inline copy
    (0x10011A24) answer whole ticks, `trunc(carry + average)`, with the game's floor `[+0x30]` and cap 20, so 0 on
    most frames above 60. In sf_host at about 300 fps the sum was 100.0-100.7% of the counter's time; at 60 fps,
    123 ticks against 123.3.
  - `frame_step=2`: that, and the untruncated average (the game's own, before `_ftol`) at the 14 call sites in 13
    functions where `pipeline/frame_step_users.py` follows every use of the value to a comparison, a drop or an
    accumulator (`sf_step_sites.h` lists them). The other 204 get whole ticks: one value per function per frame.
  - Patched once, at the first frame with `frame_step` on; after that the switch is live both ways (0 answers as
    the game's). A frame not through the kit's hooks answers as the game's.
  - Below 60 fps, 1 and 2 also end the under-count of the truncated step (RENDERER 3a's table: 62-99% of real
    time), so what the step drives moves faster than it does now: at real time.
  - Not known: code that moves a fixed amount per frame without the step (0x1024DE20 is one; how many there are is
    not known) runs fast above 60 whatever this does; a step of 0 is new to the 204 whole sites; what the server
    makes of it. Most motion is still whole ticks, so above 60 it is not smoother, only not fast.
  - Measure: `xikit.log` each `stats_every`, `frame_step=N uncap=N: F frames, T ms by the counter (X ticks of
    1/60 s); speedfix's step ... whole W (P%; Z frames of 0), fraction R (P%); the game's own G (P%)`. P near 100%
    is the step following real time. And in the game, a clock or a walk timed against real time.
  - Test: `test/test_speedfix.py` mode `step`: the game's limiter and step code run in sf_host, the getters and
    the inline copy called directly and through the fraction site 0x10227E58.
- **null_draws: a test of what the frame costs without drawing.** DrawPrimitive, DrawIndexedPrimitive,
  DrawPrimitiveUP and DrawIndexedPrimitiveUP return D3D_OK without reaching the device (signatures: d3d8.h). Every
  other call still goes to the device: state changes (held ones go out at the draw as they would), locks, Clear,
  Present. The game's own work is untouched. What remains is the frame time of the game's CPU work plus the device's
  cost of everything but drawing. Nothing is drawn, so the screen is not usable while it is on, and the occlusion
  readback reads an undrawn target: use it with `skip_all_occlusion_readbacks=1` in both arms
  (`kit/sessions/ceiling`). Live both ways; `xikit.log` each minute: `null_draws: N draw calls not sent`.
- **occlusion_late: the readback's wait.** The game's readback 0x1006C8B0 copies the box picture (level 0 of
  `[[0x1047C004]+0xC]`) into the query's lockable surface `[q+4]` with CopyRects and locks it straight after
  (0x1006C8FD, 0x1006C91C). That lock waits for the GPU to draw and copy the picture.
  - 1: each query object gets a 16x16 lockable render target of its own (the format of `[q+4]`). A probe copies the
    picture into it and answers with the count of the picture copied at the object's previous probe, counted as the
    game counts (`(dword & 0x80) >> 5` over 256 pixels). A first probe, or one whose previous picture is older than
    32 frames, gets the game's readback; 32 is a chosen bound, not a fact of the game. Reset releases the surfaces.
  - 2: measure only. The game answers; the log counts how often the late answer would have differed, and how often
    visible/hidden would have flipped.
  - The statistics line times the game's readback, the copy and the late read (mean, and the longest late read).
  - The price: an actor appears or disappears one probe late (up to `occlusion_stagger` frames).
  - Not known: whether a lock a probe later waits on dgVoodoo. The longest late read answers that. occlusion_late is
    the reverted occlusion_async's mode 2 (10 Oct) brought back, with its mode 3 as mode 2. The owner's run 13172
    measured only its mode 1 (system-memory image surfaces): +4.18 ms with phases differing by 15.14 ms, so no
    difference either way. Whether a lockable render target behaves differently on dgVoodoo is not known. It cannot
    beat `skip_all_occlusion_readbacks`, which does no copy and no read. In ours (d3d8on11) such a lock is answered from the readback snapshot without a
    wait.
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
- **skin_merge: character draws.** In the plaza trace, 1,577 of a frame's 1,620 DrawIndexedPrimitive come from
  SkinAndDrawV2, one per triangle-list record (two when the model draws twice). Consecutive list records draw
  consecutive ranges of one index buffer, so a run of them is one range.
  - What it does: SkinAndDrawV2 is replaced by speedfix's fork (fork/CYyModelDt__SkinAndDrawV2.fork.cpp). It holds a
    list record and draws the run in one call, the way the game draws one record, when the next record ends the run.
    It holds only when none of the record's envmap, projected-texture or offscreen passes can draw. The trace bound is
    1,330 draws a frame instead of 1,577; records invisible in a trace can lower the saving further.
  - The fork is installed only when skin_merge is 1 at the start; after that it switches live both ways. With it off,
    the fork is the game's function (the gate, no declarations).
  - Checked: test_skinmerge.py (the gate both ways, and the owner's captures with the merge on). Not known: the time it
    saves, and the picture: an A/B (`ab_levers=skin_merge`) and a look at characters, with it on and off.
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
`skip_actor_occlusion`, `skip_all_occlusion_readbacks`, `frame_cap_fps`, `sse_skinning`, `merge_scenes`, `skin_merge`, `precise_pacing`, `null_draws`, `occlusion_late`, `uncap`, `frame_step`. Several can be
listed together (they go off and on together). Groups separated by `;` run one after another, each its own A/B with
its own summary, 5 s apart: `ab_levers=sse_skinning; merge_scenes; batch_up_draws, lazy_state`. Empty `ab_levers`
and set it again for another run.

## Tried and reverted

- **skip_shadow_pass** did not send the render-target switches, Clear and draws of Model_BeginOffscreenPass's
  shadow pass. In run 7388 its own counter showed the game binding that texture for drawing (259 and 2,116 times a
  minute): the texture is read, and the pass's output is needed. Reverted.
- **up_ring** sent DrawPrimitiveUP as DrawPrimitive from a ring buffer, leaving stream 0 bound to the ring. In run
  7388 it was slower (A 20.3 ms, B 25.6 ms) and the owner saw corrupted frames. Why the frames broke is not known.
  One explanation was checked and is wrong: Gfx_SetStreamSourceCached (0x1002F130) does not skip a stream it has
  recorded. It records the buffer and stride, then Gfx_ApplyCachedStreamSource calls SetStreamSource for any non-NULL
  buffer (and does nothing for a NULL one; IDA listing). Reverted.
  - **A bug, reproduced afterwards** (the reverted code in the Wine host, with batching, lazy_state and up_ring on):
    the ring issued its SetStreamSource and DrawPrimitive through the device's front, which runs speedfix's own hooks
    again. The DrawPrimitive hook first sent the held state changes, so a batched UI quad reached the device after the
    next texture change: `SetTexture(A) ... SetStreamSource(ring), SetTexture(B), DrawPrimitive`. The quad was drawn
    with the wrong texture. Whether this is all of what the owner saw is not established.
  - **What the measurements allow:**
    - Through dgVoodoo, a DrawPrimitiveUP costs 157.5 ns and a vertex-buffer Lock with DISCARD 2,849 ns (probe8, run 2).
    - A ring needs a lock for every batch it sends, so it cannot beat DrawPrimitiveUP unless dgVoodoo's NOOVERWRITE
      lock is far cheaper than its DISCARD one. That is not measured yet: probe8 now measures it, and a whole ring
      draw.
    - A ring returns only if that measurement favours it, and with its calls made below speedfix's hooks
      (`g_prev`).

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
- **frame_cap_fps above 60 needs `uncap=1`** (otherwise clamped to 60; with it, up to 1000). The game advances its
  time by `max(step, 1)` ticks of 1/60 s per frame (`Camera_GetFrameScale`, docs/RENDERER.md 3a), so with
  `frame_step=0` above 60 what it drives runs faster than real time (logged as a warning). With `frame_step=0` below
  60 the step is truncated to whole ticks: 60, 30, 20 and 15 keep game time equal to real time, and another cap is
  used but logged.
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
  - occlusion_late: the game's own readback run on fake 16x16 surfaces (0, 256 and 128 for an empty, full and
    half-drawn picture); with it on, the first probe is the game's, later ones the previous picture's count, per
    object, one render target each, a picture 40 frames old not used, Reset releasing the surfaces; mode 2 answers as
    the game and logs the comparison and the times;
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
  - skin_merge: on at the start, SkinAndDrawV2's entry jumps into speedfix.dll; off, it is the game's code;
  - precise_pacing (a fifth run, `pacing`): the game's own limiter code (0x10012B05 to the lap at 0x10012BF5) runs on
    a limiter object of the test's, with the imports it reaches filled in. On: both changes made, 240 frames at a
    median of 16.674 ms (Wine, 17 Oct), the game's ticks equal to the counter's time within 0.1 tick, no frame timed
    in whole ms; another caller of the rate gets the game's 1000 / whole ms; frames without the hooks get the game's
    whole-ms timing; off again, every frame whole ms of 17 or more, the ticks within one of the time.
- **The gate:** the query hook.

`test/test_skinmerge.py` (FFXI_DLL; CAPTURES, a folder of the owner's SkinAndDrawV2 captures): the skin_merge fork,
merge off against the game's function with no declarations, merge on with fork/skinmerge.declare.json, and merge on
replayed from the captures (docs/SKINMERGE.md, Checks).

Not tested: the real game. That covers the real device and driver, dgVoodoo, which thread calls what, how much any
of this saves, and the A/B picture. The first run: `profile=1 call_time=1` with the defaults, then the alternating trace.
