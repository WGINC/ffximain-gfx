// CXi471011C8_VT_1032B478 -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CXi471011C8_VT_1032B478 {
public:
    char RebuildBlendWeights();  // 0x100565A0

    void **vftable;  // +0x0
    uint8_t _pad_4[0xF4];
    uint8_t field_F8;  // +0xF8
    uint8_t _pad_F9[0x9B];
};
#pragma pack(pop)
static_assert(sizeof(CXi471011C8_VT_1032B478) == 0x194, "CXi471011C8_VT_1032B478: sizeof");
static_assert(XI_OFFSETOF(CXi471011C8_VT_1032B478, field_F8) == 0xF8, "CXi471011C8_VT_1032B478: layout");
