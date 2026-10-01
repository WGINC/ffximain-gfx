// CXiActor -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CXiActor {
public:
    double CXiSkeletonActor_GetMoveSpeed();  // 0x10082070
    void CXiSkeletonActor_SetMoveSpeed(float speed);  // 0x10082080
    float CXiSkeletonActor_GetMoveSpeedBase();  // 0x10082090
    void CXiSkeletonActor_SetMoveSpeedBase(float speed);  // 0x100820A0
    void CXiSkeletonActor_SetWalkSpeed(float speed);  // 0x100820C0
    int CXiSkeletonActor_GetEquip(int a1);  // 0x100820D0
    void CXiSkeletonActor_AddRef();  // 0x100832D0
    void CXiSkeletonActor_AddSchRef();  // 0x10083390
    void CXiSkeletonActor_SubSchRef();  // 0x100833A0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x7C];
    uint32_t field_80;  // +0x80
    uint32_t field_84;  // +0x84
    uint8_t _pad_88[0x10];
    float field_98;  // +0x98
    uint8_t _pad_9C[0x28];
};
#pragma pack(pop)
static_assert(sizeof(CXiActor) == 0xC4, "CXiActor: sizeof");
static_assert(XI_OFFSETOF(CXiActor, field_80) == 0x80, "CXiActor: layout");
static_assert(XI_OFFSETOF(CXiActor, field_84) == 0x84, "CXiActor: layout");
static_assert(XI_OFFSETOF(CXiActor, field_98) == 0x98, "CXiActor: layout");
