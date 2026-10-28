/* 0x1030FFC2 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); the bytes are its proof. */
#include "vc6_shim.h"
#define dword_10456A2C (*(int *)0x10456A2Cu)
// FUNCTION: FFXIMAIN 0x1030FFC2
undefined4 __stdcall fn_1030FFC2(void)
{
  undefined4 uVar1;
  
                    /* WARNING: Could not recover jumptable at 0x1030ffc7. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  uVar1 = (**(code **)(dword_10456A2C + 0x2d0))();
  return uVar1;
}