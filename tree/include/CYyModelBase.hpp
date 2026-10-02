// CYyModelBase -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CYyModelBase {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x100298A0
    int * ScalarDeletingDtor(char a1);  // 0x10029DB0
    int DrawLit(int a2, int a3, int a4);  // 0x1002A3B0

    void **vftable;  // +0x0
    uint8_t _pad_4[0xC0];
    uint32_t field_C4;  // +0xC4
};
#pragma pack(pop)
static_assert(sizeof(CYyModelBase) == 0xC8, "CYyModelBase: sizeof");
static_assert(XI_OFFSETOF(CYyModelBase, field_C4) == 0xC4, "CYyModelBase: layout");
