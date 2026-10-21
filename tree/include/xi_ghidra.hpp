// xi_ghidra.hpp -- the types and helper operations Ghidra's decompiler output uses, for translating
// Ghidra decompilations (pipeline/ghidra/GfxExport.java) with the same pipeline as Hex-Rays output.
#pragma once
#include <stdint.h>
#include "xi_ghidra_shared.hpp"  // undefinedN, undefined4, XI_ANY (also in the clean tree's shared headers)

typedef unsigned char byte; typedef unsigned char uchar; typedef unsigned short ushort; typedef unsigned int uint;
typedef unsigned long ulong; typedef long long longlong; typedef unsigned long long ulonglong;
typedef unsigned short word; typedef uint32_t dword; typedef uint64_t qword;
typedef unsigned char xi_bool;               // Ghidra's bool: one byte, any value (hx2cpp.ghidra_bool)
typedef long double float10;                 // x87 80-bit; GCC's long double on i686
typedef unsigned short wchar16; typedef unsigned int wchar32;
typedef int32_t int3; typedef uint32_t uint3; typedef int64_t int5; typedef uint64_t uint5; typedef int64_t int6; typedef uint64_t uint6; typedef int64_t int7; typedef uint64_t uint7;   // Ghidra's odd-width integers
typedef int code(...);                       // `(*(code *)p)(...)`: a call through an untyped pointer
typedef void *pointer; typedef void *addr;

// piece / subpiece / extension: Ghidra's names, sizes in bytes
#define CONCAT11(h, l) ((uint16_t)(((uint16_t)(uint8_t)(h) << 8) | (uint8_t)(l)))
#define CONCAT12(h, l) ((uint32_t)(((uint32_t)(uint8_t)(h) << 16) | (uint16_t)(l)))
#define CONCAT13(h, l) ((uint32_t)(((uint32_t)(uint8_t)(h) << 24) | ((uint32_t)(l) & 0xFFFFFFu)))
#define CONCAT22(h, l) ((uint32_t)(((uint32_t)(uint16_t)(h) << 16) | (uint16_t)(l)))
#define CONCAT31(h, l) ((uint32_t)(((uint32_t)(h) << 8) | (uint8_t)(l)))
#define CONCAT44(h, l) ((uint64_t)(((uint64_t)(uint32_t)(h) << 32) | (uint32_t)(l)))
#define CONCAT26(h, l) ((uint64_t)(((uint64_t)(uint16_t)(h) << 48) | ((uint64_t)(l) & 0xFFFFFFFFFFFFull)))
#define CONCAT17(h, l) ((uint64_t)(((uint64_t)(uint8_t)(h) << 56) | ((uint64_t)(l) & 0xFFFFFFFFFFFFFFull)))
// p-code CAST between a floating and a non-floating type of one size: the same bits (GfxExport `/*XI_BC*/`)
template <class To, class From> static inline To xi_castbits(From f) { static_assert(sizeof(To) == sizeof(From), "xi_castbits: one size"); To t; __builtin_memcpy(&t, &f, sizeof(t)); return t; }
// SUBnm is p-code SUBPIECE, the bytes of x: a floating x gives its encoding, not its value converted
template <class T> static inline uint32_t xi_b32(T x) { return (uint32_t)x; }
static inline uint32_t xi_b32(float x) { uint32_t u; __builtin_memcpy(&u, &x, 4); return u; }
template <class T> static inline uint64_t xi_b64(T x) { return (uint64_t)x; }
static inline uint64_t xi_b64(double x) { uint64_t u; __builtin_memcpy(&u, &x, 8); return u; }
#define SUB41(x, b) ((uint8_t)(xi_b32(x) >> (8 * (b))))
#define SUB42(x, b) ((uint16_t)(xi_b32(x) >> (8 * (b))))
#define SUB21(x, b) ((uint8_t)((uint16_t)(x) >> (8 * (b))))
#define SUB81(x, b) ((uint8_t)(xi_b64(x) >> (8 * (b))))
#define SUB84(x, b) ((uint32_t)(xi_b64(x) >> (8 * (b))))
#define SUB82(x, b) ((uint16_t)(xi_b64(x) >> (8 * (b))))
#define ZEXT14(x) ((uint32_t)(uint8_t)(x))
#define ZEXT24(x) ((uint32_t)(uint16_t)(x))
#define ZEXT48(x) ((uint64_t)(uint32_t)(x))
#define ZEXT12(x) ((uint16_t)(uint8_t)(x))
#define SEXT14(x) ((int32_t)(int8_t)(x))
#define SEXT24(x) ((int32_t)(int16_t)(x))
#define SEXT48(x) ((int64_t)(int32_t)(x))
#define SEXT12(x) ((int16_t)(int8_t)(x))
#define CARRY1(a, b) ((uint8_t)((uint8_t)(a) + (uint8_t)(b)) < (uint8_t)(a))
#define CARRY2(a, b) ((uint16_t)((uint16_t)(a) + (uint16_t)(b)) < (uint16_t)(a))
#define CARRY4(a, b) ((uint32_t)((uint32_t)(a) + (uint32_t)(b)) < (uint32_t)(a))
#define SCARRY4(a, b) ((((int32_t)(a) ^ (int32_t)((uint32_t)(a) + (uint32_t)(b))) & ((int32_t)(b) ^ (int32_t)((uint32_t)(a) + (uint32_t)(b)))) < 0)
#define SBORROW4(a, b) ((((int32_t)(a) ^ (int32_t)(b)) & ((int32_t)(a) ^ (int32_t)((uint32_t)(a) - (uint32_t)(b)))) < 0)
#define SBORROW2(a, b) ((((int16_t)(a) ^ (int16_t)(b)) & ((int16_t)(a) ^ (int16_t)((uint16_t)(a) - (uint16_t)(b)))) < 0)
#define SBORROW1(a, b) ((((int8_t)(a) ^ (int8_t)(b)) & ((int8_t)(a) ^ (int8_t)((uint8_t)(a) - (uint8_t)(b)))) < 0)
#define NAN_CHK(x) ((x) != (x))
#define NAN(x) ((x) != (x))
#define ROUND(x) ((long double)__builtin_rintl(x))
#define TRUNC(x) ((long double)(long long)(x))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define SQRT(x) __builtin_sqrtl(x)
#define POPCOUNT(x) __builtin_popcount(x)

