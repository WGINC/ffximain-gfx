// CMoOT -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoOT {
public:
    int * ScalarDeletingDtor(char a2);  // 0x100552C0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x14];
};
#pragma pack(pop)
static_assert(sizeof(CMoOT) == 0x18, "CMoOT: sizeof");
