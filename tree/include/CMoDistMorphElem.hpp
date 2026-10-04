// CMoDistMorphElem -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoDistMorphElem {
public:
    int ApplyMorph(int a2, _DWORD *a3, int a4, int a5, _DWORD *C, char a7);  // 0x1003F440
    _DWORD * ComputeMorphHandle(int a2, int a3);  // 0x1003F8A0
    static char __stdcall ValidateMorphData(int a1, int a2, int a3, int a4);  // 0x10042D00
    static char ** __cdecl GetRuntimeClass();  // 0x10043580
    char UpdateMorphBlend();  // 0x10043590
    BOOL vf012(int a1);  // 0x10052E40
    int set_field_1C0(int a2);  // 0x10052E50
    int * ScalarDeletingDtor(char a2);  // 0x10052E60

    void **vftable;  // +0x0
    uint8_t _pad_4[0x2E];
    uint8_t field_32;  // +0x32
    uint8_t _pad_33[0x1];
    int16_t field_34;  // +0x34
    int16_t field_36;  // +0x36
    uint8_t _pad_38[0x2];
    uint16_t field_3A;  // +0x3A
    uint8_t _pad_3C[0xC8];
    int32_t pProcessor;  // +0x104 used as the CMoProcessor in UpdateMorphBlend
    uint8_t _pad_108[0xB4];
    int32_t morphHandle;  // +0x1BC computed once by CMoDistMorphElem_ComputeMorphHandle in UpdateMorphBlend and passed to Val
    uint32_t field_1C0;  // +0x1C0
};
#pragma pack(pop)
static_assert(sizeof(CMoDistMorphElem) == 0x1C4, "CMoDistMorphElem: sizeof");
static_assert(XI_OFFSETOF(CMoDistMorphElem, field_32) == 0x32, "CMoDistMorphElem: layout");
static_assert(XI_OFFSETOF(CMoDistMorphElem, field_34) == 0x34, "CMoDistMorphElem: layout");
static_assert(XI_OFFSETOF(CMoDistMorphElem, field_36) == 0x36, "CMoDistMorphElem: layout");
static_assert(XI_OFFSETOF(CMoDistMorphElem, field_3A) == 0x3A, "CMoDistMorphElem: layout");
static_assert(XI_OFFSETOF(CMoDistMorphElem, pProcessor) == 0x104, "CMoDistMorphElem: layout");
static_assert(XI_OFFSETOF(CMoDistMorphElem, morphHandle) == 0x1BC, "CMoDistMorphElem: layout");
static_assert(XI_OFFSETOF(CMoDistMorphElem, field_1C0) == 0x1C0, "CMoDistMorphElem: layout");
