/* 0x10321CE3 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
#define DAT_109b2714 (*(int *)0x109B2714u)
#define DAT_109b2718 (*(int *)0x109B2718u)
typedef code xi_codefn;
#define DAT_109b29d8 (*(xi_codefn * *)0x109B29D8u)
void __cdecl FUN_10321D1C(unsigned int);
// FUNCTION: FFXIMAIN 0x10321CE3
void __cdecl fn_10321CE3(void)
{
  if ((DAT_109b2714 == 1) || ((DAT_109b2714 == 0 && (DAT_109b2718 == 1)))) {
    FUN_10321D1C((undefined *)0xfc);
    if (DAT_109b29d8 != (code *)0x0) {
      (*DAT_109b29d8)();
    }
    FUN_10321D1C((undefined *)0xff);
  }
  return;
}