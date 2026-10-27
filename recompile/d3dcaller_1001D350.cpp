/* Task #119: 0x1001D350, a direct caller of the Direct3D-calling functions, at the game's bytes (pipeline/vc6/match.py, /O2). Behaviour: the tree's,
   gate-verified (tree/src/unit_1001B890.cpp); written the way the game's bytes require (recompile/d3d_round_118.md).
   Callees and globals are named by their addresses, so match.py compares every call target and address. */
/* the tree's type names (pipeline/vc6/sweep.py vc6_shim.h), those used here */
typedef void *LPVOID; typedef unsigned long DWORD; typedef unsigned int _DWORD; typedef unsigned short _WORD;
extern char g_translucentForce_10456D48;
extern unsigned int g_translucentColor_10456D4C;
extern void *g_pCDx_1045666C;
struct FUNt_1000A1D0 { int f(int, int); };
struct XS { char Model_TranslucentUnbindTexture(); };
// FUNCTION: FFXIMAIN 0x1001D350
char XS::Model_TranslucentUnbindTexture()
{
  _DWORD *self_ = (_DWORD *)this;
  bool b = *(self_ + 2) == 0 || g_translucentForce_10456D48 == 0 && (g_translucentColor_10456D4C & 0xFF000000) == 0x80000000;
  if ( b )
    return 0;
  ((FUNt_1000A1D0 *)g_pCDx_1045666C)->f(0, 0);
  return 1;
}
