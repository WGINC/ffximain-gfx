// xi_compat_win.hpp -- Windows handle/string typedefs that decompiled signatures mention.
// Identical typedef redeclarations are legal C++, so these never clash with the IDB's own.
#pragma once
struct HINSTANCE__; struct HWND__; struct HDC__; struct HICON__; struct HBRUSH__; struct HMENU__;
struct HBITMAP__; struct HFONT__; struct HKEY__; struct HGLRC__; struct HPEN__; struct HRGN__; struct HMONITOR__;
typedef HINSTANCE__ *HINSTANCE; typedef HINSTANCE HMODULE; typedef HWND__ *HWND; typedef HDC__ *HDC;
typedef HICON__ *HICON; typedef HICON HCURSOR; typedef HBRUSH__ *HBRUSH; typedef HMENU__ *HMENU;
typedef HBITMAP__ *HBITMAP; typedef HFONT__ *HFONT; typedef HKEY__ *HKEY; typedef HPEN__ *HPEN;
typedef HRGN__ *HRGN; typedef HMONITOR__ *HMONITOR; typedef void *HANDLE; typedef void *HGDIOBJ;
typedef void *LPVOID; typedef const void *LPCVOID; typedef char *LPSTR; typedef const char *LPCSTR;
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
