// CMoSpline -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoSpline {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x10063450
    int * ScalarDeletingDtor(char a2);  // 0x10063480
    static void __thiscall Destructor(void * self);  // 0x100634A0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x24];
};
#pragma pack(pop)
static_assert(sizeof(CMoSpline) == 0x28, "CMoSpline: sizeof");
