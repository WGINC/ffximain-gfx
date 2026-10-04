// CMoTask_VT_10329C70 -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoTask_VT_10329C70 {
public:
    _DWORD * ScalarDeletingDtor(char a2);  // 0x100119A0
    char vf8();  // 0x1006B480
    static _DWORD * __stdcall vf1(_DWORD *a1);  // 0x1006B4C0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x30];
};
#pragma pack(pop)
static_assert(sizeof(CMoTask_VT_10329C70) == 0x34, "CMoTask_VT_10329C70: sizeof");
