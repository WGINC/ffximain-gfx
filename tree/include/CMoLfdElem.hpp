// CMoLfdElem -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoLfdElem {
public:
    BOOL vf012(int a1);  // 0x10053050
    _DWORD * ScalarDeletingDtor(char a2);  // 0x10053060
    static char ** __cdecl GetRuntimeClass();  // 0x100548B0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x19C];
};
#pragma pack(pop)
static_assert(sizeof(CMoLfdElem) == 0x1A0, "CMoLfdElem: sizeof");
