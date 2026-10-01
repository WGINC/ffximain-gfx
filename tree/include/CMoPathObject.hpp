// CMoPathObject -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoPathObject {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x10055A50

    void **vftable;  // +0x0
    uint8_t _pad_4[0x78];
};
#pragma pack(pop)
static_assert(sizeof(CMoPathObject) == 0x7C, "CMoPathObject: sizeof");
