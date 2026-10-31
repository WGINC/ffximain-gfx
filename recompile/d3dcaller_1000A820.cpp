/* Task #119: 0x1000A820 DrawFadeQuad, a direct caller of the render core (and #118's 0x10009E00 beside it), at the
   game's bytes (pipeline/vc6/match.py, /O2). Behaviour: the tree's (the sweep's text of the tree's function,
   gate-verified), with these forms, each needed for the game's bytes (recompile/d3d_round_121.md):
   * rule 28: 0x10009E00 (the current viewport entry: this + 0x98 + 24 * byte[this + 0x158]) is defined before its
     caller in this file; the game calls it six times without reloading ECX, which VC6 gives only so;
   * the colour argument modified in place as the parameter (unsigned), not copied to a local;
   * cpp_permuter's forms (pipeline/vc6/cpp_perm.py): the if/else on the colour's alpha with its arms in the other
     order, two colour stores chained (`a = b = x` stores b first, recompile/d3d_round_120.md), a float temporary bound by reference.
   The shim and the Direct3D 8 header the sweep writes are inlined. */
/* ---- vc6_shim.h, inlined (pipeline/vc6/sweep.py writes it) ---- */
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
/* ---- end of vc6_shim.h ---- */
/* ---- xi_d3d8_vc6.h, inlined (pipeline/vc6/sweep.py writes it) ---- */
// xi_d3d8.hpp -- Direct3D 8 enums and COM interfaces, generated from MinGW-w64 d3d8.h /
// d3d8types.h (method order = vtable layout). C-style (lpVtbl) so decompiled calls compile
// unchanged; each interface also gets inline member wrappers so clean code can write
// dev->SetRenderState(...). The wrappers add no data: sizeof(interface) == one vtable pointer.
#pragma once
#ifndef MAKEFOURCC
#define MAKEFOURCC(a, b, c, d) ((unsigned int)(unsigned char)(a) | ((unsigned int)(unsigned char)(b) << 8) | \
                                ((unsigned int)(unsigned char)(c) << 16) | ((unsigned int)(unsigned char)(d) << 24))
#endif
// generated from MinGW-w64 d3d8.h / d3d8types.h
enum D3DRENDERSTATETYPE {
  D3DRS_ZENABLE = 7,
  D3DRS_FILLMODE = 8,
  D3DRS_SHADEMODE = 9,
  D3DRS_LINEPATTERN = 10,
  D3DRS_ZWRITEENABLE = 14,
  D3DRS_ALPHATESTENABLE = 15,
  D3DRS_LASTPIXEL = 16,
  D3DRS_SRCBLEND = 19,
  D3DRS_DESTBLEND = 20,
  D3DRS_CULLMODE = 22,
  D3DRS_ZFUNC = 23,
  D3DRS_ALPHAREF = 24,
  D3DRS_ALPHAFUNC = 25,
  D3DRS_DITHERENABLE = 26,
  D3DRS_ALPHABLENDENABLE = 27,
  D3DRS_FOGENABLE = 28,
  D3DRS_SPECULARENABLE = 29,
  D3DRS_ZVISIBLE = 30,
  D3DRS_FOGCOLOR = 34,
  D3DRS_FOGTABLEMODE = 35,
  D3DRS_FOGSTART = 36,
  D3DRS_FOGEND = 37,
  D3DRS_FOGDENSITY = 38,
  D3DRS_EDGEANTIALIAS = 40,
  D3DRS_ZBIAS = 47,
  D3DRS_RANGEFOGENABLE = 48,
  D3DRS_STENCILENABLE = 52,
  D3DRS_STENCILFAIL = 53,
  D3DRS_STENCILZFAIL = 54,
  D3DRS_STENCILPASS = 55,
  D3DRS_STENCILFUNC = 56,
  D3DRS_STENCILREF = 57,
  D3DRS_STENCILMASK = 58,
  D3DRS_STENCILWRITEMASK = 59,
  D3DRS_TEXTUREFACTOR = 60,
  D3DRS_WRAP0 = 128,
  D3DRS_WRAP1 = 129,
  D3DRS_WRAP2 = 130,
  D3DRS_WRAP3 = 131,
  D3DRS_WRAP4 = 132,
  D3DRS_WRAP5 = 133,
  D3DRS_WRAP6 = 134,
  D3DRS_WRAP7 = 135,
  D3DRS_CLIPPING = 136,
  D3DRS_LIGHTING = 137,
  D3DRS_AMBIENT = 139,
  D3DRS_FOGVERTEXMODE = 140,
  D3DRS_COLORVERTEX = 141,
  D3DRS_LOCALVIEWER = 142,
  D3DRS_NORMALIZENORMALS = 143,
  D3DRS_DIFFUSEMATERIALSOURCE = 145,
  D3DRS_SPECULARMATERIALSOURCE = 146,
  D3DRS_AMBIENTMATERIALSOURCE = 147,
  D3DRS_EMISSIVEMATERIALSOURCE = 148,
  D3DRS_VERTEXBLEND = 151,
  D3DRS_CLIPPLANEENABLE = 152,
  D3DRS_SOFTWAREVERTEXPROCESSING = 153,
  D3DRS_POINTSIZE = 154,
  D3DRS_POINTSIZE_MIN = 155,
  D3DRS_POINTSPRITEENABLE = 156,
  D3DRS_POINTSCALEENABLE = 157,
  D3DRS_POINTSCALE_A = 158,
  D3DRS_POINTSCALE_B = 159,
  D3DRS_POINTSCALE_C = 160,
  D3DRS_MULTISAMPLEANTIALIAS = 161,
  D3DRS_MULTISAMPLEMASK = 162,
  D3DRS_PATCHEDGESTYLE = 163,
  D3DRS_PATCHSEGMENTS = 164,
  D3DRS_DEBUGMONITORTOKEN = 165,
  D3DRS_POINTSIZE_MAX = 166,
  D3DRS_INDEXEDVERTEXBLENDENABLE = 167,
  D3DRS_COLORWRITEENABLE = 168,
  D3DRS_TWEENFACTOR = 170,
  D3DRS_BLENDOP = 171,
  D3DRS_POSITIONORDER = 172,
  D3DRS_NORMALORDER = 173
};
enum D3DTRANSFORMSTATETYPE {
  D3DTS_VIEW = 2,
  D3DTS_PROJECTION = 3,
  D3DTS_TEXTURE0 = 16,
  D3DTS_TEXTURE1 = 17,
  D3DTS_TEXTURE2 = 18,
  D3DTS_TEXTURE3 = 19,
  D3DTS_TEXTURE4 = 20,
  D3DTS_TEXTURE5 = 21,
  D3DTS_TEXTURE6 = 22,
  D3DTS_TEXTURE7 = 23
};
enum D3DTEXTURESTAGESTATETYPE {
  D3DTSS_COLOROP = 1,
  D3DTSS_COLORARG1 = 2,
  D3DTSS_COLORARG2 = 3,
  D3DTSS_ALPHAOP = 4,
  D3DTSS_ALPHAARG1 = 5,
  D3DTSS_ALPHAARG2 = 6,
  D3DTSS_BUMPENVMAT00 = 7,
  D3DTSS_BUMPENVMAT01 = 8,
  D3DTSS_BUMPENVMAT10 = 9,
  D3DTSS_BUMPENVMAT11 = 10,
  D3DTSS_TEXCOORDINDEX = 11,
  D3DTSS_ADDRESSU = 13,
  D3DTSS_ADDRESSV = 14,
  D3DTSS_BORDERCOLOR = 15,
  D3DTSS_MAGFILTER = 16,
  D3DTSS_MINFILTER = 17,
  D3DTSS_MIPFILTER = 18,
  D3DTSS_MIPMAPLODBIAS = 19,
  D3DTSS_MAXMIPLEVEL = 20,
  D3DTSS_MAXANISOTROPY = 21,
  D3DTSS_BUMPENVLSCALE = 22,
  D3DTSS_BUMPENVLOFFSET = 23,
  D3DTSS_TEXTURETRANSFORMFLAGS = 24,
  D3DTSS_ADDRESSW = 25,
  D3DTSS_COLORARG0 = 26,
  D3DTSS_ALPHAARG0 = 27,
  D3DTSS_RESULTARG = 28
};
enum D3DPRIMITIVETYPE {
  D3DPT_POINTLIST = 1,
  D3DPT_LINELIST = 2,
  D3DPT_LINESTRIP = 3,
  D3DPT_TRIANGLELIST = 4,
  D3DPT_TRIANGLESTRIP = 5,
  D3DPT_TRIANGLEFAN = 6
};
enum D3DFORMAT {
  D3DFMT_UNKNOWN = 0,
  D3DFMT_R8G8B8 = 20,
  D3DFMT_A8R8G8B8 = 21,
  D3DFMT_X8R8G8B8 = 22,
  D3DFMT_R5G6B5 = 23,
  D3DFMT_X1R5G5B5 = 24,
  D3DFMT_A1R5G5B5 = 25,
  D3DFMT_A4R4G4B4 = 26,
  D3DFMT_R3G3B2 = 27,
  D3DFMT_A8 = 28,
  D3DFMT_A8R3G3B2 = 29,
  D3DFMT_X4R4G4B4 = 30,
  D3DFMT_A2B10G10R10 = 31,
  D3DFMT_G16R16 = 34,
  D3DFMT_A8P8 = 40,
  D3DFMT_P8 = 41,
  D3DFMT_L8 = 50,
  D3DFMT_A8L8 = 51,
  D3DFMT_A4L4 = 52,
  D3DFMT_V8U8 = 60,
  D3DFMT_L6V5U5 = 61,
  D3DFMT_X8L8V8U8 = 62,
  D3DFMT_Q8W8V8U8 = 63,
  D3DFMT_V16U16 = 64,
  D3DFMT_W11V11U10 = 65,
  D3DFMT_A2W10V10U10 = 67,
  D3DFMT_UYVY = MAKEFOURCC('U',
  'Y',
  'V',
  'Y'),
  D3DFMT_YUY2 = MAKEFOURCC('Y',
  'U',
  'Y',
  '2'),
  D3DFMT_DXT1 = MAKEFOURCC('D',
  'X',
  'T',
  '1'),
  D3DFMT_DXT2 = MAKEFOURCC('D',
  'X',
  'T',
  '2'),
  D3DFMT_DXT3 = MAKEFOURCC('D',
  'X',
  'T',
  '3'),
  D3DFMT_DXT4 = MAKEFOURCC('D',
  'X',
  'T',
  '4'),
  D3DFMT_DXT5 = MAKEFOURCC('D',
  'X',
  'T',
  '5'),
  D3DFMT_D16_LOCKABLE = 70,
  D3DFMT_D32 = 71,
  D3DFMT_D15S1 = 73,
  D3DFMT_D24S8 = 75,
  D3DFMT_D16 = 80,
  D3DFMT_D24X8 = 77,
  D3DFMT_D24X4S4 = 79,
  D3DFMT_VERTEXDATA = 100,
  D3DFMT_INDEX16 = 101,
  D3DFMT_INDEX32 = 102
};
enum D3DPOOL {
  D3DPOOL_DEFAULT = 0,
  D3DPOOL_MANAGED = 1,
  D3DPOOL_SYSTEMMEM = 2,
  D3DPOOL_SCRATCH = 3
};
enum D3DRESOURCETYPE {
  D3DRTYPE_SURFACE = 1,
  D3DRTYPE_VOLUME = 2,
  D3DRTYPE_TEXTURE = 3,
  D3DRTYPE_VOLUMETEXTURE = 4,
  D3DRTYPE_CUBETEXTURE = 5,
  D3DRTYPE_VERTEXBUFFER = 6,
  D3DRTYPE_INDEXBUFFER = 7
};
enum D3DDEVTYPE {
  D3DDEVTYPE_HAL = 1,
  D3DDEVTYPE_REF = 2,
  D3DDEVTYPE_SW = 3
};
enum D3DMULTISAMPLE_TYPE {
  D3DMULTISAMPLE_NONE = 0,
  D3DMULTISAMPLE_2_SAMPLES = 2,
  D3DMULTISAMPLE_3_SAMPLES = 3,
  D3DMULTISAMPLE_4_SAMPLES = 4,
  D3DMULTISAMPLE_5_SAMPLES = 5,
  D3DMULTISAMPLE_6_SAMPLES = 6,
  D3DMULTISAMPLE_7_SAMPLES = 7,
  D3DMULTISAMPLE_8_SAMPLES = 8,
  D3DMULTISAMPLE_9_SAMPLES = 9,
  D3DMULTISAMPLE_10_SAMPLES = 10,
  D3DMULTISAMPLE_11_SAMPLES = 11,
  D3DMULTISAMPLE_12_SAMPLES = 12,
  D3DMULTISAMPLE_13_SAMPLES = 13,
  D3DMULTISAMPLE_14_SAMPLES = 14,
  D3DMULTISAMPLE_15_SAMPLES = 15,
  D3DMULTISAMPLE_16_SAMPLES = 16
};
enum D3DBACKBUFFER_TYPE {
  D3DBACKBUFFER_TYPE_MONO = 0,
  D3DBACKBUFFER_TYPE_LEFT = 1,
  D3DBACKBUFFER_TYPE_RIGHT = 2
};
enum D3DSTATEBLOCKTYPE {
  D3DSBT_ALL = 1,
  D3DSBT_PIXELSTATE = 2,
  D3DSBT_VERTEXSTATE = 3
};
struct D3DLOCKED_RECT { int Pitch; void *pBits; };
struct D3DLOCKED_BOX { int RowPitch; int SlicePitch; void *pBits; };
struct D3DVIEWPORT8 { unsigned int X; unsigned int Y; unsigned int Width; unsigned int Height; float MinZ; float MaxZ; };
typedef D3DVIEWPORT8 _D3DVIEWPORT8;
struct IDirect3D8;
struct IDirect3DVolume8;
struct IDirect3DSwapChain8;
struct IDirect3DSurface8;
struct IDirect3DResource8;
struct IDirect3DVertexBuffer8;
struct IDirect3DIndexBuffer8;
struct IDirect3DBaseTexture8;
struct IDirect3DCubeTexture8;
struct IDirect3DTexture8;
struct IDirect3DVolumeTexture8;
struct IDirect3DDevice8;
struct IDirect3D8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3D8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3D8 *This);
  int (__stdcall *Release)(IDirect3D8 *This);
  int (__stdcall *RegisterSoftwareDevice)(IDirect3D8 *This, void * pInitializeFunction);
  int (__stdcall *GetAdapterCount)(IDirect3D8 *This);
  int (__stdcall *GetAdapterIdentifier)(IDirect3D8 *This, unsigned int Adapter, unsigned int Flags, void * pIdentifier);
  int (__stdcall *GetAdapterModeCount)(IDirect3D8 *This, unsigned int Adapter);
  int (__stdcall *EnumAdapterModes)(IDirect3D8 *This, unsigned int Adapter, unsigned int Mode, void * pMode);
  int (__stdcall *GetAdapterDisplayMode)(IDirect3D8 *This, unsigned int Adapter, void * pMode);
  int (__stdcall *CheckDeviceType)(IDirect3D8 *This, unsigned int Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, unsigned int Windowed);
  int (__stdcall *CheckDeviceFormat)(IDirect3D8 *This, unsigned int Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, unsigned int Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat);
  int (__stdcall *CheckDeviceMultiSampleType)(IDirect3D8 *This, unsigned int Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, unsigned int Windowed, D3DMULTISAMPLE_TYPE MultiSampleType);
  int (__stdcall *CheckDepthStencilMatch)(IDirect3D8 *This, unsigned int Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat);
  int (__stdcall *GetDeviceCaps)(IDirect3D8 *This, unsigned int Adapter, D3DDEVTYPE DeviceType, void * pCaps);
  int (__stdcall *GetAdapterMonitor)(IDirect3D8 *This, unsigned int Adapter);
  int (__stdcall *CreateDevice)(IDirect3D8 *This, unsigned int Adapter, D3DDEVTYPE DeviceType, unsigned int hFocusWindow, unsigned int BehaviorFlags, void * pPresentationParameters, IDirect3DDevice8 ** ppReturnedDeviceInterface);
};

