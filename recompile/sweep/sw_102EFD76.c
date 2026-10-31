/* 0x102EFD76 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's (ghidra_frameset18), outside the tree; the bytes are its proof. Compiled /O1 (size). */
// VC6FLAGS: /O1
#include "vc6_shim.h"
// FUNCTION: FFXIMAIN 0x102EFD76
undefined4 __cdecl fn_102EFD76(int param_1, int param_2, double * param_3)
{
  if ((((param_1 != 0) && (param_2 != 0)) && ((*(byte *)(param_2 + 8) & 1) != 0)) &&
     (param_3 != (double *)0x0)) {
    *param_3 = (double)(float10)*(float *)(param_2 + 0x28);
    return 1;
  }
  return 0;
}