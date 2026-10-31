/* 0x101BCA30 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's (ghidra_frameset18), outside the tree; the bytes are its proof. */
#include "vc6_shim.h"
template <class A0> void __cdecl FUN_10311BB0(A0);
struct XS { void fn_101BCA30(void); };
// FUNCTION: FFXIMAIN 0x101BCA30
void XS::fn_101BCA30(void)
{
  void *self_ = this;

  if (*(int *)self_ != 0) {
    FUN_10311BB0(*(int *)self_);
  }
  return;

}
