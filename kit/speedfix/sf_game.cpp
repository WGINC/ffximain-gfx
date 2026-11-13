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
static volatile long sh_fit_n, sh_game_n, sh_br[6]; static float sh_dmin = 1e30f, sh_dmax, sh_dsum;
// The branches of Shadow_DrawByMode (unit_100310A0.cpp, 0x10033DA0), counted (18 Nov: the owner saw some shadows keep
// the small size; every mode-2 call had been sized, so the round shadows of other branches are counted and sized too):
//   0 mode 2, the ground quad at [0x1045B810] / 3 (0x10033DE6)             -- sized
//   1 mode 0 with [0x1045B80C] not under 20 (or NaN): the same quad (0x10033E94) -- sized
//   2 mode 0 with it under 20: the projected round shadow at [0x1045B810] / 3 (0x10033E76) -- sized, at twice the number (21 Nov, below) (19 Nov: the
//     owner saw shadows shrink as he came near them, and the log had 5,298-30,230 of these a minute; DrawProjectedShadow
//     0x10184FE0 scales its texture matrix by 1 / that size, so it is the same size as the quad's)
//   3 mode 1: the decal (0x10033E2F)                                        -- the game's
//   4 the word [0x1045B814] = 1 (modes 0 and 2 draw nothing), or another mode
static undefined4 __thiscall sf_shadow(void *self_, float *pos) {
  if (sf_cfg.shadow_fit && self_) {
    int16_t mode = *(volatile int16_t *)XI_GAME(0x1045B77Au), w = *(volatile int16_t *)XI_GAME(0x1045B814u);
    float cd = *(volatile float *)XI_GAME(0x1045B80Cu);
    int br = mode == 2 && w != 1 ? 0 : mode == 0 && w != 1 ? (!(cd < 20.0f) ? 1 : 2) : mode == 1 ? 3 : 4;
    sh_br[br]++;
    if (br <= 2) {
      const float *c = (const float *)((const char *)self_ - 72 - 0x674 + 0x918);
      float d2 = 0;
      for (int i = 0; i < 8; i++) for (int j = i + 1; j < 8; j++) {
        float dx = c[4 * i] - c[4 * j], dz = c[4 * i + 2] - c[4 * j + 2], e = dx * dx + dz * dz;
        if (e > d2) d2 = e;
      }
      if (d2 >= 0.05f * 0.05f && d2 <= 20.0f * 20.0f) {   // a box there (otherwise the game's size: no box, or the guard)
        float d; __asm__("fsqrt" : "=t"(d) : "0"(d2));   // (x87 fsqrt: the gate links no C library)
        volatile float *g = (volatile float *)XI_GAME(0x1045B810u); float keep = *g;
        // branch 2 (the projected shadow, a caster under 20 away) twice the number: DrawProjectedShadow (0x10184FE0)
        // maps the world to the texture as u = (x - px) / s + 0.5 (s = [0x1045B810] / 3, the 0.5s at 0x103773C0), so
        // the texture spans s; XiZone_DrawGroundShadowQuad spreads the same texture (CTsZoneMap +0x343E4) over the
        // quad at 0x10377360, x and z from -s to +s with u, v from 0 to 1: 2s. With one number the near shadow is
        // half the far one's width, and halves when its caster comes within 20 (21 Nov: the owner, still smaller near)
        *g = (br == 2 ? 2.0f : 1.0f) * 3.0f * d * (float)sf_cfg.shadow_fit_scale / 100.0f;
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
extern "C" void sf_shadow_branches(long *b) { for (int i = 0; i < 6; i++) { b[i] = sh_br[i]; sh_br[i] = 0; } }
extern "C" int sf_hook_shadow(const XikitHost *host) { return XI_HOOK(host, Shadow_DrawByMode, sf_shadow, prev_shadow); }
extern "C" void sf_shadow_take(long *n, long *g, float *dmin, float *dmax, float *dsum) {   // for sf_main.cpp's line
  *n = sh_fit_n; *g = sh_game_n; *dmin = sh_dmin; *dmax = sh_dmax; *dsum = sh_dsum;
  sh_fit_n = sh_game_n = 0; sh_dmin = 1e30f; sh_dmax = 0; sh_dsum = 0;
}

// name_smooth (21 Nov). The owner: names pulse as one moves, in the game as shipped too (dgVoodoo). CXiActor::NameCalc
// (0x100830C0) sets the name's size from its depth as Unit10082F60_ProjectThroughCamera (0x10082FB0) leaves it: the
// post-projection z times 4096, truncated by _ftol (0x10311C2C) to an integer; under [0x103581D4] (4020 in 6AE818B4)
// the size is 1, over [0x103581D8] (4100) the name is not drawn, between them (4100 - depth) / 80. So a name's size
// moves in 80 steps of 1/80 over that range (Actor_DrawWorldText 0x10086210 multiplies the glyph size by it). With
// name_smooth=1 the same size is made from the depth before the truncation: the game's own function runs first, then
// the hook repeats the projection with the same game functions in the same order and, only when the truncation of its
// depth equals the integer the game stored (a check that it is the same number), replaces the size by
// (4100 - depth) / 80 unrounded, kept between 0 and 1. Otherwise the game's size stays (counted).
static xikit::fn<decltype(&CXiActor::NameCalc)> prev_namecalc;
static volatile long nc_smooth, nc_kept;
static uint32_t __thiscall sf_namecalc(void *self, undefined4 *pos, int out, float *scale, uint32_t *color) {
  uint32_t r = prev_namecalc(self, pos, out, scale, color);
  if (!sf_cfg.name_smooth || !scale || !out || !pos) return r;
  int32_t *o = (int32_t *)out; uint32_t nearz = *(volatile uint32_t *)XI_GAME(0x103581D4u), farz = *(volatile uint32_t *)XI_GAME(0x103581D8u);
  uint32_t zi = (uint32_t)o[2];
  if (farz <= nearz || zi > farz || zi < nearz) return r;              // hidden, or size 1: nothing to smooth
  alignas(16) uint8_t m[64]; float v[4];
  orig::Unit10026C70_ReturnThis_v1(m);
  orig::Vec3_Copy_v1(v, pos); v[3] = 1.0f;
  orig::Mat4_Copy_v1(m, orig::Camera_GetViewMatrixPtr_v1());
  orig::Mat4_TransformPointInPlace_v2(m, v);
  if (!(v[2] < 0.0f)) { nc_kept++; return r; }
  orig::Vec3_Copy_v1(v, pos);
  orig::Mat4_PostMultiply_v1(m, orig::Camera_GetMatrix154Ptr_v1());
  orig::Mat4_TransformCoordInPlace_v1(m, v);
  if (!(v[3] >= 1.0f)) { nc_kept++; return r; }
  long double zf = (long double)v[2] * 4096.0L;
  if ((int32_t)zf != o[2]) { nc_kept++; return r; }                    // not the game's number: its size stays
  long double sz = ((long double)farz - zf) / (long double)(int32_t)(farz - nearz);
  *scale = (float)(sz < 0 ? 0 : sz > 1 ? 1 : sz);
  nc_smooth++;
  return r;
}
extern "C" void sf_name_take(long *s, long *k) { *s = nc_smooth; *k = nc_kept; nc_smooth = 0; nc_kept = 0; }
extern "C" int sf_hook_name(const XikitHost *host) { return XI_HOOK(host, CXiActor::NameCalc, sf_namecalc, prev_namecalc); }

