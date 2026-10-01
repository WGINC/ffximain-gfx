// gfx_free.hpp -- graphics functions with no established class.
#pragma once
#include "gfx_common.hpp"
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
XI_API void * __thiscall fn_100186C0(void *self_, char *Source);  // 0x100186C0
XI_API _DWORD * __cdecl fn_1001D0A0(void);  // 0x1001D0A0
XI_API char __thiscall fn_1001D240(int self_);  // 0x1001D240
XI_API char __thiscall fn_1001D350(_DWORD *self_);  // 0x1001D350
XI_API int __thiscall fn_1001D3B0(void *self_);  // 0x1001D3B0
XI_API char __cdecl fn_1001D940(int a1, int a2, int a3, int a4);  // 0x1001D940
XI_API int __thiscall fn_1001DC70(void *self_, int a1, int a2);  // 0x1001DC70
XI_API int __stdcall fn_1001DD00(int a1, int a2);  // 0x1001DD00
XI_API int __cdecl fn_10022FA0(int a1);  // 0x10022FA0
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
XI_API int __thiscall fn_1002A3B0(int self_, int a2, int a3, int a4);  // 0x1002A3B0
XI_API void __thiscall fn_1002C360(int self_, int a2, int a3);  // 0x1002C360
XI_API char __thiscall fn_1002CCB0(int *self_, int a2);  // 0x1002CCB0
XI_API int __thiscall fn_1002D680(int self_, _DWORD **a2);  // 0x1002D680
XI_API int __cdecl fn_1002F160(unsigned int a1);  // 0x1002F160
XI_API int __cdecl fn_1002F1A0(void);  // 0x1002F1A0
XI_API char __thiscall fn_10033800(int self_, int *a2, int a3);  // 0x10033800
XI_API char __thiscall fn_10033A70(_DWORD *self_, int a2);  // 0x10033A70
XI_API int __thiscall Font_DrawPass(void *self_);  // 0x10038520
XI_API unsigned __int8 * __cdecl fn_10038720(unsigned __int8 *a1, float a2, float a3, float a4);  // 0x10038720
XI_API _DWORD * __cdecl fn_10038930(void);  // 0x10038930
XI_API int __thiscall fn_10038A80(void *self_);  // 0x10038A80
XI_API int __thiscall fn_10038E10(void *self_);  // 0x10038E10
XI_API char __thiscall TexObj_Create(int self_);  // 0x10039A80
XI_API int __thiscall TexObj_Upload(void *self_);  // 0x10039E50
XI_API int __cdecl fn_1003D9A0(void);  // 0x1003D9A0
XI_API int __cdecl fn_1003DB60(void);  // 0x1003DB60
XI_API void __cdecl fn_10041C20(int a1, int a2, int a3, int a4, int a5);  // 0x10041C20
XI_API _DWORD * __cdecl fn_10044270(void);  // 0x10044270
XI_API char ** __cdecl CMoRabRes_GetRuntimeClass(void);  // 0x10056260
XI_API _DWORD * __thiscall fn_1006B2B0(void *self_);  // 0x1006B2B0
XI_API int __thiscall Occlusion_QueryScreenRect(__int16 *self_, float *a2, int a3, float a4, float a5, int a6, int a7, int a8, char a9, int a10);  // 0x1006C280
XI_API int __thiscall OcclusionMng_Prepare(_DWORD *self_);  // 0x1006C9B0
XI_API void __thiscall fn_100755A0(int *self_);  // 0x100755A0
XI_API int __cdecl fn_1011AAC0(void);  // 0x1011AAC0
XI_API char __thiscall fn_1011BAE0(int self_);  // 0x1011BAE0
XI_API int __thiscall fn_1011BBD0(float *self_, __int16 a2, int *a3, unsigned int a4, float a5, float a6);  // 0x1011BBD0
XI_API int __cdecl fn_1011BDC0(void);  // 0x1011BDC0
XI_API int __cdecl fn_1011C1F0(int a1, int a2, int a3, int a4);  // 0x1011C1F0
XI_API void __thiscall fn_1011C720(int *self_);  // 0x1011C720
XI_API int __thiscall fn_1011E800(void *self_, int a1, int a2);  // 0x1011E800
XI_API void __thiscall fn_1011EDE0(int self_, int a2, int a3, float a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16, int a17, int a18, int a19, unsigned int a20, int a21);  // 0x1011EDE0
XI_API int __thiscall fn_1011EF60(int self_, int a2, int a3, int a4, int a5, int a6, int a7, float a8, float a9, unsigned int a10, unsigned int a11, char a12);  // 0x1011EF60
XI_API int __thiscall fn_1011F300(void *self_, int a1, int a2, int a3, int a4, int a5);  // 0x1011F300
XI_API int __thiscall fn_10124440(int self_);  // 0x10124440
XI_API unsigned __int8 __thiscall fn_101252F0(unsigned __int8 *self_, unsigned __int8 a2);  // 0x101252F0
XI_API int __cdecl fn_101604B0(void);  // 0x101604B0
XI_API int __cdecl fn_10160650(void);  // 0x10160650
XI_API int __cdecl fn_101620B0(void);  // 0x101620B0
XI_API int __stdcall fn_10163400(int a1, int a2);  // 0x10163400
XI_API int __stdcall fn_1016DDF0(int a1, int a2, int a3);  // 0x1016DDF0
XI_API void __thiscall fn_10174B10(_DWORD *self_, float *a2);  // 0x10174B10
XI_API int __cdecl fn_10174EC0(void);  // 0x10174EC0
XI_API int __thiscall fn_10177240(void *self_, int a1, int a2);  // 0x10177240
XI_API int __cdecl Gfx_SetZBias(unsigned int a1);  // 0x10177400
XI_API float __thiscall ZoneMap_SetOverlayProjection(float *self_);  // 0x10177420
XI_API int __cdecl fn_101779C0(void);  // 0x101779C0
XI_API int __cdecl fn_10177B40(float *a3, char a4, unsigned __int8 a5, unsigned __int8 a6, char a7, char a8);  // 0x10177B40
XI_API int __cdecl fn_10177C50(int a1, int a2, int a3, int a4, int a5, int a6);  // 0x10177C50
XI_API int __stdcall fn_10177CD0(float *a1, int a2, float a3);  // 0x10177CD0
XI_API int __thiscall fn_101781D0(_DWORD *self_);  // 0x101781D0
XI_API int __thiscall ZoneMap_ReleaseStateBlock(unsigned int *self_);  // 0x10178B00
XI_API int __thiscall fn_101791F0(void *self_);  // 0x101791F0
XI_API int __thiscall fn_101792A0(char *self_);  // 0x101792A0
XI_API int __thiscall fn_1017ADE0(_DWORD *self_, _DWORD *a2);  // 0x1017ADE0
XI_API int __stdcall fn_10182F10(int a1);  // 0x10182F10
XI_API _DWORD * __thiscall Env_ComputeAmbient(char *self_, _DWORD *a2, int a3);  // 0x10188E00
XI_API float * __thiscall fn_10188FA0(char *self_, float *a2);  // 0x10188FA0
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
XI_API int __cdecl sqgxTextureStencilClear(int a1);  // 0x10282010
XI_API int __cdecl sqgxTextureClear(float *a1, int a2);  // 0x10282030
XI_API int __cdecl fn_102820E0(IDirect3DSurface8 *a1);  // 0x102820E0
XI_API int __cdecl sqgxTextureDraw(int a1, char a2);  // 0x10282190
XI_API int __cdecl sqgxZTest(int a1);  // 0x10282350
XI_API int __cdecl fn_102823F0(unsigned int a1, unsigned int a2);  // 0x102823F0
XI_API int __cdecl fn_10282470(int a1);  // 0x10282470
XI_API int __cdecl fn_102824C0(int a1, int a2, int a3, int a4);  // 0x102824C0
XI_API int __cdecl fn_10282640(void);  // 0x10282640
XI_API int __cdecl fn_102826C0(void);  // 0x102826C0
XI_API int __cdecl fn_102827D0(void);  // 0x102827D0
XI_API int __cdecl fn_10282910(void);  // 0x10282910
XI_API int __cdecl sqgxBlendTrsp_Candidate(void);  // 0x10282B90
XI_API int __cdecl fn_10282CB0(void);  // 0x10282CB0
XI_API int __cdecl fn_10282DC0(void);  // 0x10282DC0
XI_API int __cdecl fn_10282ED0(void);  // 0x10282ED0
XI_API int __cdecl fn_10282FF0(void);  // 0x10282FF0
XI_API int __cdecl fn_10283110(void);  // 0x10283110
XI_API int __cdecl sqgxBlendOff(void);  // 0x10283220
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
