/* 0x101E1AE0 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: an alternative decompiler export's text for this function (not the one the tree takes); the bytes are its proof. */
#include "vc6_shim.h"
struct FUNt_101E24F0 { template <class A1> undefined4 f(A1); };
struct XS { void Unit101D7020_LoadDatListSync(void); };
// FUNCTION: FFXIMAIN 0x101E1AE0
void XS::Unit101D7020_LoadDatListSync(void)
{
  void *self_ = this;

  int *this_00;
  undefined4 uVar1;
  int iVar2;
  
  iVar2 = 0;
  for (this_00 = *(int **)self_; this_00 != (int *)0x0; this_00 = (int *)this_00[8]) {
    if ((*this_00 != 0) && (uVar1 = ((FUNt_101E24F0 *)(this_00))->f(iVar2), (char)uVar1 != '\0')) {
      *this_00 = 2;
    }
    iVar2 = iVar2 + 1;
  }
  return;

}
