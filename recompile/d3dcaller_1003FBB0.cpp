/* Task #119: 0x1003FBB0, a direct caller of the render core not in the tree, written from the game's bytes (the same
   test as 0x10040BA0, recompile/d3dcaller_notree_r120.cpp). 0x1003F440 is __thiscall with six stack arguments: it
   reads ECX on entry (data/ecx_use.json: 0x1003F442 mov ebx, ecx) and returns with ret 24 (data/purges.json); ECX is
   the object *(this + 0x104) points to. The 20 bytes copied from +0x194 to +0x1A8 are written as a struct copy (the
   game's rep movsd, ecx = 5). The pointer at +0x104 is read through an inline accessor (`p ? *p : 0`): written out in
   the body, VC6 drops the second null test the game makes after the first; through the accessor it keeps it. The class,
   the accessor's class and the callee's class are stand-ins named by what they hold or their addresses. */
typedef unsigned int undefined4;
extern "C" float __cdecl fn_10002D80(void);
extern undefined4 DAT_1047BC0C;
struct FUNt_1003F440 { void f(void *, void *, void *, void *, int, int); };
struct XS_1003FBB0 { void fn_1003FBB0(); };
struct S20 { int d[5]; };
struct XP { int *p; int get() { return p ? *p : 0; } };
// FUNCTION: FFXIMAIN 0x1003FBB0
void XS_1003FBB0::fn_1003FBB0()
{
  char *self_ = (char *)this;
  int o = *(int **)(self_ + 0xfc) ? **(int **)(self_ + 0xfc) : 0;
  if ((*(char *)(o + 0xde) & 0x80) && fn_10002D80() == 0.0f) return;
  if (((XP *)(self_ + 0x104))->p == 0) return;
  ((FUNt_1003F440 *)(((XP *)(self_ + 0x104))->get()))->f(self_, &DAT_1047BC0C, self_ + 0x194, self_ + 0x1a8, *(int *)(self_ + 0x1bc), 0);
  *(int *)(self_ + 0x1bc) = 0;
  *(S20 *)(self_ + 0x1a8) = *(S20 *)(self_ + 0x194);
}
