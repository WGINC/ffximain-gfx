// CXiAtelActor -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CXiAtelActor {
public:
    static undefined4 __thiscall IsAtelType82(void * self);  // 0x10082960
    static undefined4 __thiscall IsAtelType83(void * self);  // 0x100829C0
    static undefined4 __thiscall CopyAndWrapAngles(void * self);  // 0x10086E90

    void **vftable;  // +0x0
    uint8_t _pad_4[0xD0];
};
#pragma pack(pop)
static_assert(sizeof(CXiAtelActor) == 0xD4, "CXiAtelActor: sizeof");
