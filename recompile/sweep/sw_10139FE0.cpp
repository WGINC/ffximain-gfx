/* 0x10139FE0 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: an alternative decompiler export's text for this function (not the one the tree takes); the bytes are its proof. */
#include "vc6_shim.h"
struct FUNt_1013A100 { undefined4 f(void); };
struct XS { undefined4 fn_10139FE0(void); };
// FUNCTION: FFXIMAIN 0x10139FE0
undefined4 XS::fn_10139FE0(void)
{
  void *self_ = this;

  return ((FUNt_1013A100 *)(self_))->f();

}
