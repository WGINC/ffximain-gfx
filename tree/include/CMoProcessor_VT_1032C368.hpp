// CMoProcessor_VT_1032C368 -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoProcessor_VT_1032C368 {
public:
    float * vf012(float *a1, float *a2, float a3);  // 0x1006D3B0
    double vf014(float *a1, float *a2);  // 0x1006D4D0
    float * vf042(float *a1, float *a2, float *a3);  // 0x1006D680
    float * vf043(float *a1, float *a2, float *a3);  // 0x1006D6B0
    float * vf021(float *a1, float *a2, float *a3, float a4);  // 0x1006D980

    void **vftable;  // +0x0
    uint8_t _pad_4[0xC];
};
#pragma pack(pop)
static_assert(sizeof(CMoProcessor_VT_1032C368) == 0x10, "CMoProcessor_VT_1032C368: sizeof");
