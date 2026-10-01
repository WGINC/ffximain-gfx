// CMoSphRes -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoSphRes {
public:
    char * vf008();  // 0x100562B0
    unsigned int ** ScalarDeletingDtor(char a2);  // 0x10071D00

    void **vftable;  // +0x0
    uint8_t _pad_4[0x4C];
};
#pragma pack(pop)
static_assert(sizeof(CMoSphRes) == 0x50, "CMoSphRes: sizeof");
