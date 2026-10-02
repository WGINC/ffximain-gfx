// CMoAttachments -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoAttachments {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x1003B530
    char vf004();  // 0x1003B570
    char vf005();  // 0x1003B580
    int vf006();  // 0x1003B590
    _DWORD * ScalarDeletingDtor(char a2);  // 0x1003B5A0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x3C];
};
#pragma pack(pop)
static_assert(sizeof(CMoAttachments) == 0x40, "CMoAttachments: sizeof");
