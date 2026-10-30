/* 0x10082F20 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: an alternative decompiler export's text for this function (not the one the tree takes); the bytes are its proof. */
#include "vc6_shim.h"
#define g_actorNameBuffer (*(undefined1 *)0x1047D580u)
struct FUNt_10085490 { int f(void); };
struct FUNt_10097840 { undefined1 * f(void); };
struct XS { undefined1 * GetEnglishName(void); };
// FUNCTION: FFXIMAIN 0x10082F20
undefined1 * XS::GetEnglishName(void)
{
  void *self_ = this;

  char cVar1;
  int iVar2;
  char *pcVar3;
  char *pcVar4;
  
  g_actorNameBuffer = 0;
  if (*(int *)((int)self_ + 0x70) != 0) {
    iVar2 = ((FUNt_10085490 *)(self_))->f();
    if ((char)iVar2 == '\0') {
      pcVar3 = (char *)(((FUNt_10097840 *)(*(void **)((int)self_ + 0x70)))->f());
      pcVar4 = (char *)(&g_actorNameBuffer);
      do {
        cVar1 = *pcVar3;
        pcVar3 = pcVar3 + 1;
        *pcVar4 = cVar1;
        pcVar4 = pcVar4 + 1;
      } while (cVar1 != '\0');
    }
    return &g_actorNameBuffer;
  }
  return (undefined1 *)((int)self_ + 0xb8);

}
