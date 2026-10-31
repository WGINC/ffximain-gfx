/* 0x10312F96 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
#define DAT_109b27a8 (*(int *)0x109B27A8u)
undefined8 __cdecl FUN_10312FA8(unsigned int, unsigned int);
// FUNCTION: FFXIMAIN 0x10312F96
void * __cdecl _malloc_0(size_t _Size)
{
  undefined8 uVar1;
  
  uVar1 = FUN_10312FA8(_Size, DAT_109b27a8);
  return (void *)uVar1;
}