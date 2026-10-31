/* 0x102F7101 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
undefined4 __cdecl FUN_102F701C(unsigned int, unsigned int, unsigned int, unsigned int);
// FUNCTION: FFXIMAIN 0x102F7101
void __cdecl fn_102F7101(undefined4 param_1, char * param_2, undefined4 param_3)
{
  FUN_102F701C(param_1, 0xf, param_2, param_3);
  return;
}