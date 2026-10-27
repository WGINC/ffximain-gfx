/* Task #118: 0x10054D70, a Direct3D-calling function, at the game's bytes (pipeline/vc6/match.py, /O2). Behaviour: the
   game's own, read from its bytes; the tree's text (tree/src, GATE: FAIL in tree r13) passes two arguments to
   g_pProcessor's slot 9 at 0x10054F1B where the game pushes three, (m, s, m) -- the callee 0x1006D270 rets 12
   (data/purges.json). Written from the sweep's conversion (pipeline/vc6/sweep.py --calls) with the changes in
   recompile/d3d_round_120.md, each kept only where the bytes came closer.
   The up vector is a function-local static object with an inline constructor and an empty inline destructor: the
   compiler's own guard (`mov al, [guard]; test al, 1; ... mov dl, al; or dl, 1`) and the atexit registration of its
   `$E` function, which compiles to the game's 0x10054F70 (`ret`). Their symbols are the compiler's, so their game
   addresses are given here (SYMBOL lines, match.py); 0x10313486 is the tree's _atexit (tree/include/gfx_orig.hpp). */
// SYMBOL: ?$S1@?4??BeginEffectOffscreenPass@@9@4EA 0x1047BC78
// SYMBOL: ?up@?4??BeginEffectOffscreenPass@@9@4UUpV@@A 0x1047BC7C
// SYMBOL: _$E2 0x10054F70
// SYMBOL: _atexit 0x10313486
/* vc6_shim.h, inlined */
/* vc6_shim.h: the tree's type names for Visual C++ 6.0 (pipeline/vc6/sweep.py) */
typedef unsigned char uint8_t; typedef signed char int8_t; typedef unsigned short uint16_t; typedef short int16_t;
typedef unsigned int uint32_t; typedef int int32_t; typedef unsigned __int64 uint64_t; typedef __int64 int64_t;
typedef unsigned char undefined, undefined1, byte, uchar; typedef unsigned short undefined2, ushort, word;
typedef unsigned int undefined4, uint, dword; typedef unsigned __int64 undefined8; typedef unsigned long ulong;
typedef unsigned int size_t; typedef int BOOL;
typedef void *LPVOID; typedef unsigned long DWORD; typedef unsigned int _DWORD; typedef unsigned short _WORD;
typedef unsigned char _BYTE;
#ifndef __cplusplus
typedef unsigned char bool;
#define true 1
#define false 0
typedef int code();              /* Ghidra's code: a function of any arguments */
#else
typedef int code(...);
#endif
/* the decompiler's helpers, as tree/include/xi_ghidra.hpp defines them */
#define NAN(x) ((x) != (x))
#define CONCAT31(h, l) ((uint32_t)(((uint32_t)(h) << 8) | (uint8_t)(l)))
#define xi_sw(x) ((uint32_t)(x))
#ifdef __cplusplus
extern "C"
#endif
double __cdecl sqrt(double);  /* C linkage, as <math.h> declares it */
#define SQRT(x) sqrt(x)
#define XI_GAME(ea) (ea)
/* XI_CALL_ST0(0x10311C2C)(x): 0x10311C2C is the CRT's __ftol, byte for byte LIBC.LIB's (sweep.py FTOL), which the
   compiler calls for a floating-point to integer conversion */
#define XI_FTOL(x) ((__int64)(x))
typedef unsigned char XI_PT1; typedef unsigned short XI_PT2; typedef unsigned int XI_PT4; typedef unsigned __int64 XI_PT8;
typedef signed char XI_PS1; typedef short XI_PS2; typedef int XI_PS4; typedef __int64 XI_PS8;
#define XI_PIECE(x, o, s) (*(XI_PT##s *)((char *)&(x) + (o)))
#define XI_PIECE_S(x, o, s) (*(XI_PS##s *)((char *)&(x) + (o)))
/* a piece of an array global (Visual C++ 6.0 takes no & of `*(T (*)[N])address`: C2101): from the array's first byte,
   the same address (sweep.py convert) */
#define XI_PIECEA(x, o, s) (*(XI_PT##s *)((char *)(x) + (o)))
#define XI_PIECEA_S(x, o, s) (*(XI_PS##s *)((char *)(x) + (o)))
/* a 3-byte piece has no type: its value, read from the 4-byte word (sweep.py macros rewrites XI_PIECE(x, o, 3)) */
#define XI_PIECE3(x, o) ((((uint32_t)(x)) >> (8 * (o))) & 0xFFFFFFu)
#define CONCAT13(h, l) ((uint32_t)(((uint32_t)(uint8_t)(h) << 24) | ((uint32_t)(l) & 0xFFFFFFu)))
/* the CRT's string functions (tree/include/xi_decomp.hpp: <string.h>), with C linkage in C++ as well: the intrinsic
   pragma takes only a C-linkage function (cl C4162) */
