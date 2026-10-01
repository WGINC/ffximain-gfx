// CMoLockColorDriveTask -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoLockColorDriveTask {
public:
    int vf011();  // 0x100630F0
    int vf012(int a1);  // 0x10063100

    void **vftable;  // +0x0
    uint8_t _pad_4[0x78];
};
#pragma pack(pop)
static_assert(sizeof(CMoLockColorDriveTask) == 0x7C, "CMoLockColorDriveTask: sizeof");
