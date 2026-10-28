// SPDX-License-Identifier: GPL-3.0-only
//
// sf_fork.cpp -- speedfix's forks of the game's functions (built against the reconstruction's headers, like
// sf_game.cpp, but apart from it: the gate of sf_game.cpp's hook links that file alone, without the forks).
#include <stddef.h>
#include "xikit_sdk.hpp"
#include "speedfix.h"

// skin_merge: the fork of SkinAndDrawV2 (fork/CYyModelDt__SkinAndDrawV2.fork.cpp, gated with fork.py gate --declare
// fork/skinmerge.declare.json); with xi_skinmerge_on 0 it is the game's function (the gate without declarations)
extern "C" int sf_skinmerge_installed;
int sf_skinmerge_installed;
// skin_merge=2: the fork runs between sf_group_begin and sf_group_end (sf_device.cpp, the grouping of a model's draws)
extern "C" { void sf_group_begin(void); void sf_group_end(void); }
static __int16 __attribute__((thiscall)) sfg_wrap(CYyModelDt *self, int a2, int a3, int a4, int a5, _DWORD *a6, _DWORD *a7) {
  sf_group_begin();
  __int16 r = self->SkinAndDrawV2(a2, a3, a4, a5, a6, a7);
  sf_group_end();
  return r;
}
extern "C" int sf_fork_skinmerge(const XikitHost *host) {
  int r = sf_cfg.skin_merge >= 2 && ::xikit::code_of(&CYyModelDt::SkinAndDrawV2)
        ? host->hook("CYyModelDt::SkinAndDrawV2", (void *)sfg_wrap, nullptr) : XI_FORK(host, CYyModelDt::SkinAndDrawV2);
  sf_skinmerge_installed = r == XIKIT_OK;
  return r;
}


// frame_step and uncap: Frame_Render (fork/Frame_Render.fork.cpp) and the getters 0x10014CF0, 0x10014D20
// (fork/Camera_GetFrameScale.fork.cpp, fork/Camera_GetClampedField28.fork.cpp), gated with fork.py gate (switches off, no
// declarations) and with --declare fork/framestep*.declare.json (switches on); xi_step_mode and xi_uncap set by
// sf_main.cpp each frame. All three or none: the getters answer what the Frame_Render fork makes.
//
// 0x10014CF0 is manifest flag 8: on every path it writes only AX (fnstsw ax) and ECX ([0x104568FC]), so bytes 2-3 of
// its entry EAX leave as they came, and callers read them (data/eax_kept_sites.json: 13 sites; e.g. 0x1006A000 is the
// last call before 0x10069F90 returns). C++ cannot promise that, so it is hooked through hook_keeping_registers with
// sf_cf0_wrap: switches off, it jumps to the game's own code; on, it runs the game's code first (a pure read of
// [g_pCamera+0x28]: it writes no memory), keeps the EAX, ECX, EDX and flags that left, drops its ST0 and loads the
// fork's answer (xi_step_answer_cf0: the same function the fork's own switches-on path returns) for the caller's return
// address. Every register but ST0 is then the game's. (The x87 condition bits C0, C2, C3 are undefined after FLD,
// the game's last instruction on both paths: Intel SDM, FLD.)
extern "C" { void *sf_cf0_prev; extern volatile int xi_step_mode, xi_step_have; void sf_cf0_wrap(void);
             long double __cdecl xi_step_answer_cf0(uintptr_t ret); }
__asm__(
  ".text\n.globl _sf_cf0_wrap\n_sf_cf0_wrap:\n"
  "  cmpl $0, _xi_step_mode\n  je 1f\n"
  "  cmpl $0, _xi_step_have\n  je 1f\n"
  "  call *_sf_cf0_prev\n"                       // the game's 0x10014CF0: EAX, ECX, flags as the game leaves them
  "  fstp %st(0)\n"
  "  pushfl\n  pushl %eax\n  pushl %ecx\n  pushl %edx\n"
  "  pushl 16(%esp)\n"                          // the caller's return address
  "  call _xi_step_answer_cf0\n  addl $4, %esp\n"
  "  popl %edx\n  popl %ecx\n  popl %eax\n  popfl\n  ret\n"
  "1:\n  jmp *_sf_cf0_prev\n");

extern "C" int sf_framestep_installed;
int sf_framestep_installed;
extern "C" int sf_fork_framestep(const XikitHost *host) {
  if (host->version < 3 || host->size < offsetof(XikitHost, hook_keeping_registers) + sizeof(void *)) {
    host->log("speedfix: frame_step needs kit API version 3 (hook_keeping_registers); this kit is version %u", host->version);
    return XIKIT_E_OLDKIT;
  }
  int r = host->hook_keeping_registers("Camera_GetFrameScale", (void *)sf_cf0_wrap, &sf_cf0_prev);
  if (r == XIKIT_OK) r = XI_FORK(host, Camera_GetClampedField28);
  if (r == XIKIT_OK) r = XI_FORK(host, Frame_Render);
  sf_framestep_installed = r == XIKIT_OK;
  return r;
}
