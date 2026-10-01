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
    static char ** __cdecl CMoProcessor_GetRuntimeClass();  // 0x1006CCD0
    float * vf012(float *a1, float *a2, float a3);  // 0x10070780
    double vf014(float *a1, float *a2);  // 0x10070790
    void vf041(float *a1);  // 0x10070860
    float * vf018(float *a1, float *a2, float *a3);  // 0x10070AA0
    float * vf017(float *a1, float *a2, float *a3);  // 0x10070AB0
    float * vf016(float *a1, float *a2, float *a3);  // 0x10070AC0
    float * vf021(float *a1, float *a2, float *a3, float a4);  // 0x10070AD0
    void vf032(float *a1, int a2, int a3, float *a4, float *a5, float a6, float a7);  // 0x10070D80
    float * vf033(float *a1, float *a2, int a3, float *a4, float *a5, float a6, float a7);  // 0x10070E20
    int vf035(float *a2, float *a3, int a4, int a5, int a6, float a7, float a8);  // 0x10070E40
    CMoProcessor * vf036(float *a1, float *a2, int a3, int a4, const void *a5);  // 0x10070E60

    void **vftable;  // +0x0
    uint8_t _pad_4[0xC];
};
#pragma pack(pop)
static_assert(sizeof(CMoProcessor_VT_1032C1C8) == 0x10, "CMoProcessor_VT_1032C1C8: sizeof");
