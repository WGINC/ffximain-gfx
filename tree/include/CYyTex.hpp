// CYyTex -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CYyTex {
public:
    static _UNKNOWN ** __cdecl GetRuntimeClass();  // 0x10039790
    _DWORD * ScalarDeletingDtor(char a2);  // 0x10039950

    void **vftable;  // +0x0
    uint8_t _pad_4[0x44];
};
#pragma pack(pop)
static_assert(sizeof(CYyTex) == 0x48, "CYyTex: sizeof");
