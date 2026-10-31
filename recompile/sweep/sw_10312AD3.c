/* 0x10312AD3 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
undefined4 __cdecl FUN_10312B1C(unsigned int, unsigned int, unsigned int);
// FUNCTION: FFXIMAIN 0x10312AD3
undefined4 __cdecl fn_10312AD3(undefined4 param_1)
{
  undefined4 uVar1;
  
  uVar1 = FUN_10312B1C(param_1, 0, 4);
  return uVar1;
}