struct IDirect3DVolume8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3DVolume8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3DVolume8 *This);
  int (__stdcall *Release)(IDirect3DVolume8 *This);
  int (__stdcall *GetDevice)(IDirect3DVolume8 *This, IDirect3DDevice8 ** ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DVolume8 *This, unsigned int refguid, void * data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DVolume8 *This, unsigned int refguid, void * pData, void * pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DVolume8 *This, unsigned int refguid);
  int (__stdcall *GetContainer)(IDirect3DVolume8 *This, unsigned int riid, void ** ppContainer);
  int (__stdcall *GetDesc)(IDirect3DVolume8 *This, void * pDesc);
  int (__stdcall *LockBox)(IDirect3DVolume8 *This, D3DLOCKED_BOX *locked_box, void * box, unsigned int flags);
  int (__stdcall *UnlockBox)(IDirect3DVolume8 *This);
};

struct IDirect3DSwapChain8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3DSwapChain8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3DSwapChain8 *This);
  int (__stdcall *Release)(IDirect3DSwapChain8 *This);
  int (__stdcall *Present)(IDirect3DSwapChain8 *This, void * src_rect, void * dst_rect, unsigned int dst_window_override, void * dirty_region);
  int (__stdcall *GetBackBuffer)(IDirect3DSwapChain8 *This, unsigned int BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8 ** ppBackBuffer);
};

struct IDirect3DSurface8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3DSurface8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3DSurface8 *This);
  int (__stdcall *Release)(IDirect3DSurface8 *This);
  int (__stdcall *GetDevice)(IDirect3DSurface8 *This, IDirect3DDevice8 ** ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DSurface8 *This, unsigned int refguid, void * data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DSurface8 *This, unsigned int refguid, void * pData, void * pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DSurface8 *This, unsigned int refguid);
  int (__stdcall *GetContainer)(IDirect3DSurface8 *This, unsigned int riid, void ** ppContainer);
  int (__stdcall *GetDesc)(IDirect3DSurface8 *This, void * pDesc);
  int (__stdcall *LockRect)(IDirect3DSurface8 *This, D3DLOCKED_RECT *locked_rect, void * rect, unsigned int flags);
  int (__stdcall *UnlockRect)(IDirect3DSurface8 *This);
};

struct IDirect3DResource8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3DResource8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3DResource8 *This);
  int (__stdcall *Release)(IDirect3DResource8 *This);
  int (__stdcall *GetDevice)(IDirect3DResource8 *This, IDirect3DDevice8 ** ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DResource8 *This, unsigned int refguid, void * data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DResource8 *This, unsigned int refguid, void * pData, void * pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DResource8 *This, unsigned int refguid);
  int (__stdcall *SetPriority)(IDirect3DResource8 *This, unsigned int PriorityNew);
  int (__stdcall *GetPriority)(IDirect3DResource8 *This);
  int (__stdcall *PreLoad)(IDirect3DResource8 *This);
  int (__stdcall *GetType)(IDirect3DResource8 *This);
};

struct IDirect3DVertexBuffer8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3DVertexBuffer8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3DVertexBuffer8 *This);
  int (__stdcall *Release)(IDirect3DVertexBuffer8 *This);
  int (__stdcall *GetDevice)(IDirect3DVertexBuffer8 *This, IDirect3DDevice8 ** ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DVertexBuffer8 *This, unsigned int refguid, void * data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DVertexBuffer8 *This, unsigned int refguid, void * pData, void * pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DVertexBuffer8 *This, unsigned int refguid);
  int (__stdcall *SetPriority)(IDirect3DVertexBuffer8 *This, unsigned int PriorityNew);
  int (__stdcall *GetPriority)(IDirect3DVertexBuffer8 *This);
  int (__stdcall *PreLoad)(IDirect3DVertexBuffer8 *This);
  int (__stdcall *GetType)(IDirect3DVertexBuffer8 *This);
  int (__stdcall *Lock)(IDirect3DVertexBuffer8 *This, unsigned int OffsetToLock, unsigned int SizeToLock, void ** ppbData, unsigned int Flags);
  int (__stdcall *Unlock)(IDirect3DVertexBuffer8 *This);
  int (__stdcall *GetDesc)(IDirect3DVertexBuffer8 *This, void * pDesc);
};

struct IDirect3DIndexBuffer8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3DIndexBuffer8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3DIndexBuffer8 *This);
  int (__stdcall *Release)(IDirect3DIndexBuffer8 *This);
  int (__stdcall *GetDevice)(IDirect3DIndexBuffer8 *This, IDirect3DDevice8 ** ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DIndexBuffer8 *This, unsigned int refguid, void * data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DIndexBuffer8 *This, unsigned int refguid, void * pData, void * pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DIndexBuffer8 *This, unsigned int refguid);
  int (__stdcall *SetPriority)(IDirect3DIndexBuffer8 *This, unsigned int PriorityNew);
  int (__stdcall *GetPriority)(IDirect3DIndexBuffer8 *This);
  int (__stdcall *PreLoad)(IDirect3DIndexBuffer8 *This);
  int (__stdcall *GetType)(IDirect3DIndexBuffer8 *This);
  int (__stdcall *Lock)(IDirect3DIndexBuffer8 *This, unsigned int OffsetToLock, unsigned int SizeToLock, void ** ppbData, unsigned int Flags);
  int (__stdcall *Unlock)(IDirect3DIndexBuffer8 *This);
  int (__stdcall *GetDesc)(IDirect3DIndexBuffer8 *This, void * pDesc);
};

struct IDirect3DBaseTexture8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3DBaseTexture8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3DBaseTexture8 *This);
  int (__stdcall *Release)(IDirect3DBaseTexture8 *This);
  int (__stdcall *GetDevice)(IDirect3DBaseTexture8 *This, IDirect3DDevice8 ** ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DBaseTexture8 *This, unsigned int refguid, void * data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DBaseTexture8 *This, unsigned int refguid, void * pData, void * pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DBaseTexture8 *This, unsigned int refguid);
  int (__stdcall *SetPriority)(IDirect3DBaseTexture8 *This, unsigned int PriorityNew);
  int (__stdcall *GetPriority)(IDirect3DBaseTexture8 *This);
  int (__stdcall *PreLoad)(IDirect3DBaseTexture8 *This);
  int (__stdcall *GetType)(IDirect3DBaseTexture8 *This);
  int (__stdcall *SetLOD)(IDirect3DBaseTexture8 *This, unsigned int LODNew);
  int (__stdcall *GetLOD)(IDirect3DBaseTexture8 *This);
  int (__stdcall *GetLevelCount)(IDirect3DBaseTexture8 *This);
};

