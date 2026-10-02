// CMoCameraTask -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoCameraTask {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x1003CC70
    _DWORD * ScalarDeletingDtor(char a1);  // 0x1003D010

    void **vftable;  // +0x0
    uint8_t _pad_4[0xC4];
};
#pragma pack(pop)
static_assert(sizeof(CMoCameraTask) == 0xC8, "CMoCameraTask: sizeof");
