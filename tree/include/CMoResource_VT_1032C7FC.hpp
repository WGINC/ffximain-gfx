// CMoResource_VT_1032C7FC -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoResource_VT_1032C7FC {
public:
    int vf004(int a2);  // 0x10019A30
    void vf005(int a1);  // 0x10019A40
    void vf009();  // 0x10071A00
    unsigned int ** ScalarDeletingDtor(char a2);  // 0x10071AC0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x10];
    _DWORD * field_14;  // +0x14
    uint8_t _pad_18[0x18];
};
#pragma pack(pop)
static_assert(sizeof(CMoResource_VT_1032C7FC) == 0x30, "CMoResource_VT_1032C7FC: sizeof");
static_assert(XI_OFFSETOF(CMoResource_VT_1032C7FC, field_14) == 0x14, "CMoResource_VT_1032C7FC: layout");
