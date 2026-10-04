// gfx_orig.hpp -- calls into the game's own code (not reconstructed here), by address.
#pragma once
#include "gfx_common.hpp"
#define XI_ORIG static inline __attribute__((always_inline))
namespace orig {
XI_ORIG int CApp_Constructor(int a0, int a1, HINSTANCE a2, int a3, void * a4, void * a5) { return reinterpret_cast<int (__thiscall *)(int, int, HINSTANCE, int, void *, void *)>(XI_GAME(0x10001240u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int App_ForceForegroundWindow(HWND a0) { return reinterpret_cast<int (__cdecl *)(HWND)>(XI_GAME(0x100014A0u))(a0); }
XI_ORIG char App_PreTranslateInputMessage(int a0, unsigned int a1, unsigned int a2, unsigned int a3) { return reinterpret_cast<char (__cdecl *)(int, unsigned int, unsigned int, unsigned int)>(XI_GAME(0x100016C0u))(a0, a1, a2, a3); }
XI_ORIG int App_RenderFrameMaskedFPU(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10001B50u))(a0); }
XI_ORIG char App_CreateInputSystem(HINSTANCE a0) { return reinterpret_cast<char (__stdcall *)(HINSTANCE)>(XI_GAME(0x10001BB0u))(a0); }
XI_ORIG BOOL CApp_CopyTextToClipboard(void * a0, void * a1) { return reinterpret_cast<BOOL (__thiscall *)(void *, void *)>(XI_GAME(0x10001CC0u))(a0, a1); }
XI_ORIG char App_SetCommandLine(void * a0) { return reinterpret_cast<char (__cdecl *)(void *)>(XI_GAME(0x10001E00u))(a0); }
XI_ORIG void App_FreeCommandLine() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10001E10u))(); }
XI_ORIG char sub_10001E70(void * a0) { return reinterpret_cast<char (__cdecl *)(void *)>(XI_GAME(0x10001E70u))(a0); }
XI_ORIG char Gfx_ProbeDeviceCaps(int a0, void * a1, void * a2) { return reinterpret_cast<char (__cdecl *)(int, void *, void *)>(XI_GAME(0x10002130u))(a0, a1, a2); }
XI_ORIG unsigned int sub_10002440(int a0) { return reinterpret_cast<unsigned int (__cdecl *)(int)>(XI_GAME(0x10002440u))(a0); }
XI_ORIG char App_DetectDat11() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x10002460u))(); }
XI_ORIG char App_CreateConfig() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x10002B20u))(); }
XI_ORIG int App_DestroyConfig() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10002BC0u))(); }
XI_ORIG CDx * Gfx_SetGammaRGB(int a0, int a1, int a2) { return reinterpret_cast<CDx * (__cdecl *)(int, int, int)>(XI_GAME(0x10002C00u))(a0, a1, a2); }
XI_ORIG double sub_10002D80() { return reinterpret_cast<double (__cdecl *)()>(XI_GAME(0x10002D80u))(); }
XI_ORIG std::codecvt_base * sub_10002E60(int a0) { return reinterpret_cast<std::codecvt_base * (__cdecl *)(int)>(XI_GAME(0x10002E60u))(a0); }
XI_ORIG char Gfx_CreateD3DHolder(int a0, int a1, int a2, char a3, char a4) { return reinterpret_cast<char (__cdecl *)(int, int, int, char, char)>(XI_GAME(0x10002F20u))(a0, a1, a2, a3, a4); }
XI_ORIG int Gfx_DestroyD3DHolder() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10003040u))(); }
XI_ORIG _DWORD * D3DHolder_Constructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10003070u))(a0); }
XI_ORIG char D3DHolder_Initialize(void * a0, int a1, int a2, int a3, char a4, char a5) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int, char, char)>(XI_GAME(0x10003110u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG char Gfx_TryCreateDeviceForMode(void * a0, int a1, int a2, int a3, char a4, char a5, int a6, int a7) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int, char, char, int, int)>(XI_GAME(0x100033C0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG bool D3DHolder_CheckDepthFormat(void * a0, int a1, int a2, int a3) { return reinterpret_cast<bool (__thiscall *)(void *, int, int, int)>(XI_GAME(0x10003460u))(a0, a1, a2, a3); }
XI_ORIG char Gfx_CreateDevice(void * a0, int a1, int a2, int a3, int a4, int a5, char a6, char a7, int a8) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int, int, int, char, char, int)>(XI_GAME(0x100034B0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG int nullsub_71() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100035A0u))(); }
XI_ORIG int sub_100035F0(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x100035F0u))(a0); }
XI_ORIG char sub_10003650(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10003650u))(a0); }
XI_ORIG void Debug_DrawWorldText(void * a0, void * a1, int a2) { reinterpret_cast<void (__stdcall *)(void *, void *, int)>(XI_GAME(0x10003D70u))(a0, a1, a2); }
XI_ORIG int sub_10005A40(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10005A40u))(a0); }
XI_ORIG char sub_10008830(void * a0, unsigned __int8 a1) { return reinterpret_cast<char (__thiscall *)(void *, unsigned __int8)>(XI_GAME(0x10008830u))(a0, a1); }
XI_ORIG unsigned int sub_100088F0(void * a0) { return reinterpret_cast<unsigned int (__thiscall *)(void *)>(XI_GAME(0x100088F0u))(a0); }
XI_ORIG CDx * CDx_Constructor(void * a0, void * a1, void * a2) { return reinterpret_cast<CDx * (__thiscall *)(void *, void *, void *)>(XI_GAME(0x10008F90u))(a0, a1, a2); }
XI_ORIG int CDx_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10009120u))(a0); }
XI_ORIG char CDx_Initialize(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100091B0u))(a0); }
XI_ORIG char CDx_SetDefaultRenderTargets(void * a0, int a1, int a2, int a3) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int)>(XI_GAME(0x10009620u))(a0, a1, a2, a3); }
XI_ORIG int CDx_SetClearParams(void * a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int)>(XI_GAME(0x10009640u))(a0, a1, a2, a3, a4); }
XI_ORIG int CDx_SetClearColor(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10009670u))(a0, a1); }
XI_ORIG int CDx_EndClearBegin(void * a0, int a1, int a2, int a3, int a4, int a5, int a6) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int, int, int)>(XI_GAME(0x100096B0u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG bool CDx_TestCooperativeLevel(void * a0) { return reinterpret_cast<bool (__thiscall *)(void *)>(XI_GAME(0x10009710u))(a0); }
XI_ORIG int CDx_Present() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10009720u))(); }
XI_ORIG int CDx_BeginScene(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10009730u))(a0); }
XI_ORIG int CDx_EndScene(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10009740u))(a0); }
XI_ORIG HCURSOR sub_10009750(HCURSOR a0) { return reinterpret_cast<HCURSOR (__stdcall *)(HCURSOR)>(XI_GAME(0x10009750u))(a0); }
XI_ORIG BOOL SetCursorPos(int a0, int a1) { return reinterpret_cast<BOOL (__stdcall *)(int, int)>(XI_GAME(0x10009770u))(a0, a1); }
XI_ORIG unsigned __int8 CDx_ShowCursor(void * a0, unsigned __int8 a1) { return reinterpret_cast<unsigned __int8 (__thiscall *)(void *, unsigned __int8)>(XI_GAME(0x10009780u))(a0, a1); }
XI_ORIG HCURSOR CDx_SetCursorShape(void * a0, int a1, int a2) { return reinterpret_cast<HCURSOR (__thiscall *)(void *, int, int)>(XI_GAME(0x100097F0u))(a0, a1, a2); }
XI_ORIG char sub_10009870(int a0, int a1, void * a2) { return reinterpret_cast<char (__thiscall *)(int, int, void *)>(XI_GAME(0x10009870u))(a0, a1, a2); }
XI_ORIG char sub_100098E0(void * a0, int a1, void * a2, char a3) { return reinterpret_cast<char (__thiscall *)(void *, int, void *, char)>(XI_GAME(0x100098E0u))(a0, a1, a2, a3); }
XI_ORIG int CDx_SetDefaultRenderState(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10009920u))(a0); }
XI_ORIG int sub_10009D20(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10009D20u))(a0); }
XI_ORIG int CDx_SetViewport(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x10009DC0u))(a0, a1); }
XI_ORIG unsigned __int8 * CDx_GetRenderTargetTop(void * a0) { return reinterpret_cast<unsigned __int8 * (__thiscall *)(void *)>(XI_GAME(0x10009E00u))(a0); }
XI_ORIG int CDx_PushRenderTarget(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(XI_GAME(0x10009E20u))(a0, a1, a2, a3); }
XI_ORIG int sub_10009EA0(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(XI_GAME(0x10009EA0u))(a0, a1, a2, a3); }
XI_ORIG int sub_10009EE0(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(XI_GAME(0x10009EE0u))(a0, a1, a2, a3); }
XI_ORIG int CDx_PushRenderTargetTexture(void * a0, int a1, int a2, int a3, void * a4, void * a5, int a6, int a7) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, void *, void *, int, int)>(XI_GAME(0x10009F20u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG int CDx_SetTexture(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1000A1D0u))(a0, a1, a2); }
XI_ORIG int sub_1000A200(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1000A200u))(a0, a1, a2); }
XI_ORIG int sub_1000A230(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1000A230u))(a0, a1, a2); }
XI_ORIG int CDx_PopRenderTargetDefault(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1000A290u))(a0, a1, a2); }
XI_ORIG int CDx_PopRenderTarget(int a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<int (__thiscall *)(int, int, int, int, int)>(XI_GAME(0x1000A2B0u))(a0, a1, a2, a3, a4); }
XI_ORIG int sub_1000A390(void * a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int)>(XI_GAME(0x1000A390u))(a0, a1, a2, a3, a4); }
XI_ORIG int CDx_DrawScreenQuadAtDepth(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1000A3B0u))(a0, a1, a2); }
XI_ORIG int CDx_DrawTextureToViewport(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(XI_GAME(0x1000A4F0u))(a0, a1, a2, a3); }
XI_ORIG int CDx_DrawTexturedScreenQuad(void * a0, int a1, int a2, int a3, int a4, int a5) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int, int)>(XI_GAME(0x1000A5A0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG _DWORD * CDx_CreateTexture(void * a0, unsigned __int16 a1, unsigned __int16 a2, void * a3, int a4, int a5) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, unsigned __int16, unsigned __int16, void *, int, int)>(XI_GAME(0x1000A6E0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG char CDx_PushRenderTargetRet1(void * a0, int a1, int a2, int a3) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int)>(XI_GAME(0x1000A800u))(a0, a1, a2, a3); }
XI_ORIG int CDx_DrawFadeQuad(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1000A820u))(a0, a1, a2); }
XI_ORIG int Post_DepthBlurComposite(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1000AAB0u))(a0, a1, a2); }
XI_ORIG int sub_1000ABE0(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1000ABE0u))(a0, a1, a2); }
XI_ORIG int CDx_CreateDepthSurfaceLikeMain(void * a0, unsigned __int16 a1, unsigned __int16 a2) { return reinterpret_cast<int (__thiscall *)(void *, unsigned __int16, unsigned __int16)>(XI_GAME(0x1000AD70u))(a0, a1, a2); }
XI_ORIG char CDx_StepFadeCounter(void * a0, float a1, float a2, int a3) { return reinterpret_cast<char (__thiscall *)(void *, float, float, int)>(XI_GAME(0x1000AE40u))(a0, a1, a2, a3); }
XI_ORIG void nullsub_36() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1000BB10u))(); }
XI_ORIG void nullsub_37() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1000BB20u))(); }
XI_ORIG char CDx_DrawStereoOddFrameOverlay(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(XI_GAME(0x1000BB90u))(a0, a1); }
XI_ORIG int CDx_SetTransform(void * a0, int a1, void * a2) { return reinterpret_cast<int (__thiscall *)(void *, int, void *)>(XI_GAME(0x1000BD50u))(a0, a1, a2); }
XI_ORIG int CDx_GetTransformCached(void * a0, int a1, void * a2) { return reinterpret_cast<int (__thiscall *)(void *, int, void *)>(XI_GAME(0x1000BDB0u))(a0, a1, a2); }
XI_ORIG int sub_1000BE10(int a0, int a1) { return reinterpret_cast<int (__thiscall *)(int, int)>(XI_GAME(0x1000BE10u))(a0, a1); }
XI_ORIG char Light_ApplyTimeOfDayTint(void * a0, void * a1) { return reinterpret_cast<char (__thiscall *)(void *, void *)>(XI_GAME(0x1000BE50u))(a0, a1); }
XI_ORIG int CDx_SetLight(void * a0, int a1, void * a2) { return reinterpret_cast<int (__thiscall *)(void *, int, void *)>(XI_GAME(0x1000BF30u))(a0, a1, a2); }
XI_ORIG int CDx_CompositeSceneToBackBuffer(void * a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int)>(XI_GAME(0x1000C0B0u))(a0, a1, a2, a3, a4); }
XI_ORIG int CDx_CreateStereoResources(void * a0, unsigned __int16 a1, unsigned __int16 a2) { return reinterpret_cast<int (__thiscall *)(void *, unsigned __int16, unsigned __int16)>(XI_GAME(0x1000C6E0u))(a0, a1, a2); }
XI_ORIG int CDx_ReleaseStereoResources(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1000C7C0u))(a0); }
XI_ORIG int CDx_CreateRampTexture1x16(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1000C850u))(a0, a1); }
XI_ORIG char CDx_CreateRampTextures1024x2(void * a0, void * a1, int a2) { return reinterpret_cast<char (__thiscall *)(void *, void *, int)>(XI_GAME(0x1000C900u))(a0, a1, a2); }
XI_ORIG char CDx_GetStereoSecondEyeTarget(void * a0, int a1, int a2) { return reinterpret_cast<char (__thiscall *)(void *, int, int)>(XI_GAME(0x1000CA00u))(a0, a1, a2); }
XI_ORIG int CDx_DrawPrimitive(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(XI_GAME(0x1000CB20u))(a0, a1, a2, a3); }
XI_ORIG int CDx_DrawIndexedPrimitive(void * a0, int a1, int a2, int a3, int a4, int a5) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int, int)>(XI_GAME(0x1000CC00u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int CDx_DrawPrimitiveUP(void * a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int)>(XI_GAME(0x1000CD00u))(a0, a1, a2, a3, a4); }
XI_ORIG void CDx_ApplyStereoEyeProjection(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1000CF20u))(a0); }
XI_ORIG int CDx_RestoreProjection(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1000CF90u))(a0); }
XI_ORIG _DWORD * CAcc_Constructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1000CFB0u))(a0); }
XI_ORIG char j_CEnv_UpdateDualTargetMode(int a0, int a1) { return reinterpret_cast<char (__thiscall *)(int, int)>(XI_GAME(0x1000D3F0u))(a0, a1); }
XI_ORIG void nullsub_38() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1000D620u))(); }
XI_ORIG char CEnv_IsStereoMode(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1000D660u))(a0); }
XI_ORIG int App_RegisterSystemDats() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10010370u))(); }
XI_ORIG int sub_10010520() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10010520u))(); }
XI_ORIG char * App_Constructor(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10010700u))(a0); }
XI_ORIG __int16 App_SetSeqStep(void * a0, __int16 a1) { return reinterpret_cast<__int16 (__thiscall *)(void *, __int16)>(XI_GAME(0x10010CE0u))(a0, a1); }
XI_ORIG void App_AdvanceSeqStep(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10010D00u))(a0); }
XI_ORIG char App_InitScene(int a0, int a1) { return reinterpret_cast<char (__thiscall *)(int, int)>(XI_GAME(0x10010D20u))(a0, a1); }
XI_ORIG int Post_Afterimage() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10012F40u))(); }
XI_ORIG int Post_FlashAndZoomFeedback(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100132B0u))(a0); }
XI_ORIG char sub_100139E0(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100139E0u))(a0); }
XI_ORIG bool fn_10013AC0() { return reinterpret_cast<bool (__cdecl *)()>(XI_GAME(0x10013AC0u))(); }
XI_ORIG char App_StepGameState5Connect(int a0, int a1) { return reinterpret_cast<char (__thiscall *)(int, int)>(XI_GAME(0x10013AD0u))(a0, a1); }
XI_ORIG char App_StepGameState6(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10013DF0u))(a0); }
XI_ORIG char App_StepGameState7(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10013F10u))(a0); }
XI_ORIG char App_StepGameState8(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10014030u))(a0); }
XI_ORIG bool App_StepGameState9() { return reinterpret_cast<bool (__cdecl *)()>(XI_GAME(0x10014120u))(); }
XI_ORIG char sub_10014170(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10014170u))(a0); }
XI_ORIG char sub_100149B0(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100149B0u))(a0); }
XI_ORIG bool sub_10014A20() { return reinterpret_cast<bool (__cdecl *)()>(XI_GAME(0x10014A20u))(); }
XI_ORIG char sub_10014A30(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x10014A30u))(a0); }
XI_ORIG char sub_10014AD0(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10014AD0u))(a0); }
XI_ORIG bool sub_10014C10() { return reinterpret_cast<bool (__cdecl *)()>(XI_GAME(0x10014C10u))(); }
XI_ORIG double Camera_GetFrameScale() { return reinterpret_cast<double (__cdecl *)()>(XI_GAME(0x10014CF0u))(); }
XI_ORIG int GetFrameCounter() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10014D50u))(); }
XI_ORIG double Camera_GetFloat2DC() { return reinterpret_cast<double (__cdecl *)()>(XI_GAME(0x10014D70u))(); }
XI_ORIG double Camera_GetFloat2E0() { return reinterpret_cast<double (__cdecl *)()>(XI_GAME(0x10014D80u))(); }
XI_ORIG unsigned int Camera_SetClearColorRGB(unsigned __int8 a0, unsigned __int8 a1, unsigned __int8 a2) { return reinterpret_cast<unsigned int (__cdecl *)(unsigned __int8, unsigned __int8, unsigned __int8)>(XI_GAME(0x10014D90u))(a0, a1, a2); }
XI_ORIG int sub_10014EF0(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10014EF0u))(a0); }
XI_ORIG int sub_10014F20(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10014F20u))(a0); }
XI_ORIG int sub_10015050(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10015050u))(a0); }
XI_ORIG void Camera_SetViewMatrix(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15) { reinterpret_cast<void (__cdecl *)(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int)>(XI_GAME(0x100150D0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15); }
XI_ORIG void Camera_SetMatrix154(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15) { reinterpret_cast<void (__cdecl *)(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int)>(XI_GAME(0x100151F0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15); }
XI_ORIG char * Camera_GetViewMatrixPtr() { return reinterpret_cast<char * (__cdecl *)()>(XI_GAME(0x10015220u))(); }
XI_ORIG char * sub_10015240() { return reinterpret_cast<char * (__cdecl *)()>(XI_GAME(0x10015240u))(); }
XI_ORIG int Camera_GetField50() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10015250u))(); }
XI_ORIG char * Camera_GetMatrix154Ptr() { return reinterpret_cast<char * (__cdecl *)()>(XI_GAME(0x10015270u))(); }
XI_ORIG int sub_10015290(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10015290u))(a0); }
XI_ORIG std::codecvt_base * sub_10015360(int a0) { return reinterpret_cast<std::codecvt_base * (__cdecl *)(int)>(XI_GAME(0x10015360u))(a0); }
XI_ORIG std::codecvt_base * sub_100153A0(float a0, float a1) { return reinterpret_cast<std::codecvt_base * (__cdecl *)(float, float)>(XI_GAME(0x100153A0u))(a0, a1); }
XI_ORIG void Camera_BuildProjection() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10015480u))(); }
XI_ORIG void Camera_ApplyViewProjection() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10015510u))(); }
XI_ORIG __int16 Camera_GetViewportWidth() { return reinterpret_cast<__int16 (__cdecl *)()>(XI_GAME(0x10015680u))(); }
XI_ORIG __int16 Camera_GetViewportHeight() { return reinterpret_cast<__int16 (__cdecl *)()>(XI_GAME(0x10015690u))(); }
XI_ORIG int sub_100156A0(int a0, int a1, void * a2, void * a3) { return reinterpret_cast<int (__cdecl *)(int, int, void *, void *)>(XI_GAME(0x100156A0u))(a0, a1, a2, a3); }
XI_ORIG bool fn_10015740() { return reinterpret_cast<bool (__cdecl *)()>(XI_GAME(0x10015740u))(); }
XI_ORIG int App_RunWithCOM(int a0, int a1) { return reinterpret_cast<int (__stdcall *)(int, int)>(XI_GAME(0x10015AB0u))(a0, a1); }
XI_ORIG char App_InitDataPaths() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x10015CB0u))(); }
XI_ORIG int App_DestroyDataPaths() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10015F50u))(); }
XI_ORIG char App_CheckOSVersion() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x10015F80u))(); }
XI_ORIG bool App_IsRegistryFlagSet(HKEY a0) { return reinterpret_cast<bool (__cdecl *)(HKEY)>(XI_GAME(0x100160C0u))(a0); }
XI_ORIG char App_InitSoundSystem() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x10016150u))(); }
XI_ORIG int thunk_sub_10035900() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100161A0u))(); }
XI_ORIG char sub_100163C0(char a0) { return reinterpret_cast<char (__cdecl *)(char)>(XI_GAME(0x100163C0u))(a0); }
XI_ORIG char sub_100163E0(char a0) { return reinterpret_cast<char (__cdecl *)(char)>(XI_GAME(0x100163E0u))(a0); }
XI_ORIG int App_SetGlobal_104569C8(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10016400u))(a0); }
XI_ORIG int sub_10016410() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10016410u))(); }
XI_ORIG int App_SetGlobal_104569E8(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10016490u))(a0); }
XI_ORIG int App_SetErrorCode(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x100164C0u))(a0); }
XI_ORIG int sub_10016560(void * a0, void * a1) { return reinterpret_cast<int (__cdecl *)(void *, void *)>(XI_GAME(0x10016560u))(a0, a1); }
XI_ORIG HWND App_CreateMainWindow(void * a0, void * a1, HINSTANCE a2, void * a3, int a4, int a5, int a6) { return reinterpret_cast<HWND (__cdecl *)(void *, void *, HINSTANCE, void *, int, int, int)>(XI_GAME(0x100165B0u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG BOOL App_DestroyMainWindow(int a0, void * a1, HINSTANCE a2, HWND a3) { return reinterpret_cast<BOOL (__cdecl *)(int, void *, HINSTANCE, HWND)>(XI_GAME(0x10016820u))(a0, a1, a2, a3); }
XI_ORIG char App_LoadImeUiDll(int a0, void * a1) { return reinterpret_cast<char (__cdecl *)(int, void *)>(XI_GAME(0x10016E40u))(a0, a1); }
XI_ORIG void App_UninitImeUi() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10016F10u))(); }
XI_ORIG HMODULE App_EnableImeUi() { return reinterpret_cast<HMODULE (__cdecl *)()>(XI_GAME(0x10016F40u))(); }
XI_ORIG char App_LoadKeyInputDll(int a0, void * a1) { return reinterpret_cast<char (__cdecl *)(int, void *)>(XI_GAME(0x10016F60u))(a0, a1); }
XI_ORIG HMODULE App_UnloadKeyInputDll() { return reinterpret_cast<HMODULE (__cdecl *)()>(XI_GAME(0x10016FF0u))(); }
XI_ORIG int GammaRamp_SetRGB(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(XI_GAME(0x100170A0u))(a0, a1, a2, a3); }
XI_ORIG int GammaRamp_GetRGB(void * a0, void * a1, void * a2, void * a3) { return reinterpret_cast<int (__thiscall *)(void *, void *, void *, void *)>(XI_GAME(0x100170D0u))(a0, a1, a2, a3); }
XI_ORIG int GammaRamp_GetChannelSamples(void * a0, void * a1, void * a2, void * a3) { return reinterpret_cast<int (__thiscall *)(void *, void *, void *, void *)>(XI_GAME(0x10017100u))(a0, a1, a2, a3); }
XI_ORIG __int64 GammaRamp_BuildRamps(void * a0) { return reinterpret_cast<__int64 (__thiscall *)(void *)>(XI_GAME(0x10017160u))(a0); }
XI_ORIG void GammaRamp_ApplyIfChanged(int a0) { reinterpret_cast<void (__thiscall *)(int)>(XI_GAME(0x10017230u))(a0); }
XI_ORIG int CShader_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100177A0u))(a0); }
XI_ORIG exception * fn_10017BB0(void * a0) { return reinterpret_cast<exception * (__thiscall *)(void *)>(XI_GAME(0x10017BB0u))(a0); }
XI_ORIG int sub_10017C20(int a0, void * a1, void * a2, int a3) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, int)>(XI_GAME(0x10017C20u))(a0, a1, a2, a3); }
template <class... R> XI_ORIG int nullsub_13(void * a0, R... rest) { return reinterpret_cast<int (__cdecl *)(void *, ...)>(XI_GAME(0x10017D20u))(a0, rest...); }
template <class... R> XI_ORIG int nullsub_12(void * a0, R... rest) { return reinterpret_cast<int (__cdecl *)(void *, ...)>(XI_GAME(0x10017D30u))(a0, rest...); }
XI_ORIG int nullsub_90(_DWORD a0) { return reinterpret_cast<int (__cdecl *)(_DWORD)>(XI_GAME(0x10017D50u))(a0); }
XI_ORIG void _exit_0(int a0) { reinterpret_cast<void (__cdecl *)(int)>(XI_GAME(0x100182E0u))(a0); }
XI_ORIG _DWORD * sub_10018430(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10018430u))(a0); }
XI_ORIG void sub_100185A0(void * a0, void * a1, unsigned int a2) { reinterpret_cast<void (__thiscall *)(void *, void *, unsigned int)>(XI_GAME(0x100185A0u))(a0, a1, a2); }
XI_ORIG char sub_10018850(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x10018850u))(a0); }
XI_ORIG _DWORD * OpeningSubtitle_Constructor(void * a0, int a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int)>(XI_GAME(0x10018A50u))(a0, a1); }
XI_ORIG _DWORD * OpeningSubtitle_Reset(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10018B30u))(a0); }
XI_ORIG _DWORD * Vt10329FE0_Constructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10018C50u))(a0); }
XI_ORIG int Vt10329FE0_Reset(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10018CE0u))(a0); }
XI_ORIG MMRESULT App_BeginTimerPeriod() { return reinterpret_cast<MMRESULT (__cdecl *)()>(XI_GAME(0x10019140u))(); }
XI_ORIG MMRESULT sub_100191B0() { return reinterpret_cast<MMRESULT (__cdecl *)()>(XI_GAME(0x100191B0u))(); }
XI_ORIG _DWORD * sub_100191D0() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x100191D0u))(); }
XI_ORIG int CYyMotionQue_SetSlotMotion(void * a0, void * a1, void * a2, int a3, int a4, int a5, float a6, float a7, unsigned int a8, int a9, int a10) { return reinterpret_cast<int (__thiscall *)(void *, void *, void *, int, int, int, float, float, unsigned int, int, int)>(XI_GAME(0x1001A660u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }
XI_ORIG _DWORD * Model_CreateTranslucentTargets() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x1001D0A0u))(); }
XI_ORIG char Model_BeginTranslucentPass(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x1001D240u))(a0); }
XI_ORIG char Model_TranslucentUnbindTexture(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1001D350u))(a0); }
XI_ORIG int sub_1001D390(int a0, char a1) { return reinterpret_cast<int (__stdcall *)(int, char)>(XI_GAME(0x1001D390u))(a0, a1); }
XI_ORIG int Model_CompositeTranslucentPass(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1001D3B0u))(a0); }
XI_ORIG int Gfx_DrawScreenQuad256(int a0, int a1) { return reinterpret_cast<int (__stdcall *)(int, int)>(XI_GAME(0x1001DD00u))(a0, a1); }
XI_ORIG unsigned int Tex_HeightToNormalMap(void * a0, int a1, int a2) { return reinterpret_cast<unsigned int (__thiscall *)(void *, int, int)>(XI_GAME(0x1001E0A0u))(a0, a1, a2); }
XI_ORIG int CYyCamMng2_CreateGlobal() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1001E470u))(); }
XI_ORIG int CYyCamMng2_CallGlobalVfunc1() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1001E4B0u))(); }
XI_ORIG int sub_1001E760(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x1001E760u))(a0, a1); }
XI_ORIG int sub_1001E780(int a0, void * a1) { return reinterpret_cast<int (__thiscall *)(int, void *)>(XI_GAME(0x1001E780u))(a0, a1); }
XI_ORIG int sub_10021280(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(XI_GAME(0x10021280u))(a0, a1); }
XI_ORIG void Camera_UpdateController(int a0) { reinterpret_cast<void (__thiscall *)(int)>(XI_GAME(0x10021900u))(a0); }
XI_ORIG bool sub_10021AE0(void * a0) { return reinterpret_cast<bool (__thiscall *)(void *)>(XI_GAME(0x10021AE0u))(a0); }
XI_ORIG float * sub_10021B70(void * a0) { return reinterpret_cast<float * (__thiscall *)(void *)>(XI_GAME(0x10021B70u))(a0); }
XI_ORIG double Actor_GetPercentScaleByIndex(void * a0, unsigned int a1) { return reinterpret_cast<double (__thiscall *)(void *, unsigned int)>(XI_GAME(0x10022120u))(a0, a1); }
XI_ORIG double Actor_LookupPercentScaleA(void * a0, int a1) { return reinterpret_cast<double (__thiscall *)(void *, int)>(XI_GAME(0x10022190u))(a0, a1); }
XI_ORIG double Actor_LookupPercentScaleB(void * a0, int a1) { return reinterpret_cast<double (__thiscall *)(void *, int)>(XI_GAME(0x100221E0u))(a0, a1); }
XI_ORIG _BYTE * Actor_InitIndexTable(void * a0) { return reinterpret_cast<_BYTE * (__thiscall *)(void *)>(XI_GAME(0x10022230u))(a0); }
XI_ORIG int Actor_SetPair14(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x10022270u))(a0, a1); }
XI_ORIG int Actor_SetPair1C(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x10022300u))(a0, a1); }
XI_ORIG int Actor_LookupByteField1C(int a0) { return reinterpret_cast<int (__stdcall *)(int)>(XI_GAME(0x10022340u))(a0); }
XI_ORIG char CYyModelDt_PrepareRenderState(void * a0, int a1, int a2, int a3, int a4, int a5) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int, int, int)>(XI_GAME(0x100224D0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int CYyModelDt_SetVertexShader(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10022FA0u))(a0); }
XI_ORIG __int16 CYyModelDt_ExpandTriList(void * a0, int a1, void * a2, int a3) { return reinterpret_cast<__int16 (__thiscall *)(void *, int, void *, int)>(XI_GAME(0x10023010u))(a0, a1, a2, a3); }
XI_ORIG __int16 CYyModelDt_ExpandTriStrip(void * a0, unsigned int a1, void * a2, void * a3) { return reinterpret_cast<__int16 (__thiscall *)(void *, unsigned int, void *, void *)>(XI_GAME(0x100230D0u))(a0, a1, a2, a3); }
XI_ORIG unsigned __int16 CYyModelDt_PrepareRenderStateSimple(void * a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<unsigned __int16 (__thiscall *)(void *, int, int, int, int)>(XI_GAME(0x100231C0u))(a0, a1, a2, a3, a4); }
XI_ORIG unsigned __int16 SkinVerticesSimple(int a0, int a1) { return reinterpret_cast<unsigned __int16 (__thiscall *)(int, int)>(XI_GAME(0x100231F0u))(a0, a1); }
XI_ORIG unsigned __int16 * SkinVertices(int a0, int a1, void * a2, int a3, void * a4, int a5) { return reinterpret_cast<unsigned __int16 * (__thiscall *)(int, int, void *, int, void *, int)>(XI_GAME(0x10023290u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG _DWORD * CYyModelDt_Destructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x100250C0u))(a0); }
XI_ORIG int ProjTex_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100252F0u))(a0); }
XI_ORIG char ProjTex_BeginPass(void * a0, void * a1, int a2, void * a3) { return reinterpret_cast<char (__thiscall *)(void *, void *, int, void *)>(XI_GAME(0x10025330u))(a0, a1, a2, a3); }
XI_ORIG int ProjTex_EndPass(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x100255C0u))(a0, a1); }
XI_ORIG int TexObj_GetD3DTexture(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10025860u))(a0); }
XI_ORIG int ProjTex_CaptureToTarget(void * a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int)>(XI_GAME(0x10025870u))(a0, a1, a2, a3, a4); }
XI_ORIG int nullsub_39() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10025BD0u))(); }
XI_ORIG char EnvMap_BeginPass(void * a0, int a1, void * a2) { return reinterpret_cast<char (__stdcall *)(void *, int, void *)>(XI_GAME(0x10025C40u))(a0, a1, a2); }
XI_ORIG int EnvMap_EndPass(int a0) { return reinterpret_cast<int (__stdcall *)(int)>(XI_GAME(0x10025F70u))(a0); }
XI_ORIG int sub_10026210(int a0, int a1, int a2, int a3) { return reinterpret_cast<int (__stdcall *)(int, int, int, int)>(XI_GAME(0x10026210u))(a0, a1, a2, a3); }
XI_ORIG int * CYyIcon_Constructor(void * a0, int a1, void * a2, int a3) { return reinterpret_cast<int * (__thiscall *)(void *, int, void *, int)>(XI_GAME(0x10026890u))(a0, a1, a2, a3); }
XI_ORIG int CYyIcon_DrawSubRect(void * a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int, int, int, int, int, int)>(XI_GAME(0x10026C70u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9); }
XI_ORIG _DWORD * sub_10026E00(void * a0) { return reinterpret_cast<_DWORD * (__cdecl *)(void *)>(XI_GAME(0x10026E00u))(a0); }
XI_ORIG _DWORD * Quat_SetIdentity(void * a0) { return reinterpret_cast<_DWORD * (__cdecl *)(void *)>(XI_GAME(0x10026E10u))(a0); }
XI_ORIG int Vec4_Set(int a0, float a1, int a2, int a3, int a4) { return reinterpret_cast<int (__cdecl *)(int, float, int, int, int)>(XI_GAME(0x10026E30u))(a0, a1, a2, a3, a4); }
XI_ORIG int Vec4_Copy(void * a0, void * a1) { return reinterpret_cast<int (__cdecl *)(void *, void *)>(XI_GAME(0x10026E90u))(a0, a1); }
XI_ORIG int Vec3_Copy(void * a0, void * a1) { return reinterpret_cast<int (__cdecl *)(void *, void *)>(XI_GAME(0x10026EB0u))(a0, a1); }
XI_ORIG float * sub_10026EF0(void * a0, void * a1) { return reinterpret_cast<float * (__cdecl *)(void *, void *)>(XI_GAME(0x10026EF0u))(a0, a1); }
XI_ORIG float * Vec3_AddInPlace(void * a0, void * a1) { return reinterpret_cast<float * (__cdecl *)(void *, void *)>(XI_GAME(0x10026F20u))(a0, a1); }
XI_ORIG float * sub_10026F50(void * a0, void * a1) { return reinterpret_cast<float * (__cdecl *)(void *, void *)>(XI_GAME(0x10026F50u))(a0, a1); }
XI_ORIG float * Vec3_Add_10026FA0(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, void *)>(XI_GAME(0x10026FA0u))(a0, a1, a2); }
XI_ORIG float * Vec2_Add(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, void *)>(XI_GAME(0x10026FD0u))(a0, a1, a2); }
XI_ORIG float * sub_10027020(void * a0, float a1) { return reinterpret_cast<float * (__cdecl *)(void *, float)>(XI_GAME(0x10027020u))(a0, a1); }
XI_ORIG float * Vec2_AddScalar(void * a0, float a1) { return reinterpret_cast<float * (__cdecl *)(void *, float)>(XI_GAME(0x10027050u))(a0, a1); }
XI_ORIG float * fn_100270A0(void * a0, void * a1) { return reinterpret_cast<float * (__cdecl *)(void *, void *)>(XI_GAME(0x100270A0u))(a0, a1); }
XI_ORIG float * Vec3_Sub(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, void *)>(XI_GAME(0x10027120u))(a0, a1, a2); }
XI_ORIG int fn_100272B0(int a0, float a1) { return reinterpret_cast<int (__cdecl *)(int, float)>(XI_GAME(0x100272B0u))(a0, a1); }
XI_ORIG float * Vec3_Scale(void * a0, void * a1, float a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, float)>(XI_GAME(0x100272E0u))(a0, a1, a2); }
XI_ORIG float * Vec3_MulInPlace(void * a0, void * a1) { return reinterpret_cast<float * (__cdecl *)(void *, void *)>(XI_GAME(0x10027310u))(a0, a1); }
XI_ORIG float * sub_10027340(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, void *)>(XI_GAME(0x10027340u))(a0, a1, a2); }
XI_ORIG float * Vec2_Mul(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, void *)>(XI_GAME(0x100273D0u))(a0, a1, a2); }
XI_ORIG void sub_100274B0(void * a0) { reinterpret_cast<void (__cdecl *)(void *)>(XI_GAME(0x100274B0u))(a0); }
XI_ORIG double sub_10027530(void * a0, void * a1) { return reinterpret_cast<double (__cdecl *)(void *, void *)>(XI_GAME(0x10027530u))(a0, a1); }
XI_ORIG void Gfx_ProjectWorldToClip(void * a0, void * a1) { reinterpret_cast<void (__cdecl *)(void *, void *)>(XI_GAME(0x100276F0u))(a0, a1); }
XI_ORIG void Gfx_TransformWorldToView(void * a0, void * a1) { reinterpret_cast<void (__cdecl *)(void *, void *)>(XI_GAME(0x100277E0u))(a0, a1); }
XI_ORIG void Gfx_ProjectViewToClip(void * a0, void * a1) { reinterpret_cast<void (__cdecl *)(void *, void *)>(XI_GAME(0x10027870u))(a0, a1); }
XI_ORIG float * Gfx_NdcToScreen(void * a0) { return reinterpret_cast<float * (__cdecl *)(void *)>(XI_GAME(0x100278E0u))(a0); }
XI_ORIG Iostream_init * fn_10027990(void * a0) { return reinterpret_cast<Iostream_init * (__thiscall *)(void *)>(XI_GAME(0x10027990u))(a0); }
XI_ORIG void nullsub_1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100279A0u))(); }
XI_ORIG int Mat4_SetIdentity(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100279B0u))(a0); }
XI_ORIG int Mat4_Copy(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x100279F0u))(a0, a1); }
XI_ORIG int Mat4_PostScale(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x10027A60u))(a0, a1); }
XI_ORIG int Mat4_BuildScale(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x10027AE0u))(a0, a1); }
XI_ORIG int Mat4_SetScale(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x10027B30u))(a0, a1); }
XI_ORIG int Mat4_PostRotateX(void * a0, float a1) { return reinterpret_cast<int (__thiscall *)(void *, float)>(XI_GAME(0x10027B80u))(a0, a1); }
XI_ORIG int Mat4_PostRotateY(void * a0, float a1) { return reinterpret_cast<int (__thiscall *)(void *, float)>(XI_GAME(0x10027BD0u))(a0, a1); }
XI_ORIG int Mat4_PostRotateZ(void * a0, float a1) { return reinterpret_cast<int (__thiscall *)(void *, float)>(XI_GAME(0x10027C20u))(a0, a1); }
XI_ORIG int Mat4_SetRow3(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x10027CD0u))(a0, a1); }
XI_ORIG int Mat4_SetTranslation(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x10027CF0u))(a0, a1); }
XI_ORIG int Mat4_PostMultiply(int a0, int a1) { return reinterpret_cast<int (__thiscall *)(int, int)>(XI_GAME(0x10027D10u))(a0, a1); }
XI_ORIG int Mat4_PreMultiply(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x10027E90u))(a0, a1); }
XI_ORIG _BYTE * sub_10027F60(void * a0, int a1) { return reinterpret_cast<_BYTE * (__thiscall *)(void *, int)>(XI_GAME(0x10027F60u))(a0, a1); }
XI_ORIG float * Mat4_TransformVec4InPlace(void * a0, void * a1) { return reinterpret_cast<float * (__thiscall *)(void *, void *)>(XI_GAME(0x10028140u))(a0, a1); }
XI_ORIG float * sub_10028170(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__thiscall *)(void *, void *, void *)>(XI_GAME(0x10028170u))(a0, a1, a2); }
XI_ORIG float * Mat4_TransformPointInPlace(void * a0, void * a1) { return reinterpret_cast<float * (__thiscall *)(void *, void *)>(XI_GAME(0x10028200u))(a0, a1); }
XI_ORIG float * sub_10028230(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__thiscall *)(void *, void *, void *)>(XI_GAME(0x10028230u))(a0, a1, a2); }
XI_ORIG float * Mat4_TransformNormalInPlace(void * a0, void * a1) { return reinterpret_cast<float * (__thiscall *)(void *, void *)>(XI_GAME(0x10028290u))(a0, a1); }
XI_ORIG float * Mat4_TransformNormal(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__thiscall *)(void *, void *, void *)>(XI_GAME(0x100282C0u))(a0, a1, a2); }
XI_ORIG void Mat4_TransformCoordInPlace(void * a0, void * a1) { reinterpret_cast<void (__thiscall *)(void *, void *)>(XI_GAME(0x10028320u))(a0, a1); }
XI_ORIG void sub_10028370(void * a0, void * a1, void * a2) { reinterpret_cast<void (__thiscall *)(void *, void *, void *)>(XI_GAME(0x10028370u))(a0, a1, a2); }
XI_ORIG int Mat4_InvertOrthonormal(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100283C0u))(a0); }
XI_ORIG _DWORD * Mem_AllocObject(int a0, int a1) { return reinterpret_cast<_DWORD * (__cdecl *)(int, int)>(XI_GAME(0x10028640u))(a0, a1); }
XI_ORIG _DWORD * Mem_AllocWithFallback(int a0, int a1) { return reinterpret_cast<_DWORD * (__cdecl *)(int, int)>(XI_GAME(0x100286B0u))(a0, a1); }
XI_ORIG _DWORD * Mem_FreeObject(int a0) { return reinterpret_cast<_DWORD * (__cdecl *)(int)>(XI_GAME(0x10028700u))(a0); }
XI_ORIG int SharedDeleteHelper(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10028710u))(a0); }
XI_ORIG int MemBlock_GetSize(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10028B90u))(a0); }
XI_ORIG _DWORD * Mem_AllocFromArea(int a0, int a1) { return reinterpret_cast<_DWORD * (__cdecl *)(int, int)>(XI_GAME(0x10028FD0u))(a0, a1); }
XI_ORIG int sub_10029A50() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10029A50u))(); }
XI_ORIG _DWORD * CYyModelDtBase_Destructor(void * a0, int a1) { return reinterpret_cast<_DWORD * (__fastcall *)(void *, int)>(XI_GAME(0x10029CD0u))(a0, a1); }
XI_ORIG _DWORD * CYyModelBase_Destructor(void * a0, int a1) { return reinterpret_cast<_DWORD * (__fastcall *)(void *, int)>(XI_GAME(0x10029DD0u))(a0, a1); }
XI_ORIG char CYyModelBase_SetResourceAndRebuild(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(XI_GAME(0x10029E50u))(a0, a1); }
XI_ORIG char * CYyModelBase_GetPartList(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10029E70u))(a0); }
XI_ORIG _DWORD * CYyModelBase_AddPartFromResource(void * a0, int a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int)>(XI_GAME(0x10029F70u))(a0, a1); }
XI_ORIG int CYyModelBase_DrawLit(int a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(int, int, int, int)>(XI_GAME(0x1002A3B0u))(a0, a1, a2, a3); }
XI_ORIG void CYyModelBase_ForwardToField24(void * a0, int a1, int a2) { reinterpret_cast<void (__thiscall *)(void *, int, int)>(XI_GAME(0x1002AC50u))(a0, a1, a2); }
XI_ORIG char * CYyModel_Constructor(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x1002B4B0u))(a0); }
XI_ORIG int CYyModel_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1002B550u))(a0); }
XI_ORIG char * CYyModel_GetField44Addr(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x1002B5A0u))(a0); }
XI_ORIG int CYyModel_CreateModelBase(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1002B6A0u))(a0); }
XI_ORIG char CXiSkeletonActor_DrawModel(int a0, void * a1, void * a2, int a3, void * a4, int a5, int a6, unsigned int a7) { return reinterpret_cast<char (__thiscall *)(int, void *, void *, int, void *, int, int, unsigned int)>(XI_GAME(0x1002B6E0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG char * sub_1002C270(void * a0, int a1) { return reinterpret_cast<char * (__thiscall *)(void *, int)>(XI_GAME(0x1002C270u))(a0, a1); }
XI_ORIG char * sub_1002C2A0(void * a0, void * a1) { return reinterpret_cast<char * (__thiscall *)(void *, void *)>(XI_GAME(0x1002C2A0u))(a0, a1); }
XI_ORIG char CXiSkeletonActor_AnimateModels(int a0, void * a1, void * a2, int a3, void * a4) { return reinterpret_cast<char (__thiscall *)(int, void *, void *, int, void *)>(XI_GAME(0x1002C2D0u))(a0, a1, a2, a3, a4); }
XI_ORIG void CXiSkeletonActor_PrepareModelsOnce(int a0, int a1, int a2) { reinterpret_cast<void (__thiscall *)(int, int, int)>(XI_GAME(0x1002C360u))(a0, a1, a2); }
XI_ORIG char CXiSkeletonActor_GetElemThunk(void * a0, int a1, int a2, int a3, void * a4, int a5, int a6, int a7) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int, void *, int, int, int)>(XI_GAME(0x1002C3E0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG char CXiSkeletonActor_GetModelEidMatrix(void * a0, int a1, int a2, void * a3) { return reinterpret_cast<char (__thiscall *)(void *, int, int, void *)>(XI_GAME(0x1002C570u))(a0, a1, a2, a3); }
XI_ORIG __int16 * CXiSkeletonActor_GetModelElem(void * a0, unsigned int a1, int a2) { return reinterpret_cast<__int16 * (__thiscall *)(void *, unsigned int, int)>(XI_GAME(0x1002C5C0u))(a0, a1, a2); }
XI_ORIG _DWORD * List_GetTail(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1002C7C0u))(a0); }
XI_ORIG _DWORD * Mem_AllocDefault(int a0) { return reinterpret_cast<_DWORD * (__cdecl *)(int)>(XI_GAME(0x1002C890u))(a0); }
XI_ORIG int thunk_sub_10028AA0(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x1002C8B0u))(a0); }
XI_ORIG char CYyObject_IsKindOf(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(XI_GAME(0x1002C8F0u))(a0, a1); }
XI_ORIG int Gfx_SetStreamSourceCached(unsigned int a0, int a1, int a2) { return reinterpret_cast<int (__cdecl *)(unsigned int, int, int)>(XI_GAME(0x1002F130u))(a0, a1, a2); }
XI_ORIG int Gfx_ApplyCachedVertexShader() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1002F1A0u))(); }
XI_ORIG int Gfx_SetVertexShaderCached(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x1002F1C0u))(a0); }
XI_ORIG _DWORD * Model_SkinVerticesCpu(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1002F1D0u))(a0); }
XI_ORIG void BoneXform_PostMultiplyInto(void * a0, int a1) { reinterpret_cast<void (__thiscall *)(void *, int)>(XI_GAME(0x10032FA0u))(a0, a1); }
XI_ORIG int sub_100336B0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100336B0u))(); }
XI_ORIG char Model_BeginOffscreenPass(int a0, void * a1, int a2) { return reinterpret_cast<char (__thiscall *)(int, void *, int)>(XI_GAME(0x10033800u))(a0, a1, a2); }
XI_ORIG char Model_EndOffscreenPassBindTextures(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(XI_GAME(0x10033A70u))(a0, a1); }
XI_ORIG char sub_10033DA0(int a0, void * a1) { return reinterpret_cast<char (__thiscall *)(int, void *)>(XI_GAME(0x10033DA0u))(a0, a1); }
XI_ORIG void Shadow_SmoothLightDirection(void * a0, void * a1) { reinterpret_cast<void (__thiscall *)(void *, void *)>(XI_GAME(0x10033EA0u))(a0, a1); }
XI_ORIG int Skel_OffsetAllTranslations(void * a0, void * a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, void *, int)>(XI_GAME(0x10035170u))(a0, a1, a2); }
XI_ORIG unsigned __int16 * sub_10035250(int a0, int a1) { return reinterpret_cast<unsigned __int16 * (__fastcall *)(int, int)>(XI_GAME(0x10035250u))(a0, a1); }
XI_ORIG char CMoTask_VT_1032A904_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10035B00u))(a0); }
XI_ORIG void * sub_10035E30(int a0, int a1) { return reinterpret_cast<void * (__cdecl *)(int, int)>(XI_GAME(0x10035E30u))(a0, a1); }
XI_ORIG void MoTask_KillAllGlobalSlots(float a0) { reinterpret_cast<void (__cdecl *)(float)>(XI_GAME(0x10035F00u))(a0); }
XI_ORIG void MoTask_KillGlobalSlot1(float a0) { reinterpret_cast<void (__cdecl *)(float)>(XI_GAME(0x10036070u))(a0); }
XI_ORIG int sub_100361D0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100361D0u))(); }
XI_ORIG char sub_10036230() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x10036230u))(); }
XI_ORIG char sub_10036B90(void * a0, void * a1) { return reinterpret_cast<char (__thiscall *)(void *, void *)>(XI_GAME(0x10036B90u))(a0, a1); }
XI_ORIG int sub_10036F00(void * a0, __int16 a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, __int16, int)>(XI_GAME(0x10036F00u))(a0, a1, a2); }
XI_ORIG __int16 sub_10037020(void * a0, void * a1, int a2, int a3, int a4, int a5, int a6, int a7, __int16 a8) { return reinterpret_cast<__int16 (__thiscall *)(void *, void *, int, int, int, int, int, int, __int16)>(XI_GAME(0x10037020u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG int sub_100370D0(void * a0, void * a1, int a2, int a3, int a4, int a5, int a6, int a7, __int16 a8) { return reinterpret_cast<int (__thiscall *)(void *, void *, int, int, int, int, int, int, __int16)>(XI_GAME(0x100370D0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG unsigned int Font_QueueText(void * a0, int a1, int a2, char a3) { return reinterpret_cast<unsigned int (__cdecl *)(void *, int, int, char)>(XI_GAME(0x100384B0u))(a0, a1, a2, a3); }
XI_ORIG int Font_DrawPass() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10038520u))(); }
XI_ORIG unsigned __int8 * Font_DrawString(void * a0, float a1, float a2, float a3) { return reinterpret_cast<unsigned __int8 * (__cdecl *)(void *, float, float, float)>(XI_GAME(0x10038720u))(a0, a1, a2, a3); }
XI_ORIG _DWORD * Post_CreateScreenEffectTargets() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x10038930u))(); }
XI_ORIG void sub_10038A70(int a0) { reinterpret_cast<void (__stdcall *)(int)>(XI_GAME(0x10038A70u))(a0); }
XI_ORIG int Post_CaptureSceneToTarget(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10038A80u))(a0); }
XI_ORIG int Post_DrawRotatingGhostLayers(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10038E10u))(a0); }
XI_ORIG _DWORD * sub_10039410(void * a0, void * a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *)>(XI_GAME(0x10039410u))(a0, a1); }
XI_ORIG int Util_Copy16Bytes(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10039810u))(a0, a1); }
XI_ORIG void CYyTexBase_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100398D0u))(a0); }
XI_ORIG int CYyTexBase_SetName(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x100398E0u))(a0, a1); }
XI_ORIG void CYyTex_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10039970u))(a0); }
XI_ORIG char TexObj_CreateAndUpload(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10039A30u))(a0); }
XI_ORIG int TexObj_Upload(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10039E50u))(a0); }
XI_ORIG _WORD * sub_1003AA50(void * a0) { return reinterpret_cast<_WORD * (__thiscall *)(void *)>(XI_GAME(0x1003AA50u))(a0); }
XI_ORIG int CYyTexMng_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003AA90u))(a0); }
XI_ORIG char TexMgr_UploadPumpEntry(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1003AB70u))(a0); }
XI_ORIG int CYyTexMng_FindByName(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1003AC80u))(a0, a1); }
XI_ORIG int sub_1003ACD0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1003ACD0u))(a0, a1); }
XI_ORIG int sub_1003AD40(void * a0, void * a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, void *, int)>(XI_GAME(0x1003AD40u))(a0, a1, a2); }
XI_ORIG int sub_1003ADC0(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1003ADC0u))(a0, a1, a2); }
XI_ORIG _DWORD * TexMgr_FindByName(void * a0, int a1, int a2, char a3) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int, int, char)>(XI_GAME(0x1003AEA0u))(a0, a1, a2, a3); }
XI_ORIG int CYyTexMng_FindByNameInResource(void * a0, void * a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, void *, int)>(XI_GAME(0x1003AF10u))(a0, a1, a2); }
XI_ORIG _DWORD * sub_1003B080(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1003B080u))(a0); }
XI_ORIG int CYyVbMng_AcquireVertexBuffer(void * a0, int a1, unsigned int a2, unsigned int a3, D3DPOOL a4, void * a5, char a6) { return reinterpret_cast<int (__thiscall *)(void *, int, unsigned int, unsigned int, D3DPOOL, void *, char)>(XI_GAME(0x1003B210u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG _DWORD * CYyVbMng_ReleaseVertexBuffer(void * a0) { return reinterpret_cast<_DWORD * (__stdcall *)(void *)>(XI_GAME(0x1003B3C0u))(a0); }
XI_ORIG _DWORD * CYyVb_Constructor(void * a0, int a1, int a2, int a3, int a4, int a5) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int, int, int, int, int)>(XI_GAME(0x1003B450u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG _DWORD * sub_1003B540(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1003B540u))(a0); }
XI_ORIG void ReleaseSubObjectIfPresent(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1003B5C0u))(a0); }
XI_ORIG int CMoAttachments_Reset(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003B640u))(a0); }
XI_ORIG int sub_1003B6D0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003B6D0u))(a0); }
XI_ORIG int sub_1003B700(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003B700u))(a0); }
XI_ORIG int CMoAttachments_ClearCaster(void * a0, char a1) { return reinterpret_cast<int (__thiscall *)(void *, char)>(XI_GAME(0x1003B750u))(a0, a1); }
XI_ORIG int CMoAttachments_ClearTarget(void * a0, char a1) { return reinterpret_cast<int (__thiscall *)(void *, char)>(XI_GAME(0x1003B780u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_SetCaster(void * a0, void * a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *)>(XI_GAME(0x1003B7B0u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_SetTarget(void * a0, void * a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *)>(XI_GAME(0x1003B830u))(a0, a1); }
XI_ORIG void * sub_1003B8B0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1003B8B0u))(a0); }
XI_ORIG void * sub_1003B900(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1003B900u))(a0); }
XI_ORIG int sub_1003B950(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003B950u))(a0); }
XI_ORIG long double sub_1003B980(void * a0, void * a1, void * a2) { return reinterpret_cast<long double (__cdecl *)(void *, void *, void *)>(XI_GAME(0x1003B980u))(a0, a1, a2); }
XI_ORIG float * sub_1003BA20(void * a0, int a1, void * a2, void * a3, int a4) { return reinterpret_cast<float * (__cdecl *)(void *, int, void *, void *, int)>(XI_GAME(0x1003BA20u))(a0, a1, a2, a3, a4); }
XI_ORIG int sub_1003BD90(int a0, int a1, int a2) { return reinterpret_cast<int (__cdecl *)(int, int, int)>(XI_GAME(0x1003BD90u))(a0, a1, a2); }
XI_ORIG char ** CMoCamera_GetRuntimeClass() { return reinterpret_cast<char ** (__cdecl *)()>(XI_GAME(0x1003C880u))(); }
XI_ORIG int sub_1003C8A0(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(XI_GAME(0x1003C8A0u))(a0, a1, a2, a3); }
XI_ORIG char ** sub_1003CBB0(void * a0) { return reinterpret_cast<char ** (__thiscall *)(void *)>(XI_GAME(0x1003CBB0u))(a0); }
XI_ORIG char CMoCameraTask_Destructor(void * a0, int a1) { return reinterpret_cast<char (__fastcall *)(void *, int)>(XI_GAME(0x1003D030u))(a0, a1); }
XI_ORIG int Sky_DrawGradientDome(float a0, float a1, int a2, void * a3, float a4) { return reinterpret_cast<int (__cdecl *)(float, float, int, void *, float)>(XI_GAME(0x1003D640u))(a0, a1, a2, a3, a4); }
XI_ORIG void CMoCelestial_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1003D980u))(a0); }
XI_ORIG void Sky_DestroyDomeShaderStatic() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1003DB50u))(); }
XI_ORIG void Sky_DestroyViewCenteredShaderStatic() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1003DDF0u))(); }
XI_ORIG int sub_1003DE40(int a0) { return reinterpret_cast<int (__fastcall *)(int)>(XI_GAME(0x1003DE40u))(a0); }
XI_ORIG unsigned int Color_SetFromU32SwapRB(void * a0, unsigned int a1) { return reinterpret_cast<unsigned int (__thiscall *)(void *, unsigned int)>(XI_GAME(0x1003E280u))(a0, a1); }
XI_ORIG char Color_CopySwapRB(void * a0, void * a1) { return reinterpret_cast<char (__thiscall *)(void *, void *)>(XI_GAME(0x1003E2A0u))(a0, a1); }
XI_ORIG int sub_1003E910(int a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(int, int, int, int)>(XI_GAME(0x1003E910u))(a0, a1, a2, a3); }
XI_ORIG void unknown_libname_20() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1003EB00u))(); }
XI_ORIG void unknown_libname_21() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1003EB10u))(); }
XI_ORIG void unknown_libname_22() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1003EB20u))(); }
XI_ORIG char CMoVuAnmElem_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1003EF20u))(a0); }
XI_ORIG int CMoDistMorphElem_ApplyMorph(int a0, int a1, void * a2, int a3, int a4, void * a5, char a6) { return reinterpret_cast<int (__thiscall *)(int, int, void *, int, int, void *, char)>(XI_GAME(0x1003F440u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG void unknown_libname_25() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1003F890u))(); }
XI_ORIG _DWORD * CMoDistMorphElem_ComputeMorphHandle(int a0, int a1, int a2) { return reinterpret_cast<_DWORD * (__thiscall *)(int, int, int)>(XI_GAME(0x1003F8A0u))(a0, a1, a2); }
XI_ORIG char CXi471011C8_VT_1032AC90_Dtor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1003FB10u))(a0); }
XI_ORIG bool sub_1003FCB0(void * a0) { return reinterpret_cast<bool (__thiscall *)(void *)>(XI_GAME(0x1003FCB0u))(a0); }
XI_ORIG char * sub_1003FCE0(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x1003FCE0u))(a0); }
XI_ORIG char * sub_1003FCF0(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x1003FCF0u))(a0); }
XI_ORIG char * MoElem_GetVertexData(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x1003FD70u))(a0); }
XI_ORIG int CMoDistModelElem_DrawMesh(void * a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int)>(XI_GAME(0x10040670u))(a0, a1, a2, a3, a4); }
XI_ORIG void unknown_libname_33() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10040B50u))(); }
XI_ORIG void unknown_libname_34() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10040B60u))(); }
XI_ORIG void unknown_libname_35() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10040B70u))(); }
XI_ORIG void unknown_libname_36() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10040B80u))(); }
XI_ORIG void unknown_libname_37() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10040B90u))(); }
XI_ORIG void unknown_libname_38() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10040E10u))(); }
XI_ORIG char CXi471011C8_VT_1032AD80_Dtor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10041110u))(a0); }
XI_ORIG void Font_DrawProjectedText3D(int a0, int a1, int a2, int a3, int a4) { reinterpret_cast<void (__cdecl *)(int, int, int, int, int)>(XI_GAME(0x10041C20u))(a0, a1, a2, a3, a4); }
XI_ORIG void StaticDtor_StateBlock_1047BADC() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10041F30u))(); }
XI_ORIG _DWORD * CMoOtTask_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x100425F0u))(a0, a1); }
XI_ORIG void unknown_libname_46() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10042CD0u))(); }
XI_ORIG void unknown_libname_47() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10042CE0u))(); }
XI_ORIG void nullsub_136() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10042CF0u))(); }
XI_ORIG char CMoDistMorphElem_ValidateMorphData(int a0, int a1, int a2, int a3) { return reinterpret_cast<char (__stdcall *)(int, int, int, int)>(XI_GAME(0x10042D00u))(a0, a1, a2, a3); }
XI_ORIG void unknown_libname_48() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100430A0u))(); }
XI_ORIG void unknown_libname_49() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10043310u))(); }
XI_ORIG void StaticDtor_StateBlock_1047BB7C() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100439B0u))(); }
XI_ORIG int sub_100439C0(int a0, void * a1, void * a2, void * a3, int a4, int a5) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, void *, int, int)>(XI_GAME(0x100439C0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void * CMoTask_VT_1032AF60_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<void * (__thiscall *)(void *, char)>(XI_GAME(0x10043A90u))(a0, a1); }
XI_ORIG char CMoTask_VT_1032AF60_Destructor(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x10043AB0u))(a0); }
XI_ORIG int sub_10043BB0(int a0, void * a1, void * a2, void * a3, int a4, int a5) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, void *, int, int)>(XI_GAME(0x10043BB0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int * CMoTask_VT_1032AFA8_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<int * (__thiscall *)(void *, char)>(XI_GAME(0x10043C90u))(a0, a1); }
XI_ORIG char CMoTask_VT_1032AFA8_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10043CB0u))(a0); }
XI_ORIG int sub_10043DE0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10043DE0u))(); }
XI_ORIG char * DynVB_Constructor(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10043DF0u))(a0); }
XI_ORIG void DynVB_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10043E40u))(a0); }
XI_ORIG int DynVB_Create(void * a0, int a1, unsigned int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, unsigned int, int)>(XI_GAME(0x10043E50u))(a0, a1, a2, a3); }
XI_ORIG void sub_10043F00(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10043F00u))(a0); }
XI_ORIG int sub_10043F40(void * a0, int a1, int a2, int a3, unsigned int a4, int a5) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, unsigned int, int)>(XI_GAME(0x10043F40u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int DynVB_Reserve(int a0) { return reinterpret_cast<int (__stdcall *)(int)>(XI_GAME(0x10044010u))(a0); }
XI_ORIG void * DynVB_Lock(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10044130u))(a0); }
XI_ORIG int DynVB_Unlock() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100441A0u))(); }
XI_ORIG int sub_100441C0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100441C0u))(a0); }
XI_ORIG int Gfx_SetViewMatrixFromField10(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10044210u))(a0); }
XI_ORIG int Gfx_SetViewMatrixFromField90(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10044230u))(a0); }
XI_ORIG _DWORD * Mo_InitSharedRenderResources() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x10044270u))(); }
XI_ORIG int sub_10044490() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10044490u))(); }
XI_ORIG int sub_10044540() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10044540u))(); }
XI_ORIG char CMoElemFamily_SharedCleanup(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10044970u))(a0); }
XI_ORIG float * sub_10044C40(int a0, void * a1) { return reinterpret_cast<float * (__thiscall *)(int, void *)>(XI_GAME(0x10044C40u))(a0, a1); }
XI_ORIG int sub_10045800(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x10045800u))(a0); }
XI_ORIG void nullsub_140() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10045A30u))(); }
XI_ORIG int sub_10045C50(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10045C50u))(a0); }
XI_ORIG int RegisterWithProcessor(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10045D40u))(a0); }
XI_ORIG int sub_100464D0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100464D0u))(); }
XI_ORIG int sub_10046550(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10046550u))(a0, a1); }
XI_ORIG void unknown_libname_53() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100472D0u))(); }
XI_ORIG void unknown_libname_54() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100473E0u))(); }
XI_ORIG int sub_100473F0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100473F0u))(a0); }
XI_ORIG char CMoSkeletonElem_Destructor(int a0, int a1) { return reinterpret_cast<char (__fastcall *)(int, int)>(XI_GAME(0x10047900u))(a0, a1); }
XI_ORIG int CMoGeneratorClone_VT_1032B114_Constructor(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10047F80u))(a0); }
XI_ORIG void * CMoGeneratorClone_VT_1032B114_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<void * (__thiscall *)(void *, char)>(XI_GAME(0x10047FE0u))(a0, a1); }
XI_ORIG int CMoGeneratorClone_VT_1032B114_Destructor(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10048000u))(a0); }
XI_ORIG char CMoGeneratorClone_UnlinkElemListA(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(XI_GAME(0x10048140u))(a0, a1); }
XI_ORIG char CMoGeneratorClone_UnlinkElemListB(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(XI_GAME(0x100481D0u))(a0, a1); }
XI_ORIG _DWORD * CMoGeneratorClone_FindCmdByType(void * a0, int a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int)>(XI_GAME(0x100492F0u))(a0, a1); }
XI_ORIG int sub_10049560(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10049560u))(a0); }
XI_ORIG int CMoGeneratorClone_ResolveTarget(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10049570u))(a0); }
XI_ORIG bool sub_100496F0(int a0, int a1) { return reinterpret_cast<bool (__thiscall *)(int, int)>(XI_GAME(0x100496F0u))(a0, a1); }
XI_ORIG BOOL sub_1004E3B0(void * a0, int a1) { return reinterpret_cast<BOOL (__thiscall *)(void *, int)>(XI_GAME(0x1004E3B0u))(a0, a1); }
XI_ORIG int sub_1004E3C0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1004E3C0u))(a0); }
XI_ORIG Iostream_init * fn_100527F0(void * a0) { return reinterpret_cast<Iostream_init * (__thiscall *)(void *)>(XI_GAME(0x100527F0u))(a0); }
XI_ORIG _DWORD * sub_10052870(void * a0, void * a1, int a2) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, int)>(XI_GAME(0x10052870u))(a0, a1, a2); }
XI_ORIG int sub_10052880(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10052880u))(a0, a1); }
XI_ORIG int sub_10052890(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10052890u))(a0); }
XI_ORIG int sub_100528A0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100528A0u))(a0); }
XI_ORIG __int16 SharedConditionalStateReset(int a0) { return reinterpret_cast<__int16 (__thiscall *)(int)>(XI_GAME(0x100531D0u))(a0); }
XI_ORIG __int16 sub_10053370(int a0) { return reinterpret_cast<__int16 (__thiscall *)(int)>(XI_GAME(0x10053370u))(a0); }
XI_ORIG int CMoGeneratorClone_CopyParams(int a0, int a1) { return reinterpret_cast<int (__thiscall *)(int, int)>(XI_GAME(0x10053450u))(a0, a1); }
XI_ORIG int CMoGeneratorClone_CreateInstance(void * a0, void * a1, void * a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, void *, void *, int)>(XI_GAME(0x100536C0u))(a0, a1, a2, a3); }
XI_ORIG char * CMoGeneratorClone_VT_1032B69C_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<char * (__thiscall *)(void *, char)>(XI_GAME(0x10053840u))(a0, a1); }
XI_ORIG int sub_10053890(int a0, char a1) { return reinterpret_cast<int (__thiscall *)(int, char)>(XI_GAME(0x10053890u))(a0, a1); }
XI_ORIG char sub_100538E0(int a0, void * a1) { return reinterpret_cast<char (__thiscall *)(int, void *)>(XI_GAME(0x100538E0u))(a0, a1); }
XI_ORIG bool sub_100539E0(void * a0) { return reinterpret_cast<bool (__thiscall *)(void *)>(XI_GAME(0x100539E0u))(a0); }
XI_ORIG char sub_10053A00(int a0, int a1, int a2, char a3, void * a4) { return reinterpret_cast<char (__thiscall *)(int, int, int, char, void *)>(XI_GAME(0x10053A00u))(a0, a1, a2, a3, a4); }
XI_ORIG char sub_10053E60(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10053E60u))(a0); }
XI_ORIG char CMoTask_VT_1032B654_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100541C0u))(a0); }
XI_ORIG char ** CMoGeneratorClone_GetRuntimeClass() { return reinterpret_cast<char ** (__cdecl *)()>(XI_GAME(0x100542E0u))(); }
XI_ORIG int CMoGeneratorClone_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100542F0u))(a0); }
XI_ORIG __int16 CMoGeneratorClone_Kill(int a0) { return reinterpret_cast<__int16 (__cdecl *)(int)>(XI_GAME(0x100543A0u))(a0); }
XI_ORIG char CMoLfdElem_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100548C0u))(a0); }
XI_ORIG void nullsub_130() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10054F70u))(); }
XI_ORIG int Vt1032B6D8_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10055000u))(a0); }
XI_ORIG _DWORD * LinkNode_Init(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10055240u))(a0); }
XI_ORIG int LinkNode_Unlink(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10055250u))(a0); }
XI_ORIG _DWORD * CMoOT_Destructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x100552E0u))(a0); }
XI_ORIG char sub_100553C0(int a0, int a1) { return reinterpret_cast<char (__thiscall *)(int, int)>(XI_GAME(0x100553C0u))(a0, a1); }
XI_ORIG char sub_100555B0(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(XI_GAME(0x100555B0u))(a0, a1); }
XI_ORIG _DWORD * sub_100555D0() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x100555D0u))(); }
XI_ORIG char * CMoSphRes_FixupOffsets(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10056220u))(a0); }
XI_ORIG char CMoPointLightProgElem_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10056390u))(a0); }
XI_ORIG void unknown_libname_55() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10056840u))(); }
XI_ORIG char ** CMoPathDriveActorTask_GetRuntimeClass() { return reinterpret_cast<char ** (__cdecl *)()>(XI_GAME(0x10056940u))(); }
XI_ORIG char ** CMoLockColorDriveTask_GetRuntimeClass() { return reinterpret_cast<char ** (__cdecl *)()>(XI_GAME(0x10056960u))(); }
XI_ORIG char ** CMoLockLookAtDriveTask_GetRuntimeClass() { return reinterpret_cast<char ** (__cdecl *)()>(XI_GAME(0x10056980u))(); }
XI_ORIG char ** CMoActorColorDriveTask_GetRuntimeClass() { return reinterpret_cast<char ** (__cdecl *)()>(XI_GAME(0x100569A0u))(); }
XI_ORIG char ** CMoActorRotationDriveTask_GetRuntimeClass() { return reinterpret_cast<char ** (__cdecl *)()>(XI_GAME(0x100569C0u))(); }
XI_ORIG int sub_10056BB0(int a0, void * a1, void * a2, int a3) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, int)>(XI_GAME(0x10056BB0u))(a0, a1, a2, a3); }
XI_ORIG int * Scheduler_Execute(void * a0, void * a1, void * a2, int a3, int a4) { return reinterpret_cast<int * (__thiscall *)(void *, void *, void *, int, int)>(XI_GAME(0x10056C70u))(a0, a1, a2, a3, a4); }
XI_ORIG int sub_10056EB0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10056EB0u))(a0); }
XI_ORIG int Scheduler_Kill(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x10056EF0u))(a0, a1, a2); }
XI_ORIG char CMoSchedularTask_Destructor(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x100574A0u))(a0); }
XI_ORIG int sub_10057BE0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10057BE0u))(a0); }
XI_ORIG int sub_10057BF0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10057BF0u))(a0); }
XI_ORIG int CMoSchedularTask_Interpret(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10057FB0u))(a0); }
XI_ORIG long double sub_1005DFF0(float a0, float a1) { return reinterpret_cast<long double (__cdecl *)(float, float)>(XI_GAME(0x1005DFF0u))(a0, a1); }
XI_ORIG bool sub_1005E000(void * a0, char a1) { return reinterpret_cast<bool (__thiscall *)(void *, char)>(XI_GAME(0x1005E000u))(a0, a1); }
XI_ORIG bool sub_1005E020(void * a0, char a1) { return reinterpret_cast<bool (__thiscall *)(void *, char)>(XI_GAME(0x1005E020u))(a0, a1); }
XI_ORIG _DWORD * sub_1005E040(int a0) { return reinterpret_cast<_DWORD * (__cdecl *)(int)>(XI_GAME(0x1005E040u))(a0); }
XI_ORIG _DWORD * sub_1005E070(void * a0, void * a1, int a2) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, int)>(XI_GAME(0x1005E070u))(a0, a1, a2); }
XI_ORIG int sub_1005E090(void * a0, void * a1, void * a2) { return reinterpret_cast<int (__thiscall *)(void *, void *, void *)>(XI_GAME(0x1005E090u))(a0, a1, a2); }
XI_ORIG BOOL sub_1005E0B0(int a0) { return reinterpret_cast<BOOL (__thiscall *)(int)>(XI_GAME(0x1005E0B0u))(a0); }
XI_ORIG int sub_1005E0E0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1005E0E0u))(a0); }
XI_ORIG int sub_1005E0F0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1005E0F0u))(a0, a1); }
XI_ORIG Iostream_init * fn_1005E100(void * a0) { return reinterpret_cast<Iostream_init * (__thiscall *)(void *)>(XI_GAME(0x1005E100u))(a0); }
XI_ORIG _DWORD * sub_1005E110(void * a0, int a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int)>(XI_GAME(0x1005E110u))(a0, a1); }
XI_ORIG BOOL sub_1005E120(void * a0, int a1) { return reinterpret_cast<BOOL (__thiscall *)(void *, int)>(XI_GAME(0x1005E120u))(a0, a1); }
XI_ORIG int sub_1005E130(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1005E130u))(a0); }
XI_ORIG _DWORD * sub_1005E140(void * a0, void * a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *)>(XI_GAME(0x1005E140u))(a0, a1); }
XI_ORIG _DWORD * sub_1005E150(void * a0, void * a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *)>(XI_GAME(0x1005E150u))(a0, a1); }
XI_ORIG int sub_1005E160(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1005E160u))(a0, a1); }
XI_ORIG double sub_1005E170(void * a0) { return reinterpret_cast<double (__thiscall *)(void *)>(XI_GAME(0x1005E170u))(a0); }
XI_ORIG void * sub_1005E180(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1005E180u))(a0); }
XI_ORIG BOOL sub_1005E190(void * a0) { return reinterpret_cast<BOOL (__thiscall *)(void *)>(XI_GAME(0x1005E190u))(a0); }
XI_ORIG char sub_1005E1A0(void * a0, int a1, int a2) { return reinterpret_cast<char (__thiscall *)(void *, int, int)>(XI_GAME(0x1005E1A0u))(a0, a1, a2); }
XI_ORIG unsigned int sub_1005E1C0(void * a0) { return reinterpret_cast<unsigned int (__thiscall *)(void *)>(XI_GAME(0x1005E1C0u))(a0); }
XI_ORIG char * sub_1005E1D0(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x1005E1D0u))(a0); }
XI_ORIG int sub_1005E1F0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1005E1F0u))(a0); }
XI_ORIG __int16 sub_1005E200(void * a0) { return reinterpret_cast<__int16 (__thiscall *)(void *)>(XI_GAME(0x1005E200u))(a0); }
XI_ORIG int sub_1005E210(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1005E210u))(a0); }
XI_ORIG _DWORD * sub_1005E230(void * a0, void * a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *)>(XI_GAME(0x1005E230u))(a0, a1); }
XI_ORIG _DWORD * sub_1005E240(void * a0, void * a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *)>(XI_GAME(0x1005E240u))(a0, a1); }
XI_ORIG _DWORD * sub_1005E250(void * a0, void * a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *)>(XI_GAME(0x1005E250u))(a0, a1); }
XI_ORIG _DWORD * sub_1005E260(void * a0, void * a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *)>(XI_GAME(0x1005E260u))(a0, a1); }
XI_ORIG _DWORD * sub_1005E270(void * a0, void * a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *)>(XI_GAME(0x1005E270u))(a0, a1); }
XI_ORIG _DWORD * sub_1005E280(void * a0, void * a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *)>(XI_GAME(0x1005E280u))(a0, a1); }
XI_ORIG char sub_1005E290(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1005E290u))(a0); }
XI_ORIG char sub_1005E2A0(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1005E2A0u))(a0); }
XI_ORIG char sub_1005E2B0(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1005E2B0u))(a0); }
XI_ORIG char sub_1005E2C0(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1005E2C0u))(a0); }
XI_ORIG char sub_1005E2D0(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1005E2D0u))(a0); }
XI_ORIG int sub_1005E2E0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1005E2E0u))(a0); }
XI_ORIG bool sub_1005E2F0(void * a0) { return reinterpret_cast<bool (__thiscall *)(void *)>(XI_GAME(0x1005E2F0u))(a0); }
XI_ORIG int unknown_libname_57() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1005E300u))(); }
XI_ORIG int sub_1005E310(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1005E310u))(a0); }
XI_ORIG int sub_1005E320(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1005E320u))(a0); }
XI_ORIG int sub_1005E330(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1005E330u))(a0, a1); }
XI_ORIG int sub_1005E350(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1005E350u))(a0); }
XI_ORIG int sub_1005E360(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1005E360u))(a0); }
XI_ORIG int sub_1005E380(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1005E380u))(a0); }
XI_ORIG _DWORD * sub_1005E3A0(void * a0, void * a1, float a2) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, float)>(XI_GAME(0x1005E3A0u))(a0, a1, a2); }
XI_ORIG _DWORD * sub_1005E3E0(void * a0, void * a1, float a2) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, float)>(XI_GAME(0x1005E3E0u))(a0, a1, a2); }
XI_ORIG _DWORD * sub_1005E420(void * a0, void * a1, float a2) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, float)>(XI_GAME(0x1005E420u))(a0, a1, a2); }
XI_ORIG _DWORD * sub_1005E460(void * a0, void * a1, float a2) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, float)>(XI_GAME(0x1005E460u))(a0, a1, a2); }
XI_ORIG int sub_1005E4A0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1005E4A0u))(a0); }
XI_ORIG int sub_1005E4B0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1005E4B0u))(a0); }
XI_ORIG int sub_1005E4C0(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x1005E4C0u))(a0, a1); }
XI_ORIG void sub_1005E4D0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1005E4D0u))(a0); }
XI_ORIG int sub_1005E4E0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1005E4E0u))(a0); }
XI_ORIG int sub_1005E4F0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1005E4F0u))(a0); }
XI_ORIG int sub_1005E500(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1005E500u))(a0); }
XI_ORIG unsigned int sub_1005E510(void * a0) { return reinterpret_cast<unsigned int (__thiscall *)(void *)>(XI_GAME(0x1005E510u))(a0); }
XI_ORIG char * sub_1005E520(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x1005E520u))(a0); }
XI_ORIG int sub_1005E530(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1005E530u))(a0, a1); }
XI_ORIG int sub_1005E540(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1005E540u))(a0, a1); }
XI_ORIG double sub_1005E550(int a0) { return reinterpret_cast<double (__thiscall *)(int)>(XI_GAME(0x1005E550u))(a0); }
XI_ORIG double sub_1005E570(void * a0, int a1) { return reinterpret_cast<double (__thiscall *)(void *, int)>(XI_GAME(0x1005E570u))(a0, a1); }
XI_ORIG double sub_1005E590(int a0) { return reinterpret_cast<double (__thiscall *)(int)>(XI_GAME(0x1005E590u))(a0); }
XI_ORIG double sub_1005E5B0(void * a0, int a1) { return reinterpret_cast<double (__thiscall *)(void *, int)>(XI_GAME(0x1005E5B0u))(a0, a1); }
XI_ORIG char sub_1005E5D0(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1005E5D0u))(a0); }
XI_ORIG int sub_1005E5E0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1005E5E0u))(a0); }
XI_ORIG char sub_1005E5F0(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1005E5F0u))(a0); }
XI_ORIG int sub_1005E600(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x1005E600u))(a0); }
XI_ORIG int sub_1005E610(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x1005E610u))(a0); }
XI_ORIG double sub_1005E620() { return reinterpret_cast<double (__cdecl *)()>(XI_GAME(0x1005E620u))(); }
XI_ORIG void sub_1005E630(int a0) { reinterpret_cast<void (__thiscall *)(int)>(XI_GAME(0x1005E630u))(a0); }
XI_ORIG int sub_1005E640(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1005E640u))(a0); }
XI_ORIG int sub_1005E650(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1005E650u))(a0); }
XI_ORIG double sub_1005E660(int a0) { return reinterpret_cast<double (__thiscall *)(int)>(XI_GAME(0x1005E660u))(a0); }
XI_ORIG int sub_1005E680(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1005E680u))(a0, a1); }
XI_ORIG int sub_1005E690(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1005E690u))(a0, a1); }
XI_ORIG int sub_1005E6A0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1005E6A0u))(a0, a1); }
XI_ORIG int sub_1005E6B0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1005E6B0u))(a0, a1); }
XI_ORIG int sub_1005E6C0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1005E6C0u))(a0, a1); }
XI_ORIG int sub_1005E6D0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1005E6D0u))(a0, a1); }
XI_ORIG int sub_1005E6E0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1005E6E0u))(a0, a1); }
XI_ORIG int sub_1005E6F0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1005E6F0u))(a0, a1); }
XI_ORIG int sub_1005E700(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1005E700u))(a0); }
XI_ORIG int sub_1005E710(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1005E710u))(a0); }
XI_ORIG char sub_1005E720(void * a0, char a1) { return reinterpret_cast<char (__thiscall *)(void *, char)>(XI_GAME(0x1005E720u))(a0, a1); }
XI_ORIG int sub_1005E740(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1005E740u))(a0, a1); }
XI_ORIG int sub_1005E750(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1005E750u))(a0, a1); }
XI_ORIG int sub_1005E760(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1005E760u))(a0, a1); }
XI_ORIG int sub_1005E770(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1005E770u))(a0, a1); }
XI_ORIG int sub_1005E780(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1005E780u))(a0, a1); }
XI_ORIG double sub_1005E790(void * a0) { return reinterpret_cast<double (__thiscall *)(void *)>(XI_GAME(0x1005E790u))(a0); }
XI_ORIG int sub_1005E7A0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1005E7A0u))(a0, a1); }
XI_ORIG _DWORD * sub_1005E7B0(void * a0, void * a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *)>(XI_GAME(0x1005E7B0u))(a0, a1); }
XI_ORIG int sub_1005E7C0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1005E7C0u))(a0, a1); }
XI_ORIG int sub_1005E7D0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1005E7D0u))(a0, a1); }
XI_ORIG int sub_1005E7E0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1005E7E0u))(a0, a1); }
XI_ORIG _DWORD * sub_1005E800(void * a0, int a1, int a2, int a3, void * a4, void * a5, void * a6, void * a7, int a8, int a9, int a10, int a11, void * a12) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int, int, int, void *, void *, void *, void *, int, int, int, int, void *)>(XI_GAME(0x1005E800u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12); }
XI_ORIG int sub_1005E880(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1005E880u))(a0); }
XI_ORIG int sub_1005E890(int a0, void * a1, void * a2, int a3, int a4, char a5, int a6, char a7, int a8, unsigned int a9, int a10, int a11) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, int, int, char, int, char, int, unsigned int, int, int)>(XI_GAME(0x1005E890u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11); }
XI_ORIG _DWORD * CMoPathDriveActorTask_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x1005F2C0u))(a0, a1); }
XI_ORIG int sub_1005F320(int a0, void * a1, void * a2, int a3) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, int)>(XI_GAME(0x1005F320u))(a0, a1, a2, a3); }
XI_ORIG int CMoLockColorDriveTask_ScalarDeletingDtor(int a0, char a1) { return reinterpret_cast<int (__thiscall *)(int, char)>(XI_GAME(0x1005F3F0u))(a0, a1); }
XI_ORIG int sub_1005F450(int a0, void * a1, void * a2, int a3) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, int)>(XI_GAME(0x1005F450u))(a0, a1, a2, a3); }
XI_ORIG _DWORD * CMoLockLookAtDriveTask_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x1005F660u))(a0, a1); }
XI_ORIG int sub_1005F6C0(int a0, void * a1, void * a2, void * a3, int a4, char a5, char a6) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, void *, int, char, char)>(XI_GAME(0x1005F6C0u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int CMoActorColorDriveTask_ScalarDeletingDtor(int a0, char a1) { return reinterpret_cast<int (__thiscall *)(int, char)>(XI_GAME(0x1005F990u))(a0, a1); }
XI_ORIG int sub_1005FA20(int a0, void * a1, void * a2, int a3, float a4, float a5, float a6, int a7) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, int, float, float, float, int)>(XI_GAME(0x1005FA20u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG tagRECT * CMoActorRotationDriveTask_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<tagRECT * (__thiscall *)(void *, char)>(XI_GAME(0x1005FE30u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_VT_1032B940_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10060190u))(a0, a1); }
XI_ORIG int sub_100601F0(int a0, void * a1, void * a2, int a3) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, int)>(XI_GAME(0x100601F0u))(a0, a1, a2, a3); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BAF0_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x100602D0u))(a0, a1); }
XI_ORIG int sub_100607E0(int a0, void * a1, void * a2, int a3) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, int)>(XI_GAME(0x100607E0u))(a0, a1, a2, a3); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BB38_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x100608B0u))(a0, a1); }
XI_ORIG __int16 sub_10060910(void * a0) { return reinterpret_cast<__int16 (__thiscall *)(void *)>(XI_GAME(0x10060910u))(a0); }
XI_ORIG __int16 sub_10060920(void * a0) { return reinterpret_cast<__int16 (__thiscall *)(void *)>(XI_GAME(0x10060920u))(a0); }
XI_ORIG int sub_10060930(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10060930u))(a0); }
XI_ORIG double sub_10060940(void * a0) { return reinterpret_cast<double (__thiscall *)(void *)>(XI_GAME(0x10060940u))(a0); }
XI_ORIG int sub_10060950(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10060950u))(a0); }
XI_ORIG int sub_10060990(int a0, void * a1, int a2, int a3, int a4, char a5) { return reinterpret_cast<int (__thiscall *)(int, void *, int, int, int, char)>(XI_GAME(0x10060990u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int sub_10060B30(int a0, void * a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(int, void *, int, int)>(XI_GAME(0x10060B30u))(a0, a1, a2, a3); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BBAC_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10060BB0u))(a0, a1); }
XI_ORIG int sub_10060DA0(int a0, void * a1, void * a2, void * a3, int a4, __int16 a5, __int16 a6, int a7, int a8, int a9, int a10, int a11) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, void *, int, __int16, __int16, int, int, int, int, int)>(XI_GAME(0x10060DA0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BBF4_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10060E60u))(a0, a1); }
XI_ORIG int * sub_10060F80(void * a0, void * a1, void * a2, int a3) { return reinterpret_cast<int * (__thiscall *)(void *, void *, void *, int)>(XI_GAME(0x10060F80u))(a0, a1, a2, a3); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BC68_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10061010u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BC84_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10061140u))(a0, a1); }
XI_ORIG int sub_10061240(int a0, void * a1, void * a2, int a3, int a4) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, int, int)>(XI_GAME(0x10061240u))(a0, a1, a2, a3, a4); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BCCC_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10061300u))(a0, a1); }
XI_ORIG int sub_100613D0(int a0, void * a1, void * a2, int a3, unsigned int a4) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, int, unsigned int)>(XI_GAME(0x100613D0u))(a0, a1, a2, a3, a4); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BD14_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x100614B0u))(a0, a1); }
XI_ORIG int sub_100615B0(int a0, void * a1, void * a2, int a3, int a4, char a5) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, int, int, char)>(XI_GAME(0x100615B0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BD5C_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x100616B0u))(a0, a1); }
XI_ORIG _DWORD * sub_100617B0(void * a0, void * a1, void * a2, int a3, int a4, int a5) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, void *, int, int, int)>(XI_GAME(0x100617B0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BDA4_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10061830u))(a0, a1); }
XI_ORIG void * sub_100618A0(void * a0, void * a1) { return reinterpret_cast<void * (__thiscall *)(void *, void *)>(XI_GAME(0x100618A0u))(a0, a1); }
XI_ORIG int sub_10061960(int a0, void * a1, void * a2, int a3, int a4, int a5) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, int, int, int)>(XI_GAME(0x10061960u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BDEC_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10061A00u))(a0, a1); }
XI_ORIG int sub_10061AF0(int a0, void * a1, void * a2, void * a3, int a4) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, void *, int)>(XI_GAME(0x10061AF0u))(a0, a1, a2, a3, a4); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BE34_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10061B80u))(a0, a1); }
XI_ORIG int sub_10061C70(int a0, void * a1, void * a2, void * a3, int a4) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, void *, int)>(XI_GAME(0x10061C70u))(a0, a1, a2, a3, a4); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BE7C_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10061D00u))(a0, a1); }
XI_ORIG int sub_10061E00(int a0, void * a1, void * a2, void * a3, int a4, int a5) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, void *, int, int)>(XI_GAME(0x10061E00u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BEC4_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10061EA0u))(a0, a1); }
XI_ORIG int sub_10061FD0(int a0, void * a1, void * a2, void * a3, int a4, void * a5, int a6) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, void *, int, void *, int)>(XI_GAME(0x10061FD0u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BF0C_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10062090u))(a0, a1); }
XI_ORIG int sub_10062180(int a0, void * a1, void * a2, void * a3, int a4, int a5) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, void *, int, int)>(XI_GAME(0x10062180u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BF54_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10062210u))(a0, a1); }
XI_ORIG int sub_10062310(int a0, void * a1, void * a2, int a3) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, int)>(XI_GAME(0x10062310u))(a0, a1, a2, a3); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BF9C_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x100623B0u))(a0, a1); }
XI_ORIG int sub_100624B0(int a0, void * a1, void * a2, int a3) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, int)>(XI_GAME(0x100624B0u))(a0, a1, a2, a3); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BFE4_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10062550u))(a0, a1); }
XI_ORIG int sub_10062650(int a0, void * a1, void * a2, int a3) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, int)>(XI_GAME(0x10062650u))(a0, a1, a2, a3); }
XI_ORIG _DWORD * CMoAttachments_VT_1032C02C_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10062700u))(a0, a1); }
XI_ORIG int sub_10062770(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10062770u))(a0); }
XI_ORIG int sub_100627D0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100627D0u))(a0); }
XI_ORIG int sub_10062830(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x10062830u))(a0, a1); }
XI_ORIG int sub_10062880(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x10062880u))(a0, a1); }
XI_ORIG int sub_100628D0(void * a0, int a1, void * a2) { return reinterpret_cast<int (__thiscall *)(void *, int, void *)>(XI_GAME(0x100628D0u))(a0, a1, a2); }
XI_ORIG _DWORD * sub_10062900(void * a0, void * a1, int a2) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, int)>(XI_GAME(0x10062900u))(a0, a1, a2); }
XI_ORIG void sub_10062D50(void * a0, int a1, void * a2, char a3) { reinterpret_cast<void (__thiscall *)(void *, int, void *, char)>(XI_GAME(0x10062D50u))(a0, a1, a2, a3); }
XI_ORIG void sub_10062DE0(int a0, void * a1) { reinterpret_cast<void (__thiscall *)(int, void *)>(XI_GAME(0x10062DE0u))(a0, a1); }
XI_ORIG _DWORD * CMoSpline_Destructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x100634A0u))(a0); }
XI_ORIG int sub_10063B20(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10063B20u))(a0); }
XI_ORIG _DWORD * sub_10063B50(void * a0, int a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int)>(XI_GAME(0x10063B50u))(a0, a1); }
XI_ORIG _DWORD * MoProgElem_FreeBuffer40(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10063C60u))(a0); }
XI_ORIG void sub_100683D0(int a0) { reinterpret_cast<void (__thiscall *)(int)>(XI_GAME(0x100683D0u))(a0); }
XI_ORIG int sub_100683F0(int a0, void * a1) { return reinterpret_cast<int (__thiscall *)(int, void *)>(XI_GAME(0x100683F0u))(a0, a1); }
XI_ORIG int sub_100684F0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x100684F0u))(a0, a1); }
XI_ORIG int sub_10068500(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10068500u))(a0, a1); }
XI_ORIG int sub_10068510(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10068510u))(a0, a1); }
XI_ORIG _DWORD * sub_100685C0(void * a0, void * a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *)>(XI_GAME(0x100685C0u))(a0, a1); }
XI_ORIG int sub_100685F0(void * a0, int a1, float a2) { return reinterpret_cast<int (__thiscall *)(void *, int, float)>(XI_GAME(0x100685F0u))(a0, a1, a2); }
XI_ORIG int sub_10068890(void * a0, int a1, float a2) { return reinterpret_cast<int (__thiscall *)(void *, int, float)>(XI_GAME(0x10068890u))(a0, a1, a2); }
XI_ORIG int sub_10068AE0(void * a0, int a1, float a2) { return reinterpret_cast<int (__thiscall *)(void *, int, float)>(XI_GAME(0x10068AE0u))(a0, a1, a2); }
XI_ORIG int sub_10068CE0(void * a0, int a1, float a2) { return reinterpret_cast<int (__thiscall *)(void *, int, float)>(XI_GAME(0x10068CE0u))(a0, a1, a2); }
XI_ORIG bool sub_10068EF0(void * a0, void * a1) { return reinterpret_cast<bool (__thiscall *)(void *, void *)>(XI_GAME(0x10068EF0u))(a0, a1); }
XI_ORIG bool sub_10068F80(void * a0, void * a1) { return reinterpret_cast<bool (__thiscall *)(void *, void *)>(XI_GAME(0x10068F80u))(a0, a1); }
XI_ORIG bool sub_10069010(void * a0, void * a1) { return reinterpret_cast<bool (__thiscall *)(void *, void *)>(XI_GAME(0x10069010u))(a0, a1); }
XI_ORIG bool sub_100690A0(void * a0, void * a1) { return reinterpret_cast<bool (__thiscall *)(void *, void *)>(XI_GAME(0x100690A0u))(a0, a1); }
XI_ORIG bool sub_10069130(void * a0, void * a1) { return reinterpret_cast<bool (__thiscall *)(void *, void *)>(XI_GAME(0x10069130u))(a0, a1); }
XI_ORIG bool sub_100691C0(void * a0, void * a1) { return reinterpret_cast<bool (__thiscall *)(void *, void *)>(XI_GAME(0x100691C0u))(a0, a1); }
XI_ORIG _DWORD * sub_10069D30(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10069D30u))(a0); }
XI_ORIG char CMoDX_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10069D70u))(a0); }
XI_ORIG _BYTE * CMoDX_Initialize(int a0, int a1) { return reinterpret_cast<_BYTE * (__thiscall *)(int, int)>(XI_GAME(0x10069DD0u))(a0, a1); }
XI_ORIG int nullsub_43(_DWORD a0) { return reinterpret_cast<int (__thiscall *)(_DWORD)>(XI_GAME(0x10069E90u))(a0); }
XI_ORIG char sub_10069EE0(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10069EE0u))(a0); }
XI_ORIG void sub_10069F90(int a0) { reinterpret_cast<void (__thiscall *)(int)>(XI_GAME(0x10069F90u))(a0); }
XI_ORIG int sub_1006A060(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(XI_GAME(0x1006A060u))(a0, a1, a2, a3); }
XI_ORIG int sub_1006A070(int a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(int, int, int, int)>(XI_GAME(0x1006A070u))(a0, a1, a2, a3); }
XI_ORIG int sub_1006A0E0(int a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(int, int, int)>(XI_GAME(0x1006A0E0u))(a0, a1, a2); }
XI_ORIG _DWORD * CMoDX_CreateTexture(void * a0, unsigned __int16 a1, unsigned __int16 a2, void * a3) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, unsigned __int16, unsigned __int16, void *)>(XI_GAME(0x1006A150u))(a0, a1, a2, a3); }
XI_ORIG int sub_1006A180(int a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<int (__thiscall *)(int, int, int, int, int)>(XI_GAME(0x1006A180u))(a0, a1, a2, a3, a4); }
XI_ORIG int sub_1006A420(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1006A420u))(a0); }
XI_ORIG int CMoDX_SetWorldMatrix(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x1006A480u))(a0, a1); }
XI_ORIG int CMoDX_GetWorldMatrix(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x1006A4A0u))(a0, a1); }
XI_ORIG int CMoDX_PushViewMatrix(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1006A4C0u))(a0); }
XI_ORIG int CMoDX_PopViewMatrix(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x1006A4F0u))(a0); }
XI_ORIG int CMoDX_SetViewMatrix(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x1006A520u))(a0, a1); }
XI_ORIG int Gfx_CalcFVFStride(unsigned int a0) { return reinterpret_cast<int (__cdecl *)(unsigned int)>(XI_GAME(0x1006A560u))(a0); }
XI_ORIG _DWORD * sub_1006AD30(void * a0, void * a1) { return reinterpret_cast<_DWORD * (__cdecl *)(void *, void *)>(XI_GAME(0x1006AD30u))(a0, a1); }
XI_ORIG exception * fn_1006AD90(void * a0) { return reinterpret_cast<exception * (__thiscall *)(void *)>(XI_GAME(0x1006AD90u))(a0); }
XI_ORIG int sub_1006ADF0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1006ADF0u))(a0); }
XI_ORIG int StateBlock_ApplyRSTSS(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1006AE30u))(a0, a1, a2); }
XI_ORIG int sub_1006AF00(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x1006AF00u))(a0, a1); }
XI_ORIG int StateBlock_ApplyTSS(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1006AF90u))(a0, a1); }
XI_ORIG _DWORD * CMoDxStatusMng_Constructor(int a0) { return reinterpret_cast<_DWORD * (__thiscall *)(int)>(XI_GAME(0x1006B050u))(a0); }
XI_ORIG void CMoDxStatusMng_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1006B090u))(a0); }
XI_ORIG _DWORD * VBPool_Constructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1006B0C0u))(a0); }
XI_ORIG char sub_1006B0E0(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1006B0E0u))(a0); }
XI_ORIG int sub_1006B1D0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1006B1D0u))(a0, a1); }
XI_ORIG char CMoTask_VT_10329C70_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1006B250u))(a0); }
XI_ORIG _DWORD * CMoDX_CreateAndInitSystem(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1006B2B0u))(a0); }
XI_ORIG int sub_1006BC10(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1006BC10u))(a0, a1); }
XI_ORIG int sub_1006BC80(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1006BC80u))(a0); }
XI_ORIG int Rand_SeedMT19937(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x1006BD90u))(a0); }
XI_ORIG unsigned int sub_1006BF60(unsigned int a0) { return reinterpret_cast<unsigned int (__cdecl *)(unsigned int)>(XI_GAME(0x1006BF60u))(a0); }
XI_ORIG _DWORD * Mem_AllocWithHeader12(int a0, int a1) { return reinterpret_cast<_DWORD * (__cdecl *)(int, int)>(XI_GAME(0x1006C0F0u))(a0, a1); }
XI_ORIG _DWORD * Mem_FreeWithHeader12(int a0) { return reinterpret_cast<_DWORD * (__cdecl *)(int)>(XI_GAME(0x1006C110u))(a0); }
XI_ORIG CMoOcclusionMng_vtbl ** CMoOcclusionMng_Constructor(void * a0) { return reinterpret_cast<CMoOcclusionMng_vtbl ** (__thiscall *)(void *)>(XI_GAME(0x1006C140u))(a0); }
XI_ORIG int CMoOcclusionMng_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1006C1E0u))(a0); }
XI_ORIG _DWORD * j_OcclusionMng_CreateTarget(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1006C210u))(a0); }
XI_ORIG _DWORD * OcclusionMng_CreateTarget(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1006C220u))(a0); }
XI_ORIG int OcclusionQuery_Init(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x1006C260u))(a0); }
XI_ORIG int OcclusionQuery_Release(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1006C270u))(a0); }
XI_ORIG int Occlusion_QueryScreenRect(void * a0, void * a1, int a2, int a3, int a4, int a5, int a6, int a7, char a8, int a9) { return reinterpret_cast<int (__thiscall *)(void *, void *, int, int, int, int, int, int, char, int)>(XI_GAME(0x1006C280u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9); }
XI_ORIG int Occlusion_QueryScreenRectImpl(void * a0, void * a1, int a2, int a3, int a4, int a5, int a6, int a7, char a8, int a9) { return reinterpret_cast<int (__thiscall *)(void *, void *, int, int, int, int, int, int, char, int)>(XI_GAME(0x1006C300u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9); }
XI_ORIG int sub_1006C530(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8) { return reinterpret_cast<int (__stdcall *)(int, int, int, int, int, int, int, int, int)>(XI_GAME(0x1006C530u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG void unknown_libname_59() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1006C890u))(); }
XI_ORIG void unknown_libname_60() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1006C8A0u))(); }
XI_ORIG int sub_1006C8B0(void * a0, char a1) { return reinterpret_cast<int (__thiscall *)(void *, char)>(XI_GAME(0x1006C8B0u))(a0, a1); }
XI_ORIG int OcclusionMng_Prepare(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1006C9B0u))(a0); }
XI_ORIG char sub_1006CA20(float a0) { return reinterpret_cast<char (__cdecl *)(float)>(XI_GAME(0x1006CA20u))(a0); }
XI_ORIG _DWORD * CreateProcessorInstance() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x1006CB50u))(); }
XI_ORIG void nullsub_44() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1006CF90u))(); }
XI_ORIG float * CMoProcessor_VT_1032C368_vf012(void * a0, void * a1, float a2) { return reinterpret_cast<float * (__stdcall *)(void *, void *, float)>(XI_GAME(0x1006D3B0u))(a0, a1, a2); }
XI_ORIG void CMoProcessor_Vec3Normalize(void * a0) { reinterpret_cast<void (__stdcall *)(void *)>(XI_GAME(0x1006D5D0u))(a0); }
XI_ORIG float * CMoProcessor_Vec4TransformB(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__stdcall *)(void *, void *, void *)>(XI_GAME(0x1006D7E0u))(a0, a1, a2); }
XI_ORIG float * CMoProcessor_Vec4Transform(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__stdcall *)(void *, void *, void *)>(XI_GAME(0x1006D870u))(a0, a1, a2); }
XI_ORIG float * CMoProcessor_Vec3Transform(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__stdcall *)(void *, void *, void *)>(XI_GAME(0x1006D900u))(a0, a1, a2); }
XI_ORIG float * CMoProcessor_VT_1032C368_vf021(void * a0, void * a1, void * a2, float a3) { return reinterpret_cast<float * (__stdcall *)(void *, void *, void *, float)>(XI_GAME(0x1006D980u))(a0, a1, a2, a3); }
XI_ORIG void CMoProcessor_ProjectPointsToScreen(void * a0, void * a1, int a2, int a3, void * a4, void * a5, float a6, float a7) { reinterpret_cast<void (__thiscall *)(void *, void *, int, int, void *, void *, float, float)>(XI_GAME(0x1006E7D0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG CMoProcessor * CMoProcessor_CalcScreenBounds(void * a0, void * a1, int a2, int a3, void * a4) { return reinterpret_cast<CMoProcessor * (__stdcall *)(void *, void *, int, int, void *)>(XI_GAME(0x1006E970u))(a0, a1, a2, a3, a4); }
XI_ORIG float * CMoProcessor_ProjectVertsAndBounds(void * a0, void * a1, int a2, void * a3, void * a4, float a5, float a6) { return reinterpret_cast<float * (__stdcall *)(void *, void *, int, void *, void *, float, float)>(XI_GAME(0x1006EB70u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG CMoProcessor * CMoProcessor_CalcScreenBoundsAndVisible(void * a0, void * a1, void * a2, int a3, int a4, int a5, float a6, float a7) { return reinterpret_cast<CMoProcessor * (__thiscall *)(void *, void *, void *, int, int, int, float, float)>(XI_GAME(0x1006ED10u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG char CMoTask_VT_1032C490_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10070F80u))(a0); }
XI_ORIG __int16 ResHandle_FreeIfUnreferenced(void * a0) { return reinterpret_cast<__int16 (__thiscall *)(void *)>(XI_GAME(0x10071050u))(a0); }
XI_ORIG bool ResourceHandle_IsResident(void * a0) { return reinterpret_cast<bool (__fastcall *)(void *)>(XI_GAME(0x10071060u))(a0); }
XI_ORIG int sub_100710F0(void * a0, int a1, int a2, void * a3, void * a4) { return reinterpret_cast<int (__thiscall *)(void *, int, int, void *, void *)>(XI_GAME(0x100710F0u))(a0, a1, a2, a3, a4); }
XI_ORIG _DWORD * CMoAttachments_VT_1032C4E8_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10071180u))(a0, a1); }
XI_ORIG void nullsub_171() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100719B0u))(); }
XI_ORIG unsigned int ** CMoAttachments_VT_1032C628_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<unsigned int ** (__thiscall *)(void *, char)>(XI_GAME(0x10071C10u))(a0, a1); }
XI_ORIG unsigned int ** CMoResource_VT_1032C588_ScalarDeletingDtor_10071C60(void * a0, char a1) { return reinterpret_cast<unsigned int ** (__thiscall *)(void *, char)>(XI_GAME(0x10071C60u))(a0, a1); }
XI_ORIG unsigned int * CMoResource_Destructor(void * a0) { return reinterpret_cast<unsigned int * (__thiscall *)(void *)>(XI_GAME(0x10071DE0u))(a0); }
XI_ORIG int * CMoResource_AddRefChain(int a0) { return reinterpret_cast<int * (__fastcall *)(int)>(XI_GAME(0x10071E90u))(a0); }
XI_ORIG char CMoResource_ReleaseChain(int a0, int a1) { return reinterpret_cast<char (__fastcall *)(int, int)>(XI_GAME(0x10071EC0u))(a0, a1); }
XI_ORIG _DWORD * sub_10071F50(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10071F50u))(a0); }
XI_ORIG int CMoResource_RegisterInTypeList(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10072030u))(a0); }
XI_ORIG __int16 CMoResource_Destroy(int a0) { return reinterpret_cast<__int16 (__thiscall *)(int)>(XI_GAME(0x100721F0u))(a0); }
XI_ORIG _DWORD * CMoResource_GetParent(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10072270u))(a0); }
XI_ORIG _DWORD * CMoResource_FindByTypeAndId(void * a0, void * a1, int a2, int a3) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, int, int)>(XI_GAME(0x10072290u))(a0, a1, a2, a3); }
XI_ORIG _DWORD * CMoResource_FindInScope(void * a0, int a1, int a2, int a3) { return reinterpret_cast<_DWORD * (__stdcall *)(void *, int, int, int)>(XI_GAME(0x10072430u))(a0, a1, a2, a3); }
XI_ORIG _DWORD * YmResourceFile_FindResourceUnder(int a0, void * a1, int a2, int a3) { return reinterpret_cast<_DWORD * (__thiscall *)(int, void *, int, int)>(XI_GAME(0x100724C0u))(a0, a1, a2, a3); }
XI_ORIG _DWORD * sub_100725C0(void * a0, void * a1, int a2, int a3, int a4) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, int, int, int)>(XI_GAME(0x100725C0u))(a0, a1, a2, a3, a4); }
XI_ORIG int unknown_libname_61() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100729C0u))(); }
XI_ORIG char CMoTask_VT_1032C908_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100729E0u))(a0); }
XI_ORIG int sub_10072DB0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10072DB0u))(a0); }
XI_ORIG int * MeshResourceMng_LoadSystemResources(int a0, int a1) { return reinterpret_cast<int * (__thiscall *)(int, int)>(XI_GAME(0x10072ED0u))(a0, a1); }
XI_ORIG int ResourceHandle_Deref(int a0) { return reinterpret_cast<int (__stdcall *)(int)>(XI_GAME(0x10072FB0u))(a0); }
XI_ORIG int * sub_10072FD0(void * a0, void * a1, int a2) { return reinterpret_cast<int * (__thiscall *)(void *, void *, int)>(XI_GAME(0x10072FD0u))(a0, a1, a2); }
XI_ORIG _DWORD * sub_100730E0(void * a0, int a1) { return reinterpret_cast<_DWORD * (__stdcall *)(void *, int)>(XI_GAME(0x100730E0u))(a0, a1); }
XI_ORIG int * sub_100731B0(void * a0, void * a1, int a2) { return reinterpret_cast<int * (__thiscall *)(void *, void *, int)>(XI_GAME(0x100731B0u))(a0, a1, a2); }
XI_ORIG __int16 MoRes_FreeIfUnreferenced(void * a0) { return reinterpret_cast<__int16 (__stdcall *)(void *)>(XI_GAME(0x10073B20u))(a0); }
XI_ORIG _DWORD * MeshResourceMng_AllocHandle(void * a0, void * a1, int a2) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, int)>(XI_GAME(0x10074140u))(a0, a1, a2); }
XI_ORIG unsigned int * NodePool_ReleaseNode(void * a0, void * a1) { return reinterpret_cast<unsigned int * (__thiscall *)(void *, void *)>(XI_GAME(0x10074180u))(a0, a1); }
XI_ORIG int GetGlobal_1047D114() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10074290u))(); }
XI_ORIG char ResourceIter_FindFirst(void * a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int, int)>(XI_GAME(0x100742B0u))(a0, a1, a2, a3, a4); }
XI_ORIG int ResourceIter_FindNext(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x100745C0u))(a0, a1); }
XI_ORIG int MoProcessor_CallVSlot49(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100746A0u))(a0); }
XI_ORIG _DWORD * CMoTask_Constructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10074740u))(a0); }
XI_ORIG _DWORD * sub_10074790(void * a0, void * a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *)>(XI_GAME(0x10074790u))(a0, a1); }
XI_ORIG char CMoTask_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100747C0u))(a0); }
XI_ORIG int sub_100747D0(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x100747D0u))(a0, a1); }
XI_ORIG int CMoTask_ResetFields(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x100748F0u))(a0); }
XI_ORIG _DWORD * sub_10074920(void * a0, unsigned int a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, unsigned int)>(XI_GAME(0x10074920u))(a0, a1); }
XI_ORIG int CMoTask_RegisterWithMng(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x100749B0u))(a0, a1); }
XI_ORIG int CMoTaskMng_Forward10075120(int a0) { return reinterpret_cast<int (__stdcall *)(int)>(XI_GAME(0x10074A10u))(a0); }
XI_ORIG int sub_10074A30(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10074A30u))(a0); }
XI_ORIG char CMoTaskMng_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10074B60u))(a0); }
XI_ORIG int sub_10074BA0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10074BA0u))(a0); }
XI_ORIG int sub_10074C30(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10074C30u))(a0); }
XI_ORIG int Gfx_InitCubeMapTextures() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10075280u))(); }
XI_ORIG int TexRef_ResolveByName(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10075520u))(a0, a1); }
XI_ORIG void TexRef_EnsureRenderTarget(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100755A0u))(a0); }
XI_ORIG int sub_10075700(void * a0, int a1, float a2, float a3, float a4) { return reinterpret_cast<int (__thiscall *)(void *, int, float, float, float)>(XI_GAME(0x10075700u))(a0, a1, a2, a3, a4); }
XI_ORIG void unknown_libname_63() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10075950u))(); }
XI_ORIG int TexRef_SetTexture(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10075960u))(a0, a1); }
XI_ORIG void unknown_libname_64() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10075C30u))(); }
XI_ORIG _DWORD * ActorHandle_Constructor(int a0) { return reinterpret_cast<_DWORD * (__thiscall *)(int)>(XI_GAME(0x100814A0u))(a0); }
XI_ORIG void ActorHandle_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100814E0u))(a0); }
XI_ORIG unsigned int sub_100814F0(int a0, int a1) { return reinterpret_cast<unsigned int (__thiscall *)(int, int)>(XI_GAME(0x100814F0u))(a0, a1); }
XI_ORIG int ActorHandle_Resolve(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10081550u))(a0); }
XI_ORIG int sub_10081600(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10081600u))(a0); }
XI_ORIG int Actor_ResetDrawLists() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10081A50u))(); }
XI_ORIG int Actor_UnlinkAllAttachLists(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10082B10u))(a0); }
XI_ORIG char CXiActor_NameCalc(void * a0, void * a1, int a2, void * a3, int a4) { return reinterpret_cast<char (__thiscall *)(void *, void *, int, void *, int)>(XI_GAME(0x100830C0u))(a0, a1, a2, a3, a4); }
XI_ORIG BOOL sub_10083220(void * a0) { return reinterpret_cast<BOOL (__thiscall *)(void *)>(XI_GAME(0x10083220u))(a0); }
XI_ORIG int sub_10083230(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10083230u))(a0, a1); }
XI_ORIG BOOL sub_10083340(void * a0) { return reinterpret_cast<BOOL (__thiscall *)(void *)>(XI_GAME(0x10083340u))(a0); }
XI_ORIG char CXiActor_IsModelMissingOrFlag2000(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10084140u))(a0); }
XI_ORIG std::codecvt_base * sub_10084160(void * a0, void * a1) { return reinterpret_cast<std::codecvt_base * (__thiscall *)(void *, void *)>(XI_GAME(0x10084160u))(a0, a1); }
XI_ORIG _DWORD * sub_100841A0(void * a0) { return reinterpret_cast<_DWORD * (__cdecl *)(void *)>(XI_GAME(0x100841A0u))(a0); }
XI_ORIG bool CXiActor_ModelTest95790(void * a0, int a1) { return reinterpret_cast<bool (__thiscall *)(void *, int)>(XI_GAME(0x10084330u))(a0, a1); }
XI_ORIG bool CXiActor_ModelTest95680(void * a0, int a1) { return reinterpret_cast<bool (__thiscall *)(void *, int)>(XI_GAME(0x10084350u))(a0, a1); }
XI_ORIG bool CXiActor_ModelTest956A0(void * a0, int a1) { return reinterpret_cast<bool (__thiscall *)(void *, int)>(XI_GAME(0x10084370u))(a0, a1); }
XI_ORIG int CXiActor_GetModelField170(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10084390u))(a0); }
XI_ORIG bool CXiActor_HasNonzeroField150or158(void * a0) { return reinterpret_cast<bool (__thiscall *)(void *)>(XI_GAME(0x100843B0u))(a0); }
XI_ORIG int CXiActor_GetModelByteEE(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10084400u))(a0); }
XI_ORIG int CXiActor_GetModelByteEF(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10084420u))(a0); }
XI_ORIG int CXiActor_GetModelPartWord(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10084480u))(a0, a1); }
XI_ORIG int CXiActor_GetModelFlags120Top2(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10084500u))(a0); }
XI_ORIG int CXiActor_GetModelFlags130Top2(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10084520u))(a0); }
XI_ORIG double CXiActor_GetModelFloat9AEC0(void * a0) { return reinterpret_cast<double (__thiscall *)(void *)>(XI_GAME(0x10084540u))(a0); }
XI_ORIG BOOL CXiActor_IsModelMissingOrSuppressed(int a0) { return reinterpret_cast<BOOL (__thiscall *)(int)>(XI_GAME(0x10084580u))(a0); }
XI_ORIG BOOL CXiActor_TestModelFlag128_8(void * a0) { return reinterpret_cast<BOOL (__thiscall *)(void *)>(XI_GAME(0x100845B0u))(a0); }
XI_ORIG int sub_100845E0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100845E0u))(a0); }
XI_ORIG BOOL CXiActor_TestModelFlags100000_200000_4000(void * a0) { return reinterpret_cast<BOOL (__thiscall *)(void *)>(XI_GAME(0x10084640u))(a0); }
XI_ORIG bool sub_10084670(void * a0) { return reinterpret_cast<bool (__thiscall *)(void *)>(XI_GAME(0x10084670u))(a0); }
XI_ORIG __int16 sub_10084690(void * a0) { return reinterpret_cast<__int16 (__thiscall *)(void *)>(XI_GAME(0x10084690u))(a0); }
XI_ORIG BOOL sub_100846D0(void * a0) { return reinterpret_cast<BOOL (__thiscall *)(void *)>(XI_GAME(0x100846D0u))(a0); }
XI_ORIG unsigned int sub_100846F0(void * a0, char a1) { return reinterpret_cast<unsigned int (__thiscall *)(void *, char)>(XI_GAME(0x100846F0u))(a0, a1); }
XI_ORIG int CXiActor_GetModelByte1DB(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10084720u))(a0); }
XI_ORIG int sub_10085000(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10085000u))(a0, a1); }
XI_ORIG int CXiActor_SpawnResourceGenerators(int a0, void * a1) { return reinterpret_cast<int (__thiscall *)(int, void *)>(XI_GAME(0x10085060u))(a0, a1); }
XI_ORIG bool CXiActor_IsInModelSlotTable(void * a0) { return reinterpret_cast<bool (__thiscall *)(void *)>(XI_GAME(0x10085490u))(a0); }
XI_ORIG void CXiActor_KillGeneratorClones(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10085570u))(a0); }
XI_ORIG int sub_10085BF0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10085BF0u))(a0, a1); }
XI_ORIG float * sub_10085E40(void * a0) { return reinterpret_cast<float * (__cdecl *)(void *)>(XI_GAME(0x10085E40u))(a0); }
XI_ORIG void Actor_DrawWorldText(void * a0, void * a1, float a2, unsigned int a3, int a4) { reinterpret_cast<void (__cdecl *)(void *, void *, float, unsigned int, int)>(XI_GAME(0x10086210u))(a0, a1, a2, a3, a4); }
XI_ORIG bool sub_10087060() { return reinterpret_cast<bool (__cdecl *)()>(XI_GAME(0x10087060u))(); }
XI_ORIG int Call1008A0E0With2() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1008A0D0u))(); }
XI_ORIG bool sub_1008E130(void * a0) { return reinterpret_cast<bool (__thiscall *)(void *)>(XI_GAME(0x1008E130u))(a0); }
XI_ORIG char Unit10086E90_ResetGlobals() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x10093720u))(); }
XI_ORIG bool Net_SendPacket00COnce() { return reinterpret_cast<bool (__cdecl *)()>(XI_GAME(0x10093DB0u))(); }
XI_ORIG char * Entity_BuildDisplayName(int a0) { return reinterpret_cast<char * (__thiscall *)(int)>(XI_GAME(0x10097840u))(a0); }
XI_ORIG int Entity_GetIndexedPtr(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10097A40u))(a0); }
XI_ORIG bool Entity_IsModelIdB22toB31(void * a0) { return reinterpret_cast<bool (__thiscall *)(void *)>(XI_GAME(0x10098320u))(a0); }
XI_ORIG char CMoTask_VT_1032D6D4_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100A2810u))(a0); }
XI_ORIG _DWORD * CXiCollisionActor_Constructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x100A4C20u))(a0); }
XI_ORIG int CXiCollisionActor_vf8(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100A4ED0u))(a0); }
XI_ORIG int sub_100A57C0(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x100A57C0u))(a0, a1); }
XI_ORIG int sub_100A5E10(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x100A5E10u))(a0); }
XI_ORIG char sub_100A6070(char a0) { return reinterpret_cast<char (__stdcall *)(char)>(XI_GAME(0x100A6070u))(a0); }
XI_ORIG char CXiControlActor_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100A8CA0u))(a0); }
XI_ORIG void sub_100A8E30(void * a0, void * a1) { reinterpret_cast<void (__thiscall *)(void *, void *)>(XI_GAME(0x100A8E30u))(a0, a1); }
XI_ORIG char CMoTask_VT_1032E808_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100A9D30u))(a0); }
XI_ORIG _DWORD * CMoTask_VT_1032E808_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x100A9DB0u))(a0, a1); }
XI_ORIG int sub_100AA380(int a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(int, int, int, int)>(XI_GAME(0x100AA380u))(a0, a1, a2, a3); }
XI_ORIG int sub_100AA3B0(int a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(int, int, int)>(XI_GAME(0x100AA3B0u))(a0, a1, a2); }
XI_ORIG void sub_100AA8E0(void * a0, int a1, int a2, int a3, int a4) { reinterpret_cast<void (__thiscall *)(void *, int, int, int, int)>(XI_GAME(0x100AA8E0u))(a0, a1, a2, a3, a4); }
XI_ORIG void sub_100AA940(void * a0, int a1, int a2, float a3, int a4, float a5) { reinterpret_cast<void (__thiscall *)(void *, int, int, float, int, float)>(XI_GAME(0x100AA940u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int sub_100AAD40(void * a0, int a1, float a2) { return reinterpret_cast<int (__thiscall *)(void *, int, float)>(XI_GAME(0x100AAD40u))(a0, a1, a2); }
XI_ORIG int sub_100ABEA0(int a0, void * a1, char a2) { return reinterpret_cast<int (__thiscall *)(int, void *, char)>(XI_GAME(0x100ABEA0u))(a0, a1, a2); }
XI_ORIG _DWORD * sub_100AC290(int a0, int a1) { return reinterpret_cast<_DWORD * (__thiscall *)(int, int)>(XI_GAME(0x100AC290u))(a0, a1); }
XI_ORIG int sub_100ADEE0(void * a0, void * a1, int a2) { return reinterpret_cast<int (__cdecl *)(void *, void *, int)>(XI_GAME(0x100ADEE0u))(a0, a1, a2); }
XI_ORIG double sub_100C3BC0(void * a0, int a1) { return reinterpret_cast<double (__thiscall *)(void *, int)>(XI_GAME(0x100C3BC0u))(a0, a1); }
XI_ORIG double sub_100C3BD0(void * a0) { return reinterpret_cast<double (__thiscall *)(void *)>(XI_GAME(0x100C3BD0u))(a0); }
XI_ORIG int sub_100C3CE0(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x100C3CE0u))(a0); }
XI_ORIG int sub_100C51C0(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(XI_GAME(0x100C51C0u))(a0, a1); }
XI_ORIG int TargetActor_SlotHandler1() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100C5410u))(); }
XI_ORIG _DWORD * TargetActor_SlotHandler0() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x100C5440u))(); }
XI_ORIG _DWORD * TargetActor_SlotHandler4() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x100C54C0u))(); }
XI_ORIG char CXiSkeletonActor_DestructorBody(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x100C5C00u))(a0); }
XI_ORIG int CXiSkeletonActor_Init(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100C5D60u))(a0); }
XI_ORIG char sub_100C8280(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x100C8280u))(a0); }
XI_ORIG void sub_100C83C0(int a0, void * a1) { reinterpret_cast<void (__thiscall *)(int, void *)>(XI_GAME(0x100C83C0u))(a0, a1); }
XI_ORIG void sub_100C8BB0(void * a0, float a1, void * a2, void * a3) { reinterpret_cast<void (__thiscall *)(void *, float, void *, void *)>(XI_GAME(0x100C8BB0u))(a0, a1, a2, a3); }
XI_ORIG int sub_100C8ED0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x100C8ED0u))(a0, a1); }
XI_ORIG int sub_100C9330(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x100C9330u))(a0, a1); }
XI_ORIG int sub_100C9810(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x100C9810u))(a0, a1); }
XI_ORIG int sub_100C9C10(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x100C9C10u))(a0, a1); }
XI_ORIG int sub_100CA310(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x100CA310u))(a0, a1); }
XI_ORIG int sub_100CA730(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x100CA730u))(a0, a1); }
XI_ORIG void sub_100CA8C0(int a0) { reinterpret_cast<void (__thiscall *)(int)>(XI_GAME(0x100CA8C0u))(a0); }
XI_ORIG int sub_100CAA60(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100CAA60u))(a0); }
XI_ORIG int sub_100CAF90(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100CAF90u))(a0); }
XI_ORIG void sub_100CB390(void * a0, int a1) { reinterpret_cast<void (__thiscall *)(void *, int)>(XI_GAME(0x100CB390u))(a0, a1); }
XI_ORIG void sub_100CB400(void * a0, int a1) { reinterpret_cast<void (__thiscall *)(void *, int)>(XI_GAME(0x100CB400u))(a0, a1); }
XI_ORIG int Actor_SelectNearestLights(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100CB4E0u))(a0); }
XI_ORIG char sub_100CBD90(int a0) { return reinterpret_cast<char (__cdecl *)(int)>(XI_GAME(0x100CBD90u))(a0); }
XI_ORIG void PerActor_FourBucketStaggeredUpdate(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100CBDE0u))(a0); }
XI_ORIG void nullsub_276() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100CCE20u))(); }
XI_ORIG void nullsub_277() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100CCE30u))(); }
XI_ORIG void nullsub_278() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100CCE40u))(); }
XI_ORIG void nullsub_279() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100CCE50u))(); }
XI_ORIG void Actor_SetCameraFromModelPoints(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100CCE60u))(a0); }
XI_ORIG double sub_100CD040(void * a0) { return reinterpret_cast<double (__thiscall *)(void *)>(XI_GAME(0x100CD040u))(a0); }
XI_ORIG char * sub_100CD4E0(int a0, void * a1, int a2, int a3, int a4, float a5, float a6, unsigned int a7) { return reinterpret_cast<char * (__thiscall *)(int, void *, int, int, int, float, float, unsigned int)>(XI_GAME(0x100CD4E0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG BOOL sub_100CD920(int a0, int a1, int a2, int a3, int a4, float a5, float a6, int a7, int a8, int a9, void * a10, void * a11) { return reinterpret_cast<BOOL (__thiscall *)(int, int, int, int, int, float, float, int, int, int, void *, void *)>(XI_GAME(0x100CD920u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11); }
XI_ORIG char sub_100CDD60(int a0, int a1, int a2, float a3, int a4, int a5, float a6, float a7, int a8, int a9) { return reinterpret_cast<char (__thiscall *)(int, int, int, float, int, int, float, float, int, int)>(XI_GAME(0x100CDD60u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9); }
XI_ORIG char sub_100CDE70(int a0, int a1, int a2, int a3, float a4, float a5, int a6) { return reinterpret_cast<char (__thiscall *)(int, int, int, int, float, float, int)>(XI_GAME(0x100CDE70u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG char sub_100CDF00(int a0, int a1, int a2, int a3, float a4, float a5, int a6) { return reinterpret_cast<char (__thiscall *)(int, int, int, int, float, float, int)>(XI_GAME(0x100CDF00u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG void sub_100CDF90(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100CDF90u))(a0); }
XI_ORIG int sub_100CE180(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x100CE180u))(a0, a1); }
XI_ORIG _DWORD * sub_100CE250(void * a0, void * a1, int a2, int a3) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, int, int)>(XI_GAME(0x100CE250u))(a0, a1, a2, a3); }
XI_ORIG _DWORD * CXiSkeletonActor_ReverseFindRes(void * a0, void * a1, int a2, int a3, int a4, int a5, char a6) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, int, int, int, int, char)>(XI_GAME(0x100CE480u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG void CXiSkeletonActor_DrawNameIfVisible(void * a0, void * a1, void * a2, float a3, unsigned int a4, int a5) { reinterpret_cast<void (__thiscall *)(void *, void *, void *, float, unsigned int, int)>(XI_GAME(0x100D0880u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void CXiSkeletonActor_DrawName2IfVisible(void * a0, void * a1, void * a2, float a3) { reinterpret_cast<void (__thiscall *)(void *, void *, void *, float)>(XI_GAME(0x100D08B0u))(a0, a1, a2, a3); }
XI_ORIG _DWORD * sub_100D0B50(int a0, void * a1, int a2, void * a3, char a4) { return reinterpret_cast<_DWORD * (__thiscall *)(int, void *, int, void *, char)>(XI_GAME(0x100D0B50u))(a0, a1, a2, a3, a4); }
XI_ORIG int * sub_100D0D40(void * a0, void * a1, void * a2) { return reinterpret_cast<int * (__thiscall *)(void *, void *, void *)>(XI_GAME(0x100D0D40u))(a0, a1, a2); }
XI_ORIG void sub_100D0D70(int a0, int a1, void * a2) { reinterpret_cast<void (__thiscall *)(int, int, void *)>(XI_GAME(0x100D0D70u))(a0, a1, a2); }
XI_ORIG int sub_100D0E80(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(XI_GAME(0x100D0E80u))(a0, a1); }
XI_ORIG char sub_100D1070(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x100D1070u))(a0); }
XI_ORIG int sub_100D1190(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x100D1190u))(a0); }
XI_ORIG bool CXiSkeletonActor_IsReadCompleteBody(void * a0) { return reinterpret_cast<bool (__thiscall *)(void *)>(XI_GAME(0x100D12D0u))(a0); }
XI_ORIG char CXiSkeletonActor_OnSkeletonLoaded(int a0, int a1) { return reinterpret_cast<char (__cdecl *)(int, int)>(XI_GAME(0x100D2F40u))(a0, a1); }
XI_ORIG char CXiSkeletonActor_LoadSkeletonSync(int a0, int a1) { return reinterpret_cast<char (__thiscall *)(int, int)>(XI_GAME(0x100D2F60u))(a0, a1); }
XI_ORIG int sub_100D4100(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(XI_GAME(0x100D4100u))(a0, a1); }
XI_ORIG int sub_100D4220(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(XI_GAME(0x100D4220u))(a0, a1); }
XI_ORIG char CXiSkeletonActor_LoadSkeletonResource(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(XI_GAME(0x100D4330u))(a0, a1); }
XI_ORIG int sub_100D43E0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100D43E0u))(); }
XI_ORIG int CXiSkeletonActor_DeleteOwnedList(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100D44F0u))(a0); }
XI_ORIG void sub_100D4BC0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100D4BC0u))(a0); }
XI_ORIG int sub_100D5430(void * a0, char a1) { return reinterpret_cast<int (__thiscall *)(void *, char)>(XI_GAME(0x100D5430u))(a0, a1); }
XI_ORIG char sub_100D5490(void * a0, char a1) { return reinterpret_cast<char (__thiscall *)(void *, char)>(XI_GAME(0x100D5490u))(a0, a1); }
XI_ORIG void CXiSkeletonActor_CalcGroundTiltMatrix(void * a0, void * a1, float a2) { reinterpret_cast<void (__thiscall *)(void *, void *, float)>(XI_GAME(0x100D54C0u))(a0, a1, a2); }
XI_ORIG void CXiSkeletonActor_UpdateHeadLook(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100D5B10u))(a0); }
XI_ORIG void sub_100D5F10(void * a0, int a1, int a2) { reinterpret_cast<void (__thiscall *)(void *, int, int)>(XI_GAME(0x100D5F10u))(a0, a1, a2); }
XI_ORIG void sub_100D60E0(void * a0, int a1, int a2) { reinterpret_cast<void (__thiscall *)(void *, int, int)>(XI_GAME(0x100D60E0u))(a0, a1, a2); }
XI_ORIG bool sub_100D6240(void * a0, int a1) { return reinterpret_cast<bool (__thiscall *)(void *, int)>(XI_GAME(0x100D6240u))(a0, a1); }
XI_ORIG _DWORD * sub_100D6850(void * a0, void * a1, int a2, int a3, int a4) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, int, int, int)>(XI_GAME(0x100D6850u))(a0, a1, a2, a3, a4); }
XI_ORIG int Actor_PrepareDrawGlobals(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100D6D30u))(a0); }
XI_ORIG _DWORD * CXiSkeletonActorRes_Constructor(void * a0, int a1, int a2) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int, int)>(XI_GAME(0x100D7020u))(a0, a1, a2); }
XI_ORIG char CXiSkeletonActorRes_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100D7050u))(a0); }
XI_ORIG int CXiSkeletonActorRes_GetLinkedActor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100D7090u))(a0); }
XI_ORIG char sub_100DA6A0(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x100DA6A0u))(a0); }
XI_ORIG int GameMode_GetField98(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100DB100u))(a0); }
XI_ORIG int GameMode_PollPendingState(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100DB140u))(a0); }
XI_ORIG int GameMode_RequestState(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x100DB170u))(a0, a1, a2); }
XI_ORIG int sub_100DB790(int a0) { return reinterpret_cast<int (__stdcall *)(int)>(XI_GAME(0x100DB790u))(a0); }
XI_ORIG int GameMode_OpenTacticsUrl(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x100DBB50u))(a0, a1); }
XI_ORIG char sub_100DBE50() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x100DBE50u))(); }
XI_ORIG char sub_100FD3A0(unsigned int a0) { return reinterpret_cast<char (__cdecl *)(unsigned int)>(XI_GAME(0x100FD3A0u))(a0); }
XI_ORIG int sub_101037F0(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(XI_GAME(0x101037F0u))(a0, a1); }
XI_ORIG int ntGameTimeGet() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101039E0u))(); }
XI_ORIG int sub_10118260(int a0, int a1) { return reinterpret_cast<int (__thiscall *)(int, int)>(XI_GAME(0x10118260u))(a0, a1); }
XI_ORIG void nullsub_48() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10118520u))(); }
XI_ORIG bool sub_10118C10(void * a0, void * a1) { return reinterpret_cast<bool (__thiscall *)(void *, void *)>(XI_GAME(0x10118C10u))(a0, a1); }
XI_ORIG __int16 sub_10118CB0(void * a0, void * a1, void * a2) { return reinterpret_cast<__int16 (__thiscall *)(void *, void *, void *)>(XI_GAME(0x10118CB0u))(a0, a1, a2); }
XI_ORIG int sub_10118DB0(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x10118DB0u))(a0, a1, a2); }
XI_ORIG _WORD * sub_1011AC70(void * a0, __int16 a1, __int16 a2) { return reinterpret_cast<_WORD * (__thiscall *)(void *, __int16, __int16)>(XI_GAME(0x1011AC70u))(a0, a1, a2); }
XI_ORIG int sub_1011BA90(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x1011BA90u))(a0); }
XI_ORIG char Font_BindGlyphTexture(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x1011BAE0u))(a0); }
XI_ORIG int sub_1011BBA0(void * a0, __int16 a1, void * a2, float a3, float a4) { return reinterpret_cast<int (__thiscall *)(void *, __int16, void *, float, float)>(XI_GAME(0x1011BBA0u))(a0, a1, a2, a3, a4); }
XI_ORIG int Gfx_SetAdditiveBlend() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1011BDC0u))(); }
XI_ORIG void Font_DrawChar(__int16 a0, int a1, int a2, float a3, float a4) { reinterpret_cast<void (__cdecl *)(__int16, int, int, float, float)>(XI_GAME(0x1011BF40u))(a0, a1, a2, a3, a4); }
XI_ORIG int Gfx_SetCellBlendMode(int a0, int a1, int a2, int a3) { return reinterpret_cast<int (__cdecl *)(int, int, int, int)>(XI_GAME(0x1011C1F0u))(a0, a1, a2, a3); }
XI_ORIG int Font_AllocHighResImage(void * a0, int a1, int a2) { return reinterpret_cast<int (__stdcall *)(void *, int, int)>(XI_GAME(0x1011C5D0u))(a0, a1, a2); }
XI_ORIG char Tex_LoadSurfaceFromDatFile(void * a0, int a1) { return reinterpret_cast<char (__stdcall *)(void *, int)>(XI_GAME(0x1011C670u))(a0, a1); }
XI_ORIG void Font_LoadHighResTexture(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1011C720u))(a0); }
XI_ORIG int Font_ReleaseHighResTexture(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1011C810u))(a0); }
XI_ORIG bool Font_IsHighResTextureLoaded(int a0) { return reinterpret_cast<bool (__thiscall *)(int)>(XI_GAME(0x1011C850u))(a0); }
XI_ORIG int unknown_libname_88(_DWORD a0) { return reinterpret_cast<int (__thiscall *)(_DWORD)>(XI_GAME(0x1011C870u))(a0); }
XI_ORIG char sub_1011C8B0() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x1011C8B0u))(); }
XI_ORIG __int16 Font_DecodeNextChar(void * a0) { return reinterpret_cast<__int16 (__cdecl *)(void *)>(XI_GAME(0x1011CE80u))(a0); }
XI_ORIG __int16 sub_1011D050(void * a0) { return reinterpret_cast<__int16 (__cdecl *)(void *)>(XI_GAME(0x1011D050u))(a0); }
XI_ORIG char Font_AddGlyphAdvance(unsigned __int16 a0, void * a1) { return reinterpret_cast<char (__cdecl *)(unsigned __int16, void *)>(XI_GAME(0x1011D840u))(a0, a1); }
XI_ORIG int Sjis_GetCharByteLen(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x1011DA80u))(a0); }
XI_ORIG int Cell_DrawWithDefaultBlend(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1011F2D0u))(a0); }
XI_ORIG int Cell_DrawQuadBlended(void * a0, int a1, int a2, int a3, int a4, int a5) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int, int)>(XI_GAME(0x1011F300u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int sub_1011FB60(void * a0, int a1, int a2, int a3, int a4, int a5) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int, int)>(XI_GAME(0x1011FB60u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int Shape_DrawScaled(int a0, int a1, int a2, int a3, int a4, int a5, char a6, int a7, int a8) { return reinterpret_cast<int (__thiscall *)(int, int, int, int, int, int, char, int, int)>(XI_GAME(0x1011FC80u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG void Shape_DrawTransformed(void * a0, int a1, int a2, float a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16, int a17, int a18, unsigned int a19, int a20) { reinterpret_cast<void (__thiscall *)(void *, int, int, float, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, unsigned int, int)>(XI_GAME(0x1011FF70u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20); }
XI_ORIG int Color_ModulateArgb128(unsigned int a0, unsigned int a1) { return reinterpret_cast<int (__cdecl *)(unsigned int, unsigned int)>(XI_GAME(0x101200B0u))(a0, a1); }
XI_ORIG _DWORD * ShapeMng_Init(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x101201C0u))(a0); }
XI_ORIG _DWORD * sub_10120200(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10120200u))(a0); }
XI_ORIG char * ShapeMng_FindShape(void * a0, void * a1) { return reinterpret_cast<char * (__thiscall *)(void *, void *)>(XI_GAME(0x10120450u))(a0, a1); }
XI_ORIG bool ShapeMng_GetFontGlyph(void * a0, unsigned __int8 a1, int a2, void * a3, void * a4, void * a5) { return reinterpret_cast<bool (__thiscall *)(void *, unsigned __int8, int, void *, void *, void *)>(XI_GAME(0x10120510u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG char ShapeMng_BindFontTexture(void * a0, unsigned int a1, void * a2, void * a3) { return reinterpret_cast<char (__thiscall *)(void *, unsigned int, void *, void *)>(XI_GAME(0x10120590u))(a0, a1, a2, a3); }
XI_ORIG char * ShapeMng_LoadFontTextures(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10120660u))(a0); }
XI_ORIG bool ShapeMng_GetDmgNumGlyph(void * a0, unsigned __int8 a1, int a2, void * a3, void * a4, void * a5) { return reinterpret_cast<bool (__thiscall *)(void *, unsigned __int8, int, void *, void *, void *)>(XI_GAME(0x10120730u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG char ShapeMng_BindDmgNumTexture(void * a0, unsigned int a1, void * a2, void * a3) { return reinterpret_cast<char (__thiscall *)(void *, unsigned int, void *, void *)>(XI_GAME(0x101207B0u))(a0, a1, a2, a3); }
XI_ORIG char * ShapeMng_LoadDmgNumTextures(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10120880u))(a0); }
XI_ORIG char sub_10123A70(int a0, int a1, int a2, int a3) { return reinterpret_cast<char (__cdecl *)(int, int, int, int)>(XI_GAME(0x10123A70u))(a0, a1, a2, a3); }
XI_ORIG void sub_10123E90() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10123E90u))(); }
XI_ORIG __int16 Cursor_GetHwCursorSlot(void * a0) { return reinterpret_cast<__int16 (__thiscall *)(void *)>(XI_GAME(0x101245C0u))(a0); }
XI_ORIG void Cursor_SetPosition(int a0, void * a1, float a2) { reinterpret_cast<void (__thiscall *)(int, void *, float)>(XI_GAME(0x10124700u))(a0, a1, a2); }
XI_ORIG void sub_101248A0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101248A0u))(a0); }
XI_ORIG char sub_10125040(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x10125040u))(a0); }
XI_ORIG int sub_10125170(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10125170u))(a0); }
XI_ORIG bool sub_101252C0(void * a0) { return reinterpret_cast<bool (__thiscall *)(void *)>(XI_GAME(0x101252C0u))(a0); }
XI_ORIG unsigned __int8 Cursor_SetVisible(void * a0, unsigned __int8 a1) { return reinterpret_cast<unsigned __int8 (__thiscall *)(void *, unsigned __int8)>(XI_GAME(0x101252F0u))(a0, a1); }
XI_ORIG double sub_10125FE0(void * a0) { return reinterpret_cast<double (__thiscall *)(void *)>(XI_GAME(0x10125FE0u))(a0); }
XI_ORIG double sub_101260E0(void * a0) { return reinterpret_cast<double (__thiscall *)(void *)>(XI_GAME(0x101260E0u))(a0); }
XI_ORIG double sub_101260F0(void * a0) { return reinterpret_cast<double (__thiscall *)(void *)>(XI_GAME(0x101260F0u))(a0); }
XI_ORIG double sub_10126100(void * a0) { return reinterpret_cast<double (__thiscall *)(void *)>(XI_GAME(0x10126100u))(a0); }
XI_ORIG bool sub_10126270() { return reinterpret_cast<bool (__cdecl *)()>(XI_GAME(0x10126270u))(); }
XI_ORIG int unknown_libname_89() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10126290u))(); }
XI_ORIG int Cursor_StoreScreenCenter(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101262A0u))(a0); }
XI_ORIG int sub_10126510(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10126510u))(a0); }
XI_ORIG _DWORD * sub_10127190(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10127190u))(a0); }
XI_ORIG char ChatLog_AddMessage(int a0, void * a1, void * a2, int a3, char a4, void * a5) { return reinterpret_cast<char (__thiscall *)(int, void *, void *, int, char, void *)>(XI_GAME(0x10127C00u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG _DWORD * sub_10127EE0(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10127EE0u))(a0); }
XI_ORIG char sub_1013A320(void * a0, void * a1, int a2) { return reinterpret_cast<char (__thiscall *)(void *, void *, int)>(XI_GAME(0x1013A320u))(a0, a1, a2); }
XI_ORIG char sub_10150CC0(void * a0, void * a1, int a2) { return reinterpret_cast<char (__thiscall *)(void *, void *, int)>(XI_GAME(0x10150CC0u))(a0, a1, a2); }
XI_ORIG int sub_10152F80() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10152F80u))(); }
XI_ORIG char sub_10153C10(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10153C10u))(a0); }
XI_ORIG _DWORD * sub_10153FF0() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x10153FF0u))(); }
XI_ORIG void sub_10154860(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10154860u))(a0); }
XI_ORIG char sub_10157660(int a0, int a1) { return reinterpret_cast<char (__thiscall *)(int, int)>(XI_GAME(0x10157660u))(a0, a1); }
XI_ORIG int sub_10157720(int a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(int, int, int)>(XI_GAME(0x10157720u))(a0, a1, a2); }
XI_ORIG int sub_101579C0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101579C0u))(a0); }
XI_ORIG int sub_10157CF0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10157CF0u))(a0); }
XI_ORIG char Actor_SetSlotTableEntry(unsigned int a0, int a1) { return reinterpret_cast<char (__cdecl *)(unsigned int, int)>(XI_GAME(0x10159400u))(a0, a1); }
XI_ORIG int sub_101598A0(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x101598A0u))(a0); }
XI_ORIG int MenuMng_Init(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x1015D900u))(a0); }
XI_ORIG _DWORD * sub_1015D9E0(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1015D9E0u))(a0); }
XI_ORIG bool sub_1015DCE0(void * a0) { return reinterpret_cast<bool (__thiscall *)(void *)>(XI_GAME(0x1015DCE0u))(a0); }
XI_ORIG int sub_1015DD00(int a0, int a1) { return reinterpret_cast<int (__thiscall *)(int, int)>(XI_GAME(0x1015DD00u))(a0, a1); }
XI_ORIG void nullsub_49(int a0) { reinterpret_cast<void (__stdcall *)(int)>(XI_GAME(0x1015DFC0u))(a0); }
XI_ORIG _DWORD * MenuMng_OpenByName(void * a0, void * a1, char a2, char a3) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, char, char)>(XI_GAME(0x1015E350u))(a0, a1, a2, a3); }
XI_ORIG bool sub_1015EDB0(void * a0) { return reinterpret_cast<bool (__thiscall *)(void *)>(XI_GAME(0x1015EDB0u))(a0); }
XI_ORIG int sub_1015EE20(void * a0) { return reinterpret_cast<int (__stdcall *)(void *)>(XI_GAME(0x1015EE20u))(a0); }
XI_ORIG int sub_1015F6A0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1015F6A0u))(a0); }
XI_ORIG int sub_1015FA00(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1015FA00u))(a0); }
XI_ORIG int MenuMng_GetUiScale(void * a0, void * a1, void * a2) { return reinterpret_cast<int (__thiscall *)(void *, void *, void *)>(XI_GAME(0x10160250u))(a0, a1, a2); }
XI_ORIG double MenuMng_GetOffsetX(void * a0, char a1) { return reinterpret_cast<double (__thiscall *)(void *, char)>(XI_GAME(0x10160290u))(a0, a1); }
XI_ORIG double MenuMng_GetOffsetY(void * a0, char a1) { return reinterpret_cast<double (__thiscall *)(void *, char)>(XI_GAME(0x101602C0u))(a0, a1); }
XI_ORIG int ** sub_10160440(int a0, int a1, void * a2, int a3) { return reinterpret_cast<int ** (__fastcall *)(int, int, void *, int)>(XI_GAME(0x10160440u))(a0, a1, a2, a3); }
XI_ORIG int Gfx_SetDefault2DStates() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101604B0u))(); }
XI_ORIG int Gfx_DisableFogAndAlphaBlend() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10160650u))(); }
XI_ORIG int sub_101606F0(_DWORD a0) { return reinterpret_cast<int (__thiscall *)(_DWORD)>(XI_GAME(0x101606F0u))(a0); }
XI_ORIG bool sub_10160700(void * a0) { return reinterpret_cast<bool (__thiscall *)(void *)>(XI_GAME(0x10160700u))(a0); }
XI_ORIG int sub_10160C90(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10160C90u))(a0); }
XI_ORIG int Obj_10621944_DrawBgRect(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10163390u))(a0); }
XI_ORIG _WORD * GameSystems_InitializeAll() { return reinterpret_cast<_WORD * (__cdecl *)()>(XI_GAME(0x10163640u))(); }
XI_ORIG __int16 Game_DestroyGlobalObjects(int a0, int a1) { return reinterpret_cast<__int16 (__fastcall *)(int, int)>(XI_GAME(0x10164B80u))(a0, a1); }
XI_ORIG char sub_10165070() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x10165070u))(); }
XI_ORIG signed int sub_101671D0() { return reinterpret_cast<signed int (__cdecl *)()>(XI_GAME(0x101671D0u))(); }
XI_ORIG void Vt10336614_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101674B0u))(a0); }
XI_ORIG int Vt10336634_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101678D0u))(a0); }
XI_ORIG _DWORD * GetPtr8IfField60Set(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10167940u))(a0); }
XI_ORIG int Dbg_DrawWireMesh(int a0, void * a1, int a2, float a3) { return reinterpret_cast<int (__stdcall *)(int, void *, int, float)>(XI_GAME(0x10168D20u))(a0, a1, a2, a3); }
XI_ORIG float * Mat44_Multiply(void * a0, int a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, int, void *)>(XI_GAME(0x1016C060u))(a0, a1, a2); }
XI_ORIG int Vt10336668_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1016FC80u))(a0); }
XI_ORIG float * sub_10170B30(void * a0, void * a1, void * a2, void * a3) { return reinterpret_cast<float * (__stdcall *)(void *, void *, void *, void *)>(XI_GAME(0x10170B30u))(a0, a1, a2, a3); }
XI_ORIG _DWORD * sub_10170C80(int a0) { return reinterpret_cast<_DWORD * (__thiscall *)(int)>(XI_GAME(0x10170C80u))(a0); }
XI_ORIG int sub_10170E80(int a0, int a1, int a2, int a3) { return reinterpret_cast<int (__stdcall *)(int, int, int, int)>(XI_GAME(0x10170E80u))(a0, a1, a2, a3); }
XI_ORIG int Vt1033668C_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10171920u))(a0); }
XI_ORIG _DWORD * Mat4_SetIdentity_2(void * a0) { return reinterpret_cast<_DWORD * (__cdecl *)(void *)>(XI_GAME(0x10175740u))(a0); }
XI_ORIG float * Mat4_MulTranslation(void * a0, int a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, int, void *)>(XI_GAME(0x10175780u))(a0, a1, a2); }
XI_ORIG unsigned int * CMoResource_VT_10336698_Destructor(void * a0) { return reinterpret_cast<unsigned int * (__thiscall *)(void *)>(XI_GAME(0x10176EC0u))(a0); }
XI_ORIG unsigned int * CMoResource_VT_103366C4_Destructor(void * a0) { return reinterpret_cast<unsigned int * (__thiscall *)(void *)>(XI_GAME(0x10176FA0u))(a0); }
XI_ORIG unsigned int * CMoResource_VT_103366F0_Destructor(void * a0) { return reinterpret_cast<unsigned int * (__thiscall *)(void *)>(XI_GAME(0x10177020u))(a0); }
XI_ORIG int Gfx_ShaderSrc_GetText(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10177340u))(a0); }
XI_ORIG unsigned int Gfx_ShaderSrc_GetTextLength(void * a0) { return reinterpret_cast<unsigned int (__thiscall *)(void *)>(XI_GAME(0x10177350u))(a0); }
XI_ORIG int * sub_101773F0() { return reinterpret_cast<int * (__cdecl *)()>(XI_GAME(0x101773F0u))(); }
XI_ORIG int Gfx_SetZBias(unsigned int a0) { return reinterpret_cast<int (__cdecl *)(unsigned int)>(XI_GAME(0x10177400u))(a0); }
XI_ORIG int ZoneMap_SetOverlayProjection(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10177420u))(a0); }
XI_ORIG int sub_10177D00(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10177D00u))(a0); }
XI_ORIG int sub_10177FA0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10177FA0u))(a0); }
XI_ORIG _DWORD * ZoneMap_FindLight(int a0) { return reinterpret_cast<_DWORD * (__stdcall *)(int)>(XI_GAME(0x10178760u))(a0); }
XI_ORIG int ZoneMap_ReleaseStateBlock(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10178B00u))(a0); }
XI_ORIG int CTsZoneMap_UpdateViewerPosition(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10179390u))(a0); }
XI_ORIG int ZoneMap_CopyField342C8ToField39450(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10179410u))(a0); }
XI_ORIG _DWORD * sub_1017A3F0(void * a0, int a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int)>(XI_GAME(0x1017A3F0u))(a0, a1); }
XI_ORIG int ZoneMap_DrawMeshGroupsWithFog(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x1017ADE0u))(a0, a1); }
XI_ORIG int ZoneMap_TeardownStep(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x1017B1E0u))(a0); }
XI_ORIG int sub_1017C8B0(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(XI_GAME(0x1017C8B0u))(a0, a1, a2, a3); }
XI_ORIG int sub_1017C9C0(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(XI_GAME(0x1017C9C0u))(a0, a1, a2, a3); }
XI_ORIG int sub_1017DE20(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(XI_GAME(0x1017DE20u))(a0, a1, a2, a3); }
XI_ORIG char ZoneMap_FindGroundHeight(void * a0, int a1, float a2, int a3, float a4, void * a5) { return reinterpret_cast<char (__thiscall *)(void *, int, float, int, float, void *)>(XI_GAME(0x101813F0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG BOOL sub_10181BB0(void * a0, void * a1) { return reinterpret_cast<BOOL (__stdcall *)(void *, void *)>(XI_GAME(0x10181BB0u))(a0, a1); }
XI_ORIG BOOL sub_10182070(void * a0, void * a1) { return reinterpret_cast<BOOL (__stdcall *)(void *, void *)>(XI_GAME(0x10182070u))(a0, a1); }
XI_ORIG float * sub_10182490(void * a0, float a1, float a2, float a3, float a4, float a5) { return reinterpret_cast<float * (__stdcall *)(void *, float, float, float, float, float)>(XI_GAME(0x10182490u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG float * CTsZoneMap_DrawProjectedTexture(int a0, void * a1, int a2, void * a3, float a4) { return reinterpret_cast<float * (__thiscall *)(int, void *, int, void *, float)>(XI_GAME(0x10182700u))(a0, a1, a2, a3, a4); }
XI_ORIG float * sub_10183490(void * a0, void * a1, int a2) { return reinterpret_cast<float * (__thiscall *)(void *, void *, int)>(XI_GAME(0x10183490u))(a0, a1, a2); }
XI_ORIG char sub_10183760(void * a0, void * a1, int a2, int a3, int a4) { return reinterpret_cast<char (__thiscall *)(void *, void *, int, int, int)>(XI_GAME(0x10183760u))(a0, a1, a2, a3, a4); }
XI_ORIG _DWORD * ZoneMap_DrawMeshById(int a0, int a1, int a2) { return reinterpret_cast<_DWORD * (__thiscall *)(int, int, int)>(XI_GAME(0x10183DA0u))(a0, a1, a2); }
XI_ORIG char * sub_10186E90(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10186E90u))(a0); }
XI_ORIG char * sub_10186EB0(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10186EB0u))(a0); }
XI_ORIG char * Env_GetColorPtr84(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10186F20u))(a0); }
XI_ORIG char * Env_GetAmbientColorPtr(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10186FA0u))(a0); }
XI_ORIG double sub_10186FE0(void * a0) { return reinterpret_cast<double (__thiscall *)(void *)>(XI_GAME(0x10186FE0u))(a0); }
XI_ORIG double sub_10186FF0(void * a0) { return reinterpret_cast<double (__thiscall *)(void *)>(XI_GAME(0x10186FF0u))(a0); }
XI_ORIG int sub_10187000(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10187000u))(a0); }
XI_ORIG int sub_10187010(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10187010u))(a0); }
XI_ORIG char sub_101874F0(void * a0, int a1, int a2, int a3) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int)>(XI_GAME(0x101874F0u))(a0, a1, a2, a3); }
XI_ORIG int sub_101880B0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101880B0u))(a0); }
XI_ORIG _DWORD * XiZone_GetDirLights(int a0, void * a1, void * a2) { return reinterpret_cast<_DWORD * (__thiscall *)(int, void *, void *)>(XI_GAME(0x10188B70u))(a0, a1, a2); }
XI_ORIG int sub_10188CA0(int a0, void * a1, int a2) { return reinterpret_cast<int (__thiscall *)(int, void *, int)>(XI_GAME(0x10188CA0u))(a0, a1, a2); }
XI_ORIG _DWORD * Env_ComputeAmbient(void * a0, void * a1, int a2) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, int)>(XI_GAME(0x10188E00u))(a0, a1, a2); }
XI_ORIG float * Env_ComputeAmbientFloat4(void * a0, void * a1) { return reinterpret_cast<float * (__thiscall *)(void *, void *)>(XI_GAME(0x10188FA0u))(a0, a1); }
XI_ORIG void sub_10189120(int a0, void * a1, void * a2, void * a3, int a4) { reinterpret_cast<void (__thiscall *)(int, void *, void *, void *, int)>(XI_GAME(0x10189120u))(a0, a1, a2, a3, a4); }
XI_ORIG double sub_101892B0(void * a0, int a1) { return reinterpret_cast<double (__thiscall *)(void *, int)>(XI_GAME(0x101892B0u))(a0, a1); }
XI_ORIG std::codecvt_base * sub_1018B010(void * a0) { return reinterpret_cast<std::codecvt_base * (__cdecl *)(void *)>(XI_GAME(0x1018B010u))(a0); }
XI_ORIG _DWORD * sub_1018B130() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x1018B130u))(); }
XI_ORIG void sub_1018B3A0(int a0) { reinterpret_cast<void (__cdecl *)(int)>(XI_GAME(0x1018B3A0u))(a0); }
XI_ORIG int sub_1018B860(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1018B860u))(a0); }
XI_ORIG double sub_1018B8F0(int a0, int a1) { return reinterpret_cast<double (__thiscall *)(int, int)>(XI_GAME(0x1018B8F0u))(a0, a1); }
XI_ORIG _DWORD * sub_1018B960(void * a0, void * a1, void * a2, int a3) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, void *, int)>(XI_GAME(0x1018B960u))(a0, a1, a2, a3); }
XI_ORIG int XiZone_GetDirLight(void * a0, void * a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, void *, int, int)>(XI_GAME(0x1018B9A0u))(a0, a1, a2, a3); }
XI_ORIG int XiZone_GetAmbient(void * a0, void * a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, void *, int, int)>(XI_GAME(0x1018B9E0u))(a0, a1, a2, a3); }
XI_ORIG void XiZone_GetFog(void * a0, void * a1, void * a2, void * a3, int a4, int a5) { reinterpret_cast<void (__thiscall *)(void *, void *, void *, void *, int, int)>(XI_GAME(0x1018BA20u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void * sub_1018BAE0(void * a0, unsigned int a1) { return reinterpret_cast<void * (__thiscall *)(void *, unsigned int)>(XI_GAME(0x1018BAE0u))(a0, a1); }
XI_ORIG int sub_1018BB10(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1018BB10u))(a0, a1); }
XI_ORIG int sub_1018BB20(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1018BB20u))(a0, a1); }
XI_ORIG int unknown_libname_158(_DWORD a0) { return reinterpret_cast<int (__thiscall *)(_DWORD)>(XI_GAME(0x1018BBB0u))(a0); }
XI_ORIG int * sub_1018BBC0(void * a0, int a1) { return reinterpret_cast<int * (__thiscall *)(void *, int)>(XI_GAME(0x1018BBC0u))(a0, a1); }
XI_ORIG void sub_1018BBE0(void * a0, unsigned int a1) { reinterpret_cast<void (__thiscall *)(void *, unsigned int)>(XI_GAME(0x1018BBE0u))(a0, a1); }
XI_ORIG unsigned int sub_1018BC40(void * a0, int a1) { return reinterpret_cast<unsigned int (__thiscall *)(void *, int)>(XI_GAME(0x1018BC40u))(a0, a1); }
XI_ORIG unsigned int sub_1018C210(int a0) { return reinterpret_cast<unsigned int (__cdecl *)(int)>(XI_GAME(0x1018C210u))(a0); }
XI_ORIG unsigned int sub_1018C2B0() { return reinterpret_cast<unsigned int (__cdecl *)()>(XI_GAME(0x1018C2B0u))(); }
XI_ORIG char CXiSchStatus_Destructor(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x1018C620u))(a0); }
XI_ORIG char sub_1018C810(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x1018C810u))(a0); }
XI_ORIG char sub_1018C870(int a0, void * a1) { return reinterpret_cast<char (__thiscall *)(int, void *)>(XI_GAME(0x1018C870u))(a0, a1); }
XI_ORIG int sub_1018C980(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1018C980u))(a0, a1); }
XI_ORIG int Sch_GetStatusEntry(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x1018CA20u))(a0); }
XI_ORIG char sub_1018CE10(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x1018CE10u))(a0); }
XI_ORIG _DWORD * sub_1018CEF0(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1018CEF0u))(a0); }
XI_ORIG int Settings_GetValue(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10193850u))(a0); }
XI_ORIG bool sub_10193890(char a0) { return reinterpret_cast<bool (__cdecl *)(char)>(XI_GAME(0x10193890u))(a0); }
XI_ORIG int sub_1019C3E0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1019C3E0u))(); }
XI_ORIG int sub_1019C5B0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1019C5B0u))(); }
XI_ORIG int sub_1019EBB0(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x1019EBB0u))(a0); }
XI_ORIG char sub_101A2E80(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x101A2E80u))(a0); }
XI_ORIG _BYTE * sub_101A8B50() { return reinterpret_cast<_BYTE * (__cdecl *)()>(XI_GAME(0x101A8B50u))(); }
XI_ORIG _DWORD * sub_101A8D40() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x101A8D40u))(); }
XI_ORIG int sub_101A8D60() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101A8D60u))(); }
XI_ORIG Iostream_init * sub_101AB080() { return reinterpret_cast<Iostream_init * (__cdecl *)()>(XI_GAME(0x101AB080u))(); }
XI_ORIG char * sub_101AB0D0() { return reinterpret_cast<char * (__cdecl *)()>(XI_GAME(0x101AB0D0u))(); }
XI_ORIG int sub_101AB6D0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101AB6D0u))(); }
XI_ORIG int sub_101AB720() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101AB720u))(); }
XI_ORIG char sub_101AB770() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x101AB770u))(); }
XI_ORIG void sub_101AB820(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101AB820u))(a0); }
XI_ORIG int sub_101AE3E0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101AE3E0u))(a0); }
XI_ORIG int sub_101B8E70() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101B8E70u))(); }
XI_ORIG char sub_101BCE30(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x101BCE30u))(a0); }
XI_ORIG void sub_101C46A0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101C46A0u))(a0); }
XI_ORIG int FileMng_ResolveOrExit(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x101C4AF0u))(a0, a1); }
XI_ORIG char sub_101C4CC0(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(XI_GAME(0x101C4CC0u))(a0, a1); }
XI_ORIG bool sub_101C4CD0(void * a0) { return reinterpret_cast<bool (__thiscall *)(void *)>(XI_GAME(0x101C4CD0u))(a0); }
XI_ORIG int sub_101C4CE0(int a0) { return reinterpret_cast<int (__stdcall *)(int)>(XI_GAME(0x101C4CE0u))(a0); }
XI_ORIG int sub_101C4D10(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101C4D10u))(a0); }
XI_ORIG int sub_101C9B50(__int16 a0, __int16 a1, void * a2, int a3, int a4, float a5, float a6) { return reinterpret_cast<int (__cdecl *)(__int16, __int16, void *, int, int, float, float)>(XI_GAME(0x101C9B50u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int sub_101CC4F0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101CC4F0u))(); }
XI_ORIG char sub_101D0B00() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x101D0B00u))(); }
XI_ORIG char * sub_101D0C40(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x101D0C40u))(a0); }
XI_ORIG char sub_101D0C70() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x101D0C70u))(); }
XI_ORIG _WORD * YkInit() { return reinterpret_cast<_WORD * (__cdecl *)()>(XI_GAME(0x101D11C0u))(); }
XI_ORIG void sub_101D5680(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101D5680u))(a0); }
XI_ORIG void sub_101EE7B0(int a0, void * a1) { reinterpret_cast<void (__thiscall *)(int, void *)>(XI_GAME(0x101EE7B0u))(a0, a1); }
XI_ORIG _WORD * sub_1021BB90() { return reinterpret_cast<_WORD * (__cdecl *)()>(XI_GAME(0x1021BB90u))(); }
XI_ORIG int CXiMovie_DrawFrame() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1021D4C0u))(); }
XI_ORIG char Movie_Update(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x1021D980u))(a0); }
XI_ORIG int Movie_GetCurrentTexture(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1021DDF0u))(a0); }
XI_ORIG int sub_10220EF0(int a0, int a1, int a2, char a3) { return reinterpret_cast<int (__thiscall *)(int, int, int, char)>(XI_GAME(0x10220EF0u))(a0, a1, a2, a3); }
XI_ORIG int sub_10220F40(int a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(int, int, int)>(XI_GAME(0x10220F40u))(a0, a1, a2); }
XI_ORIG int sub_10221770(unsigned __int16 a0) { return reinterpret_cast<int (__cdecl *)(unsigned __int16)>(XI_GAME(0x10221770u))(a0); }
XI_ORIG void * sub_102217A0() { return reinterpret_cast<void * (__cdecl *)()>(XI_GAME(0x102217A0u))(); }
XI_ORIG char sub_102347D0(void * a0, int a1, int a2) { return reinterpret_cast<char (__thiscall *)(void *, int, int)>(XI_GAME(0x102347D0u))(a0, a1, a2); }
XI_ORIG char sub_10234A00(void * a0, int a1, __int16 a2) { return reinterpret_cast<char (__thiscall *)(void *, int, __int16)>(XI_GAME(0x10234A00u))(a0, a1, a2); }
XI_ORIG int Ui_KeyPrompt_Init(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10239460u))(a0, a1); }
XI_ORIG int LobbyThread_Start(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10245600u))(a0); }
XI_ORIG int sub_10245B50(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10245B50u))(a0); }
XI_ORIG int Lobby_DestroyThreadPool() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10245B60u))(); }
XI_ORIG int thunk_sub_10245BB0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10245BA0u))(); }
XI_ORIG char sub_1024D130() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x1024D130u))(); }
XI_ORIG int StAvatar_ReleaseResources(void * a0, int a1) { return reinterpret_cast<int (__cdecl *)(void *, int)>(XI_GAME(0x1024D2C0u))(a0, a1); }
XI_ORIG int StAvatar_StopAndFreeBuffers(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x1024D790u))(a0); }
XI_ORIG int sub_102502D0(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x102502D0u))(a0); }
XI_ORIG int sub_10250D80(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x10250D80u))(a0); }
XI_ORIG int StDancer_Update(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10251500u))(a0); }
XI_ORIG int sub_102515F0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x102515F0u))(a0, a1); }
XI_ORIG int sub_10251960(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10251960u))(a0); }
XI_ORIG int sub_102524B0(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x102524B0u))(a0); }
XI_ORIG int sub_10252C80(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x10252C80u))(a0); }
XI_ORIG _DWORD * sub_102532E0(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x102532E0u))(a0); }
XI_ORIG char sub_10253670(__int16 a0, char a1) { return reinterpret_cast<char (__cdecl *)(__int16, char)>(XI_GAME(0x10253670u))(a0, a1); }
XI_ORIG int sub_1025B560(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x1025B560u))(a0); }
XI_ORIG BOOL sub_1025E050() { return reinterpret_cast<BOOL (__cdecl *)()>(XI_GAME(0x1025E050u))(); }
XI_ORIG void * sub_1025E0B0() { return reinterpret_cast<void * (__cdecl *)()>(XI_GAME(0x1025E0B0u))(); }
XI_ORIG void sub_1025E0C0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1025E0C0u))(); }
XI_ORIG int sub_1025E0E0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1025E0E0u))(); }
XI_ORIG char sub_1025E0F0() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x1025E0F0u))(); }
XI_ORIG int sub_1026E400() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1026E400u))(); }
XI_ORIG _DWORD * sub_1026E460(int a0, int a1) { return reinterpret_cast<_DWORD * (__cdecl *)(int, int)>(XI_GAME(0x1026E460u))(a0, a1); }
XI_ORIG int mdl_1026E900() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1026E900u))(); }
XI_ORIG int sub_10270B60() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10270B60u))(); }
XI_ORIG void sub_102717F0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x102717F0u))(); }
XI_ORIG char scnFini() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x10271B40u))(); }
XI_ORIG void sub_10271C60(int a0, int a1) { reinterpret_cast<void (__cdecl *)(int, int)>(XI_GAME(0x10271C60u))(a0, a1); }
XI_ORIG void sub_10271C80(int a0, int a1) { reinterpret_cast<void (__cdecl *)(int, int)>(XI_GAME(0x10271C80u))(a0, a1); }
XI_ORIG _DWORD * scnGetFlags(void * a0) { return reinterpret_cast<_DWORD * (__cdecl *)(void *)>(XI_GAME(0x10271E90u))(a0); }
XI_ORIG int scnCameraGetInfo(int a0, void * a1, void * a2) { return reinterpret_cast<int (__cdecl *)(int, void *, void *)>(XI_GAME(0x10273370u))(a0, a1, a2); }
XI_ORIG _DWORD * sub_102738D0(void * a0) { return reinterpret_cast<_DWORD * (__cdecl *)(void *)>(XI_GAME(0x102738D0u))(a0); }
XI_ORIG void chasm_10273920(int a0) { reinterpret_cast<void (__cdecl *)(int)>(XI_GAME(0x10273920u))(a0); }
XI_ORIG int PackVec4(int a0, float a1, int a2, int a3, int a4) { return reinterpret_cast<int (__cdecl *)(int, float, int, int, int)>(XI_GAME(0x10274D40u))(a0, a1, a2, a3, a4); }
XI_ORIG int sub_10275090() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10275090u))(); }
XI_ORIG int sqMemInit() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10275150u))(); }
XI_ORIG void sqMemPoolRegister(int a0, int a1, int a2, void * a3) { reinterpret_cast<void (__cdecl *)(int, int, int, void *)>(XI_GAME(0x10275B60u))(a0, a1, a2, a3); }
XI_ORIG int sqFree(int a0, void * a1, int a2) { return reinterpret_cast<int (__cdecl *)(int, void *, int)>(XI_GAME(0x102760F0u))(a0, a1, a2); }
XI_ORIG _DWORD * sqgxContext_Create() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x10276260u))(); }
XI_ORIG void M4fSet(void * a0, void * a1) { reinterpret_cast<void (__cdecl *)(void *, void *)>(XI_GAME(0x1027EB10u))(a0, a1); }
XI_ORIG float * M4fTransform(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, void *)>(XI_GAME(0x1027EF20u))(a0, a1, a2); }
XI_ORIG int M4fTranspose(void * a0, void * a1) { return reinterpret_cast<int (__cdecl *)(void *, void *)>(XI_GAME(0x1027F070u))(a0, a1); }
XI_ORIG int sqMatrix4_1027F0E0(void * a0, int a1) { return reinterpret_cast<int (__cdecl *)(void *, int)>(XI_GAME(0x1027F0E0u))(a0, a1); }
XI_ORIG double sqMatrix4_1027FCC0(int a0) { return reinterpret_cast<double (__cdecl *)(int)>(XI_GAME(0x1027FCC0u))(a0); }
XI_ORIG float * sqMatrix4_10280140(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, void *)>(XI_GAME(0x10280140u))(a0, a1, a2); }
XI_ORIG int * sqgxSprite(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8) { return reinterpret_cast<int * (__cdecl *)(int, int, int, int, int, int, int, int, int)>(XI_GAME(0x10280DE0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG int ___setargv_3() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10281500u))(); }
XI_ORIG int sqgxTextureCreateFromImage(int a0, void * a1) { return reinterpret_cast<int (__cdecl *)(int, void *)>(XI_GAME(0x10281510u))(a0, a1); }
XI_ORIG int sqgxTextureAddZ(int a0, int a1, int a2) { return reinterpret_cast<int (__cdecl *)(int, int, int)>(XI_GAME(0x102816E0u))(a0, a1, a2); }
XI_ORIG int sqgxCeilLog2(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10281810u))(a0); }
XI_ORIG int sqgxTextureAdd(int a0, int a1, void * a2, void * a3) { return reinterpret_cast<int (__cdecl *)(int, int, void *, void *)>(XI_GAME(0x10281830u))(a0, a1, a2, a3); }
XI_ORIG int sqgxTextureGetSize(int a0, void * a1, void * a2) { return reinterpret_cast<int (__cdecl *)(int, void *, void *)>(XI_GAME(0x10281B60u))(a0, a1, a2); }
XI_ORIG _DWORD * sqgxTextureSet(int a0) { return reinterpret_cast<_DWORD * (__cdecl *)(int)>(XI_GAME(0x10281C20u))(a0); }
XI_ORIG int sqgxTextureDecal(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10281C50u))(a0); }
XI_ORIG int sqgxTextureClamp(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10281C80u))(a0); }
XI_ORIG int sqgxTextureUpdate() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10281DB0u))(); }
XI_ORIG int sqgxTextureStencilClear(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10282010u))(a0); }
XI_ORIG int sqgxTextureClear(void * a0, int a1) { return reinterpret_cast<int (__cdecl *)(void *, int)>(XI_GAME(0x10282030u))(a0, a1); }
XI_ORIG int * sqgxTextureDraw(int a0, char a1) { return reinterpret_cast<int * (__cdecl *)(int, char)>(XI_GAME(0x10282190u))(a0, a1); }
XI_ORIG int * sqgxZTest(int a0) { return reinterpret_cast<int * (__cdecl *)(int)>(XI_GAME(0x10282350u))(a0); }
XI_ORIG int sqgxAlphaDstTest(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(XI_GAME(0x102823B0u))(a0, a1); }
XI_ORIG int * sqgxZMask(int a0) { return reinterpret_cast<int * (__cdecl *)(int)>(XI_GAME(0x10282470u))(a0); }
XI_ORIG int * sqgxFBMask(int a0, int a1, int a2, int a3) { return reinterpret_cast<int * (__cdecl *)(int, int, int, int)>(XI_GAME(0x102824C0u))(a0, a1, a2, a3); }
XI_ORIG int nullsub_59() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10282630u))(); }
XI_ORIG void sqgxBlendLin(float a0) { reinterpret_cast<void (__cdecl *)(float)>(XI_GAME(0x10282A30u))(a0); }
XI_ORIG int sqgxBlendTrsp() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10282B90u))(); }
XI_ORIG int sqgxBlendMul() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10283110u))(); }
XI_ORIG int sqgxBlendOff() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10283220u))(); }
XI_ORIG int nullsub_31() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10283240u))(); }
XI_ORIG int ___setargv_9() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10283250u))(); }
XI_ORIG int sqgxColor(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x10283330u))(a0); }
XI_ORIG int sqgxAmbient(float a0) { return reinterpret_cast<int (__cdecl *)(float)>(XI_GAME(0x10283380u))(a0); }
XI_ORIG int sqgxColorUpdate() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10283420u))(); }
XI_ORIG int ___setargv_16() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10283870u))(); }
XI_ORIG int sqgxPixelSize(float a0, float a1) { return reinterpret_cast<int (__cdecl *)(float, float)>(XI_GAME(0x102838A0u))(a0, a1); }
XI_ORIG int * sqgxViewport(float a0, float a1, float a2, float a3) { return reinterpret_cast<int * (__cdecl *)(float, float, float, float)>(XI_GAME(0x10283A10u))(a0, a1, a2, a3); }
XI_ORIG int sqgxMatrixCamera(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x10283D60u))(a0); }
XI_ORIG int sqgxMatrixShape(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x10283E00u))(a0); }
XI_ORIG int sqgxPassAdd(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10283F70u))(a0); }
XI_ORIG int sqgxMatrixUpdate() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10284410u))(); }
XI_ORIG int ___setargv_24() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10284890u))(); }
XI_ORIG int sqgxVertexShaderCreate(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x10284950u))(a0); }
XI_ORIG int * sqgxShader(int a0, int a1) { return reinterpret_cast<int * (__cdecl *)(int, int)>(XI_GAME(0x10284B20u))(a0, a1); }
XI_ORIG int nullsub_32() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10284EB0u))(); }
XI_ORIG int j_sqgxVertexShaderSelect() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10284EC0u))(); }
XI_ORIG void * sqDrawableDraw(int a0) { return reinterpret_cast<void * (__cdecl *)(int)>(XI_GAME(0x10285850u))(a0); }
template <class... R> XI_ORIG void sqDebugPrintf(void * a0, R... rest) { reinterpret_cast<void (__cdecl *)(void *, ...)>(XI_GAME(0x10285890u))(a0, rest...); }
XI_ORIG int sub_1028A730(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(XI_GAME(0x1028A730u))(a0, a1); }
XI_ORIG int sub_1028A770(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(XI_GAME(0x1028A770u))(a0, a1); }
XI_ORIG int sub_1028A790(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x1028A790u))(a0); }
XI_ORIG int * sqgxShapeBegin() { return reinterpret_cast<int * (__cdecl *)()>(XI_GAME(0x1028AFE0u))(); }
XI_ORIG int sub_1028BB10(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x1028BB10u))(a0); }
XI_ORIG int sub_1028BB70() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1028BB70u))(); }
XI_ORIG int sub_1028BBD0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1028BBD0u))(); }
XI_ORIG int sqsnScene_1028F5F0(int a0, int a1, int a2) { return reinterpret_cast<int (__cdecl *)(int, int, int)>(XI_GAME(0x1028F5F0u))(a0, a1, a2); }
XI_ORIG int sqsnSceneLayerDraw(void * a0, int a1) { return reinterpret_cast<int (__cdecl *)(void *, int)>(XI_GAME(0x1028FDD0u))(a0, a1); }
XI_ORIG int sqsnSceneLayerDrawShadowed(void * a0, int a1) { return reinterpret_cast<int (__cdecl *)(void *, int)>(XI_GAME(0x1028FFF0u))(a0, a1); }
XI_ORIG int sqsnSceneLayerSnapShotComposite(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(XI_GAME(0x102902C0u))(a0, a1); }
XI_ORIG int nullsub_60(_DWORD a0, _DWORD a1) { return reinterpret_cast<int (__cdecl *)(_DWORD, _DWORD)>(XI_GAME(0x10290430u))(a0, a1); }
XI_ORIG int sqsnSceneLayerSort(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(XI_GAME(0x10290440u))(a0, a1); }
XI_ORIG int sqrdCamera_Apply(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x102911E0u))(a0); }
XI_ORIG int sub_10297320(int a0, int a1, int a2) { return reinterpret_cast<int (__cdecl *)(int, int, int)>(XI_GAME(0x10297320u))(a0, a1, a2); }
XI_ORIG unsigned int sqsnShadowVolumeExtrudeTriList(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, void * a8, float a9, int a10) { return reinterpret_cast<unsigned int (__cdecl *)(int, int, int, int, int, int, int, int, void *, float, int)>(XI_GAME(0x102979E0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }
XI_ORIG int sqsnShadowVolumeExtrudeTriStrip(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, void * a8, float a9, int a10) { return reinterpret_cast<int (__cdecl *)(int, int, int, int, int, int, int, int, void *, float, int)>(XI_GAME(0x10297B10u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }
XI_ORIG int sqsnShadowVolumeSetPass(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10297CC0u))(a0); }
XI_ORIG int sqsnShadowComputeFacing(int a0, void * a1, int a2, int a3, void * a4, int a5, void * a6) { return reinterpret_cast<int (__cdecl *)(int, void *, int, int, void *, int, void *)>(XI_GAME(0x10297F90u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int sqsnShadowVolumeBeginVB() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x102981E0u))(); }
XI_ORIG int sqgxShadowPlaneMatrix(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10298350u))(a0); }
XI_ORIG int sub_1029BD50(int a0, int a1, void * a2, void * a3) { return reinterpret_cast<int (__cdecl *)(int, int, void *, void *)>(XI_GAME(0x1029BD50u))(a0, a1, a2, a3); }
XI_ORIG int sub_1029BF50(int a0, void * a1, void * a2) { return reinterpret_cast<int (__cdecl *)(int, void *, void *)>(XI_GAME(0x1029BF50u))(a0, a1, a2); }
XI_ORIG float * sub_1029C480(int a0, void * a1, void * a2, int a3, int a4, void * a5, void * a6) { return reinterpret_cast<float * (__cdecl *)(int, void *, void *, int, int, void *, void *)>(XI_GAME(0x1029C480u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG void sub_1029C610(int a0, void * a1, int a2, int a3) { reinterpret_cast<void (__cdecl *)(int, void *, int, int)>(XI_GAME(0x1029C610u))(a0, a1, a2, a3); }
XI_ORIG int sub_1029C950(int a0, int a1, void * a2, void * a3, void * a4, void * a5) { return reinterpret_cast<int (__cdecl *)(int, int, void *, void *, void *, void *)>(XI_GAME(0x1029C950u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int Direct3DCreate8(_DWORD a0) { return reinterpret_cast<int (__stdcall *)(_DWORD)>(XI_GAME(0x102D2600u))(a0); }
XI_ORIG int fn_102D2868(void * a0, void * a1, void * a2, int a3, int a4, int a5, int a6, void * a7, int a8, int a9) { return reinterpret_cast<int (__stdcall *)(void *, void *, void *, int, int, int, int, void *, int, int)>(XI_GAME(0x102D2868u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9); }
XI_ORIG int fn_102D2F40(void * a0, void * a1, void * a2, void * a3, int a4, void * a5, int a6) { return reinterpret_cast<int (__stdcall *)(void *, void *, void *, void *, int, void *, int)>(XI_GAME(0x102D2F40u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int fn_102D2FB9(void * a0, unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, _D3DFORMAT a5, __int32 a6, void * a7) { return reinterpret_cast<int (__stdcall *)(void *, unsigned int, unsigned int, unsigned int, unsigned int, _D3DFORMAT, __int32, void *)>(XI_GAME(0x102D2FB9u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG int fn_102D301F(void * a0, unsigned int a1, unsigned int a2, unsigned int a3, _D3DFORMAT a4, __int32 a5, void * a6) { return reinterpret_cast<int (__stdcall *)(void *, unsigned int, unsigned int, unsigned int, _D3DFORMAT, __int32, void *)>(XI_GAME(0x102D301Fu))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int fn_102D4515(void * a0, void * a1, void * a2, void * a3, void * a4, void * a5, int a6, int a7) { return reinterpret_cast<int (__stdcall *)(void *, void *, void *, void *, void *, void *, int, int)>(XI_GAME(0x102D4515u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG float * D3DXMatrixMultiply_Dispatch(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__stdcall *)(void *, void *, void *)>(XI_GAME(0x102D6F03u))(a0, a1, a2); }
XI_ORIG float * D3DXMatrixTranspose_Dispatch(void * a0, void * a1) { return reinterpret_cast<float * (__stdcall *)(void *, void *)>(XI_GAME(0x102D714Cu))(a0, a1); }
XI_ORIG float * fn_102D7FF9(void * a0, void * a1, void * a2, void * a3) { return reinterpret_cast<float * (__stdcall *)(void *, void *, void *, void *)>(XI_GAME(0x102D7FF9u))(a0, a1, a2, a3); }
XI_ORIG int D3DXMatrixPerspectiveFovLH_static(int a0, float a1, float a2, float a3, float a4) { return reinterpret_cast<int (__stdcall *)(int, float, float, float, float)>(XI_GAME(0x102D83C6u))(a0, a1, a2, a3, a4); }
XI_ORIG int fn_102DA79B(void * a0, unsigned int a1, void * a2, void * a3, void * a4) { return reinterpret_cast<int (__stdcall *)(void *, unsigned int, void *, void *, void *)>(XI_GAME(0x102DA79Bu))(a0, a1, a2, a3, a4); }
XI_ORIG int fn_102DAA0D(void * a0, char a1, unsigned int a2, void * a3, void * a4, void * a5) { return reinterpret_cast<int (__stdcall *)(void *, char, unsigned int, void *, void *, void *)>(XI_GAME(0x102DAA0Du))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int fn_102DAA71(int a0, void * a1, unsigned int a2) { return reinterpret_cast<int (__stdcall *)(int, void *, unsigned int)>(XI_GAME(0x102DAA71u))(a0, a1, a2); }
XI_ORIG int Thunk_FnTable694() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1031077Eu))(); }
XI_ORIG void operator_delete(void * a0) { reinterpret_cast<void (__cdecl *)(void *)>(XI_GAME(0x10311BB0u))(a0); }
XI_ORIG void * Allocator_Generic(unsigned int a0) { return reinterpret_cast<void * (__cdecl *)(unsigned int)>(XI_GAME(0x10311BBBu))(a0); }
template <class... R> XI_ORIG int _sprintf(void * a0, void * a1, R... rest) { return reinterpret_cast<int (__cdecl *)(void *, void *, ...)>(XI_GAME(0x10311FF1u))(a0, a1, rest...); }
XI_ORIG char * _strncpy(void * a0, void * a1, size_t a2) { return reinterpret_cast<char * (__cdecl *)(void *, void *, size_t)>(XI_GAME(0x10312050u))(a0, a1, a2); }
XI_ORIG char * _strrchr(void * a0, int a1) { return reinterpret_cast<char * (__cdecl *)(void *, int)>(XI_GAME(0x103127C0u))(a0, a1); }
XI_ORIG void * _malloc_0(size_t a0) { return reinterpret_cast<void * (__cdecl *)(size_t)>(XI_GAME(0x10312F96u))(a0); }
XI_ORIG int _atexit(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x10313486u))(a0); }
template <class... R> XI_ORIG int __snprintf(void * a0, const size_t a1, void * a2, R... rest) { return reinterpret_cast<int (__cdecl *)(void *, const size_t, void *, ...)>(XI_GAME(0x103134C7u))(a0, a1, a2, rest...); }
XI_ORIG char * __strlwr(void * a0) { return reinterpret_cast<char * (__cdecl *)(void *)>(XI_GAME(0x10315AA8u))(a0); }
}  // namespace orig
// a game function used as a value (callback, table entry): its address in the game image
namespace orig_addr {
constexpr uint32_t CApp_Constructor = 0x10001240u;
constexpr uint32_t App_ForceForegroundWindow = 0x100014A0u;
constexpr uint32_t App_PreTranslateInputMessage = 0x100016C0u;
constexpr uint32_t App_RenderFrameMaskedFPU = 0x10001B50u;
constexpr uint32_t App_CreateInputSystem = 0x10001BB0u;
constexpr uint32_t CApp_CopyTextToClipboard = 0x10001CC0u;
constexpr uint32_t App_SetCommandLine = 0x10001E00u;
constexpr uint32_t App_FreeCommandLine = 0x10001E10u;
constexpr uint32_t sub_10001E70 = 0x10001E70u;
constexpr uint32_t Gfx_ProbeDeviceCaps = 0x10002130u;
constexpr uint32_t sub_10002440 = 0x10002440u;
constexpr uint32_t App_DetectDat11 = 0x10002460u;
constexpr uint32_t App_CreateConfig = 0x10002B20u;
constexpr uint32_t App_DestroyConfig = 0x10002BC0u;
constexpr uint32_t Gfx_SetGammaRGB = 0x10002C00u;
constexpr uint32_t sub_10002D80 = 0x10002D80u;
constexpr uint32_t sub_10002E60 = 0x10002E60u;
constexpr uint32_t Gfx_CreateD3DHolder = 0x10002F20u;
constexpr uint32_t Gfx_DestroyD3DHolder = 0x10003040u;
constexpr uint32_t D3DHolder_Constructor = 0x10003070u;
constexpr uint32_t D3DHolder_Initialize = 0x10003110u;
constexpr uint32_t Gfx_TryCreateDeviceForMode = 0x100033C0u;
constexpr uint32_t D3DHolder_CheckDepthFormat = 0x10003460u;
constexpr uint32_t Gfx_CreateDevice = 0x100034B0u;
constexpr uint32_t nullsub_71 = 0x100035A0u;
constexpr uint32_t sub_100035F0 = 0x100035F0u;
constexpr uint32_t sub_10003650 = 0x10003650u;
constexpr uint32_t Debug_DrawWorldText = 0x10003D70u;
constexpr uint32_t sub_10005A40 = 0x10005A40u;
constexpr uint32_t sub_10008830 = 0x10008830u;
constexpr uint32_t sub_100088F0 = 0x100088F0u;
constexpr uint32_t CDx_Constructor = 0x10008F90u;
constexpr uint32_t CDx_Destructor = 0x10009120u;
constexpr uint32_t CDx_Initialize = 0x100091B0u;
constexpr uint32_t CDx_SetDefaultRenderTargets = 0x10009620u;
constexpr uint32_t CDx_SetClearParams = 0x10009640u;
constexpr uint32_t CDx_SetClearColor = 0x10009670u;
constexpr uint32_t CDx_EndClearBegin = 0x100096B0u;
constexpr uint32_t CDx_TestCooperativeLevel = 0x10009710u;
constexpr uint32_t CDx_Present = 0x10009720u;
constexpr uint32_t CDx_BeginScene = 0x10009730u;
constexpr uint32_t CDx_EndScene = 0x10009740u;
constexpr uint32_t sub_10009750 = 0x10009750u;
constexpr uint32_t SetCursorPos = 0x10009770u;
constexpr uint32_t CDx_ShowCursor = 0x10009780u;
constexpr uint32_t CDx_SetCursorShape = 0x100097F0u;
constexpr uint32_t sub_10009870 = 0x10009870u;
constexpr uint32_t sub_100098E0 = 0x100098E0u;
constexpr uint32_t CDx_SetDefaultRenderState = 0x10009920u;
constexpr uint32_t sub_10009D20 = 0x10009D20u;
constexpr uint32_t CDx_SetViewport = 0x10009DC0u;
constexpr uint32_t CDx_GetRenderTargetTop = 0x10009E00u;
constexpr uint32_t CDx_PushRenderTarget = 0x10009E20u;
constexpr uint32_t sub_10009EA0 = 0x10009EA0u;
constexpr uint32_t sub_10009EE0 = 0x10009EE0u;
constexpr uint32_t CDx_PushRenderTargetTexture = 0x10009F20u;
constexpr uint32_t CDx_SetTexture = 0x1000A1D0u;
constexpr uint32_t sub_1000A200 = 0x1000A200u;
constexpr uint32_t sub_1000A230 = 0x1000A230u;
constexpr uint32_t CDx_PopRenderTargetDefault = 0x1000A290u;
constexpr uint32_t CDx_PopRenderTarget = 0x1000A2B0u;
constexpr uint32_t sub_1000A390 = 0x1000A390u;
constexpr uint32_t CDx_DrawScreenQuadAtDepth = 0x1000A3B0u;
constexpr uint32_t CDx_DrawTextureToViewport = 0x1000A4F0u;
constexpr uint32_t CDx_DrawTexturedScreenQuad = 0x1000A5A0u;
constexpr uint32_t CDx_CreateTexture = 0x1000A6E0u;
constexpr uint32_t CDx_PushRenderTargetRet1 = 0x1000A800u;
constexpr uint32_t CDx_DrawFadeQuad = 0x1000A820u;
constexpr uint32_t Post_DepthBlurComposite = 0x1000AAB0u;
constexpr uint32_t sub_1000ABE0 = 0x1000ABE0u;
constexpr uint32_t CDx_CreateDepthSurfaceLikeMain = 0x1000AD70u;
constexpr uint32_t CDx_StepFadeCounter = 0x1000AE40u;
constexpr uint32_t nullsub_36 = 0x1000BB10u;
constexpr uint32_t nullsub_37 = 0x1000BB20u;
constexpr uint32_t CDx_DrawStereoOddFrameOverlay = 0x1000BB90u;
constexpr uint32_t CDx_SetTransform = 0x1000BD50u;
constexpr uint32_t CDx_GetTransformCached = 0x1000BDB0u;
constexpr uint32_t sub_1000BE10 = 0x1000BE10u;
constexpr uint32_t Light_ApplyTimeOfDayTint = 0x1000BE50u;
constexpr uint32_t CDx_SetLight = 0x1000BF30u;
constexpr uint32_t CDx_CompositeSceneToBackBuffer = 0x1000C0B0u;
constexpr uint32_t CDx_CreateStereoResources = 0x1000C6E0u;
constexpr uint32_t CDx_ReleaseStereoResources = 0x1000C7C0u;
constexpr uint32_t CDx_CreateRampTexture1x16 = 0x1000C850u;
constexpr uint32_t CDx_CreateRampTextures1024x2 = 0x1000C900u;
constexpr uint32_t CDx_GetStereoSecondEyeTarget = 0x1000CA00u;
constexpr uint32_t CDx_DrawPrimitive = 0x1000CB20u;
constexpr uint32_t CDx_DrawIndexedPrimitive = 0x1000CC00u;
constexpr uint32_t CDx_DrawPrimitiveUP = 0x1000CD00u;
constexpr uint32_t CDx_ApplyStereoEyeProjection = 0x1000CF20u;
constexpr uint32_t CDx_RestoreProjection = 0x1000CF90u;
constexpr uint32_t CAcc_Constructor = 0x1000CFB0u;
constexpr uint32_t j_CEnv_UpdateDualTargetMode = 0x1000D3F0u;
constexpr uint32_t nullsub_38 = 0x1000D620u;
constexpr uint32_t CEnv_IsStereoMode = 0x1000D660u;
constexpr uint32_t App_RegisterSystemDats = 0x10010370u;
constexpr uint32_t sub_10010520 = 0x10010520u;
constexpr uint32_t App_Constructor = 0x10010700u;
constexpr uint32_t App_SetSeqStep = 0x10010CE0u;
constexpr uint32_t App_AdvanceSeqStep = 0x10010D00u;
constexpr uint32_t App_InitScene = 0x10010D20u;
constexpr uint32_t Post_Afterimage = 0x10012F40u;
constexpr uint32_t Post_FlashAndZoomFeedback = 0x100132B0u;
constexpr uint32_t sub_100139E0 = 0x100139E0u;
constexpr uint32_t fn_10013AC0 = 0x10013AC0u;
constexpr uint32_t App_StepGameState5Connect = 0x10013AD0u;
constexpr uint32_t App_StepGameState6 = 0x10013DF0u;
constexpr uint32_t App_StepGameState7 = 0x10013F10u;
constexpr uint32_t App_StepGameState8 = 0x10014030u;
constexpr uint32_t App_StepGameState9 = 0x10014120u;
constexpr uint32_t sub_10014170 = 0x10014170u;
constexpr uint32_t sub_100149B0 = 0x100149B0u;
constexpr uint32_t sub_10014A20 = 0x10014A20u;
constexpr uint32_t sub_10014A30 = 0x10014A30u;
constexpr uint32_t sub_10014AD0 = 0x10014AD0u;
constexpr uint32_t sub_10014C10 = 0x10014C10u;
constexpr uint32_t Camera_GetFrameScale = 0x10014CF0u;
constexpr uint32_t GetFrameCounter = 0x10014D50u;
constexpr uint32_t Camera_GetFloat2DC = 0x10014D70u;
constexpr uint32_t Camera_GetFloat2E0 = 0x10014D80u;
constexpr uint32_t Camera_SetClearColorRGB = 0x10014D90u;
constexpr uint32_t sub_10014EF0 = 0x10014EF0u;
constexpr uint32_t sub_10014F20 = 0x10014F20u;
constexpr uint32_t sub_10015050 = 0x10015050u;
constexpr uint32_t Camera_SetViewMatrix = 0x100150D0u;
constexpr uint32_t Camera_SetMatrix154 = 0x100151F0u;
constexpr uint32_t Camera_GetViewMatrixPtr = 0x10015220u;
constexpr uint32_t sub_10015240 = 0x10015240u;
constexpr uint32_t Camera_GetField50 = 0x10015250u;
constexpr uint32_t Camera_GetMatrix154Ptr = 0x10015270u;
constexpr uint32_t sub_10015290 = 0x10015290u;
constexpr uint32_t sub_10015360 = 0x10015360u;
constexpr uint32_t sub_100153A0 = 0x100153A0u;
constexpr uint32_t Camera_BuildProjection = 0x10015480u;
constexpr uint32_t Camera_ApplyViewProjection = 0x10015510u;
constexpr uint32_t Camera_GetViewportWidth = 0x10015680u;
constexpr uint32_t Camera_GetViewportHeight = 0x10015690u;
constexpr uint32_t sub_100156A0 = 0x100156A0u;
constexpr uint32_t fn_10015740 = 0x10015740u;
constexpr uint32_t App_RunWithCOM = 0x10015AB0u;
constexpr uint32_t App_InitDataPaths = 0x10015CB0u;
constexpr uint32_t App_DestroyDataPaths = 0x10015F50u;
constexpr uint32_t App_CheckOSVersion = 0x10015F80u;
constexpr uint32_t App_IsRegistryFlagSet = 0x100160C0u;
constexpr uint32_t App_InitSoundSystem = 0x10016150u;
constexpr uint32_t thunk_sub_10035900 = 0x100161A0u;
constexpr uint32_t sub_100163C0 = 0x100163C0u;
constexpr uint32_t sub_100163E0 = 0x100163E0u;
constexpr uint32_t App_SetGlobal_104569C8 = 0x10016400u;
constexpr uint32_t sub_10016410 = 0x10016410u;
constexpr uint32_t App_SetGlobal_104569E8 = 0x10016490u;
constexpr uint32_t App_SetErrorCode = 0x100164C0u;
constexpr uint32_t sub_10016560 = 0x10016560u;
constexpr uint32_t App_CreateMainWindow = 0x100165B0u;
constexpr uint32_t App_DestroyMainWindow = 0x10016820u;
constexpr uint32_t App_LoadImeUiDll = 0x10016E40u;
constexpr uint32_t App_UninitImeUi = 0x10016F10u;
constexpr uint32_t App_EnableImeUi = 0x10016F40u;
constexpr uint32_t App_LoadKeyInputDll = 0x10016F60u;
constexpr uint32_t App_UnloadKeyInputDll = 0x10016FF0u;
constexpr uint32_t GammaRamp_SetRGB = 0x100170A0u;
constexpr uint32_t GammaRamp_GetRGB = 0x100170D0u;
constexpr uint32_t GammaRamp_GetChannelSamples = 0x10017100u;
constexpr uint32_t GammaRamp_BuildRamps = 0x10017160u;
constexpr uint32_t GammaRamp_ApplyIfChanged = 0x10017230u;
constexpr uint32_t CShader_Destructor = 0x100177A0u;
constexpr uint32_t fn_10017BB0 = 0x10017BB0u;
constexpr uint32_t sub_10017C20 = 0x10017C20u;
constexpr uint32_t nullsub_13 = 0x10017D20u;
constexpr uint32_t nullsub_12 = 0x10017D30u;
constexpr uint32_t nullsub_90 = 0x10017D50u;
constexpr uint32_t _exit_0 = 0x100182E0u;
constexpr uint32_t sub_10018430 = 0x10018430u;
constexpr uint32_t sub_100185A0 = 0x100185A0u;
constexpr uint32_t sub_10018850 = 0x10018850u;
constexpr uint32_t OpeningSubtitle_Constructor = 0x10018A50u;
constexpr uint32_t OpeningSubtitle_Reset = 0x10018B30u;
constexpr uint32_t Vt10329FE0_Constructor = 0x10018C50u;
constexpr uint32_t Vt10329FE0_Reset = 0x10018CE0u;
constexpr uint32_t App_BeginTimerPeriod = 0x10019140u;
constexpr uint32_t sub_100191B0 = 0x100191B0u;
constexpr uint32_t sub_100191D0 = 0x100191D0u;
constexpr uint32_t CYyMotionQue_SetSlotMotion = 0x1001A660u;
constexpr uint32_t Model_CreateTranslucentTargets = 0x1001D0A0u;
constexpr uint32_t Model_BeginTranslucentPass = 0x1001D240u;
constexpr uint32_t Model_TranslucentUnbindTexture = 0x1001D350u;
constexpr uint32_t sub_1001D390 = 0x1001D390u;
constexpr uint32_t Model_CompositeTranslucentPass = 0x1001D3B0u;
constexpr uint32_t Gfx_DrawScreenQuad256 = 0x1001DD00u;
constexpr uint32_t Tex_HeightToNormalMap = 0x1001E0A0u;
constexpr uint32_t CYyCamMng2_CreateGlobal = 0x1001E470u;
constexpr uint32_t CYyCamMng2_CallGlobalVfunc1 = 0x1001E4B0u;
constexpr uint32_t sub_1001E760 = 0x1001E760u;
constexpr uint32_t sub_1001E780 = 0x1001E780u;
constexpr uint32_t sub_10021280 = 0x10021280u;
constexpr uint32_t Camera_UpdateController = 0x10021900u;
constexpr uint32_t sub_10021AE0 = 0x10021AE0u;
constexpr uint32_t sub_10021B70 = 0x10021B70u;
constexpr uint32_t Actor_GetPercentScaleByIndex = 0x10022120u;
constexpr uint32_t Actor_LookupPercentScaleA = 0x10022190u;
constexpr uint32_t Actor_LookupPercentScaleB = 0x100221E0u;
constexpr uint32_t Actor_InitIndexTable = 0x10022230u;
constexpr uint32_t Actor_SetPair14 = 0x10022270u;
constexpr uint32_t Actor_SetPair1C = 0x10022300u;
constexpr uint32_t Actor_LookupByteField1C = 0x10022340u;
constexpr uint32_t CYyModelDt_PrepareRenderState = 0x100224D0u;
constexpr uint32_t CYyModelDt_SetVertexShader = 0x10022FA0u;
constexpr uint32_t CYyModelDt_ExpandTriList = 0x10023010u;
constexpr uint32_t CYyModelDt_ExpandTriStrip = 0x100230D0u;
constexpr uint32_t CYyModelDt_PrepareRenderStateSimple = 0x100231C0u;
constexpr uint32_t SkinVerticesSimple = 0x100231F0u;
constexpr uint32_t SkinVertices = 0x10023290u;
constexpr uint32_t CYyModelDt_Destructor = 0x100250C0u;
constexpr uint32_t ProjTex_Destructor = 0x100252F0u;
constexpr uint32_t ProjTex_BeginPass = 0x10025330u;
constexpr uint32_t ProjTex_EndPass = 0x100255C0u;
constexpr uint32_t TexObj_GetD3DTexture = 0x10025860u;
constexpr uint32_t ProjTex_CaptureToTarget = 0x10025870u;
constexpr uint32_t nullsub_39 = 0x10025BD0u;
constexpr uint32_t EnvMap_BeginPass = 0x10025C40u;
constexpr uint32_t EnvMap_EndPass = 0x10025F70u;
constexpr uint32_t sub_10026210 = 0x10026210u;
constexpr uint32_t CYyIcon_Constructor = 0x10026890u;
constexpr uint32_t CYyIcon_DrawSubRect = 0x10026C70u;
constexpr uint32_t sub_10026E00 = 0x10026E00u;
constexpr uint32_t Quat_SetIdentity = 0x10026E10u;
constexpr uint32_t Vec4_Set = 0x10026E30u;
constexpr uint32_t Vec4_Copy = 0x10026E90u;
constexpr uint32_t Vec3_Copy = 0x10026EB0u;
constexpr uint32_t sub_10026EF0 = 0x10026EF0u;
constexpr uint32_t Vec3_AddInPlace = 0x10026F20u;
constexpr uint32_t sub_10026F50 = 0x10026F50u;
constexpr uint32_t Vec3_Add_10026FA0 = 0x10026FA0u;
constexpr uint32_t Vec2_Add = 0x10026FD0u;
constexpr uint32_t sub_10027020 = 0x10027020u;
constexpr uint32_t Vec2_AddScalar = 0x10027050u;
constexpr uint32_t fn_100270A0 = 0x100270A0u;
constexpr uint32_t Vec3_Sub = 0x10027120u;
constexpr uint32_t fn_100272B0 = 0x100272B0u;
constexpr uint32_t Vec3_Scale = 0x100272E0u;
constexpr uint32_t Vec3_MulInPlace = 0x10027310u;
constexpr uint32_t sub_10027340 = 0x10027340u;
constexpr uint32_t Vec2_Mul = 0x100273D0u;
constexpr uint32_t sub_100274B0 = 0x100274B0u;
constexpr uint32_t sub_10027530 = 0x10027530u;
constexpr uint32_t Gfx_ProjectWorldToClip = 0x100276F0u;
constexpr uint32_t Gfx_TransformWorldToView = 0x100277E0u;
constexpr uint32_t Gfx_ProjectViewToClip = 0x10027870u;
constexpr uint32_t Gfx_NdcToScreen = 0x100278E0u;
constexpr uint32_t fn_10027990 = 0x10027990u;
constexpr uint32_t nullsub_1 = 0x100279A0u;
constexpr uint32_t Mat4_SetIdentity = 0x100279B0u;
constexpr uint32_t Mat4_Copy = 0x100279F0u;
constexpr uint32_t Mat4_PostScale = 0x10027A60u;
constexpr uint32_t Mat4_BuildScale = 0x10027AE0u;
constexpr uint32_t Mat4_SetScale = 0x10027B30u;
constexpr uint32_t Mat4_PostRotateX = 0x10027B80u;
constexpr uint32_t Mat4_PostRotateY = 0x10027BD0u;
constexpr uint32_t Mat4_PostRotateZ = 0x10027C20u;
constexpr uint32_t Mat4_SetRow3 = 0x10027CD0u;
constexpr uint32_t Mat4_SetTranslation = 0x10027CF0u;
constexpr uint32_t Mat4_PostMultiply = 0x10027D10u;
constexpr uint32_t Mat4_PreMultiply = 0x10027E90u;
constexpr uint32_t sub_10027F60 = 0x10027F60u;
constexpr uint32_t Mat4_TransformVec4InPlace = 0x10028140u;
constexpr uint32_t sub_10028170 = 0x10028170u;
constexpr uint32_t Mat4_TransformPointInPlace = 0x10028200u;
constexpr uint32_t sub_10028230 = 0x10028230u;
constexpr uint32_t Mat4_TransformNormalInPlace = 0x10028290u;
constexpr uint32_t Mat4_TransformNormal = 0x100282C0u;
constexpr uint32_t Mat4_TransformCoordInPlace = 0x10028320u;
constexpr uint32_t sub_10028370 = 0x10028370u;
constexpr uint32_t Mat4_InvertOrthonormal = 0x100283C0u;
constexpr uint32_t Mem_AllocObject = 0x10028640u;
constexpr uint32_t Mem_AllocWithFallback = 0x100286B0u;
constexpr uint32_t Mem_FreeObject = 0x10028700u;
constexpr uint32_t SharedDeleteHelper = 0x10028710u;
constexpr uint32_t MemBlock_GetSize = 0x10028B90u;
constexpr uint32_t Mem_AllocFromArea = 0x10028FD0u;
constexpr uint32_t sub_10029A50 = 0x10029A50u;
constexpr uint32_t CYyModelDtBase_Destructor = 0x10029CD0u;
constexpr uint32_t CYyModelBase_Destructor = 0x10029DD0u;
constexpr uint32_t CYyModelBase_SetResourceAndRebuild = 0x10029E50u;
constexpr uint32_t CYyModelBase_GetPartList = 0x10029E70u;
constexpr uint32_t CYyModelBase_AddPartFromResource = 0x10029F70u;
constexpr uint32_t CYyModelBase_DrawLit = 0x1002A3B0u;
constexpr uint32_t CYyModelBase_ForwardToField24 = 0x1002AC50u;
constexpr uint32_t CYyModel_Constructor = 0x1002B4B0u;
constexpr uint32_t CYyModel_Destructor = 0x1002B550u;
constexpr uint32_t CYyModel_GetField44Addr = 0x1002B5A0u;
constexpr uint32_t CYyModel_CreateModelBase = 0x1002B6A0u;
constexpr uint32_t CXiSkeletonActor_DrawModel = 0x1002B6E0u;
constexpr uint32_t sub_1002C270 = 0x1002C270u;
constexpr uint32_t sub_1002C2A0 = 0x1002C2A0u;
constexpr uint32_t CXiSkeletonActor_AnimateModels = 0x1002C2D0u;
constexpr uint32_t CXiSkeletonActor_PrepareModelsOnce = 0x1002C360u;
constexpr uint32_t CXiSkeletonActor_GetElemThunk = 0x1002C3E0u;
constexpr uint32_t CXiSkeletonActor_GetModelEidMatrix = 0x1002C570u;
constexpr uint32_t CXiSkeletonActor_GetModelElem = 0x1002C5C0u;
constexpr uint32_t List_GetTail = 0x1002C7C0u;
constexpr uint32_t Mem_AllocDefault = 0x1002C890u;
constexpr uint32_t thunk_sub_10028AA0 = 0x1002C8B0u;
constexpr uint32_t CYyObject_IsKindOf = 0x1002C8F0u;
constexpr uint32_t Gfx_SetStreamSourceCached = 0x1002F130u;
constexpr uint32_t Gfx_ApplyCachedVertexShader = 0x1002F1A0u;
constexpr uint32_t Gfx_SetVertexShaderCached = 0x1002F1C0u;
constexpr uint32_t Model_SkinVerticesCpu = 0x1002F1D0u;
constexpr uint32_t BoneXform_PostMultiplyInto = 0x10032FA0u;
constexpr uint32_t sub_100336B0 = 0x100336B0u;
constexpr uint32_t Model_BeginOffscreenPass = 0x10033800u;
constexpr uint32_t Model_EndOffscreenPassBindTextures = 0x10033A70u;
constexpr uint32_t sub_10033DA0 = 0x10033DA0u;
constexpr uint32_t Shadow_SmoothLightDirection = 0x10033EA0u;
constexpr uint32_t Skel_OffsetAllTranslations = 0x10035170u;
constexpr uint32_t sub_10035250 = 0x10035250u;
constexpr uint32_t CMoTask_VT_1032A904_Destructor = 0x10035B00u;
constexpr uint32_t sub_10035E30 = 0x10035E30u;
constexpr uint32_t MoTask_KillAllGlobalSlots = 0x10035F00u;
constexpr uint32_t MoTask_KillGlobalSlot1 = 0x10036070u;
constexpr uint32_t sub_100361D0 = 0x100361D0u;
constexpr uint32_t sub_10036230 = 0x10036230u;
constexpr uint32_t sub_10036B90 = 0x10036B90u;
constexpr uint32_t sub_10036F00 = 0x10036F00u;
constexpr uint32_t sub_10037020 = 0x10037020u;
constexpr uint32_t sub_100370D0 = 0x100370D0u;
constexpr uint32_t Font_QueueText = 0x100384B0u;
constexpr uint32_t Font_DrawPass = 0x10038520u;
constexpr uint32_t Font_DrawString = 0x10038720u;
constexpr uint32_t Post_CreateScreenEffectTargets = 0x10038930u;
constexpr uint32_t sub_10038A70 = 0x10038A70u;
constexpr uint32_t Post_CaptureSceneToTarget = 0x10038A80u;
constexpr uint32_t Post_DrawRotatingGhostLayers = 0x10038E10u;
constexpr uint32_t sub_10039410 = 0x10039410u;
constexpr uint32_t Util_Copy16Bytes = 0x10039810u;
constexpr uint32_t CYyTexBase_Destructor = 0x100398D0u;
constexpr uint32_t CYyTexBase_SetName = 0x100398E0u;
constexpr uint32_t CYyTex_Destructor = 0x10039970u;
constexpr uint32_t TexObj_CreateAndUpload = 0x10039A30u;
constexpr uint32_t TexObj_Upload = 0x10039E50u;
constexpr uint32_t sub_1003AA50 = 0x1003AA50u;
constexpr uint32_t CYyTexMng_Destructor = 0x1003AA90u;
constexpr uint32_t TexMgr_UploadPumpEntry = 0x1003AB70u;
constexpr uint32_t CYyTexMng_FindByName = 0x1003AC80u;
constexpr uint32_t sub_1003ACD0 = 0x1003ACD0u;
constexpr uint32_t sub_1003AD40 = 0x1003AD40u;
constexpr uint32_t sub_1003ADC0 = 0x1003ADC0u;
constexpr uint32_t TexMgr_FindByName = 0x1003AEA0u;
constexpr uint32_t CYyTexMng_FindByNameInResource = 0x1003AF10u;
constexpr uint32_t sub_1003B080 = 0x1003B080u;
constexpr uint32_t CYyVbMng_AcquireVertexBuffer = 0x1003B210u;
constexpr uint32_t CYyVbMng_ReleaseVertexBuffer = 0x1003B3C0u;
constexpr uint32_t CYyVb_Constructor = 0x1003B450u;
constexpr uint32_t sub_1003B540 = 0x1003B540u;
constexpr uint32_t ReleaseSubObjectIfPresent = 0x1003B5C0u;
constexpr uint32_t CMoAttachments_Reset = 0x1003B640u;
constexpr uint32_t sub_1003B6D0 = 0x1003B6D0u;
constexpr uint32_t sub_1003B700 = 0x1003B700u;
constexpr uint32_t CMoAttachments_ClearCaster = 0x1003B750u;
constexpr uint32_t CMoAttachments_ClearTarget = 0x1003B780u;
constexpr uint32_t CMoAttachments_SetCaster = 0x1003B7B0u;
constexpr uint32_t CMoAttachments_SetTarget = 0x1003B830u;
constexpr uint32_t sub_1003B8B0 = 0x1003B8B0u;
constexpr uint32_t sub_1003B900 = 0x1003B900u;
constexpr uint32_t sub_1003B950 = 0x1003B950u;
constexpr uint32_t sub_1003B980 = 0x1003B980u;
constexpr uint32_t sub_1003BA20 = 0x1003BA20u;
constexpr uint32_t sub_1003BD90 = 0x1003BD90u;
constexpr uint32_t CMoCamera_GetRuntimeClass = 0x1003C880u;
constexpr uint32_t sub_1003C8A0 = 0x1003C8A0u;
constexpr uint32_t sub_1003CBB0 = 0x1003CBB0u;
constexpr uint32_t CMoCameraTask_Destructor = 0x1003D030u;
constexpr uint32_t Sky_DrawGradientDome = 0x1003D640u;
constexpr uint32_t CMoCelestial_Destructor = 0x1003D980u;
constexpr uint32_t Sky_DestroyDomeShaderStatic = 0x1003DB50u;
constexpr uint32_t Sky_DestroyViewCenteredShaderStatic = 0x1003DDF0u;
constexpr uint32_t sub_1003DE40 = 0x1003DE40u;
constexpr uint32_t Color_SetFromU32SwapRB = 0x1003E280u;
constexpr uint32_t Color_CopySwapRB = 0x1003E2A0u;
constexpr uint32_t sub_1003E910 = 0x1003E910u;
constexpr uint32_t unknown_libname_20 = 0x1003EB00u;
constexpr uint32_t unknown_libname_21 = 0x1003EB10u;
constexpr uint32_t unknown_libname_22 = 0x1003EB20u;
constexpr uint32_t CMoVuAnmElem_Destructor = 0x1003EF20u;
constexpr uint32_t CMoDistMorphElem_ApplyMorph = 0x1003F440u;
constexpr uint32_t unknown_libname_25 = 0x1003F890u;
constexpr uint32_t CMoDistMorphElem_ComputeMorphHandle = 0x1003F8A0u;
constexpr uint32_t CXi471011C8_VT_1032AC90_Dtor = 0x1003FB10u;
constexpr uint32_t sub_1003FCB0 = 0x1003FCB0u;
constexpr uint32_t sub_1003FCE0 = 0x1003FCE0u;
constexpr uint32_t sub_1003FCF0 = 0x1003FCF0u;
constexpr uint32_t MoElem_GetVertexData = 0x1003FD70u;
constexpr uint32_t CMoDistModelElem_DrawMesh = 0x10040670u;
constexpr uint32_t unknown_libname_33 = 0x10040B50u;
constexpr uint32_t unknown_libname_34 = 0x10040B60u;
constexpr uint32_t unknown_libname_35 = 0x10040B70u;
constexpr uint32_t unknown_libname_36 = 0x10040B80u;
constexpr uint32_t unknown_libname_37 = 0x10040B90u;
constexpr uint32_t unknown_libname_38 = 0x10040E10u;
constexpr uint32_t CXi471011C8_VT_1032AD80_Dtor = 0x10041110u;
constexpr uint32_t Font_DrawProjectedText3D = 0x10041C20u;
constexpr uint32_t StaticDtor_StateBlock_1047BADC = 0x10041F30u;
constexpr uint32_t CMoOtTask_ScalarDeletingDtor = 0x100425F0u;
constexpr uint32_t unknown_libname_46 = 0x10042CD0u;
constexpr uint32_t unknown_libname_47 = 0x10042CE0u;
constexpr uint32_t nullsub_136 = 0x10042CF0u;
constexpr uint32_t CMoDistMorphElem_ValidateMorphData = 0x10042D00u;
constexpr uint32_t unknown_libname_48 = 0x100430A0u;
constexpr uint32_t unknown_libname_49 = 0x10043310u;
constexpr uint32_t StaticDtor_StateBlock_1047BB7C = 0x100439B0u;
constexpr uint32_t sub_100439C0 = 0x100439C0u;
constexpr uint32_t CMoTask_VT_1032AF60_ScalarDeletingDtor = 0x10043A90u;
constexpr uint32_t CMoTask_VT_1032AF60_Destructor = 0x10043AB0u;
constexpr uint32_t sub_10043BB0 = 0x10043BB0u;
constexpr uint32_t CMoTask_VT_1032AFA8_ScalarDeletingDtor = 0x10043C90u;
constexpr uint32_t CMoTask_VT_1032AFA8_Destructor = 0x10043CB0u;
constexpr uint32_t sub_10043DE0 = 0x10043DE0u;
constexpr uint32_t DynVB_Constructor = 0x10043DF0u;
constexpr uint32_t DynVB_Destructor = 0x10043E40u;
constexpr uint32_t DynVB_Create = 0x10043E50u;
constexpr uint32_t sub_10043F00 = 0x10043F00u;
constexpr uint32_t sub_10043F40 = 0x10043F40u;
constexpr uint32_t DynVB_Reserve = 0x10044010u;
constexpr uint32_t DynVB_Lock = 0x10044130u;
constexpr uint32_t DynVB_Unlock = 0x100441A0u;
constexpr uint32_t sub_100441C0 = 0x100441C0u;
constexpr uint32_t Gfx_SetViewMatrixFromField10 = 0x10044210u;
constexpr uint32_t Gfx_SetViewMatrixFromField90 = 0x10044230u;
constexpr uint32_t Mo_InitSharedRenderResources = 0x10044270u;
constexpr uint32_t sub_10044490 = 0x10044490u;
constexpr uint32_t sub_10044540 = 0x10044540u;
constexpr uint32_t CMoElemFamily_SharedCleanup = 0x10044970u;
constexpr uint32_t sub_10044C40 = 0x10044C40u;
constexpr uint32_t sub_10045800 = 0x10045800u;
constexpr uint32_t nullsub_140 = 0x10045A30u;
constexpr uint32_t sub_10045C50 = 0x10045C50u;
constexpr uint32_t RegisterWithProcessor = 0x10045D40u;
constexpr uint32_t sub_100464D0 = 0x100464D0u;
constexpr uint32_t sub_10046550 = 0x10046550u;
constexpr uint32_t unknown_libname_53 = 0x100472D0u;
constexpr uint32_t unknown_libname_54 = 0x100473E0u;
constexpr uint32_t sub_100473F0 = 0x100473F0u;
constexpr uint32_t CMoSkeletonElem_Destructor = 0x10047900u;
constexpr uint32_t CMoGeneratorClone_VT_1032B114_Constructor = 0x10047F80u;
constexpr uint32_t CMoGeneratorClone_VT_1032B114_ScalarDeletingDtor = 0x10047FE0u;
constexpr uint32_t CMoGeneratorClone_VT_1032B114_Destructor = 0x10048000u;
constexpr uint32_t CMoGeneratorClone_UnlinkElemListA = 0x10048140u;
constexpr uint32_t CMoGeneratorClone_UnlinkElemListB = 0x100481D0u;
constexpr uint32_t CMoGeneratorClone_FindCmdByType = 0x100492F0u;
constexpr uint32_t sub_10049560 = 0x10049560u;
constexpr uint32_t CMoGeneratorClone_ResolveTarget = 0x10049570u;
constexpr uint32_t sub_100496F0 = 0x100496F0u;
constexpr uint32_t sub_1004E3B0 = 0x1004E3B0u;
constexpr uint32_t sub_1004E3C0 = 0x1004E3C0u;
constexpr uint32_t fn_100527F0 = 0x100527F0u;
constexpr uint32_t sub_10052870 = 0x10052870u;
constexpr uint32_t sub_10052880 = 0x10052880u;
constexpr uint32_t sub_10052890 = 0x10052890u;
constexpr uint32_t sub_100528A0 = 0x100528A0u;
constexpr uint32_t SharedConditionalStateReset = 0x100531D0u;
constexpr uint32_t sub_10053370 = 0x10053370u;
constexpr uint32_t CMoGeneratorClone_CopyParams = 0x10053450u;
constexpr uint32_t CMoGeneratorClone_CreateInstance = 0x100536C0u;
constexpr uint32_t CMoGeneratorClone_VT_1032B69C_ScalarDeletingDtor = 0x10053840u;
constexpr uint32_t sub_10053890 = 0x10053890u;
constexpr uint32_t sub_100538E0 = 0x100538E0u;
constexpr uint32_t sub_100539E0 = 0x100539E0u;
constexpr uint32_t sub_10053A00 = 0x10053A00u;
constexpr uint32_t sub_10053E60 = 0x10053E60u;
constexpr uint32_t CMoTask_VT_1032B654_Destructor = 0x100541C0u;
constexpr uint32_t CMoGeneratorClone_GetRuntimeClass = 0x100542E0u;
constexpr uint32_t CMoGeneratorClone_Destructor = 0x100542F0u;
constexpr uint32_t CMoGeneratorClone_Kill = 0x100543A0u;
constexpr uint32_t CMoLfdElem_Destructor = 0x100548C0u;
constexpr uint32_t nullsub_130 = 0x10054F70u;
constexpr uint32_t Vt1032B6D8_Destructor = 0x10055000u;
constexpr uint32_t LinkNode_Init = 0x10055240u;
constexpr uint32_t LinkNode_Unlink = 0x10055250u;
constexpr uint32_t CMoOT_Destructor = 0x100552E0u;
constexpr uint32_t sub_100553C0 = 0x100553C0u;
constexpr uint32_t sub_100555B0 = 0x100555B0u;
constexpr uint32_t sub_100555D0 = 0x100555D0u;
constexpr uint32_t CMoSphRes_FixupOffsets = 0x10056220u;
constexpr uint32_t CMoPointLightProgElem_Destructor = 0x10056390u;
constexpr uint32_t unknown_libname_55 = 0x10056840u;
constexpr uint32_t CMoPathDriveActorTask_GetRuntimeClass = 0x10056940u;
constexpr uint32_t CMoLockColorDriveTask_GetRuntimeClass = 0x10056960u;
constexpr uint32_t CMoLockLookAtDriveTask_GetRuntimeClass = 0x10056980u;
constexpr uint32_t CMoActorColorDriveTask_GetRuntimeClass = 0x100569A0u;
constexpr uint32_t CMoActorRotationDriveTask_GetRuntimeClass = 0x100569C0u;
constexpr uint32_t sub_10056BB0 = 0x10056BB0u;
constexpr uint32_t Scheduler_Execute = 0x10056C70u;
constexpr uint32_t sub_10056EB0 = 0x10056EB0u;
constexpr uint32_t Scheduler_Kill = 0x10056EF0u;
constexpr uint32_t CMoSchedularTask_Destructor = 0x100574A0u;
constexpr uint32_t sub_10057BE0 = 0x10057BE0u;
constexpr uint32_t sub_10057BF0 = 0x10057BF0u;
constexpr uint32_t CMoSchedularTask_Interpret = 0x10057FB0u;
constexpr uint32_t sub_1005DFF0 = 0x1005DFF0u;
constexpr uint32_t sub_1005E000 = 0x1005E000u;
constexpr uint32_t sub_1005E020 = 0x1005E020u;
constexpr uint32_t sub_1005E040 = 0x1005E040u;
constexpr uint32_t sub_1005E070 = 0x1005E070u;
constexpr uint32_t sub_1005E090 = 0x1005E090u;
constexpr uint32_t sub_1005E0B0 = 0x1005E0B0u;
constexpr uint32_t sub_1005E0E0 = 0x1005E0E0u;
constexpr uint32_t sub_1005E0F0 = 0x1005E0F0u;
constexpr uint32_t fn_1005E100 = 0x1005E100u;
constexpr uint32_t sub_1005E110 = 0x1005E110u;
constexpr uint32_t sub_1005E120 = 0x1005E120u;
constexpr uint32_t sub_1005E130 = 0x1005E130u;
constexpr uint32_t sub_1005E140 = 0x1005E140u;
constexpr uint32_t sub_1005E150 = 0x1005E150u;
constexpr uint32_t sub_1005E160 = 0x1005E160u;
constexpr uint32_t sub_1005E170 = 0x1005E170u;
constexpr uint32_t sub_1005E180 = 0x1005E180u;
constexpr uint32_t sub_1005E190 = 0x1005E190u;
constexpr uint32_t sub_1005E1A0 = 0x1005E1A0u;
constexpr uint32_t sub_1005E1C0 = 0x1005E1C0u;
constexpr uint32_t sub_1005E1D0 = 0x1005E1D0u;
constexpr uint32_t sub_1005E1F0 = 0x1005E1F0u;
constexpr uint32_t sub_1005E200 = 0x1005E200u;
constexpr uint32_t sub_1005E210 = 0x1005E210u;
constexpr uint32_t sub_1005E230 = 0x1005E230u;
constexpr uint32_t sub_1005E240 = 0x1005E240u;
constexpr uint32_t sub_1005E250 = 0x1005E250u;
constexpr uint32_t sub_1005E260 = 0x1005E260u;
constexpr uint32_t sub_1005E270 = 0x1005E270u;
constexpr uint32_t sub_1005E280 = 0x1005E280u;
constexpr uint32_t sub_1005E290 = 0x1005E290u;
constexpr uint32_t sub_1005E2A0 = 0x1005E2A0u;
constexpr uint32_t sub_1005E2B0 = 0x1005E2B0u;
constexpr uint32_t sub_1005E2C0 = 0x1005E2C0u;
constexpr uint32_t sub_1005E2D0 = 0x1005E2D0u;
constexpr uint32_t sub_1005E2E0 = 0x1005E2E0u;
constexpr uint32_t sub_1005E2F0 = 0x1005E2F0u;
constexpr uint32_t unknown_libname_57 = 0x1005E300u;
constexpr uint32_t sub_1005E310 = 0x1005E310u;
constexpr uint32_t sub_1005E320 = 0x1005E320u;
constexpr uint32_t sub_1005E330 = 0x1005E330u;
constexpr uint32_t sub_1005E350 = 0x1005E350u;
constexpr uint32_t sub_1005E360 = 0x1005E360u;
constexpr uint32_t sub_1005E380 = 0x1005E380u;
constexpr uint32_t sub_1005E3A0 = 0x1005E3A0u;
constexpr uint32_t sub_1005E3E0 = 0x1005E3E0u;
constexpr uint32_t sub_1005E420 = 0x1005E420u;
constexpr uint32_t sub_1005E460 = 0x1005E460u;
constexpr uint32_t sub_1005E4A0 = 0x1005E4A0u;
constexpr uint32_t sub_1005E4B0 = 0x1005E4B0u;
constexpr uint32_t sub_1005E4C0 = 0x1005E4C0u;
constexpr uint32_t sub_1005E4D0 = 0x1005E4D0u;
constexpr uint32_t sub_1005E4E0 = 0x1005E4E0u;
constexpr uint32_t sub_1005E4F0 = 0x1005E4F0u;
constexpr uint32_t sub_1005E500 = 0x1005E500u;
constexpr uint32_t sub_1005E510 = 0x1005E510u;
constexpr uint32_t sub_1005E520 = 0x1005E520u;
constexpr uint32_t sub_1005E530 = 0x1005E530u;
constexpr uint32_t sub_1005E540 = 0x1005E540u;
constexpr uint32_t sub_1005E550 = 0x1005E550u;
constexpr uint32_t sub_1005E570 = 0x1005E570u;
constexpr uint32_t sub_1005E590 = 0x1005E590u;
constexpr uint32_t sub_1005E5B0 = 0x1005E5B0u;
constexpr uint32_t sub_1005E5D0 = 0x1005E5D0u;
constexpr uint32_t sub_1005E5E0 = 0x1005E5E0u;
constexpr uint32_t sub_1005E5F0 = 0x1005E5F0u;
constexpr uint32_t sub_1005E600 = 0x1005E600u;
constexpr uint32_t sub_1005E610 = 0x1005E610u;
constexpr uint32_t sub_1005E620 = 0x1005E620u;
constexpr uint32_t sub_1005E630 = 0x1005E630u;
constexpr uint32_t sub_1005E640 = 0x1005E640u;
constexpr uint32_t sub_1005E650 = 0x1005E650u;
constexpr uint32_t sub_1005E660 = 0x1005E660u;
constexpr uint32_t sub_1005E680 = 0x1005E680u;
constexpr uint32_t sub_1005E690 = 0x1005E690u;
constexpr uint32_t sub_1005E6A0 = 0x1005E6A0u;
constexpr uint32_t sub_1005E6B0 = 0x1005E6B0u;
constexpr uint32_t sub_1005E6C0 = 0x1005E6C0u;
constexpr uint32_t sub_1005E6D0 = 0x1005E6D0u;
constexpr uint32_t sub_1005E6E0 = 0x1005E6E0u;
constexpr uint32_t sub_1005E6F0 = 0x1005E6F0u;
constexpr uint32_t sub_1005E700 = 0x1005E700u;
constexpr uint32_t sub_1005E710 = 0x1005E710u;
constexpr uint32_t sub_1005E720 = 0x1005E720u;
constexpr uint32_t sub_1005E740 = 0x1005E740u;
constexpr uint32_t sub_1005E750 = 0x1005E750u;
constexpr uint32_t sub_1005E760 = 0x1005E760u;
constexpr uint32_t sub_1005E770 = 0x1005E770u;
constexpr uint32_t sub_1005E780 = 0x1005E780u;
constexpr uint32_t sub_1005E790 = 0x1005E790u;
constexpr uint32_t sub_1005E7A0 = 0x1005E7A0u;
constexpr uint32_t sub_1005E7B0 = 0x1005E7B0u;
constexpr uint32_t sub_1005E7C0 = 0x1005E7C0u;
constexpr uint32_t sub_1005E7D0 = 0x1005E7D0u;
constexpr uint32_t sub_1005E7E0 = 0x1005E7E0u;
constexpr uint32_t sub_1005E800 = 0x1005E800u;
constexpr uint32_t sub_1005E880 = 0x1005E880u;
constexpr uint32_t sub_1005E890 = 0x1005E890u;
constexpr uint32_t CMoPathDriveActorTask_ScalarDeletingDtor = 0x1005F2C0u;
constexpr uint32_t sub_1005F320 = 0x1005F320u;
constexpr uint32_t CMoLockColorDriveTask_ScalarDeletingDtor = 0x1005F3F0u;
constexpr uint32_t sub_1005F450 = 0x1005F450u;
constexpr uint32_t CMoLockLookAtDriveTask_ScalarDeletingDtor = 0x1005F660u;
constexpr uint32_t sub_1005F6C0 = 0x1005F6C0u;
constexpr uint32_t CMoActorColorDriveTask_ScalarDeletingDtor = 0x1005F990u;
constexpr uint32_t sub_1005FA20 = 0x1005FA20u;
constexpr uint32_t CMoActorRotationDriveTask_ScalarDeletingDtor = 0x1005FE30u;
constexpr uint32_t CMoAttachments_VT_1032B940_ScalarDeletingDtor = 0x10060190u;
constexpr uint32_t sub_100601F0 = 0x100601F0u;
constexpr uint32_t CMoAttachments_VT_1032BAF0_ScalarDeletingDtor = 0x100602D0u;
constexpr uint32_t sub_100607E0 = 0x100607E0u;
constexpr uint32_t CMoAttachments_VT_1032BB38_ScalarDeletingDtor = 0x100608B0u;
constexpr uint32_t sub_10060910 = 0x10060910u;
constexpr uint32_t sub_10060920 = 0x10060920u;
constexpr uint32_t sub_10060930 = 0x10060930u;
constexpr uint32_t sub_10060940 = 0x10060940u;
constexpr uint32_t sub_10060950 = 0x10060950u;
constexpr uint32_t sub_10060990 = 0x10060990u;
constexpr uint32_t sub_10060B30 = 0x10060B30u;
constexpr uint32_t CMoAttachments_VT_1032BBAC_VectorDeletingDtor = 0x10060BB0u;
constexpr uint32_t sub_10060DA0 = 0x10060DA0u;
constexpr uint32_t CMoAttachments_VT_1032BBF4_VectorDeletingDtor = 0x10060E60u;
constexpr uint32_t sub_10060F80 = 0x10060F80u;
constexpr uint32_t CMoAttachments_VT_1032BC68_VectorDeletingDtor = 0x10061010u;
constexpr uint32_t CMoAttachments_VT_1032BC84_VectorDeletingDtor = 0x10061140u;
constexpr uint32_t sub_10061240 = 0x10061240u;
constexpr uint32_t CMoAttachments_VT_1032BCCC_VectorDeletingDtor = 0x10061300u;
constexpr uint32_t sub_100613D0 = 0x100613D0u;
constexpr uint32_t CMoAttachments_VT_1032BD14_VectorDeletingDtor = 0x100614B0u;
constexpr uint32_t sub_100615B0 = 0x100615B0u;
constexpr uint32_t CMoAttachments_VT_1032BD5C_VectorDeletingDtor = 0x100616B0u;
constexpr uint32_t sub_100617B0 = 0x100617B0u;
constexpr uint32_t CMoAttachments_VT_1032BDA4_VectorDeletingDtor = 0x10061830u;
constexpr uint32_t sub_100618A0 = 0x100618A0u;
constexpr uint32_t sub_10061960 = 0x10061960u;
constexpr uint32_t CMoAttachments_VT_1032BDEC_VectorDeletingDtor = 0x10061A00u;
constexpr uint32_t sub_10061AF0 = 0x10061AF0u;
constexpr uint32_t CMoAttachments_VT_1032BE34_VectorDeletingDtor = 0x10061B80u;
constexpr uint32_t sub_10061C70 = 0x10061C70u;
constexpr uint32_t CMoAttachments_VT_1032BE7C_VectorDeletingDtor = 0x10061D00u;
constexpr uint32_t sub_10061E00 = 0x10061E00u;
constexpr uint32_t CMoAttachments_VT_1032BEC4_VectorDeletingDtor = 0x10061EA0u;
constexpr uint32_t sub_10061FD0 = 0x10061FD0u;
constexpr uint32_t CMoAttachments_VT_1032BF0C_VectorDeletingDtor = 0x10062090u;
constexpr uint32_t sub_10062180 = 0x10062180u;
constexpr uint32_t CMoAttachments_VT_1032BF54_VectorDeletingDtor = 0x10062210u;
constexpr uint32_t sub_10062310 = 0x10062310u;
constexpr uint32_t CMoAttachments_VT_1032BF9C_VectorDeletingDtor = 0x100623B0u;
constexpr uint32_t sub_100624B0 = 0x100624B0u;
constexpr uint32_t CMoAttachments_VT_1032BFE4_VectorDeletingDtor = 0x10062550u;
constexpr uint32_t sub_10062650 = 0x10062650u;
constexpr uint32_t CMoAttachments_VT_1032C02C_VectorDeletingDtor = 0x10062700u;
constexpr uint32_t sub_10062770 = 0x10062770u;
constexpr uint32_t sub_100627D0 = 0x100627D0u;
constexpr uint32_t sub_10062830 = 0x10062830u;
constexpr uint32_t sub_10062880 = 0x10062880u;
constexpr uint32_t sub_100628D0 = 0x100628D0u;
constexpr uint32_t sub_10062900 = 0x10062900u;
constexpr uint32_t sub_10062D50 = 0x10062D50u;
constexpr uint32_t sub_10062DE0 = 0x10062DE0u;
constexpr uint32_t CMoSpline_Destructor = 0x100634A0u;
constexpr uint32_t sub_10063B20 = 0x10063B20u;
constexpr uint32_t sub_10063B50 = 0x10063B50u;
constexpr uint32_t MoProgElem_FreeBuffer40 = 0x10063C60u;
constexpr uint32_t sub_100683D0 = 0x100683D0u;
constexpr uint32_t sub_100683F0 = 0x100683F0u;
constexpr uint32_t sub_100684F0 = 0x100684F0u;
constexpr uint32_t sub_10068500 = 0x10068500u;
constexpr uint32_t sub_10068510 = 0x10068510u;
constexpr uint32_t sub_100685C0 = 0x100685C0u;
constexpr uint32_t sub_100685F0 = 0x100685F0u;
constexpr uint32_t sub_10068890 = 0x10068890u;
constexpr uint32_t sub_10068AE0 = 0x10068AE0u;
constexpr uint32_t sub_10068CE0 = 0x10068CE0u;
constexpr uint32_t sub_10068EF0 = 0x10068EF0u;
constexpr uint32_t sub_10068F80 = 0x10068F80u;
constexpr uint32_t sub_10069010 = 0x10069010u;
constexpr uint32_t sub_100690A0 = 0x100690A0u;
constexpr uint32_t sub_10069130 = 0x10069130u;
constexpr uint32_t sub_100691C0 = 0x100691C0u;
constexpr uint32_t sub_10069D30 = 0x10069D30u;
constexpr uint32_t CMoDX_Destructor = 0x10069D70u;
constexpr uint32_t CMoDX_Initialize = 0x10069DD0u;
constexpr uint32_t nullsub_43 = 0x10069E90u;
constexpr uint32_t sub_10069EE0 = 0x10069EE0u;
constexpr uint32_t sub_10069F90 = 0x10069F90u;
constexpr uint32_t sub_1006A060 = 0x1006A060u;
constexpr uint32_t sub_1006A070 = 0x1006A070u;
constexpr uint32_t sub_1006A0E0 = 0x1006A0E0u;
constexpr uint32_t CMoDX_CreateTexture = 0x1006A150u;
constexpr uint32_t sub_1006A180 = 0x1006A180u;
constexpr uint32_t sub_1006A420 = 0x1006A420u;
constexpr uint32_t CMoDX_SetWorldMatrix = 0x1006A480u;
constexpr uint32_t CMoDX_GetWorldMatrix = 0x1006A4A0u;
constexpr uint32_t CMoDX_PushViewMatrix = 0x1006A4C0u;
constexpr uint32_t CMoDX_PopViewMatrix = 0x1006A4F0u;
constexpr uint32_t CMoDX_SetViewMatrix = 0x1006A520u;
constexpr uint32_t Gfx_CalcFVFStride = 0x1006A560u;
constexpr uint32_t sub_1006AD30 = 0x1006AD30u;
constexpr uint32_t fn_1006AD90 = 0x1006AD90u;
constexpr uint32_t sub_1006ADF0 = 0x1006ADF0u;
constexpr uint32_t StateBlock_ApplyRSTSS = 0x1006AE30u;
constexpr uint32_t sub_1006AF00 = 0x1006AF00u;
constexpr uint32_t StateBlock_ApplyTSS = 0x1006AF90u;
constexpr uint32_t CMoDxStatusMng_Constructor = 0x1006B050u;
constexpr uint32_t CMoDxStatusMng_Destructor = 0x1006B090u;
constexpr uint32_t VBPool_Constructor = 0x1006B0C0u;
constexpr uint32_t sub_1006B0E0 = 0x1006B0E0u;
constexpr uint32_t sub_1006B1D0 = 0x1006B1D0u;
constexpr uint32_t CMoTask_VT_10329C70_Destructor = 0x1006B250u;
constexpr uint32_t CMoDX_CreateAndInitSystem = 0x1006B2B0u;
constexpr uint32_t sub_1006BC10 = 0x1006BC10u;
constexpr uint32_t sub_1006BC80 = 0x1006BC80u;
constexpr uint32_t Rand_SeedMT19937 = 0x1006BD90u;
constexpr uint32_t sub_1006BF60 = 0x1006BF60u;
constexpr uint32_t Mem_AllocWithHeader12 = 0x1006C0F0u;
constexpr uint32_t Mem_FreeWithHeader12 = 0x1006C110u;
constexpr uint32_t CMoOcclusionMng_Constructor = 0x1006C140u;
constexpr uint32_t CMoOcclusionMng_Destructor = 0x1006C1E0u;
constexpr uint32_t j_OcclusionMng_CreateTarget = 0x1006C210u;
constexpr uint32_t OcclusionMng_CreateTarget = 0x1006C220u;
constexpr uint32_t OcclusionQuery_Init = 0x1006C260u;
constexpr uint32_t OcclusionQuery_Release = 0x1006C270u;
constexpr uint32_t Occlusion_QueryScreenRect = 0x1006C280u;
constexpr uint32_t Occlusion_QueryScreenRectImpl = 0x1006C300u;
constexpr uint32_t sub_1006C530 = 0x1006C530u;
constexpr uint32_t unknown_libname_59 = 0x1006C890u;
constexpr uint32_t unknown_libname_60 = 0x1006C8A0u;
constexpr uint32_t sub_1006C8B0 = 0x1006C8B0u;
constexpr uint32_t OcclusionMng_Prepare = 0x1006C9B0u;
constexpr uint32_t sub_1006CA20 = 0x1006CA20u;
constexpr uint32_t CreateProcessorInstance = 0x1006CB50u;
constexpr uint32_t nullsub_44 = 0x1006CF90u;
constexpr uint32_t CMoProcessor_VT_1032C368_vf012 = 0x1006D3B0u;
constexpr uint32_t CMoProcessor_Vec3Normalize = 0x1006D5D0u;
constexpr uint32_t CMoProcessor_Vec4TransformB = 0x1006D7E0u;
constexpr uint32_t CMoProcessor_Vec4Transform = 0x1006D870u;
constexpr uint32_t CMoProcessor_Vec3Transform = 0x1006D900u;
constexpr uint32_t CMoProcessor_VT_1032C368_vf021 = 0x1006D980u;
constexpr uint32_t CMoProcessor_ProjectPointsToScreen = 0x1006E7D0u;
constexpr uint32_t CMoProcessor_CalcScreenBounds = 0x1006E970u;
constexpr uint32_t CMoProcessor_ProjectVertsAndBounds = 0x1006EB70u;
constexpr uint32_t CMoProcessor_CalcScreenBoundsAndVisible = 0x1006ED10u;
constexpr uint32_t CMoTask_VT_1032C490_Destructor = 0x10070F80u;
constexpr uint32_t ResHandle_FreeIfUnreferenced = 0x10071050u;
constexpr uint32_t ResourceHandle_IsResident = 0x10071060u;
constexpr uint32_t sub_100710F0 = 0x100710F0u;
constexpr uint32_t CMoAttachments_VT_1032C4E8_ScalarDeletingDtor = 0x10071180u;
constexpr uint32_t nullsub_171 = 0x100719B0u;
constexpr uint32_t CMoAttachments_VT_1032C628_ScalarDeletingDtor = 0x10071C10u;
constexpr uint32_t CMoResource_VT_1032C588_ScalarDeletingDtor_10071C60 = 0x10071C60u;
constexpr uint32_t CMoResource_Destructor = 0x10071DE0u;
constexpr uint32_t CMoResource_AddRefChain = 0x10071E90u;
constexpr uint32_t CMoResource_ReleaseChain = 0x10071EC0u;
constexpr uint32_t sub_10071F50 = 0x10071F50u;
constexpr uint32_t CMoResource_RegisterInTypeList = 0x10072030u;
constexpr uint32_t CMoResource_Destroy = 0x100721F0u;
constexpr uint32_t CMoResource_GetParent = 0x10072270u;
constexpr uint32_t CMoResource_FindByTypeAndId = 0x10072290u;
constexpr uint32_t CMoResource_FindInScope = 0x10072430u;
constexpr uint32_t YmResourceFile_FindResourceUnder = 0x100724C0u;
constexpr uint32_t sub_100725C0 = 0x100725C0u;
constexpr uint32_t unknown_libname_61 = 0x100729C0u;
constexpr uint32_t CMoTask_VT_1032C908_Destructor = 0x100729E0u;
constexpr uint32_t sub_10072DB0 = 0x10072DB0u;
constexpr uint32_t MeshResourceMng_LoadSystemResources = 0x10072ED0u;
constexpr uint32_t ResourceHandle_Deref = 0x10072FB0u;
constexpr uint32_t sub_10072FD0 = 0x10072FD0u;
constexpr uint32_t sub_100730E0 = 0x100730E0u;
constexpr uint32_t sub_100731B0 = 0x100731B0u;
constexpr uint32_t MoRes_FreeIfUnreferenced = 0x10073B20u;
constexpr uint32_t MeshResourceMng_AllocHandle = 0x10074140u;
constexpr uint32_t NodePool_ReleaseNode = 0x10074180u;
constexpr uint32_t GetGlobal_1047D114 = 0x10074290u;
constexpr uint32_t ResourceIter_FindFirst = 0x100742B0u;
constexpr uint32_t ResourceIter_FindNext = 0x100745C0u;
constexpr uint32_t MoProcessor_CallVSlot49 = 0x100746A0u;
constexpr uint32_t CMoTask_Constructor = 0x10074740u;
constexpr uint32_t sub_10074790 = 0x10074790u;
constexpr uint32_t CMoTask_Destructor = 0x100747C0u;
constexpr uint32_t sub_100747D0 = 0x100747D0u;
constexpr uint32_t CMoTask_ResetFields = 0x100748F0u;
constexpr uint32_t sub_10074920 = 0x10074920u;
constexpr uint32_t CMoTask_RegisterWithMng = 0x100749B0u;
constexpr uint32_t CMoTaskMng_Forward10075120 = 0x10074A10u;
constexpr uint32_t sub_10074A30 = 0x10074A30u;
constexpr uint32_t CMoTaskMng_Destructor = 0x10074B60u;
constexpr uint32_t sub_10074BA0 = 0x10074BA0u;
constexpr uint32_t sub_10074C30 = 0x10074C30u;
constexpr uint32_t Gfx_InitCubeMapTextures = 0x10075280u;
constexpr uint32_t TexRef_ResolveByName = 0x10075520u;
constexpr uint32_t TexRef_EnsureRenderTarget = 0x100755A0u;
constexpr uint32_t sub_10075700 = 0x10075700u;
constexpr uint32_t unknown_libname_63 = 0x10075950u;
constexpr uint32_t TexRef_SetTexture = 0x10075960u;
constexpr uint32_t unknown_libname_64 = 0x10075C30u;
constexpr uint32_t ActorHandle_Constructor = 0x100814A0u;
constexpr uint32_t ActorHandle_Destructor = 0x100814E0u;
constexpr uint32_t sub_100814F0 = 0x100814F0u;
constexpr uint32_t ActorHandle_Resolve = 0x10081550u;
constexpr uint32_t sub_10081600 = 0x10081600u;
constexpr uint32_t Actor_ResetDrawLists = 0x10081A50u;
constexpr uint32_t Actor_UnlinkAllAttachLists = 0x10082B10u;
constexpr uint32_t CXiActor_NameCalc = 0x100830C0u;
constexpr uint32_t sub_10083220 = 0x10083220u;
constexpr uint32_t sub_10083230 = 0x10083230u;
constexpr uint32_t sub_10083340 = 0x10083340u;
constexpr uint32_t CXiActor_IsModelMissingOrFlag2000 = 0x10084140u;
constexpr uint32_t sub_10084160 = 0x10084160u;
constexpr uint32_t sub_100841A0 = 0x100841A0u;
constexpr uint32_t CXiActor_ModelTest95790 = 0x10084330u;
constexpr uint32_t CXiActor_ModelTest95680 = 0x10084350u;
constexpr uint32_t CXiActor_ModelTest956A0 = 0x10084370u;
constexpr uint32_t CXiActor_GetModelField170 = 0x10084390u;
constexpr uint32_t CXiActor_HasNonzeroField150or158 = 0x100843B0u;
constexpr uint32_t CXiActor_GetModelByteEE = 0x10084400u;
constexpr uint32_t CXiActor_GetModelByteEF = 0x10084420u;
constexpr uint32_t CXiActor_GetModelPartWord = 0x10084480u;
constexpr uint32_t CXiActor_GetModelFlags120Top2 = 0x10084500u;
constexpr uint32_t CXiActor_GetModelFlags130Top2 = 0x10084520u;
constexpr uint32_t CXiActor_GetModelFloat9AEC0 = 0x10084540u;
constexpr uint32_t CXiActor_IsModelMissingOrSuppressed = 0x10084580u;
constexpr uint32_t CXiActor_TestModelFlag128_8 = 0x100845B0u;
constexpr uint32_t sub_100845E0 = 0x100845E0u;
constexpr uint32_t CXiActor_TestModelFlags100000_200000_4000 = 0x10084640u;
constexpr uint32_t sub_10084670 = 0x10084670u;
constexpr uint32_t sub_10084690 = 0x10084690u;
constexpr uint32_t sub_100846D0 = 0x100846D0u;
constexpr uint32_t sub_100846F0 = 0x100846F0u;
constexpr uint32_t CXiActor_GetModelByte1DB = 0x10084720u;
constexpr uint32_t sub_10085000 = 0x10085000u;
constexpr uint32_t CXiActor_SpawnResourceGenerators = 0x10085060u;
constexpr uint32_t CXiActor_IsInModelSlotTable = 0x10085490u;
constexpr uint32_t CXiActor_KillGeneratorClones = 0x10085570u;
constexpr uint32_t sub_10085BF0 = 0x10085BF0u;
constexpr uint32_t sub_10085E40 = 0x10085E40u;
constexpr uint32_t Actor_DrawWorldText = 0x10086210u;
constexpr uint32_t sub_10087060 = 0x10087060u;
constexpr uint32_t Call1008A0E0With2 = 0x1008A0D0u;
constexpr uint32_t sub_1008E130 = 0x1008E130u;
constexpr uint32_t Unit10086E90_ResetGlobals = 0x10093720u;
constexpr uint32_t Net_SendPacket00COnce = 0x10093DB0u;
constexpr uint32_t Entity_BuildDisplayName = 0x10097840u;
constexpr uint32_t Entity_GetIndexedPtr = 0x10097A40u;
constexpr uint32_t Entity_IsModelIdB22toB31 = 0x10098320u;
constexpr uint32_t CMoTask_VT_1032D6D4_Destructor = 0x100A2810u;
constexpr uint32_t CXiCollisionActor_Constructor = 0x100A4C20u;
constexpr uint32_t CXiCollisionActor_vf8 = 0x100A4ED0u;
constexpr uint32_t sub_100A57C0 = 0x100A57C0u;
constexpr uint32_t sub_100A5E10 = 0x100A5E10u;
constexpr uint32_t sub_100A6070 = 0x100A6070u;
constexpr uint32_t CXiControlActor_Destructor = 0x100A8CA0u;
constexpr uint32_t sub_100A8E30 = 0x100A8E30u;
constexpr uint32_t CMoTask_VT_1032E808_Destructor = 0x100A9D30u;
constexpr uint32_t CMoTask_VT_1032E808_ScalarDeletingDtor = 0x100A9DB0u;
constexpr uint32_t sub_100AA380 = 0x100AA380u;
constexpr uint32_t sub_100AA3B0 = 0x100AA3B0u;
constexpr uint32_t sub_100AA8E0 = 0x100AA8E0u;
constexpr uint32_t sub_100AA940 = 0x100AA940u;
constexpr uint32_t sub_100AAD40 = 0x100AAD40u;
constexpr uint32_t sub_100ABEA0 = 0x100ABEA0u;
constexpr uint32_t sub_100AC290 = 0x100AC290u;
constexpr uint32_t sub_100ADEE0 = 0x100ADEE0u;
constexpr uint32_t sub_100C3BC0 = 0x100C3BC0u;
constexpr uint32_t sub_100C3BD0 = 0x100C3BD0u;
constexpr uint32_t sub_100C3CE0 = 0x100C3CE0u;
constexpr uint32_t sub_100C51C0 = 0x100C51C0u;
constexpr uint32_t TargetActor_SlotHandler1 = 0x100C5410u;
constexpr uint32_t TargetActor_SlotHandler0 = 0x100C5440u;
constexpr uint32_t TargetActor_SlotHandler4 = 0x100C54C0u;
constexpr uint32_t CXiSkeletonActor_DestructorBody = 0x100C5C00u;
constexpr uint32_t CXiSkeletonActor_Init = 0x100C5D60u;
constexpr uint32_t sub_100C8280 = 0x100C8280u;
constexpr uint32_t sub_100C83C0 = 0x100C83C0u;
constexpr uint32_t sub_100C8BB0 = 0x100C8BB0u;
constexpr uint32_t sub_100C8ED0 = 0x100C8ED0u;
constexpr uint32_t sub_100C9330 = 0x100C9330u;
constexpr uint32_t sub_100C9810 = 0x100C9810u;
constexpr uint32_t sub_100C9C10 = 0x100C9C10u;
constexpr uint32_t sub_100CA310 = 0x100CA310u;
constexpr uint32_t sub_100CA730 = 0x100CA730u;
constexpr uint32_t sub_100CA8C0 = 0x100CA8C0u;
constexpr uint32_t sub_100CAA60 = 0x100CAA60u;
constexpr uint32_t sub_100CAF90 = 0x100CAF90u;
constexpr uint32_t sub_100CB390 = 0x100CB390u;
constexpr uint32_t sub_100CB400 = 0x100CB400u;
constexpr uint32_t Actor_SelectNearestLights = 0x100CB4E0u;
constexpr uint32_t sub_100CBD90 = 0x100CBD90u;
constexpr uint32_t PerActor_FourBucketStaggeredUpdate = 0x100CBDE0u;
constexpr uint32_t nullsub_276 = 0x100CCE20u;
constexpr uint32_t nullsub_277 = 0x100CCE30u;
constexpr uint32_t nullsub_278 = 0x100CCE40u;
constexpr uint32_t nullsub_279 = 0x100CCE50u;
constexpr uint32_t Actor_SetCameraFromModelPoints = 0x100CCE60u;
constexpr uint32_t sub_100CD040 = 0x100CD040u;
constexpr uint32_t sub_100CD4E0 = 0x100CD4E0u;
constexpr uint32_t sub_100CD920 = 0x100CD920u;
constexpr uint32_t sub_100CDD60 = 0x100CDD60u;
constexpr uint32_t sub_100CDE70 = 0x100CDE70u;
constexpr uint32_t sub_100CDF00 = 0x100CDF00u;
constexpr uint32_t sub_100CDF90 = 0x100CDF90u;
constexpr uint32_t sub_100CE180 = 0x100CE180u;
constexpr uint32_t sub_100CE250 = 0x100CE250u;
constexpr uint32_t CXiSkeletonActor_ReverseFindRes = 0x100CE480u;
constexpr uint32_t CXiSkeletonActor_DrawNameIfVisible = 0x100D0880u;
constexpr uint32_t CXiSkeletonActor_DrawName2IfVisible = 0x100D08B0u;
constexpr uint32_t sub_100D0B50 = 0x100D0B50u;
constexpr uint32_t sub_100D0D40 = 0x100D0D40u;
constexpr uint32_t sub_100D0D70 = 0x100D0D70u;
constexpr uint32_t sub_100D0E80 = 0x100D0E80u;
constexpr uint32_t sub_100D1070 = 0x100D1070u;
constexpr uint32_t sub_100D1190 = 0x100D1190u;
constexpr uint32_t CXiSkeletonActor_IsReadCompleteBody = 0x100D12D0u;
constexpr uint32_t CXiSkeletonActor_OnSkeletonLoaded = 0x100D2F40u;
constexpr uint32_t CXiSkeletonActor_LoadSkeletonSync = 0x100D2F60u;
constexpr uint32_t sub_100D4100 = 0x100D4100u;
constexpr uint32_t sub_100D4220 = 0x100D4220u;
constexpr uint32_t CXiSkeletonActor_LoadSkeletonResource = 0x100D4330u;
constexpr uint32_t sub_100D43E0 = 0x100D43E0u;
constexpr uint32_t CXiSkeletonActor_DeleteOwnedList = 0x100D44F0u;
constexpr uint32_t sub_100D4BC0 = 0x100D4BC0u;
constexpr uint32_t sub_100D5430 = 0x100D5430u;
constexpr uint32_t sub_100D5490 = 0x100D5490u;
constexpr uint32_t CXiSkeletonActor_CalcGroundTiltMatrix = 0x100D54C0u;
constexpr uint32_t CXiSkeletonActor_UpdateHeadLook = 0x100D5B10u;
constexpr uint32_t sub_100D5F10 = 0x100D5F10u;
constexpr uint32_t sub_100D60E0 = 0x100D60E0u;
constexpr uint32_t sub_100D6240 = 0x100D6240u;
constexpr uint32_t sub_100D6850 = 0x100D6850u;
constexpr uint32_t Actor_PrepareDrawGlobals = 0x100D6D30u;
constexpr uint32_t CXiSkeletonActorRes_Constructor = 0x100D7020u;
constexpr uint32_t CXiSkeletonActorRes_Destructor = 0x100D7050u;
constexpr uint32_t CXiSkeletonActorRes_GetLinkedActor = 0x100D7090u;
constexpr uint32_t sub_100DA6A0 = 0x100DA6A0u;
constexpr uint32_t GameMode_GetField98 = 0x100DB100u;
constexpr uint32_t GameMode_PollPendingState = 0x100DB140u;
constexpr uint32_t GameMode_RequestState = 0x100DB170u;
constexpr uint32_t sub_100DB790 = 0x100DB790u;
constexpr uint32_t GameMode_OpenTacticsUrl = 0x100DBB50u;
constexpr uint32_t sub_100DBE50 = 0x100DBE50u;
constexpr uint32_t sub_100FD3A0 = 0x100FD3A0u;
constexpr uint32_t sub_101037F0 = 0x101037F0u;
constexpr uint32_t ntGameTimeGet = 0x101039E0u;
constexpr uint32_t sub_10118260 = 0x10118260u;
constexpr uint32_t nullsub_48 = 0x10118520u;
constexpr uint32_t sub_10118C10 = 0x10118C10u;
constexpr uint32_t sub_10118CB0 = 0x10118CB0u;
constexpr uint32_t sub_10118DB0 = 0x10118DB0u;
constexpr uint32_t sub_1011AC70 = 0x1011AC70u;
constexpr uint32_t sub_1011BA90 = 0x1011BA90u;
constexpr uint32_t Font_BindGlyphTexture = 0x1011BAE0u;
constexpr uint32_t sub_1011BBA0 = 0x1011BBA0u;
constexpr uint32_t Gfx_SetAdditiveBlend = 0x1011BDC0u;
constexpr uint32_t Font_DrawChar = 0x1011BF40u;
constexpr uint32_t Gfx_SetCellBlendMode = 0x1011C1F0u;
constexpr uint32_t Font_AllocHighResImage = 0x1011C5D0u;
constexpr uint32_t Tex_LoadSurfaceFromDatFile = 0x1011C670u;
constexpr uint32_t Font_LoadHighResTexture = 0x1011C720u;
constexpr uint32_t Font_ReleaseHighResTexture = 0x1011C810u;
constexpr uint32_t Font_IsHighResTextureLoaded = 0x1011C850u;
constexpr uint32_t unknown_libname_88 = 0x1011C870u;
constexpr uint32_t sub_1011C8B0 = 0x1011C8B0u;
constexpr uint32_t Font_DecodeNextChar = 0x1011CE80u;
constexpr uint32_t sub_1011D050 = 0x1011D050u;
constexpr uint32_t Font_AddGlyphAdvance = 0x1011D840u;
constexpr uint32_t Sjis_GetCharByteLen = 0x1011DA80u;
constexpr uint32_t Cell_DrawWithDefaultBlend = 0x1011F2D0u;
constexpr uint32_t Cell_DrawQuadBlended = 0x1011F300u;
constexpr uint32_t sub_1011FB60 = 0x1011FB60u;
constexpr uint32_t Shape_DrawScaled = 0x1011FC80u;
constexpr uint32_t Shape_DrawTransformed = 0x1011FF70u;
constexpr uint32_t Color_ModulateArgb128 = 0x101200B0u;
constexpr uint32_t ShapeMng_Init = 0x101201C0u;
constexpr uint32_t sub_10120200 = 0x10120200u;
constexpr uint32_t ShapeMng_FindShape = 0x10120450u;
constexpr uint32_t ShapeMng_GetFontGlyph = 0x10120510u;
constexpr uint32_t ShapeMng_BindFontTexture = 0x10120590u;
constexpr uint32_t ShapeMng_LoadFontTextures = 0x10120660u;
constexpr uint32_t ShapeMng_GetDmgNumGlyph = 0x10120730u;
constexpr uint32_t ShapeMng_BindDmgNumTexture = 0x101207B0u;
constexpr uint32_t ShapeMng_LoadDmgNumTextures = 0x10120880u;
constexpr uint32_t sub_10123A70 = 0x10123A70u;
constexpr uint32_t sub_10123E90 = 0x10123E90u;
constexpr uint32_t Cursor_GetHwCursorSlot = 0x101245C0u;
constexpr uint32_t Cursor_SetPosition = 0x10124700u;
constexpr uint32_t sub_101248A0 = 0x101248A0u;
constexpr uint32_t sub_10125040 = 0x10125040u;
constexpr uint32_t sub_10125170 = 0x10125170u;
constexpr uint32_t sub_101252C0 = 0x101252C0u;
constexpr uint32_t Cursor_SetVisible = 0x101252F0u;
constexpr uint32_t sub_10125FE0 = 0x10125FE0u;
constexpr uint32_t sub_101260E0 = 0x101260E0u;
constexpr uint32_t sub_101260F0 = 0x101260F0u;
constexpr uint32_t sub_10126100 = 0x10126100u;
constexpr uint32_t sub_10126270 = 0x10126270u;
constexpr uint32_t unknown_libname_89 = 0x10126290u;
constexpr uint32_t Cursor_StoreScreenCenter = 0x101262A0u;
constexpr uint32_t sub_10126510 = 0x10126510u;
constexpr uint32_t sub_10127190 = 0x10127190u;
constexpr uint32_t ChatLog_AddMessage = 0x10127C00u;
constexpr uint32_t sub_10127EE0 = 0x10127EE0u;
constexpr uint32_t sub_1013A320 = 0x1013A320u;
constexpr uint32_t sub_10150CC0 = 0x10150CC0u;
constexpr uint32_t sub_10152F80 = 0x10152F80u;
constexpr uint32_t sub_10153C10 = 0x10153C10u;
constexpr uint32_t sub_10153FF0 = 0x10153FF0u;
constexpr uint32_t sub_10154860 = 0x10154860u;
constexpr uint32_t sub_10157660 = 0x10157660u;
constexpr uint32_t sub_10157720 = 0x10157720u;
constexpr uint32_t sub_101579C0 = 0x101579C0u;
constexpr uint32_t sub_10157CF0 = 0x10157CF0u;
constexpr uint32_t Actor_SetSlotTableEntry = 0x10159400u;
constexpr uint32_t sub_101598A0 = 0x101598A0u;
constexpr uint32_t MenuMng_Init = 0x1015D900u;
constexpr uint32_t sub_1015D9E0 = 0x1015D9E0u;
constexpr uint32_t sub_1015DCE0 = 0x1015DCE0u;
constexpr uint32_t sub_1015DD00 = 0x1015DD00u;
constexpr uint32_t nullsub_49 = 0x1015DFC0u;
constexpr uint32_t MenuMng_OpenByName = 0x1015E350u;
constexpr uint32_t sub_1015EDB0 = 0x1015EDB0u;
constexpr uint32_t sub_1015EE20 = 0x1015EE20u;
constexpr uint32_t sub_1015F6A0 = 0x1015F6A0u;
constexpr uint32_t sub_1015FA00 = 0x1015FA00u;
constexpr uint32_t MenuMng_GetUiScale = 0x10160250u;
constexpr uint32_t MenuMng_GetOffsetX = 0x10160290u;
constexpr uint32_t MenuMng_GetOffsetY = 0x101602C0u;
constexpr uint32_t sub_10160440 = 0x10160440u;
constexpr uint32_t Gfx_SetDefault2DStates = 0x101604B0u;
constexpr uint32_t Gfx_DisableFogAndAlphaBlend = 0x10160650u;
constexpr uint32_t sub_101606F0 = 0x101606F0u;
constexpr uint32_t sub_10160700 = 0x10160700u;
constexpr uint32_t sub_10160C90 = 0x10160C90u;
constexpr uint32_t Obj_10621944_DrawBgRect = 0x10163390u;
constexpr uint32_t GameSystems_InitializeAll = 0x10163640u;
constexpr uint32_t Game_DestroyGlobalObjects = 0x10164B80u;
constexpr uint32_t sub_10165070 = 0x10165070u;
constexpr uint32_t sub_101671D0 = 0x101671D0u;
constexpr uint32_t Vt10336614_Destructor = 0x101674B0u;
constexpr uint32_t Vt10336634_Destructor = 0x101678D0u;
constexpr uint32_t GetPtr8IfField60Set = 0x10167940u;
constexpr uint32_t Dbg_DrawWireMesh = 0x10168D20u;
constexpr uint32_t Mat44_Multiply = 0x1016C060u;
constexpr uint32_t Vt10336668_Destructor = 0x1016FC80u;
constexpr uint32_t sub_10170B30 = 0x10170B30u;
constexpr uint32_t sub_10170C80 = 0x10170C80u;
constexpr uint32_t sub_10170E80 = 0x10170E80u;
constexpr uint32_t Vt1033668C_Destructor = 0x10171920u;
constexpr uint32_t Mat4_SetIdentity_2 = 0x10175740u;
constexpr uint32_t Mat4_MulTranslation = 0x10175780u;
constexpr uint32_t CMoResource_VT_10336698_Destructor = 0x10176EC0u;
constexpr uint32_t CMoResource_VT_103366C4_Destructor = 0x10176FA0u;
constexpr uint32_t CMoResource_VT_103366F0_Destructor = 0x10177020u;
constexpr uint32_t Gfx_ShaderSrc_GetText = 0x10177340u;
constexpr uint32_t Gfx_ShaderSrc_GetTextLength = 0x10177350u;
constexpr uint32_t sub_101773F0 = 0x101773F0u;
constexpr uint32_t Gfx_SetZBias = 0x10177400u;
constexpr uint32_t ZoneMap_SetOverlayProjection = 0x10177420u;
constexpr uint32_t sub_10177D00 = 0x10177D00u;
constexpr uint32_t sub_10177FA0 = 0x10177FA0u;
constexpr uint32_t ZoneMap_FindLight = 0x10178760u;
constexpr uint32_t ZoneMap_ReleaseStateBlock = 0x10178B00u;
constexpr uint32_t CTsZoneMap_UpdateViewerPosition = 0x10179390u;
constexpr uint32_t ZoneMap_CopyField342C8ToField39450 = 0x10179410u;
constexpr uint32_t sub_1017A3F0 = 0x1017A3F0u;
constexpr uint32_t ZoneMap_DrawMeshGroupsWithFog = 0x1017ADE0u;
constexpr uint32_t ZoneMap_TeardownStep = 0x1017B1E0u;
constexpr uint32_t sub_1017C8B0 = 0x1017C8B0u;
constexpr uint32_t sub_1017C9C0 = 0x1017C9C0u;
constexpr uint32_t sub_1017DE20 = 0x1017DE20u;
constexpr uint32_t ZoneMap_FindGroundHeight = 0x101813F0u;
constexpr uint32_t sub_10181BB0 = 0x10181BB0u;
constexpr uint32_t sub_10182070 = 0x10182070u;
constexpr uint32_t sub_10182490 = 0x10182490u;
constexpr uint32_t CTsZoneMap_DrawProjectedTexture = 0x10182700u;
constexpr uint32_t sub_10183490 = 0x10183490u;
constexpr uint32_t sub_10183760 = 0x10183760u;
constexpr uint32_t ZoneMap_DrawMeshById = 0x10183DA0u;
constexpr uint32_t sub_10186E90 = 0x10186E90u;
constexpr uint32_t sub_10186EB0 = 0x10186EB0u;
constexpr uint32_t Env_GetColorPtr84 = 0x10186F20u;
constexpr uint32_t Env_GetAmbientColorPtr = 0x10186FA0u;
constexpr uint32_t sub_10186FE0 = 0x10186FE0u;
constexpr uint32_t sub_10186FF0 = 0x10186FF0u;
constexpr uint32_t sub_10187000 = 0x10187000u;
constexpr uint32_t sub_10187010 = 0x10187010u;
constexpr uint32_t sub_101874F0 = 0x101874F0u;
constexpr uint32_t sub_101880B0 = 0x101880B0u;
constexpr uint32_t XiZone_GetDirLights = 0x10188B70u;
constexpr uint32_t sub_10188CA0 = 0x10188CA0u;
constexpr uint32_t Env_ComputeAmbient = 0x10188E00u;
constexpr uint32_t Env_ComputeAmbientFloat4 = 0x10188FA0u;
constexpr uint32_t sub_10189120 = 0x10189120u;
constexpr uint32_t sub_101892B0 = 0x101892B0u;
constexpr uint32_t sub_1018B010 = 0x1018B010u;
constexpr uint32_t sub_1018B130 = 0x1018B130u;
constexpr uint32_t sub_1018B3A0 = 0x1018B3A0u;
constexpr uint32_t sub_1018B860 = 0x1018B860u;
constexpr uint32_t sub_1018B8F0 = 0x1018B8F0u;
constexpr uint32_t sub_1018B960 = 0x1018B960u;
constexpr uint32_t XiZone_GetDirLight = 0x1018B9A0u;
constexpr uint32_t XiZone_GetAmbient = 0x1018B9E0u;
constexpr uint32_t XiZone_GetFog = 0x1018BA20u;
constexpr uint32_t sub_1018BAE0 = 0x1018BAE0u;
constexpr uint32_t sub_1018BB10 = 0x1018BB10u;
constexpr uint32_t sub_1018BB20 = 0x1018BB20u;
constexpr uint32_t unknown_libname_158 = 0x1018BBB0u;
constexpr uint32_t sub_1018BBC0 = 0x1018BBC0u;
constexpr uint32_t sub_1018BBE0 = 0x1018BBE0u;
constexpr uint32_t sub_1018BC40 = 0x1018BC40u;
constexpr uint32_t sub_1018C210 = 0x1018C210u;
constexpr uint32_t sub_1018C2B0 = 0x1018C2B0u;
constexpr uint32_t CXiSchStatus_Destructor = 0x1018C620u;
constexpr uint32_t sub_1018C810 = 0x1018C810u;
constexpr uint32_t sub_1018C870 = 0x1018C870u;
constexpr uint32_t sub_1018C980 = 0x1018C980u;
constexpr uint32_t Sch_GetStatusEntry = 0x1018CA20u;
constexpr uint32_t sub_1018CE10 = 0x1018CE10u;
constexpr uint32_t sub_1018CEF0 = 0x1018CEF0u;
constexpr uint32_t Settings_GetValue = 0x10193850u;
constexpr uint32_t sub_10193890 = 0x10193890u;
constexpr uint32_t sub_1019C3E0 = 0x1019C3E0u;
constexpr uint32_t sub_1019C5B0 = 0x1019C5B0u;
constexpr uint32_t sub_1019EBB0 = 0x1019EBB0u;
constexpr uint32_t sub_101A2E80 = 0x101A2E80u;
constexpr uint32_t sub_101A8B50 = 0x101A8B50u;
constexpr uint32_t sub_101A8D40 = 0x101A8D40u;
constexpr uint32_t sub_101A8D60 = 0x101A8D60u;
constexpr uint32_t sub_101AB080 = 0x101AB080u;
constexpr uint32_t sub_101AB0D0 = 0x101AB0D0u;
constexpr uint32_t sub_101AB6D0 = 0x101AB6D0u;
constexpr uint32_t sub_101AB720 = 0x101AB720u;
constexpr uint32_t sub_101AB770 = 0x101AB770u;
constexpr uint32_t sub_101AB820 = 0x101AB820u;
constexpr uint32_t sub_101AE3E0 = 0x101AE3E0u;
constexpr uint32_t sub_101B8E70 = 0x101B8E70u;
constexpr uint32_t sub_101BCE30 = 0x101BCE30u;
constexpr uint32_t sub_101C46A0 = 0x101C46A0u;
constexpr uint32_t FileMng_ResolveOrExit = 0x101C4AF0u;
constexpr uint32_t sub_101C4CC0 = 0x101C4CC0u;
constexpr uint32_t sub_101C4CD0 = 0x101C4CD0u;
constexpr uint32_t sub_101C4CE0 = 0x101C4CE0u;
constexpr uint32_t sub_101C4D10 = 0x101C4D10u;
constexpr uint32_t sub_101C9B50 = 0x101C9B50u;
constexpr uint32_t sub_101CC4F0 = 0x101CC4F0u;
constexpr uint32_t sub_101D0B00 = 0x101D0B00u;
constexpr uint32_t sub_101D0C40 = 0x101D0C40u;
constexpr uint32_t sub_101D0C70 = 0x101D0C70u;
constexpr uint32_t YkInit = 0x101D11C0u;
constexpr uint32_t sub_101D5680 = 0x101D5680u;
constexpr uint32_t sub_101EE7B0 = 0x101EE7B0u;
constexpr uint32_t sub_1021BB90 = 0x1021BB90u;
constexpr uint32_t CXiMovie_DrawFrame = 0x1021D4C0u;
constexpr uint32_t Movie_Update = 0x1021D980u;
constexpr uint32_t Movie_GetCurrentTexture = 0x1021DDF0u;
constexpr uint32_t sub_10220EF0 = 0x10220EF0u;
constexpr uint32_t sub_10220F40 = 0x10220F40u;
constexpr uint32_t sub_10221770 = 0x10221770u;
constexpr uint32_t sub_102217A0 = 0x102217A0u;
constexpr uint32_t sub_102347D0 = 0x102347D0u;
constexpr uint32_t sub_10234A00 = 0x10234A00u;
constexpr uint32_t Ui_KeyPrompt_Init = 0x10239460u;
constexpr uint32_t LobbyThread_Start = 0x10245600u;
constexpr uint32_t sub_10245B50 = 0x10245B50u;
constexpr uint32_t Lobby_DestroyThreadPool = 0x10245B60u;
constexpr uint32_t thunk_sub_10245BB0 = 0x10245BA0u;
constexpr uint32_t sub_1024D130 = 0x1024D130u;
constexpr uint32_t StAvatar_ReleaseResources = 0x1024D2C0u;
constexpr uint32_t StAvatar_StopAndFreeBuffers = 0x1024D790u;
constexpr uint32_t sub_102502D0 = 0x102502D0u;
constexpr uint32_t sub_10250D80 = 0x10250D80u;
constexpr uint32_t StDancer_Update = 0x10251500u;
constexpr uint32_t sub_102515F0 = 0x102515F0u;
constexpr uint32_t sub_10251960 = 0x10251960u;
constexpr uint32_t sub_102524B0 = 0x102524B0u;
constexpr uint32_t sub_10252C80 = 0x10252C80u;
constexpr uint32_t sub_102532E0 = 0x102532E0u;
constexpr uint32_t sub_10253670 = 0x10253670u;
constexpr uint32_t sub_1025B560 = 0x1025B560u;
constexpr uint32_t sub_1025E050 = 0x1025E050u;
constexpr uint32_t sub_1025E0B0 = 0x1025E0B0u;
constexpr uint32_t sub_1025E0C0 = 0x1025E0C0u;
constexpr uint32_t sub_1025E0E0 = 0x1025E0E0u;
constexpr uint32_t sub_1025E0F0 = 0x1025E0F0u;
constexpr uint32_t sub_1026E400 = 0x1026E400u;
constexpr uint32_t sub_1026E460 = 0x1026E460u;
constexpr uint32_t mdl_1026E900 = 0x1026E900u;
constexpr uint32_t sub_10270B60 = 0x10270B60u;
constexpr uint32_t sub_102717F0 = 0x102717F0u;
constexpr uint32_t scnFini = 0x10271B40u;
constexpr uint32_t sub_10271C60 = 0x10271C60u;
constexpr uint32_t sub_10271C80 = 0x10271C80u;
constexpr uint32_t scnGetFlags = 0x10271E90u;
constexpr uint32_t scnCameraGetInfo = 0x10273370u;
constexpr uint32_t sub_102738D0 = 0x102738D0u;
constexpr uint32_t chasm_10273920 = 0x10273920u;
constexpr uint32_t PackVec4 = 0x10274D40u;
constexpr uint32_t sub_10275090 = 0x10275090u;
constexpr uint32_t sqMemInit = 0x10275150u;
constexpr uint32_t sqMemPoolRegister = 0x10275B60u;
constexpr uint32_t sqFree = 0x102760F0u;
constexpr uint32_t sqgxContext_Create = 0x10276260u;
constexpr uint32_t M4fSet = 0x1027EB10u;
constexpr uint32_t M4fTransform = 0x1027EF20u;
constexpr uint32_t M4fTranspose = 0x1027F070u;
constexpr uint32_t sqMatrix4_1027F0E0 = 0x1027F0E0u;
constexpr uint32_t sqMatrix4_1027FCC0 = 0x1027FCC0u;
constexpr uint32_t sqMatrix4_10280140 = 0x10280140u;
constexpr uint32_t sqgxSprite = 0x10280DE0u;
constexpr uint32_t ___setargv_3 = 0x10281500u;
constexpr uint32_t sqgxTextureCreateFromImage = 0x10281510u;
constexpr uint32_t sqgxTextureAddZ = 0x102816E0u;
constexpr uint32_t sqgxCeilLog2 = 0x10281810u;
constexpr uint32_t sqgxTextureAdd = 0x10281830u;
constexpr uint32_t sqgxTextureGetSize = 0x10281B60u;
constexpr uint32_t sqgxTextureSet = 0x10281C20u;
constexpr uint32_t sqgxTextureDecal = 0x10281C50u;
constexpr uint32_t sqgxTextureClamp = 0x10281C80u;
constexpr uint32_t sqgxTextureUpdate = 0x10281DB0u;
constexpr uint32_t sqgxTextureStencilClear = 0x10282010u;
constexpr uint32_t sqgxTextureClear = 0x10282030u;
constexpr uint32_t sqgxTextureDraw = 0x10282190u;
constexpr uint32_t sqgxZTest = 0x10282350u;
constexpr uint32_t sqgxAlphaDstTest = 0x102823B0u;
constexpr uint32_t sqgxZMask = 0x10282470u;
constexpr uint32_t sqgxFBMask = 0x102824C0u;
constexpr uint32_t nullsub_59 = 0x10282630u;
constexpr uint32_t sqgxBlendLin = 0x10282A30u;
constexpr uint32_t sqgxBlendTrsp = 0x10282B90u;
constexpr uint32_t sqgxBlendMul = 0x10283110u;
constexpr uint32_t sqgxBlendOff = 0x10283220u;
constexpr uint32_t nullsub_31 = 0x10283240u;
constexpr uint32_t ___setargv_9 = 0x10283250u;
constexpr uint32_t sqgxColor = 0x10283330u;
constexpr uint32_t sqgxAmbient = 0x10283380u;
constexpr uint32_t sqgxColorUpdate = 0x10283420u;
constexpr uint32_t ___setargv_16 = 0x10283870u;
constexpr uint32_t sqgxPixelSize = 0x102838A0u;
constexpr uint32_t sqgxViewport = 0x10283A10u;
constexpr uint32_t sqgxMatrixCamera = 0x10283D60u;
constexpr uint32_t sqgxMatrixShape = 0x10283E00u;
constexpr uint32_t sqgxPassAdd = 0x10283F70u;
constexpr uint32_t sqgxMatrixUpdate = 0x10284410u;
constexpr uint32_t ___setargv_24 = 0x10284890u;
constexpr uint32_t sqgxVertexShaderCreate = 0x10284950u;
constexpr uint32_t sqgxShader = 0x10284B20u;
constexpr uint32_t nullsub_32 = 0x10284EB0u;
constexpr uint32_t j_sqgxVertexShaderSelect = 0x10284EC0u;
constexpr uint32_t sqDrawableDraw = 0x10285850u;
constexpr uint32_t sqDebugPrintf = 0x10285890u;
constexpr uint32_t sub_1028A730 = 0x1028A730u;
constexpr uint32_t sub_1028A770 = 0x1028A770u;
constexpr uint32_t sub_1028A790 = 0x1028A790u;
constexpr uint32_t sqgxShapeBegin = 0x1028AFE0u;
constexpr uint32_t sub_1028BB10 = 0x1028BB10u;
constexpr uint32_t sub_1028BB70 = 0x1028BB70u;
constexpr uint32_t sub_1028BBD0 = 0x1028BBD0u;
constexpr uint32_t sqsnScene_1028F5F0 = 0x1028F5F0u;
constexpr uint32_t sqsnSceneLayerDraw = 0x1028FDD0u;
constexpr uint32_t sqsnSceneLayerDrawShadowed = 0x1028FFF0u;
constexpr uint32_t sqsnSceneLayerSnapShotComposite = 0x102902C0u;
constexpr uint32_t nullsub_60 = 0x10290430u;
constexpr uint32_t sqsnSceneLayerSort = 0x10290440u;
constexpr uint32_t sqrdCamera_Apply = 0x102911E0u;
constexpr uint32_t sub_10297320 = 0x10297320u;
constexpr uint32_t sqsnShadowVolumeExtrudeTriList = 0x102979E0u;
constexpr uint32_t sqsnShadowVolumeExtrudeTriStrip = 0x10297B10u;
constexpr uint32_t sqsnShadowVolumeSetPass = 0x10297CC0u;
constexpr uint32_t sqsnShadowComputeFacing = 0x10297F90u;
constexpr uint32_t sqsnShadowVolumeBeginVB = 0x102981E0u;
constexpr uint32_t sqgxShadowPlaneMatrix = 0x10298350u;
constexpr uint32_t sub_1029BD50 = 0x1029BD50u;
constexpr uint32_t sub_1029BF50 = 0x1029BF50u;
constexpr uint32_t sub_1029C480 = 0x1029C480u;
constexpr uint32_t sub_1029C610 = 0x1029C610u;
constexpr uint32_t sub_1029C950 = 0x1029C950u;
constexpr uint32_t Direct3DCreate8 = 0x102D2600u;
constexpr uint32_t fn_102D2868 = 0x102D2868u;
constexpr uint32_t fn_102D2F40 = 0x102D2F40u;
constexpr uint32_t fn_102D2FB9 = 0x102D2FB9u;
constexpr uint32_t fn_102D301F = 0x102D301Fu;
constexpr uint32_t fn_102D4515 = 0x102D4515u;
constexpr uint32_t D3DXMatrixMultiply_Dispatch = 0x102D6F03u;
constexpr uint32_t D3DXMatrixTranspose_Dispatch = 0x102D714Cu;
constexpr uint32_t fn_102D7FF9 = 0x102D7FF9u;
constexpr uint32_t D3DXMatrixPerspectiveFovLH_static = 0x102D83C6u;
constexpr uint32_t fn_102DA79B = 0x102DA79Bu;
constexpr uint32_t fn_102DAA0D = 0x102DAA0Du;
constexpr uint32_t fn_102DAA71 = 0x102DAA71u;
constexpr uint32_t Thunk_FnTable694 = 0x1031077Eu;
constexpr uint32_t operator_delete = 0x10311BB0u;
constexpr uint32_t Allocator_Generic = 0x10311BBBu;
constexpr uint32_t _sprintf = 0x10311FF1u;
constexpr uint32_t _strncpy = 0x10312050u;
constexpr uint32_t _strrchr = 0x103127C0u;
constexpr uint32_t _malloc_0 = 0x10312F96u;
constexpr uint32_t _atexit = 0x10313486u;
constexpr uint32_t __snprintf = 0x103134C7u;
constexpr uint32_t __strlwr = 0x10315AA8u;
}  // namespace orig_addr
#define XI_CODE(name) reinterpret_cast<void *>(XI_GAME(orig_addr::name))
