// CMoD3mSpecularElem -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoD3mSpecularElem {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x10041010
    static undefined8 __thiscall Ctor(void * self);  // 0x10041020
    BOOL vf012(int a1);  // 0x100410E0
    _DWORD * ScalarDeletingDtor(char a2);  // 0x100410F0
    void vf18();  // 0x100412B0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x100];
    uint32_t field_104;  // +0x104
    uint8_t _pad_108[0x4];
    uint32_t field_10C;  // +0x10C
    uint8_t _pad_110[0x10];
    float field_120;  // +0x120
    float field_124;  // +0x124
    uint8_t _pad_128[0x18];
    float field_140;  // +0x140
    float field_144;  // +0x144
    float field_148;  // +0x148
    uint8_t _pad_14C[0x20];
    uint8_t field_16C;  // +0x16C
    uint8_t _pad_16D[0xC];
    uint8_t field_179;  // +0x179
    uint8_t _pad_17A[0x1];
    uint8_t field_17B;  // +0x17B
    uint8_t _pad_17C[0x1C];
    uint8_t field_198;  // +0x198
    uint8_t _pad_199[0x3];
    float field_19C;  // +0x19C
    float field_1A0;  // +0x1A0
    float field_1A4;  // +0x1A4
    uint8_t _pad_1A8[0x10];
    uint8_t field_1B8;  // +0x1B8
    uint8_t _pad_1B9[0x3];
    int8_t field_1BC;  // +0x1BC
    int8_t field_1BD;  // +0x1BD
    uint8_t _pad_1BE[0x2];
};
#pragma pack(pop)
static_assert(sizeof(CMoD3mSpecularElem) == 0x1C0, "CMoD3mSpecularElem: sizeof");
static_assert(XI_OFFSETOF(CMoD3mSpecularElem, field_104) == 0x104, "CMoD3mSpecularElem: layout");
static_assert(XI_OFFSETOF(CMoD3mSpecularElem, field_10C) == 0x10C, "CMoD3mSpecularElem: layout");
static_assert(XI_OFFSETOF(CMoD3mSpecularElem, field_120) == 0x120, "CMoD3mSpecularElem: layout");
static_assert(XI_OFFSETOF(CMoD3mSpecularElem, field_124) == 0x124, "CMoD3mSpecularElem: layout");
static_assert(XI_OFFSETOF(CMoD3mSpecularElem, field_140) == 0x140, "CMoD3mSpecularElem: layout");
static_assert(XI_OFFSETOF(CMoD3mSpecularElem, field_144) == 0x144, "CMoD3mSpecularElem: layout");
static_assert(XI_OFFSETOF(CMoD3mSpecularElem, field_148) == 0x148, "CMoD3mSpecularElem: layout");
static_assert(XI_OFFSETOF(CMoD3mSpecularElem, field_16C) == 0x16C, "CMoD3mSpecularElem: layout");
static_assert(XI_OFFSETOF(CMoD3mSpecularElem, field_179) == 0x179, "CMoD3mSpecularElem: layout");
static_assert(XI_OFFSETOF(CMoD3mSpecularElem, field_17B) == 0x17B, "CMoD3mSpecularElem: layout");
static_assert(XI_OFFSETOF(CMoD3mSpecularElem, field_198) == 0x198, "CMoD3mSpecularElem: layout");
static_assert(XI_OFFSETOF(CMoD3mSpecularElem, field_19C) == 0x19C, "CMoD3mSpecularElem: layout");
static_assert(XI_OFFSETOF(CMoD3mSpecularElem, field_1A0) == 0x1A0, "CMoD3mSpecularElem: layout");
static_assert(XI_OFFSETOF(CMoD3mSpecularElem, field_1A4) == 0x1A4, "CMoD3mSpecularElem: layout");
static_assert(XI_OFFSETOF(CMoD3mSpecularElem, field_1B8) == 0x1B8, "CMoD3mSpecularElem: layout");
static_assert(XI_OFFSETOF(CMoD3mSpecularElem, field_1BC) == 0x1BC, "CMoD3mSpecularElem: layout");
static_assert(XI_OFFSETOF(CMoD3mSpecularElem, field_1BD) == 0x1BD, "CMoD3mSpecularElem: layout");
