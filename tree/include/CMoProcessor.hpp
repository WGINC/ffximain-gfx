// CMoProcessor -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
// NOTE: class descriptor says 0x10; the IDB struct and the code use 0xBB0.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoProcessor {
public:
    _DWORD * InitRotationMatrices();  // 0x1006CE70
    static void __stdcall TransposeInPlace(int param_1);  // 0x1006D3E0
    static long double __stdcall LengthSafe(float * param_1);  // 0x1006D580
    int ScratchMatrixOp(int a2, int a3, int a4);  // 0x1006E6B0
    static CMoProcessor * __stdcall CalcScreenBounds(float *a1, float *a2, int a3, int a4, const void *a5);  // 0x1006E970
    static void __thiscall CalcScreenBoundsAndVisible(void * self, float * param_1, float * param_2, float param_3, float param_4, float param_5, undefined4 xi_s6, undefined4 xi_s7);  // 0x1006ED10
    static float * __stdcall MulPointVec3(float * param_1, float * param_2, float * param_3);  // 0x1006F800
    static undefined4 __thiscall BuildRotationXYZ(void * self, undefined4 param_1, float param_2, float param_3, float param_4);  // 0x1006FAE0
    static undefined4 __thiscall SetRotationFromAngles(void * self, undefined4 param_1, float param_2, float param_3, float param_4);  // 0x1006FC40

    void **vftable;  // +0x0
    float ScratchMatrices[512];  // +0x4
    uint8_t _pad_804[0xC];
    float RotationMatrix[64];  // +0x810
    uint8_t _pad_910[0x100];
    float * ScratchMatrixFreeList[32];  // +0xA10
    int ScratchMatrixStackTop;  // +0xA90
    uint8_t _pad_A94[0xC];
    float TransposeScratch[16];  // +0xAA0
    float TransposeScratchTemp[16];  // +0xAE0
    uint8_t _pad_B20[0x90];
};
#pragma pack(pop)
static_assert(sizeof(CMoProcessor) == 0xBB0, "CMoProcessor: sizeof");
static_assert(XI_OFFSETOF(CMoProcessor, ScratchMatrices) == 0x4, "CMoProcessor: layout");
static_assert(XI_OFFSETOF(CMoProcessor, RotationMatrix) == 0x810, "CMoProcessor: layout");
static_assert(XI_OFFSETOF(CMoProcessor, ScratchMatrixFreeList) == 0xA10, "CMoProcessor: layout");
static_assert(XI_OFFSETOF(CMoProcessor, ScratchMatrixStackTop) == 0xA90, "CMoProcessor: layout");
static_assert(XI_OFFSETOF(CMoProcessor, TransposeScratch) == 0xAA0, "CMoProcessor: layout");
static_assert(XI_OFFSETOF(CMoProcessor, TransposeScratchTemp) == 0xAE0, "CMoProcessor: layout");
