/* 0x10074AD0 at the game's bytes: pipeline/vc6/sweep.py's source of it under rule calledon0, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: an alternative decompiler export's text for this function (not the one the tree takes); the bytes are its proof. */
#include "vc6_shim.h"
#define dword_1047D140 (*(int *)0x1047D140u)
struct FUNt_100747F0 { template <class A1> void f(A1); };
struct FUNt_10074890 { void f(void); };
struct FUNt_10074A20 { void f(void); };
struct XS { void DetachAndRecycle(int param_1); };
// FUNCTION: FFXIMAIN 0x10074AD0
void XS::DetachAndRecycle(int param_1)
{
  void *self_ = this;

  if (*(void **)((int)self_ + 0x10) != (void *)0x0) {
    ((FUNt_100747F0 *)(*(void **)((int)self_ + 0x10)))->f((int)self_);
  }
  if (*(void **)((int)self_ + 0x24) != (void *)0x0) {
    ((FUNt_10074890 *)(*(void **)((int)self_ + 0x24)))->f();
  }
  if (*(int *)((int)self_ + 4) != 0) {
    if (param_1 != 0) {
      ((FUNt_10074A20 *)(this))->f();
    }
    if (*(char *)((int)self_ + 0x20) == '\0') {
      *(undefined4 *)(dword_1047D140 + 0xd5c) = *(undefined4 *)(dword_1047D140 + 0xd50);
      *(void **)(dword_1047D140 + 0xd50) = self_;
      *(undefined4 *)((int)self_ + 4) = 0;
      *(int *)(dword_1047D140 + 0xd44) = *(int *)(dword_1047D140 + 0xd44) + -1;
    }
  }
  return;

}