struct IDirect3DCubeTexture8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3DCubeTexture8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3DCubeTexture8 *This);
  int (__stdcall *Release)(IDirect3DCubeTexture8 *This);
  int (__stdcall *GetDevice)(IDirect3DCubeTexture8 *This, IDirect3DDevice8 ** ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DCubeTexture8 *This, unsigned int refguid, void * data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DCubeTexture8 *This, unsigned int refguid, void * pData, void * pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DCubeTexture8 *This, unsigned int refguid);
  int (__stdcall *SetPriority)(IDirect3DCubeTexture8 *This, unsigned int PriorityNew);
  int (__stdcall *GetPriority)(IDirect3DCubeTexture8 *This);
  int (__stdcall *PreLoad)(IDirect3DCubeTexture8 *This);
  int (__stdcall *GetType)(IDirect3DCubeTexture8 *This);
  int (__stdcall *SetLOD)(IDirect3DCubeTexture8 *This, unsigned int LODNew);
  int (__stdcall *GetLOD)(IDirect3DCubeTexture8 *This);
  int (__stdcall *GetLevelCount)(IDirect3DCubeTexture8 *This);
  int (__stdcall *GetLevelDesc)(IDirect3DCubeTexture8 *This, unsigned int Level, void * pDesc);
  int (__stdcall *GetCubeMapSurface)(IDirect3DCubeTexture8 *This, unsigned int FaceType, unsigned int Level, IDirect3DSurface8 ** ppCubeMapSurface);
  int (__stdcall *LockRect)(IDirect3DCubeTexture8 *This, unsigned int face, unsigned int level, D3DLOCKED_RECT *locked_rect, void * rect, unsigned int flags);
  int (__stdcall *UnlockRect)(IDirect3DCubeTexture8 *This, unsigned int FaceType, unsigned int Level);
  int (__stdcall *AddDirtyRect)(IDirect3DCubeTexture8 *This, unsigned int face, void * dirty_rect);
};

struct IDirect3DTexture8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3DTexture8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3DTexture8 *This);
  int (__stdcall *Release)(IDirect3DTexture8 *This);
  int (__stdcall *GetDevice)(IDirect3DTexture8 *This, IDirect3DDevice8 ** ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DTexture8 *This, unsigned int refguid, void * data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DTexture8 *This, unsigned int refguid, void * pData, void * pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DTexture8 *This, unsigned int refguid);
  int (__stdcall *SetPriority)(IDirect3DTexture8 *This, unsigned int PriorityNew);
  int (__stdcall *GetPriority)(IDirect3DTexture8 *This);
  int (__stdcall *PreLoad)(IDirect3DTexture8 *This);
  int (__stdcall *GetType)(IDirect3DTexture8 *This);
  int (__stdcall *SetLOD)(IDirect3DTexture8 *This, unsigned int LODNew);
  int (__stdcall *GetLOD)(IDirect3DTexture8 *This);
  int (__stdcall *GetLevelCount)(IDirect3DTexture8 *This);
  int (__stdcall *GetLevelDesc)(IDirect3DTexture8 *This, unsigned int Level, void * pDesc);
  int (__stdcall *GetSurfaceLevel)(IDirect3DTexture8 *This, unsigned int Level, IDirect3DSurface8 ** ppSurfaceLevel);
  int (__stdcall *LockRect)(IDirect3DTexture8 *This, unsigned int level, D3DLOCKED_RECT *locked_rect, void * rect, unsigned int flags);
  int (__stdcall *UnlockRect)(IDirect3DTexture8 *This, unsigned int Level);
  int (__stdcall *AddDirtyRect)(IDirect3DTexture8 *This, void * dirty_rect);
};

struct IDirect3DVolumeTexture8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3DVolumeTexture8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3DVolumeTexture8 *This);
  int (__stdcall *Release)(IDirect3DVolumeTexture8 *This);
  int (__stdcall *GetDevice)(IDirect3DVolumeTexture8 *This, IDirect3DDevice8 ** ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DVolumeTexture8 *This, unsigned int refguid, void * data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DVolumeTexture8 *This, unsigned int refguid, void * pData, void * pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DVolumeTexture8 *This, unsigned int refguid);
  int (__stdcall *SetPriority)(IDirect3DVolumeTexture8 *This, unsigned int PriorityNew);
  int (__stdcall *GetPriority)(IDirect3DVolumeTexture8 *This);
  int (__stdcall *PreLoad)(IDirect3DVolumeTexture8 *This);
  int (__stdcall *GetType)(IDirect3DVolumeTexture8 *This);
  int (__stdcall *SetLOD)(IDirect3DVolumeTexture8 *This, unsigned int LODNew);
  int (__stdcall *GetLOD)(IDirect3DVolumeTexture8 *This);
  int (__stdcall *GetLevelCount)(IDirect3DVolumeTexture8 *This);
  int (__stdcall *GetLevelDesc)(IDirect3DVolumeTexture8 *This, unsigned int Level, void * pDesc);
  int (__stdcall *GetVolumeLevel)(IDirect3DVolumeTexture8 *This, unsigned int Level, IDirect3DVolume8 ** ppVolumeLevel);
  int (__stdcall *LockBox)(IDirect3DVolumeTexture8 *This, unsigned int level, D3DLOCKED_BOX *locked_box, void * box, unsigned int flags);
  int (__stdcall *UnlockBox)(IDirect3DVolumeTexture8 *This, unsigned int Level);
  int (__stdcall *AddDirtyBox)(IDirect3DVolumeTexture8 *This, void * dirty_box);
};

