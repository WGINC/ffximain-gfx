/* 0x102EFF82 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's (ghidra_frameset18), outside the tree; the bytes are its proof. Compiled /O1 (size). */
// VC6FLAGS: /O1
#include "vc6_shim.h"
// FUNCTION: FFXIMAIN 0x102EFF82
undefined4 __cdecl fn_102EFF82(int param_1)
{
  if (*(char *)(param_1 + 0x113) != '\0') {
    *(uint *)(param_1 + 0x60) = *(uint *)(param_1 + 0x60) | 2;
    return 7;
  }
  return 1;
}