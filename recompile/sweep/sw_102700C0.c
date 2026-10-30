/* 0x102700C0 at the game's bytes: pipeline/vc6/sweep.py's source of it under rule globvar, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: an alternative decompiler export's text for this function (not the one the tree takes); the bytes are its proof. */
#include "vc6_shim.h"
extern int xi_g_1099AED0;
#define DAT_1099aed0 xi_g_1099AED0
undefined4 __cdecl FUN_10279870(unsigned int, unsigned int);
undefined4 __cdecl FUN_1028DE10(unsigned int, unsigned int, unsigned int, unsigned int);
void __cdecl FUN_10279530(unsigned int, unsigned int);
void __cdecl FUN_1027A220(unsigned int, unsigned int);
// FUNCTION: FFXIMAIN 0x102700C0
void __cdecl Mdl_LinkB0001ItemRefs(int param_1, int param_2, undefined4 param_3)
{
  int iVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  
  piVar4 = (int *)0x0;
  iVar1 = DAT_1099aed0 + param_1 * 0xac;
  iVar5 = *(int *)(DAT_1099aed0 + 0xa8 + param_2 * 0xac);
  iVar2 = DAT_1099aed0 + param_2 * 0xac;
  *(undefined4 *)(iVar2 + 0x88) = param_3;
  if ((iVar5 != 0) && (iVar6 = 0, 0 < *(int *)(iVar5 + 0x44))) {
    do {
      piVar3 = (int *)FUN_10279870(iVar5, iVar6);
      if (*piVar3 == 0xb0001) {
        iVar6 = *(int *)(iVar5 + 0x44);
        piVar4 = piVar3;
      }
      iVar6 = iVar6 + 1;
    } while (iVar6 < *(int *)(iVar5 + 0x44));
    if (((piVar4 != (int *)0x0) && (iVar1 = *(int *)(iVar1 + 0xa8), iVar1 != 0)) &&
       (iVar5 = 0, 0 < *(int *)(iVar1 + 0x44))) {
      do {
        piVar3 = (int *)FUN_10279870(iVar1, iVar5);
        if (*piVar3 == 0xb0001) {
          iVar5 = FUN_1028DE10((int)piVar3, piVar4, 0, *(undefined4 *)(iVar2 + 0x88));
          if (iVar5 != 0) {
            FUN_10279530(iVar1, iVar5);
            FUN_1027A220(iVar1, iVar5);
          }
          iVar5 = *(int *)(iVar1 + 0x44);
        }
        iVar5 = iVar5 + 1;
      } while (iVar5 < *(int *)(iVar1 + 0x44));
    }
  }
  return;
}