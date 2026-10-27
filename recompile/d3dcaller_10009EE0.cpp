/* Task #119: 0x10009EE0, a direct caller of the Direct3D-calling functions, at the game's bytes (pipeline/vc6/match.py, /O2). Behaviour: the tree's,
   gate-verified (tree/src/CDx.cpp); written the way the game's bytes require (recompile/d3d_round_118.md).
   Callees and globals are named by their addresses, so match.py compares every call target and address. */
/* the tree's type names (pipeline/vc6/sweep.py vc6_shim.h), those used here */
typedef unsigned int undefined4, uint, dword; typedef unsigned __int64 undefined8; typedef unsigned long ulong;
struct FUNt_10009F20 { int f(int, int, int, __int16 *, __int16 *, int, int); };
struct CDx {
  void PushRenderTargetTexture_Flag0(int *param_1, undefined4 xi_s2, undefined4 xi_s3);
};
// FUNCTION: FFXIMAIN 0x10009EE0
void CDx::PushRenderTargetTexture_Flag0(int *param_1, undefined4 xi_s2, undefined4 xi_s3)
{
  __int16 local_4[2];
  local_4[0] = 0;
  local_4[1] = 0;
  ((FUNt_10009F20 *)this)->f((int)param_1, xi_s2, xi_s3, local_4, local_4, 0, 0);
}
