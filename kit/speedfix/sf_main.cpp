// SPDX-License-Identifier: GPL-3.0-only
//
// sf_main.cpp -- speedfix as a mod of the in-game kit (xikit; docs/SDK.md, kit/speedfix/README.md): its entry points,
// its settings, and the changes it makes to the game's own code. Rewritten from speedfix_hook.cpp (the owner's,
// GPL-3.0-only); every address and instruction below was checked against the analysed build (each comment names
// what was read), and every code change checks the live bytes before it writes.
//
//   occlusion_stagger    Each actor re-tests its visibility every N frames (4 is the game's, 8, 16; live both ways).
//                        The test is in CXiSkeletonActor::PerActor_FourBucketStaggeredUpdate (0x100CBDE0):
//                          0x100CC584  mov edi, [esi+0x9F0]      the actor's bucket (0..3)
//                          0x100CC58A  call GetFrameCounter      (0x10014D50: [[0x104568FC]+0x34])
//                          0x100CC58F  and eax, 0x80000003 ...   the frame counter mod 4
//                          0x100CC59B  cmp eax, edi / je 0x100CC5CC   (EAX is not read after this)
//                        The call is re-aimed at sf_bucket_stub, which answers the bucket itself exactly when
//                        frame mod N == bucket + 4 * k (k: the actor's share, from its address), and another value mod 4
//                        otherwise. A forced update (forceUpdateFlag, the model flags, a setting) skips this test in the
//                        game's code and still queries at once. speedfix_hook's stagger moved the game's bucket
//                        counters instead, which could not be lowered while the game ran.
//   skip_actor_occlusion Every actor counts as visible without its occlusion query: Occlusion_QueryScreenRect
//                        (0x1006C280, __thiscall, 9 stack arguments, ret 0x24) hooked with XI_HOOK; it answers 256 (the
//                        largest count the readback gives: 8 x 8 samples of 4) when it is called from one of the two
//                        actor paths (returning to 0x100CC68D in 0x100CBDE0, 0x100CD016 in 0x100CCE60), and runs the
//                        game's function otherwise. Both callers treat non-zero as visible.
//   skip_all_occlusion_readbacks
//                        Every occlusion query of all six users answers 256 without the GPU-to-CPU readback, which
//                        stalls the CPU (the owner's measurement: 4-5 ms for the first in a frame, about 1.2 ms after).
//                        The readback (0x1006C8B0, __thiscall (query, flag), ret 4) has one caller, 0x1006C4D7 in
//                        Occlusion_QueryScreenRectImpl (`push 0; ...; call 0x1006C8B0`); that call is re-aimed at
//                        sf_readback, which calls the game's function when the switch is off. The query quad is still
//                        drawn and the Z states still restored after it (Impl, 0x1006C300).
//   texture_compression  g_cfgTextureCompression (0x10456AD0, registry 0018): the DXT threshold for textures loaded
//                        from now on.
//   cpu_tier             The SSE math tier on any CPU with SSE: 0x1006CDDE `je 0x1006CDF9` (74 19) skips the SSE test
//                        unless the vendor string matched (0x1006CCE0); two NOPs remove the skip. It only matters
//                        before CreateProcessorInstance (0x1006CB50) chooses the tier and sets g_pProcessor (0x1047CF7C);
//                        that runs in a load step after the device exists (0x10010D20), so the mod's init, made right
//                        after the device, is usually in time. Written as one compare-exchange of the aligned dword.
//   frame_cap_fps        An even frame rate (0 = off), at most 60 unless uncap=1: the game advances its time by max(step, 1) ticks
//                        of 1/60 s per frame (Camera_GetFrameScale, 0x10014CF0; docs/RENDERER.md 3a), so above 60 it
//                        would run faster than real time. Below 60 the step is truncated to whole ticks, so 60, 30, 20
//                        and 15 keep game time and real time equal; another value is used but logged.
//   uncap                The game's frame waits while v < [g_pCamera+0x30] (0x10012B47; v the frame's time in 1/60 s ticks;
//                        docs/RENDERER.md 3): 1 holds 60 fps, the game's own setter 0x1008A0E0 writes 2 or 4. Under uncap
//                        speedfix's fork of Frame_Render (fork/Frame_Render.fork.cpp, from the verified tree) reads that
//                        floor as 0, in the wait and in the step; the game's field itself is not written.
//   frame_step           What the game's frame step answers. The game makes it in Frame_Render (0x10012C3C..0x10012C77):
//                        min(max(trunc(average of its last 4 v), [+0x30]), 20) into [+0x28], read by the getters
//                        0x10014CF0 and 0x10014D20 (`max([g_pCamera+0x28], 1.0)`, 218 calls) and Frame_Render's copy at
//                        0x10011A24. Truncated, it under-counts time below 60 fps (62-99% in the dumps); with the floor 0
//                        above 60 it is 0, answered as 1: a tick every frame. The forks (Frame_Render and both getters,
//                        each gated: the game's function with the switch off, only the declared differences with it on)
//                        take the same value without the truncation (floor and cap as the game's) and carry its fraction
//                        from frame to frame into whole ticks; [+0x28] stays the game's. 1: whole ticks at every call;
//                        over time they add up to the game's own measure of the frames' time, and above 60 fps some
//                        frames have 0 (a value the game never answered). 2: as 1, and the calls in sf_step_sites.h
//                        (written by pipeline/frame_step_users.py: every path of every call in the function ends in an
//                        accumulator `x += f(step)`) get the fraction. Code that moves something a fixed amount each frame
//                        without the step (0x1024DE20 moves by a fixed amount once the step test passes) still runs once a
//                        frame: above 60 fps it is faster whatever the step answers, and how much of it there is is not
//                        known. Both take effect at the start (the forks are installed then).
//   precise_pacing       The game's own frame limiter, timed with the performance counter instead of timeGetTime's
//                        whole milliseconds. The limiter (in 0x10011A00; esi the limiter object, [esi+0x1C] its timer,
//                        vtable 0x1032A118) waits in this loop:
//                          0x10012B05  mov ebp, [Sleep]          (the import at 0x103290F8)
//                          0x10012B1D  call [edx+0x30]           the timer's scale: fld [ecx+8] (0x10019230)
//                          0x10012B28  call [eax+0x24]           the timer's rate (0x10019550): 1000 / ms since its lap,
//                                                                timeGetTime - [t+0xC] - [t+0x1C] (the paused ms), 0 as 1
//                          0x10012B35  v = 60 / (scale * rate)   ticks of 1/60 s; while v < [esi+0x30] (an int):
//                          0x10012B60  Sleep(1), and again
//                        then 0x10012BCB calls the rate once more: 60 / rate + the carried fraction [esi+0x3C] gives the
//                        frame's whole ticks [esi+0x38] (_ftol 0x10311C2C), and 0x10012BF5 laps the timer (0x100194F0).
//                        With whole milliseconds the wait ends at 17 ms when [esi+0x30] is 1 and the scale 1.0: 58.8
//                        frames a second. (A second loop at 0x10012B7B runs only when the compare at 0x10012B72 is
//                        unordered, a NaN: `test ah, 5; jp`.) This changes two things, both only for those calls:
//                        the rate slot (0x1032A13C, holding 0x10019550) goes to sf_rate_thunk, which answers the three
//                        calls returning to 0x10012B2B, 0x10012B94 and 0x10012BCE from the performance counter: the ms
//                        since the call returning to 0x10012BCE a frame before, less [t+0x1C]; every other caller runs
//                        the game's function. And 0x10012B05 becomes `mov ebp, sf_wait_thunk; nop`: the wait sleeps on
//                        a high-resolution timer and spins the last ~1 ms to the moment v reaches [esi+0x30], instead
//                        of Sleep(1) and polling. The game's time stays exact: each frame's ms run from one 0x10012BCE
//                        call to the next, so they add up to the counter's time less the paused ms. Off, or a frame
//                        without the hooks: the game's rate and Sleep (the counter is still read at 0x10012BCE).
//   state_cache, vertex_shader_cache, extended_state_cache, state_cache_reset_each_scene, batch_up_draws,
//   batch_up_max_prims, batch_up_max_bytes: sf_device.cpp.
//
// Not carried over: occlusion_async_readback (measured slower than the stall it removes on the owner's
// dgVoodoo/D3D11/Parallels setup) and unsafe_async_batch_submit (no gain expected: every device call waits for it).
// occlusion_late (sf_occlusion.cpp, 18 Oct) answers the readback a probe late, at the owner's request; whether it
// saves time on dgVoodoo is not known (it is measured: its read's time per call), and on d3d8on11 the late lock reads
// a snapshot without waiting (kit/d3d8on11 README).
//
// Code changes are written on the render thread inside the kit's Present hook (xikit_mod_frame), where no game code
// that renders is running, after checking every byte they replace. speedfix_hook.dll must not be loaded as well.
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <cpuid.h>
#include "speedfix.h"

