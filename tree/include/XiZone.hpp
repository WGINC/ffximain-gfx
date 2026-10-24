// XiZone -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API XiZone {
public:
    static void __cdecl RestoreSavedFields();  // 0x100BA080
    int DrawMarkerForward(float *a2, int a3, float a4);  // 0x10177CD0
    int DrawGroundShadowQuad(int a1, int a2, int a3);  // 0x10184C90
    int DrawDecalOnTriangles(float *a1, float *a2, IDirect3DBaseTexture8 *a3, float a4);  // 0x10185CA0
    static int __cdecl FindSubArea(int param_1);  // 0x101889C0
    static void __thiscall UpdateTree(void * self);  // 0x101889E0
    static void __thiscall DestroyChildList(void * self);  // 0x10188A10
    static uint32_t __thiscall TestHeadFlag(void * self);  // 0x10188B10
    static void __thiscall GetDirLights(void * self, undefined4 * param_1, undefined4 * param_2);  // 0x10188B70
    static void __cdecl GetInstance();  // 0x1018B130
    static void * __thiscall Constructor(void * self);  // 0x1018B170
    static void __thiscall SetTreeState1BC(void * self, int param_1);  // 0x1018B5E0
    static void __thiscall ResetState(void * self);  // 0x1018B620
    static void __thiscall DrawScene(void * self);  // 0x1018B860
    static long double __thiscall ComputeFogDistance(void * self, undefined4 param_2);  // 0x1018B8F0
    static void __thiscall GetDirLightsForArea(void * self, undefined4 param_1, undefined4 param_2, int param_3);  // 0x1018B960
    static void __thiscall GetDirLight(void * self, undefined4 param_1, undefined4 param_2, int param_3);  // 0x1018B9A0
    static void __thiscall GetAmbient(void * self, uint32_t * param_1, int param_2, int param_3);  // 0x1018B9E0
    static void __thiscall GetFog(void * self, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, int param_5);  // 0x1018BA20
    static void __thiscall SetPtr1D8FromTable(void * self, uint32_t param_1);  // 0x1018BAE0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x1D8];
};
#pragma pack(pop)
static_assert(sizeof(XiZone) == 0x1DC, "XiZone: sizeof");
