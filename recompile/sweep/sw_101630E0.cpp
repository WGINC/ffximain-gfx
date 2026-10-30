/* 0x101630E0 at the game's bytes: pipeline/vc6/sweep.py's source of it under rule xiany, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: an alternative decompiler export's text for this function (not the one the tree takes); the bytes are its proof. */
#include "vc6_shim.h"
#define g_pCDx_as_s32 (*(int *)0x1045666Cu)
struct FUNt_10163390 { void f(void); };
struct FUNt_1019EBB0 { undefined4 f(void); };
struct XS { void Obj_10621944_Draw(void); };
// FUNCTION: FFXIMAIN 0x101630E0
void XS::Obj_10621944_Draw(void)
{
  void *self_ = this;

  if (*(int *)((int)self_ + 0xc) != 0) {
    ((FUNt_10163390 *)(self_))->f();
    ((int (__stdcall *)(unsigned int, unsigned int, unsigned int, unsigned int))((**(code **)(**(int **)(g_pCDx_as_s32 + 0xc) + 0xfc))))((unsigned int)(*(int **)(g_pCDx_as_s32 + 0xc)), 0, 0x10, 2);
    ((FUNt_1019EBB0 *)(*(void **)((int)self_ + 0xc)))->f();
    ((int (__stdcall *)(unsigned int, unsigned int, unsigned int, unsigned int))((**(code **)(**(int **)(g_pCDx_as_s32 + 0xc) + 0xfc))))((unsigned int)(*(int **)(g_pCDx_as_s32 + 0xc)), 0, 0x10, 1);
  }
  return;

}
