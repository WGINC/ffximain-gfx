// YkJpeg -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API YkJpeg {
public:
    static void __thiscall InitTables(void * self);  // 0x101CF870
    static void __stdcall CopyBlock(int param_1, int param_2, int param_3, int param_4, int param_5, int param_6);  // 0x101CFDB0
    static int __stdcall Unpack565(uint32_t param_1, int * param_2, int * param_3, int param_4);  // 0x101CFE20
    static void __thiscall UnpackBlock565(void * self, int param_1, int param_2, int param_3, int param_4, int param_5, int * param_6);  // 0x101CFEB0
    static void __stdcall ForwardDct(int param_1);  // 0x101CFF60
    static void __thiscall EncodeBlock(void * self, int * param_1, char * param_2);  // 0x101D0210
    static undefined4 __thiscall Encode(void * self, uint32_t param_1, uint32_t param_2, int param_3, int param_4);  // 0x101D0370
    static undefined * __cdecl GetCopyrightString();  // 0x101D05E0
    static int __thiscall EncodeChecked(void * self, uint32_t param_1, uint32_t param_2, int param_3, int param_4);  // 0x101D0600
    static undefined4 __thiscall Destructor(void * self);  // 0x101D08C0
    static void __thiscall Release(void * self);  // 0x101D08D0

    void **vftable;  // +0x0
    uint8_t _pad_4[0xCF8];
};
#pragma pack(pop)
static_assert(sizeof(YkJpeg) == 0xCFC, "YkJpeg: sizeof");
