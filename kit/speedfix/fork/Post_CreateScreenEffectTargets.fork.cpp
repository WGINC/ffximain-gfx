// FORK OF: Post_CreateScreenEffectTargets 0x10038930 (from tree/src/unit_10038930.cpp)
// Edit the body below; `fork.py diff` shows the edit, `fork.py gate` runs it against the game's code.
// Hook it from the mod's xikit_mod_init with XI_FORK(host, Post_CreateScreenEffectTargets).
#include "gfx_all.hpp"
#include "xi_ghidra.hpp"     // the Ghidra decompiler's types and helpers (float10, NAN, ...)

// FUNCTION: FFXIMAIN 0x10038930  (unit unit_10038930: no unit evidence of its own; in the gap between 0x10038920 (CMoResource_VT_1032C7A4) and 0x10039470 (CYyObject_VT_1032A9F8) -- an unnamed source file)
// GATE: PASS -- 100 trials, 100 returned normally, 78 D3D calls compared (diffemu_gfx, clean build); gates: fuzz PASS 100
// ---- aura_size (14 Nov; the fix of dagnyfix's aura port of Krauerlabs's SpectralFix, GPL-3.0-only, made at its source).
// Actor auras are drawn from g_sceneCaptureTex: Post_CaptureSceneToTarget (0x10038A80) copies the scene into it as a
// 256 x 256 quad, Post_DrawRotatingGhostLayers (0x10038E10) draws it back as rotated copies at the viewport's size, so
// the aura is a 256-pixel picture blown up (jagged). SpectralFix enlarges that texture (its log in the owner's game:
// caller 0xA73A, CDx::CreateTexture's device call, candidate 1 of 2 256 x 256 targets, enlarged to 2048) and redraws
// the copies; here the texture is made xi_aura_size square and the capture's quad that size (its fork), which is all
// the game's code ties to 256: the target is bound with no depth surface and the viewport from its own size
// (CDx::PushRenderTargetTexture 0x10009F20), and the ghost copies are placed from the viewport. The stereo twin
// (g_sceneCaptureTexEye2) stays 256, as in SpectralFix. speedfix sets xi_aura_size before the device is made; 0 (the
// gate) is the game's 256.
extern "C" { volatile unsigned xi_aura_size; }
XI_API _DWORD * __cdecl Post_CreateScreenEffectTargets(void)
{
  _DWORD *result;
  g_sceneCaptureDepth = 0;
  unsigned short n = xi_aura_size >= 0x100u && xi_aura_size <= 0x1000u ? (unsigned short)xi_aura_size : 0x100u;
  g_sceneCaptureTex = (int)orig::CDx_CreateTexture(g_pCDx, n, n, &g_sceneCaptureDepth, 1, 0);
  if ( g_sceneCaptureTex == 0 && n != 0x100u )        // not made at that size: the game's
    g_sceneCaptureTex = (int)orig::CDx_CreateTexture(g_pCDx, 0x100u, 0x100u, &g_sceneCaptureDepth, 1, 0);
  result = g_pEnv;
  if ( g_pEnv[6] != 0 )
  {
    g_sceneCaptureDepthEye2 = g_translucentDepth;
    if ( g_translucentDepth != 0 )
      (*(void (__stdcall **)(_DWORD))(*(_DWORD *)g_translucentDepth + 4))(g_translucentDepth);
    result = orig::CDx_CreateTexture(g_pCDx, 0x100u, 0x100u, &g_sceneCaptureDepthEye2, 1, 0);
    g_sceneCaptureTexEye2 = (int)result;
  }
  else
  {
    g_sceneCaptureTexEye2 = 0;
    g_sceneCaptureDepthEye2 = 0;
  }
  return result;
}
