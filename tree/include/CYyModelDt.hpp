// CYyModelDt -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CYyModelDt {
public:
    char PrepareRenderState(int a2, int a3, int a4, int a5, int a6);  // 0x100224D0
    static int __cdecl SetVertexShader(int a1);  // 0x10022FA0
    unsigned __int16 PrepareRenderStateSimple(int a2, int a3, int a4, int a5);  // 0x100231C0
    char PrepareRenderStateEntry(int a2, int a3, int a4, int a5, int a6, int a7);  // 0x10025030
    unsigned __int16 PrepareRenderStateSimpleEntry(int a2, int a3, int a4, int a5, int a6);  // 0x10025060
    __int16 GetElementCount();  // 0x10025180
    int MarkReferencedBones(int a2);  // 0x100251A0
    static char ** __cdecl GetRuntimeClass();  // 0x10029880
    _DWORD * ScalarDeletingDtor(char a1);  // 0x10029CB0
    __int16 SkinAndDrawV2(int a2, int a3, int a4, int a5, _DWORD *a6, _DWORD *a7);  // 0x1002DC10

    void **vftable;  // +0x0
    uint8_t _pad_4[0x4];
    int32_t field_8;  // +0x8
    uint8_t _pad_C[0x27];
    uint8_t field_33;  // +0x33
    uint8_t field_34;  // +0x34
    uint8_t _pad_35[0x7];
};
#pragma pack(pop)
static_assert(sizeof(CYyModelDt) == 0x3C, "CYyModelDt: sizeof");
static_assert(XI_OFFSETOF(CYyModelDt, field_8) == 0x8, "CYyModelDt: layout");
static_assert(XI_OFFSETOF(CYyModelDt, field_33) == 0x33, "CYyModelDt: layout");
static_assert(XI_OFFSETOF(CYyModelDt, field_34) == 0x34, "CYyModelDt: layout");
