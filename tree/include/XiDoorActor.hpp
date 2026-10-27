// XiDoorActor -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API XiDoorActor {
public:
    static undefined4 __thiscall UpdateAndRunTimer(void * self);  // 0x100AC4E0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x890];
};
#pragma pack(pop)
static_assert(sizeof(XiDoorActor) == 0x894, "XiDoorActor: sizeof");
