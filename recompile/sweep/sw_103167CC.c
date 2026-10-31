/* 0x103167CC at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
#define DAT_109b2714 (*(int *)0x109B2714u)
#define DAT_109b2718 (*(int *)0x109B2718u)
#define PTR___exit_103d0380 (*(undefined * *)0x103D0380u)
void __cdecl FUN_10321CE3(void);
void __cdecl FUN_10321D1C(unsigned int);
// FUNCTION: FFXIMAIN 0x103167CC
void __cdecl fn_103167CC(int param_1)
{
  if ((DAT_109b2714 == 1) || ((DAT_109b2714 == 0 && (DAT_109b2718 == 1)))) {
    FUN_10321CE3();
  }
  FUN_10321D1C((undefined *)param_1);
  (*(code *)PTR___exit_103d0380)(0xff);
  return;
}