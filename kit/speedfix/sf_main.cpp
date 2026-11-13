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
  ";   2 also stop the game's under-count of time; 3 = as 2, and the 5 loops that run once per whole tick (the camera's\r\n"
  ";   follow, the actors' position and turning: data/frame_step_loops.json) take the fraction too, with the factor\r\n"
  ";   that gives a fraction of a tick exactly (no frames where actors stop). Gated forks of Frame_Render and the two getters, made\r\n"
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
  "; gpu_skin=1 (needs d3d8on11 with gpu_skin=1 in d3d8on11.ini): characters are skinned in d3d8on11's vertex shader;\r\n"
  "; the game's thread neither computes nor uploads their vertices. It takes effect at the next start (SkinAndDrawV2\r\n"
  "; is forked), then switches live; where the device declines a model, the game skins it as before.\r\n"
  "gpu_skin=0\r\n"
  "; a measurement: the draw calls counted by the task the game's task pass is running (its vtable, and whether in the\r\n"
  ";   task's +0x20 call -- the actors' OnMove -- or its +0x24), logged with the stats. Hooks CMoTask::Dispatch at the start\r\n"
  "task_draws=0\r\n"
  "; Liminality, stage 1 (needs uncap=1): the game's own logic on whole ticks of 1/60 s (frame_step 1's answers, none of\r\n"
  ";   frame_step 3's changes), and on a frame of 0 ticks the task pass runs only its three drawing tasks (the actors,\r\n"
  ";   the zone, the name plates; 9 Nov: every draw of the pass is theirs): the world drawn again as it is, nothing\r\n"
  ";   moved; stage 2: the characters' positions and facing and the camera drawn at the blend of their last two ticks.\r\n"
  ";   Hooks CMoTask::Dispatch at the start; then live\r\n"
  "liminality=0\r\n"
  "; under Liminality, the characters' animations too: each motion's frame and fade weight drawn at the blend of its last\r\n"
  ";   two ticks (the game advances them in OnMove, the drawing only samples them; docs/RENDERER.md 3d); live\r\n"
  "lim_limbs=1\r\n"
  "; the occlusion readback answered from each query's previous probe: its picture is copied to a lockable render\r\n"
  ";   target of the query's own and read at the next probe, so the lock need not wait for the GPU (sf_occlusion.cpp).\r\n"
  ";   The answer is one probe late: an actor coming into view is drawn from its next probe. 1 = on; 2 = measure only\r\n"
  ";   (the game's answer, and how often the late one differs, in the log). Whether the late lock waits depends on\r\n"
  ";   the device: with d3d8on11 it need not (its log: 'readback snapshots'); on others it is not known. Live\r\n"
  "occlusion_late=0\r\n"
  "; with occlusion_late=1: an object answered visible is hidden only when two of its answers in a row say hidden (the\r\n"
  ";   late answer flips visible to hidden on 3-4% of probes, 9 Nov: characters popping out and back in). A chosen\r\n"
  ";   rule, not the game's: a hidden object is drawn one probe longer. 1 = on (the default); 0 = off. Live\r\n"
  "occlusion_hold=1\r\n"
  "; with occlusion_late=1 on d3d8on11: the late read never waits for d3d8on11's thread (it takes the newest finished\r\n"
  "; picture, or the object's last answer again); 0 = wait for the previous probe's picture, as before 10 Nov\r\n"
  "occlusion_nowait=1\r\n"
  "; with occlusion_late=1: an object's first probe (no picture of its own yet) is answered by the game's own readback,\r\n"
  "; which waits for the GPU (0, the default). 2 = answer \"visible\" that once instead, 1 = \"hidden\": no wait, but a\r\n"
  "; chosen answer, not the game's (an effect whose brightness is the count is at full for one probe with 2)\r\n"
  "occlusion_first=0\r\n"
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
  c.merge_scenes = rd("merge_scenes", 0); c.skin_merge = rd("skin_merge", 0); c.gpu_skin = rd("gpu_skin", 0) ? 1 : 0; c.task_draws = rd("task_draws", 0); c.liminality = rd("liminality", 0) ? 1 : 0; c.lim_limbs = rd("lim_limbs", 1) ? 1 : 0; c.motion_stats = rd("motion_stats", 0); c.stats_every = rd("stats_every", 60);
  c.precise_pacing = rd("precise_pacing", 0);
  c.uncap = rd("uncap", 0) ? 1 : 0; c.frame_step = rd("frame_step", 0); if (c.frame_step < 0 || c.frame_step > 3) c.frame_step = 0;
  c.null_draws = rd("null_draws", 0);
  c.shadow_fit = rd("shadow_fit", 0) ? 1 : 0; c.shadow_fit_scale = rd("shadow_fit_scale", 46);
  if (c.shadow_fit_scale < 5 || c.shadow_fit_scale > 300) c.shadow_fit_scale = 46;
  c.occlusion_hold = rd("occlusion_hold", 1); c.occlusion_nowait = rd("occlusion_nowait", 1); c.occlusion_first = rd("occlusion_first", 0); if (c.occlusion_first < 0 || c.occlusion_first > 2) c.occlusion_first = 0; c.occlusion_late = rd("occlusion_late", 0); if (c.occlusion_late < 0 || c.occlusion_late > 2) c.occlusion_late = 0;
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
      "skip_all_occlusion_readbacks=%d texture_compression=%d(%d) cpu_tier=%d lazy_state=%d block_cache=%d vs_dedupe=%d sse_skinning=%d merge_scenes=%d skin_merge=%d motion_stats=%d precise_pacing=%d null_draws=%d occlusion_late=%d uncap=%d frame_step=%d liminality=%d lim_limbs=%d", why, c.state_cache, c.vs_cache, c.ext_cache,
      c.scene_reset, c.batch_up, c.batch_max_prims, c.batch_max_bytes, c.frame_cap_fps, c.occlusion_stagger, c.skip_actor_occlusion,
      c.skip_all_occlusion_readbacks, c.texture_compression, c.texture_compression_value, c.cpu_tier, c.lazy_state, c.block_cache, c.vs_dedupe, c.sse_skinning, c.merge_scenes, c.skin_merge, c.motion_stats, c.precise_pacing, c.null_draws, c.occlusion_late, c.uncap, c.frame_step, c.liminality, c.lim_limbs);
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
// Liminality stage 1 (sf_fork.cpp sf_task_skip): the three drawing tasks' vtables (9 Nov, task_draws: CXiActorDraw
// 0x10329C44, CTsZoneMap 0x10336748, CXiActorNameDraw 0x10329C18), checked live: their +0x20 must be the game's
// 0x100817E0, 0x10178B30, 0x100817F0 (the same in 6A995428 and 6AC37846), else it stays off
extern "C" { uintptr_t sf_drawvt[3]; volatile int sf_lim_on; extern void *sf_dispatch_prev; __declspec(dllexport) volatile int sf_lim_test_zero;
             __declspec(dllexport) volatile float sf_lim_test_alpha = -1.0f; __declspec(dllexport) volatile int sf_lim_test_noot; volatile long sf_lim_skipped; extern double xi_step_carry; }
