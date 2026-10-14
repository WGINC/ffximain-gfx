// SPDX-License-Identifier: GPL-3.0-only
//
// sf_game.cpp -- the part of the speedfix mod that is built against the reconstruction's headers (xikit_sdk.hpp): the
// mod's exports and the XI_HOOK of Occlusion_QueryScreenRect, whose type the compiler checks against the tree's
// declaration. The rest is in sf_main.cpp and sf_device.cpp (the tree's headers declare their own Windows and D3D
// types, so they are not mixed with <windows.h> or <d3d8.h> in one file).
#include "xikit_sdk.hpp"
#include "speedfix.h"
XIKIT_MOD_GLOBALS
extern "C" uintptr_t sf_game_addr(uint32_t a) { return XI_GAME(a); }

extern "C" volatile long sf_aq_skipped, sf_aq_run;

// skip_actor_occlusion (sf_main.cpp, head): 256 for the two actor paths, the game's function for everything else
static xikit::fn<decltype(&Occlusion_QueryScreenRect)> prev_query;
static int __thiscall sf_query(__int16 *q, float *rect, int a3, int a4, int a5, int a6, int a7, int a8, char a9, int a10) {
  uintptr_t ra = (uintptr_t)__builtin_return_address(0);
  if (ra == XI_GAME(0x100CC68Du) || ra == XI_GAME(0x100CD016u)) {       // the calls at 0x100CC688 and 0x100CD011
    if (sf_cfg.skip_actor_occlusion && sf_hooks_live()) { sf_aq_skipped++; return 256; }
    sf_aq_run++;
  }
  return prev_query(q, rect, a3, a4, a5, a6, a7, a8, a9, a10);
}
extern "C" int sf_hook_query(const XikitHost *host) { return XI_HOOK(host, Occlusion_QueryScreenRect, sf_query, prev_query); }

extern "C" __declspec(dllexport) int xikit_mod_init(const XikitHost *host) { return sf_init(host); }
extern "C" __declspec(dllexport) void xikit_mod_frame(void *device) { (void)device; sf_frame(); }
