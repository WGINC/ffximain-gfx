// Game types exported from the IDB (IDA print_decls); D3D types come from xi_d3d8.hpp.
#pragma once
struct _RTL_CRITICAL_SECTION_DEBUG;
struct tagBITMAPINFO;
struct _iobuf;
struct IDirect3D8;
struct IDirect3DVolume8;
struct IDirect3DSwapChain8;
struct IDirect3DSurface8;
struct IDirect3DVertexBuffer8;
struct IDirect3DIndexBuffer8;
struct IDirect3DBaseTexture8;
struct IDirect3DCubeTexture8;
struct IDirect3DTexture8;
struct IDirect3DVolumeTexture8;
struct IDirect3DDevice8;
struct IDirect3D8Vtbl;
struct IDirect3DVolume8Vtbl;
struct IDirect3DSwapChain8Vtbl;
struct IDirect3DSurface8Vtbl;
struct IDirect3DVertexBuffer8Vtbl;
struct IDirect3DIndexBuffer8Vtbl;
struct IDirect3DBaseTexture8Vtbl;
struct IDirect3DCubeTexture8Vtbl;
struct IDirect3DTexture8Vtbl;
struct IDirect3DVolumeTexture8Vtbl;
struct IDirect3DDevice8Vtbl;
struct D3DLOCKED_RECT;
struct D3DLOCKED_BOX;
struct _D3DVIEWPORT9;
struct CDx_vtbl;
struct CMoAttachments_vtbl;
struct CMoCelestial_vtbl;
struct CMoDX_vtbl;
struct CMoDxStatusMng_vtbl;
struct CMoOT_vtbl;
struct CMoOcclusionMng_vtbl;
struct CMoResource_vtbl;
struct CMoResourceMng_vtbl;
struct CMoSpline_vtbl;
struct CMoTask_vtbl;
struct CMoTaskMng_vtbl;
struct CShader_vtbl;
struct CXiOpening_vtbl;
struct CYyModel_vtbl;
struct CYyModelBase_vtbl;
struct CYyModelDt_vtbl;
struct CYyTexBase_vtbl;
struct CYyTexMng_vtbl;

/* 2 */
struct HINSTANCE__
{
  int unused;
};

/* 3 */
typedef unsigned int DWORD;

/* 14 */
typedef unsigned int UINT;

/* 16 */
typedef char CHAR;

/* 19 */
typedef unsigned __int16 wchar_t;

/* 18 */
typedef wchar_t WCHAR;

/* 24 */
typedef unsigned int ULONG_PTR;

/* 27 */
struct HKEY__
{
  int unused;
};

/* 33 */
typedef int BOOL;

/* 36 */
typedef struct _RTL_CRITICAL_SECTION_DEBUG *PRTL_CRITICAL_SECTION_DEBUG;

/* 41 */
typedef int LONG;

/* 42 */
typedef void *HANDLE;

/* 35 */
#pragma pack(push, 8)
struct _RTL_CRITICAL_SECTION
{
  PRTL_CRITICAL_SECTION_DEBUG DebugInfo;
  LONG LockCount;
  LONG RecursionCount;
  HANDLE OwningThread;
  HANDLE LockSemaphore;
  ULONG_PTR SpinCount;
};
#pragma pack(pop)

/* 38 */
typedef unsigned __int16 WORD;

/* 40 */
struct _LIST_ENTRY
{
  struct _LIST_ENTRY *Flink;
  struct _LIST_ENTRY *Blink;
};

/* 39 */
typedef struct _LIST_ENTRY LIST_ENTRY;

/* 37 */
struct _RTL_CRITICAL_SECTION_DEBUG
{
  WORD Type;
  WORD CreatorBackTraceIndex;
  struct _RTL_CRITICAL_SECTION *CriticalSection;
  LIST_ENTRY ProcessLocksList;
  DWORD EntryCount;
  DWORD ContentionCount;
  DWORD Flags;
  WORD CreatorBackTraceIndexHigh;
  WORD SpareWORD;
};

/* 62 */
typedef unsigned __int8 BYTE;

/* 64 */
struct tagRECT
{
  LONG left;
  LONG top;
  LONG right;
  LONG bottom;
};

/* 65 */
typedef struct tagBITMAPINFO BITMAPINFO;

/* 68 */
struct tagBITMAPINFOHEADER
{
  DWORD biSize;
  LONG biWidth;
  LONG biHeight;
  WORD biPlanes;
  WORD biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG biXPelsPerMeter;
  LONG biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
};

/* 67 */
typedef struct tagBITMAPINFOHEADER BITMAPINFOHEADER;

/* 70 */
struct tagRGBQUAD
{
  BYTE rgbBlue;
  BYTE rgbGreen;
  BYTE rgbRed;
  BYTE rgbReserved;
};

/* 69 */
typedef struct tagRGBQUAD RGBQUAD;

/* 66 */
struct tagBITMAPINFO
{
  BITMAPINFOHEADER bmiHeader;
  RGBQUAD bmiColors[1];
};

/* 73 */
struct _SYSTEMTIME
{
  WORD wYear;
  WORD wMonth;
  WORD wDayOfWeek;
  WORD wDay;
  WORD wHour;
  WORD wMinute;
  WORD wSecond;
  WORD wMilliseconds;
};

/* 88 */
typedef unsigned int size_t;

/* 115 */
typedef struct _iobuf FILE;

/* 116 */
struct _iobuf
{
  char *_ptr;
  int _cnt;
  char *_base;
  int _flag;
  int _file;
  int _charbuf;
  int _bufsiz;
  char *_tmpfname;
};

/* 49 */
typedef unsigned int UINT_PTR;

/* 119 */
typedef UINT_PTR WPARAM;

/* 121 */
typedef int LONG_PTR;

/* 120 */
typedef LONG_PTR LPARAM;

/* 127 */
struct HWND__
{
  int unused;
};

/* 126 */
typedef struct HWND__ *HWND;

/* 130 */
struct tagPOINT
{
  LONG x;
  LONG y;
};

/* 129 */
typedef struct tagPOINT POINT;

/* 128 */
struct tagMSG
{
  HWND hwnd;
  UINT message;
  WPARAM wParam;
  LPARAM lParam;
  DWORD time;
  POINT pt;
};

/* 153 */
typedef int HRESULT;

/* 159 */
typedef LONG_PTR LRESULT;

/* 161 */
struct HICON__
{
  int unused;
};

/* 170 */
struct HBITMAP__
{
  int unused;
};

/* 182 */
typedef ULONG_PTR DWORD_PTR;

/* 222 */
#pragma pack(push, 8)
struct tm
{
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
};
#pragma pack(pop)

/* 234 */
struct CMoProcessor
{
  void *vftable;
  float ScratchMatrices[512];
  unsigned __int8 pad_0804[12];
  float RotationMatrix[64];
  unsigned __int8 pad_0910[256];
  float *ScratchMatrixFreeList[32];
  int ScratchMatrixStackTop;
  unsigned __int8 pad_0A94[12];
  float TransposeScratch[16];
  float TransposeScratchTemp[16];
  unsigned __int8 pad_0B20[144];
};

/* 235 */
struct CXiSkeletonActorRes
{
  unsigned __int8 CMoTask_base[52];
  void *pLinkedActor;
  void *pfnOnLoaded;
  int loadState;
  int field_40;
  int resourceId;
};

/* 236 */
struct CXiSkeletonActor
{
  char gap01_000[136];
  int posHandle;
  char gap02_08c[8];
  float moveSpeedBase;
  float moveSpeed;
  float walkSpeed;
  char gap03_0a0[16];
  char hiddenFlagByte;
  char gap04_0b1;
  __int16 hiddenFlagWord;
  char gap05_0b4[1452];
  unsigned int color;
  char gap06_664[4];
  unsigned int multiTexAlpha;
  unsigned int shadowAlpha;
  char gap07_670[192];
  int emapAlphaMode;
  unsigned int effectFlags;
  char gap08_738[12];
  float distAmount;
  float separateValue;
  unsigned int emapColor;
  void *emapTexture;
  char gap09_754[412];
  char boundUnk0[16];
  float boundRadius;
  char boundUnk1[12];
  char gap10_910[220];
  unsigned int updateSerial;
  unsigned int bucketA;
  unsigned int visibleSampleCount;
  char forceUpdateFlag;
  char gap11_9f9[3];
  unsigned int bucketB;
  char gap12_a00[4];
  unsigned int drawBudgetGate;
};

/* 237 */
#pragma pack(push, 1)
/// recon: tree/include/CYyObject.hpp, sizeof 0x4 from the binary's descriptor
struct CYyObject
{
  void **vftable;
};
#pragma pack(pop)

/* 238 */
#pragma pack(push, 1)
struct CMoTask
{
  CMoTask_vtbl *base;
  float field_4;
  float field_8;
  float field_C;
  unsigned __int8 _pad_10[36];
};
#pragma pack(pop)

