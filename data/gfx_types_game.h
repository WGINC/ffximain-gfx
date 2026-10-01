// Game types exported from the IDB (IDA print_decls); D3D types come from xi_d3d8.hpp.
#pragma once
/* 65 */
typedef struct tagBITMAPINFO BITMAPINFO;

/* 3 */
typedef unsigned int DWORD;

/* 41 */
typedef int LONG;

/* 38 */
typedef unsigned __int16 WORD;

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

/* 62 */
typedef unsigned __int8 BYTE;

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

/* 33 */
typedef int BOOL;

/* 237 */
#pragma pack(push, 1)
/// recon: tree/include/CYyObject.hpp, sizeof 0x4 from the binary's descriptor
struct CYyObject
{
  void **vftable;
};
#pragma pack(pop)

/* 244 */
#pragma pack(push, 1)
struct CDx
{
  CYyObject base;
  unsigned __int8 _pad_4[4];
  IDirect3D8 *pD3D;
  IDirect3DDevice8 *pDevice;
  unsigned __int8 _pad_10[2624];
};
#pragma pack(pop)

/* 16 */
typedef char CHAR;

/* 238 */
#pragma pack(push, 1)
/// recon: tree/include/CMoTask.hpp, sizeof 0x34 from the binary's descriptor
struct CMoTask
{
  CYyObject base;
  float field_4;
  float field_8;
  float field_C;
  unsigned __int8 _pad_10[36];
};
#pragma pack(pop)

/* 246 */
#pragma pack(push, 1)
/// recon: tree/include/CMoActorColorDriveTask.hpp, sizeof 0x88 from the binary's descriptor
struct CMoActorColorDriveTask
{
  CMoTask base;
  unsigned __int8 _pad_34[84];
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

/* 306 */
#pragma pack(push, 1)
/// recon: tree/include/CMoPathDriveActorTask.hpp, sizeof 0xB0 from the binary's descriptor
struct CMoPathDriveActorTask
{
  CMoTask base;
  unsigned __int8 _pad_34[124];
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

/* 296 */
#pragma pack(push, 1)
/// recon: tree/include/CMoResource.hpp, sizeof 0x30 from the binary's descriptor
struct CMoResource
{
  CYyObject base;
  unsigned __int32 field_4;
  unsigned __int8 _pad_8[12];
  unsigned __int32 field_14;
  unsigned __int8 _pad_18[24];
};
#pragma pack(pop)

/* 315 */
#pragma pack(push, 1)
/// recon: tree/include/CMoResource_VT_103366C4.hpp, sizeof 0x30 from the binary's descriptor
struct CMoResource_VT_103366C4
{
  CMoResource base;
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
/// recon: tree/include/CMoTask_VT_1032B654.hpp, sizeof 0x34 from the binary's descriptor
struct CMoTask_VT_1032B654
{
  CMoTask base;
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

/* 429 */
#pragma pack(push, 1)
/// recon: tree/include/CYyModelDt.hpp, sizeof 0x3C from the binary's descriptor
struct CYyModelDt
{
  CYyObject base;
  unsigned __int8 _pad_4[56];
};
#pragma pack(pop)

/* 430 */
#pragma pack(push, 1)
/// recon: tree/include/CYyModelDt_VT_1032A430.hpp, sizeof 0x3C from the binary's descriptor
struct CYyModelDt_VT_1032A430
{
  CYyModelDt base;
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

/* 170 */
struct HBITMAP__
{
  int unused;
};

/* 161 */
struct HICON__
{
  int unused;
};

/* 2 */
struct HINSTANCE__
{
  int unused;
};

/* 27 */
struct HKEY__
{
  int unused;
};

/* 153 */
typedef int HRESULT;

/* 127 */
struct HWND__
{
  int unused;
};

/* 121 */
typedef int LONG_PTR;

/* 120 */
typedef LONG_PTR LPARAM;

/* 159 */
typedef LONG_PTR LRESULT;

/* 129 */
typedef struct tagPOINT POINT;

/* 130 */
struct tagPOINT
{
  LONG x;
  LONG y;
};

/* 14 */
typedef unsigned int UINT;

/* 19 */
// (wchar_t is built in)

/* 18 */
typedef wchar_t WCHAR;

/* 49 */
typedef unsigned int UINT_PTR;

/* 119 */
typedef UINT_PTR WPARAM;

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

/* 126 */
typedef struct HWND__ *HWND;

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

/* 514 */
struct tagPALETTEENTRY
{
  BYTE peRed;
  BYTE peGreen;
  BYTE peBlue;
  BYTE peFlags;
};

/* 64 */
struct tagRECT
{
  LONG left;
  LONG top;
  LONG right;
  LONG bottom;
};

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

