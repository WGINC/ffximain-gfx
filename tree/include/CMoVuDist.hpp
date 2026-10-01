// CMoVuDist -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoVuDist {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x100430C0
    BOOL vf012(int a1);  // 0x10052DD0
    int vf008();  // 0x10052DE0
    int * ScalarDeletingDtor(char a2);  // 0x10052DF0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x1DC];
};
#pragma pack(pop)
static_assert(sizeof(CMoVuDist) == 0x1E0, "CMoVuDist: sizeof");