/* 632 */
struct CMoTask_vtbl
{
  void *vf0_CMoTask_GetRuntimeClass_variant12;
  void *vf1_CApp_vf001;
  unsigned int (__thiscall *vf2_sub_1002C930)(_DWORD *this, char);
  int (__thiscall *vf3_sub_1002C920)(_DWORD *this);
  void *vf4_CApp_vf004;
  void *vf5_CApp_vf005;
  _DWORD *(__thiscall *vf6_CMoTask_ScalarDeletingDtor)(CMoTask *C, char);
  void *vf7_CMoTask_vf007;
  void *vf8_CMoTask_vf008;
  void *vf9_CMoTask_vf009;
  void (__stdcall *vf10_CMoTask_vf010)(int);
};

/* 244 */
#pragma pack(push, 1)
struct CDx
{
  CDx_vtbl *base;
  unsigned __int8 _pad_4[4];
  IDirect3D8 *pD3D;
  IDirect3DDevice8 *pDevice;
  unsigned __int8 _pad_10[476];
  unsigned int field_1EC;
  unsigned int field_1F0;
  unsigned __int8 _pad_10_1F4[2140];
};
#pragma pack(pop)

/* 555 */
struct CDx_vtbl
{
  void *vf0_CDx_GetRuntimeClass;
  void *vf1_CApp_vf001;
  unsigned int (__thiscall *vf2_sub_1002C930)(_DWORD *this, char);
  int (__thiscall *vf3_sub_1002C920)(_DWORD *this);
  void *vf4_CApp_vf004;
  void *vf5_CApp_vf005;
  void *(__thiscall *vf6_CDx_ScalarDeletingDtor)(CDx *C, char);
};

/* 246 */
#pragma pack(push, 1)
/// recon: tree/include/CMoActorColorDriveTask.hpp, sizeof 0x88 from the binary's descriptor
struct CMoActorColorDriveTask
{
  CMoTask base;
  unsigned __int8 _pad_34[84];
};
#pragma pack(pop)

/* 248 */
#pragma pack(push, 1)
struct CMoAttachments
{
  CMoAttachments_vtbl *base;
  unsigned __int8 _pad_4[44];
  unsigned __int32 field_30;
  unsigned __int8 _pad_34[12];
};
#pragma pack(pop)

/* 556 */
struct CMoAttachments_vtbl
{
  void *vf0_CMoAttachments_GetRuntimeClass;
  _DWORD *(__thiscall *vf1_CMoAttachments_ScalarDeletingDtor)(CMoAttachments *this, char);
  int (__thiscall *vf2_sub_1003C720)(_DWORD *this);
  int (__thiscall *vf3_sub_1003C730)(_DWORD *this);
  void *vf4_CMoAttachments_vf004;
  void *vf5_CMoAttachments_vf005;
  void *vf6_CMoAttachments_vf006;
};

/* 249 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032AE94.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032AE94
{
  CMoAttachments base;
};
#pragma pack(pop)

/* 250 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032AF44.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032AF44
{
  CMoAttachments base;
};
#pragma pack(pop)

/* 252 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032B140.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032B140
{
  CMoAttachments base;
};
#pragma pack(pop)

/* 253 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032B940.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032B940
{
  CMoAttachments base;
};
#pragma pack(pop)

/* 254 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032BAF0.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032BAF0
{
  CMoAttachments base;
};
#pragma pack(pop)

/* 255 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032BB38.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032BB38
{
  CMoAttachments base;
};
#pragma pack(pop)

/* 256 */
#pragma pack(push, 1)
struct CMoAttachments_VT_1032BBAC
{
  CMoAttachments base;
  unsigned int field_44;
  unsigned int field_44_2;
};
#pragma pack(pop)

/* 257 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032BBF4.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032BBF4
{
  CMoAttachments base;
};
#pragma pack(pop)

/* 258 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032BC68.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032BC68
{
  CMoAttachments base;
};
#pragma pack(pop)

/* 259 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032BC84.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032BC84
{
  CMoAttachments base;
};
#pragma pack(pop)

/* 260 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032BCCC.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032BCCC
{
  CMoAttachments base;
};
#pragma pack(pop)

/* 261 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032BD14.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032BD14
{
  CMoAttachments base;
};
#pragma pack(pop)

/* 262 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032BD5C.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032BD5C
{
  CMoAttachments base;
};
#pragma pack(pop)

/* 263 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032BDA4.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032BDA4
{
  CMoAttachments base;
};
#pragma pack(pop)

/* 264 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032BDEC.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032BDEC
{
  CMoAttachments base;
};
#pragma pack(pop)

/* 265 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032BE34.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032BE34
{
  CMoAttachments base;
};
#pragma pack(pop)

/* 266 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032BE7C.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032BE7C
{
  CMoAttachments base;
};
#pragma pack(pop)

/* 267 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032BEC4.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032BEC4
{
  CMoAttachments base;
};
#pragma pack(pop)

/* 268 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032BF0C.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032BF0C
{
  CMoAttachments base;
};
#pragma pack(pop)

/* 269 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032BF54.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032BF54
{
  CMoAttachments base;
};
#pragma pack(pop)

/* 271 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032BFE4.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032BFE4
{
  CMoAttachments base;
};
#pragma pack(pop)

/* 272 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032C02C.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032C02C
{
  CMoAttachments base;
};
#pragma pack(pop)

/* 273 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032C4E8.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032C4E8
{
  CMoAttachments base;
};
#pragma pack(pop)

/* 274 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032C628.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032C628
{
  CMoAttachments_VT_1032B140 base;
};
#pragma pack(pop)

/* 275 */
#pragma pack(push, 1)
/// recon: tree/include/CMoAttachments_VT_1032E7EC.hpp, sizeof 0x40 from the binary's descriptor
struct CMoAttachments_VT_1032E7EC
{
  CMoAttachments base;
};
#pragma pack(pop)

/* 279 */
#pragma pack(push, 1)
struct CMoCelestial
{
  CMoCelestial_vtbl *base;
};
#pragma pack(pop)

/* 583 */
struct CMoCelestial_vtbl
{
  void *vf0_CMoCelestial_GetRuntimeClass;
  void *vf1_CApp_vf001;
  unsigned int (__thiscall *vf2_sub_1002C930)(_DWORD *this, char);
  int (__thiscall *vf3_sub_1002C920)(_DWORD *this);
  void *vf4_CApp_vf004;
  void *vf5_CApp_vf005;
  _DWORD *(__thiscall *vf6_CMoCelestial_ScalarDeletingDtor)(CMoCelestial *C, char);
};

/* 280 */
#pragma pack(push, 1)
/// recon: tree/include/CMoOtTask.hpp, sizeof 0x54 from the binary's descriptor
struct CMoOtTask
{
  CMoTask base;
  unsigned __int8 _pad_34[32];
};
#pragma pack(pop)

/* 281 */
#pragma pack(push, 1)
/// recon: tree/include/CMoElem.hpp, sizeof 0x194 from the binary's descriptor
struct CMoElem
{
  CMoOtTask base;
  unsigned __int8 _pad_54[172];
  unsigned __int32 field_100;
  unsigned __int8 _pad_104[52];
  unsigned __int32 field_138;
  unsigned __int8 _pad_13C[88];
};
#pragma pack(pop)

/* 282 */
#pragma pack(push, 1)
/// recon: tree/include/CMoD3aElem.hpp, sizeof 0x194 from the binary's descriptor
struct CMoD3aElem
{
  CMoElem base;
};
#pragma pack(pop)

/* 283 */
#pragma pack(push, 1)
/// recon: tree/include/CMoD3aLightmapElem.hpp, sizeof 0x198 from the binary's descriptor
struct CMoD3aLightmapElem
{
  CMoElem base;
  unsigned __int32 field_194;
};
#pragma pack(pop)

/* 284 */
#pragma pack(push, 1)
struct CMoD3mSpecularElem
{
  CMoElem base;
  unsigned __int8 _pad_194[4];
  unsigned __int8 field_198;
  unsigned __int8 _pad_194_199[3];
  float field_19C;
  float field_1A0;
  float field_1A4;
  unsigned int field_1A8;
  unsigned int field_1AC;
  unsigned __int8 _pad_194_1B0[8];
  unsigned __int8 field_1B8;
  unsigned __int8 _pad_194_1B9[3];
  unsigned __int8 field_1BC;
  unsigned __int8 field_1BD;
  unsigned __int8 _pad_194_1BE[2];
};
#pragma pack(pop)

/* 286 */
#pragma pack(push, 1)
struct CMoDX
{
  CMoDX_vtbl *base;
  unsigned __int8 _pad_4[3768];
  unsigned int field_EBC;
  unsigned __int8 _pad_4_EC0[4108];
};
#pragma pack(pop)

/* 587 */
struct CMoDX_vtbl
{
  void *vf0_CMoDX_GetRuntimeClass;
  void *vf1_CApp_vf001;
  unsigned int (__thiscall *vf2_sub_1002C930)(_DWORD *this, char);
  int (__thiscall *vf3_sub_1002C920)(_DWORD *this);
  void *vf4_CApp_vf004;
  void *vf5_CApp_vf005;
  _BYTE *(__thiscall *vf6_CMoDX_ScalarDeletingDtor)(CMoDX *C, char);
};

