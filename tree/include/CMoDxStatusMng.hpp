// CMoDxStatusMng -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoDxStatusMng {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x1006B040
    static undefined4 * __thiscall Constructor(void * self);  // 0x1006B050
    int * ScalarDeletingDtor(char a2);  // 0x1006B070

    void **vftable;  // +0x0
    uint8_t _pad_4[0xC];
};
#pragma pack(pop)
static_assert(sizeof(CMoDxStatusMng) == 0x10, "CMoDxStatusMng: sizeof");
