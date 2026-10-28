/* 0x1018D3D0 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); the bytes are its proof. */
#include "vc6_shim.h"
struct XS { void fn_1018D3D0(void); };
// FUNCTION: FFXIMAIN 0x1018D3D0
void XS::fn_1018D3D0(void)
{
  void *self_ = this;

  *(undefined4 *)((int)self_ + 4) = 0;
  if (*(undefined1 **)self_ != (undefined1 *)0x0) {
    **(undefined1 **)self_ = 0;
  }
  return;

}
