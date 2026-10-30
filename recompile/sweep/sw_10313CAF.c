/* 0x10313CAF at the game's bytes: pipeline/vc6/sweep.py's source of it under rule ret64, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
#define DAT_103d03c0 (*(undefined4 *)0x103D03C0u)
void __cdecl FUN_1031719C(unsigned int, unsigned int);
undefined4 __cdecl FUN_1031EFF9(unsigned int);
undefined4 __cdecl FUN_10318C5E(unsigned int, unsigned int, unsigned int);
void __cdecl FUN_1031F086(unsigned int, unsigned int);
void __cdecl FUN_103171EE(unsigned int, unsigned int);
// FUNCTION: FFXIMAIN 0x10313CAF
undefined4 __cdecl fn_10313CAF(uint8_t * param_1, undefined4 xi_s2)
{
  int iVar1;
  uint32_t uVar2;
  
  FUN_1031719C(1, 0x103d03c0);
  iVar1 = FUN_1031EFF9((undefined4 *)&DAT_103d03c0);
  uVar2 = FUN_10318C5E(&DAT_103d03c0, param_1, &xi_s2);
  FUN_1031F086(iVar1, (int *)&DAT_103d03c0);
  FUN_103171EE(1, 0x103d03c0);
  return (int)uVar2;
}