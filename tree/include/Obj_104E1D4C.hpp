// Obj_104E1D4C -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API Obj_104E1D4C {
public:
    void Cursor_SetPosition(_DWORD *a2, float a3);  // 0x10124700

    void **vftable;  // +0x0
    uint32_t posX;  // +0x4 Cursor_SetPosition stores a2[0]
    uint32_t posY;  // +0x8 Cursor_SetPosition stores a2[1]
    uint8_t _pad_C[0x3C];
    float field_48;  // +0x48
    uint8_t _pad_4C[0x1];
    uint8_t field_4D;  // +0x4D
};
#pragma pack(pop)
static_assert(XI_OFFSETOF(Obj_104E1D4C, posX) == 0x4, "Obj_104E1D4C: layout");
static_assert(XI_OFFSETOF(Obj_104E1D4C, posY) == 0x8, "Obj_104E1D4C: layout");
static_assert(XI_OFFSETOF(Obj_104E1D4C, field_48) == 0x48, "Obj_104E1D4C: layout");
static_assert(XI_OFFSETOF(Obj_104E1D4C, field_4D) == 0x4D, "Obj_104E1D4C: layout");
