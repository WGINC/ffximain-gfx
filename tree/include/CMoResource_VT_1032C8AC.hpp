// CMoResource_VT_1032C8AC -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoResource_VT_1032C8AC {
public:
    int vf004(int a2);  // 0x1001DC20
    unsigned int ** ScalarDeletingDtor(char a2);  // 0x10071A40

    void **vftable;  // +0x0
    uint8_t _pad_4[0x10];
    _DWORD * field_14;  // +0x14
    uint8_t _pad_18[0x18];
};
#pragma pack(pop)
static_assert(sizeof(CMoResource_VT_1032C8AC) == 0x30, "CMoResource_VT_1032C8AC: sizeof");
static_assert(XI_OFFSETOF(CMoResource_VT_1032C8AC, field_14) == 0x14, "CMoResource_VT_1032C8AC: layout");
