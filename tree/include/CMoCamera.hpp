// CMoCamera -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoCamera {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x1003C880
    void vf008();  // 0x1003C890
    static void * __cdecl GetSharedInstance();  // 0x10071980
    void vf009();  // 0x100719F0
    __int16 GetLinkedObject(_DWORD *a2);  // 0x10071E30

    void **vftable;  // +0x0
    uint8_t _pad_4[0x10];
    uint32_t field_14;  // +0x14
    uint8_t _pad_18[0x98];
};
#pragma pack(pop)
static_assert(sizeof(CMoCamera) == 0xB0, "CMoCamera: sizeof");
static_assert(XI_OFFSETOF(CMoCamera, field_14) == 0x14, "CMoCamera: layout");
