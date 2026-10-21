// XiZone -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API XiZone {
public:
    static int __cdecl FindSubArea(int param_1);  // 0x101889C0
    static void __thiscall GetDirLights(void * self, undefined4 * param_1, undefined4 * param_2);  // 0x10188B70
    static void __cdecl GetInstance();  // 0x1018B130
    static void * __thiscall Constructor(void * self);  // 0x1018B170
    static void __thiscall DrawScene(void * self);  // 0x1018B860
    static void __thiscall GetDirLight(void * self, undefined4 param_1, undefined4 param_2, int param_3);  // 0x1018B9A0
    static void __thiscall GetAmbient(void * self, uint32_t * param_1, int param_2, int param_3);  // 0x1018B9E0
    static void __thiscall GetFog(void * self, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, int param_5);  // 0x1018BA20

    void **vftable;  // +0x0
    uint8_t _pad_4[0x1D8];
};
#pragma pack(pop)
static_assert(sizeof(XiZone) == 0x1DC, "XiZone: sizeof");
