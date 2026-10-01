// CMoProcessor_VT_1032C1C8 -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoProcessor_VT_1032C1C8 {
public:
    int vf008();  // 0x1006CC40
    int vf010();  // 0x1006CC50
    int vf040();  // 0x1006CC60
    int vf038();  // 0x1006CC70
    int vf046();  // 0x1006CC80
    int vf045();  // 0x1006CC90
    int vf035(float *a2, float *a3, int a4, int a5, int a6, float a7, float a8);  // 0x10070E40

    void **vftable;  // +0x0
    uint8_t _pad_4[0xC];
};
#pragma pack(pop)
static_assert(sizeof(CMoProcessor_VT_1032C1C8) == 0x10, "CMoProcessor_VT_1032C1C8: sizeof");
