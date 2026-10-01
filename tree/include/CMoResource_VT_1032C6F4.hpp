// CMoResource_VT_1032C6F4 -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoResource_VT_1032C6F4 {
public:
    void vf009();  // 0x1003F430
    unsigned int ** ScalarDeletingDtor(char a2);  // 0x10071B90

    void **vftable;  // +0x0
    uint8_t _pad_4[0x2C];
};
#pragma pack(pop)
static_assert(sizeof(CMoResource_VT_1032C6F4) == 0x30, "CMoResource_VT_1032C6F4: sizeof");
