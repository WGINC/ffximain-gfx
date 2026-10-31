/* 0x102F764D at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
undefined8 __cdecl FUN_10312EAD(unsigned int);
// FUNCTION: FFXIMAIN 0x102F764D
void __cdecl fn_102F764D(int param_1, LPVOID param_2)
{
  if ((param_1 != 0) && (param_2 != (LPVOID)0x0)) {
    FUN_10312EAD(param_2);
  }
  return;
}