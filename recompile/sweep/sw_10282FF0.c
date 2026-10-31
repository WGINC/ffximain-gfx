/* 0x10282FF0 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: an alternative decompiler export's text for this function (not the one the tree takes); the bytes are its proof. */
#include "vc6_shim.h"
#define g_pDancerDevice (*(int * *)0x1066941Cu)
#define g_pSqgxContext (*(int *)0x1099D200u)
// FUNCTION: FFXIMAIN 0x10282FF0
void __cdecl sqgxBlendMultAlpha(void)
{
  if (*(int *)(g_pSqgxContext + 0xb18) == 0) {
    ((int (__stdcall *)(XI_ANY, XI_ANY, XI_ANY))((**(code **)(*g_pDancerDevice + 200))))(g_pDancerDevice,0x1b,1);
    ((int (__stdcall *)(XI_ANY, XI_ANY, XI_ANY))((**(code **)(*g_pDancerDevice + 200))))(g_pDancerDevice,0xab,1);
    ((int (__stdcall *)(XI_ANY, XI_ANY, XI_ANY))((**(code **)(*g_pDancerDevice + 200))))(g_pDancerDevice,0x13,1);
    ((int (__stdcall *)(XI_ANY, XI_ANY, XI_ANY))((**(code **)(*g_pDancerDevice + 200))))(g_pDancerDevice,0x14,5);
    return;
  }
  ((int (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY))((**(code **)(*g_pDancerDevice + 0xfc))))(g_pDancerDevice,*(int *)(g_pSqgxContext + 0xb18),1,0x12);
  ((int (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY))((**(code **)(*g_pDancerDevice + 0xfc))
            ))(g_pDancerDevice,*(undefined4 *)(g_pSqgxContext + 0xb18),2,3);
  ((int (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY))((**(code **)(*g_pDancerDevice + 0xfc))
            ))(g_pDancerDevice,*(undefined4 *)(g_pSqgxContext + 0xb18),3,1);
  ((int (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY))((**(code **)(*g_pDancerDevice + 0xfc))
            ))(g_pDancerDevice,*(undefined4 *)(g_pSqgxContext + 0xb18),4,2);
  ((int (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY))((**(code **)(*g_pDancerDevice + 0xfc))
            ))(g_pDancerDevice,*(undefined4 *)(g_pSqgxContext + 0xb18),5,2);
  ((int (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY))((**(code **)(*g_pDancerDevice + 0xfc))
            ))(g_pDancerDevice,*(undefined4 *)(g_pSqgxContext + 0xb18),6,1);
  ((int (__stdcall *)(XI_ANY, XI_ANY, XI_ANY))((**(code **)(*g_pDancerDevice + 200))))(g_pDancerDevice,0x3c,0);
  return;
}