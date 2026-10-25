// CXiSkeletonActor -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CXiSkeletonActor {
public:
    char DrawModel(_DWORD *a2, __int16 *a3, int a4, _DWORD *a5, int a6, int a7, unsigned int a8);  // 0x1002B6E0
    static uint32_t __thiscall AnimateModels(void * self, int * param_1, int param_2, uint32_t param_3, undefined4 xi_s4);  // 0x1002C2D0
    void PrepareModelsOnce(int a2, int a3);  // 0x1002C360
    static undefined4 __thiscall GetElemThunk(void * self, undefined4 param_1, undefined4 param_2, undefined4 param_3, float * param_4, undefined4 xi_s5, undefined4 xi_s6, undefined4 xi_s7);  // 0x1002C3E0
    static int __thiscall GetModelElem(void * self, uint32_t param_1, int param_2);  // 0x1002C5C0
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
    _DWORD * vf256(_DWORD *a2);  // 0x100A47F0
    __int16 SetField670(__int16 a2);  // 0x100A4800
    __int16 GetField670();  // 0x100A4810
    char vf257();  // 0x100A4820
    void SetColor(unsigned int color);  // 0x100A4830
    unsigned int GetColor();  // 0x100A4840
    int vf017();  // 0x100A4850
    void SetMultiTexAlpha(int alpha);  // 0x100A4860
    void SetSahdowAlpha(int alpha);  // 0x100A4880
    void EnableEffect(unsigned int flagBit);  // 0x100A48A0
    void DisableEffect(unsigned int flagBit);  // 0x100A48C0
    int SetUVOffset(int a2, int a3);  // 0x100A48F0
    void SetEmapColor(unsigned int color);  // 0x100A4920
    void SetEmapTexture(void *texture);  // 0x100A4930
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
    int Constructor_1Arg(int a2);  // 0x100C52A0
    int ScalarDeletingDtor(char a1);  // 0x100C5540
    int Constructor_2Arg(_DWORD *a2, int a3);  // 0x100C5560
    int Constructor_3Arg(_DWORD *a2, int a3, int a4);  // 0x100C56E0
    int Constructor_4Arg(_DWORD *a2, int a3, int a4, int a5);  // 0x100C5880
    int Constructor_Factory(_DWORD *a2, int a3, int a4);  // 0x100C59B0
    char DestructorBody();  // 0x100C5C00
    int Init();  // 0x100C5D60
    static void __thiscall PlaceAttachedInRing(void * self, undefined4 xi_s1);  // 0x100C9C10
    static void __thiscall CopyModelValuesTo(void * self, int param_1);  // 0x100CB390
    void RunStaggeredUpdates();  // 0x100CBD60
    void PerActor_FourBucketStaggeredUpdate();  // 0x100CBDE0
    char * SetMotion(int *a2, float a3);  // 0x100CD490
    static void __thiscall SetSlotMotion(void * self, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, int param_7);  // 0x100CD4E0
    static undefined4 __thiscall SetMotion(void * self, int * param_1, uint32_t param_2, undefined4 param_3, float param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7, int param_8, int param_9, undefined4 param_10, undefined4 param_11);  // 0x100CD540
    static undefined4 __thiscall SetMotionFromSources(void * self, uint32_t param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7, undefined4 param_8, undefined4 param_9, int * param_10, int param_11);  // 0x100CD920
    static void __thiscall UpdateRandomTimer(void * self);  // 0x100CDF90
    static void __thiscall AdvanceMotionQueue(void * self);  // 0x100CE160
    _DWORD * ReverseFindRes(_DWORD *a2, int a3, int a4, int a5, int a6, char a7);  // 0x100CE480
    _DWORD * ActorFindResource(_DWORD *a2, int a3, int a4);  // 0x100CE9B0
    _DWORD * ActorFindResourceNth(_DWORD *a2, int a3, int a4, int a5);  // 0x100CEB00
    int ActorGetResourceCount(int a2, int a3);  // 0x100CEC90
    void ReplaceAttack(int a1, int a2, int a3, int a4);  // 0x100CEDF0
    char SetAttack(int a2, int a3, int a4, char *Source);  // 0x100CEE00
    static undefined4 __thiscall SetAction(void * self, undefined4 * param_1, undefined4 param_2, int param_3);  // 0x100CEE40
    int SetAttack_SchStatus(int a2);  // 0x100CF030
    static undefined8 __thiscall ForParts0To9(void * self, undefined4 xi_s1);  // 0x100CF100
    void vf172(int a2);  // 0x100CF200
    static void __thiscall AddResourceParts(void * self, int * param_1, void * param_2);  // 0x100CFA50
    static undefined * __thiscall GetTableOrDefault(void * self, int param_1);  // 0x100CFAD0
    static int __thiscall GetTableEntryByModelKind(void * self);  // 0x100CFB40
    char * GetField878Ptr_Candidate();  // 0x100D04B0
    char GetNamePos(int a2, float *a3, int a4);  // 0x100D04C0
    char GetArrowPos(int a2, int a3, int a4);  // 0x100D06F0
    static undefined4 __stdcall ProjectToScreen(float * param_1, undefined4 xi_s2, undefined4 xi_s3);  // 0x100D0740
    static void __thiscall DrawNameIfVisible(void * self, undefined4 * param_1, char * param_2, float param_3, uint32_t param_4, uint32_t param_5);  // 0x100D0880
    double GetWidthScale();  // 0x100D08E0
    double GetHeightScale();  // 0x100D09B0
    double GetDepthScale();  // 0x100D0A80
    static undefined4 __thiscall SetField0B4IfField9EC(void * self, int * param_1, int param_2);  // 0x100D0D40
    static void __thiscall SelectStateMessage(void * self, undefined4 xi_s1, undefined4 xi_s2);  // 0x100D0FC0
    static char __cdecl OnSkeletonLoaded(int a1, int a2);  // 0x100D2F40
    char LoadSkeletonResource(int a2);  // 0x100D4330
    static int __cdecl GetSkeletonFileNo();  // 0x100D43E0
    static int __thiscall AddListEntry(void * self, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x100D4410
    static int __thiscall FindListEntry(void * self, int param_1);  // 0x100D4540
    char IsDirectionLock_Set(char a2);  // 0x100D5460
    static uint32_t __thiscall FindPartValue(void * self, int * param_1);  // 0x100D6240
    char * DrawResourceShadows();  // 0x100D62C0
    void vf033();  // 0x100D6F90
    void vf034();  // 0x100D6FA0
    void vf035(int a1, int a2);  // 0x100D6FB0
    void vf036(int a1);  // 0x100D6FC0
    void vf037();  // 0x100D6FD0
    static char ** __cdecl GetRuntimeClass_variant2();  // 0x100D7140

    void **vftable;  // +0x0
    int32_t field_4;  // +0x4
    uint8_t _pad_8[0x28];
    uint32_t field_30;  // +0x30
    uint8_t _pad_34[0x10];
    float field_44;  // +0x44
    float field_48;  // +0x48
    float field_4C;  // +0x4C
    uint8_t _pad_50[0x20];
    uint32_t pModelInfo;  // +0x70 model descriptor: CXiActor_GetModelField170 and CXiActor_GetModelByteEE read +0x170 and +0
    uint8_t _pad_74[0xC];
    uint32_t refCount;  // +0x80 CXiSkeletonActor::SubRef decrements and returns it
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
    uint32_t field_B4;  // +0xB4
    uint8_t field_B8;  // +0xB8
    uint8_t _pad_B9[0x3];
    float distanceToCamera;  // +0xBC copied to g_shadowCasterDistance
    uint8_t _pad_C0[0x39];
    uint8_t field_F9;  // +0xF9
    uint8_t _pad_FA[0x82];
    uint8_t field_17C;  // +0x17C
    uint8_t _pad_17D[0x41F];
    int32_t field_59C;  // +0x59C
    uint8_t _pad_5A0[0x58];
    uint32_t field_5F8;  // +0x5F8
    int32_t field_5FC;  // +0x5FC
    int32_t field_600;  // +0x600
    int32_t field_604;  // +0x604
    uint32_t field_608;  // +0x608
    int32_t field_60C;  // +0x60C
    int32_t field_610;  // +0x610
    int32_t field_614;  // +0x614
    int32_t field_618;  // +0x618
    int32_t field_61C;  // +0x61C
    int32_t field_620;  // +0x620
    int32_t field_624;  // +0x624
    int32_t field_628;  // +0x628
    int32_t field_62C;  // +0x62C
    int32_t field_630;  // +0x630
    int32_t field_634;  // +0x634
    int32_t field_638;  // +0x638
    uint8_t _pad_63C[0x20];
    int32_t field_65C;  // +0x65C
    unsigned int color;  // +0x660
    int32_t field_664;  // +0x664
    unsigned int multiTexAlpha;  // +0x668
    unsigned int shadowAlpha;  // +0x66C
    uint16_t field_670;  // +0x670
    uint8_t _pad_672[0xB6];
    int32_t field_728;  // +0x728
    uint8_t _pad_72C[0x4];
    int emapAlphaMode;  // +0x730
    unsigned int effectFlags;  // +0x734
    uint32_t field_738;  // +0x738
    uint32_t uvOffsetU;  // +0x73C first argument of CXiSkeletonActor::SetUVOffset
    uint32_t uvOffsetV;  // +0x740 second argument of CXiSkeletonActor::SetUVOffset
    float distAmount;  // +0x744
    float separateValue;  // +0x748
    unsigned int emapColor;  // +0x74C
    void * emapTexture;  // +0x750
    float baseScale;  // +0x754 set from Actor_GetPercentScaleByIndex in SetModel
    float depthScale;  // +0x758 GetDepthScale returns it (x model depth x 0.588) when >= 0, else baseScale
    float heightScale;  // +0x75C GetHeightScale returns it when >= 0, else baseScale
    float widthScale;  // +0x760 GetWidthScale returns it when >= 0, else baseScale
    uint32_t field_764;  // +0x764
    uint32_t field_768;  // +0x768
    uint32_t field_76C;  // +0x76C
    uint8_t _pad_770[0x28];
    int32_t field_798;  // +0x798
    uint8_t field_79C;  // +0x79C
    uint8_t _pad_79D[0x3];
    int32_t field_7A0;  // +0x7A0
    int32_t field_7A4;  // +0x7A4
    uint8_t _pad_7A8[0xC];
    int32_t field_7B4;  // +0x7B4
    int32_t field_7B8;  // +0x7B8
    int32_t field_7BC;  // +0x7BC
    int32_t field_7C0;  // +0x7C0
    uint32_t field_7C4;  // +0x7C4
    uint8_t _pad_7C8[0x11];
    uint8_t field_7D9;  // +0x7D9
    uint16_t field_7DA;  // +0x7DA
    int32_t field_7DC;  // +0x7DC
    int32_t field_7E0;  // +0x7E0
    int32_t resourceList;  // +0x7E4 list walked with List_GetTail in IsReadCompleteBody
    int32_t field_7E8;  // +0x7E8
    int32_t field_7EC;  // +0x7EC
    int32_t field_7F0;  // +0x7F0
    int32_t field_7F4;  // +0x7F4
    int32_t field_7F8;  // +0x7F8
    int32_t field_7FC;  // +0x7FC
    int32_t field_800;  // +0x800
    int32_t field_804;  // +0x804
    int32_t field_808;  // +0x808
    int32_t field_80C;  // +0x80C
    int32_t field_810;  // +0x810
    uint32_t field_814;  // +0x814
    int32_t field_818;  // +0x818
    int32_t field_81C;  // +0x81C
    int32_t field_820;  // +0x820
    int32_t field_824;  // +0x824
    int32_t field_828;  // +0x828
    uint8_t _pad_82C[0xC];
    int32_t directionLockCount;  // +0x838 IsDirectionLock_Set increments or decrements it
    uint8_t constrain;  // +0x83C returned by CXiSkeletonActor::IsConstrain
    uint8_t _pad_83D[0x1];
    uint16_t constrain2;  // +0x83E returned by CXiSkeletonActor::IsConstrain2
    int32_t lockFlags;  // +0x840 bit 0x40 is the motion lock (IsMotionLock / IsMotionLock_Set)
    int32_t field_844;  // +0x844
    int32_t field_848;  // +0x848
    int32_t field_84C;  // +0x84C
    int32_t field_850;  // +0x850
    int32_t field_854;  // +0x854
    int32_t field_858;  // +0x858
    int32_t field_85C;  // +0x85C
    int32_t field_860;  // +0x860
    uint32_t field_864;  // +0x864
    int32_t field_868;  // +0x868
    int32_t field_86C;  // +0x86C
    int32_t field_870;  // +0x870
    int32_t field_874;  // +0x874
    uint8_t field_878;  // +0x878
    uint8_t _pad_879[0x27];
    int32_t field_8A0;  // +0x8A0
    uint8_t field_8A4;  // +0x8A4
    uint8_t field_8A5;  // +0x8A5
    uint8_t field_8A6;  // +0x8A6
    uint8_t field_8A7;  // +0x8A7
    uint8_t field_8A8;  // +0x8A8
    uint8_t field_8A9;  // +0x8A9
    uint8_t field_8AA;  // +0x8AA
    uint8_t field_8AB;  // +0x8AB
    uint8_t field_8AC;  // +0x8AC
    uint8_t _pad_8AD[0x3];
    int32_t field_8B0;  // +0x8B0
    uint8_t field_8B4;  // +0x8B4
    uint8_t field_8B5;  // +0x8B5
    uint8_t field_8B6;  // +0x8B6
    uint8_t _pad_8B7[0x1];
    int32_t field_8B8;  // +0x8B8
    int32_t field_8BC;  // +0x8BC
    float nameHeightOffset;  // +0x8C0 added to the Y of the position returned by GetNamePos and GetArrowPos
    uint32_t field_8C4;  // +0x8C4
    uint8_t _pad_8C8[0xC];
    int32_t field_8D4;  // +0x8D4
    uint8_t _pad_8D8[0x10];
    uint8_t field_8E8;  // +0x8E8
    uint8_t _pad_8E9[0x3];
    int32_t field_8EC;  // +0x8EC
    char boundUnk0[16];  // +0x8F0
    float boundRadius;  // +0x900
    char boundUnk1[12];  // +0x904
    float field_910;  // +0x910
    uint8_t _pad_914[0xC4];
    int32_t field_9D8;  // +0x9D8
    int32_t field_9DC;  // +0x9DC
    int32_t field_9E0;  // +0x9E0
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
    uint32_t pSkeletonRes;  // +0xA08 the CXiSkeletonActorRes load request created in LoadSkeletonResource (callback CXiSkeleton
};
#pragma pack(pop)
static_assert(sizeof(CXiSkeletonActor) == 0xA0C, "CXiSkeletonActor: sizeof");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_4) == 0x4, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_30) == 0x30, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_44) == 0x44, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_48) == 0x48, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_4C) == 0x4C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, pModelInfo) == 0x70, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, refCount) == 0x80, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, posHandle) == 0x88, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, moveSpeedBase) == 0x94, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, moveSpeed) == 0x98, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, walkSpeed) == 0x9C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_A4) == 0xA4, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, hiddenFlagByte) == 0xB0, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, hiddenFlagWord) == 0xB2, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_B4) == 0xB4, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_B8) == 0xB8, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, distanceToCamera) == 0xBC, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_F9) == 0xF9, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_17C) == 0x17C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_59C) == 0x59C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_5F8) == 0x5F8, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_5FC) == 0x5FC, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_600) == 0x600, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_604) == 0x604, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_608) == 0x608, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_60C) == 0x60C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_610) == 0x610, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_614) == 0x614, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_618) == 0x618, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_61C) == 0x61C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_620) == 0x620, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_624) == 0x624, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_628) == 0x628, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_62C) == 0x62C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_630) == 0x630, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_634) == 0x634, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_638) == 0x638, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_65C) == 0x65C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, color) == 0x660, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_664) == 0x664, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, multiTexAlpha) == 0x668, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, shadowAlpha) == 0x66C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_670) == 0x670, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_728) == 0x728, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, emapAlphaMode) == 0x730, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, effectFlags) == 0x734, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_738) == 0x738, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, uvOffsetU) == 0x73C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, uvOffsetV) == 0x740, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, distAmount) == 0x744, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, separateValue) == 0x748, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, emapColor) == 0x74C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, emapTexture) == 0x750, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, baseScale) == 0x754, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, depthScale) == 0x758, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, heightScale) == 0x75C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, widthScale) == 0x760, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_764) == 0x764, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_768) == 0x768, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_76C) == 0x76C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_798) == 0x798, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_79C) == 0x79C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_7A0) == 0x7A0, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_7A4) == 0x7A4, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_7B4) == 0x7B4, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_7B8) == 0x7B8, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_7BC) == 0x7BC, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_7C0) == 0x7C0, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_7C4) == 0x7C4, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_7D9) == 0x7D9, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_7DA) == 0x7DA, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_7DC) == 0x7DC, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_7E0) == 0x7E0, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, resourceList) == 0x7E4, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_7E8) == 0x7E8, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_7EC) == 0x7EC, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_7F0) == 0x7F0, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_7F4) == 0x7F4, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_7F8) == 0x7F8, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_7FC) == 0x7FC, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_800) == 0x800, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_804) == 0x804, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_808) == 0x808, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_80C) == 0x80C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_810) == 0x810, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_814) == 0x814, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_818) == 0x818, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_81C) == 0x81C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_820) == 0x820, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_824) == 0x824, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_828) == 0x828, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, directionLockCount) == 0x838, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, constrain) == 0x83C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, constrain2) == 0x83E, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, lockFlags) == 0x840, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_844) == 0x844, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_848) == 0x848, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_84C) == 0x84C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_850) == 0x850, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_854) == 0x854, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_858) == 0x858, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_85C) == 0x85C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_860) == 0x860, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_864) == 0x864, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_868) == 0x868, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_86C) == 0x86C, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_870) == 0x870, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_874) == 0x874, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_878) == 0x878, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8A0) == 0x8A0, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8A4) == 0x8A4, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8A5) == 0x8A5, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8A6) == 0x8A6, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8A7) == 0x8A7, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8A8) == 0x8A8, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8A9) == 0x8A9, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8AA) == 0x8AA, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8AB) == 0x8AB, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8AC) == 0x8AC, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8B0) == 0x8B0, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8B4) == 0x8B4, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8B5) == 0x8B5, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8B6) == 0x8B6, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8B8) == 0x8B8, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8BC) == 0x8BC, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, nameHeightOffset) == 0x8C0, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8C4) == 0x8C4, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8D4) == 0x8D4, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8E8) == 0x8E8, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_8EC) == 0x8EC, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, boundUnk0) == 0x8F0, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, boundRadius) == 0x900, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, boundUnk1) == 0x904, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_910) == 0x910, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_9D8) == 0x9D8, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_9DC) == 0x9DC, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_9E0) == 0x9E0, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_9E4) == 0x9E4, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_9E8) == 0x9E8, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, updateSerial) == 0x9EC, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, bucketA) == 0x9F0, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, visibleSampleCount) == 0x9F4, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, forceUpdateFlag) == 0x9F8, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, bucketB) == 0x9FC, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, field_A00) == 0xA00, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, drawBudgetGate) == 0xA04, "CXiSkeletonActor: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActor, pSkeletonRes) == 0xA08, "CXiSkeletonActor: layout");
