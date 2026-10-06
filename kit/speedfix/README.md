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
- **Wine host** (`sf_host.c`, the real FFXiMain.dll, the kit, a recording fake device):
  - the state, shader and scene rules, and the other-thread rule;
  - batching: held, sent in order before a state change, a strip as a list with its winding, no conversion under flat
    shading;
  - the stagger's arithmetic through the re-aimed call, and the game's own test in a frame without the hooks;
  - the readback skip;
  - the `cpu_tier` and `texture_compression` writes;
  - the kit's `respect_speedfix`.
- **The gate:** the query hook.

Not tested: the real game. That covers the real device and driver, dgVoodoo, which thread calls what, how much any
of this saves, and the A/B picture. The first run: `profile=1 call_time=1` with the defaults, then the alternating trace.
