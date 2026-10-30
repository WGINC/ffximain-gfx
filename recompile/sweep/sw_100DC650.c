/* 0x100DC650 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: an alternative decompiler export's text for this function (not the one the tree takes); the bytes are its proof. */
#include "vc6_shim.h"
typedef uint32_t (__stdcall *Ordinal_3_t)(XI_ANY);
#define Ordinal_3 (*(Ordinal_3_t *)0x10329484u)
// FUNCTION: FFXIMAIN 0x100DC650
undefined __cdecl fn_100DC650(undefined4 * xi_s1)
{
  int iVar1;
  
  iVar1 = Ordinal_3(*xi_s1);
  if (iVar1 == -1) {
    return 0;
  }
  *(undefined1 *)(xi_s1 + 1) = 0;
  return 1;
}