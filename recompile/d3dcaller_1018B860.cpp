/* Task #119: 0x1018B860, a direct caller of the Direct3D-calling functions, at the game's bytes (pipeline/vc6/match.py, /O2). Behaviour: the tree's,
   gate-verified (tree/src/XiZone.cpp); written the way the game's bytes require (recompile/d3d_round_118.md).
   Callees and globals are named by their addresses, so match.py compares every call target and address. */
/* the tree's type names (pipeline/vc6/sweep.py vc6_shim.h), those used here */
typedef unsigned int uint32_t; typedef int int32_t; typedef unsigned __int64 uint64_t; typedef __int64 int64_t;
extern int g_pXiZone_1065CB04;
struct FUNt_101874A0 { uint32_t f(void); };
extern "C" void __cdecl FUN_1003DB60(void);
struct FUNt_1018B8B0 { void f(int); };
struct XiZone { void DrawScene(); };
// FUNCTION: FFXIMAIN 0x1018B860
void XiZone::DrawScene()
{
  int *piVar1;
  if (g_pXiZone_1065CB04 != 0) {
    ((FUNt_101874A0 *)this)->f();
    if (*(int *)((int)this + 0x18) != 0) {
      FUN_1003DB60();
    }
    if (*(int *)((int)this + 0x1c0) != 0) {
      piVar1 = *(int **)(*(int *)((int)this + 0x1c0) + 0x20);
      ((FUNt_1018B8B0 *)this)->f(piVar1 ? *piVar1 : 0);
    }
  }
}
