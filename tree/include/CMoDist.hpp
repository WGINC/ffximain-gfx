// CMoDist -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoDist {
public:
    static char ** __cdecl GetRuntimeClass_variant0();  // 0x100426E0
    static char ** __cdecl GetRuntimeClass_variant1();  // 0x100430B0
    BOOL vf012(int a1);  // 0x10052D60
    int set_field_194(int a2);  // 0x10052D70
    int * ScalarDeletingDtor(char a2);  // 0x10052D80

    void **vftable;  // +0x0
    uint8_t _pad_4[0x190];
    uint32_t field_194;  // +0x194
};
#pragma pack(pop)
static_assert(sizeof(CMoDist) == 0x198, "CMoDist: sizeof");
static_assert(XI_OFFSETOF(CMoDist, field_194) == 0x194, "CMoDist: layout");
