// xi_decomp.hpp -- the helper types and macros Hex-Rays pseudocode uses, so decompiled bodies
// compile as C++ (MinGW i686, matching MSVC6's 32-bit x86 ABI). Written for this project.
#pragma once
#include <stdint.h>
#include <string.h>
typedef int8_t   _BYTE_S;
typedef uint8_t  _BYTE;
typedef uint16_t _WORD;
typedef uint32_t _DWORD;
typedef uint64_t _QWORD;
typedef int8_t   _BOOL1;
typedef int32_t  _BOOL4;
typedef uint8_t  _UNKNOWN;
typedef float    _FLOAT;
#ifndef __int8
#define __int8 char
#endif
#ifndef __int16
#define __int16 short
#endif
#ifndef __int32
#define __int32 int
#endif
#define _mm_pause() __asm__ __volatile__("pause")
// XI_GAME(ea): an address in the analysed image of FFXiMain.dll (base 0x10000000). Built with XI_INGAME, it is
// rebased to where the game's DLL is loaded: xi_rel (rt.c) is that load address minus 0x10000000, set by the
// loader before any reconstructed function runs. Otherwise it is the address itself, as the gate runs the game
// at its analysed base, and the code it compiles to is the same as a plain constant's.
#ifdef XI_INGAME
extern "C" uintptr_t xi_rel;
// Another build of the game (kit/xikit/builds/<stamp>.json, pipeline/build_translate.py): the analysed build's code
// ranges that moved there, set by the kit with xi_rel. xi_shift[0] is the number of ranges, then lo, hi, shift for
// each. Data (at or above the analysed .rdata, 0x10329000) never moves in a build the kit accepts, so for a constant
// address the range test folds away and a data access costs what it did; a code address pays the short search.
extern "C" int32_t xi_shift[25];
static inline uintptr_t xi_code(uint32_t ea) {
  int32_t d = 0;
  for (int32_t i = 0; i < xi_shift[0] && i < 8; i++)
    if (ea >= static_cast<uint32_t>(xi_shift[1 + 3 * i]) && ea < static_cast<uint32_t>(xi_shift[2 + 3 * i])) d = xi_shift[3 + 3 * i];
  return static_cast<uintptr_t>(ea) + xi_rel + d;
}
#ifndef XI_PROBE
#define XI_GAME(ea) ((static_cast<uint32_t>(ea) >= 0x10001000u && static_cast<uint32_t>(ea) < 0x10329000u) \
                     ? xi_code(static_cast<uint32_t>(ea)) : static_cast<uintptr_t>(ea) + xi_rel)
#else
// the probe build (build.sh XI_PROBE=1, never run): every rebased address is moved by 0x01000000 at compile time, so
// a game-range constant (0x10001000-0x10700000) left in its code was not written through XI_GAME
// (pipeline/raw_game_addrs.py, which kit/xikit/make_manifest.py reads)
#define XI_GAME(ea) (static_cast<uintptr_t>(static_cast<uint32_t>(ea) + 0x01000000u) + xi_rel)
#endif
#else
#define XI_GAME(ea) (static_cast<uintptr_t>(ea))
#endif
#define XI_EA(t, ea) (*reinterpret_cast<t *>(XI_GAME(ea)))
#define LAST_IND(x, part_type) (sizeof(x) / sizeof(part_type) - 1)
#define LOW_IND(x, part_type) 0
#define HIGH_IND(x, part_type) LAST_IND(x, part_type)
#define BYTEn(x, n)  (*((_BYTE *)&(x) + (n)))
#define WORDn(x, n)  (*((_WORD *)&(x) + (n)))
#define DWORDn(x, n) (*((_DWORD *)&(x) + (n)))
#define LOBYTE(x)  BYTEn(x, 0)
#define LOWORD(x)  WORDn(x, 0)
#define LODWORD(x) DWORDn(x, 0)
#define HIBYTE(x)  BYTEn(x, sizeof(x) - 1)
#define HIWORD(x)  WORDn(x, sizeof(x) / 2 - 1)
#define HIDWORD(x) DWORDn(x, 1)
#define BYTE1(x) BYTEn(x, 1)
#define BYTE2(x) BYTEn(x, 2)
#define BYTE3(x) BYTEn(x, 3)
#define BYTE4(x) BYTEn(x, 4)
#define WORD1(x) WORDn(x, 1)
#define WORD2(x) WORDn(x, 2)
#define SLOBYTE(x) (*((int8_t *)&(x)))
#define SLOWORD(x) (*((int16_t *)&(x)))
#define SHIWORD(x) (*((int16_t *)&(x) + 1))
#define SHIBYTE(x) (*((int8_t *)&(x) + sizeof(x) - 1))
#define SBYTE1(x) (*((int8_t *)&(x) + 1))
#define SLODWORD(x) (*((int32_t *)&(x)))
#define SHIDWORD(x) (*((int32_t *)&(x) + 1))
static inline uint8_t  __ROL1__(uint8_t v, int n)  { n &= 7;  return n ? (uint8_t)((v << n) | (v >> (8 - n))) : v; }
static inline uint16_t __ROL2__(uint16_t v, int n) { n &= 15; return n ? (uint16_t)((v << n) | (v >> (16 - n))) : v; }
static inline uint32_t __ROL4__(uint32_t v, int n) { n &= 31; return n ? (v << n) | (v >> (32 - n)) : v; }
static inline uint64_t __ROL8__(uint64_t v, int n) { n &= 63; return n ? (v << n) | (v >> (64 - n)) : v; }
static inline uint8_t  __ROR1__(uint8_t v, int n)  { return __ROL1__(v, -n & 7); }
static inline uint16_t __ROR2__(uint16_t v, int n) { return __ROL2__(v, -n & 15); }
static inline uint32_t __ROR4__(uint32_t v, int n) { return __ROL4__(v, -n & 31); }
static inline uint64_t __ROR8__(uint64_t v, int n) { return __ROL8__(v, -n & 63); }
#define __PAIR64__(high, low) (((uint64_t)(uint32_t)(high) << 32) | (uint32_t)(low))
#define __SPAIR64__(high, low) ((int64_t)__PAIR64__(high, low))
// sign of x as a bool (Hex-Rays: __SETS__)
template <class T> static inline bool __SETS__(T x) { return x < 0; }
// carry out of x+y
template <class T, class U> static inline bool __CFADD__(T x, U y) { return (T)(x) > (T)((T)x + (T)y); }
template <class T, class U> static inline bool __CFSUB__(T x, U y) { return (T)x < (T)y; }
template <class T, class U> static inline bool __OFSUB__(T x, U y) {
    T r = (T)(x - y); return ((x ^ y) & (x ^ r)) < 0; }
