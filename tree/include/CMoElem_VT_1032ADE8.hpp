// CMoElem_VT_1032ADE8 -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoElem_VT_1032ADE8 {
public:
    void vf18();  // 0x10041AF0
    int * ScalarDeletingDtor(char a2);  // 0x10041FE0

    void **vftable;  // +0x0
    uint8_t _pad_4[0xF8];
    int * field_FC;  // +0xFC
    uint8_t _pad_100[0x94];
};
#pragma pack(pop)
static_assert(sizeof(CMoElem_VT_1032ADE8) == 0x194, "CMoElem_VT_1032ADE8: sizeof");
static_assert(XI_OFFSETOF(CMoElem_VT_1032ADE8, field_FC) == 0xFC, "CMoElem_VT_1032ADE8: layout");
