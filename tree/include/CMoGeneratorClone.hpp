// CMoGeneratorClone -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoGeneratorClone {
public:
    _DWORD * FixupClonedPointers();  // 0x10047CE0
    int CreateInstance(_DWORD *a2, _DWORD *a3, int a4);  // 0x100536C0
    int InitDefaultParams();  // 0x10053E00

    void **vftable;  // +0x0
    uint8_t _pad_4[0x10];
    uint32_t field_14;  // +0x14
    uint8_t _pad_18[0x8];
    uint32_t field_20;  // +0x20
    uint8_t _pad_24[0x54];
    uint32_t field_78;  // +0x78
    uint32_t field_7C;  // +0x7C
    uint32_t field_80;  // +0x80
    uint32_t field_84;  // +0x84
    uint32_t field_88;  // +0x88
    uint32_t field_8C;  // +0x8C
    uint8_t _pad_90[0x8];
    uint32_t field_98;  // +0x98
    uint8_t _pad_9C[0x8];
    uint32_t field_A4;  // +0xA4
    uint8_t _pad_A8[0x8];
    uint32_t field_B0;  // +0xB0
    uint8_t _pad_B4[0x24];
    uint32_t field_D8;  // +0xD8
    uint16_t field_DC;  // +0xDC
    uint8_t field_DE;  // +0xDE
    uint8_t field_DF;  // +0xDF
    uint32_t field_E0;  // +0xE0
    uint32_t field_E4;  // +0xE4
    uint32_t field_E8;  // +0xE8
    uint32_t field_EC;  // +0xEC
    uint8_t _pad_F0[0xC];
};
#pragma pack(pop)
static_assert(sizeof(CMoGeneratorClone) == 0xFC, "CMoGeneratorClone: sizeof");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_14) == 0x14, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_20) == 0x20, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_78) == 0x78, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_7C) == 0x7C, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_80) == 0x80, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_84) == 0x84, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_88) == 0x88, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_8C) == 0x8C, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_98) == 0x98, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_A4) == 0xA4, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_B0) == 0xB0, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_D8) == 0xD8, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_DC) == 0xDC, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_DE) == 0xDE, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_DF) == 0xDF, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_E0) == 0xE0, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_E4) == 0xE4, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_E8) == 0xE8, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_EC) == 0xEC, "CMoGeneratorClone: layout");
