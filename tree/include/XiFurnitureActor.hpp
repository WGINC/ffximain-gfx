// XiFurnitureActor -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API XiFurnitureActor {
public:
    static void __cdecl Release(void * param_1);  // 0x100C3E70

    void **vftable;  // +0x0
    uint8_t _pad_4[0x790];
};
#pragma pack(pop)
static_assert(sizeof(XiFurnitureActor) == 0x794, "XiFurnitureActor: sizeof");
