// CMoPathDriveActorTask -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoPathDriveActorTask {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x10056940
    int vf011();  // 0x100630A0
    int vf012(int a1);  // 0x100630B0

    void **vftable;  // +0x0
    uint8_t _pad_4[0xAC];
};
#pragma pack(pop)
static_assert(sizeof(CMoPathDriveActorTask) == 0xB0, "CMoPathDriveActorTask: sizeof");
