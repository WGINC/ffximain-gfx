// CAtelIdleTask -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CAtelIdleTask {
public:
    static uint32_t __cdecl UpdateFrame();  // 0x100106C0
    static undefined4 __cdecl Create();  // 0x100142B0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x30];
};
#pragma pack(pop)
static_assert(sizeof(CAtelIdleTask) == 0x34, "CAtelIdleTask: sizeof");
