/* 0x10315E7F at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
uint32_t __cdecl FUN_10315E96(unsigned int, unsigned int, unsigned int, unsigned int);
// FUNCTION: FFXIMAIN 0x10315E7F
undefined4 __cdecl fn_10315E7F(undefined4 param_1, undefined4 param_2, undefined4 param_3)
{
  uint uVar1;
  
  uVar1 = FUN_10315E96(param_1, param_2, 0x40, param_3);
  return uVar1;
}