/* 0x10313CF0 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
void __cdecl FUN_1031716D(unsigned int);
int __cdecl FUN_10313D1C(unsigned int, unsigned int, unsigned int);
void __cdecl FUN_103171BF(unsigned int);
// FUNCTION: FFXIMAIN 0x10313CF0
undefined4 __cdecl fn_10313CF0(int * param_1, undefined4 param_2, undefined4 param_3)
{
  int iVar1;
  
  FUN_1031716D((uint)param_1);
  iVar1 = FUN_10313D1C(param_1, param_2, param_3);
  FUN_103171BF((uint)param_1);
  return iVar1;
}