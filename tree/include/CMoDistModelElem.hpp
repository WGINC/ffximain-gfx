// CMoDistModelElem -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoDistModelElem {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x100431A0
    void BuildDistortedMesh();  // 0x10043320
    BOOL vf012(int a1);  // 0x10052F40
    int set_field_194(int a2);  // 0x10052F50
    int * ScalarDeletingDtor(char a2);  // 0x10052F60

    void **vftable;  // +0x0
    uint8_t _pad_4[0x175];
    uint8_t field_179;  // +0x179
    uint8_t _pad_17A[0x1A];
    float field_194;  // +0x194
};
#pragma pack(pop)
static_assert(sizeof(CMoDistModelElem) == 0x198, "CMoDistModelElem: sizeof");
static_assert(XI_OFFSETOF(CMoDistModelElem, field_179) == 0x179, "CMoDistModelElem: layout");
static_assert(XI_OFFSETOF(CMoDistModelElem, field_194) == 0x194, "CMoDistModelElem: layout");
