// FORK OF: Camera_GetClampedField28 0x10014D20 (from tree/src/unit_10014350_Camera.cpp)
// Edit the body below; `fork.py diff` shows the edit, `fork.py gate` runs it against the game's code.
// Hook it from the mod's xikit_mod_init with XI_FORK(host, Camera_GetClampedField28).
#include "gfx_all.hpp"
#include "xi_ghidra.hpp"     // the Ghidra decompiler's types and helpers (float10, NAN, ...)

#include "../sf_step_sites.h"
#include "../sf_step_loops.h"

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
// called with the fraction a loop is about to count, so that its factors match it (speedfix: sf_main.cpp); none here
extern "C" { __attribute__((weak)) void (*volatile xi_step_loop_hook)(float); }
// the step rounded up (the step is 0 to 20, the cap Frame_Render's fork keeps: no overflow); the forks build freestanding
static inline float xi_step_ceil(float s) { float c = (float)(int)s; return c < s ? c + 1.0f : c; }
static inline bool xi_step_fraction_site(uintptr_t ret)
{
  for (unsigned i = 0; i < sizeof kStepFractionRet / sizeof kStepFractionRet[0]; i++)
    if (ret == XI_GAME(kStepFractionRet[i])) return true;
  return false;
}
// the answer with the switches on, for the call returning to `ret`
static long double xi_step_answer(uintptr_t ret)
{
  // 3: as 2, and the calls whose value is a chase loop's count (sf_step_loops.h) get the fraction too; speedfix
  // gives those loops the factor that makes ceil(fraction) passes do what the fraction of a tick does (sf_main.cpp)
  if (xi_step_mode == 3)
    for (unsigned i = 0; i < sizeof kStepLoopRet / sizeof kStepLoopRet[0]; i++)
      if (ret == XI_GAME(kStepLoopRet[i])) { if (xi_step_loop_hook) xi_step_loop_hook(xi_step_frac); return xi_step_frac; }
  if (xi_step_mode == 3)                      // the integer-count chases (the camera): ceil(s) passes, given k' by speedfix
    for (unsigned i = 0; i < sizeof kStepIntLoopRet / sizeof kStepIntLoopRet[0]; i++)
      if (ret == XI_GAME(kStepIntLoopRet[i])) { if (xi_step_loop_hook) xi_step_loop_hook(xi_step_frac); return xi_step_ceil(xi_step_frac); }
  if (xi_step_mode == 3)                      // the camera's linear per-frame moves (yaw, height): the fraction
    for (unsigned i = 0; i < sizeof kStepLinearRet / sizeof kStepLinearRet[0]; i++)
      if (ret == XI_GAME(kStepLinearRet[i])) return xi_step_frac;
  if (xi_step_mode == 3)                      // the walk/run rate's expected movement (0x100C8930): the fraction too
    for (unsigned i = 0; i < sizeof kStepRateRet / sizeof kStepRateRet[0]; i++)
      if (ret == XI_GAME(kStepRateRet[i])) { if (xi_step_loop_hook) xi_step_loop_hook(xi_step_frac); return xi_step_frac; }
  return xi_step_mode >= 2 && xi_step_fraction_site(ret) ? xi_step_frac : xi_step_whole;
}

// FUNCTION: FFXIMAIN 0x10014D20  (unit unit_10014350_Camera: no unit evidence of its own; in the gap between 0x10014330 (CAtelIdleTask) and 0x10017750 (CShader) -- an unnamed source file)
// GATE: PASS -- 100 trials, 100 returned normally, 0 D3D calls compared (diffemu_gfx, clean build)
XI_API long double __cdecl Camera_GetClampedField28(void)
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
