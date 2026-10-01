// xi_common.hpp -- shared helpers for the FFXiMain.dll (2026-09-03) reconstruction.
#pragma once
#include <stdint.h>
#include <stddef.h>

// XI_API exports classes/functions from the verification DLL; empty in other builds.
#ifndef XI_API
#define XI_API
#endif
#define EXPORT extern "C" XI_API
#if defined(_MSC_VER)
#define THISCALL __thiscall
#else
#define THISCALL __attribute__((thiscall))
#endif
// offsetof on classes with base classes is conditionally supported; GCC and MSVC accept it.
#define XI_OFFSETOF(T, m) __builtin_offsetof(T, m)

// Typed access to an absolute address in the game image (globals, pointers read from memory).
template <class T> static inline T& M(uint32_t va) { return *reinterpret_cast<T*>(static_cast<uintptr_t>(va)); }
// Typed access to an object field that is not (yet) a named member, e.g. a second width at a
// known offset.
template <class T, class O> static inline T& at(O* obj, uint32_t off)
{ return *reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(obj) + off); }
template <class O> static inline uint32_t xi_addr(O* obj) { return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(obj)); }
// Reinterpret a stack argument's bits as float (arguments are passed as raw 32-bit words).
static inline long double FA(uint32_t bits) { union { uint32_t u; float f; } x; x.u = bits; return x.f; }
