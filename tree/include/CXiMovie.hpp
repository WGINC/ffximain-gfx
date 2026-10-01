// CXiMovie -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CXiMovie {
public:
    int Draw(int a1, int a2, int a3, int a4, int a5);  // 0x10026AD0
    static int __cdecl DrawFrame();  // 0x1021D4C0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x4];
    uint32_t field_8;  // +0x8
    float field_C;  // +0xC
    float field_10;  // +0x10
    uint8_t _pad_14[0x8];
    uint32_t field_1C;  // +0x1C
    uint8_t _pad_20[0x8];
};
#pragma pack(pop)
static_assert(sizeof(CXiMovie) == 0x28, "CXiMovie: sizeof");
static_assert(XI_OFFSETOF(CXiMovie, field_8) == 0x8, "CXiMovie: layout");
static_assert(XI_OFFSETOF(CXiMovie, field_C) == 0xC, "CXiMovie: layout");
static_assert(XI_OFFSETOF(CXiMovie, field_10) == 0x10, "CXiMovie: layout");
static_assert(XI_OFFSETOF(CXiMovie, field_1C) == 0x1C, "CXiMovie: layout");
