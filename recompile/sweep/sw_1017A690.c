/* 0x1017A690 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); the bytes are its proof. */
#include "vc6_shim.h"
// FUNCTION: FFXIMAIN 0x1017A690
void __stdcall fn_1017A690(int param_1)
{
  int iVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 *puVar4;
  
  iVar3 = 0x100;
  puVar2 = (undefined4 *)(param_1 + 4);
  do {
    if (((puVar2[-1] != 0) && (puVar4 = (undefined4 *)*puVar2, puVar4 != (undefined4 *)0x0)) &&
       (*(short *)(puVar4 + 1) = *(short *)(puVar4 + 1) + -1, *(short *)(puVar4 + 1) == 0)) {
      for (iVar1 = 0x1c; iVar1 != 0; iVar1 = iVar1 + -1) {
        *puVar4 = 0;
        puVar4 = puVar4 + 1;
      }
    }
    puVar2 = puVar2 + 0x13;
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  return;
}