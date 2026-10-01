// CMoProcessor_VT_1032C298 -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoProcessor_VT_1032C298 {
public:
    float * vf007(float *a1, float a2);  // 0x1006D380
    double vf014(float *a1, float *a2);  // 0x1006F5B0
    float * vf021(float *a1, float *a2, float *a3, float a4);  // 0x1006FA30
    int vf035(float *a2, float *a3, int a4, int a5, int a6, float a7, float a8);  // 0x10070020
    CMoProcessor * vf036(float *a1, float *a2, int a3, int a4, const void *a5);  // 0x10070040

    void **vftable;  // +0x0
    uint8_t _pad_4[0xC];
};
#pragma pack(pop)
static_assert(sizeof(CMoProcessor_VT_1032C298) == 0x10, "CMoProcessor_VT_1032C298: sizeof");
