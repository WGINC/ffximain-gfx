/* 0x102EFD9F at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's (ghidra_frameset18), outside the tree; the bytes are its proof. Compiled /O1 (size). */
// VC6FLAGS: /O1
#include "vc6_shim.h"
// FUNCTION: FFXIMAIN 0x102EFD9F
undefined4 __cdecl fn_102EFD9F(int param_1, int param_2, uint32_t * param_3)
{
  if ((((param_1 != 0) && (param_2 != 0)) && ((*(uint *)(param_2 + 8) & 0x800) != 0)) &&
     (param_3 != (uint *)0x0)) {
    *param_3 = (uint)*(byte *)(param_2 + 0x2c);
    return 0x800;
  }
  return 0;
}