/* 0x102E1779 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's (ghidra_frameset18), outside the tree; the bytes are its proof. Compiled /O1 (size). */
// VC6FLAGS: /O1
#include "vc6_shim.h"
#define PTR_FUN_10346140 (*(undefined * *)0x10346140u)
template <class A0> void __cdecl FUN_10311BB0(A0);
struct XS { void fn_102E1779(void); };
// FUNCTION: FFXIMAIN 0x102E1779
void XS::fn_102E1779(void)
{
  void *self_ = this;

  *(undefined ***)self_ = &PTR_FUN_10346140;
  FUN_10311BB0(*(undefined4 *)((int)self_ + 0x104c));
  return;

}
