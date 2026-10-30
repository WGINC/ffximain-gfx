/* 0x10311BBB at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
undefined8 __cdecl FUN_10312FA8(unsigned int, unsigned int);
// FUNCTION: FFXIMAIN 0x10311BBB
undefined8 __cdecl Allocator_Generic(uint32_t param_1)
{
  undefined8 uVar1;
  
  uVar1 = FUN_10312FA8(param_1, 1);
  return uVar1;
}