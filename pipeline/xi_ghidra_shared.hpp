// xi_ghidra_shared.hpp -- the part of xi_ghidra.hpp the clean tree's shared headers (gfx_common.hpp) also need: Ghidra's
// undefinedN types and XI_ANY. These names occur in no Hex-Rays output, so every translation unit can see them.
#pragma once
#include <stdint.h>

typedef unsigned char undefined; typedef unsigned char undefined1; typedef uint16_t undefined2;
typedef uint32_t undefined3; typedef uint64_t undefined5; typedef uint64_t undefined6;
typedef uint64_t undefined7; typedef uint64_t undefined8;
// Ghidra's undefined4: four bytes of no type. A value of another type copied into it keeps its bits (a float stays
// the float's encoding: Ghidra's COPY), where a C++ uint32_t would convert the float's value; integers and pointers
// convert as they would to uint32_t; reading it gives the uint32_t.
struct undefined4 {
    uint32_t v;
    undefined4() = default;
#define XI_U4_INT(t) undefined4(t x) : v((uint32_t)x) {}
    XI_U4_INT(bool) XI_U4_INT(char) XI_U4_INT(signed char) XI_U4_INT(unsigned char) XI_U4_INT(short) XI_U4_INT(unsigned short)
    XI_U4_INT(int) XI_U4_INT(unsigned) XI_U4_INT(long) XI_U4_INT(unsigned long) XI_U4_INT(long long) XI_U4_INT(unsigned long long)
#undef XI_U4_INT
    template <class T> undefined4(T *p) : v((uint32_t)(uintptr_t)p) {}
    undefined4(float f) { __builtin_memcpy(&v, &f, 4); }
    undefined4(double) = delete;             // eight bytes do not copy into four: no fact says which four
    operator uint32_t() const { return v; }
    template <class T> explicit operator T *() const { return (T *)(uintptr_t)v; }
#define XI_U4_OP(op) template <class T> undefined4 &operator op##=(T x) { v op##= x; return *this; }
    XI_U4_OP(+) XI_U4_OP(-) XI_U4_OP(*) XI_U4_OP(/) XI_U4_OP(%) XI_U4_OP(&) XI_U4_OP(|) XI_U4_OP(^) XI_U4_OP(<<) XI_U4_OP(>>)
#undef XI_U4_OP
    undefined4 &operator++() { ++v; return *this; } undefined4 &operator--() { --v; return *this; }
    uint32_t operator++(int) { return v++; } uint32_t operator--(int) { return v--; }
};

// An argument slot of a called game function: 4 bytes on the stack, holding the bits of whatever the
// caller passes (int, pointer or float), exactly as the original pushes them.
struct XI_ANY {
    uint32_t bits;
    template <class T> XI_ANY(T v) { static_assert(sizeof(T) <= 4, "XI_ANY: 4-byte argument"); bits = 0; __builtin_memcpy(&bits, &v, sizeof(T)); }
    XI_ANY(float v) { __builtin_memcpy(&bits, &v, 4); }       // a float parameter is pushed as 4 bytes
    XI_ANY(double v) { float f = (float)v; __builtin_memcpy(&bits, &f, 4); }
    XI_ANY(long double v) { float f = (float)v; __builtin_memcpy(&bits, &f, 4); }
};
