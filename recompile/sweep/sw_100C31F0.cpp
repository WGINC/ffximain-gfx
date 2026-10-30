/* 0x100C31F0 at the game's bytes: pipeline/vc6/sweep.py's source of it under rule retthis, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's (ghidra_frameset18), outside the tree; the bytes are its proof. */
#include "vc6_shim.h"
struct XS { void * fn_100C31F0(void); };
// FUNCTION: FFXIMAIN 0x100C31F0
void * XS::fn_100C31F0(void)
{
  void *self_ = this;

  *(undefined4 *)self_ = 0;
  *(undefined4 *)((int)self_ + 4) = 0;
  *(undefined4 *)((int)self_ + 8) = 0;
  return this;

  return this;
}