struct IDirect3DDevice8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3DDevice8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3DDevice8 *This);
  int (__stdcall *Release)(IDirect3DDevice8 *This);
  int (__stdcall *TestCooperativeLevel)(IDirect3DDevice8 *This);
  int (__stdcall *GetAvailableTextureMem)(IDirect3DDevice8 *This);
  int (__stdcall *ResourceManagerDiscardBytes)(IDirect3DDevice8 *This, unsigned int Bytes);
  int (__stdcall *GetDirect3D)(IDirect3DDevice8 *This, IDirect3D8 ** ppD3D8);
  int (__stdcall *GetDeviceCaps)(IDirect3DDevice8 *This, void * pCaps);
  int (__stdcall *GetDisplayMode)(IDirect3DDevice8 *This, void * pMode);
  int (__stdcall *GetCreationParameters)(IDirect3DDevice8 *This, void * pParameters);
  int (__stdcall *SetCursorProperties)(IDirect3DDevice8 *This, unsigned int XHotSpot, unsigned int YHotSpot, IDirect3DSurface8 * pCursorBitmap);
  int (__stdcall *SetCursorPosition)(IDirect3DDevice8 *This, unsigned int XScreenSpace, unsigned int YScreenSpace, unsigned int Flags);
  int (__stdcall *ShowCursor)(IDirect3DDevice8 *This, unsigned int bShow);
  int (__stdcall *CreateAdditionalSwapChain)(IDirect3DDevice8 *This, void * pPresentationParameters, IDirect3DSwapChain8 ** pSwapChain);
  int (__stdcall *Reset)(IDirect3DDevice8 *This, void * pPresentationParameters);
  int (__stdcall *Present)(IDirect3DDevice8 *This, void * src_rect, void * dst_rect, unsigned int dst_window_override, void * dirty_region);
  int (__stdcall *GetBackBuffer)(IDirect3DDevice8 *This, unsigned int BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8 ** ppBackBuffer);
  int (__stdcall *GetRasterStatus)(IDirect3DDevice8 *This, void * pRasterStatus);
  int (__stdcall *SetGammaRamp)(IDirect3DDevice8 *This, unsigned int flags, void * ramp);
  int (__stdcall *GetGammaRamp)(IDirect3DDevice8 *This, void * pRamp);
  int (__stdcall *CreateTexture)(IDirect3DDevice8 *This, unsigned int Width, unsigned int Height, unsigned int Levels, unsigned int Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8 ** ppTexture);
  int (__stdcall *CreateVolumeTexture)(IDirect3DDevice8 *This, unsigned int Width, unsigned int Height, unsigned int Depth, unsigned int Levels, unsigned int Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture8 ** ppVolumeTexture);
  int (__stdcall *CreateCubeTexture)(IDirect3DDevice8 *This, unsigned int EdgeLength, unsigned int Levels, unsigned int Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture8 ** ppCubeTexture);
  int (__stdcall *CreateVertexBuffer)(IDirect3DDevice8 *This, unsigned int Length, unsigned int Usage, unsigned int FVF, D3DPOOL Pool, IDirect3DVertexBuffer8 ** ppVertexBuffer);
  int (__stdcall *CreateIndexBuffer)(IDirect3DDevice8 *This, unsigned int Length, unsigned int Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer8 ** ppIndexBuffer);
  int (__stdcall *CreateRenderTarget)(IDirect3DDevice8 *This, unsigned int Width, unsigned int Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, unsigned int Lockable, IDirect3DSurface8 ** ppSurface);
  int (__stdcall *CreateDepthStencilSurface)(IDirect3DDevice8 *This, unsigned int Width, unsigned int Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface8 ** ppSurface);
  int (__stdcall *CreateImageSurface)(IDirect3DDevice8 *This, unsigned int Width, unsigned int Height, D3DFORMAT Format, IDirect3DSurface8 ** ppSurface);
  int (__stdcall *CopyRects)(IDirect3DDevice8 *This, IDirect3DSurface8 * src_surface, void * src_rects, unsigned int rect_count, IDirect3DSurface8 * dst_surface, void * dst_points);
  int (__stdcall *UpdateTexture)(IDirect3DDevice8 *This, IDirect3DBaseTexture8 * pSourceTexture, IDirect3DBaseTexture8 * pDestinationTexture);
  int (__stdcall *GetFrontBuffer)(IDirect3DDevice8 *This, IDirect3DSurface8 * pDestSurface);
  int (__stdcall *SetRenderTarget)(IDirect3DDevice8 *This, IDirect3DSurface8 * pRenderTarget, IDirect3DSurface8 * pNewZStencil);
  int (__stdcall *GetRenderTarget)(IDirect3DDevice8 *This, IDirect3DSurface8 ** ppRenderTarget);
  int (__stdcall *GetDepthStencilSurface)(IDirect3DDevice8 *This, IDirect3DSurface8 ** ppZStencilSurface);
  int (__stdcall *BeginScene)(IDirect3DDevice8 *This);
  int (__stdcall *EndScene)(IDirect3DDevice8 *This);
  int (__stdcall *Clear)(IDirect3DDevice8 *This, unsigned int rect_count, void * rects, unsigned int flags, unsigned int color, float z, unsigned int stencil);
  int (__stdcall *SetTransform)(IDirect3DDevice8 *This, D3DTRANSFORMSTATETYPE state, void * matrix);
  int (__stdcall *GetTransform)(IDirect3DDevice8 *This, D3DTRANSFORMSTATETYPE State, void * pMatrix);
  int (__stdcall *MultiplyTransform)(IDirect3DDevice8 *This, D3DTRANSFORMSTATETYPE state, void * matrix);
  int (__stdcall *SetViewport)(IDirect3DDevice8 *This, void * viewport);
  int (__stdcall *GetViewport)(IDirect3DDevice8 *This, void * pViewport);
  int (__stdcall *SetMaterial)(IDirect3DDevice8 *This, void * material);
  int (__stdcall *GetMaterial)(IDirect3DDevice8 *This, void * pMaterial);
  int (__stdcall *SetLight)(IDirect3DDevice8 *This, unsigned int index, void * light);
  int (__stdcall *GetLight)(IDirect3DDevice8 *This, unsigned int Index, void * pLight);
  int (__stdcall *LightEnable)(IDirect3DDevice8 *This, unsigned int Index, unsigned int Enable);
  int (__stdcall *GetLightEnable)(IDirect3DDevice8 *This, unsigned int Index, void * pEnable);
  int (__stdcall *SetClipPlane)(IDirect3DDevice8 *This, unsigned int index, void * plane);
  int (__stdcall *GetClipPlane)(IDirect3DDevice8 *This, unsigned int Index, void * pPlane);
  int (__stdcall *SetRenderState)(IDirect3DDevice8 *This, D3DRENDERSTATETYPE State, unsigned int Value);
  int (__stdcall *GetRenderState)(IDirect3DDevice8 *This, D3DRENDERSTATETYPE State, void * pValue);
  int (__stdcall *BeginStateBlock)(IDirect3DDevice8 *This);
  int (__stdcall *EndStateBlock)(IDirect3DDevice8 *This, void * pToken);
  int (__stdcall *ApplyStateBlock)(IDirect3DDevice8 *This, unsigned int Token);
  int (__stdcall *CaptureStateBlock)(IDirect3DDevice8 *This, unsigned int Token);
  int (__stdcall *DeleteStateBlock)(IDirect3DDevice8 *This, unsigned int Token);
  int (__stdcall *CreateStateBlock)(IDirect3DDevice8 *This, D3DSTATEBLOCKTYPE Type, void * pToken);
  int (__stdcall *SetClipStatus)(IDirect3DDevice8 *This, void * clip_status);
  int (__stdcall *GetClipStatus)(IDirect3DDevice8 *This, void * pClipStatus);
  int (__stdcall *GetTexture)(IDirect3DDevice8 *This, unsigned int Stage, IDirect3DBaseTexture8 ** ppTexture);
  int (__stdcall *SetTexture)(IDirect3DDevice8 *This, unsigned int Stage, IDirect3DBaseTexture8 * pTexture);
  int (__stdcall *GetTextureStageState)(IDirect3DDevice8 *This, unsigned int Stage, D3DTEXTURESTAGESTATETYPE Type, void * pValue);
  int (__stdcall *SetTextureStageState)(IDirect3DDevice8 *This, unsigned int Stage, D3DTEXTURESTAGESTATETYPE Type, unsigned int Value);
  int (__stdcall *ValidateDevice)(IDirect3DDevice8 *This, void * pNumPasses);
  int (__stdcall *GetInfo)(IDirect3DDevice8 *This, unsigned int DevInfoID, void * pDevInfoStruct, unsigned int DevInfoStructSize);
  int (__stdcall *SetPaletteEntries)(IDirect3DDevice8 *This, unsigned int palette_idx, void * entries);
  int (__stdcall *GetPaletteEntries)(IDirect3DDevice8 *This, unsigned int PaletteNumber, void * pEntries);
  int (__stdcall *SetCurrentTexturePalette)(IDirect3DDevice8 *This, unsigned int PaletteNumber);
  int (__stdcall *GetCurrentTexturePalette)(IDirect3DDevice8 *This, void * PaletteNumber);
  int (__stdcall *DrawPrimitive)(IDirect3DDevice8 *This, D3DPRIMITIVETYPE PrimitiveType, unsigned int StartVertex, unsigned int PrimitiveCount);
  int (__stdcall *DrawIndexedPrimitive)(IDirect3DDevice8 *This, D3DPRIMITIVETYPE PrimitiveType, unsigned int minIndex, unsigned int NumVertices, unsigned int startIndex, unsigned int primCount);
  int (__stdcall *DrawPrimitiveUP)(IDirect3DDevice8 *This, D3DPRIMITIVETYPE primitive_type, unsigned int primitive_count, void * data, unsigned int stride);
  int (__stdcall *DrawIndexedPrimitiveUP)(IDirect3DDevice8 *This, D3DPRIMITIVETYPE primitive_type, unsigned int min_vertex_idx, unsigned int vertex_count, unsigned int primitive_count, void * index_data, D3DFORMAT index_format, void * data, unsigned int stride);
  int (__stdcall *ProcessVertices)(IDirect3DDevice8 *This, unsigned int SrcStartIndex, unsigned int DestIndex, unsigned int VertexCount, IDirect3DVertexBuffer8 * pDestBuffer, unsigned int Flags);
  int (__stdcall *CreateVertexShader)(IDirect3DDevice8 *This, void * declaration, void * byte_code, void * shader, unsigned int usage);
  int (__stdcall *SetVertexShader)(IDirect3DDevice8 *This, unsigned int Handle);
  int (__stdcall *GetVertexShader)(IDirect3DDevice8 *This, void * pHandle);
  int (__stdcall *DeleteVertexShader)(IDirect3DDevice8 *This, unsigned int Handle);
  int (__stdcall *SetVertexShaderConstant)(IDirect3DDevice8 *This, unsigned int reg_idx, void * data, unsigned int count);
  int (__stdcall *GetVertexShaderConstant)(IDirect3DDevice8 *This, unsigned int Register, void * pConstantData, unsigned int ConstantCount);
  int (__stdcall *GetVertexShaderDeclaration)(IDirect3DDevice8 *This, unsigned int Handle, void * pData, void * pSizeOfData);
  int (__stdcall *GetVertexShaderFunction)(IDirect3DDevice8 *This, unsigned int Handle, void * pData, void * pSizeOfData);
  int (__stdcall *SetStreamSource)(IDirect3DDevice8 *This, unsigned int StreamNumber, IDirect3DVertexBuffer8 * pStreamData, unsigned int Stride);
  int (__stdcall *GetStreamSource)(IDirect3DDevice8 *This, unsigned int StreamNumber, IDirect3DVertexBuffer8 ** ppStreamData, void * pStride);
  int (__stdcall *SetIndices)(IDirect3DDevice8 *This, IDirect3DIndexBuffer8 * pIndexData, unsigned int BaseVertexIndex);
  int (__stdcall *GetIndices)(IDirect3DDevice8 *This, IDirect3DIndexBuffer8 ** ppIndexData, void * pBaseVertexIndex);
  int (__stdcall *CreatePixelShader)(IDirect3DDevice8 *This, void * byte_code, void * shader);
  int (__stdcall *SetPixelShader)(IDirect3DDevice8 *This, unsigned int Handle);
  int (__stdcall *GetPixelShader)(IDirect3DDevice8 *This, void * pHandle);
  int (__stdcall *DeletePixelShader)(IDirect3DDevice8 *This, unsigned int Handle);
  int (__stdcall *SetPixelShaderConstant)(IDirect3DDevice8 *This, unsigned int reg_idx, void * data, unsigned int count);
  int (__stdcall *GetPixelShaderConstant)(IDirect3DDevice8 *This, unsigned int Register, void * pConstantData, unsigned int ConstantCount);
  int (__stdcall *GetPixelShaderFunction)(IDirect3DDevice8 *This, unsigned int Handle, void * pData, void * pSizeOfData);
  int (__stdcall *DrawRectPatch)(IDirect3DDevice8 *This, unsigned int handle, void * segment_count, void * patch_info);
  int (__stdcall *DrawTriPatch)(IDirect3DDevice8 *This, unsigned int handle, void * segment_count, void * patch_info);
  int (__stdcall *DeletePatch)(IDirect3DDevice8 *This, unsigned int Handle);
};


