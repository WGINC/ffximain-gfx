/* 0x10274690 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's (ghidra_frameset18), outside the tree; the bytes are its proof. */
#include "vc6_shim.h"
// FUNCTION: FFXIMAIN 0x10274690
long double __cdecl fn_10274690(float * xi_s1)
{
  return (float10)xi_s1[2] * (float10)xi_s1[2] +
         (float10)xi_s1[1] * (float10)xi_s1[1] + (float10)*xi_s1 * (float10)*xi_s1;
}