/* 0x10319C49 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); the bytes are its proof. */
#include "vc6_shim.h"
DWORD * __cdecl FUN_1031A00D(void);
// FUNCTION: FFXIMAIN 0x10319C49
int __cdecl fn_10319C49(void)
{
  DWORD *pDVar1;
  
  pDVar1 = FUN_1031A00D();
  return (int)(pDVar1 + 2);
}