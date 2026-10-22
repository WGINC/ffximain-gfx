// CMoActorRotationDriveTask -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoActorRotationDriveTask {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x100569C0
    char vf8();  // 0x1005FB30

    void **vftable;  // +0x0
    uint8_t _pad_4[0x70];
    float field_74;  // +0x74
    float field_78;  // +0x78
    uint32_t field_7C;  // +0x7C
    float field_80;  // +0x80
    float field_84;  // +0x84
    float field_88;  // +0x88
    uint8_t _pad_8C[0x4];
    float field_90;  // +0x90
    float field_94;  // +0x94
    float field_98;  // +0x98
    uint8_t _pad_9C[0x4];
};
#pragma pack(pop)
static_assert(sizeof(CMoActorRotationDriveTask) == 0xA0, "CMoActorRotationDriveTask: sizeof");
static_assert(XI_OFFSETOF(CMoActorRotationDriveTask, field_74) == 0x74, "CMoActorRotationDriveTask: layout");
static_assert(XI_OFFSETOF(CMoActorRotationDriveTask, field_78) == 0x78, "CMoActorRotationDriveTask: layout");
static_assert(XI_OFFSETOF(CMoActorRotationDriveTask, field_7C) == 0x7C, "CMoActorRotationDriveTask: layout");
static_assert(XI_OFFSETOF(CMoActorRotationDriveTask, field_80) == 0x80, "CMoActorRotationDriveTask: layout");
static_assert(XI_OFFSETOF(CMoActorRotationDriveTask, field_84) == 0x84, "CMoActorRotationDriveTask: layout");
static_assert(XI_OFFSETOF(CMoActorRotationDriveTask, field_88) == 0x88, "CMoActorRotationDriveTask: layout");
static_assert(XI_OFFSETOF(CMoActorRotationDriveTask, field_90) == 0x90, "CMoActorRotationDriveTask: layout");
static_assert(XI_OFFSETOF(CMoActorRotationDriveTask, field_94) == 0x94, "CMoActorRotationDriveTask: layout");
static_assert(XI_OFFSETOF(CMoActorRotationDriveTask, field_98) == 0x98, "CMoActorRotationDriveTask: layout");
