// CTsZoneMap -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CTsZoneMap {
public:
    char CTsZoneMap_Destructor();  // 0x10178070
    int InitZBiasPresets();  // 0x101781D0
    float * DrawProjectedTexture(float *arg0, int a3, IDirect3DBaseTexture8 *a4, float a5);  // 0x10182700
    void ModelActor_Draw(int a2, int a3);  // 0x101844C0
    int DrawProjectedShadow(int a1, int a2);  // 0x10184FE0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x174];
    uint8_t field_178;  // +0x178
    uint8_t _pad_179[0x3];
    float field_17C;  // +0x17C
    uint8_t _pad_180[0x14];
    uint8_t field_194;  // +0x194
    uint8_t _pad_195[0x3419B];
    uint8_t field_34330;  // +0x34330
    uint8_t _pad_34331[0xB3];
    uint32_t overlayTex;  // +0x343E4 CYyTex* of the multiplicative overlay quad
    uint8_t _pad_343E8[0x40];
    uint8_t field_34428;  // +0x34428
    uint8_t _pad_34429[0x4CD3];
    int32_t cullCW;  // +0x390FC one-sided batches: nonzero -> D3DCULL_CW, else CCW
    uint8_t _pad_39100[0x20];
    int32_t curTwoSided;  // +0x39120 cached two-sided state of the last batch
    uint8_t _pad_39124[0x330];
    uint32_t vsHandleA;  // +0x39454 vertex shader from g_zoneVSDeclA
    uint32_t vsHandleB;  // +0x39458 vertex shader from g_zoneVSDeclB
    uint8_t _pad_3945C[0x10];
    uint32_t vsHandles;  // +0x3946C 5 more vertex shader handles
    uint32_t field_39470;  // +0x39470
    uint32_t field_39474;  // +0x39474
    uint32_t field_39478;  // +0x39478
    uint32_t field_3947C;  // +0x3947C
    uint8_t _pad_39480[0x14];
    uint32_t psHandle;  // +0x39494 pixel shader handle
    uint8_t _pad_39498[0xC];
    int32_t useTwoDirLights;  // +0x394A4 nonzero: two zone-environment directional lights
    int32_t lightSlotDir0;  // +0x394A8 D3D light index, directional light 0
    int32_t lightSlotDir1;  // +0x394AC D3D light index, directional light 1
    int32_t lightSlotNamed;  // +0x394B0 D3D light index of zone-table light 'l_01'
    uint8_t _pad_394B4[0x98];
    uint32_t field_3954C;  // +0x3954C
    uint8_t _pad_39550[0x48];
    uint8_t overlayUseProjection;  // +0x39598 byte: overlay uses saved projection, else Z-bias
    uint8_t _pad_39599[0x3];
    uint8_t field_3959C;  // +0x3959C
    uint8_t _pad_3959D[0x17];
};
#pragma pack(pop)
static_assert(sizeof(CTsZoneMap) == 0x395B4, "CTsZoneMap: sizeof");
static_assert(XI_OFFSETOF(CTsZoneMap, field_178) == 0x178, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_17C) == 0x17C, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_194) == 0x194, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_34330) == 0x34330, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, overlayTex) == 0x343E4, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_34428) == 0x34428, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, cullCW) == 0x390FC, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, curTwoSided) == 0x39120, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, vsHandleA) == 0x39454, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, vsHandleB) == 0x39458, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, vsHandles) == 0x3946C, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_39470) == 0x39470, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_39474) == 0x39474, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_39478) == 0x39478, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_3947C) == 0x3947C, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, psHandle) == 0x39494, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, useTwoDirLights) == 0x394A4, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, lightSlotDir0) == 0x394A8, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, lightSlotDir1) == 0x394AC, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, lightSlotNamed) == 0x394B0, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_3954C) == 0x3954C, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, overlayUseProjection) == 0x39598, "CTsZoneMap: layout");
static_assert(XI_OFFSETOF(CTsZoneMap, field_3959C) == 0x3959C, "CTsZoneMap: layout");
