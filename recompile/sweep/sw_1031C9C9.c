/* 0x1031C9C9 at the game's bytes: pipeline/vc6/sweep.py's source of it under rule globvar, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
extern int xi_g_109B2740;
#define DAT_109b2740 xi_g_109B2740
uint64_t __cdecl FUN_1031C8B0(unsigned int);
// FUNCTION: FFXIMAIN 0x1031C9C9
void __cdecl fn_1031C9C9(int param_1, int param_2, uint8_t * param_3)
{
  int *xi_eax;
  
  xi_eax = (int *)(param_1 + 0x18 + (param_2 - *(int *)(param_1 + 0x10) >> 0xc) * 8);
  *xi_eax = *xi_eax + (uint)*param_3;
  *param_3 = 0;
  xi_eax[1] = 0xf1;
  if ((*xi_eax == 0xf0) && (DAT_109b2740 = DAT_109b2740 + 1, DAT_109b2740 == 0x20)) {
    FUN_1031C8B0(0x10);
  }
  return;
}