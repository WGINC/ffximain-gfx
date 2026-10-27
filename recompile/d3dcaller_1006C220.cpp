/* Task #119: 0x1006C220, a direct caller of the Direct3D-calling functions, at the game's bytes (pipeline/vc6/match.py, /O2). Behaviour: the tree's,
   gate-verified (tree/src/CMoOcclusionMng.cpp); written the way the game's bytes require (recompile/d3d_round_118.md).
   Callees and globals are named by their addresses, so match.py compares every call target and address. */
/* the tree's type names (pipeline/vc6/sweep.py vc6_shim.h), those used here */
typedef void *LPVOID; typedef unsigned long DWORD; typedef unsigned int _DWORD; typedef unsigned short _WORD;
extern void *g_pCDx_1045666C;
struct FUNt_1000A6E0 { _DWORD *f(unsigned __int16, unsigned __int16, _DWORD *, int, int); };
#pragma pack(push, 1)
struct CMoOcclusionMng {
  _DWORD *OcclusionMng_CreateTarget();
  char _pad0[0xC];
  _DWORD *field_C;
  int field_10;
};
#pragma pack(pop)
// FUNCTION: FFXIMAIN 0x1006C220
_DWORD *CMoOcclusionMng::OcclusionMng_CreateTarget()
{
  _DWORD *result;
  int a4;
  a4 = 0;
  result = ((FUNt_1000A6E0 *)g_pCDx_1045666C)->f(0x10u, 0x10u, (_DWORD *)&a4, 1, 0);
  field_C = result;
  field_10 = a4;
  return result;
}
