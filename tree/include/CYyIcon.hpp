// CYyIcon -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CYyIcon {
public:
    int Draw(int a1, int a2, int a3, int a4);  // 0x10026990
    int DrawSubRect(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);  // 0x10026C70

    void **vftable;  // +0x0
    uint8_t _pad_4[0x4];
    uint32_t field_8;  // +0x8
    float field_C;  // +0xC
    float field_10;  // +0x10
    uint8_t _pad_14[0x8];
    uint32_t field_1C;  // +0x1C
    float field_20;  // +0x20
    float field_24;  // +0x24
    float field_28;  // +0x28
    float field_2C;  // +0x2C
    uint8_t _pad_30[0x8];
    uint32_t field_38;  // +0x38
    uint32_t field_3C;  // +0x3C
    uint32_t field_40;  // +0x40
    float field_44;  // +0x44
    float field_48;  // +0x48
    uint8_t _pad_4C[0x8];
    uint32_t field_54;  // +0x54
    uint32_t field_58;  // +0x58
    uint32_t field_5C;  // +0x5C
    float field_60;  // +0x60
    float field_64;  // +0x64
    uint8_t _pad_68[0x8];
    uint32_t field_70;  // +0x70
    float field_74;  // +0x74
    float field_78;  // +0x78
};
#pragma pack(pop)
static_assert(sizeof(CYyIcon) == 0x7C, "CYyIcon: sizeof");
static_assert(XI_OFFSETOF(CYyIcon, field_8) == 0x8, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, field_C) == 0xC, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, field_10) == 0x10, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, field_1C) == 0x1C, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, field_20) == 0x20, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, field_24) == 0x24, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, field_28) == 0x28, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, field_2C) == 0x2C, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, field_38) == 0x38, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, field_3C) == 0x3C, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, field_40) == 0x40, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, field_44) == 0x44, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, field_48) == 0x48, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, field_54) == 0x54, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, field_58) == 0x58, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, field_5C) == 0x5C, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, field_60) == 0x60, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, field_64) == 0x64, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, field_70) == 0x70, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, field_74) == 0x74, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, field_78) == 0x78, "CYyIcon: layout");