#ifdef __cplusplus
extern "C" {
#endif
void *__cdecl memcpy(void *, const void *, size_t);
void *__cdecl memset(void *, int, size_t);
char *__cdecl strcpy(char *, const char *);
/* and the others the tree's <string.h> declares that Visual C++ 6.0 has as intrinsics (its STRING.H, without _CRTIMP:
   the static CRT) */
int __cdecl memcmp(const void *, const void *, size_t);
char *__cdecl strcat(char *, const char *);
int __cdecl strcmp(const char *, const char *);
size_t __cdecl strlen(const char *);
#ifdef __cplusplus
}
#endif
#pragma intrinsic(memcpy, memset, strcpy, memcmp, strcat, strcmp, strlen)
/* __builtin_isunordered(a, b) (GCC): either is a NaN; _isnan (FLOAT.H): nonzero for a NaN */
#ifdef __cplusplus
extern "C" int __cdecl _isnan(double);
#else
int __cdecl _isnan(double);
#endif
#define __builtin_isunordered(a, b) (_isnan(a) || _isnan(b))
#define qmemcpy memcpy
/* more of the decompiler's helpers, as tree/include/xi_ghidra.hpp and xi_decomp.hpp define them */
typedef signed char sbyte; typedef __int64 longlong; typedef unsigned __int64 ulonglong; typedef unsigned __int64 qword;
typedef unsigned char xi_bool; typedef unsigned __int64 _QWORD;
#define __PAIR64__(high, low) (((uint64_t)(uint32_t)(high) << 32) | (uint32_t)(low))
#define __SPAIR64__(high, low) ((int64_t)__PAIR64__(high, low))
#define CONCAT11(h, l) ((uint16_t)(((uint16_t)(uint8_t)(h) << 8) | (uint8_t)(l)))
#define CONCAT12(h, l) ((uint32_t)(((uint32_t)(uint8_t)(h) << 16) | (uint16_t)(l)))
#define CONCAT21(h, l) ((uint32_t)(((uint32_t)(uint16_t)(h) << 8) | (uint8_t)(l)))
#define CONCAT22(h, l) ((uint32_t)(((uint32_t)(uint16_t)(h) << 16) | (uint16_t)(l)))
#define CONCAT43(h, l) ((uint64_t)(((uint64_t)(uint32_t)(h) << 24) | ((uint64_t)(l) & 0xFFFFFFui64)))
#define CONCAT44(h, l) ((uint64_t)(((uint64_t)(uint32_t)(h) << 32) | (uint32_t)(l)))
#define ZEXT14(x) ((uint32_t)(uint8_t)(x))
#define ZEXT24(x) ((uint32_t)(uint16_t)(x))
#define ZEXT48(x) ((uint64_t)(uint32_t)(x))
#define ZEXT12(x) ((uint16_t)(uint8_t)(x))
#define SEXT14(x) ((int32_t)(int8_t)(x))
#define SEXT24(x) ((int32_t)(int16_t)(x))
#define SEXT48(x) ((int64_t)(int32_t)(x))
#define SEXT12(x) ((int16_t)(int8_t)(x))
#define LAST_IND(x, part_type) (sizeof(x) / sizeof(part_type) - 1)
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
__declspec(dllimport) DWORD __stdcall timeGetTime(void);
#ifndef __cplusplus
typedef unsigned int XI_ANY;    /* any 4 bytes (tree/include/xi_ghidra_shared.hpp): in C, the word */
#define nullptr ((void *)0)
#endif
#ifdef __cplusplus
/* any 4 bytes, from an argument of any 4-byte type: passed on the stack as the word, as MSVC passes a 4-byte struct */
struct XI_ANY { unsigned int bits; XI_ANY() {} template <class T> XI_ANY(T v) : bits((unsigned int)v) {} };
#define nullptr 0
/* xi_castbits<To>(x) (tree/include/xi_ghidra.hpp): the bits of x as a To; To deduced from a null pointer, which
   Visual C++ 6.0 handles where an explicit template argument it does not (sweep.py macros) */
template <class To, class From> inline To xi_castbits_(From f, To *) { To t; memcpy(&t, &f, sizeof t); return t; }
/* COERCE_FLOAT(v) (tree/include/xi_decomp.hpp): xi_bitcast<float>((uint32_t)(v)), the word's bits as a float */
#define COERCE_FLOAT(v) xi_castbits_((uint32_t)(v), (float *)0)
#endif

