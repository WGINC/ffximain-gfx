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
//
// Device hooks (kit API 2): XI_DEVICE_HOOK(host, SetRenderState, my_SetRenderState, prev) intercepts a method of the
// game's IDirect3DDevice8; the replacement has the vtable entry's type, XI_DEVICE_FN(SetRenderState), i.e.
//   static XI_DEVICE_FN(SetRenderState) prev;
//   static int __stdcall my_SetRenderState(IDirect3DDevice8 *dev, D3DRENDERSTATETYPE s, unsigned v) { ...; return prev(dev, s, v); }
// Forks (sdk/fork.py): XI_FORK(host, Class::Method) hooks the function with the mod's own definition of it.
#pragma once
#ifndef XI_INGAME
#define XI_INGAME 1            // game addresses rebased by xi_rel (tree/include/xi_decomp.hpp)
#endif
#include <stdint.h>
#include "xikit_mod.h"
#include "xikit_game.hpp"

#include <type_traits>
#include <stddef.h>
#include <string.h>

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
template <class F, class R> inline int device_hook(const XikitHost *host, const char *method, int slot, R replacement, F *previous) {
  static_assert(std::is_same<F, R>::value, "XI_DEVICE_HOOK: the replacement's type is not the method's vtable entry "
                "(__stdcall, the device first, then the parameters of d3d8.h); it must be XI_DEVICE_FN(method)");
  if (host->size < offsetof(XikitHost, device_hook) + sizeof(void *)) { host->log("device_hook(%s): the kit is older than API 2", method); return XIKIT_E_OLDKIT; }
  void *p = nullptr;
  int r = host->device_hook(method, slot, reinterpret_cast<void *>(replacement), &p);
  if (previous) *previous = reinterpret_cast<F>(p);
  return r;
}
// the code address of a function the mod defines (a fork): a free or static function's pointer, or a non-virtual
// member function's. A pointer to member function is {address, this adjustment} in the Itanium C++ ABI that MinGW's
// g++ follows, the address odd for a virtual one (then a vtable offset + 1, not code): refused, as is an adjustment
template <class P> inline void *code_of(P p) {
  if constexpr (std::is_member_function_pointer<P>::value) {
    static_assert(sizeof(P) == 2 * sizeof(void *), "a pointer to member function of the Itanium ABI (MinGW g++)");
    uintptr_t w[2]; memcpy(w, &p, sizeof w);
    return (w[0] & 1) || w[1] ? nullptr : reinterpret_cast<void *>(w[0]);
  } else return reinterpret_cast<void *>(p);
}
// an analysed address (as IDA / the tree show it, base 0x10000000) where the game is loaded now
template <class T = void> inline T *game(uint32_t analysed) { return reinterpret_cast<T *>(XI_GAME(analysed)); }
}

// Hook `function` (as the tree names it: Class::Method or FreeFunction) with `replacement`; `previous` (a variable of
// type xikit::fn<decltype(&function)>) receives the code that runs the game's own function. Call it from
// xikit_mod_init only. Evaluates to XIKIT_OK (0) or an XIKIT_E_ code (xikit_mod.h; the kit logs why).
#define XI_HOOK(host, function, replacement, previous) \
  (::xikit::hook< ::xikit::fn<decltype(&function)> >((host), #function, (replacement), &(previous)))

// the type of an IDirect3DDevice8 vtable entry, as the tree's xi_d3d8.hpp declares it (the order of d3d8.h)
#define XI_DEVICE_FN(method) decltype(IDirect3DDevice8Vtbl::method)
// Hook a device method; `previous` (of type XI_DEVICE_FN(method)) receives what goes on to the next layer.
// The slot is the entry's place in the vtable; the kit checks it against its own table of d3d8.h's names.
#define XI_DEVICE_HOOK(host, method, replacement, previous) \
  (::xikit::device_hook< XI_DEVICE_FN(method) >((host), #method, (int)(offsetof(IDirect3DDevice8Vtbl, method) / sizeof(void *)), \
                                               (replacement), &(previous)))
// Hook `function` with the mod's own definition of it: a fork (sdk/fork.py) compiled into the mod. Its previous is
// not kept: a fork is the whole function. Evaluates to XIKIT_OK or an XIKIT_E_ code.
#define XI_FORK(host, function) \
  (::xikit::code_of(&function) ? (host)->hook(#function, ::xikit::code_of(&function), nullptr) \
                               : ((host)->log("XI_FORK(%s): a virtual member or one with a this adjustment", #function), XIKIT_E_UNKNOWN))

// the data the tree's headers expect the module to define: xi_rel (the load delta) and xi_shift (another build's
// moved code ranges); xikit sets both before init
#define XIKIT_MOD_GLOBALS extern "C" { __declspec(dllexport) uintptr_t xi_rel = 0; __declspec(dllexport) int32_t xi_shift[25] = {0}; }
