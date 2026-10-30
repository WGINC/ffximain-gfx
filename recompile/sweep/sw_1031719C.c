/* 0x1031719C at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
typedef void (__stdcall *EnterCriticalSection_t)(XI_ANY);
#define EnterCriticalSection (*(EnterCriticalSection_t *)0x1032918Cu)
undefined8 __cdecl FUN_1031CFD6(unsigned int);
// FUNCTION: FFXIMAIN 0x1031719C
void __cdecl fn_1031719C(int param_1, int param_2)
{
  if (param_1 < 0x14) {
    FUN_1031CFD6(param_1 + 0x1c);
    return;
  }
  EnterCriticalSection(param_2 + 0x20);
  return;
}