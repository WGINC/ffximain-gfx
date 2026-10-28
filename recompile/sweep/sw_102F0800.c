/* 0x102F0800 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); the bytes are its proof. */
#include "vc6_shim.h"
// FUNCTION: FFXIMAIN 0x102F0800
void __cdecl fn_102F0800(int param_1)
{
  *(byte *)(param_1 + 0x61) = *(byte *)(param_1 + 0x61) | 0x10;
  return;
}