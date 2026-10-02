// CMoVuLineProgElem -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoVuLineProgElem {
public:
    BOOL vf012(int a1);  // 0x10052BF0
    int * ScalarDeletingDtor(char a2);  // 0x10052C00
    static char ** __cdecl GetRuntimeClass();  // 0x10064170
    void vf018();  // 0x10064180

    void **vftable;  // +0x0
    uint8_t _pad_4[0x1E4];
};
#pragma pack(pop)
static_assert(sizeof(CMoVuLineProgElem) == 0x1E8, "CMoVuLineProgElem: sizeof");
