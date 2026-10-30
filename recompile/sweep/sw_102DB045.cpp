/* 0x102DB045 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's (ghidra_frameset18), outside the tree; the bytes are its proof. Compiled /O1 (size). */
// VC6FLAGS: /O1
#include "vc6_shim.h"
struct FUNt_102DAFEC { void f(void); };
template <class A0> void __cdecl FUN_10311BB0(A0);
struct XS { void * fn_102DB045(undefined4 param_1); };
// FUNCTION: FFXIMAIN 0x102DB045
void * XS::fn_102DB045(undefined4 param_1)
{
  void *self_ = this;

  ((FUNt_102DAFEC *)(self_))->f();
  if ((param_1 & 1) != 0) {
    FUN_10311BB0(self_);
  }
  return self_;

}
