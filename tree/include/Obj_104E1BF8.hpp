// Obj_104E1BF8 -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API Obj_104E1BF8 {
public:
    char ShapeMng_BindFontTexture(unsigned int a2, _DWORD *a3, _DWORD *a4);  // 0x10120590
    char ShapeMng_BindDmgNumTexture(unsigned int a2, _DWORD *a3, _DWORD *a4);  // 0x101207B0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x18];
    int32_t field_1C;  // +0x1C
    uint8_t _pad_20[0x10];
    int32_t field_30;  // +0x30
};
#pragma pack(pop)
static_assert(XI_OFFSETOF(Obj_104E1BF8, field_1C) == 0x1C, "Obj_104E1BF8: layout");
static_assert(XI_OFFSETOF(Obj_104E1BF8, field_30) == 0x30, "Obj_104E1BF8: layout");
