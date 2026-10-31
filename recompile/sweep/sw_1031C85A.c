/* 0x1031C85A at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
#define DAT_103d09c8_as_v0 (*(undefined4 *)0x103D09C8u)
#define DAT_109f8404 (*(undefined4 *)0x109F8404u)
typedef uint32_t (__stdcall *HeapFree_t)(XI_ANY, XI_ANY, XI_ANY);
#define HeapFree (*(HeapFree_t *)0x10329260u)
#define PTR_LOOP_103d09b8 (*(void * *)0x103D09B8u)
#define PTR_LOOP_103d29d8 (*(undefined * *)0x103D29D8u)
typedef uint32_t (__stdcall *VirtualFree_t)(XI_ANY, XI_ANY, XI_ANY);
#define VirtualFree (*(VirtualFree_t *)0x1032922Cu)
// FUNCTION: FFXIMAIN 0x1031C85A
void __cdecl fn_1031C85A(undefined * * param_1)
{
  VirtualFree(param_1[4],0,0x8000);
  if ((undefined **)PTR_LOOP_103d29d8 == param_1) {
    PTR_LOOP_103d29d8 = param_1[1];
  }
  if (param_1 != &PTR_LOOP_103d09b8) {
    *(undefined **)param_1[1] = *param_1;
    *(undefined **)(*param_1 + 4) = param_1[1];
    HeapFree(DAT_109f8404,0,param_1);
    return;
  }
  DAT_103d09c8_as_v0 = 0xffffffff;
  return;
}