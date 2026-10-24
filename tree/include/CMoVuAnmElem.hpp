// CMoVuAnmElem -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoVuAnmElem {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x1003EF10
    BOOL vf012(int a1);  // 0x1003EF80
    int vf008();  // 0x1003EF90
    void * ScalarDeletingDtor(char a2);  // 0x1003EFA0
    static undefined8 __thiscall Construct(void * self);  // 0x10052CD0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x22D];
};
#pragma pack(pop)
static_assert(sizeof(CMoVuAnmElem) == 0x231, "CMoVuAnmElem: sizeof");
