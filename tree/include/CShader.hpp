// CShader -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CShader {
public:
    static char ** __cdecl GetRuntimeClass();  // 0x10017750
    unsigned int * ScalarDeletingDtor(char a2);  // 0x10017780
    int CShader_Destructor();  // 0x100177A0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x10];
};
#pragma pack(pop)
static_assert(sizeof(CShader) == 0x14, "CShader: sizeof");