// Interfaces: one vtable pointer (the COM layout) plus inline wrappers.
struct IDirect3D8 {
    struct IDirect3D8Vtbl *lpVtbl;
    int QueryInterface(unsigned int riid, void ** ppvObject) { return lpVtbl->QueryInterface(this, riid, ppvObject); }
    int AddRef() { return lpVtbl->AddRef(this); }
    int Release() { return lpVtbl->Release(this); }
    int RegisterSoftwareDevice(void * pInitializeFunction) { return lpVtbl->RegisterSoftwareDevice(this, pInitializeFunction); }
    int GetAdapterCount() { return lpVtbl->GetAdapterCount(this); }
    int GetAdapterIdentifier(unsigned int Adapter, unsigned int Flags, void * pIdentifier) { return lpVtbl->GetAdapterIdentifier(this, Adapter, Flags, pIdentifier); }
    int GetAdapterModeCount(unsigned int Adapter) { return lpVtbl->GetAdapterModeCount(this, Adapter); }
    int EnumAdapterModes(unsigned int Adapter, unsigned int Mode, void * pMode) { return lpVtbl->EnumAdapterModes(this, Adapter, Mode, pMode); }
    int GetAdapterDisplayMode(unsigned int Adapter, void * pMode) { return lpVtbl->GetAdapterDisplayMode(this, Adapter, pMode); }
    int CheckDeviceType(unsigned int Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, unsigned int Windowed) { return lpVtbl->CheckDeviceType(this, Adapter, CheckType, DisplayFormat, BackBufferFormat, Windowed); }
    int CheckDeviceFormat(unsigned int Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, unsigned int Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat) { return lpVtbl->CheckDeviceFormat(this, Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat); }
    int CheckDeviceMultiSampleType(unsigned int Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, unsigned int Windowed, D3DMULTISAMPLE_TYPE MultiSampleType) { return lpVtbl->CheckDeviceMultiSampleType(this, Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType); }
    int CheckDepthStencilMatch(unsigned int Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) { return lpVtbl->CheckDepthStencilMatch(this, Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat); }
    int GetDeviceCaps(unsigned int Adapter, D3DDEVTYPE DeviceType, void * pCaps) { return lpVtbl->GetDeviceCaps(this, Adapter, DeviceType, pCaps); }
    int GetAdapterMonitor(unsigned int Adapter) { return lpVtbl->GetAdapterMonitor(this, Adapter); }
    int CreateDevice(unsigned int Adapter, D3DDEVTYPE DeviceType, unsigned int hFocusWindow, unsigned int BehaviorFlags, void * pPresentationParameters, IDirect3DDevice8 ** ppReturnedDeviceInterface) { return lpVtbl->CreateDevice(this, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface); }
};
struct IDirect3DVolume8 {
    struct IDirect3DVolume8Vtbl *lpVtbl;
    int QueryInterface(unsigned int riid, void ** ppvObject) { return lpVtbl->QueryInterface(this, riid, ppvObject); }
    int AddRef() { return lpVtbl->AddRef(this); }
    int Release() { return lpVtbl->Release(this); }
    int GetDevice(IDirect3DDevice8 ** ppDevice) { return lpVtbl->GetDevice(this, ppDevice); }
    int SetPrivateData(unsigned int refguid, void * data, unsigned int data_size, unsigned int flags) { return lpVtbl->SetPrivateData(this, refguid, data, data_size, flags); }
    int GetPrivateData(unsigned int refguid, void * pData, void * pSizeOfData) { return lpVtbl->GetPrivateData(this, refguid, pData, pSizeOfData); }
    int FreePrivateData(unsigned int refguid) { return lpVtbl->FreePrivateData(this, refguid); }
    int GetContainer(unsigned int riid, void ** ppContainer) { return lpVtbl->GetContainer(this, riid, ppContainer); }
    int GetDesc(void * pDesc) { return lpVtbl->GetDesc(this, pDesc); }
    int LockBox(D3DLOCKED_BOX *locked_box, void * box, unsigned int flags) { return lpVtbl->LockBox(this, locked_box, box, flags); }
    int UnlockBox() { return lpVtbl->UnlockBox(this); }
};
struct IDirect3DSwapChain8 {
    struct IDirect3DSwapChain8Vtbl *lpVtbl;
    int QueryInterface(unsigned int riid, void ** ppvObject) { return lpVtbl->QueryInterface(this, riid, ppvObject); }
    int AddRef() { return lpVtbl->AddRef(this); }
    int Release() { return lpVtbl->Release(this); }
    int Present(void * src_rect, void * dst_rect, unsigned int dst_window_override, void * dirty_region) { return lpVtbl->Present(this, src_rect, dst_rect, dst_window_override, dirty_region); }
    int GetBackBuffer(unsigned int BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8 ** ppBackBuffer) { return lpVtbl->GetBackBuffer(this, BackBuffer, Type, ppBackBuffer); }
};
struct IDirect3DSurface8 {
    struct IDirect3DSurface8Vtbl *lpVtbl;
    int QueryInterface(unsigned int riid, void ** ppvObject) { return lpVtbl->QueryInterface(this, riid, ppvObject); }
    int AddRef() { return lpVtbl->AddRef(this); }
    int Release() { return lpVtbl->Release(this); }
    int GetDevice(IDirect3DDevice8 ** ppDevice) { return lpVtbl->GetDevice(this, ppDevice); }
    int SetPrivateData(unsigned int refguid, void * data, unsigned int data_size, unsigned int flags) { return lpVtbl->SetPrivateData(this, refguid, data, data_size, flags); }
    int GetPrivateData(unsigned int refguid, void * pData, void * pSizeOfData) { return lpVtbl->GetPrivateData(this, refguid, pData, pSizeOfData); }
    int FreePrivateData(unsigned int refguid) { return lpVtbl->FreePrivateData(this, refguid); }
    int GetContainer(unsigned int riid, void ** ppContainer) { return lpVtbl->GetContainer(this, riid, ppContainer); }
    int GetDesc(void * pDesc) { return lpVtbl->GetDesc(this, pDesc); }
    int LockRect(D3DLOCKED_RECT *locked_rect, void * rect, unsigned int flags) { return lpVtbl->LockRect(this, locked_rect, rect, flags); }
    int UnlockRect() { return lpVtbl->UnlockRect(this); }
};
struct IDirect3DResource8 {
    struct IDirect3DResource8Vtbl *lpVtbl;
    int QueryInterface(unsigned int riid, void ** ppvObject) { return lpVtbl->QueryInterface(this, riid, ppvObject); }
    int AddRef() { return lpVtbl->AddRef(this); }
    int Release() { return lpVtbl->Release(this); }
    int GetDevice(IDirect3DDevice8 ** ppDevice) { return lpVtbl->GetDevice(this, ppDevice); }
    int SetPrivateData(unsigned int refguid, void * data, unsigned int data_size, unsigned int flags) { return lpVtbl->SetPrivateData(this, refguid, data, data_size, flags); }
    int GetPrivateData(unsigned int refguid, void * pData, void * pSizeOfData) { return lpVtbl->GetPrivateData(this, refguid, pData, pSizeOfData); }
    int FreePrivateData(unsigned int refguid) { return lpVtbl->FreePrivateData(this, refguid); }
    int SetPriority(unsigned int PriorityNew) { return lpVtbl->SetPriority(this, PriorityNew); }
    int GetPriority() { return lpVtbl->GetPriority(this); }
    int PreLoad() { return lpVtbl->PreLoad(this); }
    int GetType() { return lpVtbl->GetType(this); }
};
struct IDirect3DVertexBuffer8 {
    struct IDirect3DVertexBuffer8Vtbl *lpVtbl;
    int QueryInterface(unsigned int riid, void ** ppvObject) { return lpVtbl->QueryInterface(this, riid, ppvObject); }
    int AddRef() { return lpVtbl->AddRef(this); }
    int Release() { return lpVtbl->Release(this); }
    int GetDevice(IDirect3DDevice8 ** ppDevice) { return lpVtbl->GetDevice(this, ppDevice); }
    int SetPrivateData(unsigned int refguid, void * data, unsigned int data_size, unsigned int flags) { return lpVtbl->SetPrivateData(this, refguid, data, data_size, flags); }
    int GetPrivateData(unsigned int refguid, void * pData, void * pSizeOfData) { return lpVtbl->GetPrivateData(this, refguid, pData, pSizeOfData); }
    int FreePrivateData(unsigned int refguid) { return lpVtbl->FreePrivateData(this, refguid); }
    int SetPriority(unsigned int PriorityNew) { return lpVtbl->SetPriority(this, PriorityNew); }
    int GetPriority() { return lpVtbl->GetPriority(this); }
    int PreLoad() { return lpVtbl->PreLoad(this); }
    int GetType() { return lpVtbl->GetType(this); }
    int Lock(unsigned int OffsetToLock, unsigned int SizeToLock, void ** ppbData, unsigned int Flags) { return lpVtbl->Lock(this, OffsetToLock, SizeToLock, ppbData, Flags); }
    int Unlock() { return lpVtbl->Unlock(this); }
    int GetDesc(void * pDesc) { return lpVtbl->GetDesc(this, pDesc); }
};
struct IDirect3DIndexBuffer8 {
    struct IDirect3DIndexBuffer8Vtbl *lpVtbl;
    int QueryInterface(unsigned int riid, void ** ppvObject) { return lpVtbl->QueryInterface(this, riid, ppvObject); }
    int AddRef() { return lpVtbl->AddRef(this); }
    int Release() { return lpVtbl->Release(this); }
    int GetDevice(IDirect3DDevice8 ** ppDevice) { return lpVtbl->GetDevice(this, ppDevice); }
    int SetPrivateData(unsigned int refguid, void * data, unsigned int data_size, unsigned int flags) { return lpVtbl->SetPrivateData(this, refguid, data, data_size, flags); }
    int GetPrivateData(unsigned int refguid, void * pData, void * pSizeOfData) { return lpVtbl->GetPrivateData(this, refguid, pData, pSizeOfData); }
    int FreePrivateData(unsigned int refguid) { return lpVtbl->FreePrivateData(this, refguid); }
    int SetPriority(unsigned int PriorityNew) { return lpVtbl->SetPriority(this, PriorityNew); }
    int GetPriority() { return lpVtbl->GetPriority(this); }
    int PreLoad() { return lpVtbl->PreLoad(this); }
    int GetType() { return lpVtbl->GetType(this); }
    int Lock(unsigned int OffsetToLock, unsigned int SizeToLock, void ** ppbData, unsigned int Flags) { return lpVtbl->Lock(this, OffsetToLock, SizeToLock, ppbData, Flags); }
    int Unlock() { return lpVtbl->Unlock(this); }
    int GetDesc(void * pDesc) { return lpVtbl->GetDesc(this, pDesc); }
};
struct IDirect3DBaseTexture8 {
    struct IDirect3DBaseTexture8Vtbl *lpVtbl;
    int QueryInterface(unsigned int riid, void ** ppvObject) { return lpVtbl->QueryInterface(this, riid, ppvObject); }
    int AddRef() { return lpVtbl->AddRef(this); }
    int Release() { return lpVtbl->Release(this); }
    int GetDevice(IDirect3DDevice8 ** ppDevice) { return lpVtbl->GetDevice(this, ppDevice); }
    int SetPrivateData(unsigned int refguid, void * data, unsigned int data_size, unsigned int flags) { return lpVtbl->SetPrivateData(this, refguid, data, data_size, flags); }
    int GetPrivateData(unsigned int refguid, void * pData, void * pSizeOfData) { return lpVtbl->GetPrivateData(this, refguid, pData, pSizeOfData); }
    int FreePrivateData(unsigned int refguid) { return lpVtbl->FreePrivateData(this, refguid); }
    int SetPriority(unsigned int PriorityNew) { return lpVtbl->SetPriority(this, PriorityNew); }
    int GetPriority() { return lpVtbl->GetPriority(this); }
    int PreLoad() { return lpVtbl->PreLoad(this); }
    int GetType() { return lpVtbl->GetType(this); }
    int SetLOD(unsigned int LODNew) { return lpVtbl->SetLOD(this, LODNew); }
    int GetLOD() { return lpVtbl->GetLOD(this); }
    int GetLevelCount() { return lpVtbl->GetLevelCount(this); }
};
struct IDirect3DCubeTexture8 {
    struct IDirect3DCubeTexture8Vtbl *lpVtbl;
    int QueryInterface(unsigned int riid, void ** ppvObject) { return lpVtbl->QueryInterface(this, riid, ppvObject); }
    int AddRef() { return lpVtbl->AddRef(this); }
    int Release() { return lpVtbl->Release(this); }
    int GetDevice(IDirect3DDevice8 ** ppDevice) { return lpVtbl->GetDevice(this, ppDevice); }
    int SetPrivateData(unsigned int refguid, void * data, unsigned int data_size, unsigned int flags) { return lpVtbl->SetPrivateData(this, refguid, data, data_size, flags); }
    int GetPrivateData(unsigned int refguid, void * pData, void * pSizeOfData) { return lpVtbl->GetPrivateData(this, refguid, pData, pSizeOfData); }
    int FreePrivateData(unsigned int refguid) { return lpVtbl->FreePrivateData(this, refguid); }
    int SetPriority(unsigned int PriorityNew) { return lpVtbl->SetPriority(this, PriorityNew); }
    int GetPriority() { return lpVtbl->GetPriority(this); }
    int PreLoad() { return lpVtbl->PreLoad(this); }
    int GetType() { return lpVtbl->GetType(this); }
    int SetLOD(unsigned int LODNew) { return lpVtbl->SetLOD(this, LODNew); }
    int GetLOD() { return lpVtbl->GetLOD(this); }
    int GetLevelCount() { return lpVtbl->GetLevelCount(this); }
    int GetLevelDesc(unsigned int Level, void * pDesc) { return lpVtbl->GetLevelDesc(this, Level, pDesc); }
    int GetCubeMapSurface(unsigned int FaceType, unsigned int Level, IDirect3DSurface8 ** ppCubeMapSurface) { return lpVtbl->GetCubeMapSurface(this, FaceType, Level, ppCubeMapSurface); }
    int LockRect(unsigned int face, unsigned int level, D3DLOCKED_RECT *locked_rect, void * rect, unsigned int flags) { return lpVtbl->LockRect(this, face, level, locked_rect, rect, flags); }
    int UnlockRect(unsigned int FaceType, unsigned int Level) { return lpVtbl->UnlockRect(this, FaceType, Level); }
    int AddDirtyRect(unsigned int face, void * dirty_rect) { return lpVtbl->AddDirtyRect(this, face, dirty_rect); }
};
struct IDirect3DTexture8 {
    struct IDirect3DTexture8Vtbl *lpVtbl;
    int QueryInterface(unsigned int riid, void ** ppvObject) { return lpVtbl->QueryInterface(this, riid, ppvObject); }
    int AddRef() { return lpVtbl->AddRef(this); }
    int Release() { return lpVtbl->Release(this); }
    int GetDevice(IDirect3DDevice8 ** ppDevice) { return lpVtbl->GetDevice(this, ppDevice); }
    int SetPrivateData(unsigned int refguid, void * data, unsigned int data_size, unsigned int flags) { return lpVtbl->SetPrivateData(this, refguid, data, data_size, flags); }
    int GetPrivateData(unsigned int refguid, void * pData, void * pSizeOfData) { return lpVtbl->GetPrivateData(this, refguid, pData, pSizeOfData); }
    int FreePrivateData(unsigned int refguid) { return lpVtbl->FreePrivateData(this, refguid); }
    int SetPriority(unsigned int PriorityNew) { return lpVtbl->SetPriority(this, PriorityNew); }
    int GetPriority() { return lpVtbl->GetPriority(this); }
    int PreLoad() { return lpVtbl->PreLoad(this); }
    int GetType() { return lpVtbl->GetType(this); }
    int SetLOD(unsigned int LODNew) { return lpVtbl->SetLOD(this, LODNew); }
    int GetLOD() { return lpVtbl->GetLOD(this); }
    int GetLevelCount() { return lpVtbl->GetLevelCount(this); }
    int GetLevelDesc(unsigned int Level, void * pDesc) { return lpVtbl->GetLevelDesc(this, Level, pDesc); }
    int GetSurfaceLevel(unsigned int Level, IDirect3DSurface8 ** ppSurfaceLevel) { return lpVtbl->GetSurfaceLevel(this, Level, ppSurfaceLevel); }
    int LockRect(unsigned int level, D3DLOCKED_RECT *locked_rect, void * rect, unsigned int flags) { return lpVtbl->LockRect(this, level, locked_rect, rect, flags); }
    int UnlockRect(unsigned int Level) { return lpVtbl->UnlockRect(this, Level); }
    int AddDirtyRect(void * dirty_rect) { return lpVtbl->AddDirtyRect(this, dirty_rect); }
};
struct IDirect3DVolumeTexture8 {
    struct IDirect3DVolumeTexture8Vtbl *lpVtbl;
    int QueryInterface(unsigned int riid, void ** ppvObject) { return lpVtbl->QueryInterface(this, riid, ppvObject); }
    int AddRef() { return lpVtbl->AddRef(this); }
    int Release() { return lpVtbl->Release(this); }
    int GetDevice(IDirect3DDevice8 ** ppDevice) { return lpVtbl->GetDevice(this, ppDevice); }
    int SetPrivateData(unsigned int refguid, void * data, unsigned int data_size, unsigned int flags) { return lpVtbl->SetPrivateData(this, refguid, data, data_size, flags); }
    int GetPrivateData(unsigned int refguid, void * pData, void * pSizeOfData) { return lpVtbl->GetPrivateData(this, refguid, pData, pSizeOfData); }
    int FreePrivateData(unsigned int refguid) { return lpVtbl->FreePrivateData(this, refguid); }
    int SetPriority(unsigned int PriorityNew) { return lpVtbl->SetPriority(this, PriorityNew); }
    int GetPriority() { return lpVtbl->GetPriority(this); }
    int PreLoad() { return lpVtbl->PreLoad(this); }
    int GetType() { return lpVtbl->GetType(this); }
    int SetLOD(unsigned int LODNew) { return lpVtbl->SetLOD(this, LODNew); }
    int GetLOD() { return lpVtbl->GetLOD(this); }
    int GetLevelCount() { return lpVtbl->GetLevelCount(this); }
    int GetLevelDesc(unsigned int Level, void * pDesc) { return lpVtbl->GetLevelDesc(this, Level, pDesc); }
    int GetVolumeLevel(unsigned int Level, IDirect3DVolume8 ** ppVolumeLevel) { return lpVtbl->GetVolumeLevel(this, Level, ppVolumeLevel); }
    int LockBox(unsigned int level, D3DLOCKED_BOX *locked_box, void * box, unsigned int flags) { return lpVtbl->LockBox(this, level, locked_box, box, flags); }
    int UnlockBox(unsigned int Level) { return lpVtbl->UnlockBox(this, Level); }
    int AddDirtyBox(void * dirty_box) { return lpVtbl->AddDirtyBox(this, dirty_box); }
};
struct IDirect3DDevice8 {
    struct IDirect3DDevice8Vtbl *lpVtbl;
    int QueryInterface(unsigned int riid, void ** ppvObject) { return lpVtbl->QueryInterface(this, riid, ppvObject); }
    int AddRef() { return lpVtbl->AddRef(this); }
    int Release() { return lpVtbl->Release(this); }
    int TestCooperativeLevel() { return lpVtbl->TestCooperativeLevel(this); }
    int GetAvailableTextureMem() { return lpVtbl->GetAvailableTextureMem(this); }
    int ResourceManagerDiscardBytes(unsigned int Bytes) { return lpVtbl->ResourceManagerDiscardBytes(this, Bytes); }
    int GetDirect3D(IDirect3D8 ** ppD3D8) { return lpVtbl->GetDirect3D(this, ppD3D8); }
    int GetDeviceCaps(void * pCaps) { return lpVtbl->GetDeviceCaps(this, pCaps); }
    int GetDisplayMode(void * pMode) { return lpVtbl->GetDisplayMode(this, pMode); }
    int GetCreationParameters(void * pParameters) { return lpVtbl->GetCreationParameters(this, pParameters); }
    int SetCursorProperties(unsigned int XHotSpot, unsigned int YHotSpot, IDirect3DSurface8 * pCursorBitmap) { return lpVtbl->SetCursorProperties(this, XHotSpot, YHotSpot, pCursorBitmap); }
    int SetCursorPosition(unsigned int XScreenSpace, unsigned int YScreenSpace, unsigned int Flags) { return lpVtbl->SetCursorPosition(this, XScreenSpace, YScreenSpace, Flags); }
    int ShowCursor(unsigned int bShow) { return lpVtbl->ShowCursor(this, bShow); }
    int CreateAdditionalSwapChain(void * pPresentationParameters, IDirect3DSwapChain8 ** pSwapChain) { return lpVtbl->CreateAdditionalSwapChain(this, pPresentationParameters, pSwapChain); }
    int Reset(void * pPresentationParameters) { return lpVtbl->Reset(this, pPresentationParameters); }
    int Present(void * src_rect, void * dst_rect, unsigned int dst_window_override, void * dirty_region) { return lpVtbl->Present(this, src_rect, dst_rect, dst_window_override, dirty_region); }
    int GetBackBuffer(unsigned int BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8 ** ppBackBuffer) { return lpVtbl->GetBackBuffer(this, BackBuffer, Type, ppBackBuffer); }
    int GetRasterStatus(void * pRasterStatus) { return lpVtbl->GetRasterStatus(this, pRasterStatus); }
    int SetGammaRamp(unsigned int flags, void * ramp) { return lpVtbl->SetGammaRamp(this, flags, ramp); }
    int GetGammaRamp(void * pRamp) { return lpVtbl->GetGammaRamp(this, pRamp); }
    int CreateTexture(unsigned int Width, unsigned int Height, unsigned int Levels, unsigned int Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8 ** ppTexture) { return lpVtbl->CreateTexture(this, Width, Height, Levels, Usage, Format, Pool, ppTexture); }
    int CreateVolumeTexture(unsigned int Width, unsigned int Height, unsigned int Depth, unsigned int Levels, unsigned int Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture8 ** ppVolumeTexture) { return lpVtbl->CreateVolumeTexture(this, Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture); }
    int CreateCubeTexture(unsigned int EdgeLength, unsigned int Levels, unsigned int Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture8 ** ppCubeTexture) { return lpVtbl->CreateCubeTexture(this, EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture); }
    int CreateVertexBuffer(unsigned int Length, unsigned int Usage, unsigned int FVF, D3DPOOL Pool, IDirect3DVertexBuffer8 ** ppVertexBuffer) { return lpVtbl->CreateVertexBuffer(this, Length, Usage, FVF, Pool, ppVertexBuffer); }
    int CreateIndexBuffer(unsigned int Length, unsigned int Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer8 ** ppIndexBuffer) { return lpVtbl->CreateIndexBuffer(this, Length, Usage, Format, Pool, ppIndexBuffer); }
    int CreateRenderTarget(unsigned int Width, unsigned int Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, unsigned int Lockable, IDirect3DSurface8 ** ppSurface) { return lpVtbl->CreateRenderTarget(this, Width, Height, Format, MultiSample, Lockable, ppSurface); }
    int CreateDepthStencilSurface(unsigned int Width, unsigned int Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface8 ** ppSurface) { return lpVtbl->CreateDepthStencilSurface(this, Width, Height, Format, MultiSample, ppSurface); }
    int CreateImageSurface(unsigned int Width, unsigned int Height, D3DFORMAT Format, IDirect3DSurface8 ** ppSurface) { return lpVtbl->CreateImageSurface(this, Width, Height, Format, ppSurface); }
    int CopyRects(IDirect3DSurface8 * src_surface, void * src_rects, unsigned int rect_count, IDirect3DSurface8 * dst_surface, void * dst_points) { return lpVtbl->CopyRects(this, src_surface, src_rects, rect_count, dst_surface, dst_points); }
    int UpdateTexture(IDirect3DBaseTexture8 * pSourceTexture, IDirect3DBaseTexture8 * pDestinationTexture) { return lpVtbl->UpdateTexture(this, pSourceTexture, pDestinationTexture); }
    int GetFrontBuffer(IDirect3DSurface8 * pDestSurface) { return lpVtbl->GetFrontBuffer(this, pDestSurface); }
    int SetRenderTarget(IDirect3DSurface8 * pRenderTarget, IDirect3DSurface8 * pNewZStencil) { return lpVtbl->SetRenderTarget(this, pRenderTarget, pNewZStencil); }
    int GetRenderTarget(IDirect3DSurface8 ** ppRenderTarget) { return lpVtbl->GetRenderTarget(this, ppRenderTarget); }
    int GetDepthStencilSurface(IDirect3DSurface8 ** ppZStencilSurface) { return lpVtbl->GetDepthStencilSurface(this, ppZStencilSurface); }
    int BeginScene() { return lpVtbl->BeginScene(this); }
    int EndScene() { return lpVtbl->EndScene(this); }
    int Clear(unsigned int rect_count, void * rects, unsigned int flags, unsigned int color, float z, unsigned int stencil) { return lpVtbl->Clear(this, rect_count, rects, flags, color, z, stencil); }
    int SetTransform(D3DTRANSFORMSTATETYPE state, void * matrix) { return lpVtbl->SetTransform(this, state, matrix); }
    int GetTransform(D3DTRANSFORMSTATETYPE State, void * pMatrix) { return lpVtbl->GetTransform(this, State, pMatrix); }
    int MultiplyTransform(D3DTRANSFORMSTATETYPE state, void * matrix) { return lpVtbl->MultiplyTransform(this, state, matrix); }
    int SetViewport(void * viewport) { return lpVtbl->SetViewport(this, viewport); }
    int GetViewport(void * pViewport) { return lpVtbl->GetViewport(this, pViewport); }
    int SetMaterial(void * material) { return lpVtbl->SetMaterial(this, material); }
    int GetMaterial(void * pMaterial) { return lpVtbl->GetMaterial(this, pMaterial); }
    int SetLight(unsigned int index, void * light) { return lpVtbl->SetLight(this, index, light); }
    int GetLight(unsigned int Index, void * pLight) { return lpVtbl->GetLight(this, Index, pLight); }
    int LightEnable(unsigned int Index, unsigned int Enable) { return lpVtbl->LightEnable(this, Index, Enable); }
    int GetLightEnable(unsigned int Index, void * pEnable) { return lpVtbl->GetLightEnable(this, Index, pEnable); }
    int SetClipPlane(unsigned int index, void * plane) { return lpVtbl->SetClipPlane(this, index, plane); }
    int GetClipPlane(unsigned int Index, void * pPlane) { return lpVtbl->GetClipPlane(this, Index, pPlane); }
    int SetRenderState(D3DRENDERSTATETYPE State, unsigned int Value) { return lpVtbl->SetRenderState(this, State, Value); }
    int GetRenderState(D3DRENDERSTATETYPE State, void * pValue) { return lpVtbl->GetRenderState(this, State, pValue); }
    int BeginStateBlock() { return lpVtbl->BeginStateBlock(this); }
    int EndStateBlock(void * pToken) { return lpVtbl->EndStateBlock(this, pToken); }
    int ApplyStateBlock(unsigned int Token) { return lpVtbl->ApplyStateBlock(this, Token); }
    int CaptureStateBlock(unsigned int Token) { return lpVtbl->CaptureStateBlock(this, Token); }
    int DeleteStateBlock(unsigned int Token) { return lpVtbl->DeleteStateBlock(this, Token); }
    int CreateStateBlock(D3DSTATEBLOCKTYPE Type, void * pToken) { return lpVtbl->CreateStateBlock(this, Type, pToken); }
    int SetClipStatus(void * clip_status) { return lpVtbl->SetClipStatus(this, clip_status); }
    int GetClipStatus(void * pClipStatus) { return lpVtbl->GetClipStatus(this, pClipStatus); }
    int GetTexture(unsigned int Stage, IDirect3DBaseTexture8 ** ppTexture) { return lpVtbl->GetTexture(this, Stage, ppTexture); }
    int SetTexture(unsigned int Stage, IDirect3DBaseTexture8 * pTexture) { return lpVtbl->SetTexture(this, Stage, pTexture); }
    int GetTextureStageState(unsigned int Stage, D3DTEXTURESTAGESTATETYPE Type, void * pValue) { return lpVtbl->GetTextureStageState(this, Stage, Type, pValue); }
    int SetTextureStageState(unsigned int Stage, D3DTEXTURESTAGESTATETYPE Type, unsigned int Value) { return lpVtbl->SetTextureStageState(this, Stage, Type, Value); }
    int ValidateDevice(void * pNumPasses) { return lpVtbl->ValidateDevice(this, pNumPasses); }
    int GetInfo(unsigned int DevInfoID, void * pDevInfoStruct, unsigned int DevInfoStructSize) { return lpVtbl->GetInfo(this, DevInfoID, pDevInfoStruct, DevInfoStructSize); }
    int SetPaletteEntries(unsigned int palette_idx, void * entries) { return lpVtbl->SetPaletteEntries(this, palette_idx, entries); }
    int GetPaletteEntries(unsigned int PaletteNumber, void * pEntries) { return lpVtbl->GetPaletteEntries(this, PaletteNumber, pEntries); }
    int SetCurrentTexturePalette(unsigned int PaletteNumber) { return lpVtbl->SetCurrentTexturePalette(this, PaletteNumber); }
    int GetCurrentTexturePalette(void * PaletteNumber) { return lpVtbl->GetCurrentTexturePalette(this, PaletteNumber); }
    int DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, unsigned int StartVertex, unsigned int PrimitiveCount) { return lpVtbl->DrawPrimitive(this, PrimitiveType, StartVertex, PrimitiveCount); }
    int DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, unsigned int minIndex, unsigned int NumVertices, unsigned int startIndex, unsigned int primCount) { return lpVtbl->DrawIndexedPrimitive(this, PrimitiveType, minIndex, NumVertices, startIndex, primCount); }
    int DrawPrimitiveUP(D3DPRIMITIVETYPE primitive_type, unsigned int primitive_count, void * data, unsigned int stride) { return lpVtbl->DrawPrimitiveUP(this, primitive_type, primitive_count, data, stride); }
    int DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE primitive_type, unsigned int min_vertex_idx, unsigned int vertex_count, unsigned int primitive_count, void * index_data, D3DFORMAT index_format, void * data, unsigned int stride) { return lpVtbl->DrawIndexedPrimitiveUP(this, primitive_type, min_vertex_idx, vertex_count, primitive_count, index_data, index_format, data, stride); }
    int ProcessVertices(unsigned int SrcStartIndex, unsigned int DestIndex, unsigned int VertexCount, IDirect3DVertexBuffer8 * pDestBuffer, unsigned int Flags) { return lpVtbl->ProcessVertices(this, SrcStartIndex, DestIndex, VertexCount, pDestBuffer, Flags); }
    int CreateVertexShader(void * declaration, void * byte_code, void * shader, unsigned int usage) { return lpVtbl->CreateVertexShader(this, declaration, byte_code, shader, usage); }
    int SetVertexShader(unsigned int Handle) { return lpVtbl->SetVertexShader(this, Handle); }
    int GetVertexShader(void * pHandle) { return lpVtbl->GetVertexShader(this, pHandle); }
    int DeleteVertexShader(unsigned int Handle) { return lpVtbl->DeleteVertexShader(this, Handle); }
    int SetVertexShaderConstant(unsigned int reg_idx, void * data, unsigned int count) { return lpVtbl->SetVertexShaderConstant(this, reg_idx, data, count); }
    int GetVertexShaderConstant(unsigned int Register, void * pConstantData, unsigned int ConstantCount) { return lpVtbl->GetVertexShaderConstant(this, Register, pConstantData, ConstantCount); }
    int GetVertexShaderDeclaration(unsigned int Handle, void * pData, void * pSizeOfData) { return lpVtbl->GetVertexShaderDeclaration(this, Handle, pData, pSizeOfData); }
    int GetVertexShaderFunction(unsigned int Handle, void * pData, void * pSizeOfData) { return lpVtbl->GetVertexShaderFunction(this, Handle, pData, pSizeOfData); }
    int SetStreamSource(unsigned int StreamNumber, IDirect3DVertexBuffer8 * pStreamData, unsigned int Stride) { return lpVtbl->SetStreamSource(this, StreamNumber, pStreamData, Stride); }
    int GetStreamSource(unsigned int StreamNumber, IDirect3DVertexBuffer8 ** ppStreamData, void * pStride) { return lpVtbl->GetStreamSource(this, StreamNumber, ppStreamData, pStride); }
    int SetIndices(IDirect3DIndexBuffer8 * pIndexData, unsigned int BaseVertexIndex) { return lpVtbl->SetIndices(this, pIndexData, BaseVertexIndex); }
    int GetIndices(IDirect3DIndexBuffer8 ** ppIndexData, void * pBaseVertexIndex) { return lpVtbl->GetIndices(this, ppIndexData, pBaseVertexIndex); }
    int CreatePixelShader(void * byte_code, void * shader) { return lpVtbl->CreatePixelShader(this, byte_code, shader); }
    int SetPixelShader(unsigned int Handle) { return lpVtbl->SetPixelShader(this, Handle); }
    int GetPixelShader(void * pHandle) { return lpVtbl->GetPixelShader(this, pHandle); }
    int DeletePixelShader(unsigned int Handle) { return lpVtbl->DeletePixelShader(this, Handle); }
    int SetPixelShaderConstant(unsigned int reg_idx, void * data, unsigned int count) { return lpVtbl->SetPixelShaderConstant(this, reg_idx, data, count); }
    int GetPixelShaderConstant(unsigned int Register, void * pConstantData, unsigned int ConstantCount) { return lpVtbl->GetPixelShaderConstant(this, Register, pConstantData, ConstantCount); }
    int GetPixelShaderFunction(unsigned int Handle, void * pData, void * pSizeOfData) { return lpVtbl->GetPixelShaderFunction(this, Handle, pData, pSizeOfData); }
    int DrawRectPatch(unsigned int handle, void * segment_count, void * patch_info) { return lpVtbl->DrawRectPatch(this, handle, segment_count, patch_info); }
    int DrawTriPatch(unsigned int handle, void * segment_count, void * patch_info) { return lpVtbl->DrawTriPatch(this, handle, segment_count, patch_info); }
    int DeletePatch(unsigned int Handle) { return lpVtbl->DeletePatch(this, Handle); }
};

