/* 0x100F2A70 at the game's bytes: pipeline/vc6/sweep.py's source of it under rule retvoid, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); the bytes are its proof. */
#include "vc6_shim.h"
template <class A0> undefined8 __cdecl FUN_10312EAD(A0);
struct XS { void fn_100F2A70(void); };
// FUNCTION: FFXIMAIN 0x100F2A70
void XS::fn_100F2A70(void)
{
  void *self_ = this;

  code *pcVar1;
  undefined4 uVar2;
  undefined8 uVar3;
  
  if ((*(int *)((int)self_ + 0x105c) != 0) &&
     (pcVar1 = *(code **)(*(int *)((int)self_ + 0x105c) + (uint)*(ushort *)((int)self_ + 0x105a) * 4),
     pcVar1 != (code *)0x0)) {
    (*pcVar1)(self_,2);
    *(undefined4 *)((int)self_ + 0x105c) = 0;
  }
  if (*(code **)((int)self_ + 0x104c) != (code *)0x0) {
    (**(code **)((int)self_ + 0x104c))(*(undefined4 *)((int)self_ + 0x1048),0,1,0);
  }
  if (*(code **)((int)self_ + 0x1050) != (code *)0x0) {
    (**(code **)((int)self_ + 0x1050))(*(undefined4 *)((int)self_ + 0x1048),0,1,0,0);
  }
  if (*(code **)((int)self_ + 0x1054) != (code *)0x0) {
    (**(code **)((int)self_ + 0x1054))(*(undefined4 *)((int)self_ + 0x1048),0,1,0,0);
  }
  *(undefined2 *)((int)self_ + 0x1058) = 0;
  *(undefined4 *)((int)self_ + 0x104c) = 0;
  *(undefined4 *)((int)self_ + 0x1050) = 0;
  *(undefined4 *)((int)self_ + 0x1054) = 0;
  *(undefined4 *)((int)self_ + 0x1048) = 0;
  *(undefined2 *)((int)self_ + 0x105a) = 0;
  if (*(LPVOID *)((int)self_ + 0x9c) != (LPVOID)0x0) {
    FUN_10312EAD(*(LPVOID *)((int)self_ + 0x9c));
    *(undefined4 *)((int)self_ + 0x9c) = 0;
    *(undefined4 *)((int)self_ + 0xa0) = 0;
    *(undefined4 *)((int)self_ + 0xa4) = 0;
  }
  uVar2 = 0;
  if (*(LPVOID *)((int)self_ + 0xac) != (LPVOID)0x0) {
    uVar3 = FUN_10312EAD(*(LPVOID *)((int)self_ + 0xac));
    uVar2 = (undefined4)uVar3;
    *(undefined4 *)((int)self_ + 0xac) = 0;
    *(undefined4 *)((int)self_ + 0xb0) = 0;
    *(undefined4 *)((int)self_ + 0xb4) = 0;
  }
  *(undefined4 *)((int)self_ + 0xd0) = 0;
  *(undefined1 *)((int)self_ + 0x68) = 0;
  *(undefined1 *)((int)self_ + 0x78) = 0;
  *(undefined1 *)((int)self_ + 0x88) = 0;
  return;

}