/* 287 */
#pragma pack(push, 1)
/// recon: tree/include/CMoDisintegrateProgElem.hpp, sizeof 0x1E0 from the binary's descriptor
struct CMoDisintegrateProgElem
{
  CMoElem base;
  unsigned __int8 _pad_194[76];
};
#pragma pack(pop)

/* 288 */
#pragma pack(push, 1)
/// recon: tree/include/CMoDist.hpp, sizeof 0x198 from the binary's descriptor
struct CMoDist
{
  CMoElem base;
  unsigned __int32 field_194;
};
#pragma pack(pop)

/* 289 */
#pragma pack(push, 1)
/// recon: tree/include/CMoDistModelElem.hpp, sizeof 0x198 from the binary's descriptor
struct CMoDistModelElem
{
  CMoElem base;
  unsigned __int32 field_194;
};
#pragma pack(pop)

/* 290 */
#pragma pack(push, 1)
/// recon: tree/include/CXi471011C8_VT_1032AC90.hpp, sizeof 0x194 from the binary's descriptor
struct CXi471011C8_VT_1032AC90
{
  CMoElem base;
};
#pragma pack(pop)

/* 291 */
#pragma pack(push, 1)
struct CMoDistMorphElem
{
  CXi471011C8_VT_1032AC90 base;
  unsigned __int8 _pad_194[40];
  unsigned int field_1BC;
  unsigned int field_1C0;
};
#pragma pack(pop)

/* 292 */
#pragma pack(push, 1)
struct CMoDistRingElem
{
  CMoElem base;
  unsigned __int8 _pad_194[32];
  unsigned __int8 field_1B4;
  unsigned __int8 field_1B5;
  unsigned __int32 field_1B6;
};
#pragma pack(pop)

/* 293 */
#pragma pack(push, 1)
struct CMoDxStatusMng
{
  CMoDxStatusMng_vtbl *base;
  unsigned __int8 _pad_4[8];
  unsigned int field_C;
};
#pragma pack(pop)

/* 593 */
struct CMoDxStatusMng_vtbl
{
  void *vf0_CMoDxStatusMng_GetRuntimeClass;
  void *vf1_CApp_vf001;
  unsigned int (__thiscall *vf2_sub_1002C930)(_DWORD *this, char);
  int (__thiscall *vf3_sub_1002C920)(_DWORD *this);
  void *vf4_CApp_vf004;
  void *vf5_CApp_vf005;
  int *(__thiscall *vf6_CMoDxStatusMng_ScalarDeletingDtor)(CMoDxStatusMng *C, char);
};

/* 294 */
#pragma pack(push, 1)
struct CMoElem_VT_1032ADE8
{
  CMoElem base;
  unsigned int field_194;
};
#pragma pack(pop)

/* 296 */
#pragma pack(push, 1)
struct CMoResource
{
  CMoResource_vtbl *base;
  unsigned __int32 field_4;
  unsigned __int8 _pad_8[12];
  unsigned __int32 field_14;
  unsigned __int8 _pad_18[2];
  unsigned __int16 field_1A;
  unsigned __int8 _pad_18_1C[12];
  unsigned int field_28;
  unsigned __int8 _pad_18_2C[4];
};
#pragma pack(pop)

/* 606 */
struct CMoResource_vtbl
{
  void *vf0_CMoResource_GetRuntimeClass_variant3;
  __int16 (__thiscall *vf1_CMoCamera_GetLinkedObject)(_DWORD *this, _DWORD *);
  unsigned int (__thiscall *vf2_sub_1002C930)(_DWORD *this, char);
  int (__thiscall *vf3_sub_1002C920)(_DWORD *this);
  int (__thiscall *vf4_CMoCamera_SetLinkedObject)(_DWORD **this, int);
  void (__stdcall *vf5_CMoResource_vf005)(int);
  unsigned int **(__thiscall *vf6_CMoResource_ScalarDeletingDtor)(CMoResource *C, char);
  unsigned int (__thiscall *vf7_sub_10071E00)(int this, _DWORD *);
  void *vf8_CMoResource_vf008;
  __int16 (__thiscall *vf9_CMoResource_Destroy)(int this);
  void *vf10_CMoCamera_GetSharedInstance;
};

/* 299 */
#pragma pack(push, 1)
/// recon: tree/include/CMoLfdElem.hpp, sizeof 0x1A0 from the binary's descriptor
struct CMoLfdElem
{
  CMoElem base;
  unsigned __int8 _pad_194[12];
};
#pragma pack(pop)

/* 300 */
#pragma pack(push, 1)
/// recon: tree/include/CMoLockColorDriveTask.hpp, sizeof 0x7C from the binary's descriptor
struct CMoLockColorDriveTask
{
  CMoTask base;
  unsigned __int8 _pad_34[72];
};
#pragma pack(pop)

/* 301 */
#pragma pack(push, 1)
/// recon: tree/include/CMoLockLookAtDriveTask.hpp, sizeof 0x80 from the binary's descriptor
struct CMoLockLookAtDriveTask
{
  CMoTask base;
  unsigned __int8 _pad_34[76];
};
#pragma pack(pop)

/* 302 */
#pragma pack(push, 1)
/// recon: tree/include/CMoNullProgElem.hpp, sizeof 0x194 from the binary's descriptor
struct CMoNullProgElem
{
  CMoElem base;
};
#pragma pack(pop)

/* 303 */
#pragma pack(push, 1)
struct CMoOT
{
  CMoOT_vtbl *base;
  unsigned int field_4;
  unsigned __int8 _pad_4_8[16];
};
#pragma pack(pop)

/* 598 */
struct CMoOT_vtbl
{
  void *vf0_CMoOT_GetRuntimeClass;
  void *vf1_CApp_vf001;
  unsigned int (__thiscall *vf2_sub_1002C930)(_DWORD *this, char);
  int (__thiscall *vf3_sub_1002C920)(_DWORD *this);
  void *vf4_CApp_vf004;
  void *vf5_CApp_vf005;
  int *(__thiscall *vf6_CMoOT_ScalarDeletingDtor)(CMoOT *C, char);
};

/* 304 */
#pragma pack(push, 1)
struct CMoOcclusionMng
{
  CMoOcclusionMng_vtbl *base;
  unsigned __int8 _pad_4[20];
  unsigned int field_18;
};
#pragma pack(pop)

/* 599 */
struct CMoOcclusionMng_vtbl
{
  void *vf0_CMoOcclusionMng_GetRuntimeClass;
  void *vf1_CApp_vf001;
  unsigned int (__thiscall *vf2_sub_1002C930)(_DWORD *this, char);
  int (__thiscall *vf3_sub_1002C920)(_DWORD *this);
  void *vf4_CApp_vf004;
  void *vf5_CApp_vf005;
  int *(__thiscall *vf6_CMoOcclusionMng_ScalarDeletingDtor)(CMoOcclusionMng *C, char);
};

/* 306 */
#pragma pack(push, 1)
/// recon: tree/include/CMoPathDriveActorTask.hpp, sizeof 0xB0 from the binary's descriptor
struct CMoPathDriveActorTask
{
  CMoTask base;
  unsigned __int8 _pad_34[124];
};
#pragma pack(pop)

/* 308 */
#pragma pack(push, 1)
struct CMoPointLightProgElem
{
  CMoElem base;
  unsigned int field_194;
  unsigned __int8 _pad_194_198[4];
  float field_19C;
  float field_1A0;
  float field_1A4;
  float field_1A8;
  unsigned int field_1AC;
  unsigned __int8 field_1B0;
  unsigned __int8 _pad_194_1B1[3];
};
#pragma pack(pop)

/* 309 */
#pragma pack(push, 1)
/// recon: tree/include/CMoProcessor.hpp, sizeof 0x10 from the binary's descriptor
struct CMoProcessor_recon
{
  CYyObject base;
  unsigned __int8 _pad_4[12];
};
#pragma pack(pop)

/* 310 */
#pragma pack(push, 1)
struct CMoProcessor_VT_1032C1C8
{
  CMoProcessor_recon base;
  float field_864;
  float field_868;
  float field_874;
  float field_878;
  float field_898;
  float field_8B0;
  float field_8B8;
  float field_8D4;
  float field_8E0;
  float field_8E4;
  float field_864_2;
  float field_868_2;
  float field_874_2;
  float field_878_2;
  float field_898_2;
  float field_8B0_2;
  float field_8B8_2;
  float field_8D4_2;
  float field_8E0_2;
  float field_8E4_2;
  float field_864_3;
  float field_868_3;
  float field_874_3;
  float field_878_3;
  float field_898_3;
  float field_8B0_3;
  float field_8B8_3;
  float field_8D4_3;
  float field_8E0_3;
  float field_8E4_3;
};
#pragma pack(pop)

