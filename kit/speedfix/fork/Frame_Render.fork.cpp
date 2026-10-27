// FORK OF: Frame_Render 0x10011A00 (from tree/src/unit_10011A00_Post.cpp)
// Edit the body below; `fork.py diff` shows the edit, `fork.py gate` runs it against the game's code.
// Hook it from the mod's xikit_mod_init with XI_FORK(host, Frame_Render).
#include "gfx_all.hpp"
#include "xi_ghidra.hpp"     // the Ghidra decompiler's types and helpers (float10, NAN, ...)

// ---- frame_step and uncap (speedfix; docs/RENDERER.md 3, 3a). Frame_Render makes the game's time step: the wait
// below runs until the frame's time v (in 1/60 s ticks) reaches the floor [this+0x30] (1: 60 fps); the step is
// min(max(trunc(the average of the last 4 v), [this+0x30]), 20) into [this+0x28], which the getters 0x10014CF0 and
// 0x10014D20 answer as max([+0x28], 1.0).
//   xi_uncap      the floor is read as 0 in this function (the wait and the step), so the frame does not wait; the
//                 game's [this+0x30] itself is not written.
//   xi_step_mode  1: the step without the truncation (same floor and cap) is added to a carry, and its whole part
//                 (xi_step_whole, possibly 0 above 60 fps) is what the getters' forks and this function's own copy at
//                 the top answer. [this+0x28] stays the game's.
// speedfix sets both; in the gate (XI_FRAMESTEP_GATE) they start at 1, otherwise at 0, which is the game's function.
#ifdef XI_FRAMESTEP_GATE
extern "C" { volatile int xi_step_mode = 1; volatile int xi_uncap = 1; volatile int xi_step_have = 1; float xi_step_whole = 0.0f; }
#else
extern "C" { __declspec(dllexport) volatile int xi_step_mode; __declspec(dllexport) volatile int xi_uncap; volatile int xi_step_have; float xi_step_whole; }   // (exported: the host test reads them)
#endif
extern "C" { float xi_step_frac; double xi_step_carry; }   // (weak in the getters' forks)
static inline int xi_floor(void *self_) { return xi_uncap != 0 ? 0 : *((int *)self_ + 12); }
// the step without the truncation (floor and cap as the game's) added to the carry; its whole part is this frame's
// answer. Exported so the host test (kit/speedfix/test/sf_host.c) can run it on the game's own ring average.
extern "C" __declspec(dllexport) void xi_step_make(void *self_, double avg)
{
  double f = avg;
  int fl = xi_floor(self_);
  if ( !(f >= fl) )                            // (a NaN average: the game's _ftol gives INT_MIN, the floor wins)
    f = fl;
  if ( f > 20.0 )
    f = 20.0;
  double c = xi_step_carry + f;
  int w = (int)c;                              // whole ticks; the fraction carried to the next frame
  xi_step_carry = c - w;
  xi_step_whole = (float)w;
  xi_step_frac = (float)f;
  xi_step_have = 1;
}

