/* 0x102F20C1 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's (ghidra_frameset18), outside the tree; the bytes are its proof. Compiled /O1 (size). */
// VC6FLAGS: /O1
#include "vc6_shim.h"
uint32_t __cdecl FUN_102F89DB(unsigned int, unsigned int, unsigned int);
// FUNCTION: FFXIMAIN 0x102F20C1
void __cdecl fn_102F20C1(int param_1)
{
  uint uVar1;
  
  uVar1 = FUN_102F89DB(0, (byte *)0x0, 0);
  *(uint *)(param_1 + 0x100) = uVar1;
  return;
}