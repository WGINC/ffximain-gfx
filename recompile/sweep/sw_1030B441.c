/* 0x1030B441 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
// FUNCTION: FFXIMAIN 0x1030B441
void __cdecl fn_1030B441(undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, int param_5)
{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)(**(code **)(param_5 + 0x20))(*(undefined4 *)(param_5 + 0x28),1,0x1c);
  if (puVar1 != (undefined4 *)0x0) {
    *puVar1 = 0;
    *(undefined1 *)(puVar1 + 4) = (undefined1)param_1;
    *(undefined1 *)((int)puVar1 + 0x11) = (undefined1)param_2;
    puVar1[5] = param_3;
    puVar1[6] = param_4;
  }
  return;
}