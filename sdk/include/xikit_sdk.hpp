// xikit_sdk.hpp -- the one header a C++ mod includes (docs/SDK.md).
//
//   #include "xikit_sdk.hpp"
//   XIKIT_MOD_GLOBALS                       // in exactly one source file of the mod
//
//   static xikit::fn<decltype(&CDx::DrawFadeQuad)> prev_fade;            // the game's own function
//   static int __thiscall my_fade(CDx *self, int a1, int a2) { ...; return prev_fade(self, a1, a2); }
//
//   extern "C" __declspec(dllexport) int xikit_mod_init(const XikitHost *host) {
//     return XI_HOOK(host, CDx::DrawFadeQuad, my_fade, prev_fade);        // 0 = hooked
//   }
//
// XI_HOOK checks at compile time that the replacement has the type of the tree's declaration of the function
// (calling convention, `this`, parameters, return), which is the declaration the gate verified the reconstruction
// with. Everything the tree declares is visible: its classes with field offsets (static_assert-checked), free
// functions, orig:: (the game's own code by address), the game's globals (gfx_globals.hpp), Direct3D 8
// (xi_d3d8.hpp), and xigame::Name() (a typed pointer to any reconstructed function's game entry).
#pragma once
#ifndef XI_INGAME
#define XI_INGAME 1            // game addresses rebased by xi_rel (tree/include/xi_decomp.hpp)
#endif
#include <stdint.h>
#include "xikit_mod.h"
#include "xikit_game.hpp"

#include <type_traits>

namespace xikit {
template <class F, class R> inline int hook(const XikitHost *host, const char *name, R replacement, F *previous) {
  // (a static_assert, not a conversion: the tree's headers need -fpermissive, which would let a wrong type through)
  static_assert(std::is_same<F, R>::value, "XI_HOOK: the replacement's type is not the function's (calling convention, "
                "this, parameters, return); it must be xikit::fn<decltype(&function)>");
  void *p = nullptr;
  int r = host->hook(name, reinterpret_cast<void *>(replacement), &p);
  if (previous) *previous = reinterpret_cast<F>(p);
  return r;
}
// an analysed address (as IDA / the tree show it, base 0x10000000) where the game is loaded now
template <class T = void> inline T *game(uint32_t analysed) { return reinterpret_cast<T *>(XI_GAME(analysed)); }
}

// Hook `function` (as the tree names it: Class::Method or FreeFunction) with `replacement`; `previous` (a variable of
// type xikit::fn<decltype(&function)>) receives the code that runs the game's own function. Call it from
// xikit_mod_init only. Evaluates to XIKIT_OK (0) or an XIKIT_E_ code (xikit_mod.h; the kit logs why).
#define XI_HOOK(host, function, replacement, previous) \
  (::xikit::hook< ::xikit::fn<decltype(&function)> >((host), #function, (replacement), &(previous)))

// the data the tree's headers expect the module to define: xi_rel (the load delta; xikit sets it before init)
#define XIKIT_MOD_GLOBALS extern "C" { __declspec(dllexport) uintptr_t xi_rel = 0; }
