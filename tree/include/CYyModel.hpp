// CYyModel -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CYyModel {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x100298C0
    static void __thiscall Constructor(void * self);  // 0x1002B4B0
    int * ScalarDeletingDtor(char a2);  // 0x1002B530
    static void __thiscall Destructor(void * self);  // 0x1002B550
    static int __thiscall GetField44Addr(void * self);  // 0x1002B5A0
    static int __thiscall TailLink(void * self);  // 0x1002B5E0
    static void __thiscall AppendBase(void * self, undefined4 param_1);  // 0x1002B630
    static undefined4 __thiscall CreateModelBase(void * self);  // 0x1002B6A0
    static void __thiscall ApplyToParts(void * self, undefined4 xi_s1);  // 0x1002C270
    static void __thiscall ForEachPart130(void * self, undefined4 param_1);  // 0x1002C2A0
    static undefined4 __thiscall GetPartOffset(void * self, int param_1, int param_2, float * param_3);  // 0x1002C400
    static undefined4 __thiscall CallPartIndex(void * self, undefined4 param_1, int param_2);  // 0x1002C600
    static undefined4 __thiscall QueryPartsA(void * self, undefined4 xi_s1);  // 0x1002C710
    static uint32_t __thiscall QueryPartsB(void * self, undefined4 xi_s1);  // 0x1002C750

    void **vftable;  // +0x0
    uint8_t _pad_4[0xB8];
};
#pragma pack(pop)
static_assert(sizeof(CYyModel) == 0xBC, "CYyModel: sizeof");
