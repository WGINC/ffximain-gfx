/* 0x10319FFA at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); the bytes are its proof. */
#include "vc6_shim.h"
#define DAT_103d2e08 (*(undefined4 *)0x103D2E08u)
// FUNCTION: FFXIMAIN 0x10319FFA
void __cdecl fn_10319FFA(int param_1)
{
  *(undefined **)(param_1 + 0x50) = &DAT_103d2e08;
  *(undefined4 *)(param_1 + 0x14) = 1;
  return;
}