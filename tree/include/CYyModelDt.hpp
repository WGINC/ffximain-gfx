// CYyModelDt -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CYyModelDt {
public:
    static int __cdecl SetVertexShader(int a1);  // 0x10022FA0
    static void * __thiscall Construct(void * self);  // 0x10025010
    char PrepareRenderStateEntry(int a2, int a3, int a4, int a5, int a6, int a7);  // 0x10025030
    unsigned __int16 PrepareRenderStateSimpleEntry(int a2, int a3, int a4, int a5, int a6);  // 0x10025060
    int MarkReferencedBones(int a2);  // 0x100251A0
    static char ** __cdecl GetRuntimeClass();  // 0x10029880
    static void __thiscall InitBase(void * self);  // 0x10029BB0
    static undefined4 * __thiscall Init(void * self);  // 0x10029C90
    _DWORD * ScalarDeletingDtor(char a1);  // 0x10029CB0
    static void __thiscall PrepareRenderStateV2Entry(void * self, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6);  // 0x1002A000
    static undefined4 __thiscall GetResourceWord5E(void * self);  // 0x1002A080
    __int16 SkinAndDrawV2(int a2, int a3, int _180, int a5, _DWORD *a6, _DWORD *a7);  // 0x1002DC10

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
