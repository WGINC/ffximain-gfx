// CApp -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CApp {
public:
    char fn_10001520();  // 0x10001520

    void **vftable;  // +0x0
    uint8_t _pad_4[0x19];
    uint8_t field_1D;  // +0x1D
    uint8_t _pad_1E[0x26];
};
#pragma pack(pop)
static_assert(sizeof(CApp) == 0x44, "CApp: sizeof");
static_assert(XI_OFFSETOF(CApp, field_1D) == 0x1D, "CApp: layout");
