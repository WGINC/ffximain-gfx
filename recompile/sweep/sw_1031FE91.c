/* 0x1031FE91 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
#define DAT_103d2c60 (*(undefined4 *)0x103D2C60u)
// FUNCTION: FFXIMAIN 0x1031FE91
undefined4 __cdecl fn_1031FE91(int param_1)
{
  int *piVar1;
  int iVar2;
  
  iVar2 = 0;
  piVar1 = &DAT_103d2c60;
  do {
    if (*piVar1 == param_1) {
      return *(undefined4 *)(iVar2 * 8 + 0x103d2c64);
    }
    piVar1 = piVar1 + 2;
    iVar2 = iVar2 + 1;
  } while ((int)piVar1 < 0x103d2d38);
  return 0;
}