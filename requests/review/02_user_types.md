# User-set function types to mark (request 02)

372 functions; from `data/ida_user_types.json` (FFXiMain_recon.i64, 2026-10-21). Fill **Origin** with `mine` (you set this type), `session` (an earlier session set it) or `unsure`.

| address | name | prototype | gate sites | in tree | Origin |
|---|---|---|---|---|---|
| 0x10009E00 | CDx_GetRenderTargetTop | `unsigned __int8 *__thiscall __spoils<eax,edx>(unsigned __int8 *this)` | 96 | yes | |
| 0x1000CD00 | CDx_DrawPrimitiveUP | `int __thiscall(void *self, int a1, int a2, int a3, int a4)` | 76 | yes | |
| 0x100279A0 | nullsub_1 | `void __cdecl __spoils<eax,edx>()` | 68 | yes | |
| 0x10027990 | ??0Iostream_init@@QAE@XZ_0 | `Iostream_init *__thiscall __spoils<eax,ecx>(Iostream_init *__hidden this)` | 54 | yes | |
| 0x10313486 | _atexit | `int __cdecl(void (__cdecl *)())` | 52 | no | |
| 0x1000BF30 | CDx_SetLight | `int __thiscall(CDx *this, int, float *)` | 35 | yes | |
| 0x1000BDB0 | CDx_GetTransformCached | `int __thiscall(CDx *this, int, void *)` | 30 | yes | |
| 0x10071060 | ResourceHandle_IsResident | `bool __fastcall(_DWORD ***a1)` | 30 | yes | |
| 0x10009E20 | CDx_PushRenderTarget | `int __thiscall(CDx *this, int, int, int)` | 27 | yes | |
| 0x1005E510 | sub_1005E510 | `unsigned int __thiscall __spoils<eax,edx>(void *this)` | 23 | no | |
| 0x100544D0 | sub_100544D0 | `float __thiscall(int this, float, int)` | 21 | yes | |
| 0x1000CC00 | CDx_DrawIndexedPrimitive | `int __thiscall(void *self, int a1, int a2, int a3, int a4, int a5)` | 19 | yes | |
| 0x103138BA | __CIfmod | `double __usercall@<st0>(double x@<st1>, double y@<st0>)` | 18 | no | |
| 0x10015680 | Camera_GetViewportWidth | `__int16 __cdecl __spoils<eax,edx>()` | 16 | yes | |
| 0x10015690 | Camera_GetViewportHeight | `__int16 __cdecl __spoils<eax,edx>()` | 16 | yes | |
| 0x1005E1F0 | sub_1005E1F0 | `int __thiscall __spoils<eax,edx>(_DWORD *this)` | 14 | no | |
| 0x1005E4C0 | sub_1005E4C0 | `int __thiscall(_DWORD *this, float *)` | 14 | no | |
| 0x1018BC40 | sub_1018BC40 | `unsigned int __thiscall __spoils<eax,edx>(int *this, int)` | 14 | yes | |
| 0x1000CB20 | CDx_DrawPrimitive | `int __thiscall(void *self, int a1, int a2, int a3)` | 13 | yes | |
| 0x1005E4A0 | sub_1005E4A0 | `int __thiscall(_DWORD *this)` | 13 | no | |
| 0x10009920 | CDx_SetDefaultRenderState | `int __thiscall(CDx *this)` | 11 | no | |
| 0x1000D660 | CEnv_IsStereoMode | `char __thiscall(_DWORD *this)` | 11 | yes | |
| 0x1005E110 | sub_1005E110 | `_DWORD *__thiscall __spoils<eax,edx>(_DWORD *this, int)` | 11 | yes | |
| 0x10009DC0 | CDx_SetViewport | `int __thiscall(CDx *this, const void *)` | 7 | yes | |
| 0x1000CF20 | CDx_ApplyStereoEyeProjection | `void __thiscall(CDx *this)` | 6 | yes | |
| 0x102E72EA | sub_102E72EA | `void __thiscall(void *this)` | 6 | no | |
| 0x102E7317 | ?Open@CResource@D3DXCore@@QAEJPAUHINSTANCE__@@PBXHH@Z | `int __thiscall(void *self, HMODULE hModule, LPCSTR lpName, int, int)` | 6 | no | |
| 0x100528A0 | sub_100528A0 | `int __thiscall __spoils<eax,ecx>(void *this)` | 5 | yes | |
| 0x100182E0 | _exit_0 | `void __cdecl __noreturn(int Code)` | 4 | yes | |
| 0x1005E4B0 | sub_1005E4B0 | `int __thiscall(_DWORD *this)` | 4 | no | |
| 0x102D2C16 | sub_102D2C16 | `int __cdecl(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8)` | 4 | no | |
| 0x10315AA8 | __strlwr | `char *__cdecl(char *String)` | 4 | no | |
| 0x1000BB10 | nullsub_36 | `void __cdecl __spoils<eax,ecx>()` | 3 | yes | |
| 0x1000CA00 | CDx_GetStereoSecondEyeTarget | `char __thiscall(void *self, int a1, int a2)` | 3 | yes | |
| 0x10057FB0 | CMoSchedularTask_Interpret | `int __thiscall(void *self)` | 3 | no | |
| 0x1017C9C0 | sub_1017C9C0 | `int __thiscall(CTsZoneMap *this, int, int, int)` | 3 | no | |
| 0x1017DE20 | sub_1017DE20 | `int __thiscall(float *this, int, int, int)` | 3 | no | |
| 0x102D538A | sub_102D538A | `int __cdecl(int, unsigned __int8 *, unsigned int, int, int, int, unsigned int, int, int, unsigned int, int, int, int, int *, void *, int, struct IDirect3DSurface8 **)` | 3 | no | |
| 0x10009870 | sub_10009870 | `char __thiscall(int this, int a2, _DWORD *a3)` | 2 | yes | |
| 0x1000BB20 | nullsub_37 | `void __cdecl()` | 2 | yes | |
| 0x1000C900 | CDx_CreateRampTextures1024x2 | `char __thiscall(CDx *this, IDirect3DTexture8 **, int)` | 2 | yes | |
| 0x1000D620 | nullsub_38 | `void __cdecl()` | 2 | yes | |
| 0x10046550 | sub_10046550 | `int __thiscall(void *self, int a1)` | 2 | no | |
| 0x100467E0 | sub_100467E0 | `int __thiscall(void *self, int a1)` | 2 | yes | |
| 0x10052AB0 | sub_10052AB0 | `void *__thiscall(void *this)` | 2 | yes | |
| 0x10052AD0 | sub_10052AD0 | `unsigned int __thiscall(void *this)` | 2 | yes | |
| 0x1005E1C0 | sub_1005E1C0 | `unsigned int __thiscall __spoils<eax,edx>(void *this)` | 2 | no | |
| 0x10085000 | sub_10085000 | `int __thiscall __spoils<eax,edx>(_DWORD *this, int)` | 2 | no | |
| 0x100CBDE0 | PerActor_FourBucketStaggeredUpdate | `void __thiscall(void *self)` | 2 | yes | |
| 0x100D7090 | CXiSkeletonActorRes_GetLinkedActor | `int __thiscall(CXiSkeletonActorRes *this)` | 2 | yes | |
| 0x10150CC0 | sub_10150CC0 | `char __thiscall(void *this, unsigned __int8 *a2, int a3)` | 2 | yes | |
| 0x1015DFC0 | nullsub_49 | `void __stdcall(int)` | 2 | yes | |
| 0x101606F0 | sub_101606F0 | `int __thiscall(_DWORD)` | 2 | yes | |
| 0x10281DB0 | sqgxTextureUpdate | `int __cdecl()` | 2 | yes | |
| 0x10283420 | sqgxColorUpdate | `int __cdecl()` | 2 | yes | |
| 0x102E0E52 | ?Blt@CD3DXBlt@@QAEJPAUD3DX_BLT@@0K@Z | `int __thiscall(void *self, int a1, int a2, int a3)` | 2 | no | |
| 0x103127C0 | _strrchr | `char *__cdecl(const char *Str, int Ch)` | 2 | no | |
| 0x10312C90 | __CIpow | `double __usercall@<st0>(double x@<st1>, double y@<st0>)` | 2 | no | |
| 0x10008F90 | CDx_Constructor | `CDx *__thiscall(CDx *this, IDirect3D8 *pD3D, IDirect3DDevice8 *pDevice)` | 1 | no | |
| 0x100091B0 | CDx_Initialize | `char __thiscall(CDx *this)` | 1 | yes | |
| 0x10009770 | SetCursorPos | `BOOL __stdcall(int X, int Y)` | 1 | no | |
| 0x10009F20 | CDx_PushRenderTargetTexture | `int __thiscall(CDx *this, int, int, int, __int16 *, __int16 *, int, int)` | 1 | yes | |
| 0x1000A3B0 | CDx_DrawScreenQuadAtDepth | `int __thiscall(void *self, int a1, int a2)` | 1 | yes | |
| 0x1000A820 | CDx_DrawFadeQuad | `int __thiscall(CDx *self, int a1, int a2)` | 1 | yes | |
| 0x1000AAB0 | Post_DepthBlurComposite | `int __thiscall(CDx *self, int a1, int a2)` | 1 | yes | |
| 0x1000ABE0 | sub_1000ABE0 | `int __thiscall(void *self, int a1, int a2)` | 1 | no | |
| 0x1000BB90 | CDx_DrawStereoOddFrameOverlay | `char __thiscall(void *self, int a1)` | 1 | yes | |
| 0x1000C0B0 | CDx_CompositeSceneToBackBuffer | `int __thiscall(void *self, int a1, int a2, int a3, int a4)` | 1 | yes | |
| 0x1000C7C0 | CDx_ReleaseStereoResources | `int __thiscall(CDx *this)` | 1 | yes | |
| 0x1000C850 | CDx_CreateRampTexture1x16 | `int __thiscall(void *self, int a1)` | 1 | yes | |
| 0x10012F40 | Post_Afterimage | `int __cdecl()` | 1 | yes | |
| 0x100132B0 | Post_FlashAndZoomFeedback | `int __thiscall(void *self)` | 1 | yes | |
| 0x10013AC0 | ?do_always_noconv@codecvt_base@std@@MBE_NXZ_2 | `bool __cdecl()` | 1 | yes | |
| 0x10015480 | Camera_BuildProjection | `void __cdecl()` | 1 | yes | |
| 0x10015510 | Camera_ApplyViewProjection | `void __cdecl()` | 1 | no | |
| 0x10015740 | ?do_always_noconv@codecvt_base@std@@MBE_NXZ_3 | `bool __cdecl()` | 1 | yes | |
| 0x10021280 | sub_10021280 | `int __cdecl(int a1, int a2)` | 1 | no | |
| 0x10021900 | Camera_UpdateController | `void __thiscall(int this)` | 1 | no | |
| 0x10033DA0 | sub_10033DA0 | `char __thiscall(int self, float *a1)` | 1 | yes | |
| 0x10039E50 | TexObj_Upload | `int __thiscall(void *self)` | 1 | yes | |
| 0x1003C8A0 | sub_1003C8A0 | `int __thiscall(void *self, int a1, int a2, int a3)` | 1 | no | |
| 0x1003D160 | sub_1003D160 | `long double __thiscall(CMoCameraTask *this, float)` | 1 | no | |
| 0x1003EBF0 | sub_1003EBF0 | `int __thiscall(void *self, int a1, int a2, int a3, int a4, int a5, int a6)` | 1 | no | |
| 0x10041110 | CXi471011C8_VT_1032AD80_Dtor | `char __thiscall(CMoD3mSpecularElem *this)` | 1 | no | |
| 0x10052A80 | ?Win95IsDebuggerPresent@@YGHXZ | `int __thiscall(void *self)` | 1 | yes | |
| 0x1005E180 | sub_1005E180 | `void *__thiscall(void *this)` | 1 | no | |
| 0x1005EEF0 | sub_1005EEF0 | `int __thiscall(CMoPathDriveActorTask *this, float)` | 1 | yes | |
| 0x10060940 | sub_10060940 | `double __thiscall __spoils<eax,edx>(float *this)` | 1 | no | |
| 0x1006C530 | sub_1006C530 | `int __stdcall(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9)` | 1 | yes | |
| 0x100A4ED0 | sub_100A4ED0 | `int __thiscall(void *self)` | 1 | no | |
| 0x100A5E10 | sub_100A5E10 | `int __cdecl(int a1)` | 1 | yes | |
| 0x100C5C00 | CXiSkeletonActor_DestructorBody | `char __thiscall(int a0)` | 1 | yes | |
| 0x100C8ED0 | sub_100C8ED0 | `int __thiscall(void *self, int a1)` | 1 | no | |
| 0x100C9330 | sub_100C9330 | `int __thiscall(void *self, int a1)` | 1 | no | |
| 0x100C9810 | sub_100C9810 | `int __thiscall(void *self, int a1)` | 1 | no | |
| 0x100C9C10 | sub_100C9C10 | `int __thiscall(void *self, int a1)` | 1 | no | |
| 0x100CA310 | sub_100CA310 | `int __thiscall(void *self, int a1)` | 1 | no | |
| 0x100CA730 | sub_100CA730 | `int __thiscall(void *self, int a1)` | 1 | no | |
| 0x100CAA60 | sub_100CAA60 | `int __thiscall(void *self)` | 1 | no | |
| 0x100CAF90 | sub_100CAF90 | `int __thiscall(void *self)` | 1 | no | |
| 0x100D7050 | CXiSkeletonActorRes_Destructor | `char __thiscall(_DWORD *this)` | 1 | yes | |
| 0x100FD3A0 | sub_100FD3A0 | `char __cdecl(unsigned int a1)` | 1 | yes | |
| 0x10118520 | nullsub_48 | `void __cdecl()` | 1 | yes | |
| 0x10118DB0 | sub_10118DB0 | `int __thiscall(void *self, int a1, int a2)` | 1 | yes | |
| 0x10125170 | sub_10125170 | `int __thiscall(void *this)` | 1 | yes | |
| 0x10126290 | unknown_libname_89 | `int __cdecl()` | 1 | yes | |
| 0x10152F80 | sub_10152F80 | `int __cdecl()` | 1 | yes | |
| 0x101579C0 | sub_101579C0 | `int __thiscall(void *self)` | 1 | yes | |
| 0x1015F6A0 | sub_1015F6A0 | `int __thiscall(void *self)` | 1 | yes | |
| 0x101772E0 | sub_101772E0 | `int __thiscall(const char **this, int a2)` | 1 | yes | |
| 0x1017C8B0 | sub_1017C8B0 | `int __thiscall(CTsZoneMap *this, int, int, int)` | 1 | no | |
| 0x1018B860 | sub_1018B860 | `int __thiscall(void *self)` | 1 | yes | |
| 0x10251500 | StDancer_Update | `int __thiscall(void *self)` | 1 | yes | |
| 0x1025E0C0 | sub_1025E0C0 | `void __cdecl()` | 1 | yes | |
| 0x1025E0E0 | sub_1025E0E0 | `int __cdecl()` | 1 | yes | |
| 0x1025E0F0 | sub_1025E0F0 | `char __cdecl()` | 1 | yes | |
| 0x10282A30 | sqgxBlendLin | `void __cdecl(float a1)` | 1 | yes | |
| 0x1028BBD0 | sub_1028BBD0 | `int __cdecl()` | 1 | no | |
| 0x102D4BF7 | _D3DXFilterTexture@16 | `int __stdcall(void *pBaseTexture, void *pPalette, unsigned int SrcLevel, unsigned int Filter)` | 1 | no | |
| 0x10312050 | _strncpy | `char *__cdecl(char *Destination, const char *Source, size_t Count)` | 1 | no | |
| 0x10312F96 | _malloc_0 | `void *__cdecl(size_t Size)` | 1 | no | |
| 0x10009100 | CDx_ScalarDeletingDtor | `void *__thiscall(CDx *C, char)` | 0 | yes | |
| 0x10011950 | CMoTaskMng_ScalarDeletingDtor | `_DWORD *__thiscall(CMoTaskMng *C, char)` | 0 | yes | |
| 0x10015100 | sub_10015100 | `float __cdecl(int)` | 0 | yes | |
| 0x10015490 | sub_10015490 | `float __thiscall(int this)` | 0 | yes | |
| 0x10017310 | sub_10017310 | `int __stdcall(int a1, int a2)` | 0 | yes | |
| 0x10017780 | CShader_ScalarDeletingDtor | `unsigned int *__thiscall(CShader *C, char)` | 0 | yes | |
| 0x10017810 | sub_10017810 | `int __thiscall(void *this, int a1)` | 0 | yes | |
| 0x100182C0 | _exit | `void __cdecl __noreturn(int Code)` | 0 | yes | |
| 0x10019A30 | CMoResource_VT_1032C7FC_vf004 | `int __thiscall(_DWORD **this, int)` | 0 | yes | |
| 0x10019B30 | SampleAnimationTrack_ToBuffer | `float __thiscall(char *this, int, int, int, float, _DWORD *)` | 0 | yes | |
| 0x10019EE0 | SampleAnimationTrack_Live | `float __thiscall(char *this, int, int, int, float, float, _DWORD *)` | 0 | yes | |
| 0x1001DBD0 | sub_1001DBD0 | `char *__thiscall(CMoResource_VT_1032C8AC *this)` | 0 | yes | |
| 0x1001DC20 | CMoResource_VT_1032C8AC_vf004 | `int __thiscall(_DWORD **this, int)` | 0 | yes | |
| 0x1001E050 | CMoResource_VT_1032C880_vf004 | `int __thiscall(_DWORD **this, int)` | 0 | yes | |
| 0x10021C10 | sub_10021C10 | `float __thiscall(int this)` | 0 | yes | |
| 0x10025030 | CYyModelDt_PrepareRenderStateEntry | `char __thiscall(int *this, int, int, int, int, int, int)` | 0 | yes | |
| 0x10025060 | CYyModelDt_PrepareRenderStateSimpleEntry | `unsigned __int16 __thiscall(int *this, int, int, int, int, int)` | 0 | yes | |
| 0x100250A0 | CYyModelDt_VT_1032A430_ScalarDeletingDtor | `int *__thiscall(CYyModelDt_VT_1032A430 *C, char)` | 0 | yes | |
| 0x10025180 | CYyModelDt_GetElementCount | `__int16 __thiscall(int *this)` | 0 | yes | |
| 0x100251A0 | CYyModelDt_MarkReferencedBones | `int __thiscall(int *this, int)` | 0 | yes | |
| 0x100252D0 | CYyModelDt_VT_1032A430_ScalarDeletingDtor_2 | `int __thiscall(Vt_1032A460 *this, int a1)` | 0 | yes | |
| 0x10026AD0 | CXiMovie_Draw | `int __thiscall(void *self, int a1, int a2, int a3, int a4, int a5)` | 0 | yes | |
| 0x10027620 | sub_10027620 | `float __cdecl(float *, float *)` | 0 | yes | |
| 0x10028730 | __toupper | `int __cdecl(int C)` | 0 | yes | |
| 0x10029CB0 | CYyModelDt_ScalarDeletingDtor | `_DWORD *__thiscall(CYyModelDt *C, char a1)` | 0 | yes | |
| 0x10029DB0 | CYyModelBase_ScalarDeletingDtor | `int *__thiscall(CYyModelBase *C, char a1)` | 0 | yes | |
| 0x1002A000 | sub_1002A000 | `__int16 __thiscall(CYyModelDt_VT_1032A72C *this, int, int, int, int, _DWORD *, _DWORD *)` | 0 | yes | |
| 0x1002A080 | sub_1002A080 | `int __thiscall(CYyModelDt_VT_1032A72C *this)` | 0 | yes | |
| 0x1002A0A0 | CYyModelDt_VT_1032A72C_ScalarDeletingDtor | `_DWORD *__thiscall(CYyModelDt_VT_1032A72C *C, char a1)` | 0 | yes | |
| 0x1002B530 | CYyModel_ScalarDeletingDtor | `int *__thiscall(CYyModel *C, char)` | 0 | yes | |
| 0x1002D660 | CYyOs2VtxBuffer_ScalarDeletingDtor | `void *__thiscall(CYyOs2VtxBuffer *C, char)` | 0 | yes | |
| 0x1002D860 | sub_1002D860 | `int __thiscall(CMoResource_VT_1032C854 *this)` | 0 | yes | |
| 0x1002D880 | CMoResource_VT_1032C854_vf004 | `int __thiscall(CMoResource_VT_1032C854 *this, int)` | 0 | yes | |
| 0x1002D8F0 | sub_1002D8F0 | `char __thiscall(CMoResource_VT_1032C854 *this)` | 0 | yes | |
| 0x10034170 | CMoResource_VT_1032C828_vf004 | `int __thiscall(_DWORD **this, int)` | 0 | yes | |
| 0x10035AE0 | CMoTask_VT_1032A904_ScalarDeletingDtor | `void *__thiscall(CMoTask_VT_1032A904 *C, char)` | 0 | yes | |
| 0x100398B0 | CYyTexBase_ScalarDeletingDtor | `_DWORD *__thiscall(CYyTexBase *C, char)` | 0 | yes | |
| 0x10039950 | CYyTex_ScalarDeletingDtor | `_DWORD *__thiscall(CYyTex *C, char)` | 0 | yes | |
| 0x1003AA70 | CYyTexMng_ScalarDeletingDtor | `_DWORD *__thiscall(CYyTexMng *C, char)` | 0 | yes | |
| 0x1003B5A0 | CMoAttachments_ScalarDeletingDtor | `_DWORD *__thiscall(CMoAttachments *this, char)` | 0 | yes | |
| 0x1003CB30 | CMoCamera_SetPathData | `int __thiscall(_DWORD *this, _DWORD *)` | 0 | yes | |
| 0x1003D010 | CMoCameraTask_ScalarDeletingDtor | `_DWORD *__thiscall(_DWORD *C, char a1)` | 0 | yes | |
| 0x1003D960 | CMoCelestial_ScalarDeletingDtor | `_DWORD *__thiscall(CMoCelestial *C, char)` | 0 | yes | |
| 0x1003E660 | sub_1003E660 | `float __thiscall(void *this, int a1)` | 0 | yes | |
| 0x1003EF90 | CMoVuAnmElem_vf008 | `int __thiscall(CMoVuAnmElem *this)` | 0 | yes | |
| 0x1003EFA0 | CMoVuAnmElem_ScalarDeletingDtor | `void *__thiscall(CMoVuAnmElem *C, char)` | 0 | yes | |
| 0x10040E70 | CXi471011C8_VT_1032B3B8_RebuildBlendWeights | `CMoProcessor *__thiscall(CXi471011C8_VT_1032B3B8 *this)` | 0 | yes | |
| 0x100410F0 | CMoD3mSpecularElem_ScalarDeletingDtor | `_DWORD *__thiscall(CMoD3mSpecularElem *C, char)` | 0 | yes | |
| 0x10041160 | sub_10041160 | `char __thiscall(CMoD3mSpecularElem *this)` | 0 | yes | |
| 0x10041FE0 | CMoElem_VT_1032ADE8_ScalarDeletingDtor | `int *__thiscall(CMoElem_VT_1032ADE8 *C, char)` | 0 | yes | |
| 0x10042690 | CMoAttachments_VT_1032AE94_vf001 | `_DWORD *__thiscall(CMoAttachments_VT_1032AE94 *this, char)` | 0 | yes | |
| 0x100431B0 | sub_100431B0 | `int __thiscall(void *this)` | 0 | yes | |
| 0x10043590 | CMoDistMorphElem_UpdateMorphBlend | `char __thiscall(CMoDistMorphElem *this)` | 0 | yes | |
| 0x10043720 | CMoDistRingElem_AllocRingGeometry | `void __thiscall(CMoDistRingElem *this)` | 0 | yes | |
| 0x10043DB0 | CMoAttachments_VT_1032AF44_vf001 | `void *__thiscall(CMoAttachments_VT_1032AF44 *this, char)` | 0 | yes | |
| 0x10043E20 | CMoTask_VT_1032AFA8_ScalarDeletingDtor_2 | `int __thiscall(Vt_1032AFD4 *this, int a1)` | 0 | yes | |
| 0x10044950 | CMoElem_ScalarDeletingDtor | `int *__thiscall(CMoElem *C, char)` | 0 | yes | |
| 0x10047940 | CMoSkeletonElem_ScalarDeletingDtor | `int __thiscall(CMoSkeletonElem *C, char a1)` | 0 | yes | |
| 0x10052C50 | CMoD3aLightmapElem_set_field_194 | `int __thiscall(CMoD3aLightmapElem *this, int)` | 0 | yes | |
| 0x10052C60 | CMoD3aLightmapElem_get_field_194 | `int __thiscall(CMoD3aLightmapElem *this)` | 0 | yes | |
| 0x10052C70 | CMoD3aLightmapElem_ScalarDeletingDtor | `int *__thiscall(CMoD3aLightmapElem *C, char)` | 0 | yes | |
| 0x10052CB0 | CMoD3aElem_ScalarDeletingDtor | `int *__thiscall(CMoD3aElem *C, char)` | 0 | yes | |
| 0x10052D80 | CMoDist_ScalarDeletingDtor | `int *__thiscall(CMoDist *C, char)` | 0 | yes | |
| 0x10052DE0 | CMoVuDist_vf008 | `int __thiscall(CMoVuDist *this)` | 0 | yes | |
| 0x10052DF0 | CMoVuDist_ScalarDeletingDtor | `int *__thiscall(CMoVuDist *C, char)` | 0 | yes | |
| 0x10052E50 | CMoDistMorphElem_set_field_1C0 | `int __thiscall(CMoDistMorphElem *this, int)` | 0 | yes | |
| 0x10052E60 | CMoDistMorphElem_ScalarDeletingDtor | `int *__thiscall(CMoDistMorphElem *C, char)` | 0 | yes | |
| 0x10052F50 | CMoDistModelElem_set_field_194 | `int __thiscall(CMoDistModelElem *this, int)` | 0 | yes | |
| 0x10052F60 | CMoDistModelElem_ScalarDeletingDtor | `int *__thiscall(CMoDistModelElem *C, char)` | 0 | yes | |
| 0x10053000 | CMoDistRingElem_set_field_1B6 | `int __thiscall(CMoDistRingElem *this, int)` | 0 | yes | |
| 0x10053140 | CMoDisintegrateProgElem_vf008 | `int __thiscall(CMoDisintegrateProgElem *this)` | 0 | yes | |
| 0x10053150 | CMoDisintegrateProgElem_ScalarDeletingDtor | `int *__thiscall(CMoDisintegrateProgElem *C, char)` | 0 | yes | |
| 0x100531A0 | CMoNullProgElem_SetField138 | `void __thiscall(CMoNullProgElem *this)` | 0 | yes | |
| 0x100531B0 | CMoNullProgElem_Destructor | `int *__thiscall(CMoNullProgElem *C, char)` | 0 | yes | |
| 0x10053350 | CMoTask_VT_1032B654_ScalarDeletingDtor | `_DWORD *__thiscall(CMoTask_VT_1032B654 *C, char)` | 0 | yes | |
| 0x10054220 | sub_10054220 | `char __thiscall(CMoTask_VT_1032B654 *this)` | 0 | yes | |
| 0x10054450 | CMoAttachments_VT_1032B140_vf001 | `void *__thiscall(CMoAttachments_VT_1032B140 *this, char)` | 0 | yes | |
| 0x10054460 | CMoTask_VT_1032B654_vf011 | `int __thiscall(Vt_1032B680 *this)` | 0 | yes | |
| 0x10054470 | CMoTask_VT_1032B654_vf012 | `int __thiscall(Vt_1032B680 *this, int a1)` | 0 | yes | |
| 0x100552C0 | CMoOT_ScalarDeletingDtor | `int *__thiscall(CMoOT *C, char)` | 0 | yes | |
| 0x100552F0 | sub_100552F0 | `void __thiscall(CMoOT *this)` | 0 | yes | |
| 0x100562B0 | CMoSphRes_vf008 | `char *__thiscall(CMoSphRes *this)` | 0 | yes | |
| 0x10056370 | CMoPointLightProgElem_ScalarDeletingDtor | `int *__thiscall(CMoPointLightProgElem *C, char)` | 0 | yes | |
| 0x10056400 | sub_10056400 | `int __thiscall(CMoPointLightProgElem *this)` | 0 | yes | |
| 0x100565A0 | CXi471011C8_VT_1032B478_RebuildBlendWeights | `char __thiscall(CXi471011C8_VT_1032B478 *this)` | 0 | yes | |
| 0x10062F50 | CMoAttachments_VT_1032BCCC_vf001 | `_DWORD *__thiscall(CMoAttachments_VT_1032BCCC *this, char)` | 0 | yes | |
| 0x10062F60 | CMoAttachments_VT_1032BAF0_vf001 | `_DWORD *__thiscall(CMoAttachments_VT_1032BAF0 *this, char)` | 0 | yes | |
| 0x10062F70 | CMoAttachments_VT_1032BBAC_vf001 | `_DWORD *__thiscall(CMoAttachments_VT_1032BBAC *this, char)` | 0 | yes | |
| 0x10062F80 | CMoLockLookAtDriveTask_vf011 | `int __thiscall(Vt_1032BA60 *this)` | 0 | yes | |
| 0x10062F90 | CMoLockLookAtDriveTask_vf012 | `int __thiscall(Vt_1032BA60 *this, int a1)` | 0 | yes | |
| 0x10062FA0 | CMoAttachments_VT_1032BF0C_vf001 | `_DWORD *__thiscall(CMoAttachments_VT_1032BF0C *this, char)` | 0 | yes | |
| 0x10062FB0 | CMoAttachments_VT_1032BD14_vf001 | `_DWORD *__thiscall(CMoAttachments_VT_1032BD14 *this, char)` | 0 | yes | |
| 0x10062FC0 | CMoAttachments_VT_1032BF54_vf001 | `_DWORD *__thiscall(CMoAttachments_VT_1032BF54 *this, char)` | 0 | yes | |
| 0x10062FD0 | CMoAttachments_VT_1032BBF4_vf001 | `_DWORD *__thiscall(CMoAttachments_VT_1032BBF4 *this, char)` | 0 | yes | |
| 0x10062FE0 | CMoActorColorDriveTask_vf011 | `int __thiscall(Vt_1032BAA8 *this)` | 0 | yes | |
| 0x10062FF0 | CMoActorColorDriveTask_vf012 | `tagRECT *__thiscall(Vt_1032BAA8 *this, char)` | 0 | yes | |
| 0x10063000 | CMoAttachments_VT_1032BD5C_vf001 | `_DWORD *__thiscall(CMoAttachments_VT_1032BD5C *this, char)` | 0 | yes | |
| 0x10063020 | CMoAttachments_VT_1032BC68_vf001 | `_DWORD *__thiscall(CMoAttachments_VT_1032BC68 *this, char)` | 0 | yes | |
| 0x10063030 | CMoAttachments_VT_1032BDA4_vf001 | `_DWORD *__thiscall(CMoAttachments_VT_1032BDA4 *this, char)` | 0 | yes | |
| 0x10063040 | CMoAttachments_VT_1032BFE4_vf001 | `_DWORD *__thiscall(CMoAttachments_VT_1032BFE4 *this, char)` | 0 | yes | |
| 0x10063070 | CMoAttachments_VT_1032BB38_vf001 | `_DWORD *__thiscall(CMoAttachments_VT_1032BB38 *this, char)` | 0 | yes | |
| 0x10063080 | CMoAttachments_VT_1032BDEC_vf001 | `_DWORD *__thiscall(CMoAttachments_VT_1032BDEC *this, char)` | 0 | yes | |
| 0x10063090 | CMoAttachments_VT_1032C02C_vf001 | `_DWORD *__thiscall(CMoAttachments_VT_1032C02C *this, char)` | 0 | yes | |
| 0x100630A0 | CMoPathDriveActorTask_vf011 | `int __thiscall(Vt_1032B9D0 *this)` | 0 | yes | |
| 0x100630B0 | CMoPathDriveActorTask_vf012 | `int __thiscall(Vt_1032B9D0 *this, int a1)` | 0 | yes | |
| 0x100630C0 | CMoAttachments_VT_1032BE34_vf001 | `_DWORD *__thiscall(CMoAttachments_VT_1032BE34 *this, char)` | 0 | yes | |
| 0x100630D0 | CMoAttachments_VT_1032BE7C_vf001 | `_DWORD *__thiscall(CMoAttachments_VT_1032BE7C *this, char)` | 0 | yes | |
| 0x100630E0 | CMoAttachments_VT_1032B940_vf001 | `_DWORD *__thiscall(CMoAttachments_VT_1032B940 *this, char)` | 0 | yes | |
| 0x100630F0 | CMoLockColorDriveTask_vf011 | `int __thiscall(Vt_1032BA18 *this)` | 0 | yes | |
| 0x10063100 | CMoLockColorDriveTask_vf012 | `int __thiscall(Vt_1032BA18 *this, int a1)` | 0 | yes | |
| 0x10063110 | CMoAttachments_VT_1032BC84_vf001 | `_DWORD *__thiscall(CMoAttachments_VT_1032BC84 *this, char)` | 0 | yes | |
| 0x10063120 | CMoAttachments_VT_1032BEC4_vf001 | `_DWORD *__thiscall(CMoAttachments_VT_1032BEC4 *this, char)` | 0 | yes | |
| 0x10063130 | CMoTask_VT_1032B95C_vf011 | `int __thiscall(Vt_1032B988 *this)` | 0 | yes | |
| 0x10063140 | CMoTask_VT_1032B95C_vf012 | `int __thiscall(CMoTask_VT_1032B95C *this, int a1)` | 0 | yes | |
| 0x10063480 | CMoSpline_ScalarDeletingDtor | `int *__thiscall(CMoSpline *C, char)` | 0 | yes | |
| 0x10063C50 | CMoResource_VT_1032C55C_vf008 | `void __thiscall(CMoResource_VT_1032C55C *this)` | 0 | yes | |
| 0x10069DB0 | CMoDX_ScalarDeletingDtor | `_BYTE *__thiscall(CMoDX *C, char)` | 0 | yes | |
| 0x1006B070 | CMoDxStatusMng_ScalarDeletingDtor | `int *__thiscall(CMoDxStatusMng *C, char)` | 0 | yes | |
| 0x1006C1C0 | CMoOcclusionMng_ScalarDeletingDtor | `int *__thiscall(CMoOcclusionMng *C, char)` | 0 | yes | |
| 0x1006F0A0 | Mat4x4_SafeTranspose | `__m128 *__thiscall(CMoProcessor_VT_1032C298 *this, __m128 *, int, unsigned int *)` | 0 | yes | |
| 0x1006FAE0 | sub_1006FAE0 | `int __thiscall(CMoProcessor_VT_1032C298 *this, int, float, float, float)` | 0 | yes | |
| 0x1006FC40 | sub_1006FC40 | `int __thiscall(CMoProcessor_VT_1032C298 *this, int, float, float, float)` | 0 | yes | |
| 0x10070E40 | CMoProcessor_VT_1032C1C8_vf035 | `CMoProcessor *__thiscall(CMoProcessor_VT_1032C1C8 *this, float *, float *, int, int, int, float, float)` | 0 | yes | |
| 0x10070F60 | CMoTask_VT_1032C490_ScalarDeletingDtor | `_DWORD *__thiscall(CMoTask_VT_1032C490 *C, char)` | 0 | yes | |
| 0x10071A40 | CMoResource_VT_1032C8AC_ScalarDeletingDtor | `unsigned int **__thiscall(CMoResource_VT_1032C8AC *C, char)` | 0 | yes | |
| 0x10071A60 | CMoResource_VT_1032C880_ScalarDeletingDtor | `unsigned int **__thiscall(CMoResource_VT_1032C880 *C, char)` | 0 | yes | |
| 0x10071A80 | CMoResource_VT_1032C854_ScalarDeletingDtor | `unsigned int **__thiscall(CMoResource_VT_1032C854 *C, char)` | 0 | yes | |
| 0x10071AA0 | CMoResource_VT_1032C828_ScalarDeletingDtor | `unsigned int **__thiscall(CMoResource_VT_1032C828 *C, char)` | 0 | yes | |
| 0x10071AC0 | CMoResource_VT_1032C7FC_ScalarDeletingDtor | `unsigned int **__thiscall(CMoResource_VT_1032C7FC *C, char)` | 0 | yes | |
| 0x10071AE0 | CMoResource_VT_1032C7D0_ScalarDeletingDtor | `unsigned int **__thiscall(CMoResource_VT_1032C7D0 *C, char)` | 0 | yes | |
| 0x10071B00 | CMoResource_VT_1032C7A4_ScalarDeletingDtor | `unsigned int **__thiscall(CMoResource_VT_1032C7A4 *C, char)` | 0 | yes | |
| 0x10071B50 | CMoResource_VT_1032C74C_ScalarDeletingDtor | `unsigned int **__thiscall(CMoResource_VT_1032C74C *C, char)` | 0 | yes | |
| 0x10071B70 | CMoResource_VT_1032C720_ScalarDeletingDtor | `unsigned int **__thiscall(CMoResource_VT_1032C720 *C, char)` | 0 | yes | |
| 0x10071B90 | CMoResource_VT_1032C6F4_ScalarDeletingDtor | `unsigned int **__thiscall(CMoResource_VT_1032C6F4 *C, char)` | 0 | yes | |
| 0x10071BB0 | CMoResource_VT_1032C6C8_ScalarDeletingDtor | `unsigned int **__thiscall(CMoResource_VT_1032C6C8 *C, char)` | 0 | yes | |
| 0x10071BD0 | CMoResource_VT_1032C69C_ScalarDeletingDtor | `unsigned int **__thiscall(CMoResource_VT_1032C69C *C, char)` | 0 | yes | |
| 0x10071BF0 | CMoResource_VT_1032C670_ScalarDeletingDtor | `unsigned int **__thiscall(CMoResource_VT_1032C670 *C, char)` | 0 | yes | |
| 0x10071C40 | CMoResource_VT_1032C5FC_ScalarDeletingDtor | `unsigned int **__thiscall(CMoResource_VT_1032C5FC *C, char)` | 0 | yes | |
| 0x10071C90 | CMoResource_VT_1032C588_ScalarDeletingDtor | `unsigned int **__thiscall(CMoResource_VT_1032C588 *C, char)` | 0 | yes | |
| 0x10071D00 | CMoSphRes_ScalarDeletingDtor | `unsigned int **__thiscall(CMoSphRes *C, char)` | 0 | yes | |
| 0x10071DC0 | CMoResource_ScalarDeletingDtor | `unsigned int **__thiscall(CMoResource *C, char)` | 0 | yes | |
| 0x10072A30 | CMoTask_VT_1032C908_ScalarDeletingDtor | `void *__thiscall(CMoTask_VT_1032C908 *C, char)` | 0 | yes | |
| 0x10072A70 | sub_10072A70 | `char __thiscall(CMoTask_VT_1032C908 *this)` | 0 | yes | |
| 0x10073730 | CMoResource_VT_1032C934_ScalarDeletingDtor | `unsigned int **__thiscall(CMoResource_VT_1032C934 *C, char)` | 0 | yes | |
| 0x100745D0 | CMoAttachments_VT_1032C4E8_vf001 | `_DWORD *__thiscall(CMoAttachments_VT_1032C4E8 *this, char)` | 0 | yes | |
| 0x100745E0 | CMoResource_VT_1032C588_vf011 | `int __thiscall(Vt_1032C5B4 *this)` | 0 | yes | |
| 0x100745F0 | CMoResource_VT_1032C588_vf012 | `int __thiscall(Vt_1032C5B4 *this, int a1)` | 0 | yes | |
| 0x10074600 | CMoAttachments_VT_1032C628_vf001 | `unsigned int **__thiscall(CMoAttachments_VT_1032C628 *this, char)` | 0 | yes | |
| 0x10074620 | CMoResource_VT_1032C588_vf009 | `__int16 __thiscall(CMoResource_VT_1032C588 *this)` | 0 | yes | |
| 0x10074770 | CMoTask_ScalarDeletingDtor | `_DWORD *__thiscall(CMoTask *C, char)` | 0 | yes | |
| 0x100817F0 | CXiActorNameDraw_SetRenderState | `int __cdecl()` | 0 | yes | |
| 0x10082080 | CXiSkeletonActor_SetMoveSpeed | `void __thiscall(CXiSkeletonActor *this, float speed)` | 0 | yes | |
| 0x10082090 | CXiSkeletonActor_GetMoveSpeedBase | `float __thiscall(CXiSkeletonActor *this)` | 0 | yes | |
| 0x100820A0 | CXiSkeletonActor_SetMoveSpeedBase | `void __thiscall(CXiSkeletonActor *this, float speed)` | 0 | yes | |
| 0x100820C0 | CXiSkeletonActor_SetWalkSpeed | `void __thiscall(CXiSkeletonActor *this, float speed)` | 0 | yes | |
| 0x1008A2F0 | sub_1008A2F0 | `int __thiscall(void *this)` | 0 | yes | |
| 0x100A27F0 | CMoTask_VT_1032D6D4_ScalarDeletingDtor | `_DWORD *__thiscall(CMoTask_VT_1032D6D4 *C, char)` | 0 | yes | |
| 0x100A4830 | CXiSkeletonActor_SetColor | `void __thiscall(CXiSkeletonActor *this, unsigned int color)` | 0 | yes | |
| 0x100A4840 | CXiSkeletonActor_GetColor | `unsigned int __thiscall(CXiSkeletonActor *this)` | 0 | yes | |
| 0x100A4860 | CXiSkeletonActor_SetMultiTexAlpha | `void __thiscall(CXiSkeletonActor *self, int alpha)` | 0 | yes | |
| 0x100A4870 | CXiSkeletonActor_GetMultiTexAlpha | `char __thiscall(CXiSkeletonActor *this)` | 0 | yes | |
| 0x100A4880 | CXiSkeletonActor_SetSahdowAlpha | `void __thiscall(CXiSkeletonActor *self, int alpha)` | 0 | yes | |
| 0x100A4890 | CXiSkeletonActor_GetShadowAlpha | `char __thiscall(CXiSkeletonActor *this)` | 0 | yes | |
| 0x100A48A0 | CXiSkeletonActor_EnableEffect | `void __thiscall(CXiSkeletonActor *this, unsigned int flagBit)` | 0 | yes | |
| 0x100A48C0 | CXiSkeletonActor_DisableEffect | `void __thiscall(CXiSkeletonActor *this, unsigned int flagBit)` | 0 | yes | |
| 0x100A4910 | CXiSkeletonActor_SetDistAmount | `void __thiscall(CXiSkeletonActor *this, float amount)` | 0 | yes | |
| 0x100A4920 | CXiSkeletonActor_SetEmapColor | `void __thiscall(CXiSkeletonActor *this, unsigned int color)` | 0 | yes | |
| 0x100A4930 | CXiSkeletonActor_SetEmapTexture | `void __thiscall(CXiSkeletonActor *this, void *texture)` | 0 | yes | |
| 0x100A4940 | CXiSkeletonActor_SetSeparateValue | `void __thiscall(CXiSkeletonActor *this, float value)` | 0 | yes | |
| 0x100A4950 | CXiSkeletonActor_SetEmapAlphaMode | `void __thiscall(CXiSkeletonActor *this, int mode)` | 0 | yes | |
| 0x100A4960 | CXiSkeletonActor_GetEmapAlphaMode | `int __thiscall(CXiSkeletonActor *this)` | 0 | yes | |
| 0x100AB060 | CMoAttachments_VT_1032E7EC_vf001 | `_DWORD *__thiscall(CMoAttachments_VT_1032E7EC *this, char)` | 0 | yes | |
| 0x100C5540 | CXiSkeletonActor_ScalarDeletingDtor | `int __thiscall(CXiSkeletonActor *C, char a1)` | 0 | yes | |
| 0x100D7000 | CXiSkeletonActorRes_ScalarDeletingDtor | `_DWORD *__thiscall(CXiSkeletonActorRes *C, char)` | 0 | yes | |
| 0x100D70A0 | CXiSkeletonActorRes_ProcessLoadState | `char __thiscall(CXiSkeletonActorRes *this)` | 0 | yes | |
| 0x100D9950 | sub_100D9950 | `int __thiscall(void *this)` | 0 | yes | |
| 0x100D9A70 | sub_100D9A70 | `int __thiscall(void *this)` | 0 | yes | |
| 0x100D9AD0 | sub_100D9AD0 | `int __cdecl()` | 0 | yes | |
| 0x100D9B00 | sub_100D9B00 | `int __cdecl()` | 0 | yes | |
| 0x100DBE00 | timeGetTime | `DWORD __stdcall()` | 0 | yes | |
| 0x100DD920 | j__rand | `int __cdecl()` | 0 | yes | |
| 0x100E2EE0 | _EVP_EncryptInit_ex | `int __cdecl(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *cipher, ENGINE *impl, const unsigned __int8 *key, const unsigned __int8 *iv)` | 0 | yes | |
| 0x101085E0 | _malloc | `void *__cdecl(size_t Size)` | 0 | yes | |
| 0x1011D000 | __mkgmtime | `__time32_t __cdecl(struct tm *const Tm)` | 0 | yes | |
| 0x1011D010 | j___mkgmtime | `__time32_t __cdecl(struct tm *const Tm)` | 0 | yes | |
| 0x1011E980 | sub_1011E980 | `int __thiscall(void *self, int a1, int a2, int a3, int a4, int a5, int a6)` | 0 | yes | |
| 0x10122E20 | sub_10122E20 | `float()` | 0 | yes | |
| 0x10124640 | sub_10124640 | `int __thiscall(int *this, int *a2, __int16 a3)` | 0 | yes | |
| 0x1013A030 | sub_1013A030 | `float __thiscall(_DWORD *this)` | 0 | yes | |
| 0x10152FA0 | sub_10152FA0 | `int __cdecl()` | 0 | yes | |
| 0x10152FC0 | sub_10152FC0 | `int __cdecl(int a1)` | 0 | yes | |
| 0x10152FF0 | sub_10152FF0 | `int __cdecl(int a1, int a2)` | 0 | yes | |
| 0x10153090 | sub_10153090 | `int __cdecl(int a1, int a2)` | 0 | yes | |
| 0x101530C0 | sub_101530C0 | `int __cdecl(int a1, int a2)` | 0 | yes | |
| 0x10159970 | sub_10159970 | `float __thiscall(void *this, int)` | 0 | yes | |
| 0x10159A20 | sub_10159A20 | `float __thiscall(int this, float *)` | 0 | yes | |
| 0x10159B10 | sub_10159B10 | `float __stdcall(float *)` | 0 | yes | |
| 0x1015AF60 | sub_1015AF60 | `int __thiscall(void *this)` | 0 | yes | |
| 0x1015AF80 | sub_1015AF80 | `int __thiscall(void *this)` | 0 | yes | |
| 0x10176EA0 | CMoResource_VT_10336698_ScalarDeletingDtor | `unsigned int **__thiscall(CMoResource_VT_10336698 *C, char)` | 0 | yes | |
| 0x10176F80 | CMoResource_VT_103366C4_ScalarDeletingDtor | `unsigned int **__thiscall(CMoResource_VT_103366C4 *C, char)` | 0 | yes | |
| 0x10177000 | CMoResource_VT_103366F0_ScalarDeletingDtor | `unsigned int **__thiscall(CMoResource_VT_103366F0 *C, char)` | 0 | yes | |
| 0x101771B0 | sub_101771B0 | `int __stdcall(int a1, int a2)` | 0 | yes | |
| 0x10181A10 | sub_10181A10 | `float __thiscall(int this, float *, float *, float *)` | 0 | yes | |
| 0x10184C90 | sub_10184C90 | `int __thiscall(void *self, int a1, int a2, int a3)` | 0 | yes | |
| 0x10185CA0 | sub_10185CA0 | `int __thiscall(void *this, float *a1, float *a2, IDirect3DBaseTexture8 *a3, float a4)` | 0 | yes | |
| 0x10186F30 | sub_10186F30 | `const void *__thiscall(void *this)` | 0 | yes | |
| 0x101893D0 | CMoResource_VT_103367E8_ScalarDeletingDtor | `unsigned int **__thiscall(CMoResource_VT_103367E8 *C, char)` | 0 | yes | |
| 0x1018C270 | sub_1018C270 | `float __thiscall(_DWORD *this)` | 0 | yes | |
| 0x1018C290 | sub_1018C290 | `float __thiscall(_DWORD *this)` | 0 | yes | |
| 0x1019D150 | sub_1019D150 | `int __thiscall(void *this)` | 0 | yes | |
| 0x1019D1C0 | sub_1019D1C0 | `int __thiscall(void *this)` | 0 | yes | |
| 0x101ACF40 | _ZN5CRGBAC2Ehhhh | `int __thiscall(CRGBA *this, unsigned __int8 r, unsigned __int8 g, unsigned __int8 b, unsigned __int8 a)` | 0 | yes | |
| 0x101CAF00 | sub_101CAF00 | `float __cdecl(__int16, __int16, int, int, unsigned __int8, unsigned __int8 *, float, float, float, float)` | 0 | yes | |
| 0x101CB000 | sub_101CB000 | `float __cdecl(__int16, __int16, int, int, unsigned __int8, unsigned __int8 *)` | 0 | yes | |
| 0x101CB090 | sub_101CB090 | `float __cdecl(__int16, __int16, unsigned __int8, unsigned __int8 *)` | 0 | yes | |
| 0x101CB1B0 | sub_101CB1B0 | `float __cdecl(__int16, __int16, unsigned __int8, unsigned __int8 *)` | 0 | yes | |
| 0x101CB230 | sub_101CB230 | `float __cdecl(__int16, __int16, unsigned __int8, unsigned __int8 *, float, float, float, float)` | 0 | yes | |
| 0x101CB430 | sub_101CB430 | `float __cdecl(__int16, __int16, int, int, int, int, unsigned __int8, unsigned __int8 *, float, float, float, float)` | 0 | yes | |
| 0x101CB500 | sub_101CB500 | `float __cdecl(__int16, __int16, int, int, int, int, unsigned __int8, unsigned __int8 *)` | 0 | yes | |
| 0x101CB550 | sub_101CB550 | `float __cdecl(__int16, __int16, unsigned __int8, unsigned __int8 *)` | 0 | yes | |
| 0x102014F0 | sub_102014F0 | `float __thiscall(int *this, int)` | 0 | yes | |
| 0x10205500 | sub_10205500 | `int __thiscall(void *this)` | 0 | yes | |
| 0x10206420 | sub_10206420 | `int __thiscall(void *this)` | 0 | yes | |
| 0x10207730 | sub_10207730 | `int __thiscall(void *this)` | 0 | yes | |
| 0x10208500 | sub_10208500 | `int __thiscall(void *this)` | 0 | yes | |
| 0x10209300 | sub_10209300 | `int __thiscall(void *this)` | 0 | yes | |
| 0x10209B90 | sub_10209B90 | `int __thiscall(void *this)` | 0 | yes | |
| 0x1020B500 | sub_1020B500 | `int __thiscall(void *this)` | 0 | yes | |
| 0x1020BA40 | sub_1020BA40 | `int __thiscall(void *this, int a1)` | 0 | yes | |
| 0x1020BE90 | sub_1020BE90 | `int __thiscall(void *this)` | 0 | yes | |
| 0x1021EDC0 | ??__F?m_badAllocExceptionPtr@__ExceptionPtr@@0V?$shared_ptr@V__ExceptionPtr@@@tr1@std@@A@@YAXXZ_8 | `int __cdecl()` | 0 | yes | |
| 0x1021EDD0 | sub_1021EDD0 | `int __cdecl()` | 0 | yes | |
| 0x1021EDE0 | sub_1021EDE0 | `int __cdecl()` | 0 | yes | |
| 0x1021EE00 | sub_1021EE00 | `int __cdecl(int a1)` | 0 | yes | |
| 0x1021EE20 | sub_1021EE20 | `int __cdecl(int a1)` | 0 | yes | |
| 0x1021EE70 | sub_1021EE70 | `int __cdecl(int a1, int a2, int a3, int a4, int a5)` | 0 | yes | |
| 0x1021EEB0 | sub_1021EEB0 | `int __thiscall(void *this)` | 0 | yes | |
| 0x1021EF10 | sub_1021EF10 | `int __thiscall(void *this)` | 0 | yes | |
| 0x1021F950 | sub_1021F950 | `float __thiscall(unsigned __int8 *this, int)` | 0 | yes | |
| 0x1021FB80 | sub_1021FB80 | `float __stdcall(__int16, __int16, char *, char *, unsigned __int8, unsigned __int8 *)` | 0 | yes | |
| 0x10220350 | sub_10220350 | `float __thiscall(char *this)` | 0 | yes | |
| 0x102214D0 | sub_102214D0 | `int __thiscall(int *this)` | 0 | yes | |
| 0x1022B080 | sub_1022B080 | `int __cdecl()` | 0 | yes | |
| 0x1022B260 | sub_1022B260 | `int __cdecl()` | 0 | yes | |
| 0x1022BF00 | _Ctl3dAutoSubclass@4_8 | `BOOL __stdcall(HINSTANCE)` | 0 | yes | |
| 0x10257950 | _Ctl3dAutoSubclass@4_9 | `BOOL __stdcall(HINSTANCE)` | 0 | yes | |
| 0x1025E020 | sub_1025E020 | `char __cdecl()` | 0 | yes | |
| 0x1025E230 | sub_1025E230 | `void __cdecl()` | 0 | yes | |
| 0x1025E240 | sub_1025E240 | `int __cdecl()` | 0 | yes | |
| 0x1028AB50 | sub_1028AB50 | `int __cdecl()` | 0 | yes | |
