// CYyVbMng -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CYyVbMng {
public:
    int AcquireVertexBuffer(int a2, unsigned int a3, unsigned int a4, D3DPOOL a5, int *a6, char a7);  // 0x1003B210

    void **vftable;  // +0x0
    uint32_t field_4;  // +0x4
    uint8_t _pad_8[0x4];
};
#pragma pack(pop)
static_assert(sizeof(CYyVbMng) == 0xC, "CYyVbMng: sizeof");
static_assert(XI_OFFSETOF(CYyVbMng, field_4) == 0x4, "CYyVbMng: layout");
