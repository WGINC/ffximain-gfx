/* 0x10323C79 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
undefined8 __cdecl FUN_1031CFD6(unsigned int);
uint32_t * __cdecl FUN_10323CC9(unsigned int, unsigned int, unsigned int);
undefined8 __cdecl FUN_1031D037(unsigned int);
// FUNCTION: FFXIMAIN 0x10323C79
undefined4 __cdecl fn_10323C79(uint32_t * param_1, undefined4 param_2)
{
  uint *puVar1;
  
  FUN_1031CFD6(0xc);
  puVar1 = FUN_10323CC9(0, param_1, param_2);
  FUN_1031D037(0xc);
  return puVar1;
}