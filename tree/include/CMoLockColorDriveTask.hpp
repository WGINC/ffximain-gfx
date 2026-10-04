// CMoLockColorDriveTask -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoLockColorDriveTask {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x10056960
    char vf8();  // 0x1005F3C0
    int vf011();  // 0x100630F0
    int vf012(int a1);  // 0x10063100

    void **vftable;  // +0x0
    uint8_t _pad_4[0x70];
    float field_74;  // +0x74
    uint8_t field_78;  // +0x78
    uint8_t _pad_79[0x3];
};
#pragma pack(pop)
static_assert(sizeof(CMoLockColorDriveTask) == 0x7C, "CMoLockColorDriveTask: sizeof");
static_assert(XI_OFFSETOF(CMoLockColorDriveTask, field_74) == 0x74, "CMoLockColorDriveTask: layout");
static_assert(XI_OFFSETOF(CMoLockColorDriveTask, field_78) == 0x78, "CMoLockColorDriveTask: layout");
