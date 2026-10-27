// CXiActorDraw -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CXiActorDraw {
public:
    static undefined4 __cdecl Update();  // 0x100817E0
    static undefined4 __cdecl UpdateDrawList();  // 0x10082D70

    void **vftable;  // +0x0
    uint8_t _pad_4[0x30];
};
#pragma pack(pop)
static_assert(sizeof(CXiActorDraw) == 0x34, "CXiActorDraw: sizeof");
