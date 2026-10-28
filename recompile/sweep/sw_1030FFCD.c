/* 0x1030FFCD at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); the bytes are its proof. */
#include "vc6_shim.h"
#define dword_10456A2C (*(int *)0x10456A2Cu)
// FUNCTION: FFXIMAIN 0x1030FFCD
undefined4 __stdcall fn_1030FFCD(void)
{
  undefined4 uVar1;
  
                    /* WARNING: Could not recover jumptable at 0x1030ffd2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  uVar1 = (**(code **)(dword_10456A2C + 0x2d4))();
  return uVar1;
}