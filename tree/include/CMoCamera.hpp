// CMoCamera -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoCamera {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x1003C880
    void vf008();  // 0x1003C890
    int SetPathData(_DWORD *a2);  // 0x1003CB30
    static void * __cdecl GetSharedInstance();  // 0x10071980
    void vf009();  // 0x100719F0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x30];
    uint32_t field_34;  // +0x34
    uint32_t field_38;  // +0x38
    uint32_t field_3C;  // +0x3C
    uint32_t field_40;  // +0x40
    uint8_t _pad_44[0x6C];
};
#pragma pack(pop)
static_assert(sizeof(CMoCamera) == 0xB0, "CMoCamera: sizeof");
static_assert(XI_OFFSETOF(CMoCamera, field_34) == 0x34, "CMoCamera: layout");
static_assert(XI_OFFSETOF(CMoCamera, field_38) == 0x38, "CMoCamera: layout");
static_assert(XI_OFFSETOF(CMoCamera, field_3C) == 0x3C, "CMoCamera: layout");
static_assert(XI_OFFSETOF(CMoCamera, field_40) == 0x40, "CMoCamera: layout");
