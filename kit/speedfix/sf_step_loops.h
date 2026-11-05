// sf_step_loops.h -- written by pipeline/frame_step_loops.py --header (do not edit): the calls of the step
// getter whose value is a loop count, the loop a chase (x += (target - x) * k a pass; data/frame_step_loops.json),
// by return address (call site + 5), analysed build; and where each loop's factor k comes from.
#pragma once
#include <stdint.h>
static const uint32_t kStepLoopRet[] = {
  0x100205B6u,   // call at 0x100205B1 in 0x1001EE60
  0x100C487Du,   // call at 0x100C4878 in 0x100C47F0
  0x100C65BFu,   // call at 0x100C65BA in 0x100C63D0
  0x100C6887u,   // call at 0x100C6882 in 0x100C63D0
  0x100C6C69u,   // call at 0x100C6C64 in 0x100C63D0
};
struct StepLoopOperand { uint32_t at; uint8_t op[2]; uint32_t constant; };   // the instruction, its opcode, its constant
static const StepLoopOperand kStepLoopOperand[] = {
  {0x100205CDu, {0xD9, 0x05}, 0x10329CE4u},   // 0.25, loop of 0x100205B1
  {0x100C65EEu, {0xD8, 0x0D}, 0x1032A3BCu},   // 0.125, loop of 0x100C65BA
  {0x100C65FFu, {0xD8, 0x0D}, 0x1032A3BCu},   // 0.125, loop of 0x100C65BA
  {0x100C660Du, {0xD8, 0x0D}, 0x1032A3BCu},   // 0.125, loop of 0x100C65BA
};
static const uint32_t kStepLoopScaleCall[] = {   // `call 0x100272B0` (E8 rel32) in a chase loop
  0x100C490Bu,   // loop of 0x100C4878, factor push 0x3e000000
  0x100C6994u,   // loop of 0x100C6882, factor push ebp
  0x100C6D76u,   // loop of 0x100C6C64, factor push ebp
};
// linear per-frame moves (frame_step_loops.py LINEAR_RET): the fraction under frame_step=3
static const uint32_t kStepLinearRet[] = {0x1001F024u, 0x1001F0F7u};
// integer-count chases (frame_step_loops.py INT_LOOPS, FOLLOW_*): the getter calls answered ceil(s) under
// frame_step=3, and the 0x100272B0 calls in them that get k' (sf_scale_k)
static const uint32_t kStepIntLoopRet[] = {0x1001F5A7u, 0x1002014Du, 0x1001F670u, 0x1001F716u, 0x1001FA3Du, 0x1001FE81u};
static const uint32_t kStepIntLoopScaleCall[] = {0x1001F5D9u, 0x1002017Cu, 0x1001FDAFu};
// the camera follow (frame_step_loops.py FOLLOW_*): its chase call (two forms: sf_follow_k), its 0x100272E0
// calls (sf_scale3_k), its fmul [0.125] (the 0.125 factor's float), the hold timer's fsub [1.0], the counter's
// mov dword ptr [0x10456D8C], 4
static const uint32_t kStepFollowCall = 0x1001FD5Au;
static const uint32_t kStepScale3Call[] = {0x1001F6D1u, 0x1001FCD3u};
static const StepLoopOperand kStepIntLoopOperand[] = {
  {0x1001FE36u, {0xD8, 0x0D}, 0x1032A3BCu},   // 0.125
  {0x1001FE46u, {0xD8, 0x0D}, 0x1032A3BCu},   // 0.125
};
static const StepLoopOperand kStepTickDec = {0x1001FA55u, {0xD8, 0x25}, 0x1032961Cu};   // 1
static const uint32_t kStepHoldSet = 0x1001FB19u, kStepHoldVar = 0x10456D8Cu; static const int kStepHold = 4;
static const uint32_t kStepFollowFar = 0x10329A08u;   // the far form's factor (0.5)
// the walk/run rate 0x100C8930 (frame_step_loops.py RATE_*): its getter calls (return addresses) and the
// per-frame smoothing and standing test, made per-tick below one tick a frame
static const uint32_t kStepRateRet[] = {0x100C8ACDu, 0x100C8AECu};
enum { RATE_NEW, RATE_OLD, RATE_THR };
struct StepRateOperand { uint32_t at; uint8_t op[2]; uint32_t constant; int kind; };
static const StepRateOperand kStepRateOperand[] = {
  {0x100C8B3Eu, {0xD8, 0x0D}, 0x10329CE4u, RATE_NEW},   // 0.25
  {0x100C8B36u, {0xD8, 0x0D}, 0x10329D34u, RATE_OLD},   // 0.75
  {0x100C8A74u, {0xD8, 0x1D}, 0x1032A1A8u, RATE_THR},   // 0.0001
};
