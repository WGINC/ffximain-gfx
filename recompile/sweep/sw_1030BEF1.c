/* 0x1030BEF1 at the game's bytes: pipeline/vc6/sweep.py's source of it under rule globvar, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
extern undefined4 xi_g_103CEE58;
#define DAT_103cee58 xi_g_103CEE58
extern undefined4 xi_g_103CEE5C;
#define DAT_103cee5c xi_g_103CEE5C
extern undefined4 xi_g_103CEE60;
#define DAT_103cee60 xi_g_103CEE60
extern undefined4 xi_g_103CFE60;
#define DAT_103cfe60 xi_g_103CFE60
// FUNCTION: FFXIMAIN 0x1030BEF1
undefined4 __cdecl fn_1030BEF1(undefined4 * param_1, undefined4 * param_2, undefined4 * param_3, undefined4 * param_4)
{
  *param_1 = DAT_103cee58;
  *param_2 = DAT_103cee5c;
  *param_3 = &DAT_103cee60;
  *param_4 = &DAT_103cfe60;
  return 0;
}