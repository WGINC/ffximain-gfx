/* Task #119: 0x10033710, a direct caller of the Direct3D-calling functions, at the game's bytes (pipeline/vc6/match.py, /O2). Behaviour: the tree's,
   gate-verified (tree/src/unit_100310A0.cpp); written the way the game's bytes require (recompile/d3d_round_118.md).
   Callees and globals are named by their addresses, so match.py compares every call target and address. */
/* the tree's type names (pipeline/vc6/sweep.py vc6_shim.h), those used here */
typedef void *LPVOID; typedef unsigned long DWORD; typedef unsigned int _DWORD; typedef unsigned short _WORD;
typedef unsigned char _BYTE;
/* Direct3D 8, from tree/include/xi_d3d8.hpp (MinGW-w64 d3d8.h / d3d8types.h): the enums and structs used here;
   each interface in the SDK's C++ form (d3d8.h DECLARE_INTERFACE_ / STDMETHOD: __stdcall virtual methods in
   vtable order), declared through the slot each call uses */
struct IDirect3DDevice8;
struct IDirect3DSurface8;
struct IDirect3DTexture8;
struct D3DLOCKED_RECT { int Pitch; void *pBits; };
struct IDirect3DSurface8 {
  virtual int __stdcall QueryInterface(unsigned int riid, void ** ppvObject) = 0;
  virtual int __stdcall AddRef() = 0;
  virtual int __stdcall Release() = 0;
  virtual int __stdcall GetDevice(IDirect3DDevice8 ** ppDevice) = 0;
  virtual int __stdcall SetPrivateData(unsigned int refguid, void * data, unsigned int data_size, unsigned int flags) = 0;
  virtual int __stdcall GetPrivateData(unsigned int refguid, void * pData, void * pSizeOfData) = 0;
  virtual int __stdcall FreePrivateData(unsigned int refguid) = 0;
  virtual int __stdcall GetContainer(unsigned int riid, void ** ppContainer) = 0;
  virtual int __stdcall GetDesc(void * pDesc) = 0;
  virtual int __stdcall LockRect(D3DLOCKED_RECT *locked_rect, void * rect, unsigned int flags) = 0;
  virtual int __stdcall UnlockRect() = 0;
};
struct IDirect3DTexture8 {
  virtual int __stdcall QueryInterface(unsigned int riid, void ** ppvObject) = 0;
  virtual int __stdcall AddRef() = 0;
  virtual int __stdcall Release() = 0;
  virtual int __stdcall GetDevice(IDirect3DDevice8 ** ppDevice) = 0;
  virtual int __stdcall SetPrivateData(unsigned int refguid, void * data, unsigned int data_size, unsigned int flags) = 0;
  virtual int __stdcall GetPrivateData(unsigned int refguid, void * pData, void * pSizeOfData) = 0;
  virtual int __stdcall FreePrivateData(unsigned int refguid) = 0;
  virtual int __stdcall SetPriority(unsigned int PriorityNew) = 0;
  virtual int __stdcall GetPriority() = 0;
  virtual int __stdcall PreLoad() = 0;
  virtual int __stdcall GetType() = 0;
  virtual int __stdcall SetLOD(unsigned int LODNew) = 0;
  virtual int __stdcall GetLOD() = 0;
  virtual int __stdcall GetLevelCount() = 0;
  virtual int __stdcall GetLevelDesc(unsigned int Level, void * pDesc) = 0;
  virtual int __stdcall GetSurfaceLevel(unsigned int Level, IDirect3DSurface8 ** ppSurfaceLevel) = 0;
  virtual int __stdcall LockRect(unsigned int level, D3DLOCKED_RECT *locked_rect, void * rect, unsigned int flags) = 0;
  virtual int __stdcall UnlockRect(unsigned int Level) = 0;
  virtual int __stdcall AddDirtyRect(void * dirty_rect) = 0;
};
extern void *g_pCDx_1045666C;
extern void *g_vtbl_1032A844[1];
extern IDirect3DTexture8 *g_targetTex_1045B818;
extern IDirect3DSurface8 *g_targetDepth_1045B81C;
extern float g_1045B810;
extern "C" __int16 __cdecl FUN_10015680(void);
extern "C" __int16 __cdecl FUN_10015690(void);
extern "C" _DWORD *__cdecl FUN_10026E00(void *);
struct FUNt_1000A6E0 { _DWORD *f(unsigned __int16, unsigned __int16, _DWORD *, int, int); };
struct XS { _DWORD *Unit100310A0_Construct(); };
// FUNCTION: FFXIMAIN 0x10033710
_DWORD *XS::Unit100310A0_Construct()
{
  _DWORD *self_ = (_DWORD *)this;
  *self_ = (_DWORD)g_vtbl_1032A844;
  if ( g_targetTex_1045B818 == 0 )
    g_targetTex_1045B818 = (IDirect3DTexture8 *)((FUNt_1000A6E0 *)g_pCDx_1045666C)->f(FUN_10015680(), FUN_10015690(), (_DWORD *)&g_targetDepth_1045B81C, 1, 0);
  if ( g_targetTex_1045B818 != 0 )
    g_targetTex_1045B818->AddRef();

  *(self_ + 2) = (_DWORD)g_targetTex_1045B818;
  if ( g_targetDepth_1045B81C != 0 )
    g_targetDepth_1045B81C->AddRef();
  *(self_ + 3) = (_DWORD)g_targetDepth_1045B81C;
  FUN_10026E00(self_ + 5);
  FUN_10026E00(self_ + 8);
  g_1045B810 = 1.0f;
  *((_BYTE *)self_ + 4) = 0;
  return self_;
}
