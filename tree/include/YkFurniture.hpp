// YkFurniture -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API YkFurniture {
public:
    static void __thiscall Destructor(void * self);  // 0x101CD900

    void **vftable;  // +0x0
    uint8_t _pad_4[0xF0];
};
#pragma pack(pop)
static_assert(sizeof(YkFurniture) == 0xF4, "YkFurniture: sizeof");
