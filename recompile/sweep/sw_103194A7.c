/* 0x103194A7 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
// FUNCTION: FFXIMAIN 0x103194A7
undefined8 __cdecl fn_103194A7(int * param_1)
{
  *param_1 = *param_1 + 8;
  return *(undefined8 *)(*param_1 + -8);
}