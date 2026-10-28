/* 0x1011A560 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); the bytes are its proof. */
#include "vc6_shim.h"
#define g_pTkHelp (*(void * *)0x105781F8u)
#define g_pTkTitle (*(void * *)0x1057847Cu)
struct FUNt_1013A320 { template <class A1, class A2> void f(A1, A2); };
struct FUNt_10150CC0 { template <class A1, class A2> void f(A1, A2); };
struct XS { void fn_1011A560(void); };
// FUNCTION: FFXIMAIN 0x1011A560
void XS::fn_1011A560(void)
{
  void *self_ = this;

  if (*(char *)((int)self_ + 0x6d) != '\0') {
    ((FUNt_1013A320 *)(g_pTkHelp))->f(0, 1);
    *(undefined1 *)((int)self_ + 0x6d) = 0;
  }
  if (*(char *)((int)self_ + 0x6c) != '\0') {
    ((FUNt_10150CC0 *)(g_pTkTitle))->f(0, 1);
    *(undefined1 *)((int)self_ + 0x6c) = 0;
  }
  return;

}
