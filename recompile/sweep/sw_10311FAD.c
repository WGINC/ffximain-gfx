/* 0x10311FAD at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
undefined4 * __cdecl FUN_10318A7E(void);
undefined4 * __cdecl FUN_1031890E(unsigned int, unsigned int, unsigned int, unsigned int);
void __cdecl FUN_103171BF(unsigned int);
// FUNCTION: FFXIMAIN 0x10311FAD
undefined4 __cdecl fn_10311FAD(undefined4 param_1, char * param_2, undefined4 param_3)
{
  undefined4 *puVar1;
  undefined4 *puVar2;
  
  puVar1 = FUN_10318A7E();
  if (puVar1 == (undefined4 *)0x0) {
    return 0;
  }
  puVar2 = FUN_1031890E(param_1, param_2, param_3, puVar1);
  FUN_103171BF((uint)puVar1);
  return puVar2;
}