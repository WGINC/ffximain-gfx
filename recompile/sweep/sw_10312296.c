/* 0x10312296 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
void __cdecl FUN_1031716D(unsigned int);
uint32_t __cdecl FUN_103122C5(unsigned int, unsigned int, unsigned int, unsigned int);
void __cdecl FUN_103171BF(unsigned int);
// FUNCTION: FFXIMAIN 0x10312296
undefined4 __cdecl fn_10312296(char * param_1, undefined4 param_2, undefined4 param_3, int * param_4)
{
  uint uVar1;
  
  FUN_1031716D((uint)param_4);
  uVar1 = FUN_103122C5(param_1, param_2, param_3, param_4);
  FUN_103171BF((uint)param_4);
  return uVar1;
}