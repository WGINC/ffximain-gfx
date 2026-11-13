// d8_device.cpp -- d3d8on11: IDirect3DDevice8, milestone 1 (docs/D3D11.md phase 1; kit/d3d8on11/README.md).
//
// Milestone 1 makes the device, its swap chain, resources and their uploads, Clear, Present, Reset, render targets,
// copies and read-backs, and keeps every state the game sets and reads back, state blocks included. Milestone 2
// executes draws through generated shaders (d8_ff.cpp).
#include "d8.h"
#include "d8_skin.h"
static Dev *g_skin_dev;                       // gpu skinning: the device D8ON11_Skin hands its jobs to (set in init)
#include <string>
#include <stdlib.h>

static DWORD fbits(float f) { DWORD d; memcpy(&d, &f, 4); return d; }
static void identity(D3DMATRIX *m) { memset(m, 0, sizeof *m); m->_11 = m->_22 = m->_33 = m->_44 = 1; }

// ---- creation, swap chain, defaults ----------------------------------------------------------------------------------
HRESULT Dev::init(D8 *o, HWND focus, DWORD beh, D3DPRESENT_PARAMETERS *p) { sgen++;
  d3d = o; d3d->AddRef(); behavior = beh; pp = *p;
  wnd = pp.hDeviceWindow ? pp.hDeviceWindow : focus;
  // the executor: on this thread (direct), on its own thread (thread, the default), or in its own process (remote: the
  // ARM64 build on an ARM64 machine). d3d8on11.ini next to the DLL, [d3d8on11] mode=; D8ON11_MODE overrides it
  char mode[32] = "thread", ini[MAX_PATH]; HMODULE m = NULL;
  GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&identity, &m);
  GetModuleFileNameA(m, ini, sizeof ini); char *sl = strrchr(ini, '\\'); strcpy(sl ? sl + 1 : ini, "d3d8on11.ini");
  GetPrivateProfileStringA("d3d8on11", "mode", "thread", mode, sizeof mode, ini);
  char env[32]; DWORD ne = GetEnvironmentVariableA("D8ON11_MODE", env, sizeof env);
  if (ne > 0 && ne < sizeof env) strcpy(mode, env);
  link = link_make(mode);
  if (!link && strcmp(mode, "direct")) { d8_log("mode %s could not start: thread instead", mode); link = link_make("thread"); }
  if (!link) link = link_make("direct");
  d8_log("executor: %s", link->name());
  // exec_filter=1: the executor skips the Direct3D 11 calls that set what is already set (d8_exec.cpp, Exec::draw);
  // D8ON11_EXEC_FILTER overrides it. The default is 1: the owner's exec_ab run of 16 Oct (docs/HISTORY.md, 17 Oct),
  // every phase with it faster than every phase without. exec_ab=1: the A/B below, from the setting's value
  ab.filter = (int)GetPrivateProfileIntA("d3d8on11", "exec_filter", 1, ini);
  ne = GetEnvironmentVariableA("D8ON11_EXEC_FILTER", env, sizeof env);
  if (ne > 0 && ne < sizeof env) ab.filter = atoi(env);
  // exec_batch=1: the executor holds records and runs them together, one Map of each large buffer per batch instead of
  // Maps by each draw (d8_exec.h, Exec::batch); D8ON11_EXEC_BATCH overrides it
  ab.batch = (int)GetPrivateProfileIntA("d3d8on11", "exec_batch", 1, ini);
  ne = GetEnvironmentVariableA("D8ON11_EXEC_BATCH", env, sizeof env);
  if (ne > 0 && ne < sizeof env) ab.batch = atoi(env);
  // exec_ab_lever: which switch the A/B turns, filter (exec_filter) or batch (exec_batch)
  { char lv[16]; GetPrivateProfileStringA("d3d8on11", "exec_ab_lever", "filter", lv, sizeof lv, ini);
    ab.key = !_stricmp(lv, "batch") ? OPT_BATCH : !_stricmp(lv, "flip") ? OPT_FLIP : !_stricmp(lv, "merge") ? OPT_MERGE : OPT_FILTER; }
  // swap_flip=1: the flip-model swap chain (DXGI_SWAP_EFFECT_FLIP_DISCARD, 2 buffers) instead of the blt model (DISCARD,
  // 1 buffer, as before); D8ON11_SWAP_FLIP overrides it. exec_ab_lever=flip: the A/B turns it (off = blt, on = flip)
  ab.flip = (int)GetPrivateProfileIntA("d3d8on11", "swap_flip", 0, ini);
  ne = GetEnvironmentVariableA("D8ON11_SWAP_FLIP", env, sizeof env);
  if (ne > 0 && ne < sizeof env) ab.flip = atoi(env);
  // exec_ab_go=1: the A/B starts when d3d8on11_ab_go.txt appears next to this DLL (it is deleted then), instead of
  // exec_ab_after seconds after the device: the session's abtest.bat writes it, so the phases start with its marks
  ab.go = (int)GetPrivateProfileIntA("d3d8on11", "exec_ab_go", 0, ini);
  strcpy(ab_go_path, ini); { char *s2 = strrchr(ab_go_path, '\\'); strcpy(s2 ? s2 + 1 : ab_go_path, "d3d8on11_ab_go.txt"); }
  if (ab.go) DeleteFileA(ab_go_path);                      // a file left from an earlier start does not start this one's
  ab.on = (int)GetPrivateProfileIntA("d3d8on11", "exec_ab", 0, ini);
  ab.after = (int)GetPrivateProfileIntA("d3d8on11", "exec_ab_after", 60, ini);
  ab.phase = (int)GetPrivateProfileIntA("d3d8on11", "exec_ab_phase", 30, ini);
  ab.rounds = (int)GetPrivateProfileIntA("d3d8on11", "exec_ab_rounds", 4, ini);
  ab.settle = (int)GetPrivateProfileIntA("d3d8on11", "exec_ab_settle", 60, ini);
  // exec_ab_lever=flip is refused (7 Nov: the owner's game froze for good at the switch from the flip model back to the
  // blt model, the second segment of abtest flip; the cause is not established, its log was lost): swap_flip is a
  // setting for the whole start, compared by two starts, not switched while the game runs
  if (ab.on && ab.key == OPT_FLIP) { ab.on = 0; d8_log("exec_ab_lever=flip: refused (the swap chain is not switched while the game runs; set swap_flip for the whole start): the A/B is off"); }
  if (ab.phase < 1) ab.phase = 1; if (ab.settle < 1) ab.settle = 1; if (ab.rounds < 1) ab.rounds = 1; if (ab.after < 0) ab.after = 0;
  ab.i = -1;
  { XOption o = {OPT_FILTER, (uint32_t)(ab.filter != 0), 0}; link->put(X_OPTION, &o, sizeof o); }
  { XOption o = {OPT_BATCH, (uint32_t)(ab.batch != 0), 0}; link->put(X_OPTION, &o, sizeof o); }
  { XOption o = {OPT_FLIP, (uint32_t)(ab.flip != 0), 0}; link->put(X_OPTION, &o, sizeof o); }
  // gpu_time=1: the GPU's time for each frame's work (timestamp queries), logged every 600 Presents; D8ON11_GPU_TIME
  { int g = (int)GetPrivateProfileIntA("d3d8on11", "gpu_time", 0, ini);
    ne = GetEnvironmentVariableA("D8ON11_GPU_TIME", env, sizeof env); if (ne > 0 && ne < sizeof env) g = atoi(env);
    XOption o = {OPT_GPUTIME, (uint32_t)(g != 0), 0}; link->put(X_OPTION, &o, sizeof o);
    if (g) d8_log("gpu_time=1: the GPU's time per frame is logged every 600 frames"); }
  bool backend12 = false;
  // backend=d3d12 (12 Nov; default d3d11): the executor runs the stream on Direct3D 12 (d8_exec12.h), or on Direct3D 11
  // when Direct3D 12 cannot start. Before X_INIT, for the whole start. D8ON11_BACKEND overrides it
  { char be[16] = {}; GetPrivateProfileStringA("d3d8on11", "backend", "d3d11", be, sizeof be, ini);
    ne = GetEnvironmentVariableA("D8ON11_BACKEND", env, sizeof env); if (ne > 0 && ne < sizeof env) snprintf(be, sizeof be, "%s", env);
    if (!_stricmp(be, "d3d12")) { XOption o = {OPT_BACKEND, 12u, 0}; link->put(X_OPTION, &o, sizeof o); backend12 = true; } }
  // d3d12_probe=1 (the default, 12 Nov): once, after the first Present, the executor tries to make a Direct3D 12 device
  // and run a command list on it, and logs each step (Exec::d3d12_probe); 0 turns it off. D8ON11_D3D12_PROBE overrides it
  { int p = (int)GetPrivateProfileIntA("d3d8on11", "d3d12_probe", 1, ini);
    ne = GetEnvironmentVariableA("D8ON11_D3D12_PROBE", env, sizeof env); if (ne > 0 && ne < sizeof env) p = atoi(env);
    XOption o = {OPT_D3D12, (uint32_t)(p != 0), 0}; link->put(X_OPTION, &o, sizeof o); }
  if (ab.flip) d8_log("swap_flip=1: the flip-model swap chain (FLIP_DISCARD, 2 buffers)");
  // frame_latency=N (1..16): IDXGIDevice1::SetMaximumFrameLatency(N), the frames the driver may queue ahead of the GPU
  // (DXGI's default 3). 1: each frame shown sooner after it is made, and Present waits instead of queueing. A start
  // setting; D8ON11_FRAME_LATENCY overrides it
  // (1 is the default since 9 Nov: with dynamic_buffers, the owner's smoothest plaza; 0 = DXGI's own default)
  { int fl = (int)GetPrivateProfileIntA("d3d8on11", "frame_latency", 1, ini);
    ne = GetEnvironmentVariableA("D8ON11_FRAME_LATENCY", env, sizeof env); if (ne > 0 && ne < sizeof env) fl = atoi(env);
    if (fl < 0 || fl > 16) fl = 0;
    XOption o = {OPT_LATENCY, (uint32_t)fl, 0}; link->put(X_OPTION, &o, sizeof o); }
  // draw_sites=1: each draw carries the game's call site, and the executor's log lists its draw time by site every 600
  // Presents (a measurement; off by default, so that a run measuring something else is not changed by it)
  // dynamic_buffers=1: the buffers the game makes D3DUSAGE_DYNAMIC are D3D11 dynamic buffers, written by Map with the
  // game's lock flags (DISCARD: fresh memory, NOOVERWRITE: beside what the GPU reads) instead of UpdateSubresource of a
  // default buffer, which the driver must copy when the GPU still uses it (7 Nov: in the owner's runs Present's time
  // follows the upload time, r = 0.88-0.89, not the draws). Set for a whole start; D8ON11_DYNAMIC_BUFFERS overrides it
  // (the default since 9 Nov: the owner's runs, Present 12.9 -> 10.2 ms in the plaza with it alone)
  dynamic_buffers = (int)GetPrivateProfileIntA("d3d8on11", "dynamic_buffers", 1, ini) != 0;
  ne = GetEnvironmentVariableA("D8ON11_DYNAMIC_BUFFERS", env, sizeof env);
  if (ne > 0 && ne < sizeof env) dynamic_buffers = atoi(env) != 0;
  if (dynamic_buffers) d8_log("dynamic_buffers=1: the game's dynamic vertex and index buffers are written by Map");
  // buffer_arena (10 Nov; with dynamic_buffers): the dynamic buffers' contents live in one large Direct3D 11 buffer
  // (d8_exec.h, BArena), so a batch writes all its buffer uploads under one Map instead of one Map(DISCARD) of a buffer
  // each (the owner's plaza: 300-650 a frame). 1: a 16 MB vertex arena; a larger number: that many KB (1024 to
  // 262144); 0 (the default until it is measured in the game): off. Set for a whole start; D8ON11_BUFFER_ARENA overrides.
  { int ba = (int)GetPrivateProfileIntA("d3d8on11", "buffer_arena", 0, ini);
    ne = GetEnvironmentVariableA("D8ON11_BUFFER_ARENA", env, sizeof env);
    if (ne > 0 && ne < sizeof env) ba = atoi(env);
    uint32_t kb = ba <= 0 || !dynamic_buffers ? 0u : ba == 1 ? 16384u : ba < 1024 ? 1024u : ba > 262144 ? 262144u : (uint32_t)ba;
    ne = GetEnvironmentVariableA("D8ON11_BUFFER_ARENA_TEST_KB", env, sizeof env);       // the tests: a small arena, so it starts again
    if (kb && ne > 0 && ne < sizeof env && atoi(env) >= 1) kb = (uint32_t)atoi(env);
    XOption o = {OPT_BUFARENA, kb, 0}; link->put(X_OPTION, &o, sizeof o);
    // draw_merge (11 Nov; needs buffer_arena and exec_batch): consecutive indexed triangle-list draws equal in all but
    // their buffers and index range are drawn as one (d8_exec.h). 0 (the default until it is measured in the game): off.
    // Set for a whole start (the executor keeps a copy of every buffer up to 256 KB); exec_ab_lever=merge switches it
    // off and on while the game runs. D8ON11_DRAW_MERGE overrides.
    ab.merge = (int)GetPrivateProfileIntA("d3d8on11", "draw_merge", 0, ini);
    ne = GetEnvironmentVariableA("D8ON11_DRAW_MERGE", env, sizeof env);
    if (ne > 0 && ne < sizeof env) ab.merge = atoi(env);
    // (the Direct3D 12 executor merges without them: it merges runs of delta draws, so it needs draw_delta=1 instead)
    if (ab.merge && !(kb && ab.batch) && !backend12) { d8_log("draw_merge: needs buffer_arena=1 and exec_batch=1 (off)"); ab.merge = 0; }
    if (ab.merge) { XOption om = {OPT_MERGE, 3u, 0}; link->put(X_OPTION, &om, sizeof om);
                    d8_log("draw_merge=1: consecutive indexed list draws equal in all but their buffers and range are drawn as one"); }
    if (ab.on && ab.key == OPT_MERGE && !ab.merge) { ab.on = 0; d8_log("exec_ab_lever=merge: needs draw_merge=1 at the start: the A/B is off"); }
    // draw_delta (11 Nov): a buffer draw made while no state has changed since the last one sends only the packet's
    // tail (176 bytes) instead of building and sending all 3,200 (the owner's run of 16 Nov: 2.3 us of the game's
    // thread a draw). 0 (the default until it is measured in the game): off. D8ON11_DRAW_DELTA overrides.
    { int dd = (int)GetPrivateProfileIntA("d3d8on11", "draw_delta", 0, ini);
      ne = GetEnvironmentVariableA("D8ON11_DRAW_DELTA", env, sizeof env);
      if (ne > 0 && ne < sizeof env) dd = atoi(env);
      draw_delta = dd != 0;
      if (draw_delta) d8_log("draw_delta=1: a buffer draw with no state changed since the last sends only its buffers and range"); }
    // gpu_skin (11 Nov; d8_skin.h): D8ON11_Skin takes skinning jobs from a mod; the vertex shader then makes each
    // vertex from a static source and a bone palette, and the game's thread neither skins nor uploads. 0 (the default
    // until it is measured in the game): every job is refused. D8ON11_GPU_SKIN overrides.
    { int gs = (int)GetPrivateProfileIntA("d3d8on11", "gpu_skin", 0, ini);
      ne = GetEnvironmentVariableA("D8ON11_GPU_SKIN", env, sizeof env);
      if (ne > 0 && ne < sizeof env) gs = atoi(env);
      gpu_skin = gs != 0; g_skin_dev = this;
      if (gpu_skin) d8_log("gpu_skin=1: D8ON11_Skin takes skinning jobs (positions and normals made in the vertex shader)"); }
    // interp (17 Nov): frames between the game's ticks drawn with skinned characters blended between their last two
    // ticks ("interp" below). Needs gpu_skin=1 and a mod's step clock (speedfix with frame_step and uncap). 0 (the
    // default until it is seen in the game): off. D8ON11_INTERP overrides.
    { int it = (int)GetPrivateProfileIntA("d3d8on11", "interp", 0, ini);
      ne = GetEnvironmentVariableA("D8ON11_INTERP", env, sizeof env);
      if (ne > 0 && ne < sizeof env) it = atoi(env);
      interp = it != 0 && gpu_skin;
      if (it && !gpu_skin) d8_log("interp: needs gpu_skin=1 (off)");
      if (interp) d8_log("interp=1: between the game's ticks, skinned characters are drawn at the blend of their last two ticks (needs a mod's step clock: D8ON11_InterpClock)"); }
    // mipmaps (19 Nov; d8_mip.cpp): the game's one-level managed textures (A8R8G8B8, X8R8G8B8, DXT1/3/5) get levels,
    // sampled trilinear in 3D draws; anisotropy N > 1: anisotropic where the game magnifies LINEAR. Changes the look at
    // a distance (that is its purpose). 0 (the default): the game's textures as they are. D8ON11_MIPMAPS and
    // D8ON11_ANISOTROPY override
    { int mm = (int)GetPrivateProfileIntA("d3d8on11", "mipmaps", 0, ini), an = (int)GetPrivateProfileIntA("d3d8on11", "anisotropy", 0, ini);
      ne = GetEnvironmentVariableA("D8ON11_MIPMAPS", env, sizeof env); if (ne > 0 && ne < sizeof env) mm = atoi(env);
      ne = GetEnvironmentVariableA("D8ON11_ANISOTROPY", env, sizeof env); if (ne > 0 && ne < sizeof env) an = atoi(env);
      mipmaps = mm != 0; anisotropy = an < 0 ? 0 : an > 16 ? 16 : an;
      if (mipmaps) d8_log("mipmaps=1: one-level managed textures get mip levels, sampled trilinear in 3D draws%s", "");
      if (mipmaps && anisotropy > 1) d8_log("anisotropy=%d: anisotropic filtering in 3D draws of textures with made levels, where the game magnifies LINEAR", anisotropy); }
    if (ba > 0 && !dynamic_buffers) d8_log("buffer_arena: needs dynamic_buffers=1 (off)");
    else if (kb) d8_log("buffer_arena=%d: the game's dynamic buffers share one Direct3D 11 buffer (%u KB of vertex data)", ba, kb); }
  draw_sites = (int)GetPrivateProfileIntA("d3d8on11", "draw_sites", 0, ini) != 0;
  ne = GetEnvironmentVariableA("D8ON11_DRAW_SITES", env, sizeof env);
  if (ne > 0 && ne < sizeof env) draw_sites = atoi(env) != 0;
  { XOption o = {OPT_SITES, (uint32_t)draw_sites, 0}; link->put(X_OPTION, &o, sizeof o); }
  if (draw_sites) d8_log("draw_sites=1: the executor's draw time is logged by the game's call site");
  d8_log("executor filter: %d, batch: %d%s", ab.filter != 0, ab.batch != 0,
         ab.on ? (ab.key == OPT_BATCH ? "; the A/B of batch is on (exec_ab)" : ab.key == OPT_FLIP ? "; the A/B of the flip-model swap chain is on (exec_ab)" :
                  ab.key == OPT_MERGE ? "; the A/B of draw_merge is on (exec_ab)" : "; the A/B of filter is on (exec_ab)") : "");
  if (ab.on) d8_log("executor ab: %d s after the device, %d phases of %d s (%s off, on, on, off, ...), the first %d frames of each left out; "
                    "stand still while it runs", ab.after, 4 * ab.rounds, ab.phase,
                    ab.key == OPT_BATCH ? "batch" : ab.key == OPT_FLIP ? "flip" : ab.key == OPT_MERGE ? "merge" : "filter", ab.settle);
  // present_interval: Present's SyncInterval, 0 to 4, instead of the one the game's parameters map to (anything else,
  // or nothing: the game's); D8ON11_PRESENT_INTERVAL overrides it (Dev::present_interval)
  { char pv[16] = ""; GetPrivateProfileStringA("d3d8on11", "present_interval", "", pv, sizeof pv, ini);
    ne = GetEnvironmentVariableA("D8ON11_PRESENT_INTERVAL", env, sizeof env);
    if (ne > 0 && ne < sizeof env) strcpy(pv, env);
    present_force = pv[0] >= '0' && pv[0] <= '4' && !pv[1] ? pv[0] - '0' : -1; }
  sync = present_interval(*p, "CreateDevice");
  HRESULT hr = make_swap(false);
  if (FAILED(hr) && strcmp(link->name(), "thread") && strcmp(link->name(), "direct")) {
    // the executor process could not make the device or the swap chain (d3d8on11_exec.log says why): in-process instead
    d8_log("the %s executor could not start the device: thread instead", link->name());
    delete link; link = link_make("thread"); if (!link) link = link_make("direct");
    d8_log("executor: %s", link->name());
    hr = make_swap(false);
  }
  if (FAILED(hr)) return hr;
  vshaders.push_back(VShader{false, 0, {}, {}});            // index 0 unused: handles start at 3
  memset(&gamma, 0, sizeof gamma);
  for (int i = 0; i < 256; i++) gamma.red[i] = gamma.green[i] = gamma.blue[i] = (WORD)(i * 257);
  defaults();
  d8_log("device: %ux%u, back buffer format %lu, %s, depth %s (format %lu), behavior 0x%lX, window %p",
         pp.BackBufferWidth, pp.BackBufferHeight, (unsigned long)pp.BackBufferFormat, pp.Windowed ? "windowed" : "full screen asked: windowed",
         pp.EnableAutoDepthStencil ? "yes" : "no", (unsigned long)pp.AutoDepthStencilFormat, (unsigned long)beh, (void *)wnd);
  return D3D_OK;
}
// The game's presentation parameters, logged at CreateDevice and Reset as it passed them, and the SyncInterval of
// IDXGISwapChain::Present they map to. DXGI: SyncInterval 0, "the presentation occurs immediately, there is no
// synchronization"; 1 through 4, "synchronize presentation after the nth vertical blank" (IDXGISwapChain::Present).
// D3D8 (DirectX 8.1 reference, D3DPRESENT_PARAMETERS and D3DSWAPEFFECT; the values from d3d8types.h and d3d8caps.h):
//   - D3DSWAPEFFECT_COPY_VSYNC, a windowed swap effect, copies at the vertical retrace: 1;
//   - full screen, FullScreen_PresentationInterval D3DPRESENT_INTERVAL_ONE, TWO, THREE, FOUR: Present waits for that
//     many vertical retraces: 1 to 4;
//   - IMMEDIATE, and windowed with another swap effect (FullScreen_PresentationInterval must be DEFAULT there): 0.
// Full screen with D3DPRESENT_INTERVAL_DEFAULT is not mapped from a source here (this device renders windowed, and the
// text that would say what DEFAULT does in full screen is not quoted in this repository): 0, as every Present was
// before 26 Oct, and the log line says so. present_interval in d3d8on11.ini (present_force) overrides all of these.
uint32_t Dev::present_interval(const D3DPRESENT_PARAMETERS &p, const char *at) {
  static const char *se[] = {"?", "DISCARD", "FLIP", "COPY", "COPY_VSYNC"};
  DWORD iv = p.FullScreen_PresentationInterval; uint32_t g = 0; const char *why;
  if (p.SwapEffect == D3DSWAPEFFECT_COPY_VSYNC) { g = 1; why = "COPY_VSYNC"; }
  else if (!p.Windowed && iv == D3DPRESENT_INTERVAL_ONE) { g = 1; why = "full screen, INTERVAL_ONE"; }
  else if (!p.Windowed && iv == D3DPRESENT_INTERVAL_TWO) { g = 2; why = "full screen, INTERVAL_TWO"; }
  else if (!p.Windowed && iv == D3DPRESENT_INTERVAL_THREE) { g = 3; why = "full screen, INTERVAL_THREE"; }
  else if (!p.Windowed && iv == D3DPRESENT_INTERVAL_FOUR) { g = 4; why = "full screen, INTERVAL_FOUR"; }
  else if (iv == D3DPRESENT_INTERVAL_IMMEDIATE) why = "INTERVAL_IMMEDIATE";
  else if (p.Windowed) why = "windowed";
  else why = "full screen, INTERVAL_DEFAULT or another value: not mapped from a source, 0 as before";
  uint32_t v = present_force >= 0 ? (uint32_t)present_force : g;
  d8_log("%s: the game asked SwapEffect %lu (%s), FullScreen_PresentationInterval 0x%lX, BackBufferCount %u, Flags 0x%lX, %s; "
         "Present's SyncInterval %u (%s)", at, (unsigned long)p.SwapEffect, p.SwapEffect <= 4 ? se[p.SwapEffect] : "?", (unsigned long)iv,
         p.BackBufferCount, (unsigned long)p.Flags, p.Windowed ? "windowed" : "full screen", v,
         present_force >= 0 ? "present_interval in d3d8on11.ini" : why);
  if (present_force >= 0) d8_log("%s: the game's parameters map to SyncInterval %u (%s); present_interval=%d is used instead", at, g, why, present_force);
  return v;
}
HRESULT Dev::make_swap(bool resize) { sgen++;
  if (!pp.Windowed) d8_once("fullscreen", "full screen was asked: rendered windowed");
  RECT cr; GetClientRect(wnd, &cr);
  if (!pp.BackBufferWidth) pp.BackBufferWidth = cr.right > 0 ? cr.right : 1;     // D3D8: 0 in windowed mode is the client area
  if (!pp.BackBufferHeight) pp.BackBufferHeight = cr.bottom > 0 ? cr.bottom : 1;
  if (pp.BackBufferFormat == D3DFMT_UNKNOWN) pp.BackBufferFormat = D3DFMT_X8R8G8B8;
  if (!pp.BackBufferCount) pp.BackBufferCount = 1;
  uint32_t ds_id = pp.EnableAutoDepthStencil ? new_id() : 0;
  if (pp.EnableAutoDepthStencil && (!d8_fmt(pp.AutoDepthStencilFormat).depth)) { d8_notimpl("this automatic depth-stencil format"); ds_id = 0; }
  XInit x = {(uint64_t)(uintptr_t)wnd, pp.BackBufferWidth, pp.BackBufferHeight, ds_id ? (uint32_t)pp.AutoDepthStencilFormat : 0u, ds_id, sync, 0};
  int32_t hr = link->call(resize ? X_RESIZE : X_INIT, &x, sizeof x);
  if (hr < 0) return D3DERR_NOTAVAILABLE;
  backbuf = new Surf(this, SK_BACKBUFFER, pp.BackBufferWidth, pp.BackBufferHeight, pp.BackBufferFormat, D3DPOOL_DEFAULT, D3DUSAGE_RENDERTARGET);
  backbuf->id = X_BACKBUFFER; backbuf->lockable = (pp.Flags & D3DPRESENTFLAG_LOCKABLE_BACKBUFFER) != 0;
  curRT = backbuf; curRT->AddRef();
  if (ds_id) {
    autods = new Surf(this, SK_DS, pp.BackBufferWidth, pp.BackBufferHeight, pp.AutoDepthStencilFormat, D3DPOOL_DEFAULT, D3DUSAGE_DEPTHSTENCIL);
    autods->id = ds_id; curDS = autods; curDS->AddRef();
  }
  targets();
  return D3D_OK;
}
void Dev::targets() { sgen++;
  XTargets t; memset(&t, 0, sizeof t);
  if (curRT) t.rt = curRT->xref();
  t.ds = curDS ? curDS->id : 0;
  link->put(X_TARGETS, &t, sizeof t);
}
void Dev::release_swap() { sgen++;
  if (curRT) curRT->Release(); if (curDS) curDS->Release(); curRT = curDS = nullptr;
  if (autods) autods->Release(); autods = nullptr;
  if (backbuf) backbuf->Release(); backbuf = nullptr;
}
// D3D8's default state (the DirectX 8 SDK reference, the D3DRENDERSTATETYPE and D3DTEXTURESTAGESTATETYPE pages).
// test/test_d3d8on11.py reads every render state and stage state back from a fresh device here and under Wine's own
// d3d8, and lists each difference.
void Dev::defaults() { sgen++;
  for (int i = 0; i < 8; i++) if (st.tex[i]) st.tex[i]->Release();
  for (int i = 0; i < 16; i++) if (st.stream[i]) st.stream[i]->Release();
  if (st.ib) st.ib->Release();
  memset(st.rs, 0, sizeof st.rs); memset(st.tss, 0, sizeof st.tss); memset(st.tex, 0, sizeof st.tex);
  memset(st.stream, 0, sizeof st.stream); memset(st.stride, 0, sizeof st.stride); st.ib = nullptr; st.baseVertex = 0;
  DWORD *r = st.rs;
  r[D3DRS_ZENABLE] = pp.EnableAutoDepthStencil ? D3DZB_TRUE : D3DZB_FALSE;
  r[D3DRS_FILLMODE] = D3DFILL_SOLID; r[D3DRS_SHADEMODE] = D3DSHADE_GOURAUD; r[D3DRS_ZWRITEENABLE] = TRUE;
  r[D3DRS_LASTPIXEL] = TRUE; r[D3DRS_SRCBLEND] = D3DBLEND_ONE; r[D3DRS_DESTBLEND] = D3DBLEND_ZERO;
  r[D3DRS_CULLMODE] = D3DCULL_CCW; r[D3DRS_ZFUNC] = D3DCMP_LESSEQUAL; r[D3DRS_ALPHAFUNC] = D3DCMP_ALWAYS;
  r[D3DRS_FOGSTART] = fbits(0.0f); r[D3DRS_FOGEND] = fbits(1.0f); r[D3DRS_FOGDENSITY] = fbits(1.0f);
  r[D3DRS_STENCILFAIL] = r[D3DRS_STENCILZFAIL] = r[D3DRS_STENCILPASS] = D3DSTENCILOP_KEEP;
  r[D3DRS_STENCILFUNC] = D3DCMP_ALWAYS; r[D3DRS_STENCILMASK] = r[D3DRS_STENCILWRITEMASK] = 0xFFFFFFFF;
  r[D3DRS_TEXTUREFACTOR] = 0xFFFFFFFF; r[D3DRS_CLIPPING] = TRUE; r[D3DRS_LIGHTING] = TRUE;
  r[D3DRS_COLORVERTEX] = TRUE; r[D3DRS_LOCALVIEWER] = TRUE;
  r[D3DRS_DIFFUSEMATERIALSOURCE] = D3DMCS_COLOR1; r[D3DRS_SPECULARMATERIALSOURCE] = D3DMCS_COLOR2;
  // AMBIENTMATERIALSOURCE: D3DMCS_MATERIAL, as Microsoft's DirectX 8.1 runtime reports on a fresh device (t8 defaults,
  // T8_REF). The DirectX 8.1 reference contradicts itself (D3DRENDERSTATETYPE: "The default value is D3DMCS_COLOR2";
  // "Color Vertices": D3DMCS_MATERIAL); Wine and the Direct3D 9 reference give MATERIAL too. The two differ only for a
  // lit vertex with a specular colour, which the game's traced formats do not have (FVF codes 0x42, 0x44, 0x104, 0x142,
  // 0x144, 0x152; its declarations map no register 6)
  r[D3DRS_AMBIENTMATERIALSOURCE] = D3DMCS_MATERIAL; r[D3DRS_EMISSIVEMATERIALSOURCE] = D3DMCS_MATERIAL;
  r[D3DRS_SOFTWAREVERTEXPROCESSING] = (behavior & D3DCREATE_SOFTWARE_VERTEXPROCESSING) ? TRUE : FALSE;
  r[D3DRS_POINTSIZE] = fbits(1.0f); r[D3DRS_POINTSIZE_MIN] = fbits(0.0f); r[D3DRS_POINTSCALE_A] = fbits(1.0f);
  r[D3DRS_POINTSCALE_B] = fbits(0.0f); r[D3DRS_POINTSCALE_C] = fbits(0.0f);
  r[D3DRS_MULTISAMPLEANTIALIAS] = TRUE; r[D3DRS_MULTISAMPLEMASK] = 0xFFFFFFFF; r[D3DRS_PATCHSEGMENTS] = fbits(1.0f);
  r[D3DRS_POINTSIZE_MAX] = fbits(64.0f); r[D3DRS_COLORWRITEENABLE] = 0xF; r[D3DRS_TWEENFACTOR] = fbits(0.0f);
  r[D3DRS_BLENDOP] = D3DBLENDOP_ADD;
  // D3DRS_DEBUGMONITORTOKEN: the reference says it "defaults to D3DDMT_ENABLE" (0), but Microsoft's DirectX 8.1 runtime
  // answers 0xBAADCAFE on a fresh device (t8 defaults, T8_REF), as Wine does; the runtime's answer is kept
  r[D3DRS_DEBUGMONITORTOKEN] = 0xBAADCAFE;
  // N-patch interpolation orders: D3DORDER_CUBIC (3) and D3DORDER_LINEAR (1), the DirectX 8.1 reference's defaults
  // and d3d8types.h's comments; not drawn here (no N-patches), only answered
  r[D3DRS_POSITIONORDER] = 3; r[D3DRS_NORMALORDER] = 1;
  for (int s = 0; s < 8; s++) {
    DWORD *t = st.tss[s];
    t[D3DTSS_COLOROP] = s ? D3DTOP_DISABLE : D3DTOP_MODULATE; t[D3DTSS_ALPHAOP] = s ? D3DTOP_DISABLE : D3DTOP_SELECTARG1;
    t[D3DTSS_COLORARG1] = t[D3DTSS_ALPHAARG1] = D3DTA_TEXTURE; t[D3DTSS_COLORARG2] = t[D3DTSS_ALPHAARG2] = D3DTA_CURRENT;
    t[D3DTSS_TEXCOORDINDEX] = s;
    t[D3DTSS_ADDRESSU] = t[D3DTSS_ADDRESSV] = t[D3DTSS_ADDRESSW] = D3DTADDRESS_WRAP;
    t[D3DTSS_MAGFILTER] = t[D3DTSS_MINFILTER] = D3DTEXF_POINT; t[D3DTSS_MIPFILTER] = D3DTEXF_NONE;
    t[D3DTSS_MAXANISOTROPY] = 1; t[D3DTSS_COLORARG0] = t[D3DTSS_ALPHAARG0] = t[D3DTSS_RESULTARG] = D3DTA_CURRENT;
  }
  for (int i = 0; i < 512; i++) { identity(&st.xf[i]); st.xfSet[i] = false; }
  st.vp = {0, 0, pp.BackBufferWidth, pp.BackBufferHeight, 0.0f, 1.0f};
  memset(&st.mat, 0, sizeof st.mat); st.light.clear(); st.lightOn.clear(); st.vs = 0; st.ps = 0; memset(st.clip, 0, sizeof st.clip);
}

