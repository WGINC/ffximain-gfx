/* 0x102EEF5A at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
// FUNCTION: FFXIMAIN 0x102EEF5A
void __cdecl fn_102EEF5A(int param_1)
{
  int *piVar1;
  
  *(undefined4 *)(param_1 + 0xc4) = 0;
  *(undefined4 *)(param_1 + 0x7c) = 0;
  *(undefined4 *)(param_1 + 0x178) = 0;
  piVar1 = (int *)(param_1 + 400);
  *(undefined1 *)(*piVar1 + 0x50) = 0;
  *(undefined1 *)(*piVar1 + 0x51) = 0;
  *(undefined4 *)(*piVar1 + 0x58) = 0;
  return;
}