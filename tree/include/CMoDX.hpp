// CMoDX -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoDX {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x10069D60
    _BYTE * ScalarDeletingDtor(char a2);  // 0x10069DB0
    _DWORD * CreateAndInitSystem();  // 0x1006B2B0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x1EC8];
};
#pragma pack(pop)
static_assert(sizeof(CMoDX) == 0x1ECC, "CMoDX: sizeof");
