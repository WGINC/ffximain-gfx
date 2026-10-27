/* Task #119: 0x10177C50, a direct caller of the Direct3D-calling functions, at the game's bytes (pipeline/vc6/match.py, /O2). Behaviour: the tree's,
   gate-verified (tree/src/CTsZoneMap.cpp); written the way the game's bytes require (recompile/d3d_round_118.md).
   Callees and globals are named by their addresses, so match.py compares every call target and address. */
extern void *g_pCDx_1045666C;
struct FUNt_1000CD00 { int f(int, int, int, int); };
struct LV { float x, y, z; unsigned int color; float u, v; };
struct V3 { float x, y, z; };
#define D3DCOLOR_ARGB(a,r,g,b) ((unsigned int)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
// FUNCTION: FFXIMAIN 0x10177C50
extern "C" int __cdecl Gfx_DrawLine3D(int a1, int a2, int a3, int a4, int a5, int a6)
{
  LV v[2];
  v[0].x = ((V3 *)a1)->x;
  v[0].y = ((V3 *)a1)->y;
  v[0].z = ((V3 *)a1)->z;
  v[1].x = ((V3 *)a2)->x;
  v[1].y = ((V3 *)a2)->y;
  v[1].z = ((V3 *)a2)->z;
  v[0].color = v[1].color = (unsigned char)a5 | (((unsigned char)a4 | ((unsigned short)((unsigned char)a6 << 8 | (unsigned char)a3) << 8)) << 8);
  return ((FUNt_1000CD00 *)g_pCDx_1045666C)->f(2, 1, (int)v, 24);
}
