// sf_step_sites.h -- written by pipeline/frame_step_users.py --header (do not edit): the calls of the frame-step
// getters (0x10014CF0, 0x10014D20) answered with the fractional step under frame_step=2, by return address
// (call site + 5), analysed build. Why each: data/frame_step_users.json.
#pragma once
#include <stdint.h>
static const uint32_t kStepFractionRet[] = {
  0x1001A9B0u,   // call at 0x1001A9AB in 0x1001A8E0
  0x1001B5D9u,   // call at 0x1001B5D4 in 0x1001B5D0
  0x1001C5B4u,   // call at 0x1001C5AF in 0x1001C260
  0x100CDF9Bu,   // call at 0x100CDF96 in 0x100CDF90
  0x1012C17Eu,   // call at 0x1012C179 in 0x1012C0F0
  0x1012CBD4u,   // call at 0x1012CBCF in 0x1012CAE0
  0x10178BD5u,   // call at 0x10178BD0 in 0x10178B30
  0x10188A59u,   // call at 0x10188A54 in 0x10188A50
  0x101EDF51u,   // call at 0x101EDF4C in 0x101EDEE0
  0x101EFA2Cu,   // call at 0x101EFA27 in 0x101EF990
  0x10227E5Du,   // call at 0x10227E58 in 0x10227BF0
  0x1024DE9Du,   // call at 0x1024DE98 in 0x1024DE20
  0x1024DF53u,   // call at 0x1024DF4E in 0x1024DE20
  0x1024F6EBu,   // call at 0x1024F6E6 in 0x1024F5E0
};
