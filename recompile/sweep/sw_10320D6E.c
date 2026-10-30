/* 0x10320D6E at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
#define DAT_109f82e0 (*(undefined4 *)0x109F82E0u)
typedef void (__stdcall *LeaveCriticalSection_t)(XI_ANY);
#define LeaveCriticalSection (*(LeaveCriticalSection_t *)0x10329188u)
// FUNCTION: FFXIMAIN 0x10320D6E
void __cdecl fn_10320D6E(uint32_t param_1)
{
  LeaveCriticalSection((&DAT_109f82e0)[(int)param_1 >> 5] + 0xc + (param_1 & 0x1f) * 0x24);
  return;
}