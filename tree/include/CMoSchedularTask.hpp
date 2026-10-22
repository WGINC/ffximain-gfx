// CMoSchedularTask -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoSchedularTask {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x10057090
    void * ScalarDeletingDtor(char a2);  // 0x10057250
    static void __thiscall Destructor(void * self);  // 0x100574A0
    static undefined4 __thiscall vf8(void * self);  // 0x10057E10

    void **vftable;  // +0x0
    uint8_t _pad_4[0x146];
};
#pragma pack(pop)
static_assert(sizeof(CMoSchedularTask) == 0x14A, "CMoSchedularTask: sizeof");
