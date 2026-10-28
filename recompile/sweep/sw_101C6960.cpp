/* 0x101C6960 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); the bytes are its proof. */
#include "vc6_shim.h"
struct FUNt_101B5BC0 { template <class A1, class A2> uint32_t f(A1, A2); };
struct XS { void fn_101C6960(int param_1, undefined4 xi_s2); };
// FUNCTION: FFXIMAIN 0x101C6960
void XS::fn_101C6960(int param_1, undefined4 xi_s2)
{
  void *self_ = this;

  char cVar1;
  uint uVar2;
  char *pcVar3;
  char xi_frame[452];
  char  (&local_1c4)[452] = *reinterpret_cast<char  (*)[452]>(xi_frame + 0);
  
  uVar2 = ((FUNt_101B5BC0 *)(*(void **)((int)self_ + 0xc)))->f((undefined4 *)local_1c4, xi_s2);
  pcVar3 = local_1c4;
  do {
    cVar1 = *pcVar3;
    pcVar3[param_1 - (int)local_1c4] = cVar1;
    pcVar3 = pcVar3 + 1;
  } while (cVar1 != '\0');
  *(undefined1 *)(uVar2 + param_1) = 0;
  return;

}
