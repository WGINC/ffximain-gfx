/* 0x10314D55 at the game's bytes: pipeline/vc6/sweep.py's source of it under rule ret64, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
void __cdecl FUN_1031716D(unsigned int);
undefined4 __cdecl FUN_1031EFF9(unsigned int);
undefined4 __cdecl FUN_10318C5E(unsigned int, unsigned int, unsigned int);
void __cdecl FUN_1031F086(unsigned int, unsigned int);
void __cdecl FUN_103171BF(unsigned int);
// FUNCTION: FFXIMAIN 0x10314D55
undefined4 __cdecl fn_10314D55(int * param_1, uint8_t * param_2, undefined4 xi_s3)
{
  int iVar1;
  uint32_t uVar2;
  
  FUN_1031716D((uint)param_1);
  iVar1 = FUN_1031EFF9(param_1);
  uVar2 = FUN_10318C5E(param_1, param_2, &xi_s3);
  FUN_1031F086(iVar1, param_1);
  FUN_103171BF((uint)param_1);
  return (int)uVar2;
}