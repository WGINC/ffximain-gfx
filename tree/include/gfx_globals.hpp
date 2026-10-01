// gfx_globals.hpp -- game globals the graphics code uses, bound to their addresses.
#pragma once
#include "gfx_common.hpp"
typedef void (__stdcall *Sleep_t)(DWORD dwMilliseconds);
#define Sleep XI_EA(Sleep_t, 0x103290F8u)
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
typedef char * CShader_classdesc_t;
#define CShader_classdesc XI_EA(CShader_classdesc_t, 0x10329EC4u)
typedef int (*CShader_vtbl_t)();
#define CShader_vtbl XI_EA(CShader_vtbl_t, 0x10329ED0u)
typedef int (*CXiOpening_vtbl_t)();
#define CXiOpening_vtbl XI_EA(CXiOpening_vtbl_t, 0x10329FBCu)
typedef char * CYyModelDt_classdesc_t;
#define CYyModelDt_classdesc XI_EA(CYyModelDt_classdesc_t, 0x1032A6B8u)
typedef char * CYyModelBase_classdesc_t;
#define CYyModelBase_classdesc XI_EA(CYyModelBase_classdesc_t, 0x1032A6C4u)
typedef char * CYyModel_classdesc_t;
#define CYyModel_classdesc XI_EA(CYyModel_classdesc_t, 0x1032A6D0u)
typedef int (*CYyOs2VtxBuffer_vtbl_t)();
#define CYyOs2VtxBuffer_vtbl XI_EA(CYyOs2VtxBuffer_vtbl_t, 0x1032A7B0u)
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
typedef char * CXiSkeletonActor_classdesc_t;
#define CXiSkeletonActor_classdesc XI_EA(CXiSkeletonActor_classdesc_t, 0x10330EBCu)
typedef char * CXiSkeletonActorRes_classdesc_t;
#define CXiSkeletonActorRes_classdesc XI_EA(CXiSkeletonActorRes_classdesc_t, 0x10330EC8u)
typedef int (*CXiSkeletonActor_vtbl_ambiguous0_t)();
#define CXiSkeletonActor_vtbl_ambiguous0 XI_EA(CXiSkeletonActor_vtbl_ambiguous0_t, 0x10330F40u)
typedef int (*CXiSkeletonActorRes_vtbl_t)();
#define CXiSkeletonActorRes_vtbl XI_EA(CXiSkeletonActorRes_vtbl_t, 0x103313ACu)
typedef int dword_10333000_t[];
#define dword_10333000 XI_EA(dword_10333000_t, 0x10333000u)
typedef int dword_10333004_t[];
#define dword_10333004 XI_EA(dword_10333004_t, 0x10333004u)
typedef int dword_10333008_t[];
#define dword_10333008 XI_EA(dword_10333008_t, 0x10333008u)
typedef int (*CTsZoneMap_vtbl_t)();
#define CTsZoneMap_vtbl XI_EA(CTsZoneMap_vtbl_t, 0x10336748u)
typedef _BYTE unk_10341DBC_t;
#define unk_10341DBC XI_EA(unk_10341DBC_t, 0x10341DBCu)
typedef _BYTE unk_10349EF4_t;
#define unk_10349EF4 XI_EA(unk_10349EF4_t, 0x10349EF4u)
typedef _UNKNOWN * off_1034F698_t;
#define off_1034F698 XI_EA(off_1034F698_t, 0x1034F698u)
typedef char aDeviceLost_t[1];
#define aDeviceLost XI_EA(aDeviceLost_t, 0x1034F6A0u)
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
typedef char byte_1034F92C_t;
#define byte_1034F92C XI_EA(byte_1034F92C_t, 0x1034F92Cu)
typedef int g_stereoEyeOffset_t;
#define g_stereoEyeOffset XI_EA(g_stereoEyeOffset_t, 0x1034FFA0u)
typedef float flt_103501A8_t;
#define flt_103501A8 XI_EA(flt_103501A8_t, 0x103501A8u)
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
typedef char ClassName_t[1];
#define ClassName XI_EA(ClassName_t, 0x10350350u)
typedef char aFinalFantasyXi_t[1];
#define aFinalFantasyXi XI_EA(aFinalFantasyXi_t, 0x10350450u)
typedef _BYTE unk_1035055C_t;
#define unk_1035055C XI_EA(unk_1035055C_t, 0x1035055Cu)
typedef _BYTE unk_1035055E_t;
#define unk_1035055E XI_EA(unk_1035055E_t, 0x1035055Eu)
typedef char aAfbVersionErro_t[1];
#define aAfbVersionErro XI_EA(aAfbVersionErro_t, 0x103511B0u)
typedef int dword_103511CC_t[5];
#define dword_103511CC XI_EA(dword_103511CC_t, 0x103511CCu)
typedef int dword_103511E0_t[];
#define dword_103511E0 XI_EA(dword_103511E0_t, 0x103511E0u)
typedef char * off_10351514_t;
#define off_10351514 XI_EA(off_10351514_t, 0x10351514u)
typedef int dword_103515BC_t;
#define dword_103515BC XI_EA(dword_103515BC_t, 0x103515BCu)
typedef _BYTE g_vsDecl_1628_t;
#define g_vsDecl_1628 XI_EA(g_vsDecl_1628_t, 0x10351628u)
typedef int g_texFourccTable_t[];
#define g_texFourccTable XI_EA(g_texFourccTable_t, 0x10351C94u)
typedef _DWORD dword_10351D60_t[22];
#define dword_10351D60 XI_EA(dword_10351D60_t, 0x10351D60u)
typedef int dword_10351DB8_t[16];
#define dword_10351DB8 XI_EA(dword_10351DB8_t, 0x10351DB8u)
typedef _DWORD dword_10351DF8_t[21];
#define dword_10351DF8 XI_EA(dword_10351DF8_t, 0x10351DF8u)
typedef int dword_10351E4C_t[12];
#define dword_10351E4C XI_EA(dword_10351E4C_t, 0x10351E4Cu)
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
typedef _DWORD dword_10352D30_t[64];
#define dword_10352D30 XI_EA(dword_10352D30_t, 0x10352D30u)
typedef _DWORD dword_10353378_t[30];
#define dword_10353378 XI_EA(dword_10353378_t, 0x10353378u)
typedef char byte_1035F638_t[72];
#define byte_1035F638 XI_EA(byte_1035F638_t, 0x1035F638u)
typedef char byte_1035F680_t[88];
#define byte_1035F680 XI_EA(byte_1035F680_t, 0x1035F680u)
typedef char byte_1035F6D8_t[80];
#define byte_1035F6D8 XI_EA(byte_1035F6D8_t, 0x1035F6D8u)
typedef char byte_1035F728_t[80];
#define byte_1035F728 XI_EA(byte_1035F728_t, 0x1035F728u)
typedef char byte_1035F778_t[72];
#define byte_1035F778 XI_EA(byte_1035F778_t, 0x1035F778u)
typedef char byte_1035F7C0_t[72];
#define byte_1035F7C0 XI_EA(byte_1035F7C0_t, 0x1035F7C0u)
typedef char byte_1035F808_t[72];
#define byte_1035F808 XI_EA(byte_1035F808_t, 0x1035F808u)
typedef char byte_1035F870_t[64];
#define byte_1035F870 XI_EA(byte_1035F870_t, 0x1035F870u)
typedef char byte_1035F8B0_t[60];
#define byte_1035F8B0 XI_EA(byte_1035F8B0_t, 0x1035F8B0u)
typedef char byte_1035F8EC_t[60];
#define byte_1035F8EC XI_EA(byte_1035F8EC_t, 0x1035F8ECu)
typedef char byte_1035F928_t[60];
#define byte_1035F928 XI_EA(byte_1035F928_t, 0x1035F928u)
typedef char byte_1035F964_t[60];
#define byte_1035F964 XI_EA(byte_1035F964_t, 0x1035F964u)
typedef void * off_103601BC_t;
#define off_103601BC XI_EA(off_103601BC_t, 0x103601BCu)
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
typedef int dword_1036E4EC_t[];
#define dword_1036E4EC XI_EA(dword_1036E4EC_t, 0x1036E4ECu)
typedef int dword_1036E4F4_t[];
#define dword_1036E4F4 XI_EA(dword_1036E4F4_t, 0x1036E4F4u)
typedef __int16 word_1036E4F8_t[];
#define word_1036E4F8 XI_EA(word_1036E4F8_t, 0x1036E4F8u)
typedef __int16 word_1036E4FA_t[125];
#define word_1036E4FA XI_EA(word_1036E4FA_t, 0x1036E4FAu)
typedef char aAncAncScro_t[1];
#define aAncAncScro XI_EA(aAncAncScro_t, 0x1036E6C8u)
typedef char aMenuKeytops3_t[1];
#define aMenuKeytops3 XI_EA(aMenuKeytops3_t, 0x1036FFC8u)
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
typedef char aErrorCouldNotA_t[1];
#define aErrorCouldNotA XI_EA(aErrorCouldNotA_t, 0x103B4874u)
typedef char aErrorSqgxtextu_t[1];
#define aErrorSqgxtextu XI_EA(aErrorSqgxtextu_t, 0x103B48A8u)
typedef char aError_t[1];
#define aError XI_EA(aError_t, 0x103B48D0u)
typedef char aCreatingTextur_t[1];
#define aCreatingTextur XI_EA(aCreatingTextur_t, 0x103B48D8u)
typedef int dword_103B4910_t[1024];
#define dword_103B4910 XI_EA(dword_103B4910_t, 0x103B4910u)
typedef int dword_103B5910_t[];
#define dword_103B5910 XI_EA(dword_103B5910_t, 0x103B5910u)
typedef int dword_103B5914_t[2047];
#define dword_103B5914 XI_EA(dword_103B5914_t, 0x103B5914u)
typedef int g_sceneFadeMaskRT_t;
#define g_sceneFadeMaskRT XI_EA(g_sceneFadeMaskRT_t, 0x103BA68Cu)
typedef char aErrorInSetVert_t[1];
#define aErrorInSetVert XI_EA(aErrorInSetVert_t, 0x103BBC38u)
typedef char aErrorInSetStre_t[1];
#define aErrorInSetStre XI_EA(aErrorInSetStre_t, 0x103BBC54u)
typedef char aShadowVolumesO_0_t[1];
#define aShadowVolumesO_0 XI_EA(aShadowVolumesO_0_t, 0x103BD56Cu)
typedef char aShadowVolumesO_t[1];
#define aShadowVolumesO XI_EA(aShadowVolumesO_t, 0x103BD5A0u)
typedef void * dword_103D3C64_t;
#define dword_103D3C64 XI_EA(dword_103D3C64_t, 0x103D3C64u)
typedef CHAR Read_t;
#define Read XI_EA(Read_t, 0x103D3C84u)
typedef int dword_103D3CB8_t;
#define dword_103D3CB8 XI_EA(dword_103D3CB8_t, 0x103D3CB8u)
typedef D3DHolder * g_pD3DHolder_t;
#define g_pD3DHolder XI_EA(g_pD3DHolder_t, 0x103D3CBCu)
typedef CDx * g_pCDx_t;
#define g_pCDx XI_EA(g_pCDx_t, 0x1045666Cu)
typedef int g_pEnv_t;
#define g_pEnv XI_EA(g_pEnv_t, 0x104566E8u)
typedef float g_afterimageStrength_t;
#define g_afterimageStrength XI_EA(g_afterimageStrength_t, 0x1045689Cu)
typedef __int16 word_104568F8_t;
#define word_104568F8 XI_EA(word_104568F8_t, 0x104568F8u)
typedef std::codecvt_base * g_pCamera_t;
#define g_pCamera XI_EA(g_pCamera_t, 0x104568FCu)
typedef int dword_10456908_t;
#define dword_10456908 XI_EA(dword_10456908_t, 0x10456908u)
typedef int g_afterimageTex_t[];
#define g_afterimageTex XI_EA(g_afterimageTex_t, 0x10456910u)
typedef int dword_10456918_t[];
#define dword_10456918 XI_EA(dword_10456918_t, 0x10456918u)
typedef int dword_10456920_t;
#define dword_10456920 XI_EA(dword_10456920_t, 0x10456920u)
typedef int dword_10456924_t[];
#define dword_10456924 XI_EA(dword_10456924_t, 0x10456924u)
typedef int dword_10456928_t;
#define dword_10456928 XI_EA(dword_10456928_t, 0x10456928u)
typedef int dword_1045692C_t;
#define dword_1045692C XI_EA(dword_1045692C_t, 0x1045692Cu)
typedef int dword_10456930_t[];
#define dword_10456930 XI_EA(dword_10456930_t, 0x10456930u)
typedef int dword_10456934_t;
#define dword_10456934 XI_EA(dword_10456934_t, 0x10456934u)
typedef int g_feedbackTexHalfRes_t[];
#define g_feedbackTexHalfRes XI_EA(g_feedbackTexHalfRes_t, 0x10456938u)
typedef int dword_10456940_t[];
#define dword_10456940 XI_EA(dword_10456940_t, 0x10456940u)
typedef int dword_1045694C_t;
#define dword_1045694C XI_EA(dword_1045694C_t, 0x1045694Cu)
typedef int g_countdown_10456978_t;
#define g_countdown_10456978 XI_EA(g_countdown_10456978_t, 0x10456978u)
typedef int dword_10456984_t;
#define dword_10456984 XI_EA(dword_10456984_t, 0x10456984u)
typedef HWND hWnd_t;
#define hWnd XI_EA(hWnd_t, 0x104569A8u)
typedef HINSTANCE hmod_t;
#define hmod XI_EA(hmod_t, 0x104569ACu)
typedef int dword_104569B0_t;
#define dword_104569B0 XI_EA(dword_104569B0_t, 0x104569B0u)
typedef int dword_104569B4_t;
#define dword_104569B4 XI_EA(dword_104569B4_t, 0x104569B4u)
typedef int dword_104569B8_t;
#define dword_104569B8 XI_EA(dword_104569B8_t, 0x104569B8u)
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
typedef int dword_10456A74_t;
#define dword_10456A74 XI_EA(dword_10456A74_t, 0x10456A74u)
typedef int g_timeOfDayTintEnabled_t;
#define g_timeOfDayTintEnabled XI_EA(g_timeOfDayTintEnabled_t, 0x10456A7Cu)
typedef int dword_10456A80_t;
#define dword_10456A80 XI_EA(dword_10456A80_t, 0x10456A80u)
typedef int dword_10456A88_t;
#define dword_10456A88 XI_EA(dword_10456A88_t, 0x10456A88u)
typedef int dword_10456AA0_t;
#define dword_10456AA0 XI_EA(dword_10456AA0_t, 0x10456AA0u)
typedef int g_cfgMapCompression_t;
#define g_cfgMapCompression XI_EA(g_cfgMapCompression_t, 0x10456AB0u)
typedef int dword_10456ACC_t;
#define dword_10456ACC XI_EA(dword_10456ACC_t, 0x10456ACCu)
typedef int g_cfgTextureCompression_t;
#define g_cfgTextureCompression XI_EA(g_cfgTextureCompression_t, 0x10456AD0u)
typedef int dword_10456BF8_t;
#define dword_10456BF8 XI_EA(dword_10456BF8_t, 0x10456BF8u)
typedef int dword_10456C00_t;
#define dword_10456C00 XI_EA(dword_10456C00_t, 0x10456C00u)
typedef int yBottom_t;
#define yBottom XI_EA(yBottom_t, 0x10456C08u)
typedef int dword_10456D20_t;
#define dword_10456D20 XI_EA(dword_10456D20_t, 0x10456D20u)
typedef char byte_10456D48_t;
#define byte_10456D48 XI_EA(byte_10456D48_t, 0x10456D48u)
typedef int dword_10456D4C_t;
#define dword_10456D4C XI_EA(dword_10456D4C_t, 0x10456D4Cu)
typedef IDirect3DBaseTexture8 * pTexture_t;
#define pTexture XI_EA(pTexture_t, 0x10456D50u)
typedef int dword_10456D54_t;
#define dword_10456D54 XI_EA(dword_10456D54_t, 0x10456D54u)
typedef int dword_10456D58_t;
#define dword_10456D58 XI_EA(dword_10456D58_t, 0x10456D58u)
typedef int dword_10456D5C_t;
#define dword_10456D5C XI_EA(dword_10456D5C_t, 0x10456D5Cu)
typedef float flt_10456DD0_t;
#define flt_10456DD0 XI_EA(flt_10456DD0_t, 0x10456DD0u)
typedef float flt_10456DD4_t;
#define flt_10456DD4 XI_EA(flt_10456DD4_t, 0x10456DD4u)
typedef float flt_10456DD8_t;
#define flt_10456DD8 XI_EA(flt_10456DD8_t, 0x10456DD8u)
typedef float flt_10456DDC_t;
#define flt_10456DDC XI_EA(flt_10456DDC_t, 0x10456DDCu)
typedef int g_alignedBufferSize_t;
#define g_alignedBufferSize XI_EA(g_alignedBufferSize_t, 0x10456E40u)
typedef int dword_10456E44_t;
#define dword_10456E44 XI_EA(dword_10456E44_t, 0x10456E44u)
typedef int dword_10456E48_t;
#define dword_10456E48 XI_EA(dword_10456E48_t, 0x10456E48u)
typedef int g_vertexShader_6E4C_t;
#define g_vertexShader_6E4C XI_EA(g_vertexShader_6E4C_t, 0x10456E4Cu)
typedef int dword_10456E50_t;
#define dword_10456E50 XI_EA(dword_10456E50_t, 0x10456E50u)
typedef int dword_10456E54_t;
#define dword_10456E54 XI_EA(dword_10456E54_t, 0x10456E54u)
typedef int dword_10456E58_t;
#define dword_10456E58 XI_EA(dword_10456E58_t, 0x10456E58u)
typedef int dword_10456E5C_t;
#define dword_10456E5C XI_EA(dword_10456E5C_t, 0x10456E5Cu)
typedef int dword_10456E60_t;
#define dword_10456E60 XI_EA(dword_10456E60_t, 0x10456E60u)
typedef int dword_10456E64_t;
#define dword_10456E64 XI_EA(dword_10456E64_t, 0x10456E64u)
typedef int dword_10456E68_t;
#define dword_10456E68 XI_EA(dword_10456E68_t, 0x10456E68u)
typedef int dword_10456E74_t;
#define dword_10456E74 XI_EA(dword_10456E74_t, 0x10456E74u)
typedef int dword_10456E78_t;
#define dword_10456E78 XI_EA(dword_10456E78_t, 0x10456E78u)
typedef int dword_10456E7C_t;
#define dword_10456E7C XI_EA(dword_10456E7C_t, 0x10456E7Cu)
typedef int dword_10456E80_t;
#define dword_10456E80 XI_EA(dword_10456E80_t, 0x10456E80u)
typedef int dword_10456E84_t;
#define dword_10456E84 XI_EA(dword_10456E84_t, 0x10456E84u)
typedef int dword_10456E88_t;
#define dword_10456E88 XI_EA(dword_10456E88_t, 0x10456E88u)
typedef int dword_10456E8C_t;
#define dword_10456E8C XI_EA(dword_10456E8C_t, 0x10456E8Cu)
typedef char byte_104570F0_t;
#define byte_104570F0 XI_EA(byte_104570F0_t, 0x104570F0u)
typedef int dword_104570F4_t;
#define dword_104570F4 XI_EA(dword_104570F4_t, 0x104570F4u)
typedef char byte_104570FC_t;
#define byte_104570FC XI_EA(byte_104570FC_t, 0x104570FCu)
typedef char dword_10457108_t[1];
#define dword_10457108 XI_EA(dword_10457108_t, 0x10457108u)
typedef int g_vertexBuffer_7150_t;
#define g_vertexBuffer_7150 XI_EA(g_vertexBuffer_7150_t, 0x10457150u)
typedef int g_vertexBuffer_7154_t;
#define g_vertexBuffer_7154 XI_EA(g_vertexBuffer_7154_t, 0x10457154u)
typedef char dword_10457158_t[1];
#define dword_10457158 XI_EA(dword_10457158_t, 0x10457158u)
typedef char flt_10457198_t[1];
#define flt_10457198 XI_EA(flt_10457198_t, 0x10457198u)
typedef _DWORD unk_104571D8_t;
#define unk_104571D8 XI_EA(unk_104571D8_t, 0x104571D8u)
typedef unsigned __int8 unk_104571D8_as_u8_t;
#define unk_104571D8_as_u8 XI_EA(unk_104571D8_as_u8_t, 0x104571D8u)
typedef char byte_104571D9_t;
#define byte_104571D9 XI_EA(byte_104571D9_t, 0x104571D9u)
typedef char byte_104571DA_t;
#define byte_104571DA XI_EA(byte_104571DA_t, 0x104571DAu)
typedef char byte_104571DB_t;
#define byte_104571DB XI_EA(byte_104571DB_t, 0x104571DBu)
typedef _BYTE unk_104571E0_t;
#define unk_104571E0 XI_EA(unk_104571E0_t, 0x104571E0u)
typedef float flt_10457210_t;
#define flt_10457210 XI_EA(flt_10457210_t, 0x10457210u)
typedef float flt_10457214_t;
#define flt_10457214 XI_EA(flt_10457214_t, 0x10457214u)
typedef float flt_10457218_t;
#define flt_10457218 XI_EA(flt_10457218_t, 0x10457218u)
typedef _BYTE g_matrix_7228_t;
#define g_matrix_7228 XI_EA(g_matrix_7228_t, 0x10457228u)
typedef _DWORD g_matrix_7270_t[16];
#define g_matrix_7270 XI_EA(g_matrix_7270_t, 0x10457270u)
typedef char byte_104572B0_t;
#define byte_104572B0 XI_EA(byte_104572B0_t, 0x104572B0u)
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
typedef int dword_104574A8_t;
#define dword_104574A8 XI_EA(dword_104574A8_t, 0x104574A8u)
typedef int dword_104574AC_t;
#define dword_104574AC XI_EA(dword_104574AC_t, 0x104574ACu)
typedef char dword_104574B8_t[1];
#define dword_104574B8 XI_EA(dword_104574B8_t, 0x104574B8u)
typedef char dword_104574F8_t[1];
#define dword_104574F8 XI_EA(dword_104574F8_t, 0x104574F8u)
typedef char byte_10457538_t;
#define byte_10457538 XI_EA(byte_10457538_t, 0x10457538u)
typedef int g_vertexBuffer_7540_t;
#define g_vertexBuffer_7540 XI_EA(g_vertexBuffer_7540_t, 0x10457540u)
typedef int dword_10457550_t;
#define dword_10457550 XI_EA(dword_10457550_t, 0x10457550u)
typedef int dword_10457554_t[5];
#define dword_10457554 XI_EA(dword_10457554_t, 0x10457554u)
typedef float flt_10457568_t;
#define flt_10457568 XI_EA(flt_10457568_t, 0x10457568u)
typedef float flt_1045756C_t;
#define flt_1045756C XI_EA(flt_1045756C_t, 0x1045756Cu)
typedef float flt_10457570_t;
#define flt_10457570 XI_EA(flt_10457570_t, 0x10457570u)
typedef float flt_10457574_t;
#define flt_10457574 XI_EA(flt_10457574_t, 0x10457574u)
typedef int dword_1045B618_t;
#define dword_1045B618 XI_EA(dword_1045B618_t, 0x1045B618u)
typedef int dword_1045B61C_t;
#define dword_1045B61C XI_EA(dword_1045B61C_t, 0x1045B61Cu)
typedef int dword_1045B620_t;
#define dword_1045B620 XI_EA(dword_1045B620_t, 0x1045B620u)
typedef int dword_1045B62C_t;
#define dword_1045B62C XI_EA(dword_1045B62C_t, 0x1045B62Cu)
typedef int dword_1045B630_t;
#define dword_1045B630 XI_EA(dword_1045B630_t, 0x1045B630u)
typedef int dword_1045B648_t;
#define dword_1045B648 XI_EA(dword_1045B648_t, 0x1045B648u)
typedef int dword_1045B64C_t;
#define dword_1045B64C XI_EA(dword_1045B64C_t, 0x1045B64Cu)
typedef int dword_1045B650_t[16];
#define dword_1045B650 XI_EA(dword_1045B650_t, 0x1045B650u)
typedef int dword_1045B690_t[];
#define dword_1045B690 XI_EA(dword_1045B690_t, 0x1045B690u)
typedef int g_vertexShader_B6A0_t;
#define g_vertexShader_B6A0 XI_EA(g_vertexShader_B6A0_t, 0x1045B6A0u)
typedef __int16 word_1045B77A_t;
#define word_1045B77A XI_EA(word_1045B77A_t, 0x1045B77Au)
typedef char dword_1045B780_t[1];
#define dword_1045B780 XI_EA(dword_1045B780_t, 0x1045B780u)
typedef char flt_1045B7C0_t[1];
#define flt_1045B7C0 XI_EA(flt_1045B7C0_t, 0x1045B7C0u)
typedef int dword_1045B800_t;
#define dword_1045B800 XI_EA(dword_1045B800_t, 0x1045B800u)
typedef int dword_1045B804_t;
#define dword_1045B804 XI_EA(dword_1045B804_t, 0x1045B804u)
typedef int g_texturefactor_B808_t;
#define g_texturefactor_B808 XI_EA(g_texturefactor_B808_t, 0x1045B808u)
typedef float flt_1045B80C_t;
#define flt_1045B80C XI_EA(flt_1045B80C_t, 0x1045B80Cu)
typedef int dword_1045B810_t;
#define dword_1045B810 XI_EA(dword_1045B810_t, 0x1045B810u)
typedef __int16 word_1045B814_t;
#define word_1045B814 XI_EA(word_1045B814_t, 0x1045B814u)
typedef int g_boneWeightMatrixTable_t;
#define g_boneWeightMatrixTable XI_EA(g_boneWeightMatrixTable_t, 0x1045EC20u)
typedef int g_pSharedSkeleton_t[];
#define g_pSharedSkeleton XI_EA(g_pSharedSkeleton_t, 0x1045EC24u)
typedef int dword_1045EC28_t[256];
#define dword_1045EC28 XI_EA(dword_1045EC28_t, 0x1045EC28u)
typedef float g_poseScratch_t[];
#define g_poseScratch XI_EA(g_poseScratch_t, 0x1045F030u)
typedef _DWORD dword_1045F040_t[3];
#define dword_1045F040 XI_EA(dword_1045F040_t, 0x1045F040u)
typedef _DWORD dword_1045F04C_t[3];
#define dword_1045F04C XI_EA(dword_1045F04C_t, 0x1045F04Cu)
typedef _BYTE unk_1045F064_t;
#define unk_1045F064 XI_EA(unk_1045F064_t, 0x1045F064u)
typedef int dword_10463950_t;
#define dword_10463950 XI_EA(dword_10463950_t, 0x10463950u)
typedef int dword_1047B950_t;
#define dword_1047B950 XI_EA(dword_1047B950_t, 0x1047B950u)
typedef int dword_1047B954_t;
#define dword_1047B954 XI_EA(dword_1047B954_t, 0x1047B954u)
typedef int dword_1047B958_t;
#define dword_1047B958 XI_EA(dword_1047B958_t, 0x1047B958u)
typedef int dword_1047B95C_t;
#define dword_1047B95C XI_EA(dword_1047B95C_t, 0x1047B95Cu)
typedef int dword_1047B960_t;
#define dword_1047B960 XI_EA(dword_1047B960_t, 0x1047B960u)
typedef int dword_1047B964_t;
#define dword_1047B964 XI_EA(dword_1047B964_t, 0x1047B964u)
typedef type_info * g_pTextureMng_t;
#define g_pTextureMng XI_EA(g_pTextureMng_t, 0x1047B970u)
typedef int dword_1047B974_t;
#define dword_1047B974 XI_EA(dword_1047B974_t, 0x1047B974u)
typedef int g_pVertexBufferListHead_t;
#define g_pVertexBufferListHead XI_EA(g_pVertexBufferListHead_t, 0x1047B978u)
typedef char byte_1047B984_t;
#define byte_1047B984 XI_EA(byte_1047B984_t, 0x1047B984u)
typedef char byte_1047B985_t;
#define byte_1047B985 XI_EA(byte_1047B985_t, 0x1047B985u)
typedef char dword_1047B988_t[1];
#define dword_1047B988 XI_EA(dword_1047B988_t, 0x1047B988u)
typedef char dword_1047B994_t[1];
#define dword_1047B994 XI_EA(dword_1047B994_t, 0x1047B994u)
typedef char byte_1047BAC0_t;
#define byte_1047BAC0 XI_EA(byte_1047BAC0_t, 0x1047BAC0u)
typedef _DWORD dword_1047BADC_t[5];
#define dword_1047BADC XI_EA(dword_1047BADC_t, 0x1047BADCu)
typedef char byte_1047BB78_t;
#define byte_1047BB78 XI_EA(byte_1047BB78_t, 0x1047BB78u)
typedef _DWORD dword_1047BB7C_t[3];
#define dword_1047BB7C XI_EA(dword_1047BB7C_t, 0x1047BB7Cu)
typedef int dword_1047BB88_t;
#define dword_1047BB88 XI_EA(dword_1047BB88_t, 0x1047BB88u)
typedef int dword_1047BB90_t;
#define dword_1047BB90 XI_EA(dword_1047BB90_t, 0x1047BB90u)
typedef int dword_1047BBA0_t;
#define dword_1047BBA0 XI_EA(dword_1047BBA0_t, 0x1047BBA0u)
typedef int dword_1047BBA4_t;
#define dword_1047BBA4 XI_EA(dword_1047BBA4_t, 0x1047BBA4u)
typedef int dword_1047BBA8_t;
#define dword_1047BBA8 XI_EA(dword_1047BBA8_t, 0x1047BBA8u)
typedef int dword_1047BBAC_t;
#define dword_1047BBAC XI_EA(dword_1047BBAC_t, 0x1047BBACu)
typedef int dword_1047BBB0_t;
#define dword_1047BBB0 XI_EA(dword_1047BBB0_t, 0x1047BBB0u)
typedef int dword_1047BBB4_t;
#define dword_1047BBB4 XI_EA(dword_1047BBB4_t, 0x1047BBB4u)
typedef int g_scratchValue_t;
#define g_scratchValue XI_EA(g_scratchValue_t, 0x1047BC0Cu)
typedef int g_countdown_1047BC6C_t;
#define g_countdown_1047BC6C XI_EA(g_countdown_1047BC6C_t, 0x1047BC6Cu)
typedef int g_pResourceMng_t;
#define g_pResourceMng XI_EA(g_pResourceMng_t, 0x1047BFA8u)
typedef int dword_1047BFF4_t;
#define dword_1047BFF4 XI_EA(dword_1047BFF4_t, 0x1047BFF4u)
typedef int dword_1047BFF8_t;
#define dword_1047BFF8 XI_EA(dword_1047BFF8_t, 0x1047BFF8u)
typedef int dword_1047C004_t;
#define dword_1047C004 XI_EA(dword_1047C004_t, 0x1047C004u)
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
typedef char byte_1047CF31_t;
#define byte_1047CF31 XI_EA(byte_1047CF31_t, 0x1047CF31u)
typedef CMoProcessor * g_pProcessor_t;
#define g_pProcessor XI_EA(g_pProcessor_t, 0x1047CF7Cu)
typedef int g_pMeshResourceMng_t;
#define g_pMeshResourceMng XI_EA(g_pMeshResourceMng_t, 0x1047D128u)
typedef char byte_1047D12C_t;
#define byte_1047D12C XI_EA(byte_1047D12C_t, 0x1047D12Cu)
typedef int dword_1047D130_t;
#define dword_1047D130 XI_EA(dword_1047D130_t, 0x1047D130u)
typedef char byte_1047D13C_t;
#define byte_1047D13C XI_EA(byte_1047D13C_t, 0x1047D13Cu)
typedef int dword_1047D150_t;
#define dword_1047D150 XI_EA(dword_1047D150_t, 0x1047D150u)
typedef int dword_1047D158_t;
#define dword_1047D158 XI_EA(dword_1047D158_t, 0x1047D158u)
typedef int dword_1047D200_t;
#define dword_1047D200 XI_EA(dword_1047D200_t, 0x1047D200u)
typedef int g_pActorDrawListHead_t;
#define g_pActorDrawListHead XI_EA(g_pActorDrawListHead_t, 0x1047D578u)
typedef char byte_1047D580_t;
#define byte_1047D580 XI_EA(byte_1047D580_t, 0x1047D580u)
typedef _DWORD dword_1047D600_t[3];
#define dword_1047D600 XI_EA(dword_1047D600_t, 0x1047D600u)
typedef char byte_1047D694_t;
#define byte_1047D694 XI_EA(byte_1047D694_t, 0x1047D694u)
typedef char byte_1047D695_t;
#define byte_1047D695 XI_EA(byte_1047D695_t, 0x1047D695u)
typedef char byte_10487F80_t;
#define byte_10487F80 XI_EA(byte_10487F80_t, 0x10487F80u)
typedef int dword_1048A308_t;
#define dword_1048A308 XI_EA(dword_1048A308_t, 0x1048A308u)
typedef int dword_1048A30C_t;
#define dword_1048A30C XI_EA(dword_1048A30C_t, 0x1048A30Cu)
typedef int dword_1048A310_t;
#define dword_1048A310 XI_EA(dword_1048A310_t, 0x1048A310u)
typedef int dword_1048A314_t;
#define dword_1048A314 XI_EA(dword_1048A314_t, 0x1048A314u)
typedef int dword_1048A318_t;
#define dword_1048A318 XI_EA(dword_1048A318_t, 0x1048A318u)
typedef int dword_1048A31C_t;
#define dword_1048A31C XI_EA(dword_1048A31C_t, 0x1048A31Cu)
typedef int dword_1048A320_t;
#define dword_1048A320 XI_EA(dword_1048A320_t, 0x1048A320u)
typedef char byte_1048A328_t;
#define byte_1048A328 XI_EA(byte_1048A328_t, 0x1048A328u)
typedef int dword_1048A32C_t;
#define dword_1048A32C XI_EA(dword_1048A32C_t, 0x1048A32Cu)
typedef int dword_1048A330_t;
#define dword_1048A330 XI_EA(dword_1048A330_t, 0x1048A330u)
typedef int dword_1048A334_t;
#define dword_1048A334 XI_EA(dword_1048A334_t, 0x1048A334u)
typedef int dword_1048A338_t;
#define dword_1048A338 XI_EA(dword_1048A338_t, 0x1048A338u)
typedef int dword_1048A33C_t;
#define dword_1048A33C XI_EA(dword_1048A33C_t, 0x1048A33Cu)
typedef int dword_1048A340_t;
#define dword_1048A340 XI_EA(dword_1048A340_t, 0x1048A340u)
typedef int dword_1048A350_t;
#define dword_1048A350 XI_EA(dword_1048A350_t, 0x1048A350u)
typedef int dword_1048A354_t;
#define dword_1048A354 XI_EA(dword_1048A354_t, 0x1048A354u)
typedef int g_actorUpdateBucketCountA_t[];
#define g_actorUpdateBucketCountA XI_EA(g_actorUpdateBucketCountA_t, 0x1048A358u)
typedef int g_actorUpdateBucketCountB_t[];
#define g_actorUpdateBucketCountB XI_EA(g_actorUpdateBucketCountB_t, 0x1048A368u)
typedef int g_pActiveSkeletonActorRes_t;
#define g_pActiveSkeletonActorRes XI_EA(g_pActiveSkeletonActorRes_t, 0x1048A3D8u)
typedef _BYTE g_viewport_1B70_t;
#define g_viewport_1B70 XI_EA(g_viewport_1B70_t, 0x104E1B70u)
typedef int dword_104E1B88_t;
#define dword_104E1B88 XI_EA(dword_104E1B88_t, 0x104E1B88u)
typedef int dword_104E1B8C_t;
#define dword_104E1B8C XI_EA(dword_104E1B8C_t, 0x104E1B8Cu)
typedef void * dword_104E1B90_t;
#define dword_104E1B90 XI_EA(dword_104E1B90_t, 0x104E1B90u)
typedef int dword_104E1BF8_t[17];
#define dword_104E1BF8 XI_EA(dword_104E1BF8_t, 0x104E1BF8u)
typedef int dword_104E1D4C_t;
#define dword_104E1D4C XI_EA(dword_104E1D4C_t, 0x104E1D4Cu)
typedef int dword_10578158_t;
#define dword_10578158 XI_EA(dword_10578158_t, 0x10578158u)
typedef void * dword_1057876C_t;
#define dword_1057876C XI_EA(dword_1057876C_t, 0x1057876Cu)
typedef __int16 word_10621838_t[14];
#define word_10621838 XI_EA(word_10621838_t, 0x10621838u)
typedef int dword_106218B4_t;
#define dword_106218B4 XI_EA(dword_106218B4_t, 0x106218B4u)
typedef int dword_106218B8_t;
#define dword_106218B8 XI_EA(dword_106218B8_t, 0x106218B8u)
typedef _BYTE g_viewport_1904_t;
#define g_viewport_1904 XI_EA(g_viewport_1904_t, 0x10621904u)
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
typedef int dword_10621A08_t;
#define dword_10621A08 XI_EA(dword_10621A08_t, 0x10621A08u)
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
typedef _BYTE unk_10624DD3_t;
#define unk_10624DD3 XI_EA(unk_10624DD3_t, 0x10624DD3u)
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
typedef _BYTE unk_10639088_t;
#define unk_10639088 XI_EA(unk_10639088_t, 0x10639088u)
typedef _BYTE unk_106390B0_t;
#define unk_106390B0 XI_EA(unk_106390B0_t, 0x106390B0u)
typedef _BYTE unk_106390B4_t;
#define unk_106390B4 XI_EA(unk_106390B4_t, 0x106390B4u)
typedef _BYTE unk_106390C8_t;
#define unk_106390C8 XI_EA(unk_106390C8_t, 0x106390C8u)
typedef _BYTE unk_106390CC_t;
#define unk_106390CC XI_EA(unk_106390CC_t, 0x106390CCu)
typedef int g_zbiasPreset_C4D0_t;
#define g_zbiasPreset_C4D0 XI_EA(g_zbiasPreset_C4D0_t, 0x1065C4D0u)
typedef void * g_pXiZone_t;
#define g_pXiZone XI_EA(g_pXiZone_t, 0x1065CB04u)
typedef int dword_10662DCC_t;
#define dword_10662DCC XI_EA(dword_10662DCC_t, 0x10662DCCu)
typedef int C_t;
#define C XI_EA(C_t, 0x106631D4u)
typedef int dword_10666E7C_t;
#define dword_10666E7C XI_EA(dword_10666E7C_t, 0x10666E7Cu)
typedef int dword_10668FD8_t;
#define dword_10668FD8 XI_EA(dword_10668FD8_t, 0x10668FD8u)
typedef int dword_10669014_t;
#define dword_10669014 XI_EA(dword_10669014_t, 0x10669014u)
typedef int g_stateBlock_914C_t;
#define g_stateBlock_914C XI_EA(g_stateBlock_914C_t, 0x1066914Cu)
typedef int g_stateBlock_9150_t;
#define g_stateBlock_9150 XI_EA(g_stateBlock_9150_t, 0x10669150u)
typedef int dword_10669400_t;
#define dword_10669400 XI_EA(dword_10669400_t, 0x10669400u)
typedef int dword_10669404_t;
#define dword_10669404 XI_EA(dword_10669404_t, 0x10669404u)
typedef int dword_10669408_t;
#define dword_10669408 XI_EA(dword_10669408_t, 0x10669408u)
typedef int dword_1066940C_t;
#define dword_1066940C XI_EA(dword_1066940C_t, 0x1066940Cu)
typedef int dword_10669410_t;
#define dword_10669410 XI_EA(dword_10669410_t, 0x10669410u)
typedef IDirect3D8 * This_t;
#define This XI_EA(This_t, 0x10669418u)
typedef IDirect3DDevice8 * g_pDancerDevice_t;
#define g_pDancerDevice XI_EA(g_pDancerDevice_t, 0x1066941Cu)
typedef int dword_1099D200_t;
#define dword_1099D200 XI_EA(dword_1099D200_t, 0x1099D200u)
typedef int dword_1099D618_t[];
#define dword_1099D618 XI_EA(dword_1099D618_t, 0x1099D618u)
typedef int dword_1099D61C_t[];
#define dword_1099D61C XI_EA(dword_1099D61C_t, 0x1099D61Cu)
typedef int dword_1099D620_t[];
#define dword_1099D620 XI_EA(dword_1099D620_t, 0x1099D620u)
typedef int dword_1099D624_t[];
#define dword_1099D624 XI_EA(dword_1099D624_t, 0x1099D624u)
typedef int dword_1099D628_t[];
#define dword_1099D628 XI_EA(dword_1099D628_t, 0x1099D628u)
typedef int dword_1099D62C_t[];
#define dword_1099D62C XI_EA(dword_1099D62C_t, 0x1099D62Cu)
typedef int dword_1099D630_t[];
#define dword_1099D630 XI_EA(dword_1099D630_t, 0x1099D630u)
typedef int dword_1099D634_t[];
#define dword_1099D634 XI_EA(dword_1099D634_t, 0x1099D634u)
typedef IDirect3DBaseTexture8 * dword_1099D638_t;
#define dword_1099D638 XI_EA(dword_1099D638_t, 0x1099D638u)
typedef int dword_1099D63C_t[5111];
#define dword_1099D63C XI_EA(dword_1099D63C_t, 0x1099D63Cu)
typedef int dword_109A2618_t;
#define dword_109A2618 XI_EA(dword_109A2618_t, 0x109A2618u)
typedef int dword_109A261C_t;
#define dword_109A261C XI_EA(dword_109A261C_t, 0x109A261Cu)
typedef char byte_109A2620_t[256];
#define byte_109A2620 XI_EA(byte_109A2620_t, 0x109A2620u)
typedef int dword_109A2730_t;
#define dword_109A2730 XI_EA(dword_109A2730_t, 0x109A2730u)
typedef int dword_109A2740_t;
#define dword_109A2740 XI_EA(dword_109A2740_t, 0x109A2740u)
typedef int g_counter_109A2744_t;
#define g_counter_109A2744 XI_EA(g_counter_109A2744_t, 0x109A2744u)
typedef int g_sceneRT_t[];
#define g_sceneRT XI_EA(g_sceneRT_t, 0x109A2798u)
typedef int g_sceneRT1_t;
#define g_sceneRT1 XI_EA(g_sceneRT1_t, 0x109A279Cu)
typedef char byte_109F81A4_t;
#define byte_109F81A4 XI_EA(byte_109F81A4_t, 0x109F81A4u)
typedef _BYTE g_pCameraSharedInstance_t;
#define g_pCameraSharedInstance XI_EA(g_pCameraSharedInstance_t, 0x109F81C0u)
