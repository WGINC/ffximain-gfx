// CXiSkeletonActor -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CXiSkeletonActor {
public:
    _BYTE * PoseCompose(int a2, int a3, int a4, int a5, _BYTE *a6);  // 0x100343C0
    char * GetEnglishName();  // 0x10082F20
    int SubRef();  // 0x100832E0
    char * GetPos();  // 0x100A4740
    char * GetDir();  // 0x100A4750
    BOOL IsMotionLock();  // 0x100A4760
    int IsMotionLock_Set(char a2);  // 0x100A4770
    void EnableFlag8AC();  // 0x100A4790
    void DisableFlag8AC();  // 0x100A47A0
    int IsFlag8ACSet();  // 0x100A47B0
    char GetField79C();  // 0x100A47C0
    char SetField79C(char a2);  // 0x100A47D0
    bool IsReadComplete();  // 0x100A47E0
    _DWORD * vf256(_DWORD *a2);  // 0x100A47F0
    __int16 SetField670(__int16 a2);  // 0x100A4800
    __int16 GetField670();  // 0x100A4810
    char vf257();  // 0x100A4820
    void SetColor(unsigned int color);  // 0x100A4830
    unsigned int GetColor();  // 0x100A4840
    int vf017();  // 0x100A4850
    void SetMultiTexAlpha(int alpha);  // 0x100A4860
    char GetMultiTexAlpha();  // 0x100A4870
    void SetSahdowAlpha(int alpha);  // 0x100A4880
    char GetShadowAlpha();  // 0x100A4890
    void EnableEffect(unsigned int flagBit);  // 0x100A48A0
    void DisableEffect(unsigned int flagBit);  // 0x100A48C0
    void DisableEffectAll();  // 0x100A48E0
    int SetUVOffset(int a2, int a3);  // 0x100A48F0
    void SetDistAmount(float amount);  // 0x100A4910
    void SetEmapColor(unsigned int color);  // 0x100A4920
    void SetEmapTexture(void *texture);  // 0x100A4930
    void SetSeparateValue(float value);  // 0x100A4940
    void SetEmapAlphaMode(int mode);  // 0x100A4950
    int GetEmapAlphaMode();  // 0x100A4960
    _DWORD * vf258(_DWORD *a2);  // 0x100A4970
    int get_field_74C();  // 0x100A4980
    int set_field_73C(int a2);  // 0x100A4990
    double get_field_73C();  // 0x100A49A0
    int set_field_740(int a2);  // 0x100A49B0
    double get_field_740();  // 0x100A49C0
    int IsEnvmap();  // 0x100A49D0
    BOOL IsDistortion();  // 0x100A49E0
    BOOL IsMasking();  // 0x100A49F0
    BOOL vf030();  // 0x100A4A00
    int IsDirectionLock();  // 0x100A4A10
    __int16 IsConstrain();  // 0x100A4A20
    bool IsConstrain_Set(char a2, int a3);  // 0x100A4A30
    __int16 IsConstrain2();  // 0x100A4A50
    char IsConstrain2_Get(int a2);  // 0x100A4A60
    bool IsConstrain2_Set(char a2, int a3);  // 0x100A4A70
    char vf041();  // 0x100A4A90
    char vf042(char a2);  // 0x100A4AA0
    char vf043();  // 0x100A4AB0
    char vf044(char a2);  // 0x100A4AC0
    char vf045();  // 0x100A4AD0
    char vf046(char a2);  // 0x100A4AE0
    char vf047();  // 0x100A4AF0
    char ActivateFlag9F8(char a2);  // 0x100A4B00
    char ActivateFlagA04();  // 0x100A4B10
    char vf050(char a2);  // 0x100A4B20
    void vf099();  // 0x100A4B30
    void vf100();  // 0x100A4B40
    static char ** __cdecl GetRuntimeClass_variant0();  // 0x100AB070
    static char ** __cdecl GetRuntimeClass_variant1();  // 0x100C5190
    int ScalarDeletingDtor(char a1);  // 0x100C5540
    int Constructor_3Arg(_DWORD *a2, int a3, int a4);  // 0x100C56E0
    int Constructor_4Arg(_DWORD *a2, int a3, int a4, int a5);  // 0x100C5880
    int Constructor_Factory(_DWORD *a2, int a3, int a4);  // 0x100C59B0
    char DestructorBody();  // 0x100C5C00
    int RunStaggeredUpdates();  // 0x100CBD60
    int PerActor_FourBucketStaggeredUpdate();  // 0x100CBDE0
    char * SetMotion(int *a2, float a3);  // 0x100CD490
    _DWORD * ActorFindResource(_DWORD *a2, int a3, int a4);  // 0x100CE9B0
    _DWORD * ActorFindResourceNth(_DWORD *a2, int a3, int a4, int a5);  // 0x100CEB00
    int ActorGetResourceCount(int a2, int a3);  // 0x100CEC90
    void ReplaceAttack(int a1, int a2, int a3, int a4);  // 0x100CEDF0
    char SetAttack(int a2, int a3, int a4, char *Source);  // 0x100CEE00
    char SetAction(int a2, _DWORD *a3, void *a4);  // 0x100CEE40
    int SetAttack_SchStatus(int a2);  // 0x100CF030
    void vf172(int a2);  // 0x100CF200
    char SetModel(int a2);  // 0x100CF210
    char * GetField878Ptr_Candidate();  // 0x100D04B0
    char GetNamePos(int a2, float *a3, int a4);  // 0x100D04C0
    char OnDrawName();  // 0x100D0510
    int GetArrowPos(int a2, int a3, int a4);  // 0x100D06F0
    static char __stdcall ProjectToScreen(int a1, int a2, int a3);  // 0x100D0740
    bool IsReadCompleteBody();  // 0x100D12D0
    static char __cdecl OnSkeletonLoaded(int a1, int a2);  // 0x100D2F40
    char LoadSkeletonResource(int a2);  // 0x100D4330
    char GetElem(unsigned int a2, int a3);  // 0x100D4560
    char GetElemLocal(unsigned int a2, int a3);  // 0x100D4800
    char GetEidMatrix(int a2, _DWORD *a3);  // 0x100D49B0
    char IsDirectionLock_Set(char a2);  // 0x100D5460
    void KillAllGenerater();  // 0x100D6370
    void vf033();  // 0x100D6F90
    void vf034();  // 0x100D6FA0
    void vf035(int a1, int a2);  // 0x100D6FB0
    void vf036(int a1);  // 0x100D6FC0
    void vf037();  // 0x100D6FD0
    static char ** __cdecl GetRuntimeClass_variant2();  // 0x100D7140

    void **vftable;  // +0x0
    int32_t field_4;  // +0x4
    uint8_t _pad_8[0x68];
    uint32_t field_70;  // +0x70
    uint8_t _pad_74[0x8];
    int32_t field_7C;  // +0x7C
    uint32_t field_80;  // +0x80
    uint8_t _pad_84[0x4];
    int posHandle;  // +0x88
    uint8_t _pad_8C[0x8];
    float moveSpeedBase;  // +0x94
    float moveSpeed;  // +0x98
    float walkSpeed;  // +0x9C
    uint8_t _pad_A0[0x4];
    uint32_t field_A4;  // +0xA4
    uint8_t _pad_A8[0x8];
    char hiddenFlagByte;  // +0xB0
    uint8_t _pad_B1[0x1];
    __int16 hiddenFlagWord;  // +0xB2
    uint8_t _pad_B4[0x8];
    float field_BC;  // +0xBC
    uint8_t _pad_C0[0x39];
    uint8_t field_F9;  // +0xF9
    uint8_t _pad_FA[0x82];
    uint8_t field_17C;  // +0x17C
    uint8_t _pad_17D[0x41F];
    float field_59C;  // +0x59C
    uint8_t _pad_5A0[0x58];
    uint32_t field_5F8;  // +0x5F8
    uint8_t _pad_5FC[0xC];
    uint32_t field_608;  // +0x608
    uint8_t _pad_60C[0x54];
    unsigned int color;  // +0x660
    float field_664;  // +0x664
    unsigned int multiTexAlpha;  // +0x668
    unsigned int shadowAlpha;  // +0x66C
    uint16_t field_670;  // +0x670
    uint8_t _pad_672[0x26];
    float field_698;  // +0x698
    float field_69C;  // +0x69C
    float field_6A0;  // +0x6A0
    uint8_t _pad_6A4[0x84];
    int32_t field_728;  // +0x728
    uint8_t _pad_72C[0x4];
    int emapAlphaMode;  // +0x730
    unsigned int effectFlags;  // +0x734
    uint32_t field_738;  // +0x738
    uint32_t field_73C;  // +0x73C
    uint32_t field_740;  // +0x740
    float distAmount;  // +0x744
    float separateValue;  // +0x748
    unsigned int emapColor;  // +0x74C
    void * emapTexture;  // +0x750
    float field_754;  // +0x754
    float field_758;  // +0x758
    float field_75C;  // +0x75C
    float field_760;  // +0x760
    uint32_t field_764;  // +0x764
    uint32_t field_768;  // +0x768
    uint32_t field_76C;  // +0x76C
    uint8_t _pad_770[0x2C];
    uint8_t field_79C;  // +0x79C
    uint8_t _pad_79D[0x47];
    uint32_t field_7E4;  // +0x7E4
    uint8_t _pad_7E8[0x2C];
    uint32_t field_814;  // +0x814
    uint8_t _pad_818[0x20];
    int32_t field_838;  // +0x838
    uint16_t field_83C;  // +0x83C
    uint16_t field_83E;  // +0x83E
    uint32_t field_840;  // +0x840
    uint8_t _pad_844[0x20];
    uint32_t field_864;  // +0x864
    uint8_t _pad_868[0x10];
    uint8_t field_878;  // +0x878
    int8_t field_879;  // +0x879
    int8_t field_87A;  // +0x87A
    int8_t field_87B;  // +0x87B
    int8_t field_87C;  // +0x87C
    int8_t field_87D;  // +0x87D
    int8_t field_87E;  // +0x87E
    uint8_t _pad_87F[0x2];
    int8_t field_881;  // +0x881
    uint8_t field_882;  // +0x882
    uint8_t field_883;  // +0x883
    uint8_t field_884;  // +0x884
    uint8_t field_885;  // +0x885
    int8_t field_886;  // +0x886
    uint8_t _pad_887[0x1];
    int8_t field_888;  // +0x888
    int8_t field_889;  // +0x889
    uint8_t _pad_88A[0x12];
    uint8_t field_89C;  // +0x89C
    uint8_t field_89D;  // +0x89D
    uint8_t _pad_89E[0x6];
    uint8_t field_8A4;  // +0x8A4
    uint8_t field_8A5;  // +0x8A5
    uint8_t field_8A6;  // +0x8A6
    uint8_t field_8A7;  // +0x8A7
    uint8_t field_8A8;  // +0x8A8
    uint8_t field_8A9;  // +0x8A9
    uint8_t _pad_8AA[0x2];
    uint8_t field_8AC;  // +0x8AC
    uint8_t _pad_8AD[0x13];
    float field_8C0;  // +0x8C0
    uint32_t field_8C4;  // +0x8C4
    uint8_t _pad_8C8[0xC];
    uint32_t field_8D4;  // +0x8D4
    uint8_t _pad_8D8[0x10];
    uint8_t field_8E8;  // +0x8E8
    uint8_t _pad_8E9[0x7];
    char boundUnk0[16];  // +0x8F0
    float boundRadius;  // +0x900
    char boundUnk1[12];  // +0x904
    float field_910;  // +0x910
    uint8_t _pad_914[0xD0];
    uint32_t field_9E4;  // +0x9E4
    uint8_t field_9E8;  // +0x9E8
    uint8_t _pad_9E9[0x3];
    unsigned int updateSerial;  // +0x9EC
    unsigned int bucketA;  // +0x9F0
    unsigned int visibleSampleCount;  // +0x9F4
    char forceUpdateFlag;  // +0x9F8
    uint8_t _pad_9F9[0x3];
    unsigned int bucketB;  // +0x9FC
    uint32_t field_A00;  // +0xA00
    unsigned int drawBudgetGate;  // +0xA04
    uint32_t field_A08;  // +0xA08
};
#pragma pack(pop)
static_assert(sizeof(CXiSkeletonActor) == 0xA0C, "CXiSkeletonActor: sizeof");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_4) == 0x4, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_70) == 0x70, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_7C) == 0x7C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_80) == 0x80, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, posHandle) == 0x88, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, moveSpeedBase) == 0x94, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, moveSpeed) == 0x98, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, walkSpeed) == 0x9C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_A4) == 0xA4, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, hiddenFlagByte) == 0xB0, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, hiddenFlagWord) == 0xB2, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_BC) == 0xBC, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_F9) == 0xF9, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_17C) == 0x17C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_59C) == 0x59C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_5F8) == 0x5F8, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_608) == 0x608, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, color) == 0x660, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_664) == 0x664, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, multiTexAlpha) == 0x668, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, shadowAlpha) == 0x66C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_670) == 0x670, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_698) == 0x698, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_69C) == 0x69C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_6A0) == 0x6A0, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_728) == 0x728, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, emapAlphaMode) == 0x730, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, effectFlags) == 0x734, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_738) == 0x738, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_73C) == 0x73C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_740) == 0x740, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, distAmount) == 0x744, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, separateValue) == 0x748, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, emapColor) == 0x74C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, emapTexture) == 0x750, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_754) == 0x754, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_758) == 0x758, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_75C) == 0x75C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_760) == 0x760, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_764) == 0x764, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_768) == 0x768, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_76C) == 0x76C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_79C) == 0x79C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_7E4) == 0x7E4, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_814) == 0x814, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_838) == 0x838, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_83C) == 0x83C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_83E) == 0x83E, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_840) == 0x840, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_864) == 0x864, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_878) == 0x878, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_879) == 0x879, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_87A) == 0x87A, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_87B) == 0x87B, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_87C) == 0x87C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_87D) == 0x87D, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_87E) == 0x87E, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_881) == 0x881, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_882) == 0x882, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_883) == 0x883, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_884) == 0x884, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_885) == 0x885, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_886) == 0x886, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_888) == 0x888, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_889) == 0x889, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_89C) == 0x89C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_89D) == 0x89D, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8A4) == 0x8A4, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8A5) == 0x8A5, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8A6) == 0x8A6, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8A7) == 0x8A7, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8A8) == 0x8A8, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8A9) == 0x8A9, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8AC) == 0x8AC, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8C0) == 0x8C0, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8C4) == 0x8C4, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8D4) == 0x8D4, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8E8) == 0x8E8, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, boundUnk0) == 0x8F0, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, boundRadius) == 0x900, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, boundUnk1) == 0x904, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_910) == 0x910, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_9E4) == 0x9E4, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_9E8) == 0x9E8, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, updateSerial) == 0x9EC, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, bucketA) == 0x9F0, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, visibleSampleCount) == 0x9F4, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, forceUpdateFlag) == 0x9F8, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, bucketB) == 0x9FC, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_A00) == 0xA00, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, drawBudgetGate) == 0xA04, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_A08) == 0xA08, "CXiSkeletonActor: layout");
