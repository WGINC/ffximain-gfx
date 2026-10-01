// CMoAttachments_VT_1032C02C -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoAttachments_VT_1032C02C {
public:
    _DWORD * vf001(char a2);  // 0x10063090

    void **vftable;  // +0x0
    uint8_t _pad_4[0x3C];
};
#pragma pack(pop)
static_assert(sizeof(CMoAttachments_VT_1032C02C) == 0x40, "CMoAttachments_VT_1032C02C: sizeof");
