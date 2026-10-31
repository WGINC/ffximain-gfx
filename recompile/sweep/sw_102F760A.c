/* 0x102F760A at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's (ghidra_frameset18), outside the tree; the bytes are its proof. Compiled /O1 (size). */
// VC6FLAGS: /O1
#include "vc6_shim.h"
void __cdecl FUN_10312EAD(unsigned int);
// FUNCTION: FFXIMAIN 0x102F760A
void __cdecl fn_102F760A(int param_1)
{
  if (param_1 != 0) {
    FUN_10312EAD((LPVOID)param_1);
    return;
  }
  return;
}