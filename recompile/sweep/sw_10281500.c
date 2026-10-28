/* 0x10281500 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); the bytes are its proof. */
#include "vc6_shim.h"
#define g_sqgxRenderStateDirty_as_v0 (*(undefined4 *)0x109A2618u)
// FUNCTION: FFXIMAIN 0x10281500
void __cdecl ___setargv_3(void)
{
  g_sqgxRenderStateDirty_as_v0 = 1;
  return;
}