// value enums for render / texture-stage states (from MinGW-w64 d3d8types.h)
#ifndef TRUE
#define TRUE 1
typedef D3DFORMAT _D3DFORMAT;
#define FALSE 0
#endif
enum D3DZBUFFERTYPE {
  D3DZB_FALSE                 = 0,
  D3DZB_TRUE                  = 1,
  D3DZB_USEW                  = 2
};
enum D3DFILLMODE {
  D3DFILL_POINT               = 1,
  D3DFILL_WIREFRAME           = 2,
  D3DFILL_SOLID               = 3
};
enum D3DSHADEMODE {
  D3DSHADE_FLAT               = 1,
  D3DSHADE_GOURAUD            = 2,
  D3DSHADE_PHONG              = 3
};
enum D3DBLEND {
  D3DBLEND_ZERO               =  1,
  D3DBLEND_ONE                =  2,
  D3DBLEND_SRCCOLOR           =  3,
  D3DBLEND_INVSRCCOLOR        =  4,
  D3DBLEND_SRCALPHA           =  5,
  D3DBLEND_INVSRCALPHA        =  6,
  D3DBLEND_DESTALPHA          =  7,
  D3DBLEND_INVDESTALPHA       =  8,
  D3DBLEND_DESTCOLOR          =  9,
  D3DBLEND_INVDESTCOLOR       = 10,
  D3DBLEND_SRCALPHASAT        = 11,
  D3DBLEND_BOTHSRCALPHA       = 12,
  D3DBLEND_BOTHINVSRCALPHA    = 13
};
enum D3DCULL {
  D3DCULL_NONE                = 1,
  D3DCULL_CW                  = 2,
  D3DCULL_CCW                 = 3
};
enum D3DCMPFUNC {
  D3DCMP_NEVER                = 1,
  D3DCMP_LESS                 = 2,
  D3DCMP_EQUAL                = 3,
  D3DCMP_LESSEQUAL            = 4,
  D3DCMP_GREATER              = 5,
  D3DCMP_NOTEQUAL             = 6,
  D3DCMP_GREATEREQUAL         = 7,
  D3DCMP_ALWAYS               = 8
};
enum D3DFOGMODE {
  D3DFOG_NONE                 = 0,
  D3DFOG_EXP                  = 1,
  D3DFOG_EXP2                 = 2,
  D3DFOG_LINEAR               = 3
};
enum D3DBLENDOP {
  D3DBLENDOP_ADD              = 1,
  D3DBLENDOP_SUBTRACT         = 2,
  D3DBLENDOP_REVSUBTRACT      = 3,
  D3DBLENDOP_MIN              = 4,
  D3DBLENDOP_MAX              = 5
};
enum D3DSTENCILOP {
  D3DSTENCILOP_KEEP           = 1,
  D3DSTENCILOP_ZERO           = 2,
  D3DSTENCILOP_REPLACE        = 3,
  D3DSTENCILOP_INCRSAT        = 4,
  D3DSTENCILOP_DECRSAT        = 5,
  D3DSTENCILOP_INVERT         = 6,
  D3DSTENCILOP_INCR           = 7,
  D3DSTENCILOP_DECR           = 8
};
enum D3DTEXTUREOP {
  D3DTOP_DISABLE                   =  1,
  D3DTOP_SELECTARG1                =  2,
  D3DTOP_SELECTARG2                =  3,
  D3DTOP_MODULATE                  =  4,
  D3DTOP_MODULATE2X                =  5,
  D3DTOP_MODULATE4X                =  6,
  D3DTOP_ADD                       =  7,
  D3DTOP_ADDSIGNED                 =  8,
  D3DTOP_ADDSIGNED2X               =  9,
  D3DTOP_SUBTRACT                  = 10,
  D3DTOP_ADDSMOOTH                 = 11,
  D3DTOP_BLENDDIFFUSEALPHA         = 12,
  D3DTOP_BLENDTEXTUREALPHA         = 13,
  D3DTOP_BLENDFACTORALPHA          = 14,
  D3DTOP_BLENDTEXTUREALPHAPM       = 15,
  D3DTOP_BLENDCURRENTALPHA         = 16,
  D3DTOP_PREMODULATE               = 17,
  D3DTOP_MODULATEALPHA_ADDCOLOR    = 18,
  D3DTOP_MODULATECOLOR_ADDALPHA    = 19,
  D3DTOP_MODULATEINVALPHA_ADDCOLOR = 20,
  D3DTOP_MODULATEINVCOLOR_ADDALPHA = 21,
  D3DTOP_BUMPENVMAP                = 22,
  D3DTOP_BUMPENVMAPLUMINANCE       = 23,
  D3DTOP_DOTPRODUCT3               = 24,
  D3DTOP_MULTIPLYADD               = 25,
  D3DTOP_LERP                      = 26
};
enum D3DTEXTUREADDRESS {
  D3DTADDRESS_WRAP            = 1,
  D3DTADDRESS_MIRROR          = 2,
  D3DTADDRESS_CLAMP           = 3,
  D3DTADDRESS_BORDER          = 4,
  D3DTADDRESS_MIRRORONCE      = 5
};
enum D3DTEXTUREFILTERTYPE {
  D3DTEXF_NONE            = 0,
  D3DTEXF_POINT           = 1,
  D3DTEXF_LINEAR          = 2,
  D3DTEXF_ANISOTROPIC     = 3,
  D3DTEXF_FLATCUBIC       = 4,
  D3DTEXF_GAUSSIANCUBIC   = 5
};
#define D3DTA_SELECTMASK 0xF
#define D3DTA_DIFFUSE 0x0
#define D3DTA_CURRENT 0x1
#define D3DTA_TEXTURE 0x2
#define D3DTA_TFACTOR 0x3
#define D3DTA_SPECULAR 0x4
#define D3DTA_TEMP 0x5
#define D3DTA_COMPLEMENT 0x10
#define D3DTA_ALPHAREPLICATE 0x20

