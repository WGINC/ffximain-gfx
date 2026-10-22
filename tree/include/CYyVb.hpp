// CYyVb -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CYyVb {
public:
    static undefined4 __thiscall Init(void * self);  // 0x1003B420
    static void * __thiscall Constructor(void * self, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5);  // 0x1003B450
    static void __thiscall Destroy(void * self);  // 0x1003B4A0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x1C];
};
#pragma pack(pop)
static_assert(sizeof(CYyVb) == 0x20, "CYyVb: sizeof");
