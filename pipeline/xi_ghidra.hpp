// xi_ghidra.hpp -- the types and helper operations Ghidra's decompiler output uses, for translating
// Ghidra decompilations (pipeline/ghidra/GfxExport.java) with the same pipeline as Hex-Rays output.
#pragma once
#include <stdint.h>

typedef unsigned char undefined; typedef unsigned char undefined1; typedef uint16_t undefined2;
typedef uint32_t undefined3; typedef uint32_t undefined4; typedef uint64_t undefined5; typedef uint64_t undefined6;
typedef uint64_t undefined7; typedef uint64_t undefined8;
typedef unsigned char byte; typedef unsigned char uchar; typedef unsigned short ushort; typedef unsigned int uint;
typedef unsigned long ulong; typedef long long longlong; typedef unsigned long long ulonglong;
typedef unsigned short word; typedef uint32_t dword; typedef uint64_t qword;
typedef long double float10;                 // x87 80-bit; GCC's long double on i686
typedef unsigned short wchar16; typedef unsigned int wchar32;
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
#define SUB41(x, b) ((uint8_t)((uint32_t)(x) >> (8 * (b))))
#define SUB42(x, b) ((uint16_t)((uint32_t)(x) >> (8 * (b))))
#define SUB21(x, b) ((uint8_t)((uint16_t)(x) >> (8 * (b))))
#define SUB81(x, b) ((uint8_t)((uint64_t)(x) >> (8 * (b))))
#define SUB84(x, b) ((uint32_t)((uint64_t)(x) >> (8 * (b))))
#define SUB82(x, b) ((uint16_t)((uint64_t)(x) >> (8 * (b))))
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
