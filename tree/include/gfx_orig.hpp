// gfx_orig.hpp -- calls into the game's own code (not reconstructed here), by address.
#pragma once
#include "gfx_common.hpp"
#define XI_ORIG static inline __attribute__((always_inline))
namespace orig {
XI_ORIG int sub_10001240(int a0, int a1, HINSTANCE a2, int a3, void * a4, void * a5) { return reinterpret_cast<int (__thiscall *)(int, int, HINSTANCE, int, void *, void *)>(0x10001240u)(a0, a1, a2, a3, a4, a5); }
XI_ORIG int sub_100014A0(HWND a0) { return reinterpret_cast<int (__cdecl *)(HWND)>(0x100014A0u)(a0); }
XI_ORIG char sub_100016C0(int a0, unsigned int a1, unsigned int a2, unsigned int a3) { return reinterpret_cast<char (__cdecl *)(int, unsigned int, unsigned int, unsigned int)>(0x100016C0u)(a0, a1, a2, a3); }
XI_ORIG int sub_10001B50(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10001B50u)(a0); }
XI_ORIG char sub_10001BB0(HINSTANCE a0) { return reinterpret_cast<char (__stdcall *)(HINSTANCE)>(0x10001BB0u)(a0); }
XI_ORIG BOOL sub_10001CC0(void * a0, LPCSTR a1) { return reinterpret_cast<BOOL (__thiscall *)(void *, LPCSTR)>(0x10001CC0u)(a0, a1); }
XI_ORIG char sub_10001E00(void * a0) { return reinterpret_cast<char (__cdecl *)(void *)>(0x10001E00u)(a0); }
XI_ORIG void sub_10001E10() { reinterpret_cast<void (__cdecl *)()>(0x10001E10u)(); }
XI_ORIG char Gfx_ProbeDeviceCaps(int a0, void * a1, void * a2) { return reinterpret_cast<char (__cdecl *)(int, void *, void *)>(0x10002130u)(a0, a1, a2); }
XI_ORIG char sub_10002460() { return reinterpret_cast<char (__cdecl *)()>(0x10002460u)(); }
XI_ORIG char sub_10002B20() { return reinterpret_cast<char (__cdecl *)()>(0x10002B20u)(); }
XI_ORIG int sub_10002BC0() { return reinterpret_cast<int (__cdecl *)()>(0x10002BC0u)(); }
XI_ORIG CDx * sub_10002C00(int a0, int a1, int a2) { return reinterpret_cast<CDx * (__cdecl *)(int, int, int)>(0x10002C00u)(a0, a1, a2); }
XI_ORIG double sub_10002D80() { return reinterpret_cast<double (__cdecl *)()>(0x10002D80u)(); }
XI_ORIG char Gfx_CreateD3DHolder(int a0, int a1, int a2, char a3, char a4) { return reinterpret_cast<char (__cdecl *)(int, int, int, char, char)>(0x10002F20u)(a0, a1, a2, a3, a4); }
XI_ORIG int sub_10003040() { return reinterpret_cast<int (__cdecl *)()>(0x10003040u)(); }
XI_ORIG _DWORD * sub_10003070(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(0x10003070u)(a0); }
XI_ORIG char sub_10003110(void * a0, int a1, int a2, int a3, char a4, char a5) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int, char, char)>(0x10003110u)(a0, a1, a2, a3, a4, a5); }
XI_ORIG char Gfx_TryCreateDeviceForMode(void * a0, int a1, int a2, int a3, char a4, char a5, int a6, int a7) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int, char, char, int, int)>(0x100033C0u)(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG bool sub_10003460(void * a0, int a1, int a2, int a3) { return reinterpret_cast<bool (__thiscall *)(void *, int, int, int)>(0x10003460u)(a0, a1, a2, a3); }
XI_ORIG char Gfx_CreateDevice(void * a0, int a1, int a2, int a3, int a4, int a5, char a6, char a7, int a8) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int, int, int, char, char, int)>(0x100034B0u)(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG void sub_10003D70(void * a0, void * a1, int a2) { reinterpret_cast<void (__stdcall *)(void *, void *, int)>(0x10003D70u)(a0, a1, a2); }
XI_ORIG CDx * CDx_Constructor(void * a0, void * a1, void * a2) { return reinterpret_cast<CDx * (__thiscall *)(void *, void *, void *)>(0x10008F90u)(a0, a1, a2); }
XI_ORIG int sub_10009120(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(0x10009120u)(a0); }
XI_ORIG char CDx_Initialize(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(0x100091B0u)(a0); }
XI_ORIG char sub_10009620(void * a0, int a1, int a2, int a3) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int)>(0x10009620u)(a0, a1, a2, a3); }
XI_ORIG int sub_10009640(void * a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int)>(0x10009640u)(a0, a1, a2, a3, a4); }
XI_ORIG int sub_10009670(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(0x10009670u)(a0, a1); }
XI_ORIG int CDx_EndClearBegin(void * a0, int a1, int a2, int a3, int a4, int a5, int a6) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int, int, int)>(0x100096B0u)(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int CDx_BeginScene(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10009730u)(a0); }
XI_ORIG int CDx_EndScene(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10009740u)(a0); }
XI_ORIG BOOL SetCursorPos(int a0, int a1) { return reinterpret_cast<BOOL (__stdcall *)(int, int)>(0x10009770u)(a0, a1); }
XI_ORIG unsigned __int8 sub_10009780(void * a0, unsigned __int8 a1) { return reinterpret_cast<unsigned __int8 (__thiscall *)(void *, unsigned __int8)>(0x10009780u)(a0, a1); }
XI_ORIG HCURSOR sub_100097F0(void * a0, int a1, int a2) { return reinterpret_cast<HCURSOR (__thiscall *)(void *, int, int)>(0x100097F0u)(a0, a1, a2); }
XI_ORIG int CDx_SetDefaultRenderState(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10009920u)(a0); }
XI_ORIG int CDx_SetViewport(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(0x10009DC0u)(a0, a1); }
XI_ORIG unsigned __int8 * CDx_GetRenderTargetTop(void * a0) { return reinterpret_cast<unsigned __int8 * (__thiscall *)(void *)>(0x10009E00u)(a0); }
XI_ORIG int CDx_PushRenderTarget(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(0x10009E20u)(a0, a1, a2, a3); }
XI_ORIG int CDx_PushRenderTargetTexture(void * a0, int a1, int a2, int a3, void * a4, void * a5, int a6, int a7) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, void *, void *, int, int)>(0x10009F20u)(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG int CDx_SetTexture(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(0x1000A1D0u)(a0, a1, a2); }
XI_ORIG int sub_1000A290(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(0x1000A290u)(a0, a1, a2); }
XI_ORIG int sub_1000A2B0(int a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<int (__thiscall *)(int, int, int, int, int)>(0x1000A2B0u)(a0, a1, a2, a3, a4); }
XI_ORIG int CDx_DrawScreenQuadAtDepth(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(0x1000A3B0u)(a0, a1, a2); }
XI_ORIG int sub_1000A4F0(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(0x1000A4F0u)(a0, a1, a2, a3); }
XI_ORIG int sub_1000A5A0(void * a0, int a1, int a2, int a3, int a4, int a5) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int, int)>(0x1000A5A0u)(a0, a1, a2, a3, a4, a5); }
XI_ORIG _DWORD * CDx_CreateTexture(void * a0, unsigned __int16 a1, unsigned __int16 a2, void * a3, int a4, int a5) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, unsigned __int16, unsigned __int16, void *, int, int)>(0x1000A6E0u)(a0, a1, a2, a3, a4, a5); }
XI_ORIG int sub_1000AD70(void * a0, unsigned __int16 a1, unsigned __int16 a2) { return reinterpret_cast<int (__thiscall *)(void *, unsigned __int16, unsigned __int16)>(0x1000AD70u)(a0, a1, a2); }
XI_ORIG char sub_1000AE40(void * a0, float a1, float a2, int a3) { return reinterpret_cast<char (__thiscall *)(void *, float, float, int)>(0x1000AE40u)(a0, a1, a2, a3); }
XI_ORIG void nullsub_36() { reinterpret_cast<void (__cdecl *)()>(0x1000BB10u)(); }
XI_ORIG void nullsub_37() { reinterpret_cast<void (__cdecl *)()>(0x1000BB20u)(); }
XI_ORIG int CDx_SetTransform(void * a0, int a1, void * a2) { return reinterpret_cast<int (__thiscall *)(void *, int, void *)>(0x1000BD50u)(a0, a1, a2); }
XI_ORIG int CDx_GetTransformCached(void * a0, int a1, void * a2) { return reinterpret_cast<int (__thiscall *)(void *, int, void *)>(0x1000BDB0u)(a0, a1, a2); }
XI_ORIG char Light_ApplyTimeOfDayTint(void * a0, void * a1) { return reinterpret_cast<char (__thiscall *)(void *, void *)>(0x1000BE50u)(a0, a1); }
XI_ORIG int CDx_SetLight(void * a0, int a1, void * a2) { return reinterpret_cast<int (__thiscall *)(void *, int, void *)>(0x1000BF30u)(a0, a1, a2); }
XI_ORIG int CDx_ReleaseStereoResources(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x1000C7C0u)(a0); }
XI_ORIG int CDx_CreateRampTexture1x16(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(0x1000C850u)(a0, a1); }
XI_ORIG char CDx_CreateRampTextures1024x2(void * a0, void * a1, int a2) { return reinterpret_cast<char (__thiscall *)(void *, void *, int)>(0x1000C900u)(a0, a1, a2); }
XI_ORIG int CDx_GetStereoSecondEyeTarget(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(0x1000CA00u)(a0, a1, a2); }
XI_ORIG int CDx_DrawPrimitive(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(0x1000CB20u)(a0, a1, a2, a3); }
XI_ORIG int CDx_DrawIndexedPrimitive(void * a0, int a1, int a2, int a3, int a4, int a5) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int, int)>(0x1000CC00u)(a0, a1, a2, a3, a4, a5); }
XI_ORIG int CDx_DrawPrimitiveUP(void * a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int)>(0x1000CD00u)(a0, a1, a2, a3, a4); }
XI_ORIG void CDx_ApplyStereoEyeProjection(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(0x1000CF20u)(a0); }
XI_ORIG int CDx_RestoreProjection(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x1000CF90u)(a0); }
XI_ORIG _DWORD * CAcc_Constructor(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(0x1000CFB0u)(a0); }
XI_ORIG _DWORD * sub_1000D040(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(0x1000D040u)(a0); }
XI_ORIG void nullsub_38() { reinterpret_cast<void (__cdecl *)()>(0x1000D620u)(); }
XI_ORIG bool CEnv_IsStereoMode(void * a0) { return reinterpret_cast<bool (__thiscall *)(void *)>(0x1000D660u)(a0); }
XI_ORIG int sub_10010370() { return reinterpret_cast<int (__cdecl *)()>(0x10010370u)(); }
XI_ORIG char * App_Constructor(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(0x10010700u)(a0); }
XI_ORIG __int16 sub_10010CE0(void * a0, __int16 a1) { return reinterpret_cast<__int16 (__thiscall *)(void *, __int16)>(0x10010CE0u)(a0, a1); }
XI_ORIG void sub_10010D00(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(0x10010D00u)(a0); }
XI_ORIG char App_InitScene(int a0, int a1) { return reinterpret_cast<char (__thiscall *)(int, int)>(0x10010D20u)(a0, a1); }
XI_ORIG bool sub_10014C10() { return reinterpret_cast<bool (__cdecl *)()>(0x10014C10u)(); }
XI_ORIG double sub_10014CF0() { return reinterpret_cast<double (__cdecl *)()>(0x10014CF0u)(); }
XI_ORIG int GetFrameCounter() { return reinterpret_cast<int (__cdecl *)()>(0x10014D50u)(); }
XI_ORIG double sub_10014D70() { return reinterpret_cast<double (__cdecl *)()>(0x10014D70u)(); }
XI_ORIG double sub_10014D80() { return reinterpret_cast<double (__cdecl *)()>(0x10014D80u)(); }
XI_ORIG unsigned int sub_10014D90(unsigned __int8 a0, unsigned __int8 a1, unsigned __int8 a2) { return reinterpret_cast<unsigned int (__cdecl *)(unsigned __int8, unsigned __int8, unsigned __int8)>(0x10014D90u)(a0, a1, a2); }
XI_ORIG int sub_100150D0(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15) { return reinterpret_cast<int (__cdecl *)(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int)>(0x100150D0u)(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15); }
XI_ORIG int sub_100151F0(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15) { return reinterpret_cast<int (__cdecl *)(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int)>(0x100151F0u)(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15); }
XI_ORIG char * sub_10015220() { return reinterpret_cast<char * (__cdecl *)()>(0x10015220u)(); }
XI_ORIG int sub_10015250() { return reinterpret_cast<int (__cdecl *)()>(0x10015250u)(); }
XI_ORIG char * sub_10015270() { return reinterpret_cast<char * (__cdecl *)()>(0x10015270u)(); }
XI_ORIG __int16 Camera_GetViewportWidth() { return reinterpret_cast<__int16 (__cdecl *)()>(0x10015680u)(); }
XI_ORIG __int16 Camera_GetViewportHeight() { return reinterpret_cast<__int16 (__cdecl *)()>(0x10015690u)(); }
XI_ORIG int sub_10015AB0(int a0, int a1) { return reinterpret_cast<int (__stdcall *)(int, int)>(0x10015AB0u)(a0, a1); }
XI_ORIG char sub_10015CB0() { return reinterpret_cast<char (__cdecl *)()>(0x10015CB0u)(); }
XI_ORIG int sub_10015F50() { return reinterpret_cast<int (__cdecl *)()>(0x10015F50u)(); }
XI_ORIG char sub_10015F80() { return reinterpret_cast<char (__cdecl *)()>(0x10015F80u)(); }
XI_ORIG bool sub_100160C0(HKEY a0) { return reinterpret_cast<bool (__cdecl *)(HKEY)>(0x100160C0u)(a0); }
XI_ORIG char sub_10016150() { return reinterpret_cast<char (__cdecl *)()>(0x10016150u)(); }
XI_ORIG int sub_100161A0() { return reinterpret_cast<int (__cdecl *)()>(0x100161A0u)(); }
XI_ORIG int sub_10016400(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(0x10016400u)(a0); }
XI_ORIG int sub_10016490(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(0x10016490u)(a0); }
XI_ORIG int sub_100164C0(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(0x100164C0u)(a0); }
XI_ORIG HWND sub_100165B0(LPCSTR a0, LPCSTR a1, HINSTANCE a2, void * a3, int a4, int a5, int a6) { return reinterpret_cast<HWND (__cdecl *)(LPCSTR, LPCSTR, HINSTANCE, void *, int, int, int)>(0x100165B0u)(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG BOOL sub_10016820(int a0, LPCSTR a1, HINSTANCE a2, HWND a3) { return reinterpret_cast<BOOL (__cdecl *)(int, LPCSTR, HINSTANCE, HWND)>(0x10016820u)(a0, a1, a2, a3); }
XI_ORIG char sub_10016E40(int a0, void * a1) { return reinterpret_cast<char (__cdecl *)(int, void *)>(0x10016E40u)(a0, a1); }
XI_ORIG void sub_10016F10() { reinterpret_cast<void (__cdecl *)()>(0x10016F10u)(); }
XI_ORIG HMODULE sub_10016F40() { return reinterpret_cast<HMODULE (__cdecl *)()>(0x10016F40u)(); }
XI_ORIG char sub_10016F60(int a0, void * a1) { return reinterpret_cast<char (__cdecl *)(int, void *)>(0x10016F60u)(a0, a1); }
XI_ORIG HMODULE sub_10016FF0() { return reinterpret_cast<HMODULE (__cdecl *)()>(0x10016FF0u)(); }
XI_ORIG int sub_10017030(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(0x10017030u)(a0); }
XI_ORIG int sub_100170A0(void * a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int)>(0x100170A0u)(a0, a1, a2, a3); }
XI_ORIG int sub_100170D0(void * a0, void * a1, void * a2, void * a3) { return reinterpret_cast<int (__thiscall *)(void *, void *, void *, void *)>(0x100170D0u)(a0, a1, a2, a3); }
XI_ORIG int sub_10017100(void * a0, void * a1, void * a2, void * a3) { return reinterpret_cast<int (__thiscall *)(void *, void *, void *, void *)>(0x10017100u)(a0, a1, a2, a3); }
XI_ORIG __int64 sub_10017160(void * a0) { return reinterpret_cast<__int64 (__thiscall *)(void *)>(0x10017160u)(a0); }
XI_ORIG int sub_100177A0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x100177A0u)(a0); }
template <class... R> XI_ORIG int nullsub_13(void * a0, R... rest) { return reinterpret_cast<int (__cdecl *)(void *, ...)>(0x10017D20u)(a0, rest...); }
template <class... R> XI_ORIG int nullsub_12(void * a0, R... rest) { return reinterpret_cast<int (__cdecl *)(void *, ...)>(0x10017D30u)(a0, rest...); }
XI_ORIG int nullsub_90(_DWORD a0) { return reinterpret_cast<int (__cdecl *)(_DWORD)>(0x10017D50u)(a0); }
XI_ORIG _DWORD * sub_10018A50(void * a0, int a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int)>(0x10018A50u)(a0, a1); }
XI_ORIG _DWORD * sub_10018B30(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(0x10018B30u)(a0); }
XI_ORIG _DWORD * sub_10018C50(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(0x10018C50u)(a0); }
XI_ORIG int sub_10018CE0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10018CE0u)(a0); }
XI_ORIG MMRESULT sub_10019140() { return reinterpret_cast<MMRESULT (__cdecl *)()>(0x10019140u)(); }
XI_ORIG _DWORD * sub_100191D0() { return reinterpret_cast<_DWORD * (__cdecl *)()>(0x100191D0u)(); }
XI_ORIG int sub_1001A660(void * a0, void * a1, void * a2, int a3, int a4, int a5, float a6, float a7, unsigned int a8, int a9, int a10) { return reinterpret_cast<int (__thiscall *)(void *, void *, void *, int, int, int, float, float, unsigned int, int, int)>(0x1001A660u)(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }
XI_ORIG int sub_1001DD00(int a0, int a1) { return reinterpret_cast<int (__stdcall *)(int, int)>(0x1001DD00u)(a0, a1); }
XI_ORIG unsigned int sub_1001E0A0(void * a0, int a1, int a2) { return reinterpret_cast<unsigned int (__thiscall *)(void *, int, int)>(0x1001E0A0u)(a0, a1, a2); }
XI_ORIG float sub_1001E470() { return reinterpret_cast<float (__cdecl *)()>(0x1001E470u)(); }
XI_ORIG int sub_1001E4B0() { return reinterpret_cast<int (__cdecl *)()>(0x1001E4B0u)(); }
XI_ORIG double sub_10022120(void * a0, unsigned int a1) { return reinterpret_cast<double (__thiscall *)(void *, unsigned int)>(0x10022120u)(a0, a1); }
XI_ORIG double sub_10022190(int a0) { return reinterpret_cast<double (__stdcall *)(int)>(0x10022190u)(a0); }
XI_ORIG double sub_100221E0(int a0) { return reinterpret_cast<double (__stdcall *)(int)>(0x100221E0u)(a0); }
XI_ORIG _BYTE * sub_10022230(void * a0) { return reinterpret_cast<_BYTE * (__thiscall *)(void *)>(0x10022230u)(a0); }
XI_ORIG int sub_10022270(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(0x10022270u)(a0, a1); }
XI_ORIG int sub_10022300(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(0x10022300u)(a0, a1); }
XI_ORIG int sub_10022340(int a0) { return reinterpret_cast<int (__stdcall *)(int)>(0x10022340u)(a0); }
XI_ORIG char CYyModelDt_PrepareRenderState(void * a0, int a1, int a2, int a3, int a4, int a5) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int, int, int)>(0x100224D0u)(a0, a1, a2, a3, a4, a5); }
XI_ORIG int sub_10022FA0(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(0x10022FA0u)(a0); }
XI_ORIG __int16 sub_10023010(void * a0, int a1, void * a2, int a3) { return reinterpret_cast<__int16 (__thiscall *)(void *, int, void *, int)>(0x10023010u)(a0, a1, a2, a3); }
XI_ORIG __int16 sub_100230D0(unsigned int a0, void * a1, void * a2) { return reinterpret_cast<__int16 (__stdcall *)(unsigned int, void *, void *)>(0x100230D0u)(a0, a1, a2); }
XI_ORIG unsigned __int16 CYyModelDt_PrepareRenderStateSimple(void * a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<unsigned __int16 (__thiscall *)(void *, int, int, int, int)>(0x100231C0u)(a0, a1, a2, a3, a4); }
XI_ORIG unsigned __int16 SkinVerticesSimple(int a0, int a1) { return reinterpret_cast<unsigned __int16 (__thiscall *)(int, int)>(0x100231F0u)(a0, a1); }
XI_ORIG unsigned __int16 * SkinVertices(int a0, int a1, void * a2, int a3, void * a4, int a5) { return reinterpret_cast<unsigned __int16 * (__thiscall *)(int, int, void *, int, void *, int)>(0x10023290u)(a0, a1, a2, a3, a4, a5); }
XI_ORIG _DWORD * sub_100250C0(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(0x100250C0u)(a0); }
XI_ORIG int sub_100252F0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x100252F0u)(a0); }
XI_ORIG char sub_10025330(void * a0, void * a1, int a2, void * a3) { return reinterpret_cast<char (__thiscall *)(void *, void *, int, void *)>(0x10025330u)(a0, a1, a2, a3); }
XI_ORIG int sub_100255C0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(0x100255C0u)(a0, a1); }
XI_ORIG int sub_10025860(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10025860u)(a0); }
XI_ORIG char sub_10025C40(void * a0, int a1, void * a2) { return reinterpret_cast<char (__stdcall *)(void *, int, void *)>(0x10025C40u)(a0, a1, a2); }
XI_ORIG int sub_10025F70(int a0) { return reinterpret_cast<int (__stdcall *)(int)>(0x10025F70u)(a0); }
XI_ORIG int * sub_10026890(void * a0, int a1, void * a2, int a3) { return reinterpret_cast<int * (__thiscall *)(void *, int, void *, int)>(0x10026890u)(a0, a1, a2, a3); }
XI_ORIG int sub_10026C70(void * a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int, int, int, int, int, int)>(0x10026C70u)(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9); }
XI_ORIG _DWORD * sub_10026E10(void * a0) { return reinterpret_cast<_DWORD * (__cdecl *)(void *)>(0x10026E10u)(a0); }
XI_ORIG int sub_10026E30(int a0, float a1, int a2, int a3, int a4) { return reinterpret_cast<int (__cdecl *)(int, float, int, int, int)>(0x10026E30u)(a0, a1, a2, a3, a4); }
XI_ORIG int sub_10026E90(void * a0, void * a1) { return reinterpret_cast<int (__cdecl *)(void *, void *)>(0x10026E90u)(a0, a1); }
XI_ORIG int Vec3_Copy(void * a0, void * a1) { return reinterpret_cast<int (__cdecl *)(void *, void *)>(0x10026EB0u)(a0, a1); }
XI_ORIG float * Vec3_AddInPlace(void * a0, void * a1) { return reinterpret_cast<float * (__cdecl *)(void *, void *)>(0x10026F20u)(a0, a1); }
XI_ORIG float * sub_10026FA0(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, void *)>(0x10026FA0u)(a0, a1, a2); }
XI_ORIG float * sub_10026FD0(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, void *)>(0x10026FD0u)(a0, a1, a2); }
XI_ORIG float * sub_10027050(void * a0, float a1) { return reinterpret_cast<float * (__cdecl *)(void *, float)>(0x10027050u)(a0, a1); }
XI_ORIG float * fn_100270A0(void * a0, void * a1) { return reinterpret_cast<float * (__cdecl *)(void *, void *)>(0x100270A0u)(a0, a1); }
XI_ORIG float * sub_10027120(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, void *)>(0x10027120u)(a0, a1, a2); }
XI_ORIG float * sub_100272E0(void * a0, void * a1, float a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, float)>(0x100272E0u)(a0, a1, a2); }
XI_ORIG float * sub_10027310(void * a0, void * a1) { return reinterpret_cast<float * (__cdecl *)(void *, void *)>(0x10027310u)(a0, a1); }
XI_ORIG float * sub_100273D0(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, void *)>(0x100273D0u)(a0, a1, a2); }
XI_ORIG void sub_100276F0(void * a0, void * a1) { reinterpret_cast<void (__cdecl *)(void *, void *)>(0x100276F0u)(a0, a1); }
XI_ORIG void sub_100277E0(void * a0, void * a1) { reinterpret_cast<void (__cdecl *)(void *, void *)>(0x100277E0u)(a0, a1); }
XI_ORIG void sub_10027870(void * a0, void * a1) { reinterpret_cast<void (__cdecl *)(void *, void *)>(0x10027870u)(a0, a1); }
XI_ORIG float * sub_100278E0(void * a0) { return reinterpret_cast<float * (__cdecl *)(void *)>(0x100278E0u)(a0); }
XI_ORIG Iostream_init * fn_10027990(void * a0) { return reinterpret_cast<Iostream_init * (__thiscall *)(void *)>(0x10027990u)(a0); }
XI_ORIG void nullsub_1() { reinterpret_cast<void (__cdecl *)()>(0x100279A0u)(); }
XI_ORIG int sub_100279B0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x100279B0u)(a0); }
XI_ORIG int Mat4_Copy(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(0x100279F0u)(a0, a1); }
XI_ORIG int sub_10027A60(void * a0) { return reinterpret_cast<int (__stdcall *)(void *)>(0x10027A60u)(a0); }
XI_ORIG int Mat4_BuildScale(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(0x10027AE0u)(a0, a1); }
XI_ORIG int sub_10027B30(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(0x10027B30u)(a0, a1); }
XI_ORIG int sub_10027B80(float a0) { return reinterpret_cast<int (__stdcall *)(float)>(0x10027B80u)(a0); }
XI_ORIG int sub_10027BD0(float a0) { return reinterpret_cast<int (__stdcall *)(float)>(0x10027BD0u)(a0); }
XI_ORIG int sub_10027C20(float a0) { return reinterpret_cast<int (__stdcall *)(float)>(0x10027C20u)(a0); }
XI_ORIG int sub_10027CD0(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(0x10027CD0u)(a0, a1); }
XI_ORIG int sub_10027CF0(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(0x10027CF0u)(a0, a1); }
XI_ORIG int sub_10027D10(int a0, int a1) { return reinterpret_cast<int (__thiscall *)(int, int)>(0x10027D10u)(a0, a1); }
XI_ORIG int sub_10027E90(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(0x10027E90u)(a0, a1); }
XI_ORIG float * sub_10028140(void * a0, void * a1) { return reinterpret_cast<float * (__thiscall *)(void *, void *)>(0x10028140u)(a0, a1); }
XI_ORIG float * sub_10028200(void * a0, void * a1) { return reinterpret_cast<float * (__thiscall *)(void *, void *)>(0x10028200u)(a0, a1); }
XI_ORIG float * sub_10028290(void * a0, void * a1) { return reinterpret_cast<float * (__thiscall *)(void *, void *)>(0x10028290u)(a0, a1); }
XI_ORIG float * sub_100282C0(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__thiscall *)(void *, void *, void *)>(0x100282C0u)(a0, a1, a2); }
XI_ORIG void sub_10028320(void * a0, void * a1) { reinterpret_cast<void (__thiscall *)(void *, void *)>(0x10028320u)(a0, a1); }
XI_ORIG int sub_100283C0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x100283C0u)(a0); }
XI_ORIG _DWORD * sub_10028640(int a0, int a1) { return reinterpret_cast<_DWORD * (__cdecl *)(int, int)>(0x10028640u)(a0, a1); }
XI_ORIG _DWORD * sub_100286B0(int a0, int a1) { return reinterpret_cast<_DWORD * (__cdecl *)(int, int)>(0x100286B0u)(a0, a1); }
XI_ORIG _DWORD * sub_10028700(int a0) { return reinterpret_cast<_DWORD * (__cdecl *)(int)>(0x10028700u)(a0); }
XI_ORIG int SharedDeleteHelper(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(0x10028710u)(a0); }
XI_ORIG int sub_10028B90(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10028B90u)(a0); }
XI_ORIG _DWORD * sub_10028FD0(int a0, int a1) { return reinterpret_cast<_DWORD * (__cdecl *)(int, int)>(0x10028FD0u)(a0, a1); }
XI_ORIG char sub_10029E50(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(0x10029E50u)(a0, a1); }
XI_ORIG char * sub_10029E70(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(0x10029E70u)(a0); }
XI_ORIG _DWORD * sub_10029F70(void * a0, int a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int)>(0x10029F70u)(a0, a1); }
XI_ORIG int sub_1002A3B0(int a0, int a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(int, int, int, int)>(0x1002A3B0u)(a0, a1, a2, a3); }
XI_ORIG void sub_1002AC50(void * a0, int a1, int a2) { reinterpret_cast<void (__thiscall *)(void *, int, int)>(0x1002AC50u)(a0, a1, a2); }
XI_ORIG char * sub_1002B4B0(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(0x1002B4B0u)(a0); }
XI_ORIG int sub_1002B550(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x1002B550u)(a0); }
XI_ORIG char * sub_1002B5A0(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(0x1002B5A0u)(a0); }
XI_ORIG int sub_1002B6A0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x1002B6A0u)(a0); }
XI_ORIG char sub_1002B6E0(int a0, void * a1, void * a2, int a3, void * a4, int a5, int a6, unsigned int a7) { return reinterpret_cast<char (__thiscall *)(int, void *, void *, int, void *, int, int, unsigned int)>(0x1002B6E0u)(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG char sub_1002C2D0(int a0, void * a1, void * a2, int a3, void * a4) { return reinterpret_cast<char (__thiscall *)(int, void *, void *, int, void *)>(0x1002C2D0u)(a0, a1, a2, a3, a4); }
XI_ORIG char sub_1002C3E0(void * a0, int a1, int a2, int a3, void * a4, int a5, int a6, int a7) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int, void *, int, int, int)>(0x1002C3E0u)(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG char sub_1002C570(void * a0, int a1, int a2, void * a3) { return reinterpret_cast<char (__thiscall *)(void *, int, int, void *)>(0x1002C570u)(a0, a1, a2, a3); }
XI_ORIG __int16 * sub_1002C5C0(void * a0, unsigned int a1, int a2) { return reinterpret_cast<__int16 * (__thiscall *)(void *, unsigned int, int)>(0x1002C5C0u)(a0, a1, a2); }
XI_ORIG _DWORD * sub_1002C7C0(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(0x1002C7C0u)(a0); }
XI_ORIG _DWORD * sub_1002C890(int a0) { return reinterpret_cast<_DWORD * (__cdecl *)(int)>(0x1002C890u)(a0); }
XI_ORIG int sub_1002C8B0(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(0x1002C8B0u)(a0); }
XI_ORIG int sub_1002F130(int a0, int a1, int a2) { return reinterpret_cast<int (__cdecl *)(int, int, int)>(0x1002F130u)(a0, a1, a2); }
XI_ORIG int sub_1002F1A0() { return reinterpret_cast<int (__cdecl *)()>(0x1002F1A0u)(); }
XI_ORIG int sub_1002F1C0(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(0x1002F1C0u)(a0); }
XI_ORIG signed int sub_1002F1D0(int a0) { return reinterpret_cast<signed int (__thiscall *)(int)>(0x1002F1D0u)(a0); }
XI_ORIG int sub_10032FA0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(0x10032FA0u)(a0, a1); }
XI_ORIG char sub_10033800(int a0, void * a1, int a2) { return reinterpret_cast<char (__thiscall *)(int, void *, int)>(0x10033800u)(a0, a1, a2); }
XI_ORIG char sub_10033A70(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(0x10033A70u)(a0, a1); }
XI_ORIG void sub_10033EA0(void * a0, void * a1) { reinterpret_cast<void (__thiscall *)(void *, void *)>(0x10033EA0u)(a0, a1); }
XI_ORIG char sub_10035B00(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(0x10035B00u)(a0); }
XI_ORIG void sub_10035F00(float a0) { reinterpret_cast<void (__cdecl *)(float)>(0x10035F00u)(a0); }
XI_ORIG void sub_10036070(float a0) { reinterpret_cast<void (__cdecl *)(float)>(0x10036070u)(a0); }
XI_ORIG unsigned int sub_100384B0(void * a0, int a1, int a2, char a3) { return reinterpret_cast<unsigned int (__cdecl *)(void *, int, int, char)>(0x100384B0u)(a0, a1, a2, a3); }
XI_ORIG unsigned __int8 * sub_10038720(void * a0, float a1, float a2, float a3) { return reinterpret_cast<unsigned __int8 * (__cdecl *)(void *, float, float, float)>(0x10038720u)(a0, a1, a2, a3); }
XI_ORIG int sub_10039810(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(0x10039810u)(a0, a1); }
XI_ORIG void sub_100398D0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(0x100398D0u)(a0); }
XI_ORIG int sub_100398E0(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(0x100398E0u)(a0, a1); }
XI_ORIG void sub_10039970(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(0x10039970u)(a0); }
XI_ORIG char TexObj_CreateAndUpload(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(0x10039A30u)(a0); }
XI_ORIG int sub_1003AA90(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x1003AA90u)(a0); }
XI_ORIG int sub_1003AC80(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(0x1003AC80u)(a0, a1); }
XI_ORIG _DWORD * TexMgr_FindByName(void * a0, int a1, int a2, char a3) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int, int, char)>(0x1003AEA0u)(a0, a1, a2, a3); }
XI_ORIG int sub_1003AF10(void * a0, void * a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, void *, int)>(0x1003AF10u)(a0, a1, a2); }
XI_ORIG int sub_1003B210(void * a0, int a1, int a2, int a3, int a4, void * a5, char a6) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int, void *, char)>(0x1003B210u)(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG _DWORD * sub_1003B3C0(void * a0) { return reinterpret_cast<_DWORD * (__stdcall *)(void *)>(0x1003B3C0u)(a0); }
XI_ORIG _DWORD * sub_1003B450(void * a0, int a1, int a2, int a3, int a4, int a5) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int, int, int, int, int)>(0x1003B450u)(a0, a1, a2, a3, a4, a5); }
XI_ORIG void ReleaseSubObjectIfPresent(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(0x1003B5C0u)(a0); }
XI_ORIG int sub_1003B640(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x1003B640u)(a0); }
XI_ORIG int sub_1003B750(void * a0, char a1) { return reinterpret_cast<int (__thiscall *)(void *, char)>(0x1003B750u)(a0, a1); }
XI_ORIG int sub_1003B780(void * a0, char a1) { return reinterpret_cast<int (__thiscall *)(void *, char)>(0x1003B780u)(a0, a1); }
XI_ORIG int sub_1003B7B0(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(0x1003B7B0u)(a0, a1); }
XI_ORIG int sub_1003B830(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(0x1003B830u)(a0, a1); }
XI_ORIG char ** CMoCamera_GetRuntimeClass() { return reinterpret_cast<char ** (__cdecl *)()>(0x1003C880u)(); }
XI_ORIG int sub_1003D640(float a0, float a1, int a2, void * a3, float a4) { return reinterpret_cast<int (__cdecl *)(float, float, int, void *, float)>(0x1003D640u)(a0, a1, a2, a3, a4); }
XI_ORIG void sub_1003D980(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(0x1003D980u)(a0); }
XI_ORIG void unknown_libname_18() { reinterpret_cast<void (__cdecl *)()>(0x1003DB50u)(); }
XI_ORIG void unknown_libname_19() { reinterpret_cast<void (__cdecl *)()>(0x1003DDF0u)(); }
XI_ORIG unsigned int sub_1003E280(void * a0, unsigned int a1) { return reinterpret_cast<unsigned int (__thiscall *)(void *, unsigned int)>(0x1003E280u)(a0, a1); }
XI_ORIG char sub_1003E2A0(void * a0, void * a1) { return reinterpret_cast<char (__thiscall *)(void *, void *)>(0x1003E2A0u)(a0, a1); }
XI_ORIG char sub_1003EF20(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(0x1003EF20u)(a0); }
XI_ORIG int CMoDistMorphElem_ApplyMorph(int a0, int a1, void * a2, int a3, int a4, void * a5, char a6) { return reinterpret_cast<int (__thiscall *)(int, int, void *, int, int, void *, char)>(0x1003F440u)(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG _DWORD * CMoDistMorphElem_ComputeMorphHandle(int a0, int a1, int a2) { return reinterpret_cast<_DWORD * (__thiscall *)(int, int, int)>(0x1003F8A0u)(a0, a1, a2); }
XI_ORIG char CXi471011C8_VT_1032AC90_Dtor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(0x1003FB10u)(a0); }
XI_ORIG bool sub_1003FCB0(void * a0) { return reinterpret_cast<bool (__thiscall *)(void *)>(0x1003FCB0u)(a0); }
XI_ORIG char * sub_1003FD70(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(0x1003FD70u)(a0); }
XI_ORIG int sub_10040670(void * a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int)>(0x10040670u)(a0, a1, a2, a3, a4); }
XI_ORIG char CXi471011C8_VT_1032AD80_Dtor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(0x10041110u)(a0); }
XI_ORIG void unknown_libname_43() { reinterpret_cast<void (__cdecl *)()>(0x10041F30u)(); }
XI_ORIG _DWORD * sub_100425F0(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x100425F0u)(a0, a1); }
XI_ORIG char CMoDistMorphElem_ValidateMorphData(int a0, int a1, int a2, int a3) { return reinterpret_cast<char (__stdcall *)(int, int, int, int)>(0x10042D00u)(a0, a1, a2, a3); }
XI_ORIG void unknown_libname_50() { reinterpret_cast<void (__cdecl *)()>(0x100439B0u)(); }
XI_ORIG void * CMoTask_VT_1032AF60_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<void * (__thiscall *)(void *, char)>(0x10043A90u)(a0, a1); }
XI_ORIG char sub_10043AB0(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(0x10043AB0u)(a0); }
XI_ORIG _DWORD * CMoTask_VT_1032AFA8_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x10043C90u)(a0, a1); }
XI_ORIG char sub_10043CB0(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(0x10043CB0u)(a0); }
XI_ORIG char * sub_10043DF0(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(0x10043DF0u)(a0); }
XI_ORIG void sub_10043E40(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(0x10043E40u)(a0); }
XI_ORIG int sub_10043E50(void * a0, int a1, unsigned int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, int, unsigned int, int)>(0x10043E50u)(a0, a1, a2, a3); }
XI_ORIG int sub_10044010(int a0) { return reinterpret_cast<int (__stdcall *)(int)>(0x10044010u)(a0); }
XI_ORIG void * sub_10044130(void * a0) { return reinterpret_cast<void * (__thiscall *)(void *)>(0x10044130u)(a0); }
XI_ORIG int sub_100441A0() { return reinterpret_cast<int (__cdecl *)()>(0x100441A0u)(); }
XI_ORIG int sub_10044210(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(0x10044210u)(a0); }
XI_ORIG int sub_10044230(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(0x10044230u)(a0); }
XI_ORIG _DWORD * sub_10044270() { return reinterpret_cast<_DWORD * (__cdecl *)()>(0x10044270u)(); }
XI_ORIG int sub_10044890(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(0x10044890u)(a0); }
XI_ORIG char CMoElemFamily_SharedCleanup(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(0x10044970u)(a0); }
XI_ORIG int RegisterWithProcessor(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(0x10045D40u)(a0); }
XI_ORIG int sub_10047F80(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(0x10047F80u)(a0); }
XI_ORIG void * CMoGeneratorClone_VT_1032B114_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<void * (__thiscall *)(void *, char)>(0x10047FE0u)(a0, a1); }
XI_ORIG int sub_10048000(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(0x10048000u)(a0); }
XI_ORIG char sub_10048140(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(0x10048140u)(a0, a1); }
XI_ORIG char sub_100481D0(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(0x100481D0u)(a0, a1); }
XI_ORIG _DWORD * sub_100492F0(void * a0, int a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int)>(0x100492F0u)(a0, a1); }
XI_ORIG int sub_10049570(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10049570u)(a0); }
XI_ORIG __int16 SharedConditionalStateReset(int a0) { return reinterpret_cast<__int16 (__thiscall *)(int)>(0x100531D0u)(a0); }
XI_ORIG int sub_10053450(int a0, int a1) { return reinterpret_cast<int (__thiscall *)(int, int)>(0x10053450u)(a0, a1); }
XI_ORIG int CMoGeneratorClone_CreateInstance(void * a0, void * a1, void * a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, void *, void *, int)>(0x100536C0u)(a0, a1, a2, a3); }
XI_ORIG char * CMoGeneratorClone_VT_1032B69C_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<char * (__thiscall *)(void *, char)>(0x10053840u)(a0, a1); }
XI_ORIG char sub_100541C0(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(0x100541C0u)(a0); }
XI_ORIG char ** CMoGeneratorClone_GetRuntimeClass() { return reinterpret_cast<char ** (__cdecl *)()>(0x100542E0u)(); }
XI_ORIG int sub_100542F0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x100542F0u)(a0); }
XI_ORIG char sub_100548C0(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(0x100548C0u)(a0); }
XI_ORIG int sub_10055000(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10055000u)(a0); }
XI_ORIG _DWORD * sub_10055240(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(0x10055240u)(a0); }
XI_ORIG int sub_10055250(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10055250u)(a0); }
XI_ORIG _DWORD * sub_100552E0(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(0x100552E0u)(a0); }
XI_ORIG _DWORD * sub_100555D0() { return reinterpret_cast<_DWORD * (__cdecl *)()>(0x100555D0u)(); }
XI_ORIG char * sub_10056220(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(0x10056220u)(a0); }
XI_ORIG char sub_10056390(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(0x10056390u)(a0); }
XI_ORIG char ** CMoPathDriveActorTask_GetRuntimeClass() { return reinterpret_cast<char ** (__cdecl *)()>(0x10056940u)(); }
XI_ORIG char ** CMoLockColorDriveTask_GetRuntimeClass() { return reinterpret_cast<char ** (__cdecl *)()>(0x10056960u)(); }
XI_ORIG char ** CMoLockLookAtDriveTask_GetRuntimeClass() { return reinterpret_cast<char ** (__cdecl *)()>(0x10056980u)(); }
XI_ORIG char ** CMoActorColorDriveTask_GetRuntimeClass() { return reinterpret_cast<char ** (__cdecl *)()>(0x100569A0u)(); }
XI_ORIG char ** CMoActorRotationDriveTask_GetRuntimeClass() { return reinterpret_cast<char ** (__cdecl *)()>(0x100569C0u)(); }
XI_ORIG int * Scheduler_Execute(void * a0, void * a1, void * a2, int a3, int a4) { return reinterpret_cast<int * (__thiscall *)(void *, void *, void *, int, int)>(0x10056C70u)(a0, a1, a2, a3, a4); }
XI_ORIG char sub_100574A0(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(0x100574A0u)(a0); }
XI_ORIG _DWORD * sub_1005F2C0(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x1005F2C0u)(a0, a1); }
XI_ORIG int sub_1005F3F0(int a0, char a1) { return reinterpret_cast<int (__thiscall *)(int, char)>(0x1005F3F0u)(a0, a1); }
XI_ORIG _DWORD * sub_1005F660(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x1005F660u)(a0, a1); }
XI_ORIG int sub_1005F990(int a0, char a1) { return reinterpret_cast<int (__thiscall *)(int, char)>(0x1005F990u)(a0, a1); }
XI_ORIG _DWORD * sub_1005FE30(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x1005FE30u)(a0, a1); }
XI_ORIG _DWORD * sub_10060190(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x10060190u)(a0, a1); }
XI_ORIG _DWORD * sub_100602D0(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x100602D0u)(a0, a1); }
XI_ORIG _DWORD * sub_100608B0(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x100608B0u)(a0, a1); }
XI_ORIG _DWORD * sub_10060BB0(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x10060BB0u)(a0, a1); }
XI_ORIG _DWORD * sub_10060E60(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x10060E60u)(a0, a1); }
XI_ORIG _DWORD * sub_10061010(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x10061010u)(a0, a1); }
XI_ORIG _DWORD * sub_10061140(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x10061140u)(a0, a1); }
XI_ORIG _DWORD * sub_10061300(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x10061300u)(a0, a1); }
XI_ORIG _DWORD * sub_100614B0(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x100614B0u)(a0, a1); }
XI_ORIG _DWORD * sub_100616B0(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x100616B0u)(a0, a1); }
XI_ORIG _DWORD * sub_10061830(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x10061830u)(a0, a1); }
XI_ORIG _DWORD * sub_10061A00(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x10061A00u)(a0, a1); }
XI_ORIG _DWORD * sub_10061B80(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x10061B80u)(a0, a1); }
XI_ORIG _DWORD * sub_10061D00(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x10061D00u)(a0, a1); }
XI_ORIG _DWORD * sub_10061EA0(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x10061EA0u)(a0, a1); }
XI_ORIG _DWORD * sub_10062090(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x10062090u)(a0, a1); }
XI_ORIG _DWORD * sub_10062210(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x10062210u)(a0, a1); }
XI_ORIG _DWORD * sub_100623B0(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x100623B0u)(a0, a1); }
XI_ORIG _DWORD * sub_10062550(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x10062550u)(a0, a1); }
XI_ORIG _DWORD * sub_10062700(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x10062700u)(a0, a1); }
XI_ORIG _DWORD * sub_100634A0(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(0x100634A0u)(a0); }
XI_ORIG _DWORD * sub_10063C60(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(0x10063C60u)(a0); }
XI_ORIG char sub_10069D70(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(0x10069D70u)(a0); }
XI_ORIG _BYTE * sub_10069DD0(int a0, int a1) { return reinterpret_cast<_BYTE * (__thiscall *)(int, int)>(0x10069DD0u)(a0, a1); }
XI_ORIG _DWORD * sub_1006A150(void * a0, unsigned __int16 a1, unsigned __int16 a2, void * a3) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, unsigned __int16, unsigned __int16, void *)>(0x1006A150u)(a0, a1, a2, a3); }
XI_ORIG int sub_1006A480(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(0x1006A480u)(a0, a1); }
XI_ORIG int sub_1006A4A0(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(0x1006A4A0u)(a0, a1); }
XI_ORIG int sub_1006A4C0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x1006A4C0u)(a0); }
XI_ORIG int sub_1006A4F0(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(0x1006A4F0u)(a0); }
XI_ORIG int sub_1006A520(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(0x1006A520u)(a0, a1); }
XI_ORIG int sub_1006A560(unsigned int a0) { return reinterpret_cast<int (__cdecl *)(unsigned int)>(0x1006A560u)(a0); }
XI_ORIG exception * fn_1006AD90(void * a0) { return reinterpret_cast<exception * (__thiscall *)(void *)>(0x1006AD90u)(a0); }
XI_ORIG int sub_1006AE30(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(0x1006AE30u)(a0, a1, a2); }
XI_ORIG int sub_1006AF90(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(0x1006AF90u)(a0, a1); }
XI_ORIG _DWORD * sub_1006B050(int a0) { return reinterpret_cast<_DWORD * (__thiscall *)(int)>(0x1006B050u)(a0); }
XI_ORIG void sub_1006B090(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(0x1006B090u)(a0); }
XI_ORIG _DWORD * sub_1006B0C0(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(0x1006B0C0u)(a0); }
XI_ORIG char sub_1006B250(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(0x1006B250u)(a0); }
XI_ORIG int sub_1006BD90(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(0x1006BD90u)(a0); }
XI_ORIG _DWORD * sub_1006C0F0(int a0, int a1) { return reinterpret_cast<_DWORD * (__cdecl *)(int, int)>(0x1006C0F0u)(a0, a1); }
XI_ORIG _DWORD * sub_1006C110(int a0) { return reinterpret_cast<_DWORD * (__cdecl *)(int)>(0x1006C110u)(a0); }
XI_ORIG _DWORD * sub_1006C140(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(0x1006C140u)(a0); }
XI_ORIG int sub_1006C1E0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x1006C1E0u)(a0); }
XI_ORIG _DWORD * j_OcclusionMng_CreateTarget(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(0x1006C210u)(a0); }
XI_ORIG _DWORD * OcclusionMng_CreateTarget(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(0x1006C220u)(a0); }
XI_ORIG int sub_1006C260(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(0x1006C260u)(a0); }
XI_ORIG int sub_1006C270(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x1006C270u)(a0); }
XI_ORIG int Occlusion_QueryScreenRect(void * a0, void * a1, int a2, float a3, float a4, int a5, int a6, int a7, char a8, int a9) { return reinterpret_cast<int (__thiscall *)(void *, void *, int, float, float, int, int, int, char, int)>(0x1006C280u)(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9); }
XI_ORIG int Occlusion_QueryScreenRectImpl(void * a0, void * a1, int a2, float a3, float a4, int a5, int a6, int a7, char a8, int a9) { return reinterpret_cast<int (__thiscall *)(void *, void *, int, float, float, int, int, int, char, int)>(0x1006C300u)(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9); }
XI_ORIG int sub_1006C9B0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x1006C9B0u)(a0); }
XI_ORIG void nullsub_44() { reinterpret_cast<void (__cdecl *)()>(0x1006CF90u)(); }
XI_ORIG float * CMoProcessor_VT_1032C368_vf012(void * a0, void * a1, float a2) { return reinterpret_cast<float * (__stdcall *)(void *, void *, float)>(0x1006D3B0u)(a0, a1, a2); }
XI_ORIG void sub_1006D5D0(void * a0) { reinterpret_cast<void (__stdcall *)(void *)>(0x1006D5D0u)(a0); }
XI_ORIG float * sub_1006D7E0(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__stdcall *)(void *, void *, void *)>(0x1006D7E0u)(a0, a1, a2); }
XI_ORIG float * sub_1006D870(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__stdcall *)(void *, void *, void *)>(0x1006D870u)(a0, a1, a2); }
XI_ORIG float * sub_1006D900(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__stdcall *)(void *, void *, void *)>(0x1006D900u)(a0, a1, a2); }
XI_ORIG float * CMoProcessor_VT_1032C368_vf021(void * a0, void * a1, void * a2, float a3) { return reinterpret_cast<float * (__stdcall *)(void *, void *, void *, float)>(0x1006D980u)(a0, a1, a2, a3); }
XI_ORIG void sub_1006E7D0(void * a0, int a1, int a2, void * a3, void * a4, float a5, float a6) { reinterpret_cast<void (__stdcall *)(void *, int, int, void *, void *, float, float)>(0x1006E7D0u)(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG CMoProcessor * sub_1006E970(void * a0, void * a1, int a2, int a3, void * a4) { return reinterpret_cast<CMoProcessor * (__stdcall *)(void *, void *, int, int, void *)>(0x1006E970u)(a0, a1, a2, a3, a4); }
XI_ORIG float * sub_1006EB70(void * a0, void * a1, int a2, void * a3, void * a4, float a5, float a6) { return reinterpret_cast<float * (__stdcall *)(void *, void *, int, void *, void *, float, float)>(0x1006EB70u)(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int sub_1006ED10(void * a0, void * a1, void * a2, int a3, int a4, int a5, float a6, float a7) { return reinterpret_cast<int (__thiscall *)(void *, void *, void *, int, int, int, float, float)>(0x1006ED10u)(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG char sub_10070F80(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(0x10070F80u)(a0); }
XI_ORIG __int16 sub_10071050(void * a0) { return reinterpret_cast<__int16 (__thiscall *)(void *)>(0x10071050u)(a0); }
XI_ORIG bool ResourceHandle_IsResident(void * a0, int a1) { return reinterpret_cast<bool (__fastcall *)(void *, int)>(0x10071060u)(a0, a1); }
XI_ORIG _DWORD * sub_10071180(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x10071180u)(a0, a1); }
XI_ORIG void nullsub_171() { reinterpret_cast<void (__cdecl *)()>(0x100719B0u)(); }
XI_ORIG unsigned int ** sub_10071C10(void * a0, char a1) { return reinterpret_cast<unsigned int ** (__thiscall *)(void *, char)>(0x10071C10u)(a0, a1); }
XI_ORIG unsigned int ** sub_10071C60(void * a0, char a1) { return reinterpret_cast<unsigned int ** (__thiscall *)(void *, char)>(0x10071C60u)(a0, a1); }
XI_ORIG int sub_10071DE0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10071DE0u)(a0); }
XI_ORIG int * sub_10071E90(int a0) { return reinterpret_cast<int * (__fastcall *)(int)>(0x10071E90u)(a0); }
XI_ORIG char sub_10071EC0(int a0, int a1) { return reinterpret_cast<char (__fastcall *)(int, int)>(0x10071EC0u)(a0, a1); }
XI_ORIG _DWORD * sub_10071F50(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(0x10071F50u)(a0); }
XI_ORIG int sub_10072030(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10072030u)(a0); }
XI_ORIG __int16 sub_100721F0(int a0) { return reinterpret_cast<__int16 (__thiscall *)(int)>(0x100721F0u)(a0); }
XI_ORIG _DWORD * sub_10072270(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(0x10072270u)(a0); }
XI_ORIG _DWORD * sub_10072290(void * a0, void * a1, int a2, int a3) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, int, int)>(0x10072290u)(a0, a1, a2, a3); }
XI_ORIG _DWORD * sub_10072430(void * a0, int a1, int a2, int a3) { return reinterpret_cast<_DWORD * (__stdcall *)(void *, int, int, int)>(0x10072430u)(a0, a1, a2, a3); }
XI_ORIG _DWORD * YmResourceFile_FindResourceUnder(int a0, void * a1, int a2, int a3) { return reinterpret_cast<_DWORD * (__thiscall *)(int, void *, int, int)>(0x100724C0u)(a0, a1, a2, a3); }
XI_ORIG char sub_100729E0(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(0x100729E0u)(a0); }
XI_ORIG int * sub_10072ED0(int a0, int a1) { return reinterpret_cast<int * (__thiscall *)(int, int)>(0x10072ED0u)(a0, a1); }
XI_ORIG int ResourceHandle_Deref(int a0) { return reinterpret_cast<int (__stdcall *)(int)>(0x10072FB0u)(a0); }
XI_ORIG __int16 sub_10073B20(void * a0) { return reinterpret_cast<__int16 (__stdcall *)(void *)>(0x10073B20u)(a0); }
XI_ORIG _DWORD * sub_10074140(void * a0, void * a1, int a2) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, int)>(0x10074140u)(a0, a1, a2); }
XI_ORIG int sub_10074180(void * a0, void * a1) { return reinterpret_cast<int (__thiscall *)(void *, void *)>(0x10074180u)(a0, a1); }
XI_ORIG int sub_10074290() { return reinterpret_cast<int (__cdecl *)()>(0x10074290u)(); }
XI_ORIG char ResourceIter_FindFirst(void * a0, int a1, int a2, int a3, int a4) { return reinterpret_cast<char (__thiscall *)(void *, int, int, int, int)>(0x100742B0u)(a0, a1, a2, a3, a4); }
XI_ORIG int ResourceIter_FindNext(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(0x100745C0u)(a0, a1); }
XI_ORIG int sub_100746A0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x100746A0u)(a0); }
XI_ORIG _DWORD * sub_10074740(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(0x10074740u)(a0); }
XI_ORIG char sub_100747C0(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(0x100747C0u)(a0); }
XI_ORIG int sub_100748F0(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(0x100748F0u)(a0); }
XI_ORIG int sub_100749B0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(0x100749B0u)(a0, a1); }
XI_ORIG int sub_10074A10(int a0) { return reinterpret_cast<int (__stdcall *)(int)>(0x10074A10u)(a0); }
XI_ORIG char sub_10074B60(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(0x10074B60u)(a0); }
XI_ORIG int sub_10075280() { return reinterpret_cast<int (__cdecl *)()>(0x10075280u)(); }
XI_ORIG int sub_10075520(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(0x10075520u)(a0, a1); }
XI_ORIG int sub_10075960(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(0x10075960u)(a0, a1); }
XI_ORIG _DWORD * sub_100814A0(int a0) { return reinterpret_cast<_DWORD * (__thiscall *)(int)>(0x100814A0u)(a0); }
XI_ORIG int ActorHandle_Resolve(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10081550u)(a0); }
XI_ORIG int sub_10081A50() { return reinterpret_cast<int (__cdecl *)()>(0x10081A50u)(); }
XI_ORIG char CXiActor_NameCalc(void * a0, void * a1, int a2, void * a3, int a4) { return reinterpret_cast<char (__thiscall *)(void *, void *, int, void *, int)>(0x100830C0u)(a0, a1, a2, a3, a4); }
XI_ORIG BOOL sub_10084140(void * a0) { return reinterpret_cast<BOOL (__thiscall *)(void *)>(0x10084140u)(a0); }
XI_ORIG bool sub_10084330(void * a0, int a1) { return reinterpret_cast<bool (__thiscall *)(void *, int)>(0x10084330u)(a0, a1); }
XI_ORIG bool sub_10084350(void * a0, int a1) { return reinterpret_cast<bool (__thiscall *)(void *, int)>(0x10084350u)(a0, a1); }
XI_ORIG bool sub_10084370(void * a0, int a1) { return reinterpret_cast<bool (__thiscall *)(void *, int)>(0x10084370u)(a0, a1); }
XI_ORIG int sub_10084390(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10084390u)(a0); }
XI_ORIG bool sub_100843B0(void * a0) { return reinterpret_cast<bool (__thiscall *)(void *)>(0x100843B0u)(a0); }
XI_ORIG int sub_10084400(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10084400u)(a0); }
XI_ORIG int sub_10084420(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10084420u)(a0); }
XI_ORIG __int16 sub_10084480(void * a0, int a1) { return reinterpret_cast<__int16 (__thiscall *)(void *, int)>(0x10084480u)(a0, a1); }
XI_ORIG int sub_10084500(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10084500u)(a0); }
XI_ORIG int sub_10084520(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10084520u)(a0); }
XI_ORIG double sub_10084540(void * a0) { return reinterpret_cast<double (__thiscall *)(void *)>(0x10084540u)(a0); }
XI_ORIG BOOL sub_10084580(int a0) { return reinterpret_cast<BOOL (__thiscall *)(int)>(0x10084580u)(a0); }
XI_ORIG BOOL sub_100845B0(void * a0) { return reinterpret_cast<BOOL (__thiscall *)(void *)>(0x100845B0u)(a0); }
XI_ORIG BOOL sub_10084640(void * a0) { return reinterpret_cast<BOOL (__thiscall *)(void *)>(0x10084640u)(a0); }
XI_ORIG int sub_10084720(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10084720u)(a0); }
XI_ORIG int sub_10085060(int a0, void * a1) { return reinterpret_cast<int (__thiscall *)(int, void *)>(0x10085060u)(a0, a1); }
XI_ORIG bool sub_10085490(void * a0) { return reinterpret_cast<bool (__thiscall *)(void *)>(0x10085490u)(a0); }
XI_ORIG void sub_10086210(void * a0, void * a1, float a2, int a3, int a4) { reinterpret_cast<void (__cdecl *)(void *, void *, float, int, int)>(0x10086210u)(a0, a1, a2, a3, a4); }
XI_ORIG int sub_1008A0D0() { return reinterpret_cast<int (__cdecl *)()>(0x1008A0D0u)(); }
XI_ORIG char sub_10093720() { return reinterpret_cast<char (__cdecl *)()>(0x10093720u)(); }
XI_ORIG bool sub_10093DB0() { return reinterpret_cast<bool (__cdecl *)()>(0x10093DB0u)(); }
XI_ORIG char * sub_10097840(int a0) { return reinterpret_cast<char * (__thiscall *)(int)>(0x10097840u)(a0); }
XI_ORIG int sub_10097A40(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10097A40u)(a0); }
XI_ORIG bool sub_10098320(void * a0) { return reinterpret_cast<bool (__thiscall *)(void *)>(0x10098320u)(a0); }
XI_ORIG char sub_100A2810(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(0x100A2810u)(a0); }
XI_ORIG _DWORD * sub_100A4C20(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(0x100A4C20u)(a0); }
XI_ORIG char sub_100A9D30(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(0x100A9D30u)(a0); }
XI_ORIG _DWORD * CMoTask_VT_1032E808_ScalarDeletingDtor(void * a0, char a1) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, char)>(0x100A9DB0u)(a0, a1); }
XI_ORIG int sub_100C5410() { return reinterpret_cast<int (__cdecl *)()>(0x100C5410u)(); }
XI_ORIG int sub_100C5440() { return reinterpret_cast<int (__cdecl *)()>(0x100C5440u)(); }
XI_ORIG int sub_100C54C0() { return reinterpret_cast<int (__cdecl *)()>(0x100C54C0u)(); }
XI_ORIG int CXiSkeletonActor_Init(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x100C5D60u)(a0); }
XI_ORIG int sub_100CB4E0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x100CB4E0u)(a0); }
XI_ORIG int PerActor_FourBucketStaggeredUpdate(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x100CBDE0u)(a0); }
XI_ORIG void nullsub_276() { reinterpret_cast<void (__cdecl *)()>(0x100CCE20u)(); }
XI_ORIG void nullsub_277() { reinterpret_cast<void (__cdecl *)()>(0x100CCE30u)(); }
XI_ORIG void nullsub_278() { reinterpret_cast<void (__cdecl *)()>(0x100CCE40u)(); }
XI_ORIG void nullsub_279() { reinterpret_cast<void (__cdecl *)()>(0x100CCE50u)(); }
XI_ORIG int sub_100CCE60(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x100CCE60u)(a0); }
XI_ORIG _DWORD * CXiSkeletonActor_ReverseFindRes(void * a0, void * a1, int a2, int a3, int a4, int a5, char a6) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, int, int, int, int, char)>(0x100CE480u)(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int sub_100D0880(void * a0, void * a1, void * a2, float a3, unsigned int a4, int a5) { return reinterpret_cast<int (__thiscall *)(void *, void *, void *, float, unsigned int, int)>(0x100D0880u)(a0, a1, a2, a3, a4, a5); }
XI_ORIG int sub_100D08B0(void * a0, void * a1, void * a2, float a3) { return reinterpret_cast<int (__thiscall *)(void *, void *, void *, float)>(0x100D08B0u)(a0, a1, a2, a3); }
XI_ORIG bool CXiSkeletonActor_IsReadCompleteBody(void * a0) { return reinterpret_cast<bool (__thiscall *)(void *)>(0x100D12D0u)(a0); }
XI_ORIG char CXiSkeletonActor_OnSkeletonLoaded(int a0, int a1) { return reinterpret_cast<char (__cdecl *)(int, int)>(0x100D2F40u)(a0, a1); }
XI_ORIG char CXiSkeletonActor_LoadSkeletonSync(int a0, int a1) { return reinterpret_cast<char (__thiscall *)(int, int)>(0x100D2F60u)(a0, a1); }
XI_ORIG char CXiSkeletonActor_LoadSkeletonResource(void * a0, int a1) { return reinterpret_cast<char (__thiscall *)(void *, int)>(0x100D4330u)(a0, a1); }
XI_ORIG int sub_100D54C0(void * a0, void * a1, float a2) { return reinterpret_cast<int (__thiscall *)(void *, void *, float)>(0x100D54C0u)(a0, a1, a2); }
XI_ORIG void sub_100D5B10(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(0x100D5B10u)(a0); }
XI_ORIG int sub_100D6D30(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x100D6D30u)(a0); }
XI_ORIG _DWORD * CXiSkeletonActorRes_Constructor(void * a0, int a1, int a2) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, int, int)>(0x100D7020u)(a0, a1, a2); }
XI_ORIG char CXiSkeletonActorRes_Destructor(void * a0) { return reinterpret_cast<char (__thiscall *)(void *)>(0x100D7050u)(a0); }
XI_ORIG int CXiSkeletonActorRes_GetLinkedActor(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x100D7090u)(a0); }
XI_ORIG int sub_100DB100(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x100DB100u)(a0); }
XI_ORIG int sub_100DB140(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x100DB140u)(a0); }
XI_ORIG int sub_100DB170(void * a0, int a1, int a2) { return reinterpret_cast<int (__thiscall *)(void *, int, int)>(0x100DB170u)(a0, a1, a2); }
XI_ORIG int sub_100DBB50(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(0x100DBB50u)(a0, a1); }
XI_ORIG char sub_1011BAE0(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(0x1011BAE0u)(a0); }
XI_ORIG int sub_1011BDC0() { return reinterpret_cast<int (__cdecl *)()>(0x1011BDC0u)(); }
XI_ORIG char sub_1011BF40(__int16 a0, int a1, int a2, float a3, float a4) { return reinterpret_cast<char (__cdecl *)(__int16, int, int, float, float)>(0x1011BF40u)(a0, a1, a2, a3, a4); }
XI_ORIG int sub_1011C1F0(int a0, int a1, int a2, int a3) { return reinterpret_cast<int (__cdecl *)(int, int, int, int)>(0x1011C1F0u)(a0, a1, a2, a3); }
XI_ORIG int sub_1011C5D0(void * a0, int a1, int a2) { return reinterpret_cast<int (__stdcall *)(void *, int, int)>(0x1011C5D0u)(a0, a1, a2); }
XI_ORIG char sub_1011C670(void * a0, int a1) { return reinterpret_cast<char (__stdcall *)(void *, int)>(0x1011C670u)(a0, a1); }
XI_ORIG void sub_1011C720(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(0x1011C720u)(a0); }
XI_ORIG int sub_1011C810(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x1011C810u)(a0); }
XI_ORIG bool sub_1011C850(int a0) { return reinterpret_cast<bool (__thiscall *)(int)>(0x1011C850u)(a0); }
XI_ORIG __int16 sub_1011CE80(void * a0) { return reinterpret_cast<__int16 (__cdecl *)(void *)>(0x1011CE80u)(a0); }
XI_ORIG char sub_1011D840(unsigned __int16 a0, void * a1) { return reinterpret_cast<char (__cdecl *)(unsigned __int16, void *)>(0x1011D840u)(a0, a1); }
XI_ORIG int sub_1011DA80(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(0x1011DA80u)(a0); }
XI_ORIG int sub_1011F2D0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x1011F2D0u)(a0); }
XI_ORIG int sub_1011F300(void * a0, int a1, int a2, int a3, int a4, int a5) { return reinterpret_cast<int (__thiscall *)(void *, int, int, int, int, int)>(0x1011F300u)(a0, a1, a2, a3, a4, a5); }
XI_ORIG int sub_1011FC80(int a0, int a1, int a2, float a3, float a4, int a5, char a6, int a7, int a8) { return reinterpret_cast<int (__thiscall *)(int, int, int, float, float, int, char, int, int)>(0x1011FC80u)(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG int sub_1011FF70(void * a0, int a1, int a2, float a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16, int a17, int a18, unsigned int a19, int a20) { return reinterpret_cast<int (__thiscall *)(void *, int, int, float, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, unsigned int, int)>(0x1011FF70u)(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20); }
XI_ORIG int sub_101200B0(unsigned int a0, unsigned int a1) { return reinterpret_cast<int (__cdecl *)(unsigned int, unsigned int)>(0x101200B0u)(a0, a1); }
XI_ORIG _DWORD * sub_101201C0(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(0x101201C0u)(a0); }
XI_ORIG char * sub_10120450(void * a0, void * a1) { return reinterpret_cast<char * (__thiscall *)(void *, void *)>(0x10120450u)(a0, a1); }
XI_ORIG bool sub_10120510(void * a0, unsigned __int8 a1, int a2, void * a3, void * a4, void * a5) { return reinterpret_cast<bool (__thiscall *)(void *, unsigned __int8, int, void *, void *, void *)>(0x10120510u)(a0, a1, a2, a3, a4, a5); }
XI_ORIG char sub_10120590(void * a0, int a1, void * a2, void * a3) { return reinterpret_cast<char (__thiscall *)(void *, int, void *, void *)>(0x10120590u)(a0, a1, a2, a3); }
XI_ORIG char * sub_10120660(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(0x10120660u)(a0); }
XI_ORIG bool sub_10120730(void * a0, unsigned __int8 a1, int a2, void * a3, void * a4, void * a5) { return reinterpret_cast<bool (__thiscall *)(void *, unsigned __int8, int, void *, void *, void *)>(0x10120730u)(a0, a1, a2, a3, a4, a5); }
XI_ORIG char sub_101207B0(void * a0, int a1, void * a2, void * a3) { return reinterpret_cast<char (__thiscall *)(void *, int, void *, void *)>(0x101207B0u)(a0, a1, a2, a3); }
XI_ORIG char * sub_10120880(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(0x10120880u)(a0); }
XI_ORIG __int16 sub_101245C0(void * a0) { return reinterpret_cast<__int16 (__thiscall *)(void *)>(0x101245C0u)(a0); }
XI_ORIG void sub_10124700(int a0, void * a1, float a2) { reinterpret_cast<void (__thiscall *)(int, void *, float)>(0x10124700u)(a0, a1, a2); }
XI_ORIG int sub_101262A0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x101262A0u)(a0); }
XI_ORIG char sub_10127C00(int a0, void * a1, void * a2, int a3, char a4, void * a5) { return reinterpret_cast<char (__thiscall *)(int, void *, void *, int, char, void *)>(0x10127C00u)(a0, a1, a2, a3, a4, a5); }
XI_ORIG char sub_10159400(unsigned int a0, int a1) { return reinterpret_cast<char (__cdecl *)(unsigned int, int)>(0x10159400u)(a0, a1); }
XI_ORIG int sub_1015D900(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(0x1015D900u)(a0); }
XI_ORIG _DWORD * sub_1015E350(void * a0, void * a1, char a2, char a3) { return reinterpret_cast<_DWORD * (__thiscall *)(void *, void *, char, char)>(0x1015E350u)(a0, a1, a2, a3); }
XI_ORIG int sub_10160250(void * a0, void * a1, void * a2) { return reinterpret_cast<int (__thiscall *)(void *, void *, void *)>(0x10160250u)(a0, a1, a2); }
XI_ORIG double sub_10160290(void * a0, char a1) { return reinterpret_cast<double (__thiscall *)(void *, char)>(0x10160290u)(a0, a1); }
XI_ORIG double sub_101602C0(void * a0, char a1) { return reinterpret_cast<double (__thiscall *)(void *, char)>(0x101602C0u)(a0, a1); }
XI_ORIG int sub_101604B0() { return reinterpret_cast<int (__cdecl *)()>(0x101604B0u)(); }
XI_ORIG int sub_10163390(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10163390u)(a0); }
XI_ORIG _WORD * GameSystems_InitializeAll() { return reinterpret_cast<_WORD * (__cdecl *)()>(0x10163640u)(); }
XI_ORIG __int16 sub_10164B80(int a0, int a1) { return reinterpret_cast<__int16 (__fastcall *)(int, int)>(0x10164B80u)(a0, a1); }
XI_ORIG void sub_101674B0(void * a0) { reinterpret_cast<void (__thiscall *)(void *)>(0x101674B0u)(a0); }
XI_ORIG int sub_101678D0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x101678D0u)(a0); }
XI_ORIG _DWORD * sub_10167940(void * a0) { return reinterpret_cast<_DWORD * (__thiscall *)(void *)>(0x10167940u)(a0); }
XI_ORIG int sub_10168D20(int a0, void * a1, int a2, float a3) { return reinterpret_cast<int (__stdcall *)(int, void *, int, float)>(0x10168D20u)(a0, a1, a2, a3); }
XI_ORIG float * sub_1016C060(void * a0, int a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, int, void *)>(0x1016C060u)(a0, a1, a2); }
XI_ORIG int sub_1016FC80(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x1016FC80u)(a0); }
XI_ORIG int sub_10171920(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10171920u)(a0); }
XI_ORIG _DWORD * sub_10175740(void * a0) { return reinterpret_cast<_DWORD * (__cdecl *)(void *)>(0x10175740u)(a0); }
XI_ORIG float * sub_10175780(void * a0, int a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, int, void *)>(0x10175780u)(a0, a1, a2); }
XI_ORIG int sub_10176EC0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10176EC0u)(a0); }
XI_ORIG int sub_10176FA0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10176FA0u)(a0); }
XI_ORIG int sub_10177020(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10177020u)(a0); }
XI_ORIG int sub_10177340(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10177340u)(a0); }
XI_ORIG unsigned int sub_10177350(void * a0) { return reinterpret_cast<unsigned int (__thiscall *)(void *)>(0x10177350u)(a0); }
XI_ORIG int sub_10177400(unsigned int a0) { return reinterpret_cast<int (__cdecl *)(unsigned int)>(0x10177400u)(a0); }
XI_ORIG float sub_10177420(void * a0) { return reinterpret_cast<float (__thiscall *)(void *)>(0x10177420u)(a0); }
XI_ORIG _DWORD * sub_10178760(int a0) { return reinterpret_cast<_DWORD * (__stdcall *)(int)>(0x10178760u)(a0); }
XI_ORIG int sub_10178B00(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10178B00u)(a0); }
XI_ORIG int sub_10179390(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10179390u)(a0); }
XI_ORIG int sub_10179410(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x10179410u)(a0); }
XI_ORIG int sub_1017B1E0(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(0x1017B1E0u)(a0); }
XI_ORIG char sub_101813F0(void * a0, int a1, float a2, int a3, float a4, void * a5) { return reinterpret_cast<char (__thiscall *)(void *, int, float, int, float, void *)>(0x101813F0u)(a0, a1, a2, a3, a4, a5); }
XI_ORIG _DWORD * sub_10183DA0(int a0, int a1, int a2) { return reinterpret_cast<_DWORD * (__thiscall *)(int, int, int)>(0x10183DA0u)(a0, a1, a2); }
XI_ORIG char * sub_10186F20(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(0x10186F20u)(a0); }
XI_ORIG char * sub_10186FA0(void * a0) { return reinterpret_cast<char * (__thiscall *)(void *)>(0x10186FA0u)(a0); }
XI_ORIG _DWORD * sub_10188B70(int a0, void * a1, void * a2) { return reinterpret_cast<_DWORD * (__thiscall *)(int, void *, void *)>(0x10188B70u)(a0, a1, a2); }
XI_ORIG int sub_1018B9A0(void * a0, void * a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, void *, int, int)>(0x1018B9A0u)(a0, a1, a2, a3); }
XI_ORIG int sub_1018B9E0(void * a0, void * a1, int a2, int a3) { return reinterpret_cast<int (__thiscall *)(void *, void *, int, int)>(0x1018B9E0u)(a0, a1, a2, a3); }
XI_ORIG void sub_1018BA20(void * a0, void * a1, void * a2, void * a3, int a4, int a5) { reinterpret_cast<void (__thiscall *)(void *, void *, void *, void *, int, int)>(0x1018BA20u)(a0, a1, a2, a3, a4, a5); }
XI_ORIG char CXiSchStatus_Destructor(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(0x1018C620u)(a0); }
XI_ORIG int sub_1018CA20(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(0x1018CA20u)(a0); }
XI_ORIG int sub_10193850(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(0x10193850u)(a0); }
XI_ORIG int sub_1019EBB0(int a0) { return reinterpret_cast<int (__thiscall *)(int)>(0x1019EBB0u)(a0); }
XI_ORIG int sub_101AB6D0() { return reinterpret_cast<int (__cdecl *)()>(0x101AB6D0u)(); }
XI_ORIG int sub_101AB720() { return reinterpret_cast<int (__cdecl *)()>(0x101AB720u)(); }
XI_ORIG int sub_101C4AF0(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(0x101C4AF0u)(a0, a1); }
XI_ORIG int sub_101CC4F0() { return reinterpret_cast<int (__cdecl *)()>(0x101CC4F0u)(); }
XI_ORIG char sub_1021D980(int a0) { return reinterpret_cast<char (__thiscall *)(int)>(0x1021D980u)(a0); }
XI_ORIG int sub_1021DDF0(void * a0) { return reinterpret_cast<int (__thiscall *)(void *)>(0x1021DDF0u)(a0); }
XI_ORIG int sub_10239460(void * a0, int a1) { return reinterpret_cast<int (__thiscall *)(void *, int)>(0x10239460u)(a0, a1); }
XI_ORIG int LobbyThread_Start(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(0x10245600u)(a0); }
XI_ORIG int sub_10245B60() { return reinterpret_cast<int (__cdecl *)()>(0x10245B60u)(); }
XI_ORIG int sub_10245BA0() { return reinterpret_cast<int (__cdecl *)()>(0x10245BA0u)(); }
XI_ORIG int StAvatar_ReleaseResources(void * a0, int a1) { return reinterpret_cast<int (__cdecl *)(void *, int)>(0x1024D2C0u)(a0, a1); }
XI_ORIG int sub_1024D790(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(0x1024D790u)(a0); }
XI_ORIG int sub_1026E900() { return reinterpret_cast<int (__cdecl *)()>(0x1026E900u)(); }
XI_ORIG char sub_10271B40() { return reinterpret_cast<char (__cdecl *)()>(0x10271B40u)(); }
XI_ORIG _DWORD * sub_10271E90(void * a0) { return reinterpret_cast<_DWORD * (__cdecl *)(void *)>(0x10271E90u)(a0); }
XI_ORIG int sub_10273370(int a0, void * a1, void * a2) { return reinterpret_cast<int (__cdecl *)(int, void *, void *)>(0x10273370u)(a0, a1, a2); }
XI_ORIG void sub_10273920(int a0) { reinterpret_cast<void (__cdecl *)(int)>(0x10273920u)(a0); }
XI_ORIG int PackVec4(int a0, float a1, int a2, int a3, int a4) { return reinterpret_cast<int (__cdecl *)(int, float, int, int, int)>(0x10274D40u)(a0, a1, a2, a3, a4); }
XI_ORIG int sub_10275150() { return reinterpret_cast<int (__cdecl *)()>(0x10275150u)(); }
XI_ORIG void sub_10275B60(int a0, int a1, int a2, void * a3) { reinterpret_cast<void (__cdecl *)(int, int, int, void *)>(0x10275B60u)(a0, a1, a2, a3); }
XI_ORIG _DWORD * sqgxContext_Create() { return reinterpret_cast<_DWORD * (__cdecl *)()>(0x10276260u)(); }
XI_ORIG void sub_1027EB10(void * a0, void * a1) { reinterpret_cast<void (__cdecl *)(void *, void *)>(0x1027EB10u)(a0, a1); }
XI_ORIG float * sub_1027EF20(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, void *)>(0x1027EF20u)(a0, a1, a2); }
XI_ORIG int sub_1027F070(void * a0, void * a1) { return reinterpret_cast<int (__cdecl *)(void *, void *)>(0x1027F070u)(a0, a1); }
XI_ORIG int sub_1027F0E0(void * a0, int a1) { return reinterpret_cast<int (__cdecl *)(void *, int)>(0x1027F0E0u)(a0, a1); }
XI_ORIG double sub_1027FCC0(int a0) { return reinterpret_cast<double (__cdecl *)(int)>(0x1027FCC0u)(a0); }
XI_ORIG float * sub_10280140(void * a0, void * a1, void * a2) { return reinterpret_cast<float * (__cdecl *)(void *, void *, void *)>(0x10280140u)(a0, a1, a2); }
XI_ORIG int sub_10280DE0(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8) { return reinterpret_cast<int (__cdecl *)(int, int, int, int, int, int, int, int, int)>(0x10280DE0u)(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
XI_ORIG int sub_10281510(int a0, void * a1) { return reinterpret_cast<int (__cdecl *)(int, void *)>(0x10281510u)(a0, a1); }
XI_ORIG int sub_102816E0(int a0, int a1, int a2) { return reinterpret_cast<int (__cdecl *)(int, int, int)>(0x102816E0u)(a0, a1, a2); }
XI_ORIG int sub_10281810(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(0x10281810u)(a0); }
XI_ORIG int sub_10281830(int a0, int a1, void * a2, void * a3) { return reinterpret_cast<int (__cdecl *)(int, int, void *, void *)>(0x10281830u)(a0, a1, a2, a3); }
XI_ORIG int sub_10281B60(int a0, void * a1, void * a2) { return reinterpret_cast<int (__cdecl *)(int, void *, void *)>(0x10281B60u)(a0, a1, a2); }
XI_ORIG _DWORD * sub_10281C20(int a0) { return reinterpret_cast<_DWORD * (__cdecl *)(int)>(0x10281C20u)(a0); }
XI_ORIG int sub_10281C50(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(0x10281C50u)(a0); }
XI_ORIG int sub_10281C80(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(0x10281C80u)(a0); }
XI_ORIG int sub_10282010(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(0x10282010u)(a0); }
XI_ORIG int sub_10282030(void * a0, int a1) { return reinterpret_cast<int (__cdecl *)(void *, int)>(0x10282030u)(a0, a1); }
XI_ORIG int sub_10282190(int a0, char a1) { return reinterpret_cast<int (__cdecl *)(int, char)>(0x10282190u)(a0, a1); }
XI_ORIG int sub_10282350(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(0x10282350u)(a0); }
XI_ORIG int sub_102823B0(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(0x102823B0u)(a0, a1); }
XI_ORIG int sub_10282470(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(0x10282470u)(a0); }
XI_ORIG int sub_102824C0(int a0, int a1, int a2, int a3) { return reinterpret_cast<int (__cdecl *)(int, int, int, int)>(0x102824C0u)(a0, a1, a2, a3); }
XI_ORIG int sub_10282B90() { return reinterpret_cast<int (__cdecl *)()>(0x10282B90u)(); }
XI_ORIG int sub_10283110() { return reinterpret_cast<int (__cdecl *)()>(0x10283110u)(); }
XI_ORIG int sub_10283220() { return reinterpret_cast<int (__cdecl *)()>(0x10283220u)(); }
XI_ORIG int sub_10283330(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(0x10283330u)(a0); }
XI_ORIG int sub_10283380(float a0) { return reinterpret_cast<int (__cdecl *)(float)>(0x10283380u)(a0); }
XI_ORIG int sub_102838A0(float a0, float a1) { return reinterpret_cast<int (__cdecl *)(float, float)>(0x102838A0u)(a0, a1); }
XI_ORIG int sub_10283A10(float a0, float a1, float a2, float a3) { return reinterpret_cast<int (__cdecl *)(float, float, float, float)>(0x10283A10u)(a0, a1, a2, a3); }
XI_ORIG int sub_10283D60(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(0x10283D60u)(a0); }
XI_ORIG int sub_10283E00(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(0x10283E00u)(a0); }
XI_ORIG int sub_10283F70(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(0x10283F70u)(a0); }
XI_ORIG int sub_10284950(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(0x10284950u)(a0); }
XI_ORIG _DWORD * sqgxContext_SelectShaderMode(int a0, int a1) { return reinterpret_cast<_DWORD * (__cdecl *)(int, int)>(0x10284B20u)(a0, a1); }
XI_ORIG int nullsub_32() { return reinterpret_cast<int (__cdecl *)()>(0x10284EB0u)(); }
XI_ORIG int sub_10285850(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(0x10285850u)(a0); }
template <class... R> XI_ORIG void sub_10285890(void * a0, R... rest) { reinterpret_cast<void (__cdecl *)(void *, ...)>(0x10285890u)(a0, rest...); }
XI_ORIG int sub_1028F5F0(int a0, int a1, int a2) { return reinterpret_cast<int (__cdecl *)(int, int, int)>(0x1028F5F0u)(a0, a1, a2); }
XI_ORIG int sub_1028FDD0(void * a0, int a1) { return reinterpret_cast<int (__cdecl *)(void *, int)>(0x1028FDD0u)(a0, a1); }
XI_ORIG int sub_1028FFF0(void * a0, int a1) { return reinterpret_cast<int (__cdecl *)(void *, int)>(0x1028FFF0u)(a0, a1); }
XI_ORIG int sub_102902C0(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(0x102902C0u)(a0, a1); }
XI_ORIG int nullsub_60(_DWORD a0, _DWORD a1) { return reinterpret_cast<int (__cdecl *)(_DWORD, _DWORD)>(0x10290430u)(a0, a1); }
XI_ORIG int sub_10290440(int a0, int a1) { return reinterpret_cast<int (__cdecl *)(int, int)>(0x10290440u)(a0, a1); }
XI_ORIG int sub_102911E0(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(0x102911E0u)(a0); }
XI_ORIG unsigned int sub_102979E0(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, void * a8, float a9, int a10) { return reinterpret_cast<unsigned int (__cdecl *)(int, int, int, int, int, int, int, int, void *, float, int)>(0x102979E0u)(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }
XI_ORIG int sub_10297B10(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, void * a8, float a9, int a10) { return reinterpret_cast<int (__cdecl *)(int, int, int, int, int, int, int, int, void *, float, int)>(0x10297B10u)(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }
XI_ORIG int sub_10297CC0(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(0x10297CC0u)(a0); }
XI_ORIG int sub_10297F90(int a0, void * a1, int a2, int a3, void * a4, int a5, void * a6) { return reinterpret_cast<int (__cdecl *)(int, void *, int, int, void *, int, void *)>(0x10297F90u)(a0, a1, a2, a3, a4, a5, a6); }
XI_ORIG int sub_102981E0() { return reinterpret_cast<int (__cdecl *)()>(0x102981E0u)(); }
XI_ORIG int sub_10298350(int a0) { return reinterpret_cast<int (__cdecl *)(int)>(0x10298350u)(a0); }
XI_ORIG int Direct3DCreate8(_DWORD a0) { return reinterpret_cast<int (__stdcall *)(_DWORD)>(0x102D2600u)(a0); }
XI_ORIG int fn_102D2868(void * a0, void * a1, void * a2, int a3, int a4, int a5, int a6, void * a7, int a8, int a9) { return reinterpret_cast<int (__stdcall *)(void *, void *, void *, int, int, int, int, void *, int, int)>(0x102D2868u)(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9); }
XI_ORIG int fn_102D2FB9(void * a0, unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, _D3DFORMAT a5, __int32 a6, void * a7) { return reinterpret_cast<int (__stdcall *)(void *, unsigned int, unsigned int, unsigned int, unsigned int, _D3DFORMAT, __int32, void *)>(0x102D2FB9u)(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG int fn_102D4515(void * a0, void * a1, void * a2, void * a3, void * a4, void * a5, int a6, int a7) { return reinterpret_cast<int (__stdcall *)(void *, void *, void *, void *, void *, void *, int, int)>(0x102D4515u)(a0, a1, a2, a3, a4, a5, a6, a7); }
XI_ORIG float * fn_102D7FF9(void * a0, void * a1, void * a2, void * a3) { return reinterpret_cast<float * (__stdcall *)(void *, void *, void *, void *)>(0x102D7FF9u)(a0, a1, a2, a3); }
XI_ORIG int D3DXMatrixPerspectiveFovLH_static(int a0, float a1, float a2, float a3, float a4) { return reinterpret_cast<int (__stdcall *)(int, float, float, float, float)>(0x102D83C6u)(a0, a1, a2, a3, a4); }
XI_ORIG int fn_102DAA0D(void * a0, char a1, unsigned int a2, void * a3, void * a4, void * a5) { return reinterpret_cast<int (__stdcall *)(void *, char, unsigned int, void *, void *, void *)>(0x102DAA0Du)(a0, a1, a2, a3, a4, a5); }
XI_ORIG int fn_102DAA71(int a0, void * a1, unsigned int a2) { return reinterpret_cast<int (__stdcall *)(int, void *, unsigned int)>(0x102DAA71u)(a0, a1, a2); }
XI_ORIG int sub_1031077E() { return reinterpret_cast<int (__cdecl *)()>(0x1031077Eu)(); }
XI_ORIG void operator_delete(LPVOID a0) { reinterpret_cast<void (__cdecl *)(LPVOID)>(0x10311BB0u)(a0); }
XI_ORIG void * Allocator_Generic(unsigned int a0) { return reinterpret_cast<void * (__cdecl *)(unsigned int)>(0x10311BBBu)(a0); }
template <class... R> XI_ORIG int _sprintf(void * a0, void * a1, R... rest) { return reinterpret_cast<int (__cdecl *)(void *, void *, ...)>(0x10311FF1u)(a0, a1, rest...); }
XI_ORIG char * _strncpy(void * a0, void * a1, size_t a2) { return reinterpret_cast<char * (__cdecl *)(void *, void *, size_t)>(0x10312050u)(a0, a1, a2); }
XI_ORIG void * _malloc_0(size_t a0) { return reinterpret_cast<void * (__cdecl *)(size_t)>(0x10312F96u)(a0); }
XI_ORIG int _atexit(void * a0) { return reinterpret_cast<int (__cdecl *)(void *)>(0x10313486u)(a0); }
XI_ORIG char * __strlwr(void * a0) { return reinterpret_cast<char * (__cdecl *)(void *)>(0x10315AA8u)(a0); }
}  // namespace orig
// a game function used as a value (callback, table entry): its address in the game image
namespace orig_addr {
constexpr uint32_t sub_10001240 = 0x10001240u;
constexpr uint32_t sub_100014A0 = 0x100014A0u;
constexpr uint32_t sub_100016C0 = 0x100016C0u;
constexpr uint32_t sub_10001B50 = 0x10001B50u;
constexpr uint32_t sub_10001BB0 = 0x10001BB0u;
constexpr uint32_t sub_10001CC0 = 0x10001CC0u;
constexpr uint32_t sub_10001E00 = 0x10001E00u;
constexpr uint32_t sub_10001E10 = 0x10001E10u;
constexpr uint32_t Gfx_ProbeDeviceCaps = 0x10002130u;
constexpr uint32_t sub_10002460 = 0x10002460u;
constexpr uint32_t sub_10002B20 = 0x10002B20u;
constexpr uint32_t sub_10002BC0 = 0x10002BC0u;
constexpr uint32_t sub_10002C00 = 0x10002C00u;
constexpr uint32_t sub_10002D80 = 0x10002D80u;
constexpr uint32_t Gfx_CreateD3DHolder = 0x10002F20u;
constexpr uint32_t sub_10003040 = 0x10003040u;
constexpr uint32_t sub_10003070 = 0x10003070u;
constexpr uint32_t sub_10003110 = 0x10003110u;
constexpr uint32_t Gfx_TryCreateDeviceForMode = 0x100033C0u;
constexpr uint32_t sub_10003460 = 0x10003460u;
constexpr uint32_t Gfx_CreateDevice = 0x100034B0u;
constexpr uint32_t sub_10003D70 = 0x10003D70u;
constexpr uint32_t CDx_Constructor = 0x10008F90u;
constexpr uint32_t sub_10009120 = 0x10009120u;
constexpr uint32_t CDx_Initialize = 0x100091B0u;
constexpr uint32_t sub_10009620 = 0x10009620u;
constexpr uint32_t sub_10009640 = 0x10009640u;
constexpr uint32_t sub_10009670 = 0x10009670u;
constexpr uint32_t CDx_EndClearBegin = 0x100096B0u;
constexpr uint32_t CDx_BeginScene = 0x10009730u;
constexpr uint32_t CDx_EndScene = 0x10009740u;
constexpr uint32_t SetCursorPos = 0x10009770u;
constexpr uint32_t sub_10009780 = 0x10009780u;
constexpr uint32_t sub_100097F0 = 0x100097F0u;
constexpr uint32_t CDx_SetDefaultRenderState = 0x10009920u;
constexpr uint32_t CDx_SetViewport = 0x10009DC0u;
constexpr uint32_t CDx_GetRenderTargetTop = 0x10009E00u;
constexpr uint32_t CDx_PushRenderTarget = 0x10009E20u;
constexpr uint32_t CDx_PushRenderTargetTexture = 0x10009F20u;
constexpr uint32_t CDx_SetTexture = 0x1000A1D0u;
constexpr uint32_t sub_1000A290 = 0x1000A290u;
constexpr uint32_t sub_1000A2B0 = 0x1000A2B0u;
constexpr uint32_t CDx_DrawScreenQuadAtDepth = 0x1000A3B0u;
constexpr uint32_t sub_1000A4F0 = 0x1000A4F0u;
constexpr uint32_t sub_1000A5A0 = 0x1000A5A0u;
constexpr uint32_t CDx_CreateTexture = 0x1000A6E0u;
constexpr uint32_t sub_1000AD70 = 0x1000AD70u;
constexpr uint32_t sub_1000AE40 = 0x1000AE40u;
constexpr uint32_t nullsub_36 = 0x1000BB10u;
constexpr uint32_t nullsub_37 = 0x1000BB20u;
constexpr uint32_t CDx_SetTransform = 0x1000BD50u;
constexpr uint32_t CDx_GetTransformCached = 0x1000BDB0u;
constexpr uint32_t Light_ApplyTimeOfDayTint = 0x1000BE50u;
constexpr uint32_t CDx_SetLight = 0x1000BF30u;
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
constexpr uint32_t sub_1000D040 = 0x1000D040u;
constexpr uint32_t nullsub_38 = 0x1000D620u;
constexpr uint32_t CEnv_IsStereoMode = 0x1000D660u;
constexpr uint32_t sub_10010370 = 0x10010370u;
constexpr uint32_t App_Constructor = 0x10010700u;
constexpr uint32_t sub_10010CE0 = 0x10010CE0u;
constexpr uint32_t sub_10010D00 = 0x10010D00u;
constexpr uint32_t App_InitScene = 0x10010D20u;
constexpr uint32_t sub_10014C10 = 0x10014C10u;
constexpr uint32_t sub_10014CF0 = 0x10014CF0u;
constexpr uint32_t GetFrameCounter = 0x10014D50u;
constexpr uint32_t sub_10014D70 = 0x10014D70u;
constexpr uint32_t sub_10014D80 = 0x10014D80u;
constexpr uint32_t sub_10014D90 = 0x10014D90u;
constexpr uint32_t sub_100150D0 = 0x100150D0u;
constexpr uint32_t sub_100151F0 = 0x100151F0u;
constexpr uint32_t sub_10015220 = 0x10015220u;
constexpr uint32_t sub_10015250 = 0x10015250u;
constexpr uint32_t sub_10015270 = 0x10015270u;
constexpr uint32_t Camera_GetViewportWidth = 0x10015680u;
constexpr uint32_t Camera_GetViewportHeight = 0x10015690u;
constexpr uint32_t sub_10015AB0 = 0x10015AB0u;
constexpr uint32_t sub_10015CB0 = 0x10015CB0u;
constexpr uint32_t sub_10015F50 = 0x10015F50u;
constexpr uint32_t sub_10015F80 = 0x10015F80u;
constexpr uint32_t sub_100160C0 = 0x100160C0u;
constexpr uint32_t sub_10016150 = 0x10016150u;
constexpr uint32_t sub_100161A0 = 0x100161A0u;
constexpr uint32_t sub_10016400 = 0x10016400u;
constexpr uint32_t sub_10016490 = 0x10016490u;
constexpr uint32_t sub_100164C0 = 0x100164C0u;
constexpr uint32_t sub_100165B0 = 0x100165B0u;
constexpr uint32_t sub_10016820 = 0x10016820u;
constexpr uint32_t sub_10016E40 = 0x10016E40u;
constexpr uint32_t sub_10016F10 = 0x10016F10u;
constexpr uint32_t sub_10016F40 = 0x10016F40u;
constexpr uint32_t sub_10016F60 = 0x10016F60u;
constexpr uint32_t sub_10016FF0 = 0x10016FF0u;
constexpr uint32_t sub_10017030 = 0x10017030u;
constexpr uint32_t sub_100170A0 = 0x100170A0u;
constexpr uint32_t sub_100170D0 = 0x100170D0u;
constexpr uint32_t sub_10017100 = 0x10017100u;
constexpr uint32_t sub_10017160 = 0x10017160u;
constexpr uint32_t sub_100177A0 = 0x100177A0u;
constexpr uint32_t nullsub_13 = 0x10017D20u;
constexpr uint32_t nullsub_12 = 0x10017D30u;
constexpr uint32_t nullsub_90 = 0x10017D50u;
constexpr uint32_t sub_10018A50 = 0x10018A50u;
constexpr uint32_t sub_10018B30 = 0x10018B30u;
constexpr uint32_t sub_10018C50 = 0x10018C50u;
constexpr uint32_t sub_10018CE0 = 0x10018CE0u;
constexpr uint32_t sub_10019140 = 0x10019140u;
constexpr uint32_t sub_100191D0 = 0x100191D0u;
constexpr uint32_t sub_1001A660 = 0x1001A660u;
constexpr uint32_t sub_1001DD00 = 0x1001DD00u;
constexpr uint32_t sub_1001E0A0 = 0x1001E0A0u;
constexpr uint32_t sub_1001E470 = 0x1001E470u;
constexpr uint32_t sub_1001E4B0 = 0x1001E4B0u;
constexpr uint32_t sub_10022120 = 0x10022120u;
constexpr uint32_t sub_10022190 = 0x10022190u;
constexpr uint32_t sub_100221E0 = 0x100221E0u;
constexpr uint32_t sub_10022230 = 0x10022230u;
constexpr uint32_t sub_10022270 = 0x10022270u;
constexpr uint32_t sub_10022300 = 0x10022300u;
constexpr uint32_t sub_10022340 = 0x10022340u;
constexpr uint32_t CYyModelDt_PrepareRenderState = 0x100224D0u;
constexpr uint32_t sub_10022FA0 = 0x10022FA0u;
constexpr uint32_t sub_10023010 = 0x10023010u;
constexpr uint32_t sub_100230D0 = 0x100230D0u;
constexpr uint32_t CYyModelDt_PrepareRenderStateSimple = 0x100231C0u;
constexpr uint32_t SkinVerticesSimple = 0x100231F0u;
constexpr uint32_t SkinVertices = 0x10023290u;
constexpr uint32_t sub_100250C0 = 0x100250C0u;
constexpr uint32_t sub_100252F0 = 0x100252F0u;
constexpr uint32_t sub_10025330 = 0x10025330u;
constexpr uint32_t sub_100255C0 = 0x100255C0u;
constexpr uint32_t sub_10025860 = 0x10025860u;
constexpr uint32_t sub_10025C40 = 0x10025C40u;
constexpr uint32_t sub_10025F70 = 0x10025F70u;
constexpr uint32_t sub_10026890 = 0x10026890u;
constexpr uint32_t sub_10026C70 = 0x10026C70u;
constexpr uint32_t sub_10026E10 = 0x10026E10u;
constexpr uint32_t sub_10026E30 = 0x10026E30u;
constexpr uint32_t sub_10026E90 = 0x10026E90u;
constexpr uint32_t Vec3_Copy = 0x10026EB0u;
constexpr uint32_t Vec3_AddInPlace = 0x10026F20u;
constexpr uint32_t sub_10026FA0 = 0x10026FA0u;
constexpr uint32_t sub_10026FD0 = 0x10026FD0u;
constexpr uint32_t sub_10027050 = 0x10027050u;
constexpr uint32_t fn_100270A0 = 0x100270A0u;
constexpr uint32_t sub_10027120 = 0x10027120u;
constexpr uint32_t sub_100272E0 = 0x100272E0u;
constexpr uint32_t sub_10027310 = 0x10027310u;
constexpr uint32_t sub_100273D0 = 0x100273D0u;
constexpr uint32_t sub_100276F0 = 0x100276F0u;
constexpr uint32_t sub_100277E0 = 0x100277E0u;
constexpr uint32_t sub_10027870 = 0x10027870u;
constexpr uint32_t sub_100278E0 = 0x100278E0u;
constexpr uint32_t fn_10027990 = 0x10027990u;
constexpr uint32_t nullsub_1 = 0x100279A0u;
constexpr uint32_t sub_100279B0 = 0x100279B0u;
constexpr uint32_t Mat4_Copy = 0x100279F0u;
constexpr uint32_t sub_10027A60 = 0x10027A60u;
constexpr uint32_t Mat4_BuildScale = 0x10027AE0u;
constexpr uint32_t sub_10027B30 = 0x10027B30u;
constexpr uint32_t sub_10027B80 = 0x10027B80u;
constexpr uint32_t sub_10027BD0 = 0x10027BD0u;
constexpr uint32_t sub_10027C20 = 0x10027C20u;
constexpr uint32_t sub_10027CD0 = 0x10027CD0u;
constexpr uint32_t sub_10027CF0 = 0x10027CF0u;
constexpr uint32_t sub_10027D10 = 0x10027D10u;
constexpr uint32_t sub_10027E90 = 0x10027E90u;
constexpr uint32_t sub_10028140 = 0x10028140u;
constexpr uint32_t sub_10028200 = 0x10028200u;
constexpr uint32_t sub_10028290 = 0x10028290u;
constexpr uint32_t sub_100282C0 = 0x100282C0u;
constexpr uint32_t sub_10028320 = 0x10028320u;
constexpr uint32_t sub_100283C0 = 0x100283C0u;
constexpr uint32_t sub_10028640 = 0x10028640u;
constexpr uint32_t sub_100286B0 = 0x100286B0u;
constexpr uint32_t sub_10028700 = 0x10028700u;
constexpr uint32_t SharedDeleteHelper = 0x10028710u;
constexpr uint32_t sub_10028B90 = 0x10028B90u;
constexpr uint32_t sub_10028FD0 = 0x10028FD0u;
constexpr uint32_t sub_10029E50 = 0x10029E50u;
constexpr uint32_t sub_10029E70 = 0x10029E70u;
constexpr uint32_t sub_10029F70 = 0x10029F70u;
constexpr uint32_t sub_1002A3B0 = 0x1002A3B0u;
constexpr uint32_t sub_1002AC50 = 0x1002AC50u;
constexpr uint32_t sub_1002B4B0 = 0x1002B4B0u;
constexpr uint32_t sub_1002B550 = 0x1002B550u;
constexpr uint32_t sub_1002B5A0 = 0x1002B5A0u;
constexpr uint32_t sub_1002B6A0 = 0x1002B6A0u;
constexpr uint32_t sub_1002B6E0 = 0x1002B6E0u;
constexpr uint32_t sub_1002C2D0 = 0x1002C2D0u;
constexpr uint32_t sub_1002C3E0 = 0x1002C3E0u;
constexpr uint32_t sub_1002C570 = 0x1002C570u;
constexpr uint32_t sub_1002C5C0 = 0x1002C5C0u;
constexpr uint32_t sub_1002C7C0 = 0x1002C7C0u;
constexpr uint32_t sub_1002C890 = 0x1002C890u;
constexpr uint32_t sub_1002C8B0 = 0x1002C8B0u;
constexpr uint32_t sub_1002F130 = 0x1002F130u;
constexpr uint32_t sub_1002F1A0 = 0x1002F1A0u;
constexpr uint32_t sub_1002F1C0 = 0x1002F1C0u;
constexpr uint32_t sub_1002F1D0 = 0x1002F1D0u;
constexpr uint32_t sub_10032FA0 = 0x10032FA0u;
constexpr uint32_t sub_10033800 = 0x10033800u;
constexpr uint32_t sub_10033A70 = 0x10033A70u;
constexpr uint32_t sub_10033EA0 = 0x10033EA0u;
constexpr uint32_t sub_10035B00 = 0x10035B00u;
constexpr uint32_t sub_10035F00 = 0x10035F00u;
constexpr uint32_t sub_10036070 = 0x10036070u;
constexpr uint32_t sub_100384B0 = 0x100384B0u;
constexpr uint32_t sub_10038720 = 0x10038720u;
constexpr uint32_t sub_10039810 = 0x10039810u;
constexpr uint32_t sub_100398D0 = 0x100398D0u;
constexpr uint32_t sub_100398E0 = 0x100398E0u;
constexpr uint32_t sub_10039970 = 0x10039970u;
constexpr uint32_t TexObj_CreateAndUpload = 0x10039A30u;
constexpr uint32_t sub_1003AA90 = 0x1003AA90u;
constexpr uint32_t sub_1003AC80 = 0x1003AC80u;
constexpr uint32_t TexMgr_FindByName = 0x1003AEA0u;
constexpr uint32_t sub_1003AF10 = 0x1003AF10u;
constexpr uint32_t sub_1003B210 = 0x1003B210u;
constexpr uint32_t sub_1003B3C0 = 0x1003B3C0u;
constexpr uint32_t sub_1003B450 = 0x1003B450u;
constexpr uint32_t ReleaseSubObjectIfPresent = 0x1003B5C0u;
constexpr uint32_t sub_1003B640 = 0x1003B640u;
constexpr uint32_t sub_1003B750 = 0x1003B750u;
constexpr uint32_t sub_1003B780 = 0x1003B780u;
constexpr uint32_t sub_1003B7B0 = 0x1003B7B0u;
constexpr uint32_t sub_1003B830 = 0x1003B830u;
constexpr uint32_t CMoCamera_GetRuntimeClass = 0x1003C880u;
constexpr uint32_t sub_1003D640 = 0x1003D640u;
constexpr uint32_t sub_1003D980 = 0x1003D980u;
constexpr uint32_t unknown_libname_18 = 0x1003DB50u;
constexpr uint32_t unknown_libname_19 = 0x1003DDF0u;
constexpr uint32_t sub_1003E280 = 0x1003E280u;
constexpr uint32_t sub_1003E2A0 = 0x1003E2A0u;
constexpr uint32_t sub_1003EF20 = 0x1003EF20u;
constexpr uint32_t CMoDistMorphElem_ApplyMorph = 0x1003F440u;
constexpr uint32_t CMoDistMorphElem_ComputeMorphHandle = 0x1003F8A0u;
constexpr uint32_t CXi471011C8_VT_1032AC90_Dtor = 0x1003FB10u;
constexpr uint32_t sub_1003FCB0 = 0x1003FCB0u;
constexpr uint32_t sub_1003FD70 = 0x1003FD70u;
constexpr uint32_t sub_10040670 = 0x10040670u;
constexpr uint32_t CXi471011C8_VT_1032AD80_Dtor = 0x10041110u;
constexpr uint32_t unknown_libname_43 = 0x10041F30u;
constexpr uint32_t sub_100425F0 = 0x100425F0u;
constexpr uint32_t CMoDistMorphElem_ValidateMorphData = 0x10042D00u;
constexpr uint32_t unknown_libname_50 = 0x100439B0u;
constexpr uint32_t CMoTask_VT_1032AF60_ScalarDeletingDtor = 0x10043A90u;
constexpr uint32_t sub_10043AB0 = 0x10043AB0u;
constexpr uint32_t CMoTask_VT_1032AFA8_ScalarDeletingDtor = 0x10043C90u;
constexpr uint32_t sub_10043CB0 = 0x10043CB0u;
constexpr uint32_t sub_10043DF0 = 0x10043DF0u;
constexpr uint32_t sub_10043E40 = 0x10043E40u;
constexpr uint32_t sub_10043E50 = 0x10043E50u;
constexpr uint32_t sub_10044010 = 0x10044010u;
constexpr uint32_t sub_10044130 = 0x10044130u;
constexpr uint32_t sub_100441A0 = 0x100441A0u;
constexpr uint32_t sub_10044210 = 0x10044210u;
constexpr uint32_t sub_10044230 = 0x10044230u;
constexpr uint32_t sub_10044270 = 0x10044270u;
constexpr uint32_t sub_10044890 = 0x10044890u;
constexpr uint32_t CMoElemFamily_SharedCleanup = 0x10044970u;
constexpr uint32_t RegisterWithProcessor = 0x10045D40u;
constexpr uint32_t sub_10047F80 = 0x10047F80u;
constexpr uint32_t CMoGeneratorClone_VT_1032B114_ScalarDeletingDtor = 0x10047FE0u;
constexpr uint32_t sub_10048000 = 0x10048000u;
constexpr uint32_t sub_10048140 = 0x10048140u;
constexpr uint32_t sub_100481D0 = 0x100481D0u;
constexpr uint32_t sub_100492F0 = 0x100492F0u;
constexpr uint32_t sub_10049570 = 0x10049570u;
constexpr uint32_t SharedConditionalStateReset = 0x100531D0u;
constexpr uint32_t sub_10053450 = 0x10053450u;
constexpr uint32_t CMoGeneratorClone_CreateInstance = 0x100536C0u;
constexpr uint32_t CMoGeneratorClone_VT_1032B69C_ScalarDeletingDtor = 0x10053840u;
constexpr uint32_t sub_100541C0 = 0x100541C0u;
constexpr uint32_t CMoGeneratorClone_GetRuntimeClass = 0x100542E0u;
constexpr uint32_t sub_100542F0 = 0x100542F0u;
constexpr uint32_t sub_100548C0 = 0x100548C0u;
constexpr uint32_t sub_10055000 = 0x10055000u;
constexpr uint32_t sub_10055240 = 0x10055240u;
constexpr uint32_t sub_10055250 = 0x10055250u;
constexpr uint32_t sub_100552E0 = 0x100552E0u;
constexpr uint32_t sub_100555D0 = 0x100555D0u;
constexpr uint32_t sub_10056220 = 0x10056220u;
constexpr uint32_t sub_10056390 = 0x10056390u;
constexpr uint32_t CMoPathDriveActorTask_GetRuntimeClass = 0x10056940u;
constexpr uint32_t CMoLockColorDriveTask_GetRuntimeClass = 0x10056960u;
constexpr uint32_t CMoLockLookAtDriveTask_GetRuntimeClass = 0x10056980u;
constexpr uint32_t CMoActorColorDriveTask_GetRuntimeClass = 0x100569A0u;
constexpr uint32_t CMoActorRotationDriveTask_GetRuntimeClass = 0x100569C0u;
constexpr uint32_t Scheduler_Execute = 0x10056C70u;
constexpr uint32_t sub_100574A0 = 0x100574A0u;
constexpr uint32_t sub_1005F2C0 = 0x1005F2C0u;
constexpr uint32_t sub_1005F3F0 = 0x1005F3F0u;
constexpr uint32_t sub_1005F660 = 0x1005F660u;
constexpr uint32_t sub_1005F990 = 0x1005F990u;
constexpr uint32_t sub_1005FE30 = 0x1005FE30u;
constexpr uint32_t sub_10060190 = 0x10060190u;
constexpr uint32_t sub_100602D0 = 0x100602D0u;
constexpr uint32_t sub_100608B0 = 0x100608B0u;
constexpr uint32_t sub_10060BB0 = 0x10060BB0u;
constexpr uint32_t sub_10060E60 = 0x10060E60u;
constexpr uint32_t sub_10061010 = 0x10061010u;
constexpr uint32_t sub_10061140 = 0x10061140u;
constexpr uint32_t sub_10061300 = 0x10061300u;
constexpr uint32_t sub_100614B0 = 0x100614B0u;
constexpr uint32_t sub_100616B0 = 0x100616B0u;
constexpr uint32_t sub_10061830 = 0x10061830u;
constexpr uint32_t sub_10061A00 = 0x10061A00u;
constexpr uint32_t sub_10061B80 = 0x10061B80u;
constexpr uint32_t sub_10061D00 = 0x10061D00u;
constexpr uint32_t sub_10061EA0 = 0x10061EA0u;
constexpr uint32_t sub_10062090 = 0x10062090u;
constexpr uint32_t sub_10062210 = 0x10062210u;
constexpr uint32_t sub_100623B0 = 0x100623B0u;
constexpr uint32_t sub_10062550 = 0x10062550u;
constexpr uint32_t sub_10062700 = 0x10062700u;
constexpr uint32_t sub_100634A0 = 0x100634A0u;
constexpr uint32_t sub_10063C60 = 0x10063C60u;
constexpr uint32_t sub_10069D70 = 0x10069D70u;
constexpr uint32_t sub_10069DD0 = 0x10069DD0u;
constexpr uint32_t sub_1006A150 = 0x1006A150u;
constexpr uint32_t sub_1006A480 = 0x1006A480u;
constexpr uint32_t sub_1006A4A0 = 0x1006A4A0u;
constexpr uint32_t sub_1006A4C0 = 0x1006A4C0u;
constexpr uint32_t sub_1006A4F0 = 0x1006A4F0u;
constexpr uint32_t sub_1006A520 = 0x1006A520u;
constexpr uint32_t sub_1006A560 = 0x1006A560u;
constexpr uint32_t fn_1006AD90 = 0x1006AD90u;
constexpr uint32_t sub_1006AE30 = 0x1006AE30u;
constexpr uint32_t sub_1006AF90 = 0x1006AF90u;
constexpr uint32_t sub_1006B050 = 0x1006B050u;
constexpr uint32_t sub_1006B090 = 0x1006B090u;
constexpr uint32_t sub_1006B0C0 = 0x1006B0C0u;
constexpr uint32_t sub_1006B250 = 0x1006B250u;
constexpr uint32_t sub_1006BD90 = 0x1006BD90u;
constexpr uint32_t sub_1006C0F0 = 0x1006C0F0u;
constexpr uint32_t sub_1006C110 = 0x1006C110u;
constexpr uint32_t sub_1006C140 = 0x1006C140u;
constexpr uint32_t sub_1006C1E0 = 0x1006C1E0u;
constexpr uint32_t j_OcclusionMng_CreateTarget = 0x1006C210u;
constexpr uint32_t OcclusionMng_CreateTarget = 0x1006C220u;
constexpr uint32_t sub_1006C260 = 0x1006C260u;
constexpr uint32_t sub_1006C270 = 0x1006C270u;
constexpr uint32_t Occlusion_QueryScreenRect = 0x1006C280u;
constexpr uint32_t Occlusion_QueryScreenRectImpl = 0x1006C300u;
constexpr uint32_t sub_1006C9B0 = 0x1006C9B0u;
constexpr uint32_t nullsub_44 = 0x1006CF90u;
constexpr uint32_t CMoProcessor_VT_1032C368_vf012 = 0x1006D3B0u;
constexpr uint32_t sub_1006D5D0 = 0x1006D5D0u;
constexpr uint32_t sub_1006D7E0 = 0x1006D7E0u;
constexpr uint32_t sub_1006D870 = 0x1006D870u;
constexpr uint32_t sub_1006D900 = 0x1006D900u;
constexpr uint32_t CMoProcessor_VT_1032C368_vf021 = 0x1006D980u;
constexpr uint32_t sub_1006E7D0 = 0x1006E7D0u;
constexpr uint32_t sub_1006E970 = 0x1006E970u;
constexpr uint32_t sub_1006EB70 = 0x1006EB70u;
constexpr uint32_t sub_1006ED10 = 0x1006ED10u;
constexpr uint32_t sub_10070F80 = 0x10070F80u;
constexpr uint32_t sub_10071050 = 0x10071050u;
constexpr uint32_t ResourceHandle_IsResident = 0x10071060u;
constexpr uint32_t sub_10071180 = 0x10071180u;
constexpr uint32_t nullsub_171 = 0x100719B0u;
constexpr uint32_t sub_10071C10 = 0x10071C10u;
constexpr uint32_t sub_10071C60 = 0x10071C60u;
constexpr uint32_t sub_10071DE0 = 0x10071DE0u;
constexpr uint32_t sub_10071E90 = 0x10071E90u;
constexpr uint32_t sub_10071EC0 = 0x10071EC0u;
constexpr uint32_t sub_10071F50 = 0x10071F50u;
constexpr uint32_t sub_10072030 = 0x10072030u;
constexpr uint32_t sub_100721F0 = 0x100721F0u;
constexpr uint32_t sub_10072270 = 0x10072270u;
constexpr uint32_t sub_10072290 = 0x10072290u;
constexpr uint32_t sub_10072430 = 0x10072430u;
constexpr uint32_t YmResourceFile_FindResourceUnder = 0x100724C0u;
constexpr uint32_t sub_100729E0 = 0x100729E0u;
constexpr uint32_t sub_10072ED0 = 0x10072ED0u;
constexpr uint32_t ResourceHandle_Deref = 0x10072FB0u;
constexpr uint32_t sub_10073B20 = 0x10073B20u;
constexpr uint32_t sub_10074140 = 0x10074140u;
constexpr uint32_t sub_10074180 = 0x10074180u;
constexpr uint32_t sub_10074290 = 0x10074290u;
constexpr uint32_t ResourceIter_FindFirst = 0x100742B0u;
constexpr uint32_t ResourceIter_FindNext = 0x100745C0u;
constexpr uint32_t sub_100746A0 = 0x100746A0u;
constexpr uint32_t sub_10074740 = 0x10074740u;
constexpr uint32_t sub_100747C0 = 0x100747C0u;
constexpr uint32_t sub_100748F0 = 0x100748F0u;
constexpr uint32_t sub_100749B0 = 0x100749B0u;
constexpr uint32_t sub_10074A10 = 0x10074A10u;
constexpr uint32_t sub_10074B60 = 0x10074B60u;
constexpr uint32_t sub_10075280 = 0x10075280u;
constexpr uint32_t sub_10075520 = 0x10075520u;
constexpr uint32_t sub_10075960 = 0x10075960u;
constexpr uint32_t sub_100814A0 = 0x100814A0u;
constexpr uint32_t ActorHandle_Resolve = 0x10081550u;
constexpr uint32_t sub_10081A50 = 0x10081A50u;
constexpr uint32_t CXiActor_NameCalc = 0x100830C0u;
constexpr uint32_t sub_10084140 = 0x10084140u;
constexpr uint32_t sub_10084330 = 0x10084330u;
constexpr uint32_t sub_10084350 = 0x10084350u;
constexpr uint32_t sub_10084370 = 0x10084370u;
constexpr uint32_t sub_10084390 = 0x10084390u;
constexpr uint32_t sub_100843B0 = 0x100843B0u;
constexpr uint32_t sub_10084400 = 0x10084400u;
constexpr uint32_t sub_10084420 = 0x10084420u;
constexpr uint32_t sub_10084480 = 0x10084480u;
constexpr uint32_t sub_10084500 = 0x10084500u;
constexpr uint32_t sub_10084520 = 0x10084520u;
constexpr uint32_t sub_10084540 = 0x10084540u;
constexpr uint32_t sub_10084580 = 0x10084580u;
constexpr uint32_t sub_100845B0 = 0x100845B0u;
constexpr uint32_t sub_10084640 = 0x10084640u;
constexpr uint32_t sub_10084720 = 0x10084720u;
constexpr uint32_t sub_10085060 = 0x10085060u;
constexpr uint32_t sub_10085490 = 0x10085490u;
constexpr uint32_t sub_10086210 = 0x10086210u;
constexpr uint32_t sub_1008A0D0 = 0x1008A0D0u;
constexpr uint32_t sub_10093720 = 0x10093720u;
constexpr uint32_t sub_10093DB0 = 0x10093DB0u;
constexpr uint32_t sub_10097840 = 0x10097840u;
constexpr uint32_t sub_10097A40 = 0x10097A40u;
constexpr uint32_t sub_10098320 = 0x10098320u;
constexpr uint32_t sub_100A2810 = 0x100A2810u;
constexpr uint32_t sub_100A4C20 = 0x100A4C20u;
constexpr uint32_t sub_100A9D30 = 0x100A9D30u;
constexpr uint32_t CMoTask_VT_1032E808_ScalarDeletingDtor = 0x100A9DB0u;
constexpr uint32_t sub_100C5410 = 0x100C5410u;
constexpr uint32_t sub_100C5440 = 0x100C5440u;
constexpr uint32_t sub_100C54C0 = 0x100C54C0u;
constexpr uint32_t CXiSkeletonActor_Init = 0x100C5D60u;
constexpr uint32_t sub_100CB4E0 = 0x100CB4E0u;
constexpr uint32_t PerActor_FourBucketStaggeredUpdate = 0x100CBDE0u;
constexpr uint32_t nullsub_276 = 0x100CCE20u;
constexpr uint32_t nullsub_277 = 0x100CCE30u;
constexpr uint32_t nullsub_278 = 0x100CCE40u;
constexpr uint32_t nullsub_279 = 0x100CCE50u;
constexpr uint32_t sub_100CCE60 = 0x100CCE60u;
constexpr uint32_t CXiSkeletonActor_ReverseFindRes = 0x100CE480u;
constexpr uint32_t sub_100D0880 = 0x100D0880u;
constexpr uint32_t sub_100D08B0 = 0x100D08B0u;
constexpr uint32_t CXiSkeletonActor_IsReadCompleteBody = 0x100D12D0u;
constexpr uint32_t CXiSkeletonActor_OnSkeletonLoaded = 0x100D2F40u;
constexpr uint32_t CXiSkeletonActor_LoadSkeletonSync = 0x100D2F60u;
constexpr uint32_t CXiSkeletonActor_LoadSkeletonResource = 0x100D4330u;
constexpr uint32_t sub_100D54C0 = 0x100D54C0u;
constexpr uint32_t sub_100D5B10 = 0x100D5B10u;
constexpr uint32_t sub_100D6D30 = 0x100D6D30u;
constexpr uint32_t CXiSkeletonActorRes_Constructor = 0x100D7020u;
constexpr uint32_t CXiSkeletonActorRes_Destructor = 0x100D7050u;
constexpr uint32_t CXiSkeletonActorRes_GetLinkedActor = 0x100D7090u;
constexpr uint32_t sub_100DB100 = 0x100DB100u;
constexpr uint32_t sub_100DB140 = 0x100DB140u;
constexpr uint32_t sub_100DB170 = 0x100DB170u;
constexpr uint32_t sub_100DBB50 = 0x100DBB50u;
constexpr uint32_t sub_1011BAE0 = 0x1011BAE0u;
constexpr uint32_t sub_1011BDC0 = 0x1011BDC0u;
constexpr uint32_t sub_1011BF40 = 0x1011BF40u;
constexpr uint32_t sub_1011C1F0 = 0x1011C1F0u;
constexpr uint32_t sub_1011C5D0 = 0x1011C5D0u;
constexpr uint32_t sub_1011C670 = 0x1011C670u;
constexpr uint32_t sub_1011C720 = 0x1011C720u;
constexpr uint32_t sub_1011C810 = 0x1011C810u;
constexpr uint32_t sub_1011C850 = 0x1011C850u;
constexpr uint32_t sub_1011CE80 = 0x1011CE80u;
constexpr uint32_t sub_1011D840 = 0x1011D840u;
constexpr uint32_t sub_1011DA80 = 0x1011DA80u;
constexpr uint32_t sub_1011F2D0 = 0x1011F2D0u;
constexpr uint32_t sub_1011F300 = 0x1011F300u;
constexpr uint32_t sub_1011FC80 = 0x1011FC80u;
constexpr uint32_t sub_1011FF70 = 0x1011FF70u;
constexpr uint32_t sub_101200B0 = 0x101200B0u;
constexpr uint32_t sub_101201C0 = 0x101201C0u;
constexpr uint32_t sub_10120450 = 0x10120450u;
constexpr uint32_t sub_10120510 = 0x10120510u;
constexpr uint32_t sub_10120590 = 0x10120590u;
constexpr uint32_t sub_10120660 = 0x10120660u;
constexpr uint32_t sub_10120730 = 0x10120730u;
constexpr uint32_t sub_101207B0 = 0x101207B0u;
constexpr uint32_t sub_10120880 = 0x10120880u;
constexpr uint32_t sub_101245C0 = 0x101245C0u;
constexpr uint32_t sub_10124700 = 0x10124700u;
constexpr uint32_t sub_101262A0 = 0x101262A0u;
constexpr uint32_t sub_10127C00 = 0x10127C00u;
constexpr uint32_t sub_10159400 = 0x10159400u;
constexpr uint32_t sub_1015D900 = 0x1015D900u;
constexpr uint32_t sub_1015E350 = 0x1015E350u;
constexpr uint32_t sub_10160250 = 0x10160250u;
constexpr uint32_t sub_10160290 = 0x10160290u;
constexpr uint32_t sub_101602C0 = 0x101602C0u;
constexpr uint32_t sub_101604B0 = 0x101604B0u;
constexpr uint32_t sub_10163390 = 0x10163390u;
constexpr uint32_t GameSystems_InitializeAll = 0x10163640u;
constexpr uint32_t sub_10164B80 = 0x10164B80u;
constexpr uint32_t sub_101674B0 = 0x101674B0u;
constexpr uint32_t sub_101678D0 = 0x101678D0u;
constexpr uint32_t sub_10167940 = 0x10167940u;
constexpr uint32_t sub_10168D20 = 0x10168D20u;
constexpr uint32_t sub_1016C060 = 0x1016C060u;
constexpr uint32_t sub_1016FC80 = 0x1016FC80u;
constexpr uint32_t sub_10171920 = 0x10171920u;
constexpr uint32_t sub_10175740 = 0x10175740u;
constexpr uint32_t sub_10175780 = 0x10175780u;
constexpr uint32_t sub_10176EC0 = 0x10176EC0u;
constexpr uint32_t sub_10176FA0 = 0x10176FA0u;
constexpr uint32_t sub_10177020 = 0x10177020u;
constexpr uint32_t sub_10177340 = 0x10177340u;
constexpr uint32_t sub_10177350 = 0x10177350u;
constexpr uint32_t sub_10177400 = 0x10177400u;
constexpr uint32_t sub_10177420 = 0x10177420u;
constexpr uint32_t sub_10178760 = 0x10178760u;
constexpr uint32_t sub_10178B00 = 0x10178B00u;
constexpr uint32_t sub_10179390 = 0x10179390u;
constexpr uint32_t sub_10179410 = 0x10179410u;
constexpr uint32_t sub_1017B1E0 = 0x1017B1E0u;
constexpr uint32_t sub_101813F0 = 0x101813F0u;
constexpr uint32_t sub_10183DA0 = 0x10183DA0u;
constexpr uint32_t sub_10186F20 = 0x10186F20u;
constexpr uint32_t sub_10186FA0 = 0x10186FA0u;
constexpr uint32_t sub_10188B70 = 0x10188B70u;
constexpr uint32_t sub_1018B9A0 = 0x1018B9A0u;
constexpr uint32_t sub_1018B9E0 = 0x1018B9E0u;
constexpr uint32_t sub_1018BA20 = 0x1018BA20u;
constexpr uint32_t CXiSchStatus_Destructor = 0x1018C620u;
constexpr uint32_t sub_1018CA20 = 0x1018CA20u;
constexpr uint32_t sub_10193850 = 0x10193850u;
constexpr uint32_t sub_1019EBB0 = 0x1019EBB0u;
constexpr uint32_t sub_101AB6D0 = 0x101AB6D0u;
constexpr uint32_t sub_101AB720 = 0x101AB720u;
constexpr uint32_t sub_101C4AF0 = 0x101C4AF0u;
constexpr uint32_t sub_101CC4F0 = 0x101CC4F0u;
constexpr uint32_t sub_1021D980 = 0x1021D980u;
constexpr uint32_t sub_1021DDF0 = 0x1021DDF0u;
constexpr uint32_t sub_10239460 = 0x10239460u;
constexpr uint32_t LobbyThread_Start = 0x10245600u;
constexpr uint32_t sub_10245B60 = 0x10245B60u;
constexpr uint32_t sub_10245BA0 = 0x10245BA0u;
constexpr uint32_t StAvatar_ReleaseResources = 0x1024D2C0u;
constexpr uint32_t sub_1024D790 = 0x1024D790u;
constexpr uint32_t sub_1026E900 = 0x1026E900u;
constexpr uint32_t sub_10271B40 = 0x10271B40u;
constexpr uint32_t sub_10271E90 = 0x10271E90u;
constexpr uint32_t sub_10273370 = 0x10273370u;
constexpr uint32_t sub_10273920 = 0x10273920u;
constexpr uint32_t PackVec4 = 0x10274D40u;
constexpr uint32_t sub_10275150 = 0x10275150u;
constexpr uint32_t sub_10275B60 = 0x10275B60u;
constexpr uint32_t sqgxContext_Create = 0x10276260u;
constexpr uint32_t sub_1027EB10 = 0x1027EB10u;
constexpr uint32_t sub_1027EF20 = 0x1027EF20u;
constexpr uint32_t sub_1027F070 = 0x1027F070u;
constexpr uint32_t sub_1027F0E0 = 0x1027F0E0u;
constexpr uint32_t sub_1027FCC0 = 0x1027FCC0u;
constexpr uint32_t sub_10280140 = 0x10280140u;
constexpr uint32_t sub_10280DE0 = 0x10280DE0u;
constexpr uint32_t sub_10281510 = 0x10281510u;
constexpr uint32_t sub_102816E0 = 0x102816E0u;
constexpr uint32_t sub_10281810 = 0x10281810u;
constexpr uint32_t sub_10281830 = 0x10281830u;
constexpr uint32_t sub_10281B60 = 0x10281B60u;
constexpr uint32_t sub_10281C20 = 0x10281C20u;
constexpr uint32_t sub_10281C50 = 0x10281C50u;
constexpr uint32_t sub_10281C80 = 0x10281C80u;
constexpr uint32_t sub_10282010 = 0x10282010u;
constexpr uint32_t sub_10282030 = 0x10282030u;
constexpr uint32_t sub_10282190 = 0x10282190u;
constexpr uint32_t sub_10282350 = 0x10282350u;
constexpr uint32_t sub_102823B0 = 0x102823B0u;
constexpr uint32_t sub_10282470 = 0x10282470u;
constexpr uint32_t sub_102824C0 = 0x102824C0u;
constexpr uint32_t sub_10282B90 = 0x10282B90u;
constexpr uint32_t sub_10283110 = 0x10283110u;
constexpr uint32_t sub_10283220 = 0x10283220u;
constexpr uint32_t sub_10283330 = 0x10283330u;
constexpr uint32_t sub_10283380 = 0x10283380u;
constexpr uint32_t sub_102838A0 = 0x102838A0u;
constexpr uint32_t sub_10283A10 = 0x10283A10u;
constexpr uint32_t sub_10283D60 = 0x10283D60u;
constexpr uint32_t sub_10283E00 = 0x10283E00u;
constexpr uint32_t sub_10283F70 = 0x10283F70u;
constexpr uint32_t sub_10284950 = 0x10284950u;
constexpr uint32_t sqgxContext_SelectShaderMode = 0x10284B20u;
constexpr uint32_t nullsub_32 = 0x10284EB0u;
constexpr uint32_t sub_10285850 = 0x10285850u;
constexpr uint32_t sub_10285890 = 0x10285890u;
constexpr uint32_t sub_1028F5F0 = 0x1028F5F0u;
constexpr uint32_t sub_1028FDD0 = 0x1028FDD0u;
constexpr uint32_t sub_1028FFF0 = 0x1028FFF0u;
constexpr uint32_t sub_102902C0 = 0x102902C0u;
constexpr uint32_t nullsub_60 = 0x10290430u;
constexpr uint32_t sub_10290440 = 0x10290440u;
constexpr uint32_t sub_102911E0 = 0x102911E0u;
constexpr uint32_t sub_102979E0 = 0x102979E0u;
constexpr uint32_t sub_10297B10 = 0x10297B10u;
constexpr uint32_t sub_10297CC0 = 0x10297CC0u;
constexpr uint32_t sub_10297F90 = 0x10297F90u;
constexpr uint32_t sub_102981E0 = 0x102981E0u;
constexpr uint32_t sub_10298350 = 0x10298350u;
constexpr uint32_t Direct3DCreate8 = 0x102D2600u;
constexpr uint32_t fn_102D2868 = 0x102D2868u;
constexpr uint32_t fn_102D2FB9 = 0x102D2FB9u;
constexpr uint32_t fn_102D4515 = 0x102D4515u;
constexpr uint32_t fn_102D7FF9 = 0x102D7FF9u;
constexpr uint32_t D3DXMatrixPerspectiveFovLH_static = 0x102D83C6u;
constexpr uint32_t fn_102DAA0D = 0x102DAA0Du;
constexpr uint32_t fn_102DAA71 = 0x102DAA71u;
constexpr uint32_t sub_1031077E = 0x1031077Eu;
constexpr uint32_t operator_delete = 0x10311BB0u;
constexpr uint32_t Allocator_Generic = 0x10311BBBu;
constexpr uint32_t _sprintf = 0x10311FF1u;
constexpr uint32_t _strncpy = 0x10312050u;
constexpr uint32_t _malloc_0 = 0x10312F96u;
constexpr uint32_t _atexit = 0x10313486u;
constexpr uint32_t __strlwr = 0x10315AA8u;
}  // namespace orig_addr
#define XI_CODE(name) reinterpret_cast<void *>(orig_addr::name)
