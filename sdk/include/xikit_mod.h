/* xikit_mod.h -- the interface between xikit.dll and a mod DLL (docs/SDK.md). C, so a mod may be C or C++.

   A mod is a 32-bit DLL in xikit.dll's folder, listed in xikit.ini (mods=name.dll,...). The kit loads it once the
   game has made its Direct3D device and before the kit's first frame, and calls its xikit_mod_init with an
   XikitHost. In xikit_mod_init (only there) the mod may hook functions of the kit's manifest (the 774 whose entries
   the kit has checked): the game's entry then jumps to the mod's replacement, which must have the original's
   calling convention and parameters (C++: sdk/include/xikit_sdk.hpp checks that at compile time). It may also hook
   methods of the game's Direct3D 8 device (device_hook, version 2). With mod_reload=1 in xikit.ini the kit loads a
   rebuilt mod again while the game runs: its hooks are taken off, xikit_mod_unload (if exported) is called, and the
   new build's xikit_mod_init runs (docs/SDK.md, "Reloading"). */
#ifndef XIKIT_MOD_H
#define XIKIT_MOD_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#define XIKIT_API_VERSION 3      /* 2: device_hook; 3: hook_keeping_registers */

/* hook() results */
#define XIKIT_OK              0
#define XIKIT_E_UNKNOWN      -1   /* no function of that name / address in the manifest */
#define XIKIT_E_TAKEN        -2   /* another mod already hooks it */
#define XIKIT_E_REFUSED      -3   /* the live entry is not the analysed build's, or speedfix_hook patches it */
#define XIKIT_E_LATE         -4   /* hook() called outside xikit_mod_init */
#define XIKIT_E_NOPREV       -5   /* no way to reach the previous code (no trampoline and no reconstruction export) */
#define XIKIT_E_OLDKIT       -6   /* the kit is older than the call (host->size does not reach the field) */

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
  /* --- version 2 --- */
  /* Intercept a method of the game's IDirect3DDevice8: `method` is its name in the DirectX 8 SDK's d3d8.h and `slot`
     its index in the vtable (the order of d3d8.h; xikit_sdk.hpp's XI_DEVICE_HOOK computes both from the tree's
     xi_d3d8.hpp). The kit refuses the hook when the two disagree with its own table. The replacement is __stdcall
     with the device first, as the vtable entry; *previous receives code that goes on to the next layer (another
     mod's hook of the same method, then the kit's profiler, then Direct3D). Several mods may hook one method; the
     one hooked last runs first. Only in xikit_mod_init. Calls the game makes from other threads run the hook too. */
  int (__cdecl *device_hook)(const char *method, int slot, void *replacement, void **previous);
  /* --- version 3 --- */
  /* hook(), also for a function of manifest flag 8 (the game's code reads or hands back a register its declaration
     does not carry, e.g. bytes of its entry EAX that callers read), which hook() refuses. The mod states that its
     replacement leaves every register as the game's code would, typically by calling *previous and keeping what
     it leaves. So *previous is always the game's own code here: without a trampoline the call is refused
     (XIKIT_E_NOPREV), never answered with the reconstruction. Only in xikit_mod_init. */
  int (__cdecl *hook_keeping_registers)(const char *function, void *replacement, void **previous);
} XikitHost;

/* what a mod exports (C names, cdecl; with MinGW link with -Wl,--kill-at or none is needed for cdecl):
     int  xikit_mod_init(const XikitHost *host);     required: 0 = ok; anything else unloads nothing but is logged
     void xikit_mod_frame(void *device);             optional: each frame, on the render thread, before Present
                                                     (after the game's EndScene: wrap draws in BeginScene/EndScene)
     void xikit_mod_unload(void);                    optional: before a reload replaces this build (mod_reload=1):
                                                     release what the mod created (textures, threads); its hooks are
                                                     already off. The old build stays loaded (code on a stack may
                                                     still return into it) but is never called again
     uintptr_t xi_rel;                               optional data export: set to host->rel before init (the tree
                                                     headers' XI_GAME / orig:: / globals use it; xikit_sdk.hpp
                                                     defines it) */
typedef int (__cdecl *xikit_mod_init_fn)(const XikitHost *host);
typedef void (__cdecl *xikit_mod_frame_fn)(void *device);
typedef void (__cdecl *xikit_mod_unload_fn)(void);

#ifdef __cplusplus
}
#endif
#endif
