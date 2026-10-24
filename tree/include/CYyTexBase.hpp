// CYyTexBase -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CYyTexBase {
public:
    static char ** __cdecl GetRuntimeClass_variant0();  // 0x10039770
    static char ** __cdecl GetRuntimeClass_variant1();  // 0x10039780
    static void * __thiscall Constructor(void * self);  // 0x10039830
    _DWORD * ScalarDeletingDtor(char a2);  // 0x100398B0
    static void __thiscall SetName(void * self, undefined4 * param_1);  // 0x100398E0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x30];
};
#pragma pack(pop)
static_assert(sizeof(CYyTexBase) == 0x34, "CYyTexBase: sizeof");
