/* 0x102F68EF at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's (ghidra_frameset18), outside the tree; the bytes are its proof. Compiled /O1 (size). */
// VC6FLAGS: /O1
#include "vc6_shim.h"
// FUNCTION: FFXIMAIN 0x102F68EF
void __cdecl fn_102F68EF(int param_1)
{
  if (*(int *)(param_1 + 4) != 0) {
    (**(code **)(*(int *)(param_1 + 4) + 0x28))(param_1);
  }
  *(undefined4 *)(param_1 + 4) = 0;
  *(undefined4 *)(param_1 + 0x10) = 0;
  return;
}