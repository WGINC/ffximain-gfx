// CTsZoneMap -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CTsZoneMap {
public:
    char fn_10178070();  // 0x10178070
    float * fn_10182700(float *a2, int a3, IDirect3DBaseTexture8 *a4, float a5);  // 0x10182700
    void ModelActor_Draw(int a2, int a3);  // 0x101844C0
    int fn_10184FE0(int a1, int a2);  // 0x10184FE0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x178];
    float field_17C;  // +0x17C
    uint8_t _pad_180[0x34264];
    uint32_t field_343E4;  // +0x343E4
    uint8_t _pad_343E8[0x4D14];
    int32_t field_390FC;  // +0x390FC
    uint8_t _pad_39100[0x20];
    int32_t field_39120;  // +0x39120
    uint8_t _pad_39124[0x330];
    uint32_t field_39454;  // +0x39454
    uint32_t field_39458;  // +0x39458
    uint8_t _pad_3945C[0x10];
    uint32_t field_3946C;  // +0x3946C
    uint32_t field_39470;  // +0x39470
    uint32_t field_39474;  // +0x39474
    uint32_t field_39478;  // +0x39478
    uint32_t field_3947C;  // +0x3947C
    uint8_t _pad_39480[0x14];
    uint32_t field_39494;  // +0x39494
    uint8_t _pad_39498[0xC];
    int32_t field_394A4;  // +0x394A4
    int32_t field_394A8;  // +0x394A8
    int32_t field_394AC;  // +0x394AC
    int32_t field_394B0;  // +0x394B0
    uint8_t _pad_394B4[0xE4];
    uint8_t field_39598;  // +0x39598
    uint8_t _pad_39599[0x1B];
};
#pragma pack(pop)
static_assert(sizeof(CTsZoneMap) == 0x395B4, "CTsZoneMap: sizeof");
static_assert(XI_OFFSETOF(CTsZoneMap, field_17C) == 0x17C, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_343E4) == 0x343E4, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_390FC) == 0x390FC, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_39120) == 0x39120, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_39454) == 0x39454, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_39458) == 0x39458, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_3946C) == 0x3946C, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_39470) == 0x39470, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_39474) == 0x39474, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_39478) == 0x39478, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_3947C) == 0x3947C, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_39494) == 0x39494, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_394A4) == 0x394A4, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_394A8) == 0x394A8, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_394AC) == 0x394AC, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_394B0) == 0x394B0, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_39598) == 0x39598, "CTsZoneMap: layout");
