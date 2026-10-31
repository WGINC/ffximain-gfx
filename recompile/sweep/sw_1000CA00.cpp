/* 0x1000CA00 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); the bytes are its proof. */
#include "vc6_shim.h"
#include "xi_d3d8_vc6.h"
struct IDirect3DDevice8; typedef struct IDirect3DDevice8 IDirect3DDevice8;
struct IDirect3DSurface8; typedef struct IDirect3DSurface8 IDirect3DSurface8;
struct IDirect3D8;
struct IDirect3DDevice8;
struct IDirect3DTexture8;
struct CDx; typedef struct CDx CDx;
#define g_pCDx (*(CDx * *)0x1045666Cu)
#define g_pEnv (*(_DWORD * *)0x104566E8u)
#define g_translucentTargetEye2 (*(int *)0x10456D54u)
struct IDirect3DTexture8; typedef struct IDirect3DTexture8 IDirect3DTexture8;
#define pTexture (*(IDirect3DTexture8 * *)0x10456D50u)
struct FUNt_1000D660 { char f(void); };
#pragma pack(push, 1)
struct CDx {
  char GetStereoSecondEyeTarget(int a1, int a2);
  void **vftable;
  uint8_t _pad_4[0x4];
  IDirect3D8 * pD3D;
  IDirect3DDevice8 * pDevice;
  uint8_t _pad_10[0x80];
  _DWORD * defaultBackBuffer;
  uint8_t defaultDepthSurface;
  uint8_t _pad_95[0xC3];
  uint8_t renderTargetDepth;
  uint8_t _pad_159[0xB];
  uint8_t clearFlags;
  uint8_t _pad_165[0x3];
  uint8_t clearColor;
  uint8_t _pad_169[0x3];
  uint8_t clearZ;
  uint8_t _pad_16D[0x3];
  uint8_t clearStencil;
  uint8_t _pad_171[0x23];
  _DWORD * sceneTargetA;
  uint32_t sceneTargetB;
  uint8_t _pad_19C[0xC];
  uint32_t sceneTargetBDepth;
  uint8_t _pad_1AC[0x664];
  uint32_t stereoEye;
  uint8_t field_814;
  uint8_t _pad_815[0x3];
  uint8_t field_818;
  uint8_t _pad_819[0x3];
  uint8_t field_81C;
  uint8_t _pad_81D[0x3];
  uint8_t field_820;
  uint8_t _pad_821[0x3];
  uint8_t field_824;
  uint8_t _pad_825[0x3];
  uint8_t stereoTargetsReady;
  uint8_t _pad_829[0x3];
  IDirect3DTexture8 * stereoOddLineTexture;
  uint8_t field_830;
  uint8_t _pad_831[0x3];
  D3DCAPS8 caps;
  uint8_t field_908;
  uint8_t field_909;
  uint8_t field_90A;
  uint8_t _pad_90B[0x1];
  uint8_t field_90C;
  uint8_t _pad_90D[0x1];
  uint8_t field_90E;
  uint8_t _pad_90F[0x1];
  uint8_t field_910;
  uint8_t field_911;
  uint8_t field_912;
  uint8_t field_913;
  uint8_t field_914;
  uint8_t field_915;
  uint8_t _pad_916[0x2];
  uint8_t field_918;
  uint8_t _pad_919[0x3];
  uint8_t field_91C;
  uint8_t _pad_91D[0x3];
  uint8_t field_920;
  uint8_t field_921;
  uint8_t _pad_922[0x2];
  uint8_t field_924;
  uint8_t _pad_925[0x3];
  uint8_t field_928;
  uint8_t _pad_929[0x3];
  uint8_t field_92C;
  uint8_t field_92D;
  uint8_t field_92E;
  uint8_t field_92F;
  uint8_t field_930;
  uint8_t field_931;
  uint8_t _pad_932[0x2];
  uint32_t maxTextureWidth;
  uint32_t maxTextureHeight;
  D3DMATRIX cachedWorld;
  D3DMATRIX cachedView;
  D3DMATRIX cachedProjection;
  uint8_t _pad_9FC[0x4];
  uint8_t field_A00;
  uint8_t _pad_A01[0x3];
  uint8_t field_A04;
  uint8_t _pad_A05[0x47];
  uint8_t field_A4C;
  uint8_t _pad_A4D[0x3];
};
#pragma pack(pop)
// FUNCTION: FFXIMAIN 0x1000CA00
char CDx::GetStereoSecondEyeTarget(int a1, int a2)
{
  char v4;
  IDirect3DSurface8 *v5;
  int v6;
  IDirect3DSurface8 *v7;
  int v8;
  IDirect3DSurface8 *v10;
  IDirect3DSurface8 *v11;
  IDirect3DSurface8 *v12;
  v4 = 0;
  if ( this->stereoTargetsReady != 0 && ((FUNt_1000D660 *)(g_pEnv))->f() != 0 )
  {
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->GetRenderTarget((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), &v11);
    (*(void (__stdcall **)(_DWORD, _DWORD, IDirect3DSurface8 **))(*this->sceneTargetA + 60))(
      (*(uint32_t *)((char *)(this) + (0x194))),
      0,
      &v10);
    (*(void (__stdcall **)(_DWORD, _DWORD, IDirect3DSurface8 **))(*(*(_DWORD * *)((char *)(this) + (0x198))) + 60))(
      this->sceneTargetB,
      0,
      &v12);
    v5 = v10;
    if ( v11 == v10 || v11 == v12 )
    {
      *(_DWORD *)a1 = this->sceneTargetB;
      v6 = this->sceneTargetBDepth;
      *(_DWORD *)a2 = v6;
      v4 = *(_DWORD *)a1 != 0 && v6 != 0;
    }
    v5->Release();
    v12->Release();
    if ( v4 != 1 )
    {
      pTexture->GetSurfaceLevel(0, &v10);
      v7 = v10;
      if ( v11 == v10 )
      {
        *(_DWORD *)a1 = g_translucentTargetEye2;
        v8 = this->sceneTargetBDepth;
        *(_DWORD *)a2 = v8;
        v4 = *(_DWORD *)a1 != 0 && v8 != 0;
      }
      v7->Release();
    }
    if ( v11 != nullptr )
      v11->Release();
  }
  return v4;
}