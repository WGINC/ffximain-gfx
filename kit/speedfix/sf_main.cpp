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
//   frame_cap_fps        An even frame rate (0 = off), at most 60: the game advances its time by max(step, 1) ticks
//                        of 1/60 s per frame (Camera_GetFrameScale, 0x10014CF0; docs/RENDERER.md 3a), so above 60 it
//                        would run faster than real time. Below 60 the step is truncated to whole ticks, so 60, 30, 20
//                        and 15 keep game time and real time equal; another value is used but logged.
//   state_cache, vertex_shader_cache, extended_state_cache, state_cache_reset_each_scene, batch_up_draws,
//   batch_up_max_prims, batch_up_max_bytes: sf_device.cpp.
//
// Not carried over: occlusion_async_readback (measured slower than the stall it removes on the owner's
// dgVoodoo/D3D11/Parallels setup) and unsafe_async_batch_submit (no gain expected: every device call waits for it).
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
#define XI_GAME(a) ((uintptr_t)(a) + (uintptr_t)g_host->rel)

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
  "; even frame pacing (0 = off); 60, 30, 20 or 15 keep the game's time exact; more than 60 is not allowed\r\n"
  "frame_cap_fps=0\r\n"
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
  "; a vertex shader with the same declaration (and no shader code) is made once and its handle given out again\r\n"
  ";   (loading: some 1,450 creations a session, 3 distinct shaders); the same at the next start or live\r\n"
  "vs_dedupe=0\r\n"
  "; the game's SSE skinning loops (0x1002F1D0, 0x100311A0) on any CPU that reports SSE: the game's CPU detection\r\n"
  ";   (0x1000D400) sets its SSE flag only for an Intel or Transmeta vendor string, so other CPUs, and the x86\r\n"
  ";   emulation of Windows on ARM if it reports another vendor, skin with x87; live both ways\r\n"
  "sse_skinning=0\r\n"
  "; A/B of levers with no keys: ab_levers lists settings above (comma-separated); ab_after seconds after it is\r\n"
  ";   saved non-empty, phases of ab_phase seconds A B B A, ab_rounds times: A = those settings off (occlusion_stagger\r\n"
  ";   4, the game's), B = as set above. The first ab_settle frames of a phase are left out. Results in xikit.log\r\n"
  ";   (lines 'speedfix: ab'). Stand still while it runs; keep xikit.ini's ab=0. Empty it and set it again for\r\n"
  ";   another run\r\n"
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
  c.lazy_state = rd("lazy_state", 0); c.vs_dedupe = rd("vs_dedupe", 0); c.sse_skinning = rd("sse_skinning", 0);
  if (c.batch_max_prims < 0) c.batch_max_prims = 0;
  if (c.batch_max_bytes < 0) c.batch_max_bytes = 0;
  if (c.frame_cap_fps < 0 || (c.frame_cap_fps > 0 && c.frame_cap_fps < 10)) c.frame_cap_fps = 0;
  if (c.frame_cap_fps > 60) c.frame_cap_fps = 60;
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
      "skip_all_occlusion_readbacks=%d texture_compression=%d(%d) cpu_tier=%d lazy_state=%d vs_dedupe=%d sse_skinning=%d", why, c.state_cache, c.vs_cache, c.ext_cache,
      c.scene_reset, c.batch_up, c.batch_max_prims, c.batch_max_bytes, c.frame_cap_fps, c.occlusion_stagger, c.skip_actor_occlusion,
      c.skip_all_occlusion_readbacks, c.texture_compression, c.texture_compression_value, c.cpu_tier, c.lazy_state, c.vs_dedupe, c.sse_skinning);
  if (c.frame_cap_fps && 60 % c.frame_cap_fps)
    LOG("speedfix: frame_cap_fps=%d is not 60/n: the game truncates its time step to whole 1/60 s ticks, so its time runs "
        "slower than real time at this cap (60, 30, 20, 15 keep them equal)", c.frame_cap_fps);
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
  g_rb_run++;
  return ((ReadbackFn)(uintptr_t)XI_GAME(0x1006C8B0u))(q, flag);
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
static void apply_sse_skinning(void);
static void log_cpu_tier(void);
static void apply_code(void) {                      // render thread, inside Present
  static const uint8_t kBucketCall[5] = {0xE8, 0xC1, 0x87, 0xF4, 0xFF};     // 0x100CC58A call 0x10014D50
  static const uint8_t kReadbackCall[5] = {0xE8, 0xD4, 0x03, 0x00, 0x00};   // 0x1006C4D7 call 0x1006C8B0
  if (!g_bucket_done && sf_cfg.occlusion_stagger > 4) { g_bucket_done = true; reaim_call(0x100CC58Au, kBucketCall, (void *)sf_bucket_stub, "occlusion_stagger"); }
  if (!g_readback_done && sf_cfg.skip_all_occlusion_readbacks) { g_readback_done = true; reaim_call(0x1006C4D7u, kReadbackCall, (void *)sf_readback, "skip_all_occlusion_readbacks"); }
  apply_texture();
  apply_sse_skinning();
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
  {"batch_up_draws", &SfConfig::batch_up, 0}, {"lazy_state", &SfConfig::lazy_state, 0}, {"vs_dedupe", &SfConfig::vs_dedupe, 0},
  {"occlusion_stagger", &SfConfig::occlusion_stagger, 4}, {"skip_actor_occlusion", &SfConfig::skip_actor_occlusion, 0},
  {"skip_all_occlusion_readbacks", &SfConfig::skip_all_occlusion_readbacks, 0}, {"frame_cap_fps", &SfConfig::frame_cap_fps, 0},
  {"sse_skinning", &SfConfig::sse_skinning, 0},
};
enum { NLEV = sizeof kLevers / sizeof kLevers[0], AB_MAX = 16384 };
static SfConfig g_base;                     // the settings as the ini has them (B)
static char g_ab_list[256];                 // ab_levers as read
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
      "phases of one label differ by up to %.2f ms%s", g_ab_run, g_ab_list, a, b, b - a, a > 0 ? 100.0 * (b - a) / a : 0.0,
      m[1] / c[1] - m[0] / c[0], p[1] / c[1] - p[0] / c[0], noise, c[0] < 2 || c[1] < 2 ? " (one phase of a label: no measure of the noise; use ab_rounds of 2 or more)"
      : fabs(b - a) <= noise ? " (the difference is within that)" : "");
}
static void ab_read(void) {                 // ab_levers and its timings from the ini (each re-read)
  char list[256]; GetPrivateProfileStringA("speedfix", "ab_levers", "", list, sizeof list, g_ini);
  if (strcmp(list, g_ab_list)) {
    if (g_ab_state == 1 || g_ab_state == 2) { set_config(g_base); LOG("speedfix: ab: ab_levers changed: this run stopped, the settings as set"); }
    g_ab_state = 0; strcpy(g_ab_list, list);
  }
  if (g_ab_state || !list[0]) return;
  int n = 0; char bad[256] = "";
  for (int i = 0; i < NLEV; i++) g_ab_use[i] = false;
  for (char *t = strtok(list, ", \t"); t; t = strtok(NULL, ", \t")) {
    int j = 0; while (j < NLEV && strcmp(kLevers[j].key, t)) j++;
    if (j == NLEV) { snprintf(bad + strlen(bad), sizeof bad - strlen(bad), " %s (not a lever)", t); continue; }
    if (!sf_device_allows(t)) { snprintf(bad + strlen(bad), sizeof bad - strlen(bad), " %s (off at this start: its hooks are not in place)", t); continue; }
    if (g_base.*(kLevers[j].field) == kLevers[j].off) { snprintf(bad + strlen(bad), sizeof bad - strlen(bad), " %s (set to its off value: A and B would be the same)", t); continue; }
    g_ab_use[j] = true; n++;
  }
  if (bad[0]) LOG("speedfix: ab: left out:%s", bad);
  if (!n) { LOG("speedfix: ab: no lever to compare; nothing runs"); g_ab_state = 3; return; }
  g_ab_after = rd("ab_after", 60); g_ab_phase = rd("ab_phase", 30); g_ab_settle = rd("ab_settle", 60);
  int rounds = rd("ab_rounds", 4);
  if (g_ab_after < 0) g_ab_after = 0;
  if (g_ab_phase < 1) g_ab_phase = 1;
  if (rounds < 1) rounds = 1;
  if (rounds > 16) rounds = 16;
  if (g_ab_settle < 0) g_ab_settle = 0;
  g_ab_nph = 2 * rounds; for (int i = 0; i < 32; i++) g_ab_cnt[i] = 0;
  g_ab_state = 1; g_ab_t0 = GetTickCount();
  LOG("speedfix: ab: %s: starts in %d s: %d phases of %d s (A B B A ...), the first %d frames of each left out; stand still",
      g_ab_list, g_ab_after, g_ab_nph, g_ab_phase, g_ab_settle);
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
    else { ab_finish(); g_ab_state = 3; LOG("speedfix: ab: empty ab_levers and set it again for another run"); }
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
  if (sf_device_install(host) != 0) return 1;
  if (sf_hook_query(host) != XIKIT_OK) LOG("speedfix: Occlusion_QueryScreenRect not hooked: skip_actor_occlusion does nothing");
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
      log_config(g_base, "changed");
      if (sf_cfg.cpu_tier && !before.cpu_tier) LOG("speedfix: cpu_tier is decided once per start");
    }
  }
  if (n % 60 == 1) ab_read();
  ab_control();
  apply_code();
  sf_device_frame();
  if (GetTickCount() - last_stats >= 60000) {
    last_stats = GetTickCount();
    sf_device_stats();
    LONG rs = InterlockedExchange(&g_rb_skipped, 0), rr = InterlockedExchange(&g_rb_run, 0);
    LONG as = InterlockedExchange(&sf_aq_skipped, 0), ar = InterlockedExchange(&sf_aq_run, 0);
    if (rs + rr) LOG("speedfix: occlusion readbacks: %ld run, %ld skipped", rr, rs);
    if (as + ar) LOG("speedfix: actor occlusion queries: %ld run, %ld skipped", ar, as);
  }
}
