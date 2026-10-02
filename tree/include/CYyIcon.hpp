// CYyIcon -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CYyIcon {
public:
    int Draw(int a1, int a2, int a3, int a4);  // 0x10026990
    int DrawSubRect(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);  // 0x10026C70

    void **vftable;  // +0x0
    uint8_t _pad_4[0x4];
    uint32_t pIconTexture;  // +0x8 read and passed on as the texture by CYyIcon::Draw and DrawSubRect
    float quad0_x;  // +0xC vertex 0 (28-byte XYZRHW|DIFFUSE|TEX1 vertices from +0xC) component x: written per corner 
    float quad0_y;  // +0x10 vertex 0 (28-byte XYZRHW|DIFFUSE|TEX1 vertices from +0xC) component y: written per corner 
    uint8_t _pad_14[0x8];
    uint32_t quad0_color;  // +0x1C vertex 0 (28-byte XYZRHW|DIFFUSE|TEX1 vertices from +0xC) component color: written per cor
    float quad0_u;  // +0x20 vertex 0 (28-byte XYZRHW|DIFFUSE|TEX1 vertices from +0xC) component u: written per corner 
    float quad0_v;  // +0x24 vertex 0 (28-byte XYZRHW|DIFFUSE|TEX1 vertices from +0xC) component v: written per corner 
    float quad1_x;  // +0x28 vertex 1 (28-byte XYZRHW|DIFFUSE|TEX1 vertices from +0xC) component x: written per corner 
    float quad1_y;  // +0x2C vertex 1 (28-byte XYZRHW|DIFFUSE|TEX1 vertices from +0xC) component y: written per corner 
    uint8_t _pad_30[0x8];
    uint32_t quad1_color;  // +0x38 vertex 1 (28-byte XYZRHW|DIFFUSE|TEX1 vertices from +0xC) component color: written per cor
    uint32_t quad1_u;  // +0x3C vertex 1 (28-byte XYZRHW|DIFFUSE|TEX1 vertices from +0xC) component u: written per corner 
    uint32_t quad1_v;  // +0x40 vertex 1 (28-byte XYZRHW|DIFFUSE|TEX1 vertices from +0xC) component v: written per corner 
    float quad2_x;  // +0x44 vertex 2 (28-byte XYZRHW|DIFFUSE|TEX1 vertices from +0xC) component x: written per corner 
    float quad2_y;  // +0x48 vertex 2 (28-byte XYZRHW|DIFFUSE|TEX1 vertices from +0xC) component y: written per corner 
    uint8_t _pad_4C[0x8];
    uint32_t quad2_color;  // +0x54 vertex 2 (28-byte XYZRHW|DIFFUSE|TEX1 vertices from +0xC) component color: written per cor
    uint32_t quad2_u;  // +0x58 vertex 2 (28-byte XYZRHW|DIFFUSE|TEX1 vertices from +0xC) component u: written per corner 
    uint32_t quad2_v;  // +0x5C vertex 2 (28-byte XYZRHW|DIFFUSE|TEX1 vertices from +0xC) component v: written per corner 
    float quad3_x;  // +0x60 vertex 3 (28-byte XYZRHW|DIFFUSE|TEX1 vertices from +0xC) component x: written per corner 
    float quad3_y;  // +0x64 vertex 3 (28-byte XYZRHW|DIFFUSE|TEX1 vertices from +0xC) component y: written per corner 
    uint8_t _pad_68[0x8];
    uint32_t quad3_color;  // +0x70 vertex 3 (28-byte XYZRHW|DIFFUSE|TEX1 vertices from +0xC) component color: written per cor
    float quad3_u;  // +0x74 vertex 3 (28-byte XYZRHW|DIFFUSE|TEX1 vertices from +0xC) component u: written per corner 
    float quad3_v;  // +0x78 vertex 3 (28-byte XYZRHW|DIFFUSE|TEX1 vertices from +0xC) component v: written per corner 
};
#pragma pack(pop)
static_assert(sizeof(CYyIcon) == 0x7C, "CYyIcon: sizeof");
static_assert(XI_OFFSETOF(CYyIcon, pIconTexture) == 0x8, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, quad0_x) == 0xC, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, quad0_y) == 0x10, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, quad0_color) == 0x1C, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, quad0_u) == 0x20, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, quad0_v) == 0x24, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, quad1_x) == 0x28, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, quad1_y) == 0x2C, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, quad1_color) == 0x38, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, quad1_u) == 0x3C, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, quad1_v) == 0x40, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, quad2_x) == 0x44, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, quad2_y) == 0x48, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, quad2_color) == 0x54, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, quad2_u) == 0x58, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, quad2_v) == 0x5C, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, quad3_x) == 0x60, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, quad3_y) == 0x64, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, quad3_color) == 0x70, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, quad3_u) == 0x74, "CYyIcon: layout");
static_assert(XI_OFFSETOF(CYyIcon, quad3_v) == 0x78, "CYyIcon: layout");
