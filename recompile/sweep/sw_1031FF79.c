/* 0x1031FF79 at the game's bytes: pipeline/vc6/sweep.py's source of it under rule globvar, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
extern int xi_g_109B2898;
#define DAT_109b2898 xi_g_109B2898
undefined8 __cdecl FUN_1031CFD6(unsigned int);
void __cdecl FUN_1031FFBD(void);
undefined8 __cdecl FUN_1031D037(unsigned int);
// FUNCTION: FFXIMAIN 0x1031FF79
void __cdecl fn_1031FF79(void)
{
  if (DAT_109b2898 == 0) {
    FUN_1031CFD6(0xb);
    if (DAT_109b2898 == 0) {
      FUN_1031FFBD();
      DAT_109b2898 = DAT_109b2898 + 1;
    }
    FUN_1031D037(0xb);
  }
  return;
}