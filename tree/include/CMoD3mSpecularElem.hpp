// CMoD3mSpecularElem -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoD3mSpecularElem {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x10041010
    static undefined8 __thiscall Ctor(void * self);  // 0x10041020
    BOOL vf012(int a1);  // 0x100410E0
    _DWORD * ScalarDeletingDtor(char a2);  // 0x100410F0
    static undefined4 __thiscall BindTintedTexture(void * self);  // 0x10041160

    void **vftable;  // +0x0
    uint8_t _pad_4[0x1BC];
};
#pragma pack(pop)
static_assert(sizeof(CMoD3mSpecularElem) == 0x1C0, "CMoD3mSpecularElem: sizeof");
