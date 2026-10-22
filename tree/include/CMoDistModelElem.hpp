// CMoDistModelElem -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoDistModelElem {
public:
    int DrawMesh(int a1, int a2, int a3, int a4);  // 0x10040670
    static char ** __cdecl GetRuntimeClass();  // 0x100431A0
    BOOL vf012(int a1);  // 0x10052F40
    int set_field_194(int a2);  // 0x10052F50
    int * ScalarDeletingDtor(char a2);  // 0x10052F60

    void **vftable;  // +0x0
    uint8_t _pad_4[0x2E];
    uint8_t field_32;  // +0x32
    uint8_t _pad_33[0x1];
    uint8_t field_34;  // +0x34
    uint8_t field_35;  // +0x35
    uint16_t field_36;  // +0x36
    uint8_t _pad_38[0x160];
};
#pragma pack(pop)
static_assert(sizeof(CMoDistModelElem) == 0x198, "CMoDistModelElem: sizeof");
static_assert(XI_OFFSETOF(CMoDistModelElem, field_32) == 0x32, "CMoDistModelElem: layout");
static_assert(XI_OFFSETOF(CMoDistModelElem, field_34) == 0x34, "CMoDistModelElem: layout");
static_assert(XI_OFFSETOF(CMoDistModelElem, field_35) == 0x35, "CMoDistModelElem: layout");
static_assert(XI_OFFSETOF(CMoDistModelElem, field_36) == 0x36, "CMoDistModelElem: layout");
