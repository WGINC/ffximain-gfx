// my_mod.cpp -- a starting point for a mod (docs/SDK.md). Build:  sdk/build_mod.sh my_mod.dll my_mod.cpp
// Install: put my_mod.dll next to xikit.dll and add it to xikit.ini:  mods=my_mod.dll
#include "xikit_sdk.hpp"
XIKIT_MOD_GLOBALS                       // once per mod

static const XikitHost *g_host;

// 1. Pick a function from sdk/include/xikit_game.hpp (namespace xihook lists the ones a mod may hook) and look up
//    its declaration in tree/include (the class header, or gfx_free.hpp for a free function). Its reconstructed
//    body, gate-verified, is in tree/src: read it to see what it does before you change it.
// 2. Declare `previous` with its type and write the replacement with the same signature. A member function becomes
//    a __thiscall function taking the object first. XI_HOOK refuses to compile if the types differ.
//
// static xikit::fn<decltype(&CDx::DrawFadeQuad)> prev_DrawFadeQuad;
// static int __thiscall my_DrawFadeQuad(CDx *self, int a1, int a2) {
//   // ... before ...
//   int r = prev_DrawFadeQuad(self, a1, a2);       // the game's own function (or leave it out to replace it)
//   // ... after ...
//   return r;
// }

extern "C" __declspec(dllexport) int xikit_mod_init(const XikitHost *host) {
  g_host = host;
  host->log("my_mod: hello (game at %p)", (void *)host->game_base);
  // 3. Hook it here (only here). A non-zero result means it was not hooked; xikit.log says why.
  // if (XI_HOOK(host, CDx::DrawFadeQuad, my_DrawFadeQuad, prev_DrawFadeQuad) != XIKIT_OK) return 1;
  return 0;
}

// Optional: called every frame on the render thread, just before Present (after the game's EndScene).
// extern "C" __declspec(dllexport) void xikit_mod_frame(void *device) { }