// ---- IUnknown and the simple queries ------------------------------------------------------------------------------------
HRESULT Dev::QueryInterface(REFIID r, void **o) {
  if (IsEqualGUID(r, IID_IUnknown) || IsEqualGUID(r, IID_IDirect3DDevice8)) { *o = this; AddRef(); return S_OK; }
  *o = NULL; return E_NOINTERFACE;
}
ULONG Dev::AddRef() { return InterlockedIncrement(&ref); }
ULONG Dev::Release() {
  LONG n = InterlockedDecrement(&ref);
  if (n) return n;
  d8_log("device released: %llu draws, %llu of them not executed", (unsigned long long)draws, (unsigned long long)drawsNotExecuted);
  if (gpu_skin) d8_log("gpu skin totals: %llu jobs, %llu palettes sent, %llu sources made, %llu refused, %llu draws not made", (unsigned long long)skin_jobs,
                       (unsigned long long)skin_pals, (unsigned long long)skin_built, (unsigned long long)skin_refused, (unsigned long long)skin_undrawn);
  g_skin_dev = nullptr;
  if (mipmaps) d8_log("mipmaps totals: levels built whole %llu times, in part %llu times, %.1f ms in all, the longest %.2f ms; %llu textures with made levels, %llu dropped",
                      (unsigned long long)mip_built_t, (unsigned long long)mip_parts_t, mip_ms_t, mip_ms_max_t, (unsigned long long)mip_textures, (unsigned long long)mip_dropped);
  if (draw_delta) d8_log("draw delta totals: %llu draws sent as deltas, %llu in full", (unsigned long long)delta_tot, (unsigned long long)full_tot);
  for (Block *b : blocks) delete b;
  st.vs = 0; defaults();
  release_swap();
  delete link; link = nullptr;
  d3d->Release();
  delete this;
  return 0;
}
// lost devices (DirectX 8.1, IDirect3DDevice8::Reset and TestCooperativeLevel): "If a call to Reset fails, the device
// will be placed in the 'lost' state"; a device that can be reset answers D3DERR_DEVICENOTRESET, and "Present will
// return D3DERR_DEVICELOST if the device is either 'lost' or 'not reset'". This device is windowed and loses nothing
// on its own (no full-screen focus to lose), so it is only ever "not reset", after a failed Reset
HRESULT Dev::TestCooperativeLevel() { return notReset ? D3DERR_DEVICENOTRESET : D3D_OK; }
UINT Dev::GetAvailableTextureMem() { return 512u << 20; }
HRESULT Dev::GetDirect3D(IDirect3D8 **o) { if (!o) return D3DERR_INVALIDCALL; *o = d3d; d3d->AddRef(); return D3D_OK; }
HRESULT Dev::GetDeviceCaps(D3DCAPS8 *c) { if (!c) return D3DERR_INVALIDCALL; d8_caps(c); return D3D_OK; }
HRESULT Dev::GetDisplayMode(D3DDISPLAYMODE *m) { return d3d->GetAdapterDisplayMode(0, m); }
HRESULT Dev::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *p) {
  if (!p) return D3DERR_INVALIDCALL;
  p->AdapterOrdinal = 0; p->DeviceType = D3DDEVTYPE_HAL; p->hFocusWindow = wnd; p->BehaviorFlags = behavior; return D3D_OK;
}
static BOOL g_cursorShown = TRUE;
WINBOOL Dev::ShowCursor(WINBOOL show) { BOOL was = g_cursorShown; g_cursorShown = show; d8_once("cursor", "ShowCursor: kept, the device draws no cursor in milestone 1"); return was; }
void Dev::SetGammaRamp(DWORD, const D3DGAMMARAMP *r) { if (r) gamma = *r; d8_once("gamma", "SetGammaRamp: kept, not applied (windowed)"); }
void Dev::GetGammaRamp(D3DGAMMARAMP *r) { if (r) *r = gamma; }