// an analysed address (base 0x10000000) where the game is loaded now
#define XI_GAME(a) sf_game_addr((uint32_t)(a))     /* the tree's XI_GAME (sf_game.cpp): another build's moved code too */

static const XikitHost *g_host;
SfConfig sf_cfg;
static char g_ini[MAX_PATH];
static FILETIME g_ini_time;
#define LOG(...) g_host->log(__VA_ARGS__)

// ---------------------------------------------------------------------------------------------- settings
static const char kDefaultIni[] =
  "; speedfix.ini -- the speedfix mod of the in-game kit (kit/speedfix/README.md). Re-read when it changes;\r\n"
  "; what changed is written to xikit.log. Delete this file to get the defaults back.\r\n"
  "[speedfix]\r\n"
  "; skip SetRenderState / SetTextureStageState / SetTexture / SetVertexShader that repeat the current value\r\n"
  "state_cache=1\r\nvertex_shader_cache=1\r\n"
  "; also SetTransform / SetViewport / SetLight / LightEnable / SetStreamSource / SetIndices\r\n"
  "extended_state_cache=0\r\n"
  "; forget the cache at each BeginScene / EndScene (guards against overlays that call the device below the kit)\r\n"
  "state_cache_reset_each_scene=1\r\n"
  "; merge back-to-back DrawPrimitiveUP calls into one; caps (0 = none) on one merged draw. With this and\r\n"
  ";   extended_state_cache off at the start, speedfix hooks only the cached setters (less overhead on every other\r\n"
  ";   call); turning either on then takes effect at the next start\r\n"
  "batch_up_draws=0\r\nbatch_up_max_prims=0\r\nbatch_up_max_bytes=0\r\n"
  "; even frame pacing (0 = off); 60, 30, 20 or 15 keep the game's time exact; more than 60 only with uncap=1\r\n"
  "frame_cap_fps=0\r\n"
  "; uncap=1: the game's own wait for a minimum of whole 1/60 s ticks a frame (60 fps with 1, 30 with 2) removed:\r\n"
  ";   speedfix's gated fork of Frame_Render reads that minimum ([g_pCamera+0x30], not written) as 0. Forked at the start\r\n"
  ";   when uncap or frame_step is on there; then live both ways\r\n"
  "uncap=0\r\n"
  "; the game's time step (Camera_GetFrameScale): 0 = the game's (with uncap=1 that is one tick every frame, so above\r\n"
  ";   60 fps what it drives runs faster than real time); 1 = real time in whole ticks: the game's own average of its\r\n"
  ";   last 4 frames, not truncated, its fraction carried to the next frame (frames of 0 ticks above 60 fps); 2 = as 1,\r\n"
  ";   and the 14 calls proven to only accumulate it get the fraction (data/frame_step_users.json). Below 60 fps 1 and\r\n"
  ";   2 also stop the game's under-count of time. Gated forks of Frame_Render and the two getters, made\r\n"
  ";   at the start when frame_step or uncap is on there; then live both ways\r\n"
  "frame_step=0\r\n"
  "; each actor's visibility re-tested every 4 (the game's), 8 or 16 frames\r\n"
  "occlusion_stagger=8\r\n"
  "; A/B switches: every actor visible without its query; every occlusion query visible without its readback\r\n"
  "skip_actor_occlusion=0\r\nskip_all_occlusion_readbacks=0\r\n"
  "; fewer textures CPU-encoded at load (1-15; textures loaded afterwards)\r\n"
  "texture_compression=1\r\ntexture_compression_value=15\r\n"
  "; the SSE math tier on any CPU with SSE (only before the game has chosen its tier)\r\n"
  "cpu_tier=1\r\n"
  "; levers to measure (each off by default; README.md):\r\n"
  "; hold render-state / texture-stage-state / texture changes until a draw reads them: a change undone before the\r\n"
  ";   draw never reaches the device (the game's UI quads turn alpha blending off and on again for each quad). Hooks\r\n"
  ";   every method that reads that state, so it takes effect at the next start when it was off at this one\r\n"
  "lazy_state=0\r\n"
  "; applying a state block the game recorded (BeginStateBlock..EndStateBlock) updates the state cache with what the\r\n"
  ";   block sets, instead of forgetting the whole cache (the effects apply 2-3 blocks each; docs/HISTORY.md 11 Oct);\r\n"
  ";   live both ways\r\n"
  "block_cache=0\r\n"
  "; a vertex shader with the same declaration (and no shader code) is made once and its handle given out again\r\n"
  ";   (loading: some 1,450 creations a session, 3 distinct shaders); the same at the next start or live\r\n"
  "vs_dedupe=0\r\n"
  "; the game's SSE skinning loops (0x1002F1D0, 0x100311A0) on any CPU that reports SSE: the game's CPU detection\r\n"
  ";   (0x1000D400) sets its SSE flag only for an Intel or Transmeta vendor string, so other CPUs, and the x86\r\n"
  ";   emulation of Windows on ARM if it reports another vendor, skin with x87; live both ways\r\n"
  "sse_skinning=0\r\n"
  "; one BeginScene/EndScene pair a frame: an EndScene is held, and dropped with the BeginScene after it, until Present,\r\n"
  ";   Reset, CopyRects, UpdateTexture or GetFrontBuffer (the game ends and begins scenes several times a frame)\r\n"
  "merge_scenes=0\r\n"
  "; a model's runs of triangle-list records (contiguous indices, no state call between them, no envmap, projected-\r\n"
  ";   texture or offscreen pass) drawn as one DrawIndexedPrimitive (docs/SKINMERGE.md). Forks SkinAndDrawV2 at the\r\n"
  ";   start when on there; then live both ways\r\n"
  "skin_merge=0\r\n"
  "; the occlusion readback answered from each query's previous probe: its picture is copied to a lockable render\r\n"
  ";   target of the query's own and read at the next probe, so the lock need not wait for the GPU (sf_occlusion.cpp).\r\n"
  ";   The answer is one probe late: an actor coming into view is drawn from its next probe. 1 = on; 2 = measure only\r\n"
  ";   (the game's answer, and how often the late one differs, in the log). Whether the late lock waits depends on\r\n"
  ";   the device: with d3d8on11 it need not (its log: 'readback snapshots'); on others it is not known. Live\r\n"
  "occlusion_late=0\r\n"
  "; at the start: each minute, frame time and draws, readbacks, new textures and creations per frame, split by\r\n"
  ";   whether the camera moved (the view transform changed); hooks the draws, SetTransform and the Create methods\r\n"
  "motion_stats=0\r\n"
  "; the game's frame limiter timed with the performance counter instead of whole milliseconds, and its wait made\r\n"
  ";   precise (a high-resolution timer, then a spin to the moment): 60.0 frames a second instead of 58.8. Live both\r\n"
  ";   ways; use it without frame_cap_fps\r\n"
  "precise_pacing=0\r\n"
  "; a test, not a speedup: the draw calls return without drawing (nothing is drawn); the game's own work and every\r\n"
  ";   other device call stay. Live both ways; use it with skip_all_occlusion_readbacks=1 (what the game reads back\r\n"
  ";   from an undrawn frame is wrong)\r\n"
  "null_draws=0\r\n"
  "; seconds between speedfix's statistics lines in xikit.log\r\n"
  "stats_every=60\r\n"
  "; A/B of levers with no keys: ab_levers takes the NAMES of settings above, e.g.\r\n"
  ";     ab_levers=merge_scenes; skip_all_occlusion_readbacks; sse_skinning\r\n"
  ";   (comma: measured together; semicolon: one after another). ab_after seconds after it is saved, speedfix\r\n"
  ";   switches by itself between A = those settings off (occlusion_stagger 4, the game's) and B = as set above,\r\n"
  ";   ab_phase seconds each, in the order A,B,B,A, ab_rounds times; nothing to type for A or B. The first\r\n"
  ";   ab_settle frames of a phase are left out. Results in xikit.log (lines 'speedfix: ab'). Stand still while it\r\n"
  ";   runs; keep xikit.ini's ab=0. Empty it and set it again for another run\r\n"
  "ab_levers=\r\nab_after=60\r\nab_phase=30\r\nab_rounds=4\r\nab_settle=60\r\n";

