/* 0x102DD5BA at the game's bytes: pipeline/vc6/sweep.py's source of it under rule retthis, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
struct XS { void * fn_102DD5BA(void); };
// FUNCTION: FFXIMAIN 0x102DD5BA
void * XS::fn_102DD5BA(void)
{
  void *self_ = this;

  *(undefined4 *)((int)self_ + 4) = 0;
  *(undefined4 *)self_ = 0;
  return this;

  return this;
}
