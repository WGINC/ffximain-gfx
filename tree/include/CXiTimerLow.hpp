// CXiTimerLow -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CXiTimerLow {
public:
    static void * __thiscall Init(void * self);  // 0x10019490
    static undefined4 __thiscall AccumulateElapsed(void * self);  // 0x100194F0
    static long double __thiscall GetFrameRate(void * self);  // 0x10019550
    static long double __thiscall GetElapsed(void * self);  // 0x10019590

    void **vftable;  // +0x0
    uint8_t _pad_4[0x1C];
};
#pragma pack(pop)
static_assert(sizeof(CXiTimerLow) == 0x20, "CXiTimerLow: sizeof");
