// CXiActor -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CXiActor {
public:
    double CXiSkeletonActor_GetMoveSpeed();  // 0x10082070
    void CXiSkeletonActor_SetMoveSpeed(float speed);  // 0x10082080
    float CXiSkeletonActor_GetMoveSpeedBase();  // 0x10082090
    void CXiSkeletonActor_SetMoveSpeedBase(float speed);  // 0x100820A0
    double CXiSkeletonActor_GetWalkSpeed();  // 0x100820B0
    void CXiSkeletonActor_SetWalkSpeed(float speed);  // 0x100820C0
    int CXiSkeletonActor_GetEquip(int a1);  // 0x100820D0
    static undefined4 __thiscall IsChocobo(void * self);  // 0x100827A0
    void CXiSkeletonActor_AddRef();  // 0x100832D0
    void CXiSkeletonActor_AddSchRef();  // 0x10083390
    void CXiSkeletonActor_SubSchRef();  // 0x100833A0
    static uint32_t __thiscall IsModelMissingOrFlag2000(void * self);  // 0x10084140
    static undefined4 __thiscall GetModelField170(void * self);  // 0x10084390
    static undefined4 __thiscall HasNonzeroField150or158(void * self);  // 0x100843B0
    static int __thiscall GetModelByteEE(void * self);  // 0x10084400
    static int __thiscall GetModelByteEF(void * self);  // 0x10084420
    static uint32_t __thiscall GetModelPartWord(void * self, int param_1);  // 0x10084480
    static uint32_t __thiscall GetModelFlags120Top2(void * self);  // 0x10084500
    static uint32_t __thiscall GetModelFlags130Top2(void * self);  // 0x10084520
    static long double __thiscall GetModelFloat9AEC0(void * self);  // 0x10084540
    static uint32_t __thiscall IsModelMissingOrSuppressed(void * self);  // 0x10084580
    static uint32_t __thiscall TestModelFlags100000_200000_4000(void * self);  // 0x10084640
    static undefined4 __thiscall GetModelByte1DB(void * self);  // 0x10084720
    static void __thiscall SpawnResourceGenerators(void * self, undefined4 * param_1);  // 0x10085060
    static int __thiscall IsInModelSlotTable(void * self);  // 0x10085490

    void **vftable;  // +0x0
    uint8_t _pad_4[0x7C];
    uint32_t field_80;  // +0x80
    uint32_t field_84;  // +0x84
    uint8_t _pad_88[0xC];
    float field_94;  // +0x94
    float field_98;  // +0x98
    float field_9C;  // +0x9C
    uint8_t _pad_A0[0x24];
};
#pragma pack(pop)
static_assert(sizeof(CXiActor) == 0xC4, "CXiActor: sizeof");
static_assert(XI_OFFSETOF(CXiActor, field_80) == 0x80, "CXiActor: layout");
static_assert(XI_OFFSETOF(CXiActor, field_84) == 0x84, "CXiActor: layout");
static_assert(XI_OFFSETOF(CXiActor, field_94) == 0x94, "CXiActor: layout");
static_assert(XI_OFFSETOF(CXiActor, field_98) == 0x98, "CXiActor: layout");
static_assert(XI_OFFSETOF(CXiActor, field_9C) == 0x9C, "CXiActor: layout");
