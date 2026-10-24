// CXiMovie -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CXiMovie {
public:
    int Draw(int a1, int a2, int a3, int a4, int a5);  // 0x10026AD0
    static undefined4 __cdecl Create(undefined4 param_1);  // 0x1021D3D0
    static undefined4 __cdecl GetField4();  // 0x1021D490
    static long double __cdecl GetDoubleField8();  // 0x1021D4A0
    static int __cdecl DrawFrame(int rcLeft, int rcTop, int rcRight, int rcBottom, float * scale);  // 0x1021D4C0
    static void * __thiscall Construct(void * self);  // 0x1021D7F0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x4];
    uint32_t pMovieTexture;  // +0x8 read as the texture by CXiMovie::Draw (same quad layout as CYyIcon)
    float quad0_x;  // +0xC vertex 0 of the 28-byte XYZRHW|DIFFUSE|TEX1 quad from +0xC, written by CXiMovie::Draw (sam
    float quad0_y;  // +0x10 vertex 0 of the 28-byte XYZRHW|DIFFUSE|TEX1 quad from +0xC, written by CXiMovie::Draw (sam
    uint8_t _pad_14[0x8];
    uint32_t quad0_color;  // +0x1C vertex 0 of the 28-byte XYZRHW|DIFFUSE|TEX1 quad from +0xC, written by CXiMovie::Draw (sam
    uint8_t _pad_20[0x8];
};
#pragma pack(pop)
static_assert(sizeof(CXiMovie) == 0x28, "CXiMovie: sizeof");
static_assert(XI_OFFSETOF(CXiMovie, pMovieTexture) == 0x8, "CXiMovie: layout");
static_assert(XI_OFFSETOF(CXiMovie, quad0_x) == 0xC, "CXiMovie: layout");
static_assert(XI_OFFSETOF(CXiMovie, quad0_y) == 0x10, "CXiMovie: layout");
static_assert(XI_OFFSETOF(CXiMovie, quad0_color) == 0x1C, "CXiMovie: layout");
