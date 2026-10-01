// CMoDistRingElem -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoDistRingElem {
public:
    void AllocRingGeometry();  // 0x10043720
    int set_field_1B6(int a2);  // 0x10053000
    int * ScalarDeletingDtor(char a2);  // 0x10053010

    void **vftable;  // +0x0
    uint8_t _pad_4[0x1B0];
    int8_t field_1B4;  // +0x1B4
    int8_t field_1B5;  // +0x1B5
    uint32_t field_1B6;  // +0x1B6
};
#pragma pack(pop)
static_assert(sizeof(CMoDistRingElem) == 0x1BA, "CMoDistRingElem: sizeof");
static_assert(XI_OFFSETOF(CMoDistRingElem, field_1B4) == 0x1B4, "CMoDistRingElem: layout");
static_assert(XI_OFFSETOF(CMoDistRingElem, field_1B5) == 0x1B5, "CMoDistRingElem: layout");
static_assert(XI_OFFSETOF(CMoDistRingElem, field_1B6) == 0x1B6, "CMoDistRingElem: layout");
