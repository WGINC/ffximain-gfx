/* 0x102E1001 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); the bytes are its proof. */
#include "vc6_shim.h"
struct FUNt_102E0F7C { undefined4 f(void); };
struct XS { undefined4 fn_102E1001(void); };
// FUNCTION: FFXIMAIN 0x102E1001
undefined4 XS::fn_102E1001(void)
{
  void *self_ = this;

  return ((FUNt_102E0F7C *)(self_))->f();

}