// FUNCTION: FFXIMAIN 0x10011A00  (unit unit_10011A00_Post: no unit evidence of its own; in the gap between 0x100119E0 (CXiActorNameDraw) and 0x100142B0 (CAtelIdleTask) -- an unnamed source file)
// GATE: PASS -- 100 trials, 68 returned normally, 560 D3D calls compared (diffemu_gfx, clean build)
XI_API unsigned __int8 __thiscall Frame_Render(void * self_)
{
  double v2;
  int result;
  int v4;
  int v5;
  int v6;
  int v7;
  int v8;
  int v9;
  int v10;
  int v11;
  int v12;
  int v13;
  __int16 v14;
  int v15; // kr00_4
  __int16 v16;
  __int16 v17;
  unsigned __int16 v18;
  int v19;
  unsigned __int16 v20;
  unsigned __int16 v21;
  unsigned __int16 v22;
  int v23;
  int v24;
  int v25;
  int v26;
  int *v27;
  CDx *v28;
  char v29;
  int v30;
  double v31;
  double v32;
  int v33;
  int *v34;
  double v35;
  int v36;
  double v37;
  double v38;
  double v39;
  __int16 v40; // fps
  bool v41; // c0
  char v42; // c2
  bool v43; // c3
  int *v44;
  double v45;
  int v46;
  double v47;
  double v48;
  __int16 v49; // fps
  double v50;
  bool v51; // c0
  char v52; // c2
  bool v53; // c3
  double v54;
  int v55;
  double v56;
  float *v57;
  double v58;
  double v59;
  double v60;
  __int16 v61; // fps
  double v62;
  bool v63; // c0
  char v64; // c2
  bool v65; // c3
  bool v66; // c0
  char v67; // c2
  bool v68; // c3
  int v69;
  int v70;
  double v71;
  __int16 v72; // fps
  bool v73; // c0
  char v74; // c2
  bool v75; // c3
  double v76;
  __int16 v77; // fps
  double v78;
  bool v79; // c0
  char v80; // c2
  bool v81; // c3
  bool v83; // c0
  char v84; // c2
  bool v85; // c3
  int v86;
  int v87;
  int v88;
  unsigned int v89;
  unsigned int v90;
  char v91;
  alignas(16) unsigned char xi_frame[0x888];  // address-taken locals at their original offsets
  float &v92 = *reinterpret_cast<float *>(xi_frame + 0x234);
  int &v93 = *reinterpret_cast<int *>(xi_frame + 0x248);
  int &v94 = *reinterpret_cast<int *>(xi_frame + 0x248);
  int &v95 = *reinterpret_cast<int *>(xi_frame + 0x248);
  float &v96 = *reinterpret_cast<float *>(xi_frame + 0x248);
  int &v97 = *reinterpret_cast<int *>(xi_frame + 0x248);
  int &v98 = *reinterpret_cast<int *>(xi_frame + 0x248);
  char &v99 = *reinterpret_cast<char *>(xi_frame + 0x24F);
  char &i = *reinterpret_cast<char *>(xi_frame + 0x24F);
  int &a4 = *reinterpret_cast<int *>(xi_frame + 0x250);
  int &v102 = *reinterpret_cast<int *>(xi_frame + 0x254);
  int &a2 = *reinterpret_cast<int *>(xi_frame + 0x258);
  int &v104 = *reinterpret_cast<int *>(xi_frame + 0x25C);
  struct _SYSTEMTIME &SystemTime = *reinterpret_cast<struct _SYSTEMTIME *>(xi_frame + 0x260);
  _DWORD (&v106)[6] = *reinterpret_cast<_DWORD (*)[6]>(xi_frame + 0x270);
  char (&Buffer)[512] = *reinterpret_cast<char (*)[512]>(xi_frame + 0x288);
  bool xi_fpu_unord = false;

  g_modelsDrawnThisFrame = 0;
  g_modelDrawCallCount = 0;
  if ( g_pEnv != nullptr )
  {
    if ( xi_step_mode != 0 && xi_step_have != 0 )
      v2 = xi_step_whole;
    else if ( !(*((float *)self_ + 10) < 1.0) )
      v2 = *((float *)self_ + 10);
    else
      v2 = 1.0;
    orig::j_CEnv_UpdateDualTargetMode((int)g_pEnv, (__int64)v2);
  }
  if ( !orig::CDx_TestCooperativeLevel(g_pCDx) )
  {
    *((_BYTE *)g_pApp + 28) = 0;
    if ( g_windowMode != 0 )
      result = orig::App_SetErrorCode(10);
    else
      result = orig::App_SetErrorCode(3);
    result = (int)(((uint32_t)(result) & 0xFFFFFF00u) | ((uint32_t)(uint8_t)(0) << 0));
    return result;
  }
  orig::CDx_BeginScene(g_pCDx);
  orig::CDx_SetDefaultRenderState(g_pCDx);
  orig::CDx_NopA();
  orig::TexMgr_UploadPumpEntry(g_pTextureMng);
  if ( g_pAppInputState != 0 && orig::Input_IsCursorActive((_DWORD *)g_pAppInputState, 0) != 0 )
    orig::Unit1018BA60_Call103113C8();
  if ( orig::CEnv_IsStereoMode(g_pEnv) != 0 )
  {
    orig::CDx_PushRenderTarget(
      g_pCDx,
      *(_DWORD *)&(*(unsigned __int8 *)((char *)g_pCDx + 0x198)),
      *(_DWORD *)&(*(unsigned __int8 *)((char *)g_pCDx + 0x1A0)),
      *(_DWORD *)&(*(unsigned __int8 *)((char *)g_pCDx + 0x1A8)));
    orig::CDx_EndAndClearBegin(g_pCDx, 3, *((_DWORD *)self_ + 194), 1065353216, 0);
    orig::CDx_SetTexture(g_pCDx, 0, 0);
  }
  v99 = orig::CDx_PushRenderTargetRet1(
          g_pCDx,
          *(_DWORD *)&(*(unsigned __int8 *)((char *)g_pCDx + 0x194)),
          *(_DWORD *)&(*(unsigned __int8 *)((char *)g_pCDx + 0x19C)),
          *(_DWORD *)&(*(unsigned __int8 *)((char *)g_pCDx + 0x1A4)));
  orig::CDx_EndAndClearBegin(g_pCDx, 3, *((_DWORD *)self_ + 194), 1065353216, 0);
  v4 = g_pSceneState;
  if ( *(_DWORD *)g_pSceneState == 160 )
  {
    *((_DWORD *)self_ + 17) = 13;
LABEL_39:
    while ( 2 )
    {
      v4 = g_pSceneState;
LABEL_40:
      v7 = *((_DWORD *)self_ + 17);
      if ( v7 != *((_DWORD *)self_ + 16) )
      {
        *((_DWORD *)self_ + 16) = v7;
        orig::App_SetSeqStep(self_, 0);
        *((_DWORD *)self_ + 18) = 0;
        v4 = g_pSceneState;
      }
      switch ( *((_DWORD *)self_ + 17) )
      {
        case 0:
          if ( *(_DWORD *)(g_pAppSettings + 96) != 0 )
          {
            v8 = orig::Scene_GetTableValue(1);
            g_frameMeshRes = *orig::sub_100731B0((_DWORD *)g_pMeshResourceMng, &a4, v8);
            orig::CMoResource_AddRefChain(*(_DWORD *)g_frameMeshRes);
          }
          if ( *(_DWORD *)(g_pAppSettings + 40) != 0 )
          {
            dword_10669014 = 0;
            v9 = orig::App_GetGlobal_104569C8();
            if ( v9 != 0 )
            {
              v10 = v9 - 1;
              if ( v10 != 0 )
              {
                if ( v10 == 1 )
                {
                  dword_1045694C = 0;
                  dword_10669014 = 1;
                  if ( byte_10669420 == 1 )
                    dword_10456950 = 1;
                  orig::LobbyThread_SetGlobal106690AC(1);
                  orig::App_SetGlobal_104569C8(0);
                  *((_BYTE *)g_pCamera + 9) = 1;
                  *((_DWORD *)self_ + 205) = 0x80000000;
                  *((_DWORD *)self_ + 17) = 2;
                  orig::Unit1018BA60_RunPairA();
                }
              }
              else
              {
                orig::App_SetGlobal_104569C8(0);
                *((_DWORD *)self_ + 17) = 8;
                *((_BYTE *)g_pCamera + 9) = 1;
                if ( byte_10669420 == 1 )
                  dword_10456950 = 1;
                orig::Unit1018BA60_RunPairA();
              }
            }
            else
            {
              *((_DWORD *)self_ + 17) = 1;
            }
          }
          else
          {
            *((_BYTE *)g_pCamera + 9) = 0;
            *((_DWORD *)self_ + 17) = 10;
            orig::Unit100D7420_InitTimer();
          }
          break;
        case 1:
          if ( g_cfg0022 == 0 || orig::App_RunOpening(self_) != 0 )
            *((_DWORD *)self_ + 17) = 2;
          break;
        case 2:
          orig::__snprintf(Buffer, 0x200u, aNet3Port54001);
          orig::Unit100014A0_AppendWithSpace(Buffer);
          *(_DWORD *)(g_pAppSettings + 40) = 3;
          *((_DWORD *)self_ + 17) = 3;
          *((_WORD *)self_ + 2) = 0;
          if ( *(_DWORD *)(g_pAppSettings + 40) != 0 )
          {
            if ( orig::Game_InitSceneState((int)g_pGameMode) != 0 )
            {
              *((_BYTE *)g_pCamera + 9) = 1;
              orig::Unit1018BA60_RunPairA();
            }
            else
            {
              orig::App_SetErrorCode(5);
              *((_BYTE *)self_ + 10) = 0;
            }
          }
          break;
        case 3:
          if ( *(_DWORD *)(g_pAppSettings + 36) == 0 || (*(_DWORD *)v4 = 32, orig::fn_10015740()) )
            *((_DWORD *)self_ + 17) = 4;
          break;
        case 4:
          if ( orig::fn_10015740() )
          {
            if ( dword_10456950 != 0 || *(_DWORD *)(g_pAppSettings + 88) != 0 )
              g_cfg0023 = 1;
            if ( g_pObj_Vtbl10329F8C != 0 )
            {
              orig::Unit10017C10_LoadChecksum((_DWORD *)(g_pObj_Vtbl10329F8C + 388), (FILE *)&Stream._file, 7u);
              *(_DWORD *)(g_pSceneState + 28) = *(_DWORD *)(g_pObj_Vtbl10329F8C + 396);
            }
            *((_DWORD *)self_ + 17) = 5;
          }
          break;
        case 5:
          *(_DWORD *)v4 = 64;
          if ( orig::App_StepGameState5Connect((int)self_, (int)self_ + 72) != 0 )
          {
            v11 = *((_DWORD *)self_ + 18);
            if ( v11 != 0 )
            {
              if ( v11 == 1 )
                *((_DWORD *)self_ + 17) = 13;
            }
            else
            {
              *((_DWORD *)g_pGameMode + 4) = 1;
              XiControlActor_isFirstPersonView = 0;
              XiControlActor_isAutoRunning = 0;
              byte_1047D694 = 0;
              orig::Unit10253210_SendBytePairKind2(0, 1);
              v12 = orig::Unit100D7420_GetRecordDwordFwd(*((_DWORD *)g_pGameMode + 32));
              if ( orig::Unit1018BA60_FileMngFwd101C2B60((int *)g_pFileMng, v12) == 0 )
              {
                if ( !orig::Unit1018BA60_FileMngFwd101C2C80((int *)g_pFileMng) )
                  orig::Unit1018BA60_FileMngFwd101C2E70((int *)g_pFileMng);
                v13 = orig::Unit100D7420_GetRecordDwordFwd(*((_DWORD *)g_pGameMode + 32));
                orig::Unit1018BA60_FileMngFwd101C4170((void *)g_pFileMng, v13);
              }
              orig::Unit10253210_SendBytePairKind2(1, 1);
              *((_DWORD *)self_ + 17) = 6;
            }
          }
          break;
        case 6:
          *(_DWORD *)v4 = 112;
          if ( orig::App_StepGameState6(self_) != 0 )
            *((_DWORD *)self_ + 17) = 10;
          break;
        case 7:
          *(_DWORD *)v4 = 112;
          if ( orig::App_StepGameState7(self_) != 0 )
            *((_DWORD *)self_ + 17) = 10;
          break;
        case 8:
          *(_DWORD *)v4 = 112;
          if ( orig::App_StepGameState8(self_) != 0 )
            *((_DWORD *)self_ + 17) = 10;
          break;
        case 9:
          *(_DWORD *)v4 = 112;
          if ( orig::App_StepGameState9() )
            *((_DWORD *)self_ + 17) = 10;
          break;
        case 0xA:
          orig::App_DispatchSubState(self_);
          break;
        case 0xB:
          if ( (*(_BYTE *)(v4 + 24) & 8) != 0 )
          {
            *((_DWORD *)self_ + 17) = 13;
            orig::App_SetGlobal_104569C5(1);
          }
          else
          {
            orig::App_SaveAllSlotData();
            *((_DWORD *)g_pGameMode + 4) = 0;
            *(_DWORD *)g_pSceneState = 112;
            orig::Unit10014350_EnterState4AfterShutdown(self_);
          }
          break;
        case 0xC:
          *((_DWORD *)g_pGameMode + 4) = 0;
          *(_DWORD *)g_pSceneState = 112;
          if ( orig::Unit10014350_TryShutdownActors(self_) )
          {
            orig::Unit10035B00_ResetGlobals();
            orig::Ui_KeyPrompt_Init((_DWORD *)g_pKeyPrompt, 2);
            orig::MenuMng_OpenByName(g_menuMng, aMenuChdummy, 1, 0);
            *((_DWORD *)self_ + 17) = 7;
          }
          break;
        case 0xD:
          orig::App_SaveAllSlotData();
          *((_DWORD *)g_pGameMode + 4) = 0;
          *(_DWORD *)g_pSceneState = 160;
          orig::Unit10014350_StepStagedRelease((int)self_);
          break;
        case 0x11:
          orig::CDx_StepFadeCounter((float *)g_pCDx, 0.0, 0.0, 1);
          orig::CDx_SetClearColor(g_pCDx, 0x80000000);
          *((_DWORD *)self_ + 206) = 0x80000000;
          *((_DWORD *)self_ + 205) = -2139062144;
          if ( orig::Unit10014350_StepMenuOkThenRelease(self_) != 0 )
          {
            if ( dword_10456948 != 12 )
            {
              *((_DWORD *)self_ + 17) = 13;
              continue;
            }
            *((_DWORD *)self_ + 17) = 11;
          }
          break;
        default:
          goto LABEL_102;
      }
      goto LABEL_102;
    }
  }
  v5 = *((_DWORD *)self_ + 17);
  if ( v5 == 17 || *(_DWORD *)g_pSceneState == 144 )
    goto LABEL_40;
  if ( v5 == 13 )
  {
LABEL_38:
    (*(unsigned __int8 *)((char *)g_pCDx + 0x828)) = 1;
    orig::sub_1018B130();
    orig::sub_1018C2B0();
    orig::StDancer_Update(g_pStDancer);
    orig::sub_1018B860(g_pXiZone);
    orig::sub_10074C30((_DWORD *)dword_1047D140);
    orig::sub_102515F0(g_pStDancer, 1);
    orig::CYySoundElem_UpdateQueue();
    goto LABEL_39;
  }
  if ( g_pGameMode == nullptr )
  {
LABEL_34:
    if ( *((int *)self_ + 17) >= 5 && *(_DWORD *)v4 != 64 && Code < 0 )
      { orig::_exit_0(2001); goto LABEL_102; }
    goto LABEL_38;
  }
  v6 = orig::GameMode_PollPendingState((int *)g_pGameMode);
  if ( v6 != 12 )
  {
    switch ( v6 )
    {
      case 21:
        *((_DWORD *)self_ + 17) = 17;
        dword_10456948 = 21;
        goto LABEL_102;
      case 202:
        orig::_exit_0(4003); goto LABEL_102;
      case 203:
        v4 = g_pSceneState;
        if ( *(_DWORD *)g_pSceneState != 64 )
          { orig::_exit_0(4004); goto LABEL_102; }
        break;
      case 201:
        orig::_exit_0(4005); goto LABEL_102;
      default:
        v4 = g_pSceneState;
        break;
    }
    goto LABEL_34;
  }
  *((_DWORD *)self_ + 17) = 17;
  dword_10456948 = 12;
LABEL_102:
  (*(unsigned __int8 *)((char *)g_pCDx + 0x828)) = 0;
  if ( g_pDebugOverlay != 0 )
    orig::sub_10003650((void *)g_pDebugOverlay);
  orig::CDx_SetDefaultRenderState(g_pCDx);
  if ( g_depthBlurEnabled_Dead != 0
    && v99 == 1
    && g_pMovie == 0
    && *(_DWORD *)&(*(unsigned __int8 *)((char *)g_pCDx + 0x1C0)) != 0
    && g_pXiZone != nullptr
    && orig::ZoneMap_GetByteRef1CPlus10(g_pXiZone) != 0 )
  {
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_TEXCOORDINDEX, 0);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_MAGFILTER, 2u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_MINFILTER, 2u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_ADDRESSU, 3u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_ADDRESSV, 3u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_COLOROP, 2u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_COLORARG1, 2u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_ALPHAOP, 2u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_ALPHAARG1, 2u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 1u, D3DTSS_ALPHAOP, 1u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 1u, D3DTSS_COLOROP, 1u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), D3DRS_ZENABLE, 0);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), D3DRS_ZWRITEENABLE, 0);
    orig::CDx_PushRenderTarget(
      g_pCDx,
      *(_DWORD *)&(*(unsigned __int8 *)((char *)g_pCDx + 0x1C0)),
      *(_DWORD *)&(*(unsigned __int8 *)((char *)g_pCDx + 0x1C4)),
      *(_DWORD *)&(*(unsigned __int8 *)((char *)g_pCDx + 0x1C8)));
    orig::CDx_DrawTextureToViewport(g_pCDx, *(_DWORD *)&(*(unsigned __int8 *)((char *)g_pCDx + 0x194)), -2139062144, -1);
    orig::CDx_SetTexture(g_pCDx, 0, 0);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), D3DRS_ZENABLE, 1u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), D3DRS_ZWRITEENABLE, 1u);
    *(float *)&a4 = 0.0;
    v102 = 0;
    *(float *)&a2 = 0.0;
    v104 = 1065353216;
    *(float *)&a2 = -orig::ZoneMap_GetFloatRef1CPlus4(g_pXiZone);
    orig::Mat4_TransformPointInPlace((float *)self_ + 165, (float *)&a4);
    orig::CDx_SetTransform(g_pCDx, 256, g_frameWorldMatrix);
    orig::CDx_SetTransform(g_pCDx, 2, (char *)self_ + 148);
    orig::Gfx_ProjectWorldToClip((float *)&a4, &a4);
    orig::Post_DepthBlurComposite(g_pCDx, *(_DWORD *)&(*(unsigned __int8 *)((char *)g_pCDx + 0x1C0)), a2);
  }
  orig::Post_Afterimage();
  orig::Post_FlashAndZoomFeedback(self_);
  if ( (unsigned __int8)g_screenFadeLevel < 0x80u )
  {
    HIWORD(v93) = (unsigned __int8)(g_screenFadeLevel + *((_BYTE *)self_ + 802) + 0x80);
    BYTE1(v93) = g_screenFadeLevel + *((_BYTE *)self_ + 801) + 0x80;
    LOBYTE(v93) = g_screenFadeLevel + *((_BYTE *)self_ + 800) + 0x80;
    *((_DWORD *)self_ + 206) = v93;
  }
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_TEXCOORDINDEX, 0);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_MAGFILTER, 2u);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_MINFILTER, 2u);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_ADDRESSU, 3u);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_ADDRESSV, 3u);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_COLOROP, 5u);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_COLORARG1, 2u);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_COLORARG2, 0);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_ALPHAOP, 5u);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_ALPHAARG1, 2u);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_ALPHAARG2, 0);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 1u, D3DTSS_ALPHAOP, 1u);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 1u, D3DTSS_COLOROP, 1u);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), D3DRS_ZENABLE, 0);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), D3DRS_ZWRITEENABLE, 0);
  v14 = *((_WORD *)self_ + 12);
  v15 = g_screenRectBottomRight;
  v16 = HIWORD(g_screenRectTopLeft);
  LOWORD(a4) = g_screenRectTopLeft;
  LOWORD(g_screenRectBottomRight) = v14;
  v17 = *((_WORD *)self_ + 13);
  LOWORD(v102) = v15;
  HIWORD(g_screenRectBottomRight) = v17;
  orig::sub_101D0B00();
  LOWORD(g_screenRectTopLeft) = a4;
  HIWORD(g_screenRectTopLeft) = v16;
  LOWORD(g_screenRectBottomRight) = v102;
  HIWORD(g_screenRectBottomRight) = (_WORD)((uint32_t)(v15) >> 16);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), D3DRS_ZENABLE, 1u);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), D3DRS_ZWRITEENABLE, 1u);
  orig::CDx_SetTexture(g_pCDx, 0, 0);
  v18 = *((_WORD *)self_ + 8);
  v19 = *((unsigned __int16 *)self_ + 9);
  v106[0] = 0;
  v106[1] = 0;
  v106[2] = v18;
  v106[3] = v19;
  v106[4] = 0;
  v106[5] = 1065353216;
  orig::CDx_SetViewport(g_pCDx, v106);
  v20 = *((_WORD *)self_ + 12);
  *(_DWORD *)&SystemTime.wHour = *((unsigned __int16 *)self_ + 8);
  v21 = *((_WORD *)self_ + 9);
  v22 = *((_WORD *)self_ + 13);
  a2 = v20;
  *(_DWORD *)&SystemTime.wSecond = v21;
  v23 = *((_DWORD *)self_ + 200);
  v104 = v22;
  v94 = v23;
  HIBYTE(v94) = 0x80;
  *(_DWORD *)&SystemTime.wYear = 0;
  *(_DWORD *)&SystemTime.wDayOfWeek = 0;
  *(float *)&a4 = 0.0;
  v102 = 0;
  orig::CDx_CompositeSceneToBackBuffer(g_pCDx, (int)&(*(unsigned __int8 *)((char *)g_pCDx + 0x190)), v94, (int)&SystemTime, (int)&a4);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), D3DRS_ZENABLE, 0);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), D3DRS_ZWRITEENABLE, 0);
  orig::CDx_NopB();
  if ( g_pGameMode != nullptr )
  {
    v24 = *((_DWORD *)g_pGameMode + 46);
    if ( v24 < 0 )
    {
      Code = *((_DWORD *)g_pGameMode + 46);
      if ( v24 == -1056 )
        orig::App_SetGlobal_104569C6(1);
    }
  }
  v25 = *(_DWORD *)&(*(unsigned __int8 *)((char *)g_pCDx + 0x1E0));
  v26 = *(_DWORD *)&(*(unsigned __int8 *)((char *)g_pCDx + 0x1E8));
  if ( v25 != 0 && v26 != 0 )
  {
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), D3DRS_ZENABLE, 0);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), D3DRS_ZWRITEENABLE, 0);
    orig::CDx_PushRenderTarget(g_pCDx, v25, *(_DWORD *)&(*(unsigned __int8 *)((char *)g_pCDx + 0x1E4)), v26);
    v95 = *((_DWORD *)self_ + 200);
    HIBYTE(v95) = 0;
    orig::CDx_DrawTextureToViewport(g_pCDx, *(_DWORD *)&(*(unsigned __int8 *)((char *)g_pCDx + 0x194)), v95, -1);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), D3DRS_ZENABLE, 1u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), D3DRS_ZWRITEENABLE, 1u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), D3DRS_COLORWRITEENABLE, 15u);
  }
  orig::Unit1018BA60_WatchConfigWindow((int)ArgList);
  orig::sub_10005A40(g_pAppInputState);
  orig::Cell_ClearFlag104E1C3C();
  orig::Input_DispatchButtons((_DWORD *)g_pAppInputState);
  if ( g_pTargetMng != nullptr )
    orig::Target_Update(g_pTargetMng);
  if ( *((_BYTE *)g_pCamera + 8) != 0 )
  {
    if ( dword_106627EC != 0 )
      orig::Unit1018BA60_HandleMenuKeys((_BYTE *)dword_106627EC);
    if ( dword_10662DA8 != 0 )
      orig::Unit1018BA60_UpdateShortcuts((void *)dword_10662DA8);
    if ( dword_1066279C != 0 )
      orig::Unit1018BA60_UpdateMenuState(dword_1066279C);
  }
  if ( *((_BYTE *)g_pCamera + 8) != 0 && g_pFileMng != 0 )
    orig::Unit1018BA60_FileMngCall101BEB90((_DWORD *)g_pFileMng);
  if ( *((_BYTE *)g_pCamera + 8) != 0 )
    orig::Unit1018BA60_HandleJumpMapKeys();
  v27 = orig::Unit1012AC80_GetInstance();
  orig::Unit1012AC80_UpdatePendingRead(v27);
  if ( *((_BYTE *)g_pCamera + 8) != 0 )
  {
    if ( dword_106627EC != 0 )
    {
      orig::Gfx_SetDefault2DStates();
      *(_BYTE *)dword_106627EC = 1;
      orig::sub_1015FA00((int *)g_menuMng);
      orig::Menu_DrawLayers(g_menuMng);
      orig::Gfx_DisableFogAndAlphaBlend();
    }
    orig::FontGlyphCache_GetCounterBlock();
    orig::FontGlyphCache_StepCounter();
    orig::FontGlyphCache_GetCounterBlock();
    orig::Unit1025C9E0_ReturnFalseFwd();
    if ( *((_DWORD *)self_ + 17) != 17 && *(_DWORD *)g_pSceneState != 144 && g_C_106631D4 != 0 )
      orig::Unit101D2FF0_Update((_WORD *)g_C_106631D4);
    if ( g_pXiOpening != 0 )
      orig::sub_10018850(g_pXiOpening);
  }
  orig::Cursor_UpdateTimer((float *)g_pCursor);
  v28 = g_pCDx;
  if ( *(_DWORD *)&(*(unsigned __int8 *)((char *)g_pCDx + 0x1E0)) != 0 && *(_DWORD *)&(*(unsigned __int8 *)((char *)g_pCDx + 0x1E8)) != 0 )
  {
    orig::CDx_SetTexture(g_pCDx, 0, 0);
    orig::sub_1000ABE0(g_pCDx, *(_DWORD *)&(*(unsigned __int8 *)((char *)g_pCDx + 0x1E0)), 1065353216);
    v28 = g_pCDx;
  }
  orig::CDx_DrawFadeQuad(v28, 0, *((_DWORD *)self_ + 199));
  v29 = *(_BYTE *)(g_pSceneState + 24);
  a4 = *((unsigned __int16 *)self_ + 9);
  v30 = *(_DWORD *)(g_pSceneState + 8);
  v31 = (double)a4 * 0x1.1931940000000p-9;
  if ( v29 < 0 && (v30 == 2 || v30 == 3) || v30 == 1 )
    v32 = -110.0;
  else
    v32 = -160.0;
  a4 = *((unsigned __int16 *)self_ + 8);
  *(_DWORD *)&SystemTime.wYear = 0;
  *(float *)&SystemTime.wDayOfWeek = (double)a4 * 0x1.99999a0000000p-10;
  *(float *)&SystemTime.wHour = v32;
  *(float *)&SystemTime.wSecond = v31;
  orig::CXiMovie_DrawFrame(0, 0, 640, 512, (float *)&SystemTime);
  orig::sub_101D0C70();
  orig::Font_QueueText(c_strSpace, 1134886912, 1111490560, 0);
  orig::Font_DrawPass();
  if ( g_pEnv != nullptr && (g_pEnv[7] & 1) != 0 )
  {
    *(_DWORD *)&SystemTime.wYear = 0;
    v33 = *((unsigned __int16 *)self_ + 9);
    a4 = v33;
    *(_DWORD *)&SystemTime.wDayOfWeek = (__int64)((double)v33 - 10.0);
    *(_DWORD *)&SystemTime.wHour = *((unsigned __int16 *)self_ + 8);
    *(_DWORD *)&SystemTime.wSecond = v33;
    orig::CDx_DrawStereoOddFrameOverlay(g_pCDx, (int)&SystemTime);
  }
  orig::CDx_EndScene(g_pCDx);
  orig::CDx_Present();
  *((_DWORD *)self_ + 10) = 0;
  for ( i = 0; ; i = 1 )
  {
    a4 = *((int *)self_ + 7);
    v34 = (int *)a4;
    v35 = ((double (__thiscall *)(int *))*(_DWORD *)(*(_DWORD *)a4 + 48))(v34);
    v36 = *v34;
    *(float *)&a4 = v35;
    v37 = ((double (__thiscall *)(int *))*(_DWORD *)(v36 + 36))(v34);
    g_frameStepFps = v37 * *(float *)&a4;
    v38 = 60.0 / g_frameStepFps;
    *((float *)self_ + 11) = v38;
    *((float *)self_ + 10) = v38;
    v39 = (double)xi_floor(self_);
    (xi_fpu_unord = __builtin_isunordered(v38, v39), v41 = v38 < v39);
    v42 = 0;
    v43 = v38 == v39;
    if ( ((__int16)(((v41 || xi_fpu_unord) ? 0x100 : 0) | ((xi_fpu_unord) ? 0x400 : 0) | ((v43 || xi_fpu_unord) ? 0x4000 : 0)) & 0x100) == 0 )
      break;
    Sleep(1u);
    _mm_pause();
  }
  if ( i != 1 && v38 < (double)xi_floor(self_) )
  {
    do
    {
      a4 = *((int *)self_ + 7);
      v44 = (int *)a4;
      v45 = ((double (__thiscall *)(int *))*(_DWORD *)(*(_DWORD *)a4 + 48))(v44);
      v46 = *v44;
      *(float *)&a4 = v45;
      v47 = ((double (__thiscall *)(int *))*(_DWORD *)(v46 + 36))(v44);
      g_frameStepFps = v47 * *(float *)&a4;
      v48 = 60.0 / g_frameStepFps;
      *((float *)self_ + 11) = v48;
      *((float *)self_ + 10) = v48;
      Sleep(0);
      v50 = (double)xi_floor(self_);
      (xi_fpu_unord = __builtin_isunordered(v50, *((float *)self_ + 10)), v51 = v50 < *((float *)self_ + 10));
      v52 = 0;
      v53 = v50 == *((float *)self_ + 10);
    }
    while ( ((__int16)(((v51 || xi_fpu_unord) ? 0x100 : 0) | ((xi_fpu_unord) ? 0x400 : 0) | ((v53 || xi_fpu_unord) ? 0x4000 : 0)) & 0x4100) == 0 );
  }
  v54 = 60.0 / ((double (__thiscall *)(_DWORD))*(_DWORD *)(**((_DWORD **)self_ + 7) + 36))(*((_DWORD *)self_ + 7))
      + *((float *)self_ + 15);
  v55 = *((_DWORD *)self_ + 7);
  a4 = (__int64)v54;
  *((float *)self_ + 14) = *(float *)&a4;
  *((float *)self_ + 15) = v54 - (double)(int)(__int64)v54;
  (*(void (__fastcall **)(int, _DWORD))(*(_DWORD *)v55 + 32))(v55, (unsigned __int64)(__int64)v54 >> 32);
  v56 = *((float *)self_ + 10);
  g_frameStepRingIndex = ((_BYTE)g_frameStepRingIndex + 1) & 3;
  g_frameStepRing[g_frameStepRingIndex] = *((_DWORD *)self_ + 10);
  *((_DWORD *)self_ + 11) = 0;
  v57 = (float *)g_frameStepRing;
  do
  {
    v58 = *v57++ + *((float *)self_ + 11);
    *((float *)self_ + 11) = v58;
  }
  while ( (int)v57 < (int)&g_frameStepRingIndex );
  v59 = *((float *)self_ + 11) * 0.25;
  *((float *)self_ + 11) = v59;
  if ( xi_step_mode != 0 )
    xi_step_make(self_, v59);
  a4 = (__int64)v59;
  v60 = (double)a4;
  *((float *)self_ + 10) = v60;
  v96 = (float)xi_floor(self_);
  if ( v60 < v96 )
    *((float *)self_ + 10) = v96;
  v62 = *((float *)self_ + 10);
  (xi_fpu_unord = __builtin_isunordered(v62, 20.0), v63 = v62 < 20.0);
  v64 = 0;
  v65 = v62 == 20.0;
  if ( ((__int16)(((v63 || xi_fpu_unord) ? 0x100 : 0) | ((xi_fpu_unord) ? 0x400 : 0) | ((v65 || xi_fpu_unord) ? 0x4000 : 0)) & 0x4100) == 0 )
    *((_DWORD *)self_ + 10) = 1101004800;
  (xi_fpu_unord = __builtin_isunordered(v56, 20.0), v66 = v56 < 20.0);
  v67 = 0;
  v68 = v56 == 20.0;
  if ( ((__int16)(((v66 || xi_fpu_unord) ? 0x100 : 0) | ((xi_fpu_unord) ? 0x400 : 0) | ((v68 || xi_fpu_unord) ? 0x4000 : 0)) & 0x4100) == 0 )
    v56 = 20.0;
  *((float *)self_ + 11) = v56;
  g_frameStepFps = 60.0 / *((float *)self_ + 10);
  v69 = *((_DWORD *)self_ + 13) + 1;
  *((_DWORD *)self_ + 13) = v69;
  if ( v69 == 0 )
    *((_DWORD *)self_ + 13) = 1;
  v70 = *((_DWORD *)self_ + 8);
  if ( v70 != 0 )
  {
    v71 = ((double (__thiscall *)(int))*(_DWORD *)(*(_DWORD *)v70 + 40))(v70);
    *(float *)&v97 = v71;
    (xi_fpu_unord = __builtin_isunordered(v71, 4000.0), v73 = v71 < 4000.0);
    v74 = 0;
    v75 = v71 == 4000.0;
    if ( ((__int16)(((v73 || xi_fpu_unord) ? 0x100 : 0) | ((xi_fpu_unord) ? 0x400 : 0) | ((v75 || xi_fpu_unord) ? 0x4000 : 0)) & 0x4100) == 0 )
    {
      GetLocalTime(&SystemTime);
      if ( SystemTime.wSecond != (_WORD)g_frameCounterLastSecond )
      {
        (*(void (__thiscall **)(_DWORD))(**((_DWORD **)self_ + 8) + 32))(*((_DWORD *)self_ + 8));
        a4 = SystemTime.wSecond - (unsigned __int16)g_frameCounterLastSecond;
        v76 = (double)a4;
        if ( v76 < 0.0 )
          v76 = v76 + 60.0;
        *(float *)&a4 = v76 * 1000.0;
        v78 = *(float *)&v97 - *(float *)&a4;
        if ( v78 < 0.0 )
          v78 = -v78;
        if ( dword_1045695C != 0 )
        {
          if ( dword_1045695C == 1 )
          {
            (xi_fpu_unord = __builtin_isunordered(v78, 1500.0), v83 = v78 < 1500.0);
            v84 = 0;
            v85 = v78 == 1500.0;
            if ( ((__int16)(((v83 || xi_fpu_unord) ? 0x100 : 0) | ((xi_fpu_unord) ? 0x400 : 0) | ((v85 || xi_fpu_unord) ? 0x4000 : 0)) & 0x100) == 0 )
              dword_1045695C = 2;
          }
          else if ( dword_1045695C == 2 )
          {
            (xi_fpu_unord = __builtin_isunordered(v78, 1500.0), v79 = v78 < 1500.0);
            v80 = 0;
            v81 = v78 == 1500.0;
            if ( ((__int16)(((v79 || xi_fpu_unord) ? 0x100 : 0) | ((xi_fpu_unord) ? 0x400 : 0) | ((v81 || xi_fpu_unord) ? 0x4000 : 0)) & 0x100) != 0 )
            {
              if ( dword_10456958++ > 2 )
              {
                (*(void (__thiscall **)(_DWORD, int))(**((_DWORD **)self_ + 7) + 52))(*((_DWORD *)self_ + 7), 1065353216);
                dword_1045695C = 1;
                dword_10456954 = 0;
                dword_10456958 = 0;
              }
            }
            else
            {
              dword_10456958 = 0;
              v92 = *(float *)&v97 / *(float *)&a4;
              (*(void (__thiscall **)(_DWORD, _DWORD))(**((_DWORD **)self_ + 7) + 52))(
                *((_DWORD *)self_ + 7),
                LODWORD(v92));
              if ( ++dword_10456954 > 45 )
              {
                dword_10456954 = 0;
                orig::Game_SetSlot44C(0, 1);
              }
            }
          }
        }
        else
        {
          dword_1045695C = 1;
        }
        LOWORD(g_frameCounterLastSecond) = SystemTime.wSecond;
      }
    }
  }
  orig::Camera_ApplyViewProjection();
  v86 = *((_DWORD *)self_ + 206);
  v87 = *((_DWORD *)self_ + 202);
  *((_DWORD *)self_ + 199) = *((_DWORD *)self_ + 205);
  *((_DWORD *)self_ + 200) = v86;
  *((_DWORD *)self_ + 201) = v87;
  if ( (unsigned __int8)g_screenFadeLevel > 0x80u )
  {
    v88 = *((unsigned __int8 *)self_ + 806);
    a4 = *((unsigned __int8 *)self_ + 805) + (unsigned __int8)g_screenFadeLevel - 128;
    v89 = v88 + (unsigned __int8)g_screenFadeLevel - 128;
    v90 = *((unsigned __int8 *)self_ + 804) + (unsigned __int8)g_screenFadeLevel - 128;
    if ( v89 > 0xFF )
      v89 = (unsigned int)(((uint32_t)(v89) & 0xFFFFFF00u) | ((uint32_t)(uint8_t)(-1) << 0));
    if ( (unsigned int)a4 <= 0xFF )
      v91 = a4;
    else
      v91 = -1;
    if ( v90 > 0xFF )
      v90 = (unsigned int)(((uint32_t)(v90) & 0xFFFFFF00u) | ((uint32_t)(uint8_t)(-1) << 0));
    HIWORD(v98) = (unsigned __int8)v89;
    BYTE1(v98) = v91;
    LOBYTE(v98) = v90;
    *((_DWORD *)self_ + 201) = v98;
  }
  result = (int)g_pCDx;
  if ( (*(unsigned __int8 *)((char *)g_pCDx + 0x915)) != 0 )
    orig::GammaRamp_ApplyIfChanged((int)((unsigned __int8 *)((char *)g_pCDx + 0x1F4)));
  result = (int)(((uint32_t)(result) & 0xFFFFFF00u) | ((uint32_t)(uint8_t)(*((_BYTE *)self_ + 10)) << 0));
  return result;
}
