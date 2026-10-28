/* 0x10057D60 at the game's bytes: pipeline/vc6/sweep.py's source of it under rule ret64, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); the bytes are its proof. */
#include "vc6_shim.h"
struct XV90_1be6ee { virtual void s0(); virtual void s1(); virtual void s2(); virtual void s3(); virtual void s4(); virtual void s5(); virtual void s6(); virtual void s7(); virtual void s8(); virtual void s9(); virtual void s10(); virtual void s11(); virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15(); virtual void s16(); virtual void s17(); virtual void s18(); virtual void s19(); virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23(); virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27(); virtual void s28(); virtual void s29(); virtual void s30(); virtual void s31(); virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35(); virtual void s36(); virtual void s37(); virtual void s38(); virtual void s39(); virtual void s40(); virtual void s41(); virtual void s42(); virtual void s43(); virtual void s44(); virtual void s45(); virtual void s46(); virtual void s47(); virtual void s48(); virtual void s49(); virtual void s50(); virtual void s51(); virtual void s52(); virtual void s53(); virtual void s54(); virtual void s55(); virtual void s56(); virtual void s57(); virtual void s58(); virtual void s59(); virtual void s60(); virtual void s61(); virtual void s62(); virtual void s63(); virtual void s64(); virtual void s65(); virtual void s66(); virtual void s67(); virtual void s68(); virtual void s69(); virtual void s70(); virtual void s71(); virtual void s72(); virtual void s73(); virtual void s74(); virtual void s75(); virtual void s76(); virtual void s77(); virtual void s78(); virtual void s79(); virtual void s80(); virtual void s81(); virtual void s82(); virtual void s83(); virtual void s84(); virtual void s85(); virtual void s86(); virtual void s87(); virtual void s88(); virtual void s89(); virtual int f(void); };
struct FUNt_100573B0 { void f(void); };
struct FUNt_1003B640 { undefined4 f(void); };
struct FUNt_1003B6D0 { undefined4 f(void); };
struct FUNt_1003B700 { undefined4 f(void); };
struct FUNt_1003B7B0 { template <class A1> void f(A1); };
struct FUNt_1003B830 { template <class A1> undefined4 f(A1); };
struct FUNt_10071E90 { uint64_t f(void); };
struct XS { void fn_10057D60(void * param_1); };
// FUNCTION: FFXIMAIN 0x10057D60
void XS::fn_10057D60(void * param_1)
{
  void *self_ = this;

  void *this_00;
  void *this_01;
  short *psVar1;
  undefined4 xi_eax;
  uint32_t uVar2;
  uint32_t uVar3;
  
  ((FUNt_100573B0 *)(param_1))->f();
  this_00 = (void *)((int)param_1 + 0x34);
  ((FUNt_1003B640 *)(this_00))->f();
  this_01 = (void *)((int)self_ + 0x34);
  uVar2 = ((FUNt_1003B6D0 *)(this_01))->f();
  uVar3 = ((FUNt_1003B700 *)(this_01))->f();
  ((FUNt_1003B7B0 *)(this_00))->f((int)uVar2);
  xi_eax = ((FUNt_1003B830 *)(this_00))->f((int)uVar3);
  ((FUNt_10071E90 *)((void *)**(undefined4 **)((int)param_1 + 0x80)))->f();
  *(undefined4 *)((int)param_1 + 0x18) = 0;
  uVar2 = ((FUNt_1003B6D0 *)(this_01))->f();
  if ((int)uVar2 != 0) {
    uVar2 = ((FUNt_1003B6D0 *)(this_01))->f();
    ((XV90_1be6ee *)(uVar2))->f();
  }
  uVar2 = ((FUNt_1003B700 *)(this_01))->f();
  if ((int)uVar2 != 0) {
    uVar2 = ((FUNt_1003B700 *)(this_01))->f();
    ((XV90_1be6ee *)(uVar2))->f();
  }
  psVar1 = *(short **)((int)self_ + 0x90);
  if (psVar1 != (short *)0x0) {
    *psVar1 = *psVar1 + 1;
  }
  return;

}
