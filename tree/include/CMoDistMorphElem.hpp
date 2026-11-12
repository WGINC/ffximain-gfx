// CMoDistMorphElem -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CMoDistMorphElem {
public:
    int ApplyMorph(int a2, _DWORD *a3, int a4, int a5, _DWORD *C, char a7);  // 0x1003F440
    _DWORD * ComputeMorphHandle(int a2, int a3);  // 0x1003F8A0
    static char ** __cdecl GetRuntimeClass();  // 0x10043580
    static undefined8 __thiscall Construct(void * self);  // 0x10052E20
    BOOL vf012(int a1);  // 0x10052E40
    int set_field_1C0(int a2);  // 0x10052E50
    int * ScalarDeletingDtor(char a2);  // 0x10052E60

    void **vftable;  // +0x0
    uint8_t _pad_4[0x2E];
    uint8_t morphTargetCountAndFlags;  // +0x32 low 7 bits: number of morph targets/weights; bit 7: alpha test
    uint8_t _pad_33[0x1];
    int16_t positionCount;  // +0x34 number of 3-float positions per morph target
    int16_t packedNormalCount;  // +0x36 number of packed 10:10:10 normals per morph target
    uint8_t _pad_38[0x2];
    uint16_t triangleCount;  // +0x3A number of triangles emitted by ApplyMorph (3 vertices each)
    uint8_t _pad_3C[0x184];
    uint32_t field_1C0;  // +0x1C0
};
#pragma pack(pop)
static_assert(sizeof(CMoDistMorphElem) == 0x1C4, "CMoDistMorphElem: sizeof");
static_assert(XI_OFFSETOF(CMoDistMorphElem, morphTargetCountAndFlags) == 0x32, "CMoDistMorphElem: layout");
static_assert(XI_OFFSETOF(CMoDistMorphElem, positionCount) == 0x34, "CMoDistMorphElem: layout");
static_assert(XI_OFFSETOF(CMoDistMorphElem, packedNormalCount) == 0x36, "CMoDistMorphElem: layout");
static_assert(XI_OFFSETOF(CMoDistMorphElem, triangleCount) == 0x3A, "CMoDistMorphElem: layout");
static_assert(XI_OFFSETOF(CMoDistMorphElem, field_1C0) == 0x1C0, "CMoDistMorphElem: layout");
