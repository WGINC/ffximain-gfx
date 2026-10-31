/* 0x10063A60 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); the bytes are its proof. */
#include "vc6_shim.h"
struct FUNt_10063890 { template <class A1, class A2> long double f(A1, A2); };
struct XS { void fn_10063A60(float param_1, float * param_2); };
// FUNCTION: FFXIMAIN 0x10063A60
void XS::fn_10063A60(float param_1, float * param_2)
{
  void *self_ = this;

  undefined4 xi_eax;
  undefined4 xi_eax_00;
  float10 fVar1;
  
  fVar1 = ((FUNt_10063890 *)(self_))->f(param_1, *(int *)((int)self_ + 0xc));
  *param_2 = (float)fVar1;
  fVar1 = ((FUNt_10063890 *)(self_))->f(param_1, *(int *)((int)self_ + 0x10));
  param_2[1] = (float)fVar1;
  fVar1 = ((FUNt_10063890 *)(self_))->f(param_1, *(int *)((int)self_ + 0x14));
  param_2[2] = (float)fVar1;
  return;

}
