// CMoD3mSpecularElem -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoD3mSpecularElem {
public:
    _DWORD * Ctor();  // 0x10041020
    _DWORD * ScalarDeletingDtor(char a2);  // 0x100410F0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x172];
    uint16_t field_176;  // +0x176
    uint8_t _pad_178[0x20];
    uint32_t field_198;  // +0x198
    uint8_t _pad_19C[0xC];
    uint32_t field_1A8;  // +0x1A8
    uint8_t _pad_1AC[0xC];
    uint32_t field_1B8;  // +0x1B8
    uint8_t field_1BC;  // +0x1BC
    uint8_t field_1BD;  // +0x1BD
    uint8_t _pad_1BE[0x2];
};
#pragma pack(pop)
static_assert(sizeof(CMoD3mSpecularElem) == 0x1C0, "CMoD3mSpecularElem: sizeof");
static_assert(XI_OFFSETOF(CMoD3mSpecularElem, field_176) == 0x176, "CMoD3mSpecularElem: layout");
static_assert(XI_OFFSETOF(CMoD3mSpecularElem, field_198) == 0x198, "CMoD3mSpecularElem: layout");
static_assert(XI_OFFSETOF(CMoD3mSpecularElem, field_1A8) == 0x1A8, "CMoD3mSpecularElem: layout");
static_assert(XI_OFFSETOF(CMoD3mSpecularElem, field_1B8) == 0x1B8, "CMoD3mSpecularElem: layout");
static_assert(XI_OFFSETOF(CMoD3mSpecularElem, field_1BC) == 0x1BC, "CMoD3mSpecularElem: layout");
static_assert(XI_OFFSETOF(CMoD3mSpecularElem, field_1BD) == 0x1BD, "CMoD3mSpecularElem: layout");
