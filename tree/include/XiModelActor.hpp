// XiModelActor -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API XiModelActor {
public:
    static undefined4 __thiscall CopyRectFromSlot(void * self, undefined4 param_1, undefined4 * param_2);  // 0x100C4B00
    static undefined4 __thiscall BuildWorldMatrix(void * self, undefined4 param_1, void * param_2);  // 0x100C4B50

    void **vftable;  // +0x0
    uint8_t _pad_4[0x790];
};
#pragma pack(pop)
static_assert(sizeof(XiModelActor) == 0x794, "XiModelActor: sizeof");