/* 314 */
#pragma pack(push, 1)
struct CMoResourceMng
{
  CMoResourceMng_vtbl *base;
  unsigned __int8 _pad_4[262148];
  unsigned int field_40008;
  unsigned __int8 _pad_4_4000C[4];
};
#pragma pack(pop)

/* 607 */
struct CMoResourceMng_vtbl
{
  void *vf0_CMoResourceMng_GetRuntimeClass;
  void *vf1_CApp_vf001;
  unsigned int (__thiscall *vf2_sub_1002C930)(_DWORD *this, char);
  int (__thiscall *vf3_sub_1002C920)(_DWORD *this);
  void *vf4_CApp_vf004;
  void *vf5_CApp_vf005;
  _DWORD *(__thiscall *vf6____G__basic_ios_DU__char_traits_D_std___std__UAEPAXI_Z_0_0)(CMoResourceMng *this, char);
};

/* 315 */
#pragma pack(push, 1)
/// recon: tree/include/CMoResource_VT_103366C4.hpp, sizeof 0x30 from the binary's descriptor
struct CMoResource_VT_103366C4
{
  CMoResource base;
};
#pragma pack(pop)

/* 316 */
#pragma pack(push, 1)
struct CMoResource_VT_1032C55C
{
  CMoResource_VT_103366C4 base;
  unsigned __int8 _pad_30[4];
  unsigned int field_34;
};
#pragma pack(pop)

/* 317 */
#pragma pack(push, 1)
/// recon: tree/include/CMoResource_VT_1032C588.hpp, sizeof 0x30 from the binary's descriptor
struct CMoResource_VT_1032C588
{
  CMoResource_VT_103366C4 base;
};
#pragma pack(pop)

/* 318 */
#pragma pack(push, 1)
/// recon: tree/include/CMoResource_VT_1032C5FC.hpp, sizeof 0x30 from the binary's descriptor
struct CMoResource_VT_1032C5FC
{
  CMoResource_VT_103366C4 base;
};
#pragma pack(pop)

/* 320 */
#pragma pack(push, 1)
/// recon: tree/include/CMoResource_VT_1032C670.hpp, sizeof 0x30 from the binary's descriptor
struct CMoResource_VT_1032C670
{
  CMoResource_VT_103366C4 base;
};
#pragma pack(pop)

/* 321 */
#pragma pack(push, 1)
/// recon: tree/include/CMoResource_VT_1032C69C.hpp, sizeof 0x30 from the binary's descriptor
struct CMoResource_VT_1032C69C
{
  CMoResource_VT_103366C4 base;
};
#pragma pack(pop)

/* 322 */
#pragma pack(push, 1)
/// recon: tree/include/CMoResource_VT_1032C6C8.hpp, sizeof 0x30 from the binary's descriptor
struct CMoResource_VT_1032C6C8
{
  CMoResource_VT_103366C4 base;
};
#pragma pack(pop)

/* 323 */
#pragma pack(push, 1)
/// recon: tree/include/CMoResource_VT_1032C6F4.hpp, sizeof 0x30 from the binary's descriptor
struct CMoResource_VT_1032C6F4
{
  CMoResource_VT_103366C4 base;
};
#pragma pack(pop)

/* 324 */
#pragma pack(push, 1)
struct CMoResource_VT_1032C720
{
  CMoResource_VT_103366C4 base;
  unsigned __int16 field_32;
  unsigned __int8 field_34;
  unsigned int field_3C;
  unsigned int field_44;
  unsigned int field_3C_2;
  unsigned int field_44_2;
  unsigned int field_44_3;
};
#pragma pack(pop)

/* 325 */
#pragma pack(push, 1)
struct CMoResource_VT_1032C74C
{
  CMoResource_VT_103366C4 base;
  unsigned __int16 field_32;
  unsigned __int8 field_34;
  unsigned __int16 field_36;
  unsigned __int16 field_36_2;
};
#pragma pack(pop)

/* 327 */
#pragma pack(push, 1)
/// recon: tree/include/CMoResource_VT_1032C7A4.hpp, sizeof 0x30 from the binary's descriptor
struct CMoResource_VT_1032C7A4
{
  CMoResource_VT_103366C4 base;
};
#pragma pack(pop)

/* 328 */
#pragma pack(push, 1)
/// recon: tree/include/CMoResource_VT_1032C7D0.hpp, sizeof 0x30 from the binary's descriptor
struct CMoResource_VT_1032C7D0
{
  CMoResource_VT_103366C4 base;
};
#pragma pack(pop)

/* 329 */
#pragma pack(push, 1)
struct CMoResource_VT_1032C7FC
{
  CMoResource_VT_103366C4 base;
  unsigned __int8 field_30;
  unsigned __int8 field_31;
  unsigned __int16 field_32;
  unsigned __int16 field_34;
};
#pragma pack(pop)

/* 330 */
#pragma pack(push, 1)
struct CMoResource_VT_1032C828
{
  CMoResource_VT_103366C4 base;
  unsigned __int8 field_30;
};
#pragma pack(pop)

/* 331 */
#pragma pack(push, 1)
struct CMoResource_VT_1032C854
{
  CMoResource_VT_103366C4 base;
  unsigned __int8 field_30;
  unsigned __int8 field_31;
  unsigned __int8 field_32;
  unsigned __int8 _pad_30_33[1];
  unsigned __int8 field_34;
  unsigned __int8 _pad_30_35[1];
  unsigned int field_36;
  unsigned __int8 _pad_30_3A[2];
  unsigned int field_3C;
  unsigned __int8 _pad_30_40[2];
  unsigned int field_42;
  unsigned __int8 _pad_30_46[2];
  unsigned int field_48;
  unsigned __int8 _pad_30_4C[2];
  unsigned int field_4E;
  unsigned __int8 _pad_30_52[2];
  unsigned int field_54;
  unsigned int field_5E;
  unsigned int field_7C;
  unsigned int field_7C_2;
  unsigned int field_7C_3;
};
#pragma pack(pop)

/* 332 */
#pragma pack(push, 1)
struct CMoResource_VT_1032C880
{
  CMoResource_VT_103366C4 base;
  unsigned __int16 field_34;
  unsigned __int16 field_36;
  unsigned int field_48;
  unsigned int field_4C;
  unsigned int field_48_2;
  unsigned int field_4C_2;
  unsigned int field_48_3;
  unsigned int field_4C_3;
};
#pragma pack(pop)

/* 333 */
#pragma pack(push, 1)
struct CMoResource_VT_1032C8AC
{
  CMoResource_VT_103366C4 base;
  unsigned __int8 field_30;
};
#pragma pack(pop)

/* 334 */
#pragma pack(push, 1)
/// recon: tree/include/CMoResource_VT_1032C934.hpp, sizeof 0x30 from the binary's descriptor
struct CMoResource_VT_1032C934
{
  CMoResource base;
};
#pragma pack(pop)

/* 335 */
#pragma pack(push, 1)
/// recon: tree/include/CMoResource_VT_10336698.hpp, sizeof 0x30 from the binary's descriptor
struct CMoResource_VT_10336698
{
  CMoResource base;
};
#pragma pack(pop)

/* 336 */
#pragma pack(push, 1)
/// recon: tree/include/CMoResource_VT_103366F0.hpp, sizeof 0x30 from the binary's descriptor
struct CMoResource_VT_103366F0
{
  CMoResource base;
};
#pragma pack(pop)

/* 337 */
#pragma pack(push, 1)
/// recon: tree/include/CMoResource_VT_103367E8.hpp, sizeof 0x30 from the binary's descriptor
struct CMoResource_VT_103367E8
{
  CMoResource base;
};
#pragma pack(pop)

/* 339 */
#pragma pack(push, 1)
struct CMoSkeletonElem
{
  CMoElem base;
  unsigned __int8 _pad_194[64];
  unsigned __int8 field_1D4;
  unsigned int field_1D5;
};
#pragma pack(pop)

/* 340 */
#pragma pack(push, 1)
/// recon: tree/include/CMoSphRes.hpp, sizeof 0x50 from the binary's descriptor
struct CMoSphRes
{
  CMoResource_VT_103366C4 base;
  unsigned __int8 _pad_30[32];
};
#pragma pack(pop)

/* 341 */
#pragma pack(push, 1)
struct CMoSpline
{
  CMoSpline_vtbl *base;
  unsigned __int8 _pad_4[8];
  unsigned __int32 field_C;
  unsigned __int32 field_10;
  unsigned __int32 field_14;
  unsigned __int32 field_18;
  unsigned __int8 _pad_1C[4];
  unsigned __int32 field_20;
  unsigned __int8 _pad_24[4];
};
#pragma pack(pop)

/* 631 */
struct CMoSpline_vtbl
{
  void *vf0_CMoSpline_GetRuntimeClass;
  void *vf1_CApp_vf001;
  unsigned int (__thiscall *vf2_sub_1002C930)(_DWORD *this, char);
  int (__thiscall *vf3_sub_1002C920)(_DWORD *this);
  void *vf4_CApp_vf004;
  void *vf5_CApp_vf005;
  int *(__thiscall *vf6_CMoSpline_ScalarDeletingDtor)(CMoSpline *C, char);
};

