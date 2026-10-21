// CMoSchedularTask -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoSchedularTask {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x10057090
    void * ScalarDeletingDtor(char a2);  // 0x10057250
    static void __thiscall Destructor(void * self);  // 0x100574A0
    static undefined4 __thiscall vf8(void * self);  // 0x10057E10
    int Interpret();  // 0x10057FB0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x7C];
    _DWORD ** field_80;  // +0x80
    uint8_t _pad_84[0x4];
    uint32_t field_88;  // +0x88
    uint8_t _pad_8C[0x4];
    _DWORD * field_90;  // +0x90
    uint8_t _pad_94[0x4];
    float field_98;  // +0x98
    float field_9C;  // +0x9C
    uint8_t _pad_A0[0xA0];
    uint8_t field_140;  // +0x140
    uint8_t _pad_141[0x9];
};
#pragma pack(pop)
static_assert(sizeof(CMoSchedularTask) == 0x14A, "CMoSchedularTask: sizeof");
static_assert(XI_OFFSETOF(CMoSchedularTask, field_80) == 0x80, "CMoSchedularTask: layout");
static_assert(XI_OFFSETOF(CMoSchedularTask, field_88) == 0x88, "CMoSchedularTask: layout");
static_assert(XI_OFFSETOF(CMoSchedularTask, field_90) == 0x90, "CMoSchedularTask: layout");
static_assert(XI_OFFSETOF(CMoSchedularTask, field_98) == 0x98, "CMoSchedularTask: layout");
static_assert(XI_OFFSETOF(CMoSchedularTask, field_9C) == 0x9C, "CMoSchedularTask: layout");
static_assert(XI_OFFSETOF(CMoSchedularTask, field_140) == 0x140, "CMoSchedularTask: layout");