static int rd(const char *k, int def) { return (int)GetPrivateProfileIntA("speedfix", k, def, g_ini); }
static SfConfig read_config(void) {
  SfConfig c;
  c.state_cache = rd("state_cache", 1); c.vs_cache = rd("vertex_shader_cache", 1); c.ext_cache = rd("extended_state_cache", 0);
  c.scene_reset = rd("state_cache_reset_each_scene", 1); c.batch_up = rd("batch_up_draws", 0);
  c.batch_max_prims = rd("batch_up_max_prims", 0); c.batch_max_bytes = rd("batch_up_max_bytes", 0);
  c.frame_cap_fps = rd("frame_cap_fps", 0); c.occlusion_stagger = rd("occlusion_stagger", 8);
  c.skip_actor_occlusion = rd("skip_actor_occlusion", 0); c.skip_all_occlusion_readbacks = rd("skip_all_occlusion_readbacks", 0);
  c.texture_compression = rd("texture_compression", 1); c.texture_compression_value = rd("texture_compression_value", 15);
  c.cpu_tier = rd("cpu_tier", 1);
  c.lazy_state = rd("lazy_state", 0); c.vs_dedupe = rd("vs_dedupe", 0); c.sse_skinning = rd("sse_skinning", 0); c.block_cache = rd("block_cache", 0);
  c.merge_scenes = rd("merge_scenes", 0); c.skin_merge = rd("skin_merge", 0); c.motion_stats = rd("motion_stats", 0); c.stats_every = rd("stats_every", 60);
  c.precise_pacing = rd("precise_pacing", 0);
  c.uncap = rd("uncap", 0) ? 1 : 0; c.frame_step = rd("frame_step", 0); if (c.frame_step < 0 || c.frame_step > 2) c.frame_step = 0;
  c.null_draws = rd("null_draws", 0);
  c.occlusion_late = rd("occlusion_late", 0); if (c.occlusion_late < 0 || c.occlusion_late > 2) c.occlusion_late = 0;
  if (c.stats_every < 1) c.stats_every = 1;
  if (c.batch_max_prims < 0) c.batch_max_prims = 0;
  if (c.batch_max_bytes < 0) c.batch_max_bytes = 0;
  if (c.frame_cap_fps < 0 || (c.frame_cap_fps > 0 && c.frame_cap_fps < 10)) c.frame_cap_fps = 0;
  if (c.frame_cap_fps > (c.uncap ? 1000 : 60)) c.frame_cap_fps = c.uncap ? 1000 : 60;
  c.occlusion_stagger = c.occlusion_stagger >= 16 ? 16 : c.occlusion_stagger >= 8 ? 8 : 4;
  if (c.texture_compression_value < 0) c.texture_compression_value = 0;
  if (c.texture_compression_value > 15) c.texture_compression_value = 15;
  return c;
}
static bool ini_time(FILETIME *t) {
  WIN32_FILE_ATTRIBUTE_DATA a;
  if (!GetFileAttributesExA(g_ini, GetFileExInfoStandard, &a)) { memset(t, 0, sizeof *t); return false; }
  *t = a.ftLastWriteTime; return true;
}
static void log_config(const SfConfig &c, const char *why) {
  LOG("speedfix: settings (%s): state_cache=%d vertex_shader_cache=%d extended_state_cache=%d state_cache_reset_each_scene=%d "
      "batch_up_draws=%d batch_up_max_prims=%d batch_up_max_bytes=%d frame_cap_fps=%d occlusion_stagger=%d skip_actor_occlusion=%d "
      "skip_all_occlusion_readbacks=%d texture_compression=%d(%d) cpu_tier=%d lazy_state=%d block_cache=%d vs_dedupe=%d sse_skinning=%d merge_scenes=%d skin_merge=%d motion_stats=%d precise_pacing=%d null_draws=%d occlusion_late=%d uncap=%d frame_step=%d", why, c.state_cache, c.vs_cache, c.ext_cache,
      c.scene_reset, c.batch_up, c.batch_max_prims, c.batch_max_bytes, c.frame_cap_fps, c.occlusion_stagger, c.skip_actor_occlusion,
      c.skip_all_occlusion_readbacks, c.texture_compression, c.texture_compression_value, c.cpu_tier, c.lazy_state, c.block_cache, c.vs_dedupe, c.sse_skinning, c.merge_scenes, c.skin_merge, c.motion_stats, c.precise_pacing, c.null_draws, c.occlusion_late, c.uncap, c.frame_step);
  if (c.uncap && !c.frame_step)
    LOG("speedfix: uncap=1 with frame_step=0: the game advances one tick of 1/60 s every frame, so above 60 fps what its step "
        "drives runs faster than real time (frame_step=1 or 2 ties it to real time)");
  if (c.frame_cap_fps && !c.frame_step && (c.frame_cap_fps > 60 || 60 % c.frame_cap_fps))
    LOG("speedfix: frame_cap_fps=%d is not 60/n: with frame_step=0 the game's time step is whole 1/60 s ticks, so its time "
        "does not follow real time at this cap (60, 30, 20, 15 keep them equal; frame_step=1 or 2 at any cap)", c.frame_cap_fps);
}

