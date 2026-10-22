// CMoNullProgElem -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoNullProgElem {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x10047560
    static undefined8 __thiscall Construct(void * self);  // 0x10053180
    int * Destructor(char a2);  // 0x100531B0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x190];
};
#pragma pack(pop)
static_assert(sizeof(CMoNullProgElem) == 0x194, "CMoNullProgElem: sizeof");
