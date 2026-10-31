/* 0x102EF112 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's (ghidra_frameset18), outside the tree; the bytes are its proof. */
#include "vc6_shim.h"
#define LAB_102ef039 (*(undefined4 *)0x102EF039u)
#define LAB_102ef06a (*(undefined4 *)0x102EF06Au)
#define LAB_102ef101 (*(undefined4 *)0x102EF101u)
#define PTR_s_Bogus_message_code__d_10346b08 (*(undefined4 *)0x10346B08u)
// FUNCTION: FFXIMAIN 0x102EF112
void __cdecl fn_102EF112(undefined4 * param_1)
{
  *param_1 = ((void *)0x102EF001u);
  param_1[1] = &LAB_102ef039;
  param_1[2] = 0x102ef01c;
  param_1[3] = &LAB_102ef06a;
  param_1[4] = &LAB_102ef101;
  param_1[0x1a] = 0;
  param_1[0x1b] = 0;
  param_1[5] = 0;
  param_1[0x1c] = &PTR_s_Bogus_message_code__d_10346b08;
  param_1[0x1d] = 0x77;
  param_1[0x1e] = 0;
  param_1[0x1f] = 0;
  param_1[0x20] = 0;
  return;
}