/* 342 */
#pragma pack(push, 1)
struct CMoTaskMng
{
  CMoTaskMng_vtbl *base;
  unsigned __int8 _pad_4[3404];
  unsigned int field_D50;
  unsigned __int8 _pad_4_D54[16];
};
#pragma pack(pop)

/* 633 */
struct CMoTaskMng_vtbl
{
  void *vf0_CMoTaskMng_GetRuntimeClass;
  void *vf1_CApp_vf001;
  unsigned int (__thiscall *vf2_sub_1002C930)(_DWORD *this, char);
  int (__thiscall *vf3_sub_1002C920)(_DWORD *this);
  void *vf4_CApp_vf004;
  void *vf5_CApp_vf005;
  _DWORD *(__thiscall *vf6_CMoTaskMng_ScalarDeletingDtor)(CMoTaskMng *C, char);
};

/* 343 */
#pragma pack(push, 1)
struct CMoTask_VT_10329C70
{
  CMoTask base;
  unsigned int field_34;
};
#pragma pack(pop)

/* 344 */
#pragma pack(push, 1)
struct CMoTask_VT_1032A904
{
  CMoTask base;
  unsigned int field_34;
  float field_38;
  float field_3C;
  float field_40;
  unsigned int field_44;
};
#pragma pack(pop)

/* 347 */
#pragma pack(push, 1)
/// recon: tree/include/CMoTask_VT_1032AFA8.hpp, sizeof 0x34 from the binary's descriptor
struct CMoTask_VT_1032AFA8
{
  CMoTask base;
};
#pragma pack(pop)

/* 348 */
#pragma pack(push, 1)
struct CMoTask_VT_1032B654
{
  CMoTask base;
  unsigned int field_38;
  unsigned __int8 field_3C;
  unsigned __int8 field_3C_2;
  unsigned __int8 field_3C_3;
};
#pragma pack(pop)

/* 353 */
#pragma pack(push, 1)
/// recon: tree/include/CMoTask_VT_1032B95C.hpp, sizeof 0x34 from the binary's descriptor
struct CMoTask_VT_1032B95C
{
  CMoTask base;
};
#pragma pack(pop)

/* 374 */
#pragma pack(push, 1)
/// recon: tree/include/CMoTask_VT_1032C490.hpp, sizeof 0x34 from the binary's descriptor
struct CMoTask_VT_1032C490
{
  CMoTask base;
};
#pragma pack(pop)

/* 376 */
#pragma pack(push, 1)
struct CMoTask_VT_1032C908
{
  CMoTask base;
  unsigned int field_34;
  unsigned int field_38;
  unsigned __int8 field_3C;
  unsigned int field_3D;
  unsigned int field_41;
  unsigned int field_45;
};
#pragma pack(pop)

/* 377 */
#pragma pack(push, 1)
struct CMoTask_VT_1032D6D4
{
  CMoTask base;
  unsigned int field_34;
  unsigned int field_38;
  unsigned int field_3C;
  unsigned int field_44;
  unsigned int field_48;
  unsigned __int8 field_58;
  unsigned int field_60;
  unsigned int field_64;
  unsigned int field_6C;
  unsigned __int8 field_58_2;
  unsigned int field_60_2;
  unsigned int field_64_2;
  unsigned int field_6C_2;
  unsigned int field_64_3;
  unsigned int field_6C_3;
};
#pragma pack(pop)

/* 379 */
#pragma pack(push, 1)
struct CMoVuAnmElem
{
  CMoElem base;
  unsigned __int8 _pad_194[68];
  unsigned __int16 field_1D8;
  unsigned __int8 _pad_194_1DA[2];
  unsigned __int8 field_1DC;
  unsigned __int8 _pad_194_1DD[72];
  unsigned int field_225;
  unsigned int field_229;
  unsigned int field_22D;
};
#pragma pack(pop)

/* 380 */
#pragma pack(push, 1)
/// recon: tree/include/CMoVuDist.hpp, sizeof 0x1E0 from the binary's descriptor
struct CMoVuDist
{
  CMoElem base;
  unsigned __int8 _pad_194[76];
};
#pragma pack(pop)

/* 382 */
#pragma pack(push, 1)
struct CShader
{
  CShader_vtbl *base;
  unsigned __int32 field_4;
  unsigned __int32 field_8;
  unsigned __int32 field_C;
  unsigned __int32 field_10;
};
#pragma pack(pop)

/* 643 */
struct CShader_vtbl
{
  void *vf0_CShader_GetRuntimeClass;
  void *vf1_CApp_vf001;
  unsigned int (__thiscall *vf2_sub_1002C930)(_DWORD *this, char);
  int (__thiscall *vf3_sub_1002C920)(_DWORD *this);
  void *vf4_CApp_vf004;
  void *vf5_CApp_vf005;
  unsigned int *(__thiscall *vf6_CShader_ScalarDeletingDtor)(CShader *C, char);
};

/* 383 */
#pragma pack(push, 1)
struct CTsZoneMap
{
  CMoTask base;
  unsigned __int16 field_34;
  unsigned __int8 _pad_34_36[2];
  unsigned int field_38;
  unsigned __int8 _pad_34_3C[4];
  unsigned int field_40;
  unsigned __int32 field_44;
  unsigned __int8 _pad_48[4];
  unsigned int field_4C;
  unsigned int field_50;
  unsigned int field_54;
  unsigned __int8 _pad_48_58[248];
  unsigned int field_150;
  unsigned __int8 _pad_48_154[4];
  unsigned int field_158;
  unsigned __int8 _pad_48_15C[8];
  unsigned int field_164;
  float field_168;
  unsigned __int8 _pad_48_16C[16];
  unsigned int field_17C;
  unsigned __int8 _pad_48_180[218160];
  unsigned int field_355B0;
  unsigned int field_355B4;
  unsigned __int8 _pad_48_355B8[14216];
  unsigned int field_38D40;
  unsigned int field_38D44;
  unsigned int field_38D48;
  unsigned __int8 _pad_48_38D4C[72];
  unsigned int field_38D94;
  unsigned __int8 _pad_48_38D98[856];
  float field_390F0;
  float field_390F4;
  float field_390F8;
  unsigned int field_390FC;
  unsigned int field_39100;
  unsigned int field_39104;
  unsigned int field_39108;
  unsigned int field_3910C;
  unsigned int field_39110;
  unsigned int field_39114;
  unsigned int field_39118;
  unsigned int field_3911C;
  unsigned int field_39120;
  unsigned __int8 _pad_48_39124[8];
  unsigned int field_3912C;
  unsigned __int8 _pad_48_39130[4];
  unsigned int field_39134;
  unsigned int field_39138;
  unsigned __int8 _pad_48_3913C[132];
  unsigned int field_391C0;
  unsigned int field_391C4;
  unsigned int field_391C8;
  unsigned __int8 _pad_48_391CC[132];
  unsigned int field_39250;
  unsigned int field_39254;
  unsigned int field_39258;
  unsigned int field_3925C;
  unsigned __int8 _pad_48_39260[64];
  unsigned int field_392A0;
  unsigned int field_392A4;
  unsigned int field_392A8;
  unsigned __int8 _pad_48_392AC[144];
  unsigned int field_3933C;
  unsigned __int8 _pad_48_39340[224];
  unsigned int field_39420;
  float field_39424;
  float field_39428;
  unsigned int field_3942C;
  unsigned int field_39430;
  unsigned __int8 _pad_48_39434[20];
  unsigned int field_39448;
  unsigned int field_3944C;
  unsigned __int8 _pad_48_39450[4];
  unsigned int field_39454;
  unsigned int field_39458;
  unsigned int field_3945C;
  unsigned int field_39460;
  unsigned int field_39464;
  unsigned int field_39468;
  unsigned int field_3946C;
  unsigned int field_39470;
  unsigned int field_39474;
  unsigned int field_39478;
  unsigned int field_3947C;
  unsigned int field_39480;
  unsigned int field_39484;
  unsigned int field_39488;
  unsigned int field_3948C;
  unsigned int field_39490;
  unsigned int field_39494;
  unsigned int field_39498;
  unsigned int field_3949C;
  unsigned int field_394A0;
  unsigned int field_394A4;
  unsigned int field_394A8;
  unsigned int field_394AC;
  unsigned int field_394B0;
  unsigned int field_394B4;
  unsigned int field_394B8;
  unsigned int field_394BC;
  unsigned int field_394C0;
  unsigned int field_394C4;
  unsigned int field_394C8;
  unsigned int field_394CC;
  unsigned int field_394D0;
  unsigned int field_394D4;
  unsigned int field_394D8;
  unsigned int field_394DC;
  unsigned __int8 _pad_48_394E0[16];
  unsigned int field_394F0;
  unsigned int field_394F4;
  unsigned int field_394F8;
  unsigned int field_394FC;
  float field_39500;
  unsigned int field_39504;
  unsigned int field_39508;
  unsigned int field_3950C;
  unsigned int field_39510;
  unsigned int field_39514;
  unsigned int field_39518;
  unsigned int field_3951C;
  unsigned __int8 _pad_48_39520[32];
  float field_39540;
  float field_39544;
  float field_39548;
  unsigned int field_3954C;
  unsigned __int8 _pad_48_39550[100];
};
#pragma pack(pop)