// ---------------------------------------------------------------------------------------------- code changes
static bool live_bytes(uint32_t analysed, const uint8_t *expect, int n, uint8_t *live) {
  SIZE_T got = 0;
  if (!ReadProcessMemory(GetCurrentProcess(), (void *)(uintptr_t)XI_GAME(analysed), live, n, &got) || got != (SIZE_T)n) return false;
  return !memcmp(live, expect, n);
}
// re-aim a 5-byte `call rel32` at `to` (render thread, inside Present: the code is not running). The rel32 of a call
// within the game's DLL does not change when the DLL is relocated, so the expected bytes are the analysed ones.
static bool reaim_call(uint32_t site, const uint8_t expect[5], void *to, const char *what) {
  uint8_t live[5];
  if (!live_bytes(site, expect, 5, live)) {
    LOG("speedfix: %s not changed: the bytes at %08X are %02X %02X %02X %02X %02X, not %02X %02X %02X %02X %02X (another build, or "
        "something else changed them)", what, site, live[0], live[1], live[2], live[3], live[4], expect[0], expect[1], expect[2], expect[3], expect[4]);
    return false;
  }
  uint8_t *p = (uint8_t *)(uintptr_t)XI_GAME(site);
  int32_t rel = (int32_t)((uintptr_t)to - (uintptr_t)(p + 5));
  DWORD old;
  if (!VirtualProtect(p, 5, PAGE_EXECUTE_READWRITE, &old)) { LOG("speedfix: %s: VirtualProtect failed", what); return false; }
  memcpy(p + 1, &rel, 4);
  VirtualProtect(p, 5, old, &old);
  FlushInstructionCache(GetCurrentProcess(), p, 5);
  LOG("speedfix: %s: the call at %08X now goes to speedfix", what, site);
  return true;
}

// occlusion_stagger. The game: frame mod 4 == bucket -> query. Here: frame mod N == bucket + 4 k -> query, where k is
// the actor's share of the bucket (N / 4 shares). Answer the bucket on a query frame, another value mod 4 otherwise.
extern "C" int __cdecl sf_bucket(const uint8_t *actor) {
  int frame = *(const int *)(*(const uintptr_t *)(uintptr_t)XI_GAME(0x104568FCu) + 0x34);   // GetFrameCounter
  int n = sf_cfg.occlusion_stagger;
  if (n <= 4 || !sf_hooks_live()) return frame;                     // the game's own test
  int bucket = *(const int *)(actor + 0x9F0);
  int k = (int)(((uintptr_t)actor >> 4) * 2654435761u >> 16) % (n / 4);
  return frame % n == bucket + 4 * k ? bucket : (bucket + 1) & 3;
}
// called where the game called GetFrameCounter: the actor is in ESI (0x100CC584 `mov edi, [esi+0x9F0]`)
asm(".text\n.globl _sf_bucket_stub\n_sf_bucket_stub:\n  push %esi\n  call _sf_bucket\n  add $4, %esp\n  ret\n");
extern "C" void sf_bucket_stub(void);

// skip_all_occlusion_readbacks
typedef int (__thiscall *ReadbackFn)(void *, int);
static volatile LONG g_rb_skipped, g_rb_run;
static int __thiscall sf_readback(void *q, int flag) {
  if (sf_cfg.skip_all_occlusion_readbacks && flag == 0 && sf_hooks_live()) { g_rb_skipped++; return 256; }
  if (sf_cfg.occlusion_late && flag == 0 && sf_hooks_live()) return sf_occl_readback(q, sf_cfg.occlusion_late);
  g_rb_run++;
  return sf_occl_game(q, flag);              // the game's readback, timed (sf_occlusion.cpp)
}

// skip_actor_occlusion: the hook itself is in sf_game.cpp (it needs the tree's declaration of the function)
extern "C" { volatile LONG sf_aq_skipped, sf_aq_run; }

static bool g_bucket_done, g_readback_done, g_tex_saved;
static uint32_t g_tex_orig; static int g_tex_applied = -1;
static void apply_texture(void) {
  volatile uint32_t *g = (volatile uint32_t *)(uintptr_t)XI_GAME(0x10456AD0u);       // g_cfgTextureCompression
  int want = sf_cfg.texture_compression ? sf_cfg.texture_compression_value : -1;
  if (want == g_tex_applied) return;
  if (!g_tex_saved) { g_tex_orig = *g; g_tex_saved = true; }
  uint32_t before = *g;
  *g = want < 0 ? g_tex_orig : (uint32_t)want;
  g_tex_applied = want;
  if (want < 0) LOG("speedfix: texture_compression off: the game's value %u back (textures loaded from now on)", g_tex_orig);
  else LOG("speedfix: texture_compression %u -> %d (textures loaded from now on)", before, want);
}
// precise_pacing (head). Both thunks are reached only from the game's limiter, on the thread that runs it.
static uintptr_t g_pp_rate_orig, g_pp_sleep;                 // the game's rate (0x10019550) and its Sleep import
extern "C" { uintptr_t sf_pp_ret[3]; uintptr_t sf_pp_rate_orig; }   // 0x10012B2B, 0x10012B94, 0x10012BCE; the same rate
static LARGE_INTEGER g_pp_qpf; static LONGLONG g_pp_base;    // the counter at the last call returning to 0x10012BCE
static volatile LONG g_pp_frames, g_pp_waits, g_pp_sleeps, g_pp_late; static double g_pp_wait_ms; static DWORD g_pp_thread;
typedef float (__thiscall *RateFn)(void *);
// on, and the frame went through the hooks: the one now (the limiter ran before Present) or the one just presented
// (it runs after Present, before the next BeginScene). Which of the two the game does is not established here.
static bool pp_on(void) { return sf_cfg.precise_pacing && sf_frame_no - sf_hooks_frame <= 1; }
extern "C" float __cdecl sf_rate(uint8_t *timer, uintptr_t ret) {
  LARGE_INTEGER now; QueryPerformanceCounter(&now);
  LONGLONG base = g_pp_base;
  if (ret == sf_pp_ret[2]) g_pp_base = now.QuadPart;         // the next frame's start, whether on or off
  if (!base || !pp_on()) return ((RateFn)g_pp_rate_orig)(timer);
  double ms = (now.QuadPart - base) * 1000.0 / (double)g_pp_qpf.QuadPart - *(int32_t *)(timer + 0x1C);
  if (!(ms > 0.001)) return ((RateFn)g_pp_rate_orig)(timer);  // (the paused ms exceed the counter's: the game's own)
  float r = (float)(1000.0 / ms);
  *(float *)(timer + 4) = r;                                 // as the game's rate stores it (0x10019583)
  if (ret == sf_pp_ret[2]) g_pp_frames++;
  return r;
}
static HANDLE g_pp_timer; static bool g_pp_timer_tried;
static volatile int32_t g_pp_ticks; static volatile float g_pp_scale;   // as the last wait read them, for the log
extern "C" void __cdecl sf_wait(uint8_t *obj, DWORD ms) {
  if (!g_pp_thread) g_pp_thread = GetCurrentThreadId();
  uint8_t *timer = *(uint8_t **)(obj + 0x1C);
  int32_t ticks = *(int32_t *)(obj + 0x30);
  float scale = *(float *)(timer + 8);
  g_pp_ticks = ticks; g_pp_scale = scale;
  LARGE_INTEGER t0; QueryPerformanceCounter(&t0);
  if (!g_pp_base || !pp_on() || ticks <= 0 || !(scale > 0.0f && scale < 100.0f)) {
    g_pp_sleeps++; ((void (WINAPI *)(DWORD))g_pp_sleep)(ms);
  } else {
    // the moment v = 60 / (scale * rate) reaches ticks: rate = 1000 / ms, so ms = ticks * 1000 * scale / 60 (+ 2 us,
    // so that the game's float compare does not land a hair short and wait again)
    double want = ticks * 1000.0 * scale / 60.0 + *(int32_t *)(timer + 0x1C) + 0.002;
    LONGLONG end = g_pp_base + (LONGLONG)(want * (double)g_pp_qpf.QuadPart / 1000.0);
    double remain = (end - t0.QuadPart) * 1000.0 / (double)g_pp_qpf.QuadPart;
    if (remain > 1000.0) { remain = 1000.0; end = t0.QuadPart + g_pp_qpf.QuadPart; }   // the game asks again
    if (remain > 0) {
      if (!g_pp_timer_tried) {
        g_pp_timer_tried = true;
        typedef HANDLE (WINAPI *CWTEx)(LPSECURITY_ATTRIBUTES, LPCWSTR, DWORD, DWORD);
        CWTEx f = (CWTEx)(void *)GetProcAddress(GetModuleHandleA("kernel32.dll"), "CreateWaitableTimerExW");
        g_pp_timer = f ? f(NULL, NULL, 0x2 /* CREATE_WAITABLE_TIMER_HIGH_RESOLUTION */, TIMER_ALL_ACCESS) : NULL;
      }
      if (remain > 1.5 && g_pp_timer) {         // sleep most of it on a high-resolution timer, spin the last ~1 ms
        LARGE_INTEGER due; due.QuadPart = -(LONGLONG)((remain - 1.0) * 10000.0);
        if (SetWaitableTimer(g_pp_timer, &due, 0, NULL, NULL, FALSE)) WaitForSingleObject(g_pp_timer, 1100);
      } else if (remain > 1.5) ((void (WINAPI *)(DWORD))g_pp_sleep)((DWORD)(remain - 1.0));
      LARGE_INTEGER t;
      do { YieldProcessor(); QueryPerformanceCounter(&t); } while (t.QuadPart < end);
    } else g_pp_late++;
    g_pp_waits++;
  }
  LARGE_INTEGER t1; QueryPerformanceCounter(&t1);
  g_pp_wait_ms += (t1.QuadPart - t0.QuadPart) * 1000.0 / (double)g_pp_qpf.QuadPart;
}
// thiscall, the timer in ECX: the limiter's three calls (by their return address) to sf_rate, every other to the game's
asm(".text\n.globl _sf_rate_thunk\n_sf_rate_thunk:\n"
    "  movl (%esp), %eax\n  cmpl _sf_pp_ret, %eax\n  je 1f\n  cmpl _sf_pp_ret+4, %eax\n  je 1f\n  cmpl _sf_pp_ret+8, %eax\n  je 1f\n"
    "  jmp *_sf_pp_rate_orig\n"
    "1:\n  pushl %eax\n  pushl %ecx\n  call _sf_rate\n  addl $8, %esp\n  ret\n");
