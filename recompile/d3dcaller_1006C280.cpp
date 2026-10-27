/* Task #119: 0x1006C280, a direct caller of the Direct3D-calling functions, at the game's bytes (pipeline/vc6/match.py, /O2). Behaviour: the tree's,
   gate-verified (tree/src/unit_1006C210_Occlusion.cpp); written the way the game's bytes require (recompile/d3d_round_118.md).
   Callees and globals are named by their addresses, so match.py compares every call target and address. */
extern char *g_pCDx_1045666C;
struct FUNt_1006C9B0 { int f(void); };
struct FUNt_1006C300 { int f(void *, int, int, int, int, int, int, char, int); };
struct FUNt_1000BB10 { void f(void); };
struct FUNt_1000BB20 { void f(void); };
struct XS { int Occlusion_QueryScreenRect(float *a2, int a3, int a4, int a5, int a6, int a7, int a8, char a9, int a10); };
// FUNCTION: FFXIMAIN 0x1006C280
int XS::Occlusion_QueryScreenRect(float *a2, int a3, int a4, int a5, int a6, int a7, int a8, char a9, int a10)
{
  unsigned __int8 v11;
  int ScreenRectImpl;
  v11 = *(unsigned __int8 *)(g_pCDx_1045666C + 0x828);
  *(unsigned __int8 *)(g_pCDx_1045666C + 0x828) = 0;
  ((FUNt_1006C9B0 *)this)->f();
  ((FUNt_1000BB20 *)g_pCDx_1045666C)->f();
  ScreenRectImpl = ((FUNt_1006C300 *)this)->f(a2, a3, a4, a5, a6, a7, a8, a9, a10);
  ((FUNt_1000BB10 *)g_pCDx_1045666C)->f();
  *(unsigned __int8 *)(g_pCDx_1045666C + 0x828) = v11;
  return ScreenRectImpl;
}
