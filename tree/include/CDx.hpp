// CDx -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CDx {
public:
    static _UNKNOWN ** __cdecl GetRuntimeClass();  // 0x10008F80
    CDx * Constructor(IDirect3D8 *pD3D, IDirect3DDevice8 *pDevice);  // 0x10008F90
    void * ScalarDeletingDtor(char a2);  // 0x10009100
    char SetDefaultRenderTargets(int a2, int a3, int a4);  // 0x10009620
    int SetClearParams(int a2, int a3, int a4, int a5);  // 0x10009640
    int SetClearColor(int a2);  // 0x10009670
    int EndClearBegin(int a2, int a3, int a4, int a5, int a6, int a7);  // 0x100096B0
    bool TestCooperativeLevel();  // 0x10009710
    static int __cdecl Present();  // 0x10009720
    int BeginScene();  // 0x10009730
    int EndScene();  // 0x10009740
    HCURSOR SetCursorShape(int a2, int a3);  // 0x100097F0
    int SetViewport(const void *a2);  // 0x10009DC0
    unsigned __int8 * GetRenderTargetTop();  // 0x10009E00
    int PushRenderTarget(int a2, int a3, int a4);  // 0x10009E20
    int SetTexture(int a2, int a3);  // 0x1000A1D0
    int DrawScreenQuadAtDepth(int a1, int a2);  // 0x1000A3B0
    char PushRenderTargetRet1(int a2, int a3, int a4);  // 0x1000A800
    int Post_DepthBlurComposite(int a1, int a2);  // 0x1000AAB0
    int SetTransform(int a2, const void *a3);  // 0x1000BD50
    int GetTransformCached(int a2, void *a3);  // 0x1000BDB0
    int SetLight(int a2, float *a3);  // 0x1000BF30
    int CreateStereoResources(int a1, int a2);  // 0x1000C6E0
    char CreateRampTextures1024x2(IDirect3DTexture8 **a2, int a3);  // 0x1000C900
    char GetStereoSecondEyeTarget(int a1, int a2);  // 0x1000CA00
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
    uint8_t field_90;  // +0x90
    uint8_t _pad_91[0x3];
    uint8_t field_94;  // +0x94
    uint8_t _pad_95[0xC3];
    uint8_t field_158;  // +0x158
    uint8_t _pad_159[0xB];
    uint8_t field_164;  // +0x164
    uint8_t _pad_165[0x3];
    uint8_t field_168;  // +0x168
    uint8_t _pad_169[0x3];
    uint8_t field_16C;  // +0x16C
    uint8_t _pad_16D[0x3];
    uint8_t field_170;  // +0x170
    uint8_t _pad_171[0x3];
    uint8_t field_174;  // +0x174
    uint8_t _pad_175[0x3];
    uint8_t field_178;  // +0x178
    uint8_t _pad_179[0x3];
    uint8_t field_17C;  // +0x17C
    uint8_t _pad_17D[0x3];
    uint8_t field_180;  // +0x180
    uint8_t _pad_181[0x3];
    uint8_t field_184;  // +0x184
    uint8_t _pad_185[0x3];
    uint8_t field_188;  // +0x188
    uint8_t _pad_189[0x3];
    uint8_t field_18C;  // +0x18C
    uint8_t _pad_18D[0x3];
    uint8_t field_190;  // +0x190
    uint8_t _pad_191[0x3];
    _DWORD * field_194;  // +0x194
    uint32_t field_198;  // +0x198
    uint8_t _pad_19C[0xC];
    uint32_t field_1A8;  // +0x1A8
    uint8_t field_1AC;  // +0x1AC
    uint8_t _pad_1AD[0x2F];
    uint8_t field_1DC;  // +0x1DC
    uint8_t _pad_1DD[0xF];
    uint8_t field_1EC;  // +0x1EC
    uint8_t _pad_1ED[0x3];
    uint8_t field_1F0;  // +0x1F0
    uint8_t _pad_1F1[0x3];
    uint8_t field_1F4;  // +0x1F4
    uint8_t _pad_1F5[0x61B];
    uint32_t field_810;  // +0x810
    uint8_t field_814;  // +0x814
    uint8_t _pad_815[0x3];
    uint8_t field_818;  // +0x818
    uint8_t _pad_819[0x3];
    uint8_t field_81C;  // +0x81C
    uint8_t _pad_81D[0x3];
    uint8_t field_820;  // +0x820
    uint8_t _pad_821[0x3];
    uint8_t field_824;  // +0x824
    uint8_t _pad_825[0x3];
    uint8_t field_828;  // +0x828
    uint8_t _pad_829[0x3];
    uint8_t field_82C;  // +0x82C
    uint8_t _pad_82D[0xDB];
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
    uint8_t field_916;  // +0x916
    uint8_t _pad_917[0x1];
    uint8_t field_918;  // +0x918
    uint8_t _pad_919[0x3];
    uint8_t field_91C;  // +0x91C
    uint8_t _pad_91D[0x3];
    uint8_t field_920;  // +0x920
    uint8_t field_921;  // +0x921
    uint8_t _pad_922[0xA];
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
    uint8_t field_93C;  // +0x93C
    uint8_t _pad_93D[0x3F];
    uint8_t field_97C;  // +0x97C
    uint8_t _pad_97D[0x3F];
    uint8_t field_9BC;  // +0x9BC
    uint8_t _pad_9BD[0x3F];
    uint8_t field_9FC;  // +0x9FC
    uint8_t _pad_9FD[0x53];
};
#pragma pack(pop)
static_assert(sizeof(CDx) == 0xA50, "CDx: sizeof");
static_assert(XI_OFFSETOF(CDx, pD3D) == 0x8, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, pDevice) == 0xC, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_90) == 0x90, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_94) == 0x94, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_158) == 0x158, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_164) == 0x164, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_168) == 0x168, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_16C) == 0x16C, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_170) == 0x170, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_174) == 0x174, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_178) == 0x178, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_17C) == 0x17C, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_180) == 0x180, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_184) == 0x184, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_188) == 0x188, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_18C) == 0x18C, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_190) == 0x190, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_194) == 0x194, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_198) == 0x198, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_1A8) == 0x1A8, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_1AC) == 0x1AC, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_1DC) == 0x1DC, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_1EC) == 0x1EC, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_1F0) == 0x1F0, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_1F4) == 0x1F4, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_810) == 0x810, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_814) == 0x814, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_818) == 0x818, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_81C) == 0x81C, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_820) == 0x820, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_824) == 0x824, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_828) == 0x828, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_82C) == 0x82C, "CDx: layout");
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
static_assert(XI_OFFSETOF(CDx, field_916) == 0x916, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_918) == 0x918, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_91C) == 0x91C, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_920) == 0x920, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_921) == 0x921, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_92C) == 0x92C, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_92D) == 0x92D, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_92E) == 0x92E, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_92F) == 0x92F, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_930) == 0x930, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_931) == 0x931, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_934) == 0x934, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_938) == 0x938, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_93C) == 0x93C, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_97C) == 0x97C, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_9BC) == 0x9BC, "CDx: layout");
static_assert(XI_OFFSETOF(CDx, field_9FC) == 0x9FC, "CDx: layout");
