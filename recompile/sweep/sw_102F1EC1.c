/* 0x102F1EC1 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
// FUNCTION: FFXIMAIN 0x102F1EC1
void __cdecl fn_102F1EC1(int param_1, int param_2, undefined4 param_3)
{
  if ((param_1 != 0) && (param_2 != 0)) {
    *(byte *)(param_2 + 9) = *(byte *)(param_2 + 9) | 8;
    *(undefined1 *)(param_2 + 0x2c) = (undefined1)param_3;
  }
  return;
}