/* 387 */
#pragma pack(push, 1)
/// recon: tree/include/CXiActor.hpp, sizeof 0xC4 from the binary's descriptor
struct CXiActor
{
  CMoTask base;
  unsigned __int32 field_34;
  float field_38;
  unsigned __int32 field_3C;
  unsigned __int32 field_40;
  unsigned __int8 _pad_44[60];
  unsigned __int32 field_80;
  unsigned __int32 field_84;
  unsigned __int32 field_88;
  unsigned __int32 field_8C;
  unsigned __int8 _pad_90[4];
  float field_94;
  float field_98;
  unsigned __int32 field_9C;
  unsigned __int8 _pad_A0[4];
  unsigned __int32 field_A4;
  unsigned __int32 field_A8;
  unsigned __int8 _pad_AC[6];
  unsigned __int8 field_B2;
  unsigned __int8 _pad_B3[1];
  unsigned __int32 field_B4;
  unsigned __int8 _pad_B8[4];
  unsigned __int32 field_BC;
  unsigned __int8 _pad_C0[4];
};
#pragma pack(pop)

/* 391 */
#pragma pack(push, 1)
/// recon: tree/include/CXiControlActor.hpp, sizeof 0x5C4 from the binary's descriptor
struct CXiControlActor
{
  CXiActor base;
  unsigned __int8 _pad_C4[16];
  unsigned __int32 field_D4;
  unsigned __int32 field_D8;
  unsigned __int32 field_DC;
  unsigned __int32 field_E0;
  unsigned __int8 _pad_E4[21];
  unsigned __int8 field_F9;
  unsigned __int8 field_FA;
  unsigned __int8 field_FB;
  unsigned __int32 field_FC;
  unsigned __int8 _pad_100[3];
  unsigned __int8 field_103;
  unsigned __int8 field_104;
  unsigned __int8 _pad_105[3];
  unsigned __int32 field_108;
  unsigned __int32 field_10C;
  unsigned __int32 field_110;
  unsigned __int8 _pad_114[92];
  unsigned __int32 field_170;
  unsigned __int32 field_174;
  unsigned __int32 field_178;
  unsigned __int8 field_17C;
  unsigned __int8 _pad_17D[1095];
};
#pragma pack(pop)

/* 399 */
#pragma pack(push, 1)
struct CXiOpening
{
  CXiOpening_vtbl *base;
  unsigned __int8 _pad_4[272];
  unsigned int field_114;
  unsigned int field_118;
  unsigned int field_11C;
  unsigned int field_120;
  unsigned int field_124;
};
#pragma pack(pop)

/* 646 */
struct CXiOpening_vtbl
{
  void *vf0_CXiOpening_GetRuntimeClass;
  void *vf1_CApp_vf001;
  unsigned int (__thiscall *vf2_sub_1002C930)(_DWORD *this, char);
  int (__thiscall *vf3_sub_1002C920)(_DWORD *this);
  void *vf4_CApp_vf004;
  void *vf5_CApp_vf005;
  int *(__thiscall *vf6_CXiOpening_ScalarDeletingDtor)(CXiOpening *C, char);
};

/* 427 */
#pragma pack(push, 1)
struct CYyModel
{
  CYyModel_vtbl *base;
  unsigned __int8 _pad_4[184];
};
#pragma pack(pop)

/* 649 */
struct CYyModel_vtbl
{
  void *vf0_CYyModel_GetRuntimeClass;
  void *vf1_CApp_vf001;
  unsigned int (__thiscall *vf2_sub_1002C930)(_DWORD *this, char);
  int (__thiscall *vf3_sub_1002C920)(_DWORD *this);
  void *vf4_CApp_vf004;
  void *vf5_CApp_vf005;
  int *(__thiscall *vf6_CYyModel_ScalarDeletingDtor)(CYyModel *C, char);
};

/* 428 */
#pragma pack(push, 1)
struct CYyModelBase
{
  CYyModelBase_vtbl *base;
  unsigned int field_4;
  unsigned __int8 _pad_4_8[4];
  unsigned int field_C;
  unsigned __int8 _pad_4_10[16];
  unsigned int field_20;
  unsigned __int8 _pad_4_24[164];
};
#pragma pack(pop)

/* 650 */
struct CYyModelBase_vtbl
{
  void *vf0_CYyModelBase_GetRuntimeClass;
  void *vf1_CApp_vf001;
  unsigned int (__thiscall *vf2_sub_1002C930)(_DWORD *this, char);
  int (__thiscall *vf3_sub_1002C920)(_DWORD *this);
  void *vf4_CApp_vf004;
  void *vf5_CApp_vf005;
  int *(__thiscall *vf6_CYyModelBase_ScalarDeletingDtor)(CYyModelBase *C, char a1);
};

/* 429 */
#pragma pack(push, 1)
struct CYyModelDt
{
  CYyModelDt_vtbl *base;
  unsigned __int8 _pad_4[56];
};
#pragma pack(pop)

/* 651 */
struct CYyModelDt_vtbl
{
  void *vf0_CYyModelDt_GetRuntimeClass;
  void *vf1_CApp_vf001;
  unsigned int (__thiscall *vf2_sub_1002C930)(_DWORD *this, char);
  int (__thiscall *vf3_sub_1002C920)(_DWORD *this);
  void *vf4_CApp_vf004;
  void *vf5_CApp_vf005;
  _DWORD *(__thiscall *vf6_CYyModelDt_ScalarDeletingDtor)(CYyModelDt *C, char a1);
  void *vf7___purecall;
  void *vf8___purecall;
  void *vf9___purecall;
  void *vf10___purecall;
  void *vf11___purecall;
};

/* 430 */
#pragma pack(push, 1)
struct CYyModelDt_VT_1032A430
{
  CYyModelDt base;
  unsigned int field_40;
  unsigned int field_40_2;
};
#pragma pack(pop)

/* 431 */
#pragma pack(push, 1)
/// recon: tree/include/CYyModelDt_VT_1032A72C.hpp, sizeof 0x3C from the binary's descriptor
struct CYyModelDt_VT_1032A72C
{
  CYyModelDt base;
};
#pragma pack(pop)

/* 443 */
#pragma pack(push, 1)
struct CYyOs2VtxBuffer
{
  CMoTask base;
  unsigned int field_34;
  unsigned int field_38;
  unsigned int field_3C;
  unsigned int field_40;
  unsigned int field_44;
  unsigned int field_48;
  unsigned int field_4C;
  unsigned int field_50;
  unsigned int field_54;
  unsigned int field_58;
  unsigned int field_5C;
  unsigned int field_60;
  unsigned int field_64;
  unsigned __int8 field_68;
  unsigned __int8 field_69;
  unsigned int field_6A;
  unsigned int field_6E;
  unsigned int field_72;
  unsigned int field_76;
  unsigned int field_7A;
  unsigned int field_7E;
  unsigned int field_82;
  unsigned int field_86;
  unsigned int field_8A;
  unsigned __int8 _pad_34_8E[16];
};
#pragma pack(pop)

/* 450 */
#pragma pack(push, 1)
struct CYyTexBase
{
  CYyTexBase_vtbl *base;
  unsigned __int8 _pad_4[48];
};
#pragma pack(pop)

/* 656 */
struct CYyTexBase_vtbl
{
  void *vf0_CYyTexBase_GetRuntimeClass_variant0;
  void *vf1_CApp_vf001;
  unsigned int (__thiscall *vf2_sub_1002C930)(_DWORD *this, char);
  int (__thiscall *vf3_sub_1002C920)(_DWORD *this);
  void *vf4_CApp_vf004;
  void *vf5_CApp_vf005;
  _DWORD *(__thiscall *vf6_CYyTexBase_ScalarDeletingDtor)(CYyTexBase *C, char);
};

/* 451 */
#pragma pack(push, 1)
struct CYyTex
{
  CYyTexBase base;
  unsigned __int8 _pad_34[12];
  unsigned int field_40;
  unsigned int field_44;
};
#pragma pack(pop)

/* 452 */
#pragma pack(push, 1)
struct CYyTexMng
{
  CYyTexMng_vtbl *base;
  unsigned __int16 field_4;
  unsigned __int8 _pad_6[2];
  unsigned __int32 field_8;
  unsigned __int32 field_C;
  unsigned __int8 field_10;
  unsigned __int8 _pad_11[3];
};
#pragma pack(pop)

/* 657 */
struct CYyTexMng_vtbl
{
  void *vf0_CYyTexMng_GetRuntimeClass;
  void *vf1_CApp_vf001;
  unsigned int (__thiscall *vf2_sub_1002C930)(_DWORD *this, char);
  int (__thiscall *vf3_sub_1002C920)(_DWORD *this);
  void *vf4_CApp_vf004;
  void *vf5_CApp_vf005;
  _DWORD *(__thiscall *vf6_CYyTexMng_ScalarDeletingDtor)(CYyTexMng *C, char);
};

