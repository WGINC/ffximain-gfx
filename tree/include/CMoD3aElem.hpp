// CMoD3aElem -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoD3aElem {
public:
    static char ** __cdecl GetRuntimeClass_variant0();  // 0x1003E6A0
    static char ** __cdecl GetRuntimeClass_variant1();  // 0x1003EE50
    static char ** __cdecl GetRuntimeClass_variant2();  // 0x1003EF00
    int * ScalarDeletingDtor(char a2);  // 0x10052CB0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x190];
};
#pragma pack(pop)
static_assert(sizeof(CMoD3aElem) == 0x194, "CMoD3aElem: sizeof");
