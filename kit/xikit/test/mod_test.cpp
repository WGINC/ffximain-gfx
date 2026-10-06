// mod_test.cpp -- the mod test_xikit.py's "mod" run loads (host.c): one hook with a trampoline, one refused hook,
// a frame callback. Built with sdk/build_mod.sh like any mod.
#include "xikit_sdk.hpp"
XIKIT_MOD_GLOBALS

static xikit::fn<decltype(&CXiSkeletonActor::GetColor)> prev_GetColor;
// the game's GetColor returns this+0x660 (host.c); the hook adds 1 to what the game's own code returned
static unsigned int __thiscall my_GetColor(CXiSkeletonActor *self) { return prev_GetColor(self) + 1; }
// a pass-through hook whose trampoline copies a rel32 call (manifest T=9:5): host.c checks the copy's call target
static xikit::fn<decltype(&CDx::EndClearBegin)> prev_EndClearBegin;
static int __thiscall my_EndClearBegin(CDx *self, int a, int b, int c, int d, int e, int f) { return prev_EndClearBegin(self, a, b, c, d, e, f); }

extern "C" {
__declspec(dllexport) volatile long mod_frames, mod_unknown_result, mod_init_rel;
__declspec(dllexport) void *mod_prev, *mod_prev_ecb;
__declspec(dllexport) int xikit_mod_init(const XikitHost *host) {
  host->log("init: api %u, game at %p", host->version, (void *)host->game_base);
  mod_init_rel = (long)xi_rel;                               // the kit set it before calling init
  int r = XI_HOOK(host, CXiSkeletonActor::GetColor, my_GetColor, prev_GetColor);
  mod_prev = (void *)prev_GetColor;
  r |= XI_HOOK(host, CDx::EndClearBegin, my_EndClearBegin, prev_EndClearBegin);
  mod_prev_ecb = (void *)prev_EndClearBegin;
  mod_unknown_result = host->hook("No::SuchFunction", (void *)my_GetColor, nullptr);
  return r;
}
__declspec(dllexport) void xikit_mod_frame(void *device) { (void)device; mod_frames++; }
}
