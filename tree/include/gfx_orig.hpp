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
XI_ORIG char App_CreateInputSystem(HINSTANCE a0) { return reinterpret_cast<char (__stdcall *)(HINSTANCE)>(XI_GAME(0x10001BB0u))(a0); }
XI_ORIG BOOL CApp_CopyTextToClipboard(void * a0, void * a1) { return reinterpret_cast<BOOL (__thiscall *)(void *, void *)>(XI_GAME(0x10001CC0u))(a0, a1); }
XI_ORIG char sub_10001E70(void * a0) { return reinterpret_cast<char (__cdecl *)(void *)>(XI_GAME(0x10001E70u))(a0); }
XI_ORIG char * FUN_10001f40(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<char * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10001F40u))(a0, a1, a2); }
XI_ORIG unsigned long long Gfx_CopySurfaceRectToTexture(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) {
    uint32_t xs[4] = {a0.bits, a1.bits, a2.bits, a3.bits}; uint32_t ra = a4.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 12(%%esi)\n\t""pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $16, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10002040u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG char Gfx_ProbeDeviceCaps(int a0, void * a1, void * a2) { return reinterpret_cast<char (__cdecl *)(int, void *, void *)>(XI_GAME(0x10002130u))(a0, a1, a2); }
XI_ORIG unsigned int sub_10002440(int a0) { return reinterpret_cast<unsigned int (__cdecl *)(int)>(XI_GAME(0x10002440u))(a0); }
XI_ORIG CDx * Gfx_SetGammaRGB(int a0, int a1, int a2) { return reinterpret_cast<CDx * (__cdecl *)(int, int, int)>(XI_GAME(0x10002C00u))(a0, a1, a2); }
XI_ORIG long double FUN_10002d80(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0; long double st0;
    __asm__ volatile("call *%%edi\n\t"
        : "=t"(st0), "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10002D80u) : "memory", "cc", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return st0;
}
XI_ORIG double sub_10002D80() { return reinterpret_cast<double (__cdecl *)()>(XI_GAME(0x10002D80u))(); }
XI_ORIG std::codecvt_base * sub_10002E60(int a0) { return reinterpret_cast<std::codecvt_base * (__cdecl *)(int)>(XI_GAME(0x10002E60u))(a0); }
XI_ORIG void FUN_10002e80(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10002E80u))(a0); }
XI_ORIG long double FUN_10002ec0(XI_ANY a0) { return reinterpret_cast<long double (__cdecl *)(XI_ANY)>(XI_GAME(0x10002EC0u))(a0); }
XI_ORIG long double FUN_10002ee0(XI_ANY a0) { return reinterpret_cast<long double (__cdecl *)(XI_ANY)>(XI_GAME(0x10002EE0u))(a0); }
XI_ORIG _DWORD * D3DHolder_Constructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10003070u))(a0); }
XI_ORIG char D3DHolder_Initialize(void * a0, int a1, int a2, int a3, char a4, char a5) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int, char, char)>(XI_GAME(0x10003110u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG char Gfx_TryCreateDeviceForMode(void * a0, int a1, int a2, int a3, char a4, char a5, int a6, int a7) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int, char, char, int, int)>(XI_GAME(0x100033C0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG uint8_t D3DHolder_CheckDepthFormat(void * a0, int a1, int a2, int a3) { return reinterpret_cast<uint8_t (__thiscall *)(void *, int, int, int)>(XI_GAME(0x10003460u))(a0, a1, a2, a3); }
XI_ORIG char Gfx_CreateDevice(void * a0, int a1, int a2, int a3, int a4, int a5, char a6, char a7, int a8) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int, int, int, char, char, int)>(XI_GAME(0x100034B0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG int nullsub_71() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100035A0u))(); }
XI_ORIG int sub_100035F0(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x100035F0u))(a0); }
XI_ORIG void FUN_10003640(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10003640u))(a0); }
XI_ORIG char sub_10003650(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10003650u))(a0); }
XI_ORIG void FUN_10003660(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10003660u))(a0); }
XI_ORIG void FUN_10003700() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x10003700u))(); }
XI_ORIG void FUN_10003710(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10003710u))(a0); }
XI_ORIG unsigned long long FUN_100037d0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) {
    uint32_t xs[7] = {a1.bits, a2.bits, a3.bits, a4.bits, a5.bits, a6.bits, a7.bits}; uint32_t ra = a8.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 24(%%esi)\n\t""pushl 20(%%esi)\n\t""pushl 16(%%esi)\n\t""pushl 12(%%esi)\n\t""pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100037D0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_10003890(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10003890u))(a0); }
XI_ORIG void Debug_DrawTextTable(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10003B60u))(a0); }
XI_ORIG void Debug_DrawWorldText(void * a0, void * a1, int a2) { reinterpret_cast<void (__stdcall *)(void *, void *, int)>(XI_GAME(0x10003D70u))(a0, a1, a2); }
XI_ORIG void FUN_10005600(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10005600u))(a0, a1); }
XI_ORIG void * FUN_10005660(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10005660u))(a0); }
XI_ORIG undefined FUN_100057d0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x100057D0u))(a0, a1); }
XI_ORIG undefined4 FUN_100058f0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100058F0u))(a0, a1); }
XI_ORIG int sub_10005A40(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10005A40u))(a0); }
XI_ORIG undefined4 DirectInput_InstallMouse(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10007EF0u))(a0); }
XI_ORIG undefined4 FUN_10008770(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10008770u))(a0, a1); }
XI_ORIG char sub_10008830(void * a0, unsigned __int8 a1) { return reinterpret_cast<char (__thiscall *)(void *, unsigned __int8)>(XI_GAME(0x10008830u))(a0, a1); }
XI_ORIG unsigned int sub_100088F0(void * a0) { return reinterpret_cast<unsigned int (__thiscall *)(void *)>(XI_GAME(0x100088F0u))(a0); }
XI_ORIG undefined4 FUN_10008d90(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10008D90u))(a0); }
XI_ORIG undefined4 FUN_10008e10(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10008E10u))(a0); }
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
XI_ORIG BOOL SetCursorPos(int a0, int a1) { return reinterpret_cast<BOOL (__stdcall *)(int, int)>(XI_GAME(0x10009770u))(a0, a1); }
XI_ORIG unsigned __int8 CDx_ShowCursor(void * a0, unsigned __int8 a1) { return reinterpret_cast<unsigned __int8 (__thiscall *)(void *, unsigned __int8)>(XI_GAME(0x10009780u))(a0, a1); }
XI_ORIG HCURSOR CDx_SetCursorShape(void * a0, int a1, int a2) { return reinterpret_cast<HCURSOR (__thiscall *)(void *, int, int)>(XI_GAME(0x100097F0u))(a0, a1, a2); }
XI_ORIG int CDx_SetDefaultRenderState(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10009920u))(a0); }
XI_ORIG int sub_10009D20(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10009D20u))(a0); }
XI_ORIG int CDx_SetViewport(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x10009DC0u))(a0, a1); }
XI_ORIG unsigned __int8 * CDx_GetRenderTargetTop(void * a0) { return reinterpret_cast<unsigned __int8 * (__thiscall *)(void *)>(XI_GAME(0x10009E00u))(a0); }
XI_ORIG int CDx_GetRenderTargetTop_v1(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10009E00u))(a0); }
XI_ORIG int CDx_PushRenderTarget(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(XI_GAME(0x10009E20u))(a0, a1, a2, a3); }
XI_ORIG undefined4 CDx_PushRenderTarget_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10009E20u))(a0, a1, a2, a3); }
XI_ORIG int sub_10009EA0(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(XI_GAME(0x10009EA0u))(a0, a1, a2, a3); }
XI_ORIG int sub_10009EE0(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(XI_GAME(0x10009EE0u))(a0, a1, a2, a3); }
XI_ORIG int CDx_PushRenderTargetTexture(void * a0, int a1, int a2, int a3, void * a4, void * a5, int a6, int a7) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, void *, void *, int, int)>(XI_GAME(0x10009F20u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG undefined4 CDx_PushRenderTargetTexture_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10009F20u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG void FUN_1000a0a0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1000A0A0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int CDx_SetTexture(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1000A1D0u))(a0, a1, a2); }
XI_ORIG undefined4 CDx_SetTexture_v1(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1000A1D0u))(a0, a1, a2); }
XI_ORIG int sub_1000A200(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1000A200u))(a0, a1, a2); }
XI_ORIG int sub_1000A230(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1000A230u))(a0, a1, a2); }
XI_ORIG int CDx_PopRenderTargetDefault(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1000A290u))(a0, a1, a2); }
XI_ORIG int CDx_PopRenderTarget(int a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<int (__thiscall *)(int, int, int, int, int)>(XI_GAME(0x1000A2B0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 CDx_PopRenderTarget_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1000A2B0u))(a0, a1, a2, a3, a4); }
XI_ORIG int sub_1000A390(void * a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int)>(XI_GAME(0x1000A390u))(a0, a1, a2, a3, a4); }
XI_ORIG int CDx_DrawScreenQuadAtDepth(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1000A3B0u))(a0, a1, a2); }
XI_ORIG int CDx_DrawTextureToViewport(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(XI_GAME(0x1000A4F0u))(a0, a1, a2, a3); }
XI_ORIG void FUN_1000a550(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1000A550u))(a0, a1, a2, a3, a4); }
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
XI_ORIG void nullsub_36() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1000BB10u))(); }
XI_ORIG void nullsub_37() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1000BB20u))(); }
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
XI_ORIG undefined8 FUN_1000d150(XI_ANY a0) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY)>(XI_GAME(0x1000D150u))(a0); }
XI_ORIG char j_CEnv_UpdateDualTargetMode(int a0, int a1) { return reinterpret_cast<char (__thiscall *)(int, int)>(XI_GAME(0x1000D3F0u))(a0, a1); }
XI_ORIG void nullsub_38() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1000D620u))(); }
XI_ORIG void FUN_1000d630(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1000D630u))(a0, a1); }
XI_ORIG char CEnv_IsStereoMode(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1000D660u))(a0); }
XI_ORIG undefined4 CEnv_IsStereoMode_v1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1000D660u))(a0); }
XI_ORIG undefined4 FUN_1000d680() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1000D680u))(); }
XI_ORIG undefined4 FUN_1000d6b0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1000D6B0u))(); }
XI_ORIG int App_RegisterSystemDats() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10010370u))(); }
XI_ORIG void FUN_10010520() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10010520u))(); }
XI_ORIG int sub_10010520() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10010520u))(); }
XI_ORIG char * App_Constructor(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10010700u))(a0); }
XI_ORIG __int16 App_SetSeqStep(void * a0, __int16 a1) { return reinterpret_cast<__int16 (__thiscall *)(void *, __int16)>(XI_GAME(0x10010CE0u))(a0, a1); }
XI_ORIG void App_AdvanceSeqStep(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10010D00u))(a0); }
XI_ORIG void App_AdvanceSeqStep_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10010D00u))(a0); }
XI_ORIG char App_InitScene(int a0, int a1) { return reinterpret_cast<char (__thiscall *)(int, int)>(XI_GAME(0x10010D20u))(a0, a1); }
XI_ORIG undefined4 Frame_Render(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10011A00u))(a0); }
XI_ORIG int Post_Afterimage() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10012F40u))(); }
XI_ORIG int Post_FlashAndZoomFeedback(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100132B0u))(a0); }
XI_ORIG char sub_100139E0(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100139E0u))(a0); }
XI_ORIG uint8_t fn_10013AC0() { return reinterpret_cast<uint8_t (__cdecl *)()>(XI_GAME(0x10013AC0u))(); }
XI_ORIG char App_StepGameState5Connect(int a0, int a1) { return reinterpret_cast<char (__thiscall *)(int, int)>(XI_GAME(0x10013AD0u))(a0, a1); }
XI_ORIG char App_StepGameState6(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10013DF0u))(a0); }
XI_ORIG char App_StepGameState7(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10013F10u))(a0); }
XI_ORIG char App_StepGameState8(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10014030u))(a0); }
XI_ORIG uint8_t App_StepGameState9() { return reinterpret_cast<uint8_t (__cdecl *)()>(XI_GAME(0x10014120u))(); }
XI_ORIG char sub_10014170(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10014170u))(a0); }
XI_ORIG undefined4 Game_InGameSubState0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100141C0u))(a0); }
XI_ORIG unsigned long long FUN_10014200(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10014200u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_10014220(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10014220u))(a0, a1); }
XI_ORIG undefined4 FUN_100142a0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100142A0u))(a0); }
XI_ORIG undefined4 FUN_100142b0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100142B0u))(); }
XI_ORIG undefined4 FUN_10014350(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10014350u))(a0); }
XI_ORIG char sub_100149B0(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100149B0u))(a0); }
XI_ORIG uint8_t sub_10014A20(void * a0) { return reinterpret_cast<uint8_t (__thiscall *)(void *)>(XI_GAME(0x10014A20u))(a0); }
XI_ORIG char sub_10014A30(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x10014A30u))(a0); }
XI_ORIG char sub_10014AD0(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10014AD0u))(a0); }
XI_ORIG uint8_t sub_10014C10(void * a0) { return reinterpret_cast<uint8_t (__thiscall *)(void *)>(XI_GAME(0x10014C10u))(a0); }
XI_ORIG void FUN_10014c60() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10014C60u))(); }
XI_ORIG undefined4 FUN_10014c90(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10014C90u))(a0); }
XI_ORIG undefined4 FUN_10014cb0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10014CB0u))(); }
XI_ORIG undefined4 FUN_10014ce0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10014CE0u))(); }
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
XI_ORIG undefined4 FUN_10014d60() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10014D60u))(); }
XI_ORIG double Camera_GetFloat2DC() { return reinterpret_cast<double (__cdecl *)()>(XI_GAME(0x10014D70u))(); }
XI_ORIG double Camera_GetFloat2E0() { return reinterpret_cast<double (__cdecl *)()>(XI_GAME(0x10014D80u))(); }
XI_ORIG unsigned int Camera_SetClearColorRGB(unsigned __int8 a0, unsigned __int8 a1, unsigned __int8 a2) { return reinterpret_cast<unsigned int (__cdecl *)(unsigned __int8, unsigned __int8, unsigned __int8)>(XI_GAME(0x10014D90u))(a0, a1, a2); }
XI_ORIG void FUN_10014db0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10014DB0u))(a0, a1, a2, a3); }
XI_ORIG void FUN_10014e00(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10014E00u))(a0, a1, a2); }
XI_ORIG void FUN_10014e20(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10014E20u))(a0, a1, a2, a3); }
XI_ORIG void FUN_10014e60(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10014E60u))(a0); }
XI_ORIG void FUN_10014e80(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10014E80u))(a0); }
XI_ORIG void FUN_10014eb0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10014EB0u))(a0); }
XI_ORIG void FUN_10014ed0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10014ED0u))(a0); }
XI_ORIG int sub_10014EF0(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10014EF0u))(a0); }
XI_ORIG int sub_10014F20(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10014F20u))(a0); }
XI_ORIG int sub_10015050(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10015050u))(a0); }
XI_ORIG void Camera_SetViewMatrix(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15) { reinterpret_cast<void (__cdecl *)(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int)>(XI_GAME(0x100150D0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15); }
XI_ORIG undefined4 FUN_10015100(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10015100u))(a0); }
XI_ORIG undefined4 FUN_100151a0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100151A0u))(a0, a1); }
XI_ORIG void Camera_SetMatrix154(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15) { reinterpret_cast<void (__cdecl *)(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int)>(XI_GAME(0x100151F0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15); }
XI_ORIG char * Camera_GetViewMatrixPtr() { return reinterpret_cast<char * (__cdecl *)()>(XI_GAME(0x10015220u))(); }
XI_ORIG int Camera_GetViewMatrixPtr_v1() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10015220u))(); }
XI_ORIG char * sub_10015240() { return reinterpret_cast<char * (__cdecl *)()>(XI_GAME(0x10015240u))(); }
XI_ORIG int Camera_GetField50() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10015250u))(); }
XI_ORIG undefined4 Camera_GetField50_v1() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10015250u))(); }
XI_ORIG char * Camera_GetMatrix154Ptr() { return reinterpret_cast<char * (__cdecl *)()>(XI_GAME(0x10015270u))(); }
XI_ORIG int Camera_GetMatrix154Ptr_v1() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10015270u))(); }
XI_ORIG void FUN_10015290(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10015290u))(a0); }
XI_ORIG void FUN_100152b0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100152B0u))(a0, a1); }
XI_ORIG long double FUN_100152c0() { return reinterpret_cast<long double (__cdecl *)()>(XI_GAME(0x100152C0u))(); }
XI_ORIG std::codecvt_base * sub_10015360(int a0) { return reinterpret_cast<std::codecvt_base * (__cdecl *)(int)>(XI_GAME(0x10015360u))(a0); }
XI_ORIG void FUN_100153a0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100153A0u))(a0, a1); }
XI_ORIG std::codecvt_base * sub_100153A0(float a0, float a1) { return reinterpret_cast<std::codecvt_base * (__cdecl *)(float, float)>(XI_GAME(0x100153A0u))(a0, a1); }
XI_ORIG void FUN_100153d0() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x100153D0u))(); }
XI_ORIG void FUN_10015490(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10015490u))(a0); }
XI_ORIG void Camera_ApplyViewProjection() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10015510u))(); }
XI_ORIG undefined4 FUN_100155d0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100155D0u))(); }
XI_ORIG undefined4 FUN_10015600() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10015600u))(); }
XI_ORIG undefined4 FUN_10015610() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10015610u))(); }
XI_ORIG __int16 Camera_GetViewportWidth() { return reinterpret_cast<__int16 (__cdecl *)()>(XI_GAME(0x10015680u))(); }
XI_ORIG __int16 Camera_GetViewportHeight() { return reinterpret_cast<__int16 (__cdecl *)()>(XI_GAME(0x10015690u))(); }
XI_ORIG int FUN_100156a0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100156A0u))(a0, a1, a2, a3); }
XI_ORIG int sub_100156A0(int a0, int a1, void * a2, void * a3) { return reinterpret_cast<int (__cdecl *)(int, int, void *, void *)>(XI_GAME(0x100156A0u))(a0, a1, a2, a3); }
XI_ORIG uint8_t fn_10015740() { return reinterpret_cast<uint8_t (__cdecl *)()>(XI_GAME(0x10015740u))(); }
XI_ORIG undefined4 App_WinMain() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100157E0u))(); }
XI_ORIG char sub_100163C0(char a0) { return reinterpret_cast<char (__cdecl *)(char)>(XI_GAME(0x100163C0u))(a0); }
XI_ORIG char sub_100163E0(char a0) { return reinterpret_cast<char (__cdecl *)(char)>(XI_GAME(0x100163E0u))(a0); }
XI_ORIG int App_SetGlobal_104569C8(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10016400u))(a0); }
XI_ORIG undefined4 App_SetGlobal_104569C8_v1(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10016400u))(a0); }
XI_ORIG undefined4 FUN_10016410() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10016410u))(); }
XI_ORIG int sub_10016410() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10016410u))(); }
XI_ORIG unsigned long long FUN_100164b0(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100164B0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG int App_SetErrorCode(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x100164C0u))(a0); }
XI_ORIG int sub_10016560(void * a0, void * a1) { return reinterpret_cast<int (__cdecl *)(void *, void *)>(XI_GAME(0x10016560u))(a0, a1); }
XI_ORIG int GammaRamp_SetRGB(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(XI_GAME(0x100170A0u))(a0, a1, a2, a3); }
XI_ORIG int GammaRamp_GetRGB(void * a0, void * a1, void * a2, void * a3) { return reinterpret_cast<int (__thiscall *)(void *, void *, void *, void *)>(XI_GAME(0x100170D0u))(a0, a1, a2, a3); }
XI_ORIG int GammaRamp_GetChannelSamples(void * a0, void * a1, void * a2, void * a3) { return reinterpret_cast<int (__thiscall *)(void *, void *, void *, void *)>(XI_GAME(0x10017100u))(a0, a1, a2, a3); }
XI_ORIG undefined4 GammaRamp_GetChannelSamples_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10017100u))(a0, a1, a2, a3); }
XI_ORIG __int64 GammaRamp_BuildRamps(void * a0) { return reinterpret_cast<__int64 (__thiscall *)(void *)>(XI_GAME(0x10017160u))(a0); }
XI_ORIG void FUN_100171b0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x100171B0u))(a0, a1); }
XI_ORIG void GammaRamp_ApplyIfChanged(int a0) { reinterpret_cast<void (__thiscall *)(int)>(XI_GAME(0x10017230u))(a0); }
XI_ORIG int CShader_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100177A0u))(a0); }
XI_ORIG exception * fn_10017BB0(void * a0) { return reinterpret_cast<exception * (__thiscall *)(void *)>(XI_GAME(0x10017BB0u))(a0); }
XI_ORIG int sub_10017C20(int a0, void * a1, void * a2, int a3) { return reinterpret_cast<int (__thiscall *)(int, void *, void *, int)>(XI_GAME(0x10017C20u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_10017c60(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10017C60u))(a0, a1, a2, a3); }
template <class... R> XI_ORIG int nullsub_13(void * a0, R... rest) { return reinterpret_cast<int (__cdecl *)(void *, ...)>(XI_GAME(0x10017D20u))(a0, rest...); }
XI_ORIG void nullsub_13_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10017D20u))(a0); }
XI_ORIG void nullsub_13_v2(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10017D20u))(a0, a1); }
template <class... R> XI_ORIG int nullsub_12(void * a0, R... rest) { return reinterpret_cast<int (__cdecl *)(void *, ...)>(XI_GAME(0x10017D30u))(a0, rest...); }
XI_ORIG void nullsub_12_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10017D30u))(a0); }
XI_ORIG undefined8 FUN_100181d0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100181D0u))(a0, a1); }
XI_ORIG void FUN_100182c0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100182C0u))(a0); }
XI_ORIG void _exit_0(int a0) { reinterpret_cast<void (__cdecl *)(int)>(XI_GAME(0x100182E0u))(a0); }
XI_ORIG undefined8 _exit_0_v1(XI_ANY a0) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY)>(XI_GAME(0x100182E0u))(a0); }
XI_ORIG void thunk_FUN_10114f10(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10018300u))(a0, a1, a2); }
XI_ORIG unsigned long long FUN_10018310(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[3] = {a0.bits, a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $12, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10018310u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void * FUN_10018350(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10018350u))(a0); }
XI_ORIG _DWORD * sub_10018430(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10018430u))(a0); }
XI_ORIG void * FUN_10018550(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10018550u))(a0); }
XI_ORIG void sub_100185A0(void * a0, void * a1, unsigned int a2) { reinterpret_cast<void (__thiscall *)(void *, void *, unsigned int)>(XI_GAME(0x100185A0u))(a0, a1, a2); }
XI_ORIG void * CXiOpening_Constructor(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100186C0u))(a0, a1); }
XI_ORIG undefined4 FUN_10018840(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10018840u))(a0); }
XI_ORIG char sub_10018850(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x10018850u))(a0); }
XI_ORIG _DWORD * OpeningSubtitle_Constructor(void * a0, int a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int)>(XI_GAME(0x10018A50u))(a0, a1); }
XI_ORIG _DWORD * OpeningSubtitle_Reset(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10018B30u))(a0); }
XI_ORIG void OpeningSubtitle_Reset_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10018B30u))(a0); }
XI_ORIG _DWORD * Vt10329FE0_Constructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10018C50u))(a0); }
XI_ORIG int Vt10329FE0_Reset(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10018CE0u))(a0); }
XI_ORIG void Vt10329FE0_Reset_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10018CE0u))(a0); }
XI_ORIG MMRESULT App_BeginTimerPeriod() { return reinterpret_cast<MMRESULT (__cdecl *)()>(XI_GAME(0x10019140u))(); }
XI_ORIG MMRESULT sub_100191B0() { return reinterpret_cast<MMRESULT (__cdecl *)()>(XI_GAME(0x100191B0u))(); }
XI_ORIG _DWORD * sub_100191D0() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x100191D0u))(); }
XI_ORIG void FUN_10019210(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10019210u))(a0); }
XI_ORIG void * FUN_10019490(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10019490u))(a0); }
XI_ORIG void FUN_100194f0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100194F0u))(a0); }
XI_ORIG unsigned long long FUN_10019a50(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) {
    uint32_t xs[4] = {a1.bits, a2.bits, a3.bits, a4.bits}; uint32_t ra = a5.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 12(%%esi)\n\t""pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10019A50u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 SampleAnimationTrack_ToBuffer(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10019B30u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined4 SampleAnimationTrack_Live(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10019EE0u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int CYyMotionQue_SetSlotMotion(void * a0, void * a1, void * a2, int a3, int a4, int a5, float a6, float a7, unsigned int a8, int a9, int a10) { return reinterpret_cast<int (__thiscall *)(void *, void *, void *, int, int, int, float, float, unsigned int, int, int)>(XI_GAME(0x1001A660u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }
XI_ORIG void CYyMotionQue_SetSlotMotion_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9, XI_ANY a10) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1001A660u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }
XI_ORIG void FUN_1001a6e0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9, XI_ANY a10) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1001A6E0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }
XI_ORIG void FUN_1001abf0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1001ABF0u))(a0); }
XI_ORIG void FUN_1001ac30(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1001AC30u))(a0); }
XI_ORIG void FUN_1001ac40(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1001AC40u))(a0); }
XI_ORIG void FUN_1001acb0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1001ACB0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG void KzMotionQueList_AppendSync(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1001AD00u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG void FUN_1001b0f0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1001B0F0u))(a0, a1); }
XI_ORIG void FUN_1001b140(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1001B140u))(a0); }
XI_ORIG void FUN_1001b170(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1001B170u))(a0, a1); }
XI_ORIG int FUN_1001b1a0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1001B1A0u))(a0); }
XI_ORIG void FUN_1001b1f0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1001B1F0u))(a0, a1, a2); }
XI_ORIG undefined4 CYyMotionQue_SampleInto(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1001B230u))(a0, a1, a2); }
XI_ORIG void FUN_1001b2e0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1001B2E0u))(a0, a1, a2); }
XI_ORIG int CYyMotionQue_IsActive(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1001B340u))(a0); }
XI_ORIG void FUN_1001b520(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1001B520u))(a0, a1); }
XI_ORIG void FUN_1001b590(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1001B590u))(a0); }
XI_ORIG undefined4 FUN_1001b5d0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1001B5D0u))(a0); }
XI_ORIG undefined4 FUN_1001b710(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1001B710u))(a0); }
XI_ORIG unsigned long long FUN_1001b7b0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1001B7B0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG _DWORD * Model_CreateTranslucentTargets() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x1001D0A0u))(); }
XI_ORIG void FUN_1001d110() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x1001D110u))(); }
XI_ORIG void FUN_1001d170(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1001D170u))(a0); }
XI_ORIG void FUN_1001d1f0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1001D1F0u))(a0); }
XI_ORIG char Model_BeginTranslucentPass(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x1001D240u))(a0); }
XI_ORIG char Model_TranslucentUnbindTexture(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1001D350u))(a0); }
XI_ORIG int sub_1001D390(int a0, char a1) { return reinterpret_cast<int (__stdcall *)(int, char)>(XI_GAME(0x1001D390u))(a0, a1); }
XI_ORIG int Model_CompositeTranslucentPass(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1001D3B0u))(a0); }
XI_ORIG int Gfx_DrawScreenQuad256(int a0, int a1) { return reinterpret_cast<int (__stdcall *)(int, int)>(XI_GAME(0x1001DD00u))(a0, a1); }
XI_ORIG unsigned int Tex_HeightToNormalMap(void * a0, int a1, int a2) { return reinterpret_cast<unsigned int (__thiscall *)(void *, int, int)>(XI_GAME(0x1001E0A0u))(a0, a1, a2); }
XI_ORIG unsigned long long FUN_1001e2f0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[3] = {a0.bits, a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $12, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1001E2F0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 CYyCamMng2_CreateGlobal() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1001E470u))(); }
XI_ORIG long double FUN_1001e4d0() { return reinterpret_cast<long double (__cdecl *)()>(XI_GAME(0x1001E4D0u))(); }
XI_ORIG undefined4 FUN_1001e4f0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1001E4F0u))(a0); }
XI_ORIG undefined4 FUN_1001e590(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1001E590u))(a0); }
XI_ORIG void * FUN_1001e600(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1001E600u))(a0); }
XI_ORIG undefined4 FUN_1001e760(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1001E760u))(a0, a1); }
XI_ORIG undefined4 FUN_1001e780(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1001E780u))(a0, a1); }
XI_ORIG void YmCameraManager_Follow(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1001EE60u))(a0, a1); }
XI_ORIG void FUN_10021110(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10021110u))(a0, a1); }
XI_ORIG void FUN_10021680(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10021680u))(a0, a1, a2); }
XI_ORIG int FUN_10021c10(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10021C10u))(a0); }
XI_ORIG void FUN_100220e0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100220E0u))(a0); }
XI_ORIG double Actor_GetPercentScaleByIndex(void * a0, unsigned int a1) { return reinterpret_cast<double (__thiscall *)(void *, unsigned int)>(XI_GAME(0x10022120u))(a0, a1); }
XI_ORIG undefined4 FUN_10022170(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10022170u))(a0); }
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
XI_ORIG void * FUN_10025010(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10025010u))(a0); }
XI_ORIG _DWORD * CYyModelDt_Destructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x100250C0u))(a0); }
XI_ORIG undefined4 FUN_100250f0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100250F0u))(a0, a1); }
XI_ORIG void __sub_10095B10() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10025220u))(); }
XI_ORIG void FUN_10025260(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10025260u))(a0); }
XI_ORIG int ProjTex_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100252F0u))(a0); }
XI_ORIG void ProjTex_Destructor_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100252F0u))(a0); }
XI_ORIG char ProjTex_BeginPass(void * a0, void * a1, int a2, void * a3) { return reinterpret_cast<char (__thiscall *)(void *, void *, int, void *)>(XI_GAME(0x10025330u))(a0, a1, a2, a3); }
XI_ORIG int ProjTex_EndPass(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x100255C0u))(a0, a1); }
XI_ORIG int TexObj_GetD3DTexture(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10025860u))(a0); }
XI_ORIG int ProjTex_CaptureToTarget(void * a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int)>(XI_GAME(0x10025870u))(a0, a1, a2, a3, a4); }
XI_ORIG int nullsub_39() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10025BD0u))(); }
XI_ORIG void FUN_10025be0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10025BE0u))(); }
XI_ORIG void FUN_10025bf0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10025BF0u))(a0); }
XI_ORIG void FUN_10025c30(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10025C30u))(a0); }
XI_ORIG char EnvMap_BeginPass(void * a0, int a1, void * a2) { return reinterpret_cast<char (__stdcall *)(void *, int, void *)>(XI_GAME(0x10025C40u))(a0, a1, a2); }
XI_ORIG int EnvMap_EndPass(int a0) { return reinterpret_cast<int (__stdcall *)(int)>(XI_GAME(0x10025F70u))(a0); }
XI_ORIG int sub_10026210(int a0, int a1, int a2, int a3) { return reinterpret_cast<int (__stdcall *)(int, int, int, int)>(XI_GAME(0x10026210u))(a0, a1, a2, a3); }
XI_ORIG int * CYyIcon_Constructor(void * a0, int a1, void * a2, int a3) { return reinterpret_cast<int * (__thiscall *)(void *, int, void *, int)>(XI_GAME(0x10026890u))(a0, a1, a2, a3); }
XI_ORIG void CXiMovie_Draw(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10026AD0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int CYyIcon_DrawSubRect(void * a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int, int, int, int, int, int)>(XI_GAME(0x10026C70u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9); }
XI_ORIG _DWORD * sub_10026E00(void * a0) { return reinterpret_cast<_DWORD * (__cdecl *)(void *)>(XI_GAME(0x10026E00u))(a0); }
XI_ORIG void Quat_SetIdentity(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10026E10u))(a0); }
XI_ORIG int Vec4_Set(int a0, float a1, int a2, int a3, int a4) { return reinterpret_cast<int (__cdecl *)(int, float, int, int, int)>(XI_GAME(0x10026E30u))(a0, a1, a2, a3, a4); }
XI_ORIG void Vec4_Set_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10026E30u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_10026e50(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10026E50u))(a0, a1, a2, a3); }
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
XI_ORIG undefined4 FUN_100270a0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100270A0u))(a0, a1); }
XI_ORIG float * fn_100270A0(void * a0, void * a1) { return reinterpret_cast<float * (__cdecl *)(void *, void *)>(XI_GAME(0x100270A0u))(a0, a1); }
XI_ORIG float * Vec3_Sub(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, void *)>(XI_GAME(0x10027120u))(a0, a1, a2); }
XI_ORIG undefined4 Vec3_Sub_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10027120u))(a0, a1, a2); }
XI_ORIG void FUN_100272b0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100272B0u))(a0, a1); }
XI_ORIG int fn_100272B0(int a0, float a1) { return reinterpret_cast<int (__cdecl *)(int, float)>(XI_GAME(0x100272B0u))(a0, a1); }
XI_ORIG float * Vec3_Scale(void * a0, void * a1, float a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, float)>(XI_GAME(0x100272E0u))(a0, a1, a2); }
XI_ORIG undefined4 Vec3_Scale_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100272E0u))(a0, a1, a2); }
XI_ORIG float * Vec3_MulInPlace(void * a0, void * a1) { return reinterpret_cast<float * (__cdecl *)(void *, void *)>(XI_GAME(0x10027310u))(a0, a1); }
XI_ORIG void Vec3_MulInPlace_v1(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10027310u))(a0, a1); }
XI_ORIG undefined4 FUN_10027340(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10027340u))(a0, a1, a2); }
XI_ORIG float * sub_10027340(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, void *)>(XI_GAME(0x10027340u))(a0, a1, a2); }
XI_ORIG unsigned long long FUN_100274b0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100274B0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void sub_100274B0(void * a0) { reinterpret_cast<void (__cdecl *)(void *)>(XI_GAME(0x100274B0u))(a0); }
XI_ORIG void FUN_10027510(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10027510u))(a0, a1); }
XI_ORIG long double FUN_10027530(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<long double (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10027530u))(a0, a1); }
XI_ORIG long double FUN_100275d0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<long double (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100275D0u))(a0, a1); }
XI_ORIG long double FUN_10027600(XI_ANY a0) { return reinterpret_cast<long double (__cdecl *)(XI_ANY)>(XI_GAME(0x10027600u))(a0); }
XI_ORIG undefined4 Vec3_Lerp(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100276A0u))(a0, a1, a2, a3); }
XI_ORIG void Gfx_ProjectWorldToClip(void * a0, void * a1) { reinterpret_cast<void (__cdecl *)(void *, void *)>(XI_GAME(0x100276F0u))(a0, a1); }
XI_ORIG float * Gfx_NdcToScreen(void * a0) { return reinterpret_cast<float * (__cdecl *)(void *)>(XI_GAME(0x100278E0u))(a0); }
XI_ORIG undefined4 FUN_10027990(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10027990u))(a0); }
XI_ORIG Iostream_init * fn_10027990(void * a0) { return reinterpret_cast<Iostream_init * (__thiscall *)(void *)>(XI_GAME(0x10027990u))(a0); }
XI_ORIG void nullsub_1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100279A0u))(); }
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
XI_ORIG undefined4 FUN_10028170(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10028170u))(a0, a1, a2); }
XI_ORIG float * Mat4_TransformPointInPlace(void * a0, void * a1) { return reinterpret_cast<float * (__thiscall *)(void *, void *)>(XI_GAME(0x10028200u))(a0, a1); }
XI_ORIG undefined4 Mat4_TransformPointInPlace_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10028200u))(a0, a1); }
XI_ORIG undefined8 FUN_10028230(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10028230u))(a0, a1, a2); }
XI_ORIG float * Mat4_TransformNormalInPlace(void * a0, void * a1) { return reinterpret_cast<float * (__thiscall *)(void *, void *)>(XI_GAME(0x10028290u))(a0, a1); }
XI_ORIG undefined4 Mat4_TransformNormalInPlace_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10028290u))(a0, a1); }
XI_ORIG float * Mat4_TransformNormal(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__thiscall *)(void *, void *, void *)>(XI_GAME(0x100282C0u))(a0, a1, a2); }
XI_ORIG undefined4 Mat4_TransformNormal_v1(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x100282C0u))(a0, a1, a2); }
XI_ORIG void Mat4_TransformCoordInPlace(void * a0, void * a1) { reinterpret_cast<void (__thiscall *)(void *, void *)>(XI_GAME(0x10028320u))(a0, a1); }
XI_ORIG undefined4 Mat4_InvertOrthonormal(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100283C0u))(a0); }
XI_ORIG _DWORD * Mem_AllocObject(int a0, int a1) { return reinterpret_cast<_DWORD * (__cdecl *)(int, int)>(XI_GAME(0x10028640u))(a0, a1); }
XI_ORIG undefined4 * Mem_AllocObject_v1(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10028640u))(a0, a1); }
XI_ORIG _DWORD * Mem_AllocWithFallback(int a0, int a1) { return reinterpret_cast<_DWORD * (__cdecl *)(int, int)>(XI_GAME(0x100286B0u))(a0, a1); }
XI_ORIG undefined8 Mem_AllocWithFallback_v1(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100286B0u))(a0, a1); }
XI_ORIG _DWORD * Mem_FreeObject(int a0) { return reinterpret_cast<_DWORD * (__cdecl *)(int)>(XI_GAME(0x10028700u))(a0); }
XI_ORIG void Mem_FreeObject_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10028700u))(a0); }
XI_ORIG int SharedDeleteHelper(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10028710u))(a0); }
XI_ORIG void SharedDeleteHelper_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10028710u))(a0); }
XI_ORIG int FUN_10028730() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10028730u))(); }
XI_ORIG int FUN_10028730_v1(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x10028730u))(a0); }
XI_ORIG int FUN_100289f0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100289F0u))(); }
XI_ORIG void FUN_10028aa0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10028AA0u))(a0); }
XI_ORIG int MemBlock_GetSize(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10028B90u))(a0); }
XI_ORIG int MemBlock_GetSize_v1(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10028B90u))(a0); }
XI_ORIG unsigned long long FUN_10028bf0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[3] = {a0.bits, a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $12, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10028BF0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_10028c30(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10028C30u))(a0); }
XI_ORIG void FUN_10028c50(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10028C50u))(a0); }
XI_ORIG void FUN_10028c70(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10028C70u))(a0); }
XI_ORIG void FUN_10028c90(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10028C90u))(a0); }
XI_ORIG _DWORD * Mem_AllocFromArea(int a0, int a1) { return reinterpret_cast<_DWORD * (__cdecl *)(int, int)>(XI_GAME(0x10028FD0u))(a0, a1); }
XI_ORIG undefined8 Mem_AllocFromArea_v1(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10028FD0u))(a0, a1); }
XI_ORIG void FUN_10029150(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10029150u))(a0); }
XI_ORIG undefined4 FUN_10029390(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x10029390u))(a0); }
XI_ORIG int CreateSharedVertexBuffers() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10029A50u))(); }
XI_ORIG void FUN_10029b60() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10029B60u))(); }
XI_ORIG void FUN_10029bb0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10029BB0u))(a0); }
XI_ORIG void FUN_10029c00(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10029C00u))(a0); }
XI_ORIG undefined4 * FUN_10029c90(void * a0) { return reinterpret_cast<undefined4 * (__thiscall *)(void *)>(XI_GAME(0x10029C90u))(a0); }
XI_ORIG _DWORD * CYyModelDtBase_Destructor(void * a0, int a1) { return reinterpret_cast<_DWORD * (__fastcall *)(void *, int)>(XI_GAME(0x10029CD0u))(a0, a1); }
XI_ORIG void * FUN_10029d30(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10029D30u))(a0); }
XI_ORIG _DWORD * CYyModelBase_Destructor(void * a0, int a1) { return reinterpret_cast<_DWORD * (__fastcall *)(void *, int)>(XI_GAME(0x10029DD0u))(a0, a1); }
XI_ORIG char CYyModelBase_SetResourceAndRebuild(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(XI_GAME(0x10029E50u))(a0, a1); }
XI_ORIG char * CYyModelBase_GetPartList(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10029E70u))(a0); }
XI_ORIG int CYyModelBase_GetPartList_v1(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10029E70u))(a0); }
XI_ORIG int FUN_10029eb0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10029EB0u))(a0); }
XI_ORIG void FUN_10029f00(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10029F00u))(a0, a1); }
XI_ORIG _DWORD * CYyModelBase_AddPartFromResource(void * a0, int a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int)>(XI_GAME(0x10029F70u))(a0, a1); }
XI_ORIG undefined4 * CYyModelBase_AddPartFromResource_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10029F70u))(a0, a1); }
XI_ORIG void FUN_1002a0c0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1002A0C0u))(a0); }
XI_ORIG void FUN_1002a130(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1002A130u))(a0, a1); }
XI_ORIG undefined4 Model_AnimateAndPose(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1002A140u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int CYyModelBase_DrawLit(int a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(int, int, int, int)>(XI_GAME(0x1002A3B0u))(a0, a1, a2, a3); }
XI_ORIG void FUN_1002a700(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1002A700u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1002a750(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1002A750u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1002a780(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1002A780u))(a0, a1, a2); }
XI_ORIG void FUN_1002a890(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x1002A890u))(a0, a1); }
XI_ORIG void FUN_1002a8b0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1002A8B0u))(a0, a1, a2); }
XI_ORIG int FUN_1002a9b0(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1002A9B0u))(a0, a1); }
XI_ORIG int FUN_1002a9e0(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1002A9E0u))(a0, a1); }
XI_ORIG void CYyModelBase_ForwardToField24(void * a0, int a1, int a2) { reinterpret_cast<void (__thiscall *)(void *, int, int)>(XI_GAME(0x1002AC50u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1002b3f0(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x1002B3F0u))(a0); }
XI_ORIG uint32_t FUN_1002b470(XI_ANY a0) { return reinterpret_cast<uint32_t (__stdcall *)(XI_ANY)>(XI_GAME(0x1002B470u))(a0); }
XI_ORIG char * CYyModel_Constructor(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x1002B4B0u))(a0); }
XI_ORIG int CYyModel_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1002B550u))(a0); }
XI_ORIG char * CYyModel_GetField44Addr(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x1002B5A0u))(a0); }
XI_ORIG int CYyModel_GetField44Addr_v1(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1002B5A0u))(a0); }
XI_ORIG int FUN_1002b5e0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1002B5E0u))(a0); }
XI_ORIG void FUN_1002b630(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1002B630u))(a0, a1); }
XI_ORIG int CYyModel_CreateModelBase(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1002B6A0u))(a0); }
XI_ORIG char CXiSkeletonActor_DrawModel(int a0, void * a1, void * a2, int a3, void * a4, int a5, int a6, unsigned int a7) { return reinterpret_cast<char (__thiscall *)(int, void *, void *, int, void *, int, int, unsigned int)>(XI_GAME(0x1002B6E0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG char * sub_1002C270(void * a0, int a1) { return reinterpret_cast<char * (__thiscall *)(void *, int)>(XI_GAME(0x1002C270u))(a0, a1); }
XI_ORIG char CXiSkeletonActor_AnimateModels(int a0, void * a1, void * a2, int a3, void * a4) { return reinterpret_cast<char (__thiscall *)(int, void *, void *, int, void *)>(XI_GAME(0x1002C2D0u))(a0, a1, a2, a3, a4); }
XI_ORIG void CXiSkeletonActor_PrepareModelsOnce(int a0, int a1, int a2) { reinterpret_cast<void (__thiscall *)(int, int, int)>(XI_GAME(0x1002C360u))(a0, a1, a2); }
XI_ORIG char CXiSkeletonActor_GetElemThunk(void * a0, int a1, int a2, int a3, void * a4, int a5, int a6, int a7) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int, void *, int, int, int)>(XI_GAME(0x1002C3E0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG undefined4 FUN_1002c400(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1002C400u))(a0, a1, a2, a3); }
XI_ORIG __int16 * CXiSkeletonActor_GetModelElem(void * a0, unsigned int a1, int a2) { return reinterpret_cast<__int16 * (__thiscall *)(void *, unsigned int, int)>(XI_GAME(0x1002C5C0u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1002c710(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1002C710u))(a0, a1); }
XI_ORIG uint32_t FUN_1002c750(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1002C750u))(a0, a1); }
XI_ORIG _DWORD * List_GetTail(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1002C7C0u))(a0); }
XI_ORIG void FUN_1002c7e0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1002C7E0u))(a0, a1); }
XI_ORIG void FUN_1002c800(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1002C800u))(a0, a1); }
XI_ORIG _DWORD * Mem_AllocDefault(int a0) { return reinterpret_cast<_DWORD * (__cdecl *)(int)>(XI_GAME(0x1002C890u))(a0); }
XI_ORIG undefined4 Mem_AllocDefault_v1(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1002C890u))(a0); }
XI_ORIG int thunk_sub_10028AA0(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x1002C8B0u))(a0); }
XI_ORIG void thunk_sub_10028AA0_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1002C8B0u))(a0); }
XI_ORIG void thunk_sub_10028AA0_v2() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1002C8B0u))(); }
XI_ORIG char CYyObject_IsKindOf(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(XI_GAME(0x1002C8F0u))(a0, a1); }
XI_ORIG undefined8 CYyObject_IsKindOf_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1002C8F0u))(a0, a1); }
XI_ORIG void * CYyOs2VtxBuffer_Constructor(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1002D680u))(a0, a1); }
XI_ORIG void CYyOs2VtxBuffer_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1002D700u))(a0); }
XI_ORIG void CYyModelDt_SkinAndDrawV2(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1002DC10u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int Gfx_SetStreamSourceCached(unsigned int a0, int a1, int a2) { return reinterpret_cast<int (__cdecl *)(unsigned int, int, int)>(XI_GAME(0x1002F130u))(a0, a1, a2); }
XI_ORIG undefined4 Gfx_ApplyCachedStreamSource(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1002F160u))(a0); }
XI_ORIG int Gfx_ApplyCachedVertexShader() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1002F1A0u))(); }
XI_ORIG int Gfx_SetVertexShaderCached(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x1002F1C0u))(a0); }
XI_ORIG _DWORD * Model_SkinVerticesCpu(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1002F1D0u))(a0); }
XI_ORIG void FUN_100309b0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100309B0u))(a0, a1, a2, a3); }
XI_ORIG void FUN_10030a40(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10030A40u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_10030fb0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10030FB0u))(a0); }
XI_ORIG void * FUN_10032a40(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10032A40u))(a0); }
XI_ORIG void FUN_10032a50(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10032A50u))(a0); }
XI_ORIG void FUN_10032a70() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10032A70u))(); }
XI_ORIG undefined4 FUN_10032b50(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10032B50u))(a0, a1, a2); }
XI_ORIG void FUN_10032da0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10032DA0u))(a0); }
XI_ORIG void BoneXform_PostMultiplyInto(void * a0, int a1) { reinterpret_cast<void (__thiscall *)(void *, int)>(XI_GAME(0x10032FA0u))(a0, a1); }
XI_ORIG void FUN_10032fe0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10032FE0u))(a0, a1); }
XI_ORIG undefined4 Quat_NLerp(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10033220u))(a0, a1, a2, a3); }
XI_ORIG void FUN_10033360(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10033360u))(a0, a1, a2, a3); }
XI_ORIG int sub_100336B0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100336B0u))(); }
XI_ORIG void FUN_100336d0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100336D0u))(); }
XI_ORIG void FUN_10033710(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10033710u))(a0); }
XI_ORIG void FUN_100337c0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100337C0u))(a0); }
XI_ORIG char Model_BeginOffscreenPass(int a0, void * a1, int a2) { return reinterpret_cast<char (__thiscall *)(int, void *, int)>(XI_GAME(0x10033800u))(a0, a1, a2); }
XI_ORIG char Model_EndOffscreenPassBindTextures(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(XI_GAME(0x10033A70u))(a0, a1); }
XI_ORIG char sub_10033DA0(int a0, void * a1) { return reinterpret_cast<char (__thiscall *)(int, void *)>(XI_GAME(0x10033DA0u))(a0, a1); }
XI_ORIG void Shadow_SmoothLightDirection(void * a0, void * a1) { reinterpret_cast<void (__thiscall *)(void *, void *)>(XI_GAME(0x10033EA0u))(a0, a1); }
XI_ORIG void FUN_10034230(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10034230u))(a0); }
XI_ORIG void FUN_10034270(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10034270u))(a0); }
XI_ORIG void FUN_100342a0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100342A0u))(a0); }
XI_ORIG int FUN_100351f0(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100351F0u))(a0, a1); }
XI_ORIG int FUN_10035210(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10035210u))(a0); }
XI_ORIG uint16_t * FUN_10035250(void * a0) { return reinterpret_cast<uint16_t * (__thiscall *)(void *)>(XI_GAME(0x10035250u))(a0); }
XI_ORIG unsigned __int16 * sub_10035250(int a0, int a1) { return reinterpret_cast<unsigned __int16 * (__fastcall *)(int, int)>(XI_GAME(0x10035250u))(a0, a1); }
XI_ORIG undefined4 FUN_100352a0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100352A0u))(a0); }
XI_ORIG void FUN_100352f0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100352F0u))(a0); }
XI_ORIG void FUN_10035330(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10035330u))(a0, a1, a2, a3); }
XI_ORIG void * FUN_10035a00(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10035A00u))(a0, a1, a2, a3); }
XI_ORIG char CMoTask_VT_1032A904_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10035B00u))(a0); }
XI_ORIG unsigned long long MoTask_KillAllGlobalSlots(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10035F00u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_10035f80(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10035F80u))(a0); }
XI_ORIG undefined4 FUN_10035fb0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10035FB0u))(a0); }
XI_ORIG unsigned long long FUN_10036020(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10036020u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_10036050(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10036050u))(a0); }
XI_ORIG unsigned long long MoTask_KillGlobalSlot1(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10036070u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_100360a0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100360A0u))(a0); }
XI_ORIG undefined4 FUN_100360c0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100360C0u))(a0); }
XI_ORIG unsigned long long FUN_100360f0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100360F0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_10036170(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10036170u))(a0); }
XI_ORIG int sub_100361D0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100361D0u))(); }
XI_ORIG char sub_10036230() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x10036230u))(); }
XI_ORIG undefined4 FUN_10036360(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10036360u))(a0); }
XI_ORIG void FUN_100363b0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x100363B0u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10036420(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10036420u))(a0); }
XI_ORIG void FUN_100364a0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100364A0u))(a0); }
XI_ORIG undefined4 FUN_100364e0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100364E0u))(a0); }
XI_ORIG undefined4 FUN_100367c0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100367C0u))(a0); }
XI_ORIG unsigned long long FUN_100367f0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100367F0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG uint32_t FUN_10036830(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x10036830u))(a0); }
XI_ORIG undefined4 FUN_10036870(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10036870u))(a0); }
XI_ORIG void FUN_10036980(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10036980u))(a0); }
XI_ORIG undefined4 FUN_100369f0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100369F0u))(a0); }
XI_ORIG undefined4 FUN_10036b90(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10036B90u))(a0, a1); }
XI_ORIG uint32_t FUN_10036c70(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10036C70u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG undefined8 FUN_10036f00(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10036F00u))(a0, a1, a2); }
XI_ORIG undefined8 FUN_10036f50(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10036F50u))(a0, a1, a2); }
XI_ORIG void FUN_10037020(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10037020u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG void FUN_100378b0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100378B0u))(a0); }
XI_ORIG void FUN_10037960(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10037960u))(a0); }
XI_ORIG void FUN_10037a10() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10037A10u))(); }
XI_ORIG undefined8 SetMusicTrackVolume_A() { return reinterpret_cast<undefined8 (__cdecl *)()>(XI_GAME(0x10037B90u))(); }
XI_ORIG undefined8 SetMusicTrackVolume_B() { return reinterpret_cast<undefined8 (__cdecl *)()>(XI_GAME(0x10037BF0u))(); }
XI_ORIG undefined8 YmSePlayBeep() { return reinterpret_cast<undefined8 (__cdecl *)()>(XI_GAME(0x10037C50u))(); }
XI_ORIG undefined4 FUN_10037d70() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10037D70u))(); }
XI_ORIG undefined4 FUN_10037dd0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10037DD0u))(); }
XI_ORIG undefined4 FUN_10037e30() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10037E30u))(); }
XI_ORIG void FUN_10037e90() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10037E90u))(); }
XI_ORIG void FUN_10037ef0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10037EF0u))(); }
XI_ORIG undefined4 FUN_10037fb0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10037FB0u))(); }
XI_ORIG unsigned long long FUN_100382a0(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[2] = {a0.bits, a1.bits}; uint32_t ra = a2.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $8, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100382A0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_10038440(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[3] = {a0.bits, a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $12, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10038440u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_10038470(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[3] = {a0.bits, a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $12, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10038470u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned int Font_QueueText(void * a0, int a1, int a2, char a3) { return reinterpret_cast<unsigned int (__cdecl *)(void *, int, int, char)>(XI_GAME(0x100384B0u))(a0, a1, a2, a3); }
XI_ORIG int Font_DrawPass() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10038520u))(); }
XI_ORIG unsigned __int8 * Font_DrawString(void * a0, float a1, float a2, float a3) { return reinterpret_cast<unsigned __int8 * (__cdecl *)(void *, float, float, float)>(XI_GAME(0x10038720u))(a0, a1, a2, a3); }
XI_ORIG _DWORD * Post_CreateScreenEffectTargets() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x10038930u))(); }
XI_ORIG void FUN_100389c0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100389C0u))(); }
XI_ORIG void FUN_10038a20(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10038A20u))(a0); }
XI_ORIG void FUN_10038a60(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10038A60u))(a0); }
XI_ORIG void sub_10038A70(int a0) { reinterpret_cast<void (__stdcall *)(int)>(XI_GAME(0x10038A70u))(a0); }
XI_ORIG int Post_CaptureSceneToTarget(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10038A80u))(a0); }
XI_ORIG int Post_DrawRotatingGhostLayers(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10038E10u))(a0); }
XI_ORIG _DWORD * sub_10039410(void * a0, void * a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *)>(XI_GAME(0x10039410u))(a0, a1); }
XI_ORIG void * FUN_10039440(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10039440u))(a0); }
XI_ORIG void ThreadPool_StartThread(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100394D0u))(a0, a1, a2, a3); }
XI_ORIG void FUN_10039550(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10039550u))(a0); }
XI_ORIG undefined4 FUN_10039590(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10039590u))(a0, a1); }
XI_ORIG undefined4 FUN_100395d0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100395D0u))(a0); }
XI_ORIG void FUN_10039670(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10039670u))(a0, a1, a2, a3); }
XI_ORIG void FUN_100396a0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100396A0u))(a0); }
XI_ORIG void FUN_100397a0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100397A0u))(a0, a1); }
XI_ORIG int Util_Copy16Bytes(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10039810u))(a0, a1); }
XI_ORIG void Util_Copy16Bytes_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10039810u))(a0, a1); }
XI_ORIG void * FUN_10039830(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10039830u))(a0); }
XI_ORIG void CYyTexBase_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100398D0u))(a0); }
XI_ORIG int CYyTexBase_SetName(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x100398E0u))(a0, a1); }
XI_ORIG void CYyTexBase_SetName_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100398E0u))(a0, a1); }
XI_ORIG void CYyTex_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10039970u))(a0); }
XI_ORIG void FUN_100399d0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100399D0u))(a0); }
XI_ORIG char TexObj_CreateAndUpload(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10039A30u))(a0); }
XI_ORIG undefined1 TexObj_CreateAndUpload_v1(void * a0) { return reinterpret_cast<undefined1 (__thiscall *)(void *)>(XI_GAME(0x10039A30u))(a0); }
XI_ORIG undefined1 TexObj_Reupload(void * a0) { return reinterpret_cast<undefined1 (__thiscall *)(void *)>(XI_GAME(0x10039A60u))(a0); }
XI_ORIG undefined4 TexObj_Create(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10039A80u))(a0); }
XI_ORIG int TexObj_Upload(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10039E50u))(a0); }
XI_ORIG undefined4 TexObj_Upload_v1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10039E50u))(a0); }
XI_ORIG _WORD * sub_1003AA50(void * a0) { return reinterpret_cast<_WORD * (__thiscall *)(void *)>(XI_GAME(0x1003AA50u))(a0); }
XI_ORIG int CYyTexMng_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003AA90u))(a0); }
XI_ORIG int FUN_1003aab0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003AAB0u))(a0); }
XI_ORIG int * FUN_1003ab10(void * a0, XI_ANY a1) { return reinterpret_cast<int * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1003AB10u))(a0, a1); }
XI_ORIG void FUN_1003ab40(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x1003AB40u))(a0); }
XI_ORIG char TexMgr_UploadPumpEntry(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1003AB70u))(a0); }
XI_ORIG undefined4 TexMgr_UploadPump(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1003ABB0u))(a0); }
XI_ORIG int CYyTexMng_FindByName(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1003AC80u))(a0, a1); }
XI_ORIG undefined8 CYyTexMng_FindByName_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1003AC80u))(a0, a1); }
XI_ORIG int sub_1003ACD0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1003ACD0u))(a0, a1); }
XI_ORIG int sub_1003AD40(void * a0, void * a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, void *, int)>(XI_GAME(0x1003AD40u))(a0, a1, a2); }
XI_ORIG int sub_1003ADC0(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1003ADC0u))(a0, a1, a2); }
XI_ORIG _DWORD * TexMgr_FindByName(void * a0, int a1, int a2, char a3) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int, int, char)>(XI_GAME(0x1003AEA0u))(a0, a1, a2, a3); }
XI_ORIG int TexMgr_FindByName_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1003AEA0u))(a0, a1, a2, a3); }
XI_ORIG int CYyTexMng_FindByNameInResource(void * a0, void * a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, void *, int)>(XI_GAME(0x1003AF10u))(a0, a1, a2); }
XI_ORIG int CYyTexMng_FindByNameInResource_v1(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1003AF10u))(a0, a1, a2); }
XI_ORIG _DWORD * sub_1003B080(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1003B080u))(a0); }
XI_ORIG int CYyVbMng_AcquireVertexBuffer(void * a0, int a1, unsigned int a2, unsigned int a3, D3DPOOL a4, void * a5, char a6) { return reinterpret_cast<int (__thiscall *)(void *, int, unsigned int, unsigned int, D3DPOOL, void *, char)>(XI_GAME(0x1003B210u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG _DWORD * CYyVbMng_ReleaseVertexBuffer(void * a0) { return reinterpret_cast<_DWORD * (__stdcall *)(void *)>(XI_GAME(0x1003B3C0u))(a0); }
XI_ORIG _DWORD * CYyVb_Constructor(void * a0, int a1, int a2, int a3, int a4, int a5) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int, int, int, int, int)>(XI_GAME(0x1003B450u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void FUN_1003b4a0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1003B4A0u))(a0); }
XI_ORIG void FUN_1003b4d0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1003B4D0u))(a0); }
XI_ORIG void FUN_1003b4e0() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x1003B4E0u))(); }
XI_ORIG void * FUN_1003b540(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1003B540u))(a0); }
XI_ORIG void ReleaseSubObjectIfPresent(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1003B5C0u))(a0); }
XI_ORIG void ReleaseSubObjectIfPresent_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1003B5C0u))(a0); }
XI_ORIG int CMoAttachments_Reset(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003B640u))(a0); }
XI_ORIG undefined4 CMoAttachments_Reset_v1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1003B640u))(a0); }
XI_ORIG undefined4 FUN_1003b690(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1003B690u))(a0); }
XI_ORIG void FUN_1003b690_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1003B690u))(a0); }
XI_ORIG undefined8 FUN_1003b6d0(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x1003B6D0u))(a0); }
XI_ORIG int sub_1003B6D0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003B6D0u))(a0); }
XI_ORIG undefined8 FUN_1003b700(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x1003B700u))(a0); }
XI_ORIG int sub_1003B700(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003B700u))(a0); }
XI_ORIG int CMoAttachments_ClearCaster(void * a0, char a1) { return reinterpret_cast<int (__thiscall *)(void *, char)>(XI_GAME(0x1003B750u))(a0, a1); }
XI_ORIG void CMoAttachments_ClearCaster_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1003B750u))(a0, a1); }
XI_ORIG int CMoAttachments_ClearTarget(void * a0, char a1) { return reinterpret_cast<int (__thiscall *)(void *, char)>(XI_GAME(0x1003B780u))(a0, a1); }
XI_ORIG void CMoAttachments_ClearTarget_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1003B780u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_SetCaster(void * a0, void * a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *)>(XI_GAME(0x1003B7B0u))(a0, a1); }
XI_ORIG void CMoAttachments_SetCaster_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1003B7B0u))(a0, a1); }
XI_ORIG _DWORD * CMoAttachments_SetTarget(void * a0, void * a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *)>(XI_GAME(0x1003B830u))(a0, a1); }
XI_ORIG undefined4 CMoAttachments_SetTarget_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1003B830u))(a0, a1); }
XI_ORIG int FUN_1003b8b0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003B8B0u))(a0); }
XI_ORIG int FUN_1003b900(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003B900u))(a0); }
XI_ORIG long double sub_1003B980(void * a0, void * a1, void * a2) { return reinterpret_cast<long double (__cdecl *)(void *, void *, void *)>(XI_GAME(0x1003B980u))(a0, a1, a2); }
XI_ORIG float * sub_1003BA20(void * a0, int a1, void * a2, void * a3, int a4) { return reinterpret_cast<float * (__cdecl *)(void *, int, void *, void *, int)>(XI_GAME(0x1003BA20u))(a0, a1, a2, a3, a4); }
XI_ORIG int sub_1003BD90(int a0, int a1, int a2) { return reinterpret_cast<int (__cdecl *)(int, int, int)>(XI_GAME(0x1003BD90u))(a0, a1, a2); }
XI_ORIG long double YmGenerater_BuildAttachMatrix(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<long double (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1003BE10u))(a0, a1, a2, a3); }
XI_ORIG void FUN_1003c6e0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1003C6E0u))(a0); }
XI_ORIG void FUN_1003c700(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1003C700u))(a0); }
XI_ORIG void FUN_1003c740(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1003C740u))(a0, a1); }
XI_ORIG char ** CMoCamera_GetRuntimeClass() { return reinterpret_cast<char ** (__cdecl *)()>(XI_GAME(0x1003C880u))(); }
XI_ORIG char CMoCameraTask_Destructor(void * a0, int a1) { return reinterpret_cast<char (__fastcall *)(void *, int)>(XI_GAME(0x1003D030u))(a0, a1); }
XI_ORIG int Sky_DrawGradientDome(float a0, float a1, int a2, void * a3, float a4) { return reinterpret_cast<int (__cdecl *)(float, float, int, void *, float)>(XI_GAME(0x1003D640u))(a0, a1, a2, a3, a4); }
XI_ORIG void CMoCelestial_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1003D980u))(a0); }
XI_ORIG void Sky_DestroyDomeShaderStatic() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1003DB50u))(); }
XI_ORIG void Sky_DrawDomeViewCentered() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1003DB60u))(); }
XI_ORIG void Sky_DestroyViewCenteredShaderStatic() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1003DDF0u))(); }
XI_ORIG unsigned int Color_SetFromU32SwapRB(void * a0, unsigned int a1) { return reinterpret_cast<unsigned int (__thiscall *)(void *, unsigned int)>(XI_GAME(0x1003E280u))(a0, a1); }
XI_ORIG char Color_CopySwapRB(void * a0, void * a1) { return reinterpret_cast<char (__thiscall *)(void *, void *)>(XI_GAME(0x1003E2A0u))(a0, a1); }
XI_ORIG uint32_t * FUN_1003e330(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<uint32_t * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1003E330u))(a0, a1, a2); }
XI_ORIG int sub_1003E910(int a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(int, int, int, int)>(XI_GAME(0x1003E910u))(a0, a1, a2, a3); }
XI_ORIG int YmElem_SetD3ADrawState(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003E9A0u))(a0); }
XI_ORIG void unknown_libname_20() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1003EB00u))(); }
XI_ORIG void unknown_libname_21() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1003EB10u))(); }
XI_ORIG void unknown_libname_22() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1003EB20u))(); }
XI_ORIG char CMoVuAnmElem_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1003EF20u))(a0); }
XI_ORIG int CMoDistMorphElem_ApplyMorph(int a0, int a1, void * a2, int a3, int a4, void * a5, char a6) { return reinterpret_cast<int (__thiscall *)(int, int, void *, int, int, void *, char)>(XI_GAME(0x1003F440u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG void unknown_libname_25() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1003F890u))(); }
XI_ORIG _DWORD * CMoDistMorphElem_ComputeMorphHandle(int a0, int a1, int a2) { return reinterpret_cast<_DWORD * (__thiscall *)(int, int, int)>(XI_GAME(0x1003F8A0u))(a0, a1, a2); }
XI_ORIG char CXi471011C8_VT_1032AC90_Dtor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x1003FB10u))(a0); }
XI_ORIG undefined8 CXi471011C8_VT_1032AC90_Ctor(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x1003FB70u))(a0); }
XI_ORIG uint8_t sub_1003FCB0(void * a0) { return reinterpret_cast<uint8_t (__thiscall *)(void *)>(XI_GAME(0x1003FCB0u))(a0); }
XI_ORIG int FUN_1003fce0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003FCE0u))(a0); }
XI_ORIG char * sub_1003FCE0(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x1003FCE0u))(a0); }
XI_ORIG int FUN_1003fcf0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1003FCF0u))(a0); }
XI_ORIG char * sub_1003FCF0(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x1003FCF0u))(a0); }
XI_ORIG void FUN_10040220(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10040220u))(a0, a1, a2); }
XI_ORIG void unknown_libname_33() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10040B50u))(); }
XI_ORIG void unknown_libname_34() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10040B60u))(); }
XI_ORIG void unknown_libname_35() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10040B70u))(); }
XI_ORIG void unknown_libname_36() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10040B80u))(); }
XI_ORIG void unknown_libname_37() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10040B90u))(); }
XI_ORIG void CMoElemFamily_SharedEarlyExit(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10040C20u))(a0); }
XI_ORIG void unknown_libname_38() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10040E10u))(); }
XI_ORIG char CXi471011C8_VT_1032AD80_Dtor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10041110u))(a0); }
XI_ORIG void Font_DrawProjectedText3D(int a0, int a1, int a2, int a3, int a4) { reinterpret_cast<void (__cdecl *)(int, int, int, int, int)>(XI_GAME(0x10041C20u))(a0, a1, a2, a3, a4); }
XI_ORIG _DWORD * CMoOtTask_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x100425F0u))(a0, a1); }
XI_ORIG void unknown_libname_46() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10042CD0u))(); }
XI_ORIG void unknown_libname_47() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10042CE0u))(); }
XI_ORIG void nullsub_136() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10042CF0u))(); }
XI_ORIG char CMoDistMorphElem_ValidateMorphData(int a0, int a1, int a2, int a3) { return reinterpret_cast<char (__stdcall *)(int, int, int, int)>(XI_GAME(0x10042D00u))(a0, a1, a2, a3); }
XI_ORIG void unknown_libname_48() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100430A0u))(); }
XI_ORIG void unknown_libname_49() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10043310u))(); }
XI_ORIG void StaticDtor_StateBlock_1047BB7C() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100439B0u))(); }
XI_ORIG void * CMoTask_VT_1032AF60_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<void * (__thiscall *)(void *, char)>(XI_GAME(0x10043A90u))(a0, a1); }
XI_ORIG char CMoTask_VT_1032AF60_Destructor(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x10043AB0u))(a0); }
XI_ORIG int * CMoTask_VT_1032AFA8_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<int * (__thiscall *)(void *, char)>(XI_GAME(0x10043C90u))(a0, a1); }
XI_ORIG char CMoTask_VT_1032AFA8_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10043CB0u))(a0); }
XI_ORIG undefined4 FUN_10043de0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10043DE0u))(); }
XI_ORIG int sub_10043DE0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10043DE0u))(); }
XI_ORIG char * DynVB_Constructor(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10043DF0u))(a0); }
XI_ORIG undefined8 DynVB_Constructor_v1(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x10043DF0u))(a0); }
XI_ORIG void DynVB_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10043E40u))(a0); }
XI_ORIG int DynVB_Create(void * a0, int a1, unsigned int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, unsigned int, int)>(XI_GAME(0x10043E50u))(a0, a1, a2, a3); }
XI_ORIG void sub_10043F00(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10043F00u))(a0); }
XI_ORIG int VertexBatch_BindAndDraw(void * a0, int a1, int a2, int a3, unsigned int a4, int a5) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, unsigned int, int)>(XI_GAME(0x10043F40u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void VertexBatch_BindAndDraw_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10043F40u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int DynVB_Reserve(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10044010u))(a0, a1); }
XI_ORIG void * DynVB_Lock(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10044130u))(a0); }
XI_ORIG int DynVB_Unlock(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100441A0u))(a0); }
XI_ORIG int CameraCache_CacheViewProjection(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100441C0u))(a0); }
XI_ORIG int Gfx_SetViewMatrixFromField10(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10044210u))(a0); }
XI_ORIG int Gfx_SetViewMatrixFromField90(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10044230u))(a0); }
XI_ORIG void FUN_10044250(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10044250u))(a0, a1); }
XI_ORIG _DWORD * Mo_InitSharedRenderResources() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x10044270u))(); }
XI_ORIG void FUN_100446a0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100446A0u))(a0); }
XI_ORIG undefined8 CMoElem_Constructor(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x10044890u))(a0); }
XI_ORIG char CMoElemFamily_SharedCleanup(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10044970u))(a0); }
XI_ORIG void CMoElemFamily_SharedCleanup_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10044970u))(a0); }
XI_ORIG undefined4 FUN_10044aa0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10044AA0u))(a0, a1); }
XI_ORIG undefined4 FUN_10044ae0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10044AE0u))(a0); }
XI_ORIG undefined4 FUN_10044c40(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10044C40u))(a0, a1); }
XI_ORIG float * sub_10044C40(int a0, void * a1) { return reinterpret_cast<float * (__thiscall *)(int, void *)>(XI_GAME(0x10044C40u))(a0, a1); }
XI_ORIG undefined4 FUN_10044cf0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10044CF0u))(a0); }
XI_ORIG void FUN_10044d40(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10044D40u))(a0); }
XI_ORIG undefined4 YmElem_ComputeViewDepthKey(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10044F10u))(a0); }
XI_ORIG undefined4 FUN_10045190(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10045190u))(a0, a1, a2, a3); }
XI_ORIG int sub_10045800(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x10045800u))(a0); }
XI_ORIG undefined4 FUN_100458f0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100458F0u))(a0); }
XI_ORIG unsigned long long YmElem_BuildDrawMatrixAndTest(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10045920u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void nullsub_140() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10045A30u))(); }
XI_ORIG int FUN_10045c50(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10045C50u))(a0); }
XI_ORIG int sub_10045C50(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10045C50u))(a0); }
XI_ORIG int RegisterWithProcessor(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10045D40u))(a0); }
XI_ORIG void RegisterWithProcessor_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10045D40u))(a0); }
XI_ORIG void FUN_10046380() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10046380u))(); }
XI_ORIG int sub_100464D0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100464D0u))(); }
XI_ORIG int sub_10046550(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10046550u))(a0, a1); }
XI_ORIG uint32_t YmElem_SetupDevice(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100467E0u))(a0, a1); }
XI_ORIG void unknown_libname_53() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100472D0u))(); }
XI_ORIG void YmElem_RestoreDevice(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100472F0u))(a0, a1); }
XI_ORIG void unknown_libname_54() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100473E0u))(); }
XI_ORIG int FUN_100473f0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100473F0u))(a0); }
XI_ORIG int sub_100473F0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100473F0u))(a0); }
XI_ORIG int FUN_10047450(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10047450u))(a0); }
XI_ORIG int FUN_10047490(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10047490u))(a0); }
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
XI_ORIG int FUN_100493b0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100493B0u))(a0); }
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
XI_ORIG void FUN_1004e720(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1004E720u))(a0, a1); }
XI_ORIG __int16 SharedConditionalStateReset(int a0) { return reinterpret_cast<__int16 (__thiscall *)(int)>(XI_GAME(0x100531D0u))(a0); }
XI_ORIG __int16 sub_10053370(int a0) { return reinterpret_cast<__int16 (__thiscall *)(int)>(XI_GAME(0x10053370u))(a0); }
XI_ORIG int CMoGeneratorClone_CopyParams(int a0, int a1) { return reinterpret_cast<int (__thiscall *)(int, int)>(XI_GAME(0x10053450u))(a0, a1); }
XI_ORIG int CMoGeneratorClone_CreateInstance(void * a0, void * a1, void * a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, void *, void *, int)>(XI_GAME(0x100536C0u))(a0, a1, a2, a3); }
XI_ORIG char * CMoGeneratorClone_VT_1032B69C_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<char * (__thiscall *)(void *, char)>(XI_GAME(0x10053840u))(a0, a1); }
XI_ORIG unsigned long long FUN_10053890(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10053890u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_10053890_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10053890u))(a0, a1); }
XI_ORIG int sub_10053890(int a0, char a1) { return reinterpret_cast<int (__thiscall *)(int, char)>(XI_GAME(0x10053890u))(a0, a1); }
XI_ORIG char YmGenerater_Stop(int a0, void * a1) { return reinterpret_cast<char (__thiscall *)(int, void *)>(XI_GAME(0x100538E0u))(a0, a1); }
XI_ORIG unsigned long long YmGenerater_Stop_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100538E0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 YmGenerater_Stop_v2(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100538E0u))(a0, a1); }
XI_ORIG undefined4 FUN_100539e0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100539E0u))(a0); }
XI_ORIG uint8_t sub_100539E0(void * a0) { return reinterpret_cast<uint8_t (__thiscall *)(void *)>(XI_GAME(0x100539E0u))(a0); }
XI_ORIG undefined4 FUN_10053a00(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10053A00u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_10053c80(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10053C80u))(a0, a1, a2); }
XI_ORIG unsigned long long FUN_10053dd0(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10053DD0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_10053e60(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10053E60u))(a0); }
XI_ORIG char CMoTask_VT_1032B654_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100541C0u))(a0); }
XI_ORIG char ** CMoGeneratorClone_GetRuntimeClass() { return reinterpret_cast<char ** (__cdecl *)()>(XI_GAME(0x100542E0u))(); }
XI_ORIG int CMoGeneratorClone_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100542F0u))(a0); }
XI_ORIG __int16 CMoGeneratorClone_Kill(int a0) { return reinterpret_cast<__int16 (__cdecl *)(int)>(XI_GAME(0x100543A0u))(a0); }
XI_ORIG long double FUN_100544d0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<long double (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x100544D0u))(a0, a1, a2); }
XI_ORIG long double FUN_10054500(void * a0, XI_ANY a1) { return reinterpret_cast<long double (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10054500u))(a0, a1); }
XI_ORIG long double FUN_100546f0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<long double (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x100546F0u))(a0, a1, a2); }
XI_ORIG long double FUN_100547a0(void * a0, XI_ANY a1) { return reinterpret_cast<long double (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100547A0u))(a0, a1); }
XI_ORIG char CMoLfdElem_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100548C0u))(a0); }
XI_ORIG uint32_t BeginEffectOffscreenPass() { return reinterpret_cast<uint32_t (__cdecl *)()>(XI_GAME(0x10054D70u))(); }
XI_ORIG void nullsub_130() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10054F70u))(); }
XI_ORIG void EndEffectOffscreenPass(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x10054F80u))(a0, a1); }
XI_ORIG void FUN_10054fc0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10054FC0u))(a0); }
XI_ORIG int Vt1032B6D8_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10055000u))(a0); }
XI_ORIG unsigned long long FUN_100550f0(XI_ANY a0, XI_ANY a1) {
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
XI_ORIG void * FUN_100552a0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x100552A0u))(a0); }
XI_ORIG _DWORD * CMoOT_Destructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x100552E0u))(a0); }
XI_ORIG void FUN_100552f0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100552F0u))(a0); }
XI_ORIG void FUN_10055300(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10055300u))(a0, a1, a2, a3); }
XI_ORIG void FUN_100554b0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100554B0u))(a0); }
XI_ORIG void FUN_10055520(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10055520u))(a0); }
XI_ORIG undefined8 FUN_10055570(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x10055570u))(a0); }
XI_ORIG undefined4 FUN_100555b0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100555B0u))(a0, a1); }
XI_ORIG char sub_100555B0(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(XI_GAME(0x100555B0u))(a0, a1); }
XI_ORIG _DWORD * sub_100555D0() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x100555D0u))(); }
XI_ORIG void FUN_10055620() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10055620u))(); }
XI_ORIG void FUN_10055640(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10055640u))(a0); }
XI_ORIG void FUN_10055650(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10055650u))(a0); }
XI_ORIG long double FUN_10055ad0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<long double (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10055AD0u))(a0, a1); }
XI_ORIG long double FUN_10056160(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<long double (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10056160u))(a0, a1, a2); }
XI_ORIG char * CMoSphRes_FixupOffsets(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10056220u))(a0); }
XI_ORIG char CMoPointLightProgElem_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10056390u))(a0); }
XI_ORIG void unknown_libname_55() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10056840u))(); }
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
XI_ORIG void FUN_10056eb0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10056EB0u))(a0); }
XI_ORIG undefined8 Scheduler_Kill(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10056EF0u))(a0, a1, a2); }
XI_ORIG void * FUN_10057270(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10057270u))(a0, a1, a2, a3); }
XI_ORIG void FUN_100573b0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100573B0u))(a0); }
XI_ORIG void FUN_100573e0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100573E0u))(a0, a1, a2, a3); }
XI_ORIG char CMoSchedularTask_Destructor(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x100574A0u))(a0); }
XI_ORIG uint32_t FUN_10057c20(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x10057C20u))(a0); }
XI_ORIG void FUN_10057cc0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10057CC0u))(a0, a1); }
XI_ORIG undefined4 CMoSchedularTask_Interpret(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10057FB0u))(a0); }
XI_ORIG void * FUN_1005e100(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1005E100u))(a0); }
XI_ORIG undefined4 FUN_1005ed30(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1005ED30u))(a0, a1, a2); }
XI_ORIG void FUN_1005ed90(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1005ED90u))(a0, a1); }
XI_ORIG void FUN_1005edc0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1005EDC0u))(a0, a1); }
XI_ORIG void FUN_1005ee90(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1005EE90u))(a0, a1); }
XI_ORIG void FUN_1005eec0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1005EEC0u))(a0, a1); }
XI_ORIG undefined4 FUN_1005eef0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1005EEF0u))(a0, a1); }
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
XI_ORIG void FUN_10062830(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10062830u))(a0, a1); }
XI_ORIG void FUN_10062880(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10062880u))(a0, a1); }
XI_ORIG undefined4 FUN_10062de0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10062DE0u))(a0, a1); }
XI_ORIG undefined8 FUN_10062e20(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10062E20u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined8 FUN_10062e80(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10062E80u))(a0, a1, a2, a3); }
XI_ORIG uint32_t FUN_10062ed0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10062ED0u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_10063460(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10063460u))(a0); }
XI_ORIG _DWORD * CMoSpline_Destructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x100634A0u))(a0); }
XI_ORIG void CMoSpline_Destructor_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100634A0u))(a0); }
XI_ORIG void FUN_100634b0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100634B0u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_10063550(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10063550u))(a0); }
XI_ORIG void FUN_100635b0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100635B0u))(a0); }
XI_ORIG long double FUN_10063890(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[2] = {a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = a0.bits, rd = 0; long double st0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "=t"(st0), "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10063890u) : "memory", "cc", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return st0;
}
XI_ORIG void FUN_10063a60(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10063A60u))(a0, a1, a2); }
XI_ORIG void FUN_10063b20(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10063B20u))(a0); }
XI_ORIG int sub_10063B20(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10063B20u))(a0); }
XI_ORIG int * FUN_10063b50(void * a0, XI_ANY a1) { return reinterpret_cast<int * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10063B50u))(a0, a1); }
XI_ORIG _DWORD * sub_10063B50(void * a0, int a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int)>(XI_GAME(0x10063B50u))(a0, a1); }
XI_ORIG int FUN_10063b70(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10063B70u))(a0); }
XI_ORIG undefined4 FUN_10063b80(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10063B80u))(a0); }
XI_ORIG void FUN_10063c30(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10063C30u))(a0, a1, a2); }
XI_ORIG _DWORD * MoProgElem_FreeBuffer40(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10063C60u))(a0); }
XI_ORIG unsigned long long FUN_100683d0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100683D0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_100684f0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100684F0u))(a0, a1); }
XI_ORIG void FUN_10068590(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10068590u))(a0, a1, a2); }
XI_ORIG char CMoDX_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10069D70u))(a0); }
XI_ORIG _BYTE * CMoDX_Initialize(int a0, int a1) { return reinterpret_cast<_BYTE * (__thiscall *)(int, int)>(XI_GAME(0x10069DD0u))(a0, a1); }
XI_ORIG char sub_10069EE0(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10069EE0u))(a0); }
XI_ORIG int sub_1006A060(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(XI_GAME(0x1006A060u))(a0, a1, a2, a3); }
XI_ORIG int sub_1006A070(int a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(int, int, int, int)>(XI_GAME(0x1006A070u))(a0, a1, a2, a3); }
XI_ORIG void FUN_1006a0e0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1006A0E0u))(a0, a1, a2); }
XI_ORIG int sub_1006A0E0(int a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(int, int, int)>(XI_GAME(0x1006A0E0u))(a0, a1, a2); }
XI_ORIG _DWORD * CMoDX_CreateTexture(void * a0, unsigned __int16 a1, unsigned __int16 a2, void * a3) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, unsigned __int16, unsigned __int16, void *)>(XI_GAME(0x1006A150u))(a0, a1, a2, a3); }
XI_ORIG int sub_1006A180(int a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<int (__thiscall *)(int, int, int, int, int)>(XI_GAME(0x1006A180u))(a0, a1, a2, a3, a4); }
XI_ORIG int sub_1006A420(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1006A420u))(a0); }
XI_ORIG void FUN_1006a450(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1006A450u))(a0); }
XI_ORIG int CMoDX_SetWorldMatrix(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x1006A480u))(a0, a1); }
XI_ORIG void CMoDX_SetWorldMatrix_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1006A480u))(a0, a1); }
XI_ORIG int CMoDX_GetWorldMatrix(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x1006A4A0u))(a0, a1); }
XI_ORIG int CMoDX_PushViewMatrix(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1006A4C0u))(a0); }
XI_ORIG int CMoDX_PopViewMatrix(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x1006A4F0u))(a0); }
XI_ORIG void CMoDX_PopViewMatrix_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1006A4F0u))(a0); }
XI_ORIG int CMoDX_SetViewMatrix(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x1006A520u))(a0, a1); }
XI_ORIG void CMoDX_SetViewMatrix_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1006A520u))(a0, a1); }
XI_ORIG void FUN_1006a540(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1006A540u))(a0, a1); }
XI_ORIG int Gfx_CalcFVFStride(unsigned int a0) { return reinterpret_cast<int (__cdecl *)(unsigned int)>(XI_GAME(0x1006A560u))(a0); }
XI_ORIG _DWORD * sub_1006AD30(void * a0, void * a1) { return reinterpret_cast<_DWORD * (__cdecl *)(void *, void *)>(XI_GAME(0x1006AD30u))(a0, a1); }
XI_ORIG int FUN_1006ad50(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1006AD50u))(a0, a1); }
XI_ORIG void FUN_1006ad90(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1006AD90u))(a0); }
XI_ORIG exception * fn_1006AD90(void * a0) { return reinterpret_cast<exception * (__thiscall *)(void *)>(XI_GAME(0x1006AD90u))(a0); }
XI_ORIG int sub_1006ADF0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1006ADF0u))(a0); }
XI_ORIG int StateBlock_ApplyRSTSS(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x1006AE30u))(a0, a1, a2); }
XI_ORIG int StateBlockHolder_ApplyRenderStates(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(XI_GAME(0x1006AF00u))(a0, a1); }
XI_ORIG int StateBlock_ApplyTSS(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1006AF90u))(a0, a1); }
XI_ORIG void StateBlock_ApplyTSS_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1006AF90u))(a0, a1); }
XI_ORIG _DWORD * CMoDxStatusMng_Constructor(int a0) { return reinterpret_cast<_DWORD * (__thiscall *)(int)>(XI_GAME(0x1006B050u))(a0); }
XI_ORIG void CMoDxStatusMng_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1006B090u))(a0); }
XI_ORIG _DWORD * VBPool_Constructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1006B0C0u))(a0); }
XI_ORIG uint32_t FUN_1006b120(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1006B120u))(a0, a1); }
XI_ORIG int sub_1006B1D0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1006B1D0u))(a0, a1); }
XI_ORIG _DWORD * CMoDX_CreateAndInitSystem(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1006B2B0u))(a0); }
XI_ORIG int sub_1006BC10(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x1006BC10u))(a0, a1); }
XI_ORIG int sub_1006BC80(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1006BC80u))(a0); }
XI_ORIG int Rand_SeedMT19937(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x1006BD90u))(a0); }
XI_ORIG void Rand_SeedMT19937_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1006BD90u))(a0); }
XI_ORIG uint32_t FUN_1006be20() { return reinterpret_cast<uint32_t (__cdecl *)()>(XI_GAME(0x1006BE20u))(); }
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
XI_ORIG undefined4 FUN_1006ca20(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1006CA20u))(a0); }
XI_ORIG undefined4 FUN_1006ca60(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1006CA60u))(a0); }
XI_ORIG _DWORD * CreateProcessorInstance() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x1006CB50u))(); }
XI_ORIG void FUN_1006ce40() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x1006CE40u))(); }
XI_ORIG void nullsub_44() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1006CF90u))(); }
XI_ORIG float * CMoProcessor_VT_1032C368_vf012(void * a0, void * a1, float a2) { return reinterpret_cast<float * (__stdcall *)(void *, void *, float)>(XI_GAME(0x1006D3B0u))(a0, a1, a2); }
XI_ORIG void FUN_1006d3e0(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x1006D3E0u))(a0); }
XI_ORIG void CMoProcessor_Vec3Normalize(void * a0) { reinterpret_cast<void (__stdcall *)(void *)>(XI_GAME(0x1006D5D0u))(a0); }
XI_ORIG float * CMoProcessor_Vec4TransformB(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__stdcall *)(void *, void *, void *)>(XI_GAME(0x1006D7E0u))(a0, a1, a2); }
XI_ORIG float * CMoProcessor_Vec4Transform(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__stdcall *)(void *, void *, void *)>(XI_GAME(0x1006D870u))(a0, a1, a2); }
XI_ORIG float * CMoProcessor_Vec3Transform(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__stdcall *)(void *, void *, void *)>(XI_GAME(0x1006D900u))(a0, a1, a2); }
XI_ORIG float * CMoProcessor_VT_1032C368_vf021(void * a0, void * a1, void * a2, float a3) { return reinterpret_cast<float * (__stdcall *)(void *, void *, void *, float)>(XI_GAME(0x1006D980u))(a0, a1, a2, a3); }
XI_ORIG void CMoProcessor_ProjectPointsToScreen(void * a0, void * a1, int a2, int a3, void * a4, void * a5, float a6, float a7) { reinterpret_cast<void (__thiscall *)(void *, void *, int, int, void *, void *, float, float)>(XI_GAME(0x1006E7D0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG CMoProcessor * CMoProcessor_CalcScreenBounds(void * a0, void * a1, int a2, int a3, void * a4) { return reinterpret_cast<CMoProcessor * (__stdcall *)(void *, void *, int, int, void *)>(XI_GAME(0x1006E970u))(a0, a1, a2, a3, a4); }
XI_ORIG float * CMoProcessor_ProjectVertsAndBounds(void * a0, void * a1, int a2, void * a3, void * a4, float a5, float a6) { return reinterpret_cast<float * (__stdcall *)(void *, void *, int, void *, void *, float, float)>(XI_GAME(0x1006EB70u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG CMoProcessor * CMoProcessor_CalcScreenBoundsAndVisible(void * a0, void * a1, void * a2, int a3, int a4, int a5, float a6, float a7) { return reinterpret_cast<CMoProcessor * (__thiscall *)(void *, void *, void *, int, int, int, float, float)>(XI_GAME(0x1006ED10u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG void * FUN_10070f40(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10070F40u))(a0); }
XI_ORIG char CMoTask_VT_1032C490_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10070F80u))(a0); }
XI_ORIG void FUN_10070f90(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10070F90u))(a0, a1, a2, a3, a4); }
XI_ORIG __int16 ResHandle_FreeIfUnreferenced(void * a0) { return reinterpret_cast<__int16 (__thiscall *)(void *)>(XI_GAME(0x10071050u))(a0); }
XI_ORIG undefined4 ResHandle_FreeIfUnreferenced_v1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10071050u))(a0); }
XI_ORIG uint8_t ResourceHandle_IsResident(void * a0) { return reinterpret_cast<uint8_t (__fastcall *)(void *)>(XI_GAME(0x10071060u))(a0); }
XI_ORIG undefined4 ResourceHandle_IsResident_v1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10071060u))(a0); }
XI_ORIG undefined4 FUN_100710f0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100710F0u))(a0, a1, a2, a3, a4); }
XI_ORIG _DWORD * CMoAttachments_VT_1032C4E8_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(XI_GAME(0x10071180u))(a0, a1); }
XI_ORIG void FUN_100711d0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100711D0u))(a0, a1); }
XI_ORIG void FUN_100711e0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100711E0u))(a0, a1); }
XI_ORIG void FUN_10071200(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10071200u))(a0, a1); }
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
XI_ORIG _DWORD * sub_10071F50(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10071F50u))(a0); }
XI_ORIG void FUN_10071f60(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10071F60u))(a0); }
XI_ORIG void FUN_10071f70(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10071F70u))(a0, a1); }
XI_ORIG undefined4 FUN_10071f80(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10071F80u))(a0); }
XI_ORIG int CMoResource_RegisterInTypeList(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10072030u))(a0); }
XI_ORIG void CMoResource_RegisterInTypeList_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10072030u))(a0); }
XI_ORIG void FUN_100720b0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100720B0u))(a0); }
XI_ORIG undefined4 FUN_10072180(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10072180u))(a0, a1); }
XI_ORIG __int16 CMoResource_Destroy(int a0) { return reinterpret_cast<__int16 (__thiscall *)(int)>(XI_GAME(0x100721F0u))(a0); }
XI_ORIG unsigned long long CMoResource_Destroy_v1(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100721F0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void * FUN_10072240(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10072240u))(a0); }
XI_ORIG _DWORD * CMoResource_GetParent(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10072270u))(a0); }
XI_ORIG void * CMoResource_GetParent_v1(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10072270u))(a0); }
XI_ORIG _DWORD * CMoResource_FindByTypeAndId(void * a0, void * a1, int a2, int a3) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, int, int)>(XI_GAME(0x10072290u))(a0, a1, a2, a3); }
XI_ORIG undefined8 CMoResource_FindByTypeAndId_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10072290u))(a0, a1, a2, a3); }
XI_ORIG undefined8 FUN_100722c0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100722C0u))(a0, a1, a2, a3, a4); }
XI_ORIG _DWORD * CMoResource_FindInScope(void * a0, int a1, int a2, int a3) { return reinterpret_cast<_DWORD * (__stdcall *)(void *, int, int, int)>(XI_GAME(0x10072430u))(a0, a1, a2, a3); }
XI_ORIG unsigned long long CMoResource_FindInScope_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) {
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
XI_ORIG unsigned long long FUN_10072540(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) {
    uint32_t xs[4] = {a1.bits, a2.bits, a3.bits, a4.bits}; uint32_t ra = a5.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 12(%%esi)\n\t""pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10072540u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_10072970() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x10072970u))(); }
XI_ORIG int unknown_libname_61() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100729C0u))(); }
XI_ORIG void FUN_100729d0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100729D0u))(); }
XI_ORIG char CMoTask_VT_1032C908_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100729E0u))(a0); }
XI_ORIG undefined8 FUN_10072c30(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10072C30u))(a0, a1, a2); }
XI_ORIG int sub_10072DB0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10072DB0u))(a0); }
XI_ORIG int * MeshResourceMng_LoadSystemResources(int a0, int a1) { return reinterpret_cast<int * (__thiscall *)(int, int)>(XI_GAME(0x10072ED0u))(a0, a1); }
XI_ORIG int ResourceHandle_Deref(int a0) { return reinterpret_cast<int (__stdcall *)(int)>(XI_GAME(0x10072FB0u))(a0); }
XI_ORIG undefined4 ResourceHandle_Deref_v1(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x10072FB0u))(a0); }
XI_ORIG undefined4 FUN_10072fd0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10072FD0u))(a0, a1, a2); }
XI_ORIG int * sub_10072FD0(void * a0, void * a1, int a2) { return reinterpret_cast<int * (__thiscall *)(void *, void *, int)>(XI_GAME(0x10072FD0u))(a0, a1, a2); }
XI_ORIG undefined4 * FUN_100730e0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 * (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x100730E0u))(a0, a1); }
XI_ORIG undefined4 FUN_100731b0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x100731B0u))(a0, a1, a2); }
XI_ORIG int * sub_100731B0(void * a0, void * a1, int a2) { return reinterpret_cast<int * (__thiscall *)(void *, void *, int)>(XI_GAME(0x100731B0u))(a0, a1, a2); }
XI_ORIG undefined4 MeshResourceMng_ResolveTree(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10073430u))(a0, a1, a2, a3); }
XI_ORIG void FUN_10073a60(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x10073A60u))(a0, a1); }
XI_ORIG __int16 MoRes_FreeIfUnreferenced(void * a0) { return reinterpret_cast<__int16 (__stdcall *)(void *)>(XI_GAME(0x10073B20u))(a0); }
XI_ORIG undefined4 MoRes_FreeIfUnreferenced_v1(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x10073B20u))(a0); }
XI_ORIG undefined4 FUN_10073b80(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x10073B80u))(a0); }
XI_ORIG undefined4 FUN_10073ba0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10073BA0u))(a0, a1, a2); }
XI_ORIG undefined8 FUN_10073d80(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__stdcall *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10073D80u))(a0, a1, a2); }
XI_ORIG undefined8 FUN_10074110(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10074110u))(a0, a1); }
XI_ORIG _DWORD * MeshResourceMng_AllocHandle(void * a0, void * a1, int a2) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, int)>(XI_GAME(0x10074140u))(a0, a1, a2); }
XI_ORIG void MeshResourceMng_AllocHandle_v1(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10074140u))(a0, a1, a2); }
XI_ORIG unsigned int * NodePool_ReleaseNode(void * a0, void * a1) { return reinterpret_cast<unsigned int * (__thiscall *)(void *, void *)>(XI_GAME(0x10074180u))(a0, a1); }
XI_ORIG void NodePool_ReleaseNode_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10074180u))(a0, a1); }
XI_ORIG undefined4 FUN_100741e0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100741E0u))(a0, a1); }
XI_ORIG void FUN_10074280(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10074280u))(a0); }
XI_ORIG int GetGlobal_1047D114() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10074290u))(); }
XI_ORIG void FUN_100742a0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100742A0u))(a0); }
XI_ORIG char ResourceIter_FindFirst(void * a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int, int)>(XI_GAME(0x100742B0u))(a0, a1, a2, a3, a4); }
XI_ORIG void ResourceIter_FindFirst_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100742B0u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_10074310(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10074310u))(a0, a1, a2, a3, a4); }
XI_ORIG uint32_t FUN_10074380(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x10074380u))(a0); }
XI_ORIG void FUN_100743d0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100743D0u))(a0); }
XI_ORIG int ResourceIter_FindNext(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x100745C0u))(a0, a1); }
XI_ORIG undefined4 ResourceIter_FindNext_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100745C0u))(a0, a1); }
XI_ORIG int MoProcessor_CallVSlot49(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100746A0u))(a0); }
XI_ORIG void MoProcessor_CallVSlot49_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100746A0u))(a0); }
XI_ORIG _DWORD * CMoTask_Constructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10074740u))(a0); }
XI_ORIG undefined8 CMoTask_Constructor_v1(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x10074740u))(a0); }
XI_ORIG char CMoTask_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100747C0u))(a0); }
XI_ORIG void CMoTask_Destructor_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100747C0u))(a0); }
XI_ORIG void FUN_100747f0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100747F0u))(a0, a1); }
XI_ORIG void FUN_10074850(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10074850u))(a0, a1); }
XI_ORIG void FUN_10074890(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10074890u))(a0); }
XI_ORIG int CMoTask_ResetFields(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x100748F0u))(a0); }
XI_ORIG undefined4 CMoTask_ResetFields_v1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100748F0u))(a0); }
XI_ORIG int CMoTask_RegisterWithMng(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x100749B0u))(a0, a1); }
XI_ORIG undefined4 CMoTask_RegisterWithMng_v1(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x100749B0u))(a0); }
XI_ORIG undefined8 FUN_100749f0() { return reinterpret_cast<undefined8 (__cdecl *)()>(XI_GAME(0x100749F0u))(); }
XI_ORIG undefined4 FUN_10074a00() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10074A00u))(); }
XI_ORIG void FUN_10074a00_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10074A00u))(); }
XI_ORIG int CMoTaskMng_Forward10075120(int a0) { return reinterpret_cast<int (__stdcall *)(int)>(XI_GAME(0x10074A10u))(a0); }
XI_ORIG void FUN_10074a20() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10074A20u))(); }
XI_ORIG unsigned long long FUN_10074a50(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10074A50u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_10074ab0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10074AB0u))(a0); }
XI_ORIG void FUN_10074ad0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10074AD0u))(a0, a1); }
XI_ORIG char CMoTaskMng_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10074B60u))(a0); }
XI_ORIG int sub_10074BA0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10074BA0u))(a0); }
XI_ORIG int sub_10074C30(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10074C30u))(a0); }
XI_ORIG void FUN_10074cc0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10074CC0u))(a0, a1, a2); }
XI_ORIG void FUN_10074cf0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10074CF0u))(a0, a1, a2); }
XI_ORIG int FUN_10074df0(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10074DF0u))(a0, a1); }
XI_ORIG undefined8 FUN_10074f30(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10074F30u))(a0, a1, a2); }
XI_ORIG undefined8 FUN_10074f80(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10074F80u))(a0, a1, a2); }
XI_ORIG void FUN_10075000() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10075000u))(); }
XI_ORIG int FUN_10075120(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10075120u))(a0, a1); }
XI_ORIG void FUN_100751b0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100751B0u))(a0, a1); }
XI_ORIG int Gfx_InitCubeMapTextures() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10075280u))(); }
XI_ORIG int TexRef_ResolveByName(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10075520u))(a0, a1); }
XI_ORIG void TexRef_EnsureRenderTarget(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100755A0u))(a0); }
XI_ORIG int TextureHolder_BindWarped(void * a0, int a1, float a2, float a3, float a4) { return reinterpret_cast<int (__thiscall *)(void *, int, float, float, float)>(XI_GAME(0x10075700u))(a0, a1, a2, a3, a4); }
XI_ORIG void unknown_libname_63() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10075950u))(); }
XI_ORIG int TexRef_SetTexture(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10075960u))(a0, a1); }
XI_ORIG void TexRef_SetTexture_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10075960u))(a0, a1); }
XI_ORIG void TextureHolder_BindTinted(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x100759C0u))(a0, a1, a2); }
XI_ORIG void unknown_libname_64() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10075C30u))(); }
XI_ORIG void FUN_10076750(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10076750u))(a0, a1); }
XI_ORIG int FUN_100769f0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100769F0u))(a0, a1, a2); }
XI_ORIG char * FUN_10076a70(XI_ANY a0) { return reinterpret_cast<char * (__cdecl *)(XI_ANY)>(XI_GAME(0x10076A70u))(a0); }
XI_ORIG undefined1 * getNounFromPronoun(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined1 * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10076FF0u))(a0, a1); }
XI_ORIG unsigned long long FUN_10077a00(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10077A00u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG uint8_t * FUN_10077ad0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<uint8_t * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10077AD0u))(a0, a1); }
XI_ORIG undefined4 FUN_10077b40(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10077B40u))(a0, a1); }
XI_ORIG undefined4 getLegalPCname(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10077C30u))(a0); }
XI_ORIG void FUN_10077d70() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10077D70u))(); }
XI_ORIG int FUN_10077ed0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10077ED0u))(); }
XI_ORIG void FUN_10077fb0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10077FB0u))(); }
XI_ORIG undefined thunk_FUN_100e97d0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10078090u))(); }
XI_ORIG undefined4 FUN_100780a0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100780A0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_100780d0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100780D0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG int FUN_10079170(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x10079170u))(a0); }
XI_ORIG unsigned long long SeekIdByString(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) {
    uint32_t xs[4] = {a0.bits, a1.bits, a2.bits, a3.bits}; uint32_t ra = a4.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 12(%%esi)\n\t""pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $16, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1007A1C0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_1007bf30(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1007BF30u))(a0, a1, a2, a3, a4); }
XI_ORIG int FUN_1007cd60(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1007CD60u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_1007cf80() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1007CF80u))(); }
XI_ORIG undefined4 FUN_10080c70(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10080C70u))(a0); }
XI_ORIG unsigned long long FUN_10080d00(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10080D00u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_10080d20(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10080D20u))(a0); }
XI_ORIG uint32_t FUN_10080d90(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x10080D90u))(a0); }
XI_ORIG int CommandCalc(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10080DC0u))(a0, a1); }
XI_ORIG void FUN_10081440(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10081440u))(a0); }
XI_ORIG undefined4 FUN_10081480(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10081480u))(a0); }
XI_ORIG _DWORD * ActorHandle_Constructor(int a0) { return reinterpret_cast<_DWORD * (__thiscall *)(int)>(XI_GAME(0x100814A0u))(a0); }
XI_ORIG void ActorHandle_Constructor_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100814A0u))(a0); }
XI_ORIG void ActorHandle_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100814E0u))(a0); }
XI_ORIG void ActorHandle_Destructor_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100814E0u))(a0); }
XI_ORIG undefined8 FUN_100814f0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100814F0u))(a0, a1); }
XI_ORIG unsigned int sub_100814F0(int a0, int a1) { return reinterpret_cast<unsigned int (__thiscall *)(int, int)>(XI_GAME(0x100814F0u))(a0, a1); }
XI_ORIG undefined4 FUN_10081530(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10081530u))(a0, a1, a2); }
XI_ORIG int ActorHandle_Resolve(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10081550u))(a0); }
XI_ORIG undefined8 ActorHandle_Resolve_v1(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x10081550u))(a0); }
XI_ORIG int sub_10081600(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10081600u))(a0); }
XI_ORIG void FUN_10081a30() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10081A30u))(); }
XI_ORIG void Actor_ResetDrawLists() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10081A50u))(); }
XI_ORIG unsigned long long FUN_10081ae0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10081AE0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void CXiActor_Init(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10081BA0u))(a0); }
XI_ORIG void * FUN_10081cf0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10081CF0u))(a0); }
XI_ORIG int Actor_UnlinkAllAttachLists(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10082B10u))(a0); }
XI_ORIG int FUN_10082bf0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10082BF0u))(); }
XI_ORIG void CXiActorDraw_UpdateDrawList() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10082D70u))(); }
XI_ORIG undefined4 FUN_10082fa0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10082FA0u))(a0); }
XI_ORIG undefined4 FUN_10082fb0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x10082FB0u))(a0, a1); }
XI_ORIG char CXiActor_NameCalc(void * a0, void * a1, int a2, void * a3, int a4) { return reinterpret_cast<char (__thiscall *)(void *, void *, int, void *, int)>(XI_GAME(0x100830C0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_10083430(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10083430u))(a0, a1, a2, a3, a4); }
XI_ORIG int * FUN_10083620(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10083620u))(a0, a1); }
XI_ORIG int * FUN_10083750(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10083750u))(a0, a1, a2); }
XI_ORIG int * FUN_10083b40(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10083B40u))(a0, a1, a2); }
XI_ORIG int * FUN_10083d00(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10083D00u))(a0, a1, a2); }
XI_ORIG int * FUN_10083f20(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10083F20u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10084100(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10084100u))(a0, a1, a2); }
XI_ORIG char CXiActor_IsModelMissingOrFlag2000(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10084140u))(a0); }
XI_ORIG uint32_t CXiActor_IsModelMissingOrFlag2000_v1(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x10084140u))(a0); }
XI_ORIG uint8_t CXiActor_ModelTest95790(void * a0, int a1) { return reinterpret_cast<uint8_t (__thiscall *)(void *, int)>(XI_GAME(0x10084330u))(a0, a1); }
XI_ORIG uint8_t CXiActor_ModelTest95680(void * a0, int a1) { return reinterpret_cast<uint8_t (__thiscall *)(void *, int)>(XI_GAME(0x10084350u))(a0, a1); }
XI_ORIG unsigned long long CXiActor_ModelTest95680_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
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
XI_ORIG int CXiActor_GetModelByteEF_v1(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10084420u))(a0); }
XI_ORIG int CXiActor_GetModelPartWord(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10084480u))(a0, a1); }
XI_ORIG uint32_t CXiActor_GetModelPartWord_v1(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10084480u))(a0, a1); }
XI_ORIG int CXiActor_GetModelFlags120Top2(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10084500u))(a0); }
XI_ORIG int CXiActor_GetModelFlags130Top2(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10084520u))(a0); }
XI_ORIG double CXiActor_GetModelFloat9AEC0(void * a0) { return reinterpret_cast<double (__thiscall *)(void *)>(XI_GAME(0x10084540u))(a0); }
XI_ORIG BOOL CXiActor_IsModelMissingOrSuppressed(int a0) { return reinterpret_cast<BOOL (__thiscall *)(int)>(XI_GAME(0x10084580u))(a0); }
XI_ORIG uint32_t CXiActor_IsModelMissingOrSuppressed_v1(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x10084580u))(a0); }
XI_ORIG BOOL CXiActor_TestModelFlag128_8(void * a0) { return reinterpret_cast<BOOL (__thiscall *)(void *)>(XI_GAME(0x100845B0u))(a0); }
XI_ORIG int sub_100845E0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100845E0u))(a0); }
XI_ORIG uint32_t FUN_10084620(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x10084620u))(a0); }
XI_ORIG BOOL CXiActor_TestModelFlags100000_200000_4000(void * a0) { return reinterpret_cast<BOOL (__thiscall *)(void *)>(XI_GAME(0x10084640u))(a0); }
XI_ORIG int CXiActor_GetModelByte1DB(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10084720u))(a0); }
XI_ORIG void CXiActor_set_field_B4(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10085010u))(a0, a1); }
XI_ORIG int CXiActor_SpawnResourceGenerators(int a0, void * a1) { return reinterpret_cast<int (__thiscall *)(int, void *)>(XI_GAME(0x10085060u))(a0, a1); }
XI_ORIG void CXiActor_SpawnResourceGenerators_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10085060u))(a0, a1); }
XI_ORIG undefined8 FUN_10085240() { return reinterpret_cast<undefined8 (__cdecl *)()>(XI_GAME(0x10085240u))(); }
XI_ORIG undefined8 FUN_10085270(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x10085270u))(a0); }
XI_ORIG void FUN_100852d0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100852D0u))(); }
XI_ORIG uint8_t CXiActor_IsInModelSlotTable(void * a0) { return reinterpret_cast<uint8_t (__thiscall *)(void *)>(XI_GAME(0x10085490u))(a0); }
XI_ORIG int CXiActor_IsInModelSlotTable_v1(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10085490u))(a0); }
XI_ORIG uint32_t FUN_10085500(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x10085500u))(a0); }
XI_ORIG void FUN_10085d50(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10085D50u))(a0, a1); }
XI_ORIG undefined4 FUN_10085d60(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10085D60u))(a0, a1); }
XI_ORIG void Actor_DrawWorldText(void * a0, void * a1, float a2, unsigned int a3, int a4) { reinterpret_cast<void (__cdecl *)(void *, void *, float, unsigned int, int)>(XI_GAME(0x10086210u))(a0, a1, a2, a3, a4); }
XI_ORIG void Actor_DrawWorldText_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10086210u))(a0, a1, a2, a3, a4); }
XI_ORIG void DrawNameplateSecondString(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10086900u))(a0, a1, a2); }
XI_ORIG unsigned long long FUN_10086fc0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10086FC0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined8 FUN_10086fe0(XI_ANY a0) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY)>(XI_GAME(0x10086FE0u))(a0); }
XI_ORIG uint32_t FUN_10087010() { return reinterpret_cast<uint32_t (__cdecl *)()>(XI_GAME(0x10087010u))(); }
XI_ORIG unsigned long long FUN_10087060(XI_ANY a0) {
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
XI_ORIG undefined4 FUN_100870e0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100870E0u))(a0, a1); }
XI_ORIG unsigned long long FUN_10087110(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10087110u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined thunk_FUN_10087230() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10087220u))(); }
XI_ORIG undefined4 FUN_100872c0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100872C0u))(); }
XI_ORIG unsigned long long FUN_100873b0(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100873B0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG uint32_t FUN_100875d0() { return reinterpret_cast<uint32_t (__cdecl *)()>(XI_GAME(0x100875D0u))(); }
XI_ORIG unsigned long long FUN_10087660(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10087660u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_10087670() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10087670u))(); }
XI_ORIG unsigned long long FUN_10087680(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10087680u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_10087690(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10087690u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_100876b0(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100876B0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_10087760(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10087760u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_10087830() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10087830u))(); }
XI_ORIG void FUN_10087d40(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10087D40u))(a0); }
XI_ORIG uint32_t FUN_10087dc0(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x10087DC0u))(a0); }
XI_ORIG void FUN_10087df0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10087DF0u))(a0, a1); }
XI_ORIG undefined4 FUN_10087e40(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10087E40u))(a0); }
XI_ORIG void FUN_10087e80(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10087E80u))(a0); }
XI_ORIG void FUN_10087f00(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10087F00u))(a0); }
XI_ORIG undefined4 FUN_10088270(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10088270u))(a0, a1); }
XI_ORIG void FUN_100882d0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100882D0u))(); }
XI_ORIG void FUN_100883e0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100883E0u))(a0); }
XI_ORIG void FUN_10089100(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10089100u))(a0); }
XI_ORIG undefined4 FUN_1008a0e0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1008A0E0u))(a0); }
XI_ORIG undefined4 FUN_1008a120(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1008A120u))(a0); }
XI_ORIG void FUN_1008a160(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008A160u))(a0); }
XI_ORIG undefined4 FUN_1008a1a0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1008A1A0u))(a0); }
XI_ORIG undefined4 FUN_1008a2f0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1008A2F0u))(a0); }
XI_ORIG undefined4 FUN_1008a350(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1008A350u))(a0, a1); }
XI_ORIG void FUN_1008a7b0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008A7B0u))(a0); }
XI_ORIG int FUN_1008a930(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x1008A930u))(a0); }
XI_ORIG void FUN_1008a9e0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1008A9E0u))(a0, a1); }
XI_ORIG void FUN_1008aa30(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1008AA30u))(a0, a1); }
XI_ORIG undefined8 FUN_1008abe0() { return reinterpret_cast<undefined8 (__cdecl *)()>(XI_GAME(0x1008ABE0u))(); }
XI_ORIG void FUN_1008af60(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1008AF60u))(a0, a1, a2); }
XI_ORIG void FUN_1008b100(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008B100u))(a0); }
XI_ORIG void FUN_1008b170(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008B170u))(a0); }
XI_ORIG int FUN_1008b270(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1008B270u))(a0, a1, a2); }
XI_ORIG uint32_t FUN_1008b2b0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1008B2B0u))(a0, a1, a2); }
XI_ORIG int FUN_1008b380(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1008B380u))(a0, a1); }
XI_ORIG undefined4 FUN_1008b410(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1008B410u))(a0); }
XI_ORIG undefined4 FUN_1008b460(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1008B460u))(a0, a1); }
XI_ORIG void FUN_1008c480(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008C480u))(a0); }
XI_ORIG undefined4 FUN_1008c6c0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1008C6C0u))(a0, a1); }
XI_ORIG unsigned long long FUN_1008cce0(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1008CCE0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_1008cd20(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1008CD20u))(a0, a1); }
XI_ORIG void FUN_1008cd90(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008CD90u))(a0); }
XI_ORIG undefined4 FUN_1008d910(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1008D910u))(a0, a1); }
XI_ORIG void FUN_1008d980(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008D980u))(a0); }
XI_ORIG void FUN_1008da00(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008DA00u))(a0); }
XI_ORIG void FUN_1008db40(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008DB40u))(a0); }
XI_ORIG unsigned long long FUN_1008e130(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1008E130u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_1008e140(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1008E140u))(a0); }
XI_ORIG undefined4 FUN_1008e150(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1008E150u))(a0); }
XI_ORIG undefined4 FUN_1008e260(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1008E260u))(a0); }
XI_ORIG void FUN_1008e290(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008E290u))(a0); }
XI_ORIG void FUN_1008e3f0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008E3F0u))(a0); }
XI_ORIG void FUN_1008e4d0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008E4D0u))(a0); }
XI_ORIG void FUN_1008e740(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008E740u))(a0); }
XI_ORIG void FUN_1008e870(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008E870u))(a0); }
XI_ORIG uint32_t FUN_1008e8d0(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1008E8D0u))(a0, a1); }
XI_ORIG undefined4 FUN_1008e9a0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1008E9A0u))(a0); }
XI_ORIG void FUN_1008ee30(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008EE30u))(a0); }
XI_ORIG void FUN_1008f1d0() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x1008F1D0u))(); }
XI_ORIG void FUN_1008f740(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1008F740u))(a0); }
XI_ORIG void FUN_100926c0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100926C0u))(a0); }
XI_ORIG void FUN_10092710(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10092710u))(a0); }
XI_ORIG void FUN_10092760(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10092760u))(a0); }
XI_ORIG void FUN_10092780(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10092780u))(a0, a1); }
XI_ORIG void FUN_10092800(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10092800u))(a0); }
XI_ORIG void FUN_10092870(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10092870u))(a0); }
XI_ORIG void FUN_10092900(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10092900u))(a0); }
XI_ORIG void FUN_100929a0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100929A0u))(a0); }
XI_ORIG void FUN_100929d0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100929D0u))(a0); }
XI_ORIG void FUN_10092c20(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10092C20u))(a0); }
XI_ORIG undefined4 FUN_10093430(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10093430u))(a0, a1); }
XI_ORIG void FUN_100935c0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100935C0u))(a0, a1, a2, a3, a4); }
XI_ORIG char Unit10086E90_ResetGlobals() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x10093720u))(); }
XI_ORIG void FUN_10093a30() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10093A30u))(); }
XI_ORIG void FUN_10093a90() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10093A90u))(); }
XI_ORIG void FUN_10093ad0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10093AD0u))(); }
XI_ORIG void FUN_10093af0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10093AF0u))(); }
XI_ORIG void FUN_10093b10() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10093B10u))(); }
XI_ORIG void FUN_10093b30() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10093B30u))(); }
XI_ORIG void FUN_10093b50() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10093B50u))(); }
XI_ORIG undefined4 FUN_10093b70() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10093B70u))(); }
XI_ORIG void FUN_10093da0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10093DA0u))(); }
XI_ORIG uint8_t Net_SendPacket00COnce() { return reinterpret_cast<uint8_t (__cdecl *)()>(XI_GAME(0x10093DB0u))(); }
XI_ORIG undefined4 Net_SendPacket00COnce_v1() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10093DB0u))(); }
XI_ORIG void FUN_10093e30() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10093E30u))(); }
XI_ORIG void FUN_10093e50() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10093E50u))(); }
XI_ORIG undefined4 FUN_10094190() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10094190u))(); }
XI_ORIG void FUN_10095660() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10095660u))(); }
XI_ORIG undefined4 FUN_10095680(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10095680u))(a0, a1); }
XI_ORIG undefined4 FUN_100956a0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100956A0u))(a0, a1); }
XI_ORIG undefined4 FUN_100956c0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100956C0u))(a0, a1); }
XI_ORIG undefined4 FUN_100956f0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100956F0u))(a0, a1); }
XI_ORIG undefined4 FUN_10095750(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10095750u))(a0, a1); }
XI_ORIG int FUN_10095790(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10095790u))(a0, a1); }
XI_ORIG uint32_t FUN_10095850(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x10095850u))(a0); }
XI_ORIG void FUN_10095900() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10095900u))(); }
XI_ORIG int FUN_10095d80() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10095D80u))(); }
XI_ORIG void FUN_10095da0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10095DA0u))(a0); }
XI_ORIG void FUN_10095dd0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10095DD0u))(a0); }
XI_ORIG void FUN_10095e30(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10095E30u))(a0); }
XI_ORIG void FUN_10095e90(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10095E90u))(a0); }
XI_ORIG uint32_t FUN_10096040(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x10096040u))(a0); }
XI_ORIG undefined4 FUN_100960f0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100960F0u))(a0, a1); }
XI_ORIG uint32_t FUN_100961d0(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x100961D0u))(a0); }
XI_ORIG undefined4 FUN_10096200(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10096200u))(a0); }
XI_ORIG undefined4 FUN_10096640() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10096640u))(); }
XI_ORIG void FUN_10096c90(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10096C90u))(a0, a1); }
XI_ORIG void FUN_100973b0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100973B0u))(a0); }
XI_ORIG undefined8 FUN_10097500(void * a0, void * a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined8 (__fastcall *)(void *, void *, XI_ANY, XI_ANY)>(XI_GAME(0x10097500u))(a0, a1, a2, a3); }
XI_ORIG uint32_t FUN_10097730(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10097730u))(a0, a1); }
XI_ORIG undefined8 FUN_100977a0(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x100977A0u))(a0); }
XI_ORIG undefined8 FUN_100977f0(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x100977F0u))(a0); }
XI_ORIG char * Entity_BuildDisplayName(int a0) { return reinterpret_cast<char * (__thiscall *)(int)>(XI_GAME(0x10097840u))(a0); }
XI_ORIG undefined1 * FUN_100979b0(void * a0) { return reinterpret_cast<undefined1 * (__thiscall *)(void *)>(XI_GAME(0x100979B0u))(a0); }
XI_ORIG void FUN_10097a20(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10097A20u))(a0, a1); }
XI_ORIG void FUN_10097af0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10097AF0u))(); }
XI_ORIG undefined4 FUN_10097d50() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10097D50u))(); }
XI_ORIG undefined4 FUN_10097e50(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10097E50u))(a0, a1); }
XI_ORIG undefined4 FUN_10097fd0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10097FD0u))(a0, a1); }
XI_ORIG undefined4 FUN_10098180(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10098180u))(a0); }
XI_ORIG undefined4 FUN_10098360() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x10098360u))(); }
XI_ORIG uint32_t FUN_100983a0(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x100983A0u))(a0); }
XI_ORIG void FUN_100983f0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100983F0u))(a0); }
XI_ORIG unsigned long long FUN_100991f0(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100991F0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_10099730() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10099730u))(); }
XI_ORIG void FUN_100997b0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100997B0u))(); }
XI_ORIG unsigned long long FUN_10099a40(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[2] = {a0.bits, a1.bits}; uint32_t ra = a2.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $8, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10099A40u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_1009aad0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1009AAD0u))(a0); }
XI_ORIG undefined4 FUN_1009aaf0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1009AAF0u))(a0); }
XI_ORIG void FUN_1009ab40(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1009AB40u))(a0, a1); }
XI_ORIG unsigned long long FUN_1009acb0(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1009ACB0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_1009ace0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1009ACE0u))(a0, a1, a2); }
XI_ORIG void FUN_1009ad20(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1009AD20u))(a0, a1, a2, a3); }
XI_ORIG void FUN_1009ade0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1009ADE0u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG long double FUN_1009aec0(void * a0) { return reinterpret_cast<long double (__thiscall *)(void *)>(XI_GAME(0x1009AEC0u))(a0); }
XI_ORIG void FUN_1009af60(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1009AF60u))(a0, a1, a2, a3); }
XI_ORIG void FUN_1009afe0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1009AFE0u))(a0, a1, a2); }
XI_ORIG void FUN_1009f2f0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1009F2F0u))(); }
XI_ORIG void FUN_1009f4c0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1009F4C0u))(a0); }
XI_ORIG undefined4 FUN_100a2720() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100A2720u))(); }
XI_ORIG char CMoTask_VT_1032D6D4_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100A2810u))(a0); }
XI_ORIG void FUN_100a3cd0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100A3CD0u))(); }
XI_ORIG void FUN_100a3d00() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100A3D00u))(); }
XI_ORIG _DWORD * CXiCollisionActor_Constructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x100A4C20u))(a0); }
XI_ORIG void FUN_100a5770(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100A5770u))(a0, a1); }
XI_ORIG undefined4 FUN_100a57c0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100A57C0u))(a0, a1); }
XI_ORIG undefined4 FUN_100a57f0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100A57F0u))(a0); }
XI_ORIG void FUN_100a5e10(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100A5E10u))(a0); }
XI_ORIG undefined4 FUN_100a5ef0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100A5EF0u))(a0); }
XI_ORIG unsigned long long FUN_100a6070(XI_ANY a0, XI_ANY a1) {
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
XI_ORIG void FUN_100ab550(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100AB550u))(a0); }
XI_ORIG void FUN_100ab750(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100AB750u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_100ab830(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100AB830u))(a0, a1); }
XI_ORIG unsigned long long FUN_100ab880(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100AB880u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_100ab9a0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100AB9A0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_100abc70(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100ABC70u))(a0, a1); }
XI_ORIG undefined4 FUN_100ae3f0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100AE3F0u))(); }
XI_ORIG void FUN_100ae960() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100AE960u))(); }
XI_ORIG undefined4 FUN_100aea40() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100AEA40u))(); }
XI_ORIG undefined4 FUN_100aec70(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100AEC70u))(a0); }
XI_ORIG void FUN_100aed00(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100AED00u))(a0, a1, a2, a3); }
XI_ORIG int FUN_100b37a0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100B37A0u))(a0); }
XI_ORIG void XiEvent_EventIdle(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100BCD10u))(a0); }
XI_ORIG undefined4 FUN_100bd0e0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100BD0E0u))(a0); }
XI_ORIG void FUN_100bd5f0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100BD5F0u))(a0); }
XI_ORIG void * FUN_100bd820(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x100BD820u))(a0); }
XI_ORIG char * FUN_100c2d00(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<char * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100C2D00u))(a0, a1); }
XI_ORIG void * FUN_100c2e20(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100C2E20u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG undefined4 FUN_100c2fb0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100C2FB0u))(a0); }
XI_ORIG int FUN_100c2fe0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100C2FE0u))(a0); }
XI_ORIG unsigned long long FUN_100c3020(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100C3020u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_100c3030(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100C3030u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_100c3050(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100C3050u))(a0); }
XI_ORIG void * FUN_100c3060(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100C3060u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG void * FUN_100c30f0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x100C30F0u))(a0); }
XI_ORIG int FUN_100c3100(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100C3100u))(a0); }
XI_ORIG undefined4 FUN_100c3120(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100C3120u))(a0); }
XI_ORIG undefined4 FUN_100c3130(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100C3130u))(a0); }
XI_ORIG undefined4 FUN_100c3140(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100C3140u))(a0); }
XI_ORIG undefined4 FUN_100c3150(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100C3150u))(a0); }
XI_ORIG undefined4 FUN_100c3160(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100C3160u))(a0); }
XI_ORIG void FUN_100c3170(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100C3170u))(a0); }
XI_ORIG undefined * FUN_100c3190() { return reinterpret_cast<undefined * (__cdecl *)()>(XI_GAME(0x100C3190u))(); }
XI_ORIG void FUN_100c31f0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100C31F0u))(a0); }
XI_ORIG void FUN_100c3200(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100C3200u))(a0); }
XI_ORIG undefined4 FUN_100c3210(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100C3210u))(a0); }
XI_ORIG undefined4 FUN_100c3220(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100C3220u))(a0); }
XI_ORIG void FUN_100c3260(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100C3260u))(a0, a1); }
XI_ORIG void FUN_100c3300(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100C3300u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG void FUN_100c3e70(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100C3E70u))(a0); }
XI_ORIG undefined4 FUN_100c4400(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100C4400u))(a0); }
XI_ORIG undefined4 FUN_100c4bc0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x100C4BC0u))(a0, a1, a2); }
XI_ORIG int FUN_100c51c0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100C51C0u))(a0, a1); }
XI_ORIG int FUN_100c5260(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x100C5260u))(a0); }
XI_ORIG int FUN_100c5270(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x100C5270u))(a0); }
XI_ORIG int FUN_100c5280(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x100C5280u))(a0); }
XI_ORIG int thunk_FUN_101cce00(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x100C5290u))(a0); }
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
XI_ORIG void FUN_100cd4e0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100CD4E0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG void FUN_100ce0e0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100CE0E0u))(a0); }
XI_ORIG void FUN_100ce160(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100CE160u))(a0); }
XI_ORIG _DWORD * CXiSkeletonActor_ReverseFindRes(void * a0, void * a1, int a2, int a3, int a4, int a5, char a6) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, int, int, int, int, char)>(XI_GAME(0x100CE480u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG void * FUN_100cf750(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100CF750u))(a0, a1); }
XI_ORIG uint16_t * FUN_100cfb40(void * a0) { return reinterpret_cast<uint16_t * (__thiscall *)(void *)>(XI_GAME(0x100CFB40u))(a0); }
XI_ORIG undefined4 FUN_100d0b50(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100D0B50u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined FUN_100d0e80() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x100D0E80u))(); }
XI_ORIG void FUN_100d0fc0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x100D0FC0u))(a0, a1, a2); }
XI_ORIG uint8_t CXiSkeletonActor_IsReadCompleteBody(void * a0) { return reinterpret_cast<uint8_t (__thiscall *)(void *)>(XI_GAME(0x100D12D0u))(a0); }
XI_ORIG unsigned long long CXiSkeletonActor_IsReadCompleteBody_v1(XI_ANY a0, XI_ANY a1) {
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
XI_ORIG int FUN_100d4410(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100D4410u))(a0, a1, a2, a3, a4); }
XI_ORIG unsigned long long FUN_100d44b0(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
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
XI_ORIG uint32_t FUN_100d6240(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100D6240u))(a0, a1); }
XI_ORIG uint8_t sub_100D6240(void * a0, int a1) { return reinterpret_cast<uint8_t (__thiscall *)(void *, int)>(XI_GAME(0x100D6240u))(a0, a1); }
XI_ORIG undefined4 FUN_100d65b0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100D65B0u))(a0, a1); }
XI_ORIG int Actor_PrepareDrawGlobals(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100D6D30u))(a0); }
XI_ORIG _DWORD * CXiSkeletonActorRes_Constructor(void * a0, int a1, int a2) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int, int)>(XI_GAME(0x100D7020u))(a0, a1, a2); }
XI_ORIG char CXiSkeletonActorRes_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x100D7050u))(a0); }
XI_ORIG int CXiSkeletonActorRes_GetLinkedActor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100D7090u))(a0); }
XI_ORIG void PHP_MD4Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100D7420u))(a0); }
XI_ORIG void _opiemd4update(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100D7450u))(a0, a1, a2); }
XI_ORIG void FUN_100d7510(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100D7510u))(a0); }
XI_ORIG void FUN_100d7560(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100D7560u))(a0, a1); }
XI_ORIG void FUN_100d75a0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100D75A0u))(a0, a1); }
XI_ORIG int FUN_100d7e90(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x100D7E90u))(a0); }
XI_ORIG void FUN_100d7ea0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100D7EA0u))(a0, a1, a2); }
XI_ORIG void FUN_100d7ed0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100D7ED0u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_100d8490(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100D8490u))(a0); }
XI_ORIG undefined4 FUN_100d8a20(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100D8A20u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_100d93e0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100D93E0u))(); }
XI_ORIG undefined4 FUN_100d9410() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100D9410u))(); }
XI_ORIG undefined4 FUN_100d9430() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100D9430u))(); }
XI_ORIG undefined4 FUN_100d9460() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100D9460u))(); }
XI_ORIG void FUN_100d9830() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100D9830u))(); }
XI_ORIG undefined4 FUN_100d9950(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100D9950u))(a0); }
XI_ORIG undefined4 FUN_100d9b00() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100D9B00u))(); }
XI_ORIG char sub_100DA6A0(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x100DA6A0u))(a0); }
XI_ORIG undefined4 FUN_100daf30(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100DAF30u))(a0); }
XI_ORIG int GameMode_PollPendingState(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100DB140u))(a0); }
XI_ORIG int GameMode_PollPendingState_v1(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100DB140u))(a0); }
XI_ORIG int GameMode_RequestState(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(XI_GAME(0x100DB170u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_100db780(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x100DB780u))(a0); }
XI_ORIG int sub_100DB790(int a0) { return reinterpret_cast<int (__stdcall *)(int)>(XI_GAME(0x100DB790u))(a0); }
XI_ORIG undefined4 FUN_100db930() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DB930u))(); }
XI_ORIG undefined4 FUN_100dbca0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DBCA0u))(); }
XI_ORIG undefined1 FUN_100dbde0() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100DBDE0u))(); }
XI_ORIG DWORD timeGetTime_100DBE00() { return reinterpret_cast<DWORD (__stdcall *)()>(XI_GAME(0x100DBE00u))(); }
XI_ORIG undefined4 FUN_100dbe10() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DBE10u))(); }
XI_ORIG undefined4 FUN_100dbe30(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100DBE30u))(a0, a1); }
XI_ORIG char sub_100DBE50() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x100DBE50u))(); }
XI_ORIG undefined4 FUN_100dbeb0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DBEB0u))(); }
XI_ORIG int FUN_100dbf00(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100DBF00u))(a0, a1, a2); }
XI_ORIG void FUN_100dbf20() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100DBF20u))(); }
XI_ORIG undefined4 FUN_100dbfe0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100DBFE0u))(a0, a1); }
XI_ORIG int FUN_100dc060() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100DC060u))(); }
XI_ORIG undefined4 FUN_100dc080() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DC080u))(); }
XI_ORIG undefined4 FUN_100dc090() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DC090u))(); }
XI_ORIG undefined4 FUN_100dc120() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DC120u))(); }
XI_ORIG undefined4 FUN_100dc130() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DC130u))(); }
XI_ORIG undefined4 FUN_100dc1b0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100DC1B0u))(a0, a1); }
XI_ORIG undefined4 FUN_100dc1f0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DC1F0u))(); }
XI_ORIG undefined4 FUN_100dc210() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DC210u))(); }
XI_ORIG undefined4 FUN_100dc250() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DC250u))(); }
XI_ORIG undefined4 FUN_100dc260() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DC260u))(); }
XI_ORIG undefined4 FUN_100dc270() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DC270u))(); }
XI_ORIG undefined4 FUN_100dc2c0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DC2C0u))(); }
XI_ORIG unsigned long long FUN_100dc2d0(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100DC2D0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG uint32_t FUN_100dc3b0(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x100DC3B0u))(a0); }
XI_ORIG undefined4 FUN_100dc420() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100DC420u))(); }
XI_ORIG undefined FUN_100dc650(XI_ANY a0) { return reinterpret_cast<undefined (__cdecl *)(XI_ANY)>(XI_GAME(0x100DC650u))(a0); }
XI_ORIG void FUN_100dca00(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100DCA00u))(a0, a1, a2); }
XI_ORIG void FUN_100dcf60(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100DCF60u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_100dd800(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100DD800u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG long double FUN_100dd990(XI_ANY a0) { return reinterpret_cast<long double (__cdecl *)(XI_ANY)>(XI_GAME(0x100DD990u))(a0); }
XI_ORIG undefined4 FUN_100ddd20(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100DDD20u))(a0, a1, a2); }
XI_ORIG int FUN_100dde40(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100DDE40u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_100dde90(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100DDE90u))(a0); }
XI_ORIG void FUN_100ddec0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100DDEC0u))(a0, a1, a2); }
XI_ORIG void FUN_100ddf60(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100DDF60u))(a0); }
XI_ORIG uint32_t FUN_100df5d0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100DF5D0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_100df6e0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100DF6E0u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int FUN_100df780(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100DF780u))(a0, a1, a2); }
XI_ORIG int FUN_100df7c0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100DF7C0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined1 FUN_100e0100() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100E0100u))(); }
XI_ORIG int FUN_100e08d0(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x100E08D0u))(a0); }
XI_ORIG void FUN_100e09a0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100E09A0u))(a0, a1); }
XI_ORIG undefined4 FUN_100e0a00() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100E0A00u))(); }
XI_ORIG undefined4 FUN_100e0aa0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100E0AA0u))(); }
XI_ORIG void FUN_100e0b10() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100E0B10u))(); }
XI_ORIG undefined4 FUN_100e0d50(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100E0D50u))(a0); }
XI_ORIG int FUN_100e19c0(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x100E19C0u))(a0); }
XI_ORIG undefined4 FUN_100e1ad0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100E1AD0u))(); }
XI_ORIG int FUN_100e1b50() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100E1B50u))(); }
XI_ORIG undefined4 FUN_100e1c00(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100E1C00u))(a0, a1); }
XI_ORIG undefined4 * FUN_100e1c30(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9, XI_ANY a10, XI_ANY a11) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100E1C30u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11); }
XI_ORIG undefined4 FUN_100e2e70(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100E2E70u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined4 _EVP_EncryptInit_ex(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100E2EE0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined1 FUN_100e33f0() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100E33F0u))(); }
XI_ORIG undefined1 FUN_100e4110() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100E4110u))(); }
XI_ORIG undefined1 FUN_100e4330() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100E4330u))(); }
XI_ORIG undefined4 FUN_100e43f0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100E43F0u))(a0); }
XI_ORIG undefined4 FUN_100e47a0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100E47A0u))(a0); }
XI_ORIG void FUN_100e5e30(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100E5E30u))(a0); }
XI_ORIG undefined1 FUN_100e5ff0() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100E5FF0u))(); }
XI_ORIG int FUN_100e6020() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100E6020u))(); }
XI_ORIG void FUN_100e6080(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100E6080u))(a0, a1, a2); }
XI_ORIG undefined1 FUN_100e61c0() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100E61C0u))(); }
XI_ORIG void FUN_100e62c0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100E62C0u))(a0); }
XI_ORIG undefined4 FUN_100e6370(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100E6370u))(a0); }
XI_ORIG undefined4 FUN_100e63c0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100E63C0u))(a0); }
XI_ORIG undefined4 FUN_100e63e0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100E63E0u))(a0); }
XI_ORIG undefined4 FUN_100e6650(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100E6650u))(a0); }
XI_ORIG undefined4 FUN_100e6790() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100E6790u))(); }
XI_ORIG undefined4 FUN_100e67e0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100E67E0u))(); }
XI_ORIG undefined4 FUN_100e6810() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100E6810u))(); }
XI_ORIG undefined4 FUN_100e6830() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100E6830u))(); }
XI_ORIG undefined4 FUN_100e6860() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100E6860u))(); }
XI_ORIG void FUN_100e6880(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x100E6880u))(a0); }
XI_ORIG undefined4 FUN_100e6b10() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100E6B10u))(); }
XI_ORIG undefined4 FUN_100e6b20() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100E6B20u))(); }
XI_ORIG int FUN_100e6c40(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x100E6C40u))(a0); }
XI_ORIG void FUN_100e7260() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100E7260u))(); }
XI_ORIG int FUN_100e7b20(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100E7B20u))(a0, a1); }
XI_ORIG undefined1 FUN_100e84b0() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100E84B0u))(); }
XI_ORIG undefined1 FUN_100e8b70() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100E8B70u))(); }
XI_ORIG int FUN_100e8b80(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x100E8B80u))(a0); }
XI_ORIG int FUN_100e8c70() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100E8C70u))(); }
XI_ORIG undefined4 FUN_100e9550(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100E9550u))(a0); }
XI_ORIG undefined1 FUN_100eb0a0() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100EB0A0u))(); }
XI_ORIG short * FUN_100eb490(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<short * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100EB490u))(a0, a1); }
XI_ORIG int FUN_100eb8d0(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x100EB8D0u))(a0); }
XI_ORIG undefined4 FUN_100eb940(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100EB940u))(a0); }
XI_ORIG undefined4 FUN_100ebf40(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100EBF40u))(a0); }
XI_ORIG int FUN_100ebf80(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100EBF80u))(a0, a1, a2); }
XI_ORIG void FUN_100ec060(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100EC060u))(a0, a1); }
XI_ORIG void FUN_100edea0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100EDEA0u))(); }
XI_ORIG undefined4 FUN_100edeb0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100EDEB0u))(); }
XI_ORIG undefined FUN_100ef1a0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x100EF1A0u))(); }
XI_ORIG undefined4 FUN_100efc70(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100EFC70u))(a0); }
XI_ORIG undefined4 FUN_100efca0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100EFCA0u))(a0); }
XI_ORIG char * FUN_100efce0(XI_ANY a0) { return reinterpret_cast<char * (__cdecl *)(XI_ANY)>(XI_GAME(0x100EFCE0u))(a0); }
XI_ORIG int FUN_100efdf0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100EFDF0u))(a0, a1, a2); }
XI_ORIG undefined1 FUN_100f0090() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100F0090u))(); }
XI_ORIG unsigned long long FUN_100f0130(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) {
    uint32_t xs[6] = {a0.bits, a1.bits, a2.bits, a3.bits, a4.bits, a5.bits}; uint32_t ra = a6.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 20(%%esi)\n\t""pushl 16(%%esi)\n\t""pushl 12(%%esi)\n\t""pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $24, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100F0130u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_100f02a0(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100F02A0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_100f04f0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100F04F0u))(a0); }
XI_ORIG undefined4 FUN_100f0550(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100F0550u))(a0); }
XI_ORIG undefined1 FUN_100f06b0() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100F06B0u))(); }
XI_ORIG undefined1 FUN_100f09c0() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100F09C0u))(); }
XI_ORIG uint32_t FUN_100f0c50() { return reinterpret_cast<uint32_t (__cdecl *)()>(XI_GAME(0x100F0C50u))(); }
XI_ORIG undefined4 FUN_100f10c0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100F10C0u))(a0); }
XI_ORIG int FUN_100f10f0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x100F10F0u))(); }
XI_ORIG int FUN_100f1160(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x100F1160u))(a0); }
XI_ORIG undefined4 FUN_100f1280(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100F1280u))(a0, a1, a2); }
XI_ORIG char * FUN_100f1300(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<char * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100F1300u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_100f1340(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100F1340u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_100f1410(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100F1410u))(a0); }
XI_ORIG undefined4 thunk_FUN_1030ff33() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x100F1770u))(); }
XI_ORIG undefined4 thunk_FUN_1030ff49() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x100F1780u))(); }
XI_ORIG void FUN_100f1870() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100F1870u))(); }
XI_ORIG void FUN_100f1900() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100F1900u))(); }
XI_ORIG int FUN_100f1b20(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x100F1B20u))(a0); }
XI_ORIG undefined4 FUN_100f1ba0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100F1BA0u))(a0); }
XI_ORIG uint32_t FUN_100f1c40(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100F1C40u))(a0, a1); }
XI_ORIG undefined4 FUN_100f1d20(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100F1D20u))(a0); }
XI_ORIG undefined1 FUN_100f1e70(void * a0) { return reinterpret_cast<undefined1 (__thiscall *)(void *)>(XI_GAME(0x100F1E70u))(a0); }
XI_ORIG undefined4 FUN_100f1f00(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100F1F00u))(a0, a1); }
XI_ORIG undefined4 FUN_100f25f0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100F25F0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_100f2690(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100F2690u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_100f2720(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100F2720u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_100f29e0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100F29E0u))(a0); }
XI_ORIG void FUN_100f2a40(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x100F2A40u))(a0); }
XI_ORIG undefined4 FUN_100f2a70(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100F2A70u))(a0); }
XI_ORIG undefined4 FUN_100f2b80(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x100F2B80u))(a0); }
XI_ORIG unsigned long long FUN_100f2c40(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100F2C40u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_100f2c80(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100F2C80u))(a0, a1); }
XI_ORIG void FUN_100f3010(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100F3010u))(a0, a1, a2); }
XI_ORIG int FUN_100f32b0(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x100F32B0u))(a0, a1); }
XI_ORIG undefined4 FUN_100f3bb0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100F3BB0u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG undefined4 FUN_100f3c20(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100F3C20u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG undefined4 FUN_100f3c90(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100F3C90u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG undefined4 FUN_100f6f00() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100F6F00u))(); }
XI_ORIG undefined4 FUN_100f6f30() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100F6F30u))(); }
XI_ORIG undefined4 FUN_100f6f90() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100F6F90u))(); }
XI_ORIG undefined4 FUN_100f76b0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100F76B0u))(); }
XI_ORIG undefined4 FUN_100f7700() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100F7700u))(); }
XI_ORIG undefined1 FUN_100f8920() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100F8920u))(); }
XI_ORIG undefined1 FUN_100f8cb0() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100F8CB0u))(); }
XI_ORIG undefined1 FUN_100f9350() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x100F9350u))(); }
XI_ORIG undefined4 FUN_100f9cc0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100F9CC0u))(); }
XI_ORIG void FUN_100f9d50() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x100F9D50u))(); }
XI_ORIG undefined4 FUN_100f9da0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100F9DA0u))(); }
XI_ORIG undefined4 FUN_100f9db0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100F9DB0u))(); }
XI_ORIG undefined4 FUN_100f9df0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100F9DF0u))(a0); }
XI_ORIG undefined4 FUN_100f9e10(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100F9E10u))(a0); }
XI_ORIG undefined1 FUN_100fa180(XI_ANY a0) { return reinterpret_cast<undefined1 (__cdecl *)(XI_ANY)>(XI_GAME(0x100FA180u))(a0); }
XI_ORIG undefined4 FUN_100fa4a0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100FA4A0u))(a0); }
XI_ORIG unsigned long long FUN_100fa940(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100FA940u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_100fc0f0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100FC0F0u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_100fc140(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100FC140u))(a0); }
XI_ORIG undefined4 FUN_100fc160() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100FC160u))(); }
XI_ORIG undefined4 FUN_100fc180(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100FC180u))(a0); }
XI_ORIG undefined4 FUN_100fc2b0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100FC2B0u))(a0); }
XI_ORIG undefined4 FUN_100fc2e0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100FC2E0u))(); }
XI_ORIG undefined4 FUN_100fc300() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100FC300u))(); }
XI_ORIG undefined4 FUN_100fc320(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x100FC320u))(a0); }
XI_ORIG unsigned long long thunk_FUN_100fc370(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[2] = {a0.bits, a1.bits}; uint32_t ra = a2.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $8, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x100FC3B0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined8 FUN_100fc3c0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100FC3C0u))(a0, a1, a2); }
XI_ORIG undefined4 thunk_FUN_100ddd20(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100FC4D0u))(a0, a1, a2); }
XI_ORIG undefined * FUN_100fc510() { return reinterpret_cast<undefined * (__cdecl *)()>(XI_GAME(0x100FC510u))(); }
XI_ORIG undefined4 FUN_100fc680(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x100FC680u))(a0, a1); }
XI_ORIG void FUN_100fca20(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100FCA20u))(a0, a1, a2); }
XI_ORIG void FUN_100fca40(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100FCA40u))(a0, a1, a2); }
XI_ORIG void FUN_100fca60(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x100FCA60u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_100fcd70() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x100FCD70u))(); }
XI_ORIG undefined4 FUN_10101bc0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10101BC0u))(a0); }
XI_ORIG uint32_t FUN_10101bf0(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x10101BF0u))(a0); }
XI_ORIG uint32_t ntGameInit() { return reinterpret_cast<uint32_t (__cdecl *)()>(XI_GAME(0x10102340u))(); }
XI_ORIG void FUN_10103720() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x10103720u))(); }
XI_ORIG undefined4 FUN_10103740() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10103740u))(); }
XI_ORIG int sub_101037F0(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(XI_GAME(0x101037F0u))(a0, a1); }
XI_ORIG undefined4 FUN_101038c0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x101038C0u))(); }
XI_ORIG undefined4 FUN_101038d0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x101038D0u))(); }
XI_ORIG undefined4 FUN_101038e0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x101038E0u))(); }
XI_ORIG int ntGameTimeGet() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101039E0u))(); }
XI_ORIG uint32_t FUN_10103ae0(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x10103AE0u))(a0); }
XI_ORIG undefined4 FUN_10103b10(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10103B10u))(a0); }
XI_ORIG uint32_t FUN_10103b30(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x10103B30u))(a0); }
XI_ORIG uint32_t FUN_10103b60(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x10103B60u))(a0); }
XI_ORIG void FUN_10103f50(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10103F50u))(a0); }
XI_ORIG undefined4 FUN_10107610(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10107610u))(a0, a1); }
XI_ORIG void FUN_10108150() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10108150u))(); }
XI_ORIG void FUN_10108300(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10108300u))(a0, a1, a2); }
XI_ORIG int FUN_101083c0(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x101083C0u))(a0); }
XI_ORIG undefined4 * FUN_10108460(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10108460u))(a0, a1); }
XI_ORIG undefined4 FUN_10108590(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10108590u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_101085c0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101085C0u))(a0, a1); }
XI_ORIG undefined4 FUN_101085e0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101085E0u))(a0); }
XI_ORIG void thunk_FUN_10108600(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x101086B0u))(a0); }
XI_ORIG void FUN_10108700(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10108700u))(a0, a1); }
XI_ORIG void FUN_10108780(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10108780u))(a0, a1); }
XI_ORIG uint32_t FUN_10108870(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10108870u))(a0, a1); }
XI_ORIG int FUN_101088a0(XI_ANY a0) { return reinterpret_cast<int (__stdcall *)(XI_ANY)>(XI_GAME(0x101088A0u))(a0); }
XI_ORIG void FUN_101088c0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101088C0u))(a0); }
XI_ORIG void FUN_10108d80(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10108D80u))(a0, a1); }
XI_ORIG void FUN_10108e80(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10108E80u))(a0, a1, a2); }
XI_ORIG void FUN_10108f40(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10108F40u))(a0, a1, a2); }
XI_ORIG undefined4 * FUN_10109000(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 * (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10109000u))(a0, a1, a2, a3); }
XI_ORIG void FUN_101090a0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101090A0u))(a0); }
XI_ORIG void FUN_10109100(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10109100u))(a0); }
XI_ORIG void * FUN_10109140(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10109140u))(a0, a1); }
XI_ORIG int FUN_101091d0(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101091D0u))(a0, a1); }
XI_ORIG unsigned long long FUN_101092c0(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101092C0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_101094d0(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101094D0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_1010e040(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1010E040u))(a0, a1); }
XI_ORIG void FUN_1010f8b0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1010F8B0u))(); }
XI_ORIG void FUN_1010fa30(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1010FA30u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void FUN_1010fbd0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1010FBD0u))(a0, a1, a2); }
XI_ORIG void FUN_1010fc50(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1010FC50u))(a0, a1, a2, a3); }
XI_ORIG void FUN_1010feb0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1010FEB0u))(a0, a1); }
XI_ORIG void FUN_1010feb0_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1010FEB0u))(); }
XI_ORIG void FUN_1010fee0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1010FEE0u))(a0, a1); }
XI_ORIG void FUN_10110040(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10110040u))(a0, a1); }
XI_ORIG void FUN_10113220() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10113220u))(); }
XI_ORIG void FUN_101133a0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x101133A0u))(); }
XI_ORIG undefined8 FUN_10113ca0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10113CA0u))(a0, a1, a2); }
XI_ORIG void * FDtXiErrorReportCtrl_Constructor(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10113D30u))(a0); }
XI_ORIG void FUN_10114070(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10114070u))(a0); }
XI_ORIG undefined4 * FUN_10114120(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10114120u))(a0, a1); }
XI_ORIG void FUN_10114250(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10114250u))(a0, a1); }
XI_ORIG void FUN_10114320(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10114320u))(a0); }
XI_ORIG void FUN_10114500(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10114500u))(a0); }
XI_ORIG void FUN_10114f10(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10114F10u))(a0, a1, a2); }
XI_ORIG void FDtGmNoticeProc() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10115020u))(); }
XI_ORIG uint32_t FUN_10115250() { return reinterpret_cast<uint32_t (__cdecl *)()>(XI_GAME(0x10115250u))(); }
XI_ORIG undefined4 * FUN_101152f0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101152F0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined4 FUN_101154d0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101154D0u))(a0); }
XI_ORIG undefined4 FUN_10115520(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10115520u))(a0); }
XI_ORIG undefined4 FUN_10115580(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10115580u))(a0); }
XI_ORIG undefined4 FUN_101164b0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101164B0u))(a0, a1); }
XI_ORIG undefined4 FUN_10116540(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10116540u))(a0); }
XI_ORIG void FUN_10116610(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10116610u))(a0, a1, a2); }
XI_ORIG void FUN_10116610_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10116610u))(); }
XI_ORIG undefined8 FUN_101170d0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101170D0u))(a0, a1, a2); }
XI_ORIG void FUN_10117460(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10117460u))(a0); }
XI_ORIG undefined8 FUN_101177d0(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x101177D0u))(a0); }
XI_ORIG void FUN_10117f30(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10117F30u))(a0); }
XI_ORIG void FUN_101180f0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101180F0u))(a0); }
XI_ORIG void FUN_10118170(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10118170u))(a0, a1); }
XI_ORIG undefined8 FUN_101181d0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101181D0u))(a0, a1); }
XI_ORIG short * FUN_10118220(void * a0) { return reinterpret_cast<short * (__thiscall *)(void *)>(XI_GAME(0x10118220u))(a0); }
XI_ORIG unsigned long long FUN_10118260(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10118260u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined8 FUN_101186a0(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x101186A0u))(a0); }
XI_ORIG short * FUN_101188e0(void * a0, XI_ANY a1) { return reinterpret_cast<short * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101188E0u))(a0, a1); }
XI_ORIG void CTkMenuCtrlData_GetButtonLocate(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10118900u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_10118b60(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10118B60u))(a0, a1, a2); }
XI_ORIG uint32_t FUN_10118c10(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10118C10u))(a0, a1); }
XI_ORIG undefined4 FUN_10118cb0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10118CB0u))(a0, a1, a2); }
XI_ORIG unsigned long long FUN_10118db0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[2] = {a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10118DB0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_10118e90(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10118E90u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_10118f40(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10118F40u))(a0); }
XI_ORIG undefined8 CTkMenuCtrlData_GetWindowLocate(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10118F70u))(a0, a1); }
XI_ORIG undefined4 FUN_10119110(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10119110u))(a0, a1, a2); }
XI_ORIG void FUN_10119200(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10119200u))(a0, a1); }
XI_ORIG void FUN_101192e0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101192E0u))(a0, a1); }
XI_ORIG void * FUN_10119300(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10119300u))(a0); }
XI_ORIG void FUN_10119360(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10119360u))(a0); }
XI_ORIG void FUN_10119390(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10119390u))(a0); }
XI_ORIG undefined4 FUN_101193d0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101193D0u))(a0, a1); }
XI_ORIG undefined4 FUN_101193f0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101193F0u))(a0, a1, a2, a3); }
XI_ORIG undefined8 FUN_10119550(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10119550u))(a0, a1); }
XI_ORIG undefined4 FUN_10119870(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10119870u))(a0); }
XI_ORIG void * FUN_101198d0(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101198D0u))(a0, a1); }
XI_ORIG undefined8 FUN_10119970(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10119970u))(a0, a1); }
XI_ORIG void FUN_10119970_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10119970u))(a0, a1); }
XI_ORIG undefined4 FUN_10119a50(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10119A50u))(a0, a1); }
XI_ORIG undefined4 FUN_10119ad0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10119AD0u))(a0, a1, a2, a3); }
XI_ORIG uint32_t FUN_10119bc0(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10119BC0u))(a0, a1); }
XI_ORIG unsigned long long FUN_10119c00(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10119C00u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void * FUN_10119da0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10119DA0u))(a0); }
XI_ORIG void FUN_10119e20(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10119E20u))(a0, a1); }
XI_ORIG undefined4 FUN_10119e30(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10119E30u))(a0, a1); }
XI_ORIG void FUN_10119e70(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10119E70u))(a0); }
XI_ORIG unsigned long long FUN_10119f20(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10119F20u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined8 FUN_10119fb0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10119FB0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG void FUN_1011a490(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1011A490u))(a0); }
XI_ORIG unsigned long long FUN_1011a560(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1011A560u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_1011a5a0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1011A5A0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG undefined4 FUN_1011abf0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1011ABF0u))(a0, a1); }
XI_ORIG _WORD * sub_1011AC70(void * a0, __int16 a1, __int16 a2) { return reinterpret_cast<_WORD * (__thiscall *)(void *, __int16, __int16)>(XI_GAME(0x1011AC70u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1011ae80(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1011AE80u))(a0); }
XI_ORIG int FUN_1011b2e0(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1011B2E0u))(a0, a1); }
XI_ORIG undefined4 FUN_1011b310(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1011B310u))(a0); }
XI_ORIG undefined4 FUN_1011b320(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1011B320u))(a0); }
XI_ORIG void FUN_1011b330(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1011B330u))(a0, a1, a2); }
XI_ORIG undefined8 FUN_1011ba40(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x1011BA40u))(a0); }
XI_ORIG int sub_1011BA90(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x1011BA90u))(a0); }
XI_ORIG char Font_BindGlyphTexture(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x1011BAE0u))(a0); }
XI_ORIG undefined4 Font_BindGlyphTexture_v1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1011BAE0u))(a0); }
XI_ORIG undefined4 FUN_1011bba0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1011BBA0u))(a0, a1, a2, a3, a4); }
XI_ORIG int sub_1011BBA0(void * a0, __int16 a1, void * a2, float a3, float a4) { return reinterpret_cast<int (__thiscall *)(void *, __int16, void *, float, float)>(XI_GAME(0x1011BBA0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 Font_DrawGlyphQuad(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1011BBD0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int Gfx_SetAdditiveBlend() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1011BDC0u))(); }
XI_ORIG void Gfx_SetAdditiveBlend_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1011BDC0u))(); }
XI_ORIG void Font_DrawChar(__int16 a0, int a1, int a2, float a3, float a4) { reinterpret_cast<void (__cdecl *)(__int16, int, int, float, float)>(XI_GAME(0x1011BF40u))(a0, a1, a2, a3, a4); }
XI_ORIG int FUN_1011bfd0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x1011BFD0u))(a0, a1); }
XI_ORIG undefined8 Obj_104E1B90_DrawString(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1011C080u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG int Gfx_SetCellBlendMode(int a0, int a1, int a2, int a3) { return reinterpret_cast<int (__cdecl *)(int, int, int, int)>(XI_GAME(0x1011C1F0u))(a0, a1, a2, a3); }
XI_ORIG int Font_AllocHighResImage(void * a0, int a1, int a2) { return reinterpret_cast<int (__stdcall *)(void *, int, int)>(XI_GAME(0x1011C5D0u))(a0, a1, a2); }
XI_ORIG char Tex_LoadSurfaceFromDatFile(void * a0, int a1) { return reinterpret_cast<char (__stdcall *)(void *, int)>(XI_GAME(0x1011C670u))(a0, a1); }
XI_ORIG void Font_LoadHighResTexture(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1011C720u))(a0); }
XI_ORIG int Font_ReleaseHighResTexture(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1011C810u))(a0); }
XI_ORIG uint8_t Font_IsHighResTextureLoaded(int a0) { return reinterpret_cast<uint8_t (__thiscall *)(int)>(XI_GAME(0x1011C850u))(a0); }
XI_ORIG int unknown_libname_88(_DWORD a0) { return reinterpret_cast<int (__thiscall *)(_DWORD)>(XI_GAME(0x1011C870u))(a0); }
XI_ORIG char sub_1011C8B0() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x1011C8B0u))(); }
XI_ORIG undefined4 FUN_1011c900(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1011C900u))(a0); }
XI_ORIG undefined8 FUN_1011c940(XI_ANY a0) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY)>(XI_GAME(0x1011C940u))(a0); }
XI_ORIG __int16 Font_DecodeNextChar(void * a0) { return reinterpret_cast<__int16 (__cdecl *)(void *)>(XI_GAME(0x1011CE80u))(a0); }
XI_ORIG int Font_DecodeNextChar_v1(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x1011CE80u))(a0); }
XI_ORIG int FUN_1011cec0(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x1011CEC0u))(a0); }
XI_ORIG int FUN_1011cf70() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1011CF70u))(); }
XI_ORIG int FUN_1011cf70_v1(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1011CF70u))(a0, a1); }
XI_ORIG void FUN_1011d000(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1011D000u))(a0); }
XI_ORIG __int16 sub_1011D050(void * a0) { return reinterpret_cast<__int16 (__cdecl *)(void *)>(XI_GAME(0x1011D050u))(a0); }
XI_ORIG undefined4 FUN_1011d290(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1011D290u))(a0); }
XI_ORIG char Font_AddGlyphAdvance(unsigned __int16 a0, void * a1) { return reinterpret_cast<char (__cdecl *)(unsigned __int16, void *)>(XI_GAME(0x1011D840u))(a0, a1); }
XI_ORIG unsigned long long Font_AddGlyphAdvance_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[2] = {a0.bits, a1.bits}; uint32_t ra = a2.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $8, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1011D840u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_1011d8b0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1011D8B0u))(a0); }
XI_ORIG unsigned long long FUN_1011d990(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = 0, rc = 0, rd = a1.bits;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1011D990u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_1011da30(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1011DA30u))(a0); }
XI_ORIG int Sjis_GetCharByteLen(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x1011DA80u))(a0); }
XI_ORIG undefined4 Sjis_GetCharByteLen_v1(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1011DA80u))(a0); }
XI_ORIG undefined4 FUN_1011daa0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1011DAA0u))(a0); }
XI_ORIG undefined4 FUN_1011db40(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1011DB40u))(a0); }
XI_ORIG undefined4 FUN_1011dcc0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1011DCC0u))(a0, a1); }
XI_ORIG size_t FUN_1011de80(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<size_t (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1011DE80u))(a0, a1, a2); }
XI_ORIG int FUN_1011e290(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1011E290u))(a0); }
XI_ORIG void FUN_1011e2a0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1011E2A0u))(a0); }
XI_ORIG undefined4 FUN_1011e6d0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1011E6D0u))(a0); }
XI_ORIG undefined8 Cell_DrawStretchedWrapped(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9, XI_ANY a10, XI_ANY a11) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1011EF60u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11); }
XI_ORIG int Cell_DrawWithDefaultBlend(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1011F2D0u))(a0); }
XI_ORIG int Cell_DrawQuadBlended(void * a0, int a1, int a2, int a3, int a4, int a5) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int, int)>(XI_GAME(0x1011F300u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void Cell_DrawQuadBlended_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1011F300u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int FUN_1011fa80(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1011FA80u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1011fb00(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1011FB00u))(a0, a1, a2); }
XI_ORIG undefined8 FUN_1011fb60(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1011FB60u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int sub_1011FB60(void * a0, int a1, int a2, int a3, int a4, int a5) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int, int)>(XI_GAME(0x1011FB60u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int Shape_DrawScaled(int a0, int a1, int a2, int a3, int a4, int a5, char a6, int a7, int a8) { return reinterpret_cast<int (__thiscall *)(int, int, int, int, int, int, char, int, int)>(XI_GAME(0x1011FC80u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG undefined8 Shape_DrawScaled_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1011FC80u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG void FUN_10120000(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9, XI_ANY a10, XI_ANY a11) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10120000u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11); }
XI_ORIG void FUN_10120070(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10120070u))(a0); }
XI_ORIG int Color_ModulateArgb128(unsigned int a0, unsigned int a1) { return reinterpret_cast<int (__cdecl *)(unsigned int, unsigned int)>(XI_GAME(0x101200B0u))(a0, a1); }
XI_ORIG _DWORD * ShapeMng_Init(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x101201C0u))(a0); }
XI_ORIG _DWORD * sub_10120200(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10120200u))(a0); }
XI_ORIG char * ShapeMng_FindShape(void * a0, void * a1) { return reinterpret_cast<char * (__thiscall *)(void *, void *)>(XI_GAME(0x10120450u))(a0, a1); }
XI_ORIG undefined8 ShapeMng_FindShape_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10120450u))(a0, a1); }
XI_ORIG undefined4 ShapeMng_FindShape_v2(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10120450u))(a0, a1); }
XI_ORIG undefined4 FUN_101204b0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101204B0u))(a0, a1); }
XI_ORIG uint8_t ShapeMng_GetFontGlyph(void * a0, unsigned __int8 a1, int a2, void * a3, void * a4, void * a5) { return reinterpret_cast<uint8_t (__thiscall *)(void *, unsigned __int8, int, void *, void *, void *)>(XI_GAME(0x10120510u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG char ShapeMng_BindFontTexture(void * a0, unsigned int a1, void * a2, void * a3) { return reinterpret_cast<char (__thiscall *)(void *, unsigned int, void *, void *)>(XI_GAME(0x10120590u))(a0, a1, a2, a3); }
XI_ORIG char * ShapeMng_LoadFontTextures(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10120660u))(a0); }
XI_ORIG char * ShapeMng_LoadDmgNumTextures(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10120880u))(a0); }
XI_ORIG void FUN_10120950(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10120950u))(a0); }
XI_ORIG undefined4 FUN_10120b80() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10120B80u))(); }
XI_ORIG undefined4 FUN_10120bb0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10120BB0u))(); }
XI_ORIG undefined4 FUN_10120be0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10120BE0u))(); }
XI_ORIG long double FUN_10122e30(XI_ANY a0) { return reinterpret_cast<long double (__cdecl *)(XI_ANY)>(XI_GAME(0x10122E30u))(a0); }
XI_ORIG undefined4 FUN_10123350(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10123350u))(a0); }
XI_ORIG undefined4 FUN_10123380(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10123380u))(a0, a1); }
XI_ORIG undefined4 FUN_10123670(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10123670u))(a0); }
XI_ORIG long double GetAnalogKey(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<long double (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10123970u))(a0, a1); }
XI_ORIG unsigned long long FUN_10123a70(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) {
    uint32_t xs[4] = {a0.bits, a1.bits, a2.bits, a3.bits}; uint32_t ra = a4.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 12(%%esi)\n\t""pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $16, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10123A70u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_10123e50(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10123E50u))(a0); }
XI_ORIG unsigned long long FUN_10123e60(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10123E60u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void sub_10123E90() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10123E90u))(); }
XI_ORIG void FUN_10123ea0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10123EA0u))(); }
XI_ORIG undefined4 FUN_10124010(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10124010u))(a0, a1); }
XI_ORIG undefined4 FUN_10124050(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10124050u))(a0); }
XI_ORIG undefined4 FUN_10124070(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10124070u))(a0, a1); }
XI_ORIG undefined4 FUN_10124090(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10124090u))(a0, a1); }
XI_ORIG undefined4 FUN_101240b0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101240B0u))(a0, a1); }
XI_ORIG undefined4 FUN_101240d0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101240D0u))(a0, a1); }
XI_ORIG undefined4 FUN_101240f0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101240F0u))(a0, a1); }
XI_ORIG undefined4 FUN_10124110(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10124110u))(a0, a1); }
XI_ORIG undefined4 FUN_10124130(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10124130u))(a0, a1); }
XI_ORIG __int16 Cursor_GetHwCursorSlot(void * a0) { return reinterpret_cast<__int16 (__thiscall *)(void *)>(XI_GAME(0x101245C0u))(a0); }
XI_ORIG void CTkMouse_OnMouseMove(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10124640u))(a0, a1, a2); }
XI_ORIG void Cursor_SetPosition(int a0, void * a1, float a2) { reinterpret_cast<void (__thiscall *)(int, void *, float)>(XI_GAME(0x10124700u))(a0, a1, a2); }
XI_ORIG unsigned long long Cursor_SetPosition_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[2] = {a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10124700u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void sub_101248A0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101248A0u))(a0); }
XI_ORIG void FUN_10124930(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10124930u))(a0); }
XI_ORIG void FUN_101249e0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101249E0u))(a0); }
XI_ORIG void FUN_10124a00(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10124A00u))(a0); }
XI_ORIG void FUN_10124ab0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10124AB0u))(a0); }
XI_ORIG void FUN_10124ad0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10124AD0u))(a0); }
XI_ORIG void FUN_10124b50(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10124B50u))(a0); }
XI_ORIG undefined4 FUN_10124b70(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10124B70u))(a0); }
XI_ORIG undefined8 FUN_10124bc0(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x10124BC0u))(a0); }
XI_ORIG unsigned long long Cursor_Draw(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {0}; uint32_t ra = a2.bits, rc = a0.bits, rd = a1.bits;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10124EB0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_101252c0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101252C0u))(a0); }
XI_ORIG unsigned __int8 Cursor_SetVisible(void * a0, unsigned __int8 a1) { return reinterpret_cast<unsigned __int8 (__thiscall *)(void *, unsigned __int8)>(XI_GAME(0x101252F0u))(a0, a1); }
XI_ORIG undefined8 Cursor_SetVisible_v1(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101252F0u))(a0, a1); }
XI_ORIG long double FUN_101260d0(void * a0) { return reinterpret_cast<long double (__thiscall *)(void *)>(XI_GAME(0x101260D0u))(a0); }
XI_ORIG int Cursor_StoreScreenCenter(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101262A0u))(a0); }
XI_ORIG void Cursor_StoreScreenCenter_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101262A0u))(a0); }
XI_ORIG undefined4 FUN_10126300(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10126300u))(a0); }
XI_ORIG undefined4 FUN_10126320(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10126320u))(a0); }
XI_ORIG void FUN_10126320_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10126320u))(a0); }
XI_ORIG void FUN_10126330() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10126330u))(); }
XI_ORIG void FUN_10126350(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10126350u))(a0); }
XI_ORIG void FUN_10126370(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10126370u))(a0); }
XI_ORIG undefined4 * FUN_101263b0(void * a0) { return reinterpret_cast<undefined4 * (__thiscall *)(void *)>(XI_GAME(0x101263B0u))(a0); }
XI_ORIG void FUN_10126410(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x10126410u))(a0); }
XI_ORIG undefined8 FUN_10126480(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10126480u))(a0, a1); }
XI_ORIG void FUN_10126480_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10126480u))(a0, a1); }
XI_ORIG undefined4 FUN_10126500(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10126500u))(a0); }
XI_ORIG undefined4 FUN_10126510(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10126510u))(a0); }
XI_ORIG undefined8 FUN_10126520(XI_ANY a0) { return reinterpret_cast<undefined8 (__stdcall *)(XI_ANY)>(XI_GAME(0x10126520u))(a0); }
XI_ORIG undefined8 FUN_10126550(XI_ANY a0) { return reinterpret_cast<undefined8 (__stdcall *)(XI_ANY)>(XI_GAME(0x10126550u))(a0); }
XI_ORIG void FUN_10126580(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10126580u))(a0, a1); }
XI_ORIG void FUN_101265d0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101265D0u))(a0); }
XI_ORIG void FUN_10126700(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10126700u))(a0, a1, a2); }
XI_ORIG void * FUN_10126790(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10126790u))(a0, a1); }
XI_ORIG void * FUN_10126b60(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10126B60u))(a0, a1, a2); }
XI_ORIG void FUN_10126bc0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10126BC0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined8 FUN_10126c60(void * a0, void * a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined8 (__fastcall *)(void *, void *, XI_ANY, XI_ANY)>(XI_GAME(0x10126C60u))(a0, a1, a2, a3); }
XI_ORIG _DWORD * sub_10127190(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10127190u))(a0); }
XI_ORIG void FUN_101271c0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101271C0u))(a0); }
XI_ORIG undefined8 FUN_10127250(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x10127250u))(a0); }
XI_ORIG char * FUN_101278b0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<char * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101278B0u))(a0, a1); }
XI_ORIG undefined4 FUN_101278d0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101278D0u))(a0, a1, a2); }
XI_ORIG void FUN_10127930(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10127930u))(a0, a1, a2); }
XI_ORIG void FUN_10127970(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10127970u))(a0); }
XI_ORIG char ChatLog_AddMessage(int a0, void * a1, void * a2, int a3, char a4, void * a5) { return reinterpret_cast<char (__thiscall *)(int, void *, void *, int, char, void *)>(XI_GAME(0x10127C00u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined4 ChatLog_AddMessage_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10127C00u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined4 FUN_10127ea0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10127EA0u))(); }
XI_ORIG _DWORD * sub_10127EE0(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10127EE0u))(a0); }
XI_ORIG void FUN_10127f00(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10127F00u))(a0); }
XI_ORIG undefined8 FUN_101284f0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101284F0u))(a0, a1, a2, a3, a4); }
XI_ORIG int FUN_10128680(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x10128680u))(a0); }
XI_ORIG void FUN_10128830(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10128830u))(a0, a1, a2); }
XI_ORIG undefined8 FUN_10128950(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10128950u))(a0, a1, a2); }
XI_ORIG unsigned long long FUN_10129290(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10129290u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_101292e0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[2] = {a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101292E0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_101295d0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101295D0u))(a0, a1, a2, a3, a4); }
XI_ORIG unsigned long long FUN_10129c60(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10129C60u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined8 FUN_10129cd0(XI_ANY a0) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY)>(XI_GAME(0x10129CD0u))(a0); }
XI_ORIG undefined4 FUN_10129d00(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10129D00u))(a0); }
XI_ORIG uint32_t FUN_10129d90(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10129D90u))(a0, a1); }
XI_ORIG undefined1 FUN_10129de0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined1 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10129DE0u))(a0, a1); }
XI_ORIG undefined4 FUN_10129e30(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10129E30u))(a0, a1); }
XI_ORIG undefined4 FUN_1012a120(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1012A120u))(a0); }
XI_ORIG void FUN_1012a3b0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1012A3B0u))(a0); }
XI_ORIG undefined4 FUN_1012ca90(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x1012CA90u))(a0); }
XI_ORIG undefined4 FUN_1012cad0() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x1012CAD0u))(); }
XI_ORIG void FUN_10130f90(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10130F90u))(a0); }
XI_ORIG undefined4 * FUN_101312e0(void * a0) { return reinterpret_cast<undefined4 * (__thiscall *)(void *)>(XI_GAME(0x101312E0u))(a0); }
XI_ORIG void * FUN_10131530(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10131530u))(a0); }
XI_ORIG void * FUN_10136380(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10136380u))(a0, a1); }
XI_ORIG void FUN_10136670(void * a0, void * a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__fastcall *)(void *, void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10136670u))(a0, a1, a2, a3, a4); }
XI_ORIG void * FEMultiLineText__FEMultiLineText_0(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10136A30u))(a0, a1); }
XI_ORIG void * FUN_10136f60(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10136F60u))(a0, a1); }
XI_ORIG void FUN_10138ec0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10138EC0u))(a0); }
XI_ORIG unsigned long long thunk_FUN_1013a100(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10139FE0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_1013a010(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1013A010u))(a0); }
XI_ORIG int FUN_1013a030(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1013A030u))(a0); }
XI_ORIG unsigned long long FUN_1013a060(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1013A060u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_1013a070(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1013A070u))(a0); }
XI_ORIG void FUN_1013a0d0() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x1013A0D0u))(); }
XI_ORIG unsigned long long FUN_1013a100(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1013A100u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_1013a100_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1013A100u))(a0); }
XI_ORIG void FUN_1013a320(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1013A320u))(a0, a1, a2); }
XI_ORIG char sub_1013A320(void * a0, void * a1, int a2) { return reinterpret_cast<char (__thiscall *)(void *, void *, int)>(XI_GAME(0x1013A320u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1013a470(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1013A470u))(a0); }
XI_ORIG void * FUN_1013be50(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1013BE50u))(a0); }
XI_ORIG void FUN_1013bf40(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1013BF40u))(a0); }
XI_ORIG void * FUN_10144f40(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10144F40u))(a0); }
XI_ORIG void FUN_10146930(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10146930u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_10146960(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10146960u))(a0, a1, a2, a3, a4); }
XI_ORIG void * FUN_101475b0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101475B0u))(a0); }
XI_ORIG void * FUN_10148430(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10148430u))(a0); }
XI_ORIG void * FUN_10148640(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10148640u))(a0); }
XI_ORIG void * FUN_10148ed0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10148ED0u))(a0); }
XI_ORIG void * FUN_1014a170(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1014A170u))(a0); }
XI_ORIG void * FUN_1014a7d0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1014A7D0u))(a0); }
XI_ORIG void * FUN_1014b670(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1014B670u))(a0); }
XI_ORIG void * FUN_1014ba60(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1014BA60u))(a0); }
XI_ORIG void FUN_1014e020(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1014E020u))(a0); }
XI_ORIG void FUN_1014e050(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x1014E050u))(a0); }
XI_ORIG undefined4 FUN_1014ff70() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1014FF70u))(); }
XI_ORIG void FUN_10150a30(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10150A30u))(a0, a1); }
XI_ORIG void FUN_10150a60(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10150A60u))(a0, a1); }
XI_ORIG char sub_10150CC0(void * a0, void * a1, int a2) { return reinterpret_cast<char (__thiscall *)(void *, void *, int)>(XI_GAME(0x10150CC0u))(a0, a1, a2); }
XI_ORIG void thunk_FUN_10150ce0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10150CC0u))(a0, a1, a2); }
XI_ORIG void FUN_10150ce0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10150CE0u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10150d60(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10150D60u))(a0); }
XI_ORIG void FUN_10151140(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10151140u))(a0); }
XI_ORIG void FUN_10152f20(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10152F20u))(a0, a1); }
XI_ORIG int sub_10152F80() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10152F80u))(); }
XI_ORIG undefined4 FUN_10152fa0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10152FA0u))(); }
XI_ORIG undefined4 FUN_10152fc0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10152FC0u))(a0); }
XI_ORIG undefined4 FUN_10152ff0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10152FF0u))(a0, a1); }
XI_ORIG int FUN_101530c0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101530C0u))(); }
XI_ORIG int FUN_101530c0_v1(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101530C0u))(a0, a1); }
XI_ORIG int FUN_101531b0(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x101531B0u))(a0); }
XI_ORIG undefined4 FUN_10153a40(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10153A40u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined4 FUN_10153af0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10153AF0u))(); }
XI_ORIG undefined4 FUN_10153b30() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10153B30u))(); }
XI_ORIG undefined4 FUN_10153b70() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10153B70u))(); }
XI_ORIG char sub_10153C10(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10153C10u))(a0); }
XI_ORIG undefined4 FUN_10153ef0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10153EF0u))(a0, a1); }
XI_ORIG undefined * FUN_10153ff0() { return reinterpret_cast<undefined * (__cdecl *)()>(XI_GAME(0x10153FF0u))(); }
XI_ORIG _DWORD * sub_10153FF0() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x10153FF0u))(); }
XI_ORIG void * FUN_10154070(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10154070u))(a0); }
XI_ORIG uint32_t FUN_101542b0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x101542B0u))(a0, a1); }
XI_ORIG int FUN_101544f0(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101544F0u))(a0, a1); }
XI_ORIG undefined1 * FUN_10154550(void * a0, XI_ANY a1) { return reinterpret_cast<undefined1 * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10154550u))(a0, a1); }
XI_ORIG void FUN_10154840(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10154840u))(a0); }
XI_ORIG void sub_10154860(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10154860u))(a0); }
XI_ORIG undefined4 FUN_101549e0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101549E0u))(a0); }
XI_ORIG undefined4 FUN_101549f0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101549F0u))(a0, a1); }
XI_ORIG void FUN_10154ab0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10154AB0u))(a0); }
XI_ORIG void FUN_10154ad0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10154AD0u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG void FUN_10154f60(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10154F60u))(a0); }
XI_ORIG void FUN_10154fc0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10154FC0u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_101569a0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101569A0u))(a0); }
XI_ORIG void FUN_101569f0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101569F0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG void FUN_10156be0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10156BE0u))(a0); }
XI_ORIG void FUN_10156bf0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10156BF0u))(a0, a1); }
XI_ORIG int FUN_10156c30(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10156C30u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG unsigned long long thunk_FUN_10157620(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10157610u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_10157620(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10157620u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_10157620_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10157620u))(a0); }
XI_ORIG undefined4 FUN_10157660(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10157660u))(a0, a1); }
XI_ORIG char sub_10157660(int a0, int a1) { return reinterpret_cast<char (__thiscall *)(int, int)>(XI_GAME(0x10157660u))(a0, a1); }
XI_ORIG undefined4 FUN_101576f0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x101576F0u))(); }
XI_ORIG void FUN_10157780(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10157780u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10157860(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10157860u))(a0, a1, a2); }
XI_ORIG unsigned long long FUN_10157880(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10157880u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_10157890(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10157890u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_101578a0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101578A0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_101578b0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101578B0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_101578c0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101578C0u))(a0); }
XI_ORIG undefined4 FUN_101578d0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101578D0u))(a0); }
XI_ORIG undefined4 FUN_101578e0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101578E0u))(a0); }
XI_ORIG void FUN_101578f0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101578F0u))(a0); }
XI_ORIG void FUN_101579b0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101579B0u))(a0); }
XI_ORIG int sub_101579C0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101579C0u))(a0); }
XI_ORIG undefined4 FUN_10157b40(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10157B40u))(a0, a1, a2, a3); }
XI_ORIG char FUN_10157c60(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<char (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10157C60u))(a0, a1, a2, a3, a4); }
XI_ORIG int sub_10157CF0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10157CF0u))(a0); }
XI_ORIG undefined4 FUN_10157d20() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10157D20u))(); }
XI_ORIG undefined4 FUN_10157d60() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10157D60u))(); }
XI_ORIG void FUN_10157e20(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10157E20u))(a0); }
XI_ORIG unsigned long long FUN_10157fa0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10157FA0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_101581e0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101581E0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_101582f0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101582F0u))(a0); }
XI_ORIG void FUN_101583f0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101583F0u))(a0); }
XI_ORIG undefined4 FUN_10158470(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10158470u))(a0); }
XI_ORIG unsigned long long FUN_10158480(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10158480u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_10158510(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10158510u))(a0); }
XI_ORIG int * FUN_10158550(void * a0) { return reinterpret_cast<int * (__thiscall *)(void *)>(XI_GAME(0x10158550u))(a0); }
XI_ORIG undefined4 FUN_10158590(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10158590u))(a0); }
XI_ORIG undefined4 FUN_101585f0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101585F0u))(a0); }
XI_ORIG undefined4 FUN_101587b0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101587B0u))(a0); }
XI_ORIG undefined4 FUN_101587f0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101587F0u))(a0, a1); }
XI_ORIG unsigned long long FUN_10158aa0(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10158AA0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_10158b00(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10158B00u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_10158b40(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10158B40u))(a0, a1); }
XI_ORIG undefined4 FUN_10158d90(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10158D90u))(a0); }
XI_ORIG undefined4 FUN_10158db0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10158DB0u))(a0, a1); }
XI_ORIG undefined4 FUN_10158ff0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10158FF0u))(a0); }
XI_ORIG unsigned long long FUN_10159040(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10159040u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_101590a0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101590A0u))(a0); }
XI_ORIG void FUN_101590c0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101590C0u))(a0, a1); }
XI_ORIG void FUN_10159170(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10159170u))(a0, a1); }
XI_ORIG undefined4 FUN_101591d0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101591D0u))(a0); }
XI_ORIG int FUN_10159370(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10159370u))(a0); }
XI_ORIG char Actor_SetSlotTableEntry(unsigned int a0, int a1) { return reinterpret_cast<char (__cdecl *)(unsigned int, int)>(XI_GAME(0x10159400u))(a0, a1); }
XI_ORIG undefined4 Actor_SetSlotTableEntry_v1(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10159400u))(a0, a1); }
XI_ORIG undefined4 FUN_10159420(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10159420u))(a0, a1); }
XI_ORIG undefined4 FUN_10159440(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10159440u))(a0, a1, a2); }
XI_ORIG void FUN_101594d0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101594D0u))(a0, a1); }
XI_ORIG void FUN_101594e0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101594E0u))(a0, a1); }
XI_ORIG undefined4 FUN_101594f0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101594F0u))(a0); }
XI_ORIG void * FUN_10159530(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10159530u))(a0); }
XI_ORIG unsigned long long FUN_10159540(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10159540u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_101595d0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101595D0u))(a0, a1); }
XI_ORIG undefined4 FUN_10159660(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10159660u))(a0, a1); }
XI_ORIG undefined4 FUN_10159740(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10159740u))(a0); }
XI_ORIG undefined4 FUN_10159780(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10159780u))(a0); }
XI_ORIG undefined4 FUN_101597c0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101597C0u))(a0, a1, a2); }
XI_ORIG uint32_t FUN_101597f0(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x101597F0u))(a0); }
XI_ORIG void FUN_10159830(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10159830u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10159880(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10159880u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_101598a0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101598A0u))(a0); }
XI_ORIG undefined4 FUN_101598b0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101598B0u))(a0); }
XI_ORIG undefined4 FUN_101598c0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101598C0u))(a0, a1); }
XI_ORIG uint32_t FUN_10159910(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10159910u))(a0, a1); }
XI_ORIG undefined4 FUN_10159970(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10159970u))(a0, a1); }
XI_ORIG undefined4 FUN_10159a20(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10159A20u))(a0, a1); }
XI_ORIG void FUN_10159b10(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x10159B10u))(a0); }
XI_ORIG undefined4 FUN_10159ba0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10159BA0u))(a0, a1); }
XI_ORIG undefined4 FUN_10159c80(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10159C80u))(a0, a1); }
XI_ORIG undefined4 FUN_10159d40(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10159D40u))(a0, a1, a2); }
XI_ORIG int * FUN_10159de0(void * a0) { return reinterpret_cast<int * (__thiscall *)(void *)>(XI_GAME(0x10159DE0u))(a0); }
XI_ORIG unsigned long long FUN_10159e10(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10159E10u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined8 FUN_10159eb0(XI_ANY a0) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY)>(XI_GAME(0x10159EB0u))(a0); }
XI_ORIG undefined8 FUN_1015afa0(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x1015AFA0u))(a0); }
XI_ORIG undefined4 FUN_1015b270(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1015B270u))(a0); }
XI_ORIG void FUN_1015b410(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1015B410u))(a0); }
XI_ORIG undefined8 FUN_1015b9e0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015B9E0u))(a0, a1); }
XI_ORIG int FUN_1015bc40(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015BC40u))(a0, a1); }
XI_ORIG void FUN_1015d7f0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1015D7F0u))(a0); }
XI_ORIG undefined1 FUN_1015d860() { return reinterpret_cast<undefined1 (__cdecl *)()>(XI_GAME(0x1015D860u))(); }
XI_ORIG int MenuMng_Init(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x1015D900u))(a0); }
XI_ORIG undefined4 FUN_1015d9a0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1015D9A0u))(a0); }
XI_ORIG _DWORD * sub_1015D9E0(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x1015D9E0u))(a0); }
XI_ORIG undefined8 FUN_1015db80(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1015DB80u))(a0, a1, a2); }
XI_ORIG void FUN_1015dbb0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1015DBB0u))(a0, a1, a2); }
XI_ORIG void FUN_1015dbd0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015DBD0u))(a0, a1); }
XI_ORIG int FUN_1015dc10(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015DC10u))(a0, a1); }
XI_ORIG undefined8 FUN_1015dce0(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x1015DCE0u))(a0); }
XI_ORIG undefined8 FUN_1015dd00(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015DD00u))(a0, a1); }
XI_ORIG void FUN_1015dd50(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015DD50u))(a0, a1); }
XI_ORIG void FUN_1015dd70(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015DD70u))(a0, a1); }
XI_ORIG void nullsub_49(int a0) { reinterpret_cast<void (__stdcall *)(int)>(XI_GAME(0x1015DFC0u))(a0); }
XI_ORIG _DWORD * MenuMng_OpenByName(void * a0, void * a1, char a2, char a3) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, char, char)>(XI_GAME(0x1015E350u))(a0, a1, a2, a3); }
XI_ORIG undefined8 MenuMng_OpenByName_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1015E350u))(a0, a1, a2, a3); }
XI_ORIG uint32_t MenuMng_OpenByName_v2(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1015E350u))(a0, a1, a2, a3); }
XI_ORIG unsigned long long CTkMenuMng_DestroyDrawMenu(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[2] = {a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1015E790u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined8 FUN_1015e830(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015E830u))(a0, a1); }
XI_ORIG unsigned long long FUN_1015e850(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1015E850u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_1015e850_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015E850u))(a0, a1); }
XI_ORIG unsigned long long FUN_1015e8a0(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1015E8A0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_1015e910(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015E910u))(a0, a1); }
XI_ORIG void FUN_1015eb60(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015EB60u))(a0, a1); }
XI_ORIG undefined4 FUN_1015ec50(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x1015EC50u))(a0); }
XI_ORIG undefined4 FUN_1015ecc0(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x1015ECC0u))(a0); }
XI_ORIG undefined4 FUN_1015ed80(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015ED80u))(a0, a1); }
XI_ORIG int FUN_1015edb0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1015EDB0u))(a0); }
XI_ORIG undefined8 FUN_1015ee00(XI_ANY a0) { return reinterpret_cast<undefined8 (__stdcall *)(XI_ANY)>(XI_GAME(0x1015EE00u))(a0); }
XI_ORIG undefined8 FUN_1015ee40() { return reinterpret_cast<undefined8 (__cdecl *)()>(XI_GAME(0x1015EE40u))(); }
XI_ORIG unsigned long long FUN_1015eea0(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1015EEA0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_1015ef20(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1015EF20u))(a0); }
XI_ORIG undefined4 FUN_1015f3e0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015F3E0u))(a0, a1); }
XI_ORIG int sub_1015F6A0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1015F6A0u))(a0); }
XI_ORIG int FUN_1015f980(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x1015F980u))(a0); }
XI_ORIG int FUN_1015f9e0(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1015F9E0u))(a0, a1); }
XI_ORIG int sub_1015FA00(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1015FA00u))(a0); }
XI_ORIG undefined8 FUN_101601d0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101601D0u))(a0, a1, a2); }
XI_ORIG void FUN_101601d0_v1(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101601D0u))(a0, a1, a2); }
XI_ORIG int MenuMng_GetUiScale(void * a0, void * a1, void * a2) { return reinterpret_cast<int (__thiscall *)(void *, void *, void *)>(XI_GAME(0x10160250u))(a0, a1, a2); }
XI_ORIG double MenuMng_GetOffsetX(void * a0, char a1) { return reinterpret_cast<double (__thiscall *)(void *, char)>(XI_GAME(0x10160290u))(a0, a1); }
XI_ORIG long double MenuMng_GetOffsetX_v1(void * a0, XI_ANY a1) { return reinterpret_cast<long double (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10160290u))(a0, a1); }
XI_ORIG double MenuMng_GetOffsetY(void * a0, char a1) { return reinterpret_cast<double (__thiscall *)(void *, char)>(XI_GAME(0x101602C0u))(a0, a1); }
XI_ORIG void CTkMenuMng_IsDrawWindow(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101602F0u))(a0, a1, a2, a3); }
XI_ORIG int ** sub_10160440(int a0, int a1, void * a2, int a3) { return reinterpret_cast<int ** (__fastcall *)(int, int, void *, int)>(XI_GAME(0x10160440u))(a0, a1, a2, a3); }
XI_ORIG int Gfx_SetDefault2DStates() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101604B0u))(); }
XI_ORIG void Gfx_SetDefault2DStates_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x101604B0u))(); }
XI_ORIG int Gfx_DisableFogAndAlphaBlend() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10160650u))(); }
XI_ORIG void Gfx_DisableFogAndAlphaBlend_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10160650u))(); }
XI_ORIG unsigned long long FUN_101606f0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101606F0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG int sub_101606F0(_DWORD a0) { return reinterpret_cast<int (__thiscall *)(_DWORD)>(XI_GAME(0x101606F0u))(a0); }
XI_ORIG unsigned long long FUN_10160700(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10160700u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_10160760(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10160760u))(a0, a1, a2); }
XI_ORIG void FUN_10160930() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10160930u))(); }
XI_ORIG void FUN_101609c0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x101609C0u))(); }
XI_ORIG void FUN_10160a50() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10160A50u))(); }
XI_ORIG undefined4 FUN_10160c90(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10160C90u))(a0); }
XI_ORIG void * FUN_10161110(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10161110u))(a0); }
XI_ORIG void * FUN_10161380(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10161380u))(a0); }
XI_ORIG void * FUN_101615e0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101615E0u))(a0); }
XI_ORIG void * FUN_10161ed0(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10161ED0u))(a0, a1); }
XI_ORIG unsigned long long FUN_10162510(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10162510u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_101626e0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101626E0u))(a0, a1); }
XI_ORIG undefined8 FUN_10162f30(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x10162F30u))(a0, a1); }
XI_ORIG undefined8 FUN_10162fb0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10162FB0u))(a0, a1); }
XI_ORIG undefined8 FUN_10162fe0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x10162FE0u))(a0, a1); }
XI_ORIG void Obj_10621944_Draw(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101630E0u))(a0); }
XI_ORIG void * FUN_10163350(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10163350u))(a0); }
XI_ORIG void FUN_10163380() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10163380u))(); }
XI_ORIG int Obj_10621944_DrawBgRect(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10163390u))(a0); }
XI_ORIG void FUN_101633d0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101633D0u))(a0); }
XI_ORIG void Gfx_DrawSolidRect2D(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x10163400u))(a0, a1); }
XI_ORIG char sub_10165070() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x10165070u))(); }
XI_ORIG signed int sub_101671D0() { return reinterpret_cast<signed int (__cdecl *)()>(XI_GAME(0x101671D0u))(); }
XI_ORIG void * FUN_10167440(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10167440u))(a0); }
XI_ORIG void Vt10336614_Destructor(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101674B0u))(a0); }
XI_ORIG void * FUN_10167800(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10167800u))(a0); }
XI_ORIG int Vt10336634_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101678D0u))(a0); }
XI_ORIG _DWORD * GetPtr8IfField60Set(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x10167940u))(a0); }
XI_ORIG void FUN_10167950(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10167950u))(a0); }
XI_ORIG void FUN_101679a0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101679A0u))(a0, a1); }
XI_ORIG int Dbg_DrawWireMesh(int a0, void * a1, int a2, float a3) { return reinterpret_cast<int (__stdcall *)(int, void *, int, float)>(XI_GAME(0x10168D20u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_10168fd0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10168FD0u))(a0); }
XI_ORIG undefined4 FUN_10169290(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10169290u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG uint32_t FUN_101693b0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101693B0u))(a0, a1, a2, a3); }
XI_ORIG uint32_t FUN_10169b40(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10169B40u))(a0, a1, a2, a3); }
XI_ORIG uint32_t FUN_1016a2d0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1016A2D0u))(a0, a1, a2, a3); }
XI_ORIG uint32_t FUN_1016a990(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1016A990u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_1016b1d0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1016B1D0u))(a0, a1, a2, a3); }
XI_ORIG float * Mat44_Multiply(void * a0, int a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, int, void *)>(XI_GAME(0x1016C060u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1016c150(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1016C150u))(a0); }
XI_ORIG undefined4 FUN_1016c390(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x1016C390u))(a0, a1); }
XI_ORIG undefined4 FUN_1016c5d0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1016C5D0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG undefined4 FUN_1016e7e0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1016E7E0u))(a0, a1, a2, a3); }
XI_ORIG void FUN_1016f320(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1016F320u))(a0, a1, a2); }
XI_ORIG void * FUN_1016fc40(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1016FC40u))(a0); }
XI_ORIG int Vt10336668_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1016FC80u))(a0); }
XI_ORIG void FUN_1016fc90(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1016FC90u))(a0); }
XI_ORIG float * sub_10170B30(void * a0, void * a1, void * a2, void * a3) { return reinterpret_cast<float * (__stdcall *)(void *, void *, void *, void *)>(XI_GAME(0x10170B30u))(a0, a1, a2, a3); }
XI_ORIG _DWORD * sub_10170C80(int a0) { return reinterpret_cast<_DWORD * (__thiscall *)(int)>(XI_GAME(0x10170C80u))(a0); }
XI_ORIG void FUN_10170cd0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10170CD0u))(a0, a1); }
XI_ORIG int sub_10170E80(int a0, int a1, int a2, int a3) { return reinterpret_cast<int (__stdcall *)(int, int, int, int)>(XI_GAME(0x10170E80u))(a0, a1, a2, a3); }
XI_ORIG int * FUN_10170f30(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10170F30u))(a0, a1, a2); }
XI_ORIG void * FUN_101718e0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101718E0u))(a0); }
XI_ORIG int Vt1033668C_Destructor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10171920u))(a0); }
XI_ORIG undefined4 FUN_10173ae0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x10173AE0u))(a0, a1); }
XI_ORIG void FUN_10174770(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10174770u))(a0); }
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
XI_ORIG int * sub_101773F0() { return reinterpret_cast<int * (__cdecl *)()>(XI_GAME(0x101773F0u))(); }
XI_ORIG int Gfx_SetZBias(unsigned int a0) { return reinterpret_cast<int (__cdecl *)(unsigned int)>(XI_GAME(0x10177400u))(a0); }
XI_ORIG int ZoneMap_SetOverlayProjection(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10177420u))(a0); }
XI_ORIG unsigned long long FUN_10177770(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) {
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
XI_ORIG undefined4 FUN_101779b0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101779B0u))(a0, a1, a2); }
XI_ORIG void XiZone_DrawMarkerForward(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10177CD0u))(a0, a1, a2, a3); }
XI_ORIG int sub_10177D00(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x10177D00u))(a0); }
XI_ORIG void FUN_10177fa0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10177FA0u))(a0); }
XI_ORIG void CTsZoneMap_InitZBiasPresets(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101781D0u))(a0); }
XI_ORIG void FUN_10178250(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10178250u))(a0); }
XI_ORIG void FUN_101784e0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x101784E0u))(); }
XI_ORIG void FUN_10178530(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10178530u))(a0, a1, a2, a3, a4); }
XI_ORIG int FUN_10178610(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10178610u))(a0, a1, a2, a3, a4); }
XI_ORIG _DWORD * ZoneMap_FindLight(int a0) { return reinterpret_cast<_DWORD * (__stdcall *)(int)>(XI_GAME(0x10178760u))(a0); }
XI_ORIG void FUN_10178790(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10178790u))(a0); }
XI_ORIG void FUN_101787c0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101787C0u))(a0); }
XI_ORIG void FUN_10178ac0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10178AC0u))(a0, a1); }
XI_ORIG int ZoneMap_ReleaseStateBlock(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10178B00u))(a0); }
XI_ORIG undefined4 * FUN_10178d90(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10178D90u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10178dd0(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x10178DD0u))(a0); }
XI_ORIG void FUN_10178f70(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10178F70u))(a0); }
XI_ORIG void ZoneMap_CreateVertexShaders(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101792A0u))(a0); }
XI_ORIG void FUN_1017a630(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1017A630u))(a0); }
XI_ORIG void FUN_1017a690(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x1017A690u))(a0); }
XI_ORIG undefined4 FUN_1017b110(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1017B110u))(a0, a1); }
XI_ORIG int ZoneMap_TeardownStep(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x1017B1E0u))(a0); }
XI_ORIG undefined4 ZoneMap_TeardownStep_v1(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1017B1E0u))(a0); }
XI_ORIG void XiZone_Draw(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1017B510u))(a0); }
XI_ORIG undefined4 FUN_10181210(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10181210u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined4 FUN_10181290(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10181290u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined4 FUN_10181300(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10181300u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_10181510(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10181510u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10181590(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10181590u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_101819b0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101819B0u))(a0, a1, a2, a3, a4); }
XI_ORIG BOOL sub_10181BB0(void * a0, void * a1) { return reinterpret_cast<BOOL (__stdcall *)(void *, void *)>(XI_GAME(0x10181BB0u))(a0, a1); }
XI_ORIG float * CTsZoneMap_DrawProjectedTexture(int a0, void * a1, int a2, void * a3, float a4) { return reinterpret_cast<float * (__thiscall *)(int, void *, int, void *, float)>(XI_GAME(0x10182700u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_10183370(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x10183370u))(a0, a1); }
XI_ORIG int FUN_10183420(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10183420u))(a0, a1); }
XI_ORIG float * XiZone_DrawModelAtMatrix(void * a0, void * a1, int a2) { return reinterpret_cast<float * (__thiscall *)(void *, void *, int)>(XI_GAME(0x10183490u))(a0, a1, a2); }
XI_ORIG char XiZone_DrawEffectElementModel(void * a0, void * a1, int a2, int a3, int a4) { return reinterpret_cast<char (__thiscall *)(void *, void *, int, int, int)>(XI_GAME(0x10183760u))(a0, a1, a2, a3, a4); }
XI_ORIG _DWORD * ZoneMap_DrawMeshById(int a0, int a1, int a2) { return reinterpret_cast<_DWORD * (__thiscall *)(int, int, int)>(XI_GAME(0x10183DA0u))(a0, a1, a2); }
XI_ORIG unsigned long long ModelActor_Draw(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[2] = {a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101844C0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 XiZone_DrawGroundShadowQuad(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10184C90u))(a0, a1, a2, a3); }
XI_ORIG undefined4 CTsZoneMap_DrawProjectedShadow(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10184FE0u))(a0, a1, a2); }
XI_ORIG undefined4 XiZone_DrawDecalOnTriangles(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10185CA0u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_10186d30(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10186D30u))(a0, a1, a2); }
XI_ORIG void FUN_10186db0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10186DB0u))(a0); }
XI_ORIG void FUN_10186dd0(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x10186DD0u))(a0); }
XI_ORIG void FUN_10186e70() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10186E70u))(); }
XI_ORIG char * sub_10186E90(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10186E90u))(a0); }
XI_ORIG int FUN_10186ea0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186EA0u))(a0); }
XI_ORIG char * sub_10186EB0(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10186EB0u))(a0); }
XI_ORIG int FUN_10186ec0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186EC0u))(a0); }
XI_ORIG int FUN_10186ee0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186EE0u))(a0); }
XI_ORIG int FUN_10186f00(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186F00u))(a0); }
XI_ORIG int FUN_10186f10(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186F10u))(a0); }
XI_ORIG char * Env_GetColorPtr84(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10186F20u))(a0); }
XI_ORIG int Env_GetColorPtr84_v1(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186F20u))(a0); }
XI_ORIG int FUN_10186f30(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186F30u))(a0); }
XI_ORIG int FUN_10186f40(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186F40u))(a0); }
XI_ORIG int FUN_10186f50(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186F50u))(a0); }
XI_ORIG long double FUN_10186f60(void * a0) { return reinterpret_cast<long double (__thiscall *)(void *)>(XI_GAME(0x10186F60u))(a0); }
XI_ORIG int FUN_10186f70(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186F70u))(a0); }
XI_ORIG int FUN_10186f80(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186F80u))(a0); }
XI_ORIG int FUN_10186f90(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186F90u))(a0); }
XI_ORIG char * Env_GetAmbientColorPtr(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(XI_GAME(0x10186FA0u))(a0); }
XI_ORIG int Env_GetAmbientColorPtr_v1(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186FA0u))(a0); }
XI_ORIG int FUN_10186fb0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186FB0u))(a0); }
XI_ORIG int FUN_10186fc0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10186FC0u))(a0); }
XI_ORIG long double FUN_10186fd0(void * a0) { return reinterpret_cast<long double (__thiscall *)(void *)>(XI_GAME(0x10186FD0u))(a0); }
XI_ORIG long double FUN_10186fe0(void * a0) { return reinterpret_cast<long double (__thiscall *)(void *)>(XI_GAME(0x10186FE0u))(a0); }
XI_ORIG double sub_10186FE0(void * a0) { return reinterpret_cast<double (__thiscall *)(void *)>(XI_GAME(0x10186FE0u))(a0); }
XI_ORIG long double FUN_10186ff0(void * a0) { return reinterpret_cast<long double (__thiscall *)(void *)>(XI_GAME(0x10186FF0u))(a0); }
XI_ORIG int sub_10187000(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10187000u))(a0); }
XI_ORIG long double FUN_10187020(void * a0) { return reinterpret_cast<long double (__thiscall *)(void *)>(XI_GAME(0x10187020u))(a0); }
XI_ORIG void FUN_10187030(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10187030u))(a0); }
XI_ORIG void FUN_10187050(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10187050u))(a0); }
XI_ORIG void * FUN_10187380(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10187380u))(a0); }
XI_ORIG void FUN_10187470(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10187470u))(a0); }
XI_ORIG uint32_t FUN_101874a0(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x101874A0u))(a0); }
XI_ORIG unsigned long long FUN_101875c0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101875C0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_101875e0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101875E0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_10187600(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10187600u))(a0); }
XI_ORIG void FUN_10187720(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10187720u))(a0, a1); }
XI_ORIG void FUN_101880c0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101880C0u))(a0, a1); }
XI_ORIG void FUN_10188190(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10188190u))(a0, a1); }
XI_ORIG int FUN_10188900() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10188900u))(); }
XI_ORIG void FUN_10188910(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10188910u))(a0); }
XI_ORIG void FUN_10188940() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x10188940u))(); }
XI_ORIG int FUN_101889a0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101889A0u))(); }
XI_ORIG int FUN_101889c0(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x101889C0u))(a0); }
XI_ORIG unsigned long long FUN_101889e0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101889E0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_101889e0_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101889E0u))(a0); }
XI_ORIG undefined4 FUN_10188a10(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10188A10u))(a0); }
XI_ORIG void FUN_10188a10_v1(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10188A10u))(a0); }
XI_ORIG uint32_t FUN_10188a50(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x10188A50u))(a0); }
XI_ORIG _DWORD * XiZone_GetDirLights(int a0, void * a1, void * a2) { return reinterpret_cast<_DWORD * (__thiscall *)(int, void *, void *)>(XI_GAME(0x10188B70u))(a0, a1, a2); }
XI_ORIG void XiZone_GetDirLights_v1(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10188B70u))(a0, a1, a2); }
XI_ORIG void FUN_10188ca0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10188CA0u))(a0, a1, a2); }
XI_ORIG int sub_10188CA0(int a0, void * a1, int a2) { return reinterpret_cast<int (__thiscall *)(int, void *, int)>(XI_GAME(0x10188CA0u))(a0, a1, a2); }
XI_ORIG _DWORD * Env_ComputeAmbient(void * a0, void * a1, int a2) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, int)>(XI_GAME(0x10188E00u))(a0, a1, a2); }
XI_ORIG void Env_ComputeAmbient_v1(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10188E00u))(a0, a1, a2); }
XI_ORIG void FUN_10189120(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10189120u))(a0, a1, a2, a3, a4); }
XI_ORIG void sub_10189120(int a0, void * a1, void * a2, void * a3, int a4) { reinterpret_cast<void (__thiscall *)(int, void *, void *, void *, int)>(XI_GAME(0x10189120u))(a0, a1, a2, a3, a4); }
XI_ORIG long double FUN_101892b0(void * a0, XI_ANY a1) { return reinterpret_cast<long double (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101892B0u))(a0, a1); }
XI_ORIG void FUN_10189490(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10189490u))(a0, a1, a2, a3); }
XI_ORIG void FUN_10189750(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10189750u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_10189810(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10189810u))(a0, a1, a2, a3); }
XI_ORIG void FUN_101898e0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101898E0u))(a0, a1, a2, a3); }
XI_ORIG void FUN_10189980(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10189980u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_10189bd0(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x10189BD0u))(a0); }
XI_ORIG void FUN_10189c60(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x10189C60u))(a0); }
XI_ORIG void FUN_10189ca0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10189CA0u))(a0, a1, a2); }
XI_ORIG void * FUN_10189e30(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10189E30u))(a0, a1); }
XI_ORIG void FUN_10189f20(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10189F20u))(a0, a1, a2); }
XI_ORIG void * FUN_1018a0f0(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018A0F0u))(a0, a1); }
XI_ORIG void FUN_1018a220(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018A220u))(a0); }
XI_ORIG void FUN_1018a260(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018A260u))(a0); }
XI_ORIG void FUN_1018a2a0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018A2A0u))(a0); }
XI_ORIG undefined4 FUN_1018a2c0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1018A2C0u))(a0); }
XI_ORIG void FUN_1018a2e0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018A2E0u))(a0, a1); }
XI_ORIG void FUN_1018a2f0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018A2F0u))(a0, a1); }
XI_ORIG void FUN_1018a300(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018A300u))(a0, a1); }
XI_ORIG undefined4 FUN_1018a310(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018A310u))(a0, a1); }
XI_ORIG void FUN_1018a320(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018A320u))(a0); }
XI_ORIG void FUN_1018a340(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018A340u))(a0); }
XI_ORIG unsigned long long FUN_1018aac0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1018AAC0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_1018ab40(XI_ANY a0, XI_ANY a1) {
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
XI_ORIG undefined4 FUN_1018ad50(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1018AD50u))(a0); }
XI_ORIG void FUN_1018ad60(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018AD60u))(a0, a1); }
XI_ORIG void * FUN_1018ad80(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1018AD80u))(a0); }
XI_ORIG void FUN_1018ada0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018ADA0u))(a0); }
XI_ORIG void thunk_FUN_1018ade0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018ADD0u))(a0); }
XI_ORIG void FUN_1018ade0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018ADE0u))(a0); }
XI_ORIG undefined4 FUN_1018aeb0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x1018AEB0u))(a0, a1); }
XI_ORIG void FUN_1018b130() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1018B130u))(); }
XI_ORIG _DWORD * sub_1018B130() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x1018B130u))(); }
XI_ORIG void * FUN_1018b170(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1018B170u))(a0); }
XI_ORIG void FUN_1018b620(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018B620u))(a0); }
XI_ORIG int sub_1018B860(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1018B860u))(a0); }
XI_ORIG void FUN_1018b8b0(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x1018B8B0u))(a0); }
XI_ORIG _DWORD * sub_1018B960(void * a0, void * a1, void * a2, int a3) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, void *, int)>(XI_GAME(0x1018B960u))(a0, a1, a2, a3); }
XI_ORIG int XiZone_GetDirLight(void * a0, void * a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, void *, int, int)>(XI_GAME(0x1018B9A0u))(a0, a1, a2, a3); }
XI_ORIG int XiZone_GetAmbient(void * a0, void * a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, void *, int, int)>(XI_GAME(0x1018B9E0u))(a0, a1, a2, a3); }
XI_ORIG void XiZone_GetAmbient_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1018B9E0u))(a0, a1, a2, a3); }
XI_ORIG void XiZone_GetFog(void * a0, void * a1, void * a2, void * a3, int a4, int a5) { reinterpret_cast<void (__thiscall *)(void *, void *, void *, void *, int, int)>(XI_GAME(0x1018BA20u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void XiZone_GetFog_v1(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1018BA20u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void FUN_1018bae0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018BAE0u))(a0, a1); }
XI_ORIG undefined4 unknown_libname_158(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1018BBB0u))(a0); }
XI_ORIG void * FUN_1018bbc0(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018BBC0u))(a0, a1); }
XI_ORIG unsigned long long FUN_1018bc40(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1018BC40u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG int FUN_1018bd30(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1018BD30u))(a0); }
XI_ORIG void FUN_1018bf00(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018BF00u))(a0, a1); }
XI_ORIG void FUN_1018bf50(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018BF50u))(a0, a1); }
XI_ORIG void FUN_1018bfa0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018BFA0u))(a0, a1); }
XI_ORIG void FUN_1018c180(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1018C180u))(a0); }
XI_ORIG undefined4 FUN_1018c290(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1018C290u))(a0); }
XI_ORIG unsigned int sub_1018C2B0() { return reinterpret_cast<unsigned int (__cdecl *)()>(XI_GAME(0x1018C2B0u))(); }
XI_ORIG char CXiSchStatus_Destructor(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x1018C620u))(a0); }
XI_ORIG int Sch_GetStatusEntry(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x1018CA20u))(a0); }
XI_ORIG void FUN_1018cea0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018CEA0u))(a0); }
XI_ORIG uint32_t FUN_1018d230(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1018D230u))(a0, a1); }
XI_ORIG undefined4 FUN_1018d250(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1018D250u))(a0); }
XI_ORIG void FUN_1018d280(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018D280u))(a0); }
XI_ORIG void * FUN_1018d2b0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1018D2B0u))(a0); }
XI_ORIG void * FUN_1018d2d0(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018D2D0u))(a0, a1); }
XI_ORIG void FUN_1018d3d0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018D3D0u))(a0); }
XI_ORIG void FUN_1018d430(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018D430u))(a0, a1); }
XI_ORIG void FUN_1018d440(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018D440u))(a0, a1); }
XI_ORIG void FUN_1018d450(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018D450u))(a0, a1); }
XI_ORIG undefined1 * FUN_1018d470(void * a0) { return reinterpret_cast<undefined1 * (__thiscall *)(void *)>(XI_GAME(0x1018D470u))(a0); }
XI_ORIG undefined4 FUN_1018d480(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1018D480u))(a0); }
XI_ORIG undefined4 FUN_1018d490(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1018D490u))(a0); }
XI_ORIG void * FUN_1018d4a0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1018D4A0u))(a0, a1, a2); }
XI_ORIG void * FUN_1018d580(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1018D580u))(a0, a1, a2); }
XI_ORIG void * FUN_1018d640(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1018D640u))(a0); }
XI_ORIG int FUN_1018d7f0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1018D7F0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_1018d980(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1018D980u))(a0); }
XI_ORIG undefined4 FUN_1018db10(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1018DB10u))(a0, a1, a2, a3); }
XI_ORIG int FUN_1018dc70(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018DC70u))(a0, a1); }
XI_ORIG void FUN_1018dcb0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018DCB0u))(a0); }
XI_ORIG void * FUN_1018dcd0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1018DCD0u))(a0); }
XI_ORIG undefined4 FUN_1018e0a0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018E0A0u))(a0, a1); }
XI_ORIG void FUN_1018e0e0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018E0E0u))(a0); }
XI_ORIG void FUN_1018e170(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1018E170u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1018e1a0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1018E1A0u))(a0); }
XI_ORIG undefined4 FsLogPage_load(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018E1D0u))(a0, a1); }
XI_ORIG undefined4 FUN_1018e310(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018E310u))(a0, a1); }
XI_ORIG undefined4 FUN_1018e4d0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018E4D0u))(a0, a1); }
XI_ORIG void FUN_1018e660(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018E660u))(a0); }
XI_ORIG void FUN_1018e6a0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1018E6A0u))(a0); }
XI_ORIG void * FUN_1018e6f0(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018E6F0u))(a0, a1); }
XI_ORIG undefined4 FUN_1018e7f0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018E7F0u))(a0, a1); }
XI_ORIG int FUN_1018e800(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018E800u))(a0, a1); }
XI_ORIG undefined4 FUN_1018e8d0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018E8D0u))(a0, a1); }
XI_ORIG void * FUN_1018e9b0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1018E9B0u))(a0); }
XI_ORIG void * FUN_1018ea00(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1018EA00u))(a0); }
XI_ORIG void FUN_1018ea50(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1018EA50u))(a0, a1); }
XI_ORIG undefined4 FUN_1018eb00(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1018EB00u))(a0); }
XI_ORIG undefined4 FUN_1018fa20(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1018FA20u))(a0, a1); }
XI_ORIG undefined4 FUN_1018fa80(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1018FA80u))(a0, a1); }
XI_ORIG int FUN_1018fb00(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1018FB00u))(a0, a1, a2); }
XI_ORIG unsigned long long FUN_1018fd90(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[2] = {a0.bits, a1.bits}; uint32_t ra = a2.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $8, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1018FD90u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG int FUN_1018fdf0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1018FDF0u))(a0, a1, a2); }
XI_ORIG void FUN_101907a0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101907A0u))(a0, a1); }
XI_ORIG undefined8 operator_delete_101907D0(XI_ANY a0) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY)>(XI_GAME(0x101907D0u))(a0); }
XI_ORIG void FUN_101908d0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x101908D0u))(); }
XI_ORIG void FUN_10190bb0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10190BB0u))(a0); }
XI_ORIG undefined4 FUN_10193800(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10193800u))(a0, a1, a2); }
XI_ORIG undefined4 SetConfig(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10193830u))(a0, a1); }
XI_ORIG int Settings_GetValue(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10193850u))(a0); }
XI_ORIG undefined8 Settings_GetValue_v1(XI_ANY a0) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY)>(XI_GAME(0x10193850u))(a0); }
XI_ORIG undefined4 Settings_GetValue_v2(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10193850u))(a0); }
XI_ORIG undefined4 FUN_10193890(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10193890u))(a0); }
XI_ORIG undefined4 FUN_101938b0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101938B0u))(a0, a1); }
XI_ORIG undefined4 FUN_101938d0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101938D0u))(a0, a1); }
XI_ORIG undefined8 FUN_10193a10(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10193A10u))(a0, a1); }
XI_ORIG undefined4 FUN_10193b00(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10193B00u))(a0, a1, a2); }
XI_ORIG void FUN_10193b60(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10193B60u))(a0, a1, a2); }
XI_ORIG void FUN_10193bc0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10193BC0u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10195220(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10195220u))(a0); }
XI_ORIG undefined4 FUN_10195240(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10195240u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_101952c0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101952C0u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10195330(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10195330u))(a0, a1); }
XI_ORIG undefined4 FUN_10195350(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10195350u))(a0); }
XI_ORIG void FUN_101953a0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101953A0u))(a0); }
XI_ORIG undefined4 FUN_101953c0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101953C0u))(a0); }
XI_ORIG undefined4 FUN_101953d0() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x101953D0u))(); }
XI_ORIG undefined4 FUN_101953f0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101953F0u))(a0); }
XI_ORIG void FUN_10195460(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10195460u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG undefined8 FUN_101954d0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101954D0u))(a0, a1); }
XI_ORIG undefined4 FUN_10195570(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10195570u))(a0); }
XI_ORIG undefined4 FUN_10195640(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10195640u))(a0); }
XI_ORIG undefined4 FUN_101956f0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101956F0u))(a0); }
XI_ORIG undefined4 FUN_10195780(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x10195780u))(a0); }
XI_ORIG undefined4 FUN_101957b0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x101957B0u))(a0, a1); }
XI_ORIG undefined * FUN_10195810(XI_ANY a0) { return reinterpret_cast<undefined * (__stdcall *)(XI_ANY)>(XI_GAME(0x10195810u))(a0); }
XI_ORIG int FUN_101958c0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x101958C0u))(a0, a1); }
XI_ORIG undefined4 FUN_10195900(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10195900u))(a0); }
XI_ORIG undefined4 FUN_101959a0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101959A0u))(a0); }
XI_ORIG undefined4 FUN_10196040(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x10196040u))(a0, a1); }
XI_ORIG void FUN_10196260(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10196260u))(a0, a1); }
XI_ORIG void FUN_101962d0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101962D0u))(a0); }
XI_ORIG void * FUN_10196980(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10196980u))(a0); }
XI_ORIG undefined4 FUN_10196be0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10196BE0u))(); }
XI_ORIG void * FUN_10197a90(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10197A90u))(a0); }
XI_ORIG void * FUN_10199710(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10199710u))(a0); }
XI_ORIG int sub_1019C3E0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1019C3E0u))(); }
XI_ORIG void FUN_1019c4f0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1019C4F0u))(); }
XI_ORIG undefined4 FUN_1019c580() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1019C580u))(); }
XI_ORIG undefined4 FUN_1019c590() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1019C590u))(); }
XI_ORIG int sub_1019C5B0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1019C5B0u))(); }
XI_ORIG void * FUN_1019cd40(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1019CD40u))(a0); }
XI_ORIG unsigned long long FUN_1019cdc0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1019CDC0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_1019ce70(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1019CE70u))(a0); }
XI_ORIG unsigned long long FUN_1019cf60(XI_ANY a0, XI_ANY a1) {
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
XI_ORIG void FUN_1019d1c0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1019D1C0u))(a0); }
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
XI_ORIG void FUN_1019d8b0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1019D8B0u))(a0); }
XI_ORIG undefined4 FUN_1019ead0() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x1019EAD0u))(); }
XI_ORIG int sub_1019EBB0(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(XI_GAME(0x1019EBB0u))(a0); }
XI_ORIG int FUN_1019f8b0(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1019F8B0u))(a0, a1); }
XI_ORIG undefined4 FUN_1019f930(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1019F930u))(a0, a1); }
XI_ORIG undefined8 FUN_1019f960(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1019F960u))(a0, a1); }
XI_ORIG int FUN_1019fa90(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1019FA90u))(a0); }
XI_ORIG int FUN_1019fab0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1019FAB0u))(a0); }
XI_ORIG int FsFileThreadManager_handleFileEvent(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1019FD50u))(a0, a1); }
XI_ORIG int FUN_101a0910(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101A0910u))(a0); }
XI_ORIG int FUN_101a0930(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A0930u))(a0, a1); }
XI_ORIG undefined8 FUN_101a0950(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x101A0950u))(a0); }
XI_ORIG undefined8 FUN_101a0970(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x101A0970u))(a0); }
XI_ORIG undefined4 FUN_101a0be0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A0BE0u))(a0, a1); }
XI_ORIG undefined4 FUN_101a0d30(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A0D30u))(a0, a1); }
XI_ORIG void * FUN_101a1400(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101A1400u))(a0); }
XI_ORIG void FUN_101a1460(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101A1460u))(a0); }
XI_ORIG undefined4 FUN_101a1470(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A1470u))(a0, a1, a2); }
XI_ORIG int FUN_101a1500(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A1500u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_101a1580(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A1580u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_101a1660(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101A1660u))(a0); }
XI_ORIG undefined4 FUN_101a1690(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101A1690u))(a0); }
XI_ORIG undefined4 FUN_101a16b0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101A16B0u))(a0); }
XI_ORIG void * FUN_101a16e0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101A16E0u))(a0); }
XI_ORIG void FUN_101a1740(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A1740u))(a0, a1); }
XI_ORIG int FUN_101a1910(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A1910u))(a0, a1); }
XI_ORIG int FUN_101a19b0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101A19B0u))(); }
XI_ORIG void * FUN_101a1a20(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101A1A20u))(a0); }
XI_ORIG undefined4 FUN_101a1c60(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A1C60u))(a0, a1); }
XI_ORIG void FUN_101a1cb0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101A1CB0u))(a0); }
XI_ORIG undefined4 FUN_101a1cc0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101A1CC0u))(a0); }
XI_ORIG undefined FUN_101a23d0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x101A23D0u))(); }
XI_ORIG undefined4 FUN_101a2480(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101A2480u))(a0, a1, a2); }
XI_ORIG void FUN_101a2660(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A2660u))(a0, a1, a2); }
XI_ORIG void FUN_101a26c0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A26C0u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_101a2890(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A2890u))(a0, a1); }
XI_ORIG undefined4 FUN_101a2e00(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A2E00u))(a0, a1); }
XI_ORIG void FUN_101a2e60(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A2E60u))(a0, a1, a2); }
XI_ORIG char sub_101A2E80(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x101A2E80u))(a0); }
XI_ORIG undefined4 FUN_101a3300(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A3300u))(a0, a1, a2); }
XI_ORIG unsigned long long FUN_101a33e0(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101A33E0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_101a3500(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101A3500u))(a0, a1, a2, a3); }
XI_ORIG unsigned long long FUN_101a3550(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101A3550u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void * FUN_101a36f0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A36F0u))(a0, a1, a2); }
XI_ORIG void FUN_101a3cf0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101A3CF0u))(a0); }
XI_ORIG undefined4 FUN_101a3d00(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A3D00u))(a0, a1); }
XI_ORIG void * FUN_101a3dc0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A3DC0u))(a0, a1, a2); }
XI_ORIG void FUN_101a43a0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101A43A0u))(a0); }
XI_ORIG undefined4 FUN_101a43b0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A43B0u))(a0, a1); }
XI_ORIG undefined4 FUN_101a4440(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A4440u))(a0, a1); }
XI_ORIG void * FUN_101a46c0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A46C0u))(a0, a1, a2); }
XI_ORIG void * FUN_101a4d70(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A4D70u))(a0, a1, a2); }
XI_ORIG void * FUN_101a54f0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101A54F0u))(a0, a1, a2); }
XI_ORIG void * FUN_101a6430(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101A6430u))(a0); }
XI_ORIG void * FUN_101a6c10(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A6C10u))(a0, a1); }
XI_ORIG void * FUN_101a7280(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A7280u))(a0, a1); }
XI_ORIG void * FUN_101a83c0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101A83C0u))(a0); }
XI_ORIG void FUN_101a8410(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A8410u))(a0, a1); }
XI_ORIG undefined4 FUN_101a8750(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101A8750u))(a0, a1); }
XI_ORIG _BYTE * sub_101A8B50() { return reinterpret_cast<_BYTE * (__cdecl *)()>(XI_GAME(0x101A8B50u))(); }
XI_ORIG _DWORD * sub_101A8D40() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x101A8D40u))(); }
XI_ORIG int sub_101A8D60() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101A8D60u))(); }
XI_ORIG Iostream_init * sub_101AB080() { return reinterpret_cast<Iostream_init * (__cdecl *)()>(XI_GAME(0x101AB080u))(); }
XI_ORIG char * sub_101AB0D0() { return reinterpret_cast<char * (__cdecl *)()>(XI_GAME(0x101AB0D0u))(); }
XI_ORIG void FUN_101ab730() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x101AB730u))(); }
XI_ORIG char sub_101AB770() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x101AB770u))(); }
XI_ORIG void * FUN_101ab810(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101AB810u))(a0); }
XI_ORIG void sub_101AB820(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101AB820u))(a0); }
XI_ORIG void FUN_101abb40(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101ABB40u))(a0, a1); }
XI_ORIG undefined4 FUN_101abc50(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101ABC50u))(a0, a1, a2); }
XI_ORIG unsigned long long FUN_101abe40(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101ABE40u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_101abe50(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101ABE50u))(a0); }
XI_ORIG undefined4 FUN_101abe80(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101ABE80u))(a0, a1); }
XI_ORIG void FUN_101abf40(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101ABF40u))(a0, a1); }
XI_ORIG int FUN_101ac1b0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101AC1B0u))(a0); }
XI_ORIG undefined4 FUN_101ac200(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101AC200u))(a0); }
XI_ORIG undefined4 FUN_101ac580(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101AC580u))(a0); }
XI_ORIG uint32_t FUN_101ac5e0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101AC5E0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG int FUN_101ac990(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101AC990u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int FUN_101acac0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101ACAC0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG uint32_t FUN_101acc30(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101ACC30u))(a0, a1); }
XI_ORIG void _ZN5CRGBAC2Ehhhh(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101ACF40u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_101acf60(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101ACF60u))(a0); }
XI_ORIG void FUN_101acf70(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101ACF70u))(a0, a1); }
XI_ORIG undefined4 FUN_101acf90(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101ACF90u))(a0); }
XI_ORIG undefined4 FUN_101acfb0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101ACFB0u))(a0); }
XI_ORIG undefined4 FUN_101ad1b0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x101AD1B0u))(a0, a1); }
XI_ORIG int FUN_101ad820(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101AD820u))(a0); }
XI_ORIG undefined4 FUN_101adbc0(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x101ADBC0u))(a0); }
XI_ORIG undefined4 FUN_101adbd0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101ADBD0u))(a0, a1); }
XI_ORIG undefined4 FUN_101adbe0(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x101ADBE0u))(a0); }
XI_ORIG undefined4 FUN_101addc0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101ADDC0u))(a0, a1); }
XI_ORIG undefined4 FUN_101adf20(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101ADF20u))(a0, a1); }
XI_ORIG undefined4 FsShortcut_CanUseShortcut(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101AE010u))(a0); }
XI_ORIG int sub_101AE3E0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101AE3E0u))(a0); }
XI_ORIG void FUN_101aff30() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x101AFF30u))(); }
XI_ORIG HIMC ImmGetContext(XI_ANY a0) { return reinterpret_cast<HIMC (__stdcall *)(XI_ANY)>(XI_GAME(0x101AFF50u))(a0); }
XI_ORIG BOOL ImmReleaseContext(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<BOOL (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x101AFF60u))(a0, a1); }
XI_ORIG void FUN_101b36f0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101B36F0u))(a0, a1); }
XI_ORIG void FUN_101b45b0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101B45B0u))(a0); }
XI_ORIG void FUN_101b45e0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101B45E0u))(a0); }
XI_ORIG void * FUN_101b4710(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101B4710u))(a0); }
XI_ORIG void FUN_101b4f90(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101B4F90u))(a0); }
XI_ORIG undefined4 FUN_101b5c90(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101B5C90u))(a0); }
XI_ORIG undefined4 FUN_101b5cc0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101B5CC0u))(a0); }
XI_ORIG int FUN_101b6810(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101B6810u))(a0, a1); }
XI_ORIG unsigned long long FUN_101b6d50(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[2] = {a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101B6D50u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_101b6e90(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101B6E90u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void * FUN_101b6ec0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101B6EC0u))(a0); }
XI_ORIG undefined4 FUN_101b6ee0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101B6EE0u))(a0, a1, a2); }
XI_ORIG void FUN_101b7190(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101B7190u))(a0); }
XI_ORIG undefined4 FUN_101b7280(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101B7280u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG undefined4 FUN_101b72b0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101B72B0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_101b72e0(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x101B72E0u))(a0); }
XI_ORIG uint32_t FUN_101b7ee0() { return reinterpret_cast<uint32_t (__cdecl *)()>(XI_GAME(0x101B7EE0u))(); }
XI_ORIG int sub_101B8E70() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101B8E70u))(); }
XI_ORIG undefined8 FUN_101b9530(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x101B9530u))(a0); }
XI_ORIG undefined8 FUN_101b9550(void * a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101B9550u))(a0, a1); }
XI_ORIG int FUN_101b95d0(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101B95D0u))(a0, a1); }
XI_ORIG int FUN_101b9610(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101B9610u))(a0); }
XI_ORIG undefined4 FUN_101b9650(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101B9650u))(a0); }
XI_ORIG int FsTinyString_init(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101B9690u))(a0); }
XI_ORIG void FUN_101b9880(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101B9880u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG void FUN_101b9980(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101B9980u))(a0, a1); }
XI_ORIG void FUN_101b9a00(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101B9A00u))(a0); }
XI_ORIG undefined4 FUN_101b9a10(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101B9A10u))(a0); }
XI_ORIG undefined4 FUN_101b9bc0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101B9BC0u))(a0, a1); }
XI_ORIG void * FUN_101b9e00(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101B9E00u))(a0, a1); }
XI_ORIG int FUN_101b9e60() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101B9E60u))(); }
XI_ORIG undefined4 FUN_101b9ec0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101B9EC0u))(a0, a1); }
XI_ORIG void FUN_101b9f20(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101B9F20u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG undefined4 FUN_101b9ff0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101B9FF0u))(a0); }
XI_ORIG undefined4 FUN_101ba030(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BA030u))(a0); }
XI_ORIG undefined4 FUN_101ba180(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BA180u))(a0); }
XI_ORIG void * FUN_101ba280(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101BA280u))(a0); }
XI_ORIG void FUN_101ba2d0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101BA2D0u))(a0); }
XI_ORIG void FUN_101ba300(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101BA300u))(a0); }
XI_ORIG void * FUN_101ba570(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101BA570u))(a0); }
XI_ORIG void FUN_101ba5b0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101BA5B0u))(a0); }
XI_ORIG undefined4 FUN_101ba5c0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BA5C0u))(a0); }
XI_ORIG undefined4 FUN_101badd0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BADD0u))(a0); }
XI_ORIG undefined4 FUN_101bae20(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BAE20u))(a0); }
XI_ORIG undefined4 FUN_101bae30(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BAE30u))(a0); }
XI_ORIG undefined4 FUN_101baeb0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BAEB0u))(a0); }
XI_ORIG undefined4 FUN_101baec0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BAEC0u))(a0); }
XI_ORIG void FUN_101baf90(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101BAF90u))(a0); }
XI_ORIG void FUN_101bafc0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101BAFC0u))(a0, a1); }
XI_ORIG int FUN_101bb070(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101BB070u))(a0, a1); }
XI_ORIG uint32_t FUN_101bb100(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x101BB100u))(a0); }
XI_ORIG undefined4 FUN_101bb2b0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BB2B0u))(a0); }
XI_ORIG void FUN_101bb4b0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101BB4B0u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_101bb680(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BB680u))(a0); }
XI_ORIG uint32_t FUN_101bb740(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x101BB740u))(a0); }
XI_ORIG undefined4 FUN_101bb7a0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BB7A0u))(a0); }
XI_ORIG undefined1 FUN_101bb9e0(void * a0) { return reinterpret_cast<undefined1 (__thiscall *)(void *)>(XI_GAME(0x101BB9E0u))(a0); }
XI_ORIG undefined4 FUN_101bba90(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101BBA90u))(a0, a1, a2, a3); }
XI_ORIG void FUN_101bbc60(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101BBC60u))(a0); }
XI_ORIG undefined4 FUN_101bbe50(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BBE50u))(a0); }
XI_ORIG undefined4 FsWordCompletion_getEntryList(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101BBE60u))(a0, a1, a2); }
XI_ORIG unsigned long long FsWordCompletion_lookUp(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) {
    uint32_t xs[3] = {a1.bits, a2.bits, a3.bits}; uint32_t ra = a4.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101BC100u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FsWordCompletion__prepareAllData(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101BC520u))(a0, a1); }
XI_ORIG undefined4 FUN_101bc6f0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BC6F0u))(a0); }
XI_ORIG undefined4 FUN_101bc720(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BC720u))(a0); }
XI_ORIG void FUN_101bc740(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101BC740u))(a0); }
XI_ORIG uint32_t FUN_101bc7b0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101BC7B0u))(a0, a1, a2, a3); }
XI_ORIG int FUN_101bc900(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101BC900u))(a0, a1); }
XI_ORIG undefined4 FUN_101bc9b0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BC9B0u))(a0); }
XI_ORIG void * FsMemory_Constructor(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101BC9E0u))(a0, a1); }
XI_ORIG void FUN_101bca30(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101BCA30u))(a0); }
XI_ORIG undefined4 * FUN_101bca40(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101BCA40u))(a0, a1, a2); }
XI_ORIG int FUN_101bcac0(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101BCAC0u))(a0, a1); }
XI_ORIG void FUN_101bcb30(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101BCB30u))(a0); }
XI_ORIG undefined4 FUN_101bcba0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101BCBA0u))(a0); }
XI_ORIG char sub_101BCE30(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x101BCE30u))(a0); }
XI_ORIG void * FUN_101bcef0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101BCEF0u))(a0); }
XI_ORIG void FUN_101bcf30(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101BCF30u))(a0); }
XI_ORIG void * FUN_101be260(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101BE260u))(a0); }
XI_ORIG void FUN_101be2a0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101BE2A0u))(a0); }
XI_ORIG void FUN_101be2b0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101BE2B0u))(a0, a1); }
XI_ORIG void FUN_101be2c0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101BE2C0u))(a0, a1); }
XI_ORIG void FUN_101be330(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101BE330u))(a0, a1); }
XI_ORIG void FUN_101be3a0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101BE3A0u))(a0, a1); }
XI_ORIG void FUN_101be410(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101BE410u))(a0); }
XI_ORIG undefined4 FUN_101be470(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101BE470u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_101be500(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x101BE500u))(a0); }
XI_ORIG undefined4 FUN_101be510(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x101BE510u))(a0); }
XI_ORIG int FUN_101be590(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__stdcall *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101BE590u))(a0, a1, a2); }
XI_ORIG int FUN_101be670(XI_ANY a0) { return reinterpret_cast<int (__stdcall *)(XI_ANY)>(XI_GAME(0x101BE670u))(a0); }
XI_ORIG undefined4 FUN_101be690(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x101BE690u))(a0); }
XI_ORIG undefined4 FUN_101be6c0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x101BE6C0u))(a0, a1); }
XI_ORIG undefined4 FUN_101be730(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x101BE730u))(a0); }
XI_ORIG undefined4 FUN_101beb90() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x101BEB90u))(); }
XI_ORIG int FUN_101becc0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101BECC0u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int FUN_101bf280(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101BF280u))(a0, a1, a2, a3, a4); }
XI_ORIG int FUN_101bf370(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101BF370u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int FUN_101bf460(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101BF460u))(a0, a1, a2, a3, a4); }
XI_ORIG int FUN_101bf550(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101BF550u))(a0, a1, a2, a3, a4); }
XI_ORIG int FUN_101bf970(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101BF970u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int FUN_101c0050(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C0050u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int FUN_101c0160(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C0160u))(a0, a1, a2, a3, a4); }
XI_ORIG int FUN_101c0260(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C0260u))(a0, a1, a2, a3, a4); }
XI_ORIG int FUN_101c0440(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C0440u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int FUN_101c0540(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C0540u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined8 FUN_101c0630(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C0630u))(a0, a1, a2, a3, a4); }
XI_ORIG int FUN_101c0760(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C0760u))(a0, a1, a2, a3, a4); }
XI_ORIG int FUN_101c09b0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101C09B0u))(a0); }
XI_ORIG int FUN_101c0a40(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C0A40u))(a0, a1, a2, a3, a4); }
XI_ORIG int FUN_101c0fc0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C0FC0u))(a0, a1, a2); }
XI_ORIG int FUN_101c1090(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C1090u))(a0, a1); }
XI_ORIG uint32_t FUN_101c11f0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C11F0u))(a0, a1, a2, a3); }
XI_ORIG uint32_t FUN_101c13a0(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C13A0u))(a0, a1); }
XI_ORIG uint32_t FUN_101c1750(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C1750u))(a0, a1, a2); }
XI_ORIG uint32_t FUN_101c1820(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C1820u))(a0, a1); }
XI_ORIG uint32_t FUN_101c18e0(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C18E0u))(a0, a1); }
XI_ORIG undefined4 FUN_101c1a40() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x101C1A40u))(); }
XI_ORIG void FUN_101c1b10() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x101C1B10u))(); }
XI_ORIG int FUN_101c1b20(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C1B20u))(a0, a1); }
XI_ORIG int FUN_101c1c90(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C1C90u))(a0, a1); }
XI_ORIG int FUN_101c1df0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C1DF0u))(a0, a1, a2); }
XI_ORIG int FUN_101c1ec0(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C1EC0u))(a0, a1); }
XI_ORIG int FUN_101c2450(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C2450u))(a0, a1, a2); }
XI_ORIG void FUN_101c2670() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x101C2670u))(); }
XI_ORIG undefined4 FUN_101c2840(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C2840u))(a0, a1); }
XI_ORIG undefined4 FUN_101c2b60(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C2B60u))(a0, a1); }
XI_ORIG undefined4 FUN_101c2c80(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101C2C80u))(a0); }
XI_ORIG void thunk_FUN_101c4170(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C2D80u))(a0, a1); }
XI_ORIG undefined4 FUN_101c2d90() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x101C2D90u))(); }
XI_ORIG int FUN_101c2e70(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101C2E70u))(a0); }
XI_ORIG void FUN_101c33e0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101C33E0u))(a0); }
XI_ORIG undefined4 FUN_101c33f0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101C33F0u))(a0); }
XI_ORIG undefined4 XiFileImpl_getFileName(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C3A90u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_101c3d00(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C3D00u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_101c3db0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C3DB0u))(a0, a1, a2); }
XI_ORIG undefined8 FUN_101c3de0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C3DE0u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_101c3e10(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C3E10u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_101c3e90(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C3E90u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_101c3f40(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C3F40u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_101c3fb0(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x101C3FB0u))(a0); }
XI_ORIG unsigned long long FUN_101c3fe0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101C3FE0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_101c4170(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C4170u))(a0, a1); }
XI_ORIG void sub_101C46A0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101C46A0u))(a0); }
XI_ORIG undefined4 FUN_101c46d0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C46D0u))(a0, a1, a2, a3, a4); }
XI_ORIG int FUN_101c4740(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C4740u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_101c4790(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C4790u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined4 FUN_101c47a0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C47A0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_101c47b0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C47B0u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_101c4820(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C4820u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG undefined4 FUN_101c48e0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C48E0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void FUN_101c48f0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C48F0u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_101c4900(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C4900u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_101c4920(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C4920u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void FUN_101c4930(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C4930u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_101c4950(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C4950u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_101c49a0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C49A0u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_101c49f0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C49F0u))(a0, a1); }
XI_ORIG undefined4 FUN_101c4a10(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C4A10u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_101c4a30(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C4A30u))(a0, a1); }
XI_ORIG undefined4 FUN_101c4a80(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C4A80u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_101c4a90(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C4A90u))(a0, a1); }
XI_ORIG undefined4 FUN_101c4aa0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C4AA0u))(a0, a1); }
XI_ORIG undefined4 FUN_101c4ac0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101C4AC0u))(a0); }
XI_ORIG undefined4 FUN_101c4ad0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C4AD0u))(a0, a1); }
XI_ORIG int FileMng_ResolveOrExit(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x101C4AF0u))(a0, a1); }
XI_ORIG int FileMng_ResolveOrExit_v1(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C4AF0u))(a0, a1); }
XI_ORIG undefined4 FUN_101c4b40(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C4B40u))(a0, a1); }
XI_ORIG undefined4 FUN_101c4b60(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C4B60u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_101c4b70(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C4B70u))(a0, a1); }
XI_ORIG undefined4 FUN_101c4bc0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C4BC0u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_101c4bf0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C4BF0u))(a0, a1, a2); }
XI_ORIG void FUN_101c4c20(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101C4C20u))(a0); }
XI_ORIG undefined4 FUN_101c4c70(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C4C70u))(a0, a1); }
XI_ORIG char sub_101C4CC0(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(XI_GAME(0x101C4CC0u))(a0, a1); }
XI_ORIG uint8_t sub_101C4CD0(void * a0) { return reinterpret_cast<uint8_t (__thiscall *)(void *)>(XI_GAME(0x101C4CD0u))(a0); }
XI_ORIG int sub_101C4CE0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x101C4CE0u))(a0, a1); }
XI_ORIG undefined4 FUN_101c4cf0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101C4CF0u))(a0); }
XI_ORIG int sub_101C4D10(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x101C4D10u))(a0); }
XI_ORIG void FUN_101c4d30(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101C4D30u))(a0); }
XI_ORIG void FUN_101c4d40(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101C4D40u))(a0); }
XI_ORIG void * FUN_101c5920(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101C5920u))(a0); }
XI_ORIG void * FUN_101c5930(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C5930u))(a0, a1); }
XI_ORIG void FUN_101c59b0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C59B0u))(a0, a1); }
XI_ORIG void FUN_101c5a30(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C5A30u))(a0, a1); }
XI_ORIG uint8_t FUN_101c5a40(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<uint8_t (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C5A40u))(a0, a1, a2, a3); }
XI_ORIG void * FUN_101c5be0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C5BE0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG undefined4 FUN_101c5ec0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101C5EC0u))(a0); }
XI_ORIG void FUN_101c60c0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101C60C0u))(a0); }
XI_ORIG void FUN_101c6960(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101C6960u))(a0, a1, a2); }
XI_ORIG void FUN_101c7c50(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101C7C50u))(a0); }
XI_ORIG void FUN_101c7f80(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101C7F80u))(a0); }
XI_ORIG void * FUN_101c8070(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101C8070u))(a0); }
XI_ORIG void FUN_101c80a0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101C80A0u))(a0); }
XI_ORIG undefined4 FUN_101c80c0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C80C0u))(a0, a1); }
XI_ORIG void * FUN_101c8260(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101C8260u))(a0); }
XI_ORIG void FUN_101c8290(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C8290u))(a0, a1); }
XI_ORIG undefined4 FUN_101c8380(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101C8380u))(a0, a1); }
XI_ORIG undefined4 FUN_101c87a0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C87A0u))(a0, a1, a2); }
XI_ORIG int FUN_101c88a0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C88A0u))(a0, a1, a2, a3); }
XI_ORIG void YkCommonInit() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x101C8950u))(); }
XI_ORIG undefined4 FUN_101c8c30(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101C8C30u))(a0); }
XI_ORIG undefined4 FUN_101c8d10(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101C8D10u))(a0); }
XI_ORIG int FUN_101c8d70() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x101C8D70u))(); }
XI_ORIG void Gfx_ProjectWorldToScreen(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C8DA0u))(a0, a1, a2); }
XI_ORIG undefined8 FUN_101c9780(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C9780u))(a0, a1, a2, a3); }
XI_ORIG undefined8 FUN_101c97b0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C97B0u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG undefined8 FUN_101c9b50(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C9B50u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG undefined8 FUN_101c9d50(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101C9D50u))(a0, a1, a2, a3, a4); }
XI_ORIG int FUN_101ca180(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CA180u))(a0, a1, a2, a3); }
XI_ORIG void FUN_101cad00(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CAD00u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9); }
XI_ORIG void FUN_101caec0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CAEC0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined8 FUN_101caf00(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CAF00u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9); }
XI_ORIG undefined8 FUN_101cb000(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CB000u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void FUN_101cb040(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CB040u))(a0, a1, a2, a3); }
XI_ORIG undefined8 FUN_101cb090(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CB090u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_101cb230(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CB230u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG void FUN_101cb280(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9, XI_ANY a10) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CB280u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }
XI_ORIG void FUN_101cb430(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9, XI_ANY a10, XI_ANY a11) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CB430u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11); }
XI_ORIG void FUN_101cb500(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CB500u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG void FUN_101cb550(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CB550u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_101cb770(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CB770u))(a0, a1, a2); }
XI_ORIG unsigned long long FUN_101cbdf0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101CBDF0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_101cbe70(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101CBE70u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_101cc070(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101CC070u))(a0); }
XI_ORIG unsigned long long FUN_101cc0d0(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[2] = {a0.bits, a1.bits}; uint32_t ra = a2.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $8, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101CC0D0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_101cc1d0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101CC1D0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG unsigned long long FUN_101cc220(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101CC220u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_101cc350(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101CC350u))(a0, a1); }
XI_ORIG undefined8 FUN_101cc4f0() { return reinterpret_cast<undefined8 (__cdecl *)()>(XI_GAME(0x101CC4F0u))(); }
XI_ORIG undefined4 FUN_101cc510() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x101CC510u))(); }
XI_ORIG undefined4 FUN_101cc520(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101CC520u))(a0); }
XI_ORIG undefined4 FUN_101cc550() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x101CC550u))(); }
XI_ORIG undefined4 FUN_101cc560(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101CC560u))(a0); }
XI_ORIG undefined4 FUN_101cc620(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101CC620u))(a0, a1); }
XI_ORIG int FUN_101cce00(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x101CCE00u))(a0); }
XI_ORIG void FUN_101cd900(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101CD900u))(a0); }
XI_ORIG undefined4 FUN_101cd9d0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101CD9D0u))(a0); }
XI_ORIG undefined4 FUN_101cda00(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101CDA00u))(a0); }
XI_ORIG undefined4 FUN_101cda30(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101CDA30u))(a0); }
XI_ORIG void FUN_101cf660(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101CF660u))(a0, a1); }
XI_ORIG void FUN_101cf820(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CF820u))(a0, a1, a2, a3); }
XI_ORIG void FUN_101cf870(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101CF870u))(a0); }
XI_ORIG void YkJpeg_CutColor(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101CFA00u))(a0, a1); }
XI_ORIG void FUN_101cfdb0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CFDB0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG int FUN_101cfe20(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CFE20u))(a0, a1, a2, a3); }
XI_ORIG void FUN_101cfeb0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101CFEB0u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG void FUN_101cff60(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x101CFF60u))(a0); }
XI_ORIG void FUN_101d01b0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101D01B0u))(a0, a1, a2); }
XI_ORIG void FUN_101d0210(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101D0210u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_101d0370(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101D0370u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined * FUN_101d05e0() { return reinterpret_cast<undefined * (__cdecl *)()>(XI_GAME(0x101D05E0u))(); }
XI_ORIG int FUN_101d0600(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101D0600u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_101d0860(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101D0860u))(a0); }
XI_ORIG void FUN_101d0880(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101D0880u))(a0); }
XI_ORIG undefined4 FUN_101d08e0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101D08E0u))(a0, a1, a2); }
XI_ORIG void FUN_101d0910(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101D0910u))(a0, a1); }
XI_ORIG void FUN_101d0970(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101D0970u))(a0, a1, a2); }
XI_ORIG void FUN_101d09c0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101D09C0u))(a0, a1); }
XI_ORIG void FUN_101d09e0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101D09E0u))(a0, a1, a2); }
XI_ORIG void FUN_101d0a20(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101D0A20u))(a0, a1); }
XI_ORIG void FUN_101d0a40(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101D0A40u))(a0, a1); }
XI_ORIG void FUN_101d0a60(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101D0A60u))(a0); }
XI_ORIG char sub_101D0B00() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x101D0B00u))(); }
XI_ORIG char sub_101D0C70() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x101D0C70u))(); }
XI_ORIG _WORD * YkInit() { return reinterpret_cast<_WORD * (__cdecl *)()>(XI_GAME(0x101D11C0u))(); }
XI_ORIG undefined4 FUN_101d2260(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101D2260u))(a0, a1); }
XI_ORIG undefined4 FUN_101d29c0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101D29C0u))(a0, a1); }
XI_ORIG void FUN_101d2f00(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101D2F00u))(a0); }
XI_ORIG void FUN_101d30b0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101D30B0u))(a0); }
XI_ORIG undefined4 FUN_101d30f0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101D30F0u))(a0); }
XI_ORIG void FUN_101d31b0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101D31B0u))(a0); }
XI_ORIG void FUN_101d3dc0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101D3DC0u))(a0); }
XI_ORIG void FUN_101d3fc0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101D3FC0u))(a0, a1); }
XI_ORIG undefined4 FUN_101d4a20(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101D4A20u))(a0); }
XI_ORIG unsigned long long FUN_101d5370(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101D5370u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void sub_101D5680(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101D5680u))(a0); }
XI_ORIG undefined4 FUN_101d5fe0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x101D5FE0u))(); }
XI_ORIG void * FUN_101d7c10(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101D7C10u))(a0); }
XI_ORIG void * FUN_101d87a0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101D87A0u))(a0); }
XI_ORIG void * FUN_101d9b10(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101D9B10u))(a0); }
XI_ORIG void * FUN_101d9e90(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101D9E90u))(a0); }
XI_ORIG void * FUN_101dc460(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101DC460u))(a0); }
XI_ORIG void * FUN_101dce20(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101DCE20u))(a0); }
XI_ORIG void * FUN_101dd000(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101DD000u))(a0); }
XI_ORIG void FUN_101dda70(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101DDA70u))(a0); }
XI_ORIG void FUN_101dda90(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101DDA90u))(a0, a1); }
XI_ORIG undefined4 FUN_101ddf90(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101DDF90u))(a0); }
XI_ORIG undefined4 FUN_101de6b0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101DE6B0u))(a0); }
XI_ORIG void * FUN_101dedc0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101DEDC0u))(a0); }
XI_ORIG void * FUN_101df210(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101DF210u))(a0); }
XI_ORIG void FUN_101e06c0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101E06C0u))(a0); }
XI_ORIG void FUN_101e0700(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101E0700u))(a0); }
XI_ORIG void * FUN_101e0a90(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101E0A90u))(a0); }
XI_ORIG void FUN_101e15f0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101E15F0u))(a0); }
XI_ORIG undefined4 FUN_101e1a00(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101E1A00u))(a0); }
XI_ORIG undefined4 FUN_101e24f0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101E24F0u))(a0, a1); }
XI_ORIG void * FUN_101e2b10(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101E2B10u))(a0); }
XI_ORIG void * FUN_101e3f50(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101E3F50u))(a0); }
XI_ORIG void * FUN_101e48f0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101E48F0u))(a0); }
XI_ORIG void * FUN_101e4a20(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101E4A20u))(a0); }
XI_ORIG void * FUN_101e51b0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101E51B0u))(a0); }
XI_ORIG void * FUN_101e5750(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101E5750u))(a0); }
XI_ORIG void * FUN_101e5ba0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101E5BA0u))(a0); }
XI_ORIG void * FUN_101e6fb0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101E6FB0u))(a0); }
XI_ORIG void * FUN_101eb390(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101EB390u))(a0); }
XI_ORIG void * FUN_101eb7e0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101EB7E0u))(a0); }
XI_ORIG void * FUN_101ebe00(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101EBE00u))(a0); }
XI_ORIG void * FUN_101ec0d0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101EC0D0u))(a0); }
XI_ORIG void FUN_101ec3c0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101EC3C0u))(a0); }
XI_ORIG void * FUN_101ed450(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101ED450u))(a0); }
XI_ORIG void * FUN_101ee230(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101EE230u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void sub_101EE7B0(int a0, void * a1) { reinterpret_cast<void (__thiscall *)(int, void *)>(XI_GAME(0x101EE7B0u))(a0, a1); }
XI_ORIG void FUN_101efca0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101EFCA0u))(a0); }
XI_ORIG void FUN_101efea0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101EFEA0u))(a0); }
XI_ORIG void * FUN_101f17c0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101F17C0u))(a0); }
XI_ORIG void * FUN_101f43f0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101F43F0u))(a0); }
XI_ORIG void FUN_101f4af0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101F4AF0u))(a0); }
XI_ORIG unsigned long long YkWndList_SetLine(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101F6700u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void YkWndList_SetLine_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101F6700u))(a0, a1); }
XI_ORIG void * FUN_101f67c0(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x101F67C0u))(a0, a1); }
XI_ORIG unsigned long long FUN_101f71a0(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x101F71A0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG uint32_t FUN_101f71c0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x101F71C0u))(a0, a1); }
XI_ORIG undefined4 FUN_101f7200(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101F7200u))(a0); }
XI_ORIG undefined4 FUN_101f7240(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x101F7240u))(a0); }
XI_ORIG uint32_t FUN_101f7260(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x101F7260u))(a0); }
XI_ORIG int FUN_101f7430(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101F7430u))(a0, a1, a2); }
XI_ORIG int FUN_101f7480(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x101F7480u))(a0); }
XI_ORIG void FUN_101f7570(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x101F7570u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined * FUN_101f7600(XI_ANY a0) { return reinterpret_cast<undefined * (__cdecl *)(XI_ANY)>(XI_GAME(0x101F7600u))(a0); }
XI_ORIG void FUN_101f82b0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x101F82B0u))(a0, a1, a2); }
XI_ORIG void * FUN_101f86c0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101F86C0u))(a0); }
XI_ORIG void FUN_101f92b0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101F92B0u))(a0); }
XI_ORIG void FUN_101f9370(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x101F9370u))(a0); }
XI_ORIG void * FUN_101fb710(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101FB710u))(a0); }
XI_ORIG void * FUN_101fc2c0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101FC2C0u))(a0); }
XI_ORIG void * FUN_101fcbc0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101FCBC0u))(a0); }
XI_ORIG void * FUN_101fd000(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101FD000u))(a0); }
XI_ORIG undefined4 FUN_101fe430(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x101FE430u))(a0); }
XI_ORIG void * FUN_101ff000(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x101FF000u))(a0); }
XI_ORIG void * FUN_10200b60(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10200B60u))(a0); }
XI_ORIG void * FUN_10201020(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10201020u))(a0); }
XI_ORIG void FUN_102010a0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x102010A0u))(a0, a1); }
XI_ORIG void FUN_102010e0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x102010E0u))(a0, a1); }
XI_ORIG void * FUN_10201130(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10201130u))(a0); }
XI_ORIG undefined8 FUN_10201180(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10201180u))(a0, a1, a2); }
XI_ORIG void FUN_10201340(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10201340u))(a0, a1, a2); }
XI_ORIG void FUN_102014c0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x102014C0u))(a0, a1); }
XI_ORIG void FUN_102017a0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x102017A0u))(); }
XI_ORIG void * FUN_10201c90(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10201C90u))(a0); }
XI_ORIG undefined4 FUN_10201eb0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10201EB0u))(a0); }
XI_ORIG unsigned long long FUN_10201f00(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10201F00u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined * FUN_10202c90(XI_ANY a0) { return reinterpret_cast<undefined * (__stdcall *)(XI_ANY)>(XI_GAME(0x10202C90u))(a0); }
XI_ORIG void FUN_10202d30(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10202D30u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void FUN_10202d80(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10202D80u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG undefined4 FUN_10202f00() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10202F00u))(); }
XI_ORIG undefined1 FUN_10202fe0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined1 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10202FE0u))(a0, a1); }
XI_ORIG unsigned long long FUN_102031f0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[3] = {a0.bits, a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x102031F0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_102033c0(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x102033C0u))(a0); }
XI_ORIG undefined4 FUN_102034a0(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x102034A0u))(a0); }
XI_ORIG undefined4 FUN_102034e0(XI_ANY a0) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY)>(XI_GAME(0x102034E0u))(a0); }
XI_ORIG undefined4 FUN_10203500(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x10203500u))(a0, a1); }
XI_ORIG void FUN_10203520(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x10203520u))(a0); }
XI_ORIG int FUN_10203540(XI_ANY a0) { return reinterpret_cast<int (__stdcall *)(XI_ANY)>(XI_GAME(0x10203540u))(a0); }
XI_ORIG unsigned long long FUN_10203920(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {0}; uint32_t ra = a1.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10203920u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_10203be0(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x10203BE0u))(a0); }
XI_ORIG void * FUN_10205500(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10205500u))(a0); }
XI_ORIG void FUN_10206420(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10206420u))(a0); }
XI_ORIG void * FUN_10207730(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10207730u))(a0); }
XI_ORIG undefined4 FUN_10208500(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10208500u))(a0); }
XI_ORIG void * FUN_10209300(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10209300u))(a0); }
XI_ORIG void * FUN_10209b90(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10209B90u))(a0); }
XI_ORIG void * FUN_1020b500(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1020B500u))(a0); }
XI_ORIG undefined4 FUN_1020ba40(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1020BA40u))(a0, a1); }
XI_ORIG void FUN_1020be90(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1020BE90u))(a0); }
XI_ORIG void * FUN_1020c470(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1020C470u))(a0); }
XI_ORIG void * FUN_1020cc30(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1020CC30u))(a0); }
XI_ORIG void * FUN_1020ed00(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1020ED00u))(a0); }
XI_ORIG void * YkWndShop_Constructor(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10210310u))(a0); }
XI_ORIG void FUN_10210d60(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10210D60u))(a0); }
XI_ORIG void * FUN_10210da0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10210DA0u))(a0, a1, a2); }
XI_ORIG void FUN_10210fc0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10210FC0u))(a0, a1); }
XI_ORIG undefined4 FUN_102112b0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x102112B0u))(a0, a1); }
XI_ORIG undefined4 FUN_10211340() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10211340u))(); }
XI_ORIG void FUN_102174f0() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x102174F0u))(); }
XI_ORIG void * FUN_102175c0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x102175C0u))(a0); }
XI_ORIG void FUN_1021b330(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1021B330u))(a0); }
XI_ORIG _WORD * sub_1021BB90() { return reinterpret_cast<_WORD * (__cdecl *)()>(XI_GAME(0x1021BB90u))(); }
XI_ORIG void FUN_1021bc20(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1021BC20u))(a0); }
XI_ORIG undefined4 * FUN_1021bc30(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1021BC30u))(a0, a1); }
XI_ORIG undefined4 FUN_1021bcb0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1021BCB0u))(); }
XI_ORIG uint8_t FUN_1021bcc0(void * a0, XI_ANY a1) { return reinterpret_cast<uint8_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1021BCC0u))(a0, a1); }
XI_ORIG uint8_t FUN_1021bdb0(void * a0, XI_ANY a1) { return reinterpret_cast<uint8_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1021BDB0u))(a0, a1); }
XI_ORIG undefined4 FUN_1021be50(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1021BE50u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1021bf60(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1021BF60u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1021c070(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1021C070u))(a0, a1, a2, a3); }
XI_ORIG unsigned long long FUN_1021c100(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[2] = {a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1021C100u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_1021c160(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1021C160u))(a0); }
XI_ORIG undefined4 FUN_1021c1b0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1021C1B0u))(a0); }
XI_ORIG undefined4 FUN_1021c210(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1021C210u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1021c2e0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1021C2E0u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1021c350(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1021C350u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1021c380(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1021C380u))(a0); }
XI_ORIG uint32_t FUN_1021c5b0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1021C5B0u))(a0, a1, a2, a3); }
XI_ORIG int FUN_1021c680(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1021C680u))(a0, a1); }
XI_ORIG unsigned long long FUN_1021c7c0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) {
    uint32_t xs[3] = {a1.bits, a2.bits, a3.bits}; uint32_t ra = a4.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1021C7C0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_1021c7e0(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1021C7E0u))(a0, a1, a2, a3, a4); }
XI_ORIG void * FUN_1021d250(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1021D250u))(a0); }
XI_ORIG undefined4 FUN_1021d270(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1021D270u))(a0); }
XI_ORIG undefined4 FUN_1021d2d0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1021D2D0u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1021d380(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1021D380u))(a0, a1, a2); }
XI_ORIG unsigned long long FUN_1021d460(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1021D460u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG int CXiMovie_DrawFrame(int a0, int a1, int a2, int a3, void * a4) { return reinterpret_cast<int (__cdecl *)(int, int, int, int, void *)>(XI_GAME(0x1021D4C0u))(a0, a1, a2, a3, a4); }
XI_ORIG void * FUN_1021d7f0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1021D7F0u))(a0); }
XI_ORIG uint32_t FUN_1021d930(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1021D930u))(a0, a1); }
XI_ORIG undefined4 FUN_1021d960() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x1021D960u))(); }
XI_ORIG char Movie_Update(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(XI_GAME(0x1021D980u))(a0); }
XI_ORIG unsigned long long FUN_1021d9a0(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1021D9A0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG int FUN_1021d9b0(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1021D9B0u))(a0, a1); }
XI_ORIG int * FUN_1021dd50(void * a0) { return reinterpret_cast<int * (__thiscall *)(void *)>(XI_GAME(0x1021DD50u))(a0); }
XI_ORIG int Movie_GetCurrentTexture(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x1021DDF0u))(a0); }
XI_ORIG int FUN_1021e3b0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1021E3B0u))(a0, a1); }
XI_ORIG int FUN_1021e480(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1021E480u))(a0, a1, a2); }
XI_ORIG void FUN_1021e4d0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1021E4D0u))(a0, a1); }
XI_ORIG void FUN_1021e500(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1021E500u))(a0, a1, a2); }
XI_ORIG int FUN_1021e550(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1021E550u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG uint32_t FUN_1021e670(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x1021E670u))(a0); }
XI_ORIG void FUN_1021e690(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1021E690u))(a0); }
XI_ORIG void FUN_1021e760(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1021E760u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_1021e870(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1021E870u))(a0, a1); }
XI_ORIG void FUN_1021ed30() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x1021ED30u))(); }
XI_ORIG void FUN_1021ef10(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1021EF10u))(a0); }
XI_ORIG void FUN_1021ef30() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x1021EF30u))(); }
XI_ORIG int FUN_1021f140(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x1021F140u))(a0, a1); }
XI_ORIG void FUN_1021f240(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1021F240u))(a0); }
XI_ORIG void FUN_1021f360(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x1021F360u))(a0); }
XI_ORIG void FUN_1021f480(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x1021F480u))(a0); }
XI_ORIG void FUN_1021f950(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1021F950u))(a0, a1); }
XI_ORIG undefined4 FUN_1021fa90(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1021FA90u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG void FUN_1021fb10(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1021FB10u))(a0, a1, a2); }
XI_ORIG void FUN_1021fb80(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1021FB80u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void FUN_1021fbd0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1021FBD0u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void FUN_102201a0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102201A0u))(a0, a1, a2, a3, a4); }
XI_ORIG int FUN_10220350(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10220350u))(a0); }
XI_ORIG void FUN_102203b0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x102203B0u))(a0); }
XI_ORIG void FUN_10220c80() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10220C80u))(); }
XI_ORIG undefined4 FUN_10220cf0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10220CF0u))(a0, a1); }
XI_ORIG int sub_10220EF0(int a0, int a1, int a2, char a3) { return reinterpret_cast<int (__thiscall *)(int, int, int, char)>(XI_GAME(0x10220EF0u))(a0, a1, a2, a3); }
XI_ORIG int sub_10220F40(int a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(int, int, int)>(XI_GAME(0x10220F40u))(a0, a1, a2); }
XI_ORIG void * FUN_10220fd0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10220FD0u))(a0); }
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
XI_ORIG void FUN_102214d0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x102214D0u))(a0); }
XI_ORIG undefined4 FUN_10221680(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10221680u))(a0); }
XI_ORIG int sub_10221770(unsigned __int16 a0) { return reinterpret_cast<int (__cdecl *)(unsigned __int16)>(XI_GAME(0x10221770u))(a0); }
XI_ORIG void * sub_102217A0() { return reinterpret_cast<void * (__cdecl *)()>(XI_GAME(0x102217A0u))(); }
XI_ORIG void * FUN_102217c0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x102217C0u))(a0); }
XI_ORIG void FUN_10221900(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10221900u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_10221920(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10221920u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void FUN_10221b90(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10221B90u))(a0, a1); }
XI_ORIG void FUN_10221be0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10221BE0u))(a0, a1, a2); }
XI_ORIG void FUN_10221e10(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10221E10u))(a0, a1); }
XI_ORIG void FUN_10221ed0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10221ED0u))(a0); }
XI_ORIG void FUN_102224c0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x102224C0u))(); }
XI_ORIG undefined4 FUN_102225f0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102225F0u))(a0); }
XI_ORIG uint32_t FUN_10222c10(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x10222C10u))(a0); }
XI_ORIG void * FUN_102238e0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x102238E0u))(a0); }
XI_ORIG undefined * FUN_10223930() { return reinterpret_cast<undefined * (__cdecl *)()>(XI_GAME(0x10223930u))(); }
XI_ORIG void * FUN_102239a0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x102239A0u))(a0); }
XI_ORIG void FUN_102239c0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x102239C0u))(a0); }
XI_ORIG void * FUN_10223bb0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10223BB0u))(a0); }
XI_ORIG int FUN_10223c30() { return reinterpret_cast<int (__stdcall *)()>(XI_GAME(0x10223C30u))(); }
XI_ORIG char FUN_10223c90(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(XI_GAME(0x10223C90u))(a0); }
XI_ORIG void * FUN_10223e60(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10223E60u))(a0); }
XI_ORIG int FUN_10225090(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10225090u))(a0, a1); }
XI_ORIG void FUN_102250f0() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x102250F0u))(); }
XI_ORIG int FUN_10225120(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10225120u))(a0, a1, a2, a3); }
XI_ORIG int FUN_10225220(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10225220u))(a0, a1, a2, a3); }
XI_ORIG void * FUN_102254d0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x102254D0u))(a0); }
XI_ORIG void * FUN_10226080(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10226080u))(a0); }
XI_ORIG undefined * FUN_10226180() { return reinterpret_cast<undefined * (__cdecl *)()>(XI_GAME(0x10226180u))(); }
XI_ORIG void * FUN_102261f0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x102261F0u))(a0); }
XI_ORIG void FUN_10226210(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10226210u))(a0); }
XI_ORIG void * FUN_10226550(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10226550u))(a0); }
XI_ORIG void * FUN_10226c80(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10226C80u))(a0); }
XI_ORIG void FUN_10226e30() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x10226E30u))(); }
XI_ORIG void * FUN_10227080(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10227080u))(a0); }
XI_ORIG undefined4 FUN_102270f0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x102270F0u))(a0); }
XI_ORIG undefined4 FUN_10227100(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10227100u))(a0); }
XI_ORIG undefined4 FUN_10227130(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10227130u))(a0); }
XI_ORIG void * FUN_102271b0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x102271B0u))(a0); }
XI_ORIG void FUN_102271d0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x102271D0u))(a0); }
XI_ORIG void * FUN_102278b0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x102278B0u))(a0); }
XI_ORIG undefined4 FUN_10227920(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10227920u))(a0); }
XI_ORIG int FUN_10227930(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10227930u))(a0, a1); }
XI_ORIG void FUN_102281c0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x102281C0u))(a0); }
XI_ORIG void * FUN_10228200(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10228200u))(a0); }
XI_ORIG void * FUN_10229670(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10229670u))(a0); }
XI_ORIG undefined4 FUN_1022af80(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1022AF80u))(a0); }
XI_ORIG void FUN_1022afb0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1022AFB0u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1022aff0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1022AFF0u))(a0); }
XI_ORIG void FUN_1022b260() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1022B260u))(); }
XI_ORIG void FUN_1022b3b0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1022B3B0u))(a0); }
XI_ORIG void * FUN_1022bd50(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1022BD50u))(a0); }
XI_ORIG undefined * FUN_1022bda0() { return reinterpret_cast<undefined * (__cdecl *)()>(XI_GAME(0x1022BDA0u))(); }
XI_ORIG void * FUN_1022be10(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1022BE10u))(a0); }
XI_ORIG void FUN_1022be30(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1022BE30u))(a0); }
XI_ORIG void * FUN_1022bfb0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1022BFB0u))(a0); }
XI_ORIG void * FUN_1022c350(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1022C350u))(a0); }
XI_ORIG void FUN_1022ed00(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1022ED00u))(a0); }
XI_ORIG void FUN_1022ed30(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x1022ED30u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1022ed70(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1022ED70u))(a0, a1); }
XI_ORIG int FUN_1022edf0(XI_ANY a0) { return reinterpret_cast<int (__stdcall *)(XI_ANY)>(XI_GAME(0x1022EDF0u))(a0); }
XI_ORIG undefined1 FUN_1022eee0(void * a0, XI_ANY a1) { return reinterpret_cast<undefined1 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1022EEE0u))(a0, a1); }
XI_ORIG undefined4 FUN_1022f240(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1022F240u))(a0); }
XI_ORIG void * FUN_1022f610(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1022F610u))(a0); }
XI_ORIG void * FUN_102303b0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x102303B0u))(a0); }
XI_ORIG void FUN_10230410(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10230410u))(a0); }
XI_ORIG void * FUN_10233be0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x10233BE0u))(a0); }
XI_ORIG void * FUN_102340a0(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x102340A0u))(a0); }
XI_ORIG undefined8 FUN_10234910(void * a0) { return reinterpret_cast<undefined8 (__thiscall *)(void *)>(XI_GAME(0x10234910u))(a0); }
XI_ORIG void FUN_10234a70(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10234A70u))(a0, a1, a2); }
XI_ORIG int Ui_KeyPrompt_Init(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x10239460u))(a0, a1); }
XI_ORIG void Ui_KeyPrompt_Init_v1(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10239460u))(a0, a1); }
XI_ORIG undefined4 FUN_10239490() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10239490u))(); }
XI_ORIG void FUN_1023be10(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1023BE10u))(a0, a1); }
XI_ORIG void FUN_10240860() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x10240860u))(); }
XI_ORIG void FUN_10244f20() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10244F20u))(); }
XI_ORIG void FUN_10245040(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10245040u))(a0); }
XI_ORIG int sub_10245B50(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10245B50u))(a0); }
XI_ORIG void FUN_10245bb0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10245BB0u))(); }
XI_ORIG undefined4 FUN_10245be0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10245BE0u))(); }
XI_ORIG undefined4 FUN_10245f30(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10245F30u))(a0); }
XI_ORIG undefined4 FUN_10246000(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x10246000u))(a0, a1); }
XI_ORIG undefined8 FUN_10246160(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__stdcall *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10246160u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10246320(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__stdcall *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10246320u))(a0, a1, a2); }
XI_ORIG void FUN_102463c0(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x102463C0u))(a0); }
XI_ORIG uint32_t FUN_102465e0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<uint32_t (__stdcall *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102465E0u))(a0, a1, a2); }
XI_ORIG void FUN_10246770(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10246770u))(a0, a1, a2); }
XI_ORIG void FUN_102467a0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__stdcall *)(XI_ANY, XI_ANY)>(XI_GAME(0x102467A0u))(a0, a1); }
XI_ORIG undefined8 FUN_102467f0(XI_ANY a0) { return reinterpret_cast<undefined8 (__stdcall *)(XI_ANY)>(XI_GAME(0x102467F0u))(a0); }
XI_ORIG undefined8 FUN_1024c060(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { return reinterpret_cast<undefined8 (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1024C060u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG void * FUN_1024c930(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(XI_GAME(0x1024C930u))(a0); }
XI_ORIG undefined4 StAvatarInit(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1024CF50u))(a0); }
XI_ORIG char sub_1024D130() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x1024D130u))(); }
XI_ORIG uint32_t StAvatar_Teardown(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1024D180u))(a0, a1, a2); }
XI_ORIG undefined4 StAvatar_ReleaseResources(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1024D2C0u))(a0, a1); }
XI_ORIG void FUN_1024d400(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1024D400u))(a0); }
XI_ORIG void FUN_1024d470(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1024D470u))(a0, a1); }
XI_ORIG undefined4 FUN_1024d4a0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1024D4A0u))(a0, a1); }
XI_ORIG undefined4 FUN_1024d540(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1024D540u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1024d5c0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1024D5C0u))(a0, a1); }
XI_ORIG undefined4 FUN_1024d610(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1024D610u))(a0); }
XI_ORIG void FUN_1024d6b0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1024D6B0u))(a0); }
XI_ORIG undefined4 FUN_1024d740(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1024D740u))(a0, a1, a2); }
XI_ORIG undefined4 StAvatar_StopAndFreeBuffers(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1024D790u))(a0); }
XI_ORIG void FUN_1024d850(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1024D850u))(a0); }
XI_ORIG void FUN_1024d9d0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1024D9D0u))(); }
XI_ORIG void FUN_1024da10(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1024DA10u))(a0); }
XI_ORIG undefined4 FUN_1024db60(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1024DB60u))(a0); }
XI_ORIG void FUN_1024dd10(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1024DD10u))(a0, a1); }
XI_ORIG void FUN_1024de20(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1024DE20u))(a0); }
XI_ORIG void FUN_1024dfe0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1024DFE0u))(a0); }
XI_ORIG void FUN_1024e280(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1024E280u))(a0, a1); }
XI_ORIG void FUN_1024e720(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1024E720u))(a0, a1, a2); }
XI_ORIG void FUN_1024f2c0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1024F2C0u))(a0); }
XI_ORIG void FUN_1024f5e0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1024F5E0u))(a0, a1); }
XI_ORIG void StAvatar_SetViewFromDancerCamera(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1024F900u))(a0); }
XI_ORIG void FUN_1024fc10(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1024FC10u))(a0, a1); }
XI_ORIG void FUN_1024fdf0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1024FDF0u))(a0, a1); }
XI_ORIG void FUN_1024fec0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1024FEC0u))(a0); }
XI_ORIG undefined4 FUN_1024ff50(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1024FF50u))(a0, a1); }
XI_ORIG unsigned long long FUN_10250030(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10250030u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_10250190(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10250190u))(a0, a1); }
XI_ORIG void StAvatarRender(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102501C0u))(a0); }
XI_ORIG int sub_102502D0(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x102502D0u))(a0); }
XI_ORIG undefined4 StAvatarFini(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102504D0u))(a0); }
XI_ORIG undefined4 FUN_10250580(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10250580u))(a0, a1); }
XI_ORIG undefined4 FUN_10250660(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10250660u))(a0, a1, a2); }
XI_ORIG unsigned long long FUN_102506c0(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[2] = {a0.bits, a1.bits}; uint32_t ra = a2.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $8, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x102506C0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_10250760(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10250760u))(a0, a1); }
XI_ORIG undefined4 FUN_10250790(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10250790u))(a0, a1); }
XI_ORIG undefined4 FUN_102507c0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102507C0u))(a0, a1); }
XI_ORIG undefined4 FUN_10250800(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10250800u))(a0, a1); }
XI_ORIG void FUN_102509c0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102509C0u))(a0, a1); }
XI_ORIG void FUN_10250a40(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10250A40u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10250b10(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10250B10u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10250c00(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10250C00u))(a0, a1); }
XI_ORIG int sub_10250D80(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x10250D80u))(a0); }
XI_ORIG undefined4 FUN_10250db0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10250DB0u))(a0, a1); }
XI_ORIG undefined4 StChannelItemResolve(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10250F10u))(a0); }
XI_ORIG uint32_t FUN_10251070(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10251070u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10251160(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10251160u))(a0); }
XI_ORIG undefined4 FUN_10251180(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10251180u))(a0); }
XI_ORIG undefined4 FUN_102511b0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102511B0u))(a0, a1); }
XI_ORIG undefined4 FUN_10251200(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10251200u))(a0, a1, a2); }
XI_ORIG undefined4 StChannelItemClear(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102512B0u))(a0); }
XI_ORIG undefined4 FUN_10251320(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10251320u))(a0, a1); }
XI_ORIG undefined4 FUN_10251370(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10251370u))(a0, a1); }
XI_ORIG undefined4 FUN_10251410(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10251410u))(a0, a1); }
XI_ORIG int StDancer_Update(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10251500u))(a0); }
XI_ORIG int sub_102515F0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(XI_GAME(0x102515F0u))(a0, a1); }
XI_ORIG void FUN_10251620(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10251620u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_102516b0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x102516B0u))(a0, a1, a2); }
XI_ORIG void FUN_10251720(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10251720u))(a0, a1); }
XI_ORIG undefined4 FUN_10251750(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10251750u))(a0, a1); }
XI_ORIG unsigned long long FUN_10251780(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[1] = {a1.bits}; uint32_t ra = a2.bits, rc = a0.bits, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10251780u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void StDancer_SetJobLock(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10251890u))(a0, a1); }
XI_ORIG void FUN_10251920(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10251920u))(a0, a1); }
XI_ORIG int sub_10251960(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10251960u))(a0); }
XI_ORIG uint32_t Dancer_InitDevice(void * a0) { return reinterpret_cast<uint32_t (__thiscall *)(void *)>(XI_GAME(0x10251A80u))(a0); }
XI_ORIG undefined4 FUN_10251ce0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10251CE0u))(); }
XI_ORIG int sub_102524B0(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x102524B0u))(a0); }
XI_ORIG undefined4 StModelItemClear(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102524E0u))(a0); }
XI_ORIG undefined4 FUN_10252610(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10252610u))(a0, a1); }
XI_ORIG undefined4 FUN_102528d0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102528D0u))(a0); }
XI_ORIG undefined4 FUN_10252a00(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10252A00u))(a0, a1); }
XI_ORIG undefined4 FUN_10252a40(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10252A40u))(a0); }
XI_ORIG undefined4 FUN_10252ac0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10252AC0u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10252b50(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10252B50u))(a0, a1); }
XI_ORIG undefined4 FUN_10252c10(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10252C10u))(a0); }
XI_ORIG undefined4 FUN_10252c30(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10252C30u))(a0, a1); }
XI_ORIG int sub_10252C80(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x10252C80u))(a0); }
XI_ORIG int StTrigger_ProcessOpcodesUpToTime(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10252CC0u))(a0, a1, a2, a3); }
XI_ORIG int FUN_10252ea0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10252EA0u))(a0, a1, a2); }
XI_ORIG undefined4 StTrigger_RegisterCallbacks(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10252F00u))(a0, a1, a2); }
XI_ORIG undefined StTrigger_AllocFromPool() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10252F70u))(); }
XI_ORIG undefined4 FUN_10253160(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10253160u))(a0); }
XI_ORIG undefined4 sqopSystemActivate(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102531B0u))(a0); }
XI_ORIG undefined4 FUN_10253210(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10253210u))(a0); }
XI_ORIG undefined4 FUN_102532a0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102532A0u))(a0); }
XI_ORIG unsigned long long FUN_102532c0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[3] = {a0.bits, a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $12, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x102532C0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG _DWORD * sub_102532E0(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(XI_GAME(0x102532E0u))(a0); }
XI_ORIG void FUN_10253330() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10253330u))(); }
XI_ORIG void FUN_10253360(XI_ANY a0) { reinterpret_cast<void (__stdcall *)(XI_ANY)>(XI_GAME(0x10253360u))(a0); }
XI_ORIG int FUN_102534b0(XI_ANY a0) { return reinterpret_cast<int (__stdcall *)(XI_ANY)>(XI_GAME(0x102534B0u))(a0); }
XI_ORIG unsigned long long FUN_102535f0(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[2] = {a0.bits, a1.bits}; uint32_t ra = a2.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $8, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x102535F0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_10253620() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x10253620u))(); }
XI_ORIG void FUN_10253670(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10253670u))(a0, a1); }
XI_ORIG char sub_10253670(__int16 a0, char a1) { return reinterpret_cast<char (__cdecl *)(__int16, char)>(XI_GAME(0x10253670u))(a0, a1); }
XI_ORIG undefined4 FUN_102536a0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x102536A0u))(); }
XI_ORIG undefined4 FUN_102572e0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102572E0u))(a0, a1, a2); }
XI_ORIG unsigned long long FUN_10257310(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10257310u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_102573c0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102573C0u))(a0); }
XI_ORIG unsigned long long FUN_102573e0(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x102573E0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG short * FUN_102574f0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<short * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102574F0u))(a0, a1); }
XI_ORIG short * FUN_10257550(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<short * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10257550u))(a0, a1, a2); }
XI_ORIG int FUN_10257590(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x10257590u))(a0); }
XI_ORIG undefined4 FUN_102575b0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102575B0u))(a0); }
XI_ORIG undefined4 FUN_102575d0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102575D0u))(a0); }
XI_ORIG int FUN_102575e0(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x102575E0u))(a0); }
XI_ORIG undefined4 * FUN_10257670(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10257670u))(a0, a1); }
XI_ORIG undefined * FUN_102577c0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102577C0u))(a0, a1, a2, a3); }
XI_ORIG undefined4 FUN_10257800(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10257800u))(a0, a1); }
XI_ORIG undefined1 * FUN_10257830() { return reinterpret_cast<undefined1 * (__cdecl *)()>(XI_GAME(0x10257830u))(); }
XI_ORIG void thunk_FUN_10257850(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10257840u))(a0); }
XI_ORIG void FUN_10257850(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10257850u))(a0); }
XI_ORIG void FUN_10257860(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10257860u))(a0); }
XI_ORIG int FUN_10257870(void * a0, XI_ANY a1) { return reinterpret_cast<int (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10257870u))(a0, a1); }
XI_ORIG short * FUN_102578b0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<short * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x102578B0u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10257920(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10257920u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10257950(void * a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10257950u))(a0, a1); }
XI_ORIG undefined4 FUN_10257980(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10257980u))(a0, a1, a2); }
XI_ORIG void FUN_102579b0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x102579B0u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10257af0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10257AF0u))(a0); }
XI_ORIG undefined4 FUN_10257b10(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10257B10u))(a0); }
XI_ORIG void FUN_10257b60(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10257B60u))(a0); }
XI_ORIG void FUN_10257ba0(void * a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10257BA0u))(a0, a1, a2); }
XI_ORIG int FUN_10257bc0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(XI_GAME(0x10257BC0u))(a0); }
XI_ORIG void FUN_10257bd0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10257BD0u))(a0, a1); }
XI_ORIG void FUN_10257bf0(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10257BF0u))(a0, a1); }
XI_ORIG undefined4 FUN_10257c60(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10257C60u))(a0); }
XI_ORIG undefined4 FUN_10257c70(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10257C70u))(a0); }
XI_ORIG undefined4 FUN_10257c80(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x10257C80u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10257ff0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10257FF0u))(a0); }
XI_ORIG undefined * FUN_10258150(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10258150u))(a0, a1, a2, a3, a4); }
XI_ORIG int FUN_102581d0(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x102581D0u))(a0); }
XI_ORIG void FUN_10258430(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10258430u))(a0, a1, a2, a3); }
XI_ORIG void FUN_10258600() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x10258600u))(); }
XI_ORIG undefined4 FUN_102586d0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x102586D0u))(a0); }
XI_ORIG undefined4 FUN_102586f0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x102586F0u))(a0); }
XI_ORIG void FUN_10258740(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10258740u))(a0, a1, a2, a3); }
XI_ORIG void FUN_10258830() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x10258830u))(); }
XI_ORIG void FUN_102589f0() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x102589F0u))(); }
XI_ORIG undefined4 FUN_10258ad0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10258AD0u))(a0); }
XI_ORIG void FUN_10258bb0() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x10258BB0u))(); }
XI_ORIG undefined * FUN_10258d00(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10258D00u))(a0, a1, a2, a3); }
XI_ORIG char * FUN_10258d70(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<char * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10258D70u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined1 * FUN_10258e10(XI_ANY a0) { return reinterpret_cast<undefined1 * (__cdecl *)(XI_ANY)>(XI_GAME(0x10258E10u))(a0); }
XI_ORIG undefined * FUN_10258e50(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10258E50u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined1 * FUN_10258ed0(XI_ANY a0) { return reinterpret_cast<undefined1 * (__cdecl *)(XI_ANY)>(XI_GAME(0x10258ED0u))(a0); }
XI_ORIG undefined * FUN_10258f00(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10258F00u))(a0, a1, a2); }
XI_ORIG undefined * FUN_10258f60(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10258F60u))(a0, a1, a2); }
XI_ORIG void FUN_10259100(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10259100u))(a0, a1); }
XI_ORIG undefined4 FUN_10259130(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x10259130u))(a0); }
XI_ORIG int FUN_10259150(XI_ANY a0) { return reinterpret_cast<int (__stdcall *)(XI_ANY)>(XI_GAME(0x10259150u))(a0); }
XI_ORIG void FUN_10259de0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10259DE0u))(a0); }
XI_ORIG undefined4 FUN_1025a580(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1025A580u))(a0, a1); }
XI_ORIG undefined1 * FUN_1025a5a0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined1 * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1025A5A0u))(a0, a1, a2); }
XI_ORIG undefined1 * FUN_1025a610(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined1 * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1025A610u))(a0, a1, a2); }
XI_ORIG undefined1 * FUN_1025a6c0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined1 * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1025A6C0u))(a0, a1); }
XI_ORIG undefined1 * FUN_1025a6f0(XI_ANY a0) { return reinterpret_cast<undefined1 * (__cdecl *)(XI_ANY)>(XI_GAME(0x1025A6F0u))(a0); }
XI_ORIG undefined * FUN_1025a730(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1025A730u))(a0, a1, a2); }
XI_ORIG undefined1 * FUN_1025a760(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined1 * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1025A760u))(a0, a1, a2, a3); }
XI_ORIG void FUN_1025a820(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1025A820u))(a0); }
XI_ORIG void FUN_1025a860(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1025A860u))(a0, a1); }
XI_ORIG int FUN_1025a890(XI_ANY a0) { return reinterpret_cast<int (__stdcall *)(XI_ANY)>(XI_GAME(0x1025A890u))(a0); }
XI_ORIG undefined4 FUN_1025a8c0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1025A8C0u))(a0); }
XI_ORIG uint32_t FUN_1025a930(void * a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1025A930u))(a0, a1); }
XI_ORIG undefined4 FUN_1025aa90(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1025AA90u))(a0); }
XI_ORIG void FUN_1025b100(void * a0, XI_ANY a1) { reinterpret_cast<void (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x1025B100u))(a0, a1); }
XI_ORIG void FUN_1025b4f0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1025B4F0u))(a0); }
XI_ORIG undefined4 FUN_1025b560(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1025B560u))(a0); }
XI_ORIG int sub_1025B560(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x1025B560u))(a0); }
XI_ORIG undefined * FUN_1025b5a0(XI_ANY a0) { return reinterpret_cast<undefined * (__cdecl *)(XI_ANY)>(XI_GAME(0x1025B5A0u))(a0); }
XI_ORIG int FUN_1025b5e0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1025B5E0u))(a0, a1, a2, a3); }
XI_ORIG int FUN_1025b680(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1025B680u))(a0, a1); }
XI_ORIG undefined4 FUN_1025bb00(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1025BB00u))(a0); }
XI_ORIG uint32_t FUN_1025bbe0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1025BBE0u))(a0, a1, a2); }
XI_ORIG int FUN_1025bde0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1025BDE0u))(a0, a1); }
XI_ORIG undefined4 FUN_1025be20(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1025BE20u))(a0); }
XI_ORIG uint32_t FUN_1025be80(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x1025BE80u))(a0); }
XI_ORIG uint32_t FUN_1025bfb0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1025BFB0u))(a0, a1, a2); }
XI_ORIG int FUN_1025c0f0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1025C0F0u))(); }
XI_ORIG int FUN_1025c0f0_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1025C0F0u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1025e020(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1025E020u))(a0); }
XI_ORIG unsigned long long FUN_1025e020_v1(XI_ANY a0) {
    uint32_t xs[1] = {0}; uint32_t ra = a0.bits, rc = 0, rd = 0;
    __asm__ volatile("call *%%edi\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1025E020u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_1025e050() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1025E050u))(); }
XI_ORIG void FUN_1025e090(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1025E090u))(a0); }
XI_ORIG void * sub_1025E0B0() { return reinterpret_cast<void * (__cdecl *)()>(XI_GAME(0x1025E0B0u))(); }
XI_ORIG void sub_1025E0C0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1025E0C0u))(); }
XI_ORIG int sub_1025E0E0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1025E0E0u))(); }
XI_ORIG char sub_1025E0F0() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x1025E0F0u))(); }
XI_ORIG void FUN_1025e210(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1025E210u))(a0); }
XI_ORIG void FUN_1025e230() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1025E230u))(); }
XI_ORIG void FUN_1025e240() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1025E240u))(); }
XI_ORIG void * FUN_1025e260(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1025E260u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_1025f2f0(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1025F2F0u))(a0); }
XI_ORIG undefined4 FUN_1025f640(void * a0) { return reinterpret_cast<undefined4 (__thiscall *)(void *)>(XI_GAME(0x1025F640u))(a0); }
XI_ORIG void * FUN_1025fd50(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1025FD50u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_1025fe40(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x1025FE40u))(a0); }
XI_ORIG void * FUN_10261210(void * a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10261210u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_102612d0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x102612D0u))(a0); }
XI_ORIG void * FUN_10265940(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10265940u))(a0, a1); }
XI_ORIG void _dynamic_initializer_for__std__num_put_char_std__ostreambuf_iterator_char_std__char_traits_char_______id___47() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10265960u))(); }
XI_ORIG void * FUN_102659b0(void * a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY, XI_ANY)>(XI_GAME(0x102659B0u))(a0, a1, a2); }
XI_ORIG void * FUN_10265b70(void * a0, XI_ANY a1) { return reinterpret_cast<void * (__thiscall *)(void *, XI_ANY)>(XI_GAME(0x10265B70u))(a0, a1); }
XI_ORIG void FUN_10265b90(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(XI_GAME(0x10265B90u))(a0); }
XI_ORIG void FUN_1026cc30() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1026CC30u))(); }
XI_ORIG void FUN_1026cca5(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026CCA5u))(a0, a1); }
XI_ORIG undefined4 FUN_1026cdf4(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1026CDF4u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1026cdf4_v1() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1026CDF4u))(); }
XI_ORIG undefined4 FUN_1026cfa0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1026CFA0u))(a0); }
XI_ORIG void FUN_1026d015(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1026D015u))(a0); }
XI_ORIG undefined4 FUN_1026d0ca(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026D0CAu))(a0, a1); }
XI_ORIG undefined4 FUN_1026d179(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1026D179u))(a0); }
XI_ORIG undefined4 FUN_1026d1b4(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026D1B4u))(a0, a1); }
XI_ORIG unsigned long long FUN_1026d634(XI_ANY a0, XI_ANY a1) {
    uint32_t xs[1] = {a0.bits}; uint32_t ra = a1.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $4, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x1026D634u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_1026d70c(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1026D70Cu))(a0); }
XI_ORIG undefined4 FUN_1026d866(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1026D866u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG void FUN_1026d9b1(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1026D9B1u))(a0, a1, a2); }
XI_ORIG uint8_t FUN_1026db24(XI_ANY a0) { return reinterpret_cast<uint8_t (__cdecl *)(XI_ANY)>(XI_GAME(0x1026DB24u))(a0); }
XI_ORIG void FUN_1026dbce(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026DBCEu))(a0, a1); }
XI_ORIG void FUN_1026dbce_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1026DBCEu))(); }
XI_ORIG undefined4 FUN_1026dbf0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1026DBF0u))(a0); }
XI_ORIG undefined4 FUN_1026dbf0_v1() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1026DBF0u))(); }
XI_ORIG void FUN_1026dc0d(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026DC0Du))(a0, a1); }
XI_ORIG void FUN_1026dc0d_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1026DC0Du))(); }
XI_ORIG void sqxfXformTD_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1026DF00u))(a0); }
XI_ORIG undefined FUN_1026df90() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1026DF90u))(); }
XI_ORIG void sqxfXformTD_Update(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1026E1F0u))(a0); }
XI_ORIG void sqxfXformTD_Update_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1026E1F0u))(); }
XI_ORIG undefined FUN_1026e2c0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1026E2C0u))(); }
XI_ORIG void FUN_1026e2f0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1026E2F0u))(a0); }
XI_ORIG undefined FUN_1026e350() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1026E350u))(); }
XI_ORIG void FUN_1026e3d0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1026E3D0u))(a0); }
XI_ORIG int sub_1026E400() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1026E400u))(); }
XI_ORIG _DWORD * sub_1026E460(int a0, int a1) { return reinterpret_cast<_DWORD * (__cdecl *)(int, int)>(XI_GAME(0x1026E460u))(a0, a1); }
XI_ORIG undefined4 FUN_1026e530(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1026E530u))(a0); }
XI_ORIG void FUN_1026e590(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1026E590u))(a0); }
XI_ORIG int FUN_1026ebf0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026EBF0u))(a0, a1); }
XI_ORIG undefined4 FUN_1026ecc0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1026ECC0u))(); }
XI_ORIG int FUN_1026ef00(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026EF00u))(a0, a1); }
XI_ORIG undefined4 FUN_1026efb0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026EFB0u))(a0, a1); }
XI_ORIG undefined4 FUN_1026f080(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1026F080u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1026f1b0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1026F1B0u))(); }
XI_ORIG undefined4 FUN_1026f4f0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026F4F0u))(a0, a1); }
XI_ORIG void FUN_1026f580(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026F580u))(a0, a1); }
XI_ORIG void FUN_1026f7b0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026F7B0u))(a0, a1); }
XI_ORIG void FUN_1026f900(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1026F900u))(a0, a1, a2); }
XI_ORIG void FUN_1026f9a0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1026F9A0u))(a0, a1, a2); }
XI_ORIG void FUN_1026f9e0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026F9E0u))(a0, a1); }
XI_ORIG undefined4 FUN_1026fbb0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1026FBB0u))(a0); }
XI_ORIG undefined4 FUN_1026ff80(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1026FF80u))(a0, a1); }
XI_ORIG void FUN_102701b0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102701B0u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_10270290(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10270290u))(a0, a1); }
XI_ORIG void FUN_102703c0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102703C0u))(a0, a1, a2); }
XI_ORIG void FUN_10270410(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10270410u))(a0, a1); }
XI_ORIG void FUN_10270690(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10270690u))(a0); }
XI_ORIG undefined4 FUN_10270840(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10270840u))(a0); }
XI_ORIG undefined4 FUN_102709e0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102709E0u))(a0); }
XI_ORIG int sub_10270B60() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10270B60u))(); }
XI_ORIG void FUN_10270ec0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10270EC0u))(); }
XI_ORIG undefined4 FUN_10270f30(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10270F30u))(a0); }
XI_ORIG void FUN_10271030(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10271030u))(a0); }
XI_ORIG void FUN_102710b0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102710B0u))(a0, a1, a2, a3); }
XI_ORIG void FUN_102712e0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102712E0u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_102715f0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102715F0u))(a0); }
XI_ORIG void FUN_10271630(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10271630u))(a0, a1, a2, a3); }
XI_ORIG void sub_102717F0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x102717F0u))(); }
XI_ORIG char scnFini() { return reinterpret_cast<char (__cdecl *)()>(XI_GAME(0x10271B40u))(); }
XI_ORIG void FUN_10271b90(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10271B90u))(a0, a1); }
XI_ORIG void FUN_10271c30(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10271C30u))(a0, a1, a2); }
XI_ORIG void sub_10271C60(int a0, int a1) { reinterpret_cast<void (__cdecl *)(int, int)>(XI_GAME(0x10271C60u))(a0, a1); }
XI_ORIG void sub_10271C80(int a0, int a1) { reinterpret_cast<void (__cdecl *)(int, int)>(XI_GAME(0x10271C80u))(a0, a1); }
XI_ORIG void FUN_10271ca0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10271CA0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG void FUN_10271d00() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10271D00u))(); }
XI_ORIG void FUN_10271d70(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10271D70u))(a0); }
XI_ORIG _DWORD * scnGetFlags(void * a0) { return reinterpret_cast<_DWORD * (__cdecl *)(void *)>(XI_GAME(0x10271E90u))(a0); }
XI_ORIG void scnGetFlags_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10271E90u))(a0); }
XI_ORIG void FUN_10271f20(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10271F20u))(a0); }
XI_ORIG void FUN_102720a0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x102720A0u))(); }
XI_ORIG void FUN_102725a0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102725A0u))(a0, a1); }
XI_ORIG void FUN_10272630(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10272630u))(a0, a1); }
XI_ORIG void FUN_102726e0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102726E0u))(a0); }
XI_ORIG void FUN_10272750(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10272750u))(a0, a1); }
XI_ORIG void FUN_102727f0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102727F0u))(a0, a1); }
XI_ORIG void FUN_10272880(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10272880u))(a0, a1, a2); }
XI_ORIG void FUN_102729e0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102729E0u))(a0); }
XI_ORIG void FUN_10272a10(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10272A10u))(a0); }
XI_ORIG void FUN_10272ba0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10272BA0u))(); }
XI_ORIG int scnCameraGetInfo(int a0, void * a1, void * a2) { return reinterpret_cast<int (__cdecl *)(int, void *, void *)>(XI_GAME(0x10273370u))(a0, a1, a2); }
XI_ORIG void scnCameraGetInfo_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10273370u))(a0, a1, a2); }
XI_ORIG void FUN_102733b0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102733B0u))(a0); }
XI_ORIG void FUN_10273490(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10273490u))(a0, a1, a2); }
XI_ORIG void FUN_10273560(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10273560u))(a0); }
XI_ORIG void FUN_102736d0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102736D0u))(a0); }
XI_ORIG void FUN_102736e0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102736E0u))(a0, a1); }
XI_ORIG void FUN_10273810(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10273810u))(a0, a1); }
XI_ORIG _DWORD * sub_102738D0(void * a0) { return reinterpret_cast<_DWORD * (__cdecl *)(void *)>(XI_GAME(0x102738D0u))(a0); }
XI_ORIG undefined4 FUN_10273e80(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10273E80u))(a0, a1, a2); }
XI_ORIG int FUN_102741f0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102741F0u))(a0, a1); }
XI_ORIG undefined FUN_102742c0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102742C0u))(); }
XI_ORIG undefined4 FUN_10274370(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10274370u))(a0, a1, a2, a3); }
XI_ORIG void FUN_10274640(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10274640u))(a0); }
XI_ORIG void FUN_10274650(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10274650u))(a0, a1); }
XI_ORIG void FUN_10274670(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10274670u))(a0, a1, a2, a3); }
XI_ORIG long double FUN_102746c0(XI_ANY a0) { return reinterpret_cast<long double (__cdecl *)(XI_ANY)>(XI_GAME(0x102746C0u))(a0); }
XI_ORIG long double FUN_10274870(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<long double (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10274870u))(a0, a1); }
XI_ORIG long double FUN_102748f0(XI_ANY a0) { return reinterpret_cast<long double (__cdecl *)(XI_ANY)>(XI_GAME(0x102748F0u))(a0); }
XI_ORIG void FUN_102749c0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102749C0u))(a0, a1, a2); }
XI_ORIG void FUN_10274a20(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10274A20u))(a0, a1, a2, a3); }
XI_ORIG int PackVec4(int a0, float a1, int a2, int a3, int a4) { return reinterpret_cast<int (__cdecl *)(int, float, int, int, int)>(XI_GAME(0x10274D40u))(a0, a1, a2, a3, a4); }
XI_ORIG void PackVec4_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10274D40u))(a0, a1, a2, a3, a4); }
XI_ORIG int FUN_10275090() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10275090u))(); }
XI_ORIG int sub_10275090() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10275090u))(); }
XI_ORIG int sqMemInit() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10275150u))(); }
XI_ORIG void FUN_10275190(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10275190u))(a0); }
XI_ORIG unsigned long long FUN_102755c0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[2] = {a0.bits, a1.bits}; uint32_t ra = a2.bits, rc = 0, rd = a3.bits;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $8, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x102755C0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_102756a0(void * a0, void * a1) { reinterpret_cast<void (__fastcall *)(void *, void *)>(XI_GAME(0x102756A0u))(a0, a1); }
XI_ORIG void sqMemPoolRegister(int a0, int a1, int a2, void * a3) { reinterpret_cast<void (__cdecl *)(int, int, int, void *)>(XI_GAME(0x10275B60u))(a0, a1, a2, a3); }
XI_ORIG undefined4 * FUN_10275d40(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10275D40u))(a0, a1, a2, a3); }
XI_ORIG undefined4 * FUN_10275d80(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10275D80u))(a0, a1, a2, a3); }
XI_ORIG undefined4 * FUN_10275e90(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10275E90u))(a0, a1, a2, a3); }
XI_ORIG int sqFree(int a0, void * a1, int a2) { return reinterpret_cast<int (__cdecl *)(int, void *, int)>(XI_GAME(0x102760F0u))(a0, a1, a2); }
XI_ORIG undefined4 sqFree_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102760F0u))(a0, a1, a2); }
XI_ORIG _DWORD * sqgxContext_Create() { return reinterpret_cast<_DWORD * (__cdecl *)()>(XI_GAME(0x10276260u))(); }
XI_ORIG void sqgxContext_Init(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10276290u))(a0); }
XI_ORIG undefined FUN_10276770() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10276770u))(); }
XI_ORIG undefined sqgxContext_Dump() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102768E0u))(); }
XI_ORIG void FUN_10277ab0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10277AB0u))(a0); }
XI_ORIG void FUN_10277b70(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10277B70u))(a0); }
XI_ORIG void FUN_10278490(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10278490u))(a0); }
XI_ORIG void FUN_10278710(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10278710u))(a0, a1, a2, a3); }
XI_ORIG void FUN_10278990(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10278990u))(a0, a1, a2, a3); }
XI_ORIG void FUN_10278b50(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10278B50u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_10278e10(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10278E10u))(a0, a1); }
XI_ORIG void FUN_10278e90(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10278E90u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_10279530(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10279530u))(a0, a1); }
XI_ORIG void FUN_102796a0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102796A0u))(a0, a1); }
XI_ORIG undefined4 FUN_10279710(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10279710u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10279870(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10279870u))(a0, a1); }
XI_ORIG char * sqmdModel_GetItemName(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<char * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10279AB0u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_10279bf0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10279BF0u))(a0, a1, a2, a3); }
XI_ORIG void FUN_1027a220(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027A220u))(a0, a1); }
XI_ORIG void FUN_1027a250(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027A250u))(a0); }
XI_ORIG void thunk_FUN_1027ae50(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027A6F0u))(a0); }
XI_ORIG undefined4 FUN_1027a700(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027A700u))(a0, a1); }
XI_ORIG void FUN_1027a8a0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027A8A0u))(a0, a1); }
XI_ORIG void FUN_1027a9d0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027A9D0u))(a0, a1); }
XI_ORIG void FUN_1027aae0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027AAE0u))(a0, a1, a2, a3); }
XI_ORIG void FUN_1027aba0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027ABA0u))(a0, a1); }
XI_ORIG void FUN_1027abd0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027ABD0u))(a0, a1); }
XI_ORIG void FUN_1027ac30(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027AC30u))(a0, a1, a2); }
XI_ORIG void FUN_1027ae50(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027AE50u))(a0); }
XI_ORIG long double FUN_1027aec0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<long double (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027AEC0u))(a0, a1); }
XI_ORIG void FUN_1027aef0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027AEF0u))(a0); }
XI_ORIG undefined4 FUN_1027afb0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1027AFB0u))(a0); }
XI_ORIG void FUN_1027b060(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027B060u))(a0); }
XI_ORIG void FUN_1027b080(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027B080u))(a0, a1, a2); }
XI_ORIG void FUN_1027b170(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027B170u))(a0, a1, a2); }
XI_ORIG void FUN_1027b2c0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027B2C0u))(a0, a1, a2, a3); }
XI_ORIG void FUN_1027b5a0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027B5A0u))(a0); }
XI_ORIG undefined FUN_1027b610() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1027B610u))(); }
XI_ORIG int FUN_1027c900(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027C900u))(a0, a1); }
XI_ORIG int FUN_1027c980(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027C980u))(a0, a1); }
XI_ORIG void FUN_1027c9d0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1027C9D0u))(); }
XI_ORIG void FUN_1027c9d0_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027C9D0u))(a0); }
XI_ORIG void FUN_1027ccf0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027CCF0u))(a0, a1, a2); }
XI_ORIG void FUN_1027cd30(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027CD30u))(a0, a1, a2, a3); }
XI_ORIG void FUN_1027cfd0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027CFD0u))(a0, a1, a2, a3); }
XI_ORIG int FUN_1027d110(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027D110u))(a0, a1, a2, a3); }
XI_ORIG void FUN_1027d650(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027D650u))(a0, a1, a2, a3); }
XI_ORIG void FUN_1027dae0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027DAE0u))(a0); }
XI_ORIG void FUN_1027db80(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027DB80u))(a0); }
XI_ORIG uint32_t * FUN_1027dcc0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<uint32_t * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027DCC0u))(a0, a1); }
XI_ORIG void FUN_1027dd70(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027DD70u))(a0, a1); }
XI_ORIG void FUN_1027dd90(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027DD90u))(a0, a1); }
XI_ORIG int FUN_1027df70(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x1027DF70u))(a0); }
XI_ORIG void FUN_1027e060(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027E060u))(a0); }
XI_ORIG void FUN_1027e0d0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027E0D0u))(a0); }
XI_ORIG void FUN_1027e150(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027E150u))(a0, a1, a2); }
XI_ORIG void FUN_1027e190(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027E190u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_1027e3b0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1027E3B0u))(a0); }
XI_ORIG undefined4 FUN_1027e440(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1027E440u))(a0); }
XI_ORIG undefined4 FUN_1027e450(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x1027E450u))(a0); }
XI_ORIG void FUN_1027e490(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027E490u))(a0, a1); }
XI_ORIG void thunk_FUN_1027ebd0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027EAF0u))(a0); }
XI_ORIG void FUN_1027eb00() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1027EB00u))(); }
XI_ORIG void FUN_1027eb00_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027EB00u))(a0); }
XI_ORIG void M4fSet(void * a0, void * a1) { reinterpret_cast<void (__cdecl *)(void *, void *)>(XI_GAME(0x1027EB10u))(a0, a1); }
XI_ORIG void M4fSet_v1(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027EB10u))(a0, a1); }
XI_ORIG void FUN_1027eb50(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9, XI_ANY a10, XI_ANY a11, XI_ANY a12, XI_ANY a13, XI_ANY a14, XI_ANY a15, XI_ANY a16) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027EB50u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16); }
XI_ORIG void FUN_1027ebd0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1027EBD0u))(a0); }
XI_ORIG float * M4fTransform(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, void *)>(XI_GAME(0x1027EF20u))(a0, a1, a2); }
XI_ORIG void M4fTransform_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027EF20u))(a0, a1, a2); }
XI_ORIG void FUN_1027ef80(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027EF80u))(a0, a1, a2); }
XI_ORIG void Mat4x4_TransformVec4(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1027EFE0u))(a0, a1, a2); }
XI_ORIG int M4fTranspose(void * a0, void * a1) { return reinterpret_cast<int (__cdecl *)(void *, void *)>(XI_GAME(0x1027F070u))(a0, a1); }
XI_ORIG void M4fTranspose_v1(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027F070u))(a0, a1); }
XI_ORIG int sqMatrix4_1027F0E0(void * a0, int a1) { return reinterpret_cast<int (__cdecl *)(void *, int)>(XI_GAME(0x1027F0E0u))(a0, a1); }
XI_ORIG undefined4 sqMatrix4_1027F0E0_v1(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1027F0E0u))(a0, a1); }
XI_ORIG double sqMatrix4_1027FCC0(int a0) { return reinterpret_cast<double (__cdecl *)(int)>(XI_GAME(0x1027FCC0u))(a0); }
XI_ORIG float * sqMatrix4_10280140(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, void *)>(XI_GAME(0x10280140u))(a0, a1, a2); }
XI_ORIG void sqMatrix4_10280140_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10280140u))(a0, a1, a2); }
XI_ORIG undefined4 FUN_102806b0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102806B0u))(a0, a1, a2, a3); }
XI_ORIG int * sqgxSprite(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8) { return reinterpret_cast<int * (__cdecl *)(int, int, int, int, int, int, int, int, int)>(XI_GAME(0x10280DE0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG void sqgxSprite_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10280DE0u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG void FUN_102813b0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102813B0u))(a0, a1); }
XI_ORIG int ___setargv_3() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10281500u))(); }
XI_ORIG int sqgxTextureCreateFromImage(int a0, void * a1) { return reinterpret_cast<int (__cdecl *)(int, void *)>(XI_GAME(0x10281510u))(a0, a1); }
XI_ORIG int sqgxTextureAddZ(int a0, int a1, int a2) { return reinterpret_cast<int (__cdecl *)(int, int, int)>(XI_GAME(0x102816E0u))(a0, a1, a2); }
XI_ORIG int sqgxCeilLog2(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10281810u))(a0); }
XI_ORIG int sqgxTextureAdd(int a0, int a1, void * a2, void * a3) { return reinterpret_cast<int (__cdecl *)(int, int, void *, void *)>(XI_GAME(0x10281830u))(a0, a1, a2, a3); }
XI_ORIG int sqgxTextureAdd_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10281830u))(a0, a1, a2, a3); }
XI_ORIG void FUN_102819c0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102819C0u))(a0); }
XI_ORIG void FUN_10281a70(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10281A70u))(a0); }
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
XI_ORIG void FUN_10282590() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10282590u))(); }
XI_ORIG int nullsub_59() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10282630u))(); }
XI_ORIG void sqgxBlendAdd() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x102826C0u))(); }
XI_ORIG void FUN_102827d0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x102827D0u))(); }
XI_ORIG void sqgxBlendSub() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10282910u))(); }
XI_ORIG void sqgxBlendLin(float a0) { reinterpret_cast<void (__cdecl *)(float)>(XI_GAME(0x10282A30u))(a0); }
XI_ORIG int sqgxBlendTrsp() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10282B90u))(); }
XI_ORIG void sqgxBlendTrsp_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10282B90u))(); }
XI_ORIG void sqgxBlendAddDestAlpha() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10282DC0u))(); }
XI_ORIG void sqgxBlendZeroSrcAlpha() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10282FF0u))(); }
XI_ORIG int sqgxBlendMul() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10283110u))(); }
XI_ORIG int sqgxBlendOff() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10283220u))(); }
XI_ORIG void sqgxBlendOff_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10283220u))(); }
XI_ORIG int nullsub_31() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10283240u))(); }
XI_ORIG void nullsub_31_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10283240u))(); }
XI_ORIG int ___setargv_9() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10283250u))(); }
XI_ORIG void FUN_10283260(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10283260u))(a0, a1); }
XI_ORIG void FUN_10283280(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283280u))(a0); }
XI_ORIG void FUN_10283300(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283300u))(a0); }
XI_ORIG int sqgxColor(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x10283330u))(a0); }
XI_ORIG void sqgxColor_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283330u))(a0); }
XI_ORIG int sqgxAmbient(float a0) { return reinterpret_cast<int (__cdecl *)(float)>(XI_GAME(0x10283380u))(a0); }
XI_ORIG void sqgxAmbient_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283380u))(a0); }
XI_ORIG void FUN_102833a0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102833A0u))(a0); }
XI_ORIG void FUN_102833c0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102833C0u))(a0, a1, a2); }
XI_ORIG int sqgxColorUpdate() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10283420u))(); }
XI_ORIG void sqgxColorUpdate_v1() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10283420u))(); }
XI_ORIG int ___setargv_16() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10283870u))(); }
XI_ORIG int sqgxPixelSize(float a0, float a1) { return reinterpret_cast<int (__cdecl *)(float, float)>(XI_GAME(0x102838A0u))(a0, a1); }
XI_ORIG void FUN_10283970(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283970u))(a0); }
XI_ORIG int * sqgxViewport(float a0, float a1, float a2, float a3) { return reinterpret_cast<int * (__cdecl *)(float, float, float, float)>(XI_GAME(0x10283A10u))(a0, a1, a2, a3); }
XI_ORIG void sqgxPerspective(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10283C10u))(a0, a1, a2); }
XI_ORIG int sqgxMatrixCamera(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x10283D60u))(a0); }
XI_ORIG void sqgxMatrixCamera_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283D60u))(a0); }
XI_ORIG void FUN_10283da0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283DA0u))(a0); }
XI_ORIG void FUN_10283de0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283DE0u))(a0); }
XI_ORIG int sqgxMatrixShape(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x10283E00u))(a0); }
XI_ORIG void sqgxMatrixShape_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283E00u))(a0); }
XI_ORIG void FUN_10283f20(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283F20u))(a0); }
XI_ORIG int sqgxPassAdd(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10283F70u))(a0); }
XI_ORIG void sqgxPassAdd_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283F70u))(a0); }
XI_ORIG void sqgxPass(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10283FC0u))(a0); }
XI_ORIG undefined4 FUN_10284000(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10284000u))(a0, a1); }
XI_ORIG int sqgxMatrixUpdate() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10284410u))(); }
XI_ORIG int ___setargv_24() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10284890u))(); }
XI_ORIG int sqgxVertexShaderCreate(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x10284950u))(a0); }
XI_ORIG int * sqgxShader(int a0, int a1) { return reinterpret_cast<int * (__cdecl *)(int, int)>(XI_GAME(0x10284B20u))(a0, a1); }
XI_ORIG undefined4 sqgxShader_v1(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10284B20u))(a0, a1); }
XI_ORIG int nullsub_32() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10284EB0u))(); }
XI_ORIG int j_sqgxVertexShaderSelect() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x10284EC0u))(); }
XI_ORIG long double FUN_10284f20(XI_ANY a0) { return reinterpret_cast<long double (__cdecl *)(XI_ANY)>(XI_GAME(0x10284F20u))(a0); }
XI_ORIG undefined4 FUN_10284f50(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10284F50u))(a0, a1); }
XI_ORIG void FUN_10284fd0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10284FD0u))(a0, a1); }
XI_ORIG void FUN_10285000(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10285000u))(a0, a1); }
XI_ORIG void FUN_10285740(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10285740u))(a0); }
XI_ORIG undefined4 FUN_102857d0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102857D0u))(a0); }
XI_ORIG unsigned long long FUN_102857e0(XI_ANY a0, XI_ANY a1, XI_ANY a2) {
    uint32_t xs[2] = {a0.bits, a1.bits}; uint32_t ra = a2.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $8, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x102857E0u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG undefined4 FUN_102857f0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102857F0u))(a0); }
XI_ORIG void * sqDrawableDraw(int a0) { return reinterpret_cast<void * (__cdecl *)(int)>(XI_GAME(0x10285850u))(a0); }
XI_ORIG void sqDrawableDraw_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10285850u))(a0); }
template <class... R> XI_ORIG void sqDebugPrintf(void * a0, R... rest) { reinterpret_cast<void (__cdecl *)(void *, ...)>(XI_GAME(0x10285890u))(a0, rest...); }
XI_ORIG undefined8 sqDebugPrintf_v1(XI_ANY a0) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY)>(XI_GAME(0x10285890u))(a0); }
XI_ORIG undefined8 sqDebugPrintf_v2(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10285890u))(a0, a1, a2); }
XI_ORIG undefined8 sqDebugPrintf_v3(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10285890u))(a0, a1); }
XI_ORIG undefined8 sqDebugPrintf_v4(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10285890u))(a0, a1, a2, a3, a4); }
XI_ORIG int FUN_10288660(XI_ANY a0) { return reinterpret_cast<int (__cdecl *)(XI_ANY)>(XI_GAME(0x10288660u))(a0); }
XI_ORIG uint32_t FUN_10288680(XI_ANY a0) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY)>(XI_GAME(0x10288680u))(a0); }
XI_ORIG void FUN_10289620(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10289620u))(a0); }
XI_ORIG undefined FUN_10289670() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10289670u))(); }
XI_ORIG void FUN_102896a0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102896A0u))(a0); }
XI_ORIG void FUN_10289b40(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10289B40u))(a0); }
XI_ORIG void FUN_10289ba0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10289BA0u))(a0); }
XI_ORIG int sqopOpcodeGet(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10289EA0u))(a0, a1); }
XI_ORIG int sqopTrackSetIndex(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10289EF0u))(a0, a1); }
XI_ORIG undefined4 sqopTrackAdvance(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10289F10u))(a0); }
XI_ORIG void FUN_10289f50(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10289F50u))(a0); }
XI_ORIG undefined FUN_10289fb0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10289FB0u))(); }
XI_ORIG undefined FUN_1028a040() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1028A040u))(); }
XI_ORIG undefined4 FUN_1028a0e0() { return reinterpret_cast<undefined4 (__cdecl *)()>(XI_GAME(0x1028A0E0u))(); }
XI_ORIG undefined4 * FUN_1028a0f0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028A0F0u))(a0, a1, a2, a3, a4); }
XI_ORIG int sub_1028A730(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(XI_GAME(0x1028A730u))(a0, a1); }
XI_ORIG int sub_1028A770(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(XI_GAME(0x1028A770u))(a0, a1); }
XI_ORIG int sub_1028A790(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x1028A790u))(a0); }
XI_ORIG int * sqgxShapeBegin() { return reinterpret_cast<int * (__cdecl *)()>(XI_GAME(0x1028AFE0u))(); }
XI_ORIG int sub_1028BB10(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x1028BB10u))(a0); }
XI_ORIG int sub_1028BB70() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1028BB70u))(); }
XI_ORIG int sub_1028BBD0() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x1028BBD0u))(); }
XI_ORIG undefined4 * FUN_1028d6d0() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x1028D6D0u))(); }
XI_ORIG void FUN_1028d6f0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1028D6F0u))(a0); }
XI_ORIG undefined FUN_1028d750() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1028D750u))(); }
XI_ORIG undefined sqcoConnectorPrint() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1028D770u))(); }
XI_ORIG long double FUN_1028d990(XI_ANY a0) { return reinterpret_cast<long double (__cdecl *)(XI_ANY)>(XI_GAME(0x1028D990u))(a0); }
XI_ORIG void FUN_1028d9a0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1028D9A0u))(a0); }
XI_ORIG void FUN_1028d9c0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1028D9C0u))(a0, a1); }
XI_ORIG void FUN_1028dbb0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1028DBB0u))(a0, a1); }
XI_ORIG undefined4 * FUN_1028dc00() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x1028DC00u))(); }
XI_ORIG void FUN_1028dc20(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1028DC20u))(a0); }
XI_ORIG undefined FUN_1028dca0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1028DCA0u))(); }
XI_ORIG undefined4 FUN_1028de10(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028DE10u))(a0, a1, a2, a3); }
XI_ORIG undefined FUN_1028e180() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1028E180u))(); }
XI_ORIG void FUN_1028e1b0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1028E1B0u))(a0); }
XI_ORIG void FUN_1028e350(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1028E350u))(a0); }
XI_ORIG void FUN_1028ec30(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028EC30u))(a0, a1, a2); }
XI_ORIG void FUN_1028ec50(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028EC50u))(a0, a1, a2); }
XI_ORIG void FUN_1028ecc0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028ECC0u))(a0, a1, a2); }
XI_ORIG void FUN_1028ece0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028ECE0u))(a0, a1, a2); }
XI_ORIG int FUN_1028ed00(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028ED00u))(a0, a1, a2); }
XI_ORIG int FUN_1028ed50(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028ED50u))(a0, a1, a2); }
XI_ORIG int FUN_1028ede0(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028EDE0u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_1028efb0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1028EFB0u))(a0); }
XI_ORIG int FUN_1028f130(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1028F130u))(a0, a1); }
XI_ORIG void FUN_1028f220(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028F220u))(a0, a1, a2); }
XI_ORIG void FUN_1028f2e0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028F2E0u))(a0, a1, a2); }
XI_ORIG void FUN_1028f4a0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1028F4A0u))(a0, a1); }
XI_ORIG void FUN_1028f4c0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1028F4C0u))(a0, a1); }
XI_ORIG int sqsnScene_1028F5F0(int a0, int a1, int a2) { return reinterpret_cast<int (__cdecl *)(int, int, int)>(XI_GAME(0x1028F5F0u))(a0, a1, a2); }
XI_ORIG void sqsnScene_1028F5F0_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1028F5F0u))(a0, a1, a2); }
XI_ORIG void FUN_1028f6e0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1028F6E0u))(a0, a1); }
XI_ORIG void sqsnSceneDraw(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1028F8C0u))(a0); }
XI_ORIG int sqsnSceneLayerDraw(void * a0, int a1) { return reinterpret_cast<int (__cdecl *)(void *, int)>(XI_GAME(0x1028FDD0u))(a0, a1); }
XI_ORIG void FUN_1028ff80(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1028FF80u))(a0); }
XI_ORIG int sqsnSceneLayerDrawShadowed(void * a0, int a1) { return reinterpret_cast<int (__cdecl *)(void *, int)>(XI_GAME(0x1028FFF0u))(a0, a1); }
XI_ORIG int sqsnSceneLayerSnapShotComposite(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(XI_GAME(0x102902C0u))(a0, a1); }
XI_ORIG int nullsub_60(_DWORD a0, _DWORD a1) { return reinterpret_cast<int (__cdecl *)(_DWORD, _DWORD)>(XI_GAME(0x10290430u))(a0, a1); }
XI_ORIG int sqsnSceneLayerSort(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(XI_GAME(0x10290440u))(a0, a1); }
XI_ORIG void FUN_10290610(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10290610u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_102907c0(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102907C0u))(a0, a1, a2); }
XI_ORIG void FUN_10290840(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10290840u))(a0, a1, a2); }
XI_ORIG undefined FUN_102908e0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102908E0u))(); }
XI_ORIG void FUN_10290910(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10290910u))(a0); }
XI_ORIG undefined FUN_102910e0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102910E0u))(); }
XI_ORIG void FUN_10291110(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10291110u))(a0); }
XI_ORIG undefined FUN_10291170() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10291170u))(); }
XI_ORIG int sqrdCamera_Apply(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(XI_GAME(0x102911E0u))(a0); }
XI_ORIG void sqrdCamera_Apply_v1(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102911E0u))(a0); }
XI_ORIG void FUN_102912f0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102912F0u))(a0); }
XI_ORIG void FUN_10291390(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10291390u))(a0); }
XI_ORIG undefined FUN_10291430() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10291430u))(); }
XI_ORIG void FUN_10291460() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10291460u))(); }
XI_ORIG undefined FUN_10291470() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10291470u))(); }
XI_ORIG void FUN_10291ba0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10291BA0u))(a0); }
XI_ORIG undefined FUN_10291c20() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10291C20u))(); }
XI_ORIG void FUN_10291c50() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x10291C50u))(); }
XI_ORIG undefined FUN_10291c60() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10291C60u))(); }
XI_ORIG undefined FUN_10291fe0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10291FE0u))(); }
XI_ORIG unsigned long long FUN_10292130(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) {
    uint32_t xs[3] = {a0.bits, a1.bits, a2.bits}; uint32_t ra = a3.bits, rc = 0, rd = 0;
    __asm__ volatile("pushl 8(%%esi)\n\t""pushl 4(%%esi)\n\t""pushl 0(%%esi)\n\t""call *%%edi\n\t""addl $12, %%esp\n\t"
        : "+a"(ra), "+c"(rc), "+d"(rd) : "S"(xs), "D"(0x10292130u) : "memory", "cc", "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
    return ((unsigned long long)rd << 32) | ra;
}
XI_ORIG void FUN_10292490(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10292490u))(a0, a1); }
XI_ORIG undefined4 * FUN_102924f0() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x102924F0u))(); }
XI_ORIG void FUN_10292520(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10292520u))(a0); }
XI_ORIG undefined FUN_102925a0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102925A0u))(); }
XI_ORIG undefined FUN_10292740() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10292740u))(); }
XI_ORIG void FUN_10293000(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10293000u))(a0, a1); }
XI_ORIG void FUN_10293f00(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10293F00u))(a0, a1); }
XI_ORIG undefined4 * FUN_10293f80() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x10293F80u))(); }
XI_ORIG void FUN_10293fa0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10293FA0u))(a0); }
XI_ORIG undefined FUN_10294000() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10294000u))(); }
XI_ORIG undefined sqmoFrameChannel_Dump() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102940A0u))(); }
XI_ORIG void FUN_102944e0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102944E0u))(a0, a1); }
XI_ORIG undefined4 * FUN_102957b0() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x102957B0u))(); }
XI_ORIG void FUN_102957d0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102957D0u))(a0); }
XI_ORIG undefined FUN_10295830() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10295830u))(); }
XI_ORIG undefined sqmoKeyChannel_Dump() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102958B0u))(); }
XI_ORIG undefined FUN_10295a20() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10295A20u))(); }
XI_ORIG undefined4 * FUN_10295e70() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x10295E70u))(); }
XI_ORIG void FUN_10295e90(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10295E90u))(a0); }
XI_ORIG undefined FUN_10295ef0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10295EF0u))(); }
XI_ORIG undefined FUN_10295f30() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10295F30u))(); }
XI_ORIG undefined4 FUN_102960a0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102960A0u))(a0, a1); }
XI_ORIG void FUN_10296410(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10296410u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9); }
XI_ORIG long double FUN_102967e0(XI_ANY a0) { return reinterpret_cast<long double (__cdecl *)(XI_ANY)>(XI_GAME(0x102967E0u))(a0); }
XI_ORIG void FUN_102972d0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102972D0u))(a0); }
XI_ORIG int sub_10297320(int a0, int a1, int a2) { return reinterpret_cast<int (__cdecl *)(int, int, int)>(XI_GAME(0x10297320u))(a0, a1, a2); }
XI_ORIG unsigned int sqsnShadowVolumeExtrudeTriList(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, void * a8, float a9, int a10) { return reinterpret_cast<unsigned int (__cdecl *)(int, int, int, int, int, int, int, int, void *, float, int)>(XI_GAME(0x102979E0u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }
XI_ORIG int sqsnShadowVolumeExtrudeTriStrip(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, void * a8, float a9, int a10) { return reinterpret_cast<int (__cdecl *)(int, int, int, int, int, int, int, int, void *, float, int)>(XI_GAME(0x10297B10u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }
XI_ORIG int sqsnShadowVolumeSetPass(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10297CC0u))(a0); }
XI_ORIG int sqsnShadowComputeFacing(int a0, void * a1, int a2, int a3, void * a4, int a5, void * a6) { return reinterpret_cast<int (__cdecl *)(int, void *, int, int, void *, int, void *)>(XI_GAME(0x10297F90u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int sqsnShadowVolumeBeginVB() { return reinterpret_cast<int (__cdecl *)()>(XI_GAME(0x102981E0u))(); }
XI_ORIG int sqgxShadowPlaneMatrix(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(XI_GAME(0x10298350u))(a0); }
XI_ORIG undefined4 * FUN_102984b0() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x102984B0u))(); }
XI_ORIG void FUN_10298570(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10298570u))(a0); }
XI_ORIG undefined4 * FUN_10298b30() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x10298B30u))(); }
XI_ORIG undefined FUN_10298b60() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10298B60u))(); }
XI_ORIG void FUN_10298b90(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10298B90u))(a0); }
XI_ORIG undefined sqrdTexturePrint() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x10298C10u))(); }
XI_ORIG void FUN_10298c50(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x10298C50u))(a0); }
XI_ORIG int FUN_10298e10(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<int (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10298E10u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined4 FUN_102999a0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102999A0u))(a0, a1); }
XI_ORIG void FUN_1029a000(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1029A000u))(a0); }
XI_ORIG undefined FUN_1029a060() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1029A060u))(); }
XI_ORIG undefined FUN_1029a0e0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1029A0E0u))(); }
XI_ORIG undefined4 FUN_1029a330(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x1029A330u))(a0, a1); }
XI_ORIG void FUN_1029a450(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1029A450u))(a0); }
XI_ORIG void FUN_1029b120(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1029B120u))(a0); }
XI_ORIG undefined FUN_1029b1a0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x1029B1A0u))(); }
XI_ORIG void FUN_1029b5c0() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1029B5C0u))(); }
XI_ORIG undefined4 * FUN_1029b620() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x1029B620u))(); }
XI_ORIG void FUN_1029bb50() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x1029BB50u))(); }
XI_ORIG void FUN_1029bcd0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1029BCD0u))(a0); }
XI_ORIG void FUN_1029bd40(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1029BD40u))(a0); }
XI_ORIG int sub_1029BD50(int a0, int a1, void * a2, void * a3) { return reinterpret_cast<int (__cdecl *)(int, int, void *, void *)>(XI_GAME(0x1029BD50u))(a0, a1, a2, a3); }
XI_ORIG int sub_1029BF50(int a0, void * a1, void * a2) { return reinterpret_cast<int (__cdecl *)(int, void *, void *)>(XI_GAME(0x1029BF50u))(a0, a1, a2); }
XI_ORIG undefined4 * FUN_1029c260(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined4 * (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x1029C260u))(a0, a1, a2, a3, a4); }
XI_ORIG void FUN_1029c460(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x1029C460u))(a0); }
XI_ORIG float * sub_1029C480(int a0, void * a1, void * a2, int a3, int a4, void * a5, void * a6) { return reinterpret_cast<float * (__cdecl *)(int, void *, void *, int, int, void *, void *)>(XI_GAME(0x1029C480u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG void sub_1029C610(int a0, void * a1, int a2, int a3) { reinterpret_cast<void (__cdecl *)(int, void *, int, int)>(XI_GAME(0x1029C610u))(a0, a1, a2, a3); }
XI_ORIG int sub_1029C950(int a0, int a1, void * a2, void * a3, void * a4, void * a5) { return reinterpret_cast<int (__cdecl *)(int, int, void *, void *, void *, void *)>(XI_GAME(0x1029C950u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG void FUN_102a1f20(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102A1F20u))(a0, a1); }
XI_ORIG undefined4 FUN_102a1fa0(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x102A1FA0u))(a0); }
XI_ORIG undefined4 * FUN_102a2000() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x102A2000u))(); }
XI_ORIG void FUN_102a2020(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102A2020u))(a0); }
XI_ORIG undefined FUN_102a2080() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102A2080u))(); }
XI_ORIG undefined sqshDispListPrint() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102A20D0u))(); }
XI_ORIG void FUN_102a2130(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102A2130u))(a0, a1); }
XI_ORIG undefined4 FUN_102a2160(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102A2160u))(a0, a1); }
XI_ORIG undefined4 FUN_102a2190(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102A2190u))(a0, a1); }
XI_ORIG void sqsnPlanarShadowDraw(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102A2530u))(a0, a1); }
XI_ORIG void FUN_102a2720(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102A2720u))(a0, a1); }
XI_ORIG void FUN_102a2a30(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102A2A30u))(a0, a1); }
XI_ORIG void FUN_102a2a50(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102A2A50u))(a0, a1, a2); }
XI_ORIG void FUN_102a2c00(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102A2C00u))(a0, a1, a2, a3); }
XI_ORIG undefined FUN_102a3360() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102A3360u))(); }
XI_ORIG void FUN_102a3390(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102A3390u))(a0); }
XI_ORIG undefined FUN_102a33f0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102A33F0u))(); }
XI_ORIG void FUN_102a34d0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102A34D0u))(a0); }
XI_ORIG void FUN_102a3760() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x102A3760u))(); }
XI_ORIG void FUN_102a3770() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x102A3770u))(); }
XI_ORIG undefined4 * FUN_102a9520() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x102A9520u))(); }
XI_ORIG void FUN_102a9550(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102A9550u))(a0); }
XI_ORIG undefined FUN_102a95a0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102A95A0u))(); }
XI_ORIG undefined sqVtxPrint() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102A96C0u))(); }
XI_ORIG void FUN_102a9980(XI_ANY a0, XI_ANY a1, XI_ANY a2) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x102A9980u))(a0, a1, a2); }
XI_ORIG void FUN_102a9ef0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102A9EF0u))(a0); }
XI_ORIG void FUN_102a9f30(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102A9F30u))(a0); }
XI_ORIG void FUN_102aa2f0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102AA2F0u))(a0); }
XI_ORIG undefined FUN_102aa370() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102AA370u))(); }
XI_ORIG undefined FUN_102aa420() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102AA420u))(); }
XI_ORIG undefined FUN_102aa580() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102AA580u))(); }
XI_ORIG void FUN_102acb80(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102ACB80u))(a0); }
XI_ORIG undefined FUN_102acbe0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102ACBE0u))(); }
XI_ORIG undefined sqshPointsPrint() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102ACC30u))(); }
XI_ORIG void FUN_102acf40(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102ACF40u))(a0); }
XI_ORIG undefined FUN_102acfc0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102ACFC0u))(); }
XI_ORIG undefined sqshCurvesPrint() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102AD090u))(); }
XI_ORIG void FUN_102ad1d0(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102AD1D0u))(a0, a1); }
XI_ORIG void FUN_102ad770() { reinterpret_cast<void (__cdecl *)()>(XI_GAME(0x102AD770u))(); }
XI_ORIG void FUN_102ad770_v1(XI_ANY a0, XI_ANY a1) { reinterpret_cast<void (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x102AD770u))(a0, a1); }
XI_ORIG void FUN_102aec50(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102AEC50u))(a0); }
XI_ORIG undefined4 * FUN_102aee90() { return reinterpret_cast<undefined4 * (__cdecl *)()>(XI_GAME(0x102AEE90u))(); }
XI_ORIG void FUN_102aeeb0(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102AEEB0u))(a0); }
XI_ORIG undefined FUN_102aefe0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102AEFE0u))(); }
XI_ORIG undefined FUN_102af020() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102AF020u))(); }
XI_ORIG void FUN_102af170(XI_ANY a0) { reinterpret_cast<void (__cdecl *)(XI_ANY)>(XI_GAME(0x102AF170u))(a0); }
XI_ORIG undefined FUN_102af2a0() { return reinterpret_cast<undefined (__cdecl *)()>(XI_GAME(0x102AF2A0u))(); }
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
XI_ORIG int fn_102D2F40(void * a0, void * a1, void * a2, void * a3, int a4, void * a5, int a6) { return reinterpret_cast<int (__stdcall *)(void *, void *, void *, void *, int, void *, int)>(XI_GAME(0x102D2F40u))(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int fn_102D2FB9(void * a0, unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, _D3DFORMAT a5, __int32 a6, void * a7) { return reinterpret_cast<int (__stdcall *)(void *, unsigned int, unsigned int, unsigned int, unsigned int, _D3DFORMAT, __int32, void *)>(XI_GAME(0x102D2FB9u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG int fn_102D301F(void * a0, unsigned int a1, unsigned int a2, unsigned int a3, _D3DFORMAT a4, __int32 a5, void * a6) { return reinterpret_cast<int (__stdcall *)(void *, unsigned int, unsigned int, unsigned int, _D3DFORMAT, __int32, void *)>(XI_GAME(0x102D301Fu))(a0, a1, a2, a3, a4, a5, a6); }
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
XI_ORIG undefined4 FUN_10311386() { return reinterpret_cast<undefined4 (__stdcall *)()>(XI_GAME(0x10311386u))(); }
XI_ORIG void FUN_103113c8() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x103113C8u))(); }
XI_ORIG void FUN_103113d3() { reinterpret_cast<void (__stdcall *)()>(XI_GAME(0x103113D3u))(); }
XI_ORIG void operator_delete_10311BB0(void * a0) { reinterpret_cast<void (__cdecl *)(void *)>(XI_GAME(0x10311BB0u))(a0); }
XI_ORIG undefined8 operator_delete_10311BB0_v1(XI_ANY a0) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY)>(XI_GAME(0x10311BB0u))(a0); }
XI_ORIG undefined8 operator_delete_10311BB0_v2() { return reinterpret_cast<undefined8 (__cdecl *)()>(XI_GAME(0x10311BB0u))(); }
XI_ORIG void * Allocator_Generic(unsigned int a0) { return reinterpret_cast<void * (__cdecl *)(unsigned int)>(XI_GAME(0x10311BBBu))(a0); }
XI_ORIG undefined8 Allocator_Generic_v1(XI_ANY a0) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY)>(XI_GAME(0x10311BBBu))(a0); }
XI_ORIG undefined8 Allocator_Generic_v2() { return reinterpret_cast<undefined8 (__cdecl *)()>(XI_GAME(0x10311BBBu))(); }
XI_ORIG uint32_t FUN_10311c7a(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<uint32_t (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10311C7Au))(a0, a1); }
XI_ORIG uint32_t * FUN_10311eb0(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<uint32_t * (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10311EB0u))(a0, a1); }
XI_ORIG undefined4 FUN_10311f30(XI_ANY a0) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY)>(XI_GAME(0x10311F30u))(a0); }
XI_ORIG undefined4 FUN_10311fde(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined4 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10311FDEu))(a0, a1); }
XI_ORIG undefined8 _sprintf(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10311FF1u))(a0, a1, a2); }
XI_ORIG undefined8 _sprintf_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10311FF1u))(a0, a1, a2, a3); }
XI_ORIG undefined8 _sprintf_v2(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10311FF1u))(a0, a1, a2, a3, a4); }
XI_ORIG undefined8 _sprintf_v3(XI_ANY a0, XI_ANY a1) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY)>(XI_GAME(0x10311FF1u))(a0, a1); }
XI_ORIG undefined8 _sprintf_v4(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10311FF1u))(a0, a1, a2, a3, a4, a5); }
XI_ORIG undefined8 _sprintf_v5(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10311FF1u))(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG undefined8 _sprintf_v6(XI_ANY a0, XI_ANY a1, XI_ANY a2, XI_ANY a3, XI_ANY a4, XI_ANY a5, XI_ANY a6, XI_ANY a7, XI_ANY a8, XI_ANY a9, XI_ANY a10, XI_ANY a11, XI_ANY a12, XI_ANY a13, XI_ANY a14, XI_ANY a15, XI_ANY a16, XI_ANY a17, XI_ANY a18, XI_ANY a19, XI_ANY a20, XI_ANY a21, XI_ANY a22) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10311FF1u))(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22); }
XI_ORIG char * _strncpy(void * a0, void * a1, size_t a2) { return reinterpret_cast<char * (__cdecl *)(void *, void *, size_t)>(XI_GAME(0x10312050u))(a0, a1, a2); }
XI_ORIG undefined8 _strncpy_v1(XI_ANY a0, XI_ANY a1, XI_ANY a2) { return reinterpret_cast<undefined8 (__cdecl *)(XI_ANY, XI_ANY, XI_ANY)>(XI_GAME(0x10312050u))(a0, a1, a2); }
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
constexpr uint32_t App_CreateInputSystem = 0x10001BB0u;
constexpr uint32_t CApp_CopyTextToClipboard = 0x10001CC0u;
constexpr uint32_t sub_10001E70 = 0x10001E70u;
constexpr uint32_t FUN_10001f40 = 0x10001F40u;
constexpr uint32_t Gfx_CopySurfaceRectToTexture = 0x10002040u;
constexpr uint32_t Gfx_ProbeDeviceCaps = 0x10002130u;
constexpr uint32_t sub_10002440 = 0x10002440u;
constexpr uint32_t Gfx_SetGammaRGB = 0x10002C00u;
constexpr uint32_t FUN_10002d80 = 0x10002D80u;
constexpr uint32_t sub_10002D80 = 0x10002D80u;
constexpr uint32_t sub_10002E60 = 0x10002E60u;
constexpr uint32_t FUN_10002e80 = 0x10002E80u;
constexpr uint32_t FUN_10002ec0 = 0x10002EC0u;
constexpr uint32_t FUN_10002ee0 = 0x10002EE0u;
constexpr uint32_t D3DHolder_Constructor = 0x10003070u;
constexpr uint32_t D3DHolder_Initialize = 0x10003110u;
constexpr uint32_t Gfx_TryCreateDeviceForMode = 0x100033C0u;
constexpr uint32_t D3DHolder_CheckDepthFormat = 0x10003460u;
constexpr uint32_t Gfx_CreateDevice = 0x100034B0u;
constexpr uint32_t nullsub_71 = 0x100035A0u;
constexpr uint32_t sub_100035F0 = 0x100035F0u;
constexpr uint32_t FUN_10003640 = 0x10003640u;
constexpr uint32_t sub_10003650 = 0x10003650u;
constexpr uint32_t FUN_10003660 = 0x10003660u;
constexpr uint32_t FUN_10003700 = 0x10003700u;
constexpr uint32_t FUN_10003710 = 0x10003710u;
constexpr uint32_t FUN_100037d0 = 0x100037D0u;
constexpr uint32_t FUN_10003890 = 0x10003890u;
constexpr uint32_t Debug_DrawTextTable = 0x10003B60u;
constexpr uint32_t Debug_DrawWorldText = 0x10003D70u;
constexpr uint32_t FUN_10005600 = 0x10005600u;
constexpr uint32_t FUN_10005660 = 0x10005660u;
constexpr uint32_t FUN_100057d0 = 0x100057D0u;
constexpr uint32_t FUN_100058f0 = 0x100058F0u;
constexpr uint32_t sub_10005A40 = 0x10005A40u;
constexpr uint32_t DirectInput_InstallMouse = 0x10007EF0u;
constexpr uint32_t FUN_10008770 = 0x10008770u;
constexpr uint32_t sub_10008830 = 0x10008830u;
constexpr uint32_t sub_100088F0 = 0x100088F0u;
constexpr uint32_t FUN_10008d90 = 0x10008D90u;
constexpr uint32_t FUN_10008e10 = 0x10008E10u;
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
constexpr uint32_t sub_10009D20 = 0x10009D20u;
constexpr uint32_t CDx_SetViewport = 0x10009DC0u;
constexpr uint32_t CDx_GetRenderTargetTop = 0x10009E00u;
constexpr uint32_t CDx_GetRenderTargetTop_v1 = 0x10009E00u;
constexpr uint32_t CDx_PushRenderTarget = 0x10009E20u;
constexpr uint32_t CDx_PushRenderTarget_v1 = 0x10009E20u;
constexpr uint32_t sub_10009EA0 = 0x10009EA0u;
constexpr uint32_t sub_10009EE0 = 0x10009EE0u;
constexpr uint32_t CDx_PushRenderTargetTexture = 0x10009F20u;
constexpr uint32_t CDx_PushRenderTargetTexture_v1 = 0x10009F20u;
constexpr uint32_t FUN_1000a0a0 = 0x1000A0A0u;
constexpr uint32_t CDx_SetTexture = 0x1000A1D0u;
constexpr uint32_t CDx_SetTexture_v1 = 0x1000A1D0u;
constexpr uint32_t sub_1000A200 = 0x1000A200u;
constexpr uint32_t sub_1000A230 = 0x1000A230u;
constexpr uint32_t CDx_PopRenderTargetDefault = 0x1000A290u;
constexpr uint32_t CDx_PopRenderTarget = 0x1000A2B0u;
constexpr uint32_t CDx_PopRenderTarget_v1 = 0x1000A2B0u;
constexpr uint32_t sub_1000A390 = 0x1000A390u;
constexpr uint32_t CDx_DrawScreenQuadAtDepth = 0x1000A3B0u;
constexpr uint32_t CDx_DrawTextureToViewport = 0x1000A4F0u;
constexpr uint32_t FUN_1000a550 = 0x1000A550u;
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
constexpr uint32_t nullsub_36 = 0x1000BB10u;
constexpr uint32_t nullsub_37 = 0x1000BB20u;
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
constexpr uint32_t FUN_1000d150 = 0x1000D150u;
constexpr uint32_t j_CEnv_UpdateDualTargetMode = 0x1000D3F0u;
constexpr uint32_t nullsub_38 = 0x1000D620u;
constexpr uint32_t FUN_1000d630 = 0x1000D630u;
constexpr uint32_t CEnv_IsStereoMode = 0x1000D660u;
constexpr uint32_t CEnv_IsStereoMode_v1 = 0x1000D660u;
constexpr uint32_t FUN_1000d680 = 0x1000D680u;
constexpr uint32_t FUN_1000d6b0 = 0x1000D6B0u;
constexpr uint32_t App_RegisterSystemDats = 0x10010370u;
constexpr uint32_t FUN_10010520 = 0x10010520u;
constexpr uint32_t sub_10010520 = 0x10010520u;
constexpr uint32_t App_Constructor = 0x10010700u;
constexpr uint32_t App_SetSeqStep = 0x10010CE0u;
constexpr uint32_t App_AdvanceSeqStep = 0x10010D00u;
constexpr uint32_t App_AdvanceSeqStep_v1 = 0x10010D00u;
constexpr uint32_t App_InitScene = 0x10010D20u;
constexpr uint32_t Frame_Render = 0x10011A00u;
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
constexpr uint32_t Game_InGameSubState0 = 0x100141C0u;
constexpr uint32_t FUN_10014200 = 0x10014200u;
constexpr uint32_t FUN_10014220 = 0x10014220u;
constexpr uint32_t FUN_100142a0 = 0x100142A0u;
constexpr uint32_t FUN_100142b0 = 0x100142B0u;
constexpr uint32_t FUN_10014350 = 0x10014350u;
constexpr uint32_t sub_100149B0 = 0x100149B0u;
constexpr uint32_t sub_10014A20 = 0x10014A20u;
constexpr uint32_t sub_10014A30 = 0x10014A30u;
constexpr uint32_t sub_10014AD0 = 0x10014AD0u;
constexpr uint32_t sub_10014C10 = 0x10014C10u;
constexpr uint32_t FUN_10014c60 = 0x10014C60u;
constexpr uint32_t FUN_10014c90 = 0x10014C90u;
constexpr uint32_t FUN_10014cb0 = 0x10014CB0u;
constexpr uint32_t FUN_10014ce0 = 0x10014CE0u;
constexpr uint32_t Camera_GetFrameScale = 0x10014CF0u;
constexpr uint32_t Camera_GetFrameScale_v1 = 0x10014CF0u;
constexpr uint32_t Camera_GetFrameScale_v2 = 0x10014CF0u;
constexpr uint32_t GetFrameCounter = 0x10014D50u;
constexpr uint32_t GetFrameCounter_v1 = 0x10014D50u;
constexpr uint32_t FUN_10014d60 = 0x10014D60u;
constexpr uint32_t Camera_GetFloat2DC = 0x10014D70u;
constexpr uint32_t Camera_GetFloat2E0 = 0x10014D80u;
constexpr uint32_t Camera_SetClearColorRGB = 0x10014D90u;
constexpr uint32_t FUN_10014db0 = 0x10014DB0u;
constexpr uint32_t FUN_10014e00 = 0x10014E00u;
constexpr uint32_t FUN_10014e20 = 0x10014E20u;
constexpr uint32_t FUN_10014e60 = 0x10014E60u;
constexpr uint32_t FUN_10014e80 = 0x10014E80u;
constexpr uint32_t FUN_10014eb0 = 0x10014EB0u;
constexpr uint32_t FUN_10014ed0 = 0x10014ED0u;
constexpr uint32_t sub_10014EF0 = 0x10014EF0u;
constexpr uint32_t sub_10014F20 = 0x10014F20u;
constexpr uint32_t sub_10015050 = 0x10015050u;
constexpr uint32_t Camera_SetViewMatrix = 0x100150D0u;
constexpr uint32_t FUN_10015100 = 0x10015100u;
constexpr uint32_t FUN_100151a0 = 0x100151A0u;
constexpr uint32_t Camera_SetMatrix154 = 0x100151F0u;
constexpr uint32_t Camera_GetViewMatrixPtr = 0x10015220u;
constexpr uint32_t Camera_GetViewMatrixPtr_v1 = 0x10015220u;
constexpr uint32_t sub_10015240 = 0x10015240u;
constexpr uint32_t Camera_GetField50 = 0x10015250u;
constexpr uint32_t Camera_GetField50_v1 = 0x10015250u;
constexpr uint32_t Camera_GetMatrix154Ptr = 0x10015270u;
constexpr uint32_t Camera_GetMatrix154Ptr_v1 = 0x10015270u;
constexpr uint32_t FUN_10015290 = 0x10015290u;
constexpr uint32_t FUN_100152b0 = 0x100152B0u;
constexpr uint32_t FUN_100152c0 = 0x100152C0u;
constexpr uint32_t sub_10015360 = 0x10015360u;
constexpr uint32_t FUN_100153a0 = 0x100153A0u;
constexpr uint32_t sub_100153A0 = 0x100153A0u;
constexpr uint32_t FUN_100153d0 = 0x100153D0u;
constexpr uint32_t FUN_10015490 = 0x10015490u;
constexpr uint32_t Camera_ApplyViewProjection = 0x10015510u;
constexpr uint32_t FUN_100155d0 = 0x100155D0u;
constexpr uint32_t FUN_10015600 = 0x10015600u;
constexpr uint32_t FUN_10015610 = 0x10015610u;
constexpr uint32_t Camera_GetViewportWidth = 0x10015680u;
constexpr uint32_t Camera_GetViewportHeight = 0x10015690u;
constexpr uint32_t FUN_100156a0 = 0x100156A0u;
constexpr uint32_t sub_100156A0 = 0x100156A0u;
constexpr uint32_t fn_10015740 = 0x10015740u;
constexpr uint32_t App_WinMain = 0x100157E0u;
constexpr uint32_t sub_100163C0 = 0x100163C0u;
constexpr uint32_t sub_100163E0 = 0x100163E0u;
constexpr uint32_t App_SetGlobal_104569C8 = 0x10016400u;
constexpr uint32_t App_SetGlobal_104569C8_v1 = 0x10016400u;
constexpr uint32_t FUN_10016410 = 0x10016410u;
constexpr uint32_t sub_10016410 = 0x10016410u;
constexpr uint32_t FUN_100164b0 = 0x100164B0u;
constexpr uint32_t App_SetErrorCode = 0x100164C0u;
constexpr uint32_t sub_10016560 = 0x10016560u;
constexpr uint32_t GammaRamp_SetRGB = 0x100170A0u;
constexpr uint32_t GammaRamp_GetRGB = 0x100170D0u;
constexpr uint32_t GammaRamp_GetChannelSamples = 0x10017100u;
constexpr uint32_t GammaRamp_GetChannelSamples_v1 = 0x10017100u;
constexpr uint32_t GammaRamp_BuildRamps = 0x10017160u;
constexpr uint32_t FUN_100171b0 = 0x100171B0u;
constexpr uint32_t GammaRamp_ApplyIfChanged = 0x10017230u;
constexpr uint32_t CShader_Destructor = 0x100177A0u;
constexpr uint32_t fn_10017BB0 = 0x10017BB0u;
constexpr uint32_t sub_10017C20 = 0x10017C20u;
constexpr uint32_t FUN_10017c60 = 0x10017C60u;
constexpr uint32_t nullsub_13 = 0x10017D20u;
constexpr uint32_t nullsub_13_v1 = 0x10017D20u;
constexpr uint32_t nullsub_13_v2 = 0x10017D20u;
constexpr uint32_t nullsub_12 = 0x10017D30u;
constexpr uint32_t nullsub_12_v1 = 0x10017D30u;
constexpr uint32_t FUN_100181d0 = 0x100181D0u;
constexpr uint32_t FUN_100182c0 = 0x100182C0u;
constexpr uint32_t _exit_0 = 0x100182E0u;
constexpr uint32_t _exit_0_v1 = 0x100182E0u;
constexpr uint32_t thunk_FUN_10114f10 = 0x10018300u;
constexpr uint32_t FUN_10018310 = 0x10018310u;
constexpr uint32_t FUN_10018350 = 0x10018350u;
constexpr uint32_t sub_10018430 = 0x10018430u;
constexpr uint32_t FUN_10018550 = 0x10018550u;
constexpr uint32_t sub_100185A0 = 0x100185A0u;
constexpr uint32_t CXiOpening_Constructor = 0x100186C0u;
constexpr uint32_t FUN_10018840 = 0x10018840u;
constexpr uint32_t sub_10018850 = 0x10018850u;
constexpr uint32_t OpeningSubtitle_Constructor = 0x10018A50u;
constexpr uint32_t OpeningSubtitle_Reset = 0x10018B30u;
constexpr uint32_t OpeningSubtitle_Reset_v1 = 0x10018B30u;
constexpr uint32_t Vt10329FE0_Constructor = 0x10018C50u;
constexpr uint32_t Vt10329FE0_Reset = 0x10018CE0u;
constexpr uint32_t Vt10329FE0_Reset_v1 = 0x10018CE0u;
constexpr uint32_t App_BeginTimerPeriod = 0x10019140u;
constexpr uint32_t sub_100191B0 = 0x100191B0u;
constexpr uint32_t sub_100191D0 = 0x100191D0u;
constexpr uint32_t FUN_10019210 = 0x10019210u;
constexpr uint32_t FUN_10019490 = 0x10019490u;
constexpr uint32_t FUN_100194f0 = 0x100194F0u;
constexpr uint32_t FUN_10019a50 = 0x10019A50u;
constexpr uint32_t SampleAnimationTrack_ToBuffer = 0x10019B30u;
constexpr uint32_t SampleAnimationTrack_Live = 0x10019EE0u;
constexpr uint32_t CYyMotionQue_SetSlotMotion = 0x1001A660u;
constexpr uint32_t CYyMotionQue_SetSlotMotion_v1 = 0x1001A660u;
constexpr uint32_t FUN_1001a6e0 = 0x1001A6E0u;
constexpr uint32_t FUN_1001abf0 = 0x1001ABF0u;
constexpr uint32_t FUN_1001ac30 = 0x1001AC30u;
constexpr uint32_t FUN_1001ac40 = 0x1001AC40u;
constexpr uint32_t FUN_1001acb0 = 0x1001ACB0u;
constexpr uint32_t KzMotionQueList_AppendSync = 0x1001AD00u;
constexpr uint32_t FUN_1001b0f0 = 0x1001B0F0u;
constexpr uint32_t FUN_1001b140 = 0x1001B140u;
constexpr uint32_t FUN_1001b170 = 0x1001B170u;
constexpr uint32_t FUN_1001b1a0 = 0x1001B1A0u;
constexpr uint32_t FUN_1001b1f0 = 0x1001B1F0u;
constexpr uint32_t CYyMotionQue_SampleInto = 0x1001B230u;
constexpr uint32_t FUN_1001b2e0 = 0x1001B2E0u;
constexpr uint32_t CYyMotionQue_IsActive = 0x1001B340u;
constexpr uint32_t FUN_1001b520 = 0x1001B520u;
constexpr uint32_t FUN_1001b590 = 0x1001B590u;
constexpr uint32_t FUN_1001b5d0 = 0x1001B5D0u;
constexpr uint32_t FUN_1001b710 = 0x1001B710u;
constexpr uint32_t FUN_1001b7b0 = 0x1001B7B0u;
constexpr uint32_t Model_CreateTranslucentTargets = 0x1001D0A0u;
constexpr uint32_t FUN_1001d110 = 0x1001D110u;
constexpr uint32_t FUN_1001d170 = 0x1001D170u;
constexpr uint32_t FUN_1001d1f0 = 0x1001D1F0u;
constexpr uint32_t Model_BeginTranslucentPass = 0x1001D240u;
constexpr uint32_t Model_TranslucentUnbindTexture = 0x1001D350u;
constexpr uint32_t sub_1001D390 = 0x1001D390u;
constexpr uint32_t Model_CompositeTranslucentPass = 0x1001D3B0u;
constexpr uint32_t Gfx_DrawScreenQuad256 = 0x1001DD00u;
constexpr uint32_t Tex_HeightToNormalMap = 0x1001E0A0u;
constexpr uint32_t FUN_1001e2f0 = 0x1001E2F0u;
constexpr uint32_t CYyCamMng2_CreateGlobal = 0x1001E470u;
constexpr uint32_t FUN_1001e4d0 = 0x1001E4D0u;
constexpr uint32_t FUN_1001e4f0 = 0x1001E4F0u;
constexpr uint32_t FUN_1001e590 = 0x1001E590u;
constexpr uint32_t FUN_1001e600 = 0x1001E600u;
constexpr uint32_t FUN_1001e760 = 0x1001E760u;
constexpr uint32_t FUN_1001e780 = 0x1001E780u;
constexpr uint32_t YmCameraManager_Follow = 0x1001EE60u;
constexpr uint32_t FUN_10021110 = 0x10021110u;
constexpr uint32_t FUN_10021680 = 0x10021680u;
constexpr uint32_t FUN_10021c10 = 0x10021C10u;
constexpr uint32_t FUN_100220e0 = 0x100220E0u;
constexpr uint32_t Actor_GetPercentScaleByIndex = 0x10022120u;
constexpr uint32_t FUN_10022170 = 0x10022170u;
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
constexpr uint32_t FUN_10025010 = 0x10025010u;
constexpr uint32_t CYyModelDt_Destructor = 0x100250C0u;
constexpr uint32_t FUN_100250f0 = 0x100250F0u;
constexpr uint32_t __sub_10095B10 = 0x10025220u;
constexpr uint32_t FUN_10025260 = 0x10025260u;
constexpr uint32_t ProjTex_Destructor = 0x100252F0u;
constexpr uint32_t ProjTex_Destructor_v1 = 0x100252F0u;
constexpr uint32_t ProjTex_BeginPass = 0x10025330u;
constexpr uint32_t ProjTex_EndPass = 0x100255C0u;
constexpr uint32_t TexObj_GetD3DTexture = 0x10025860u;
constexpr uint32_t ProjTex_CaptureToTarget = 0x10025870u;
constexpr uint32_t nullsub_39 = 0x10025BD0u;
constexpr uint32_t FUN_10025be0 = 0x10025BE0u;
constexpr uint32_t FUN_10025bf0 = 0x10025BF0u;
constexpr uint32_t FUN_10025c30 = 0x10025C30u;
constexpr uint32_t EnvMap_BeginPass = 0x10025C40u;
constexpr uint32_t EnvMap_EndPass = 0x10025F70u;
constexpr uint32_t sub_10026210 = 0x10026210u;
constexpr uint32_t CYyIcon_Constructor = 0x10026890u;
constexpr uint32_t CXiMovie_Draw = 0x10026AD0u;
constexpr uint32_t CYyIcon_DrawSubRect = 0x10026C70u;
constexpr uint32_t sub_10026E00 = 0x10026E00u;
constexpr uint32_t Quat_SetIdentity = 0x10026E10u;
constexpr uint32_t Vec4_Set = 0x10026E30u;
constexpr uint32_t Vec4_Set_v1 = 0x10026E30u;
constexpr uint32_t FUN_10026e50 = 0x10026E50u;
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
constexpr uint32_t FUN_100270a0 = 0x100270A0u;
constexpr uint32_t fn_100270A0 = 0x100270A0u;
constexpr uint32_t Vec3_Sub = 0x10027120u;
constexpr uint32_t Vec3_Sub_v1 = 0x10027120u;
constexpr uint32_t FUN_100272b0 = 0x100272B0u;
constexpr uint32_t fn_100272B0 = 0x100272B0u;
constexpr uint32_t Vec3_Scale = 0x100272E0u;
constexpr uint32_t Vec3_Scale_v1 = 0x100272E0u;
constexpr uint32_t Vec3_MulInPlace = 0x10027310u;
constexpr uint32_t Vec3_MulInPlace_v1 = 0x10027310u;
constexpr uint32_t FUN_10027340 = 0x10027340u;
constexpr uint32_t sub_10027340 = 0x10027340u;
constexpr uint32_t FUN_100274b0 = 0x100274B0u;
constexpr uint32_t sub_100274B0 = 0x100274B0u;
constexpr uint32_t FUN_10027510 = 0x10027510u;
constexpr uint32_t FUN_10027530 = 0x10027530u;
constexpr uint32_t FUN_100275d0 = 0x100275D0u;
constexpr uint32_t FUN_10027600 = 0x10027600u;
constexpr uint32_t Vec3_Lerp = 0x100276A0u;
constexpr uint32_t Gfx_ProjectWorldToClip = 0x100276F0u;
constexpr uint32_t Gfx_NdcToScreen = 0x100278E0u;
constexpr uint32_t FUN_10027990 = 0x10027990u;
constexpr uint32_t fn_10027990 = 0x10027990u;
constexpr uint32_t nullsub_1 = 0x100279A0u;
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
constexpr uint32_t FUN_10028170 = 0x10028170u;
constexpr uint32_t Mat4_TransformPointInPlace = 0x10028200u;
constexpr uint32_t Mat4_TransformPointInPlace_v1 = 0x10028200u;
constexpr uint32_t FUN_10028230 = 0x10028230u;
constexpr uint32_t Mat4_TransformNormalInPlace = 0x10028290u;
constexpr uint32_t Mat4_TransformNormalInPlace_v1 = 0x10028290u;
constexpr uint32_t Mat4_TransformNormal = 0x100282C0u;
constexpr uint32_t Mat4_TransformNormal_v1 = 0x100282C0u;
constexpr uint32_t Mat4_TransformCoordInPlace = 0x10028320u;
constexpr uint32_t Mat4_InvertOrthonormal = 0x100283C0u;
constexpr uint32_t Mem_AllocObject = 0x10028640u;
constexpr uint32_t Mem_AllocObject_v1 = 0x10028640u;
constexpr uint32_t Mem_AllocWithFallback = 0x100286B0u;
constexpr uint32_t Mem_AllocWithFallback_v1 = 0x100286B0u;
constexpr uint32_t Mem_FreeObject = 0x10028700u;
constexpr uint32_t Mem_FreeObject_v1 = 0x10028700u;
constexpr uint32_t SharedDeleteHelper = 0x10028710u;
constexpr uint32_t SharedDeleteHelper_v1 = 0x10028710u;
constexpr uint32_t FUN_10028730 = 0x10028730u;
constexpr uint32_t FUN_10028730_v1 = 0x10028730u;
constexpr uint32_t FUN_100289f0 = 0x100289F0u;
constexpr uint32_t FUN_10028aa0 = 0x10028AA0u;
constexpr uint32_t MemBlock_GetSize = 0x10028B90u;
constexpr uint32_t MemBlock_GetSize_v1 = 0x10028B90u;
constexpr uint32_t FUN_10028bf0 = 0x10028BF0u;
constexpr uint32_t FUN_10028c30 = 0x10028C30u;
constexpr uint32_t FUN_10028c50 = 0x10028C50u;
constexpr uint32_t FUN_10028c70 = 0x10028C70u;
constexpr uint32_t FUN_10028c90 = 0x10028C90u;
constexpr uint32_t Mem_AllocFromArea = 0x10028FD0u;
constexpr uint32_t Mem_AllocFromArea_v1 = 0x10028FD0u;
constexpr uint32_t FUN_10029150 = 0x10029150u;
constexpr uint32_t FUN_10029390 = 0x10029390u;
constexpr uint32_t CreateSharedVertexBuffers = 0x10029A50u;
constexpr uint32_t FUN_10029b60 = 0x10029B60u;
constexpr uint32_t FUN_10029bb0 = 0x10029BB0u;
constexpr uint32_t FUN_10029c00 = 0x10029C00u;
constexpr uint32_t FUN_10029c90 = 0x10029C90u;
constexpr uint32_t CYyModelDtBase_Destructor = 0x10029CD0u;
constexpr uint32_t FUN_10029d30 = 0x10029D30u;
constexpr uint32_t CYyModelBase_Destructor = 0x10029DD0u;
constexpr uint32_t CYyModelBase_SetResourceAndRebuild = 0x10029E50u;
constexpr uint32_t CYyModelBase_GetPartList = 0x10029E70u;
constexpr uint32_t CYyModelBase_GetPartList_v1 = 0x10029E70u;
constexpr uint32_t FUN_10029eb0 = 0x10029EB0u;
constexpr uint32_t FUN_10029f00 = 0x10029F00u;
constexpr uint32_t CYyModelBase_AddPartFromResource = 0x10029F70u;
constexpr uint32_t CYyModelBase_AddPartFromResource_v1 = 0x10029F70u;
constexpr uint32_t FUN_1002a0c0 = 0x1002A0C0u;
constexpr uint32_t FUN_1002a130 = 0x1002A130u;
constexpr uint32_t Model_AnimateAndPose = 0x1002A140u;
constexpr uint32_t CYyModelBase_DrawLit = 0x1002A3B0u;
constexpr uint32_t FUN_1002a700 = 0x1002A700u;
constexpr uint32_t FUN_1002a750 = 0x1002A750u;
constexpr uint32_t FUN_1002a780 = 0x1002A780u;
constexpr uint32_t FUN_1002a890 = 0x1002A890u;
constexpr uint32_t FUN_1002a8b0 = 0x1002A8B0u;
constexpr uint32_t FUN_1002a9b0 = 0x1002A9B0u;
constexpr uint32_t FUN_1002a9e0 = 0x1002A9E0u;
constexpr uint32_t CYyModelBase_ForwardToField24 = 0x1002AC50u;
constexpr uint32_t FUN_1002b3f0 = 0x1002B3F0u;
constexpr uint32_t FUN_1002b470 = 0x1002B470u;
constexpr uint32_t CYyModel_Constructor = 0x1002B4B0u;
constexpr uint32_t CYyModel_Destructor = 0x1002B550u;
constexpr uint32_t CYyModel_GetField44Addr = 0x1002B5A0u;
constexpr uint32_t CYyModel_GetField44Addr_v1 = 0x1002B5A0u;
constexpr uint32_t FUN_1002b5e0 = 0x1002B5E0u;
constexpr uint32_t FUN_1002b630 = 0x1002B630u;
constexpr uint32_t CYyModel_CreateModelBase = 0x1002B6A0u;
constexpr uint32_t CXiSkeletonActor_DrawModel = 0x1002B6E0u;
constexpr uint32_t sub_1002C270 = 0x1002C270u;
constexpr uint32_t CXiSkeletonActor_AnimateModels = 0x1002C2D0u;
constexpr uint32_t CXiSkeletonActor_PrepareModelsOnce = 0x1002C360u;
constexpr uint32_t CXiSkeletonActor_GetElemThunk = 0x1002C3E0u;
constexpr uint32_t FUN_1002c400 = 0x1002C400u;
constexpr uint32_t CXiSkeletonActor_GetModelElem = 0x1002C5C0u;
constexpr uint32_t FUN_1002c710 = 0x1002C710u;
constexpr uint32_t FUN_1002c750 = 0x1002C750u;
constexpr uint32_t List_GetTail = 0x1002C7C0u;
constexpr uint32_t FUN_1002c7e0 = 0x1002C7E0u;
constexpr uint32_t FUN_1002c800 = 0x1002C800u;
constexpr uint32_t Mem_AllocDefault = 0x1002C890u;
constexpr uint32_t Mem_AllocDefault_v1 = 0x1002C890u;
constexpr uint32_t thunk_sub_10028AA0 = 0x1002C8B0u;
constexpr uint32_t thunk_sub_10028AA0_v1 = 0x1002C8B0u;
constexpr uint32_t thunk_sub_10028AA0_v2 = 0x1002C8B0u;
constexpr uint32_t CYyObject_IsKindOf = 0x1002C8F0u;
constexpr uint32_t CYyObject_IsKindOf_v1 = 0x1002C8F0u;
constexpr uint32_t CYyOs2VtxBuffer_Constructor = 0x1002D680u;
constexpr uint32_t CYyOs2VtxBuffer_Destructor = 0x1002D700u;
constexpr uint32_t CYyModelDt_SkinAndDrawV2 = 0x1002DC10u;
constexpr uint32_t Gfx_SetStreamSourceCached = 0x1002F130u;
constexpr uint32_t Gfx_ApplyCachedStreamSource = 0x1002F160u;
constexpr uint32_t Gfx_ApplyCachedVertexShader = 0x1002F1A0u;
constexpr uint32_t Gfx_SetVertexShaderCached = 0x1002F1C0u;
constexpr uint32_t Model_SkinVerticesCpu = 0x1002F1D0u;
constexpr uint32_t FUN_100309b0 = 0x100309B0u;
constexpr uint32_t FUN_10030a40 = 0x10030A40u;
constexpr uint32_t FUN_10030fb0 = 0x10030FB0u;
constexpr uint32_t FUN_10032a40 = 0x10032A40u;
constexpr uint32_t FUN_10032a50 = 0x10032A50u;
constexpr uint32_t FUN_10032a70 = 0x10032A70u;
constexpr uint32_t FUN_10032b50 = 0x10032B50u;
constexpr uint32_t FUN_10032da0 = 0x10032DA0u;
constexpr uint32_t BoneXform_PostMultiplyInto = 0x10032FA0u;
constexpr uint32_t FUN_10032fe0 = 0x10032FE0u;
constexpr uint32_t Quat_NLerp = 0x10033220u;
constexpr uint32_t FUN_10033360 = 0x10033360u;
constexpr uint32_t sub_100336B0 = 0x100336B0u;
constexpr uint32_t FUN_100336d0 = 0x100336D0u;
constexpr uint32_t FUN_10033710 = 0x10033710u;
constexpr uint32_t FUN_100337c0 = 0x100337C0u;
constexpr uint32_t Model_BeginOffscreenPass = 0x10033800u;
constexpr uint32_t Model_EndOffscreenPassBindTextures = 0x10033A70u;
constexpr uint32_t sub_10033DA0 = 0x10033DA0u;
constexpr uint32_t Shadow_SmoothLightDirection = 0x10033EA0u;
constexpr uint32_t FUN_10034230 = 0x10034230u;
constexpr uint32_t FUN_10034270 = 0x10034270u;
constexpr uint32_t FUN_100342a0 = 0x100342A0u;
constexpr uint32_t FUN_100351f0 = 0x100351F0u;
constexpr uint32_t FUN_10035210 = 0x10035210u;
constexpr uint32_t FUN_10035250 = 0x10035250u;
constexpr uint32_t sub_10035250 = 0x10035250u;
constexpr uint32_t FUN_100352a0 = 0x100352A0u;
constexpr uint32_t FUN_100352f0 = 0x100352F0u;
constexpr uint32_t FUN_10035330 = 0x10035330u;
constexpr uint32_t FUN_10035a00 = 0x10035A00u;
constexpr uint32_t CMoTask_VT_1032A904_Destructor = 0x10035B00u;
constexpr uint32_t MoTask_KillAllGlobalSlots = 0x10035F00u;
constexpr uint32_t FUN_10035f80 = 0x10035F80u;
constexpr uint32_t FUN_10035fb0 = 0x10035FB0u;
constexpr uint32_t FUN_10036020 = 0x10036020u;
constexpr uint32_t FUN_10036050 = 0x10036050u;
constexpr uint32_t MoTask_KillGlobalSlot1 = 0x10036070u;
constexpr uint32_t FUN_100360a0 = 0x100360A0u;
constexpr uint32_t FUN_100360c0 = 0x100360C0u;
constexpr uint32_t FUN_100360f0 = 0x100360F0u;
constexpr uint32_t FUN_10036170 = 0x10036170u;
constexpr uint32_t sub_100361D0 = 0x100361D0u;
constexpr uint32_t sub_10036230 = 0x10036230u;
constexpr uint32_t FUN_10036360 = 0x10036360u;
constexpr uint32_t FUN_100363b0 = 0x100363B0u;
constexpr uint32_t FUN_10036420 = 0x10036420u;
constexpr uint32_t FUN_100364a0 = 0x100364A0u;
constexpr uint32_t FUN_100364e0 = 0x100364E0u;
constexpr uint32_t FUN_100367c0 = 0x100367C0u;
constexpr uint32_t FUN_100367f0 = 0x100367F0u;
constexpr uint32_t FUN_10036830 = 0x10036830u;
constexpr uint32_t FUN_10036870 = 0x10036870u;
constexpr uint32_t FUN_10036980 = 0x10036980u;
constexpr uint32_t FUN_100369f0 = 0x100369F0u;
constexpr uint32_t FUN_10036b90 = 0x10036B90u;
constexpr uint32_t FUN_10036c70 = 0x10036C70u;
constexpr uint32_t FUN_10036f00 = 0x10036F00u;
constexpr uint32_t FUN_10036f50 = 0x10036F50u;
constexpr uint32_t FUN_10037020 = 0x10037020u;
constexpr uint32_t FUN_100378b0 = 0x100378B0u;
constexpr uint32_t FUN_10037960 = 0x10037960u;
constexpr uint32_t FUN_10037a10 = 0x10037A10u;
constexpr uint32_t SetMusicTrackVolume_A = 0x10037B90u;
constexpr uint32_t SetMusicTrackVolume_B = 0x10037BF0u;
constexpr uint32_t YmSePlayBeep = 0x10037C50u;
constexpr uint32_t FUN_10037d70 = 0x10037D70u;
constexpr uint32_t FUN_10037dd0 = 0x10037DD0u;
constexpr uint32_t FUN_10037e30 = 0x10037E30u;
constexpr uint32_t FUN_10037e90 = 0x10037E90u;
constexpr uint32_t FUN_10037ef0 = 0x10037EF0u;
constexpr uint32_t FUN_10037fb0 = 0x10037FB0u;
constexpr uint32_t FUN_100382a0 = 0x100382A0u;
constexpr uint32_t FUN_10038440 = 0x10038440u;
constexpr uint32_t FUN_10038470 = 0x10038470u;
constexpr uint32_t Font_QueueText = 0x100384B0u;
constexpr uint32_t Font_DrawPass = 0x10038520u;
constexpr uint32_t Font_DrawString = 0x10038720u;
constexpr uint32_t Post_CreateScreenEffectTargets = 0x10038930u;
constexpr uint32_t FUN_100389c0 = 0x100389C0u;
constexpr uint32_t FUN_10038a20 = 0x10038A20u;
constexpr uint32_t FUN_10038a60 = 0x10038A60u;
constexpr uint32_t sub_10038A70 = 0x10038A70u;
constexpr uint32_t Post_CaptureSceneToTarget = 0x10038A80u;
constexpr uint32_t Post_DrawRotatingGhostLayers = 0x10038E10u;
constexpr uint32_t sub_10039410 = 0x10039410u;
constexpr uint32_t FUN_10039440 = 0x10039440u;
constexpr uint32_t ThreadPool_StartThread = 0x100394D0u;
constexpr uint32_t FUN_10039550 = 0x10039550u;
constexpr uint32_t FUN_10039590 = 0x10039590u;
constexpr uint32_t FUN_100395d0 = 0x100395D0u;
constexpr uint32_t FUN_10039670 = 0x10039670u;
constexpr uint32_t FUN_100396a0 = 0x100396A0u;
constexpr uint32_t FUN_100397a0 = 0x100397A0u;
constexpr uint32_t Util_Copy16Bytes = 0x10039810u;
constexpr uint32_t Util_Copy16Bytes_v1 = 0x10039810u;
constexpr uint32_t FUN_10039830 = 0x10039830u;
constexpr uint32_t CYyTexBase_Destructor = 0x100398D0u;
constexpr uint32_t CYyTexBase_SetName = 0x100398E0u;
constexpr uint32_t CYyTexBase_SetName_v1 = 0x100398E0u;
constexpr uint32_t CYyTex_Destructor = 0x10039970u;
constexpr uint32_t FUN_100399d0 = 0x100399D0u;
constexpr uint32_t TexObj_CreateAndUpload = 0x10039A30u;
constexpr uint32_t TexObj_CreateAndUpload_v1 = 0x10039A30u;
constexpr uint32_t TexObj_Reupload = 0x10039A60u;
constexpr uint32_t TexObj_Create = 0x10039A80u;
constexpr uint32_t TexObj_Upload = 0x10039E50u;
constexpr uint32_t TexObj_Upload_v1 = 0x10039E50u;
constexpr uint32_t sub_1003AA50 = 0x1003AA50u;
constexpr uint32_t CYyTexMng_Destructor = 0x1003AA90u;
constexpr uint32_t FUN_1003aab0 = 0x1003AAB0u;
constexpr uint32_t FUN_1003ab10 = 0x1003AB10u;
constexpr uint32_t FUN_1003ab40 = 0x1003AB40u;
constexpr uint32_t TexMgr_UploadPumpEntry = 0x1003AB70u;
constexpr uint32_t TexMgr_UploadPump = 0x1003ABB0u;
constexpr uint32_t CYyTexMng_FindByName = 0x1003AC80u;
constexpr uint32_t CYyTexMng_FindByName_v1 = 0x1003AC80u;
constexpr uint32_t sub_1003ACD0 = 0x1003ACD0u;
constexpr uint32_t sub_1003AD40 = 0x1003AD40u;
constexpr uint32_t sub_1003ADC0 = 0x1003ADC0u;
constexpr uint32_t TexMgr_FindByName = 0x1003AEA0u;
constexpr uint32_t TexMgr_FindByName_v1 = 0x1003AEA0u;
constexpr uint32_t CYyTexMng_FindByNameInResource = 0x1003AF10u;
constexpr uint32_t CYyTexMng_FindByNameInResource_v1 = 0x1003AF10u;
constexpr uint32_t sub_1003B080 = 0x1003B080u;
constexpr uint32_t CYyVbMng_AcquireVertexBuffer = 0x1003B210u;
constexpr uint32_t CYyVbMng_ReleaseVertexBuffer = 0x1003B3C0u;
constexpr uint32_t CYyVb_Constructor = 0x1003B450u;
constexpr uint32_t FUN_1003b4a0 = 0x1003B4A0u;
constexpr uint32_t FUN_1003b4d0 = 0x1003B4D0u;
constexpr uint32_t FUN_1003b4e0 = 0x1003B4E0u;
constexpr uint32_t FUN_1003b540 = 0x1003B540u;
constexpr uint32_t ReleaseSubObjectIfPresent = 0x1003B5C0u;
constexpr uint32_t ReleaseSubObjectIfPresent_v1 = 0x1003B5C0u;
constexpr uint32_t CMoAttachments_Reset = 0x1003B640u;
constexpr uint32_t CMoAttachments_Reset_v1 = 0x1003B640u;
constexpr uint32_t FUN_1003b690 = 0x1003B690u;
constexpr uint32_t FUN_1003b690_v1 = 0x1003B690u;
constexpr uint32_t FUN_1003b6d0 = 0x1003B6D0u;
constexpr uint32_t sub_1003B6D0 = 0x1003B6D0u;
constexpr uint32_t FUN_1003b700 = 0x1003B700u;
constexpr uint32_t sub_1003B700 = 0x1003B700u;
constexpr uint32_t CMoAttachments_ClearCaster = 0x1003B750u;
constexpr uint32_t CMoAttachments_ClearCaster_v1 = 0x1003B750u;
constexpr uint32_t CMoAttachments_ClearTarget = 0x1003B780u;
constexpr uint32_t CMoAttachments_ClearTarget_v1 = 0x1003B780u;
constexpr uint32_t CMoAttachments_SetCaster = 0x1003B7B0u;
constexpr uint32_t CMoAttachments_SetCaster_v1 = 0x1003B7B0u;
constexpr uint32_t CMoAttachments_SetTarget = 0x1003B830u;
constexpr uint32_t CMoAttachments_SetTarget_v1 = 0x1003B830u;
constexpr uint32_t FUN_1003b8b0 = 0x1003B8B0u;
constexpr uint32_t FUN_1003b900 = 0x1003B900u;
constexpr uint32_t sub_1003B980 = 0x1003B980u;
constexpr uint32_t sub_1003BA20 = 0x1003BA20u;
constexpr uint32_t sub_1003BD90 = 0x1003BD90u;
constexpr uint32_t YmGenerater_BuildAttachMatrix = 0x1003BE10u;
constexpr uint32_t FUN_1003c6e0 = 0x1003C6E0u;
constexpr uint32_t FUN_1003c700 = 0x1003C700u;
constexpr uint32_t FUN_1003c740 = 0x1003C740u;
constexpr uint32_t CMoCamera_GetRuntimeClass = 0x1003C880u;
constexpr uint32_t CMoCameraTask_Destructor = 0x1003D030u;
constexpr uint32_t Sky_DrawGradientDome = 0x1003D640u;
constexpr uint32_t CMoCelestial_Destructor = 0x1003D980u;
constexpr uint32_t Sky_DestroyDomeShaderStatic = 0x1003DB50u;
constexpr uint32_t Sky_DrawDomeViewCentered = 0x1003DB60u;
constexpr uint32_t Sky_DestroyViewCenteredShaderStatic = 0x1003DDF0u;
constexpr uint32_t Color_SetFromU32SwapRB = 0x1003E280u;
constexpr uint32_t Color_CopySwapRB = 0x1003E2A0u;
constexpr uint32_t FUN_1003e330 = 0x1003E330u;
constexpr uint32_t sub_1003E910 = 0x1003E910u;
constexpr uint32_t YmElem_SetD3ADrawState = 0x1003E9A0u;
constexpr uint32_t unknown_libname_20 = 0x1003EB00u;
constexpr uint32_t unknown_libname_21 = 0x1003EB10u;
constexpr uint32_t unknown_libname_22 = 0x1003EB20u;
constexpr uint32_t CMoVuAnmElem_Destructor = 0x1003EF20u;
constexpr uint32_t CMoDistMorphElem_ApplyMorph = 0x1003F440u;
constexpr uint32_t unknown_libname_25 = 0x1003F890u;
constexpr uint32_t CMoDistMorphElem_ComputeMorphHandle = 0x1003F8A0u;
constexpr uint32_t CXi471011C8_VT_1032AC90_Dtor = 0x1003FB10u;
constexpr uint32_t CXi471011C8_VT_1032AC90_Ctor = 0x1003FB70u;
constexpr uint32_t sub_1003FCB0 = 0x1003FCB0u;
constexpr uint32_t FUN_1003fce0 = 0x1003FCE0u;
constexpr uint32_t sub_1003FCE0 = 0x1003FCE0u;
constexpr uint32_t FUN_1003fcf0 = 0x1003FCF0u;
constexpr uint32_t sub_1003FCF0 = 0x1003FCF0u;
constexpr uint32_t FUN_10040220 = 0x10040220u;
constexpr uint32_t unknown_libname_33 = 0x10040B50u;
constexpr uint32_t unknown_libname_34 = 0x10040B60u;
constexpr uint32_t unknown_libname_35 = 0x10040B70u;
constexpr uint32_t unknown_libname_36 = 0x10040B80u;
constexpr uint32_t unknown_libname_37 = 0x10040B90u;
constexpr uint32_t CMoElemFamily_SharedEarlyExit = 0x10040C20u;
constexpr uint32_t unknown_libname_38 = 0x10040E10u;
constexpr uint32_t CXi471011C8_VT_1032AD80_Dtor = 0x10041110u;
constexpr uint32_t Font_DrawProjectedText3D = 0x10041C20u;
constexpr uint32_t CMoOtTask_ScalarDeletingDtor = 0x100425F0u;
constexpr uint32_t unknown_libname_46 = 0x10042CD0u;
constexpr uint32_t unknown_libname_47 = 0x10042CE0u;
constexpr uint32_t nullsub_136 = 0x10042CF0u;
constexpr uint32_t CMoDistMorphElem_ValidateMorphData = 0x10042D00u;
constexpr uint32_t unknown_libname_48 = 0x100430A0u;
constexpr uint32_t unknown_libname_49 = 0x10043310u;
constexpr uint32_t StaticDtor_StateBlock_1047BB7C = 0x100439B0u;
constexpr uint32_t CMoTask_VT_1032AF60_ScalarDeletingDtor = 0x10043A90u;
constexpr uint32_t CMoTask_VT_1032AF60_Destructor = 0x10043AB0u;
constexpr uint32_t CMoTask_VT_1032AFA8_ScalarDeletingDtor = 0x10043C90u;
constexpr uint32_t CMoTask_VT_1032AFA8_Destructor = 0x10043CB0u;
constexpr uint32_t FUN_10043de0 = 0x10043DE0u;
constexpr uint32_t sub_10043DE0 = 0x10043DE0u;
constexpr uint32_t DynVB_Constructor = 0x10043DF0u;
constexpr uint32_t DynVB_Constructor_v1 = 0x10043DF0u;
constexpr uint32_t DynVB_Destructor = 0x10043E40u;
constexpr uint32_t DynVB_Create = 0x10043E50u;
constexpr uint32_t sub_10043F00 = 0x10043F00u;
constexpr uint32_t VertexBatch_BindAndDraw = 0x10043F40u;
constexpr uint32_t VertexBatch_BindAndDraw_v1 = 0x10043F40u;
constexpr uint32_t DynVB_Reserve = 0x10044010u;
constexpr uint32_t DynVB_Lock = 0x10044130u;
constexpr uint32_t DynVB_Unlock = 0x100441A0u;
constexpr uint32_t CameraCache_CacheViewProjection = 0x100441C0u;
constexpr uint32_t Gfx_SetViewMatrixFromField10 = 0x10044210u;
constexpr uint32_t Gfx_SetViewMatrixFromField90 = 0x10044230u;
constexpr uint32_t FUN_10044250 = 0x10044250u;
constexpr uint32_t Mo_InitSharedRenderResources = 0x10044270u;
constexpr uint32_t FUN_100446a0 = 0x100446A0u;
constexpr uint32_t CMoElem_Constructor = 0x10044890u;
constexpr uint32_t CMoElemFamily_SharedCleanup = 0x10044970u;
constexpr uint32_t CMoElemFamily_SharedCleanup_v1 = 0x10044970u;
constexpr uint32_t FUN_10044aa0 = 0x10044AA0u;
constexpr uint32_t FUN_10044ae0 = 0x10044AE0u;
constexpr uint32_t FUN_10044c40 = 0x10044C40u;
constexpr uint32_t sub_10044C40 = 0x10044C40u;
constexpr uint32_t FUN_10044cf0 = 0x10044CF0u;
constexpr uint32_t FUN_10044d40 = 0x10044D40u;
constexpr uint32_t YmElem_ComputeViewDepthKey = 0x10044F10u;
constexpr uint32_t FUN_10045190 = 0x10045190u;
constexpr uint32_t sub_10045800 = 0x10045800u;
constexpr uint32_t FUN_100458f0 = 0x100458F0u;
constexpr uint32_t YmElem_BuildDrawMatrixAndTest = 0x10045920u;
constexpr uint32_t nullsub_140 = 0x10045A30u;
constexpr uint32_t FUN_10045c50 = 0x10045C50u;
constexpr uint32_t sub_10045C50 = 0x10045C50u;
constexpr uint32_t RegisterWithProcessor = 0x10045D40u;
constexpr uint32_t RegisterWithProcessor_v1 = 0x10045D40u;
constexpr uint32_t FUN_10046380 = 0x10046380u;
constexpr uint32_t sub_100464D0 = 0x100464D0u;
constexpr uint32_t sub_10046550 = 0x10046550u;
constexpr uint32_t YmElem_SetupDevice = 0x100467E0u;
constexpr uint32_t unknown_libname_53 = 0x100472D0u;
constexpr uint32_t YmElem_RestoreDevice = 0x100472F0u;
constexpr uint32_t unknown_libname_54 = 0x100473E0u;
constexpr uint32_t FUN_100473f0 = 0x100473F0u;
constexpr uint32_t sub_100473F0 = 0x100473F0u;
constexpr uint32_t FUN_10047450 = 0x10047450u;
constexpr uint32_t FUN_10047490 = 0x10047490u;
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
constexpr uint32_t FUN_100493b0 = 0x100493B0u;
constexpr uint32_t CMoGeneratorClone_ResolveTarget = 0x10049570u;
constexpr uint32_t CMoGeneratorClone_ResolveTarget_v1 = 0x10049570u;
constexpr uint32_t FUN_100496f0 = 0x100496F0u;
constexpr uint32_t sub_100496F0 = 0x100496F0u;
constexpr uint32_t FUN_1004a980 = 0x1004A980u;
constexpr uint32_t FUN_1004e720 = 0x1004E720u;
constexpr uint32_t SharedConditionalStateReset = 0x100531D0u;
constexpr uint32_t sub_10053370 = 0x10053370u;
constexpr uint32_t CMoGeneratorClone_CopyParams = 0x10053450u;
constexpr uint32_t CMoGeneratorClone_CreateInstance = 0x100536C0u;
constexpr uint32_t CMoGeneratorClone_VT_1032B69C_ScalarDeletingDtor = 0x10053840u;
constexpr uint32_t FUN_10053890 = 0x10053890u;
constexpr uint32_t FUN_10053890_v1 = 0x10053890u;
constexpr uint32_t sub_10053890 = 0x10053890u;
constexpr uint32_t YmGenerater_Stop = 0x100538E0u;
constexpr uint32_t YmGenerater_Stop_v1 = 0x100538E0u;
constexpr uint32_t YmGenerater_Stop_v2 = 0x100538E0u;
constexpr uint32_t FUN_100539e0 = 0x100539E0u;
constexpr uint32_t sub_100539E0 = 0x100539E0u;
constexpr uint32_t FUN_10053a00 = 0x10053A00u;
constexpr uint32_t FUN_10053c80 = 0x10053C80u;
constexpr uint32_t FUN_10053dd0 = 0x10053DD0u;
constexpr uint32_t FUN_10053e60 = 0x10053E60u;
constexpr uint32_t CMoTask_VT_1032B654_Destructor = 0x100541C0u;
constexpr uint32_t CMoGeneratorClone_GetRuntimeClass = 0x100542E0u;
constexpr uint32_t CMoGeneratorClone_Destructor = 0x100542F0u;
constexpr uint32_t CMoGeneratorClone_Kill = 0x100543A0u;
constexpr uint32_t FUN_100544d0 = 0x100544D0u;
constexpr uint32_t FUN_10054500 = 0x10054500u;
constexpr uint32_t FUN_100546f0 = 0x100546F0u;
constexpr uint32_t FUN_100547a0 = 0x100547A0u;
constexpr uint32_t CMoLfdElem_Destructor = 0x100548C0u;
constexpr uint32_t BeginEffectOffscreenPass = 0x10054D70u;
constexpr uint32_t nullsub_130 = 0x10054F70u;
constexpr uint32_t EndEffectOffscreenPass = 0x10054F80u;
constexpr uint32_t FUN_10054fc0 = 0x10054FC0u;
constexpr uint32_t Vt1032B6D8_Destructor = 0x10055000u;
constexpr uint32_t FUN_100550f0 = 0x100550F0u;
constexpr uint32_t FUN_10055130 = 0x10055130u;
constexpr uint32_t FUN_10055160 = 0x10055160u;
constexpr uint32_t LinkNode_Init = 0x10055240u;
constexpr uint32_t LinkNode_Init_v1 = 0x10055240u;
constexpr uint32_t LinkNode_Unlink = 0x10055250u;
constexpr uint32_t FUN_100552a0 = 0x100552A0u;
constexpr uint32_t CMoOT_Destructor = 0x100552E0u;
constexpr uint32_t FUN_100552f0 = 0x100552F0u;
constexpr uint32_t FUN_10055300 = 0x10055300u;
constexpr uint32_t FUN_100554b0 = 0x100554B0u;
constexpr uint32_t FUN_10055520 = 0x10055520u;
constexpr uint32_t FUN_10055570 = 0x10055570u;
constexpr uint32_t FUN_100555b0 = 0x100555B0u;
constexpr uint32_t sub_100555B0 = 0x100555B0u;
constexpr uint32_t sub_100555D0 = 0x100555D0u;
constexpr uint32_t FUN_10055620 = 0x10055620u;
constexpr uint32_t FUN_10055640 = 0x10055640u;
constexpr uint32_t FUN_10055650 = 0x10055650u;
constexpr uint32_t FUN_10055ad0 = 0x10055AD0u;
constexpr uint32_t FUN_10056160 = 0x10056160u;
constexpr uint32_t CMoSphRes_FixupOffsets = 0x10056220u;
constexpr uint32_t CMoPointLightProgElem_Destructor = 0x10056390u;
constexpr uint32_t unknown_libname_55 = 0x10056840u;
constexpr uint32_t CMoPathDriveActorTask_GetRuntimeClass = 0x10056940u;
constexpr uint32_t CMoLockColorDriveTask_GetRuntimeClass = 0x10056960u;
constexpr uint32_t CMoLockLookAtDriveTask_GetRuntimeClass = 0x10056980u;
constexpr uint32_t CMoActorColorDriveTask_GetRuntimeClass = 0x100569A0u;
constexpr uint32_t CMoActorRotationDriveTask_GetRuntimeClass = 0x100569C0u;
constexpr uint32_t Scheduler_Execute = 0x10056C70u;
constexpr uint32_t Scheduler_Execute_v1 = 0x10056C70u;
constexpr uint32_t FUN_10056eb0 = 0x10056EB0u;
constexpr uint32_t Scheduler_Kill = 0x10056EF0u;
constexpr uint32_t FUN_10057270 = 0x10057270u;
constexpr uint32_t FUN_100573b0 = 0x100573B0u;
constexpr uint32_t FUN_100573e0 = 0x100573E0u;
constexpr uint32_t CMoSchedularTask_Destructor = 0x100574A0u;
constexpr uint32_t FUN_10057c20 = 0x10057C20u;
constexpr uint32_t FUN_10057cc0 = 0x10057CC0u;
constexpr uint32_t CMoSchedularTask_Interpret = 0x10057FB0u;
constexpr uint32_t FUN_1005e100 = 0x1005E100u;
constexpr uint32_t FUN_1005ed30 = 0x1005ED30u;
constexpr uint32_t FUN_1005ed90 = 0x1005ED90u;
constexpr uint32_t FUN_1005edc0 = 0x1005EDC0u;
constexpr uint32_t FUN_1005ee90 = 0x1005EE90u;
constexpr uint32_t FUN_1005eec0 = 0x1005EEC0u;
constexpr uint32_t FUN_1005eef0 = 0x1005EEF0u;
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
constexpr uint32_t FUN_10062830 = 0x10062830u;
constexpr uint32_t FUN_10062880 = 0x10062880u;
constexpr uint32_t FUN_10062de0 = 0x10062DE0u;
constexpr uint32_t FUN_10062e20 = 0x10062E20u;
constexpr uint32_t FUN_10062e80 = 0x10062E80u;
constexpr uint32_t FUN_10062ed0 = 0x10062ED0u;
constexpr uint32_t FUN_10063460 = 0x10063460u;
constexpr uint32_t CMoSpline_Destructor = 0x100634A0u;
constexpr uint32_t CMoSpline_Destructor_v1 = 0x100634A0u;
constexpr uint32_t FUN_100634b0 = 0x100634B0u;
constexpr uint32_t FUN_10063550 = 0x10063550u;
constexpr uint32_t FUN_100635b0 = 0x100635B0u;
constexpr uint32_t FUN_10063890 = 0x10063890u;
constexpr uint32_t FUN_10063a60 = 0x10063A60u;
constexpr uint32_t FUN_10063b20 = 0x10063B20u;
constexpr uint32_t sub_10063B20 = 0x10063B20u;
constexpr uint32_t FUN_10063b50 = 0x10063B50u;
constexpr uint32_t sub_10063B50 = 0x10063B50u;
constexpr uint32_t FUN_10063b70 = 0x10063B70u;
constexpr uint32_t FUN_10063b80 = 0x10063B80u;
constexpr uint32_t FUN_10063c30 = 0x10063C30u;
constexpr uint32_t MoProgElem_FreeBuffer40 = 0x10063C60u;
constexpr uint32_t FUN_100683d0 = 0x100683D0u;
constexpr uint32_t FUN_100684f0 = 0x100684F0u;
constexpr uint32_t FUN_10068590 = 0x10068590u;
constexpr uint32_t CMoDX_Destructor = 0x10069D70u;
constexpr uint32_t CMoDX_Initialize = 0x10069DD0u;
constexpr uint32_t sub_10069EE0 = 0x10069EE0u;
constexpr uint32_t sub_1006A060 = 0x1006A060u;
constexpr uint32_t sub_1006A070 = 0x1006A070u;
constexpr uint32_t FUN_1006a0e0 = 0x1006A0E0u;
constexpr uint32_t sub_1006A0E0 = 0x1006A0E0u;
constexpr uint32_t CMoDX_CreateTexture = 0x1006A150u;
constexpr uint32_t sub_1006A180 = 0x1006A180u;
constexpr uint32_t sub_1006A420 = 0x1006A420u;
constexpr uint32_t FUN_1006a450 = 0x1006A450u;
constexpr uint32_t CMoDX_SetWorldMatrix = 0x1006A480u;
constexpr uint32_t CMoDX_SetWorldMatrix_v1 = 0x1006A480u;
constexpr uint32_t CMoDX_GetWorldMatrix = 0x1006A4A0u;
constexpr uint32_t CMoDX_PushViewMatrix = 0x1006A4C0u;
constexpr uint32_t CMoDX_PopViewMatrix = 0x1006A4F0u;
constexpr uint32_t CMoDX_PopViewMatrix_v1 = 0x1006A4F0u;
constexpr uint32_t CMoDX_SetViewMatrix = 0x1006A520u;
constexpr uint32_t CMoDX_SetViewMatrix_v1 = 0x1006A520u;
constexpr uint32_t FUN_1006a540 = 0x1006A540u;
constexpr uint32_t Gfx_CalcFVFStride = 0x1006A560u;
constexpr uint32_t sub_1006AD30 = 0x1006AD30u;
constexpr uint32_t FUN_1006ad50 = 0x1006AD50u;
constexpr uint32_t FUN_1006ad90 = 0x1006AD90u;
constexpr uint32_t fn_1006AD90 = 0x1006AD90u;
constexpr uint32_t sub_1006ADF0 = 0x1006ADF0u;
constexpr uint32_t StateBlock_ApplyRSTSS = 0x1006AE30u;
constexpr uint32_t StateBlockHolder_ApplyRenderStates = 0x1006AF00u;
constexpr uint32_t StateBlock_ApplyTSS = 0x1006AF90u;
constexpr uint32_t StateBlock_ApplyTSS_v1 = 0x1006AF90u;
constexpr uint32_t CMoDxStatusMng_Constructor = 0x1006B050u;
constexpr uint32_t CMoDxStatusMng_Destructor = 0x1006B090u;
constexpr uint32_t VBPool_Constructor = 0x1006B0C0u;
constexpr uint32_t FUN_1006b120 = 0x1006B120u;
constexpr uint32_t sub_1006B1D0 = 0x1006B1D0u;
constexpr uint32_t CMoDX_CreateAndInitSystem = 0x1006B2B0u;
constexpr uint32_t sub_1006BC10 = 0x1006BC10u;
constexpr uint32_t sub_1006BC80 = 0x1006BC80u;
constexpr uint32_t Rand_SeedMT19937 = 0x1006BD90u;
constexpr uint32_t Rand_SeedMT19937_v1 = 0x1006BD90u;
constexpr uint32_t FUN_1006be20 = 0x1006BE20u;
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
constexpr uint32_t FUN_1006ca20 = 0x1006CA20u;
constexpr uint32_t FUN_1006ca60 = 0x1006CA60u;
constexpr uint32_t CreateProcessorInstance = 0x1006CB50u;
constexpr uint32_t FUN_1006ce40 = 0x1006CE40u;
constexpr uint32_t nullsub_44 = 0x1006CF90u;
constexpr uint32_t CMoProcessor_VT_1032C368_vf012 = 0x1006D3B0u;
constexpr uint32_t FUN_1006d3e0 = 0x1006D3E0u;
constexpr uint32_t CMoProcessor_Vec3Normalize = 0x1006D5D0u;
constexpr uint32_t CMoProcessor_Vec4TransformB = 0x1006D7E0u;
constexpr uint32_t CMoProcessor_Vec4Transform = 0x1006D870u;
constexpr uint32_t CMoProcessor_Vec3Transform = 0x1006D900u;
constexpr uint32_t CMoProcessor_VT_1032C368_vf021 = 0x1006D980u;
constexpr uint32_t CMoProcessor_ProjectPointsToScreen = 0x1006E7D0u;
constexpr uint32_t CMoProcessor_CalcScreenBounds = 0x1006E970u;
constexpr uint32_t CMoProcessor_ProjectVertsAndBounds = 0x1006EB70u;
constexpr uint32_t CMoProcessor_CalcScreenBoundsAndVisible = 0x1006ED10u;
constexpr uint32_t FUN_10070f40 = 0x10070F40u;
constexpr uint32_t CMoTask_VT_1032C490_Destructor = 0x10070F80u;
constexpr uint32_t FUN_10070f90 = 0x10070F90u;
constexpr uint32_t ResHandle_FreeIfUnreferenced = 0x10071050u;
constexpr uint32_t ResHandle_FreeIfUnreferenced_v1 = 0x10071050u;
constexpr uint32_t ResourceHandle_IsResident = 0x10071060u;
constexpr uint32_t ResourceHandle_IsResident_v1 = 0x10071060u;
constexpr uint32_t FUN_100710f0 = 0x100710F0u;
constexpr uint32_t CMoAttachments_VT_1032C4E8_ScalarDeletingDtor = 0x10071180u;
constexpr uint32_t FUN_100711d0 = 0x100711D0u;
constexpr uint32_t FUN_100711e0 = 0x100711E0u;
constexpr uint32_t FUN_10071200 = 0x10071200u;
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
constexpr uint32_t sub_10071F50 = 0x10071F50u;
constexpr uint32_t FUN_10071f60 = 0x10071F60u;
constexpr uint32_t FUN_10071f70 = 0x10071F70u;
constexpr uint32_t FUN_10071f80 = 0x10071F80u;
constexpr uint32_t CMoResource_RegisterInTypeList = 0x10072030u;
constexpr uint32_t CMoResource_RegisterInTypeList_v1 = 0x10072030u;
constexpr uint32_t FUN_100720b0 = 0x100720B0u;
constexpr uint32_t FUN_10072180 = 0x10072180u;
constexpr uint32_t CMoResource_Destroy = 0x100721F0u;
constexpr uint32_t CMoResource_Destroy_v1 = 0x100721F0u;
constexpr uint32_t FUN_10072240 = 0x10072240u;
constexpr uint32_t CMoResource_GetParent = 0x10072270u;
constexpr uint32_t CMoResource_GetParent_v1 = 0x10072270u;
constexpr uint32_t CMoResource_FindByTypeAndId = 0x10072290u;
constexpr uint32_t CMoResource_FindByTypeAndId_v1 = 0x10072290u;
constexpr uint32_t FUN_100722c0 = 0x100722C0u;
constexpr uint32_t CMoResource_FindInScope = 0x10072430u;
constexpr uint32_t CMoResource_FindInScope_v1 = 0x10072430u;
constexpr uint32_t YmResourceFile_FindResourceUnder = 0x100724C0u;
constexpr uint32_t YmResourceFile_FindResourceUnder_v1 = 0x100724C0u;
constexpr uint32_t FUN_10072540 = 0x10072540u;
constexpr uint32_t FUN_10072970 = 0x10072970u;
constexpr uint32_t unknown_libname_61 = 0x100729C0u;
constexpr uint32_t FUN_100729d0 = 0x100729D0u;
constexpr uint32_t CMoTask_VT_1032C908_Destructor = 0x100729E0u;
constexpr uint32_t FUN_10072c30 = 0x10072C30u;
constexpr uint32_t sub_10072DB0 = 0x10072DB0u;
constexpr uint32_t MeshResourceMng_LoadSystemResources = 0x10072ED0u;
constexpr uint32_t ResourceHandle_Deref = 0x10072FB0u;
constexpr uint32_t ResourceHandle_Deref_v1 = 0x10072FB0u;
constexpr uint32_t FUN_10072fd0 = 0x10072FD0u;
constexpr uint32_t sub_10072FD0 = 0x10072FD0u;
constexpr uint32_t FUN_100730e0 = 0x100730E0u;
constexpr uint32_t FUN_100731b0 = 0x100731B0u;
constexpr uint32_t sub_100731B0 = 0x100731B0u;
constexpr uint32_t MeshResourceMng_ResolveTree = 0x10073430u;
constexpr uint32_t FUN_10073a60 = 0x10073A60u;
constexpr uint32_t MoRes_FreeIfUnreferenced = 0x10073B20u;
constexpr uint32_t MoRes_FreeIfUnreferenced_v1 = 0x10073B20u;
constexpr uint32_t FUN_10073b80 = 0x10073B80u;
constexpr uint32_t FUN_10073ba0 = 0x10073BA0u;
constexpr uint32_t FUN_10073d80 = 0x10073D80u;
constexpr uint32_t FUN_10074110 = 0x10074110u;
constexpr uint32_t MeshResourceMng_AllocHandle = 0x10074140u;
constexpr uint32_t MeshResourceMng_AllocHandle_v1 = 0x10074140u;
constexpr uint32_t NodePool_ReleaseNode = 0x10074180u;
constexpr uint32_t NodePool_ReleaseNode_v1 = 0x10074180u;
constexpr uint32_t FUN_100741e0 = 0x100741E0u;
constexpr uint32_t FUN_10074280 = 0x10074280u;
constexpr uint32_t GetGlobal_1047D114 = 0x10074290u;
constexpr uint32_t FUN_100742a0 = 0x100742A0u;
constexpr uint32_t ResourceIter_FindFirst = 0x100742B0u;
constexpr uint32_t ResourceIter_FindFirst_v1 = 0x100742B0u;
constexpr uint32_t FUN_10074310 = 0x10074310u;
constexpr uint32_t FUN_10074380 = 0x10074380u;
constexpr uint32_t FUN_100743d0 = 0x100743D0u;
constexpr uint32_t ResourceIter_FindNext = 0x100745C0u;
constexpr uint32_t ResourceIter_FindNext_v1 = 0x100745C0u;
constexpr uint32_t MoProcessor_CallVSlot49 = 0x100746A0u;
constexpr uint32_t MoProcessor_CallVSlot49_v1 = 0x100746A0u;
constexpr uint32_t CMoTask_Constructor = 0x10074740u;
constexpr uint32_t CMoTask_Constructor_v1 = 0x10074740u;
constexpr uint32_t CMoTask_Destructor = 0x100747C0u;
constexpr uint32_t CMoTask_Destructor_v1 = 0x100747C0u;
constexpr uint32_t FUN_100747f0 = 0x100747F0u;
constexpr uint32_t FUN_10074850 = 0x10074850u;
constexpr uint32_t FUN_10074890 = 0x10074890u;
constexpr uint32_t CMoTask_ResetFields = 0x100748F0u;
constexpr uint32_t CMoTask_ResetFields_v1 = 0x100748F0u;
constexpr uint32_t CMoTask_RegisterWithMng = 0x100749B0u;
constexpr uint32_t CMoTask_RegisterWithMng_v1 = 0x100749B0u;
constexpr uint32_t FUN_100749f0 = 0x100749F0u;
constexpr uint32_t FUN_10074a00 = 0x10074A00u;
constexpr uint32_t FUN_10074a00_v1 = 0x10074A00u;
constexpr uint32_t CMoTaskMng_Forward10075120 = 0x10074A10u;
constexpr uint32_t FUN_10074a20 = 0x10074A20u;
constexpr uint32_t FUN_10074a50 = 0x10074A50u;
constexpr uint32_t FUN_10074ab0 = 0x10074AB0u;
constexpr uint32_t FUN_10074ad0 = 0x10074AD0u;
constexpr uint32_t CMoTaskMng_Destructor = 0x10074B60u;
constexpr uint32_t sub_10074BA0 = 0x10074BA0u;
constexpr uint32_t sub_10074C30 = 0x10074C30u;
constexpr uint32_t FUN_10074cc0 = 0x10074CC0u;
constexpr uint32_t FUN_10074cf0 = 0x10074CF0u;
constexpr uint32_t FUN_10074df0 = 0x10074DF0u;
constexpr uint32_t FUN_10074f30 = 0x10074F30u;
constexpr uint32_t FUN_10074f80 = 0x10074F80u;
constexpr uint32_t FUN_10075000 = 0x10075000u;
constexpr uint32_t FUN_10075120 = 0x10075120u;
constexpr uint32_t FUN_100751b0 = 0x100751B0u;
constexpr uint32_t Gfx_InitCubeMapTextures = 0x10075280u;
constexpr uint32_t TexRef_ResolveByName = 0x10075520u;
constexpr uint32_t TexRef_EnsureRenderTarget = 0x100755A0u;
constexpr uint32_t TextureHolder_BindWarped = 0x10075700u;
constexpr uint32_t unknown_libname_63 = 0x10075950u;
constexpr uint32_t TexRef_SetTexture = 0x10075960u;
constexpr uint32_t TexRef_SetTexture_v1 = 0x10075960u;
constexpr uint32_t TextureHolder_BindTinted = 0x100759C0u;
constexpr uint32_t unknown_libname_64 = 0x10075C30u;
constexpr uint32_t FUN_10076750 = 0x10076750u;
constexpr uint32_t FUN_100769f0 = 0x100769F0u;
constexpr uint32_t FUN_10076a70 = 0x10076A70u;
constexpr uint32_t getNounFromPronoun = 0x10076FF0u;
constexpr uint32_t FUN_10077a00 = 0x10077A00u;
constexpr uint32_t FUN_10077ad0 = 0x10077AD0u;
constexpr uint32_t FUN_10077b40 = 0x10077B40u;
constexpr uint32_t getLegalPCname = 0x10077C30u;
constexpr uint32_t FUN_10077d70 = 0x10077D70u;
constexpr uint32_t FUN_10077ed0 = 0x10077ED0u;
constexpr uint32_t FUN_10077fb0 = 0x10077FB0u;
constexpr uint32_t thunk_FUN_100e97d0 = 0x10078090u;
constexpr uint32_t FUN_100780a0 = 0x100780A0u;
constexpr uint32_t FUN_100780d0 = 0x100780D0u;
constexpr uint32_t FUN_10079170 = 0x10079170u;
constexpr uint32_t SeekIdByString = 0x1007A1C0u;
constexpr uint32_t FUN_1007bf30 = 0x1007BF30u;
constexpr uint32_t FUN_1007cd60 = 0x1007CD60u;
constexpr uint32_t FUN_1007cf80 = 0x1007CF80u;
constexpr uint32_t FUN_10080c70 = 0x10080C70u;
constexpr uint32_t FUN_10080d00 = 0x10080D00u;
constexpr uint32_t FUN_10080d20 = 0x10080D20u;
constexpr uint32_t FUN_10080d90 = 0x10080D90u;
constexpr uint32_t CommandCalc = 0x10080DC0u;
constexpr uint32_t FUN_10081440 = 0x10081440u;
constexpr uint32_t FUN_10081480 = 0x10081480u;
constexpr uint32_t ActorHandle_Constructor = 0x100814A0u;
constexpr uint32_t ActorHandle_Constructor_v1 = 0x100814A0u;
constexpr uint32_t ActorHandle_Destructor = 0x100814E0u;
constexpr uint32_t ActorHandle_Destructor_v1 = 0x100814E0u;
constexpr uint32_t FUN_100814f0 = 0x100814F0u;
constexpr uint32_t sub_100814F0 = 0x100814F0u;
constexpr uint32_t FUN_10081530 = 0x10081530u;
constexpr uint32_t ActorHandle_Resolve = 0x10081550u;
constexpr uint32_t ActorHandle_Resolve_v1 = 0x10081550u;
constexpr uint32_t sub_10081600 = 0x10081600u;
constexpr uint32_t FUN_10081a30 = 0x10081A30u;
constexpr uint32_t Actor_ResetDrawLists = 0x10081A50u;
constexpr uint32_t FUN_10081ae0 = 0x10081AE0u;
constexpr uint32_t CXiActor_Init = 0x10081BA0u;
constexpr uint32_t FUN_10081cf0 = 0x10081CF0u;
constexpr uint32_t Actor_UnlinkAllAttachLists = 0x10082B10u;
constexpr uint32_t FUN_10082bf0 = 0x10082BF0u;
constexpr uint32_t CXiActorDraw_UpdateDrawList = 0x10082D70u;
constexpr uint32_t FUN_10082fa0 = 0x10082FA0u;
constexpr uint32_t FUN_10082fb0 = 0x10082FB0u;
constexpr uint32_t CXiActor_NameCalc = 0x100830C0u;
constexpr uint32_t FUN_10083430 = 0x10083430u;
constexpr uint32_t FUN_10083620 = 0x10083620u;
constexpr uint32_t FUN_10083750 = 0x10083750u;
constexpr uint32_t FUN_10083b40 = 0x10083B40u;
constexpr uint32_t FUN_10083d00 = 0x10083D00u;
constexpr uint32_t FUN_10083f20 = 0x10083F20u;
constexpr uint32_t FUN_10084100 = 0x10084100u;
constexpr uint32_t CXiActor_IsModelMissingOrFlag2000 = 0x10084140u;
constexpr uint32_t CXiActor_IsModelMissingOrFlag2000_v1 = 0x10084140u;
constexpr uint32_t CXiActor_ModelTest95790 = 0x10084330u;
constexpr uint32_t CXiActor_ModelTest95680 = 0x10084350u;
constexpr uint32_t CXiActor_ModelTest95680_v1 = 0x10084350u;
constexpr uint32_t CXiActor_ModelTest956A0 = 0x10084370u;
constexpr uint32_t CXiActor_ModelTest956A0_v1 = 0x10084370u;
constexpr uint32_t CXiActor_GetModelField170 = 0x10084390u;
constexpr uint32_t CXiActor_GetModelField170_v1 = 0x10084390u;
constexpr uint32_t CXiActor_HasNonzeroField150or158 = 0x100843B0u;
constexpr uint32_t CXiActor_GetModelByteEE = 0x10084400u;
constexpr uint32_t CXiActor_GetModelByteEE_v1 = 0x10084400u;
constexpr uint32_t CXiActor_GetModelByteEF = 0x10084420u;
constexpr uint32_t CXiActor_GetModelByteEF_v1 = 0x10084420u;
constexpr uint32_t CXiActor_GetModelPartWord = 0x10084480u;
constexpr uint32_t CXiActor_GetModelPartWord_v1 = 0x10084480u;
constexpr uint32_t CXiActor_GetModelFlags120Top2 = 0x10084500u;
constexpr uint32_t CXiActor_GetModelFlags130Top2 = 0x10084520u;
constexpr uint32_t CXiActor_GetModelFloat9AEC0 = 0x10084540u;
constexpr uint32_t CXiActor_IsModelMissingOrSuppressed = 0x10084580u;
constexpr uint32_t CXiActor_IsModelMissingOrSuppressed_v1 = 0x10084580u;
constexpr uint32_t CXiActor_TestModelFlag128_8 = 0x100845B0u;
constexpr uint32_t sub_100845E0 = 0x100845E0u;
constexpr uint32_t FUN_10084620 = 0x10084620u;
constexpr uint32_t CXiActor_TestModelFlags100000_200000_4000 = 0x10084640u;
constexpr uint32_t CXiActor_GetModelByte1DB = 0x10084720u;
constexpr uint32_t CXiActor_set_field_B4 = 0x10085010u;
constexpr uint32_t CXiActor_SpawnResourceGenerators = 0x10085060u;
constexpr uint32_t CXiActor_SpawnResourceGenerators_v1 = 0x10085060u;
constexpr uint32_t FUN_10085240 = 0x10085240u;
constexpr uint32_t FUN_10085270 = 0x10085270u;
constexpr uint32_t FUN_100852d0 = 0x100852D0u;
constexpr uint32_t CXiActor_IsInModelSlotTable = 0x10085490u;
constexpr uint32_t CXiActor_IsInModelSlotTable_v1 = 0x10085490u;
constexpr uint32_t FUN_10085500 = 0x10085500u;
constexpr uint32_t FUN_10085d50 = 0x10085D50u;
constexpr uint32_t FUN_10085d60 = 0x10085D60u;
constexpr uint32_t Actor_DrawWorldText = 0x10086210u;
constexpr uint32_t Actor_DrawWorldText_v1 = 0x10086210u;
constexpr uint32_t DrawNameplateSecondString = 0x10086900u;
constexpr uint32_t FUN_10086fc0 = 0x10086FC0u;
constexpr uint32_t FUN_10086fe0 = 0x10086FE0u;
constexpr uint32_t FUN_10087010 = 0x10087010u;
constexpr uint32_t FUN_10087060 = 0x10087060u;
constexpr uint32_t IsEvent = 0x10087070u;
constexpr uint32_t FUN_100870e0 = 0x100870E0u;
constexpr uint32_t FUN_10087110 = 0x10087110u;
constexpr uint32_t thunk_FUN_10087230 = 0x10087220u;
constexpr uint32_t FUN_100872c0 = 0x100872C0u;
constexpr uint32_t FUN_100873b0 = 0x100873B0u;
constexpr uint32_t FUN_100875d0 = 0x100875D0u;
constexpr uint32_t FUN_10087660 = 0x10087660u;
constexpr uint32_t FUN_10087670 = 0x10087670u;
constexpr uint32_t FUN_10087680 = 0x10087680u;
constexpr uint32_t FUN_10087690 = 0x10087690u;
constexpr uint32_t FUN_100876b0 = 0x100876B0u;
constexpr uint32_t FUN_10087760 = 0x10087760u;
constexpr uint32_t FUN_10087830 = 0x10087830u;
constexpr uint32_t FUN_10087d40 = 0x10087D40u;
constexpr uint32_t FUN_10087dc0 = 0x10087DC0u;
constexpr uint32_t FUN_10087df0 = 0x10087DF0u;
constexpr uint32_t FUN_10087e40 = 0x10087E40u;
constexpr uint32_t FUN_10087e80 = 0x10087E80u;
constexpr uint32_t FUN_10087f00 = 0x10087F00u;
constexpr uint32_t FUN_10088270 = 0x10088270u;
constexpr uint32_t FUN_100882d0 = 0x100882D0u;
constexpr uint32_t FUN_100883e0 = 0x100883E0u;
constexpr uint32_t FUN_10089100 = 0x10089100u;
constexpr uint32_t FUN_1008a0e0 = 0x1008A0E0u;
constexpr uint32_t FUN_1008a120 = 0x1008A120u;
constexpr uint32_t FUN_1008a160 = 0x1008A160u;
constexpr uint32_t FUN_1008a1a0 = 0x1008A1A0u;
constexpr uint32_t FUN_1008a2f0 = 0x1008A2F0u;
constexpr uint32_t FUN_1008a350 = 0x1008A350u;
constexpr uint32_t FUN_1008a7b0 = 0x1008A7B0u;
constexpr uint32_t FUN_1008a930 = 0x1008A930u;
constexpr uint32_t FUN_1008a9e0 = 0x1008A9E0u;
constexpr uint32_t FUN_1008aa30 = 0x1008AA30u;
constexpr uint32_t FUN_1008abe0 = 0x1008ABE0u;
constexpr uint32_t FUN_1008af60 = 0x1008AF60u;
constexpr uint32_t FUN_1008b100 = 0x1008B100u;
constexpr uint32_t FUN_1008b170 = 0x1008B170u;
constexpr uint32_t FUN_1008b270 = 0x1008B270u;
constexpr uint32_t FUN_1008b2b0 = 0x1008B2B0u;
constexpr uint32_t FUN_1008b380 = 0x1008B380u;
constexpr uint32_t FUN_1008b410 = 0x1008B410u;
constexpr uint32_t FUN_1008b460 = 0x1008B460u;
constexpr uint32_t FUN_1008c480 = 0x1008C480u;
constexpr uint32_t FUN_1008c6c0 = 0x1008C6C0u;
constexpr uint32_t FUN_1008cce0 = 0x1008CCE0u;
constexpr uint32_t FUN_1008cd20 = 0x1008CD20u;
constexpr uint32_t FUN_1008cd90 = 0x1008CD90u;
constexpr uint32_t FUN_1008d910 = 0x1008D910u;
constexpr uint32_t FUN_1008d980 = 0x1008D980u;
constexpr uint32_t FUN_1008da00 = 0x1008DA00u;
constexpr uint32_t FUN_1008db40 = 0x1008DB40u;
constexpr uint32_t FUN_1008e130 = 0x1008E130u;
constexpr uint32_t FUN_1008e140 = 0x1008E140u;
constexpr uint32_t FUN_1008e150 = 0x1008E150u;
constexpr uint32_t FUN_1008e260 = 0x1008E260u;
constexpr uint32_t FUN_1008e290 = 0x1008E290u;
constexpr uint32_t FUN_1008e3f0 = 0x1008E3F0u;
constexpr uint32_t FUN_1008e4d0 = 0x1008E4D0u;
constexpr uint32_t FUN_1008e740 = 0x1008E740u;
constexpr uint32_t FUN_1008e870 = 0x1008E870u;
constexpr uint32_t FUN_1008e8d0 = 0x1008E8D0u;
constexpr uint32_t FUN_1008e9a0 = 0x1008E9A0u;
constexpr uint32_t FUN_1008ee30 = 0x1008EE30u;
constexpr uint32_t FUN_1008f1d0 = 0x1008F1D0u;
constexpr uint32_t FUN_1008f740 = 0x1008F740u;
constexpr uint32_t FUN_100926c0 = 0x100926C0u;
constexpr uint32_t FUN_10092710 = 0x10092710u;
constexpr uint32_t FUN_10092760 = 0x10092760u;
constexpr uint32_t FUN_10092780 = 0x10092780u;
constexpr uint32_t FUN_10092800 = 0x10092800u;
constexpr uint32_t FUN_10092870 = 0x10092870u;
constexpr uint32_t FUN_10092900 = 0x10092900u;
constexpr uint32_t FUN_100929a0 = 0x100929A0u;
constexpr uint32_t FUN_100929d0 = 0x100929D0u;
constexpr uint32_t FUN_10092c20 = 0x10092C20u;
constexpr uint32_t FUN_10093430 = 0x10093430u;
constexpr uint32_t FUN_100935c0 = 0x100935C0u;
constexpr uint32_t Unit10086E90_ResetGlobals = 0x10093720u;
constexpr uint32_t FUN_10093a30 = 0x10093A30u;
constexpr uint32_t FUN_10093a90 = 0x10093A90u;
constexpr uint32_t FUN_10093ad0 = 0x10093AD0u;
constexpr uint32_t FUN_10093af0 = 0x10093AF0u;
constexpr uint32_t FUN_10093b10 = 0x10093B10u;
constexpr uint32_t FUN_10093b30 = 0x10093B30u;
constexpr uint32_t FUN_10093b50 = 0x10093B50u;
constexpr uint32_t FUN_10093b70 = 0x10093B70u;
constexpr uint32_t FUN_10093da0 = 0x10093DA0u;
constexpr uint32_t Net_SendPacket00COnce = 0x10093DB0u;
constexpr uint32_t Net_SendPacket00COnce_v1 = 0x10093DB0u;
constexpr uint32_t FUN_10093e30 = 0x10093E30u;
constexpr uint32_t FUN_10093e50 = 0x10093E50u;
constexpr uint32_t FUN_10094190 = 0x10094190u;
constexpr uint32_t FUN_10095660 = 0x10095660u;
constexpr uint32_t FUN_10095680 = 0x10095680u;
constexpr uint32_t FUN_100956a0 = 0x100956A0u;
constexpr uint32_t FUN_100956c0 = 0x100956C0u;
constexpr uint32_t FUN_100956f0 = 0x100956F0u;
constexpr uint32_t FUN_10095750 = 0x10095750u;
constexpr uint32_t FUN_10095790 = 0x10095790u;
constexpr uint32_t FUN_10095850 = 0x10095850u;
constexpr uint32_t FUN_10095900 = 0x10095900u;
constexpr uint32_t FUN_10095d80 = 0x10095D80u;
constexpr uint32_t FUN_10095da0 = 0x10095DA0u;
constexpr uint32_t FUN_10095dd0 = 0x10095DD0u;
constexpr uint32_t FUN_10095e30 = 0x10095E30u;
constexpr uint32_t FUN_10095e90 = 0x10095E90u;
constexpr uint32_t FUN_10096040 = 0x10096040u;
constexpr uint32_t FUN_100960f0 = 0x100960F0u;
constexpr uint32_t FUN_100961d0 = 0x100961D0u;
constexpr uint32_t FUN_10096200 = 0x10096200u;
constexpr uint32_t FUN_10096640 = 0x10096640u;
constexpr uint32_t FUN_10096c90 = 0x10096C90u;
constexpr uint32_t FUN_100973b0 = 0x100973B0u;
constexpr uint32_t FUN_10097500 = 0x10097500u;
constexpr uint32_t FUN_10097730 = 0x10097730u;
constexpr uint32_t FUN_100977a0 = 0x100977A0u;
constexpr uint32_t FUN_100977f0 = 0x100977F0u;
constexpr uint32_t Entity_BuildDisplayName = 0x10097840u;
constexpr uint32_t FUN_100979b0 = 0x100979B0u;
constexpr uint32_t FUN_10097a20 = 0x10097A20u;
constexpr uint32_t FUN_10097af0 = 0x10097AF0u;
constexpr uint32_t FUN_10097d50 = 0x10097D50u;
constexpr uint32_t FUN_10097e50 = 0x10097E50u;
constexpr uint32_t FUN_10097fd0 = 0x10097FD0u;
constexpr uint32_t FUN_10098180 = 0x10098180u;
constexpr uint32_t FUN_10098360 = 0x10098360u;
constexpr uint32_t FUN_100983a0 = 0x100983A0u;
constexpr uint32_t FUN_100983f0 = 0x100983F0u;
constexpr uint32_t FUN_100991f0 = 0x100991F0u;
constexpr uint32_t FUN_10099730 = 0x10099730u;
constexpr uint32_t FUN_100997b0 = 0x100997B0u;
constexpr uint32_t FUN_10099a40 = 0x10099A40u;
constexpr uint32_t FUN_1009aad0 = 0x1009AAD0u;
constexpr uint32_t FUN_1009aaf0 = 0x1009AAF0u;
constexpr uint32_t FUN_1009ab40 = 0x1009AB40u;
constexpr uint32_t FUN_1009acb0 = 0x1009ACB0u;
constexpr uint32_t FUN_1009ace0 = 0x1009ACE0u;
constexpr uint32_t FUN_1009ad20 = 0x1009AD20u;
constexpr uint32_t FUN_1009ade0 = 0x1009ADE0u;
constexpr uint32_t FUN_1009aec0 = 0x1009AEC0u;
constexpr uint32_t FUN_1009af60 = 0x1009AF60u;
constexpr uint32_t FUN_1009afe0 = 0x1009AFE0u;
constexpr uint32_t FUN_1009f2f0 = 0x1009F2F0u;
constexpr uint32_t FUN_1009f4c0 = 0x1009F4C0u;
constexpr uint32_t FUN_100a2720 = 0x100A2720u;
constexpr uint32_t CMoTask_VT_1032D6D4_Destructor = 0x100A2810u;
constexpr uint32_t FUN_100a3cd0 = 0x100A3CD0u;
constexpr uint32_t FUN_100a3d00 = 0x100A3D00u;
constexpr uint32_t CXiCollisionActor_Constructor = 0x100A4C20u;
constexpr uint32_t FUN_100a5770 = 0x100A5770u;
constexpr uint32_t FUN_100a57c0 = 0x100A57C0u;
constexpr uint32_t FUN_100a57f0 = 0x100A57F0u;
constexpr uint32_t FUN_100a5e10 = 0x100A5E10u;
constexpr uint32_t FUN_100a5ef0 = 0x100A5EF0u;
constexpr uint32_t FUN_100a6070 = 0x100A6070u;
constexpr uint32_t FUN_100a6240 = 0x100A6240u;
constexpr uint32_t CXiControlActor_Constructor = 0x100A89E0u;
constexpr uint32_t CXiControlActor_Destructor = 0x100A8CA0u;
constexpr uint32_t FUN_100a8cc0 = 0x100A8CC0u;
constexpr uint32_t CMoTask_VT_1032E808_Destructor = 0x100A9D30u;
constexpr uint32_t CMoTask_VT_1032E808_ScalarDeletingDtor = 0x100A9DB0u;
constexpr uint32_t FUN_100aafb0 = 0x100AAFB0u;
constexpr uint32_t FUN_100ab320 = 0x100AB320u;
constexpr uint32_t FUN_100ab550 = 0x100AB550u;
constexpr uint32_t FUN_100ab750 = 0x100AB750u;
constexpr uint32_t FUN_100ab830 = 0x100AB830u;
constexpr uint32_t FUN_100ab880 = 0x100AB880u;
constexpr uint32_t FUN_100ab9a0 = 0x100AB9A0u;
constexpr uint32_t FUN_100abc70 = 0x100ABC70u;
constexpr uint32_t FUN_100ae3f0 = 0x100AE3F0u;
constexpr uint32_t FUN_100ae960 = 0x100AE960u;
constexpr uint32_t FUN_100aea40 = 0x100AEA40u;
constexpr uint32_t FUN_100aec70 = 0x100AEC70u;
constexpr uint32_t FUN_100aed00 = 0x100AED00u;
constexpr uint32_t FUN_100b37a0 = 0x100B37A0u;
constexpr uint32_t XiEvent_EventIdle = 0x100BCD10u;
constexpr uint32_t FUN_100bd0e0 = 0x100BD0E0u;
constexpr uint32_t FUN_100bd5f0 = 0x100BD5F0u;
constexpr uint32_t FUN_100bd820 = 0x100BD820u;
constexpr uint32_t FUN_100c2d00 = 0x100C2D00u;
constexpr uint32_t FUN_100c2e20 = 0x100C2E20u;
constexpr uint32_t FUN_100c2fb0 = 0x100C2FB0u;
constexpr uint32_t FUN_100c2fe0 = 0x100C2FE0u;
constexpr uint32_t FUN_100c3020 = 0x100C3020u;
constexpr uint32_t FUN_100c3030 = 0x100C3030u;
constexpr uint32_t FUN_100c3050 = 0x100C3050u;
constexpr uint32_t FUN_100c3060 = 0x100C3060u;
constexpr uint32_t FUN_100c30f0 = 0x100C30F0u;
constexpr uint32_t FUN_100c3100 = 0x100C3100u;
constexpr uint32_t FUN_100c3120 = 0x100C3120u;
constexpr uint32_t FUN_100c3130 = 0x100C3130u;
constexpr uint32_t FUN_100c3140 = 0x100C3140u;
constexpr uint32_t FUN_100c3150 = 0x100C3150u;
constexpr uint32_t FUN_100c3160 = 0x100C3160u;
constexpr uint32_t FUN_100c3170 = 0x100C3170u;
constexpr uint32_t FUN_100c3190 = 0x100C3190u;
constexpr uint32_t FUN_100c31f0 = 0x100C31F0u;
constexpr uint32_t FUN_100c3200 = 0x100C3200u;
constexpr uint32_t FUN_100c3210 = 0x100C3210u;
constexpr uint32_t FUN_100c3220 = 0x100C3220u;
constexpr uint32_t FUN_100c3260 = 0x100C3260u;
constexpr uint32_t FUN_100c3300 = 0x100C3300u;
constexpr uint32_t FUN_100c3e70 = 0x100C3E70u;
constexpr uint32_t FUN_100c4400 = 0x100C4400u;
constexpr uint32_t FUN_100c4bc0 = 0x100C4BC0u;
constexpr uint32_t FUN_100c51c0 = 0x100C51C0u;
constexpr uint32_t FUN_100c5260 = 0x100C5260u;
constexpr uint32_t FUN_100c5270 = 0x100C5270u;
constexpr uint32_t FUN_100c5280 = 0x100C5280u;
constexpr uint32_t thunk_FUN_101cce00 = 0x100C5290u;
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
constexpr uint32_t FUN_100cd4e0 = 0x100CD4E0u;
constexpr uint32_t FUN_100ce0e0 = 0x100CE0E0u;
constexpr uint32_t FUN_100ce160 = 0x100CE160u;
constexpr uint32_t CXiSkeletonActor_ReverseFindRes = 0x100CE480u;
constexpr uint32_t FUN_100cf750 = 0x100CF750u;
constexpr uint32_t FUN_100cfb40 = 0x100CFB40u;
constexpr uint32_t FUN_100d0b50 = 0x100D0B50u;
constexpr uint32_t FUN_100d0e80 = 0x100D0E80u;
constexpr uint32_t FUN_100d0fc0 = 0x100D0FC0u;
constexpr uint32_t CXiSkeletonActor_IsReadCompleteBody = 0x100D12D0u;
constexpr uint32_t CXiSkeletonActor_IsReadCompleteBody_v1 = 0x100D12D0u;
constexpr uint32_t FUN_100d1450 = 0x100D1450u;
constexpr uint32_t FUN_100d2800 = 0x100D2800u;
constexpr uint32_t FUN_100d2d70 = 0x100D2D70u;
constexpr uint32_t FUN_100d2e30 = 0x100D2E30u;
constexpr uint32_t CXiSkeletonActor_OnSkeletonLoaded = 0x100D2F40u;
constexpr uint32_t CXiSkeletonActor_LoadSkeletonSync = 0x100D2F60u;
constexpr uint32_t CXiSkeletonActor_LoadSkeletonResource = 0x100D4330u;
constexpr uint32_t CXiSkeletonActor_LoadSkeletonResource_v1 = 0x100D4330u;
constexpr uint32_t FUN_100d4410 = 0x100D4410u;
constexpr uint32_t FUN_100d44b0 = 0x100D44B0u;
constexpr uint32_t CXiSkeletonActor_DeleteOwnedList = 0x100D44F0u;
constexpr uint32_t FUN_100d4520 = 0x100D4520u;
constexpr uint32_t FUN_100d5430 = 0x100D5430u;
constexpr uint32_t sub_100D5430 = 0x100D5430u;
constexpr uint32_t CXiSkeletonActor_CalcGroundTiltMatrix = 0x100D54C0u;
constexpr uint32_t CXiSkeletonActor_UpdateHeadLook = 0x100D5B10u;
constexpr uint32_t FUN_100d5fe0 = 0x100D5FE0u;
constexpr uint32_t FUN_100d6180 = 0x100D6180u;
constexpr uint32_t FUN_100d6240 = 0x100D6240u;
constexpr uint32_t sub_100D6240 = 0x100D6240u;
constexpr uint32_t FUN_100d65b0 = 0x100D65B0u;
constexpr uint32_t Actor_PrepareDrawGlobals = 0x100D6D30u;
constexpr uint32_t CXiSkeletonActorRes_Constructor = 0x100D7020u;
constexpr uint32_t CXiSkeletonActorRes_Destructor = 0x100D7050u;
constexpr uint32_t CXiSkeletonActorRes_GetLinkedActor = 0x100D7090u;
constexpr uint32_t PHP_MD4Init = 0x100D7420u;
constexpr uint32_t _opiemd4update = 0x100D7450u;
constexpr uint32_t FUN_100d7510 = 0x100D7510u;
constexpr uint32_t FUN_100d7560 = 0x100D7560u;
constexpr uint32_t FUN_100d75a0 = 0x100D75A0u;
constexpr uint32_t FUN_100d7e90 = 0x100D7E90u;
constexpr uint32_t FUN_100d7ea0 = 0x100D7EA0u;
constexpr uint32_t FUN_100d7ed0 = 0x100D7ED0u;
constexpr uint32_t FUN_100d8490 = 0x100D8490u;
constexpr uint32_t FUN_100d8a20 = 0x100D8A20u;
constexpr uint32_t FUN_100d93e0 = 0x100D93E0u;
constexpr uint32_t FUN_100d9410 = 0x100D9410u;
constexpr uint32_t FUN_100d9430 = 0x100D9430u;
constexpr uint32_t FUN_100d9460 = 0x100D9460u;
constexpr uint32_t FUN_100d9830 = 0x100D9830u;
constexpr uint32_t FUN_100d9950 = 0x100D9950u;
constexpr uint32_t FUN_100d9b00 = 0x100D9B00u;
constexpr uint32_t sub_100DA6A0 = 0x100DA6A0u;
constexpr uint32_t FUN_100daf30 = 0x100DAF30u;
constexpr uint32_t GameMode_PollPendingState = 0x100DB140u;
constexpr uint32_t GameMode_PollPendingState_v1 = 0x100DB140u;
constexpr uint32_t GameMode_RequestState = 0x100DB170u;
constexpr uint32_t FUN_100db780 = 0x100DB780u;
constexpr uint32_t sub_100DB790 = 0x100DB790u;
constexpr uint32_t FUN_100db930 = 0x100DB930u;
constexpr uint32_t FUN_100dbca0 = 0x100DBCA0u;
constexpr uint32_t FUN_100dbde0 = 0x100DBDE0u;
constexpr uint32_t timeGetTime_100DBE00 = 0x100DBE00u;
constexpr uint32_t FUN_100dbe10 = 0x100DBE10u;
constexpr uint32_t FUN_100dbe30 = 0x100DBE30u;
constexpr uint32_t sub_100DBE50 = 0x100DBE50u;
constexpr uint32_t FUN_100dbeb0 = 0x100DBEB0u;
constexpr uint32_t FUN_100dbf00 = 0x100DBF00u;
constexpr uint32_t FUN_100dbf20 = 0x100DBF20u;
constexpr uint32_t FUN_100dbfe0 = 0x100DBFE0u;
constexpr uint32_t FUN_100dc060 = 0x100DC060u;
constexpr uint32_t FUN_100dc080 = 0x100DC080u;
constexpr uint32_t FUN_100dc090 = 0x100DC090u;
constexpr uint32_t FUN_100dc120 = 0x100DC120u;
constexpr uint32_t FUN_100dc130 = 0x100DC130u;
constexpr uint32_t FUN_100dc1b0 = 0x100DC1B0u;
constexpr uint32_t FUN_100dc1f0 = 0x100DC1F0u;
constexpr uint32_t FUN_100dc210 = 0x100DC210u;
constexpr uint32_t FUN_100dc250 = 0x100DC250u;
constexpr uint32_t FUN_100dc260 = 0x100DC260u;
constexpr uint32_t FUN_100dc270 = 0x100DC270u;
constexpr uint32_t FUN_100dc2c0 = 0x100DC2C0u;
constexpr uint32_t FUN_100dc2d0 = 0x100DC2D0u;
constexpr uint32_t FUN_100dc3b0 = 0x100DC3B0u;
constexpr uint32_t FUN_100dc420 = 0x100DC420u;
constexpr uint32_t FUN_100dc650 = 0x100DC650u;
constexpr uint32_t FUN_100dca00 = 0x100DCA00u;
constexpr uint32_t FUN_100dcf60 = 0x100DCF60u;
constexpr uint32_t FUN_100dd800 = 0x100DD800u;
constexpr uint32_t FUN_100dd990 = 0x100DD990u;
constexpr uint32_t FUN_100ddd20 = 0x100DDD20u;
constexpr uint32_t FUN_100dde40 = 0x100DDE40u;
constexpr uint32_t FUN_100dde90 = 0x100DDE90u;
constexpr uint32_t FUN_100ddec0 = 0x100DDEC0u;
constexpr uint32_t FUN_100ddf60 = 0x100DDF60u;
constexpr uint32_t FUN_100df5d0 = 0x100DF5D0u;
constexpr uint32_t FUN_100df6e0 = 0x100DF6E0u;
constexpr uint32_t FUN_100df780 = 0x100DF780u;
constexpr uint32_t FUN_100df7c0 = 0x100DF7C0u;
constexpr uint32_t FUN_100e0100 = 0x100E0100u;
constexpr uint32_t FUN_100e08d0 = 0x100E08D0u;
constexpr uint32_t FUN_100e09a0 = 0x100E09A0u;
constexpr uint32_t FUN_100e0a00 = 0x100E0A00u;
constexpr uint32_t FUN_100e0aa0 = 0x100E0AA0u;
constexpr uint32_t FUN_100e0b10 = 0x100E0B10u;
constexpr uint32_t FUN_100e0d50 = 0x100E0D50u;
constexpr uint32_t FUN_100e19c0 = 0x100E19C0u;
constexpr uint32_t FUN_100e1ad0 = 0x100E1AD0u;
constexpr uint32_t FUN_100e1b50 = 0x100E1B50u;
constexpr uint32_t FUN_100e1c00 = 0x100E1C00u;
constexpr uint32_t FUN_100e1c30 = 0x100E1C30u;
constexpr uint32_t FUN_100e2e70 = 0x100E2E70u;
constexpr uint32_t _EVP_EncryptInit_ex = 0x100E2EE0u;
constexpr uint32_t FUN_100e33f0 = 0x100E33F0u;
constexpr uint32_t FUN_100e4110 = 0x100E4110u;
constexpr uint32_t FUN_100e4330 = 0x100E4330u;
constexpr uint32_t FUN_100e43f0 = 0x100E43F0u;
constexpr uint32_t FUN_100e47a0 = 0x100E47A0u;
constexpr uint32_t FUN_100e5e30 = 0x100E5E30u;
constexpr uint32_t FUN_100e5ff0 = 0x100E5FF0u;
constexpr uint32_t FUN_100e6020 = 0x100E6020u;
constexpr uint32_t FUN_100e6080 = 0x100E6080u;
constexpr uint32_t FUN_100e61c0 = 0x100E61C0u;
constexpr uint32_t FUN_100e62c0 = 0x100E62C0u;
constexpr uint32_t FUN_100e6370 = 0x100E6370u;
constexpr uint32_t FUN_100e63c0 = 0x100E63C0u;
constexpr uint32_t FUN_100e63e0 = 0x100E63E0u;
constexpr uint32_t FUN_100e6650 = 0x100E6650u;
constexpr uint32_t FUN_100e6790 = 0x100E6790u;
constexpr uint32_t FUN_100e67e0 = 0x100E67E0u;
constexpr uint32_t FUN_100e6810 = 0x100E6810u;
constexpr uint32_t FUN_100e6830 = 0x100E6830u;
constexpr uint32_t FUN_100e6860 = 0x100E6860u;
constexpr uint32_t FUN_100e6880 = 0x100E6880u;
constexpr uint32_t FUN_100e6b10 = 0x100E6B10u;
constexpr uint32_t FUN_100e6b20 = 0x100E6B20u;
constexpr uint32_t FUN_100e6c40 = 0x100E6C40u;
constexpr uint32_t FUN_100e7260 = 0x100E7260u;
constexpr uint32_t FUN_100e7b20 = 0x100E7B20u;
constexpr uint32_t FUN_100e84b0 = 0x100E84B0u;
constexpr uint32_t FUN_100e8b70 = 0x100E8B70u;
constexpr uint32_t FUN_100e8b80 = 0x100E8B80u;
constexpr uint32_t FUN_100e8c70 = 0x100E8C70u;
constexpr uint32_t FUN_100e9550 = 0x100E9550u;
constexpr uint32_t FUN_100eb0a0 = 0x100EB0A0u;
constexpr uint32_t FUN_100eb490 = 0x100EB490u;
constexpr uint32_t FUN_100eb8d0 = 0x100EB8D0u;
constexpr uint32_t FUN_100eb940 = 0x100EB940u;
constexpr uint32_t FUN_100ebf40 = 0x100EBF40u;
constexpr uint32_t FUN_100ebf80 = 0x100EBF80u;
constexpr uint32_t FUN_100ec060 = 0x100EC060u;
constexpr uint32_t FUN_100edea0 = 0x100EDEA0u;
constexpr uint32_t FUN_100edeb0 = 0x100EDEB0u;
constexpr uint32_t FUN_100ef1a0 = 0x100EF1A0u;
constexpr uint32_t FUN_100efc70 = 0x100EFC70u;
constexpr uint32_t FUN_100efca0 = 0x100EFCA0u;
constexpr uint32_t FUN_100efce0 = 0x100EFCE0u;
constexpr uint32_t FUN_100efdf0 = 0x100EFDF0u;
constexpr uint32_t FUN_100f0090 = 0x100F0090u;
constexpr uint32_t FUN_100f0130 = 0x100F0130u;
constexpr uint32_t FUN_100f02a0 = 0x100F02A0u;
constexpr uint32_t FUN_100f04f0 = 0x100F04F0u;
constexpr uint32_t FUN_100f0550 = 0x100F0550u;
constexpr uint32_t FUN_100f06b0 = 0x100F06B0u;
constexpr uint32_t FUN_100f09c0 = 0x100F09C0u;
constexpr uint32_t FUN_100f0c50 = 0x100F0C50u;
constexpr uint32_t FUN_100f10c0 = 0x100F10C0u;
constexpr uint32_t FUN_100f10f0 = 0x100F10F0u;
constexpr uint32_t FUN_100f1160 = 0x100F1160u;
constexpr uint32_t FUN_100f1280 = 0x100F1280u;
constexpr uint32_t FUN_100f1300 = 0x100F1300u;
constexpr uint32_t FUN_100f1340 = 0x100F1340u;
constexpr uint32_t FUN_100f1410 = 0x100F1410u;
constexpr uint32_t thunk_FUN_1030ff33 = 0x100F1770u;
constexpr uint32_t thunk_FUN_1030ff49 = 0x100F1780u;
constexpr uint32_t FUN_100f1870 = 0x100F1870u;
constexpr uint32_t FUN_100f1900 = 0x100F1900u;
constexpr uint32_t FUN_100f1b20 = 0x100F1B20u;
constexpr uint32_t FUN_100f1ba0 = 0x100F1BA0u;
constexpr uint32_t FUN_100f1c40 = 0x100F1C40u;
constexpr uint32_t FUN_100f1d20 = 0x100F1D20u;
constexpr uint32_t FUN_100f1e70 = 0x100F1E70u;
constexpr uint32_t FUN_100f1f00 = 0x100F1F00u;
constexpr uint32_t FUN_100f25f0 = 0x100F25F0u;
constexpr uint32_t FUN_100f2690 = 0x100F2690u;
constexpr uint32_t FUN_100f2720 = 0x100F2720u;
constexpr uint32_t FUN_100f29e0 = 0x100F29E0u;
constexpr uint32_t FUN_100f2a40 = 0x100F2A40u;
constexpr uint32_t FUN_100f2a70 = 0x100F2A70u;
constexpr uint32_t FUN_100f2b80 = 0x100F2B80u;
constexpr uint32_t FUN_100f2c40 = 0x100F2C40u;
constexpr uint32_t FUN_100f2c80 = 0x100F2C80u;
constexpr uint32_t FUN_100f3010 = 0x100F3010u;
constexpr uint32_t FUN_100f32b0 = 0x100F32B0u;
constexpr uint32_t FUN_100f3bb0 = 0x100F3BB0u;
constexpr uint32_t FUN_100f3c20 = 0x100F3C20u;
constexpr uint32_t FUN_100f3c90 = 0x100F3C90u;
constexpr uint32_t FUN_100f6f00 = 0x100F6F00u;
constexpr uint32_t FUN_100f6f30 = 0x100F6F30u;
constexpr uint32_t FUN_100f6f90 = 0x100F6F90u;
constexpr uint32_t FUN_100f76b0 = 0x100F76B0u;
constexpr uint32_t FUN_100f7700 = 0x100F7700u;
constexpr uint32_t FUN_100f8920 = 0x100F8920u;
constexpr uint32_t FUN_100f8cb0 = 0x100F8CB0u;
constexpr uint32_t FUN_100f9350 = 0x100F9350u;
constexpr uint32_t FUN_100f9cc0 = 0x100F9CC0u;
constexpr uint32_t FUN_100f9d50 = 0x100F9D50u;
constexpr uint32_t FUN_100f9da0 = 0x100F9DA0u;
constexpr uint32_t FUN_100f9db0 = 0x100F9DB0u;
constexpr uint32_t FUN_100f9df0 = 0x100F9DF0u;
constexpr uint32_t FUN_100f9e10 = 0x100F9E10u;
constexpr uint32_t FUN_100fa180 = 0x100FA180u;
constexpr uint32_t FUN_100fa4a0 = 0x100FA4A0u;
constexpr uint32_t FUN_100fa940 = 0x100FA940u;
constexpr uint32_t FUN_100fc0f0 = 0x100FC0F0u;
constexpr uint32_t FUN_100fc140 = 0x100FC140u;
constexpr uint32_t FUN_100fc160 = 0x100FC160u;
constexpr uint32_t FUN_100fc180 = 0x100FC180u;
constexpr uint32_t FUN_100fc2b0 = 0x100FC2B0u;
constexpr uint32_t FUN_100fc2e0 = 0x100FC2E0u;
constexpr uint32_t FUN_100fc300 = 0x100FC300u;
constexpr uint32_t FUN_100fc320 = 0x100FC320u;
constexpr uint32_t thunk_FUN_100fc370 = 0x100FC3B0u;
constexpr uint32_t FUN_100fc3c0 = 0x100FC3C0u;
constexpr uint32_t thunk_FUN_100ddd20 = 0x100FC4D0u;
constexpr uint32_t FUN_100fc510 = 0x100FC510u;
constexpr uint32_t FUN_100fc680 = 0x100FC680u;
constexpr uint32_t FUN_100fca20 = 0x100FCA20u;
constexpr uint32_t FUN_100fca40 = 0x100FCA40u;
constexpr uint32_t FUN_100fca60 = 0x100FCA60u;
constexpr uint32_t FUN_100fcd70 = 0x100FCD70u;
constexpr uint32_t FUN_10101bc0 = 0x10101BC0u;
constexpr uint32_t FUN_10101bf0 = 0x10101BF0u;
constexpr uint32_t ntGameInit = 0x10102340u;
constexpr uint32_t FUN_10103720 = 0x10103720u;
constexpr uint32_t FUN_10103740 = 0x10103740u;
constexpr uint32_t sub_101037F0 = 0x101037F0u;
constexpr uint32_t FUN_101038c0 = 0x101038C0u;
constexpr uint32_t FUN_101038d0 = 0x101038D0u;
constexpr uint32_t FUN_101038e0 = 0x101038E0u;
constexpr uint32_t ntGameTimeGet = 0x101039E0u;
constexpr uint32_t FUN_10103ae0 = 0x10103AE0u;
constexpr uint32_t FUN_10103b10 = 0x10103B10u;
constexpr uint32_t FUN_10103b30 = 0x10103B30u;
constexpr uint32_t FUN_10103b60 = 0x10103B60u;
constexpr uint32_t FUN_10103f50 = 0x10103F50u;
constexpr uint32_t FUN_10107610 = 0x10107610u;
constexpr uint32_t FUN_10108150 = 0x10108150u;
constexpr uint32_t FUN_10108300 = 0x10108300u;
constexpr uint32_t FUN_101083c0 = 0x101083C0u;
constexpr uint32_t FUN_10108460 = 0x10108460u;
constexpr uint32_t FUN_10108590 = 0x10108590u;
constexpr uint32_t FUN_101085c0 = 0x101085C0u;
constexpr uint32_t FUN_101085e0 = 0x101085E0u;
constexpr uint32_t thunk_FUN_10108600 = 0x101086B0u;
constexpr uint32_t FUN_10108700 = 0x10108700u;
constexpr uint32_t FUN_10108780 = 0x10108780u;
constexpr uint32_t FUN_10108870 = 0x10108870u;
constexpr uint32_t FUN_101088a0 = 0x101088A0u;
constexpr uint32_t FUN_101088c0 = 0x101088C0u;
constexpr uint32_t FUN_10108d80 = 0x10108D80u;
constexpr uint32_t FUN_10108e80 = 0x10108E80u;
constexpr uint32_t FUN_10108f40 = 0x10108F40u;
constexpr uint32_t FUN_10109000 = 0x10109000u;
constexpr uint32_t FUN_101090a0 = 0x101090A0u;
constexpr uint32_t FUN_10109100 = 0x10109100u;
constexpr uint32_t FUN_10109140 = 0x10109140u;
constexpr uint32_t FUN_101091d0 = 0x101091D0u;
constexpr uint32_t FUN_101092c0 = 0x101092C0u;
constexpr uint32_t FUN_101094d0 = 0x101094D0u;
constexpr uint32_t FUN_1010e040 = 0x1010E040u;
constexpr uint32_t FUN_1010f8b0 = 0x1010F8B0u;
constexpr uint32_t FUN_1010fa30 = 0x1010FA30u;
constexpr uint32_t FUN_1010fbd0 = 0x1010FBD0u;
constexpr uint32_t FUN_1010fc50 = 0x1010FC50u;
constexpr uint32_t FUN_1010feb0 = 0x1010FEB0u;
constexpr uint32_t FUN_1010feb0_v1 = 0x1010FEB0u;
constexpr uint32_t FUN_1010fee0 = 0x1010FEE0u;
constexpr uint32_t FUN_10110040 = 0x10110040u;
constexpr uint32_t FUN_10113220 = 0x10113220u;
constexpr uint32_t FUN_101133a0 = 0x101133A0u;
constexpr uint32_t FUN_10113ca0 = 0x10113CA0u;
constexpr uint32_t FDtXiErrorReportCtrl_Constructor = 0x10113D30u;
constexpr uint32_t FUN_10114070 = 0x10114070u;
constexpr uint32_t FUN_10114120 = 0x10114120u;
constexpr uint32_t FUN_10114250 = 0x10114250u;
constexpr uint32_t FUN_10114320 = 0x10114320u;
constexpr uint32_t FUN_10114500 = 0x10114500u;
constexpr uint32_t FUN_10114f10 = 0x10114F10u;
constexpr uint32_t FDtGmNoticeProc = 0x10115020u;
constexpr uint32_t FUN_10115250 = 0x10115250u;
constexpr uint32_t FUN_101152f0 = 0x101152F0u;
constexpr uint32_t FUN_101154d0 = 0x101154D0u;
constexpr uint32_t FUN_10115520 = 0x10115520u;
constexpr uint32_t FUN_10115580 = 0x10115580u;
constexpr uint32_t FUN_101164b0 = 0x101164B0u;
constexpr uint32_t FUN_10116540 = 0x10116540u;
constexpr uint32_t FUN_10116610 = 0x10116610u;
constexpr uint32_t FUN_10116610_v1 = 0x10116610u;
constexpr uint32_t FUN_101170d0 = 0x101170D0u;
constexpr uint32_t FUN_10117460 = 0x10117460u;
constexpr uint32_t FUN_101177d0 = 0x101177D0u;
constexpr uint32_t FUN_10117f30 = 0x10117F30u;
constexpr uint32_t FUN_101180f0 = 0x101180F0u;
constexpr uint32_t FUN_10118170 = 0x10118170u;
constexpr uint32_t FUN_101181d0 = 0x101181D0u;
constexpr uint32_t FUN_10118220 = 0x10118220u;
constexpr uint32_t FUN_10118260 = 0x10118260u;
constexpr uint32_t FUN_101186a0 = 0x101186A0u;
constexpr uint32_t FUN_101188e0 = 0x101188E0u;
constexpr uint32_t CTkMenuCtrlData_GetButtonLocate = 0x10118900u;
constexpr uint32_t FUN_10118b60 = 0x10118B60u;
constexpr uint32_t FUN_10118c10 = 0x10118C10u;
constexpr uint32_t FUN_10118cb0 = 0x10118CB0u;
constexpr uint32_t FUN_10118db0 = 0x10118DB0u;
constexpr uint32_t FUN_10118e90 = 0x10118E90u;
constexpr uint32_t FUN_10118f40 = 0x10118F40u;
constexpr uint32_t CTkMenuCtrlData_GetWindowLocate = 0x10118F70u;
constexpr uint32_t FUN_10119110 = 0x10119110u;
constexpr uint32_t FUN_10119200 = 0x10119200u;
constexpr uint32_t FUN_101192e0 = 0x101192E0u;
constexpr uint32_t FUN_10119300 = 0x10119300u;
constexpr uint32_t FUN_10119360 = 0x10119360u;
constexpr uint32_t FUN_10119390 = 0x10119390u;
constexpr uint32_t FUN_101193d0 = 0x101193D0u;
constexpr uint32_t FUN_101193f0 = 0x101193F0u;
constexpr uint32_t FUN_10119550 = 0x10119550u;
constexpr uint32_t FUN_10119870 = 0x10119870u;
constexpr uint32_t FUN_101198d0 = 0x101198D0u;
constexpr uint32_t FUN_10119970 = 0x10119970u;
constexpr uint32_t FUN_10119970_v1 = 0x10119970u;
constexpr uint32_t FUN_10119a50 = 0x10119A50u;
constexpr uint32_t FUN_10119ad0 = 0x10119AD0u;
constexpr uint32_t FUN_10119bc0 = 0x10119BC0u;
constexpr uint32_t FUN_10119c00 = 0x10119C00u;
constexpr uint32_t FUN_10119da0 = 0x10119DA0u;
constexpr uint32_t FUN_10119e20 = 0x10119E20u;
constexpr uint32_t FUN_10119e30 = 0x10119E30u;
constexpr uint32_t FUN_10119e70 = 0x10119E70u;
constexpr uint32_t FUN_10119f20 = 0x10119F20u;
constexpr uint32_t FUN_10119fb0 = 0x10119FB0u;
constexpr uint32_t FUN_1011a490 = 0x1011A490u;
constexpr uint32_t FUN_1011a560 = 0x1011A560u;
constexpr uint32_t FUN_1011a5a0 = 0x1011A5A0u;
constexpr uint32_t FUN_1011abf0 = 0x1011ABF0u;
constexpr uint32_t sub_1011AC70 = 0x1011AC70u;
constexpr uint32_t FUN_1011ae80 = 0x1011AE80u;
constexpr uint32_t FUN_1011b2e0 = 0x1011B2E0u;
constexpr uint32_t FUN_1011b310 = 0x1011B310u;
constexpr uint32_t FUN_1011b320 = 0x1011B320u;
constexpr uint32_t FUN_1011b330 = 0x1011B330u;
constexpr uint32_t FUN_1011ba40 = 0x1011BA40u;
constexpr uint32_t sub_1011BA90 = 0x1011BA90u;
constexpr uint32_t Font_BindGlyphTexture = 0x1011BAE0u;
constexpr uint32_t Font_BindGlyphTexture_v1 = 0x1011BAE0u;
constexpr uint32_t FUN_1011bba0 = 0x1011BBA0u;
constexpr uint32_t sub_1011BBA0 = 0x1011BBA0u;
constexpr uint32_t Font_DrawGlyphQuad = 0x1011BBD0u;
constexpr uint32_t Gfx_SetAdditiveBlend = 0x1011BDC0u;
constexpr uint32_t Gfx_SetAdditiveBlend_v1 = 0x1011BDC0u;
constexpr uint32_t Font_DrawChar = 0x1011BF40u;
constexpr uint32_t FUN_1011bfd0 = 0x1011BFD0u;
constexpr uint32_t Obj_104E1B90_DrawString = 0x1011C080u;
constexpr uint32_t Gfx_SetCellBlendMode = 0x1011C1F0u;
constexpr uint32_t Font_AllocHighResImage = 0x1011C5D0u;
constexpr uint32_t Tex_LoadSurfaceFromDatFile = 0x1011C670u;
constexpr uint32_t Font_LoadHighResTexture = 0x1011C720u;
constexpr uint32_t Font_ReleaseHighResTexture = 0x1011C810u;
constexpr uint32_t Font_IsHighResTextureLoaded = 0x1011C850u;
constexpr uint32_t unknown_libname_88 = 0x1011C870u;
constexpr uint32_t sub_1011C8B0 = 0x1011C8B0u;
constexpr uint32_t FUN_1011c900 = 0x1011C900u;
constexpr uint32_t FUN_1011c940 = 0x1011C940u;
constexpr uint32_t Font_DecodeNextChar = 0x1011CE80u;
constexpr uint32_t Font_DecodeNextChar_v1 = 0x1011CE80u;
constexpr uint32_t FUN_1011cec0 = 0x1011CEC0u;
constexpr uint32_t FUN_1011cf70 = 0x1011CF70u;
constexpr uint32_t FUN_1011cf70_v1 = 0x1011CF70u;
constexpr uint32_t FUN_1011d000 = 0x1011D000u;
constexpr uint32_t sub_1011D050 = 0x1011D050u;
constexpr uint32_t FUN_1011d290 = 0x1011D290u;
constexpr uint32_t Font_AddGlyphAdvance = 0x1011D840u;
constexpr uint32_t Font_AddGlyphAdvance_v1 = 0x1011D840u;
constexpr uint32_t FUN_1011d8b0 = 0x1011D8B0u;
constexpr uint32_t FUN_1011d990 = 0x1011D990u;
constexpr uint32_t FUN_1011da30 = 0x1011DA30u;
constexpr uint32_t Sjis_GetCharByteLen = 0x1011DA80u;
constexpr uint32_t Sjis_GetCharByteLen_v1 = 0x1011DA80u;
constexpr uint32_t FUN_1011daa0 = 0x1011DAA0u;
constexpr uint32_t FUN_1011db40 = 0x1011DB40u;
constexpr uint32_t FUN_1011dcc0 = 0x1011DCC0u;
constexpr uint32_t FUN_1011de80 = 0x1011DE80u;
constexpr uint32_t FUN_1011e290 = 0x1011E290u;
constexpr uint32_t FUN_1011e2a0 = 0x1011E2A0u;
constexpr uint32_t FUN_1011e6d0 = 0x1011E6D0u;
constexpr uint32_t Cell_DrawStretchedWrapped = 0x1011EF60u;
constexpr uint32_t Cell_DrawWithDefaultBlend = 0x1011F2D0u;
constexpr uint32_t Cell_DrawQuadBlended = 0x1011F300u;
constexpr uint32_t Cell_DrawQuadBlended_v1 = 0x1011F300u;
constexpr uint32_t FUN_1011fa80 = 0x1011FA80u;
constexpr uint32_t FUN_1011fb00 = 0x1011FB00u;
constexpr uint32_t FUN_1011fb60 = 0x1011FB60u;
constexpr uint32_t sub_1011FB60 = 0x1011FB60u;
constexpr uint32_t Shape_DrawScaled = 0x1011FC80u;
constexpr uint32_t Shape_DrawScaled_v1 = 0x1011FC80u;
constexpr uint32_t FUN_10120000 = 0x10120000u;
constexpr uint32_t FUN_10120070 = 0x10120070u;
constexpr uint32_t Color_ModulateArgb128 = 0x101200B0u;
constexpr uint32_t ShapeMng_Init = 0x101201C0u;
constexpr uint32_t sub_10120200 = 0x10120200u;
constexpr uint32_t ShapeMng_FindShape = 0x10120450u;
constexpr uint32_t ShapeMng_FindShape_v1 = 0x10120450u;
constexpr uint32_t ShapeMng_FindShape_v2 = 0x10120450u;
constexpr uint32_t FUN_101204b0 = 0x101204B0u;
constexpr uint32_t ShapeMng_GetFontGlyph = 0x10120510u;
constexpr uint32_t ShapeMng_BindFontTexture = 0x10120590u;
constexpr uint32_t ShapeMng_LoadFontTextures = 0x10120660u;
constexpr uint32_t ShapeMng_LoadDmgNumTextures = 0x10120880u;
constexpr uint32_t FUN_10120950 = 0x10120950u;
constexpr uint32_t FUN_10120b80 = 0x10120B80u;
constexpr uint32_t FUN_10120bb0 = 0x10120BB0u;
constexpr uint32_t FUN_10120be0 = 0x10120BE0u;
constexpr uint32_t FUN_10122e30 = 0x10122E30u;
constexpr uint32_t FUN_10123350 = 0x10123350u;
constexpr uint32_t FUN_10123380 = 0x10123380u;
constexpr uint32_t FUN_10123670 = 0x10123670u;
constexpr uint32_t GetAnalogKey = 0x10123970u;
constexpr uint32_t FUN_10123a70 = 0x10123A70u;
constexpr uint32_t FUN_10123e50 = 0x10123E50u;
constexpr uint32_t FUN_10123e60 = 0x10123E60u;
constexpr uint32_t sub_10123E90 = 0x10123E90u;
constexpr uint32_t FUN_10123ea0 = 0x10123EA0u;
constexpr uint32_t FUN_10124010 = 0x10124010u;
constexpr uint32_t FUN_10124050 = 0x10124050u;
constexpr uint32_t FUN_10124070 = 0x10124070u;
constexpr uint32_t FUN_10124090 = 0x10124090u;
constexpr uint32_t FUN_101240b0 = 0x101240B0u;
constexpr uint32_t FUN_101240d0 = 0x101240D0u;
constexpr uint32_t FUN_101240f0 = 0x101240F0u;
constexpr uint32_t FUN_10124110 = 0x10124110u;
constexpr uint32_t FUN_10124130 = 0x10124130u;
constexpr uint32_t Cursor_GetHwCursorSlot = 0x101245C0u;
constexpr uint32_t CTkMouse_OnMouseMove = 0x10124640u;
constexpr uint32_t Cursor_SetPosition = 0x10124700u;
constexpr uint32_t Cursor_SetPosition_v1 = 0x10124700u;
constexpr uint32_t sub_101248A0 = 0x101248A0u;
constexpr uint32_t FUN_10124930 = 0x10124930u;
constexpr uint32_t FUN_101249e0 = 0x101249E0u;
constexpr uint32_t FUN_10124a00 = 0x10124A00u;
constexpr uint32_t FUN_10124ab0 = 0x10124AB0u;
constexpr uint32_t FUN_10124ad0 = 0x10124AD0u;
constexpr uint32_t FUN_10124b50 = 0x10124B50u;
constexpr uint32_t FUN_10124b70 = 0x10124B70u;
constexpr uint32_t FUN_10124bc0 = 0x10124BC0u;
constexpr uint32_t Cursor_Draw = 0x10124EB0u;
constexpr uint32_t FUN_101252c0 = 0x101252C0u;
constexpr uint32_t Cursor_SetVisible = 0x101252F0u;
constexpr uint32_t Cursor_SetVisible_v1 = 0x101252F0u;
constexpr uint32_t FUN_101260d0 = 0x101260D0u;
constexpr uint32_t Cursor_StoreScreenCenter = 0x101262A0u;
constexpr uint32_t Cursor_StoreScreenCenter_v1 = 0x101262A0u;
constexpr uint32_t FUN_10126300 = 0x10126300u;
constexpr uint32_t FUN_10126320 = 0x10126320u;
constexpr uint32_t FUN_10126320_v1 = 0x10126320u;
constexpr uint32_t FUN_10126330 = 0x10126330u;
constexpr uint32_t FUN_10126350 = 0x10126350u;
constexpr uint32_t FUN_10126370 = 0x10126370u;
constexpr uint32_t FUN_101263b0 = 0x101263B0u;
constexpr uint32_t FUN_10126410 = 0x10126410u;
constexpr uint32_t FUN_10126480 = 0x10126480u;
constexpr uint32_t FUN_10126480_v1 = 0x10126480u;
constexpr uint32_t FUN_10126500 = 0x10126500u;
constexpr uint32_t FUN_10126510 = 0x10126510u;
constexpr uint32_t FUN_10126520 = 0x10126520u;
constexpr uint32_t FUN_10126550 = 0x10126550u;
constexpr uint32_t FUN_10126580 = 0x10126580u;
constexpr uint32_t FUN_101265d0 = 0x101265D0u;
constexpr uint32_t FUN_10126700 = 0x10126700u;
constexpr uint32_t FUN_10126790 = 0x10126790u;
constexpr uint32_t FUN_10126b60 = 0x10126B60u;
constexpr uint32_t FUN_10126bc0 = 0x10126BC0u;
constexpr uint32_t FUN_10126c60 = 0x10126C60u;
constexpr uint32_t sub_10127190 = 0x10127190u;
constexpr uint32_t FUN_101271c0 = 0x101271C0u;
constexpr uint32_t FUN_10127250 = 0x10127250u;
constexpr uint32_t FUN_101278b0 = 0x101278B0u;
constexpr uint32_t FUN_101278d0 = 0x101278D0u;
constexpr uint32_t FUN_10127930 = 0x10127930u;
constexpr uint32_t FUN_10127970 = 0x10127970u;
constexpr uint32_t ChatLog_AddMessage = 0x10127C00u;
constexpr uint32_t ChatLog_AddMessage_v1 = 0x10127C00u;
constexpr uint32_t FUN_10127ea0 = 0x10127EA0u;
constexpr uint32_t sub_10127EE0 = 0x10127EE0u;
constexpr uint32_t FUN_10127f00 = 0x10127F00u;
constexpr uint32_t FUN_101284f0 = 0x101284F0u;
constexpr uint32_t FUN_10128680 = 0x10128680u;
constexpr uint32_t FUN_10128830 = 0x10128830u;
constexpr uint32_t FUN_10128950 = 0x10128950u;
constexpr uint32_t FUN_10129290 = 0x10129290u;
constexpr uint32_t FUN_101292e0 = 0x101292E0u;
constexpr uint32_t FUN_101295d0 = 0x101295D0u;
constexpr uint32_t FUN_10129c60 = 0x10129C60u;
constexpr uint32_t FUN_10129cd0 = 0x10129CD0u;
constexpr uint32_t FUN_10129d00 = 0x10129D00u;
constexpr uint32_t FUN_10129d90 = 0x10129D90u;
constexpr uint32_t FUN_10129de0 = 0x10129DE0u;
constexpr uint32_t FUN_10129e30 = 0x10129E30u;
constexpr uint32_t FUN_1012a120 = 0x1012A120u;
constexpr uint32_t FUN_1012a3b0 = 0x1012A3B0u;
constexpr uint32_t FUN_1012ca90 = 0x1012CA90u;
constexpr uint32_t FUN_1012cad0 = 0x1012CAD0u;
constexpr uint32_t FUN_10130f90 = 0x10130F90u;
constexpr uint32_t FUN_101312e0 = 0x101312E0u;
constexpr uint32_t FUN_10131530 = 0x10131530u;
constexpr uint32_t FUN_10136380 = 0x10136380u;
constexpr uint32_t FUN_10136670 = 0x10136670u;
constexpr uint32_t FEMultiLineText__FEMultiLineText_0 = 0x10136A30u;
constexpr uint32_t FUN_10136f60 = 0x10136F60u;
constexpr uint32_t FUN_10138ec0 = 0x10138EC0u;
constexpr uint32_t thunk_FUN_1013a100 = 0x10139FE0u;
constexpr uint32_t FUN_1013a010 = 0x1013A010u;
constexpr uint32_t FUN_1013a030 = 0x1013A030u;
constexpr uint32_t FUN_1013a060 = 0x1013A060u;
constexpr uint32_t FUN_1013a070 = 0x1013A070u;
constexpr uint32_t FUN_1013a0d0 = 0x1013A0D0u;
constexpr uint32_t FUN_1013a100 = 0x1013A100u;
constexpr uint32_t FUN_1013a100_v1 = 0x1013A100u;
constexpr uint32_t FUN_1013a320 = 0x1013A320u;
constexpr uint32_t sub_1013A320 = 0x1013A320u;
constexpr uint32_t FUN_1013a470 = 0x1013A470u;
constexpr uint32_t FUN_1013be50 = 0x1013BE50u;
constexpr uint32_t FUN_1013bf40 = 0x1013BF40u;
constexpr uint32_t FUN_10144f40 = 0x10144F40u;
constexpr uint32_t FUN_10146930 = 0x10146930u;
constexpr uint32_t FUN_10146960 = 0x10146960u;
constexpr uint32_t FUN_101475b0 = 0x101475B0u;
constexpr uint32_t FUN_10148430 = 0x10148430u;
constexpr uint32_t FUN_10148640 = 0x10148640u;
constexpr uint32_t FUN_10148ed0 = 0x10148ED0u;
constexpr uint32_t FUN_1014a170 = 0x1014A170u;
constexpr uint32_t FUN_1014a7d0 = 0x1014A7D0u;
constexpr uint32_t FUN_1014b670 = 0x1014B670u;
constexpr uint32_t FUN_1014ba60 = 0x1014BA60u;
constexpr uint32_t FUN_1014e020 = 0x1014E020u;
constexpr uint32_t FUN_1014e050 = 0x1014E050u;
constexpr uint32_t FUN_1014ff70 = 0x1014FF70u;
constexpr uint32_t FUN_10150a30 = 0x10150A30u;
constexpr uint32_t FUN_10150a60 = 0x10150A60u;
constexpr uint32_t sub_10150CC0 = 0x10150CC0u;
constexpr uint32_t thunk_FUN_10150ce0 = 0x10150CC0u;
constexpr uint32_t FUN_10150ce0 = 0x10150CE0u;
constexpr uint32_t FUN_10150d60 = 0x10150D60u;
constexpr uint32_t FUN_10151140 = 0x10151140u;
constexpr uint32_t FUN_10152f20 = 0x10152F20u;
constexpr uint32_t sub_10152F80 = 0x10152F80u;
constexpr uint32_t FUN_10152fa0 = 0x10152FA0u;
constexpr uint32_t FUN_10152fc0 = 0x10152FC0u;
constexpr uint32_t FUN_10152ff0 = 0x10152FF0u;
constexpr uint32_t FUN_101530c0 = 0x101530C0u;
constexpr uint32_t FUN_101530c0_v1 = 0x101530C0u;
constexpr uint32_t FUN_101531b0 = 0x101531B0u;
constexpr uint32_t FUN_10153a40 = 0x10153A40u;
constexpr uint32_t FUN_10153af0 = 0x10153AF0u;
constexpr uint32_t FUN_10153b30 = 0x10153B30u;
constexpr uint32_t FUN_10153b70 = 0x10153B70u;
constexpr uint32_t sub_10153C10 = 0x10153C10u;
constexpr uint32_t FUN_10153ef0 = 0x10153EF0u;
constexpr uint32_t FUN_10153ff0 = 0x10153FF0u;
constexpr uint32_t sub_10153FF0 = 0x10153FF0u;
constexpr uint32_t FUN_10154070 = 0x10154070u;
constexpr uint32_t FUN_101542b0 = 0x101542B0u;
constexpr uint32_t FUN_101544f0 = 0x101544F0u;
constexpr uint32_t FUN_10154550 = 0x10154550u;
constexpr uint32_t FUN_10154840 = 0x10154840u;
constexpr uint32_t sub_10154860 = 0x10154860u;
constexpr uint32_t FUN_101549e0 = 0x101549E0u;
constexpr uint32_t FUN_101549f0 = 0x101549F0u;
constexpr uint32_t FUN_10154ab0 = 0x10154AB0u;
constexpr uint32_t FUN_10154ad0 = 0x10154AD0u;
constexpr uint32_t FUN_10154f60 = 0x10154F60u;
constexpr uint32_t FUN_10154fc0 = 0x10154FC0u;
constexpr uint32_t FUN_101569a0 = 0x101569A0u;
constexpr uint32_t FUN_101569f0 = 0x101569F0u;
constexpr uint32_t FUN_10156be0 = 0x10156BE0u;
constexpr uint32_t FUN_10156bf0 = 0x10156BF0u;
constexpr uint32_t FUN_10156c30 = 0x10156C30u;
constexpr uint32_t thunk_FUN_10157620 = 0x10157610u;
constexpr uint32_t FUN_10157620 = 0x10157620u;
constexpr uint32_t FUN_10157620_v1 = 0x10157620u;
constexpr uint32_t FUN_10157660 = 0x10157660u;
constexpr uint32_t sub_10157660 = 0x10157660u;
constexpr uint32_t FUN_101576f0 = 0x101576F0u;
constexpr uint32_t FUN_10157780 = 0x10157780u;
constexpr uint32_t FUN_10157860 = 0x10157860u;
constexpr uint32_t FUN_10157880 = 0x10157880u;
constexpr uint32_t FUN_10157890 = 0x10157890u;
constexpr uint32_t FUN_101578a0 = 0x101578A0u;
constexpr uint32_t FUN_101578b0 = 0x101578B0u;
constexpr uint32_t FUN_101578c0 = 0x101578C0u;
constexpr uint32_t FUN_101578d0 = 0x101578D0u;
constexpr uint32_t FUN_101578e0 = 0x101578E0u;
constexpr uint32_t FUN_101578f0 = 0x101578F0u;
constexpr uint32_t FUN_101579b0 = 0x101579B0u;
constexpr uint32_t sub_101579C0 = 0x101579C0u;
constexpr uint32_t FUN_10157b40 = 0x10157B40u;
constexpr uint32_t FUN_10157c60 = 0x10157C60u;
constexpr uint32_t sub_10157CF0 = 0x10157CF0u;
constexpr uint32_t FUN_10157d20 = 0x10157D20u;
constexpr uint32_t FUN_10157d60 = 0x10157D60u;
constexpr uint32_t FUN_10157e20 = 0x10157E20u;
constexpr uint32_t FUN_10157fa0 = 0x10157FA0u;
constexpr uint32_t FUN_101581e0 = 0x101581E0u;
constexpr uint32_t FUN_101582f0 = 0x101582F0u;
constexpr uint32_t FUN_101583f0 = 0x101583F0u;
constexpr uint32_t FUN_10158470 = 0x10158470u;
constexpr uint32_t FUN_10158480 = 0x10158480u;
constexpr uint32_t FUN_10158510 = 0x10158510u;
constexpr uint32_t FUN_10158550 = 0x10158550u;
constexpr uint32_t FUN_10158590 = 0x10158590u;
constexpr uint32_t FUN_101585f0 = 0x101585F0u;
constexpr uint32_t FUN_101587b0 = 0x101587B0u;
constexpr uint32_t FUN_101587f0 = 0x101587F0u;
constexpr uint32_t FUN_10158aa0 = 0x10158AA0u;
constexpr uint32_t FUN_10158b00 = 0x10158B00u;
constexpr uint32_t FUN_10158b40 = 0x10158B40u;
constexpr uint32_t FUN_10158d90 = 0x10158D90u;
constexpr uint32_t FUN_10158db0 = 0x10158DB0u;
constexpr uint32_t FUN_10158ff0 = 0x10158FF0u;
constexpr uint32_t FUN_10159040 = 0x10159040u;
constexpr uint32_t FUN_101590a0 = 0x101590A0u;
constexpr uint32_t FUN_101590c0 = 0x101590C0u;
constexpr uint32_t FUN_10159170 = 0x10159170u;
constexpr uint32_t FUN_101591d0 = 0x101591D0u;
constexpr uint32_t FUN_10159370 = 0x10159370u;
constexpr uint32_t Actor_SetSlotTableEntry = 0x10159400u;
constexpr uint32_t Actor_SetSlotTableEntry_v1 = 0x10159400u;
constexpr uint32_t FUN_10159420 = 0x10159420u;
constexpr uint32_t FUN_10159440 = 0x10159440u;
constexpr uint32_t FUN_101594d0 = 0x101594D0u;
constexpr uint32_t FUN_101594e0 = 0x101594E0u;
constexpr uint32_t FUN_101594f0 = 0x101594F0u;
constexpr uint32_t FUN_10159530 = 0x10159530u;
constexpr uint32_t FUN_10159540 = 0x10159540u;
constexpr uint32_t FUN_101595d0 = 0x101595D0u;
constexpr uint32_t FUN_10159660 = 0x10159660u;
constexpr uint32_t FUN_10159740 = 0x10159740u;
constexpr uint32_t FUN_10159780 = 0x10159780u;
constexpr uint32_t FUN_101597c0 = 0x101597C0u;
constexpr uint32_t FUN_101597f0 = 0x101597F0u;
constexpr uint32_t FUN_10159830 = 0x10159830u;
constexpr uint32_t FUN_10159880 = 0x10159880u;
constexpr uint32_t FUN_101598a0 = 0x101598A0u;
constexpr uint32_t FUN_101598b0 = 0x101598B0u;
constexpr uint32_t FUN_101598c0 = 0x101598C0u;
constexpr uint32_t FUN_10159910 = 0x10159910u;
constexpr uint32_t FUN_10159970 = 0x10159970u;
constexpr uint32_t FUN_10159a20 = 0x10159A20u;
constexpr uint32_t FUN_10159b10 = 0x10159B10u;
constexpr uint32_t FUN_10159ba0 = 0x10159BA0u;
constexpr uint32_t FUN_10159c80 = 0x10159C80u;
constexpr uint32_t FUN_10159d40 = 0x10159D40u;
constexpr uint32_t FUN_10159de0 = 0x10159DE0u;
constexpr uint32_t FUN_10159e10 = 0x10159E10u;
constexpr uint32_t FUN_10159eb0 = 0x10159EB0u;
constexpr uint32_t FUN_1015afa0 = 0x1015AFA0u;
constexpr uint32_t FUN_1015b270 = 0x1015B270u;
constexpr uint32_t FUN_1015b410 = 0x1015B410u;
constexpr uint32_t FUN_1015b9e0 = 0x1015B9E0u;
constexpr uint32_t FUN_1015bc40 = 0x1015BC40u;
constexpr uint32_t FUN_1015d7f0 = 0x1015D7F0u;
constexpr uint32_t FUN_1015d860 = 0x1015D860u;
constexpr uint32_t MenuMng_Init = 0x1015D900u;
constexpr uint32_t FUN_1015d9a0 = 0x1015D9A0u;
constexpr uint32_t sub_1015D9E0 = 0x1015D9E0u;
constexpr uint32_t FUN_1015db80 = 0x1015DB80u;
constexpr uint32_t FUN_1015dbb0 = 0x1015DBB0u;
constexpr uint32_t FUN_1015dbd0 = 0x1015DBD0u;
constexpr uint32_t FUN_1015dc10 = 0x1015DC10u;
constexpr uint32_t FUN_1015dce0 = 0x1015DCE0u;
constexpr uint32_t FUN_1015dd00 = 0x1015DD00u;
constexpr uint32_t FUN_1015dd50 = 0x1015DD50u;
constexpr uint32_t FUN_1015dd70 = 0x1015DD70u;
constexpr uint32_t nullsub_49 = 0x1015DFC0u;
constexpr uint32_t MenuMng_OpenByName = 0x1015E350u;
constexpr uint32_t MenuMng_OpenByName_v1 = 0x1015E350u;
constexpr uint32_t MenuMng_OpenByName_v2 = 0x1015E350u;
constexpr uint32_t CTkMenuMng_DestroyDrawMenu = 0x1015E790u;
constexpr uint32_t FUN_1015e830 = 0x1015E830u;
constexpr uint32_t FUN_1015e850 = 0x1015E850u;
constexpr uint32_t FUN_1015e850_v1 = 0x1015E850u;
constexpr uint32_t FUN_1015e8a0 = 0x1015E8A0u;
constexpr uint32_t FUN_1015e910 = 0x1015E910u;
constexpr uint32_t FUN_1015eb60 = 0x1015EB60u;
constexpr uint32_t FUN_1015ec50 = 0x1015EC50u;
constexpr uint32_t FUN_1015ecc0 = 0x1015ECC0u;
constexpr uint32_t FUN_1015ed80 = 0x1015ED80u;
constexpr uint32_t FUN_1015edb0 = 0x1015EDB0u;
constexpr uint32_t FUN_1015ee00 = 0x1015EE00u;
constexpr uint32_t FUN_1015ee40 = 0x1015EE40u;
constexpr uint32_t FUN_1015eea0 = 0x1015EEA0u;
constexpr uint32_t FUN_1015ef20 = 0x1015EF20u;
constexpr uint32_t FUN_1015f3e0 = 0x1015F3E0u;
constexpr uint32_t sub_1015F6A0 = 0x1015F6A0u;
constexpr uint32_t FUN_1015f980 = 0x1015F980u;
constexpr uint32_t FUN_1015f9e0 = 0x1015F9E0u;
constexpr uint32_t sub_1015FA00 = 0x1015FA00u;
constexpr uint32_t FUN_101601d0 = 0x101601D0u;
constexpr uint32_t FUN_101601d0_v1 = 0x101601D0u;
constexpr uint32_t MenuMng_GetUiScale = 0x10160250u;
constexpr uint32_t MenuMng_GetOffsetX = 0x10160290u;
constexpr uint32_t MenuMng_GetOffsetX_v1 = 0x10160290u;
constexpr uint32_t MenuMng_GetOffsetY = 0x101602C0u;
constexpr uint32_t CTkMenuMng_IsDrawWindow = 0x101602F0u;
constexpr uint32_t sub_10160440 = 0x10160440u;
constexpr uint32_t Gfx_SetDefault2DStates = 0x101604B0u;
constexpr uint32_t Gfx_SetDefault2DStates_v1 = 0x101604B0u;
constexpr uint32_t Gfx_DisableFogAndAlphaBlend = 0x10160650u;
constexpr uint32_t Gfx_DisableFogAndAlphaBlend_v1 = 0x10160650u;
constexpr uint32_t FUN_101606f0 = 0x101606F0u;
constexpr uint32_t sub_101606F0 = 0x101606F0u;
constexpr uint32_t FUN_10160700 = 0x10160700u;
constexpr uint32_t FUN_10160760 = 0x10160760u;
constexpr uint32_t FUN_10160930 = 0x10160930u;
constexpr uint32_t FUN_101609c0 = 0x101609C0u;
constexpr uint32_t FUN_10160a50 = 0x10160A50u;
constexpr uint32_t FUN_10160c90 = 0x10160C90u;
constexpr uint32_t FUN_10161110 = 0x10161110u;
constexpr uint32_t FUN_10161380 = 0x10161380u;
constexpr uint32_t FUN_101615e0 = 0x101615E0u;
constexpr uint32_t FUN_10161ed0 = 0x10161ED0u;
constexpr uint32_t FUN_10162510 = 0x10162510u;
constexpr uint32_t FUN_101626e0 = 0x101626E0u;
constexpr uint32_t FUN_10162f30 = 0x10162F30u;
constexpr uint32_t FUN_10162fb0 = 0x10162FB0u;
constexpr uint32_t FUN_10162fe0 = 0x10162FE0u;
constexpr uint32_t Obj_10621944_Draw = 0x101630E0u;
constexpr uint32_t FUN_10163350 = 0x10163350u;
constexpr uint32_t FUN_10163380 = 0x10163380u;
constexpr uint32_t Obj_10621944_DrawBgRect = 0x10163390u;
constexpr uint32_t FUN_101633d0 = 0x101633D0u;
constexpr uint32_t Gfx_DrawSolidRect2D = 0x10163400u;
constexpr uint32_t sub_10165070 = 0x10165070u;
constexpr uint32_t sub_101671D0 = 0x101671D0u;
constexpr uint32_t FUN_10167440 = 0x10167440u;
constexpr uint32_t Vt10336614_Destructor = 0x101674B0u;
constexpr uint32_t FUN_10167800 = 0x10167800u;
constexpr uint32_t Vt10336634_Destructor = 0x101678D0u;
constexpr uint32_t GetPtr8IfField60Set = 0x10167940u;
constexpr uint32_t FUN_10167950 = 0x10167950u;
constexpr uint32_t FUN_101679a0 = 0x101679A0u;
constexpr uint32_t Dbg_DrawWireMesh = 0x10168D20u;
constexpr uint32_t FUN_10168fd0 = 0x10168FD0u;
constexpr uint32_t FUN_10169290 = 0x10169290u;
constexpr uint32_t FUN_101693b0 = 0x101693B0u;
constexpr uint32_t FUN_10169b40 = 0x10169B40u;
constexpr uint32_t FUN_1016a2d0 = 0x1016A2D0u;
constexpr uint32_t FUN_1016a990 = 0x1016A990u;
constexpr uint32_t FUN_1016b1d0 = 0x1016B1D0u;
constexpr uint32_t Mat44_Multiply = 0x1016C060u;
constexpr uint32_t FUN_1016c150 = 0x1016C150u;
constexpr uint32_t FUN_1016c390 = 0x1016C390u;
constexpr uint32_t FUN_1016c5d0 = 0x1016C5D0u;
constexpr uint32_t FUN_1016e7e0 = 0x1016E7E0u;
constexpr uint32_t FUN_1016f320 = 0x1016F320u;
constexpr uint32_t FUN_1016fc40 = 0x1016FC40u;
constexpr uint32_t Vt10336668_Destructor = 0x1016FC80u;
constexpr uint32_t FUN_1016fc90 = 0x1016FC90u;
constexpr uint32_t sub_10170B30 = 0x10170B30u;
constexpr uint32_t sub_10170C80 = 0x10170C80u;
constexpr uint32_t FUN_10170cd0 = 0x10170CD0u;
constexpr uint32_t sub_10170E80 = 0x10170E80u;
constexpr uint32_t FUN_10170f30 = 0x10170F30u;
constexpr uint32_t FUN_101718e0 = 0x101718E0u;
constexpr uint32_t Vt1033668C_Destructor = 0x10171920u;
constexpr uint32_t FUN_10173ae0 = 0x10173AE0u;
constexpr uint32_t FUN_10174770 = 0x10174770u;
constexpr uint32_t FUN_10174a60 = 0x10174A60u;
constexpr uint32_t Mat4_SetIdentity_2 = 0x10175740u;
constexpr uint32_t Mat4_MulTranslation = 0x10175780u;
constexpr uint32_t FUN_10176dd0 = 0x10176DD0u;
constexpr uint32_t CMoResource_VT_10336698_Destructor = 0x10176EC0u;
constexpr uint32_t CMoResource_VT_103366C4_Destructor = 0x10176FA0u;
constexpr uint32_t CMoResource_VT_103366F0_Destructor = 0x10177020u;
constexpr uint32_t Gfx_ShaderSrc_GetText = 0x10177340u;
constexpr uint32_t Gfx_ShaderSrc_GetTextLength = 0x10177350u;
constexpr uint32_t sub_101773F0 = 0x101773F0u;
constexpr uint32_t Gfx_SetZBias = 0x10177400u;
constexpr uint32_t ZoneMap_SetOverlayProjection = 0x10177420u;
constexpr uint32_t FUN_10177770 = 0x10177770u;
constexpr uint32_t CTsZoneMap_LogZoneMetrics = 0x10177800u;
constexpr uint32_t FUN_101779b0 = 0x101779B0u;
constexpr uint32_t XiZone_DrawMarkerForward = 0x10177CD0u;
constexpr uint32_t sub_10177D00 = 0x10177D00u;
constexpr uint32_t FUN_10177fa0 = 0x10177FA0u;
constexpr uint32_t CTsZoneMap_InitZBiasPresets = 0x101781D0u;
constexpr uint32_t FUN_10178250 = 0x10178250u;
constexpr uint32_t FUN_101784e0 = 0x101784E0u;
constexpr uint32_t FUN_10178530 = 0x10178530u;
constexpr uint32_t FUN_10178610 = 0x10178610u;
constexpr uint32_t ZoneMap_FindLight = 0x10178760u;
constexpr uint32_t FUN_10178790 = 0x10178790u;
constexpr uint32_t FUN_101787c0 = 0x101787C0u;
constexpr uint32_t FUN_10178ac0 = 0x10178AC0u;
constexpr uint32_t ZoneMap_ReleaseStateBlock = 0x10178B00u;
constexpr uint32_t FUN_10178d90 = 0x10178D90u;
constexpr uint32_t FUN_10178dd0 = 0x10178DD0u;
constexpr uint32_t FUN_10178f70 = 0x10178F70u;
constexpr uint32_t ZoneMap_CreateVertexShaders = 0x101792A0u;
constexpr uint32_t FUN_1017a630 = 0x1017A630u;
constexpr uint32_t FUN_1017a690 = 0x1017A690u;
constexpr uint32_t FUN_1017b110 = 0x1017B110u;
constexpr uint32_t ZoneMap_TeardownStep = 0x1017B1E0u;
constexpr uint32_t ZoneMap_TeardownStep_v1 = 0x1017B1E0u;
constexpr uint32_t XiZone_Draw = 0x1017B510u;
constexpr uint32_t FUN_10181210 = 0x10181210u;
constexpr uint32_t FUN_10181290 = 0x10181290u;
constexpr uint32_t FUN_10181300 = 0x10181300u;
constexpr uint32_t FUN_10181510 = 0x10181510u;
constexpr uint32_t FUN_10181590 = 0x10181590u;
constexpr uint32_t FUN_101819b0 = 0x101819B0u;
constexpr uint32_t sub_10181BB0 = 0x10181BB0u;
constexpr uint32_t CTsZoneMap_DrawProjectedTexture = 0x10182700u;
constexpr uint32_t FUN_10183370 = 0x10183370u;
constexpr uint32_t FUN_10183420 = 0x10183420u;
constexpr uint32_t XiZone_DrawModelAtMatrix = 0x10183490u;
constexpr uint32_t XiZone_DrawEffectElementModel = 0x10183760u;
constexpr uint32_t ZoneMap_DrawMeshById = 0x10183DA0u;
constexpr uint32_t ModelActor_Draw = 0x101844C0u;
constexpr uint32_t XiZone_DrawGroundShadowQuad = 0x10184C90u;
constexpr uint32_t CTsZoneMap_DrawProjectedShadow = 0x10184FE0u;
constexpr uint32_t XiZone_DrawDecalOnTriangles = 0x10185CA0u;
constexpr uint32_t FUN_10186d30 = 0x10186D30u;
constexpr uint32_t FUN_10186db0 = 0x10186DB0u;
constexpr uint32_t FUN_10186dd0 = 0x10186DD0u;
constexpr uint32_t FUN_10186e70 = 0x10186E70u;
constexpr uint32_t sub_10186E90 = 0x10186E90u;
constexpr uint32_t FUN_10186ea0 = 0x10186EA0u;
constexpr uint32_t sub_10186EB0 = 0x10186EB0u;
constexpr uint32_t FUN_10186ec0 = 0x10186EC0u;
constexpr uint32_t FUN_10186ee0 = 0x10186EE0u;
constexpr uint32_t FUN_10186f00 = 0x10186F00u;
constexpr uint32_t FUN_10186f10 = 0x10186F10u;
constexpr uint32_t Env_GetColorPtr84 = 0x10186F20u;
constexpr uint32_t Env_GetColorPtr84_v1 = 0x10186F20u;
constexpr uint32_t FUN_10186f30 = 0x10186F30u;
constexpr uint32_t FUN_10186f40 = 0x10186F40u;
constexpr uint32_t FUN_10186f50 = 0x10186F50u;
constexpr uint32_t FUN_10186f60 = 0x10186F60u;
constexpr uint32_t FUN_10186f70 = 0x10186F70u;
constexpr uint32_t FUN_10186f80 = 0x10186F80u;
constexpr uint32_t FUN_10186f90 = 0x10186F90u;
constexpr uint32_t Env_GetAmbientColorPtr = 0x10186FA0u;
constexpr uint32_t Env_GetAmbientColorPtr_v1 = 0x10186FA0u;
constexpr uint32_t FUN_10186fb0 = 0x10186FB0u;
constexpr uint32_t FUN_10186fc0 = 0x10186FC0u;
constexpr uint32_t FUN_10186fd0 = 0x10186FD0u;
constexpr uint32_t FUN_10186fe0 = 0x10186FE0u;
constexpr uint32_t sub_10186FE0 = 0x10186FE0u;
constexpr uint32_t FUN_10186ff0 = 0x10186FF0u;
constexpr uint32_t sub_10187000 = 0x10187000u;
constexpr uint32_t FUN_10187020 = 0x10187020u;
constexpr uint32_t FUN_10187030 = 0x10187030u;
constexpr uint32_t FUN_10187050 = 0x10187050u;
constexpr uint32_t FUN_10187380 = 0x10187380u;
constexpr uint32_t FUN_10187470 = 0x10187470u;
constexpr uint32_t FUN_101874a0 = 0x101874A0u;
constexpr uint32_t FUN_101875c0 = 0x101875C0u;
constexpr uint32_t FUN_101875e0 = 0x101875E0u;
constexpr uint32_t FUN_10187600 = 0x10187600u;
constexpr uint32_t FUN_10187720 = 0x10187720u;
constexpr uint32_t FUN_101880c0 = 0x101880C0u;
constexpr uint32_t FUN_10188190 = 0x10188190u;
constexpr uint32_t FUN_10188900 = 0x10188900u;
constexpr uint32_t FUN_10188910 = 0x10188910u;
constexpr uint32_t FUN_10188940 = 0x10188940u;
constexpr uint32_t FUN_101889a0 = 0x101889A0u;
constexpr uint32_t FUN_101889c0 = 0x101889C0u;
constexpr uint32_t FUN_101889e0 = 0x101889E0u;
constexpr uint32_t FUN_101889e0_v1 = 0x101889E0u;
constexpr uint32_t FUN_10188a10 = 0x10188A10u;
constexpr uint32_t FUN_10188a10_v1 = 0x10188A10u;
constexpr uint32_t FUN_10188a50 = 0x10188A50u;
constexpr uint32_t XiZone_GetDirLights = 0x10188B70u;
constexpr uint32_t XiZone_GetDirLights_v1 = 0x10188B70u;
constexpr uint32_t FUN_10188ca0 = 0x10188CA0u;
constexpr uint32_t sub_10188CA0 = 0x10188CA0u;
constexpr uint32_t Env_ComputeAmbient = 0x10188E00u;
constexpr uint32_t Env_ComputeAmbient_v1 = 0x10188E00u;
constexpr uint32_t FUN_10189120 = 0x10189120u;
constexpr uint32_t sub_10189120 = 0x10189120u;
constexpr uint32_t FUN_101892b0 = 0x101892B0u;
constexpr uint32_t FUN_10189490 = 0x10189490u;
constexpr uint32_t FUN_10189750 = 0x10189750u;
constexpr uint32_t FUN_10189810 = 0x10189810u;
constexpr uint32_t FUN_101898e0 = 0x101898E0u;
constexpr uint32_t FUN_10189980 = 0x10189980u;
constexpr uint32_t FUN_10189bd0 = 0x10189BD0u;
constexpr uint32_t FUN_10189c60 = 0x10189C60u;
constexpr uint32_t FUN_10189ca0 = 0x10189CA0u;
constexpr uint32_t FUN_10189e30 = 0x10189E30u;
constexpr uint32_t FUN_10189f20 = 0x10189F20u;
constexpr uint32_t FUN_1018a0f0 = 0x1018A0F0u;
constexpr uint32_t FUN_1018a220 = 0x1018A220u;
constexpr uint32_t FUN_1018a260 = 0x1018A260u;
constexpr uint32_t FUN_1018a2a0 = 0x1018A2A0u;
constexpr uint32_t FUN_1018a2c0 = 0x1018A2C0u;
constexpr uint32_t FUN_1018a2e0 = 0x1018A2E0u;
constexpr uint32_t FUN_1018a2f0 = 0x1018A2F0u;
constexpr uint32_t FUN_1018a300 = 0x1018A300u;
constexpr uint32_t FUN_1018a310 = 0x1018A310u;
constexpr uint32_t FUN_1018a320 = 0x1018A320u;
constexpr uint32_t FUN_1018a340 = 0x1018A340u;
constexpr uint32_t FUN_1018aac0 = 0x1018AAC0u;
constexpr uint32_t FUN_1018ab40 = 0x1018AB40u;
constexpr uint32_t FUN_1018abb0 = 0x1018ABB0u;
constexpr uint32_t FUN_1018abf0 = 0x1018ABF0u;
constexpr uint32_t FUN_1018ad50 = 0x1018AD50u;
constexpr uint32_t FUN_1018ad60 = 0x1018AD60u;
constexpr uint32_t FUN_1018ad80 = 0x1018AD80u;
constexpr uint32_t FUN_1018ada0 = 0x1018ADA0u;
constexpr uint32_t thunk_FUN_1018ade0 = 0x1018ADD0u;
constexpr uint32_t FUN_1018ade0 = 0x1018ADE0u;
constexpr uint32_t FUN_1018aeb0 = 0x1018AEB0u;
constexpr uint32_t FUN_1018b130 = 0x1018B130u;
constexpr uint32_t sub_1018B130 = 0x1018B130u;
constexpr uint32_t FUN_1018b170 = 0x1018B170u;
constexpr uint32_t FUN_1018b620 = 0x1018B620u;
constexpr uint32_t sub_1018B860 = 0x1018B860u;
constexpr uint32_t FUN_1018b8b0 = 0x1018B8B0u;
constexpr uint32_t sub_1018B960 = 0x1018B960u;
constexpr uint32_t XiZone_GetDirLight = 0x1018B9A0u;
constexpr uint32_t XiZone_GetAmbient = 0x1018B9E0u;
constexpr uint32_t XiZone_GetAmbient_v1 = 0x1018B9E0u;
constexpr uint32_t XiZone_GetFog = 0x1018BA20u;
constexpr uint32_t XiZone_GetFog_v1 = 0x1018BA20u;
constexpr uint32_t FUN_1018bae0 = 0x1018BAE0u;
constexpr uint32_t unknown_libname_158 = 0x1018BBB0u;
constexpr uint32_t FUN_1018bbc0 = 0x1018BBC0u;
constexpr uint32_t FUN_1018bc40 = 0x1018BC40u;
constexpr uint32_t FUN_1018bd30 = 0x1018BD30u;
constexpr uint32_t FUN_1018bf00 = 0x1018BF00u;
constexpr uint32_t FUN_1018bf50 = 0x1018BF50u;
constexpr uint32_t FUN_1018bfa0 = 0x1018BFA0u;
constexpr uint32_t FUN_1018c180 = 0x1018C180u;
constexpr uint32_t FUN_1018c290 = 0x1018C290u;
constexpr uint32_t sub_1018C2B0 = 0x1018C2B0u;
constexpr uint32_t CXiSchStatus_Destructor = 0x1018C620u;
constexpr uint32_t Sch_GetStatusEntry = 0x1018CA20u;
constexpr uint32_t FUN_1018cea0 = 0x1018CEA0u;
constexpr uint32_t FUN_1018d230 = 0x1018D230u;
constexpr uint32_t FUN_1018d250 = 0x1018D250u;
constexpr uint32_t FUN_1018d280 = 0x1018D280u;
constexpr uint32_t FUN_1018d2b0 = 0x1018D2B0u;
constexpr uint32_t FUN_1018d2d0 = 0x1018D2D0u;
constexpr uint32_t FUN_1018d3d0 = 0x1018D3D0u;
constexpr uint32_t FUN_1018d430 = 0x1018D430u;
constexpr uint32_t FUN_1018d440 = 0x1018D440u;
constexpr uint32_t FUN_1018d450 = 0x1018D450u;
constexpr uint32_t FUN_1018d470 = 0x1018D470u;
constexpr uint32_t FUN_1018d480 = 0x1018D480u;
constexpr uint32_t FUN_1018d490 = 0x1018D490u;
constexpr uint32_t FUN_1018d4a0 = 0x1018D4A0u;
constexpr uint32_t FUN_1018d580 = 0x1018D580u;
constexpr uint32_t FUN_1018d640 = 0x1018D640u;
constexpr uint32_t FUN_1018d7f0 = 0x1018D7F0u;
constexpr uint32_t FUN_1018d980 = 0x1018D980u;
constexpr uint32_t FUN_1018db10 = 0x1018DB10u;
constexpr uint32_t FUN_1018dc70 = 0x1018DC70u;
constexpr uint32_t FUN_1018dcb0 = 0x1018DCB0u;
constexpr uint32_t FUN_1018dcd0 = 0x1018DCD0u;
constexpr uint32_t FUN_1018e0a0 = 0x1018E0A0u;
constexpr uint32_t FUN_1018e0e0 = 0x1018E0E0u;
constexpr uint32_t FUN_1018e170 = 0x1018E170u;
constexpr uint32_t FUN_1018e1a0 = 0x1018E1A0u;
constexpr uint32_t FsLogPage_load = 0x1018E1D0u;
constexpr uint32_t FUN_1018e310 = 0x1018E310u;
constexpr uint32_t FUN_1018e4d0 = 0x1018E4D0u;
constexpr uint32_t FUN_1018e660 = 0x1018E660u;
constexpr uint32_t FUN_1018e6a0 = 0x1018E6A0u;
constexpr uint32_t FUN_1018e6f0 = 0x1018E6F0u;
constexpr uint32_t FUN_1018e7f0 = 0x1018E7F0u;
constexpr uint32_t FUN_1018e800 = 0x1018E800u;
constexpr uint32_t FUN_1018e8d0 = 0x1018E8D0u;
constexpr uint32_t FUN_1018e9b0 = 0x1018E9B0u;
constexpr uint32_t FUN_1018ea00 = 0x1018EA00u;
constexpr uint32_t FUN_1018ea50 = 0x1018EA50u;
constexpr uint32_t FUN_1018eb00 = 0x1018EB00u;
constexpr uint32_t FUN_1018fa20 = 0x1018FA20u;
constexpr uint32_t FUN_1018fa80 = 0x1018FA80u;
constexpr uint32_t FUN_1018fb00 = 0x1018FB00u;
constexpr uint32_t FUN_1018fd90 = 0x1018FD90u;
constexpr uint32_t FUN_1018fdf0 = 0x1018FDF0u;
constexpr uint32_t FUN_101907a0 = 0x101907A0u;
constexpr uint32_t operator_delete_101907D0 = 0x101907D0u;
constexpr uint32_t FUN_101908d0 = 0x101908D0u;
constexpr uint32_t FUN_10190bb0 = 0x10190BB0u;
constexpr uint32_t FUN_10193800 = 0x10193800u;
constexpr uint32_t SetConfig = 0x10193830u;
constexpr uint32_t Settings_GetValue = 0x10193850u;
constexpr uint32_t Settings_GetValue_v1 = 0x10193850u;
constexpr uint32_t Settings_GetValue_v2 = 0x10193850u;
constexpr uint32_t FUN_10193890 = 0x10193890u;
constexpr uint32_t FUN_101938b0 = 0x101938B0u;
constexpr uint32_t FUN_101938d0 = 0x101938D0u;
constexpr uint32_t FUN_10193a10 = 0x10193A10u;
constexpr uint32_t FUN_10193b00 = 0x10193B00u;
constexpr uint32_t FUN_10193b60 = 0x10193B60u;
constexpr uint32_t FUN_10193bc0 = 0x10193BC0u;
constexpr uint32_t FUN_10195220 = 0x10195220u;
constexpr uint32_t FUN_10195240 = 0x10195240u;
constexpr uint32_t FUN_101952c0 = 0x101952C0u;
constexpr uint32_t FUN_10195330 = 0x10195330u;
constexpr uint32_t FUN_10195350 = 0x10195350u;
constexpr uint32_t FUN_101953a0 = 0x101953A0u;
constexpr uint32_t FUN_101953c0 = 0x101953C0u;
constexpr uint32_t FUN_101953d0 = 0x101953D0u;
constexpr uint32_t FUN_101953f0 = 0x101953F0u;
constexpr uint32_t FUN_10195460 = 0x10195460u;
constexpr uint32_t FUN_101954d0 = 0x101954D0u;
constexpr uint32_t FUN_10195570 = 0x10195570u;
constexpr uint32_t FUN_10195640 = 0x10195640u;
constexpr uint32_t FUN_101956f0 = 0x101956F0u;
constexpr uint32_t FUN_10195780 = 0x10195780u;
constexpr uint32_t FUN_101957b0 = 0x101957B0u;
constexpr uint32_t FUN_10195810 = 0x10195810u;
constexpr uint32_t FUN_101958c0 = 0x101958C0u;
constexpr uint32_t FUN_10195900 = 0x10195900u;
constexpr uint32_t FUN_101959a0 = 0x101959A0u;
constexpr uint32_t FUN_10196040 = 0x10196040u;
constexpr uint32_t FUN_10196260 = 0x10196260u;
constexpr uint32_t FUN_101962d0 = 0x101962D0u;
constexpr uint32_t FUN_10196980 = 0x10196980u;
constexpr uint32_t FUN_10196be0 = 0x10196BE0u;
constexpr uint32_t FUN_10197a90 = 0x10197A90u;
constexpr uint32_t FUN_10199710 = 0x10199710u;
constexpr uint32_t sub_1019C3E0 = 0x1019C3E0u;
constexpr uint32_t FUN_1019c4f0 = 0x1019C4F0u;
constexpr uint32_t FUN_1019c580 = 0x1019C580u;
constexpr uint32_t FUN_1019c590 = 0x1019C590u;
constexpr uint32_t sub_1019C5B0 = 0x1019C5B0u;
constexpr uint32_t FUN_1019cd40 = 0x1019CD40u;
constexpr uint32_t FUN_1019cdc0 = 0x1019CDC0u;
constexpr uint32_t FUN_1019ce70 = 0x1019CE70u;
constexpr uint32_t FUN_1019cf60 = 0x1019CF60u;
constexpr uint32_t FUN_1019cfa0 = 0x1019CFA0u;
constexpr uint32_t FUN_1019d1c0 = 0x1019D1C0u;
constexpr uint32_t FUN_1019d2b0 = 0x1019D2B0u;
constexpr uint32_t FUN_1019d320 = 0x1019D320u;
constexpr uint32_t FUN_1019d350 = 0x1019D350u;
constexpr uint32_t FUN_1019d8b0 = 0x1019D8B0u;
constexpr uint32_t FUN_1019ead0 = 0x1019EAD0u;
constexpr uint32_t sub_1019EBB0 = 0x1019EBB0u;
constexpr uint32_t FUN_1019f8b0 = 0x1019F8B0u;
constexpr uint32_t FUN_1019f930 = 0x1019F930u;
constexpr uint32_t FUN_1019f960 = 0x1019F960u;
constexpr uint32_t FUN_1019fa90 = 0x1019FA90u;
constexpr uint32_t FUN_1019fab0 = 0x1019FAB0u;
constexpr uint32_t FsFileThreadManager_handleFileEvent = 0x1019FD50u;
constexpr uint32_t FUN_101a0910 = 0x101A0910u;
constexpr uint32_t FUN_101a0930 = 0x101A0930u;
constexpr uint32_t FUN_101a0950 = 0x101A0950u;
constexpr uint32_t FUN_101a0970 = 0x101A0970u;
constexpr uint32_t FUN_101a0be0 = 0x101A0BE0u;
constexpr uint32_t FUN_101a0d30 = 0x101A0D30u;
constexpr uint32_t FUN_101a1400 = 0x101A1400u;
constexpr uint32_t FUN_101a1460 = 0x101A1460u;
constexpr uint32_t FUN_101a1470 = 0x101A1470u;
constexpr uint32_t FUN_101a1500 = 0x101A1500u;
constexpr uint32_t FUN_101a1580 = 0x101A1580u;
constexpr uint32_t FUN_101a1660 = 0x101A1660u;
constexpr uint32_t FUN_101a1690 = 0x101A1690u;
constexpr uint32_t FUN_101a16b0 = 0x101A16B0u;
constexpr uint32_t FUN_101a16e0 = 0x101A16E0u;
constexpr uint32_t FUN_101a1740 = 0x101A1740u;
constexpr uint32_t FUN_101a1910 = 0x101A1910u;
constexpr uint32_t FUN_101a19b0 = 0x101A19B0u;
constexpr uint32_t FUN_101a1a20 = 0x101A1A20u;
constexpr uint32_t FUN_101a1c60 = 0x101A1C60u;
constexpr uint32_t FUN_101a1cb0 = 0x101A1CB0u;
constexpr uint32_t FUN_101a1cc0 = 0x101A1CC0u;
constexpr uint32_t FUN_101a23d0 = 0x101A23D0u;
constexpr uint32_t FUN_101a2480 = 0x101A2480u;
constexpr uint32_t FUN_101a2660 = 0x101A2660u;
constexpr uint32_t FUN_101a26c0 = 0x101A26C0u;
constexpr uint32_t FUN_101a2890 = 0x101A2890u;
constexpr uint32_t FUN_101a2e00 = 0x101A2E00u;
constexpr uint32_t FUN_101a2e60 = 0x101A2E60u;
constexpr uint32_t sub_101A2E80 = 0x101A2E80u;
constexpr uint32_t FUN_101a3300 = 0x101A3300u;
constexpr uint32_t FUN_101a33e0 = 0x101A33E0u;
constexpr uint32_t FUN_101a3500 = 0x101A3500u;
constexpr uint32_t FUN_101a3550 = 0x101A3550u;
constexpr uint32_t FUN_101a36f0 = 0x101A36F0u;
constexpr uint32_t FUN_101a3cf0 = 0x101A3CF0u;
constexpr uint32_t FUN_101a3d00 = 0x101A3D00u;
constexpr uint32_t FUN_101a3dc0 = 0x101A3DC0u;
constexpr uint32_t FUN_101a43a0 = 0x101A43A0u;
constexpr uint32_t FUN_101a43b0 = 0x101A43B0u;
constexpr uint32_t FUN_101a4440 = 0x101A4440u;
constexpr uint32_t FUN_101a46c0 = 0x101A46C0u;
constexpr uint32_t FUN_101a4d70 = 0x101A4D70u;
constexpr uint32_t FUN_101a54f0 = 0x101A54F0u;
constexpr uint32_t FUN_101a6430 = 0x101A6430u;
constexpr uint32_t FUN_101a6c10 = 0x101A6C10u;
constexpr uint32_t FUN_101a7280 = 0x101A7280u;
constexpr uint32_t FUN_101a83c0 = 0x101A83C0u;
constexpr uint32_t FUN_101a8410 = 0x101A8410u;
constexpr uint32_t FUN_101a8750 = 0x101A8750u;
constexpr uint32_t sub_101A8B50 = 0x101A8B50u;
constexpr uint32_t sub_101A8D40 = 0x101A8D40u;
constexpr uint32_t sub_101A8D60 = 0x101A8D60u;
constexpr uint32_t sub_101AB080 = 0x101AB080u;
constexpr uint32_t sub_101AB0D0 = 0x101AB0D0u;
constexpr uint32_t FUN_101ab730 = 0x101AB730u;
constexpr uint32_t sub_101AB770 = 0x101AB770u;
constexpr uint32_t FUN_101ab810 = 0x101AB810u;
constexpr uint32_t sub_101AB820 = 0x101AB820u;
constexpr uint32_t FUN_101abb40 = 0x101ABB40u;
constexpr uint32_t FUN_101abc50 = 0x101ABC50u;
constexpr uint32_t FUN_101abe40 = 0x101ABE40u;
constexpr uint32_t FUN_101abe50 = 0x101ABE50u;
constexpr uint32_t FUN_101abe80 = 0x101ABE80u;
constexpr uint32_t FUN_101abf40 = 0x101ABF40u;
constexpr uint32_t FUN_101ac1b0 = 0x101AC1B0u;
constexpr uint32_t FUN_101ac200 = 0x101AC200u;
constexpr uint32_t FUN_101ac580 = 0x101AC580u;
constexpr uint32_t FUN_101ac5e0 = 0x101AC5E0u;
constexpr uint32_t FUN_101ac990 = 0x101AC990u;
constexpr uint32_t FUN_101acac0 = 0x101ACAC0u;
constexpr uint32_t FUN_101acc30 = 0x101ACC30u;
constexpr uint32_t _ZN5CRGBAC2Ehhhh = 0x101ACF40u;
constexpr uint32_t FUN_101acf60 = 0x101ACF60u;
constexpr uint32_t FUN_101acf70 = 0x101ACF70u;
constexpr uint32_t FUN_101acf90 = 0x101ACF90u;
constexpr uint32_t FUN_101acfb0 = 0x101ACFB0u;
constexpr uint32_t FUN_101ad1b0 = 0x101AD1B0u;
constexpr uint32_t FUN_101ad820 = 0x101AD820u;
constexpr uint32_t FUN_101adbc0 = 0x101ADBC0u;
constexpr uint32_t FUN_101adbd0 = 0x101ADBD0u;
constexpr uint32_t FUN_101adbe0 = 0x101ADBE0u;
constexpr uint32_t FUN_101addc0 = 0x101ADDC0u;
constexpr uint32_t FUN_101adf20 = 0x101ADF20u;
constexpr uint32_t FsShortcut_CanUseShortcut = 0x101AE010u;
constexpr uint32_t sub_101AE3E0 = 0x101AE3E0u;
constexpr uint32_t FUN_101aff30 = 0x101AFF30u;
constexpr uint32_t ImmGetContext = 0x101AFF50u;
constexpr uint32_t ImmReleaseContext = 0x101AFF60u;
constexpr uint32_t FUN_101b36f0 = 0x101B36F0u;
constexpr uint32_t FUN_101b45b0 = 0x101B45B0u;
constexpr uint32_t FUN_101b45e0 = 0x101B45E0u;
constexpr uint32_t FUN_101b4710 = 0x101B4710u;
constexpr uint32_t FUN_101b4f90 = 0x101B4F90u;
constexpr uint32_t FUN_101b5c90 = 0x101B5C90u;
constexpr uint32_t FUN_101b5cc0 = 0x101B5CC0u;
constexpr uint32_t FUN_101b6810 = 0x101B6810u;
constexpr uint32_t FUN_101b6d50 = 0x101B6D50u;
constexpr uint32_t FUN_101b6e90 = 0x101B6E90u;
constexpr uint32_t FUN_101b6ec0 = 0x101B6EC0u;
constexpr uint32_t FUN_101b6ee0 = 0x101B6EE0u;
constexpr uint32_t FUN_101b7190 = 0x101B7190u;
constexpr uint32_t FUN_101b7280 = 0x101B7280u;
constexpr uint32_t FUN_101b72b0 = 0x101B72B0u;
constexpr uint32_t FUN_101b72e0 = 0x101B72E0u;
constexpr uint32_t FUN_101b7ee0 = 0x101B7EE0u;
constexpr uint32_t sub_101B8E70 = 0x101B8E70u;
constexpr uint32_t FUN_101b9530 = 0x101B9530u;
constexpr uint32_t FUN_101b9550 = 0x101B9550u;
constexpr uint32_t FUN_101b95d0 = 0x101B95D0u;
constexpr uint32_t FUN_101b9610 = 0x101B9610u;
constexpr uint32_t FUN_101b9650 = 0x101B9650u;
constexpr uint32_t FsTinyString_init = 0x101B9690u;
constexpr uint32_t FUN_101b9880 = 0x101B9880u;
constexpr uint32_t FUN_101b9980 = 0x101B9980u;
constexpr uint32_t FUN_101b9a00 = 0x101B9A00u;
constexpr uint32_t FUN_101b9a10 = 0x101B9A10u;
constexpr uint32_t FUN_101b9bc0 = 0x101B9BC0u;
constexpr uint32_t FUN_101b9e00 = 0x101B9E00u;
constexpr uint32_t FUN_101b9e60 = 0x101B9E60u;
constexpr uint32_t FUN_101b9ec0 = 0x101B9EC0u;
constexpr uint32_t FUN_101b9f20 = 0x101B9F20u;
constexpr uint32_t FUN_101b9ff0 = 0x101B9FF0u;
constexpr uint32_t FUN_101ba030 = 0x101BA030u;
constexpr uint32_t FUN_101ba180 = 0x101BA180u;
constexpr uint32_t FUN_101ba280 = 0x101BA280u;
constexpr uint32_t FUN_101ba2d0 = 0x101BA2D0u;
constexpr uint32_t FUN_101ba300 = 0x101BA300u;
constexpr uint32_t FUN_101ba570 = 0x101BA570u;
constexpr uint32_t FUN_101ba5b0 = 0x101BA5B0u;
constexpr uint32_t FUN_101ba5c0 = 0x101BA5C0u;
constexpr uint32_t FUN_101badd0 = 0x101BADD0u;
constexpr uint32_t FUN_101bae20 = 0x101BAE20u;
constexpr uint32_t FUN_101bae30 = 0x101BAE30u;
constexpr uint32_t FUN_101baeb0 = 0x101BAEB0u;
constexpr uint32_t FUN_101baec0 = 0x101BAEC0u;
constexpr uint32_t FUN_101baf90 = 0x101BAF90u;
constexpr uint32_t FUN_101bafc0 = 0x101BAFC0u;
constexpr uint32_t FUN_101bb070 = 0x101BB070u;
constexpr uint32_t FUN_101bb100 = 0x101BB100u;
constexpr uint32_t FUN_101bb2b0 = 0x101BB2B0u;
constexpr uint32_t FUN_101bb4b0 = 0x101BB4B0u;
constexpr uint32_t FUN_101bb680 = 0x101BB680u;
constexpr uint32_t FUN_101bb740 = 0x101BB740u;
constexpr uint32_t FUN_101bb7a0 = 0x101BB7A0u;
constexpr uint32_t FUN_101bb9e0 = 0x101BB9E0u;
constexpr uint32_t FUN_101bba90 = 0x101BBA90u;
constexpr uint32_t FUN_101bbc60 = 0x101BBC60u;
constexpr uint32_t FUN_101bbe50 = 0x101BBE50u;
constexpr uint32_t FsWordCompletion_getEntryList = 0x101BBE60u;
constexpr uint32_t FsWordCompletion_lookUp = 0x101BC100u;
constexpr uint32_t FsWordCompletion__prepareAllData = 0x101BC520u;
constexpr uint32_t FUN_101bc6f0 = 0x101BC6F0u;
constexpr uint32_t FUN_101bc720 = 0x101BC720u;
constexpr uint32_t FUN_101bc740 = 0x101BC740u;
constexpr uint32_t FUN_101bc7b0 = 0x101BC7B0u;
constexpr uint32_t FUN_101bc900 = 0x101BC900u;
constexpr uint32_t FUN_101bc9b0 = 0x101BC9B0u;
constexpr uint32_t FsMemory_Constructor = 0x101BC9E0u;
constexpr uint32_t FUN_101bca30 = 0x101BCA30u;
constexpr uint32_t FUN_101bca40 = 0x101BCA40u;
constexpr uint32_t FUN_101bcac0 = 0x101BCAC0u;
constexpr uint32_t FUN_101bcb30 = 0x101BCB30u;
constexpr uint32_t FUN_101bcba0 = 0x101BCBA0u;
constexpr uint32_t sub_101BCE30 = 0x101BCE30u;
constexpr uint32_t FUN_101bcef0 = 0x101BCEF0u;
constexpr uint32_t FUN_101bcf30 = 0x101BCF30u;
constexpr uint32_t FUN_101be260 = 0x101BE260u;
constexpr uint32_t FUN_101be2a0 = 0x101BE2A0u;
constexpr uint32_t FUN_101be2b0 = 0x101BE2B0u;
constexpr uint32_t FUN_101be2c0 = 0x101BE2C0u;
constexpr uint32_t FUN_101be330 = 0x101BE330u;
constexpr uint32_t FUN_101be3a0 = 0x101BE3A0u;
constexpr uint32_t FUN_101be410 = 0x101BE410u;
constexpr uint32_t FUN_101be470 = 0x101BE470u;
constexpr uint32_t FUN_101be500 = 0x101BE500u;
constexpr uint32_t FUN_101be510 = 0x101BE510u;
constexpr uint32_t FUN_101be590 = 0x101BE590u;
constexpr uint32_t FUN_101be670 = 0x101BE670u;
constexpr uint32_t FUN_101be690 = 0x101BE690u;
constexpr uint32_t FUN_101be6c0 = 0x101BE6C0u;
constexpr uint32_t FUN_101be730 = 0x101BE730u;
constexpr uint32_t FUN_101beb90 = 0x101BEB90u;
constexpr uint32_t FUN_101becc0 = 0x101BECC0u;
constexpr uint32_t FUN_101bf280 = 0x101BF280u;
constexpr uint32_t FUN_101bf370 = 0x101BF370u;
constexpr uint32_t FUN_101bf460 = 0x101BF460u;
constexpr uint32_t FUN_101bf550 = 0x101BF550u;
constexpr uint32_t FUN_101bf970 = 0x101BF970u;
constexpr uint32_t FUN_101c0050 = 0x101C0050u;
constexpr uint32_t FUN_101c0160 = 0x101C0160u;
constexpr uint32_t FUN_101c0260 = 0x101C0260u;
constexpr uint32_t FUN_101c0440 = 0x101C0440u;
constexpr uint32_t FUN_101c0540 = 0x101C0540u;
constexpr uint32_t FUN_101c0630 = 0x101C0630u;
constexpr uint32_t FUN_101c0760 = 0x101C0760u;
constexpr uint32_t FUN_101c09b0 = 0x101C09B0u;
constexpr uint32_t FUN_101c0a40 = 0x101C0A40u;
constexpr uint32_t FUN_101c0fc0 = 0x101C0FC0u;
constexpr uint32_t FUN_101c1090 = 0x101C1090u;
constexpr uint32_t FUN_101c11f0 = 0x101C11F0u;
constexpr uint32_t FUN_101c13a0 = 0x101C13A0u;
constexpr uint32_t FUN_101c1750 = 0x101C1750u;
constexpr uint32_t FUN_101c1820 = 0x101C1820u;
constexpr uint32_t FUN_101c18e0 = 0x101C18E0u;
constexpr uint32_t FUN_101c1a40 = 0x101C1A40u;
constexpr uint32_t FUN_101c1b10 = 0x101C1B10u;
constexpr uint32_t FUN_101c1b20 = 0x101C1B20u;
constexpr uint32_t FUN_101c1c90 = 0x101C1C90u;
constexpr uint32_t FUN_101c1df0 = 0x101C1DF0u;
constexpr uint32_t FUN_101c1ec0 = 0x101C1EC0u;
constexpr uint32_t FUN_101c2450 = 0x101C2450u;
constexpr uint32_t FUN_101c2670 = 0x101C2670u;
constexpr uint32_t FUN_101c2840 = 0x101C2840u;
constexpr uint32_t FUN_101c2b60 = 0x101C2B60u;
constexpr uint32_t FUN_101c2c80 = 0x101C2C80u;
constexpr uint32_t thunk_FUN_101c4170 = 0x101C2D80u;
constexpr uint32_t FUN_101c2d90 = 0x101C2D90u;
constexpr uint32_t FUN_101c2e70 = 0x101C2E70u;
constexpr uint32_t FUN_101c33e0 = 0x101C33E0u;
constexpr uint32_t FUN_101c33f0 = 0x101C33F0u;
constexpr uint32_t XiFileImpl_getFileName = 0x101C3A90u;
constexpr uint32_t FUN_101c3d00 = 0x101C3D00u;
constexpr uint32_t FUN_101c3db0 = 0x101C3DB0u;
constexpr uint32_t FUN_101c3de0 = 0x101C3DE0u;
constexpr uint32_t FUN_101c3e10 = 0x101C3E10u;
constexpr uint32_t FUN_101c3e90 = 0x101C3E90u;
constexpr uint32_t FUN_101c3f40 = 0x101C3F40u;
constexpr uint32_t FUN_101c3fb0 = 0x101C3FB0u;
constexpr uint32_t FUN_101c3fe0 = 0x101C3FE0u;
constexpr uint32_t FUN_101c4170 = 0x101C4170u;
constexpr uint32_t sub_101C46A0 = 0x101C46A0u;
constexpr uint32_t FUN_101c46d0 = 0x101C46D0u;
constexpr uint32_t FUN_101c4740 = 0x101C4740u;
constexpr uint32_t FUN_101c4790 = 0x101C4790u;
constexpr uint32_t FUN_101c47a0 = 0x101C47A0u;
constexpr uint32_t FUN_101c47b0 = 0x101C47B0u;
constexpr uint32_t FUN_101c4820 = 0x101C4820u;
constexpr uint32_t FUN_101c48e0 = 0x101C48E0u;
constexpr uint32_t FUN_101c48f0 = 0x101C48F0u;
constexpr uint32_t FUN_101c4900 = 0x101C4900u;
constexpr uint32_t FUN_101c4920 = 0x101C4920u;
constexpr uint32_t FUN_101c4930 = 0x101C4930u;
constexpr uint32_t FUN_101c4950 = 0x101C4950u;
constexpr uint32_t FUN_101c49a0 = 0x101C49A0u;
constexpr uint32_t FUN_101c49f0 = 0x101C49F0u;
constexpr uint32_t FUN_101c4a10 = 0x101C4A10u;
constexpr uint32_t FUN_101c4a30 = 0x101C4A30u;
constexpr uint32_t FUN_101c4a80 = 0x101C4A80u;
constexpr uint32_t FUN_101c4a90 = 0x101C4A90u;
constexpr uint32_t FUN_101c4aa0 = 0x101C4AA0u;
constexpr uint32_t FUN_101c4ac0 = 0x101C4AC0u;
constexpr uint32_t FUN_101c4ad0 = 0x101C4AD0u;
constexpr uint32_t FileMng_ResolveOrExit = 0x101C4AF0u;
constexpr uint32_t FileMng_ResolveOrExit_v1 = 0x101C4AF0u;
constexpr uint32_t FUN_101c4b40 = 0x101C4B40u;
constexpr uint32_t FUN_101c4b60 = 0x101C4B60u;
constexpr uint32_t FUN_101c4b70 = 0x101C4B70u;
constexpr uint32_t FUN_101c4bc0 = 0x101C4BC0u;
constexpr uint32_t FUN_101c4bf0 = 0x101C4BF0u;
constexpr uint32_t FUN_101c4c20 = 0x101C4C20u;
constexpr uint32_t FUN_101c4c70 = 0x101C4C70u;
constexpr uint32_t sub_101C4CC0 = 0x101C4CC0u;
constexpr uint32_t sub_101C4CD0 = 0x101C4CD0u;
constexpr uint32_t sub_101C4CE0 = 0x101C4CE0u;
constexpr uint32_t FUN_101c4cf0 = 0x101C4CF0u;
constexpr uint32_t sub_101C4D10 = 0x101C4D10u;
constexpr uint32_t FUN_101c4d30 = 0x101C4D30u;
constexpr uint32_t FUN_101c4d40 = 0x101C4D40u;
constexpr uint32_t FUN_101c5920 = 0x101C5920u;
constexpr uint32_t FUN_101c5930 = 0x101C5930u;
constexpr uint32_t FUN_101c59b0 = 0x101C59B0u;
constexpr uint32_t FUN_101c5a30 = 0x101C5A30u;
constexpr uint32_t FUN_101c5a40 = 0x101C5A40u;
constexpr uint32_t FUN_101c5be0 = 0x101C5BE0u;
constexpr uint32_t FUN_101c5ec0 = 0x101C5EC0u;
constexpr uint32_t FUN_101c60c0 = 0x101C60C0u;
constexpr uint32_t FUN_101c6960 = 0x101C6960u;
constexpr uint32_t FUN_101c7c50 = 0x101C7C50u;
constexpr uint32_t FUN_101c7f80 = 0x101C7F80u;
constexpr uint32_t FUN_101c8070 = 0x101C8070u;
constexpr uint32_t FUN_101c80a0 = 0x101C80A0u;
constexpr uint32_t FUN_101c80c0 = 0x101C80C0u;
constexpr uint32_t FUN_101c8260 = 0x101C8260u;
constexpr uint32_t FUN_101c8290 = 0x101C8290u;
constexpr uint32_t FUN_101c8380 = 0x101C8380u;
constexpr uint32_t FUN_101c87a0 = 0x101C87A0u;
constexpr uint32_t FUN_101c88a0 = 0x101C88A0u;
constexpr uint32_t YkCommonInit = 0x101C8950u;
constexpr uint32_t FUN_101c8c30 = 0x101C8C30u;
constexpr uint32_t FUN_101c8d10 = 0x101C8D10u;
constexpr uint32_t FUN_101c8d70 = 0x101C8D70u;
constexpr uint32_t Gfx_ProjectWorldToScreen = 0x101C8DA0u;
constexpr uint32_t FUN_101c9780 = 0x101C9780u;
constexpr uint32_t FUN_101c97b0 = 0x101C97B0u;
constexpr uint32_t FUN_101c9b50 = 0x101C9B50u;
constexpr uint32_t FUN_101c9d50 = 0x101C9D50u;
constexpr uint32_t FUN_101ca180 = 0x101CA180u;
constexpr uint32_t FUN_101cad00 = 0x101CAD00u;
constexpr uint32_t FUN_101caec0 = 0x101CAEC0u;
constexpr uint32_t FUN_101caf00 = 0x101CAF00u;
constexpr uint32_t FUN_101cb000 = 0x101CB000u;
constexpr uint32_t FUN_101cb040 = 0x101CB040u;
constexpr uint32_t FUN_101cb090 = 0x101CB090u;
constexpr uint32_t FUN_101cb230 = 0x101CB230u;
constexpr uint32_t FUN_101cb280 = 0x101CB280u;
constexpr uint32_t FUN_101cb430 = 0x101CB430u;
constexpr uint32_t FUN_101cb500 = 0x101CB500u;
constexpr uint32_t FUN_101cb550 = 0x101CB550u;
constexpr uint32_t FUN_101cb770 = 0x101CB770u;
constexpr uint32_t FUN_101cbdf0 = 0x101CBDF0u;
constexpr uint32_t FUN_101cbe70 = 0x101CBE70u;
constexpr uint32_t FUN_101cc070 = 0x101CC070u;
constexpr uint32_t FUN_101cc0d0 = 0x101CC0D0u;
constexpr uint32_t FUN_101cc1d0 = 0x101CC1D0u;
constexpr uint32_t FUN_101cc220 = 0x101CC220u;
constexpr uint32_t FUN_101cc350 = 0x101CC350u;
constexpr uint32_t FUN_101cc4f0 = 0x101CC4F0u;
constexpr uint32_t FUN_101cc510 = 0x101CC510u;
constexpr uint32_t FUN_101cc520 = 0x101CC520u;
constexpr uint32_t FUN_101cc550 = 0x101CC550u;
constexpr uint32_t FUN_101cc560 = 0x101CC560u;
constexpr uint32_t FUN_101cc620 = 0x101CC620u;
constexpr uint32_t FUN_101cce00 = 0x101CCE00u;
constexpr uint32_t FUN_101cd900 = 0x101CD900u;
constexpr uint32_t FUN_101cd9d0 = 0x101CD9D0u;
constexpr uint32_t FUN_101cda00 = 0x101CDA00u;
constexpr uint32_t FUN_101cda30 = 0x101CDA30u;
constexpr uint32_t FUN_101cf660 = 0x101CF660u;
constexpr uint32_t FUN_101cf820 = 0x101CF820u;
constexpr uint32_t FUN_101cf870 = 0x101CF870u;
constexpr uint32_t YkJpeg_CutColor = 0x101CFA00u;
constexpr uint32_t FUN_101cfdb0 = 0x101CFDB0u;
constexpr uint32_t FUN_101cfe20 = 0x101CFE20u;
constexpr uint32_t FUN_101cfeb0 = 0x101CFEB0u;
constexpr uint32_t FUN_101cff60 = 0x101CFF60u;
constexpr uint32_t FUN_101d01b0 = 0x101D01B0u;
constexpr uint32_t FUN_101d0210 = 0x101D0210u;
constexpr uint32_t FUN_101d0370 = 0x101D0370u;
constexpr uint32_t FUN_101d05e0 = 0x101D05E0u;
constexpr uint32_t FUN_101d0600 = 0x101D0600u;
constexpr uint32_t FUN_101d0860 = 0x101D0860u;
constexpr uint32_t FUN_101d0880 = 0x101D0880u;
constexpr uint32_t FUN_101d08e0 = 0x101D08E0u;
constexpr uint32_t FUN_101d0910 = 0x101D0910u;
constexpr uint32_t FUN_101d0970 = 0x101D0970u;
constexpr uint32_t FUN_101d09c0 = 0x101D09C0u;
constexpr uint32_t FUN_101d09e0 = 0x101D09E0u;
constexpr uint32_t FUN_101d0a20 = 0x101D0A20u;
constexpr uint32_t FUN_101d0a40 = 0x101D0A40u;
constexpr uint32_t FUN_101d0a60 = 0x101D0A60u;
constexpr uint32_t sub_101D0B00 = 0x101D0B00u;
constexpr uint32_t sub_101D0C70 = 0x101D0C70u;
constexpr uint32_t YkInit = 0x101D11C0u;
constexpr uint32_t FUN_101d2260 = 0x101D2260u;
constexpr uint32_t FUN_101d29c0 = 0x101D29C0u;
constexpr uint32_t FUN_101d2f00 = 0x101D2F00u;
constexpr uint32_t FUN_101d30b0 = 0x101D30B0u;
constexpr uint32_t FUN_101d30f0 = 0x101D30F0u;
constexpr uint32_t FUN_101d31b0 = 0x101D31B0u;
constexpr uint32_t FUN_101d3dc0 = 0x101D3DC0u;
constexpr uint32_t FUN_101d3fc0 = 0x101D3FC0u;
constexpr uint32_t FUN_101d4a20 = 0x101D4A20u;
constexpr uint32_t FUN_101d5370 = 0x101D5370u;
constexpr uint32_t sub_101D5680 = 0x101D5680u;
constexpr uint32_t FUN_101d5fe0 = 0x101D5FE0u;
constexpr uint32_t FUN_101d7c10 = 0x101D7C10u;
constexpr uint32_t FUN_101d87a0 = 0x101D87A0u;
constexpr uint32_t FUN_101d9b10 = 0x101D9B10u;
constexpr uint32_t FUN_101d9e90 = 0x101D9E90u;
constexpr uint32_t FUN_101dc460 = 0x101DC460u;
constexpr uint32_t FUN_101dce20 = 0x101DCE20u;
constexpr uint32_t FUN_101dd000 = 0x101DD000u;
constexpr uint32_t FUN_101dda70 = 0x101DDA70u;
constexpr uint32_t FUN_101dda90 = 0x101DDA90u;
constexpr uint32_t FUN_101ddf90 = 0x101DDF90u;
constexpr uint32_t FUN_101de6b0 = 0x101DE6B0u;
constexpr uint32_t FUN_101dedc0 = 0x101DEDC0u;
constexpr uint32_t FUN_101df210 = 0x101DF210u;
constexpr uint32_t FUN_101e06c0 = 0x101E06C0u;
constexpr uint32_t FUN_101e0700 = 0x101E0700u;
constexpr uint32_t FUN_101e0a90 = 0x101E0A90u;
constexpr uint32_t FUN_101e15f0 = 0x101E15F0u;
constexpr uint32_t FUN_101e1a00 = 0x101E1A00u;
constexpr uint32_t FUN_101e24f0 = 0x101E24F0u;
constexpr uint32_t FUN_101e2b10 = 0x101E2B10u;
constexpr uint32_t FUN_101e3f50 = 0x101E3F50u;
constexpr uint32_t FUN_101e48f0 = 0x101E48F0u;
constexpr uint32_t FUN_101e4a20 = 0x101E4A20u;
constexpr uint32_t FUN_101e51b0 = 0x101E51B0u;
constexpr uint32_t FUN_101e5750 = 0x101E5750u;
constexpr uint32_t FUN_101e5ba0 = 0x101E5BA0u;
constexpr uint32_t FUN_101e6fb0 = 0x101E6FB0u;
constexpr uint32_t FUN_101eb390 = 0x101EB390u;
constexpr uint32_t FUN_101eb7e0 = 0x101EB7E0u;
constexpr uint32_t FUN_101ebe00 = 0x101EBE00u;
constexpr uint32_t FUN_101ec0d0 = 0x101EC0D0u;
constexpr uint32_t FUN_101ec3c0 = 0x101EC3C0u;
constexpr uint32_t FUN_101ed450 = 0x101ED450u;
constexpr uint32_t FUN_101ee230 = 0x101EE230u;
constexpr uint32_t sub_101EE7B0 = 0x101EE7B0u;
constexpr uint32_t FUN_101efca0 = 0x101EFCA0u;
constexpr uint32_t FUN_101efea0 = 0x101EFEA0u;
constexpr uint32_t FUN_101f17c0 = 0x101F17C0u;
constexpr uint32_t FUN_101f43f0 = 0x101F43F0u;
constexpr uint32_t FUN_101f4af0 = 0x101F4AF0u;
constexpr uint32_t YkWndList_SetLine = 0x101F6700u;
constexpr uint32_t YkWndList_SetLine_v1 = 0x101F6700u;
constexpr uint32_t FUN_101f67c0 = 0x101F67C0u;
constexpr uint32_t FUN_101f71a0 = 0x101F71A0u;
constexpr uint32_t FUN_101f71c0 = 0x101F71C0u;
constexpr uint32_t FUN_101f7200 = 0x101F7200u;
constexpr uint32_t FUN_101f7240 = 0x101F7240u;
constexpr uint32_t FUN_101f7260 = 0x101F7260u;
constexpr uint32_t FUN_101f7430 = 0x101F7430u;
constexpr uint32_t FUN_101f7480 = 0x101F7480u;
constexpr uint32_t FUN_101f7570 = 0x101F7570u;
constexpr uint32_t FUN_101f7600 = 0x101F7600u;
constexpr uint32_t FUN_101f82b0 = 0x101F82B0u;
constexpr uint32_t FUN_101f86c0 = 0x101F86C0u;
constexpr uint32_t FUN_101f92b0 = 0x101F92B0u;
constexpr uint32_t FUN_101f9370 = 0x101F9370u;
constexpr uint32_t FUN_101fb710 = 0x101FB710u;
constexpr uint32_t FUN_101fc2c0 = 0x101FC2C0u;
constexpr uint32_t FUN_101fcbc0 = 0x101FCBC0u;
constexpr uint32_t FUN_101fd000 = 0x101FD000u;
constexpr uint32_t FUN_101fe430 = 0x101FE430u;
constexpr uint32_t FUN_101ff000 = 0x101FF000u;
constexpr uint32_t FUN_10200b60 = 0x10200B60u;
constexpr uint32_t FUN_10201020 = 0x10201020u;
constexpr uint32_t FUN_102010a0 = 0x102010A0u;
constexpr uint32_t FUN_102010e0 = 0x102010E0u;
constexpr uint32_t FUN_10201130 = 0x10201130u;
constexpr uint32_t FUN_10201180 = 0x10201180u;
constexpr uint32_t FUN_10201340 = 0x10201340u;
constexpr uint32_t FUN_102014c0 = 0x102014C0u;
constexpr uint32_t FUN_102017a0 = 0x102017A0u;
constexpr uint32_t FUN_10201c90 = 0x10201C90u;
constexpr uint32_t FUN_10201eb0 = 0x10201EB0u;
constexpr uint32_t FUN_10201f00 = 0x10201F00u;
constexpr uint32_t FUN_10202c90 = 0x10202C90u;
constexpr uint32_t FUN_10202d30 = 0x10202D30u;
constexpr uint32_t FUN_10202d80 = 0x10202D80u;
constexpr uint32_t FUN_10202f00 = 0x10202F00u;
constexpr uint32_t FUN_10202fe0 = 0x10202FE0u;
constexpr uint32_t FUN_102031f0 = 0x102031F0u;
constexpr uint32_t FUN_102033c0 = 0x102033C0u;
constexpr uint32_t FUN_102034a0 = 0x102034A0u;
constexpr uint32_t FUN_102034e0 = 0x102034E0u;
constexpr uint32_t FUN_10203500 = 0x10203500u;
constexpr uint32_t FUN_10203520 = 0x10203520u;
constexpr uint32_t FUN_10203540 = 0x10203540u;
constexpr uint32_t FUN_10203920 = 0x10203920u;
constexpr uint32_t FUN_10203be0 = 0x10203BE0u;
constexpr uint32_t FUN_10205500 = 0x10205500u;
constexpr uint32_t FUN_10206420 = 0x10206420u;
constexpr uint32_t FUN_10207730 = 0x10207730u;
constexpr uint32_t FUN_10208500 = 0x10208500u;
constexpr uint32_t FUN_10209300 = 0x10209300u;
constexpr uint32_t FUN_10209b90 = 0x10209B90u;
constexpr uint32_t FUN_1020b500 = 0x1020B500u;
constexpr uint32_t FUN_1020ba40 = 0x1020BA40u;
constexpr uint32_t FUN_1020be90 = 0x1020BE90u;
constexpr uint32_t FUN_1020c470 = 0x1020C470u;
constexpr uint32_t FUN_1020cc30 = 0x1020CC30u;
constexpr uint32_t FUN_1020ed00 = 0x1020ED00u;
constexpr uint32_t YkWndShop_Constructor = 0x10210310u;
constexpr uint32_t FUN_10210d60 = 0x10210D60u;
constexpr uint32_t FUN_10210da0 = 0x10210DA0u;
constexpr uint32_t FUN_10210fc0 = 0x10210FC0u;
constexpr uint32_t FUN_102112b0 = 0x102112B0u;
constexpr uint32_t FUN_10211340 = 0x10211340u;
constexpr uint32_t FUN_102174f0 = 0x102174F0u;
constexpr uint32_t FUN_102175c0 = 0x102175C0u;
constexpr uint32_t FUN_1021b330 = 0x1021B330u;
constexpr uint32_t sub_1021BB90 = 0x1021BB90u;
constexpr uint32_t FUN_1021bc20 = 0x1021BC20u;
constexpr uint32_t FUN_1021bc30 = 0x1021BC30u;
constexpr uint32_t FUN_1021bcb0 = 0x1021BCB0u;
constexpr uint32_t FUN_1021bcc0 = 0x1021BCC0u;
constexpr uint32_t FUN_1021bdb0 = 0x1021BDB0u;
constexpr uint32_t FUN_1021be50 = 0x1021BE50u;
constexpr uint32_t FUN_1021bf60 = 0x1021BF60u;
constexpr uint32_t FUN_1021c070 = 0x1021C070u;
constexpr uint32_t FUN_1021c100 = 0x1021C100u;
constexpr uint32_t FUN_1021c160 = 0x1021C160u;
constexpr uint32_t FUN_1021c1b0 = 0x1021C1B0u;
constexpr uint32_t FUN_1021c210 = 0x1021C210u;
constexpr uint32_t FUN_1021c2e0 = 0x1021C2E0u;
constexpr uint32_t FUN_1021c350 = 0x1021C350u;
constexpr uint32_t FUN_1021c380 = 0x1021C380u;
constexpr uint32_t FUN_1021c5b0 = 0x1021C5B0u;
constexpr uint32_t FUN_1021c680 = 0x1021C680u;
constexpr uint32_t FUN_1021c7c0 = 0x1021C7C0u;
constexpr uint32_t FUN_1021c7e0 = 0x1021C7E0u;
constexpr uint32_t FUN_1021d250 = 0x1021D250u;
constexpr uint32_t FUN_1021d270 = 0x1021D270u;
constexpr uint32_t FUN_1021d2d0 = 0x1021D2D0u;
constexpr uint32_t FUN_1021d380 = 0x1021D380u;
constexpr uint32_t FUN_1021d460 = 0x1021D460u;
constexpr uint32_t CXiMovie_DrawFrame = 0x1021D4C0u;
constexpr uint32_t FUN_1021d7f0 = 0x1021D7F0u;
constexpr uint32_t FUN_1021d930 = 0x1021D930u;
constexpr uint32_t FUN_1021d960 = 0x1021D960u;
constexpr uint32_t Movie_Update = 0x1021D980u;
constexpr uint32_t FUN_1021d9a0 = 0x1021D9A0u;
constexpr uint32_t FUN_1021d9b0 = 0x1021D9B0u;
constexpr uint32_t FUN_1021dd50 = 0x1021DD50u;
constexpr uint32_t Movie_GetCurrentTexture = 0x1021DDF0u;
constexpr uint32_t FUN_1021e3b0 = 0x1021E3B0u;
constexpr uint32_t FUN_1021e480 = 0x1021E480u;
constexpr uint32_t FUN_1021e4d0 = 0x1021E4D0u;
constexpr uint32_t FUN_1021e500 = 0x1021E500u;
constexpr uint32_t FUN_1021e550 = 0x1021E550u;
constexpr uint32_t FUN_1021e670 = 0x1021E670u;
constexpr uint32_t FUN_1021e690 = 0x1021E690u;
constexpr uint32_t FUN_1021e760 = 0x1021E760u;
constexpr uint32_t FUN_1021e870 = 0x1021E870u;
constexpr uint32_t FUN_1021ed30 = 0x1021ED30u;
constexpr uint32_t FUN_1021ef10 = 0x1021EF10u;
constexpr uint32_t FUN_1021ef30 = 0x1021EF30u;
constexpr uint32_t FUN_1021f140 = 0x1021F140u;
constexpr uint32_t FUN_1021f240 = 0x1021F240u;
constexpr uint32_t FUN_1021f360 = 0x1021F360u;
constexpr uint32_t FUN_1021f480 = 0x1021F480u;
constexpr uint32_t FUN_1021f950 = 0x1021F950u;
constexpr uint32_t FUN_1021fa90 = 0x1021FA90u;
constexpr uint32_t FUN_1021fb10 = 0x1021FB10u;
constexpr uint32_t FUN_1021fb80 = 0x1021FB80u;
constexpr uint32_t FUN_1021fbd0 = 0x1021FBD0u;
constexpr uint32_t FUN_102201a0 = 0x102201A0u;
constexpr uint32_t FUN_10220350 = 0x10220350u;
constexpr uint32_t FUN_102203b0 = 0x102203B0u;
constexpr uint32_t FUN_10220c80 = 0x10220C80u;
constexpr uint32_t FUN_10220cf0 = 0x10220CF0u;
constexpr uint32_t sub_10220EF0 = 0x10220EF0u;
constexpr uint32_t sub_10220F40 = 0x10220F40u;
constexpr uint32_t FUN_10220fd0 = 0x10220FD0u;
constexpr uint32_t FUN_10221030 = 0x10221030u;
constexpr uint32_t FUN_102211f0 = 0x102211F0u;
constexpr uint32_t FUN_10221230 = 0x10221230u;
constexpr uint32_t FUN_102212c0 = 0x102212C0u;
constexpr uint32_t FUN_102212e0 = 0x102212E0u;
constexpr uint32_t FUN_10221320 = 0x10221320u;
constexpr uint32_t FUN_10221340 = 0x10221340u;
constexpr uint32_t FUN_102213b0 = 0x102213B0u;
constexpr uint32_t FUN_102213e0 = 0x102213E0u;
constexpr uint32_t FUN_102214d0 = 0x102214D0u;
constexpr uint32_t FUN_10221680 = 0x10221680u;
constexpr uint32_t sub_10221770 = 0x10221770u;
constexpr uint32_t sub_102217A0 = 0x102217A0u;
constexpr uint32_t FUN_102217c0 = 0x102217C0u;
constexpr uint32_t FUN_10221900 = 0x10221900u;
constexpr uint32_t FUN_10221920 = 0x10221920u;
constexpr uint32_t FUN_10221b90 = 0x10221B90u;
constexpr uint32_t FUN_10221be0 = 0x10221BE0u;
constexpr uint32_t FUN_10221e10 = 0x10221E10u;
constexpr uint32_t FUN_10221ed0 = 0x10221ED0u;
constexpr uint32_t FUN_102224c0 = 0x102224C0u;
constexpr uint32_t FUN_102225f0 = 0x102225F0u;
constexpr uint32_t FUN_10222c10 = 0x10222C10u;
constexpr uint32_t FUN_102238e0 = 0x102238E0u;
constexpr uint32_t FUN_10223930 = 0x10223930u;
constexpr uint32_t FUN_102239a0 = 0x102239A0u;
constexpr uint32_t FUN_102239c0 = 0x102239C0u;
constexpr uint32_t FUN_10223bb0 = 0x10223BB0u;
constexpr uint32_t FUN_10223c30 = 0x10223C30u;
constexpr uint32_t FUN_10223c90 = 0x10223C90u;
constexpr uint32_t FUN_10223e60 = 0x10223E60u;
constexpr uint32_t FUN_10225090 = 0x10225090u;
constexpr uint32_t FUN_102250f0 = 0x102250F0u;
constexpr uint32_t FUN_10225120 = 0x10225120u;
constexpr uint32_t FUN_10225220 = 0x10225220u;
constexpr uint32_t FUN_102254d0 = 0x102254D0u;
constexpr uint32_t FUN_10226080 = 0x10226080u;
constexpr uint32_t FUN_10226180 = 0x10226180u;
constexpr uint32_t FUN_102261f0 = 0x102261F0u;
constexpr uint32_t FUN_10226210 = 0x10226210u;
constexpr uint32_t FUN_10226550 = 0x10226550u;
constexpr uint32_t FUN_10226c80 = 0x10226C80u;
constexpr uint32_t FUN_10226e30 = 0x10226E30u;
constexpr uint32_t FUN_10227080 = 0x10227080u;
constexpr uint32_t FUN_102270f0 = 0x102270F0u;
constexpr uint32_t FUN_10227100 = 0x10227100u;
constexpr uint32_t FUN_10227130 = 0x10227130u;
constexpr uint32_t FUN_102271b0 = 0x102271B0u;
constexpr uint32_t FUN_102271d0 = 0x102271D0u;
constexpr uint32_t FUN_102278b0 = 0x102278B0u;
constexpr uint32_t FUN_10227920 = 0x10227920u;
constexpr uint32_t FUN_10227930 = 0x10227930u;
constexpr uint32_t FUN_102281c0 = 0x102281C0u;
constexpr uint32_t FUN_10228200 = 0x10228200u;
constexpr uint32_t FUN_10229670 = 0x10229670u;
constexpr uint32_t FUN_1022af80 = 0x1022AF80u;
constexpr uint32_t FUN_1022afb0 = 0x1022AFB0u;
constexpr uint32_t FUN_1022aff0 = 0x1022AFF0u;
constexpr uint32_t FUN_1022b260 = 0x1022B260u;
constexpr uint32_t FUN_1022b3b0 = 0x1022B3B0u;
constexpr uint32_t FUN_1022bd50 = 0x1022BD50u;
constexpr uint32_t FUN_1022bda0 = 0x1022BDA0u;
constexpr uint32_t FUN_1022be10 = 0x1022BE10u;
constexpr uint32_t FUN_1022be30 = 0x1022BE30u;
constexpr uint32_t FUN_1022bfb0 = 0x1022BFB0u;
constexpr uint32_t FUN_1022c350 = 0x1022C350u;
constexpr uint32_t FUN_1022ed00 = 0x1022ED00u;
constexpr uint32_t FUN_1022ed30 = 0x1022ED30u;
constexpr uint32_t FUN_1022ed70 = 0x1022ED70u;
constexpr uint32_t FUN_1022edf0 = 0x1022EDF0u;
constexpr uint32_t FUN_1022eee0 = 0x1022EEE0u;
constexpr uint32_t FUN_1022f240 = 0x1022F240u;
constexpr uint32_t FUN_1022f610 = 0x1022F610u;
constexpr uint32_t FUN_102303b0 = 0x102303B0u;
constexpr uint32_t FUN_10230410 = 0x10230410u;
constexpr uint32_t FUN_10233be0 = 0x10233BE0u;
constexpr uint32_t FUN_102340a0 = 0x102340A0u;
constexpr uint32_t FUN_10234910 = 0x10234910u;
constexpr uint32_t FUN_10234a70 = 0x10234A70u;
constexpr uint32_t Ui_KeyPrompt_Init = 0x10239460u;
constexpr uint32_t Ui_KeyPrompt_Init_v1 = 0x10239460u;
constexpr uint32_t FUN_10239490 = 0x10239490u;
constexpr uint32_t FUN_1023be10 = 0x1023BE10u;
constexpr uint32_t FUN_10240860 = 0x10240860u;
constexpr uint32_t FUN_10244f20 = 0x10244F20u;
constexpr uint32_t FUN_10245040 = 0x10245040u;
constexpr uint32_t sub_10245B50 = 0x10245B50u;
constexpr uint32_t FUN_10245bb0 = 0x10245BB0u;
constexpr uint32_t FUN_10245be0 = 0x10245BE0u;
constexpr uint32_t FUN_10245f30 = 0x10245F30u;
constexpr uint32_t FUN_10246000 = 0x10246000u;
constexpr uint32_t FUN_10246160 = 0x10246160u;
constexpr uint32_t FUN_10246320 = 0x10246320u;
constexpr uint32_t FUN_102463c0 = 0x102463C0u;
constexpr uint32_t FUN_102465e0 = 0x102465E0u;
constexpr uint32_t FUN_10246770 = 0x10246770u;
constexpr uint32_t FUN_102467a0 = 0x102467A0u;
constexpr uint32_t FUN_102467f0 = 0x102467F0u;
constexpr uint32_t FUN_1024c060 = 0x1024C060u;
constexpr uint32_t FUN_1024c930 = 0x1024C930u;
constexpr uint32_t StAvatarInit = 0x1024CF50u;
constexpr uint32_t sub_1024D130 = 0x1024D130u;
constexpr uint32_t StAvatar_Teardown = 0x1024D180u;
constexpr uint32_t StAvatar_ReleaseResources = 0x1024D2C0u;
constexpr uint32_t FUN_1024d400 = 0x1024D400u;
constexpr uint32_t FUN_1024d470 = 0x1024D470u;
constexpr uint32_t FUN_1024d4a0 = 0x1024D4A0u;
constexpr uint32_t FUN_1024d540 = 0x1024D540u;
constexpr uint32_t FUN_1024d5c0 = 0x1024D5C0u;
constexpr uint32_t FUN_1024d610 = 0x1024D610u;
constexpr uint32_t FUN_1024d6b0 = 0x1024D6B0u;
constexpr uint32_t FUN_1024d740 = 0x1024D740u;
constexpr uint32_t StAvatar_StopAndFreeBuffers = 0x1024D790u;
constexpr uint32_t FUN_1024d850 = 0x1024D850u;
constexpr uint32_t FUN_1024d9d0 = 0x1024D9D0u;
constexpr uint32_t FUN_1024da10 = 0x1024DA10u;
constexpr uint32_t FUN_1024db60 = 0x1024DB60u;
constexpr uint32_t FUN_1024dd10 = 0x1024DD10u;
constexpr uint32_t FUN_1024de20 = 0x1024DE20u;
constexpr uint32_t FUN_1024dfe0 = 0x1024DFE0u;
constexpr uint32_t FUN_1024e280 = 0x1024E280u;
constexpr uint32_t FUN_1024e720 = 0x1024E720u;
constexpr uint32_t FUN_1024f2c0 = 0x1024F2C0u;
constexpr uint32_t FUN_1024f5e0 = 0x1024F5E0u;
constexpr uint32_t StAvatar_SetViewFromDancerCamera = 0x1024F900u;
constexpr uint32_t FUN_1024fc10 = 0x1024FC10u;
constexpr uint32_t FUN_1024fdf0 = 0x1024FDF0u;
constexpr uint32_t FUN_1024fec0 = 0x1024FEC0u;
constexpr uint32_t FUN_1024ff50 = 0x1024FF50u;
constexpr uint32_t FUN_10250030 = 0x10250030u;
constexpr uint32_t FUN_10250190 = 0x10250190u;
constexpr uint32_t StAvatarRender = 0x102501C0u;
constexpr uint32_t sub_102502D0 = 0x102502D0u;
constexpr uint32_t StAvatarFini = 0x102504D0u;
constexpr uint32_t FUN_10250580 = 0x10250580u;
constexpr uint32_t FUN_10250660 = 0x10250660u;
constexpr uint32_t FUN_102506c0 = 0x102506C0u;
constexpr uint32_t FUN_10250760 = 0x10250760u;
constexpr uint32_t FUN_10250790 = 0x10250790u;
constexpr uint32_t FUN_102507c0 = 0x102507C0u;
constexpr uint32_t FUN_10250800 = 0x10250800u;
constexpr uint32_t FUN_102509c0 = 0x102509C0u;
constexpr uint32_t FUN_10250a40 = 0x10250A40u;
constexpr uint32_t FUN_10250b10 = 0x10250B10u;
constexpr uint32_t FUN_10250c00 = 0x10250C00u;
constexpr uint32_t sub_10250D80 = 0x10250D80u;
constexpr uint32_t FUN_10250db0 = 0x10250DB0u;
constexpr uint32_t StChannelItemResolve = 0x10250F10u;
constexpr uint32_t FUN_10251070 = 0x10251070u;
constexpr uint32_t FUN_10251160 = 0x10251160u;
constexpr uint32_t FUN_10251180 = 0x10251180u;
constexpr uint32_t FUN_102511b0 = 0x102511B0u;
constexpr uint32_t FUN_10251200 = 0x10251200u;
constexpr uint32_t StChannelItemClear = 0x102512B0u;
constexpr uint32_t FUN_10251320 = 0x10251320u;
constexpr uint32_t FUN_10251370 = 0x10251370u;
constexpr uint32_t FUN_10251410 = 0x10251410u;
constexpr uint32_t StDancer_Update = 0x10251500u;
constexpr uint32_t sub_102515F0 = 0x102515F0u;
constexpr uint32_t FUN_10251620 = 0x10251620u;
constexpr uint32_t FUN_102516b0 = 0x102516B0u;
constexpr uint32_t FUN_10251720 = 0x10251720u;
constexpr uint32_t FUN_10251750 = 0x10251750u;
constexpr uint32_t FUN_10251780 = 0x10251780u;
constexpr uint32_t StDancer_SetJobLock = 0x10251890u;
constexpr uint32_t FUN_10251920 = 0x10251920u;
constexpr uint32_t sub_10251960 = 0x10251960u;
constexpr uint32_t Dancer_InitDevice = 0x10251A80u;
constexpr uint32_t FUN_10251ce0 = 0x10251CE0u;
constexpr uint32_t sub_102524B0 = 0x102524B0u;
constexpr uint32_t StModelItemClear = 0x102524E0u;
constexpr uint32_t FUN_10252610 = 0x10252610u;
constexpr uint32_t FUN_102528d0 = 0x102528D0u;
constexpr uint32_t FUN_10252a00 = 0x10252A00u;
constexpr uint32_t FUN_10252a40 = 0x10252A40u;
constexpr uint32_t FUN_10252ac0 = 0x10252AC0u;
constexpr uint32_t FUN_10252b50 = 0x10252B50u;
constexpr uint32_t FUN_10252c10 = 0x10252C10u;
constexpr uint32_t FUN_10252c30 = 0x10252C30u;
constexpr uint32_t sub_10252C80 = 0x10252C80u;
constexpr uint32_t StTrigger_ProcessOpcodesUpToTime = 0x10252CC0u;
constexpr uint32_t FUN_10252ea0 = 0x10252EA0u;
constexpr uint32_t StTrigger_RegisterCallbacks = 0x10252F00u;
constexpr uint32_t StTrigger_AllocFromPool = 0x10252F70u;
constexpr uint32_t FUN_10253160 = 0x10253160u;
constexpr uint32_t sqopSystemActivate = 0x102531B0u;
constexpr uint32_t FUN_10253210 = 0x10253210u;
constexpr uint32_t FUN_102532a0 = 0x102532A0u;
constexpr uint32_t FUN_102532c0 = 0x102532C0u;
constexpr uint32_t sub_102532E0 = 0x102532E0u;
constexpr uint32_t FUN_10253330 = 0x10253330u;
constexpr uint32_t FUN_10253360 = 0x10253360u;
constexpr uint32_t FUN_102534b0 = 0x102534B0u;
constexpr uint32_t FUN_102535f0 = 0x102535F0u;
constexpr uint32_t FUN_10253620 = 0x10253620u;
constexpr uint32_t FUN_10253670 = 0x10253670u;
constexpr uint32_t sub_10253670 = 0x10253670u;
constexpr uint32_t FUN_102536a0 = 0x102536A0u;
constexpr uint32_t FUN_102572e0 = 0x102572E0u;
constexpr uint32_t FUN_10257310 = 0x10257310u;
constexpr uint32_t FUN_102573c0 = 0x102573C0u;
constexpr uint32_t FUN_102573e0 = 0x102573E0u;
constexpr uint32_t FUN_102574f0 = 0x102574F0u;
constexpr uint32_t FUN_10257550 = 0x10257550u;
constexpr uint32_t FUN_10257590 = 0x10257590u;
constexpr uint32_t FUN_102575b0 = 0x102575B0u;
constexpr uint32_t FUN_102575d0 = 0x102575D0u;
constexpr uint32_t FUN_102575e0 = 0x102575E0u;
constexpr uint32_t FUN_10257670 = 0x10257670u;
constexpr uint32_t FUN_102577c0 = 0x102577C0u;
constexpr uint32_t FUN_10257800 = 0x10257800u;
constexpr uint32_t FUN_10257830 = 0x10257830u;
constexpr uint32_t thunk_FUN_10257850 = 0x10257840u;
constexpr uint32_t FUN_10257850 = 0x10257850u;
constexpr uint32_t FUN_10257860 = 0x10257860u;
constexpr uint32_t FUN_10257870 = 0x10257870u;
constexpr uint32_t FUN_102578b0 = 0x102578B0u;
constexpr uint32_t FUN_10257920 = 0x10257920u;
constexpr uint32_t FUN_10257950 = 0x10257950u;
constexpr uint32_t FUN_10257980 = 0x10257980u;
constexpr uint32_t FUN_102579b0 = 0x102579B0u;
constexpr uint32_t FUN_10257af0 = 0x10257AF0u;
constexpr uint32_t FUN_10257b10 = 0x10257B10u;
constexpr uint32_t FUN_10257b60 = 0x10257B60u;
constexpr uint32_t FUN_10257ba0 = 0x10257BA0u;
constexpr uint32_t FUN_10257bc0 = 0x10257BC0u;
constexpr uint32_t FUN_10257bd0 = 0x10257BD0u;
constexpr uint32_t FUN_10257bf0 = 0x10257BF0u;
constexpr uint32_t FUN_10257c60 = 0x10257C60u;
constexpr uint32_t FUN_10257c70 = 0x10257C70u;
constexpr uint32_t FUN_10257c80 = 0x10257C80u;
constexpr uint32_t FUN_10257ff0 = 0x10257FF0u;
constexpr uint32_t FUN_10258150 = 0x10258150u;
constexpr uint32_t FUN_102581d0 = 0x102581D0u;
constexpr uint32_t FUN_10258430 = 0x10258430u;
constexpr uint32_t FUN_10258600 = 0x10258600u;
constexpr uint32_t FUN_102586d0 = 0x102586D0u;
constexpr uint32_t FUN_102586f0 = 0x102586F0u;
constexpr uint32_t FUN_10258740 = 0x10258740u;
constexpr uint32_t FUN_10258830 = 0x10258830u;
constexpr uint32_t FUN_102589f0 = 0x102589F0u;
constexpr uint32_t FUN_10258ad0 = 0x10258AD0u;
constexpr uint32_t FUN_10258bb0 = 0x10258BB0u;
constexpr uint32_t FUN_10258d00 = 0x10258D00u;
constexpr uint32_t FUN_10258d70 = 0x10258D70u;
constexpr uint32_t FUN_10258e10 = 0x10258E10u;
constexpr uint32_t FUN_10258e50 = 0x10258E50u;
constexpr uint32_t FUN_10258ed0 = 0x10258ED0u;
constexpr uint32_t FUN_10258f00 = 0x10258F00u;
constexpr uint32_t FUN_10258f60 = 0x10258F60u;
constexpr uint32_t FUN_10259100 = 0x10259100u;
constexpr uint32_t FUN_10259130 = 0x10259130u;
constexpr uint32_t FUN_10259150 = 0x10259150u;
constexpr uint32_t FUN_10259de0 = 0x10259DE0u;
constexpr uint32_t FUN_1025a580 = 0x1025A580u;
constexpr uint32_t FUN_1025a5a0 = 0x1025A5A0u;
constexpr uint32_t FUN_1025a610 = 0x1025A610u;
constexpr uint32_t FUN_1025a6c0 = 0x1025A6C0u;
constexpr uint32_t FUN_1025a6f0 = 0x1025A6F0u;
constexpr uint32_t FUN_1025a730 = 0x1025A730u;
constexpr uint32_t FUN_1025a760 = 0x1025A760u;
constexpr uint32_t FUN_1025a820 = 0x1025A820u;
constexpr uint32_t FUN_1025a860 = 0x1025A860u;
constexpr uint32_t FUN_1025a890 = 0x1025A890u;
constexpr uint32_t FUN_1025a8c0 = 0x1025A8C0u;
constexpr uint32_t FUN_1025a930 = 0x1025A930u;
constexpr uint32_t FUN_1025aa90 = 0x1025AA90u;
constexpr uint32_t FUN_1025b100 = 0x1025B100u;
constexpr uint32_t FUN_1025b4f0 = 0x1025B4F0u;
constexpr uint32_t FUN_1025b560 = 0x1025B560u;
constexpr uint32_t sub_1025B560 = 0x1025B560u;
constexpr uint32_t FUN_1025b5a0 = 0x1025B5A0u;
constexpr uint32_t FUN_1025b5e0 = 0x1025B5E0u;
constexpr uint32_t FUN_1025b680 = 0x1025B680u;
constexpr uint32_t FUN_1025bb00 = 0x1025BB00u;
constexpr uint32_t FUN_1025bbe0 = 0x1025BBE0u;
constexpr uint32_t FUN_1025bde0 = 0x1025BDE0u;
constexpr uint32_t FUN_1025be20 = 0x1025BE20u;
constexpr uint32_t FUN_1025be80 = 0x1025BE80u;
constexpr uint32_t FUN_1025bfb0 = 0x1025BFB0u;
constexpr uint32_t FUN_1025c0f0 = 0x1025C0F0u;
constexpr uint32_t FUN_1025c0f0_v1 = 0x1025C0F0u;
constexpr uint32_t FUN_1025e020 = 0x1025E020u;
constexpr uint32_t FUN_1025e020_v1 = 0x1025E020u;
constexpr uint32_t FUN_1025e050 = 0x1025E050u;
constexpr uint32_t FUN_1025e090 = 0x1025E090u;
constexpr uint32_t sub_1025E0B0 = 0x1025E0B0u;
constexpr uint32_t sub_1025E0C0 = 0x1025E0C0u;
constexpr uint32_t sub_1025E0E0 = 0x1025E0E0u;
constexpr uint32_t sub_1025E0F0 = 0x1025E0F0u;
constexpr uint32_t FUN_1025e210 = 0x1025E210u;
constexpr uint32_t FUN_1025e230 = 0x1025E230u;
constexpr uint32_t FUN_1025e240 = 0x1025E240u;
constexpr uint32_t FUN_1025e260 = 0x1025E260u;
constexpr uint32_t FUN_1025f2f0 = 0x1025F2F0u;
constexpr uint32_t FUN_1025f640 = 0x1025F640u;
constexpr uint32_t FUN_1025fd50 = 0x1025FD50u;
constexpr uint32_t FUN_1025fe40 = 0x1025FE40u;
constexpr uint32_t FUN_10261210 = 0x10261210u;
constexpr uint32_t FUN_102612d0 = 0x102612D0u;
constexpr uint32_t FUN_10265940 = 0x10265940u;
constexpr uint32_t _dynamic_initializer_for__std__num_put_char_std__ostreambuf_iterator_char_std__char_traits_char_______id___47 = 0x10265960u;
constexpr uint32_t FUN_102659b0 = 0x102659B0u;
constexpr uint32_t FUN_10265b70 = 0x10265B70u;
constexpr uint32_t FUN_10265b90 = 0x10265B90u;
constexpr uint32_t FUN_1026cc30 = 0x1026CC30u;
constexpr uint32_t FUN_1026cca5 = 0x1026CCA5u;
constexpr uint32_t FUN_1026cdf4 = 0x1026CDF4u;
constexpr uint32_t FUN_1026cdf4_v1 = 0x1026CDF4u;
constexpr uint32_t FUN_1026cfa0 = 0x1026CFA0u;
constexpr uint32_t FUN_1026d015 = 0x1026D015u;
constexpr uint32_t FUN_1026d0ca = 0x1026D0CAu;
constexpr uint32_t FUN_1026d179 = 0x1026D179u;
constexpr uint32_t FUN_1026d1b4 = 0x1026D1B4u;
constexpr uint32_t FUN_1026d634 = 0x1026D634u;
constexpr uint32_t FUN_1026d70c = 0x1026D70Cu;
constexpr uint32_t FUN_1026d866 = 0x1026D866u;
constexpr uint32_t FUN_1026d9b1 = 0x1026D9B1u;
constexpr uint32_t FUN_1026db24 = 0x1026DB24u;
constexpr uint32_t FUN_1026dbce = 0x1026DBCEu;
constexpr uint32_t FUN_1026dbce_v1 = 0x1026DBCEu;
constexpr uint32_t FUN_1026dbf0 = 0x1026DBF0u;
constexpr uint32_t FUN_1026dbf0_v1 = 0x1026DBF0u;
constexpr uint32_t FUN_1026dc0d = 0x1026DC0Du;
constexpr uint32_t FUN_1026dc0d_v1 = 0x1026DC0Du;
constexpr uint32_t sqxfXformTD_Init = 0x1026DF00u;
constexpr uint32_t FUN_1026df90 = 0x1026DF90u;
constexpr uint32_t sqxfXformTD_Update = 0x1026E1F0u;
constexpr uint32_t sqxfXformTD_Update_v1 = 0x1026E1F0u;
constexpr uint32_t FUN_1026e2c0 = 0x1026E2C0u;
constexpr uint32_t FUN_1026e2f0 = 0x1026E2F0u;
constexpr uint32_t FUN_1026e350 = 0x1026E350u;
constexpr uint32_t FUN_1026e3d0 = 0x1026E3D0u;
constexpr uint32_t sub_1026E400 = 0x1026E400u;
constexpr uint32_t sub_1026E460 = 0x1026E460u;
constexpr uint32_t FUN_1026e530 = 0x1026E530u;
constexpr uint32_t FUN_1026e590 = 0x1026E590u;
constexpr uint32_t FUN_1026ebf0 = 0x1026EBF0u;
constexpr uint32_t FUN_1026ecc0 = 0x1026ECC0u;
constexpr uint32_t FUN_1026ef00 = 0x1026EF00u;
constexpr uint32_t FUN_1026efb0 = 0x1026EFB0u;
constexpr uint32_t FUN_1026f080 = 0x1026F080u;
constexpr uint32_t FUN_1026f1b0 = 0x1026F1B0u;
constexpr uint32_t FUN_1026f4f0 = 0x1026F4F0u;
constexpr uint32_t FUN_1026f580 = 0x1026F580u;
constexpr uint32_t FUN_1026f7b0 = 0x1026F7B0u;
constexpr uint32_t FUN_1026f900 = 0x1026F900u;
constexpr uint32_t FUN_1026f9a0 = 0x1026F9A0u;
constexpr uint32_t FUN_1026f9e0 = 0x1026F9E0u;
constexpr uint32_t FUN_1026fbb0 = 0x1026FBB0u;
constexpr uint32_t FUN_1026ff80 = 0x1026FF80u;
constexpr uint32_t FUN_102701b0 = 0x102701B0u;
constexpr uint32_t FUN_10270290 = 0x10270290u;
constexpr uint32_t FUN_102703c0 = 0x102703C0u;
constexpr uint32_t FUN_10270410 = 0x10270410u;
constexpr uint32_t FUN_10270690 = 0x10270690u;
constexpr uint32_t FUN_10270840 = 0x10270840u;
constexpr uint32_t FUN_102709e0 = 0x102709E0u;
constexpr uint32_t sub_10270B60 = 0x10270B60u;
constexpr uint32_t FUN_10270ec0 = 0x10270EC0u;
constexpr uint32_t FUN_10270f30 = 0x10270F30u;
constexpr uint32_t FUN_10271030 = 0x10271030u;
constexpr uint32_t FUN_102710b0 = 0x102710B0u;
constexpr uint32_t FUN_102712e0 = 0x102712E0u;
constexpr uint32_t FUN_102715f0 = 0x102715F0u;
constexpr uint32_t FUN_10271630 = 0x10271630u;
constexpr uint32_t sub_102717F0 = 0x102717F0u;
constexpr uint32_t scnFini = 0x10271B40u;
constexpr uint32_t FUN_10271b90 = 0x10271B90u;
constexpr uint32_t FUN_10271c30 = 0x10271C30u;
constexpr uint32_t sub_10271C60 = 0x10271C60u;
constexpr uint32_t sub_10271C80 = 0x10271C80u;
constexpr uint32_t FUN_10271ca0 = 0x10271CA0u;
constexpr uint32_t FUN_10271d00 = 0x10271D00u;
constexpr uint32_t FUN_10271d70 = 0x10271D70u;
constexpr uint32_t scnGetFlags = 0x10271E90u;
constexpr uint32_t scnGetFlags_v1 = 0x10271E90u;
constexpr uint32_t FUN_10271f20 = 0x10271F20u;
constexpr uint32_t FUN_102720a0 = 0x102720A0u;
constexpr uint32_t FUN_102725a0 = 0x102725A0u;
constexpr uint32_t FUN_10272630 = 0x10272630u;
constexpr uint32_t FUN_102726e0 = 0x102726E0u;
constexpr uint32_t FUN_10272750 = 0x10272750u;
constexpr uint32_t FUN_102727f0 = 0x102727F0u;
constexpr uint32_t FUN_10272880 = 0x10272880u;
constexpr uint32_t FUN_102729e0 = 0x102729E0u;
constexpr uint32_t FUN_10272a10 = 0x10272A10u;
constexpr uint32_t FUN_10272ba0 = 0x10272BA0u;
constexpr uint32_t scnCameraGetInfo = 0x10273370u;
constexpr uint32_t scnCameraGetInfo_v1 = 0x10273370u;
constexpr uint32_t FUN_102733b0 = 0x102733B0u;
constexpr uint32_t FUN_10273490 = 0x10273490u;
constexpr uint32_t FUN_10273560 = 0x10273560u;
constexpr uint32_t FUN_102736d0 = 0x102736D0u;
constexpr uint32_t FUN_102736e0 = 0x102736E0u;
constexpr uint32_t FUN_10273810 = 0x10273810u;
constexpr uint32_t sub_102738D0 = 0x102738D0u;
constexpr uint32_t FUN_10273e80 = 0x10273E80u;
constexpr uint32_t FUN_102741f0 = 0x102741F0u;
constexpr uint32_t FUN_102742c0 = 0x102742C0u;
constexpr uint32_t FUN_10274370 = 0x10274370u;
constexpr uint32_t FUN_10274640 = 0x10274640u;
constexpr uint32_t FUN_10274650 = 0x10274650u;
constexpr uint32_t FUN_10274670 = 0x10274670u;
constexpr uint32_t FUN_102746c0 = 0x102746C0u;
constexpr uint32_t FUN_10274870 = 0x10274870u;
constexpr uint32_t FUN_102748f0 = 0x102748F0u;
constexpr uint32_t FUN_102749c0 = 0x102749C0u;
constexpr uint32_t FUN_10274a20 = 0x10274A20u;
constexpr uint32_t PackVec4 = 0x10274D40u;
constexpr uint32_t PackVec4_v1 = 0x10274D40u;
constexpr uint32_t FUN_10275090 = 0x10275090u;
constexpr uint32_t sub_10275090 = 0x10275090u;
constexpr uint32_t sqMemInit = 0x10275150u;
constexpr uint32_t FUN_10275190 = 0x10275190u;
constexpr uint32_t FUN_102755c0 = 0x102755C0u;
constexpr uint32_t FUN_102756a0 = 0x102756A0u;
constexpr uint32_t sqMemPoolRegister = 0x10275B60u;
constexpr uint32_t FUN_10275d40 = 0x10275D40u;
constexpr uint32_t FUN_10275d80 = 0x10275D80u;
constexpr uint32_t FUN_10275e90 = 0x10275E90u;
constexpr uint32_t sqFree = 0x102760F0u;
constexpr uint32_t sqFree_v1 = 0x102760F0u;
constexpr uint32_t sqgxContext_Create = 0x10276260u;
constexpr uint32_t sqgxContext_Init = 0x10276290u;
constexpr uint32_t FUN_10276770 = 0x10276770u;
constexpr uint32_t sqgxContext_Dump = 0x102768E0u;
constexpr uint32_t FUN_10277ab0 = 0x10277AB0u;
constexpr uint32_t FUN_10277b70 = 0x10277B70u;
constexpr uint32_t FUN_10278490 = 0x10278490u;
constexpr uint32_t FUN_10278710 = 0x10278710u;
constexpr uint32_t FUN_10278990 = 0x10278990u;
constexpr uint32_t FUN_10278b50 = 0x10278B50u;
constexpr uint32_t FUN_10278e10 = 0x10278E10u;
constexpr uint32_t FUN_10278e90 = 0x10278E90u;
constexpr uint32_t FUN_10279530 = 0x10279530u;
constexpr uint32_t FUN_102796a0 = 0x102796A0u;
constexpr uint32_t FUN_10279710 = 0x10279710u;
constexpr uint32_t FUN_10279870 = 0x10279870u;
constexpr uint32_t sqmdModel_GetItemName = 0x10279AB0u;
constexpr uint32_t FUN_10279bf0 = 0x10279BF0u;
constexpr uint32_t FUN_1027a220 = 0x1027A220u;
constexpr uint32_t FUN_1027a250 = 0x1027A250u;
constexpr uint32_t thunk_FUN_1027ae50 = 0x1027A6F0u;
constexpr uint32_t FUN_1027a700 = 0x1027A700u;
constexpr uint32_t FUN_1027a8a0 = 0x1027A8A0u;
constexpr uint32_t FUN_1027a9d0 = 0x1027A9D0u;
constexpr uint32_t FUN_1027aae0 = 0x1027AAE0u;
constexpr uint32_t FUN_1027aba0 = 0x1027ABA0u;
constexpr uint32_t FUN_1027abd0 = 0x1027ABD0u;
constexpr uint32_t FUN_1027ac30 = 0x1027AC30u;
constexpr uint32_t FUN_1027ae50 = 0x1027AE50u;
constexpr uint32_t FUN_1027aec0 = 0x1027AEC0u;
constexpr uint32_t FUN_1027aef0 = 0x1027AEF0u;
constexpr uint32_t FUN_1027afb0 = 0x1027AFB0u;
constexpr uint32_t FUN_1027b060 = 0x1027B060u;
constexpr uint32_t FUN_1027b080 = 0x1027B080u;
constexpr uint32_t FUN_1027b170 = 0x1027B170u;
constexpr uint32_t FUN_1027b2c0 = 0x1027B2C0u;
constexpr uint32_t FUN_1027b5a0 = 0x1027B5A0u;
constexpr uint32_t FUN_1027b610 = 0x1027B610u;
constexpr uint32_t FUN_1027c900 = 0x1027C900u;
constexpr uint32_t FUN_1027c980 = 0x1027C980u;
constexpr uint32_t FUN_1027c9d0 = 0x1027C9D0u;
constexpr uint32_t FUN_1027c9d0_v1 = 0x1027C9D0u;
constexpr uint32_t FUN_1027ccf0 = 0x1027CCF0u;
constexpr uint32_t FUN_1027cd30 = 0x1027CD30u;
constexpr uint32_t FUN_1027cfd0 = 0x1027CFD0u;
constexpr uint32_t FUN_1027d110 = 0x1027D110u;
constexpr uint32_t FUN_1027d650 = 0x1027D650u;
constexpr uint32_t FUN_1027dae0 = 0x1027DAE0u;
constexpr uint32_t FUN_1027db80 = 0x1027DB80u;
constexpr uint32_t FUN_1027dcc0 = 0x1027DCC0u;
constexpr uint32_t FUN_1027dd70 = 0x1027DD70u;
constexpr uint32_t FUN_1027dd90 = 0x1027DD90u;
constexpr uint32_t FUN_1027df70 = 0x1027DF70u;
constexpr uint32_t FUN_1027e060 = 0x1027E060u;
constexpr uint32_t FUN_1027e0d0 = 0x1027E0D0u;
constexpr uint32_t FUN_1027e150 = 0x1027E150u;
constexpr uint32_t FUN_1027e190 = 0x1027E190u;
constexpr uint32_t FUN_1027e3b0 = 0x1027E3B0u;
constexpr uint32_t FUN_1027e440 = 0x1027E440u;
constexpr uint32_t FUN_1027e450 = 0x1027E450u;
constexpr uint32_t FUN_1027e490 = 0x1027E490u;
constexpr uint32_t thunk_FUN_1027ebd0 = 0x1027EAF0u;
constexpr uint32_t FUN_1027eb00 = 0x1027EB00u;
constexpr uint32_t FUN_1027eb00_v1 = 0x1027EB00u;
constexpr uint32_t M4fSet = 0x1027EB10u;
constexpr uint32_t M4fSet_v1 = 0x1027EB10u;
constexpr uint32_t FUN_1027eb50 = 0x1027EB50u;
constexpr uint32_t FUN_1027ebd0 = 0x1027EBD0u;
constexpr uint32_t M4fTransform = 0x1027EF20u;
constexpr uint32_t M4fTransform_v1 = 0x1027EF20u;
constexpr uint32_t FUN_1027ef80 = 0x1027EF80u;
constexpr uint32_t Mat4x4_TransformVec4 = 0x1027EFE0u;
constexpr uint32_t M4fTranspose = 0x1027F070u;
constexpr uint32_t M4fTranspose_v1 = 0x1027F070u;
constexpr uint32_t sqMatrix4_1027F0E0 = 0x1027F0E0u;
constexpr uint32_t sqMatrix4_1027F0E0_v1 = 0x1027F0E0u;
constexpr uint32_t sqMatrix4_1027FCC0 = 0x1027FCC0u;
constexpr uint32_t sqMatrix4_10280140 = 0x10280140u;
constexpr uint32_t sqMatrix4_10280140_v1 = 0x10280140u;
constexpr uint32_t FUN_102806b0 = 0x102806B0u;
constexpr uint32_t sqgxSprite = 0x10280DE0u;
constexpr uint32_t sqgxSprite_v1 = 0x10280DE0u;
constexpr uint32_t FUN_102813b0 = 0x102813B0u;
constexpr uint32_t ___setargv_3 = 0x10281500u;
constexpr uint32_t sqgxTextureCreateFromImage = 0x10281510u;
constexpr uint32_t sqgxTextureAddZ = 0x102816E0u;
constexpr uint32_t sqgxCeilLog2 = 0x10281810u;
constexpr uint32_t sqgxTextureAdd = 0x10281830u;
constexpr uint32_t sqgxTextureAdd_v1 = 0x10281830u;
constexpr uint32_t FUN_102819c0 = 0x102819C0u;
constexpr uint32_t FUN_10281a70 = 0x10281A70u;
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
constexpr uint32_t FUN_10282590 = 0x10282590u;
constexpr uint32_t nullsub_59 = 0x10282630u;
constexpr uint32_t sqgxBlendAdd = 0x102826C0u;
constexpr uint32_t FUN_102827d0 = 0x102827D0u;
constexpr uint32_t sqgxBlendSub = 0x10282910u;
constexpr uint32_t sqgxBlendLin = 0x10282A30u;
constexpr uint32_t sqgxBlendTrsp = 0x10282B90u;
constexpr uint32_t sqgxBlendTrsp_v1 = 0x10282B90u;
constexpr uint32_t sqgxBlendAddDestAlpha = 0x10282DC0u;
constexpr uint32_t sqgxBlendZeroSrcAlpha = 0x10282FF0u;
constexpr uint32_t sqgxBlendMul = 0x10283110u;
constexpr uint32_t sqgxBlendOff = 0x10283220u;
constexpr uint32_t sqgxBlendOff_v1 = 0x10283220u;
constexpr uint32_t nullsub_31 = 0x10283240u;
constexpr uint32_t nullsub_31_v1 = 0x10283240u;
constexpr uint32_t ___setargv_9 = 0x10283250u;
constexpr uint32_t FUN_10283260 = 0x10283260u;
constexpr uint32_t FUN_10283280 = 0x10283280u;
constexpr uint32_t FUN_10283300 = 0x10283300u;
constexpr uint32_t sqgxColor = 0x10283330u;
constexpr uint32_t sqgxColor_v1 = 0x10283330u;
constexpr uint32_t sqgxAmbient = 0x10283380u;
constexpr uint32_t sqgxAmbient_v1 = 0x10283380u;
constexpr uint32_t FUN_102833a0 = 0x102833A0u;
constexpr uint32_t FUN_102833c0 = 0x102833C0u;
constexpr uint32_t sqgxColorUpdate = 0x10283420u;
constexpr uint32_t sqgxColorUpdate_v1 = 0x10283420u;
constexpr uint32_t ___setargv_16 = 0x10283870u;
constexpr uint32_t sqgxPixelSize = 0x102838A0u;
constexpr uint32_t FUN_10283970 = 0x10283970u;
constexpr uint32_t sqgxViewport = 0x10283A10u;
constexpr uint32_t sqgxPerspective = 0x10283C10u;
constexpr uint32_t sqgxMatrixCamera = 0x10283D60u;
constexpr uint32_t sqgxMatrixCamera_v1 = 0x10283D60u;
constexpr uint32_t FUN_10283da0 = 0x10283DA0u;
constexpr uint32_t FUN_10283de0 = 0x10283DE0u;
constexpr uint32_t sqgxMatrixShape = 0x10283E00u;
constexpr uint32_t sqgxMatrixShape_v1 = 0x10283E00u;
constexpr uint32_t FUN_10283f20 = 0x10283F20u;
constexpr uint32_t sqgxPassAdd = 0x10283F70u;
constexpr uint32_t sqgxPassAdd_v1 = 0x10283F70u;
constexpr uint32_t sqgxPass = 0x10283FC0u;
constexpr uint32_t FUN_10284000 = 0x10284000u;
constexpr uint32_t sqgxMatrixUpdate = 0x10284410u;
constexpr uint32_t ___setargv_24 = 0x10284890u;
constexpr uint32_t sqgxVertexShaderCreate = 0x10284950u;
constexpr uint32_t sqgxShader = 0x10284B20u;
constexpr uint32_t sqgxShader_v1 = 0x10284B20u;
constexpr uint32_t nullsub_32 = 0x10284EB0u;
constexpr uint32_t j_sqgxVertexShaderSelect = 0x10284EC0u;
constexpr uint32_t FUN_10284f20 = 0x10284F20u;
constexpr uint32_t FUN_10284f50 = 0x10284F50u;
constexpr uint32_t FUN_10284fd0 = 0x10284FD0u;
constexpr uint32_t FUN_10285000 = 0x10285000u;
constexpr uint32_t FUN_10285740 = 0x10285740u;
constexpr uint32_t FUN_102857d0 = 0x102857D0u;
constexpr uint32_t FUN_102857e0 = 0x102857E0u;
constexpr uint32_t FUN_102857f0 = 0x102857F0u;
constexpr uint32_t sqDrawableDraw = 0x10285850u;
constexpr uint32_t sqDrawableDraw_v1 = 0x10285850u;
constexpr uint32_t sqDebugPrintf = 0x10285890u;
constexpr uint32_t sqDebugPrintf_v1 = 0x10285890u;
constexpr uint32_t sqDebugPrintf_v2 = 0x10285890u;
constexpr uint32_t sqDebugPrintf_v3 = 0x10285890u;
constexpr uint32_t sqDebugPrintf_v4 = 0x10285890u;
constexpr uint32_t FUN_10288660 = 0x10288660u;
constexpr uint32_t FUN_10288680 = 0x10288680u;
constexpr uint32_t FUN_10289620 = 0x10289620u;
constexpr uint32_t FUN_10289670 = 0x10289670u;
constexpr uint32_t FUN_102896a0 = 0x102896A0u;
constexpr uint32_t FUN_10289b40 = 0x10289B40u;
constexpr uint32_t FUN_10289ba0 = 0x10289BA0u;
constexpr uint32_t sqopOpcodeGet = 0x10289EA0u;
constexpr uint32_t sqopTrackSetIndex = 0x10289EF0u;
constexpr uint32_t sqopTrackAdvance = 0x10289F10u;
constexpr uint32_t FUN_10289f50 = 0x10289F50u;
constexpr uint32_t FUN_10289fb0 = 0x10289FB0u;
constexpr uint32_t FUN_1028a040 = 0x1028A040u;
constexpr uint32_t FUN_1028a0e0 = 0x1028A0E0u;
constexpr uint32_t FUN_1028a0f0 = 0x1028A0F0u;
constexpr uint32_t sub_1028A730 = 0x1028A730u;
constexpr uint32_t sub_1028A770 = 0x1028A770u;
constexpr uint32_t sub_1028A790 = 0x1028A790u;
constexpr uint32_t sqgxShapeBegin = 0x1028AFE0u;
constexpr uint32_t sub_1028BB10 = 0x1028BB10u;
constexpr uint32_t sub_1028BB70 = 0x1028BB70u;
constexpr uint32_t sub_1028BBD0 = 0x1028BBD0u;
constexpr uint32_t FUN_1028d6d0 = 0x1028D6D0u;
constexpr uint32_t FUN_1028d6f0 = 0x1028D6F0u;
constexpr uint32_t FUN_1028d750 = 0x1028D750u;
constexpr uint32_t sqcoConnectorPrint = 0x1028D770u;
constexpr uint32_t FUN_1028d990 = 0x1028D990u;
constexpr uint32_t FUN_1028d9a0 = 0x1028D9A0u;
constexpr uint32_t FUN_1028d9c0 = 0x1028D9C0u;
constexpr uint32_t FUN_1028dbb0 = 0x1028DBB0u;
constexpr uint32_t FUN_1028dc00 = 0x1028DC00u;
constexpr uint32_t FUN_1028dc20 = 0x1028DC20u;
constexpr uint32_t FUN_1028dca0 = 0x1028DCA0u;
constexpr uint32_t FUN_1028de10 = 0x1028DE10u;
constexpr uint32_t FUN_1028e180 = 0x1028E180u;
constexpr uint32_t FUN_1028e1b0 = 0x1028E1B0u;
constexpr uint32_t FUN_1028e350 = 0x1028E350u;
constexpr uint32_t FUN_1028ec30 = 0x1028EC30u;
constexpr uint32_t FUN_1028ec50 = 0x1028EC50u;
constexpr uint32_t FUN_1028ecc0 = 0x1028ECC0u;
constexpr uint32_t FUN_1028ece0 = 0x1028ECE0u;
constexpr uint32_t FUN_1028ed00 = 0x1028ED00u;
constexpr uint32_t FUN_1028ed50 = 0x1028ED50u;
constexpr uint32_t FUN_1028ede0 = 0x1028EDE0u;
constexpr uint32_t FUN_1028efb0 = 0x1028EFB0u;
constexpr uint32_t FUN_1028f130 = 0x1028F130u;
constexpr uint32_t FUN_1028f220 = 0x1028F220u;
constexpr uint32_t FUN_1028f2e0 = 0x1028F2E0u;
constexpr uint32_t FUN_1028f4a0 = 0x1028F4A0u;
constexpr uint32_t FUN_1028f4c0 = 0x1028F4C0u;
constexpr uint32_t sqsnScene_1028F5F0 = 0x1028F5F0u;
constexpr uint32_t sqsnScene_1028F5F0_v1 = 0x1028F5F0u;
constexpr uint32_t FUN_1028f6e0 = 0x1028F6E0u;
constexpr uint32_t sqsnSceneDraw = 0x1028F8C0u;
constexpr uint32_t sqsnSceneLayerDraw = 0x1028FDD0u;
constexpr uint32_t FUN_1028ff80 = 0x1028FF80u;
constexpr uint32_t sqsnSceneLayerDrawShadowed = 0x1028FFF0u;
constexpr uint32_t sqsnSceneLayerSnapShotComposite = 0x102902C0u;
constexpr uint32_t nullsub_60 = 0x10290430u;
constexpr uint32_t sqsnSceneLayerSort = 0x10290440u;
constexpr uint32_t FUN_10290610 = 0x10290610u;
constexpr uint32_t FUN_102907c0 = 0x102907C0u;
constexpr uint32_t FUN_10290840 = 0x10290840u;
constexpr uint32_t FUN_102908e0 = 0x102908E0u;
constexpr uint32_t FUN_10290910 = 0x10290910u;
constexpr uint32_t FUN_102910e0 = 0x102910E0u;
constexpr uint32_t FUN_10291110 = 0x10291110u;
constexpr uint32_t FUN_10291170 = 0x10291170u;
constexpr uint32_t sqrdCamera_Apply = 0x102911E0u;
constexpr uint32_t sqrdCamera_Apply_v1 = 0x102911E0u;
constexpr uint32_t FUN_102912f0 = 0x102912F0u;
constexpr uint32_t FUN_10291390 = 0x10291390u;
constexpr uint32_t FUN_10291430 = 0x10291430u;
constexpr uint32_t FUN_10291460 = 0x10291460u;
constexpr uint32_t FUN_10291470 = 0x10291470u;
constexpr uint32_t FUN_10291ba0 = 0x10291BA0u;
constexpr uint32_t FUN_10291c20 = 0x10291C20u;
constexpr uint32_t FUN_10291c50 = 0x10291C50u;
constexpr uint32_t FUN_10291c60 = 0x10291C60u;
constexpr uint32_t FUN_10291fe0 = 0x10291FE0u;
constexpr uint32_t FUN_10292130 = 0x10292130u;
constexpr uint32_t FUN_10292490 = 0x10292490u;
constexpr uint32_t FUN_102924f0 = 0x102924F0u;
constexpr uint32_t FUN_10292520 = 0x10292520u;
constexpr uint32_t FUN_102925a0 = 0x102925A0u;
constexpr uint32_t FUN_10292740 = 0x10292740u;
constexpr uint32_t FUN_10293000 = 0x10293000u;
constexpr uint32_t FUN_10293f00 = 0x10293F00u;
constexpr uint32_t FUN_10293f80 = 0x10293F80u;
constexpr uint32_t FUN_10293fa0 = 0x10293FA0u;
constexpr uint32_t FUN_10294000 = 0x10294000u;
constexpr uint32_t sqmoFrameChannel_Dump = 0x102940A0u;
constexpr uint32_t FUN_102944e0 = 0x102944E0u;
constexpr uint32_t FUN_102957b0 = 0x102957B0u;
constexpr uint32_t FUN_102957d0 = 0x102957D0u;
constexpr uint32_t FUN_10295830 = 0x10295830u;
constexpr uint32_t sqmoKeyChannel_Dump = 0x102958B0u;
constexpr uint32_t FUN_10295a20 = 0x10295A20u;
constexpr uint32_t FUN_10295e70 = 0x10295E70u;
constexpr uint32_t FUN_10295e90 = 0x10295E90u;
constexpr uint32_t FUN_10295ef0 = 0x10295EF0u;
constexpr uint32_t FUN_10295f30 = 0x10295F30u;
constexpr uint32_t FUN_102960a0 = 0x102960A0u;
constexpr uint32_t FUN_10296410 = 0x10296410u;
constexpr uint32_t FUN_102967e0 = 0x102967E0u;
constexpr uint32_t FUN_102972d0 = 0x102972D0u;
constexpr uint32_t sub_10297320 = 0x10297320u;
constexpr uint32_t sqsnShadowVolumeExtrudeTriList = 0x102979E0u;
constexpr uint32_t sqsnShadowVolumeExtrudeTriStrip = 0x10297B10u;
constexpr uint32_t sqsnShadowVolumeSetPass = 0x10297CC0u;
constexpr uint32_t sqsnShadowComputeFacing = 0x10297F90u;
constexpr uint32_t sqsnShadowVolumeBeginVB = 0x102981E0u;
constexpr uint32_t sqgxShadowPlaneMatrix = 0x10298350u;
constexpr uint32_t FUN_102984b0 = 0x102984B0u;
constexpr uint32_t FUN_10298570 = 0x10298570u;
constexpr uint32_t FUN_10298b30 = 0x10298B30u;
constexpr uint32_t FUN_10298b60 = 0x10298B60u;
constexpr uint32_t FUN_10298b90 = 0x10298B90u;
constexpr uint32_t sqrdTexturePrint = 0x10298C10u;
constexpr uint32_t FUN_10298c50 = 0x10298C50u;
constexpr uint32_t FUN_10298e10 = 0x10298E10u;
constexpr uint32_t FUN_102999a0 = 0x102999A0u;
constexpr uint32_t FUN_1029a000 = 0x1029A000u;
constexpr uint32_t FUN_1029a060 = 0x1029A060u;
constexpr uint32_t FUN_1029a0e0 = 0x1029A0E0u;
constexpr uint32_t FUN_1029a330 = 0x1029A330u;
constexpr uint32_t FUN_1029a450 = 0x1029A450u;
constexpr uint32_t FUN_1029b120 = 0x1029B120u;
constexpr uint32_t FUN_1029b1a0 = 0x1029B1A0u;
constexpr uint32_t FUN_1029b5c0 = 0x1029B5C0u;
constexpr uint32_t FUN_1029b620 = 0x1029B620u;
constexpr uint32_t FUN_1029bb50 = 0x1029BB50u;
constexpr uint32_t FUN_1029bcd0 = 0x1029BCD0u;
constexpr uint32_t FUN_1029bd40 = 0x1029BD40u;
constexpr uint32_t sub_1029BD50 = 0x1029BD50u;
constexpr uint32_t sub_1029BF50 = 0x1029BF50u;
constexpr uint32_t FUN_1029c260 = 0x1029C260u;
constexpr uint32_t FUN_1029c460 = 0x1029C460u;
constexpr uint32_t sub_1029C480 = 0x1029C480u;
constexpr uint32_t sub_1029C610 = 0x1029C610u;
constexpr uint32_t sub_1029C950 = 0x1029C950u;
constexpr uint32_t FUN_102a1f20 = 0x102A1F20u;
constexpr uint32_t FUN_102a1fa0 = 0x102A1FA0u;
constexpr uint32_t FUN_102a2000 = 0x102A2000u;
constexpr uint32_t FUN_102a2020 = 0x102A2020u;
constexpr uint32_t FUN_102a2080 = 0x102A2080u;
constexpr uint32_t sqshDispListPrint = 0x102A20D0u;
constexpr uint32_t FUN_102a2130 = 0x102A2130u;
constexpr uint32_t FUN_102a2160 = 0x102A2160u;
constexpr uint32_t FUN_102a2190 = 0x102A2190u;
constexpr uint32_t sqsnPlanarShadowDraw = 0x102A2530u;
constexpr uint32_t FUN_102a2720 = 0x102A2720u;
constexpr uint32_t FUN_102a2a30 = 0x102A2A30u;
constexpr uint32_t FUN_102a2a50 = 0x102A2A50u;
constexpr uint32_t FUN_102a2c00 = 0x102A2C00u;
constexpr uint32_t FUN_102a3360 = 0x102A3360u;
constexpr uint32_t FUN_102a3390 = 0x102A3390u;
constexpr uint32_t FUN_102a33f0 = 0x102A33F0u;
constexpr uint32_t FUN_102a34d0 = 0x102A34D0u;
constexpr uint32_t FUN_102a3760 = 0x102A3760u;
constexpr uint32_t FUN_102a3770 = 0x102A3770u;
constexpr uint32_t FUN_102a9520 = 0x102A9520u;
constexpr uint32_t FUN_102a9550 = 0x102A9550u;
constexpr uint32_t FUN_102a95a0 = 0x102A95A0u;
constexpr uint32_t sqVtxPrint = 0x102A96C0u;
constexpr uint32_t FUN_102a9980 = 0x102A9980u;
constexpr uint32_t FUN_102a9ef0 = 0x102A9EF0u;
constexpr uint32_t FUN_102a9f30 = 0x102A9F30u;
constexpr uint32_t FUN_102aa2f0 = 0x102AA2F0u;
constexpr uint32_t FUN_102aa370 = 0x102AA370u;
constexpr uint32_t FUN_102aa420 = 0x102AA420u;
constexpr uint32_t FUN_102aa580 = 0x102AA580u;
constexpr uint32_t FUN_102acb80 = 0x102ACB80u;
constexpr uint32_t FUN_102acbe0 = 0x102ACBE0u;
constexpr uint32_t sqshPointsPrint = 0x102ACC30u;
constexpr uint32_t FUN_102acf40 = 0x102ACF40u;
constexpr uint32_t FUN_102acfc0 = 0x102ACFC0u;
constexpr uint32_t sqshCurvesPrint = 0x102AD090u;
constexpr uint32_t FUN_102ad1d0 = 0x102AD1D0u;
constexpr uint32_t FUN_102ad770 = 0x102AD770u;
constexpr uint32_t FUN_102ad770_v1 = 0x102AD770u;
constexpr uint32_t FUN_102aec50 = 0x102AEC50u;
constexpr uint32_t FUN_102aee90 = 0x102AEE90u;
constexpr uint32_t FUN_102aeeb0 = 0x102AEEB0u;
constexpr uint32_t FUN_102aefe0 = 0x102AEFE0u;
constexpr uint32_t FUN_102af020 = 0x102AF020u;
constexpr uint32_t FUN_102af170 = 0x102AF170u;
constexpr uint32_t FUN_102af2a0 = 0x102AF2A0u;
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
constexpr uint32_t fn_102D2F40 = 0x102D2F40u;
constexpr uint32_t fn_102D2FB9 = 0x102D2FB9u;
constexpr uint32_t fn_102D301F = 0x102D301Fu;
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
constexpr uint32_t FUN_10311386 = 0x10311386u;
constexpr uint32_t FUN_103113c8 = 0x103113C8u;
constexpr uint32_t FUN_103113d3 = 0x103113D3u;
constexpr uint32_t operator_delete_10311BB0 = 0x10311BB0u;
constexpr uint32_t operator_delete_10311BB0_v1 = 0x10311BB0u;
constexpr uint32_t operator_delete_10311BB0_v2 = 0x10311BB0u;
constexpr uint32_t Allocator_Generic = 0x10311BBBu;
constexpr uint32_t Allocator_Generic_v1 = 0x10311BBBu;
constexpr uint32_t Allocator_Generic_v2 = 0x10311BBBu;
constexpr uint32_t FUN_10311c7a = 0x10311C7Au;
constexpr uint32_t FUN_10311eb0 = 0x10311EB0u;
constexpr uint32_t FUN_10311f30 = 0x10311F30u;
constexpr uint32_t FUN_10311fde = 0x10311FDEu;
constexpr uint32_t _sprintf = 0x10311FF1u;
constexpr uint32_t _sprintf_v1 = 0x10311FF1u;
constexpr uint32_t _sprintf_v2 = 0x10311FF1u;
constexpr uint32_t _sprintf_v3 = 0x10311FF1u;
constexpr uint32_t _sprintf_v4 = 0x10311FF1u;
constexpr uint32_t _sprintf_v5 = 0x10311FF1u;
constexpr uint32_t _sprintf_v6 = 0x10311FF1u;
constexpr uint32_t _strncpy = 0x10312050u;
constexpr uint32_t _strncpy_v1 = 0x10312050u;
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
