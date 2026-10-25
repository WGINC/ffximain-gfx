// gfx_orig.hpp -- calls into the game's own code (not reconstructed here), by address.
#pragma once
#include "gfx_common.hpp"
#define XI_ORIG static inline __attribute__((always_inline))
namespace orig {
XI_ORIG void FUN_10001070(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10001070u))(a0, a1, a2, a3); }
XI_ORIG int CApp_Constructor(int a0, int a1, HINSTANCE a2, int a3, void * a4, void * a5) { return reinterpret_cast<int (__thiscall *)(int, int, HINSTANCE, int, void *, void *)>(XI_GAME(0x10001240u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int App_ForceForegroundWindow(HWND a0) { return reinterpret_cast<int (__cdecl *)(HWND)>(XI_GAME(0x100014A0u))(a0); }
XI_ORIG void CApp_RunMainLoop(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10001520u))(a0); }
XI_ORIG char App_PreTranslateInputMessage(int a0, unsigned int a1, unsigned int a2, unsigned int a3) { return reinterpret_cast<char (__cdecl *)(int, unsigned int, unsigned int, unsigned int)>(XI_GAME(0x100016C0u))(a0, a1, a2, a3); }
XI_ORIG int App_RenderFrameMaskedFPU(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10001B50u))(a0); }
XI_ORIG undefined4 App_RenderFrameMaskedFPU_v1() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10001B50u))(); }
XI_ORIG char App_CreateInputSystem(HINSTANCE a0) { return reinterpret_cast<char (__stdcall *)(HINSTANCE)>(XI_GAME(0x10001BB0u))(a0); }
XI_ORIG BOOL CApp_CopyTextToClipboard(void * a0, void * a1) { return reinterpret_cast<BOOL (__thiscall *)(void *, void *)>(XI_GAME(0x10001CC0u))(a0, a1); }
XI_ORIG char Unit100014A0_AppendWithSpace(void * a0) { return reinterpret_cast<char (__cdecl *)(void *)>(XI_GAME(0x10001E70u))(a0); }
XI_ORIG char * Unit100014A0_FindTokenCopy(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<char * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10001F40u))(a0, a1, a2); }
XI_ORIG unsigned long long Gfx_CopySurfaceRectToTexture(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) {
    uint32_t xs[4] = {a0.bits, a1.bits, a2.bits, a3.bits}; uint32_t ra = a4.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 12(%%esi)\n\t""pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $16, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10002040u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG char Gfx_ProbeDeviceCaps(int a0, void * a1, void * a2) { return reinterpret_cast<char (__cdecl *)(int, void *, void *)>(XI_GAME(0x10002130u))(a0, a1, a2); }
XI_ORIG unsigned int Unit100014A0_SwapRedBlue(int a0) { return reinterpret_cast<unsigned int (__cdecl *)(int)>(XI_GAME(0x10002440u))(a0); }
XI_ORIG void FUN_10002840(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10002840u))(a0, a1); }
XI_ORIG CDx * Gfx_SetGammaRGB(int a0, int a1, int a2) { return reinterpret_cast<CDx * (__cdecl *)(int, int, int)>(XI_GAME(0x10002C00u))(a0, a1, a2); }
XI_ORIG double Unit100014A0_GetFloat10456A68Or1() { return reinterpret_cast<double (__cdecl *)()>(XI_GAME(0x10002D80u))(); }
XI_ORIG std::codecvt_base * Gfx_SetAspectIndex(int a0) { return reinterpret_cast<std::codecvt_base * (__cdecl *)(int)>(XI_GAME(0x10002E60u))(a0); }
XI_ORIG void Gfx_ApplyAspectScale(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10002E80u))(a0); }
XI_ORIG long double Gfx_GetAspectTableA(XI_ANY a0) { return reinterpret_cast<long double (__cdecl *)(XI_ANY)>(XI_GAME(0x10002EC0u))(a0); }
XI_ORIG long double Gfx_GetAspectTableB(XI_ANY a0) { return reinterpret_cast<long double (__cdecl *)(XI_ANY)>(XI_GAME(0x10002EE0u))(a0); }
XI_ORIG _DWORD * D3DHolder_Constructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10003070u))(a0); }
XI_ORIG char D3DHolder_Initialize(void * a0, int a1, int a2, int a3, char a4, char a5) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int, char, char)>(XI_GAME(0x10003110u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG char Gfx_TryCreateDeviceForMode(void * a0, int a1, int a2, int a3, char a4, char a5, int a6, int a7) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int, char, char, int, int)>(XI_GAME(0x100033C0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG uint8_t D3DHolder_CheckDepthFormat(void * a0, int a1, int a2, int a3) { return reinterpret_cast<uint8_t (__thiscall *)(void *, int, int, int)>(XI_GAME(0x10003460u))(a0, a1, a2, a3); }
XI_ORIG char Gfx_CreateDevice(void * a0, int a1, int a2, int a3, int a4, int a5, char a6, char a7, int a8) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int, int, int, char, char, int)>(XI_GAME(0x100034B0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG int nullsub_71() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100035A0u))(); }
XI_ORIG int Gfx_InitObject(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x100035F0u))(a0); }
XI_ORIG void Gfx_ResetObject(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10003640u))(a0); }
XI_ORIG char sub_10003650(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10003650u))(a0); }
XI_ORIG void Debug_Construct(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10003660u))(a0); }
XI_ORIG void FUN_10003700() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x10003700u))(); }
XI_ORIG void DebugOverlay_Update(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10003710u))(a0); }
XI_ORIG unsigned long long FUN_100037d0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) {
    uint32_t xs[7] = {a1.bits, a2.bits, a3.bits, a4.bits, a5.bits, a6.bits, a7.bits}; uint32_t ra = a8.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 24(%%esi)\n\t""pushl 20(%%esi)\n\t""pushl 16(%%esi)\n\t""pushl 12(%%esi)\n\t""pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100037D0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Unit10003890_DrawActorAxes(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10003890u))(a0); }
XI_ORIG void Debug_DrawTextTable(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10003B60u))(a0); }
XI_ORIG void Debug_DrawWorldText(void * a0, void * a1, int a2) { reinterpret_cast<void (__stdcall *)(void *, void *, int)>(XI_GAME(0x10003D70u))(a0, a1, a2); }
XI_ORIG void Input_SetFlagToggle(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10005600u))(a0, a1); }
XI_ORIG void * Input_InitState(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10005660u))(a0); }
XI_ORIG undefined FUN_100057d0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x100057D0u))(a0, a1); }
XI_ORIG undefined4 FUN_100058f0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100058F0u))(a0, a1); }
XI_ORIG int sub_10005A40(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10005A40u))(a0); }
XI_ORIG undefined4 DirectInput_InstallMouse(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10007EF0u))(a0); }
XI_ORIG undefined4 DirectInput_ReleaseDevice(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10008770u))(a0, a1); }
XI_ORIG char Input_IsCursorActive(void * a0, unsigned __int8 a1) { return reinterpret_cast<char (__thiscall *)(void *, unsigned __int8)>(XI_GAME(0x10008830u))(a0, a1); }
XI_ORIG unsigned int Input_DispatchButtons(void * a0) { return reinterpret_cast<unsigned int (__thiscall *)(void *)>(XI_GAME(0x100088F0u))(a0); }
XI_ORIG undefined4 XInput_ProbeDll(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10008D90u))(a0); }
XI_ORIG undefined4 XInput_LoadFunctions(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10008E10u))(a0); }
XI_ORIG CDx * CDx_Constructor(void * a0, void * a1, void * a2) { return reinterpret_cast<CDx * (__thiscall *)(void *, void *, void *)>(XI_GAME(0x10008F90u))(a0, a1, a2); }
XI_ORIG int CDx_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10009120u))(a0); }
XI_ORIG char CDx_Initialize(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100091B0u))(a0); }
XI_ORIG char CDx_SetDefaultRenderTargets(void * a0, int a1, int a2, int a3) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int)>(XI_GAME(0x10009620u))(a0, a1, a2, a3); }
XI_ORIG int CDx_SetClearParams(void * a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int)>(XI_GAME(0x10009640u))(a0, a1, a2, a3, a4); }
XI_ORIG int CDx_SetClearColor(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10009670u))(a0, a1); }
XI_ORIG int CDx_EndClearBegin(void * a0, int a1, int a2, int a3, int a4, int a5, int a6) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int, int, int)>(XI_GAME(0x100096B0u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG undefined4 CDx_EndClearBegin_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100096B0u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG uint8_t CDx_TestCooperativeLevel(void * a0) { return reinterpret_cast<uint8_t (__thiscall *)(void *)>(XI_GAME(0x10009710u))(a0); }
XI_ORIG int CDx_Present() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10009720u))(); }
XI_ORIG int CDx_BeginScene(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10009730u))(a0); }
XI_ORIG int CDx_EndScene(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10009740u))(a0); }
XI_ORIG BOOL SetCursorPos(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<BOOL (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x10009770u))(a0, a1); }
XI_ORIG unsigned __int8 CDx_ShowCursor(void * a0, unsigned __int8 a1) { return reinterpret_cast<unsigned __int8 (__thiscall *)(void *, unsigned __int8)>(XI_GAME(0x10009780u))(a0, a1); }
XI_ORIG HCURSOR CDx_SetCursorShape(void * a0, int a1, int a2) { return reinterpret_cast<HCURSOR (__thiscall *)(void *, int, int)>(XI_GAME(0x100097F0u))(a0, a1, a2); }
XI_ORIG int CDx_SetDefaultRenderState(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10009920u))(a0); }
XI_ORIG int CDx_AppendStateRecord(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10009D20u))(a0); }
XI_ORIG int CDx_SetViewport(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x10009DC0u))(a0, a1); }
XI_ORIG unsigned __int8 * CDx_GetRenderTargetTop(void * a0) { return reinterpret_cast<unsigned __int8 * (__thiscall *)(void *)>(XI_GAME(0x10009E00u))(a0); }
XI_ORIG int CDx_GetRenderTargetTop_v1(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10009E00u))(a0); }
XI_ORIG int CDx_PushRenderTarget(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(XI_GAME(0x10009E20u))(a0, a1, a2, a3); }
XI_ORIG undefined4 CDx_PushRenderTarget_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10009E20u))(a0, a1, a2, a3); }
XI_ORIG int CDx_PushRenderTargetTexture_Flag1(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(XI_GAME(0x10009EA0u))(a0, a1, a2, a3); }
XI_ORIG int CDx_PushRenderTargetTexture_Flag0(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(XI_GAME(0x10009EE0u))(a0, a1, a2, a3); }
XI_ORIG int CDx_PushRenderTargetTexture(void * a0, int a1, int a2, int a3, void * a4, void * a5, int a6, int a7) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, void *, void *, int, int)>(XI_GAME(0x10009F20u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG undefined4 CDx_PushRenderTargetTexture_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10009F20u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG void FUN_1000a0a0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1000A0A0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int CDx_SetTexture(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1000A1D0u))(a0, a1, a2); }
XI_ORIG undefined4 CDx_SetTexture_v1(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1000A1D0u))(a0, a1, a2); }
XI_ORIG int CDx_PopRenderTargetA(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1000A200u))(a0, a1, a2); }
XI_ORIG int CDx_PopRenderTargetB(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1000A230u))(a0, a1, a2); }
XI_ORIG int CDx_PopRenderTargetDefault(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1000A290u))(a0, a1, a2); }
XI_ORIG int CDx_PopRenderTarget(int a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<int (__thiscall *)(int, int, int, int, int)>(XI_GAME(0x1000A2B0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 CDx_PopRenderTarget_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1000A2B0u))(a0, a1, a2, a3, a4); }
XI_ORIG int CDx_EndAndClearBegin(void * a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int)>(XI_GAME(0x1000A390u))(a0, a1, a2, a3, a4); }
XI_ORIG int CDx_DrawScreenQuadAtDepth(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1000A3B0u))(a0, a1, a2); }
XI_ORIG int CDx_DrawTextureToViewport(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(XI_GAME(0x1000A4F0u))(a0, a1, a2, a3); }
XI_ORIG void CDx_DrawTexturedScreenQuadUnit(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1000A550u))(a0, a1, a2, a3, a4); }
XI_ORIG int CDx_DrawTexturedScreenQuad(void * a0, int a1, int a2, int a3, int a4, int a5) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int, int)>(XI_GAME(0x1000A5A0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void CDx_DrawTexturedScreenQuad_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1000A5A0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG _DWORD * CDx_CreateTexture(void * a0, unsigned __int16 a1, unsigned __int16 a2, void * a3, int a4, int a5) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, unsigned __int16, unsigned __int16, void *, int, int)>(XI_GAME(0x1000A6E0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined8 CDx_CreateTexture_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1000A6E0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG char CDx_PushRenderTargetRet1(void * a0, int a1, int a2, int a3) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int)>(XI_GAME(0x1000A800u))(a0, a1, a2, a3); }
XI_ORIG int CDx_DrawFadeQuad(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1000A820u))(a0, a1, a2); }
XI_ORIG int Post_DepthBlurComposite(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1000AAB0u))(a0, a1, a2); }
XI_ORIG int sub_1000ABE0(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1000ABE0u))(a0, a1, a2); }
XI_ORIG int CDx_CreateDepthSurfaceLikeMain(void * a0, unsigned __int16 a1, unsigned __int16 a2) { return reinterpret_cast<int (__thiscall *)(void *, unsigned __int16, unsigned __int16)>(XI_GAME(0x1000AD70u))(a0, a1, a2); }
XI_ORIG char CDx_StepFadeCounter(void * a0, float a1, float a2, int a3) { return reinterpret_cast<char (__thiscall *)(void *, float, float, int)>(XI_GAME(0x1000AE40u))(a0, a1, a2, a3); }
XI_ORIG void CDx_NopA() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1000BB10u))(); }
XI_ORIG void CDx_NopB() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1000BB20u))(); }
XI_ORIG char CDx_DrawStereoOddFrameOverlay(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(XI_GAME(0x1000BB90u))(a0, a1); }
XI_ORIG int CDx_SetTransform(void * a0, int a1, void * a2) { return reinterpret_cast<int (__thiscall *)(void *, int, void *)>(XI_GAME(0x1000BD50u))(a0, a1, a2); }
XI_ORIG undefined4 CDx_SetTransform_v1(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1000BD50u))(a0, a1, a2); }
XI_ORIG int CDx_GetTransformCached(void * a0, int a1, void * a2) { return reinterpret_cast<int (__thiscall *)(void *, int, void *)>(XI_GAME(0x1000BDB0u))(a0, a1, a2); }
XI_ORIG undefined4 CDx_GetTransformCached_v1(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1000BDB0u))(a0, a1, a2); }
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
XI_ORIG undefined4 CDx_DrawPrimitiveUP_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1000CD00u))(a0, a1, a2, a3, a4); }
XI_ORIG void CDx_ApplyStereoEyeProjection(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1000CF20u))(a0); }
XI_ORIG int CDx_RestoreProjection(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1000CF90u))(a0); }
XI_ORIG _DWORD * CAcc_Constructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1000CFB0u))(a0); }
XI_ORIG undefined8 Unit1000D0F0_ByteSwap32(XI_ANY a0) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY)>(XI_GAME(0x1000D150u))(a0); }
XI_ORIG char j_CEnv_UpdateDualTargetMode(int a0, int a1) { return reinterpret_cast<char (__thiscall *)(int, int)>(XI_GAME(0x1000D3F0u))(a0, a1); }
XI_ORIG void CEnv_Nop() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1000D620u))(); }
XI_ORIG void FUN_1000d630(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1000D630u))(a0, a1); }
XI_ORIG char CEnv_IsStereoMode(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1000D660u))(a0); }
XI_ORIG undefined4 CEnv_IsStereoMode_v1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1000D660u))(a0); }
XI_ORIG undefined4 FUN_1000d680() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1000D680u))(); }
XI_ORIG undefined4 FUN_1000d6b0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1000D6B0u))(); }
XI_ORIG int App_RegisterSystemDats() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10010370u))(); }
XI_ORIG int App_SaveAllSlotData() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10010520u))(); }
XI_ORIG void App_SaveAllSlotData_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10010520u))(); }
XI_ORIG char * App_Constructor(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10010700u))(a0); }
XI_ORIG __int16 App_SetSeqStep(void * a0, __int16 a1) { return reinterpret_cast<__int16 (__thiscall *)(void *, __int16)>(XI_GAME(0x10010CE0u))(a0, a1); }
XI_ORIG void App_AdvanceSeqStep(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10010D00u))(a0); }
XI_ORIG void App_AdvanceSeqStep_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10010D00u))(a0); }
XI_ORIG void Unit10010370_IncrementShort6(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10010D10u))(a0); }
XI_ORIG char App_InitScene(int a0, int a1) { return reinterpret_cast<char (__thiscall *)(int, int)>(XI_GAME(0x10010D20u))(a0, a1); }
XI_ORIG undefined4 Frame_Render(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10011A00u))(a0); }
XI_ORIG int Post_Afterimage() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10012F40u))(); }
XI_ORIG int Post_FlashAndZoomFeedback(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100132B0u))(a0); }
XI_ORIG char App_RunOpening(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100139E0u))(a0); }
XI_ORIG uint8_t fn_10013AC0() { return reinterpret_cast<uint8_t (__cdecl *)()>(XI_GAME(0x10013AC0u))(); }
XI_ORIG char App_StepGameState5Connect(int a0, int a1) { return reinterpret_cast<char (__thiscall *)(int, int)>(XI_GAME(0x10013AD0u))(a0, a1); }
XI_ORIG char App_StepGameState6(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10013DF0u))(a0); }
XI_ORIG char App_StepGameState7(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10013F10u))(a0); }
XI_ORIG char App_StepGameState8(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10014030u))(a0); }
XI_ORIG uint8_t App_StepGameState9() { return reinterpret_cast<uint8_t (__cdecl *)()>(XI_GAME(0x10014120u))(); }
XI_ORIG char App_DispatchSubState(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10014170u))(a0); }
XI_ORIG undefined4 Game_InGameSubState0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100141C0u))(a0); }
XI_ORIG unsigned long long App_SubStateInitCamera(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10014200u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 App_InitCamera(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10014220u))(a0, a1); }
XI_ORIG undefined4 App_SubStateAdvance(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100142A0u))(a0); }
XI_ORIG undefined4 CAtelIdleTask_Create() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100142B0u))(); }
XI_ORIG undefined4 FUN_10014350(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10014350u))(a0); }
XI_ORIG char Unit10014350_EnterState4AfterShutdown(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100149B0u))(a0); }
XI_ORIG uint8_t Unit10014350_TryShutdownActors(void * a0) { return reinterpret_cast<uint8_t (__thiscall *)(void *)>(XI_GAME(0x10014A20u))(a0); }
XI_ORIG char Unit10014350_StepStagedRelease(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x10014A30u))(a0); }
XI_ORIG char Unit10014350_StepMenuOkThenRelease(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10014AD0u))(a0); }
XI_ORIG uint8_t Camera_ReleaseAll(void * a0) { return reinterpret_cast<uint8_t (__thiscall *)(void *)>(XI_GAME(0x10014C10u))(a0); }
XI_ORIG void Unit10014350_ApplyTables103501F8() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10014C60u))(); }
XI_ORIG undefined4 Unit10014350_ShutdownActorsAndTables(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10014C90u))(a0); }
XI_ORIG undefined4 FUN_10014cb0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10014CB0u))(); }
XI_ORIG undefined4 Unit10014350_ApplyTables10350210() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10014CB0u))(); }
XI_ORIG undefined4 Camera_ClearByte0A() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10014CE0u))(); }
XI_ORIG double Camera_GetFrameScale() { return reinterpret_cast<double (__cdecl *)()>(XI_GAME(0x10014CF0u))(); }
XI_ORIG long double Camera_GetFrameScale_v1(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0; long double st0;
    __asm__ volatile("call *%%edi\n\t"
        : "=t"(st0), "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10014CF0u) : "memory", "cc", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return st0;
}
XI_ORIG long double Camera_GetFrameScale_v2() { return reinterpret_cast<long double (__cdecl *)()>(XI_GAME(0x10014CF0u))(); }
XI_ORIG int GetFrameCounter() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10014D50u))(); }
XI_ORIG undefined4 GetFrameCounter_v1() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10014D50u))(); }
XI_ORIG undefined4 Camera_GetField38() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10014D60u))(); }
XI_ORIG double Camera_GetFloat2DC() { return reinterpret_cast<double (__cdecl *)()>(XI_GAME(0x10014D70u))(); }
XI_ORIG double Camera_GetFloat2E0() { return reinterpret_cast<double (__cdecl *)()>(XI_GAME(0x10014D80u))(); }
XI_ORIG unsigned int Camera_SetClearColorRGB(unsigned __int8 a0, unsigned __int8 a1, unsigned __int8 a2) { return reinterpret_cast<unsigned int (__cdecl *)(unsigned __int8, unsigned __int8, unsigned __int8)>(XI_GAME(0x10014D90u))(a0, a1, a2); }
XI_ORIG void Camera_SetClearColorPacked(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10014DB0u))(a0, a1, a2, a3); }
XI_ORIG void Camera_SetField304BytesOnGlobal(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10014E00u))(a0, a1, a2); }
XI_ORIG void Camera_SetField304Bytes(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10014E20u))(a0, a1, a2, a3); }
XI_ORIG void Camera_SetField2FC(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10014E60u))(a0); }
XI_ORIG void Camera_SetField300(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10014E80u))(a0); }
XI_ORIG void Camera_SetField314(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10014EB0u))(a0); }
XI_ORIG void Camera_SetField318(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10014ED0u))(a0); }
XI_ORIG void Camera_SetViewMatrix(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15) { reinterpret_cast<void (__cdecl *)(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int)>(XI_GAME(0x100150D0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15); }
XI_ORIG undefined4 Camera_Call100151A0OnGlobal(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10015100u))(a0); }
XI_ORIG undefined4 FUN_100151a0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100151A0u))(a0, a1); }
XI_ORIG void Camera_SetMatrix154(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15) { reinterpret_cast<void (__cdecl *)(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int)>(XI_GAME(0x100151F0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15); }
XI_ORIG char * Camera_GetViewMatrixPtr() { return reinterpret_cast<char * (__cdecl *)()>(XI_GAME(0x10015220u))(); }
XI_ORIG int Camera_GetViewMatrixPtr_v1() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10015220u))(); }
XI_ORIG char * sub_10015240() { return reinterpret_cast<char * (__cdecl *)()>(XI_GAME(0x10015240u))(); }
XI_ORIG int Camera_GetField50() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10015250u))(); }
XI_ORIG undefined4 Camera_GetField50_v1() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10015250u))(); }
XI_ORIG char * Camera_GetMatrix154Ptr() { return reinterpret_cast<char * (__cdecl *)()>(XI_GAME(0x10015270u))(); }
XI_ORIG int Camera_GetMatrix154Ptr_v1() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10015270u))(); }
XI_ORIG void Camera_SetField2F8OnGlobal(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10015290u))(a0); }
XI_ORIG void Camera_SetField2F8(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100152B0u))(a0, a1); }
XI_ORIG long double Camera_GetField2F4() { return reinterpret_cast<long double (__cdecl *)()>(XI_GAME(0x100152C0u))(); }
XI_ORIG std::codecvt_base * Camera_SetField2E4(int a0) { return reinterpret_cast<std::codecvt_base * (__cdecl *)(int)>(XI_GAME(0x10015360u))(a0); }
XI_ORIG std::codecvt_base * Camera_SetField2F0(float a0, float a1) { return reinterpret_cast<std::codecvt_base * (__cdecl *)(float, float)>(XI_GAME(0x100153A0u))(a0, a1); }
XI_ORIG void Camera_SetField2F0_v1(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100153A0u))(a0, a1); }
XI_ORIG void FUN_100153d0() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x100153D0u))(); }
XI_ORIG void Camera_BuildPerspectiveFov(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10015490u))(a0); }
XI_ORIG void Camera_ApplyViewProjection() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10015510u))(); }
XI_ORIG undefined4 Camera_GetByte0B() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100155D0u))(); }
XI_ORIG undefined4 Camera_GetWord10() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10015600u))(); }
XI_ORIG undefined4 Camera_GetWord12() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10015610u))(); }
XI_ORIG __int16 Camera_GetViewportWidth() { return reinterpret_cast<__int16 (__cdecl *)()>(XI_GAME(0x10015680u))(); }
XI_ORIG __int16 Camera_GetViewportHeight() { return reinterpret_cast<__int16 (__cdecl *)()>(XI_GAME(0x10015690u))(); }
XI_ORIG int Camera_PickNearestAspect(int a0, int a1, void * a2, void * a3) { return reinterpret_cast<int (__cdecl *)(int, int, void *, void *)>(XI_GAME(0x100156A0u))(a0, a1, a2, a3); }
XI_ORIG int Camera_PickNearestAspect_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100156A0u))(a0, a1, a2, a3); }
XI_ORIG uint8_t fn_10015740() { return reinterpret_cast<uint8_t (__cdecl *)()>(XI_GAME(0x10015740u))(); }
XI_ORIG undefined4 App_WinMain() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100157E0u))(); }
XI_ORIG char App_SetGlobal_104569C5(char a0) { return reinterpret_cast<char (__cdecl *)(char)>(XI_GAME(0x100163C0u))(a0); }
XI_ORIG char App_SetGlobal_104569C6(char a0) { return reinterpret_cast<char (__cdecl *)(char)>(XI_GAME(0x100163E0u))(a0); }
XI_ORIG int App_SetGlobal_104569C8(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10016400u))(a0); }
XI_ORIG undefined4 App_SetGlobal_104569C8_v1(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10016400u))(a0); }
XI_ORIG int App_GetGlobal_104569C8() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10016410u))(); }
XI_ORIG undefined4 App_GetGlobal_104569C8_v1() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10016410u))(); }
XI_ORIG unsigned long long Unit10014350_GetSceneStateByte68(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100164B0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG int App_SetErrorCode(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x100164C0u))(a0); }
XI_ORIG int App_GetGlobalPair(void * a0, void * a1) { return reinterpret_cast<int (__cdecl *)(void *, void *)>(XI_GAME(0x10016560u))(a0, a1); }
XI_ORIG int GammaRamp_SetRGB(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(XI_GAME(0x100170A0u))(a0, a1, a2, a3); }
XI_ORIG int GammaRamp_GetRGB(void * a0, void * a1, void * a2, void * a3) { return reinterpret_cast<int (__thiscall *)(void *, void *, void *, void *)>(XI_GAME(0x100170D0u))(a0, a1, a2, a3); }
XI_ORIG int GammaRamp_GetChannelSamples(void * a0, void * a1, void * a2, void * a3) { return reinterpret_cast<int (__thiscall *)(void *, void *, void *, void *)>(XI_GAME(0x10017100u))(a0, a1, a2, a3); }
XI_ORIG undefined4 GammaRamp_GetChannelSamples_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10017100u))(a0, a1, a2, a3); }
XI_ORIG __int64 GammaRamp_BuildRamps(void * a0) { return reinterpret_cast<__int64 (__thiscall *)(void *)>(XI_GAME(0x10017160u))(a0); }
XI_ORIG void FUN_100171b0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x100171B0u))(a0, a1); }
XI_ORIG void GammaRamp_ApplyIfChanged(int a0) { reinterpret_cast<void (__thiscall *)(int)>(XI_GAME(0x10017230u))(a0); }
XI_ORIG void setPolUrl(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10017630u))(a0, a1); }
XI_ORIG int CShader_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100177A0u))(a0); }
XI_ORIG exception * fn_10017BB0(void * a0) { return reinterpret_cast<exception * (__thiscall *)(void *)>(XI_GAME(0x10017BB0u))(a0); }
XI_ORIG int Unit10017C10_Step(int a0, void * a1, void * a2, int a3) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, int)>(XI_GAME(0x10017C20u))(a0, a1, a2, a3); }
XI_ORIG undefined4 Unit10017C10_Begin(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10017C60u))(a0, a1, a2, a3); }
template <class... R> XI_ORIG int Unit10017C10_NoOpVarargsA(void * a0, R... rest) { return reinterpret_cast<int (__cdecl *)(void *, ...)>(XI_GAME(0x10017D20u))(a0, rest...); }
template <class... R> XI_ORIG void Unit10017C10_NoOpVarargsA_v1(XI_ANY a0, R... rest) { reinterpret_cast<void (__cdecl *)(XI_ANY, decltype(XI_ANY(rest))...)>(XI_GAME(0x10017D20u))(a0, XI_ANY(rest)...); }
template <class... R> XI_ORIG int Unit10017C10_NoOpVarargsB(void * a0, R... rest) { return reinterpret_cast<int (__cdecl *)(void *, ...)>(XI_GAME(0x10017D30u))(a0, rest...); }
template <class... R> XI_ORIG void Unit10017C10_NoOpVarargsB_v1(XI_ANY a0, R... rest) { reinterpret_cast<void (__cdecl *)(XI_ANY, decltype(XI_ANY(rest))...)>(XI_GAME(0x10017D30u))(a0, XI_ANY(rest)...); }
XI_ORIG int nullsub_90(_DWORD a0) { return reinterpret_cast<int (__cdecl *)(_DWORD)>(XI_GAME(0x10017D50u))(a0); }
XI_ORIG void nullsub_90_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10017D50u))(); }
XI_ORIG undefined8 FUN_100181d0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100181D0u))(a0, a1); }
XI_ORIG void Unit10017C10_Call10114F10NoArgs(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100182C0u))(a0); }
XI_ORIG void _exit_0(int a0) { reinterpret_cast<void (__cdecl *)(int)>(XI_GAME(0x100182E0u))(a0); }
XI_ORIG undefined8 _exit_0_v1(XI_ANY a0) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY)>(XI_GAME(0x100182E0u))(a0); }
XI_ORIG void _exit_0_v2(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100182E0u))(a0); }
XI_ORIG void Unit10017C10_Thunk10114F10(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10018300u))(a0, a1, a2); }
XI_ORIG unsigned long long FUN_10018310(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[3] = {a0.bits, a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $12, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10018310u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void * Unit10017C10_CtorVt10329F88(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10018350u))(a0); }
XI_ORIG _DWORD * Unit10017C10_CtorVt10329F8C(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10018430u))(a0); }
XI_ORIG void * Unit10017C10_CtorVt10329F90(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10018550u))(a0); }
XI_ORIG void Unit10017C10_LoadChecksum(void * a0, void * a1, unsigned int a2) { reinterpret_cast<void (__thiscall *)(void *, void *, unsigned int)>(XI_GAME(0x100185A0u))(a0, a1, a2); }
XI_ORIG void * CXiOpening_Constructor(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100186C0u))(a0, a1); }
XI_ORIG undefined4 Unit10018840_IsStateFour(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10018840u))(a0); }
XI_ORIG char sub_10018850(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x10018850u))(a0); }
XI_ORIG _DWORD * OpeningSubtitle_Constructor(void * a0, int a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int)>(XI_GAME(0x10018A50u))(a0, a1); }
XI_ORIG _DWORD * OpeningSubtitle_Reset(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10018B30u))(a0); }
XI_ORIG void OpeningSubtitle_Reset_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10018B30u))(a0); }
XI_ORIG _DWORD * Vt10329FE0_Constructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10018C50u))(a0); }
XI_ORIG int Vt10329FE0_Reset(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10018CE0u))(a0); }
XI_ORIG void Vt10329FE0_Reset_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10018CE0u))(a0); }
XI_ORIG MMRESULT App_BeginTimerPeriod() { return reinterpret_cast<MMRESULT (__cdecl *)()>(XI_GAME(0x10019140u))(); }
XI_ORIG MMRESULT sub_100191B0() { return reinterpret_cast<MMRESULT (__cdecl *)()>(XI_GAME(0x100191B0u))(); }
XI_ORIG _DWORD * Unit10019140_CreateObject() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x100191D0u))(); }
XI_ORIG void CXiTimer_Constructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10019210u))(a0); }
XI_ORIG void * CXiTimerLow_Init(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10019490u))(a0); }
XI_ORIG void FUN_100194f0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100194F0u))(a0); }
XI_ORIG unsigned long long FUN_10019a50(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) {
    uint32_t xs[4] = {a1.bits, a2.bits, a3.bits, a4.bits}; uint32_t ra = a5.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 12(%%esi)\n\t""pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10019A50u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG int CYyMotionQue_SetSlotMotion(void * a0, void * a1, void * a2, int a3, int a4, int a5, float a6, float a7, unsigned int a8, int a9, int a10) { return reinterpret_cast<int (__thiscall *)(void *, void *, void *, int, int, int, float, float, unsigned int, int, int)>(XI_GAME(0x1001A660u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }
XI_ORIG void CYyMotionQue_SetSlotMotion_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9, XI_ANY a10) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1001A660u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }
XI_ORIG void MotionQueList_QueueMotion(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9, XI_ANY a10) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1001A6E0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }
XI_ORIG void CYyMotionQueList_Init(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1001ABF0u))(a0); }
XI_ORIG void FUN_1001ac30(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1001AC30u))(a0); }
XI_ORIG void FUN_1001ac40(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1001AC40u))(a0); }
XI_ORIG void CYyMotionQue_AppendSync(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1001ACB0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG void KzMotionQueList_AppendSync(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1001AD00u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG void FUN_1001b0f0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1001B0F0u))(a0, a1); }
XI_ORIG void FUN_1001b140(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1001B140u))(a0); }
XI_ORIG void KzMotionQueList_AdvanceAll(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1001B170u))(a0, a1); }
XI_ORIG int FUN_1001b1a0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1001B1A0u))(a0); }
XI_ORIG void KzMotionQueList_NotifyAll(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1001B1F0u))(a0, a1, a2); }
XI_ORIG undefined4 CYyMotionQue_SampleInto(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1001B230u))(a0, a1, a2); }
XI_ORIG void CYyMotionQue_Update(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1001B2E0u))(a0, a1, a2); }
XI_ORIG int CYyMotionQue_IsActive(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1001B340u))(a0); }
XI_ORIG undefined4 FUN_1001b360(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1001B360u))(a0); }
XI_ORIG void FUN_1001b520(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1001B520u))(a0, a1); }
XI_ORIG undefined4 Unit1001B510_Update(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1001B5D0u))(a0); }
XI_ORIG undefined4 FUN_1001b710(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1001B710u))(a0); }
XI_ORIG _DWORD * Model_CreateTranslucentTargets() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x1001D0A0u))(); }
XI_ORIG void FUN_1001d110() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x1001D110u))(); }
XI_ORIG void Unit1001B890_Construct(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1001D170u))(a0); }
XI_ORIG void FUN_1001d1f0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1001D1F0u))(a0); }
XI_ORIG char Model_BeginTranslucentPass(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x1001D240u))(a0); }
XI_ORIG char Model_TranslucentUnbindTexture(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1001D350u))(a0); }
XI_ORIG int Unit1001B890_SetColorForce(int a0, char a1) { return reinterpret_cast<int (__stdcall *)(int, char)>(XI_GAME(0x1001D390u))(a0, a1); }
XI_ORIG int Model_CompositeTranslucentPass(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1001D3B0u))(a0); }
XI_ORIG void FUN_1001d8e0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1001D8E0u))(a0, a1); }
XI_ORIG int Gfx_DrawScreenQuad256(int a0, int a1) { return reinterpret_cast<int (__stdcall *)(int, int)>(XI_GAME(0x1001DD00u))(a0, a1); }
XI_ORIG unsigned int Tex_HeightToNormalMap(void * a0, int a1, int a2) { return reinterpret_cast<unsigned int (__thiscall *)(void *, int, int)>(XI_GAME(0x1001E0A0u))(a0, a1, a2); }
XI_ORIG unsigned long long CYyCamMng2_SetGlobalFlags(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[3] = {a0.bits, a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $12, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1001E2F0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 CYyCamMng2_CreateGlobal() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1001E470u))(); }
XI_ORIG long double CYyCamMng2_GetYaw() { return reinterpret_cast<long double (__cdecl *)()>(XI_GAME(0x1001E4D0u))(); }
XI_ORIG undefined4 CYyCamMng2_Reset(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1001E4F0u))(a0); }
XI_ORIG undefined4 CYyCamMng2_SetDefaultDistances(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1001E590u))(a0); }
XI_ORIG void * CYyCamMng2_Construct(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1001E600u))(a0); }
XI_ORIG undefined4 Camera_SetPosition(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1001E760u))(a0, a1); }
XI_ORIG undefined4 Camera_SetTargetIfMoved(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1001E780u))(a0, a1); }
XI_ORIG void YmCameraManager_Follow(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1001EE60u))(a0, a1); }
XI_ORIG void FUN_10021110(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10021110u))(a0, a1); }
XI_ORIG void FUN_10021680(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10021680u))(a0, a1, a2); }
XI_ORIG int Camera_BuildLookAtView(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10021C10u))(a0); }
XI_ORIG void Actor_ClearIndexTable(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100220E0u))(a0); }
XI_ORIG undefined4 Actor_MapPercentScaleIndex(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10022170u))(a0); }
XI_ORIG _BYTE * Actor_InitIndexTable(void * a0) { return reinterpret_cast<_BYTE * (__thiscall *)(void *)>(XI_GAME(0x10022230u))(a0); }
XI_ORIG char CYyModelDt_PrepareRenderState(void * a0, int a1, int a2, int a3, int a4, int a5) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int, int, int)>(XI_GAME(0x100224D0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG unsigned __int16 CYyModelDt_PrepareRenderStateSimple(void * a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<unsigned __int16 (__thiscall *)(void *, int, int, int, int)>(XI_GAME(0x100231C0u))(a0, a1, a2, a3, a4); }
XI_ORIG void * CYyModelDt_Construct(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10025010u))(a0); }
XI_ORIG _DWORD * CYyModelDt_Destructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x100250C0u))(a0); }
XI_ORIG undefined4 CYyModelBase_AllocParts(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100250F0u))(a0, a1); }
XI_ORIG void Unit10025220_Release() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10025220u))(); }
XI_ORIG void Unit10025220_Init(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10025260u))(a0); }
XI_ORIG int ProjTex_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100252F0u))(a0); }
XI_ORIG void ProjTex_Destructor_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100252F0u))(a0); }
XI_ORIG char ProjTex_BeginPass(void * a0, void * a1, int a2, void * a3) { return reinterpret_cast<char (__thiscall *)(void *, void *, int, void *)>(XI_GAME(0x10025330u))(a0, a1, a2, a3); }
XI_ORIG int ProjTex_EndPass(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x100255C0u))(a0, a1); }
XI_ORIG int TexObj_GetD3DTexture(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10025860u))(a0); }
XI_ORIG int ProjTex_CaptureToTarget(void * a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int)>(XI_GAME(0x10025870u))(a0, a1, a2, a3, a4); }
XI_ORIG int Unit100252F0_Nop_10025BD0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10025BD0u))(); }
XI_ORIG void Unit100252F0_Nop_10025BE0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10025BE0u))(); }
XI_ORIG void Unit100252F0_ClearCounter(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10025BF0u))(a0); }
XI_ORIG void FUN_10025c30(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10025C30u))(a0); }
XI_ORIG char EnvMap_BeginPass(void * a0, int a1, void * a2) { return reinterpret_cast<char (__stdcall *)(void *, int, void *)>(XI_GAME(0x10025C40u))(a0, a1, a2); }
XI_ORIG int EnvMap_EndPass(int a0) { return reinterpret_cast<int (__stdcall *)(int)>(XI_GAME(0x10025F70u))(a0); }
XI_ORIG int sub_10026210(int a0, int a1, int a2, int a3) { return reinterpret_cast<int (__stdcall *)(int, int, int, int)>(XI_GAME(0x10026210u))(a0, a1, a2, a3); }
XI_ORIG int * CYyIcon_Constructor(void * a0, int a1, void * a2, int a3) { return reinterpret_cast<int * (__thiscall *)(void *, int, void *, int)>(XI_GAME(0x10026890u))(a0, a1, a2, a3); }
XI_ORIG void CXiMovie_Draw(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10026AD0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int CYyIcon_DrawSubRect(void * a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int, int, int, int, int, int)>(XI_GAME(0x10026C70u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9); }
XI_ORIG _DWORD * Vec3_SetZero(void * a0) { return reinterpret_cast<_DWORD * (__cdecl *)(void *)>(XI_GAME(0x10026E00u))(a0); }
XI_ORIG void Quat_SetIdentity(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10026E10u))(a0); }
XI_ORIG int Vec4_Set(int a0, float a1, int a2, int a3, int a4) { return reinterpret_cast<int (__cdecl *)(int, float, int, int, int)>(XI_GAME(0x10026E30u))(a0, a1, a2, a3, a4); }
XI_ORIG void Vec4_Set_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10026E30u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 Vec3_Set(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10026E50u))(a0, a1, a2, a3); }
XI_ORIG int Vec4_Copy(void * a0, void * a1) { return reinterpret_cast<int (__cdecl *)(void *, void *)>(XI_GAME(0x10026E90u))(a0, a1); }
XI_ORIG undefined8 Vec4_Copy_v1(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10026E90u))(a0, a1); }
XI_ORIG int Vec3_Copy(void * a0, void * a1) { return reinterpret_cast<int (__cdecl *)(void *, void *)>(XI_GAME(0x10026EB0u))(a0, a1); }
XI_ORIG undefined4 Vec3_Copy_v1(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10026EB0u))(a0, a1); }
XI_ORIG float * Vec3_AddInPlace(void * a0, void * a1) { return reinterpret_cast<float * (__cdecl *)(void *, void *)>(XI_GAME(0x10026F20u))(a0, a1); }
XI_ORIG undefined4 Vec3_AddInPlace_v1(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10026F20u))(a0, a1); }
XI_ORIG float * sub_10026F50(void * a0, void * a1) { return reinterpret_cast<float * (__cdecl *)(void *, void *)>(XI_GAME(0x10026F50u))(a0, a1); }
XI_ORIG float * Vec3_Add_10026FA0(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, void *)>(XI_GAME(0x10026FA0u))(a0, a1, a2); }
XI_ORIG void Vec3_Add_10026FA0_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10026FA0u))(a0, a1, a2); }
XI_ORIG float * Vec2_Add(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, void *)>(XI_GAME(0x10026FD0u))(a0, a1, a2); }
XI_ORIG float * sub_10027020(void * a0, float a1) { return reinterpret_cast<float * (__cdecl *)(void *, float)>(XI_GAME(0x10027020u))(a0, a1); }
XI_ORIG float * Vec2_AddScalar(void * a0, float a1) { return reinterpret_cast<float * (__cdecl *)(void *, float)>(XI_GAME(0x10027050u))(a0, a1); }
XI_ORIG undefined4 Vec3_SubInPlace(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100270A0u))(a0, a1); }
XI_ORIG float * Vec3_Sub(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, void *)>(XI_GAME(0x10027120u))(a0, a1, a2); }
XI_ORIG undefined4 Vec3_Sub_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10027120u))(a0, a1, a2); }
XI_ORIG void Vec3_ScaleInPlace(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100272B0u))(a0, a1); }
XI_ORIG int fn_100272B0(int a0, float a1) { return reinterpret_cast<int (__cdecl *)(int, float)>(XI_GAME(0x100272B0u))(a0, a1); }
XI_ORIG float * Vec3_Scale(void * a0, void * a1, float a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, float)>(XI_GAME(0x100272E0u))(a0, a1, a2); }
XI_ORIG undefined4 Vec3_Scale_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100272E0u))(a0, a1, a2); }
XI_ORIG float * Vec3_MulInPlace(void * a0, void * a1) { return reinterpret_cast<float * (__cdecl *)(void *, void *)>(XI_GAME(0x10027310u))(a0, a1); }
XI_ORIG void Vec3_MulInPlace_v1(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10027310u))(a0, a1); }
XI_ORIG float * Vec3_MulComponents(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, void *)>(XI_GAME(0x10027340u))(a0, a1, a2); }
XI_ORIG undefined4 Vec3_MulComponents_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10027340u))(a0, a1, a2); }
XI_ORIG unsigned long long FUN_100274b0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100274B0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void sub_100274B0(void * a0) { reinterpret_cast<void (__cdecl *)(void *)>(XI_GAME(0x100274B0u))(a0); }
XI_ORIG void Vec3_CopyThen100274B0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10027510u))(a0, a1); }
XI_ORIG long double FUN_10027530(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<long double (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10027530u))(a0, a1); }
XI_ORIG long double FUN_100275d0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<long double (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100275D0u))(a0, a1); }
XI_ORIG long double FUN_10027600(XI_ANY a0) { return reinterpret_cast<long double (__cdecl *)(XI_ANY)>(XI_GAME(0x10027600u))(a0); }
XI_ORIG long double Vec3_Call100275D0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<long double (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10027620u))(a0, a1); }
XI_ORIG undefined4 Vec3_Lerp(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100276A0u))(a0, a1, a2, a3); }
XI_ORIG void Gfx_ProjectWorldToClip(void * a0, void * a1) { reinterpret_cast<void (__cdecl *)(void *, void *)>(XI_GAME(0x100276F0u))(a0, a1); }
XI_ORIG void Gfx_ProjectWorldToClip_v1(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100276F0u))(a0, a1); }
XI_ORIG float * Gfx_NdcToScreen(void * a0) { return reinterpret_cast<float * (__cdecl *)(void *)>(XI_GAME(0x100278E0u))(a0); }
XI_ORIG void Gfx_NdcToScreen_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100278E0u))(a0); }
XI_ORIG undefined4 Unit10026C70_ReturnThis(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10027990u))(a0); }
XI_ORIG Iostream_init * fn_10027990(void * a0) { return reinterpret_cast<Iostream_init * (__thiscall *)(void *)>(XI_GAME(0x10027990u))(a0); }
XI_ORIG void Unit10026C70_Nop() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100279A0u))(); }
XI_ORIG int Mat4_SetIdentity(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100279B0u))(a0); }
XI_ORIG void Mat4_SetIdentity_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100279B0u))(a0); }
XI_ORIG int Mat4_Copy(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x100279F0u))(a0, a1); }
XI_ORIG undefined4 Mat4_Copy_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100279F0u))(a0, a1); }
XI_ORIG int Mat4_BuildScale(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x10027AE0u))(a0, a1); }
XI_ORIG int Mat4_SetScale(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x10027B30u))(a0, a1); }
XI_ORIG void Mat4_SetScale_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10027B30u))(a0, a1); }
XI_ORIG int Mat4_PostRotateX(void * a0, float a1) { return reinterpret_cast<int (__thiscall *)(void *, float)>(XI_GAME(0x10027B80u))(a0, a1); }
XI_ORIG void Mat4_PostRotateX_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10027B80u))(a0, a1); }
XI_ORIG int Mat4_PostRotateY(void * a0, float a1) { return reinterpret_cast<int (__thiscall *)(void *, float)>(XI_GAME(0x10027BD0u))(a0, a1); }
XI_ORIG void Mat4_PostRotateY_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10027BD0u))(a0, a1); }
XI_ORIG int Mat4_PostRotateZ(void * a0, float a1) { return reinterpret_cast<int (__thiscall *)(void *, float)>(XI_GAME(0x10027C20u))(a0, a1); }
XI_ORIG void Mat4_PostRotateZ_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10027C20u))(a0, a1); }
XI_ORIG int Mat4_SetRow3(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x10027CD0u))(a0, a1); }
XI_ORIG void Mat4_SetRow3_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10027CD0u))(a0, a1); }
XI_ORIG int Mat4_SetTranslation(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x10027CF0u))(a0, a1); }
XI_ORIG undefined4 Mat4_SetTranslation_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10027CF0u))(a0, a1); }
XI_ORIG int Mat4_PostMultiply(int a0, int a1) { return reinterpret_cast<int (__thiscall *)(int, int)>(XI_GAME(0x10027D10u))(a0, a1); }
XI_ORIG void Mat4_PostMultiply_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10027D10u))(a0, a1); }
XI_ORIG int Mat4_PreMultiply(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x10027E90u))(a0, a1); }
XI_ORIG void Mat4_PreMultiply_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10027E90u))(a0, a1); }
XI_ORIG _BYTE * sub_10027F60(void * a0, int a1) { return reinterpret_cast<_BYTE * (__thiscall *)(void *, int)>(XI_GAME(0x10027F60u))(a0, a1); }
XI_ORIG float * Mat4_TransformVec4InPlace(void * a0, void * a1) { return reinterpret_cast<float * (__thiscall *)(void *, void *)>(XI_GAME(0x10028140u))(a0, a1); }
XI_ORIG undefined4 Mat4_TransformVec4InPlace_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10028140u))(a0, a1); }
XI_ORIG undefined4 Mat4_TransformVec4(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10028170u))(a0, a1, a2); }
XI_ORIG float * Mat4_TransformPointInPlace(void * a0, void * a1) { return reinterpret_cast<float * (__thiscall *)(void *, void *)>(XI_GAME(0x10028200u))(a0, a1); }
XI_ORIG undefined4 Mat4_TransformPointInPlace_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10028200u))(a0, a1); }
XI_ORIG undefined8 Mat4_TransformPoint(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10028230u))(a0, a1, a2); }
XI_ORIG float * Mat4_TransformNormalInPlace(void * a0, void * a1) { return reinterpret_cast<float * (__thiscall *)(void *, void *)>(XI_GAME(0x10028290u))(a0, a1); }
XI_ORIG undefined4 Mat4_TransformNormalInPlace_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10028290u))(a0, a1); }
XI_ORIG float * Mat4_TransformNormal(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__thiscall *)(void *, void *, void *)>(XI_GAME(0x100282C0u))(a0, a1, a2); }
XI_ORIG undefined4 Mat4_TransformNormal_v1(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x100282C0u))(a0, a1, a2); }
XI_ORIG void Mat4_TransformCoordInPlace(void * a0, void * a1) { reinterpret_cast<void (__thiscall *)(void *, void *)>(XI_GAME(0x10028320u))(a0, a1); }
XI_ORIG undefined4 Mat4_TransformCoordInPlace_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10028320u))(a0, a1); }
XI_ORIG int Mat4_InvertOrthonormal(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100283C0u))(a0); }
XI_ORIG undefined4 Mat4_InvertOrthonormal_v1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100283C0u))(a0); }
XI_ORIG _DWORD * Mem_AllocObject(int a0, int a1) { return reinterpret_cast<_DWORD * (__cdecl *)(int, int)>(XI_GAME(0x10028640u))(a0, a1); }
XI_ORIG undefined4 * Mem_AllocObject_v1(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10028640u))(a0, a1); }
XI_ORIG _DWORD * Mem_AllocWithFallback(int a0, int a1) { return reinterpret_cast<_DWORD * (__cdecl *)(int, int)>(XI_GAME(0x100286B0u))(a0, a1); }
XI_ORIG undefined8 Mem_AllocWithFallback_v1(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100286B0u))(a0, a1); }
XI_ORIG _DWORD * Mem_FreeObject(int a0) { return reinterpret_cast<_DWORD * (__cdecl *)(int)>(XI_GAME(0x10028700u))(a0); }
XI_ORIG void Mem_FreeObject_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10028700u))(a0); }
XI_ORIG int SharedDeleteHelper(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10028710u))(a0); }
XI_ORIG void SharedDeleteHelper_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10028710u))(a0); }
XI_ORIG int FUN_10028730() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10028730u))(); }
XI_ORIG int MemBlock_AllocFromFront(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10028850u))(a0, a1, a2); }
XI_ORIG int MemBlock_AllocFromBack(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10028930u))(a0, a1, a2); }
XI_ORIG int FUN_100289f0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100289F0u))(); }
XI_ORIG int FUN_100289f0_v1(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100289F0u))(a0, a1); }
XI_ORIG void FUN_10028aa0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10028AA0u))(a0); }
XI_ORIG int MemBlock_GetSize(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10028B90u))(a0); }
XI_ORIG int MemBlock_GetSize_v1(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10028B90u))(a0); }
XI_ORIG unsigned long long Unit10028A50_CallEnabledEntries(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[3] = {a0.bits, a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $12, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10028BF0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Unit10028A50_CallEnabledList0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10028C30u))(a0); }
XI_ORIG void Unit10028A50_CallEnabledList1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10028C50u))(a0); }
XI_ORIG void Unit10028A50_CallEnabledList2(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10028C70u))(a0); }
XI_ORIG void Unit10028A50_CallEnabledList3(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10028C90u))(a0); }
XI_ORIG _DWORD * Mem_AllocFromArea(int a0, int a1) { return reinterpret_cast<_DWORD * (__cdecl *)(int, int)>(XI_GAME(0x10028FD0u))(a0, a1); }
XI_ORIG undefined8 Mem_AllocFromArea_v1(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10028FD0u))(a0, a1); }
XI_ORIG undefined4 * Mem_AllocFromArea_v2(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10028FD0u))(a0, a1); }
XI_ORIG void Unit10028FD0_CallListByKind(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10029150u))(a0); }
XI_ORIG undefined4 Unit10029260_ApplyFourTables(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x10029390u))(a0); }
XI_ORIG int CreateSharedVertexBuffers() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10029A50u))(); }
XI_ORIG void Unit100298D0_ReleaseAll() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10029B60u))(); }
XI_ORIG void CYyModelDt_InitBase(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10029BB0u))(a0); }
XI_ORIG void FUN_10029c00(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10029C00u))(a0); }
XI_ORIG undefined4 * CYyModelDt_Init(void * a0) { return reinterpret_cast<undefined4 * (__thiscall *)(void *)>(XI_GAME(0x10029C90u))(a0); }
XI_ORIG _DWORD * CYyModelDtBase_Destructor(void * a0, int a1) { return reinterpret_cast<_DWORD * (__fastcall *)(void *, int)>(XI_GAME(0x10029CD0u))(a0, a1); }
XI_ORIG void * CYyModelBase_Construct(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10029D30u))(a0); }
XI_ORIG _DWORD * CYyModelBase_Destructor(void * a0, int a1) { return reinterpret_cast<_DWORD * (__fastcall *)(void *, int)>(XI_GAME(0x10029DD0u))(a0, a1); }
XI_ORIG char * CYyModelBase_GetPartList(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10029E70u))(a0); }
XI_ORIG int CYyModelBase_GetPartList_v1(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10029E70u))(a0); }
XI_ORIG int Unit10029E50_FindTailLink(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10029EB0u))(a0); }
XI_ORIG void Unit10029E50_AppendAtTail(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10029F00u))(a0, a1); }
XI_ORIG undefined4 * CYyModelBase_AddPartFromResource(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10029F70u))(a0, a1); }
XI_ORIG void CYyModelBase_RefreshParts(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1002A0C0u))(a0); }
XI_ORIG void FUN_1002a130(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1002A130u))(a0, a1); }
XI_ORIG undefined4 Model_AnimateAndPose(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1002A140u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int CYyModelBase_DrawLit(int a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(int, int, int, int)>(XI_GAME(0x1002A3B0u))(a0, a1, a2, a3); }
XI_ORIG void FUN_1002a700(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1002A700u))(a0, a1, a2); }
XI_ORIG undefined4 CYyModelBase_TransformVec4ByPart(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1002A750u))(a0, a1, a2); }
XI_ORIG undefined4 Unit1002A0C0_ApplyToVec4(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1002A780u))(a0, a1, a2); }
XI_ORIG void Part_CopyTranslation(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x1002A890u))(a0, a1); }
XI_ORIG void Part_AddTranslation(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1002A8B0u))(a0, a1, a2); }
XI_ORIG int CYyModelBase_FindPartIndex(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1002A9B0u))(a0, a1); }
XI_ORIG int FUN_1002a9e0(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1002A9E0u))(a0, a1); }
XI_ORIG void CYyModelBase_ForwardToField24(void * a0, int a1, int a2) { reinterpret_cast<void (__thiscall *)(void *, int, int)>(XI_GAME(0x1002AC50u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1002b3f0(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x1002B3F0u))(a0); }
XI_ORIG uint32_t FUN_1002b470(XI_ANY a0) { return reinterpret_cast<uint32_t (__stdcall *)(XI_ANY)>(XI_GAME(0x1002B470u))(a0); }
XI_ORIG char * CYyModel_Constructor(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x1002B4B0u))(a0); }
XI_ORIG int CYyModel_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1002B550u))(a0); }
XI_ORIG char * CYyModel_GetField44Addr(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x1002B5A0u))(a0); }
XI_ORIG int CYyModel_GetField44Addr_v1(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1002B5A0u))(a0); }
XI_ORIG int CYyModel_TailLink(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1002B5E0u))(a0); }
XI_ORIG void CYyModel_AppendBase(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1002B630u))(a0, a1); }
XI_ORIG char CXiSkeletonActor_DrawModel(int a0, void * a1, void * a2, int a3, void * a4, int a5, int a6, unsigned int a7) { return reinterpret_cast<char (__thiscall *)(int, void *, void *, int, void *, int, int, unsigned int)>(XI_GAME(0x1002B6E0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG char CXiSkeletonActor_AnimateModels(int a0, void * a1, void * a2, int a3, void * a4) { return reinterpret_cast<char (__thiscall *)(int, void *, void *, int, void *)>(XI_GAME(0x1002C2D0u))(a0, a1, a2, a3, a4); }
XI_ORIG void CXiSkeletonActor_PrepareModelsOnce(int a0, int a1, int a2) { reinterpret_cast<void (__thiscall *)(int, int, int)>(XI_GAME(0x1002C360u))(a0, a1, a2); }
XI_ORIG undefined4 CYyModel_GetPartOffset(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1002C400u))(a0, a1, a2, a3); }
XI_ORIG undefined4 CYyModel_QueryPartsA(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1002C710u))(a0, a1); }
XI_ORIG uint32_t CYyModel_QueryPartsB(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1002C750u))(a0, a1); }
XI_ORIG _DWORD * List_GetTail(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1002C7C0u))(a0); }
XI_ORIG void * List_GetTail_v1(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1002C7C0u))(a0); }
XI_ORIG void FUN_1002c7e0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1002C7E0u))(a0, a1); }
XI_ORIG void Unit1002C7C0_PopHead(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1002C800u))(a0, a1); }
XI_ORIG _DWORD * Mem_AllocDefault(int a0) { return reinterpret_cast<_DWORD * (__cdecl *)(int)>(XI_GAME(0x1002C890u))(a0); }
XI_ORIG undefined4 Mem_AllocDefault_v1(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1002C890u))(a0); }
XI_ORIG int thunk_sub_10028AA0(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x1002C8B0u))(a0); }
XI_ORIG void thunk_sub_10028AA0_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1002C8B0u))(a0); }
XI_ORIG void thunk_sub_10028AA0_v2() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1002C8B0u))(); }
XI_ORIG undefined thunk_sub_10028AA0_v3(XI_ANY a0) { return reinterpret_cast<undefined (__cdecl *)(XI_ANY)>(XI_GAME(0x1002C8B0u))(a0); }
XI_ORIG undefined8 CYyObject_IsKindOf(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1002C8F0u))(a0, a1); }
XI_ORIG void CYyModelDt_SkinAndDrawV2(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1002DC10u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int Gfx_SetStreamSourceCached(unsigned int a0, int a1, int a2) { return reinterpret_cast<int (__cdecl *)(unsigned int, int, int)>(XI_GAME(0x1002F130u))(a0, a1, a2); }
XI_ORIG undefined4 Gfx_ApplyCachedStreamSource(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1002F160u))(a0); }
XI_ORIG int Gfx_ApplyCachedVertexShader() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1002F1A0u))(); }
XI_ORIG int Gfx_SetVertexShaderCached(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x1002F1C0u))(a0); }
XI_ORIG _DWORD * Model_SkinVerticesCpu(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1002F1D0u))(a0); }
XI_ORIG void FUN_100309b0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100309B0u))(a0, a1, a2, a3); }
XI_ORIG void FUN_10030a40(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10030A40u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_10030fb0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10030FB0u))(a0); }
XI_ORIG void * Quat_SetIdentityChained(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10032A40u))(a0); }
XI_ORIG void Quat_SetIdentityB(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10032A50u))(a0); }
XI_ORIG void Unit100310A0_Nop() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10032A70u))(); }
XI_ORIG undefined4 Quat_Multiply(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10032B50u))(a0, a1, a2); }
XI_ORIG void Quat_NormalizeInPlace(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10032DA0u))(a0); }
XI_ORIG void BoneXform_PostMultiplyInto(void * a0, int a1) { reinterpret_cast<void (__thiscall *)(void *, int)>(XI_GAME(0x10032FA0u))(a0, a1); }
XI_ORIG void Unit100310A0_FromQuat(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10032FE0u))(a0, a1); }
XI_ORIG undefined4 Quat_NLerp(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10033220u))(a0, a1, a2, a3); }
XI_ORIG void FUN_10033360(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10033360u))(a0, a1, a2, a3); }
XI_ORIG int Unit100310A0_InitTargets() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100336B0u))(); }
XI_ORIG void Unit100310A0_ReleaseTargets() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100336D0u))(); }
XI_ORIG void Unit100310A0_Construct(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10033710u))(a0); }
XI_ORIG void FUN_100337c0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100337C0u))(a0); }
XI_ORIG char Model_BeginOffscreenPass(int a0, void * a1, int a2) { return reinterpret_cast<char (__thiscall *)(int, void *, int)>(XI_GAME(0x10033800u))(a0, a1, a2); }
XI_ORIG char Model_EndOffscreenPassBindTextures(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(XI_GAME(0x10033A70u))(a0, a1); }
XI_ORIG char Shadow_DrawByMode(int a0, void * a1) { return reinterpret_cast<char (__thiscall *)(int, void *)>(XI_GAME(0x10033DA0u))(a0, a1); }
XI_ORIG void Shadow_SmoothLightDirection(void * a0, void * a1) { reinterpret_cast<void (__thiscall *)(void *, void *)>(XI_GAME(0x10033EA0u))(a0, a1); }
XI_ORIG void CYySkl_Construct(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10034230u))(a0); }
XI_ORIG void FUN_10034270(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10034270u))(a0); }
XI_ORIG void CYyModelBase_AllocMatrixBuffers(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100342A0u))(a0); }
XI_ORIG int Skel_GetRecordAt(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100351F0u))(a0, a1); }
XI_ORIG int Skel_GetBoneTableBase(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10035210u))(a0); }
XI_ORIG unsigned __int16 * Skel_GetTableAfterBones(int a0, int a1) { return reinterpret_cast<unsigned __int16 * (__fastcall *)(int, int)>(XI_GAME(0x10035250u))(a0, a1); }
XI_ORIG uint16_t * Skel_GetTableAfterBones_v1(void * a0) { return reinterpret_cast<uint16_t * (__thiscall *)(void *)>(XI_GAME(0x10035250u))(a0); }
XI_ORIG undefined4 Skel_ClearMarks(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100352A0u))(a0); }
XI_ORIG void Skel_MarkMarkedChains(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100352F0u))(a0); }
XI_ORIG void FUN_10035330(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10035330u))(a0, a1, a2, a3); }
XI_ORIG void Skel_MarkChain(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10035330u))(a0, a1, a2, a3); }
XI_ORIG void * CMoTask_CtorByMode(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10035A00u))(a0, a1, a2, a3); }
XI_ORIG char CMoTask_VT_1032A904_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10035B00u))(a0); }
XI_ORIG unsigned long long MoTask_KillAllGlobalSlots(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10035F00u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Unit10035B00_ClearGlobal10463930(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10035F80u))(a0); }
XI_ORIG undefined4 Unit10035B00_SetScaled10351708(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10035FB0u))(a0); }
XI_ORIG unsigned long long FUN_10036020(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10036020u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long MoTask_KillGlobalSlot1(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10036070u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Unit10035B00_ClearGlobal10463934(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100360C0u))(a0); }
XI_ORIG unsigned long long Music_ApplyVolume(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100360F0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Unit10035B00_SetCurrent10463914(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10036170u))(a0); }
XI_ORIG int Unit10035B00_ResetGlobals() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100361D0u))(); }
XI_ORIG char CYySoundElem_UpdateQueue() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x10036230u))(); }
XI_ORIG undefined4 CYySoundElem_QueueIfAbsent(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10036360u))(a0); }
XI_ORIG void FUN_100363b0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x100363B0u))(a0, a1, a2); }
XI_ORIG undefined4 CYySoundElem_ClearHandle(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10036420u))(a0); }
XI_ORIG uint32_t Unit10035B00_AddUniqueId(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x10036460u))(a0); }
XI_ORIG void CYySoundElem_UpdateOne(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100364A0u))(a0); }
XI_ORIG undefined4 CYySoundElem_UpdateTimerAndPosition(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100364E0u))(a0); }
XI_ORIG undefined4 CYySoundElem_IsTimerPositive(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100367C0u))(a0); }
XI_ORIG unsigned long long CYySoundElem_IsHandleInFrameList(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100367F0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG uint32_t CYySoundElem_FindInQueue(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x10036830u))(a0); }
XI_ORIG undefined4 CYySoundElem_AppendToQueue(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10036870u))(a0); }
XI_ORIG void CYySoundElem_PushActive(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10036980u))(a0); }
XI_ORIG undefined4 CYySoundElem_Constructor(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100369F0u))(a0); }
XI_ORIG undefined4 Unit10036B90_SendStopToLobby(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10036B90u))(a0, a1); }
XI_ORIG uint32_t FUN_10036c70(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10036C70u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG undefined8 Unit10036B90_SetVolumeScalar(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10036F00u))(a0, a1, a2); }
XI_ORIG undefined8 Unit10036B90_SetVolumeVector(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10036F50u))(a0, a1, a2); }
XI_ORIG void Unit10036B90_CreateSoundElem(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10037020u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG void FUN_10037960(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10037960u))(a0); }
XI_ORIG void SetMusicTrackVolume_C() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10037B30u))(); }
XI_ORIG undefined8 SetMusicTrackVolume_A() { return reinterpret_cast<undefined8 (__cdecl *)()>(XI_GAME(0x10037B90u))(); }
XI_ORIG undefined8 SetMusicTrackVolume_B() { return reinterpret_cast<undefined8 (__cdecl *)()>(XI_GAME(0x10037BF0u))(); }
XI_ORIG undefined4 SetMusicTrackVolume_B_v1() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10037BF0u))(); }
XI_ORIG undefined8 YmSePlayBeep() { return reinterpret_cast<undefined8 (__cdecl *)()>(XI_GAME(0x10037C50u))(); }
XI_ORIG undefined4 MusicTrack_SetVolumeId0009() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10037D70u))(); }
XI_ORIG undefined4 MusicTrack_SetVolumeId0010() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10037DD0u))(); }
XI_ORIG undefined4 MusicTrack_SetVolumeId0011() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10037E30u))(); }
XI_ORIG void MusicTrack_SetVolumeId0013() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10037E90u))(); }
XI_ORIG void FUN_10037ef0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10037EF0u))(); }
XI_ORIG undefined4 MusicTrack_SetVolumeId0016() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10037FB0u))(); }
XI_ORIG void Unit10037AC0_Create(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100380F0u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10038330(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10038330u))(a0, a1, a2); }
XI_ORIG unsigned long long FUN_10038440(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[3] = {a0.bits, a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $12, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10038440u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Music_SetTrackVolume(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[3] = {a0.bits, a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $12, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10038470u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned int Font_QueueText(void * a0, int a1, int a2, char a3) { return reinterpret_cast<unsigned int (__cdecl *)(void *, int, int, char)>(XI_GAME(0x100384B0u))(a0, a1, a2, a3); }
XI_ORIG int Font_DrawPass() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10038520u))(); }
XI_ORIG unsigned __int8 * Font_DrawString(void * a0, float a1, float a2, float a3) { return reinterpret_cast<unsigned __int8 * (__cdecl *)(void *, float, float, float)>(XI_GAME(0x10038720u))(a0, a1, a2, a3); }
XI_ORIG _DWORD * Post_CreateScreenEffectTargets() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x10038930u))(); }
XI_ORIG void Unit10038930_ReleaseTextures() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100389C0u))(); }
XI_ORIG void Unit10038930_Init(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10038A20u))(a0); }
XI_ORIG void FUN_10038a60(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10038A60u))(a0); }
XI_ORIG void sub_10038A70(int a0) { reinterpret_cast<void (__stdcall *)(int)>(XI_GAME(0x10038A70u))(a0); }
XI_ORIG int Post_CaptureSceneToTarget(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10038A80u))(a0); }
XI_ORIG int Post_DrawRotatingGhostLayers(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10038E10u))(a0); }
XI_ORIG _DWORD * sub_10039410(void * a0, void * a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *)>(XI_GAME(0x10039410u))(a0, a1); }
XI_ORIG void * CYyObject_Init(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10039440u))(a0); }
XI_ORIG void ThreadPool_StartThread(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100394D0u))(a0, a1, a2, a3); }
XI_ORIG void ThreadPool_Shutdown(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10039550u))(a0); }
XI_ORIG undefined4 FUN_10039590(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10039590u))(a0, a1); }
XI_ORIG undefined4 CYyObject_VT_1032A9F8_Construct(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100395D0u))(a0); }
XI_ORIG void FUN_10039670(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10039670u))(a0, a1, a2, a3); }
XI_ORIG void Thread_ResumeAndWait(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100396A0u))(a0); }
XI_ORIG void FUN_100397a0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100397A0u))(a0, a1); }
XI_ORIG int Util_Copy16Bytes(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10039810u))(a0, a1); }
XI_ORIG void Util_Copy16Bytes_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10039810u))(a0, a1); }
XI_ORIG void * CYyTexBase_Constructor(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10039830u))(a0); }
XI_ORIG void CYyTexBase_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100398D0u))(a0); }
XI_ORIG void CYyTexBase_SetName(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100398E0u))(a0, a1); }
XI_ORIG void * TexObj_Constructor(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10039920u))(a0); }
XI_ORIG void CYyTex_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10039970u))(a0); }
XI_ORIG char TexObj_CreateAndUpload(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10039A30u))(a0); }
XI_ORIG undefined1 TexObj_CreateAndUpload_v1(void * a0) { return reinterpret_cast<undefined1 (__thiscall *)(void *)>(XI_GAME(0x10039A30u))(a0); }
XI_ORIG undefined1 TexObj_Reupload(void * a0) { return reinterpret_cast<undefined1 (__thiscall *)(void *)>(XI_GAME(0x10039A60u))(a0); }
XI_ORIG undefined4 TexObj_Create(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10039A80u))(a0); }
XI_ORIG undefined4 TexObj_Upload(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10039E50u))(a0); }
XI_ORIG _WORD * CYyTexMng_Init(void * a0) { return reinterpret_cast<_WORD * (__thiscall *)(void *)>(XI_GAME(0x1003AA50u))(a0); }
XI_ORIG int CYyTexMng_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003AA90u))(a0); }
XI_ORIG int TexMgr_GetField8(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003AAB0u))(a0); }
XI_ORIG int * FUN_1003ab10(void * a0, XI_ANY a1) { return reinterpret_cast<int * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1003AB10u))(a0, a1); }
XI_ORIG void FUN_1003ab40(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x1003AB40u))(a0); }
XI_ORIG char TexMgr_UploadPumpEntry(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1003AB70u))(a0); }
XI_ORIG void TexMgr_Enqueue(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1003AB80u))(a0, a1); }
XI_ORIG undefined4 TexMgr_UploadPump(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1003ABB0u))(a0); }
XI_ORIG int CYyTexMng_FindByName(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1003AC80u))(a0, a1); }
XI_ORIG undefined8 CYyTexMng_FindByName_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1003AC80u))(a0, a1); }
XI_ORIG int sub_1003ACD0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1003ACD0u))(a0, a1); }
XI_ORIG int sub_1003AD40(void * a0, void * a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, void *, int)>(XI_GAME(0x1003AD40u))(a0, a1, a2); }
XI_ORIG int sub_1003ADC0(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1003ADC0u))(a0, a1, a2); }
XI_ORIG _DWORD * TexMgr_FindByName(void * a0, int a1, int a2, char a3) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int, int, char)>(XI_GAME(0x1003AEA0u))(a0, a1, a2, a3); }
XI_ORIG int TexMgr_FindByName_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1003AEA0u))(a0, a1, a2, a3); }
XI_ORIG int CYyTexMng_FindByNameInResource(void * a0, void * a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, void *, int)>(XI_GAME(0x1003AF10u))(a0, a1, a2); }
XI_ORIG _DWORD * sub_1003B080(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1003B080u))(a0); }
XI_ORIG int CYyVbMng_AcquireVertexBuffer(void * a0, int a1, unsigned int a2, unsigned int a3, D3DPOOL a4, void * a5, char a6) { return reinterpret_cast<int (__thiscall *)(void *, int, unsigned int, unsigned int, D3DPOOL, void *, char)>(XI_GAME(0x1003B210u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG _DWORD * CYyVbMng_ReleaseVertexBuffer(void * a0) { return reinterpret_cast<_DWORD * (__stdcall *)(void *)>(XI_GAME(0x1003B3C0u))(a0); }
XI_ORIG _DWORD * CYyVb_Constructor(void * a0, int a1, int a2, int a3, int a4, int a5) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int, int, int, int, int)>(XI_GAME(0x1003B450u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void CYyVb_Destroy(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1003B4A0u))(a0); }
XI_ORIG void VertexBufferList_Register(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1003B4D0u))(a0); }
XI_ORIG void FUN_1003b4e0() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x1003B4E0u))(); }
XI_ORIG void * CMoAttachments_Construct(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1003B540u))(a0); }
XI_ORIG void ReleaseSubObjectIfPresent(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1003B5C0u))(a0); }
XI_ORIG void ReleaseSubObjectIfPresent_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1003B5C0u))(a0); }
XI_ORIG int CMoAttachments_Reset(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003B640u))(a0); }
XI_ORIG undefined4 CMoAttachments_Reset_v1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1003B640u))(a0); }
XI_ORIG undefined4 CMoAttachments_ResetAll(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1003B690u))(a0); }
XI_ORIG void CMoAttachments_ResetAll_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1003B690u))(a0); }
XI_ORIG int CMoAttachments_GetCaster(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003B6D0u))(a0); }
XI_ORIG undefined8 CMoAttachments_GetCaster_v1(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x1003B6D0u))(a0); }
XI_ORIG int CMoAttachments_GetTarget(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003B700u))(a0); }
XI_ORIG undefined8 CMoAttachments_GetTarget_v1(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x1003B700u))(a0); }
XI_ORIG int CMoAttachments_ClearCaster(void * a0, char a1) { return reinterpret_cast<int (__thiscall *)(void *, char)>(XI_GAME(0x1003B750u))(a0, a1); }
XI_ORIG void CMoAttachments_ClearCaster_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1003B750u))(a0, a1); }
XI_ORIG int CMoAttachments_ClearTarget(void * a0, char a1) { return reinterpret_cast<int (__thiscall *)(void *, char)>(XI_GAME(0x1003B780u))(a0, a1); }
XI_ORIG void CMoAttachments_ClearTarget_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1003B780u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_SetCaster(void * a0, void * a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *)>(XI_GAME(0x1003B7B0u))(a0, a1); }
XI_ORIG void CMoAttachments_SetCaster_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1003B7B0u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_SetTarget(void * a0, void * a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *)>(XI_GAME(0x1003B830u))(a0, a1); }
XI_ORIG undefined4 CMoAttachments_SetTarget_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1003B830u))(a0, a1); }
XI_ORIG int CMoAttachments_FindKindInChain(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003B8B0u))(a0); }
XI_ORIG int CMoAttachments_FindKindFromRoot(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003B900u))(a0); }
XI_ORIG long double sub_1003B980(void * a0, void * a1, void * a2) { return reinterpret_cast<long double (__cdecl *)(void *, void *, void *)>(XI_GAME(0x1003B980u))(a0, a1, a2); }
XI_ORIG float * sub_1003BA20(void * a0, int a1, void * a2, void * a3, int a4) { return reinterpret_cast<float * (__cdecl *)(void *, int, void *, void *, int)>(XI_GAME(0x1003BA20u))(a0, a1, a2, a3, a4); }
XI_ORIG int CMoAttachments_SetMappedSlot(int a0, int a1, int a2) { return reinterpret_cast<int (__cdecl *)(int, int, int)>(XI_GAME(0x1003BD90u))(a0, a1, a2); }
XI_ORIG long double YmGenerater_BuildAttachMatrix(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<long double (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1003BE10u))(a0, a1, a2, a3); }
XI_ORIG void CMoAttachments_NotifyCaster(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1003C6E0u))(a0); }
XI_ORIG void CMoAttachments_NotifyTarget(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1003C700u))(a0); }
XI_ORIG void Unit1003C6E0_CopyFields(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1003C740u))(a0, a1); }
XI_ORIG char ** CMoCamera_GetRuntimeClass() { return reinterpret_cast<char ** (__cdecl *)()>(XI_GAME(0x1003C880u))(); }
XI_ORIG char CMoCameraTask_Destructor(void * a0, int a1) { return reinterpret_cast<char (__fastcall *)(void *, int)>(XI_GAME(0x1003D030u))(a0, a1); }
XI_ORIG int Sky_DrawGradientDome(float a0, float a1, int a2, void * a3, float a4) { return reinterpret_cast<int (__cdecl *)(float, float, int, void *, float)>(XI_GAME(0x1003D640u))(a0, a1, a2, a3, a4); }
XI_ORIG void CMoCelestial_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1003D980u))(a0); }
XI_ORIG void Sky_DestroyDomeShaderStatic() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1003DB50u))(); }
XI_ORIG void Sky_DrawDomeViewCentered() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1003DB60u))(); }
XI_ORIG void Sky_DestroyViewCenteredShaderStatic() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1003DDF0u))(); }
XI_ORIG unsigned int Color_SetFromU32SwapRB(void * a0, unsigned int a1) { return reinterpret_cast<unsigned int (__thiscall *)(void *, unsigned int)>(XI_GAME(0x1003E280u))(a0, a1); }
XI_ORIG char Color_CopySwapRB(void * a0, void * a1) { return reinterpret_cast<char (__thiscall *)(void *, void *)>(XI_GAME(0x1003E2A0u))(a0, a1); }
XI_ORIG int YmElem_DrawD3ASegment(int a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(int, int, int, int)>(XI_GAME(0x1003E910u))(a0, a1, a2, a3); }
XI_ORIG int YmElem_SetD3ADrawState(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003E9A0u))(a0); }
XI_ORIG void unknown_libname_20() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1003EB00u))(); }
XI_ORIG void unknown_libname_21() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1003EB10u))(); }
XI_ORIG void unknown_libname_22() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1003EB20u))(); }
XI_ORIG char CMoVuAnmElem_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1003EF20u))(a0); }
XI_ORIG void unknown_libname_25() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1003F890u))(); }
XI_ORIG _DWORD * CMoDistMorphElem_ComputeMorphHandle(int a0, int a1, int a2) { return reinterpret_cast<_DWORD * (__thiscall *)(int, int, int)>(XI_GAME(0x1003F8A0u))(a0, a1, a2); }
XI_ORIG char CXi471011C8_VT_1032AC90_Dtor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1003FB10u))(a0); }
XI_ORIG undefined8 CXi471011C8_VT_1032AC90_Ctor(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x1003FB70u))(a0); }
XI_ORIG uint8_t CMoDistModelElem_IsDistortedMode(void * a0) { return reinterpret_cast<uint8_t (__thiscall *)(void *)>(XI_GAME(0x1003FCB0u))(a0); }
XI_ORIG char * CMoDistModelElem_GetVertexBase(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x1003FCE0u))(a0); }
XI_ORIG int CMoDistModelElem_GetVertexBase_v1(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003FCE0u))(a0); }
XI_ORIG char * CMoDistModelElem_GetDataEnd(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x1003FCF0u))(a0); }
XI_ORIG int CMoDistModelElem_GetDataEnd_v1(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003FCF0u))(a0); }
XI_ORIG void unknown_libname_33() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10040B50u))(); }
XI_ORIG void unknown_libname_34() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10040B60u))(); }
XI_ORIG void unknown_libname_35() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10040B70u))(); }
XI_ORIG void unknown_libname_36() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10040B80u))(); }
XI_ORIG void unknown_libname_37() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10040B90u))(); }
XI_ORIG char CXi471011C8_VT_1032AD80_Dtor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10041110u))(a0); }
XI_ORIG void Font_DrawProjectedText3D(int a0, int a1, int a2, int a3, int a4) { reinterpret_cast<void (__cdecl *)(int, int, int, int, int)>(XI_GAME(0x10041C20u))(a0, a1, a2, a3, a4); }
XI_ORIG _DWORD * CMoOtTask_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x100425F0u))(a0, a1); }
XI_ORIG void unknown_libname_46() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10042CD0u))(); }
XI_ORIG void unknown_libname_47() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10042CE0u))(); }
XI_ORIG void nullsub_136() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10042CF0u))(); }
XI_ORIG char CMoDistMorphElem_ValidateMorphData(int a0, int a1, int a2, int a3) { return reinterpret_cast<char (__stdcall *)(int, int, int, int)>(XI_GAME(0x10042D00u))(a0, a1, a2, a3); }
XI_ORIG void unknown_libname_49() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10043310u))(); }
XI_ORIG void StaticDtor_StateBlock_1047BB7C() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100439B0u))(); }
XI_ORIG void * CMoTask_VT_1032AF60_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<void * (__thiscall *)(void *, char)>(XI_GAME(0x10043A90u))(a0, a1); }
XI_ORIG char CMoTask_VT_1032AF60_Destructor(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x10043AB0u))(a0); }
XI_ORIG int * CMoTask_VT_1032AFA8_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<int * (__thiscall *)(void *, char)>(XI_GAME(0x10043C90u))(a0, a1); }
XI_ORIG char CMoTask_VT_1032AFA8_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10043CB0u))(a0); }
XI_ORIG void Unit10043DD0_ToggleByte45(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10043DD0u))(a0); }
XI_ORIG int Unit10043DD0_ReturnZero() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10043DE0u))(); }
XI_ORIG undefined4 Unit10043DD0_ReturnZero_v1() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10043DE0u))(); }
XI_ORIG char * DynVB_Constructor(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10043DF0u))(a0); }
XI_ORIG undefined8 DynVB_Constructor_v1(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x10043DF0u))(a0); }
XI_ORIG void DynVB_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10043E40u))(a0); }
XI_ORIG int DynVB_Create(void * a0, int a1, unsigned int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, unsigned int, int)>(XI_GAME(0x10043E50u))(a0, a1, a2, a3); }
XI_ORIG void Unit10043E40_ReturnBuffersToPool(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10043F00u))(a0); }
XI_ORIG int VertexBatch_BindAndDraw(void * a0, int a1, int a2, int a3, unsigned int a4, int a5) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, unsigned int, int)>(XI_GAME(0x10043F40u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void VertexBatch_BindAndDraw_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10043F40u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int DynVB_Reserve(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10044010u))(a0, a1); }
XI_ORIG void * DynVB_Lock(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10044130u))(a0); }
XI_ORIG int DynVB_Unlock(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100441A0u))(a0); }
XI_ORIG void CameraCache_CacheViewProjection(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100441C0u))(a0); }
XI_ORIG int Gfx_SetViewMatrixFromField10(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10044210u))(a0); }
XI_ORIG int Gfx_SetViewMatrixFromField90(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10044230u))(a0); }
XI_ORIG _DWORD * Mo_InitSharedRenderResources() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x10044270u))(); }
XI_ORIG void Unit10043E40_ReleaseResourcesOf(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100446A0u))(a0); }
XI_ORIG undefined8 CMoElem_Constructor(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x10044890u))(a0); }
XI_ORIG char CMoElemFamily_SharedCleanup(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10044970u))(a0); }
XI_ORIG void CMoElemFamily_SharedCleanup_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10044970u))(a0); }
XI_ORIG undefined4 CMoElem_SetSourceAt0FC(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10044AA0u))(a0, a1); }
XI_ORIG undefined4 CMoElem_AdvanceTimer(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10044AE0u))(a0); }
XI_ORIG float * Unit10044C40_ComputeWorldPos(int a0, void * a1) { return reinterpret_cast<float * (__thiscall *)(int, void *)>(XI_GAME(0x10044C40u))(a0, a1); }
XI_ORIG undefined4 Unit10044C40_ComputeWorldPos_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10044C40u))(a0, a1); }
XI_ORIG undefined4 Unit10044C40_IsActive(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10044CF0u))(a0); }
XI_ORIG void Unit10044C40_UpdateTransform(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10044D40u))(a0); }
XI_ORIG undefined4 YmElem_ComputeViewDepthKey(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10044F10u))(a0); }
XI_ORIG undefined4 FUN_10045190(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10045190u))(a0, a1, a2, a3); }
XI_ORIG int CMoElem_InvertInPlace(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x10045800u))(a0); }
XI_ORIG undefined4 CMoElem_CheckAt138ThenCall(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100458F0u))(a0); }
XI_ORIG unsigned long long YmElem_BuildDrawMatrixAndTest(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10045920u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG int Unit10044C40_GetFlaggedMatrix(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10045C50u))(a0); }
XI_ORIG int Unit10044C40_GetFlaggedMatrix_v1(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10045C50u))(a0); }
XI_ORIG int RegisterWithProcessor(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10045D40u))(a0); }
XI_ORIG void RegisterWithProcessor_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10045D40u))(a0); }
XI_ORIG void Zone_UpdateFogAmbient() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10046380u))(); }
XI_ORIG int Fog_SetRenderStates() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100464D0u))(); }
XI_ORIG int sub_10046550(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10046550u))(a0, a1); }
XI_ORIG void unknown_libname_53() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100472D0u))(); }
XI_ORIG void unknown_libname_54() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100473E0u))(); }
XI_ORIG int CMoElem_GetActiveField0A4(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100473F0u))(a0); }
XI_ORIG int CMoElem_GetActiveField0A4_v1(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100473F0u))(a0); }
XI_ORIG int CMoElem_GetAddr78(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10047450u))(a0); }
XI_ORIG int CMoElem_GetAddr84(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10047490u))(a0); }
XI_ORIG char CMoSkeletonElem_Destructor(int a0, int a1) { return reinterpret_cast<char (__fastcall *)(int, int)>(XI_GAME(0x10047900u))(a0, a1); }
XI_ORIG int CMoGeneratorClone_VT_1032B114_Constructor(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10047F80u))(a0); }
XI_ORIG void * CMoGeneratorClone_VT_1032B114_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<void * (__thiscall *)(void *, char)>(XI_GAME(0x10047FE0u))(a0, a1); }
XI_ORIG int CMoGeneratorClone_VT_1032B114_Destructor(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10048000u))(a0); }
XI_ORIG void CMoGeneratorClone_VT_1032B114_Destructor_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10048000u))(a0); }
XI_ORIG undefined4 FUN_10048100(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x10048100u))(a0); }
XI_ORIG char CMoGeneratorClone_UnlinkElemListA(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(XI_GAME(0x10048140u))(a0, a1); }
XI_ORIG void FUN_10048190(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x10048190u))(a0); }
XI_ORIG char CMoGeneratorClone_UnlinkElemListB(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(XI_GAME(0x100481D0u))(a0, a1); }
XI_ORIG uint32_t * FUN_100492b0(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100492B0u))(a0, a1); }
XI_ORIG _DWORD * CMoGeneratorClone_FindCmdByType(void * a0, int a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int)>(XI_GAME(0x100492F0u))(a0, a1); }
XI_ORIG uint32_t * CMoGeneratorClone_FindCmdByType_v1(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100492F0u))(a0, a1); }
XI_ORIG int Unit10048000_GetShort22ViaPtrE4(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100493B0u))(a0); }
XI_ORIG int CMoGeneratorClone_ResolveTarget(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10049570u))(a0); }
XI_ORIG undefined4 CMoGeneratorClone_ResolveTarget_v1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10049570u))(a0); }
XI_ORIG unsigned long long FUN_100496f0(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100496F0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG uint8_t sub_100496F0(int a0, int a1) { return reinterpret_cast<uint8_t (__thiscall *)(int, int)>(XI_GAME(0x100496F0u))(a0, a1); }
XI_ORIG undefined4 FUN_1004a980(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1004A980u))(a0, a1); }
XI_ORIG void CMoGeneratorClone_ExecuteCmdList(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1004E720u))(a0, a1); }
XI_ORIG __int16 SharedConditionalStateReset(int a0) { return reinterpret_cast<__int16 (__thiscall *)(int)>(XI_GAME(0x100531D0u))(a0); }
XI_ORIG __int16 CMoTask_Restart(int a0) { return reinterpret_cast<__int16 (__thiscall *)(int)>(XI_GAME(0x10053370u))(a0); }
XI_ORIG undefined4 CMoTask_Restart_v1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10053370u))(a0); }
XI_ORIG int CMoGeneratorClone_CopyParams(int a0, int a1) { return reinterpret_cast<int (__thiscall *)(int, int)>(XI_GAME(0x10053450u))(a0, a1); }
XI_ORIG undefined4 * CMoGeneratorClone_CreateInstance(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 * (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100536C0u))(a0, a1, a2, a3); }
XI_ORIG char * CMoGeneratorClone_VT_1032B69C_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<char * (__thiscall *)(void *, char)>(XI_GAME(0x10053840u))(a0, a1); }
XI_ORIG int Generator_ClearSubobjectOrSetFlag(int a0, char a1) { return reinterpret_cast<int (__thiscall *)(int, char)>(XI_GAME(0x10053890u))(a0, a1); }
XI_ORIG unsigned long long Generator_ClearSubobjectOrSetFlag_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10053890u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Generator_ClearSubobjectOrSetFlag_v2(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10053890u))(a0, a1); }
XI_ORIG char YmGenerater_Stop(int a0, void * a1) { return reinterpret_cast<char (__thiscall *)(int, void *)>(XI_GAME(0x100538E0u))(a0, a1); }
XI_ORIG unsigned long long YmGenerater_Stop_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100538E0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 YmGenerater_Stop_v2(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100538E0u))(a0, a1); }
XI_ORIG uint8_t Generator_IsFlag400OrField22Zero(void * a0) { return reinterpret_cast<uint8_t (__thiscall *)(void *)>(XI_GAME(0x100539E0u))(a0); }
XI_ORIG undefined4 Generator_IsFlag400OrField22Zero_v1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100539E0u))(a0); }
XI_ORIG undefined4 Unit10053860_StopGenerators(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10053A00u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 YmGenerater_UpdateAttach(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10053C80u))(a0, a1, a2); }
XI_ORIG unsigned long long YmGenerater_UpdateAttachIfPresent(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10053DD0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG char CMoTask_VT_1032B654_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100541C0u))(a0); }
XI_ORIG char ** CMoGeneratorClone_GetRuntimeClass() { return reinterpret_cast<char ** (__cdecl *)()>(XI_GAME(0x100542E0u))(); }
XI_ORIG int CMoGeneratorClone_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100542F0u))(a0); }
XI_ORIG __int16 CMoGeneratorClone_Kill(int a0) { return reinterpret_cast<__int16 (__cdecl *)(int)>(XI_GAME(0x100543A0u))(a0); }
XI_ORIG long double Unit100544A0_EvaluateWithFlag34(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<long double (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x100544D0u))(a0, a1, a2); }
XI_ORIG long double Unit100544A0_Evaluate(void * a0, XI_ANY a1) { return reinterpret_cast<long double (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10054500u))(a0, a1); }
XI_ORIG long double Unit100544A0_EvaluateEaseIn(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<long double (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x100546F0u))(a0, a1, a2); }
XI_ORIG long double Unit100544A0_EvaluateEaseInOut(void * a0, XI_ANY a1) { return reinterpret_cast<long double (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100547A0u))(a0, a1); }
XI_ORIG char CMoLfdElem_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100548C0u))(a0); }
XI_ORIG void nullsub_130() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10054F70u))(); }
XI_ORIG void Unit100548D0_Init(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10054FC0u))(a0); }
XI_ORIG int Vt1032B6D8_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10055000u))(a0); }
XI_ORIG unsigned long long Unit100548D0_FreeSplineSet(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100550F0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_10055130(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10055130u))(a0, a1, a2); }
XI_ORIG int FUN_10055160(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10055160u))(a0); }
XI_ORIG _DWORD * LinkNode_Init(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10055240u))(a0); }
XI_ORIG void LinkNode_Init_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10055240u))(a0); }
XI_ORIG int LinkNode_Unlink(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10055250u))(a0); }
XI_ORIG void * CMoOT_Constructor(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x100552A0u))(a0); }
XI_ORIG _DWORD * CMoOT_Destructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x100552E0u))(a0); }
XI_ORIG void Unit100552F0_ClearLink(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100552F0u))(a0); }
XI_ORIG void Unit100552F0_Init(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10055300u))(a0, a1, a2, a3); }
XI_ORIG void FUN_100554b0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100554B0u))(a0); }
XI_ORIG void Unit100552F0_Free(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10055520u))(a0); }
XI_ORIG undefined8 CMoOtTask_Constructor(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x10055570u))(a0); }
XI_ORIG undefined4 FUN_100555b0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100555B0u))(a0, a1); }
XI_ORIG char sub_100555B0(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(XI_GAME(0x100555B0u))(a0, a1); }
XI_ORIG _DWORD * CMoOt_InitSystem() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x100555D0u))(); }
XI_ORIG void CMoOt_Update() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10055620u))(); }
XI_ORIG void CMoOtTask_Clear(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10055640u))(a0); }
XI_ORIG void FUN_10055650(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10055650u))(a0); }
XI_ORIG long double FUN_10055ad0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<long double (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10055AD0u))(a0, a1); }
XI_ORIG long double FUN_10056160(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<long double (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10056160u))(a0, a1, a2); }
XI_ORIG char * CMoSphRes_FixupOffsets(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10056220u))(a0); }
XI_ORIG char CMoPointLightProgElem_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10056390u))(a0); }
XI_ORIG char ** CMoPathDriveActorTask_GetRuntimeClass() { return reinterpret_cast<char ** (__cdecl *)()>(XI_GAME(0x10056940u))(); }
XI_ORIG char ** CMoLockColorDriveTask_GetRuntimeClass() { return reinterpret_cast<char ** (__cdecl *)()>(XI_GAME(0x10056960u))(); }
XI_ORIG char ** CMoLockLookAtDriveTask_GetRuntimeClass() { return reinterpret_cast<char ** (__cdecl *)()>(XI_GAME(0x10056980u))(); }
XI_ORIG char ** CMoActorColorDriveTask_GetRuntimeClass() { return reinterpret_cast<char ** (__cdecl *)()>(XI_GAME(0x100569A0u))(); }
XI_ORIG char ** CMoActorRotationDriveTask_GetRuntimeClass() { return reinterpret_cast<char ** (__cdecl *)()>(XI_GAME(0x100569C0u))(); }
XI_ORIG int * Scheduler_Execute(void * a0, void * a1, void * a2, int a3, int a4) { return reinterpret_cast<int * (__thiscall *)(void *, void *, void *, int, int)>(XI_GAME(0x10056C70u))(a0, a1, a2, a3, a4); }
XI_ORIG unsigned long long Scheduler_Execute_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) {
    uint32_t xs[4] = {a1.bits, a2.bits, a3.bits, a4.bits}; uint32_t ra = a5.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 12(%%esi)\n\t""pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10056C70u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Scheduler_RemoveTasksFor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10056EB0u))(a0); }
XI_ORIG undefined8 Scheduler_Kill(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10056EF0u))(a0, a1, a2); }
XI_ORIG void * CMoSchedularTask_Construct(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10057270u))(a0, a1, a2, a3); }
XI_ORIG void CMoSchedularTask_ResetFlags(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100573B0u))(a0); }
XI_ORIG void FUN_100573e0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100573E0u))(a0, a1, a2, a3); }
XI_ORIG char CMoSchedularTask_Destructor(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x100574A0u))(a0); }
XI_ORIG uint32_t FUN_10057c20(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x10057C20u))(a0); }
XI_ORIG undefined4 CMoSchedularTask_Interpret(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10057FB0u))(a0); }
XI_ORIG void * FUN_1005e100(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1005E100u))(a0); }
XI_ORIG undefined4 FUN_1005ed30(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1005ED30u))(a0, a1, a2); }
XI_ORIG void FUN_1005ed90(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1005ED90u))(a0, a1); }
XI_ORIG void FUN_1005edc0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1005EDC0u))(a0, a1); }
XI_ORIG void FUN_1005ee90(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1005EE90u))(a0, a1); }
XI_ORIG void FUN_1005eec0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1005EEC0u))(a0, a1); }
XI_ORIG undefined4 CMoPathDriveActorTask_ApplyFraction(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1005EEF0u))(a0, a1); }
XI_ORIG _DWORD * CMoPathDriveActorTask_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x1005F2C0u))(a0, a1); }
XI_ORIG int CMoLockColorDriveTask_ScalarDeletingDtor(int a0, char a1) { return reinterpret_cast<int (__thiscall *)(int, char)>(XI_GAME(0x1005F3F0u))(a0, a1); }
XI_ORIG _DWORD * CMoLockLookAtDriveTask_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x1005F660u))(a0, a1); }
XI_ORIG int CMoActorColorDriveTask_ScalarDeletingDtor(int a0, char a1) { return reinterpret_cast<int (__thiscall *)(int, char)>(XI_GAME(0x1005F990u))(a0, a1); }
XI_ORIG tagRECT * CMoActorRotationDriveTask_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<tagRECT * (__thiscall *)(void *, char)>(XI_GAME(0x1005FE30u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_VT_1032B940_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10060190u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BAF0_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x100602D0u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BB38_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x100608B0u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BBAC_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10060BB0u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BBF4_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10060E60u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BC68_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10061010u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BC84_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10061140u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BCCC_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10061300u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BD14_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x100614B0u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BD5C_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x100616B0u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BDA4_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10061830u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BDEC_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10061A00u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BE34_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10061B80u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BE7C_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10061D00u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BEC4_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10061EA0u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BF0C_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10062090u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BF54_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10062210u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BF9C_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x100623B0u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_VT_1032BFE4_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10062550u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_VT_1032C02C_VectorDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10062700u))(a0, a1); }
XI_ORIG void AfterimageTask_SetCaster(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10062830u))(a0, a1); }
XI_ORIG void AfterimageTask_SetTarget(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10062880u))(a0, a1); }
XI_ORIG undefined4 FUN_10062de0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10062DE0u))(a0, a1); }
XI_ORIG undefined8 AfterimageTask_Execute(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10062E20u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined8 AfterimageTask_Kill(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10062E80u))(a0, a1, a2, a3); }
XI_ORIG uint32_t FUN_10062ed0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10062ED0u))(a0, a1, a2, a3); }
XI_ORIG undefined4 CMoSpline_Construct(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10063460u))(a0); }
XI_ORIG _DWORD * CMoSpline_Destructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x100634A0u))(a0); }
XI_ORIG void CMoSpline_Destructor_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100634A0u))(a0); }
XI_ORIG void CMoSpline_Init(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100634B0u))(a0, a1, a2, a3, a4); }
XI_ORIG void CMoSpline_FreeBuffers(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10063550u))(a0); }
XI_ORIG void FUN_100635b0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100635B0u))(a0); }
XI_ORIG void FUN_10063a60(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10063A60u))(a0, a1, a2); }
XI_ORIG int CMoSpline_ReleaseRef(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10063B20u))(a0); }
XI_ORIG void CMoSpline_ReleaseRef_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10063B20u))(a0); }
XI_ORIG _DWORD * CMoSpline_GetWrapped(void * a0, int a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int)>(XI_GAME(0x10063B50u))(a0, a1); }
XI_ORIG int * CMoSpline_GetWrapped_v1(void * a0, XI_ANY a1) { return reinterpret_cast<int * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10063B50u))(a0, a1); }
XI_ORIG int CMoSpline_GetData(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10063B70u))(a0); }
XI_ORIG undefined4 CMoSpline_EnsureSamples(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10063B80u))(a0); }
XI_ORIG void CMoSpline_SetValue(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10063C30u))(a0, a1, a2); }
XI_ORIG _DWORD * MoProgElem_FreeBuffer40(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10063C60u))(a0); }
XI_ORIG unsigned long long FUN_100683d0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100683D0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Unit10064190_SetDwordValue(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100684F0u))(a0, a1); }
XI_ORIG void Unit10064190_AssignTyped(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10068590u))(a0, a1, a2); }
XI_ORIG char CMoDX_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10069D70u))(a0); }
XI_ORIG _BYTE * CMoDX_Initialize(int a0, int a1) { return reinterpret_cast<_BYTE * (__thiscall *)(int, int)>(XI_GAME(0x10069DD0u))(a0, a1); }
XI_ORIG int Unit10069DD0_Nop(_DWORD a0) { return reinterpret_cast<int (__thiscall *)(_DWORD)>(XI_GAME(0x10069E90u))(a0); }
XI_ORIG void FUN_10069ee0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10069EE0u))(a0); }
XI_ORIG int sub_1006A060(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(XI_GAME(0x1006A060u))(a0, a1, a2, a3); }
XI_ORIG int sub_1006A070(int a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(int, int, int, int)>(XI_GAME(0x1006A070u))(a0, a1, a2, a3); }
XI_ORIG int sub_1006A0E0(int a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(int, int, int)>(XI_GAME(0x1006A0E0u))(a0, a1, a2); }
XI_ORIG _DWORD * CMoDX_CreateTexture(void * a0, unsigned __int16 a1, unsigned __int16 a2, void * a3) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, unsigned __int16, unsigned __int16, void *)>(XI_GAME(0x1006A150u))(a0, a1, a2, a3); }
XI_ORIG undefined4 Camera_SetupMatrices(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1006A180u))(a0, a1, a2, a3, a4); }
XI_ORIG int sub_1006A420(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1006A420u))(a0); }
XI_ORIG int CMoDX_SetWorldMatrix(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x1006A480u))(a0, a1); }
XI_ORIG int CMoDX_GetWorldMatrix(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x1006A4A0u))(a0, a1); }
XI_ORIG int CMoDX_PushViewMatrix(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1006A4C0u))(a0); }
XI_ORIG int CMoDX_PopViewMatrix(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x1006A4F0u))(a0); }
XI_ORIG int CMoDX_SetViewMatrix(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x1006A520u))(a0, a1); }
XI_ORIG void CMoDX_SetViewMatrix_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1006A520u))(a0, a1); }
XI_ORIG void Dx_GetCachedTransform2(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1006A540u))(a0, a1); }
XI_ORIG int Gfx_CalcFVFStride(unsigned int a0) { return reinterpret_cast<int (__cdecl *)(unsigned int)>(XI_GAME(0x1006A560u))(a0); }
XI_ORIG _DWORD * Attach_SetTranslation(void * a0, void * a1) { return reinterpret_cast<_DWORD * (__cdecl *)(void *, void *)>(XI_GAME(0x1006AD30u))(a0, a1); }
XI_ORIG int Processor_ApplyPair(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1006AD50u))(a0, a1); }
XI_ORIG void Unit10069DD0_SetInvalid(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1006AD90u))(a0); }
XI_ORIG exception * fn_1006AD90(void * a0) { return reinterpret_cast<exception * (__thiscall *)(void *)>(XI_GAME(0x1006AD90u))(a0); }
XI_ORIG int Unit10069DD0_Insert(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1006ADF0u))(a0); }
XI_ORIG int StateBlock_ApplyRSTSS(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1006AE30u))(a0, a1, a2); }
XI_ORIG int StateBlockHolder_ApplyRenderStates(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x1006AF00u))(a0, a1); }
XI_ORIG int StateBlock_ApplyTSS(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1006AF90u))(a0, a1); }
XI_ORIG _DWORD * CMoDxStatusMng_Constructor(int a0) { return reinterpret_cast<_DWORD * (__thiscall *)(int)>(XI_GAME(0x1006B050u))(a0); }
XI_ORIG void CMoDxStatusMng_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1006B090u))(a0); }
XI_ORIG _DWORD * VBPool_Constructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1006B0C0u))(a0); }
XI_ORIG uint32_t Unit1006B0C0_Insert(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1006B120u))(a0, a1); }
XI_ORIG int Unit1006B0C0_FindMatch(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1006B1D0u))(a0, a1); }
XI_ORIG _DWORD * CMoDX_CreateAndInitSystem(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1006B2B0u))(a0); }
XI_ORIG int sub_1006BC10(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1006BC10u))(a0, a1); }
XI_ORIG int sub_1006BC80(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1006BC80u))(a0); }
XI_ORIG int Rand_SeedMT19937(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x1006BD90u))(a0); }
XI_ORIG void Rand_SeedMT19937_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1006BD90u))(a0); }
XI_ORIG uint32_t Unit1006B3B0_Next() { return reinterpret_cast<uint32_t (__cdecl *)()>(XI_GAME(0x1006BE20u))(); }
XI_ORIG _DWORD * Mem_AllocWithHeader12(int a0, int a1) { return reinterpret_cast<_DWORD * (__cdecl *)(int, int)>(XI_GAME(0x1006C0F0u))(a0, a1); }
XI_ORIG _DWORD * Mem_FreeWithHeader12(int a0) { return reinterpret_cast<_DWORD * (__cdecl *)(int)>(XI_GAME(0x1006C110u))(a0); }
XI_ORIG CMoOcclusionMng_vtbl ** CMoOcclusionMng_Constructor(void * a0) { return reinterpret_cast<CMoOcclusionMng_vtbl ** (__thiscall *)(void *)>(XI_GAME(0x1006C140u))(a0); }
XI_ORIG int CMoOcclusionMng_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1006C1E0u))(a0); }
XI_ORIG _DWORD * j_OcclusionMng_CreateTarget(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1006C210u))(a0); }
XI_ORIG _DWORD * OcclusionMng_CreateTarget(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1006C220u))(a0); }
XI_ORIG void OcclusionMng_CreateTarget_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1006C220u))(a0); }
XI_ORIG int OcclusionQuery_Init(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x1006C260u))(a0); }
XI_ORIG void OcclusionQuery_Init_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1006C260u))(a0); }
XI_ORIG int OcclusionQuery_Release(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1006C270u))(a0); }
XI_ORIG int Occlusion_QueryScreenRect(void * a0, void * a1, int a2, int a3, int a4, int a5, int a6, int a7, char a8, int a9) { return reinterpret_cast<int (__thiscall *)(void *, void *, int, int, int, int, int, int, char, int)>(XI_GAME(0x1006C280u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9); }
XI_ORIG int Occlusion_QueryScreenRectImpl(void * a0, void * a1, int a2, int a3, int a4, int a5, int a6, int a7, char a8, int a9) { return reinterpret_cast<int (__thiscall *)(void *, void *, int, int, int, int, int, int, char, int)>(XI_GAME(0x1006C300u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9); }
XI_ORIG int DrawScreenProbe(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8) { return reinterpret_cast<int (__stdcall *)(int, int, int, int, int, int, int, int, int)>(XI_GAME(0x1006C530u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG void unknown_libname_59() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1006C890u))(); }
XI_ORIG void unknown_libname_60() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1006C8A0u))(); }
XI_ORIG int sub_1006C8B0(void * a0, char a1) { return reinterpret_cast<int (__thiscall *)(void *, char)>(XI_GAME(0x1006C8B0u))(a0, a1); }
XI_ORIG int OcclusionMng_Prepare(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1006C9B0u))(a0); }
XI_ORIG undefined4 Unit1006C210_IsFloatClassAccepted(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1006CA20u))(a0); }
XI_ORIG undefined4 Unit1006C210_AreAll16FloatsAccepted(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1006CA60u))(a0); }
XI_ORIG _DWORD * CreateProcessorInstance() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x1006CB50u))(); }
XI_ORIG void FUN_1006ce40() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x1006CE40u))(); }
XI_ORIG void nullsub_44() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1006CF90u))(); }
XI_ORIG float * CMoProcessor_AddScaled3(void * a0, void * a1, float a2) { return reinterpret_cast<float * (__stdcall *)(void *, void *, float)>(XI_GAME(0x1006D3B0u))(a0, a1, a2); }
XI_ORIG void CMoProcessor_TransposeInPlace(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x1006D3E0u))(a0); }
XI_ORIG float * CMoProcessor_Vec4TransformB(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__stdcall *)(void *, void *, void *)>(XI_GAME(0x1006D7E0u))(a0, a1, a2); }
XI_ORIG float * CMoProcessor_Vec4Transform(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__stdcall *)(void *, void *, void *)>(XI_GAME(0x1006D870u))(a0, a1, a2); }
XI_ORIG float * CMoProcessor_Vec3Transform(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__stdcall *)(void *, void *, void *)>(XI_GAME(0x1006D900u))(a0, a1, a2); }
XI_ORIG float * CMoProcessor_Lerp3(void * a0, void * a1, void * a2, float a3) { return reinterpret_cast<float * (__stdcall *)(void *, void *, void *, float)>(XI_GAME(0x1006D980u))(a0, a1, a2, a3); }
XI_ORIG CMoProcessor * CMoProcessor_CalcScreenBounds(void * a0, void * a1, int a2, int a3, void * a4) { return reinterpret_cast<CMoProcessor * (__stdcall *)(void *, void *, int, int, void *)>(XI_GAME(0x1006E970u))(a0, a1, a2, a3, a4); }
XI_ORIG float * CMoProcessor_ProjectVertsAndBounds(void * a0, void * a1, int a2, void * a3, void * a4, float a5, float a6) { return reinterpret_cast<float * (__stdcall *)(void *, void *, int, void *, void *, float, float)>(XI_GAME(0x1006EB70u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG CMoProcessor * CMoProcessor_CalcScreenBoundsAndVisible(void * a0, void * a1, void * a2, int a3, int a4, int a5, float a6, float a7) { return reinterpret_cast<CMoProcessor * (__thiscall *)(void *, void *, void *, int, int, int, float, float)>(XI_GAME(0x1006ED10u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG void * CMoTask_InitVariant12(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10070F40u))(a0); }
XI_ORIG char CMoTask_VT_1032C490_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10070F80u))(a0); }
XI_ORIG void CMoTask_InitAndRegister(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10070F90u))(a0, a1, a2, a3, a4); }
XI_ORIG __int16 ResHandle_FreeIfUnreferenced(void * a0) { return reinterpret_cast<__int16 (__thiscall *)(void *)>(XI_GAME(0x10071050u))(a0); }
XI_ORIG undefined4 ResHandle_FreeIfUnreferenced_v1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10071050u))(a0); }
XI_ORIG uint8_t ResourceHandle_IsResident(void * a0) { return reinterpret_cast<uint8_t (__fastcall *)(void *)>(XI_GAME(0x10071060u))(a0); }
XI_ORIG undefined4 ResourceHandle_IsResident_v1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10071060u))(a0); }
XI_ORIG undefined4 Unit10070F90_Create(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100710F0u))(a0, a1, a2, a3, a4); }
XI_ORIG _DWORD * CMoAttachments_VT_1032C4E8_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10071180u))(a0, a1); }
XI_ORIG void Unit10070F90_CopyField14At0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100711D0u))(a0, a1); }
XI_ORIG void Unit10070F90_CopyField14At4(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100711E0u))(a0, a1); }
XI_ORIG int * FUN_10071270(void * a0, XI_ANY a1) { return reinterpret_cast<int * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10071270u))(a0, a1); }
XI_ORIG void nullsub_171() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100719B0u))(); }
XI_ORIG unsigned int ** CMoAttachments_VT_1032C628_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<unsigned int ** (__thiscall *)(void *, char)>(XI_GAME(0x10071C10u))(a0, a1); }
XI_ORIG unsigned int ** CMoResource_VT_1032C588_ScalarDeletingDtor_10071C60(void * a0, char a1) { return reinterpret_cast<unsigned int ** (__thiscall *)(void *, char)>(XI_GAME(0x10071C60u))(a0, a1); }
XI_ORIG undefined4 * FUN_10071d80(void * a0, void * a1) { return reinterpret_cast<undefined4 * (__fastcall *)(void *, void *)>(XI_GAME(0x10071D80u))(a0, a1); }
XI_ORIG unsigned int * CMoResource_Destructor(void * a0) { return reinterpret_cast<unsigned int * (__thiscall *)(void *)>(XI_GAME(0x10071DE0u))(a0); }
XI_ORIG void FUN_10071e50() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10071E50u))(); }
XI_ORIG int * CMoResource_AddRefChain(int a0) { return reinterpret_cast<int * (__fastcall *)(int)>(XI_GAME(0x10071E90u))(a0); }
XI_ORIG unsigned long long CMoResource_AddRefChain_v1(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10071E90u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined8 CMoResource_AddRefChain_v2(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x10071E90u))(a0); }
XI_ORIG char CMoResource_ReleaseChain(int a0, int a1) { return reinterpret_cast<char (__fastcall *)(int, int)>(XI_GAME(0x10071EC0u))(a0, a1); }
XI_ORIG undefined4 CMoResource_ReleaseChain_v1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10071EC0u))(a0); }
XI_ORIG void FUN_10071f50(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10071F50u))(a0); }
XI_ORIG _DWORD * sub_10071F50(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10071F50u))(a0); }
XI_ORIG void FUN_10071f60(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10071F60u))(a0); }
XI_ORIG void FUN_10071f70(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10071F70u))(a0, a1); }
XI_ORIG undefined4 FUN_10071f80(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10071F80u))(a0); }
XI_ORIG int CMoResource_RegisterInTypeList(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10072030u))(a0); }
XI_ORIG void CMoResource_RegisterInTypeList_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10072030u))(a0); }
XI_ORIG void FUN_100720b0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100720B0u))(a0); }
XI_ORIG undefined4 FUN_10072180(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10072180u))(a0, a1); }
XI_ORIG unsigned long long CMoResource_Destroy(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100721F0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void * Unit10071E50_FindSelfLinkedNode(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10072240u))(a0); }
XI_ORIG _DWORD * CMoResource_GetParent(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10072270u))(a0); }
XI_ORIG void * CMoResource_GetParent_v1(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10072270u))(a0); }
XI_ORIG _DWORD * CMoResource_FindByTypeAndId(void * a0, void * a1, int a2, int a3) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, int, int)>(XI_GAME(0x10072290u))(a0, a1, a2, a3); }
XI_ORIG undefined8 CMoResource_FindByTypeAndId_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10072290u))(a0, a1, a2, a3); }
XI_ORIG undefined8 CMoResource_FindWorker(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100722C0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined8 FUN_100722c0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100722C0u))(a0, a1, a2, a3, a4); }
XI_ORIG unsigned long long CMoResource_FindInScope(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) {
    uint32_t xs[4] = {a0.bits, a1.bits, a2.bits, a3.bits}; uint32_t ra = a4.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 12(%%esi)\n\t""pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10072430u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG _DWORD * YmResourceFile_FindResourceUnder(int a0, void * a1, int a2, int a3) { return reinterpret_cast<_DWORD * (__thiscall *)(int, void *, int, int)>(XI_GAME(0x100724C0u))(a0, a1, a2, a3); }
XI_ORIG unsigned long long YmResourceFile_FindResourceUnder_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) {
    uint32_t xs[3] = {a1.bits, a2.bits, a3.bits}; uint32_t ra = a4.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100724C0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Unit10071E50_FindInChainByKind(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) {
    uint32_t xs[4] = {a1.bits, a2.bits, a3.bits, a4.bits}; uint32_t ra = a5.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 12(%%esi)\n\t""pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10072540u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_10072970() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x10072970u))(); }
XI_ORIG int unknown_libname_61() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100729C0u))(); }
XI_ORIG void Unit10071E50_ClearGlobal1047D138() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100729D0u))(); }
XI_ORIG char CMoTask_VT_1032C908_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100729E0u))(a0); }
XI_ORIG undefined8 Unit10072A50_Start(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10072C30u))(a0, a1, a2); }
XI_ORIG void Unit10072A50_Start_v1(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10072C30u))(a0, a1, a2); }
XI_ORIG int sub_10072DB0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10072DB0u))(a0); }
XI_ORIG int * MeshResourceMng_LoadSystemResources(int a0, int a1) { return reinterpret_cast<int * (__thiscall *)(int, int)>(XI_GAME(0x10072ED0u))(a0, a1); }
XI_ORIG int ResourceHandle_Deref(int a0) { return reinterpret_cast<int (__stdcall *)(int)>(XI_GAME(0x10072FB0u))(a0); }
XI_ORIG undefined4 ResourceHandle_Deref_v1(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x10072FB0u))(a0); }
XI_ORIG undefined4 FUN_10072fd0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10072FD0u))(a0, a1, a2); }
XI_ORIG int * sub_10072FD0(void * a0, void * a1, int a2) { return reinterpret_cast<int * (__thiscall *)(void *, void *, int)>(XI_GAME(0x10072FD0u))(a0, a1, a2); }
XI_ORIG undefined4 * CMoResource_FindOrCreateTask(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 * (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x100730E0u))(a0, a1); }
XI_ORIG undefined4 FUN_100731b0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x100731B0u))(a0, a1, a2); }
XI_ORIG int * sub_100731B0(void * a0, void * a1, int a2) { return reinterpret_cast<int * (__thiscall *)(void *, void *, int)>(XI_GAME(0x100731B0u))(a0, a1, a2); }
XI_ORIG undefined4 MeshResourceMng_ResolveTree(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10073430u))(a0, a1, a2, a3); }
XI_ORIG void CMoResource_RegisterAndMark(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x10073A60u))(a0, a1); }
XI_ORIG __int16 MoRes_FreeIfUnreferenced(void * a0) { return reinterpret_cast<__int16 (__stdcall *)(void *)>(XI_GAME(0x10073B20u))(a0); }
XI_ORIG undefined4 MoRes_FreeIfUnreferenced_v1(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x10073B20u))(a0); }
XI_ORIG undefined4 CMoResource_InitAndGetFlag(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x10073B80u))(a0); }
XI_ORIG undefined4 ResourceIter_FindById(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10073BA0u))(a0, a1, a2); }
XI_ORIG undefined8 ResourceIter_FindOrStartTask(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__stdcall *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10073D80u))(a0, a1, a2); }
XI_ORIG undefined8 ResourceIter_PopFreeNode(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10074110u))(a0, a1); }
XI_ORIG _DWORD * MeshResourceMng_AllocHandle(void * a0, void * a1, int a2) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, int)>(XI_GAME(0x10074140u))(a0, a1, a2); }
XI_ORIG void MeshResourceMng_AllocHandle_v1(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10074140u))(a0, a1, a2); }
XI_ORIG unsigned int * NodePool_ReleaseNode(void * a0, void * a1) { return reinterpret_cast<unsigned int * (__thiscall *)(void *, void *)>(XI_GAME(0x10074180u))(a0, a1); }
XI_ORIG void NodePool_ReleaseNode_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10074180u))(a0, a1); }
XI_ORIG undefined4 ResourceIter_InPoolRange(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100741E0u))(a0, a1); }
XI_ORIG void ResourceIter_SetCurrentGlobal(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10074280u))(a0); }
XI_ORIG int GetGlobal_1047D114() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10074290u))(); }
XI_ORIG void FUN_100742a0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100742A0u))(a0); }
XI_ORIG char ResourceIter_FindFirst(void * a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int, int)>(XI_GAME(0x100742B0u))(a0, a1, a2, a3, a4); }
XI_ORIG void ResourceIter_FindFirst_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100742B0u))(a0, a1, a2, a3, a4); }
XI_ORIG void ResourceIter_Start(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10074310u))(a0, a1, a2, a3, a4); }
XI_ORIG uint32_t ResourceIter_MatchCurrent(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x10074380u))(a0); }
XI_ORIG void ResourceIter_Advance(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100743D0u))(a0); }
XI_ORIG int ResourceIter_FindNext(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x100745C0u))(a0, a1); }
XI_ORIG undefined4 ResourceIter_FindNext_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100745C0u))(a0, a1); }
XI_ORIG int MoProcessor_CallVSlot49(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100746A0u))(a0); }
XI_ORIG void MoProcessor_CallVSlot49_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100746A0u))(a0); }
XI_ORIG _DWORD * CMoTask_Constructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10074740u))(a0); }
XI_ORIG undefined8 CMoTask_Constructor_v1(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x10074740u))(a0); }
XI_ORIG void * CMoTask_Constructor_v2(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10074740u))(a0); }
XI_ORIG char CMoTask_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100747C0u))(a0); }
XI_ORIG void CMoTask_Destructor_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100747C0u))(a0); }
XI_ORIG void FUN_10074850(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10074850u))(a0, a1); }
XI_ORIG void FUN_10074890(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10074890u))(a0); }
XI_ORIG int CMoTask_ResetFields(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x100748F0u))(a0); }
XI_ORIG undefined4 CMoTask_ResetFields_v1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100748F0u))(a0); }
XI_ORIG void CMoTask_ResetFields_v2(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100748F0u))(a0); }
XI_ORIG int CMoTask_RegisterWithMng(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x100749B0u))(a0, a1); }
XI_ORIG undefined4 CMoTask_RegisterWithMng_v1(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x100749B0u))(a0); }
XI_ORIG undefined8 FUN_100749f0() { return reinterpret_cast<undefined8 (__cdecl *)()>(XI_GAME(0x100749F0u))(); }
XI_ORIG void FUN_100749f0_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100749F0u))(); }
XI_ORIG undefined4 FUN_10074a00() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10074A00u))(); }
XI_ORIG void FUN_10074a00_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10074A00u))(); }
XI_ORIG int CMoTaskMng_Forward10075120(int a0) { return reinterpret_cast<int (__stdcall *)(int)>(XI_GAME(0x10074A10u))(a0); }
XI_ORIG unsigned long long FUN_10074a50(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10074A50u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void CMoTask_Release(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10074AB0u))(a0); }
XI_ORIG void FUN_10074ad0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10074AD0u))(a0, a1); }
XI_ORIG char CMoTaskMng_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10074B60u))(a0); }
XI_ORIG int CMoTaskMng_InitPool(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10074BA0u))(a0); }
XI_ORIG int sub_10074C30(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10074C30u))(a0); }
XI_ORIG void FUN_10074cc0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10074CC0u))(a0, a1, a2); }
XI_ORIG void CMoTaskMng_RemoveByKey(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10074CF0u))(a0, a1, a2); }
XI_ORIG int CMoTaskMng_AllocTask(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10074DF0u))(a0, a1); }
XI_ORIG undefined8 Unit10074CA0_FindTask(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10074F30u))(a0, a1, a2); }
XI_ORIG int Unit10074CA0_FindTask_v1(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10074F30u))(a0, a1, a2); }
XI_ORIG undefined8 CMoTaskMng_InsertIntoBucket(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10074F80u))(a0, a1, a2); }
XI_ORIG void CMoTask_Dispatch() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10075000u))(); }
XI_ORIG int CMoTaskMng_InsertAfterCursor(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10075120u))(a0, a1); }
XI_ORIG void CMoTaskMng_UnlinkTask(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100751B0u))(a0, a1); }
XI_ORIG int Gfx_InitCubeMapTextures() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10075280u))(); }
XI_ORIG int TexRef_ResolveByName(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10075520u))(a0, a1); }
XI_ORIG void TexRef_EnsureRenderTarget(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100755A0u))(a0); }
XI_ORIG int TextureHolder_BindWarped(void * a0, int a1, float a2, float a3, float a4) { return reinterpret_cast<int (__thiscall *)(void *, int, float, float, float)>(XI_GAME(0x10075700u))(a0, a1, a2, a3, a4); }
XI_ORIG void unknown_libname_63() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10075950u))(); }
XI_ORIG int TexRef_SetTexture(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10075960u))(a0, a1); }
XI_ORIG void TexRef_SetTexture_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10075960u))(a0, a1); }
XI_ORIG void unknown_libname_64() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10075C30u))(); }
XI_ORIG void CMoTask_SetRect30(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10076750u))(a0, a1); }
XI_ORIG int Arg_SplitQuoted(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10076830u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int Arg_CopyCharOrPair(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100769F0u))(a0, a1, a2); }
XI_ORIG char * Arg_StripQuotes(XI_ANY a0) { return reinterpret_cast<char * (__cdecl *)(XI_ANY)>(XI_GAME(0x10076A70u))(a0); }
XI_ORIG int preprocessArg(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10076AB0u))(a0, a1, a2); }
XI_ORIG undefined1 * getNounFromPronoun(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined1 * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10076FF0u))(a0, a1); }
XI_ORIG unsigned long long FUN_10077a00(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10077A00u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG uint8_t * Arg_FindInSortedTable(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<uint8_t * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10077AD0u))(a0, a1); }
XI_ORIG undefined4 Arg_LookupWord(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10077B40u))(a0, a1); }
XI_ORIG undefined4 Unit10074CA0_MapNameLetters(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10077C30u))(a0); }
XI_ORIG void Acter_CopyNameToBuffer(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10077CF0u))(a0); }
XI_ORIG undefined4 * Acter_GetCurrentIdRef() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x10077D40u))(); }
XI_ORIG void Acter_ClearCurrentName() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10077D70u))(); }
XI_ORIG undefined4 Acter_FindMatchingActor() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10077D80u))(); }
XI_ORIG undefined4 ChatLog_AddFormattedMessage(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10077E30u))(a0, a1); }
XI_ORIG int FUN_10077ed0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10077ED0u))(); }
XI_ORIG void Unit10074CA0_ToggleOnKey() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10077FB0u))(); }
XI_ORIG undefined thunk_FUN_100e97d0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10078090u))(); }
XI_ORIG undefined4 Unit10074CA0_BuildPacket1A_Default(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100780A0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 Unit10074CA0_BuildPacket1A(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100780D0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG int FUN_10079170(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x10079170u))(a0); }
XI_ORIG int Unit10078260_ParseNumber(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x10079A00u))(a0); }
XI_ORIG unsigned long long SeekIdByString(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) {
    uint32_t xs[4] = {a0.bits, a1.bits, a2.bits, a3.bits}; uint32_t ra = a4.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 12(%%esi)\n\t""pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $16, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1007A1C0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG int Actor_FindInRange(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1007A440u))(a0, a1, a2); }
XI_ORIG int Actor_FindByName(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1007A4C0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_1007bf30(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1007BF30u))(a0, a1, a2, a3, a4); }
XI_ORIG int Unit10078260_ToggleStatusAndSend(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1007CD60u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_1007cf80() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1007CF80u))(); }
XI_ORIG undefined4 Actor_GetObjectByName(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10080C70u))(a0); }
XI_ORIG unsigned long long Unit10078260_GetWord1047D3FC(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10080D00u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Unit10078260_IsIdNotInTable10357BB8(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10080D20u))(a0); }
XI_ORIG uint32_t Actor_IsInTable(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x10080D90u))(a0); }
XI_ORIG int Command_RunLine(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10080DC0u))(a0, a1); }
XI_ORIG unsigned long long Unit10078260_CheckCommandAllowed(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10081230u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Unit10078260_SetFlag1047D498(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10081440u))(a0); }
XI_ORIG undefined4 Unit10078260_TestBit1047D499(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10081480u))(a0); }
XI_ORIG _DWORD * ActorHandle_Constructor(int a0) { return reinterpret_cast<_DWORD * (__thiscall *)(int)>(XI_GAME(0x100814A0u))(a0); }
XI_ORIG void ActorHandle_Constructor_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100814A0u))(a0); }
XI_ORIG void ActorHandle_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100814E0u))(a0); }
XI_ORIG void ActorHandle_Destructor_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100814E0u))(a0); }
XI_ORIG undefined8 FUN_100814f0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100814F0u))(a0, a1); }
XI_ORIG unsigned int sub_100814F0(int a0, int a1) { return reinterpret_cast<unsigned int (__thiscall *)(int, int)>(XI_GAME(0x100814F0u))(a0, a1); }
XI_ORIG undefined4 ActorHandle_SetFields(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10081530u))(a0, a1, a2); }
XI_ORIG int ActorHandle_Resolve(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10081550u))(a0); }
XI_ORIG undefined8 ActorHandle_Resolve_v1(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x10081550u))(a0); }
XI_ORIG int ActorHandle_ResolveObject(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10081600u))(a0); }
XI_ORIG void Unit10081A30_ReleaseQueued() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10081A30u))(); }
XI_ORIG void Actor_ResetDrawLists() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10081A50u))(); }
XI_ORIG unsigned long long FUN_10081ae0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10081AE0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void CXiActor_Init(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10081BA0u))(a0); }
XI_ORIG void * CXiActor_ConstructBase(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10081CF0u))(a0); }
XI_ORIG int Actor_UnlinkAllAttachLists(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10082B10u))(a0); }
XI_ORIG int Actor_CountAll() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10082BF0u))(); }
XI_ORIG undefined4 CXiActor_GetField78Of70(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10082FA0u))(a0); }
XI_ORIG undefined4 Unit10082F60_ProjectThroughCamera(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x10082FB0u))(a0, a1); }
XI_ORIG char CXiActor_NameCalc(void * a0, void * a1, int a2, void * a3, int a4) { return reinterpret_cast<char (__thiscall *)(void *, void *, int, void *, int)>(XI_GAME(0x100830C0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 CXiActor_ComputeDrawState(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10083430u))(a0, a1, a2, a3, a4); }
XI_ORIG int * FUN_10083620(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10083620u))(a0, a1); }
XI_ORIG int * CXiActor_FindNearestToPoint(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10083750u))(a0, a1, a2); }
XI_ORIG int * CXiActor_FindNearestLowerInView(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10083B40u))(a0, a1, a2); }
XI_ORIG int * CXiActor_FindNearestHigherInView(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10083D00u))(a0, a1, a2); }
XI_ORIG int * CXiActor_FindNearestToTarget(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10083F20u))(a0, a1, a2); }
XI_ORIG undefined4 CXiActor_IsWithinRadius(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10084100u))(a0, a1, a2); }
XI_ORIG char CXiActor_IsModelMissingOrFlag2000(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10084140u))(a0); }
XI_ORIG uint32_t CXiActor_IsModelMissingOrFlag2000_v1(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x10084140u))(a0); }
XI_ORIG unsigned long long CXiActor_ModelTest95680(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10084350u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG uint8_t CXiActor_ModelTest956A0(void * a0, int a1) { return reinterpret_cast<uint8_t (__thiscall *)(void *, int)>(XI_GAME(0x10084370u))(a0, a1); }
XI_ORIG unsigned long long CXiActor_ModelTest956A0_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10084370u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG int CXiActor_GetModelField170(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10084390u))(a0); }
XI_ORIG undefined4 CXiActor_GetModelField170_v1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10084390u))(a0); }
XI_ORIG uint8_t CXiActor_HasNonzeroField150or158(void * a0) { return reinterpret_cast<uint8_t (__thiscall *)(void *)>(XI_GAME(0x100843B0u))(a0); }
XI_ORIG int CXiActor_GetModelByteEE(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10084400u))(a0); }
XI_ORIG int CXiActor_GetModelByteEE_v1(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10084400u))(a0); }
XI_ORIG int CXiActor_GetModelByteEF(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10084420u))(a0); }
XI_ORIG uint32_t CXiActor_GetModelPartWord(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10084480u))(a0, a1); }
XI_ORIG BOOL CXiActor_IsModelMissingOrSuppressed(int a0) { return reinterpret_cast<BOOL (__thiscall *)(int)>(XI_GAME(0x10084580u))(a0); }
XI_ORIG uint32_t CXiActor_IsModelMissingOrSuppressed_v1(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x10084580u))(a0); }
XI_ORIG BOOL CXiActor_TestModelFlag128_8(void * a0) { return reinterpret_cast<BOOL (__thiscall *)(void *)>(XI_GAME(0x100845B0u))(a0); }
XI_ORIG int CXiActor_GetSubModelResult(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100845E0u))(a0); }
XI_ORIG uint32_t FUN_10084620(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x10084620u))(a0); }
XI_ORIG BOOL CXiActor_TestModelFlags100000_200000_4000(void * a0) { return reinterpret_cast<BOOL (__thiscall *)(void *)>(XI_GAME(0x10084640u))(a0); }
XI_ORIG void CXiActor_SetField0B4(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10085010u))(a0, a1); }
XI_ORIG int CXiActor_SpawnResourceGenerators(int a0, void * a1) { return reinterpret_cast<int (__thiscall *)(int, void *)>(XI_GAME(0x10085060u))(a0, a1); }
XI_ORIG void CXiActor_SpawnResourceGenerators_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10085060u))(a0, a1); }
XI_ORIG undefined8 FUN_10085240() { return reinterpret_cast<undefined8 (__cdecl *)()>(XI_GAME(0x10085240u))(); }
XI_ORIG undefined8 FUN_10085270(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x10085270u))(a0); }
XI_ORIG void FUN_100852d0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100852D0u))(); }
XI_ORIG uint8_t CXiActor_IsInModelSlotTable(void * a0) { return reinterpret_cast<uint8_t (__thiscall *)(void *)>(XI_GAME(0x10085490u))(a0); }
XI_ORIG int CXiActor_IsInModelSlotTable_v1(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10085490u))(a0); }
XI_ORIG uint32_t CXiActor_GetField88Bit3(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x10085500u))(a0); }
XI_ORIG unsigned long long FUN_10085a10(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10085A10u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_10085a50(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10085A50u))(a0); }
XI_ORIG void CMoAttachments_PushCasterLink(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10085D50u))(a0, a1); }
XI_ORIG undefined4 CMoAttachments_PushTargetLink(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10085D60u))(a0, a1); }
XI_ORIG void Actor_DrawWorldText(void * a0, void * a1, float a2, unsigned int a3, int a4) { reinterpret_cast<void (__cdecl *)(void *, void *, float, unsigned int, int)>(XI_GAME(0x10086210u))(a0, a1, a2, a3, a4); }
XI_ORIG void Actor_DrawWorldText_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10086210u))(a0, a1, a2, a3, a4); }
XI_ORIG void DrawNameplateSecondString(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10086900u))(a0, a1, a2); }
XI_ORIG unsigned long long Unit10086E90_IsParamBelow901(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10086FC0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined8 ActerBuff_FindByField78(XI_ANY a0) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY)>(XI_GAME(0x10086FE0u))(a0); }
XI_ORIG uint32_t Unit10086E90_GetGlobalFlag() { return reinterpret_cast<uint32_t (__cdecl *)()>(XI_GAME(0x10087010u))(); }
XI_ORIG unsigned long long Acter_IsEvent(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10087060u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long IsEvent(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10087070u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Menu_HasAnyFlag(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100870E0u))(a0, a1); }
XI_ORIG unsigned long long Unit10086E90_GetGlobalByte(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10087110u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined thunk_FUN_10087230() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10087220u))(); }
XI_ORIG undefined4 Help_UpdateFromFlags() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100872C0u))(); }
XI_ORIG unsigned long long Unit10086E90_UpdateObject10578184(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100873B0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG uint32_t Unit10086E90_IsIdMatchGlobal() { return reinterpret_cast<uint32_t (__cdecl *)()>(XI_GAME(0x100875D0u))(); }
XI_ORIG unsigned long long Unit10086E90_GetMaskedFlagByte(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10087660u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Unit10086E90_IsGlobalNonzero() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10087670u))(); }
XI_ORIG unsigned long long FUN_10087680(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10087680u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Unit10086E90_GetDword10480800IfFlag(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10087690u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG uint32_t Unit10086E90_GetDword10480800IfFlag_v1() { return reinterpret_cast<uint32_t (__cdecl *)()>(XI_GAME(0x10087690u))(); }
XI_ORIG unsigned long long Unit10086E90_GetByte10480824IfFlag(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100876B0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_100876d0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100876D0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_100876e0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100876E0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Unit10086E90_GetTable10480828IfFlag(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10087760u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Unit10086E90_GetByte10480825IfSet() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10087830u))(); }
XI_ORIG uint32_t FUN_10087850() { return reinterpret_cast<uint32_t (__cdecl *)()>(XI_GAME(0x10087850u))(); }
XI_ORIG undefined4 FUN_10087870() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10087870u))(); }
XI_ORIG void Unit10086E90_ApplyPair1EC1F4(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10087D40u))(a0); }
XI_ORIG uint32_t CXiActor_GetTwoBitField(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x10087DC0u))(a0); }
XI_ORIG void FUN_10087df0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10087DF0u))(a0, a1); }
XI_ORIG undefined4 Acter_ResetMotionCounters(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10087E40u))(a0); }
XI_ORIG void Acter_AdvanceMotionTimer(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10087E80u))(a0); }
XI_ORIG void FUN_10087f00(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10087F00u))(a0); }
XI_ORIG undefined4 FUN_10088270(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10088270u))(a0, a1); }
XI_ORIG void FUN_100882d0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100882D0u))(); }
XI_ORIG undefined4 FUN_10088320(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10088320u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10088380(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10088380u))(a0, a1, a2); }
XI_ORIG void Unit10086E90_UpdateByKind170NotGroup38(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100883E0u))(a0); }
XI_ORIG undefined4 FUN_10088b20() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10088B20u))(); }
XI_ORIG uint32_t FUN_10088b50() { return reinterpret_cast<uint32_t (__cdecl *)()>(XI_GAME(0x10088B50u))(); }
XI_ORIG undefined4 FUN_10088bb0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10088BB0u))(); }
XI_ORIG int FUN_10088bd0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10088BD0u))(a0); }
XI_ORIG void Unit10086E90_UpdateByKind170NotGroup06(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10089100u))(a0); }
XI_ORIG int FUN_10089850(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10089850u))(a0); }
XI_ORIG unsigned long long FUN_10089ff0(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10089FF0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Camera_SetField30(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1008A0E0u))(a0); }
XI_ORIG undefined4 CXiActor_Construct(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1008A120u))(a0); }
XI_ORIG void Actor_SetDefaultMotionTags(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008A160u))(a0); }
XI_ORIG undefined4 CXiActor_SelectMotionTags(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1008A1A0u))(a0); }
XI_ORIG undefined4 CXiActor_PushTagsToObject(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1008A2F0u))(a0); }
XI_ORIG undefined4 CXiActor_InitDefaults(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1008A350u))(a0, a1); }
XI_ORIG void CXiActor_Destroy(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008A7B0u))(a0); }
XI_ORIG int ActerBuff_FindById(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x1008A930u))(a0); }
XI_ORIG undefined8 FUN_1008abe0() { return reinterpret_cast<undefined8 (__cdecl *)()>(XI_GAME(0x1008ABE0u))(); }
XI_ORIG void FUN_1008af60(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1008AF60u))(a0, a1, a2); }
XI_ORIG void Unit10086E90_StartAutoRunFromZoneMap(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008B100u))(a0); }
XI_ORIG void FUN_1008b170(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008B170u))(a0); }
XI_ORIG int Actor_FindByNameEntry(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1008B270u))(a0, a1, a2); }
XI_ORIG uint32_t Actor_CompareNames(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1008B2B0u))(a0, a1, a2); }
XI_ORIG int Actor_FindByField78Entry(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1008B380u))(a0, a1); }
XI_ORIG undefined4 Unit10086E90_LatchFlag13CBit22(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1008B410u))(a0); }
XI_ORIG undefined4 FUN_1008b460(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1008B460u))(a0, a1); }
XI_ORIG void FUN_1008c480(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008C480u))(a0); }
XI_ORIG undefined4 FUN_1008c6c0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1008C6C0u))(a0, a1); }
XI_ORIG void Unit10086E90_UpdateOnePart1008CD90(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008CD90u))(a0); }
XI_ORIG undefined4 FUN_1008d910(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1008D910u))(a0, a1); }
XI_ORIG void CXiActor_ResetRandomTimer(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008D980u))(a0); }
XI_ORIG void CXiActor_TickRandomTimer(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008DA00u))(a0); }
XI_ORIG void Unit10086E90_SetModelField78FromTable(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008DB40u))(a0); }
XI_ORIG unsigned long long CXiActor_IsIdAbove6ff(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1008E130u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 CXiActor_IsField78HighZero(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1008E140u))(a0); }
XI_ORIG undefined4 CXiActor_IsFlag1d0Clear(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1008E150u))(a0); }
XI_ORIG int FUN_1008e180(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1008E180u))(a0); }
XI_ORIG undefined4 FUN_1008e240(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1008E240u))(a0); }
XI_ORIG undefined4 Actor_FindByField2a4(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1008E260u))(a0); }
XI_ORIG void FUN_1008e290(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008E290u))(a0); }
XI_ORIG void FUN_1008e3f0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008E3F0u))(a0); }
XI_ORIG void Unit10086E90_UpdateFlags120Bit23(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008E4D0u))(a0); }
XI_ORIG void Unit10086E90_PlayChorMotion(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008E740u))(a0); }
XI_ORIG void FUN_1008e870(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008E870u))(a0); }
XI_ORIG uint32_t Acter_IsBytes1FFBothIn89NotEqual(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1008E8D0u))(a0, a1); }
XI_ORIG uint32_t FUN_1008e920(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1008E920u))(a0, a1); }
XI_ORIG undefined4 Acter_UpdateOne(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1008E9A0u))(a0); }
XI_ORIG void Acter_ReleaseEventRecord(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008EE30u))(a0); }
XI_ORIG void FUN_1008f1d0() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x1008F1D0u))(); }
XI_ORIG void FUN_1008f740(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008F740u))(a0); }
XI_ORIG void Acter_ReleaseSlotD0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100926C0u))(a0); }
XI_ORIG void Acter_ReleaseSlotCC(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10092710u))(a0); }
XI_ORIG void Acter_ReleaseTenSlots(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10092760u))(a0); }
XI_ORIG void Acter_ReleaseSlotByIndex(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10092780u))(a0, a1); }
XI_ORIG void Acter_ReleaseSlotA8(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10092800u))(a0); }
XI_ORIG void Acter_ReleaseSlotA4(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10092870u))(a0); }
XI_ORIG void Acter_ReleaseAllSlots(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10092900u))(a0); }
XI_ORIG void Acter_CopyPosToCached(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100929A0u))(a0); }
XI_ORIG void Acter_SnapHeightToZoneMap(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100929D0u))(a0); }
XI_ORIG void FUN_10092c20(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10092C20u))(a0); }
XI_ORIG undefined4 Acter_CopySixDwordsTo7C(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10093430u))(a0, a1); }
XI_ORIG void FUN_100935c0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100935C0u))(a0, a1, a2, a3, a4); }
XI_ORIG char Unit10086E90_ResetGlobals() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x10093720u))(); }
XI_ORIG void Acter_ClearGlobalBlocks() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10093A30u))(); }
XI_ORIG void Acter_ResetGlobalSentinels() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10093A90u))(); }
XI_ORIG void Acter_ClearGlobalBlock182() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10093AD0u))(); }
XI_ORIG void Acter_ClearGlobalBlock2C() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10093AF0u))(); }
XI_ORIG void Acter_ClearGlobalBlock23() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10093B10u))(); }
XI_ORIG void Acter_ClearGlobalBlock71() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10093B30u))(); }
XI_ORIG void FUN_10093b50() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10093B50u))(); }
XI_ORIG undefined4 Acter_ShutdownGlobals() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10093B70u))(); }
XI_ORIG void Acter_Nop() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10093DA0u))(); }
XI_ORIG uint8_t Net_SendPacket00COnce() { return reinterpret_cast<uint8_t (__cdecl *)()>(XI_GAME(0x10093DB0u))(); }
XI_ORIG undefined4 Net_SendPacket00COnce_v1() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10093DB0u))(); }
XI_ORIG void FUN_10093e30() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10093E30u))(); }
XI_ORIG void FUN_10093e50() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10093E50u))(); }
XI_ORIG undefined4 FUN_10094190() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10094190u))(); }
XI_ORIG void FUN_10095660() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10095660u))(); }
XI_ORIG undefined4 Acter_IsState5(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10095680u))(a0, a1); }
XI_ORIG undefined4 Acter_IsState55(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100956A0u))(a0, a1); }
XI_ORIG undefined4 Acter_IsStateGroup06Or38(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100956C0u))(a0, a1); }
XI_ORIG undefined4 Acter_IsStateGroup06(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100956F0u))(a0, a1); }
XI_ORIG undefined4 Acter_IsStateGroup38(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10095750u))(a0, a1); }
XI_ORIG int Acter_IsStateGroup3F(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10095790u))(a0, a1); }
XI_ORIG uint32_t Unit10086E90_CanClearPendingState(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x10095850u))(a0); }
XI_ORIG void Acter_UpdateAllLists() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10095900u))(); }
XI_ORIG int FUN_10095d80() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10095D80u))(); }
XI_ORIG void FUN_10095da0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10095DA0u))(a0); }
XI_ORIG void Acter_ClearPendingState(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10095DD0u))(a0); }
XI_ORIG void Acter_ClearPending272(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10095E30u))(a0); }
XI_ORIG void Acter_UpdateActionState(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10095E90u))(a0); }
XI_ORIG uint32_t FUN_10096040(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x10096040u))(a0); }
XI_ORIG undefined4 FUN_100960f0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100960F0u))(a0, a1); }
XI_ORIG uint32_t FUN_100961d0(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x100961D0u))(a0); }
XI_ORIG undefined4 FUN_10096200(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10096200u))(a0); }
XI_ORIG undefined4 Acter_UpdateFrame() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10096640u))(); }
XI_ORIG void FUN_10096c90(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10096C90u))(a0, a1); }
XI_ORIG void FUN_100973b0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100973B0u))(a0); }
XI_ORIG undefined8 FUN_10097500(void * a0, void * a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined8 (__fastcall *)(void *, void *, XI_ANY, XI_ANY)>(XI_GAME(0x10097500u))(a0, a1, a2, a3); }
XI_ORIG uint32_t FUN_10097730(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10097730u))(a0, a1); }
XI_ORIG undefined8 FUN_100977a0(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x100977A0u))(a0); }
XI_ORIG undefined8 FUN_100977f0(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x100977F0u))(a0); }
XI_ORIG char * Entity_BuildDisplayName(int a0) { return reinterpret_cast<char * (__thiscall *)(int)>(XI_GAME(0x10097840u))(a0); }
XI_ORIG undefined1 * FUN_100979b0(void * a0) { return reinterpret_cast<undefined1 * (__thiscall *)(void *)>(XI_GAME(0x100979B0u))(a0); }
XI_ORIG int FUN_10097a60(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10097A60u))(a0); }
XI_ORIG void FUN_10097af0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10097AF0u))(); }
XI_ORIG undefined4 Unit10086E90_IsFlag40DB6OrState17() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10097D50u))(); }
XI_ORIG undefined4 Acter_TestFlagTable(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10097E50u))(a0, a1); }
XI_ORIG undefined4 Net_BuildPacket61(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10097FD0u))(a0, a1); }
XI_ORIG undefined4 Acter_IsSpecialIdRange(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10098180u))(a0); }
XI_ORIG undefined4 FUN_10098360() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x10098360u))(); }
XI_ORIG uint32_t Acter_GetPartnerIfKind(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x100983A0u))(a0); }
XI_ORIG void FUN_100983f0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100983F0u))(a0); }
XI_ORIG unsigned long long Unit10086E90_GetTable10485830IfBit2(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100991F0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Unit10086E90_ResolveEntryNames() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10099730u))(); }
XI_ORIG void Unit10086E90_UpdateFrame997B0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100997B0u))(); }
XI_ORIG undefined4 Acter_IsByte123Mask7In234(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1009AAF0u))(a0); }
XI_ORIG void Acter_SetFieldsAt134And138(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1009ADE0u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG long double Acter_GetField138Clamped(void * a0) { return reinterpret_cast<long double (__thiscall *)(void *)>(XI_GAME(0x1009AEC0u))(a0); }
XI_ORIG void Acter_SetFlagBits138(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1009AF60u))(a0, a1, a2, a3); }
XI_ORIG void FUN_1009f2f0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1009F2F0u))(); }
XI_ORIG void Acter_StepAfterimageStates(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1009F4C0u))(a0); }
XI_ORIG undefined1 * Unit10086E90_FindStringById(XI_ANY a0) { return reinterpret_cast<undefined1 * (__cdecl *)(XI_ANY)>(XI_GAME(0x100A1770u))(a0); }
XI_ORIG undefined4 FUN_100a2720() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100A2720u))(); }
XI_ORIG char CMoTask_VT_1032D6D4_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100A2810u))(a0); }
XI_ORIG void Unit100A2810_ResetRecords() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100A3CD0u))(); }
XI_ORIG void FUN_100a3d00() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100A3D00u))(); }
XI_ORIG _DWORD * CXiCollisionActor_Constructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x100A4C20u))(a0); }
XI_ORIG void CXiControlActor_FillHistory(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100A5770u))(a0, a1); }
XI_ORIG undefined4 CXiControlActor_SetVec4(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100A57C0u))(a0, a1); }
XI_ORIG void XiControlActor_UpdateCamera(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100A5E10u))(a0); }
XI_ORIG undefined4 XiControlActor_UpdateView(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100A5EF0u))(a0); }
XI_ORIG unsigned long long XiControlActor_SetAutoRunning(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100A6070u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_100a6240(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100A6240u))(a0, a1); }
XI_ORIG void * CXiControlActor_Constructor(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x100A89E0u))(a0); }
XI_ORIG char CXiControlActor_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100A8CA0u))(a0); }
XI_ORIG undefined4 FUN_100a8cc0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100A8CC0u))(a0); }
XI_ORIG char CMoTask_VT_1032E808_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100A9D30u))(a0); }
XI_ORIG _DWORD * CMoTask_VT_1032E808_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x100A9DB0u))(a0, a1); }
XI_ORIG void FUN_100aafb0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x100AAFB0u))(a0, a1); }
XI_ORIG void FUN_100ab320(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100AB320u))(a0); }
XI_ORIG void Unit100AB450_ReleaseSlot(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100AB550u))(a0); }
XI_ORIG void CXiDancerActor_InitBind(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100AB750u))(a0, a1, a2, a3, a4); }
XI_ORIG void CXiDancerActor_SetPosition(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100AB830u))(a0, a1); }
XI_ORIG unsigned long long FUN_100ab880(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100AB880u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void CXiDancerActor_ApplyStateBlock(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100AB9A0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 Unit100ABC20_CopyReturnZ(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100ABC70u))(a0, a1); }
XI_ORIG undefined4 FUN_100ae3f0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100AE3F0u))(); }
XI_ORIG void FUN_100ae960() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100AE960u))(); }
XI_ORIG undefined4 FUN_100aea40() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100AEA40u))(); }
XI_ORIG undefined4 FUN_100aec70(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100AEC70u))(a0); }
XI_ORIG void FUN_100aed00(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100AED00u))(a0, a1, a2, a3); }
XI_ORIG int FUN_100b37a0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100B37A0u))(a0); }
XI_ORIG void XiEvent_EventIdle(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100BCD10u))(a0); }
XI_ORIG undefined4 FUN_100bd0e0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100BD0E0u))(a0); }
XI_ORIG void XiEvent_RestoreActer(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100BD5F0u))(a0); }
XI_ORIG void * FUN_100bd820(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x100BD820u))(a0); }
XI_ORIG char * FUN_100c2d00(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<char * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100C2D00u))(a0, a1); }
XI_ORIG void * XiEvent_Init(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100C2E20u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG undefined4 FUN_100c2fb0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100C2FB0u))(a0); }
XI_ORIG int FUN_100c2fe0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100C2FE0u))(a0); }
XI_ORIG unsigned long long Unit100ACAB0_GetWordBFE(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100C3020u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Unit100ACAB0_GetWordC00(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100C3030u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Unit100ACAB0_FreeData(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100C3050u))(a0); }
XI_ORIG void * Unit100ACAB0_Init(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100C3060u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG void * FUN_100c30f0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x100C30F0u))(a0); }
XI_ORIG int FUN_100c3100(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100C3100u))(a0); }
XI_ORIG undefined4 FUN_100c3120(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100C3120u))(a0); }
XI_ORIG undefined4 FUN_100c3130(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100C3130u))(a0); }
XI_ORIG undefined4 FUN_100c3140(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100C3140u))(a0); }
XI_ORIG undefined4 FUN_100c3150(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100C3150u))(a0); }
XI_ORIG undefined4 FUN_100c3160(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100C3160u))(a0); }
XI_ORIG void FUN_100c3170(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100C3170u))(a0); }
XI_ORIG undefined * Unit100ACAB0_GetSingleton() { return reinterpret_cast<undefined * (__cdecl *)()>(XI_GAME(0x100C3190u))(); }
XI_ORIG void FUN_100c31f0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100C31F0u))(a0); }
XI_ORIG void Unit100ACAB0_SetFlag8(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100C3200u))(a0); }
XI_ORIG undefined4 Unit100ACAB0_HasItems(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100C3210u))(a0); }
XI_ORIG undefined4 Unit100ACAB0_Clear(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100C3220u))(a0); }
XI_ORIG void Unit100ACAB0_Append(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100C3260u))(a0, a1); }
XI_ORIG void Unit100ACAB0_AddEvent(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100C3300u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG void XiFurnitureActor_Release(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100C3E70u))(a0); }
XI_ORIG undefined4 FUN_100c4400(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100C4400u))(a0); }
XI_ORIG undefined4 FUN_100c4bc0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x100C4BC0u))(a0, a1, a2); }
XI_ORIG int Unit100C51C0_MapCodeIndex(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100C51C0u))(a0, a1); }
XI_ORIG int Unit100C51C0_AddOffset996B(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x100C5260u))(a0); }
XI_ORIG int Unit100C51C0_AddOffset19131(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x100C5270u))(a0); }
XI_ORIG int Unit100C51C0_AddOffset1889B(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x100C5280u))(a0); }
XI_ORIG int Unit100C51C0_ForwardToCce00(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x100C5290u))(a0); }
XI_ORIG undefined4 CXiSkeletonActor_Constructor_1Arg(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100C52A0u))(a0, a1); }
XI_ORIG int TargetActor_SlotHandler1() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100C5410u))(); }
XI_ORIG _DWORD * TargetActor_SlotHandler0() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x100C5440u))(); }
XI_ORIG _DWORD * TargetActor_SlotHandler4() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x100C54C0u))(); }
XI_ORIG char CXiSkeletonActor_DestructorBody(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x100C5C00u))(a0); }
XI_ORIG int CXiSkeletonActor_Init(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100C5D60u))(a0); }
XI_ORIG int Actor_SelectNearestLights(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100CB4E0u))(a0); }
XI_ORIG void PerActor_FourBucketStaggeredUpdate(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100CBDE0u))(a0); }
XI_ORIG void nullsub_276() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100CCE20u))(); }
XI_ORIG void nullsub_277() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100CCE30u))(); }
XI_ORIG void nullsub_278() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100CCE40u))(); }
XI_ORIG void nullsub_279() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100CCE50u))(); }
XI_ORIG void Actor_SetCameraFromModelPoints(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100CCE60u))(a0); }
XI_ORIG void CXiSkeletonActor_SetSlotMotion(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100CD4E0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG undefined4 CXiSkeletonActor_SetMotion(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9, XI_ANY a10, XI_ANY a11) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100CD540u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11); }
XI_ORIG void FUN_100ce0e0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100CE0E0u))(a0); }
XI_ORIG void CXiSkeletonActor_AdvanceMotionQueue(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100CE160u))(a0); }
XI_ORIG undefined4 CXiSkeletonActor_ReverseFindRes(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100CE480u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG undefined4 FUN_100cf060(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100CF060u))(a0, a1); }
XI_ORIG void FUN_100cf0b0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100CF0B0u))(a0, a1); }
XI_ORIG undefined4 CXiSkeletonActor_ForParts0To9(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100CF100u))(a0, a1); }
XI_ORIG undefined4 FUN_100cf160(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100CF160u))(a0, a1); }
XI_ORIG void * FUN_100cf750(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100CF750u))(a0, a1); }
XI_ORIG uint16_t * CXiSkeletonActor_GetTableEntryByModelKind(void * a0) { return reinterpret_cast<uint16_t * (__thiscall *)(void *)>(XI_GAME(0x100CFB40u))(a0); }
XI_ORIG undefined4 FUN_100d0b50(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100D0B50u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined FUN_100d0e80() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x100D0E80u))(); }
XI_ORIG void CXiSkeletonActor_SelectStateMessage(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x100D0FC0u))(a0, a1, a2); }
XI_ORIG unsigned long long CXiSkeletonActor_IsReadCompleteBody(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100D12D0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_100d1450(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x100D1450u))(a0, a1, a2); }
XI_ORIG void FUN_100d2800(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100D2800u))(a0); }
XI_ORIG int FUN_100d2d70(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100D2D70u))(a0, a1, a2, a3); }
XI_ORIG int FUN_100d2e30(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x100D2E30u))(a0, a1, a2); }
XI_ORIG char CXiSkeletonActor_OnSkeletonLoaded(int a0, int a1) { return reinterpret_cast<char (__cdecl *)(int, int)>(XI_GAME(0x100D2F40u))(a0, a1); }
XI_ORIG char CXiSkeletonActor_LoadSkeletonSync(int a0, int a1) { return reinterpret_cast<char (__thiscall *)(int, int)>(XI_GAME(0x100D2F60u))(a0, a1); }
XI_ORIG char CXiSkeletonActor_LoadSkeletonResource(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(XI_GAME(0x100D4330u))(a0, a1); }
XI_ORIG undefined4 CXiSkeletonActor_LoadSkeletonResource_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100D4330u))(a0, a1); }
XI_ORIG int CXiSkeletonActor_AddListEntry(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100D4410u))(a0, a1, a2, a3, a4); }
XI_ORIG unsigned long long CXiSkeletonActor_ReleaseListEntriesMatching(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100D44B0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG int CXiSkeletonActor_DeleteOwnedList(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100D44F0u))(a0); }
XI_ORIG int FUN_100d4520(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100D4520u))(a0, a1); }
XI_ORIG void FUN_100d5430(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100D5430u))(a0, a1); }
XI_ORIG int sub_100D5430(void * a0, char a1) { return reinterpret_cast<int (__thiscall *)(void *, char)>(XI_GAME(0x100D5430u))(a0, a1); }
XI_ORIG void CXiSkeletonActor_CalcGroundTiltMatrix(void * a0, void * a1, float a2) { reinterpret_cast<void (__thiscall *)(void *, void *, float)>(XI_GAME(0x100D54C0u))(a0, a1, a2); }
XI_ORIG void CXiSkeletonActor_UpdateHeadLook(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100D5B10u))(a0); }
XI_ORIG undefined8 FUN_100d5fe0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100D5FE0u))(a0, a1); }
XI_ORIG undefined8 FUN_100d6180(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100D6180u))(a0, a1); }
XI_ORIG uint8_t CXiSkeletonActor_FindPartValue(void * a0, int a1) { return reinterpret_cast<uint8_t (__thiscall *)(void *, int)>(XI_GAME(0x100D6240u))(a0, a1); }
XI_ORIG uint32_t CXiSkeletonActor_FindPartValue_v1(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100D6240u))(a0, a1); }
XI_ORIG undefined4 FUN_100d65b0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100D65B0u))(a0, a1); }
XI_ORIG int Actor_PrepareDrawGlobals(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100D6D30u))(a0); }
XI_ORIG _DWORD * CXiSkeletonActorRes_Constructor(void * a0, int a1, int a2) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int, int)>(XI_GAME(0x100D7020u))(a0, a1, a2); }
XI_ORIG char CXiSkeletonActorRes_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100D7050u))(a0); }
XI_ORIG int CXiSkeletonActorRes_GetLinkedActor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100D7090u))(a0); }
XI_ORIG undefined4 CXiSkeletonActorRes_GetLinkedActor_v1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100D7090u))(a0); }
XI_ORIG void Unit100D7420_Md5Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100D7420u))(a0); }
XI_ORIG void Unit100D7420_Md5Update(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100D7450u))(a0, a1, a2); }
XI_ORIG void Unit100D7420_PadAndAppendLength(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100D7510u))(a0); }
XI_ORIG void Unit100D7420_FinalDigest(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100D7560u))(a0, a1); }
XI_ORIG void Unit100D7420_Md5ComputeBlock(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100D75A0u))(a0, a1); }
XI_ORIG int Unit100D7420_AddOne(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x100D7E90u))(a0); }
XI_ORIG void Unit100D7420_HashStringWithSeed(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100D7EA0u))(a0, a1, a2); }
XI_ORIG void Unit100D7420_HashBytesWithSeed(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100D7ED0u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_100d8490(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100D8490u))(a0); }
XI_ORIG undefined4 Mutex_WriteRecordLocked(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100D8A20u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_100d93e0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100D93E0u))(); }
XI_ORIG undefined4 Game_InitGlobal() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100D9410u))(); }
XI_ORIG undefined4 Unit100D7420_SetGlobalBlock1048BEB8() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100D9430u))(); }
XI_ORIG undefined4 Unit100D7420_Return0_100D9460() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100D9460u))(); }
XI_ORIG void Game_ResetGlobals() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100D9830u))(); }
XI_ORIG undefined4 Game_InitOnce(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100D9950u))(a0); }
XI_ORIG undefined4 Unit100D7420_GetEntryMod1066268C() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100D9B00u))(); }
XI_ORIG char Game_InitSceneState(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x100DA6A0u))(a0); }
XI_ORIG undefined4 FUN_100daf30(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100DAF30u))(a0); }
XI_ORIG int GameMode_PollPendingState(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100DB140u))(a0); }
XI_ORIG int GameMode_PollPendingState_v1(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100DB140u))(a0); }
XI_ORIG undefined4 FUN_100db170(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x100DB170u))(a0, a1, a2); }
XI_ORIG int GameMode_RequestState(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x100DB170u))(a0, a1, a2); }
XI_ORIG undefined4 Unit100D7420_GetRecordWordFwd(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x100DB780u))(a0); }
XI_ORIG int Unit100D7420_GetRecordDwordFwd(int a0) { return reinterpret_cast<int (__stdcall *)(int)>(XI_GAME(0x100DB790u))(a0); }
XI_ORIG undefined4 Unit100D7420_GetRecordDwordFwd_v1(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x100DB790u))(a0); }
XI_ORIG undefined4 Unit100D7420_LookupNameIfKind3(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100DB7B0u))(a0, a1); }
XI_ORIG undefined4 Unit100D7420_GetWord40D7C() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DB930u))(); }
XI_ORIG undefined4 Game_IsFlag10492DB4() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DBCA0u))(); }
XI_ORIG undefined1 Unit100D7420_ReturnTrue100DBDE0() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100DBDE0u))(); }
XI_ORIG DWORD Time_GetMilliseconds() { return reinterpret_cast<DWORD (__stdcall *)()>(XI_GAME(0x100DBE00u))(); }
XI_ORIG undefined4 FUN_100dbe10() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DBE10u))(); }
XI_ORIG undefined4 Unit100D7420_GetTimeNow(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100DBE30u))(a0, a1); }
XI_ORIG char Unit100D7420_InitTimer() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x100DBE50u))(); }
XI_ORIG undefined4 Unit100D7420_Sync() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DBEB0u))(); }
XI_ORIG int Unit100D7420_DiffWrapped(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100DBF00u))(a0, a1, a2); }
XI_ORIG void Unit100D7420_AdvanceClock() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100DBF20u))(); }
XI_ORIG undefined4 Unit100D7420_CheckSlotExpired(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100DBFE0u))(a0, a1); }
XI_ORIG int Unit100D7420_GetTotalMilliseconds() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100DC060u))(); }
XI_ORIG undefined4 Unit100D7420_GetSeconds() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DC080u))(); }
XI_ORIG undefined4 Unit100D7420_GetMilliseconds() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DC090u))(); }
XI_ORIG undefined4 Unit100D7420_AdvanceAndGetMs() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DC120u))(); }
XI_ORIG undefined4 Unit100D7420_AdvanceAndGetSeconds() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DC130u))(); }
XI_ORIG undefined4 Unit100D7420_SetOffsetMilliseconds(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100DC1B0u))(a0, a1); }
XI_ORIG undefined4 Unit100D7420_Reset() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DC1F0u))(); }
XI_ORIG undefined4 Unit100D7420_Update() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DC210u))(); }
XI_ORIG undefined4 Unit100D7420_GetWrapCount() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DC250u))(); }
XI_ORIG undefined4 Unit100D7420_GetLastTick() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DC260u))(); }
XI_ORIG undefined4 Unit100D7420_InitContext() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DC270u))(); }
XI_ORIG undefined4 Unit100D7420_IsInitialized() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DC2C0u))(); }
XI_ORIG unsigned long long FUN_100dc2d0(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100DC2D0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG uint32_t FUN_100dc3b0(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x100DC3B0u))(a0); }
XI_ORIG undefined4 Unit100D7420_GetContextField1AFC0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DC420u))(); }
XI_ORIG undefined FUN_100dc650(XI_ANY a0) { return reinterpret_cast<undefined (__cdecl *)(XI_ANY)>(XI_GAME(0x100DC650u))(a0); }
XI_ORIG void Unit100D7420_EncryptBlocks(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100DCA00u))(a0, a1, a2); }
XI_ORIG void Unit100D7420_DecryptBlocks(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100DCF60u))(a0, a1, a2); }
XI_ORIG undefined4 Unit100D7420_HashPair(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100DD800u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG uint16_t * Unit100D7420_QueueFindOrAllocSlot(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<uint16_t * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100DDA60u))(a0, a1, a2, a3); }
XI_ORIG undefined4 Unit100D7420_SlotSetLength(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100DDD20u))(a0, a1, a2); }
XI_ORIG void Unit100D7420_Md5InitCountFirst(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100DDE90u))(a0); }
XI_ORIG void Unit100D7420_Md5UpdateCountFirst(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100DDEC0u))(a0, a1, a2); }
XI_ORIG void Unit100D7420_Md5FinalCountFirst(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100DDF60u))(a0); }
XI_ORIG void Unit100D7420_Transform(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100DE010u))(a0, a1); }
XI_ORIG uint32_t Unit100D7420_Decode(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100DF5D0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 Unit100D7420_CopyBits(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100DF6E0u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int Unit100D7420_EstimateBytes(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100DF780u))(a0, a1, a2); }
XI_ORIG int Unit100D7420_Encode(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100DF7C0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined1 Unit100D7420_ReturnTrue100E0100() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100E0100u))(); }
XI_ORIG int Entry_FindByNormalizedName(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x100E08D0u))(a0); }
XI_ORIG void Name_Normalize(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100E09A0u))(a0, a1); }
XI_ORIG undefined4 Unit100D7420_ClearFlag1A698() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100E0A00u))(); }
XI_ORIG undefined4 Unit100D7420_FindEntryByName(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100E0A50u))(a0); }
XI_ORIG undefined4 Unit100D7420_SendRequest3CThrottled() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100E0AA0u))(); }
XI_ORIG void Unit100D7420_NoOp100E0B10() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100E0B10u))(); }
XI_ORIG undefined4 Unit100D7420_ApplyNoticeRecord(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100E0D50u))(a0); }
XI_ORIG undefined4 Entry_ResetFields(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100E13C0u))(a0); }
XI_ORIG int Unit100D7420_AllocEntry4(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x100E19C0u))(a0); }
XI_ORIG undefined4 Unit100D7420_AllocEntryFromTable() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100E1AD0u))(); }
XI_ORIG int Unit100D7420_CountActive() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100E1B50u))(); }
XI_ORIG undefined4 Unit100D7420_ClearMatching(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100E1C00u))(a0, a1); }
XI_ORIG undefined4 * Unit100D7420_FindFreeHandlerSlot(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9, XI_ANY a10, XI_ANY a11) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100E1C30u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11); }
XI_ORIG undefined4 Unit100D7420_RegisterHandler1369C(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100E2E70u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined4 Unit100D7420_RegisterHandler1369CFlag1(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100E2EE0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined1 Unit100D7420_ReturnTrue100E33F0() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100E33F0u))(); }
XI_ORIG undefined1 Unit100D7420_ReturnTrue100E4110() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100E4110u))(); }
XI_ORIG undefined1 Unit100D7420_ReturnTrue100E4330() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100E4330u))(); }
XI_ORIG undefined4 Unit100D7420_ResetObject2D000(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100E43F0u))(a0); }
XI_ORIG undefined4 Unit100D7420_GetBlock2CFD8(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100E47A0u))(a0); }
XI_ORIG void Unit100D7420_SetGlobal104B1774(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100E5E30u))(a0); }
XI_ORIG undefined1 Unit100D7420_ReturnTrue100E5FF0() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100E5FF0u))(); }
XI_ORIG int Unit100D7420_GetBlock2CDE8IfReady() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100E6020u))(); }
XI_ORIG void FUN_100e6080(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100E6080u))(a0, a1, a2); }
XI_ORIG undefined1 Unit100D7420_ReturnTrue100E61C0() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100E61C0u))(); }
XI_ORIG void Unit100D7420_SetWord130(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100E62C0u))(a0); }
XI_ORIG undefined4 Unit100D7420_ClearBit0Slot(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100E6370u))(a0); }
XI_ORIG undefined4 Unit100D7420_ClearBit1Fc(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100E63C0u))(a0); }
XI_ORIG undefined4 Unit100D7420_ScanEntriesAndInit(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100E63E0u))(a0); }
XI_ORIG undefined4 Unit100D7420_InitEncrypt(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100E6650u))(a0); }
XI_ORIG undefined4 Unit100D7420_IsBeforeDeadline() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100E6790u))(); }
XI_ORIG undefined4 Unit100D7420_IsCountOrByte134Set() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100E67E0u))(); }
XI_ORIG undefined4 Unit100D7420_ClearTimer140() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100E6810u))(); }
XI_ORIG undefined4 Unit100D7420_ScanIfTimer140Set() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100E6830u))(); }
XI_ORIG undefined4 Unit100D7420_IsTimer140Clear() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100E6860u))(); }
XI_ORIG void Unit100D7420_SetDuration138(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100E6880u))(a0); }
XI_ORIG undefined4 Unit100D7420_GetWord132() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100E6B10u))(); }
XI_ORIG undefined4 Unit100D7420_BuildListA90() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100E6B20u))(); }
XI_ORIG int Unit100D7420_GetEntryPtrA90(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x100E6C40u))(a0); }
XI_ORIG void Unit100D7420_ClearPending() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100E7260u))(); }
XI_ORIG int Unit100D7420_ThunkByIndex200(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100E7B20u))(a0, a1); }
XI_ORIG undefined1 Unit100D7420_ReturnTrue100E84B0() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100E84B0u))(); }
XI_ORIG undefined1 Unit100D7420_ReturnTrue100E8B70() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100E8B70u))(); }
XI_ORIG int Unit100D7420_SetByte9800AndGetEntry(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x100E8B80u))(a0); }
XI_ORIG int Unit100D7420_SelectNext() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100E8C70u))(); }
XI_ORIG undefined4 Unit100D7420_SendByte76(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100E9550u))(a0); }
XI_ORIG undefined1 Unit100D7420_ReturnTrue100EB0A0() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100EB0A0u))(); }
XI_ORIG short * Unit100D7420_GetSlotPtr(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<short * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100EB490u))(a0, a1); }
XI_ORIG int Unit100D7420_CountFreeEntries(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x100EB8D0u))(a0); }
XI_ORIG undefined4 Unit100D7420_GetEntryCount(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100EB940u))(a0); }
XI_ORIG undefined4 Unit100D7420_SendDwordPacket(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100EBF40u))(a0); }
XI_ORIG int Unit100D7420_FindOrRemove(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100EBF80u))(a0, a1, a2); }
XI_ORIG void Unit100D7420_RemoveRecord(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100EC060u))(a0, a1); }
XI_ORIG void Unit100D7420_ResetCounter() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100EDEA0u))(); }
XI_ORIG undefined4 Unit100D7420_Advance() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100EDEB0u))(); }
XI_ORIG undefined FUN_100ef1a0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x100EF1A0u))(); }
XI_ORIG undefined4 Unit100D7420_GetRecordWord(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100EFC70u))(a0); }
XI_ORIG undefined4 Unit100D7420_GetRecordDword(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100EFCA0u))(a0); }
XI_ORIG char * Name_LookupByKey(XI_ANY a0) { return reinterpret_cast<char * (__cdecl *)(XI_ANY)>(XI_GAME(0x100EFCE0u))(a0); }
XI_ORIG int Unit100D7420_FindMatchingEntry(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100EFDF0u))(a0, a1, a2); }
XI_ORIG undefined1 Unit100D7420_ReturnTrue100F0090() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100F0090u))(); }
XI_ORIG unsigned long long FUN_100f0130(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) {
    uint32_t xs[6] = {a0.bits, a1.bits, a2.bits, a3.bits, a4.bits, a5.bits}; uint32_t ra = a6.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 20(%%esi)\n\t""pushl 16(%%esi)\n\t""pushl 12(%%esi)\n\t""pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $24, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100F0130u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Unit100D7420_ReturnTrue100F02A0(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100F02A0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Unit100D7420_SetByte15110(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100F04F0u))(a0); }
XI_ORIG undefined4 Unit100D7420_SetByte15114(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100F0550u))(a0); }
XI_ORIG undefined1 Unit100D7420_ReturnTrue100F06B0() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100F06B0u))(); }
XI_ORIG undefined1 Unit100D7420_ReturnTrue100F09C0() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100F09C0u))(); }
XI_ORIG uint32_t Unit100D7420_GetFlagsMasked() { return reinterpret_cast<uint32_t (__cdecl *)()>(XI_GAME(0x100F0C50u))(); }
XI_ORIG undefined4 Unit100D7420_ClearRecord(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100F10C0u))(a0); }
XI_ORIG int Unit100D7420_UpdateActive() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100F10F0u))(); }
XI_ORIG int Unit100D7420_StepBitGuardedTask(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x100F1160u))(a0); }
XI_ORIG undefined4 Unit100D7420_AcquireIfFree(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100F1280u))(a0, a1, a2); }
XI_ORIG char * Unit100D7420_AllocRecord(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<char * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100F1300u))(a0, a1, a2, a3); }
XI_ORIG undefined4 Unit100D7420_InitEntry(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100F1340u))(a0, a1, a2, a3); }
XI_ORIG uint32_t Unit100D7420_IsBitSet(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x100F13B0u))(a0); }
XI_ORIG void Unit100D7420_SetBit(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100F13D0u))(a0); }
XI_ORIG void Unit100D7420_ClearBit(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100F13F0u))(a0); }
XI_ORIG undefined4 FUN_100f1410(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100F1410u))(a0); }
XI_ORIG undefined4 thunk_FUN_1030ff33() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x100F1770u))(); }
XI_ORIG undefined4 thunk_FUN_1030ff49() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x100F1780u))(); }
XI_ORIG void Unit100D7420_Call10310067() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100F1870u))(); }
XI_ORIG void Unit100D7420_Call10310072() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100F1900u))(); }
XI_ORIG int Unit100D7420_AcquireNode(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100F1B20u))(a0); }
XI_ORIG undefined4 Unit100D7420_InitTwoNodes(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100F1BA0u))(a0); }
XI_ORIG uint32_t Unit100D7420_Destroy(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100F1C40u))(a0, a1); }
XI_ORIG undefined4 Unit100D7420_UpdateRequests(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100F1D20u))(a0); }
XI_ORIG undefined1 Unit100D7420_ResetPending(void * a0) { return reinterpret_cast<undefined1 (__thiscall *)(void *)>(XI_GAME(0x100F1E70u))(a0); }
XI_ORIG undefined4 Unit100D7420_BeginRequests(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100F1F00u))(a0, a1); }
XI_ORIG undefined4 Unit100D7420_StartRequestSlot20(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100F25F0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 Unit100D7420_StartRequestOnNewNodeA(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100F2690u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 Unit100D7420_StartRequestOnNewNodeB(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100F2720u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 Unit100D7420_StartRequestSlot24(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100F2760u))(a0, a1, a2, a3); }
XI_ORIG undefined4 Unit100D7420_ClearNode(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100F29E0u))(a0); }
XI_ORIG void FUN_100f2a40(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100F2A40u))(a0); }
XI_ORIG void Unit100D7420_DestroyNode(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100F2A40u))(a0); }
XI_ORIG undefined4 FUN_100f2a70(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100F2A70u))(a0); }
XI_ORIG undefined4 Unit100D7420_Complete(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100F2B80u))(a0); }
XI_ORIG unsigned long long Unit100D7420_CallStateCallback3(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100F2C40u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Unit100D7420_RunCallbacks(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100F2C80u))(a0, a1); }
XI_ORIG void Unit100D7420_CopyBounded(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100F3010u))(a0, a1, a2); }
XI_ORIG int Unit100D7420_StartCallbacks(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100F32B0u))(a0, a1); }
XI_ORIG undefined4 Unit100D7420_InitRequestA(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100F3BB0u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG undefined4 Unit100D7420_InitRequestB(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100F3C20u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG undefined4 Unit100D7420_InitRequestC(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100F3C90u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG undefined4 Unit100D7420_InitRequestD(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100F3D00u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined4 Unit100D7420_EnsureGlobal() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100F76B0u))(); }
XI_ORIG unsigned long long Unit100D7420_DestroyGlobal(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100F76D0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Unit100D7420_UpdateGlobalRequests() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100F7700u))(); }
XI_ORIG undefined4 FUN_100f78c0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100F78C0u))(); }
XI_ORIG undefined1 Unit100D7420_ReturnTrue_100F8920() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100F8920u))(); }
XI_ORIG undefined1 Unit100D7420_ReturnTrue_100F8CB0() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100F8CB0u))(); }
XI_ORIG undefined1 Unit100D7420_ReturnTrue_100F9350() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100F9350u))(); }
XI_ORIG int Unit100D7420_GetActiveBlock3AE98(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100F94D0u))(a0); }
XI_ORIG undefined4 Unit100D7420_TickSecondCounters() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100F9CC0u))(); }
XI_ORIG void Unit100D7420_UpdateSession() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100F9D50u))(); }
XI_ORIG undefined4 Unit100D7420_ClearSessionGlobals() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100F9DA0u))(); }
XI_ORIG undefined4 Unit100D7420_ShutdownSessions() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100F9DB0u))(); }
XI_ORIG undefined4 Unit100D7420_ShutdownIfActive(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100F9DF0u))(a0); }
XI_ORIG undefined4 Unit100D7420_ShutdownEntry(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100F9E10u))(a0); }
XI_ORIG undefined1 FUN_100fa180(XI_ANY a0) { return reinterpret_cast<undefined1 (__cdecl *)(XI_ANY)>(XI_GAME(0x100FA180u))(a0); }
XI_ORIG undefined4 FUN_100fa4a0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100FA4A0u))(a0); }
XI_ORIG unsigned long long Unit100D7420_SetPacketType(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100FA940u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_100fc0f0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100FC0F0u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_100fc140(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100FC140u))(a0); }
XI_ORIG undefined4 Unit100D7420_GetFlags() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100FC160u))(); }
XI_ORIG undefined4 Unit100D7420_ApplyStatusText(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100FC180u))(a0); }
XI_ORIG undefined4 FUN_100fc2b0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100FC2B0u))(a0); }
XI_ORIG undefined4 FUN_100fc2e0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100FC2E0u))(); }
XI_ORIG undefined4 FUN_100fc300() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100FC300u))(); }
XI_ORIG undefined4 Unit100D7420_SetWord40DAA(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100FC320u))(a0); }
XI_ORIG unsigned long long thunk_FUN_100fc370(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[2] = {a0.bits, a1.bits}; uint32_t ra = a2.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $8, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100FC3B0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined8 Unit100D7420_CheckTimer41B4(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100FC3C0u))(a0, a1, a2); }
XI_ORIG undefined4 Unit100D7420_SendRaw(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100FC4D0u))(a0, a1, a2); }
XI_ORIG undefined * Unit100D7420_GetIdPair() { return reinterpret_cast<undefined * (__cdecl *)()>(XI_GAME(0x100FC510u))(); }
XI_ORIG undefined4 Unit100D7420_SendTypeF2(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100FC680u))(a0, a1); }
XI_ORIG void Unit100D7420_SetPerMille(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100FCA20u))(a0, a1, a2); }
XI_ORIG void Unit100D7420_SetPerMilleAt4(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100FCA40u))(a0, a1, a2); }
XI_ORIG void Unit100D7420_AdjustLevel(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100FCA60u))(a0, a1, a2); }
XI_ORIG undefined4 Unit100D7420_FindTimerKind10() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100FCD70u))(); }
XI_ORIG undefined4 Unit100D7420_GetTableByteForTens(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10101BC0u))(a0); }
XI_ORIG uint32_t Unit100D7420_CeilDiv10Max10(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x10101BF0u))(a0); }
XI_ORIG uint32_t ntGameInit() { return reinterpret_cast<uint32_t (__cdecl *)()>(XI_GAME(0x10102340u))(); }
XI_ORIG void FUN_10103720() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x10103720u))(); }
XI_ORIG undefined4 Game_ClearFlagsBits() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10103740u))(); }
XI_ORIG int Game_SetSlot44C(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(XI_GAME(0x101037F0u))(a0, a1); }
XI_ORIG undefined4 Unit100D7420_ReturnZero() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x101038C0u))(); }
XI_ORIG undefined4 Game_GetField4() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x101038D0u))(); }
XI_ORIG undefined4 Unit100D7420_GetWord8Of104E0AF4() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x101038E0u))(); }
XI_ORIG int ntGameTimeGet() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101039E0u))(); }
XI_ORIG uint32_t Time_ModDiv6C00Scaled(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x10103AE0u))(a0); }
XI_ORIG undefined4 Time_ModDivD80Div90(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10103B10u))(a0); }
XI_ORIG uint32_t Time_ModMul3CDiv90(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x10103B30u))(a0); }
XI_ORIG uint32_t FUN_10103b60(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x10103B60u))(a0); }
XI_ORIG void Unit100D7420_SetZero(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10103F50u))(a0); }
XI_ORIG undefined4 Unit100D7420_AppendMax20(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10107610u))(a0, a1); }
XI_ORIG void Unit100D7420_DumpHexBytes() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10108150u))(); }
XI_ORIG void Unit100D7420_InitDumpTable(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10108300u))(a0, a1, a2); }
XI_ORIG int Unit100D7420_GetPoolBlockPayload(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x101083C0u))(a0); }
XI_ORIG undefined4 * FUN_10108460(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10108460u))(a0, a1); }
XI_ORIG undefined4 Unit100D7420_AllocEntry(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10108590u))(a0, a1, a2); }
XI_ORIG undefined4 Unit100D7420_AllocFromPool(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101085C0u))(a0, a1); }
XI_ORIG undefined4 Unit100D7420_AllocFromPool0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101085E0u))(a0); }
XI_ORIG void thunk_FUN_10108600(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x101086B0u))(a0); }
XI_ORIG void Unit100D7420_InsertBlock(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10108700u))(a0, a1); }
XI_ORIG void Unit100D7420_InsertAndCoalesce(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10108780u))(a0, a1); }
XI_ORIG uint32_t Unit100D7420_GetNextBlock(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10108870u))(a0, a1); }
XI_ORIG int Unit100D7420_GetPrevBlock(XI_ANY a0) { return reinterpret_cast<int (__stdcall *)(XI_ANY)>(XI_GAME(0x101088A0u))(a0); }
XI_ORIG void Unit100D7420_RebalanceList(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101088C0u))(a0); }
XI_ORIG void Unit100D7420_UnlinkBlock(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10108D80u))(a0, a1); }
XI_ORIG void Unit100D7420_LinkAfter(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10108E80u))(a0, a1, a2); }
XI_ORIG void Unit100D7420_LinkBefore(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10108F40u))(a0, a1, a2); }
XI_ORIG undefined4 * Unit100D7420_AllocCopy(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 * (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10109000u))(a0, a1, a2, a3); }
XI_ORIG void Unit100D7420_Release(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101090A0u))(a0); }
XI_ORIG void Unit100D7420_RefStringClear(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10109100u))(a0); }
XI_ORIG void * Unit100D7420_RefStringCtor(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10109140u))(a0, a1); }
XI_ORIG int Unit100D7420_RefStringReserve(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101091D0u))(a0, a1); }
XI_ORIG unsigned long long Unit100D7420_RefStringAssign(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101092C0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Unit100D7420_RefStringRelease(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101094D0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Unit100D7420_Copy12(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1010E040u))(a0, a1); }
XI_ORIG void Unit100D7420_CreateEventRecorder() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1010F8B0u))(); }
XI_ORIG void Unit100D7420_RecorderAddHandler(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1010FA30u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void Unit100D7420_RecorderRecordCode(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1010FBD0u))(a0, a1, a2); }
XI_ORIG void Unit100D7420_RegisterHandlerByName(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1010FC50u))(a0, a1, a2, a3); }
XI_ORIG void FUN_1010feb0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1010FEB0u))(); }
XI_ORIG void Unit100D7420_ForwardToGlobal1708(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1010FEB0u))(a0, a1); }
XI_ORIG void Unit100D7420_Copy12B(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1010FEE0u))(a0, a1); }
XI_ORIG void Unit100D7420_ListInsertBefore(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10110040u))(a0, a1); }
XI_ORIG void Unit100D7420_ResetB44() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10113220u))(); }
XI_ORIG void Unit100D7420_ClearTable1710() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x101133A0u))(); }
XI_ORIG undefined8 FUN_10113ca0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10113CA0u))(a0, a1, a2); }
XI_ORIG void * FDtXiErrorReportCtrl_Constructor(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10113D30u))(a0); }
XI_ORIG void Unit100D7420_ErrorLogDtor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10114070u))(a0); }
XI_ORIG undefined4 * Unit100D7420_ErrorLogPop(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10114120u))(a0, a1); }
XI_ORIG void FUN_10114250(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10114250u))(a0, a1); }
XI_ORIG void Unit100D7420_ErrorLogFlush(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10114320u))(a0); }
XI_ORIG void Unit100D7420_ErrorLogReplay(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10114500u))(a0); }
XI_ORIG void Unit100D7420_ReportError(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10114F10u))(a0, a1, a2); }
XI_ORIG void FDtGmNoticeProc() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10115020u))(); }
XI_ORIG undefined4 * Unit100D7420_CreateSession(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101152F0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined4 Unit100D7420_CreateSessionFromParams(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101154D0u))(a0); }
XI_ORIG undefined4 Unit100D7420_StartChannel(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10115520u))(a0); }
XI_ORIG undefined4 Unit100D7420_QueueRequest26(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10115580u))(a0); }
XI_ORIG undefined4 Unit100D7420_GetField13C(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101164B0u))(a0, a1); }
XI_ORIG uint32_t Unit100D7420_GetVerifiedSize(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x10116520u))(a0); }
XI_ORIG undefined4 Unit100D7420_VerifyMd4(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10116540u))(a0); }
XI_ORIG void Unit100D7420_InitHeader(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101165F0u))(a0, a1); }
XI_ORIG void FUN_10116610() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10116610u))(); }
XI_ORIG void Unit100D7420_BuildRequestWithMd5(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10116610u))(a0, a1, a2); }
XI_ORIG undefined4 Unit100D7420_ComputeMd4(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10116C90u))(a0); }
XI_ORIG int Unit100D7420_NextSequence(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10116D30u))(a0, a1); }
XI_ORIG void Unit100D7420_ComputeDigest16(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10116D80u))(a0, a1, a2, a3); }
XI_ORIG undefined4 Unit100D7420_ApplyEntrySequence(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10116DE0u))(a0, a1); }
XI_ORIG undefined8 Unit100D7420_SetPair(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101170D0u))(a0, a1, a2); }
XI_ORIG void * Unit100D7420_MenuCtor(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101172C0u))(a0); }
XI_ORIG void Unit100D7420_DeleteAllEntries(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10117460u))(a0); }
XI_ORIG undefined8 FUN_101177d0(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x101177D0u))(a0); }
XI_ORIG unsigned long long Unit100D7420_MenuReleaseItemCount(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10117EE0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Unit100D7420_InvokeChild34(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10117F30u))(a0); }
XI_ORIG void Unit100D7420_Rebuild(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10117F40u))(a0, a1); }
XI_ORIG void Unit100D7420_ClearEntries(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101180F0u))(a0); }
XI_ORIG void Unit100D7420_AttachSelection(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10118170u))(a0, a1); }
XI_ORIG undefined8 FUN_101181d0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101181D0u))(a0, a1); }
XI_ORIG short * Unit100D7420_FindFreeEntry(void * a0) { return reinterpret_cast<short * (__thiscall *)(void *)>(XI_GAME(0x10118220u))(a0); }
XI_ORIG unsigned long long Unit100D7420_MenuMoveSelection(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10118260u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG int Unit100D7420_GetFocusedValueA(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101185E0u))(a0); }
XI_ORIG int Unit100D7420_GetFocusedValueB(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10118640u))(a0); }
XI_ORIG undefined8 Unit100D7420_MenuUpdateFocusedHelp(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x101186A0u))(a0); }
XI_ORIG void Unit100D7420_MenuStartFade(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101187F0u))(a0, a1); }
XI_ORIG undefined4 FUN_10118830(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10118830u))(a0); }
XI_ORIG undefined4 Unit100D7420_SelectFirstVisible(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10118870u))(a0); }
XI_ORIG undefined4 Unit100D7420_SetStateByIndex(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10118B60u))(a0, a1, a2); }
XI_ORIG uint32_t Unit100D7420_HitTest(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10118C10u))(a0, a1); }
XI_ORIG undefined4 Unit100D7420_PickEntry(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10118CB0u))(a0, a1, a2); }
XI_ORIG unsigned long long Unit100D7420_SelectEntryMoveCursor(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[2] = {a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10118DB0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Unit100D7420_SelectEntry(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10118E90u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Unit100D7420_SelectEntry_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10118E90u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Unit100D7420_CallMember0CSlot2(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10118F30u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Unit100D7420_FreeChild(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10118F40u))(a0); }
XI_ORIG undefined8 Unit100D7420_GetRect(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10118F70u))(a0, a1); }
XI_ORIG void Unit100D7420_SetState10ForIndex58(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101190F0u))(a0); }
XI_ORIG undefined4 FUN_10119110(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10119110u))(a0, a1, a2); }
XI_ORIG void Unit100D7420_SetActive(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10119200u))(a0, a1); }
XI_ORIG void Unit100D7420_MarkDirty(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101192E0u))(a0, a1); }
XI_ORIG void * Unit100D7420_InitScaled(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10119300u))(a0); }
XI_ORIG void Unit100D7420_FreeEntryData(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10119360u))(a0); }
XI_ORIG void Unit100D7420_ClearField34(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10119390u))(a0); }
XI_ORIG undefined4 Unit100D7420_SetSource(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101193D0u))(a0, a1); }
XI_ORIG undefined4 Unit100D7420_ComputeRect(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101193F0u))(a0, a1, a2, a3); }
XI_ORIG undefined8 Unit100D7420_UpdateLayout(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10119550u))(a0, a1); }
XI_ORIG undefined4 Unit100D7420_MapKind119870(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10119870u))(a0); }
XI_ORIG void * Unit100D7420_LayoutCtor(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101198D0u))(a0, a1); }
XI_ORIG undefined8 Unit100D7420_SetState(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10119970u))(a0, a1); }
XI_ORIG void Unit100D7420_SetState_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10119970u))(a0, a1); }
XI_ORIG undefined4 Unit100D7420_SetNames(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10119A50u))(a0, a1); }
XI_ORIG undefined4 Unit100D7420_ComputeOffsets(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10119AD0u))(a0, a1, a2, a3); }
XI_ORIG uint32_t FUN_10119bc0(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10119BC0u))(a0, a1); }
XI_ORIG unsigned long long Unit100D7420_UpdateState(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10119C00u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void * Unit100D7420_InitDefaults(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10119DA0u))(a0); }
XI_ORIG void Unit100D7420_SetShape(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10119E20u))(a0, a1); }
XI_ORIG undefined4 Unit100D7420_AttachTo(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10119E30u))(a0, a1); }
XI_ORIG void Unit100D7420_ResolveShape(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10119E70u))(a0); }
XI_ORIG unsigned long long FUN_10119f20(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10119F20u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined8 FUN_10119fb0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10119FB0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG undefined4 Unit100D7420_InvokeCallback7C(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1011A3B0u))(a0); }
XI_ORIG void Unit100D7420_ApplyHelpAndTitle(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1011A490u))(a0); }
XI_ORIG unsigned long long FUN_1011a560(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1011A560u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_1011a5a0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1011A5A0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG undefined4 Unit100D7420_GetCenter(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1011ABF0u))(a0, a1); }
XI_ORIG undefined4 FUN_1011ae80(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1011AE80u))(a0); }
XI_ORIG uint32_t Unit100D7420_GetFieldByIndex(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1011B220u))(a0, a1, a2); }
XI_ORIG int Unit100D7420_GetEntryValueById(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1011B2A0u))(a0, a1); }
XI_ORIG int Unit100D7420_GetEntry(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1011B2E0u))(a0, a1); }
XI_ORIG undefined4 Unit100D7420_GetField8(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1011B310u))(a0); }
XI_ORIG undefined4 Unit100D7420_GetFieldC(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1011B320u))(a0); }
XI_ORIG void Unit100D7420_GetOffsets(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1011B330u))(a0, a1, a2); }
XI_ORIG undefined8 FUN_1011ba40(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x1011BA40u))(a0); }
XI_ORIG int Unit100D7420_InitColors(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x1011BA90u))(a0); }
XI_ORIG char Font_BindGlyphTexture(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x1011BAE0u))(a0); }
XI_ORIG undefined4 Font_BindGlyphTexture_v1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1011BAE0u))(a0); }
XI_ORIG int Font_DrawGlyphQuadGrey(void * a0, __int16 a1, void * a2, float a3, float a4) { return reinterpret_cast<int (__thiscall *)(void *, __int16, void *, float, float)>(XI_GAME(0x1011BBA0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 Font_DrawGlyphQuadGrey_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1011BBA0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 Font_DrawGlyphQuad(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1011BBD0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int Gfx_SetAdditiveBlend() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1011BDC0u))(); }
XI_ORIG void Gfx_SetAdditiveBlend_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1011BDC0u))(); }
XI_ORIG void Font_DrawChar(__int16 a0, int a1, int a2, float a3, float a4) { reinterpret_cast<void (__cdecl *)(__int16, int, int, float, float)>(XI_GAME(0x1011BF40u))(a0, a1, a2, a3, a4); }
XI_ORIG int Font_MeasureString(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x1011BFD0u))(a0, a1); }
XI_ORIG undefined8 Obj_104E1B90_DrawString(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1011C080u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG int Gfx_SetCellBlendMode(int a0, int a1, int a2, int a3) { return reinterpret_cast<int (__cdecl *)(int, int, int, int)>(XI_GAME(0x1011C1F0u))(a0, a1, a2, a3); }
XI_ORIG int Font_AllocHighResImage(void * a0, int a1, int a2) { return reinterpret_cast<int (__stdcall *)(void *, int, int)>(XI_GAME(0x1011C5D0u))(a0, a1, a2); }
XI_ORIG char Tex_LoadSurfaceFromDatFile(void * a0, int a1) { return reinterpret_cast<char (__stdcall *)(void *, int)>(XI_GAME(0x1011C670u))(a0, a1); }
XI_ORIG void Font_LoadHighResTexture(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1011C720u))(a0); }
XI_ORIG int Font_ReleaseHighResTexture(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1011C810u))(a0); }
XI_ORIG uint8_t Font_IsHighResTextureLoaded(int a0) { return reinterpret_cast<uint8_t (__thiscall *)(int)>(XI_GAME(0x1011C850u))(a0); }
XI_ORIG int unknown_libname_88(_DWORD a0) { return reinterpret_cast<int (__thiscall *)(_DWORD)>(XI_GAME(0x1011C870u))(a0); }
XI_ORIG char Font_LoadWidthTable() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x1011C8B0u))(); }
XI_ORIG undefined4 Font_IsSingleByte(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1011C900u))(a0); }
XI_ORIG undefined8 Sjis_ToJisIndex(XI_ANY a0) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY)>(XI_GAME(0x1011C940u))(a0); }
XI_ORIG __int16 Font_DecodeNextChar(void * a0) { return reinterpret_cast<__int16 (__cdecl *)(void *)>(XI_GAME(0x1011CE80u))(a0); }
XI_ORIG int Font_DecodeNextChar_v1(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x1011CE80u))(a0); }
XI_ORIG int Font_GetCharWidth(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x1011CEC0u))(a0); }
XI_ORIG int FUN_1011cf70() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1011CF70u))(); }
XI_ORIG int Font_MeasureText(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1011CF70u))(a0, a1); }
XI_ORIG void Font_MeasureTextAll(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1011D000u))(a0); }
XI_ORIG __int16 sub_1011D050(void * a0) { return reinterpret_cast<__int16 (__cdecl *)(void *)>(XI_GAME(0x1011D050u))(a0); }
XI_ORIG undefined4 FUN_1011d290(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1011D290u))(a0); }
XI_ORIG char Font_AddGlyphAdvance(unsigned __int16 a0, void * a1) { return reinterpret_cast<char (__cdecl *)(unsigned __int16, void *)>(XI_GAME(0x1011D840u))(a0, a1); }
XI_ORIG unsigned long long Font_AddGlyphAdvance_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[2] = {a0.bits, a1.bits}; uint32_t ra = a2.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $8, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1011D840u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Font_IsPrintableAscii(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1011D8B0u))(a0); }
XI_ORIG unsigned long long Font_GetCharWidthAt(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = 0, rc = 0, rd = a1.bits;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1011D990u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Sjis_IsLeadByte(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1011DA30u))(a0); }
XI_ORIG int Sjis_GetCharByteLen(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x1011DA80u))(a0); }
XI_ORIG undefined4 Sjis_GetCharByteLen_v1(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1011DA80u))(a0); }
XI_ORIG undefined4 FUN_1011daa0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1011DAA0u))(a0); }
XI_ORIG undefined4 Sjis_IsCodeInSet(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1011DB40u))(a0); }
XI_ORIG undefined4 FUN_1011dc00(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1011DC00u))(a0, a1); }
XI_ORIG undefined4 FUN_1011dcc0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1011DCC0u))(a0, a1); }
XI_ORIG size_t Sjis_CopyPrefix(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<size_t (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1011DE80u))(a0, a1, a2); }
XI_ORIG int Cell_GetField20Ptr(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1011E290u))(a0); }
XI_ORIG void Cell_ResolveSubCells(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1011E2A0u))(a0); }
XI_ORIG undefined4 Cell_ResolveTextureUV(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1011E6D0u))(a0); }
XI_ORIG undefined8 Cell_DrawStretchedWrapped(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9, XI_ANY a10, XI_ANY a11) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1011EF60u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11); }
XI_ORIG int Cell_DrawWithDefaultBlend(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1011F2D0u))(a0); }
XI_ORIG int Cell_DrawQuadBlended(void * a0, int a1, int a2, int a3, int a4, int a5) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int, int)>(XI_GAME(0x1011F300u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void Cell_DrawQuadBlended_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1011F300u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int Cell_GetFrameBounds(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1011FA80u))(a0, a1, a2); }
XI_ORIG undefined4 Unit100D7420_GetNthEntryBytePair(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1011FB00u))(a0, a1, a2); }
XI_ORIG undefined8 FUN_1011fb60(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1011FB60u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int Shape_DrawScaled(int a0, int a1, int a2, int a3, int a4, int a5, char a6, int a7, int a8) { return reinterpret_cast<int (__thiscall *)(int, int, int, int, int, int, char, int, int)>(XI_GAME(0x1011FC80u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG undefined8 Shape_DrawScaled_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1011FC80u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG void Cell_DrawFirstStretched(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9, XI_ANY a10, XI_ANY a11) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10120000u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11); }
XI_ORIG void Cell_ResolveTextureList(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10120070u))(a0); }
XI_ORIG int Color_ModulateArgb128(unsigned int a0, unsigned int a1) { return reinterpret_cast<int (__cdecl *)(unsigned int, unsigned int)>(XI_GAME(0x101200B0u))(a0, a1); }
XI_ORIG _DWORD * ShapeMng_Init(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x101201C0u))(a0); }
XI_ORIG _DWORD * sub_10120200(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10120200u))(a0); }
XI_ORIG char * ShapeMng_FindShape(void * a0, void * a1) { return reinterpret_cast<char * (__thiscall *)(void *, void *)>(XI_GAME(0x10120450u))(a0, a1); }
XI_ORIG undefined8 ShapeMng_FindShape_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10120450u))(a0, a1); }
XI_ORIG undefined4 ShapeMng_FindShape_v2(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10120450u))(a0, a1); }
XI_ORIG undefined4 Cell_FindEntryByName(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101204B0u))(a0, a1); }
XI_ORIG uint8_t ShapeMng_GetFontGlyph(void * a0, unsigned __int8 a1, int a2, void * a3, void * a4, void * a5) { return reinterpret_cast<uint8_t (__thiscall *)(void *, unsigned __int8, int, void *, void *, void *)>(XI_GAME(0x10120510u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG char ShapeMng_BindFontTexture(void * a0, unsigned int a1, void * a2, void * a3) { return reinterpret_cast<char (__thiscall *)(void *, unsigned int, void *, void *)>(XI_GAME(0x10120590u))(a0, a1, a2, a3); }
XI_ORIG char * ShapeMng_LoadFontTextures(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10120660u))(a0); }
XI_ORIG char * ShapeMng_LoadDmgNumTextures(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10120880u))(a0); }
XI_ORIG void Cell_ResolveAllSubCells(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10120950u))(a0); }
XI_ORIG undefined4 Cell_TestAny38Or0B8() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10120B80u))(); }
XI_ORIG undefined4 Cell_TestAny1DOr9D() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10120BB0u))(); }
XI_ORIG undefined4 Cell_TestAny2AOr36() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10120BE0u))(); }
XI_ORIG long double FUN_10122e30(XI_ANY a0) { return reinterpret_cast<long double (__cdecl *)(XI_ANY)>(XI_GAME(0x10122E30u))(a0); }
XI_ORIG undefined4 Cell_TestFlagAny2AOr36(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10123350u))(a0); }
XI_ORIG undefined4 Cell_TestFlagKind(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10123380u))(a0, a1); }
XI_ORIG undefined4 Cell_GetWord1036D6B0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10123670u))(a0); }
XI_ORIG long double GetAnalogKey(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<long double (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10123970u))(a0, a1); }
XI_ORIG unsigned long long Unit100D7420_TestCommand36CF60(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) {
    uint32_t xs[4] = {a0.bits, a1.bits, a2.bits, a3.bits}; uint32_t ra = a4.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 12(%%esi)\n\t""pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $16, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10123A70u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Unit100D7420_TestCommand36CF60_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) {
    uint32_t xs[4] = {a0.bits, a1.bits, a2.bits, a3.bits}; uint32_t ra = a4.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 12(%%esi)\n\t""pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $16, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10123A70u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Cursor_TestAny1DOr9D() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10123DC0u))(); }
XI_ORIG undefined4 Cursor_TestAnyB8Or38() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10123DF0u))(); }
XI_ORIG undefined4 Cell_TestFlagAny2AOr36Kind4() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10123E20u))(); }
XI_ORIG undefined4 Cell_TestGlobal1036E3A0Mask(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10123E50u))(a0); }
XI_ORIG unsigned long long Cell_SetByte104E1C3E(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10123E60u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Cell_ClearFlag104E1C3C() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10123E90u))(); }
XI_ORIG void Cell_SetFlag104E1C3C() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10123EA0u))(); }
XI_ORIG unsigned long long Cell_GetByte1036CF5C(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10123EE0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Cell_ClearFlag01And08(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10124010u))(a0, a1); }
XI_ORIG undefined4 Cell_ClearAllFlags(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10124050u))(a0); }
XI_ORIG undefined4 Cell_TestFlagBit1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10124070u))(a0, a1); }
XI_ORIG undefined4 Cell_TakeFlagBit2(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10124090u))(a0, a1); }
XI_ORIG undefined4 Cell_TestFlagBit3(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101240B0u))(a0, a1); }
XI_ORIG undefined4 Cell_TestFlagBit0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101240D0u))(a0, a1); }
XI_ORIG undefined4 Cursor_TakeEventFlag(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101240F0u))(a0, a1); }
XI_ORIG undefined4 Cursor_GetFlagBit4(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10124110u))(a0, a1); }
XI_ORIG undefined4 Cursor_GetFlagBit2(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10124130u))(a0, a1); }
XI_ORIG __int16 Cursor_GetHwCursorSlot(void * a0) { return reinterpret_cast<__int16 (__thiscall *)(void *)>(XI_GAME(0x101245C0u))(a0); }
XI_ORIG void CTkMouse_OnMouseMove(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10124640u))(a0, a1, a2); }
XI_ORIG void Cursor_SetPosition(int a0, void * a1, float a2) { reinterpret_cast<void (__thiscall *)(int, void *, float)>(XI_GAME(0x10124700u))(a0, a1, a2); }
XI_ORIG unsigned long long Cursor_SetPosition_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[2] = {a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10124700u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Cursor_UpdateTimer(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101248A0u))(a0); }
XI_ORIG void Cursor_EnterMode1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10124930u))(a0); }
XI_ORIG void Cursor_ToggleFlag1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101249E0u))(a0); }
XI_ORIG void Cursor_EnterMode2(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10124A00u))(a0); }
XI_ORIG void Cursor_ToggleFlag2(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10124AB0u))(a0); }
XI_ORIG void Cursor_EnterMode4(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10124AD0u))(a0); }
XI_ORIG void Cursor_ToggleFlag4(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10124B50u))(a0); }
XI_ORIG undefined4 Cursor_ResetPositionToMenu(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10124B70u))(a0); }
XI_ORIG undefined8 FUN_10124bc0(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x10124BC0u))(a0); }
XI_ORIG unsigned long long Cursor_Draw(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {0}; uint32_t ra = a2.bits, rc = a0.bits, rd = a1.bits;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10124EB0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Cursor_SetModeFromMenuActive(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101252C0u))(a0); }
XI_ORIG unsigned __int8 Cursor_SetVisible(void * a0, unsigned __int8 a1) { return reinterpret_cast<unsigned __int8 (__thiscall *)(void *, unsigned __int8)>(XI_GAME(0x101252F0u))(a0, a1); }
XI_ORIG undefined8 Cursor_SetVisible_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101252F0u))(a0, a1); }
XI_ORIG undefined4 Cursor_UpdateVisibilityForMode(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10125F70u))(a0, a1); }
XI_ORIG long double FUN_101260d0(void * a0) { return reinterpret_cast<long double (__thiscall *)(void *)>(XI_GAME(0x101260D0u))(a0); }
XI_ORIG int Cursor_StoreScreenCenter(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101262A0u))(a0); }
XI_ORIG void Cursor_StoreScreenCenter_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101262A0u))(a0); }
XI_ORIG undefined4 Unit100D7420_Init(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10126300u))(a0); }
XI_ORIG undefined4 Unit100D7420_ClearWords(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10126320u))(a0); }
XI_ORIG void Unit100D7420_ClearWords_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10126320u))(a0); }
XI_ORIG void Unit100D7420_NoOp() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10126330u))(); }
XI_ORIG void Unit100D7420_Clear(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10126350u))(a0); }
XI_ORIG void FUN_10126370(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10126370u))(a0); }
XI_ORIG undefined4 * Unit100D7420_Alloc(void * a0) { return reinterpret_cast<undefined4 * (__thiscall *)(void *)>(XI_GAME(0x101263B0u))(a0); }
XI_ORIG void FUN_10126410(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x10126410u))(a0); }
XI_ORIG undefined4 Unit100D7420_GetCount(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x10126460u))(a0); }
XI_ORIG undefined8 Unit100D7420_PushBack(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10126480u))(a0, a1); }
XI_ORIG void Unit100D7420_PushBack_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10126480u))(a0, a1); }
XI_ORIG void Unit100D7420_PushFront(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101264C0u))(a0, a1); }
XI_ORIG undefined4 Unit100D7420_GetHead(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10126500u))(a0); }
XI_ORIG undefined4 Unit100D7420_GetTail(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10126510u))(a0); }
XI_ORIG undefined8 FUN_10126520(XI_ANY a0) { return reinterpret_cast<undefined8 (__stdcall *)(XI_ANY)>(XI_GAME(0x10126520u))(a0); }
XI_ORIG undefined8 FUN_10126550(XI_ANY a0) { return reinterpret_cast<undefined8 (__stdcall *)(XI_ANY)>(XI_GAME(0x10126550u))(a0); }
XI_ORIG void Unit100D7420_Remove(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10126580u))(a0, a1); }
XI_ORIG void FUN_101265d0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101265D0u))(a0); }
XI_ORIG void Unit100D7420_InsertAfter(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10126700u))(a0, a1, a2); }
XI_ORIG void * Unit100D7420_ChatListInit(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10126790u))(a0, a1); }
XI_ORIG undefined4 Unit100D7420_AddMessage(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101268F0u))(a0, a1, a2, a3); }
XI_ORIG void * Unit100D7420_MessageCtor(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10126B60u))(a0, a1, a2); }
XI_ORIG void Unit100D7420_MessageSet(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10126BC0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined8 FUN_10126c60(void * a0, void * a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined8 (__fastcall *)(void *, void *, XI_ANY, XI_ANY)>(XI_GAME(0x10126C60u))(a0, a1, a2, a3); }
XI_ORIG _DWORD * Unit100D7420_MsgWinDataCtor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10127190u))(a0); }
XI_ORIG void Unit100D7420_MsgWinDataReset(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101271C0u))(a0); }
XI_ORIG undefined8 Unit100D7420_ResetEntryRing(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x10127250u))(a0); }
XI_ORIG void Unit100D7420_BuildRecord(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101273C0u))(a0, a1, a2, a3); }
XI_ORIG uint32_t Unit100D7420_FindByKeyByte(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x10127870u))(a0); }
XI_ORIG char * Unit100D7420_FindChar(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<char * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101278B0u))(a0, a1); }
XI_ORIG undefined4 Unit100D7420_FindSubstringIndex(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101278D0u))(a0, a1, a2); }
XI_ORIG void Unit100D7420_CopyLimited(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10127930u))(a0, a1, a2); }
XI_ORIG void Unit100D7420_ToLowerInPlace(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10127970u))(a0); }
XI_ORIG uint32_t GetCallNum(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10127990u))(a0, a1); }
XI_ORIG undefined4 Unit100D7420_IsNearNamedActor(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10127AB0u))(a0); }
XI_ORIG char ChatLog_AddMessage(int a0, void * a1, void * a2, int a3, char a4, void * a5) { return reinterpret_cast<char (__thiscall *)(int, void *, void *, int, char, void *)>(XI_GAME(0x10127C00u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined4 ChatLog_AddMessage_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10127C00u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined4 FUN_10127ea0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10127EA0u))(); }
XI_ORIG _DWORD * Unit100D7420_CtorResetAll(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10127EE0u))(a0); }
XI_ORIG void Unit100D7420_ResetAll(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10127F00u))(a0); }
XI_ORIG undefined8 FUN_101284f0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101284F0u))(a0, a1, a2, a3, a4); }
XI_ORIG int Unit100D7420_CountTextChars(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x10128680u))(a0); }
XI_ORIG void Unit100D7420_CountTextCharsAndBytes(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10128830u))(a0, a1, a2); }
XI_ORIG undefined8 Unit100D7420_BuildTextCharIndex(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10128950u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10128ce0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10128CE0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG unsigned long long Unit100D7420_ClearFlag12FromValue(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10129290u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Unit100D7420_RingStepBack(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[2] = {a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101292E0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_101295d0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101295D0u))(a0, a1, a2, a3, a4); }
XI_ORIG unsigned long long Unit100D7420_GetRingHead14(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10129C60u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined8 Unit100D7420_FindKindRecord(XI_ANY a0) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY)>(XI_GAME(0x10129CD0u))(a0); }
XI_ORIG undefined4 Unit100D7420_MapCode(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10129D00u))(a0); }
XI_ORIG uint32_t FUN_10129d90(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10129D90u))(a0, a1); }
XI_ORIG undefined1 FUN_10129de0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined1 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10129DE0u))(a0, a1); }
XI_ORIG undefined4 FUN_10129e30(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10129E30u))(a0, a1); }
XI_ORIG uint32_t FUN_10129e30_v1(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10129E30u))(a0, a1); }
XI_ORIG undefined4 Unit100D7420_MapCode54ToIndex(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1012A120u))(a0); }
XI_ORIG void FUN_1012a340() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1012A340u))(); }
XI_ORIG void Unit100D7420_ClearBuffers(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1012A3B0u))(a0); }
XI_ORIG undefined4 FUN_1012c8c0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1012C8C0u))(a0, a1); }
XI_ORIG undefined4 FUN_1012c8e0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1012C8E0u))(a0); }
XI_ORIG undefined8 thunk_FUN_1011ba40(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x1012C900u))(a0); }
XI_ORIG undefined4 FUN_1012ca90(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x1012CA90u))(a0); }
XI_ORIG undefined4 FUN_1012cad0() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x1012CAD0u))(); }
XI_ORIG void Unit1012AC80_ConstructVt10333648(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10130F90u))(a0); }
XI_ORIG undefined4 * Unit1012AC80_ConstructVt10333660(void * a0) { return reinterpret_cast<undefined4 * (__thiscall *)(void *)>(XI_GAME(0x101312E0u))(a0); }
XI_ORIG void * Unit1012AC80_ConstructVt10333670(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10131530u))(a0); }
XI_ORIG void * Unit1012AC80_ConstructVt10333908(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10136380u))(a0, a1); }
XI_ORIG void FUN_10136670(void * a0, void * a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__fastcall *)(void *, void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10136670u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 Unit1012AC80_SetPosition(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10136870u))(a0, a1, a2, a3); }
XI_ORIG void * Unit1012AC80_ConstructVt103339F0(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10136A30u))(a0, a1); }
XI_ORIG void * Unit1012AC80_ConstructVt10333AE0(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10136F60u))(a0, a1); }
XI_ORIG void Unit1012AC80_ClearRanges(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10138EC0u))(a0); }
XI_ORIG unsigned long long FUN_10139f40(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) {
    uint32_t xs[3] = {a1.bits, a2.bits, a3.bits}; uint32_t ra = a4.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10139F40u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_10139fb0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10139FB0u))(a0, a1); }
XI_ORIG unsigned long long thunk_FUN_1013a100(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10139FE0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Unit1012AC80_TickHelp(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1013A010u))(a0); }
XI_ORIG int TkHelp_AdvanceTimer(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1013A030u))(a0); }
XI_ORIG unsigned long long Unit1012AC80_IsEvent(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1013A060u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Unit1012AC80_AdvanceHelpTimer(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1013A070u))(a0); }
XI_ORIG void FUN_1013a0d0() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x1013A0D0u))(); }
XI_ORIG unsigned long long FUN_1013a100(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1013A100u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Unit1012AC80_ForwardToPanel(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1013A320u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1013a470(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1013A470u))(a0); }
XI_ORIG void * Unit1012AC80_CtorVt10333EC0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1013BE50u))(a0); }
XI_ORIG void FUN_1013bf40(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1013BF40u))(a0); }
XI_ORIG void * Unit1012AC80_Construct(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10144F40u))(a0); }
XI_ORIG void Unit1012AC80_Set(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10146930u))(a0, a1, a2, a3, a4); }
XI_ORIG void Unit1012AC80_SetCopy(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10146960u))(a0, a1, a2, a3, a4); }
XI_ORIG void * Unit1012AC80_CtorVt10334970(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101475B0u))(a0); }
XI_ORIG void * Unit1012AC80_CtorVt103349B8(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10148430u))(a0); }
XI_ORIG void * Unit1012AC80_CtorVt10334A00(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10148640u))(a0); }
XI_ORIG void * YkWndList_Construct(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10148ED0u))(a0); }
XI_ORIG void * Unit1012AC80_CtorVt10334AB8(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1014A170u))(a0); }
XI_ORIG void * Unit1012AC80_CtorVt10334B28(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1014A7D0u))(a0); }
XI_ORIG void * Unit1012AC80_CtorVt10334B98(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1014B670u))(a0); }
XI_ORIG void * Unit1012AC80_CtorVt10334C08(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1014BA60u))(a0); }
XI_ORIG void FUN_1014e020(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1014E020u))(a0); }
XI_ORIG void Unit1012AC80_SetName(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10150A30u))(a0, a1); }
XI_ORIG void Unit1012AC80_SetField48(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10150A60u))(a0, a1); }
XI_ORIG void Unit1012AC80_SetTextFwd(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10150CC0u))(a0, a1, a2); }
XI_ORIG undefined Unit1012AC80_SetTextFwd_v1(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x10150CC0u))(a0, a1); }
XI_ORIG void Unit1012AC80_SetText(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10150CE0u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10150d60(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10150D60u))(a0); }
XI_ORIG void Unit1012AC80_ClearFields(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10151140u))(a0); }
XI_ORIG undefined Unit1012AC80_GetMessageFwd(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10152EF0u))(a0, a1); }
XI_ORIG void Unit1012AC80_GetMessage(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10152F20u))(a0, a1); }
XI_ORIG int Unit1012AC80_Clear32() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10152F80u))(); }
XI_ORIG undefined4 Unit1012AC80_ReleaseAll32() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10152FA0u))(); }
XI_ORIG undefined4 Unit1012AC80_ReleaseOne(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10152FC0u))(a0); }
XI_ORIG undefined4 Unit1012AC80_LoadMessageBlock(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10152FF0u))(a0, a1); }
XI_ORIG int FUN_101530c0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101530C0u))(); }
XI_ORIG int Unit1012AC80_GetEntryPtrByLayout(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101530C0u))(a0, a1); }
XI_ORIG int Unit1012AC80_GetEntryWord3E(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10153100u))(a0, a1); }
XI_ORIG int Unit1012AC80_Read2Byte7Bit(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x101531B0u))(a0); }
XI_ORIG int FUN_10153310(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10153310u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined4 Unit1012AC80_FormatMessageWithArgs(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10153A40u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined4 Unit1012AC80_LoadMessageBlocksSetA() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10153AF0u))(); }
XI_ORIG undefined4 Unit1012AC80_LoadMessageBlocksSetB() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10153B30u))(); }
XI_ORIG undefined4 FUN_10153b70() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10153B70u))(); }
XI_ORIG char Unit1012AC80_LoadAllMessageTables(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10153C10u))(a0); }
XI_ORIG undefined4 Unit1012AC80_LoadStringTable(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10153EF0u))(a0, a1); }
XI_ORIG void Unit1012AC80_DtorVt103350EC(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10153F80u))(a0); }
XI_ORIG _DWORD * Unit1012AC80_GetInstance() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x10153FF0u))(); }
XI_ORIG undefined * Unit1012AC80_GetInstance_v1() { return reinterpret_cast<undefined * (__cdecl *)()>(XI_GAME(0x10153FF0u))(); }
XI_ORIG void * Unit1012AC80_MessageSetCtor(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10154070u))(a0); }
XI_ORIG void FUN_101540a0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101540A0u))(a0, a1); }
XI_ORIG uint32_t Unit1012AC80_LoadMessageSetByState(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x101542B0u))(a0, a1); }
XI_ORIG int Unit1012AC80_IndexOf(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101544F0u))(a0, a1); }
XI_ORIG undefined4 Unit1012AC80_GetByValue(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10154520u))(a0, a1); }
XI_ORIG undefined1 * Unit1012AC80_GetEntryOrEmpty(void * a0, XI_ANY a1) { return reinterpret_cast<undefined1 * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10154550u))(a0, a1); }
XI_ORIG void Unit1012AC80_ReleaseField7C(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10154840u))(a0); }
XI_ORIG void Unit1012AC80_UpdatePendingRead(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10154860u))(a0); }
XI_ORIG undefined4 Unit1012AC80_GetCount(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101549E0u))(a0); }
XI_ORIG undefined4 Unit1012AC80_GetWordAt(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101549F0u))(a0, a1); }
XI_ORIG void Unit1012AC80_CtorVt103350F8(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10154AB0u))(a0); }
XI_ORIG void Entry_Init6(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10154AD0u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG undefined4 FUN_10154d70(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10154D70u))(a0); }
XI_ORIG undefined4 * Unit1012AC80_CtorVt1033512C(void * a0) { return reinterpret_cast<undefined4 * (__thiscall *)(void *)>(XI_GAME(0x10154DC0u))(a0); }
XI_ORIG int Unit1012AC80_GetInnerPtr10(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10154F00u))(a0); }
XI_ORIG void Unit1012AC80_DtorVt10335138(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10154F30u))(a0); }
XI_ORIG void Unit1012AC80_CtorVt10335138(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10154F60u))(a0); }
XI_ORIG void Entry_SetFields4(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10154FC0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 Unit1012AC80_DtorVt1033515C(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10155100u))(a0); }
XI_ORIG undefined4 Entry_ReleaseField4Zero(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10155140u))(a0); }
XI_ORIG void * Unit1012AC80_CtorVt1033515C(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10155160u))(a0); }
XI_ORIG int Unit1012AC80_GetMember14(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101552D0u))(a0); }
XI_ORIG undefined4 FUN_10155320(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10155320u))(a0); }
XI_ORIG void * Unit1012AC80_CtorVt10335168(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10155370u))(a0); }
XI_ORIG int Unit1012AC80_GetMemberC(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101554E0u))(a0); }
XI_ORIG undefined4 FUN_10155500(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10155500u))(a0); }
XI_ORIG void * Unit1012AC80_CtorVt10335174(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10155550u))(a0); }
XI_ORIG int Unit1012AC80_GetField8(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10155600u))(a0); }
XI_ORIG void FUN_101569a0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101569A0u))(a0); }
XI_ORIG void Unit1012AC80_SetFields(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101569F0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG void FUN_10156be0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10156BE0u))(a0); }
XI_ORIG void Unit1012AC80_SetField04(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10156BF0u))(a0, a1); }
XI_ORIG unsigned long long Unit1012AC80_ClearTargetAndRedraw(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10157610u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Unit1012AC80_ClearField68AndDraw(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10157620u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Unit1012AC80_ClearField68AndDraw_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10157620u))(a0); }
XI_ORIG char Unit1012AC80_SetTargetMode(int a0, int a1) { return reinterpret_cast<char (__thiscall *)(int, int)>(XI_GAME(0x10157660u))(a0, a1); }
XI_ORIG undefined4 Unit1012AC80_SetTargetMode_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10157660u))(a0, a1); }
XI_ORIG undefined4 Unit1012AC80_IsTargetUnusable() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x101576F0u))(); }
XI_ORIG void Unit1012AC80_SetValues3(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10157780u))(a0, a1, a2); }
XI_ORIG undefined4 Unit1012AC80_SetFields78D0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10157860u))(a0, a1, a2); }
XI_ORIG unsigned long long Unit1012AC80_GetByte75(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10157880u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Unit1012AC80_GetByte76(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10157890u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Unit1012AC80_GetByte77(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101578A0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Unit1012AC80_GetWord80(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101578B0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Unit1012AC80_GetField7C(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101578C0u))(a0); }
XI_ORIG undefined4 Target_GetFlag82(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101578D0u))(a0); }
XI_ORIG undefined4 Target_GetFlag83(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101578E0u))(a0); }
XI_ORIG void Target_Reset(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101578F0u))(a0); }
XI_ORIG void Unit1012AC80_ClearFields84And88(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101579B0u))(a0); }
XI_ORIG int Target_Update(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101579C0u))(a0); }
XI_ORIG undefined4 Target_Select(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10157B40u))(a0, a1, a2, a3); }
XI_ORIG char Target_SelectByIndex(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<char (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10157C60u))(a0, a1, a2, a3, a4); }
XI_ORIG int sub_10157CF0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10157CF0u))(a0); }
XI_ORIG undefined4 Menu_IsFocusNotInList() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10157D20u))(); }
XI_ORIG undefined4 Target_FindCandidate() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10157D60u))(); }
XI_ORIG void Target_CycleMenuStep(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10157E20u))(a0); }
XI_ORIG unsigned long long Target_IssueStateCommand(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10157FA0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Target_UpdateSelectionByState(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101581E0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Target_InvokeFieldCallback(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101582F0u))(a0); }
XI_ORIG void Target_UpdateActorFlags138(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101583F0u))(a0); }
XI_ORIG undefined4 Target_HasActor(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10158470u))(a0); }
XI_ORIG unsigned long long Target_UpdateSelection(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10158480u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Target_SendAttackOff(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10158510u))(a0); }
XI_ORIG int * Target_GetActor(void * a0) { return reinterpret_cast<int * (__thiscall *)(void *)>(XI_GAME(0x10158550u))(a0); }
XI_ORIG undefined4 Target_Clear(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10158590u))(a0); }
XI_ORIG undefined4 Target_RunActiveCommand(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101585F0u))(a0); }
XI_ORIG undefined4 Unit1012AC80_FindActiveCommand(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101587B0u))(a0); }
XI_ORIG undefined4 FUN_101587f0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101587F0u))(a0, a1); }
XI_ORIG unsigned long long Unit1012AC80_IsCommand76Or77Active(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10158AA0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Target_TrySubSelect(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10158B00u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Target_ActOrTrySubSelect(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10158B40u))(a0, a1); }
XI_ORIG undefined4 Target_PickNearestToPoint(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10158D90u))(a0); }
XI_ORIG undefined4 Target_ApplyAction(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10158DB0u))(a0, a1); }
XI_ORIG undefined4 Target_ClearAll(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10158FF0u))(a0); }
XI_ORIG unsigned long long Target_CommitSubTarget(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10159040u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Target_ClearAndSetFlag53(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101590A0u))(a0); }
XI_ORIG void Target_SetDisplayName(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101590C0u))(a0, a1); }
XI_ORIG void Target_SetFlagsFromMask(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10159170u))(a0, a1); }
XI_ORIG undefined4 Target_IsUsable(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101591D0u))(a0); }
XI_ORIG int Unit1012AC80_GetField84Ptr(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10159370u))(a0); }
XI_ORIG char Actor_SetSlotTableEntry(unsigned int a0, int a1) { return reinterpret_cast<char (__cdecl *)(unsigned int, int)>(XI_GAME(0x10159400u))(a0, a1); }
XI_ORIG undefined4 Actor_SetSlotTableEntry_v1(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10159400u))(a0, a1); }
XI_ORIG undefined4 Target_DispatchByIndex(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10159420u))(a0, a1); }
XI_ORIG undefined4 Target_SetIfUsable(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10159440u))(a0, a1, a2); }
XI_ORIG void Target_SetHandlerCc(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101594D0u))(a0, a1); }
XI_ORIG void Target_SetHandlerC8(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101594E0u))(a0, a1); }
XI_ORIG undefined4 Target_ReleaseSelection(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101594F0u))(a0); }
XI_ORIG void * Target_ResetRecordReturnSelf(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10159530u))(a0); }
XI_ORIG unsigned long long Target_IsRecordStillValid(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10159540u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Target_CanTarget(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101595D0u))(a0, a1); }
XI_ORIG undefined4 Target_SetActor(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10159660u))(a0, a1); }
XI_ORIG undefined4 Target_ResetRecord(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10159740u))(a0); }
XI_ORIG undefined4 Target_RefreshActor(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10159780u))(a0); }
XI_ORIG undefined4 Entry_InitKey(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101597C0u))(a0, a1, a2); }
XI_ORIG uint32_t Entry_ValidateKey(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x101597F0u))(a0); }
XI_ORIG void Entry_SetNameAndKey(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10159830u))(a0, a1, a2); }
XI_ORIG undefined4 Target_GetPosShortsMainOrSub(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10159880u))(a0, a1, a2, a3); }
XI_ORIG undefined4 Target_GetCachedIfValid(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101598A0u))(a0); }
XI_ORIG undefined4 Target_GetActorPtr(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101598B0u))(a0); }
XI_ORIG undefined4 Unit1012AC80_IsWithinRadius(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101598C0u))(a0, a1); }
XI_ORIG uint32_t Entry_MatchesPair(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10159910u))(a0, a1); }
XI_ORIG undefined4 Unit1012AC80_MoveByDirection(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10159970u))(a0, a1); }
XI_ORIG undefined4 Unit1012AC80_ApplyOffset(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10159A20u))(a0, a1); }
XI_ORIG void Camera_RotateVecYaw(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x10159B10u))(a0); }
XI_ORIG undefined4 FUN_10159ba0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10159BA0u))(a0, a1); }
XI_ORIG undefined4 FUN_10159c80(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10159C80u))(a0, a1); }
XI_ORIG undefined4 Target_GetPosShorts(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10159D40u))(a0, a1, a2); }
XI_ORIG int * Entry_ResolveValid(void * a0) { return reinterpret_cast<int * (__thiscall *)(void *)>(XI_GAME(0x10159DE0u))(a0); }
XI_ORIG unsigned long long Entry_IsActorFieldMatch(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10159E10u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined8 Unit1012AC80_FindRange(XI_ANY a0) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY)>(XI_GAME(0x10159EB0u))(a0); }
XI_ORIG int Unit1012AC80_FindRange_v1(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x10159EB0u))(a0); }
XI_ORIG undefined8 Unit1012AC80_ResetState(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x1015AFA0u))(a0); }
XI_ORIG void Unit1012AC80_ResetState_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1015AFA0u))(a0); }
XI_ORIG uint32_t Unit1012AC80_IsCachedPtrMatch(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x1015B190u))(a0); }
XI_ORIG undefined4 Unit1012AC80_GetObjectPtr(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1015B270u))(a0); }
XI_ORIG void FUN_1015b410(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1015B410u))(a0); }
XI_ORIG int Unit1012AC80_FindCachedEntry(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1015B970u))(a0, a1, a2); }
XI_ORIG undefined8 Unit1012AC80_GetCachedOrLoad(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015B9E0u))(a0, a1); }
XI_ORIG undefined * Unit1012AC80_FindEmptySlot() { return reinterpret_cast<undefined * (__cdecl *)()>(XI_GAME(0x1015BAF0u))(); }
XI_ORIG int FUN_1015bc40(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015BC40u))(a0, a1); }
XI_ORIG void Unit1012AC80_RotateBits3(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1015D7F0u))(a0); }
XI_ORIG undefined1 MenuMng_ReinitState() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x1015D860u))(); }
XI_ORIG int MenuMng_Init(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x1015D900u))(a0); }
XI_ORIG undefined4 MenuMng_ResetFields(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1015D9A0u))(a0); }
XI_ORIG _DWORD * sub_1015D9E0(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1015D9E0u))(a0); }
XI_ORIG undefined4 Unit100D7420_InsertSortedBy60(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015DB10u))(a0, a1); }
XI_ORIG void Menu_SetFlag76(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x1015DB70u))(a0); }
XI_ORIG undefined8 FUN_1015db80(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1015DB80u))(a0, a1, a2); }
XI_ORIG void FUN_1015db80_v1(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1015DB80u))(a0, a1, a2); }
XI_ORIG void Unit1012AC80_MenuForwardItem8(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1015DBB0u))(a0, a1, a2); }
XI_ORIG void Unit100D7420_RemoveMatching(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015DBD0u))(a0, a1); }
XI_ORIG int Menu_FindItemByName(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015DC10u))(a0, a1); }
XI_ORIG undefined4 Menu_LookupWindowByName(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x1015DC70u))(a0, a1); }
XI_ORIG undefined8 Unit1012AC80_MenuTest48OrStateB4(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x1015DCE0u))(a0); }
XI_ORIG undefined4 Unit1012AC80_MenuTest48OrStateB4_v1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1015DCE0u))(a0); }
XI_ORIG undefined8 Menu_HandleEvent(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015DD00u))(a0, a1); }
XI_ORIG int Menu_HandleEvent_v1(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015DD00u))(a0, a1); }
XI_ORIG void FUN_1015dd50(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015DD50u))(a0, a1); }
XI_ORIG void FUN_1015dd70(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015DD70u))(a0, a1); }
XI_ORIG void Unit1012AC80_Nop(int a0) { reinterpret_cast<void (__stdcall *)(int)>(XI_GAME(0x1015DFC0u))(a0); }
XI_ORIG int Menu_FindItemByNameAt1C(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015E030u))(a0, a1); }
XI_ORIG undefined8 FUN_1015e0f0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015E0F0u))(a0, a1); }
XI_ORIG _DWORD * MenuMng_OpenByName(void * a0, void * a1, char a2, char a3) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, char, char)>(XI_GAME(0x1015E350u))(a0, a1, a2, a3); }
XI_ORIG undefined8 MenuMng_OpenByName_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1015E350u))(a0, a1, a2, a3); }
XI_ORIG uint32_t MenuMng_OpenByName_v2(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1015E350u))(a0, a1, a2, a3); }
XI_ORIG int MenuMng_OpenByName_v3(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1015E350u))(a0, a1, a2, a3); }
XI_ORIG unsigned long long FUN_1015e530(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1015E530u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Unit1012AC80_MenuItemFade(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) {
    uint32_t xs[5] = {a1.bits, a2.bits, a3.bits, a4.bits, a5.bits}; uint32_t ra = a6.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 16(%%esi)\n\t""pushl 12(%%esi)\n\t""pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1015E6E0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Unit1012AC80_MenuInitAndFadeItem(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[2] = {a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1015E790u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Unit1012AC80_MenuInitAndFadeItem_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[2] = {a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1015E790u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined8 FUN_1015e830(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015E830u))(a0, a1); }
XI_ORIG void FUN_1015e850(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015E850u))(a0, a1); }
XI_ORIG unsigned long long Menu_SetItemAt54(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1015E850u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Menu_RemoveAndFreeItem(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1015E8A0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Menu_CloseEntry(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015E910u))(a0, a1); }
XI_ORIG void Menu_FindWindowDefByName(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x1015E950u))(a0, a1); }
XI_ORIG void Menu_CloseQuery(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015EB60u))(a0, a1); }
XI_ORIG undefined4 Menu_FindByName(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x1015EC50u))(a0); }
XI_ORIG undefined4 Menu_FindByFlags34(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x1015ECC0u))(a0); }
XI_ORIG undefined4 Menu_FindByFlags38(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1015ED10u))(a0, a1, a2); }
XI_ORIG undefined4 Menu_IsFocusedItem(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015ED80u))(a0, a1); }
XI_ORIG int Menu_HasFocusedItem(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1015EDB0u))(a0); }
XI_ORIG undefined8 Menu_NextVisibleItem(XI_ANY a0) { return reinterpret_cast<undefined8 (__stdcall *)(XI_ANY)>(XI_GAME(0x1015EE00u))(a0); }
XI_ORIG undefined8 Menu_OpenMenuWindowWhenIdle() { return reinterpret_cast<undefined8 (__cdecl *)()>(XI_GAME(0x1015EE40u))(); }
XI_ORIG unsigned long long FUN_1015eea0(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1015EEA0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Menu_CallChildSlot34ForEachItem(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1015EF20u))(a0); }
XI_ORIG undefined4 Menu_ApplyFlagsToItems(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015F3E0u))(a0, a1); }
XI_ORIG int Menu_DrawLayers(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1015F6A0u))(a0); }
XI_ORIG int Unit1012AC80_ResolveLayoutIndex(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x1015F980u))(a0); }
XI_ORIG int Menu_GetWordPtrAtA0(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015F9E0u))(a0, a1); }
XI_ORIG int sub_1015FA00(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1015FA00u))(a0); }
XI_ORIG undefined8 Menu_GetNormalizedPos(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101601D0u))(a0, a1, a2); }
XI_ORIG void Menu_GetNormalizedPos_v1(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101601D0u))(a0, a1, a2); }
XI_ORIG int MenuMng_GetUiScale(void * a0, void * a1, void * a2) { return reinterpret_cast<int (__thiscall *)(void *, void *, void *)>(XI_GAME(0x10160250u))(a0, a1, a2); }
XI_ORIG double MenuMng_GetOffsetX(void * a0, char a1) { return reinterpret_cast<double (__thiscall *)(void *, char)>(XI_GAME(0x10160290u))(a0, a1); }
XI_ORIG double MenuMng_GetOffsetY(void * a0, char a1) { return reinterpret_cast<double (__thiscall *)(void *, char)>(XI_GAME(0x101602C0u))(a0, a1); }
XI_ORIG void CTkMenuMng_IsDrawWindow(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101602F0u))(a0, a1, a2, a3); }
XI_ORIG int ** Menu_ReloadResource(int a0, int a1, void * a2, int a3) { return reinterpret_cast<int ** (__fastcall *)(int, int, void *, int)>(XI_GAME(0x10160440u))(a0, a1, a2, a3); }
XI_ORIG int Gfx_SetDefault2DStates() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101604B0u))(); }
XI_ORIG void Gfx_SetDefault2DStates_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x101604B0u))(); }
XI_ORIG int Gfx_DisableFogAndAlphaBlend() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10160650u))(); }
XI_ORIG void Gfx_DisableFogAndAlphaBlend_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10160650u))(); }
XI_ORIG int Menu_GetStateB4(_DWORD a0) { return reinterpret_cast<int (__thiscall *)(_DWORD)>(XI_GAME(0x101606F0u))(a0); }
XI_ORIG unsigned long long Menu_GetStateB4_v1(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101606F0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Menu_IsStateB4Eq1(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10160700u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_10160760(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10160760u))(a0, a1, a2); }
XI_ORIG void FUN_10160930() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10160930u))(); }
XI_ORIG void Unit101D7020_DestroyDrawMenuIfAt14() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x101609C0u))(); }
XI_ORIG void Unit101D7020_CloseIfAt14() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10160A50u))(); }
XI_ORIG undefined4 Menu_GetFocusedItem(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10160C90u))(a0); }
XI_ORIG void * Unit1012AC80_CtorVt10335350(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10161110u))(a0); }
XI_ORIG void * Unit1012AC80_CtorVt1033536C(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10161380u))(a0); }
XI_ORIG void * Unit1012AC80_CtorVt10335388(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101615E0u))(a0); }
XI_ORIG void * Unit1012AC80_InitFromScreenRect(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10161ED0u))(a0, a1); }
XI_ORIG void FUN_101626e0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101626E0u))(a0, a1); }
XI_ORIG undefined8 FUN_10162f30(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x10162F30u))(a0, a1); }
XI_ORIG undefined8 Unit1012AC80_ApplySlot20Result(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10162FB0u))(a0, a1); }
XI_ORIG undefined8 FUN_10162fe0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x10162FE0u))(a0, a1); }
XI_ORIG void Obj_10621944_Draw(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101630E0u))(a0); }
XI_ORIG void * Unit1012AC80_ZeroWordsAndDwords(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10163350u))(a0); }
XI_ORIG void Unit1012AC80_NoOpE() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10163380u))(); }
XI_ORIG int Obj_10621944_DrawBgRect(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10163390u))(a0); }
XI_ORIG void Unit1012AC80_ZeroDwords(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101633D0u))(a0); }
XI_ORIG void Gfx_DrawSolidRect2D(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x10163400u))(a0, a1); }
XI_ORIG char sub_10165070() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x10165070u))(); }
XI_ORIG signed int Unit10163E60_InitGlobalTables() { return reinterpret_cast<signed int (__cdecl *)()>(XI_GAME(0x101671D0u))(); }
XI_ORIG void * ZoneMap_ConstructVt10336614(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10167440u))(a0); }
XI_ORIG void Vt10336614_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101674B0u))(a0); }
XI_ORIG void * ZoneMap_ConstructVt10336634(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10167800u))(a0); }
XI_ORIG int Vt10336634_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101678D0u))(a0); }
XI_ORIG _DWORD * GetPtr8IfField60Set(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10167940u))(a0); }
XI_ORIG void ZoneMap_ReleaseBuffers(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10167950u))(a0); }
XI_ORIG void ZoneMap_UnlinkSlotCells(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101679A0u))(a0, a1); }
XI_ORIG int Dbg_DrawWireMesh(int a0, void * a1, int a2, float a3) { return reinterpret_cast<int (__stdcall *)(int, void *, int, float)>(XI_GAME(0x10168D20u))(a0, a1, a2, a3); }
XI_ORIG undefined4 ZoneMap_GetCount34154(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10168FD0u))(a0); }
XI_ORIG undefined4 Unit10163E60_ProbeAxis1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10169290u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG uint32_t Unit10163E60_SegmentHit3413A(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101693B0u))(a0, a1, a2, a3); }
XI_ORIG uint32_t Unit10163E60_SegmentHit34139(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10169B40u))(a0, a1, a2, a3); }
XI_ORIG uint32_t Unit10163E60_SegmentTest34139(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1016A2D0u))(a0, a1, a2, a3); }
XI_ORIG uint32_t FUN_1016a990(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1016A990u))(a0, a1, a2, a3); }
XI_ORIG undefined4 Unit10163E60_SegmentHitWithObjects(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1016B1D0u))(a0, a1, a2, a3); }
XI_ORIG float * Mat44_Multiply(void * a0, int a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, int, void *)>(XI_GAME(0x1016C060u))(a0, a1, a2); }
XI_ORIG undefined4 ZoneMap_GetHitCount(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1016C150u))(a0); }
XI_ORIG undefined4 ZoneMap_IsPointInTriangle(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x1016C390u))(a0, a1); }
XI_ORIG void FUN_1016c5c0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1016C5C0u))(a0); }
XI_ORIG undefined4 FUN_1016c5d0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1016C5D0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG void FUN_1016d580(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1016D580u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_1016dfd0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1016DFD0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined4 FUN_1016e7e0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1016E7E0u))(a0, a1, a2, a3); }
XI_ORIG undefined4 Unit10163E60_SweepStep(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1016F0E0u))(a0, a1, a2); }
XI_ORIG void FUN_1016f320(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1016F320u))(a0, a1, a2); }
XI_ORIG void FUN_1016fa20(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1016FA20u))(a0, a1, a2, a3); }
XI_ORIG void * ZoneMap_ConstructVt10336668(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1016FC40u))(a0); }
XI_ORIG int Vt10336668_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1016FC80u))(a0); }
XI_ORIG void FUN_1016fc90(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1016FC90u))(a0); }
XI_ORIG float * sub_10170B30(void * a0, void * a1, void * a2, void * a3) { return reinterpret_cast<float * (__stdcall *)(void *, void *, void *, void *)>(XI_GAME(0x10170B30u))(a0, a1, a2, a3); }
XI_ORIG _DWORD * sub_10170C80(int a0) { return reinterpret_cast<_DWORD * (__thiscall *)(int)>(XI_GAME(0x10170C80u))(a0); }
XI_ORIG void FUN_10170cd0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10170CD0u))(a0, a1); }
XI_ORIG int sub_10170E80(int a0, int a1, int a2, int a3) { return reinterpret_cast<int (__stdcall *)(int, int, int, int)>(XI_GAME(0x10170E80u))(a0, a1, a2, a3); }
XI_ORIG int * ZoneMap_FindEntryByName(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10170F30u))(a0, a1, a2); }
XI_ORIG void * ZoneMap_ConstructVt1033668C(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101718E0u))(a0); }
XI_ORIG int Vt1033668C_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10171920u))(a0); }
XI_ORIG undefined4 FUN_10173ae0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x10173AE0u))(a0, a1); }
XI_ORIG void ZoneMap_ResetDbgBoxList(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10174770u))(a0); }
XI_ORIG void FUN_10174a60(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10174A60u))(a0); }
XI_ORIG _DWORD * Mat4_SetIdentity_2(void * a0) { return reinterpret_cast<_DWORD * (__cdecl *)(void *)>(XI_GAME(0x10175740u))(a0); }
XI_ORIG float * Mat4_MulTranslation(void * a0, int a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, int, void *)>(XI_GAME(0x10175780u))(a0, a1, a2); }
XI_ORIG unsigned long long FUN_10176dd0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10176DD0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned int * CMoResource_VT_10336698_Destructor(void * a0) { return reinterpret_cast<unsigned int * (__thiscall *)(void *)>(XI_GAME(0x10176EC0u))(a0); }
XI_ORIG unsigned int * CMoResource_VT_103366C4_Destructor(void * a0) { return reinterpret_cast<unsigned int * (__thiscall *)(void *)>(XI_GAME(0x10176FA0u))(a0); }
XI_ORIG unsigned int * CMoResource_VT_103366F0_Destructor(void * a0) { return reinterpret_cast<unsigned int * (__thiscall *)(void *)>(XI_GAME(0x10177020u))(a0); }
XI_ORIG int Gfx_ShaderSrc_GetText(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10177340u))(a0); }
XI_ORIG unsigned int Gfx_ShaderSrc_GetTextLength(void * a0) { return reinterpret_cast<unsigned int (__thiscall *)(void *)>(XI_GAME(0x10177350u))(a0); }
XI_ORIG int * ZoneMap_GetMatrixGlobal() { return reinterpret_cast<int * (__cdecl *)()>(XI_GAME(0x101773F0u))(); }
XI_ORIG int Gfx_SetZBias(unsigned int a0) { return reinterpret_cast<int (__cdecl *)(unsigned int)>(XI_GAME(0x10177400u))(a0); }
XI_ORIG int ZoneMap_SetOverlayProjection(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10177420u))(a0); }
XI_ORIG undefined4 FUN_10177730() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10177730u))(); }
XI_ORIG unsigned long long ZoneMap_QueryBoxHeightGlobal(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) {
    uint32_t xs[4] = {a0.bits, a1.bits, a2.bits, a3.bits}; uint32_t ra = a4.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 12(%%esi)\n\t""pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $16, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10177770u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long CTsZoneMap_LogZoneMetrics(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10177800u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 ZoneMap_Call10173AE0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101779B0u))(a0, a1, a2); }
XI_ORIG void XiZone_DrawMarkerForward(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10177CD0u))(a0, a1, a2, a3); }
XI_ORIG int CTsZoneMap_Init(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10177D00u))(a0); }
XI_ORIG void CTsZoneMap_SetLayerOrder(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10177FA0u))(a0); }
XI_ORIG void CTsZoneMap_InitZBiasPresets(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101781D0u))(a0); }
XI_ORIG void FUN_10178250(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10178250u))(a0); }
XI_ORIG void ZoneMap_ResetGlobalTables() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x101784E0u))(); }
XI_ORIG void FUN_10178530(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10178530u))(a0, a1, a2, a3, a4); }
XI_ORIG int FUN_10178610(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10178610u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_10178790(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10178790u))(a0); }
XI_ORIG void ZoneMap_UpdateLoadState(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101787C0u))(a0); }
XI_ORIG void ZoneMap_ReleaseSlot(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10178AC0u))(a0, a1); }
XI_ORIG int ZoneMap_ReleaseStateBlock(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10178B00u))(a0); }
XI_ORIG undefined4 * FUN_10178d90(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10178D90u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10178dd0(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x10178DD0u))(a0); }
XI_ORIG void ZoneMap_Reset(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10178F70u))(a0); }
XI_ORIG void ZoneMap_CreateVertexShaders(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101792A0u))(a0); }
XI_ORIG void FUN_1017a630(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1017A630u))(a0); }
XI_ORIG void FUN_1017a690(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x1017A690u))(a0); }
XI_ORIG undefined4 ZoneMap_FreeSlot(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1017B110u))(a0, a1); }
XI_ORIG int ZoneMap_TeardownStep(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x1017B1E0u))(a0); }
XI_ORIG undefined4 ZoneMap_TeardownStep_v1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1017B1E0u))(a0); }
XI_ORIG void XiZone_Draw(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1017B510u))(a0); }
XI_ORIG undefined4 ZoneMap_QueryBoxHeightBandThenGetCount34154(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101811D0u))(a0, a1, a2, a3); }
XI_ORIG undefined4 ZoneMap_QueryBoxHeight(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10181210u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined4 ZoneMap_QueryBoxHeightBand(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10181290u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined4 ZoneMap_QueryBoxHeightByRadius(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10181300u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_10181510(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10181510u))(a0, a1, a2); }
XI_ORIG undefined4 ZoneMap_Query1693B0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10181590u))(a0, a1, a2, a3); }
XI_ORIG undefined4 ZoneMap_IsBoxHeightHitByRadius(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101819B0u))(a0, a1, a2, a3, a4); }
XI_ORIG BOOL sub_10181BB0(void * a0, void * a1) { return reinterpret_cast<BOOL (__stdcall *)(void *, void *)>(XI_GAME(0x10181BB0u))(a0, a1); }
XI_ORIG float * CTsZoneMap_DrawProjectedTexture(int a0, void * a1, int a2, void * a3, float a4) { return reinterpret_cast<float * (__thiscall *)(int, void *, int, void *, float)>(XI_GAME(0x10182700u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_10183370(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x10183370u))(a0, a1); }
XI_ORIG int ZoneMap_FindSlotByName(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10183420u))(a0, a1); }
XI_ORIG float * XiZone_DrawModelAtMatrix(void * a0, void * a1, int a2) { return reinterpret_cast<float * (__thiscall *)(void *, void *, int)>(XI_GAME(0x10183490u))(a0, a1, a2); }
XI_ORIG _DWORD * ZoneMap_DrawMeshById(int a0, int a1, int a2) { return reinterpret_cast<_DWORD * (__thiscall *)(int, int, int)>(XI_GAME(0x10183DA0u))(a0, a1, a2); }
XI_ORIG undefined4 XiZone_DrawGroundShadowQuad(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10184C90u))(a0, a1, a2, a3); }
XI_ORIG undefined4 CTsZoneMap_DrawProjectedShadow(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10184FE0u))(a0, a1, a2); }
XI_ORIG undefined4 XiZone_DrawDecalOnTriangles(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10185CA0u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_10186d30(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10186D30u))(a0, a1, a2); }
XI_ORIG void ZoneMap_ClearSlots(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10186DB0u))(a0); }
XI_ORIG void ZoneMap_FreeSubAreas(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x10186DD0u))(a0); }
XI_ORIG void FUN_10186e70() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10186E70u))(); }
XI_ORIG char * ZoneMap_GetAddr0B4(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10186E90u))(a0); }
XI_ORIG int ZoneMap_GetAddr0C4(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186EA0u))(a0); }
XI_ORIG char * ZoneMap_GetAddr0D4(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10186EB0u))(a0); }
XI_ORIG int ZoneMap_GetAddr024(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186EC0u))(a0); }
XI_ORIG int ZoneMap_GetAddr044(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186EE0u))(a0); }
XI_ORIG int ZoneMap_GetAddr064(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186F00u))(a0); }
XI_ORIG int ZoneMap_GetAddr074(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186F10u))(a0); }
XI_ORIG char * Env_GetColorPtr84(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10186F20u))(a0); }
XI_ORIG int Env_GetColorPtr84_v1(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186F20u))(a0); }
XI_ORIG int ZoneMap_GetAddr094(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186F30u))(a0); }
XI_ORIG int ZoneMap_GetAddrRef18Plus1C(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186F40u))(a0); }
XI_ORIG int ZoneMap_GetAddrRef18Plus20(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186F50u))(a0); }
XI_ORIG long double FUN_10186f60(void * a0) { return reinterpret_cast<long double (__thiscall *)(void *)>(XI_GAME(0x10186F60u))(a0); }
XI_ORIG int ZoneMap_GetAddr0E4(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186F70u))(a0); }
XI_ORIG int ZoneMap_GetAddr0F4(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186F80u))(a0); }
XI_ORIG int ZoneMap_GetAddr114(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186F90u))(a0); }
XI_ORIG char * Env_GetAmbientColorPtr(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10186FA0u))(a0); }
XI_ORIG int Env_GetAmbientColorPtr_v1(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186FA0u))(a0); }
XI_ORIG int ZoneMap_GetAddrRef18Plus3C(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186FB0u))(a0); }
XI_ORIG int ZoneMap_GetAddrRef18Plus40(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186FC0u))(a0); }
XI_ORIG long double ZoneMap_GetFloatRef18Plus44(void * a0) { return reinterpret_cast<long double (__thiscall *)(void *)>(XI_GAME(0x10186FD0u))(a0); }
XI_ORIG double ZoneMap_GetFloatRef1CPlus4(void * a0) { return reinterpret_cast<double (__thiscall *)(void *)>(XI_GAME(0x10186FE0u))(a0); }
XI_ORIG long double ZoneMap_GetFloatRef1CPlus4_v1(void * a0) { return reinterpret_cast<long double (__thiscall *)(void *)>(XI_GAME(0x10186FE0u))(a0); }
XI_ORIG long double ZoneMap_GetFloatRef1CPlus8(void * a0) { return reinterpret_cast<long double (__thiscall *)(void *)>(XI_GAME(0x10186FF0u))(a0); }
XI_ORIG int ZoneMap_GetByteRef1CPlus10(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10187000u))(a0); }
XI_ORIG long double ZoneMap_GetFloatRef1CPlus0C(void * a0) { return reinterpret_cast<long double (__thiscall *)(void *)>(XI_GAME(0x10187020u))(a0); }
XI_ORIG void FUN_10187030(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10187030u))(a0); }
XI_ORIG void ZoneMap_ReleaseChain(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10187030u))(a0); }
XI_ORIG void FUN_10187050(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10187050u))(a0); }
XI_ORIG void * ZoneMap_SetVtableApplyRecord(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10187380u))(a0); }
XI_ORIG void ZoneMap_Destroy(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10187470u))(a0); }
XI_ORIG uint32_t FUN_101874a0(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x101874A0u))(a0); }
XI_ORIG uint32_t ZoneMap_Update(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x101874A0u))(a0); }
XI_ORIG unsigned long long ZoneMap_ForEachFindKind5B(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101875C0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long ZoneMap_ForEachFindKind5A(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101875E0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void ZoneMap_SaveState(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10187600u))(a0); }
XI_ORIG void ZoneMap_UpdateOrbit(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10187720u))(a0, a1); }
XI_ORIG void ZoneMap_SetRecord(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101880C0u))(a0, a1); }
XI_ORIG void ZoneMap_ApplyColors(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10188190u))(a0, a1); }
XI_ORIG int ZoneMap_GetCurrent() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10188900u))(); }
XI_ORIG void ZoneMap_AppendToList(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10188910u))(a0); }
XI_ORIG void FUN_10188940() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x10188940u))(); }
XI_ORIG int ZoneMap_GetTail() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101889A0u))(); }
XI_ORIG int FUN_101889c0(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x101889C0u))(a0); }
XI_ORIG void FUN_101889e0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101889E0u))(a0); }
XI_ORIG unsigned long long XiZone_UpdateTree(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101889E0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_10188a10(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10188A10u))(a0); }
XI_ORIG undefined4 XiZone_DestroyChildList(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10188A10u))(a0); }
XI_ORIG uint32_t FUN_10188a50(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x10188A50u))(a0); }
XI_ORIG _DWORD * XiZone_GetDirLights(int a0, void * a1, void * a2) { return reinterpret_cast<_DWORD * (__thiscall *)(int, void *, void *)>(XI_GAME(0x10188B70u))(a0, a1, a2); }
XI_ORIG void XiZone_GetDirLights_v1(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10188B70u))(a0, a1, a2); }
XI_ORIG int ZoneMap_FillDirLight(int a0, void * a1, int a2) { return reinterpret_cast<int (__thiscall *)(int, void *, int)>(XI_GAME(0x10188CA0u))(a0, a1, a2); }
XI_ORIG void ZoneMap_FillDirLight_v1(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10188CA0u))(a0, a1, a2); }
XI_ORIG _DWORD * Env_ComputeAmbient(void * a0, void * a1, int a2) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, int)>(XI_GAME(0x10188E00u))(a0, a1, a2); }
XI_ORIG void Env_ComputeAmbient_v1(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10188E00u))(a0, a1, a2); }
XI_ORIG void FUN_10189120(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10189120u))(a0, a1, a2, a3, a4); }
XI_ORIG void sub_10189120(int a0, void * a1, void * a2, void * a3, int a4) { reinterpret_cast<void (__thiscall *)(int, void *, void *, void *, int)>(XI_GAME(0x10189120u))(a0, a1, a2, a3, a4); }
XI_ORIG long double FUN_101892b0(void * a0, XI_ANY a1) { return reinterpret_cast<long double (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101892B0u))(a0, a1); }
XI_ORIG void Env_BlendKeys(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10189490u))(a0, a1, a2, a3); }
XI_ORIG void FUN_10189490(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10189490u))(a0, a1, a2, a3); }
XI_ORIG void FUN_10189750(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10189750u))(a0, a1, a2, a3); }
XI_ORIG undefined4 Env_LerpFloat(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10189810u))(a0, a1, a2, a3); }
XI_ORIG void Env_LerpKeyValues(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101898E0u))(a0, a1, a2, a3); }
XI_ORIG void FUN_10189980(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10189980u))(a0, a1, a2, a3); }
XI_ORIG undefined4 WeatherRes_IsTimeKeyName(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x10189BD0u))(a0); }
XI_ORIG void FUN_10189c60(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x10189C60u))(a0); }
XI_ORIG void Env_FindPrevKey(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10189CA0u))(a0, a1, a2); }
XI_ORIG void * FUN_10189e30(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10189E30u))(a0, a1); }
XI_ORIG void Env_FindNextKey(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10189F20u))(a0, a1, a2); }
XI_ORIG void * FUN_1018a0f0(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018A0F0u))(a0, a1); }
XI_ORIG void Env_UpdatePrevKey(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018A220u))(a0); }
XI_ORIG void Env_UpdateNextKey(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018A260u))(a0); }
XI_ORIG void Env_UpdateKeyAtOrBefore(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018A2A0u))(a0); }
XI_ORIG undefined4 Env_UpdateKeyNeighbor(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1018A2C0u))(a0); }
XI_ORIG void Env_SetKeyField2C(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018A2E0u))(a0, a1); }
XI_ORIG void Env_SetKeyField30(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018A2F0u))(a0, a1); }
XI_ORIG void Env_SetKeyField24(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018A300u))(a0, a1); }
XI_ORIG undefined4 Env_SetKeyField28(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018A310u))(a0, a1); }
XI_ORIG void Env_UpdateTimeKeys(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018A320u))(a0); }
XI_ORIG void FUN_1018a340(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018A340u))(a0); }
XI_ORIG unsigned long long YmCombineWeather_FindKind5A(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1018AAC0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long YmCombineWeather_FindKind5B(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1018AB40u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_1018abb0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1018ABB0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_1018abf0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018ABF0u))(a0); }
XI_ORIG void YmCombineWeather_Release(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018ABF0u))(a0); }
XI_ORIG undefined4 Unit1018AD50_GetField4(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1018AD50u))(a0); }
XI_ORIG void Unit1018AD50_SetField10Of4(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018AD60u))(a0, a1); }
XI_ORIG void * Blend_FindPrevious(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1018AD80u))(a0); }
XI_ORIG void Blend_FreeResult(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018ADA0u))(a0); }
XI_ORIG void FUN_1018ada0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018ADA0u))(a0); }
XI_ORIG void Unit1018AD50_CallBlendUpdate(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018ADD0u))(a0); }
XI_ORIG void Blend_Update(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018ADE0u))(a0); }
XI_ORIG void FUN_1018ade0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018ADE0u))(a0); }
XI_ORIG undefined4 Blend_AllocResult(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x1018AEB0u))(a0, a1); }
XI_ORIG undefined4 FUN_1018aeb0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x1018AEB0u))(a0, a1); }
XI_ORIG void FUN_1018b130() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1018B130u))(); }
XI_ORIG _DWORD * sub_1018B130() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x1018B130u))(); }
XI_ORIG void * FUN_1018b170(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1018B170u))(a0); }
XI_ORIG void FUN_1018b620(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018B620u))(a0); }
XI_ORIG void XiZone_ResetState(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018B620u))(a0); }
XI_ORIG int sub_1018B860(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1018B860u))(a0); }
XI_ORIG void XiZone_Call10036170(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x1018B8B0u))(a0); }
XI_ORIG _DWORD * XiZone_GetDirLightsForArea(void * a0, void * a1, void * a2, int a3) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, void *, int)>(XI_GAME(0x1018B960u))(a0, a1, a2, a3); }
XI_ORIG int XiZone_GetDirLight(void * a0, void * a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, void *, int, int)>(XI_GAME(0x1018B9A0u))(a0, a1, a2, a3); }
XI_ORIG int XiZone_GetAmbient(void * a0, void * a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, void *, int, int)>(XI_GAME(0x1018B9E0u))(a0, a1, a2, a3); }
XI_ORIG void XiZone_GetAmbient_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1018B9E0u))(a0, a1, a2, a3); }
XI_ORIG void XiZone_GetFog(void * a0, void * a1, void * a2, void * a3, int a4, int a5) { reinterpret_cast<void (__thiscall *)(void *, void *, void *, void *, int, int)>(XI_GAME(0x1018BA20u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void XiZone_GetFog_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1018BA20u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void XiZone_SetPtr1D8FromTable(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018BAE0u))(a0, a1); }
XI_ORIG undefined4 unknown_libname_158(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1018BBB0u))(a0); }
XI_ORIG void * Time_SetNormalized(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018BBC0u))(a0, a1); }
XI_ORIG unsigned long long FUN_1018bc40(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1018BC40u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG int Time_GetDiv5184000(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1018BD30u))(a0); }
XI_ORIG void Time_SetModulo216000Div3600(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018BF00u))(a0, a1); }
XI_ORIG void FUN_1018bf50(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018BF50u))(a0, a1); }
XI_ORIG void Time_SetModulo155520000Div5184000(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018BFA0u))(a0, a1); }
XI_ORIG void FUN_1018c180(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1018C180u))(a0); }
XI_ORIG undefined4 FUN_1018c290(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1018C290u))(a0); }
XI_ORIG undefined4 Time_SubtractFrameScale(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1018C290u))(a0); }
XI_ORIG unsigned int sub_1018C2B0() { return reinterpret_cast<unsigned int (__cdecl *)()>(XI_GAME(0x1018C2B0u))(); }
XI_ORIG void CXiSchStatus_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018C620u))(a0); }
XI_ORIG int Sch_GetStatusEntry(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x1018CA20u))(a0); }
XI_ORIG void FUN_1018cea0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018CEA0u))(a0); }
XI_ORIG uint32_t Unit1018BA60_GrowCapacity(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1018D230u))(a0, a1); }
XI_ORIG undefined4 Unit1018BA60_Free(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1018D250u))(a0); }
XI_ORIG void Unit1018BA60_InitEmpty(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018D280u))(a0); }
XI_ORIG void * Unit1018BA60_Init(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1018D2B0u))(a0); }
XI_ORIG void * Unit1018BA60_AssignCStr(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018D2D0u))(a0, a1); }
XI_ORIG void FUN_1018d3d0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018D3D0u))(a0); }
XI_ORIG void Unit1018BA60_AssignWhole(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018D430u))(a0, a1); }
XI_ORIG void Unit1018BA60_Append(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018D440u))(a0, a1); }
XI_ORIG void Unit1018BA60_AppendBuf(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018D450u))(a0, a1); }
XI_ORIG undefined1 * Unit1018BA60_GetCStr(void * a0) { return reinterpret_cast<undefined1 * (__thiscall *)(void *)>(XI_GAME(0x1018D470u))(a0); }
XI_ORIG undefined4 Unit1018BA60_GetLength(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1018D480u))(a0); }
XI_ORIG undefined4 Unit1018BA60_GetField4_1018D490(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1018D490u))(a0); }
XI_ORIG void * Unit1018BA60_AssignN(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1018D4A0u))(a0, a1, a2); }
XI_ORIG void * Unit1018BA60_AppendN(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1018D580u))(a0, a1, a2); }
XI_ORIG void * Unit1018BA60_InitAcqDat(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1018D640u))(a0); }
XI_ORIG int Unit1018BA60_FillCompletionWords(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1018D7F0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 Unit1018BA60_CommitPending8C(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1018D980u))(a0); }
XI_ORIG undefined4 Unit1018BA60_AppendCompletionWord(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1018DB10u))(a0, a1, a2, a3); }
XI_ORIG int FUN_1018dc70(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018DC70u))(a0, a1); }
XI_ORIG int Unit1018BA60_GetEntryMod(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018DC70u))(a0, a1); }
XI_ORIG void Unit1018BA60_ClearEntries(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018DCB0u))(a0); }
XI_ORIG void * Unit1018BA60_InitPecDat(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1018DCD0u))(a0); }
XI_ORIG undefined4 FsLogPage_InitPage(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018E0A0u))(a0, a1); }
XI_ORIG void FsLogPage_ClearAndVisitFiles(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018E0E0u))(a0); }
XI_ORIG void FsLogPage_FormatFileName(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1018E170u))(a0, a1, a2); }
XI_ORIG undefined4 FsLogPage_AllocBuffer(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1018E1A0u))(a0); }
XI_ORIG undefined4 FsLogPage_load(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018E1D0u))(a0, a1); }
XI_ORIG undefined4 FsLogPage_WriteFile(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018E310u))(a0, a1); }
XI_ORIG undefined4 FsLogPage_AddString(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018E4D0u))(a0, a1); }
XI_ORIG void FsLogPage_ClearBuffer(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018E660u))(a0); }
XI_ORIG void FsLogPage_PruneOffsets(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018E6A0u))(a0); }
XI_ORIG void * FsLogPage_Init(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018E6F0u))(a0, a1); }
XI_ORIG undefined4 FsLogPage_HasPage(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018E7F0u))(a0, a1); }
XI_ORIG int FsLogPage_EnsurePage(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018E800u))(a0, a1); }
XI_ORIG undefined4 FsLogPage_Rotate(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018E8D0u))(a0, a1); }
XI_ORIG void * Unit1018BA60_ConstructKind1(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1018E9B0u))(a0); }
XI_ORIG void * Unit1018BA60_ConstructKind2(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1018EA00u))(a0); }
XI_ORIG void Unit1018BA60_InitFields(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018EA50u))(a0, a1); }
XI_ORIG undefined4 Unit1018BA60_RecreateMembers(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1018EB00u))(a0); }
XI_ORIG undefined4 Text_ClassifyChar(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1018FA20u))(a0, a1); }
XI_ORIG undefined4 Text_GetCharLength(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1018FA80u))(a0, a1); }
XI_ORIG int Text_CountChars(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1018FB00u))(a0, a1, a2); }
XI_ORIG unsigned long long Sjis_Remap82To83(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[2] = {a0.bits, a1.bits}; uint32_t ra = a2.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $8, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1018FD90u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG int FUN_1018fdf0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1018FDF0u))(a0, a1, a2); }
XI_ORIG int Text_EncodeQuotes(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10190140u))(a0, a1, a2); }
XI_ORIG void Unit1018BA60_AllocZeroed(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101907A0u))(a0, a1); }
XI_ORIG undefined8 Unit1018BA60_FreeBlock(XI_ANY a0) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY)>(XI_GAME(0x101907D0u))(a0); }
XI_ORIG void Unit1018BA60_Nop() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x101908D0u))(); }
XI_ORIG void Unit1018BA60_Update10336950(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10190BB0u))(a0); }
XI_ORIG undefined4 Unit1018BA60_AddPairChecked(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10193800u))(a0, a1, a2); }
XI_ORIG undefined4 SetConfig(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10193830u))(a0, a1); }
XI_ORIG int Settings_GetValue(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10193850u))(a0); }
XI_ORIG undefined8 Settings_GetValue_v1(XI_ANY a0) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY)>(XI_GAME(0x10193850u))(a0); }
XI_ORIG undefined4 Settings_GetValue_v2(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10193850u))(a0); }
XI_ORIG undefined4 Unit1018BA60_GetSetting99(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10193890u))(a0); }
XI_ORIG undefined4 Unit1018BA60_GetSettingMapped(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101938B0u))(a0, a1); }
XI_ORIG undefined4 FUN_101938d0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101938D0u))(a0, a1); }
XI_ORIG undefined8 Unit1018BA60_TestBit64(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10193A10u))(a0, a1); }
XI_ORIG undefined4 Unit1018BA60_InitPairNode(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10193B00u))(a0, a1, a2); }
XI_ORIG void Unit1018BA60_FindOrAdd(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10193B60u))(a0, a1, a2); }
XI_ORIG void Unit1018BA60_ForEach(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10193BC0u))(a0, a1, a2); }
XI_ORIG undefined4 Unit1018BA60_ResetConfigReturn1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10195220u))(a0); }
XI_ORIG undefined4 Unit1018BA60_AddPairAndCall(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10195240u))(a0, a1, a2, a3); }
XI_ORIG undefined4 Unit1018BA60_SetEntryValue(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101952C0u))(a0, a1, a2); }
XI_ORIG undefined4 Unit1018BA60_GetEntryValue(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10195330u))(a0, a1); }
XI_ORIG undefined4 Unit1018BA60_ReloadConfig(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10195350u))(a0); }
XI_ORIG void Unit1018BA60_ReadTigDatSetFlag(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101953A0u))(a0); }
XI_ORIG undefined4 Unit1018BA60_FlushThenTrue(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101953C0u))(a0); }
XI_ORIG undefined4 FUN_101953d0() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x101953D0u))(); }
XI_ORIG undefined4 Unit1018BA60_UpdateIfFlag9(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101953F0u))(a0); }
XI_ORIG void Unit1018BA60_DefineEntry(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10195460u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG undefined8 Unit1018BA60_GetEntry2C(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101954D0u))(a0, a1); }
XI_ORIG undefined4 Unit1018BA60_ReadTigDat(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10195570u))(a0); }
XI_ORIG undefined4 Unit1018BA60_StoreExternalConfig(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10195640u))(a0); }
XI_ORIG undefined4 Unit1018BA60_LoadExternalConfig(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101956F0u))(a0); }
XI_ORIG undefined4 Unit1018BA60_ReadExternalKind1(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x10195780u))(a0); }
XI_ORIG undefined4 Unit1018BA60_WriteExternalKind1(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x101957B0u))(a0, a1); }
XI_ORIG undefined * Unit1018BA60_ReadExternalKind2(XI_ANY a0) { return reinterpret_cast<undefined * (__stdcall *)(XI_ANY)>(XI_GAME(0x10195810u))(a0); }
XI_ORIG int Unit1018BA60_IsExternalKind2Key(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x101958C0u))(a0, a1); }
XI_ORIG undefined4 Unit1018BA60_SaveCnfDat(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10195900u))(a0); }
XI_ORIG undefined4 Unit1018BA60_LoadCnfDat(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101959A0u))(a0); }
XI_ORIG undefined4 Unit1018BA60_SetConfigEntryValue(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x10196040u))(a0, a1); }
XI_ORIG void Unit1018BA60_ResetConfigToDefaults(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10196260u))(a0, a1); }
XI_ORIG void Unit1018BA60_ConstructVt10336A0C(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101962D0u))(a0); }
XI_ORIG void * Unit1018BA60_WindowItemCtor(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10196980u))(a0); }
XI_ORIG undefined4 Unit1018BA60_ReturnZero() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10196BE0u))(); }
XI_ORIG void * Unit1018BA60_FrameItemCtor(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10197A90u))(a0); }
XI_ORIG void * Unit1018BA60_WindowItemCtorVt10336D20(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10199710u))(a0); }
XI_ORIG int Unit1018BA60_RunPairA() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1019C3E0u))(); }
XI_ORIG void Unit1018BA60_RunPairB() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1019C4F0u))(); }
XI_ORIG undefined4 Unit1018BA60_Call10311386() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1019C580u))(); }
XI_ORIG undefined4 Unit1018BA60_GetSetting41() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1019C590u))(); }
XI_ORIG int Unit1018BA60_Call103113C8() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1019C5B0u))(); }
XI_ORIG void * Unit1018BA60_ByteTableCtor(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1019CD40u))(a0); }
XI_ORIG unsigned long long FUN_1019cdc0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1019CDC0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Config_LoadDefaultTable(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1019CE70u))(a0); }
XI_ORIG unsigned long long Unit1018BA60_FillTableBytes(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1019CF60u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_1019cfa0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1019CFA0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Config_LoadAltTable(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1019D1C0u))(a0); }
XI_ORIG unsigned long long FUN_1019d2b0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1019D2B0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_1019d320(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1019D320u))(a0); }
XI_ORIG unsigned long long FUN_1019d350(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1019D350u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Settings_CacheValues(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1019D8B0u))(a0); }
XI_ORIG undefined4 FUN_1019ead0() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x1019EAD0u))(); }
XI_ORIG int sub_1019EBB0(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x1019EBB0u))(a0); }
XI_ORIG int Unit1018BA60_PushWithRetry(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1019F8B0u))(a0, a1); }
XI_ORIG undefined4 Unit1018BA60_FindMatchAt8(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1019F930u))(a0, a1); }
XI_ORIG undefined8 Unit1018BA60_WaitThenHandleFileEvent(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1019F960u))(a0, a1); }
XI_ORIG int Unit1018BA60_WaitThenHandleFileEvent_v1(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1019F960u))(a0, a1); }
XI_ORIG int Unit1018BA60_WaitEvent18At8(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1019FA90u))(a0); }
XI_ORIG int Unit1018BA60_SignalMember8(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1019FAB0u))(a0); }
XI_ORIG int FsFileThreadManager_handleFileEvent(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1019FD50u))(a0, a1); }
XI_ORIG int Unit1018BA60_SignalEvent10(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101A0910u))(a0); }
XI_ORIG int Unit1018BA60_ResetAndWaitEvent14(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A0930u))(a0, a1); }
XI_ORIG undefined8 Unit1018BA60_WaitEvent18(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x101A0950u))(a0); }
XI_ORIG undefined8 Unit1018BA60_SignalMember18(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x101A0970u))(a0); }
XI_ORIG undefined4 Unit1018BA60_Push(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A0BE0u))(a0, a1); }
XI_ORIG undefined4 Unit1018BA60_FindMatch(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A0D30u))(a0, a1); }
XI_ORIG void * Unit1018BA60_StringListCtor(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101A1400u))(a0); }
XI_ORIG void Unit1018BA60_SetVtable10337210(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101A1460u))(a0); }
XI_ORIG undefined4 Unit1018BA60_PushFrontString(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A1470u))(a0, a1, a2); }
XI_ORIG int Unit1018BA60_CopyCurrent(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A1500u))(a0, a1, a2); }
XI_ORIG undefined4 Unit1018BA60_SetCurrent(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A1580u))(a0, a1, a2); }
XI_ORIG undefined4 Unit1018BA60_Advance(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101A1660u))(a0); }
XI_ORIG undefined4 Unit1018BA60_Retreat(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101A1690u))(a0); }
XI_ORIG undefined4 Unit1018BA60_ClearFirstString(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101A16B0u))(a0); }
XI_ORIG void * Unit1018BA60_McrEntryCtor(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101A16E0u))(a0); }
XI_ORIG void Unit1018BA60_CopyMcrEntry(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A1740u))(a0, a1); }
XI_ORIG int Unit1018BA60_GetMcrEntryLine(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A1910u))(a0, a1); }
XI_ORIG int Unit1018BA60_Create() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101A19B0u))(); }
XI_ORIG void * Unit1018BA60_McrManagerCtor(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101A1A20u))(a0); }
XI_ORIG undefined4 Unit1018BA60_QueueEntry(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A1C60u))(a0, a1); }
XI_ORIG void FUN_101a1cb0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101A1CB0u))(a0); }
XI_ORIG undefined4 Unit1018BA60_StepScript(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101A1CC0u))(a0); }
XI_ORIG undefined FUN_101a1e20() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x101A1E20u))(); }
XI_ORIG undefined FsMacroContainer_loadCallback() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x101A1FE0u))(); }
XI_ORIG undefined FUN_101a23d0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x101A23D0u))(); }
XI_ORIG undefined4 Unit1018BA60_ParseWaitCommand(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101A2480u))(a0, a1, a2); }
XI_ORIG void Unit1018BA60_FormatNumberedDatName(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101A2610u))(a0, a1, a2); }
XI_ORIG void FUN_101a2660(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A2660u))(a0, a1, a2); }
XI_ORIG void Unit1018BA60_LoadMacroDat(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A26C0u))(a0, a1, a2); }
XI_ORIG undefined4 Unit1018BA60_SetReadPos(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A2890u))(a0, a1); }
XI_ORIG undefined4 Unit1018BA60_CtorVt10337314(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A2E00u))(a0, a1); }
XI_ORIG void Unit1018BA60_SetFields1C20(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A2E60u))(a0, a1, a2); }
XI_ORIG char Unit1018BA60_UpdateMenuState(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x101A2E80u))(a0); }
XI_ORIG undefined4 Unit1018BA60_SetSlot(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A3300u))(a0, a1, a2); }
XI_ORIG unsigned long long Unit1018BA60_CanOpenMenuShortcut(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101A33E0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Unit1018BA60_ApplyKeyAction(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101A3500u))(a0, a1, a2, a3); }
XI_ORIG unsigned long long Unit1018BA60_HandleReadPosKeys(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101A3550u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void * Unit1018BA60_PanelCtorVt10337320(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A36F0u))(a0, a1, a2); }
XI_ORIG void Unit1018BA60_SetFlag29A(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101A3CF0u))(a0); }
XI_ORIG undefined4 FUN_101a3d00(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A3D00u))(a0, a1); }
XI_ORIG void * Unit1018BA60_PanelCtorVt10337378(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A3DC0u))(a0, a1, a2); }
XI_ORIG void Unit1018BA60_SetFlag29B(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101A43A0u))(a0); }
XI_ORIG undefined4 FUN_101a43b0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A43B0u))(a0, a1); }
XI_ORIG undefined4 Unit1018BA60_CtorVt103373BC(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A4440u))(a0, a1); }
XI_ORIG void * Unit1018BA60_PanelCtorVt103373C8(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A46C0u))(a0, a1, a2); }
XI_ORIG void * Unit1018BA60_PanelCtorVt10337418(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A4D70u))(a0, a1, a2); }
XI_ORIG void * Unit1018BA60_CtorVt10337460(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A54F0u))(a0, a1, a2); }
XI_ORIG void * Unit1018BA60_CtorVt103374A8(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101A6430u))(a0); }
XI_ORIG void * Unit1018BA60_CtorVt103374F0(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A6C10u))(a0, a1); }
XI_ORIG void * Unit1018BA60_CtorVt10337538(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A7280u))(a0, a1); }
XI_ORIG void * Unit1018BA60_CtorVt1033757C(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101A83C0u))(a0); }
XI_ORIG void Unit1018BA60_BuildSlotLinks(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A8410u))(a0, a1); }
XI_ORIG undefined4 Unit1018BA60_CtorVt10337580(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A8750u))(a0, a1); }
XI_ORIG _BYTE * Unit1018BA60_CreateAll() { return reinterpret_cast<_BYTE * (__cdecl *)()>(XI_GAME(0x101A8B50u))(); }
XI_ORIG _DWORD * Unit1018BA60_CreateDatManager() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x101A8D40u))(); }
XI_ORIG int sub_101A8D60() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101A8D60u))(); }
XI_ORIG Iostream_init * Unit1018BA60_CreateEmptyObject() { return reinterpret_cast<Iostream_init * (__cdecl *)()>(XI_GAME(0x101AB080u))(); }
XI_ORIG char * Unit1018BA60_CreateMenuSystem() { return reinterpret_cast<char * (__cdecl *)()>(XI_GAME(0x101AB0D0u))(); }
XI_ORIG void Unit1018BA60_ReloadAll() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x101AB730u))(); }
XI_ORIG char Unit1018BA60_HandleJumpMapKeys() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x101AB770u))(); }
XI_ORIG unsigned long long Unit1018BA60_LoadAllAndTimestamp(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101AB7D0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void * Unit1018BA60_ReturnThis(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101AB810u))(a0); }
XI_ORIG void Unit1018BA60_HandleMenuKeys(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101AB820u))(a0); }
XI_ORIG void FsTextInput_Init(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101ABB40u))(a0, a1); }
XI_ORIG undefined4 FsTextInput_SetText(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101ABC50u))(a0, a1, a2); }
XI_ORIG unsigned long long FsTextInput_GetFlag(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101ABE40u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FsTextInput_ClearState(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101ABE50u))(a0); }
XI_ORIG undefined4 FsTextInput_GetCode(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101ABE80u))(a0, a1); }
XI_ORIG void FsTextInput_RebuildAtCursor(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101ABF40u))(a0, a1); }
XI_ORIG int FsTextInput_GetActiveBuffer(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101AC1B0u))(a0); }
XI_ORIG undefined4 FsTextInput_GetLength(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101AC200u))(a0); }
XI_ORIG int FsTextInput_GetMaxLength(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101AC230u))(a0); }
XI_ORIG undefined4 FsTextInput_ResetBuffer(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101AC580u))(a0); }
XI_ORIG uint32_t FUN_101ac5e0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101AC5E0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG uint32_t FsTextInput_ExpandMarkedPhrase(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101AC5E0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG int FsTextInput_BuildDisplayText(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101AC990u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int FsTextInput_BuildDisplayTextTrimmed(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101ACAC0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG uint32_t FsTextInput_CommitEntry(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101ACC30u))(a0, a1); }
XI_ORIG void FsTextInput_SetCode4(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101ACF40u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FsTextInput_MapCodeByte(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101ACF60u))(a0); }
XI_ORIG void FsTextInput_CopyCode4(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101ACF70u))(a0, a1); }
XI_ORIG undefined4 FsTextInput_ClearPair(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101ACF90u))(a0); }
XI_ORIG undefined4 FsTextInput_InsertAtCursor(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101ACFB0u))(a0); }
XI_ORIG undefined4 FsTextInput_IsMarkerAt(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x101AD1B0u))(a0, a1); }
XI_ORIG void Unit1018BA60_ProcessInputLine(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101AD4B0u))(a0, a1, a2, a3); }
XI_ORIG int FsTextInput_GetDataPtr8(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101AD820u))(a0); }
XI_ORIG undefined4 Unit1018BA60_IsSlashChar(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x101ADBC0u))(a0); }
XI_ORIG undefined4 Unit1018BA60_Call101B72E0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101ADBD0u))(a0, a1); }
XI_ORIG undefined4 Unit1018BA60_IsKnownCommand(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x101ADBE0u))(a0); }
XI_ORIG undefined4 Shape_GetValueAt(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101ADDC0u))(a0, a1); }
XI_ORIG void Unit1018BA60_ShortcutCtor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101ADE00u))(a0); }
XI_ORIG void FsTextInput_SetName(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101ADE80u))(a0, a1, a2, a3, a4); }
XI_ORIG void Unit1018BA60_ShortcutSet(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101ADEC0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined4 Unit1018BA60_ShortcutCheck(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101ADF20u))(a0, a1); }
XI_ORIG undefined4 FsShortcut_CanUseShortcut(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101AE010u))(a0); }
XI_ORIG int Unit1018BA60_UpdateShortcuts(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101AE3E0u))(a0); }
XI_ORIG void Unit1018BA60_NoOpC() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x101AFF30u))(); }
XI_ORIG HIMC ImmGetContext(XI_ANY a0) { return reinterpret_cast<HIMC (__stdcall *)(XI_ANY)>(XI_GAME(0x101AFF50u))(a0); }
XI_ORIG BOOL ImmReleaseContext(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<BOOL (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x101AFF60u))(a0, a1); }
XI_ORIG void Unit1018BA60_OnChar(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101B36F0u))(a0, a1); }
XI_ORIG void Unit1018BA60_DtorVt103379BC(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101B45B0u))(a0); }
XI_ORIG void Unit1018BA60_CtorVt103379C0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101B45E0u))(a0); }
XI_ORIG void * Unit1018BA60_CtorVt103379C4(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101B4710u))(a0); }
XI_ORIG undefined4 Unit1018BA60_AppendByte(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101B4C20u))(a0, a1); }
XI_ORIG void Unit1018BA60_ClearAll(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101B4F90u))(a0); }
XI_ORIG undefined4 Unit1018BA60_IsModeFourOrFive(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101B5C90u))(a0); }
XI_ORIG undefined4 Unit1018BA60_IsModeThree(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101B5CC0u))(a0); }
XI_ORIG int FsTextInput_OnChar(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101B6810u))(a0, a1); }
XI_ORIG undefined4 Unit1018BA60_InputSubmit(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101B6C00u))(a0, a1, a2); }
XI_ORIG unsigned long long Unit1018BA60_AcceptsChar(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[2] = {a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101B6D50u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Unit1018BA60_GetFlag8C81(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101B6E90u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void * Unit1018BA60_GetEntry(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101B6EC0u))(a0); }
XI_ORIG undefined4 Unit1018BA60_SetWordForCompletion(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101B6EE0u))(a0, a1, a2); }
XI_ORIG void Unit1018BA60_ResetBuffer(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101B7190u))(a0); }
XI_ORIG void Unit1018BA60_InputCall101AC990(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101B7260u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG undefined4 Unit1018BA60_InputCall101AC990Swapped(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101B7280u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG undefined4 Unit1018BA60_InputCall101ACAC0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101B72B0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_101b72e0(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x101B72E0u))(a0); }
XI_ORIG uint32_t Unit1018BA60_IsState1Or3() { return reinterpret_cast<uint32_t (__cdecl *)()>(XI_GAME(0x101B7EE0u))(); }
XI_ORIG int sub_101B8E70() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101B8E70u))(); }
XI_ORIG undefined8 Unit1018BA60_ReleaseSemaphoreAt4(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x101B9530u))(a0); }
XI_ORIG undefined8 Event_Wait(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101B9550u))(a0, a1); }
XI_ORIG int Event_ResetAndWait(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101B95D0u))(a0, a1); }
XI_ORIG int Event_Signal(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101B9610u))(a0); }
XI_ORIG undefined4 Unit1018BA60_CtorVt10337B1C(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101B9650u))(a0); }
XI_ORIG int FsTinyString_init(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101B9690u))(a0); }
XI_ORIG void Unit1018BA60_DatEntryCtor(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101B9880u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG void Unit1018BA60_InitWithCallback(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101B9910u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void Unit1018BA60_CopyFrom(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101B9980u))(a0, a1); }
XI_ORIG void Unit1018BA60_ClearVtable(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101B9A00u))(a0); }
XI_ORIG undefined4 Unit1018BA60_LoadFromFile(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101B9A10u))(a0); }
XI_ORIG undefined4 Unit1018BA60_Read(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101B9BC0u))(a0, a1); }
XI_ORIG void * Unit1018BA60_InitCopy(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101B9E00u))(a0, a1); }
XI_ORIG int Unit1018BA60_CreateGlobal10662DB4() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101B9E60u))(); }
XI_ORIG undefined4 Unit1018BA60_AppendToList(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101B9EC0u))(a0, a1); }
XI_ORIG void Unit1018BA60_Register(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101B9F20u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG undefined4 Unit1018BA60_RemoveById(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101B9F90u))(a0, a1); }
XI_ORIG undefined4 Unit1018BA60_LoadAll(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101B9FF0u))(a0); }
XI_ORIG undefined4 Unit1018BA60_ReadAll(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BA030u))(a0); }
XI_ORIG undefined4 Unit1018BA60_Construct(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BA180u))(a0); }
XI_ORIG void * Unit1018BA60_CtorVt10337B2C(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101BA280u))(a0); }
XI_ORIG void Unit1018BA60_ClearBufferA(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101BA2D0u))(a0); }
XI_ORIG void Unit1018BA60_ClearBufferB(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101BA300u))(a0); }
XI_ORIG void * Unit1018BA60_CtorVt10337B4C(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101BA570u))(a0); }
XI_ORIG void Unit1018BA60_Destruct(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101BA5B0u))(a0); }
XI_ORIG undefined4 Unit1018BA60_ResetCounters(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BA5C0u))(a0); }
XI_ORIG undefined4 FUN_101badd0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BADD0u))(a0); }
XI_ORIG undefined4 FsWordCompletion_GetCount18(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BAE20u))(a0); }
XI_ORIG undefined4 FsWordCompletion_GetCount1C(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BAE30u))(a0); }
XI_ORIG int FsWordCompletion_FindNextByKey(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101BAE40u))(a0, a1); }
XI_ORIG undefined4 FsWordCompletion_GetField20(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BAEB0u))(a0); }
XI_ORIG undefined4 FsWordCompletion_GetField28(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BAEC0u))(a0); }
XI_ORIG void FsWordCompletion_ClearEntries(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101BAF90u))(a0); }
XI_ORIG void FsWordCompletion_Init(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101BAFC0u))(a0, a1); }
XI_ORIG int FUN_101bb070(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101BB070u))(a0, a1); }
XI_ORIG uint32_t FsWordCompletion_IsEntryValid(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x101BB100u))(a0); }
XI_ORIG undefined4 FUN_101bb2b0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BB2B0u))(a0); }
XI_ORIG void FsWordCompletion_BuildItems(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101BB4B0u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_101bb680(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BB680u))(a0); }
XI_ORIG uint32_t FsWordCompletion_Refresh(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x101BB740u))(a0); }
XI_ORIG undefined4 FsWordCompletion_LoadData(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BB7A0u))(a0); }
XI_ORIG undefined1 FsWordCompletion_Release(void * a0) { return reinterpret_cast<undefined1 (__thiscall *)(void *)>(XI_GAME(0x101BB9E0u))(a0); }
XI_ORIG undefined4 FsWordCompletion_SetWord(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101BBA90u))(a0, a1, a2, a3); }
XI_ORIG void FsWordCompletion_Update(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101BBC60u))(a0); }
XI_ORIG undefined4 Unit1018BA60_IsState204Not1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BBE50u))(a0); }
XI_ORIG undefined4 FsWordCompletion_getEntryList(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101BBE60u))(a0, a1, a2); }
XI_ORIG unsigned long long FsWordCompletion_lookUp(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) {
    uint32_t xs[3] = {a1.bits, a2.bits, a3.bits}; uint32_t ra = a4.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101BC100u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FsWordCompletion__prepareAllData(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101BC520u))(a0, a1); }
XI_ORIG undefined4 Unit1018BA60_Store101BB070At8(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BC6F0u))(a0); }
XI_ORIG undefined4 Unit1018BA60_FreeBuffer(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BC720u))(a0); }
XI_ORIG void FUN_101bc740(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101BC740u))(a0); }
XI_ORIG uint32_t Unit1018BA60_Set(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101BC7B0u))(a0, a1, a2, a3); }
XI_ORIG int Entry_CompareKey(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101BC900u))(a0, a1); }
XI_ORIG undefined4 Unit1018BA60_GetUsed(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BC9B0u))(a0); }
XI_ORIG void * FsMemory_Constructor(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101BC9E0u))(a0, a1); }
XI_ORIG void FUN_101bca30(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101BCA30u))(a0); }
XI_ORIG undefined4 * Unit1018BA60_AppendPadded(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101BCA40u))(a0, a1, a2); }
XI_ORIG int Unit1018BA60_ReserveZeroed(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101BCAC0u))(a0, a1); }
XI_ORIG void Unit1018BA60_Clear(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101BCB30u))(a0); }
XI_ORIG undefined4 Unit1018BA60_CtorVt10337C3C(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BCBA0u))(a0); }
XI_ORIG char Unit1018BA60_WatchConfigWindow(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x101BCE30u))(a0); }
XI_ORIG void * Unit1018BA60_CtorVt10337C5C(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101BCEF0u))(a0); }
XI_ORIG void Unit1018BA60_ClearTwoBuffers(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101BCF30u))(a0); }
XI_ORIG void * Unit1018BA60_CtorVt10337C60(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101BE260u))(a0); }
XI_ORIG void Unit1018BA60_SetVtable10337C60(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101BE2A0u))(a0); }
XI_ORIG void Unit1018BA60_SetField4(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101BE2B0u))(a0, a1); }
XI_ORIG void Unit1018BA60_SetDateAt9(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101BE2C0u))(a0, a1); }
XI_ORIG void Unit1018BA60_SetDateAt11(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101BE330u))(a0, a1); }
XI_ORIG void Unit1018BA60_SetDateAt19(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101BE3A0u))(a0, a1); }
XI_ORIG void Unit1018BA60_SetVtable10337C64(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101BE410u))(a0); }
XI_ORIG undefined4 File_FindByKind(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101BE470u))(a0, a1, a2, a3); }
XI_ORIG undefined4 Unit1018BA60_Call10311F30(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x101BE500u))(a0); }
XI_ORIG undefined4 FUN_101be510(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x101BE510u))(a0); }
XI_ORIG int File_WriteChunked(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__stdcall *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101BE590u))(a0, a1, a2); }
XI_ORIG int FUN_101be670(XI_ANY a0) { return reinterpret_cast<int (__stdcall *)(XI_ANY)>(XI_GAME(0x101BE670u))(a0); }
XI_ORIG undefined4 FUN_101be690(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x101BE690u))(a0); }
XI_ORIG undefined4 Unit1018BA60_FillSizeAndThreeDates(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x101BE6C0u))(a0, a1); }
XI_ORIG undefined4 File_CreateParentDirs(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x101BE730u))(a0); }
XI_ORIG undefined4 FUN_101beb90() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x101BEB90u))(); }
XI_ORIG int XiFileImpl_QueueLoad(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101BECC0u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int Unit1018BA60_SubmitFileNumKind1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101BF280u))(a0, a1, a2, a3, a4); }
XI_ORIG int Unit1018BA60_SubmitPathKind1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101BF370u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int Unit1018BA60_SubmitPath2F4Kind1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101BF460u))(a0, a1, a2, a3, a4); }
XI_ORIG int Unit1018BA60_SubmitPath3F4Kind1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101BF550u))(a0, a1, a2, a3, a4); }
XI_ORIG int Unit1018BA60_QueueFileNumKind2(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101BF970u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int Unit1018BA60_QueuePathKind3(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C0050u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int Unit1018BA60_QueuePath2F4Kind3(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C0160u))(a0, a1, a2, a3, a4); }
XI_ORIG int Unit1018BA60_QueuePath3F4Kind3(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C0260u))(a0, a1, a2, a3, a4); }
XI_ORIG int Unit1018BA60_SubmitPathKind3(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C0440u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int Unit1018BA60_SubmitPath2F4Kind3(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C0540u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined8 Unit1018BA60_SubmitPath3F4Kind3(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C0630u))(a0, a1, a2, a3, a4); }
XI_ORIG int Unit1018BA60_SubmitPath4F4Kind3(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C0760u))(a0, a1, a2, a3, a4); }
XI_ORIG int Unit1018BA60_CreateDirAndWait(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101C09B0u))(a0); }
XI_ORIG int Unit1018BA60_CreateDirAndHandleFile(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C0A40u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_101c0bc0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101C0BC0u))(a0, a1); }
XI_ORIG int Unit1018BA60_SubmitPathKind6(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C0FC0u))(a0, a1, a2); }
XI_ORIG int Unit1018BA60_SubmitPath3F4Kind6(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C1090u))(a0, a1); }
XI_ORIG uint32_t Unit1018BA60_FindFileNumKind1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C11F0u))(a0, a1, a2, a3); }
XI_ORIG uint32_t Unit1018BA60_FindFileNumKind2(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C13A0u))(a0, a1); }
XI_ORIG uint32_t Unit1018BA60_FindPathKind3(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C1750u))(a0, a1, a2); }
XI_ORIG uint32_t Unit1018BA60_FindPath2F4Kind3(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C1820u))(a0, a1); }
XI_ORIG uint32_t Unit1018BA60_FindPath3F4Kind3(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C18E0u))(a0, a1); }
XI_ORIG undefined4 FUN_101c1a40() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x101C1A40u))(); }
XI_ORIG void FUN_101c1b10() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x101C1B10u))(); }
XI_ORIG int Unit1018BA60_SubmitFileNumKind4(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C1B20u))(a0, a1); }
XI_ORIG int Unit1018BA60_SubmitPath3F4Kind4(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C1C90u))(a0, a1); }
XI_ORIG int Unit1018BA60_SubmitPathKind4(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C1DF0u))(a0, a1, a2); }
XI_ORIG int Unit1018BA60_SubmitPath2F4Kind4(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C1EC0u))(a0, a1); }
XI_ORIG int Unit1018BA60_SubmitPath3F4Kind5(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C2450u))(a0, a1, a2); }
XI_ORIG void FUN_101c2670() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x101C2670u))(); }
XI_ORIG undefined4 Unit1018BA60_SubmitPath3F4Kind7(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C2840u))(a0, a1); }
XI_ORIG undefined4 FUN_101c2b60(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C2B60u))(a0, a1); }
XI_ORIG undefined4 FUN_101c2c80(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101C2C80u))(a0); }
XI_ORIG void Unit1018BA60_WriteTimestampFwd(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C2D80u))(a0, a1); }
XI_ORIG undefined4 FUN_101c2d90() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x101C2D90u))(); }
XI_ORIG int FUN_101c2e70(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101C2E70u))(a0); }
XI_ORIG void Unit1018BA60_ReleaseAt1F0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101C33E0u))(a0); }
XI_ORIG undefined4 Unit1018BA60_GetValueAt1F0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101C33F0u))(a0); }
XI_ORIG undefined4 XiFileImpl_getFileName(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C3A90u))(a0, a1, a2); }
XI_ORIG undefined4 Unit1018BA60_FormatUserPath(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C3D00u))(a0, a1, a2, a3); }
XI_ORIG undefined4 Unit1018BA60_FormatStrAt2F4(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C3DB0u))(a0, a1, a2); }
XI_ORIG undefined8 Unit1018BA60_FormatStrAt3F4(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C3DE0u))(a0, a1, a2); }
XI_ORIG undefined4 Unit1018BA60_FormatStrAt4F4(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C3E10u))(a0, a1, a2); }
XI_ORIG undefined4 Unit1018BA60_FormatWithBase(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C3E40u))(a0, a1, a2, a3); }
XI_ORIG undefined4 Unit1018BA60_FormatPathByKind(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C3E90u))(a0, a1, a2, a3); }
XI_ORIG undefined4 Unit1018BA60_CopyPathByMode(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C3F40u))(a0, a1, a2, a3); }
XI_ORIG undefined4 Unit1018BA60_MapCode012(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x101C3FB0u))(a0); }
XI_ORIG unsigned long long Unit1018BA60_CreateDirectories(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101C3FE0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG int FUN_101c4080(XI_ANY a0) { return reinterpret_cast<int (__stdcall *)(XI_ANY)>(XI_GAME(0x101C4080u))(a0); }
XI_ORIG void Unit1018BA60_WriteTimestamp(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C4170u))(a0, a1); }
XI_ORIG void Unit1018BA60_FileMngCall101BEB90(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101C46A0u))(a0); }
XI_ORIG undefined4 FUN_101c46d0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C46D0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 Unit1018BA60_FileMngQueueLoad(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C46E0u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int Unit1018BA60_ForwardOrExit(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C4740u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 Unit1018BA60_FileMngSubmitPathKind1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C4790u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined4 Unit1018BA60_FileMngSubmitPath2F4Kind1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C47A0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 Unit1018BA60_FileMngSubmitPath3F4Kind1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C47B0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 Unit1018BA60_FileMngQueueFileNumKind2(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C4820u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG undefined4 Unit1018BA60_FileMngQueuePathKind3(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C48E0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void Unit1018BA60_FileMngQueuePath2F4Kind3(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C48F0u))(a0, a1, a2, a3, a4); }
XI_ORIG void Unit1018BA60_FileMngQueuePath3F4Kind3(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C4900u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 Unit1018BA60_FileMngSubmitPathKind3(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C4920u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void Unit1018BA60_FileMngSubmitPathKind3_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C4920u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void Unit1018BA60_FileMngSubmitPath2F4Kind3(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C4930u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 Unit1018BA60_FileMngSubmitPath4F4Kind3(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C4950u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 Unit1018BA60_FileMngCall101C0A40(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C49A0u))(a0, a1, a2, a3, a4); }
XI_ORIG void Unit1018BA60_FileMngSubmitPathKind6(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C49E0u))(a0, a1, a2); }
XI_ORIG void Unit1018BA60_FileMngSubmitPath3F4Kind6(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C49F0u))(a0, a1); }
XI_ORIG undefined4 Unit1018BA60_FileMngFindFileNumKind1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C4A10u))(a0, a1, a2, a3); }
XI_ORIG undefined4 Unit1018BA60_FileMngFindFileNumKind2(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C4A30u))(a0, a1); }
XI_ORIG undefined4 Unit1018BA60_FileMngFwd101C1750(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C4A80u))(a0, a1, a2); }
XI_ORIG undefined4 Unit1018BA60_FileMngFwd101C1820(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C4A90u))(a0, a1); }
XI_ORIG undefined4 Unit1018BA60_FileMngFwd101C18E0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C4AA0u))(a0, a1); }
XI_ORIG undefined4 Unit1018BA60_FileMngCall101C1A40(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101C4AC0u))(a0); }
XI_ORIG undefined4 FUN_101c4ad0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C4AD0u))(a0, a1); }
XI_ORIG int FileMng_ResolveOrExit(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x101C4AF0u))(a0, a1); }
XI_ORIG int FileMng_ResolveOrExit_v1(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C4AF0u))(a0, a1); }
XI_ORIG undefined4 Unit1018BA60_FileMngFwd101C1C90(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C4B40u))(a0, a1); }
XI_ORIG undefined4 Unit1018BA60_FileMngFwd101C1DF0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C4B60u))(a0, a1, a2); }
XI_ORIG undefined4 Unit1018BA60_FileMngFwd101C1EC0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C4B70u))(a0, a1); }
XI_ORIG undefined4 FUN_101c4bc0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C4BC0u))(a0, a1, a2); }
XI_ORIG undefined4 Unit1018BA60_FileMngFwd101C2450(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C4BF0u))(a0, a1, a2); }
XI_ORIG void Unit1018BA60_FileMngCall101C2670(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101C4C20u))(a0); }
XI_ORIG undefined4 Unit1018BA60_FileMngFwd101C2840(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C4C70u))(a0, a1); }
XI_ORIG char Unit1018BA60_FileMngFwd101C2B60(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(XI_GAME(0x101C4CC0u))(a0, a1); }
XI_ORIG uint8_t Unit1018BA60_FileMngFwd101C2C80(void * a0) { return reinterpret_cast<uint8_t (__thiscall *)(void *)>(XI_GAME(0x101C4CD0u))(a0); }
XI_ORIG int Unit1018BA60_FileMngFwd101C4170(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x101C4CE0u))(a0, a1); }
XI_ORIG undefined4 Unit1018BA60_FileMngCall101C2D90(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101C4CF0u))(a0); }
XI_ORIG int Unit1018BA60_FileMngFwd101C2E70(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101C4D10u))(a0); }
XI_ORIG void Unit1018BA60_FileMngReleaseAt1F0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101C4D30u))(a0); }
XI_ORIG void Unit1018BA60_FileMngGetValueAt1F0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101C4D40u))(a0); }
XI_ORIG void * Unit1018BA60_EntriesCtorEmpty(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101C5920u))(a0); }
XI_ORIG void * Unit1018BA60_EntriesCtor(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C5930u))(a0, a1); }
XI_ORIG void Unit1018BA60_AllocEntries(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C59B0u))(a0, a1); }
XI_ORIG void Unit1018BA60_EntriesSetCount(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C5A30u))(a0, a1); }
XI_ORIG uint8_t Unit1018BA60_SubmitFileOrExit(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<uint8_t (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C5A40u))(a0, a1, a2, a3); }
XI_ORIG void * Unit1018BA60_CtorVt10337C90(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C5BE0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG undefined4 Unit1018BA60_CommitTextInput(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101C5EC0u))(a0); }
XI_ORIG void Unit1018BA60_EndTextInput(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101C60C0u))(a0); }
XI_ORIG void FUN_101c6960(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C6960u))(a0, a1, a2); }
XI_ORIG void FUN_101c7c50(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101C7C50u))(a0); }
XI_ORIG void Unit101C72E0_Construct(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101C7F80u))(a0); }
XI_ORIG void * Unit101C72E0_Clear(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101C8070u))(a0); }
XI_ORIG void Unit101C72E0_FreeBuffer(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101C80A0u))(a0); }
XI_ORIG undefined4 Unit101C72E0_Load(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C80C0u))(a0, a1); }
XI_ORIG void * Unit101C72E0_Init(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101C8260u))(a0); }
XI_ORIG void Unit101C72E0_ResizeEntries(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C8290u))(a0, a1); }
XI_ORIG undefined4 Unit101C72E0_LoadTable(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C8380u))(a0, a1); }
XI_ORIG undefined4 Unit101C72E0_MapChar(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C87A0u))(a0, a1, a2); }
XI_ORIG int Unit101C72E0_CompareMapped(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C88A0u))(a0, a1, a2, a3); }
XI_ORIG void YkCommonInit() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x101C8950u))(); }
XI_ORIG undefined4 Unit101C72E0_GetIndexed(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101C8C30u))(a0); }
XI_ORIG uint32_t Unit101C72E0_Checksum(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101C8C50u))(a0, a1); }
XI_ORIG undefined4 Unit101C72E0_TimeToTmWrap28Years(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101C8D10u))(a0); }
XI_ORIG int Unit101C72E0_GetOffset() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101C8D70u))(); }
XI_ORIG void Gfx_ProjectWorldToScreen(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C8DA0u))(a0, a1, a2); }
XI_ORIG int Unit101C72E0_MeasureText(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x101C9550u))(a0); }
XI_ORIG undefined8 Unit101C72E0_DrawUnscaled(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C9780u))(a0, a1, a2, a3); }
XI_ORIG undefined8 Unit101C72E0_DrawTextItem0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C97B0u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG undefined8 FUN_101c9b50(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C9B50u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG undefined8 Unit101C72E0_DrawString(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C9D50u))(a0, a1, a2, a3, a4); }
XI_ORIG int Unit101C72E0_DrawTruncated(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CA180u))(a0, a1, a2, a3); }
XI_ORIG void Unit101C72E0_DrawFractionRange(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CAD00u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9); }
XI_ORIG void Unit101C72E0_DrawFractionDefault(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CAEC0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined8 Unit101C72E0_DrawSmoothed(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CAF00u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9); }
XI_ORIG undefined8 Unit101C72E0_DrawSmoothedDefault(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CB000u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void Unit101C72E0_DrawFractionItem1A(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CB040u))(a0, a1, a2, a3); }
XI_ORIG undefined8 Unit101C72E0_DrawSmoothedItem1A(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CB090u))(a0, a1, a2, a3); }
XI_ORIG void FUN_101cb0d0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CB0D0u))(a0, a1, a2, a3); }
XI_ORIG undefined4 Unit101C72E0_DrawSmoothedExItem1B(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CB230u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG void FUN_101cb280(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9, XI_ANY a10) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CB280u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }
XI_ORIG void Unit101C72E0_DrawEasedPercent(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9, XI_ANY a10, XI_ANY a11) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CB430u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11); }
XI_ORIG void Unit101C72E0_DrawEasedPercentUnitScale(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CB500u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG void Unit101C72E0_DrawEasedPercentItem1A(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CB550u))(a0, a1, a2, a3); }
XI_ORIG undefined4 Unit101C72E0_BeginCapture(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CB770u))(a0, a1, a2); }
XI_ORIG unsigned long long Unit101C72E0_IsActorField170(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101CBDF0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Unit101C72E0_IsActorField170Is6(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101CBE70u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Unit101C72E0_TestFlags100Or40DAC(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101CC070u))(a0); }
XI_ORIG unsigned long long FUN_101cc0d0(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[2] = {a0.bits, a1.bits}; uint32_t ra = a2.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $8, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101CC0D0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Unit101C72E0_ActorTest1D0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101CC1D0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long Unit101C72E0_ActorPairTest1FF(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101CC220u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Unit101C72E0_CC4F0ActorTest400(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101CC350u))(a0, a1); }
XI_ORIG undefined8 FUN_101cc4f0() { return reinterpret_cast<undefined8 (__cdecl *)()>(XI_GAME(0x101CC4F0u))(); }
XI_ORIG undefined4 Unit101C72E0_GetCC4F0ActorByIndexAt1FA() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x101CC510u))(); }
XI_ORIG undefined4 Acter_GetByIndexAt1FA(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101CC520u))(a0); }
XI_ORIG undefined4 Unit101C72E0_GetCC4F0ActorAt2A0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x101CC550u))(); }
XI_ORIG undefined4 Unit101C72E0_GetActorAt2A0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101CC560u))(a0); }
XI_ORIG undefined4 FUN_101cc620(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101CC620u))(a0, a1); }
XI_ORIG int Yk_MapCodeOffset(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x101CCE00u))(a0); }
XI_ORIG void YkFurniture_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101CD900u))(a0); }
XI_ORIG undefined4 Unit101CD990_GetValueAt54(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101CD9D0u))(a0); }
XI_ORIG undefined4 Unit101CD990_GetValueAt56(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101CDA00u))(a0); }
XI_ORIG undefined4 Unit101CD990_GetValueAt5A(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101CDA30u))(a0); }
XI_ORIG void Unit101CF660_BuildTables(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101CF660u))(a0, a1); }
XI_ORIG void Unit101CF660_BuildCodeTable(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CF820u))(a0, a1, a2, a3); }
XI_ORIG void YkJpeg_InitTables(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101CF870u))(a0); }
XI_ORIG void YkJpeg_CutColor(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101CFA00u))(a0, a1); }
XI_ORIG void YkJpeg_CopyBlock(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CFDB0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int YkJpeg_Unpack565(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CFE20u))(a0, a1, a2, a3); }
XI_ORIG void YkJpeg_UnpackBlock565(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CFEB0u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG void YkJpeg_ForwardDct(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x101CFF60u))(a0); }
XI_ORIG void FUN_101d01b0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101D01B0u))(a0, a1, a2); }
XI_ORIG void YkJpeg_EncodeBlock(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101D0210u))(a0, a1, a2); }
XI_ORIG undefined4 YkJpeg_Encode(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101D0370u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined * YkJpeg_GetCopyrightString() { return reinterpret_cast<undefined * (__cdecl *)()>(XI_GAME(0x101D05E0u))(); }
XI_ORIG int YkJpeg_EncodeChecked(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101D0600u))(a0, a1, a2, a3, a4); }
XI_ORIG void Unit101CF660_Reset(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101D0860u))(a0); }
XI_ORIG void Unit101CF660_Free(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101D0880u))(a0); }
XI_ORIG undefined4 YkJpeg_Destructor(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101D08C0u))(a0); }
XI_ORIG void YkJpeg_Release(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101D08D0u))(a0); }
XI_ORIG undefined4 YkJpgBit_Init(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101D08E0u))(a0, a1, a2); }
XI_ORIG void YkJpeg_PutBit(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101D0910u))(a0, a1); }
XI_ORIG void YkJpeg_PutBits(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101D0970u))(a0, a1, a2); }
XI_ORIG void Unit101D0910_PutByte(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101D09C0u))(a0, a1); }
XI_ORIG void YkJpeg_PutBytes(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101D09E0u))(a0, a1, a2); }
XI_ORIG void Unit101D0910_PutWordBE(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101D0A20u))(a0, a1); }
XI_ORIG void Unit101D0910_PutFFThenByte(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101D0A40u))(a0, a1); }
XI_ORIG void YkJpeg_FlushBits(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101D0A60u))(a0); }
XI_ORIG char sub_101D0B00() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x101D0B00u))(); }
XI_ORIG undefined * CopyrightText_Format() { return reinterpret_cast<undefined * (__cdecl *)()>(XI_GAME(0x101D0C40u))(); }
XI_ORIG char sub_101D0C70() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x101D0C70u))(); }
XI_ORIG undefined4 Unit101D0910_SaveJpeg512x382(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101D1000u))(a0); }
XI_ORIG undefined4 YkJpeg_CaptureAndEncode(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101D1060u))(a0, a1, a2); }
XI_ORIG undefined4 Unit101D0910_WriteWithFileMngA(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101D10C0u))(a0, a1); }
XI_ORIG undefined4 Unit101D0910_SaveJpegScreenSize(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101D1100u))(a0); }
XI_ORIG undefined4 Unit101D0910_WriteWithFileMngB(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101D1180u))(a0, a1); }
XI_ORIG _WORD * YkInit() { return reinterpret_cast<_WORD * (__cdecl *)()>(XI_GAME(0x101D11C0u))(); }
XI_ORIG undefined4 FUN_101d2260(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101D2260u))(a0, a1); }
XI_ORIG undefined4 Unit101D0910_TestMask10663188(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101D29C0u))(a0, a1); }
XI_ORIG void YkMyroom_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101D2F00u))(a0); }
XI_ORIG void YkMyroom_ReleaseObject38(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101D30B0u))(a0); }
XI_ORIG undefined4 Unit101D2FF0_GetObjectAtIndex(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101D30F0u))(a0); }
XI_ORIG void YkMyroom_ReleaseResource8(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101D31B0u))(a0); }
XI_ORIG void Unit101D2FF0_ReleaseTable(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101D3DC0u))(a0); }
XI_ORIG void Unit101D2FF0_Draw(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101D3FC0u))(a0, a1); }
XI_ORIG undefined4 FUN_101d4a20(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101D4A20u))(a0); }
XI_ORIG unsigned long long FUN_101d5370(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101D5370u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Unit101D2FF0_Update(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101D5680u))(a0); }
XI_ORIG undefined4 FUN_101d5fe0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x101D5FE0u))(); }
XI_ORIG void * Unit101D7020_CtorVt10338670(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101D7C10u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt10338700(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101D87A0u))(a0); }
XI_ORIG void * Yk_CreateCommonObjects(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101D9B10u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt10338BE8(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101D9E90u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt10338D88(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101DC460u))(a0); }
XI_ORIG void * Yk_CreateSecondaryObjects(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101DCE20u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt10338E60(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101DD000u))(a0); }
XI_ORIG void Yk_InitSlots(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101DDA70u))(a0); }
XI_ORIG void FUN_101dda90(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101DDA90u))(a0, a1); }
XI_ORIG undefined4 Unit101D7020_CtorVt10338EA4(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101DDF90u))(a0); }
XI_ORIG undefined4 Unit101D7020_CtorVt10338ED4(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101DE6B0u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt10338F68(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101DEDC0u))(a0); }
XI_ORIG void * YkWndList_Init(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101DF210u))(a0); }
XI_ORIG void FUN_101e06c0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101E06C0u))(a0); }
XI_ORIG void FUN_101e0700(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101E0700u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt10339250(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101E0A90u))(a0); }
XI_ORIG void Unit101D7020_ClearField74(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101E15F0u))(a0); }
XI_ORIG undefined4 Yk_ClearEntries(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101E1A00u))(a0); }
XI_ORIG void FUN_101e2880(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x101E2880u))(a0, a1); }
XI_ORIG void * Unit101D7020_CtorVt10339298(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101E2B10u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt103394C0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101E3F50u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt10339530(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101E48F0u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt10339698(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101E4A20u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt10339728(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101E51B0u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt10339770(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101E5750u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt103397B8(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101E5BA0u))(a0); }
XI_ORIG void * FUN_101e6fb0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101E6FB0u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt10339D78(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101EB390u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt10339DE8(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101EB7E0u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt10339E48(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101EBE00u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt10339EB8(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101EC0D0u))(a0); }
XI_ORIG void FUN_101ec3c0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101EC3C0u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt10339FA0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101ED450u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt1033A000(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101EE230u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void Yk_ResetFields(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101EFCA0u))(a0); }
XI_ORIG void Unit101D7020_ClearSixFlags(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101EFEA0u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt1033A5D8(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101F17C0u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt1033A6D0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101F43F0u))(a0); }
XI_ORIG void FUN_101f4af0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101F4AF0u))(a0); }
XI_ORIG unsigned long long YkWndList_SetLine(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101F6700u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void YkWndList_SetLine_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101F6700u))(a0, a1); }
XI_ORIG void * Yk_InitBase(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101F67C0u))(a0, a1); }
XI_ORIG unsigned long long Unit101D7020_GetValueIfLoaded(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101F71A0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG uint32_t Yk_GetSharePercent(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101F71C0u))(a0, a1); }
XI_ORIG undefined4 Unit101D7020_GetEntryFieldA(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101F7200u))(a0); }
XI_ORIG undefined4 Unit101D7020_GetEntryFieldB(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101F7240u))(a0); }
XI_ORIG uint32_t Unit101D7020_GetEntryFieldBPercent(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x101F7260u))(a0); }
XI_ORIG int Yk_FindEntryIndex(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101F7430u))(a0, a1, a2); }
XI_ORIG int Unit101D7020_GetEntryCode(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x101F7480u))(a0); }
XI_ORIG uint32_t Unit101D7020_GetEntryPosition(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101F74F0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void FUN_101f7570(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101F7570u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined * Unit101D7020_FormatTargetMapGrid(XI_ANY a0) { return reinterpret_cast<undefined * (__cdecl *)(XI_ANY)>(XI_GAME(0x101F7600u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt1033A8B8(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101F86C0u))(a0); }
XI_ORIG void Yk_ResetSlots(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101F92B0u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt1033A918(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101FB710u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt1033A988(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101FC2C0u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt1033A9D0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101FCBC0u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt1033AAB0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101FD000u))(a0); }
XI_ORIG undefined4 Unit101D7020_CtorVt1033AAF8(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101FE430u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt1033ABE0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101FF000u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt1033AC28(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10200B60u))(a0); }
XI_ORIG void * Unit101D7020_InitFromTablesA(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10201020u))(a0); }
XI_ORIG void * Unit101D7020_InitFromTablesB(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10201130u))(a0); }
XI_ORIG undefined8 FUN_10201180(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10201180u))(a0, a1, a2); }
XI_ORIG void FUN_10201340(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10201340u))(a0, a1, a2); }
XI_ORIG void Unit101D7020_CreateMenuObjects() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x102017A0u))(); }
XI_ORIG void * Unit101D7020_CtorVt1033B0C8(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10201C90u))(a0); }
XI_ORIG undefined4 Yk_CheckFlagAndWord1C(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10201EB0u))(a0); }
XI_ORIG unsigned long long Unit101D7020_IsSingleUncheckedEntry(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10201F00u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined * Yk_GetActiveEntry(XI_ANY a0) { return reinterpret_cast<undefined * (__stdcall *)(XI_ANY)>(XI_GAME(0x10202C90u))(a0); }
XI_ORIG void Unit101D7020_SetModeFwd(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10202D30u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void Yk_SetMode(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10202D80u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG undefined4 FUN_10202f00() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10202F00u))(); }
XI_ORIG undefined1 FUN_10202fe0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined1 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10202FE0u))(a0, a1); }
XI_ORIG unsigned long long Unit101D7020_FindActiveEntryById(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[3] = {a0.bits, a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x102031F0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Yk_GetActerIfValid(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x102033C0u))(a0); }
XI_ORIG undefined4 Yk_GetActerPtrA0(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x102034A0u))(a0); }
XI_ORIG undefined4 Unit101D7020_GetCurrentEntry(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x102034E0u))(a0); }
XI_ORIG undefined4 Yk_GetEntryOrZero(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x10203500u))(a0, a1); }
XI_ORIG void Yk_SetCurrentIndex(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x10203520u))(a0); }
XI_ORIG int Yk_FindIndexById(XI_ANY a0) { return reinterpret_cast<int (__stdcall *)(XI_ANY)>(XI_GAME(0x10203540u))(a0); }
XI_ORIG unsigned long long Unit101D7020_UpdateEntryCycle(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10203920u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_10203be0(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x10203BE0u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt1033B110(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10205500u))(a0); }
XI_ORIG void FUN_10206420(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10206420u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt1033B248(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10207730u))(a0); }
XI_ORIG undefined4 Unit101D7020_CtorVt1033B28C(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10208500u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt1033B340(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10209300u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt1033B3A0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10209B90u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt1033B948(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1020B500u))(a0); }
XI_ORIG undefined4 Unit101D7020_RecordInit(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1020BA40u))(a0, a1); }
XI_ORIG void Yk_LoadIndexedTables(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1020BE90u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt1033B9D8(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1020C470u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt1033BA20(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1020CC30u))(a0); }
XI_ORIG void * Unit101D7020_CtorVt1033BA68(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1020ED00u))(a0); }
XI_ORIG void * YkWndShop_Constructor(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10210310u))(a0); }
XI_ORIG void Unit101D7020_ResetState(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10210D60u))(a0); }
XI_ORIG void * Unit101D7020_Init(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10210DA0u))(a0, a1, a2); }
XI_ORIG void Unit101D7020_Close(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10210FC0u))(a0, a1); }
XI_ORIG undefined4 Unit101D7020_HandleChoice(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x102112B0u))(a0, a1); }
XI_ORIG undefined4 Unit101D7020_ApplyMusicVolume() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10211340u))(); }
XI_ORIG void FUN_102174f0() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x102174F0u))(); }
XI_ORIG void * Unit10211440_CtorVt1033BD58(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x102175C0u))(a0); }
XI_ORIG void FUN_1021b330(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1021B330u))(a0); }
XI_ORIG _WORD * Unit10211440_Create() { return reinterpret_cast<_WORD * (__cdecl *)()>(XI_GAME(0x1021BB90u))(); }
XI_ORIG void Unit10211440_SetVtable1033BDA0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1021BC20u))(a0); }
XI_ORIG undefined4 * Unit10211440_CopyRecordCtor(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1021BC30u))(a0, a1); }
XI_ORIG undefined4 Unit10211440_ReturnTrue() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1021BCB0u))(); }
XI_ORIG uint8_t FUN_1021bcc0(void * a0, XI_ANY a1) { return reinterpret_cast<uint8_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1021BCC0u))(a0, a1); }
XI_ORIG uint8_t Unit10211440_SaveToDat(void * a0, XI_ANY a1) { return reinterpret_cast<uint8_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1021BDB0u))(a0, a1); }
XI_ORIG undefined4 FUN_1021be50(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1021BE50u))(a0, a1, a2); }
XI_ORIG undefined4 Unit10211440_LoadFromDat(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1021BF60u))(a0, a1, a2); }
XI_ORIG undefined4 Unit10211440_Construct(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1021C070u))(a0, a1, a2, a3); }
XI_ORIG unsigned long long Unit10211440_SaveAndRelease(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[2] = {a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1021C100u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Unit10211440_CanUpdate(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1021C160u))(a0); }
XI_ORIG undefined4 Unit10211440_IsSpecialKey(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1021C1B0u))(a0); }
XI_ORIG undefined4 Unit10211440_AssignItem(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1021C210u))(a0, a1, a2); }
XI_ORIG undefined4 Unit10211440_ClearAndAssign(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1021C2E0u))(a0, a1, a2); }
XI_ORIG undefined4 Unit10211440_ResetSlot(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1021C350u))(a0, a1, a2); }
XI_ORIG undefined4 Unit10211440_SyncSlots(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1021C380u))(a0); }
XI_ORIG uint32_t Unit10211440_AppendEntry(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1021C5B0u))(a0, a1, a2, a3); }
XI_ORIG int Unit10211440_RemoveByKey(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1021C680u))(a0, a1); }
XI_ORIG unsigned long long Unit10211440_RebuildWithGlobalFlag(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) {
    uint32_t xs[3] = {a1.bits, a2.bits, a3.bits}; uint32_t ra = a4.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1021C7C0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Unit10211440_Rebuild(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1021C7E0u))(a0, a1, a2, a3, a4); }
XI_ORIG void * Unit10211440_ResetEntryCtor(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1021D250u))(a0); }
XI_ORIG undefined4 Unit10211440_Reset(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1021D270u))(a0); }
XI_ORIG undefined4 Unit10211440_LoadFromId(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1021D2D0u))(a0, a1, a2); }
XI_ORIG undefined4 Unit10211440_SetId(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1021D380u))(a0, a1, a2); }
XI_ORIG unsigned long long CXiMovie_DestroyGlobal(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1021D460u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG int CXiMovie_DrawFrame(int a0, int a1, int a2, int a3, void * a4) { return reinterpret_cast<int (__cdecl *)(int, int, int, int, void *)>(XI_GAME(0x1021D4C0u))(a0, a1, a2, a3, a4); }
XI_ORIG void * CXiMovie_Construct(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1021D7F0u))(a0); }
XI_ORIG uint32_t Unit1021D930_InitAndMarkReady(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1021D930u))(a0, a1); }
XI_ORIG undefined4 FUN_1021d960() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x1021D960u))(); }
XI_ORIG char Movie_Update(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x1021D980u))(a0); }
XI_ORIG unsigned long long Unit1021D930_ReleaseReturnTrue(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1021D9A0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG int FUN_1021d9b0(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1021D9B0u))(a0, a1); }
XI_ORIG int * FUN_1021dd50(void * a0) { return reinterpret_cast<int * (__thiscall *)(void *)>(XI_GAME(0x1021DD50u))(a0); }
XI_ORIG void Unit1021D930_NoOp() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1021DDC0u))(); }
XI_ORIG int Movie_GetCurrentTexture(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1021DDF0u))(a0); }
XI_ORIG int Unit1021D930_FindByName(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1021E3B0u))(a0, a1); }
XI_ORIG int Unit1021D930_FindForIds(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1021E480u))(a0, a1, a2); }
XI_ORIG void ShapeMng_FindShapes(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1021E4D0u))(a0, a1); }
XI_ORIG void Unit1021D930_ClearArrays(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1021E500u))(a0, a1, a2); }
XI_ORIG int Unit1021D930_UpdateIndexList(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1021E550u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG uint32_t Unit1021D930_IsIdInTable10485A30(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x1021E670u))(a0); }
XI_ORIG void Unit1021D930_ResetFields(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1021E690u))(a0); }
XI_ORIG void Unit1021D930_Init(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1021E760u))(a0, a1, a2, a3, a4); }
XI_ORIG void Unit1021D930_RotateBytesByBitCount(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1021E870u))(a0, a1); }
XI_ORIG void FUN_1021ed30() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x1021ED30u))(); }
XI_ORIG void Menu_SetPanelMetrics(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1021EE70u))(a0, a1, a2); }
XI_ORIG void FUN_1021ef10(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1021EF10u))(a0); }
XI_ORIG long double ActerBuff_FindNearest(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<long double (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x1021F030u))(a0, a1); }
XI_ORIG int ActerBuff_CollectNearby(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x1021F140u))(a0, a1); }
XI_ORIG void FUN_1021f240(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1021F240u))(a0); }
XI_ORIG void Unit1021D930_GetWindowRect(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x1021F360u))(a0); }
XI_ORIG void FUN_1021f480(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x1021F480u))(a0); }
XI_ORIG void Unit1021D930_DrawEntryRows(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1021F950u))(a0, a1); }
XI_ORIG undefined4 Unit1021D930_DrawLabelledRow(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1021FA90u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG void Unit1021D930_DrawTwoDigitValue(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1021FB10u))(a0, a1, a2); }
XI_ORIG void Unit1021D930_DrawBarRow(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1021FB80u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void Unit1021D930_DrawNameRows(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1021FBD0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void Unit1021D930_DrawLabelBar(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102201A0u))(a0, a1, a2, a3, a4); }
XI_ORIG int Unit1021D930_RefreshTargetGridText(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10220350u))(a0); }
XI_ORIG void FUN_102203b0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x102203B0u))(a0); }
XI_ORIG void ShapeMng_FillMenuShapes() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10220C80u))(); }
XI_ORIG undefined4 Menu_GetTableEntry(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10220CF0u))(a0, a1); }
XI_ORIG int sub_10220EF0(int a0, int a1, int a2, char a3) { return reinterpret_cast<int (__thiscall *)(int, int, int, char)>(XI_GAME(0x10220EF0u))(a0, a1, a2, a3); }
XI_ORIG int sub_10220F40(int a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(int, int, int)>(XI_GAME(0x10220F40u))(a0, a1, a2); }
XI_ORIG void * Unit1021D930_InitPanelState(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10220FD0u))(a0); }
XI_ORIG undefined4 FUN_10221030(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10221030u))(a0, a1, a2); }
XI_ORIG void FUN_102211f0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x102211F0u))(a0, a1); }
XI_ORIG void FUN_10221230(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10221230u))(a0, a1); }
XI_ORIG undefined4 FUN_102212c0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x102212C0u))(a0, a1); }
XI_ORIG void FUN_102212e0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x102212E0u))(a0, a1); }
XI_ORIG undefined4 FUN_10221320(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10221320u))(a0, a1); }
XI_ORIG undefined4 FUN_10221340(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10221340u))(a0, a1); }
XI_ORIG unsigned long long FUN_102213b0(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x102213B0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_102213e0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x102213E0u))(a0); }
XI_ORIG void Unit1021D930_Draw(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x102214D0u))(a0); }
XI_ORIG undefined4 FUN_10221680(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10221680u))(a0); }
XI_ORIG int sub_10221770(unsigned __int16 a0) { return reinterpret_cast<int (__cdecl *)(unsigned __int16)>(XI_GAME(0x10221770u))(a0); }
XI_ORIG void * Unit1021D930_InitMenuShapes() { return reinterpret_cast<void * (__cdecl *)()>(XI_GAME(0x102217A0u))(); }
XI_ORIG void * Unit1021D930_Construct(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x102217C0u))(a0); }
XI_ORIG void Unit1021D930_InitRect(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10221900u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_10221920(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10221920u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void FUN_102224c0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x102224C0u))(); }
XI_ORIG uint32_t Unit1021D930_ResolveAlias(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x10222C10u))(a0); }
XI_ORIG void * Unit1021D930_ConstructFromScene(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x102238E0u))(a0); }
XI_ORIG undefined * Unit1021D930_GetObjectOnce() { return reinterpret_cast<undefined * (__cdecl *)()>(XI_GAME(0x10223930u))(); }
XI_ORIG void * Unit1021D930_Ctor102239A0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x102239A0u))(a0); }
XI_ORIG void Unit1021D930_LoadFromFile(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x102239C0u))(a0); }
XI_ORIG void * Unit1021D930_InitAndRegister(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10223BB0u))(a0); }
XI_ORIG int FUN_10223c30() { return reinterpret_cast<int (__stdcall *)()>(XI_GAME(0x10223C30u))(); }
XI_ORIG char Unit1021D930_Refresh(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10223C90u))(a0); }
XI_ORIG void * Unit1021D930_ConstructFillMinus1(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10223E60u))(a0); }
XI_ORIG int Unit1021D930_FindByteIndex(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10225090u))(a0, a1); }
XI_ORIG void FUN_102250f0() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x102250F0u))(); }
XI_ORIG int Unit1021D930_ComputeCount(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10225120u))(a0, a1, a2, a3); }
XI_ORIG int Unit1021D930_ComputeDelta(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10225220u))(a0, a1, a2, a3); }
XI_ORIG void * Unit1021D930_ConstructSetGlobals(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x102254D0u))(a0); }
XI_ORIG void * Unit1021D930_InitIndexMap128(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10226080u))(a0); }
XI_ORIG undefined * Unit1021D930_GetObject106670C8Once() { return reinterpret_cast<undefined * (__cdecl *)()>(XI_GAME(0x10226180u))(); }
XI_ORIG void * Unit1021D930_Ctor102261F0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x102261F0u))(a0); }
XI_ORIG void Unit1021D930_LoadDMsgTable(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10226210u))(a0); }
XI_ORIG void * Unit1021D930_ConstructZeroFields(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10226550u))(a0); }
XI_ORIG void * Unit1021D930_InitIndexMap64(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10226C80u))(a0); }
XI_ORIG void FUN_10226e30() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x10226E30u))(); }
XI_ORIG void * Unit1021D930_Alloc1800(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10227080u))(a0); }
XI_ORIG undefined4 FUN_102270f0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x102270F0u))(a0); }
XI_ORIG undefined4 FUN_10227100(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10227100u))(a0); }
XI_ORIG undefined4 FUN_10227130(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10227130u))(a0); }
XI_ORIG void * Unit1021D930_ConstructAllocEntries(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x102271B0u))(a0); }
XI_ORIG void Unit1021D930_LoadNamedEntries(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x102271D0u))(a0); }
XI_ORIG void * Unit1021D930_ConstructBuffersAt68(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x102278B0u))(a0); }
XI_ORIG undefined4 FUN_10227920(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10227920u))(a0); }
XI_ORIG int FUN_10227930(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10227930u))(a0, a1); }
XI_ORIG void Unit1021D930_ClearHeaderFields(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x102281C0u))(a0); }
XI_ORIG void * Unit1021D930_ConstructBuffersAt8(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10228200u))(a0); }
XI_ORIG void * Unit1021D930_ConstructInitAt64(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10229670u))(a0); }
XI_ORIG undefined4 Unit1021D930_TestBitForId(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1022AF80u))(a0); }
XI_ORIG void Unit1021D930_GetBitIndexForId(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1022AFB0u))(a0, a1, a2); }
XI_ORIG undefined4 Unit1021D930_IsBitSetForId(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1022AFF0u))(a0); }
XI_ORIG void Unit1021D930_ClearCounters() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1022B260u))(); }
XI_ORIG void Unit1021D930_SetTimerSeconds(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1022B3B0u))(a0); }
XI_ORIG void * Unit1021D930_CtorVt1033C50C(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1022BD50u))(a0); }
XI_ORIG undefined * Unit1021D930_GetSingleton() { return reinterpret_cast<undefined * (__cdecl *)()>(XI_GAME(0x1022BDA0u))(); }
XI_ORIG void * Unit1021D930_SingletonCtor(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1022BE10u))(a0); }
XI_ORIG void Unit1021D930_LoadMessageTable(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1022BE30u))(a0); }
XI_ORIG void * Unit1021D930_MixDatCtor(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1022BFB0u))(a0); }
XI_ORIG void * Unit1021D930_PanelCtorVt1033C510(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1022C350u))(a0); }
XI_ORIG void Unit1021D930_Clear(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1022ED00u))(a0); }
XI_ORIG void Unit1021D930_Alloc(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1022ED30u))(a0, a1, a2); }
XI_ORIG undefined4 Unit1021D930_Append(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1022ED70u))(a0, a1); }
XI_ORIG int FUN_1022edf0(XI_ANY a0) { return reinterpret_cast<int (__stdcall *)(XI_ANY)>(XI_GAME(0x1022EDF0u))(a0); }
XI_ORIG undefined1 Unit1021D930_LoadStringTable(void * a0, XI_ANY a1) { return reinterpret_cast<undefined1 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1022EEE0u))(a0, a1); }
XI_ORIG undefined4 Unit1021D930_ReadChunk(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1022F240u))(a0); }
XI_ORIG void * Unit1021D930_PanelCtorVt1033C6B0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1022F610u))(a0); }
XI_ORIG void * Unit1021D930_CtorVt1033C718(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x102303B0u))(a0); }
XI_ORIG void Unit1021D930_Reset(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10230410u))(a0); }
XI_ORIG void * Unit1021D930_PanelCtorVt1033C720(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10233BE0u))(a0); }
XI_ORIG void * Unit1021D930_PanelCtorVt1033C788(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x102340A0u))(a0); }
XI_ORIG undefined8 Unit1021D930_SendMenuEventByKind(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x10234910u))(a0); }
XI_ORIG void Unit1021D930_SetPair(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10234A70u))(a0, a1, a2); }
XI_ORIG int Ui_KeyPrompt_Init(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10239460u))(a0, a1); }
XI_ORIG void Ui_KeyPrompt_Init_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10239460u))(a0, a1); }
XI_ORIG undefined4 Unit10237880_GetGlobalDword() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10239490u))(); }
XI_ORIG void FUN_1023be10(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1023BE10u))(a0, a1); }
XI_ORIG void Unit10237880_ForwardFlag1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1023BF20u))(a0); }
XI_ORIG void FUN_10240860() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x10240860u))(); }
XI_ORIG void LobbyThread_NoOp() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10244F20u))(); }
XI_ORIG void LobbyThread_InitState(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10245040u))(a0); }
XI_ORIG int LobbyThread_SetGlobal106690AC(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10245B50u))(a0); }
XI_ORIG void LobbyThread_ResetZone() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10245BB0u))(); }
XI_ORIG undefined4 LobbyThread_IsSystemState1() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10245BE0u))(); }
XI_ORIG undefined4 LobbyThread_NextSequence(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10245F30u))(a0); }
XI_ORIG undefined4 LobbyThread_SendCmd0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x10246000u))(a0, a1); }
XI_ORIG undefined8 LobbyThread_QueryChannels(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__stdcall *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10246160u))(a0, a1, a2); }
XI_ORIG undefined4 LobbyThread_SendCmd4(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10246320u))(a0, a1, a2); }
XI_ORIG void LobbyThread_SendCmd6(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x102463C0u))(a0); }
XI_ORIG uint32_t LobbyThread_QueueMusicFile(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<uint32_t (__stdcall *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102465E0u))(a0, a1, a2); }
XI_ORIG undefined8 LobbyThread_SendCmd11(XI_ANY a0) { return reinterpret_cast<undefined8 (__stdcall *)(XI_ANY)>(XI_GAME(0x102467F0u))(a0); }
XI_ORIG undefined8 LobbyThread_PushCmd(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { return reinterpret_cast<undefined8 (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1024C060u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG void * LobbyThread_ConstructQueue(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1024C930u))(a0); }
XI_ORIG undefined4 StAvatarInit(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1024CF50u))(a0); }
XI_ORIG char StAvatar_InitEntriesFromTables() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x1024D130u))(); }
XI_ORIG uint32_t StAvatar_Teardown(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1024D180u))(a0, a1, a2); }
XI_ORIG undefined4 StAvatar_ReleaseResources(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1024D2C0u))(a0, a1); }
XI_ORIG void StAvatar_Reset(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1024D400u))(a0); }
XI_ORIG void StAvatar_RegisterTriggers(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1024D470u))(a0, a1); }
XI_ORIG undefined4 StAvatar_SetTableValues(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1024D4A0u))(a0, a1); }
XI_ORIG undefined4 StAvatar_SetChannel(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1024D540u))(a0, a1, a2); }
XI_ORIG undefined4 StAvatar_SetPending(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1024D5C0u))(a0, a1); }
XI_ORIG undefined4 StAvatar_Unload(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1024D610u))(a0); }
XI_ORIG void StAvatar_ApplyChannels(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1024D6B0u))(a0); }
XI_ORIG undefined4 StAvatar_SetTransition(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1024D740u))(a0, a1, a2); }
XI_ORIG undefined4 StAvatar_StopAndFreeBuffers(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1024D790u))(a0); }
XI_ORIG void FUN_1024d850(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1024D850u))(a0); }
XI_ORIG undefined4 StAvatar_Update(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1024DB60u))(a0); }
XI_ORIG void StAvatar_UpdateInput(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1024DD10u))(a0, a1); }
XI_ORIG void StAvatar_UpdateCameraYaw(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1024DE20u))(a0); }
XI_ORIG void StAvatar_UpdateCameraFollow(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1024DFE0u))(a0); }
XI_ORIG void FUN_1024e280(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1024E280u))(a0, a1); }
XI_ORIG void StAvatar_SetFlagsBoth(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1024E720u))(a0, a1, a2); }
XI_ORIG void StAvatar_ProjectNodes(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1024F2C0u))(a0); }
XI_ORIG void StAvatar_UpdateSteer(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1024F5E0u))(a0, a1); }
XI_ORIG void StAvatar_SetViewFromDancerCamera(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1024F900u))(a0); }
XI_ORIG void StAvatar_AdvanceTimers(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1024FC10u))(a0, a1); }
XI_ORIG void FUN_1024fdf0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1024FDF0u))(a0, a1); }
XI_ORIG void StAvatar_ProcessTriggers(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1024FEC0u))(a0); }
XI_ORIG undefined4 StAvatar_GetOffset(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1024FF50u))(a0, a1); }
XI_ORIG unsigned long long FUN_10250030(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10250030u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 StAvatar_RenderIfEnabled(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10250190u))(a0, a1); }
XI_ORIG void StAvatarRender(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102501C0u))(a0); }
XI_ORIG int StAvatar_UpdateNodes(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x102502D0u))(a0); }
XI_ORIG undefined4 StAvatarFini(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102504D0u))(a0); }
XI_ORIG undefined4 StAvatar_ResetChannels(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10250580u))(a0, a1); }
XI_ORIG undefined4 StAvatar_SetFlagBit(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10250660u))(a0, a1, a2); }
XI_ORIG unsigned long long StAvatar_SetStopped(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[2] = {a0.bits, a1.bits}; uint32_t ra = a2.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $8, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x102506C0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 StAvatar_SetField10(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10250760u))(a0, a1); }
XI_ORIG undefined4 StAvatar_StoreMode(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102507C0u))(a0, a1); }
XI_ORIG undefined4 StAvatar_Start(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10250800u))(a0, a1); }
XI_ORIG void StAvatar_SetModelFlags(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102509C0u))(a0, a1); }
XI_ORIG void StAvatar_SetModelItemFlags(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10250A40u))(a0, a1, a2); }
XI_ORIG undefined4 StAvatar_SetLock(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10250B10u))(a0, a1, a2); }
XI_ORIG undefined4 StAvatar_GetField20(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10250C00u))(a0, a1); }
XI_ORIG int StAvatar_InitIndices(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x10250D80u))(a0); }
XI_ORIG undefined4 StChannelItem_Load(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10250DB0u))(a0, a1); }
XI_ORIG undefined FUN_10250e10() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10250E10u))(); }
XI_ORIG void FUN_10251020(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10251020u))(a0, a1); }
XI_ORIG uint32_t StChannel_RegisterItem(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10251070u))(a0, a1, a2); }
XI_ORIG undefined4 StChannel_ClearFlags(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10251160u))(a0); }
XI_ORIG undefined4 StChannel_MarkIfLoaded(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10251180u))(a0); }
XI_ORIG undefined4 StChannel_SetValue9C(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102511B0u))(a0, a1); }
XI_ORIG undefined4 StChannel_ApplyItem(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10251200u))(a0, a1, a2); }
XI_ORIG undefined4 StChannelItemClear(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102512B0u))(a0); }
XI_ORIG undefined4 StAvatar_UpdateFlag2(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10251320u))(a0, a1); }
XI_ORIG undefined4 StAvatar_GetValue2B(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10251370u))(a0, a1); }
XI_ORIG undefined4 StAvatar_ComputeRatioValue(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10251410u))(a0, a1); }
XI_ORIG int StDancer_Update(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10251500u))(a0); }
XI_ORIG int sub_102515F0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x102515F0u))(a0, a1); }
XI_ORIG void StAvatar_SetActive(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10251620u))(a0, a1, a2, a3, a4); }
XI_ORIG void StAvatar_SetStateTeardown(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x102516B0u))(a0, a1, a2); }
XI_ORIG void StAvatar_SetMode(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10251720u))(a0, a1); }
XI_ORIG undefined4 StAvatar_SetLevel(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10251750u))(a0, a1); }
XI_ORIG unsigned long long Dancer_SetStateIfIdle(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10251780u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void StDancer_SetJobLock(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10251890u))(a0, a1); }
XI_ORIG void StAvatar_ResetOrSetFlag2(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10251920u))(a0, a1); }
XI_ORIG int StAvatar_IsFlag20(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10251960u))(a0); }
XI_ORIG uint32_t Dancer_InitDevice(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x10251A80u))(a0); }
XI_ORIG undefined4 StAvatar_ReleaseMemory() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10251CE0u))(); }
XI_ORIG int StAvatar_InitSlotRecord(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x102524B0u))(a0); }
XI_ORIG undefined4 StModelItemClear(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102524E0u))(a0); }
XI_ORIG undefined4 StModelItem_ReadStart(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10252610u))(a0, a1); }
XI_ORIG undefined FUN_102526f0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102526F0u))(); }
XI_ORIG undefined FUN_10252790() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10252790u))(); }
XI_ORIG undefined4 Unit10252A00_Activate(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10252A00u))(a0, a1); }
XI_ORIG undefined4 Unit10252A00_CheckFileRefs(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10252A40u))(a0); }
XI_ORIG undefined4 Unit10252A00_SetPositionAndScale(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10252AC0u))(a0, a1, a2); }
XI_ORIG undefined4 Unit10252A00_SetVec78(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10252B50u))(a0, a1); }
XI_ORIG undefined4 Unit10252A00_ClearModelItem(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10252C10u))(a0); }
XI_ORIG undefined4 Unit10252A00_Refresh(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10252C30u))(a0, a1); }
XI_ORIG int Unit10252A00_Init(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x10252C80u))(a0); }
XI_ORIG int StTrigger_ProcessOpcodesUpToTime(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10252CC0u))(a0, a1, a2, a3); }
XI_ORIG int StTrigger_FindIndexAtLeast(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10252EA0u))(a0, a1, a2); }
XI_ORIG undefined4 StTrigger_RegisterCallbacks(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10252F00u))(a0, a1, a2); }
XI_ORIG undefined StTrigger_AllocFromPool() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10252F70u))(); }
XI_ORIG undefined4 StAvatar_CheckBuffersReady(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10253160u))(a0); }
XI_ORIG undefined4 sqopSystemActivate(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102531B0u))(a0); }
XI_ORIG undefined4 StAvatar_ClearResource(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10253210u))(a0); }
XI_ORIG undefined4 Unit10253210_GetInfoBuffer(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102532A0u))(a0); }
XI_ORIG unsigned long long FUN_102532c0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[3] = {a0.bits, a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $12, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x102532C0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG _DWORD * Unit10253210_Ctor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x102532E0u))(a0); }
XI_ORIG void Unit10253210_NoOp() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10253330u))(); }
XI_ORIG void StAvatar_LoadInfoFile(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x10253360u))(a0); }
XI_ORIG int StAvatar_GetInfoBuffer(XI_ANY a0) { return reinterpret_cast<int (__stdcall *)(XI_ANY)>(XI_GAME(0x102534B0u))(a0); }
XI_ORIG unsigned long long Unit10253210_SendBytePairKind0(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[2] = {a0.bits, a1.bits}; uint32_t ra = a2.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $8, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x102535F0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Unit10253210_GetInfoBuffer0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10253620u))(); }
XI_ORIG char Unit10253210_SendBytePairKind2(__int16 a0, char a1) { return reinterpret_cast<char (__cdecl *)(__int16, char)>(XI_GAME(0x10253670u))(a0, a1); }
XI_ORIG void Unit10253210_SendBytePairKind2_v1(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10253670u))(a0, a1); }
XI_ORIG undefined4 Unit10253210_GetInfoBuffer2() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x102536A0u))(); }
XI_ORIG undefined4 Unit10253A20_InvertBytes(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102572E0u))(a0, a1, a2); }
XI_ORIG unsigned long long FUN_10257310(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10257310u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Unit10253A20_SwapEndianPair(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102573C0u))(a0); }
XI_ORIG unsigned long long FUN_102573e0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x102573E0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG short * Unit10253A20_GetEntryPtr(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<short * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102574F0u))(a0, a1); }
XI_ORIG short * Unit10253A20_GetCellData(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<short * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10257550u))(a0, a1, a2); }
XI_ORIG int Unit10253A20_Align4(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x10257590u))(a0); }
XI_ORIG undefined4 Unit10253A20_IsNotHashOrDot(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102575B0u))(a0); }
XI_ORIG undefined4 Unit10253A20_GetWord40(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102575D0u))(a0); }
XI_ORIG int Unit10253A20_ComputeTotalSize(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x102575E0u))(a0); }
XI_ORIG undefined4 * Unit10253A20_Clone(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10257670u))(a0, a1); }
XI_ORIG undefined * Unit10253A20_GetDataOrDefault(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102577C0u))(a0, a1, a2, a3); }
XI_ORIG undefined4 Unit10253A20_GetDwordEntry(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10257800u))(a0, a1); }
XI_ORIG undefined1 * Unit10253A20_GetDMsgTable() { return reinterpret_cast<undefined1 * (__cdecl *)()>(XI_GAME(0x10257830u))(); }
XI_ORIG void Unit10253A20_CallFreeData(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10257840u))(a0); }
XI_ORIG void Unit10253A20_FreeData(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10257850u))(a0); }
XI_ORIG void FUN_10257860(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10257860u))(a0); }
XI_ORIG int Unit10253A20_GetRowBase(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10257870u))(a0, a1); }
XI_ORIG short * Unit10253A20_GetCell(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<short * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x102578B0u))(a0, a1, a2); }
XI_ORIG undefined4 Unit10253A20_GetDwordCell(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10257920u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10257950(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10257950u))(a0, a1); }
XI_ORIG undefined4 Unit10253A20_GetCellCol0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10257950u))(a0, a1); }
XI_ORIG undefined4 Unit10253A20_Assign(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10257980u))(a0, a1, a2); }
XI_ORIG void Unit10253A20_Bind(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x102579B0u))(a0, a1, a2); }
XI_ORIG undefined4 Unit10253A20_GetRowCount(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10257AF0u))(a0); }
XI_ORIG undefined4 FUN_10257b10(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10257B10u))(a0); }
XI_ORIG undefined4 Unit10253A20_IsBound(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10257B10u))(a0); }
XI_ORIG void Unit10253A20_Clear(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10257B60u))(a0); }
XI_ORIG void Unit10253A20_BindSource(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10257BA0u))(a0, a1, a2); }
XI_ORIG int Unit10253A20_GetRowBytes(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10257BC0u))(a0); }
XI_ORIG void Unit10253A20_BindDataAndMark(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10257BD0u))(a0, a1); }
XI_ORIG void Unit10253A20_BindData(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10257BF0u))(a0, a1); }
XI_ORIG undefined4 Unit10253A20_GetDataField28(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10257C60u))(a0); }
XI_ORIG undefined4 Unit10253A20_GetStride(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10257C70u))(a0); }
XI_ORIG undefined4 Unit10253A20_GetRowInfo(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10257C80u))(a0, a1, a2); }
XI_ORIG undefined4 Unit10253A20_IsTypeRange(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10257FF0u))(a0); }
XI_ORIG undefined * Unit10253A20_PickFormString(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10258150u))(a0, a1, a2, a3, a4); }
XI_ORIG int Unit10253A20_FindFirstTypeByte(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x102581D0u))(a0); }
XI_ORIG void FUN_10258430(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10258430u))(a0, a1, a2, a3); }
XI_ORIG void FUN_10258600() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x10258600u))(); }
XI_ORIG undefined4 Unit10253A20_FindFirstTypeByteInText(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x102586D0u))(a0); }
XI_ORIG undefined4 Unit10253A20_GetDataPtr(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x102586F0u))(a0); }
XI_ORIG void Unit10253A20_FormatTextAtC(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10258740u))(a0, a1, a2, a3); }
XI_ORIG void FUN_10258830() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x10258830u))(); }
XI_ORIG void FUN_102589f0() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x102589F0u))(); }
XI_ORIG undefined4 Unit10253A20_GetText14(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10258AD0u))(a0); }
XI_ORIG void FUN_10258bb0() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x10258BB0u))(); }
XI_ORIG undefined * Unit10253A20_PickFormString2(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10258D00u))(a0, a1, a2, a3); }
XI_ORIG char * Unit10253A20_SelectQuantifierForm(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<char * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10258D70u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined1 * Unit10253A20_GetSolchForm(XI_ANY a0) { return reinterpret_cast<undefined1 * (__cdecl *)(XI_ANY)>(XI_GAME(0x10258E10u))(a0); }
XI_ORIG undefined * Unit10253A20_SelectNounForm(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10258E50u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined1 * Unit10253A20_GetQuantifierByIndex(XI_ANY a0) { return reinterpret_cast<undefined1 * (__cdecl *)(XI_ANY)>(XI_GAME(0x10258ED0u))(a0); }
XI_ORIG undefined * Unit10253A20_SelectAdjectiveForm(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10258F00u))(a0, a1, a2); }
XI_ORIG undefined * Unit10253A20_SelectGenderForm(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10258F60u))(a0, a1, a2); }
XI_ORIG void Unit10253A20_Append(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10259100u))(a0, a1); }
XI_ORIG undefined4 Unit10253A20_IsEmpty(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10259130u))(a0); }
XI_ORIG int Unit10253A20_FindNextTyped(XI_ANY a0) { return reinterpret_cast<int (__stdcall *)(XI_ANY)>(XI_GAME(0x10259150u))(a0); }
XI_ORIG undefined4 Unit10253A20_IsTypeTwoZero(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1025A580u))(a0, a1); }
XI_ORIG undefined1 * Unit10253A20_GetFormById(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined1 * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1025A5A0u))(a0, a1, a2); }
XI_ORIG undefined1 * Unit10253A20_GetFormAlt(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined1 * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1025A610u))(a0, a1, a2); }
XI_ORIG undefined1 * Unit10253A20_GetSuffixByFlag(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined1 * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1025A6C0u))(a0, a1); }
XI_ORIG undefined1 * Unit10253A20_GetEntryById(XI_ANY a0) { return reinterpret_cast<undefined1 * (__cdecl *)(XI_ANY)>(XI_GAME(0x1025A6F0u))(a0); }
XI_ORIG undefined * Unit10253A20_GetPrefixByFlags(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1025A730u))(a0, a1, a2); }
XI_ORIG undefined1 * Unit10253A20_GetPrefixByFlags2(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined1 * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1025A760u))(a0, a1, a2, a3); }
XI_ORIG void Unit10253A20_AppendNode(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1025A860u))(a0, a1); }
XI_ORIG int Unit10253A20_FindNextTyped9(XI_ANY a0) { return reinterpret_cast<int (__stdcall *)(XI_ANY)>(XI_GAME(0x1025A890u))(a0); }
XI_ORIG undefined4 Unit10253A20_ContainsChar(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1025A8C0u))(a0); }
XI_ORIG uint32_t Unit10253A20_BuildNodeString(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1025A930u))(a0, a1); }
XI_ORIG undefined4 Unit10253A20_HasNoHead(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1025AA90u))(a0); }
XI_ORIG void Unit10253A20_LinkNode(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1025B100u))(a0, a1); }
XI_ORIG void Unit10253A20_SelectTableByLanguage(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1025B4F0u))(a0); }
XI_ORIG int Scene_GetTableValue(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x1025B560u))(a0); }
XI_ORIG undefined4 Scene_GetTableValue_v1(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1025B560u))(a0); }
XI_ORIG undefined * Scene_GetMenuFrameName(XI_ANY a0) { return reinterpret_cast<undefined * (__cdecl *)(XI_ANY)>(XI_GAME(0x1025B5A0u))(a0); }
XI_ORIG int Unit10253A20_CopyWithoutMarker(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1025B5E0u))(a0, a1, a2, a3); }
XI_ORIG int Unit10253A20_CountChars(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1025B680u))(a0, a1); }
XI_ORIG undefined4 Unit10253A20_HasKnownPrefix(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1025BB00u))(a0); }
XI_ORIG uint32_t FUN_1025bbe0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1025BBE0u))(a0, a1, a2); }
XI_ORIG int Unit10253A20_FindMarker(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1025BDE0u))(a0, a1); }
XI_ORIG undefined4 Unit10253A20_IsWrapBreak(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1025BE20u))(a0); }
XI_ORIG uint32_t FUN_1025be80(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x1025BE80u))(a0); }
XI_ORIG uint32_t Unit10253A20_ConvertCharByLanguage(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1025BFB0u))(a0, a1, a2); }
XI_ORIG int FUN_1025c0f0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1025C0F0u))(); }
XI_ORIG int Unit10253A20_WrapSjisAt(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1025C0F0u))(a0, a1, a2); }
XI_ORIG undefined4 Unit1025C9E0_ReturnFalse(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1025E020u))(a0); }
XI_ORIG unsigned long long Unit1025C9E0_ReturnFalse_v1(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1025E020u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FontGlyphCache_IsStateTwo() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1025E050u))(); }
XI_ORIG void FUN_1025e090(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1025E090u))(a0); }
XI_ORIG void * FontGlyphCache_GetCounterBlock() { return reinterpret_cast<void * (__cdecl *)()>(XI_GAME(0x1025E0B0u))(); }
XI_ORIG void FontGlyphCache_ResetCounter() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1025E0C0u))(); }
XI_ORIG int FontGlyphCache_StepCounter() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1025E0E0u))(); }
XI_ORIG char Unit1025C9E0_ReturnFalseFwd() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x1025E0F0u))(); }
XI_ORIG void FontGlyphCache_AddToCounter(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1025E210u))(a0); }
XI_ORIG void FontGlyphCache_ClearCounter() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1025E230u))(); }
XI_ORIG void FontGlyphCache_DecayCounter() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1025E240u))(); }
XI_ORIG void * Unit1025C9E0_BaseCtor(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1025E260u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 Unit1025C9E0_ReleaseField6C(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1025F2F0u))(a0); }
XI_ORIG undefined4 FUN_1025f640(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1025F640u))(a0); }
XI_ORIG void * Unit1025C9E0_DerivedCtor(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1025FD50u))(a0, a1, a2, a3, a4); }
XI_ORIG void Unit1025C9E0_ResetState(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1025FE40u))(a0); }
XI_ORIG void * Unit1025C9E0_InnerCtor(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10261210u))(a0, a1, a2, a3, a4); }
XI_ORIG void Unit1025C9E0_InnerDtor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x102612D0u))(a0); }
XI_ORIG void * FUN_10265940(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10265940u))(a0, a1); }
XI_ORIG void _dynamic_initializer_for__std__num_put_char_std__ostreambuf_iterator_char_std__char_traits_char_______id___47() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10265960u))(); }
XI_ORIG void * Unit1025C9E0_RefObjCtor(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x102659B0u))(a0, a1, a2); }
XI_ORIG void * FUN_10265b70(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10265B70u))(a0, a1); }
XI_ORIG void FUN_10265b90(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10265B90u))(a0); }
XI_ORIG void FUN_1026cc30() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1026CC30u))(); }
XI_ORIG void Unit1025C9E0_ChannelInit(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026CCA5u))(a0, a1); }
XI_ORIG undefined4 FUN_1026cdf4(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1026CDF4u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1026cdf4_v1() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1026CDF4u))(); }
XI_ORIG undefined4 Unit1025C9E0_MoveOneUsedToFree(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1026CFA0u))(a0); }
XI_ORIG void Unit1025C9E0_MoveAllBack(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1026D015u))(a0); }
XI_ORIG undefined4 Unit1025C9E0_RingPop(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026D0CAu))(a0, a1); }
XI_ORIG undefined4 FontGlyphCache_PeekRing(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1026D179u))(a0); }
XI_ORIG undefined4 FUN_1026d1b4(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026D1B4u))(a0, a1); }
XI_ORIG unsigned long long FUN_1026d634(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1026D634u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Unit1025C9E0_Restart(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1026D70Cu))(a0); }
XI_ORIG undefined4 Unit1025C9E0_InitChannels(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1026D866u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG void FUN_1026d9b1(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1026D9B1u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1026dac6(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1026DAC6u))(a0); }
XI_ORIG uint8_t FontGlyphCache_MarkState3(XI_ANY a0) { return reinterpret_cast<uint8_t (__cdecl *)(XI_ANY)>(XI_GAME(0x1026DB24u))(a0); }
XI_ORIG undefined4 Unit1025C9E0_PopRingC044(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026DB6Du))(a0, a1); }
XI_ORIG void Unit1025C9E0_PeekRingC044(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1026DB8Fu))(a0); }
XI_ORIG void Unit1025C9E0_PutRing803C(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026DBACu))(a0, a1); }
XI_ORIG void FUN_1026dbce() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1026DBCEu))(); }
XI_ORIG void Unit1025C9E0_PopRing4034(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026DBCEu))(a0, a1); }
XI_ORIG undefined4 FUN_1026dbf0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1026DBF0u))(); }
XI_ORIG undefined4 Unit1025C9E0_PeekRing4034(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1026DBF0u))(a0); }
XI_ORIG void FUN_1026dc0d() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1026DC0Du))(); }
XI_ORIG void Unit1025C9E0_PutRing2C(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026DC0Du))(a0, a1); }
XI_ORIG void sqxfXformTD_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1026DF00u))(a0); }
XI_ORIG undefined FUN_1026df90() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1026DF90u))(); }
XI_ORIG void sqxfXformTD_Update(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1026E1F0u))(a0); }
XI_ORIG void sqxfXformTD_Update_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1026E1F0u))(); }
XI_ORIG undefined FUN_1026e2c0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1026E2C0u))(); }
XI_ORIG void FUN_1026e2f0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1026E2F0u))(a0); }
XI_ORIG undefined FUN_1026e350() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1026E350u))(); }
XI_ORIG void SqgxContext_StoreVec14(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1026E3D0u))(a0); }
XI_ORIG int StAvatar_InitGlobals1099AEB8() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1026E400u))(); }
XI_ORIG _DWORD * StAvatar_AllocArrays(int a0, int a1) { return reinterpret_cast<_DWORD * (__cdecl *)(int, int)>(XI_GAME(0x1026E460u))(a0, a1); }
XI_ORIG undefined4 StAvatar_InitModel(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1026E530u))(a0); }
XI_ORIG void StAvatar_UpdateModels(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1026E590u))(a0); }
XI_ORIG void mdl_1026E900() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1026E900u))(); }
XI_ORIG int Mdl_Register(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026EBF0u))(a0, a1); }
XI_ORIG undefined4 Mdl_FreeAllSlots() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1026ECC0u))(); }
XI_ORIG void Mdl_ClearSlotFlags() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1026EE00u))(); }
XI_ORIG int Mdl_ChannelRegister(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026EF00u))(a0, a1); }
XI_ORIG undefined4 FUN_1026efb0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026EFB0u))(a0, a1); }
XI_ORIG undefined4 FUN_1026f080(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1026F080u))(a0, a1, a2); }
XI_ORIG undefined4 Mdl_ReleaseEntries() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1026F1B0u))(); }
XI_ORIG undefined4 Mdl_GetEntryTable(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026F4F0u))(a0, a1); }
XI_ORIG void Mdl_QueryEntryFlag(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026F580u))(a0, a1); }
XI_ORIG void Mdl_GetEntryWord0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026F7B0u))(a0, a1); }
XI_ORIG void Mdl_SetChannelActive(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1026F900u))(a0, a1, a2); }
XI_ORIG void Mdl_UpdateEntryFlags(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1026F9A0u))(a0, a1, a2); }
XI_ORIG void Mdl_SetChannelFlag80(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026F9E0u))(a0, a1); }
XI_ORIG undefined4 Unit1026EA00_ComputeMinMax(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1026FBB0u))(a0); }
XI_ORIG undefined4 Mdl_ComputeSlotBounds(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026FF80u))(a0, a1); }
XI_ORIG void Mdl_SetSlotLock(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102701B0u))(a0, a1, a2, a3, a4); }
XI_ORIG void Mdl_GetSlotWord0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10270290u))(a0, a1); }
XI_ORIG void Mdl_UpdateSlotFlags(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102703C0u))(a0, a1, a2); }
XI_ORIG void Mdl_SetSlotVisible(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10270410u))(a0, a1); }
XI_ORIG void Mdl_ReleaseSlotPtr(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10270690u))(a0); }
XI_ORIG undefined4 FUN_10270840(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10270840u))(a0); }
XI_ORIG undefined4 FUN_102709e0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102709E0u))(a0); }
XI_ORIG int Unit102706F0_InitScene() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10270B60u))(); }
XI_ORIG void StAvatar_UpdateTransform() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10270EC0u))(); }
XI_ORIG undefined4 Unit102706F0_UpdateScene(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10270F30u))(a0); }
XI_ORIG void StAvatar_ApplyModes(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10271030u))(a0); }
XI_ORIG void FUN_102710b0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102710B0u))(a0, a1, a2, a3); }
XI_ORIG void FUN_102712e0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102712E0u))(a0, a1, a2, a3, a4); }
XI_ORIG void StAvatar_ApplyEffectFlags(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102715F0u))(a0); }
XI_ORIG void StAvatar_UpdateAngles(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10271630u))(a0, a1, a2, a3); }
XI_ORIG void StAvatar_DrawScene() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x102717F0u))(); }
XI_ORIG char scnFini() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x10271B40u))(); }
XI_ORIG undefined4 scnFini_v1() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10271B40u))(); }
XI_ORIG void StAvatar_UpdateFlagBits(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10271B90u))(a0, a1); }
XI_ORIG void StAvatar_SetOffsets(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10271C30u))(a0, a1, a2); }
XI_ORIG void StAvatar_SetSceneValue1(int a0, int a1) { reinterpret_cast<void (__cdecl *)(int, int)>(XI_GAME(0x10271C60u))(a0, a1); }
XI_ORIG void StAvatar_SetSceneValue2(int a0, int a1) { reinterpret_cast<void (__cdecl *)(int, int)>(XI_GAME(0x10271C80u))(a0, a1); }
XI_ORIG void StAvatar_StoreParams(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10271CA0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG void StAvatar_StoreEntry() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10271D00u))(); }
XI_ORIG void StAvatar_SelectEntry(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10271D70u))(a0); }
XI_ORIG _DWORD * scnGetFlags(void * a0) { return reinterpret_cast<_DWORD * (__cdecl *)(void *)>(XI_GAME(0x10271E90u))(a0); }
XI_ORIG void scnGetFlags_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10271E90u))(a0); }
XI_ORIG void StAvatar_InitEntry(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10271F20u))(a0); }
XI_ORIG void Unit102706F0_NoOpA() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x102720A0u))(); }
XI_ORIG void StAvatar_SetEntryPosition(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102725A0u))(a0, a1); }
XI_ORIG void StAvatar_SetEntryFlag(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10272630u))(a0, a1); }
XI_ORIG void StAvatar_SetFlag3(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102726E0u))(a0); }
XI_ORIG void StAvatar_SetEntryFlag2(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10272750u))(a0, a1); }
XI_ORIG void StAvatar_SetEntryColor(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102727F0u))(a0, a1); }
XI_ORIG void StAvatar_SetEntryTransform(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10272880u))(a0, a1, a2); }
XI_ORIG void StAvatar_ToggleFlag168(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102729E0u))(a0); }
XI_ORIG void StAvatar_SelectIndex(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10272A10u))(a0); }
XI_ORIG void Unit102706F0_NoOpB() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10272BA0u))(); }
XI_ORIG int scnCameraGetInfo(int a0, void * a1, void * a2) { return reinterpret_cast<int (__cdecl *)(int, void *, void *)>(XI_GAME(0x10273370u))(a0, a1, a2); }
XI_ORIG void scnCameraGetInfo_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10273370u))(a0, a1, a2); }
XI_ORIG void Unit10272F20_SelectCamera(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102733B0u))(a0); }
XI_ORIG void Unit10272F20_SetCameraEntry(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10273490u))(a0, a1, a2); }
XI_ORIG void Unit10272F20_UpdateCameraBlend(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10273560u))(a0); }
XI_ORIG void Unit10272F20_SetVtable1099B138(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102736D0u))(a0); }
XI_ORIG void Unit10272F20_ApplySceneToModel(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102736E0u))(a0, a1); }
XI_ORIG void FUN_10273810(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10273810u))(a0, a1); }
XI_ORIG void Unit10273840_ClearGroupField4(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10273870u))(a0); }
XI_ORIG void Unit10273840_ClearCurrentField4() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x102738A0u))(); }
XI_ORIG _DWORD * Unit10273840_InitRecord(void * a0) { return reinterpret_cast<_DWORD * (__cdecl *)(void *)>(XI_GAME(0x102738D0u))(a0); }
XI_ORIG void chasm_10273920(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10273920u))(a0); }
XI_ORIG undefined4 Chasm_ToggleLayerState(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10273E80u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10273e80(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10273E80u))(a0, a1, a2); }
XI_ORIG int Unit10274050_Relink(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102741F0u))(a0, a1); }
XI_ORIG undefined FUN_102742c0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102742C0u))(); }
XI_ORIG undefined4 Unit10274050_GetInterpolated(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10274370u))(a0, a1, a2, a3); }
XI_ORIG void Unit10274050_ZeroDwords(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10274640u))(a0); }
XI_ORIG void FUN_10274650(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10274650u))(a0, a1); }
XI_ORIG void Unit10274050_SetXyz(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10274670u))(a0, a1, a2, a3); }
XI_ORIG long double FUN_102746c0(XI_ANY a0) { return reinterpret_cast<long double (__cdecl *)(XI_ANY)>(XI_GAME(0x102746C0u))(a0); }
XI_ORIG long double FUN_10274870(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<long double (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10274870u))(a0, a1); }
XI_ORIG long double FUN_102748f0(XI_ANY a0) { return reinterpret_cast<long double (__cdecl *)(XI_ANY)>(XI_GAME(0x102748F0u))(a0); }
XI_ORIG void Unit10274050_Sub(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102749C0u))(a0, a1, a2); }
XI_ORIG void Unit10274050_LerpFactor(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10274A20u))(a0, a1, a2, a3); }
XI_ORIG int PackVec4(int a0, float a1, int a2, int a3, int a4) { return reinterpret_cast<int (__cdecl *)(int, float, int, int, int)>(XI_GAME(0x10274D40u))(a0, a1, a2, a3, a4); }
XI_ORIG void PackVec4_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10274D40u))(a0, a1, a2, a3, a4); }
XI_ORIG int FUN_10275090() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10275090u))(); }
XI_ORIG int sub_10275090() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10275090u))(); }
XI_ORIG int sqMemInit() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10275150u))(); }
XI_ORIG void SqMem_InitBlock(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10275190u))(a0); }
XI_ORIG void FUN_102751d0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x102751D0u))(a0); }
XI_ORIG undefined4 SqMem_ClassifyBlock(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102752F0u))(a0); }
XI_ORIG int SqMem_GetPayloadEnd(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x10275320u))(a0); }
XI_ORIG undefined8 SqMem_Check() { return reinterpret_cast<undefined8 (__cdecl *)()>(XI_GAME(0x10275510u))(); }
XI_ORIG unsigned long long SqMem_HexDump(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[2] = {a0.bits, a1.bits}; uint32_t ra = a2.bits, rc = 0, rd = a3.bits;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $8, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x102755C0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 * SqMem_FindArena(XI_ANY a0) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY)>(XI_GAME(0x10275670u))(a0); }
XI_ORIG void FUN_102756a0(void * a0, void * a1) { reinterpret_cast<void (__fastcall *)(void *, void *)>(XI_GAME(0x102756A0u))(a0, a1); }
XI_ORIG void sqMemPoolRegister(int a0, int a1, int a2, void * a3) { reinterpret_cast<void (__cdecl *)(int, int, int, void *)>(XI_GAME(0x10275B60u))(a0, a1, a2, a3); }
XI_ORIG undefined4 * SqMem_AllocZeroed(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10275D40u))(a0, a1, a2, a3); }
XI_ORIG undefined4 * FUN_10275d80(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10275D80u))(a0, a1, a2, a3); }
XI_ORIG undefined4 * SqMem_Realloc(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10275E90u))(a0, a1, a2, a3); }
XI_ORIG int sqFree(int a0, void * a1, int a2) { return reinterpret_cast<int (__cdecl *)(int, void *, int)>(XI_GAME(0x102760F0u))(a0, a1, a2); }
XI_ORIG undefined4 sqFree_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102760F0u))(a0, a1, a2); }
XI_ORIG _DWORD * sqgxContext_Create() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x10276260u))(); }
XI_ORIG void sqgxContext_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10276290u))(a0); }
XI_ORIG undefined FUN_10276770() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10276770u))(); }
XI_ORIG undefined sqgxContext_Dump() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102768E0u))(); }
XI_ORIG uint32_t SqmdModel_Validate(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x10277A50u))(a0); }
XI_ORIG uint32_t SqmdModel_Validate_v1() { return reinterpret_cast<uint32_t (__cdecl *)()>(XI_GAME(0x10277A50u))(); }
XI_ORIG void SqmdModel_Free(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10277AB0u))(a0); }
XI_ORIG void SqmdModel_Free_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10277AB0u))(); }
XI_ORIG void SqmdModel_FreeBuffers(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10277B70u))(a0); }
XI_ORIG undefined FUN_10277ff0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10277FF0u))(); }
XI_ORIG void SqmdModel_Update(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10278490u))(a0); }
XI_ORIG void SqmdModel_Update_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10278490u))(); }
XI_ORIG void SqmdModel_Link(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10278710u))(a0, a1, a2, a3); }
XI_ORIG void SqmdModel_LinkShape(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102787F0u))(a0, a1, a2, a3, a4); }
XI_ORIG void SqmdModel_AddIllumLight(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10278990u))(a0, a1, a2, a3); }
XI_ORIG void SqmdModel_AddShadowLightInLayer(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10278B50u))(a0, a1, a2, a3, a4); }
XI_ORIG void SqmdModel_SetLayerSort(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10278E10u))(a0, a1); }
XI_ORIG void SqmdModel_LookAt(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10278E90u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 SqmdModel_GetItem(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10279710u))(a0, a1, a2); }
XI_ORIG undefined4 SqmdModel_GetItemKind0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10279830u))(a0, a1); }
XI_ORIG undefined4 SqmdModel_GetItemKind1(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10279850u))(a0, a1); }
XI_ORIG undefined4 SqmdModel_GetItemKind2(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10279870u))(a0, a1); }
XI_ORIG undefined4 SqmdModel_GetItemKind3(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10279890u))(a0, a1); }
XI_ORIG undefined4 SqmdModel_GetItemKind4(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102798B0u))(a0, a1); }
XI_ORIG char * sqmdModel_GetItemName(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<char * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10279AB0u))(a0, a1, a2); }
XI_ORIG undefined4 SqmdModel_GetItemNameKind3(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10279B40u))(a0, a1); }
XI_ORIG char * SqmdModel_FindItemName(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<char * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10279BA0u))(a0, a1, a2); }
XI_ORIG undefined4 SqmdModel_FindItemByName(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10279BF0u))(a0, a1, a2, a3); }
XI_ORIG undefined4 SqmdModel_LookupKind2(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10279D50u))(a0, a1, a2); }
XI_ORIG char * SqmdModel_ItemKindName(XI_ANY a0) { return reinterpret_cast<char * (__cdecl *)(XI_ANY)>(XI_GAME(0x1027A1C0u))(a0); }
XI_ORIG void FUN_1027a220(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027A220u))(a0, a1); }
XI_ORIG void SqmdModel_FreeItems(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027A250u))(a0); }
XI_ORIG void Unit1027A4B0_SetIdentityFwd(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027A6F0u))(a0); }
XI_ORIG undefined4 Unit1027A4B0_Copy(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027A700u))(a0, a1); }
XI_ORIG void Unit1027A4B0_Set(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027A720u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_1027a8a0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027A8A0u))(a0, a1); }
XI_ORIG void Unit1027A4B0_ToMatrix3(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027A9D0u))(a0, a1); }
XI_ORIG void FUN_1027aae0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027AAE0u))(a0, a1, a2, a3); }
XI_ORIG void Unit1027A4B0_FromRotationX(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027ABA0u))(a0, a1); }
XI_ORIG void Unit1027A4B0_FromRotationY(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027ABD0u))(a0, a1); }
XI_ORIG void Unit1027A4B0_FromAxisAngle(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027AC30u))(a0, a1, a2); }
XI_ORIG void Unit1027A4B0_SetIdentity(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027AE50u))(a0); }
XI_ORIG long double FUN_1027aec0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<long double (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027AEC0u))(a0, a1); }
XI_ORIG void Unit1027A4B0_Negate(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027AEF0u))(a0); }
XI_ORIG undefined4 SqQuat_Invert(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1027AFB0u))(a0); }
XI_ORIG void Unit1027B060_NegateInPlace(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027B060u))(a0); }
XI_ORIG void Unit1027B060_RotateVec(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027B080u))(a0, a1, a2); }
XI_ORIG void Unit1027B060_Mul(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027B170u))(a0, a1, a2); }
XI_ORIG void FUN_1027b2c0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027B2C0u))(a0, a1, a2, a3); }
XI_ORIG void FUN_1027b480(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027B480u))(a0, a1, a2); }
XI_ORIG undefined4 * SqskSkeleton_Create() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x1027B580u))(); }
XI_ORIG void SqskSkeleton_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027B5A0u))(a0); }
XI_ORIG undefined FUN_1027b610() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1027B610u))(); }
XI_ORIG void SqskSkeleton_SetNodeCount(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027C4C0u))(a0, a1); }
XI_ORIG int SqskSkeleton_FindBoneIndex(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027C900u))(a0, a1); }
XI_ORIG int SqskSkeleton_GetNode(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027C980u))(a0, a1); }
XI_ORIG void SqskSkeleton_Release() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1027C9D0u))(); }
XI_ORIG void SqskSkeleton_Release_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027C9D0u))(a0); }
XI_ORIG void Unit1027CCF0_TransformOrigin(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027CCF0u))(a0, a1, a2); }
XI_ORIG void Unit1027CCF0_ComputeTransform(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027CD30u))(a0, a1, a2, a3); }
XI_ORIG void Unit1027CCF0_TransformPoint(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027CFD0u))(a0, a1, a2, a3); }
XI_ORIG int FUN_1027d110(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027D110u))(a0, a1, a2, a3); }
XI_ORIG void SqmoUtil_MarkSlot(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027D650u))(a0, a1, a2, a3); }
XI_ORIG void Unit1027D980_InitDefaults(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027DAE0u))(a0); }
XI_ORIG void Chn_ReleaseRefs(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027DB80u))(a0); }
XI_ORIG uint32_t * Chn_RegisterItem(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<uint32_t * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027DCC0u))(a0, a1); }
XI_ORIG void Chn_ForwardIfFlag2(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027DD70u))(a0, a1); }
XI_ORIG void FUN_1027dd90(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027DD90u))(a0, a1); }
XI_ORIG int Chn_GetSubIndex(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x1027DF70u))(a0); }
XI_ORIG void Chn_InitDefaults(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027E060u))(a0); }
XI_ORIG void Chn_ReleaseIfSet(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027E0D0u))(a0); }
XI_ORIG void Unit1027E120_UpdateMask(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027E150u))(a0, a1, a2); }
XI_ORIG void Unit1027E120_SetCallback(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027E190u))(a0, a1, a2); }
XI_ORIG undefined4 Unit1027E120_ReleaseConnection(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1027E3B0u))(a0); }
XI_ORIG undefined4 Unit1027E120_IsConnected(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1027E440u))(a0); }
XI_ORIG undefined4 Unit1027E120_UpdateRange(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1027E450u))(a0); }
XI_ORIG void Unit1027E120_Advance(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027E490u))(a0, a1); }
XI_ORIG void Unit1027E120_Forward1027EBD0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027EAF0u))(a0); }
XI_ORIG void FUN_1027eb00() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1027EB00u))(); }
XI_ORIG void Unit1027E120_Zero(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027EB00u))(a0); }
XI_ORIG void M4fSet(void * a0, void * a1) { reinterpret_cast<void (__cdecl *)(void *, void *)>(XI_GAME(0x1027EB10u))(a0, a1); }
XI_ORIG void M4fSet_v1(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027EB10u))(a0, a1); }
XI_ORIG void Unit1027E120_Set(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9, XI_ANY a10, XI_ANY a11, XI_ANY a12, XI_ANY a13, XI_ANY a14, XI_ANY a15, XI_ANY a16) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027EB50u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16); }
XI_ORIG void FUN_1027ebd0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027EBD0u))(a0); }
XI_ORIG float * M4fTransform(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, void *)>(XI_GAME(0x1027EF20u))(a0, a1, a2); }
XI_ORIG void M4fTransform_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027EF20u))(a0, a1, a2); }
XI_ORIG void Unit1027E120_TransformVec3(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027EF80u))(a0, a1, a2); }
XI_ORIG void Mat4x4_TransformVec4(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027EFE0u))(a0, a1, a2); }
XI_ORIG int M4fTranspose(void * a0, void * a1) { return reinterpret_cast<int (__cdecl *)(void *, void *)>(XI_GAME(0x1027F070u))(a0, a1); }
XI_ORIG void M4fTranspose_v1(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027F070u))(a0, a1); }
XI_ORIG int sqMatrix4_1027F0E0(void * a0, int a1) { return reinterpret_cast<int (__cdecl *)(void *, int)>(XI_GAME(0x1027F0E0u))(a0, a1); }
XI_ORIG undefined4 sqMatrix4_1027F0E0_v1(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027F0E0u))(a0, a1); }
XI_ORIG double sqMatrix4_1027FCC0(int a0) { return reinterpret_cast<double (__cdecl *)(int)>(XI_GAME(0x1027FCC0u))(a0); }
XI_ORIG float * sqMatrix4_10280140(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, void *)>(XI_GAME(0x10280140u))(a0, a1, a2); }
XI_ORIG void sqMatrix4_10280140_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10280140u))(a0, a1, a2); }
XI_ORIG undefined4 SqMatrix4_ScaleAxes(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102806B0u))(a0, a1, a2, a3); }
XI_ORIG int * sqgxSprite(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8) { return reinterpret_cast<int * (__cdecl *)(int, int, int, int, int, int, int, int, int)>(XI_GAME(0x10280DE0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG void sqgxSprite_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10280DE0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG void Unit10280C40_CompositeBlendPasses(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102813B0u))(a0, a1); }
XI_ORIG int ___setargv_3() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10281500u))(); }
XI_ORIG int sqgxTextureCreateFromImage(int a0, void * a1) { return reinterpret_cast<int (__cdecl *)(int, void *)>(XI_GAME(0x10281510u))(a0, a1); }
XI_ORIG int sqgxTextureAddZ(int a0, int a1, int a2) { return reinterpret_cast<int (__cdecl *)(int, int, int)>(XI_GAME(0x102816E0u))(a0, a1, a2); }
XI_ORIG int sqgxCeilLog2(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10281810u))(a0); }
XI_ORIG int sqgxTextureAdd(int a0, int a1, void * a2, void * a3) { return reinterpret_cast<int (__cdecl *)(int, int, void *, void *)>(XI_GAME(0x10281830u))(a0, a1, a2, a3); }
XI_ORIG int sqgxTextureAdd_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10281830u))(a0, a1, a2, a3); }
XI_ORIG void SqgxTexture_ClearPaletteUsed(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102819C0u))(a0); }
XI_ORIG int sqgxTextureClutAdd(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102819D0u))(a0, a1, a2); }
XI_ORIG void SqgxTexture_FreeSlot(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10281A70u))(a0); }
XI_ORIG int sqgxTextureGetSize(int a0, void * a1, void * a2) { return reinterpret_cast<int (__cdecl *)(int, void *, void *)>(XI_GAME(0x10281B60u))(a0, a1, a2); }
XI_ORIG void sqgxTextureGetSize_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10281B60u))(a0, a1, a2); }
XI_ORIG _DWORD * sqgxTextureSet(int a0) { return reinterpret_cast<_DWORD * (__cdecl *)(int)>(XI_GAME(0x10281C20u))(a0); }
XI_ORIG void sqgxTextureSet_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10281C20u))(a0); }
XI_ORIG int sqgxTextureDecal(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10281C50u))(a0); }
XI_ORIG void sqgxTextureDecal_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10281C50u))(a0); }
XI_ORIG int sqgxTextureClamp(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10281C80u))(a0); }
XI_ORIG void sqgxTextureClamp_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10281C80u))(a0); }
XI_ORIG void sqgxTextureSetPalette(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10281D60u))(a0); }
XI_ORIG int sqgxTextureUpdate() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10281DB0u))(); }
XI_ORIG void sqgxTextureUpdate_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10281DB0u))(); }
XI_ORIG int sqgxTextureStencilClear(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10282010u))(a0); }
XI_ORIG int sqgxTextureClear(void * a0, int a1) { return reinterpret_cast<int (__cdecl *)(void *, int)>(XI_GAME(0x10282030u))(a0, a1); }
XI_ORIG void sqgxTextureClear_v1(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10282030u))(a0, a1); }
XI_ORIG int * sqgxTextureDraw(int a0, char a1) { return reinterpret_cast<int * (__cdecl *)(int, char)>(XI_GAME(0x10282190u))(a0, a1); }
XI_ORIG undefined4 sqgxTextureDraw_v1(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10282190u))(a0, a1); }
XI_ORIG int * sqgxZTest(int a0) { return reinterpret_cast<int * (__cdecl *)(int)>(XI_GAME(0x10282350u))(a0); }
XI_ORIG void sqgxZTest_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10282350u))(a0); }
XI_ORIG int sqgxAlphaDstTest(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(XI_GAME(0x102823B0u))(a0, a1); }
XI_ORIG void sqgxSetAlphaTest(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102823F0u))(a0, a1); }
XI_ORIG int * sqgxZMask(int a0) { return reinterpret_cast<int * (__cdecl *)(int)>(XI_GAME(0x10282470u))(a0); }
XI_ORIG void sqgxZMask_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10282470u))(a0); }
XI_ORIG int * sqgxFBMask(int a0, int a1, int a2, int a3) { return reinterpret_cast<int * (__cdecl *)(int, int, int, int)>(XI_GAME(0x102824C0u))(a0, a1, a2, a3); }
XI_ORIG void SqgxContext_ResetHandles() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10282590u))(); }
XI_ORIG int Unit102822A0_Nop() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10282630u))(); }
XI_ORIG void sqgxBlendAdd() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x102826C0u))(); }
XI_ORIG void Unit102822A0_SetMultiplyAdd() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x102827D0u))(); }
XI_ORIG void sqgxBlendSub() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10282910u))(); }
XI_ORIG void sqgxBlendLin(float a0) { reinterpret_cast<void (__cdecl *)(float)>(XI_GAME(0x10282A30u))(a0); }
XI_ORIG int sqgxBlendTrsp() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10282B90u))(); }
XI_ORIG void sqgxBlendTrsp_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10282B90u))(); }
XI_ORIG void sqgxBlendAddDestAlpha() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10282DC0u))(); }
XI_ORIG void sqgxBlendZeroSrcAlpha() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10282FF0u))(); }
XI_ORIG int sqgxBlendMul() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10283110u))(); }
XI_ORIG int sqgxBlendOff() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10283220u))(); }
XI_ORIG void sqgxBlendOff_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10283220u))(); }
XI_ORIG int SqgxContext_NoOpA() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10283240u))(); }
XI_ORIG void SqgxContext_NoOpA_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10283240u))(); }
XI_ORIG int ___setargv_9() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10283250u))(); }
XI_ORIG void SqgxContext_SetCurrentIndex(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10283260u))(a0, a1); }
XI_ORIG void SqgxContext_StoreEntryVec(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283280u))(a0); }
XI_ORIG void FUN_10283300(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283300u))(a0); }
XI_ORIG int sqgxColor(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x10283330u))(a0); }
XI_ORIG void sqgxColor_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283330u))(a0); }
XI_ORIG int sqgxAmbient(float a0) { return reinterpret_cast<int (__cdecl *)(float)>(XI_GAME(0x10283380u))(a0); }
XI_ORIG void sqgxAmbient_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283380u))(a0); }
XI_ORIG void SqgxContext_SetFloat76C(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102833A0u))(a0); }
XI_ORIG void SqgxContext_SetDword774AndFactor(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102833C0u))(a0, a1, a2); }
XI_ORIG int sqgxColorUpdate() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10283420u))(); }
XI_ORIG void sqgxColorUpdate_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10283420u))(); }
XI_ORIG int ___setargv_16() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10283870u))(); }
XI_ORIG int sqgxPixelSize(float a0, float a1) { return reinterpret_cast<int (__cdecl *)(float, float)>(XI_GAME(0x102838A0u))(a0, a1); }
XI_ORIG void SqgxContext_SetViewportVec4(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283970u))(a0); }
XI_ORIG int * sqgxViewport(float a0, float a1, float a2, float a3) { return reinterpret_cast<int * (__cdecl *)(float, float, float, float)>(XI_GAME(0x10283A10u))(a0, a1, a2, a3); }
XI_ORIG void sqgxPerspective(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10283C10u))(a0, a1, a2); }
XI_ORIG int sqgxMatrixCamera(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x10283D60u))(a0); }
XI_ORIG void sqgxMatrixCamera_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283D60u))(a0); }
XI_ORIG void SqgxContext_SetTransposedMatrix(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283DA0u))(a0); }
XI_ORIG void SqgxContext_SetFloat72C(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283DE0u))(a0); }
XI_ORIG int sqgxMatrixShape(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x10283E00u))(a0); }
XI_ORIG void sqgxMatrixShape_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283E00u))(a0); }
XI_ORIG void SqgxContext_SetInverseFactor(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283F20u))(a0); }
XI_ORIG int sqgxPassAdd(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10283F70u))(a0); }
XI_ORIG void sqgxPassAdd_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283F70u))(a0); }
XI_ORIG void sqgxPass(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283FC0u))(a0); }
XI_ORIG undefined4 SqgxContext_TestClipBounds(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10284000u))(a0, a1); }
XI_ORIG int sqgxMatrixUpdate() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10284410u))(); }
XI_ORIG int ___setargv_24() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10284890u))(); }
XI_ORIG int sqgxVertexShaderCreate(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x10284950u))(a0); }
XI_ORIG int * sqgxShader(int a0, int a1) { return reinterpret_cast<int * (__cdecl *)(int, int)>(XI_GAME(0x10284B20u))(a0, a1); }
XI_ORIG undefined4 sqgxShader_v1(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10284B20u))(a0, a1); }
XI_ORIG int Unit10284EB0_NoOp() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10284EB0u))(); }
XI_ORIG int j_sqgxVertexShaderSelect() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10284EC0u))(); }
XI_ORIG long double Unit10284EB0_GetFloat20(XI_ANY a0) { return reinterpret_cast<long double (__cdecl *)(XI_ANY)>(XI_GAME(0x10284F20u))(a0); }
XI_ORIG undefined4 SqmoChannel_Evaluate(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10284F50u))(a0, a1); }
XI_ORIG void Unit10284F90_SetValueByType(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10284FD0u))(a0, a1); }
XI_ORIG void FUN_10285000(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10285000u))(a0, a1); }
XI_ORIG undefined4 FUN_10285040(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10285040u))(a0, a1); }
XI_ORIG void SqObject_Release(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10285740u))(a0); }
XI_ORIG undefined4 Unit102857D0_GetFlagByte14(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102857D0u))(a0); }
XI_ORIG unsigned long long FUN_102857e0(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[2] = {a0.bits, a1.bits}; uint32_t ra = a2.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $8, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x102857E0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 Unit102857D0_InvokeCallback18(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102857F0u))(a0); }
XI_ORIG void Unit102857D0_InvokeCallback18_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102857F0u))(a0); }
XI_ORIG void * sqDrawableDraw(int a0) { return reinterpret_cast<void * (__cdecl *)(int)>(XI_GAME(0x10285850u))(a0); }
XI_ORIG void sqDrawableDraw_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10285850u))(a0); }
template <class... R> XI_ORIG void sqDebugPrintf(void * a0, R... rest) { reinterpret_cast<void (__cdecl *)(void *, ...)>(XI_GAME(0x10285890u))(a0, rest...); }
template <class... R> XI_ORIG undefined8 sqDebugPrintf_v1(XI_ANY a0, R... rest) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, decltype(XI_ANY(rest))...)>(XI_GAME(0x10285890u))(a0, XI_ANY(rest)...); }
XI_ORIG int SqmdDMB_Resolve(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10287FB0u))(a0, a1); }
XI_ORIG int SqmdDMB_TraceDelta(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x10288660u))(a0); }
XI_ORIG uint32_t SqmdDMB_AlignMem(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x10288680u))(a0); }
XI_ORIG uint32_t SqmdDMB_ResolveTextures(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102886D0u))(a0, a1); }
XI_ORIG int * SqmdDMB_ResolveShaders(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10288950u))(a0, a1); }
XI_ORIG undefined4 SqmdDMB_TextureLink(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10288B70u))(a0, a1); }
XI_ORIG undefined4 SqmdDMB_ResolveMatrices(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10288BB0u))(a0, a1); }
XI_ORIG char * SqmdDMB_ResolveStrings(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<char * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10288C20u))(a0, a1); }
XI_ORIG void SqmdDMB_ClearTables(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10288DC0u))(a0); }
XI_ORIG void SqmdDMB_ResetCounts(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10288E00u))(a0); }
XI_ORIG uint8_t * FUN_10288e60(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<uint8_t * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10288E60u))(a0, a1); }
XI_ORIG uint8_t * SqmdDMB_BuildUpdaters(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<uint8_t * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10288FF0u))(a0, a1); }
XI_ORIG void Unit10288FF0_ResolveSkelRefs(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102894E0u))(a0, a1, a2); }
XI_ORIG void SqopSystem_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10289620u))(a0); }
XI_ORIG undefined FUN_10289670() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10289670u))(); }
XI_ORIG void SqopSystem_ClearOpcodes(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102896A0u))(a0); }
XI_ORIG void SqopTrack_Reset(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10289B40u))(a0); }
XI_ORIG void SqopTrack_Free(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10289BA0u))(a0); }
XI_ORIG int sqopOpcodeGet(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10289EA0u))(a0, a1); }
XI_ORIG int sqopTrackSetIndex(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10289EF0u))(a0, a1); }
XI_ORIG undefined4 sqopTrackAdvance(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10289F10u))(a0); }
XI_ORIG void SqxfXformM_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10289F50u))(a0); }
XI_ORIG undefined FUN_10289fb0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10289FB0u))(); }
XI_ORIG undefined FUN_1028a040() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1028A040u))(); }
XI_ORIG undefined4 Unit1028A0A0_ReturnZero() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1028A0E0u))(); }
XI_ORIG undefined4 * Unit1028A0A0_ReadBlock(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028A0F0u))(a0, a1, a2, a3, a4); }
XI_ORIG int sqScript_TokenEquals(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1028A150u))(a0, a1); }
XI_ORIG int sub_1028A730(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(XI_GAME(0x1028A730u))(a0, a1); }
XI_ORIG int sub_1028A770(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(XI_GAME(0x1028A770u))(a0, a1); }
XI_ORIG int sub_1028A790(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x1028A790u))(a0); }
XI_ORIG int * sqgxShapeBegin() { return reinterpret_cast<int * (__cdecl *)()>(XI_GAME(0x1028AFE0u))(); }
XI_ORIG int sub_1028BB10(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x1028BB10u))(a0); }
XI_ORIG int sub_1028BB70() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1028BB70u))(); }
XI_ORIG int sub_1028BBD0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1028BBD0u))(); }
XI_ORIG undefined4 * SqcoConnector_Create() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x1028D6D0u))(); }
XI_ORIG void SqcoConnector_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1028D6F0u))(a0); }
XI_ORIG undefined FUN_1028d750() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1028D750u))(); }
XI_ORIG undefined sqcoConnectorPrint() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1028D770u))(); }
XI_ORIG long double Unit1028D970_SumAxis(XI_ANY a0) { return reinterpret_cast<long double (__cdecl *)(XI_ANY)>(XI_GAME(0x1028D990u))(a0); }
XI_ORIG void Unit1028D970_Reset(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1028D9A0u))(a0); }
XI_ORIG void Unit1028D970_Copy(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1028D9C0u))(a0, a1); }
XI_ORIG unsigned long long FUN_1028da20(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[3] = {a0.bits, a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $12, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1028DA20u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Unit1028D970_GetCenter(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1028DBB0u))(a0, a1); }
XI_ORIG undefined4 * SqmdSnap_Create() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x1028DC00u))(); }
XI_ORIG void SqmdSnap_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1028DC20u))(a0); }
XI_ORIG undefined FUN_1028dca0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1028DCA0u))(); }
XI_ORIG undefined FUN_1028e180() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1028E180u))(); }
XI_ORIG void SqsnScene_Free(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1028E1B0u))(a0); }
XI_ORIG void SqsnScene_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1028E350u))(a0); }
XI_ORIG void FUN_1028ec30(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028EC30u))(a0, a1, a2); }
XI_ORIG void SqsnScene_SetItemByte19(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028EC50u))(a0, a1, a2); }
XI_ORIG void SqsnScene_SetByteAt28(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028ECC0u))(a0, a1, a2); }
XI_ORIG void SqsnScene_SetItemByte18(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028ECE0u))(a0, a1, a2); }
XI_ORIG int SqsnScene_AddCamera(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028ED00u))(a0, a1, a2); }
XI_ORIG int SqsnScene_AddLight(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028ED50u))(a0, a1, a2); }
XI_ORIG int SqsnScene_LayerAddItem(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028EDE0u))(a0, a1, a2, a3, a4); }
XI_ORIG void SqsnScene_LayerInit(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1028EFB0u))(a0); }
XI_ORIG int SqsnScene_AddShadow(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1028F130u))(a0, a1); }
XI_ORIG void SqsnScene_SetItemDword28(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028F220u))(a0, a1, a2); }
XI_ORIG void SqsnScene_ItemAddLight(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028F2E0u))(a0, a1, a2); }
XI_ORIG void SqsnScene_ItemAddShadowLight(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028F350u))(a0, a1, a2); }
XI_ORIG void SqsnScene_ShadowAddMember(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028F3C0u))(a0, a1, a2); }
XI_ORIG int SqsnScene_GetEntryAddr(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1028F430u))(a0, a1); }
XI_ORIG void SqsnScene_SetCursorIfInRange(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1028F4A0u))(a0, a1); }
XI_ORIG void SqsnScene_DrawMatrixShape(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1028F4C0u))(a0, a1); }
XI_ORIG int sqsnScene_1028F5F0(int a0, int a1, int a2) { return reinterpret_cast<int (__cdecl *)(int, int, int)>(XI_GAME(0x1028F5F0u))(a0, a1, a2); }
XI_ORIG void sqsnScene_1028F5F0_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028F5F0u))(a0, a1, a2); }
XI_ORIG void SqsnScene_SetupLayerLights(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1028F6E0u))(a0, a1); }
XI_ORIG void sqsnSceneDraw(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1028F8C0u))(a0); }
XI_ORIG int sqsnSceneLayerDraw(void * a0, int a1) { return reinterpret_cast<int (__cdecl *)(void *, int)>(XI_GAME(0x1028FDD0u))(a0, a1); }
XI_ORIG void SqsnScene_DrawFadeSprite(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1028FF80u))(a0); }
XI_ORIG int sqsnSceneLayerDrawShadowed(void * a0, int a1) { return reinterpret_cast<int (__cdecl *)(void *, int)>(XI_GAME(0x1028FFF0u))(a0, a1); }
XI_ORIG int sqsnSceneLayerSnapShotComposite(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(XI_GAME(0x102902C0u))(a0, a1); }
XI_ORIG int SqsnScene_NoOp(_DWORD a0, _DWORD a1) { return reinterpret_cast<int (__cdecl *)(_DWORD, _DWORD)>(XI_GAME(0x10290430u))(a0, a1); }
XI_ORIG int sqsnSceneLayerSort(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(XI_GAME(0x10290440u))(a0, a1); }
XI_ORIG void SqsnScene_Preallocate(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10290610u))(a0, a1, a2, a3, a4); }
XI_ORIG void SqsnScene_LayerPreallocate(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102907C0u))(a0, a1, a2); }
XI_ORIG void SqsnScene_ShadowPreallocate(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10290840u))(a0, a1, a2); }
XI_ORIG undefined FUN_102908e0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102908E0u))(); }
XI_ORIG void SqsdGeneric_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10290910u))(a0); }
XI_ORIG int * sqsdGeneric_BakeOntoShape(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10290FC0u))(a0, a1); }
XI_ORIG undefined FUN_102910e0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102910E0u))(); }
XI_ORIG void SqrdCamera_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10291110u))(a0); }
XI_ORIG undefined FUN_10291170() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10291170u))(); }
XI_ORIG int sqrdCamera_Apply(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x102911E0u))(a0); }
XI_ORIG void sqrdCamera_Apply_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102911E0u))(a0); }
XI_ORIG void Unit10291200_CallHook2C(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10291290u))(a0); }
XI_ORIG void Unit10291200_CallHook3C(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102912D0u))(a0); }
XI_ORIG void Unit10291200_CallHook40(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102912F0u))(a0); }
XI_ORIG undefined4 * SqskJoint_Create() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x10291360u))(); }
XI_ORIG void SqskJoint_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10291390u))(a0); }
XI_ORIG undefined FUN_10291430() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10291430u))(); }
XI_ORIG void SqskJoint_Nop() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10291460u))(); }
XI_ORIG undefined FUN_10291470() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10291470u))(); }
XI_ORIG void Unit102915B0_CopyVec3At5C(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102915B0u))(a0, a1); }
XI_ORIG void Unit102915B0_CopyQuatAt68(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102915E0u))(a0, a1); }
XI_ORIG void Unit102915B0_CopyVec3At78(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102915F0u))(a0, a1); }
XI_ORIG void Unit102915B0_SetTripleAt84(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102916D0u))(a0, a1); }
XI_ORIG void Unit102915B0_SetFloatAt94(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10291780u))(a0, a1); }
XI_ORIG void Unit102915B0_SetFloatAt98(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10291790u))(a0, a1); }
XI_ORIG undefined4 * SqhiNode_Create() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x10291B70u))(); }
XI_ORIG void SqhiNode_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10291BA0u))(a0); }
XI_ORIG undefined FUN_10291c20() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10291C20u))(); }
XI_ORIG void SqhiNode_Nop() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10291C50u))(); }
XI_ORIG undefined FUN_10291c60() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10291C60u))(); }
XI_ORIG void SqhiNode_SetField64(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10291F00u))(a0, a1); }
XI_ORIG void SqhiNode_AppendChild(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10291F10u))(a0, a1); }
XI_ORIG undefined FUN_10291fe0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10291FE0u))(); }
XI_ORIG unsigned long long FUN_10292130(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[3] = {a0.bits, a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $12, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10292130u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void Unit10292470_SwapNibblesXor(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10292490u))(a0, a1); }
XI_ORIG undefined4 * SqmoPlaybackChannel_Create() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x102924F0u))(); }
XI_ORIG void SqmoPlaybackChannel_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10292520u))(a0); }
XI_ORIG undefined FUN_102925a0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102925A0u))(); }
XI_ORIG undefined FUN_10292740() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10292740u))(); }
XI_ORIG void SqmoPlaybackChannel_Update(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10293000u))(a0, a1); }
XI_ORIG undefined FUN_102934c0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102934C0u))(); }
XI_ORIG void Unit10293EE0_SetHandlerByType(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10293F00u))(a0, a1); }
XI_ORIG undefined4 * SqmoFrameChannel_Create() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x10293F80u))(); }
XI_ORIG void SqmoFrameChannel_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10293FA0u))(a0); }
XI_ORIG undefined FUN_10294000() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10294000u))(); }
XI_ORIG undefined sqmoFrameChannel_Dump() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102940A0u))(); }
XI_ORIG void SqmoFrameChannel_Apply(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102944E0u))(a0, a1); }
XI_ORIG undefined4 * SqmoKeyChannel_Create() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x102957B0u))(); }
XI_ORIG void SqmoKeyChannel_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102957D0u))(a0); }
XI_ORIG undefined FUN_10295830() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10295830u))(); }
XI_ORIG undefined sqmoKeyChannel_Dump() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102958B0u))(); }
XI_ORIG undefined FUN_10295a20() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10295A20u))(); }
XI_ORIG undefined4 * SqmoChannelMotion_Create() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x10295E70u))(); }
XI_ORIG void SqmoChannelMotion_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10295E90u))(a0); }
XI_ORIG undefined FUN_10295ef0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10295EF0u))(); }
XI_ORIG undefined FUN_10295f30() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10295F30u))(); }
XI_ORIG undefined4 Unit10296080_CreateWithOwner(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102960A0u))(a0, a1); }
XI_ORIG void Unit102963D0_Set(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10296410u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9); }
XI_ORIG long double SqMatrix3_Determinant(XI_ANY a0) { return reinterpret_cast<long double (__cdecl *)(XI_ANY)>(XI_GAME(0x102967E0u))(a0); }
XI_ORIG void SqgxContext_SetCotHalfAngle(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102972D0u))(a0); }
XI_ORIG int sub_10297320(int a0, int a1, int a2) { return reinterpret_cast<int (__cdecl *)(int, int, int)>(XI_GAME(0x10297320u))(a0, a1, a2); }
XI_ORIG unsigned int sqsnShadowVolumeExtrudeTriList(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, void * a8, float a9, int a10) { return reinterpret_cast<unsigned int (__cdecl *)(int, int, int, int, int, int, int, int, void *, float, int)>(XI_GAME(0x102979E0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }
XI_ORIG int sqsnShadowVolumeExtrudeTriStrip(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, void * a8, float a9, int a10) { return reinterpret_cast<int (__cdecl *)(int, int, int, int, int, int, int, int, void *, float, int)>(XI_GAME(0x10297B10u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }
XI_ORIG int sqsnShadowVolumeSetPass(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10297CC0u))(a0); }
XI_ORIG int sqsnShadowComputeFacing(int a0, void * a1, int a2, int a3, void * a4, int a5, void * a6) { return reinterpret_cast<int (__cdecl *)(int, void *, int, int, void *, int, void *)>(XI_GAME(0x10297F90u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int sqsnShadowVolumeBeginVB() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x102981E0u))(); }
XI_ORIG int sqgxShadowPlaneMatrix(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10298350u))(a0); }
XI_ORIG undefined4 * SqimImage_Create() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x102984B0u))(); }
XI_ORIG undefined FUN_10298540() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10298540u))(); }
XI_ORIG void SqimImage_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10298570u))(a0); }
XI_ORIG undefined4 * SqrdTexture_Create() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x10298B30u))(); }
XI_ORIG undefined FUN_10298b60() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10298B60u))(); }
XI_ORIG void SqrdTexture_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10298B90u))(a0); }
XI_ORIG undefined sqrdTexturePrint() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10298C10u))(); }
XI_ORIG void Texture_BindFromObject(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10298C50u))(a0); }
XI_ORIG int Texture_CreateEntry(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10298E10u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined FUN_10298ee0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10298EE0u))(); }
XI_ORIG int sqsdSampling_BakeOntoShape(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10299250u))(a0, a1); }
XI_ORIG undefined FUN_102992f0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102992F0u))(); }
XI_ORIG int * sqsdAniso_BakeOntoShape(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10299610u))(a0, a1); }
XI_ORIG undefined4 sqsdCartoon_BakeOntoShape_Stub() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10299840u))(); }
XI_ORIG undefined4 SqsdGlow_ApplyShader(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102999A0u))(a0, a1); }
XI_ORIG int * sqsdGlow_BakeOntoShape(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10299AB0u))(a0, a1); }
XI_ORIG undefined4 SqshBuild_BuildSQO(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10299B30u))(a0, a1); }
XI_ORIG int SqshBuild_BuildTriStrip(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10299C30u))(a0, a1); }
XI_ORIG int SqshBuild_Read(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10299D60u))(a0, a1, a2); }
XI_ORIG int SqshBuild_BuildPoints(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10299E40u))(a0, a1); }
XI_ORIG int SqshBuild_BuildCurves(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10299EE0u))(a0, a1); }
XI_ORIG undefined4 * SqinSkin_Create() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x10299FE0u))(); }
XI_ORIG void SqinSkin_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1029A000u))(a0); }
XI_ORIG undefined FUN_1029a060() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1029A060u))(); }
XI_ORIG undefined FUN_1029a0e0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1029A0E0u))(); }
XI_ORIG void SqinSkin_InitShapeData(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1029A160u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void SqinSkin_PropagateValue38(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1029A230u))(a0, a1); }
XI_ORIG undefined4 SqinSkin_ResizeItemArray(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1029A330u))(a0, a1); }
XI_ORIG int Unit1029A380_Append(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1029A380u))(a0, a1); }
XI_ORIG int sqskSkeleton_GetNodeByIndex(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1029A420u))(a0, a1); }
XI_ORIG void SqinSkin_BuildPartShapes(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1029A450u))(a0); }
XI_ORIG void SqinSkin_ReleaseIfHasData(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1029A6F0u))(a0); }
XI_ORIG int SqinBuild_BuildSQO(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1029AA80u))(a0, a1, a2); }
XI_ORIG void FUN_1029abd0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1029ABD0u))(a0, a1, a2, a3); }
XI_ORIG undefined4 SqskSkeleton_BuildSQO(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1029ADF0u))(a0, a1); }
XI_ORIG int SqskSkeleton_BuildBinary(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1029AF20u))(a0, a1, a2); }
XI_ORIG undefined4 * SqmdSort_Create() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x1029B100u))(); }
XI_ORIG void SqmdSort_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1029B120u))(a0); }
XI_ORIG undefined FUN_1029b1a0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1029B1A0u))(); }
XI_ORIG void SqmdSort_AllocLists(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1029B310u))(a0, a1, a2); }
XI_ORIG int SqmdSort_CreateForStrips(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1029B3A0u))(a0, a1, a2, a3); }
XI_ORIG void SqgxDlist_CreateContextList() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1029B5C0u))(); }
XI_ORIG undefined4 * FUN_1029b620() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x1029B620u))(); }
XI_ORIG void SqgxDlist_Nop() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1029BB50u))(); }
XI_ORIG void Sqgx_CopyBlockToContext(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1029BCD0u))(a0); }
XI_ORIG void Sqgx_SetFloat18(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1029BD40u))(a0); }
XI_ORIG int sub_1029BD50(int a0, int a1, void * a2, void * a3) { return reinterpret_cast<int (__cdecl *)(int, int, void *, void *)>(XI_GAME(0x1029BD50u))(a0, a1, a2, a3); }
XI_ORIG int sub_1029BF50(int a0, void * a1, void * a2) { return reinterpret_cast<int (__cdecl *)(int, void *, void *)>(XI_GAME(0x1029BF50u))(a0, a1, a2); }
XI_ORIG undefined4 * FUN_1029c260(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1029C260u))(a0, a1, a2, a3, a4); }
XI_ORIG void Sqgx_SetContextFieldE24(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1029C460u))(a0); }
XI_ORIG float * sub_1029C480(int a0, void * a1, void * a2, int a3, int a4, void * a5, void * a6) { return reinterpret_cast<float * (__cdecl *)(int, void *, void *, int, int, void *, void *)>(XI_GAME(0x1029C480u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG void sub_1029C610(int a0, void * a1, int a2, int a3) { reinterpret_cast<void (__cdecl *)(int, void *, int, int)>(XI_GAME(0x1029C610u))(a0, a1, a2, a3); }
XI_ORIG int sub_1029C950(int a0, int a1, void * a2, void * a3, void * a4, void * a5) { return reinterpret_cast<int (__cdecl *)(int, int, void *, void *, void *, void *)>(XI_GAME(0x1029C950u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void FUN_102a1f20(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102A1F20u))(a0, a1); }
XI_ORIG undefined4 Unit102A1F20_TestSeparationHalf(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102A1FA0u))(a0); }
XI_ORIG undefined4 * SqshDispList_Create() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x102A2000u))(); }
XI_ORIG void SqshDispList_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102A2020u))(a0); }
XI_ORIG undefined FUN_102a2080() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102A2080u))(); }
XI_ORIG undefined sqshDispListPrint() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102A20D0u))(); }
XI_ORIG void Unit102A2130_CopyBounds(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102A2130u))(a0, a1); }
XI_ORIG undefined4 Unit102A2130_ContainsInt(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102A2160u))(a0, a1); }
XI_ORIG undefined4 Unit102A2130_ContainsInAnyEntry(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102A2190u))(a0, a1); }
XI_ORIG void sqsnPlanarShadowDraw(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102A2530u))(a0, a1); }
XI_ORIG void Unit102A2530_RenderShadowTexture(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102A2720u))(a0, a1); }
XI_ORIG void Sqgx_SetTextureAt(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102A2A30u))(a0, a1); }
XI_ORIG void Sqgx_CheckShadowGroupMember(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102A2A50u))(a0, a1, a2); }
XI_ORIG void Unit102A2530_DrawShadowedShape(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102A2C00u))(a0, a1, a2, a3); }
XI_ORIG undefined FUN_102a3360() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102A3360u))(); }
XI_ORIG void SqrdMaterial_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102A3390u))(a0); }
XI_ORIG undefined FUN_102a33f0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102A33F0u))(); }
XI_ORIG void Unit102A34D0_ApplyLighting(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102A34D0u))(a0); }
XI_ORIG void Sqgx_ResetCounter() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x102A3760u))(); }
XI_ORIG void Sqgx_IncCounter() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x102A3770u))(); }
XI_ORIG undefined4 * SqVtx_Create() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x102A9520u))(); }
XI_ORIG void SqVtx_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102A9550u))(a0); }
XI_ORIG undefined FUN_102a95a0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102A95A0u))(); }
XI_ORIG undefined sqVtxPrint() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102A96C0u))(); }
XI_ORIG void SqVtx_AllocateField(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102A9980u))(a0, a1, a2); }
XI_ORIG void SqVtx_ClearEntries(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102A9EF0u))(a0); }
XI_ORIG void SqVtx_ClearEntriesAlt(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102A9F30u))(a0); }
XI_ORIG void SqVtx_PackFloatsToBytes(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102A9FB0u))(a0, a1); }
XI_ORIG undefined4 SqshTriStrip_Create() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x102AA2D0u))(); }
XI_ORIG void SqshTriStrip_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102AA2F0u))(a0); }
XI_ORIG undefined FUN_102aa370() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102AA370u))(); }
XI_ORIG undefined FUN_102aa420() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102AA420u))(); }
XI_ORIG undefined FUN_102aa580() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102AA580u))(); }
XI_ORIG undefined4 SqshTriStrip_GetGlobal() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x102ABEB0u))(); }
XI_ORIG void FUN_102abec0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102ABEC0u))(a0, a1, a2); }
XI_ORIG undefined4 * SqshPoints_Create() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x102ACB60u))(); }
XI_ORIG void SqshPoints_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102ACB80u))(a0); }
XI_ORIG undefined FUN_102acbe0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102ACBE0u))(); }
XI_ORIG undefined sqshPointsPrint() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102ACC30u))(); }
XI_ORIG undefined4 * SqshCurves_Create() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x102ACF20u))(); }
XI_ORIG void SqshCurves_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102ACF40u))(a0); }
XI_ORIG undefined FUN_102acfc0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102ACFC0u))(); }
XI_ORIG undefined sqshCurvesPrint() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102AD090u))(); }
XI_ORIG void SqshCurves_ComputeNormals(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102AD1D0u))(a0, a1); }
XI_ORIG void FUN_102ad770() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x102AD770u))(); }
XI_ORIG void FUN_102ad770_v1(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102AD770u))(a0, a1); }
XI_ORIG void SqshCurves_RebuildIndex(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102AD990u))(a0); }
XI_ORIG void SqinShape_Reset(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102AEC50u))(a0); }
XI_ORIG undefined4 * SqdfShape_Create() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x102AEE90u))(); }
XI_ORIG void SqdfShape_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102AEEB0u))(a0); }
XI_ORIG undefined FUN_102aefe0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102AEFE0u))(); }
XI_ORIG undefined FUN_102af020() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102AF020u))(); }
XI_ORIG void SqdfShape_InitCallbacks(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102AF170u))(a0); }
XI_ORIG undefined FUN_102af2a0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102AF2A0u))(); }
XI_ORIG undefined4 * sqinCluster_Create() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x102AF540u))(); }
XI_ORIG void sqinCluster_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102AF570u))(a0); }
XI_ORIG undefined FUN_102af5e0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102AF5E0u))(); }
XI_ORIG undefined sqinCluster_Dump() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102AF630u))(); }
XI_ORIG BOOL ImmNotifyIME(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<BOOL (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102BC838u))(a0, a1, a2, a3); }
XI_ORIG BOOL ImmGetOpenStatus(XI_ANY a0) { return reinterpret_cast<BOOL (__stdcall *)(XI_ANY)>(XI_GAME(0x102BC85Cu))(a0); }
XI_ORIG BOOL ImmGetConversionStatus(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<BOOL (__stdcall *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102BC862u))(a0, a1, a2); }
XI_ORIG LONG ImmGetCompositionStringA(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<LONG (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102BC868u))(a0, a1, a2, a3); }
XI_ORIG BOOL ImmSetCompositionStringA(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<BOOL (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102BC86Eu))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int Direct3DCreate8(_DWORD a0) { return reinterpret_cast<int (__stdcall *)(_DWORD)>(XI_GAME(0x102D2600u))(a0); }
XI_ORIG int fn_102D2868(void * a0, void * a1, void * a2, int a3, int a4, int a5, int a6, void * a7, int a8, int a9) { return reinterpret_cast<int (__stdcall *)(void *, void *, void *, int, int, int, int, void *, int, int)>(XI_GAME(0x102D2868u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9); }
XI_ORIG int fn_102D2FB9(void * a0, unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, _D3DFORMAT a5, __int32 a6, void * a7) { return reinterpret_cast<int (__stdcall *)(void *, unsigned int, unsigned int, unsigned int, unsigned int, _D3DFORMAT, __int32, void *)>(XI_GAME(0x102D2FB9u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG int fn_102D4515(void * a0, void * a1, void * a2, void * a3, void * a4, void * a5, int a6, int a7) { return reinterpret_cast<int (__stdcall *)(void *, void *, void *, void *, void *, void *, int, int)>(XI_GAME(0x102D4515u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG void FUN_102d7ff9(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102D7FF9u))(a0, a1, a2, a3); }
XI_ORIG float * fn_102D7FF9(void * a0, void * a1, void * a2, void * a3) { return reinterpret_cast<float * (__stdcall *)(void *, void *, void *, void *)>(XI_GAME(0x102D7FF9u))(a0, a1, a2, a3); }
XI_ORIG int D3DXMatrixPerspectiveFovLH_static(int a0, float a1, float a2, float a3, float a4) { return reinterpret_cast<int (__stdcall *)(int, float, float, float, float)>(XI_GAME(0x102D83C6u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 D3DXMatrixPerspectiveFovLH_static_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102D83C6u))(a0, a1, a2, a3, a4); }
XI_ORIG int fn_102DA79B(void * a0, unsigned int a1, void * a2, void * a3, void * a4) { return reinterpret_cast<int (__stdcall *)(void *, unsigned int, void *, void *, void *)>(XI_GAME(0x102DA79Bu))(a0, a1, a2, a3, a4); }
XI_ORIG int fn_102DAA0D(void * a0, char a1, unsigned int a2, void * a3, void * a4, void * a5) { return reinterpret_cast<int (__stdcall *)(void *, char, unsigned int, void *, void *, void *)>(XI_GAME(0x102DAA0Du))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int fn_102DAA71(int a0, void * a1, unsigned int a2) { return reinterpret_cast<int (__stdcall *)(int, void *, unsigned int)>(XI_GAME(0x102DAA71u))(a0, a1, a2); }
XI_ORIG void FUN_1030fed0() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x1030FED0u))(); }
XI_ORIG undefined4 FUN_1030ff33() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x1030FF33u))(); }
XI_ORIG undefined4 FUN_1030ff49() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x1030FF49u))(); }
XI_ORIG undefined4 FUN_1030ff75() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x1030FF75u))(); }
XI_ORIG undefined4 FUN_1030ffa1() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x1030FFA1u))(); }
XI_ORIG undefined4 FUN_1030ffb7() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x1030FFB7u))(); }
XI_ORIG undefined4 FUN_1030ffc2() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x1030FFC2u))(); }
XI_ORIG undefined4 FUN_1030ffcd() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x1030FFCDu))(); }
XI_ORIG undefined4 FUN_1030ffd8() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x1030FFD8u))(); }
XI_ORIG undefined4 FUN_10310030() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x10310030u))(); }
XI_ORIG undefined4 FUN_10310067() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x10310067u))(); }
XI_ORIG undefined4 FUN_10310072() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x10310072u))(); }
XI_ORIG undefined4 FUN_103100eb() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x103100EBu))(); }
XI_ORIG undefined4 FUN_10310353() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x10310353u))(); }
XI_ORIG undefined4 FUN_1031040e() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x1031040Eu))(); }
XI_ORIG undefined4 FUN_10310b30() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x10310B30u))(); }
XI_ORIG undefined4 FUN_10310b3b() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x10310B3Bu))(); }
XI_ORIG undefined4 FUN_10310cd2() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x10310CD2u))(); }
XI_ORIG undefined4 FUN_10310d82() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x10310D82u))(); }
XI_ORIG undefined4 FUN_10310df0() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x10310DF0u))(); }
XI_ORIG undefined4 FUN_10310dfb() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x10310DFBu))(); }
XI_ORIG undefined4 FUN_10310e06() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x10310E06u))(); }
XI_ORIG undefined4 FUN_10310e11() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x10310E11u))(); }
XI_ORIG void FUN_10310e1c() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x10310E1Cu))(); }
XI_ORIG undefined4 FUN_10310e27() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x10310E27u))(); }
XI_ORIG undefined4 FUN_10310e32() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x10310E32u))(); }
XI_ORIG void FUN_1031111e() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x1031111Eu))(); }
XI_ORIG undefined8 FUN_10311344() { return reinterpret_cast<undefined8 (__stdcall *)()>(XI_GAME(0x10311344u))(); }
XI_ORIG undefined4 FUN_1031135a() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x1031135Au))(); }
XI_ORIG undefined4 FUN_10311365() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x10311365u))(); }
XI_ORIG undefined4 FUN_10311370() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x10311370u))(); }
XI_ORIG undefined4 FUN_10311386() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x10311386u))(); }
XI_ORIG void FUN_103113c8() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x103113C8u))(); }
XI_ORIG void FUN_103113d3() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x103113D3u))(); }
XI_ORIG void operator_delete(void * a0) { reinterpret_cast<void (__cdecl *)(void *)>(XI_GAME(0x10311BB0u))(a0); }
XI_ORIG undefined8 operator_delete_v1(XI_ANY a0) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY)>(XI_GAME(0x10311BB0u))(a0); }
XI_ORIG undefined8 operator_delete_v2() { return reinterpret_cast<undefined8 (__cdecl *)()>(XI_GAME(0x10311BB0u))(); }
XI_ORIG void * Allocator_Generic(unsigned int a0) { return reinterpret_cast<void * (__cdecl *)(unsigned int)>(XI_GAME(0x10311BBBu))(a0); }
XI_ORIG undefined8 Allocator_Generic_v1(XI_ANY a0) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY)>(XI_GAME(0x10311BBBu))(a0); }
XI_ORIG undefined8 Allocator_Generic_v2() { return reinterpret_cast<undefined8 (__cdecl *)()>(XI_GAME(0x10311BBBu))(); }
XI_ORIG void * Allocator_Generic_v3(XI_ANY a0) { return reinterpret_cast<void * (__cdecl *)(XI_ANY)>(XI_GAME(0x10311BBBu))(a0); }
XI_ORIG uint32_t FUN_10311c7a(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10311C7Au))(a0, a1); }
XI_ORIG uint32_t * FUN_10311eb0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<uint32_t * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10311EB0u))(a0, a1); }
XI_ORIG undefined4 FUN_10311f30(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10311F30u))(a0); }
XI_ORIG undefined4 FUN_10311fde(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10311FDEu))(a0, a1); }
template <class... R> XI_ORIG undefined8 _sprintf(XI_ANY a0, XI_ANY a1, R... rest) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, decltype(XI_ANY(rest))...)>(XI_GAME(0x10311FF1u))(a0, a1, XI_ANY(rest)...); }
template <class... R> XI_ORIG undefined4 _sprintf_v1(XI_ANY a0, XI_ANY a1, R... rest) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, decltype(XI_ANY(rest))...)>(XI_GAME(0x10311FF1u))(a0, a1, XI_ANY(rest)...); }
XI_ORIG char * _strncpy(void * a0, void * a1, size_t a2) { return reinterpret_cast<char * (__cdecl *)(void *, void *, size_t)>(XI_GAME(0x10312050u))(a0, a1, a2); }
XI_ORIG undefined8 _strncpy_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10312050u))(a0, a1, a2); }
XI_ORIG char * _strncpy_v2(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<char * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10312050u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1031217f(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1031217Fu))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_10312296(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10312296u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_10312487(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10312487u))(a0); }
XI_ORIG char * _strrchr(void * a0, int a1) { return reinterpret_cast<char * (__cdecl *)(void *, int)>(XI_GAME(0x103127C0u))(a0, a1); }
XI_ORIG undefined4 FUN_10312872(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10312872u))(a0); }
XI_ORIG undefined8 _strncmp(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10312940u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10312ad3(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10312AD3u))(a0); }
XI_ORIG undefined4 FUN_10312bdb(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10312BDBu))(a0); }
XI_ORIG undefined8 FUN_10312ead(XI_ANY a0) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY)>(XI_GAME(0x10312EADu))(a0); }
XI_ORIG undefined8 FUN_10312ead_v1() { return reinterpret_cast<undefined8 (__cdecl *)()>(XI_GAME(0x10312EADu))(); }
XI_ORIG void * _malloc_0(size_t a0) { return reinterpret_cast<void * (__cdecl *)(size_t)>(XI_GAME(0x10312F96u))(a0); }
XI_ORIG void * _malloc_0_v1(XI_ANY a0) { return reinterpret_cast<void * (__cdecl *)(XI_ANY)>(XI_GAME(0x10312F96u))(a0); }
XI_ORIG void * _malloc_0_v2() { return reinterpret_cast<void * (__cdecl *)()>(XI_GAME(0x10312F96u))(); }
XI_ORIG int _atexit(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x10313486u))(a0); }
XI_ORIG int _atexit_v1(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x10313486u))(a0); }
template <class... R> XI_ORIG int __snprintf(void * a0, const size_t a1, void * a2, R... rest) { return reinterpret_cast<int (__cdecl *)(void *, const size_t, void *, ...)>(XI_GAME(0x103134C7u))(a0, a1, a2, rest...); }
XI_ORIG char * _strncat(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<char * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10313520u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10313643(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10313643u))(a0, a1, a2); }
XI_ORIG int FUN_10313b74(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x10313B74u))(a0); }
XI_ORIG undefined4 FUN_10313cf0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10313CF0u))(a0, a1, a2); }
XI_ORIG int FUN_1031450a(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1031450Au))(a0, a1); }
XI_ORIG int FUN_103145a4(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x103145A4u))(a0); }
XI_ORIG uint32_t FUN_103146de(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x103146DEu))(a0); }
XI_ORIG uint32_t FUN_1031475c(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x1031475Cu))(a0); }
XI_ORIG uint32_t FUN_10314801(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x10314801u))(a0); }
XI_ORIG uint32_t FUN_1031482f(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x1031482Fu))(a0); }
XI_ORIG undefined4 FUN_10314942(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10314942u))(a0, a1, a2); }
XI_ORIG int _rand() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x103149A0u))(); }
XI_ORIG undefined4 * FUN_103149d0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x103149D0u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10314d55(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10314D55u))(a0, a1, a2); }
XI_ORIG int * FUN_10314d91(XI_ANY a0) { return reinterpret_cast<int * (__cdecl *)(XI_ANY)>(XI_GAME(0x10314D91u))(a0); }
XI_ORIG undefined4 FUN_10314ef1(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10314EF1u))(a0); }
XI_ORIG undefined8 FUN_10315140(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10315140u))(a0, a1); }
XI_ORIG uint32_t * FUN_103151d0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<uint32_t * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x103151D0u))(a0, a1); }
XI_ORIG unsigned long long FUN_103156b3(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) {
    uint32_t xs[4] = {a0.bits, a1.bits, a2.bits, a3.bits}; uint32_t ra = a4.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 12(%%esi)\n\t""pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $16, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x103156B3u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG char * __strlwr(void * a0) { return reinterpret_cast<char * (__cdecl *)(void *)>(XI_GAME(0x10315AA8u))(a0); }
XI_ORIG char * __strlwr_v1(XI_ANY a0) { return reinterpret_cast<char * (__cdecl *)(XI_ANY)>(XI_GAME(0x10315AA8u))(a0); }
XI_ORIG undefined4 * FUN_103169a0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x103169A0u))(a0, a1, a2); }
XI_ORIG unsigned long long FUN_10316d6b(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10316D6Bu) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void * _memset(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<void * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10316D90u))(a0, a1, a2); }
XI_ORIG DWORD * FUN_1031a00d() { return reinterpret_cast<DWORD * (__cdecl *)()>(XI_GAME(0x1031A00Du))(); }
XI_ORIG undefined8 FUN_10325d00(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10325D00u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10325d00_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10325D00u))(a0, a1, a2); }
}  // namespace orig
// a game function used as a value (callback, table entry): its address in the game image
namespace orig_addr {
constexpr uint32_t FUN_10001070 = 0x10001070u;
constexpr uint32_t CApp_Constructor = 0x10001240u;
constexpr uint32_t App_ForceForegroundWindow = 0x100014A0u;
constexpr uint32_t CApp_RunMainLoop = 0x10001520u;
constexpr uint32_t App_PreTranslateInputMessage = 0x100016C0u;
constexpr uint32_t App_RenderFrameMaskedFPU = 0x10001B50u;
constexpr uint32_t App_RenderFrameMaskedFPU_v1 = 0x10001B50u;
constexpr uint32_t App_CreateInputSystem = 0x10001BB0u;
constexpr uint32_t CApp_CopyTextToClipboard = 0x10001CC0u;
constexpr uint32_t Unit100014A0_AppendWithSpace = 0x10001E70u;
constexpr uint32_t Unit100014A0_FindTokenCopy = 0x10001F40u;
constexpr uint32_t Gfx_CopySurfaceRectToTexture = 0x10002040u;
constexpr uint32_t Gfx_ProbeDeviceCaps = 0x10002130u;
constexpr uint32_t Unit100014A0_SwapRedBlue = 0x10002440u;
constexpr uint32_t FUN_10002840 = 0x10002840u;
constexpr uint32_t Gfx_SetGammaRGB = 0x10002C00u;
constexpr uint32_t Unit100014A0_GetFloat10456A68Or1 = 0x10002D80u;
constexpr uint32_t Gfx_SetAspectIndex = 0x10002E60u;
constexpr uint32_t Gfx_ApplyAspectScale = 0x10002E80u;
constexpr uint32_t Gfx_GetAspectTableA = 0x10002EC0u;
constexpr uint32_t Gfx_GetAspectTableB = 0x10002EE0u;
constexpr uint32_t D3DHolder_Constructor = 0x10003070u;
constexpr uint32_t D3DHolder_Initialize = 0x10003110u;
constexpr uint32_t Gfx_TryCreateDeviceForMode = 0x100033C0u;
constexpr uint32_t D3DHolder_CheckDepthFormat = 0x10003460u;
constexpr uint32_t Gfx_CreateDevice = 0x100034B0u;
constexpr uint32_t nullsub_71 = 0x100035A0u;
constexpr uint32_t Gfx_InitObject = 0x100035F0u;
constexpr uint32_t Gfx_ResetObject = 0x10003640u;
constexpr uint32_t sub_10003650 = 0x10003650u;
constexpr uint32_t Debug_Construct = 0x10003660u;
constexpr uint32_t FUN_10003700 = 0x10003700u;
constexpr uint32_t DebugOverlay_Update = 0x10003710u;
constexpr uint32_t FUN_100037d0 = 0x100037D0u;
constexpr uint32_t Unit10003890_DrawActorAxes = 0x10003890u;
constexpr uint32_t Debug_DrawTextTable = 0x10003B60u;
constexpr uint32_t Debug_DrawWorldText = 0x10003D70u;
constexpr uint32_t Input_SetFlagToggle = 0x10005600u;
constexpr uint32_t Input_InitState = 0x10005660u;
constexpr uint32_t FUN_100057d0 = 0x100057D0u;
constexpr uint32_t FUN_100058f0 = 0x100058F0u;
constexpr uint32_t sub_10005A40 = 0x10005A40u;
constexpr uint32_t DirectInput_InstallMouse = 0x10007EF0u;
constexpr uint32_t DirectInput_ReleaseDevice = 0x10008770u;
constexpr uint32_t Input_IsCursorActive = 0x10008830u;
constexpr uint32_t Input_DispatchButtons = 0x100088F0u;
constexpr uint32_t XInput_ProbeDll = 0x10008D90u;
constexpr uint32_t XInput_LoadFunctions = 0x10008E10u;
constexpr uint32_t CDx_Constructor = 0x10008F90u;
constexpr uint32_t CDx_Destructor = 0x10009120u;
constexpr uint32_t CDx_Initialize = 0x100091B0u;
constexpr uint32_t CDx_SetDefaultRenderTargets = 0x10009620u;
constexpr uint32_t CDx_SetClearParams = 0x10009640u;
constexpr uint32_t CDx_SetClearColor = 0x10009670u;
constexpr uint32_t CDx_EndClearBegin = 0x100096B0u;
constexpr uint32_t CDx_EndClearBegin_v1 = 0x100096B0u;
constexpr uint32_t CDx_TestCooperativeLevel = 0x10009710u;
constexpr uint32_t CDx_Present = 0x10009720u;
constexpr uint32_t CDx_BeginScene = 0x10009730u;
constexpr uint32_t CDx_EndScene = 0x10009740u;
constexpr uint32_t SetCursorPos = 0x10009770u;
constexpr uint32_t CDx_ShowCursor = 0x10009780u;
constexpr uint32_t CDx_SetCursorShape = 0x100097F0u;
constexpr uint32_t CDx_SetDefaultRenderState = 0x10009920u;
constexpr uint32_t CDx_AppendStateRecord = 0x10009D20u;
constexpr uint32_t CDx_SetViewport = 0x10009DC0u;
constexpr uint32_t CDx_GetRenderTargetTop = 0x10009E00u;
constexpr uint32_t CDx_GetRenderTargetTop_v1 = 0x10009E00u;
constexpr uint32_t CDx_PushRenderTarget = 0x10009E20u;
constexpr uint32_t CDx_PushRenderTarget_v1 = 0x10009E20u;
constexpr uint32_t CDx_PushRenderTargetTexture_Flag1 = 0x10009EA0u;
constexpr uint32_t CDx_PushRenderTargetTexture_Flag0 = 0x10009EE0u;
constexpr uint32_t CDx_PushRenderTargetTexture = 0x10009F20u;
constexpr uint32_t CDx_PushRenderTargetTexture_v1 = 0x10009F20u;
constexpr uint32_t FUN_1000a0a0 = 0x1000A0A0u;
constexpr uint32_t CDx_SetTexture = 0x1000A1D0u;
constexpr uint32_t CDx_SetTexture_v1 = 0x1000A1D0u;
constexpr uint32_t CDx_PopRenderTargetA = 0x1000A200u;
constexpr uint32_t CDx_PopRenderTargetB = 0x1000A230u;
constexpr uint32_t CDx_PopRenderTargetDefault = 0x1000A290u;
constexpr uint32_t CDx_PopRenderTarget = 0x1000A2B0u;
constexpr uint32_t CDx_PopRenderTarget_v1 = 0x1000A2B0u;
constexpr uint32_t CDx_EndAndClearBegin = 0x1000A390u;
constexpr uint32_t CDx_DrawScreenQuadAtDepth = 0x1000A3B0u;
constexpr uint32_t CDx_DrawTextureToViewport = 0x1000A4F0u;
constexpr uint32_t CDx_DrawTexturedScreenQuadUnit = 0x1000A550u;
constexpr uint32_t CDx_DrawTexturedScreenQuad = 0x1000A5A0u;
constexpr uint32_t CDx_DrawTexturedScreenQuad_v1 = 0x1000A5A0u;
constexpr uint32_t CDx_CreateTexture = 0x1000A6E0u;
constexpr uint32_t CDx_CreateTexture_v1 = 0x1000A6E0u;
constexpr uint32_t CDx_PushRenderTargetRet1 = 0x1000A800u;
constexpr uint32_t CDx_DrawFadeQuad = 0x1000A820u;
constexpr uint32_t Post_DepthBlurComposite = 0x1000AAB0u;
constexpr uint32_t sub_1000ABE0 = 0x1000ABE0u;
constexpr uint32_t CDx_CreateDepthSurfaceLikeMain = 0x1000AD70u;
constexpr uint32_t CDx_StepFadeCounter = 0x1000AE40u;
constexpr uint32_t CDx_NopA = 0x1000BB10u;
constexpr uint32_t CDx_NopB = 0x1000BB20u;
constexpr uint32_t CDx_DrawStereoOddFrameOverlay = 0x1000BB90u;
constexpr uint32_t CDx_SetTransform = 0x1000BD50u;
constexpr uint32_t CDx_SetTransform_v1 = 0x1000BD50u;
constexpr uint32_t CDx_GetTransformCached = 0x1000BDB0u;
constexpr uint32_t CDx_GetTransformCached_v1 = 0x1000BDB0u;
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
constexpr uint32_t CDx_DrawPrimitiveUP_v1 = 0x1000CD00u;
constexpr uint32_t CDx_ApplyStereoEyeProjection = 0x1000CF20u;
constexpr uint32_t CDx_RestoreProjection = 0x1000CF90u;
constexpr uint32_t CAcc_Constructor = 0x1000CFB0u;
constexpr uint32_t Unit1000D0F0_ByteSwap32 = 0x1000D150u;
constexpr uint32_t j_CEnv_UpdateDualTargetMode = 0x1000D3F0u;
constexpr uint32_t CEnv_Nop = 0x1000D620u;
constexpr uint32_t FUN_1000d630 = 0x1000D630u;
constexpr uint32_t CEnv_IsStereoMode = 0x1000D660u;
constexpr uint32_t CEnv_IsStereoMode_v1 = 0x1000D660u;
constexpr uint32_t FUN_1000d680 = 0x1000D680u;
constexpr uint32_t FUN_1000d6b0 = 0x1000D6B0u;
constexpr uint32_t App_RegisterSystemDats = 0x10010370u;
constexpr uint32_t App_SaveAllSlotData = 0x10010520u;
constexpr uint32_t App_SaveAllSlotData_v1 = 0x10010520u;
constexpr uint32_t App_Constructor = 0x10010700u;
constexpr uint32_t App_SetSeqStep = 0x10010CE0u;
constexpr uint32_t App_AdvanceSeqStep = 0x10010D00u;
constexpr uint32_t App_AdvanceSeqStep_v1 = 0x10010D00u;
constexpr uint32_t Unit10010370_IncrementShort6 = 0x10010D10u;
constexpr uint32_t App_InitScene = 0x10010D20u;
constexpr uint32_t Frame_Render = 0x10011A00u;
constexpr uint32_t Post_Afterimage = 0x10012F40u;
constexpr uint32_t Post_FlashAndZoomFeedback = 0x100132B0u;
constexpr uint32_t App_RunOpening = 0x100139E0u;
constexpr uint32_t fn_10013AC0 = 0x10013AC0u;
constexpr uint32_t App_StepGameState5Connect = 0x10013AD0u;
constexpr uint32_t App_StepGameState6 = 0x10013DF0u;
constexpr uint32_t App_StepGameState7 = 0x10013F10u;
constexpr uint32_t App_StepGameState8 = 0x10014030u;
constexpr uint32_t App_StepGameState9 = 0x10014120u;
constexpr uint32_t App_DispatchSubState = 0x10014170u;
constexpr uint32_t Game_InGameSubState0 = 0x100141C0u;
constexpr uint32_t App_SubStateInitCamera = 0x10014200u;
constexpr uint32_t App_InitCamera = 0x10014220u;
constexpr uint32_t App_SubStateAdvance = 0x100142A0u;
constexpr uint32_t CAtelIdleTask_Create = 0x100142B0u;
constexpr uint32_t FUN_10014350 = 0x10014350u;
constexpr uint32_t Unit10014350_EnterState4AfterShutdown = 0x100149B0u;
constexpr uint32_t Unit10014350_TryShutdownActors = 0x10014A20u;
constexpr uint32_t Unit10014350_StepStagedRelease = 0x10014A30u;
constexpr uint32_t Unit10014350_StepMenuOkThenRelease = 0x10014AD0u;
constexpr uint32_t Camera_ReleaseAll = 0x10014C10u;
constexpr uint32_t Unit10014350_ApplyTables103501F8 = 0x10014C60u;
constexpr uint32_t Unit10014350_ShutdownActorsAndTables = 0x10014C90u;
constexpr uint32_t FUN_10014cb0 = 0x10014CB0u;
constexpr uint32_t Unit10014350_ApplyTables10350210 = 0x10014CB0u;
constexpr uint32_t Camera_ClearByte0A = 0x10014CE0u;
constexpr uint32_t Camera_GetFrameScale = 0x10014CF0u;
constexpr uint32_t Camera_GetFrameScale_v1 = 0x10014CF0u;
constexpr uint32_t Camera_GetFrameScale_v2 = 0x10014CF0u;
constexpr uint32_t GetFrameCounter = 0x10014D50u;
constexpr uint32_t GetFrameCounter_v1 = 0x10014D50u;
constexpr uint32_t Camera_GetField38 = 0x10014D60u;
constexpr uint32_t Camera_GetFloat2DC = 0x10014D70u;
constexpr uint32_t Camera_GetFloat2E0 = 0x10014D80u;
constexpr uint32_t Camera_SetClearColorRGB = 0x10014D90u;
constexpr uint32_t Camera_SetClearColorPacked = 0x10014DB0u;
constexpr uint32_t Camera_SetField304BytesOnGlobal = 0x10014E00u;
constexpr uint32_t Camera_SetField304Bytes = 0x10014E20u;
constexpr uint32_t Camera_SetField2FC = 0x10014E60u;
constexpr uint32_t Camera_SetField300 = 0x10014E80u;
constexpr uint32_t Camera_SetField314 = 0x10014EB0u;
constexpr uint32_t Camera_SetField318 = 0x10014ED0u;
constexpr uint32_t Camera_SetViewMatrix = 0x100150D0u;
constexpr uint32_t Camera_Call100151A0OnGlobal = 0x10015100u;
constexpr uint32_t FUN_100151a0 = 0x100151A0u;
constexpr uint32_t Camera_SetMatrix154 = 0x100151F0u;
constexpr uint32_t Camera_GetViewMatrixPtr = 0x10015220u;
constexpr uint32_t Camera_GetViewMatrixPtr_v1 = 0x10015220u;
constexpr uint32_t sub_10015240 = 0x10015240u;
constexpr uint32_t Camera_GetField50 = 0x10015250u;
constexpr uint32_t Camera_GetField50_v1 = 0x10015250u;
constexpr uint32_t Camera_GetMatrix154Ptr = 0x10015270u;
constexpr uint32_t Camera_GetMatrix154Ptr_v1 = 0x10015270u;
constexpr uint32_t Camera_SetField2F8OnGlobal = 0x10015290u;
constexpr uint32_t Camera_SetField2F8 = 0x100152B0u;
constexpr uint32_t Camera_GetField2F4 = 0x100152C0u;
constexpr uint32_t Camera_SetField2E4 = 0x10015360u;
constexpr uint32_t Camera_SetField2F0 = 0x100153A0u;
constexpr uint32_t Camera_SetField2F0_v1 = 0x100153A0u;
constexpr uint32_t FUN_100153d0 = 0x100153D0u;
constexpr uint32_t Camera_BuildPerspectiveFov = 0x10015490u;
constexpr uint32_t Camera_ApplyViewProjection = 0x10015510u;
constexpr uint32_t Camera_GetByte0B = 0x100155D0u;
constexpr uint32_t Camera_GetWord10 = 0x10015600u;
constexpr uint32_t Camera_GetWord12 = 0x10015610u;
constexpr uint32_t Camera_GetViewportWidth = 0x10015680u;
constexpr uint32_t Camera_GetViewportHeight = 0x10015690u;
constexpr uint32_t Camera_PickNearestAspect = 0x100156A0u;
constexpr uint32_t Camera_PickNearestAspect_v1 = 0x100156A0u;
constexpr uint32_t fn_10015740 = 0x10015740u;
constexpr uint32_t App_WinMain = 0x100157E0u;
constexpr uint32_t App_SetGlobal_104569C5 = 0x100163C0u;
constexpr uint32_t App_SetGlobal_104569C6 = 0x100163E0u;
constexpr uint32_t App_SetGlobal_104569C8 = 0x10016400u;
constexpr uint32_t App_SetGlobal_104569C8_v1 = 0x10016400u;
constexpr uint32_t App_GetGlobal_104569C8 = 0x10016410u;
constexpr uint32_t App_GetGlobal_104569C8_v1 = 0x10016410u;
constexpr uint32_t Unit10014350_GetSceneStateByte68 = 0x100164B0u;
constexpr uint32_t App_SetErrorCode = 0x100164C0u;
constexpr uint32_t App_GetGlobalPair = 0x10016560u;
constexpr uint32_t GammaRamp_SetRGB = 0x100170A0u;
constexpr uint32_t GammaRamp_GetRGB = 0x100170D0u;
constexpr uint32_t GammaRamp_GetChannelSamples = 0x10017100u;
constexpr uint32_t GammaRamp_GetChannelSamples_v1 = 0x10017100u;
constexpr uint32_t GammaRamp_BuildRamps = 0x10017160u;
constexpr uint32_t FUN_100171b0 = 0x100171B0u;
constexpr uint32_t GammaRamp_ApplyIfChanged = 0x10017230u;
constexpr uint32_t setPolUrl = 0x10017630u;
constexpr uint32_t CShader_Destructor = 0x100177A0u;
constexpr uint32_t fn_10017BB0 = 0x10017BB0u;
constexpr uint32_t Unit10017C10_Step = 0x10017C20u;
constexpr uint32_t Unit10017C10_Begin = 0x10017C60u;
constexpr uint32_t Unit10017C10_NoOpVarargsA = 0x10017D20u;
constexpr uint32_t Unit10017C10_NoOpVarargsA_v1 = 0x10017D20u;
constexpr uint32_t Unit10017C10_NoOpVarargsB = 0x10017D30u;
constexpr uint32_t Unit10017C10_NoOpVarargsB_v1 = 0x10017D30u;
constexpr uint32_t nullsub_90 = 0x10017D50u;
constexpr uint32_t nullsub_90_v1 = 0x10017D50u;
constexpr uint32_t FUN_100181d0 = 0x100181D0u;
constexpr uint32_t Unit10017C10_Call10114F10NoArgs = 0x100182C0u;
constexpr uint32_t _exit_0 = 0x100182E0u;
constexpr uint32_t _exit_0_v1 = 0x100182E0u;
constexpr uint32_t _exit_0_v2 = 0x100182E0u;
constexpr uint32_t Unit10017C10_Thunk10114F10 = 0x10018300u;
constexpr uint32_t FUN_10018310 = 0x10018310u;
constexpr uint32_t Unit10017C10_CtorVt10329F88 = 0x10018350u;
constexpr uint32_t Unit10017C10_CtorVt10329F8C = 0x10018430u;
constexpr uint32_t Unit10017C10_CtorVt10329F90 = 0x10018550u;
constexpr uint32_t Unit10017C10_LoadChecksum = 0x100185A0u;
constexpr uint32_t CXiOpening_Constructor = 0x100186C0u;
constexpr uint32_t Unit10018840_IsStateFour = 0x10018840u;
constexpr uint32_t sub_10018850 = 0x10018850u;
constexpr uint32_t OpeningSubtitle_Constructor = 0x10018A50u;
constexpr uint32_t OpeningSubtitle_Reset = 0x10018B30u;
constexpr uint32_t OpeningSubtitle_Reset_v1 = 0x10018B30u;
constexpr uint32_t Vt10329FE0_Constructor = 0x10018C50u;
constexpr uint32_t Vt10329FE0_Reset = 0x10018CE0u;
constexpr uint32_t Vt10329FE0_Reset_v1 = 0x10018CE0u;
constexpr uint32_t App_BeginTimerPeriod = 0x10019140u;
constexpr uint32_t sub_100191B0 = 0x100191B0u;
constexpr uint32_t Unit10019140_CreateObject = 0x100191D0u;
constexpr uint32_t CXiTimer_Constructor = 0x10019210u;
constexpr uint32_t CXiTimerLow_Init = 0x10019490u;
constexpr uint32_t FUN_100194f0 = 0x100194F0u;
constexpr uint32_t FUN_10019a50 = 0x10019A50u;
constexpr uint32_t CYyMotionQue_SetSlotMotion = 0x1001A660u;
constexpr uint32_t CYyMotionQue_SetSlotMotion_v1 = 0x1001A660u;
constexpr uint32_t MotionQueList_QueueMotion = 0x1001A6E0u;
constexpr uint32_t CYyMotionQueList_Init = 0x1001ABF0u;
constexpr uint32_t FUN_1001ac30 = 0x1001AC30u;
constexpr uint32_t FUN_1001ac40 = 0x1001AC40u;
constexpr uint32_t CYyMotionQue_AppendSync = 0x1001ACB0u;
constexpr uint32_t KzMotionQueList_AppendSync = 0x1001AD00u;
constexpr uint32_t FUN_1001b0f0 = 0x1001B0F0u;
constexpr uint32_t FUN_1001b140 = 0x1001B140u;
constexpr uint32_t KzMotionQueList_AdvanceAll = 0x1001B170u;
constexpr uint32_t FUN_1001b1a0 = 0x1001B1A0u;
constexpr uint32_t KzMotionQueList_NotifyAll = 0x1001B1F0u;
constexpr uint32_t CYyMotionQue_SampleInto = 0x1001B230u;
constexpr uint32_t CYyMotionQue_Update = 0x1001B2E0u;
constexpr uint32_t CYyMotionQue_IsActive = 0x1001B340u;
constexpr uint32_t FUN_1001b360 = 0x1001B360u;
constexpr uint32_t FUN_1001b520 = 0x1001B520u;
constexpr uint32_t Unit1001B510_Update = 0x1001B5D0u;
constexpr uint32_t FUN_1001b710 = 0x1001B710u;
constexpr uint32_t Model_CreateTranslucentTargets = 0x1001D0A0u;
constexpr uint32_t FUN_1001d110 = 0x1001D110u;
constexpr uint32_t Unit1001B890_Construct = 0x1001D170u;
constexpr uint32_t FUN_1001d1f0 = 0x1001D1F0u;
constexpr uint32_t Model_BeginTranslucentPass = 0x1001D240u;
constexpr uint32_t Model_TranslucentUnbindTexture = 0x1001D350u;
constexpr uint32_t Unit1001B890_SetColorForce = 0x1001D390u;
constexpr uint32_t Model_CompositeTranslucentPass = 0x1001D3B0u;
constexpr uint32_t FUN_1001d8e0 = 0x1001D8E0u;
constexpr uint32_t Gfx_DrawScreenQuad256 = 0x1001DD00u;
constexpr uint32_t Tex_HeightToNormalMap = 0x1001E0A0u;
constexpr uint32_t CYyCamMng2_SetGlobalFlags = 0x1001E2F0u;
constexpr uint32_t CYyCamMng2_CreateGlobal = 0x1001E470u;
constexpr uint32_t CYyCamMng2_GetYaw = 0x1001E4D0u;
constexpr uint32_t CYyCamMng2_Reset = 0x1001E4F0u;
constexpr uint32_t CYyCamMng2_SetDefaultDistances = 0x1001E590u;
constexpr uint32_t CYyCamMng2_Construct = 0x1001E600u;
constexpr uint32_t Camera_SetPosition = 0x1001E760u;
constexpr uint32_t Camera_SetTargetIfMoved = 0x1001E780u;
constexpr uint32_t YmCameraManager_Follow = 0x1001EE60u;
constexpr uint32_t FUN_10021110 = 0x10021110u;
constexpr uint32_t FUN_10021680 = 0x10021680u;
constexpr uint32_t Camera_BuildLookAtView = 0x10021C10u;
constexpr uint32_t Actor_ClearIndexTable = 0x100220E0u;
constexpr uint32_t Actor_MapPercentScaleIndex = 0x10022170u;
constexpr uint32_t Actor_InitIndexTable = 0x10022230u;
constexpr uint32_t CYyModelDt_PrepareRenderState = 0x100224D0u;
constexpr uint32_t CYyModelDt_PrepareRenderStateSimple = 0x100231C0u;
constexpr uint32_t CYyModelDt_Construct = 0x10025010u;
constexpr uint32_t CYyModelDt_Destructor = 0x100250C0u;
constexpr uint32_t CYyModelBase_AllocParts = 0x100250F0u;
constexpr uint32_t Unit10025220_Release = 0x10025220u;
constexpr uint32_t Unit10025220_Init = 0x10025260u;
constexpr uint32_t ProjTex_Destructor = 0x100252F0u;
constexpr uint32_t ProjTex_Destructor_v1 = 0x100252F0u;
constexpr uint32_t ProjTex_BeginPass = 0x10025330u;
constexpr uint32_t ProjTex_EndPass = 0x100255C0u;
constexpr uint32_t TexObj_GetD3DTexture = 0x10025860u;
constexpr uint32_t ProjTex_CaptureToTarget = 0x10025870u;
constexpr uint32_t Unit100252F0_Nop_10025BD0 = 0x10025BD0u;
constexpr uint32_t Unit100252F0_Nop_10025BE0 = 0x10025BE0u;
constexpr uint32_t Unit100252F0_ClearCounter = 0x10025BF0u;
constexpr uint32_t FUN_10025c30 = 0x10025C30u;
constexpr uint32_t EnvMap_BeginPass = 0x10025C40u;
constexpr uint32_t EnvMap_EndPass = 0x10025F70u;
constexpr uint32_t sub_10026210 = 0x10026210u;
constexpr uint32_t CYyIcon_Constructor = 0x10026890u;
constexpr uint32_t CXiMovie_Draw = 0x10026AD0u;
constexpr uint32_t CYyIcon_DrawSubRect = 0x10026C70u;
constexpr uint32_t Vec3_SetZero = 0x10026E00u;
constexpr uint32_t Quat_SetIdentity = 0x10026E10u;
constexpr uint32_t Vec4_Set = 0x10026E30u;
constexpr uint32_t Vec4_Set_v1 = 0x10026E30u;
constexpr uint32_t Vec3_Set = 0x10026E50u;
constexpr uint32_t Vec4_Copy = 0x10026E90u;
constexpr uint32_t Vec4_Copy_v1 = 0x10026E90u;
constexpr uint32_t Vec3_Copy = 0x10026EB0u;
constexpr uint32_t Vec3_Copy_v1 = 0x10026EB0u;
constexpr uint32_t Vec3_AddInPlace = 0x10026F20u;
constexpr uint32_t Vec3_AddInPlace_v1 = 0x10026F20u;
constexpr uint32_t sub_10026F50 = 0x10026F50u;
constexpr uint32_t Vec3_Add_10026FA0 = 0x10026FA0u;
constexpr uint32_t Vec3_Add_10026FA0_v1 = 0x10026FA0u;
constexpr uint32_t Vec2_Add = 0x10026FD0u;
constexpr uint32_t sub_10027020 = 0x10027020u;
constexpr uint32_t Vec2_AddScalar = 0x10027050u;
constexpr uint32_t Vec3_SubInPlace = 0x100270A0u;
constexpr uint32_t Vec3_Sub = 0x10027120u;
constexpr uint32_t Vec3_Sub_v1 = 0x10027120u;
constexpr uint32_t Vec3_ScaleInPlace = 0x100272B0u;
constexpr uint32_t fn_100272B0 = 0x100272B0u;
constexpr uint32_t Vec3_Scale = 0x100272E0u;
constexpr uint32_t Vec3_Scale_v1 = 0x100272E0u;
constexpr uint32_t Vec3_MulInPlace = 0x10027310u;
constexpr uint32_t Vec3_MulInPlace_v1 = 0x10027310u;
constexpr uint32_t Vec3_MulComponents = 0x10027340u;
constexpr uint32_t Vec3_MulComponents_v1 = 0x10027340u;
constexpr uint32_t FUN_100274b0 = 0x100274B0u;
constexpr uint32_t sub_100274B0 = 0x100274B0u;
constexpr uint32_t Vec3_CopyThen100274B0 = 0x10027510u;
constexpr uint32_t FUN_10027530 = 0x10027530u;
constexpr uint32_t FUN_100275d0 = 0x100275D0u;
constexpr uint32_t FUN_10027600 = 0x10027600u;
constexpr uint32_t Vec3_Call100275D0 = 0x10027620u;
constexpr uint32_t Vec3_Lerp = 0x100276A0u;
constexpr uint32_t Gfx_ProjectWorldToClip = 0x100276F0u;
constexpr uint32_t Gfx_ProjectWorldToClip_v1 = 0x100276F0u;
constexpr uint32_t Gfx_NdcToScreen = 0x100278E0u;
constexpr uint32_t Gfx_NdcToScreen_v1 = 0x100278E0u;
constexpr uint32_t Unit10026C70_ReturnThis = 0x10027990u;
constexpr uint32_t fn_10027990 = 0x10027990u;
constexpr uint32_t Unit10026C70_Nop = 0x100279A0u;
constexpr uint32_t Mat4_SetIdentity = 0x100279B0u;
constexpr uint32_t Mat4_SetIdentity_v1 = 0x100279B0u;
constexpr uint32_t Mat4_Copy = 0x100279F0u;
constexpr uint32_t Mat4_Copy_v1 = 0x100279F0u;
constexpr uint32_t Mat4_BuildScale = 0x10027AE0u;
constexpr uint32_t Mat4_SetScale = 0x10027B30u;
constexpr uint32_t Mat4_SetScale_v1 = 0x10027B30u;
constexpr uint32_t Mat4_PostRotateX = 0x10027B80u;
constexpr uint32_t Mat4_PostRotateX_v1 = 0x10027B80u;
constexpr uint32_t Mat4_PostRotateY = 0x10027BD0u;
constexpr uint32_t Mat4_PostRotateY_v1 = 0x10027BD0u;
constexpr uint32_t Mat4_PostRotateZ = 0x10027C20u;
constexpr uint32_t Mat4_PostRotateZ_v1 = 0x10027C20u;
constexpr uint32_t Mat4_SetRow3 = 0x10027CD0u;
constexpr uint32_t Mat4_SetRow3_v1 = 0x10027CD0u;
constexpr uint32_t Mat4_SetTranslation = 0x10027CF0u;
constexpr uint32_t Mat4_SetTranslation_v1 = 0x10027CF0u;
constexpr uint32_t Mat4_PostMultiply = 0x10027D10u;
constexpr uint32_t Mat4_PostMultiply_v1 = 0x10027D10u;
constexpr uint32_t Mat4_PreMultiply = 0x10027E90u;
constexpr uint32_t Mat4_PreMultiply_v1 = 0x10027E90u;
constexpr uint32_t sub_10027F60 = 0x10027F60u;
constexpr uint32_t Mat4_TransformVec4InPlace = 0x10028140u;
constexpr uint32_t Mat4_TransformVec4InPlace_v1 = 0x10028140u;
constexpr uint32_t Mat4_TransformVec4 = 0x10028170u;
constexpr uint32_t Mat4_TransformPointInPlace = 0x10028200u;
constexpr uint32_t Mat4_TransformPointInPlace_v1 = 0x10028200u;
constexpr uint32_t Mat4_TransformPoint = 0x10028230u;
constexpr uint32_t Mat4_TransformNormalInPlace = 0x10028290u;
constexpr uint32_t Mat4_TransformNormalInPlace_v1 = 0x10028290u;
constexpr uint32_t Mat4_TransformNormal = 0x100282C0u;
constexpr uint32_t Mat4_TransformNormal_v1 = 0x100282C0u;
constexpr uint32_t Mat4_TransformCoordInPlace = 0x10028320u;
constexpr uint32_t Mat4_TransformCoordInPlace_v1 = 0x10028320u;
constexpr uint32_t Mat4_InvertOrthonormal = 0x100283C0u;
constexpr uint32_t Mat4_InvertOrthonormal_v1 = 0x100283C0u;
constexpr uint32_t Mem_AllocObject = 0x10028640u;
constexpr uint32_t Mem_AllocObject_v1 = 0x10028640u;
constexpr uint32_t Mem_AllocWithFallback = 0x100286B0u;
constexpr uint32_t Mem_AllocWithFallback_v1 = 0x100286B0u;
constexpr uint32_t Mem_FreeObject = 0x10028700u;
constexpr uint32_t Mem_FreeObject_v1 = 0x10028700u;
constexpr uint32_t SharedDeleteHelper = 0x10028710u;
constexpr uint32_t SharedDeleteHelper_v1 = 0x10028710u;
constexpr uint32_t FUN_10028730 = 0x10028730u;
constexpr uint32_t MemBlock_AllocFromFront = 0x10028850u;
constexpr uint32_t MemBlock_AllocFromBack = 0x10028930u;
constexpr uint32_t FUN_100289f0 = 0x100289F0u;
constexpr uint32_t FUN_100289f0_v1 = 0x100289F0u;
constexpr uint32_t FUN_10028aa0 = 0x10028AA0u;
constexpr uint32_t MemBlock_GetSize = 0x10028B90u;
constexpr uint32_t MemBlock_GetSize_v1 = 0x10028B90u;
constexpr uint32_t Unit10028A50_CallEnabledEntries = 0x10028BF0u;
constexpr uint32_t Unit10028A50_CallEnabledList0 = 0x10028C30u;
constexpr uint32_t Unit10028A50_CallEnabledList1 = 0x10028C50u;
constexpr uint32_t Unit10028A50_CallEnabledList2 = 0x10028C70u;
constexpr uint32_t Unit10028A50_CallEnabledList3 = 0x10028C90u;
constexpr uint32_t Mem_AllocFromArea = 0x10028FD0u;
constexpr uint32_t Mem_AllocFromArea_v1 = 0x10028FD0u;
constexpr uint32_t Mem_AllocFromArea_v2 = 0x10028FD0u;
constexpr uint32_t Unit10028FD0_CallListByKind = 0x10029150u;
constexpr uint32_t Unit10029260_ApplyFourTables = 0x10029390u;
constexpr uint32_t CreateSharedVertexBuffers = 0x10029A50u;
constexpr uint32_t Unit100298D0_ReleaseAll = 0x10029B60u;
constexpr uint32_t CYyModelDt_InitBase = 0x10029BB0u;
constexpr uint32_t FUN_10029c00 = 0x10029C00u;
constexpr uint32_t CYyModelDt_Init = 0x10029C90u;
constexpr uint32_t CYyModelDtBase_Destructor = 0x10029CD0u;
constexpr uint32_t CYyModelBase_Construct = 0x10029D30u;
constexpr uint32_t CYyModelBase_Destructor = 0x10029DD0u;
constexpr uint32_t CYyModelBase_GetPartList = 0x10029E70u;
constexpr uint32_t CYyModelBase_GetPartList_v1 = 0x10029E70u;
constexpr uint32_t Unit10029E50_FindTailLink = 0x10029EB0u;
constexpr uint32_t Unit10029E50_AppendAtTail = 0x10029F00u;
constexpr uint32_t CYyModelBase_AddPartFromResource = 0x10029F70u;
constexpr uint32_t CYyModelBase_RefreshParts = 0x1002A0C0u;
constexpr uint32_t FUN_1002a130 = 0x1002A130u;
constexpr uint32_t Model_AnimateAndPose = 0x1002A140u;
constexpr uint32_t CYyModelBase_DrawLit = 0x1002A3B0u;
constexpr uint32_t FUN_1002a700 = 0x1002A700u;
constexpr uint32_t CYyModelBase_TransformVec4ByPart = 0x1002A750u;
constexpr uint32_t Unit1002A0C0_ApplyToVec4 = 0x1002A780u;
constexpr uint32_t Part_CopyTranslation = 0x1002A890u;
constexpr uint32_t Part_AddTranslation = 0x1002A8B0u;
constexpr uint32_t CYyModelBase_FindPartIndex = 0x1002A9B0u;
constexpr uint32_t FUN_1002a9e0 = 0x1002A9E0u;
constexpr uint32_t CYyModelBase_ForwardToField24 = 0x1002AC50u;
constexpr uint32_t FUN_1002b3f0 = 0x1002B3F0u;
constexpr uint32_t FUN_1002b470 = 0x1002B470u;
constexpr uint32_t CYyModel_Constructor = 0x1002B4B0u;
constexpr uint32_t CYyModel_Destructor = 0x1002B550u;
constexpr uint32_t CYyModel_GetField44Addr = 0x1002B5A0u;
constexpr uint32_t CYyModel_GetField44Addr_v1 = 0x1002B5A0u;
constexpr uint32_t CYyModel_TailLink = 0x1002B5E0u;
constexpr uint32_t CYyModel_AppendBase = 0x1002B630u;
constexpr uint32_t CXiSkeletonActor_DrawModel = 0x1002B6E0u;
constexpr uint32_t CXiSkeletonActor_AnimateModels = 0x1002C2D0u;
constexpr uint32_t CXiSkeletonActor_PrepareModelsOnce = 0x1002C360u;
constexpr uint32_t CYyModel_GetPartOffset = 0x1002C400u;
constexpr uint32_t CYyModel_QueryPartsA = 0x1002C710u;
constexpr uint32_t CYyModel_QueryPartsB = 0x1002C750u;
constexpr uint32_t List_GetTail = 0x1002C7C0u;
constexpr uint32_t List_GetTail_v1 = 0x1002C7C0u;
constexpr uint32_t FUN_1002c7e0 = 0x1002C7E0u;
constexpr uint32_t Unit1002C7C0_PopHead = 0x1002C800u;
constexpr uint32_t Mem_AllocDefault = 0x1002C890u;
constexpr uint32_t Mem_AllocDefault_v1 = 0x1002C890u;
constexpr uint32_t thunk_sub_10028AA0 = 0x1002C8B0u;
constexpr uint32_t thunk_sub_10028AA0_v1 = 0x1002C8B0u;
constexpr uint32_t thunk_sub_10028AA0_v2 = 0x1002C8B0u;
constexpr uint32_t thunk_sub_10028AA0_v3 = 0x1002C8B0u;
constexpr uint32_t CYyObject_IsKindOf = 0x1002C8F0u;
constexpr uint32_t CYyModelDt_SkinAndDrawV2 = 0x1002DC10u;
constexpr uint32_t Gfx_SetStreamSourceCached = 0x1002F130u;
constexpr uint32_t Gfx_ApplyCachedStreamSource = 0x1002F160u;
constexpr uint32_t Gfx_ApplyCachedVertexShader = 0x1002F1A0u;
constexpr uint32_t Gfx_SetVertexShaderCached = 0x1002F1C0u;
constexpr uint32_t Model_SkinVerticesCpu = 0x1002F1D0u;
constexpr uint32_t FUN_100309b0 = 0x100309B0u;
constexpr uint32_t FUN_10030a40 = 0x10030A40u;
constexpr uint32_t FUN_10030fb0 = 0x10030FB0u;
constexpr uint32_t Quat_SetIdentityChained = 0x10032A40u;
constexpr uint32_t Quat_SetIdentityB = 0x10032A50u;
constexpr uint32_t Unit100310A0_Nop = 0x10032A70u;
constexpr uint32_t Quat_Multiply = 0x10032B50u;
constexpr uint32_t Quat_NormalizeInPlace = 0x10032DA0u;
constexpr uint32_t BoneXform_PostMultiplyInto = 0x10032FA0u;
constexpr uint32_t Unit100310A0_FromQuat = 0x10032FE0u;
constexpr uint32_t Quat_NLerp = 0x10033220u;
constexpr uint32_t FUN_10033360 = 0x10033360u;
constexpr uint32_t Unit100310A0_InitTargets = 0x100336B0u;
constexpr uint32_t Unit100310A0_ReleaseTargets = 0x100336D0u;
constexpr uint32_t Unit100310A0_Construct = 0x10033710u;
constexpr uint32_t FUN_100337c0 = 0x100337C0u;
constexpr uint32_t Model_BeginOffscreenPass = 0x10033800u;
constexpr uint32_t Model_EndOffscreenPassBindTextures = 0x10033A70u;
constexpr uint32_t Shadow_DrawByMode = 0x10033DA0u;
constexpr uint32_t Shadow_SmoothLightDirection = 0x10033EA0u;
constexpr uint32_t CYySkl_Construct = 0x10034230u;
constexpr uint32_t FUN_10034270 = 0x10034270u;
constexpr uint32_t CYyModelBase_AllocMatrixBuffers = 0x100342A0u;
constexpr uint32_t Skel_GetRecordAt = 0x100351F0u;
constexpr uint32_t Skel_GetBoneTableBase = 0x10035210u;
constexpr uint32_t Skel_GetTableAfterBones = 0x10035250u;
constexpr uint32_t Skel_GetTableAfterBones_v1 = 0x10035250u;
constexpr uint32_t Skel_ClearMarks = 0x100352A0u;
constexpr uint32_t Skel_MarkMarkedChains = 0x100352F0u;
constexpr uint32_t FUN_10035330 = 0x10035330u;
constexpr uint32_t Skel_MarkChain = 0x10035330u;
constexpr uint32_t CMoTask_CtorByMode = 0x10035A00u;
constexpr uint32_t CMoTask_VT_1032A904_Destructor = 0x10035B00u;
constexpr uint32_t MoTask_KillAllGlobalSlots = 0x10035F00u;
constexpr uint32_t Unit10035B00_ClearGlobal10463930 = 0x10035F80u;
constexpr uint32_t Unit10035B00_SetScaled10351708 = 0x10035FB0u;
constexpr uint32_t FUN_10036020 = 0x10036020u;
constexpr uint32_t MoTask_KillGlobalSlot1 = 0x10036070u;
constexpr uint32_t Unit10035B00_ClearGlobal10463934 = 0x100360C0u;
constexpr uint32_t Music_ApplyVolume = 0x100360F0u;
constexpr uint32_t Unit10035B00_SetCurrent10463914 = 0x10036170u;
constexpr uint32_t Unit10035B00_ResetGlobals = 0x100361D0u;
constexpr uint32_t CYySoundElem_UpdateQueue = 0x10036230u;
constexpr uint32_t CYySoundElem_QueueIfAbsent = 0x10036360u;
constexpr uint32_t FUN_100363b0 = 0x100363B0u;
constexpr uint32_t CYySoundElem_ClearHandle = 0x10036420u;
constexpr uint32_t Unit10035B00_AddUniqueId = 0x10036460u;
constexpr uint32_t CYySoundElem_UpdateOne = 0x100364A0u;
constexpr uint32_t CYySoundElem_UpdateTimerAndPosition = 0x100364E0u;
constexpr uint32_t CYySoundElem_IsTimerPositive = 0x100367C0u;
constexpr uint32_t CYySoundElem_IsHandleInFrameList = 0x100367F0u;
constexpr uint32_t CYySoundElem_FindInQueue = 0x10036830u;
constexpr uint32_t CYySoundElem_AppendToQueue = 0x10036870u;
constexpr uint32_t CYySoundElem_PushActive = 0x10036980u;
constexpr uint32_t CYySoundElem_Constructor = 0x100369F0u;
constexpr uint32_t Unit10036B90_SendStopToLobby = 0x10036B90u;
constexpr uint32_t FUN_10036c70 = 0x10036C70u;
constexpr uint32_t Unit10036B90_SetVolumeScalar = 0x10036F00u;
constexpr uint32_t Unit10036B90_SetVolumeVector = 0x10036F50u;
constexpr uint32_t Unit10036B90_CreateSoundElem = 0x10037020u;
constexpr uint32_t FUN_10037960 = 0x10037960u;
constexpr uint32_t SetMusicTrackVolume_C = 0x10037B30u;
constexpr uint32_t SetMusicTrackVolume_A = 0x10037B90u;
constexpr uint32_t SetMusicTrackVolume_B = 0x10037BF0u;
constexpr uint32_t SetMusicTrackVolume_B_v1 = 0x10037BF0u;
constexpr uint32_t YmSePlayBeep = 0x10037C50u;
constexpr uint32_t MusicTrack_SetVolumeId0009 = 0x10037D70u;
constexpr uint32_t MusicTrack_SetVolumeId0010 = 0x10037DD0u;
constexpr uint32_t MusicTrack_SetVolumeId0011 = 0x10037E30u;
constexpr uint32_t MusicTrack_SetVolumeId0013 = 0x10037E90u;
constexpr uint32_t FUN_10037ef0 = 0x10037EF0u;
constexpr uint32_t MusicTrack_SetVolumeId0016 = 0x10037FB0u;
constexpr uint32_t Unit10037AC0_Create = 0x100380F0u;
constexpr uint32_t FUN_10038330 = 0x10038330u;
constexpr uint32_t FUN_10038440 = 0x10038440u;
constexpr uint32_t Music_SetTrackVolume = 0x10038470u;
constexpr uint32_t Font_QueueText = 0x100384B0u;
constexpr uint32_t Font_DrawPass = 0x10038520u;
constexpr uint32_t Font_DrawString = 0x10038720u;
constexpr uint32_t Post_CreateScreenEffectTargets = 0x10038930u;
constexpr uint32_t Unit10038930_ReleaseTextures = 0x100389C0u;
constexpr uint32_t Unit10038930_Init = 0x10038A20u;
constexpr uint32_t FUN_10038a60 = 0x10038A60u;
constexpr uint32_t sub_10038A70 = 0x10038A70u;
constexpr uint32_t Post_CaptureSceneToTarget = 0x10038A80u;
constexpr uint32_t Post_DrawRotatingGhostLayers = 0x10038E10u;
constexpr uint32_t sub_10039410 = 0x10039410u;
constexpr uint32_t CYyObject_Init = 0x10039440u;
constexpr uint32_t ThreadPool_StartThread = 0x100394D0u;
constexpr uint32_t ThreadPool_Shutdown = 0x10039550u;
constexpr uint32_t FUN_10039590 = 0x10039590u;
constexpr uint32_t CYyObject_VT_1032A9F8_Construct = 0x100395D0u;
constexpr uint32_t FUN_10039670 = 0x10039670u;
constexpr uint32_t Thread_ResumeAndWait = 0x100396A0u;
constexpr uint32_t FUN_100397a0 = 0x100397A0u;
constexpr uint32_t Util_Copy16Bytes = 0x10039810u;
constexpr uint32_t Util_Copy16Bytes_v1 = 0x10039810u;
constexpr uint32_t CYyTexBase_Constructor = 0x10039830u;
constexpr uint32_t CYyTexBase_Destructor = 0x100398D0u;
constexpr uint32_t CYyTexBase_SetName = 0x100398E0u;
constexpr uint32_t TexObj_Constructor = 0x10039920u;
constexpr uint32_t CYyTex_Destructor = 0x10039970u;
constexpr uint32_t TexObj_CreateAndUpload = 0x10039A30u;
constexpr uint32_t TexObj_CreateAndUpload_v1 = 0x10039A30u;
constexpr uint32_t TexObj_Reupload = 0x10039A60u;
constexpr uint32_t TexObj_Create = 0x10039A80u;
constexpr uint32_t TexObj_Upload = 0x10039E50u;
constexpr uint32_t CYyTexMng_Init = 0x1003AA50u;
constexpr uint32_t CYyTexMng_Destructor = 0x1003AA90u;
constexpr uint32_t TexMgr_GetField8 = 0x1003AAB0u;
constexpr uint32_t FUN_1003ab10 = 0x1003AB10u;
constexpr uint32_t FUN_1003ab40 = 0x1003AB40u;
constexpr uint32_t TexMgr_UploadPumpEntry = 0x1003AB70u;
constexpr uint32_t TexMgr_Enqueue = 0x1003AB80u;
constexpr uint32_t TexMgr_UploadPump = 0x1003ABB0u;
constexpr uint32_t CYyTexMng_FindByName = 0x1003AC80u;
constexpr uint32_t CYyTexMng_FindByName_v1 = 0x1003AC80u;
constexpr uint32_t sub_1003ACD0 = 0x1003ACD0u;
constexpr uint32_t sub_1003AD40 = 0x1003AD40u;
constexpr uint32_t sub_1003ADC0 = 0x1003ADC0u;
constexpr uint32_t TexMgr_FindByName = 0x1003AEA0u;
constexpr uint32_t TexMgr_FindByName_v1 = 0x1003AEA0u;
constexpr uint32_t CYyTexMng_FindByNameInResource = 0x1003AF10u;
constexpr uint32_t sub_1003B080 = 0x1003B080u;
constexpr uint32_t CYyVbMng_AcquireVertexBuffer = 0x1003B210u;
constexpr uint32_t CYyVbMng_ReleaseVertexBuffer = 0x1003B3C0u;
constexpr uint32_t CYyVb_Constructor = 0x1003B450u;
constexpr uint32_t CYyVb_Destroy = 0x1003B4A0u;
constexpr uint32_t VertexBufferList_Register = 0x1003B4D0u;
constexpr uint32_t FUN_1003b4e0 = 0x1003B4E0u;
constexpr uint32_t CMoAttachments_Construct = 0x1003B540u;
constexpr uint32_t ReleaseSubObjectIfPresent = 0x1003B5C0u;
constexpr uint32_t ReleaseSubObjectIfPresent_v1 = 0x1003B5C0u;
constexpr uint32_t CMoAttachments_Reset = 0x1003B640u;
constexpr uint32_t CMoAttachments_Reset_v1 = 0x1003B640u;
constexpr uint32_t CMoAttachments_ResetAll = 0x1003B690u;
constexpr uint32_t CMoAttachments_ResetAll_v1 = 0x1003B690u;
constexpr uint32_t CMoAttachments_GetCaster = 0x1003B6D0u;
constexpr uint32_t CMoAttachments_GetCaster_v1 = 0x1003B6D0u;
constexpr uint32_t CMoAttachments_GetTarget = 0x1003B700u;
constexpr uint32_t CMoAttachments_GetTarget_v1 = 0x1003B700u;
constexpr uint32_t CMoAttachments_ClearCaster = 0x1003B750u;
constexpr uint32_t CMoAttachments_ClearCaster_v1 = 0x1003B750u;
constexpr uint32_t CMoAttachments_ClearTarget = 0x1003B780u;
constexpr uint32_t CMoAttachments_ClearTarget_v1 = 0x1003B780u;
constexpr uint32_t CMoAttachments_SetCaster = 0x1003B7B0u;
constexpr uint32_t CMoAttachments_SetCaster_v1 = 0x1003B7B0u;
constexpr uint32_t CMoAttachments_SetTarget = 0x1003B830u;
constexpr uint32_t CMoAttachments_SetTarget_v1 = 0x1003B830u;
constexpr uint32_t CMoAttachments_FindKindInChain = 0x1003B8B0u;
constexpr uint32_t CMoAttachments_FindKindFromRoot = 0x1003B900u;
constexpr uint32_t sub_1003B980 = 0x1003B980u;
constexpr uint32_t sub_1003BA20 = 0x1003BA20u;
constexpr uint32_t CMoAttachments_SetMappedSlot = 0x1003BD90u;
constexpr uint32_t YmGenerater_BuildAttachMatrix = 0x1003BE10u;
constexpr uint32_t CMoAttachments_NotifyCaster = 0x1003C6E0u;
constexpr uint32_t CMoAttachments_NotifyTarget = 0x1003C700u;
constexpr uint32_t Unit1003C6E0_CopyFields = 0x1003C740u;
constexpr uint32_t CMoCamera_GetRuntimeClass = 0x1003C880u;
constexpr uint32_t CMoCameraTask_Destructor = 0x1003D030u;
constexpr uint32_t Sky_DrawGradientDome = 0x1003D640u;
constexpr uint32_t CMoCelestial_Destructor = 0x1003D980u;
constexpr uint32_t Sky_DestroyDomeShaderStatic = 0x1003DB50u;
constexpr uint32_t Sky_DrawDomeViewCentered = 0x1003DB60u;
constexpr uint32_t Sky_DestroyViewCenteredShaderStatic = 0x1003DDF0u;
constexpr uint32_t Color_SetFromU32SwapRB = 0x1003E280u;
constexpr uint32_t Color_CopySwapRB = 0x1003E2A0u;
constexpr uint32_t YmElem_DrawD3ASegment = 0x1003E910u;
constexpr uint32_t YmElem_SetD3ADrawState = 0x1003E9A0u;
constexpr uint32_t unknown_libname_20 = 0x1003EB00u;
constexpr uint32_t unknown_libname_21 = 0x1003EB10u;
constexpr uint32_t unknown_libname_22 = 0x1003EB20u;
constexpr uint32_t CMoVuAnmElem_Destructor = 0x1003EF20u;
constexpr uint32_t unknown_libname_25 = 0x1003F890u;
constexpr uint32_t CMoDistMorphElem_ComputeMorphHandle = 0x1003F8A0u;
constexpr uint32_t CXi471011C8_VT_1032AC90_Dtor = 0x1003FB10u;
constexpr uint32_t CXi471011C8_VT_1032AC90_Ctor = 0x1003FB70u;
constexpr uint32_t CMoDistModelElem_IsDistortedMode = 0x1003FCB0u;
constexpr uint32_t CMoDistModelElem_GetVertexBase = 0x1003FCE0u;
constexpr uint32_t CMoDistModelElem_GetVertexBase_v1 = 0x1003FCE0u;
constexpr uint32_t CMoDistModelElem_GetDataEnd = 0x1003FCF0u;
constexpr uint32_t CMoDistModelElem_GetDataEnd_v1 = 0x1003FCF0u;
constexpr uint32_t unknown_libname_33 = 0x10040B50u;
constexpr uint32_t unknown_libname_34 = 0x10040B60u;
constexpr uint32_t unknown_libname_35 = 0x10040B70u;
constexpr uint32_t unknown_libname_36 = 0x10040B80u;
constexpr uint32_t unknown_libname_37 = 0x10040B90u;
constexpr uint32_t CXi471011C8_VT_1032AD80_Dtor = 0x10041110u;
constexpr uint32_t Font_DrawProjectedText3D = 0x10041C20u;
constexpr uint32_t CMoOtTask_ScalarDeletingDtor = 0x100425F0u;
constexpr uint32_t unknown_libname_46 = 0x10042CD0u;
constexpr uint32_t unknown_libname_47 = 0x10042CE0u;
constexpr uint32_t nullsub_136 = 0x10042CF0u;
constexpr uint32_t CMoDistMorphElem_ValidateMorphData = 0x10042D00u;
constexpr uint32_t unknown_libname_49 = 0x10043310u;
constexpr uint32_t StaticDtor_StateBlock_1047BB7C = 0x100439B0u;
constexpr uint32_t CMoTask_VT_1032AF60_ScalarDeletingDtor = 0x10043A90u;
constexpr uint32_t CMoTask_VT_1032AF60_Destructor = 0x10043AB0u;
constexpr uint32_t CMoTask_VT_1032AFA8_ScalarDeletingDtor = 0x10043C90u;
constexpr uint32_t CMoTask_VT_1032AFA8_Destructor = 0x10043CB0u;
constexpr uint32_t Unit10043DD0_ToggleByte45 = 0x10043DD0u;
constexpr uint32_t Unit10043DD0_ReturnZero = 0x10043DE0u;
constexpr uint32_t Unit10043DD0_ReturnZero_v1 = 0x10043DE0u;
constexpr uint32_t DynVB_Constructor = 0x10043DF0u;
constexpr uint32_t DynVB_Constructor_v1 = 0x10043DF0u;
constexpr uint32_t DynVB_Destructor = 0x10043E40u;
constexpr uint32_t DynVB_Create = 0x10043E50u;
constexpr uint32_t Unit10043E40_ReturnBuffersToPool = 0x10043F00u;
constexpr uint32_t VertexBatch_BindAndDraw = 0x10043F40u;
constexpr uint32_t VertexBatch_BindAndDraw_v1 = 0x10043F40u;
constexpr uint32_t DynVB_Reserve = 0x10044010u;
constexpr uint32_t DynVB_Lock = 0x10044130u;
constexpr uint32_t DynVB_Unlock = 0x100441A0u;
constexpr uint32_t CameraCache_CacheViewProjection = 0x100441C0u;
constexpr uint32_t Gfx_SetViewMatrixFromField10 = 0x10044210u;
constexpr uint32_t Gfx_SetViewMatrixFromField90 = 0x10044230u;
constexpr uint32_t Mo_InitSharedRenderResources = 0x10044270u;
constexpr uint32_t Unit10043E40_ReleaseResourcesOf = 0x100446A0u;
constexpr uint32_t CMoElem_Constructor = 0x10044890u;
constexpr uint32_t CMoElemFamily_SharedCleanup = 0x10044970u;
constexpr uint32_t CMoElemFamily_SharedCleanup_v1 = 0x10044970u;
constexpr uint32_t CMoElem_SetSourceAt0FC = 0x10044AA0u;
constexpr uint32_t CMoElem_AdvanceTimer = 0x10044AE0u;
constexpr uint32_t Unit10044C40_ComputeWorldPos = 0x10044C40u;
constexpr uint32_t Unit10044C40_ComputeWorldPos_v1 = 0x10044C40u;
constexpr uint32_t Unit10044C40_IsActive = 0x10044CF0u;
constexpr uint32_t Unit10044C40_UpdateTransform = 0x10044D40u;
constexpr uint32_t YmElem_ComputeViewDepthKey = 0x10044F10u;
constexpr uint32_t FUN_10045190 = 0x10045190u;
constexpr uint32_t CMoElem_InvertInPlace = 0x10045800u;
constexpr uint32_t CMoElem_CheckAt138ThenCall = 0x100458F0u;
constexpr uint32_t YmElem_BuildDrawMatrixAndTest = 0x10045920u;
constexpr uint32_t Unit10044C40_GetFlaggedMatrix = 0x10045C50u;
constexpr uint32_t Unit10044C40_GetFlaggedMatrix_v1 = 0x10045C50u;
constexpr uint32_t RegisterWithProcessor = 0x10045D40u;
constexpr uint32_t RegisterWithProcessor_v1 = 0x10045D40u;
constexpr uint32_t Zone_UpdateFogAmbient = 0x10046380u;
constexpr uint32_t Fog_SetRenderStates = 0x100464D0u;
constexpr uint32_t sub_10046550 = 0x10046550u;
constexpr uint32_t unknown_libname_53 = 0x100472D0u;
constexpr uint32_t unknown_libname_54 = 0x100473E0u;
constexpr uint32_t CMoElem_GetActiveField0A4 = 0x100473F0u;
constexpr uint32_t CMoElem_GetActiveField0A4_v1 = 0x100473F0u;
constexpr uint32_t CMoElem_GetAddr78 = 0x10047450u;
constexpr uint32_t CMoElem_GetAddr84 = 0x10047490u;
constexpr uint32_t CMoSkeletonElem_Destructor = 0x10047900u;
constexpr uint32_t CMoGeneratorClone_VT_1032B114_Constructor = 0x10047F80u;
constexpr uint32_t CMoGeneratorClone_VT_1032B114_ScalarDeletingDtor = 0x10047FE0u;
constexpr uint32_t CMoGeneratorClone_VT_1032B114_Destructor = 0x10048000u;
constexpr uint32_t CMoGeneratorClone_VT_1032B114_Destructor_v1 = 0x10048000u;
constexpr uint32_t FUN_10048100 = 0x10048100u;
constexpr uint32_t CMoGeneratorClone_UnlinkElemListA = 0x10048140u;
constexpr uint32_t FUN_10048190 = 0x10048190u;
constexpr uint32_t CMoGeneratorClone_UnlinkElemListB = 0x100481D0u;
constexpr uint32_t FUN_100492b0 = 0x100492B0u;
constexpr uint32_t CMoGeneratorClone_FindCmdByType = 0x100492F0u;
constexpr uint32_t CMoGeneratorClone_FindCmdByType_v1 = 0x100492F0u;
constexpr uint32_t Unit10048000_GetShort22ViaPtrE4 = 0x100493B0u;
constexpr uint32_t CMoGeneratorClone_ResolveTarget = 0x10049570u;
constexpr uint32_t CMoGeneratorClone_ResolveTarget_v1 = 0x10049570u;
constexpr uint32_t FUN_100496f0 = 0x100496F0u;
constexpr uint32_t sub_100496F0 = 0x100496F0u;
constexpr uint32_t FUN_1004a980 = 0x1004A980u;
constexpr uint32_t CMoGeneratorClone_ExecuteCmdList = 0x1004E720u;
constexpr uint32_t SharedConditionalStateReset = 0x100531D0u;
constexpr uint32_t CMoTask_Restart = 0x10053370u;
constexpr uint32_t CMoTask_Restart_v1 = 0x10053370u;
constexpr uint32_t CMoGeneratorClone_CopyParams = 0x10053450u;
constexpr uint32_t CMoGeneratorClone_CreateInstance = 0x100536C0u;
constexpr uint32_t CMoGeneratorClone_VT_1032B69C_ScalarDeletingDtor = 0x10053840u;
constexpr uint32_t Generator_ClearSubobjectOrSetFlag = 0x10053890u;
constexpr uint32_t Generator_ClearSubobjectOrSetFlag_v1 = 0x10053890u;
constexpr uint32_t Generator_ClearSubobjectOrSetFlag_v2 = 0x10053890u;
constexpr uint32_t YmGenerater_Stop = 0x100538E0u;
constexpr uint32_t YmGenerater_Stop_v1 = 0x100538E0u;
constexpr uint32_t YmGenerater_Stop_v2 = 0x100538E0u;
constexpr uint32_t Generator_IsFlag400OrField22Zero = 0x100539E0u;
constexpr uint32_t Generator_IsFlag400OrField22Zero_v1 = 0x100539E0u;
constexpr uint32_t Unit10053860_StopGenerators = 0x10053A00u;
constexpr uint32_t YmGenerater_UpdateAttach = 0x10053C80u;
constexpr uint32_t YmGenerater_UpdateAttachIfPresent = 0x10053DD0u;
constexpr uint32_t CMoTask_VT_1032B654_Destructor = 0x100541C0u;
constexpr uint32_t CMoGeneratorClone_GetRuntimeClass = 0x100542E0u;
constexpr uint32_t CMoGeneratorClone_Destructor = 0x100542F0u;
constexpr uint32_t CMoGeneratorClone_Kill = 0x100543A0u;
constexpr uint32_t Unit100544A0_EvaluateWithFlag34 = 0x100544D0u;
constexpr uint32_t Unit100544A0_Evaluate = 0x10054500u;
constexpr uint32_t Unit100544A0_EvaluateEaseIn = 0x100546F0u;
constexpr uint32_t Unit100544A0_EvaluateEaseInOut = 0x100547A0u;
constexpr uint32_t CMoLfdElem_Destructor = 0x100548C0u;
constexpr uint32_t nullsub_130 = 0x10054F70u;
constexpr uint32_t Unit100548D0_Init = 0x10054FC0u;
constexpr uint32_t Vt1032B6D8_Destructor = 0x10055000u;
constexpr uint32_t Unit100548D0_FreeSplineSet = 0x100550F0u;
constexpr uint32_t FUN_10055130 = 0x10055130u;
constexpr uint32_t FUN_10055160 = 0x10055160u;
constexpr uint32_t LinkNode_Init = 0x10055240u;
constexpr uint32_t LinkNode_Init_v1 = 0x10055240u;
constexpr uint32_t LinkNode_Unlink = 0x10055250u;
constexpr uint32_t CMoOT_Constructor = 0x100552A0u;
constexpr uint32_t CMoOT_Destructor = 0x100552E0u;
constexpr uint32_t Unit100552F0_ClearLink = 0x100552F0u;
constexpr uint32_t Unit100552F0_Init = 0x10055300u;
constexpr uint32_t FUN_100554b0 = 0x100554B0u;
constexpr uint32_t Unit100552F0_Free = 0x10055520u;
constexpr uint32_t CMoOtTask_Constructor = 0x10055570u;
constexpr uint32_t FUN_100555b0 = 0x100555B0u;
constexpr uint32_t sub_100555B0 = 0x100555B0u;
constexpr uint32_t CMoOt_InitSystem = 0x100555D0u;
constexpr uint32_t CMoOt_Update = 0x10055620u;
constexpr uint32_t CMoOtTask_Clear = 0x10055640u;
constexpr uint32_t FUN_10055650 = 0x10055650u;
constexpr uint32_t FUN_10055ad0 = 0x10055AD0u;
constexpr uint32_t FUN_10056160 = 0x10056160u;
constexpr uint32_t CMoSphRes_FixupOffsets = 0x10056220u;
constexpr uint32_t CMoPointLightProgElem_Destructor = 0x10056390u;
constexpr uint32_t CMoPathDriveActorTask_GetRuntimeClass = 0x10056940u;
constexpr uint32_t CMoLockColorDriveTask_GetRuntimeClass = 0x10056960u;
constexpr uint32_t CMoLockLookAtDriveTask_GetRuntimeClass = 0x10056980u;
constexpr uint32_t CMoActorColorDriveTask_GetRuntimeClass = 0x100569A0u;
constexpr uint32_t CMoActorRotationDriveTask_GetRuntimeClass = 0x100569C0u;
constexpr uint32_t Scheduler_Execute = 0x10056C70u;
constexpr uint32_t Scheduler_Execute_v1 = 0x10056C70u;
constexpr uint32_t Scheduler_RemoveTasksFor = 0x10056EB0u;
constexpr uint32_t Scheduler_Kill = 0x10056EF0u;
constexpr uint32_t CMoSchedularTask_Construct = 0x10057270u;
constexpr uint32_t CMoSchedularTask_ResetFlags = 0x100573B0u;
constexpr uint32_t FUN_100573e0 = 0x100573E0u;
constexpr uint32_t CMoSchedularTask_Destructor = 0x100574A0u;
constexpr uint32_t FUN_10057c20 = 0x10057C20u;
constexpr uint32_t CMoSchedularTask_Interpret = 0x10057FB0u;
constexpr uint32_t FUN_1005e100 = 0x1005E100u;
constexpr uint32_t FUN_1005ed30 = 0x1005ED30u;
constexpr uint32_t FUN_1005ed90 = 0x1005ED90u;
constexpr uint32_t FUN_1005edc0 = 0x1005EDC0u;
constexpr uint32_t FUN_1005ee90 = 0x1005EE90u;
constexpr uint32_t FUN_1005eec0 = 0x1005EEC0u;
constexpr uint32_t CMoPathDriveActorTask_ApplyFraction = 0x1005EEF0u;
constexpr uint32_t CMoPathDriveActorTask_ScalarDeletingDtor = 0x1005F2C0u;
constexpr uint32_t CMoLockColorDriveTask_ScalarDeletingDtor = 0x1005F3F0u;
constexpr uint32_t CMoLockLookAtDriveTask_ScalarDeletingDtor = 0x1005F660u;
constexpr uint32_t CMoActorColorDriveTask_ScalarDeletingDtor = 0x1005F990u;
constexpr uint32_t CMoActorRotationDriveTask_ScalarDeletingDtor = 0x1005FE30u;
constexpr uint32_t CMoAttachments_VT_1032B940_ScalarDeletingDtor = 0x10060190u;
constexpr uint32_t CMoAttachments_VT_1032BAF0_ScalarDeletingDtor = 0x100602D0u;
constexpr uint32_t CMoAttachments_VT_1032BB38_ScalarDeletingDtor = 0x100608B0u;
constexpr uint32_t CMoAttachments_VT_1032BBAC_VectorDeletingDtor = 0x10060BB0u;
constexpr uint32_t CMoAttachments_VT_1032BBF4_VectorDeletingDtor = 0x10060E60u;
constexpr uint32_t CMoAttachments_VT_1032BC68_VectorDeletingDtor = 0x10061010u;
constexpr uint32_t CMoAttachments_VT_1032BC84_VectorDeletingDtor = 0x10061140u;
constexpr uint32_t CMoAttachments_VT_1032BCCC_VectorDeletingDtor = 0x10061300u;
constexpr uint32_t CMoAttachments_VT_1032BD14_VectorDeletingDtor = 0x100614B0u;
constexpr uint32_t CMoAttachments_VT_1032BD5C_VectorDeletingDtor = 0x100616B0u;
constexpr uint32_t CMoAttachments_VT_1032BDA4_VectorDeletingDtor = 0x10061830u;
constexpr uint32_t CMoAttachments_VT_1032BDEC_VectorDeletingDtor = 0x10061A00u;
constexpr uint32_t CMoAttachments_VT_1032BE34_VectorDeletingDtor = 0x10061B80u;
constexpr uint32_t CMoAttachments_VT_1032BE7C_VectorDeletingDtor = 0x10061D00u;
constexpr uint32_t CMoAttachments_VT_1032BEC4_VectorDeletingDtor = 0x10061EA0u;
constexpr uint32_t CMoAttachments_VT_1032BF0C_VectorDeletingDtor = 0x10062090u;
constexpr uint32_t CMoAttachments_VT_1032BF54_VectorDeletingDtor = 0x10062210u;
constexpr uint32_t CMoAttachments_VT_1032BF9C_VectorDeletingDtor = 0x100623B0u;
constexpr uint32_t CMoAttachments_VT_1032BFE4_VectorDeletingDtor = 0x10062550u;
constexpr uint32_t CMoAttachments_VT_1032C02C_VectorDeletingDtor = 0x10062700u;
constexpr uint32_t AfterimageTask_SetCaster = 0x10062830u;
constexpr uint32_t AfterimageTask_SetTarget = 0x10062880u;
constexpr uint32_t FUN_10062de0 = 0x10062DE0u;
constexpr uint32_t AfterimageTask_Execute = 0x10062E20u;
constexpr uint32_t AfterimageTask_Kill = 0x10062E80u;
constexpr uint32_t FUN_10062ed0 = 0x10062ED0u;
constexpr uint32_t CMoSpline_Construct = 0x10063460u;
constexpr uint32_t CMoSpline_Destructor = 0x100634A0u;
constexpr uint32_t CMoSpline_Destructor_v1 = 0x100634A0u;
constexpr uint32_t CMoSpline_Init = 0x100634B0u;
constexpr uint32_t CMoSpline_FreeBuffers = 0x10063550u;
constexpr uint32_t FUN_100635b0 = 0x100635B0u;
constexpr uint32_t FUN_10063a60 = 0x10063A60u;
constexpr uint32_t CMoSpline_ReleaseRef = 0x10063B20u;
constexpr uint32_t CMoSpline_ReleaseRef_v1 = 0x10063B20u;
constexpr uint32_t CMoSpline_GetWrapped = 0x10063B50u;
constexpr uint32_t CMoSpline_GetWrapped_v1 = 0x10063B50u;
constexpr uint32_t CMoSpline_GetData = 0x10063B70u;
constexpr uint32_t CMoSpline_EnsureSamples = 0x10063B80u;
constexpr uint32_t CMoSpline_SetValue = 0x10063C30u;
constexpr uint32_t MoProgElem_FreeBuffer40 = 0x10063C60u;
constexpr uint32_t FUN_100683d0 = 0x100683D0u;
constexpr uint32_t Unit10064190_SetDwordValue = 0x100684F0u;
constexpr uint32_t Unit10064190_AssignTyped = 0x10068590u;
constexpr uint32_t CMoDX_Destructor = 0x10069D70u;
constexpr uint32_t CMoDX_Initialize = 0x10069DD0u;
constexpr uint32_t Unit10069DD0_Nop = 0x10069E90u;
constexpr uint32_t FUN_10069ee0 = 0x10069EE0u;
constexpr uint32_t sub_1006A060 = 0x1006A060u;
constexpr uint32_t sub_1006A070 = 0x1006A070u;
constexpr uint32_t sub_1006A0E0 = 0x1006A0E0u;
constexpr uint32_t CMoDX_CreateTexture = 0x1006A150u;
constexpr uint32_t Camera_SetupMatrices = 0x1006A180u;
constexpr uint32_t sub_1006A420 = 0x1006A420u;
constexpr uint32_t CMoDX_SetWorldMatrix = 0x1006A480u;
constexpr uint32_t CMoDX_GetWorldMatrix = 0x1006A4A0u;
constexpr uint32_t CMoDX_PushViewMatrix = 0x1006A4C0u;
constexpr uint32_t CMoDX_PopViewMatrix = 0x1006A4F0u;
constexpr uint32_t CMoDX_SetViewMatrix = 0x1006A520u;
constexpr uint32_t CMoDX_SetViewMatrix_v1 = 0x1006A520u;
constexpr uint32_t Dx_GetCachedTransform2 = 0x1006A540u;
constexpr uint32_t Gfx_CalcFVFStride = 0x1006A560u;
constexpr uint32_t Attach_SetTranslation = 0x1006AD30u;
constexpr uint32_t Processor_ApplyPair = 0x1006AD50u;
constexpr uint32_t Unit10069DD0_SetInvalid = 0x1006AD90u;
constexpr uint32_t fn_1006AD90 = 0x1006AD90u;
constexpr uint32_t Unit10069DD0_Insert = 0x1006ADF0u;
constexpr uint32_t StateBlock_ApplyRSTSS = 0x1006AE30u;
constexpr uint32_t StateBlockHolder_ApplyRenderStates = 0x1006AF00u;
constexpr uint32_t StateBlock_ApplyTSS = 0x1006AF90u;
constexpr uint32_t CMoDxStatusMng_Constructor = 0x1006B050u;
constexpr uint32_t CMoDxStatusMng_Destructor = 0x1006B090u;
constexpr uint32_t VBPool_Constructor = 0x1006B0C0u;
constexpr uint32_t Unit1006B0C0_Insert = 0x1006B120u;
constexpr uint32_t Unit1006B0C0_FindMatch = 0x1006B1D0u;
constexpr uint32_t CMoDX_CreateAndInitSystem = 0x1006B2B0u;
constexpr uint32_t sub_1006BC10 = 0x1006BC10u;
constexpr uint32_t sub_1006BC80 = 0x1006BC80u;
constexpr uint32_t Rand_SeedMT19937 = 0x1006BD90u;
constexpr uint32_t Rand_SeedMT19937_v1 = 0x1006BD90u;
constexpr uint32_t Unit1006B3B0_Next = 0x1006BE20u;
constexpr uint32_t Mem_AllocWithHeader12 = 0x1006C0F0u;
constexpr uint32_t Mem_FreeWithHeader12 = 0x1006C110u;
constexpr uint32_t CMoOcclusionMng_Constructor = 0x1006C140u;
constexpr uint32_t CMoOcclusionMng_Destructor = 0x1006C1E0u;
constexpr uint32_t j_OcclusionMng_CreateTarget = 0x1006C210u;
constexpr uint32_t OcclusionMng_CreateTarget = 0x1006C220u;
constexpr uint32_t OcclusionMng_CreateTarget_v1 = 0x1006C220u;
constexpr uint32_t OcclusionQuery_Init = 0x1006C260u;
constexpr uint32_t OcclusionQuery_Init_v1 = 0x1006C260u;
constexpr uint32_t OcclusionQuery_Release = 0x1006C270u;
constexpr uint32_t Occlusion_QueryScreenRect = 0x1006C280u;
constexpr uint32_t Occlusion_QueryScreenRectImpl = 0x1006C300u;
constexpr uint32_t DrawScreenProbe = 0x1006C530u;
constexpr uint32_t unknown_libname_59 = 0x1006C890u;
constexpr uint32_t unknown_libname_60 = 0x1006C8A0u;
constexpr uint32_t sub_1006C8B0 = 0x1006C8B0u;
constexpr uint32_t OcclusionMng_Prepare = 0x1006C9B0u;
constexpr uint32_t Unit1006C210_IsFloatClassAccepted = 0x1006CA20u;
constexpr uint32_t Unit1006C210_AreAll16FloatsAccepted = 0x1006CA60u;
constexpr uint32_t CreateProcessorInstance = 0x1006CB50u;
constexpr uint32_t FUN_1006ce40 = 0x1006CE40u;
constexpr uint32_t nullsub_44 = 0x1006CF90u;
constexpr uint32_t CMoProcessor_AddScaled3 = 0x1006D3B0u;
constexpr uint32_t CMoProcessor_TransposeInPlace = 0x1006D3E0u;
constexpr uint32_t CMoProcessor_Vec4TransformB = 0x1006D7E0u;
constexpr uint32_t CMoProcessor_Vec4Transform = 0x1006D870u;
constexpr uint32_t CMoProcessor_Vec3Transform = 0x1006D900u;
constexpr uint32_t CMoProcessor_Lerp3 = 0x1006D980u;
constexpr uint32_t CMoProcessor_CalcScreenBounds = 0x1006E970u;
constexpr uint32_t CMoProcessor_ProjectVertsAndBounds = 0x1006EB70u;
constexpr uint32_t CMoProcessor_CalcScreenBoundsAndVisible = 0x1006ED10u;
constexpr uint32_t CMoTask_InitVariant12 = 0x10070F40u;
constexpr uint32_t CMoTask_VT_1032C490_Destructor = 0x10070F80u;
constexpr uint32_t CMoTask_InitAndRegister = 0x10070F90u;
constexpr uint32_t ResHandle_FreeIfUnreferenced = 0x10071050u;
constexpr uint32_t ResHandle_FreeIfUnreferenced_v1 = 0x10071050u;
constexpr uint32_t ResourceHandle_IsResident = 0x10071060u;
constexpr uint32_t ResourceHandle_IsResident_v1 = 0x10071060u;
constexpr uint32_t Unit10070F90_Create = 0x100710F0u;
constexpr uint32_t CMoAttachments_VT_1032C4E8_ScalarDeletingDtor = 0x10071180u;
constexpr uint32_t Unit10070F90_CopyField14At0 = 0x100711D0u;
constexpr uint32_t Unit10070F90_CopyField14At4 = 0x100711E0u;
constexpr uint32_t FUN_10071270 = 0x10071270u;
constexpr uint32_t nullsub_171 = 0x100719B0u;
constexpr uint32_t CMoAttachments_VT_1032C628_ScalarDeletingDtor = 0x10071C10u;
constexpr uint32_t CMoResource_VT_1032C588_ScalarDeletingDtor_10071C60 = 0x10071C60u;
constexpr uint32_t FUN_10071d80 = 0x10071D80u;
constexpr uint32_t CMoResource_Destructor = 0x10071DE0u;
constexpr uint32_t FUN_10071e50 = 0x10071E50u;
constexpr uint32_t CMoResource_AddRefChain = 0x10071E90u;
constexpr uint32_t CMoResource_AddRefChain_v1 = 0x10071E90u;
constexpr uint32_t CMoResource_AddRefChain_v2 = 0x10071E90u;
constexpr uint32_t CMoResource_ReleaseChain = 0x10071EC0u;
constexpr uint32_t CMoResource_ReleaseChain_v1 = 0x10071EC0u;
constexpr uint32_t FUN_10071f50 = 0x10071F50u;
constexpr uint32_t sub_10071F50 = 0x10071F50u;
constexpr uint32_t FUN_10071f60 = 0x10071F60u;
constexpr uint32_t FUN_10071f70 = 0x10071F70u;
constexpr uint32_t FUN_10071f80 = 0x10071F80u;
constexpr uint32_t CMoResource_RegisterInTypeList = 0x10072030u;
constexpr uint32_t CMoResource_RegisterInTypeList_v1 = 0x10072030u;
constexpr uint32_t FUN_100720b0 = 0x100720B0u;
constexpr uint32_t FUN_10072180 = 0x10072180u;
constexpr uint32_t CMoResource_Destroy = 0x100721F0u;
constexpr uint32_t Unit10071E50_FindSelfLinkedNode = 0x10072240u;
constexpr uint32_t CMoResource_GetParent = 0x10072270u;
constexpr uint32_t CMoResource_GetParent_v1 = 0x10072270u;
constexpr uint32_t CMoResource_FindByTypeAndId = 0x10072290u;
constexpr uint32_t CMoResource_FindByTypeAndId_v1 = 0x10072290u;
constexpr uint32_t CMoResource_FindWorker = 0x100722C0u;
constexpr uint32_t FUN_100722c0 = 0x100722C0u;
constexpr uint32_t CMoResource_FindInScope = 0x10072430u;
constexpr uint32_t YmResourceFile_FindResourceUnder = 0x100724C0u;
constexpr uint32_t YmResourceFile_FindResourceUnder_v1 = 0x100724C0u;
constexpr uint32_t Unit10071E50_FindInChainByKind = 0x10072540u;
constexpr uint32_t FUN_10072970 = 0x10072970u;
constexpr uint32_t unknown_libname_61 = 0x100729C0u;
constexpr uint32_t Unit10071E50_ClearGlobal1047D138 = 0x100729D0u;
constexpr uint32_t CMoTask_VT_1032C908_Destructor = 0x100729E0u;
constexpr uint32_t Unit10072A50_Start = 0x10072C30u;
constexpr uint32_t Unit10072A50_Start_v1 = 0x10072C30u;
constexpr uint32_t sub_10072DB0 = 0x10072DB0u;
constexpr uint32_t MeshResourceMng_LoadSystemResources = 0x10072ED0u;
constexpr uint32_t ResourceHandle_Deref = 0x10072FB0u;
constexpr uint32_t ResourceHandle_Deref_v1 = 0x10072FB0u;
constexpr uint32_t FUN_10072fd0 = 0x10072FD0u;
constexpr uint32_t sub_10072FD0 = 0x10072FD0u;
constexpr uint32_t CMoResource_FindOrCreateTask = 0x100730E0u;
constexpr uint32_t FUN_100731b0 = 0x100731B0u;
constexpr uint32_t sub_100731B0 = 0x100731B0u;
constexpr uint32_t MeshResourceMng_ResolveTree = 0x10073430u;
constexpr uint32_t CMoResource_RegisterAndMark = 0x10073A60u;
constexpr uint32_t MoRes_FreeIfUnreferenced = 0x10073B20u;
constexpr uint32_t MoRes_FreeIfUnreferenced_v1 = 0x10073B20u;
constexpr uint32_t CMoResource_InitAndGetFlag = 0x10073B80u;
constexpr uint32_t ResourceIter_FindById = 0x10073BA0u;
constexpr uint32_t ResourceIter_FindOrStartTask = 0x10073D80u;
constexpr uint32_t ResourceIter_PopFreeNode = 0x10074110u;
constexpr uint32_t MeshResourceMng_AllocHandle = 0x10074140u;
constexpr uint32_t MeshResourceMng_AllocHandle_v1 = 0x10074140u;
constexpr uint32_t NodePool_ReleaseNode = 0x10074180u;
constexpr uint32_t NodePool_ReleaseNode_v1 = 0x10074180u;
constexpr uint32_t ResourceIter_InPoolRange = 0x100741E0u;
constexpr uint32_t ResourceIter_SetCurrentGlobal = 0x10074280u;
constexpr uint32_t GetGlobal_1047D114 = 0x10074290u;
constexpr uint32_t FUN_100742a0 = 0x100742A0u;
constexpr uint32_t ResourceIter_FindFirst = 0x100742B0u;
constexpr uint32_t ResourceIter_FindFirst_v1 = 0x100742B0u;
constexpr uint32_t ResourceIter_Start = 0x10074310u;
constexpr uint32_t ResourceIter_MatchCurrent = 0x10074380u;
constexpr uint32_t ResourceIter_Advance = 0x100743D0u;
constexpr uint32_t ResourceIter_FindNext = 0x100745C0u;
constexpr uint32_t ResourceIter_FindNext_v1 = 0x100745C0u;
constexpr uint32_t MoProcessor_CallVSlot49 = 0x100746A0u;
constexpr uint32_t MoProcessor_CallVSlot49_v1 = 0x100746A0u;
constexpr uint32_t CMoTask_Constructor = 0x10074740u;
constexpr uint32_t CMoTask_Constructor_v1 = 0x10074740u;
constexpr uint32_t CMoTask_Constructor_v2 = 0x10074740u;
constexpr uint32_t CMoTask_Destructor = 0x100747C0u;
constexpr uint32_t CMoTask_Destructor_v1 = 0x100747C0u;
constexpr uint32_t FUN_10074850 = 0x10074850u;
constexpr uint32_t FUN_10074890 = 0x10074890u;
constexpr uint32_t CMoTask_ResetFields = 0x100748F0u;
constexpr uint32_t CMoTask_ResetFields_v1 = 0x100748F0u;
constexpr uint32_t CMoTask_ResetFields_v2 = 0x100748F0u;
constexpr uint32_t CMoTask_RegisterWithMng = 0x100749B0u;
constexpr uint32_t CMoTask_RegisterWithMng_v1 = 0x100749B0u;
constexpr uint32_t FUN_100749f0 = 0x100749F0u;
constexpr uint32_t FUN_100749f0_v1 = 0x100749F0u;
constexpr uint32_t FUN_10074a00 = 0x10074A00u;
constexpr uint32_t FUN_10074a00_v1 = 0x10074A00u;
constexpr uint32_t CMoTaskMng_Forward10075120 = 0x10074A10u;
constexpr uint32_t FUN_10074a50 = 0x10074A50u;
constexpr uint32_t CMoTask_Release = 0x10074AB0u;
constexpr uint32_t FUN_10074ad0 = 0x10074AD0u;
constexpr uint32_t CMoTaskMng_Destructor = 0x10074B60u;
constexpr uint32_t CMoTaskMng_InitPool = 0x10074BA0u;
constexpr uint32_t sub_10074C30 = 0x10074C30u;
constexpr uint32_t FUN_10074cc0 = 0x10074CC0u;
constexpr uint32_t CMoTaskMng_RemoveByKey = 0x10074CF0u;
constexpr uint32_t CMoTaskMng_AllocTask = 0x10074DF0u;
constexpr uint32_t Unit10074CA0_FindTask = 0x10074F30u;
constexpr uint32_t Unit10074CA0_FindTask_v1 = 0x10074F30u;
constexpr uint32_t CMoTaskMng_InsertIntoBucket = 0x10074F80u;
constexpr uint32_t CMoTask_Dispatch = 0x10075000u;
constexpr uint32_t CMoTaskMng_InsertAfterCursor = 0x10075120u;
constexpr uint32_t CMoTaskMng_UnlinkTask = 0x100751B0u;
constexpr uint32_t Gfx_InitCubeMapTextures = 0x10075280u;
constexpr uint32_t TexRef_ResolveByName = 0x10075520u;
constexpr uint32_t TexRef_EnsureRenderTarget = 0x100755A0u;
constexpr uint32_t TextureHolder_BindWarped = 0x10075700u;
constexpr uint32_t unknown_libname_63 = 0x10075950u;
constexpr uint32_t TexRef_SetTexture = 0x10075960u;
constexpr uint32_t TexRef_SetTexture_v1 = 0x10075960u;
constexpr uint32_t unknown_libname_64 = 0x10075C30u;
constexpr uint32_t CMoTask_SetRect30 = 0x10076750u;
constexpr uint32_t Arg_SplitQuoted = 0x10076830u;
constexpr uint32_t Arg_CopyCharOrPair = 0x100769F0u;
constexpr uint32_t Arg_StripQuotes = 0x10076A70u;
constexpr uint32_t preprocessArg = 0x10076AB0u;
constexpr uint32_t getNounFromPronoun = 0x10076FF0u;
constexpr uint32_t FUN_10077a00 = 0x10077A00u;
constexpr uint32_t Arg_FindInSortedTable = 0x10077AD0u;
constexpr uint32_t Arg_LookupWord = 0x10077B40u;
constexpr uint32_t Unit10074CA0_MapNameLetters = 0x10077C30u;
constexpr uint32_t Acter_CopyNameToBuffer = 0x10077CF0u;
constexpr uint32_t Acter_GetCurrentIdRef = 0x10077D40u;
constexpr uint32_t Acter_ClearCurrentName = 0x10077D70u;
constexpr uint32_t Acter_FindMatchingActor = 0x10077D80u;
constexpr uint32_t ChatLog_AddFormattedMessage = 0x10077E30u;
constexpr uint32_t FUN_10077ed0 = 0x10077ED0u;
constexpr uint32_t Unit10074CA0_ToggleOnKey = 0x10077FB0u;
constexpr uint32_t thunk_FUN_100e97d0 = 0x10078090u;
constexpr uint32_t Unit10074CA0_BuildPacket1A_Default = 0x100780A0u;
constexpr uint32_t Unit10074CA0_BuildPacket1A = 0x100780D0u;
constexpr uint32_t FUN_10079170 = 0x10079170u;
constexpr uint32_t Unit10078260_ParseNumber = 0x10079A00u;
constexpr uint32_t SeekIdByString = 0x1007A1C0u;
constexpr uint32_t Actor_FindInRange = 0x1007A440u;
constexpr uint32_t Actor_FindByName = 0x1007A4C0u;
constexpr uint32_t FUN_1007bf30 = 0x1007BF30u;
constexpr uint32_t Unit10078260_ToggleStatusAndSend = 0x1007CD60u;
constexpr uint32_t FUN_1007cf80 = 0x1007CF80u;
constexpr uint32_t Actor_GetObjectByName = 0x10080C70u;
constexpr uint32_t Unit10078260_GetWord1047D3FC = 0x10080D00u;
constexpr uint32_t Unit10078260_IsIdNotInTable10357BB8 = 0x10080D20u;
constexpr uint32_t Actor_IsInTable = 0x10080D90u;
constexpr uint32_t Command_RunLine = 0x10080DC0u;
constexpr uint32_t Unit10078260_CheckCommandAllowed = 0x10081230u;
constexpr uint32_t Unit10078260_SetFlag1047D498 = 0x10081440u;
constexpr uint32_t Unit10078260_TestBit1047D499 = 0x10081480u;
constexpr uint32_t ActorHandle_Constructor = 0x100814A0u;
constexpr uint32_t ActorHandle_Constructor_v1 = 0x100814A0u;
constexpr uint32_t ActorHandle_Destructor = 0x100814E0u;
constexpr uint32_t ActorHandle_Destructor_v1 = 0x100814E0u;
constexpr uint32_t FUN_100814f0 = 0x100814F0u;
constexpr uint32_t sub_100814F0 = 0x100814F0u;
constexpr uint32_t ActorHandle_SetFields = 0x10081530u;
constexpr uint32_t ActorHandle_Resolve = 0x10081550u;
constexpr uint32_t ActorHandle_Resolve_v1 = 0x10081550u;
constexpr uint32_t ActorHandle_ResolveObject = 0x10081600u;
constexpr uint32_t Unit10081A30_ReleaseQueued = 0x10081A30u;
constexpr uint32_t Actor_ResetDrawLists = 0x10081A50u;
constexpr uint32_t FUN_10081ae0 = 0x10081AE0u;
constexpr uint32_t CXiActor_Init = 0x10081BA0u;
constexpr uint32_t CXiActor_ConstructBase = 0x10081CF0u;
constexpr uint32_t Actor_UnlinkAllAttachLists = 0x10082B10u;
constexpr uint32_t Actor_CountAll = 0x10082BF0u;
constexpr uint32_t CXiActor_GetField78Of70 = 0x10082FA0u;
constexpr uint32_t Unit10082F60_ProjectThroughCamera = 0x10082FB0u;
constexpr uint32_t CXiActor_NameCalc = 0x100830C0u;
constexpr uint32_t CXiActor_ComputeDrawState = 0x10083430u;
constexpr uint32_t FUN_10083620 = 0x10083620u;
constexpr uint32_t CXiActor_FindNearestToPoint = 0x10083750u;
constexpr uint32_t CXiActor_FindNearestLowerInView = 0x10083B40u;
constexpr uint32_t CXiActor_FindNearestHigherInView = 0x10083D00u;
constexpr uint32_t CXiActor_FindNearestToTarget = 0x10083F20u;
constexpr uint32_t CXiActor_IsWithinRadius = 0x10084100u;
constexpr uint32_t CXiActor_IsModelMissingOrFlag2000 = 0x10084140u;
constexpr uint32_t CXiActor_IsModelMissingOrFlag2000_v1 = 0x10084140u;
constexpr uint32_t CXiActor_ModelTest95680 = 0x10084350u;
constexpr uint32_t CXiActor_ModelTest956A0 = 0x10084370u;
constexpr uint32_t CXiActor_ModelTest956A0_v1 = 0x10084370u;
constexpr uint32_t CXiActor_GetModelField170 = 0x10084390u;
constexpr uint32_t CXiActor_GetModelField170_v1 = 0x10084390u;
constexpr uint32_t CXiActor_HasNonzeroField150or158 = 0x100843B0u;
constexpr uint32_t CXiActor_GetModelByteEE = 0x10084400u;
constexpr uint32_t CXiActor_GetModelByteEE_v1 = 0x10084400u;
constexpr uint32_t CXiActor_GetModelByteEF = 0x10084420u;
constexpr uint32_t CXiActor_GetModelPartWord = 0x10084480u;
constexpr uint32_t CXiActor_IsModelMissingOrSuppressed = 0x10084580u;
constexpr uint32_t CXiActor_IsModelMissingOrSuppressed_v1 = 0x10084580u;
constexpr uint32_t CXiActor_TestModelFlag128_8 = 0x100845B0u;
constexpr uint32_t CXiActor_GetSubModelResult = 0x100845E0u;
constexpr uint32_t FUN_10084620 = 0x10084620u;
constexpr uint32_t CXiActor_TestModelFlags100000_200000_4000 = 0x10084640u;
constexpr uint32_t CXiActor_SetField0B4 = 0x10085010u;
constexpr uint32_t CXiActor_SpawnResourceGenerators = 0x10085060u;
constexpr uint32_t CXiActor_SpawnResourceGenerators_v1 = 0x10085060u;
constexpr uint32_t FUN_10085240 = 0x10085240u;
constexpr uint32_t FUN_10085270 = 0x10085270u;
constexpr uint32_t FUN_100852d0 = 0x100852D0u;
constexpr uint32_t CXiActor_IsInModelSlotTable = 0x10085490u;
constexpr uint32_t CXiActor_IsInModelSlotTable_v1 = 0x10085490u;
constexpr uint32_t CXiActor_GetField88Bit3 = 0x10085500u;
constexpr uint32_t FUN_10085a10 = 0x10085A10u;
constexpr uint32_t FUN_10085a50 = 0x10085A50u;
constexpr uint32_t CMoAttachments_PushCasterLink = 0x10085D50u;
constexpr uint32_t CMoAttachments_PushTargetLink = 0x10085D60u;
constexpr uint32_t Actor_DrawWorldText = 0x10086210u;
constexpr uint32_t Actor_DrawWorldText_v1 = 0x10086210u;
constexpr uint32_t DrawNameplateSecondString = 0x10086900u;
constexpr uint32_t Unit10086E90_IsParamBelow901 = 0x10086FC0u;
constexpr uint32_t ActerBuff_FindByField78 = 0x10086FE0u;
constexpr uint32_t Unit10086E90_GetGlobalFlag = 0x10087010u;
constexpr uint32_t Acter_IsEvent = 0x10087060u;
constexpr uint32_t IsEvent = 0x10087070u;
constexpr uint32_t Menu_HasAnyFlag = 0x100870E0u;
constexpr uint32_t Unit10086E90_GetGlobalByte = 0x10087110u;
constexpr uint32_t thunk_FUN_10087230 = 0x10087220u;
constexpr uint32_t Help_UpdateFromFlags = 0x100872C0u;
constexpr uint32_t Unit10086E90_UpdateObject10578184 = 0x100873B0u;
constexpr uint32_t Unit10086E90_IsIdMatchGlobal = 0x100875D0u;
constexpr uint32_t Unit10086E90_GetMaskedFlagByte = 0x10087660u;
constexpr uint32_t Unit10086E90_IsGlobalNonzero = 0x10087670u;
constexpr uint32_t FUN_10087680 = 0x10087680u;
constexpr uint32_t Unit10086E90_GetDword10480800IfFlag = 0x10087690u;
constexpr uint32_t Unit10086E90_GetDword10480800IfFlag_v1 = 0x10087690u;
constexpr uint32_t Unit10086E90_GetByte10480824IfFlag = 0x100876B0u;
constexpr uint32_t FUN_100876d0 = 0x100876D0u;
constexpr uint32_t FUN_100876e0 = 0x100876E0u;
constexpr uint32_t Unit10086E90_GetTable10480828IfFlag = 0x10087760u;
constexpr uint32_t Unit10086E90_GetByte10480825IfSet = 0x10087830u;
constexpr uint32_t FUN_10087850 = 0x10087850u;
constexpr uint32_t FUN_10087870 = 0x10087870u;
constexpr uint32_t Unit10086E90_ApplyPair1EC1F4 = 0x10087D40u;
constexpr uint32_t CXiActor_GetTwoBitField = 0x10087DC0u;
constexpr uint32_t FUN_10087df0 = 0x10087DF0u;
constexpr uint32_t Acter_ResetMotionCounters = 0x10087E40u;
constexpr uint32_t Acter_AdvanceMotionTimer = 0x10087E80u;
constexpr uint32_t FUN_10087f00 = 0x10087F00u;
constexpr uint32_t FUN_10088270 = 0x10088270u;
constexpr uint32_t FUN_100882d0 = 0x100882D0u;
constexpr uint32_t FUN_10088320 = 0x10088320u;
constexpr uint32_t FUN_10088380 = 0x10088380u;
constexpr uint32_t Unit10086E90_UpdateByKind170NotGroup38 = 0x100883E0u;
constexpr uint32_t FUN_10088b20 = 0x10088B20u;
constexpr uint32_t FUN_10088b50 = 0x10088B50u;
constexpr uint32_t FUN_10088bb0 = 0x10088BB0u;
constexpr uint32_t FUN_10088bd0 = 0x10088BD0u;
constexpr uint32_t Unit10086E90_UpdateByKind170NotGroup06 = 0x10089100u;
constexpr uint32_t FUN_10089850 = 0x10089850u;
constexpr uint32_t FUN_10089ff0 = 0x10089FF0u;
constexpr uint32_t Camera_SetField30 = 0x1008A0E0u;
constexpr uint32_t CXiActor_Construct = 0x1008A120u;
constexpr uint32_t Actor_SetDefaultMotionTags = 0x1008A160u;
constexpr uint32_t CXiActor_SelectMotionTags = 0x1008A1A0u;
constexpr uint32_t CXiActor_PushTagsToObject = 0x1008A2F0u;
constexpr uint32_t CXiActor_InitDefaults = 0x1008A350u;
constexpr uint32_t CXiActor_Destroy = 0x1008A7B0u;
constexpr uint32_t ActerBuff_FindById = 0x1008A930u;
constexpr uint32_t FUN_1008abe0 = 0x1008ABE0u;
constexpr uint32_t FUN_1008af60 = 0x1008AF60u;
constexpr uint32_t Unit10086E90_StartAutoRunFromZoneMap = 0x1008B100u;
constexpr uint32_t FUN_1008b170 = 0x1008B170u;
constexpr uint32_t Actor_FindByNameEntry = 0x1008B270u;
constexpr uint32_t Actor_CompareNames = 0x1008B2B0u;
constexpr uint32_t Actor_FindByField78Entry = 0x1008B380u;
constexpr uint32_t Unit10086E90_LatchFlag13CBit22 = 0x1008B410u;
constexpr uint32_t FUN_1008b460 = 0x1008B460u;
constexpr uint32_t FUN_1008c480 = 0x1008C480u;
constexpr uint32_t FUN_1008c6c0 = 0x1008C6C0u;
constexpr uint32_t Unit10086E90_UpdateOnePart1008CD90 = 0x1008CD90u;
constexpr uint32_t FUN_1008d910 = 0x1008D910u;
constexpr uint32_t CXiActor_ResetRandomTimer = 0x1008D980u;
constexpr uint32_t CXiActor_TickRandomTimer = 0x1008DA00u;
constexpr uint32_t Unit10086E90_SetModelField78FromTable = 0x1008DB40u;
constexpr uint32_t CXiActor_IsIdAbove6ff = 0x1008E130u;
constexpr uint32_t CXiActor_IsField78HighZero = 0x1008E140u;
constexpr uint32_t CXiActor_IsFlag1d0Clear = 0x1008E150u;
constexpr uint32_t FUN_1008e180 = 0x1008E180u;
constexpr uint32_t FUN_1008e240 = 0x1008E240u;
constexpr uint32_t Actor_FindByField2a4 = 0x1008E260u;
constexpr uint32_t FUN_1008e290 = 0x1008E290u;
constexpr uint32_t FUN_1008e3f0 = 0x1008E3F0u;
constexpr uint32_t Unit10086E90_UpdateFlags120Bit23 = 0x1008E4D0u;
constexpr uint32_t Unit10086E90_PlayChorMotion = 0x1008E740u;
constexpr uint32_t FUN_1008e870 = 0x1008E870u;
constexpr uint32_t Acter_IsBytes1FFBothIn89NotEqual = 0x1008E8D0u;
constexpr uint32_t FUN_1008e920 = 0x1008E920u;
constexpr uint32_t Acter_UpdateOne = 0x1008E9A0u;
constexpr uint32_t Acter_ReleaseEventRecord = 0x1008EE30u;
constexpr uint32_t FUN_1008f1d0 = 0x1008F1D0u;
constexpr uint32_t FUN_1008f740 = 0x1008F740u;
constexpr uint32_t Acter_ReleaseSlotD0 = 0x100926C0u;
constexpr uint32_t Acter_ReleaseSlotCC = 0x10092710u;
constexpr uint32_t Acter_ReleaseTenSlots = 0x10092760u;
constexpr uint32_t Acter_ReleaseSlotByIndex = 0x10092780u;
constexpr uint32_t Acter_ReleaseSlotA8 = 0x10092800u;
constexpr uint32_t Acter_ReleaseSlotA4 = 0x10092870u;
constexpr uint32_t Acter_ReleaseAllSlots = 0x10092900u;
constexpr uint32_t Acter_CopyPosToCached = 0x100929A0u;
constexpr uint32_t Acter_SnapHeightToZoneMap = 0x100929D0u;
constexpr uint32_t FUN_10092c20 = 0x10092C20u;
constexpr uint32_t Acter_CopySixDwordsTo7C = 0x10093430u;
constexpr uint32_t FUN_100935c0 = 0x100935C0u;
constexpr uint32_t Unit10086E90_ResetGlobals = 0x10093720u;
constexpr uint32_t Acter_ClearGlobalBlocks = 0x10093A30u;
constexpr uint32_t Acter_ResetGlobalSentinels = 0x10093A90u;
constexpr uint32_t Acter_ClearGlobalBlock182 = 0x10093AD0u;
constexpr uint32_t Acter_ClearGlobalBlock2C = 0x10093AF0u;
constexpr uint32_t Acter_ClearGlobalBlock23 = 0x10093B10u;
constexpr uint32_t Acter_ClearGlobalBlock71 = 0x10093B30u;
constexpr uint32_t FUN_10093b50 = 0x10093B50u;
constexpr uint32_t Acter_ShutdownGlobals = 0x10093B70u;
constexpr uint32_t Acter_Nop = 0x10093DA0u;
constexpr uint32_t Net_SendPacket00COnce = 0x10093DB0u;
constexpr uint32_t Net_SendPacket00COnce_v1 = 0x10093DB0u;
constexpr uint32_t FUN_10093e30 = 0x10093E30u;
constexpr uint32_t FUN_10093e50 = 0x10093E50u;
constexpr uint32_t FUN_10094190 = 0x10094190u;
constexpr uint32_t FUN_10095660 = 0x10095660u;
constexpr uint32_t Acter_IsState5 = 0x10095680u;
constexpr uint32_t Acter_IsState55 = 0x100956A0u;
constexpr uint32_t Acter_IsStateGroup06Or38 = 0x100956C0u;
constexpr uint32_t Acter_IsStateGroup06 = 0x100956F0u;
constexpr uint32_t Acter_IsStateGroup38 = 0x10095750u;
constexpr uint32_t Acter_IsStateGroup3F = 0x10095790u;
constexpr uint32_t Unit10086E90_CanClearPendingState = 0x10095850u;
constexpr uint32_t Acter_UpdateAllLists = 0x10095900u;
constexpr uint32_t FUN_10095d80 = 0x10095D80u;
constexpr uint32_t FUN_10095da0 = 0x10095DA0u;
constexpr uint32_t Acter_ClearPendingState = 0x10095DD0u;
constexpr uint32_t Acter_ClearPending272 = 0x10095E30u;
constexpr uint32_t Acter_UpdateActionState = 0x10095E90u;
constexpr uint32_t FUN_10096040 = 0x10096040u;
constexpr uint32_t FUN_100960f0 = 0x100960F0u;
constexpr uint32_t FUN_100961d0 = 0x100961D0u;
constexpr uint32_t FUN_10096200 = 0x10096200u;
constexpr uint32_t Acter_UpdateFrame = 0x10096640u;
constexpr uint32_t FUN_10096c90 = 0x10096C90u;
constexpr uint32_t FUN_100973b0 = 0x100973B0u;
constexpr uint32_t FUN_10097500 = 0x10097500u;
constexpr uint32_t FUN_10097730 = 0x10097730u;
constexpr uint32_t FUN_100977a0 = 0x100977A0u;
constexpr uint32_t FUN_100977f0 = 0x100977F0u;
constexpr uint32_t Entity_BuildDisplayName = 0x10097840u;
constexpr uint32_t FUN_100979b0 = 0x100979B0u;
constexpr uint32_t FUN_10097a60 = 0x10097A60u;
constexpr uint32_t FUN_10097af0 = 0x10097AF0u;
constexpr uint32_t Unit10086E90_IsFlag40DB6OrState17 = 0x10097D50u;
constexpr uint32_t Acter_TestFlagTable = 0x10097E50u;
constexpr uint32_t Net_BuildPacket61 = 0x10097FD0u;
constexpr uint32_t Acter_IsSpecialIdRange = 0x10098180u;
constexpr uint32_t FUN_10098360 = 0x10098360u;
constexpr uint32_t Acter_GetPartnerIfKind = 0x100983A0u;
constexpr uint32_t FUN_100983f0 = 0x100983F0u;
constexpr uint32_t Unit10086E90_GetTable10485830IfBit2 = 0x100991F0u;
constexpr uint32_t Unit10086E90_ResolveEntryNames = 0x10099730u;
constexpr uint32_t Unit10086E90_UpdateFrame997B0 = 0x100997B0u;
constexpr uint32_t Acter_IsByte123Mask7In234 = 0x1009AAF0u;
constexpr uint32_t Acter_SetFieldsAt134And138 = 0x1009ADE0u;
constexpr uint32_t Acter_GetField138Clamped = 0x1009AEC0u;
constexpr uint32_t Acter_SetFlagBits138 = 0x1009AF60u;
constexpr uint32_t FUN_1009f2f0 = 0x1009F2F0u;
constexpr uint32_t Acter_StepAfterimageStates = 0x1009F4C0u;
constexpr uint32_t Unit10086E90_FindStringById = 0x100A1770u;
constexpr uint32_t FUN_100a2720 = 0x100A2720u;
constexpr uint32_t CMoTask_VT_1032D6D4_Destructor = 0x100A2810u;
constexpr uint32_t Unit100A2810_ResetRecords = 0x100A3CD0u;
constexpr uint32_t FUN_100a3d00 = 0x100A3D00u;
constexpr uint32_t CXiCollisionActor_Constructor = 0x100A4C20u;
constexpr uint32_t CXiControlActor_FillHistory = 0x100A5770u;
constexpr uint32_t CXiControlActor_SetVec4 = 0x100A57C0u;
constexpr uint32_t XiControlActor_UpdateCamera = 0x100A5E10u;
constexpr uint32_t XiControlActor_UpdateView = 0x100A5EF0u;
constexpr uint32_t XiControlActor_SetAutoRunning = 0x100A6070u;
constexpr uint32_t FUN_100a6240 = 0x100A6240u;
constexpr uint32_t CXiControlActor_Constructor = 0x100A89E0u;
constexpr uint32_t CXiControlActor_Destructor = 0x100A8CA0u;
constexpr uint32_t FUN_100a8cc0 = 0x100A8CC0u;
constexpr uint32_t CMoTask_VT_1032E808_Destructor = 0x100A9D30u;
constexpr uint32_t CMoTask_VT_1032E808_ScalarDeletingDtor = 0x100A9DB0u;
constexpr uint32_t FUN_100aafb0 = 0x100AAFB0u;
constexpr uint32_t FUN_100ab320 = 0x100AB320u;
constexpr uint32_t Unit100AB450_ReleaseSlot = 0x100AB550u;
constexpr uint32_t CXiDancerActor_InitBind = 0x100AB750u;
constexpr uint32_t CXiDancerActor_SetPosition = 0x100AB830u;
constexpr uint32_t FUN_100ab880 = 0x100AB880u;
constexpr uint32_t CXiDancerActor_ApplyStateBlock = 0x100AB9A0u;
constexpr uint32_t Unit100ABC20_CopyReturnZ = 0x100ABC70u;
constexpr uint32_t FUN_100ae3f0 = 0x100AE3F0u;
constexpr uint32_t FUN_100ae960 = 0x100AE960u;
constexpr uint32_t FUN_100aea40 = 0x100AEA40u;
constexpr uint32_t FUN_100aec70 = 0x100AEC70u;
constexpr uint32_t FUN_100aed00 = 0x100AED00u;
constexpr uint32_t FUN_100b37a0 = 0x100B37A0u;
constexpr uint32_t XiEvent_EventIdle = 0x100BCD10u;
constexpr uint32_t FUN_100bd0e0 = 0x100BD0E0u;
constexpr uint32_t XiEvent_RestoreActer = 0x100BD5F0u;
constexpr uint32_t FUN_100bd820 = 0x100BD820u;
constexpr uint32_t FUN_100c2d00 = 0x100C2D00u;
constexpr uint32_t XiEvent_Init = 0x100C2E20u;
constexpr uint32_t FUN_100c2fb0 = 0x100C2FB0u;
constexpr uint32_t FUN_100c2fe0 = 0x100C2FE0u;
constexpr uint32_t Unit100ACAB0_GetWordBFE = 0x100C3020u;
constexpr uint32_t Unit100ACAB0_GetWordC00 = 0x100C3030u;
constexpr uint32_t Unit100ACAB0_FreeData = 0x100C3050u;
constexpr uint32_t Unit100ACAB0_Init = 0x100C3060u;
constexpr uint32_t FUN_100c30f0 = 0x100C30F0u;
constexpr uint32_t FUN_100c3100 = 0x100C3100u;
constexpr uint32_t FUN_100c3120 = 0x100C3120u;
constexpr uint32_t FUN_100c3130 = 0x100C3130u;
constexpr uint32_t FUN_100c3140 = 0x100C3140u;
constexpr uint32_t FUN_100c3150 = 0x100C3150u;
constexpr uint32_t FUN_100c3160 = 0x100C3160u;
constexpr uint32_t FUN_100c3170 = 0x100C3170u;
constexpr uint32_t Unit100ACAB0_GetSingleton = 0x100C3190u;
constexpr uint32_t FUN_100c31f0 = 0x100C31F0u;
constexpr uint32_t Unit100ACAB0_SetFlag8 = 0x100C3200u;
constexpr uint32_t Unit100ACAB0_HasItems = 0x100C3210u;
constexpr uint32_t Unit100ACAB0_Clear = 0x100C3220u;
constexpr uint32_t Unit100ACAB0_Append = 0x100C3260u;
constexpr uint32_t Unit100ACAB0_AddEvent = 0x100C3300u;
constexpr uint32_t XiFurnitureActor_Release = 0x100C3E70u;
constexpr uint32_t FUN_100c4400 = 0x100C4400u;
constexpr uint32_t FUN_100c4bc0 = 0x100C4BC0u;
constexpr uint32_t Unit100C51C0_MapCodeIndex = 0x100C51C0u;
constexpr uint32_t Unit100C51C0_AddOffset996B = 0x100C5260u;
constexpr uint32_t Unit100C51C0_AddOffset19131 = 0x100C5270u;
constexpr uint32_t Unit100C51C0_AddOffset1889B = 0x100C5280u;
constexpr uint32_t Unit100C51C0_ForwardToCce00 = 0x100C5290u;
constexpr uint32_t CXiSkeletonActor_Constructor_1Arg = 0x100C52A0u;
constexpr uint32_t TargetActor_SlotHandler1 = 0x100C5410u;
constexpr uint32_t TargetActor_SlotHandler0 = 0x100C5440u;
constexpr uint32_t TargetActor_SlotHandler4 = 0x100C54C0u;
constexpr uint32_t CXiSkeletonActor_DestructorBody = 0x100C5C00u;
constexpr uint32_t CXiSkeletonActor_Init = 0x100C5D60u;
constexpr uint32_t Actor_SelectNearestLights = 0x100CB4E0u;
constexpr uint32_t PerActor_FourBucketStaggeredUpdate = 0x100CBDE0u;
constexpr uint32_t nullsub_276 = 0x100CCE20u;
constexpr uint32_t nullsub_277 = 0x100CCE30u;
constexpr uint32_t nullsub_278 = 0x100CCE40u;
constexpr uint32_t nullsub_279 = 0x100CCE50u;
constexpr uint32_t Actor_SetCameraFromModelPoints = 0x100CCE60u;
constexpr uint32_t CXiSkeletonActor_SetSlotMotion = 0x100CD4E0u;
constexpr uint32_t CXiSkeletonActor_SetMotion = 0x100CD540u;
constexpr uint32_t FUN_100ce0e0 = 0x100CE0E0u;
constexpr uint32_t CXiSkeletonActor_AdvanceMotionQueue = 0x100CE160u;
constexpr uint32_t CXiSkeletonActor_ReverseFindRes = 0x100CE480u;
constexpr uint32_t FUN_100cf060 = 0x100CF060u;
constexpr uint32_t FUN_100cf0b0 = 0x100CF0B0u;
constexpr uint32_t CXiSkeletonActor_ForParts0To9 = 0x100CF100u;
constexpr uint32_t FUN_100cf160 = 0x100CF160u;
constexpr uint32_t FUN_100cf750 = 0x100CF750u;
constexpr uint32_t CXiSkeletonActor_GetTableEntryByModelKind = 0x100CFB40u;
constexpr uint32_t FUN_100d0b50 = 0x100D0B50u;
constexpr uint32_t FUN_100d0e80 = 0x100D0E80u;
constexpr uint32_t CXiSkeletonActor_SelectStateMessage = 0x100D0FC0u;
constexpr uint32_t CXiSkeletonActor_IsReadCompleteBody = 0x100D12D0u;
constexpr uint32_t FUN_100d1450 = 0x100D1450u;
constexpr uint32_t FUN_100d2800 = 0x100D2800u;
constexpr uint32_t FUN_100d2d70 = 0x100D2D70u;
constexpr uint32_t FUN_100d2e30 = 0x100D2E30u;
constexpr uint32_t CXiSkeletonActor_OnSkeletonLoaded = 0x100D2F40u;
constexpr uint32_t CXiSkeletonActor_LoadSkeletonSync = 0x100D2F60u;
constexpr uint32_t CXiSkeletonActor_LoadSkeletonResource = 0x100D4330u;
constexpr uint32_t CXiSkeletonActor_LoadSkeletonResource_v1 = 0x100D4330u;
constexpr uint32_t CXiSkeletonActor_AddListEntry = 0x100D4410u;
constexpr uint32_t CXiSkeletonActor_ReleaseListEntriesMatching = 0x100D44B0u;
constexpr uint32_t CXiSkeletonActor_DeleteOwnedList = 0x100D44F0u;
constexpr uint32_t FUN_100d4520 = 0x100D4520u;
constexpr uint32_t FUN_100d5430 = 0x100D5430u;
constexpr uint32_t sub_100D5430 = 0x100D5430u;
constexpr uint32_t CXiSkeletonActor_CalcGroundTiltMatrix = 0x100D54C0u;
constexpr uint32_t CXiSkeletonActor_UpdateHeadLook = 0x100D5B10u;
constexpr uint32_t FUN_100d5fe0 = 0x100D5FE0u;
constexpr uint32_t FUN_100d6180 = 0x100D6180u;
constexpr uint32_t CXiSkeletonActor_FindPartValue = 0x100D6240u;
constexpr uint32_t CXiSkeletonActor_FindPartValue_v1 = 0x100D6240u;
constexpr uint32_t FUN_100d65b0 = 0x100D65B0u;
constexpr uint32_t Actor_PrepareDrawGlobals = 0x100D6D30u;
constexpr uint32_t CXiSkeletonActorRes_Constructor = 0x100D7020u;
constexpr uint32_t CXiSkeletonActorRes_Destructor = 0x100D7050u;
constexpr uint32_t CXiSkeletonActorRes_GetLinkedActor = 0x100D7090u;
constexpr uint32_t CXiSkeletonActorRes_GetLinkedActor_v1 = 0x100D7090u;
constexpr uint32_t Unit100D7420_Md5Init = 0x100D7420u;
constexpr uint32_t Unit100D7420_Md5Update = 0x100D7450u;
constexpr uint32_t Unit100D7420_PadAndAppendLength = 0x100D7510u;
constexpr uint32_t Unit100D7420_FinalDigest = 0x100D7560u;
constexpr uint32_t Unit100D7420_Md5ComputeBlock = 0x100D75A0u;
constexpr uint32_t Unit100D7420_AddOne = 0x100D7E90u;
constexpr uint32_t Unit100D7420_HashStringWithSeed = 0x100D7EA0u;
constexpr uint32_t Unit100D7420_HashBytesWithSeed = 0x100D7ED0u;
constexpr uint32_t FUN_100d8490 = 0x100D8490u;
constexpr uint32_t Mutex_WriteRecordLocked = 0x100D8A20u;
constexpr uint32_t FUN_100d93e0 = 0x100D93E0u;
constexpr uint32_t Game_InitGlobal = 0x100D9410u;
constexpr uint32_t Unit100D7420_SetGlobalBlock1048BEB8 = 0x100D9430u;
constexpr uint32_t Unit100D7420_Return0_100D9460 = 0x100D9460u;
constexpr uint32_t Game_ResetGlobals = 0x100D9830u;
constexpr uint32_t Game_InitOnce = 0x100D9950u;
constexpr uint32_t Unit100D7420_GetEntryMod1066268C = 0x100D9B00u;
constexpr uint32_t Game_InitSceneState = 0x100DA6A0u;
constexpr uint32_t FUN_100daf30 = 0x100DAF30u;
constexpr uint32_t GameMode_PollPendingState = 0x100DB140u;
constexpr uint32_t GameMode_PollPendingState_v1 = 0x100DB140u;
constexpr uint32_t FUN_100db170 = 0x100DB170u;
constexpr uint32_t GameMode_RequestState = 0x100DB170u;
constexpr uint32_t Unit100D7420_GetRecordWordFwd = 0x100DB780u;
constexpr uint32_t Unit100D7420_GetRecordDwordFwd = 0x100DB790u;
constexpr uint32_t Unit100D7420_GetRecordDwordFwd_v1 = 0x100DB790u;
constexpr uint32_t Unit100D7420_LookupNameIfKind3 = 0x100DB7B0u;
constexpr uint32_t Unit100D7420_GetWord40D7C = 0x100DB930u;
constexpr uint32_t Game_IsFlag10492DB4 = 0x100DBCA0u;
constexpr uint32_t Unit100D7420_ReturnTrue100DBDE0 = 0x100DBDE0u;
constexpr uint32_t Time_GetMilliseconds = 0x100DBE00u;
constexpr uint32_t FUN_100dbe10 = 0x100DBE10u;
constexpr uint32_t Unit100D7420_GetTimeNow = 0x100DBE30u;
constexpr uint32_t Unit100D7420_InitTimer = 0x100DBE50u;
constexpr uint32_t Unit100D7420_Sync = 0x100DBEB0u;
constexpr uint32_t Unit100D7420_DiffWrapped = 0x100DBF00u;
constexpr uint32_t Unit100D7420_AdvanceClock = 0x100DBF20u;
constexpr uint32_t Unit100D7420_CheckSlotExpired = 0x100DBFE0u;
constexpr uint32_t Unit100D7420_GetTotalMilliseconds = 0x100DC060u;
constexpr uint32_t Unit100D7420_GetSeconds = 0x100DC080u;
constexpr uint32_t Unit100D7420_GetMilliseconds = 0x100DC090u;
constexpr uint32_t Unit100D7420_AdvanceAndGetMs = 0x100DC120u;
constexpr uint32_t Unit100D7420_AdvanceAndGetSeconds = 0x100DC130u;
constexpr uint32_t Unit100D7420_SetOffsetMilliseconds = 0x100DC1B0u;
constexpr uint32_t Unit100D7420_Reset = 0x100DC1F0u;
constexpr uint32_t Unit100D7420_Update = 0x100DC210u;
constexpr uint32_t Unit100D7420_GetWrapCount = 0x100DC250u;
constexpr uint32_t Unit100D7420_GetLastTick = 0x100DC260u;
constexpr uint32_t Unit100D7420_InitContext = 0x100DC270u;
constexpr uint32_t Unit100D7420_IsInitialized = 0x100DC2C0u;
constexpr uint32_t FUN_100dc2d0 = 0x100DC2D0u;
constexpr uint32_t FUN_100dc3b0 = 0x100DC3B0u;
constexpr uint32_t Unit100D7420_GetContextField1AFC0 = 0x100DC420u;
constexpr uint32_t FUN_100dc650 = 0x100DC650u;
constexpr uint32_t Unit100D7420_EncryptBlocks = 0x100DCA00u;
constexpr uint32_t Unit100D7420_DecryptBlocks = 0x100DCF60u;
constexpr uint32_t Unit100D7420_HashPair = 0x100DD800u;
constexpr uint32_t Unit100D7420_QueueFindOrAllocSlot = 0x100DDA60u;
constexpr uint32_t Unit100D7420_SlotSetLength = 0x100DDD20u;
constexpr uint32_t Unit100D7420_Md5InitCountFirst = 0x100DDE90u;
constexpr uint32_t Unit100D7420_Md5UpdateCountFirst = 0x100DDEC0u;
constexpr uint32_t Unit100D7420_Md5FinalCountFirst = 0x100DDF60u;
constexpr uint32_t Unit100D7420_Transform = 0x100DE010u;
constexpr uint32_t Unit100D7420_Decode = 0x100DF5D0u;
constexpr uint32_t Unit100D7420_CopyBits = 0x100DF6E0u;
constexpr uint32_t Unit100D7420_EstimateBytes = 0x100DF780u;
constexpr uint32_t Unit100D7420_Encode = 0x100DF7C0u;
constexpr uint32_t Unit100D7420_ReturnTrue100E0100 = 0x100E0100u;
constexpr uint32_t Entry_FindByNormalizedName = 0x100E08D0u;
constexpr uint32_t Name_Normalize = 0x100E09A0u;
constexpr uint32_t Unit100D7420_ClearFlag1A698 = 0x100E0A00u;
constexpr uint32_t Unit100D7420_FindEntryByName = 0x100E0A50u;
constexpr uint32_t Unit100D7420_SendRequest3CThrottled = 0x100E0AA0u;
constexpr uint32_t Unit100D7420_NoOp100E0B10 = 0x100E0B10u;
constexpr uint32_t Unit100D7420_ApplyNoticeRecord = 0x100E0D50u;
constexpr uint32_t Entry_ResetFields = 0x100E13C0u;
constexpr uint32_t Unit100D7420_AllocEntry4 = 0x100E19C0u;
constexpr uint32_t Unit100D7420_AllocEntryFromTable = 0x100E1AD0u;
constexpr uint32_t Unit100D7420_CountActive = 0x100E1B50u;
constexpr uint32_t Unit100D7420_ClearMatching = 0x100E1C00u;
constexpr uint32_t Unit100D7420_FindFreeHandlerSlot = 0x100E1C30u;
constexpr uint32_t Unit100D7420_RegisterHandler1369C = 0x100E2E70u;
constexpr uint32_t Unit100D7420_RegisterHandler1369CFlag1 = 0x100E2EE0u;
constexpr uint32_t Unit100D7420_ReturnTrue100E33F0 = 0x100E33F0u;
constexpr uint32_t Unit100D7420_ReturnTrue100E4110 = 0x100E4110u;
constexpr uint32_t Unit100D7420_ReturnTrue100E4330 = 0x100E4330u;
constexpr uint32_t Unit100D7420_ResetObject2D000 = 0x100E43F0u;
constexpr uint32_t Unit100D7420_GetBlock2CFD8 = 0x100E47A0u;
constexpr uint32_t Unit100D7420_SetGlobal104B1774 = 0x100E5E30u;
constexpr uint32_t Unit100D7420_ReturnTrue100E5FF0 = 0x100E5FF0u;
constexpr uint32_t Unit100D7420_GetBlock2CDE8IfReady = 0x100E6020u;
constexpr uint32_t FUN_100e6080 = 0x100E6080u;
constexpr uint32_t Unit100D7420_ReturnTrue100E61C0 = 0x100E61C0u;
constexpr uint32_t Unit100D7420_SetWord130 = 0x100E62C0u;
constexpr uint32_t Unit100D7420_ClearBit0Slot = 0x100E6370u;
constexpr uint32_t Unit100D7420_ClearBit1Fc = 0x100E63C0u;
constexpr uint32_t Unit100D7420_ScanEntriesAndInit = 0x100E63E0u;
constexpr uint32_t Unit100D7420_InitEncrypt = 0x100E6650u;
constexpr uint32_t Unit100D7420_IsBeforeDeadline = 0x100E6790u;
constexpr uint32_t Unit100D7420_IsCountOrByte134Set = 0x100E67E0u;
constexpr uint32_t Unit100D7420_ClearTimer140 = 0x100E6810u;
constexpr uint32_t Unit100D7420_ScanIfTimer140Set = 0x100E6830u;
constexpr uint32_t Unit100D7420_IsTimer140Clear = 0x100E6860u;
constexpr uint32_t Unit100D7420_SetDuration138 = 0x100E6880u;
constexpr uint32_t Unit100D7420_GetWord132 = 0x100E6B10u;
constexpr uint32_t Unit100D7420_BuildListA90 = 0x100E6B20u;
constexpr uint32_t Unit100D7420_GetEntryPtrA90 = 0x100E6C40u;
constexpr uint32_t Unit100D7420_ClearPending = 0x100E7260u;
constexpr uint32_t Unit100D7420_ThunkByIndex200 = 0x100E7B20u;
constexpr uint32_t Unit100D7420_ReturnTrue100E84B0 = 0x100E84B0u;
constexpr uint32_t Unit100D7420_ReturnTrue100E8B70 = 0x100E8B70u;
constexpr uint32_t Unit100D7420_SetByte9800AndGetEntry = 0x100E8B80u;
constexpr uint32_t Unit100D7420_SelectNext = 0x100E8C70u;
constexpr uint32_t Unit100D7420_SendByte76 = 0x100E9550u;
constexpr uint32_t Unit100D7420_ReturnTrue100EB0A0 = 0x100EB0A0u;
constexpr uint32_t Unit100D7420_GetSlotPtr = 0x100EB490u;
constexpr uint32_t Unit100D7420_CountFreeEntries = 0x100EB8D0u;
constexpr uint32_t Unit100D7420_GetEntryCount = 0x100EB940u;
constexpr uint32_t Unit100D7420_SendDwordPacket = 0x100EBF40u;
constexpr uint32_t Unit100D7420_FindOrRemove = 0x100EBF80u;
constexpr uint32_t Unit100D7420_RemoveRecord = 0x100EC060u;
constexpr uint32_t Unit100D7420_ResetCounter = 0x100EDEA0u;
constexpr uint32_t Unit100D7420_Advance = 0x100EDEB0u;
constexpr uint32_t FUN_100ef1a0 = 0x100EF1A0u;
constexpr uint32_t Unit100D7420_GetRecordWord = 0x100EFC70u;
constexpr uint32_t Unit100D7420_GetRecordDword = 0x100EFCA0u;
constexpr uint32_t Name_LookupByKey = 0x100EFCE0u;
constexpr uint32_t Unit100D7420_FindMatchingEntry = 0x100EFDF0u;
constexpr uint32_t Unit100D7420_ReturnTrue100F0090 = 0x100F0090u;
constexpr uint32_t FUN_100f0130 = 0x100F0130u;
constexpr uint32_t Unit100D7420_ReturnTrue100F02A0 = 0x100F02A0u;
constexpr uint32_t Unit100D7420_SetByte15110 = 0x100F04F0u;
constexpr uint32_t Unit100D7420_SetByte15114 = 0x100F0550u;
constexpr uint32_t Unit100D7420_ReturnTrue100F06B0 = 0x100F06B0u;
constexpr uint32_t Unit100D7420_ReturnTrue100F09C0 = 0x100F09C0u;
constexpr uint32_t Unit100D7420_GetFlagsMasked = 0x100F0C50u;
constexpr uint32_t Unit100D7420_ClearRecord = 0x100F10C0u;
constexpr uint32_t Unit100D7420_UpdateActive = 0x100F10F0u;
constexpr uint32_t Unit100D7420_StepBitGuardedTask = 0x100F1160u;
constexpr uint32_t Unit100D7420_AcquireIfFree = 0x100F1280u;
constexpr uint32_t Unit100D7420_AllocRecord = 0x100F1300u;
constexpr uint32_t Unit100D7420_InitEntry = 0x100F1340u;
constexpr uint32_t Unit100D7420_IsBitSet = 0x100F13B0u;
constexpr uint32_t Unit100D7420_SetBit = 0x100F13D0u;
constexpr uint32_t Unit100D7420_ClearBit = 0x100F13F0u;
constexpr uint32_t FUN_100f1410 = 0x100F1410u;
constexpr uint32_t thunk_FUN_1030ff33 = 0x100F1770u;
constexpr uint32_t thunk_FUN_1030ff49 = 0x100F1780u;
constexpr uint32_t Unit100D7420_Call10310067 = 0x100F1870u;
constexpr uint32_t Unit100D7420_Call10310072 = 0x100F1900u;
constexpr uint32_t Unit100D7420_AcquireNode = 0x100F1B20u;
constexpr uint32_t Unit100D7420_InitTwoNodes = 0x100F1BA0u;
constexpr uint32_t Unit100D7420_Destroy = 0x100F1C40u;
constexpr uint32_t Unit100D7420_UpdateRequests = 0x100F1D20u;
constexpr uint32_t Unit100D7420_ResetPending = 0x100F1E70u;
constexpr uint32_t Unit100D7420_BeginRequests = 0x100F1F00u;
constexpr uint32_t Unit100D7420_StartRequestSlot20 = 0x100F25F0u;
constexpr uint32_t Unit100D7420_StartRequestOnNewNodeA = 0x100F2690u;
constexpr uint32_t Unit100D7420_StartRequestOnNewNodeB = 0x100F2720u;
constexpr uint32_t Unit100D7420_StartRequestSlot24 = 0x100F2760u;
constexpr uint32_t Unit100D7420_ClearNode = 0x100F29E0u;
constexpr uint32_t FUN_100f2a40 = 0x100F2A40u;
constexpr uint32_t Unit100D7420_DestroyNode = 0x100F2A40u;
constexpr uint32_t FUN_100f2a70 = 0x100F2A70u;
constexpr uint32_t Unit100D7420_Complete = 0x100F2B80u;
constexpr uint32_t Unit100D7420_CallStateCallback3 = 0x100F2C40u;
constexpr uint32_t Unit100D7420_RunCallbacks = 0x100F2C80u;
constexpr uint32_t Unit100D7420_CopyBounded = 0x100F3010u;
constexpr uint32_t Unit100D7420_StartCallbacks = 0x100F32B0u;
constexpr uint32_t Unit100D7420_InitRequestA = 0x100F3BB0u;
constexpr uint32_t Unit100D7420_InitRequestB = 0x100F3C20u;
constexpr uint32_t Unit100D7420_InitRequestC = 0x100F3C90u;
constexpr uint32_t Unit100D7420_InitRequestD = 0x100F3D00u;
constexpr uint32_t Unit100D7420_EnsureGlobal = 0x100F76B0u;
constexpr uint32_t Unit100D7420_DestroyGlobal = 0x100F76D0u;
constexpr uint32_t Unit100D7420_UpdateGlobalRequests = 0x100F7700u;
constexpr uint32_t FUN_100f78c0 = 0x100F78C0u;
constexpr uint32_t Unit100D7420_ReturnTrue_100F8920 = 0x100F8920u;
constexpr uint32_t Unit100D7420_ReturnTrue_100F8CB0 = 0x100F8CB0u;
constexpr uint32_t Unit100D7420_ReturnTrue_100F9350 = 0x100F9350u;
constexpr uint32_t Unit100D7420_GetActiveBlock3AE98 = 0x100F94D0u;
constexpr uint32_t Unit100D7420_TickSecondCounters = 0x100F9CC0u;
constexpr uint32_t Unit100D7420_UpdateSession = 0x100F9D50u;
constexpr uint32_t Unit100D7420_ClearSessionGlobals = 0x100F9DA0u;
constexpr uint32_t Unit100D7420_ShutdownSessions = 0x100F9DB0u;
constexpr uint32_t Unit100D7420_ShutdownIfActive = 0x100F9DF0u;
constexpr uint32_t Unit100D7420_ShutdownEntry = 0x100F9E10u;
constexpr uint32_t FUN_100fa180 = 0x100FA180u;
constexpr uint32_t FUN_100fa4a0 = 0x100FA4A0u;
constexpr uint32_t Unit100D7420_SetPacketType = 0x100FA940u;
constexpr uint32_t FUN_100fc0f0 = 0x100FC0F0u;
constexpr uint32_t FUN_100fc140 = 0x100FC140u;
constexpr uint32_t Unit100D7420_GetFlags = 0x100FC160u;
constexpr uint32_t Unit100D7420_ApplyStatusText = 0x100FC180u;
constexpr uint32_t FUN_100fc2b0 = 0x100FC2B0u;
constexpr uint32_t FUN_100fc2e0 = 0x100FC2E0u;
constexpr uint32_t FUN_100fc300 = 0x100FC300u;
constexpr uint32_t Unit100D7420_SetWord40DAA = 0x100FC320u;
constexpr uint32_t thunk_FUN_100fc370 = 0x100FC3B0u;
constexpr uint32_t Unit100D7420_CheckTimer41B4 = 0x100FC3C0u;
constexpr uint32_t Unit100D7420_SendRaw = 0x100FC4D0u;
constexpr uint32_t Unit100D7420_GetIdPair = 0x100FC510u;
constexpr uint32_t Unit100D7420_SendTypeF2 = 0x100FC680u;
constexpr uint32_t Unit100D7420_SetPerMille = 0x100FCA20u;
constexpr uint32_t Unit100D7420_SetPerMilleAt4 = 0x100FCA40u;
constexpr uint32_t Unit100D7420_AdjustLevel = 0x100FCA60u;
constexpr uint32_t Unit100D7420_FindTimerKind10 = 0x100FCD70u;
constexpr uint32_t Unit100D7420_GetTableByteForTens = 0x10101BC0u;
constexpr uint32_t Unit100D7420_CeilDiv10Max10 = 0x10101BF0u;
constexpr uint32_t ntGameInit = 0x10102340u;
constexpr uint32_t FUN_10103720 = 0x10103720u;
constexpr uint32_t Game_ClearFlagsBits = 0x10103740u;
constexpr uint32_t Game_SetSlot44C = 0x101037F0u;
constexpr uint32_t Unit100D7420_ReturnZero = 0x101038C0u;
constexpr uint32_t Game_GetField4 = 0x101038D0u;
constexpr uint32_t Unit100D7420_GetWord8Of104E0AF4 = 0x101038E0u;
constexpr uint32_t ntGameTimeGet = 0x101039E0u;
constexpr uint32_t Time_ModDiv6C00Scaled = 0x10103AE0u;
constexpr uint32_t Time_ModDivD80Div90 = 0x10103B10u;
constexpr uint32_t Time_ModMul3CDiv90 = 0x10103B30u;
constexpr uint32_t FUN_10103b60 = 0x10103B60u;
constexpr uint32_t Unit100D7420_SetZero = 0x10103F50u;
constexpr uint32_t Unit100D7420_AppendMax20 = 0x10107610u;
constexpr uint32_t Unit100D7420_DumpHexBytes = 0x10108150u;
constexpr uint32_t Unit100D7420_InitDumpTable = 0x10108300u;
constexpr uint32_t Unit100D7420_GetPoolBlockPayload = 0x101083C0u;
constexpr uint32_t FUN_10108460 = 0x10108460u;
constexpr uint32_t Unit100D7420_AllocEntry = 0x10108590u;
constexpr uint32_t Unit100D7420_AllocFromPool = 0x101085C0u;
constexpr uint32_t Unit100D7420_AllocFromPool0 = 0x101085E0u;
constexpr uint32_t thunk_FUN_10108600 = 0x101086B0u;
constexpr uint32_t Unit100D7420_InsertBlock = 0x10108700u;
constexpr uint32_t Unit100D7420_InsertAndCoalesce = 0x10108780u;
constexpr uint32_t Unit100D7420_GetNextBlock = 0x10108870u;
constexpr uint32_t Unit100D7420_GetPrevBlock = 0x101088A0u;
constexpr uint32_t Unit100D7420_RebalanceList = 0x101088C0u;
constexpr uint32_t Unit100D7420_UnlinkBlock = 0x10108D80u;
constexpr uint32_t Unit100D7420_LinkAfter = 0x10108E80u;
constexpr uint32_t Unit100D7420_LinkBefore = 0x10108F40u;
constexpr uint32_t Unit100D7420_AllocCopy = 0x10109000u;
constexpr uint32_t Unit100D7420_Release = 0x101090A0u;
constexpr uint32_t Unit100D7420_RefStringClear = 0x10109100u;
constexpr uint32_t Unit100D7420_RefStringCtor = 0x10109140u;
constexpr uint32_t Unit100D7420_RefStringReserve = 0x101091D0u;
constexpr uint32_t Unit100D7420_RefStringAssign = 0x101092C0u;
constexpr uint32_t Unit100D7420_RefStringRelease = 0x101094D0u;
constexpr uint32_t Unit100D7420_Copy12 = 0x1010E040u;
constexpr uint32_t Unit100D7420_CreateEventRecorder = 0x1010F8B0u;
constexpr uint32_t Unit100D7420_RecorderAddHandler = 0x1010FA30u;
constexpr uint32_t Unit100D7420_RecorderRecordCode = 0x1010FBD0u;
constexpr uint32_t Unit100D7420_RegisterHandlerByName = 0x1010FC50u;
constexpr uint32_t FUN_1010feb0 = 0x1010FEB0u;
constexpr uint32_t Unit100D7420_ForwardToGlobal1708 = 0x1010FEB0u;
constexpr uint32_t Unit100D7420_Copy12B = 0x1010FEE0u;
constexpr uint32_t Unit100D7420_ListInsertBefore = 0x10110040u;
constexpr uint32_t Unit100D7420_ResetB44 = 0x10113220u;
constexpr uint32_t Unit100D7420_ClearTable1710 = 0x101133A0u;
constexpr uint32_t FUN_10113ca0 = 0x10113CA0u;
constexpr uint32_t FDtXiErrorReportCtrl_Constructor = 0x10113D30u;
constexpr uint32_t Unit100D7420_ErrorLogDtor = 0x10114070u;
constexpr uint32_t Unit100D7420_ErrorLogPop = 0x10114120u;
constexpr uint32_t FUN_10114250 = 0x10114250u;
constexpr uint32_t Unit100D7420_ErrorLogFlush = 0x10114320u;
constexpr uint32_t Unit100D7420_ErrorLogReplay = 0x10114500u;
constexpr uint32_t Unit100D7420_ReportError = 0x10114F10u;
constexpr uint32_t FDtGmNoticeProc = 0x10115020u;
constexpr uint32_t Unit100D7420_CreateSession = 0x101152F0u;
constexpr uint32_t Unit100D7420_CreateSessionFromParams = 0x101154D0u;
constexpr uint32_t Unit100D7420_StartChannel = 0x10115520u;
constexpr uint32_t Unit100D7420_QueueRequest26 = 0x10115580u;
constexpr uint32_t Unit100D7420_GetField13C = 0x101164B0u;
constexpr uint32_t Unit100D7420_GetVerifiedSize = 0x10116520u;
constexpr uint32_t Unit100D7420_VerifyMd4 = 0x10116540u;
constexpr uint32_t Unit100D7420_InitHeader = 0x101165F0u;
constexpr uint32_t FUN_10116610 = 0x10116610u;
constexpr uint32_t Unit100D7420_BuildRequestWithMd5 = 0x10116610u;
constexpr uint32_t Unit100D7420_ComputeMd4 = 0x10116C90u;
constexpr uint32_t Unit100D7420_NextSequence = 0x10116D30u;
constexpr uint32_t Unit100D7420_ComputeDigest16 = 0x10116D80u;
constexpr uint32_t Unit100D7420_ApplyEntrySequence = 0x10116DE0u;
constexpr uint32_t Unit100D7420_SetPair = 0x101170D0u;
constexpr uint32_t Unit100D7420_MenuCtor = 0x101172C0u;
constexpr uint32_t Unit100D7420_DeleteAllEntries = 0x10117460u;
constexpr uint32_t FUN_101177d0 = 0x101177D0u;
constexpr uint32_t Unit100D7420_MenuReleaseItemCount = 0x10117EE0u;
constexpr uint32_t Unit100D7420_InvokeChild34 = 0x10117F30u;
constexpr uint32_t Unit100D7420_Rebuild = 0x10117F40u;
constexpr uint32_t Unit100D7420_ClearEntries = 0x101180F0u;
constexpr uint32_t Unit100D7420_AttachSelection = 0x10118170u;
constexpr uint32_t FUN_101181d0 = 0x101181D0u;
constexpr uint32_t Unit100D7420_FindFreeEntry = 0x10118220u;
constexpr uint32_t Unit100D7420_MenuMoveSelection = 0x10118260u;
constexpr uint32_t Unit100D7420_GetFocusedValueA = 0x101185E0u;
constexpr uint32_t Unit100D7420_GetFocusedValueB = 0x10118640u;
constexpr uint32_t Unit100D7420_MenuUpdateFocusedHelp = 0x101186A0u;
constexpr uint32_t Unit100D7420_MenuStartFade = 0x101187F0u;
constexpr uint32_t FUN_10118830 = 0x10118830u;
constexpr uint32_t Unit100D7420_SelectFirstVisible = 0x10118870u;
constexpr uint32_t Unit100D7420_SetStateByIndex = 0x10118B60u;
constexpr uint32_t Unit100D7420_HitTest = 0x10118C10u;
constexpr uint32_t Unit100D7420_PickEntry = 0x10118CB0u;
constexpr uint32_t Unit100D7420_SelectEntryMoveCursor = 0x10118DB0u;
constexpr uint32_t Unit100D7420_SelectEntry = 0x10118E90u;
constexpr uint32_t Unit100D7420_SelectEntry_v1 = 0x10118E90u;
constexpr uint32_t Unit100D7420_CallMember0CSlot2 = 0x10118F30u;
constexpr uint32_t Unit100D7420_FreeChild = 0x10118F40u;
constexpr uint32_t Unit100D7420_GetRect = 0x10118F70u;
constexpr uint32_t Unit100D7420_SetState10ForIndex58 = 0x101190F0u;
constexpr uint32_t FUN_10119110 = 0x10119110u;
constexpr uint32_t Unit100D7420_SetActive = 0x10119200u;
constexpr uint32_t Unit100D7420_MarkDirty = 0x101192E0u;
constexpr uint32_t Unit100D7420_InitScaled = 0x10119300u;
constexpr uint32_t Unit100D7420_FreeEntryData = 0x10119360u;
constexpr uint32_t Unit100D7420_ClearField34 = 0x10119390u;
constexpr uint32_t Unit100D7420_SetSource = 0x101193D0u;
constexpr uint32_t Unit100D7420_ComputeRect = 0x101193F0u;
constexpr uint32_t Unit100D7420_UpdateLayout = 0x10119550u;
constexpr uint32_t Unit100D7420_MapKind119870 = 0x10119870u;
constexpr uint32_t Unit100D7420_LayoutCtor = 0x101198D0u;
constexpr uint32_t Unit100D7420_SetState = 0x10119970u;
constexpr uint32_t Unit100D7420_SetState_v1 = 0x10119970u;
constexpr uint32_t Unit100D7420_SetNames = 0x10119A50u;
constexpr uint32_t Unit100D7420_ComputeOffsets = 0x10119AD0u;
constexpr uint32_t FUN_10119bc0 = 0x10119BC0u;
constexpr uint32_t Unit100D7420_UpdateState = 0x10119C00u;
constexpr uint32_t Unit100D7420_InitDefaults = 0x10119DA0u;
constexpr uint32_t Unit100D7420_SetShape = 0x10119E20u;
constexpr uint32_t Unit100D7420_AttachTo = 0x10119E30u;
constexpr uint32_t Unit100D7420_ResolveShape = 0x10119E70u;
constexpr uint32_t FUN_10119f20 = 0x10119F20u;
constexpr uint32_t FUN_10119fb0 = 0x10119FB0u;
constexpr uint32_t Unit100D7420_InvokeCallback7C = 0x1011A3B0u;
constexpr uint32_t Unit100D7420_ApplyHelpAndTitle = 0x1011A490u;
constexpr uint32_t FUN_1011a560 = 0x1011A560u;
constexpr uint32_t FUN_1011a5a0 = 0x1011A5A0u;
constexpr uint32_t Unit100D7420_GetCenter = 0x1011ABF0u;
constexpr uint32_t FUN_1011ae80 = 0x1011AE80u;
constexpr uint32_t Unit100D7420_GetFieldByIndex = 0x1011B220u;
constexpr uint32_t Unit100D7420_GetEntryValueById = 0x1011B2A0u;
constexpr uint32_t Unit100D7420_GetEntry = 0x1011B2E0u;
constexpr uint32_t Unit100D7420_GetField8 = 0x1011B310u;
constexpr uint32_t Unit100D7420_GetFieldC = 0x1011B320u;
constexpr uint32_t Unit100D7420_GetOffsets = 0x1011B330u;
constexpr uint32_t FUN_1011ba40 = 0x1011BA40u;
constexpr uint32_t Unit100D7420_InitColors = 0x1011BA90u;
constexpr uint32_t Font_BindGlyphTexture = 0x1011BAE0u;
constexpr uint32_t Font_BindGlyphTexture_v1 = 0x1011BAE0u;
constexpr uint32_t Font_DrawGlyphQuadGrey = 0x1011BBA0u;
constexpr uint32_t Font_DrawGlyphQuadGrey_v1 = 0x1011BBA0u;
constexpr uint32_t Font_DrawGlyphQuad = 0x1011BBD0u;
constexpr uint32_t Gfx_SetAdditiveBlend = 0x1011BDC0u;
constexpr uint32_t Gfx_SetAdditiveBlend_v1 = 0x1011BDC0u;
constexpr uint32_t Font_DrawChar = 0x1011BF40u;
constexpr uint32_t Font_MeasureString = 0x1011BFD0u;
constexpr uint32_t Obj_104E1B90_DrawString = 0x1011C080u;
constexpr uint32_t Gfx_SetCellBlendMode = 0x1011C1F0u;
constexpr uint32_t Font_AllocHighResImage = 0x1011C5D0u;
constexpr uint32_t Tex_LoadSurfaceFromDatFile = 0x1011C670u;
constexpr uint32_t Font_LoadHighResTexture = 0x1011C720u;
constexpr uint32_t Font_ReleaseHighResTexture = 0x1011C810u;
constexpr uint32_t Font_IsHighResTextureLoaded = 0x1011C850u;
constexpr uint32_t unknown_libname_88 = 0x1011C870u;
constexpr uint32_t Font_LoadWidthTable = 0x1011C8B0u;
constexpr uint32_t Font_IsSingleByte = 0x1011C900u;
constexpr uint32_t Sjis_ToJisIndex = 0x1011C940u;
constexpr uint32_t Font_DecodeNextChar = 0x1011CE80u;
constexpr uint32_t Font_DecodeNextChar_v1 = 0x1011CE80u;
constexpr uint32_t Font_GetCharWidth = 0x1011CEC0u;
constexpr uint32_t FUN_1011cf70 = 0x1011CF70u;
constexpr uint32_t Font_MeasureText = 0x1011CF70u;
constexpr uint32_t Font_MeasureTextAll = 0x1011D000u;
constexpr uint32_t sub_1011D050 = 0x1011D050u;
constexpr uint32_t FUN_1011d290 = 0x1011D290u;
constexpr uint32_t Font_AddGlyphAdvance = 0x1011D840u;
constexpr uint32_t Font_AddGlyphAdvance_v1 = 0x1011D840u;
constexpr uint32_t Font_IsPrintableAscii = 0x1011D8B0u;
constexpr uint32_t Font_GetCharWidthAt = 0x1011D990u;
constexpr uint32_t Sjis_IsLeadByte = 0x1011DA30u;
constexpr uint32_t Sjis_GetCharByteLen = 0x1011DA80u;
constexpr uint32_t Sjis_GetCharByteLen_v1 = 0x1011DA80u;
constexpr uint32_t FUN_1011daa0 = 0x1011DAA0u;
constexpr uint32_t Sjis_IsCodeInSet = 0x1011DB40u;
constexpr uint32_t FUN_1011dc00 = 0x1011DC00u;
constexpr uint32_t FUN_1011dcc0 = 0x1011DCC0u;
constexpr uint32_t Sjis_CopyPrefix = 0x1011DE80u;
constexpr uint32_t Cell_GetField20Ptr = 0x1011E290u;
constexpr uint32_t Cell_ResolveSubCells = 0x1011E2A0u;
constexpr uint32_t Cell_ResolveTextureUV = 0x1011E6D0u;
constexpr uint32_t Cell_DrawStretchedWrapped = 0x1011EF60u;
constexpr uint32_t Cell_DrawWithDefaultBlend = 0x1011F2D0u;
constexpr uint32_t Cell_DrawQuadBlended = 0x1011F300u;
constexpr uint32_t Cell_DrawQuadBlended_v1 = 0x1011F300u;
constexpr uint32_t Cell_GetFrameBounds = 0x1011FA80u;
constexpr uint32_t Unit100D7420_GetNthEntryBytePair = 0x1011FB00u;
constexpr uint32_t FUN_1011fb60 = 0x1011FB60u;
constexpr uint32_t Shape_DrawScaled = 0x1011FC80u;
constexpr uint32_t Shape_DrawScaled_v1 = 0x1011FC80u;
constexpr uint32_t Cell_DrawFirstStretched = 0x10120000u;
constexpr uint32_t Cell_ResolveTextureList = 0x10120070u;
constexpr uint32_t Color_ModulateArgb128 = 0x101200B0u;
constexpr uint32_t ShapeMng_Init = 0x101201C0u;
constexpr uint32_t sub_10120200 = 0x10120200u;
constexpr uint32_t ShapeMng_FindShape = 0x10120450u;
constexpr uint32_t ShapeMng_FindShape_v1 = 0x10120450u;
constexpr uint32_t ShapeMng_FindShape_v2 = 0x10120450u;
constexpr uint32_t Cell_FindEntryByName = 0x101204B0u;
constexpr uint32_t ShapeMng_GetFontGlyph = 0x10120510u;
constexpr uint32_t ShapeMng_BindFontTexture = 0x10120590u;
constexpr uint32_t ShapeMng_LoadFontTextures = 0x10120660u;
constexpr uint32_t ShapeMng_LoadDmgNumTextures = 0x10120880u;
constexpr uint32_t Cell_ResolveAllSubCells = 0x10120950u;
constexpr uint32_t Cell_TestAny38Or0B8 = 0x10120B80u;
constexpr uint32_t Cell_TestAny1DOr9D = 0x10120BB0u;
constexpr uint32_t Cell_TestAny2AOr36 = 0x10120BE0u;
constexpr uint32_t FUN_10122e30 = 0x10122E30u;
constexpr uint32_t Cell_TestFlagAny2AOr36 = 0x10123350u;
constexpr uint32_t Cell_TestFlagKind = 0x10123380u;
constexpr uint32_t Cell_GetWord1036D6B0 = 0x10123670u;
constexpr uint32_t GetAnalogKey = 0x10123970u;
constexpr uint32_t Unit100D7420_TestCommand36CF60 = 0x10123A70u;
constexpr uint32_t Unit100D7420_TestCommand36CF60_v1 = 0x10123A70u;
constexpr uint32_t Cursor_TestAny1DOr9D = 0x10123DC0u;
constexpr uint32_t Cursor_TestAnyB8Or38 = 0x10123DF0u;
constexpr uint32_t Cell_TestFlagAny2AOr36Kind4 = 0x10123E20u;
constexpr uint32_t Cell_TestGlobal1036E3A0Mask = 0x10123E50u;
constexpr uint32_t Cell_SetByte104E1C3E = 0x10123E60u;
constexpr uint32_t Cell_ClearFlag104E1C3C = 0x10123E90u;
constexpr uint32_t Cell_SetFlag104E1C3C = 0x10123EA0u;
constexpr uint32_t Cell_GetByte1036CF5C = 0x10123EE0u;
constexpr uint32_t Cell_ClearFlag01And08 = 0x10124010u;
constexpr uint32_t Cell_ClearAllFlags = 0x10124050u;
constexpr uint32_t Cell_TestFlagBit1 = 0x10124070u;
constexpr uint32_t Cell_TakeFlagBit2 = 0x10124090u;
constexpr uint32_t Cell_TestFlagBit3 = 0x101240B0u;
constexpr uint32_t Cell_TestFlagBit0 = 0x101240D0u;
constexpr uint32_t Cursor_TakeEventFlag = 0x101240F0u;
constexpr uint32_t Cursor_GetFlagBit4 = 0x10124110u;
constexpr uint32_t Cursor_GetFlagBit2 = 0x10124130u;
constexpr uint32_t Cursor_GetHwCursorSlot = 0x101245C0u;
constexpr uint32_t CTkMouse_OnMouseMove = 0x10124640u;
constexpr uint32_t Cursor_SetPosition = 0x10124700u;
constexpr uint32_t Cursor_SetPosition_v1 = 0x10124700u;
constexpr uint32_t Cursor_UpdateTimer = 0x101248A0u;
constexpr uint32_t Cursor_EnterMode1 = 0x10124930u;
constexpr uint32_t Cursor_ToggleFlag1 = 0x101249E0u;
constexpr uint32_t Cursor_EnterMode2 = 0x10124A00u;
constexpr uint32_t Cursor_ToggleFlag2 = 0x10124AB0u;
constexpr uint32_t Cursor_EnterMode4 = 0x10124AD0u;
constexpr uint32_t Cursor_ToggleFlag4 = 0x10124B50u;
constexpr uint32_t Cursor_ResetPositionToMenu = 0x10124B70u;
constexpr uint32_t FUN_10124bc0 = 0x10124BC0u;
constexpr uint32_t Cursor_Draw = 0x10124EB0u;
constexpr uint32_t Cursor_SetModeFromMenuActive = 0x101252C0u;
constexpr uint32_t Cursor_SetVisible = 0x101252F0u;
constexpr uint32_t Cursor_SetVisible_v1 = 0x101252F0u;
constexpr uint32_t Cursor_UpdateVisibilityForMode = 0x10125F70u;
constexpr uint32_t FUN_101260d0 = 0x101260D0u;
constexpr uint32_t Cursor_StoreScreenCenter = 0x101262A0u;
constexpr uint32_t Cursor_StoreScreenCenter_v1 = 0x101262A0u;
constexpr uint32_t Unit100D7420_Init = 0x10126300u;
constexpr uint32_t Unit100D7420_ClearWords = 0x10126320u;
constexpr uint32_t Unit100D7420_ClearWords_v1 = 0x10126320u;
constexpr uint32_t Unit100D7420_NoOp = 0x10126330u;
constexpr uint32_t Unit100D7420_Clear = 0x10126350u;
constexpr uint32_t FUN_10126370 = 0x10126370u;
constexpr uint32_t Unit100D7420_Alloc = 0x101263B0u;
constexpr uint32_t FUN_10126410 = 0x10126410u;
constexpr uint32_t Unit100D7420_GetCount = 0x10126460u;
constexpr uint32_t Unit100D7420_PushBack = 0x10126480u;
constexpr uint32_t Unit100D7420_PushBack_v1 = 0x10126480u;
constexpr uint32_t Unit100D7420_PushFront = 0x101264C0u;
constexpr uint32_t Unit100D7420_GetHead = 0x10126500u;
constexpr uint32_t Unit100D7420_GetTail = 0x10126510u;
constexpr uint32_t FUN_10126520 = 0x10126520u;
constexpr uint32_t FUN_10126550 = 0x10126550u;
constexpr uint32_t Unit100D7420_Remove = 0x10126580u;
constexpr uint32_t FUN_101265d0 = 0x101265D0u;
constexpr uint32_t Unit100D7420_InsertAfter = 0x10126700u;
constexpr uint32_t Unit100D7420_ChatListInit = 0x10126790u;
constexpr uint32_t Unit100D7420_AddMessage = 0x101268F0u;
constexpr uint32_t Unit100D7420_MessageCtor = 0x10126B60u;
constexpr uint32_t Unit100D7420_MessageSet = 0x10126BC0u;
constexpr uint32_t FUN_10126c60 = 0x10126C60u;
constexpr uint32_t Unit100D7420_MsgWinDataCtor = 0x10127190u;
constexpr uint32_t Unit100D7420_MsgWinDataReset = 0x101271C0u;
constexpr uint32_t Unit100D7420_ResetEntryRing = 0x10127250u;
constexpr uint32_t Unit100D7420_BuildRecord = 0x101273C0u;
constexpr uint32_t Unit100D7420_FindByKeyByte = 0x10127870u;
constexpr uint32_t Unit100D7420_FindChar = 0x101278B0u;
constexpr uint32_t Unit100D7420_FindSubstringIndex = 0x101278D0u;
constexpr uint32_t Unit100D7420_CopyLimited = 0x10127930u;
constexpr uint32_t Unit100D7420_ToLowerInPlace = 0x10127970u;
constexpr uint32_t GetCallNum = 0x10127990u;
constexpr uint32_t Unit100D7420_IsNearNamedActor = 0x10127AB0u;
constexpr uint32_t ChatLog_AddMessage = 0x10127C00u;
constexpr uint32_t ChatLog_AddMessage_v1 = 0x10127C00u;
constexpr uint32_t FUN_10127ea0 = 0x10127EA0u;
constexpr uint32_t Unit100D7420_CtorResetAll = 0x10127EE0u;
constexpr uint32_t Unit100D7420_ResetAll = 0x10127F00u;
constexpr uint32_t FUN_101284f0 = 0x101284F0u;
constexpr uint32_t Unit100D7420_CountTextChars = 0x10128680u;
constexpr uint32_t Unit100D7420_CountTextCharsAndBytes = 0x10128830u;
constexpr uint32_t Unit100D7420_BuildTextCharIndex = 0x10128950u;
constexpr uint32_t FUN_10128ce0 = 0x10128CE0u;
constexpr uint32_t Unit100D7420_ClearFlag12FromValue = 0x10129290u;
constexpr uint32_t Unit100D7420_RingStepBack = 0x101292E0u;
constexpr uint32_t FUN_101295d0 = 0x101295D0u;
constexpr uint32_t Unit100D7420_GetRingHead14 = 0x10129C60u;
constexpr uint32_t Unit100D7420_FindKindRecord = 0x10129CD0u;
constexpr uint32_t Unit100D7420_MapCode = 0x10129D00u;
constexpr uint32_t FUN_10129d90 = 0x10129D90u;
constexpr uint32_t FUN_10129de0 = 0x10129DE0u;
constexpr uint32_t FUN_10129e30 = 0x10129E30u;
constexpr uint32_t FUN_10129e30_v1 = 0x10129E30u;
constexpr uint32_t Unit100D7420_MapCode54ToIndex = 0x1012A120u;
constexpr uint32_t FUN_1012a340 = 0x1012A340u;
constexpr uint32_t Unit100D7420_ClearBuffers = 0x1012A3B0u;
constexpr uint32_t FUN_1012c8c0 = 0x1012C8C0u;
constexpr uint32_t FUN_1012c8e0 = 0x1012C8E0u;
constexpr uint32_t thunk_FUN_1011ba40 = 0x1012C900u;
constexpr uint32_t FUN_1012ca90 = 0x1012CA90u;
constexpr uint32_t FUN_1012cad0 = 0x1012CAD0u;
constexpr uint32_t Unit1012AC80_ConstructVt10333648 = 0x10130F90u;
constexpr uint32_t Unit1012AC80_ConstructVt10333660 = 0x101312E0u;
constexpr uint32_t Unit1012AC80_ConstructVt10333670 = 0x10131530u;
constexpr uint32_t Unit1012AC80_ConstructVt10333908 = 0x10136380u;
constexpr uint32_t FUN_10136670 = 0x10136670u;
constexpr uint32_t Unit1012AC80_SetPosition = 0x10136870u;
constexpr uint32_t Unit1012AC80_ConstructVt103339F0 = 0x10136A30u;
constexpr uint32_t Unit1012AC80_ConstructVt10333AE0 = 0x10136F60u;
constexpr uint32_t Unit1012AC80_ClearRanges = 0x10138EC0u;
constexpr uint32_t FUN_10139f40 = 0x10139F40u;
constexpr uint32_t FUN_10139fb0 = 0x10139FB0u;
constexpr uint32_t thunk_FUN_1013a100 = 0x10139FE0u;
constexpr uint32_t Unit1012AC80_TickHelp = 0x1013A010u;
constexpr uint32_t TkHelp_AdvanceTimer = 0x1013A030u;
constexpr uint32_t Unit1012AC80_IsEvent = 0x1013A060u;
constexpr uint32_t Unit1012AC80_AdvanceHelpTimer = 0x1013A070u;
constexpr uint32_t FUN_1013a0d0 = 0x1013A0D0u;
constexpr uint32_t FUN_1013a100 = 0x1013A100u;
constexpr uint32_t Unit1012AC80_ForwardToPanel = 0x1013A320u;
constexpr uint32_t FUN_1013a470 = 0x1013A470u;
constexpr uint32_t Unit1012AC80_CtorVt10333EC0 = 0x1013BE50u;
constexpr uint32_t FUN_1013bf40 = 0x1013BF40u;
constexpr uint32_t Unit1012AC80_Construct = 0x10144F40u;
constexpr uint32_t Unit1012AC80_Set = 0x10146930u;
constexpr uint32_t Unit1012AC80_SetCopy = 0x10146960u;
constexpr uint32_t Unit1012AC80_CtorVt10334970 = 0x101475B0u;
constexpr uint32_t Unit1012AC80_CtorVt103349B8 = 0x10148430u;
constexpr uint32_t Unit1012AC80_CtorVt10334A00 = 0x10148640u;
constexpr uint32_t YkWndList_Construct = 0x10148ED0u;
constexpr uint32_t Unit1012AC80_CtorVt10334AB8 = 0x1014A170u;
constexpr uint32_t Unit1012AC80_CtorVt10334B28 = 0x1014A7D0u;
constexpr uint32_t Unit1012AC80_CtorVt10334B98 = 0x1014B670u;
constexpr uint32_t Unit1012AC80_CtorVt10334C08 = 0x1014BA60u;
constexpr uint32_t FUN_1014e020 = 0x1014E020u;
constexpr uint32_t Unit1012AC80_SetName = 0x10150A30u;
constexpr uint32_t Unit1012AC80_SetField48 = 0x10150A60u;
constexpr uint32_t Unit1012AC80_SetTextFwd = 0x10150CC0u;
constexpr uint32_t Unit1012AC80_SetTextFwd_v1 = 0x10150CC0u;
constexpr uint32_t Unit1012AC80_SetText = 0x10150CE0u;
constexpr uint32_t FUN_10150d60 = 0x10150D60u;
constexpr uint32_t Unit1012AC80_ClearFields = 0x10151140u;
constexpr uint32_t Unit1012AC80_GetMessageFwd = 0x10152EF0u;
constexpr uint32_t Unit1012AC80_GetMessage = 0x10152F20u;
constexpr uint32_t Unit1012AC80_Clear32 = 0x10152F80u;
constexpr uint32_t Unit1012AC80_ReleaseAll32 = 0x10152FA0u;
constexpr uint32_t Unit1012AC80_ReleaseOne = 0x10152FC0u;
constexpr uint32_t Unit1012AC80_LoadMessageBlock = 0x10152FF0u;
constexpr uint32_t FUN_101530c0 = 0x101530C0u;
constexpr uint32_t Unit1012AC80_GetEntryPtrByLayout = 0x101530C0u;
constexpr uint32_t Unit1012AC80_GetEntryWord3E = 0x10153100u;
constexpr uint32_t Unit1012AC80_Read2Byte7Bit = 0x101531B0u;
constexpr uint32_t FUN_10153310 = 0x10153310u;
constexpr uint32_t Unit1012AC80_FormatMessageWithArgs = 0x10153A40u;
constexpr uint32_t Unit1012AC80_LoadMessageBlocksSetA = 0x10153AF0u;
constexpr uint32_t Unit1012AC80_LoadMessageBlocksSetB = 0x10153B30u;
constexpr uint32_t FUN_10153b70 = 0x10153B70u;
constexpr uint32_t Unit1012AC80_LoadAllMessageTables = 0x10153C10u;
constexpr uint32_t Unit1012AC80_LoadStringTable = 0x10153EF0u;
constexpr uint32_t Unit1012AC80_DtorVt103350EC = 0x10153F80u;
constexpr uint32_t Unit1012AC80_GetInstance = 0x10153FF0u;
constexpr uint32_t Unit1012AC80_GetInstance_v1 = 0x10153FF0u;
constexpr uint32_t Unit1012AC80_MessageSetCtor = 0x10154070u;
constexpr uint32_t FUN_101540a0 = 0x101540A0u;
constexpr uint32_t Unit1012AC80_LoadMessageSetByState = 0x101542B0u;
constexpr uint32_t Unit1012AC80_IndexOf = 0x101544F0u;
constexpr uint32_t Unit1012AC80_GetByValue = 0x10154520u;
constexpr uint32_t Unit1012AC80_GetEntryOrEmpty = 0x10154550u;
constexpr uint32_t Unit1012AC80_ReleaseField7C = 0x10154840u;
constexpr uint32_t Unit1012AC80_UpdatePendingRead = 0x10154860u;
constexpr uint32_t Unit1012AC80_GetCount = 0x101549E0u;
constexpr uint32_t Unit1012AC80_GetWordAt = 0x101549F0u;
constexpr uint32_t Unit1012AC80_CtorVt103350F8 = 0x10154AB0u;
constexpr uint32_t Entry_Init6 = 0x10154AD0u;
constexpr uint32_t FUN_10154d70 = 0x10154D70u;
constexpr uint32_t Unit1012AC80_CtorVt1033512C = 0x10154DC0u;
constexpr uint32_t Unit1012AC80_GetInnerPtr10 = 0x10154F00u;
constexpr uint32_t Unit1012AC80_DtorVt10335138 = 0x10154F30u;
constexpr uint32_t Unit1012AC80_CtorVt10335138 = 0x10154F60u;
constexpr uint32_t Entry_SetFields4 = 0x10154FC0u;
constexpr uint32_t Unit1012AC80_DtorVt1033515C = 0x10155100u;
constexpr uint32_t Entry_ReleaseField4Zero = 0x10155140u;
constexpr uint32_t Unit1012AC80_CtorVt1033515C = 0x10155160u;
constexpr uint32_t Unit1012AC80_GetMember14 = 0x101552D0u;
constexpr uint32_t FUN_10155320 = 0x10155320u;
constexpr uint32_t Unit1012AC80_CtorVt10335168 = 0x10155370u;
constexpr uint32_t Unit1012AC80_GetMemberC = 0x101554E0u;
constexpr uint32_t FUN_10155500 = 0x10155500u;
constexpr uint32_t Unit1012AC80_CtorVt10335174 = 0x10155550u;
constexpr uint32_t Unit1012AC80_GetField8 = 0x10155600u;
constexpr uint32_t FUN_101569a0 = 0x101569A0u;
constexpr uint32_t Unit1012AC80_SetFields = 0x101569F0u;
constexpr uint32_t FUN_10156be0 = 0x10156BE0u;
constexpr uint32_t Unit1012AC80_SetField04 = 0x10156BF0u;
constexpr uint32_t Unit1012AC80_ClearTargetAndRedraw = 0x10157610u;
constexpr uint32_t Unit1012AC80_ClearField68AndDraw = 0x10157620u;
constexpr uint32_t Unit1012AC80_ClearField68AndDraw_v1 = 0x10157620u;
constexpr uint32_t Unit1012AC80_SetTargetMode = 0x10157660u;
constexpr uint32_t Unit1012AC80_SetTargetMode_v1 = 0x10157660u;
constexpr uint32_t Unit1012AC80_IsTargetUnusable = 0x101576F0u;
constexpr uint32_t Unit1012AC80_SetValues3 = 0x10157780u;
constexpr uint32_t Unit1012AC80_SetFields78D0 = 0x10157860u;
constexpr uint32_t Unit1012AC80_GetByte75 = 0x10157880u;
constexpr uint32_t Unit1012AC80_GetByte76 = 0x10157890u;
constexpr uint32_t Unit1012AC80_GetByte77 = 0x101578A0u;
constexpr uint32_t Unit1012AC80_GetWord80 = 0x101578B0u;
constexpr uint32_t Unit1012AC80_GetField7C = 0x101578C0u;
constexpr uint32_t Target_GetFlag82 = 0x101578D0u;
constexpr uint32_t Target_GetFlag83 = 0x101578E0u;
constexpr uint32_t Target_Reset = 0x101578F0u;
constexpr uint32_t Unit1012AC80_ClearFields84And88 = 0x101579B0u;
constexpr uint32_t Target_Update = 0x101579C0u;
constexpr uint32_t Target_Select = 0x10157B40u;
constexpr uint32_t Target_SelectByIndex = 0x10157C60u;
constexpr uint32_t sub_10157CF0 = 0x10157CF0u;
constexpr uint32_t Menu_IsFocusNotInList = 0x10157D20u;
constexpr uint32_t Target_FindCandidate = 0x10157D60u;
constexpr uint32_t Target_CycleMenuStep = 0x10157E20u;
constexpr uint32_t Target_IssueStateCommand = 0x10157FA0u;
constexpr uint32_t Target_UpdateSelectionByState = 0x101581E0u;
constexpr uint32_t Target_InvokeFieldCallback = 0x101582F0u;
constexpr uint32_t Target_UpdateActorFlags138 = 0x101583F0u;
constexpr uint32_t Target_HasActor = 0x10158470u;
constexpr uint32_t Target_UpdateSelection = 0x10158480u;
constexpr uint32_t Target_SendAttackOff = 0x10158510u;
constexpr uint32_t Target_GetActor = 0x10158550u;
constexpr uint32_t Target_Clear = 0x10158590u;
constexpr uint32_t Target_RunActiveCommand = 0x101585F0u;
constexpr uint32_t Unit1012AC80_FindActiveCommand = 0x101587B0u;
constexpr uint32_t FUN_101587f0 = 0x101587F0u;
constexpr uint32_t Unit1012AC80_IsCommand76Or77Active = 0x10158AA0u;
constexpr uint32_t Target_TrySubSelect = 0x10158B00u;
constexpr uint32_t Target_ActOrTrySubSelect = 0x10158B40u;
constexpr uint32_t Target_PickNearestToPoint = 0x10158D90u;
constexpr uint32_t Target_ApplyAction = 0x10158DB0u;
constexpr uint32_t Target_ClearAll = 0x10158FF0u;
constexpr uint32_t Target_CommitSubTarget = 0x10159040u;
constexpr uint32_t Target_ClearAndSetFlag53 = 0x101590A0u;
constexpr uint32_t Target_SetDisplayName = 0x101590C0u;
constexpr uint32_t Target_SetFlagsFromMask = 0x10159170u;
constexpr uint32_t Target_IsUsable = 0x101591D0u;
constexpr uint32_t Unit1012AC80_GetField84Ptr = 0x10159370u;
constexpr uint32_t Actor_SetSlotTableEntry = 0x10159400u;
constexpr uint32_t Actor_SetSlotTableEntry_v1 = 0x10159400u;
constexpr uint32_t Target_DispatchByIndex = 0x10159420u;
constexpr uint32_t Target_SetIfUsable = 0x10159440u;
constexpr uint32_t Target_SetHandlerCc = 0x101594D0u;
constexpr uint32_t Target_SetHandlerC8 = 0x101594E0u;
constexpr uint32_t Target_ReleaseSelection = 0x101594F0u;
constexpr uint32_t Target_ResetRecordReturnSelf = 0x10159530u;
constexpr uint32_t Target_IsRecordStillValid = 0x10159540u;
constexpr uint32_t Target_CanTarget = 0x101595D0u;
constexpr uint32_t Target_SetActor = 0x10159660u;
constexpr uint32_t Target_ResetRecord = 0x10159740u;
constexpr uint32_t Target_RefreshActor = 0x10159780u;
constexpr uint32_t Entry_InitKey = 0x101597C0u;
constexpr uint32_t Entry_ValidateKey = 0x101597F0u;
constexpr uint32_t Entry_SetNameAndKey = 0x10159830u;
constexpr uint32_t Target_GetPosShortsMainOrSub = 0x10159880u;
constexpr uint32_t Target_GetCachedIfValid = 0x101598A0u;
constexpr uint32_t Target_GetActorPtr = 0x101598B0u;
constexpr uint32_t Unit1012AC80_IsWithinRadius = 0x101598C0u;
constexpr uint32_t Entry_MatchesPair = 0x10159910u;
constexpr uint32_t Unit1012AC80_MoveByDirection = 0x10159970u;
constexpr uint32_t Unit1012AC80_ApplyOffset = 0x10159A20u;
constexpr uint32_t Camera_RotateVecYaw = 0x10159B10u;
constexpr uint32_t FUN_10159ba0 = 0x10159BA0u;
constexpr uint32_t FUN_10159c80 = 0x10159C80u;
constexpr uint32_t Target_GetPosShorts = 0x10159D40u;
constexpr uint32_t Entry_ResolveValid = 0x10159DE0u;
constexpr uint32_t Entry_IsActorFieldMatch = 0x10159E10u;
constexpr uint32_t Unit1012AC80_FindRange = 0x10159EB0u;
constexpr uint32_t Unit1012AC80_FindRange_v1 = 0x10159EB0u;
constexpr uint32_t Unit1012AC80_ResetState = 0x1015AFA0u;
constexpr uint32_t Unit1012AC80_ResetState_v1 = 0x1015AFA0u;
constexpr uint32_t Unit1012AC80_IsCachedPtrMatch = 0x1015B190u;
constexpr uint32_t Unit1012AC80_GetObjectPtr = 0x1015B270u;
constexpr uint32_t FUN_1015b410 = 0x1015B410u;
constexpr uint32_t Unit1012AC80_FindCachedEntry = 0x1015B970u;
constexpr uint32_t Unit1012AC80_GetCachedOrLoad = 0x1015B9E0u;
constexpr uint32_t Unit1012AC80_FindEmptySlot = 0x1015BAF0u;
constexpr uint32_t FUN_1015bc40 = 0x1015BC40u;
constexpr uint32_t Unit1012AC80_RotateBits3 = 0x1015D7F0u;
constexpr uint32_t MenuMng_ReinitState = 0x1015D860u;
constexpr uint32_t MenuMng_Init = 0x1015D900u;
constexpr uint32_t MenuMng_ResetFields = 0x1015D9A0u;
constexpr uint32_t sub_1015D9E0 = 0x1015D9E0u;
constexpr uint32_t Unit100D7420_InsertSortedBy60 = 0x1015DB10u;
constexpr uint32_t Menu_SetFlag76 = 0x1015DB70u;
constexpr uint32_t FUN_1015db80 = 0x1015DB80u;
constexpr uint32_t FUN_1015db80_v1 = 0x1015DB80u;
constexpr uint32_t Unit1012AC80_MenuForwardItem8 = 0x1015DBB0u;
constexpr uint32_t Unit100D7420_RemoveMatching = 0x1015DBD0u;
constexpr uint32_t Menu_FindItemByName = 0x1015DC10u;
constexpr uint32_t Menu_LookupWindowByName = 0x1015DC70u;
constexpr uint32_t Unit1012AC80_MenuTest48OrStateB4 = 0x1015DCE0u;
constexpr uint32_t Unit1012AC80_MenuTest48OrStateB4_v1 = 0x1015DCE0u;
constexpr uint32_t Menu_HandleEvent = 0x1015DD00u;
constexpr uint32_t Menu_HandleEvent_v1 = 0x1015DD00u;
constexpr uint32_t FUN_1015dd50 = 0x1015DD50u;
constexpr uint32_t FUN_1015dd70 = 0x1015DD70u;
constexpr uint32_t Unit1012AC80_Nop = 0x1015DFC0u;
constexpr uint32_t Menu_FindItemByNameAt1C = 0x1015E030u;
constexpr uint32_t FUN_1015e0f0 = 0x1015E0F0u;
constexpr uint32_t MenuMng_OpenByName = 0x1015E350u;
constexpr uint32_t MenuMng_OpenByName_v1 = 0x1015E350u;
constexpr uint32_t MenuMng_OpenByName_v2 = 0x1015E350u;
constexpr uint32_t MenuMng_OpenByName_v3 = 0x1015E350u;
constexpr uint32_t FUN_1015e530 = 0x1015E530u;
constexpr uint32_t Unit1012AC80_MenuItemFade = 0x1015E6E0u;
constexpr uint32_t Unit1012AC80_MenuInitAndFadeItem = 0x1015E790u;
constexpr uint32_t Unit1012AC80_MenuInitAndFadeItem_v1 = 0x1015E790u;
constexpr uint32_t FUN_1015e830 = 0x1015E830u;
constexpr uint32_t FUN_1015e850 = 0x1015E850u;
constexpr uint32_t Menu_SetItemAt54 = 0x1015E850u;
constexpr uint32_t Menu_RemoveAndFreeItem = 0x1015E8A0u;
constexpr uint32_t Menu_CloseEntry = 0x1015E910u;
constexpr uint32_t Menu_FindWindowDefByName = 0x1015E950u;
constexpr uint32_t Menu_CloseQuery = 0x1015EB60u;
constexpr uint32_t Menu_FindByName = 0x1015EC50u;
constexpr uint32_t Menu_FindByFlags34 = 0x1015ECC0u;
constexpr uint32_t Menu_FindByFlags38 = 0x1015ED10u;
constexpr uint32_t Menu_IsFocusedItem = 0x1015ED80u;
constexpr uint32_t Menu_HasFocusedItem = 0x1015EDB0u;
constexpr uint32_t Menu_NextVisibleItem = 0x1015EE00u;
constexpr uint32_t Menu_OpenMenuWindowWhenIdle = 0x1015EE40u;
constexpr uint32_t FUN_1015eea0 = 0x1015EEA0u;
constexpr uint32_t Menu_CallChildSlot34ForEachItem = 0x1015EF20u;
constexpr uint32_t Menu_ApplyFlagsToItems = 0x1015F3E0u;
constexpr uint32_t Menu_DrawLayers = 0x1015F6A0u;
constexpr uint32_t Unit1012AC80_ResolveLayoutIndex = 0x1015F980u;
constexpr uint32_t Menu_GetWordPtrAtA0 = 0x1015F9E0u;
constexpr uint32_t sub_1015FA00 = 0x1015FA00u;
constexpr uint32_t Menu_GetNormalizedPos = 0x101601D0u;
constexpr uint32_t Menu_GetNormalizedPos_v1 = 0x101601D0u;
constexpr uint32_t MenuMng_GetUiScale = 0x10160250u;
constexpr uint32_t MenuMng_GetOffsetX = 0x10160290u;
constexpr uint32_t MenuMng_GetOffsetY = 0x101602C0u;
constexpr uint32_t CTkMenuMng_IsDrawWindow = 0x101602F0u;
constexpr uint32_t Menu_ReloadResource = 0x10160440u;
constexpr uint32_t Gfx_SetDefault2DStates = 0x101604B0u;
constexpr uint32_t Gfx_SetDefault2DStates_v1 = 0x101604B0u;
constexpr uint32_t Gfx_DisableFogAndAlphaBlend = 0x10160650u;
constexpr uint32_t Gfx_DisableFogAndAlphaBlend_v1 = 0x10160650u;
constexpr uint32_t Menu_GetStateB4 = 0x101606F0u;
constexpr uint32_t Menu_GetStateB4_v1 = 0x101606F0u;
constexpr uint32_t Menu_IsStateB4Eq1 = 0x10160700u;
constexpr uint32_t FUN_10160760 = 0x10160760u;
constexpr uint32_t FUN_10160930 = 0x10160930u;
constexpr uint32_t Unit101D7020_DestroyDrawMenuIfAt14 = 0x101609C0u;
constexpr uint32_t Unit101D7020_CloseIfAt14 = 0x10160A50u;
constexpr uint32_t Menu_GetFocusedItem = 0x10160C90u;
constexpr uint32_t Unit1012AC80_CtorVt10335350 = 0x10161110u;
constexpr uint32_t Unit1012AC80_CtorVt1033536C = 0x10161380u;
constexpr uint32_t Unit1012AC80_CtorVt10335388 = 0x101615E0u;
constexpr uint32_t Unit1012AC80_InitFromScreenRect = 0x10161ED0u;
constexpr uint32_t FUN_101626e0 = 0x101626E0u;
constexpr uint32_t FUN_10162f30 = 0x10162F30u;
constexpr uint32_t Unit1012AC80_ApplySlot20Result = 0x10162FB0u;
constexpr uint32_t FUN_10162fe0 = 0x10162FE0u;
constexpr uint32_t Obj_10621944_Draw = 0x101630E0u;
constexpr uint32_t Unit1012AC80_ZeroWordsAndDwords = 0x10163350u;
constexpr uint32_t Unit1012AC80_NoOpE = 0x10163380u;
constexpr uint32_t Obj_10621944_DrawBgRect = 0x10163390u;
constexpr uint32_t Unit1012AC80_ZeroDwords = 0x101633D0u;
constexpr uint32_t Gfx_DrawSolidRect2D = 0x10163400u;
constexpr uint32_t sub_10165070 = 0x10165070u;
constexpr uint32_t Unit10163E60_InitGlobalTables = 0x101671D0u;
constexpr uint32_t ZoneMap_ConstructVt10336614 = 0x10167440u;
constexpr uint32_t Vt10336614_Destructor = 0x101674B0u;
constexpr uint32_t ZoneMap_ConstructVt10336634 = 0x10167800u;
constexpr uint32_t Vt10336634_Destructor = 0x101678D0u;
constexpr uint32_t GetPtr8IfField60Set = 0x10167940u;
constexpr uint32_t ZoneMap_ReleaseBuffers = 0x10167950u;
constexpr uint32_t ZoneMap_UnlinkSlotCells = 0x101679A0u;
constexpr uint32_t Dbg_DrawWireMesh = 0x10168D20u;
constexpr uint32_t ZoneMap_GetCount34154 = 0x10168FD0u;
constexpr uint32_t Unit10163E60_ProbeAxis1 = 0x10169290u;
constexpr uint32_t Unit10163E60_SegmentHit3413A = 0x101693B0u;
constexpr uint32_t Unit10163E60_SegmentHit34139 = 0x10169B40u;
constexpr uint32_t Unit10163E60_SegmentTest34139 = 0x1016A2D0u;
constexpr uint32_t FUN_1016a990 = 0x1016A990u;
constexpr uint32_t Unit10163E60_SegmentHitWithObjects = 0x1016B1D0u;
constexpr uint32_t Mat44_Multiply = 0x1016C060u;
constexpr uint32_t ZoneMap_GetHitCount = 0x1016C150u;
constexpr uint32_t ZoneMap_IsPointInTriangle = 0x1016C390u;
constexpr uint32_t FUN_1016c5c0 = 0x1016C5C0u;
constexpr uint32_t FUN_1016c5d0 = 0x1016C5D0u;
constexpr uint32_t FUN_1016d580 = 0x1016D580u;
constexpr uint32_t FUN_1016dfd0 = 0x1016DFD0u;
constexpr uint32_t FUN_1016e7e0 = 0x1016E7E0u;
constexpr uint32_t Unit10163E60_SweepStep = 0x1016F0E0u;
constexpr uint32_t FUN_1016f320 = 0x1016F320u;
constexpr uint32_t FUN_1016fa20 = 0x1016FA20u;
constexpr uint32_t ZoneMap_ConstructVt10336668 = 0x1016FC40u;
constexpr uint32_t Vt10336668_Destructor = 0x1016FC80u;
constexpr uint32_t FUN_1016fc90 = 0x1016FC90u;
constexpr uint32_t sub_10170B30 = 0x10170B30u;
constexpr uint32_t sub_10170C80 = 0x10170C80u;
constexpr uint32_t FUN_10170cd0 = 0x10170CD0u;
constexpr uint32_t sub_10170E80 = 0x10170E80u;
constexpr uint32_t ZoneMap_FindEntryByName = 0x10170F30u;
constexpr uint32_t ZoneMap_ConstructVt1033668C = 0x101718E0u;
constexpr uint32_t Vt1033668C_Destructor = 0x10171920u;
constexpr uint32_t FUN_10173ae0 = 0x10173AE0u;
constexpr uint32_t ZoneMap_ResetDbgBoxList = 0x10174770u;
constexpr uint32_t FUN_10174a60 = 0x10174A60u;
constexpr uint32_t Mat4_SetIdentity_2 = 0x10175740u;
constexpr uint32_t Mat4_MulTranslation = 0x10175780u;
constexpr uint32_t FUN_10176dd0 = 0x10176DD0u;
constexpr uint32_t CMoResource_VT_10336698_Destructor = 0x10176EC0u;
constexpr uint32_t CMoResource_VT_103366C4_Destructor = 0x10176FA0u;
constexpr uint32_t CMoResource_VT_103366F0_Destructor = 0x10177020u;
constexpr uint32_t Gfx_ShaderSrc_GetText = 0x10177340u;
constexpr uint32_t Gfx_ShaderSrc_GetTextLength = 0x10177350u;
constexpr uint32_t ZoneMap_GetMatrixGlobal = 0x101773F0u;
constexpr uint32_t Gfx_SetZBias = 0x10177400u;
constexpr uint32_t ZoneMap_SetOverlayProjection = 0x10177420u;
constexpr uint32_t FUN_10177730 = 0x10177730u;
constexpr uint32_t ZoneMap_QueryBoxHeightGlobal = 0x10177770u;
constexpr uint32_t CTsZoneMap_LogZoneMetrics = 0x10177800u;
constexpr uint32_t ZoneMap_Call10173AE0 = 0x101779B0u;
constexpr uint32_t XiZone_DrawMarkerForward = 0x10177CD0u;
constexpr uint32_t CTsZoneMap_Init = 0x10177D00u;
constexpr uint32_t CTsZoneMap_SetLayerOrder = 0x10177FA0u;
constexpr uint32_t CTsZoneMap_InitZBiasPresets = 0x101781D0u;
constexpr uint32_t FUN_10178250 = 0x10178250u;
constexpr uint32_t ZoneMap_ResetGlobalTables = 0x101784E0u;
constexpr uint32_t FUN_10178530 = 0x10178530u;
constexpr uint32_t FUN_10178610 = 0x10178610u;
constexpr uint32_t FUN_10178790 = 0x10178790u;
constexpr uint32_t ZoneMap_UpdateLoadState = 0x101787C0u;
constexpr uint32_t ZoneMap_ReleaseSlot = 0x10178AC0u;
constexpr uint32_t ZoneMap_ReleaseStateBlock = 0x10178B00u;
constexpr uint32_t FUN_10178d90 = 0x10178D90u;
constexpr uint32_t FUN_10178dd0 = 0x10178DD0u;
constexpr uint32_t ZoneMap_Reset = 0x10178F70u;
constexpr uint32_t ZoneMap_CreateVertexShaders = 0x101792A0u;
constexpr uint32_t FUN_1017a630 = 0x1017A630u;
constexpr uint32_t FUN_1017a690 = 0x1017A690u;
constexpr uint32_t ZoneMap_FreeSlot = 0x1017B110u;
constexpr uint32_t ZoneMap_TeardownStep = 0x1017B1E0u;
constexpr uint32_t ZoneMap_TeardownStep_v1 = 0x1017B1E0u;
constexpr uint32_t XiZone_Draw = 0x1017B510u;
constexpr uint32_t ZoneMap_QueryBoxHeightBandThenGetCount34154 = 0x101811D0u;
constexpr uint32_t ZoneMap_QueryBoxHeight = 0x10181210u;
constexpr uint32_t ZoneMap_QueryBoxHeightBand = 0x10181290u;
constexpr uint32_t ZoneMap_QueryBoxHeightByRadius = 0x10181300u;
constexpr uint32_t FUN_10181510 = 0x10181510u;
constexpr uint32_t ZoneMap_Query1693B0 = 0x10181590u;
constexpr uint32_t ZoneMap_IsBoxHeightHitByRadius = 0x101819B0u;
constexpr uint32_t sub_10181BB0 = 0x10181BB0u;
constexpr uint32_t CTsZoneMap_DrawProjectedTexture = 0x10182700u;
constexpr uint32_t FUN_10183370 = 0x10183370u;
constexpr uint32_t ZoneMap_FindSlotByName = 0x10183420u;
constexpr uint32_t XiZone_DrawModelAtMatrix = 0x10183490u;
constexpr uint32_t ZoneMap_DrawMeshById = 0x10183DA0u;
constexpr uint32_t XiZone_DrawGroundShadowQuad = 0x10184C90u;
constexpr uint32_t CTsZoneMap_DrawProjectedShadow = 0x10184FE0u;
constexpr uint32_t XiZone_DrawDecalOnTriangles = 0x10185CA0u;
constexpr uint32_t FUN_10186d30 = 0x10186D30u;
constexpr uint32_t ZoneMap_ClearSlots = 0x10186DB0u;
constexpr uint32_t ZoneMap_FreeSubAreas = 0x10186DD0u;
constexpr uint32_t FUN_10186e70 = 0x10186E70u;
constexpr uint32_t ZoneMap_GetAddr0B4 = 0x10186E90u;
constexpr uint32_t ZoneMap_GetAddr0C4 = 0x10186EA0u;
constexpr uint32_t ZoneMap_GetAddr0D4 = 0x10186EB0u;
constexpr uint32_t ZoneMap_GetAddr024 = 0x10186EC0u;
constexpr uint32_t ZoneMap_GetAddr044 = 0x10186EE0u;
constexpr uint32_t ZoneMap_GetAddr064 = 0x10186F00u;
constexpr uint32_t ZoneMap_GetAddr074 = 0x10186F10u;
constexpr uint32_t Env_GetColorPtr84 = 0x10186F20u;
constexpr uint32_t Env_GetColorPtr84_v1 = 0x10186F20u;
constexpr uint32_t ZoneMap_GetAddr094 = 0x10186F30u;
constexpr uint32_t ZoneMap_GetAddrRef18Plus1C = 0x10186F40u;
constexpr uint32_t ZoneMap_GetAddrRef18Plus20 = 0x10186F50u;
constexpr uint32_t FUN_10186f60 = 0x10186F60u;
constexpr uint32_t ZoneMap_GetAddr0E4 = 0x10186F70u;
constexpr uint32_t ZoneMap_GetAddr0F4 = 0x10186F80u;
constexpr uint32_t ZoneMap_GetAddr114 = 0x10186F90u;
constexpr uint32_t Env_GetAmbientColorPtr = 0x10186FA0u;
constexpr uint32_t Env_GetAmbientColorPtr_v1 = 0x10186FA0u;
constexpr uint32_t ZoneMap_GetAddrRef18Plus3C = 0x10186FB0u;
constexpr uint32_t ZoneMap_GetAddrRef18Plus40 = 0x10186FC0u;
constexpr uint32_t ZoneMap_GetFloatRef18Plus44 = 0x10186FD0u;
constexpr uint32_t ZoneMap_GetFloatRef1CPlus4 = 0x10186FE0u;
constexpr uint32_t ZoneMap_GetFloatRef1CPlus4_v1 = 0x10186FE0u;
constexpr uint32_t ZoneMap_GetFloatRef1CPlus8 = 0x10186FF0u;
constexpr uint32_t ZoneMap_GetByteRef1CPlus10 = 0x10187000u;
constexpr uint32_t ZoneMap_GetFloatRef1CPlus0C = 0x10187020u;
constexpr uint32_t FUN_10187030 = 0x10187030u;
constexpr uint32_t ZoneMap_ReleaseChain = 0x10187030u;
constexpr uint32_t FUN_10187050 = 0x10187050u;
constexpr uint32_t ZoneMap_SetVtableApplyRecord = 0x10187380u;
constexpr uint32_t ZoneMap_Destroy = 0x10187470u;
constexpr uint32_t FUN_101874a0 = 0x101874A0u;
constexpr uint32_t ZoneMap_Update = 0x101874A0u;
constexpr uint32_t ZoneMap_ForEachFindKind5B = 0x101875C0u;
constexpr uint32_t ZoneMap_ForEachFindKind5A = 0x101875E0u;
constexpr uint32_t ZoneMap_SaveState = 0x10187600u;
constexpr uint32_t ZoneMap_UpdateOrbit = 0x10187720u;
constexpr uint32_t ZoneMap_SetRecord = 0x101880C0u;
constexpr uint32_t ZoneMap_ApplyColors = 0x10188190u;
constexpr uint32_t ZoneMap_GetCurrent = 0x10188900u;
constexpr uint32_t ZoneMap_AppendToList = 0x10188910u;
constexpr uint32_t FUN_10188940 = 0x10188940u;
constexpr uint32_t ZoneMap_GetTail = 0x101889A0u;
constexpr uint32_t FUN_101889c0 = 0x101889C0u;
constexpr uint32_t FUN_101889e0 = 0x101889E0u;
constexpr uint32_t XiZone_UpdateTree = 0x101889E0u;
constexpr uint32_t FUN_10188a10 = 0x10188A10u;
constexpr uint32_t XiZone_DestroyChildList = 0x10188A10u;
constexpr uint32_t FUN_10188a50 = 0x10188A50u;
constexpr uint32_t XiZone_GetDirLights = 0x10188B70u;
constexpr uint32_t XiZone_GetDirLights_v1 = 0x10188B70u;
constexpr uint32_t ZoneMap_FillDirLight = 0x10188CA0u;
constexpr uint32_t ZoneMap_FillDirLight_v1 = 0x10188CA0u;
constexpr uint32_t Env_ComputeAmbient = 0x10188E00u;
constexpr uint32_t Env_ComputeAmbient_v1 = 0x10188E00u;
constexpr uint32_t FUN_10189120 = 0x10189120u;
constexpr uint32_t sub_10189120 = 0x10189120u;
constexpr uint32_t FUN_101892b0 = 0x101892B0u;
constexpr uint32_t Env_BlendKeys = 0x10189490u;
constexpr uint32_t FUN_10189490 = 0x10189490u;
constexpr uint32_t FUN_10189750 = 0x10189750u;
constexpr uint32_t Env_LerpFloat = 0x10189810u;
constexpr uint32_t Env_LerpKeyValues = 0x101898E0u;
constexpr uint32_t FUN_10189980 = 0x10189980u;
constexpr uint32_t WeatherRes_IsTimeKeyName = 0x10189BD0u;
constexpr uint32_t FUN_10189c60 = 0x10189C60u;
constexpr uint32_t Env_FindPrevKey = 0x10189CA0u;
constexpr uint32_t FUN_10189e30 = 0x10189E30u;
constexpr uint32_t Env_FindNextKey = 0x10189F20u;
constexpr uint32_t FUN_1018a0f0 = 0x1018A0F0u;
constexpr uint32_t Env_UpdatePrevKey = 0x1018A220u;
constexpr uint32_t Env_UpdateNextKey = 0x1018A260u;
constexpr uint32_t Env_UpdateKeyAtOrBefore = 0x1018A2A0u;
constexpr uint32_t Env_UpdateKeyNeighbor = 0x1018A2C0u;
constexpr uint32_t Env_SetKeyField2C = 0x1018A2E0u;
constexpr uint32_t Env_SetKeyField30 = 0x1018A2F0u;
constexpr uint32_t Env_SetKeyField24 = 0x1018A300u;
constexpr uint32_t Env_SetKeyField28 = 0x1018A310u;
constexpr uint32_t Env_UpdateTimeKeys = 0x1018A320u;
constexpr uint32_t FUN_1018a340 = 0x1018A340u;
constexpr uint32_t YmCombineWeather_FindKind5A = 0x1018AAC0u;
constexpr uint32_t YmCombineWeather_FindKind5B = 0x1018AB40u;
constexpr uint32_t FUN_1018abb0 = 0x1018ABB0u;
constexpr uint32_t FUN_1018abf0 = 0x1018ABF0u;
constexpr uint32_t YmCombineWeather_Release = 0x1018ABF0u;
constexpr uint32_t Unit1018AD50_GetField4 = 0x1018AD50u;
constexpr uint32_t Unit1018AD50_SetField10Of4 = 0x1018AD60u;
constexpr uint32_t Blend_FindPrevious = 0x1018AD80u;
constexpr uint32_t Blend_FreeResult = 0x1018ADA0u;
constexpr uint32_t FUN_1018ada0 = 0x1018ADA0u;
constexpr uint32_t Unit1018AD50_CallBlendUpdate = 0x1018ADD0u;
constexpr uint32_t Blend_Update = 0x1018ADE0u;
constexpr uint32_t FUN_1018ade0 = 0x1018ADE0u;
constexpr uint32_t Blend_AllocResult = 0x1018AEB0u;
constexpr uint32_t FUN_1018aeb0 = 0x1018AEB0u;
constexpr uint32_t FUN_1018b130 = 0x1018B130u;
constexpr uint32_t sub_1018B130 = 0x1018B130u;
constexpr uint32_t FUN_1018b170 = 0x1018B170u;
constexpr uint32_t FUN_1018b620 = 0x1018B620u;
constexpr uint32_t XiZone_ResetState = 0x1018B620u;
constexpr uint32_t sub_1018B860 = 0x1018B860u;
constexpr uint32_t XiZone_Call10036170 = 0x1018B8B0u;
constexpr uint32_t XiZone_GetDirLightsForArea = 0x1018B960u;
constexpr uint32_t XiZone_GetDirLight = 0x1018B9A0u;
constexpr uint32_t XiZone_GetAmbient = 0x1018B9E0u;
constexpr uint32_t XiZone_GetAmbient_v1 = 0x1018B9E0u;
constexpr uint32_t XiZone_GetFog = 0x1018BA20u;
constexpr uint32_t XiZone_GetFog_v1 = 0x1018BA20u;
constexpr uint32_t XiZone_SetPtr1D8FromTable = 0x1018BAE0u;
constexpr uint32_t unknown_libname_158 = 0x1018BBB0u;
constexpr uint32_t Time_SetNormalized = 0x1018BBC0u;
constexpr uint32_t FUN_1018bc40 = 0x1018BC40u;
constexpr uint32_t Time_GetDiv5184000 = 0x1018BD30u;
constexpr uint32_t Time_SetModulo216000Div3600 = 0x1018BF00u;
constexpr uint32_t FUN_1018bf50 = 0x1018BF50u;
constexpr uint32_t Time_SetModulo155520000Div5184000 = 0x1018BFA0u;
constexpr uint32_t FUN_1018c180 = 0x1018C180u;
constexpr uint32_t FUN_1018c290 = 0x1018C290u;
constexpr uint32_t Time_SubtractFrameScale = 0x1018C290u;
constexpr uint32_t sub_1018C2B0 = 0x1018C2B0u;
constexpr uint32_t CXiSchStatus_Destructor = 0x1018C620u;
constexpr uint32_t Sch_GetStatusEntry = 0x1018CA20u;
constexpr uint32_t FUN_1018cea0 = 0x1018CEA0u;
constexpr uint32_t Unit1018BA60_GrowCapacity = 0x1018D230u;
constexpr uint32_t Unit1018BA60_Free = 0x1018D250u;
constexpr uint32_t Unit1018BA60_InitEmpty = 0x1018D280u;
constexpr uint32_t Unit1018BA60_Init = 0x1018D2B0u;
constexpr uint32_t Unit1018BA60_AssignCStr = 0x1018D2D0u;
constexpr uint32_t FUN_1018d3d0 = 0x1018D3D0u;
constexpr uint32_t Unit1018BA60_AssignWhole = 0x1018D430u;
constexpr uint32_t Unit1018BA60_Append = 0x1018D440u;
constexpr uint32_t Unit1018BA60_AppendBuf = 0x1018D450u;
constexpr uint32_t Unit1018BA60_GetCStr = 0x1018D470u;
constexpr uint32_t Unit1018BA60_GetLength = 0x1018D480u;
constexpr uint32_t Unit1018BA60_GetField4_1018D490 = 0x1018D490u;
constexpr uint32_t Unit1018BA60_AssignN = 0x1018D4A0u;
constexpr uint32_t Unit1018BA60_AppendN = 0x1018D580u;
constexpr uint32_t Unit1018BA60_InitAcqDat = 0x1018D640u;
constexpr uint32_t Unit1018BA60_FillCompletionWords = 0x1018D7F0u;
constexpr uint32_t Unit1018BA60_CommitPending8C = 0x1018D980u;
constexpr uint32_t Unit1018BA60_AppendCompletionWord = 0x1018DB10u;
constexpr uint32_t FUN_1018dc70 = 0x1018DC70u;
constexpr uint32_t Unit1018BA60_GetEntryMod = 0x1018DC70u;
constexpr uint32_t Unit1018BA60_ClearEntries = 0x1018DCB0u;
constexpr uint32_t Unit1018BA60_InitPecDat = 0x1018DCD0u;
constexpr uint32_t FsLogPage_InitPage = 0x1018E0A0u;
constexpr uint32_t FsLogPage_ClearAndVisitFiles = 0x1018E0E0u;
constexpr uint32_t FsLogPage_FormatFileName = 0x1018E170u;
constexpr uint32_t FsLogPage_AllocBuffer = 0x1018E1A0u;
constexpr uint32_t FsLogPage_load = 0x1018E1D0u;
constexpr uint32_t FsLogPage_WriteFile = 0x1018E310u;
constexpr uint32_t FsLogPage_AddString = 0x1018E4D0u;
constexpr uint32_t FsLogPage_ClearBuffer = 0x1018E660u;
constexpr uint32_t FsLogPage_PruneOffsets = 0x1018E6A0u;
constexpr uint32_t FsLogPage_Init = 0x1018E6F0u;
constexpr uint32_t FsLogPage_HasPage = 0x1018E7F0u;
constexpr uint32_t FsLogPage_EnsurePage = 0x1018E800u;
constexpr uint32_t FsLogPage_Rotate = 0x1018E8D0u;
constexpr uint32_t Unit1018BA60_ConstructKind1 = 0x1018E9B0u;
constexpr uint32_t Unit1018BA60_ConstructKind2 = 0x1018EA00u;
constexpr uint32_t Unit1018BA60_InitFields = 0x1018EA50u;
constexpr uint32_t Unit1018BA60_RecreateMembers = 0x1018EB00u;
constexpr uint32_t Text_ClassifyChar = 0x1018FA20u;
constexpr uint32_t Text_GetCharLength = 0x1018FA80u;
constexpr uint32_t Text_CountChars = 0x1018FB00u;
constexpr uint32_t Sjis_Remap82To83 = 0x1018FD90u;
constexpr uint32_t FUN_1018fdf0 = 0x1018FDF0u;
constexpr uint32_t Text_EncodeQuotes = 0x10190140u;
constexpr uint32_t Unit1018BA60_AllocZeroed = 0x101907A0u;
constexpr uint32_t Unit1018BA60_FreeBlock = 0x101907D0u;
constexpr uint32_t Unit1018BA60_Nop = 0x101908D0u;
constexpr uint32_t Unit1018BA60_Update10336950 = 0x10190BB0u;
constexpr uint32_t Unit1018BA60_AddPairChecked = 0x10193800u;
constexpr uint32_t SetConfig = 0x10193830u;
constexpr uint32_t Settings_GetValue = 0x10193850u;
constexpr uint32_t Settings_GetValue_v1 = 0x10193850u;
constexpr uint32_t Settings_GetValue_v2 = 0x10193850u;
constexpr uint32_t Unit1018BA60_GetSetting99 = 0x10193890u;
constexpr uint32_t Unit1018BA60_GetSettingMapped = 0x101938B0u;
constexpr uint32_t FUN_101938d0 = 0x101938D0u;
constexpr uint32_t Unit1018BA60_TestBit64 = 0x10193A10u;
constexpr uint32_t Unit1018BA60_InitPairNode = 0x10193B00u;
constexpr uint32_t Unit1018BA60_FindOrAdd = 0x10193B60u;
constexpr uint32_t Unit1018BA60_ForEach = 0x10193BC0u;
constexpr uint32_t Unit1018BA60_ResetConfigReturn1 = 0x10195220u;
constexpr uint32_t Unit1018BA60_AddPairAndCall = 0x10195240u;
constexpr uint32_t Unit1018BA60_SetEntryValue = 0x101952C0u;
constexpr uint32_t Unit1018BA60_GetEntryValue = 0x10195330u;
constexpr uint32_t Unit1018BA60_ReloadConfig = 0x10195350u;
constexpr uint32_t Unit1018BA60_ReadTigDatSetFlag = 0x101953A0u;
constexpr uint32_t Unit1018BA60_FlushThenTrue = 0x101953C0u;
constexpr uint32_t FUN_101953d0 = 0x101953D0u;
constexpr uint32_t Unit1018BA60_UpdateIfFlag9 = 0x101953F0u;
constexpr uint32_t Unit1018BA60_DefineEntry = 0x10195460u;
constexpr uint32_t Unit1018BA60_GetEntry2C = 0x101954D0u;
constexpr uint32_t Unit1018BA60_ReadTigDat = 0x10195570u;
constexpr uint32_t Unit1018BA60_StoreExternalConfig = 0x10195640u;
constexpr uint32_t Unit1018BA60_LoadExternalConfig = 0x101956F0u;
constexpr uint32_t Unit1018BA60_ReadExternalKind1 = 0x10195780u;
constexpr uint32_t Unit1018BA60_WriteExternalKind1 = 0x101957B0u;
constexpr uint32_t Unit1018BA60_ReadExternalKind2 = 0x10195810u;
constexpr uint32_t Unit1018BA60_IsExternalKind2Key = 0x101958C0u;
constexpr uint32_t Unit1018BA60_SaveCnfDat = 0x10195900u;
constexpr uint32_t Unit1018BA60_LoadCnfDat = 0x101959A0u;
constexpr uint32_t Unit1018BA60_SetConfigEntryValue = 0x10196040u;
constexpr uint32_t Unit1018BA60_ResetConfigToDefaults = 0x10196260u;
constexpr uint32_t Unit1018BA60_ConstructVt10336A0C = 0x101962D0u;
constexpr uint32_t Unit1018BA60_WindowItemCtor = 0x10196980u;
constexpr uint32_t Unit1018BA60_ReturnZero = 0x10196BE0u;
constexpr uint32_t Unit1018BA60_FrameItemCtor = 0x10197A90u;
constexpr uint32_t Unit1018BA60_WindowItemCtorVt10336D20 = 0x10199710u;
constexpr uint32_t Unit1018BA60_RunPairA = 0x1019C3E0u;
constexpr uint32_t Unit1018BA60_RunPairB = 0x1019C4F0u;
constexpr uint32_t Unit1018BA60_Call10311386 = 0x1019C580u;
constexpr uint32_t Unit1018BA60_GetSetting41 = 0x1019C590u;
constexpr uint32_t Unit1018BA60_Call103113C8 = 0x1019C5B0u;
constexpr uint32_t Unit1018BA60_ByteTableCtor = 0x1019CD40u;
constexpr uint32_t FUN_1019cdc0 = 0x1019CDC0u;
constexpr uint32_t Config_LoadDefaultTable = 0x1019CE70u;
constexpr uint32_t Unit1018BA60_FillTableBytes = 0x1019CF60u;
constexpr uint32_t FUN_1019cfa0 = 0x1019CFA0u;
constexpr uint32_t Config_LoadAltTable = 0x1019D1C0u;
constexpr uint32_t FUN_1019d2b0 = 0x1019D2B0u;
constexpr uint32_t FUN_1019d320 = 0x1019D320u;
constexpr uint32_t FUN_1019d350 = 0x1019D350u;
constexpr uint32_t Settings_CacheValues = 0x1019D8B0u;
constexpr uint32_t FUN_1019ead0 = 0x1019EAD0u;
constexpr uint32_t sub_1019EBB0 = 0x1019EBB0u;
constexpr uint32_t Unit1018BA60_PushWithRetry = 0x1019F8B0u;
constexpr uint32_t Unit1018BA60_FindMatchAt8 = 0x1019F930u;
constexpr uint32_t Unit1018BA60_WaitThenHandleFileEvent = 0x1019F960u;
constexpr uint32_t Unit1018BA60_WaitThenHandleFileEvent_v1 = 0x1019F960u;
constexpr uint32_t Unit1018BA60_WaitEvent18At8 = 0x1019FA90u;
constexpr uint32_t Unit1018BA60_SignalMember8 = 0x1019FAB0u;
constexpr uint32_t FsFileThreadManager_handleFileEvent = 0x1019FD50u;
constexpr uint32_t Unit1018BA60_SignalEvent10 = 0x101A0910u;
constexpr uint32_t Unit1018BA60_ResetAndWaitEvent14 = 0x101A0930u;
constexpr uint32_t Unit1018BA60_WaitEvent18 = 0x101A0950u;
constexpr uint32_t Unit1018BA60_SignalMember18 = 0x101A0970u;
constexpr uint32_t Unit1018BA60_Push = 0x101A0BE0u;
constexpr uint32_t Unit1018BA60_FindMatch = 0x101A0D30u;
constexpr uint32_t Unit1018BA60_StringListCtor = 0x101A1400u;
constexpr uint32_t Unit1018BA60_SetVtable10337210 = 0x101A1460u;
constexpr uint32_t Unit1018BA60_PushFrontString = 0x101A1470u;
constexpr uint32_t Unit1018BA60_CopyCurrent = 0x101A1500u;
constexpr uint32_t Unit1018BA60_SetCurrent = 0x101A1580u;
constexpr uint32_t Unit1018BA60_Advance = 0x101A1660u;
constexpr uint32_t Unit1018BA60_Retreat = 0x101A1690u;
constexpr uint32_t Unit1018BA60_ClearFirstString = 0x101A16B0u;
constexpr uint32_t Unit1018BA60_McrEntryCtor = 0x101A16E0u;
constexpr uint32_t Unit1018BA60_CopyMcrEntry = 0x101A1740u;
constexpr uint32_t Unit1018BA60_GetMcrEntryLine = 0x101A1910u;
constexpr uint32_t Unit1018BA60_Create = 0x101A19B0u;
constexpr uint32_t Unit1018BA60_McrManagerCtor = 0x101A1A20u;
constexpr uint32_t Unit1018BA60_QueueEntry = 0x101A1C60u;
constexpr uint32_t FUN_101a1cb0 = 0x101A1CB0u;
constexpr uint32_t Unit1018BA60_StepScript = 0x101A1CC0u;
constexpr uint32_t FUN_101a1e20 = 0x101A1E20u;
constexpr uint32_t FsMacroContainer_loadCallback = 0x101A1FE0u;
constexpr uint32_t FUN_101a23d0 = 0x101A23D0u;
constexpr uint32_t Unit1018BA60_ParseWaitCommand = 0x101A2480u;
constexpr uint32_t Unit1018BA60_FormatNumberedDatName = 0x101A2610u;
constexpr uint32_t FUN_101a2660 = 0x101A2660u;
constexpr uint32_t Unit1018BA60_LoadMacroDat = 0x101A26C0u;
constexpr uint32_t Unit1018BA60_SetReadPos = 0x101A2890u;
constexpr uint32_t Unit1018BA60_CtorVt10337314 = 0x101A2E00u;
constexpr uint32_t Unit1018BA60_SetFields1C20 = 0x101A2E60u;
constexpr uint32_t Unit1018BA60_UpdateMenuState = 0x101A2E80u;
constexpr uint32_t Unit1018BA60_SetSlot = 0x101A3300u;
constexpr uint32_t Unit1018BA60_CanOpenMenuShortcut = 0x101A33E0u;
constexpr uint32_t Unit1018BA60_ApplyKeyAction = 0x101A3500u;
constexpr uint32_t Unit1018BA60_HandleReadPosKeys = 0x101A3550u;
constexpr uint32_t Unit1018BA60_PanelCtorVt10337320 = 0x101A36F0u;
constexpr uint32_t Unit1018BA60_SetFlag29A = 0x101A3CF0u;
constexpr uint32_t FUN_101a3d00 = 0x101A3D00u;
constexpr uint32_t Unit1018BA60_PanelCtorVt10337378 = 0x101A3DC0u;
constexpr uint32_t Unit1018BA60_SetFlag29B = 0x101A43A0u;
constexpr uint32_t FUN_101a43b0 = 0x101A43B0u;
constexpr uint32_t Unit1018BA60_CtorVt103373BC = 0x101A4440u;
constexpr uint32_t Unit1018BA60_PanelCtorVt103373C8 = 0x101A46C0u;
constexpr uint32_t Unit1018BA60_PanelCtorVt10337418 = 0x101A4D70u;
constexpr uint32_t Unit1018BA60_CtorVt10337460 = 0x101A54F0u;
constexpr uint32_t Unit1018BA60_CtorVt103374A8 = 0x101A6430u;
constexpr uint32_t Unit1018BA60_CtorVt103374F0 = 0x101A6C10u;
constexpr uint32_t Unit1018BA60_CtorVt10337538 = 0x101A7280u;
constexpr uint32_t Unit1018BA60_CtorVt1033757C = 0x101A83C0u;
constexpr uint32_t Unit1018BA60_BuildSlotLinks = 0x101A8410u;
constexpr uint32_t Unit1018BA60_CtorVt10337580 = 0x101A8750u;
constexpr uint32_t Unit1018BA60_CreateAll = 0x101A8B50u;
constexpr uint32_t Unit1018BA60_CreateDatManager = 0x101A8D40u;
constexpr uint32_t sub_101A8D60 = 0x101A8D60u;
constexpr uint32_t Unit1018BA60_CreateEmptyObject = 0x101AB080u;
constexpr uint32_t Unit1018BA60_CreateMenuSystem = 0x101AB0D0u;
constexpr uint32_t Unit1018BA60_ReloadAll = 0x101AB730u;
constexpr uint32_t Unit1018BA60_HandleJumpMapKeys = 0x101AB770u;
constexpr uint32_t Unit1018BA60_LoadAllAndTimestamp = 0x101AB7D0u;
constexpr uint32_t Unit1018BA60_ReturnThis = 0x101AB810u;
constexpr uint32_t Unit1018BA60_HandleMenuKeys = 0x101AB820u;
constexpr uint32_t FsTextInput_Init = 0x101ABB40u;
constexpr uint32_t FsTextInput_SetText = 0x101ABC50u;
constexpr uint32_t FsTextInput_GetFlag = 0x101ABE40u;
constexpr uint32_t FsTextInput_ClearState = 0x101ABE50u;
constexpr uint32_t FsTextInput_GetCode = 0x101ABE80u;
constexpr uint32_t FsTextInput_RebuildAtCursor = 0x101ABF40u;
constexpr uint32_t FsTextInput_GetActiveBuffer = 0x101AC1B0u;
constexpr uint32_t FsTextInput_GetLength = 0x101AC200u;
constexpr uint32_t FsTextInput_GetMaxLength = 0x101AC230u;
constexpr uint32_t FsTextInput_ResetBuffer = 0x101AC580u;
constexpr uint32_t FUN_101ac5e0 = 0x101AC5E0u;
constexpr uint32_t FsTextInput_ExpandMarkedPhrase = 0x101AC5E0u;
constexpr uint32_t FsTextInput_BuildDisplayText = 0x101AC990u;
constexpr uint32_t FsTextInput_BuildDisplayTextTrimmed = 0x101ACAC0u;
constexpr uint32_t FsTextInput_CommitEntry = 0x101ACC30u;
constexpr uint32_t FsTextInput_SetCode4 = 0x101ACF40u;
constexpr uint32_t FsTextInput_MapCodeByte = 0x101ACF60u;
constexpr uint32_t FsTextInput_CopyCode4 = 0x101ACF70u;
constexpr uint32_t FsTextInput_ClearPair = 0x101ACF90u;
constexpr uint32_t FsTextInput_InsertAtCursor = 0x101ACFB0u;
constexpr uint32_t FsTextInput_IsMarkerAt = 0x101AD1B0u;
constexpr uint32_t Unit1018BA60_ProcessInputLine = 0x101AD4B0u;
constexpr uint32_t FsTextInput_GetDataPtr8 = 0x101AD820u;
constexpr uint32_t Unit1018BA60_IsSlashChar = 0x101ADBC0u;
constexpr uint32_t Unit1018BA60_Call101B72E0 = 0x101ADBD0u;
constexpr uint32_t Unit1018BA60_IsKnownCommand = 0x101ADBE0u;
constexpr uint32_t Shape_GetValueAt = 0x101ADDC0u;
constexpr uint32_t Unit1018BA60_ShortcutCtor = 0x101ADE00u;
constexpr uint32_t FsTextInput_SetName = 0x101ADE80u;
constexpr uint32_t Unit1018BA60_ShortcutSet = 0x101ADEC0u;
constexpr uint32_t Unit1018BA60_ShortcutCheck = 0x101ADF20u;
constexpr uint32_t FsShortcut_CanUseShortcut = 0x101AE010u;
constexpr uint32_t Unit1018BA60_UpdateShortcuts = 0x101AE3E0u;
constexpr uint32_t Unit1018BA60_NoOpC = 0x101AFF30u;
constexpr uint32_t ImmGetContext = 0x101AFF50u;
constexpr uint32_t ImmReleaseContext = 0x101AFF60u;
constexpr uint32_t Unit1018BA60_OnChar = 0x101B36F0u;
constexpr uint32_t Unit1018BA60_DtorVt103379BC = 0x101B45B0u;
constexpr uint32_t Unit1018BA60_CtorVt103379C0 = 0x101B45E0u;
constexpr uint32_t Unit1018BA60_CtorVt103379C4 = 0x101B4710u;
constexpr uint32_t Unit1018BA60_AppendByte = 0x101B4C20u;
constexpr uint32_t Unit1018BA60_ClearAll = 0x101B4F90u;
constexpr uint32_t Unit1018BA60_IsModeFourOrFive = 0x101B5C90u;
constexpr uint32_t Unit1018BA60_IsModeThree = 0x101B5CC0u;
constexpr uint32_t FsTextInput_OnChar = 0x101B6810u;
constexpr uint32_t Unit1018BA60_InputSubmit = 0x101B6C00u;
constexpr uint32_t Unit1018BA60_AcceptsChar = 0x101B6D50u;
constexpr uint32_t Unit1018BA60_GetFlag8C81 = 0x101B6E90u;
constexpr uint32_t Unit1018BA60_GetEntry = 0x101B6EC0u;
constexpr uint32_t Unit1018BA60_SetWordForCompletion = 0x101B6EE0u;
constexpr uint32_t Unit1018BA60_ResetBuffer = 0x101B7190u;
constexpr uint32_t Unit1018BA60_InputCall101AC990 = 0x101B7260u;
constexpr uint32_t Unit1018BA60_InputCall101AC990Swapped = 0x101B7280u;
constexpr uint32_t Unit1018BA60_InputCall101ACAC0 = 0x101B72B0u;
constexpr uint32_t FUN_101b72e0 = 0x101B72E0u;
constexpr uint32_t Unit1018BA60_IsState1Or3 = 0x101B7EE0u;
constexpr uint32_t sub_101B8E70 = 0x101B8E70u;
constexpr uint32_t Unit1018BA60_ReleaseSemaphoreAt4 = 0x101B9530u;
constexpr uint32_t Event_Wait = 0x101B9550u;
constexpr uint32_t Event_ResetAndWait = 0x101B95D0u;
constexpr uint32_t Event_Signal = 0x101B9610u;
constexpr uint32_t Unit1018BA60_CtorVt10337B1C = 0x101B9650u;
constexpr uint32_t FsTinyString_init = 0x101B9690u;
constexpr uint32_t Unit1018BA60_DatEntryCtor = 0x101B9880u;
constexpr uint32_t Unit1018BA60_InitWithCallback = 0x101B9910u;
constexpr uint32_t Unit1018BA60_CopyFrom = 0x101B9980u;
constexpr uint32_t Unit1018BA60_ClearVtable = 0x101B9A00u;
constexpr uint32_t Unit1018BA60_LoadFromFile = 0x101B9A10u;
constexpr uint32_t Unit1018BA60_Read = 0x101B9BC0u;
constexpr uint32_t Unit1018BA60_InitCopy = 0x101B9E00u;
constexpr uint32_t Unit1018BA60_CreateGlobal10662DB4 = 0x101B9E60u;
constexpr uint32_t Unit1018BA60_AppendToList = 0x101B9EC0u;
constexpr uint32_t Unit1018BA60_Register = 0x101B9F20u;
constexpr uint32_t Unit1018BA60_RemoveById = 0x101B9F90u;
constexpr uint32_t Unit1018BA60_LoadAll = 0x101B9FF0u;
constexpr uint32_t Unit1018BA60_ReadAll = 0x101BA030u;
constexpr uint32_t Unit1018BA60_Construct = 0x101BA180u;
constexpr uint32_t Unit1018BA60_CtorVt10337B2C = 0x101BA280u;
constexpr uint32_t Unit1018BA60_ClearBufferA = 0x101BA2D0u;
constexpr uint32_t Unit1018BA60_ClearBufferB = 0x101BA300u;
constexpr uint32_t Unit1018BA60_CtorVt10337B4C = 0x101BA570u;
constexpr uint32_t Unit1018BA60_Destruct = 0x101BA5B0u;
constexpr uint32_t Unit1018BA60_ResetCounters = 0x101BA5C0u;
constexpr uint32_t FUN_101badd0 = 0x101BADD0u;
constexpr uint32_t FsWordCompletion_GetCount18 = 0x101BAE20u;
constexpr uint32_t FsWordCompletion_GetCount1C = 0x101BAE30u;
constexpr uint32_t FsWordCompletion_FindNextByKey = 0x101BAE40u;
constexpr uint32_t FsWordCompletion_GetField20 = 0x101BAEB0u;
constexpr uint32_t FsWordCompletion_GetField28 = 0x101BAEC0u;
constexpr uint32_t FsWordCompletion_ClearEntries = 0x101BAF90u;
constexpr uint32_t FsWordCompletion_Init = 0x101BAFC0u;
constexpr uint32_t FUN_101bb070 = 0x101BB070u;
constexpr uint32_t FsWordCompletion_IsEntryValid = 0x101BB100u;
constexpr uint32_t FUN_101bb2b0 = 0x101BB2B0u;
constexpr uint32_t FsWordCompletion_BuildItems = 0x101BB4B0u;
constexpr uint32_t FUN_101bb680 = 0x101BB680u;
constexpr uint32_t FsWordCompletion_Refresh = 0x101BB740u;
constexpr uint32_t FsWordCompletion_LoadData = 0x101BB7A0u;
constexpr uint32_t FsWordCompletion_Release = 0x101BB9E0u;
constexpr uint32_t FsWordCompletion_SetWord = 0x101BBA90u;
constexpr uint32_t FsWordCompletion_Update = 0x101BBC60u;
constexpr uint32_t Unit1018BA60_IsState204Not1 = 0x101BBE50u;
constexpr uint32_t FsWordCompletion_getEntryList = 0x101BBE60u;
constexpr uint32_t FsWordCompletion_lookUp = 0x101BC100u;
constexpr uint32_t FsWordCompletion__prepareAllData = 0x101BC520u;
constexpr uint32_t Unit1018BA60_Store101BB070At8 = 0x101BC6F0u;
constexpr uint32_t Unit1018BA60_FreeBuffer = 0x101BC720u;
constexpr uint32_t FUN_101bc740 = 0x101BC740u;
constexpr uint32_t Unit1018BA60_Set = 0x101BC7B0u;
constexpr uint32_t Entry_CompareKey = 0x101BC900u;
constexpr uint32_t Unit1018BA60_GetUsed = 0x101BC9B0u;
constexpr uint32_t FsMemory_Constructor = 0x101BC9E0u;
constexpr uint32_t FUN_101bca30 = 0x101BCA30u;
constexpr uint32_t Unit1018BA60_AppendPadded = 0x101BCA40u;
constexpr uint32_t Unit1018BA60_ReserveZeroed = 0x101BCAC0u;
constexpr uint32_t Unit1018BA60_Clear = 0x101BCB30u;
constexpr uint32_t Unit1018BA60_CtorVt10337C3C = 0x101BCBA0u;
constexpr uint32_t Unit1018BA60_WatchConfigWindow = 0x101BCE30u;
constexpr uint32_t Unit1018BA60_CtorVt10337C5C = 0x101BCEF0u;
constexpr uint32_t Unit1018BA60_ClearTwoBuffers = 0x101BCF30u;
constexpr uint32_t Unit1018BA60_CtorVt10337C60 = 0x101BE260u;
constexpr uint32_t Unit1018BA60_SetVtable10337C60 = 0x101BE2A0u;
constexpr uint32_t Unit1018BA60_SetField4 = 0x101BE2B0u;
constexpr uint32_t Unit1018BA60_SetDateAt9 = 0x101BE2C0u;
constexpr uint32_t Unit1018BA60_SetDateAt11 = 0x101BE330u;
constexpr uint32_t Unit1018BA60_SetDateAt19 = 0x101BE3A0u;
constexpr uint32_t Unit1018BA60_SetVtable10337C64 = 0x101BE410u;
constexpr uint32_t File_FindByKind = 0x101BE470u;
constexpr uint32_t Unit1018BA60_Call10311F30 = 0x101BE500u;
constexpr uint32_t FUN_101be510 = 0x101BE510u;
constexpr uint32_t File_WriteChunked = 0x101BE590u;
constexpr uint32_t FUN_101be670 = 0x101BE670u;
constexpr uint32_t FUN_101be690 = 0x101BE690u;
constexpr uint32_t Unit1018BA60_FillSizeAndThreeDates = 0x101BE6C0u;
constexpr uint32_t File_CreateParentDirs = 0x101BE730u;
constexpr uint32_t FUN_101beb90 = 0x101BEB90u;
constexpr uint32_t XiFileImpl_QueueLoad = 0x101BECC0u;
constexpr uint32_t Unit1018BA60_SubmitFileNumKind1 = 0x101BF280u;
constexpr uint32_t Unit1018BA60_SubmitPathKind1 = 0x101BF370u;
constexpr uint32_t Unit1018BA60_SubmitPath2F4Kind1 = 0x101BF460u;
constexpr uint32_t Unit1018BA60_SubmitPath3F4Kind1 = 0x101BF550u;
constexpr uint32_t Unit1018BA60_QueueFileNumKind2 = 0x101BF970u;
constexpr uint32_t Unit1018BA60_QueuePathKind3 = 0x101C0050u;
constexpr uint32_t Unit1018BA60_QueuePath2F4Kind3 = 0x101C0160u;
constexpr uint32_t Unit1018BA60_QueuePath3F4Kind3 = 0x101C0260u;
constexpr uint32_t Unit1018BA60_SubmitPathKind3 = 0x101C0440u;
constexpr uint32_t Unit1018BA60_SubmitPath2F4Kind3 = 0x101C0540u;
constexpr uint32_t Unit1018BA60_SubmitPath3F4Kind3 = 0x101C0630u;
constexpr uint32_t Unit1018BA60_SubmitPath4F4Kind3 = 0x101C0760u;
constexpr uint32_t Unit1018BA60_CreateDirAndWait = 0x101C09B0u;
constexpr uint32_t Unit1018BA60_CreateDirAndHandleFile = 0x101C0A40u;
constexpr uint32_t FUN_101c0bc0 = 0x101C0BC0u;
constexpr uint32_t Unit1018BA60_SubmitPathKind6 = 0x101C0FC0u;
constexpr uint32_t Unit1018BA60_SubmitPath3F4Kind6 = 0x101C1090u;
constexpr uint32_t Unit1018BA60_FindFileNumKind1 = 0x101C11F0u;
constexpr uint32_t Unit1018BA60_FindFileNumKind2 = 0x101C13A0u;
constexpr uint32_t Unit1018BA60_FindPathKind3 = 0x101C1750u;
constexpr uint32_t Unit1018BA60_FindPath2F4Kind3 = 0x101C1820u;
constexpr uint32_t Unit1018BA60_FindPath3F4Kind3 = 0x101C18E0u;
constexpr uint32_t FUN_101c1a40 = 0x101C1A40u;
constexpr uint32_t FUN_101c1b10 = 0x101C1B10u;
constexpr uint32_t Unit1018BA60_SubmitFileNumKind4 = 0x101C1B20u;
constexpr uint32_t Unit1018BA60_SubmitPath3F4Kind4 = 0x101C1C90u;
constexpr uint32_t Unit1018BA60_SubmitPathKind4 = 0x101C1DF0u;
constexpr uint32_t Unit1018BA60_SubmitPath2F4Kind4 = 0x101C1EC0u;
constexpr uint32_t Unit1018BA60_SubmitPath3F4Kind5 = 0x101C2450u;
constexpr uint32_t FUN_101c2670 = 0x101C2670u;
constexpr uint32_t Unit1018BA60_SubmitPath3F4Kind7 = 0x101C2840u;
constexpr uint32_t FUN_101c2b60 = 0x101C2B60u;
constexpr uint32_t FUN_101c2c80 = 0x101C2C80u;
constexpr uint32_t Unit1018BA60_WriteTimestampFwd = 0x101C2D80u;
constexpr uint32_t FUN_101c2d90 = 0x101C2D90u;
constexpr uint32_t FUN_101c2e70 = 0x101C2E70u;
constexpr uint32_t Unit1018BA60_ReleaseAt1F0 = 0x101C33E0u;
constexpr uint32_t Unit1018BA60_GetValueAt1F0 = 0x101C33F0u;
constexpr uint32_t XiFileImpl_getFileName = 0x101C3A90u;
constexpr uint32_t Unit1018BA60_FormatUserPath = 0x101C3D00u;
constexpr uint32_t Unit1018BA60_FormatStrAt2F4 = 0x101C3DB0u;
constexpr uint32_t Unit1018BA60_FormatStrAt3F4 = 0x101C3DE0u;
constexpr uint32_t Unit1018BA60_FormatStrAt4F4 = 0x101C3E10u;
constexpr uint32_t Unit1018BA60_FormatWithBase = 0x101C3E40u;
constexpr uint32_t Unit1018BA60_FormatPathByKind = 0x101C3E90u;
constexpr uint32_t Unit1018BA60_CopyPathByMode = 0x101C3F40u;
constexpr uint32_t Unit1018BA60_MapCode012 = 0x101C3FB0u;
constexpr uint32_t Unit1018BA60_CreateDirectories = 0x101C3FE0u;
constexpr uint32_t FUN_101c4080 = 0x101C4080u;
constexpr uint32_t Unit1018BA60_WriteTimestamp = 0x101C4170u;
constexpr uint32_t Unit1018BA60_FileMngCall101BEB90 = 0x101C46A0u;
constexpr uint32_t FUN_101c46d0 = 0x101C46D0u;
constexpr uint32_t Unit1018BA60_FileMngQueueLoad = 0x101C46E0u;
constexpr uint32_t Unit1018BA60_ForwardOrExit = 0x101C4740u;
constexpr uint32_t Unit1018BA60_FileMngSubmitPathKind1 = 0x101C4790u;
constexpr uint32_t Unit1018BA60_FileMngSubmitPath2F4Kind1 = 0x101C47A0u;
constexpr uint32_t Unit1018BA60_FileMngSubmitPath3F4Kind1 = 0x101C47B0u;
constexpr uint32_t Unit1018BA60_FileMngQueueFileNumKind2 = 0x101C4820u;
constexpr uint32_t Unit1018BA60_FileMngQueuePathKind3 = 0x101C48E0u;
constexpr uint32_t Unit1018BA60_FileMngQueuePath2F4Kind3 = 0x101C48F0u;
constexpr uint32_t Unit1018BA60_FileMngQueuePath3F4Kind3 = 0x101C4900u;
constexpr uint32_t Unit1018BA60_FileMngSubmitPathKind3 = 0x101C4920u;
constexpr uint32_t Unit1018BA60_FileMngSubmitPathKind3_v1 = 0x101C4920u;
constexpr uint32_t Unit1018BA60_FileMngSubmitPath2F4Kind3 = 0x101C4930u;
constexpr uint32_t Unit1018BA60_FileMngSubmitPath4F4Kind3 = 0x101C4950u;
constexpr uint32_t Unit1018BA60_FileMngCall101C0A40 = 0x101C49A0u;
constexpr uint32_t Unit1018BA60_FileMngSubmitPathKind6 = 0x101C49E0u;
constexpr uint32_t Unit1018BA60_FileMngSubmitPath3F4Kind6 = 0x101C49F0u;
constexpr uint32_t Unit1018BA60_FileMngFindFileNumKind1 = 0x101C4A10u;
constexpr uint32_t Unit1018BA60_FileMngFindFileNumKind2 = 0x101C4A30u;
constexpr uint32_t Unit1018BA60_FileMngFwd101C1750 = 0x101C4A80u;
constexpr uint32_t Unit1018BA60_FileMngFwd101C1820 = 0x101C4A90u;
constexpr uint32_t Unit1018BA60_FileMngFwd101C18E0 = 0x101C4AA0u;
constexpr uint32_t Unit1018BA60_FileMngCall101C1A40 = 0x101C4AC0u;
constexpr uint32_t FUN_101c4ad0 = 0x101C4AD0u;
constexpr uint32_t FileMng_ResolveOrExit = 0x101C4AF0u;
constexpr uint32_t FileMng_ResolveOrExit_v1 = 0x101C4AF0u;
constexpr uint32_t Unit1018BA60_FileMngFwd101C1C90 = 0x101C4B40u;
constexpr uint32_t Unit1018BA60_FileMngFwd101C1DF0 = 0x101C4B60u;
constexpr uint32_t Unit1018BA60_FileMngFwd101C1EC0 = 0x101C4B70u;
constexpr uint32_t FUN_101c4bc0 = 0x101C4BC0u;
constexpr uint32_t Unit1018BA60_FileMngFwd101C2450 = 0x101C4BF0u;
constexpr uint32_t Unit1018BA60_FileMngCall101C2670 = 0x101C4C20u;
constexpr uint32_t Unit1018BA60_FileMngFwd101C2840 = 0x101C4C70u;
constexpr uint32_t Unit1018BA60_FileMngFwd101C2B60 = 0x101C4CC0u;
constexpr uint32_t Unit1018BA60_FileMngFwd101C2C80 = 0x101C4CD0u;
constexpr uint32_t Unit1018BA60_FileMngFwd101C4170 = 0x101C4CE0u;
constexpr uint32_t Unit1018BA60_FileMngCall101C2D90 = 0x101C4CF0u;
constexpr uint32_t Unit1018BA60_FileMngFwd101C2E70 = 0x101C4D10u;
constexpr uint32_t Unit1018BA60_FileMngReleaseAt1F0 = 0x101C4D30u;
constexpr uint32_t Unit1018BA60_FileMngGetValueAt1F0 = 0x101C4D40u;
constexpr uint32_t Unit1018BA60_EntriesCtorEmpty = 0x101C5920u;
constexpr uint32_t Unit1018BA60_EntriesCtor = 0x101C5930u;
constexpr uint32_t Unit1018BA60_AllocEntries = 0x101C59B0u;
constexpr uint32_t Unit1018BA60_EntriesSetCount = 0x101C5A30u;
constexpr uint32_t Unit1018BA60_SubmitFileOrExit = 0x101C5A40u;
constexpr uint32_t Unit1018BA60_CtorVt10337C90 = 0x101C5BE0u;
constexpr uint32_t Unit1018BA60_CommitTextInput = 0x101C5EC0u;
constexpr uint32_t Unit1018BA60_EndTextInput = 0x101C60C0u;
constexpr uint32_t FUN_101c6960 = 0x101C6960u;
constexpr uint32_t FUN_101c7c50 = 0x101C7C50u;
constexpr uint32_t Unit101C72E0_Construct = 0x101C7F80u;
constexpr uint32_t Unit101C72E0_Clear = 0x101C8070u;
constexpr uint32_t Unit101C72E0_FreeBuffer = 0x101C80A0u;
constexpr uint32_t Unit101C72E0_Load = 0x101C80C0u;
constexpr uint32_t Unit101C72E0_Init = 0x101C8260u;
constexpr uint32_t Unit101C72E0_ResizeEntries = 0x101C8290u;
constexpr uint32_t Unit101C72E0_LoadTable = 0x101C8380u;
constexpr uint32_t Unit101C72E0_MapChar = 0x101C87A0u;
constexpr uint32_t Unit101C72E0_CompareMapped = 0x101C88A0u;
constexpr uint32_t YkCommonInit = 0x101C8950u;
constexpr uint32_t Unit101C72E0_GetIndexed = 0x101C8C30u;
constexpr uint32_t Unit101C72E0_Checksum = 0x101C8C50u;
constexpr uint32_t Unit101C72E0_TimeToTmWrap28Years = 0x101C8D10u;
constexpr uint32_t Unit101C72E0_GetOffset = 0x101C8D70u;
constexpr uint32_t Gfx_ProjectWorldToScreen = 0x101C8DA0u;
constexpr uint32_t Unit101C72E0_MeasureText = 0x101C9550u;
constexpr uint32_t Unit101C72E0_DrawUnscaled = 0x101C9780u;
constexpr uint32_t Unit101C72E0_DrawTextItem0 = 0x101C97B0u;
constexpr uint32_t FUN_101c9b50 = 0x101C9B50u;
constexpr uint32_t Unit101C72E0_DrawString = 0x101C9D50u;
constexpr uint32_t Unit101C72E0_DrawTruncated = 0x101CA180u;
constexpr uint32_t Unit101C72E0_DrawFractionRange = 0x101CAD00u;
constexpr uint32_t Unit101C72E0_DrawFractionDefault = 0x101CAEC0u;
constexpr uint32_t Unit101C72E0_DrawSmoothed = 0x101CAF00u;
constexpr uint32_t Unit101C72E0_DrawSmoothedDefault = 0x101CB000u;
constexpr uint32_t Unit101C72E0_DrawFractionItem1A = 0x101CB040u;
constexpr uint32_t Unit101C72E0_DrawSmoothedItem1A = 0x101CB090u;
constexpr uint32_t FUN_101cb0d0 = 0x101CB0D0u;
constexpr uint32_t Unit101C72E0_DrawSmoothedExItem1B = 0x101CB230u;
constexpr uint32_t FUN_101cb280 = 0x101CB280u;
constexpr uint32_t Unit101C72E0_DrawEasedPercent = 0x101CB430u;
constexpr uint32_t Unit101C72E0_DrawEasedPercentUnitScale = 0x101CB500u;
constexpr uint32_t Unit101C72E0_DrawEasedPercentItem1A = 0x101CB550u;
constexpr uint32_t Unit101C72E0_BeginCapture = 0x101CB770u;
constexpr uint32_t Unit101C72E0_IsActorField170 = 0x101CBDF0u;
constexpr uint32_t Unit101C72E0_IsActorField170Is6 = 0x101CBE70u;
constexpr uint32_t Unit101C72E0_TestFlags100Or40DAC = 0x101CC070u;
constexpr uint32_t FUN_101cc0d0 = 0x101CC0D0u;
constexpr uint32_t Unit101C72E0_ActorTest1D0 = 0x101CC1D0u;
constexpr uint32_t Unit101C72E0_ActorPairTest1FF = 0x101CC220u;
constexpr uint32_t Unit101C72E0_CC4F0ActorTest400 = 0x101CC350u;
constexpr uint32_t FUN_101cc4f0 = 0x101CC4F0u;
constexpr uint32_t Unit101C72E0_GetCC4F0ActorByIndexAt1FA = 0x101CC510u;
constexpr uint32_t Acter_GetByIndexAt1FA = 0x101CC520u;
constexpr uint32_t Unit101C72E0_GetCC4F0ActorAt2A0 = 0x101CC550u;
constexpr uint32_t Unit101C72E0_GetActorAt2A0 = 0x101CC560u;
constexpr uint32_t FUN_101cc620 = 0x101CC620u;
constexpr uint32_t Yk_MapCodeOffset = 0x101CCE00u;
constexpr uint32_t YkFurniture_Destructor = 0x101CD900u;
constexpr uint32_t Unit101CD990_GetValueAt54 = 0x101CD9D0u;
constexpr uint32_t Unit101CD990_GetValueAt56 = 0x101CDA00u;
constexpr uint32_t Unit101CD990_GetValueAt5A = 0x101CDA30u;
constexpr uint32_t Unit101CF660_BuildTables = 0x101CF660u;
constexpr uint32_t Unit101CF660_BuildCodeTable = 0x101CF820u;
constexpr uint32_t YkJpeg_InitTables = 0x101CF870u;
constexpr uint32_t YkJpeg_CutColor = 0x101CFA00u;
constexpr uint32_t YkJpeg_CopyBlock = 0x101CFDB0u;
constexpr uint32_t YkJpeg_Unpack565 = 0x101CFE20u;
constexpr uint32_t YkJpeg_UnpackBlock565 = 0x101CFEB0u;
constexpr uint32_t YkJpeg_ForwardDct = 0x101CFF60u;
constexpr uint32_t FUN_101d01b0 = 0x101D01B0u;
constexpr uint32_t YkJpeg_EncodeBlock = 0x101D0210u;
constexpr uint32_t YkJpeg_Encode = 0x101D0370u;
constexpr uint32_t YkJpeg_GetCopyrightString = 0x101D05E0u;
constexpr uint32_t YkJpeg_EncodeChecked = 0x101D0600u;
constexpr uint32_t Unit101CF660_Reset = 0x101D0860u;
constexpr uint32_t Unit101CF660_Free = 0x101D0880u;
constexpr uint32_t YkJpeg_Destructor = 0x101D08C0u;
constexpr uint32_t YkJpeg_Release = 0x101D08D0u;
constexpr uint32_t YkJpgBit_Init = 0x101D08E0u;
constexpr uint32_t YkJpeg_PutBit = 0x101D0910u;
constexpr uint32_t YkJpeg_PutBits = 0x101D0970u;
constexpr uint32_t Unit101D0910_PutByte = 0x101D09C0u;
constexpr uint32_t YkJpeg_PutBytes = 0x101D09E0u;
constexpr uint32_t Unit101D0910_PutWordBE = 0x101D0A20u;
constexpr uint32_t Unit101D0910_PutFFThenByte = 0x101D0A40u;
constexpr uint32_t YkJpeg_FlushBits = 0x101D0A60u;
constexpr uint32_t sub_101D0B00 = 0x101D0B00u;
constexpr uint32_t CopyrightText_Format = 0x101D0C40u;
constexpr uint32_t sub_101D0C70 = 0x101D0C70u;
constexpr uint32_t Unit101D0910_SaveJpeg512x382 = 0x101D1000u;
constexpr uint32_t YkJpeg_CaptureAndEncode = 0x101D1060u;
constexpr uint32_t Unit101D0910_WriteWithFileMngA = 0x101D10C0u;
constexpr uint32_t Unit101D0910_SaveJpegScreenSize = 0x101D1100u;
constexpr uint32_t Unit101D0910_WriteWithFileMngB = 0x101D1180u;
constexpr uint32_t YkInit = 0x101D11C0u;
constexpr uint32_t FUN_101d2260 = 0x101D2260u;
constexpr uint32_t Unit101D0910_TestMask10663188 = 0x101D29C0u;
constexpr uint32_t YkMyroom_Destructor = 0x101D2F00u;
constexpr uint32_t YkMyroom_ReleaseObject38 = 0x101D30B0u;
constexpr uint32_t Unit101D2FF0_GetObjectAtIndex = 0x101D30F0u;
constexpr uint32_t YkMyroom_ReleaseResource8 = 0x101D31B0u;
constexpr uint32_t Unit101D2FF0_ReleaseTable = 0x101D3DC0u;
constexpr uint32_t Unit101D2FF0_Draw = 0x101D3FC0u;
constexpr uint32_t FUN_101d4a20 = 0x101D4A20u;
constexpr uint32_t FUN_101d5370 = 0x101D5370u;
constexpr uint32_t Unit101D2FF0_Update = 0x101D5680u;
constexpr uint32_t FUN_101d5fe0 = 0x101D5FE0u;
constexpr uint32_t Unit101D7020_CtorVt10338670 = 0x101D7C10u;
constexpr uint32_t Unit101D7020_CtorVt10338700 = 0x101D87A0u;
constexpr uint32_t Yk_CreateCommonObjects = 0x101D9B10u;
constexpr uint32_t Unit101D7020_CtorVt10338BE8 = 0x101D9E90u;
constexpr uint32_t Unit101D7020_CtorVt10338D88 = 0x101DC460u;
constexpr uint32_t Yk_CreateSecondaryObjects = 0x101DCE20u;
constexpr uint32_t Unit101D7020_CtorVt10338E60 = 0x101DD000u;
constexpr uint32_t Yk_InitSlots = 0x101DDA70u;
constexpr uint32_t FUN_101dda90 = 0x101DDA90u;
constexpr uint32_t Unit101D7020_CtorVt10338EA4 = 0x101DDF90u;
constexpr uint32_t Unit101D7020_CtorVt10338ED4 = 0x101DE6B0u;
constexpr uint32_t Unit101D7020_CtorVt10338F68 = 0x101DEDC0u;
constexpr uint32_t YkWndList_Init = 0x101DF210u;
constexpr uint32_t FUN_101e06c0 = 0x101E06C0u;
constexpr uint32_t FUN_101e0700 = 0x101E0700u;
constexpr uint32_t Unit101D7020_CtorVt10339250 = 0x101E0A90u;
constexpr uint32_t Unit101D7020_ClearField74 = 0x101E15F0u;
constexpr uint32_t Yk_ClearEntries = 0x101E1A00u;
constexpr uint32_t FUN_101e2880 = 0x101E2880u;
constexpr uint32_t Unit101D7020_CtorVt10339298 = 0x101E2B10u;
constexpr uint32_t Unit101D7020_CtorVt103394C0 = 0x101E3F50u;
constexpr uint32_t Unit101D7020_CtorVt10339530 = 0x101E48F0u;
constexpr uint32_t Unit101D7020_CtorVt10339698 = 0x101E4A20u;
constexpr uint32_t Unit101D7020_CtorVt10339728 = 0x101E51B0u;
constexpr uint32_t Unit101D7020_CtorVt10339770 = 0x101E5750u;
constexpr uint32_t Unit101D7020_CtorVt103397B8 = 0x101E5BA0u;
constexpr uint32_t FUN_101e6fb0 = 0x101E6FB0u;
constexpr uint32_t Unit101D7020_CtorVt10339D78 = 0x101EB390u;
constexpr uint32_t Unit101D7020_CtorVt10339DE8 = 0x101EB7E0u;
constexpr uint32_t Unit101D7020_CtorVt10339E48 = 0x101EBE00u;
constexpr uint32_t Unit101D7020_CtorVt10339EB8 = 0x101EC0D0u;
constexpr uint32_t FUN_101ec3c0 = 0x101EC3C0u;
constexpr uint32_t Unit101D7020_CtorVt10339FA0 = 0x101ED450u;
constexpr uint32_t Unit101D7020_CtorVt1033A000 = 0x101EE230u;
constexpr uint32_t Yk_ResetFields = 0x101EFCA0u;
constexpr uint32_t Unit101D7020_ClearSixFlags = 0x101EFEA0u;
constexpr uint32_t Unit101D7020_CtorVt1033A5D8 = 0x101F17C0u;
constexpr uint32_t Unit101D7020_CtorVt1033A6D0 = 0x101F43F0u;
constexpr uint32_t FUN_101f4af0 = 0x101F4AF0u;
constexpr uint32_t YkWndList_SetLine = 0x101F6700u;
constexpr uint32_t YkWndList_SetLine_v1 = 0x101F6700u;
constexpr uint32_t Yk_InitBase = 0x101F67C0u;
constexpr uint32_t Unit101D7020_GetValueIfLoaded = 0x101F71A0u;
constexpr uint32_t Yk_GetSharePercent = 0x101F71C0u;
constexpr uint32_t Unit101D7020_GetEntryFieldA = 0x101F7200u;
constexpr uint32_t Unit101D7020_GetEntryFieldB = 0x101F7240u;
constexpr uint32_t Unit101D7020_GetEntryFieldBPercent = 0x101F7260u;
constexpr uint32_t Yk_FindEntryIndex = 0x101F7430u;
constexpr uint32_t Unit101D7020_GetEntryCode = 0x101F7480u;
constexpr uint32_t Unit101D7020_GetEntryPosition = 0x101F74F0u;
constexpr uint32_t FUN_101f7570 = 0x101F7570u;
constexpr uint32_t Unit101D7020_FormatTargetMapGrid = 0x101F7600u;
constexpr uint32_t Unit101D7020_CtorVt1033A8B8 = 0x101F86C0u;
constexpr uint32_t Yk_ResetSlots = 0x101F92B0u;
constexpr uint32_t Unit101D7020_CtorVt1033A918 = 0x101FB710u;
constexpr uint32_t Unit101D7020_CtorVt1033A988 = 0x101FC2C0u;
constexpr uint32_t Unit101D7020_CtorVt1033A9D0 = 0x101FCBC0u;
constexpr uint32_t Unit101D7020_CtorVt1033AAB0 = 0x101FD000u;
constexpr uint32_t Unit101D7020_CtorVt1033AAF8 = 0x101FE430u;
constexpr uint32_t Unit101D7020_CtorVt1033ABE0 = 0x101FF000u;
constexpr uint32_t Unit101D7020_CtorVt1033AC28 = 0x10200B60u;
constexpr uint32_t Unit101D7020_InitFromTablesA = 0x10201020u;
constexpr uint32_t Unit101D7020_InitFromTablesB = 0x10201130u;
constexpr uint32_t FUN_10201180 = 0x10201180u;
constexpr uint32_t FUN_10201340 = 0x10201340u;
constexpr uint32_t Unit101D7020_CreateMenuObjects = 0x102017A0u;
constexpr uint32_t Unit101D7020_CtorVt1033B0C8 = 0x10201C90u;
constexpr uint32_t Yk_CheckFlagAndWord1C = 0x10201EB0u;
constexpr uint32_t Unit101D7020_IsSingleUncheckedEntry = 0x10201F00u;
constexpr uint32_t Yk_GetActiveEntry = 0x10202C90u;
constexpr uint32_t Unit101D7020_SetModeFwd = 0x10202D30u;
constexpr uint32_t Yk_SetMode = 0x10202D80u;
constexpr uint32_t FUN_10202f00 = 0x10202F00u;
constexpr uint32_t FUN_10202fe0 = 0x10202FE0u;
constexpr uint32_t Unit101D7020_FindActiveEntryById = 0x102031F0u;
constexpr uint32_t Yk_GetActerIfValid = 0x102033C0u;
constexpr uint32_t Yk_GetActerPtrA0 = 0x102034A0u;
constexpr uint32_t Unit101D7020_GetCurrentEntry = 0x102034E0u;
constexpr uint32_t Yk_GetEntryOrZero = 0x10203500u;
constexpr uint32_t Yk_SetCurrentIndex = 0x10203520u;
constexpr uint32_t Yk_FindIndexById = 0x10203540u;
constexpr uint32_t Unit101D7020_UpdateEntryCycle = 0x10203920u;
constexpr uint32_t FUN_10203be0 = 0x10203BE0u;
constexpr uint32_t Unit101D7020_CtorVt1033B110 = 0x10205500u;
constexpr uint32_t FUN_10206420 = 0x10206420u;
constexpr uint32_t Unit101D7020_CtorVt1033B248 = 0x10207730u;
constexpr uint32_t Unit101D7020_CtorVt1033B28C = 0x10208500u;
constexpr uint32_t Unit101D7020_CtorVt1033B340 = 0x10209300u;
constexpr uint32_t Unit101D7020_CtorVt1033B3A0 = 0x10209B90u;
constexpr uint32_t Unit101D7020_CtorVt1033B948 = 0x1020B500u;
constexpr uint32_t Unit101D7020_RecordInit = 0x1020BA40u;
constexpr uint32_t Yk_LoadIndexedTables = 0x1020BE90u;
constexpr uint32_t Unit101D7020_CtorVt1033B9D8 = 0x1020C470u;
constexpr uint32_t Unit101D7020_CtorVt1033BA20 = 0x1020CC30u;
constexpr uint32_t Unit101D7020_CtorVt1033BA68 = 0x1020ED00u;
constexpr uint32_t YkWndShop_Constructor = 0x10210310u;
constexpr uint32_t Unit101D7020_ResetState = 0x10210D60u;
constexpr uint32_t Unit101D7020_Init = 0x10210DA0u;
constexpr uint32_t Unit101D7020_Close = 0x10210FC0u;
constexpr uint32_t Unit101D7020_HandleChoice = 0x102112B0u;
constexpr uint32_t Unit101D7020_ApplyMusicVolume = 0x10211340u;
constexpr uint32_t FUN_102174f0 = 0x102174F0u;
constexpr uint32_t Unit10211440_CtorVt1033BD58 = 0x102175C0u;
constexpr uint32_t FUN_1021b330 = 0x1021B330u;
constexpr uint32_t Unit10211440_Create = 0x1021BB90u;
constexpr uint32_t Unit10211440_SetVtable1033BDA0 = 0x1021BC20u;
constexpr uint32_t Unit10211440_CopyRecordCtor = 0x1021BC30u;
constexpr uint32_t Unit10211440_ReturnTrue = 0x1021BCB0u;
constexpr uint32_t FUN_1021bcc0 = 0x1021BCC0u;
constexpr uint32_t Unit10211440_SaveToDat = 0x1021BDB0u;
constexpr uint32_t FUN_1021be50 = 0x1021BE50u;
constexpr uint32_t Unit10211440_LoadFromDat = 0x1021BF60u;
constexpr uint32_t Unit10211440_Construct = 0x1021C070u;
constexpr uint32_t Unit10211440_SaveAndRelease = 0x1021C100u;
constexpr uint32_t Unit10211440_CanUpdate = 0x1021C160u;
constexpr uint32_t Unit10211440_IsSpecialKey = 0x1021C1B0u;
constexpr uint32_t Unit10211440_AssignItem = 0x1021C210u;
constexpr uint32_t Unit10211440_ClearAndAssign = 0x1021C2E0u;
constexpr uint32_t Unit10211440_ResetSlot = 0x1021C350u;
constexpr uint32_t Unit10211440_SyncSlots = 0x1021C380u;
constexpr uint32_t Unit10211440_AppendEntry = 0x1021C5B0u;
constexpr uint32_t Unit10211440_RemoveByKey = 0x1021C680u;
constexpr uint32_t Unit10211440_RebuildWithGlobalFlag = 0x1021C7C0u;
constexpr uint32_t Unit10211440_Rebuild = 0x1021C7E0u;
constexpr uint32_t Unit10211440_ResetEntryCtor = 0x1021D250u;
constexpr uint32_t Unit10211440_Reset = 0x1021D270u;
constexpr uint32_t Unit10211440_LoadFromId = 0x1021D2D0u;
constexpr uint32_t Unit10211440_SetId = 0x1021D380u;
constexpr uint32_t CXiMovie_DestroyGlobal = 0x1021D460u;
constexpr uint32_t CXiMovie_DrawFrame = 0x1021D4C0u;
constexpr uint32_t CXiMovie_Construct = 0x1021D7F0u;
constexpr uint32_t Unit1021D930_InitAndMarkReady = 0x1021D930u;
constexpr uint32_t FUN_1021d960 = 0x1021D960u;
constexpr uint32_t Movie_Update = 0x1021D980u;
constexpr uint32_t Unit1021D930_ReleaseReturnTrue = 0x1021D9A0u;
constexpr uint32_t FUN_1021d9b0 = 0x1021D9B0u;
constexpr uint32_t FUN_1021dd50 = 0x1021DD50u;
constexpr uint32_t Unit1021D930_NoOp = 0x1021DDC0u;
constexpr uint32_t Movie_GetCurrentTexture = 0x1021DDF0u;
constexpr uint32_t Unit1021D930_FindByName = 0x1021E3B0u;
constexpr uint32_t Unit1021D930_FindForIds = 0x1021E480u;
constexpr uint32_t ShapeMng_FindShapes = 0x1021E4D0u;
constexpr uint32_t Unit1021D930_ClearArrays = 0x1021E500u;
constexpr uint32_t Unit1021D930_UpdateIndexList = 0x1021E550u;
constexpr uint32_t Unit1021D930_IsIdInTable10485A30 = 0x1021E670u;
constexpr uint32_t Unit1021D930_ResetFields = 0x1021E690u;
constexpr uint32_t Unit1021D930_Init = 0x1021E760u;
constexpr uint32_t Unit1021D930_RotateBytesByBitCount = 0x1021E870u;
constexpr uint32_t FUN_1021ed30 = 0x1021ED30u;
constexpr uint32_t Menu_SetPanelMetrics = 0x1021EE70u;
constexpr uint32_t FUN_1021ef10 = 0x1021EF10u;
constexpr uint32_t ActerBuff_FindNearest = 0x1021F030u;
constexpr uint32_t ActerBuff_CollectNearby = 0x1021F140u;
constexpr uint32_t FUN_1021f240 = 0x1021F240u;
constexpr uint32_t Unit1021D930_GetWindowRect = 0x1021F360u;
constexpr uint32_t FUN_1021f480 = 0x1021F480u;
constexpr uint32_t Unit1021D930_DrawEntryRows = 0x1021F950u;
constexpr uint32_t Unit1021D930_DrawLabelledRow = 0x1021FA90u;
constexpr uint32_t Unit1021D930_DrawTwoDigitValue = 0x1021FB10u;
constexpr uint32_t Unit1021D930_DrawBarRow = 0x1021FB80u;
constexpr uint32_t Unit1021D930_DrawNameRows = 0x1021FBD0u;
constexpr uint32_t Unit1021D930_DrawLabelBar = 0x102201A0u;
constexpr uint32_t Unit1021D930_RefreshTargetGridText = 0x10220350u;
constexpr uint32_t FUN_102203b0 = 0x102203B0u;
constexpr uint32_t ShapeMng_FillMenuShapes = 0x10220C80u;
constexpr uint32_t Menu_GetTableEntry = 0x10220CF0u;
constexpr uint32_t sub_10220EF0 = 0x10220EF0u;
constexpr uint32_t sub_10220F40 = 0x10220F40u;
constexpr uint32_t Unit1021D930_InitPanelState = 0x10220FD0u;
constexpr uint32_t FUN_10221030 = 0x10221030u;
constexpr uint32_t FUN_102211f0 = 0x102211F0u;
constexpr uint32_t FUN_10221230 = 0x10221230u;
constexpr uint32_t FUN_102212c0 = 0x102212C0u;
constexpr uint32_t FUN_102212e0 = 0x102212E0u;
constexpr uint32_t FUN_10221320 = 0x10221320u;
constexpr uint32_t FUN_10221340 = 0x10221340u;
constexpr uint32_t FUN_102213b0 = 0x102213B0u;
constexpr uint32_t FUN_102213e0 = 0x102213E0u;
constexpr uint32_t Unit1021D930_Draw = 0x102214D0u;
constexpr uint32_t FUN_10221680 = 0x10221680u;
constexpr uint32_t sub_10221770 = 0x10221770u;
constexpr uint32_t Unit1021D930_InitMenuShapes = 0x102217A0u;
constexpr uint32_t Unit1021D930_Construct = 0x102217C0u;
constexpr uint32_t Unit1021D930_InitRect = 0x10221900u;
constexpr uint32_t FUN_10221920 = 0x10221920u;
constexpr uint32_t FUN_102224c0 = 0x102224C0u;
constexpr uint32_t Unit1021D930_ResolveAlias = 0x10222C10u;
constexpr uint32_t Unit1021D930_ConstructFromScene = 0x102238E0u;
constexpr uint32_t Unit1021D930_GetObjectOnce = 0x10223930u;
constexpr uint32_t Unit1021D930_Ctor102239A0 = 0x102239A0u;
constexpr uint32_t Unit1021D930_LoadFromFile = 0x102239C0u;
constexpr uint32_t Unit1021D930_InitAndRegister = 0x10223BB0u;
constexpr uint32_t FUN_10223c30 = 0x10223C30u;
constexpr uint32_t Unit1021D930_Refresh = 0x10223C90u;
constexpr uint32_t Unit1021D930_ConstructFillMinus1 = 0x10223E60u;
constexpr uint32_t Unit1021D930_FindByteIndex = 0x10225090u;
constexpr uint32_t FUN_102250f0 = 0x102250F0u;
constexpr uint32_t Unit1021D930_ComputeCount = 0x10225120u;
constexpr uint32_t Unit1021D930_ComputeDelta = 0x10225220u;
constexpr uint32_t Unit1021D930_ConstructSetGlobals = 0x102254D0u;
constexpr uint32_t Unit1021D930_InitIndexMap128 = 0x10226080u;
constexpr uint32_t Unit1021D930_GetObject106670C8Once = 0x10226180u;
constexpr uint32_t Unit1021D930_Ctor102261F0 = 0x102261F0u;
constexpr uint32_t Unit1021D930_LoadDMsgTable = 0x10226210u;
constexpr uint32_t Unit1021D930_ConstructZeroFields = 0x10226550u;
constexpr uint32_t Unit1021D930_InitIndexMap64 = 0x10226C80u;
constexpr uint32_t FUN_10226e30 = 0x10226E30u;
constexpr uint32_t Unit1021D930_Alloc1800 = 0x10227080u;
constexpr uint32_t FUN_102270f0 = 0x102270F0u;
constexpr uint32_t FUN_10227100 = 0x10227100u;
constexpr uint32_t FUN_10227130 = 0x10227130u;
constexpr uint32_t Unit1021D930_ConstructAllocEntries = 0x102271B0u;
constexpr uint32_t Unit1021D930_LoadNamedEntries = 0x102271D0u;
constexpr uint32_t Unit1021D930_ConstructBuffersAt68 = 0x102278B0u;
constexpr uint32_t FUN_10227920 = 0x10227920u;
constexpr uint32_t FUN_10227930 = 0x10227930u;
constexpr uint32_t Unit1021D930_ClearHeaderFields = 0x102281C0u;
constexpr uint32_t Unit1021D930_ConstructBuffersAt8 = 0x10228200u;
constexpr uint32_t Unit1021D930_ConstructInitAt64 = 0x10229670u;
constexpr uint32_t Unit1021D930_TestBitForId = 0x1022AF80u;
constexpr uint32_t Unit1021D930_GetBitIndexForId = 0x1022AFB0u;
constexpr uint32_t Unit1021D930_IsBitSetForId = 0x1022AFF0u;
constexpr uint32_t Unit1021D930_ClearCounters = 0x1022B260u;
constexpr uint32_t Unit1021D930_SetTimerSeconds = 0x1022B3B0u;
constexpr uint32_t Unit1021D930_CtorVt1033C50C = 0x1022BD50u;
constexpr uint32_t Unit1021D930_GetSingleton = 0x1022BDA0u;
constexpr uint32_t Unit1021D930_SingletonCtor = 0x1022BE10u;
constexpr uint32_t Unit1021D930_LoadMessageTable = 0x1022BE30u;
constexpr uint32_t Unit1021D930_MixDatCtor = 0x1022BFB0u;
constexpr uint32_t Unit1021D930_PanelCtorVt1033C510 = 0x1022C350u;
constexpr uint32_t Unit1021D930_Clear = 0x1022ED00u;
constexpr uint32_t Unit1021D930_Alloc = 0x1022ED30u;
constexpr uint32_t Unit1021D930_Append = 0x1022ED70u;
constexpr uint32_t FUN_1022edf0 = 0x1022EDF0u;
constexpr uint32_t Unit1021D930_LoadStringTable = 0x1022EEE0u;
constexpr uint32_t Unit1021D930_ReadChunk = 0x1022F240u;
constexpr uint32_t Unit1021D930_PanelCtorVt1033C6B0 = 0x1022F610u;
constexpr uint32_t Unit1021D930_CtorVt1033C718 = 0x102303B0u;
constexpr uint32_t Unit1021D930_Reset = 0x10230410u;
constexpr uint32_t Unit1021D930_PanelCtorVt1033C720 = 0x10233BE0u;
constexpr uint32_t Unit1021D930_PanelCtorVt1033C788 = 0x102340A0u;
constexpr uint32_t Unit1021D930_SendMenuEventByKind = 0x10234910u;
constexpr uint32_t Unit1021D930_SetPair = 0x10234A70u;
constexpr uint32_t Ui_KeyPrompt_Init = 0x10239460u;
constexpr uint32_t Ui_KeyPrompt_Init_v1 = 0x10239460u;
constexpr uint32_t Unit10237880_GetGlobalDword = 0x10239490u;
constexpr uint32_t FUN_1023be10 = 0x1023BE10u;
constexpr uint32_t Unit10237880_ForwardFlag1 = 0x1023BF20u;
constexpr uint32_t FUN_10240860 = 0x10240860u;
constexpr uint32_t LobbyThread_NoOp = 0x10244F20u;
constexpr uint32_t LobbyThread_InitState = 0x10245040u;
constexpr uint32_t LobbyThread_SetGlobal106690AC = 0x10245B50u;
constexpr uint32_t LobbyThread_ResetZone = 0x10245BB0u;
constexpr uint32_t LobbyThread_IsSystemState1 = 0x10245BE0u;
constexpr uint32_t LobbyThread_NextSequence = 0x10245F30u;
constexpr uint32_t LobbyThread_SendCmd0 = 0x10246000u;
constexpr uint32_t LobbyThread_QueryChannels = 0x10246160u;
constexpr uint32_t LobbyThread_SendCmd4 = 0x10246320u;
constexpr uint32_t LobbyThread_SendCmd6 = 0x102463C0u;
constexpr uint32_t LobbyThread_QueueMusicFile = 0x102465E0u;
constexpr uint32_t LobbyThread_SendCmd11 = 0x102467F0u;
constexpr uint32_t LobbyThread_PushCmd = 0x1024C060u;
constexpr uint32_t LobbyThread_ConstructQueue = 0x1024C930u;
constexpr uint32_t StAvatarInit = 0x1024CF50u;
constexpr uint32_t StAvatar_InitEntriesFromTables = 0x1024D130u;
constexpr uint32_t StAvatar_Teardown = 0x1024D180u;
constexpr uint32_t StAvatar_ReleaseResources = 0x1024D2C0u;
constexpr uint32_t StAvatar_Reset = 0x1024D400u;
constexpr uint32_t StAvatar_RegisterTriggers = 0x1024D470u;
constexpr uint32_t StAvatar_SetTableValues = 0x1024D4A0u;
constexpr uint32_t StAvatar_SetChannel = 0x1024D540u;
constexpr uint32_t StAvatar_SetPending = 0x1024D5C0u;
constexpr uint32_t StAvatar_Unload = 0x1024D610u;
constexpr uint32_t StAvatar_ApplyChannels = 0x1024D6B0u;
constexpr uint32_t StAvatar_SetTransition = 0x1024D740u;
constexpr uint32_t StAvatar_StopAndFreeBuffers = 0x1024D790u;
constexpr uint32_t FUN_1024d850 = 0x1024D850u;
constexpr uint32_t StAvatar_Update = 0x1024DB60u;
constexpr uint32_t StAvatar_UpdateInput = 0x1024DD10u;
constexpr uint32_t StAvatar_UpdateCameraYaw = 0x1024DE20u;
constexpr uint32_t StAvatar_UpdateCameraFollow = 0x1024DFE0u;
constexpr uint32_t FUN_1024e280 = 0x1024E280u;
constexpr uint32_t StAvatar_SetFlagsBoth = 0x1024E720u;
constexpr uint32_t StAvatar_ProjectNodes = 0x1024F2C0u;
constexpr uint32_t StAvatar_UpdateSteer = 0x1024F5E0u;
constexpr uint32_t StAvatar_SetViewFromDancerCamera = 0x1024F900u;
constexpr uint32_t StAvatar_AdvanceTimers = 0x1024FC10u;
constexpr uint32_t FUN_1024fdf0 = 0x1024FDF0u;
constexpr uint32_t StAvatar_ProcessTriggers = 0x1024FEC0u;
constexpr uint32_t StAvatar_GetOffset = 0x1024FF50u;
constexpr uint32_t FUN_10250030 = 0x10250030u;
constexpr uint32_t StAvatar_RenderIfEnabled = 0x10250190u;
constexpr uint32_t StAvatarRender = 0x102501C0u;
constexpr uint32_t StAvatar_UpdateNodes = 0x102502D0u;
constexpr uint32_t StAvatarFini = 0x102504D0u;
constexpr uint32_t StAvatar_ResetChannels = 0x10250580u;
constexpr uint32_t StAvatar_SetFlagBit = 0x10250660u;
constexpr uint32_t StAvatar_SetStopped = 0x102506C0u;
constexpr uint32_t StAvatar_SetField10 = 0x10250760u;
constexpr uint32_t StAvatar_StoreMode = 0x102507C0u;
constexpr uint32_t StAvatar_Start = 0x10250800u;
constexpr uint32_t StAvatar_SetModelFlags = 0x102509C0u;
constexpr uint32_t StAvatar_SetModelItemFlags = 0x10250A40u;
constexpr uint32_t StAvatar_SetLock = 0x10250B10u;
constexpr uint32_t StAvatar_GetField20 = 0x10250C00u;
constexpr uint32_t StAvatar_InitIndices = 0x10250D80u;
constexpr uint32_t StChannelItem_Load = 0x10250DB0u;
constexpr uint32_t FUN_10250e10 = 0x10250E10u;
constexpr uint32_t FUN_10251020 = 0x10251020u;
constexpr uint32_t StChannel_RegisterItem = 0x10251070u;
constexpr uint32_t StChannel_ClearFlags = 0x10251160u;
constexpr uint32_t StChannel_MarkIfLoaded = 0x10251180u;
constexpr uint32_t StChannel_SetValue9C = 0x102511B0u;
constexpr uint32_t StChannel_ApplyItem = 0x10251200u;
constexpr uint32_t StChannelItemClear = 0x102512B0u;
constexpr uint32_t StAvatar_UpdateFlag2 = 0x10251320u;
constexpr uint32_t StAvatar_GetValue2B = 0x10251370u;
constexpr uint32_t StAvatar_ComputeRatioValue = 0x10251410u;
constexpr uint32_t StDancer_Update = 0x10251500u;
constexpr uint32_t sub_102515F0 = 0x102515F0u;
constexpr uint32_t StAvatar_SetActive = 0x10251620u;
constexpr uint32_t StAvatar_SetStateTeardown = 0x102516B0u;
constexpr uint32_t StAvatar_SetMode = 0x10251720u;
constexpr uint32_t StAvatar_SetLevel = 0x10251750u;
constexpr uint32_t Dancer_SetStateIfIdle = 0x10251780u;
constexpr uint32_t StDancer_SetJobLock = 0x10251890u;
constexpr uint32_t StAvatar_ResetOrSetFlag2 = 0x10251920u;
constexpr uint32_t StAvatar_IsFlag20 = 0x10251960u;
constexpr uint32_t Dancer_InitDevice = 0x10251A80u;
constexpr uint32_t StAvatar_ReleaseMemory = 0x10251CE0u;
constexpr uint32_t StAvatar_InitSlotRecord = 0x102524B0u;
constexpr uint32_t StModelItemClear = 0x102524E0u;
constexpr uint32_t StModelItem_ReadStart = 0x10252610u;
constexpr uint32_t FUN_102526f0 = 0x102526F0u;
constexpr uint32_t FUN_10252790 = 0x10252790u;
constexpr uint32_t Unit10252A00_Activate = 0x10252A00u;
constexpr uint32_t Unit10252A00_CheckFileRefs = 0x10252A40u;
constexpr uint32_t Unit10252A00_SetPositionAndScale = 0x10252AC0u;
constexpr uint32_t Unit10252A00_SetVec78 = 0x10252B50u;
constexpr uint32_t Unit10252A00_ClearModelItem = 0x10252C10u;
constexpr uint32_t Unit10252A00_Refresh = 0x10252C30u;
constexpr uint32_t Unit10252A00_Init = 0x10252C80u;
constexpr uint32_t StTrigger_ProcessOpcodesUpToTime = 0x10252CC0u;
constexpr uint32_t StTrigger_FindIndexAtLeast = 0x10252EA0u;
constexpr uint32_t StTrigger_RegisterCallbacks = 0x10252F00u;
constexpr uint32_t StTrigger_AllocFromPool = 0x10252F70u;
constexpr uint32_t StAvatar_CheckBuffersReady = 0x10253160u;
constexpr uint32_t sqopSystemActivate = 0x102531B0u;
constexpr uint32_t StAvatar_ClearResource = 0x10253210u;
constexpr uint32_t Unit10253210_GetInfoBuffer = 0x102532A0u;
constexpr uint32_t FUN_102532c0 = 0x102532C0u;
constexpr uint32_t Unit10253210_Ctor = 0x102532E0u;
constexpr uint32_t Unit10253210_NoOp = 0x10253330u;
constexpr uint32_t StAvatar_LoadInfoFile = 0x10253360u;
constexpr uint32_t StAvatar_GetInfoBuffer = 0x102534B0u;
constexpr uint32_t Unit10253210_SendBytePairKind0 = 0x102535F0u;
constexpr uint32_t Unit10253210_GetInfoBuffer0 = 0x10253620u;
constexpr uint32_t Unit10253210_SendBytePairKind2 = 0x10253670u;
constexpr uint32_t Unit10253210_SendBytePairKind2_v1 = 0x10253670u;
constexpr uint32_t Unit10253210_GetInfoBuffer2 = 0x102536A0u;
constexpr uint32_t Unit10253A20_InvertBytes = 0x102572E0u;
constexpr uint32_t FUN_10257310 = 0x10257310u;
constexpr uint32_t Unit10253A20_SwapEndianPair = 0x102573C0u;
constexpr uint32_t FUN_102573e0 = 0x102573E0u;
constexpr uint32_t Unit10253A20_GetEntryPtr = 0x102574F0u;
constexpr uint32_t Unit10253A20_GetCellData = 0x10257550u;
constexpr uint32_t Unit10253A20_Align4 = 0x10257590u;
constexpr uint32_t Unit10253A20_IsNotHashOrDot = 0x102575B0u;
constexpr uint32_t Unit10253A20_GetWord40 = 0x102575D0u;
constexpr uint32_t Unit10253A20_ComputeTotalSize = 0x102575E0u;
constexpr uint32_t Unit10253A20_Clone = 0x10257670u;
constexpr uint32_t Unit10253A20_GetDataOrDefault = 0x102577C0u;
constexpr uint32_t Unit10253A20_GetDwordEntry = 0x10257800u;
constexpr uint32_t Unit10253A20_GetDMsgTable = 0x10257830u;
constexpr uint32_t Unit10253A20_CallFreeData = 0x10257840u;
constexpr uint32_t Unit10253A20_FreeData = 0x10257850u;
constexpr uint32_t FUN_10257860 = 0x10257860u;
constexpr uint32_t Unit10253A20_GetRowBase = 0x10257870u;
constexpr uint32_t Unit10253A20_GetCell = 0x102578B0u;
constexpr uint32_t Unit10253A20_GetDwordCell = 0x10257920u;
constexpr uint32_t FUN_10257950 = 0x10257950u;
constexpr uint32_t Unit10253A20_GetCellCol0 = 0x10257950u;
constexpr uint32_t Unit10253A20_Assign = 0x10257980u;
constexpr uint32_t Unit10253A20_Bind = 0x102579B0u;
constexpr uint32_t Unit10253A20_GetRowCount = 0x10257AF0u;
constexpr uint32_t FUN_10257b10 = 0x10257B10u;
constexpr uint32_t Unit10253A20_IsBound = 0x10257B10u;
constexpr uint32_t Unit10253A20_Clear = 0x10257B60u;
constexpr uint32_t Unit10253A20_BindSource = 0x10257BA0u;
constexpr uint32_t Unit10253A20_GetRowBytes = 0x10257BC0u;
constexpr uint32_t Unit10253A20_BindDataAndMark = 0x10257BD0u;
constexpr uint32_t Unit10253A20_BindData = 0x10257BF0u;
constexpr uint32_t Unit10253A20_GetDataField28 = 0x10257C60u;
constexpr uint32_t Unit10253A20_GetStride = 0x10257C70u;
constexpr uint32_t Unit10253A20_GetRowInfo = 0x10257C80u;
constexpr uint32_t Unit10253A20_IsTypeRange = 0x10257FF0u;
constexpr uint32_t Unit10253A20_PickFormString = 0x10258150u;
constexpr uint32_t Unit10253A20_FindFirstTypeByte = 0x102581D0u;
constexpr uint32_t FUN_10258430 = 0x10258430u;
constexpr uint32_t FUN_10258600 = 0x10258600u;
constexpr uint32_t Unit10253A20_FindFirstTypeByteInText = 0x102586D0u;
constexpr uint32_t Unit10253A20_GetDataPtr = 0x102586F0u;
constexpr uint32_t Unit10253A20_FormatTextAtC = 0x10258740u;
constexpr uint32_t FUN_10258830 = 0x10258830u;
constexpr uint32_t FUN_102589f0 = 0x102589F0u;
constexpr uint32_t Unit10253A20_GetText14 = 0x10258AD0u;
constexpr uint32_t FUN_10258bb0 = 0x10258BB0u;
constexpr uint32_t Unit10253A20_PickFormString2 = 0x10258D00u;
constexpr uint32_t Unit10253A20_SelectQuantifierForm = 0x10258D70u;
constexpr uint32_t Unit10253A20_GetSolchForm = 0x10258E10u;
constexpr uint32_t Unit10253A20_SelectNounForm = 0x10258E50u;
constexpr uint32_t Unit10253A20_GetQuantifierByIndex = 0x10258ED0u;
constexpr uint32_t Unit10253A20_SelectAdjectiveForm = 0x10258F00u;
constexpr uint32_t Unit10253A20_SelectGenderForm = 0x10258F60u;
constexpr uint32_t Unit10253A20_Append = 0x10259100u;
constexpr uint32_t Unit10253A20_IsEmpty = 0x10259130u;
constexpr uint32_t Unit10253A20_FindNextTyped = 0x10259150u;
constexpr uint32_t Unit10253A20_IsTypeTwoZero = 0x1025A580u;
constexpr uint32_t Unit10253A20_GetFormById = 0x1025A5A0u;
constexpr uint32_t Unit10253A20_GetFormAlt = 0x1025A610u;
constexpr uint32_t Unit10253A20_GetSuffixByFlag = 0x1025A6C0u;
constexpr uint32_t Unit10253A20_GetEntryById = 0x1025A6F0u;
constexpr uint32_t Unit10253A20_GetPrefixByFlags = 0x1025A730u;
constexpr uint32_t Unit10253A20_GetPrefixByFlags2 = 0x1025A760u;
constexpr uint32_t Unit10253A20_AppendNode = 0x1025A860u;
constexpr uint32_t Unit10253A20_FindNextTyped9 = 0x1025A890u;
constexpr uint32_t Unit10253A20_ContainsChar = 0x1025A8C0u;
constexpr uint32_t Unit10253A20_BuildNodeString = 0x1025A930u;
constexpr uint32_t Unit10253A20_HasNoHead = 0x1025AA90u;
constexpr uint32_t Unit10253A20_LinkNode = 0x1025B100u;
constexpr uint32_t Unit10253A20_SelectTableByLanguage = 0x1025B4F0u;
constexpr uint32_t Scene_GetTableValue = 0x1025B560u;
constexpr uint32_t Scene_GetTableValue_v1 = 0x1025B560u;
constexpr uint32_t Scene_GetMenuFrameName = 0x1025B5A0u;
constexpr uint32_t Unit10253A20_CopyWithoutMarker = 0x1025B5E0u;
constexpr uint32_t Unit10253A20_CountChars = 0x1025B680u;
constexpr uint32_t Unit10253A20_HasKnownPrefix = 0x1025BB00u;
constexpr uint32_t FUN_1025bbe0 = 0x1025BBE0u;
constexpr uint32_t Unit10253A20_FindMarker = 0x1025BDE0u;
constexpr uint32_t Unit10253A20_IsWrapBreak = 0x1025BE20u;
constexpr uint32_t FUN_1025be80 = 0x1025BE80u;
constexpr uint32_t Unit10253A20_ConvertCharByLanguage = 0x1025BFB0u;
constexpr uint32_t FUN_1025c0f0 = 0x1025C0F0u;
constexpr uint32_t Unit10253A20_WrapSjisAt = 0x1025C0F0u;
constexpr uint32_t Unit1025C9E0_ReturnFalse = 0x1025E020u;
constexpr uint32_t Unit1025C9E0_ReturnFalse_v1 = 0x1025E020u;
constexpr uint32_t FontGlyphCache_IsStateTwo = 0x1025E050u;
constexpr uint32_t FUN_1025e090 = 0x1025E090u;
constexpr uint32_t FontGlyphCache_GetCounterBlock = 0x1025E0B0u;
constexpr uint32_t FontGlyphCache_ResetCounter = 0x1025E0C0u;
constexpr uint32_t FontGlyphCache_StepCounter = 0x1025E0E0u;
constexpr uint32_t Unit1025C9E0_ReturnFalseFwd = 0x1025E0F0u;
constexpr uint32_t FontGlyphCache_AddToCounter = 0x1025E210u;
constexpr uint32_t FontGlyphCache_ClearCounter = 0x1025E230u;
constexpr uint32_t FontGlyphCache_DecayCounter = 0x1025E240u;
constexpr uint32_t Unit1025C9E0_BaseCtor = 0x1025E260u;
constexpr uint32_t Unit1025C9E0_ReleaseField6C = 0x1025F2F0u;
constexpr uint32_t FUN_1025f640 = 0x1025F640u;
constexpr uint32_t Unit1025C9E0_DerivedCtor = 0x1025FD50u;
constexpr uint32_t Unit1025C9E0_ResetState = 0x1025FE40u;
constexpr uint32_t Unit1025C9E0_InnerCtor = 0x10261210u;
constexpr uint32_t Unit1025C9E0_InnerDtor = 0x102612D0u;
constexpr uint32_t FUN_10265940 = 0x10265940u;
constexpr uint32_t _dynamic_initializer_for__std__num_put_char_std__ostreambuf_iterator_char_std__char_traits_char_______id___47 = 0x10265960u;
constexpr uint32_t Unit1025C9E0_RefObjCtor = 0x102659B0u;
constexpr uint32_t FUN_10265b70 = 0x10265B70u;
constexpr uint32_t FUN_10265b90 = 0x10265B90u;
constexpr uint32_t FUN_1026cc30 = 0x1026CC30u;
constexpr uint32_t Unit1025C9E0_ChannelInit = 0x1026CCA5u;
constexpr uint32_t FUN_1026cdf4 = 0x1026CDF4u;
constexpr uint32_t FUN_1026cdf4_v1 = 0x1026CDF4u;
constexpr uint32_t Unit1025C9E0_MoveOneUsedToFree = 0x1026CFA0u;
constexpr uint32_t Unit1025C9E0_MoveAllBack = 0x1026D015u;
constexpr uint32_t Unit1025C9E0_RingPop = 0x1026D0CAu;
constexpr uint32_t FontGlyphCache_PeekRing = 0x1026D179u;
constexpr uint32_t FUN_1026d1b4 = 0x1026D1B4u;
constexpr uint32_t FUN_1026d634 = 0x1026D634u;
constexpr uint32_t Unit1025C9E0_Restart = 0x1026D70Cu;
constexpr uint32_t Unit1025C9E0_InitChannels = 0x1026D866u;
constexpr uint32_t FUN_1026d9b1 = 0x1026D9B1u;
constexpr uint32_t FUN_1026dac6 = 0x1026DAC6u;
constexpr uint32_t FontGlyphCache_MarkState3 = 0x1026DB24u;
constexpr uint32_t Unit1025C9E0_PopRingC044 = 0x1026DB6Du;
constexpr uint32_t Unit1025C9E0_PeekRingC044 = 0x1026DB8Fu;
constexpr uint32_t Unit1025C9E0_PutRing803C = 0x1026DBACu;
constexpr uint32_t FUN_1026dbce = 0x1026DBCEu;
constexpr uint32_t Unit1025C9E0_PopRing4034 = 0x1026DBCEu;
constexpr uint32_t FUN_1026dbf0 = 0x1026DBF0u;
constexpr uint32_t Unit1025C9E0_PeekRing4034 = 0x1026DBF0u;
constexpr uint32_t FUN_1026dc0d = 0x1026DC0Du;
constexpr uint32_t Unit1025C9E0_PutRing2C = 0x1026DC0Du;
constexpr uint32_t sqxfXformTD_Init = 0x1026DF00u;
constexpr uint32_t FUN_1026df90 = 0x1026DF90u;
constexpr uint32_t sqxfXformTD_Update = 0x1026E1F0u;
constexpr uint32_t sqxfXformTD_Update_v1 = 0x1026E1F0u;
constexpr uint32_t FUN_1026e2c0 = 0x1026E2C0u;
constexpr uint32_t FUN_1026e2f0 = 0x1026E2F0u;
constexpr uint32_t FUN_1026e350 = 0x1026E350u;
constexpr uint32_t SqgxContext_StoreVec14 = 0x1026E3D0u;
constexpr uint32_t StAvatar_InitGlobals1099AEB8 = 0x1026E400u;
constexpr uint32_t StAvatar_AllocArrays = 0x1026E460u;
constexpr uint32_t StAvatar_InitModel = 0x1026E530u;
constexpr uint32_t StAvatar_UpdateModels = 0x1026E590u;
constexpr uint32_t mdl_1026E900 = 0x1026E900u;
constexpr uint32_t Mdl_Register = 0x1026EBF0u;
constexpr uint32_t Mdl_FreeAllSlots = 0x1026ECC0u;
constexpr uint32_t Mdl_ClearSlotFlags = 0x1026EE00u;
constexpr uint32_t Mdl_ChannelRegister = 0x1026EF00u;
constexpr uint32_t FUN_1026efb0 = 0x1026EFB0u;
constexpr uint32_t FUN_1026f080 = 0x1026F080u;
constexpr uint32_t Mdl_ReleaseEntries = 0x1026F1B0u;
constexpr uint32_t Mdl_GetEntryTable = 0x1026F4F0u;
constexpr uint32_t Mdl_QueryEntryFlag = 0x1026F580u;
constexpr uint32_t Mdl_GetEntryWord0 = 0x1026F7B0u;
constexpr uint32_t Mdl_SetChannelActive = 0x1026F900u;
constexpr uint32_t Mdl_UpdateEntryFlags = 0x1026F9A0u;
constexpr uint32_t Mdl_SetChannelFlag80 = 0x1026F9E0u;
constexpr uint32_t Unit1026EA00_ComputeMinMax = 0x1026FBB0u;
constexpr uint32_t Mdl_ComputeSlotBounds = 0x1026FF80u;
constexpr uint32_t Mdl_SetSlotLock = 0x102701B0u;
constexpr uint32_t Mdl_GetSlotWord0 = 0x10270290u;
constexpr uint32_t Mdl_UpdateSlotFlags = 0x102703C0u;
constexpr uint32_t Mdl_SetSlotVisible = 0x10270410u;
constexpr uint32_t Mdl_ReleaseSlotPtr = 0x10270690u;
constexpr uint32_t FUN_10270840 = 0x10270840u;
constexpr uint32_t FUN_102709e0 = 0x102709E0u;
constexpr uint32_t Unit102706F0_InitScene = 0x10270B60u;
constexpr uint32_t StAvatar_UpdateTransform = 0x10270EC0u;
constexpr uint32_t Unit102706F0_UpdateScene = 0x10270F30u;
constexpr uint32_t StAvatar_ApplyModes = 0x10271030u;
constexpr uint32_t FUN_102710b0 = 0x102710B0u;
constexpr uint32_t FUN_102712e0 = 0x102712E0u;
constexpr uint32_t StAvatar_ApplyEffectFlags = 0x102715F0u;
constexpr uint32_t StAvatar_UpdateAngles = 0x10271630u;
constexpr uint32_t StAvatar_DrawScene = 0x102717F0u;
constexpr uint32_t scnFini = 0x10271B40u;
constexpr uint32_t scnFini_v1 = 0x10271B40u;
constexpr uint32_t StAvatar_UpdateFlagBits = 0x10271B90u;
constexpr uint32_t StAvatar_SetOffsets = 0x10271C30u;
constexpr uint32_t StAvatar_SetSceneValue1 = 0x10271C60u;
constexpr uint32_t StAvatar_SetSceneValue2 = 0x10271C80u;
constexpr uint32_t StAvatar_StoreParams = 0x10271CA0u;
constexpr uint32_t StAvatar_StoreEntry = 0x10271D00u;
constexpr uint32_t StAvatar_SelectEntry = 0x10271D70u;
constexpr uint32_t scnGetFlags = 0x10271E90u;
constexpr uint32_t scnGetFlags_v1 = 0x10271E90u;
constexpr uint32_t StAvatar_InitEntry = 0x10271F20u;
constexpr uint32_t Unit102706F0_NoOpA = 0x102720A0u;
constexpr uint32_t StAvatar_SetEntryPosition = 0x102725A0u;
constexpr uint32_t StAvatar_SetEntryFlag = 0x10272630u;
constexpr uint32_t StAvatar_SetFlag3 = 0x102726E0u;
constexpr uint32_t StAvatar_SetEntryFlag2 = 0x10272750u;
constexpr uint32_t StAvatar_SetEntryColor = 0x102727F0u;
constexpr uint32_t StAvatar_SetEntryTransform = 0x10272880u;
constexpr uint32_t StAvatar_ToggleFlag168 = 0x102729E0u;
constexpr uint32_t StAvatar_SelectIndex = 0x10272A10u;
constexpr uint32_t Unit102706F0_NoOpB = 0x10272BA0u;
constexpr uint32_t scnCameraGetInfo = 0x10273370u;
constexpr uint32_t scnCameraGetInfo_v1 = 0x10273370u;
constexpr uint32_t Unit10272F20_SelectCamera = 0x102733B0u;
constexpr uint32_t Unit10272F20_SetCameraEntry = 0x10273490u;
constexpr uint32_t Unit10272F20_UpdateCameraBlend = 0x10273560u;
constexpr uint32_t Unit10272F20_SetVtable1099B138 = 0x102736D0u;
constexpr uint32_t Unit10272F20_ApplySceneToModel = 0x102736E0u;
constexpr uint32_t FUN_10273810 = 0x10273810u;
constexpr uint32_t Unit10273840_ClearGroupField4 = 0x10273870u;
constexpr uint32_t Unit10273840_ClearCurrentField4 = 0x102738A0u;
constexpr uint32_t Unit10273840_InitRecord = 0x102738D0u;
constexpr uint32_t chasm_10273920 = 0x10273920u;
constexpr uint32_t Chasm_ToggleLayerState = 0x10273E80u;
constexpr uint32_t FUN_10273e80 = 0x10273E80u;
constexpr uint32_t Unit10274050_Relink = 0x102741F0u;
constexpr uint32_t FUN_102742c0 = 0x102742C0u;
constexpr uint32_t Unit10274050_GetInterpolated = 0x10274370u;
constexpr uint32_t Unit10274050_ZeroDwords = 0x10274640u;
constexpr uint32_t FUN_10274650 = 0x10274650u;
constexpr uint32_t Unit10274050_SetXyz = 0x10274670u;
constexpr uint32_t FUN_102746c0 = 0x102746C0u;
constexpr uint32_t FUN_10274870 = 0x10274870u;
constexpr uint32_t FUN_102748f0 = 0x102748F0u;
constexpr uint32_t Unit10274050_Sub = 0x102749C0u;
constexpr uint32_t Unit10274050_LerpFactor = 0x10274A20u;
constexpr uint32_t PackVec4 = 0x10274D40u;
constexpr uint32_t PackVec4_v1 = 0x10274D40u;
constexpr uint32_t FUN_10275090 = 0x10275090u;
constexpr uint32_t sub_10275090 = 0x10275090u;
constexpr uint32_t sqMemInit = 0x10275150u;
constexpr uint32_t SqMem_InitBlock = 0x10275190u;
constexpr uint32_t FUN_102751d0 = 0x102751D0u;
constexpr uint32_t SqMem_ClassifyBlock = 0x102752F0u;
constexpr uint32_t SqMem_GetPayloadEnd = 0x10275320u;
constexpr uint32_t SqMem_Check = 0x10275510u;
constexpr uint32_t SqMem_HexDump = 0x102755C0u;
constexpr uint32_t SqMem_FindArena = 0x10275670u;
constexpr uint32_t FUN_102756a0 = 0x102756A0u;
constexpr uint32_t sqMemPoolRegister = 0x10275B60u;
constexpr uint32_t SqMem_AllocZeroed = 0x10275D40u;
constexpr uint32_t FUN_10275d80 = 0x10275D80u;
constexpr uint32_t SqMem_Realloc = 0x10275E90u;
constexpr uint32_t sqFree = 0x102760F0u;
constexpr uint32_t sqFree_v1 = 0x102760F0u;
constexpr uint32_t sqgxContext_Create = 0x10276260u;
constexpr uint32_t sqgxContext_Init = 0x10276290u;
constexpr uint32_t FUN_10276770 = 0x10276770u;
constexpr uint32_t sqgxContext_Dump = 0x102768E0u;
constexpr uint32_t SqmdModel_Validate = 0x10277A50u;
constexpr uint32_t SqmdModel_Validate_v1 = 0x10277A50u;
constexpr uint32_t SqmdModel_Free = 0x10277AB0u;
constexpr uint32_t SqmdModel_Free_v1 = 0x10277AB0u;
constexpr uint32_t SqmdModel_FreeBuffers = 0x10277B70u;
constexpr uint32_t FUN_10277ff0 = 0x10277FF0u;
constexpr uint32_t SqmdModel_Update = 0x10278490u;
constexpr uint32_t SqmdModel_Update_v1 = 0x10278490u;
constexpr uint32_t SqmdModel_Link = 0x10278710u;
constexpr uint32_t SqmdModel_LinkShape = 0x102787F0u;
constexpr uint32_t SqmdModel_AddIllumLight = 0x10278990u;
constexpr uint32_t SqmdModel_AddShadowLightInLayer = 0x10278B50u;
constexpr uint32_t SqmdModel_SetLayerSort = 0x10278E10u;
constexpr uint32_t SqmdModel_LookAt = 0x10278E90u;
constexpr uint32_t SqmdModel_GetItem = 0x10279710u;
constexpr uint32_t SqmdModel_GetItemKind0 = 0x10279830u;
constexpr uint32_t SqmdModel_GetItemKind1 = 0x10279850u;
constexpr uint32_t SqmdModel_GetItemKind2 = 0x10279870u;
constexpr uint32_t SqmdModel_GetItemKind3 = 0x10279890u;
constexpr uint32_t SqmdModel_GetItemKind4 = 0x102798B0u;
constexpr uint32_t sqmdModel_GetItemName = 0x10279AB0u;
constexpr uint32_t SqmdModel_GetItemNameKind3 = 0x10279B40u;
constexpr uint32_t SqmdModel_FindItemName = 0x10279BA0u;
constexpr uint32_t SqmdModel_FindItemByName = 0x10279BF0u;
constexpr uint32_t SqmdModel_LookupKind2 = 0x10279D50u;
constexpr uint32_t SqmdModel_ItemKindName = 0x1027A1C0u;
constexpr uint32_t FUN_1027a220 = 0x1027A220u;
constexpr uint32_t SqmdModel_FreeItems = 0x1027A250u;
constexpr uint32_t Unit1027A4B0_SetIdentityFwd = 0x1027A6F0u;
constexpr uint32_t Unit1027A4B0_Copy = 0x1027A700u;
constexpr uint32_t Unit1027A4B0_Set = 0x1027A720u;
constexpr uint32_t FUN_1027a8a0 = 0x1027A8A0u;
constexpr uint32_t Unit1027A4B0_ToMatrix3 = 0x1027A9D0u;
constexpr uint32_t FUN_1027aae0 = 0x1027AAE0u;
constexpr uint32_t Unit1027A4B0_FromRotationX = 0x1027ABA0u;
constexpr uint32_t Unit1027A4B0_FromRotationY = 0x1027ABD0u;
constexpr uint32_t Unit1027A4B0_FromAxisAngle = 0x1027AC30u;
constexpr uint32_t Unit1027A4B0_SetIdentity = 0x1027AE50u;
constexpr uint32_t FUN_1027aec0 = 0x1027AEC0u;
constexpr uint32_t Unit1027A4B0_Negate = 0x1027AEF0u;
constexpr uint32_t SqQuat_Invert = 0x1027AFB0u;
constexpr uint32_t Unit1027B060_NegateInPlace = 0x1027B060u;
constexpr uint32_t Unit1027B060_RotateVec = 0x1027B080u;
constexpr uint32_t Unit1027B060_Mul = 0x1027B170u;
constexpr uint32_t FUN_1027b2c0 = 0x1027B2C0u;
constexpr uint32_t FUN_1027b480 = 0x1027B480u;
constexpr uint32_t SqskSkeleton_Create = 0x1027B580u;
constexpr uint32_t SqskSkeleton_Init = 0x1027B5A0u;
constexpr uint32_t FUN_1027b610 = 0x1027B610u;
constexpr uint32_t SqskSkeleton_SetNodeCount = 0x1027C4C0u;
constexpr uint32_t SqskSkeleton_FindBoneIndex = 0x1027C900u;
constexpr uint32_t SqskSkeleton_GetNode = 0x1027C980u;
constexpr uint32_t SqskSkeleton_Release = 0x1027C9D0u;
constexpr uint32_t SqskSkeleton_Release_v1 = 0x1027C9D0u;
constexpr uint32_t Unit1027CCF0_TransformOrigin = 0x1027CCF0u;
constexpr uint32_t Unit1027CCF0_ComputeTransform = 0x1027CD30u;
constexpr uint32_t Unit1027CCF0_TransformPoint = 0x1027CFD0u;
constexpr uint32_t FUN_1027d110 = 0x1027D110u;
constexpr uint32_t SqmoUtil_MarkSlot = 0x1027D650u;
constexpr uint32_t Unit1027D980_InitDefaults = 0x1027DAE0u;
constexpr uint32_t Chn_ReleaseRefs = 0x1027DB80u;
constexpr uint32_t Chn_RegisterItem = 0x1027DCC0u;
constexpr uint32_t Chn_ForwardIfFlag2 = 0x1027DD70u;
constexpr uint32_t FUN_1027dd90 = 0x1027DD90u;
constexpr uint32_t Chn_GetSubIndex = 0x1027DF70u;
constexpr uint32_t Chn_InitDefaults = 0x1027E060u;
constexpr uint32_t Chn_ReleaseIfSet = 0x1027E0D0u;
constexpr uint32_t Unit1027E120_UpdateMask = 0x1027E150u;
constexpr uint32_t Unit1027E120_SetCallback = 0x1027E190u;
constexpr uint32_t Unit1027E120_ReleaseConnection = 0x1027E3B0u;
constexpr uint32_t Unit1027E120_IsConnected = 0x1027E440u;
constexpr uint32_t Unit1027E120_UpdateRange = 0x1027E450u;
constexpr uint32_t Unit1027E120_Advance = 0x1027E490u;
constexpr uint32_t Unit1027E120_Forward1027EBD0 = 0x1027EAF0u;
constexpr uint32_t FUN_1027eb00 = 0x1027EB00u;
constexpr uint32_t Unit1027E120_Zero = 0x1027EB00u;
constexpr uint32_t M4fSet = 0x1027EB10u;
constexpr uint32_t M4fSet_v1 = 0x1027EB10u;
constexpr uint32_t Unit1027E120_Set = 0x1027EB50u;
constexpr uint32_t FUN_1027ebd0 = 0x1027EBD0u;
constexpr uint32_t M4fTransform = 0x1027EF20u;
constexpr uint32_t M4fTransform_v1 = 0x1027EF20u;
constexpr uint32_t Unit1027E120_TransformVec3 = 0x1027EF80u;
constexpr uint32_t Mat4x4_TransformVec4 = 0x1027EFE0u;
constexpr uint32_t M4fTranspose = 0x1027F070u;
constexpr uint32_t M4fTranspose_v1 = 0x1027F070u;
constexpr uint32_t sqMatrix4_1027F0E0 = 0x1027F0E0u;
constexpr uint32_t sqMatrix4_1027F0E0_v1 = 0x1027F0E0u;
constexpr uint32_t sqMatrix4_1027FCC0 = 0x1027FCC0u;
constexpr uint32_t sqMatrix4_10280140 = 0x10280140u;
constexpr uint32_t sqMatrix4_10280140_v1 = 0x10280140u;
constexpr uint32_t SqMatrix4_ScaleAxes = 0x102806B0u;
constexpr uint32_t sqgxSprite = 0x10280DE0u;
constexpr uint32_t sqgxSprite_v1 = 0x10280DE0u;
constexpr uint32_t Unit10280C40_CompositeBlendPasses = 0x102813B0u;
constexpr uint32_t ___setargv_3 = 0x10281500u;
constexpr uint32_t sqgxTextureCreateFromImage = 0x10281510u;
constexpr uint32_t sqgxTextureAddZ = 0x102816E0u;
constexpr uint32_t sqgxCeilLog2 = 0x10281810u;
constexpr uint32_t sqgxTextureAdd = 0x10281830u;
constexpr uint32_t sqgxTextureAdd_v1 = 0x10281830u;
constexpr uint32_t SqgxTexture_ClearPaletteUsed = 0x102819C0u;
constexpr uint32_t sqgxTextureClutAdd = 0x102819D0u;
constexpr uint32_t SqgxTexture_FreeSlot = 0x10281A70u;
constexpr uint32_t sqgxTextureGetSize = 0x10281B60u;
constexpr uint32_t sqgxTextureGetSize_v1 = 0x10281B60u;
constexpr uint32_t sqgxTextureSet = 0x10281C20u;
constexpr uint32_t sqgxTextureSet_v1 = 0x10281C20u;
constexpr uint32_t sqgxTextureDecal = 0x10281C50u;
constexpr uint32_t sqgxTextureDecal_v1 = 0x10281C50u;
constexpr uint32_t sqgxTextureClamp = 0x10281C80u;
constexpr uint32_t sqgxTextureClamp_v1 = 0x10281C80u;
constexpr uint32_t sqgxTextureSetPalette = 0x10281D60u;
constexpr uint32_t sqgxTextureUpdate = 0x10281DB0u;
constexpr uint32_t sqgxTextureUpdate_v1 = 0x10281DB0u;
constexpr uint32_t sqgxTextureStencilClear = 0x10282010u;
constexpr uint32_t sqgxTextureClear = 0x10282030u;
constexpr uint32_t sqgxTextureClear_v1 = 0x10282030u;
constexpr uint32_t sqgxTextureDraw = 0x10282190u;
constexpr uint32_t sqgxTextureDraw_v1 = 0x10282190u;
constexpr uint32_t sqgxZTest = 0x10282350u;
constexpr uint32_t sqgxZTest_v1 = 0x10282350u;
constexpr uint32_t sqgxAlphaDstTest = 0x102823B0u;
constexpr uint32_t sqgxSetAlphaTest = 0x102823F0u;
constexpr uint32_t sqgxZMask = 0x10282470u;
constexpr uint32_t sqgxZMask_v1 = 0x10282470u;
constexpr uint32_t sqgxFBMask = 0x102824C0u;
constexpr uint32_t SqgxContext_ResetHandles = 0x10282590u;
constexpr uint32_t Unit102822A0_Nop = 0x10282630u;
constexpr uint32_t sqgxBlendAdd = 0x102826C0u;
constexpr uint32_t Unit102822A0_SetMultiplyAdd = 0x102827D0u;
constexpr uint32_t sqgxBlendSub = 0x10282910u;
constexpr uint32_t sqgxBlendLin = 0x10282A30u;
constexpr uint32_t sqgxBlendTrsp = 0x10282B90u;
constexpr uint32_t sqgxBlendTrsp_v1 = 0x10282B90u;
constexpr uint32_t sqgxBlendAddDestAlpha = 0x10282DC0u;
constexpr uint32_t sqgxBlendZeroSrcAlpha = 0x10282FF0u;
constexpr uint32_t sqgxBlendMul = 0x10283110u;
constexpr uint32_t sqgxBlendOff = 0x10283220u;
constexpr uint32_t sqgxBlendOff_v1 = 0x10283220u;
constexpr uint32_t SqgxContext_NoOpA = 0x10283240u;
constexpr uint32_t SqgxContext_NoOpA_v1 = 0x10283240u;
constexpr uint32_t ___setargv_9 = 0x10283250u;
constexpr uint32_t SqgxContext_SetCurrentIndex = 0x10283260u;
constexpr uint32_t SqgxContext_StoreEntryVec = 0x10283280u;
constexpr uint32_t FUN_10283300 = 0x10283300u;
constexpr uint32_t sqgxColor = 0x10283330u;
constexpr uint32_t sqgxColor_v1 = 0x10283330u;
constexpr uint32_t sqgxAmbient = 0x10283380u;
constexpr uint32_t sqgxAmbient_v1 = 0x10283380u;
constexpr uint32_t SqgxContext_SetFloat76C = 0x102833A0u;
constexpr uint32_t SqgxContext_SetDword774AndFactor = 0x102833C0u;
constexpr uint32_t sqgxColorUpdate = 0x10283420u;
constexpr uint32_t sqgxColorUpdate_v1 = 0x10283420u;
constexpr uint32_t ___setargv_16 = 0x10283870u;
constexpr uint32_t sqgxPixelSize = 0x102838A0u;
constexpr uint32_t SqgxContext_SetViewportVec4 = 0x10283970u;
constexpr uint32_t sqgxViewport = 0x10283A10u;
constexpr uint32_t sqgxPerspective = 0x10283C10u;
constexpr uint32_t sqgxMatrixCamera = 0x10283D60u;
constexpr uint32_t sqgxMatrixCamera_v1 = 0x10283D60u;
constexpr uint32_t SqgxContext_SetTransposedMatrix = 0x10283DA0u;
constexpr uint32_t SqgxContext_SetFloat72C = 0x10283DE0u;
constexpr uint32_t sqgxMatrixShape = 0x10283E00u;
constexpr uint32_t sqgxMatrixShape_v1 = 0x10283E00u;
constexpr uint32_t SqgxContext_SetInverseFactor = 0x10283F20u;
constexpr uint32_t sqgxPassAdd = 0x10283F70u;
constexpr uint32_t sqgxPassAdd_v1 = 0x10283F70u;
constexpr uint32_t sqgxPass = 0x10283FC0u;
constexpr uint32_t SqgxContext_TestClipBounds = 0x10284000u;
constexpr uint32_t sqgxMatrixUpdate = 0x10284410u;
constexpr uint32_t ___setargv_24 = 0x10284890u;
constexpr uint32_t sqgxVertexShaderCreate = 0x10284950u;
constexpr uint32_t sqgxShader = 0x10284B20u;
constexpr uint32_t sqgxShader_v1 = 0x10284B20u;
constexpr uint32_t Unit10284EB0_NoOp = 0x10284EB0u;
constexpr uint32_t j_sqgxVertexShaderSelect = 0x10284EC0u;
constexpr uint32_t Unit10284EB0_GetFloat20 = 0x10284F20u;
constexpr uint32_t SqmoChannel_Evaluate = 0x10284F50u;
constexpr uint32_t Unit10284F90_SetValueByType = 0x10284FD0u;
constexpr uint32_t FUN_10285000 = 0x10285000u;
constexpr uint32_t FUN_10285040 = 0x10285040u;
constexpr uint32_t SqObject_Release = 0x10285740u;
constexpr uint32_t Unit102857D0_GetFlagByte14 = 0x102857D0u;
constexpr uint32_t FUN_102857e0 = 0x102857E0u;
constexpr uint32_t Unit102857D0_InvokeCallback18 = 0x102857F0u;
constexpr uint32_t Unit102857D0_InvokeCallback18_v1 = 0x102857F0u;
constexpr uint32_t sqDrawableDraw = 0x10285850u;
constexpr uint32_t sqDrawableDraw_v1 = 0x10285850u;
constexpr uint32_t sqDebugPrintf = 0x10285890u;
constexpr uint32_t sqDebugPrintf_v1 = 0x10285890u;
constexpr uint32_t SqmdDMB_Resolve = 0x10287FB0u;
constexpr uint32_t SqmdDMB_TraceDelta = 0x10288660u;
constexpr uint32_t SqmdDMB_AlignMem = 0x10288680u;
constexpr uint32_t SqmdDMB_ResolveTextures = 0x102886D0u;
constexpr uint32_t SqmdDMB_ResolveShaders = 0x10288950u;
constexpr uint32_t SqmdDMB_TextureLink = 0x10288B70u;
constexpr uint32_t SqmdDMB_ResolveMatrices = 0x10288BB0u;
constexpr uint32_t SqmdDMB_ResolveStrings = 0x10288C20u;
constexpr uint32_t SqmdDMB_ClearTables = 0x10288DC0u;
constexpr uint32_t SqmdDMB_ResetCounts = 0x10288E00u;
constexpr uint32_t FUN_10288e60 = 0x10288E60u;
constexpr uint32_t SqmdDMB_BuildUpdaters = 0x10288FF0u;
constexpr uint32_t Unit10288FF0_ResolveSkelRefs = 0x102894E0u;
constexpr uint32_t SqopSystem_Init = 0x10289620u;
constexpr uint32_t FUN_10289670 = 0x10289670u;
constexpr uint32_t SqopSystem_ClearOpcodes = 0x102896A0u;
constexpr uint32_t SqopTrack_Reset = 0x10289B40u;
constexpr uint32_t SqopTrack_Free = 0x10289BA0u;
constexpr uint32_t sqopOpcodeGet = 0x10289EA0u;
constexpr uint32_t sqopTrackSetIndex = 0x10289EF0u;
constexpr uint32_t sqopTrackAdvance = 0x10289F10u;
constexpr uint32_t SqxfXformM_Init = 0x10289F50u;
constexpr uint32_t FUN_10289fb0 = 0x10289FB0u;
constexpr uint32_t FUN_1028a040 = 0x1028A040u;
constexpr uint32_t Unit1028A0A0_ReturnZero = 0x1028A0E0u;
constexpr uint32_t Unit1028A0A0_ReadBlock = 0x1028A0F0u;
constexpr uint32_t sqScript_TokenEquals = 0x1028A150u;
constexpr uint32_t sub_1028A730 = 0x1028A730u;
constexpr uint32_t sub_1028A770 = 0x1028A770u;
constexpr uint32_t sub_1028A790 = 0x1028A790u;
constexpr uint32_t sqgxShapeBegin = 0x1028AFE0u;
constexpr uint32_t sub_1028BB10 = 0x1028BB10u;
constexpr uint32_t sub_1028BB70 = 0x1028BB70u;
constexpr uint32_t sub_1028BBD0 = 0x1028BBD0u;
constexpr uint32_t SqcoConnector_Create = 0x1028D6D0u;
constexpr uint32_t SqcoConnector_Init = 0x1028D6F0u;
constexpr uint32_t FUN_1028d750 = 0x1028D750u;
constexpr uint32_t sqcoConnectorPrint = 0x1028D770u;
constexpr uint32_t Unit1028D970_SumAxis = 0x1028D990u;
constexpr uint32_t Unit1028D970_Reset = 0x1028D9A0u;
constexpr uint32_t Unit1028D970_Copy = 0x1028D9C0u;
constexpr uint32_t FUN_1028da20 = 0x1028DA20u;
constexpr uint32_t Unit1028D970_GetCenter = 0x1028DBB0u;
constexpr uint32_t SqmdSnap_Create = 0x1028DC00u;
constexpr uint32_t SqmdSnap_Init = 0x1028DC20u;
constexpr uint32_t FUN_1028dca0 = 0x1028DCA0u;
constexpr uint32_t FUN_1028e180 = 0x1028E180u;
constexpr uint32_t SqsnScene_Free = 0x1028E1B0u;
constexpr uint32_t SqsnScene_Init = 0x1028E350u;
constexpr uint32_t FUN_1028ec30 = 0x1028EC30u;
constexpr uint32_t SqsnScene_SetItemByte19 = 0x1028EC50u;
constexpr uint32_t SqsnScene_SetByteAt28 = 0x1028ECC0u;
constexpr uint32_t SqsnScene_SetItemByte18 = 0x1028ECE0u;
constexpr uint32_t SqsnScene_AddCamera = 0x1028ED00u;
constexpr uint32_t SqsnScene_AddLight = 0x1028ED50u;
constexpr uint32_t SqsnScene_LayerAddItem = 0x1028EDE0u;
constexpr uint32_t SqsnScene_LayerInit = 0x1028EFB0u;
constexpr uint32_t SqsnScene_AddShadow = 0x1028F130u;
constexpr uint32_t SqsnScene_SetItemDword28 = 0x1028F220u;
constexpr uint32_t SqsnScene_ItemAddLight = 0x1028F2E0u;
constexpr uint32_t SqsnScene_ItemAddShadowLight = 0x1028F350u;
constexpr uint32_t SqsnScene_ShadowAddMember = 0x1028F3C0u;
constexpr uint32_t SqsnScene_GetEntryAddr = 0x1028F430u;
constexpr uint32_t SqsnScene_SetCursorIfInRange = 0x1028F4A0u;
constexpr uint32_t SqsnScene_DrawMatrixShape = 0x1028F4C0u;
constexpr uint32_t sqsnScene_1028F5F0 = 0x1028F5F0u;
constexpr uint32_t sqsnScene_1028F5F0_v1 = 0x1028F5F0u;
constexpr uint32_t SqsnScene_SetupLayerLights = 0x1028F6E0u;
constexpr uint32_t sqsnSceneDraw = 0x1028F8C0u;
constexpr uint32_t sqsnSceneLayerDraw = 0x1028FDD0u;
constexpr uint32_t SqsnScene_DrawFadeSprite = 0x1028FF80u;
constexpr uint32_t sqsnSceneLayerDrawShadowed = 0x1028FFF0u;
constexpr uint32_t sqsnSceneLayerSnapShotComposite = 0x102902C0u;
constexpr uint32_t SqsnScene_NoOp = 0x10290430u;
constexpr uint32_t sqsnSceneLayerSort = 0x10290440u;
constexpr uint32_t SqsnScene_Preallocate = 0x10290610u;
constexpr uint32_t SqsnScene_LayerPreallocate = 0x102907C0u;
constexpr uint32_t SqsnScene_ShadowPreallocate = 0x10290840u;
constexpr uint32_t FUN_102908e0 = 0x102908E0u;
constexpr uint32_t SqsdGeneric_Init = 0x10290910u;
constexpr uint32_t sqsdGeneric_BakeOntoShape = 0x10290FC0u;
constexpr uint32_t FUN_102910e0 = 0x102910E0u;
constexpr uint32_t SqrdCamera_Init = 0x10291110u;
constexpr uint32_t FUN_10291170 = 0x10291170u;
constexpr uint32_t sqrdCamera_Apply = 0x102911E0u;
constexpr uint32_t sqrdCamera_Apply_v1 = 0x102911E0u;
constexpr uint32_t Unit10291200_CallHook2C = 0x10291290u;
constexpr uint32_t Unit10291200_CallHook3C = 0x102912D0u;
constexpr uint32_t Unit10291200_CallHook40 = 0x102912F0u;
constexpr uint32_t SqskJoint_Create = 0x10291360u;
constexpr uint32_t SqskJoint_Init = 0x10291390u;
constexpr uint32_t FUN_10291430 = 0x10291430u;
constexpr uint32_t SqskJoint_Nop = 0x10291460u;
constexpr uint32_t FUN_10291470 = 0x10291470u;
constexpr uint32_t Unit102915B0_CopyVec3At5C = 0x102915B0u;
constexpr uint32_t Unit102915B0_CopyQuatAt68 = 0x102915E0u;
constexpr uint32_t Unit102915B0_CopyVec3At78 = 0x102915F0u;
constexpr uint32_t Unit102915B0_SetTripleAt84 = 0x102916D0u;
constexpr uint32_t Unit102915B0_SetFloatAt94 = 0x10291780u;
constexpr uint32_t Unit102915B0_SetFloatAt98 = 0x10291790u;
constexpr uint32_t SqhiNode_Create = 0x10291B70u;
constexpr uint32_t SqhiNode_Init = 0x10291BA0u;
constexpr uint32_t FUN_10291c20 = 0x10291C20u;
constexpr uint32_t SqhiNode_Nop = 0x10291C50u;
constexpr uint32_t FUN_10291c60 = 0x10291C60u;
constexpr uint32_t SqhiNode_SetField64 = 0x10291F00u;
constexpr uint32_t SqhiNode_AppendChild = 0x10291F10u;
constexpr uint32_t FUN_10291fe0 = 0x10291FE0u;
constexpr uint32_t FUN_10292130 = 0x10292130u;
constexpr uint32_t Unit10292470_SwapNibblesXor = 0x10292490u;
constexpr uint32_t SqmoPlaybackChannel_Create = 0x102924F0u;
constexpr uint32_t SqmoPlaybackChannel_Init = 0x10292520u;
constexpr uint32_t FUN_102925a0 = 0x102925A0u;
constexpr uint32_t FUN_10292740 = 0x10292740u;
constexpr uint32_t SqmoPlaybackChannel_Update = 0x10293000u;
constexpr uint32_t FUN_102934c0 = 0x102934C0u;
constexpr uint32_t Unit10293EE0_SetHandlerByType = 0x10293F00u;
constexpr uint32_t SqmoFrameChannel_Create = 0x10293F80u;
constexpr uint32_t SqmoFrameChannel_Init = 0x10293FA0u;
constexpr uint32_t FUN_10294000 = 0x10294000u;
constexpr uint32_t sqmoFrameChannel_Dump = 0x102940A0u;
constexpr uint32_t SqmoFrameChannel_Apply = 0x102944E0u;
constexpr uint32_t SqmoKeyChannel_Create = 0x102957B0u;
constexpr uint32_t SqmoKeyChannel_Init = 0x102957D0u;
constexpr uint32_t FUN_10295830 = 0x10295830u;
constexpr uint32_t sqmoKeyChannel_Dump = 0x102958B0u;
constexpr uint32_t FUN_10295a20 = 0x10295A20u;
constexpr uint32_t SqmoChannelMotion_Create = 0x10295E70u;
constexpr uint32_t SqmoChannelMotion_Init = 0x10295E90u;
constexpr uint32_t FUN_10295ef0 = 0x10295EF0u;
constexpr uint32_t FUN_10295f30 = 0x10295F30u;
constexpr uint32_t Unit10296080_CreateWithOwner = 0x102960A0u;
constexpr uint32_t Unit102963D0_Set = 0x10296410u;
constexpr uint32_t SqMatrix3_Determinant = 0x102967E0u;
constexpr uint32_t SqgxContext_SetCotHalfAngle = 0x102972D0u;
constexpr uint32_t sub_10297320 = 0x10297320u;
constexpr uint32_t sqsnShadowVolumeExtrudeTriList = 0x102979E0u;
constexpr uint32_t sqsnShadowVolumeExtrudeTriStrip = 0x10297B10u;
constexpr uint32_t sqsnShadowVolumeSetPass = 0x10297CC0u;
constexpr uint32_t sqsnShadowComputeFacing = 0x10297F90u;
constexpr uint32_t sqsnShadowVolumeBeginVB = 0x102981E0u;
constexpr uint32_t sqgxShadowPlaneMatrix = 0x10298350u;
constexpr uint32_t SqimImage_Create = 0x102984B0u;
constexpr uint32_t FUN_10298540 = 0x10298540u;
constexpr uint32_t SqimImage_Init = 0x10298570u;
constexpr uint32_t SqrdTexture_Create = 0x10298B30u;
constexpr uint32_t FUN_10298b60 = 0x10298B60u;
constexpr uint32_t SqrdTexture_Init = 0x10298B90u;
constexpr uint32_t sqrdTexturePrint = 0x10298C10u;
constexpr uint32_t Texture_BindFromObject = 0x10298C50u;
constexpr uint32_t Texture_CreateEntry = 0x10298E10u;
constexpr uint32_t FUN_10298ee0 = 0x10298EE0u;
constexpr uint32_t sqsdSampling_BakeOntoShape = 0x10299250u;
constexpr uint32_t FUN_102992f0 = 0x102992F0u;
constexpr uint32_t sqsdAniso_BakeOntoShape = 0x10299610u;
constexpr uint32_t sqsdCartoon_BakeOntoShape_Stub = 0x10299840u;
constexpr uint32_t SqsdGlow_ApplyShader = 0x102999A0u;
constexpr uint32_t sqsdGlow_BakeOntoShape = 0x10299AB0u;
constexpr uint32_t SqshBuild_BuildSQO = 0x10299B30u;
constexpr uint32_t SqshBuild_BuildTriStrip = 0x10299C30u;
constexpr uint32_t SqshBuild_Read = 0x10299D60u;
constexpr uint32_t SqshBuild_BuildPoints = 0x10299E40u;
constexpr uint32_t SqshBuild_BuildCurves = 0x10299EE0u;
constexpr uint32_t SqinSkin_Create = 0x10299FE0u;
constexpr uint32_t SqinSkin_Init = 0x1029A000u;
constexpr uint32_t FUN_1029a060 = 0x1029A060u;
constexpr uint32_t FUN_1029a0e0 = 0x1029A0E0u;
constexpr uint32_t SqinSkin_InitShapeData = 0x1029A160u;
constexpr uint32_t SqinSkin_PropagateValue38 = 0x1029A230u;
constexpr uint32_t SqinSkin_ResizeItemArray = 0x1029A330u;
constexpr uint32_t Unit1029A380_Append = 0x1029A380u;
constexpr uint32_t sqskSkeleton_GetNodeByIndex = 0x1029A420u;
constexpr uint32_t SqinSkin_BuildPartShapes = 0x1029A450u;
constexpr uint32_t SqinSkin_ReleaseIfHasData = 0x1029A6F0u;
constexpr uint32_t SqinBuild_BuildSQO = 0x1029AA80u;
constexpr uint32_t FUN_1029abd0 = 0x1029ABD0u;
constexpr uint32_t SqskSkeleton_BuildSQO = 0x1029ADF0u;
constexpr uint32_t SqskSkeleton_BuildBinary = 0x1029AF20u;
constexpr uint32_t SqmdSort_Create = 0x1029B100u;
constexpr uint32_t SqmdSort_Init = 0x1029B120u;
constexpr uint32_t FUN_1029b1a0 = 0x1029B1A0u;
constexpr uint32_t SqmdSort_AllocLists = 0x1029B310u;
constexpr uint32_t SqmdSort_CreateForStrips = 0x1029B3A0u;
constexpr uint32_t SqgxDlist_CreateContextList = 0x1029B5C0u;
constexpr uint32_t FUN_1029b620 = 0x1029B620u;
constexpr uint32_t SqgxDlist_Nop = 0x1029BB50u;
constexpr uint32_t Sqgx_CopyBlockToContext = 0x1029BCD0u;
constexpr uint32_t Sqgx_SetFloat18 = 0x1029BD40u;
constexpr uint32_t sub_1029BD50 = 0x1029BD50u;
constexpr uint32_t sub_1029BF50 = 0x1029BF50u;
constexpr uint32_t FUN_1029c260 = 0x1029C260u;
constexpr uint32_t Sqgx_SetContextFieldE24 = 0x1029C460u;
constexpr uint32_t sub_1029C480 = 0x1029C480u;
constexpr uint32_t sub_1029C610 = 0x1029C610u;
constexpr uint32_t sub_1029C950 = 0x1029C950u;
constexpr uint32_t FUN_102a1f20 = 0x102A1F20u;
constexpr uint32_t Unit102A1F20_TestSeparationHalf = 0x102A1FA0u;
constexpr uint32_t SqshDispList_Create = 0x102A2000u;
constexpr uint32_t SqshDispList_Init = 0x102A2020u;
constexpr uint32_t FUN_102a2080 = 0x102A2080u;
constexpr uint32_t sqshDispListPrint = 0x102A20D0u;
constexpr uint32_t Unit102A2130_CopyBounds = 0x102A2130u;
constexpr uint32_t Unit102A2130_ContainsInt = 0x102A2160u;
constexpr uint32_t Unit102A2130_ContainsInAnyEntry = 0x102A2190u;
constexpr uint32_t sqsnPlanarShadowDraw = 0x102A2530u;
constexpr uint32_t Unit102A2530_RenderShadowTexture = 0x102A2720u;
constexpr uint32_t Sqgx_SetTextureAt = 0x102A2A30u;
constexpr uint32_t Sqgx_CheckShadowGroupMember = 0x102A2A50u;
constexpr uint32_t Unit102A2530_DrawShadowedShape = 0x102A2C00u;
constexpr uint32_t FUN_102a3360 = 0x102A3360u;
constexpr uint32_t SqrdMaterial_Init = 0x102A3390u;
constexpr uint32_t FUN_102a33f0 = 0x102A33F0u;
constexpr uint32_t Unit102A34D0_ApplyLighting = 0x102A34D0u;
constexpr uint32_t Sqgx_ResetCounter = 0x102A3760u;
constexpr uint32_t Sqgx_IncCounter = 0x102A3770u;
constexpr uint32_t SqVtx_Create = 0x102A9520u;
constexpr uint32_t SqVtx_Init = 0x102A9550u;
constexpr uint32_t FUN_102a95a0 = 0x102A95A0u;
constexpr uint32_t sqVtxPrint = 0x102A96C0u;
constexpr uint32_t SqVtx_AllocateField = 0x102A9980u;
constexpr uint32_t SqVtx_ClearEntries = 0x102A9EF0u;
constexpr uint32_t SqVtx_ClearEntriesAlt = 0x102A9F30u;
constexpr uint32_t SqVtx_PackFloatsToBytes = 0x102A9FB0u;
constexpr uint32_t SqshTriStrip_Create = 0x102AA2D0u;
constexpr uint32_t SqshTriStrip_Init = 0x102AA2F0u;
constexpr uint32_t FUN_102aa370 = 0x102AA370u;
constexpr uint32_t FUN_102aa420 = 0x102AA420u;
constexpr uint32_t FUN_102aa580 = 0x102AA580u;
constexpr uint32_t SqshTriStrip_GetGlobal = 0x102ABEB0u;
constexpr uint32_t FUN_102abec0 = 0x102ABEC0u;
constexpr uint32_t SqshPoints_Create = 0x102ACB60u;
constexpr uint32_t SqshPoints_Init = 0x102ACB80u;
constexpr uint32_t FUN_102acbe0 = 0x102ACBE0u;
constexpr uint32_t sqshPointsPrint = 0x102ACC30u;
constexpr uint32_t SqshCurves_Create = 0x102ACF20u;
constexpr uint32_t SqshCurves_Init = 0x102ACF40u;
constexpr uint32_t FUN_102acfc0 = 0x102ACFC0u;
constexpr uint32_t sqshCurvesPrint = 0x102AD090u;
constexpr uint32_t SqshCurves_ComputeNormals = 0x102AD1D0u;
constexpr uint32_t FUN_102ad770 = 0x102AD770u;
constexpr uint32_t FUN_102ad770_v1 = 0x102AD770u;
constexpr uint32_t SqshCurves_RebuildIndex = 0x102AD990u;
constexpr uint32_t SqinShape_Reset = 0x102AEC50u;
constexpr uint32_t SqdfShape_Create = 0x102AEE90u;
constexpr uint32_t SqdfShape_Init = 0x102AEEB0u;
constexpr uint32_t FUN_102aefe0 = 0x102AEFE0u;
constexpr uint32_t FUN_102af020 = 0x102AF020u;
constexpr uint32_t SqdfShape_InitCallbacks = 0x102AF170u;
constexpr uint32_t FUN_102af2a0 = 0x102AF2A0u;
constexpr uint32_t sqinCluster_Create = 0x102AF540u;
constexpr uint32_t sqinCluster_Init = 0x102AF570u;
constexpr uint32_t FUN_102af5e0 = 0x102AF5E0u;
constexpr uint32_t sqinCluster_Dump = 0x102AF630u;
constexpr uint32_t ImmNotifyIME = 0x102BC838u;
constexpr uint32_t ImmGetOpenStatus = 0x102BC85Cu;
constexpr uint32_t ImmGetConversionStatus = 0x102BC862u;
constexpr uint32_t ImmGetCompositionStringA = 0x102BC868u;
constexpr uint32_t ImmSetCompositionStringA = 0x102BC86Eu;
constexpr uint32_t Direct3DCreate8 = 0x102D2600u;
constexpr uint32_t fn_102D2868 = 0x102D2868u;
constexpr uint32_t fn_102D2FB9 = 0x102D2FB9u;
constexpr uint32_t fn_102D4515 = 0x102D4515u;
constexpr uint32_t FUN_102d7ff9 = 0x102D7FF9u;
constexpr uint32_t fn_102D7FF9 = 0x102D7FF9u;
constexpr uint32_t D3DXMatrixPerspectiveFovLH_static = 0x102D83C6u;
constexpr uint32_t D3DXMatrixPerspectiveFovLH_static_v1 = 0x102D83C6u;
constexpr uint32_t fn_102DA79B = 0x102DA79Bu;
constexpr uint32_t fn_102DAA0D = 0x102DAA0Du;
constexpr uint32_t fn_102DAA71 = 0x102DAA71u;
constexpr uint32_t FUN_1030fed0 = 0x1030FED0u;
constexpr uint32_t FUN_1030ff33 = 0x1030FF33u;
constexpr uint32_t FUN_1030ff49 = 0x1030FF49u;
constexpr uint32_t FUN_1030ff75 = 0x1030FF75u;
constexpr uint32_t FUN_1030ffa1 = 0x1030FFA1u;
constexpr uint32_t FUN_1030ffb7 = 0x1030FFB7u;
constexpr uint32_t FUN_1030ffc2 = 0x1030FFC2u;
constexpr uint32_t FUN_1030ffcd = 0x1030FFCDu;
constexpr uint32_t FUN_1030ffd8 = 0x1030FFD8u;
constexpr uint32_t FUN_10310030 = 0x10310030u;
constexpr uint32_t FUN_10310067 = 0x10310067u;
constexpr uint32_t FUN_10310072 = 0x10310072u;
constexpr uint32_t FUN_103100eb = 0x103100EBu;
constexpr uint32_t FUN_10310353 = 0x10310353u;
constexpr uint32_t FUN_1031040e = 0x1031040Eu;
constexpr uint32_t FUN_10310b30 = 0x10310B30u;
constexpr uint32_t FUN_10310b3b = 0x10310B3Bu;
constexpr uint32_t FUN_10310cd2 = 0x10310CD2u;
constexpr uint32_t FUN_10310d82 = 0x10310D82u;
constexpr uint32_t FUN_10310df0 = 0x10310DF0u;
constexpr uint32_t FUN_10310dfb = 0x10310DFBu;
constexpr uint32_t FUN_10310e06 = 0x10310E06u;
constexpr uint32_t FUN_10310e11 = 0x10310E11u;
constexpr uint32_t FUN_10310e1c = 0x10310E1Cu;
constexpr uint32_t FUN_10310e27 = 0x10310E27u;
constexpr uint32_t FUN_10310e32 = 0x10310E32u;
constexpr uint32_t FUN_1031111e = 0x1031111Eu;
constexpr uint32_t FUN_10311344 = 0x10311344u;
constexpr uint32_t FUN_1031135a = 0x1031135Au;
constexpr uint32_t FUN_10311365 = 0x10311365u;
constexpr uint32_t FUN_10311370 = 0x10311370u;
constexpr uint32_t FUN_10311386 = 0x10311386u;
constexpr uint32_t FUN_103113c8 = 0x103113C8u;
constexpr uint32_t FUN_103113d3 = 0x103113D3u;
constexpr uint32_t operator_delete = 0x10311BB0u;
constexpr uint32_t operator_delete_v1 = 0x10311BB0u;
constexpr uint32_t operator_delete_v2 = 0x10311BB0u;
constexpr uint32_t Allocator_Generic = 0x10311BBBu;
constexpr uint32_t Allocator_Generic_v1 = 0x10311BBBu;
constexpr uint32_t Allocator_Generic_v2 = 0x10311BBBu;
constexpr uint32_t Allocator_Generic_v3 = 0x10311BBBu;
constexpr uint32_t FUN_10311c7a = 0x10311C7Au;
constexpr uint32_t FUN_10311eb0 = 0x10311EB0u;
constexpr uint32_t FUN_10311f30 = 0x10311F30u;
constexpr uint32_t FUN_10311fde = 0x10311FDEu;
constexpr uint32_t _sprintf = 0x10311FF1u;
constexpr uint32_t _sprintf_v1 = 0x10311FF1u;
constexpr uint32_t _strncpy = 0x10312050u;
constexpr uint32_t _strncpy_v1 = 0x10312050u;
constexpr uint32_t _strncpy_v2 = 0x10312050u;
constexpr uint32_t FUN_1031217f = 0x1031217Fu;
constexpr uint32_t FUN_10312296 = 0x10312296u;
constexpr uint32_t FUN_10312487 = 0x10312487u;
constexpr uint32_t _strrchr = 0x103127C0u;
constexpr uint32_t FUN_10312872 = 0x10312872u;
constexpr uint32_t _strncmp = 0x10312940u;
constexpr uint32_t FUN_10312ad3 = 0x10312AD3u;
constexpr uint32_t FUN_10312bdb = 0x10312BDBu;
constexpr uint32_t FUN_10312ead = 0x10312EADu;
constexpr uint32_t FUN_10312ead_v1 = 0x10312EADu;
constexpr uint32_t _malloc_0 = 0x10312F96u;
constexpr uint32_t _malloc_0_v1 = 0x10312F96u;
constexpr uint32_t _malloc_0_v2 = 0x10312F96u;
constexpr uint32_t _atexit = 0x10313486u;
constexpr uint32_t _atexit_v1 = 0x10313486u;
constexpr uint32_t __snprintf = 0x103134C7u;
constexpr uint32_t _strncat = 0x10313520u;
constexpr uint32_t FUN_10313643 = 0x10313643u;
constexpr uint32_t FUN_10313b74 = 0x10313B74u;
constexpr uint32_t FUN_10313cf0 = 0x10313CF0u;
constexpr uint32_t FUN_1031450a = 0x1031450Au;
constexpr uint32_t FUN_103145a4 = 0x103145A4u;
constexpr uint32_t FUN_103146de = 0x103146DEu;
constexpr uint32_t FUN_1031475c = 0x1031475Cu;
constexpr uint32_t FUN_10314801 = 0x10314801u;
constexpr uint32_t FUN_1031482f = 0x1031482Fu;
constexpr uint32_t FUN_10314942 = 0x10314942u;
constexpr uint32_t _rand = 0x103149A0u;
constexpr uint32_t FUN_103149d0 = 0x103149D0u;
constexpr uint32_t FUN_10314d55 = 0x10314D55u;
constexpr uint32_t FUN_10314d91 = 0x10314D91u;
constexpr uint32_t FUN_10314ef1 = 0x10314EF1u;
constexpr uint32_t FUN_10315140 = 0x10315140u;
constexpr uint32_t FUN_103151d0 = 0x103151D0u;
constexpr uint32_t FUN_103156b3 = 0x103156B3u;
constexpr uint32_t __strlwr = 0x10315AA8u;
constexpr uint32_t __strlwr_v1 = 0x10315AA8u;
constexpr uint32_t FUN_103169a0 = 0x103169A0u;
constexpr uint32_t FUN_10316d6b = 0x10316D6Bu;
constexpr uint32_t _memset = 0x10316D90u;
constexpr uint32_t FUN_1031a00d = 0x1031A00Du;
constexpr uint32_t FUN_10325d00 = 0x10325D00u;
constexpr uint32_t FUN_10325d00_v1 = 0x10325D00u;
}  // namespace orig_addr
#define XI_CODE(name) reinterpret_cast<void *>(XI_GAME(orig_addr::name))