// ---- frame ------------------------------------------------------------------------------------------------------------
HRESULT Dev::Reset(D3DPRESENT_PARAMETERS *p) { sgen++;
  if (!p) return D3DERR_INVALIDCALL;
  release_swap();
  sync = present_interval(*p, "Reset");
  pp = *p; if (pp.hDeviceWindow) wnd = pp.hDeviceWindow;
  HRESULT hr = make_swap(true);
  if (FAILED(hr)) { notReset = true; return D3DERR_DEVICELOST; }
  notReset = false;
  defaults();
  d8_log("Reset: %ux%u", pp.BackBufferWidth, pp.BackBufferHeight);
  return D3D_OK;
}
HRESULT Dev::Present(const RECT *s, const RECT *d, HWND w, const RGNDATA *) {
  if (notReset) return D3DERR_DEVICELOST;
  if (s || d || w) d8_once("presentargs", "Present with rectangles or another window: they are not used in milestone 1");
  link->put(X_PRESENT, nullptr, 0);
  frame_no++;
  if (interp) interp_present();
  if (ab.on) ab_frame();
  if (snap_frames + 1 >= 600) { d8_rb_log(600);
    if (mipmaps) { d8_log("mipmaps, 600 frames: levels built whole %llu times, in part %llu times, %.1f ms in all, the longest %.2f ms; %llu textures with made levels so far, %llu dropped (rebuilt more than twice their size in 60 frames)",
                          (unsigned long long)mip_built, (unsigned long long)mip_parts, mip_ms, mip_ms_max, (unsigned long long)mip_textures, (unsigned long long)mip_dropped);
                   mip_built = mip_parts = 0; mip_ms = mip_ms_max = 0; } }
  if (++snap_frames >= 600) {
    if (snap_hits + snap_misses)
      d8_log("readback snapshots, %u frames: %u locks of a copied render target read its snapshot (no wait), %u read it "
             "back (the snapshot was not there yet), %u do-not-wait locks were told it was not there yet (no wait)",
             snap_frames, snap_hits, snap_misses, snap_notyet);
    if (gpu_skin) { d8_log("gpu skin, %u frames: %.1f jobs a frame, %.1f palettes sent, %.2f sources made; %llu draws not made so far (0 is right)", snap_frames, (double)(skin_jobs - skin_jobs0) / snap_frames,
                           (double)(skin_pals - skin_pals0) / snap_frames, (double)(skin_built - skin_built0) / snap_frames, (unsigned long long)skin_undrawn);
                    skin_jobs0 = skin_jobs; skin_pals0 = skin_pals; skin_built0 = skin_built; }
    if (draw_delta) { d8_log("draw delta, %u frames: %.1f draws a frame sent as deltas, %.1f in full", snap_frames,
                             (double)delta_n / snap_frames, (double)full_n / snap_frames); delta_n = full_n = 0; }
    if (dt_draw_s || dt_unlock_s) {
      // sampled: the mean of the sampled calls times all calls
      LARGE_INTEGER qf; QueryPerformanceFrequency(&qf); double ms = 1000.0 / (double)qf.QuadPart;
      d8_log("game thread in d3d8on11, %u frames (sampled): the draw methods %.1f a frame, about %.3f ms a frame (%.2f us each); buffer "
             "unlocks that upload %.1f a frame, about %.3f ms a frame (%.2f us each)", snap_frames, (double)dt_draw_n / snap_frames,
             dt_draw_s ? (double)dt_draw_ticks * ms / dt_draw_s * dt_draw_n / snap_frames : 0.0, dt_draw_s ? (double)dt_draw_ticks * ms * 1000.0 / dt_draw_s : 0.0,
             (double)dt_unlock_n / snap_frames, dt_unlock_s ? (double)dt_unlock_ticks * ms / dt_unlock_s * dt_unlock_n / snap_frames : 0.0,
             dt_unlock_s ? (double)dt_unlock_ticks * ms * 1000.0 / dt_unlock_s : 0.0);
      dt_draw_ticks = dt_unlock_ticks = 0; dt_draw_s = dt_draw_n = dt_unlock_s = dt_unlock_n = 0;
    }
    snap_frames = snap_hits = snap_misses = snap_notyet = 0;
  }
  return D3D_OK;
}
// the executor filter's A/B (exec_ab=1): exec_ab_after seconds after the device is made, phases of exec_ab_phase
// seconds in the order off, on, on, off, exec_ab_rounds times, then exec_filter's value again. Each switch is an
// X_OPTION; the executor logs its busy time per phase ("executor ab", in d3d8on11_exec.log in remote mode), and this
// side the game's frame time from Present to Present, leaving out the first exec_ab_settle frames of each phase.
static double now_ms() {
  static LARGE_INTEGER f; if (!f.QuadPart) QueryPerformanceFrequency(&f);
  LARGE_INTEGER t; QueryPerformanceCounter(&t); return (double)t.QuadPart * 1000.0 / (double)f.QuadPart;
}
void Dev::ab_frame() {
  double t = now_ms();
  if (!ab.t0) { ab.t0 = t; ab.last = t; return; }
  double dt = t - ab.last; ab.last = t;
  int n = 4 * ab.rounds;
  if (ab.i >= n) return;
  if (ab.i >= 0 && ++ab.frames > ab.settle) { ab.sum += dt; if (dt > ab.max) ab.max = dt; }
  bool next;
  if (ab.i < 0 && ab.go) {                                 // exec_ab_go: wait for the file (looked for every 60 frames)
    next = ++ab.polls % 60 == 0 && GetFileAttributesA(ab_go_path) != INVALID_FILE_ATTRIBUTES;
    if (next) { DeleteFileA(ab_go_path); d8_log("executor ab: d3d8on11_ab_go.txt found: the A/B starts"); }
  } else next = ab.i < 0 ? t - ab.t0 >= ab.after * 1000.0 : t - ab.ph0 >= ab.phase * 1000.0;
  if (!next) return;
  if (ab.i >= 0) {
    int v = (ab.i % 4 == 1 || ab.i % 4 == 2), k = ab.frames - ab.settle;
    if (k > 0) d8_log("executor ab: phase %d, %s=%d, %d frames: the game's frame %.3f ms on average (%.1f fps), %.3f at most",
                      ab.i + 1, ab.key == OPT_BATCH ? "batch" : ab.key == OPT_FLIP ? "flip" : ab.key == OPT_MERGE ? "merge" : "filter", v, k, ab.sum / k, 1000.0 * k / ab.sum, ab.max);
  }
  ab.i++; ab.ph0 = t; ab.frames = 0; ab.sum = 0; ab.max = 0;
  XOption o = {ab.key, 0, (uint32_t)ab.settle};
  if (ab.i < n) o.value = (ab.i % 4 == 1 || ab.i % 4 == 2);
  else if (ab.key == OPT_BATCH) { o.value = ab.batch != 0; d8_log("executor ab: done; batch=%u from now on (exec_batch)", o.value); }
  else if (ab.key == OPT_MERGE) { o.value = ab.merge != 0; d8_log("executor ab: done; merge=%u from now on (draw_merge)", o.value); }
  else if (ab.key == OPT_FLIP) { o.value = ab.flip != 0; d8_log("executor ab: done; flip=%u from now on (swap_flip)", o.value); }
  else { o.value = ab.filter != 0; d8_log("executor ab: done; filter=%u from now on (exec_filter)", o.value); }
  link->put(X_OPTION, &o, sizeof o);
}
HRESULT Dev::GetBackBuffer(UINT i, D3DBACKBUFFER_TYPE, IDirect3DSurface8 **o) {
  if (i || !o || !backbuf) return D3DERR_INVALIDCALL;
  *o = backbuf; backbuf->AddRef(); return D3D_OK;
}
HRESULT Dev::BeginScene() { if (inScene) return D3DERR_INVALIDCALL; inScene = true; return D3D_OK; }
HRESULT Dev::EndScene() { if (!inScene) return D3DERR_INVALIDCALL; inScene = false; return D3D_OK; }
HRESULT Dev::Clear(DWORD n, const D3DRECT *r, DWORD flags, D3DCOLOR color, float z, DWORD stencil) {
  // "Clears the viewport, or a set of rectangles in the viewport ... Coordinates are clipped to the bounds of the
  // viewport rectangle", and it "fails if you specify the D3DCLEAR_ZBUFFER or D3DCLEAR_STENCIL flags when the render
  // target does not have an attached depth buffer", or STENCIL when the depth format has no stencil (DirectX 8.1,
  // IDirect3DDevice8::Clear)
  if (!(flags & (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL)) || (n && !r) || !curRT) return D3DERR_INVALIDCALL;
  if ((flags & (D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL)) && !curDS) return D3DERR_INVALIDCALL;
  if ((flags & D3DCLEAR_STENCIL) && curDS->fmt != D3DFMT_D24S8 && curDS->fmt != D3DFMT_D15S1 && curDS->fmt != D3DFMT_D24X4S4)
    return D3DERR_INVALIDCALL;
  LONG vx1 = (LONG)st.vp.X, vy1 = (LONG)st.vp.Y;
  LONG vx2 = (LONG)(st.vp.X + st.vp.Width), vy2 = (LONG)(st.vp.Y + st.vp.Height);
  if (vx2 > (LONG)curRT->w) vx2 = curRT->w; if (vy2 > (LONG)curRT->h) vy2 = curRT->h;
  std::vector<XRect> rs;
  if (!r) { if (vx2 > vx1 && vy2 > vy1) rs.push_back({vx1, vy1, vx2, vy2}); }
  else for (DWORD i = 0; i < n; i++) {
    XRect q = {r[i].x1 > vx1 ? r[i].x1 : vx1, r[i].y1 > vy1 ? r[i].y1 : vy1, r[i].x2 < vx2 ? r[i].x2 : vx2, r[i].y2 < vy2 ? r[i].y2 : vy2};
    if (q.x2 > q.x1 && q.y2 > q.y1) rs.push_back(q);
  }
  if (rs.empty()) return D3D_OK;
  // the whole target in one rectangle, and a depth surface (if cleared) of the same size: a view clear does it all
  bool dsSame = !(flags & (D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL)) || (curDS->w == curRT->w && curDS->h == curRT->h);
  if (rs.size() == 1 && rs[0].x1 == 0 && rs[0].y1 == 0 && rs[0].x2 == (LONG)curRT->w && rs[0].y2 == (LONG)curRT->h && dsSame) rs.clear();
  XClear c = {flags, color, z, stencil, curRT->w, curRT->h, (uint32_t)rs.size()};
  link->put(X_CLEAR, &c, sizeof c, rs.data(), (uint32_t)(rs.size() * sizeof(XRect)));
  return D3D_OK;
}

