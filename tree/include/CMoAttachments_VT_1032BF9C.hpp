// CMoAttachments_VT_1032BF9C -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoAttachments_VT_1032BF9C {
public:
    _DWORD * vf001(char a2);  // 0x10063010

    void **vftable;  // +0x0
    uint8_t _pad_4[0x3C];
};
#pragma pack(pop)
static_assert(sizeof(CMoAttachments_VT_1032BF9C) == 0x40, "CMoAttachments_VT_1032BF9C: sizeof");
