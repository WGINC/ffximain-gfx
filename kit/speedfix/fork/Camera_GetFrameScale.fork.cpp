// FORK OF: Camera_GetFrameScale 0x10014CF0 (from tree/src/unit_10014350_Camera.cpp)
// Edit the body below; `fork.py diff` shows the edit, `fork.py gate` runs it against the game's code.
// Hook it from the mod's xikit_mod_init with XI_FORK(host, Camera_GetFrameScale).
#include "gfx_all.hpp"
#include "xi_ghidra.hpp"     // the Ghidra decompiler's types and helpers (float10, NAN, ...)

#include "../sf_step_sites.h"

// ---- frame_step (speedfix; the Frame_Render fork makes the step): with xi_step_mode 1 every call is answered with
// the whole ticks of the untruncated step (xi_step_whole); with 2, the calls in sf_step_sites.h (return addresses in
// the analysed build, translated with XI_GAME) get the step itself (xi_step_frac). Before the Frame_Render fork has
// made a step (xi_step_have 0), or with xi_step_mode 0, this is the game's function. The state is defined weak here,
// strong in Frame_Render.fork.cpp, so this fork gates alone; XI_FRAMESTEP_GATE starts it on, with a step of 0.
#ifdef XI_FRAMESTEP_GATE
extern "C" { __attribute__((weak)) volatile int xi_step_mode = 1; __attribute__((weak)) volatile int xi_step_have = 1;
             __attribute__((weak)) float xi_step_whole = 0.0f; __attribute__((weak)) float xi_step_frac = 0.5f; }
#else
extern "C" { __attribute__((weak)) volatile int xi_step_mode; __attribute__((weak)) volatile int xi_step_have;
             __attribute__((weak)) float xi_step_whole; __attribute__((weak)) float xi_step_frac; }
#endif
static inline bool xi_step_fraction_site(uintptr_t ret)
{
  for (unsigned i = 0; i < sizeof kStepFractionRet / sizeof kStepFractionRet[0]; i++)
    if (ret == XI_GAME(kStepFractionRet[i])) return true;
  return false;
}
// the answer with the switches on, for the call returning to `ret`
static long double xi_step_answer(uintptr_t ret)
{
  return xi_step_mode == 2 && xi_step_fraction_site(ret) ? xi_step_frac : xi_step_whole;
}
// the same answer for sf_fork.cpp's register-keeping wrapper of 0x10014CF0, which passes its caller's return address
extern "C" long double __cdecl xi_step_answer_cf0(uintptr_t ret) { return xi_step_answer(ret); }

// FUNCTION: FFXIMAIN 0x10014CF0  (unit unit_10014350_Camera: no unit evidence of its own; in the gap between 0x10014330 (CAtelIdleTask) and 0x10017750 (CShader) -- an unnamed source file)
// GATE: PASS -- 100 trials, 100 returned normally, 0 D3D calls compared (diffemu_gfx, clean build)
XI_API long double __cdecl Camera_GetFrameScale(void)
{
  if ( xi_step_mode != 0 && xi_step_have != 0 )
    return xi_step_answer((uintptr_t)__builtin_return_address(0));
  uint8_t bVar1;
  
  bVar1 = NAN((long double)*(float *)(g_pCamera_as_s32 + 0x28)) || NAN((long double)1.0f);
  if ((bVar1 || (long double)*(float *)(g_pCamera_as_s32 + 0x28) < (long double)1.0f) != bVar1) {
    return (long double)1.0f;
  }
  return (long double)*(float *)(g_pCamera_as_s32 + 0x28);
}
