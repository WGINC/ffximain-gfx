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
    HCURSOR SetCursorShape(int a2, int a3);  // 0x100097F0
    static undefined4 __thiscall AppendStateRecord(void * self);  // 0x10009D20
    int SetViewport(const void *a2);  // 0x10009DC0
    unsigned __int8 * GetRenderTargetTop();  // 0x10009E00
    int PushRenderTarget(int a2, int a3, int a4);  // 0x10009E20
    static void __thiscall PushRenderTargetTexture_Flag1(void * self, int * param_1, undefined4 xi_s2, undefined4 xi_s3);  // 0x10009EA0
    static void __thiscall PushRenderTargetTexture_Flag0(void * self, int * param_1, undefined4 xi_s2, undefined4 xi_s3);  // 0x10009EE0
    int PushRenderTargetTexture(int a2, int a3, int a4, __int16 *a5, __int16 *a6, int a7, int a8);  // 0x10009F20
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
    uint8_t renderTargetDepth;  // +0x158 index into the 24-byte viewport/target stack entries (CDx::SetViewport writes entry [n], C
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
    uint8_t field_814;  // +0x814
    uint8_t _pad_815[0x3];
    uint8_t field_818;  // +0x818
    uint8_t _pad_819[0x3];
    uint8_t field_81C;  // +0x81C
    uint8_t _pad_81D[0x3];
    uint8_t field_820;  // +0x820
    uint8_t _pad_821[0x3];
    uint8_t field_824;  // +0x824
    uint8_t _pad_825[0x7];
    uint8_t stereoOddLineTexture;  // +0x82C the 1x16 texture CDx_DrawStereoOddFrameOverlay stretches over the screen in stereo mode (I
    uint8_t _pad_82D[0x3];
    uint8_t field_830;  // +0x830
    uint8_t _pad_831[0x3];
    uint8_t field_834;  // +0x834
    uint8_t _pad_835[0xD3];
    uint8_t field_908;  // +0x908
    uint8_t field_909;  // +0x909
    uint8_t field_90A;  // +0x90A
    uint8_t _pad_90B[0x1];
    uint8_t field_90C;  // +0x90C
    uint8_t _pad_90D[0x1];
    uint8_t field_90E;  // +0x90E
    uint8_t _pad_90F[0x1];
    uint8_t field_910;  // +0x910
    uint8_t field_911;  // +0x911
    uint8_t field_912;  // +0x912
    uint8_t field_913;  // +0x913
    uint8_t field_914;  // +0x914
    uint8_t field_915;  // +0x915
    uint8_t _pad_916[0x2];
    uint8_t field_918;  // +0x918
    uint8_t _pad_919[0x3];
    uint8_t field_91C;  // +0x91C
    uint8_t _pad_91D[0x3];
    uint8_t field_920;  // +0x920
    uint8_t field_921;  // +0x921
    uint8_t _pad_922[0x2];
    uint8_t field_924;  // +0x924
    uint8_t _pad_925[0x3];
    uint8_t field_928;  // +0x928
    uint8_t _pad_929[0x3];
    uint8_t field_92C;  // +0x92C
    uint8_t field_92D;  // +0x92D
    uint8_t field_92E;  // +0x92E
    uint8_t field_92F;  // +0x92F
    uint8_t field_930;  // +0x930
    uint8_t field_931;  // +0x931
    uint8_t _pad_932[0x2];
    uint8_t field_934;  // +0x934
    uint8_t _pad_935[0x3];
    uint8_t field_938;  // +0x938
    uint8_t _pad_939[0x3];
    uint8_t cachedWorld;  // +0x93C D3DTS_WORLD (256) matrix returned by CDx::GetTransformCached
    uint8_t _pad_93D[0x3F];
    uint8_t cachedView;  // +0x97C D3DTS_VIEW (2) matrix returned by CDx::GetTransformCached
    uint8_t _pad_97D[0x3F];
    uint8_t cachedProjection;  // +0x9BC D3DTS_PROJECTION (3) matrix returned by CDx::GetTransformCached
    uint8_t _pad_9BD[0x43];
    uint8_t field_A00;  // +0xA00
    uint8_t _pad_A01[0x3];
    uint8_t field_A04;  // +0xA04
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
static_assert(XI_OFFSETOF(CDx, field_814) == 0x814, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_818) == 0x818, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_81C) == 0x81C, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_820) == 0x820, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_824) == 0x824, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, stereoOddLineTexture) == 0x82C, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_830) == 0x830, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_834) == 0x834, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_908) == 0x908, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_909) == 0x909, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_90A) == 0x90A, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_90C) == 0x90C, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_90E) == 0x90E, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_910) == 0x910, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_911) == 0x911, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_912) == 0x912, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_913) == 0x913, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_914) == 0x914, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_915) == 0x915, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_918) == 0x918, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_91C) == 0x91C, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_920) == 0x920, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_921) == 0x921, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_924) == 0x924, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_928) == 0x928, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_92C) == 0x92C, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_92D) == 0x92D, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_92E) == 0x92E, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_92F) == 0x92F, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_930) == 0x930, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_931) == 0x931, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_934) == 0x934, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_938) == 0x938, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, cachedWorld) == 0x93C, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, cachedView) == 0x97C, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, cachedProjection) == 0x9BC, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_A00) == 0xA00, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_A04) == 0xA04, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_A4C) == 0xA4C, "CDx: layout");
