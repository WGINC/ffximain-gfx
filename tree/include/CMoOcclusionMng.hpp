// CMoOcclusionMng -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoOcclusionMng {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x1006C130
    int * ScalarDeletingDtor(char a2);  // 0x1006C1C0
    _DWORD * OcclusionMng_CreateTarget();  // 0x1006C220

    void **vftable;  // +0x0
    uint8_t _pad_4[0x8];
    uint32_t field_C;  // +0xC
    uint32_t field_10;  // +0x10
    uint8_t _pad_14[0x8];
};
#pragma pack(pop)
static_assert(sizeof(CMoOcclusionMng) == 0x1C, "CMoOcclusionMng: sizeof");
static_assert(XI_OFFSETOF(CMoOcclusionMng, field_C) == 0xC, "CMoOcclusionMng: layout");
static_assert(XI_OFFSETOF(CMoOcclusionMng, field_10) == 0x10, "CMoOcclusionMng: layout");
