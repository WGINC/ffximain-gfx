// CYyModelBase -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CYyModelBase {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x100298A0
    int * ScalarDeletingDtor(char a1);  // 0x10029DB0
    static void __thiscall Destructor(void * self);  // 0x10029DD0
    static void __thiscall SetResourceAndRebuild(void * self, undefined4 param_1);  // 0x10029E50
    static int __thiscall GetPartList(void * self);  // 0x10029E70
    static undefined4 * __thiscall AddPartFromResource(void * self, undefined4 * param_1);  // 0x10029F70
    int DrawLit(int a2, int a3, int a4);  // 0x1002A3B0

    void **vftable;  // +0x0
    uint8_t _pad_4[0xC0];
    uint32_t field_C4;  // +0xC4
};
#pragma pack(pop)
static_assert(sizeof(CYyModelBase) == 0xC8, "CYyModelBase: sizeof");
static_assert(XI_OFFSETOF(CYyModelBase, field_C4) == 0xC4, "CYyModelBase: layout");
