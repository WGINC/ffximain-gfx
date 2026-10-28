/* 0x1030FF33 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); the bytes are its proof. */
#include "vc6_shim.h"
#define dword_10456A2C (*(int *)0x10456A2Cu)
// FUNCTION: FFXIMAIN 0x1030FF33
undefined4 __stdcall fn_1030FF33(void)
{
  undefined4 uVar1;
  
                    /* WARNING: Could not recover jumptable at 0x1030ff38. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  uVar1 = (**(code **)(dword_10456A2C + 0x29c))();
  return uVar1;
}