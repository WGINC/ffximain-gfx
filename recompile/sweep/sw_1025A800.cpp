/* 0x1025A800 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: an alternative decompiler export's text for this function (not the one the tree takes); the bytes are its proof. */
#include "vc6_shim.h"
struct FUNt_1025A820 { undefined4 f(void); };
struct XS { undefined4 fn_1025A800(void); };
// FUNCTION: FFXIMAIN 0x1025A800
undefined4 XS::fn_1025A800(void)
{
  void *self_ = this;

  return ((FUNt_1025A820 *)(self_))->f();

}