template <class T, class U> static inline bool __OFADD__(T x, U y) {
    T r = (T)(x + y); return ((~(x ^ y)) & (x ^ r)) < 0; }
#define abs32(x) ((int32_t)(x) < 0 ? -(int32_t)(x) : (int32_t)(x))
#define fabsf_(x) __builtin_fabsf(x)
// Hex-Rays prints inline `rep movs` as qmemcpy
#define qmemcpy memcpy
// x87 intrinsics as the original executes them (MSVC6 /Oi emits fsin/fcos/fsqrt/fabs/fpatan inline)
static inline double sin(double x)  { double r; __asm__("fsin" : "=t"(r) : "0"(x)); return r; }
static inline double cos(double x)  { double r; __asm__("fcos" : "=t"(r) : "0"(x)); return r; }
static inline double sqrt(double x) { double r; __asm__("fsqrt" : "=t"(r) : "0"(x)); return r; }
static inline double fabs(double x) { double r; __asm__("fabs" : "=t"(r) : "0"(x)); return r; }
static inline double atan2(double y, double x) { double r; __asm__("fpatan" : "=t"(r) : "0"(x), "u"(y) : "st(1)"); return r; }
static inline double atan(double x) { return atan2(x, 1.0); }
static inline double tan(double x)  { double r, one; __asm__("fptan" : "=t"(one), "=u"(r) : "0"(x)); (void)one; return r; }
// the same on an 80-bit value: a `long double` local (Hex-Rays: an x87 register the original never stored) keeps all its
// bits, as in the original's fsin/fcos on st(0); through the double versions it was rounded to 53 bits first, and near a
// zero of cos the sign of the result came out different (Post_DrawRotatingGhostLayers 0x10038E10: -0.0 vs +0.0, 6 Oct)
static inline long double sin(long double x)  { long double r; __asm__("fsin" : "=t"(r) : "0"(x)); return r; }
static inline long double cos(long double x)  { long double r; __asm__("fcos" : "=t"(r) : "0"(x)); return r; }
static inline long double sqrt(long double x) { long double r; __asm__("fsqrt" : "=t"(r) : "0"(x)); return r; }
static inline long double fabs(long double x) { long double r; __asm__("fabs" : "=t"(r) : "0"(x)); return r; }
static inline long double atan2(long double y, long double x) { long double r; __asm__("fpatan" : "=t"(r) : "0"(x), "u"(y) : "st(1)"); return r; }
static inline long double tan(long double x)  { long double r, one; __asm__("fptan" : "=t"(one), "=u"(r) : "0"(x)); (void)one; return r; }
// Hex-Rays bit reinterpretation helpers
template <class To, class From> static inline To xi_bitcast(From f) { To t; memcpy(&t, &f, sizeof(t)); return t; }
#define COERCE_FLOAT(v)    xi_bitcast<float>((uint32_t)(v))
#define COERCE_INT(v)      xi_bitcast<int32_t>(v)
#define COERCE_UNSIGNED_INT(v) xi_bitcast<uint32_t>(v)
#define COERCE_DOUBLE(v)   xi_bitcast<double>((uint64_t)(v))
#define COERCE_UNSIGNED_INT64(v) xi_bitcast<uint64_t>(v)
