// Obj_10621944 -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API Obj_10621944 {
public:
    int Obj_10621944_Draw();  // 0x101630E0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x8];
    uint32_t field_C;  // +0xC
};
#pragma pack(pop)
static_assert(XI_OFFSETOF(Obj_10621944, field_C) == 0xC, "Obj_10621944: layout");
