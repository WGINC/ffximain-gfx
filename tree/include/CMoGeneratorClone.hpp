// CMoGeneratorClone -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoGeneratorClone {
public:
    int InitCloneFromSource(_DWORD *a2);  // 0x10047C30
    _DWORD * FixupClonedPointers();  // 0x10047CE0
    static void __thiscall UnlinkElemListA(void * self, int param_1);  // 0x10048140
    static uint32_t * __thiscall FindCmdByType(void * self, uint32_t param_1);  // 0x100492F0
    static undefined4 __thiscall ResolveTarget(void * self);  // 0x10049570
    static void __thiscall CopyParams(void * self, int param_1);  // 0x10053450
    int CreateInstance(_DWORD *a2, _DWORD *a3, int a4);  // 0x100536C0
    int InitDefaultParams();  // 0x10053E00
    static void __thiscall Destructor(void * self);  // 0x100542F0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x10];
    uint32_t field_14;  // +0x14
    uint8_t _pad_18[0x8];
    uint32_t typeTag;  // +0x20 compared with 0x31343870 (the four characters 'p841') in FixupClonedPointers: a four-chara
    uint32_t field_24;  // +0x24
    uint32_t field_28;  // +0x28
    uint32_t field_2C;  // +0x2C
    uint8_t _pad_30[0x4];
    uint32_t field_34;  // +0x34
    uint32_t field_38;  // +0x38
    uint32_t field_3C;  // +0x3C
    uint32_t field_40;  // +0x40
    uint8_t _pad_44[0x2C];
    uint32_t field_70;  // +0x70
    uint32_t field_74;  // +0x74
    uint32_t field_78;  // +0x78
    uint32_t field_7C;  // +0x7C
    uint32_t field_80;  // +0x80
    uint32_t field_84;  // +0x84
    uint32_t field_88;  // +0x88
    uint32_t field_8C;  // +0x8C
    uint8_t _pad_90[0x8];
    uint32_t field_98;  // +0x98
    uint32_t field_9C;  // +0x9C
    uint8_t _pad_A0[0x4];
    uint32_t field_A4;  // +0xA4
    uint8_t _pad_A8[0x8];
    uint32_t field_B0;  // +0xB0
    uint8_t _pad_B4[0x24];
    uint32_t flags;  // +0xD8 bit 0x1000 tested in FixupClonedPointers; the low 9 bits are passed on as a count in Creat
    uint16_t field_DC;  // +0xDC
    uint8_t field_DE;  // +0xDE
    uint8_t field_DF;  // +0xDF
    uint32_t pCommandStream;  // +0xE0 stored as an offset and rebased by this+0x60 in FixupClonedPointers; CreateInstance walks 
    uint32_t field_E4;  // +0xE4
    uint32_t field_E8;  // +0xE8
    uint32_t field_EC;  // +0xEC
    uint8_t _pad_F0[0xC];
};
#pragma pack(pop)
static_assert(sizeof(CMoGeneratorClone) == 0xFC, "CMoGeneratorClone: sizeof");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_14) == 0x14, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, typeTag) == 0x20, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_24) == 0x24, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_28) == 0x28, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_2C) == 0x2C, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_34) == 0x34, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_38) == 0x38, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_3C) == 0x3C, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_40) == 0x40, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_70) == 0x70, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_74) == 0x74, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_78) == 0x78, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_7C) == 0x7C, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_80) == 0x80, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_84) == 0x84, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_88) == 0x88, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_8C) == 0x8C, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_98) == 0x98, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_9C) == 0x9C, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_A4) == 0xA4, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_B0) == 0xB0, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, flags) == 0xD8, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_DC) == 0xDC, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_DE) == 0xDE, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_DF) == 0xDF, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, pCommandStream) == 0xE0, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_E4) == 0xE4, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_E8) == 0xE8, "CMoGeneratorClone: layout");
static_assert(XI_OFFSETOF(CMoGeneratorClone, field_EC) == 0xEC, "CMoGeneratorClone: layout");
