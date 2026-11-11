// SPDX-License-Identifier: GPL-3.0-only
//
// sf_skin.cpp -- speedfix's gpu_skin: the job the fork of SkinAndDrawV2 hands to d3d8on11 (apart from sf_fork.cpp, which
// is built against the reconstruction's headers and cannot include windows.h).
#include <windows.h>
#include <stdint.h>
#include <string.h>
#include "speedfix.h"
#include "../d3d8on11/d8_skin.h"
#define XI_GAME(a) sf_game_addr((uint32_t)(a))

// gpu_skin: one model's skinning handed to d3d8on11 (kit/d3d8on11/d8_skin.h) from the fork of SkinAndDrawV2. What the
// job is made of, each read as Model_SkinVerticesCpu (0x1002F1D0, tree/src/unit_1002F130.cpp) reads it:
//   the counts            dword 0x1045B618 (count1 | count2 << 16; SkinAndDrawV2 sets it from **(self+0x42)), and the
//                         first u16 at *(self+0x42), which the routine uses for its offsets: they must agree
//   the bone words        *(self+0x48), 4 bytes a vertex; the bone list *(self+0x3C) when word self+0x32 has bit 7
//   the source vertices   *(v10+0x6E); the mirrored copy's *(v10+0x76), bone numbers from bits 7..13 (self+0x34 bit 0)
//   the duplicates        *(v10+0x5C), u16 each, *(v10+0x60) of them
//   the bone matrices     [0x10457550], 64 bytes a bone
//   the buffers           v10+0x44 / +0x48 (positions, normals), mirrored v10+0x50 / +0x54; the index buffer v10+0x40
// Not taken (the game's own skinning runs): the routine's second route ([0x1045B774] non-zero), a d3d8.dll without
// D8ON11_Skin, or a job the device refuses. If the mirrored job is refused after the first was taken, the game's
// locks write the first pair again, which ends its stand-in.
extern "C" { volatile long sf_gs_jobs, sf_gs_refused; int sf_gpuskin_api; }
typedef int (__stdcall *PFN_SKIN)(const D8Skin *);
static PFN_SKIN gs_fn(void) {
  static bool done; static PFN_SKIN f;
  if (!done) { done = true; HMODULE m = GetModuleHandleA("d3d8.dll"); f = m ? (PFN_SKIN)(void *)GetProcAddress(m, "D8ON11_Skin") : NULL; sf_gpuskin_api = f != NULL; }
  return f;
}
// for the host test (sf_host.c): the function to hand jobs to, in place of d3d8.dll's
static PFN_SKIN gs_test;
extern "C" __declspec(dllexport) void sf_gpu_skin_test(void *fn) { gs_test = (PFN_SKIN)fn; }
extern "C" __declspec(dllexport) int sf_gpu_skin(void *self_, int v10_) {
  PFN_SKIN f = gs_test ? gs_test : gs_fn();
  if (!f) return 0;
  const uint8_t *s = (const uint8_t *)self_, *v10 = (const uint8_t *)(uintptr_t)(unsigned)v10_;
  if (*(void **)XI_GAME(0x1045B774u)) { __sync_fetch_and_add(&sf_gs_refused, 1); return 0; }
  uint32_t g = *(uint32_t *)XI_GAME(0x1045B618u), c1 = g & 0xFFFF, c2 = g >> 16;
  const uint16_t *cnt = *(const uint16_t *const *)(s + 0x42);
  if (!cnt || cnt[0] != c1) { __sync_fetch_and_add(&sf_gs_refused, 1); return 0; }
  D8Skin j; memset(&j, 0, sizeof j); j.size = sizeof j;
  j.key = *(void *const *)(v10 + 0x40);
  j.count1 = c1; j.count2 = c2;
  j.words = *(const uint16_t *const *)(s + 0x48);
  j.bonelist = (*(const uint16_t *)(s + 0x32) & 0x80) ? *(const uint16_t *const *)(s + 0x3C) : NULL;
  j.dups = *(const uint16_t *const *)(v10 + 0x5C); j.ndups = *(const uint32_t *)(v10 + 0x60);
  j.matrices = *(const float *const *)XI_GAME(0x10457550u);
  j.pos = *(void *const *)(v10 + 0x44); j.nrm = *(void *const *)(v10 + 0x48); j.variant = 0; j.shift = 0;
  j.src = *(const float *const *)(v10 + 0x6E);
  if (!f(&j)) { __sync_fetch_and_add(&sf_gs_refused, 1); return 0; }
  if (s[0x34] & 1) {
    j.pos = *(void *const *)(v10 + 0x50); j.nrm = *(void *const *)(v10 + 0x54); j.variant = 1; j.shift = 7;
    j.src = *(const float *const *)(v10 + 0x76);
    if (!f(&j)) { __sync_fetch_and_add(&sf_gs_refused, 1); return 0; }
  }
  __sync_fetch_and_add(&sf_gs_jobs, 1);
  return 1;
}