// in place of Sleep (stdcall, one argument), called with the limiter object in ESI
asm(".text\n.globl _sf_wait_thunk\n_sf_wait_thunk:\n"
    "  pushl 4(%esp)\n  pushl %esi\n  call _sf_wait\n  addl $8, %esp\n  ret $4\n");
extern "C" void sf_rate_thunk(void); extern "C" void sf_wait_thunk(void);
static bool g_pp_done;
static void apply_precise_pacing(void) {
  if (g_pp_done || !sf_cfg.precise_pacing) return;
  g_pp_done = true;
  QueryPerformanceFrequency(&g_pp_qpf);
  volatile LONG *slot = (volatile LONG *)(uintptr_t)XI_GAME(0x1032A13Cu);      // the timer vtable's rate entry
  uint8_t *mov = (uint8_t *)(uintptr_t)XI_GAME(0x10012B05u);
  uintptr_t rate = XI_GAME(0x10019550u), imp = XI_GAME(0x103290F8u);
  uint8_t expect[6] = {0x8B, 0x2D}, live[6];                                   // mov ebp, [imp]
  memcpy(expect + 2, &imp, 4);
  if ((uintptr_t)(uint32_t)*slot != rate) {
    LOG("speedfix: precise_pacing not applied: the timer's rate entry at %08lX is %08lX, not %08lX", (unsigned long)(uintptr_t)slot,
        (unsigned long)(uint32_t)*slot, (unsigned long)rate);
    return;
  }
  if (!live_bytes(0x10012B05u, expect, 6, live)) {
    LOG("speedfix: precise_pacing not applied: the bytes at 10012B05 are %02X %02X %02X %02X %02X %02X, not mov ebp, [Sleep]",
        live[0], live[1], live[2], live[3], live[4], live[5]);
    return;
  }
  g_pp_rate_orig = sf_pp_rate_orig = rate; g_pp_sleep = *(uintptr_t *)imp;
  sf_pp_ret[0] = XI_GAME(0x10012B2Bu); sf_pp_ret[1] = XI_GAME(0x10012B94u); sf_pp_ret[2] = XI_GAME(0x10012BCEu);
  DWORD old;
  // the code first: until the slot changes, sf_wait sees no counter base and sleeps as the game does
  if (!VirtualProtect(mov, 6, PAGE_EXECUTE_READWRITE, &old)) { LOG("speedfix: precise_pacing: VirtualProtect failed"); return; }
  uintptr_t w = (uintptr_t)sf_wait_thunk; uint8_t code[6] = {0xBD, 0, 0, 0, 0, 0x90};    // mov ebp, imm32; nop
  memcpy(code + 1, &w, 4); memcpy(mov, code, 6);
  VirtualProtect(mov, 6, old, &old);
  FlushInstructionCache(GetCurrentProcess(), mov, 6);
  if (!VirtualProtect((void *)slot, 4, PAGE_READWRITE, &old)) { LOG("speedfix: precise_pacing: VirtualProtect of the vtable failed"); return; }
  InterlockedCompareExchange(slot, (LONG)(uintptr_t)sf_rate_thunk, (LONG)rate);
  VirtualProtect((void *)slot, 4, old, &old);
  LOG("speedfix: precise_pacing: the frame limiter's wait (10012B05) and its timer's rate entry (1032A13C) now go to speedfix");
}
static void pp_stats(void) {
  static bool told;
  LONG f = InterlockedExchange(&g_pp_frames, 0), w = InterlockedExchange(&g_pp_waits, 0), s = InterlockedExchange(&g_pp_sleeps, 0);
  LONG late = InterlockedExchange(&g_pp_late, 0); double ms = g_pp_wait_ms; g_pp_wait_ms = 0;
  if (!g_pp_done) return;
  if (!told && g_pp_thread) {
    told = true;
    LOG("speedfix: precise_pacing: the limiter runs on thread %lu, the frame callback on %lu", (unsigned long)g_pp_thread, (unsigned long)GetCurrentThreadId());
  }
  if (f + w + s) LOG("speedfix: precise_pacing: %ld frames timed by the counter; the limiter waited %.1f ms in all (%ld precise waits, %ld "
                     "already past their moment, %ld Sleep calls as the game's); its tick count [+0x30] %d, its timer's scale [+8] %.4f",
                     f, ms, w, late, s, (int)g_pp_ticks, (double)g_pp_scale);
}
// uncap and frame_step (the comment at the top of the file): forks of the game's verified functions (sf_fork.cpp;
// fork/Frame_Render.fork.cpp, Camera_GetFrameScale.fork.cpp, Camera_GetClampedField28.fork.cpp), each the game's function with its
// switch at 0 (the gate without declarations) and with it on differing only as declared (fork/framestep*.declare.json).
// Here, each frame (render thread, inside Present): the switches, on only while the kit's hooks are live, and the
// figures for the statistics line.
extern "C" { extern volatile int xi_step_mode, xi_uncap, xi_step_have; extern float xi_step_whole, xi_step_frac; extern int sf_framestep_installed; }
static LARGE_INTEGER g_st_qpf; static LONGLONG g_st_last;
static LONG g_st_frames, g_st_zero; static double g_st_whole, g_st_frac, g_st_game, g_st_ms;
static void apply_uncap_step(void) {
  bool live = sf_frame_no - sf_hooks_frame <= 1;
  xi_uncap = sf_framestep_installed && sf_cfg.uncap && live;
  xi_step_mode = sf_framestep_installed && live ? sf_cfg.frame_step : 0;
  if (!sf_framestep_installed || !xi_step_mode || !xi_step_have) { g_st_last = 0; return; }
  if (!g_st_qpf.QuadPart) QueryPerformanceFrequency(&g_st_qpf);
  LARGE_INTEGER now; QueryPerformanceCounter(&now);
  if (g_st_last) g_st_ms += (now.QuadPart - g_st_last) * 1000.0 / (double)g_st_qpf.QuadPart;
  g_st_last = now.QuadPart;
  uint8_t *cam = *(uint8_t *volatile *)(uintptr_t)XI_GAME(0x104568FCu);
  float g = cam ? *(volatile float *)(cam + 0x28) : 1.0f;      // what the game's getters answer: max([+0x28], 1.0)
  g_st_frames++; if (xi_step_whole == 0) g_st_zero++;
  g_st_whole += xi_step_whole; g_st_frac += xi_step_frac; g_st_game += !(g < 1.0f) ? g : 1.0f;
}
static void st_stats(void) {
  LONG f = g_st_frames, z = g_st_zero; g_st_frames = g_st_zero = 0;
  double w = g_st_whole, fr = g_st_frac, gm = g_st_game, ms = g_st_ms; g_st_whole = g_st_frac = g_st_game = g_st_ms = 0;
  if (!sf_framestep_installed || !f) return;
  double real = ms * 0.06;
  LOG("speedfix: frame_step=%d uncap=%d: %ld frames, %.0f ms by the counter (%.1f ticks of 1/60 s); speedfix's step (what frame_step 1 "
      "and 2 answer) added up to: whole %.0f (%.1f%%; %ld frames of 0), fraction %.1f (%.1f%%); the game's own (what frame_step 0 "
      "answers) %.0f (%.1f%%)", sf_cfg.frame_step, sf_cfg.uncap, f, ms, real,
      w, real > 0 ? 100 * w / real : 0.0, z, fr, real > 0 ? 100 * fr / real : 0.0, gm, real > 0 ? 100 * gm / real : 0.0);
}
static void apply_sse_skinning(void);
static void log_cpu_tier(void);
static void apply_code(void) {                      // render thread, inside Present
  static const uint8_t kBucketCall[5] = {0xE8, 0xC1, 0x87, 0xF4, 0xFF};     // 0x100CC58A call 0x10014D50
  static const uint8_t kReadbackCall[5] = {0xE8, 0xD4, 0x03, 0x00, 0x00};   // 0x1006C4D7 call 0x1006C8B0
  if (!g_bucket_done && sf_cfg.occlusion_stagger > 4) { g_bucket_done = true; reaim_call(0x100CC58Au, kBucketCall, (void *)sf_bucket_stub, "occlusion_stagger"); }
  if (!g_readback_done && (sf_cfg.skip_all_occlusion_readbacks || sf_cfg.occlusion_late)) { g_readback_done = true; reaim_call(0x1006C4D7u, kReadbackCall, (void *)sf_readback, sf_cfg.occlusion_late ? "occlusion_late" : "skip_all_occlusion_readbacks"); }
  apply_texture();
  apply_precise_pacing();
  apply_uncap_step();
  apply_sse_skinning();
  xi_skinmerge_on = sf_cfg.skin_merge && sf_skinmerge_installed && sf_hooks_live();   // the game's draws otherwise
  sf_group_on = sf_cfg.skin_merge >= 2 && sf_skinmerge_installed && sf_hooks_live();   // skin_merge=2: a model's draws grouped (sf_device.cpp)
  log_cpu_tier();
}

