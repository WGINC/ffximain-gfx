// CEnv -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CEnv {
public:
    static void __thiscall UpdateStereoMode(void * self, undefined4 xi_s1);  // 0x1000D630
    static undefined4 __thiscall IsStereoMode(void * self);  // 0x1000D660

    void **vftable;  // +0x0
    uint8_t _pad_4[0x20];
};
#pragma pack(pop)
static_assert(sizeof(CEnv) == 0x24, "CEnv: sizeof");
