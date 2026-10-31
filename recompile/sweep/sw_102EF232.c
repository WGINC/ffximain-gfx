/* 0x102EF232 at the game's bytes: pipeline/vc6/sweep.py's source of it under rule retvoid, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's (ghidra_frameset18), outside the tree; the bytes are its proof. Compiled /O1 (size). */
// VC6FLAGS: /O1
#include "vc6_shim.h"
// FUNCTION: FFXIMAIN 0x102EF232
void __cdecl fn_102EF232(int param_1, undefined4 param_2)
{
  undefined4 uVar1;
  
  uVar1 = 0;
  if (*(code **)(param_1 + 0x44) != (code *)0x0) {
    uVar1 = (**(code **)(param_1 + 0x44))(param_1,param_2);
  }
  return;
}