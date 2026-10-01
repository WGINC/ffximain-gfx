// CMoResource_VT_1032C55C -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoResource_VT_1032C55C {
public:
    void vf008();  // 0x10063C50

    void **vftable;  // +0x0
    uint8_t _pad_4[0x30];
    uint32_t field_34;  // +0x34
};
#pragma pack(pop)
static_assert(sizeof(CMoResource_VT_1032C55C) == 0x38, "CMoResource_VT_1032C55C: sizeof");
static_assert(XI_OFFSETOF(CMoResource_VT_1032C55C, field_34) == 0x34, "CMoResource_VT_1032C55C: layout");
