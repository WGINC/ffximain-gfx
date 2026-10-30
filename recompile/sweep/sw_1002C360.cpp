/* 0x1002C360 at the game's bytes: pipeline/vc6/sweep.py's source of it under rule globvar_xiany, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: an alternative decompiler export's text for this function (not the one the tree takes); the bytes are its proof. */
#include "vc6_shim.h"
extern int xi_g_1045666C;
#define g_pCDx_as_s32 xi_g_1045666C
struct FUNt_1002B5A0 { int f(void); };
struct FUNt_1002A3B0 { template <class A1, class A2, class A3> void f(A1, A2, A3); };
struct XS { void PrepareModelsOnce(undefined4 xi_s1, undefined4 xi_s2); };
// FUNCTION: FFXIMAIN 0x1002C360
void XS::PrepareModelsOnce(undefined4 xi_s1, undefined4 xi_s2)
{
  void *self_ = this;

  int *piVar1;
  void *this_00;
  
  if ((*(byte *)((int)self_ + 0x30) & 4) == 0) {
    piVar1 = (int *)((FUNt_1002B5A0 *)(self_))->f();
    this_00 = (void *)*piVar1;
    if (this_00 != (void *)0x0) {
      do {
        ((FUNt_1002A3B0 *)(this_00))->f(xi_s1, self_, xi_s2);
        piVar1 = (int *)(*piVar1 + 4);
        this_00 = (void *)*piVar1;
      } while (this_00 != (void *)0x0);
      ((int (__stdcall *)(unsigned int, unsigned int, unsigned int, unsigned int))((**(code **)(**(int **)(g_pCDx_as_s32 + 0xc) + 0xfc))))((unsigned int)(*(int **)(g_pCDx_as_s32 + 0xc)), 1, 0x18, 0);
      ((int (__stdcall *)(unsigned int, unsigned int, unsigned int, unsigned int))((**(code **)(**(int **)(g_pCDx_as_s32 + 0xc) + 0xfc))))((unsigned int)(*(int **)(g_pCDx_as_s32 + 0xc)), 1, 0xb, 0);
      *(uint *)((int)self_ + 0x30) = *(uint *)((int)self_ + 0x30) | 4;
    }
  }
  return;

}
