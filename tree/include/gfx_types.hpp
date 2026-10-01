// gfx_types.hpp -- game types from the IDB that this tree does not define itself.
#pragma once
class CApp;
class CDx;
class CMoActorColorDriveTask;
class CMoActorRotationDriveTask;
class CMoAttachments;
class CMoAttachments_VT_1032AE94;
class CMoAttachments_VT_1032AF44;
class CMoAttachments_VT_1032AF8C;
class CMoAttachments_VT_1032B140;
class CMoAttachments_VT_1032B940;
class CMoAttachments_VT_1032BAF0;
class CMoAttachments_VT_1032BB38;
class CMoAttachments_VT_1032BBAC;
class CMoAttachments_VT_1032BBF4;
class CMoAttachments_VT_1032BC68;
class CMoAttachments_VT_1032BC84;
class CMoAttachments_VT_1032BCCC;
class CMoAttachments_VT_1032BD14;
class CMoAttachments_VT_1032BD5C;
class CMoAttachments_VT_1032BDA4;
class CMoAttachments_VT_1032BDEC;
class CMoAttachments_VT_1032BE34;
class CMoAttachments_VT_1032BE7C;
class CMoAttachments_VT_1032BEC4;
class CMoAttachments_VT_1032BF0C;
class CMoAttachments_VT_1032BF54;
class CMoAttachments_VT_1032BF9C;
class CMoAttachments_VT_1032BFE4;
class CMoAttachments_VT_1032C02C;
class CMoAttachments_VT_1032C4E8;
class CMoAttachments_VT_1032C628;
class CMoAttachments_VT_1032E7EC;
class CMoCamera;
class CMoCameraTask;
class CMoCelestial;
class CMoD3aElem;
class CMoD3aLightmapElem;
class CMoD3mSpecularElem;
class CMoDX;
class CMoDisintegrateProgElem;
class CMoDist;
class CMoDistModelElem;
class CMoDistMorphElem;
class CMoDistRingElem;
class CMoDxStatusMng;
class CMoElem;
class CMoElem_VT_1032ADE8;
class CMoGeneratorClone;
class CMoGeneratorClone_VT_1032B114;
class CMoGeneratorClone_VT_1032B69C;
class CMoLfdElem;
class CMoLockColorDriveTask;
class CMoLockLookAtDriveTask;
class CMoNullProgElem;
class CMoOT;
class CMoOcclusionMng;
class CMoOtTask;
class CMoPathDriveActorTask;
class CMoPathObject;
class CMoPointLightProgElem;
class CMoProcessor;
class CMoProcessor_VT_1032C1C8;
class CMoProcessor_VT_1032C298;
class CMoProcessor_VT_1032C368;
class CMoResource;
class CMoResourceMng;
class CMoResource_VT_1032C55C;
class CMoResource_VT_1032C588;
class CMoResource_VT_1032C5FC;
class CMoResource_VT_1032C670;
class CMoResource_VT_1032C69C;
class CMoResource_VT_1032C6C8;
class CMoResource_VT_1032C6F4;
class CMoResource_VT_1032C720;
class CMoResource_VT_1032C74C;
class CMoResource_VT_1032C778;
class CMoResource_VT_1032C7A4;
class CMoResource_VT_1032C7D0;
class CMoResource_VT_1032C7FC;
class CMoResource_VT_1032C828;
class CMoResource_VT_1032C854;
class CMoResource_VT_1032C880;
class CMoResource_VT_1032C8AC;
class CMoResource_VT_1032C934;
class CMoResource_VT_10336698;
class CMoResource_VT_103366C4;
class CMoResource_VT_103366F0;
class CMoResource_VT_103367E8;
class CMoSchedularTask;
class CMoSkeletonElem;
class CMoSphRes;
class CMoSpline;
class CMoTask;
class CMoTaskMng;
class CMoTask_VT_10329C70;
class CMoTask_VT_1032A904;
class CMoTask_VT_1032AF60;
class CMoTask_VT_1032AFA8;
class CMoTask_VT_1032B654;
class CMoTask_VT_1032B95C;
class CMoTask_VT_1032C490;
class CMoTask_VT_1032C908;
class CMoTask_VT_1032D6D4;
class CMoTask_VT_1032E808;
class CMoVuAnmElem;
class CMoVuDist;
class CMoVuLineProgElem;
class CShader;
class CTsZoneMap;
class CXiActor;
class CXiActorNameDraw;
class CXiMovie;
class CXiOpening;
class CXiSkeletonActor;
class CXiSkeletonActorRes;
class CYyModel;
class CYyModelBase;
class CYyModelDt;
class CYyModelDt_VT_1032A430;
class CYyModelDt_VT_1032A72C;
class CYyOs2VtxBuffer;
class CYyTex;
class CYyTexBase;
class CYyTexMng;
class CYyVbMng;
class Obj_104E1B90;
class Obj_104E1BF8;
class Obj_104E1D4C;
class Obj_10621944;
// Game types exported from the IDB (IDA print_decls); D3D types come from xi_d3d8.hpp.

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

/* 16 */
typedef char CHAR;

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