struct XV9_dbef47 { virtual void s0(); virtual void s1(); virtual void s2(); virtual void s3(); virtual void s4(); virtual void s5(); virtual void s6(); virtual void s7(); virtual void s8(); virtual void f(float *, float *); };
struct CMoProcessor; typedef struct CMoProcessor CMoProcessor;
#define Fn10054D70_lookAtUp (*(int *)0x1047BC7Cu)
#define Fn10054D70_lookAtUp_y (*(int *)0x1047BC80u)
#define Fn10054D70_lookAtUp_z (*(int *)0x1047BC84u)
#define Fn10054D70_staticGuards (*(char *)0x1047BC78u)
#define g_pMoAccumTarget (*(int *)0x1047BB88u)
#define g_pProcessor (*(CMoProcessor * *)0x1047CF7Cu)
#define g_pResourceMng (*(int *)0x1047BFA8u)
struct FUNt_1006A060 { int f(int, int, int); };
struct FUNt_1006A420 { int f(void); };
struct FUNt_1006A4C0 { int f(void); };
extern "C" int __cdecl FUN_10313486(void *);
struct FUNt_1006A4A0 { int f(void *); };
extern "C" float * __stdcall FUN_102D7FF9(void *, void *, void *, void *);
struct FUNt_1000BD50 { int f(int, void *); };
struct FUNt_100746A0 { int f(void); };
struct FUNt_1006A480 { int f(void *); };
struct XProc9 { virtual void s0(); virtual void s1(); virtual void s2(); virtual void s3(); virtual void s4(); virtual void s5(); virtual void s6(); virtual void s7(); virtual void s8(); virtual void f(float *, float *, float *); };
struct UpV { float x, y, z; UpV(float a, float b, float c) { x = a; y = b; z = c; } ~UpV() {} };
struct VP { unsigned int X, Y, Width, Height; float MinZ, MaxZ; };
struct V3f { float x, y, z; };
// FUNCTION: FFXIMAIN 0x10054D70
extern "C" char __cdecl BeginEffectOffscreenPass(void)
{
  int t = *(int *)(g_pMoAccumTarget + 4);
  if (t == 0) return 0;
  ((FUNt_1006A060 *)((void **)g_pResourceMng))->f(t, *(int *)(g_pMoAccumTarget + 8), *(int *)(g_pMoAccumTarget + 12));
  (*(void (__stdcall **)(void *, int, int, int, int, int, int))(**(int **)(g_pResourceMng + 3772) + 144))(
    *(void **)(g_pResourceMng + 3772), 0, 0, 3, 0, 1065353216, 0);
  ((FUNt_1006A420 *)((int *)g_pResourceMng))->f();
  ((FUNt_1006A4C0 *)((int *)g_pResourceMng))->f();
  VP vp;
  memset(&vp, 0, sizeof vp);
  vp.Y = 0; vp.Width = 256; vp.Height = 256;
  { int g = (int)g_pResourceMng; vp.X = 0; vp.MinZ = 0.0f; vp.MaxZ = 1.0f;
  (*(void (__stdcall **)(int, VP *))(**(int **)(g + 3772) + 160))(*(int *)(g + 3772), &vp); }
  static UpV up(0.0f, 0.0f, -1.0f);
  int top = (*(int *)((char *)g_pProcessor + 0xA90));
  float *m = ((float * *)((char *)g_pProcessor + 0xA10))[top];
  (*(int *)((char *)g_pProcessor + 0xA90)) = top + 1;
  ((FUNt_1006A4A0 *)((int *)g_pResourceMng))->f(m);
  V3f eye, at;
  float lk[16];
  eye.x = m[12]; eye.y = m[13]; eye.z = m[14];
  at = eye;
  at.y = eye.y - 2.0f;
  FUN_102D7FF9(lk, (float *)&at, (float *)&eye, (float *)&up);
  ((FUNt_1000BD50 *)(*(int ***)(g_pResourceMng + 3528)))->f(2, lk);
  int top2 = (*(int *)((char *)g_pProcessor + 0xA90));
  float *s = ((float * *)((char *)g_pProcessor + 0xA10))[top2];
  (*(int *)((char *)g_pProcessor + 0xA90)) = top2 + 1;
  ((FUNt_100746A0 *)(s))->f();
  s[10] = 0.25f; s[5] = 0.25f; s[0] = 0.25f;
  ((XProc9 *)(g_pProcessor))->f(m, s, m);
  ((FUNt_1006A480 *)((int ***)g_pResourceMng))->f(m);
  { char *pr = (char *)g_pProcessor; ((float **)(pr + 0xA10))[--*(int *)(pr + 0xA90)] = s; }
  { char *pr = (char *)g_pProcessor; ((float **)(pr + 0xA10))[--*(int *)(pr + 0xA90)] = m; }
  return 1;
}
