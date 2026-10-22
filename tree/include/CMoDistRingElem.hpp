// CMoDistRingElem -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoDistRingElem {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x10043710
    void AllocRingGeometry();  // 0x10043720
    static undefined8 __thiscall Construct(void * self);  // 0x10052FD0
    BOOL vf012(int a1);  // 0x10052FF0
    int set_field_1B6(int a2);  // 0x10053000
    int * ScalarDeletingDtor(char a2);  // 0x10053010

    void **vftable;  // +0x0
    uint8_t _pad_4[0x9C];
    uint8_t field_A0;  // +0xA0
    uint8_t _pad_A1[0xF7];
    uint8_t field_198;  // +0x198
    uint8_t _pad_199[0xC];
    uint8_t field_1A5;  // +0x1A5
    uint8_t _pad_1A6[0xE];
    uint8_t field_1B4;  // +0x1B4
    uint8_t field_1B5;  // +0x1B5
    uint8_t _pad_1B6[0x4];
};
#pragma pack(pop)
static_assert(sizeof(CMoDistRingElem) == 0x1BA, "CMoDistRingElem: sizeof");
static_assert(XI_OFFSETOF(CMoDistRingElem, field_A0) == 0xA0, "CMoDistRingElem: layout");
static_assert(XI_OFFSETOF(CMoDistRingElem, field_198) == 0x198, "CMoDistRingElem: layout");
static_assert(XI_OFFSETOF(CMoDistRingElem, field_1A5) == 0x1A5, "CMoDistRingElem: layout");
static_assert(XI_OFFSETOF(CMoDistRingElem, field_1B4) == 0x1B4, "CMoDistRingElem: layout");
static_assert(XI_OFFSETOF(CMoDistRingElem, field_1B5) == 0x1B5, "CMoDistRingElem: layout");
