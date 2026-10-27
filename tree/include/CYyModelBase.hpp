// CYyModelBase -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CYyModelBase {
public:
    static undefined4 __thiscall AllocParts(void * self, int param_1);  // 0x100250F0
    static char ** __cdecl GetRuntimeClass();  // 0x100298A0
    static void * __thiscall Construct(void * self);  // 0x10029D30
    int * ScalarDeletingDtor(char a1);  // 0x10029DB0
    static void __thiscall Destructor(void * self);  // 0x10029DD0
    static void __thiscall SetResourceAndRebuild(void * self, undefined4 param_1);  // 0x10029E50
    static int __thiscall GetPartList(void * self);  // 0x10029E70
    static undefined4 * __thiscall AddPartFromResource(void * self, undefined4 * param_1);  // 0x10029F70
    static void __thiscall RefreshParts(void * self);  // 0x1002A0C0
    int DrawLit(int a2, int a3, int a4);  // 0x1002A3B0
    static undefined4 __thiscall CallPartsSlot20(void * self, undefined4 param_1, undefined4 param_2);  // 0x1002A700
    static undefined4 __thiscall TransformVec4ByPart(void * self, undefined4 param_1, undefined4 * param_2);  // 0x1002A750
    static int __thiscall FindPartIndex(void * self, uint32_t param_1);  // 0x1002A9B0
    static void __thiscall AllocMatrixBuffers(void * self);  // 0x100342A0

    void **vftable;  // +0x0
    uint8_t _pad_4[0xC0];
    uint32_t field_C4;  // +0xC4
};
#pragma pack(pop)
static_assert(sizeof(CYyModelBase) == 0xC8, "CYyModelBase: sizeof");
static_assert(XI_OFFSETOF(CYyModelBase, field_C4) == 0xC4, "CYyModelBase: layout");