// ---- resources --------------------------------------------------------------------------------------------------------
HRESULT Dev::CreateTexture(UINT w, UINT h, UINT l, DWORD u, D3DFORMAT f, D3DPOOL p, IDirect3DTexture8 **o) {
  if (!o || !w || !h) return D3DERR_INVALIDCALL;
  Tex *t = new Tex(); HRESULT hr = t->c.init(this, w, h, l, 1, u, f, p);
  if (FAILED(hr)) { delete t; *o = NULL; return hr; }
  *o = t; return D3D_OK;
}
HRESULT Dev::CreateCubeTexture(UINT e, UINT l, DWORD u, D3DFORMAT f, D3DPOOL p, IDirect3DCubeTexture8 **o) {
  if (!o || !e) return D3DERR_INVALIDCALL;
  CubeTex *t = new CubeTex(); HRESULT hr = t->c.init(this, e, e, l, 6, u, f, p);
  if (FAILED(hr)) { delete t; *o = NULL; return hr; }
  *o = t; return D3D_OK;
}
HRESULT Dev::CreateVertexBuffer(UINT len, DWORD u, DWORD fvf, D3DPOOL p, IDirect3DVertexBuffer8 **o) {
  if (!o) return D3DERR_INVALIDCALL;
  VB *b = new VB(); HRESULT hr = b->c.init(this, len, u, fvf, D3DFMT_VERTEXDATA, p, false);
  if (FAILED(hr)) { delete b; *o = NULL; return hr; }
  buf_live(b, true);
  *o = b; return D3D_OK;
}
HRESULT Dev::CreateIndexBuffer(UINT len, DWORD u, D3DFORMAT f, D3DPOOL p, IDirect3DIndexBuffer8 **o) {
  if (!o || (f != D3DFMT_INDEX16 && f != D3DFMT_INDEX32)) return D3DERR_INVALIDCALL;
  IB *b = new IB(); HRESULT hr = b->c.init(this, len, u, 0, f, p, true);
  if (FAILED(hr)) { delete b; *o = NULL; return hr; }
  buf_live(b, true);
  *o = b; return D3D_OK;
}
HRESULT Dev::CreateRenderTarget(UINT w, UINT h, D3DFORMAT f, D3DMULTISAMPLE_TYPE ms, WINBOOL lockable, IDirect3DSurface8 **o) {
  if (!o) return D3DERR_INVALIDCALL;
  FmtInfo fi = d8_fmt(f);
  if (ms != D3DMULTISAMPLE_NONE || fi.dxgi == DXGI_FORMAT_UNKNOWN || fi.depth || fi.block) { d8_notimpl("CreateRenderTarget of this format or multisampling"); return D3DERR_INVALIDCALL; }
  Surf *s = new Surf(this, SK_RT, w, h, f, D3DPOOL_DEFAULT, D3DUSAGE_RENDERTARGET); s->lockable = lockable != 0;
  s->id = new_id(); XTex t = {s->id, XT_RENDERTARGET, w, h, 1, 1, (uint32_t)f, D3DUSAGE_RENDERTARGET}; link->put(X_TEX, &t, sizeof t);
  *o = s; return D3D_OK;
}
HRESULT Dev::CreateDepthStencilSurface(UINT w, UINT h, D3DFORMAT f, D3DMULTISAMPLE_TYPE ms, IDirect3DSurface8 **o) {
  if (!o) return D3DERR_INVALIDCALL;
  FmtInfo fi = d8_fmt(f);
  if (ms != D3DMULTISAMPLE_NONE || !fi.depth) { d8_notimpl("CreateDepthStencilSurface of this format or multisampling"); return D3DERR_INVALIDCALL; }
  Surf *s = new Surf(this, SK_DS, w, h, f, D3DPOOL_DEFAULT, D3DUSAGE_DEPTHSTENCIL);
  s->id = new_id(); XTex t = {s->id, XT_DEPTH, w, h, 1, 1, (uint32_t)f, D3DUSAGE_DEPTHSTENCIL}; link->put(X_TEX, &t, sizeof t);
  *o = s; return D3D_OK;
}
HRESULT Dev::CreateImageSurface(UINT w, UINT h, D3DFORMAT f, IDirect3DSurface8 **o) {
  if (!o || d8_fmt(f).dxgi == DXGI_FORMAT_UNKNOWN) return D3DERR_INVALIDCALL;
  Surf *s = new Surf(this, SK_IMAGE, w, h, f, D3DPOOL_SYSTEMMEM, 0);
  s->cpu.assign((size_t)d8_pitch(f, w) * d8_rows(f, h), 0);
  *o = s; return D3D_OK;
}
// the texture a texture level belongs to
static TexCore *core_of(Surf *s) {
  if (s->kind != SK_TEXLEVEL || !s->container) return nullptr;
  IDirect3DBaseTexture8 *t = (IDirect3DBaseTexture8 *)s->container;
  return t->GetType() == D3DRTYPE_CUBETEXTURE ? &((CubeTex *)(IDirect3DCubeTexture8 *)t)->c : &((Tex *)(IDirect3DTexture8 *)t)->c;
}
HRESULT Dev::CopyRects(IDirect3DSurface8 *src, const RECT *rects, UINT n, IDirect3DSurface8 *dst, const POINT *pts) {
  d8_rb_site = __builtin_return_address(0);
  // DirectX 8.1, IDirect3DDevice8::CopyRects: source and destination "must point to a different surface"; "The format
  // of the two surfaces must match"; "this method will fail unless all the source rectangles and their corresponding
  // destination rectangles are completely contained within the source and destination surfaces"; it "cannot be
  // applied to surfaces whose formats are classified as depth stencil formats"; NULL points copy each rectangle "to
  // the same offset"; NULL rectangles copy "the entire surface"
  Surf *s = (Surf *)src, *d = (Surf *)dst;
  if (!s || !d || s == d || s->fmt != d->fmt || d8_fmt(s->fmt).depth) return D3DERR_INVALIDCALL;
  RECT whole = {0, 0, (LONG)s->w, (LONG)s->h}; POINT zero = {0, 0};
  UINT cnt = rects && n ? n : 1;
  for (UINT i = 0; i < cnt; i++) {
    RECT r = rects && n ? rects[i] : whole; POINT p = !(rects && n) ? zero : pts ? pts[i] : POINT{r.left, r.top};
    if (r.left < 0 || r.top < 0 || r.right <= r.left || r.bottom <= r.top || r.right > (LONG)s->w || r.bottom > (LONG)s->h ||
        p.x < 0 || p.y < 0 || p.x + (r.right - r.left) > (LONG)d->w || p.y + (r.bottom - r.top) > (LONG)d->h) return D3DERR_INVALIDCALL;
  }
  for (UINT i = 0; i < cnt; i++) {
    RECT r = rects && n ? rects[i] : whole; POINT p = pts ? pts[i] : POINT{r.left, r.top};
    if (!rects || !n) p = zero;
    bool sg = s->kind != SK_IMAGE && s->id, dg = d->kind != SK_IMAGE && d->id;
    if (sg && dg) {                                           // GPU to GPU
      XCopy c = {s->xref(), d->xref(), r.left, r.top, r.right, r.bottom, p.x, p.y}; link->put(X_COPY, &c, sizeof c);
      d->wseq++;
      // a texture level's CPU copy (what LockRect gives and UnlockRect sends whole) is now older than the GPU's: the
      // next lock reads the level back (d8_res.cpp, TexCore::lock)
      if (TexCore *tc = d->kind == SK_TEXLEVEL ? core_of(d) : nullptr) {
        UINT sub = d->face * tc->levels + d->level;
        if (tc->stale.size() < tc->cpu.size()) tc->stale.resize(tc->cpu.size(), 0);
        tc->stale[sub] = 1;
        if (tc->gen && d->level == 0) { tc->gen = false; sgen++; }   // mipmaps: the made levels no longer hold level 0
        if (tc->pool != D3DPOOL_DEFAULT || (tc->usage & D3DUSAGE_DYNAMIC))
          { char at[MAX_PATH + 16]; d8_once("copystale", "CopyRects from a GPU surface into a lockable texture level: its next LockRect reads it back (waits for the executor); first called from %s", d8_caller(__builtin_return_address(0), at, sizeof at)); }
      }
    } else if (sg && d->kind == SK_IMAGE) {                   // GPU to system memory: a read back
      std::vector<uint8_t> out; XRef ref = s->xref();
      { double t0 = qpc_now_ms(); int rc = link->call(X_READBACK, &ref, sizeof ref, &out); d8_rb_count(qpc_now_ms() - t0); if (rc < 0) return D3DERR_INVALIDCALL; }
      FmtInfo fi = d8_fmt(s->fmt);
      UINT bw = fi.block ? 4 : 1, sw = s->kind == SK_TEXLEVEL ? s->w : s->w;
      UINT sp = d8_pitch(s->fmt, sw), dp = d8_pitch(d->fmt, d->w), rowb = d8_pitch(s->fmt, (UINT)(r.right - r.left)), rows = d8_rows(s->fmt, (UINT)(r.bottom - r.top));
      for (UINT y = 0; y < rows; y++) {
        size_t so = (size_t)(r.top / bw + y) * sp + d8_pitch(s->fmt, (UINT)r.left), dof = (size_t)(p.y / bw + y) * dp + d8_pitch(d->fmt, (UINT)p.x);
        if (so + rowb <= out.size() && dof + rowb <= d->cpu.size()) memcpy(&d->cpu[dof], &out[so], rowb);
      }
    } else if (s->kind == SK_IMAGE && (dg || d->kind == SK_IMAGE)) {   // system memory to the GPU, or to system memory
      FmtInfo fi = d8_fmt(s->fmt);
      UINT bw = fi.block ? 4 : 1;                               // block formats copy whole 4x4 blocks
      if ((r.left | r.top | p.x | p.y) % bw) { d8_once("copyalign", "CopyRects of a block format not on block edges (refused)"); return D3DERR_INVALIDCALL; }
      UINT sp = d8_pitch(s->fmt, s->w), rowb = d8_pitch(s->fmt, (UINT)(r.right - r.left)), rows = d8_rows(s->fmt, (UINT)(r.bottom - r.top));
      const uint8_t *src = s->cpu.data() + (size_t)(r.top / bw) * sp + (size_t)d8_pitch(s->fmt, (UINT)r.left);
      if (d->kind == SK_IMAGE) {
        UINT dp = d8_pitch(d->fmt, d->w);
        for (UINT y = 0; y < rows; y++) memcpy(&d->cpu[(size_t)(p.y / bw + y) * dp + d8_pitch(d->fmt, (UINT)p.x)], src + (size_t)y * sp, rowb);
      } else {
        UINT levels = 1;                                    // the executor's level count (mipmaps: xlevels)
        if (d->kind == SK_TEXLEVEL && d->container) {
          const TexCore &cc = ((IDirect3DBaseTexture8 *)d->container)->GetType() == D3DRTYPE_CUBETEXTURE ? ((CubeTex *)(IDirect3DCubeTexture8 *)d->container)->c : ((Tex *)(IDirect3DTexture8 *)d->container)->c;
          levels = cc.xlevels ? cc.xlevels : cc.levels; }
        std::vector<uint8_t> rowsdata((size_t)rowb * rows);
        for (UINT y = 0; y < rows; y++) memcpy(&rowsdata[(size_t)y * rowb], src + (size_t)y * sp, rowb);
        XUpTex u = {d->id, d->kind == SK_TEXLEVEL ? D3D11CalcSubresource(d->level, d->face, levels) : 0u, (uint32_t)p.x, (uint32_t)p.y,
                    (uint32_t)(r.right - r.left), (uint32_t)(r.bottom - r.top), rowb, rows};
        link->upload_tex(u, rowsdata.data(), d->w, d->h, bw);
        // a texture level's CPU copy is what LockRect reads and UnlockRect sends whole: it gets the rows too
        if (TexCore *tc = d->kind == SK_TEXLEVEL ? core_of(d) : nullptr) {
          std::vector<uint8_t> &lv = tc->cpu[d->face * tc->levels + d->level];
          UINT dp = d8_pitch(d->fmt, d->w);
          if (lv.size() < (size_t)dp * d8_rows(d->fmt, d->h)) lv.resize((size_t)dp * d8_rows(d->fmt, d->h), 0);
          for (UINT y = 0; y < rows; y++) memcpy(&lv[(size_t)(p.y / bw + y) * dp + d8_pitch(d->fmt, (UINT)p.x)], &rowsdata[(size_t)y * rowb], rowb);
          if (tc->mipped && d->level == 0 && d->face == 0) {     // mipmaps: level 0 changed, its levels with it (the part copied)
            RECT dr = {p.x, p.y, p.x + (r.right - r.left), p.y + (r.bottom - r.top)}; tc->dirty(&dr); tc->build_mips(); }
        }
      }
    } else {
      d8_notimpl("CopyRects between these kinds of surface");
      return D3DERR_INVALIDCALL;
    }
  }
  // a lockable render target written only by copies: the executor copies it to a staging texture now and fills its
  // snapshot when the GPU is done, so a later LockRect need not wait (d8_proto.h, X_SNAP; d8_res.cpp, Surf::LockRect)
  if (d->kind == SK_RT && d->lockable && !d->drawn && s->kind != SK_IMAGE &&
      (size_t)d8_pitch(d->fmt, d->w) * d8_rows(d->fmt, d->h) <= SNAP_BYTES) {
    if (d->snap < 0) d->snap = snap_take();
    if (d->snap >= 0) { XSnap q = {d->xref(), (uint32_t)d->snap, d->wseq}; link->put(X_SNAP, &q, sizeof q); }
  }
  return D3D_OK;
}
HRESULT Dev::SetRenderTarget(IDirect3DSurface8 *rt, IDirect3DSurface8 *ds) { sgen++;
  Surf *r = (Surf *)rt, *z = (Surf *)ds;
  if (r && r->kind != SK_RT && r->kind != SK_BACKBUFFER && !(r->kind == SK_TEXLEVEL && (r->usage & D3DUSAGE_RENDERTARGET))) return D3DERR_INVALIDCALL;
  if (z && z->kind != SK_DS) return D3DERR_INVALIDCALL;
  if (r) { r->AddRef(); if (curRT) curRT->Release(); curRT = r; r->drawn = true; }   // D3D8: a NULL target keeps the current one
  if (z) z->AddRef(); if (curDS) curDS->Release(); curDS = z;
  st.vp = {0, 0, curRT->w, curRT->h, 0.0f, 1.0f};                       // D3D8: setting a target resets the viewport
  targets();
  return D3D_OK;
}
HRESULT Dev::GetRenderTarget(IDirect3DSurface8 **o) { if (!o || !curRT) return D3DERR_INVALIDCALL; *o = curRT; curRT->AddRef(); return D3D_OK; }
HRESULT Dev::GetDepthStencilSurface(IDirect3DSurface8 **o) {
  if (!o) return D3DERR_INVALIDCALL;
  *o = curDS; if (!curDS) return D3DERR_NOTFOUND;
  curDS->AddRef(); return D3D_OK;
}

