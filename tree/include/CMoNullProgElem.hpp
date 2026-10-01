// CMoNullProgElem -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoNullProgElem {
public:
    void SetField138();  // 0x100531A0
    int * Destructor(char a2);  // 0x100531B0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x134];
    uint32_t field_138;  // +0x138
    uint8_t _pad_13C[0x58];
};
#pragma pack(pop)
static_assert(sizeof(CMoNullProgElem) == 0x194, "CMoNullProgElem: sizeof");
static_assert(XI_OFFSETOF(CMoNullProgElem, field_138) == 0x138, "CMoNullProgElem: layout");
