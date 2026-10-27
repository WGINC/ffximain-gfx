/* Task #118: 0x10076520, a Direct3D-calling function, at the game's bytes (pipeline/vc6/match.py, /O2). Behaviour: the tree's,
   gate-verified (tree/src/CMoTask.cpp); written the way the game's bytes require (recompile/d3d_round_118.md, d3d_round_119.md).
   Callees and globals are named by their addresses, so match.py compares every call target and address. */
typedef unsigned char undefined, undefined1, byte, uchar; typedef unsigned short undefined2, ushort, word;
typedef void *LPVOID; typedef unsigned long DWORD; typedef unsigned int _DWORD; typedef unsigned short _WORD;
extern int g_gpDynVBFVF144_1047BBA8;
#define g_pDynVB_FVF144 g_gpDynVBFVF144_1047BBA8
extern int g_pResourceMng_1047BFA8;
#define g_pResourceMng g_pResourceMng_1047BFA8
struct FUNt_10044010 { int f(int); };
struct FUNt_10044130 { void * f(void); };
struct FUNt_100441A0 { int f(void); };
struct FUNt_10075960 { int f(int); };
struct FUNt_10043F40 { int f(int, int, int, unsigned int, int); };
struct XS { int DrawQuadDynVB(float *a2, float *a3, char a4); };
// FUNCTION: FFXIMAIN 0x10076520
int XS::DrawQuadDynVB(float *a2, float *a3, char a4)
{

  int * self_ = reinterpret_cast<int *>(this);  // byte-exact view for the rest
  float *v5;
  ((FUNt_10044010 *)((void *)g_pDynVB_FVF144))->f(4);
  v5 = (float *)((FUNt_10044130 *)((void *)g_pDynVB_FVF144))->f();
  v5[24] = 1.0;
  v5[17] = 1.0;
  v5[10] = 1.0;
  v5[3] = 1.0;
  v5[23] = 0.0;
  v5[25] = -2.3418052e-38;
  v5[18] = -2.3418052e-38;
  v5[11] = -2.3418052e-38;
  v5[4] = -2.3418052e-38;
  v5[16] = 0.0;
  v5[9] = 0.0;
  v5[2] = 0.0;
  v5[0] = v5[14] = a2[0];
  v5[7] = v5[21] = a2[2] + a2[0];
  v5[1] = v5[8] = a2[1];
  v5[15] = v5[22] = a2[3] + a2[1];
  v5[5] = v5[19] = a3[0];
  v5[12] = v5[26] = a3[2];
  v5[6] = v5[13] = a3[1];
  v5[20] = v5[27] = a3[3];
  ((FUNt_100441A0 *)((void *)g_pDynVB_FVF144))->f();
  if ( a4 != 0 )
    ((FUNt_10075960 *)(self_))->f(0);
  { int g10 = g_pResourceMng;
  (*(void (__stdcall **)(_DWORD, _DWORD, int, _DWORD))(**(_DWORD **)(g10 + 3772) + 252))(
    *(_DWORD *)(g10 + 3772),
    0,
    24,
    0);
  }
  { int g9 = g_pResourceMng;
  (*(void (__stdcall **)(_DWORD, _DWORD, int, _DWORD))(**(_DWORD **)(g9 + 3772) + 252))(
    *(_DWORD *)(g9 + 3772),
    0,
    11,
    0);
  }
  { int g8 = g_pResourceMng;
  (*(void (__stdcall **)(_DWORD, _DWORD, int, int))(**(_DWORD **)(g8 + 3772) + 252))(
    *(_DWORD *)(g8 + 3772),
    0,
    1,
    2);
  }
  { int g7 = g_pResourceMng;
  (*(void (__stdcall **)(_DWORD, _DWORD, int, int))(**(_DWORD **)(g7 + 3772) + 252))(
    *(_DWORD *)(g7 + 3772),
    0,
    2,
    2);
  }
  { int g6 = g_pResourceMng;
  (*(void (__stdcall **)(_DWORD, _DWORD, int, int))(**(_DWORD **)(g6 + 3772) + 252))(
    *(_DWORD *)(g6 + 3772),
    0,
    4,
    2);
  }
  { int g5 = g_pResourceMng;
  (*(void (__stdcall **)(_DWORD, _DWORD, int, _DWORD))(**(_DWORD **)(g5 + 3772) + 252))(
    *(_DWORD *)(g5 + 3772),
    0,
    5,
    0);
  }
  { int g4 = g_pResourceMng;
  (*(void (__stdcall **)(_DWORD, _DWORD, int, int))(**(_DWORD **)(g4 + 3772) + 252))(
    *(_DWORD *)(g4 + 3772),
    0,
    16,
    2);
  }
  { int g3 = g_pResourceMng;
  (*(void (__stdcall **)(_DWORD, _DWORD, int, int))(**(_DWORD **)(g3 + 3772) + 252))(
    *(_DWORD *)(g3 + 3772),
    0,
    17,
    2);
  }
  { int g2 = g_pResourceMng;
  (*(void (__stdcall **)(_DWORD, int, int, int))(**(_DWORD **)(g2 + 3772) + 252))(
    *(_DWORD *)(g2 + 3772),
    1,
    1,
    1);
  }
  { int g1 = g_pResourceMng;
  (*(void (__stdcall **)(_DWORD, int, int, int))(**(_DWORD **)(g1 + 3772) + 252))(
    *(_DWORD *)(g1 + 3772),
    1,
    4,
    1);
  }
  return ((FUNt_10043F40 *)((int *)g_pDynVB_FVF144))->f(0, 2, 5, 0, -1);

}
