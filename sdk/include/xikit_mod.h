/* xikit_mod.h -- the interface between xikit.dll and a mod DLL (docs/SDK.md). C, so a mod may be C or C++.

   A mod is a 32-bit DLL in xikit.dll's folder, listed in xikit.ini (mods=name.dll,...). The kit loads it once the
   game has made its Direct3D device and before the kit's first frame, and calls its xikit_mod_init with an
   XikitHost. In xikit_mod_init (only there) the mod may hook functions of the kit's manifest (the 774 whose entries
   the kit has checked): the game's entry then jumps to the mod's replacement, which must have the original's
   calling convention and parameters (C++: sdk/include/xikit_sdk.hpp checks that at compile time). */
#ifndef XIKIT_MOD_H
#define XIKIT_MOD_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#define XIKIT_API_VERSION 1

/* hook() results */
#define XIKIT_OK              0
#define XIKIT_E_UNKNOWN      -1   /* no function of that name / address in the manifest */
#define XIKIT_E_TAKEN        -2   /* another mod already hooks it */
#define XIKIT_E_REFUSED      -3   /* the live entry is not the analysed build's, or speedfix_hook patches it */
#define XIKIT_E_LATE         -4   /* hook() called outside xikit_mod_init */
#define XIKIT_E_NOPREV       -5   /* no way to reach the previous code (no trampoline and no reconstruction export) */

typedef struct XikitHost {
  uint32_t version;        /* XIKIT_API_VERSION of the kit */
  uint32_t size;           /* sizeof(XikitHost) as the kit knows it; fields are only ever added at the end */
  uintptr_t game_base;     /* where FFXiMain.dll is loaded */
  intptr_t rel;            /* game_base - 0x10000000: add it to an analysed address (the tree's XI_GAME does) */
  void *device;            /* the game's IDirect3DDevice8 */
  void *recon;             /* gfx_recon_ingame.dll (HMODULE) */
  const char *dir;         /* the folder of xikit.dll and the mods, no trailing backslash */
  /* a line in xikit.log, prefixed with the mod's name */
  void (__cdecl *log)(const char *fmt, ...);
  /* Replace a manifest function (its name as xikit_manifest.txt gives it, e.g. "CDx::DrawFadeQuad", its export
     symbol, or its analysed address as "0x1000A820"). *previous (may be NULL) receives code that runs the game's
     own function: a trampoline of its first instructions where the manifest has one (T=), else the reconstruction's
     export of it. Returns XIKIT_OK or an XIKIT_E_ code (also logged). */
  int (__cdecl *hook)(const char *function, void *replacement, void **previous);
  /* the reconstruction's export of a manifest function (gate-verified C++ of the same function), or NULL */
  void *(__cdecl *reconstruction)(const char *function);
} XikitHost;

/* what a mod exports (C names, cdecl; with MinGW link with -Wl,--kill-at or none is needed for cdecl):
     int  xikit_mod_init(const XikitHost *host);     required: 0 = ok; anything else unloads nothing but is logged
     void xikit_mod_frame(void *device);             optional: each frame, on the render thread, before Present
                                                     (after the game's EndScene: wrap draws in BeginScene/EndScene)
     uintptr_t xi_rel;                               optional data export: set to host->rel before init (the tree
                                                     headers' XI_GAME / orig:: / globals use it; xikit_sdk.hpp
                                                     defines it) */
typedef int (__cdecl *xikit_mod_init_fn)(const XikitHost *host);
typedef void (__cdecl *xikit_mod_frame_fn)(void *device);

#ifdef __cplusplus
}
#endif
#endif
