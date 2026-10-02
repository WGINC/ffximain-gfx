// CXiActorNameDraw -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CXiActorNameDraw {
public:
    static int __cdecl SetRenderState();  // 0x100817F0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x30];
};
#pragma pack(pop)
static_assert(sizeof(CXiActorNameDraw) == 0x34, "CXiActorNameDraw: sizeof");
