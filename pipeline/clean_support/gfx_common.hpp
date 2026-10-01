// gfx_common.hpp -- shared by every file of the reconstructed graphics code.
#pragma once
#include "xi_decomp.hpp"      // helper types/macros the decompiled bodies use (_DWORD, LOBYTE, ...)
#include "xi_d3d8.hpp"        // Direct3D 8 enums + interfaces (dev->SetRenderState(...))
#include "gfx_types.hpp"      // game types from the IDB not defined as classes here
#include "xi_compat_win.hpp"  // Windows handle typedefs used in signatures
#ifndef XI_API
#define XI_API
#endif
#define XI_OFFSETOF(T, m) __builtin_offsetof(T, m)
// a field at a byte offset that has no member of its own (a second width at a known offset)
template <class T, class O> static inline T &at(O *obj, uint32_t off)
{ return *reinterpret_cast<T *>(reinterpret_cast<uint8_t *>(obj) + off); }
