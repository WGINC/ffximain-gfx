// gfx_free.hpp -- graphics functions with no established class.
#pragma once
#include "gfx_common.hpp"
XI_API char __thiscall fn_10001520(void *self_);  // 0x10001520
XI_API char __cdecl fn_10002040(struct IDirect3DSurface8 *a1, const struct tagRECT *a2, unsigned int *a3, int a4);  // 0x10002040
XI_API unsigned __int8 __cdecl fn_10002BE0(void);  // 0x10002BE0
XI_API CDx * __cdecl fn_10002C00(int a1, int a2, int a3);  // 0x10002C00
XI_API CDx * __cdecl fn_10002C30(int a1);  // 0x10002C30
XI_API CDx * __cdecl fn_10002C80(int a1);  // 0x10002C80
XI_API CDx * __cdecl fn_10002CD0(int a1);  // 0x10002CD0
XI_API int __cdecl fn_10002D20(int a1, int a2, int a3);  // 0x10002D20
XI_API char __cdecl Gfx_CreateD3DHolder(int a1, int a2, int a3, char a4, char a5);  // 0x10002F20
XI_API char __thiscall Gfx_D3DBootstrap(int self_, int a2, int a3, int a4, char a5, char a6, int a7);  // 0x100031F0
XI_API char __thiscall Gfx_TryCreateDeviceForMode(_DWORD **self_, int a2, int a3, int a4, char a5, char a6, int a7, int a8);  // 0x100033C0
XI_API char __thiscall Gfx_CreateDevice(_DWORD *self_, int a2, int a3, int a4, int a5, int a6, char a7, char a8, int a9);  // 0x100034B0
XI_API int __thiscall fn_10003B60(_DWORD *self_);  // 0x10003B60
XI_API void __stdcall fn_10003D70(_DWORD *a1, const char *a2, int a3);  // 0x10003D70
XI_API char __thiscall DirectInput_InstallMouse(_DWORD **self_);  // 0x10007EF0
XI_API _UNKNOWN ** __cdecl CDx_GetRuntimeClass(void);  // 0x10008F80
XI_API int __cdecl CDx_Present(void);  // 0x10009720
XI_API char ** __cdecl CMoTask_GetRuntimeClass_variant0(void);  // 0x10010310
XI_API void __cdecl CMoTask_vf007(void);  // 0x10011920
XI_API void __cdecl CMoTask_vf009(void);  // 0x10011930
XI_API void __stdcall CMoTask_vf010(int a1);  // 0x10011940
XI_API int __cdecl Post_Afterimage(void);  // 0x10012F40
XI_API int __thiscall Post_FlashAndZoomFeedback(void *self_);  // 0x100132B0
XI_API char __thiscall fn_10013AD0(int self_, int a2);  // 0x10013AD0
XI_API char __thiscall fn_10013DF0(_WORD *self_);  // 0x10013DF0
XI_API char __thiscall fn_10013F10(_WORD *self_);  // 0x10013F10
XI_API char __thiscall fn_10014030(_WORD *self_);  // 0x10014030
XI_API bool __cdecl fn_10014120(void);  // 0x10014120
XI_API char __thiscall fn_100141C0(_WORD *self_);  // 0x100141C0
XI_API int __cdecl fn_100157E0(int a1, int a2);  // 0x100157E0
XI_API char __cdecl fn_100161B0(void);  // 0x100161B0
XI_API void __thiscall fn_10017230(int self_);  // 0x10017230
XI_API char ** __cdecl CShader_GetRuntimeClass(void);  // 0x10017750
XI_API int __thiscall fn_100177A0(unsigned int *self_);  // 0x100177A0
XI_API void * __thiscall fn_100186C0(void *self_, char *Source);  // 0x100186C0
XI_API CDx * __thiscall fn_100187D0(int *self_);  // 0x100187D0
XI_API void __stdcall CMoResource_VT_1032C7FC_vf005(int a1);  // 0x10019A40
XI_API char ** __cdecl CMoTask_GetRuntimeClass_variant1(void);  // 0x1001B800
XI_API _DWORD * __cdecl fn_1001D0A0(void);  // 0x1001D0A0
XI_API char __thiscall fn_1001D240(int self_);  // 0x1001D240
XI_API char __thiscall fn_1001D350(_DWORD *self_);  // 0x1001D350
XI_API int __thiscall fn_1001D3B0(void *self_);  // 0x1001D3B0
XI_API char __cdecl fn_1001D940(int a1, int a2, int a3, int a4);  // 0x1001D940
XI_API int __thiscall fn_1001DC70(void *self_, int a1, int a2);  // 0x1001DC70
XI_API int __stdcall fn_1001DD00(int a1, int a2);  // 0x1001DD00
XI_API void __stdcall CMoResource_VT_1032C880_vf005(int a1);  // 0x1001E060
XI_API int __cdecl fn_10022FA0(int a1);  // 0x10022FA0
XI_API int __cdecl CYyModelDt_VT_1032A430_vf011(void);  // 0x10025090
XI_API char __thiscall fn_10025330(_DWORD *self_, int *a2, int a3, _DWORD *a4);  // 0x10025330
XI_API int __thiscall fn_100255C0(void *self_, int a1);  // 0x100255C0
XI_API int __thiscall fn_10025870(void *self_, int a1, int a2, int a3, int a4);  // 0x10025870
XI_API char __stdcall fn_10025C40(int *a1, int a2, float *a3);  // 0x10025C40
XI_API int __stdcall fn_10025F70(int a1);  // 0x10025F70
XI_API void __cdecl fn_100262C0(void);  // 0x100262C0
XI_API void __cdecl fn_100263F0(void);  // 0x100263F0
XI_API void __stdcall fn_100266C0(int a1);  // 0x100266C0
XI_API int __thiscall fn_10026990(void *self_, int a1, int a2, int a3, int a4);  // 0x10026990
XI_API int __thiscall fn_10026C70(void *self_, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);  // 0x10026C70
XI_API void __cdecl fn_100276F0(float *a1, _DWORD *a2);  // 0x100276F0
XI_API void __cdecl fn_100277E0(float *a1, _DWORD *a2);  // 0x100277E0
XI_API void __cdecl fn_10027870(float *a1, _DWORD *a2);  // 0x10027870
XI_API float * __cdecl fn_100278E0(float *a1);  // 0x100278E0
XI_API float * __cdecl fn_10027930(float *a1, float *a2);  // 0x10027930
XI_API int __stdcall fn_100295A0(int a1, int a2, int a3, int a4, int a5);  // 0x100295A0
XI_API char ** __cdecl CYyModelDt_GetRuntimeClass(void);  // 0x10029880
XI_API char ** __cdecl CYyModelBase_GetRuntimeClass(void);  // 0x100298A0
XI_API char ** __cdecl CYyModel_GetRuntimeClass(void);  // 0x100298C0
XI_API int __thiscall fn_1002A3B0(int self_, int a2, int a3, int a4);  // 0x1002A3B0
XI_API void __thiscall fn_1002C360(int self_, int a2, int a3);  // 0x1002C360
XI_API char ** __cdecl CMoTask_GetRuntimeClass_variant2(void);  // 0x1002C950
XI_API char __thiscall fn_1002CCB0(int *self_, int a2);  // 0x1002CCB0
XI_API int __thiscall fn_1002D680(int self_, _DWORD **a2);  // 0x1002D680
XI_API char __thiscall fn_1002D700(int self_);  // 0x1002D700
XI_API int __cdecl fn_1002F160(unsigned int a1);  // 0x1002F160
XI_API int __cdecl fn_1002F1A0(void);  // 0x1002F1A0
XI_API char __thiscall fn_10033800(int self_, int *a2, int a3);  // 0x10033800
XI_API char __thiscall fn_10033A70(_DWORD *self_, int a2);  // 0x10033A70
XI_API void __stdcall CMoResource_VT_1032C828_vf005(int a1);  // 0x10034180
XI_API char ** __cdecl CMoElem_GetRuntimeClass_variant0(void);  // 0x100353C0
XI_API void __stdcall CMoElem_vf013(int a1);  // 0x10036A80
XI_API void __stdcall CMoElem_vf014(int a1);  // 0x10036A90
XI_API int __cdecl CMoElem_vf015(void);  // 0x10036AA0
XI_API void __cdecl CMoElem_vf019(void);  // 0x10036AB0
XI_API char ** __cdecl CMoResource_GetRuntimeClass_variant0(void);  // 0x10036B70
XI_API char ** __cdecl CMoTask_GetRuntimeClass_variant3(void);  // 0x10037280
XI_API int __thiscall Font_DrawPass(void *self_);  // 0x10038520
XI_API unsigned __int8 * __cdecl fn_10038720(unsigned __int8 *a1, float a2, float a3, float a4);  // 0x10038720
XI_API void __cdecl CMoResource_VT_1032C7A4_vf008(void);  // 0x10038920
XI_API _DWORD * __cdecl fn_10038930(void);  // 0x10038930
XI_API int __thiscall fn_10038A80(void *self_);  // 0x10038A80
XI_API int __thiscall fn_10038E10(void *self_);  // 0x10038E10
XI_API char ** __cdecl CYyTexBase_GetRuntimeClass_variant0(void);  // 0x10039770
XI_API char ** __cdecl CYyTexBase_GetRuntimeClass_variant1(void);  // 0x10039780
XI_API _UNKNOWN ** __cdecl CYyTex_GetRuntimeClass(void);  // 0x10039790
XI_API int __thiscall TexObj_Upload(void *self_);  // 0x10039E50
XI_API char ** __cdecl CYyTexMng_GetRuntimeClass(void);  // 0x1003AA40
XI_API int __thiscall fn_1003B210(_DWORD *self_, int a2, unsigned int a3, unsigned int a4, D3DPOOL a5, int *a6, char a7);  // 0x1003B210
XI_API char ** __cdecl CMoAttachments_GetRuntimeClass(void);  // 0x1003B530
XI_API char __cdecl CMoAttachments_vf004(void);  // 0x1003B570
XI_API char __cdecl CMoAttachments_vf005(void);  // 0x1003B580
XI_API int __cdecl CMoAttachments_vf006(void);  // 0x1003B590
XI_API char ** __cdecl CMoCamera_GetRuntimeClass(void);  // 0x1003C880
XI_API void __cdecl CMoCamera_vf008(void);  // 0x1003C890
XI_API char ** __cdecl CMoTask_GetRuntimeClass_variant4(void);  // 0x1003CC60
XI_API char ** __cdecl CMoCameraTask_GetRuntimeClass(void);  // 0x1003CC70
XI_API char ** __cdecl CMoCelestial_GetRuntimeClass(void);  // 0x1003D940
XI_API int __cdecl fn_1003D9A0(void);  // 0x1003D9A0
XI_API int __cdecl fn_1003DB60(void);  // 0x1003DB60
XI_API char ** __cdecl CMoElem_GetRuntimeClass_variant1(void);  // 0x1003E690
XI_API char ** __cdecl CMoD3aElem_GetRuntimeClass_variant0(void);  // 0x1003E6A0
XI_API char ** __cdecl CMoD3aElem_GetRuntimeClass_variant1(void);  // 0x1003EE50
XI_API char ** __cdecl CMoD3aLightmapElem_GetRuntimeClass(void);  // 0x1003EE60
XI_API char ** __cdecl CMoD3aElem_GetRuntimeClass_variant2(void);  // 0x1003EF00
XI_API char ** __cdecl CMoVuAnmElem_GetRuntimeClass(void);  // 0x1003EF10
XI_API BOOL __stdcall CMoVuAnmElem_vf012(int a1);  // 0x1003EF80
XI_API void __cdecl CMoResource_VT_1032C6F4_vf009(void);  // 0x1003F430
XI_API char ** __cdecl CMoElem_GetRuntimeClass_variant2(void);  // 0x10041000
XI_API char ** __cdecl CMoD3mSpecularElem_GetRuntimeClass(void);  // 0x10041010
XI_API BOOL __stdcall CMoD3mSpecularElem_vf012(int a1);  // 0x100410E0
XI_API BOOL __stdcall CMoElemFamily_IsKind0(int a1);  // 0x10041120
XI_API void __cdecl fn_10041C20(int a1, int a2, int a3, int a4, int a5);  // 0x10041C20
XI_API char __cdecl CMoTask_vf008(void);  // 0x10042640
XI_API void __cdecl CMoOtTask_vf011(void);  // 0x10042650
XI_API char ** __cdecl CMoElem_GetRuntimeClass_variant3(void);  // 0x100426A0
XI_API char ** __cdecl CMoDisintegrateProgElem_GetRuntimeClass(void);  // 0x100426B0
XI_API void __cdecl CMoDisintegrateProgElem_vf018(void);  // 0x100426C0
XI_API char ** __cdecl CMoElem_GetRuntimeClass_variant4(void);  // 0x100426D0
XI_API char ** __cdecl CMoDist_GetRuntimeClass_variant0(void);  // 0x100426E0
XI_API char ** __cdecl CMoDist_GetRuntimeClass_variant1(void);  // 0x100430B0
XI_API char ** __cdecl CMoVuDist_GetRuntimeClass(void);  // 0x100430C0
XI_API char ** __cdecl CMoElem_GetRuntimeClass_variant5(void);  // 0x10043190
XI_API char ** __cdecl CMoDistModelElem_GetRuntimeClass(void);  // 0x100431A0
XI_API char ** __cdecl CMoElem_GetRuntimeClass_variant6(void);  // 0x10043570
XI_API char ** __cdecl CMoDistMorphElem_GetRuntimeClass(void);  // 0x10043580
XI_API char ** __cdecl CMoElem_GetRuntimeClass_variant7(void);  // 0x10043700
XI_API char ** __cdecl CMoDistRingElem_GetRuntimeClass(void);  // 0x10043710
XI_API _DWORD * __cdecl fn_10044270(void);  // 0x10044270
XI_API char ** __cdecl CMoOtTask_GetRuntimeClass_variant0(void);  // 0x10044870
XI_API char ** __cdecl CMoElem_GetRuntimeClass_variant8(void);  // 0x10044880
XI_API char __thiscall CMoElemFamily_SharedCleanup(int *self_);  // 0x10044970
XI_API void __cdecl CMoElem_vf018(void);  // 0x100472E0
XI_API char ** __cdecl CMoElem_GetRuntimeClass_variant9(void);  // 0x10047550
XI_API char ** __cdecl CMoNullProgElem_GetRuntimeClass(void);  // 0x10047560
XI_API char ** __cdecl CMoElem_GetRuntimeClass_variant10(void);  // 0x10047570
XI_API char ** __cdecl CMoSkeletonElem_GetRuntimeClass(void);  // 0x10047580
XI_API void __stdcall CMoResource_vf005(int a1);  // 0x10047FD0
XI_API BOOL __stdcall CMoVuLineProgElem_vf012(int a1);  // 0x10052BF0
XI_API BOOL __stdcall CMoDist_vf012(int a1);  // 0x10052D60
XI_API BOOL __stdcall CMoVuDist_vf012(int a1);  // 0x10052DD0
XI_API BOOL __stdcall CMoDistMorphElem_vf012(int a1);  // 0x10052E40
XI_API BOOL __stdcall CMoDistModelElem_vf012(int a1);  // 0x10052F40
XI_API BOOL __stdcall CMoDistRingElem_vf012(int a1);  // 0x10052FF0
XI_API BOOL __stdcall CMoLfdElem_vf012(int a1);  // 0x10053050
XI_API BOOL __stdcall CMoDisintegrateProgElem_vf012(int a1);  // 0x10053130
XI_API char ** __cdecl CMoGeneratorClone_GetRuntimeClass(void);  // 0x100542E0
XI_API void __cdecl CMoResource_VT_1032C5FC_vf008(void);  // 0x10054480
XI_API void __cdecl CMoResource_VT_1032C5FC_vf009(void);  // 0x10054490
XI_API char ** __cdecl CMoElem_GetRuntimeClass_variant11(void);  // 0x100548A0
XI_API char ** __cdecl CMoLfdElem_GetRuntimeClass(void);  // 0x100548B0
XI_API char ** __cdecl CMoOT_GetRuntimeClass(void);  // 0x10055290
XI_API char ** __cdecl CMoTask_GetRuntimeClass_variant5(void);  // 0x10055550
XI_API char ** __cdecl CMoOtTask_GetRuntimeClass_variant1(void);  // 0x10055560
XI_API char ** __cdecl CMoPathObject_GetRuntimeClass(void);  // 0x10055A50
XI_API char ** __cdecl CMoResource_GetRuntimeClass_variant1(void);  // 0x10056250
XI_API char ** __cdecl CMoRabRes_GetRuntimeClass(void);  // 0x10056260
XI_API char ** __cdecl CMoResource_GetRuntimeClass_variant2(void);  // 0x10056290
XI_API char ** __cdecl CMoSphRes_GetRuntimeClass(void);  // 0x100562A0
XI_API void __cdecl CMoSphRes_vf009(void);  // 0x100562C0
XI_API char ** __cdecl CMoElem_GetRuntimeClass_variant12(void);  // 0x100562D0
XI_API char ** __cdecl CMoPointLightProgElem_GetRuntimeClass(void);  // 0x100562E0
XI_API BOOL __stdcall CMoPointLightProgElem_vf012(int a1);  // 0x10056360
XI_API char ** __cdecl CMoTask_GetRuntimeClass_variant6(void);  // 0x10056930
XI_API char ** __cdecl CMoPathDriveActorTask_GetRuntimeClass(void);  // 0x10056940
XI_API char ** __cdecl CMoTask_GetRuntimeClass_variant7(void);  // 0x10056950
XI_API char ** __cdecl CMoLockColorDriveTask_GetRuntimeClass(void);  // 0x10056960
XI_API char ** __cdecl CMoTask_GetRuntimeClass_variant8(void);  // 0x10056970
XI_API char ** __cdecl CMoLockLookAtDriveTask_GetRuntimeClass(void);  // 0x10056980
XI_API char ** __cdecl CMoTask_GetRuntimeClass_variant9(void);  // 0x10056990
XI_API char ** __cdecl CMoActorColorDriveTask_GetRuntimeClass(void);  // 0x100569A0
XI_API char ** __cdecl CMoTask_GetRuntimeClass_variant10(void);  // 0x100569B0
XI_API char ** __cdecl CMoActorRotationDriveTask_GetRuntimeClass(void);  // 0x100569C0
XI_API char ** __cdecl CMoTask_GetRuntimeClass_variant11(void);  // 0x10057080
XI_API char ** __cdecl CMoSchedularTask_GetRuntimeClass(void);  // 0x10057090
XI_API char ** __cdecl CMoSpline_GetRuntimeClass(void);  // 0x10063450
XI_API char ** __cdecl CMoElem_GetRuntimeClass_variant13(void);  // 0x10064160
XI_API char ** __cdecl CMoVuLineProgElem_GetRuntimeClass(void);  // 0x10064170
XI_API void __cdecl CMoVuLineProgElem_vf018(void);  // 0x10064180
XI_API char ** __cdecl CMoDX_GetRuntimeClass(void);  // 0x10069D60
XI_API char ** __cdecl CMoDxStatusMng_GetRuntimeClass(void);  // 0x1006B040
XI_API _DWORD * __thiscall fn_1006B2B0(void *self_);  // 0x1006B2B0
XI_API char ** __cdecl CMoOcclusionMng_GetRuntimeClass(void);  // 0x1006C130
XI_API _DWORD * __thiscall OcclusionMng_CreateTarget(_DWORD *self_);  // 0x1006C220
XI_API int __thiscall Occlusion_QueryScreenRect(__int16 *self_, float *a2, int a3, float a4, float a5, int a6, int a7, int a8, char a9, int a10);  // 0x1006C280
XI_API int __thiscall OcclusionMng_Prepare(_DWORD *self_);  // 0x1006C9B0
XI_API char ** __cdecl CMoProcessor_GetRuntimeClass(void);  // 0x1006CCD0
XI_API float * __stdcall CMoProcessor_VT_1032C298_vf007(float *a1, float a2);  // 0x1006D380
XI_API float * __stdcall CMoProcessor_VT_1032C368_vf012(float *a1, float *a2, float a3);  // 0x1006D3B0
XI_API double __stdcall CMoProcessor_VT_1032C368_vf014(float *a1, float *a2);  // 0x1006D4D0
XI_API float * __stdcall CMoProcessor_VT_1032C368_vf042(float *a1, float *a2, float *a3);  // 0x1006D680
XI_API float * __stdcall CMoProcessor_VT_1032C368_vf043(float *a1, float *a2, float *a3);  // 0x1006D6B0
XI_API float * __stdcall CMoProcessor_VT_1032C368_vf021(float *a1, float *a2, float *a3, float a4);  // 0x1006D980
XI_API double __stdcall CMoProcessor_VT_1032C298_vf014(float *a1, float *a2);  // 0x1006F5B0
XI_API float * __stdcall CMoProcessor_VT_1032C298_vf021(float *a1, float *a2, float *a3, float a4);  // 0x1006FA30
XI_API CMoProcessor * __stdcall CMoProcessor_VT_1032C298_vf036(float *a1, float *a2, int a3, int a4, const void *a5);  // 0x10070040
XI_API float * __stdcall CMoProcessor_VT_1032C1C8_vf012(float *a1, float *a2, float a3);  // 0x10070780
XI_API double __stdcall CMoProcessor_VT_1032C1C8_vf014(float *a1, float *a2);  // 0x10070790
XI_API void __stdcall CMoProcessor_VT_1032C1C8_vf041(float *a1);  // 0x10070860
XI_API float * __stdcall CMoProcessor_VT_1032C1C8_vf018(float *a1, float *a2, float *a3);  // 0x10070AA0
XI_API float * __stdcall CMoProcessor_VT_1032C1C8_vf017(float *a1, float *a2, float *a3);  // 0x10070AB0
XI_API float * __stdcall CMoProcessor_VT_1032C1C8_vf016(float *a1, float *a2, float *a3);  // 0x10070AC0
XI_API float * __stdcall CMoProcessor_VT_1032C1C8_vf021(float *a1, float *a2, float *a3, float a4);  // 0x10070AD0
XI_API void __stdcall CMoProcessor_VT_1032C1C8_vf032(float *a1, int a2, int a3, float *a4, float *a5, float a6, float a7);  // 0x10070D80
XI_API float * __stdcall CMoProcessor_VT_1032C1C8_vf033(float *a1, float *a2, int a3, float *a4, float *a5, float a6, float a7);  // 0x10070E20
XI_API CMoProcessor * __stdcall CMoProcessor_VT_1032C1C8_vf036(float *a1, float *a2, int a3, int a4, const void *a5);  // 0x10070E60
XI_API void * __cdecl CMoCamera_GetSharedInstance(void);  // 0x10071980
XI_API void __cdecl CMoResource_VT_1032C7D0_vf008(void);  // 0x100719C0
XI_API void __cdecl CMoResource_VT_1032C7D0_vf009(void);  // 0x100719D0
XI_API void __cdecl CMoResource_VT_1032C828_vf009(void);  // 0x100719E0
XI_API void __cdecl CMoCamera_vf009(void);  // 0x100719F0
XI_API void __cdecl CMoResource_VT_1032C7FC_vf009(void);  // 0x10071A00
XI_API void __cdecl CMoResource_VT_1032C7A4_vf009(void);  // 0x10071A10
XI_API char ** __cdecl CMoResource_GetRuntimeClass_variant3(void);  // 0x10071D70
XI_API void __cdecl CMoResource_vf008(void);  // 0x10071DB0
XI_API char ** __cdecl CMoResourceMng_GetRuntimeClass(void);  // 0x10072D00
XI_API void __cdecl CMoResource_VT_1032C588_vf008(void);  // 0x10074610
XI_API char ** __cdecl CMoTask_GetRuntimeClass_variant12(void);  // 0x10074730
XI_API char ** __cdecl CMoTaskMng_GetRuntimeClass(void);  // 0x10074B50
XI_API void __thiscall fn_100755A0(int *self_);  // 0x100755A0
XI_API char ** __cdecl CMoTask_GetRuntimeClass_variant13(void);  // 0x10081710
XI_API char ** __cdecl CMoTask_GetRuntimeClass_variant14(void);  // 0x10081730
XI_API char ** __cdecl CMoTask_GetRuntimeClass_variant15(void);  // 0x10081750
XI_API int __cdecl CXiActorNameDraw_SetRenderState(void);  // 0x100817F0
XI_API int __stdcall CXiSkeletonActor_GetEquip(int a1);  // 0x100820D0
XI_API char ** __cdecl CMoTask_GetRuntimeClass_variant16(void);  // 0x10084E50
XI_API char __cdecl CXiSkeletonActor_vf257(void);  // 0x100A4820
XI_API char ** __cdecl CMoTask_GetRuntimeClass_variant17(void);  // 0x100A8E10
XI_API char ** __cdecl CXiSkeletonActor_GetRuntimeClass_variant0(void);  // 0x100AB070
XI_API char ** __cdecl CXiSkeletonActor_GetRuntimeClass_variant1(void);  // 0x100C5190
XI_API char ** __cdecl CMoTask_GetRuntimeClass_variant18(void);  // 0x100C51A0
XI_API char ** __cdecl CXiSkeletonActorRes_GetRuntimeClass(void);  // 0x100C51B0
XI_API int __thiscall PerActor_FourBucketStaggeredUpdate(void *self_);  // 0x100CBDE0
XI_API void __stdcall CXiSkeletonActor_ReplaceAttack(int a1, int a2, int a3, int a4);  // 0x100CEDF0
XI_API char __stdcall CXiSkeletonActor_ProjectToScreen(int a1, int a2, int a3);  // 0x100D0740
XI_API char __cdecl CXiSkeletonActor_OnSkeletonLoaded(int a1, int a2);  // 0x100D2F40
XI_API void __cdecl CXiSkeletonActor_vf033(void);  // 0x100D6F90
XI_API void __cdecl CXiSkeletonActor_vf034(void);  // 0x100D6FA0
XI_API void __stdcall CXiSkeletonActor_vf035(int a1, int a2);  // 0x100D6FB0
XI_API void __stdcall CXiSkeletonActor_vf036(int a1);  // 0x100D6FC0
XI_API void __cdecl CXiSkeletonActor_vf037(void);  // 0x100D6FD0
XI_API char ** __cdecl CXiSkeletonActor_GetRuntimeClass_variant2(void);  // 0x100D7140
XI_API int __cdecl fn_1011AAC0(void);  // 0x1011AAC0
XI_API char __thiscall fn_1011BAE0(int self_);  // 0x1011BAE0
XI_API int __thiscall fn_1011BBD0(float *self_, __int16 a2, int *a3, unsigned int a4, float a5, float a6);  // 0x1011BBD0
XI_API int __cdecl fn_1011BDC0(void);  // 0x1011BDC0
XI_API int __thiscall fn_1011C080(void *self_, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);  // 0x1011C080
XI_API int __cdecl fn_1011C1F0(int a1, int a2, int a3, int a4);  // 0x1011C1F0
XI_API void __thiscall fn_1011C720(int *self_);  // 0x1011C720
XI_API int __thiscall fn_1011E800(void *self_, int a1, int a2);  // 0x1011E800
XI_API void __thiscall fn_1011EDE0(int self_, int a2, int a3, float a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16, int a17, int a18, int a19, unsigned int a20, int a21);  // 0x1011EDE0
XI_API int __thiscall fn_1011EF60(int self_, int a2, int a3, int a4, int a5, int a6, int a7, float a8, float a9, unsigned int a10, unsigned int a11, char a12);  // 0x1011EF60
XI_API int __thiscall fn_1011F300(void *self_, int a1, int a2, int a3, int a4, int a5);  // 0x1011F300
XI_API char __thiscall fn_10120590(int *self_, unsigned int a2, _DWORD *a3, _DWORD *a4);  // 0x10120590
XI_API char __thiscall fn_101207B0(int *self_, unsigned int a2, _DWORD *a3, _DWORD *a4);  // 0x101207B0
XI_API int __thiscall fn_10124440(int self_);  // 0x10124440
XI_API void __thiscall fn_10124700(int self_, _DWORD *a2, float a3);  // 0x10124700
XI_API void __thiscall fn_10124EB0(int self_);  // 0x10124EB0
XI_API unsigned __int8 __thiscall fn_101252F0(unsigned __int8 *self_, unsigned __int8 a2);  // 0x101252F0
XI_API int __cdecl fn_101604B0(void);  // 0x101604B0
XI_API int __cdecl fn_10160650(void);  // 0x10160650
XI_API int __cdecl fn_101620B0(void);  // 0x101620B0
XI_API int __thiscall fn_101630E0(void *self_);  // 0x101630E0
XI_API int __stdcall fn_10163400(int a1, int a2);  // 0x10163400
XI_API int __stdcall fn_1016DDF0(int a1, int a2, int a3);  // 0x1016DDF0
XI_API void __thiscall fn_10174B10(_DWORD *self_, float *a2);  // 0x10174B10
XI_API int __cdecl fn_10174EC0(void);  // 0x10174EC0
XI_API int __thiscall fn_10177240(void *self_, int a1, int a2);  // 0x10177240
XI_API char ** __cdecl CMoTask_GetRuntimeClass_variant19(void);  // 0x101773B0
XI_API int __cdecl fn_10177400(unsigned int a1);  // 0x10177400
XI_API float __thiscall fn_10177420(float *self_);  // 0x10177420
XI_API int __cdecl fn_101779C0(void);  // 0x101779C0
XI_API int __cdecl fn_10177B40(float *a3, char a4, unsigned __int8 a5, unsigned __int8 a6, char a7, char a8);  // 0x10177B40
XI_API int __cdecl fn_10177C50(int a1, int a2, int a3, int a4, int a5, int a6);  // 0x10177C50
XI_API int __stdcall fn_10177CD0(float *a1, int a2, float a3);  // 0x10177CD0
XI_API char __thiscall fn_10178070(int self_);  // 0x10178070
XI_API int __thiscall fn_101781D0(_DWORD *self_);  // 0x101781D0
XI_API int __thiscall fn_10178B00(unsigned int *self_);  // 0x10178B00
XI_API int __thiscall fn_101791F0(void *self_);  // 0x101791F0
XI_API int __thiscall fn_101792A0(char *self_);  // 0x101792A0
XI_API int __thiscall fn_1017ADE0(_DWORD *self_, _DWORD *a2);  // 0x1017ADE0
XI_API float * __thiscall fn_10182700(int self_, float *a2, int a3, IDirect3DBaseTexture8 *a4, float a5);  // 0x10182700
XI_API int __stdcall fn_10182F10(int a1);  // 0x10182F10
XI_API void __thiscall ModelActor_Draw(int *self_, int a2, int a3);  // 0x101844C0
XI_API int __thiscall fn_10184FE0(void *self_, int a1, int a2);  // 0x10184FE0
XI_API _DWORD * __thiscall Env_ComputeAmbient(char *self_, _DWORD *a2, int a3);  // 0x10188E00
XI_API float * __thiscall fn_10188FA0(char *self_, float *a2);  // 0x10188FA0
XI_API void __cdecl CMoResource_VT_103367E8_vf008(void);  // 0x101893F0
XI_API void __cdecl fn_101C8DA0(_DWORD *a1, _WORD *a2, _WORD *a3);  // 0x101C8DA0
XI_API int __cdecl Draw2DLayer(void);  // 0x1021D4C0
XI_API int __thiscall fn_1021E060(int self_, int a2);  // 0x1021E060
XI_API char * __thiscall fn_102394A0(char **self_);  // 0x102394A0
XI_API void __cdecl fn_1024F900(int a1);  // 0x1024F900
XI_API int __cdecl fn_102504D0(int *a1);  // 0x102504D0
XI_API char __thiscall Dancer_InitDevice(int *self_);  // 0x10251A80
XI_API int __thiscall fn_10253F60(void *self_, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);  // 0x10253F60
XI_API IDirect3DDevice8 * __cdecl fn_1025D750(void);  // 0x1025D750
XI_API int __cdecl fn_1025D760(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, D3DFORMAT a5, D3DPOOL a6);  // 0x1025D760
XI_API int __cdecl fn_1025D7B0(unsigned int a1, unsigned int a2, unsigned int a3, D3DPOOL a4);  // 0x1025D7B0
XI_API int __cdecl fn_102816E0(int a1, int a2, int a3);  // 0x102816E0
XI_API int __cdecl fn_10281830(int a1, int a2, IDirect3DBaseTexture8 *a3, _BYTE *a4);  // 0x10281830
XI_API int __cdecl fn_102819D0(int a1, int a2, int a3);  // 0x102819D0
XI_API int __cdecl fn_10281D60(int a1);  // 0x10281D60
XI_API int __cdecl fn_10282010(int a1);  // 0x10282010
XI_API int __cdecl fn_10282030(float *a1, int a2);  // 0x10282030
XI_API int __cdecl fn_102820E0(IDirect3DSurface8 *a1);  // 0x102820E0
XI_API int __cdecl fn_10282190(int a1, char a2);  // 0x10282190
XI_API int __cdecl fn_10282350(int a1);  // 0x10282350
XI_API int __cdecl fn_102823F0(unsigned int a1, unsigned int a2);  // 0x102823F0
XI_API int __cdecl fn_10282470(int a1);  // 0x10282470
XI_API int __cdecl fn_102824C0(int a1, int a2, int a3, int a4);  // 0x102824C0
XI_API int __cdecl fn_10282640(void);  // 0x10282640
XI_API int __cdecl fn_102826C0(void);  // 0x102826C0
XI_API int __cdecl fn_102827D0(void);  // 0x102827D0
XI_API int __cdecl fn_10282910(void);  // 0x10282910
XI_API int __cdecl fn_10282B90(void);  // 0x10282B90
XI_API int __cdecl fn_10282CB0(void);  // 0x10282CB0
XI_API int __cdecl fn_10282DC0(void);  // 0x10282DC0
XI_API int __cdecl fn_10282ED0(void);  // 0x10282ED0
XI_API int __cdecl fn_10282FF0(void);  // 0x10282FF0
XI_API int __cdecl fn_10283110(void);  // 0x10283110
XI_API int __cdecl fn_10283220(void);  // 0x10283220
XI_API int __cdecl fn_10283A10(float a1, float a2, float a3, float a4);  // 0x10283A10
XI_API int __cdecl fn_10283F70(int a1);  // 0x10283F70
XI_API int __cdecl fn_10283FC0(int a1);  // 0x10283FC0
XI_API int __cdecl fn_10284410(void);  // 0x10284410
XI_API int __cdecl fn_10284A80(void);  // 0x10284A80
XI_API _DWORD * __cdecl sqgxContext_SelectShaderMode(int a1, int a2);  // 0x10284B20
XI_API int __cdecl fn_10284EF0(void);  // 0x10284EF0
XI_API _DWORD * __cdecl fn_1028AA70(void);  // 0x1028AA70
XI_API void __cdecl fn_1028F8C0(int a1);  // 0x1028F8C0
XI_API int __cdecl fn_10297CC0(int a1);  // 0x10297CC0
XI_API int __cdecl fn_10297EC0(int a1);  // 0x10297EC0
XI_API int __cdecl fn_102981E0(void);  // 0x102981E0
XI_API void __cdecl fn_102A21F0(int a1, int a2, int a3);  // 0x102A21F0
XI_API void __cdecl fn_102A2530(int a1, int a2);  // 0x102A2530
