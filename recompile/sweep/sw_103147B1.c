/* 0x103147B1 at the game's bytes: pipeline/vc6/sweep.py's source of it under rule globvar, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
extern int xi_g_103D023C;
#define DAT_103d023c xi_g_103D023C
extern undefined * xi_g_103D0030;
#define PTR_DAT_103d0030 xi_g_103D0030
undefined8 __cdecl FUN_10315140(unsigned int, unsigned int);
// FUNCTION: FFXIMAIN 0x103147B1
uint32_t __cdecl fn_103147B1(int param_1)
{
  undefined8 uVar1;
  
  if (1 < DAT_103d023c) {
    uVar1 = FUN_10315140(param_1, 8);
    return (uint)uVar1;
  }
  return (byte)PTR_DAT_103d0030[param_1 * 2] & 8;
}