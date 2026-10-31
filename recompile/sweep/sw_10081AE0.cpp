/* 0x10081AE0 at the game's bytes: pipeline/vc6/sweep.py's source of it under rule globvar, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: an alternative decompiler export's text for this function (not the one the tree takes); the bytes are its proof. */
#include "vc6_shim.h"
extern void * xi_g_1047D578;
#define g_pActorDrawListHead_as_v1 xi_g_1047D578
extern "C" int __cdecl FUN_10082BF0(void);
struct XS { void CXiActor_LinkIntoDrawList(void); };
// FUNCTION: FFXIMAIN 0x10081AE0
void XS::CXiActor_LinkIntoDrawList(void)
{
  void *self_ = this;

  int iVar1;
  
  if ((*(byte *)((int)self_ + 0x88) & 2) == 0) {
    if (g_pActorDrawListHead_as_v1 != (void *)0x0) {
      *(void **)((int)g_pActorDrawListHead_as_v1 + 0x58) = self_;
    }
    *(void **)((int)self_ + 0x54) = g_pActorDrawListHead_as_v1;
    *(undefined4 *)((int)self_ + 0x58) = 0;
    g_pActorDrawListHead_as_v1 = self_;
    iVar1 = FUN_10082BF0();
    *(int *)((int)self_ + 0x74) = iVar1;
    *(uint *)((int)self_ + 0x88) = *(uint *)((int)self_ + 0x88) | 2;
  }
  return;

}
