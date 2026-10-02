// CMoOtTask -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoOtTask {
public:
    void vf011();  // 0x10042650
    static char ** __cdecl GetRuntimeClass_variant0();  // 0x10044870
    static char ** __cdecl GetRuntimeClass_variant1();  // 0x10055560

    void **vftable;  // +0x0
    uint8_t _pad_4[0x50];
};
#pragma pack(pop)
static_assert(sizeof(CMoOtTask) == 0x54, "CMoOtTask: sizeof");
