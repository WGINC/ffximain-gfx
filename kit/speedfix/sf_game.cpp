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

// shadow_fit (16 Nov). Shadow_DrawByMode (0x10033DA0) draws the simple shadow (shadow mode [0x1045B77A] = 2, and the
// word [0x1045B814] not 1) with XiZone_DrawGroundShadowQuad at [0x1045B810] / 3: a quad of +-1 in x and z (its vertices,
// 0x10377360) scaled by that in x and z, so the shadow's half-width is [0x1045B810] / 3. That number is one for every
// character: 1.0 from the constructor (0x10033710), then whatever 0x100CBDE0 last wrote from one character's light-space
// extent. Here, for each character, it is set for the call from that character's own box and put back after: the eight
// world-space corners at actor +0x918 (16 bytes apart; OnMove 0x100C63D0 writes them each tick from the box its model
// returns, transformed by the actor's world matrix at 0x100C7AE4). The pointer the function gets is actor + 0x674 + 72:
// its only caller is DrawModel (0x1002B6E0, at 0x1002C211, Shadow_DrawByMode(self + 72, ...)), whose only callers are
// the two at 0x100CCDC1 and 0x100CCDF5 in 0x100CBDE0, both with this = actor + 0x674 (lea ecx, [esi+0x674]; esi the
// actor whose OnMove wrote the corners). (17 Nov: the first build took actor = pointer - 72, so it read 0x674 bytes
// into the actor; the owner's log had diagonals up to 151 units and the ground went black under the giant shadows.)
// A diagonal outside 0.05 to 20 units keeps the game's size (counted): a guard against a box that is not there, a
// chosen range, not a measured one. The footprint's diagonal (the longest x-z distance between two corners)
// does not change as the character turns. The half-width is shadow_fit_scale/100 of it: 46 is a starting value, not
// measured (the log's diagonals say what each start saw; the scale is to be set from them).
static xikit::fn<decltype(&Shadow_DrawByMode)> prev_shadow;
static volatile long sh_fit_n, sh_game_n; static float sh_dmin = 1e30f, sh_dmax, sh_dsum;
static undefined4 __thiscall sf_shadow(void *self_, float *pos) {
  if (sf_cfg.shadow_fit && self_) {
    int16_t mode = *(volatile int16_t *)XI_GAME(0x1045B77Au), w = *(volatile int16_t *)XI_GAME(0x1045B814u);
    if (mode == 2 && w != 1) {
      const float *c = (const float *)((const char *)self_ - 72 - 0x674 + 0x918);
      float d2 = 0;
      for (int i = 0; i < 8; i++) for (int j = i + 1; j < 8; j++) {
        float dx = c[4 * i] - c[4 * j], dz = c[4 * i + 2] - c[4 * j + 2], e = dx * dx + dz * dz;
        if (e > d2) d2 = e;
      }
      if (d2 >= 0.05f * 0.05f && d2 <= 20.0f * 20.0f) {   // a box there (otherwise the game's size: no box, or the guard)
        float d; __asm__("fsqrt" : "=t"(d) : "0"(d2));   // (x87 fsqrt: the gate links no C library)
        volatile float *g = (volatile float *)XI_GAME(0x1045B810u); float keep = *g;
        *g = 3.0f * d * (float)sf_cfg.shadow_fit_scale / 100.0f;
        undefined4 r = prev_shadow(self_, pos);
        *g = keep;
        sh_fit_n++; sh_dsum += d; if (d < sh_dmin) sh_dmin = d; if (d > sh_dmax) sh_dmax = d;
        return r;
      }
      sh_game_n++;
    }
  }
  return prev_shadow(self_, pos);
}
extern "C" int sf_hook_shadow(const XikitHost *host) { return XI_HOOK(host, Shadow_DrawByMode, sf_shadow, prev_shadow); }
extern "C" void sf_shadow_take(long *n, long *g, float *dmin, float *dmax, float *dsum) {   // for sf_main.cpp's line
  *n = sh_fit_n; *g = sh_game_n; *dmin = sh_dmin; *dmax = sh_dmax; *dsum = sh_dsum;
  sh_fit_n = sh_game_n = 0; sh_dmin = 1e30f; sh_dmax = 0; sh_dsum = 0;
}
