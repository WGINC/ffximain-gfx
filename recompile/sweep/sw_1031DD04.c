/* 0x1031DD04 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
uint32_t __cdecl FUN_1031DCD3(unsigned int);
uint32_t __cdecl FUN_103147B1(unsigned int);
// FUNCTION: FFXIMAIN 0x1031DD04
int __cdecl fn_1031DD04(int * param_1, undefined4 * param_2)
{
  uint uVar1;
  uint uVar2;
  
  do {
    *param_1 = *param_1 + 1;
    uVar1 = FUN_1031DCD3(param_2);
    uVar2 = FUN_103147B1(uVar1);
  } while (uVar2 != 0);
  return uVar1;
}