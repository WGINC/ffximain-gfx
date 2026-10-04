// gfx_free.hpp -- graphics functions with no established class.
#pragma once
#include "gfx_common.hpp"
XI_API char __cdecl Gfx_CopySurfaceRectToTexture(struct IDirect3DSurface8 *a1, const struct tagRECT *a2, unsigned int *a3, int a4);  // 0x10002040
XI_API char __cdecl Gfx_ProbeDeviceCaps(int a1, unsigned int *a2, int *a3);  // 0x10002130
XI_API unsigned __int8 __cdecl Gfx_IsGammaSupported(void);  // 0x10002BE0
XI_API CDx * __cdecl Gfx_SetGammaRGB(int a1, int a2, int a3);  // 0x10002C00
XI_API CDx * __cdecl Gfx_SetGammaR(int a1);  // 0x10002C30
XI_API CDx * __cdecl Gfx_SetGammaG(int a1);  // 0x10002C80
XI_API CDx * __cdecl Gfx_SetGammaB(int a1);  // 0x10002CD0
XI_API int __cdecl Gfx_GetGammaRGB(int a1, int a2, int a3);  // 0x10002D20
XI_API char __cdecl Gfx_CreateD3DHolder(int a1, int a2, int a3, char a4, char a5);  // 0x10002F20
XI_API int __cdecl Gfx_DestroyD3DHolder(void);  // 0x10003040
XI_API char __thiscall Gfx_D3DBootstrap(int self_, int a2, int a3, int a4, char a5, char a6, int a7);  // 0x100031F0
XI_API char __thiscall Gfx_TryCreateDeviceForMode(_DWORD **self_, int a2, int a3, int a4, char a5, char a6, int a7, int a8);  // 0x100033C0
XI_API char __thiscall Gfx_CreateDevice(_DWORD *self_, int a2, int a3, int a4, int a5, int a6, char a7, char a8, int a9);  // 0x100034B0
XI_API int __thiscall Debug_DrawTextTable(_DWORD *self_);  // 0x10003B60
XI_API void __stdcall Debug_DrawWorldText(_DWORD *a1, const char *a2, int arg8);  // 0x10003D70
XI_API char __thiscall DirectInput_InstallMouse(_DWORD **self_);  // 0x10007EF0
XI_API char __thiscall App_InitScene(int self_, int a4);  // 0x10010D20
XI_API int __thiscall Frame_Render(void *self_);  // 0x10011A00
XI_API int __cdecl Post_Afterimage(void);  // 0x10012F40
XI_API int __thiscall Post_FlashAndZoomFeedback(void *self_);  // 0x100132B0
XI_API char __thiscall App_StepGameState5Connect(int self_, int a2);  // 0x10013AD0
XI_API char __thiscall App_StepGameState6(_WORD *self_);  // 0x10013DF0
XI_API char __thiscall App_StepGameState7(_WORD *self_);  // 0x10013F10
XI_API char __thiscall App_StepGameState8(_WORD *self_);  // 0x10014030
XI_API bool __cdecl App_StepGameState9(void);  // 0x10014120
XI_API char __thiscall fn_100141C0(_WORD *self_);  // 0x100141C0
XI_API void __cdecl Camera_ApplyViewProjection(void);  // 0x10015510
XI_API int __cdecl App_WinMain(int a1, int a2);  // 0x100157E0
XI_API char __cdecl App_CreateRendererAndApp(void);  // 0x100161B0
XI_API void __thiscall GammaRamp_ApplyIfChanged(int self_);  // 0x10017230
XI_API int __stdcall fn_10017310(int a1, int a2);  // 0x10017310
XI_API int __thiscall fn_10017810(void *self_, int a1);  // 0x10017810
XI_API int __thiscall fn_10017C60(int *self_, int a2, int a3, int a4);  // 0x10017C60
XI_API void __cdecl fn_1001C930(int *a1);  // 0x1001C930
XI_API _DWORD * __cdecl Model_CreateTranslucentTargets(void);  // 0x1001D0A0
XI_API char __thiscall Model_BeginTranslucentPass(int self_);  // 0x1001D240
XI_API char __thiscall Model_TranslucentUnbindTexture(_DWORD *self_);  // 0x1001D350
XI_API int __thiscall Model_CompositeTranslucentPass(void *self_);  // 0x1001D3B0
XI_API char __cdecl TexObj_InitFromHeader(int a1, int a2, int a3, int a4);  // 0x1001D940
XI_API int __thiscall Tex_DrawNamedTexturePreview(void *self_, int a1, int a2);  // 0x1001DC70
XI_API int __stdcall Gfx_DrawScreenQuad256(int a1, int a2);  // 0x1001DD00
XI_API _DWORD * __thiscall fn_10025260(_DWORD *self_);  // 0x10025260
XI_API char __thiscall ProjTex_BeginPass(_DWORD *self_, int *a2, int a3, _DWORD *a4);  // 0x10025330
XI_API int __thiscall ProjTex_EndPass(void *self_, int a1);  // 0x100255C0
XI_API int __thiscall ProjTex_CaptureToTarget(void *self_, int a1, int a2, int a3, int a4);  // 0x10025870
XI_API char __stdcall EnvMap_BeginPass(int *a1, int a2, float *a3);  // 0x10025C40
XI_API int __stdcall EnvMap_EndPass(int a1);  // 0x10025F70
XI_API void __cdecl Dbg_DrawGroundGrid(void);  // 0x100262C0
XI_API void __cdecl Dbg_DrawViewAxisGizmo(void);  // 0x100263F0
XI_API void __stdcall Dbg_DrawAxesAt(int a1);  // 0x100266C0
XI_API void __cdecl Gfx_ProjectWorldToClip(float *a1, _DWORD *a2);  // 0x100276F0
XI_API void __cdecl Gfx_TransformWorldToView(float *a1, _DWORD *a2);  // 0x100277E0
XI_API void __cdecl Gfx_ProjectViewToClip(float *a1, _DWORD *a2);  // 0x10027870
XI_API float * __cdecl Gfx_NdcToScreen(float *a1);  // 0x100278E0
XI_API float * __cdecl Gfx_ScreenDeltaToNdc(float *a1, float *a2);  // 0x10027930
XI_API int __stdcall Mem_DrawHeapUsageBar(int a1, int a2, int a3, int a4, int a5);  // 0x100295A0
XI_API int __cdecl fn_10029A50(void);  // 0x10029A50
XI_API int __cdecl Gfx_ApplyCachedStreamSource(unsigned int a1);  // 0x1002F160
XI_API int __cdecl Gfx_ApplyCachedVertexShader(void);  // 0x1002F1A0
XI_API _DWORD * __thiscall fn_10033710(_DWORD *self_);  // 0x10033710
XI_API char __thiscall Model_BeginOffscreenPass(int self_, int *a2, int a3);  // 0x10033800
XI_API char __thiscall Model_EndOffscreenPassBindTextures(_DWORD *self_, int a2);  // 0x10033A70
XI_API int __cdecl Font_DrawPass(void);  // 0x10038520
XI_API unsigned __int8 * __cdecl Font_DrawString(unsigned __int8 *a1, float a2, float arg8, float a4);  // 0x10038720
XI_API _DWORD * __cdecl Post_CreateScreenEffectTargets(void);  // 0x10038930
XI_API int __thiscall Post_CaptureSceneToTarget(void *self_);  // 0x10038A80
XI_API int __thiscall Post_DrawRotatingGhostLayers(void *self_);  // 0x10038E10
XI_API char __thiscall TexObj_Create(int self_);  // 0x10039A80
XI_API int __thiscall TexObj_Upload(void *self_);  // 0x10039E50
XI_API int __cdecl Sky_DrawDome(void);  // 0x1003D9A0
XI_API int __cdecl Sky_DrawDomeViewCentered(void);  // 0x1003DB60
XI_API void __thiscall CMoElemFamily_SharedEarlyExit(void * self_);  // 0x10040C20
XI_API void __cdecl Font_DrawProjectedText3D(int a1, int a2, int a3, int a4, int a5);  // 0x10041C20
XI_API _DWORD * __thiscall fn_100426F0(float *self_);  // 0x100426F0
XI_API _DWORD * __cdecl Mo_InitSharedRenderResources(void);  // 0x10044270
XI_API int __thiscall fn_100467E0(void *self_, int a1);  // 0x100467E0
XI_API char ** __cdecl CMoRabRes_GetRuntimeClass(void);  // 0x10056260
XI_API int __thiscall Occlusion_QueryScreenRect(__int16 *self_, float *a2, int a3, float a4, float a5, int a6, int a7, int a8, char a9, int a10);  // 0x1006C280
XI_API int __stdcall Occlusion_DrawQuery(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);  // 0x1006C530
XI_API int __thiscall OcclusionMng_Prepare(_DWORD *self_);  // 0x1006C9B0
XI_API int __cdecl Gfx_InitCubeMapTextures(void);  // 0x10075280
XI_API void __thiscall TexRef_EnsureRenderTarget(int *self_);  // 0x100755A0
XI_API void __cdecl Actor_DrawWorldText(_DWORD *a1, const char *a2, float a3, unsigned int a4, int a5);  // 0x10086210
XI_API void __cdecl fn_10086900(_DWORD *a1, const char *a2, float a3);  // 0x10086900
XI_API char __thiscall fn_100AC390(void *self_, int a2, int a3, int a4);  // 0x100AC390
XI_API char __thiscall fn_100C4CE0(float *self_, float *a2, int a3, int a4);  // 0x100C4CE0
XI_API int __thiscall fn_1011AA40(__int16 *self_);  // 0x1011AA40
XI_API int __cdecl Gfx_ApplyViewport1B70(void);  // 0x1011AAC0
XI_API char __thiscall Font_BindGlyphTexture(int self_);  // 0x1011BAE0
XI_API int __thiscall Font_DrawGlyphQuad(float *self_, __int16 a2, int *a3, unsigned int a4, float a5, float a6);  // 0x1011BBD0
XI_API int __cdecl Gfx_SetAdditiveBlend(void);  // 0x1011BDC0
XI_API int __cdecl Gfx_SetCellBlendMode(int a1, int a2, int a3, int a4);  // 0x1011C1F0
XI_API void __thiscall Font_LoadHighResTexture(int *self_);  // 0x1011C720
XI_API int __thiscall Cell_DrawAt(void *self_, int a1, int a2);  // 0x1011E800
XI_API int __thiscall fn_1011E980(void *self_, int a1, int a2, int a3, int a4, int a5, int a6);  // 0x1011E980
XI_API void __thiscall Cell_DrawTransformed(int self_, int a2, int a3, float a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16, int a17, int a18, int a19, unsigned int a20, int a21);  // 0x1011EDE0
XI_API int __thiscall Cell_DrawStretchedWrapped(int self_, int a2, int a3, int a4, int a5, int a6, int a7, float a8, float a9, unsigned int a10, unsigned int a11, char a12);  // 0x1011EF60
XI_API int __thiscall Cell_DrawQuadBlended(void *self_, int a1, int a2, int a3, int a4, int a5);  // 0x1011F300
XI_API int __thiscall Cursor_Init(int self_);  // 0x10124440
XI_API int __thiscall fn_10124640(int *self_, int *a2, __int16 a3);  // 0x10124640
XI_API unsigned __int8 __thiscall Cursor_SetVisible(unsigned __int8 *self_, unsigned __int8 a2);  // 0x101252F0
XI_API int __cdecl Gfx_SetDefault2DStates(void);  // 0x101604B0
XI_API int __cdecl Gfx_DisableFogAndAlphaBlend(void);  // 0x10160650
XI_API int __cdecl fn_10162010(int a1, int a2);  // 0x10162010
XI_API int __cdecl Gfx_ApplyViewport1904(void);  // 0x101620B0
XI_API int __stdcall Gfx_DrawSolidRect2D(int a1, int a2);  // 0x10163400
XI_API int __stdcall Dbg_DrawTriangleLines(int a1, int a2, int a3);  // 0x1016DDF0
XI_API _DWORD * __thiscall fn_1016FCD0(void *self_, int *a2, int a3, int a4, int a5, int a6, _DWORD *a7, int a8);  // 0x1016FCD0
XI_API void __thiscall Dbg_DrawOverheadMapMarkers(_DWORD *self_, float *a2);  // 0x10174B10
XI_API int __cdecl Dbg_DrawOrientedBoxes(void);  // 0x10174EC0
XI_API int __stdcall fn_101771B0(int a1, int a2);  // 0x101771B0
XI_API int __thiscall Gfx_AssembleVertexShader(void *self_, int a1, int a2);  // 0x10177240
XI_API int __thiscall fn_101772E0(const char **self_, int a2);  // 0x101772E0
XI_API int __cdecl Gfx_SetZBias(unsigned int a1);  // 0x10177400
XI_API void __thiscall ZoneMap_SetOverlayProjection(float *self_);  // 0x10177420
XI_API int __cdecl Gfx_SetupLineDrawState(void);  // 0x101779C0
XI_API int __cdecl Gfx_DrawQuadOutline(float *a1, char a2, unsigned __int8 arg8, unsigned __int8 a4, char a5, char a6);  // 0x10177B40
XI_API int __cdecl Gfx_DrawLine3D(int a1, int a2, int a3, int a4, int a5, int a6);  // 0x10177C50
XI_API int __thiscall fn_10177CD0(void *self_, float *a2, int a3, float a4);  // 0x10177CD0
XI_API void __thiscall fn_10178250(void * self_);  // 0x10178250
XI_API int __thiscall ZoneMap_ReleaseStateBlock(unsigned int *self_);  // 0x10178B00
XI_API int __thiscall ZoneMap_DrawBox(void *self_);  // 0x101791F0
XI_API int __thiscall ZoneMap_CreateVertexShaders(char *self_);  // 0x101792A0
XI_API int __thiscall ZoneMap_DrawMeshGroupsWithFog(_DWORD *self_, _DWORD *a2);  // 0x1017ADE0
XI_API int __thiscall Zone_RenderEnvironment(int self_);  // 0x1017B510
XI_API int __stdcall Gfx_DrawWireBox(int a1);  // 0x10182F10
XI_API float * __thiscall fn_10183490(float *self_, float *pM1, int a3);  // 0x10183490
XI_API char __thiscall fn_10183760(float *self_, float *pM1, int a3, int a4, int a5);  // 0x10183760
XI_API _DWORD * __thiscall ZoneMap_DrawMeshById(int self_, int a2, int a3);  // 0x10183DA0
XI_API int __thiscall fn_10185CA0(void *self_, float *a1, float *a2, IDirect3DBaseTexture8 *a3, float a4);  // 0x10185CA0
XI_API _DWORD * __thiscall Env_ComputeAmbient(char *self_, _DWORD *a2, int a3);  // 0x10188E00
XI_API float * __thiscall Env_ComputeAmbientFloat4(char *self_, float *a2);  // 0x10188FA0
XI_API void __cdecl Gfx_ProjectWorldToScreen(_DWORD *a1, _WORD *a2, _WORD *a3);  // 0x101C8DA0
XI_API char __thiscall fn_101EEBD0(int self_);  // 0x101EEBD0
XI_API int __thiscall Movie_CreateFrameTextures(int self_, int a2);  // 0x1021E060
XI_API int __thiscall fn_102214D0(int *self_);  // 0x102214D0
XI_API char * __thiscall Ui_DrawKeyPrompt(char **self_);  // 0x102394A0
XI_API int __cdecl fn_1024CF50(_DWORD *a1);  // 0x1024CF50
XI_API void __cdecl StAvatar_SetViewFromDancerCamera(int a1);  // 0x1024F900
XI_API int __cdecl fn_102501C0(_BYTE *arg0);  // 0x102501C0
XI_API int __cdecl StAvatarFini(int *a1);  // 0x102504D0
XI_API char __thiscall Dancer_InitDevice(int *self_);  // 0x10251A80
XI_API int __thiscall Gfx_DrawSpriteAlphaTested(void *self_, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);  // 0x10253F60
XI_API IDirect3DDevice8 * __cdecl FontGlyphCache_GetDevice(void);  // 0x1025D750
XI_API int __cdecl FontGlyphCache_CreateTexture(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, D3DFORMAT a5, D3DPOOL a6);  // 0x1025D760
XI_API int __cdecl FontGlyphCache_CreateVertexBuffer(unsigned int a1, unsigned int a2, unsigned int a3, D3DPOOL a4);  // 0x1025D7B0
XI_API int __cdecl sqgxTextureCreateFromImage(int a1, _BYTE *a2);  // 0x10281510
XI_API int __cdecl sqgxTextureAddZ(int a1, int a2, int a3);  // 0x102816E0
XI_API int __cdecl sqgxTextureAdd(int a1, int a2, IDirect3DTexture8 *a3, _BYTE *a4);  // 0x10281830
XI_API int __cdecl sqgxTextureClutAdd(int a1, int a2, int a3);  // 0x102819D0
XI_API int __cdecl sqgxTextureClut(int a1);  // 0x10281D60
XI_API int __cdecl sqgxTextureUpdate(void);  // 0x10281DB0
XI_API int __cdecl sqgxTextureStencilClear(int a1);  // 0x10282010
XI_API int __cdecl sqgxTextureClear(float *a1, int a2);  // 0x10282030
XI_API int * __cdecl sqgxTextureDrawZ(IDirect3DSurface8 *a1);  // 0x102820E0
XI_API int * __cdecl sqgxTextureDraw(int a1, char a2);  // 0x10282190
XI_API int * __cdecl sqgxZTest(int a1);  // 0x10282350
XI_API int __cdecl sqgxAlphaTest(unsigned int a1, unsigned int a2);  // 0x102823F0
XI_API int * __cdecl sqgxZMask(int a1);  // 0x10282470
XI_API int * __cdecl sqgxFBMask(int a1, int a2, int a3, int a4);  // 0x102824C0
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
XI_API int __cdecl sqgxColorUpdate(void);  // 0x10283420
XI_API int * __cdecl sqgxViewport(float a1, float a2, float a3, float a4);  // 0x10283A10
XI_API int __cdecl sqgxPassAdd(int a1);  // 0x10283F70
XI_API int * __cdecl sqgxPass(int a1);  // 0x10283FC0
XI_API int __cdecl sqgxMatrixUpdate(void);  // 0x10284410
XI_API int __cdecl sqgxVertexShaderCreate(_DWORD *a1);  // 0x10284950
XI_API int __cdecl sqgxVertexShaderSelect(void);  // 0x10284A80
XI_API int * __cdecl sqgxShader(int a1, int a2);  // 0x10284B20
XI_API int __cdecl sqgxVertexShaderRelease(void);  // 0x10284EF0
XI_API int * __cdecl sqgxShapeIndexBufferBegin(void);  // 0x1028AA70
XI_API int __cdecl fn_1028AB50(void);  // 0x1028AB50
XI_API int * __cdecl sqgxShapeBegin(void);  // 0x1028AFE0
XI_API void __cdecl sqsnSceneDraw(int a1);  // 0x1028F8C0
XI_API int __cdecl sqsnShadowVolumeSetPass(int a1);  // 0x10297CC0
XI_API int __cdecl sqgxShadowStencilEnable(int a1);  // 0x10297EC0
XI_API int __cdecl sqsnShadowVolumeBeginVB(void);  // 0x102981E0
XI_API _DWORD * __cdecl sqgxDlistEnd(void);  // 0x1029B620
XI_API void __cdecl sqgxDlistDraw(IDirect3DVertexBuffer8 **a1);  // 0x1029B9D0
XI_API void __cdecl sqsnShadowVolumeDraw(int a1, int a2, int a3);  // 0x102A21F0
XI_API void __cdecl sqsnPlanarShadowDraw(int a1, int a2);  // 0x102A2530
