// CMoResource -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoResource {
public:
    static char ** __cdecl GetRuntimeClass_variant0();  // 0x10036B70
    int CMoCamera_SetLinkedObject(int a2);  // 0x10047FC0
    void vf005(int a1);  // 0x10047FD0
    static char ** __cdecl GetRuntimeClass_variant1();  // 0x10056250
    static char ** __cdecl GetRuntimeClass_variant2();  // 0x10056290
    static char ** __cdecl GetRuntimeClass_variant3();  // 0x10071D70
    void vf008();  // 0x10071DB0
    unsigned int ** ScalarDeletingDtor(char a2);  // 0x10071DC0
    static void __thiscall Destructor(void * self);  // 0x10071DE0
    static undefined4 __thiscall ReleaseChain(void * self);  // 0x10071EC0
    static void __thiscall RegisterInTypeList(void * self);  // 0x10072030
    static void * __thiscall GetParent(void * self);  // 0x10072270
    static undefined8 __thiscall FindByTypeAndId(void * self, undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x10072290

    void **vftable;  // +0x0
    uint8_t _pad_4[0x10];
    _DWORD * field_14;  // +0x14
    uint8_t _pad_18[0x18];
};
#pragma pack(pop)
static_assert(sizeof(CMoResource) == 0x30, "CMoResource: sizeof");
static_assert(XI_OFFSETOF(CMoResource, field_14) == 0x14, "CMoResource: layout");