/* sse_skinning. The game's CPU object ([0x104566E8], made at start by 0x1000D380; detection 0x1000D400) has a byte at
   +9 that only its getter 0x1000D680 reads, called by two functions: the skinning dispatcher 0x1002F1D0 (SSE loop at
   0x100300B0 when +9 or +0xC is set, else the x87 loop at 0x1002F200) and 0x100311A0. Detection sets +9 to CPUID.1:EDX
   bit 25 (SSE) only for vendor class 1 (a "GenuineTMx86" vendor, or "Intel" from the vendor string's first 'I'). This
   writes the CPU's own SSE bit there while the lever is on and the kit's hooks are live, and the game's value back
   otherwise; on the render thread, where the skinning runs (the owner's samples), so never during a skinning call. */
static void apply_sse_skinning(void) {
  static int have_sse = -1, saved = -1; static bool logged;
  volatile uint8_t **pp = (volatile uint8_t **)(uintptr_t)XI_GAME(0x104566E8u);
  volatile uint8_t *obj = *pp;
  if (!obj) return;
  if (have_sse < 0) { unsigned a, b, c, d; have_sse = __get_cpuid(1, &a, &b, &c, &d) && (d >> 25 & 1); }
  if (saved < 0) {
    saved = obj[9];
    LOG("speedfix: sse_skinning: the game's SSE flag is %d (the CPU reports SSE: %s)", saved, have_sse ? "yes" : "no");
  }
  int want = sf_cfg.sse_skinning && have_sse && sf_hooks_live() ? 1 : saved;
  if (obj[9] != want) obj[9] = (uint8_t)want;
  if (sf_cfg.sse_skinning && !have_sse && !logged) { logged = true; LOG("speedfix: sse_skinning: the CPU reports no SSE; left as the game set it"); }
}

/* which math tier the game chose (speedfix_hook's LogCpuTierInUse): g_pProcessor's vtable is one of the three tier
   classes CreateProcessorInstance (0x1006CB50) makes. Logged once, when the game has chosen. */
static void log_cpu_tier(void) {
  static bool done;
  if (done) return;
  uintptr_t proc = *(volatile uint32_t *)(uintptr_t)XI_GAME(0x1047CF7Cu);
  if (!proc) return;
  done = true;
  uintptr_t vt = *(volatile uint32_t *)proc;
  const char *t = vt == XI_GAME(0x1032C298u) ? "SSE" : vt == XI_GAME(0x1032C1C8u) ? "3DNow!" : vt == XI_GAME(0x1032C368u) ? "x87" : "unknown";
  LOG("speedfix: cpu tier in use: %s (g_pProcessor's vtable %08lX)", t, (unsigned long)vt);
}

static void apply_cpu_tier(void) {
  if (*(volatile uint32_t *)(uintptr_t)XI_GAME(0x1047CF7Cu)) {                       // g_pProcessor
    LOG("speedfix: cpu_tier: the game has already chosen its math tier; it applies at the next start if the kit is injected earlier");
    return;
  }
  volatile LONG *dw = (volatile LONG *)(uintptr_t)XI_GAME(0x1006CDDCu);              // 84 C0 74 19: test al, al; je +0x19
  const LONG expect = 0x1974C084, want = (LONG)0x9090C084;
  DWORD old;
  if (*dw != expect) { LOG("speedfix: cpu_tier not changed: the dword at 1006CDDC is %08lX, not %08lX", (unsigned long)*dw, (unsigned long)expect); return; }
  if (!VirtualProtect((void *)dw, 4, PAGE_EXECUTE_READWRITE, &old)) { LOG("speedfix: cpu_tier: VirtualProtect failed"); return; }
  LONG prev = InterlockedCompareExchange(dw, want, expect);
  VirtualProtect((void *)dw, 4, old, &old);
  FlushInstructionCache(GetCurrentProcess(), (void *)dw, 4);
  LOG(prev == expect ? "speedfix: cpu_tier: the vendor test before the SSE check is removed" : "speedfix: cpu_tier: the bytes changed meanwhile; not changed");
}

