/* 0x101A2660 at the game's bytes: pipeline/vc6/sweep.py's source of it under rule calledon0, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: an alternative decompiler export's text for this function (not the one the tree takes); the bytes are its proof. */
#include "vc6_shim.h"
struct FUNt_101A2610 { template <class A0, class A1, class A2> void f(A0, A1, A2); };
struct FUNt_101B9880 { template <class A1, class A2, class A3, class A4, class A5, class A6, class A7> void f(A1, A2, A3, A4, A5, A6, A7); };
struct FUNt_101B9A10 { undefined4 f(void); };
struct FUNt_101B9A00 { void f(void); };
struct XS { void Unit1018BA60_LoadNumberedDatIntoBuffer(int param_1, undefined4 xi_s2); };
// FUNCTION: FFXIMAIN 0x101A2660
void XS::Unit1018BA60_LoadNumberedDatIntoBuffer(int param_1, undefined4 xi_s2)
{
  void *self_ = this;

  char xi_frame[352];
  char  (&local_160)[64] = *reinterpret_cast<char  (*)[64]>(xi_frame + 0);
  undefined1  (&local_120)[288] = *reinterpret_cast<undefined1  (*)[288]>(xi_frame + 64);
  
  ((FUNt_101A2610 *)(this))->f(local_160, param_1, xi_s2);
  ((FUNt_101B9880 *)(local_120))->f(1, local_160, (int)self_ + 4, 0x1db0, 0, 0, 1);
  ((FUNt_101B9A10 *)(local_120))->f();
  ((FUNt_101B9A00 *)(local_120))->f();
  return;

}
