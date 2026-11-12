// CDx -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CDx {
public:
    static _UNKNOWN ** __cdecl GetRuntimeClass();  // 0x10008F80
    void * ScalarDeletingDtor(char a2);  // 0x10009100
    char Initialize();  // 0x100091B0
    char SetDefaultRenderTargets(int a2, int a3, int a4);  // 0x10009620
    int SetClearParams(int a2, int a3, int a4, int a5);  // 0x10009640
    int SetClearColor(int a2);  // 0x10009670
    int EndClearBegin(int a2, int a3, int a4, int a5, int a6, int a7);  // 0x100096B0
    bool TestCooperativeLevel();  // 0x10009710
    static int __cdecl Present();  // 0x10009720
    int BeginScene();  // 0x10009730
    int EndScene();  // 0x10009740
    static void __stdcall SetHardwareCursor(HCURSOR param_1);  // 0x10009750
    static undefined4 __thiscall ShowCursor(void * self, undefined4 param_1);  // 0x10009780
    HCURSOR SetCursorShape(int a2, int a3);  // 0x100097F0
    static undefined4 __thiscall CreateDeviceObjectOnce(void * self, undefined4 param_1, int * param_2);  // 0x10009870
    static undefined4 __thiscall AppendStateRecord(void * self);  // 0x10009D20
    int SetViewport(const void *a2);  // 0x10009DC0
    unsigned __int8 * GetRenderTargetTop();  // 0x10009E00
    int PushRenderTarget(int a2, int a3, int a4);  // 0x10009E20
    static void __thiscall PushRenderTargetTexture_Flag1(void * self, int * param_1, undefined4 xi_s2, undefined4 xi_s3);  // 0x10009EA0
    static void __thiscall PushRenderTargetTexture_Flag0(void * self, int * param_1, undefined4 xi_s2, undefined4 xi_s3);  // 0x10009EE0
    int PushRenderTargetTexture(int a2, int a3, int a4, __int16 *a5, __int16 *a6, int a7, int a8);  // 0x10009F20
    static void __thiscall Call1000A0A0Zeroed(void * self, int * param_1, undefined4 param_2, undefined4 param_3);  // 0x1000A070
    int SetTexture(int a2, int a3);  // 0x1000A1D0
    static void __thiscall PopRenderTargetA(void * self, undefined4 xi_s1, undefined4 xi_s2);  // 0x1000A200
    static void __thiscall PopRenderTargetB(void * self, undefined4 xi_s1, undefined4 xi_s2);  // 0x1000A230
    static void __thiscall PopRenderTargetDefault(void * self, undefined4 xi_s1, undefined4 xi_s2);  // 0x1000A290
    static void __thiscall EndAndClearBegin(void * self, uint32_t param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x1000A390
    int DrawScreenQuadAtDepth(int a1, int a2);  // 0x1000A3B0
    static void __thiscall DrawTextureToViewport(void * self, undefined4 param_1, undefined4 param_2, undefined4 xi_s3);  // 0x1000A4F0
    static void __thiscall DrawTexturedScreenQuadUnit(void * self, undefined4 param_1, undefined4 xi_s2, undefined4 xi_s3, undefined4 xi_s4);  // 0x1000A550
    int DrawTexturedScreenQuad(int a1, int a2, int a3, int a4, int a5);  // 0x1000A5A0
    _DWORD * CreateTexture(unsigned __int16 width, unsigned __int16 height, _DWORD * a4, int a5, int a6);  // 0x1000A6E0
    char PushRenderTargetRet1(int a2, int a3, int a4);  // 0x1000A800
    int DrawFadeQuad(int a1, int a2);  // 0x1000A820
    int Post_DepthBlurComposite(int a1, int a2);  // 0x1000AAB0
    static uint32_t __thiscall StepFadeCounter(void * self, float param_1, float param_2, int param_3);  // 0x1000AE40
    static void __cdecl NopA();  // 0x1000BB10
    static void __cdecl NopB();  // 0x1000BB20
    char DrawStereoOddFrameOverlay(int a1);  // 0x1000BB90
    int SetTransform(int a2, const void *a3);  // 0x1000BD50
    int GetTransformCached(int a2, void *a3);  // 0x1000BDB0
    int SetLight(int a2, float *a3);  // 0x1000BF30
    int CompositeSceneToBackBuffer(int a1, int a2, int a3, int a4);  // 0x1000C0B0
    int CreateStereoResources(unsigned __int16 a1, unsigned __int16 a2);  // 0x1000C6E0
    int ReleaseStereoResources();  // 0x1000C7C0
    int CreateRampTexture1x16(int a1);  // 0x1000C850
    char CreateRampTextures1024x2(IDirect3DTexture8 **a2, int a3);  // 0x1000C900
    int DrawPrimitive(int a1, int a2, int a3);  // 0x1000CB20
    int DrawIndexedPrimitive(int a1, int a2, int a3, int a4, int a5);  // 0x1000CC00
    int DrawPrimitiveUP(int a1, int a2, int a3, int a4);  // 0x1000CD00
    void ApplyStereoEyeProjection();  // 0x1000CF20
    int RestoreProjection();  // 0x1000CF90

    void **vftable;  // +0x0
    uint8_t _pad_4[0x4];
    IDirect3D8 * pD3D;  // +0x8
    IDirect3DDevice8 * pDevice;  // +0xC
    uint8_t _pad_10[0x80];
    _DWORD * defaultBackBuffer;  // +0x90 default render target: set by CDx::SetDefaultRenderTargets, restored by CDx_PopRenderTarge
    uint8_t defaultDepthSurface;  // +0x94 default depth surface, set with defaultBackBuffer by CDx::SetDefaultRenderTargets
    uint8_t _pad_95[0xC3];
    uint8_t renderTargetDepth;  // +0x158
    uint8_t _pad_159[0xB];
    uint8_t clearFlags;  // +0x164 first of the four values CDx::SetClearParams stores, in IDirect3DDevice8::Clear order (Fla
    uint8_t _pad_165[0x3];
    uint8_t clearColor;  // +0x168 second Clear value
    uint8_t _pad_169[0x3];
    uint8_t clearZ;  // +0x16C third Clear value (Z)
    uint8_t _pad_16D[0x3];
    uint8_t clearStencil;  // +0x170 fourth Clear value (Stencil)
    uint8_t _pad_171[0x69F];
    uint32_t stereoEye;  // +0x810 0 or 1: CDx::DrawPrimitive/DrawIndexedPrimitive/DrawPrimitiveUP draw once per eye, setting
    uint8_t stereoStripeTexture0;  // +0x814 First of the two 1024x2 column-stripe textures made by the first CreateRampTextures1024x2 call in CreateStereoResources.
    uint8_t _pad_815[0x3];
    uint8_t stereoStripeTexture1;  // +0x818 Second texture of the first stripe-texture pair.
    uint8_t _pad_819[0x3];
    uint8_t stereoStripeTexture2;  // +0x81C First texture of the second stripe-texture pair.
    uint8_t _pad_81D[0x3];
    uint8_t stereoStripeTexture3;  // +0x820 Second texture of the second stripe-texture pair.
    uint8_t _pad_821[0x3];
    uint8_t stereoRenderTargetTexture;  // +0x824 Render-target texture created at the requested size for stereo mode and pushed as render target when compositing.
    uint8_t _pad_825[0x7];
    IDirect3DTexture8 * stereoOddLineTexture;  // +0x82C
    uint8_t displayBitDepth;  // +0x830 Bits per pixel of the current display mode format.
    uint8_t _pad_831[0x3];
    D3DCAPS8 caps;  // +0x834
    uint8_t hasHwTransformAndLight;  // +0x908 Device reports hardware transform and lighting.
    uint8_t hasVertexShader11;  // +0x909 Device supports vertex shader version 1.1 or later.
    uint8_t hasPixelShader11;  // +0x90A Device supports pixel shader version 1.1 or later.
    uint8_t _pad_90B[0x1];
    uint8_t vertexShaderVersion;  // +0x90C Supported vertex shader version as major/minor bytes (0x0101 = 1.1).
    uint8_t _pad_90D[0x1];
    uint8_t pixelShaderVersion;  // +0x90E Highest supported pixel shader version as major/minor bytes.
    uint8_t _pad_90F[0x1];
    uint8_t supportsDxt1;  // +0x910 DXT1 textures are supported.
    uint8_t supportsDxt3;  // +0x911 DXT3 textures are supported.
    uint8_t supportsP8Texture;  // +0x912 8-bit palettized (P8) textures are supported.
    uint8_t supportsBlendOp;  // +0x913 Device supports D3DRS_BLENDOP.
    uint8_t supportsTssArgTemp;  // +0x914 Device supports the D3DTA_TEMP texture-stage argument.
    uint8_t supportsFullscreenGamma;  // +0x915 Device supports a full-screen gamma ramp.
    uint8_t _pad_916[0x2];
    uint8_t maxVertexBlendMatrixIndex;  // +0x918 Copy of D3DCAPS8::MaxVertexBlendMatrixIndex.
    uint8_t _pad_919[0x3];
    uint8_t maxMultiSampleType;  // +0x91C Highest of the 2/4-sample multisample types the adapter format supports.
    uint8_t _pad_91D[0x3];
    uint8_t isAtiAdapter;  // +0x920 Adapter's PCI vendor ID is 0x1002 (ATI).
    uint8_t supportsCubeMap;  // +0x921 Device supports cube textures.
    uint8_t _pad_922[0x2];
    uint8_t vertexBufferUsage;  // +0x924 Usage flags passed to CreateVertexBuffer, chosen by hardware T&L support.
    uint8_t _pad_925[0x3];
    uint8_t vertexBufferPool;  // +0x928 D3DPOOL passed to CreateVertexBuffer, chosen by hardware T&L support.
    uint8_t _pad_929[0x3];
    uint8_t supportsAnisotropicFilter;  // +0x92C Device supports anisotropic min/mag filtering with MaxAnisotropy of at least 2.
    uint8_t supportsTableFog;  // +0x92D Device supports table (pixel) fog.
    uint8_t supportsVertexFog;  // +0x92E Device supports vertex fog.
    uint8_t supportsRangeFog;  // +0x92F Device supports range-based vertex fog.
    uint8_t texturesMustBePow2;  // +0x930 Device requires power-of-two texture dimensions.
    uint8_t texturesMustBeSquare;  // +0x931 Device requires square textures.
    uint8_t _pad_932[0x2];
    uint32_t maxTextureWidth;  // +0x934
    uint32_t maxTextureHeight;  // +0x938
    D3DMATRIX cachedWorld;  // +0x93C
    D3DMATRIX cachedView;  // +0x97C
    D3DMATRIX cachedProjection;  // +0x9BC
    uint8_t _pad_9FC[0x4];
    uint8_t field_A00;  // +0xA00
    uint8_t _pad_A01[0x3];
    uint8_t hardwareCursors;  // +0xA04 Array of loaded hardware cursor handles indexed by cursor slot.
    uint8_t _pad_A05[0x47];
    uint8_t field_A4C;  // +0xA4C
    uint8_t _pad_A4D[0x3];
};
#pragma pack(pop)
static_assert(sizeof(CDx) == 0xA50, "CDx: sizeof");
static_assert(XI_OFFSETOF(CDx, pD3D) == 0x8, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, pDevice) == 0xC, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, defaultBackBuffer) == 0x90, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, defaultDepthSurface) == 0x94, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, renderTargetDepth) == 0x158, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, clearFlags) == 0x164, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, clearColor) == 0x168, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, clearZ) == 0x16C, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, clearStencil) == 0x170, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, stereoEye) == 0x810, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, stereoStripeTexture0) == 0x814, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, stereoStripeTexture1) == 0x818, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, stereoStripeTexture2) == 0x81C, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, stereoStripeTexture3) == 0x820, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, stereoRenderTargetTexture) == 0x824, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, stereoOddLineTexture) == 0x82C, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, displayBitDepth) == 0x830, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, caps) == 0x834, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, hasHwTransformAndLight) == 0x908, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, hasVertexShader11) == 0x909, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, hasPixelShader11) == 0x90A, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, vertexShaderVersion) == 0x90C, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, pixelShaderVersion) == 0x90E, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, supportsDxt1) == 0x910, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, supportsDxt3) == 0x911, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, supportsP8Texture) == 0x912, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, supportsBlendOp) == 0x913, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, supportsTssArgTemp) == 0x914, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, supportsFullscreenGamma) == 0x915, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, maxVertexBlendMatrixIndex) == 0x918, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, maxMultiSampleType) == 0x91C, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, isAtiAdapter) == 0x920, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, supportsCubeMap) == 0x921, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, vertexBufferUsage) == 0x924, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, vertexBufferPool) == 0x928, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, supportsAnisotropicFilter) == 0x92C, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, supportsTableFog) == 0x92D, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, supportsVertexFog) == 0x92E, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, supportsRangeFog) == 0x92F, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, texturesMustBePow2) == 0x930, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, texturesMustBeSquare) == 0x931, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, maxTextureWidth) == 0x934, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, maxTextureHeight) == 0x938, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, cachedWorld) == 0x93C, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, cachedView) == 0x97C, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, cachedProjection) == 0x9BC, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_A00) == 0xA00, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, hardwareCursors) == 0xA04, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_A4C) == 0xA4C, "CDx: layout");