// ---- state (kept for Get*, recorded into a state block between BeginStateBlock and EndStateBlock) -------------------------
// While a block records, a set goes into the block only: the device's state does not change (probe8 checks that).
#define TARGET (rec ? rec->s : st)
HRESULT Dev::SetTransform(D3DTRANSFORMSTATETYPE t, const D3DMATRIX *m) { sgen++;
  if ((UINT)t >= 512 || !m) return D3DERR_INVALIDCALL;
  TARGET.xf[t] = *m; if (rec) rec->xf[t] = true; else st.xfSet[t] = true;
  return D3D_OK;
}
HRESULT Dev::GetTransform(D3DTRANSFORMSTATETYPE t, D3DMATRIX *m) { if ((UINT)t >= 512 || !m) return D3DERR_INVALIDCALL; *m = st.xf[t]; return D3D_OK; }
HRESULT Dev::MultiplyTransform(D3DTRANSFORMSTATETYPE t, const D3DMATRIX *m) { sgen++;
  if ((UINT)t >= 512 || !m) return D3DERR_INVALIDCALL;
  D3DMATRIX a = st.xf[t], o;                                  // D3D8: the new matrix is m times the current one
  for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) {
    float s = 0; for (int k = 0; k < 4; k++) s += m->m[i][k] * a.m[k][j];
    o.m[i][j] = s;
  }
  return SetTransform(t, &o);
}
// user clip planes: kept and read back; drawing with D3DRS_CLIPPLANEENABLE set is logged as not implemented (d8_ff.cpp).
// The game's traces never set CLIPPLANEENABLE (data/d3d8_scope.json)
HRESULT Dev::SetClipPlane(DWORD i, const float *p) { sgen++;
  if (i >= 6 || !p) return D3DERR_INVALIDCALL;
  memcpy(TARGET.clip[i], p, 16); if (rec) rec->clip[i] = true;
  return D3D_OK;
}
HRESULT Dev::GetClipPlane(DWORD i, float *p) { if (i >= 6 || !p) return D3DERR_INVALIDCALL; memcpy(p, st.clip[i], 16); return D3D_OK; }
// texture palettes (DirectX 8.1, IDirect3DDevice8::SetPaletteEntries and SetCurrentTexturePalette): 256 entries each;
// "A single logical palette is associated with the device, and is shared by all texture stages"; without
// D3DPTEXTURECAPS_ALPHAPALETTE "every entry in the palette must have alpha set to 1.0 or this method will fail".
// Kept and read back only: no palettized texture format is implemented, and the game creates none (its formats:
// data/d3d8_scope.json, resources). The current palette's value before any set is not in the reference: 0 here
HRESULT Dev::SetPaletteEntries(UINT n, const PALETTEENTRY *e) { sgen++;
  if (!e) return D3DERR_INVALIDCALL;
  D3DCAPS8 c; GetDeviceCaps(&c);
  if (!(c.TextureCaps & D3DPTEXTURECAPS_ALPHAPALETTE)) for (int i = 0; i < 256; i++) if (e[i].peFlags != 0xFF) return D3DERR_INVALIDCALL;
  palettes[n].assign(e, e + 256);
  return D3D_OK;
}
HRESULT Dev::GetPaletteEntries(UINT n, PALETTEENTRY *e) {
  auto it = palettes.find(n);
  if (!e || it == palettes.end()) return D3DERR_INVALIDCALL;
  memcpy(e, it->second.data(), 256 * sizeof(PALETTEENTRY));
  return D3D_OK;
}
// not among the methods a state block records (DirectX 8.1, BeginStateBlock), so it takes effect while one records;
// a D3DSBT_ALL block holds "the current palette"
HRESULT Dev::SetCurrentTexturePalette(UINT n) { sgen++; st.palette = n; return D3D_OK; }
HRESULT Dev::GetCurrentTexturePalette(UINT *n) { if (!n) return D3DERR_INVALIDCALL; *n = st.palette; return D3D_OK; }
HRESULT Dev::SetViewport(const D3DVIEWPORT8 *v) { sgen++; if (!v) return D3DERR_INVALIDCALL; TARGET.vp = *v; if (rec) rec->vp = true; return D3D_OK; }
HRESULT Dev::GetViewport(D3DVIEWPORT8 *v) { if (!v) return D3DERR_INVALIDCALL; *v = st.vp; return D3D_OK; }
HRESULT Dev::SetMaterial(const D3DMATERIAL8 *m) { sgen++; if (!m) return D3DERR_INVALIDCALL; TARGET.mat = *m; if (rec) rec->mat = true; return D3D_OK; }
HRESULT Dev::GetMaterial(D3DMATERIAL8 *m) { if (!m) return D3DERR_INVALIDCALL; *m = st.mat; return D3D_OK; }
static void grow(State &s, DWORD i) {
  if (s.light.size() <= i) {
    D3DLIGHT8 def = {}; def.Type = D3DLIGHT_DIRECTIONAL; def.Diffuse.r = def.Diffuse.g = def.Diffuse.b = 1.0f; def.Direction.z = 1.0f;
    s.light.resize(i + 1, def); s.lightOn.resize(i + 1, FALSE);       // D3D8: LightEnable on an unset light sets this default
  }
}
HRESULT Dev::SetLight(DWORD i, const D3DLIGHT8 *l) { sgen++;
  if (!l || i > 0xFFFF) return D3DERR_INVALIDCALL;
  State &s = TARGET; grow(s, i); s.light[i] = *l; if (rec) rec->lights.push_back((int)i);
  return D3D_OK;
}
HRESULT Dev::GetLight(DWORD i, D3DLIGHT8 *l) { if (!l || i >= st.light.size()) return D3DERR_INVALIDCALL; *l = st.light[i]; return D3D_OK; }
HRESULT Dev::LightEnable(DWORD i, WINBOOL on) { sgen++;
  if (i > 0xFFFF) return D3DERR_INVALIDCALL;
  State &s = TARGET; grow(s, i); s.lightOn[i] = on ? TRUE : FALSE; if (rec) rec->lights.push_back((int)i);
  return D3D_OK;
}
HRESULT Dev::GetLightEnable(DWORD i, WINBOOL *on) { if (!on || i >= st.lightOn.size()) return D3DERR_INVALIDCALL; *on = st.lightOn[i]; return D3D_OK; }
HRESULT Dev::SetRenderState(D3DRENDERSTATETYPE s, DWORD v) { sgen++;
  if ((UINT)s >= 256) return D3DERR_INVALIDCALL;
  TARGET.rs[s] = v; if (rec) rec->rs[s] = true;
  return D3D_OK;
}
HRESULT Dev::GetRenderState(D3DRENDERSTATETYPE s, DWORD *v) { if ((UINT)s >= 256 || !v) return D3DERR_INVALIDCALL; *v = st.rs[s]; return D3D_OK; }
HRESULT Dev::GetTextureStageState(DWORD stg, D3DTEXTURESTAGESTATETYPE t, DWORD *v) {
  if (stg >= 8 || (UINT)t >= 32 || !v) return D3DERR_INVALIDCALL;
  *v = st.tss[stg][t]; return D3D_OK;
}
HRESULT Dev::SetTextureStageState(DWORD stg, D3DTEXTURESTAGESTATETYPE t, DWORD v) { sgen++;
  if (stg >= 8 || (UINT)t >= 32) return D3DERR_INVALIDCALL;
  TARGET.tss[stg][t] = v; if (rec) rec->tss[stg][t] = true;
  return D3D_OK;
}
// bound objects hold a reference, as D3D8 does
template <class T> static void rebind(T *&slot, T *v) { if (v) v->AddRef(); if (slot) slot->Release(); slot = v; }
HRESULT Dev::SetTexture(DWORD stg, IDirect3DBaseTexture8 *t) { sgen++;
  if (stg >= 8) return D3DERR_INVALIDCALL;
  if (rec) { rebind(rec->s.tex[stg], t); rec->tex[stg] = true; } else rebind(st.tex[stg], t);
  return D3D_OK;
}
HRESULT Dev::GetTexture(DWORD stg, IDirect3DBaseTexture8 **t) {
  if (stg >= 8 || !t) return D3DERR_INVALIDCALL;
  *t = st.tex[stg]; if (*t) (*t)->AddRef(); return D3D_OK;
}
HRESULT Dev::SetStreamSource(UINT n, IDirect3DVertexBuffer8 *b, UINT stride) {
  if (n >= 16) return D3DERR_INVALIDCALL;
  State &s = TARGET; VB *v = (VB *)b; rebind(s.stream[n], v); s.stride[n] = stride; if (rec) rec->stream[n] = true;
  return D3D_OK;
}
HRESULT Dev::GetStreamSource(UINT n, IDirect3DVertexBuffer8 **b, UINT *stride) {
  if (n >= 16 || !b || !stride) return D3DERR_INVALIDCALL;
  *b = st.stream[n]; if (*b) (*b)->AddRef(); *stride = st.stride[n]; return D3D_OK;
}
HRESULT Dev::SetIndices(IDirect3DIndexBuffer8 *b, UINT base) {
  State &s = TARGET; IB *i = (IB *)b; rebind(s.ib, i); s.baseVertex = base; if (rec) rec->ib = true;
  return D3D_OK;
}
HRESULT Dev::GetIndices(IDirect3DIndexBuffer8 **b, UINT *base) {
  if (!b || !base) return D3DERR_INVALIDCALL;
  *b = st.ib; if (*b) (*b)->AddRef(); *base = st.baseVertex; return D3D_OK;
}

