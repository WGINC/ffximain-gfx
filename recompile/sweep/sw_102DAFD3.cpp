/* 0x102DAFD3 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's (ghidra_frameset18), outside the tree; the bytes are its proof. */
#include "vc6_shim.h"
struct XS { undefined4 fn_102DAFD3(void); };
// FUNCTION: FFXIMAIN 0x102DAFD3
undefined4 XS::fn_102DAFD3(void)
{
  void *self_ = this;

  *(undefined4 *)self_ = 0;
  *(undefined4 *)((int)self_ + 4) = 0;
  *(undefined4 *)((int)self_ + 8) = 0;
  *(undefined4 *)((int)self_ + 0x38) = 0;
  *(undefined4 *)((int)self_ + 0x3c) = 0;
  *(undefined4 *)((int)self_ + 0x4c) = 0;
  *(undefined4 *)((int)self_ + 0x50) = 0;
  return (unsigned int)(self_);

}
