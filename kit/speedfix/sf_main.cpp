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
  "; merge back-to-back DrawPrimitiveUP calls into one; caps (0 = none) on one merged draw\r\n"
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
  "cpu_tier=1\r\n";

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
      "skip_all_occlusion_readbacks=%d texture_compression=%d(%d) cpu_tier=%d", why, c.state_cache, c.vs_cache, c.ext_cache,
      c.scene_reset, c.batch_up, c.batch_max_prims, c.batch_max_bytes, c.frame_cap_fps, c.occlusion_stagger, c.skip_actor_occlusion,
      c.skip_all_occlusion_readbacks, c.texture_compression, c.texture_compression_value, c.cpu_tier);
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
static void apply_code(void) {                      // render thread, inside Present
  static const uint8_t kBucketCall[5] = {0xE8, 0xC1, 0x87, 0xF4, 0xFF};     // 0x100CC58A call 0x10014D50
  static const uint8_t kReadbackCall[5] = {0xE8, 0xD4, 0x03, 0x00, 0x00};   // 0x1006C4D7 call 0x1006C8B0
  if (!g_bucket_done && sf_cfg.occlusion_stagger > 4) { g_bucket_done = true; reaim_call(0x100CC58Au, kBucketCall, (void *)sf_bucket_stub, "occlusion_stagger"); }
  if (!g_readback_done && sf_cfg.skip_all_occlusion_readbacks) { g_readback_done = true; reaim_call(0x1006C4D7u, kReadbackCall, (void *)sf_readback, "skip_all_occlusion_readbacks"); }
  apply_texture();
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
  sf_cfg = read_config(); ini_time(&g_ini_time);
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
      SfConfig before = sf_cfg; sf_cfg = read_config();
      sf_device_config_changed(&before);
      log_config(sf_cfg, "changed");
      if (sf_cfg.cpu_tier && !before.cpu_tier) LOG("speedfix: cpu_tier is decided once per start");
    }
  }
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
