// CMoDisintegrateProgElem -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoDisintegrateProgElem {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x100426B0
    void vf018();  // 0x100426C0
    BOOL vf012(int a1);  // 0x10053130
    int vf008();  // 0x10053140
    int * ScalarDeletingDtor(char a2);  // 0x10053150

    void **vftable;  // +0x0
    uint8_t _pad_4[0x1DC];
};
#pragma pack(pop)
static_assert(sizeof(CMoDisintegrateProgElem) == 0x1E0, "CMoDisintegrateProgElem: sizeof");
