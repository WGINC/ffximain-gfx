// XiActorTimerTask -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API XiActorTimerTask {
public:
    char vf8();  // 0x10084F40

    void **vftable;  // +0x0
    uint8_t _pad_4[0x70];
    float field_74;  // +0x74
    uint8_t _pad_78[0x4];
};
#pragma pack(pop)
static_assert(sizeof(XiActorTimerTask) == 0x7C, "XiActorTimerTask: sizeof");
static_assert(XI_OFFSETOF(XiActorTimerTask, field_74) == 0x74, "XiActorTimerTask: layout");
