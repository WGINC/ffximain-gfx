/* 0x10320244 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
undefined8 __cdecl FUN_1031CFD6(unsigned int);
undefined4 __cdecl FUN_10320265(unsigned int);
undefined8 __cdecl FUN_1031D037(unsigned int);
// FUNCTION: FFXIMAIN 0x10320244
undefined4 __cdecl fn_10320244(int * param_1)
{
  undefined4 uVar1;
  
  FUN_1031CFD6(0xb);
  uVar1 = FUN_10320265(param_1);
  FUN_1031D037(0xb);
  return uVar1;
}