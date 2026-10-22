// gfx_free.hpp -- graphics functions with no established class.
#pragma once
#include "gfx_common.hpp"
XI_API void __stdcall fn_10001070(undefined4 param_1, undefined4 param_2, int param_3, undefined * param_4);  // 0x10001070
XI_API undefined4 __cdecl App_RenderFrameMaskedFPU(void);  // 0x10001B50
XI_API undefined4 __stdcall App_CreateInputSystem(undefined4 param_1);  // 0x10001BB0
XI_API void __cdecl fn_10001D90(undefined4 param_1);  // 0x10001D90
XI_API undefined4 __cdecl fn_10001E70(char * param_1);  // 0x10001E70
XI_API undefined4 __cdecl fn_10001F20(undefined4 param_1, undefined4 param_2);  // 0x10001F20
XI_API char * __cdecl fn_10001F40(char * param_1, char * param_2, int param_3);  // 0x10001F40
XI_API char __cdecl Gfx_CopySurfaceRectToTexture(struct IDirect3DSurface8 *a1, const struct tagRECT *a2, unsigned int *a3, int a4);  // 0x10002040
XI_API char __cdecl Gfx_ProbeDeviceCaps(int a1, unsigned int *a2, int *a3);  // 0x10002130
XI_API uint32_t __cdecl fn_10002440(uint32_t param_1);  // 0x10002440
XI_API unsigned __int8 __cdecl Gfx_IsGammaSupported(void);  // 0x10002BE0
XI_API CDx * __cdecl Gfx_SetGammaRGB(int a1, int a2, int a3);  // 0x10002C00
XI_API CDx * __cdecl Gfx_SetGammaR(int a1);  // 0x10002C30
XI_API CDx * __cdecl Gfx_SetGammaG(int a1);  // 0x10002C80
XI_API CDx * __cdecl Gfx_SetGammaB(int a1);  // 0x10002CD0
XI_API int __cdecl Gfx_GetGammaRGB(int a1, int a2, int a3);  // 0x10002D20
XI_API long double __attribute__((cdecl, regparm(1))) fn_10002D80(undefined4 xi_eax);  // 0x10002D80
XI_API void __cdecl fn_10002E60(undefined4 param_1);  // 0x10002E60
XI_API void __cdecl fn_10002E80(undefined4 param_1);  // 0x10002E80
XI_API long double __cdecl fn_10002EC0(int param_1);  // 0x10002EC0
XI_API long double __cdecl fn_10002EE0(int param_1);  // 0x10002EE0
XI_API char __cdecl Gfx_CreateD3DHolder(int a1, int a2, int a3, char a4, char a5);  // 0x10002F20
XI_API int __cdecl Gfx_DestroyD3DHolder(void);  // 0x10003040
XI_API char __thiscall Gfx_D3DBootstrap(int self_, int a2, int a3, int a4, char a5, char a6, int a7);  // 0x100031F0
XI_API char __thiscall Gfx_TryCreateDeviceForMode(_DWORD **self_, int a2, int a3, int a4, char a5, char a6, int a7, int a8);  // 0x100033C0
XI_API char __thiscall Gfx_CreateDevice(_DWORD *self_, int a2, int a3, int a4, int a5, int a6, char a7, char a8, int a9);  // 0x100034B0
XI_API void * __thiscall fn_100035F0(void * self_);  // 0x100035F0
XI_API void __thiscall fn_10003640(void * self_);  // 0x10003640
XI_API void __thiscall DebugOverlay_Draw(void * self_);  // 0x10003650
XI_API void __thiscall fn_10003660(void * self_);  // 0x10003660
XI_API void __thiscall fn_10003710(void * self_);  // 0x10003710
XI_API void __thiscall fn_10003890(_DWORD *self_);  // 0x10003890
XI_API int __thiscall Debug_DrawTextTable(_DWORD *self_);  // 0x10003B60
XI_API void __stdcall Debug_DrawWorldText(_DWORD *a1, const char *a2, int arg8);  // 0x10003D70
XI_API void __thiscall fn_10005600(void * self_, undefined4 param_1);  // 0x10005600
XI_API void * __thiscall fn_10005660(void * self_);  // 0x10005660
XI_API undefined4 __cdecl fn_100059C0(uint32_t param_1);  // 0x100059C0
XI_API undefined4 __cdecl fn_10005A00(uint32_t param_1);  // 0x10005A00
XI_API undefined4 __cdecl fn_10007830(char * param_1, undefined4 param_2, undefined4 param_3);  // 0x10007830
XI_API void __stdcall fn_10007860(uint32_t param_1, undefined4 xi_s2);  // 0x10007860
XI_API undefined4 __thiscall DirectInput_InstallKeyboard(void * self_);  // 0x10007E20
XI_API char __thiscall DirectInput_InstallMouse(_DWORD **self_);  // 0x10007EF0
XI_API undefined4 __thiscall DirectInput_InstallJoystick(void * self_);  // 0x100083D0
XI_API undefined4 __thiscall fn_10008770(void * self_, int param_1);  // 0x10008770
XI_API void __stdcall fn_100087D0(int param_1, int param_2, undefined4 xi_s3);  // 0x100087D0
XI_API undefined4 __thiscall fn_10008830(void * self_, uint32_t param_1);  // 0x10008830
XI_API void __thiscall fn_100088F0(void * self_);  // 0x100088F0
XI_API void __thiscall fn_100089D0(void * self_);  // 0x100089D0
XI_API undefined4 __thiscall fn_10008D90(void * self_);  // 0x10008D90
XI_API undefined4 __thiscall fn_10008E10(void * self_);  // 0x10008E10
XI_API void __stdcall fn_10009750(HCURSOR param_1);  // 0x10009750
XI_API undefined4 __thiscall fn_10009870(void * self_, undefined4 param_1, int * param_2);  // 0x10009870
XI_API void __attribute__((stdcall, regparm(3))) fn_100098E0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, undefined4 param_2, int * param_3, undefined4 param_4);  // 0x100098E0
XI_API undefined4 __thiscall fn_10009D20(void * self_);  // 0x10009D20
XI_API void __thiscall fn_10009EA0(void * self_, int * param_1, undefined4 xi_s2, undefined4 xi_s3);  // 0x10009EA0
XI_API void __thiscall fn_10009EE0(void * self_, int * param_1, undefined4 xi_s2, undefined4 xi_s3);  // 0x10009EE0
XI_API void __thiscall fn_1000A070(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x1000A070
XI_API void __thiscall fn_1000A200(void * self_, undefined4 xi_s1, undefined4 xi_s2);  // 0x1000A200
XI_API void __thiscall fn_1000A230(void * self_, undefined4 xi_s1, undefined4 xi_s2);  // 0x1000A230
XI_API void __thiscall fn_1000A390(void * self_, uint32_t param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x1000A390
XI_API void __thiscall fn_1000A550(void * self_, undefined4 param_1, undefined4 xi_s2, undefined4 xi_s3, undefined4 xi_s4);  // 0x1000A550
XI_API void __cdecl fn_1000BB10(void);  // 0x1000BB10
XI_API void __cdecl fn_1000BB20(void);  // 0x1000BB20
XI_API void __attribute__((stdcall, regparm(3))) fn_1000BE10(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, int param_2);  // 0x1000BE10
XI_API undefined4 __thiscall Light_ApplyTimeOfDayTint(void * self_, int param_1);  // 0x1000BE50
XI_API uint32_t __cdecl fn_1000D100(uint32_t param_1);  // 0x1000D100
XI_API uint32_t __cdecl fn_1000D120(uint32_t param_1);  // 0x1000D120
XI_API undefined8 __cdecl fn_1000D150(uint32_t param_1);  // 0x1000D150
XI_API void __thiscall j_CEnv_UpdateDualTargetMode(void * self_, undefined4 xi_s1);  // 0x1000D3F0
XI_API void __cdecl fn_1000D620(void);  // 0x1000D620
XI_API void __cdecl App_RegisterSystemDats(void);  // 0x10010370
XI_API void __cdecl fn_10010520(void);  // 0x10010520
XI_API uint32_t __cdecl fn_100106C0(void);  // 0x100106C0
XI_API void * __thiscall App_Constructor(void * self_);  // 0x10010700
XI_API void __thiscall App_SetSeqStep(void * self_, undefined4 param_1);  // 0x10010CE0
XI_API void __thiscall App_AdvanceSeqStep(void * self_);  // 0x10010D00
XI_API void __thiscall fn_10010D10(void * self_);  // 0x10010D10
XI_API char __thiscall App_InitScene(int self_, int a4);  // 0x10010D20
XI_API unsigned __int8 __thiscall Frame_Render(void * self_);  // 0x10011A00
XI_API int __cdecl Post_Afterimage(void);  // 0x10012F40
XI_API int __thiscall Post_FlashAndZoomFeedback(void *self_);  // 0x100132B0
XI_API undefined4 __thiscall fn_100139E0(void * self_);  // 0x100139E0
XI_API undefined4 __cdecl fn_10013AC0(void);  // 0x10013AC0
XI_API char __thiscall App_StepGameState6(_WORD *self_);  // 0x10013DF0
XI_API char __thiscall App_StepGameState7(_WORD *self_);  // 0x10013F10
XI_API char __thiscall App_StepGameState8(_WORD *self_);  // 0x10014030
XI_API bool __cdecl App_StepGameState9(void);  // 0x10014120
XI_API undefined4 __thiscall fn_10014170(void * self_);  // 0x10014170
XI_API char __thiscall fn_100141C0(_WORD *self_);  // 0x100141C0
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_10014200(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x10014200
XI_API undefined4 __thiscall fn_10014220(void * self_, undefined4 param_1);  // 0x10014220
XI_API undefined4 __thiscall fn_100142A0(void * self_);  // 0x100142A0
XI_API undefined4 __cdecl fn_100142B0(void);  // 0x100142B0
XI_API undefined1 __thiscall fn_100149B0(void * self_);  // 0x100149B0
XI_API undefined4 __thiscall fn_10014A20(void * self_);  // 0x10014A20
XI_API undefined1 __thiscall fn_10014C10(void * self_);  // 0x10014C10
XI_API void __cdecl fn_10014C60(void);  // 0x10014C60
XI_API undefined4 __thiscall fn_10014C90(void * self_);  // 0x10014C90
XI_API undefined4 __cdecl fn_10014CB0(void);  // 0x10014CB0
XI_API undefined4 __cdecl fn_10014CE0(void);  // 0x10014CE0
XI_API long double __cdecl Camera_GetFrameScale(void);  // 0x10014CF0
XI_API long double __cdecl fn_10014D20(void);  // 0x10014D20
XI_API undefined4 __cdecl fn_10014D60(void);  // 0x10014D60
XI_API long double __cdecl Camera_GetFloat2DC(void);  // 0x10014D70
XI_API long double __cdecl Camera_GetFloat2E0(void);  // 0x10014D80
XI_API void __cdecl Camera_SetClearColorRGB(undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x10014D90
XI_API void __thiscall fn_10014DB0(void * self_, uint32_t param_1, uint32_t param_2, uint32_t param_3);  // 0x10014DB0
XI_API void __cdecl fn_10014E00(undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x10014E00
XI_API void __thiscall fn_10014E20(void * self_, undefined4 param_1, undefined4 param_2, uint32_t param_3);  // 0x10014E20
XI_API void __cdecl fn_10014E60(float param_1);  // 0x10014E60
XI_API void __cdecl fn_10014E80(float param_1);  // 0x10014E80
XI_API void __cdecl fn_10014EB0(float param_1);  // 0x10014EB0
XI_API void __cdecl fn_10014ED0(float param_1);  // 0x10014ED0
XI_API undefined4 __cdecl fn_10015100(undefined4 param_1);  // 0x10015100
XI_API undefined4 __thiscall fn_10015120(void * self_, undefined4 xi_s1, undefined4 xi_s2, undefined4 xi_s3, undefined4 xi_s4, undefined4 xi_s5, undefined4 xi_s6, undefined4 xi_s7, undefined4 xi_s8, undefined4 xi_s9, undefined4 xi_s10, undefined4 xi_s11, undefined4 xi_s12, undefined4 xi_s13, undefined4 xi_s14, undefined4 xi_s15, undefined4 xi_s16);  // 0x10015120
XI_API void __thiscall fn_100151D0(void * self_, undefined4 xi_s1, undefined4 xi_s2, undefined4 xi_s3, undefined4 xi_s4, undefined4 xi_s5, undefined4 xi_s6, undefined4 xi_s7, undefined4 xi_s8, undefined4 xi_s9, undefined4 xi_s10, undefined4 xi_s11, undefined4 xi_s12, undefined4 xi_s13, undefined4 xi_s14, undefined4 xi_s15, undefined4 xi_s16);  // 0x100151D0
XI_API int __cdecl Camera_GetViewMatrixPtr(void);  // 0x10015220
XI_API undefined4 __cdecl Camera_GetField50(void);  // 0x10015250
XI_API int __cdecl Camera_GetMatrix154Ptr(void);  // 0x10015270
XI_API void __cdecl fn_10015290(undefined4 param_1);  // 0x10015290
XI_API void __thiscall fn_100152B0(void * self_, undefined4 param_1);  // 0x100152B0
XI_API long double __cdecl fn_100152C0(void);  // 0x100152C0
XI_API void __cdecl fn_10015360(int param_1);  // 0x10015360
XI_API void __cdecl fn_100153A0(float param_1, float param_2);  // 0x100153A0
XI_API void __cdecl Camera_BuildProjection(void);  // 0x10015480
XI_API void __thiscall fn_10015490(void * self_);  // 0x10015490
XI_API undefined4 __cdecl fn_100155D0(void);  // 0x100155D0
XI_API undefined4 __cdecl fn_10015600(void);  // 0x10015600
XI_API undefined4 __cdecl fn_10015610(void);  // 0x10015610
XI_API undefined4 __cdecl fn_10015640(void);  // 0x10015640
XI_API undefined4 __cdecl fn_10015650(void);  // 0x10015650
XI_API undefined4 __cdecl Camera_GetViewportWidth(void);  // 0x10015680
XI_API undefined4 __cdecl Camera_GetViewportHeight(void);  // 0x10015690
XI_API int __cdecl fn_100156A0(int param_1, int param_2, undefined4 * param_3, undefined4 * param_4);  // 0x100156A0
XI_API undefined4 __cdecl fn_10015740(void);  // 0x10015740
XI_API void __stdcall fn_100157B0(undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x100157B0
XI_API char __cdecl App_CreateRendererAndApp(void);  // 0x100161B0
XI_API void __cdecl fn_10016300(void);  // 0x10016300
XI_API _DWORD * __cdecl fn_10016320(void);  // 0x10016320
XI_API void __cdecl fn_100163C0(undefined4 param_1);  // 0x100163C0
XI_API void __cdecl fn_100163E0(undefined4 param_1);  // 0x100163E0
XI_API undefined4 __cdecl App_SetGlobal_104569C8(undefined4 param_1);  // 0x10016400
XI_API undefined4 __cdecl fn_10016410(void);  // 0x10016410
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_100164B0(undefined4 xi_eax);  // 0x100164B0
XI_API undefined4 __cdecl App_SetErrorCode(undefined4 param_1);  // 0x100164C0
XI_API void __cdecl fn_10016560(undefined4 * param_1, undefined4 * param_2);  // 0x10016560
XI_API void __thiscall GammaRamp_SetRGB(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x100170A0
XI_API void __thiscall GammaRamp_GetRGB(void * self_, undefined4 * param_1, undefined4 * param_2, undefined4 * param_3);  // 0x100170D0
XI_API undefined4 __thiscall GammaRamp_GetChannelSamples(void * self_, float * param_1, float * param_2, float * param_3);  // 0x10017100
XI_API void __thiscall GammaRamp_BuildRamps(void * self_);  // 0x10017160
XI_API void __thiscall GammaRamp_ApplyIfChanged(int self_);  // 0x10017230
XI_API int __stdcall fn_10017310(int a1, int a2);  // 0x10017310
XI_API void __cdecl setPolUrl(undefined4 param_1, undefined4 param_2);  // 0x10017630
XI_API undefined4 __cdecl fn_10017670(void);  // 0x10017670
XI_API int __thiscall fn_10017810(void *self_, int a1);  // 0x10017810
XI_API undefined4 __thiscall fn_10017BB0(void * self_);  // 0x10017BB0
XI_API void __thiscall fn_10017C20(void * self_, undefined4 param_1, undefined4 param_2, short * param_3);  // 0x10017C20
XI_API int __thiscall fn_10017C60(int *self_, int a2, int a3, int a4);  // 0x10017C60
XI_API void __cdecl fn_10017D20(undefined4 xi_s1, ...);  // 0x10017D20
XI_API void __cdecl fn_10017D30(undefined4 xi_s1, ...);  // 0x10017D30
XI_API undefined8 __cdecl fn_10018180(undefined4 param_1);  // 0x10018180
XI_API undefined4 __cdecl fn_10018270(void);  // 0x10018270
XI_API void __cdecl fn_100182C0(undefined4 param_1);  // 0x100182C0
XI_API undefined8 __cdecl _exit_0(undefined4 param_1);  // 0x100182E0
XI_API void __cdecl fn_10018300(uint32_t param_1, undefined4 * param_2, uint32_t param_3);  // 0x10018300
XI_API void * __thiscall fn_10018350(void * self_);  // 0x10018350
XI_API void * __thiscall fn_10018430(void * self_);  // 0x10018430
XI_API void * __thiscall fn_10018550(void * self_);  // 0x10018550
XI_API void __thiscall fn_100185A0(void * self_, undefined4 param_1, uint32_t param_2);  // 0x100185A0
XI_API undefined4 __thiscall fn_10018840(void * self_);  // 0x10018840
XI_API void * __thiscall OpeningSubtitle_Constructor(void * self_, int param_1);  // 0x10018A50
XI_API void __thiscall OpeningSubtitle_Reset(void * self_);  // 0x10018B30
XI_API void __thiscall fn_10018B50(void * self_);  // 0x10018B50
XI_API void * __thiscall Vt10329FE0_Constructor(void * self_);  // 0x10018C50
XI_API void __thiscall Vt10329FE0_Reset(void * self_);  // 0x10018CE0
XI_API int * __cdecl fn_100191D0(void);  // 0x100191D0
XI_API void __thiscall fn_10019210(void * self_);  // 0x10019210
XI_API void * __thiscall fn_10019490(void * self_);  // 0x10019490
XI_API void __thiscall fn_100194F0(void * self_);  // 0x100194F0
XI_API long double __thiscall fn_10019550(void * self_);  // 0x10019550
XI_API long double __thiscall fn_10019590(void * self_);  // 0x10019590
XI_API void __attribute__((stdcall, regparm(3))) MotionQueue_ApplyPolicy(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, undefined4 param_2, undefined4 param_3, float param_4, undefined4 param_5);  // 0x10019A50
XI_API undefined4 __thiscall SampleAnimationTrack_ToBuffer(void * self_, int param_1, int * param_2, int param_3, float param_4, int param_5);  // 0x10019B30
XI_API undefined4 __thiscall SampleAnimationTrack_Live(void * self_, int param_1, int * param_2, int param_3, float param_4, float param_5, int param_6);  // 0x10019EE0
XI_API void __thiscall fn_1001A3E0(void * self_, undefined4 param_1);  // 0x1001A3E0
XI_API undefined4 __thiscall MotionQueue_UpdateAllChannels(void * self_, undefined4 xi_s1);  // 0x1001A420
XI_API void __thiscall fn_1001A6E0(void * self_, int * param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7, int param_8, undefined4 param_9, undefined4 param_10);  // 0x1001A6E0
XI_API void __thiscall CYyMotionQue_GetResource(void * self_, undefined4 * param_1);  // 0x1001A850
XI_API void __thiscall fn_1001ABF0(void * self_);  // 0x1001ABF0
XI_API void __thiscall fn_1001ACB0(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7, undefined4 param_8);  // 0x1001ACB0
XI_API void __thiscall fn_1001B170(void * self_, float param_1);  // 0x1001B170
XI_API void __thiscall fn_1001B1F0(void * self_, undefined4 param_1, undefined4 param_2);  // 0x1001B1F0
XI_API void __thiscall fn_1001B2E0(void * self_, int * param_1, int param_2);  // 0x1001B2E0
XI_API void __thiscall fn_1001B520(void * self_, float param_1);  // 0x1001B520
XI_API undefined4 __thiscall fn_1001B5D0(void * self_);  // 0x1001B5D0
XI_API void * __thiscall fn_1001CF80(void * self_);  // 0x1001CF80
XI_API _DWORD * __cdecl Model_CreateTranslucentTargets(void);  // 0x1001D0A0
XI_API void __thiscall fn_1001D170(void * self_);  // 0x1001D170
XI_API char __thiscall Model_BeginTranslucentPass(int self_);  // 0x1001D240
XI_API char __thiscall Model_TranslucentUnbindTexture(_DWORD *self_);  // 0x1001D350
XI_API void __stdcall fn_1001D390(undefined4 param_1, undefined4 param_2);  // 0x1001D390
XI_API int __thiscall Model_CompositeTranslucentPass(void *self_);  // 0x1001D3B0
XI_API char __cdecl TexObj_InitFromHeader(int a1, int a2, int a3, int a4);  // 0x1001D940
XI_API void __thiscall fn_1001DBD0(void * self_);  // 0x1001DBD0
XI_API int __thiscall Tex_DrawNamedTexturePreview(void *self_, int a1, int a2);  // 0x1001DC70
XI_API int __stdcall Gfx_DrawScreenQuad256(int a1, int a2);  // 0x1001DD00
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_1001E2F0(undefined4 xi_eax, undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x1001E2F0
XI_API undefined4 __attribute__((cdecl, regparm(1))) CYyCamMng2_CallGlobalVfunc1(undefined4 xi_eax);  // 0x1001E4B0
XI_API long double __cdecl fn_1001E4D0(void);  // 0x1001E4D0
XI_API undefined4 __thiscall fn_1001E4F0(void * self_);  // 0x1001E4F0
XI_API undefined4 __thiscall fn_1001E590(void * self_);  // 0x1001E590
XI_API void * __thiscall fn_1001E600(void * self_);  // 0x1001E600
XI_API undefined4 __thiscall fn_1001E760(void * self_, undefined4 * param_1);  // 0x1001E760
XI_API undefined4 __thiscall fn_1001E780(void * self_, float * param_1);  // 0x1001E780
XI_API undefined4 __thiscall fn_10021AE0(void * self_);  // 0x10021AE0
XI_API int __thiscall fn_10021B70(void * self_);  // 0x10021B70
XI_API int __thiscall fn_10021C10(void * self_);  // 0x10021C10
XI_API void __thiscall fn_100220E0(void * self_);  // 0x100220E0
XI_API long double __thiscall Actor_GetPercentScaleByIndex(void * self_, int param_2);  // 0x10022120
XI_API undefined4 __cdecl fn_10022170(int param_1);  // 0x10022170
XI_API long double __thiscall Actor_LookupPercentScaleA(void * self_, int param_2);  // 0x10022190
XI_API long double __thiscall Actor_LookupPercentScaleB(void * self_, int param_2);  // 0x100221E0
XI_API void * __thiscall Actor_InitIndexTable(void * self_);  // 0x10022230
XI_API void * __thiscall fn_10025010(void * self_);  // 0x10025010
XI_API undefined4 __thiscall fn_100250F0(void * self_, int param_1);  // 0x100250F0
XI_API void __cdecl fn_10025220(void);  // 0x10025220
XI_API Vt_1032A460_vtbl ** __thiscall fn_10025260(Vt_1032A460_vtbl **self_);  // 0x10025260
XI_API char __thiscall ProjTex_BeginPass(_DWORD *self_, int *a2, int a3, _DWORD *a4);  // 0x10025330
XI_API int __thiscall ProjTex_EndPass(void *self_, int a1);  // 0x100255C0
XI_API int __thiscall ProjTex_CaptureToTarget(void *self_, int a1, int a2, int a3, int a4);  // 0x10025870
XI_API void __cdecl fn_10025BD0(void);  // 0x10025BD0
XI_API void __cdecl fn_10025BE0(void);  // 0x10025BE0
XI_API void __thiscall fn_10025BF0(void * self_);  // 0x10025BF0
XI_API char __stdcall EnvMap_BeginPass(int *a1, int a2, float *a3);  // 0x10025C40
XI_API int __stdcall EnvMap_EndPass(int a1);  // 0x10025F70
XI_API void __cdecl fn_10026E00(undefined4 * param_1);  // 0x10026E00
XI_API void __cdecl Quat_SetIdentity(undefined4 * param_1);  // 0x10026E10
XI_API void __cdecl Vec4_Set(float * param_1, float param_2, float param_3, float param_4, float param_5);  // 0x10026E30
XI_API undefined4 __cdecl fn_10026E50(float * param_1, float param_2, float param_3, float param_4);  // 0x10026E50
XI_API undefined8 __cdecl Vec4_Copy(undefined4 * param_1, undefined4 * param_2);  // 0x10026E90
XI_API undefined4 __cdecl Vec3_Copy(undefined4 * param_1, undefined4 * param_2);  // 0x10026EB0
XI_API undefined4 __cdecl Vec3_AddInPlace(float * param_1, float * param_2);  // 0x10026F20
XI_API void __cdecl Vec3_Add_10026FA0(float * param_1, float * param_2, float * param_3);  // 0x10026FA0
XI_API void __cdecl Vec2_Add(float * param_1, float * param_2, float * param_3);  // 0x10026FD0
XI_API undefined4 __cdecl fn_100270A0(float * param_1, float * param_2);  // 0x100270A0
XI_API undefined4 __cdecl Vec3_Sub(float * param_1, float * param_2, float * param_3);  // 0x10027120
XI_API void __cdecl fn_100272B0(float * param_1, float param_2);  // 0x100272B0
XI_API undefined4 __cdecl Vec3_Scale(float * param_1, float * param_2, float param_3);  // 0x100272E0
XI_API void __cdecl Vec3_MulInPlace(float * param_1, float * param_2);  // 0x10027310
XI_API undefined4 __cdecl fn_10027340(float * param_1, float * param_2, float * param_3);  // 0x10027340
XI_API void __cdecl fn_10027510(float * param_1, undefined4 * param_2);  // 0x10027510
XI_API undefined4 __cdecl fn_10027550(undefined4 * param_1, float * param_2, float * param_3);  // 0x10027550
XI_API long double __cdecl fn_10027620(undefined4 param_1, undefined4 param_2);  // 0x10027620
XI_API void __cdecl Gfx_TransformWorldToView(float *a1, _DWORD *a2);  // 0x100277E0
XI_API void __cdecl Gfx_ProjectViewToClip(float *a1, _DWORD *a2);  // 0x10027870
XI_API float * __cdecl Gfx_NdcToScreen(float *a1);  // 0x100278E0
XI_API float * __cdecl Gfx_ScreenDeltaToNdc(float *a1, float *a2);  // 0x10027930
XI_API undefined4 __thiscall fn_10027990(void * self_);  // 0x10027990
XI_API void __cdecl fn_100279A0(void);  // 0x100279A0
XI_API void __thiscall Mat4_SetIdentity(void * self_);  // 0x100279B0
XI_API undefined4 __thiscall Mat4_Copy(void * self_, undefined4 * param_1);  // 0x100279F0
XI_API void __thiscall Mat4_PostScale(void * self_, undefined4 * param_1);  // 0x10027A60
XI_API void __thiscall Mat4_BuildScale(void * self_, undefined4 * param_1);  // 0x10027AE0
XI_API void __thiscall Mat4_SetScale(void * self_, undefined4 * param_1);  // 0x10027B30
XI_API void __thiscall Mat4_SetRow3(void * self_, undefined4 * param_1);  // 0x10027CD0
XI_API undefined4 __thiscall Mat4_SetTranslation(void * self_, undefined4 * param_1);  // 0x10027CF0
XI_API void __thiscall Mat4_PostMultiply(void * self_, int param_1);  // 0x10027D10
XI_API void __thiscall Mat4_PreMultiply(void * self_, float * param_1);  // 0x10027E90
XI_API undefined4 __thiscall Mat4_TransformVec4InPlace(void * self_, undefined4 * param_1);  // 0x10028140
XI_API undefined4 __thiscall fn_10028170(void * self_, float * param_1, float * param_2);  // 0x10028170
XI_API undefined4 __thiscall Mat4_TransformPointInPlace(void * self_, float * param_1);  // 0x10028200
XI_API undefined8 __thiscall fn_10028230(void * self_, float * param_1, float * param_2);  // 0x10028230
XI_API undefined4 __thiscall Mat4_TransformNormalInPlace(void * self_, undefined4 * param_1);  // 0x10028290
XI_API undefined4 __thiscall Mat4_TransformNormal(void * self_, float * param_1, float * param_2);  // 0x100282C0
XI_API undefined4 __thiscall Mat4_TransformCoordInPlace(void * self_, float * param_1);  // 0x10028320
XI_API undefined4 __thiscall Mat4_InvertOrthonormal(void * self_);  // 0x100283C0
XI_API undefined4 * __cdecl Mem_AllocObject(int param_1, undefined4 param_2);  // 0x10028640
XI_API undefined8 __cdecl Mem_AllocWithFallback(int param_1, int param_2);  // 0x100286B0
XI_API void __cdecl Mem_FreeObject(undefined4 xi_s1);  // 0x10028700
XI_API void __cdecl SharedDeleteHelper(int param_1);  // 0x10028710
XI_API void __cdecl fn_10028720(undefined4 param_1);  // 0x10028720
XI_API int __cdecl fn_10028730(int param_1);  // 0x10028730
XI_API undefined8 __cdecl fn_10028850(int param_1, int param_2, int param_3);  // 0x10028850
XI_API undefined8 __cdecl fn_10028930(int param_1, undefined4 param_2, void * param_3);  // 0x10028930
XI_API void __cdecl fn_10028AA0(int param_1);  // 0x10028AA0
XI_API void __cdecl fn_10028AC0(int param_1);  // 0x10028AC0
XI_API int __thiscall MemBlock_GetSize(void * self_);  // 0x10028B90
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_10028BF0(undefined4 xi_eax, undefined4 param_1, int param_2, int param_3);  // 0x10028BF0
XI_API undefined4 __cdecl fn_10028C30(undefined4 param_1);  // 0x10028C30
XI_API void __cdecl fn_10028C50(undefined4 param_1);  // 0x10028C50
XI_API void __cdecl fn_10028C70(undefined4 param_1);  // 0x10028C70
XI_API void __cdecl fn_10028C90(undefined4 param_1);  // 0x10028C90
XI_API undefined4 __stdcall fn_10029390(undefined4 param_1);  // 0x10029390
XI_API int __stdcall Mem_DrawHeapUsageBar(int a1, int a2, int a3, int a4, int a5);  // 0x100295A0
XI_API int __cdecl fn_10029A50(void);  // 0x10029A50
XI_API void __cdecl fn_10029B60(void);  // 0x10029B60
XI_API void __thiscall fn_10029BB0(void * self_);  // 0x10029BB0
XI_API undefined4 * __thiscall fn_10029C90(void * self_);  // 0x10029C90
XI_API void __thiscall CYyModelDtBase_Destructor(void * self_);  // 0x10029CD0
XI_API undefined4 __thiscall fn_10029D20(void * self_);  // 0x10029D20
XI_API void * __thiscall fn_10029D30(void * self_);  // 0x10029D30
XI_API int __thiscall fn_10029EB0(void * self_);  // 0x10029EB0
XI_API void __thiscall fn_10029F00(void * self_, undefined4 param_1);  // 0x10029F00
XI_API undefined4 __thiscall fn_1002A080(void * self_);  // 0x1002A080
XI_API void __thiscall fn_1002A0C0(void * self_);  // 0x1002A0C0
XI_API void __thiscall fn_1002A700(void * self_, undefined4 param_1, undefined4 param_2);  // 0x1002A700
XI_API undefined4 __thiscall fn_1002A750(void * self_, undefined4 param_1, undefined4 * param_2);  // 0x1002A750
XI_API undefined4 __thiscall fn_1002A780(void * self_, undefined4 param_1, undefined4 * param_2);  // 0x1002A780
XI_API void __stdcall fn_1002A890(int param_1, undefined4 * param_2);  // 0x1002A890
XI_API void __thiscall fn_1002A8B0(void * self_, int param_1, float * param_2);  // 0x1002A8B0
XI_API int __thiscall fn_1002A9B0(void * self_, uint32_t param_1);  // 0x1002A9B0
XI_API int __thiscall fn_1002B5E0(void * self_);  // 0x1002B5E0
XI_API void __thiscall fn_1002B630(void * self_, undefined4 param_1);  // 0x1002B630
XI_API void __thiscall fn_1002C270(void * self_, undefined4 xi_s1);  // 0x1002C270
XI_API void __thiscall fn_1002C2A0(void * self_, undefined4 param_1);  // 0x1002C2A0
XI_API undefined4 __thiscall fn_1002C400(void * self_, int param_1, int param_2, float * param_3);  // 0x1002C400
XI_API undefined4 __thiscall fn_1002C600(void * self_, undefined4 param_1, int param_2);  // 0x1002C600
XI_API undefined4 __thiscall fn_1002C710(void * self_, undefined4 xi_s1);  // 0x1002C710
XI_API uint32_t __thiscall fn_1002C750(void * self_, undefined4 xi_s1);  // 0x1002C750
XI_API void * __thiscall List_GetTail(void * self_);  // 0x1002C7C0
XI_API void __thiscall fn_1002C800(void * self_, int * param_1);  // 0x1002C800
XI_API undefined4 __cdecl Mem_AllocDefault(int param_1);  // 0x1002C890
XI_API void __cdecl thunk_sub_10028AA0(int param_1);  // 0x1002C8B0
XI_API void __thiscall fn_1002D660(void * self_, undefined4 param_1);  // 0x1002D660
XI_API void __attribute__((stdcall, regparm(3))) fn_1002D860(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x1002D860
XI_API void __thiscall fn_1002D8F0(void * self_);  // 0x1002D8F0
XI_API void __cdecl Gfx_SetStreamSourceCached(int param_1, undefined4 param_2, undefined4 param_3);  // 0x1002F130
XI_API int __cdecl Gfx_ApplyCachedStreamSource(unsigned int a1);  // 0x1002F160
XI_API int __cdecl Gfx_ApplyCachedVertexShader(void);  // 0x1002F1A0
XI_API void __thiscall Model_SkinVerticesCpu(void * self_);  // 0x1002F1D0
XI_API void __thiscall fn_100303B0(void * self_);  // 0x100303B0
XI_API void * __thiscall fn_10032A40(void * self_);  // 0x10032A40
XI_API void __thiscall fn_10032A50(void * self_);  // 0x10032A50
XI_API void __cdecl fn_10032A70(void);  // 0x10032A70
XI_API undefined4 __thiscall fn_10032B50(void * self_, float * param_1, float * param_2);  // 0x10032B50
XI_API void __thiscall fn_10032DA0(void * self_);  // 0x10032DA0
XI_API void __thiscall fn_10032FE0(void * self_, float * param_1);  // 0x10032FE0
XI_API undefined4 __thiscall Quat_NLerp(void * self_, float * param_1, float * param_2, float param_3);  // 0x10033220
XI_API void __cdecl fn_100336B0(void);  // 0x100336B0
XI_API void __cdecl fn_100336D0(void);  // 0x100336D0
XI_API _DWORD * __thiscall fn_10033710(_DWORD *self_);  // 0x10033710
XI_API char __thiscall Model_BeginOffscreenPass(int self_, int *a2, int a3);  // 0x10033800
XI_API char __thiscall Model_EndOffscreenPassBindTextures(_DWORD *self_, int a2);  // 0x10033A70
XI_API undefined4 __thiscall fn_10033DA0(void * self_, float * param_1);  // 0x10033DA0
XI_API void __thiscall Shadow_SmoothLightDirection(void * self_, float * param_1);  // 0x10033EA0
XI_API void __thiscall fn_10034230(void * self_);  // 0x10034230
XI_API void __thiscall fn_100342A0(void * self_);  // 0x100342A0
XI_API undefined4 __thiscall Skel_BindShared(void * self_);  // 0x10034330
XI_API void __thiscall Skel_LoadBoneRotTrans(void * self_);  // 0x10034620
XI_API undefined4 __thiscall Skel_ComposeMode(void * self_, int param_1, int param_2, int param_3, int param_4);  // 0x100347D0
XI_API void __stdcall Skel_ComposeFromBone(uint8_t * param_1);  // 0x10034B80
XI_API void __thiscall Skel_OffsetAllTranslations(void * self_, float * param_1, int param_2);  // 0x10035170
XI_API int __thiscall fn_100351F0(void * self_, int param_1);  // 0x100351F0
XI_API int __thiscall fn_10035210(void * self_);  // 0x10035210
XI_API uint16_t * __thiscall fn_10035250(void * self_);  // 0x10035250
XI_API int __thiscall fn_10035270(void * self_, int param_1);  // 0x10035270
XI_API undefined4 __thiscall fn_100352A0(void * self_);  // 0x100352A0
XI_API void __thiscall fn_100352F0(void * self_);  // 0x100352F0
XI_API void __thiscall fn_10035330(void * self_, int param_1, undefined4 param_2, uint32_t param_3);  // 0x10035330
XI_API void __cdecl fn_10035D40(undefined4 param_1, int param_2);  // 0x10035D40
XI_API void __cdecl fn_10035D90(undefined4 param_1, int param_2);  // 0x10035D90
XI_API void __cdecl fn_10035DE0(undefined4 param_1, int param_2);  // 0x10035DE0
XI_API undefined4 __attribute__((cdecl, regparm(1))) MoTask_KillAllGlobalSlots(undefined4 xi_eax, undefined4 param_1);  // 0x10035F00
XI_API undefined4 __cdecl fn_10035F80(undefined4 xi_s1);  // 0x10035F80
XI_API undefined4 __cdecl fn_10035FB0(float param_1);  // 0x10035FB0
XI_API void __attribute__((cdecl, regparm(1))) fn_10036020(undefined4 xi_eax, undefined4 xi_s1);  // 0x10036020
XI_API void __cdecl fn_10036050(float param_1);  // 0x10036050
XI_API void __attribute__((cdecl, regparm(1))) MoTask_KillGlobalSlot1(undefined4 xi_eax, undefined4 xi_s1);  // 0x10036070
XI_API void __cdecl fn_100360A0(float param_1);  // 0x100360A0
XI_API undefined4 __cdecl fn_100360C0(undefined4 xi_s1);  // 0x100360C0
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_100360F0(undefined4 xi_eax, float param_1);  // 0x100360F0
XI_API void __cdecl fn_10036170(int param_1);  // 0x10036170
XI_API void __cdecl fn_100361D0(void);  // 0x100361D0
XI_API void __cdecl fn_10036230(void);  // 0x10036230
XI_API undefined4 __cdecl fn_10036360(int * param_1);  // 0x10036360
XI_API undefined4 __thiscall fn_10036420(void * self_);  // 0x10036420
XI_API void __thiscall fn_100364A0(void * self_);  // 0x100364A0
XI_API undefined4 __thiscall fn_100364E0(void * self_);  // 0x100364E0
XI_API undefined4 __thiscall fn_10036770(void * self_);  // 0x10036770
XI_API undefined4 __thiscall fn_100367C0(void * self_);  // 0x100367C0
XI_API uint32_t __attribute__((cdecl, regparm(1))) fn_100367F0(undefined4 xi_eax, int param_1);  // 0x100367F0
XI_API uint32_t __cdecl fn_10036830(undefined4 * param_1);  // 0x10036830
XI_API undefined4 __cdecl fn_10036870(int * param_1);  // 0x10036870
XI_API undefined4 __thiscall fn_100368B0(void * self_);  // 0x100368B0
XI_API void __thiscall fn_10036980(void * self_);  // 0x10036980
XI_API undefined4 __thiscall fn_100369F0(void * self_);  // 0x100369F0
XI_API undefined4 __thiscall fn_10036B90(void * self_, undefined4 param_1);  // 0x10036B90
XI_API undefined8 __thiscall fn_10036F00(void * self_, undefined4 param_1, undefined4 param_2);  // 0x10036F00
XI_API void __thiscall fn_10037020(void * self_, undefined4 * param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7, undefined4 param_8);  // 0x10037020
XI_API void __cdecl fn_100378B0(int param_1);  // 0x100378B0
XI_API void __cdecl StAvatar_SwitchMusicTask(int param_1);  // 0x10037960
XI_API void __cdecl fn_10037A10(void);  // 0x10037A10
XI_API void __cdecl SetMusicTrackVolume_C(void);  // 0x10037B30
XI_API void __cdecl fn_10037D70(void);  // 0x10037D70
XI_API void __cdecl fn_10037E30(void);  // 0x10037E30
XI_API void __cdecl fn_10037E90(void);  // 0x10037E90
XI_API void __cdecl fn_100380F0(uint32_t param_1, undefined4 param_2, undefined4 param_3);  // 0x100380F0
XI_API void __attribute__((cdecl, regparm(1))) fn_100382A0(undefined4 xi_eax, undefined4 param_1, undefined4 param_2);  // 0x100382A0
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_10038470(undefined4 xi_eax, int param_1, int param_2, undefined4 param_3);  // 0x10038470
XI_API void __cdecl Font_QueueText(char * param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x100384B0
XI_API int __cdecl Font_DrawPass(void);  // 0x10038520
XI_API unsigned __int8 * __cdecl Font_DrawString(unsigned __int8 *a1, float a2, float arg8, float a4);  // 0x10038720
XI_API _DWORD * __cdecl Post_CreateScreenEffectTargets(void);  // 0x10038930
XI_API void __cdecl fn_100389C0(void);  // 0x100389C0
XI_API void __thiscall fn_10038A20(void * self_);  // 0x10038A20
XI_API int __thiscall Post_CaptureSceneToTarget(void *self_);  // 0x10038A80
XI_API undefined4 __thiscall Post_DrawRotatingGhostLayers(void * self_);  // 0x10038E10
XI_API void * __thiscall fn_10039440(void * self_);  // 0x10039440
XI_API void __thiscall ThreadPool_StartThread(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x100394D0
XI_API void __thiscall fn_10039550(void * self_);  // 0x10039550
XI_API undefined4 __thiscall fn_100395D0(void * self_);  // 0x100395D0
XI_API void __thiscall fn_100396A0(void * self_);  // 0x100396A0
XI_API void __thiscall fn_100397A0(void * self_, char * param_1);  // 0x100397A0
XI_API void __thiscall Util_Copy16Bytes(void * self_, int param_1);  // 0x10039810
XI_API void * __thiscall fn_10039830(void * self_);  // 0x10039830
XI_API undefined4 __thiscall TexObj_Constructor(void * self_);  // 0x10039920
XI_API void __thiscall fn_100399D0(void * self_);  // 0x100399D0
XI_API undefined1 __thiscall TexObj_CreateAndUpload(void * self_);  // 0x10039A30
XI_API undefined1 __thiscall TexObj_Reupload(void * self_);  // 0x10039A60
XI_API char __thiscall TexObj_Create(int self_);  // 0x10039A80
XI_API int __thiscall TexObj_Upload(void *self_);  // 0x10039E50
XI_API undefined4 __thiscall fn_1003AA50(void * self_);  // 0x1003AA50
XI_API int __thiscall fn_1003AAB0(void * self_);  // 0x1003AAB0
XI_API int * __thiscall fn_1003AAF0(void * self_);  // 0x1003AAF0
XI_API undefined1 __thiscall TexMgr_UploadPumpEntry(void * self_);  // 0x1003AB70
XI_API void __thiscall TexMgr_Enqueue(void * self_, int param_1);  // 0x1003AB80
XI_API undefined4 __thiscall TexMgr_UploadPump(void * self_);  // 0x1003ABB0
XI_API int __thiscall TexMgr_FindByName(void * self_, int param_1, int param_2, undefined4 param_3);  // 0x1003AEA0
XI_API undefined4 __thiscall fn_1003B420(void * self_);  // 0x1003B420
XI_API void * __thiscall fn_1003B430(void * self_, undefined4 param_1);  // 0x1003B430
XI_API void __thiscall fn_1003B4A0(void * self_);  // 0x1003B4A0
XI_API void __thiscall fn_1003B4D0(void * self_);  // 0x1003B4D0
XI_API void * __thiscall fn_1003B540(void * self_);  // 0x1003B540
XI_API undefined4 __thiscall fn_1003B690(void * self_);  // 0x1003B690
XI_API int __thiscall fn_1003B8B0(void * self_);  // 0x1003B8B0
XI_API int __thiscall fn_1003B900(void * self_);  // 0x1003B900
XI_API void __cdecl fn_1003BD90(undefined4 param_1, undefined4 param_2, int * param_3);  // 0x1003BD90
XI_API double __thiscall fn_1003BE10(int self_, _DWORD *a2, int a3, int a4);  // 0x1003BE10
XI_API void __thiscall fn_1003C6E0(void * self_);  // 0x1003C6E0
XI_API void __thiscall fn_1003C700(void * self_);  // 0x1003C700
XI_API void __thiscall fn_1003C740(void * self_, int param_1);  // 0x1003C740
XI_API long double __attribute__((stdcall, regparm(3))) fn_1003C7D0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, uint8_t * param_2, float param_3);  // 0x1003C7D0
XI_API int __cdecl Sky_DrawDome(void);  // 0x1003D9A0
XI_API int __cdecl Sky_DrawDomeViewCentered(void);  // 0x1003DB60
XI_API undefined4 __thiscall fn_1003E0D0(void * self_, undefined4 * param_1, float * param_2);  // 0x1003E0D0
XI_API void __thiscall fn_1003E1D0(void * self_, char * param_1);  // 0x1003E1D0
XI_API void __thiscall Color_SetFromU32SwapRB(void * self_, undefined4 param_1);  // 0x1003E280
XI_API uint32_t * __thiscall fn_1003E330(void * self_, uint32_t * param_1, float param_2);  // 0x1003E330
XI_API __int64 __thiscall fn_1003E3A0(unsigned __int8 *self_, unsigned __int8 *a2);  // 0x1003E3A0
XI_API __int64 __thiscall fn_1003E450(unsigned __int8 *self_, __int16 *a2);  // 0x1003E450
XI_API int __thiscall fn_1003E500(unsigned __int8 *self_, __int16 *a2);  // 0x1003E500
XI_API void __thiscall fn_1003E630(void * self_, undefined4 param_1);  // 0x1003E630
XI_API void __thiscall fn_1003E640(void * self_, undefined4 param_1);  // 0x1003E640
XI_API void __thiscall fn_1003E650(void * self_, undefined4 param_1);  // 0x1003E650
XI_API void __thiscall fn_1003E660(void *self_, int a1);  // 0x1003E660
XI_API void __thiscall fn_1003E910(void * self_, undefined4 param_1, undefined4 param_2, int param_3);  // 0x1003E910
XI_API int __thiscall fn_1003E9A0(int self_);  // 0x1003E9A0
XI_API undefined4 __thiscall fn_1003FCB0(void * self_);  // 0x1003FCB0
XI_API int __thiscall fn_1003FCE0(void * self_);  // 0x1003FCE0
XI_API int __thiscall fn_1003FCF0(void * self_);  // 0x1003FCF0
XI_API int __thiscall MoElem_GetVertexData(void * self_);  // 0x1003FD70
XI_API void __thiscall fn_10040220(void * self_, uint32_t * param_1, int param_2);  // 0x10040220
XI_API void __thiscall fn_10040BA0(void * self_);  // 0x10040BA0
XI_API void __thiscall CMoElemFamily_SharedEarlyExit(void * self_);  // 0x10040C20
XI_API void * __thiscall CXi471011C8_VT_1032AD80_DeletingDtor(void * self_, undefined4 param_1);  // 0x10041130
XI_API void __thiscall fn_10041160(void * self_);  // 0x10041160
XI_API undefined8 __thiscall CXiSub_10041E70_Ctor(void * self_, int param_1);  // 0x10041F40
XI_API _DWORD * __thiscall fn_100426F0(float *self_);  // 0x100426F0
XI_API int __thiscall fn_100431B0(void *self_);  // 0x100431B0
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_10043DD0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x10043DD0
XI_API undefined4 __cdecl fn_10043DE0(void);  // 0x10043DE0
XI_API undefined8 __thiscall DynVB_Constructor(void * self_);  // 0x10043DF0
XI_API int __thiscall DynVB_Create(_DWORD *self_, int a2, unsigned int a3, int a4);  // 0x10043E50
XI_API void __thiscall fn_10043F00(void * self_);  // 0x10043F00
XI_API int __thiscall CMoVuElem_Draw(int *self_, int a2, int a3, int a1, unsigned int a5, int a6);  // 0x10043F40
XI_API void __thiscall DynVB_Reserve(void * self_, int param_1);  // 0x10044010
XI_API undefined1 * __thiscall DynVB_Lock(void * self_);  // 0x10044130
XI_API void __thiscall DynVB_Unlock(void * self_);  // 0x100441A0
XI_API int __thiscall fn_100441C0(_DWORD *self_);  // 0x100441C0
XI_API int __thiscall Gfx_SetViewMatrixFromField10(int self_);  // 0x10044210
XI_API int __thiscall Gfx_SetViewMatrixFromField90(int self_);  // 0x10044230
XI_API _DWORD * __cdecl Mo_InitSharedRenderResources(void);  // 0x10044270
XI_API void __cdecl fn_100446A0(int param_1);  // 0x100446A0
XI_API undefined4 __thiscall fn_10044AA0(void * self_, int param_1);  // 0x10044AA0
XI_API unsigned char __thiscall fn_10044AC0(void * self_);  // 0x10044AC0
XI_API undefined4 __thiscall fn_10044AE0(void * self_);  // 0x10044AE0
XI_API void __thiscall fn_10044C20(void * self_, int param_1);  // 0x10044C20
XI_API undefined4 __thiscall fn_10044C40(void * self_, float * param_1);  // 0x10044C40
XI_API undefined4 __thiscall fn_10044CF0(void * self_);  // 0x10044CF0
XI_API void __thiscall fn_10044D40(void * self_);  // 0x10044D40
XI_API void __thiscall fn_10044F10(int self_);  // 0x10044F10
XI_API void __cdecl fn_10045800(float * param_1);  // 0x10045800
XI_API undefined4 __thiscall fn_100458F0(void * self_);  // 0x100458F0
XI_API bool __thiscall fn_10045920(int self_);  // 0x10045920
XI_API undefined4 __thiscall fn_10045A40(void * self_);  // 0x10045A40
XI_API CMoProcessor * __thiscall fn_10045C50(_DWORD *self_);  // 0x10045C50
XI_API void __thiscall RegisterWithProcessor(void * self_);  // 0x10045D40
XI_API void __thiscall fn_10046070(void * self_);  // 0x10046070
XI_API void __cdecl fn_10046380(void);  // 0x10046380
XI_API int __cdecl fn_100464D0(void);  // 0x100464D0
XI_API int __thiscall fn_100467E0(void *self_, int a1);  // 0x100467E0
XI_API int __thiscall fn_100472F0(_WORD *a1, __int16 a4);  // 0x100472F0
XI_API int __thiscall fn_100473F0(void * self_);  // 0x100473F0
XI_API int __thiscall fn_10047450(void * self_);  // 0x10047450
XI_API int __thiscall fn_10047490(void * self_);  // 0x10047490
XI_API unsigned __int8 * __cdecl fn_100474D0(void);  // 0x100474D0
XI_API undefined * __thiscall fn_10047B80(void * self_);  // 0x10047B80
XI_API void __thiscall fn_100480D0(void * self_);  // 0x100480D0
XI_API void __thiscall fn_10048220(void * self_);  // 0x10048220
XI_API uint32_t * __thiscall fn_10049330(void * self_, uint32_t param_1);  // 0x10049330
XI_API int __thiscall fn_100493B0(void * self_);  // 0x100493B0
XI_API void __thiscall fn_10049520(void * self_, uint32_t * param_1);  // 0x10049520
XI_API bool __cdecl fn_1004A870(int a1, int a2, __int16 a3, __int16 *a4);  // 0x1004A870
XI_API undefined4 __thiscall fn_1004E210(void * self_);  // 0x1004E210
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_1004E250(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, undefined4 param_2);  // 0x1004E250
XI_API int __thiscall fn_1004E260(void * self_);  // 0x1004E260
XI_API int __thiscall fn_1004E270(void * self_);  // 0x1004E270
XI_API undefined4 __thiscall fn_1004E280(void * self_);  // 0x1004E280
XI_API int __thiscall fn_1004E290(void * self_);  // 0x1004E290
XI_API int __thiscall fn_1004E2A0(void * self_);  // 0x1004E2A0
XI_API int __thiscall fn_1004E2B0(void * self_);  // 0x1004E2B0
XI_API undefined4 __thiscall fn_1004E2C0(void * self_);  // 0x1004E2C0
XI_API int __thiscall fn_1004E2D0(void * self_);  // 0x1004E2D0
XI_API void __thiscall fn_1004E320(void * self_, undefined4 * param_1);  // 0x1004E320
XI_API void __thiscall fn_1004E360(void * self_, float * param_1);  // 0x1004E360
XI_API undefined8 __thiscall fn_1004E3B0(void * self_, int param_1);  // 0x1004E3B0
XI_API undefined4 __thiscall Pool_FreeSlot(void * self_);  // 0x1004E3F0
XI_API undefined4 __thiscall fn_1004E470(void * self_);  // 0x1004E470
XI_API uint32_t __thiscall fn_1004E490(void * self_);  // 0x1004E490
XI_API long double __thiscall fn_1004E4E0(void * self_, float param_2);  // 0x1004E4E0
XI_API double __cdecl fn_1004E630(int a1, int a2, __int16 *a3);  // 0x1004E630
XI_API void __thiscall fn_1004E720(_DWORD *self_, int a2);  // 0x1004E720
XI_API undefined4 __thiscall fn_100527B0(void * self_);  // 0x100527B0
XI_API void __thiscall fn_100527C0(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x100527C0
XI_API void __thiscall fn_100527E0(void * self_, undefined4 xi_s1);  // 0x100527E0
XI_API void __thiscall fn_100527F0(void * self_);  // 0x100527F0
XI_API void __thiscall fn_10052800(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x10052800
XI_API undefined4 __thiscall fn_10052820(void * self_, float * param_1, float * param_2);  // 0x10052820
XI_API undefined4 __thiscall fn_10052870(void * self_, undefined4 * param_1, undefined4 param_2);  // 0x10052870
XI_API void __thiscall fn_10052880(void * self_, undefined4 param_1);  // 0x10052880
XI_API undefined4 __thiscall fn_10052890(void * self_);  // 0x10052890
XI_API undefined4 __thiscall fn_100528A0(void * self_);  // 0x100528A0
XI_API int __thiscall fn_100528B0(void * self_);  // 0x100528B0
XI_API undefined4 __thiscall fn_100528C0(void * self_, undefined4 * param_1);  // 0x100528C0
XI_API undefined4 __thiscall fn_100528E0(void * self_);  // 0x100528E0
XI_API undefined4 __thiscall fn_100528F0(void * self_);  // 0x100528F0
XI_API int __thiscall fn_10052900(void * self_);  // 0x10052900
XI_API void __thiscall fn_10052910(void * self_);  // 0x10052910
XI_API undefined4 __thiscall fn_10052940(void * self_);  // 0x10052940
XI_API undefined4 __thiscall fn_10052950(void * self_);  // 0x10052950
XI_API void __thiscall fn_10052960(void * self_, undefined4 param_1);  // 0x10052960
XI_API void __thiscall fn_10052980(void * self_, undefined4 param_1);  // 0x10052980
XI_API int __thiscall fn_100529A0(void * self_);  // 0x100529A0
XI_API void __thiscall fn_100529B0(void * self_, uint32_t param_1);  // 0x100529B0
XI_API void __thiscall fn_100529D0(void * self_, undefined4 param_1);  // 0x100529D0
XI_API void __thiscall fn_100529E0(void * self_, undefined4 param_1, undefined4 param_2);  // 0x100529E0
XI_API long double __thiscall fn_10052A00(void * self_);  // 0x10052A00
XI_API void __thiscall fn_10052A10(void * self_, undefined4 param_1);  // 0x10052A10
XI_API void __thiscall fn_10052A20(void * self_, undefined4 param_1);  // 0x10052A20
XI_API void __thiscall fn_10052A30(void * self_, undefined4 param_1);  // 0x10052A30
XI_API long double __thiscall fn_10052A40(void * self_);  // 0x10052A40
XI_API long double __thiscall fn_10052A50(void * self_);  // 0x10052A50
XI_API undefined8 __cdecl fn_10052A60(int param_1, int param_2);  // 0x10052A60
XI_API undefined4 __thiscall fn_10052A80(void * self_);  // 0x10052A80
XI_API int __thiscall fn_10052A90(void * self_);  // 0x10052A90
XI_API undefined4 __thiscall fn_10052AB0(void * self_);  // 0x10052AB0
XI_API void __thiscall fn_10052AC0(void * self_, undefined4 param_1);  // 0x10052AC0
XI_API undefined4 __thiscall fn_10052AD0(void * self_);  // 0x10052AD0
XI_API int __thiscall fn_10052AE0(void * self_);  // 0x10052AE0
XI_API undefined4 __thiscall fn_10052AF0(void * self_);  // 0x10052AF0
XI_API undefined4 __thiscall fn_10052B00(void * self_);  // 0x10052B00
XI_API int __thiscall fn_10052B10(void * self_);  // 0x10052B10
XI_API int __thiscall fn_10052B20(void * self_);  // 0x10052B20
XI_API undefined4 __thiscall fn_10052B30(void * self_, undefined4 * param_1);  // 0x10052B30
XI_API void __thiscall fn_10052B40(void * self_, undefined4 param_1);  // 0x10052B40
XI_API void __thiscall fn_10052B50(void * self_, undefined4 param_1);  // 0x10052B50
XI_API void __thiscall fn_10052B60(void * self_, undefined4 param_1);  // 0x10052B60
XI_API void __thiscall fn_10052B70(void * self_, float param_1);  // 0x10052B70
XI_API void __thiscall fn_10052B80(void * self_, int param_1);  // 0x10052B80
XI_API void __thiscall fn_10052B90(void * self_, float param_1);  // 0x10052B90
XI_API void __thiscall fn_10052BA0(void * self_, undefined4 param_1);  // 0x10052BA0
XI_API void __thiscall fn_10052BB0(void * self_, undefined4 param_1);  // 0x10052BB0
XI_API undefined8 __thiscall fn_10052BC0(void * self_);  // 0x10052BC0
XI_API undefined8 __thiscall fn_10052C30(void * self_);  // 0x10052C30
XI_API undefined8 __thiscall fn_10052C90(void * self_);  // 0x10052C90
XI_API undefined8 __thiscall fn_10052CD0(void * self_);  // 0x10052CD0
XI_API long double __cdecl fn_10052D20(void);  // 0x10052D20
XI_API undefined4 __thiscall fn_10052D30(void * self_);  // 0x10052D30
XI_API undefined8 __thiscall fn_10052D40(void * self_);  // 0x10052D40
XI_API undefined8 __thiscall fn_10052DA0(void * self_);  // 0x10052DA0
XI_API undefined8 __thiscall fn_10052E20(void * self_);  // 0x10052E20
XI_API void __thiscall fn_10052E80(void * self_, undefined4 param_1);  // 0x10052E80
XI_API undefined8 __thiscall fn_10052F20(void * self_);  // 0x10052F20
XI_API undefined8 __thiscall fn_10052FD0(void * self_);  // 0x10052FD0
XI_API undefined8 __thiscall fn_10053030(void * self_);  // 0x10053030
XI_API void __thiscall fn_10053080(void * self_, undefined4 param_1);  // 0x10053080
XI_API void __thiscall fn_10053090(void * self_, undefined4 param_1);  // 0x10053090
XI_API long double __thiscall fn_100530A0(void * self_);  // 0x100530A0
XI_API long double __thiscall fn_100530B0(void * self_);  // 0x100530B0
XI_API undefined4 __thiscall fn_100530C0(void * self_);  // 0x100530C0
XI_API void __thiscall fn_100530D0(void * self_, undefined4 param_1);  // 0x100530D0
XI_API void __thiscall fn_100530E0(void * self_, undefined4 param_1);  // 0x100530E0
XI_API undefined8 __thiscall fn_100530F0(void * self_, undefined4 param_1);  // 0x100530F0
XI_API undefined8 __thiscall fn_10053180(void * self_);  // 0x10053180
XI_API void __thiscall SharedConditionalStateReset(void * self_);  // 0x100531D0
XI_API undefined4 __thiscall fn_10053370(void * self_);  // 0x10053370
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_10053890(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, undefined4 param_2);  // 0x10053890
XI_API char __thiscall fn_100538E0(int self_, int *a2);  // 0x100538E0
XI_API undefined4 __thiscall fn_100539E0(void * self_);  // 0x100539E0
XI_API undefined4 __thiscall fn_10053A00(void * self_, int param_1, int param_2, undefined4 param_3, undefined4 * param_4);  // 0x10053A00
XI_API undefined4 __thiscall fn_10053C80(void * self_, int param_1, float * param_2);  // 0x10053C80
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_10053DD0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, int param_2);  // 0x10053DD0
XI_API void __thiscall fn_10054220(void * self_);  // 0x10054220
XI_API undefined4 __cdecl fn_10054410(void);  // 0x10054410
XI_API void __cdecl fn_10054430(undefined4 * param_1, undefined4 * param_2);  // 0x10054430
XI_API long double __thiscall fn_100544D0(void * self_, undefined4 param_2, undefined4 param_3);  // 0x100544D0
XI_API long double __thiscall fn_10054500(void * self_, float param_2);  // 0x10054500
XI_API long double __thiscall fn_100546F0(void * self_, float param_2, undefined4 param_3);  // 0x100546F0
XI_API long double __thiscall fn_100547A0(void * self_, float param_2);  // 0x100547A0
XI_API char __cdecl fn_10054D70(void);  // 0x10054D70
XI_API float * __stdcall fn_10054F80(float *a1, int a2);  // 0x10054F80
XI_API void __thiscall fn_10054FC0(void * self_);  // 0x10054FC0
XI_API void __attribute__((stdcall, regparm(3))) fn_100550F0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x100550F0
XI_API void __thiscall LinkNode_Init(void * self_);  // 0x10055240
XI_API void * __thiscall fn_100552A0(void * self_);  // 0x100552A0
XI_API void __thiscall fn_100552F0(void * self_);  // 0x100552F0
XI_API void __thiscall fn_10055300(void * self_, int param_1, float param_2, float param_3);  // 0x10055300
XI_API void __thiscall fn_100554B0(void * self_);  // 0x100554B0
XI_API void __thiscall fn_10055520(void * self_);  // 0x10055520
XI_API undefined8 __thiscall fn_10055570(void * self_);  // 0x10055570
XI_API void __cdecl fn_100555D0(void);  // 0x100555D0
XI_API void __cdecl fn_10055620(void);  // 0x10055620
XI_API void __thiscall fn_10055640(void * self_);  // 0x10055640
XI_API char ** __cdecl CMoRabRes_GetRuntimeClass(void);  // 0x10056260
XI_API undefined8 __thiscall fn_100562F0(void * self_, uint32_t param_1, undefined4 param_2);  // 0x100562F0
XI_API void __thiscall fn_10056400(void * self_);  // 0x10056400
XI_API void __thiscall fn_10056AA0(void * self_);  // 0x10056AA0
XI_API int __thiscall fn_10056CE0(void * self_, int param_1, int param_2, undefined4 param_3, undefined4 param_4);  // 0x10056CE0
XI_API void __thiscall fn_10056EB0(void * self_);  // 0x10056EB0
XI_API void * __thiscall fn_10057270(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x10057270
XI_API void __thiscall fn_100573B0(void * self_);  // 0x100573B0
XI_API void __thiscall fn_100573E0(void * self_, int param_1, uint32_t param_2, undefined4 param_3);  // 0x100573E0
XI_API void __thiscall fn_10057CC0(void * self_, short * param_1);  // 0x10057CC0
XI_API undefined4 __thiscall fn_1005E110(void * self_, undefined4 xi_s1);  // 0x1005E110
XI_API undefined4 __thiscall fn_1005EEF0(void * self_, undefined4 param_1);  // 0x1005EEF0
XI_API char __thiscall fn_10061110(float *self_);  // 0x10061110
XI_API undefined8 __thiscall fn_10062770(void * self_);  // 0x10062770
XI_API undefined8 __thiscall fn_100627D0(void * self_);  // 0x100627D0
XI_API void __thiscall fn_10062830(void * self_, int param_1);  // 0x10062830
XI_API void __thiscall fn_10062880(void * self_, int param_1);  // 0x10062880
XI_API undefined8 __cdecl fn_10062E20(undefined4 * param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5);  // 0x10062E20
XI_API undefined8 __cdecl fn_10062E80(undefined4 param_1, undefined4 param_2, int param_3, int param_4);  // 0x10062E80
XI_API undefined4 __thiscall fn_10063460(void * self_);  // 0x10063460
XI_API void __thiscall fn_100634B0(void * self_, int param_1, uint32_t param_2, undefined4 param_3, undefined4 param_4);  // 0x100634B0
XI_API void __thiscall fn_10063550(void * self_);  // 0x10063550
XI_API long double __attribute__((stdcall, regparm(3))) fn_10063890(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, float param_2, int param_3);  // 0x10063890
XI_API void __thiscall fn_10063A60(void * self_, float param_1, float * param_2);  // 0x10063A60
XI_API void __thiscall fn_10063B20(void * self_);  // 0x10063B20
XI_API int * __thiscall fn_10063B50(void * self_, int param_1);  // 0x10063B50
XI_API int __thiscall fn_10063B70(void * self_);  // 0x10063B70
XI_API undefined4 __thiscall fn_10063B80(void * self_);  // 0x10063B80
XI_API void __thiscall fn_10063C30(void * self_, undefined4 param_1, undefined4 param_2);  // 0x10063C30
XI_API int __thiscall fn_10063C80(__int16 *self_, void (__cdecl *a2)(int, float *, int), int a3);  // 0x10063C80
XI_API void __thiscall fn_100684F0(void * self_, undefined4 xi_s1);  // 0x100684F0
XI_API void __thiscall fn_10068590(void * self_, undefined4 * param_1, undefined4 param_2);  // 0x10068590
XI_API void __cdecl fn_10069E90(void);  // 0x10069E90
XI_API void __thiscall fn_10069EA0(void * self_);  // 0x10069EA0
XI_API void __thiscall fn_10069F90(void * self_);  // 0x10069F90
XI_API unsigned int __stdcall fn_1006A020(unsigned int a1, int a2, int a3, int a4);  // 0x1006A020
XI_API undefined4 __thiscall fn_1006A180(void * self_, undefined4 xi_s1, undefined4 xi_s2, undefined4 xi_s3, undefined4 xi_s4);  // 0x1006A180
XI_API void __thiscall fn_1006A540(void * self_, undefined4 * param_1);  // 0x1006A540
XI_API int __cdecl Gfx_CalcFVFStride(uint32_t param_1);  // 0x1006A560
XI_API int __thiscall fn_1006AA30(_DWORD *self_, int arg0);  // 0x1006AA30
XI_API void __cdecl fn_1006AD30(int param_1, undefined4 * param_2);  // 0x1006AD30
XI_API int __cdecl fn_1006AD50(int param_1, int param_2);  // 0x1006AD50
XI_API void __thiscall fn_1006AD90(void * self_);  // 0x1006AD90
XI_API void __thiscall fn_1006ADC0(int *self_);  // 0x1006ADC0
XI_API void __thiscall fn_1006ADF0(void * self_);  // 0x1006ADF0
XI_API int __thiscall fn_1006AF00(_DWORD *self_, int *a2);  // 0x1006AF00
XI_API int __thiscall StateBlock_ApplyTSS(void *self_, int a1);  // 0x1006AF90
XI_API void * __thiscall VBPool_Constructor(void * self_);  // 0x1006B0C0
XI_API uint32_t __thiscall fn_1006B120(void * self_, undefined4 * param_1);  // 0x1006B120
XI_API int __thiscall fn_1006B1D0(void * self_, int param_1);  // 0x1006B1D0
XI_API void __cdecl Rand_SeedMT19937(uint32_t param_1);  // 0x1006BD90
XI_API uint32_t __cdecl fn_1006BE20(void);  // 0x1006BE20
XI_API int __cdecl fn_1006BF40(int param_1);  // 0x1006BF40
XI_API uint32_t __cdecl fn_1006BF60(uint32_t param_1);  // 0x1006BF60
XI_API long double __attribute__((cdecl, regparm(1))) fn_1006BF80(undefined4 xi_eax, float param_1);  // 0x1006BF80
XI_API long double __attribute__((cdecl, regparm(1))) fn_1006BFD0(undefined4 xi_eax, float param_1);  // 0x1006BFD0
XI_API int __cdecl Mem_AllocWithHeader12(int param_1, undefined4 param_2);  // 0x1006C0F0
XI_API void __thiscall j_OcclusionMng_CreateTarget(void * self_);  // 0x1006C210
XI_API void __thiscall OcclusionQuery_Init(void * self_);  // 0x1006C260
XI_API int __thiscall Occlusion_QueryScreenRect(__int16 *self_, float *a2, int a3, int a4, int a5, int a6, int a7, int a8, char a9, int a10);  // 0x1006C280
XI_API int __thiscall Occlusion_QueryScreenRectImpl(__int16 *self_, float *arg0, int a2, int a5, int a6, int a7, int a8, int _60, char a9, int a10);  // 0x1006C300
XI_API int __stdcall Occlusion_DrawQuery(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);  // 0x1006C530
XI_API int __thiscall Occlusion_ReadbackCount(__int16 *self_, char a2);  // 0x1006C8B0
XI_API int __thiscall OcclusionMng_Prepare(_DWORD *self_);  // 0x1006C9B0
XI_API undefined4 __cdecl fn_1006CA20(float param_1);  // 0x1006CA20
XI_API undefined4 __cdecl fn_1006CA60(undefined4 * param_1);  // 0x1006CA60
XI_API void __attribute__((stdcall, regparm(1))) fn_1006D0A0(undefined4 xi_eax, undefined4 * param_1);  // 0x1006D0A0
XI_API void __stdcall fn_1006D270(float * param_1, int param_2, float * param_3);  // 0x1006D270
XI_API void __stdcall fn_1006D3E0(int param_1);  // 0x1006D3E0
XI_API long double __stdcall fn_1006D580(float * param_1);  // 0x1006D580
XI_API void __attribute__((stdcall, regparm(1))) CMoProcessor_Vec3Normalize(undefined4 xi_eax, float * param_1);  // 0x1006D5D0
XI_API void __stdcall fn_1006D720(float * param_1, float * param_2, float * param_3);  // 0x1006D720
XI_API void __stdcall fn_1006E140(undefined4 * param_1, float param_2);  // 0x1006E140
XI_API void __stdcall fn_1006E260(undefined4 * param_1, float param_2);  // 0x1006E260
XI_API void __thiscall fn_1006EF70(void * self_);  // 0x1006EF70
XI_API float * __thiscall Mat4x4_SafeTranspose(void * self_, float * param_1, undefined4 param_2, float * param_3);  // 0x1006F0A0
XI_API void __stdcall fn_1006F560(undefined8 * param_1, undefined4 * param_2);  // 0x1006F560
XI_API float * __stdcall fn_1006F800(float * param_1, float * param_2, float * param_3);  // 0x1006F800
XI_API undefined4 __thiscall fn_1006FAE0(void * self_, undefined4 param_1, float param_2, float param_3, float param_4);  // 0x1006FAE0
XI_API undefined4 __thiscall fn_1006FC40(void * self_, undefined4 param_1, float param_2, float param_3, float param_4);  // 0x1006FC40
XI_API void __thiscall fn_100700A0(void * self_, undefined4 * param_1);  // 0x100700A0
XI_API void __thiscall fn_100700E0(void * self_);  // 0x100700E0
XI_API void * __thiscall fn_10070F40(void * self_);  // 0x10070F40
XI_API void __thiscall fn_10070F90(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x10070F90
XI_API undefined4 __thiscall ResHandle_FreeIfUnreferenced(void * self_);  // 0x10071050
XI_API undefined4 __thiscall ResourceHandle_IsResident(void * self_);  // 0x10071060
XI_API undefined4 __thiscall fn_100710F0(void * self_, undefined4 param_1, undefined4 param_2, int param_3, int param_4);  // 0x100710F0
XI_API void __thiscall fn_100711B0(void * self_);  // 0x100711B0
XI_API void __thiscall fn_100711D0(void * self_, int param_1);  // 0x100711D0
XI_API void __thiscall fn_100711E0(void * self_, int param_1);  // 0x100711E0
XI_API void __thiscall fn_10071200(void * self_, int param_1);  // 0x10071200
XI_API undefined4 __thiscall fn_10071FD0(void * self_);  // 0x10071FD0
XI_API undefined4 __thiscall WeatherRes_GetTimeKey(void * self_, undefined4 * param_1);  // 0x10072180
XI_API undefined4 __attribute__((stdcall, regparm(3))) CMoResource_Destroy(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x100721F0
XI_API void * __thiscall fn_10072240(void * self_);  // 0x10072240
XI_API undefined8 __thiscall fn_100722C0(void * self_, undefined4 * param_1, uint32_t param_2, int param_3, int param_4);  // 0x100722C0
XI_API undefined4 __attribute__((stdcall, regparm(1))) CMoResource_FindInScope(undefined4 xi_eax, undefined4 * param_1, int param_2, uint32_t param_3, int param_4);  // 0x10072430
XI_API undefined4 __attribute__((stdcall, regparm(3))) YmResourceFile_FindResourceUnder(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, undefined4 * param_2, uint32_t param_3, int param_4);  // 0x100724C0
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_10072540(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, uint32_t param_2, int param_3, undefined * param_4, undefined4 param_5);  // 0x10072540
XI_API void __cdecl unknown_libname_61(void);  // 0x100729C0
XI_API void __cdecl fn_100729D0(void);  // 0x100729D0
XI_API void __thiscall fn_10072A70(void * self_);  // 0x10072A70
XI_API undefined8 __thiscall fn_10072C30(void * self_, undefined4 * param_1, undefined4 param_2);  // 0x10072C30
XI_API void __thiscall MeshResourceMng_LoadSystemResources(void * self_, undefined4 * param_1);  // 0x10072ED0
XI_API undefined4 __stdcall ResourceHandle_Deref(undefined4 * param_1);  // 0x10072FB0
XI_API undefined4 * __stdcall fn_100730E0(undefined4 * param_1, int param_2);  // 0x100730E0
XI_API undefined4 __thiscall MeshResourceMng_ResolveTree(void * self_, int * param_1, void * param_2, undefined4 * param_3);  // 0x10073430
XI_API void __stdcall fn_10073A60(int * param_1, undefined4 param_2);  // 0x10073A60
XI_API undefined4 __stdcall MoRes_FreeIfUnreferenced(int * param_1);  // 0x10073B20
XI_API undefined4 __stdcall fn_10073B80(int * param_1);  // 0x10073B80
XI_API undefined4 __stdcall fn_10073BA0(undefined4 * param_1, int param_2, int param_3);  // 0x10073BA0
XI_API undefined4 __thiscall MeshResourceMng_Evict(void * self_);  // 0x100740C0
XI_API undefined8 __thiscall fn_10074110(void * self_, undefined4 * param_1);  // 0x10074110
XI_API void __thiscall MeshResourceMng_AllocHandle(void * self_, int * param_1, int param_2);  // 0x10074140
XI_API void __thiscall NodePool_ReleaseNode(void * self_, uint32_t * param_1);  // 0x10074180
XI_API undefined4 __thiscall fn_100741E0(void * self_, undefined4 * param_1);  // 0x100741E0
XI_API void __cdecl fn_10074280(undefined4 param_1);  // 0x10074280
XI_API void __thiscall ResourceIter_FindFirst(void * self_, int param_1, int param_2, uint32_t param_3, uint32_t param_4);  // 0x100742B0
XI_API void __thiscall fn_10074310(void * self_, undefined4 param_1, int param_2, uint32_t param_3, uint32_t param_4);  // 0x10074310
XI_API uint32_t __thiscall fn_10074380(void * self_);  // 0x10074380
XI_API void __thiscall fn_100743D0(void * self_);  // 0x100743D0
XI_API undefined4 __thiscall ResourceIter_FindNext(void * self_, undefined4 xi_s1);  // 0x100745C0
XI_API void __thiscall MoProcessor_CallVSlot49(void * self_);  // 0x100746A0
XI_API void __thiscall fn_10074700(void * self_);  // 0x10074700
XI_API void __thiscall fn_100747F0(void * self_, int param_1);  // 0x100747F0
XI_API void __thiscall fn_10074890(void * self_);  // 0x10074890
XI_API undefined4 * __thiscall fn_10074920(void * self_, uint32_t param_1);  // 0x10074920
XI_API void __thiscall fn_10074AB0(void * self_);  // 0x10074AB0
XI_API void __thiscall fn_10074AD0(void * self_, int param_1);  // 0x10074AD0
XI_API void __thiscall fn_10074BA0(void * self_);  // 0x10074BA0
XI_API void __thiscall fn_10074CF0(void * self_, int param_1, int param_2);  // 0x10074CF0
XI_API int __thiscall fn_10074DF0(void * self_, undefined4 param_1);  // 0x10074DF0
XI_API undefined8 __thiscall fn_10074F80(void * self_, int param_1, int param_2);  // 0x10074F80
XI_API void __cdecl fn_10075000(int * param_1);  // 0x10075000
XI_API undefined8 __thiscall fn_100750B0(void * self_, int param_1);  // 0x100750B0
XI_API int __thiscall fn_10075120(void * self_, int param_1);  // 0x10075120
XI_API void __thiscall fn_100751B0(void * self_, int param_1);  // 0x100751B0
XI_API int __cdecl Gfx_InitCubeMapTextures(void);  // 0x10075280
XI_API void __thiscall TexRef_ResolveByName(void * self_, int param_1);  // 0x10075520
XI_API void __thiscall TexRef_EnsureRenderTarget(int *self_);  // 0x100755A0
XI_API int __thiscall fn_10075700(int *self_, int a2, float a3, float a4, float a5);  // 0x10075700
XI_API int __thiscall TexRef_SetTexture(int *self_, int a2);  // 0x10075960
XI_API int __thiscall fn_100759C0(int *self_, int a2, int a3);  // 0x100759C0
XI_API int __thiscall fn_10076030(int *self_, float *a2, char arg4);  // 0x10076030
XI_API int __thiscall fn_10076520(int *self_, float *a2, float *a3, char a4);  // 0x10076520
XI_API void __thiscall fn_10076750(void * self_, undefined4 * param_1);  // 0x10076750
XI_API int __cdecl fn_10076830(char * param_1, char * param_2, undefined4 * param_3, undefined4 * param_4, int param_5, undefined4 param_6);  // 0x10076830
XI_API int __cdecl fn_100769F0(char * param_1, char * param_2, int param_3);  // 0x100769F0
XI_API char * __cdecl fn_10076A70(char * param_1);  // 0x10076A70
XI_API int __cdecl preprocessArg(char * param_1, char * param_2, int param_3);  // 0x10076AB0
XI_API void __attribute__((cdecl, regparm(1))) fn_10077A00(undefined4 xi_eax);  // 0x10077A00
XI_API uint8_t * __cdecl fn_10077AD0(uint8_t * param_1, uint8_t * param_2);  // 0x10077AD0
XI_API undefined4 __cdecl fn_10077B40(undefined4 param_1, char * param_2);  // 0x10077B40
XI_API undefined4 __cdecl fn_10077C30(uint8_t * param_1);  // 0x10077C30
XI_API void __cdecl fn_10077CF0(int * param_1);  // 0x10077CF0
XI_API undefined4 * __cdecl fn_10077D40(void);  // 0x10077D40
XI_API void __cdecl fn_10077D70(void);  // 0x10077D70
XI_API undefined4 __cdecl fn_10077D80(void);  // 0x10077D80
XI_API undefined4 __cdecl fn_10077E30(undefined4 param_1, undefined4 xi_s2);  // 0x10077E30
XI_API void __cdecl fn_10077FB0(void);  // 0x10077FB0
XI_API undefined4 __cdecl fn_100780A0(undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5);  // 0x100780A0
XI_API undefined4 __cdecl fn_100780D0(undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7, undefined4 param_8);  // 0x100780D0
XI_API int __cdecl fn_10079A00(char * param_1);  // 0x10079A00
XI_API int __cdecl fn_1007A440(int param_1, int param_2, int param_3);  // 0x1007A440
XI_API int __cdecl fn_1007A4C0(char * param_1, uint32_t param_2, int param_3, int param_4, undefined4 param_5);  // 0x1007A4C0
XI_API int __cdecl fn_1007CD60(undefined4 param_1, undefined4 param_2, int param_3, undefined4 param_4);  // 0x1007CD60
XI_API int __cdecl fn_1007CFD0(undefined4 param_1, undefined4 param_2, int param_3);  // 0x1007CFD0
XI_API undefined4 __cdecl fn_10080C70(char * param_1);  // 0x10080C70
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_10080D00(undefined4 xi_eax);  // 0x10080D00
XI_API undefined4 __cdecl fn_10080D20(int param_1);  // 0x10080D20
XI_API uint32_t __cdecl fn_10080D90(int param_1);  // 0x10080D90
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_10081230(undefined4 xi_eax, int param_1);  // 0x10081230
XI_API void __cdecl fn_10081440(undefined4 param_1);  // 0x10081440
XI_API undefined4 __cdecl fn_10081480(undefined4 param_1);  // 0x10081480
XI_API void __thiscall ActorHandle_Constructor(void * self_);  // 0x100814A0
XI_API void __thiscall ActorHandle_Destructor(void * self_);  // 0x100814E0
XI_API undefined4 __thiscall fn_10081530(void * self_, undefined4 param_1, undefined4 param_2);  // 0x10081530
XI_API undefined4 __thiscall fn_10081600(void * self_);  // 0x10081600
XI_API void __cdecl fn_10081A30(void);  // 0x10081A30
XI_API void __cdecl Actor_ResetDrawLists(void);  // 0x10081A50
XI_API void __attribute__((stdcall, regparm(3))) fn_10081AE0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x10081AE0
XI_API void * __thiscall fn_10081CF0(void * self_);  // 0x10081CF0
XI_API int __thiscall GetEmbeddedMember_0x34(void * self_);  // 0x100820E0
XI_API int __thiscall GetEmbeddedMember_0x44(void * self_);  // 0x100820F0
XI_API void __thiscall CXiActor_GetModel(void * self_, undefined4 * param_1);  // 0x10082200
XI_API undefined4 __thiscall CXiActor_GetFieldA8(void * self_);  // 0x10082380
XI_API void __thiscall CXiActor_SetSubActorStatus(void * self_, undefined4 param_1);  // 0x10082790
XI_API undefined4 __thiscall fn_100827F0(void * self_);  // 0x100827F0
XI_API undefined4 __thiscall fn_10082860(void * self_);  // 0x10082860
XI_API undefined4 __thiscall fn_100828B0(void * self_);  // 0x100828B0
XI_API undefined4 __thiscall fn_10082900(void * self_);  // 0x10082900
XI_API int __cdecl fn_10082BF0(void);  // 0x10082BF0
XI_API undefined1 * __thiscall fn_10082F60(void * self_);  // 0x10082F60
XI_API undefined4 __thiscall fn_10082FA0(void * self_);  // 0x10082FA0
XI_API uint32_t __thiscall fn_10083340(void * self_);  // 0x10083340
XI_API undefined4 __cdecl fn_10083430(void * param_1, int param_2, uint32_t param_3, uint32_t param_4, undefined4 param_5);  // 0x10083430
XI_API int * __cdecl fn_10083750(float * param_1, uint32_t param_2, uint32_t param_3);  // 0x10083750
XI_API int * __cdecl fn_10083B40(int * param_1, uint32_t param_2, float param_3);  // 0x10083B40
XI_API int * __cdecl fn_10083D00(int * param_1, uint32_t param_2, float param_3);  // 0x10083D00
XI_API undefined4 __cdecl fn_10083EC0(undefined4 param_1, undefined4 param_2);  // 0x10083EC0
XI_API undefined4 __cdecl fn_10083EE0(uint32_t param_1, uint32_t param_2);  // 0x10083EE0
XI_API undefined4 __cdecl fn_10083F00(uint32_t param_1, uint32_t param_2);  // 0x10083F00
XI_API int * __cdecl fn_10083F20(uint32_t param_1, uint32_t param_2, int param_3);  // 0x10083F20
XI_API undefined4 __cdecl fn_10084100(int param_1, int param_2, float param_3);  // 0x10084100
XI_API uint32_t __attribute__((stdcall, regparm(3))) CXiActor_ModelTest95790(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, int param_2);  // 0x10084330
XI_API undefined4 __attribute__((stdcall, regparm(3))) CXiActor_ModelTest95680(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, int param_2);  // 0x10084350
XI_API undefined4 __attribute__((stdcall, regparm(3))) CXiActor_ModelTest956A0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, int param_2);  // 0x10084370
XI_API undefined4 __thiscall fn_100845E0(void * self_);  // 0x100845E0
XI_API uint32_t __thiscall fn_10084670(void * self_);  // 0x10084670
XI_API undefined4 __thiscall fn_10084690(void * self_);  // 0x10084690
XI_API long double __thiscall fn_100849A0(void * self_);  // 0x100849A0
XI_API uint32_t __thiscall fn_10084BA0(void * self_);  // 0x10084BA0
XI_API undefined4 __thiscall fn_10084BC0(void * self_);  // 0x10084BC0
XI_API void __thiscall fn_10085010(void * self_, undefined4 param_1);  // 0x10085010
XI_API uint32_t __thiscall fn_10085500(void * self_);  // 0x10085500
XI_API void __thiscall fn_10085BF0(void * self_, undefined4 param_1);  // 0x10085BF0
XI_API void __attribute__((stdcall, regparm(3))) CXiAtelActor_UnlinkCasterAttachments(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, void * param_2);  // 0x10085C90
XI_API void __attribute__((stdcall, regparm(3))) CXiAtelActor_UnlinkTargetAttachments(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, void * param_2);  // 0x10085CF0
XI_API void __thiscall fn_10085D50(void * self_, int param_1);  // 0x10085D50
XI_API undefined4 __thiscall fn_10085D60(void * self_, int param_1);  // 0x10085D60
XI_API void __cdecl Actor_DrawWorldText(_DWORD *a1, const char *a2, float a3, unsigned int a4, int a5);  // 0x10086210
XI_API undefined4 __thiscall fn_10086E90(void * self_);  // 0x10086E90
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_10086FC0(undefined4 xi_eax, uint32_t param_1);  // 0x10086FC0
XI_API undefined8 __cdecl fn_10086FE0(int param_1);  // 0x10086FE0
XI_API uint32_t __cdecl fn_10087010(void);  // 0x10087010
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_10087060(undefined4 xi_eax);  // 0x10087060
XI_API undefined4 __cdecl fn_100870E0(undefined4 param_1, undefined4 param_2);  // 0x100870E0
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_10087110(undefined4 xi_eax);  // 0x10087110
XI_API undefined4 __cdecl fn_100872C0(void);  // 0x100872C0
XI_API void __cdecl fn_100875C0(void);  // 0x100875C0
XI_API uint32_t __cdecl fn_100875D0(void);  // 0x100875D0
XI_API undefined4 __cdecl fn_100875F0(void);  // 0x100875F0
XI_API uint32_t __cdecl fn_10087610(void);  // 0x10087610
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_10087660(undefined4 xi_eax);  // 0x10087660
XI_API undefined4 __cdecl fn_10087670(void);  // 0x10087670
XI_API uint32_t __attribute__((cdecl, regparm(1))) fn_10087690(undefined4 xi_eax);  // 0x10087690
XI_API uint32_t __attribute__((cdecl, regparm(1))) fn_100876B0(undefined4 xi_eax);  // 0x100876B0
XI_API uint32_t __attribute__((cdecl, regparm(1))) fn_10087760(undefined4 xi_eax);  // 0x10087760
XI_API undefined4 __cdecl fn_10087830(void);  // 0x10087830
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_10087890(undefined4 xi_eax);  // 0x10087890
XI_API void __thiscall fn_10087D40(void * self_);  // 0x10087D40
XI_API uint32_t __thiscall fn_10087DC0(void * self_);  // 0x10087DC0
XI_API undefined4 __thiscall fn_10087E40(void * self_);  // 0x10087E40
XI_API void __thiscall fn_10087E80(void * self_);  // 0x10087E80
XI_API void __cdecl Call1008A0E0With2(void);  // 0x1008A0D0
XI_API undefined4 __cdecl fn_1008A0E0(int param_1);  // 0x1008A0E0
XI_API undefined4 __thiscall fn_1008A120(void * self_);  // 0x1008A120
XI_API void __thiscall fn_1008A160(void * self_);  // 0x1008A160
XI_API undefined4 __thiscall fn_1008A1A0(void * self_);  // 0x1008A1A0
XI_API undefined4 __thiscall fn_1008A2F0(void * self_);  // 0x1008A2F0
XI_API undefined4 __thiscall fn_1008A350(void * self_, int param_1);  // 0x1008A350
XI_API void __thiscall fn_1008A7B0(void * self_);  // 0x1008A7B0
XI_API int __cdecl fn_1008A8E0(uint32_t param_1);  // 0x1008A8E0
XI_API int __cdecl fn_1008A930(int param_1);  // 0x1008A930
XI_API void __cdecl fn_1008A9E0(int param_1, undefined4 param_2);  // 0x1008A9E0
XI_API void __thiscall fn_1008B100(void * self_);  // 0x1008B100
XI_API int __thiscall fn_1008B270(void * self_, undefined4 param_1, undefined4 param_2);  // 0x1008B270
XI_API uint32_t __cdecl fn_1008B2B0(uint8_t * param_1, uint8_t * param_2, undefined4 param_3);  // 0x1008B2B0
XI_API int __thiscall fn_1008B380(void * self_, int param_1);  // 0x1008B380
XI_API undefined4 __thiscall fn_1008B410(void * self_);  // 0x1008B410
XI_API void __thiscall fn_1008CD90(void * self_);  // 0x1008CD90
XI_API void __thiscall fn_1008D980(void * self_);  // 0x1008D980
XI_API void __thiscall fn_1008DA00(void * self_);  // 0x1008DA00
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_1008E130(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x1008E130
XI_API undefined4 __thiscall fn_1008E140(void * self_);  // 0x1008E140
XI_API undefined4 __thiscall fn_1008E150(void * self_);  // 0x1008E150
XI_API undefined4 __thiscall fn_1008E260(void * self_);  // 0x1008E260
XI_API void __thiscall fn_1008E740(void * self_);  // 0x1008E740
XI_API uint32_t __thiscall fn_1008E8D0(void * self_, int param_1);  // 0x1008E8D0
XI_API undefined4 __thiscall fn_1008E9A0(void * self_);  // 0x1008E9A0
XI_API void __thiscall fn_1008EE30(void * self_);  // 0x1008EE30
XI_API void __thiscall fn_1008F2A0(void * self_);  // 0x1008F2A0
XI_API void __thiscall fn_100926C0(void * self_);  // 0x100926C0
XI_API void __thiscall fn_10092710(void * self_);  // 0x10092710
XI_API void __thiscall fn_10092760(void * self_);  // 0x10092760
XI_API void __thiscall fn_10092780(void * self_, int param_1);  // 0x10092780
XI_API void __thiscall fn_10092800(void * self_);  // 0x10092800
XI_API void __thiscall fn_10092870(void * self_);  // 0x10092870
XI_API void __thiscall fn_10092900(void * self_);  // 0x10092900
XI_API void __thiscall fn_100929A0(void * self_);  // 0x100929A0
XI_API void __thiscall fn_100929D0(void * self_);  // 0x100929D0
XI_API void __thiscall fn_10092C20(void * self_);  // 0x10092C20
XI_API void __thiscall fn_100933C0(void * self_);  // 0x100933C0
XI_API undefined4 __thiscall fn_10093430(void * self_, undefined4 * param_1);  // 0x10093430
XI_API undefined4 __cdecl Unit10086E90_ResetGlobals(void);  // 0x10093720
XI_API void __cdecl fn_10093A30(void);  // 0x10093A30
XI_API void __cdecl fn_10093A90(void);  // 0x10093A90
XI_API void __cdecl fn_10093AD0(void);  // 0x10093AD0
XI_API void __cdecl fn_10093AF0(void);  // 0x10093AF0
XI_API void __cdecl fn_10093B10(void);  // 0x10093B10
XI_API void __cdecl fn_10093B30(void);  // 0x10093B30
XI_API void __cdecl fn_10093B50(void);  // 0x10093B50
XI_API undefined4 __cdecl fn_10093B70(void);  // 0x10093B70
XI_API void __cdecl fn_10093DA0(void);  // 0x10093DA0
XI_API undefined4 __cdecl Net_SendPacket00COnce(void);  // 0x10093DB0
XI_API undefined4 __attribute__((cdecl, regparm(1))) CallMotd(undefined4 xi_eax);  // 0x100955D0
XI_API undefined4 __thiscall fn_10095680(void * self_, int param_1);  // 0x10095680
XI_API undefined4 __thiscall fn_100956A0(void * self_, int param_1);  // 0x100956A0
XI_API undefined4 __thiscall fn_100956C0(void * self_, undefined4 xi_s1);  // 0x100956C0
XI_API undefined4 __thiscall fn_100956F0(void * self_, int param_1);  // 0x100956F0
XI_API undefined4 __thiscall fn_10095750(void * self_, int param_1);  // 0x10095750
XI_API int __thiscall fn_10095790(void * self_, int param_1);  // 0x10095790
XI_API uint32_t __thiscall fn_10095850(void * self_);  // 0x10095850
XI_API void __cdecl fn_10095900(void);  // 0x10095900
XI_API void __cdecl fn_10095DD0(void * param_1);  // 0x10095DD0
XI_API void __cdecl fn_10095E30(void * param_1);  // 0x10095E30
XI_API void __cdecl fn_10095E90(void * param_1);  // 0x10095E90
XI_API undefined4 __cdecl fn_10096270(void);  // 0x10096270
XI_API undefined4 __cdecl fn_10096640(void);  // 0x10096640
XI_API undefined1 * __thiscall Entity_BuildDisplayName(void * self_);  // 0x10097840
XI_API undefined4 __thiscall Entity_GetIndexedPtr(void * self_);  // 0x10097A40
XI_API undefined4 __cdecl fn_10097BC0(void);  // 0x10097BC0
XI_API undefined4 __cdecl fn_10097D50(void);  // 0x10097D50
XI_API undefined4 __cdecl fn_10097E50(int param_1, uint32_t param_2);  // 0x10097E50
XI_API undefined4 __cdecl fn_10097FD0(undefined4 param_1, undefined4 param_2);  // 0x10097FD0
XI_API undefined4 __thiscall fn_10098180(void * self_);  // 0x10098180
XI_API undefined4 __thiscall fn_10098210(void * self_, undefined4 param_1);  // 0x10098210
XI_API uint32_t __thiscall fn_100983A0(void * self_);  // 0x100983A0
XI_API uint32_t __attribute__((cdecl, regparm(1))) fn_100991F0(undefined4 xi_eax);  // 0x100991F0
XI_API void __cdecl fn_10099730(void);  // 0x10099730
XI_API void __cdecl fn_100997B0(void);  // 0x100997B0
XI_API undefined1 __cdecl fn_10099AE0(undefined4 param_1, undefined4 param_2, uint16_t * param_3);  // 0x10099AE0
XI_API undefined4 __thiscall fn_1009AAF0(void * self_);  // 0x1009AAF0
XI_API void __thiscall fn_1009AD20(void * self_, uint32_t param_1, uint32_t param_2, uint32_t param_3);  // 0x1009AD20
XI_API void __thiscall fn_1009ADE0(void * self_, uint32_t param_1, uint32_t param_2, uint32_t param_3, int param_4, int param_5, int param_6);  // 0x1009ADE0
XI_API long double __thiscall fn_1009AEC0(void * self_);  // 0x1009AEC0
XI_API void __thiscall fn_1009AF60(void * self_, uint32_t param_1, uint32_t param_2, int param_3);  // 0x1009AF60
XI_API void __cdecl fn_1009F3F0(void);  // 0x1009F3F0
XI_API void __thiscall fn_1009F4C0(void * self_);  // 0x1009F4C0
XI_API undefined1 * __cdecl fn_100A1770(int param_1);  // 0x100A1770
XI_API undefined4 __cdecl fn_100A19B0(undefined4 param_1);  // 0x100A19B0
XI_API undefined1 * __cdecl fn_100A1D50(int param_1);  // 0x100A1D50
XI_API void __cdecl fn_100A3CD0(void);  // 0x100A3CD0
XI_API undefined4 __thiscall CXiControlActor_GetGroundMaterial(void * self_);  // 0x100A45D0
XI_API undefined1 __attribute__((stdcall, regparm(3))) CXiControlActor_IsFreeRun(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x100A4670
XI_API undefined1 __attribute__((stdcall, regparm(3))) CXiControlActor_IsWalkLock(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x100A4690
XI_API undefined1 __attribute__((stdcall, regparm(3))) CXiControlActor_IsParallelMove(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x100A46B0
XI_API undefined4 __thiscall CXiControlActor_IsControlLock(void * self_);  // 0x100A46C0
XI_API undefined1 __attribute__((stdcall, regparm(3))) CXiCollisionActor_IsTouchDown(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x100A46D0
XI_API undefined1 __attribute__((stdcall, regparm(3))) CXiCollisionActor_IsOnLift(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x100A46F0
XI_API undefined4 __thiscall CXiCollisionActor_GetCurrentAreaId(void * self_);  // 0x100A4710
XI_API void __thiscall fn_100A5770(void * self_, undefined4 * param_1);  // 0x100A5770
XI_API undefined4 __thiscall fn_100A57C0(void * self_, undefined4 * param_1);  // 0x100A57C0
XI_API undefined4 __cdecl fn_100A5E10(int * param_1);  // 0x100A5E10
XI_API void __thiscall fn_100A5EC0(void * self_, undefined4 param_1);  // 0x100A5EC0
XI_API undefined4 __thiscall fn_100A5EF0(void * self_);  // 0x100A5EF0
XI_API undefined4 __attribute__((stdcall, regparm(1))) fn_100A6070(undefined4 xi_eax, undefined4 param_1);  // 0x100A6070
XI_API void __thiscall fn_100A78D0(void * self_, float * param_1, float * param_2, undefined4 xi_s3);  // 0x100A78D0
XI_API void * __thiscall fn_100A89E0(void * self_);  // 0x100A89E0
XI_API long double __stdcall fn_100AAEF0(int * param_1);  // 0x100AAEF0
XI_API void __cdecl fn_100AB320(int param_1);  // 0x100AB320
XI_API void __cdecl fn_100AB550(int param_1);  // 0x100AB550
XI_API undefined4 __thiscall fn_100AB700(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, uint32_t param_4);  // 0x100AB700
XI_API void __thiscall fn_100AB750(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, uint32_t param_4);  // 0x100AB750
XI_API void __thiscall fn_100AB830(void * self_, undefined4 * param_1);  // 0x100AB830
XI_API void __attribute__((stdcall, regparm(3))) fn_100AB880(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, undefined4 * param_2);  // 0x100AB880
XI_API void __thiscall fn_100AB9A0(void * self_, void * param_1, undefined4 param_2, undefined4 param_3, undefined4 * param_4);  // 0x100AB9A0
XI_API void * __thiscall fn_100ABC20(void * self_, char * param_1, int param_2);  // 0x100ABC20
XI_API undefined4 __cdecl fn_100ABC70(undefined4 * param_1, undefined4 * param_2);  // 0x100ABC70
XI_API void __cdecl fn_100AC380(void);  // 0x100AC380
XI_API char __thiscall fn_100AC4E0(void * self_);  // 0x100AC4E0
XI_API void __cdecl fn_100AD6D0(undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7, undefined4 param_8, undefined4 param_9);  // 0x100AD6D0
XI_API int __cdecl fn_100AE350(void);  // 0x100AE350
XI_API void __cdecl fn_100AE3A0(void);  // 0x100AE3A0
XI_API void __cdecl fn_100BA080(void);  // 0x100BA080
XI_API void __thiscall fn_100BD5F0(void * self_);  // 0x100BD5F0
XI_API undefined * __cdecl fn_100BD760(void);  // 0x100BD760
XI_API void * __thiscall fn_100C2E20(void * self_, undefined4 * param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, char * param_5, undefined4 param_6, undefined4 param_7);  // 0x100C2E20
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_100C3020(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x100C3020
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_100C3030(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x100C3030
XI_API void __thiscall fn_100C3050(void * self_);  // 0x100C3050
XI_API void * __thiscall fn_100C3060(void * self_, undefined4 * param_1, undefined4 param_2, undefined2 * param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7, undefined4 param_8);  // 0x100C3060
XI_API undefined * __cdecl fn_100C3190(void);  // 0x100C3190
XI_API void __thiscall fn_100C3200(void * self_);  // 0x100C3200
XI_API undefined4 __thiscall fn_100C3210(void * self_);  // 0x100C3210
XI_API undefined4 __thiscall fn_100C3220(void * self_);  // 0x100C3220
XI_API void __thiscall fn_100C3260(void * self_, int param_1);  // 0x100C3260
XI_API void __thiscall fn_100C3300(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6);  // 0x100C3300
XI_API undefined4 __thiscall fn_100C3480(void * self_);  // 0x100C3480
XI_API void __cdecl fn_100C3E70(void * param_1);  // 0x100C3E70
XI_API int __thiscall fn_100C3FB0(void * self_);  // 0x100C3FB0
XI_API int __thiscall fn_100C3FC0(void * self_);  // 0x100C3FC0
XI_API undefined4 __thiscall fn_100C4B00(void * self_, undefined4 param_1, undefined4 * param_2);  // 0x100C4B00
XI_API undefined4 __thiscall fn_100C4B50(void * self_, undefined4 param_1, void * param_2);  // 0x100C4B50
XI_API void __thiscall fn_100C4C20(void * self_);  // 0x100C4C20
XI_API int __cdecl fn_100C51C0(undefined4 param_1, int param_2);  // 0x100C51C0
XI_API int __cdecl fn_100C5260(int param_1);  // 0x100C5260
XI_API int __cdecl fn_100C5270(int param_1);  // 0x100C5270
XI_API int __cdecl fn_100C5280(int param_1);  // 0x100C5280
XI_API int __cdecl fn_100C5290(int param_1);  // 0x100C5290
XI_API uint32_t __thiscall fn_100C8280(void * self_);  // 0x100C8280
XI_API int __thiscall Actor_SelectNearestLights(void *self_);  // 0x100CB4E0
XI_API void __thiscall fn_100CD4E0(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, int param_7);  // 0x100CD4E0
XI_API void __thiscall fn_100CDF90(void * self_);  // 0x100CDF90
XI_API void __thiscall fn_100CE160(void * self_);  // 0x100CE160
XI_API undefined8 __thiscall fn_100CF100(void * self_, undefined4 xi_s1);  // 0x100CF100
XI_API void __thiscall fn_100CFA50(void * self_, int * param_1, void * param_2);  // 0x100CFA50
XI_API undefined * __thiscall fn_100CFAD0(void * self_, int param_1);  // 0x100CFAD0
XI_API void __attribute__((stdcall, regparm(3))) CXiSkeletonActor_DrawName2IfVisible(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, undefined4 * param_2, char * param_3, float param_4);  // 0x100D08B0
XI_API undefined4 __thiscall fn_100D0D40(void * self_, int * param_1, int param_2);  // 0x100D0D40
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_100D0D70(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, int param_2, int * param_3);  // 0x100D0D70
XI_API void __thiscall fn_100D0FC0(void * self_, undefined4 xi_s1, undefined4 xi_s2);  // 0x100D0FC0
XI_API void __thiscall fn_100D1450(void * self_, int * param_1, int param_2);  // 0x100D1450
XI_API void __thiscall fn_100D14C0(void * self_);  // 0x100D14C0
XI_API int __cdecl fn_100D43E0(void);  // 0x100D43E0
XI_API int __thiscall fn_100D4410(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x100D4410
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_100D44B0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, int param_2);  // 0x100D44B0
XI_API int __thiscall fn_100D4540(void * self_, int param_1);  // 0x100D4540
XI_API void __thiscall fn_100D5430(void * self_, undefined4 param_1);  // 0x100D5430
XI_API uint32_t __thiscall fn_100D6240(void * self_, int * param_1);  // 0x100D6240
XI_API void __cdecl fn_100D7420(undefined4 * param_1);  // 0x100D7420
XI_API void __cdecl fn_100D7450(int param_1, undefined4 * param_2, uint32_t param_3);  // 0x100D7450
XI_API void __cdecl fn_100D7510(int param_1);  // 0x100D7510
XI_API void __cdecl fn_100D7560(undefined4 * param_1, undefined4 * param_2);  // 0x100D7560
XI_API void __cdecl fn_100D75A0(int * param_1, int * param_2);  // 0x100D75A0
XI_API int __cdecl fn_100D7E90(int param_1);  // 0x100D7E90
XI_API void __cdecl fn_100D7EA0(undefined4 param_1, char * param_2, undefined4 param_3);  // 0x100D7EA0
XI_API void __cdecl fn_100D7ED0(undefined4 param_1, undefined4 * param_2, undefined4 * param_3, uint32_t param_4);  // 0x100D7ED0
XI_API undefined4 __cdecl fn_100D8A20(int param_1, undefined4 param_2, undefined * param_3);  // 0x100D8A20
XI_API undefined4 __cdecl fn_100D9380(void);  // 0x100D9380
XI_API undefined4 __cdecl fn_100D9410(void);  // 0x100D9410
XI_API undefined4 __cdecl fn_100D9430(void);  // 0x100D9430
XI_API undefined4 __cdecl fn_100D9460(void);  // 0x100D9460
XI_API void __cdecl fn_100D9830(void);  // 0x100D9830
XI_API undefined4 __thiscall fn_100D9950(void * self_);  // 0x100D9950
XI_API undefined4 __thiscall fn_100D9A70(void * self_);  // 0x100D9A70
XI_API undefined1 __cdecl fn_100D9AD0(void);  // 0x100D9AD0
XI_API undefined4 __cdecl fn_100D9B00(void);  // 0x100D9B00
XI_API uint32_t __thiscall fn_100DA6A0(void * self_);  // 0x100DA6A0
XI_API undefined4 __thiscall GameMode_GetField98(void * self_);  // 0x100DB100
XI_API int __thiscall GameMode_PollPendingState(void * self_);  // 0x100DB140
XI_API undefined4 __stdcall fn_100DB780(undefined4 param_1);  // 0x100DB780
XI_API undefined4 __stdcall fn_100DB790(undefined4 param_1);  // 0x100DB790
XI_API undefined4 __thiscall fn_100DB7B0(void * self_, undefined4 param_1);  // 0x100DB7B0
XI_API undefined4 __cdecl fn_100DB930(void);  // 0x100DB930
XI_API undefined4 __thiscall fn_100DBAA0(void * self_);  // 0x100DBAA0
XI_API undefined4 __cdecl fn_100DBCA0(void);  // 0x100DBCA0
XI_API undefined1 __cdecl fn_100DBDE0(void);  // 0x100DBDE0
XI_API DWORD __stdcall fn_100DBE00(void);  // 0x100DBE00
XI_API undefined4 __cdecl fn_100DBE30(undefined4 * param_1, undefined4 * param_2);  // 0x100DBE30
XI_API undefined4 __cdecl fn_100DBE50(void);  // 0x100DBE50
XI_API undefined4 __cdecl fn_100DBEB0(void);  // 0x100DBEB0
XI_API int __cdecl fn_100DBF00(int param_1, uint32_t param_2, uint32_t param_3);  // 0x100DBF00
XI_API void __cdecl fn_100DBF20(void);  // 0x100DBF20
XI_API undefined4 __cdecl fn_100DBFE0(uint32_t param_1, uint32_t param_2);  // 0x100DBFE0
XI_API int __cdecl fn_100DC060(void);  // 0x100DC060
XI_API undefined4 __cdecl fn_100DC080(void);  // 0x100DC080
XI_API undefined4 __cdecl fn_100DC090(void);  // 0x100DC090
XI_API undefined4 __cdecl fn_100DC120(void);  // 0x100DC120
XI_API undefined4 __cdecl fn_100DC130(void);  // 0x100DC130
XI_API undefined4 __cdecl fn_100DC140(void);  // 0x100DC140
XI_API undefined4 __cdecl fn_100DC150(DWORD param_1, DWORD param_2);  // 0x100DC150
XI_API undefined4 __cdecl fn_100DC1B0(int param_1, int param_2);  // 0x100DC1B0
XI_API undefined4 __cdecl fn_100DC1F0(void);  // 0x100DC1F0
XI_API undefined4 __cdecl fn_100DC210(void);  // 0x100DC210
XI_API undefined4 __cdecl fn_100DC250(void);  // 0x100DC250
XI_API undefined4 __cdecl fn_100DC260(void);  // 0x100DC260
XI_API undefined4 __cdecl fn_100DC270(void);  // 0x100DC270
XI_API undefined4 __cdecl fn_100DC2C0(void);  // 0x100DC2C0
XI_API undefined4 __cdecl fn_100DC420(void);  // 0x100DC420
XI_API undefined4 __cdecl fn_100DC6C0(void);  // 0x100DC6C0
XI_API undefined4 * __cdecl fn_100DC6D0(void);  // 0x100DC6D0
XI_API void __cdecl fn_100DCA00(uint32_t * param_1, int param_2, uint32_t param_3);  // 0x100DCA00
XI_API void __cdecl fn_100DCF60(uint32_t * param_1, int param_2, uint32_t param_3);  // 0x100DCF60
XI_API undefined4 __cdecl fn_100DD570(int param_1);  // 0x100DD570
XI_API int __cdecl fn_100DD590(undefined4 * param_1, uint32_t param_2, uint32_t * param_3, undefined4 * param_4, int param_5, undefined4 param_6);  // 0x100DD590
XI_API int __cdecl fn_100DD6C0(undefined4 * param_1, uint32_t param_2, uint32_t * param_3, undefined4 param_4, undefined4 * param_5, uint32_t param_6);  // 0x100DD6C0
XI_API undefined4 __cdecl fn_100DD800(undefined4 * param_1, int param_2, undefined1 * param_3, uint32_t param_4, undefined1 * param_5, uint32_t param_6);  // 0x100DD800
XI_API int __cdecl fn_100DD920(void);  // 0x100DD920
XI_API undefined4 __cdecl fn_100DDD20(uint16_t * param_1, int param_2, int param_3);  // 0x100DDD20
XI_API void __cdecl fn_100DDE90(undefined4 * param_1);  // 0x100DDE90
XI_API void __cdecl fn_100DE010(int * param_1, int * param_2);  // 0x100DE010
XI_API uint32_t __cdecl fn_100DF5D0(char * param_1, int param_2, char * param_3, uint32_t param_4, undefined4 * param_5);  // 0x100DF5D0
XI_API undefined4 __cdecl fn_100DF6E0(int param_1, uint32_t param_2, int param_3, int param_4, uint32_t param_5, uint32_t param_6, int param_7);  // 0x100DF6E0
XI_API int __cdecl fn_100DF780(int param_1, uint32_t param_2, int param_3);  // 0x100DF780
XI_API int __cdecl fn_100DF7C0(undefined4 * param_1, uint32_t param_2, undefined4 * param_3, uint32_t param_4, int param_5);  // 0x100DF7C0
XI_API undefined1 __cdecl fn_100E00F0(void);  // 0x100E00F0
XI_API undefined1 __cdecl fn_100E0100(void);  // 0x100E0100
XI_API int __cdecl fn_100E08D0(undefined4 param_1);  // 0x100E08D0
XI_API void __cdecl fn_100E09A0(int param_1, char * param_2);  // 0x100E09A0
XI_API undefined4 __cdecl fn_100E0A00(void);  // 0x100E0A00
XI_API undefined4 __cdecl fn_100E0A50(undefined4 param_1);  // 0x100E0A50
XI_API undefined4 __cdecl fn_100E0AA0(void);  // 0x100E0AA0
XI_API void __cdecl fn_100E0B10(void);  // 0x100E0B10
XI_API undefined4 __thiscall fn_100E13C0(void * self_);  // 0x100E13C0
XI_API undefined4 __cdecl fn_100E1470(undefined4 * param_1);  // 0x100E1470
XI_API undefined4 __cdecl fn_100E1790(int * param_1, int param_2, undefined4 param_3);  // 0x100E1790
XI_API int __cdecl fn_100E19C0(int param_1);  // 0x100E19C0
XI_API undefined4 __cdecl fn_100E1AD0(void);  // 0x100E1AD0
XI_API int __cdecl fn_100E1B50(void);  // 0x100E1B50
XI_API undefined4 __cdecl fn_100E1C00(int param_1, undefined2 * param_2);  // 0x100E1C00
XI_API undefined4 * __cdecl fn_100E1C30(int param_1, uint32_t * param_2, undefined4 * param_3, undefined4 * param_4, undefined4 * param_5, undefined4 * param_6, undefined4 * param_7, undefined4 param_8, undefined4 param_9, undefined4 param_10, undefined4 param_11, undefined4 param_12);  // 0x100E1C30
XI_API undefined4 __cdecl fn_100E2E70(undefined4 * param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6);  // 0x100E2E70
XI_API undefined4 __cdecl fn_100E2EE0(undefined4 * param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5);  // 0x100E2EE0
XI_API undefined1 __cdecl fn_100E33E0(void);  // 0x100E33E0
XI_API undefined1 __cdecl fn_100E33F0(void);  // 0x100E33F0
XI_API undefined4 __cdecl fn_100E3E30(void);  // 0x100E3E30
XI_API undefined4 __cdecl fn_100E40A0(void);  // 0x100E40A0
XI_API undefined1 __cdecl fn_100E4110(void);  // 0x100E4110
XI_API undefined1 __cdecl fn_100E4320(void);  // 0x100E4320
XI_API undefined1 __cdecl fn_100E4330(void);  // 0x100E4330
XI_API undefined4 __cdecl fn_100E43F0(int param_1);  // 0x100E43F0
XI_API undefined4 __cdecl fn_100E47A0(int * param_1);  // 0x100E47A0
XI_API void __cdecl fn_100E5E30(undefined4 param_1);  // 0x100E5E30
XI_API undefined4 __cdecl fn_100E5FC0(void);  // 0x100E5FC0
XI_API undefined1 __cdecl fn_100E5FF0(void);  // 0x100E5FF0
XI_API int __cdecl fn_100E6020(void);  // 0x100E6020
XI_API undefined4 __cdecl fn_100E61A0(void);  // 0x100E61A0
XI_API undefined1 __cdecl fn_100E61C0(void);  // 0x100E61C0
XI_API void __cdecl fn_100E6220(undefined4 param_1);  // 0x100E6220
XI_API void __cdecl fn_100E62C0(undefined4 param_1);  // 0x100E62C0
XI_API void __cdecl fn_100E6300(void);  // 0x100E6300
XI_API undefined4 __cdecl fn_100E6370(undefined4 param_1);  // 0x100E6370
XI_API undefined4 __cdecl fn_100E63C0(int param_1);  // 0x100E63C0
XI_API undefined4 __cdecl fn_100E63E0(undefined4 param_1);  // 0x100E63E0
XI_API undefined4 __cdecl fn_100E6650(undefined4 param_1);  // 0x100E6650
XI_API undefined4 __cdecl fn_100E6790(void);  // 0x100E6790
XI_API undefined4 __cdecl fn_100E67E0(void);  // 0x100E67E0
XI_API undefined4 __cdecl fn_100E6810(void);  // 0x100E6810
XI_API undefined4 __cdecl fn_100E6830(void);  // 0x100E6830
XI_API undefined4 __cdecl fn_100E6860(void);  // 0x100E6860
XI_API void __cdecl fn_100E6880(undefined4 param_1);  // 0x100E6880
XI_API undefined4 __cdecl fn_100E6B10(void);  // 0x100E6B10
XI_API undefined4 __cdecl fn_100E6B20(void);  // 0x100E6B20
XI_API int __cdecl fn_100E6C40(undefined4 param_1);  // 0x100E6C40
XI_API void __cdecl fn_100E7260(void);  // 0x100E7260
XI_API int __cdecl fn_100E7B20(int param_1, undefined4 param_2);  // 0x100E7B20
XI_API undefined1 __cdecl fn_100E84A0(void);  // 0x100E84A0
XI_API undefined1 __cdecl fn_100E84B0(void);  // 0x100E84B0
XI_API undefined1 __cdecl fn_100E8B60(void);  // 0x100E8B60
XI_API undefined1 __cdecl fn_100E8B70(void);  // 0x100E8B70
XI_API int __cdecl fn_100E8B80(undefined4 param_1);  // 0x100E8B80
XI_API int __cdecl fn_100E8C70(void);  // 0x100E8C70
XI_API undefined4 __cdecl fn_100E9550(undefined4 param_1);  // 0x100E9550
XI_API undefined1 __cdecl fn_100EB090(void);  // 0x100EB090
XI_API undefined1 __cdecl fn_100EB0A0(void);  // 0x100EB0A0
XI_API short * __cdecl fn_100EB490(int param_1, undefined4 param_2);  // 0x100EB490
XI_API int __cdecl fn_100EB8D0(int param_1);  // 0x100EB8D0
XI_API undefined4 __cdecl fn_100EB940(int param_1);  // 0x100EB940
XI_API undefined4 __cdecl fn_100EBF40(undefined4 param_1);  // 0x100EBF40
XI_API int __cdecl fn_100EBF80(uint32_t param_1, undefined4 param_2, undefined4 param_3);  // 0x100EBF80
XI_API void __cdecl fn_100EC060(uint32_t param_1, undefined4 param_2);  // 0x100EC060
XI_API undefined4 __cdecl fn_100EC3F0(undefined4 param_1, undefined4 param_2, undefined4 * param_3);  // 0x100EC3F0
XI_API undefined4 __cdecl fn_100ED720(void);  // 0x100ED720
XI_API void __cdecl fn_100EDEA0(void);  // 0x100EDEA0
XI_API undefined4 __cdecl fn_100EDEB0(void);  // 0x100EDEB0
XI_API undefined4 __cdecl fn_100EDED0(void);  // 0x100EDED0
XI_API int * __cdecl fn_100EF0E0(int * param_1);  // 0x100EF0E0
XI_API undefined4 __cdecl fn_100EF4C0(int * param_1);  // 0x100EF4C0
XI_API undefined4 __cdecl fn_100EF550(int * param_1);  // 0x100EF550
XI_API undefined4 __cdecl fn_100EF5B0(void);  // 0x100EF5B0
XI_API undefined4 __cdecl fn_100EF5BD(void);  // 0x100EF5BD
XI_API undefined4 __cdecl fn_100EF6F0(int * param_1);  // 0x100EF6F0
XI_API undefined4 __cdecl fn_100EF730(int * param_1);  // 0x100EF730
XI_API undefined4 __cdecl fn_100EF780(void);  // 0x100EF780
XI_API undefined4 __cdecl fn_100EF870(void);  // 0x100EF870
XI_API undefined4 __cdecl fn_100EF8B0(void);  // 0x100EF8B0
XI_API undefined4 __cdecl fn_100EF8F0(void);  // 0x100EF8F0
XI_API undefined4 __cdecl fn_100EFC70(int param_1);  // 0x100EFC70
XI_API undefined4 __cdecl fn_100EFCA0(int param_1);  // 0x100EFCA0
XI_API char * __cdecl fn_100EFCE0(int param_1);  // 0x100EFCE0
XI_API int __cdecl fn_100EFD20(void);  // 0x100EFD20
XI_API undefined4 __cdecl fn_100EFDA0(int param_1);  // 0x100EFDA0
XI_API int __cdecl fn_100EFDF0(int param_1, uint32_t param_2, uint32_t param_3);  // 0x100EFDF0
XI_API undefined4 __cdecl fn_100F0000(char * param_1);  // 0x100F0000
XI_API undefined4 __cdecl fn_100F0070(void);  // 0x100F0070
XI_API undefined1 __cdecl fn_100F0090(void);  // 0x100F0090
XI_API int __cdecl fn_100F00A0(void);  // 0x100F00A0
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_100F02A0(undefined4 xi_eax);  // 0x100F02A0
XI_API undefined4 __cdecl fn_100F04F0(undefined1 * param_1);  // 0x100F04F0
XI_API undefined4 __cdecl fn_100F0550(undefined1 * param_1);  // 0x100F0550
XI_API undefined4 __cdecl fn_100F0570(undefined1 * param_1);  // 0x100F0570
XI_API undefined4 __cdecl fn_100F0590(undefined1 * param_1);  // 0x100F0590
XI_API undefined4 __cdecl fn_100F05C0(void);  // 0x100F05C0
XI_API undefined4 __cdecl fn_100F05F0(void);  // 0x100F05F0
XI_API undefined1 __cdecl fn_100F06B0(void);  // 0x100F06B0
XI_API undefined1 __cdecl fn_100F09C0(void);  // 0x100F09C0
XI_API uint32_t __cdecl fn_100F0C50(void);  // 0x100F0C50
XI_API void __cdecl fn_100F10A0(void);  // 0x100F10A0
XI_API undefined4 __cdecl fn_100F10C0(undefined1 * param_1);  // 0x100F10C0
XI_API int __cdecl fn_100F10F0(void);  // 0x100F10F0
XI_API undefined4 __cdecl fn_100F1280(undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x100F1280
XI_API char * __cdecl fn_100F1300(undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x100F1300
XI_API undefined4 __cdecl fn_100F1340(char * param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x100F1340
XI_API uint32_t __cdecl fn_100F13B0(undefined4 param_1);  // 0x100F13B0
XI_API void __cdecl fn_100F13D0(undefined4 param_1);  // 0x100F13D0
XI_API void __cdecl fn_100F13F0(undefined4 param_1);  // 0x100F13F0
XI_API uint32_t __cdecl fn_100F1670(void);  // 0x100F1670
XI_API void __cdecl fn_100F1870(void);  // 0x100F1870
XI_API void __cdecl fn_100F1900(void);  // 0x100F1900
XI_API undefined4 __cdecl fn_100F1950(void);  // 0x100F1950
XI_API int __thiscall fn_100F1B20(void * self_);  // 0x100F1B20
XI_API undefined4 __thiscall fn_100F1BA0(void * self_);  // 0x100F1BA0
XI_API uint32_t __thiscall fn_100F1C40(void * self_, undefined4 param_1);  // 0x100F1C40
XI_API undefined4 __thiscall fn_100F1D20(void * self_);  // 0x100F1D20
XI_API undefined1 __thiscall fn_100F1E70(void * self_);  // 0x100F1E70
XI_API undefined4 __thiscall fn_100F1F00(void * self_, undefined4 param_1);  // 0x100F1F00
XI_API undefined4 __thiscall fn_100F25F0(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x100F25F0
XI_API undefined4 __thiscall fn_100F2690(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x100F2690
XI_API undefined4 __thiscall fn_100F2720(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x100F2720
XI_API undefined4 __thiscall fn_100F29E0(void * self_);  // 0x100F29E0
XI_API void __thiscall fn_100F2A40(void * self_);  // 0x100F2A40
XI_API undefined4 __thiscall fn_100F2B80(void * self_);  // 0x100F2B80
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_100F2C40(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x100F2C40
XI_API undefined4 __thiscall fn_100F2C80(void * self_, undefined4 param_1);  // 0x100F2C80
XI_API void __cdecl fn_100F3010(undefined4 * param_1, undefined4 * param_2, uint32_t param_3);  // 0x100F3010
XI_API int __thiscall fn_100F32B0(void * self_, undefined4 * param_1);  // 0x100F32B0
XI_API undefined4 __thiscall fn_100F3BB0(void * self_, undefined4 param_1, undefined4 param_2, undefined4 * param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6);  // 0x100F3BB0
XI_API undefined4 __thiscall fn_100F3C20(void * self_, undefined4 param_1, undefined4 param_2, undefined4 * param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6);  // 0x100F3C20
XI_API undefined4 __thiscall fn_100F3C90(void * self_, undefined4 param_1, undefined4 param_2, undefined4 * param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6);  // 0x100F3C90
XI_API undefined4 __thiscall fn_100F3D00(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5);  // 0x100F3D00
XI_API undefined4 __cdecl fn_100F76B0(void);  // 0x100F76B0
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_100F76D0(undefined4 xi_eax, undefined4 param_1);  // 0x100F76D0
XI_API undefined4 __cdecl fn_100F7700(void);  // 0x100F7700
XI_API undefined1 __cdecl fn_100F7740(void);  // 0x100F7740
XI_API void __cdecl fn_100F7A00(void);  // 0x100F7A00
XI_API undefined4 __cdecl fn_100F7A80(void);  // 0x100F7A80
XI_API undefined1 __cdecl fn_100F8910(void);  // 0x100F8910
XI_API undefined1 __cdecl fn_100F8920(void);  // 0x100F8920
XI_API undefined1 __cdecl fn_100F8CA0(void);  // 0x100F8CA0
XI_API undefined1 __cdecl fn_100F8CB0(void);  // 0x100F8CB0
XI_API undefined4 * __cdecl fn_100F91C0(undefined4 * param_1);  // 0x100F91C0
XI_API undefined1 __cdecl fn_100F9340(void);  // 0x100F9340
XI_API undefined1 __cdecl fn_100F9350(void);  // 0x100F9350
XI_API int __thiscall fn_100F94D0(void * self_);  // 0x100F94D0
XI_API undefined4 __cdecl fn_100F9CC0(void);  // 0x100F9CC0
XI_API void __cdecl fn_100F9D50(void);  // 0x100F9D50
XI_API undefined4 __cdecl fn_100F9D70(void);  // 0x100F9D70
XI_API undefined4 __cdecl fn_100F9DA0(void);  // 0x100F9DA0
XI_API undefined4 __cdecl fn_100F9DB0(void);  // 0x100F9DB0
XI_API undefined4 __cdecl fn_100F9DF0(int param_1);  // 0x100F9DF0
XI_API undefined4 __cdecl fn_100F9E10(int * param_1);  // 0x100F9E10
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_100FA940(undefined4 xi_eax, undefined4 param_1);  // 0x100FA940
XI_API void __attribute__((cdecl, regparm(1))) fn_100FA950(undefined4 xi_eax, undefined4 param_1);  // 0x100FA950
XI_API undefined4 __cdecl fn_100FA9C0(int param_1, uint32_t param_2);  // 0x100FA9C0
XI_API undefined4 __cdecl fn_100FB920(undefined4 param_1, char * param_2, int param_3, undefined4 param_4, char * param_5, char * param_6, undefined4 param_7);  // 0x100FB920
XI_API undefined4 __cdecl fn_100FBA40(undefined4 param_1, char * param_2, int param_3, undefined4 param_4, undefined4 param_5, char * param_6);  // 0x100FBA40
XI_API undefined4 __cdecl fn_100FBB40(int param_1);  // 0x100FBB40
XI_API undefined4 __cdecl fn_100FBD10(int param_1, int param_2, int param_3);  // 0x100FBD10
XI_API undefined4 __cdecl fn_100FC160(void);  // 0x100FC160
XI_API undefined4 __cdecl fn_100FC180(uint32_t param_1);  // 0x100FC180
XI_API undefined4 __cdecl fn_100FC320(undefined4 param_1);  // 0x100FC320
XI_API uint32_t __attribute__((cdecl, regparm(1))) fn_100FC370(undefined4 xi_eax, int param_1, undefined4 param_2);  // 0x100FC370
XI_API undefined4 __cdecl fn_100FC4D0(uint16_t * param_1, int param_2, int param_3);  // 0x100FC4D0
XI_API undefined * __cdecl fn_100FC4E0(void);  // 0x100FC4E0
XI_API undefined * __cdecl fn_100FC510(void);  // 0x100FC510
XI_API void __cdecl fn_100FC650(void);  // 0x100FC650
XI_API void __cdecl fn_100FC670(void);  // 0x100FC670
XI_API undefined4 __cdecl fn_100FC680(undefined4 param_1, undefined4 param_2);  // 0x100FC680
XI_API undefined4 __cdecl fn_100FC9C0(undefined4 param_1, undefined4 param_2);  // 0x100FC9C0
XI_API void __cdecl fn_100FCA20(uint32_t * param_1, int param_2, uint32_t param_3);  // 0x100FCA20
XI_API void __cdecl fn_100FCA40(int param_1, int param_2, uint32_t param_3);  // 0x100FCA40
XI_API void __cdecl fn_100FCA60(int param_1, uint32_t param_2, int param_3);  // 0x100FCA60
XI_API void __cdecl fn_100FCAC0(int param_1, int param_2);  // 0x100FCAC0
XI_API void __cdecl fn_100FCAD0(int param_1, int param_2);  // 0x100FCAD0
XI_API void __cdecl fn_100FCAE0(int param_1);  // 0x100FCAE0
XI_API undefined4 __cdecl fn_100FCD70(void);  // 0x100FCD70
XI_API undefined4 __cdecl fn_100FD3A0(uint32_t param_1);  // 0x100FD3A0
XI_API undefined4 __cdecl fn_10101BC0(uint32_t param_1);  // 0x10101BC0
XI_API uint32_t __cdecl fn_10101BF0(uint32_t param_1);  // 0x10101BF0
XI_API undefined4 __cdecl fn_101033C0(int param_1);  // 0x101033C0
XI_API undefined4 __cdecl fn_10103700(undefined4 * param_1);  // 0x10103700
XI_API undefined4 __cdecl fn_10103740(void);  // 0x10103740
XI_API void __cdecl fn_101037F0(undefined4 param_1, undefined4 param_2);  // 0x101037F0
XI_API int __cdecl fn_10103870(char * param_1, char * param_2);  // 0x10103870
XI_API undefined4 __cdecl fn_101038C0(void);  // 0x101038C0
XI_API undefined4 __cdecl fn_101038D0(void);  // 0x101038D0
XI_API undefined4 __cdecl fn_101038E0(void);  // 0x101038E0
XI_API undefined4 __cdecl fn_10103920(void);  // 0x10103920
XI_API undefined4 __cdecl fn_101039C0(int param_1);  // 0x101039C0
XI_API int __cdecl ntGameTimeGet(void);  // 0x101039E0
XI_API undefined4 __cdecl fn_10103A90(uint32_t param_1);  // 0x10103A90
XI_API uint32_t __cdecl fn_10103AB0(uint32_t param_1);  // 0x10103AB0
XI_API uint32_t __cdecl fn_10103AE0(uint32_t param_1);  // 0x10103AE0
XI_API undefined4 __cdecl fn_10103B10(uint32_t param_1);  // 0x10103B10
XI_API uint32_t __cdecl fn_10103B30(uint32_t param_1);  // 0x10103B30
XI_API void __cdecl fn_10103F50(undefined2 * param_1);  // 0x10103F50
XI_API undefined4 __cdecl fn_10107610(uint16_t * param_1, undefined4 param_2);  // 0x10107610
XI_API void __cdecl fn_10108150(void);  // 0x10108150
XI_API void __cdecl fn_10108300(undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x10108300
XI_API int __cdecl fn_101083C0(uint32_t param_1);  // 0x101083C0
XI_API undefined4 __cdecl fn_10108590(uint32_t param_1, uint32_t param_2, uint32_t param_3);  // 0x10108590
XI_API undefined4 __cdecl fn_101085C0(uint32_t param_1, uint32_t param_2);  // 0x101085C0
XI_API undefined4 __cdecl fn_101085E0(uint32_t param_1);  // 0x101085E0
XI_API void __thiscall fn_101086C0(void * self_, undefined4 param_1, undefined4 param_2);  // 0x101086C0
XI_API void __thiscall fn_10108700(void * self_, int param_1);  // 0x10108700
XI_API void __thiscall fn_10108780(void * self_, int param_1);  // 0x10108780
XI_API uint32_t __thiscall fn_10108870(void * self_, int param_1);  // 0x10108870
XI_API int __stdcall fn_101088A0(int param_1);  // 0x101088A0
XI_API void __thiscall fn_101088C0(void * self_);  // 0x101088C0
XI_API undefined4 * __thiscall fn_10108990(void * self_, int param_1);  // 0x10108990
XI_API void __thiscall fn_10108A50(void * self_, int * param_1);  // 0x10108A50
XI_API void __thiscall fn_10108D80(void * self_, int param_1);  // 0x10108D80
XI_API void __thiscall fn_10108E80(void * self_, int param_1, int param_2);  // 0x10108E80
XI_API void __thiscall fn_10108F40(void * self_, int param_1, int param_2);  // 0x10108F40
XI_API undefined4 * __thiscall fn_10109000(void * self_, undefined4 param_1, undefined4 * param_2, uint32_t param_3);  // 0x10109000
XI_API void __thiscall fn_101090A0(void * self_);  // 0x101090A0
XI_API void __thiscall fn_10109100(void * self_);  // 0x10109100
XI_API void * __thiscall fn_10109140(void * self_, undefined4 xi_s1);  // 0x10109140
XI_API int __thiscall fn_101091D0(void * self_, undefined4 param_1);  // 0x101091D0
XI_API void * __attribute__((stdcall, regparm(3))) fn_101092C0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, char * param_2);  // 0x101092C0
XI_API void __attribute__((stdcall, regparm(3))) fn_101094D0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, undefined4 param_2);  // 0x101094D0
XI_API void __thiscall fn_1010E040(void * self_, void * param_1);  // 0x1010E040
XI_API void __cdecl fn_1010F8B0(void);  // 0x1010F8B0
XI_API void __thiscall fn_1010FA30(void * self_, uint32_t * param_1, uint32_t * param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5);  // 0x1010FA30
XI_API void __thiscall fn_1010FBD0(void * self_, undefined4 param_1, undefined4 param_2);  // 0x1010FBD0
XI_API void __cdecl fn_1010FC50(undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x1010FC50
XI_API void __cdecl fn_1010FEB0(undefined4 param_1, undefined4 param_2);  // 0x1010FEB0
XI_API void __thiscall fn_1010FEE0(void * self_, void * param_1);  // 0x1010FEE0
XI_API void __thiscall fn_10110040(void * self_, int param_1);  // 0x10110040
XI_API void __cdecl fn_10113220(void);  // 0x10113220
XI_API void __cdecl fn_101133A0(void);  // 0x101133A0
XI_API void __thiscall fn_10114070(void * self_);  // 0x10114070
XI_API undefined4 * __thiscall fn_10114120(void * self_, undefined4 * param_1);  // 0x10114120
XI_API void __thiscall fn_10114250(void * self_, undefined4 * param_1);  // 0x10114250
XI_API void __thiscall fn_10114320(void * self_);  // 0x10114320
XI_API void __thiscall fn_10114500(void * self_);  // 0x10114500
XI_API int __stdcall fn_10114620(uint16_t * param_1, undefined4 param_2);  // 0x10114620
XI_API undefined4 __cdecl FDtXiInit(void);  // 0x10114B60
XI_API void __cdecl fn_10114F10(uint32_t param_1, undefined4 * param_2, uint32_t param_3);  // 0x10114F10
XI_API void __cdecl FDtGmNoticeProc(void);  // 0x10115020
XI_API undefined4 __cdecl fn_101152D0(void);  // 0x101152D0
XI_API undefined4 __cdecl fn_101152E0(void);  // 0x101152E0
XI_API undefined4 * __cdecl fn_101152F0(char * param_1, undefined4 param_2, undefined4 * param_3, undefined4 * param_4, undefined4 * param_5, undefined4 * param_6);  // 0x101152F0
XI_API undefined4 __cdecl fn_101154D0(undefined4 * param_1);  // 0x101154D0
XI_API undefined4 __cdecl fn_10115500(undefined4 * param_1, undefined4 param_2);  // 0x10115500
XI_API undefined4 __cdecl fn_10115520(int param_1);  // 0x10115520
XI_API undefined4 __cdecl fn_10115570(int param_1);  // 0x10115570
XI_API undefined4 __cdecl fn_10115580(int param_1);  // 0x10115580
XI_API undefined4 __cdecl fn_10115600(int param_1);  // 0x10115600
XI_API undefined4 __cdecl fn_10115610(int param_1);  // 0x10115610
XI_API undefined4 __cdecl fn_10115690(int param_1, undefined4 param_2);  // 0x10115690
XI_API undefined4 __cdecl fn_101157E0(int param_1);  // 0x101157E0
XI_API undefined4 __cdecl fn_10115820(int param_1);  // 0x10115820
XI_API undefined4 __cdecl fn_10115870(int param_1);  // 0x10115870
XI_API undefined4 __cdecl fn_101158A0(int param_1, undefined4 param_2);  // 0x101158A0
XI_API undefined4 __cdecl fn_10115930(int param_1, undefined4 * param_2);  // 0x10115930
XI_API undefined4 __cdecl fn_10115970(int param_1, undefined4 * param_2);  // 0x10115970
XI_API undefined4 __cdecl fn_10115A00(int param_1, undefined4 param_2);  // 0x10115A00
XI_API undefined4 __cdecl fn_10115A90(int param_1, undefined4 param_2);  // 0x10115A90
XI_API undefined4 __cdecl fn_10115B20(int param_1, undefined4 param_2);  // 0x10115B20
XI_API undefined4 __cdecl fn_10115BB0(int param_1);  // 0x10115BB0
XI_API undefined4 __cdecl fn_10115C30(int param_1, undefined4 param_2);  // 0x10115C30
XI_API undefined4 __cdecl fn_10115CC0(int param_1, undefined4 param_2);  // 0x10115CC0
XI_API undefined4 __cdecl fn_101163A0(int param_1);  // 0x101163A0
XI_API undefined4 __cdecl fn_101163C0(int param_1, undefined4 * param_2);  // 0x101163C0
XI_API undefined4 __cdecl fn_101163F0(int param_1, undefined4 * param_2);  // 0x101163F0
XI_API undefined4 __cdecl fn_10116420(int param_1, undefined4 param_2);  // 0x10116420
XI_API undefined4 __cdecl fn_10116440(int param_1, undefined4 * param_2);  // 0x10116440
XI_API undefined4 __cdecl fn_10116460(int param_1, undefined4 * param_2);  // 0x10116460
XI_API void __cdecl fn_10116480(int param_1, undefined4 param_2, undefined4 param_3);  // 0x10116480
XI_API undefined4 __cdecl fn_101164B0(int param_1, undefined4 * param_2);  // 0x101164B0
XI_API undefined4 __cdecl fn_101164D0(int param_1, undefined4 * param_2);  // 0x101164D0
XI_API uint32_t __cdecl fn_10116520(int param_1);  // 0x10116520
XI_API undefined4 __cdecl fn_10116540(uint32_t * param_1);  // 0x10116540
XI_API void __cdecl fn_101165F0(int param_1, uint32_t param_2);  // 0x101165F0
XI_API undefined4 __cdecl fn_10116C90(uint32_t * param_1);  // 0x10116C90
XI_API int __cdecl fn_10116D30(int param_1, int param_2);  // 0x10116D30
XI_API void __cdecl fn_10116D80(int param_1, undefined4 param_2, char * param_3, undefined4 * param_4);  // 0x10116D80
XI_API undefined8 __thiscall fn_101170D0(void * self_, undefined4 param_1, undefined4 param_2);  // 0x101170D0
XI_API undefined8 __thiscall fn_101172C0(void * self_);  // 0x101172C0
XI_API void __thiscall fn_10117460(void * self_);  // 0x10117460
XI_API void __thiscall fn_10117F30(void * self_);  // 0x10117F30
XI_API void __thiscall fn_10117F40(void * self_, undefined4 param_1);  // 0x10117F40
XI_API void __thiscall fn_101180F0(void * self_);  // 0x101180F0
XI_API void __thiscall fn_10118170(void * self_, int param_1);  // 0x10118170
XI_API short * __thiscall fn_10118220(void * self_);  // 0x10118220
XI_API void __cdecl fn_10118520(void);  // 0x10118520
XI_API int __thiscall fn_101185E0(void * self_);  // 0x101185E0
XI_API int __thiscall fn_10118640(void * self_);  // 0x10118640
XI_API void __thiscall fn_101187F0(void * self_, undefined4 xi_s1);  // 0x101187F0
XI_API undefined4 __thiscall fn_10118870(void * self_);  // 0x10118870
XI_API undefined4 __thiscall fn_10118B60(void * self_, undefined4 param_1, int param_2);  // 0x10118B60
XI_API uint32_t __thiscall fn_10118C10(void * self_, uint32_t * param_1);  // 0x10118C10
XI_API undefined4 __thiscall fn_10118CB0(void * self_, int * param_1, int param_2);  // 0x10118CB0
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_10118DB0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, int param_2, undefined4 param_3);  // 0x10118DB0
XI_API undefined8 __attribute__((stdcall, regparm(3))) fn_10118E90(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, undefined4 param_2);  // 0x10118E90
XI_API void __thiscall fn_10118F20(void * self_, undefined4 xi_s1);  // 0x10118F20
XI_API void __thiscall fn_10118F40(void * self_);  // 0x10118F40
XI_API undefined8 __thiscall fn_10118F70(void * self_, undefined4 * param_1);  // 0x10118F70
XI_API void __thiscall fn_101190F0(void * self_);  // 0x101190F0
XI_API void __thiscall fn_10119200(void * self_, uint32_t param_1);  // 0x10119200
XI_API void __thiscall fn_101192E0(void * self_, undefined4 param_1);  // 0x101192E0
XI_API void * __thiscall fn_10119300(void * self_);  // 0x10119300
XI_API void __thiscall fn_10119360(void * self_);  // 0x10119360
XI_API void __thiscall fn_10119390(void * self_);  // 0x10119390
XI_API void __attribute__((stdcall, regparm(3))) fn_101193A0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x101193A0
XI_API void __thiscall fn_101193B0(void * self_);  // 0x101193B0
XI_API undefined4 __thiscall fn_101193D0(void * self_, int * param_1);  // 0x101193D0
XI_API undefined4 __thiscall fn_101193F0(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x101193F0
XI_API void __thiscall fn_10119550(void * self_, float param_1);  // 0x10119550
XI_API void * __thiscall fn_101198D0(void * self_, undefined4 param_1);  // 0x101198D0
XI_API void __thiscall fn_10119970(void * self_, undefined4 param_1);  // 0x10119970
XI_API undefined4 __thiscall fn_10119A50(void * self_, int * param_1);  // 0x10119A50
XI_API undefined4 __thiscall fn_10119AD0(void * self_, undefined4 param_1, undefined4 param_2, int param_3);  // 0x10119AD0
XI_API void __thiscall fn_10119C00(void * self_, undefined4 param_1);  // 0x10119C00
XI_API void * __thiscall fn_10119DA0(void * self_);  // 0x10119DA0
XI_API void __thiscall fn_10119E20(void * self_, undefined4 param_1);  // 0x10119E20
XI_API undefined4 __thiscall fn_10119E30(void * self_, int param_1);  // 0x10119E30
XI_API void __thiscall fn_10119E70(void * self_);  // 0x10119E70
XI_API void __thiscall fn_10119F20(void * self_);  // 0x10119F20
XI_API undefined8 __thiscall fn_10119F80(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5);  // 0x10119F80
XI_API undefined4 __thiscall fn_1011A3B0(void * self_);  // 0x1011A3B0
XI_API void __thiscall fn_1011A490(void * self_);  // 0x1011A490
XI_API void __attribute__((stdcall, regparm(3))) fn_1011A560(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x1011A560
XI_API void __thiscall fn_1011A5A0(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7);  // 0x1011A5A0
XI_API int __thiscall fn_1011AA40(__int16 *self_);  // 0x1011AA40
XI_API int __cdecl Gfx_ApplyViewport1B70(void);  // 0x1011AAC0
XI_API undefined4 __thiscall fn_1011ABF0(void * self_, int * param_1);  // 0x1011ABF0
XI_API void __thiscall fn_1011AE10(void * self_, undefined4 param_1);  // 0x1011AE10
XI_API uint32_t __thiscall fn_1011B220(void * self_, undefined4 param_1, undefined4 param_2);  // 0x1011B220
XI_API int __thiscall fn_1011B2A0(void * self_, undefined4 param_1);  // 0x1011B2A0
XI_API int __thiscall fn_1011B2E0(void * self_, undefined4 param_1);  // 0x1011B2E0
XI_API undefined4 __thiscall fn_1011B310(void * self_);  // 0x1011B310
XI_API undefined4 __thiscall fn_1011B320(void * self_);  // 0x1011B320
XI_API void __thiscall fn_1011B330(void * self_, undefined2 * param_1, undefined2 * param_2);  // 0x1011B330
XI_API undefined4 __thiscall fn_1011BA90(void * self_);  // 0x1011BA90
XI_API char __thiscall Font_BindGlyphTexture(int self_);  // 0x1011BAE0
XI_API undefined4 __thiscall fn_1011BBA0(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x1011BBA0
XI_API int __thiscall Font_DrawGlyphQuad(float *self_, __int16 a2, int *a3, unsigned int a4, float a5, float a6);  // 0x1011BBD0
XI_API int __cdecl Gfx_SetAdditiveBlend(void);  // 0x1011BDC0
XI_API undefined4 __cdecl Font_DrawChar(undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5);  // 0x1011BF40
XI_API int __stdcall fn_1011BFD0(char * param_1, undefined4 xi_s2);  // 0x1011BFD0
XI_API int __cdecl Gfx_SetCellBlendMode(int a1, int a2, int a3, int a4);  // 0x1011C1F0
XI_API void __stdcall Font_AllocHighResImage(undefined4 * param_1, int param_2, int param_3);  // 0x1011C5D0
XI_API void __thiscall Font_LoadHighResTexture(int *self_);  // 0x1011C720
XI_API void __thiscall Font_ReleaseHighResTexture(void * self_);  // 0x1011C810
XI_API uint32_t __thiscall Font_IsHighResTextureLoaded(void * self_);  // 0x1011C850
XI_API undefined4 __thiscall unknown_libname_88(void * self_);  // 0x1011C870
XI_API undefined4 __cdecl fn_1011C8B0(void);  // 0x1011C8B0
XI_API undefined4 __cdecl fn_1011C900(undefined1 * param_1);  // 0x1011C900
XI_API undefined8 __cdecl fn_1011C940(uint8_t * param_1);  // 0x1011C940
XI_API int __cdecl Font_DecodeNextChar(int * param_1);  // 0x1011CE80
XI_API int __cdecl fn_1011CEC0(uint32_t param_1);  // 0x1011CEC0
XI_API int __cdecl fn_1011CF70(char * param_1, uint32_t param_2);  // 0x1011CF70
XI_API void __cdecl fn_1011D000(char * param_1);  // 0x1011D000
XI_API void __cdecl fn_1011D010(char * param_1);  // 0x1011D010
XI_API uint32_t __attribute__((cdecl, regparm(1))) Font_AddGlyphAdvance(undefined4 xi_eax, undefined4 param_1, int * param_2);  // 0x1011D840
XI_API undefined4 __cdecl fn_1011D8B0(int param_1);  // 0x1011D8B0
XI_API undefined4 __attribute__((cdecl, regparm(2))) fn_1011D990(uint32_t xi_eax_unused, undefined4 xi_edx, uint8_t * param_1);  // 0x1011D990
XI_API undefined4 __cdecl fn_1011DA30(uint32_t param_1);  // 0x1011DA30
XI_API undefined4 __cdecl Sjis_GetCharByteLen(undefined1 * param_1);  // 0x1011DA80
XI_API undefined4 __cdecl fn_1011DB40(void * param_1);  // 0x1011DB40
XI_API size_t __cdecl fn_1011DE80(char * param_1, int param_2, char * param_3);  // 0x1011DE80
XI_API int __thiscall fn_1011E290(void * self_);  // 0x1011E290
XI_API void __thiscall fn_1011E2A0(void * self_);  // 0x1011E2A0
XI_API undefined4 __thiscall fn_1011E6D0(void * self_);  // 0x1011E6D0
XI_API int __thiscall Cell_DrawAt(void *self_, int a1, int a2);  // 0x1011E800
XI_API int __thiscall fn_1011E980(void *self_, int a1, int a2, int a3, int a4, int a5, int a6);  // 0x1011E980
XI_API void __thiscall Cell_DrawTransformed(int self_, int a2, int a3, float a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16, int a17, int a18, int a19, unsigned int a20, int a21);  // 0x1011EDE0
XI_API int __thiscall Cell_DrawStretchedWrapped(int self_, int a2, int a3, int a4, int a5, int a6, int a7, float a8, float a9, unsigned int a10, unsigned int a11, char a12);  // 0x1011EF60
XI_API void __thiscall Cell_DrawWithDefaultBlend(void * self_);  // 0x1011F2D0
XI_API int __thiscall Cell_DrawQuadBlended(void *self_, int a1, int a2, int a3, int a4, int a5);  // 0x1011F300
XI_API int __thiscall fn_1011FA80(void * self_, int param_1, short * param_2);  // 0x1011FA80
XI_API undefined8 __thiscall fn_10120000(void * self_, uint32_t param_1, uint32_t param_2, undefined4 param_3, undefined4 xi_s4, undefined4 xi_s5, undefined4 xi_s6, undefined4 xi_s7, undefined4 xi_s8, undefined4 xi_s9, undefined4 xi_s10, undefined4 xi_s11);  // 0x10120000
XI_API void __thiscall fn_10120070(void * self_);  // 0x10120070
XI_API uint32_t __cdecl Color_ModulateArgb128(uint32_t param_1, uint32_t param_2);  // 0x101200B0
XI_API undefined4 __thiscall ShapeMng_FindShape(void * self_, undefined4 param_1);  // 0x10120450
XI_API undefined4 __thiscall fn_101204B0(void * self_, undefined4 param_1);  // 0x101204B0
XI_API undefined4 __thiscall ShapeMng_GetFontGlyph(void * self_, uint32_t param_1, undefined4 param_2, undefined4 param_3, undefined2 * param_4, undefined2 * param_5);  // 0x10120510
XI_API void __thiscall ShapeMng_LoadFontTextures(void * self_);  // 0x10120660
XI_API void __thiscall fn_10120950(void * self_);  // 0x10120950
XI_API undefined4 __cdecl fn_10120B80(void);  // 0x10120B80
XI_API undefined4 __cdecl fn_10120BB0(void);  // 0x10120BB0
XI_API undefined4 __cdecl fn_10120BE0(void);  // 0x10120BE0
XI_API long double __cdecl fn_10120C40(void);  // 0x10120C40
XI_API long double __cdecl fn_10122E20(void);  // 0x10122E20
XI_API long double __cdecl fn_101232E0(void);  // 0x101232E0
XI_API undefined4 __cdecl fn_10123350(undefined4 param_1);  // 0x10123350
XI_API undefined4 __cdecl fn_10123380(uint32_t param_1, undefined4 param_2);  // 0x10123380
XI_API undefined4 __cdecl fn_10123640(undefined4 param_1);  // 0x10123640
XI_API undefined4 __cdecl fn_10123650(undefined4 param_1, undefined4 param_2);  // 0x10123650
XI_API undefined4 __cdecl fn_10123670(undefined4 param_1);  // 0x10123670
XI_API undefined4 __cdecl fn_10123680(undefined4 param_1, undefined4 param_2);  // 0x10123680
XI_API void __cdecl fn_101236B0(undefined4 param_1, undefined4 param_2);  // 0x101236B0
XI_API void __cdecl fn_101236E0(undefined4 param_1);  // 0x101236E0
XI_API undefined8 __cdecl fn_10123700(void);  // 0x10123700
XI_API undefined4 __cdecl fn_10123DC0(void);  // 0x10123DC0
XI_API undefined4 __cdecl fn_10123DF0(void);  // 0x10123DF0
XI_API undefined4 __cdecl fn_10123E20(void);  // 0x10123E20
XI_API undefined4 __cdecl fn_10123E50(uint32_t param_1);  // 0x10123E50
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_10123E60(undefined4 xi_eax, undefined4 param_1);  // 0x10123E60
XI_API void __cdecl fn_10123E80(void);  // 0x10123E80
XI_API void __cdecl fn_10123E90(void);  // 0x10123E90
XI_API void __cdecl fn_10123EA0(void);  // 0x10123EA0
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_10123EE0(undefined4 xi_eax);  // 0x10123EE0
XI_API void __thiscall fn_10123F40(void * self_, uint32_t param_1);  // 0x10123F40
XI_API void __thiscall fn_10123F60(void * self_, uint32_t param_1);  // 0x10123F60
XI_API void __thiscall fn_10123F80(void * self_, uint32_t param_1);  // 0x10123F80
XI_API void __thiscall fn_10123FC0(void * self_, uint32_t param_1);  // 0x10123FC0
XI_API void __thiscall fn_10123FE0(void * self_, uint32_t param_1);  // 0x10123FE0
XI_API void __thiscall fn_10124000(void * self_, uint32_t param_1);  // 0x10124000
XI_API undefined4 __thiscall fn_10124010(void * self_, uint32_t param_1);  // 0x10124010
XI_API undefined4 __thiscall fn_10124050(void * self_);  // 0x10124050
XI_API undefined4 __thiscall fn_10124070(void * self_, uint32_t param_1);  // 0x10124070
XI_API undefined4 __thiscall fn_10124090(void * self_, uint32_t param_1);  // 0x10124090
XI_API undefined4 __thiscall fn_101240B0(void * self_, uint32_t param_1);  // 0x101240B0
XI_API undefined4 __thiscall fn_101240D0(void * self_, uint32_t param_1);  // 0x101240D0
XI_API undefined4 __thiscall fn_101240F0(void * self_, uint32_t param_1);  // 0x101240F0
XI_API undefined4 __thiscall fn_10124110(void * self_, uint32_t param_1);  // 0x10124110
XI_API undefined4 __thiscall fn_10124130(void * self_, uint32_t param_1);  // 0x10124130
XI_API void __attribute__((cdecl, regparm(1))) fn_10124360(undefined4 xi_eax, undefined1 * param_1, undefined4 param_2);  // 0x10124360
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_10124390(undefined4 xi_eax, undefined4 param_1);  // 0x10124390
XI_API int __thiscall Cursor_Init(int self_);  // 0x10124440
XI_API undefined4 __thiscall Cursor_GetHwCursorSlot(void * self_);  // 0x101245C0
XI_API void __thiscall fn_101245D0(void * self_, undefined4 param_1, undefined4 param_2, undefined4 xi_s3);  // 0x101245D0
XI_API int __thiscall fn_10124640(int *self_, int *a2, __int16 a3);  // 0x10124640
XI_API void __thiscall fn_101248A0(void * self_);  // 0x101248A0
XI_API void __thiscall fn_10124930(void * self_);  // 0x10124930
XI_API void __thiscall fn_101249E0(void * self_);  // 0x101249E0
XI_API void __thiscall fn_10124A00(void * self_);  // 0x10124A00
XI_API void __thiscall fn_10124AB0(void * self_);  // 0x10124AB0
XI_API void __thiscall fn_10124AD0(void * self_);  // 0x10124AD0
XI_API void __thiscall fn_10124B50(void * self_);  // 0x10124B50
XI_API undefined4 __thiscall fn_10124B70(void * self_);  // 0x10124B70
XI_API void __thiscall fn_10124BC0(void * self_);  // 0x10124BC0
XI_API undefined4 __thiscall fn_10125040(void * self_);  // 0x10125040
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_10125170(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x10125170
XI_API undefined4 __thiscall fn_101252C0(void * self_);  // 0x101252C0
XI_API unsigned __int8 __thiscall Cursor_SetVisible(unsigned __int8 *self_, unsigned __int8 a2);  // 0x101252F0
XI_API undefined8 __thiscall fn_10125F70(void * self_, undefined4 param_1);  // 0x10125F70
XI_API long double __thiscall fn_101260E0(void * self_);  // 0x101260E0
XI_API long double __thiscall fn_101260F0(void * self_);  // 0x101260F0
XI_API long double __thiscall fn_10126100(void * self_);  // 0x10126100
XI_API undefined4 __cdecl fn_10126270(void);  // 0x10126270
XI_API undefined4 __cdecl unknown_libname_89(void);  // 0x10126290
XI_API void __thiscall Cursor_StoreScreenCenter(void * self_);  // 0x101262A0
XI_API undefined4 __thiscall fn_10126300(void * self_);  // 0x10126300
XI_API undefined4 __thiscall fn_10126320(void * self_);  // 0x10126320
XI_API void __cdecl fn_10126330(void);  // 0x10126330
XI_API void __thiscall fn_10126350(void * self_);  // 0x10126350
XI_API void __thiscall fn_10126370(void * self_);  // 0x10126370
XI_API undefined4 * __thiscall fn_101263B0(void * self_);  // 0x101263B0
XI_API undefined4 __stdcall fn_10126460(int param_1);  // 0x10126460
XI_API undefined8 __thiscall fn_10126480(void * self_, undefined4 param_1);  // 0x10126480
XI_API undefined8 __thiscall fn_101264C0(void * self_, undefined4 param_1);  // 0x101264C0
XI_API undefined4 __thiscall fn_10126500(void * self_);  // 0x10126500
XI_API undefined4 __thiscall fn_10126510(void * self_);  // 0x10126510
XI_API void __thiscall fn_10126580(void * self_, int * param_1);  // 0x10126580
XI_API void __thiscall fn_101265D0(void * self_);  // 0x101265D0
XI_API void __thiscall fn_101266B0(void * self_, int param_1, int param_2);  // 0x101266B0
XI_API undefined8 __thiscall fn_10126700(void * self_, int * param_1, int param_2);  // 0x10126700
XI_API void * __thiscall fn_10126750(void * self_, undefined4 param_1);  // 0x10126750
XI_API void * __thiscall fn_10126770(void * self_, undefined4 param_1);  // 0x10126770
XI_API void * __thiscall fn_10126790(void * self_, undefined4 param_1);  // 0x10126790
XI_API undefined4 __thiscall fn_101268F0(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x101268F0
XI_API void __thiscall fn_10126A00(void * self_);  // 0x10126A00
XI_API void * __thiscall fn_10126B60(void * self_, undefined4 param_1, undefined4 param_2);  // 0x10126B60
XI_API void __thiscall fn_10126BC0(void * self_, char * param_1, char * param_2, undefined4 param_3, undefined4 param_4);  // 0x10126BC0
XI_API void * __thiscall fn_10127190(void * self_);  // 0x10127190
XI_API void __thiscall fn_101271C0(void * self_);  // 0x101271C0
XI_API undefined8 __thiscall fn_10127250(void * self_);  // 0x10127250
XI_API int __thiscall fn_10127280(void * self_, undefined4 xi_s1);  // 0x10127280
XI_API void __thiscall fn_101273C0(void * self_, undefined4 param_1, char * param_2, undefined2 * param_3);  // 0x101273C0
XI_API uint32_t __cdecl fn_10127870(uint32_t param_1);  // 0x10127870
XI_API char * __cdecl fn_101278B0(char * param_1, undefined4 param_2);  // 0x101278B0
XI_API undefined4 __cdecl fn_101278D0(char * param_1, char * param_2, int param_3);  // 0x101278D0
XI_API void __cdecl fn_10127930(int param_1, char * param_2, int param_3);  // 0x10127930
XI_API void __cdecl fn_10127970(char * param_1);  // 0x10127970
XI_API uint32_t __cdecl GetCallNum(char * param_1, char * param_2);  // 0x10127990
XI_API undefined4 __cdecl fn_10127AB0(uint32_t * param_1);  // 0x10127AB0
XI_API void * __thiscall fn_10127EE0(void * self_);  // 0x10127EE0
XI_API void __thiscall fn_10127F00(void * self_);  // 0x10127F00
XI_API int __cdecl fn_10127FA0(char * param_1, undefined4 xi_s2, undefined4 xi_s3, undefined4 xi_s4, undefined4 xi_s5, undefined4 xi_s6, undefined4 xi_s7, undefined4 xi_s8, undefined4 xi_s9, undefined4 xi_s10, undefined4 xi_s11, undefined4 xi_s12, undefined4 xi_s13, undefined4 xi_s14, undefined4 xi_s15, undefined4 xi_s16, undefined4 xi_s17);  // 0x10127FA0
XI_API void __thiscall fn_101281F0(void * self_, undefined4 param_1, undefined4 param_2);  // 0x101281F0
XI_API void __attribute__((stdcall, regparm(3))) fn_10129290(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, int param_2);  // 0x10129290
XI_API int __attribute__((stdcall, regparm(3))) fn_101292E0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, short * param_2, int * param_3);  // 0x101292E0
XI_API undefined8 __stdcall fn_10129BB0(undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x10129BB0
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_10129C60(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x10129C60
XI_API undefined4 __cdecl fn_10129D00(undefined4 param_1);  // 0x10129D00
XI_API undefined4 __cdecl fn_10129EE0(char * param_1);  // 0x10129EE0
XI_API undefined8 __cdecl fn_10129F40(int param_1, undefined4 param_2, char * param_3, int param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7);  // 0x10129F40
XI_API void __cdecl fn_1012A340(void);  // 0x1012A340
XI_API void __thiscall fn_1012A350(void * self_);  // 0x1012A350
XI_API void __thiscall fn_1012A3B0(void * self_);  // 0x1012A3B0
XI_API undefined4 __thiscall fn_1012A400(void * self_);  // 0x1012A400
XI_API undefined4 __thiscall fn_1012A410(void * self_);  // 0x1012A410
XI_API uint32_t __thiscall fn_1012A420(void * self_, char * param_1, undefined4 param_2, int param_3, int param_4);  // 0x1012A420
XI_API undefined4 __thiscall fn_1012A4E0(void * self_);  // 0x1012A4E0
XI_API void * __thiscall fn_1012AB20(void * self_);  // 0x1012AB20
XI_API void * __thiscall fn_1012BEF0(void * self_);  // 0x1012BEF0
XI_API void * __thiscall fn_1012C210(void * self_);  // 0x1012C210
XI_API void * __thiscall fn_1012C540(void * self_);  // 0x1012C540
XI_API void * __thiscall fn_1012C910(void * self_);  // 0x1012C910
XI_API void * __thiscall fn_1012D450(void * self_);  // 0x1012D450
XI_API void __thiscall fn_1012E180(void * self_);  // 0x1012E180
XI_API void * __thiscall fn_1012E360(void * self_);  // 0x1012E360
XI_API void * __thiscall fn_10130390(void * self_);  // 0x10130390
XI_API void __thiscall fn_101304D0(void * self_);  // 0x101304D0
XI_API void __thiscall fn_10130F90(void * self_);  // 0x10130F90
XI_API undefined4 * __thiscall fn_101312E0(void * self_);  // 0x101312E0
XI_API void * __thiscall fn_10131530(void * self_);  // 0x10131530
XI_API void * __thiscall fn_10131940(void * self_);  // 0x10131940
XI_API void * __thiscall fn_10133310(void * self_);  // 0x10133310
XI_API void * __thiscall fn_10135850(void * self_);  // 0x10135850
XI_API void * __thiscall fn_10136380(void * self_, undefined4 param_1);  // 0x10136380
XI_API void __thiscall fn_10136560(void * self_);  // 0x10136560
XI_API undefined4 __thiscall fn_10136870(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x10136870
XI_API void * __thiscall fn_10136900(void * self_);  // 0x10136900
XI_API void * __thiscall fn_10136950(void * self_, undefined4 param_1);  // 0x10136950
XI_API void * __thiscall fn_101369C0(void * self_, undefined4 param_1);  // 0x101369C0
XI_API void * __thiscall fn_10136A30(void * self_, undefined4 param_1);  // 0x10136A30
XI_API void * __thiscall fn_10136E20(void * self_, undefined4 param_1);  // 0x10136E20
XI_API void * __thiscall fn_10136EC0(void * self_, undefined4 param_1);  // 0x10136EC0
XI_API void * __thiscall fn_10136F60(void * self_, undefined4 param_1);  // 0x10136F60
XI_API void __thiscall fn_101371E0(void * self_, int param_1);  // 0x101371E0
XI_API void * __thiscall fn_10137630(void * self_);  // 0x10137630
XI_API void * __thiscall fn_10138960(void * self_);  // 0x10138960
XI_API void __thiscall fn_10138EC0(void * self_);  // 0x10138EC0
XI_API undefined1 __cdecl fn_10138FB0(void);  // 0x10138FB0
XI_API void * __thiscall fn_10139840(void * self_);  // 0x10139840
XI_API void * __thiscall fn_10139EE0(void * self_);  // 0x10139EE0
XI_API void __thiscall fn_10139FE0(void * self_);  // 0x10139FE0
XI_API void __thiscall fn_10139FF0(void * self_, int param_1);  // 0x10139FF0
XI_API void __thiscall fn_1013A010(void * self_);  // 0x1013A010
XI_API int __thiscall fn_1013A030(void * self_);  // 0x1013A030
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_1013A060(undefined4 xi_eax);  // 0x1013A060
XI_API undefined4 __thiscall fn_1013A070(void * self_);  // 0x1013A070
XI_API void __attribute__((stdcall, regparm(3))) fn_1013A100(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x1013A100
XI_API void * __thiscall fn_1013A1E0(void * self_);  // 0x1013A1E0
XI_API void __thiscall fn_1013A320(void * self_, undefined4 xi_s1, undefined4 xi_s2);  // 0x1013A320
XI_API void __thiscall fn_1013A330(void * self_, undefined4 xi_s1);  // 0x1013A330
XI_API void __thiscall fn_1013A350(void * self_);  // 0x1013A350
XI_API void * __thiscall fn_1013A480(void * self_);  // 0x1013A480
XI_API void * __thiscall fn_1013A930(void * self_);  // 0x1013A930
XI_API void * __thiscall fn_1013AA60(void * self_);  // 0x1013AA60
XI_API void * __thiscall fn_1013AB20(void * self_);  // 0x1013AB20
XI_API void * __thiscall fn_1013BE50(void * self_);  // 0x1013BE50
XI_API void __thiscall fn_1013BF40(void * self_);  // 0x1013BF40
XI_API void * __thiscall fn_1013E7F0(void * self_);  // 0x1013E7F0
XI_API void * __thiscall fn_1013FBE0(void * self_);  // 0x1013FBE0
XI_API void * __thiscall fn_10140950(void * self_);  // 0x10140950
XI_API void * __thiscall fn_10140E70(void * self_);  // 0x10140E70
XI_API void __thiscall fn_10141AD0(void * self_);  // 0x10141AD0
XI_API void * __thiscall fn_101436C0(void * self_);  // 0x101436C0
XI_API void * __thiscall fn_10144680(void * self_);  // 0x10144680
XI_API void * __thiscall fn_10144D60(void * self_);  // 0x10144D60
XI_API void * __thiscall fn_10144F40(void * self_);  // 0x10144F40
XI_API void * __thiscall fn_10146340(void * self_);  // 0x10146340
XI_API void __thiscall fn_10146930(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x10146930
XI_API void __thiscall fn_10146960(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x10146960
XI_API void * __thiscall fn_101475B0(void * self_);  // 0x101475B0
XI_API void * __thiscall fn_10148430(void * self_);  // 0x10148430
XI_API void * __thiscall fn_10148640(void * self_);  // 0x10148640
XI_API void * __thiscall fn_10148ED0(void * self_);  // 0x10148ED0
XI_API void * __thiscall fn_1014A170(void * self_);  // 0x1014A170
XI_API void * __thiscall fn_1014A7D0(void * self_);  // 0x1014A7D0
XI_API void * __thiscall fn_1014B670(void * self_);  // 0x1014B670
XI_API void * __thiscall fn_1014BA60(void * self_);  // 0x1014BA60
XI_API void * __thiscall fn_1014C750(void * self_);  // 0x1014C750
XI_API void * __thiscall fn_1014D970(void * self_);  // 0x1014D970
XI_API void * __thiscall fn_1014DD30(void * self_);  // 0x1014DD30
XI_API void __thiscall fn_1014E020(void * self_);  // 0x1014E020
XI_API void __stdcall fn_1014E050(undefined4 * param_1);  // 0x1014E050
XI_API void * __thiscall fn_1014EDC0(void * self_);  // 0x1014EDC0
XI_API void * __thiscall fn_1014F840(void * self_);  // 0x1014F840
XI_API void * __thiscall fn_1014FFE0(void * self_);  // 0x1014FFE0
XI_API void __attribute__((stdcall, regparm(3))) fn_10150090(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, undefined4 xi_s1);  // 0x10150090
XI_API void __thiscall fn_10150A30(void * self_, char * param_1);  // 0x10150A30
XI_API void __thiscall fn_10150A60(void * self_, undefined4 param_1);  // 0x10150A60
XI_API void * __thiscall fn_10150AB0(void * self_);  // 0x10150AB0
XI_API void __thiscall fn_10150CC0(void * self_, int param_1, int param_2);  // 0x10150CC0
XI_API void __thiscall fn_10150CE0(void * self_, int param_1, int param_2);  // 0x10150CE0
XI_API undefined4 __thiscall fn_10150D90(void * self_);  // 0x10150D90
XI_API void * __thiscall fn_10151040(void * self_);  // 0x10151040
XI_API void __thiscall fn_10151140(void * self_);  // 0x10151140
XI_API void __cdecl fn_10152EF0(int param_1, undefined4 param_2);  // 0x10152EF0
XI_API void __cdecl fn_10152F20(int param_1, undefined4 param_2);  // 0x10152F20
XI_API void __cdecl fn_10152F80(void);  // 0x10152F80
XI_API undefined4 __cdecl fn_10152FA0(void);  // 0x10152FA0
XI_API undefined4 __cdecl fn_10152FC0(int param_1);  // 0x10152FC0
XI_API undefined4 __cdecl fn_10152FF0(int param_1, int param_2);  // 0x10152FF0
XI_API int __cdecl fn_10153090(int param_1, int param_2);  // 0x10153090
XI_API int __cdecl fn_101530C0(int param_1, int param_2);  // 0x101530C0
XI_API int __cdecl fn_10153100(int param_1, int param_2);  // 0x10153100
XI_API undefined4 __cdecl fn_10153130(char * param_1);  // 0x10153130
XI_API void __cdecl fn_10153150(undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 xi_s5);  // 0x10153150
XI_API void __cdecl fn_10153180(undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 xi_s6);  // 0x10153180
XI_API int __cdecl fn_101531B0(uint8_t * param_1);  // 0x101531B0
XI_API undefined4 __cdecl fn_101531D0(uint32_t * param_1, char * param_2, int param_3);  // 0x101531D0
XI_API int __cdecl fn_10153230(int param_1, uint32_t param_2, int param_3);  // 0x10153230
XI_API undefined4 __cdecl fn_10153AF0(void);  // 0x10153AF0
XI_API undefined4 __cdecl fn_10153B30(void);  // 0x10153B30
XI_API undefined4 __cdecl fn_10153C10(void);  // 0x10153C10
XI_API undefined4 __cdecl fn_10153EF0(undefined4 param_1, int param_2);  // 0x10153EF0
XI_API void __thiscall fn_10153F80(void * self_);  // 0x10153F80
XI_API undefined * __cdecl fn_10153FF0(void);  // 0x10153FF0
XI_API void * __thiscall fn_10154070(void * self_);  // 0x10154070
XI_API int __thiscall fn_101544F0(void * self_, undefined4 param_1);  // 0x101544F0
XI_API undefined4 __thiscall fn_10154520(void * self_, undefined4 param_1);  // 0x10154520
XI_API undefined1 * __thiscall fn_10154550(void * self_, int param_1);  // 0x10154550
XI_API char __thiscall fn_10154570(void * self_);  // 0x10154570
XI_API void __thiscall fn_10154840(void * self_);  // 0x10154840
XI_API void __thiscall fn_10154860(void * self_);  // 0x10154860
XI_API undefined4 __thiscall fn_101549E0(void * self_);  // 0x101549E0
XI_API undefined4 __thiscall fn_101549F0(void * self_, int param_1);  // 0x101549F0
XI_API void __thiscall fn_10154A80(void * self_);  // 0x10154A80
XI_API void __thiscall fn_10154AB0(void * self_);  // 0x10154AB0
XI_API void __thiscall fn_10154AD0(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6);  // 0x10154AD0
XI_API undefined4 __thiscall fn_10154DB0(void * self_);  // 0x10154DB0
XI_API undefined4 * __thiscall fn_10154DC0(void * self_);  // 0x10154DC0
XI_API void __thiscall fn_10154E30(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, char * param_4, char * param_5, undefined4 param_6, char * param_7);  // 0x10154E30
XI_API int __thiscall fn_10154F00(void * self_);  // 0x10154F00
XI_API void __thiscall fn_10154F30(void * self_);  // 0x10154F30
XI_API void __thiscall fn_10154F60(void * self_);  // 0x10154F60
XI_API void __thiscall fn_10154FC0(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x10154FC0
XI_API undefined4 __thiscall fn_10155140(void * self_);  // 0x10155140
XI_API void * __thiscall fn_10155160(void * self_);  // 0x10155160
XI_API void __thiscall fn_101551C0(void * self_, int param_1, char * param_2, char * param_3, char * param_4, undefined4 param_5);  // 0x101551C0
XI_API int __thiscall fn_101552D0(void * self_);  // 0x101552D0
XI_API undefined4 __thiscall fn_10155360(void * self_);  // 0x10155360
XI_API void * __thiscall fn_10155370(void * self_);  // 0x10155370
XI_API void __thiscall fn_10155400(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, char * param_4, char * param_5, undefined4 param_6, undefined4 param_7, undefined4 param_8, char * param_9);  // 0x10155400
XI_API int __thiscall fn_101554E0(void * self_);  // 0x101554E0
XI_API undefined4 __thiscall fn_10155540(void * self_);  // 0x10155540
XI_API void * __thiscall fn_10155550(void * self_);  // 0x10155550
XI_API void __thiscall fn_101555B0(void * self_, char * param_1);  // 0x101555B0
XI_API int __thiscall fn_10155600(void * self_);  // 0x10155600
XI_API undefined4 __cdecl fn_10155BD0(undefined4 * param_1, uint32_t param_2, undefined4 param_3, undefined4 param_4, char * param_5, undefined4 param_6, undefined4 param_7, char * param_8);  // 0x10155BD0
XI_API undefined4 __cdecl fn_10155C70(undefined4 * param_1, uint32_t param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, char * param_6, undefined4 param_7, undefined4 param_8, char * param_9);  // 0x10155C70
XI_API undefined4 __thiscall fn_10156440(void * self_);  // 0x10156440
XI_API undefined4 __thiscall fn_10156450(void * self_);  // 0x10156450
XI_API undefined4 __thiscall fn_10156470(void * self_);  // 0x10156470
XI_API undefined4 __thiscall fn_10156480(void * self_);  // 0x10156480
XI_API undefined4 __cdecl fn_10156540(undefined4 * param_1, uint32_t param_2, undefined4 param_3, char * param_4, char * param_5);  // 0x10156540
XI_API undefined4 __thiscall fn_10156840(void * self_);  // 0x10156840
XI_API undefined4 __thiscall fn_10156850(void * self_);  // 0x10156850
XI_API undefined4 __thiscall fn_10156860(void * self_);  // 0x10156860
XI_API undefined4 __cdecl fn_10156870(undefined1 * param_1, uint32_t param_2, undefined4 param_3, undefined4 param_4, char * param_5, char * param_6, undefined4 param_7, undefined4 param_8, undefined4 param_9, char * param_10);  // 0x10156870
XI_API void __thiscall fn_10156970(void * self_);  // 0x10156970
XI_API void __thiscall fn_101569A0(void * self_);  // 0x101569A0
XI_API void __thiscall fn_101569F0(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7, char * param_8);  // 0x101569F0
XI_API undefined4 __thiscall fn_10156B20(void * self_);  // 0x10156B20
XI_API undefined4 __thiscall fn_10156B30(void * self_);  // 0x10156B30
XI_API undefined4 __thiscall fn_10156B40(void * self_);  // 0x10156B40
XI_API undefined4 __cdecl fn_10156B50(int * param_1, uint32_t param_2, char * param_3);  // 0x10156B50
XI_API void __thiscall fn_10156BB0(void * self_);  // 0x10156BB0
XI_API void __thiscall fn_10156BE0(void * self_);  // 0x10156BE0
XI_API void __thiscall fn_10156BF0(void * self_, undefined4 param_1);  // 0x10156BF0
XI_API void * __thiscall fn_10156D90(void * self_);  // 0x10156D90
XI_API void __thiscall fn_10156E00(void * self_);  // 0x10156E00
XI_API void * __thiscall fn_101575E0(void * self_);  // 0x101575E0
XI_API void __thiscall fn_10157610(void * self_);  // 0x10157610
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_10157620(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x10157620
XI_API undefined4 __thiscall fn_10157660(void * self_, uint32_t param_1);  // 0x10157660
XI_API undefined4 __cdecl fn_101576F0(void);  // 0x101576F0
XI_API void __thiscall fn_10157720(void * self_, int param_1, int param_2);  // 0x10157720
XI_API void __thiscall fn_10157780(void * self_, int param_1, int param_2);  // 0x10157780
XI_API undefined4 __thiscall fn_10157860(void * self_, undefined4 param_1, undefined4 param_2);  // 0x10157860
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_10157880(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x10157880
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_10157890(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x10157890
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_101578A0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x101578A0
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_101578B0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x101578B0
XI_API undefined4 __thiscall fn_101578C0(void * self_);  // 0x101578C0
XI_API undefined4 __thiscall fn_101578D0(void * self_);  // 0x101578D0
XI_API undefined4 __thiscall fn_101578E0(void * self_);  // 0x101578E0
XI_API void __thiscall fn_101578F0(void * self_);  // 0x101578F0
XI_API void __thiscall fn_101579B0(void * self_);  // 0x101579B0
XI_API void __thiscall fn_101579C0(void * self_);  // 0x101579C0
XI_API undefined4 __thiscall fn_10157B40(void * self_, int param_1, undefined4 param_2, undefined4 param_3);  // 0x10157B40
XI_API char __thiscall fn_10157C60(void * self_, int param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x10157C60
XI_API undefined4 __cdecl fn_10157D20(void);  // 0x10157D20
XI_API undefined4 __cdecl fn_10157D60(void);  // 0x10157D60
XI_API void __thiscall fn_10157E20(void * self_);  // 0x10157E20
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_10157FA0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x10157FA0
XI_API void __attribute__((stdcall, regparm(3))) fn_101581E0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x101581E0
XI_API void __thiscall fn_101582F0(void * self_);  // 0x101582F0
XI_API void __thiscall fn_101583F0(void * self_);  // 0x101583F0
XI_API undefined4 __thiscall fn_10158470(void * self_);  // 0x10158470
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_10158480(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x10158480
XI_API void __thiscall fn_10158510(void * self_);  // 0x10158510
XI_API int * __thiscall fn_10158550(void * self_);  // 0x10158550
XI_API undefined4 __thiscall fn_10158590(void * self_);  // 0x10158590
XI_API undefined4 __thiscall fn_101585F0(void * self_);  // 0x101585F0
XI_API undefined4 __cdecl fn_101587B0(undefined4 * param_1);  // 0x101587B0
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_10158AA0(undefined4 xi_eax);  // 0x10158AA0
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_10158AE0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, undefined4 xi_s1);  // 0x10158AE0
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_10158B00(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, undefined4 param_2);  // 0x10158B00
XI_API undefined4 __thiscall fn_10158B40(void * self_, int param_1);  // 0x10158B40
XI_API int __thiscall fn_10158B60(void * self_);  // 0x10158B60
XI_API undefined4 __thiscall fn_10158C40(void * self_, undefined4 param_1);  // 0x10158C40
XI_API int * __thiscall fn_10158CA0(void * self_, undefined4 param_1);  // 0x10158CA0
XI_API undefined4 __thiscall fn_10158D90(void * self_);  // 0x10158D90
XI_API undefined4 __thiscall fn_10158DB0(void * self_, undefined4 param_1);  // 0x10158DB0
XI_API undefined4 __thiscall fn_10158FF0(void * self_);  // 0x10158FF0
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_10159040(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x10159040
XI_API undefined4 __thiscall fn_101590A0(void * self_);  // 0x101590A0
XI_API void __thiscall fn_101590C0(void * self_, int * param_1);  // 0x101590C0
XI_API void __thiscall fn_10159170(void * self_, uint32_t param_1);  // 0x10159170
XI_API undefined4 __thiscall fn_101591D0(void * self_);  // 0x101591D0
XI_API int __thiscall fn_10159370(void * self_);  // 0x10159370
XI_API undefined * __thiscall fn_10159380(void * self_);  // 0x10159380
XI_API undefined4 __cdecl Actor_SetSlotTableEntry(uint32_t param_1, undefined4 param_2);  // 0x10159400
XI_API undefined4 __cdecl fn_10159420(int param_1, undefined4 param_2);  // 0x10159420
XI_API undefined4 __thiscall fn_10159440(void * self_, undefined4 param_1, undefined4 param_2);  // 0x10159440
XI_API void __thiscall fn_101594D0(void * self_, undefined4 param_1);  // 0x101594D0
XI_API void __thiscall fn_101594E0(void * self_, undefined4 param_1);  // 0x101594E0
XI_API undefined4 __thiscall fn_101594F0(void * self_);  // 0x101594F0
XI_API void * __thiscall fn_10159530(void * self_);  // 0x10159530
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_10159540(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x10159540
XI_API undefined4 __thiscall fn_101595D0(void * self_, int param_1);  // 0x101595D0
XI_API undefined4 __thiscall fn_10159660(void * self_, void * param_1);  // 0x10159660
XI_API undefined4 __thiscall fn_10159740(void * self_);  // 0x10159740
XI_API undefined4 __thiscall fn_10159780(void * self_);  // 0x10159780
XI_API undefined4 __thiscall fn_101597C0(void * self_, undefined4 param_1, undefined4 param_2);  // 0x101597C0
XI_API uint32_t __thiscall fn_101597F0(void * self_);  // 0x101597F0
XI_API void __thiscall fn_10159830(void * self_, char * param_1, int * param_2);  // 0x10159830
XI_API undefined4 __thiscall fn_10159880(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x10159880
XI_API undefined4 __thiscall fn_101598A0(void * self_);  // 0x101598A0
XI_API undefined4 __thiscall fn_101598B0(void * self_);  // 0x101598B0
XI_API undefined4 __thiscall fn_101598C0(void * self_, float param_1);  // 0x101598C0
XI_API uint32_t __thiscall fn_10159910(void * self_, int * param_1);  // 0x10159910
XI_API undefined4 __thiscall fn_10159970(void * self_, undefined4 param_1);  // 0x10159970
XI_API undefined4 __thiscall fn_10159A20(void * self_, float * param_1);  // 0x10159A20
XI_API void __stdcall fn_10159B10(float * param_1);  // 0x10159B10
XI_API undefined4 __thiscall fn_10159D40(void * self_, undefined2 * param_1, undefined2 * param_2);  // 0x10159D40
XI_API int * __thiscall fn_10159DE0(void * self_);  // 0x10159DE0
XI_API uint32_t __attribute__((cdecl, regparm(1))) fn_10159E10(undefined4 xi_eax, int * param_1);  // 0x10159E10
XI_API undefined4 * __cdecl fn_10159FB0(int param_1);  // 0x10159FB0
XI_API void __attribute__((stdcall, regparm(3))) fn_1015AF60(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x1015AF60
XI_API void * __thiscall fn_1015AF80(void * self_);  // 0x1015AF80
XI_API undefined8 __thiscall fn_1015AFA0(void * self_);  // 0x1015AFA0
XI_API uint32_t __thiscall fn_1015B190(void * self_);  // 0x1015B190
XI_API undefined4 __thiscall fn_1015B270(void * self_);  // 0x1015B270
XI_API void __thiscall fn_1015B3C0(void * self_);  // 0x1015B3C0
XI_API void * __thiscall fn_1015B720(void * self_);  // 0x1015B720
XI_API undefined * __cdecl fn_1015BAF0(void);  // 0x1015BAF0
XI_API undefined4 __stdcall fn_1015BD40(int param_1);  // 0x1015BD40
XI_API void __cdecl fn_1015D7F0(int param_1);  // 0x1015D7F0
XI_API undefined1 __cdecl fn_1015D860(void);  // 0x1015D860
XI_API undefined1 __attribute__((cdecl, regparm(1))) fn_1015D880(undefined4 xi_eax);  // 0x1015D880
XI_API void __thiscall MenuMng_Init(void * self_);  // 0x1015D900
XI_API undefined4 __thiscall fn_1015D9A0(void * self_);  // 0x1015D9A0
XI_API void __thiscall fn_1015DB10(void * self_, int param_1);  // 0x1015DB10
XI_API void __stdcall fn_1015DB70(int param_1);  // 0x1015DB70
XI_API void __thiscall fn_1015DBB0(void * self_, int param_1, undefined4 param_2);  // 0x1015DBB0
XI_API void __thiscall fn_1015DBD0(void * self_, int param_1);  // 0x1015DBD0
XI_API int __thiscall fn_1015DC10(void * self_, char * param_1);  // 0x1015DC10
XI_API undefined4 __stdcall fn_1015DC70(char * param_1, undefined1 * param_2);  // 0x1015DC70
XI_API undefined8 __thiscall fn_1015DD00(void * self_, undefined4 param_1);  // 0x1015DD00
XI_API undefined4 __thiscall fn_1015DD90(void * self_);  // 0x1015DD90
XI_API undefined4 __thiscall fn_1015DE50(void * self_);  // 0x1015DE50
XI_API int __thiscall fn_1015DEE0(void * self_);  // 0x1015DEE0
XI_API undefined4 __thiscall fn_1015DEF0(void * self_);  // 0x1015DEF0
XI_API void __thiscall fn_1015DF20(void * self_);  // 0x1015DF20
XI_API void __stdcall fn_1015DFC0(undefined4 xi_s1);  // 0x1015DFC0
XI_API int __thiscall fn_1015E030(void * self_, char * param_1);  // 0x1015E030
XI_API int __thiscall fn_1015E7E0(void * self_);  // 0x1015E7E0
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_1015E850(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, int param_2);  // 0x1015E850
XI_API void __attribute__((stdcall, regparm(3))) fn_1015E8A0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, int param_2);  // 0x1015E8A0
XI_API undefined4 __thiscall fn_1015E910(void * self_, undefined4 param_1);  // 0x1015E910
XI_API void __stdcall fn_1015E950(int param_1, uint8_t * param_2);  // 0x1015E950
XI_API void __thiscall fn_1015EB60(void * self_, undefined4 xi_s1);  // 0x1015EB60
XI_API int __thiscall fn_1015EC20(void * self_, undefined4 param_1, undefined4 param_2);  // 0x1015EC20
XI_API undefined4 __stdcall fn_1015EC50(char * param_1);  // 0x1015EC50
XI_API undefined4 __stdcall fn_1015ECC0(uint32_t param_1);  // 0x1015ECC0
XI_API undefined4 __stdcall fn_1015ED10(undefined4 param_1, uint32_t param_2, undefined4 param_3);  // 0x1015ED10
XI_API undefined4 __thiscall fn_1015ED80(void * self_, int param_1);  // 0x1015ED80
XI_API int __thiscall fn_1015EDB0(void * self_);  // 0x1015EDB0
XI_API undefined8 __stdcall fn_1015EE00(int * param_1);  // 0x1015EE00
XI_API uint32_t __stdcall fn_1015EE20(int * param_1);  // 0x1015EE20
XI_API undefined4 __cdecl fn_1015EE40(void);  // 0x1015EE40
XI_API void __cdecl fn_1015EEF0(void);  // 0x1015EEF0
XI_API void __thiscall fn_1015EF20(void * self_);  // 0x1015EF20
XI_API undefined4 __thiscall fn_1015F3E0(void * self_, uint32_t param_1);  // 0x1015F3E0
XI_API void __thiscall fn_1015F6A0(void * self_);  // 0x1015F6A0
XI_API undefined4 __stdcall fn_1015F790(int * param_1, int param_2);  // 0x1015F790
XI_API undefined4 __stdcall fn_1015F8F0(int * param_1, short * param_2);  // 0x1015F8F0
XI_API undefined4 __thiscall fn_1015F960(void * self_, undefined4 param_1);  // 0x1015F960
XI_API int __cdecl fn_1015F980(int param_1);  // 0x1015F980
XI_API undefined4 __thiscall fn_1015F9C0(void * self_, int param_1);  // 0x1015F9C0
XI_API int __thiscall fn_1015F9E0(void * self_, int param_1);  // 0x1015F9E0
XI_API undefined8 __thiscall fn_101601D0(void * self_, float * param_1, float * param_2);  // 0x101601D0
XI_API undefined8 __thiscall fn_10160210(void * self_, float * param_1, float * param_2);  // 0x10160210
XI_API long double __thiscall MenuMng_GetOffsetX(void * self_, undefined4 param_2);  // 0x10160290
XI_API long double __thiscall MenuMng_GetOffsetY(void * self_, undefined4 param_2);  // 0x101602C0
XI_API void __thiscall fn_10160440(void * self_, undefined4 * param_1, int param_2);  // 0x10160440
XI_API int __cdecl Gfx_SetDefault2DStates(void);  // 0x101604B0
XI_API int __cdecl Gfx_DisableFogAndAlphaBlend(void);  // 0x10160650
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_101606F0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x101606F0
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_10160700(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x10160700
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_10160710(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, undefined4 param_2);  // 0x10160710
XI_API void __cdecl fn_101609C0(void);  // 0x101609C0
XI_API void __cdecl fn_10160A50(void);  // 0x10160A50
XI_API undefined4 __thiscall fn_10160C90(void * self_);  // 0x10160C90
XI_API void * __thiscall fn_10160DB0(void * self_);  // 0x10160DB0
XI_API void * __thiscall fn_10161110(void * self_);  // 0x10161110
XI_API void * __thiscall fn_10161380(void * self_);  // 0x10161380
XI_API void * __thiscall fn_101615E0(void * self_);  // 0x101615E0
XI_API void * __thiscall fn_10161850(void * self_, undefined4 param_1);  // 0x10161850
XI_API void __thiscall fn_10161970(void * self_, undefined4 param_1);  // 0x10161970
XI_API void __attribute__((stdcall, regparm(3))) fn_10161990(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x10161990
XI_API void * __thiscall fn_10161B80(void * self_, undefined4 param_1);  // 0x10161B80
XI_API undefined4 __cdecl fn_10161BC0(void);  // 0x10161BC0
XI_API void __thiscall fn_10161C70(void * self_, undefined4 xi_s1);  // 0x10161C70
XI_API void __attribute__((stdcall, regparm(3))) fn_10161D60(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x10161D60
XI_API void * __thiscall fn_10161ED0(void * self_, undefined4 param_1);  // 0x10161ED0
XI_API undefined4 __cdecl fn_10161FF0(uint32_t * param_1, int param_2);  // 0x10161FF0
XI_API int __cdecl fn_10162010(int a1, int a2);  // 0x10162010
XI_API int __cdecl Gfx_ApplyViewport1904(void);  // 0x101620B0
XI_API void __cdecl fn_101620D0(void);  // 0x101620D0
XI_API undefined8 __thiscall fn_10162F60(void * self_);  // 0x10162F60
XI_API undefined8 __thiscall fn_10162FB0(void * self_, int * param_1);  // 0x10162FB0
XI_API int __thiscall fn_10163080(void * self_);  // 0x10163080
XI_API void * __thiscall fn_10163090(void * self_);  // 0x10163090
XI_API void __thiscall fn_101630C0(void * self_);  // 0x101630C0
XI_API void * __thiscall fn_10163350(void * self_);  // 0x10163350
XI_API void __cdecl fn_10163380(void);  // 0x10163380
XI_API void __thiscall Obj_10621944_DrawBgRect(void * self_);  // 0x10163390
XI_API void __thiscall fn_101633D0(void * self_);  // 0x101633D0
XI_API int __stdcall Gfx_DrawSolidRect2D(int a1, int a2);  // 0x10163400
XI_API void __thiscall fn_10163F30(void * self_);  // 0x10163F30
XI_API void __thiscall fn_10163FA0(void * self_);  // 0x10163FA0
XI_API void * __thiscall fn_101641B0(void * self_);  // 0x101641B0
XI_API void __thiscall fn_10164560(void * self_);  // 0x10164560
XI_API void * __thiscall fn_10164660(void * self_);  // 0x10164660
XI_API void __thiscall fn_10164800(void * self_);  // 0x10164800
XI_API void __thiscall fn_10164980(void * self_);  // 0x10164980
XI_API void * __thiscall fn_10164A30(void * self_);  // 0x10164A30
XI_API void * __thiscall fn_10164AF0(void * self_);  // 0x10164AF0
XI_API void __cdecl fn_101671D0(void);  // 0x101671D0
XI_API void * __thiscall fn_10167330(void * self_);  // 0x10167330
XI_API undefined4 __thiscall fn_10167340(void * self_);  // 0x10167340
XI_API void * __thiscall fn_10167440(void * self_);  // 0x10167440
XI_API void * __thiscall fn_10167800(void * self_);  // 0x10167800
XI_API void __thiscall fn_10167950(void * self_);  // 0x10167950
XI_API void __thiscall fn_101679A0(void * self_, int param_1);  // 0x101679A0
XI_API undefined4 __thiscall fn_10168FD0(void * self_);  // 0x10168FD0
XI_API undefined4 __thiscall fn_10169290(void * self_, float * param_1, float * param_2, undefined4 param_3, float param_4, float param_5, float param_6);  // 0x10169290
XI_API uint32_t __thiscall fn_101693B0(void * self_, float * param_1, float * param_2, float * param_3);  // 0x101693B0
XI_API uint32_t __thiscall fn_10169B40(void * self_, float * param_1, float * param_2, float * param_3);  // 0x10169B40
XI_API uint32_t __thiscall fn_1016A2D0(void * self_, float * param_1, float * param_2, undefined4 xi_s3);  // 0x1016A2D0
XI_API undefined4 __thiscall fn_1016B1D0(void * self_, float * param_1, float * param_2, float * param_3);  // 0x1016B1D0
XI_API void __cdecl Mat44_Multiply(float * param_1, int param_2, float * param_3);  // 0x1016C060
XI_API undefined4 __thiscall fn_1016C150(void * self_);  // 0x1016C150
XI_API undefined4 __stdcall fn_1016C390(float * param_1, int param_2);  // 0x1016C390
XI_API int __stdcall Dbg_DrawTriangleLines(int a1, int a2, int a3);  // 0x1016DDF0
XI_API undefined4 __thiscall fn_1016F0E0(void * self_, float * param_1, float * param_2);  // 0x1016F0E0
XI_API void * __thiscall fn_1016FC40(void * self_);  // 0x1016FC40
XI_API void __thiscall fn_1016FC90(void * self_);  // 0x1016FC90
XI_API _DWORD * __thiscall fn_1016FCD0(void *self_, int *a2, int a3, int a4, int a5, int a6, _DWORD *a7, int a8);  // 0x1016FCD0
XI_API void __thiscall fn_10170CD0(void * self_, undefined4 * param_1);  // 0x10170CD0
XI_API int * __thiscall fn_10170F30(void * self_, char * param_1, int param_2);  // 0x10170F30
XI_API void * __thiscall fn_101718E0(void * self_);  // 0x101718E0
XI_API void __thiscall fn_10174770(void * self_);  // 0x10174770
XI_API void __thiscall Dbg_DrawOverheadMapMarkers(_DWORD *self_, float *a2);  // 0x10174B10
XI_API int __cdecl Dbg_DrawOrientedBoxes(void);  // 0x10174EC0
XI_API void __cdecl Mat4_SetIdentity_2(undefined4 * param_1);  // 0x10175740
XI_API int __stdcall fn_101771B0(int a1, int a2);  // 0x101771B0
XI_API int __thiscall Gfx_AssembleVertexShader(void *self_, int a1, int a2);  // 0x10177240
XI_API int __thiscall fn_101772E0(const char **self_, int a2);  // 0x101772E0
XI_API undefined4 * __cdecl fn_101773F0(void);  // 0x101773F0
XI_API int __cdecl Gfx_SetZBias(unsigned int a1);  // 0x10177400
XI_API void __thiscall ZoneMap_SetOverlayProjection(void * self_);  // 0x10177420
XI_API uint32_t __attribute__((cdecl, regparm(1))) fn_10177770(undefined4 xi_eax, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x10177770
XI_API undefined4 __thiscall fn_101779B0(void * self_, undefined4 * param_1, int param_2);  // 0x101779B0
XI_API int __cdecl Gfx_SetupLineDrawState(void);  // 0x101779C0
XI_API int __cdecl Gfx_DrawQuadOutline(float *a1, char a2, unsigned __int8 arg8, unsigned __int8 a4, char a5, char a6);  // 0x10177B40
XI_API int __cdecl Gfx_DrawLine3D(int a1, int a2, int a3, int a4, int a5, int a6);  // 0x10177C50
XI_API int __thiscall fn_10177CD0(void *self_, float *a2, int a3, float a4);  // 0x10177CD0
XI_API void * __thiscall fn_10177D00(void * self_);  // 0x10177D00
XI_API void __thiscall fn_10177FA0(void * self_);  // 0x10177FA0
XI_API void __cdecl fn_101784E0(void);  // 0x101784E0
XI_API void __thiscall fn_101787C0(void * self_);  // 0x101787C0
XI_API undefined4 __thiscall fn_10178A20(void * self_, int param_1, int param_2);  // 0x10178A20
XI_API void __thiscall fn_10178AC0(void * self_, int param_1);  // 0x10178AC0
XI_API int __thiscall ZoneMap_ReleaseStateBlock(CTsZoneMap *self_);  // 0x10178B00
XI_API uint32_t __thiscall fn_10178B30(void * self_);  // 0x10178B30
XI_API void __thiscall fn_10178F70(void * self_);  // 0x10178F70
XI_API int __thiscall ZoneMap_DrawBox(void *self_);  // 0x101791F0
XI_API int __thiscall ZoneMap_CreateVertexShaders(char *self_);  // 0x101792A0
XI_API void __thiscall ZoneMap_CopyField342C8ToField39450(void * self_);  // 0x10179410
XI_API undefined4 __thiscall fn_10179420(void * self_);  // 0x10179420
XI_API int __thiscall ZoneMap_DrawMeshGroupsWithFog(CTsZoneMap *self_, _DWORD *a2);  // 0x1017ADE0
XI_API undefined4 __thiscall fn_1017B110(void * self_, int param_1);  // 0x1017B110
XI_API undefined4 __thiscall ZoneMap_TeardownStep(void * self_);  // 0x1017B1E0
XI_API undefined4 __thiscall fn_101811D0(void * self_, undefined4 param_1, float param_2, float param_3);  // 0x101811D0
XI_API undefined4 __thiscall fn_10181210(void * self_, undefined4 param_1, float param_2, undefined4 param_3, float param_4, float * param_5);  // 0x10181210
XI_API undefined4 __thiscall fn_10181290(void * self_, undefined4 param_1, float param_2, undefined4 param_3, float param_4, float * param_5);  // 0x10181290
XI_API undefined4 __thiscall fn_10181300(void * self_, undefined4 param_1, float param_2, undefined4 param_3, float * param_4);  // 0x10181300
XI_API undefined4 __thiscall fn_10181370(void * self_, float param_1, float param_2, float param_3, float param_4, float * param_5);  // 0x10181370
XI_API void __thiscall ZoneMap_FindGroundHeight(void * self_, undefined4 param_1, float param_2, undefined4 param_3, float param_4, float * param_5);  // 0x101813F0
XI_API undefined4 __thiscall fn_101814D0(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x101814D0
XI_API undefined4 __thiscall fn_10181590(void * self_, undefined4 xi_s1, undefined4 xi_s2, undefined4 xi_s3);  // 0x10181590
XI_API undefined4 __thiscall fn_101819B0(void * self_, undefined4 param_1, float param_2, undefined4 param_3, float * param_4);  // 0x101819B0
XI_API undefined4 __thiscall fn_10181A10(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x10181A10
XI_API long double __cdecl fn_10181F60(float * param_1, float * param_2, float * param_3);  // 0x10181F60
XI_API void __stdcall fn_10182490(float * param_1, float param_2, float param_3, float param_4, float param_5, float param_6);  // 0x10182490
XI_API int __stdcall Gfx_DrawWireBox(int a1);  // 0x10182F10
XI_API int __thiscall fn_10183420(void * self_, int param_1);  // 0x10183420
XI_API _DWORD * __thiscall ZoneMap_DrawMeshById(int self_, int a2, int a3);  // 0x10183DA0
XI_API int __thiscall fn_10184C90(void *self_, int a1, int a2, int a3);  // 0x10184C90
XI_API int __thiscall fn_10185CA0(void *self_, float *a1, float *a2, IDirect3DBaseTexture8 *a3, float a4);  // 0x10185CA0
XI_API void __thiscall fn_10186DB0(void * self_);  // 0x10186DB0
XI_API void __stdcall fn_10186DD0(int * param_1);  // 0x10186DD0
XI_API int __thiscall fn_10186E90(void * self_);  // 0x10186E90
XI_API int __thiscall fn_10186EA0(void * self_);  // 0x10186EA0
XI_API int __thiscall fn_10186EB0(void * self_);  // 0x10186EB0
XI_API int __thiscall fn_10186EC0(void * self_);  // 0x10186EC0
XI_API int __thiscall fn_10186EE0(void * self_);  // 0x10186EE0
XI_API int __thiscall fn_10186F00(void * self_);  // 0x10186F00
XI_API int __thiscall fn_10186F10(void * self_);  // 0x10186F10
XI_API int __thiscall Env_GetColorPtr84(void * self_);  // 0x10186F20
XI_API int __thiscall fn_10186F30(void * self_);  // 0x10186F30
XI_API int __thiscall fn_10186F40(void * self_);  // 0x10186F40
XI_API int __thiscall fn_10186F50(void * self_);  // 0x10186F50
XI_API int __thiscall fn_10186F70(void * self_);  // 0x10186F70
XI_API int __thiscall fn_10186F80(void * self_);  // 0x10186F80
XI_API int __thiscall fn_10186F90(void * self_);  // 0x10186F90
XI_API int __thiscall Env_GetAmbientColorPtr(void * self_);  // 0x10186FA0
XI_API int __thiscall fn_10186FB0(void * self_);  // 0x10186FB0
XI_API int __thiscall fn_10186FC0(void * self_);  // 0x10186FC0
XI_API long double __thiscall fn_10186FD0(void * self_);  // 0x10186FD0
XI_API long double __thiscall fn_10186FE0(void * self_);  // 0x10186FE0
XI_API long double __thiscall fn_10186FF0(void * self_);  // 0x10186FF0
XI_API undefined4 __thiscall fn_10187000(void * self_);  // 0x10187000
XI_API long double __thiscall fn_10187020(void * self_);  // 0x10187020
XI_API void __thiscall fn_10187030(void * self_);  // 0x10187030
XI_API void __thiscall Env_ApplyRecord(void * self_);  // 0x10187050
XI_API void * __thiscall fn_10187380(void * self_);  // 0x10187380
XI_API void __thiscall fn_10187470(void * self_);  // 0x10187470
XI_API uint32_t __thiscall fn_101874A0(void * self_);  // 0x101874A0
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_101875C0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x101875C0
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_101875E0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x101875E0
XI_API void __thiscall fn_10187600(void * self_);  // 0x10187600
XI_API void __thiscall fn_10187720(void * self_, int param_1);  // 0x10187720
XI_API void __thiscall fn_101880C0(void * self_, undefined4 * param_1);  // 0x101880C0
XI_API void __thiscall fn_10188190(void * self_, uint8_t * param_1);  // 0x10188190
XI_API int __cdecl fn_10188900(void);  // 0x10188900
XI_API void __thiscall fn_10188910(void * self_);  // 0x10188910
XI_API void * __thiscall fn_10188970(void * self_);  // 0x10188970
XI_API int __cdecl fn_101889A0(void);  // 0x101889A0
XI_API void __thiscall fn_101889E0(void * self_);  // 0x101889E0
XI_API void __thiscall fn_10188A10(void * self_);  // 0x10188A10
XI_API uint32_t __thiscall Env_RampWindStrength(void * self_);  // 0x10188A50
XI_API uint32_t __thiscall fn_10188B10(void * self_);  // 0x10188B10
XI_API void __thiscall fn_10188CA0(void * self_, undefined4 * param_1, int param_2);  // 0x10188CA0
XI_API _DWORD * __thiscall Env_ComputeAmbient(char *self_, _DWORD *a2, int a3);  // 0x10188E00
XI_API float * __thiscall Env_ComputeAmbientFloat4(char *self_, float *a2);  // 0x10188FA0
XI_API void __thiscall Env_ComputeFog(void * self_, uint32_t * param_1, float * param_2, float * param_3, int param_4);  // 0x10189120
XI_API void * __thiscall fn_10189400(void * self_);  // 0x10189400
XI_API void __thiscall fn_10189490(void * self_, uint32_t * param_1, uint32_t * param_2, float param_3);  // 0x10189490
XI_API undefined4 __cdecl fn_10189810(float * param_1, float param_2, float param_3, float param_4);  // 0x10189810
XI_API void __thiscall fn_101898E0(void * self_, int param_1, int param_2, undefined4 param_3);  // 0x101898E0
XI_API undefined4 __stdcall fn_10189BD0(int param_1);  // 0x10189BD0
XI_API void __stdcall Zone_GetClock(int * param_1);  // 0x10189C60
XI_API void __thiscall fn_10189CA0(void * self_, undefined4 * param_1, undefined4 xi_s2);  // 0x10189CA0
XI_API void * __thiscall WeatherRes_FindAtOrBefore(void * self_, undefined4 xi_s1);  // 0x10189E30
XI_API void __thiscall fn_10189F20(void * self_, undefined4 * param_1, undefined4 xi_s2);  // 0x10189F20
XI_API void * __thiscall WeatherRes_FindNeighbor(void * self_, undefined4 xi_s1);  // 0x1018A0F0
XI_API void __thiscall fn_1018A220(void * self_);  // 0x1018A220
XI_API void __thiscall fn_1018A260(void * self_);  // 0x1018A260
XI_API void __thiscall fn_1018A2A0(void * self_);  // 0x1018A2A0
XI_API undefined4 __thiscall fn_1018A2C0(void * self_);  // 0x1018A2C0
XI_API void __thiscall fn_1018A2E0(void * self_, undefined4 param_1);  // 0x1018A2E0
XI_API void __thiscall fn_1018A2F0(void * self_, undefined4 param_1);  // 0x1018A2F0
XI_API void __thiscall fn_1018A300(void * self_, undefined4 param_1);  // 0x1018A300
XI_API undefined4 __thiscall fn_1018A310(void * self_, undefined4 param_1);  // 0x1018A310
XI_API undefined4 __thiscall fn_1018A320(void * self_);  // 0x1018A320
XI_API void __thiscall Env_BlendTimeOfDay(void * self_);  // 0x1018A340
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_1018AAC0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x1018AAC0
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_1018AB40(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x1018AB40
XI_API void __thiscall fn_1018ABF0(void * self_);  // 0x1018ABF0
XI_API undefined4 __thiscall fn_1018AD50(void * self_);  // 0x1018AD50
XI_API void __thiscall fn_1018AD60(void * self_, undefined4 param_1);  // 0x1018AD60
XI_API void * __thiscall fn_1018AD80(void * self_);  // 0x1018AD80
XI_API void __thiscall fn_1018ADA0(void * self_);  // 0x1018ADA0
XI_API void __thiscall fn_1018ADD0(void * self_);  // 0x1018ADD0
XI_API void __thiscall fn_1018ADE0(void * self_);  // 0x1018ADE0
XI_API undefined4 __stdcall fn_1018AEB0(undefined4 xi_s1, undefined4 xi_s2);  // 0x1018AEB0
XI_API void __thiscall fn_1018B5E0(void * self_, int param_1);  // 0x1018B5E0
XI_API void __thiscall fn_1018B620(void * self_);  // 0x1018B620
XI_API void __stdcall fn_1018B8B0(int param_1);  // 0x1018B8B0
XI_API long double __thiscall fn_1018B8F0(void * self_, undefined4 param_2);  // 0x1018B8F0
XI_API void __thiscall fn_1018B960(void * self_, undefined4 param_1, undefined4 param_2, int param_3);  // 0x1018B960
XI_API void __thiscall fn_1018BAE0(void * self_, uint32_t param_1);  // 0x1018BAE0
XI_API undefined4 __thiscall unknown_libname_158(void * self_);  // 0x1018BBB0
XI_API void * __thiscall fn_1018BBC0(void * self_, undefined4 param_1);  // 0x1018BBC0
XI_API undefined4 __attribute__((stdcall, regparm(3))) Time_Normalize(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, int param_2);  // 0x1018BC40
XI_API int __thiscall fn_1018BC90(void * self_);  // 0x1018BC90
XI_API int __thiscall fn_1018BCA0(void * self_);  // 0x1018BCA0
XI_API int __thiscall fn_1018BCD0(void * self_);  // 0x1018BCD0
XI_API int __thiscall fn_1018BD00(void * self_);  // 0x1018BD00
XI_API int __thiscall fn_1018BD30(void * self_);  // 0x1018BD30
XI_API int __cdecl fn_1018BDE0(void);  // 0x1018BDE0
XI_API void __thiscall fn_1018BE80(void * self_, int param_1);  // 0x1018BE80
XI_API void __thiscall fn_1018BEB0(void * self_, int param_1);  // 0x1018BEB0
XI_API void __thiscall fn_1018BF00(void * self_, int param_1);  // 0x1018BF00
XI_API void __thiscall Time_FromHours(void * self_, int param_1);  // 0x1018BF50
XI_API void __thiscall fn_1018BFA0(void * self_, int param_1);  // 0x1018BFA0
XI_API void __thiscall fn_1018C020(void * self_, int param_1);  // 0x1018C020
XI_API void __cdecl fn_1018C1B0(undefined4 * param_1);  // 0x1018C1B0
XI_API void __thiscall fn_1018C270(void * self_);  // 0x1018C270
XI_API undefined4 __thiscall fn_1018C290(void * self_);  // 0x1018C290
XI_API uint32_t __cdecl fn_1018D230(undefined4 param_1, uint32_t param_2);  // 0x1018D230
XI_API undefined4 __thiscall fn_1018D250(void * self_);  // 0x1018D250
XI_API void __thiscall fn_1018D280(void * self_);  // 0x1018D280
XI_API void * __thiscall fn_1018D2B0(void * self_);  // 0x1018D2B0
XI_API void * __thiscall fn_1018D2D0(void * self_, char * param_1);  // 0x1018D2D0
XI_API void __thiscall fn_1018D3D0(void * self_);  // 0x1018D3D0
XI_API void __thiscall fn_1018D430(void * self_, undefined4 param_1);  // 0x1018D430
XI_API void __thiscall fn_1018D440(void * self_, undefined4 param_1);  // 0x1018D440
XI_API void __thiscall fn_1018D450(void * self_, void * param_1);  // 0x1018D450
XI_API undefined1 * __thiscall fn_1018D470(void * self_);  // 0x1018D470
XI_API undefined4 __thiscall fn_1018D480(void * self_);  // 0x1018D480
XI_API undefined4 __thiscall fn_1018D490(void * self_);  // 0x1018D490
XI_API void * __thiscall fn_1018D4A0(void * self_, char * param_1, int param_2);  // 0x1018D4A0
XI_API void * __thiscall fn_1018D580(void * self_, char * param_1, size_t param_2);  // 0x1018D580
XI_API void * __thiscall fn_1018D640(void * self_);  // 0x1018D640
XI_API int __thiscall fn_1018D7F0(void * self_, undefined4 param_1, int param_2, uint8_t * param_3, int param_4);  // 0x1018D7F0
XI_API undefined4 __thiscall fn_1018D980(void * self_);  // 0x1018D980
XI_API undefined4 __stdcall fn_1018DB10(int * param_1, int param_2, int param_3, char * param_4);  // 0x1018DB10
XI_API int __thiscall fn_1018DC70(void * self_, uint32_t param_1);  // 0x1018DC70
XI_API void __thiscall fn_1018DCB0(void * self_);  // 0x1018DCB0
XI_API void * __thiscall fn_1018DCD0(void * self_);  // 0x1018DCD0
XI_API undefined4 __thiscall fn_1018E0A0(void * self_, undefined4 param_1);  // 0x1018E0A0
XI_API void __thiscall fn_1018E0E0(void * self_);  // 0x1018E0E0
XI_API undefined4 __thiscall fn_1018E1A0(void * self_);  // 0x1018E1A0
XI_API undefined4 __thiscall FsLogPage_load(void * self_, undefined4 param_1);  // 0x1018E1D0
XI_API undefined4 __thiscall fn_1018E310(void * self_, undefined4 param_1);  // 0x1018E310
XI_API undefined4 __thiscall fn_1018E4D0(void * self_, char * param_1);  // 0x1018E4D0
XI_API void __thiscall fn_1018E660(void * self_);  // 0x1018E660
XI_API void __thiscall fn_1018E6A0(void * self_);  // 0x1018E6A0
XI_API void * __thiscall fn_1018E6F0(void * self_, undefined4 param_1);  // 0x1018E6F0
XI_API undefined4 __thiscall fn_1018E7F0(void * self_, int param_1);  // 0x1018E7F0
XI_API int __thiscall fn_1018E800(void * self_, int param_1);  // 0x1018E800
XI_API undefined4 __thiscall fn_1018E8D0(void * self_, void * param_1);  // 0x1018E8D0
XI_API void * __thiscall fn_1018E9B0(void * self_);  // 0x1018E9B0
XI_API void * __thiscall fn_1018EA00(void * self_);  // 0x1018EA00
XI_API void __thiscall fn_1018EA50(void * self_, undefined4 param_1);  // 0x1018EA50
XI_API undefined4 __thiscall fn_1018EB00(void * self_);  // 0x1018EB00
XI_API void __thiscall fn_1018ED80(void * self_, undefined4 param_1);  // 0x1018ED80
XI_API void * __thiscall fn_1018EE40(void * self_, undefined4 param_1, undefined4 param_2);  // 0x1018EE40
XI_API undefined4 __cdecl fn_1018FA20(undefined4 param_1, undefined4 param_2);  // 0x1018FA20
XI_API undefined4 __cdecl fn_1018FA80(undefined4 param_1, undefined4 param_2);  // 0x1018FA80
XI_API int __cdecl fn_1018FB00(int param_1, undefined4 param_2, int param_3);  // 0x1018FB00
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_1018FD90(undefined4 xi_eax, int param_1, int param_2);  // 0x1018FD90
XI_API int __cdecl fn_10190140(char * param_1, int param_2, int param_3);  // 0x10190140
XI_API void __cdecl fn_101907A0(undefined4 * param_1, uint32_t param_2);  // 0x101907A0
XI_API undefined8 __cdecl fn_101907D0(LPVOID param_1);  // 0x101907D0
XI_API void __cdecl fn_101908D0(void);  // 0x101908D0
XI_API undefined4 __thiscall fn_10190900(void * self_);  // 0x10190900
XI_API undefined4 __thiscall fn_10190950(void * self_, undefined4 param_1);  // 0x10190950
XI_API void __thiscall fn_101911A0(void * self_);  // 0x101911A0
XI_API void * __thiscall fn_10191AB0(void * self_);  // 0x10191AB0
XI_API int __thiscall fn_10191B20(void * self_, undefined4 param_1);  // 0x10191B20
XI_API int __thiscall fn_10192380(void * self_, undefined4 param_1);  // 0x10192380
XI_API void * __thiscall fn_10192E50(void * self_, undefined4 param_1);  // 0x10192E50
XI_API undefined4 __cdecl fn_10193800(undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x10193800
XI_API undefined4 __cdecl SetConfig(undefined4 param_1, undefined4 param_2);  // 0x10193830
XI_API undefined4 __cdecl Settings_GetValue(undefined4 param_1);  // 0x10193850
XI_API undefined4 __cdecl fn_10193890(undefined4 param_1);  // 0x10193890
XI_API undefined4 __cdecl fn_101938B0(undefined4 param_1, undefined4 xi_s2);  // 0x101938B0
XI_API undefined8 __cdecl fn_10193A10(int param_1, int param_2);  // 0x10193A10
XI_API undefined4 __thiscall fn_10193B00(void * self_, undefined4 param_1, undefined4 param_2);  // 0x10193B00
XI_API void __thiscall fn_10193B60(void * self_, int param_1, int param_2);  // 0x10193B60
XI_API void __thiscall fn_10193BC0(void * self_, undefined4 param_1, undefined4 param_2);  // 0x10193BC0
XI_API void * __thiscall FsConfig_Constructor(void * self_);  // 0x10193BF0
XI_API undefined4 __thiscall fn_10195220(void * self_);  // 0x10195220
XI_API undefined4 __thiscall fn_10195240(void * self_, undefined4 param_1, int param_2, undefined4 param_3);  // 0x10195240
XI_API undefined4 __thiscall fn_101952C0(void * self_, int param_1, int param_2);  // 0x101952C0
XI_API undefined8 __thiscall fn_10195330(void * self_, undefined4 param_1);  // 0x10195330
XI_API undefined4 __thiscall fn_10195350(void * self_);  // 0x10195350
XI_API void __thiscall fn_101953A0(void * self_);  // 0x101953A0
XI_API undefined4 __thiscall fn_101953C0(void * self_);  // 0x101953C0
XI_API undefined4 __thiscall fn_101953F0(void * self_);  // 0x101953F0
XI_API void __thiscall fn_10195460(void * self_, int param_1, undefined4 param_2, int param_3, undefined4 * param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7, undefined4 param_8);  // 0x10195460
XI_API int __thiscall fn_101954D0(void * self_, int param_1);  // 0x101954D0
XI_API undefined4 __thiscall fn_101954F0(void * self_);  // 0x101954F0
XI_API undefined4 __thiscall fn_10195570(void * self_);  // 0x10195570
XI_API undefined4 __thiscall fn_10195640(void * self_);  // 0x10195640
XI_API undefined4 __thiscall fn_101956F0(void * self_);  // 0x101956F0
XI_API undefined4 __stdcall fn_10195780(undefined4 * param_1);  // 0x10195780
XI_API undefined4 __stdcall fn_101957B0(undefined4 * param_1, undefined4 xi_s2);  // 0x101957B0
XI_API undefined * __stdcall fn_10195810(char * param_1);  // 0x10195810
XI_API int __stdcall fn_101958C0(char * param_1, undefined4 xi_s2);  // 0x101958C0
XI_API undefined4 __thiscall fn_10195900(void * self_);  // 0x10195900
XI_API undefined4 __stdcall fn_10196040(int param_1, int param_2);  // 0x10196040
XI_API void __thiscall fn_10196260(void * self_, undefined4 param_1);  // 0x10196260
XI_API void __thiscall fn_101962D0(void * self_);  // 0x101962D0
XI_API void * __thiscall fn_10196980(void * self_);  // 0x10196980
XI_API undefined4 __cdecl fn_10196BE0(void);  // 0x10196BE0
XI_API void * __thiscall fn_10196BF0(void * self_);  // 0x10196BF0
XI_API void * __thiscall fn_101972B0(void * self_);  // 0x101972B0
XI_API void * __thiscall fn_10197A90(void * self_);  // 0x10197A90
XI_API void * __thiscall fn_10197F00(void * self_);  // 0x10197F00
XI_API void * __thiscall fn_10198320(void * self_);  // 0x10198320
XI_API void * __thiscall fn_10198CA0(void * self_);  // 0x10198CA0
XI_API void * __thiscall fn_101990C0(void * self_);  // 0x101990C0
XI_API void * __thiscall fn_101991B0(void * self_);  // 0x101991B0
XI_API void * __thiscall fn_101992A0(void * self_);  // 0x101992A0
XI_API void * __thiscall fn_10199710(void * self_);  // 0x10199710
XI_API void * __thiscall fn_10199F40(void * self_);  // 0x10199F40
XI_API void * __thiscall fn_1019A010(void * self_);  // 0x1019A010
XI_API void * __thiscall fn_1019A890(void * self_);  // 0x1019A890
XI_API void * __thiscall fn_1019AF00(void * self_);  // 0x1019AF00
XI_API void * __thiscall fn_1019B860(void * self_);  // 0x1019B860
XI_API void * __thiscall fn_1019BC90(void * self_);  // 0x1019BC90
XI_API void * __thiscall fn_1019BFE0(void * self_);  // 0x1019BFE0
XI_API void * __thiscall fn_1019C190(void * self_);  // 0x1019C190
XI_API void __cdecl fn_1019C3E0(void);  // 0x1019C3E0
XI_API void __cdecl fn_1019C4F0(void);  // 0x1019C4F0
XI_API undefined4 __cdecl fn_1019C580(void);  // 0x1019C580
XI_API undefined4 __cdecl fn_1019C590(void);  // 0x1019C590
XI_API void __cdecl fn_1019C5B0(void);  // 0x1019C5B0
XI_API void * __thiscall fn_1019CD40(void * self_);  // 0x1019CD40
XI_API void * __thiscall fn_1019CE00(void * self_);  // 0x1019CE00
XI_API void __thiscall fn_1019CE70(void * self_);  // 0x1019CE70
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_1019CF60(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x1019CF60
XI_API void * __thiscall fn_1019D150(void * self_);  // 0x1019D150
XI_API void __thiscall fn_1019D1C0(void * self_);  // 0x1019D1C0
XI_API void * __thiscall fn_1019D580(void * self_);  // 0x1019D580
XI_API void * __thiscall fn_1019D770(void * self_);  // 0x1019D770
XI_API void __thiscall fn_1019D8B0(void * self_);  // 0x1019D8B0
XI_API char * __stdcall fn_1019ED70(char * param_1);  // 0x1019ED70
XI_API void * __thiscall fn_1019F160(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x1019F160
XI_API int __thiscall fn_1019F8B0(void * self_, undefined4 param_1);  // 0x1019F8B0
XI_API undefined4 __thiscall fn_1019F930(void * self_, undefined4 xi_s1);  // 0x1019F930
XI_API undefined8 __thiscall fn_1019F960(void * self_, int param_1);  // 0x1019F960
XI_API int __thiscall fn_1019FA90(void * self_);  // 0x1019FA90
XI_API int __thiscall fn_1019FAB0(void * self_);  // 0x1019FAB0
XI_API undefined4 __cdecl fn_101A0710(undefined4 param_1, int param_2, undefined4 xi_s3);  // 0x101A0710
XI_API undefined4 __cdecl fn_101A0730(undefined4 xi_s1, undefined4 xi_s2, undefined4 xi_s3, undefined4 xi_s4, undefined4 xi_s5, undefined4 xi_s6, undefined4 xi_s7, undefined4 xi_s8, undefined4 xi_s9, undefined4 xi_s10, undefined4 xi_s11, undefined4 xi_s12, undefined4 xi_s13, undefined4 xi_s14, undefined4 xi_s15, undefined4 xi_s16, undefined4 xi_s17, undefined4 xi_s18, undefined4 xi_s19, undefined4 xi_s20, undefined4 xi_s21, undefined4 xi_s22, undefined4 xi_s23, undefined4 xi_s24, undefined4 xi_s25, undefined4 xi_s26, undefined4 xi_s27, undefined4 xi_s28, undefined4 xi_s29, undefined4 xi_s30, undefined4 xi_s31, undefined4 xi_s32, undefined4 xi_s33, undefined4 xi_s34, undefined4 xi_s35, undefined4 xi_s36, undefined4 xi_s37, undefined4 xi_s38, undefined4 xi_s39, undefined4 xi_s40, undefined4 xi_s41, undefined4 xi_s42, undefined4 xi_s43, undefined4 xi_s44, undefined4 xi_s45, undefined4 xi_s46, undefined4 xi_s47, undefined4 xi_s48, undefined4 xi_s49, undefined4 xi_s50, undefined4 xi_s51, undefined4 xi_s52, undefined4 xi_s53, undefined4 xi_s54, undefined4 xi_s55, undefined4 xi_s56, undefined4 xi_s57, undefined4 xi_s58, undefined4 xi_s59, undefined4 xi_s60, undefined4 xi_s61, undefined4 xi_s62, undefined4 xi_s63, undefined4 xi_s64, undefined4 xi_s65, undefined4 xi_s66, undefined4 xi_s67, undefined4 xi_s68);  // 0x101A0730
XI_API undefined4 __cdecl fn_101A0750(undefined4 xi_s1, undefined4 xi_s2, undefined4 xi_s3);  // 0x101A0750
XI_API undefined4 __cdecl fn_101A0760(undefined4 xi_s1, undefined4 xi_s2, undefined4 xi_s3);  // 0x101A0760
XI_API int __thiscall fn_101A0910(void * self_);  // 0x101A0910
XI_API int __thiscall fn_101A0930(void * self_, undefined4 param_1);  // 0x101A0930
XI_API undefined8 __thiscall fn_101A0950(void * self_);  // 0x101A0950
XI_API undefined8 __thiscall fn_101A0970(void * self_);  // 0x101A0970
XI_API undefined4 __thiscall fn_101A0BE0(void * self_, int param_1);  // 0x101A0BE0
XI_API undefined4 __thiscall fn_101A0D30(void * self_, int param_1);  // 0x101A0D30
XI_API undefined4 __thiscall fn_101A0F30(void * self_);  // 0x101A0F30
XI_API unsigned char __thiscall fn_101A1360(void * self_);  // 0x101A1360
XI_API undefined4 __thiscall fn_101A1390(void * self_);  // 0x101A1390
XI_API void * __thiscall fn_101A1400(void * self_);  // 0x101A1400
XI_API void __thiscall fn_101A1460(void * self_);  // 0x101A1460
XI_API undefined4 __thiscall fn_101A1470(void * self_, undefined4 * param_1, uint32_t param_2);  // 0x101A1470
XI_API int __thiscall fn_101A1500(void * self_, undefined4 * param_1, int param_2);  // 0x101A1500
XI_API undefined4 __thiscall fn_101A1580(void * self_, char * param_1, int param_2);  // 0x101A1580
XI_API undefined4 __thiscall fn_101A1660(void * self_);  // 0x101A1660
XI_API undefined4 __thiscall fn_101A1690(void * self_);  // 0x101A1690
XI_API undefined4 __thiscall fn_101A16B0(void * self_);  // 0x101A16B0
XI_API void * __thiscall fn_101A16E0(void * self_);  // 0x101A16E0
XI_API void __thiscall fn_101A1740(void * self_, int param_1);  // 0x101A1740
XI_API int __thiscall fn_101A1910(void * self_, int param_1);  // 0x101A1910
XI_API int __cdecl fn_101A19B0(void);  // 0x101A19B0
XI_API void * __thiscall fn_101A1A20(void * self_);  // 0x101A1A20
XI_API undefined4 __thiscall fn_101A1C60(void * self_, int param_1);  // 0x101A1C60
XI_API void __thiscall fn_101A1CB0(void * self_);  // 0x101A1CB0
XI_API undefined4 __thiscall fn_101A1CC0(void * self_);  // 0x101A1CC0
XI_API undefined4 __thiscall fn_101A2890(void * self_, int param_1);  // 0x101A2890
XI_API int __thiscall fn_101A29F0(void * self_, int param_1);  // 0x101A29F0
XI_API undefined4 __thiscall fn_101A2E00(void * self_, undefined4 param_1);  // 0x101A2E00
XI_API void __thiscall fn_101A2E60(void * self_, undefined4 param_1, undefined4 param_2);  // 0x101A2E60
XI_API void __thiscall fn_101A2E80(void * self_);  // 0x101A2E80
XI_API undefined4 __thiscall fn_101A3300(void * self_, undefined4 param_1, undefined4 param_2);  // 0x101A3300
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_101A33E0(undefined4 xi_eax);  // 0x101A33E0
XI_API undefined4 __thiscall fn_101A3500(void * self_, int param_1, undefined4 param_2, undefined4 xi_s3);  // 0x101A3500
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_101A3550(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x101A3550
XI_API void * __thiscall fn_101A36F0(void * self_, undefined4 param_1, undefined4 param_2);  // 0x101A36F0
XI_API void __thiscall fn_101A3CF0(void * self_);  // 0x101A3CF0
XI_API void * __thiscall fn_101A3DC0(void * self_, undefined4 param_1, undefined4 param_2);  // 0x101A3DC0
XI_API void __thiscall fn_101A43A0(void * self_);  // 0x101A43A0
XI_API undefined4 __thiscall fn_101A4440(void * self_, undefined4 param_1);  // 0x101A4440
XI_API void * __thiscall fn_101A46C0(void * self_, undefined4 param_1, undefined4 param_2);  // 0x101A46C0
XI_API void * __thiscall fn_101A4D70(void * self_, undefined4 param_1, undefined4 param_2);  // 0x101A4D70
XI_API void * __thiscall fn_101A54F0(void * self_, undefined4 param_1, undefined4 param_2);  // 0x101A54F0
XI_API void * __thiscall fn_101A6430(void * self_);  // 0x101A6430
XI_API void * __thiscall fn_101A6C10(void * self_, undefined4 param_1);  // 0x101A6C10
XI_API void * __thiscall fn_101A7280(void * self_, undefined4 param_1);  // 0x101A7280
XI_API void * __thiscall fn_101A83C0(void * self_);  // 0x101A83C0
XI_API void __thiscall fn_101A8410(void * self_, int param_1);  // 0x101A8410
XI_API undefined4 __thiscall fn_101A8750(void * self_, undefined4 param_1);  // 0x101A8750
XI_API void __cdecl fn_101A8B50(void);  // 0x101A8B50
XI_API void __cdecl fn_101A8D40(void);  // 0x101A8D40
XI_API void __cdecl fn_101AA300(void);  // 0x101AA300
XI_API void __cdecl fn_101AB080(void);  // 0x101AB080
XI_API void __cdecl fn_101AB0D0(void);  // 0x101AB0D0
XI_API void __cdecl fn_101AB6D0(void);  // 0x101AB6D0
XI_API void __cdecl fn_101AB720(void);  // 0x101AB720
XI_API void __cdecl fn_101AB730(void);  // 0x101AB730
XI_API void __attribute__((cdecl, regparm(1))) fn_101AB770(undefined4 xi_eax);  // 0x101AB770
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_101AB7D0(undefined4 xi_eax);  // 0x101AB7D0
XI_API void * __thiscall fn_101AB810(void * self_);  // 0x101AB810
XI_API void __thiscall fn_101AB820(void * self_);  // 0x101AB820
XI_API void __thiscall fn_101ABB40(void * self_, undefined4 param_1);  // 0x101ABB40
XI_API undefined4 __thiscall fn_101ABC50(void * self_, int param_1, int param_2);  // 0x101ABC50
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_101ABE40(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x101ABE40
XI_API void __thiscall fn_101ABE50(void * self_);  // 0x101ABE50
XI_API undefined4 __thiscall fn_101ABE80(void * self_, undefined4 * param_1);  // 0x101ABE80
XI_API void __thiscall fn_101ABEF0(void * self_);  // 0x101ABEF0
XI_API void __thiscall fn_101ABF40(void * self_, undefined4 param_1);  // 0x101ABF40
XI_API int __thiscall fn_101AC1B0(void * self_);  // 0x101AC1B0
XI_API int __thiscall fn_101AC1D0(void * self_);  // 0x101AC1D0
XI_API undefined4 __thiscall fn_101AC200(void * self_);  // 0x101AC200
XI_API undefined4 __thiscall fn_101AC210(void * self_);  // 0x101AC210
XI_API int __thiscall fn_101AC230(void * self_);  // 0x101AC230
XI_API void __thiscall fn_101AC250(void * self_, undefined4 xi_s1);  // 0x101AC250
XI_API undefined4 __thiscall fn_101AC2C0(void * self_);  // 0x101AC2C0
XI_API undefined4 __thiscall fn_101AC3D0(void * self_);  // 0x101AC3D0
XI_API undefined4 __thiscall fn_101AC4D0(void * self_);  // 0x101AC4D0
XI_API void __thiscall fn_101AC4E0(void * self_);  // 0x101AC4E0
XI_API void __thiscall fn_101AC520(void * self_);  // 0x101AC520
XI_API undefined4 __thiscall fn_101AC580(void * self_);  // 0x101AC580
XI_API uint32_t __thiscall fn_101AC5E0(void * self_, int param_1, int param_2, undefined4 * param_3, undefined4 param_4, int * param_5, undefined4 param_6, undefined4 param_7);  // 0x101AC5E0
XI_API int __thiscall fn_101AC990(void * self_, int param_1, int param_2, char * param_3, int param_4, undefined4 param_5, undefined4 param_6);  // 0x101AC990
XI_API int __thiscall fn_101ACAC0(void * self_, char * param_1, int param_2, int param_3, int param_4, int * param_5);  // 0x101ACAC0
XI_API undefined4 __thiscall fn_101ACBE0(void * self_, char * param_1);  // 0x101ACBE0
XI_API uint32_t __thiscall fn_101ACC30(void * self_, int * param_1);  // 0x101ACC30
XI_API void __thiscall fn_101ACF40(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x101ACF40
XI_API undefined4 __thiscall fn_101ACF60(void * self_);  // 0x101ACF60
XI_API void __thiscall fn_101ACF70(void * self_, undefined1 * param_1);  // 0x101ACF70
XI_API undefined4 __thiscall fn_101ACF90(void * self_);  // 0x101ACF90
XI_API undefined4 __thiscall fn_101ACFB0(void * self_);  // 0x101ACFB0
XI_API int __thiscall fn_101AD0D0(void * self_);  // 0x101AD0D0
XI_API undefined4 __stdcall fn_101AD1B0(int param_1, int param_2);  // 0x101AD1B0
XI_API void * __thiscall FsPreParser_Constructor(void * self_, undefined4 param_1);  // 0x101AD290
XI_API void __thiscall fn_101AD4B0(void * self_, char * param_1, int param_2, size_t param_3);  // 0x101AD4B0
XI_API int __thiscall fn_101AD820(void * self_);  // 0x101AD820
XI_API undefined4 __stdcall fn_101ADBC0(char * param_1);  // 0x101ADBC0
XI_API undefined4 __thiscall fn_101ADBD0(void * self_, undefined4 xi_s1);  // 0x101ADBD0
XI_API undefined4 __stdcall fn_101ADBE0(char * param_1);  // 0x101ADBE0
XI_API undefined4 __cdecl fn_101ADDC0(int param_1, int param_2);  // 0x101ADDC0
XI_API void __thiscall fn_101ADE00(void * self_);  // 0x101ADE00
XI_API void __thiscall fn_101ADE80(void * self_, undefined4 param_1, undefined4 param_2, char * param_3, undefined4 param_4);  // 0x101ADE80
XI_API void __thiscall fn_101ADEC0(void * self_, undefined4 param_1, undefined4 param_2, char * param_3, char * param_4, undefined4 param_5);  // 0x101ADEC0
XI_API undefined4 __thiscall fn_101ADF20(void * self_, undefined * param_1);  // 0x101ADF20
XI_API void __thiscall fn_101AE3E0(void * self_);  // 0x101AE3E0
XI_API void * __thiscall fn_101AEA40(void * self_);  // 0x101AEA40
XI_API undefined4 __thiscall fn_101AEAC0(void * self_, undefined4 param_1);  // 0x101AEAC0
XI_API void __cdecl fn_101AFF30(void);  // 0x101AFF30
XI_API void __cdecl fn_101AFF40(void);  // 0x101AFF40
XI_API void __thiscall fn_101B2710(void * self_, int param_1, undefined4 xi_s2);  // 0x101B2710
XI_API void __stdcall fn_101B27F0(undefined4 xi_s1, undefined4 xi_s2);  // 0x101B27F0
XI_API void __stdcall fn_101B2800(undefined4 xi_s1, undefined4 xi_s2);  // 0x101B2800
XI_API void __thiscall fn_101B36F0(void * self_, uint32_t param_1);  // 0x101B36F0
XI_API void __thiscall fn_101B3B50(void * self_, undefined4 param_1);  // 0x101B3B50
XI_API undefined4 __cdecl fn_101B3F70(void);  // 0x101B3F70
XI_API undefined4 __stdcall fn_101B4260(int param_1);  // 0x101B4260
XI_API undefined4 __cdecl fn_101B4350(void);  // 0x101B4350
XI_API uint32_t __cdecl FsSjisToAscii(int param_1, undefined1 * param_2);  // 0x101B4450
XI_API void __thiscall fn_101B4570(void * self_);  // 0x101B4570
XI_API void __thiscall fn_101B45B0(void * self_);  // 0x101B45B0
XI_API void __thiscall fn_101B45E0(void * self_);  // 0x101B45E0
XI_API void * __thiscall fn_101B4710(void * self_);  // 0x101B4710
XI_API void * __thiscall fn_101B4BB0(void * self_);  // 0x101B4BB0
XI_API undefined4 __thiscall fn_101B4C20(void * self_, undefined4 param_1);  // 0x101B4C20
XI_API undefined4 __thiscall fn_101B4D30(void * self_);  // 0x101B4D30
XI_API undefined4 __thiscall fn_101B4E70(void * self_);  // 0x101B4E70
XI_API void __thiscall fn_101B4F90(void * self_);  // 0x101B4F90
XI_API void __thiscall fn_101B4FC0(void * self_);  // 0x101B4FC0
XI_API void __thiscall fn_101B5020(void * self_);  // 0x101B5020
XI_API undefined4 __thiscall FsTextInput_init(void * self_);  // 0x101B5210
XI_API uint32_t __thiscall FsTextInput_copyDecidedString(void * self_, undefined4 * param_1, undefined4 xi_s2);  // 0x101B5BC0
XI_API undefined4 __thiscall fn_101B5C80(void * self_);  // 0x101B5C80
XI_API undefined4 __thiscall fn_101B5C90(void * self_);  // 0x101B5C90
XI_API undefined4 __thiscall fn_101B5CC0(void * self_);  // 0x101B5CC0
XI_API undefined4 __thiscall fn_101B6C00(void * self_, undefined1 * param_1, int param_2);  // 0x101B6C00
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_101B6D50(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, undefined4 param_2, int param_3);  // 0x101B6D50
XI_API void __attribute__((stdcall, regparm(3))) fn_101B6E40(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, undefined4 param_2, int param_3, undefined4 param_4);  // 0x101B6E40
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_101B6E90(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x101B6E90
XI_API undefined4 __thiscall fn_101B6EA0(void * self_);  // 0x101B6EA0
XI_API void * __thiscall fn_101B6EC0(void * self_);  // 0x101B6EC0
XI_API undefined4 __thiscall fn_101B6EE0(void * self_, uint8_t * param_1, int param_2);  // 0x101B6EE0
XI_API void __thiscall fn_101B70A0(void * self_);  // 0x101B70A0
XI_API void __thiscall fn_101B70D0(void * self_);  // 0x101B70D0
XI_API void __thiscall fn_101B7100(void * self_);  // 0x101B7100
XI_API void __thiscall fn_101B7130(void * self_);  // 0x101B7130
XI_API void __thiscall fn_101B7190(void * self_);  // 0x101B7190
XI_API void __thiscall fn_101B7260(void * self_, undefined4 xi_s1, undefined4 xi_s2, undefined4 xi_s3, undefined4 xi_s4, undefined4 xi_s5, undefined4 xi_s6);  // 0x101B7260
XI_API undefined4 __thiscall fn_101B7280(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6);  // 0x101B7280
XI_API undefined4 __thiscall fn_101B72B0(void * self_, char * param_1, int param_2, int param_3, int param_4);  // 0x101B72B0
XI_API int __thiscall fn_101B7C20(void * self_, undefined4 * param_1);  // 0x101B7C20
XI_API uint32_t __cdecl fn_101B7EE0(void);  // 0x101B7EE0
XI_API void * __thiscall fn_101B9230(void * self_);  // 0x101B9230
XI_API void __attribute__((cdecl, regparm(1))) fn_101B92B0(undefined4 xi_eax);  // 0x101B92B0
XI_API undefined8 __thiscall fn_101B9550(void * self_, DWORD param_1);  // 0x101B9550
XI_API int __thiscall fn_101B95D0(void * self_, DWORD param_1);  // 0x101B95D0
XI_API int __thiscall fn_101B9610(void * self_);  // 0x101B9610
XI_API undefined4 __thiscall fn_101B9650(void * self_);  // 0x101B9650
XI_API int __thiscall FsTinyString_init(void * self_);  // 0x101B9690
XI_API void __thiscall fn_101B9880(void * self_, undefined4 param_1, char * param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7);  // 0x101B9880
XI_API void __thiscall fn_101B9910(void * self_, undefined4 param_1, char * param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5);  // 0x101B9910
XI_API void __thiscall fn_101B9980(void * self_, int param_1);  // 0x101B9980
XI_API void __thiscall fn_101B9A00(void * self_);  // 0x101B9A00
XI_API undefined4 __thiscall fn_101B9A10(void * self_);  // 0x101B9A10
XI_API undefined4 __thiscall fn_101B9BC0(void * self_, undefined4 param_1);  // 0x101B9BC0
XI_API void * __thiscall fn_101B9E00(void * self_, int param_1);  // 0x101B9E00
XI_API int __cdecl fn_101B9E60(void);  // 0x101B9E60
XI_API undefined4 __thiscall fn_101B9EC0(void * self_, undefined4 param_1);  // 0x101B9EC0
XI_API void __thiscall fn_101B9F20(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7, undefined4 param_8);  // 0x101B9F20
XI_API undefined4 __thiscall fn_101B9F90(void * self_, int param_1);  // 0x101B9F90
XI_API undefined4 __thiscall fn_101B9FF0(void * self_);  // 0x101B9FF0
XI_API undefined4 __thiscall fn_101BA030(void * self_);  // 0x101BA030
XI_API undefined4 __thiscall fn_101BA180(void * self_);  // 0x101BA180
XI_API void * __thiscall fn_101BA280(void * self_);  // 0x101BA280
XI_API void __thiscall fn_101BA2D0(void * self_);  // 0x101BA2D0
XI_API void __thiscall fn_101BA300(void * self_);  // 0x101BA300
XI_API void * __thiscall fn_101BA570(void * self_);  // 0x101BA570
XI_API void __thiscall fn_101BA5B0(void * self_);  // 0x101BA5B0
XI_API undefined4 __thiscall fn_101BA5C0(void * self_);  // 0x101BA5C0
XI_API undefined4 __thiscall fn_101BAE20(void * self_);  // 0x101BAE20
XI_API undefined4 __thiscall fn_101BAE30(void * self_);  // 0x101BAE30
XI_API int __thiscall fn_101BAE40(void * self_, undefined4 param_1);  // 0x101BAE40
XI_API undefined4 __thiscall fn_101BAEB0(void * self_);  // 0x101BAEB0
XI_API undefined4 __thiscall fn_101BAEC0(void * self_);  // 0x101BAEC0
XI_API void * __thiscall fn_101BAED0(void * self_);  // 0x101BAED0
XI_API void __thiscall fn_101BAF90(void * self_);  // 0x101BAF90
XI_API void __thiscall fn_101BAFC0(void * self_, undefined4 param_1);  // 0x101BAFC0
XI_API uint32_t __cdecl fn_101BB100(uint32_t * param_1);  // 0x101BB100
XI_API void __stdcall fn_101BB130(int param_1);  // 0x101BB130
XI_API void __thiscall fn_101BB1A0(void * self_, int param_1, undefined4 xi_s2);  // 0x101BB1A0
XI_API void __stdcall fn_101BB430(int param_1);  // 0x101BB430
XI_API void __thiscall fn_101BB4B0(void * self_, int param_1, int param_2, undefined4 param_3);  // 0x101BB4B0
XI_API undefined4 __thiscall fn_101BB5A0(void * self_, undefined4 * param_1, int * param_2, undefined4 param_3);  // 0x101BB5A0
XI_API uint32_t __thiscall fn_101BB740(void * self_);  // 0x101BB740
XI_API undefined4 __thiscall fn_101BB7A0(void * self_);  // 0x101BB7A0
XI_API undefined1 __thiscall fn_101BB9E0(void * self_);  // 0x101BB9E0
XI_API undefined4 __thiscall fn_101BBA90(void * self_, char * param_1, uint32_t param_2, undefined4 param_3);  // 0x101BBA90
XI_API void __thiscall fn_101BBC60(void * self_);  // 0x101BBC60
XI_API undefined4 __thiscall fn_101BBE50(void * self_);  // 0x101BBE50
XI_API undefined4 __thiscall FsWordCompletion_getEntryList(void * self_, undefined4 * param_1, int * param_2);  // 0x101BBE60
XI_API undefined4 __thiscall FsWordCompletion__prepareAllData(void * self_, undefined4 xi_s1);  // 0x101BC520
XI_API undefined4 __thiscall fn_101BC6F0(void * self_);  // 0x101BC6F0
XI_API undefined4 __thiscall fn_101BC720(void * self_);  // 0x101BC720
XI_API void __thiscall fn_101BC740(void * self_);  // 0x101BC740
XI_API uint32_t __thiscall fn_101BC7B0(void * self_, undefined4 param_1, uint32_t param_2, undefined4 param_3);  // 0x101BC7B0
XI_API int __thiscall fn_101BC900(void * self_, int * param_1);  // 0x101BC900
XI_API undefined4 __thiscall fn_101BC9B0(void * self_);  // 0x101BC9B0
XI_API void * __thiscall FsMemory_Constructor(void * self_, uint32_t param_1);  // 0x101BC9E0
XI_API undefined4 * __thiscall fn_101BCA40(void * self_, undefined4 * param_1, uint32_t param_2);  // 0x101BCA40
XI_API int __thiscall fn_101BCAC0(void * self_, uint32_t param_1);  // 0x101BCAC0
XI_API void __thiscall fn_101BCB30(void * self_);  // 0x101BCB30
XI_API undefined4 __thiscall fn_101BCBA0(void * self_);  // 0x101BCBA0
XI_API void __thiscall fn_101BCE30(void * self_);  // 0x101BCE30
XI_API void * __thiscall fn_101BCEF0(void * self_);  // 0x101BCEF0
XI_API void __thiscall fn_101BCF30(void * self_);  // 0x101BCF30
XI_API void * __thiscall fn_101BE260(void * self_);  // 0x101BE260
XI_API void __thiscall fn_101BE2A0(void * self_);  // 0x101BE2A0
XI_API void __thiscall fn_101BE2B0(void * self_, undefined4 param_1);  // 0x101BE2B0
XI_API void __thiscall fn_101BE2C0(void * self_, undefined4 xi_s1);  // 0x101BE2C0
XI_API void __thiscall fn_101BE330(void * self_, undefined4 xi_s1);  // 0x101BE330
XI_API void __thiscall fn_101BE3A0(void * self_, undefined4 xi_s1);  // 0x101BE3A0
XI_API void __thiscall fn_101BE410(void * self_);  // 0x101BE410
XI_API undefined4 __thiscall fn_101BE470(void * self_, int * param_1, int param_2, int param_3);  // 0x101BE470
XI_API undefined4 __stdcall fn_101BE500(undefined4 param_1);  // 0x101BE500
XI_API int __stdcall fn_101BE540(undefined4 param_1, undefined4 param_2, int param_3);  // 0x101BE540
XI_API int __stdcall fn_101BE590(int param_1, int param_2, uint32_t param_3);  // 0x101BE590
XI_API undefined4 __stdcall fn_101BE650(undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x101BE650
XI_API undefined4 __stdcall fn_101BE6C0(undefined4 param_1, undefined4 * param_2);  // 0x101BE6C0
XI_API undefined4 __stdcall fn_101BE730(int param_1);  // 0x101BE730
XI_API int __thiscall fn_101BECC0(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6);  // 0x101BECC0
XI_API int __thiscall fn_101BF280(void * self_, uint32_t param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x101BF280
XI_API int __thiscall fn_101BF370(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5);  // 0x101BF370
XI_API int __thiscall fn_101BF460(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x101BF460
XI_API int __thiscall fn_101BF550(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x101BF550
XI_API int __thiscall fn_101BF970(void * self_, uint32_t param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, int param_5, int param_6);  // 0x101BF970
XI_API int __thiscall fn_101C0050(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, int param_5);  // 0x101C0050
XI_API int __thiscall fn_101C0160(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, int param_4);  // 0x101C0160
XI_API int __thiscall fn_101C0260(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, int param_4);  // 0x101C0260
XI_API int __thiscall fn_101C0440(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, int param_4, int param_5);  // 0x101C0440
XI_API int __thiscall fn_101C0540(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, int param_4);  // 0x101C0540
XI_API undefined8 __thiscall fn_101C0630(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, int param_4);  // 0x101C0630
XI_API int __thiscall fn_101C0760(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, int param_4);  // 0x101C0760
XI_API int __thiscall fn_101C0FC0(void * self_, undefined4 param_1, int param_2);  // 0x101C0FC0
XI_API int __thiscall fn_101C1090(void * self_, undefined4 param_1);  // 0x101C1090
XI_API uint32_t __thiscall fn_101C11F0(void * self_, uint32_t param_1, undefined4 param_2, undefined4 param_3);  // 0x101C11F0
XI_API uint32_t __thiscall fn_101C13A0(void * self_, uint32_t param_1);  // 0x101C13A0
XI_API uint32_t __thiscall fn_101C1750(void * self_, undefined4 param_1, int param_2);  // 0x101C1750
XI_API uint32_t __thiscall fn_101C1820(void * self_, undefined4 param_1);  // 0x101C1820
XI_API uint32_t __thiscall fn_101C18E0(void * self_, undefined4 param_1);  // 0x101C18E0
XI_API int __thiscall fn_101C1B20(void * self_, uint32_t param_1);  // 0x101C1B20
XI_API int __thiscall fn_101C1C90(void * self_, undefined4 param_1);  // 0x101C1C90
XI_API int __thiscall fn_101C1DF0(void * self_, undefined4 param_1, int param_2);  // 0x101C1DF0
XI_API int __thiscall fn_101C1EC0(void * self_, undefined4 param_1);  // 0x101C1EC0
XI_API int __thiscall fn_101C2450(void * self_, undefined4 param_1, undefined4 param_2);  // 0x101C2450
XI_API undefined4 __thiscall fn_101C2840(void * self_, undefined4 param_1);  // 0x101C2840
XI_API void __thiscall fn_101C2D80(void * self_, undefined4 xi_s1);  // 0x101C2D80
XI_API void __thiscall fn_101C33E0(void * self_);  // 0x101C33E0
XI_API undefined4 __thiscall fn_101C33F0(void * self_);  // 0x101C33F0
XI_API undefined4 __thiscall fn_101C3DB0(void * self_, undefined4 param_1, undefined1 * param_2);  // 0x101C3DB0
XI_API undefined8 __thiscall fn_101C3DE0(void * self_, undefined4 param_1, undefined1 * param_2);  // 0x101C3DE0
XI_API undefined4 __thiscall fn_101C3E10(void * self_, undefined4 param_1, undefined1 * param_2);  // 0x101C3E10
XI_API undefined4 __stdcall fn_101C3E40(undefined4 param_1, undefined4 param_2, undefined1 * param_3, undefined4 xi_s4);  // 0x101C3E40
XI_API undefined4 __thiscall fn_101C3F40(void * self_, undefined1 * param_1, int param_2, char * param_3);  // 0x101C3F40
XI_API undefined4 __stdcall fn_101C3FB0(int param_1);  // 0x101C3FB0
XI_API undefined4 __attribute__((stdcall, regparm(1))) fn_101C3FE0(undefined4 xi_eax, LPCSTR param_1);  // 0x101C3FE0
XI_API void __thiscall fn_101C4170(void * self_, undefined4 xi_s1);  // 0x101C4170
XI_API uint32_t __thiscall fn_101C4260(void * self_, undefined4 xi_s1);  // 0x101C4260
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_101C46A0(undefined4 xi_eax);  // 0x101C46A0
XI_API undefined4 __thiscall fn_101C46E0(void * self_, undefined4 xi_s1, undefined4 xi_s2, undefined4 xi_s3, undefined4 xi_s4, undefined4 xi_s5, undefined4 xi_s6);  // 0x101C46E0
XI_API int __thiscall fn_101C4740(void * self_, int param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x101C4740
XI_API undefined4 __thiscall fn_101C4790(void * self_, undefined4 xi_s1, undefined4 xi_s2, undefined4 xi_s3, undefined4 xi_s4, undefined4 xi_s5);  // 0x101C4790
XI_API undefined4 __thiscall fn_101C47A0(void * self_, undefined4 xi_s1, undefined4 xi_s2, undefined4 xi_s3, undefined4 xi_s4);  // 0x101C47A0
XI_API undefined4 __thiscall fn_101C47B0(void * self_, undefined4 xi_s1, undefined4 xi_s2, undefined4 xi_s3, undefined4 xi_s4);  // 0x101C47B0
XI_API undefined4 __thiscall fn_101C4820(void * self_, uint32_t param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, int param_5, undefined1 * param_6);  // 0x101C4820
XI_API undefined4 __thiscall fn_101C48E0(void * self_, undefined4 xi_s1, undefined4 xi_s2, undefined4 xi_s3, undefined4 xi_s4, undefined4 xi_s5);  // 0x101C48E0
XI_API void __thiscall fn_101C48F0(void * self_, undefined4 xi_s1, undefined4 xi_s2, undefined4 xi_s3, undefined4 xi_s4);  // 0x101C48F0
XI_API void __thiscall fn_101C4900(void * self_, undefined4 xi_s1, undefined4 xi_s2, undefined4 xi_s3, undefined4 xi_s4);  // 0x101C4900
XI_API undefined4 __thiscall fn_101C4920(void * self_, undefined4 xi_s1, undefined4 xi_s2, undefined4 xi_s3, undefined4 xi_s4, undefined4 xi_s5);  // 0x101C4920
XI_API void __thiscall fn_101C4930(void * self_, undefined4 xi_s1, undefined4 xi_s2, undefined4 xi_s3, undefined4 xi_s4);  // 0x101C4930
XI_API undefined8 __thiscall fn_101C4940(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, int param_4);  // 0x101C4940
XI_API undefined4 __thiscall fn_101C4950(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, int param_4);  // 0x101C4950
XI_API undefined4 __thiscall fn_101C4990(void * self_);  // 0x101C4990
XI_API undefined4 __thiscall fn_101C49A0(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, int param_4);  // 0x101C49A0
XI_API void __thiscall fn_101C49E0(void * self_, undefined4 param_1, int param_2);  // 0x101C49E0
XI_API void __thiscall fn_101C49F0(void * self_, undefined4 param_1);  // 0x101C49F0
XI_API undefined4 __thiscall fn_101C4A10(void * self_, undefined4 xi_s1, undefined4 xi_s2, undefined4 xi_s3);  // 0x101C4A10
XI_API undefined4 __thiscall fn_101C4A30(void * self_, uint32_t param_1);  // 0x101C4A30
XI_API undefined4 __thiscall fn_101C4A80(void * self_, undefined4 xi_s1, undefined4 xi_s2);  // 0x101C4A80
XI_API undefined4 __thiscall fn_101C4A90(void * self_, undefined4 xi_s1);  // 0x101C4A90
XI_API undefined4 __thiscall fn_101C4AA0(void * self_, undefined4 xi_s1);  // 0x101C4AA0
XI_API undefined4 __thiscall fn_101C4AC0(void * self_);  // 0x101C4AC0
XI_API void __thiscall fn_101C4AE0(void * self_);  // 0x101C4AE0
XI_API int __thiscall FileMng_ResolveOrExit(void * self_, int param_1);  // 0x101C4AF0
XI_API undefined4 __thiscall fn_101C4B40(void * self_, undefined4 xi_s1);  // 0x101C4B40
XI_API undefined4 __thiscall fn_101C4B60(void * self_, undefined4 xi_s1, undefined4 xi_s2);  // 0x101C4B60
XI_API undefined4 __thiscall fn_101C4B70(void * self_, undefined4 xi_s1);  // 0x101C4B70
XI_API undefined4 __thiscall fn_101C4BF0(void * self_, undefined4 param_1, undefined4 param_2);  // 0x101C4BF0
XI_API void __thiscall fn_101C4C20(void * self_);  // 0x101C4C20
XI_API undefined4 __thiscall fn_101C4C70(void * self_, undefined4 xi_s1);  // 0x101C4C70
XI_API undefined4 __thiscall fn_101C4CC0(void * self_, undefined4 xi_s1);  // 0x101C4CC0
XI_API undefined4 __thiscall fn_101C4CD0(void * self_);  // 0x101C4CD0
XI_API void __thiscall fn_101C4CE0(void * self_, undefined4 xi_s1);  // 0x101C4CE0
XI_API undefined4 __thiscall fn_101C4CF0(void * self_);  // 0x101C4CF0
XI_API void __thiscall fn_101C4D10(void * self_);  // 0x101C4D10
XI_API void __thiscall fn_101C4D30(void * self_);  // 0x101C4D30
XI_API void __thiscall fn_101C4D40(void * self_);  // 0x101C4D40
XI_API void * __thiscall fn_101C5920(void * self_);  // 0x101C5920
XI_API void * __thiscall fn_101C5930(void * self_, int param_1);  // 0x101C5930
XI_API void __thiscall fn_101C59B0(void * self_, int param_1);  // 0x101C59B0
XI_API void __thiscall fn_101C5A30(void * self_, undefined4 param_1);  // 0x101C5A30
XI_API unsigned char __thiscall fn_101C5A40(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x101C5A40
XI_API undefined4 __thiscall fn_101C5EC0(void * self_);  // 0x101C5EC0
XI_API void __thiscall fn_101C60C0(void * self_);  // 0x101C60C0
XI_API undefined4 __thiscall fn_101C6810(void * self_, undefined4 param_1, undefined4 param_2);  // 0x101C6810
XI_API undefined4 __thiscall fn_101C68A0(void * self_);  // 0x101C68A0
XI_API void __thiscall fn_101C7C50(void * self_);  // 0x101C7C50
XI_API void __thiscall fn_101C7F80(void * self_);  // 0x101C7F80
XI_API void * __thiscall fn_101C8070(void * self_);  // 0x101C8070
XI_API void __thiscall fn_101C80A0(void * self_);  // 0x101C80A0
XI_API undefined4 __thiscall fn_101C80C0(void * self_, int param_1);  // 0x101C80C0
XI_API void * __thiscall fn_101C8260(void * self_);  // 0x101C8260
XI_API void __thiscall fn_101C8290(void * self_, int param_1);  // 0x101C8290
XI_API undefined4 __thiscall fn_101C8380(void * self_, int param_1);  // 0x101C8380
XI_API undefined4 __cdecl fn_101C87A0(int param_1, uint8_t * param_2, uint8_t * param_3);  // 0x101C87A0
XI_API int __cdecl fn_101C88A0(uint32_t param_1, int param_2, int param_3, undefined4 param_4);  // 0x101C88A0
XI_API undefined4 __cdecl fn_101C8C30(int param_1);  // 0x101C8C30
XI_API uint32_t __cdecl fn_101C8C50(uint8_t * param_1, int param_2);  // 0x101C8C50
XI_API undefined4 __cdecl fn_101C8D10(uint32_t param_1);  // 0x101C8D10
XI_API int __cdecl fn_101C8D70(void);  // 0x101C8D70
XI_API void __cdecl Gfx_ProjectWorldToScreen(_DWORD *a1, _WORD *a2, _WORD *a3);  // 0x101C8DA0
XI_API short * __cdecl fn_101C9270(short * param_1, undefined4 param_2);  // 0x101C9270
XI_API undefined8 __cdecl fn_101C9780(undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x101C9780
XI_API undefined8 __cdecl fn_101C9D50(undefined4 param_1, int param_2, undefined4 param_3, char * param_4, int param_5);  // 0x101C9D50
XI_API int __cdecl fn_101CA180(undefined4 param_1, undefined4 param_2, char * param_3, undefined4 param_4);  // 0x101CA180
XI_API void __cdecl fn_101CAD00(undefined4 param_1, undefined4 param_2, void * param_3, void * param_4, float param_5, float param_6, float param_7, float param_8, float param_9, float param_10);  // 0x101CAD00
XI_API void __cdecl fn_101CAEC0(undefined4 param_1, undefined4 param_2, void * param_3, void * param_4, float param_5, float param_6);  // 0x101CAEC0
XI_API undefined8 __cdecl fn_101CAF00(undefined4 param_1, undefined4 param_2, void * param_3, void * param_4, undefined4 param_5, uint8_t * param_6, float param_7, float param_8, float param_9, float param_10);  // 0x101CAF00
XI_API undefined8 __cdecl fn_101CB000(undefined4 param_1, undefined4 param_2, void * param_3, void * param_4, undefined4 param_5, uint8_t * param_6);  // 0x101CB000
XI_API void __cdecl fn_101CB040(undefined4 param_1, undefined4 param_2, float param_3, undefined4 param_4);  // 0x101CB040
XI_API undefined8 __cdecl fn_101CB090(undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x101CB090
XI_API void __cdecl fn_101CB1B0(undefined4 param_1, undefined4 param_2, undefined4 param_3, uint8_t * param_4);  // 0x101CB1B0
XI_API undefined4 __cdecl fn_101CB230(undefined4 param_1, undefined4 param_2, undefined4 param_3, uint8_t * param_4, float param_5, float param_6, float param_7, float param_8);  // 0x101CB230
XI_API void __cdecl fn_101CB430(undefined4 param_1, undefined4 param_2, void * param_3, void * param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7, uint8_t * param_8, float param_9, float param_10, float param_11, float param_12);  // 0x101CB430
XI_API void __cdecl fn_101CB500(undefined4 param_1, undefined4 param_2, void * param_3, void * param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7, uint8_t * param_8);  // 0x101CB500
XI_API void __cdecl fn_101CB550(undefined4 param_1, undefined4 param_2, undefined4 param_3, uint8_t * param_4);  // 0x101CB550
XI_API undefined4 __cdecl fn_101CB770(undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x101CB770
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_101CBDF0(undefined4 xi_eax, int param_1);  // 0x101CBDF0
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_101CBE50(undefined4 xi_eax);  // 0x101CBE50
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_101CBE60(undefined4 xi_eax);  // 0x101CBE60
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_101CBE70(undefined4 xi_eax);  // 0x101CBE70
XI_API undefined4 __cdecl fn_101CC070(undefined4 param_1);  // 0x101CC070
XI_API uint32_t __attribute__((cdecl, regparm(1))) fn_101CC1D0(undefined4 xi_eax, int param_1);  // 0x101CC1D0
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_101CC220(undefined4 xi_eax, int param_1);  // 0x101CC220
XI_API undefined4 __cdecl fn_101CC350(undefined4 param_1, undefined4 param_2);  // 0x101CC350
XI_API undefined4 __cdecl fn_101CC510(void);  // 0x101CC510
XI_API undefined4 __cdecl fn_101CC520(int param_1);  // 0x101CC520
XI_API undefined4 __cdecl fn_101CC550(void);  // 0x101CC550
XI_API undefined4 __cdecl fn_101CC560(int param_1);  // 0x101CC560
XI_API int __cdecl fn_101CCE00(int param_1);  // 0x101CCE00
XI_API void __thiscall fn_101CD900(void * self_);  // 0x101CD900
XI_API undefined4 __thiscall fn_101CD9D0(void * self_);  // 0x101CD9D0
XI_API undefined4 __thiscall fn_101CDA00(void * self_);  // 0x101CDA00
XI_API undefined4 __thiscall fn_101CDA30(void * self_);  // 0x101CDA30
XI_API void __thiscall fn_101CF660(void * self_, int param_1);  // 0x101CF660
XI_API void __stdcall fn_101CF820(int param_1, int param_2, int param_3, int param_4);  // 0x101CF820
XI_API void __thiscall fn_101CF870(void * self_);  // 0x101CF870
XI_API void __stdcall fn_101CFDB0(int param_1, int param_2, int param_3, int param_4, int param_5, int param_6);  // 0x101CFDB0
XI_API int __stdcall fn_101CFE20(uint32_t param_1, int * param_2, int * param_3, int param_4);  // 0x101CFE20
XI_API void __thiscall fn_101CFEB0(void * self_, int param_1, int param_2, int param_3, int param_4, int param_5, int * param_6);  // 0x101CFEB0
XI_API void __stdcall fn_101CFF60(int param_1);  // 0x101CFF60
XI_API void __thiscall fn_101D0210(void * self_, int * param_1, char * param_2);  // 0x101D0210
XI_API undefined4 __thiscall fn_101D0370(void * self_, uint32_t param_1, uint32_t param_2, int param_3, int param_4);  // 0x101D0370
XI_API undefined * __cdecl fn_101D05E0(void);  // 0x101D05E0
XI_API int __thiscall fn_101D0600(void * self_, uint32_t param_1, uint32_t param_2, int param_3, int param_4);  // 0x101D0600
XI_API void __thiscall fn_101D0860(void * self_);  // 0x101D0860
XI_API void __thiscall fn_101D0880(void * self_);  // 0x101D0880
XI_API undefined4 __thiscall fn_101D08C0(void * self_);  // 0x101D08C0
XI_API void __thiscall fn_101D08D0(void * self_);  // 0x101D08D0
XI_API undefined4 __thiscall fn_101D08E0(void * self_, undefined4 param_1, undefined4 param_2);  // 0x101D08E0
XI_API void __thiscall fn_101D0910(void * self_, int param_1);  // 0x101D0910
XI_API void __thiscall fn_101D0970(void * self_, uint32_t param_1, uint32_t param_2);  // 0x101D0970
XI_API void __thiscall fn_101D09C0(void * self_, undefined4 param_1);  // 0x101D09C0
XI_API void __thiscall fn_101D09E0(void * self_, int param_1, int param_2);  // 0x101D09E0
XI_API void __thiscall fn_101D0A20(void * self_, uint32_t param_1);  // 0x101D0A20
XI_API void __thiscall fn_101D0A40(void * self_, undefined4 xi_s1);  // 0x101D0A40
XI_API void __thiscall fn_101D0A60(void * self_);  // 0x101D0A60
XI_API undefined4 __cdecl fn_101D0AB0(void);  // 0x101D0AB0
XI_API undefined * __cdecl fn_101D0C40(void);  // 0x101D0C40
XI_API undefined4 __cdecl fn_101D1060(void * param_1, uint32_t param_2, uint32_t param_3);  // 0x101D1060
XI_API undefined4 __cdecl fn_101D10C0(int param_1, undefined4 param_2);  // 0x101D10C0
XI_API undefined4 __cdecl fn_101D1180(int param_1, undefined4 param_2);  // 0x101D1180
XI_API void __cdecl YkInit(void);  // 0x101D11C0
XI_API undefined4 __cdecl fn_101D29C0(uint32_t param_1, uint32_t param_2);  // 0x101D29C0
XI_API void __thiscall fn_101D2F00(void * self_);  // 0x101D2F00
XI_API void __thiscall fn_101D30B0(void * self_);  // 0x101D30B0
XI_API undefined4 __thiscall fn_101D30F0(void * self_);  // 0x101D30F0
XI_API void __thiscall fn_101D31B0(void * self_);  // 0x101D31B0
XI_API void __thiscall fn_101D3DC0(void * self_);  // 0x101D3DC0
XI_API void __thiscall fn_101D3FC0(void * self_, int * param_1);  // 0x101D3FC0
XI_API void __thiscall fn_101D5680(void * self_);  // 0x101D5680
XI_API void * __thiscall fn_101D7C10(void * self_);  // 0x101D7C10
XI_API void * __thiscall fn_101D87A0(void * self_);  // 0x101D87A0
XI_API void * __thiscall fn_101D9B10(void * self_);  // 0x101D9B10
XI_API void * __thiscall fn_101D9E90(void * self_);  // 0x101D9E90
XI_API void * __thiscall fn_101DC460(void * self_);  // 0x101DC460
XI_API void * __thiscall fn_101DCE20(void * self_);  // 0x101DCE20
XI_API void * __thiscall fn_101DD000(void * self_);  // 0x101DD000
XI_API void __thiscall fn_101DDA70(void * self_);  // 0x101DDA70
XI_API void __thiscall fn_101DDA90(void * self_, int param_1);  // 0x101DDA90
XI_API undefined4 __thiscall fn_101DDF90(void * self_);  // 0x101DDF90
XI_API undefined4 __thiscall fn_101DE6B0(void * self_);  // 0x101DE6B0
XI_API void * __thiscall fn_101DEDC0(void * self_);  // 0x101DEDC0
XI_API void * __thiscall fn_101DF210(void * self_);  // 0x101DF210
XI_API void __thiscall fn_101E06C0(void * self_);  // 0x101E06C0
XI_API void __thiscall fn_101E0700(void * self_);  // 0x101E0700
XI_API void * __thiscall fn_101E0A90(void * self_);  // 0x101E0A90
XI_API void __thiscall fn_101E15F0(void * self_);  // 0x101E15F0
XI_API undefined4 __thiscall fn_101E1A00(void * self_);  // 0x101E1A00
XI_API void __attribute__((stdcall, regparm(3))) fn_101E1AE0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x101E1AE0
XI_API void * __thiscall fn_101E2B10(void * self_);  // 0x101E2B10
XI_API void * __thiscall fn_101E3F50(void * self_);  // 0x101E3F50
XI_API void * __thiscall fn_101E48F0(void * self_);  // 0x101E48F0
XI_API void * __thiscall fn_101E4A20(void * self_);  // 0x101E4A20
XI_API void * __thiscall fn_101E51B0(void * self_);  // 0x101E51B0
XI_API void * __thiscall fn_101E5750(void * self_);  // 0x101E5750
XI_API void * __thiscall fn_101E5BA0(void * self_);  // 0x101E5BA0
XI_API void * __thiscall fn_101EB390(void * self_);  // 0x101EB390
XI_API void * __thiscall fn_101EB7E0(void * self_);  // 0x101EB7E0
XI_API void * __thiscall fn_101EBE00(void * self_);  // 0x101EBE00
XI_API void * __thiscall fn_101EC0D0(void * self_);  // 0x101EC0D0
XI_API void __thiscall fn_101EC3C0(void * self_);  // 0x101EC3C0
XI_API void * __thiscall fn_101ED450(void * self_);  // 0x101ED450
XI_API void * __thiscall fn_101EE230(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5);  // 0x101EE230
XI_API char __thiscall fn_101EEBD0(int self_);  // 0x101EEBD0
XI_API void __thiscall fn_101EFCA0(void * self_);  // 0x101EFCA0
XI_API void __thiscall fn_101EFEA0(void * self_);  // 0x101EFEA0
XI_API void * __thiscall fn_101F17C0(void * self_);  // 0x101F17C0
XI_API void * __thiscall fn_101F43F0(void * self_);  // 0x101F43F0
XI_API void __thiscall fn_101F4AF0(void * self_);  // 0x101F4AF0
XI_API void __attribute__((stdcall, regparm(3))) fn_101F6700(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, undefined4 param_2);  // 0x101F6700
XI_API void * __thiscall fn_101F67C0(void * self_, undefined4 param_1);  // 0x101F67C0
XI_API uint32_t __attribute__((cdecl, regparm(1))) fn_101F71A0(undefined4 xi_eax);  // 0x101F71A0
XI_API uint32_t __cdecl fn_101F71C0(int param_1, int param_2);  // 0x101F71C0
XI_API undefined4 __cdecl fn_101F7200(int param_1);  // 0x101F7200
XI_API undefined4 __cdecl fn_101F7240(int param_1);  // 0x101F7240
XI_API uint32_t __cdecl fn_101F7260(int param_1);  // 0x101F7260
XI_API int __cdecl fn_101F7430(int param_1, int param_2, undefined4 param_3);  // 0x101F7430
XI_API int __cdecl fn_101F7480(int param_1);  // 0x101F7480
XI_API uint32_t __cdecl fn_101F74F0(int param_1, int param_2, short * param_3, short * param_4, undefined4 param_5, undefined4 param_6);  // 0x101F74F0
XI_API long double __cdecl fn_101F75C0(int param_1);  // 0x101F75C0
XI_API void * __thiscall fn_101F86C0(void * self_);  // 0x101F86C0
XI_API void __thiscall fn_101F92B0(void * self_);  // 0x101F92B0
XI_API void __thiscall fn_101F9370(void * self_);  // 0x101F9370
XI_API void __thiscall fn_101FA420(void * self_, undefined4 param_1);  // 0x101FA420
XI_API void * __thiscall fn_101FB710(void * self_);  // 0x101FB710
XI_API void * __thiscall fn_101FC2C0(void * self_);  // 0x101FC2C0
XI_API void * __thiscall fn_101FCBC0(void * self_);  // 0x101FCBC0
XI_API void * __thiscall fn_101FD000(void * self_);  // 0x101FD000
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_101FD600(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, uint32_t * param_2);  // 0x101FD600
XI_API undefined4 __thiscall fn_101FE430(void * self_);  // 0x101FE430
XI_API void * __thiscall fn_101FF000(void * self_);  // 0x101FF000
XI_API void * __thiscall fn_10200B60(void * self_);  // 0x10200B60
XI_API void * __thiscall fn_10201020(void * self_);  // 0x10201020
XI_API void * __thiscall fn_10201130(void * self_);  // 0x10201130
XI_API void __thiscall fn_102014F0(void * self_, undefined4 xi_s1);  // 0x102014F0
XI_API void __thiscall fn_102015D0(void * self_);  // 0x102015D0
XI_API void __cdecl fn_102017A0(void);  // 0x102017A0
XI_API void * __thiscall fn_10201C90(void * self_);  // 0x10201C90
XI_API undefined4 __cdecl fn_10201EB0(int param_1);  // 0x10201EB0
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_10201F00(undefined4 xi_eax);  // 0x10201F00
XI_API void __stdcall fn_10202380(undefined4 param_1, undefined4 param_2, char * param_3, undefined4 param_4);  // 0x10202380
XI_API undefined * __stdcall fn_10202C90(int param_1);  // 0x10202C90
XI_API void __cdecl fn_10202D10(void);  // 0x10202D10
XI_API void __cdecl fn_10202D30(undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6);  // 0x10202D30
XI_API void __cdecl fn_10202D80(int param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7);  // 0x10202D80
XI_API uint32_t __attribute__((stdcall, regparm(1))) fn_102031F0(undefined4 xi_eax, int param_1, undefined4 param_2, undefined4 param_3);  // 0x102031F0
XI_API undefined4 __stdcall fn_102033C0(int param_1);  // 0x102033C0
XI_API undefined4 __stdcall fn_102034A0(undefined4 param_1);  // 0x102034A0
XI_API undefined4 __stdcall fn_102034E0(undefined4 param_1);  // 0x102034E0
XI_API undefined4 __stdcall fn_10203500(int param_1, undefined4 param_2);  // 0x10203500
XI_API void __stdcall fn_10203520(int param_1);  // 0x10203520
XI_API int __stdcall fn_10203540(int param_1);  // 0x10203540
XI_API void __attribute__((stdcall, regparm(3))) fn_10203920(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x10203920
XI_API void __stdcall fn_10203BC0(undefined4 param_1, undefined4 xi_s2);  // 0x10203BC0
XI_API void * __thiscall fn_10205500(void * self_);  // 0x10205500
XI_API void __thiscall fn_10206420(void * self_);  // 0x10206420
XI_API void * __thiscall fn_10207730(void * self_);  // 0x10207730
XI_API undefined4 __thiscall fn_10208500(void * self_);  // 0x10208500
XI_API void * __thiscall fn_10209300(void * self_);  // 0x10209300
XI_API void * __thiscall fn_10209B90(void * self_);  // 0x10209B90
XI_API void * __thiscall fn_1020B500(void * self_);  // 0x1020B500
XI_API undefined4 __thiscall fn_1020BA40(void * self_, undefined4 param_1);  // 0x1020BA40
XI_API void __thiscall fn_1020BE90(void * self_);  // 0x1020BE90
XI_API void * __thiscall fn_1020C470(void * self_);  // 0x1020C470
XI_API void * __thiscall fn_1020CC30(void * self_);  // 0x1020CC30
XI_API void * __thiscall fn_1020ED00(void * self_);  // 0x1020ED00
XI_API void * __thiscall YkWndShop_Constructor(void * self_);  // 0x10210310
XI_API void __thiscall fn_10210D60(void * self_);  // 0x10210D60
XI_API void * __thiscall fn_10210DA0(void * self_, undefined4 * param_1, undefined4 param_2);  // 0x10210DA0
XI_API undefined4 __thiscall fn_10210E30(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, int param_5, undefined4 param_6, int param_7);  // 0x10210E30
XI_API void __thiscall fn_10210FC0(void * self_, undefined4 param_1);  // 0x10210FC0
XI_API void __thiscall fn_10211050(void * self_);  // 0x10211050
XI_API undefined4 __thiscall fn_102112B0(void * self_, undefined4 param_1);  // 0x102112B0
XI_API undefined4 __cdecl fn_10211340(void);  // 0x10211340
XI_API void __thiscall fn_10211370(void * self_);  // 0x10211370
XI_API void * __thiscall fn_10216570(void * self_);  // 0x10216570
XI_API void * __thiscall fn_102175C0(void * self_);  // 0x102175C0
XI_API void __thiscall fn_1021B330(void * self_);  // 0x1021B330
XI_API void __cdecl fn_1021BB90(void);  // 0x1021BB90
XI_API void __thiscall fn_1021BC20(void * self_);  // 0x1021BC20
XI_API undefined4 * __thiscall fn_1021BC30(void * self_, char * param_1);  // 0x1021BC30
XI_API undefined4 __cdecl fn_1021BCB0(void);  // 0x1021BCB0
XI_API unsigned char __thiscall fn_1021BDB0(void * self_, char * param_1);  // 0x1021BDB0
XI_API undefined4 __thiscall fn_1021BF60(void * self_, char * param_1, undefined4 xi_s2);  // 0x1021BF60
XI_API void * __thiscall fn_1021C010(void * self_);  // 0x1021C010
XI_API undefined4 __thiscall fn_1021C070(void * self_, undefined4 param_1, int param_2, int param_3);  // 0x1021C070
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_1021C100(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, int param_2, int param_3);  // 0x1021C100
XI_API undefined4 __thiscall fn_1021C160(void * self_);  // 0x1021C160
XI_API undefined4 __cdecl fn_1021C1B0(int param_1);  // 0x1021C1B0
XI_API undefined4 __thiscall fn_1021C210(void * self_, short * param_1, uint32_t param_2);  // 0x1021C210
XI_API undefined4 __thiscall fn_1021C2E0(void * self_, short * param_1, uint32_t param_2);  // 0x1021C2E0
XI_API undefined4 __thiscall fn_1021C350(void * self_, undefined4 param_1, uint32_t param_2);  // 0x1021C350
XI_API undefined4 __thiscall fn_1021C380(void * self_);  // 0x1021C380
XI_API uint32_t __thiscall fn_1021C4C0(void * self_);  // 0x1021C4C0
XI_API uint32_t __thiscall fn_1021C5B0(void * self_, undefined2 * param_1, undefined4 param_2, int param_3);  // 0x1021C5B0
XI_API int __thiscall fn_1021C680(void * self_, uint32_t param_1);  // 0x1021C680
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_1021C7C0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x1021C7C0
XI_API undefined4 __thiscall fn_1021C7E0(void * self_, undefined4 param_1, int param_2, int param_3, undefined4 param_4);  // 0x1021C7E0
XI_API void * __thiscall fn_1021D250(void * self_);  // 0x1021D250
XI_API undefined4 __thiscall fn_1021D270(void * self_);  // 0x1021D270
XI_API undefined4 __thiscall fn_1021D2D0(void * self_, undefined4 param_1, undefined4 param_2);  // 0x1021D2D0
XI_API undefined4 __thiscall fn_1021D380(void * self_, undefined4 param_1, undefined4 param_2);  // 0x1021D380
XI_API undefined4 __cdecl fn_1021D3D0(undefined4 param_1);  // 0x1021D3D0
XI_API undefined4 __cdecl fn_1021D440(void);  // 0x1021D440
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_1021D460(undefined4 xi_eax);  // 0x1021D460
XI_API undefined4 __cdecl fn_1021D490(void);  // 0x1021D490
XI_API long double __cdecl fn_1021D4A0(void);  // 0x1021D4A0
XI_API void * __thiscall fn_1021D7F0(void * self_);  // 0x1021D7F0
XI_API uint32_t __thiscall fn_1021D930(void * self_, undefined4 param_1);  // 0x1021D930
XI_API undefined4 __thiscall Movie_Update(void * self_);  // 0x1021D980
XI_API int * __stdcall fn_1021DD20(int * param_1, int * param_2);  // 0x1021DD20
XI_API void __cdecl fn_1021DDC0(void);  // 0x1021DDC0
XI_API undefined4 __thiscall Movie_GetCurrentTexture(void * self_);  // 0x1021DDF0
XI_API void __thiscall fn_1021DE10(void * self_);  // 0x1021DE10
XI_API void __thiscall fn_1021DE20(void * self_);  // 0x1021DE20
XI_API void __thiscall fn_1021DE30(void * self_, undefined4 param_1);  // 0x1021DE30
XI_API void __thiscall fn_1021DE50(void * self_, undefined4 param_1);  // 0x1021DE50
XI_API void __thiscall fn_1021DE70(void * self_, undefined4 param_1);  // 0x1021DE70
XI_API void * __thiscall fn_1021DE90(void * self_, undefined4 param_1, undefined4 * param_2);  // 0x1021DE90
XI_API int __thiscall Movie_CreateFrameTextures(Vt_1033BE90 *self_, int a2);  // 0x1021E060
XI_API int __cdecl fn_1021E3B0(uint8_t * param_1, int param_2);  // 0x1021E3B0
XI_API int __cdecl fn_1021E480(short * param_1, int * param_2, int param_3);  // 0x1021E480
XI_API void __cdecl fn_1021E4D0(int * param_1, undefined4 * param_2);  // 0x1021E4D0
XI_API void __cdecl fn_1021E500(undefined4 * param_1, undefined4 * param_2, uint32_t param_3);  // 0x1021E500
XI_API int __cdecl fn_1021E550(int param_1, int param_2, int param_3, int param_4, int param_5, undefined1 * param_6);  // 0x1021E550
XI_API uint32_t __cdecl fn_1021E670(undefined4 param_1);  // 0x1021E670
XI_API void __thiscall fn_1021E690(void * self_);  // 0x1021E690
XI_API void __thiscall fn_1021E760(void * self_, undefined4 param_1, int param_2, uint32_t param_3, undefined4 param_4);  // 0x1021E760
XI_API void __cdecl fn_1021ED90(void);  // 0x1021ED90
XI_API void __attribute__((cdecl, regparm(1))) fn_1021EDC0(undefined4 xi_eax);  // 0x1021EDC0
XI_API void __attribute__((cdecl, regparm(1))) fn_1021EDD0(undefined4 xi_eax);  // 0x1021EDD0
XI_API undefined4 __cdecl fn_1021EDE0(void);  // 0x1021EDE0
XI_API undefined4 __cdecl fn_1021EE00(undefined4 param_1);  // 0x1021EE00
XI_API void __cdecl fn_1021EE20(undefined4 param_1);  // 0x1021EE20
XI_API void __cdecl fn_1021EE70(float param_1, float param_2, undefined4 param_3);  // 0x1021EE70
XI_API void __thiscall fn_1021EEB0(void * self_);  // 0x1021EEB0
XI_API void __thiscall fn_1021EF10(void * self_);  // 0x1021EF10
XI_API long double __stdcall fn_1021F030(float * param_1, int * param_2);  // 0x1021F030
XI_API int __stdcall fn_1021F140(int param_1, undefined4 param_2);  // 0x1021F140
XI_API void __stdcall fn_1021F360(short * param_1);  // 0x1021F360
XI_API void __thiscall fn_1021F410(void * self_);  // 0x1021F410
XI_API void __thiscall fn_1021F950(void * self_, undefined4 xi_s1);  // 0x1021F950
XI_API undefined4 __stdcall fn_1021FA90(int param_1, int param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7, uint8_t * param_8);  // 0x1021FA90
XI_API void __stdcall fn_1021FB10(int param_1, undefined4 param_2, undefined4 xi_s3);  // 0x1021FB10
XI_API void __stdcall fn_1021FB80(int param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, uint8_t * param_6);  // 0x1021FB80
XI_API void __stdcall fn_1021FBD0(undefined4 param_1, undefined4 param_2, int param_3, int param_4, int param_5, int param_6);  // 0x1021FBD0
XI_API void __thiscall fn_1021FCE0(void * self_);  // 0x1021FCE0
XI_API void __thiscall fn_1021FE10(void * self_);  // 0x1021FE10
XI_API void __stdcall fn_102201A0(int param_1, undefined4 param_2, void * param_3, float param_4, undefined4 param_5);  // 0x102201A0
XI_API void __thiscall fn_102201F0(void * self_, int param_1, int param_2);  // 0x102201F0
XI_API int __thiscall fn_10220350(void * self_);  // 0x10220350
XI_API long double __cdecl fn_10220C20(void);  // 0x10220C20
XI_API void __cdecl fn_10220C80(void);  // 0x10220C80
XI_API undefined4 __cdecl fn_10220CF0(int param_1, int param_2);  // 0x10220CF0
XI_API undefined4 __cdecl fn_10220D10(int param_1);  // 0x10220D10
XI_API void * __thiscall fn_10220FD0(void * self_);  // 0x10220FD0
XI_API undefined4 __thiscall fn_102211B0(void * self_, int param_1);  // 0x102211B0
XI_API int __thiscall fn_102214D0(int *self_);  // 0x102214D0
XI_API uint32_t __thiscall fn_10221740(void * self_);  // 0x10221740
XI_API void __cdecl fn_102217A0(void);  // 0x102217A0
XI_API void * __thiscall fn_102217C0(void * self_);  // 0x102217C0
XI_API void __thiscall fn_10221900(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x10221900
XI_API void __thiscall fn_10221920(void * self_, undefined4 param_1, undefined4 param_2, int param_3, int param_4, int param_5);  // 0x10221920
XI_API void __thiscall fn_10221A80(void * self_, int param_1, undefined4 param_2);  // 0x10221A80
XI_API void __thiscall fn_10221BE0(void * self_, int * param_1, int * param_2);  // 0x10221BE0
XI_API void __thiscall fn_10221F50(void * self_);  // 0x10221F50
XI_API void __thiscall fn_10222110(void * self_, undefined4 param_1);  // 0x10222110
XI_API void __cdecl fn_10222960(void);  // 0x10222960
XI_API uint32_t __cdecl fn_10222C10(uint32_t param_1);  // 0x10222C10
XI_API int __cdecl fn_10222C90(undefined4 param_1, undefined4 * param_2);  // 0x10222C90
XI_API void __cdecl fn_10222DF0(void);  // 0x10222DF0
XI_API uint32_t __cdecl fn_10223040(uint32_t param_1, undefined4 param_2);  // 0x10223040
XI_API void * __thiscall fn_102238E0(void * self_);  // 0x102238E0
XI_API undefined * __cdecl fn_10223930(void);  // 0x10223930
XI_API void * __thiscall fn_102239A0(void * self_);  // 0x102239A0
XI_API void __thiscall fn_102239C0(void * self_);  // 0x102239C0
XI_API void * __thiscall fn_10223BB0(void * self_);  // 0x10223BB0
XI_API char __thiscall fn_10223C90(void * self_);  // 0x10223C90
XI_API void * __thiscall fn_10223E60(void * self_);  // 0x10223E60
XI_API int __thiscall fn_10225090(void * self_, uint32_t param_1);  // 0x10225090
XI_API int __thiscall fn_10225120(void * self_, int param_1, int param_2, undefined4 xi_s3);  // 0x10225120
XI_API int __thiscall fn_10225220(void * self_, int param_1, int param_2, undefined4 xi_s3);  // 0x10225220
XI_API void * __thiscall fn_102254D0(void * self_);  // 0x102254D0
XI_API void * __thiscall fn_10226080(void * self_);  // 0x10226080
XI_API undefined * __cdecl fn_10226180(void);  // 0x10226180
XI_API void * __thiscall fn_102261F0(void * self_);  // 0x102261F0
XI_API void __thiscall fn_10226210(void * self_);  // 0x10226210
XI_API void __cdecl fn_10226540(void);  // 0x10226540
XI_API void * __thiscall fn_10226550(void * self_);  // 0x10226550
XI_API void * __thiscall fn_10226C80(void * self_);  // 0x10226C80
XI_API void * __thiscall fn_10227080(void * self_);  // 0x10227080
XI_API void * __thiscall fn_102271B0(void * self_);  // 0x102271B0
XI_API void __thiscall fn_102271D0(void * self_);  // 0x102271D0
XI_API void __cdecl fn_10227750(void);  // 0x10227750
XI_API void * __thiscall fn_102278B0(void * self_);  // 0x102278B0
XI_API void __cdecl fn_10227F70(int param_1, undefined4 param_2, int param_3, undefined4 param_4, int param_5);  // 0x10227F70
XI_API void __cdecl fn_10228080(void * param_1, undefined4 param_2, undefined4 param_3, int param_4, int param_5);  // 0x10228080
XI_API void __thiscall fn_102281C0(void * self_);  // 0x102281C0
XI_API void * __thiscall fn_10228200(void * self_);  // 0x10228200
XI_API void __cdecl fn_10229430(void);  // 0x10229430
XI_API void * __thiscall fn_10229670(void * self_);  // 0x10229670
XI_API undefined4 __thiscall fn_1022A6D0(void * self_);  // 0x1022A6D0
XI_API undefined4 __cdecl fn_1022AF80(uint32_t param_1);  // 0x1022AF80
XI_API void __cdecl fn_1022AFB0(int param_1, uint32_t * param_2, int * param_3);  // 0x1022AFB0
XI_API undefined4 __cdecl fn_1022AFF0(undefined4 param_1);  // 0x1022AFF0
XI_API void __cdecl fn_1022B080(void);  // 0x1022B080
XI_API void __cdecl fn_1022B260(void);  // 0x1022B260
XI_API void __cdecl fn_1022B290(void);  // 0x1022B290
XI_API int __cdecl fn_1022B2F0(int param_1);  // 0x1022B2F0
XI_API void __cdecl fn_1022B3B0(undefined4 param_1);  // 0x1022B3B0
XI_API undefined4 __cdecl fn_1022B7D0(int param_1);  // 0x1022B7D0
XI_API void * __thiscall fn_1022BD50(void * self_);  // 0x1022BD50
XI_API undefined * __cdecl fn_1022BDA0(void);  // 0x1022BDA0
XI_API void * __thiscall fn_1022BE10(void * self_);  // 0x1022BE10
XI_API void __thiscall fn_1022BE30(void * self_);  // 0x1022BE30
XI_API undefined4 __thiscall fn_1022BF00(void * self_, uint32_t param_1);  // 0x1022BF00
XI_API void * __thiscall fn_1022BFB0(void * self_);  // 0x1022BFB0
XI_API void * __thiscall fn_1022C350(void * self_);  // 0x1022C350
XI_API void * __thiscall fn_1022DB80(void * self_);  // 0x1022DB80
XI_API void __cdecl fn_1022EC50(void);  // 0x1022EC50
XI_API void __thiscall fn_1022ED00(void * self_);  // 0x1022ED00
XI_API void __thiscall fn_1022ED30(void * self_, int param_1, int param_2);  // 0x1022ED30
XI_API undefined4 __thiscall fn_1022ED70(void * self_, char * param_1);  // 0x1022ED70
XI_API undefined1 __thiscall fn_1022EEE0(void * self_, int param_1);  // 0x1022EEE0
XI_API undefined4 __cdecl fn_1022F240(undefined4 param_1);  // 0x1022F240
XI_API void __cdecl fn_1022F3D0(void);  // 0x1022F3D0
XI_API void * __thiscall fn_1022F610(void * self_);  // 0x1022F610
XI_API void * __thiscall fn_102303B0(void * self_);  // 0x102303B0
XI_API void __thiscall fn_10230410(void * self_);  // 0x10230410
XI_API void * __thiscall fn_10233BE0(void * self_);  // 0x10233BE0
XI_API void * __thiscall fn_102340A0(void * self_);  // 0x102340A0
XI_API void __thiscall fn_10234660(void * self_);  // 0x10234660
XI_API int __thiscall fn_10234A00(void * self_, undefined4 xi_s1, undefined4 xi_s2);  // 0x10234A00
XI_API undefined8 __thiscall fn_10234A70(void * self_, undefined4 param_1, undefined4 param_2);  // 0x10234A70
XI_API void * __thiscall fn_102380A0(void * self_);  // 0x102380A0
XI_API void * __thiscall fn_102388C0(void * self_);  // 0x102388C0
XI_API void * __thiscall fn_10238B30(void * self_);  // 0x10238B30
XI_API void * __thiscall fn_10239170(void * self_);  // 0x10239170
XI_API void __thiscall Ui_KeyPrompt_Init(void * self_, undefined4 param_1);  // 0x10239460
XI_API undefined4 __cdecl fn_10239490(void);  // 0x10239490
XI_API char * __thiscall Ui_DrawKeyPrompt(Vt_1033C8F0 *self_);  // 0x102394A0
XI_API void __cdecl fn_1023BF20(undefined4 param_1);  // 0x1023BF20
XI_API void * __thiscall fn_1023D610(void * self_);  // 0x1023D610
XI_API undefined4 __cdecl fn_1023E4B0(int * param_1);  // 0x1023E4B0
XI_API void * __thiscall CIwHnList_Constructor(void * self_);  // 0x1023E830
XI_API void * __thiscall fn_1023F890(void * self_);  // 0x1023F890
XI_API void * __thiscall fn_10244DF0(void * self_);  // 0x10244DF0
XI_API void __cdecl fn_10244F20(void);  // 0x10244F20
XI_API void * __thiscall fn_10244F30(void * self_);  // 0x10244F30
XI_API void __thiscall fn_10245040(void * self_);  // 0x10245040
XI_API undefined4 __cdecl LobbyThread_Start(undefined4 param_1);  // 0x10245600
XI_API undefined4 __cdecl fn_10245B50(undefined4 param_1);  // 0x10245B50
XI_API int __cdecl Lobby_DestroyThreadPool(void);  // 0x10245B60
XI_API void __cdecl thunk_sub_10245BB0(void);  // 0x10245BA0
XI_API void __cdecl fn_10245BB0(void);  // 0x10245BB0
XI_API undefined4 __cdecl fn_10245BE0(void);  // 0x10245BE0
XI_API undefined4 __thiscall fn_10245F30(void * self_);  // 0x10245F30
XI_API undefined4 __stdcall fn_10246000(undefined4 param_1, undefined4 param_2);  // 0x10246000
XI_API undefined8 __thiscall fn_10246030(void * self_, char * param_1, uint32_t param_2, uint32_t param_3);  // 0x10246030
XI_API undefined8 __stdcall fn_10246160(undefined4 param_1, int param_2, int param_3);  // 0x10246160
XI_API void __stdcall fn_102462F0(undefined4 param_1, uint32_t param_2, undefined4 param_3);  // 0x102462F0
XI_API undefined4 __stdcall fn_10246320(undefined4 param_1, uint32_t param_2, undefined4 param_3);  // 0x10246320
XI_API void __stdcall fn_102463C0(undefined4 param_1);  // 0x102463C0
XI_API undefined4 __stdcall fn_102463F0(undefined4 param_1, uint32_t param_2, undefined4 param_3);  // 0x102463F0
XI_API uint32_t __stdcall fn_102465E0(uint32_t param_1, uint32_t param_2, undefined4 param_3);  // 0x102465E0
XI_API void __stdcall fn_10246770(undefined4 param_1, uint32_t param_2, undefined4 param_3);  // 0x10246770
XI_API void __stdcall fn_102467A0(undefined4 param_1, undefined4 param_2);  // 0x102467A0
XI_API undefined8 __stdcall fn_102467F0(undefined4 param_1);  // 0x102467F0
XI_API undefined8 __stdcall fn_1024C060(int param_1, int * param_2, int param_3, int param_4, int param_5, int param_6, int param_7, int param_8);  // 0x1024C060
XI_API void * __thiscall fn_1024C210(void * self_);  // 0x1024C210
XI_API void * __thiscall fn_1024C930(void * self_);  // 0x1024C930
XI_API int __cdecl fn_1024CF50(_DWORD *a1);  // 0x1024CF50
XI_API void __cdecl fn_1024D130(void);  // 0x1024D130
XI_API uint32_t __cdecl StAvatar_Teardown(uint32_t * param_1, uint32_t param_2, uint32_t param_3);  // 0x1024D180
XI_API undefined4 __cdecl StAvatar_ReleaseResources(uint32_t * param_1, int param_2);  // 0x1024D2C0
XI_API void __cdecl fn_1024D400(int param_1);  // 0x1024D400
XI_API void __cdecl fn_1024D470(int param_1, int param_2);  // 0x1024D470
XI_API undefined4 __cdecl fn_1024D4A0(uint8_t * param_1, int param_2);  // 0x1024D4A0
XI_API undefined4 __cdecl fn_1024D540(uint8_t * param_1, int param_2, int param_3);  // 0x1024D540
XI_API undefined4 __cdecl fn_1024D5C0(uint32_t * param_1, uint32_t param_2);  // 0x1024D5C0
XI_API undefined4 __cdecl fn_1024D610(uint32_t * param_1);  // 0x1024D610
XI_API void __cdecl fn_1024D6B0(uint8_t * param_1);  // 0x1024D6B0
XI_API undefined4 __cdecl fn_1024D740(uint32_t * param_1, float param_2, int param_3);  // 0x1024D740
XI_API undefined4 __cdecl StAvatar_StopAndFreeBuffers(int param_1);  // 0x1024D790
XI_API void __cdecl fn_1024D850(uint32_t * param_1);  // 0x1024D850
XI_API void __cdecl fn_1024D9D0(void);  // 0x1024D9D0
XI_API undefined4 __cdecl fn_1024DB60(uint8_t * param_1);  // 0x1024DB60
XI_API void __cdecl fn_1024DD10(uint32_t * param_1, undefined4 param_2);  // 0x1024DD10
XI_API void __cdecl fn_1024DE20(float param_1);  // 0x1024DE20
XI_API void __cdecl fn_1024DFE0(int param_1);  // 0x1024DFE0
XI_API void __cdecl fn_1024E720(int param_1, undefined4 param_2, undefined4 param_3);  // 0x1024E720
XI_API void __cdecl fn_1024F050(int param_1, undefined4 param_2);  // 0x1024F050
XI_API void __cdecl fn_1024F0B0(int param_1, float * param_2, int param_3);  // 0x1024F0B0
XI_API void __cdecl fn_1024F2C0(int param_1);  // 0x1024F2C0
XI_API void __cdecl fn_1024F5E0(int param_1, int param_2);  // 0x1024F5E0
XI_API void __cdecl StAvatar_SetViewFromDancerCamera(int a1);  // 0x1024F900
XI_API void __cdecl fn_1024FC10(uint32_t * param_1, int param_2);  // 0x1024FC10
XI_API void __cdecl fn_1024FEC0(int param_1);  // 0x1024FEC0
XI_API undefined4 __cdecl fn_1024FF50(uint8_t * param_1, float * param_2);  // 0x1024FF50
XI_API undefined4 __cdecl fn_10250190(int param_1, int param_2);  // 0x10250190
XI_API int __cdecl fn_102501C0(_BYTE *arg0);  // 0x102501C0
XI_API void __cdecl fn_102502D0(uint8_t * param_1);  // 0x102502D0
XI_API undefined4 __cdecl fn_10250580(uint8_t * param_1, uint32_t param_2);  // 0x10250580
XI_API undefined4 __cdecl fn_10250660(undefined4 param_1, int param_2, int param_3);  // 0x10250660
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_102506C0(undefined4 xi_eax, uint32_t * param_1, int param_2);  // 0x102506C0
XI_API undefined4 __cdecl fn_10250760(uint8_t * param_1, int param_2);  // 0x10250760
XI_API undefined4 __cdecl fn_10250790(uint8_t * param_1, int param_2);  // 0x10250790
XI_API undefined4 __cdecl fn_102507C0(uint32_t * param_1, uint32_t param_2);  // 0x102507C0
XI_API undefined4 __cdecl fn_10250800(uint8_t * param_1, undefined4 param_2);  // 0x10250800
XI_API void __cdecl fn_102509C0(uint32_t * param_1, uint32_t param_2);  // 0x102509C0
XI_API void __cdecl fn_10250A40(uint32_t * param_1, int param_2, int param_3);  // 0x10250A40
XI_API undefined4 __cdecl fn_10250B10(uint32_t * param_1, uint32_t param_2, int param_3);  // 0x10250B10
XI_API undefined4 __cdecl fn_10250C00(uint8_t * param_1, undefined4 * param_2);  // 0x10250C00
XI_API undefined4 __cdecl fn_10250D80(undefined4 * param_1);  // 0x10250D80
XI_API uint32_t __cdecl fn_10251070(uint32_t * param_1, int param_2, float param_3);  // 0x10251070
XI_API undefined4 __cdecl fn_10251160(uint32_t * param_1);  // 0x10251160
XI_API undefined4 __cdecl fn_10251180(uint32_t * param_1);  // 0x10251180
XI_API undefined4 __cdecl fn_102511B0(uint8_t * param_1, float param_2);  // 0x102511B0
XI_API undefined4 __cdecl fn_10251200(uint8_t * param_1, undefined4 param_2, int param_3);  // 0x10251200
XI_API undefined4 __cdecl StChannelItemClear(uint32_t * param_1);  // 0x102512B0
XI_API undefined4 __cdecl fn_10251320(uint32_t * param_1, undefined4 param_2);  // 0x10251320
XI_API undefined4 __cdecl fn_10251370(uint8_t * param_1, undefined4 * param_2);  // 0x10251370
XI_API undefined4 __cdecl fn_102513C0(uint8_t * param_1, undefined4 * param_2);  // 0x102513C0
XI_API undefined4 __cdecl fn_10251410(uint8_t * param_1, undefined4 * param_2);  // 0x10251410
XI_API undefined4 __thiscall fn_102514C0(void * self_);  // 0x102514C0
XI_API void __thiscall StDancer_Update(void * self_);  // 0x10251500
XI_API void __thiscall Dancer_WorldEnd(void * self_, undefined4 param_1);  // 0x102515F0
XI_API void __thiscall fn_10251620(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, int param_4);  // 0x10251620
XI_API void __thiscall fn_102516B0(void * self_, int param_1, uint32_t param_2);  // 0x102516B0
XI_API void __thiscall fn_10251720(void * self_, int param_1);  // 0x10251720
XI_API undefined4 __thiscall fn_10251750(void * self_, int param_1);  // 0x10251750
XI_API void __attribute__((stdcall, regparm(3))) fn_10251780(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, int param_2);  // 0x10251780
XI_API void __thiscall StDancer_SetJobLock(void * self_, int param_1);  // 0x10251890
XI_API void __thiscall fn_10251920(void * self_, int param_1);  // 0x10251920
XI_API uint32_t __thiscall fn_10251960(void * self_);  // 0x10251960
XI_API char __thiscall Dancer_InitDevice(int *self_);  // 0x10251A80
XI_API undefined4 __cdecl fn_10251CE0(void);  // 0x10251CE0
XI_API undefined4 __cdecl fn_102524B0(undefined4 * param_1);  // 0x102524B0
XI_API undefined4 __cdecl StModelItemClear(uint32_t * param_1);  // 0x102524E0
XI_API undefined4 __cdecl fn_10252A00(uint8_t * param_1, uint32_t param_2);  // 0x10252A00
XI_API undefined4 __cdecl fn_10252A40(uint32_t * param_1);  // 0x10252A40
XI_API undefined4 __cdecl fn_10252AC0(uint8_t * param_1, undefined4 * param_2, undefined4 * param_3);  // 0x10252AC0
XI_API undefined4 __cdecl fn_10252B50(uint8_t * param_1, undefined4 param_2);  // 0x10252B50
XI_API undefined4 __cdecl fn_10252C10(uint8_t * param_1);  // 0x10252C10
XI_API undefined4 __cdecl fn_10252C30(uint32_t * param_1, undefined4 param_2);  // 0x10252C30
XI_API undefined4 __cdecl fn_10252C80(undefined4 * param_1);  // 0x10252C80
XI_API int __cdecl StTrigger_ProcessOpcodesUpToTime(uint32_t * param_1, uint32_t param_2, int param_3, undefined4 * param_4);  // 0x10252CC0
XI_API int __cdecl fn_10252EA0(uint32_t * param_1, int param_2, int param_3);  // 0x10252EA0
XI_API undefined4 __cdecl StTrigger_RegisterCallbacks(int param_1, uint32_t param_2, uint32_t param_3);  // 0x10252F00
XI_API undefined4 __cdecl fn_10253160(uint32_t * param_1);  // 0x10253160
XI_API undefined4 __cdecl sqopSystemActivate(uint32_t * param_1);  // 0x102531B0
XI_API undefined4 __cdecl fn_10253210(uint32_t * param_1);  // 0x10253210
XI_API undefined4 __cdecl fn_102532A0(undefined4 param_1);  // 0x102532A0
XI_API void * __thiscall fn_102532E0(void * self_);  // 0x102532E0
XI_API void __cdecl fn_10253330(void);  // 0x10253330
XI_API void __stdcall fn_10253360(int param_1);  // 0x10253360
XI_API int __stdcall fn_102534B0(int param_1);  // 0x102534B0
XI_API void __cdecl fn_102535B0(void);  // 0x102535B0
XI_API undefined8 __attribute__((cdecl, regparm(1))) fn_102535F0(undefined4 xi_eax, undefined4 param_1, undefined4 param_2);  // 0x102535F0
XI_API undefined4 __cdecl fn_10253620(void);  // 0x10253620
XI_API void __cdecl fn_10253630(void);  // 0x10253630
XI_API void __cdecl fn_10253670(undefined4 param_1, undefined4 param_2);  // 0x10253670
XI_API undefined4 __cdecl fn_102536A0(void);  // 0x102536A0
XI_API int __thiscall Gfx_DrawSpriteAlphaTested(void *self_, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);  // 0x10253F60
XI_API undefined4 __cdecl fn_102572E0(int param_1, int param_2, int param_3);  // 0x102572E0
XI_API void __cdecl fn_102573C0(uint32_t * param_1);  // 0x102573C0
XI_API short * __cdecl fn_102574F0(uint16_t * param_1, uint32_t param_2);  // 0x102574F0
XI_API short * __cdecl fn_10257550(int param_1, int param_2, int param_3);  // 0x10257550
XI_API int __cdecl fn_10257590(int param_1);  // 0x10257590
XI_API undefined4 __cdecl fn_102575B0(char * param_1);  // 0x102575B0
XI_API undefined4 __cdecl fn_102575D0(int param_1);  // 0x102575D0
XI_API int __cdecl fn_102575E0(int param_1);  // 0x102575E0
XI_API undefined4 * __cdecl fn_10257670(undefined4 * param_1, uint32_t * param_2);  // 0x10257670
XI_API undefined * __cdecl fn_102577C0(int param_1, int param_2, int param_3, uint32_t param_4);  // 0x102577C0
XI_API undefined4 __cdecl fn_10257800(uint16_t * param_1, uint32_t param_2);  // 0x10257800
XI_API undefined1 * __cdecl fn_10257830(void);  // 0x10257830
XI_API void __thiscall fn_10257840(void * self_);  // 0x10257840
XI_API void __thiscall fn_10257850(void * self_);  // 0x10257850
XI_API int __thiscall fn_10257870(void * self_, uint32_t param_1);  // 0x10257870
XI_API short * __thiscall fn_102578B0(void * self_, uint32_t param_1, uint32_t param_2);  // 0x102578B0
XI_API undefined4 __thiscall fn_10257920(void * self_, uint32_t param_1, uint32_t param_2);  // 0x10257920
XI_API undefined4 __thiscall fn_10257950(void * self_, undefined4 param_1);  // 0x10257950
XI_API undefined4 __thiscall fn_10257980(void * self_, undefined4 param_1, undefined4 param_2);  // 0x10257980
XI_API void __thiscall fn_102579B0(void * self_, uint8_t * param_1, undefined4 xi_s2);  // 0x102579B0
XI_API undefined4 __thiscall fn_10257AF0(void * self_);  // 0x10257AF0
XI_API undefined4 __thiscall fn_10257B10(void * self_);  // 0x10257B10
XI_API void __thiscall fn_10257B60(void * self_);  // 0x10257B60
XI_API void __thiscall fn_10257BA0(void * self_, int param_1, undefined4 xi_s2);  // 0x10257BA0
XI_API int __thiscall fn_10257BC0(void * self_);  // 0x10257BC0
XI_API void __thiscall fn_10257BD0(void * self_, undefined4 param_1);  // 0x10257BD0
XI_API void __thiscall fn_10257BF0(void * self_, int param_1);  // 0x10257BF0
XI_API undefined4 __thiscall fn_10257C60(void * self_);  // 0x10257C60
XI_API undefined4 __thiscall fn_10257C70(void * self_);  // 0x10257C70
XI_API undefined4 __thiscall fn_10257C80(void * self_, int * param_1, int param_2);  // 0x10257C80
XI_API undefined4 __cdecl fn_10257FE0(char * param_1);  // 0x10257FE0
XI_API undefined4 __cdecl fn_10257FF0(char * param_1);  // 0x10257FF0
XI_API undefined * __cdecl fn_10258150(int param_1, int param_2, int param_3, int param_4, int param_5);  // 0x10258150
XI_API int __cdecl fn_102581D0(char * param_1);  // 0x102581D0
XI_API undefined4 __thiscall fn_102586D0(void * self_);  // 0x102586D0
XI_API undefined4 __thiscall fn_102586F0(void * self_);  // 0x102586F0
XI_API void __thiscall fn_10258740(void * self_, void * param_1, int param_2, int param_3);  // 0x10258740
XI_API undefined4 __cdecl fn_10258990(char * param_1);  // 0x10258990
XI_API undefined4 __thiscall fn_10258AD0(void * self_);  // 0x10258AD0
XI_API undefined * __cdecl fn_10258D00(int param_1, undefined * * param_2, int param_3, int param_4);  // 0x10258D00
XI_API char * __cdecl fn_10258D70(int param_1, int param_2, int param_3, int param_4, undefined4 param_5);  // 0x10258D70
XI_API undefined * __cdecl fn_10258E50(int param_1, int param_2, undefined * * param_3, int param_4, int param_5);  // 0x10258E50
XI_API undefined1 * __cdecl fn_10258ED0(int param_1);  // 0x10258ED0
XI_API undefined * __cdecl fn_10258F00(int param_1, int param_2, int param_3);  // 0x10258F00
XI_API undefined * __cdecl fn_10258F60(int param_1, int param_2, int param_3);  // 0x10258F60
XI_API void __thiscall fn_102590E0(void * self_);  // 0x102590E0
XI_API void __thiscall fn_102590F0(void * self_);  // 0x102590F0
XI_API void __thiscall fn_10259100(void * self_, undefined4 param_1);  // 0x10259100
XI_API undefined4 __thiscall fn_10259130(void * self_);  // 0x10259130
XI_API int __stdcall fn_10259150(int param_1);  // 0x10259150
XI_API void __thiscall fn_10259180(void * self_, undefined4 param_1);  // 0x10259180
XI_API void __thiscall fn_102593F0(void * self_, char * param_1);  // 0x102593F0
XI_API void __thiscall fn_10259460(void * self_, int param_1, int param_2, int * param_3);  // 0x10259460
XI_API undefined4 __thiscall fn_10259990(void * self_, int param_1);  // 0x10259990
XI_API void __attribute__((stdcall, regparm(3))) fn_10259DE0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x10259DE0
XI_API undefined4 __cdecl fn_1025A580(int param_1, int param_2);  // 0x1025A580
XI_API undefined1 * __cdecl fn_1025A5A0(int param_1, int param_2, int param_3);  // 0x1025A5A0
XI_API undefined1 * __cdecl fn_1025A610(int param_1, int param_2, int param_3);  // 0x1025A610
XI_API undefined1 * __cdecl fn_1025A6C0(int param_1, int param_2);  // 0x1025A6C0
XI_API undefined1 * __cdecl fn_1025A6F0(int param_1);  // 0x1025A6F0
XI_API undefined * __cdecl fn_1025A730(int param_1, int param_2, int param_3);  // 0x1025A730
XI_API undefined1 * __cdecl fn_1025A760(int param_1, int param_2, int param_3, int param_4);  // 0x1025A760
XI_API void __thiscall fn_1025A800(void * self_);  // 0x1025A800
XI_API void __thiscall fn_1025A810(void * self_);  // 0x1025A810
XI_API void __attribute__((stdcall, regparm(3))) fn_1025A820(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_);  // 0x1025A820
XI_API void __thiscall fn_1025A860(void * self_, undefined4 param_1);  // 0x1025A860
XI_API int __stdcall fn_1025A890(int param_1);  // 0x1025A890
XI_API undefined4 __cdecl fn_1025A8C0(char * param_1);  // 0x1025A8C0
XI_API uint32_t __thiscall fn_1025A930(void * self_, char * param_1);  // 0x1025A930
XI_API undefined4 __thiscall fn_1025AA90(void * self_);  // 0x1025AA90
XI_API undefined4 __thiscall fn_1025AAA0(void * self_, int param_1);  // 0x1025AAA0
XI_API void __thiscall fn_1025AEF0(void * self_, char * param_1);  // 0x1025AEF0
XI_API void __thiscall fn_1025AFB0(void * self_, undefined4 param_1, int param_2, undefined4 param_3, undefined4 param_4);  // 0x1025AFB0
XI_API void __thiscall fn_1025B100(void * self_, undefined4 param_1);  // 0x1025B100
XI_API void __thiscall fn_1025B1E0(void * self_, undefined4 param_1);  // 0x1025B1E0
XI_API undefined4 __cdecl fn_1025B560(int param_1);  // 0x1025B560
XI_API undefined * __cdecl fn_1025B5A0(int param_1);  // 0x1025B5A0
XI_API int __cdecl fn_1025B5E0(undefined1 * param_1, uint32_t param_2, int param_3, uint32_t param_4);  // 0x1025B5E0
XI_API int __cdecl fn_1025B680(int param_1, uint32_t param_2);  // 0x1025B680
XI_API int __cdecl fn_1025B6C0(int param_1, uint32_t param_2, char * param_3, uint32_t * param_4, undefined4 param_5, uint32_t param_6, uint32_t param_7, undefined * param_8, undefined * param_9);  // 0x1025B6C0
XI_API undefined4 __cdecl fn_1025BB00(char * param_1);  // 0x1025BB00
XI_API undefined4 __cdecl fn_1025BB60(char * param_1);  // 0x1025BB60
XI_API size_t __cdecl fn_1025BB80(char * param_1, int param_2, size_t * param_3);  // 0x1025BB80
XI_API int __cdecl fn_1025BDE0(char * param_1, int param_2);  // 0x1025BDE0
XI_API undefined4 __cdecl fn_1025BE20(char * param_1);  // 0x1025BE20
XI_API int __cdecl fn_1025C0F0(int param_1, int param_2, undefined1 * param_3);  // 0x1025C0F0
XI_API IDirect3DDevice8 * __cdecl FontGlyphCache_GetDevice(void);  // 0x1025D750
XI_API int __cdecl FontGlyphCache_CreateTexture(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, D3DFORMAT a5, D3DPOOL a6);  // 0x1025D760
XI_API int __cdecl FontGlyphCache_CreateVertexBuffer(unsigned int a1, unsigned int a2, unsigned int a3, D3DPOOL a4);  // 0x1025D7B0
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_1025E020(undefined4 xi_eax);  // 0x1025E020
XI_API undefined4 __cdecl fn_1025E050(void);  // 0x1025E050
XI_API undefined * __cdecl fn_1025E0B0(void);  // 0x1025E0B0
XI_API void __cdecl fn_1025E0C0(void);  // 0x1025E0C0
XI_API void __cdecl fn_1025E0E0(void);  // 0x1025E0E0
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_1025E0F0(undefined4 xi_eax);  // 0x1025E0F0
XI_API void __cdecl fn_1025E210(int param_1);  // 0x1025E210
XI_API void __cdecl fn_1025E230(void);  // 0x1025E230
XI_API void __cdecl fn_1025E240(void);  // 0x1025E240
XI_API void * __thiscall fn_1025E260(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 xi_s4);  // 0x1025E260
XI_API void __thiscall fn_1025E380(void * self_);  // 0x1025E380
XI_API undefined4 __thiscall fn_1025F2F0(void * self_);  // 0x1025F2F0
XI_API void * __thiscall fn_1025FD50(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x1025FD50
XI_API void __thiscall fn_1025FE40(void * self_);  // 0x1025FE40
XI_API undefined4 __thiscall fn_1025FF10(void * self_);  // 0x1025FF10
XI_API undefined4 __cdecl fn_10260360(void);  // 0x10260360
XI_API void * __thiscall fn_10261210(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 * param_4);  // 0x10261210
XI_API void __thiscall fn_102612D0(void * self_);  // 0x102612D0
XI_API void * __thiscall fn_102659B0(void * self_, undefined4 param_1, void * param_2);  // 0x102659B0
XI_API void __cdecl fn_1026CCA5(void * param_1, undefined4 param_2);  // 0x1026CCA5
XI_API undefined4 __cdecl fn_1026CFA0(int param_1);  // 0x1026CFA0
XI_API void __cdecl fn_1026D015(int param_1);  // 0x1026D015
XI_API undefined4 __cdecl fn_1026D0CA(int param_1, undefined4 param_2);  // 0x1026D0CA
XI_API undefined4 __cdecl fn_1026D179(int param_1);  // 0x1026D179
XI_API undefined4 __cdecl fn_1026D70C(int param_1);  // 0x1026D70C
XI_API undefined4 __cdecl fn_1026D866(int param_1, undefined4 * param_2, undefined1 * param_3, int param_4, undefined1 * param_5, int param_6, int param_7);  // 0x1026D866
XI_API unsigned char __cdecl fn_1026DB24(int param_1);  // 0x1026DB24
XI_API undefined4 __cdecl fn_1026DB6D(int param_1, undefined4 param_2);  // 0x1026DB6D
XI_API void __cdecl fn_1026DB8F(int param_1);  // 0x1026DB8F
XI_API void __cdecl fn_1026DBAC(int param_1, undefined4 param_2);  // 0x1026DBAC
XI_API void __cdecl fn_1026DBCE(int param_1, undefined4 param_2);  // 0x1026DBCE
XI_API undefined4 __cdecl fn_1026DBF0(int param_1);  // 0x1026DBF0
XI_API void __cdecl fn_1026DC0D(int param_1, undefined4 param_2);  // 0x1026DC0D
XI_API void __cdecl sqxfXformTD_Init(undefined4 * param_1);  // 0x1026DF00
XI_API void __cdecl sqxfXformTD_Update(int param_1);  // 0x1026E1F0
XI_API void __cdecl sqrdLight_Init(undefined4 * param_1);  // 0x1026E2F0
XI_API void __cdecl fn_1026E3D0(int param_1);  // 0x1026E3D0
XI_API undefined4 __cdecl fn_1026E400(void);  // 0x1026E400
XI_API void __cdecl fn_1026E460(int param_1, int param_2);  // 0x1026E460
XI_API undefined4 __cdecl fn_1026E530(undefined4 * param_1);  // 0x1026E530
XI_API void __cdecl fn_1026E590(undefined4 param_1);  // 0x1026E590
XI_API void __cdecl mdl_1026E900(void);  // 0x1026E900
XI_API int __cdecl fn_1026EBF0(int param_1, undefined4 param_2);  // 0x1026EBF0
XI_API void __cdecl fn_1026EE00(void);  // 0x1026EE00
XI_API int __cdecl fn_1026EF00(int param_1, undefined4 param_2);  // 0x1026EF00
XI_API undefined4 __cdecl fn_1026F160(int param_1, int param_2);  // 0x1026F160
XI_API undefined4 __cdecl fn_1026F1B0(void);  // 0x1026F1B0
XI_API undefined4 __cdecl fn_1026F4F0(int * param_1, int * param_2);  // 0x1026F4F0
XI_API void __cdecl fn_1026F580(int param_1, undefined4 * param_2);  // 0x1026F580
XI_API void __cdecl fn_1026F7B0(int param_1, undefined4 * param_2);  // 0x1026F7B0
XI_API void __cdecl fn_1026F900(int param_1, uint32_t param_2, int param_3);  // 0x1026F900
XI_API void __cdecl fn_1026F9A0(int param_1, uint32_t param_2, int param_3);  // 0x1026F9A0
XI_API void __cdecl fn_1026F9E0(int param_1, int param_2);  // 0x1026F9E0
XI_API undefined4 __cdecl fn_1026FBB0(float * param_1);  // 0x1026FBB0
XI_API void __cdecl fn_1026FE60(int * param_1);  // 0x1026FE60
XI_API void __cdecl fn_1026FEA0(int param_1, int * param_2);  // 0x1026FEA0
XI_API void __cdecl fn_1026FEF0(int * param_1);  // 0x1026FEF0
XI_API void __cdecl fn_1026FF30(int param_1, int * param_2);  // 0x1026FF30
XI_API undefined4 __cdecl fn_1026FF80(int param_1, float * param_2);  // 0x1026FF80
XI_API void __cdecl fn_102700C0(int param_1, int param_2, undefined4 param_3);  // 0x102700C0
XI_API void __cdecl fn_102701B0(int param_1, uint32_t param_2, undefined4 * param_3, uint32_t param_4, int param_5);  // 0x102701B0
XI_API void __cdecl fn_10270240(int param_1, undefined4 * param_2, int * param_3);  // 0x10270240
XI_API void __cdecl fn_10270290(int param_1, undefined4 * param_2);  // 0x10270290
XI_API void __cdecl fn_102703C0(int param_1, uint32_t param_2, int param_3);  // 0x102703C0
XI_API void __cdecl fn_10270410(int param_1, int param_2);  // 0x10270410
XI_API void __cdecl fn_10270690(int param_1);  // 0x10270690
XI_API undefined4 __cdecl fn_10270B60(void);  // 0x10270B60
XI_API void __cdecl fn_10270EC0(void);  // 0x10270EC0
XI_API undefined4 __cdecl fn_10270F30(undefined4 param_1);  // 0x10270F30
XI_API void __cdecl fn_10271030(undefined4 param_1);  // 0x10271030
XI_API void __cdecl fn_102715F0(undefined4 param_1);  // 0x102715F0
XI_API void __cdecl fn_10271630(int param_1, int param_2, undefined4 param_3, float param_4);  // 0x10271630
XI_API void __cdecl fn_102717F0(void);  // 0x102717F0
XI_API undefined4 __cdecl scnFini(void);  // 0x10271B40
XI_API void __cdecl fn_10271B90(uint32_t param_1, int param_2);  // 0x10271B90
XI_API void __cdecl fn_10271C30(int param_1, int param_2, int param_3);  // 0x10271C30
XI_API void __cdecl fn_10271C60(undefined4 param_1, undefined4 param_2);  // 0x10271C60
XI_API void __cdecl fn_10271C80(undefined4 param_1, undefined4 param_2);  // 0x10271C80
XI_API void __cdecl fn_10271CA0(undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7, undefined4 param_8, undefined4 param_9);  // 0x10271CA0
XI_API void __cdecl fn_10271D00(void);  // 0x10271D00
XI_API void __cdecl fn_10271D70(int param_1);  // 0x10271D70
XI_API void __cdecl scnGetFlags(undefined4 * param_1);  // 0x10271E90
XI_API void __cdecl fn_10271F20(int param_1);  // 0x10271F20
XI_API void __cdecl fn_102720A0(void);  // 0x102720A0
XI_API void __cdecl fn_102725A0(int param_1, undefined4 param_2);  // 0x102725A0
XI_API void __cdecl fn_10272630(int param_1, int param_2);  // 0x10272630
XI_API void __cdecl fn_102726E0(int param_1);  // 0x102726E0
XI_API void __cdecl fn_10272750(int param_1, int param_2);  // 0x10272750
XI_API void __cdecl fn_102727F0(int param_1, undefined4 * param_2);  // 0x102727F0
XI_API void __cdecl fn_10272880(int param_1, int param_2, int param_3);  // 0x10272880
XI_API void __cdecl fn_102729E0(int param_1);  // 0x102729E0
XI_API void __cdecl fn_10272A10(int param_1);  // 0x10272A10
XI_API void __cdecl fn_10272BA0(void);  // 0x10272BA0
XI_API void __cdecl scnCameraGetInfo(int param_1, undefined4 * param_2, undefined4 * param_3);  // 0x10273370
XI_API void __cdecl fn_102733B0(int param_1);  // 0x102733B0
XI_API void __cdecl fn_10273490(int param_1, int param_2, int param_3);  // 0x10273490
XI_API void __cdecl fn_10273560(int param_1);  // 0x10273560
XI_API void __cdecl fn_102736D0(undefined4 * param_1);  // 0x102736D0
XI_API void __cdecl fn_102736E0(undefined4 param_1, int param_2);  // 0x102736E0
XI_API void __cdecl fn_10273840(undefined4 param_1, uint32_t param_2, undefined4 param_3);  // 0x10273840
XI_API void __cdecl fn_10273870(int param_1);  // 0x10273870
XI_API void __cdecl fn_102738A0(void);  // 0x102738A0
XI_API void __cdecl fn_102738D0(undefined4 * param_1);  // 0x102738D0
XI_API void __cdecl chasm_10273920(uint8_t * param_1);  // 0x10273920
XI_API undefined4 __cdecl fn_10273D60(int param_1, undefined4 param_2, undefined4 param_3);  // 0x10273D60
XI_API undefined4 __cdecl fn_10273E80(int param_1, int param_2, int param_3);  // 0x10273E80
XI_API int __cdecl fn_102741F0(int param_1, int param_2);  // 0x102741F0
XI_API undefined4 __cdecl fn_10274370(int param_1, int param_2, undefined4 * param_3, undefined4 * param_4);  // 0x10274370
XI_API void __cdecl fn_10274640(undefined4 * param_1);  // 0x10274640
XI_API void __cdecl fn_10274650(undefined4 * param_1, undefined4 * param_2);  // 0x10274650
XI_API void __cdecl fn_10274670(float * param_1, float param_2, float param_3, float param_4);  // 0x10274670
XI_API void __cdecl fn_10274890(float * param_1, float * param_2, float * param_3);  // 0x10274890
XI_API void __cdecl fn_102749C0(float * param_1, float * param_2, float * param_3);  // 0x102749C0
XI_API void __cdecl fn_10274A20(float * param_1, float * param_2, float * param_3, float param_4);  // 0x10274A20
XI_API void __cdecl PackVec4(float * param_1, float param_2, float param_3, float param_4, float param_5);  // 0x10274D40
XI_API void __cdecl sqMemInit(void);  // 0x10275150
XI_API void __cdecl fn_10275190(undefined4 * param_1);  // 0x10275190
XI_API undefined4 __cdecl fn_102752F0(int param_1);  // 0x102752F0
XI_API int __cdecl fn_10275320(int param_1);  // 0x10275320
XI_API void __attribute__((cdecl, regparm(2))) fn_102755C0(undefined4 xi_eax, undefined4 xi_edx, int param_1, int param_2);  // 0x102755C0
XI_API undefined4 * __cdecl fn_10275670(uint32_t param_1);  // 0x10275670
XI_API undefined4 * __cdecl fn_10275D40(int param_1, int param_2, undefined4 param_3, undefined4 param_4);  // 0x10275D40
XI_API undefined4 __cdecl sqgxContext_Create(void);  // 0x10276260
XI_API void __cdecl sqgxContext_Init(undefined4 * param_1);  // 0x10276290
XI_API void __cdecl fn_102778C0(int param_1, uint32_t param_2);  // 0x102778C0
XI_API void __cdecl fn_10277AB0(int * param_1);  // 0x10277AB0
XI_API void __cdecl fn_10277B70(int param_1);  // 0x10277B70
XI_API void __cdecl fn_10278E90(int param_1, int param_2, int param_3, undefined4 param_4, float param_5);  // 0x10278E90
XI_API void __cdecl fn_10279530(int param_1, undefined4 param_2);  // 0x10279530
XI_API void __cdecl fn_102796A0(int param_1, undefined4 param_2);  // 0x102796A0
XI_API undefined4 __cdecl fn_10279830(int param_1, int param_2);  // 0x10279830
XI_API undefined4 __cdecl fn_10279850(undefined4 param_1, undefined4 param_2);  // 0x10279850
XI_API undefined4 __cdecl fn_10279870(undefined4 param_1, undefined4 param_2);  // 0x10279870
XI_API undefined4 __cdecl fn_10279890(undefined4 param_1, undefined4 param_2);  // 0x10279890
XI_API undefined4 __cdecl fn_102798B0(int param_1, int param_2);  // 0x102798B0
XI_API char * __cdecl sqmdModel_GetItemName(int param_1, int param_2, int param_3);  // 0x10279AB0
XI_API undefined4 __cdecl fn_10279B40(int param_1, int param_2);  // 0x10279B40
XI_API char * __cdecl fn_10279BA0(int param_1, int param_2, int param_3);  // 0x10279BA0
XI_API undefined4 __cdecl fn_10279D50(undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x10279D50
XI_API void __cdecl fn_1027A220(int param_1, undefined4 param_2);  // 0x1027A220
XI_API void __cdecl fn_1027A250(int * param_1);  // 0x1027A250
XI_API void __cdecl fn_1027A6F0(undefined4 * param_1);  // 0x1027A6F0
XI_API undefined4 __cdecl fn_1027A700(undefined4 * param_1, undefined4 * param_2);  // 0x1027A700
XI_API void __cdecl fn_1027A720(float * param_1, float param_2, float param_3, float param_4, float param_5);  // 0x1027A720
XI_API void __cdecl fn_1027A9D0(float * param_1, float * param_2);  // 0x1027A9D0
XI_API void __cdecl fn_1027AAE0(float * param_1, float param_2, float param_3, float param_4);  // 0x1027AAE0
XI_API void __cdecl fn_1027ABA0(float * param_1, float param_2);  // 0x1027ABA0
XI_API void __cdecl fn_1027ABD0(undefined4 * param_1, float param_2);  // 0x1027ABD0
XI_API void __cdecl fn_1027AC00(undefined4 * param_1, float param_2);  // 0x1027AC00
XI_API void __cdecl fn_1027AC30(float * param_1, float * param_2, float param_3);  // 0x1027AC30
XI_API void __cdecl fn_1027AE50(undefined4 * param_1);  // 0x1027AE50
XI_API void __cdecl fn_1027AEF0(float * param_1);  // 0x1027AEF0
XI_API void __cdecl fn_1027AF20(float * param_1);  // 0x1027AF20
XI_API void __cdecl fn_1027B060(float * param_1);  // 0x1027B060
XI_API void __cdecl fn_1027B080(float * param_1, float * param_2, float * param_3);  // 0x1027B080
XI_API void __cdecl fn_1027B170(float * param_1, float * param_2, float * param_3);  // 0x1027B170
XI_API undefined4 * __cdecl fn_1027B580(void);  // 0x1027B580
XI_API void __cdecl fn_1027B5A0(undefined4 * param_1);  // 0x1027B5A0
XI_API void __cdecl fn_1027C4C0(int param_1, int param_2);  // 0x1027C4C0
XI_API int __cdecl fn_1027C980(int param_1, int param_2);  // 0x1027C980
XI_API void __cdecl fn_1027C9D0(int param_1);  // 0x1027C9D0
XI_API void __cdecl fn_1027CCF0(undefined4 param_1, undefined4 param_2, undefined4 param_3);  // 0x1027CCF0
XI_API void __cdecl fn_1027CD30(int param_1, int param_2, float * param_3, undefined4 param_4);  // 0x1027CD30
XI_API void __cdecl fn_1027CFD0(int param_1, int param_2, float * param_3, float * param_4);  // 0x1027CFD0
XI_API void __cdecl fn_1027D650(int param_1, int param_2, int param_3, undefined1 * param_4);  // 0x1027D650
XI_API void __cdecl fn_1027DAE0(undefined4 * param_1);  // 0x1027DAE0
XI_API void __cdecl fn_1027DB80(uint32_t * param_1);  // 0x1027DB80
XI_API uint32_t * __cdecl fn_1027DCC0(uint32_t * param_1, uint32_t param_2);  // 0x1027DCC0
XI_API void __cdecl fn_1027DD70(uint8_t * param_1, undefined4 xi_s2);  // 0x1027DD70
XI_API int __cdecl fn_1027DF70(int param_1);  // 0x1027DF70
XI_API void __cdecl fn_1027E060(undefined4 * param_1);  // 0x1027E060
XI_API void __cdecl fn_1027E0D0(int param_1);  // 0x1027E0D0
XI_API void __cdecl fn_1027E150(uint32_t * param_1, uint32_t param_2, int param_3);  // 0x1027E150
XI_API void __cdecl fn_1027E190(int param_1, undefined4 param_2, undefined4 param_3);  // 0x1027E190
XI_API undefined4 __cdecl fn_1027E3B0(uint8_t * param_1);  // 0x1027E3B0
XI_API undefined4 __cdecl fn_1027E440(uint8_t * param_1);  // 0x1027E440
XI_API undefined4 __cdecl fn_1027E450(uint8_t * param_1);  // 0x1027E450
XI_API void __cdecl fn_1027E490(uint8_t * param_1, int param_2);  // 0x1027E490
XI_API void __cdecl fn_1027EAF0(undefined4 param_1);  // 0x1027EAF0
XI_API void __cdecl fn_1027EB00(undefined4 * param_1);  // 0x1027EB00
XI_API void __cdecl M4fSet(undefined4 * param_1, undefined4 * param_2);  // 0x1027EB10
XI_API void __cdecl fn_1027EB50(float * param_1, float param_2, float param_3, float param_4, float param_5, float param_6, float param_7, float param_8, float param_9, float param_10, float param_11, float param_12, float param_13, float param_14, float param_15, float param_16, float param_17);  // 0x1027EB50
XI_API void __cdecl M4fTransform(float * param_1, float * param_2, float * param_3);  // 0x1027EF20
XI_API void __cdecl fn_1027EF80(float * param_1, float * param_2, float * param_3);  // 0x1027EF80
XI_API void __cdecl Mat4x4_TransformVec4(float * param_1, float * param_2, float * param_3);  // 0x1027EFE0
XI_API void __cdecl M4fTranspose(undefined4 * param_1, undefined4 * param_2);  // 0x1027F070
XI_API long double __cdecl sqMatrix4_1027FCC0(float * param_1);  // 0x1027FCC0
XI_API void __cdecl sqMatrix4_10280140(float * param_1, float * param_2, float * param_3);  // 0x10280140
XI_API void __cdecl fn_10280650(int param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x10280650
XI_API undefined4 __cdecl fn_102806B0(int param_1, float param_2, float param_3, float param_4);  // 0x102806B0
XI_API void __cdecl fn_102813B0(int param_1, float param_2);  // 0x102813B0
XI_API void __cdecl ___setargv_3(void);  // 0x10281500
XI_API int __cdecl sqgxTextureCreateFromImage(int a1, _BYTE *a2);  // 0x10281510
XI_API int __cdecl sqgxTextureAddZ(int a1, int a2, int a3);  // 0x102816E0
XI_API int __cdecl sqgxCeilLog2(int param_1);  // 0x10281810
XI_API int __cdecl sqgxTextureAdd(int a1, int a2, IDirect3DTexture8 *a3, _BYTE *a4);  // 0x10281830
XI_API void __cdecl fn_102819C0(int param_1);  // 0x102819C0
XI_API int __cdecl sqgxTextureClutAdd(int a1, int a2, int a3);  // 0x102819D0
XI_API void __cdecl fn_10281A70(int param_1);  // 0x10281A70
XI_API void __cdecl sqgxTextureGetSize(int param_1, undefined4 * param_2, undefined4 * param_3);  // 0x10281B60
XI_API void __cdecl sqgxTextureSet(int param_1);  // 0x10281C20
XI_API void __cdecl sqgxTextureDecal(undefined4 param_1);  // 0x10281C50
XI_API void __cdecl sqgxTextureClamp(undefined4 param_1);  // 0x10281C80
XI_API int __cdecl sqgxTextureClut(int a1);  // 0x10281D60
XI_API int __cdecl sqgxTextureUpdate(void);  // 0x10281DB0
XI_API int __cdecl sqgxTextureStencilClear(int a1);  // 0x10282010
XI_API int __cdecl sqgxTextureClear(float *a1, int a2);  // 0x10282030
XI_API int * __cdecl sqgxTextureDrawZ(IDirect3DSurface8 *a1);  // 0x102820E0
XI_API int * __cdecl sqgxTextureDraw(int a1, char a2);  // 0x10282190
XI_API int * __cdecl sqgxZTest(int a1);  // 0x10282350
XI_API void __cdecl sqgxAlphaDstTest(int param_1, int param_2);  // 0x102823B0
XI_API int __cdecl sqgxAlphaTest(unsigned int a1, unsigned int a2);  // 0x102823F0
XI_API int * __cdecl sqgxZMask(int a1);  // 0x10282470
XI_API int * __cdecl sqgxFBMask(int a1, int a2, int a3, int a4);  // 0x102824C0
XI_API void __cdecl fn_10282590(void);  // 0x10282590
XI_API void __cdecl fn_10282630(void);  // 0x10282630
XI_API int __cdecl fn_10282640(void);  // 0x10282640
XI_API int __cdecl sqgxBlendAdd(void);  // 0x102826C0
XI_API int __cdecl fn_102827D0(void);  // 0x102827D0
XI_API int __cdecl sqgxBlendSub(void);  // 0x10282910
XI_API void __cdecl sqgxBlendLin(float a1);  // 0x10282A30
XI_API int __cdecl sqgxBlendTrsp(void);  // 0x10282B90
XI_API int __cdecl sqgxBlendTrspInv(void);  // 0x10282CB0
XI_API int __cdecl sqgxBlendAddDestAlpha(void);  // 0x10282DC0
XI_API int __cdecl fn_10282ED0(void);  // 0x10282ED0
XI_API int __cdecl sqgxBlendMultAlpha(void);  // 0x10282FF0
XI_API int __cdecl sqgxBlendMul(void);  // 0x10283110
XI_API int __cdecl sqgxBlendOff(void);  // 0x10283220
XI_API void __cdecl fn_10283240(void);  // 0x10283240
XI_API void __cdecl ___setargv_9(void);  // 0x10283250
XI_API void __cdecl fn_10283260(undefined4 param_1, undefined4 param_2);  // 0x10283260
XI_API void __cdecl fn_10283280(undefined4 * param_1);  // 0x10283280
XI_API void __cdecl fn_10283300(undefined4 param_1);  // 0x10283300
XI_API void __cdecl sqgxColor(undefined4 * param_1);  // 0x10283330
XI_API void __cdecl sqgxAmbient(float param_1);  // 0x10283380
XI_API void __cdecl fn_102833A0(float param_1);  // 0x102833A0
XI_API void __cdecl fn_102833C0(undefined4 param_1, float param_2, undefined4 param_3);  // 0x102833C0
XI_API int __cdecl sqgxColorUpdate(void);  // 0x10283420
XI_API void __cdecl ___setargv_16(void);  // 0x10283870
XI_API void __cdecl sqgxPixelSize(float param_1, float param_2);  // 0x102838A0
XI_API void __cdecl fn_10283970(undefined4 * param_1);  // 0x10283970
XI_API int * __cdecl sqgxViewport(float a1, float a2, float a3, float a4);  // 0x10283A10
XI_API void __cdecl sqgxMatrixCamera(undefined4 * param_1);  // 0x10283D60
XI_API void __cdecl fn_10283DA0(undefined4 * param_1);  // 0x10283DA0
XI_API void __cdecl fn_10283DE0(float param_1);  // 0x10283DE0
XI_API void __cdecl sqgxMatrixShape(undefined4 * param_1);  // 0x10283E00
XI_API void __cdecl fn_10283F20(float param_1);  // 0x10283F20
XI_API int __cdecl sqgxPassAdd(int a1);  // 0x10283F70
XI_API int * __cdecl sqgxPass(int a1);  // 0x10283FC0
XI_API undefined4 __cdecl fn_10284000(float * param_1, float param_2);  // 0x10284000
XI_API int __cdecl sqgxMatrixUpdate(void);  // 0x10284410
XI_API void __cdecl ___setargv_24(void);  // 0x10284890
XI_API int __cdecl sqgxVertexShaderCreate(_DWORD *a1);  // 0x10284950
XI_API int __cdecl sqgxVertexShaderSelect(void);  // 0x10284A80
XI_API int * __cdecl sqgxShader(int a1, int a2);  // 0x10284B20
XI_API void __cdecl fn_10284EB0(void);  // 0x10284EB0
XI_API int __cdecl sqgxVertexShaderRelease(void);  // 0x10284EF0
XI_API long double __cdecl fn_10284F20(int param_1);  // 0x10284F20
XI_API void __cdecl fn_10284FD0(int * param_1, int param_2);  // 0x10284FD0
XI_API void __cdecl fn_10285000(int * param_1, int * param_2);  // 0x10285000
XI_API int __cdecl fn_102851C0(undefined4 * param_1, int param_2);  // 0x102851C0
XI_API int __cdecl fn_102852E0(undefined4 * param_1, int param_2);  // 0x102852E0
XI_API int __cdecl fn_102853F0(undefined4 * param_1, undefined4 * param_2);  // 0x102853F0
XI_API undefined4 __cdecl fn_102857D0(int param_1);  // 0x102857D0
XI_API void __attribute__((cdecl, regparm(1))) fn_102857E0(undefined4 xi_eax, int param_1, undefined4 param_2);  // 0x102857E0
XI_API undefined4 __cdecl fn_102857F0(int param_1);  // 0x102857F0
XI_API void __cdecl sqDrawableDraw(int param_1);  // 0x10285850
XI_API int __cdecl fn_10288660(int param_1);  // 0x10288660
XI_API uint32_t __cdecl fn_10288680(uint32_t param_1);  // 0x10288680
XI_API undefined4 __cdecl fn_10288BB0(int param_1, uint32_t param_2);  // 0x10288BB0
XI_API char * __cdecl fn_10288C20(int param_1, uint32_t param_2);  // 0x10288C20
XI_API void __cdecl fn_10288DC0(int param_1);  // 0x10288DC0
XI_API void __cdecl fn_10288E00(int param_1);  // 0x10288E00
XI_API void __cdecl fn_102894E0(int param_1, int param_2, uint32_t param_3);  // 0x102894E0
XI_API void __cdecl fn_10289620(undefined4 * param_1);  // 0x10289620
XI_API void __cdecl fn_102896A0(undefined4 * param_1);  // 0x102896A0
XI_API void __cdecl fn_10289B40(undefined4 * param_1);  // 0x10289B40
XI_API void __cdecl fn_10289BA0(int param_1);  // 0x10289BA0
XI_API int __cdecl sqopOpcodeGet(int * param_1, undefined4 * param_2);  // 0x10289EA0
XI_API int __cdecl sqopTrackSetIndex(int * param_1, int param_2);  // 0x10289EF0
XI_API undefined4 __cdecl sqopTrackAdvance(int * param_1);  // 0x10289F10
XI_API void __cdecl fn_10289F50(undefined4 * param_1);  // 0x10289F50
XI_API void __attribute__((cdecl, regparm(1))) fn_1028A0B0(undefined4 xi_eax, int param_1);  // 0x1028A0B0
XI_API undefined4 __cdecl fn_1028A0E0(void);  // 0x1028A0E0
XI_API undefined4 * __cdecl fn_1028A0F0(undefined4 * param_1, int param_2, int param_3, undefined4 * param_4, int param_5);  // 0x1028A0F0
XI_API int __cdecl sqScript_TokenEquals(char * param_1, char * param_2);  // 0x1028A150
XI_API int * __cdecl sqgxShapeIndexBufferBegin(void);  // 0x1028AA70
XI_API int __cdecl fn_1028AB50(void);  // 0x1028AB50
XI_API int * __cdecl sqgxShapeBegin(void);  // 0x1028AFE0
XI_API undefined4 * __cdecl fn_1028D6D0(void);  // 0x1028D6D0
XI_API void __cdecl fn_1028D6F0(undefined4 * param_1);  // 0x1028D6F0
XI_API undefined4 __cdecl fn_1028D870(undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5);  // 0x1028D870
XI_API long double __cdecl fn_1028D990(int param_1);  // 0x1028D990
XI_API void __cdecl fn_1028D9A0(undefined4 * param_1);  // 0x1028D9A0
XI_API void __cdecl fn_1028D9C0(undefined4 * param_1, undefined4 * param_2);  // 0x1028D9C0
XI_API void __attribute__((cdecl, regparm(1))) fn_1028DA20(undefined4 xi_eax, float * param_1, float * param_2, int param_3);  // 0x1028DA20
XI_API void __cdecl fn_1028DBB0(float * param_1, float * param_2);  // 0x1028DBB0
XI_API undefined4 * __cdecl fn_1028DC00(void);  // 0x1028DC00
XI_API void __cdecl fn_1028DC20(undefined4 * param_1);  // 0x1028DC20
XI_API undefined4 __cdecl fn_1028DE10(int param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);  // 0x1028DE10
XI_API void __cdecl fn_1028E1B0(int param_1);  // 0x1028E1B0
XI_API void __cdecl fn_1028E350(undefined4 * param_1);  // 0x1028E350
XI_API void __cdecl fn_1028EBF0(int param_1, int param_2, float param_3);  // 0x1028EBF0
XI_API void __cdecl fn_1028EC30(int param_1, int param_2, undefined4 param_3);  // 0x1028EC30
XI_API void __cdecl fn_1028EC50(int param_1, int param_2, undefined4 param_3);  // 0x1028EC50
XI_API void __cdecl fn_1028ECC0(int param_1, int param_2, undefined4 param_3);  // 0x1028ECC0
XI_API void __cdecl fn_1028ECE0(int param_1, int param_2, undefined4 param_3);  // 0x1028ECE0
XI_API int __cdecl fn_1028ED00(int param_1, undefined4 param_2, undefined4 param_3);  // 0x1028ED00
XI_API int __cdecl fn_1028ED50(int param_1, undefined4 param_2, undefined4 param_3);  // 0x1028ED50
XI_API int __cdecl fn_1028EDE0(int param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, int param_5);  // 0x1028EDE0
XI_API void __cdecl fn_1028EFB0(undefined4 * param_1);  // 0x1028EFB0
XI_API int __cdecl fn_1028F130(int param_1, undefined4 param_2);  // 0x1028F130
XI_API void __cdecl fn_1028F220(int param_1, int param_2, undefined4 param_3);  // 0x1028F220
XI_API void __cdecl fn_1028F3C0(int param_1, int param_2, undefined4 param_3);  // 0x1028F3C0
XI_API int __cdecl fn_1028F430(int param_1, uint32_t param_2);  // 0x1028F430
XI_API void __cdecl fn_1028F4A0(int param_1, int param_2);  // 0x1028F4A0
XI_API void __cdecl fn_1028F4C0(undefined4 param_1, int * param_2);  // 0x1028F4C0
XI_API void __cdecl sqsnScene_1028F5F0(int param_1, int param_2, int param_3);  // 0x1028F5F0
XI_API void __cdecl fn_1028F6E0(int * param_1, int param_2);  // 0x1028F6E0
XI_API void __cdecl sqsnSceneDraw(int a1);  // 0x1028F8C0
XI_API void __cdecl sqsnSceneLayerDraw(int * param_1, int param_2);  // 0x1028FDD0
XI_API void __cdecl fn_1028FF80(int param_1);  // 0x1028FF80
XI_API void __cdecl sqsnSceneLayerDrawShadowed(int * param_1, int param_2);  // 0x1028FFF0
XI_API void __cdecl sqsnSceneLayerSnapShotComposite(int param_1, int param_2);  // 0x102902C0
XI_API void __cdecl fn_10290430(void);  // 0x10290430
XI_API void __cdecl sqsnSceneLayerSort(int param_1, int param_2);  // 0x10290440
XI_API void __cdecl fn_10290610(int param_1, int param_2, int param_3, int param_4, int param_5);  // 0x10290610
XI_API void __cdecl fn_102907C0(int param_1, int param_2, int param_3);  // 0x102907C0
XI_API void __cdecl fn_10290840(int param_1, int param_2, int param_3);  // 0x10290840
XI_API void __cdecl fn_10290910(undefined4 * param_1);  // 0x10290910
XI_API int * __cdecl sqsdGeneric_BakeOntoShape(int param_1, int * param_2);  // 0x10290FC0
XI_API void __cdecl fn_10291110(undefined4 * param_1);  // 0x10291110
XI_API void __cdecl sqrdCamera_Apply(int param_1);  // 0x102911E0
XI_API void __cdecl fn_10291290(int param_1);  // 0x10291290
XI_API void __cdecl fn_102912D0(int param_1);  // 0x102912D0
XI_API void __cdecl fn_102912F0(int param_1);  // 0x102912F0
XI_API undefined4 * __cdecl fn_10291360(void);  // 0x10291360
XI_API void __cdecl fn_10291390(undefined4 * param_1);  // 0x10291390
XI_API void __cdecl fn_10291460(void);  // 0x10291460
XI_API void __cdecl fn_102915B0(int param_1, undefined4 * param_2);  // 0x102915B0
XI_API void __cdecl fn_102915E0(int param_1, undefined4 * param_2);  // 0x102915E0
XI_API void __cdecl fn_102915F0(int param_1, undefined4 * param_2);  // 0x102915F0
XI_API void __cdecl fn_102916D0(int param_1, int * param_2);  // 0x102916D0
XI_API void __cdecl fn_10291780(int param_1, float param_2);  // 0x10291780
XI_API void __cdecl fn_10291790(int param_1, float param_2);  // 0x10291790
XI_API undefined4 * __cdecl fn_10291B70(void);  // 0x10291B70
XI_API void __cdecl fn_10291BA0(undefined4 * param_1);  // 0x10291BA0
XI_API void __cdecl fn_10291C50(void);  // 0x10291C50
XI_API void __cdecl fn_10291F00(int param_1, undefined4 param_2);  // 0x10291F00
XI_API void __cdecl fn_10291F10(int param_1, int param_2);  // 0x10291F10
XI_API void __cdecl fn_10292490(int param_1, uint32_t param_2);  // 0x10292490
XI_API undefined4 * __cdecl fn_102924F0(void);  // 0x102924F0
XI_API void __cdecl fn_10292520(undefined4 * param_1);  // 0x10292520
XI_API void __cdecl fn_10293000(int param_1, int param_2);  // 0x10293000
XI_API undefined4 * __cdecl fn_10293F80(void);  // 0x10293F80
XI_API void __cdecl fn_10293FA0(undefined4 * param_1);  // 0x10293FA0
XI_API void __cdecl fn_102944E0(int param_1, int param_2);  // 0x102944E0
XI_API undefined4 * __cdecl fn_102957B0(void);  // 0x102957B0
XI_API void __cdecl fn_102957D0(undefined4 * param_1);  // 0x102957D0
XI_API undefined4 * __cdecl fn_10295E70(void);  // 0x10295E70
XI_API void __cdecl fn_10295E90(undefined4 * param_1);  // 0x10295E90
XI_API undefined4 __cdecl fn_102960A0(int param_1, undefined4 param_2);  // 0x102960A0
XI_API void __cdecl fn_10296410(float * param_1, float param_2, float param_3, float param_4, float param_5, float param_6, float param_7, float param_8, float param_9, float param_10);  // 0x10296410
XI_API long double __cdecl fn_102967E0(float * param_1);  // 0x102967E0
XI_API int __cdecl sqsnShadowVolumeSetPass(int a1);  // 0x10297CC0
XI_API int __cdecl sqgxShadowStencilEnable(int a1);  // 0x10297EC0
XI_API int __cdecl sqsnShadowVolumeBeginVB(void);  // 0x102981E0
XI_API void __cdecl sqgxShadowPlaneMatrix(float * param_1);  // 0x10298350
XI_API undefined4 * __cdecl fn_102984B0(void);  // 0x102984B0
XI_API void __cdecl fn_10298570(undefined4 * param_1);  // 0x10298570
XI_API undefined4 * __cdecl fn_10298B30(void);  // 0x10298B30
XI_API void __cdecl fn_10298B90(undefined4 * param_1);  // 0x10298B90
XI_API void __cdecl fn_10298C50(int param_1);  // 0x10298C50
XI_API int __cdecl fn_10298E10(int param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5);  // 0x10298E10
XI_API void __cdecl fn_10299090(int param_1, int * param_2);  // 0x10299090
XI_API int __cdecl sqsdSampling_BakeOntoShape(int param_1, int param_2);  // 0x10299250
XI_API void __cdecl fn_102993B0(int * param_1, int * param_2);  // 0x102993B0
XI_API int * __cdecl sqsdAniso_BakeOntoShape(int param_1, int * param_2);  // 0x10299610
XI_API void __cdecl fn_102997A0(int param_1, int * param_2);  // 0x102997A0
XI_API undefined4 __cdecl sqsdCartoon_BakeOntoShape_Stub(void);  // 0x10299840
XI_API undefined4 __cdecl fn_102999A0(int param_1, int param_2);  // 0x102999A0
XI_API int * __cdecl sqsdGlow_BakeOntoShape(int param_1, int * param_2);  // 0x10299AB0
XI_API int __cdecl fn_10299D60(undefined4 * param_1, undefined4 * param_2, int param_3);  // 0x10299D60
XI_API undefined4 * __cdecl fn_10299FE0(void);  // 0x10299FE0
XI_API void __cdecl fn_1029A000(undefined4 * param_1);  // 0x1029A000
XI_API void __cdecl fn_1029A160(int param_1, int param_2, undefined4 * param_3, undefined4 * param_4, undefined4 param_5, uint32_t param_6);  // 0x1029A160
XI_API void __cdecl fn_1029A230(int param_1, undefined4 param_2);  // 0x1029A230
XI_API undefined4 __cdecl fn_1029A330(int param_1, int param_2);  // 0x1029A330
XI_API int __cdecl fn_1029A380(int param_1, int param_2);  // 0x1029A380
XI_API int __cdecl sqskSkeleton_GetNodeByIndex(int param_1, int param_2);  // 0x1029A420
XI_API void __cdecl fn_1029A450(int param_1);  // 0x1029A450
XI_API void __cdecl fn_1029A6F0(int param_1);  // 0x1029A6F0
XI_API undefined4 * __cdecl fn_1029B100(void);  // 0x1029B100
XI_API void __cdecl fn_1029B120(undefined4 * param_1);  // 0x1029B120
XI_API void __cdecl fn_1029B310(int * param_1, int * param_2, int param_3);  // 0x1029B310
XI_API void __cdecl fn_1029B5C0(void);  // 0x1029B5C0
XI_API _DWORD * __cdecl sqgxDlistEnd(void);  // 0x1029B620
XI_API void __cdecl sqgxDlistDraw(IDirect3DVertexBuffer8 **a1);  // 0x1029B9D0
XI_API void __cdecl fn_1029BB50(void);  // 0x1029BB50
XI_API void __cdecl fn_1029BCD0(int param_1);  // 0x1029BCD0
XI_API void __cdecl fn_1029BD40(float param_1);  // 0x1029BD40
XI_API void __cdecl fn_1029C180(void);  // 0x1029C180
XI_API void __cdecl fn_1029C1B0(undefined4 param_1, undefined4 * param_2, float param_3);  // 0x1029C1B0
XI_API void __cdecl fn_1029C460(undefined4 param_1);  // 0x1029C460
XI_API undefined4 __cdecl fn_102A1FA0(int param_1);  // 0x102A1FA0
XI_API undefined4 * __cdecl fn_102A2000(void);  // 0x102A2000
XI_API void __cdecl fn_102A2020(undefined4 * param_1);  // 0x102A2020
XI_API void __cdecl fn_102A2130(int param_1, int param_2);  // 0x102A2130
XI_API undefined4 __cdecl fn_102A2160(int param_1, int param_2);  // 0x102A2160
XI_API undefined4 __cdecl fn_102A2190(int param_1, undefined4 param_2);  // 0x102A2190
XI_API void __cdecl sqsnShadowVolumeDraw(int a1, int a2, int a3);  // 0x102A21F0
XI_API void __cdecl sqsnPlanarShadowDraw(int a1, int a2);  // 0x102A2530
XI_API void __cdecl fn_102A2720(int param_1, int param_2);  // 0x102A2720
XI_API void __cdecl fn_102A2A30(int param_1, int param_2);  // 0x102A2A30
XI_API void __cdecl fn_102A2A50(undefined4 param_1, int param_2, int param_3);  // 0x102A2A50
XI_API void __cdecl fn_102A2C00(int param_1, int * param_2, int param_3, int param_4);  // 0x102A2C00
XI_API void __cdecl fn_102A3390(undefined4 * param_1);  // 0x102A3390
XI_API void __cdecl fn_102A34D0(int param_1);  // 0x102A34D0
XI_API void __cdecl fn_102A3540(float param_1);  // 0x102A3540
XI_API void __cdecl fn_102A3760(void);  // 0x102A3760
XI_API void __cdecl fn_102A3770(void);  // 0x102A3770
XI_API undefined4 * __cdecl fn_102A9520(void);  // 0x102A9520
XI_API void __cdecl fn_102A9550(undefined4 * param_1);  // 0x102A9550
XI_API void __cdecl fn_102A9EF0(int param_1);  // 0x102A9EF0
XI_API void __cdecl fn_102A9F30(int param_1);  // 0x102A9F30
XI_API void __cdecl fn_102A9FB0(int param_1, int param_2);  // 0x102A9FB0
XI_API undefined4 * __cdecl fn_102AA2D0(void);  // 0x102AA2D0
XI_API void __cdecl fn_102AA2F0(undefined4 * param_1);  // 0x102AA2F0
XI_API undefined4 __cdecl fn_102ABEB0(void);  // 0x102ABEB0
XI_API void __cdecl fn_102ABEC0(int param_1, undefined * param_2, undefined4 param_3);  // 0x102ABEC0
XI_API undefined4 * __cdecl fn_102ACB60(void);  // 0x102ACB60
XI_API void __cdecl fn_102ACB80(undefined4 * param_1);  // 0x102ACB80
XI_API undefined4 * __cdecl fn_102ACF20(void);  // 0x102ACF20
XI_API void __cdecl fn_102ACF40(undefined4 * param_1);  // 0x102ACF40
XI_API void __cdecl fn_102AD990(int param_1);  // 0x102AD990
XI_API void __cdecl fn_102AEC50(undefined4 * param_1);  // 0x102AEC50
XI_API undefined4 * __cdecl fn_102AEE90(void);  // 0x102AEE90
XI_API void __cdecl fn_102AEEB0(undefined4 * param_1);  // 0x102AEEB0
XI_API void __cdecl fn_102AF170(undefined4 * param_1);  // 0x102AF170
XI_API undefined4 * __cdecl sqinCluster_Create(void);  // 0x102AF540
XI_API void __cdecl sqinCluster_Init(undefined4 * param_1);  // 0x102AF570
XI_API void __cdecl fn_102B2CA0(int param_1, int * param_2);  // 0x102B2CA0
