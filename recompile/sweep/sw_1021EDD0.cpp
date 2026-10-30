/* 0x1021EDD0 at the game's bytes: pipeline/vc6/sweep.py's source of it under rule calledon0, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: an alternative decompiler export's text for this function (not the one the tree takes); the bytes are its proof. */
#include "vc6_shim.h"
#define DAT_10666e84 (*(int *)0x10666E84u)
struct FUNt_1021EF30 { void f(void); };
// FUNCTION: FFXIMAIN 0x1021EDD0
void __cdecl Unit1021D930_Call1021EF30If10666E84(void)
{
  if (DAT_10666e84 != 0) {
    ((FUNt_1021EF30 *)(DAT_10666e84))->f();
    return;
  }
  return;
}