static uintptr_t g_onmove;                        // CXiSkeletonActor_OnMove (0x100C63D0), live: the actors' +0x20
static uintptr_t g_fxprep;                        // CMoElem_PrepareDraw (0x10045A40), live: the effect elements' +0x20
extern "C" { volatile long sf_lim_fx; extern volatile LONG sf_pass_draws_zero, sf_pass_draws_tick; }
// Liminality, per actor (an actor is its own task: CMoTask_Dispatch's argument, CXiSkeletonActor vtable +0x20 = OnMove).
// Confirmed from the game's code (docs/RENDERER.md 3d, 10 Nov): OnMove's start clears +0xB2 bit 0 (0x100C641C) and keeps
// only bit 0x10 of +0x6A4 (0x100C640E); the drawing path sets +0xB2 bit 0 and +0xB0 = 1 when the occlusion probe counts
// 0 (0x100CC69B..0x100CC6AE) and resets +0x8E8 after drawing (0x100CBD7B), which OnMove sets to 1 (0x100C7F4C); a frame
// whose OnMove does not run therefore draws with what the last drawing left (an occluded actor stays hidden: the flashing
// of the 10 Nov run). So right after each OnMove on a tick frame the four fields are kept, and on a skipped frame put
// back: the drawing sees what it saw at the tick. The position +0x5FC (GetPos, vtable +0x1BC) and direction +0x61C
// (GetDir, +0x1C0) the world matrix is built from (0x100CBDE0) are kept for the last two ticks; every frame the actor is
// drawn at their blend by the carry (the fraction of a tick since the last whole one), and the tick's values are put back
// before OnMove runs again. A move of 50 or more in a tick (a warp) is not blended.
struct LimActor { void *a; uint32_t tick; uint8_t b0, b2; uint32_t f6a4; uint8_t f8e8; float pc[3], pp[3], dc[3], dp[3], span; int nm; bool blended, hasprev, ot; };
enum { LIM_N = 2048 };
static LimActor g_lim[LIM_N]; static uint32_t g_lim_tick = 1; static int g_lim_full;
static uint32_t g_cam_frame = 1, g_pass_onmove; static float g_pass_span = 1.0f;   // (lim_cam)
static LimActor *lim_find(void *a, bool make) {
  unsigned h = (unsigned)(((uintptr_t)a >> 3) * 2654435761u) % LIM_N;
  for (int i = 0; i < LIM_N; i++) {
    LimActor *e = &g_lim[(h + i) % LIM_N];
    if (e->a == a) return e;
    if (!e->a) { if (!make) return NULL; memset(e, 0, sizeof *e); e->a = a; return e; }
  }
  if (make) g_lim_full++;
  return NULL;
}
// the blend's weight. What is drawn is the state one tick before now: now is the last tick k plus the carry c (the
// fraction of a tick since it, Frame_Render's fork); the two kept states are at k - span and k (span = the ticks the
// last OnMove covered: 2 below 60 fps on some frames). One tick before now is k - 1 + c, so the weight is
// (span - 1 + c) / span: c at span 1, (1 + c) / 2 at span 2. (12 Nov: c / span made the lag one tick on frames of 1
// tick and two on frames of 2: what is drawn jumped back and forth below 60 fps.)
static inline float lim_alpha(float span) {
  float t = sf_lim_test_alpha; if (t >= 0.0f) return t;
  double sp = span >= 1.0f ? span : 1.0f, c = (sp - 1.0 + xi_step_carry) / sp; return c < 0 ? 0.0f : c > 1 ? 1.0f : (float)c;
}
extern "C" __declspec(dllexport) float __cdecl sf_lim_alpha_test(float span, double carry) {   // (the host test)
  double k = xi_step_carry; float ta = sf_lim_test_alpha; xi_step_carry = carry; sf_lim_test_alpha = -1.0f;
  float r = lim_alpha(span); xi_step_carry = k; sf_lim_test_alpha = ta; return r;
}
static inline float wrap_pi(float d) { while (d > 3.14159265f) d -= 6.28318531f; while (d < -3.14159265f) d += 6.28318531f; return d; }
// the blend written into the actor, for a drawing window only (lim_window_*): only where the actor still holds its
// tick's values; what was written is kept so that the restore puts the tick's back only where it is still there
static bool lim_blend(LimActor *e, uint8_t *p, float wpos[3], float wdir[3]) {
  if (!e->hasprev) return false;
  float *pos = (float *)(p + 0x5FC), *dir = (float *)(p + 0x61C);
  if (memcmp(pos, e->pc, 12) || memcmp(dir, e->dc, 12)) return false;
  float t = lim_alpha(e->span);
  for (int i = 0; i < 3; i++) { pos[i] = e->pp[i] + (e->pc[i] - e->pp[i]) * t; dir[i] = e->dp[i] + wrap_pi(e->dc[i] - e->dp[i]) * t; }
  memcpy(wpos, pos, 12); memcpy(wdir, dir, 12);
  return true;
}
// The ordering table ([0x1047BC94]: insert 0x100553C0 thiscall (node), `ret 4`; a node is depth +0, next +4, callback
// +8, its argument +0xC, previous +0x10, inserted count +0x14) is drawn and cleared every frame by CMoOt_Update
// (0x10055620), a function task of the pass that is never skipped. Two kinds of skipped task insert a node of their
// own every frame, so on a frame of 0 ticks speedfix inserts it again, as at the tick, with the game's insert:
//  - CXiSkeletonActor_OnMove (0x100C7E9D): an actor whose alpha [+0x59C] x [+0x664] is below 1, or that vtable +0x70 or
//    the object at +0x710 sends there (0x100C7DE0..0x100C7E2E), is inserted at +0x8C8, callback 0x100CBD90 (which
//    draws it: PerActor_FourBucketStaggeredUpdate), +0x8D4 = the actor; the draw list then skips it
//    (RunStaggeredUpdates 0x100CBD60: +0x8D4 set and +0x8E8 not 1). Otherwise +0x8D4 = 0 (0x100C7F42).
//  - CYyAfterImage's +0x20 (0x1001BC80, at 0x1001C24B): its node at +0x1530, callback 0x1001C930, argument the task.
// Whether the tick inserted it: right after the task's +0x20, its node has that callback and argument and an inserted
// count above 0.
struct OtKind { uint32_t fn20, node, cb; };
static const OtKind kOt[2] = {{0x100C63D0u, 0x8C8u, 0x100CBD90u}, {0x1001BC80u, 0x1530u, 0x1001C930u}};
extern "C" { volatile long sf_lim_ot; }
typedef void (__attribute__((thiscall)) *OtInsertFn)(void *, void *);
static int ot_kind(const uintptr_t *vt) { for (int i = 0; i < 2; i++) if (vt[8] == (uintptr_t)XI_GAME(kOt[i].fn20)) return i; return -1; }
static bool ot_inserted(uint8_t *task, int k) {
  uint8_t *n = task + kOt[k].node;
  return *(const uintptr_t *)(n + 8) == (uintptr_t)XI_GAME(kOt[k].cb) && *(uint8_t *const *)(n + 0xC) == task && *(const int32_t *)(n + 0x14) > 0;
}
static void ot_again(uint8_t *task, int k) {
  void *ot = *(void *const *)(uintptr_t)XI_GAME(0x1047BC94u);
  if (!ot) return;
  ((OtInsertFn)XI_GAME(0x100553C0u))(ot, task + kOt[k].node); sf_lim_ot++;
}
static inline bool lim_zero(void) { int t = sf_lim_test_zero; return t ? t == 1 : xi_step_whole == 0.0f; }   // (the host test: 1 forces a frame of 0 ticks, 2 a tick)
static inline bool is_actor(const uintptr_t *vt) { return g_onmove && vt[8] == g_onmove; }
// Liminality, the limbs (docs/RENDERER.md 3d, 11 Nov). The game advances each motion's frame in OnMove: OnMove calls
// CYyModel_ForEachPart130 (0x1002C2A0, at 0x100C7FA7) on the model at +0x674; its parts are the list at model +0x44
// (0x1002B5A0: lea eax, [ecx+0x44]), linked at +4; each part's motion lists at +0x24 go to MotionQueList_ProcessAll
// (0x1001A3E0, by the jmp at 0x1002A133): 7 queues at +0x78, +0x64 ... +0x00, each a list at queue +4 linked at
// element +4. CYyMotionQue_Update (0x1001B2E0) steps each element's fade (0x1001B5D0: in state 3 (+8) the weight
// +0x10 += step * [+0x18]) and KzMotionQueList_NotifyAll (0x1001B1F0) calls each element's vtable +0x34, for
// CYyMotionQue (vtable 0x1032A1C0) CYyMotionQue_ComputeFrameAdvance (0x1001A8E0): with the handle +0x34 valid
// (0x10071060) and the loop count +0x2C not negative, frame +0x24 += step * res[+0x36] * 0.5 * speed +0x28 (res =
// *handle, 0x10072FB0), or += 1 when the duration is 2 and res[+0x36] * 0.5 is 0.5 (0x1001A96B..0x1001A9A9); the
// duration is +0x38 if above 0, else the word res[+0x34]; looping, it wraps by the duration (0x1001AA55..0x1001AB41).
// The drawing only samples the pose at that frame (Model_AnimateAndPose -> 0x1001A420 -> 0x1001B230).
// So after each OnMove every CYyMotionQue element's frame and weight are kept; an element is blended only when what
// the game did to it in that OnMove is exactly that formula (once wrapped, or not), else it is drawn as the game has
// it. Blended just before CXiActorDraw's +0x20 for the actors of the draw list (g_pActorDrawListHead [0x1047D578],
// next at +0x54), from their live lists, and only where the element still holds the tick's values; put back right
// after it, where it still holds what was written. Nothing but the drawing ever sees a blended frame.
struct LimMot { uint8_t *el; void *h; float fc, fp, wc, wp, D; uint8_t stc, okf, okw, wrap; };
enum { LIMB_N = 24, LIMR_N = 8192 };
static LimMot g_limm[LIM_N][LIMB_N];
struct LimRest { uint8_t *a, *el; float f, w, fw, ww; };
static LimRest g_limr[LIMR_N]; static int g_nlimr;
static uintptr_t g_motvt;                                  // CYyMotionQue's vtable, live (0 = the limbs off)
extern "C" { __declspec(dllexport) volatile int sf_lim_test_valid; __declspec(dllexport) volatile long sf_limb_blended, sf_limb_asis; }
static int lim_motions(uint8_t *a, uint8_t **out, int max) {
  int n = 0, parts = 0;
  uint8_t *part = *(uint8_t *const *)(a + 0x674 + 0x44);
  while (part && parts++ < 32) {
    uint8_t *list = part + 0x24;
    for (int q = 0; q < 7; q++) {
      uint8_t *el = *(uint8_t *const *)(list + 0x78 - 0x14 * q + 4); int k = 0;
      while (el && k++ < 64) { if (*(const uintptr_t *)el == g_motvt && n < max) out[n++] = el; el = *(uint8_t *const *)(el + 4); }
    }
    part = *(uint8_t *const *)(part + 4);
  }
  return n;
}
typedef char (__attribute__((thiscall)) *HandleOkFn)(void *);
// the duration and what one OnMove of `step` ticks adds to the frame, as ComputeFrameAdvance computes them
static bool lim_mot_rate(uint8_t *el, float step, float *D, float *adv) {
  void *h = *(void *const *)(el + 0x34);
  if (!h) return false;
  if (!sf_lim_test_valid && !((HandleOkFn)XI_GAME(0x10071060u))(el + 0x34)) return false;
  const uint8_t *res = *(const uint8_t *const *)h;
  if (!res) return false;
  float d = (float)*(const int16_t *)(res + 0x34), e38 = *(const float *)(el + 0x38);
  if (e38 > 0.0f) d = e38;
  float r = *(const float *)(res + 0x36) * 0.5f;
  if (*(const int32_t *)(el + 0x2C) < 0) *adv = 0.0f;
  else if (d == 2.0f && r == 0.5f) *adv = 1.0f;
  else *adv = step * (r * *(const float *)(el + 0x28));
  *D = d; return true;
}
static inline bool near_eq(float a, float b) { return fabsf(a - b) <= 1e-3f * (fabsf(b) + 1.0f); }
static void lim_limbs_snap(LimActor *e, uint8_t *a) {           // after OnMove
  if (!g_motvt) { e->nm = 0; return; }
  LimMot *old = g_limm[e - g_lim], nw[LIMB_N]; uint8_t *els[LIMB_N];
  int n = lim_motions(a, els, LIMB_N), no = e->nm; float step = e->span;
  for (int i = 0; i < n; i++) {
    uint8_t *el = els[i]; LimMot m; memset(&m, 0, sizeof m);
    m.el = el; m.h = *(void *const *)(el + 0x34); m.fc = m.fp = *(const float *)(el + 0x24); m.wc = m.wp = *(const float *)(el + 0x10); m.stc = el[8];
    const LimMot *o = NULL;
    for (int j = 0; j < no; j++) if (old[j].el == el && old[j].h == m.h) { o = &old[j]; break; }
    float adv;
    if (o && e->hasprev && lim_mot_rate(el, step, &m.D, &adv)) {
      float d = m.fc - o->fc;
      if (near_eq(d, adv)) { m.okf = 1; m.wrap = 0; }
      else if (m.D > 0.0f && near_eq(d + m.D, adv)) { m.okf = 1; m.wrap = 1; }
      else if (m.D > 0.0f && near_eq(d - m.D, adv)) { m.okf = 1; m.wrap = 2; }
      if (m.okf) m.fp = o->fc;
      if (o->stc == 3 && m.stc == 3 && near_eq(m.wc, o->wc + step * *(const float *)(el + 0x18))) { m.okw = 1; m.wp = o->wc; }
    }
    nw[i] = m;
  }
  memcpy(old, nw, n * sizeof nw[0]); e->nm = n;
}
// The drawing windows (12 Nov): the blend is written only while the game draws -- CXiActorDraw's +0x20 and
// CXiActorNameDraw's (sf_dispatch), and CMoOt_Update (the ordering table: its task node's function swapped for
// sf_ot_wrap) -- for the actors of the draw list (g_pActorDrawListHead [0x1047D578], next at +0x54), and put back right
// after. Between them the game's logic (the other tasks, on frames with ticks) sees only the ticks' own values.
struct LimPosRest { uint8_t *a; float wpos[3], wdir[3]; LimActor *e; };
static LimPosRest g_limp[LIM_N]; static int g_nlimp; static bool g_win_open;
static void lim_window_end(void);
extern "C" { volatile long sf_lim_windows; }
static void lim_window_begin(void) {               // (the windows never nest in the game: one still open is closed first)
  if (g_win_open) lim_window_end();
  if (!sf_lim_on) return;
  g_win_open = true; g_nlimr = 0; g_nlimp = 0; sf_lim_windows++;
  bool limbs = g_motvt && sf_cfg.lim_limbs;
  uint8_t *a = *(uint8_t *const *)(uintptr_t)XI_GAME(0x1047D578u); int k = 0;
  for (; a && k++ < 4096; a = *(uint8_t *const *)(a + 0x54)) {
    if (!is_actor(*(const uintptr_t *const *)a)) continue;
    LimActor *e = lim_find(a, false); if (!e) continue;
    if (g_nlimp < LIM_N) { LimPosRest &r = g_limp[g_nlimp]; if (lim_blend(e, a, r.wpos, r.wdir)) { r.a = a; r.e = e; g_nlimp++; } }
    if (!limbs || !e->nm) continue;
    LimMot *ms = g_limm[e - g_lim]; uint8_t *els[LIMB_N]; int n = lim_motions(a, els, LIMB_N);
    float t = lim_alpha(e->span);
    for (int i = 0; i < n; i++) {
      uint8_t *el = els[i]; const LimMot *m = NULL;
      for (int j = 0; j < e->nm; j++) if (ms[j].el == el) { m = &ms[j]; break; }
      float *fr = (float *)(el + 0x24), *wt = (float *)(el + 0x10);
      if (!m || m->h != *(void *const *)(el + 0x34) || *fr != m->fc || *wt != m->wc || (!m->okf && !m->okw)) { sf_limb_asis++; continue; }
      if (g_nlimr == LIMR_N) return;
      LimRest &r = g_limr[g_nlimr++]; r.a = a; r.el = el; r.f = *fr; r.w = *wt;
      if (m->okf) {
        float cu = m->fc + (m->wrap == 1 ? m->D : m->wrap == 2 ? -m->D : 0.0f), f = m->fp + (cu - m->fp) * t;
        if (m->wrap == 1 && f >= m->D) f -= m->D; else if (m->wrap == 2 && f <= 0.0f) f += m->D;
        *fr = f;
      }
      if (m->okw) *wt = m->wp + (m->wc - m->wp) * t;
      r.fw = *fr; r.ww = *wt; sf_limb_blended++;
    }
  }
}
static void lim_window_end(void) {
  if (!g_win_open) return;
  g_win_open = false;
  for (int i = 0; i < g_nlimp; i++) {
    LimPosRest &r = g_limp[i]; float *pos = (float *)(r.a + 0x5FC), *dir = (float *)(r.a + 0x61C);
    if (!memcmp(pos, r.wpos, 12)) memcpy(pos, r.e->pc, 12);
    if (!memcmp(dir, r.wdir, 12)) memcpy(dir, r.e->dc, 12);
  }
  g_nlimp = 0;
  uint8_t *els[LIMB_N]; int n = 0; uint8_t *cur = NULL;
  for (int i = 0; i < g_nlimr; i++) {
    LimRest &r = g_limr[i];
    if (r.a != cur) { cur = r.a; n = lim_motions(cur, els, LIMB_N); }
    bool live = false; for (int j = 0; j < n; j++) if (els[j] == r.el) { live = true; break; }
    if (!live) continue;
    float *fr = (float *)(r.el + 0x24), *wt = (float *)(r.el + 0x10);
    if (*fr == r.fw) *fr = r.f;
    if (*wt == r.ww) *wt = r.w;
  }
  g_nlimr = 0;
}
// before a task's +0x20 (sf_fork.cpp sf_dispatch): 1 = skip it this frame
extern "C" __declspec(dllexport) int __cdecl sf_task_skip(void *task) {
  if (!sf_lim_on) return 0;
  const uintptr_t *vt = *(const uintptr_t *const *)task; uintptr_t v = (uintptr_t)vt;
  bool draw = v == sf_drawvt[0] || v == sf_drawvt[1] || v == sf_drawvt[2];
  if (draw) { if (v == sf_drawvt[0] || v == sf_drawvt[2]) lim_window_begin(); return 0; }
  // the effect elements (17 classes, vtable +0x20 CMoElem_PrepareDraw 0x10045A40): each frame they put themselves in
  // the ordering table at [0x1047BC94] (CMoElem_CheckAt138ThenCall 0x100458F0 -> 0x100555B0 -> 0x100553C0), which
  // CMoOt_Update (0x10055620, a function task of the pass, not a CMoTask) draws and clears (0x10055520, 0x100554B0)
  // every frame: skipped, they are not drawn on that frame (the flicker of light-emitting objects, 12 Nov). Their clock
  // is [g_pResourceMng+0xEB0], the frame step (written by 0x10069F90 at 0x1006A005, from Camera_ApplyViewProjection,
  // after this frame's step is made: the pass reads the step it runs with), 0 on a frame of 0 ticks: they run, hold
  // still and are drawn again
  if (g_fxprep && vt[8] == g_fxprep) { sf_lim_fx++; return 0; }
  uint8_t *p = (uint8_t *)task;
  if (is_actor(vt)) {
    LimActor *e = lim_find(task, false);
    if (!lim_zero()) return 0;
    if (e && e->tick) {                                  // the drawing's handshakes as OnMove left them at the tick
      p[0xB0] = e->b0; p[0xB2] = (uint8_t)((p[0xB2] & ~1) | (e->b2 & 1)); *(uint32_t *)(p + 0x6A4) = e->f6a4; p[0x8E8] = e->f8e8;
      if (e->ot && e->tick == g_lim_tick && !sf_lim_test_noot) ot_again(p, 0);      // in the ordering table again, as OnMove put it at the tick
    }
    sf_lim_skipped++; return 1;
  }
  if (!lim_zero()) return 0;
  { int k = ot_kind(vt); if (k > 0) { LimActor *e = lim_find(task, false); if (e && e->ot && e->tick == g_lim_tick && !sf_lim_test_noot) ot_again(p, k); } }
  sf_lim_skipped++; return 1;
}
// after a task's +0x20 on a frame it ran: an actor's state as OnMove left it, its last two ticks' position and direction
extern "C" __declspec(dllexport) void __cdecl sf_task_post(void *task) {
  if (!sf_lim_on) return;
  const uintptr_t *vt = *(const uintptr_t *const *)task;
  if ((uintptr_t)vt == sf_drawvt[0] || (uintptr_t)vt == sf_drawvt[2]) { lim_window_end(); return; }
  if (!is_actor(vt)) {
    int k = ot_kind(vt);
    if (k > 0) { LimActor *e = lim_find(task, true); if (e) { e->ot = ot_inserted((uint8_t *)task, k); e->tick = g_lim_tick; } }
    return;
  }
  uint8_t *p = (uint8_t *)task; LimActor *e = lim_find(task, true); if (!e) return;
  e->b0 = p[0xB0]; e->b2 = p[0xB2]; e->f6a4 = *(uint32_t *)(p + 0x6A4); e->f8e8 = p[0x8E8];
  const float *pos = (const float *)(p + 0x5FC), *dir = (const float *)(p + 0x61C);
  float dx = pos[0] - e->pc[0], dy = pos[1] - e->pc[1], dz = pos[2] - e->pc[2];
  bool warp = !e->tick || dx * dx + dy * dy + dz * dz >= 2500.0f;
  if (warp) { memcpy(e->pp, pos, 12); memcpy(e->dp, dir, 12); } else { memcpy(e->pp, e->pc, 12); memcpy(e->dp, e->dc, 12); }
  memcpy(e->pc, pos, 12); memcpy(e->dc, dir, 12); e->hasprev = true; e->tick = g_lim_tick;
  e->span = sf_lim_test_zero ? 1.0f : xi_step_whole;    // the ticks this OnMove covered
  e->ot = *(uint8_t *const *)(p + 0x8D4) == p && ot_inserted(p, 0);
  lim_limbs_snap(e, p);                                  // its motions' frames and weights at this tick
  g_pass_onmove = g_cam_frame; g_pass_span = e->span;    // (the camera: an OnMove ran in this frame's pass)
}
// the camera (Frame_Render's fork, around Camera_ApplyViewProjection; docs/RENDERER.md 3d): the view is built from the
// camera at [g_pCamera+0x50], eye +0x44, target +0x50 (FUN_10021B70 0x10021B70); its +0x88 / +0x89 are snap requests
// (Camera_UpdateController 0x10021900 sets +0x44 / +0x50 from +0x8C / +0x98): with one pending nothing is blended
// A tick for the camera is a frame whose task pass ran OnMove (the player's moves it: YmCameraManager_Follow from
// CXiSkeletonActor_OnMove, 0x100C81F0), or one on which the camera is not where its restore left it (moved by other
// code): never decided from the step, which Frame_Render's fork has already made for the next frame by now (12 Nov:
// that made the restore write an old camera back, the camera stuck). Its span is that pass's ticks.
static float g_cc[6], g_cp[6], g_c_span = 1.0f; static bool g_c_has, g_c_blended; static uint8_t *g_c_obj;
static void lim_cam(int after) {
  if (!sf_lim_on) return;
  uint8_t *app = *(uint8_t *volatile *)(uintptr_t)XI_GAME(0x104568FCu);
  uint8_t *cam = app ? *(uint8_t **)(app + 0x50) : NULL;
  if (!after) {
    g_c_blended = false;
    if (!cam || cam[0x88] || cam[0x89]) { g_c_has = false; return; }
    float *eye = (float *)(cam + 0x44), *tgt = (float *)(cam + 0x50);
    bool moved = g_c_has && (memcmp(eye, g_cc, 12) || memcmp(tgt, g_cc + 3, 12));
    bool tick = sf_lim_test_zero ? sf_lim_test_zero == 2 : g_pass_onmove == g_cam_frame;
    if (tick || moved || !g_c_has || cam != g_c_obj) {   // a tick: the camera's values are the tick's
      if (g_c_has && cam == g_c_obj) memcpy(g_cp, g_cc, sizeof g_cc); else { memcpy(g_cp, eye, 12); memcpy(g_cp + 3, tgt, 12); }
      memcpy(g_cc, eye, 12); memcpy(g_cc + 3, tgt, 12); g_c_has = true; g_c_obj = cam; g_c_span = sf_lim_test_zero || !tick ? 1.0f : g_pass_span;
      float dx = g_cc[0] - g_cp[0], dy = g_cc[1] - g_cp[1], dz = g_cc[2] - g_cp[2];
      if (dx * dx + dy * dy + dz * dz >= 2500.0f) memcpy(g_cp, g_cc, sizeof g_cc);
    }
    float t = lim_alpha(g_c_span);
    for (int i = 0; i < 3; i++) { eye[i] = g_cp[i] + (g_cc[i] - g_cp[i]) * t; tgt[i] = g_cp[3 + i] + (g_cc[3 + i] - g_cp[3 + i]) * t; }
    g_c_blended = true;
  } else {
    if (g_c_blended && cam == g_c_obj) { memcpy(cam + 0x44, g_cc, 12); memcpy(cam + 0x50, g_cc + 3, 12); }   // the tick's values back
    g_c_blended = false; g_cam_frame++;
  }
}
extern "C" { __declspec(dllexport) void (*volatile xi_lim_cam)(int) = lim_cam; }
// Step reads (task_draws, a measurement; and Liminality's one use of it): every answer of the getters' forks is told here
// with its return address and whether it is one of frame_step_users.py's fraction sites (sf_step_sites.h: every path
// of the value ends in an accumulator, `x += f(step)`, in every call site of its function). Counted by where the call
// ran: inside one of the three drawing tasks, inside another task, outside the task pass (xi_task_vt, sf_fork.cpp).
// Liminality: the drawing tasks run every frame, the others only on frames with a tick, so a fraction site reached only
// from a drawing task can be answered with the frame's own fraction (xi_step_frac, the frame's real time in ticks): its
// accumulator then adds up real time frame by frame instead of whole ticks on tick frames. A fraction site that is ever
// reached outside the drawing tasks gets whole ticks from then on (the same function must not add up two clocks); the
// log names it.
extern "C" { extern volatile void *xi_task_vt; extern volatile int xi_task_slot; }
struct StepRead { uint32_t ret; LONG n[3]; uint8_t fsite, mixed, frac; };
enum { SR_N = 512 };
static StepRead g_sr[SR_N]; static LONG g_sr_full; static uintptr_t g_sr_vt[3];
extern "C" { __declspec(dllexport) volatile long sf_sr_frac_answers, sf_sr_calls; }
static int step_ctx(uintptr_t ret, int fsite) {
  if (!sf_cfg.task_draws && !sf_lim_on) return 0;
  sf_sr_calls++;
  if (!g_sr_vt[0]) { static const uint32_t kVt[3] = {0x10329C44u, 0x10336748u, 0x10329C18u}; for (int i = 0; i < 3; i++) g_sr_vt[i] = (uintptr_t)XI_GAME(kVt[i]); }
  uintptr_t v = (uintptr_t)xi_task_vt;
  int ctx = !v ? 2 : v == g_sr_vt[0] || v == g_sr_vt[1] || v == g_sr_vt[2] ? 0 : 1;   // 0 drawing task, 1 other task, 2 outside
  unsigned h = (unsigned)((ret >> 1) * 2654435761u) % SR_N; StepRead *e = NULL;
  for (int i = 0; i < SR_N; i++) {
    StepRead *r = &g_sr[(h + i) % SR_N];
    if (r->ret == (uint32_t)ret) { e = r; break; }
    if (!r->ret) { r->ret = (uint32_t)ret; r->fsite = (uint8_t)fsite; e = r; break; }
  }
  if (!e) { g_sr_full++; return 0; }
  e->n[ctx]++;
  if (!fsite) return 0;
  if (ctx != 0 && !e->mixed) { e->mixed = 1; LOG("speedfix: step reads: the fraction site returning to %08X runs outside the drawing tasks too: whole ticks there from now on", (unsigned)(ret - (uintptr_t)GetModuleHandleA("FFXiMain.dll") + 0x10000000u)); }
  if (!sf_lim_on || ctx != 0 || e->mixed) return 0;
  e->frac = 1; sf_sr_frac_answers++;
  return 1;
}
extern "C" { __declspec(dllexport) int (*volatile xi_step_ctx)(uintptr_t, int) = step_ctx; }
static void step_read_stats(LONG frames) {
  if (!sf_cfg.task_draws) return;
  StepRead top[32] = {}; uintptr_t base = (uintptr_t)GetModuleHandleA("FFXiMain.dll");
  for (int i = 0; i < SR_N; i++) {
    StepRead r = g_sr[i]; if (!r.ret) continue; LONG t = r.n[0] + r.n[1] + r.n[2];
    for (int j = 0; j < 32; j++) { LONG u = top[j].n[0] + top[j].n[1] + top[j].n[2]; if (!top[j].ret || t > u) { memmove(top + j + 1, top + j, (31 - j) * sizeof top[0]); top[j] = r; break; } }
    g_sr[i].n[0] = g_sr[i].n[1] = g_sr[i].n[2] = 0;
  }
  char line[2600]; int o = snprintf(line, sizeof line, "speedfix: step reads, %ld frames: by return address (analysed), calls a frame in the drawing tasks / other tasks / "
                                      "outside the task pass, f = a fraction site, F = answered with the fraction:", (long)frames);
  for (int j = 0; j < 32 && top[j].ret && o < (int)sizeof line - 60; j++)
    o += snprintf(line + o, sizeof line - o, " %08X%s %.1f/%.1f/%.1f;", (unsigned)(top[j].ret - base + 0x10000000u), top[j].frac ? "F" : top[j].fsite ? "f" : "",
                  frames ? (double)top[j].n[0] / frames : 0.0, frames ? (double)top[j].n[1] / frames : 0.0, frames ? (double)top[j].n[2] / frames : 0.0);
  if (g_sr_full) o += snprintf(line + o, sizeof line - o, " (%ld calls not tabled)", (long)g_sr_full);
  LOG("%s", line); g_sr_full = 0;
}
// CMoOt_Update (0x10055620) is a function task: the task pass calls [node+4](node) for each node of CMoTaskMng
// ([0x1047D140]; list at +0xD50, next at +0xC; 0x10074C30), cdecl, the argument unused. Liminality points that node's
// function at sf_ot_wrap (a data change, put back when it goes off), which runs it inside a drawing window: the
// ordering table draws the see-through actors (0x100CBD90).
extern "C" void __cdecl sf_ot_wrap(void *node) {
  lim_window_begin(); ((void (__cdecl *)(void *))XI_GAME(0x10055620u))(node); lim_window_end();
}
extern "C" { __declspec(dllexport) volatile long sf_ot_wrapped; }
static void ot_swap(bool on) {
  uint8_t *mng = *(uint8_t *const *)(uintptr_t)XI_GAME(0x1047D140u); if (!mng) return;
  uintptr_t game = (uintptr_t)XI_GAME(0x10055620u), ours = (uintptr_t)sf_ot_wrap; int k = 0;
  for (uint8_t *n = *(uint8_t *const *)(mng + 0xD50); n && k++ < 100000; n = *(uint8_t *const *)(n + 0xC)) {
    uintptr_t *fn = (uintptr_t *)(n + 4);
    if (on && *fn == game) { *fn = ours; sf_ot_wrapped++; } else if (!on && *fn == ours) *fn = game;
  }
}
static void sf_lim_set(int on) {
  static int ok = -1;
  static const uint32_t kVt[3] = {0x10329C44u, 0x10336748u, 0x10329C18u}, kFn[3] = {0x100817E0u, 0x10178B30u, 0x100817F0u};
  if (on && ok < 0) {
    ok = 1;
    for (int i = 0; i < 3; i++) {
      uintptr_t v = (uintptr_t)XI_GAME(kVt[i]); sf_drawvt[i] = v;
      if (IsBadReadPtr((void *)v, 0x28) || ((uintptr_t *)v)[8] != (uintptr_t)XI_GAME(kFn[i])) ok = 0;
    }
    g_onmove = (uintptr_t)XI_GAME(0x100C63D0u);
    if (((uintptr_t *)XI_GAME(0x1032D710u))[8] != g_onmove) ok = 0;   // CXiSkeletonActor's vtable: +0x20 is OnMove
    g_fxprep = (uintptr_t)XI_GAME(0x10045A40u);                       // CMoElem's vtable 0x1032B... +0x20 (checked below)
    { static const uint32_t kFxVt[] = {0x1032B758u}; for (unsigned i = 0; i < 1; i++) if (((uintptr_t *)XI_GAME(kFxVt[i]))[8] != g_fxprep) { g_fxprep = 0; LOG("speedfix: liminality: CMoPointLightProgElem's +0x20 is not CMoElem_PrepareDraw (another build?): effects skipped between ticks"); } }
    g_motvt = (uintptr_t)XI_GAME(0x1032A1C0u);                        // CYyMotionQue's: +0x34 ComputeFrameAdvance
    if (((uintptr_t *)g_motvt)[13] != (uintptr_t)XI_GAME(0x1001A8E0u)) { g_motvt = 0; LOG("speedfix: liminality: CYyMotionQue's vtable +0x34 is not ComputeFrameAdvance (another build?): the limbs are not blended"); }
    LOG(ok ? "speedfix: liminality: the three drawing tasks' vtables and CXiSkeletonActor's OnMove are the game's: on" :
             "speedfix: liminality: a drawing task's vtable is not the expected one (another build?): off");
  }
  bool was = sf_lim_on;
  sf_lim_on = on && ok == 1 && sf_dispatch_prev;
  { static int every; if (sf_lim_on && (!was || ++every >= 30)) { every = 0; ot_swap(true); } else if (was && !sf_lim_on) ot_swap(false); }
  if (was && !sf_lim_on) {                               // off: every blended actor gets its tick's values back
    for (int i = 0; i < LIM_N; i++) if (g_lim[i].a && g_lim[i].blended) { /* the actor may be gone: not written */ g_lim[i].blended = false; }
    memset(g_lim, 0, sizeof g_lim); g_c_has = false;
  }
  if (sf_lim_on && !lim_zero()) g_lim_tick++;
}
static void apply_uncap_step(void) {
  bool live = sf_frame_no - sf_hooks_frame <= 1;
  xi_uncap = sf_framestep_installed && sf_cfg.uncap && live;
  xi_step_mode = sf_framestep_installed && live ? (sf_cfg.liminality ? 1 : sf_cfg.frame_step) : 0;
  sf_lim_set(sf_cfg.liminality && sf_framestep_installed && live && sf_cfg.uncap);
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
// frame_step=3: the chase loops (pipeline/frame_step_loops.py, sf_step_loops.h, docs/RENDERER.md 3c). The game runs
// each one once per whole tick: s = the getter's answer; while (s > 0) { x += (target - x) * k; s -= 1; } -- so after a
// frame of s ticks the distance left is (1 - k)^s. Under frame_step 1 a frame above 60 fps has s = 0 and the loop does
// not run (the model stops and does not turn that frame; its movement that frame is 0, which OnMove's 0.01 tests read
// as standing). Under 3 the getter answers these calls with the fraction s (so the loop runs n = ceil(s) passes) and
// the passes use k' = 1 - (1 - k)^(s / n), so the distance left after the frame is (1 - k)^s: what s ticks leave. With
// s whole (60 fps and below at whole ticks), k' is k itself: the game's code, bit for bit.
//   * a factor read from a constant of the image (`fld/fmul dword ptr [K]`): that instruction's operand is pointed at
//     a float of speedfix's (one per constant), written here each frame; the constant itself is not written (it is
//     read by other code too);
//   * a factor passed to 0x100272B0 (Vec3 scale, cdecl (float *v, float k)): that call is re-aimed at sf_scale_k, which
//     calls the game's function with k';
//   * OnMove's four compares of the frame's movement with 0.01 ([0x10329A18], 0x100C6BBC..0x100C7100): below one tick
//     a frame (s < 1) they compare with 0.01 s, the same movement per tick the game tests; at s >= 1 with 0.01.
#include "sf_step_loops.h"
#include <math.h>
static float g_loop_k[4]; static uint32_t g_loop_kc[4]; static float g_loop_k0[4]; static int g_nloop_k;   // per constant
static float g_move_thr = 0.01f, g_move_thr0 = 0.01f;
// the walk/run rate (0x100C8930, sf_step_loops.h kStepRate*): its weights and its standing test, the game's values first
static float g_rate[3], g_rate0[3];
static volatile float g_loop_s = 1.0f; static volatile int g_loop_on;     // this frame's s, and whether 3 is in effect
static bool g_loops_done; static LONG g_loop_frac_frames;
static const uint32_t kMoveThrAt[4] = {0x100C6BBCu, 0x100C6BE2u, 0x100C6C08u, 0x100C7100u};   // fcomp dword ptr [0x10329A18]
// the camera follow (sf_step_loops.h kStepFollow*, kStepTickDec, kStepHold*; frame_step_loops.py FOLLOW_*): the hold
// timer's per-pass amount and the counter's start, the game's values first
static float g_tick_dec = 1.0f, g_tick_dec0 = 1.0f; static int g_hold = 4, g_hold0 = 4; static float g_follow_far = 0.5f;
// f: the share of a tick one of this frame's ceil(s) passes stands for (1 at whole s: the game's code)
static inline double loop_f(void) {
  float s = g_loop_s;
  if (!g_loop_on || !(s > 0.0f)) return 1.0;
  float n = ceilf(s);
  return n == s ? 1.0 : (double)s / (double)n;
}
static inline float loop_k(float k) {
  double f = loop_f();
  if (f == 1.0) return k;                                // whole ticks: the game's own factor
  // k >= 1 (the follow's (d - 6) 0.012 with the camera 89 or more from the player, just after a warp): the game's pass
  // overshoots (1 - k <= 0 has no fractional power); a pass then moves f of it -- stated, not a closed form
  if (!(k < 1.0f)) return (float)(k * f);
  return (float)(1.0 - pow(1.0 - (double)k, f));
}
// the getter's forks call this with the fraction s a loop is about to count (before the loop runs), so the factors
// below are this s's (s changes once a frame, at Frame_Render's top; the factors are recomputed when it changes)
static void loop_set(float s) {
  if (g_loop_on && s == g_loop_s) return;
  g_loop_s = s; g_loop_on = 1;
  for (int j = 0; j < g_nloop_k; j++) g_loop_k[j] = loop_k(g_loop_k0[j]);
  g_move_thr = s > 0.0f && s < 1.0f ? g_move_thr0 * s : g_move_thr0;
  // once a frame the game does rate = new * 0.25 + old * 0.75 (one pass, whatever the frame's ticks): below one tick a
  // frame the weights a tick's pass gives over s of a tick, 1 - 0.75^s and 0.75^s; at s >= 1 the game's own
  if (s > 0.0f && s < 1.0f) {
    double keep = pow((double)g_rate0[RATE_OLD], (double)s);
    g_rate[RATE_OLD] = (float)keep; g_rate[RATE_NEW] = (float)(1.0 - keep); g_rate[RATE_THR] = g_rate0[RATE_THR] * s;
  } else for (int j = 0; j < 3; j++) g_rate[j] = g_rate0[j];
  double f = loop_f();                                   // the follow: a pass is f of a tick
  g_tick_dec = f == 1.0 ? g_tick_dec0 : (float)f;
  g_hold = f == 1.0 ? g_hold0 : (int)floor(g_hold0 / f + 0.5);   // the passes 4 ticks make, to the nearest pass
  if (ceilf(s) != s) g_loop_frac_frames++;
}
extern "C" { void (*volatile xi_step_loop_hook)(float) = loop_set; }
typedef void (__cdecl *ScaleFn)(float *, float);
extern "C" void __cdecl sf_scale_k(float *v, float k) { ((ScaleFn)XI_GAME(0x100272B0u))(v, loop_k(k)); }
typedef void (__cdecl *Scale3Fn)(float *, const float *, float);
extern "C" void __cdecl sf_scale3_k(float *d, const float *v, float k) { ((Scale3Fn)XI_GAME(0x100272E0u))(d, v, loop_k(k)); }
// the follow's one 0x100272B0 call (0x1001FD5A), reached two ways (frame_step_loops.py FOLLOW_FORMS): d > 100 with v
// normalised (0x1001FD17) and k = (d - 100) 0.5, the excess over 100 halving a pass -- f of a pass leaves 0.5^f of it, a
// move of (k / 0.5)(1 - 0.5^f); 6 <= d <= 100 with v = player - camera, |v| = d >= 6, and k = (d - 6) 0.012, the fraction
// of the distance a pass moves -- k'. Which way it came is |v|: 1 or at least 6.
extern "C" void __cdecl sf_follow_k(float *v, float k) {
  double f = loop_f();
  if (f != 1.0) {
    double l2 = (double)v[0] * v[0] + (double)v[1] * v[1] + (double)v[2] * v[2], h = g_follow_far;
    k = l2 < 4.0 ? (float)((k / h) * (1.0 - pow(1.0 - h, f))) : loop_k(k);
  }
  ((ScaleFn)XI_GAME(0x100272B0u))(v, k);
}
// in place of the follow's `mov dword ptr [0x10456D8C], 4` (0x1001FB19, then a jmp; eax ecx edx and the x87 stack are
// free there: the code after sets them first): the counter in passes
extern "C" void __cdecl sf_follow_hold(void) { *(volatile int *)(uintptr_t)XI_GAME(kStepHoldVar) = g_hold; }
// point the 4-byte absolute operand of the instruction at `at` (opcode op[0] op[1], operand XI_GAME(constant)) at `to`
static bool reaim_operand(uint32_t at, const uint8_t op[2], uint32_t constant, const float *to, const char *what) {
  uint8_t want[6] = {op[0], op[1]}, live[6]; uint32_t c = (uint32_t)XI_GAME(constant); memcpy(want + 2, &c, 4);
  if (!live_bytes(at, want, 6, live)) {
    LOG("speedfix: %s not changed: the bytes at %08X are not the expected %02X %02X [%08X]", what, at, op[0], op[1], c);
    return false;
  }
  uint8_t *p = (uint8_t *)(uintptr_t)XI_GAME(at); uint32_t a = (uint32_t)(uintptr_t)to; DWORD old;
  if (!VirtualProtect(p, 6, PAGE_EXECUTE_READWRITE, &old)) { LOG("speedfix: %s: VirtualProtect failed", what); return false; }
  memcpy(p + 2, &a, 4);
  VirtualProtect(p, 6, old, &old); FlushInstructionCache(GetCurrentProcess(), p, 6);
  return true;
}
static void apply_step_loops(void) {              // render thread, inside Present
  if (!g_loops_done && sf_cfg.frame_step == 3 && sf_framestep_installed) {
    g_loops_done = true; int ok = 0, all = 0;
    g_move_thr0 = g_move_thr = *(const float *)(uintptr_t)XI_GAME(0x10329A18u);
    for (unsigned i = 0; i < sizeof kStepLoopOperand / sizeof kStepLoopOperand[0]; i++) {
      const StepLoopOperand &o = kStepLoopOperand[i]; int j = 0;
      while (j < g_nloop_k && g_loop_kc[j] != o.constant) j++;
      if (j == g_nloop_k) { if (j == 4) continue; g_loop_kc[j] = o.constant; g_loop_k[j] = g_loop_k0[j] = *(const float *)(uintptr_t)XI_GAME(o.constant); g_nloop_k++; }
      all++; ok += reaim_operand(o.at, o.op, o.constant, &g_loop_k[j], "frame_step=3 (loop factor)");
    }
    for (unsigned i = 0; i < sizeof kStepLoopScaleCall / sizeof kStepLoopScaleCall[0] + sizeof kStepIntLoopScaleCall / sizeof kStepIntLoopScaleCall[0]; i++) {
      const unsigned nf = sizeof kStepLoopScaleCall / sizeof kStepLoopScaleCall[0];
      uint32_t at = i < nf ? kStepLoopScaleCall[i] : kStepIntLoopScaleCall[i - nf]; int32_t rel = (int32_t)(0x100272B0u - (at + 5)); uint8_t e[5] = {0xE8};
      memcpy(e + 1, &rel, 4); all++; ok += reaim_call(at, e, (void *)sf_scale_k, "frame_step=3 (loop scale)");
    }
    for (unsigned i = 0; i < sizeof kStepRateOperand / sizeof kStepRateOperand[0]; i++) {
      const StepRateOperand &o = kStepRateOperand[i];
      g_rate[o.kind] = g_rate0[o.kind] = *(const float *)(uintptr_t)XI_GAME(o.constant);
      all++; ok += reaim_operand(o.at, o.op, o.constant, &g_rate[o.kind], "frame_step=3 (walk/run rate)");
    }
    for (unsigned i = 0; i < sizeof kStepIntLoopOperand / sizeof kStepIntLoopOperand[0]; i++) {   // the follow's 0.125
      const StepLoopOperand &o = kStepIntLoopOperand[i]; int j = 0;
      while (j < g_nloop_k && g_loop_kc[j] != o.constant) j++;
      if (j == g_nloop_k) { if (j == 4) continue; g_loop_kc[j] = o.constant; g_loop_k[j] = g_loop_k0[j] = *(const float *)(uintptr_t)XI_GAME(o.constant); g_nloop_k++; }
      all++; ok += reaim_operand(o.at, o.op, o.constant, &g_loop_k[j], "frame_step=3 (camera follow 0.125)");
    }
    g_tick_dec = g_tick_dec0 = *(const float *)(uintptr_t)XI_GAME(kStepTickDec.constant);
    all++; ok += reaim_operand(kStepTickDec.at, kStepTickDec.op, kStepTickDec.constant, &g_tick_dec, "frame_step=3 (camera hold timer)");
    g_follow_far = *(const float *)(uintptr_t)XI_GAME(kStepFollowFar);
    { int32_t rel = (int32_t)(0x100272B0u - (kStepFollowCall + 5)); uint8_t e[5] = {0xE8}; memcpy(e + 1, &rel, 4);
      all++; ok += reaim_call(kStepFollowCall, e, (void *)sf_follow_k, "frame_step=3 (camera follow)"); }
    for (unsigned i = 0; i < sizeof kStepScale3Call / sizeof kStepScale3Call[0]; i++) {
      int32_t rel = (int32_t)(0x100272E0u - (kStepScale3Call[i] + 5)); uint8_t e[5] = {0xE8}; memcpy(e + 1, &rel, 4);
      all++; ok += reaim_call(kStepScale3Call[i], e, (void *)sf_scale3_k, "frame_step=3 (camera chase, 0x100272E0)");
    }
    { uint8_t want[10] = {0xC7, 0x05}, live[10]; uint32_t v = (uint32_t)XI_GAME(kStepHoldVar); memcpy(want + 2, &v, 4);
      memcpy(want + 6, &kStepHold, 4); all++; g_hold = g_hold0 = kStepHold;
      if (!live_bytes(kStepHoldSet, want, 10, live)) LOG("speedfix: frame_step=3 (camera hold counter) not changed: the bytes at %08X are not mov dword ptr [%08X], %d", kStepHoldSet, v, kStepHold);
      else {
        uint8_t *p = (uint8_t *)(uintptr_t)XI_GAME(kStepHoldSet); int32_t rel = (int32_t)((uintptr_t)sf_follow_hold - (uintptr_t)(p + 5));
        uint8_t e[10] = {0xE8, 0, 0, 0, 0, 0x0F, 0x1F, 0x44, 0x00, 0x00}; memcpy(e + 1, &rel, 4); DWORD old;   // call; 5-byte nop
        if (VirtualProtect(p, 10, PAGE_EXECUTE_READWRITE, &old)) {
          memcpy(p, e, 10); VirtualProtect(p, 10, old, &old); FlushInstructionCache(GetCurrentProcess(), p, 10); ok++;
          LOG("speedfix: frame_step=3 (camera hold counter): %08X now calls speedfix", kStepHoldSet);
        }
      }
    }
    static const uint8_t kThrOp[2] = {0xD8, 0x1D};
    for (int i = 0; i < 4; i++) { all++; ok += reaim_operand(kMoveThrAt[i], kThrOp, 0x10329A18u, &g_move_thr, "frame_step=3 (OnMove 0.01)"); }
    LOG("speedfix: frame_step=3: %d of %d loop changes in place (%u tick loops, the camera follow and the walk/run rate: data/frame_step_loops.json, "
        "pipeline/frame_step_loops.py)", ok, all, (unsigned)(sizeof kStepLoopRet / sizeof kStepLoopRet[0] + 4));
  }
  if (!g_loops_done) return;
  if (!(xi_step_mode == 3 && xi_step_have) && g_loop_on) {     // 3 off: the game's factors and compare (loop_set sets them on)
    g_loop_on = 0; g_loop_s = 1.0f;
    for (int j = 0; j < g_nloop_k; j++) g_loop_k[j] = g_loop_k0[j];
    for (int j = 0; j < 3; j++) g_rate[j] = g_rate0[j];
    g_move_thr = g_move_thr0;
    g_tick_dec = g_tick_dec0; g_hold = g_hold0;
  }
}
extern "C" int sf_hook_dispatch(const XikitHost *);
extern "C" { extern volatile LONG sf_phase_draws[8]; }
struct SfTaskRow { const void *vt; int slot; LONG n; };
extern "C" { extern SfTaskRow sf_task_rows[256]; extern LONG sf_task_other; }
static void task_stats(LONG frames) {                // task_draws: the 12 tasks with the most draws, by vtable and slot
  if (!sf_cfg.task_draws) return;
  SfTaskRow top[12] = {}; LONG all = 0;
  for (int i = 0; i < 256; i++) {
    SfTaskRow r = sf_task_rows[i]; if (!r.vt) continue; all += r.n;
    for (int j = 0; j < 12; j++) if (r.n > top[j].n) { memmove(top + j + 1, top + j, (11 - j) * sizeof top[0]); top[j] = r; break; }
  }
  memset(sf_task_rows, 0, sizeof sf_task_rows);
  uintptr_t base = (uintptr_t)GetModuleHandleA("FFXiMain.dll");
  char line[1400]; int o = snprintf(line, sizeof line, "speedfix: task_draws, %ld frames: %ld draw calls in tasks (%ld more not tabled); by task vtable "
                                      "(analysed address), slot, its +0x20 and +0x24, draws a frame:", frames, all, sf_task_other);
  sf_task_other = 0;
  for (int j = 0; j < 12 && top[j].vt && o < (int)sizeof line - 80; j++) {
    const uintptr_t *vt = (const uintptr_t *)top[j].vt; uintptr_t a = (uintptr_t)vt - base + 0x10000000u;
    uintptr_t f20 = 0, f24 = 0; if (!IsBadReadPtr(vt, 0x28)) { f20 = vt[8] - base + 0x10000000u; f24 = vt[9] - base + 0x10000000u; }
    o += snprintf(line + o, sizeof line - o, " %08X +0x%02X (%08X %08X) %.1f;", (unsigned)a, top[j].slot == 1 ? 0x20 : 0x24, (unsigned)f20, (unsigned)f24,
                  frames ? (double)top[j].n / frames : 0.0);
  }
  LOG("%s", line);
}

static void st_stats(void) {
  task_stats(g_st_frames); step_read_stats(g_st_frames);
  if (sf_cfg.liminality) LOG("speedfix: liminality: %ld tasks skipped on frames of 0 ticks in these %ld frames; drawing windows %ld (ordering table task wrapped: %ld); effect element tasks run on them %ld; nodes put in the ordering table again on them %ld (see-through actors, afterimages); the task pass's draws a frame: %.1f on frames with ticks, %.1f on frames of 0 ticks; limbs (lim_limbs=%d%s): %ld motion draws blended, %ld drawn as the game has them",
      (long)InterlockedExchange(&sf_lim_skipped, 0), (long)g_st_frames, (long)InterlockedExchange(&sf_lim_windows, 0), (long)sf_ot_wrapped, (long)InterlockedExchange(&sf_lim_fx, 0), (long)InterlockedExchange(&sf_lim_ot, 0),
      g_st_frames - g_st_zero > 0 ? (double)InterlockedExchange(&sf_pass_draws_tick, 0) / (g_st_frames - g_st_zero) : 0.0,
      g_st_zero > 0 ? (double)InterlockedExchange(&sf_pass_draws_zero, 0) / g_st_zero : 0.0, sf_cfg.lim_limbs, g_motvt ? "" : ", CYyMotionQue not checked or not the game's", (long)InterlockedExchange(&sf_limb_blended, 0), (long)InterlockedExchange(&sf_limb_asis, 0));
  { LONG n[8]; for (int i = 0; i < 8; i++) n[i] = InterlockedExchange(&sf_phase_draws[i], 0);
    LOG("speedfix: draw calls by Frame_Render's world pass since the last line: XiZone_GetInstance %ld, sub_1018C2B0 %ld, "
        "StDancer_Update %ld, XiZone_DrawScene %ld, the task pass (OnMove) %ld, sub_102515F0 %ld, sound %ld, the rest of the "
        "frame %ld (counted under uncap/frame_step, Frame_Render's fork)", n[1], n[2], n[3], n[4], n[5], n[6], n[7], n[0]); }
  if (sf_cfg.shadow_fit) {
    long n, g; float mn, mx, sm; sf_shadow_take(&n, &g, &mn, &mx, &sm);
    if (n) LOG("speedfix: shadow_fit: %ld simple shadows sized by their model (footprint diagonal %.2f to %.2f, mean %.2f), %ld at the game's size (no box)",
               n, mn, mx, sm / n, g);
    else LOG("speedfix: shadow_fit: no simple shadow drawn (%ld at the game's size)", g);
  }
  LONG f = g_st_frames, z = g_st_zero; g_st_frames = g_st_zero = 0;
  double w = g_st_whole, fr = g_st_frac, gm = g_st_game, ms = g_st_ms; g_st_whole = g_st_frac = g_st_game = g_st_ms = 0;
  if (!sf_framestep_installed || !f) return;
  double real = ms * 0.06;
  if (g_loops_done) { LONG lf = g_loop_frac_frames; g_loop_frac_frames = 0;
    LOG("speedfix: frame_step=3: in these %ld frames the %u tick loops were given %ld new steps of a fraction of a tick (factor k' = 1 - (1 - k)^(s/ceil s))", f,
        (unsigned)(sizeof kStepLoopRet / sizeof kStepLoopRet[0] + 4), lf); }
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
  apply_step_loops();
  apply_sse_skinning();
  xi_skinmerge_on = sf_cfg.skin_merge && sf_skinmerge_installed && sf_hooks_live();   // the game's draws otherwise
  xi_gpuskin_on = sf_cfg.gpu_skin && sf_skinmerge_installed && sf_hooks_live();        // the game's skinning otherwise
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
  {"skin_merge", &SfConfig::skin_merge, 0}, {"gpu_skin", &SfConfig::gpu_skin, 0}, {"precise_pacing", &SfConfig::precise_pacing, 0},
  {"null_draws", &SfConfig::null_draws, 0}, {"occlusion_late", &SfConfig::occlusion_late, 0},
  {"occlusion_nowait", &SfConfig::occlusion_nowait, 0}, {"occlusion_first", &SfConfig::occlusion_first, 0},
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
    if (!sf_device_allows(t) || ((!strcmp(t, "skin_merge") || !strcmp(t, "gpu_skin")) && !sf_skinmerge_installed) || ((!strcmp(t, "frame_step") || !strcmp(t, "uncap")) && !sf_framestep_installed)) { snprintf(bad + strlen(bad), sizeof bad - strlen(bad), " %s (off at this start: its hooks are not in place)", t); continue; }
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
  if (sf_cfg.shadow_fit) {
    if (sf_hook_shadow(host) != XIKIT_OK) LOG("speedfix: Shadow_DrawByMode not hooked: shadow_fit does nothing");
    else LOG("speedfix: shadow_fit=1: simple shadows sized by each character's model (half-width %d%% of its footprint's diagonal)", sf_cfg.shadow_fit_scale);
  }
  if ((sf_cfg.skin_merge || sf_cfg.gpu_skin) && sf_fork_skinmerge(host) != XIKIT_OK) LOG("speedfix: SkinAndDrawV2 not forked: skin_merge and gpu_skin do nothing");
  if (sf_cfg.gpu_skin && sf_skinmerge_installed) LOG("speedfix: gpu_skin: SkinAndDrawV2 runs speedfix's fork; a model's skinning is handed to d3d8on11 (D8ON11_Skin) where it takes it");
  if (sf_cfg.task_draws || sf_cfg.liminality) { if (sf_hook_dispatch(host) != XIKIT_OK) LOG("speedfix: CMoTask::Dispatch not hooked: task_draws does nothing"); }
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
      if (g_base.gpu_skin && !sf_skinmerge_installed) { g_base.gpu_skin = 0; LOG("speedfix: gpu_skin takes effect at the next start (it was off at this one, so SkinAndDrawV2 is not forked)"); }
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
    { long gj = __atomic_exchange_n(&sf_gs_jobs, 0L, __ATOMIC_SEQ_CST), gr = __atomic_exchange_n(&sf_gs_refused, 0L, __ATOMIC_SEQ_CST);
      if (gj + gr || sf_cfg.gpu_skin) LOG("speedfix: gpu_skin: %ld models' skinning handed to the device, %ld left to the game (declined); D8ON11_Skin %s", gj, gr, sf_gpuskin_api ? "found" : "not found yet or not exported"); }
    LONG rs = InterlockedExchange(&g_rb_skipped, 0), rr = InterlockedExchange(&g_rb_run, 0);
    LONG as = InterlockedExchange(&sf_aq_skipped, 0), ar = InterlockedExchange(&sf_aq_run, 0);
    if (rs + rr) LOG("speedfix: occlusion readbacks: %ld run, %ld skipped", rr, rs);
    if (as + ar) LOG("speedfix: actor occlusion queries: %ld run, %ld skipped", ar, as);
    sf_occl_stats();
    pp_stats();
    st_stats();
  }
}
