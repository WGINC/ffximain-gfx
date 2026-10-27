// YkMyroom -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API YkMyroom {
public:
    static void __thiscall Destructor(void * self);  // 0x101D2F00
    static void __thiscall ReleaseObject38(void * self);  // 0x101D30B0
    static void __thiscall ReleaseResource8(void * self);  // 0x101D31B0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x858];
};
#pragma pack(pop)
static_assert(sizeof(YkMyroom) == 0x85C, "YkMyroom: sizeof");
