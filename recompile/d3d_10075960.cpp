/* Task #118: 0x10075960 at the game's bytes (pipeline/vc6/match.py, /O2). Behaviour: the tree's, gate-verified
   (tree/src/unit_10074CA0.cpp); written the way the game's bytes require, which recompile/d3d_round_118.md describes.
   Callees and globals are named by their addresses, so match.py compares every call target and address. */
/* the tree's type names (pipeline/vc6/sweep.py vc6_shim.h), those used here */
typedef void *LPVOID; typedef unsigned long DWORD; typedef unsigned int _DWORD; typedef unsigned short _WORD;
#define g_pResourceMng (*(int *)0x1047BFA8u)
struct XS { int TexRef_SetTexture(int a2); };
// FUNCTION: FFXIMAIN 0x10075960
int XS::TexRef_SetTexture(int a2)
{
  int * self_ = (int *)this;
  int v2;
  int v3;
  v2 = *self_;
  if ( v2 != 0 )
  {
    if ( *(_DWORD *)(v2 + 64) != 0 )
      v3 = *(_DWORD *)(v2 + 64);
    else
      v3 = *(_DWORD *)(v2 + 68);
    return (*(int (__stdcall **)(_DWORD, int, int))(**(_DWORD **)(g_pResourceMng + 3772) + 244))(
             *(_DWORD *)(g_pResourceMng + 3772),
             a2,
             v3);
  }
  return (*(int (__stdcall **)(_DWORD, int, _DWORD))(**(_DWORD **)(g_pResourceMng + 3772) + 244))(
           *(_DWORD *)(g_pResourceMng + 3772),
           a2,
           0);
}
