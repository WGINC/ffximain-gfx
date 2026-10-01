// Obj_104E1B90 -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API Obj_104E1B90 {
public:
    int fn_1011C080(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);  // 0x1011C080

    void **vftable;  // +0x0
    uint8_t _pad_4[0xC];
    uint32_t field_10;  // +0x10
    uint8_t _pad_14[0x18];
    uint32_t field_2C;  // +0x2C
    uint8_t _pad_30[0x18];
    uint32_t field_48;  // +0x48
    uint8_t _pad_4C[0x18];
    uint32_t field_64;  // +0x64
};
#pragma pack(pop)
static_assert(XI_OFFSETOF(Obj_104E1B90, field_10) == 0x10, "Obj_104E1B90: layout");
static_assert(XI_OFFSETOF(Obj_104E1B90, field_2C) == 0x2C, "Obj_104E1B90: layout");
static_assert(XI_OFFSETOF(Obj_104E1B90, field_48) == 0x48, "Obj_104E1B90: layout");
static_assert(XI_OFFSETOF(Obj_104E1B90, field_64) == 0x64, "Obj_104E1B90: layout");
