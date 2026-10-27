/* Task #119: 0x1000A550, a direct caller of the Direct3D-calling functions, at the game's bytes (pipeline/vc6/match.py, /O2). Behaviour: the tree's,
   gate-verified (tree/src/CDx.cpp); written the way the game's bytes require (recompile/d3d_round_118.md).
   Callees and globals are named by their addresses, so match.py compares every call target and address. */
/* the tree's type names (pipeline/vc6/sweep.py vc6_shim.h), those used here */
typedef unsigned int undefined4, uint, dword; typedef unsigned __int64 undefined8; typedef unsigned long ulong;
struct FUNt_1000A5A0 { int f(int, int, int, int, int); };
struct CDx {
  void DrawTexturedScreenQuadUnit(undefined4 param_1, undefined4 xi_s2, undefined4 xi_s3, undefined4 xi_s4);
};
// FUNCTION: FFXIMAIN 0x1000A550
void CDx::DrawTexturedScreenQuadUnit(undefined4 param_1, undefined4 xi_s2, undefined4 xi_s3, undefined4 xi_s4)
{
  float uv[4];
  uv[0] = 0.0f;
  uv[1] = 1.0f;
  uv[2] = 0.0f;
  uv[3] = 1.0f;
  ((FUNt_1000A5A0 *)this)->f(param_1, xi_s2, xi_s3, (int)uv, -1);
}