// Formats MinGW's d3d8types.h (and so the IDB's enum) has but the DirectX 8.1 header does not
#ifndef XI_HAVE_ABGR_FORMATS
static const D3DFORMAT D3DFMT_A8B8G8R8 = (D3DFORMAT)32;
static const D3DFORMAT D3DFMT_X8B8G8R8 = (D3DFORMAT)33;
#endif
// Decompiled loops step D3D enums with ++/--, which C allows and C++ does not
#define XI_ENUM_STEP(E) \
  static inline E &operator++(E &e) { e = (E)((int)e + 1); return e; } \
  static inline E operator++(E &e, int) { E o = e; e = (E)((int)e + 1); return o; } \
  static inline E &operator--(E &e) { e = (E)((int)e - 1); return e; } \
  static inline E operator--(E &e, int) { E o = e; e = (E)((int)e - 1); return o; }
XI_ENUM_STEP(D3DFORMAT)
XI_ENUM_STEP(D3DTEXTURESTAGESTATETYPE)
XI_ENUM_STEP(D3DRENDERSTATETYPE)
XI_ENUM_STEP(D3DTRANSFORMSTATETYPE)
XI_ENUM_STEP(D3DPRIMITIVETYPE)
#undef XI_ENUM_STEP
/* ---- end of xi_d3d8_vc6.h ---- */
struct IDirect3DDevice8; typedef struct IDirect3DDevice8 IDirect3DDevice8;
extern float xi_g_1034FDF0;
#define g_fadeQuadVerts xi_g_1034FDF0
extern int xi_g_1034FE00;
#define g_fadeQuadVerts_v0_color xi_g_1034FE00
extern float xi_g_1034FDF4;
#define g_fadeQuadVerts_v0_y xi_g_1034FDF4
extern int xi_g_1034FE14;
#define g_fadeQuadVerts_v1_color xi_g_1034FE14
extern float xi_g_1034FE04;
#define g_fadeQuadVerts_v1_x xi_g_1034FE04
extern float xi_g_1034FE08;
#define g_fadeQuadVerts_v1_y xi_g_1034FE08
extern int xi_g_1034FE28;
#define g_fadeQuadVerts_v2_color xi_g_1034FE28
extern float xi_g_1034FE18;
#define g_fadeQuadVerts_v2_x xi_g_1034FE18
extern float xi_g_1034FE1C;
#define g_fadeQuadVerts_v2_y xi_g_1034FE1C
extern int xi_g_1034FE3C;
#define g_fadeQuadVerts_v3_color xi_g_1034FE3C
extern float xi_g_1034FE2C;
#define g_fadeQuadVerts_v3_x xi_g_1034FE2C
extern float xi_g_1034FE30;
#define g_fadeQuadVerts_v3_y xi_g_1034FE30
struct FUNt_10009920 { int f(void); };
struct FUNt_10009E00 { unsigned __int8 * f(void); };
struct FUNt_1000CD00 { int f(int, int, int, int); };
struct XS { int DrawFadeQuad(int a1, unsigned int a2); };
// FUNCTION: FFXIMAIN 0x10009E00
unsigned __int8 *FUNt_10009E00::f(void)
{
  return (unsigned __int8 *)this + 0x98 + *((unsigned __int8 *)this + 0x158) * 24;
}
// FUNCTION: FFXIMAIN 0x1000A820
int XS::DrawFadeQuad(int a1, unsigned int a2)
{
  void *self_ = this;

  IDirect3DDevice8 *pDevice;
  IDirect3DDevice8 v6;
  ((FUNt_10009920 *)(this))->f();
  if ( a2 != 0x80808080 )
  {
    if ( (a2 & 0xFF000000) == 0x80000000 )
      a2 = ((unsigned __int8)a2 * ((a2 >> 7) & 0x1FE0000)) & 0xFF000000;
    pDevice = (*(IDirect3DDevice8 * *)((char *)this + 0xC));
    if ((a2 & 0xFF000000) > 0x80000000)
    {
        v6.lpVtbl = pDevice->lpVtbl;
        v6.lpVtbl->SetRenderState(pDevice, D3DRS_SRCBLEND, 5u);
        (*(IDirect3DDevice8 * *)((char *)this + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)this + 0xC)), D3DRS_DESTBLEND, 2u);
    }
    else
    {
        v6.lpVtbl = pDevice->lpVtbl;
        v6.lpVtbl->SetRenderState(pDevice, D3DRS_SRCBLEND, 6u);
        (*(IDirect3DDevice8 * *)((char *)this + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)this + 0xC)), D3DRS_DESTBLEND, 5u);
        a2 &= 0xFF000000;
    }
    g_fadeQuadVerts_v1_color = g_fadeQuadVerts_v0_color = a2;
    g_fadeQuadVerts_v2_color = a2;
    g_fadeQuadVerts_v3_color = a2;
    g_fadeQuadVerts = (float)*(unsigned int *)((FUNt_10009E00 *)((unsigned __int8 *)this))->f();
    float tmp1 = (float)*(unsigned int *)((FUNt_10009E00 *)((unsigned __int8 *)this))->f();
    float & tmp = tmp1;
    g_fadeQuadVerts_v2_x = tmp;
    g_fadeQuadVerts_v1_x = (float)*((unsigned int *)((FUNt_10009E00 *)((unsigned __int8 *)this))->f() + 2);
    g_fadeQuadVerts_v3_x = (float)*((unsigned int *)((FUNt_10009E00 *)((unsigned __int8 *)this))->f() + 2);
    g_fadeQuadVerts_v0_y = (float)*((unsigned int *)((FUNt_10009E00 *)((unsigned __int8 *)this))->f() + 1);
    g_fadeQuadVerts_v1_y = (float)*((unsigned int *)((FUNt_10009E00 *)((unsigned __int8 *)this))->f() + 1);
    g_fadeQuadVerts_v2_y = (float)*((unsigned int *)((FUNt_10009E00 *)((unsigned __int8 *)this))->f() + 3);
    g_fadeQuadVerts_v3_y = (float)*((unsigned int *)((FUNt_10009E00 *)((unsigned __int8 *)this))->f() + 3);
    (*(IDirect3DDevice8 * *)((char *)this + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)this + 0xC)), D3DRS_ZENABLE, 0);
    (*(IDirect3DDevice8 * *)((char *)this + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)this + 0xC)), D3DRS_ZWRITEENABLE, 0);
    (*(IDirect3DDevice8 * *)((char *)this + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)this + 0xC)), D3DRS_LIGHTING, 0);
    (*(IDirect3DDevice8 * *)((char *)this + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)this + 0xC)), D3DRS_FOGENABLE, 0);
    (*(IDirect3DDevice8 * *)((char *)this + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)this + 0xC)), D3DRS_ALPHABLENDENABLE, 1u);
    (*(IDirect3DDevice8 * *)((char *)this + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)this + 0xC)), 0, D3DTSS_COLOROP, 2u);
    (*(IDirect3DDevice8 * *)((char *)this + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)this + 0xC)), 0, D3DTSS_COLORARG1, 0);
    (*(IDirect3DDevice8 * *)((char *)this + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)this + 0xC)), 0, D3DTSS_COLORARG2, 1u);
    (*(IDirect3DDevice8 * *)((char *)this + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)this + 0xC)), 0, D3DTSS_ALPHAOP, 2u);
    (*(IDirect3DDevice8 * *)((char *)this + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)this + 0xC)), 0, D3DTSS_ALPHAARG1, 0);
    (*(IDirect3DDevice8 * *)((char *)this + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)this + 0xC)), 0, D3DTSS_ALPHAARG2, 1u);
    (*(IDirect3DDevice8 * *)((char *)this + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)this + 0xC)), 1u, D3DTSS_COLOROP, 1u);
    (*(IDirect3DDevice8 * *)((char *)this + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)this + 0xC)), 1u, D3DTSS_ALPHAOP, 1u);
    (*(IDirect3DDevice8 * *)((char *)this + 0xC))->lpVtbl->SetVertexShader((*(IDirect3DDevice8 * *)((char *)this + 0xC)), 68u);
    ((FUNt_1000CD00 *)(this))->f(5, 2, (int)&g_fadeQuadVerts, 20);
  }
  return ((FUNt_10009920 *)(this))->f();

}
