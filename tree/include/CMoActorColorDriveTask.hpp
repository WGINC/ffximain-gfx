// CMoActorColorDriveTask -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoActorColorDriveTask {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x100569A0
    int vf011();  // 0x10062FE0
    tagRECT * vf012(char a2);  // 0x10062FF0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x84];
};
#pragma pack(pop)
static_assert(sizeof(CMoActorColorDriveTask) == 0x88, "CMoActorColorDriveTask: sizeof");
