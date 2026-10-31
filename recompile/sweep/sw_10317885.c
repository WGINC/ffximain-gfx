/* 0x10317885 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: an alternative decompiler export's text for this function (not the one the tree takes); the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
size_t __cdecl FUN_10317440(unsigned int);
void * __cdecl FUN_10312F96(unsigned int);
uint64_t __cdecl FUN_103174C0(unsigned int, unsigned int);
// FUNCTION: FFXIMAIN 0x10317885
uint32_t * __cdecl fn_10317885(uint32_t * param_1)
{
  size_t sVar1;
  uint *puVar2;
  
  if (param_1 != (uint *)0x0) {
    sVar1 = FUN_10317440((char *)param_1);
    puVar2 = FUN_10312F96(sVar1 + 1);
    if (puVar2 != (uint *)0x0) {
      puVar2 = FUN_103174C0(puVar2, param_1);
      return puVar2;
    }
  }
  return (uint *)0x0;
}