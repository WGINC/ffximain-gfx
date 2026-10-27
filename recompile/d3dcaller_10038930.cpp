/* Task #119: 0x10038930, a direct caller of the Direct3D-calling functions, at the game's bytes (pipeline/vc6/match.py, /O2). Behaviour: the tree's,
   gate-verified (tree/src/unit_10038930.cpp); written the way the game's bytes require (recompile/d3d_round_118.md).
   Callees and globals are named by their addresses, so match.py compares every call target and address. */
/* the tree's type names (pipeline/vc6/sweep.py vc6_shim.h), those used here */
typedef void *LPVOID; typedef unsigned long DWORD; typedef unsigned int _DWORD; typedef unsigned short _WORD;
/* Direct3D 8, from tree/include/xi_d3d8.hpp (MinGW-w64 d3d8.h / d3d8types.h): the enums and structs used here;
   each interface in the SDK's C++ form (d3d8.h DECLARE_INTERFACE_ / STDMETHOD: __stdcall virtual methods in
   vtable order), declared through the slot each call uses */
struct IDirect3DDevice8;
struct IDirect3DSurface8;
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
extern void *g_pCDx_1045666C;
extern _DWORD *g_pEnv_104566E8;
extern _DWORD g_translucentDepth_10456D5C;
extern int g_sceneCaptureTex_1047B958;
extern int g_sceneCaptureTexEye2_1047B95C;
extern _DWORD g_sceneCaptureDepth_1047B960;
extern _DWORD g_sceneCaptureDepthEye2_1047B964;
struct FUNt_1000A6E0 { _DWORD *f(unsigned __int16, unsigned __int16, _DWORD *, int, int); };
// FUNCTION: FFXIMAIN 0x10038930
extern "C" _DWORD * __cdecl Post_CreateScreenEffectTargets(void)
{
  _DWORD *result;
  g_sceneCaptureDepth_1047B960 = 0;
  g_sceneCaptureTex_1047B958 = (int)((FUNt_1000A6E0 *)g_pCDx_1045666C)->f(0x100u, 0x100u, &g_sceneCaptureDepth_1047B960, 1, 0);
  result = g_pEnv_104566E8;
  if ( g_pEnv_104566E8[6] != 0 )
  {
    g_sceneCaptureDepthEye2_1047B964 = g_translucentDepth_10456D5C;
    if ( g_translucentDepth_10456D5C != 0 )
      ((IDirect3DSurface8 *)g_translucentDepth_10456D5C)->AddRef();
    result = ((FUNt_1000A6E0 *)g_pCDx_1045666C)->f(0x100u, 0x100u, &g_sceneCaptureDepthEye2_1047B964, 1, 0);
    g_sceneCaptureTexEye2_1047B95C = (int)result;
  }
  else
  {
    g_sceneCaptureTexEye2_1047B95C = 0;
    g_sceneCaptureDepthEye2_1047B964 = 0;
  }
  return result;
}
