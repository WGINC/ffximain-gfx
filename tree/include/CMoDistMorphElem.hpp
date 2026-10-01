// CMoDistMorphElem -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoDistMorphElem {
public:
    _DWORD * ComputeMorphHandle(int a2, int a3);  // 0x1003F8A0
    char UpdateMorphBlend();  // 0x10043590
    int set_field_1C0(int a2);  // 0x10052E50
    int * ScalarDeletingDtor(char a2);  // 0x10052E60

    void **vftable;  // +0x0
    uint8_t _pad_4[0x2E];
    uint8_t field_32;  // +0x32
    uint8_t _pad_33[0x1];
    int16_t field_34;  // +0x34
    int16_t field_36;  // +0x36
    uint8_t _pad_38[0xCC];
    int32_t field_104;  // +0x104
    uint8_t _pad_108[0xB4];
    int32_t field_1BC;  // +0x1BC
    uint32_t field_1C0;  // +0x1C0
};
#pragma pack(pop)
static_assert(sizeof(CMoDistMorphElem) == 0x1C4, "CMoDistMorphElem: sizeof");
static_assert(XI_OFFSETOF(CMoDistMorphElem, field_32) == 0x32, "CMoDistMorphElem: layout");
static_assert(XI_OFFSETOF(CMoDistMorphElem, field_34) == 0x34, "CMoDistMorphElem: layout");
static_assert(XI_OFFSETOF(CMoDistMorphElem, field_36) == 0x36, "CMoDistMorphElem: layout");
static_assert(XI_OFFSETOF(CMoDistMorphElem, field_104) == 0x104, "CMoDistMorphElem: layout");
static_assert(XI_OFFSETOF(CMoDistMorphElem, field_1BC) == 0x1BC, "CMoDistMorphElem: layout");
static_assert(XI_OFFSETOF(CMoDistMorphElem, field_1C0) == 0x1C0, "CMoDistMorphElem: layout");
