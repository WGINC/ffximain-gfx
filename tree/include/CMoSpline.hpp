// CMoSpline -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoSpline {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x10063450
    int * ScalarDeletingDtor(char a2);  // 0x10063480
    static void __thiscall Destructor(void * self);  // 0x100634A0
    static void __thiscall Init(void * self, int param_1, uint32_t param_2, undefined4 param_3, undefined4 param_4);  // 0x100634B0
    static void __thiscall FreeBuffers(void * self);  // 0x10063550
    static void __thiscall ReleaseRef(void * self);  // 0x10063B20
    static int * __thiscall GetWrapped(void * self, int param_1);  // 0x10063B50
    static int __thiscall GetData(void * self);  // 0x10063B70
    static undefined4 __thiscall EnsureSamples(void * self);  // 0x10063B80
    static void __thiscall SetValue(void * self, undefined4 param_1, undefined4 param_2);  // 0x10063C30

    void **vftable;  // +0x0
    uint8_t _pad_4[0x24];
};
#pragma pack(pop)
static_assert(sizeof(CMoSpline) == 0x28, "CMoSpline: sizeof");
