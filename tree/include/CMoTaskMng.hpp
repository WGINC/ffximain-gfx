// CMoTaskMng -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoTaskMng {
public:
    _DWORD * ScalarDeletingDtor(char a2);  // 0x10011950
    static undefined4 __stdcall Forward10075120(undefined4 xi_s1);  // 0x10074A10
    static char ** __cdecl GetRuntimeClass();  // 0x10074B50
    static void __thiscall InitPool(void * self);  // 0x10074BA0
    static undefined4 __thiscall DrawAll(void * self);  // 0x10074C30
    static void __thiscall RemoveByKey(void * self, int param_1, int param_2);  // 0x10074CF0
    static int __thiscall AllocTask(void * self, undefined4 param_1);  // 0x10074DF0
    static undefined8 __thiscall InsertIntoBucket(void * self, int param_1, int param_2);  // 0x10074F80
    static undefined8 __thiscall EnqueueTask(void * self, int param_1);  // 0x100750B0
    static int __thiscall InsertAfterCursor(void * self, int param_1);  // 0x10075120
    static void __thiscall UnlinkTask(void * self, int param_1);  // 0x100751B0

    void **vftable;  // +0x0
    uint8_t _pad_4[0xD60];
};
#pragma pack(pop)
static_assert(sizeof(CMoTaskMng) == 0xD64, "CMoTaskMng: sizeof");
