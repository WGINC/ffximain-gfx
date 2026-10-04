// gfx_globals.hpp -- game globals the graphics code uses, bound to their addresses.
#pragma once
#include "gfx_common.hpp"
typedef void (__stdcall *Sleep_t)(DWORD dwMilliseconds);
#define Sleep XI_EA(Sleep_t, 0x103290F8u)
typedef DWORD (__stdcall *GetCurrentDirectoryA_t)(DWORD nBufferLength, LPSTR lpBuffer);
#define GetCurrentDirectoryA XI_EA(GetCurrentDirectoryA_t, 0x10329118u)
typedef void (__stdcall *GetLocalTime_t)(LPSYSTEMTIME lpSystemTime);
#define GetLocalTime XI_EA(GetLocalTime_t, 0x1032919Cu)
typedef HCURSOR (__stdcall *LoadCursorFromFileA_t)(LPCSTR lpFileName);
#define LoadCursorFromFileA XI_EA(LoadCursorFromFileA_t, 0x10329378u)
typedef HCURSOR (__stdcall *LoadCursorA_t)(HINSTANCE hInstance, LPCSTR lpCursorName);
#define LoadCursorA XI_EA(LoadCursorA_t, 0x1032937Cu)
typedef HCURSOR (__stdcall *SetCursor_t)(HCURSOR hCursor);
#define SetCursor XI_EA(SetCursor_t, 0x10329380u)
typedef int (__stdcall *GetSystemMetrics_t)(int nIndex);
#define GetSystemMetrics XI_EA(GetSystemMetrics_t, 0x103293A0u)
typedef LRESULT (__stdcall *DispatchMessageA_t)(const MSG *lpMsg);
#define DispatchMessageA XI_EA(DispatchMessageA_t, 0x103293C8u)
typedef BOOL (__stdcall *GetWindowRect_t)(HWND hWnd, LPRECT lpRect);
#define GetWindowRect XI_EA(GetWindowRect_t, 0x103293E4u)
typedef BOOL (__stdcall *PeekMessageA_t)(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
#define PeekMessageA XI_EA(PeekMessageA_t, 0x103293E8u)
typedef BOOL (__stdcall *TranslateMessage_t)(const MSG *lpMsg);
#define TranslateMessage XI_EA(TranslateMessage_t, 0x103293ECu)
typedef _UNKNOWN * CDx_classdesc_t;
#define CDx_classdesc XI_EA(CDx_classdesc_t, 0x103299C8u)
typedef int (*CDx_vtbl_t)();
#define CDx_vtbl XI_EA(CDx_vtbl_t, 0x103299D4u)
typedef int (*CYyObject_vtbl_ambiguous0_t)();
#define CYyObject_vtbl_ambiguous0 XI_EA(CYyObject_vtbl_ambiguous0_t, 0x103299F0u)
typedef int (*CXiActorNameDraw_vtbl_t)();
#define CXiActorNameDraw_vtbl XI_EA(CXiActorNameDraw_vtbl_t, 0x10329C18u)
typedef int (*CXiActorDraw_vtbl_t)();
#define CXiActorDraw_vtbl XI_EA(CXiActorDraw_vtbl_t, 0x10329C44u)
typedef int (*CMoTask_vtbl_ambiguous0_t)();
#define CMoTask_vtbl_ambiguous0 XI_EA(CMoTask_vtbl_ambiguous0_t, 0x10329C70u)
typedef int (*CMoResourceMng_vtbl_t)();
#define CMoResourceMng_vtbl XI_EA(CMoResourceMng_vtbl_t, 0x10329C9Cu)
typedef int (*CMoTaskMng_vtbl_t)();
#define CMoTaskMng_vtbl XI_EA(CMoTaskMng_vtbl_t, 0x10329CB8u)
typedef char * CShader_classdesc_t;
#define CShader_classdesc XI_EA(CShader_classdesc_t, 0x10329EC4u)
typedef int (*CShader_vtbl_t)();
#define CShader_vtbl XI_EA(CShader_vtbl_t, 0x10329ED0u)
typedef int (*CXiOpening_vtbl_t)();
#define CXiOpening_vtbl XI_EA(CXiOpening_vtbl_t, 0x10329FBCu)
typedef int (__stdcall *off_1032A460_t)(int a1);
#define off_1032A460 XI_EA(off_1032A460_t, 0x1032A460u)
typedef char * CYyModelDt_classdesc_t;
#define CYyModelDt_classdesc XI_EA(CYyModelDt_classdesc_t, 0x1032A6B8u)
typedef char * CYyModelBase_classdesc_t;
#define CYyModelBase_classdesc XI_EA(CYyModelBase_classdesc_t, 0x1032A6C4u)
typedef char * CYyModel_classdesc_t;
#define CYyModel_classdesc XI_EA(CYyModel_classdesc_t, 0x1032A6D0u)
typedef int (*CYyOs2VtxBuffer_vtbl_t)();
#define CYyOs2VtxBuffer_vtbl XI_EA(CYyOs2VtxBuffer_vtbl_t, 0x1032A7B0u)
typedef int (__stdcall *off_1032A844_t)(int a1);
#define off_1032A844 XI_EA(off_1032A844_t, 0x1032A844u)
typedef char * CYyTexBase_classdesc_t;
#define CYyTexBase_classdesc XI_EA(CYyTexBase_classdesc_t, 0x1032AA34u)
typedef _UNKNOWN * CYyTex_classdesc_t;
#define CYyTex_classdesc XI_EA(CYyTex_classdesc_t, 0x1032AA40u)
typedef char * CYyTexMng_classdesc_t;
#define CYyTexMng_classdesc XI_EA(CYyTexMng_classdesc_t, 0x1032AA84u)
typedef char * CMoAttachments_classdesc_t;
#define CMoAttachments_classdesc XI_EA(CMoAttachments_classdesc_t, 0x1032AAFCu)
typedef char * CMoCamera_classdesc_t;
#define CMoCamera_classdesc XI_EA(CMoCamera_classdesc_t, 0x1032AB28u)
typedef char * CMoCameraTask_classdesc_t;
#define CMoCameraTask_classdesc XI_EA(CMoCameraTask_classdesc_t, 0x1032AB34u)
typedef char * CMoCelestial_classdesc_t;
#define CMoCelestial_classdesc XI_EA(CMoCelestial_classdesc_t, 0x1032AB9Cu)
typedef char * CMoD3aElem_classdesc_t;
#define CMoD3aElem_classdesc XI_EA(CMoD3aElem_classdesc_t, 0x1032ABECu)
typedef char * CMoD3aLightmapElem_classdesc_t;
#define CMoD3aLightmapElem_classdesc XI_EA(CMoD3aLightmapElem_classdesc_t, 0x1032ABF8u)
typedef char * CMoVuAnmElem_classdesc_t;
#define CMoVuAnmElem_classdesc XI_EA(CMoVuAnmElem_classdesc_t, 0x1032AC04u)
typedef char * CMoD3mSpecularElem_classdesc_t;
#define CMoD3mSpecularElem_classdesc XI_EA(CMoD3mSpecularElem_classdesc_t, 0x1032AD08u)
typedef int (*CMoD3mSpecularElem_vtbl_t)();
#define CMoD3mSpecularElem_vtbl XI_EA(CMoD3mSpecularElem_vtbl_t, 0x1032AD20u)
typedef int (*CMoOtTask_vtbl_ambiguous1_t)();
#define CMoOtTask_vtbl_ambiguous1 XI_EA(CMoOtTask_vtbl_ambiguous1_t, 0x1032AEB0u)
typedef char * CMoDisintegrateProgElem_classdesc_t;
#define CMoDisintegrateProgElem_classdesc XI_EA(CMoDisintegrateProgElem_classdesc_t, 0x1032AEE0u)
typedef char * CMoDist_classdesc_t;
#define CMoDist_classdesc XI_EA(CMoDist_classdesc_t, 0x1032AEECu)
typedef char * CMoVuDist_classdesc_t;
#define CMoVuDist_classdesc XI_EA(CMoVuDist_classdesc_t, 0x1032AEF8u)
typedef char * CMoDistModelElem_classdesc_t;
#define CMoDistModelElem_classdesc XI_EA(CMoDistModelElem_classdesc_t, 0x1032AF20u)
typedef char * CMoDistMorphElem_classdesc_t;
#define CMoDistMorphElem_classdesc XI_EA(CMoDistMorphElem_classdesc_t, 0x1032AF2Cu)
typedef char * CMoDistRingElem_classdesc_t;
#define CMoDistRingElem_classdesc XI_EA(CMoDistRingElem_classdesc_t, 0x1032AF38u)
typedef int (*CYyObject_vtbl_ambiguous2_t)();
#define CYyObject_vtbl_ambiguous2 XI_EA(CYyObject_vtbl_ambiguous2_t, 0x1032AFD8u)
typedef char * CMoElem_classdesc_t;
#define CMoElem_classdesc XI_EA(CMoElem_classdesc_t, 0x1032AFF0u)
typedef char * CMoNullProgElem_classdesc_t;
#define CMoNullProgElem_classdesc XI_EA(CMoNullProgElem_classdesc_t, 0x1032AFFCu)
typedef char * CMoSkeletonElem_classdesc_t;
#define CMoSkeletonElem_classdesc XI_EA(CMoSkeletonElem_classdesc_t, 0x1032B008u)
typedef int (*CMoElem_vtbl_ambiguous3_t)();
#define CMoElem_vtbl_ambiguous3 XI_EA(CMoElem_vtbl_ambiguous3_t, 0x1032B018u)
typedef char * CMoGeneratorClone_classdesc_t;
#define CMoGeneratorClone_classdesc XI_EA(CMoGeneratorClone_classdesc_t, 0x1032B108u)
typedef int (*mixin_of_CMoGeneratorClone_0x1032B680_t)();
#define mixin_of_CMoGeneratorClone_0x1032B680 XI_EA(mixin_of_CMoGeneratorClone_0x1032B680_t, 0x1032B680u)
typedef int (*CMoGeneratorClone_vtbl_t)();
#define CMoGeneratorClone_vtbl XI_EA(CMoGeneratorClone_vtbl_t, 0x1032B69Cu)
typedef char * CMoLfdElem_classdesc_t;
#define CMoLfdElem_classdesc XI_EA(CMoLfdElem_classdesc_t, 0x1032B6C8u)
typedef char * CMoOT_classdesc_t;
#define CMoOT_classdesc XI_EA(CMoOT_classdesc_t, 0x1032B6DCu)
typedef char * CMoOtTask_classdesc_t;
#define CMoOtTask_classdesc XI_EA(CMoOtTask_classdesc_t, 0x1032B6E8u)
typedef char * CMoPathObject_classdesc_t;
#define CMoPathObject_classdesc XI_EA(CMoPathObject_classdesc_t, 0x1032B710u)
typedef char * CMoRabRes_classdesc_t;
#define CMoRabRes_classdesc XI_EA(CMoRabRes_classdesc_t, 0x1032B71Cu)
typedef char * CMoSphRes_classdesc_t;
#define CMoSphRes_classdesc XI_EA(CMoSphRes_classdesc_t, 0x1032B728u)
typedef char * CMoPointLightProgElem_classdesc_t;
#define CMoPointLightProgElem_classdesc XI_EA(CMoPointLightProgElem_classdesc_t, 0x1032B74Cu)
typedef char * CMoPathDriveActorTask_classdesc_t;
#define CMoPathDriveActorTask_classdesc XI_EA(CMoPathDriveActorTask_classdesc_t, 0x1032B7BCu)
typedef char * CMoLockColorDriveTask_classdesc_t;
#define CMoLockColorDriveTask_classdesc XI_EA(CMoLockColorDriveTask_classdesc_t, 0x1032B7C8u)
typedef char * CMoLockLookAtDriveTask_classdesc_t;
#define CMoLockLookAtDriveTask_classdesc XI_EA(CMoLockLookAtDriveTask_classdesc_t, 0x1032B7D4u)
typedef char * CMoActorColorDriveTask_classdesc_t;
#define CMoActorColorDriveTask_classdesc XI_EA(CMoActorColorDriveTask_classdesc_t, 0x1032B7E0u)
typedef char * CMoActorRotationDriveTask_classdesc_t;
#define CMoActorRotationDriveTask_classdesc XI_EA(CMoActorRotationDriveTask_classdesc_t, 0x1032B7ECu)
typedef char * CMoSchedularTask_classdesc_t;
#define CMoSchedularTask_classdesc XI_EA(CMoSchedularTask_classdesc_t, 0x1032B7F8u)
typedef char * CMoSpline_classdesc_t;
#define CMoSpline_classdesc XI_EA(CMoSpline_classdesc_t, 0x1032C074u)
typedef char * CMoVuLineProgElem_classdesc_t;
#define CMoVuLineProgElem_classdesc XI_EA(CMoVuLineProgElem_classdesc_t, 0x1032C0C0u)
typedef char * CMoDX_classdesc_t;
#define CMoDX_classdesc XI_EA(CMoDX_classdesc_t, 0x1032C0CCu)
typedef int (*CMoDX_vtbl_t)();
#define CMoDX_vtbl XI_EA(CMoDX_vtbl_t, 0x1032C0D8u)
typedef char * CMoDxStatusMng_classdesc_t;
#define CMoDxStatusMng_classdesc XI_EA(CMoDxStatusMng_classdesc_t, 0x1032C100u)
typedef char * CMoOcclusionMng_classdesc_t;
#define CMoOcclusionMng_classdesc XI_EA(CMoOcclusionMng_classdesc_t, 0x1032C16Cu)
typedef char * CMoProcessor_classdesc_t;
#define CMoProcessor_classdesc XI_EA(CMoProcessor_classdesc_t, 0x1032C1BCu)
typedef char * CMoResource_classdesc_t;
#define CMoResource_classdesc XI_EA(CMoResource_classdesc_t, 0x1032C46Cu)
typedef char * CMoResourceMng_classdesc_t;
#define CMoResourceMng_classdesc XI_EA(CMoResourceMng_classdesc_t, 0x1032C47Cu)
typedef char * CMoTask_classdesc_t;
#define CMoTask_classdesc XI_EA(CMoTask_classdesc_t, 0x1032C960u)
typedef char * CMoTaskMng_classdesc_t;
#define CMoTaskMng_classdesc XI_EA(CMoTaskMng_classdesc_t, 0x1032C96Cu)
typedef int (*off_1032D36C_t)();
#define off_1032D36C XI_EA(off_1032D36C_t, 0x1032D36Cu)
typedef int (*off_1032D374_t)();
#define off_1032D374 XI_EA(off_1032D374_t, 0x1032D374u)
typedef char * CXiSkeletonActor_classdesc_t;
#define CXiSkeletonActor_classdesc XI_EA(CXiSkeletonActor_classdesc_t, 0x10330EBCu)
typedef char * CXiSkeletonActorRes_classdesc_t;
#define CXiSkeletonActorRes_classdesc XI_EA(CXiSkeletonActorRes_classdesc_t, 0x10330EC8u)
typedef int (*CXiSkeletonActor_vtbl_ambiguous0_t)();
#define CXiSkeletonActor_vtbl_ambiguous0 XI_EA(CXiSkeletonActor_vtbl_ambiguous0_t, 0x10330F40u)
typedef int (*CXiSkeletonActorRes_vtbl_t)();
#define CXiSkeletonActorRes_vtbl XI_EA(CXiSkeletonActorRes_vtbl_t, 0x103313ACu)
typedef int g_hiResFont_fileId_t[];
#define g_hiResFont_fileId XI_EA(g_hiResFont_fileId_t, 0x10333000u)
typedef int g_hiResFont_width_t[];
#define g_hiResFont_width XI_EA(g_hiResFont_width_t, 0x10333004u)
typedef int g_hiResFont_height_t[];
#define g_hiResFont_height XI_EA(g_hiResFont_height_t, 0x10333008u)
typedef int (*CTsZoneMap_vtbl_t)();
#define CTsZoneMap_vtbl XI_EA(CTsZoneMap_vtbl_t, 0x10336748u)
typedef _BYTE GUID_SysMouse_ref_t;
#define GUID_SysMouse_ref XI_EA(GUID_SysMouse_ref_t, 0x10341DBCu)
typedef _BYTE c_dfDIMouse_ref_t;
#define c_dfDIMouse_ref XI_EA(c_dfDIMouse_ref_t, 0x10349EF4u)
typedef char word_1034F2B8_t[];
#define word_1034F2B8 XI_EA(word_1034F2B8_t, 0x1034F2B8u)
typedef _UNKNOWN * off_1034F698_t;
#define off_1034F698 XI_EA(off_1034F698_t, 0x1034F698u)
typedef char aDeviceLost_t[1];
#define aDeviceLost XI_EA(aDeviceLost_t, 0x1034F6A0u)
typedef _BYTE unk_1034F6C0_t;
#define unk_1034F6C0 XI_EA(unk_1034F6C0_t, 0x1034F6C0u)
typedef _BYTE unk_1034F720_t;
#define unk_1034F720 XI_EA(unk_1034F720_t, 0x1034F720u)
typedef int dword_1034F72C_t;
#define dword_1034F72C XI_EA(dword_1034F72C_t, 0x1034F72Cu)
typedef float flt_1034F730_t[4];
#define flt_1034F730 XI_EA(flt_1034F730_t, 0x1034F730u)
typedef float flt_1034F740_t;
#define flt_1034F740 XI_EA(flt_1034F740_t, 0x1034F740u)
typedef float flt_1034F744_t;
#define flt_1034F744 XI_EA(flt_1034F744_t, 0x1034F744u)
typedef float flt_1034F74C_t;
#define flt_1034F74C XI_EA(flt_1034F74C_t, 0x1034F74Cu)
typedef float flt_1034F750_t;
#define flt_1034F750 XI_EA(flt_1034F750_t, 0x1034F750u)
typedef float flt_1034F758_t;
#define flt_1034F758 XI_EA(flt_1034F758_t, 0x1034F758u)
typedef float flt_1034F75C_t;
#define flt_1034F75C XI_EA(flt_1034F75C_t, 0x1034F75Cu)
typedef float flt_1034F764_t;
#define flt_1034F764 XI_EA(flt_1034F764_t, 0x1034F764u)
typedef float flt_1034F768_t;
#define flt_1034F768 XI_EA(flt_1034F768_t, 0x1034F768u)
typedef char aErrorSSD_t[1];
#define aErrorSSD XI_EA(aErrorSSD_t, 0x1034F770u)
typedef char aS_t[1];
#define aS XI_EA(aS_t, 0x1034F7E0u)
typedef __int16 word_1034F8E0_t;
#define word_1034F8E0 XI_EA(word_1034F8E0_t, 0x1034F8E0u)
typedef char g_mouseAcquired_t;
#define g_mouseAcquired XI_EA(g_mouseAcquired_t, 0x1034F92Cu)
typedef char aMousenorAni_t[1];
#define aMousenorAni XI_EA(aMousenorAni_t, 0x1034FBB0u)
typedef float flt_1034FDF0_t;
#define flt_1034FDF0 XI_EA(flt_1034FDF0_t, 0x1034FDF0u)
typedef float flt_1034FDF4_t;
#define flt_1034FDF4 XI_EA(flt_1034FDF4_t, 0x1034FDF4u)
typedef int dword_1034FE00_t;
#define dword_1034FE00 XI_EA(dword_1034FE00_t, 0x1034FE00u)
typedef float flt_1034FE04_t;
#define flt_1034FE04 XI_EA(flt_1034FE04_t, 0x1034FE04u)
typedef float flt_1034FE08_t;
#define flt_1034FE08 XI_EA(flt_1034FE08_t, 0x1034FE08u)
typedef int dword_1034FE14_t;
#define dword_1034FE14 XI_EA(dword_1034FE14_t, 0x1034FE14u)
typedef float flt_1034FE18_t;
#define flt_1034FE18 XI_EA(flt_1034FE18_t, 0x1034FE18u)
typedef float flt_1034FE1C_t;
#define flt_1034FE1C XI_EA(flt_1034FE1C_t, 0x1034FE1Cu)
typedef int dword_1034FE28_t;
#define dword_1034FE28 XI_EA(dword_1034FE28_t, 0x1034FE28u)
typedef float flt_1034FE2C_t;
#define flt_1034FE2C XI_EA(flt_1034FE2C_t, 0x1034FE2Cu)
typedef float flt_1034FE30_t;
#define flt_1034FE30 XI_EA(flt_1034FE30_t, 0x1034FE30u)
typedef int dword_1034FE3C_t;
#define dword_1034FE3C XI_EA(dword_1034FE3C_t, 0x1034FE3Cu)
typedef int g_stereoEyeOffset_t;
#define g_stereoEyeOffset XI_EA(g_stereoEyeOffset_t, 0x1034FFA0u)
typedef char word_1034FFB8_t[];
#define word_1034FFB8 XI_EA(word_1034FFB8_t, 0x1034FFB8u)
typedef int dword_10350174_t;
#define dword_10350174 XI_EA(dword_10350174_t, 0x10350174u)
typedef FILE Stream_t;
#define Stream XI_EA(Stream_t, 0x1035017Cu)
typedef int dword_103501A4_t;
#define dword_103501A4 XI_EA(dword_103501A4_t, 0x103501A4u)
typedef float g_feedbackQuad_t;
#define g_feedbackQuad XI_EA(g_feedbackQuad_t, 0x103501A8u)
typedef float flt_103501AC_t;
#define flt_103501AC XI_EA(flt_103501AC_t, 0x103501ACu)
typedef _BYTE unk_103501B8_t;
#define unk_103501B8 XI_EA(unk_103501B8_t, 0x103501B8u)
typedef float flt_103501BC_t;
#define flt_103501BC XI_EA(flt_103501BC_t, 0x103501BCu)
typedef float flt_103501C0_t;
#define flt_103501C0 XI_EA(flt_103501C0_t, 0x103501C0u)
typedef float flt_103501D0_t;
#define flt_103501D0 XI_EA(flt_103501D0_t, 0x103501D0u)
typedef float flt_103501D4_t;
#define flt_103501D4 XI_EA(flt_103501D4_t, 0x103501D4u)
typedef float flt_103501E4_t;
#define flt_103501E4 XI_EA(flt_103501E4_t, 0x103501E4u)
typedef float flt_103501E8_t;
#define flt_103501E8 XI_EA(flt_103501E8_t, 0x103501E8u)
typedef _DWORD dword_10350208_t[2];
#define dword_10350208 XI_EA(dword_10350208_t, 0x10350208u)
typedef char aMenuChdummy_t[1];
#define aMenuChdummy XI_EA(aMenuChdummy_t, 0x103502E8u)
typedef char aNet3Port54001_t[1];
#define aNet3Port54001 XI_EA(aNet3Port54001_t, 0x103502FCu)
typedef char ClassName_t[1];
#define ClassName XI_EA(ClassName_t, 0x10350350u)
typedef char aFinalFantasyXi_t[1];
#define aFinalFantasyXi XI_EA(aFinalFantasyXi_t, 0x10350450u)
typedef _BYTE unk_1035055C_t;
#define unk_1035055C XI_EA(unk_1035055C_t, 0x1035055Cu)
typedef _BYTE unk_1035055E_t;
#define unk_1035055E XI_EA(unk_1035055E_t, 0x1035055Eu)
typedef float flt_10350748_t;
#define flt_10350748 XI_EA(flt_10350748_t, 0x10350748u)
typedef float flt_1035074C_t;
#define flt_1035074C XI_EA(flt_1035074C_t, 0x1035074Cu)
typedef int dword_10350758_t;
#define dword_10350758 XI_EA(dword_10350758_t, 0x10350758u)
typedef float flt_1035075C_t;
#define flt_1035075C XI_EA(flt_1035075C_t, 0x1035075Cu)
typedef float flt_10350760_t;
#define flt_10350760 XI_EA(flt_10350760_t, 0x10350760u)
typedef int dword_1035076C_t;
#define dword_1035076C XI_EA(dword_1035076C_t, 0x1035076Cu)
typedef char aAfbVersionErro_t[1];
#define aAfbVersionErro XI_EA(aAfbVersionErro_t, 0x103511B0u)
typedef int dword_103511CC_t[5];
#define dword_103511CC XI_EA(dword_103511CC_t, 0x103511CCu)
typedef int dword_103511E0_t[];
#define dword_103511E0 XI_EA(dword_103511E0_t, 0x103511E0u)
typedef char * off_10351514_t;
#define off_10351514 XI_EA(off_10351514_t, 0x10351514u)
typedef _DWORD stru_10351518_t[17];
#define stru_10351518 XI_EA(stru_10351518_t, 0x10351518u)
typedef int c_ps2White_t;
#define c_ps2White XI_EA(c_ps2White_t, 0x103515BCu)
typedef _BYTE g_vsDecl_1628_t;
#define g_vsDecl_1628 XI_EA(g_vsDecl_1628_t, 0x10351628u)
typedef int g_texFourccTable_t[];
#define g_texFourccTable XI_EA(g_texFourccTable_t, 0x10351C94u)
typedef _DWORD Sky_DrawDome_textureStageStates_t[22];
#define Sky_DrawDome_textureStageStates XI_EA(Sky_DrawDome_textureStageStates_t, 0x10351D60u)
typedef int Sky_DrawDome_renderStates_t[16];
#define Sky_DrawDome_renderStates XI_EA(Sky_DrawDome_renderStates_t, 0x10351DB8u)
typedef _DWORD Sky_DrawDomeViewCentered_textureStageStates_t[21];
#define Sky_DrawDomeViewCentered_textureStageStates XI_EA(Sky_DrawDomeViewCentered_textureStageStates_t, 0x10351DF8u)
typedef int Sky_DrawDomeViewCentered_renderStates_t[12];
#define Sky_DrawDomeViewCentered_renderStates XI_EA(Sky_DrawDomeViewCentered_renderStates_t, 0x10351E4Cu)
typedef _DWORD dword_10352258_t[60];
#define dword_10352258 XI_EA(dword_10352258_t, 0x10352258u)
typedef _DWORD dword_103528CC_t[9];
#define dword_103528CC XI_EA(dword_103528CC_t, 0x103528CCu)
typedef float flt_10352CFC_t;
#define flt_10352CFC XI_EA(flt_10352CFC_t, 0x10352CFCu)
typedef float flt_10352D00_t;
#define flt_10352D00 XI_EA(flt_10352D00_t, 0x10352D00u)
typedef float flt_10352D08_t;
#define flt_10352D08 XI_EA(flt_10352D08_t, 0x10352D08u)
typedef float flt_10352D0C_t;
#define flt_10352D0C XI_EA(flt_10352D0C_t, 0x10352D0Cu)
typedef float flt_10352D14_t;
#define flt_10352D14 XI_EA(flt_10352D14_t, 0x10352D14u)
typedef float flt_10352D18_t;
#define flt_10352D18 XI_EA(flt_10352D18_t, 0x10352D18u)
typedef float flt_10352D20_t;
#define flt_10352D20 XI_EA(flt_10352D20_t, 0x10352D20u)
typedef float flt_10352D24_t;
#define flt_10352D24 XI_EA(flt_10352D24_t, 0x10352D24u)
typedef float flt_10352D2C_t;
#define flt_10352D2C XI_EA(flt_10352D2C_t, 0x10352D2Cu)
typedef _DWORD Font_DrawProjectedText3D_textureStageStates_t[64];
#define Font_DrawProjectedText3D_textureStageStates XI_EA(Font_DrawProjectedText3D_textureStageStates_t, 0x10352D30u)
typedef _DWORD dword_10353028_t[42];
#define dword_10353028 XI_EA(dword_10353028_t, 0x10353028u)
typedef int dword_103530D0_t[14];
#define dword_103530D0 XI_EA(dword_103530D0_t, 0x103530D0u)
typedef _DWORD dword_10353108_t[28];
#define dword_10353108 XI_EA(dword_10353108_t, 0x10353108u)
typedef int dword_10353178_t[18];
#define dword_10353178 XI_EA(dword_10353178_t, 0x10353178u)
typedef _DWORD dword_103531C0_t[27];
#define dword_103531C0 XI_EA(dword_103531C0_t, 0x103531C0u)
typedef int dword_1035322C_t[8];
#define dword_1035322C XI_EA(dword_1035322C_t, 0x1035322Cu)
typedef _DWORD CMoDistRingElem_textureStageStates_t[30];
#define CMoDistRingElem_textureStageStates XI_EA(CMoDistRingElem_textureStageStates_t, 0x10353378u)
typedef _DWORD dword_10353598_t[52];
#define dword_10353598 XI_EA(dword_10353598_t, 0x10353598u)
typedef float flt_10354068_t;
#define flt_10354068 XI_EA(flt_10354068_t, 0x10354068u)
typedef float flt_1035406C_t;
#define flt_1035406C XI_EA(flt_1035406C_t, 0x1035406Cu)
typedef float flt_1035407C_t;
#define flt_1035407C XI_EA(flt_1035407C_t, 0x1035407Cu)
typedef float flt_10354080_t;
#define flt_10354080 XI_EA(flt_10354080_t, 0x10354080u)
typedef float flt_10354090_t;
#define flt_10354090 XI_EA(flt_10354090_t, 0x10354090u)
typedef float flt_10354094_t;
#define flt_10354094 XI_EA(flt_10354094_t, 0x10354094u)
typedef float flt_103540A4_t;
#define flt_103540A4 XI_EA(flt_103540A4_t, 0x103540A4u)
typedef float flt_103540A8_t;
#define flt_103540A8 XI_EA(flt_103540A8_t, 0x103540A8u)
typedef float flt_103540B8_t;
#define flt_103540B8 XI_EA(flt_103540B8_t, 0x103540B8u)
typedef float flt_103540BC_t;
#define flt_103540BC XI_EA(flt_103540BC_t, 0x103540BCu)
typedef int dword_103540C0_t;
#define dword_103540C0 XI_EA(dword_103540C0_t, 0x103540C0u)
typedef float flt_103540CC_t;
#define flt_103540CC XI_EA(flt_103540CC_t, 0x103540CCu)
typedef float flt_103540D0_t;
#define flt_103540D0 XI_EA(flt_103540D0_t, 0x103540D0u)
typedef int dword_103540D4_t;
#define dword_103540D4 XI_EA(dword_103540D4_t, 0x103540D4u)
typedef float flt_103540E0_t;
#define flt_103540E0 XI_EA(flt_103540E0_t, 0x103540E0u)
typedef float flt_103540E4_t;
#define flt_103540E4 XI_EA(flt_103540E4_t, 0x103540E4u)
typedef int dword_103540E8_t;
#define dword_103540E8 XI_EA(dword_103540E8_t, 0x103540E8u)
typedef float flt_103540F4_t;
#define flt_103540F4 XI_EA(flt_103540F4_t, 0x103540F4u)
typedef float flt_103540F8_t;
#define flt_103540F8 XI_EA(flt_103540F8_t, 0x103540F8u)
typedef int dword_103540FC_t;
#define dword_103540FC XI_EA(dword_103540FC_t, 0x103540FCu)
typedef _BYTE unk_10354108_t;
#define unk_10354108 XI_EA(unk_10354108_t, 0x10354108u)
typedef float flt_10354118_t;
#define flt_10354118 XI_EA(flt_10354118_t, 0x10354118u)
typedef float flt_1035411C_t;
#define flt_1035411C XI_EA(flt_1035411C_t, 0x1035411Cu)
typedef int dword_10354120_t;
#define dword_10354120 XI_EA(dword_10354120_t, 0x10354120u)
typedef float flt_10354130_t;
#define flt_10354130 XI_EA(flt_10354130_t, 0x10354130u)
typedef float flt_10354134_t;
#define flt_10354134 XI_EA(flt_10354134_t, 0x10354134u)
typedef int dword_1035413C_t;
#define dword_1035413C XI_EA(dword_1035413C_t, 0x1035413Cu)
typedef float flt_10354148_t;
#define flt_10354148 XI_EA(flt_10354148_t, 0x10354148u)
typedef float flt_1035414C_t;
#define flt_1035414C XI_EA(flt_1035414C_t, 0x1035414Cu)
typedef int dword_10354150_t;
#define dword_10354150 XI_EA(dword_10354150_t, 0x10354150u)
typedef int dword_10354154_t;
#define dword_10354154 XI_EA(dword_10354154_t, 0x10354154u)
typedef float flt_10354160_t;
#define flt_10354160 XI_EA(flt_10354160_t, 0x10354160u)
typedef float flt_10354164_t;
#define flt_10354164 XI_EA(flt_10354164_t, 0x10354164u)
typedef _DWORD dword_10354168_t[30];
#define dword_10354168 XI_EA(dword_10354168_t, 0x10354168u)
typedef int dword_103541E0_t[18];
#define dword_103541E0 XI_EA(dword_103541E0_t, 0x103541E0u)
typedef _DWORD dword_10354228_t[15];
#define dword_10354228 XI_EA(dword_10354228_t, 0x10354228u)
typedef int dword_10354264_t[6];
#define dword_10354264 XI_EA(dword_10354264_t, 0x10354264u)
typedef int dword_103546A0_t[6];
#define dword_103546A0 XI_EA(dword_103546A0_t, 0x103546A0u)
typedef _BYTE unk_103546B8_t;
#define unk_103546B8 XI_EA(unk_103546B8_t, 0x103546B8u)
typedef float flt_103546CC_t;
#define flt_103546CC XI_EA(flt_103546CC_t, 0x103546CCu)
typedef float flt_103546E4_t;
#define flt_103546E4 XI_EA(flt_103546E4_t, 0x103546E4u)
typedef float flt_103546F4_t;
#define flt_103546F4 XI_EA(flt_103546F4_t, 0x103546F4u)
typedef float flt_103546F8_t;
#define flt_103546F8 XI_EA(flt_103546F8_t, 0x103546F8u)
typedef float flt_103582E0_t;
#define flt_103582E0 XI_EA(flt_103582E0_t, 0x103582E0u)
typedef int dword_103582E4_t;
#define dword_103582E4 XI_EA(dword_103582E4_t, 0x103582E4u)
typedef float flt_103582EC_t;
#define flt_103582EC XI_EA(flt_103582EC_t, 0x103582ECu)
typedef int dword_103582F0_t;
#define dword_103582F0 XI_EA(dword_103582F0_t, 0x103582F0u)
typedef float flt_103582F8_t;
#define flt_103582F8 XI_EA(flt_103582F8_t, 0x103582F8u)
typedef float flt_103582FC_t;
#define flt_103582FC XI_EA(flt_103582FC_t, 0x103582FCu)
typedef float flt_10358304_t;
#define flt_10358304 XI_EA(flt_10358304_t, 0x10358304u)
typedef float flt_10358308_t;
#define flt_10358308 XI_EA(flt_10358308_t, 0x10358308u)
typedef int dword_1035D694_t;
#define dword_1035D694 XI_EA(dword_1035D694_t, 0x1035D694u)
typedef int dword_1035D698_t;
#define dword_1035D698 XI_EA(dword_1035D698_t, 0x1035D698u)
typedef int dword_1035D69C_t;
#define dword_1035D69C XI_EA(dword_1035D69C_t, 0x1035D69Cu)
typedef _DWORD dword_1035D6A4_t[3];
#define dword_1035D6A4 XI_EA(dword_1035D6A4_t, 0x1035D6A4u)
typedef int dword_1035D6B0_t;
#define dword_1035D6B0 XI_EA(dword_1035D6B0_t, 0x1035D6B0u)
typedef int dword_1035D6B4_t;
#define dword_1035D6B4 XI_EA(dword_1035D6B4_t, 0x1035D6B4u)
typedef int dword_1035D6B8_t;
#define dword_1035D6B8 XI_EA(dword_1035D6B8_t, 0x1035D6B8u)
typedef int dword_1035D6BC_t;
#define dword_1035D6BC XI_EA(dword_1035D6BC_t, 0x1035D6BCu)
typedef int dword_1035D6C0_t;
#define dword_1035D6C0 XI_EA(dword_1035D6C0_t, 0x1035D6C0u)
typedef int dword_1035D6C4_t;
#define dword_1035D6C4 XI_EA(dword_1035D6C4_t, 0x1035D6C4u)
typedef int dword_1035D6C8_t;
#define dword_1035D6C8 XI_EA(dword_1035D6C8_t, 0x1035D6C8u)
typedef int dword_1035D6CC_t;
#define dword_1035D6CC XI_EA(dword_1035D6CC_t, 0x1035D6CCu)
typedef int dword_1035D6D0_t;
#define dword_1035D6D0 XI_EA(dword_1035D6D0_t, 0x1035D6D0u)
typedef int dword_1035D6D4_t;
#define dword_1035D6D4 XI_EA(dword_1035D6D4_t, 0x1035D6D4u)
typedef int dword_1035D6D8_t;
#define dword_1035D6D8 XI_EA(dword_1035D6D8_t, 0x1035D6D8u)
typedef int dword_1035D6DC_t;
#define dword_1035D6DC XI_EA(dword_1035D6DC_t, 0x1035D6DCu)
typedef int dword_1035D6E0_t;
#define dword_1035D6E0 XI_EA(dword_1035D6E0_t, 0x1035D6E0u)
typedef int dword_1035D6E4_t;
#define dword_1035D6E4 XI_EA(dword_1035D6E4_t, 0x1035D6E4u)
typedef int dword_1035D6E8_t;
#define dword_1035D6E8 XI_EA(dword_1035D6E8_t, 0x1035D6E8u)
typedef int dword_1035D6EC_t;
#define dword_1035D6EC XI_EA(dword_1035D6EC_t, 0x1035D6ECu)
typedef int dword_1035D6F0_t;
#define dword_1035D6F0 XI_EA(dword_1035D6F0_t, 0x1035D6F0u)
typedef int dword_1035D6F4_t;
#define dword_1035D6F4 XI_EA(dword_1035D6F4_t, 0x1035D6F4u)
typedef int dword_1035D6F8_t;
#define dword_1035D6F8 XI_EA(dword_1035D6F8_t, 0x1035D6F8u)
typedef int dword_1035D6FC_t;
#define dword_1035D6FC XI_EA(dword_1035D6FC_t, 0x1035D6FCu)
typedef int dword_1035D700_t;
#define dword_1035D700 XI_EA(dword_1035D700_t, 0x1035D700u)
typedef int dword_1035D704_t;
#define dword_1035D704 XI_EA(dword_1035D704_t, 0x1035D704u)
typedef int dword_1035D708_t;
#define dword_1035D708 XI_EA(dword_1035D708_t, 0x1035D708u)
typedef int dword_1035D70C_t;
#define dword_1035D70C XI_EA(dword_1035D70C_t, 0x1035D70Cu)
typedef int dword_1035D710_t;
#define dword_1035D710 XI_EA(dword_1035D710_t, 0x1035D710u)
typedef int dword_1035D714_t;
#define dword_1035D714 XI_EA(dword_1035D714_t, 0x1035D714u)
typedef int dword_1035D718_t;
#define dword_1035D718 XI_EA(dword_1035D718_t, 0x1035D718u)
typedef int dword_1035D71C_t;
#define dword_1035D71C XI_EA(dword_1035D71C_t, 0x1035D71Cu)
typedef _UNKNOWN * off_1035F378_t;
#define off_1035F378 XI_EA(off_1035F378_t, 0x1035F378u)
typedef __int16 word_1035F410_t[58];
#define word_1035F410 XI_EA(word_1035F410_t, 0x1035F410u)
typedef __int16 word_1035F598_t[10];
#define word_1035F598 XI_EA(word_1035F598_t, 0x1035F598u)
typedef char aNoOwnerChocobo_t[72];
#define aNoOwnerChocobo XI_EA(aNoOwnerChocobo_t, 0x1035F638u)
typedef char aNoOwnerGroundTarget_t[88];
#define aNoOwnerGroundTarget XI_EA(aNoOwnerGroundTarget_t, 0x1035F680u)
typedef char aNoOwnerRange_t[80];
#define aNoOwnerRange XI_EA(aNoOwnerRange_t, 0x1035F6D8u)
typedef char aNoOwnerBorder_t[80];
#define aNoOwnerBorder XI_EA(aNoOwnerBorder_t, 0x1035F728u)
typedef char aNoOwnerChair_t[72];
#define aNoOwnerChair XI_EA(aNoOwnerChair_t, 0x1035F778u)
typedef char aNoOwnerFishingRod_t[72];
#define aNoOwnerFishingRod XI_EA(aNoOwnerFishingRod_t, 0x1035F7C0u)
typedef char aNoOwnerMagicCircle_t[72];
#define aNoOwnerMagicCircle XI_EA(aNoOwnerMagicCircle_t, 0x1035F808u)
typedef char aSubActorNoMainChocobo_t[64];
#define aSubActorNoMainChocobo XI_EA(aSubActorNoMainChocobo_t, 0x1035F870u)
typedef char aSubActorNoMainMagicCircle_t[60];
#define aSubActorNoMainMagicCircle XI_EA(aSubActorNoMainMagicCircle_t, 0x1035F8B0u)
typedef char aSubActorNoMainChair_t[60];
#define aSubActorNoMainChair XI_EA(aSubActorNoMainChair_t, 0x1035F8ECu)
typedef char aSubActorNoMainRange_t[60];
#define aSubActorNoMainRange XI_EA(aSubActorNoMainRange_t, 0x1035F928u)
typedef char aSubActorNoMainFishingRod_t[60];
#define aSubActorNoMainFishingRod XI_EA(aSubActorNoMainFishingRod_t, 0x1035F964u)
typedef char byte_1035FA30_t[100];
#define byte_1035FA30 XI_EA(byte_1035FA30_t, 0x1035FA30u)
typedef char byte_1035FA94_t[60];
#define byte_1035FA94 XI_EA(byte_1035FA94_t, 0x1035FA94u)
typedef char byte_1035FAD0_t[60];
#define byte_1035FAD0 XI_EA(byte_1035FAD0_t, 0x1035FAD0u)
typedef char byte_1035FB0C_t[48];
#define byte_1035FB0C XI_EA(byte_1035FB0C_t, 0x1035FB0Cu)
typedef char byte_1035FB3C_t[28];
#define byte_1035FB3C XI_EA(byte_1035FB3C_t, 0x1035FB3Cu)
typedef void * g_pGameMode_t;
#define g_pGameMode XI_EA(g_pGameMode_t, 0x103601BCu)
typedef char aFontMoji_t[1];
#define aFontMoji XI_EA(aFontMoji_t, 0x1036C224u)
typedef char aCellTextureTex_1_t[1];
#define aCellTextureTex_1 XI_EA(aCellTextureTex_1_t, 0x1036C238u)
typedef char aCellTextureTex_0_t[1];
#define aCellTextureTex_0 XI_EA(aCellTextureTex_0_t, 0x1036C25Cu)
typedef char aCellTextureTex_t[1];
#define aCellTextureTex XI_EA(aCellTextureTex_t, 0x1036C27Cu)
typedef char aCellTextureFra_t[1];
#define aCellTextureFra XI_EA(aCellTextureFra_t, 0x1036C29Cu)
typedef char aCellTextureZer_t[1];
#define aCellTextureZer XI_EA(aCellTextureZer_t, 0x1036C2B8u)
typedef char aCellFrameTextu_t[1];
#define aCellFrameTextu XI_EA(aCellFrameTextu_t, 0x1036C2D8u)
typedef char aCellFrameFrame_1_t[1];
#define aCellFrameFrame_1 XI_EA(aCellFrameFrame_1_t, 0x1036C2F4u)
typedef char aCellFrameFrame_0_t[1];
#define aCellFrameFrame_0 XI_EA(aCellFrameFrame_0_t, 0x1036C314u)
typedef char aCellFrameFrame_t[1];
#define aCellFrameFrame XI_EA(aCellFrameFrame_t, 0x1036C330u)
typedef char aCellZeroTextur_0_t[1];
#define aCellZeroTextur_0 XI_EA(aCellZeroTextur_0_t, 0x1036C34Cu)
typedef char aCellZeroTextur_t[1];
#define aCellZeroTextur XI_EA(aCellZeroTextur_t, 0x1036C36Cu)
typedef char aCellZeroFrameZ_t[1];
#define aCellZeroFrameZ XI_EA(aCellZeroFrameZ_t, 0x1036C388u)
typedef char aCellZeroZeroTe_t[1];
#define aCellZeroZeroTe XI_EA(aCellZeroZeroTe_t, 0x1036C3A4u)
typedef char aCellZeroZeroFr_t[1];
#define aCellZeroZeroFr XI_EA(aCellZeroZeroFr_t, 0x1036C3C0u)
typedef char aCellZeroZeroZe_t[1];
#define aCellZeroZeroZe XI_EA(aCellZeroZeroZe_t, 0x1036C3DCu)
typedef int dword_1036E4E8_t[];
#define dword_1036E4E8 XI_EA(dword_1036E4E8_t, 0x1036E4E8u)
typedef int g_cursorShapes_t[];
#define g_cursorShapes XI_EA(g_cursorShapes_t, 0x1036E4ECu)
typedef int g_cursorShapeRotation_t[];
#define g_cursorShapeRotation XI_EA(g_cursorShapeRotation_t, 0x1036E4F4u)
typedef __int16 g_cursorShapeFrame_t[];
#define g_cursorShapeFrame XI_EA(g_cursorShapeFrame_t, 0x1036E4F8u)
typedef __int16 word_1036E4FA_t[125];
#define word_1036E4FA XI_EA(word_1036E4FA_t, 0x1036E4FAu)
typedef char aAncAncScro_t[1];
#define aAncAncScro XI_EA(aAncAncScro_t, 0x1036E6C8u)
typedef char aMenuKeytops3_t[1];
#define aMenuKeytops3 XI_EA(aMenuKeytops3_t, 0x1036FFC8u)
typedef char byte_10376AC0_t[256];
#define byte_10376AC0 XI_EA(byte_10376AC0_t, 0x10376AC0u)
typedef char aCTestVsh_t[1];
#define aCTestVsh XI_EA(aCTestVsh_t, 0x10376CA4u)
typedef char ** off_10376FF8_t;
#define off_10376FF8 XI_EA(off_10376FF8_t, 0x10376FF8u)
typedef char ** off_10377048_t;
#define off_10377048 XI_EA(off_10377048_t, 0x10377048u)
typedef char ** off_103770D0_t;
#define off_103770D0 XI_EA(off_103770D0_t, 0x103770D0u)
typedef char ** off_10377168_t;
#define off_10377168 XI_EA(off_10377168_t, 0x10377168u)
typedef char ** off_10377210_t;
#define off_10377210 XI_EA(off_10377210_t, 0x10377210u)
typedef char ** off_103772C4_t;
#define off_103772C4 XI_EA(off_103772C4_t, 0x103772C4u)
typedef _BYTE g_zoneVSDeclA_t;
#define g_zoneVSDeclA XI_EA(g_zoneVSDeclA_t, 0x103772E8u)
typedef _BYTE g_zoneVSDeclB_t;
#define g_zoneVSDeclB XI_EA(g_zoneVSDeclB_t, 0x10377304u)
typedef int dword_10377338_t;
#define dword_10377338 XI_EA(dword_10377338_t, 0x10377338u)
typedef int dword_1037733C_t;
#define dword_1037733C XI_EA(dword_1037733C_t, 0x1037733Cu)
typedef int dword_10377340_t;
#define dword_10377340 XI_EA(dword_10377340_t, 0x10377340u)
typedef float flt_10377344_t;
#define flt_10377344 XI_EA(flt_10377344_t, 0x10377344u)
typedef int dword_10377348_t;
#define dword_10377348 XI_EA(dword_10377348_t, 0x10377348u)
typedef float flt_1037734C_t;
#define flt_1037734C XI_EA(flt_1037734C_t, 0x1037734Cu)
typedef float flt_103773C0_t;
#define flt_103773C0 XI_EA(flt_103773C0_t, 0x103773C0u)
typedef float flt_103773C4_t;
#define flt_103773C4 XI_EA(flt_103773C4_t, 0x103773C4u)
typedef float flt_103773C8_t;
#define flt_103773C8 XI_EA(flt_103773C8_t, 0x103773C8u)
typedef int dword_103773CC_t;
#define dword_103773CC XI_EA(dword_103773CC_t, 0x103773CCu)
typedef int dword_103773D0_t;
#define dword_103773D0 XI_EA(dword_103773D0_t, 0x103773D0u)
typedef int dword_103773D4_t;
#define dword_103773D4 XI_EA(dword_103773D4_t, 0x103773D4u)
typedef float flt_103773D8_t;
#define flt_103773D8 XI_EA(flt_103773D8_t, 0x103773D8u)
typedef int dword_103773DC_t;
#define dword_103773DC XI_EA(dword_103773DC_t, 0x103773DCu)
typedef float flt_103773E0_t;
#define flt_103773E0 XI_EA(flt_103773E0_t, 0x103773E0u)
typedef float flt_103890E4_t;
#define flt_103890E4 XI_EA(flt_103890E4_t, 0x103890E4u)
typedef float kF_0_9874_t[7];
#define kF_0_9874 XI_EA(kF_0_9874_t, 0x10389874u)
typedef char aWorkmem_t[1];
#define aWorkmem XI_EA(aWorkmem_t, 0x1038992Cu)
typedef char aMainmem_t[1];
#define aMainmem XI_EA(aMainmem_t, 0x10389934u)
typedef char aMainpool0x08xB_t[1];
#define aMainpool0x08xB XI_EA(aMainpool0x08xB_t, 0x1038993Cu)
typedef char aWorkpool0x08xB_t[1];
#define aWorkpool0x08xB XI_EA(aWorkpool0x08xB_t, 0x1038995Cu)
typedef char aMemtotal0x08xB_t[1];
#define aMemtotal0x08xB XI_EA(aMemtotal0x08xB_t, 0x10389984u)
typedef char aErrorCouldnTAl_t[1];
#define aErrorCouldnTAl XI_EA(aErrorCouldnTAl_t, 0x103899A4u)
typedef char aErrorCouldNotL_t[1];
#define aErrorCouldNotL XI_EA(aErrorCouldNotL_t, 0x103B4854u)
typedef char aErrorCouldNotA_t[1];
#define aErrorCouldNotA XI_EA(aErrorCouldNotA_t, 0x103B4874u)
typedef char aErrorSqgxtextu_t[1];
#define aErrorSqgxtextu XI_EA(aErrorSqgxtextu_t, 0x103B48A8u)
typedef char aError_t[1];
#define aError XI_EA(aError_t, 0x103B48D0u)
typedef char aCreatingTextur_t[1];
#define aCreatingTextur XI_EA(aCreatingTextur_t, 0x103B48D8u)
typedef int g_sqgxVertexShaderHandle_t[1024];
#define g_sqgxVertexShaderHandle XI_EA(g_sqgxVertexShaderHandle_t, 0x103B4910u)
typedef int g_sqgxVertexShaderKeyA_t[];
#define g_sqgxVertexShaderKeyA XI_EA(g_sqgxVertexShaderKeyA_t, 0x103B5910u)
typedef int g_sqgxVertexShaderKeyB_t[2047];
#define g_sqgxVertexShaderKeyB XI_EA(g_sqgxVertexShaderKeyB_t, 0x103B5914u)
typedef int g_sceneFadeMaskRT_t;
#define g_sceneFadeMaskRT XI_EA(g_sceneFadeMaskRT_t, 0x103BA68Cu)
typedef char aErrorInSetVert_t[1];
#define aErrorInSetVert XI_EA(aErrorInSetVert_t, 0x103BBC38u)
typedef char aErrorInSetStre_t[1];
#define aErrorInSetStre XI_EA(aErrorInSetStre_t, 0x103BBC54u)
typedef char aCDevDancerModu_30_t[1];
#define aCDevDancerModu_30 XI_EA(aCDevDancerModu_30_t, 0x103BC70Cu)
typedef char aShadowVolumesO_0_t[1];
#define aShadowVolumesO_0 XI_EA(aShadowVolumesO_0_t, 0x103BD56Cu)
typedef char aShadowVolumesO_t[1];
#define aShadowVolumesO XI_EA(aShadowVolumesO_t, 0x103BD5A0u)
typedef void * g_pApp_t;
#define g_pApp XI_EA(g_pApp_t, 0x103D3C64u)
typedef CHAR Read_t;
#define Read XI_EA(Read_t, 0x103D3C84u)
typedef int g_pAppSettings_t;
#define g_pAppSettings XI_EA(g_pAppSettings_t, 0x103D3CB8u)
typedef D3DHolder * g_pD3DHolder_t;
#define g_pD3DHolder XI_EA(g_pD3DHolder_t, 0x103D3CBCu)
typedef int dword_10455AF8_t;
#define dword_10455AF8 XI_EA(dword_10455AF8_t, 0x10455AF8u)
typedef int dword_10455C20_t;
#define dword_10455C20 XI_EA(dword_10455C20_t, 0x10455C20u)
typedef char byte_10456560_t;
#define byte_10456560 XI_EA(byte_10456560_t, 0x10456560u)
typedef CHAR byte_10456568_t[40];
#define byte_10456568 XI_EA(byte_10456568_t, 0x10456568u)
typedef CDx * g_pCDx_t;
#define g_pCDx XI_EA(g_pCDx_t, 0x1045666Cu)
typedef _BYTE unk_10456670_t;
#define unk_10456670 XI_EA(unk_10456670_t, 0x10456670u)
typedef float flt_10456678_t;
#define flt_10456678 XI_EA(flt_10456678_t, 0x10456678u)
typedef float flt_1045667C_t;
#define flt_1045667C XI_EA(flt_1045667C_t, 0x1045667Cu)
typedef int dword_10456680_t;
#define dword_10456680 XI_EA(dword_10456680_t, 0x10456680u)
typedef int dword_10456684_t;
#define dword_10456684 XI_EA(dword_10456684_t, 0x10456684u)
typedef int dword_10456688_t;
#define dword_10456688 XI_EA(dword_10456688_t, 0x10456688u)
typedef int dword_1045668C_t;
#define dword_1045668C XI_EA(dword_1045668C_t, 0x1045668Cu)
typedef int dword_10456690_t;
#define dword_10456690 XI_EA(dword_10456690_t, 0x10456690u)
typedef float flt_10456694_t;
#define flt_10456694 XI_EA(flt_10456694_t, 0x10456694u)
typedef float flt_10456698_t;
#define flt_10456698 XI_EA(flt_10456698_t, 0x10456698u)
typedef int dword_1045669C_t;
#define dword_1045669C XI_EA(dword_1045669C_t, 0x1045669Cu)
typedef int dword_104566A0_t;
#define dword_104566A0 XI_EA(dword_104566A0_t, 0x104566A0u)
typedef int dword_104566A4_t;
#define dword_104566A4 XI_EA(dword_104566A4_t, 0x104566A4u)
typedef int dword_104566A8_t;
#define dword_104566A8 XI_EA(dword_104566A8_t, 0x104566A8u)
typedef int dword_104566AC_t;
#define dword_104566AC XI_EA(dword_104566AC_t, 0x104566ACu)
typedef float flt_104566B0_t;
#define flt_104566B0 XI_EA(flt_104566B0_t, 0x104566B0u)
typedef float flt_104566B4_t;
#define flt_104566B4 XI_EA(flt_104566B4_t, 0x104566B4u)
typedef int dword_104566B8_t;
#define dword_104566B8 XI_EA(dword_104566B8_t, 0x104566B8u)
typedef int dword_104566BC_t;
#define dword_104566BC XI_EA(dword_104566BC_t, 0x104566BCu)
typedef int dword_104566C0_t;
#define dword_104566C0 XI_EA(dword_104566C0_t, 0x104566C0u)
typedef int dword_104566C4_t;
#define dword_104566C4 XI_EA(dword_104566C4_t, 0x104566C4u)
typedef int dword_104566C8_t;
#define dword_104566C8 XI_EA(dword_104566C8_t, 0x104566C8u)
typedef float flt_104566CC_t;
#define flt_104566CC XI_EA(flt_104566CC_t, 0x104566CCu)
typedef float flt_104566D0_t;
#define flt_104566D0 XI_EA(flt_104566D0_t, 0x104566D0u)
typedef int dword_104566D4_t;
#define dword_104566D4 XI_EA(dword_104566D4_t, 0x104566D4u)
typedef int dword_104566D8_t;
#define dword_104566D8 XI_EA(dword_104566D8_t, 0x104566D8u)
typedef int dword_104566DC_t;
#define dword_104566DC XI_EA(dword_104566DC_t, 0x104566DCu)
typedef int dword_104566E0_t;
#define dword_104566E0 XI_EA(dword_104566E0_t, 0x104566E0u)
typedef int dword_104566E4_t;
#define dword_104566E4 XI_EA(dword_104566E4_t, 0x104566E4u)
typedef _DWORD * g_pEnv_t;
#define g_pEnv XI_EA(g_pEnv_t, 0x104566E8u)
typedef int dword_10456790_t;
#define dword_10456790 XI_EA(dword_10456790_t, 0x10456790u)
typedef int dword_10456794_t;
#define dword_10456794 XI_EA(dword_10456794_t, 0x10456794u)
typedef float g_afterimageStrength_t;
#define g_afterimageStrength XI_EA(g_afterimageStrength_t, 0x1045689Cu)
typedef int dword_104568A0_t;
#define dword_104568A0 XI_EA(dword_104568A0_t, 0x104568A0u)
typedef char dword_104568A8_t[1];
#define dword_104568A8 XI_EA(dword_104568A8_t, 0x104568A8u)
typedef int dword_104568E8_t;
#define dword_104568E8 XI_EA(dword_104568E8_t, 0x104568E8u)
typedef int dword_104568EC_t;
#define dword_104568EC XI_EA(dword_104568EC_t, 0x104568ECu)
typedef int dword_104568F0_t;
#define dword_104568F0 XI_EA(dword_104568F0_t, 0x104568F0u)
typedef __int16 word_104568F8_t;
#define word_104568F8 XI_EA(word_104568F8_t, 0x104568F8u)
typedef std::codecvt_base * g_pCamera_t;
#define g_pCamera XI_EA(g_pCamera_t, 0x104568FCu)
typedef int Code_t;
#define Code XI_EA(Code_t, 0x10456900u)
typedef int dword_10456904_t;
#define dword_10456904 XI_EA(dword_10456904_t, 0x10456904u)
typedef int g_modelDrawCallCount_t;
#define g_modelDrawCallCount XI_EA(g_modelDrawCallCount_t, 0x10456908u)
typedef int g_afterimageTex_t[];
#define g_afterimageTex XI_EA(g_afterimageTex_t, 0x10456910u)
typedef int g_afterimageDepth_t[];
#define g_afterimageDepth XI_EA(g_afterimageDepth_t, 0x10456918u)
typedef int g_afterimageState_t;
#define g_afterimageState XI_EA(g_afterimageState_t, 0x10456920u)
typedef int g_afterimageBufferState_t[];
#define g_afterimageBufferState XI_EA(g_afterimageBufferState_t, 0x10456924u)
typedef int g_afterimagePhase_t;
#define g_afterimagePhase XI_EA(g_afterimagePhase_t, 0x10456928u)
typedef int g_feedbackBufferIndex_t;
#define g_feedbackBufferIndex XI_EA(g_feedbackBufferIndex_t, 0x1045692Cu)
typedef int g_feedbackBufferState_t[];
#define g_feedbackBufferState XI_EA(g_feedbackBufferState_t, 0x10456930u)
typedef int g_feedbackActive_t;
#define g_feedbackActive XI_EA(g_feedbackActive_t, 0x10456934u)
typedef int g_feedbackTexHalfRes_t[];
#define g_feedbackTexHalfRes XI_EA(g_feedbackTexHalfRes_t, 0x10456938u)
typedef int dword_1045693C_t;
#define dword_1045693C XI_EA(dword_1045693C_t, 0x1045693Cu)
typedef int g_feedbackDepth_t[];
#define g_feedbackDepth XI_EA(g_feedbackDepth_t, 0x10456940u)
typedef _DWORD dword_10456944_t;
#define dword_10456944 XI_EA(dword_10456944_t, 0x10456944u)
typedef int dword_10456948_t;
#define dword_10456948 XI_EA(dword_10456948_t, 0x10456948u)
typedef int dword_1045694C_t;
#define dword_1045694C XI_EA(dword_1045694C_t, 0x1045694Cu)
typedef int dword_10456950_t;
#define dword_10456950 XI_EA(dword_10456950_t, 0x10456950u)
typedef int dword_10456954_t;
#define dword_10456954 XI_EA(dword_10456954_t, 0x10456954u)
typedef int dword_10456958_t;
#define dword_10456958 XI_EA(dword_10456958_t, 0x10456958u)
typedef int dword_1045695C_t;
#define dword_1045695C XI_EA(dword_1045695C_t, 0x1045695Cu)
typedef float flt_10456960_t;
#define flt_10456960 XI_EA(flt_10456960_t, 0x10456960u)
typedef int dword_10456964_t[4];
#define dword_10456964 XI_EA(dword_10456964_t, 0x10456964u)
typedef int dword_10456974_t;
#define dword_10456974 XI_EA(dword_10456974_t, 0x10456974u)
typedef int g_countdown_10456978_t;
#define g_countdown_10456978 XI_EA(g_countdown_10456978_t, 0x10456978u)
typedef int dword_10456984_t;
#define dword_10456984 XI_EA(dword_10456984_t, 0x10456984u)
typedef HWND hWnd_t;
#define hWnd XI_EA(hWnd_t, 0x104569A8u)
typedef HINSTANCE hmod_t;
#define hmod XI_EA(hmod_t, 0x104569ACu)
typedef int g_cmdLine_t;
#define g_cmdLine XI_EA(g_cmdLine_t, 0x104569B0u)
typedef int dword_104569B4_t;
#define dword_104569B4 XI_EA(dword_104569B4_t, 0x104569B4u)
typedef int g_installDir_t;
#define g_installDir XI_EA(g_installDir_t, 0x104569B8u)
typedef int dword_104569BC_t;
#define dword_104569BC XI_EA(dword_104569BC_t, 0x104569BCu)
typedef int dword_104569C0_t;
#define dword_104569C0 XI_EA(dword_104569C0_t, 0x104569C0u)
typedef int dword_104569D0_t;
#define dword_104569D0 XI_EA(dword_104569D0_t, 0x104569D0u)
typedef HKEY phkResult_t;
#define phkResult XI_EA(phkResult_t, 0x104569D4u)
typedef char byte_104569EC_t;
#define byte_104569EC XI_EA(byte_104569EC_t, 0x104569ECu)
typedef int dword_104569F4_t;
#define dword_104569F4 XI_EA(dword_104569F4_t, 0x104569F4u)
typedef _BYTE unk_10456A00_t;
#define unk_10456A00 XI_EA(unk_10456A00_t, 0x10456A00u)
typedef int g_pSceneState_t;
#define g_pSceneState XI_EA(g_pSceneState_t, 0x10456A28u)
typedef int dword_10456A2C_t;
#define dword_10456A2C XI_EA(dword_10456A2C_t, 0x10456A2Cu)
typedef int xRight_t;
#define xRight XI_EA(xRight_t, 0x10456A58u)
typedef int g_cfgGraphicsStabilization_t;
#define g_cfgGraphicsStabilization XI_EA(g_cfgGraphicsStabilization_t, 0x10456A74u)
typedef int g_windowMode_t;
#define g_windowMode XI_EA(g_windowMode_t, 0x10456A7Cu)
typedef int dword_10456A80_t;
#define dword_10456A80 XI_EA(dword_10456A80_t, 0x10456A80u)
typedef int g_cfgSoundEnabled_t;
#define g_cfgSoundEnabled XI_EA(g_cfgSoundEnabled_t, 0x10456A88u)
typedef int g_cfgMenuResY_t;
#define g_cfgMenuResY XI_EA(g_cfgMenuResY_t, 0x10456A90u)
typedef int dword_10456A98_t;
#define dword_10456A98 XI_EA(dword_10456A98_t, 0x10456A98u)
typedef int g_cfgFontCompression_t;
#define g_cfgFontCompression XI_EA(g_cfgFontCompression_t, 0x10456AA0u)
typedef int dword_10456AA4_t;
#define dword_10456AA4 XI_EA(dword_10456AA4_t, 0x10456AA4u)
typedef int g_cfgMapCompression_t;
#define g_cfgMapCompression XI_EA(g_cfgMapCompression_t, 0x10456AB0u)
typedef int dword_10456AC0_t;
#define dword_10456AC0 XI_EA(dword_10456AC0_t, 0x10456AC0u)
typedef int g_cfgMenuResX_t;
#define g_cfgMenuResX XI_EA(g_cfgMenuResX_t, 0x10456AC8u)
typedef int g_shadowSetting_t;
#define g_shadowSetting XI_EA(g_shadowSetting_t, 0x10456ACCu)
typedef int g_cfgTextureCompression_t;
#define g_cfgTextureCompression XI_EA(g_cfgTextureCompression_t, 0x10456AD0u)
typedef int g_depthBlurEnabled_Dead_t;
#define g_depthBlurEnabled_Dead XI_EA(g_depthBlurEnabled_Dead_t, 0x10456AD8u)
typedef int dword_10456BE4_t;
#define dword_10456BE4 XI_EA(dword_10456BE4_t, 0x10456BE4u)
typedef int dword_10456BF0_t;
#define dword_10456BF0 XI_EA(dword_10456BF0_t, 0x10456BF0u)
typedef int g_cfgHardwareMouse_t;
#define g_cfgHardwareMouse XI_EA(g_cfgHardwareMouse_t, 0x10456BF8u)
typedef int g_cfg0020_t;
#define g_cfg0020 XI_EA(g_cfg0020_t, 0x10456C00u)
typedef int yBottom_t;
#define yBottom XI_EA(yBottom_t, 0x10456C08u)
typedef void * dword_10456C0C_t;
#define dword_10456C0C XI_EA(dword_10456C0C_t, 0x10456C0Cu)
typedef int dword_10456D18_t;
#define dword_10456D18 XI_EA(dword_10456D18_t, 0x10456D18u)
typedef int dword_10456D1C_t;
#define dword_10456D1C XI_EA(dword_10456D1C_t, 0x10456D1Cu)
typedef int dword_10456D20_t;
#define dword_10456D20 XI_EA(dword_10456D20_t, 0x10456D20u)
typedef char g_translucentForce_t;
#define g_translucentForce XI_EA(g_translucentForce_t, 0x10456D48u)
typedef int g_translucentColor_t;
#define g_translucentColor XI_EA(g_translucentColor_t, 0x10456D4Cu)
typedef IDirect3DTexture8 * pTexture_t;
#define pTexture XI_EA(pTexture_t, 0x10456D50u)
typedef int g_translucentTargetEye2_t;
#define g_translucentTargetEye2 XI_EA(g_translucentTargetEye2_t, 0x10456D54u)
typedef _DWORD g_translucentTarget_t;
#define g_translucentTarget XI_EA(g_translucentTarget_t, 0x10456D58u)
typedef _DWORD g_translucentDepth_t;
#define g_translucentDepth XI_EA(g_translucentDepth_t, 0x10456D5Cu)
typedef float g_modelLight_t;
#define g_modelLight XI_EA(g_modelLight_t, 0x10456DD0u)
typedef float g_modelLightDiffuseR_t;
#define g_modelLightDiffuseR XI_EA(g_modelLightDiffuseR_t, 0x10456DD4u)
typedef float g_modelLightDiffuseG_t;
#define g_modelLightDiffuseG XI_EA(g_modelLightDiffuseG_t, 0x10456DD8u)
typedef float g_modelLightDiffuseB_t;
#define g_modelLightDiffuseB XI_EA(g_modelLightDiffuseB_t, 0x10456DDCu)
typedef int g_alignedBufferSize_t;
#define g_alignedBufferSize XI_EA(g_alignedBufferSize_t, 0x10456E40u)
typedef int dword_10456E44_t;
#define dword_10456E44 XI_EA(dword_10456E44_t, 0x10456E44u)
typedef int dword_10456E48_t;
#define dword_10456E48 XI_EA(dword_10456E48_t, 0x10456E48u)
typedef int g_vertexShader_6E4C_t;
#define g_vertexShader_6E4C XI_EA(g_vertexShader_6E4C_t, 0x10456E4Cu)
typedef int g_projTexFactor_t;
#define g_projTexFactor XI_EA(g_projTexFactor_t, 0x10456E50u)
typedef int dword_10456E54_t;
#define dword_10456E54 XI_EA(dword_10456E54_t, 0x10456E54u)
typedef int g_projTexPrevTex0_t;
#define g_projTexPrevTex0 XI_EA(g_projTexPrevTex0_t, 0x10456E58u)
typedef int g_projTexPrevTex1_t;
#define g_projTexPrevTex1 XI_EA(g_projTexPrevTex1_t, 0x10456E5Cu)
typedef int dword_10456E60_t;
#define dword_10456E60 XI_EA(dword_10456E60_t, 0x10456E60u)
typedef int g_projTexTarget_t;
#define g_projTexTarget XI_EA(g_projTexTarget_t, 0x10456E64u)
typedef int g_projTexPrevFactor_t;
#define g_projTexPrevFactor XI_EA(g_projTexPrevFactor_t, 0x10456E68u)
typedef int dword_10456E6C_t;
#define dword_10456E6C XI_EA(dword_10456E6C_t, 0x10456E6Cu)
typedef _DWORD dword_10456E70_t;
#define dword_10456E70 XI_EA(dword_10456E70_t, 0x10456E70u)
typedef int dword_10456E74_t;
#define dword_10456E74 XI_EA(dword_10456E74_t, 0x10456E74u)
typedef int dword_10456E78_t;
#define dword_10456E78 XI_EA(dword_10456E78_t, 0x10456E78u)
typedef int g_envMapPrevFactor_t;
#define g_envMapPrevFactor XI_EA(g_envMapPrevFactor_t, 0x10456E7Cu)
typedef int g_envMapPrevTex0_t;
#define g_envMapPrevTex0 XI_EA(g_envMapPrevTex0_t, 0x10456E80u)
typedef int g_envMapPrevTex1_t;
#define g_envMapPrevTex1 XI_EA(g_envMapPrevTex1_t, 0x10456E84u)
typedef int dword_10456E88_t;
#define dword_10456E88 XI_EA(dword_10456E88_t, 0x10456E88u)
typedef int g_envMapFactor_t;
#define g_envMapFactor XI_EA(g_envMapFactor_t, 0x10456E8Cu)
typedef char byte_104570F0_t;
#define byte_104570F0 XI_EA(byte_104570F0_t, 0x104570F0u)
typedef int dword_104570F4_t;
#define dword_104570F4 XI_EA(dword_104570F4_t, 0x104570F4u)
typedef int dword_104570F8_t;
#define dword_104570F8 XI_EA(dword_104570F8_t, 0x104570F8u)
typedef char g_envMapEnabled_t;
#define g_envMapEnabled XI_EA(g_envMapEnabled_t, 0x104570FCu)
typedef int dword_10457100_t;
#define dword_10457100 XI_EA(dword_10457100_t, 0x10457100u)
typedef char g_texture1Matrix_t[1];
#define g_texture1Matrix XI_EA(g_texture1Matrix_t, 0x10457108u)
typedef int dword_10457130_t;
#define dword_10457130 XI_EA(dword_10457130_t, 0x10457130u)
typedef int dword_10457140_t;
#define dword_10457140 XI_EA(dword_10457140_t, 0x10457140u)
typedef int g_vertexBuffer_7148_t;
#define g_vertexBuffer_7148 XI_EA(g_vertexBuffer_7148_t, 0x10457148u)
typedef int g_vertexBuffer_714C_t;
#define g_vertexBuffer_714C XI_EA(g_vertexBuffer_714C_t, 0x1045714Cu)
typedef int g_vertexBuffer_7150_t;
#define g_vertexBuffer_7150 XI_EA(g_vertexBuffer_7150_t, 0x10457150u)
typedef int g_vertexBuffer_7154_t;
#define g_vertexBuffer_7154 XI_EA(g_vertexBuffer_7154_t, 0x10457154u)
typedef char g_envMapTexMatrix_t[1];
#define g_envMapTexMatrix XI_EA(g_envMapTexMatrix_t, 0x10457158u)
typedef char g_shadowTexMatrix_t[1];
#define g_shadowTexMatrix XI_EA(g_shadowTexMatrix_t, 0x10457198u)
typedef int dword_104571AC_t;
#define dword_104571AC XI_EA(dword_104571AC_t, 0x104571ACu)
typedef int dword_104571C0_t;
#define dword_104571C0 XI_EA(dword_104571C0_t, 0x104571C0u)
typedef _DWORD g_ambientColor_t;
#define g_ambientColor XI_EA(g_ambientColor_t, 0x104571D8u)
typedef unsigned __int8 g_ambientColor_as_u8_t;
#define g_ambientColor_as_u8 XI_EA(g_ambientColor_as_u8_t, 0x104571D8u)
typedef char g_ambientColorG_t;
#define g_ambientColorG XI_EA(g_ambientColorG_t, 0x104571D9u)
typedef char g_ambientColorR_t;
#define g_ambientColorR XI_EA(g_ambientColorR_t, 0x104571DAu)
typedef char g_ambientColorA_t;
#define g_ambientColorA XI_EA(g_ambientColorA_t, 0x104571DBu)
typedef _DWORD g_modelMaterial_t[17];
#define g_modelMaterial XI_EA(g_modelMaterial_t, 0x104571E0u)
typedef _BYTE g_matrix_7228_t;
#define g_matrix_7228 XI_EA(g_matrix_7228_t, 0x10457228u)
typedef int dword_10457268_t;
#define dword_10457268 XI_EA(dword_10457268_t, 0x10457268u)
typedef int dword_1045726C_t;
#define dword_1045726C XI_EA(dword_1045726C_t, 0x1045726Cu)
typedef _DWORD g_matrix_7270_t[16];
#define g_matrix_7270 XI_EA(g_matrix_7270_t, 0x10457270u)
typedef char g_projTexEnabled_t;
#define g_projTexEnabled XI_EA(g_projTexEnabled_t, 0x104572B0u)
typedef float g_lightTable_Start_t;
#define g_lightTable_Start XI_EA(g_lightTable_Start_t, 0x104572B8u)
typedef float flt_10457388_t[52];
#define flt_10457388 XI_EA(flt_10457388_t, 0x10457388u)
typedef int g_lightTable_End_t;
#define g_lightTable_End XI_EA(g_lightTable_End_t, 0x10457458u)
typedef int dword_1045745C_t;
#define dword_1045745C XI_EA(dword_1045745C_t, 0x1045745Cu)
typedef char g_skinningCapabilityFlag_t;
#define g_skinningCapabilityFlag XI_EA(g_skinningCapabilityFlag_t, 0x10457460u)
typedef _DWORD g_matrix_7468_t[16];
#define g_matrix_7468 XI_EA(g_matrix_7468_t, 0x10457468u)
typedef int g_modelPrimitiveCount_t;
#define g_modelPrimitiveCount XI_EA(g_modelPrimitiveCount_t, 0x104574A8u)
typedef int g_modelColor_t;
#define g_modelColor XI_EA(g_modelColor_t, 0x104574ACu)
typedef int dword_104574B0_t;
#define dword_104574B0 XI_EA(dword_104574B0_t, 0x104574B0u)
typedef int dword_104574B4_t;
#define dword_104574B4 XI_EA(dword_104574B4_t, 0x104574B4u)
typedef char g_identityWorldMatrix_t[1];
#define g_identityWorldMatrix XI_EA(g_identityWorldMatrix_t, 0x104574B8u)
typedef char g_projTexMatrix_t[1];
#define g_projTexMatrix XI_EA(g_projTexMatrix_t, 0x104574F8u)
typedef float flt_10457528_t[4];
#define flt_10457528 XI_EA(flt_10457528_t, 0x10457528u)
typedef char byte_10457538_t;
#define byte_10457538 XI_EA(byte_10457538_t, 0x10457538u)
typedef int g_vertexBuffer_7540_t;
#define g_vertexBuffer_7540 XI_EA(g_vertexBuffer_7540_t, 0x10457540u)
typedef int dword_10457548_t;
#define dword_10457548 XI_EA(dword_10457548_t, 0x10457548u)
typedef int dword_10457550_t;
#define dword_10457550 XI_EA(dword_10457550_t, 0x10457550u)
typedef int g_streamSourceStride_t[5];
#define g_streamSourceStride XI_EA(g_streamSourceStride_t, 0x10457554u)
typedef float g_skinLight_t;
#define g_skinLight XI_EA(g_skinLight_t, 0x10457568u)
typedef float g_skinLightDiffuseR_t;
#define g_skinLightDiffuseR XI_EA(g_skinLightDiffuseR_t, 0x1045756Cu)
typedef float g_skinLightDiffuseG_t;
#define g_skinLightDiffuseG XI_EA(g_skinLightDiffuseG_t, 0x10457570u)
typedef float g_skinLightDiffuseB_t;
#define g_skinLightDiffuseB XI_EA(g_skinLightDiffuseB_t, 0x10457574u)
typedef int dword_1045B618_t;
#define dword_1045B618 XI_EA(dword_1045B618_t, 0x1045B618u)
typedef int dword_1045B61C_t;
#define dword_1045B61C XI_EA(dword_1045B61C_t, 0x1045B61Cu)
typedef int dword_1045B620_t;
#define dword_1045B620 XI_EA(dword_1045B620_t, 0x1045B620u)
typedef int g_lineVertexCursor_t;
#define g_lineVertexCursor XI_EA(g_lineVertexCursor_t, 0x1045B62Cu)
typedef int g_stripVertexCursor_t;
#define g_stripVertexCursor XI_EA(g_stripVertexCursor_t, 0x1045B630u)
typedef int dword_1045B648_t;
#define dword_1045B648 XI_EA(dword_1045B648_t, 0x1045B648u)
typedef int dword_1045B64C_t;
#define dword_1045B64C XI_EA(dword_1045B64C_t, 0x1045B64Cu)
typedef int g_os2VtxBufferSlots_t[16];
#define g_os2VtxBufferSlots XI_EA(g_os2VtxBufferSlots_t, 0x1045B650u)
typedef int g_streamSourceVB_t[];
#define g_streamSourceVB XI_EA(g_streamSourceVB_t, 0x1045B690u)
typedef int g_vertexShader_B6A0_t;
#define g_vertexShader_B6A0 XI_EA(g_vertexShader_B6A0_t, 0x1045B6A0u)
typedef int dword_1045B774_t;
#define dword_1045B774 XI_EA(dword_1045B774_t, 0x1045B774u)
typedef __int16 g_actorShadowMode_t;
#define g_actorShadowMode XI_EA(g_actorShadowMode_t, 0x1045B77Au)
typedef char g_offscreenWorldMatrix_t[1];
#define g_offscreenWorldMatrix XI_EA(g_offscreenWorldMatrix_t, 0x1045B780u)
typedef char g_lightViewMatrix_t[1];
#define g_lightViewMatrix XI_EA(g_lightViewMatrix_t, 0x1045B7C0u)
typedef int g_offscreenPrevTex0_t;
#define g_offscreenPrevTex0 XI_EA(g_offscreenPrevTex0_t, 0x1045B800u)
typedef int g_offscreenPrevTex1_t;
#define g_offscreenPrevTex1 XI_EA(g_offscreenPrevTex1_t, 0x1045B804u)
typedef int g_texturefactor_B808_t;
#define g_texturefactor_B808 XI_EA(g_texturefactor_B808_t, 0x1045B808u)
typedef float g_shadowCasterDistance_t;
#define g_shadowCasterDistance XI_EA(g_shadowCasterDistance_t, 0x1045B80Cu)
typedef int dword_1045B810_t;
#define dword_1045B810 XI_EA(dword_1045B810_t, 0x1045B810u)
typedef __int16 word_1045B814_t;
#define word_1045B814 XI_EA(word_1045B814_t, 0x1045B814u)
typedef int dword_1045B818_t;
#define dword_1045B818 XI_EA(dword_1045B818_t, 0x1045B818u)
typedef _DWORD dword_1045B81C_t;
#define dword_1045B81C XI_EA(dword_1045B81C_t, 0x1045B81Cu)
typedef int g_boneWeightMatrixTable_t;
#define g_boneWeightMatrixTable XI_EA(g_boneWeightMatrixTable_t, 0x1045EC20u)
typedef int g_pSharedSkeleton_t[];
#define g_pSharedSkeleton XI_EA(g_pSharedSkeleton_t, 0x1045EC24u)
typedef int g_boneTouched_t[256];
#define g_boneTouched XI_EA(g_boneTouched_t, 0x1045EC28u)
typedef float g_poseScratch_t[];
#define g_poseScratch XI_EA(g_poseScratch_t, 0x1045F030u)
typedef _DWORD g_poseTranslation_t[3];
#define g_poseTranslation XI_EA(g_poseTranslation_t, 0x1045F040u)
typedef _DWORD g_poseScale_t[3];
#define g_poseScale XI_EA(g_poseScale_t, 0x1045F04Cu)
typedef _BYTE unk_1045F064_t;
#define unk_1045F064 XI_EA(unk_1045F064_t, 0x1045F064u)
typedef int dword_10463950_t;
#define dword_10463950 XI_EA(dword_10463950_t, 0x10463950u)
typedef int dword_1047B950_t;
#define dword_1047B950 XI_EA(dword_1047B950_t, 0x1047B950u)
typedef int dword_1047B954_t;
#define dword_1047B954 XI_EA(dword_1047B954_t, 0x1047B954u)
typedef int g_sceneCaptureTex_t;
#define g_sceneCaptureTex XI_EA(g_sceneCaptureTex_t, 0x1047B958u)
typedef int g_sceneCaptureTexEye2_t;
#define g_sceneCaptureTexEye2 XI_EA(g_sceneCaptureTexEye2_t, 0x1047B95Cu)
typedef _DWORD g_sceneCaptureDepth_t;
#define g_sceneCaptureDepth XI_EA(g_sceneCaptureDepth_t, 0x1047B960u)
typedef _DWORD g_sceneCaptureDepthEye2_t;
#define g_sceneCaptureDepthEye2 XI_EA(g_sceneCaptureDepthEye2_t, 0x1047B964u)
typedef type_info * g_pTextureMng_t;
#define g_pTextureMng XI_EA(g_pTextureMng_t, 0x1047B970u)
typedef int g_pVbMng_t;
#define g_pVbMng XI_EA(g_pVbMng_t, 0x1047B974u)
typedef int g_pVertexBufferListHead_t;
#define g_pVertexBufferListHead XI_EA(g_pVertexBufferListHead_t, 0x1047B978u)
typedef char Sky_DrawDomeViewCentered_staticGuard_t;
#define Sky_DrawDomeViewCentered_staticGuard XI_EA(Sky_DrawDomeViewCentered_staticGuard_t, 0x1047B984u)
typedef char Sky_DrawDome_staticGuard_t;
#define Sky_DrawDome_staticGuard XI_EA(Sky_DrawDome_staticGuard_t, 0x1047B985u)
typedef char Sky_DrawDome_stateBlock_t[1];
#define Sky_DrawDome_stateBlock XI_EA(Sky_DrawDome_stateBlock_t, 0x1047B988u)
typedef char Sky_DrawDomeViewCentered_stateBlock_t[1];
#define Sky_DrawDomeViewCentered_stateBlock XI_EA(Sky_DrawDomeViewCentered_stateBlock_t, 0x1047B994u)
typedef char byte_1047B9E4_t;
#define byte_1047B9E4 XI_EA(byte_1047B9E4_t, 0x1047B9E4u)
typedef _DWORD dword_1047B9E8_t[3];
#define dword_1047B9E8 XI_EA(dword_1047B9E8_t, 0x1047B9E8u)
typedef _DWORD dword_1047BA1C_t[3];
#define dword_1047BA1C XI_EA(dword_1047BA1C_t, 0x1047BA1Cu)
typedef char byte_1047BAA4_t;
#define byte_1047BAA4 XI_EA(byte_1047BAA4_t, 0x1047BAA4u)
typedef char Font_DrawProjectedText3D_staticGuard_t;
#define Font_DrawProjectedText3D_staticGuard XI_EA(Font_DrawProjectedText3D_staticGuard_t, 0x1047BAC0u)
typedef _DWORD Font_DrawProjectedText3D_stateBlock_t[5];
#define Font_DrawProjectedText3D_stateBlock XI_EA(Font_DrawProjectedText3D_stateBlock_t, 0x1047BADCu)
typedef int dword_1047BB04_t;
#define dword_1047BB04 XI_EA(dword_1047BB04_t, 0x1047BB04u)
typedef int dword_1047BB08_t;
#define dword_1047BB08 XI_EA(dword_1047BB08_t, 0x1047BB08u)
typedef int dword_1047BB0C_t;
#define dword_1047BB0C XI_EA(dword_1047BB0C_t, 0x1047BB0Cu)
typedef int dword_1047BB10_t;
#define dword_1047BB10 XI_EA(dword_1047BB10_t, 0x1047BB10u)
typedef int dword_1047BB14_t;
#define dword_1047BB14 XI_EA(dword_1047BB14_t, 0x1047BB14u)
typedef int dword_1047BB18_t;
#define dword_1047BB18 XI_EA(dword_1047BB18_t, 0x1047BB18u)
typedef int dword_1047BB1C_t;
#define dword_1047BB1C XI_EA(dword_1047BB1C_t, 0x1047BB1Cu)
typedef int dword_1047BB20_t;
#define dword_1047BB20 XI_EA(dword_1047BB20_t, 0x1047BB20u)
typedef int dword_1047BB24_t;
#define dword_1047BB24 XI_EA(dword_1047BB24_t, 0x1047BB24u)
typedef int dword_1047BB28_t;
#define dword_1047BB28 XI_EA(dword_1047BB28_t, 0x1047BB28u)
typedef int dword_1047BB2C_t;
#define dword_1047BB2C XI_EA(dword_1047BB2C_t, 0x1047BB2Cu)
typedef int dword_1047BB30_t;
#define dword_1047BB30 XI_EA(dword_1047BB30_t, 0x1047BB30u)
typedef int dword_1047BB34_t;
#define dword_1047BB34 XI_EA(dword_1047BB34_t, 0x1047BB34u)
typedef int dword_1047BB38_t;
#define dword_1047BB38 XI_EA(dword_1047BB38_t, 0x1047BB38u)
typedef int dword_1047BB3C_t;
#define dword_1047BB3C XI_EA(dword_1047BB3C_t, 0x1047BB3Cu)
typedef char byte_1047BB40_t;
#define byte_1047BB40 XI_EA(byte_1047BB40_t, 0x1047BB40u)
typedef char byte_1047BB41_t;
#define byte_1047BB41 XI_EA(byte_1047BB41_t, 0x1047BB41u)
typedef char dword_1047BB44_t[1];
#define dword_1047BB44 XI_EA(dword_1047BB44_t, 0x1047BB44u)
typedef char dword_1047BB50_t[1];
#define dword_1047BB50 XI_EA(dword_1047BB50_t, 0x1047BB50u)
typedef _DWORD dword_1047BB5C_t[3];
#define dword_1047BB5C XI_EA(dword_1047BB5C_t, 0x1047BB5Cu)
typedef char CMoDistRingElem_AllocRingGeometry_staticGuard_t;
#define CMoDistRingElem_AllocRingGeometry_staticGuard XI_EA(CMoDistRingElem_AllocRingGeometry_staticGuard_t, 0x1047BB78u)
typedef _DWORD CMoDistRingElem_stateBlock_t[3];
#define CMoDistRingElem_stateBlock XI_EA(CMoDistRingElem_stateBlock_t, 0x1047BB7Cu)
typedef int g_pMoAccumTarget_t;
#define g_pMoAccumTarget XI_EA(g_pMoAccumTarget_t, 0x1047BB88u)
typedef int g_cubemapSpecName_t;
#define g_cubemapSpecName XI_EA(g_cubemapSpecName_t, 0x1047BB90u)
typedef int g_pDynVB_FVF42_t;
#define g_pDynVB_FVF42 XI_EA(g_pDynVB_FVF42_t, 0x1047BBA0u)
typedef int g_pDynVB_FVF44_t;
#define g_pDynVB_FVF44 XI_EA(g_pDynVB_FVF44_t, 0x1047BBA4u)
typedef int g_pDynVB_FVF144_t;
#define g_pDynVB_FVF144 XI_EA(g_pDynVB_FVF144_t, 0x1047BBA8u)
typedef int g_pDynVB_FVF142_t;
#define g_pDynVB_FVF142 XI_EA(g_pDynVB_FVF142_t, 0x1047BBACu)
typedef int g_pDynVB_FVF152_t;
#define g_pDynVB_FVF152 XI_EA(g_pDynVB_FVF152_t, 0x1047BBB0u)
typedef int g_pMoCamera_t;
#define g_pMoCamera XI_EA(g_pMoCamera_t, 0x1047BBB4u)
typedef char dword_1047BBB8_t[1];
#define dword_1047BBB8 XI_EA(dword_1047BBB8_t, 0x1047BBB8u)
typedef int dword_1047BBDC_t;
#define dword_1047BBDC XI_EA(dword_1047BBDC_t, 0x1047BBDCu)
typedef int dword_1047BBE0_t;
#define dword_1047BBE0 XI_EA(dword_1047BBE0_t, 0x1047BBE0u)
typedef char byte_1047BBE4_t;
#define byte_1047BBE4 XI_EA(byte_1047BBE4_t, 0x1047BBE4u)
typedef int dword_1047BBF8_t;
#define dword_1047BBF8 XI_EA(dword_1047BBF8_t, 0x1047BBF8u)
typedef int g_scratchValue_t;
#define g_scratchValue XI_EA(g_scratchValue_t, 0x1047BC0Cu)
typedef int dword_1047BC28_t;
#define dword_1047BC28 XI_EA(dword_1047BC28_t, 0x1047BC28u)
typedef int g_countdown_1047BC6C_t;
#define g_countdown_1047BC6C XI_EA(g_countdown_1047BC6C_t, 0x1047BC6Cu)
typedef int dword_1047BC94_t;
#define dword_1047BC94 XI_EA(dword_1047BC94_t, 0x1047BC94u)
typedef int g_pResourceMng_t;
#define g_pResourceMng XI_EA(g_pResourceMng_t, 0x1047BFA8u)
typedef int g_pDxStatusMng_t;
#define g_pDxStatusMng XI_EA(g_pDxStatusMng_t, 0x1047BFF4u)
typedef int g_pVbPool_t;
#define g_pVbPool XI_EA(g_pVbPool_t, 0x1047BFF8u)
typedef int dword_1047BFFC_t;
#define dword_1047BFFC XI_EA(dword_1047BFFC_t, 0x1047BFFCu)
typedef int g_pOcclusionMng_t;
#define g_pOcclusionMng XI_EA(g_pOcclusionMng_t, 0x1047C004u)
typedef char dword_1047CAD0_t[1];
#define dword_1047CAD0 XI_EA(dword_1047CAD0_t, 0x1047CAD0u)
typedef char byte_1047CADC_t;
#define byte_1047CADC XI_EA(byte_1047CADC_t, 0x1047CADCu)
typedef char dword_1047CAE0_t[1];
#define dword_1047CAE0 XI_EA(dword_1047CAE0_t, 0x1047CAE0u)
typedef int g_identityMatrix_1047CC80_m00_t;
#define g_identityMatrix_1047CC80_m00 XI_EA(g_identityMatrix_1047CC80_m00_t, 0x1047CC80u)
typedef int g_identityMatrix_1047CC80_m01_t;
#define g_identityMatrix_1047CC80_m01 XI_EA(g_identityMatrix_1047CC80_m01_t, 0x1047CC84u)
typedef int g_identityMatrix_1047CC80_m02_t;
#define g_identityMatrix_1047CC80_m02 XI_EA(g_identityMatrix_1047CC80_m02_t, 0x1047CC88u)
typedef int g_identityMatrix_1047CC80_m03_t;
#define g_identityMatrix_1047CC80_m03 XI_EA(g_identityMatrix_1047CC80_m03_t, 0x1047CC8Cu)
typedef int g_identityMatrix_1047CC80_m10_t;
#define g_identityMatrix_1047CC80_m10 XI_EA(g_identityMatrix_1047CC80_m10_t, 0x1047CC90u)
typedef int g_identityMatrix_1047CC80_m11_t;
#define g_identityMatrix_1047CC80_m11 XI_EA(g_identityMatrix_1047CC80_m11_t, 0x1047CC94u)
typedef int g_identityMatrix_1047CC80_m12_t;
#define g_identityMatrix_1047CC80_m12 XI_EA(g_identityMatrix_1047CC80_m12_t, 0x1047CC98u)
typedef int g_identityMatrix_1047CC80_m13_t;
#define g_identityMatrix_1047CC80_m13 XI_EA(g_identityMatrix_1047CC80_m13_t, 0x1047CC9Cu)
typedef int g_identityMatrix_1047CC80_m20_t;
#define g_identityMatrix_1047CC80_m20 XI_EA(g_identityMatrix_1047CC80_m20_t, 0x1047CCA0u)
typedef int g_identityMatrix_1047CC80_m21_t;
#define g_identityMatrix_1047CC80_m21 XI_EA(g_identityMatrix_1047CC80_m21_t, 0x1047CCA4u)
typedef int g_identityMatrix_1047CC80_m22_t;
#define g_identityMatrix_1047CC80_m22 XI_EA(g_identityMatrix_1047CC80_m22_t, 0x1047CCA8u)
typedef int g_identityMatrix_1047CC80_m23_t;
#define g_identityMatrix_1047CC80_m23 XI_EA(g_identityMatrix_1047CC80_m23_t, 0x1047CCACu)
typedef int g_identityMatrix_1047CC80_m30_t;
#define g_identityMatrix_1047CC80_m30 XI_EA(g_identityMatrix_1047CC80_m30_t, 0x1047CCB0u)
typedef int g_identityMatrix_1047CC80_m31_t;
#define g_identityMatrix_1047CC80_m31 XI_EA(g_identityMatrix_1047CC80_m31_t, 0x1047CCB4u)
typedef int g_identityMatrix_1047CC80_m32_t;
#define g_identityMatrix_1047CC80_m32 XI_EA(g_identityMatrix_1047CC80_m32_t, 0x1047CCB8u)
typedef int g_identityMatrix_1047CC80_m33_t;
#define g_identityMatrix_1047CC80_m33 XI_EA(g_identityMatrix_1047CC80_m33_t, 0x1047CCBCu)
typedef char CMoProcessor_InitRotationMatrices_staticGuard_t;
#define CMoProcessor_InitRotationMatrices_staticGuard XI_EA(CMoProcessor_InitRotationMatrices_staticGuard_t, 0x1047CF31u)
typedef CMoProcessor * g_pProcessor_t;
#define g_pProcessor XI_EA(g_pProcessor_t, 0x1047CF7Cu)
typedef int g_pMeshResourceMng_t;
#define g_pMeshResourceMng XI_EA(g_pMeshResourceMng_t, 0x1047D128u)
typedef char byte_1047D12C_t;
#define byte_1047D12C XI_EA(byte_1047D12C_t, 0x1047D12Cu)
typedef int g_resourceFindDepth_t;
#define g_resourceFindDepth XI_EA(g_resourceFindDepth_t, 0x1047D130u)
typedef char g_resourcesDirty_t;
#define g_resourcesDirty XI_EA(g_resourcesDirty_t, 0x1047D13Cu)
typedef char byte_1047D13D_t;
#define byte_1047D13D XI_EA(byte_1047D13D_t, 0x1047D13Du)
typedef int dword_1047D140_t;
#define dword_1047D140 XI_EA(dword_1047D140_t, 0x1047D140u)
typedef int dword_1047D14C_t;
#define dword_1047D14C XI_EA(dword_1047D14C_t, 0x1047D14Cu)
typedef int g_cubeMapTargetHeight_t;
#define g_cubeMapTargetHeight XI_EA(g_cubeMapTargetHeight_t, 0x1047D150u)
typedef int g_pCubeMapTarget_t;
#define g_pCubeMapTarget XI_EA(g_pCubeMapTarget_t, 0x1047D158u)
typedef int dword_1047D15C_t[3];
#define dword_1047D15C XI_EA(dword_1047D15C_t, 0x1047D15Cu)
typedef int dword_1047D168_t[3];
#define dword_1047D168 XI_EA(dword_1047D168_t, 0x1047D168u)
typedef int g_cubeMapTargetWidth_t;
#define g_cubeMapTargetWidth XI_EA(g_cubeMapTargetWidth_t, 0x1047D200u)
typedef int g_pActorDrawListHead_t;
#define g_pActorDrawListHead XI_EA(g_pActorDrawListHead_t, 0x1047D578u)
typedef char g_actorNameBuffer_t;
#define g_actorNameBuffer XI_EA(g_actorNameBuffer_t, 0x1047D580u)
typedef _DWORD g_targetActorHandle_t[3];
#define g_targetActorHandle XI_EA(g_targetActorHandle_t, 0x1047D600u)
typedef int dword_1047D688_t;
#define dword_1047D688 XI_EA(dword_1047D688_t, 0x1047D688u)
typedef int dword_1047D68C_t;
#define dword_1047D68C XI_EA(dword_1047D68C_t, 0x1047D68Cu)
typedef char byte_1047D694_t;
#define byte_1047D694 XI_EA(byte_1047D694_t, 0x1047D694u)
typedef char byte_1047D695_t;
#define byte_1047D695 XI_EA(byte_1047D695_t, 0x1047D695u)
typedef int dword_10480AF0_t[16];
#define dword_10480AF0 XI_EA(dword_10480AF0_t, 0x10480AF0u)
typedef _DWORD dword_10487F58_t[3];
#define dword_10487F58 XI_EA(dword_10487F58_t, 0x10487F58u)
typedef char byte_10487F80_t;
#define byte_10487F80 XI_EA(byte_10487F80_t, 0x10487F80u)
typedef char byte_10487F81_t;
#define byte_10487F81 XI_EA(byte_10487F81_t, 0x10487F81u)
typedef int g_nearestLight_t;
#define g_nearestLight XI_EA(g_nearestLight_t, 0x1048A308u)
typedef int g_lightLookAtTarget_t;
#define g_lightLookAtTarget XI_EA(g_lightLookAtTarget_t, 0x1048A30Cu)
typedef int dword_1048A310_t;
#define dword_1048A310 XI_EA(dword_1048A310_t, 0x1048A310u)
typedef int dword_1048A314_t;
#define dword_1048A314 XI_EA(dword_1048A314_t, 0x1048A314u)
typedef int g_lightLookAtEye_t;
#define g_lightLookAtEye XI_EA(g_lightLookAtEye_t, 0x1048A318u)
typedef int dword_1048A31C_t;
#define dword_1048A31C XI_EA(dword_1048A31C_t, 0x1048A31Cu)
typedef int dword_1048A320_t;
#define dword_1048A320 XI_EA(dword_1048A320_t, 0x1048A320u)
typedef char PerActor_FourBucketStaggeredUpdate_staticGuards_t;
#define PerActor_FourBucketStaggeredUpdate_staticGuards XI_EA(PerActor_FourBucketStaggeredUpdate_staticGuards_t, 0x1048A328u)
typedef int dword_1048A32C_t;
#define dword_1048A32C XI_EA(dword_1048A32C_t, 0x1048A32Cu)
typedef int dword_1048A330_t;
#define dword_1048A330 XI_EA(dword_1048A330_t, 0x1048A330u)
typedef int dword_1048A334_t;
#define dword_1048A334 XI_EA(dword_1048A334_t, 0x1048A334u)
typedef int g_lightLookAtUp_t;
#define g_lightLookAtUp XI_EA(g_lightLookAtUp_t, 0x1048A338u)
typedef int dword_1048A33C_t;
#define dword_1048A33C XI_EA(dword_1048A33C_t, 0x1048A33Cu)
typedef int dword_1048A340_t;
#define dword_1048A340 XI_EA(dword_1048A340_t, 0x1048A340u)
typedef int dword_1048A344_t;
#define dword_1048A344 XI_EA(dword_1048A344_t, 0x1048A344u)
typedef int dword_1048A34C_t;
#define dword_1048A34C XI_EA(dword_1048A34C_t, 0x1048A34Cu)
typedef int g_actorUpdateCounter_t;
#define g_actorUpdateCounter XI_EA(g_actorUpdateCounter_t, 0x1048A350u)
typedef int g_actorUpdateInterval_t;
#define g_actorUpdateInterval XI_EA(g_actorUpdateInterval_t, 0x1048A354u)
typedef int g_actorUpdateBucketCountA_t[];
#define g_actorUpdateBucketCountA XI_EA(g_actorUpdateBucketCountA_t, 0x1048A358u)
typedef int g_actorUpdateBucketCountB_t[];
#define g_actorUpdateBucketCountB XI_EA(g_actorUpdateBucketCountB_t, 0x1048A368u)
typedef _DWORD dword_1048A37C_t[4];
#define dword_1048A37C XI_EA(dword_1048A37C_t, 0x1048A37Cu)
typedef int g_pActiveSkeletonActorRes_t;
#define g_pActiveSkeletonActorRes XI_EA(g_pActiveSkeletonActorRes_t, 0x1048A3D8u)
typedef D3DVIEWPORT8 g_viewport_1B70_t;
#define g_viewport_1B70 XI_EA(g_viewport_1B70_t, 0x104E1B70u)
typedef int g_textCursorX_t;
#define g_textCursorX XI_EA(g_textCursorX_t, 0x104E1B88u)
typedef int g_textCursorY_t;
#define g_textCursorY XI_EA(g_textCursorY_t, 0x104E1B8Cu)
typedef void * g_pFontGlyphs_t;
#define g_pFontGlyphs XI_EA(g_pFontGlyphs_t, 0x104E1B90u)
typedef int dword_104E1BF4_t;
#define dword_104E1BF4 XI_EA(dword_104E1BF4_t, 0x104E1BF4u)
typedef int g_shapeMng_t[17];
#define g_shapeMng XI_EA(g_shapeMng_t, 0x104E1BF8u)
typedef int dword_104E1D44_t;
#define dword_104E1D44 XI_EA(dword_104E1D44_t, 0x104E1D44u)
typedef _BYTE * g_pCursor_t;
#define g_pCursor XI_EA(g_pCursor_t, 0x104E1D4Cu)
typedef int g_pChatLog_t;
#define g_pChatLog XI_EA(g_pChatLog_t, 0x10578158u)
typedef int dword_1057815C_t;
#define dword_1057815C XI_EA(dword_1057815C_t, 0x1057815Cu)
typedef int dword_10578160_t;
#define dword_10578160 XI_EA(dword_10578160_t, 0x10578160u)
typedef int dword_105781F8_t;
#define dword_105781F8 XI_EA(dword_105781F8_t, 0x105781F8u)
typedef void * dword_1057847C_t;
#define dword_1057847C XI_EA(dword_1057847C_t, 0x1057847Cu)
typedef void * g_pTargetMng_t;
#define g_pTargetMng XI_EA(g_pTargetMng_t, 0x1057876Cu)
typedef __int16 g_menuMng_t[14];
#define g_menuMng XI_EA(g_menuMng_t, 0x10621838u)
typedef int g_screenRectTopLeft_t;
#define g_screenRectTopLeft XI_EA(g_screenRectTopLeft_t, 0x106218B4u)
typedef int g_screenRectBottomRight_t;
#define g_screenRectBottomRight XI_EA(g_screenRectBottomRight_t, 0x106218B8u)
typedef D3DVIEWPORT8 g_viewport_1904_t;
#define g_viewport_1904 XI_EA(g_viewport_1904_t, 0x10621904u)
typedef D3DVIEWPORT8 stru_1062191C_t;
#define stru_1062191C XI_EA(stru_1062191C_t, 0x1062191Cu)
typedef int g_upVerts_1988_t;
#define g_upVerts_1988 XI_EA(g_upVerts_1988_t, 0x10621988u)
typedef float g_upVerts_1988_v0_y_t;
#define g_upVerts_1988_v0_y XI_EA(g_upVerts_1988_v0_y_t, 0x1062198Cu)
typedef float g_upVerts_1988_v0_z_t;
#define g_upVerts_1988_v0_z XI_EA(g_upVerts_1988_v0_z_t, 0x10621990u)
typedef int g_upVerts_1988_v1_x_t;
#define g_upVerts_1988_v1_x XI_EA(g_upVerts_1988_v1_x_t, 0x10621998u)
typedef float g_upVerts_1988_v1_y_t;
#define g_upVerts_1988_v1_y XI_EA(g_upVerts_1988_v1_y_t, 0x1062199Cu)
typedef float g_upVerts_1988_v1_z_t;
#define g_upVerts_1988_v1_z XI_EA(g_upVerts_1988_v1_z_t, 0x106219A0u)
typedef int g_upVerts_1988_v2_x_t;
#define g_upVerts_1988_v2_x XI_EA(g_upVerts_1988_v2_x_t, 0x106219A8u)
typedef float g_upVerts_1988_v2_y_t;
#define g_upVerts_1988_v2_y XI_EA(g_upVerts_1988_v2_y_t, 0x106219ACu)
typedef float g_upVerts_1988_v2_z_t;
#define g_upVerts_1988_v2_z XI_EA(g_upVerts_1988_v2_z_t, 0x106219B0u)
typedef int g_upVerts_1988_v3_x_t;
#define g_upVerts_1988_v3_x XI_EA(g_upVerts_1988_v3_x_t, 0x106219B8u)
typedef float g_upVerts_1988_v3_y_t;
#define g_upVerts_1988_v3_y XI_EA(g_upVerts_1988_v3_y_t, 0x106219BCu)
typedef float g_upVerts_1988_v3_z_t;
#define g_upVerts_1988_v3_z XI_EA(g_upVerts_1988_v3_z_t, 0x106219C0u)
typedef int g_upVerts_1988_v4_x_t;
#define g_upVerts_1988_v4_x XI_EA(g_upVerts_1988_v4_x_t, 0x106219C8u)
typedef float g_upVerts_1988_v4_y_t;
#define g_upVerts_1988_v4_y XI_EA(g_upVerts_1988_v4_y_t, 0x106219CCu)
typedef float g_upVerts_1988_v4_z_t;
#define g_upVerts_1988_v4_z XI_EA(g_upVerts_1988_v4_z_t, 0x106219D0u)
typedef int g_upVerts_1988_v5_x_t;
#define g_upVerts_1988_v5_x XI_EA(g_upVerts_1988_v5_x_t, 0x106219D8u)
typedef float g_upVerts_1988_v5_y_t;
#define g_upVerts_1988_v5_y XI_EA(g_upVerts_1988_v5_y_t, 0x106219DCu)
typedef float g_upVerts_1988_v5_z_t;
#define g_upVerts_1988_v5_z XI_EA(g_upVerts_1988_v5_z_t, 0x106219E0u)
typedef int g_dbgBoxList_t;
#define g_dbgBoxList XI_EA(g_dbgBoxList_t, 0x10621A08u)
typedef int dword_10621A10_t;
#define dword_10621A10 XI_EA(dword_10621A10_t, 0x10621A10u)
typedef int g_upVerts_1A18_t;
#define g_upVerts_1A18 XI_EA(g_upVerts_1A18_t, 0x10621A18u)
typedef int g_upVerts_1A18_v0_y_t;
#define g_upVerts_1A18_v0_y XI_EA(g_upVerts_1A18_v0_y_t, 0x10621A1Cu)
typedef int g_upVerts_1A18_v0_z_t;
#define g_upVerts_1A18_v0_z XI_EA(g_upVerts_1A18_v0_z_t, 0x10621A20u)
typedef int g_upVerts_1A18_v0_diffuse_t;
#define g_upVerts_1A18_v0_diffuse XI_EA(g_upVerts_1A18_v0_diffuse_t, 0x10621A24u)
typedef int g_upVerts_1A18_v1_x_t;
#define g_upVerts_1A18_v1_x XI_EA(g_upVerts_1A18_v1_x_t, 0x10621A28u)
typedef int g_upVerts_1A18_v1_y_t;
#define g_upVerts_1A18_v1_y XI_EA(g_upVerts_1A18_v1_y_t, 0x10621A2Cu)
typedef int g_upVerts_1A18_v1_z_t;
#define g_upVerts_1A18_v1_z XI_EA(g_upVerts_1A18_v1_z_t, 0x10621A30u)
typedef int g_upVerts_1A18_v1_diffuse_t;
#define g_upVerts_1A18_v1_diffuse XI_EA(g_upVerts_1A18_v1_diffuse_t, 0x10621A34u)
typedef int g_upVerts_1A18_v2_x_t;
#define g_upVerts_1A18_v2_x XI_EA(g_upVerts_1A18_v2_x_t, 0x10621A38u)
typedef int g_upVerts_1A18_v2_y_t;
#define g_upVerts_1A18_v2_y XI_EA(g_upVerts_1A18_v2_y_t, 0x10621A3Cu)
typedef int g_upVerts_1A18_v2_z_t;
#define g_upVerts_1A18_v2_z XI_EA(g_upVerts_1A18_v2_z_t, 0x10621A40u)
typedef int g_upVerts_1A18_v2_diffuse_t;
#define g_upVerts_1A18_v2_diffuse XI_EA(g_upVerts_1A18_v2_diffuse_t, 0x10621A44u)
typedef int g_upVerts_1A18_v3_x_t;
#define g_upVerts_1A18_v3_x XI_EA(g_upVerts_1A18_v3_x_t, 0x10621A48u)
typedef int g_upVerts_1A18_v3_y_t;
#define g_upVerts_1A18_v3_y XI_EA(g_upVerts_1A18_v3_y_t, 0x10621A4Cu)
typedef int g_upVerts_1A18_v3_z_t;
#define g_upVerts_1A18_v3_z XI_EA(g_upVerts_1A18_v3_z_t, 0x10621A50u)
typedef int g_upVerts_1A18_v3_diffuse_t;
#define g_upVerts_1A18_v3_diffuse XI_EA(g_upVerts_1A18_v3_diffuse_t, 0x10621A54u)
typedef int g_upVerts_1A18_v4_x_t;
#define g_upVerts_1A18_v4_x XI_EA(g_upVerts_1A18_v4_x_t, 0x10621A58u)
typedef int g_upVerts_1A18_v4_y_t;
#define g_upVerts_1A18_v4_y XI_EA(g_upVerts_1A18_v4_y_t, 0x10621A5Cu)
typedef int g_upVerts_1A18_v4_z_t;
#define g_upVerts_1A18_v4_z XI_EA(g_upVerts_1A18_v4_z_t, 0x10621A60u)
typedef int g_upVerts_1A18_v4_diffuse_t;
#define g_upVerts_1A18_v4_diffuse XI_EA(g_upVerts_1A18_v4_diffuse_t, 0x10621A64u)
typedef int g_upVerts_1A68_t;
#define g_upVerts_1A68 XI_EA(g_upVerts_1A68_t, 0x10621A68u)
typedef int g_upVerts_1A68_v0_y_t;
#define g_upVerts_1A68_v0_y XI_EA(g_upVerts_1A68_v0_y_t, 0x10621A6Cu)
typedef int g_upVerts_1A68_v0_z_t;
#define g_upVerts_1A68_v0_z XI_EA(g_upVerts_1A68_v0_z_t, 0x10621A70u)
typedef int g_upVerts_1A68_v0_diffuse_t;
#define g_upVerts_1A68_v0_diffuse XI_EA(g_upVerts_1A68_v0_diffuse_t, 0x10621A74u)
typedef int g_upVerts_1A68_v1_x_t;
#define g_upVerts_1A68_v1_x XI_EA(g_upVerts_1A68_v1_x_t, 0x10621A78u)
typedef int g_upVerts_1A68_v1_y_t;
#define g_upVerts_1A68_v1_y XI_EA(g_upVerts_1A68_v1_y_t, 0x10621A7Cu)
typedef int g_upVerts_1A68_v1_z_t;
#define g_upVerts_1A68_v1_z XI_EA(g_upVerts_1A68_v1_z_t, 0x10621A80u)
typedef int g_upVerts_1A68_v1_diffuse_t;
#define g_upVerts_1A68_v1_diffuse XI_EA(g_upVerts_1A68_v1_diffuse_t, 0x10621A84u)
typedef int g_upVerts_1A68_v2_x_t;
#define g_upVerts_1A68_v2_x XI_EA(g_upVerts_1A68_v2_x_t, 0x10621A88u)
typedef int g_upVerts_1A68_v2_y_t;
#define g_upVerts_1A68_v2_y XI_EA(g_upVerts_1A68_v2_y_t, 0x10621A8Cu)
typedef int g_upVerts_1A68_v2_z_t;
#define g_upVerts_1A68_v2_z XI_EA(g_upVerts_1A68_v2_z_t, 0x10621A90u)
typedef int g_upVerts_1A68_v2_diffuse_t;
#define g_upVerts_1A68_v2_diffuse XI_EA(g_upVerts_1A68_v2_diffuse_t, 0x10621A94u)
typedef int g_upVerts_1A68_v3_x_t;
#define g_upVerts_1A68_v3_x XI_EA(g_upVerts_1A68_v3_x_t, 0x10621A98u)
typedef int g_upVerts_1A68_v3_y_t;
#define g_upVerts_1A68_v3_y XI_EA(g_upVerts_1A68_v3_y_t, 0x10621A9Cu)
typedef int g_upVerts_1A68_v3_z_t;
#define g_upVerts_1A68_v3_z XI_EA(g_upVerts_1A68_v3_z_t, 0x10621AA0u)
typedef int g_upVerts_1A68_v3_diffuse_t;
#define g_upVerts_1A68_v3_diffuse XI_EA(g_upVerts_1A68_v3_diffuse_t, 0x10621AA4u)
typedef int g_upVerts_1A68_v4_x_t;
#define g_upVerts_1A68_v4_x XI_EA(g_upVerts_1A68_v4_x_t, 0x10621AA8u)
typedef int g_upVerts_1A68_v4_y_t;
#define g_upVerts_1A68_v4_y XI_EA(g_upVerts_1A68_v4_y_t, 0x10621AACu)
typedef int g_upVerts_1A68_v4_z_t;
#define g_upVerts_1A68_v4_z XI_EA(g_upVerts_1A68_v4_z_t, 0x10621AB0u)
typedef int g_upVerts_1A68_v4_diffuse_t;
#define g_upVerts_1A68_v4_diffuse XI_EA(g_upVerts_1A68_v4_diffuse_t, 0x10621AB4u)
typedef int g_upVerts_1AB8_t;
#define g_upVerts_1AB8 XI_EA(g_upVerts_1AB8_t, 0x10621AB8u)
typedef int g_upVerts_1AB8_v0_y_t;
#define g_upVerts_1AB8_v0_y XI_EA(g_upVerts_1AB8_v0_y_t, 0x10621ABCu)
typedef int g_upVerts_1AB8_v0_z_t;
#define g_upVerts_1AB8_v0_z XI_EA(g_upVerts_1AB8_v0_z_t, 0x10621AC0u)
typedef int g_upVerts_1AB8_v0_diffuse_t;
#define g_upVerts_1AB8_v0_diffuse XI_EA(g_upVerts_1AB8_v0_diffuse_t, 0x10621AC4u)
typedef int g_upVerts_1AB8_v1_x_t;
#define g_upVerts_1AB8_v1_x XI_EA(g_upVerts_1AB8_v1_x_t, 0x10621AC8u)
typedef int g_upVerts_1AB8_v1_y_t;
#define g_upVerts_1AB8_v1_y XI_EA(g_upVerts_1AB8_v1_y_t, 0x10621ACCu)
typedef int g_upVerts_1AB8_v1_z_t;
#define g_upVerts_1AB8_v1_z XI_EA(g_upVerts_1AB8_v1_z_t, 0x10621AD0u)
typedef int g_upVerts_1AB8_v1_diffuse_t;
#define g_upVerts_1AB8_v1_diffuse XI_EA(g_upVerts_1AB8_v1_diffuse_t, 0x10621AD4u)
typedef int g_upVerts_1AB8_v2_x_t;
#define g_upVerts_1AB8_v2_x XI_EA(g_upVerts_1AB8_v2_x_t, 0x10621AD8u)
typedef int g_upVerts_1AB8_v2_y_t;
#define g_upVerts_1AB8_v2_y XI_EA(g_upVerts_1AB8_v2_y_t, 0x10621ADCu)
typedef int g_upVerts_1AB8_v2_z_t;
#define g_upVerts_1AB8_v2_z XI_EA(g_upVerts_1AB8_v2_z_t, 0x10621AE0u)
typedef int g_upVerts_1AB8_v2_diffuse_t;
#define g_upVerts_1AB8_v2_diffuse XI_EA(g_upVerts_1AB8_v2_diffuse_t, 0x10621AE4u)
typedef int g_upVerts_1AB8_v3_x_t;
#define g_upVerts_1AB8_v3_x XI_EA(g_upVerts_1AB8_v3_x_t, 0x10621AE8u)
typedef int g_upVerts_1AB8_v3_y_t;
#define g_upVerts_1AB8_v3_y XI_EA(g_upVerts_1AB8_v3_y_t, 0x10621AECu)
typedef int g_upVerts_1AB8_v3_z_t;
#define g_upVerts_1AB8_v3_z XI_EA(g_upVerts_1AB8_v3_z_t, 0x10621AF0u)
typedef int g_upVerts_1AB8_v3_diffuse_t;
#define g_upVerts_1AB8_v3_diffuse XI_EA(g_upVerts_1AB8_v3_diffuse_t, 0x10621AF4u)
typedef int g_upVerts_1AB8_v4_x_t;
#define g_upVerts_1AB8_v4_x XI_EA(g_upVerts_1AB8_v4_x_t, 0x10621AF8u)
typedef int g_upVerts_1AB8_v4_y_t;
#define g_upVerts_1AB8_v4_y XI_EA(g_upVerts_1AB8_v4_y_t, 0x10621AFCu)
typedef int g_upVerts_1AB8_v4_z_t;
#define g_upVerts_1AB8_v4_z XI_EA(g_upVerts_1AB8_v4_z_t, 0x10621B00u)
typedef int g_upVerts_1AB8_v4_diffuse_t;
#define g_upVerts_1AB8_v4_diffuse XI_EA(g_upVerts_1AB8_v4_diffuse_t, 0x10621B04u)
typedef int g_upVerts_1AB8_v5_x_t;
#define g_upVerts_1AB8_v5_x XI_EA(g_upVerts_1AB8_v5_x_t, 0x10621B08u)
typedef int g_upVerts_1AB8_v5_y_t;
#define g_upVerts_1AB8_v5_y XI_EA(g_upVerts_1AB8_v5_y_t, 0x10621B0Cu)
typedef int g_upVerts_1AB8_v5_z_t;
#define g_upVerts_1AB8_v5_z XI_EA(g_upVerts_1AB8_v5_z_t, 0x10621B10u)
typedef int g_upVerts_1AB8_v5_diffuse_t;
#define g_upVerts_1AB8_v5_diffuse XI_EA(g_upVerts_1AB8_v5_diffuse_t, 0x10621B14u)
typedef int g_upVerts_1AB8_v6_x_t;
#define g_upVerts_1AB8_v6_x XI_EA(g_upVerts_1AB8_v6_x_t, 0x10621B18u)
typedef int g_upVerts_1AB8_v6_y_t;
#define g_upVerts_1AB8_v6_y XI_EA(g_upVerts_1AB8_v6_y_t, 0x10621B1Cu)
typedef int g_upVerts_1AB8_v6_z_t;
#define g_upVerts_1AB8_v6_z XI_EA(g_upVerts_1AB8_v6_z_t, 0x10621B20u)
typedef int g_upVerts_1AB8_v6_diffuse_t;
#define g_upVerts_1AB8_v6_diffuse XI_EA(g_upVerts_1AB8_v6_diffuse_t, 0x10621B24u)
typedef int g_upVerts_1AB8_v7_x_t;
#define g_upVerts_1AB8_v7_x XI_EA(g_upVerts_1AB8_v7_x_t, 0x10621B28u)
typedef int g_upVerts_1AB8_v7_y_t;
#define g_upVerts_1AB8_v7_y XI_EA(g_upVerts_1AB8_v7_y_t, 0x10621B2Cu)
typedef int g_upVerts_1AB8_v7_z_t;
#define g_upVerts_1AB8_v7_z XI_EA(g_upVerts_1AB8_v7_z_t, 0x10621B30u)
typedef int g_upVerts_1AB8_v7_diffuse_t;
#define g_upVerts_1AB8_v7_diffuse XI_EA(g_upVerts_1AB8_v7_diffuse_t, 0x10621B34u)
typedef int g_zbiasRestore_t;
#define g_zbiasRestore XI_EA(g_zbiasRestore_t, 0x10621C00u)
typedef _BYTE c_div2000Magic_t;
#define c_div2000Magic XI_EA(c_div2000Magic_t, 0x10624DD3u)
typedef int g_zbiasOverlay_t;
#define g_zbiasOverlay XI_EA(g_zbiasOverlay_t, 0x10631C88u)
typedef int g_upVerts_1C90_t;
#define g_upVerts_1C90 XI_EA(g_upVerts_1C90_t, 0x10631C90u)
typedef float g_upVerts_1C90_v0_y_t;
#define g_upVerts_1C90_v0_y XI_EA(g_upVerts_1C90_v0_y_t, 0x10631C94u)
typedef float g_upVerts_1C90_v0_z_t;
#define g_upVerts_1C90_v0_z XI_EA(g_upVerts_1C90_v0_z_t, 0x10631C98u)
typedef int g_upVerts_1C90_v0_diffuse_t;
#define g_upVerts_1C90_v0_diffuse XI_EA(g_upVerts_1C90_v0_diffuse_t, 0x10631C9Cu)
typedef int g_upVerts_1C90_v0_u_t;
#define g_upVerts_1C90_v0_u XI_EA(g_upVerts_1C90_v0_u_t, 0x10631CA0u)
typedef int g_upVerts_1C90_v0_v_t;
#define g_upVerts_1C90_v0_v XI_EA(g_upVerts_1C90_v0_v_t, 0x10631CA4u)
typedef int g_upVerts_1C90_v1_x_t;
#define g_upVerts_1C90_v1_x XI_EA(g_upVerts_1C90_v1_x_t, 0x10631CA8u)
typedef float g_upVerts_1C90_v1_y_t;
#define g_upVerts_1C90_v1_y XI_EA(g_upVerts_1C90_v1_y_t, 0x10631CACu)
typedef float g_upVerts_1C90_v1_z_t;
#define g_upVerts_1C90_v1_z XI_EA(g_upVerts_1C90_v1_z_t, 0x10631CB0u)
typedef int g_upVerts_1C90_v1_diffuse_t;
#define g_upVerts_1C90_v1_diffuse XI_EA(g_upVerts_1C90_v1_diffuse_t, 0x10631CB4u)
typedef int g_upVerts_1C90_v1_u_t;
#define g_upVerts_1C90_v1_u XI_EA(g_upVerts_1C90_v1_u_t, 0x10631CB8u)
typedef int g_upVerts_1C90_v1_v_t;
#define g_upVerts_1C90_v1_v XI_EA(g_upVerts_1C90_v1_v_t, 0x10631CBCu)
typedef float g_upVerts_1C90_v2_x_t;
#define g_upVerts_1C90_v2_x XI_EA(g_upVerts_1C90_v2_x_t, 0x10631CC0u)
typedef float g_upVerts_1C90_v2_y_t;
#define g_upVerts_1C90_v2_y XI_EA(g_upVerts_1C90_v2_y_t, 0x10631CC4u)
typedef float g_upVerts_1C90_v2_z_t;
#define g_upVerts_1C90_v2_z XI_EA(g_upVerts_1C90_v2_z_t, 0x10631CC8u)
typedef int g_upVerts_1C90_v2_diffuse_t;
#define g_upVerts_1C90_v2_diffuse XI_EA(g_upVerts_1C90_v2_diffuse_t, 0x10631CCCu)
typedef int g_upVerts_1C90_v2_u_t;
#define g_upVerts_1C90_v2_u XI_EA(g_upVerts_1C90_v2_u_t, 0x10631CD0u)
typedef int g_upVerts_1C90_v2_v_t;
#define g_upVerts_1C90_v2_v XI_EA(g_upVerts_1C90_v2_v_t, 0x10631CD4u)
typedef float g_upVerts_1C90_v3_x_t;
#define g_upVerts_1C90_v3_x XI_EA(g_upVerts_1C90_v3_x_t, 0x10631CD8u)
typedef float g_upVerts_1C90_v3_y_t;
#define g_upVerts_1C90_v3_y XI_EA(g_upVerts_1C90_v3_y_t, 0x10631CDCu)
typedef float g_upVerts_1C90_v3_z_t;
#define g_upVerts_1C90_v3_z XI_EA(g_upVerts_1C90_v3_z_t, 0x10631CE0u)
typedef int g_upVerts_1C90_v3_diffuse_t;
#define g_upVerts_1C90_v3_diffuse XI_EA(g_upVerts_1C90_v3_diffuse_t, 0x10631CE4u)
typedef int g_upVerts_1C90_v3_u_t;
#define g_upVerts_1C90_v3_u XI_EA(g_upVerts_1C90_v3_u_t, 0x10631CE8u)
typedef int g_upVerts_1C90_v3_v_t;
#define g_upVerts_1C90_v3_v XI_EA(g_upVerts_1C90_v3_v_t, 0x10631CECu)
typedef float g_upVerts_1C90_v4_x_t;
#define g_upVerts_1C90_v4_x XI_EA(g_upVerts_1C90_v4_x_t, 0x10631CF0u)
typedef float g_upVerts_1C90_v4_y_t;
#define g_upVerts_1C90_v4_y XI_EA(g_upVerts_1C90_v4_y_t, 0x10631CF4u)
typedef float g_upVerts_1C90_v4_z_t;
#define g_upVerts_1C90_v4_z XI_EA(g_upVerts_1C90_v4_z_t, 0x10631CF8u)
typedef int g_upVerts_1C90_v4_diffuse_t;
#define g_upVerts_1C90_v4_diffuse XI_EA(g_upVerts_1C90_v4_diffuse_t, 0x10631CFCu)
typedef int g_upVerts_1C90_v4_u_t;
#define g_upVerts_1C90_v4_u XI_EA(g_upVerts_1C90_v4_u_t, 0x10631D00u)
typedef int g_upVerts_1C90_v4_v_t;
#define g_upVerts_1C90_v4_v XI_EA(g_upVerts_1C90_v4_v_t, 0x10631D04u)
typedef float g_upVerts_1C90_v5_x_t;
#define g_upVerts_1C90_v5_x XI_EA(g_upVerts_1C90_v5_x_t, 0x10631D08u)
typedef float g_upVerts_1C90_v5_y_t;
#define g_upVerts_1C90_v5_y XI_EA(g_upVerts_1C90_v5_y_t, 0x10631D0Cu)
typedef float g_upVerts_1C90_v5_z_t;
#define g_upVerts_1C90_v5_z XI_EA(g_upVerts_1C90_v5_z_t, 0x10631D10u)
typedef int g_upVerts_1C90_v5_diffuse_t;
#define g_upVerts_1C90_v5_diffuse XI_EA(g_upVerts_1C90_v5_diffuse_t, 0x10631D14u)
typedef int g_upVerts_1C90_v5_u_t;
#define g_upVerts_1C90_v5_u XI_EA(g_upVerts_1C90_v5_u_t, 0x10631D18u)
typedef int g_upVerts_1C90_v5_v_t;
#define g_upVerts_1C90_v5_v XI_EA(g_upVerts_1C90_v5_v_t, 0x10631D1Cu)
typedef int g_upVerts_1C90_v6_x_t;
#define g_upVerts_1C90_v6_x XI_EA(g_upVerts_1C90_v6_x_t, 0x10631D20u)
typedef float g_upVerts_1C90_v6_y_t;
#define g_upVerts_1C90_v6_y XI_EA(g_upVerts_1C90_v6_y_t, 0x10631D24u)
typedef float g_upVerts_1C90_v6_z_t;
#define g_upVerts_1C90_v6_z XI_EA(g_upVerts_1C90_v6_z_t, 0x10631D28u)
typedef int g_upVerts_1C90_v6_diffuse_t;
#define g_upVerts_1C90_v6_diffuse XI_EA(g_upVerts_1C90_v6_diffuse_t, 0x10631D2Cu)
typedef int g_upVerts_1C90_v6_u_t;
#define g_upVerts_1C90_v6_u XI_EA(g_upVerts_1C90_v6_u_t, 0x10631D30u)
typedef int g_upVerts_1C90_v6_v_t;
#define g_upVerts_1C90_v6_v XI_EA(g_upVerts_1C90_v6_v_t, 0x10631D34u)
typedef int g_upVerts_1C90_v7_x_t;
#define g_upVerts_1C90_v7_x XI_EA(g_upVerts_1C90_v7_x_t, 0x10631D38u)
typedef float g_upVerts_1C90_v7_y_t;
#define g_upVerts_1C90_v7_y XI_EA(g_upVerts_1C90_v7_y_t, 0x10631D3Cu)
typedef float g_upVerts_1C90_v7_z_t;
#define g_upVerts_1C90_v7_z XI_EA(g_upVerts_1C90_v7_z_t, 0x10631D40u)
typedef int g_upVerts_1C90_v7_diffuse_t;
#define g_upVerts_1C90_v7_diffuse XI_EA(g_upVerts_1C90_v7_diffuse_t, 0x10631D44u)
typedef int g_upVerts_1C90_v7_u_t;
#define g_upVerts_1C90_v7_u XI_EA(g_upVerts_1C90_v7_u_t, 0x10631D48u)
typedef int g_upVerts_1C90_v7_v_t;
#define g_upVerts_1C90_v7_v XI_EA(g_upVerts_1C90_v7_v_t, 0x10631D4Cu)
typedef int g_upVerts_1D50_t;
#define g_upVerts_1D50 XI_EA(g_upVerts_1D50_t, 0x10631D50u)
typedef float g_upVerts_1D50_v0_y_t;
#define g_upVerts_1D50_v0_y XI_EA(g_upVerts_1D50_v0_y_t, 0x10631D54u)
typedef float g_upVerts_1D50_v0_z_t;
#define g_upVerts_1D50_v0_z XI_EA(g_upVerts_1D50_v0_z_t, 0x10631D58u)
typedef int g_upVerts_1D50_v0_diffuse_t;
#define g_upVerts_1D50_v0_diffuse XI_EA(g_upVerts_1D50_v0_diffuse_t, 0x10631D5Cu)
typedef int g_upVerts_1D50_v0_u_t;
#define g_upVerts_1D50_v0_u XI_EA(g_upVerts_1D50_v0_u_t, 0x10631D60u)
typedef int g_upVerts_1D50_v0_v_t;
#define g_upVerts_1D50_v0_v XI_EA(g_upVerts_1D50_v0_v_t, 0x10631D64u)
typedef float g_upVerts_1D50_v1_x_t;
#define g_upVerts_1D50_v1_x XI_EA(g_upVerts_1D50_v1_x_t, 0x10631D68u)
typedef float g_upVerts_1D50_v1_y_t;
#define g_upVerts_1D50_v1_y XI_EA(g_upVerts_1D50_v1_y_t, 0x10631D6Cu)
typedef float g_upVerts_1D50_v1_z_t;
#define g_upVerts_1D50_v1_z XI_EA(g_upVerts_1D50_v1_z_t, 0x10631D70u)
typedef int g_upVerts_1D50_v1_diffuse_t;
#define g_upVerts_1D50_v1_diffuse XI_EA(g_upVerts_1D50_v1_diffuse_t, 0x10631D74u)
typedef int g_upVerts_1D50_v1_u_t;
#define g_upVerts_1D50_v1_u XI_EA(g_upVerts_1D50_v1_u_t, 0x10631D78u)
typedef int g_upVerts_1D50_v1_v_t;
#define g_upVerts_1D50_v1_v XI_EA(g_upVerts_1D50_v1_v_t, 0x10631D7Cu)
typedef float g_upVerts_1D50_v2_x_t;
#define g_upVerts_1D50_v2_x XI_EA(g_upVerts_1D50_v2_x_t, 0x10631D80u)
typedef float g_upVerts_1D50_v2_y_t;
#define g_upVerts_1D50_v2_y XI_EA(g_upVerts_1D50_v2_y_t, 0x10631D84u)
typedef float g_upVerts_1D50_v2_z_t;
#define g_upVerts_1D50_v2_z XI_EA(g_upVerts_1D50_v2_z_t, 0x10631D88u)
typedef int g_upVerts_1D50_v2_diffuse_t;
#define g_upVerts_1D50_v2_diffuse XI_EA(g_upVerts_1D50_v2_diffuse_t, 0x10631D8Cu)
typedef int g_upVerts_1D50_v2_u_t;
#define g_upVerts_1D50_v2_u XI_EA(g_upVerts_1D50_v2_u_t, 0x10631D90u)
typedef int g_upVerts_1D50_v2_v_t;
#define g_upVerts_1D50_v2_v XI_EA(g_upVerts_1D50_v2_v_t, 0x10631D94u)
typedef int g_upVerts_1D50_v3_x_t;
#define g_upVerts_1D50_v3_x XI_EA(g_upVerts_1D50_v3_x_t, 0x10631D98u)
typedef float g_upVerts_1D50_v3_y_t;
#define g_upVerts_1D50_v3_y XI_EA(g_upVerts_1D50_v3_y_t, 0x10631D9Cu)
typedef float g_upVerts_1D50_v3_z_t;
#define g_upVerts_1D50_v3_z XI_EA(g_upVerts_1D50_v3_z_t, 0x10631DA0u)
typedef int g_upVerts_1D50_v3_diffuse_t;
#define g_upVerts_1D50_v3_diffuse XI_EA(g_upVerts_1D50_v3_diffuse_t, 0x10631DA4u)
typedef int g_upVerts_1D50_v3_u_t;
#define g_upVerts_1D50_v3_u XI_EA(g_upVerts_1D50_v3_u_t, 0x10631DA8u)
typedef int g_upVerts_1D50_v3_v_t;
#define g_upVerts_1D50_v3_v XI_EA(g_upVerts_1D50_v3_v_t, 0x10631DACu)
typedef int g_upVerts_1D50_v4_x_t;
#define g_upVerts_1D50_v4_x XI_EA(g_upVerts_1D50_v4_x_t, 0x10631DB0u)
typedef float g_upVerts_1D50_v4_y_t;
#define g_upVerts_1D50_v4_y XI_EA(g_upVerts_1D50_v4_y_t, 0x10631DB4u)
typedef float g_upVerts_1D50_v4_z_t;
#define g_upVerts_1D50_v4_z XI_EA(g_upVerts_1D50_v4_z_t, 0x10631DB8u)
typedef int g_upVerts_1D50_v4_diffuse_t;
#define g_upVerts_1D50_v4_diffuse XI_EA(g_upVerts_1D50_v4_diffuse_t, 0x10631DBCu)
typedef int g_upVerts_1D50_v4_u_t;
#define g_upVerts_1D50_v4_u XI_EA(g_upVerts_1D50_v4_u_t, 0x10631DC0u)
typedef int g_upVerts_1D50_v4_v_t;
#define g_upVerts_1D50_v4_v XI_EA(g_upVerts_1D50_v4_v_t, 0x10631DC4u)
typedef int g_upVerts_1E10_t;
#define g_upVerts_1E10 XI_EA(g_upVerts_1E10_t, 0x10631E10u)
typedef float g_upVerts_1E10_v0_y_t;
#define g_upVerts_1E10_v0_y XI_EA(g_upVerts_1E10_v0_y_t, 0x10631E14u)
typedef float g_upVerts_1E10_v0_z_t;
#define g_upVerts_1E10_v0_z XI_EA(g_upVerts_1E10_v0_z_t, 0x10631E18u)
typedef int g_upVerts_1E10_v0_diffuse_t;
#define g_upVerts_1E10_v0_diffuse XI_EA(g_upVerts_1E10_v0_diffuse_t, 0x10631E1Cu)
typedef int g_upVerts_1E10_v0_u_t;
#define g_upVerts_1E10_v0_u XI_EA(g_upVerts_1E10_v0_u_t, 0x10631E20u)
typedef int g_upVerts_1E10_v0_v_t;
#define g_upVerts_1E10_v0_v XI_EA(g_upVerts_1E10_v0_v_t, 0x10631E24u)
typedef float g_upVerts_1E10_v1_x_t;
#define g_upVerts_1E10_v1_x XI_EA(g_upVerts_1E10_v1_x_t, 0x10631E28u)
typedef float g_upVerts_1E10_v1_y_t;
#define g_upVerts_1E10_v1_y XI_EA(g_upVerts_1E10_v1_y_t, 0x10631E2Cu)
typedef float g_upVerts_1E10_v1_z_t;
#define g_upVerts_1E10_v1_z XI_EA(g_upVerts_1E10_v1_z_t, 0x10631E30u)
typedef int g_upVerts_1E10_v1_diffuse_t;
#define g_upVerts_1E10_v1_diffuse XI_EA(g_upVerts_1E10_v1_diffuse_t, 0x10631E34u)
typedef int g_upVerts_1E10_v1_u_t;
#define g_upVerts_1E10_v1_u XI_EA(g_upVerts_1E10_v1_u_t, 0x10631E38u)
typedef int g_upVerts_1E10_v1_v_t;
#define g_upVerts_1E10_v1_v XI_EA(g_upVerts_1E10_v1_v_t, 0x10631E3Cu)
typedef float g_upVerts_1E10_v2_x_t;
#define g_upVerts_1E10_v2_x XI_EA(g_upVerts_1E10_v2_x_t, 0x10631E40u)
typedef float g_upVerts_1E10_v2_y_t;
#define g_upVerts_1E10_v2_y XI_EA(g_upVerts_1E10_v2_y_t, 0x10631E44u)
typedef float g_upVerts_1E10_v2_z_t;
#define g_upVerts_1E10_v2_z XI_EA(g_upVerts_1E10_v2_z_t, 0x10631E48u)
typedef int g_upVerts_1E10_v2_diffuse_t;
#define g_upVerts_1E10_v2_diffuse XI_EA(g_upVerts_1E10_v2_diffuse_t, 0x10631E4Cu)
typedef int g_upVerts_1E10_v2_u_t;
#define g_upVerts_1E10_v2_u XI_EA(g_upVerts_1E10_v2_u_t, 0x10631E50u)
typedef int g_upVerts_1E10_v2_v_t;
#define g_upVerts_1E10_v2_v XI_EA(g_upVerts_1E10_v2_v_t, 0x10631E54u)
typedef int g_upVerts_1E10_v3_x_t;
#define g_upVerts_1E10_v3_x XI_EA(g_upVerts_1E10_v3_x_t, 0x10631E58u)
typedef float g_upVerts_1E10_v3_y_t;
#define g_upVerts_1E10_v3_y XI_EA(g_upVerts_1E10_v3_y_t, 0x10631E5Cu)
typedef float g_upVerts_1E10_v3_z_t;
#define g_upVerts_1E10_v3_z XI_EA(g_upVerts_1E10_v3_z_t, 0x10631E60u)
typedef int g_upVerts_1E10_v3_diffuse_t;
#define g_upVerts_1E10_v3_diffuse XI_EA(g_upVerts_1E10_v3_diffuse_t, 0x10631E64u)
typedef int g_upVerts_1E10_v3_u_t;
#define g_upVerts_1E10_v3_u XI_EA(g_upVerts_1E10_v3_u_t, 0x10631E68u)
typedef int g_upVerts_1E10_v3_v_t;
#define g_upVerts_1E10_v3_v XI_EA(g_upVerts_1E10_v3_v_t, 0x10631E6Cu)
typedef int g_upVerts_1E10_v4_x_t;
#define g_upVerts_1E10_v4_x XI_EA(g_upVerts_1E10_v4_x_t, 0x10631E70u)
typedef float g_upVerts_1E10_v4_y_t;
#define g_upVerts_1E10_v4_y XI_EA(g_upVerts_1E10_v4_y_t, 0x10631E74u)
typedef float g_upVerts_1E10_v4_z_t;
#define g_upVerts_1E10_v4_z XI_EA(g_upVerts_1E10_v4_z_t, 0x10631E78u)
typedef int g_upVerts_1E10_v4_diffuse_t;
#define g_upVerts_1E10_v4_diffuse XI_EA(g_upVerts_1E10_v4_diffuse_t, 0x10631E7Cu)
typedef int g_upVerts_1E10_v4_u_t;
#define g_upVerts_1E10_v4_u XI_EA(g_upVerts_1E10_v4_u_t, 0x10631E80u)
typedef int g_upVerts_1E10_v4_v_t;
#define g_upVerts_1E10_v4_v XI_EA(g_upVerts_1E10_v4_v_t, 0x10631E84u)
typedef int g_boxIB_t;
#define g_boxIB XI_EA(g_boxIB_t, 0x10631F10u)
typedef int dword_10631F14_t;
#define dword_10631F14 XI_EA(dword_10631F14_t, 0x10631F14u)
typedef int g_zbiasPresetAux_t;
#define g_zbiasPresetAux XI_EA(g_zbiasPresetAux_t, 0x10631F18u)
typedef int g_boxPositionVB_t;
#define g_boxPositionVB XI_EA(g_boxPositionVB_t, 0x10631F20u)
typedef void * g_pZoneMap_t;
#define g_pZoneMap XI_EA(g_pZoneMap_t, 0x10631F24u)
typedef int g_zbiasZoneMesh_t;
#define g_zbiasZoneMesh XI_EA(g_zbiasZoneMesh_t, 0x10632038u)
typedef int g_boxStream1VB_t;
#define g_boxStream1VB XI_EA(g_boxStream1VB_t, 0x10639040u)
typedef _BYTE g_zoneProjVerts_t;
#define g_zoneProjVerts XI_EA(g_zoneProjVerts_t, 0x10639088u)
typedef _BYTE unk_106390B0_t;
#define unk_106390B0 XI_EA(unk_106390B0_t, 0x106390B0u)
typedef _BYTE unk_106390B4_t;
#define unk_106390B4 XI_EA(unk_106390B4_t, 0x106390B4u)
typedef _BYTE unk_106390C8_t;
#define unk_106390C8 XI_EA(unk_106390C8_t, 0x106390C8u)
typedef _BYTE unk_106390CC_t;
#define unk_106390CC XI_EA(unk_106390CC_t, 0x106390CCu)
typedef int dword_1065C4C8_t;
#define dword_1065C4C8 XI_EA(dword_1065C4C8_t, 0x1065C4C8u)
typedef int g_zbiasPreset_C4D0_t;
#define g_zbiasPreset_C4D0 XI_EA(g_zbiasPreset_C4D0_t, 0x1065C4D0u)
typedef void * g_pXiZone_t;
#define g_pXiZone XI_EA(g_pXiZone_t, 0x1065CB04u)
typedef int dword_1066279C_t;
#define dword_1066279C XI_EA(dword_1066279C_t, 0x1066279Cu)
typedef int dword_106627EC_t;
#define dword_106627EC XI_EA(dword_106627EC_t, 0x106627ECu)
typedef int dword_10662DA8_t;
#define dword_10662DA8 XI_EA(dword_10662DA8_t, 0x10662DA8u)
typedef void * ArgList_t;
#define ArgList XI_EA(ArgList_t, 0x10662DBCu)
typedef int g_pFileMng_t;
#define g_pFileMng XI_EA(g_pFileMng_t, 0x10662DCCu)
typedef int g_C_106631D4_t;
#define g_C_106631D4 XI_EA(g_C_106631D4_t, 0x106631D4u)
typedef int g_pMovie_t;
#define g_pMovie XI_EA(g_pMovie_t, 0x10666E7Cu)
typedef int g_pKeyPrompt_t;
#define g_pKeyPrompt XI_EA(g_pKeyPrompt_t, 0x10668FD8u)
typedef int dword_10669014_t;
#define dword_10669014 XI_EA(dword_10669014_t, 0x10669014u)
typedef int g_stateBlock_914C_t;
#define g_stateBlock_914C XI_EA(g_stateBlock_914C_t, 0x1066914Cu)
typedef int g_stateBlock_9150_t;
#define g_stateBlock_9150 XI_EA(g_stateBlock_9150_t, 0x10669150u)
typedef void * g_pStDancer_t;
#define g_pStDancer XI_EA(g_pStDancer_t, 0x106693FCu)
typedef int g_dancerMainMemSize_t;
#define g_dancerMainMemSize XI_EA(g_dancerMainMemSize_t, 0x10669400u)
typedef int g_dancerMainMemBase_t;
#define g_dancerMainMemBase XI_EA(g_dancerMainMemBase_t, 0x10669404u)
typedef int g_dancerMemBlock_t;
#define g_dancerMemBlock XI_EA(g_dancerMemBlock_t, 0x10669408u)
typedef int g_dancerWorkMemSize_t;
#define g_dancerWorkMemSize XI_EA(g_dancerWorkMemSize_t, 0x1066940Cu)
typedef int g_dancerWorkMemBase_t;
#define g_dancerWorkMemBase XI_EA(g_dancerWorkMemBase_t, 0x10669410u)
typedef IDirect3D8 * g_This_10669418_t;
#define g_This_10669418 XI_EA(g_This_10669418_t, 0x10669418u)
typedef IDirect3DDevice8 * g_pDancerDevice_t;
#define g_pDancerDevice XI_EA(g_pDancerDevice_t, 0x1066941Cu)
typedef char byte_10669420_t;
#define byte_10669420 XI_EA(byte_10669420_t, 0x10669420u)
typedef int dword_10669424_t;
#define dword_10669424 XI_EA(dword_10669424_t, 0x10669424u)
typedef int * g_pSqgxContext_t;
#define g_pSqgxContext XI_EA(g_pSqgxContext_t, 0x1099D200u)
typedef char byte_1099D218_t[];
#define byte_1099D218 XI_EA(byte_1099D218_t, 0x1099D218u)
typedef char byte_1099D219_t[];
#define byte_1099D219 XI_EA(byte_1099D219_t, 0x1099D219u)
typedef char byte_1099D21A_t[];
#define byte_1099D21A XI_EA(byte_1099D21A_t, 0x1099D21Au)
typedef char byte_1099D21B_t[1021];
#define byte_1099D21B XI_EA(byte_1099D21B_t, 0x1099D21Bu)
typedef int g_sqgxTex_name_t[];
#define g_sqgxTex_name XI_EA(g_sqgxTex_name_t, 0x1099D618u)
typedef int g_sqgxTex_width_t[];
#define g_sqgxTex_width XI_EA(g_sqgxTex_width_t, 0x1099D61Cu)
typedef int g_sqgxTex_height_t[];
#define g_sqgxTex_height XI_EA(g_sqgxTex_height_t, 0x1099D620u)
typedef int g_sqgxTex_kind_t[];
#define g_sqgxTex_kind XI_EA(g_sqgxTex_kind_t, 0x1099D624u)
typedef int g_sqgxTex_log2Width_t[];
#define g_sqgxTex_log2Width XI_EA(g_sqgxTex_log2Width_t, 0x1099D628u)
typedef int g_sqgxTex_log2Height_t[];
#define g_sqgxTex_log2Height XI_EA(g_sqgxTex_log2Height_t, 0x1099D62Cu)
typedef int g_sqgxTex_format_t[];
#define g_sqgxTex_format XI_EA(g_sqgxTex_format_t, 0x1099D630u)
typedef int g_sqgxTex_paletteSlot_t[];
#define g_sqgxTex_paletteSlot XI_EA(g_sqgxTex_paletteSlot_t, 0x1099D634u)
typedef IDirect3DTexture8 * g_sqgxTex_texture_t;
#define g_sqgxTex_texture XI_EA(g_sqgxTex_texture_t, 0x1099D638u)
typedef int g_sqgxTex_surface_t[5111];
#define g_sqgxTex_surface XI_EA(g_sqgxTex_surface_t, 0x1099D63Cu)
typedef int g_sqgxRenderStateDirty_t;
#define g_sqgxRenderStateDirty XI_EA(g_sqgxRenderStateDirty_t, 0x109A2618u)
typedef int g_counter_109A261C_t;
#define g_counter_109A261C XI_EA(g_counter_109A261C_t, 0x109A261Cu)
typedef char g_sqgxPaletteUsed_t[256];
#define g_sqgxPaletteUsed XI_EA(g_sqgxPaletteUsed_t, 0x109A2620u)
typedef int dword_109A2720_t;
#define dword_109A2720 XI_EA(dword_109A2720_t, 0x109A2720u)
typedef int g_sqgxViewportDirty_t;
#define g_sqgxViewportDirty XI_EA(g_sqgxViewportDirty_t, 0x109A2730u)
typedef int g_sqgxShaderDirty_t;
#define g_sqgxShaderDirty XI_EA(g_sqgxShaderDirty_t, 0x109A2740u)
typedef int g_counter_109A2744_t;
#define g_counter_109A2744 XI_EA(g_counter_109A2744_t, 0x109A2744u)
typedef int g_sceneRT_t[];
#define g_sceneRT XI_EA(g_sceneRT_t, 0x109A2798u)
typedef int g_sceneRT1_t;
#define g_sceneRT1 XI_EA(g_sceneRT1_t, 0x109A279Cu)
typedef char CMoCamera_GetSharedInstance_staticGuard_t;
#define CMoCamera_GetSharedInstance_staticGuard XI_EA(CMoCamera_GetSharedInstance_staticGuard_t, 0x109F81A4u)
typedef _BYTE g_pCameraSharedInstance_t;
#define g_pCameraSharedInstance XI_EA(g_pCameraSharedInstance_t, 0x109F81C0u)
