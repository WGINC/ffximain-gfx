/* 0x10077A00 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); the bytes are its proof. */
#include "vc6_shim.h"
#define DAT_1047d496_as_s8 (*(char *)0x1047D496u)
#define DAT_10662794_as_v0 (*(void * *)0x10662794u)
struct FUNt_101A1CB0 { void f(void); };
// FUNCTION: FFXIMAIN 0x10077A00
extern "C" void __cdecl fn_10077A00(void)
{
  if (DAT_1047d496_as_s8 != '\0') {
    ((FUNt_101A1CB0 *)(DAT_10662794_as_v0))->f();
    return;
  }
  return;
}