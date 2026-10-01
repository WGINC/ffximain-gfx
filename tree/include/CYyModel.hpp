// CYyModel -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CYyModel {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x100298C0
    int * ScalarDeletingDtor(char a2);  // 0x1002B530

    void **vftable;  // +0x0
    uint8_t _pad_4[0xB8];
};
#pragma pack(pop)
static_assert(sizeof(CYyModel) == 0xBC, "CYyModel: sizeof");
