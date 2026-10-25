// CXiActor -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CXiActor {
public:
    static void __thiscall Init(void * self);  // 0x10081BA0
    static void * __thiscall ConstructBase(void * self);  // 0x10081CF0
    double CXiSkeletonActor_GetMoveSpeed();  // 0x10082070
    float CXiSkeletonActor_GetMoveSpeedBase();  // 0x10082090
    double CXiSkeletonActor_GetWalkSpeed();  // 0x100820B0
    int CXiSkeletonActor_GetEquip(int a1);  // 0x100820D0
    static undefined4 __thiscall IsChocobo(void * self);  // 0x100827A0
    static undefined4 __thiscall IsStateA4Eq54(void * self);  // 0x100827F0
    static undefined4 __thiscall IsA4InRange3DTo51(void * self);  // 0x10082860
    static undefined4 __thiscall IsA4InRange25To34(void * self);  // 0x100828B0
    static undefined4 __thiscall IsA4InRange35To3C(void * self);  // 0x10082900
    static undefined1 * __thiscall GetName(void * self);  // 0x10082F60
    static undefined4 __thiscall GetField78Of70(void * self);  // 0x10082FA0
    static uint32_t __thiscall NameCalc(void * self, undefined4 * param_1, int param_2, float * param_3, uint32_t * param_4);  // 0x100830C0
    void CXiSkeletonActor_AddRef();  // 0x100832D0
    void CXiSkeletonActor_AddSchRef();  // 0x10083390
    void CXiSkeletonActor_SubSchRef();  // 0x100833A0
    static undefined4 __cdecl ComputeDrawState(void * param_1, int param_2, uint32_t param_3, uint32_t param_4, undefined4 param_5);  // 0x10083430
    static int * __cdecl FindNearestToPoint(float * param_1, uint32_t param_2, uint32_t param_3);  // 0x10083750
    static int * __cdecl FindNearestLowerInView(int * param_1, uint32_t param_2, float param_3);  // 0x10083B40
    static int * __cdecl FindNearestHigherInView(int * param_1, uint32_t param_2, float param_3);  // 0x10083D00
    static undefined4 __cdecl FindNearestToTarget_Mode0(undefined4 param_1, undefined4 param_2);  // 0x10083EC0
    static undefined4 __cdecl FindNearestToTarget_Mode1(uint32_t param_1, uint32_t param_2);  // 0x10083EE0
    static undefined4 __cdecl FindNearestToTarget_Mode2(uint32_t param_1, uint32_t param_2);  // 0x10083F00
    static int * __cdecl FindNearestToTarget(uint32_t param_1, uint32_t param_2, int param_3);  // 0x10083F20
    static undefined4 __cdecl IsWithinRadius(int param_1, int param_2, float param_3);  // 0x10084100
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
    static undefined4 __thiscall GetSubModelResult(void * self);  // 0x100845E0
    static uint32_t __thiscall TestModelFlags100000_200000_4000(void * self);  // 0x10084640
    static uint32_t __thiscall GetModelFlag120Bit5(void * self);  // 0x10084670
    static undefined4 __thiscall GetModelWord166(void * self);  // 0x10084690
    static undefined4 __thiscall GetModelByte1DB(void * self);  // 0x10084720
    static long double __thiscall GetModelField204(void * self);  // 0x100849A0
    static uint32_t __thiscall GetModelField12CBit17(void * self);  // 0x10084BA0
    static undefined4 __thiscall IsModelField210Set(void * self);  // 0x10084BC0
    static void __thiscall SpawnResourceGenerators(void * self, undefined4 * param_1);  // 0x10085060
    static int __thiscall IsInModelSlotTable(void * self);  // 0x10085490
    static uint32_t __thiscall GetTwoBitField(void * self);  // 0x10087DC0
    static undefined4 __thiscall Construct(void * self);  // 0x1008A120
    static undefined4 __thiscall SelectMotionTags(void * self);  // 0x1008A1A0
    static undefined4 __thiscall PushTagsToObject(void * self);  // 0x1008A2F0
    static undefined4 __thiscall InitDefaults(void * self, int param_1);  // 0x1008A350
    static void __thiscall Destroy(void * self);  // 0x1008A7B0
    static void __thiscall ResetRandomTimer(void * self);  // 0x1008D980
    static void __thiscall TickRandomTimer(void * self);  // 0x1008DA00
    static undefined4 __thiscall IsField78HighZero(void * self);  // 0x1008E140
    static undefined4 __thiscall IsFlag1d0Clear(void * self);  // 0x1008E150
    static uint32_t __thiscall CorrectOnZoneMap(void * self);  // 0x100C8280
    static void __thiscall RequestModelParts(void * self);  // 0x100D14C0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x7C];
    uint32_t field_80;  // +0x80
    uint32_t field_84;  // +0x84
    uint8_t _pad_88[0xC];
    float field_94;  // +0x94
    float field_98;  // +0x98
    uint8_t _pad_9C[0x28];
};
#pragma pack(pop)
static_assert(sizeof(CXiActor) == 0xC4, "CXiActor: sizeof");
static_assert(XI_OFFSETOF(CXiActor, field_80) == 0x80, "CXiActor: layout");
static_assert(XI_OFFSETOF(CXiActor, field_84) == 0x84, "CXiActor: layout");
static_assert(XI_OFFSETOF(CXiActor, field_94) == 0x94, "CXiActor: layout");
static_assert(XI_OFFSETOF(CXiActor, field_98) == 0x98, "CXiActor: layout");
