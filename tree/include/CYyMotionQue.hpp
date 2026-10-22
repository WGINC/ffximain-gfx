// CYyMotionQue -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CYyMotionQue {
public:
    static void __thiscall SetSlotMotion(void * self, int * param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7, int param_8, undefined4 param_9, undefined4 param_10);  // 0x1001A660
    static void __thiscall StartMotion(void * self, int param_1, undefined4 param_2, undefined4 param_3, int param_4);  // 0x1001AB60
    static void __thiscall AppendSync(void * self, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7, undefined4 param_8);  // 0x1001ACB0
    static undefined4 __thiscall SampleInto(void * self, int param_1, undefined4 param_2);  // 0x1001B230
    static void __thiscall Update(void * self, int * param_1, int param_2);  // 0x1001B2E0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x3C];
};
#pragma pack(pop)
static_assert(sizeof(CYyMotionQue) == 0x40, "CYyMotionQue: sizeof");
