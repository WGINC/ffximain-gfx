// CMoD3aLightmapElem -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoD3aLightmapElem {
public:
    int set_field_194(int a2);  // 0x10052C50
    int get_field_194();  // 0x10052C60
    int * ScalarDeletingDtor(char a2);  // 0x10052C70

    void **vftable;  // +0x0
    uint8_t _pad_4[0x190];
    uint32_t field_194;  // +0x194
};
#pragma pack(pop)
static_assert(sizeof(CMoD3aLightmapElem) == 0x198, "CMoD3aLightmapElem: sizeof");
static_assert(XI_OFFSETOF(CMoD3aLightmapElem, field_194) == 0x194, "CMoD3aLightmapElem: layout");
