/* 0x103195A1 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
#define DAT_109f82e0 (*(undefined4 *)0x109F82E0u)
#define DAT_109f83e0 (*(uint32_t *)0x109F83E0u)
void __cdecl FUN_10320D0F(unsigned int);
int __cdecl FUN_10319606(unsigned int, unsigned int, unsigned int);
void __cdecl FUN_10320D6E(unsigned int);
int __cdecl FUN_10319C49(void);
int __cdecl FUN_10319C52(void);
// FUNCTION: FFXIMAIN 0x103195A1
undefined4 __cdecl fn_103195A1(uint32_t param_1, char * param_2, char * param_3)
{
  int iVar1;
  undefined4 *puVar2;
  
  if ((param_1 < DAT_109f83e0) &&
     ((*(byte *)((&DAT_109f82e0)[(int)param_1 >> 5] + 4 + (param_1 & 0x1f) * 0x24) & 1) != 0)) {
    FUN_10320D0F(param_1);
    iVar1 = FUN_10319606(param_1, param_2, param_3);
    FUN_10320D6E(param_1);
    return iVar1;
  }
  puVar2 = (undefined4 *)FUN_10319C49();
  *puVar2 = 9;
  puVar2 = (undefined4 *)FUN_10319C52();
  *puVar2 = 0;
  return 0xffffffff;
}