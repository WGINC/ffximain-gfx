// Obj_104E1D4C -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API Obj_104E1D4C {
public:
    void Cursor_SetPosition(_DWORD *a2, float a3);  // 0x10124700
    void Cursor_Draw();  // 0x10124EB0

    void **vftable;  // +0x0
    uint32_t field_4;  // +0x4
    uint32_t field_8;  // +0x8
    uint32_t field_C;  // +0xC
    uint32_t field_10;  // +0x10
    uint8_t _pad_14[0x18];
    uint32_t field_2C;  // +0x2C
    uint32_t field_30;  // +0x30
    uint8_t _pad_34[0x10];
    int16_t field_44;  // +0x44
    uint8_t _pad_46[0x2];
    float field_48;  // +0x48
    uint8_t field_4C;  // +0x4C
    uint8_t field_4D;  // +0x4D
    uint8_t field_4E;  // +0x4E
};
#pragma pack(pop)
static_assert(XI_OFFSETOF(Obj_104E1D4C, field_4) == 0x4, "Obj_104E1D4C: layout");
static_assert(XI_OFFSETOF(Obj_104E1D4C, field_8) == 0x8, "Obj_104E1D4C: layout");
static_assert(XI_OFFSETOF(Obj_104E1D4C, field_C) == 0xC, "Obj_104E1D4C: layout");
static_assert(XI_OFFSETOF(Obj_104E1D4C, field_10) == 0x10, "Obj_104E1D4C: layout");
static_assert(XI_OFFSETOF(Obj_104E1D4C, field_2C) == 0x2C, "Obj_104E1D4C: layout");
static_assert(XI_OFFSETOF(Obj_104E1D4C, field_30) == 0x30, "Obj_104E1D4C: layout");
static_assert(XI_OFFSETOF(Obj_104E1D4C, field_44) == 0x44, "Obj_104E1D4C: layout");
static_assert(XI_OFFSETOF(Obj_104E1D4C, field_48) == 0x48, "Obj_104E1D4C: layout");
static_assert(XI_OFFSETOF(Obj_104E1D4C, field_4C) == 0x4C, "Obj_104E1D4C: layout");
static_assert(XI_OFFSETOF(Obj_104E1D4C, field_4D) == 0x4D, "Obj_104E1D4C: layout");
static_assert(XI_OFFSETOF(Obj_104E1D4C, field_4E) == 0x4E, "Obj_104E1D4C: layout");