/* 463 */
#pragma pack(push, 1)
struct XiDoorActor
{
  CXiControlActor base;
  unsigned int field_5C4;
  unsigned int field_5C8;
  unsigned int field_5CC;
  unsigned int field_5D0;
  unsigned int field_5D4;
  unsigned int field_5D8;
  unsigned int field_5DC;
  unsigned int field_5E0;
  unsigned __int8 _pad_5C4_5E4[132];
  unsigned int field_668;
  unsigned __int8 _pad_5C4_66C[516];
  unsigned int field_870;
  unsigned int field_874;
  unsigned __int8 _pad_5C4_878[4];
  unsigned int field_87C;
  unsigned int field_880;
  unsigned __int8 _pad_5C4_884[16];
};
#pragma pack(pop)

/* 513 */
struct D3DHolder
{
  void *vftable;
  IDirect3D8 *pD3D;
  IDirect3DDevice8 *pDevice;
  unsigned __int8 _rest[1132];
};

/* 514 */
struct tagPALETTEENTRY
{
  BYTE peRed;
  BYTE peGreen;
  BYTE peBlue;
  BYTE peFlags;
};

/* 523 */
typedef struct _D3DVIEWPORT9 D3DVIEWPORT9;

/* 524 */
#pragma pack(push, 4)
struct _D3DVIEWPORT9
{
  DWORD X;
  DWORD Y;
  DWORD Width;
  DWORD Height;
  float MinZ;
  float MaxZ;
};
#pragma pack(pop)

/* 525 */
struct /*VFT*/ Vt_1032A460_vtbl
{
  int (__thiscall *vf0_CYyModelDt_VT_1032A430_ScalarDeletingDtor_2)(CYyModelDt_VT_1032A430 *this, int a1);
};

/* 526 */
struct Vt_1032A460
{
  struct Vt_1032A460_vtbl *vftable;
};

/* 527 */
struct /*VFT*/ Vt_1032AFD4_vtbl
{
  int (__thiscall *vf0_CMoTask_VT_1032AFA8_ScalarDeletingDtor_2)(CMoTask_VT_1032AFA8 *this, int a1);
};

/* 528 */
struct Vt_1032AFD4
{
  struct Vt_1032AFD4_vtbl *vftable;
};

/* 529 */
struct /*VFT*/ Vt_1032B680_vtbl
{
  int (__thiscall *vf0_CMoTask_VT_1032B654_vf011)(CMoTask_VT_1032B654 *this);
  int (__thiscall *vf1_CMoTask_VT_1032B654_vf012)(CMoTask_VT_1032B654 *this, int a1);
  int (__thiscall *vf2_sub_1003C720)(_DWORD *this);
  int (__thiscall *vf3_sub_1003C730)(_DWORD *this);
  char (__thiscall *vf4_sub_10054380)(void *this);
  char (__thiscall *vf5_sub_10054390)(void *this);
  char *(__thiscall *vf6_sub_10053830)(char *this);
};

/* 530 */
struct Vt_1032B680
{
  struct Vt_1032B680_vtbl *vftable;
};

/* 531 */
struct /*VFT*/ Vt_1032B988_vtbl
{
  int (__thiscall *vf0_CMoTask_VT_1032B95C_vf011)(CMoTask_VT_1032B95C *this);
  int (__thiscall *vf1_CMoTask_VT_1032B95C_vf012)(CMoTask_VT_1032B95C *this, int a1);
  int (__thiscall *vf2_sub_1003C720)(_DWORD *this);
  int (__thiscall *vf3_sub_1003C730)(_DWORD *this);
  bool (__thiscall *vf4_sub_1005E980)(char *this);
  bool (__thiscall *vf5_sub_1005E9A0)(char *this);
  char *(__thiscall *vf6_sub_1005E970)(char *this);
};

/* 532 */
struct Vt_1032B988
{
  struct Vt_1032B988_vtbl *vftable;
};

/* 533 */
struct /*VFT*/ Vt_1032B9D0_vtbl
{
  int (__thiscall *vf0_CMoPathDriveActorTask_vf011)(CMoPathDriveActorTask *this);
  int (__thiscall *vf1_CMoPathDriveActorTask_vf012)(CMoPathDriveActorTask *this, int a1);
  int (__thiscall *vf2_sub_1003C720)(_DWORD *this);
  int (__thiscall *vf3_sub_1003C730)(_DWORD *this);
  bool (__thiscall *vf4_sub_1005F380)(char *this);
  bool (__thiscall *vf5_sub_1005F3A0)(char *this);
  char *(__thiscall *vf6_sub_1005F370)(char *this);
};

/* 534 */
struct Vt_1032B9D0
{
  struct Vt_1032B9D0_vtbl *vftable;
};

/* 535 */
struct /*VFT*/ Vt_1032BA18_vtbl
{
  int (__thiscall *vf0_CMoLockColorDriveTask_vf011)(CMoLockColorDriveTask *this);
  int (__thiscall *vf1_CMoLockColorDriveTask_vf012)(CMoLockColorDriveTask *this, int a1);
  int (__thiscall *vf2_sub_1003C720)(_DWORD *this);
  int (__thiscall *vf3_sub_1003C730)(_DWORD *this);
  bool (__thiscall *vf4_sub_1005F500)(char *this);
  bool (__thiscall *vf5_sub_1005F520)(char *this);
  char *(__thiscall *vf6_sub_1005F4F0)(char *this);
};

/* 536 */
struct Vt_1032BA18
{
  struct Vt_1032BA18_vtbl *vftable;
};

/* 537 */
struct /*VFT*/ Vt_1032BA60_vtbl
{
  int (__thiscall *vf0_CMoLockLookAtDriveTask_vf011)(CMoLockLookAtDriveTask *this);
  int (__thiscall *vf1_CMoLockLookAtDriveTask_vf012)(CMoLockLookAtDriveTask *this, int a1);
  int (__thiscall *vf2_sub_1003C720)(_DWORD *this);
  int (__thiscall *vf3_sub_1003C730)(_DWORD *this);
  bool (__thiscall *vf4_sub_1005F7E0)(char *this);
  bool (__thiscall *vf5_sub_1005F800)(char *this);
  char *(__thiscall *vf6_sub_1005F7D0)(char *this);
};

/* 538 */
struct Vt_1032BA60
{
  struct Vt_1032BA60_vtbl *vftable;
};

/* 539 */
struct /*VFT*/ Vt_1032BAA8_vtbl
{
  int (__thiscall *vf0_CMoActorColorDriveTask_vf011)(CMoActorColorDriveTask *this);
  tagRECT *(__thiscall *vf1_CMoActorColorDriveTask_vf012)(_DWORD *this, char);
  int (__thiscall *vf2_sub_1003C720)(_DWORD *this);
  int (__thiscall *vf3_sub_1003C730)(_DWORD *this);
  bool (__thiscall *vf4_sub_1005FAF0)(char *this);
  bool (__thiscall *vf5_sub_1005FB10)(char *this);
  char *(__thiscall *vf6_sub_1005FAE0)(char *this);
};

/* 540 */
struct Vt_1032BAA8
{
  struct Vt_1032BAA8_vtbl *vftable;
};

/* 541 */
struct Vt_1032C5B4_vtbl
{
  int (__thiscall *vf0_CMoResource_VT_1032C588_vf011)(CMoResource_VT_1032C588 *this);
  int (__thiscall *vf1_CMoResource_VT_1032C588_vf012)(CMoResource_VT_1032C588 *this, int a1);
  int (__thiscall *vf2_sub_1003C720)(_DWORD *this);
  int (__thiscall *vf3_sub_1003C730)(_DWORD *this);
  void *vf4_CMoAttachments_vf004;
  void *vf5_CMoAttachments_vf005;
  void *vf6_CMoAttachments_vf006;
};

/* 542 */
struct Vt_1032C5B4
{
  struct Vt_1032C5B4_vtbl *vftable;
};

/* 179 */
typedef struct _RTL_CRITICAL_SECTION *PRTL_CRITICAL_SECTION;

/* 178 */
typedef PRTL_CRITICAL_SECTION LPCRITICAL_SECTION;

