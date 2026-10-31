/* 0x10056C70 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: an alternative decompiler export's text for this function (not the one the tree takes); the bytes are its proof. */
#include "vc6_shim.h"
struct FUNt_10071E90 { undefined8 f(void); };
struct FUNt_10056CE0 { template <class A1, class A2, class A3, class A4> int f(A1, A2, A3, A4); };
struct FUNt_100748C0 { void f(void); };
struct XS { void * Scheduler_Execute(int param_1, int param_2, undefined4 param_3, int param_4); };
// FUNCTION: FFXIMAIN 0x10056C70
void * XS::Scheduler_Execute(int param_1, int param_2, undefined4 param_3, int param_4)
{
  void *self_ = this;

  int iVar1;
  void *this_00;
  
  ((FUNt_10071E90 *)(self_))->f();
  this_00 = (void *)((FUNt_10056CE0 *)(self_))->f(param_1, param_2, param_4, param_3);
  if (param_4 != 0) {
    iVar1 = *(int *)(*(int *)(param_4 + 0xc) + 0xc);
    if (((iVar1 != 0xe) && (iVar1 != 0xf)) && (1 < *(short *)(*(int *)(param_4 + 0xc) + 0x1c))) {
      *(undefined4 *)((int)this_00 + 0x9c) = 0x3f333333;
      *(undefined1 *)((int)this_00 + 0x142) = 1;
    }
  }
  ((FUNt_100748C0 *)(this_00))->f();
  return this_00;

}
