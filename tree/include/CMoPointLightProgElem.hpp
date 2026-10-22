// CMoPointLightProgElem -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoPointLightProgElem {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x100562E0
    static undefined8 __thiscall Construct(void * self, uint32_t param_1, undefined4 param_2);  // 0x100562F0
    BOOL vf012(int a1);  // 0x10056360
    int * ScalarDeletingDtor(char a2);  // 0x10056370
    static void __thiscall Update(void * self);  // 0x10056400

    void **vftable;  // +0x0
    uint8_t _pad_4[0x1B0];
};
#pragma pack(pop)
static_assert(sizeof(CMoPointLightProgElem) == 0x1B4, "CMoPointLightProgElem: sizeof");
