// CMoLockLookAtDriveTask -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoLockLookAtDriveTask {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x10056980
    char vf8();  // 0x1005F540
    int vf011();  // 0x10062F80
    int vf012(int a1);  // 0x10062F90

    void **vftable;  // +0x0
    uint8_t _pad_4[0x70];
    float field_74;  // +0x74
    float field_78;  // +0x78
    float field_7C;  // +0x7C
};
#pragma pack(pop)
static_assert(sizeof(CMoLockLookAtDriveTask) == 0x80, "CMoLockLookAtDriveTask: sizeof");
static_assert(XI_OFFSETOF(CMoLockLookAtDriveTask, field_74) == 0x74, "CMoLockLookAtDriveTask: layout");
static_assert(XI_OFFSETOF(CMoLockLookAtDriveTask, field_78) == 0x78, "CMoLockLookAtDriveTask: layout");
static_assert(XI_OFFSETOF(CMoLockLookAtDriveTask, field_7C) == 0x7C, "CMoLockLookAtDriveTask: layout");
