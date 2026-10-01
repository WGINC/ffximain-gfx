// CMoElem -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoElem {
public:
    static char ** __cdecl GetRuntimeClass_variant0();  // 0x100353C0
    void vf013(int a1);  // 0x10036A80
    void vf014(int a1);  // 0x10036A90
    int vf015();  // 0x10036AA0
    void vf019();  // 0x10036AB0
    static char ** __cdecl GetRuntimeClass_variant1();  // 0x1003E690
    static char ** __cdecl GetRuntimeClass_variant2();  // 0x10041000
    BOOL CMoElemFamily_IsKind0(int a1);  // 0x10041120
    static char ** __cdecl GetRuntimeClass_variant3();  // 0x100426A0
    static char ** __cdecl GetRuntimeClass_variant4();  // 0x100426D0
    static char ** __cdecl GetRuntimeClass_variant5();  // 0x10043190
    static char ** __cdecl GetRuntimeClass_variant6();  // 0x10043570
    static char ** __cdecl GetRuntimeClass_variant7();  // 0x10043700
    static char ** __cdecl GetRuntimeClass_variant8();  // 0x10044880
    int * ScalarDeletingDtor(char a2);  // 0x10044950
    char CMoElemFamily_SharedCleanup();  // 0x10044970
    void vf018();  // 0x100472E0
    static char ** __cdecl GetRuntimeClass_variant9();  // 0x10047550
    static char ** __cdecl GetRuntimeClass_variant10();  // 0x10047570
    static char ** __cdecl GetRuntimeClass_variant11();  // 0x100548A0
    static char ** __cdecl GetRuntimeClass_variant12();  // 0x100562D0
    static char ** __cdecl GetRuntimeClass_variant13();  // 0x10064160

    void **vftable;  // +0x0
    uint8_t _pad_4[0xF8];
    int32_t field_FC;  // +0xFC
    int32_t field_100;  // +0x100
    uint8_t _pad_104[0x4];
    int32_t field_108;  // +0x108
    uint8_t _pad_10C[0x58];
    int32_t field_164;  // +0x164
    uint8_t _pad_168[0x2C];
};
#pragma pack(pop)
static_assert(sizeof(CMoElem) == 0x194, "CMoElem: sizeof");
static_assert(XI_OFFSETOF(CMoElem, field_FC) == 0xFC, "CMoElem: layout");
static_assert(XI_OFFSETOF(CMoElem, field_100) == 0x100, "CMoElem: layout");
static_assert(XI_OFFSETOF(CMoElem, field_108) == 0x108, "CMoElem: layout");
static_assert(XI_OFFSETOF(CMoElem, field_164) == 0x164, "CMoElem: layout");
