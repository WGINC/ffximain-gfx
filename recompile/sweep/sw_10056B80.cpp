/* 0x10056B80 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); the bytes are its proof. */
#include "vc6_shim.h"
struct FUNt_10071E90 { void f(void); };
struct FUNt_10056CE0 { template <class A1, class A2, class A3, class A4> int f(A1, A2, A3, A4); };
struct FUNt_100748C0 { void f(void); };
struct XS { int fn_10056B80(undefined4 param_1, undefined4 param_2); };
// FUNCTION: FFXIMAIN 0x10056B80
int XS::fn_10056B80(undefined4 param_1, undefined4 param_2)
{
  void *self_ = this;

  void *this_00;
  
  ((FUNt_10071E90 *)(self_))->f();
  this_00 = (void *)((FUNt_10056CE0 *)(self_))->f(0, 0, param_2, param_1);
  if (this_00 != (void *)0x0) {
    ((FUNt_100748C0 *)(this_00))->f();
  }
  return (int)this_00;

}
