// CMoLfdElem -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoLfdElem {
public:
    static undefined8 __thiscall Constructor(void * self);  // 0x10053030
    BOOL vf012(int a1);  // 0x10053050
    int * ScalarDeletingDtor(char a2);  // 0x10053060
    static char ** __cdecl GetRuntimeClass();  // 0x100548B0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x19C];
};
#pragma pack(pop)
static_assert(sizeof(CMoLfdElem) == 0x1A0, "CMoLfdElem: sizeof");
