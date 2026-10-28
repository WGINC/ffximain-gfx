/* 0x10071F70 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); the bytes are its proof. */
#include "vc6_shim.h"
struct FUNt_10072240 { void * f(void); };
struct XS { void fn_10071F70(undefined4 param_1); };
// FUNCTION: FFXIMAIN 0x10071F70
void XS::fn_10071F70(undefined4 param_1)
{
  void *self_ = this;

  void *pvVar1;
  
  pvVar1 = ((FUNt_10072240 *)(self_))->f();
  *(undefined4 *)((int)pvVar1 + 0x1c) = param_1;
  return;

}
