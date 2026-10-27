/* Task #119: 0x1001D0A0, a direct caller of the Direct3D-calling functions, at the game's bytes (pipeline/vc6/match.py, /O2). Behaviour: the tree's,
   gate-verified (tree/src/unit_1001B890.cpp); written the way the game's bytes require (recompile/d3d_round_118.md).
   Callees and globals are named by their addresses, so match.py compares every call target and address. */
/* the tree's type names (pipeline/vc6/sweep.py vc6_shim.h), those used here */
typedef void *LPVOID; typedef unsigned long DWORD; typedef unsigned int _DWORD; typedef unsigned short _WORD;
extern void *g_pCDx_1045666C;
extern _DWORD *g_pEnv_104566E8;
extern void *g_pTexture_10456D50;
extern int g_translucentTargetEye2_10456D54;
extern _DWORD g_translucentTarget_10456D58;
extern _DWORD g_translucentDepth_10456D5C;
extern "C" __int16 __cdecl FUN_10015680(void);
extern "C" __int16 __cdecl FUN_10015690(void);
struct FUNt_1000A6E0 { _DWORD *f(unsigned __int16, unsigned __int16, _DWORD *, int, int); };
// FUNCTION: FFXIMAIN 0x1001D0A0
extern "C" _DWORD * __cdecl Model_CreateTranslucentTargets(void)
{
  _DWORD *result;
  g_pTexture_10456D50 = ((FUNt_1000A6E0 *)g_pCDx_1045666C)->f(FUN_10015680(), FUN_10015690(), &g_translucentTarget_10456D58, 1, 0);
  result = g_pEnv_104566E8;
  if ( g_pEnv_104566E8[6] != 0 )
  {
    result = ((FUNt_1000A6E0 *)g_pCDx_1045666C)->f(FUN_10015680(), FUN_10015690(), &g_translucentDepth_10456D5C, 1, 0);
    g_translucentTargetEye2_10456D54 = (int)result;
  }
  else
  {
    g_translucentTargetEye2_10456D54 = 0;
    g_translucentDepth_10456D5C = 0;
  }
  return result;
}
