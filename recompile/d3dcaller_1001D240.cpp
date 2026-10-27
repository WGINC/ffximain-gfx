/* Task #119: 0x1001D240, a direct caller of the Direct3D-calling functions, at the game's bytes (pipeline/vc6/match.py, /O2). Behaviour: the tree's,
   gate-verified (tree/src/unit_1001B890.cpp); written the way the game's bytes require (recompile/d3d_round_118.md).
   Callees and globals are named by their addresses, so match.py compares every call target and address. */
/* the tree's type names (pipeline/vc6/sweep.py vc6_shim.h), those used here */
typedef unsigned char uint8_t; typedef signed char int8_t; typedef unsigned short uint16_t; typedef short int16_t;
typedef void *LPVOID; typedef unsigned long DWORD; typedef unsigned int _DWORD; typedef unsigned short _WORD;
typedef unsigned char _BYTE;
extern char g_translucentForce_10456D48;
extern unsigned int g_translucentColor_10456D4C;
extern _DWORD *g_pEnv_104566E8;
struct FUNt_1000D660 { char f(void); };
struct FUNt_10009E20 { int f(int, int, int); };
struct FUNt_100096B0 { int f(int, int, int, int, int, int); };
struct FUNt_1000A1D0 { int f(int, int); };
#pragma pack(push, 1)
struct CDx {
  uint8_t _pad_0[0x70];
  int depthStack[9];
  int defaultDepthSurface;
  uint8_t _pad_98[0x158 - 0x98];
  uint8_t renderTargetDepth;
};
#pragma pack(pop)
extern CDx *g_pCDx_1045666C;
#define CURZ (g_pCDx_1045666C->renderTargetDepth == 0 ? g_pCDx_1045666C->defaultDepthSurface : g_pCDx_1045666C->depthStack[g_pCDx_1045666C->renderTargetDepth])

struct XS { char Model_BeginTranslucentPass(); };
// FUNCTION: FFXIMAIN 0x1001D240
char XS::Model_BeginTranslucentPass()
{
  int self_ = (int)this;
  int v2;
  v2 = *(_DWORD *)(self_ + 8);
  bool b = v2 == 0 || g_translucentForce_10456D48 == 0 && (g_translucentColor_10456D4C & 0xFF000000) == 0x80000000;
  if ( b )
    return 0;

  if ( *(_BYTE *)(self_ + 4) == 0 )
  {
  if ( ((FUNt_1000D660 *)g_pEnv_104566E8)->f() != 0 )
  {
    ((FUNt_10009E20 *)g_pCDx_1045666C)->f(*(_DWORD *)(self_ + 12), *(_DWORD *)(self_ + 20), 0);
    ((FUNt_100096B0 *)g_pCDx_1045666C)->f(0, 0, 1, 0, 1065353216, 0);
    ((FUNt_1000A1D0 *)g_pCDx_1045666C)->f(0, 0);
  }
  ((FUNt_10009E20 *)g_pCDx_1045666C)->f(*(_DWORD *)(self_ + 8), *(_DWORD *)(self_ + 16), CURZ);
  ((FUNt_100096B0 *)g_pCDx_1045666C)->f(0, 0, 1, 0, 1065353216, 0);
  *(_BYTE *)(self_ + 4) = 1;
  return 1;
  }
    ((FUNt_10009E20 *)g_pCDx_1045666C)->f(v2, *(_DWORD *)(self_ + 16), CURZ);
  return 1;
}
