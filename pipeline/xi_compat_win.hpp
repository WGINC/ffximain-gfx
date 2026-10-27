// xi_compat_win.hpp -- Windows handle/string typedefs that decompiled signatures mention.
// Identical typedef redeclarations are legal C++, so these never clash with the IDB's own.
#pragma once
struct HINSTANCE__; struct HWND__; struct HDC__; struct HICON__; struct HBRUSH__; struct HMENU__;
struct HBITMAP__; struct HFONT__; struct HKEY__; struct HGLRC__; struct HPEN__; struct HRGN__; struct HMONITOR__;
typedef HINSTANCE__ *HINSTANCE; typedef HINSTANCE HMODULE; typedef HWND__ *HWND; typedef HDC__ *HDC;
typedef HICON__ *HICON; typedef HICON HCURSOR; typedef HBRUSH__ *HBRUSH; typedef HMENU__ *HMENU;
typedef HBITMAP__ *HBITMAP; typedef HFONT__ *HFONT; typedef HKEY__ *HKEY; typedef HPEN__ *HPEN;
typedef HRGN__ *HRGN; typedef HMONITOR__ *HMONITOR; typedef void *HANDLE; typedef void *HGDIOBJ;
struct HIMC__; typedef HIMC__ *HIMC;                 // imm.h: DECLARE_HANDLE(HIMC)
typedef int (__stdcall *FARPROC)();                  // minwindef.h: INT_PTR (WINAPI *FARPROC)(), INT_PTR = int on x86
typedef void *LPVOID; typedef const void *LPCVOID; typedef char *LPSTR; typedef const char *LPCSTR;
struct _SYSTEMTIME; typedef struct _SYSTEMTIME *LPSYSTEMTIME;
typedef unsigned short *LPWSTR; typedef const unsigned short *LPCWSTR; typedef const unsigned short *LPCWCH;
typedef const char *LPCCH; typedef unsigned int UINT; typedef unsigned int WPARAM; typedef int LPARAM;
typedef int LRESULT; typedef int HRESULT; typedef unsigned int SIZE_T; typedef unsigned long ULONG_PTR_;
typedef unsigned int MMRESULT; typedef unsigned int DWORD_PTR_;
// opaque types FLIRT attached to game functions (the names are false positives; only pointers are used)
struct Iostream_init; struct type_info; struct exception;
namespace std { struct codecvt_base; }
struct tagRECT; typedef tagRECT *LPRECT;
struct tagMSG; typedef tagMSG *LPMSG; typedef tagMSG MSG; struct tagPOINT; typedef tagPOINT *LPPOINT;
struct tagSIZE; typedef tagSIZE *LPSIZE; typedef void *LPUNKNOWN_; typedef unsigned int *LPDWORD; typedef int BOOL_;
// MSVC runtime classes FLIRT names in the IDB (the undecorator's DNameNode): used only through pointers
struct DNameNode;
// minwindef.h / windef.h / minwinbase.h / winuser.h / imm.h / mmsystem.h / winnt.h: the layouts as the SDK declares
// them (LONG, DWORD, UINT are 4 bytes on x86), for the locals and pointers Ghidra types with them; tagPOINT and
// tagRECT come from the IDB (gfx_types_game.h), included first
struct _FILETIME { unsigned int dwLowDateTime; unsigned int dwHighDateTime; };
struct _WIN32_FIND_DATAA { unsigned int dwFileAttributes; _FILETIME ftCreationTime; _FILETIME ftLastAccessTime;
    _FILETIME ftLastWriteTime; unsigned int nFileSizeHigh; unsigned int nFileSizeLow; unsigned int dwReserved0;
    unsigned int dwReserved1; char cFileName[260]; char cAlternateFileName[14]; };
struct tagWINDOWPLACEMENT { unsigned int length; unsigned int flags; unsigned int showCmd; tagPOINT ptMinPosition;
    tagPOINT ptMaxPosition; tagRECT rcNormalPosition; }; typedef tagWINDOWPLACEMENT WINDOWPLACEMENT;
struct tagCOMPOSITIONFORM { unsigned int dwStyle; tagPOINT ptCurrentPos; tagRECT rcArea; };
struct timecaps_tag { unsigned int wPeriodMin; unsigned int wPeriodMax; };
union _LARGE_INTEGER { struct { unsigned int LowPart; int HighPart; } u; long long QuadPart; }; typedef _LARGE_INTEGER LARGE_INTEGER;
struct _SECURITY_ATTRIBUTES; typedef _SECURITY_ATTRIBUTES *LPSECURITY_ATTRIBUTES;
struct HKL__; typedef HKL__ *HKL; typedef void *PVOID; typedef void *HGLOBAL;
// imm.h CANDIDATEFORM { DWORD dwIndex; DWORD dwStyle; POINT ptCurrentPos; RECT rcArea; }; winbase.h
// LPTHREAD_START_ROUTINE: DWORD (WINAPI *)(LPVOID)
struct tagCANDIDATEFORM { unsigned int dwIndex; unsigned int dwStyle; tagPOINT ptCurrentPos; tagRECT rcArea; };
typedef unsigned int (__stdcall *LPTHREAD_START_ROUTINE)(void *); typedef tagCANDIDATEFORM *LPCANDIDATEFORM; typedef tagCOMPOSITIONFORM *LPCOMPOSITIONFORM;
// winsock2.h (Visual Studio 6 VC98/INCLUDE/WINSOCK2.H): the types the SDK prototypes of the WS2_32 imports mention
// (named by their import library, import_libs.py)
typedef unsigned char u_char; typedef unsigned short u_short; typedef unsigned int u_int; typedef unsigned long u_long;
typedef u_int SOCKET;
struct hostent { char *h_name; char **h_aliases; short h_addrtype; short h_length; char **h_addr_list; };
struct in_addr { union { struct { u_char s_b1, s_b2, s_b3, s_b4; } S_un_b; struct { u_short s_w1, s_w2; } S_un_w;
    u_long S_addr; } S_un; };
struct WSAData { unsigned short wVersion; unsigned short wHighVersion; char szDescription[257]; char szSystemStatus[129];
    unsigned short iMaxSockets; unsigned short iMaxUdpDg; char *lpVendorInfo; }; typedef WSAData WSADATA; typedef WSAData *LPWSADATA;
static_assert(sizeof(hostent) == 16 && sizeof(in_addr) == 4 && sizeof(WSADATA) == 400, "winsock2.h sizes");
static_assert(sizeof(_WIN32_FIND_DATAA) == 320 && sizeof(WINDOWPLACEMENT) == 44 && sizeof(tagCOMPOSITIONFORM) == 28
              && sizeof(tagCANDIDATEFORM) == 32, "SDK sizes");
