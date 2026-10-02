// CMoResource_VT_1032C588 -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoResource_VT_1032C588 {
public:
    unsigned int ** ScalarDeletingDtor(char a2);  // 0x10071C90
    int vf011();  // 0x100745E0
    int vf012(int a1);  // 0x100745F0
    void vf008();  // 0x10074610
    __int16 vf009();  // 0x10074620

    void **vftable;  // +0x0
    uint8_t _pad_4[0x2C];
};
#pragma pack(pop)
static_assert(sizeof(CMoResource_VT_1032C588) == 0x30, "CMoResource_VT_1032C588: sizeof");
