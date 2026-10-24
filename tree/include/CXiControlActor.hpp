// CXiControlActor -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CXiControlActor {
public:
    static unsigned char __thiscall AmIControlActor(void * self);  // 0x100A4640
    static void __thiscall FillHistory(void * self, undefined4 * param_1);  // 0x100A5770
    static void * __thiscall Constructor(void * self);  // 0x100A89E0
    static long double __stdcall GetSpeed(int * param_1);  // 0x100AAEF0
    static void __thiscall Slot137_Set388(void * self, undefined4 param_1);  // 0x100AAF50
    static void __thiscall Slot138_Scan3F0(void * self, undefined4 * param_1);  // 0x100AAF70

    void **vftable;  // +0x0
    uint8_t _pad_4[0x5C0];
};
#pragma pack(pop)
static_assert(sizeof(CXiControlActor) == 0x5C4, "CXiControlActor: sizeof");
