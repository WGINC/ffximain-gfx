/* 0x10313486 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
undefined4 __cdecl FUN_10313408(unsigned int);
// FUNCTION: FFXIMAIN 0x10313486
int __cdecl _atexit(undefined4 param_1)
{
  int iVar1;
  
  iVar1 = FUN_10313408(param_1);
  return (iVar1 != 0) - 1;
}