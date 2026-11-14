// FORK OF: Post_CaptureSceneToTarget 0x10038A80 (from tree/src/unit_10038930.cpp)
// Edit the body below; `fork.py diff` shows the edit, `fork.py gate` runs it against the game's code.
// Hook it from the mod's xikit_mod_init with XI_FORK(host, Post_CaptureSceneToTarget).
#include "gfx_all.hpp"
#include "xi_ghidra.hpp"     // the Ghidra decompiler's types and helpers (float10, NAN, ...)

// FUNCTION: FFXIMAIN 0x10038A80  (unit unit_10038930: no unit evidence of its own; in the gap between 0x10038920 (CMoResource_VT_1032C7A4) and 0x10039470 (CYyObject_VT_1032A9F8) -- an unnamed source file)
// GATE: PASS -- 100 trials, 100 returned normally, 1022 D3D calls compared (diffemu_gfx, clean build); gates: fuzz PASS 100
extern "C" { __attribute__((weak)) volatile unsigned xi_aura_size; }   // (defined in Post_CreateScreenEffectTargets.fork.cpp; weak: the gate links this fork alone, where it is 0)
XI_API int __thiscall Post_CaptureSceneToTarget(void *self_)
{
  int result;
  unsigned __int8 v3;
  int v4[28];
  result = orig::Settings_GetValue(200);
  if ( result != 0
    && (result = *((_DWORD *)self_ + 1)) != 0
    && (result = g_sceneCaptureTex, g_sceneCaptureTex != 0)
    && (result = (int)pTexture, pTexture != nullptr) )
  {
    memset(v4, 0, 12);
    v4[4] = g_sceneCaptureTint;
    v4[11] = g_sceneCaptureTint;
    v4[18] = g_sceneCaptureTint;
    v4[25] = g_sceneCaptureTint;
    v4[3] = 1065353216;
    v4[5] = 0;
    v4[6] = 0;
    v4[7] = 1132462080;
    v4[8] = 0;
    v4[9] = 0;
    v4[10] = 1065353216;
    v4[12] = 1065353216;
    v4[13] = 0;
    v4[14] = 0;
    v4[15] = 1132462080;
    v4[16] = 0;
    v4[17] = 1065353216;
    v4[19] = 0;
    v4[20] = 1065353216;
    v4[21] = 1132462080;
    v4[22] = 1132462080;
    v4[23] = 0;
    v4[24] = 1065353216;
    v4[26] = 1065353216;
    v4[27] = 1065353216;
    (g_pCDx->pDevice)->lpVtbl->SetRenderState((g_pCDx->pDevice), D3DRS_ZWRITEENABLE, FALSE);
    (g_pCDx->pDevice)->lpVtbl->SetRenderState((g_pCDx->pDevice), D3DRS_ZENABLE, D3DZB_FALSE);
    (g_pCDx->pDevice)->lpVtbl->SetTextureStageState((g_pCDx->pDevice), 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
    (g_pCDx->pDevice)->lpVtbl->SetTextureStageState((g_pCDx->pDevice), 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    (g_pCDx->pDevice)->lpVtbl->SetTextureStageState((g_pCDx->pDevice), 0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    (g_pCDx->pDevice)->lpVtbl->SetTextureStageState((g_pCDx->pDevice), 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    (g_pCDx->pDevice)->lpVtbl->SetTextureStageState((g_pCDx->pDevice), 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
    (g_pCDx->pDevice)->lpVtbl->SetTextureStageState((g_pCDx->pDevice), 1u, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    (g_pCDx->pDevice)->lpVtbl->SetTextureStageState((g_pCDx->pDevice), 1u, D3DTSS_COLOROP, D3DTOP_DISABLE);
    if ( (g_sceneCaptureTint & 0xFF000000) != 0x80000000 )
    {
      (g_pCDx->pDevice)->lpVtbl->SetTextureStageState((g_pCDx->pDevice), 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE2X);
      (g_pCDx->pDevice)->lpVtbl->SetRenderState((g_pCDx->pDevice), D3DRS_ALPHABLENDENABLE, TRUE);
      (g_pCDx->pDevice)->lpVtbl->SetRenderState((g_pCDx->pDevice), D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
      (g_pCDx->pDevice)->lpVtbl->SetRenderState((g_pCDx->pDevice), D3DRS_DESTBLEND, D3DBLEND_ZERO);
    }
    (g_pCDx->pDevice)->lpVtbl->SetVertexShader((g_pCDx->pDevice), 0x144u);
    v3 = (*(unsigned __int8 *)((char *)g_pCDx + 0x828));
    (*(unsigned __int8 *)((char *)g_pCDx + 0x828)) = 0;
    if ( orig::CEnv_IsStereoMode(g_pEnv) != 0 && g_sceneCaptureTexEye2 != 0 && g_translucentTargetEye2 != 0 )
    {
      (g_pCDx->pDevice)->lpVtbl->SetTexture((g_pCDx->pDevice), 0, (IDirect3DBaseTexture8 *)g_translucentTargetEye2);
      orig::CDx_PushRenderTarget(g_pCDx, g_sceneCaptureTexEye2, g_sceneCaptureDepthEye2, 0);
      orig::CDx_DrawPrimitiveUP(g_pCDx, 5, 2, (int)v4, 28);
      orig::CDx_SetTexture(g_pCDx, 0, 0);
    }
    (g_pCDx->pDevice)->lpVtbl->SetTexture((g_pCDx->pDevice), 0, (IDirect3DBaseTexture8 *)pTexture);
    orig::CDx_PushRenderTarget(g_pCDx, g_sceneCaptureTex, g_sceneCaptureDepth, 0);
    {                                                   // aura_size: the quad at the texture's own size
      float sz = 256.0f;
      if ( xi_aura_size != 0 )
      {
        uint32_t dd[8];                                 // D3DSURFACE_DESC (d3d8.h): Format Type Usage Pool Size MultiSampleType Width Height
        IDirect3DTexture8 *t = (IDirect3DTexture8 *)(uintptr_t)(unsigned)g_sceneCaptureTex;
        if ( t->lpVtbl->GetLevelDesc(t, 0, dd) >= 0 && dd[6] >= 256 && dd[6] <= 4096 )
          sz = (float)dd[6];
      }
      float *q = (float *)v4;                           // the strip's 4 vertices of 7 dwords: x y z rhw diffuse u v
      q[7] = sz; q[15] = sz; q[21] = sz; q[22] = sz;
    }
    orig::CDx_DrawPrimitiveUP(g_pCDx, 5, 2, (int)v4, 28);
    orig::CDx_SetTexture(g_pCDx, 0, 0);
    (*(unsigned __int8 *)((char *)g_pCDx + 0x828)) = v3;
    (g_pCDx->pDevice)->lpVtbl->SetRenderState((g_pCDx->pDevice), D3DRS_ZWRITEENABLE, TRUE);
    result = (g_pCDx->pDevice)->lpVtbl->SetRenderState((g_pCDx->pDevice), D3DRS_ZENABLE, D3DZB_TRUE);
    result = (int)(((uint32_t)(result) & 0xFFFFFF00u) | ((uint32_t)(uint8_t)(1) << 0));
  }
  else
  {
    result = (int)(((uint32_t)(result) & 0xFFFFFF00u) | ((uint32_t)(uint8_t)(0) << 0));
  }
  return result;
}