// ---------------------------------------------------------------------------------------------- the lever A/B
// xikit's ab=1 compares the game with and without everything the kit adds; this compares speedfix with and without
// the settings in ab_levers, the rest of speedfix on in both. Same design: phases A B B A (a scene that drifts
// steadily affects both labels alike), the first ab_settle frames of each left out, and frames whose scene did not go
// through speedfix's hooks (F9, a trace) too. Frame time: from one frame callback (before Present) to the next.
struct Lever { const char *key; int SfConfig::*field; int off; };
static const Lever kLevers[] = {
  {"state_cache", &SfConfig::state_cache, 0}, {"vertex_shader_cache", &SfConfig::vs_cache, 0},
  {"extended_state_cache", &SfConfig::ext_cache, 0}, {"state_cache_reset_each_scene", &SfConfig::scene_reset, 0},
  {"batch_up_draws", &SfConfig::batch_up, 0}, {"lazy_state", &SfConfig::lazy_state, 0}, {"block_cache", &SfConfig::block_cache, 0}, {"vs_dedupe", &SfConfig::vs_dedupe, 0},
  {"occlusion_stagger", &SfConfig::occlusion_stagger, 4}, {"skip_actor_occlusion", &SfConfig::skip_actor_occlusion, 0},
  {"skip_all_occlusion_readbacks", &SfConfig::skip_all_occlusion_readbacks, 0}, {"frame_cap_fps", &SfConfig::frame_cap_fps, 0},
  {"sse_skinning", &SfConfig::sse_skinning, 0}, {"merge_scenes", &SfConfig::merge_scenes, 0},
  {"skin_merge", &SfConfig::skin_merge, 0}, {"precise_pacing", &SfConfig::precise_pacing, 0},
  {"null_draws", &SfConfig::null_draws, 0}, {"occlusion_late", &SfConfig::occlusion_late, 0},
  {"uncap", &SfConfig::uncap, 0}, {"frame_step", &SfConfig::frame_step, 0},
};
enum { NLEV = sizeof kLevers / sizeof kLevers[0], AB_MAX = 16384 };
static SfConfig g_base;                     // the settings as the ini has them (B)
static char g_ab_list[256];                 // ab_levers as read
static char g_ab_cur[256]; static int g_ab_grp;   // the group running now (ab_levers groups split by ';' run in turn)
static bool g_ab_use[NLEV]; static int g_ab_state;   // 0 idle, 1 waiting ab_after, 2 running, 3 done
static DWORD g_ab_t0; static int g_ab_i, g_ab_nph, g_ab_seen, g_ab_n, g_ab_phase, g_ab_settle, g_ab_after, g_ab_run;
static double g_ab_ft[AB_MAX], g_ab_mean[32], g_ab_med[32], g_ab_p95[32]; static int g_ab_cnt[32];
static LARGE_INTEGER g_ab_qpf, g_ab_last; static int g_ab_lastFrame = -1;
static char ab_label(int i) { return ((i + 1) / 2) % 2 ? 'B' : 'A'; }
static SfConfig ab_config(bool a) {
  SfConfig c = g_base;
  if (a) for (int i = 0; i < NLEV; i++) if (g_ab_use[i]) c.*(kLevers[i].field) = kLevers[i].off;
  return c;
}
static void set_config(const SfConfig &c) { SfConfig before = sf_cfg; sf_cfg = c; sf_device_config_changed(&before); }
static int cmp_d(const void *a, const void *b) { double x = *(const double *)a, y = *(const double *)b; return x < y ? -1 : x > y; }
static void ab_set(int i) {
  g_ab_i = i; g_ab_t0 = GetTickCount(); g_ab_seen = 0; g_ab_n = 0;
  set_config(ab_config(ab_label(i) == 'A'));
  LOG("speedfix: ab: phase %d of %d: %c (%s)", i + 1, g_ab_nph, ab_label(i), ab_label(i) == 'A' ? "the levers off" : "the levers as set");
}
static void ab_end_phase(void) {
  int i = g_ab_i, n = g_ab_n; double sum = 0;
  for (int k = 0; k < n; k++) sum += g_ab_ft[k];
  qsort(g_ab_ft, n, sizeof g_ab_ft[0], cmp_d);
  g_ab_cnt[i] = n; g_ab_mean[i] = n ? sum / n : 0; g_ab_med[i] = n ? g_ab_ft[n / 2] : 0; g_ab_p95[i] = n ? g_ab_ft[(int)(n * 0.95)] : 0;
  LOG("speedfix: ab: phase %d (%c): %d frames, mean %.2f ms, median %.2f ms, p95 %.2f ms, max %.1f ms", i + 1, ab_label(i), n,
      g_ab_mean[i], g_ab_med[i], g_ab_p95[i], n ? g_ab_ft[n - 1] : 0.0);
}
static void ab_finish(void) {
  set_config(g_base);
  double s[2] = {0, 0}, m[2] = {0, 0}, p[2] = {0, 0}, lo[2] = {1e9, 1e9}, hi[2] = {0, 0}; int c[2] = {0, 0};
  for (int i = 0; i < g_ab_nph; i++) {
    if (!g_ab_cnt[i]) continue;
    int b = ab_label(i) == 'B'; s[b] += g_ab_mean[i]; m[b] += g_ab_med[i]; p[b] += g_ab_p95[i]; c[b]++;
    if (g_ab_mean[i] < lo[b]) lo[b] = g_ab_mean[i];
    if (g_ab_mean[i] > hi[b]) hi[b] = g_ab_mean[i];
  }
  if (!c[0] || !c[1]) { LOG("speedfix: ab %d: done, but a label has no frames: nothing to compare", g_ab_run); return; }
  double a = s[0] / c[0], b = s[1] / c[1], noise = hi[0] - lo[0] > hi[1] - lo[1] ? hi[0] - lo[0] : hi[1] - lo[1];
  LOG("speedfix: ab %d (%s): A (off) %.2f ms, B (on) %.2f ms: B - A %+.2f ms (%+.1f%%), medians %+.2f ms, p95 %+.2f ms; "
      "phases of one label differ by up to %.2f ms%s", g_ab_run, g_ab_cur, a, b, b - a, a > 0 ? 100.0 * (b - a) / a : 0.0,
      m[1] / c[1] - m[0] / c[0], p[1] / c[1] - p[0] / c[0], noise, c[0] < 2 || c[1] < 2 ? " (one phase of a label: no measure of the noise; use ab_rounds of 2 or more)"
      : fabs(b - a) <= noise ? " (the difference is within that)" : "");
}
static void ab_read(void) {                 // ab_levers and its timings from the ini (each re-read)
  char list[256]; GetPrivateProfileStringA("speedfix", "ab_levers", "", list, sizeof list, g_ini);
  if (strcmp(list, g_ab_list)) {
    if (g_ab_state == 1 || g_ab_state == 2) { set_config(g_base); LOG("speedfix: ab: ab_levers changed: this run stopped, the settings as set"); }
    g_ab_state = 0; g_ab_grp = 0; strcpy(g_ab_list, list);
  }
  if (g_ab_state || !list[0]) return;
  // the group g_ab_grp of the ';'-separated list
  { char *g = list; for (int k = 0; k < g_ab_grp && g; k++) { g = strchr(g, ';'); if (g) g++; }
    if (!g) { g_ab_state = 3; LOG("speedfix: ab: every group of ab_levers has run; empty it and set it again for another run"); return; }
    char *e = strchr(g, ';'); if (e) *e = 0;
    memmove(list, g, strlen(g) + 1); snprintf(g_ab_cur, sizeof g_ab_cur, "%s", list); }
  int n = 0; char bad[256] = "";
  for (int i = 0; i < NLEV; i++) g_ab_use[i] = false;
  for (char *t = strtok(list, ", \t"); t; t = strtok(NULL, ", \t")) {
    int j = 0; while (j < NLEV && strcmp(kLevers[j].key, t)) j++;
    if (j == NLEV) { snprintf(bad + strlen(bad), sizeof bad - strlen(bad), " %s (not a lever)", t); continue; }
    if (!sf_device_allows(t) || (!strcmp(t, "skin_merge") && !sf_skinmerge_installed) || ((!strcmp(t, "frame_step") || !strcmp(t, "uncap")) && !sf_framestep_installed)) { snprintf(bad + strlen(bad), sizeof bad - strlen(bad), " %s (off at this start: its hooks are not in place)", t); continue; }
    if (g_base.*(kLevers[j].field) == kLevers[j].off) { snprintf(bad + strlen(bad), sizeof bad - strlen(bad), " %s (set to its off value: A and B would be the same)", t); continue; }
    g_ab_use[j] = true; n++;
  }
  if (bad[0]) LOG("speedfix: ab: left out:%s%s", bad, strstr(bad, "(not a lever)") ? " -- ab_levers takes setting names, e.g. ab_levers=merge_scenes; sse_skinning (A and B are switched by speedfix itself)" : "");
  if (!n) { LOG("speedfix: ab: group %d (%s): no lever to compare; skipped", g_ab_grp + 1, g_ab_cur); g_ab_grp++; return; }
  g_ab_after = g_ab_grp ? 5 : rd("ab_after", 60); g_ab_phase = rd("ab_phase", 30); g_ab_settle = rd("ab_settle", 60);
  int rounds = rd("ab_rounds", 4);
  if (g_ab_after < 0) g_ab_after = 0;
  if (g_ab_phase < 1) g_ab_phase = 1;
  if (rounds < 1) rounds = 1;
  if (rounds > 16) rounds = 16;
  if (g_ab_settle < 0) g_ab_settle = 0;
  g_ab_nph = 2 * rounds; for (int i = 0; i < 32; i++) g_ab_cnt[i] = 0;
  g_ab_state = 1; g_ab_t0 = GetTickCount();
  LOG("speedfix: ab: group %d (%s): starts in %d s: %d phases of %d s (A B B A ...), the first %d frames of each left out; stand still",
      g_ab_grp + 1, g_ab_cur, g_ab_after, g_ab_nph, g_ab_phase, g_ab_settle);
}
static void ab_control(void) {              // each frame, render thread
  LARGE_INTEGER now; QueryPerformanceCounter(&now);
  if (!g_ab_qpf.QuadPart) QueryPerformanceFrequency(&g_ab_qpf);
  // the frame that just ended: its time counts when its scene went through the hooks and no phase began meanwhile
  if (g_ab_state == 2 && g_ab_last.QuadPart && g_ab_lastFrame == sf_frame_no && sf_hooks_live()) {
    double ms = (now.QuadPart - g_ab_last.QuadPart) * 1000.0 / (double)g_ab_qpf.QuadPart;
    if (g_ab_seen++ >= g_ab_settle && g_ab_n < AB_MAX) g_ab_ft[g_ab_n++] = ms;
  }
  g_ab_last = now; g_ab_lastFrame = sf_frame_no + 1;  // sf_device_frame numbers the next frame
  if (g_ab_state == 1 && GetTickCount() - g_ab_t0 >= (DWORD)g_ab_after * 1000) { g_ab_run++; g_ab_state = 2; ab_set(0); }
  else if (g_ab_state == 2 && GetTickCount() - g_ab_t0 >= (DWORD)g_ab_phase * 1000) {
    ab_end_phase();
    if (g_ab_i + 1 < g_ab_nph) ab_set(g_ab_i + 1);
    else { ab_finish(); g_ab_grp++; g_ab_state = 0; }      // the next group, if any, at the next read (ab_read)
  }
}