// ---- state blocks -----------------------------------------------------------------------------------------------------
static void copy_block(State &to, const State &from, const Block &b) {
  for (int i = 0; i < 256; i++) if (b.rs[i]) to.rs[i] = from.rs[i];
  for (int s = 0; s < 8; s++) for (int t = 0; t < 32; t++) if (b.tss[s][t]) to.tss[s][t] = from.tss[s][t];
  for (int s = 0; s < 8; s++) if (b.tex[s]) rebind(to.tex[s], from.tex[s]);
  for (int i = 0; i < 512; i++) if (b.xf[i]) to.xf[i] = from.xf[i];
  if (b.vp) to.vp = from.vp;
  if (b.mat) to.mat = from.mat;
  for (int i = 0; i < 6; i++) if (b.clip[i]) memcpy(to.clip[i], from.clip[i], 16);
  if (b.palette) to.palette = from.palette;
  for (int n = 0; n < 16; n++) if (b.stream[n]) { rebind(to.stream[n], from.stream[n]); to.stride[n] = from.stride[n]; }
  if (b.ib) { rebind(to.ib, from.ib); to.baseVertex = from.baseVertex; }
  if (b.vs) to.vs = from.vs;
  if (b.ps) to.ps = from.ps;
  for (int i : b.lights) {
    grow(to, i);
    if ((size_t)i < from.light.size()) { to.light[i] = from.light[i]; to.lightOn[i] = from.lightOn[i]; }
  }
}
static Block *new_block() {
  Block *b = new Block(); memset((void *)b, 0, offsetof(Block, lights));
  new (&b->lights) std::vector<int>(); new (&b->s) State();
  memset(b->s.tex, 0, sizeof b->s.tex); memset(b->s.stream, 0, sizeof b->s.stream); b->s.ib = nullptr;
  return b;
}
static void free_block(Block *b) {
  for (int i = 0; i < 8; i++) if (b->s.tex[i]) b->s.tex[i]->Release();
  for (int i = 0; i < 16; i++) if (b->s.stream[i]) b->s.stream[i]->Release();
  if (b->s.ib) b->s.ib->Release();
  delete b;
}
HRESULT Dev::BeginStateBlock() { sgen++; if (rec) return D3DERR_INVALIDCALL; rec = new_block(); return D3D_OK; }
HRESULT Dev::EndStateBlock(DWORD *tok) { sgen++;
  if (!rec || !tok) return D3DERR_INVALIDCALL;
  blocks.push_back(rec); rec = nullptr; *tok = (DWORD)blocks.size();
  return D3D_OK;
}
static Block *find(std::vector<Block *> &v, DWORD t) { return t && t <= v.size() ? v[t - 1] : nullptr; }
HRESULT Dev::ApplyStateBlock(DWORD t) { sgen++;
  Block *b = find(blocks, t);
  if (!b || rec) return D3DERR_INVALIDCALL;                  // D3D8: no apply while another block records
  copy_block(st, b->s, *b);
  return D3D_OK;
}
HRESULT Dev::CaptureStateBlock(DWORD t) { sgen++;
  Block *b = find(blocks, t);
  if (!b || rec) return D3DERR_INVALIDCALL;
  copy_block(b->s, st, *b);
  return D3D_OK;
}
HRESULT Dev::DeleteStateBlock(DWORD t) { sgen++;
  Block *b = find(blocks, t);
  if (!b) return D3DERR_INVALIDCALL;
  free_block(b); blocks[t - 1] = nullptr;
  return D3D_OK;
}
// What each D3DSTATEBLOCKTYPE captures, from Microsoft's DirectX 8.1 runtime itself, not from its documentation: the
// documentation's two lists (D3DSTATEBLOCKTYPE and "Creating Predefined State Blocks") disagree with each other. Measured
// on the retail d3d8.dll with its reference device, one state at a time (test/t8.c sbtypes: set A, create, set B,
// apply, read back), and the same render state and texture stage state numbers are lists in that d3d8.dll's data. ALL
// takes render states 7..173 that d3d8types.h defines except D3DRS_ZVISIBLE (30) and D3DRS_DEBUGMONITORTOKEN (165), every
// texture stage state 1..28 but 12 (not defined), and the index buffer (not in the documentation's list). The pixel and
// vertex blocks: the lists below, the pixel shader (pixel) or the lights, their enables and the vertex shader (vertex);
// neither takes a texture, transform, clip plane, material, viewport, stream, index buffer or palette. Shader constants
// are in all three, but ours keeps none (SetVertexShaderConstant is not implemented: the game sets none)
static const uint8_t SB_RS_ALL[] = {7, 8, 9, 10, 14, 15, 16, 19, 20, 22, 23, 24, 25, 26, 27, 28, 29, 34, 35, 36, 37, 38, 40,
  47, 48, 52, 53, 54, 55, 56, 57, 58, 59, 60, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 139, 140, 141, 142, 143,
  145, 146, 147, 148, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 166, 167, 168, 170, 171, 172, 173};
