// SPDX-License-Identifier: GPL-3.0-only
//
// sf_game.cpp -- the part of the speedfix mod that is built against the reconstruction's headers (xikit_sdk.hpp): the
// mod's exports and the XI_HOOK of Occlusion_QueryScreenRect, whose type the compiler checks against the tree's
// declaration. The rest is in sf_main.cpp and sf_device.cpp (the tree's headers declare their own Windows and D3D
// types, so they are not mixed with <windows.h> or <d3d8.h> in one file).
#include "xikit_sdk.hpp"
#include "speedfix.h"
XIKIT_MOD_GLOBALS

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

// skip_shadow_pass (sf_device.cpp, head): the pass runs from Model_BeginOffscreenPass returning non-zero to the end of
// Model_EndOffscreenPassBindTextures; its four callers (CYyModelDt_PrepareRenderState, CYyModelDt_SkinAndDrawV2, two
// each) call End only when Begin returned non-zero (tree/src/CYyModelDt.cpp). Both run the game's code.
static xikit::fn<decltype(&Model_BeginOffscreenPass)> prev_begin;
static xikit::fn<decltype(&Model_EndOffscreenPassBindTextures)> prev_end;
static char __thiscall sf_begin(int self_, int *a2, int a3) {
  sf_shadow_skip = sf_cfg.skip_shadow_pass && sf_device_level() >= 1 && sf_hooks_live();
  char r = prev_begin(self_, a2, a3);
  if (!r) sf_shadow_skip = 0;
  return r;
}
static char __thiscall sf_end(_DWORD *self_, int a2) {
  char r = prev_end(self_, a2);
  sf_shadow_skip = 0;
  return r;
}
extern "C" int sf_hook_shadow(const XikitHost *host) {
  int a = XI_HOOK(host, Model_BeginOffscreenPass, sf_begin, prev_begin);
  if (a != XIKIT_OK) return a;
  return XI_HOOK(host, Model_EndOffscreenPassBindTextures, sf_end, prev_end);
}

extern "C" __declspec(dllexport) int xikit_mod_init(const XikitHost *host) { return sf_init(host); }
extern "C" __declspec(dllexport) void xikit_mod_frame(void *device) { (void)device; sf_frame(); }