// Ghidra intrinsics
static inline long double xi_fpatan(long double y, long double x) {      // x87 fpatan: atan(ST1 / ST0)
    long double r; __asm__("fpatan" : "=t"(r) : "0"(x), "u"(y) : "st(1)"); return r;
}
#define fpatan(y, x) xi_fpatan((y), (x))
static inline long double xi_fsin(long double x) { long double r; __asm__("fsin" : "=t"(r) : "0"(x)); return r; }   // x87 fsin
static inline long double xi_fcos(long double x) { long double r; __asm__("fcos" : "=t"(r) : "0"(x)); return r; }   // x87 fcos
#define fsin(x) xi_fsin((x))
#define fcos(x) xi_fcos((x))

// A call to a game function that pops a value the caller leaves in ST0 (fpret.json `consumes`: MSVC __ftol):
// the value goes in ST0, the callee pops it; EDX:EAX come back as the callee leaves them. ECX is the caller's
// to lose under every MSVC convention.
static inline long long xi_call_st0(uint32_t fn, long double x) {
    long long r; __asm__ volatile("call *%1" : "=A"(r) : "r"(fn), "t"(x) : "st", "ecx", "memory", "cc"); return r;
}


// Ghidra's piece accessor `x._O_S_`: the S bytes at byte offset O of x's storage (x86 is little-endian)
template<int S> struct xi_piece_t; template<> struct xi_piece_t<1> { typedef uint8_t t; };
template<> struct xi_piece_t<2> { typedef uint16_t t; }; template<> struct xi_piece_t<4> { typedef uint32_t t; };
template<> struct xi_piece_t<8> { typedef uint64_t t; };
// a 3-byte piece (Ghidra's undefined3, `x._1_3_`): its bytes copied in and out, read zero- or sign-extended
struct xi_u24 { uint8_t b[3]; operator uint32_t() const { uint32_t v = 0; __builtin_memcpy(&v, b, 3); return v; }
                xi_u24 &operator=(uint32_t v) { __builtin_memcpy(b, &v, 3); return *this; } };
struct xi_s24 { uint8_t b[3]; operator int32_t() const { uint32_t v = 0; __builtin_memcpy(&v, b, 3); return (int32_t)(v << 8) >> 8; }
                xi_s24 &operator=(int32_t v) { __builtin_memcpy(b, &v, 3); return *this; } };
template<> struct xi_piece_t<3> { typedef xi_u24 t; };
#define XI_PIECE(x, o, s) (*reinterpret_cast<xi_piece_t<s>::t *>(reinterpret_cast<char *>(&(x)) + (o)))
template<int S> struct xi_spiece_t; template<> struct xi_spiece_t<1> { typedef int8_t t; };
template<> struct xi_spiece_t<2> { typedef int16_t t; }; template<> struct xi_spiece_t<4> { typedef int32_t t; };
template<> struct xi_spiece_t<8> { typedef int64_t t; }; template<> struct xi_spiece_t<3> { typedef xi_s24 t; };
#define XI_PIECE_S(x, o, s) (*reinterpret_cast<xi_spiece_t<s>::t *>(reinterpret_cast<char *>(&(x)) + (o)))   // a signed piece

// a floating constant by its encoding (GfxExport `/*XI_K:size:bits*/`): the float or double the code loads
static inline float xi_k4(unsigned long long b) { uint32_t u = (uint32_t)b; float f; __builtin_memcpy(&f, &u, 4); return f; }
static inline double xi_k8(unsigned long long b) { double d; __builtin_memcpy(&d, &b, 8); return d; }
