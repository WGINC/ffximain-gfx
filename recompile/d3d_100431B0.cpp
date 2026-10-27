/* Task #118: 0x100431B0, a Direct3D-calling function, at the game's bytes (pipeline/vc6/match.py, /O2). Behaviour: the tree's,
   gate-verified (tree/src/CMoDistModelElem.cpp); written the way the game's bytes require (recompile/d3d_round_118.md, d3d_round_119.md).
   Callees and globals are named by their addresses, so match.py compares every call target and address. */
typedef unsigned char uint8_t; typedef signed char int8_t; typedef unsigned short uint16_t; typedef short int16_t;
typedef unsigned int uint32_t; typedef int int32_t; typedef unsigned __int64 uint64_t; typedef __int64 int64_t;
typedef unsigned char undefined, undefined1, byte, uchar; typedef unsigned short undefined2, ushort, word;
typedef void *LPVOID; typedef unsigned long DWORD; typedef unsigned int _DWORD; typedef unsigned short _WORD;
typedef unsigned char _BYTE;
struct CMoProcessor; typedef struct CMoProcessor CMoProcessor;
extern char g_Fn100431B0stateBlock_1047BB68[1];
#define Fn100431B0_stateBlock g_Fn100431B0stateBlock_1047BB68
extern char g_Fn100431B0staticGuards_1047BB74;
#define Fn100431B0_staticGuards g_Fn100431B0staticGuards_1047BB74
extern _BYTE g_Fn100431B0textureStageStates_10353260;
#define Fn100431B0_textureStageStates g_Fn100431B0textureStageStates_10353260
extern CMoProcessor * g_pProcessor_1047CF7C;
#define g_pProcessor g_pProcessor_1047CF7C
extern int g_pResourceMng_1047BFA8;
#define g_pResourceMng g_pResourceMng_1047BFA8
extern void * g_pZoneMap_10631F24;
#define g_pZoneMap g_pZoneMap_10631F24
extern int g_scratchValue_1047BC0C;
#define g_scratchValue g_scratchValue_1047BC0C
struct FUNt_100746A0 { int f(void); };
struct FUNt_1000BD50 { int f(int, void *); };
struct exception; typedef struct exception exception;
struct FUNt_1006AD90 { exception * f(void); };
extern "C" int __cdecl FUN_10313486(void *);
struct FUNt_1006AF90 { int f(int); };
struct FUNt_10045D40 { int f(void); };
struct FUNt_10183490 { float * f(void *, int); };
#pragma pack(push, 1)
struct CMoDistModelElem {
  float gx() { return field_120; } float gy() { return field_124; }
  int Draw(void);
  void **vftable;
  uint8_t _pad_4[0x2E];
  uint8_t field_32;
  uint8_t _pad_33[0x1];
  uint8_t field_34;
  uint8_t field_35;
  uint16_t field_36;
  uint8_t _pad_38[0xCC];
  uint32_t field_104;
  uint8_t _pad_108[0x18];
  float field_120;
  float field_124;
  uint8_t _pad_128[0x53];
  uint8_t field_17B;
  uint8_t _pad_17C[0x1C];
};
#pragma pack(pop)
// FUNCTION: FFXIMAIN 0x100431B0
int CMoDistModelElem::Draw(void)
{
  void * self = reinterpret_cast<void *>(this);  // byte-exact view for the rest
  int ScratchMatrixStackTop;
  float *v3;
  CMoProcessor *v4;
  int v5;
  if ( (this->field_17B & 3) != 0 )
  {
    ScratchMatrixStackTop = (*(int *)((char *)g_pProcessor + 0xA90));
    v3 = ((float * *)((char *)g_pProcessor + 0xA10))[ScratchMatrixStackTop];
    (*(int *)((char *)g_pProcessor + 0xA90)) = ScratchMatrixStackTop + 1;
    ((FUNt_100746A0 *)(v3))->f();
    v3[8] = gx();
    v3[9] = gy();
    { int g5 = g_pResourceMng;
    (*(void (__stdcall **)(_DWORD, _DWORD, int, int))(**(_DWORD **)(g5 + 3772) + 252))(
      *(_DWORD *)(g5 + 3772),
      0,
      24,
      2);
    }
    { int gm = g_pResourceMng; ((FUNt_1000BD50 *)(*(_DWORD ***)(gm + 3528)))->f(16, v3); }
    { char *pr = (char *)g_pProcessor; ((float **)(pr + 0xA10))[--*(int *)(pr + 0xA90)] = v3; }
  }
  else
  {
    { int g4 = g_pResourceMng;
    (*(void (__stdcall **)(_DWORD, _DWORD, int, _DWORD))(**(_DWORD **)(g4 + 3772) + 252))(
      *(_DWORD *)(g4 + 3772),
      0,
      11,
      0);
    }
    { int g3 = g_pResourceMng;
    (*(void (__stdcall **)(_DWORD, _DWORD, int, _DWORD))(**(_DWORD **)(g3 + 3772) + 252))(
      *(_DWORD *)(g3 + 3772),
      0,
      24,
      0);
    }
  }
  if ( (Fn100431B0_staticGuards & 1) == 0 )
  {
    Fn100431B0_staticGuards |= 1u;
    ((FUNt_1006AD90 *)((exception *)&Fn100431B0_stateBlock))->f();
    FUN_10313486(((void *)0x10043310u));
  }
  ((FUNt_1006AF90 *)(&Fn100431B0_stateBlock))->f((int)&Fn100431B0_textureStageStates);
  { int g2 = g_pResourceMng;
  (*(void (__stdcall **)(_DWORD, int, int))(**(_DWORD **)(g2 + 3772) + 200))(
    *(_DWORD *)(g2 + 3772),
    60,
    g_scratchValue);
  }
  ((FUNt_10045D40 *)((int)self))->f();
  ((FUNt_10183490 *)((float *)g_pZoneMap))->f((float *)self + 40, this->field_104);
  { int g1 = g_pResourceMng;
  return (*(int (__stdcall **)(_DWORD, _DWORD, int, _DWORD))(**(_DWORD **)(g1 + 3772) + 252))(
           *(_DWORD *)(g1 + 3772),
           0,
           24,
           0);
  }
}