static const uint8_t SB_RS_PIXEL[] = {7, 8, 9, 10, 14, 15, 16, 19, 20, 23, 24, 25, 26, 27, 36, 37, 38, 40, 47, 52, 53, 54, 55,
  56, 57, 58, 59, 60, 128, 129, 130, 131, 132, 133, 134, 135, 168, 171};
static const uint8_t SB_RS_VERTEX[] = {9, 22, 28, 29, 34, 35, 36, 37, 38, 48, 136, 137, 139, 140, 141, 142, 143, 145, 146, 147,
  148, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 166, 167, 170, 172, 173};
HRESULT Dev::CreateStateBlock(D3DSTATEBLOCKTYPE type, DWORD *tok) { sgen++;
  if (!tok || rec || type < D3DSBT_ALL || type > D3DSBT_VERTEXSTATE) return D3DERR_INVALIDCALL;
  Block *b = new_block();
  bool all = type == D3DSBT_ALL, px = type == D3DSBT_PIXELSTATE, vx = type == D3DSBT_VERTEXSTATE;
  const uint8_t *rs = all ? SB_RS_ALL : px ? SB_RS_PIXEL : SB_RS_VERTEX;
  size_t nrs = all ? sizeof SB_RS_ALL : px ? sizeof SB_RS_PIXEL : sizeof SB_RS_VERTEX;
  for (size_t i = 0; i < nrs; i++) b->rs[rs[i]] = true;
  for (int s = 0; s < 8; s++) for (int t = 1; t <= 28; t++)
    if (t != 12 && (!vx || t == D3DTSS_TEXCOORDINDEX || t == D3DTSS_TEXTURETRANSFORMFLAGS)) b->tss[s][t] = true;
  if (all) {
    memset(b->tex, 1, sizeof b->tex); memset(b->xf, 1, sizeof b->xf); memset(b->stream, 1, sizeof b->stream);
    memset(b->clip, 1, sizeof b->clip); b->vp = b->mat = b->palette = b->ib = true;
  }
  b->vs = !px; b->ps = !vx;
  if (!px) for (size_t i = 0; i < st.light.size(); i++) b->lights.push_back((int)i);
  copy_block(b->s, st, *b);
  blocks.push_back(b); *tok = (DWORD)blocks.size();
  return D3D_OK;
}

// ---- shaders ------------------------------------------------------------------------------------------------------------
static size_t decl_len(const DWORD *d) {                    // tokens up to and with D3DVSD_END
  size_t i = 0;
  for (;;) {
    DWORD t = d[i];
    if (t == D3DVSD_END()) return i + 1;
    DWORD ty = (t & D3DVSD_TOKENTYPEMASK) >> D3DVSD_TOKENTYPESHIFT;
    if (ty == D3DVSD_TOKEN_CONSTMEM) i += 1 + 4 * ((t & D3DVSD_CONSTCOUNTMASK) >> D3DVSD_CONSTCOUNTSHIFT);
    else if (ty == D3DVSD_TOKEN_EXT) i += 1 + ((t & D3DVSD_EXTCOUNTMASK) >> D3DVSD_EXTCOUNTSHIFT);
    else i++;
  }
}
static size_t code_len(const DWORD *c) { size_t i = 0; while (c[i] != 0x0000FFFF) i++; return i + 1; }
HRESULT Dev::CreateVertexShader(const DWORD *decl, const DWORD *code, DWORD *h, DWORD) { sgen++;
  if (!decl || !h) return D3DERR_INVALIDCALL;
  VShader v; v.used = true; v.fvf = 0;
  v.decl.assign(decl, decl + decl_len(decl));
  if (code) v.code.assign(code, code + code_len(code));
  vshaders.push_back(v);
  *h = (DWORD)((vshaders.size() - 1) << 1) | 1;              // D3D8 handles have bit 0 set; FVF codes never do
  return D3D_OK;
}
HRESULT Dev::SetVertexShader(DWORD h) { sgen++;
  if (h & 1) { size_t i = h >> 1; if (i >= vshaders.size() || !vshaders[i].used) return D3DERR_INVALIDCALL; }
  TARGET.vs = h; if (rec) rec->vs = true;
  return D3D_OK;
}
HRESULT Dev::GetVertexShader(DWORD *h) { if (!h) return D3DERR_INVALIDCALL; *h = st.vs; return D3D_OK; }
HRESULT Dev::DeleteVertexShader(DWORD h) { sgen++;
  size_t i = h >> 1;
  if (!(h & 1) || i >= vshaders.size() || !vshaders[i].used) return D3DERR_INVALIDCALL;
  vshaders[i].used = false; vshaders[i].decl.clear(); vshaders[i].code.clear();
  if (st.vs == h) st.vs = 0;
  return D3D_OK;
}

// ---- draws: executed through generated shaders (d8_ff.cpp) ----------------------------------------------------------------
static HRESULT counted(Dev *d, HRESULT hr) { d->draws++; if (hr != D3D_OK) d->drawsNotExecuted++; return hr; }
// the return address of a Draw* method (the game calls them through the vtable: it is the game's call site) as an offset
// in FFXiMain.dll; ~0 when it is not in FFXiMain.dll (a mod's call: speedfix's merged ...UP draws, or no game loaded)
static uint32_t game_site(void *ra) {
  static uintptr_t lo, hi; static bool done;
  if (!done) {
    done = true; HMODULE m = GetModuleHandleA("FFXiMain.dll");
    if (m) { const IMAGE_DOS_HEADER *dos = (const IMAGE_DOS_HEADER *)m; const IMAGE_NT_HEADERS *nt = (const IMAGE_NT_HEADERS *)((const uint8_t *)m + dos->e_lfanew);
             lo = (uintptr_t)m; hi = lo + nt->OptionalHeader.SizeOfImage; }
  }
  uintptr_t a = (uintptr_t)ra;
  return lo && a >= lo && a < hi ? (uint32_t)(a - lo) : ~0u;
}
// a mod whose device hooks are in front of these methods (speedfix) calls this with the game's return address at its
// hook's entry, just before it calls the method: the game's call site when the method's own return address is the mod's.
// speedfix's merged ...UP draws give the first merged draw's (kit/speedfix/sf_device.cpp, flush)
static void *volatile g_ext_site;
extern "C" void __stdcall D8ON11_DrawSite(void *ra) { g_ext_site = ra; }
// the lock flag a mod may pass to IDirect3DSurface8::LockRect of a lockable render target on this device (d8_res.cpp,
// Surf::LockRect): the export's presence tells the mod this d3d8.dll knows the flag
extern "C" unsigned __stdcall D8ON11_LockDoNotWait(void) { return D8ON11_LOCK_DONOTWAIT; }

// ---- gpu skinning (d8_skin.h) ----------------------------------------------------------------------------------------
// One job: two of this device's vertex buffers become stand-ins for a skinned mesh. The static source (64 bytes a
// vertex: both bones' position and normal terms and the two bone numbers) is made once per mesh and variant and kept
// with the mesh's index buffer; it is made again if the job's pointers or counts are not the ones it was made from.
// The bone table goes with the job only when a bone this mesh uses differs from the table last sent.
int Dev::skin(const D8Skin *j) {
  if (!j || j->size != sizeof(D8Skin) || !gpu_skin || j->variant > 1 || !j->src || !j->words || !j->matrices || (j->ndups && !j->dups)) return 0;
  if (!buf_is_live(j->pos) || !buf_is_live(j->nrm) || !buf_is_live(j->key) || j->pos == j->nrm) {
    if (!skin_refused++) d8_log("gpu_skin: a job's buffers are not this device's vertex and index buffers (another layer wraps them?): refused; the caller skins as before");
    return 0;
  }
  VB *vp = (VB *)(IDirect3DVertexBuffer8 *)j->pos, *vn = (VB *)(IDirect3DVertexBuffer8 *)j->nrm; IB *ib = (IB *)(IDirect3DIndexBuffer8 *)j->key;
  if (vp->c.index || vn->c.index || !ib->c.index || !vp->c.id || !vn->c.id || vp->c.locks || vn->c.locks) return 0;
  const uint32_t n = j->count1 + j->count2 + j->ndups;
  if (!n || n > 65536) return 0;
  BufCore::SkinSrc &k = ib->c.skin[j->variant];
  if (!k.id || k.src != j->src || k.words != j->words || k.bonelist != j->bonelist || k.dups != j->dups || k.c1 != j->count1 ||
      k.c2 != j->count2 || k.nd != j->ndups || k.shift != j->shift) {
    std::vector<XSkinVtx> v(n); bool seen[SKIN_BONES] = {}; uint32_t maxb = 0; bool ok = true;
    auto bone = [&](uint16_t w) -> uint16_t {
      uint32_t b = (w >> j->shift) & 0x7F;
      if (j->bonelist) b = j->bonelist[b];
      if (b >= SKIN_BONES) { ok = false; return 0; }
      seen[b] = true; if (b > maxb) maxb = b;
      return (uint16_t)b;
    };
    for (uint32_t i = 0; i < j->count1; i++) {
      const float *f = j->src + 6 * i; XSkinVtx &o = v[i]; memset(&o, 0, sizeof o);
      o.pa[0] = f[0]; o.pa[1] = f[1]; o.pa[2] = f[2]; o.pa[3] = 1; o.na[0] = f[3]; o.na[1] = f[4]; o.na[2] = f[5];
      o.bone[0] = o.bone[1] = bone(j->words[2 * i]);
    }
    for (uint32_t i = 0; i < j->count2; i++) {
      const float *f = j->src + 6 * j->count1 + 14 * i; XSkinVtx &o = v[j->count1 + i]; memset(&o, 0, sizeof o);
      o.pa[0] = f[0]; o.pa[1] = f[2]; o.pa[2] = f[4]; o.pa[3] = f[6]; o.pb[0] = f[1]; o.pb[1] = f[3]; o.pb[2] = f[5]; o.pb[3] = f[7];
      o.na[0] = f[8]; o.na[1] = f[10]; o.na[2] = f[12]; o.nb[0] = f[9]; o.nb[1] = f[11]; o.nb[2] = f[13];
      o.bone[0] = bone(j->words[2 * (j->count1 + i)]); o.bone[1] = bone(j->words[2 * (j->count1 + i) + 1]);
    }
    for (uint32_t i = 0; i < j->ndups; i++) {
      if (j->dups[i] >= j->count1 + j->count2) { ok = false; break; }
      v[j->count1 + j->count2 + i] = v[j->dups[i]];
    }
    if (!ok) { d8_once("skinbone", "gpu_skin: a mesh names a bone past %u or a duplicate past its vertices: refused; the caller skins as before", (unsigned)SKIN_BONES); return 0; }
    if (k.id) destroy_obj(k.id);
    k.id = new_id(); XBuf b = {k.id, n * (uint32_t)sizeof(XSkinVtx), 0u, 0u}; link->put(X_BUF, &b, sizeof b);
    link->upload_buf(k.id, 0, (const uint8_t *)v.data(), n * (uint32_t)sizeof(XSkinVtx), 0);
    k.src = j->src; k.words = j->words; k.bonelist = j->bonelist; k.dups = j->dups; k.c1 = j->count1; k.c2 = j->count2; k.nd = j->ndups; k.shift = j->shift;
    k.maxb = maxb; k.used.clear(); for (uint32_t b2 = 0; b2 <= maxb; b2++) if (seen[b2]) k.used.push_back((uint16_t)b2);
    skin_built++;
  }
  // the palette: sent when a bone this mesh uses is not what the executor has
  bool same = skin_table_n > k.maxb;
  const uint8_t *m = (const uint8_t *)j->matrices;
  if (interp) { interp_frame(); if (i_live) m = (const uint8_t *)interp_palette(j, k.maxb + 1, k.used, vp); }
  for (size_t u = 0; same && u < k.used.size(); u++) same = !memcmp(m + 64u * k.used[u], skin_table.data() + 64u * k.used[u], 64);
  XSkin x = {vp->c.id, vn->c.id, k.id, 0};
  if (same) link->put(X_SKIN, &x, sizeof x);
  else {
    x.nbones = k.maxb + 1; skin_table.resize(64u * SKIN_BONES); memcpy(skin_table.data(), m, 64u * x.nbones); skin_table_n = x.nbones;
    link->put(X_SKIN, &x, sizeof x, m, 64u * x.nbones); skin_pals++;
  }
  vp->c.virt = 1; vp->c.partner = vn->c.id; vn->c.virt = 2; vn->c.partner = vp->c.id;
  skin_jobs++;
  return 1;
}
extern "C" int __stdcall D8ON11_Skin(const D8Skin *j) { return g_skin_dev ? g_skin_dev->skin(j) : 0; }

