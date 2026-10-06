// mod_test.cpp -- the mod test_xikit.py's "mod" runs load (host.c): one hook with a trampoline, one refused hook,
// a frame callback, two device hooks (one passing on, one answering by itself) and one refused device hook. Built
// with sdk/build_mod.sh like any mod, with a fork of CXiSkeletonActor::SetColor (sdk/fork.py new, unedited);
// built again with -DMOD_GEN=2 for the reload run (its GetColor adds 2).
#include "xikit_sdk.hpp"
XIKIT_MOD_GLOBALS
#ifndef MOD_GEN
#define MOD_GEN 1
#endif

static xikit::fn<decltype(&CXiSkeletonActor::GetColor)> prev_GetColor;
// the game's GetColor returns this+0x660 (host.c); the hook adds MOD_GEN to what the game's own code returned
static unsigned int __thiscall my_GetColor(CXiSkeletonActor *self) { return prev_GetColor(self) + MOD_GEN; }
// a pass-through hook whose trampoline copies a rel32 call (manifest T=9:5): host.c checks the copy's call target
static xikit::fn<decltype(&CDx::EndClearBegin)> prev_EndClearBegin;
static int __thiscall my_EndClearBegin(CDx *self, int a, int b, int c, int d, int e, int f) { return prev_EndClearBegin(self, a, b, c, d, e, f); }

extern "C" {
__declspec(dllexport) volatile long mod_frames, mod_unknown_result, mod_init_rel, mod_dev_result, mod_bad_slot_result, mod_unloaded;
__declspec(dllexport) volatile long mod_begins;          // BeginScene calls the device hook saw
__declspec(dllexport) volatile long mod_fork_result;
__declspec(dllexport) void *mod_fork_code;
__declspec(dllexport) void *mod_prev, *mod_prev_ecb;
}

static XI_DEVICE_FN(BeginScene) prev_BeginScene;
static int __stdcall my_BeginScene(IDirect3DDevice8 *dev) { mod_begins += MOD_GEN; return prev_BeginScene(dev); }
static XI_DEVICE_FN(TestCooperativeLevel) prev_TestCooperativeLevel;
static int __stdcall my_TestCooperativeLevel(IDirect3DDevice8 *dev) { (void)dev; return 7; }   // never reaches the device
static XI_DEVICE_FN(CreateTexture) prev_CreateTexture;    // passes on: the resource log names the caller, not the mod
static int __stdcall my_CreateTexture(IDirect3DDevice8 *dev, unsigned w, unsigned h, unsigned lv, unsigned us, D3DFORMAT f, D3DPOOL p, IDirect3DTexture8 **t) {
  return prev_CreateTexture(dev, w, h, lv, us, f, p, t);
}

extern "C" {
__declspec(dllexport) int xikit_mod_init(const XikitHost *host) {
  host->log("init: api %u, game at %p, build %d", host->version, (void *)host->game_base, MOD_GEN);
  mod_init_rel = (long)xi_rel;                               // the kit set it before calling init
  int r = XI_HOOK(host, CXiSkeletonActor::GetColor, my_GetColor, prev_GetColor);
  mod_prev = (void *)prev_GetColor;
  r |= XI_HOOK(host, CDx::EndClearBegin, my_EndClearBegin, prev_EndClearBegin);
  mod_prev_ecb = (void *)prev_EndClearBegin;
  mod_unknown_result = host->hook("No::SuchFunction", (void *)my_GetColor, nullptr);
  mod_dev_result = XI_DEVICE_HOOK(host, BeginScene, my_BeginScene, prev_BeginScene)
                 | XI_DEVICE_HOOK(host, TestCooperativeLevel, my_TestCooperativeLevel, prev_TestCooperativeLevel)
                 | XI_DEVICE_HOOK(host, CreateTexture, my_CreateTexture, prev_CreateTexture);
  mod_bad_slot_result = host->device_hook("BeginScene", 35, (void *)my_BeginScene, nullptr);   // 35 is EndScene
  mod_fork_result = XI_FORK(host, CXiSkeletonActor::SetColor);        // the fork's own definition (SetColor.fork.cpp)
  mod_fork_code = ::xikit::code_of(&CXiSkeletonActor::SetColor);
  return r;
}
__declspec(dllexport) void xikit_mod_frame(void *device) { (void)device; mod_frames++; }
__declspec(dllexport) void xikit_mod_unload(void) { mod_unloaded = 1; }
}
