/* 0x102EFD4A at the game's bytes: pipeline/vc6/sweep.py's source of it under rule retnarrow1, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's (ghidra_frameset18), outside the tree; the bytes are its proof. Compiled /O1 (size). */
// VC6FLAGS: /O1
#include "vc6_shim.h"
// FUNCTION: FFXIMAIN 0x102EFD4A
unsigned char __cdecl fn_102EFD4A(int param_1, int param_2)
{
  uint xi_entry_eax = 0;
  
  if ((param_1 != 0) && (xi_entry_eax = 0, param_2 != 0)) {
    return *(undefined1 *)(param_2 + 0x1d);
  }
  return 0;
}