// ---- interp (17 Nov) --------------------------------------------------------------------------------------------------
// Above 60 fps most frames carry no game time (speedfix frame_step: 0 whole ticks), and the game draws its characters
// where its last tick left them: motion at 60 a second, however many frames are drawn. Here those frames are drawn
// with each skinned character at the blend of its last two ticks, one tick behind the game (the rule speedfix's
// Liminality settled on, lim_alpha): with c the fraction of a tick since the last whole one and s the ticks the last
// tick frame covered, the weight of the newer tick is (s - 1 + c) / s. Nothing of the game's is written: what is
// blended is what d3d8on11 is handed (the bone palette of a skinning job, the world matrix of a draw that reads a
// skinned buffer), so the game's own logic sees what it always saw (the old Liminality blended the game's own fields).
// What is blended is checked to be what the game would draw otherwise:
//   * a track (one mesh of one character) is the job's position buffer, its variant, the bone table's address and its
//     place among the frame's jobs with those three (two characters wearing the same piece share the buffers);
//   * a tick frame's palette is a keyframe; every frame draws the blend of the last two keyframes. Between ticks the game
//     may change a palette too (the owner's 17 Nov log: as many changes as blends; speedfix's frame_step=3 runs some
//     chase loops on the fraction): that is counted and not drawn, it is in the next tick's keyframe. (The first build
//     drew the game's palette on those frames and the blend on the others: what was drawn went back and forth by up to
//     a tick, the jitter and blur the owner saw.) A draw's world matrix the same way;
//   * a track not seen at the tick before has nothing to blend from; a bone or the world moving 4 units or more in a
//     tick (a warp, a zone change) is not blended.
// The blend is linear, entry by entry (for the rotation a tick makes, the difference from a true rotation is small;
// not measured). Everything else (effects, the camera, nameplates) is drawn as the game has it.
static const volatile float *g_ic_whole; static const volatile double *g_ic_carry; static const volatile int *g_ic_mode;
extern "C" void __stdcall D8ON11_InterpClock(const volatile float *whole, const volatile double *carry, const volatile int *mode) {
  g_ic_whole = whole; g_ic_carry = carry; g_ic_mode = mode;
  d8_log("interp: a step clock was given (whole ticks, carry, mode)");
}
void Dev::interp_frame() {
  if (i_read == i_frame) return;
  i_read = i_frame; i_live = false;
  if (!g_ic_whole || !g_ic_carry || !g_ic_mode || !*g_ic_mode) { i_n[0]++; return; }
  float w = *g_ic_whole; double c = *g_ic_carry;
  i_istick = w >= 1.0f; if (i_istick) { i_tick++; i_span = w; i_n[1]++; } else i_n[2]++;
  double sp = i_span >= 1 ? i_span : 1, a = (sp - 1 + c) / sp; i_alpha = a < 0 ? 0.0f : a > 1 ? 1.0f : (float)a;
  i_live = true;
}
static bool far4(const float *a, const float *b) {          // a matrix's translation row moved 4 units or more
  float dx = a[12] - b[12], dy = a[13] - b[13], dz = a[14] - b[14]; return dx * dx + dy * dy + dz * dz >= 16.0f;
}
const float *Dev::interp_palette(const D8Skin *j, uint32_t nb, const std::vector<uint16_t> &used, VB *vp) {
  const float *m = j->matrices;
  char kb[sizeof(void *) * 2 + 8]; void *pk[2] = {j->pos, (void *)j->matrices}; memcpy(kb, pk, sizeof pk); memcpy(kb + sizeof pk, &j->variant, 4);
  std::string k0(kb, sizeof pk + 4);
  uint32_t occ = i_occ[k0]++; if (occ) i_n[3]++;
  std::string key = k0; key.append((const char *)&occ, 4);
  ITrack &t = i_tracks[key]; t.seen = i_frame;
  vp->c.itrack = &t; vp->c.itrack_frame = i_frame; vp->c.iword = 0;
  t.blend = false;
  if (i_istick) {
    t.hasprev = t.tick == i_tick - 1 && t.nb == nb && !t.pc.empty();
    if (t.hasprev) t.pp.swap(t.pc);
    t.pc.assign(m, m + 16 * nb); t.nb = nb; t.tick = i_tick;
  } else {
    if (t.tick != i_tick || t.nb != nb) { i_n[4]++; return m; }                       // not seen at the last tick
    if (memcmp(m, t.pc.data(), 64u * nb)) i_n[5]++;  // the game changed it between ticks: counted, the ticks' blend drawn (below)
  }
  if (!t.hasprev) { i_n[4]++; return m; }
  for (uint16_t b : used) if (far4(&t.pp[16 * b], &t.pc[16 * b])) { i_n[6]++; return m; }
  i_blend.resize(16 * nb); const float a = i_alpha, *p0 = t.pp.data(), *p1 = t.pc.data();
  for (uint32_t i = 0; i < 16 * nb; i++) i_blend[i] = p0[i] + (p1[i] - p0[i]) * a;
  t.blend = true; i_n[7]++;
  return i_blend.data();
}
bool Dev::interp_world(VB *s0, D3DMATRIX *out) {
  if (!interp || !s0 || s0->c.virt != 1 || !s0->c.itrack || s0->c.itrack_frame != i_frame) return false;
  interp_frame(); if (!i_live) return false;
  ITrack &t = *s0->c.itrack; uint32_t o = s0->c.iword++;
  if (o >= 8) { i_n[8]++; return false; }
  const D3DMATRIX &g = st.xf[D3DTS_WORLD];
  if (i_istick) {
    t.whas[o] = t.wtick[o] == i_tick - 1;
    if (t.whas[o]) t.wp[o] = t.wc[o];
    t.wc[o] = g; t.wtick[o] = i_tick;
  } else if (t.wtick[o] != i_tick) { i_n[9]++; return false; }   // this ordinal not drawn at the last tick
  else if (memcmp(&g, &t.wc[o], sizeof g)) i_n[9]++;          // changed between ticks: counted, the ticks' blend drawn
  if (!t.whas[o] || !t.blend) return false;                 // (a track whose palette is not blended keeps the game's world too)
  if (far4(&t.wp[o].m[0][0], &t.wc[o].m[0][0])) { i_n[6]++; return false; }
  const float *a = &t.wp[o].m[0][0], *b = &t.wc[o].m[0][0]; float *r = &out->m[0][0];
  if (!memcmp(a, b, 64)) return false;                      // nothing to blend: the draw as it is (and draw_delta keeps working)
  for (int i = 0; i < 16; i++) r[i] = a[i] + (b[i] - a[i]) * i_alpha;
  i_n[10]++;
  return true;
}
void Dev::interp_present() {
  i_frame++; i_occ.clear();
  if ((i_frame & 1023) == 0) for (auto it = i_tracks.begin(); it != i_tracks.end();) { if (i_frame - it->second.seen > 600) it = i_tracks.erase(it); else ++it; }
  if (i_frame % 600 == 0) {
    d8_log("interp, 600 frames: %llu with a tick, %llu between ticks, %llu with no clock; jobs: %llu blended, %llu not seen at the last tick or with nothing before, "
           "%llu changed by the game between ticks (the ticks' blend drawn), %llu moved 4 units or more in a tick; %llu jobs shared a buffer and bone table "
           "(told apart by order); draws: %llu world matrices blended, %llu changed between ticks or not drawn at the last tick, %llu past 8 draws a job; %u tracks",
           (unsigned long long)i_n[1], (unsigned long long)i_n[2], (unsigned long long)i_n[0], (unsigned long long)i_n[7], (unsigned long long)i_n[4],
           (unsigned long long)i_n[5], (unsigned long long)i_n[6], (unsigned long long)i_n[3], (unsigned long long)i_n[10], (unsigned long long)i_n[9],
           (unsigned long long)i_n[8], (unsigned)i_tracks.size());
    memset(i_n, 0, sizeof i_n);
  }
}
// who calls the draws (9 Nov: draw_sites put every draw under a mod in the owner's game, and the hook order between
// the game, the kit and speedfix there is not traced): each distinct pair (the module this method returns into, the
// module of the address a mod handed D8ON11_DrawSite) is logged once, with the addresses as module+offset; up to 12
static void mod_of(const void *a, char *out, size_t n) {
  HMODULE m = NULL; char path[MAX_PATH] = "";
  if (a && GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)a, &m) && m) {
    GetModuleFileNameA(m, path, sizeof path); const char *b = strrchr(path, '\\');
    snprintf(out, n, "%s+0x%X", b ? b + 1 : path, (unsigned)((uintptr_t)a - (uintptr_t)m));
  } else snprintf(out, n, a ? "%p (no module)" : "none", a);
}
static void note_caller(void *ra) {
  static HMODULE seen[12][2]; static int nseen; static LONG draws;
  LONG d = InterlockedIncrement(&draws);
  if (nseen >= 12) return;
  void *ext = g_ext_site; HMODULE m1 = NULL, m2 = NULL;
  GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)ra, &m1);
  if (ext) GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)ext, &m2);
  for (int i = 0; i < nseen; i++) if (seen[i][0] == m1 && seen[i][1] == m2) return;
  seen[nseen][0] = m1; seen[nseen][1] = m2; nseen++;
  char a[300], b[300]; mod_of(ra, a, sizeof a); mod_of(ext, b, sizeof b);
  d8_log("draw callers: a draw method returns into %s; the call site a mod handed over: %s (draw %ld)", a, b, (long)d);
}
static uint32_t site_of(void *ra) {
  uint32_t s = game_site(ra);
  if (s == ~0u && g_ext_site) s = game_site(g_ext_site);
  g_ext_site = nullptr;
  return s;
}
HRESULT Dev::DrawPrimitive(D3DPRIMITIVETYPE t, UINT start, UINT n) {
  note_caller(__builtin_return_address(0)); draw_site = draw_sites ? site_of(__builtin_return_address(0)) : ~0u;
  return counted(this, ff_draw(0, t, n, start, 0, 0, 0, NULL, 0, NULL, D3DFMT_UNKNOWN));
}
HRESULT Dev::DrawIndexedPrimitive(D3DPRIMITIVETYPE t, UINT minIndex, UINT numVerts, UINT startIndex, UINT n) {
  note_caller(__builtin_return_address(0)); draw_site = draw_sites ? site_of(__builtin_return_address(0)) : ~0u;
  return counted(this, ff_draw(1, t, n, 0, minIndex, numVerts, startIndex, NULL, 0, NULL, D3DFMT_UNKNOWN));
}
HRESULT Dev::DrawPrimitiveUP(D3DPRIMITIVETYPE t, UINT n, const void *v, UINT stride) {
  note_caller(__builtin_return_address(0)); draw_site = draw_sites ? site_of(__builtin_return_address(0)) : ~0u;
  if (!v || !stride) return D3DERR_INVALIDCALL;
  return counted(this, ff_draw(2, t, n, 0, 0, 0, 0, v, stride, NULL, D3DFMT_UNKNOWN));
}
HRESULT Dev::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE t, UINT minIndex, UINT numVerts, UINT n, const void *i, D3DFORMAT f, const void *v, UINT stride) {
  note_caller(__builtin_return_address(0)); draw_site = draw_sites ? site_of(__builtin_return_address(0)) : ~0u;
  if (!v || !i || !stride) return D3DERR_INVALIDCALL;
  return counted(this, ff_draw(3, t, n, 0, minIndex, numVerts, 0, v, stride, i, f));
}