// ---------------------------------------------------------------------------------------------- the mod
extern "C" int sf_init(const XikitHost *host) {
  g_host = host;
  if (GetModuleHandleA("speedfix_hook.dll")) {
    LOG("speedfix: speedfix_hook.dll is loaded in this process; this mod does the same work and does nothing beside it");
    return 1;
  }
  snprintf(g_ini, sizeof g_ini, "%s\\speedfix.ini", host->dir);
  if (GetFileAttributesA(g_ini) == INVALID_FILE_ATTRIBUTES) {
    FILE *f = fopen(g_ini, "wb");
    if (f) { fwrite(kDefaultIni, 1, sizeof kDefaultIni - 1, f); fclose(f); LOG("speedfix: %s written with the defaults", g_ini); }
  }
  sf_cfg = g_base = read_config(); ini_time(&g_ini_time);
  log_config(sf_cfg, "start");
  if (sf_cfg.cpu_tier) apply_cpu_tier();
  sf_occl_init(host);
  if (sf_device_install(host) != 0) return 1;
  if (sf_hook_query(host) != XIKIT_OK) LOG("speedfix: Occlusion_QueryScreenRect not hooked: skip_actor_occlusion does nothing");
  if (sf_cfg.skin_merge && sf_fork_skinmerge(host) != XIKIT_OK) LOG("speedfix: SkinAndDrawV2 not forked: skin_merge does nothing");
  if ((sf_cfg.frame_step || sf_cfg.uncap) && sf_fork_framestep(host) != XIKIT_OK) LOG("speedfix: Frame_Render and the step getters not forked: frame_step and uncap do nothing");
  else if (sf_framestep_installed) LOG("speedfix: frame_step / uncap: Frame_Render (0x10011A00), Camera_GetFrameScale (0x10014CF0) and Camera_GetClampedField28 (0x10014D20) run speedfix's gated forks");
  return 0;
}

extern "C" void sf_frame(void) {
  static int n; static DWORD last_stats = GetTickCount();
  if (++n % 60 == 0) {                                 // the settings, when the file changed
    FILETIME t; ini_time(&t);
    if (CompareFileTime(&t, &g_ini_time)) {
      g_ini_time = t;
      SfConfig before = g_base; g_base = read_config();
      set_config(g_ab_state == 2 ? ab_config(ab_label(g_ab_i) == 'A') : g_base);
      // what this start's hooks cannot do stays off in B as well (sf_device_config_changed has logged it)
      if (!sf_device_allows("batch_up_draws")) g_base.batch_up = g_base.ext_cache = 0;
      if (!sf_device_allows("lazy_state")) g_base.lazy_state = 0;
      if ((g_base.frame_step || g_base.uncap) && !sf_framestep_installed) { g_base.frame_step = g_base.uncap = 0; LOG("speedfix: frame_step / uncap take effect at the next start (both were off at this one, so Frame_Render is not forked)"); }
      if (g_base.skin_merge && !sf_skinmerge_installed) { g_base.skin_merge = 0; LOG("speedfix: skin_merge takes effect at the next start (it was off at this one, so SkinAndDrawV2 is not forked)"); }
      log_config(g_base, "changed");
      if (sf_cfg.cpu_tier && !before.cpu_tier) LOG("speedfix: cpu_tier is decided once per start");
    }
  }
  if (n % 60 == 1) ab_read();
  ab_control();
  apply_code();
  sf_occl_frame(sf_cfg.skip_all_occlusion_readbacks && sf_hooks_live() ? -1 : sf_cfg.occlusion_late && sf_hooks_live());
  sf_device_frame();
  if (GetTickCount() - last_stats >= (DWORD)sf_cfg.stats_every * 1000) {
    last_stats = GetTickCount();
    sf_device_stats();
    LONG rs = InterlockedExchange(&g_rb_skipped, 0), rr = InterlockedExchange(&g_rb_run, 0);
    LONG as = InterlockedExchange(&sf_aq_skipped, 0), ar = InterlockedExchange(&sf_aq_run, 0);
    if (rs + rr) LOG("speedfix: occlusion readbacks: %ld run, %ld skipped", rr, rs);
    if (as + ar) LOG("speedfix: actor occlusion queries: %ld run, %ld skipped", ar, as);
    sf_occl_stats();
    pp_stats();
    st_stats();
  }
}