/* 543 */
struct Vt_1033BE90_vtbl
{
  int (__stdcall *vf0_sub_10260B30)(int a1, int a2, int a3);
  unsigned int (__stdcall *vf1_sub_10265A80)(int);
  int (__stdcall *vf2_sub_10265AA0)(volatile LONG *);
  _DWORD *(__thiscall *vf3_sub_1021DF70)(_DWORD *this, char);
  int (__thiscall *vf4_sub_10261710)(unsigned int *this, unsigned int *);
  int (__thiscall *vf5_sub_10261AA0)(_DWORD *this);
  void *vf6_sub_1025EAB0;
  int (__thiscall *vf7_sub_1025EAC0)(int this, int);
  void *vf8_sub_1021DF10;
  int (__fastcall *vf9_sub_1025E460)(int, int, int, int);
  int (__thiscall *vf10_sub_1025E5E0)(HANDLE *this, int);
  int (__thiscall *vf11_sub_1025E610)(HANDLE *this);
  int (__thiscall *vf12_sub_1025E750)(HANDLE *this, int);
  void *vf13_nullsub_469;
  DWORD (__thiscall *vf14_sub_10260100)(_DWORD *this, int);
  void (__thiscall *vf15_sub_10260130)(int *this, int);
  int (__thiscall *vf16_sub_1025FF00)(void *this);
  int (__thiscall *vf17_sub_1025FF10)(_DWORD *this);
  void *vf18_nullsub_527;
  void *vf19_nullsub_528;
  void *vf20_nullsub_470;
  int (__thiscall *vf21_sub_102607F0)(_DWORD *this, BOOL);
  int (__thiscall *vf22_sub_1025EDE0)(_DWORD *this, int, _QWORD *, _QWORD *);
  int (__thiscall *vf23_sub_10260390)(int this, int, _DWORD *, _QWORD *);
  int (__thiscall *vf24_sub_1025F3D0)(DWORD_PTR dwUser);
  int (__thiscall *vf25_sub_1025F530)(int this);
  int (__thiscall *vf26_sub_1025EC40)(void *this);
  BOOL (__thiscall *vf27_sub_1025EE60)(int this);
  int (__thiscall *vf28_sub_1025F2F0)(int this);
  void *vf29_sub_1025ED20;
  int (__thiscall *vf30_sub_1025ED30)(int *this);
  int (__thiscall *vf31_sub_1025F590)(char *this);
  int (__thiscall *vf32_sub_1025F640)(char *this);
  int (__thiscall *vf33_sub_1025EC80)(_DWORD *this);
  int (__thiscall *vf34_sub_1025ECC0)(int *this);
  int (__thiscall *vf35_sub_1025ED60)(_DWORD *this);
  int (__thiscall *vf36_Movie_CreateFrameTextures)(int this, int);
  int (__thiscall *vf37_sub_1025ECE0)(_DWORD *this, int);
  int (__thiscall *vf38_sub_1025F000)(char *this, int *);
  int (__thiscall *vf39_sub_1025F190)(char *this, LPCRITICAL_SECTION);
  BOOL (__thiscall *vf40_sub_1025EF70)(int this);
  int (__thiscall *vf41_sub_1025EFA0)(int this);
  int (__thiscall *vf42_sub_1025EF20)(_DWORD *this, int);
  int (__thiscall *vf43_sub_1021E120)(_DWORD *this, _BYTE *);
  int (__stdcall *vf44_sub_1021E000)(int);
  unsigned int (__thiscall *vf45_sub_1025FF70)(int this, int, int);
  void (__thiscall *vf46_sub_102600C0)(int *this, int);
  int (__thiscall *vf47_sub_1025FE40)(_DWORD *this);
  int (__thiscall *vf48_sub_102601E0)(int *this, __int64, __int64);
};

/* 544 */
struct Vt_1033BE90
{
  struct Vt_1033BE90_vtbl *vftable;
  unsigned __int8 gap_4[8];
  unsigned int field_C;
  unsigned int field_10;
  unsigned __int8 gap_14[204];
  unsigned int field_E0;
  unsigned int field_E4;
  unsigned __int8 gap_E8[120];
  unsigned int field_160;
  unsigned int field_164;
  unsigned int field_168;
  unsigned int field_16C;
  unsigned int field_170;
};

/* 545 */
struct Vt_1033C8F0_vtbl
{
  int (__thiscall *vf0_sub_102391A0)(void *this, int a1);
  int (__thiscall *vf1_sub_102391D0)(int *this);
  void *vf2_YkZip_vf008;
  void *vf3_YkZip_vf009;
  char *(__thiscall *vf4_Ui_DrawKeyPrompt)(char **this);
  void *vf5_YkZip_vf011;
  void *vf6_nullsub_321;
  void *vf7_YkZip_vf013;
  void *vf8_YkZip_vf014;
  void *vf9_YkZip_vf015;
  char (__fastcall *vf10_sub_10239240)(int, int, int);
  void *vf11_YkZip_vf017;
  void *vf12_YkZip_vf018;
  void *vf13_YkZip_vf019;
  void *vf14_YkZip_vf020;
  void *vf15_YkZip_vf021;
  void *vf16_YkZip_vf022;
};

/* 546 */
struct Vt_1033C8F0
{
  struct Vt_1033C8F0_vtbl *vftable;
  unsigned __int8 gap_4[4];
  unsigned int field_8;
  unsigned __int8 gap_C[12];
  unsigned int field_18;
  unsigned int field_1C;
  unsigned int field_20;
  unsigned int field_24;
  unsigned int field_28;
  unsigned int field_2C;
  unsigned int field_30;
  unsigned int field_34;
};

/* 586 */
struct CMoD3mSpecularElem_vtbl
{
  void *vf0_CMoD3mSpecularElem_GetRuntimeClass;
  int (__thiscall *vf1_sub_10047520)(void *this, _DWORD *);
  unsigned int (__thiscall *vf2_sub_1002C930)(_DWORD *this, char);
  int (__thiscall *vf3_sub_1002C920)(_DWORD *this);
  void *vf4_CApp_vf004;
  void *vf5_CApp_vf005;
  _DWORD *(__thiscall *vf6_CMoD3mSpecularElem_ScalarDeletingDtor)(CMoD3mSpecularElem *C, char);
  void *vf7_CMoTask_vf007;
  void *vf8_sub_10045A40;
  void *vf9_CMoTask_vf009;
  void (__stdcall *vf10_CMoTask_vf010)(int);
  void *vf11_sub_10046070;
  BOOL (__stdcall *vf12_CMoD3mSpecularElem_vf012)(int);
  void (__stdcall *vf13_CMoElem_vf013)(int);
  void (__stdcall *vf14_CMoElem_vf014)(int);
  void *vf15_CMoElem_vf015;
  bool (__thiscall *vf16_sub_10044AC0)(_DWORD *this);
  double (__thiscall *vf17_sub_10041A40)(int this);
  void *vf18_sub_100412B0;
  char (__thiscall *vf19_sub_10041160)(CMoD3mSpecularElem *this);
  int *(__thiscall *vf20_CXi471011C8_VT_1032AD80_SubmitVertices)(int **this);
  void *vf21_sub_10045AE0;
  void *vf22_sub_10045BB0;
};

/* 634 */
struct CMoTask_VT_10329C70_vtbl
{
  void *vf0_CMoTask_GetRuntimeClass_variant12;
  int (__stdcall *vf1_sub_1006B4C0)(_DWORD *);
  unsigned int (__thiscall *vf2_sub_1002C930)(_DWORD *this, char);
  int (__thiscall *vf3_sub_1002C920)(_DWORD *this);
  void *vf4_CApp_vf004;
  void *vf5_CApp_vf005;
  _DWORD *(__thiscall *vf6_CMoTask_VT_10329C70_ScalarDeletingDtor)(CMoTask_VT_10329C70 *C, char);
  void *vf7_CMoTask_vf007;
  char (__thiscall *vf8_sub_1006B480)(CMoTask_VT_10329C70 *this);
  void *vf9_CMoTask_vf009;
  void (__stdcall *vf10_CMoTask_vf010)(int);
};

/* 644 */
struct CTsZoneMap_vtbl
{
  void *vf0_CTsZoneMap_GetRuntimeClass;
  int (__thiscall *vf1_sub_10178D40)(CTsZoneMap *this, int *);
  unsigned int (__thiscall *vf2_sub_1002C930)(_DWORD *this, char);
  int (__thiscall *vf3_sub_1002C920)(_DWORD *this);
  void *vf4_CApp_vf004;
  void *vf5_CApp_vf005;
  void *(__thiscall *vf6_CTsZoneMap_ScalarDeletingDtor)(CTsZoneMap *C, char);
  void *vf7_CMoTask_vf007;
  void *vf8_sub_10178B30;
  void *vf9_CMoTask_vf009;
  void (__stdcall *vf10_CMoTask_vf010)(int);
};

/* 654 */
struct CYyOs2VtxBuffer_vtbl
{
  void *vf0_CYyOs2VtxBuffer_GetRuntimeClass;
  void *vf1_CApp_vf001;
  unsigned int (__thiscall *vf2_sub_1002C930)(_DWORD *this, char);
  int (__thiscall *vf3_sub_1002C920)(_DWORD *this);
  void *vf4_CApp_vf004;
  void *vf5_CApp_vf005;
  void *(__thiscall *vf6_CYyOs2VtxBuffer_ScalarDeletingDtor)(CYyOs2VtxBuffer *C, char);
  void *vf7_CMoTask_vf007;
  char (__thiscall *vf8_sub_1002C990)(CYyOs2VtxBuffer *this);
  void *vf9_CMoTask_vf009;
  void (__stdcall *vf10_CMoTask_vf010)(int);
};

