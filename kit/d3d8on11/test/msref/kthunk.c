/* kthunk.c -- a stand-in for d3d8thk.dll (the D3D8 runtime's calls into the NT kernel's DirectDraw), so that
   Microsoft's own DirectX 8.1 runtime (d3d8.dll) and its reference device (d3dref8.dll) run under Wine as a test
   reference (build.sh; test_d3d8on11.py). It emulates a display driver with no 3D and system-memory surfaces only:
   the reference device rasterizes in software into them. Structures from Microsoft's WDK 7.1 headers (ddrawint.h,
   d3dnthal.h, ddrawi.h), which stay outside the repository (build.sh takes their directory). The thunks this file does
   not implement are logging stubs (stubs.inc, made by gen_thunks.py from the real d3d8thk.dll's exports). */
#define __out_ecount_part_opt(a,b)
#define __in_ecount_opt(a)
#define __out_bcount_opt(a)
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <ddraw.h>
#include <ddrawi.h>
#include <ddrawint.h>
#include <d3dnthal.h>
static void lg(const char *f, ...) {
  FILE *o = fopen("thk.log", "a"); if (!o) return;
  va_list a; va_start(a, f); vfprintf(o, f, a); va_end(a); fputc('\n', o); fclose(o);
}
#define FAKE_HDD ((HANDLE)0xDD000001)
#define DISP_W 1024
#define DISP_H 768

/* GDI32's DdCreateDirectDrawObject (GdiEntry1) and DdQueryDisplaySettingsUniqueness (GdiEntry13), which Wine does not
   implement; d3d8.dll's import slots for them are re-aimed here */
static BOOL WINAPI my_GdiEntry1(LPDDRAWI_DIRECTDRAW_GBL g, HDC hdc) {
  lg("GdiEntry1 %p %p", g, hdc); g->hDD = (ULONG_PTR)FAKE_HDD; return TRUE;
}
static ULONG WINAPI my_GdiEntry13(void) { return 1; }
static void patch_iat(HMODULE m) {
  BYTE *b = (BYTE *)m; IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS *)(b + ((IMAGE_DOS_HEADER *)b)->e_lfanew);
  IMAGE_DATA_DIRECTORY dd = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
  for (IMAGE_IMPORT_DESCRIPTOR *id = (IMAGE_IMPORT_DESCRIPTOR *)(b + dd.VirtualAddress); id->Name; id++) {
    if (_stricmp((char *)(b + id->Name), "gdi32.dll")) continue;
    IMAGE_THUNK_DATA *on = (IMAGE_THUNK_DATA *)(b + id->OriginalFirstThunk), *ft = (IMAGE_THUNK_DATA *)(b + id->FirstThunk);
    for (; on->u1.AddressOfData; on++, ft++) {
      if (IMAGE_SNAP_BY_ORDINAL(on->u1.Ordinal)) continue;
      const char *n = (char *)((IMAGE_IMPORT_BY_NAME *)(b + on->u1.AddressOfData))->Name; void *to = NULL;
      if (!strcmp(n, "GdiEntry1")) to = (void *)my_GdiEntry1; else if (!strcmp(n, "GdiEntry13")) to = (void *)my_GdiEntry13;
      if (!to) continue;
      DWORD old; VirtualProtect(&ft->u1.Function, 4, PAGE_READWRITE, &old); ft->u1.Function = (ULONG_PTR)to; VirtualProtect(&ft->u1.Function, 4, old, &old);
      lg("patched %s", n);
    }
  }
}
BOOL WINAPI DllMain(HINSTANCE h, DWORD r, void *x) {
  if (r == DLL_PROCESS_ATTACH) { HMODULE m = GetModuleHandleA("d3d8.dll"); lg("attach: d3d8.dll at %p", m); if (m) patch_iat(m); }
  return TRUE;
}

/* NtGdiDdQueryDirectDrawObject: the display driver's HAL information */
#define IMPL_OsThunkDdQueryDirectDrawObject
BOOL WINAPI OsThunkDdQueryDirectDrawObject(HANDLE hdd, DD_HALINFO *hi, DWORD *cbflags, LPD3DNTHAL_CALLBACKS d3dcb,
    LPD3DNTHAL_GLOBALDRIVERDATA d3dgd, PDD_D3DBUFCALLBACKS bufcb, LPDDSURFACEDESC texfmts, DWORD *nheaps, VIDEOMEMORY *heaps,
    DWORD *nfourcc, DWORD *fourcc) {
  lg("OsThunkDdQueryDirectDrawObject %p %p %p %p %p %p %p %p %p %p %p", hdd, hi, cbflags, d3dcb, d3dgd, bufcb, texfmts, nheaps, heaps, nfourcc, fourcc);
  if (hi) {
    memset(hi, 0, sizeof *hi); hi->dwSize = sizeof *hi;
    hi->vmiData.dwDisplayWidth = DISP_W; hi->vmiData.dwDisplayHeight = DISP_H; hi->vmiData.lDisplayPitch = DISP_W * 4;
    DDPIXELFORMAT *pf = &hi->vmiData.ddpfDisplay; pf->dwSize = sizeof *pf; pf->dwFlags = DDPF_RGB; pf->dwRGBBitCount = 32;
    pf->dwRBitMask = 0xFF0000; pf->dwGBitMask = 0xFF00; pf->dwBBitMask = 0xFF;
    hi->ddCaps.dwSize = sizeof hi->ddCaps;
  }
  if (cbflags) cbflags[0] = cbflags[1] = cbflags[2] = 0;
  if (d3dcb) memset(d3dcb, 0, sizeof *d3dcb);
  if (d3dgd) memset(d3dgd, 0, sizeof *d3dgd);
  if (bufcb) memset(bufcb, 0, sizeof *bufcb);
  if (nheaps) *nheaps = 0;
  if (nfourcc) *nfourcc = 0;
  return TRUE;
}

typedef struct { HANDLE h; PDD_SURFACE_GLOBAL sg; PDD_SURFACE_LOCAL sl; } SObj;
static SObj g_so[4096]; static int g_nso;
/* NtGdiDdCreateSurfaceObject: register a system-memory surface with the kernel; a handle back. Ours is a counter */
static LONG g_surf = 0x5F000000;
#define IMPL_OsThunkDdCreateSurfaceObject
HANDLE WINAPI OsThunkDdCreateSurfaceObject(HANDLE hdd, HANDLE hs, PDD_SURFACE_LOCAL sl, PDD_SURFACE_MORE sm, PDD_SURFACE_GLOBAL sg, BOOL complete) {
  HANDLE h = hs ? hs : (HANDLE)(ULONG_PTR)InterlockedIncrement(&g_surf);
  if (!hs && sg && g_nso < 4096) { SObj *o = &g_so[g_nso++]; o->h = h; o->sg = sg; o->sl = sl; }
  lg("OsThunkDdCreateSurfaceObject %p %p %p %p %p %d -> %p: caps %08lx, %lux%lu pitch %ld fp %08lx", hdd, hs, sl, sm, sg, complete, h,
     sl ? sl->ddsCaps.dwCaps : 0, sg ? (unsigned long)sg->wWidth : 0, sg ? (unsigned long)sg->wHeight : 0, sg ? sg->lPitch : 0, sg ? (unsigned long)sg->fpVidMem : 0);
  return h;
}
#define IMPL_OsThunkDdDeleteSurfaceObject
BOOL WINAPI OsThunkDdDeleteSurfaceObject(HANDLE h) { lg("OsThunkDdDeleteSurfaceObject %p", h); return TRUE; }
#define IMPL_OsThunkDdReenableDirectDrawObject
BOOL WINAPI OsThunkDdReenableDirectDrawObject(HANDLE hdd, BOOL *newmode) { lg("OsThunkDdReenableDirectDrawObject %p", hdd); if (newmode) *newmode = FALSE; return TRUE; }
#define IMPL_OsThunkDdDeleteDirectDrawObject
BOOL WINAPI OsThunkDdDeleteDirectDrawObject(HANDLE hdd) { lg("OsThunkDdDeleteDirectDrawObject %p", hdd); return TRUE; }

/* "video memory" surfaces the runtime asks the display driver for (the primary): ordinary memory here */
typedef struct { HANDLE h; BYTE *mem; LONG pitch; DWORD w, h_, bpp; } VSurf;
static VSurf g_vs[256]; static int g_nvs;
static VSurf *vs_find(HANDLE h) { for (int i = 0; i < g_nvs; i++) if (g_vs[i].h == h) return &g_vs[i]; return NULL; }
#define IMPL_OsThunkDdCanCreateSurface
DWORD WINAPI OsThunkDdCanCreateSurface(HANDLE hdd, PDD_CANCREATESURFACEDATA d) {
  lg("OsThunkDdCanCreateSurface caps %08lx", d->lpDDSurfaceDesc ? d->lpDDSurfaceDesc->ddsCaps.dwCaps : 0);
  d->ddRVal = DD_OK; return DDHAL_DRIVER_HANDLED;
}
#define IMPL_OsThunkDdCreateSurface
DWORD WINAPI OsThunkDdCreateSurface(HANDLE hdd, HANDLE *hs, DDSURFACEDESC *desc, DD_SURFACE_GLOBAL *sg, DD_SURFACE_LOCAL *sl,
    DD_SURFACE_MORE *sm, PDD_CREATESURFACEDATA cd, HANDLE *out) {
  DWORD n = cd->dwSCnt ? cd->dwSCnt : 1;
  for (DWORD i = 0; i < n; i++) {
    DWORD w = sg[i].wWidth ? sg[i].wWidth : DISP_W, h = sg[i].wHeight ? sg[i].wHeight : DISP_H;
    DWORD bpp = sg[i].ddpfSurface.dwRGBBitCount ? sg[i].ddpfSurface.dwRGBBitCount : 32;
    if (sl[i].ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE) { w = DISP_W; h = DISP_H; bpp = 32; }
    LONG pitch = (LONG)((w * bpp / 8 + 15) & ~15u);
    VSurf *v = &g_vs[g_nvs++]; v->h = (HANDLE)(ULONG_PTR)InterlockedIncrement(&g_surf); v->mem = calloc((size_t)pitch, h + 1);
    v->pitch = pitch; v->w = w; v->h_ = h; v->bpp = bpp;
    sg[i].fpVidMem = (FLATPTR)v->mem; sg[i].lPitch = pitch; if (!sg[i].wWidth) sg[i].wWidth = w; if (!sg[i].wHeight) sg[i].wHeight = h;
    out[i] = v->h;
    lg("OsThunkDdCreateSurface [%lu] caps %08lx %lux%lu bpp %lu -> %p mem %p pitch %ld", i, sl[i].ddsCaps.dwCaps, w, h, bpp, v->h, v->mem, pitch);
  }
  cd->ddRVal = DD_OK; return DDHAL_DRIVER_HANDLED;
}
#define IMPL_OsThunkDdDestroySurface
DWORD WINAPI OsThunkDdDestroySurface(HANDLE h, BOOL real) { lg("OsThunkDdDestroySurface %p %d", h, real); return DDHAL_DRIVER_HANDLED; }
#define IMPL_OsThunkDdLock
DWORD WINAPI OsThunkDdLock(HANDLE h, PDD_LOCKDATA d, HDC dc) {
  VSurf *v = vs_find(h);
  lg("OsThunkDdLock %p rect %lu (%ld,%ld) -> %p", h, d->bHasRect, d->rArea.left, d->rArea.top, v ? v->mem : NULL);
  if (!v) { d->ddRVal = DDERR_GENERIC; return DDHAL_DRIVER_HANDLED; }
  d->lpSurfData = v->mem + (d->bHasRect ? d->rArea.top * v->pitch + d->rArea.left * (LONG)(v->bpp / 8) : 0);
  d->ddRVal = DD_OK; return DDHAL_DRIVER_HANDLED;
}
#define IMPL_OsThunkDdUnlock
DWORD WINAPI OsThunkDdUnlock(HANDLE h, PDD_UNLOCKDATA d) { lg("OsThunkDdUnlock %p", h); d->ddRVal = DD_OK; return DDHAL_DRIVER_HANDLED; }
#define IMPL_OsThunkDdBlt
DWORD WINAPI OsThunkDdBlt(HANDLE dst, HANDLE src, PDD_BLTDATA d) {
  lg("OsThunkDdBlt %p <- %p flags %08lx", dst, src, d->dwFlags); d->ddRVal = DD_OK; return DDHAL_DRIVER_HANDLED;
}

/* NtGdiDdGetDC: a DC on a surface, which the runtime BitBlts to the window in Present. A memory DC over a DIB holding a
   copy of the surface (32 bits a pixel only) */
typedef struct { HDC dc; HBITMAP bm, old; } DCRec;
static DCRec g_dc[16];
#define IMPL_OsThunkDdGetDC
HDC WINAPI OsThunkDdGetDC(HANDLE h, PALETTEENTRY *pal) {
  SObj *o = NULL; for (int i = g_nso - 1; i >= 0; i--) if (g_so[i].h == h) { o = &g_so[i]; break; }
  if (!o) { lg("OsThunkDdGetDC %p: unknown", h); return NULL; }
  DWORD w = o->sg->wWidth, hh = o->sg->wHeight, bpp = o->sg->ddpfSurface.dwRGBBitCount ? o->sg->ddpfSurface.dwRGBBitCount : 32;
  BITMAPINFO bi; memset(&bi, 0, sizeof bi); bi.bmiHeader.biSize = sizeof bi.bmiHeader; bi.bmiHeader.biWidth = w; bi.bmiHeader.biHeight = -(LONG)hh;
  bi.bmiHeader.biPlanes = 1; bi.bmiHeader.biBitCount = 32; bi.bmiHeader.biCompression = BI_RGB;
  void *bits = NULL; HDC dc = CreateCompatibleDC(NULL); HBITMAP bm = CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &bits, NULL, 0);
  if (bits && bpp == 32) for (DWORD y = 0; y < hh; y++) memcpy((BYTE *)bits + y * w * 4, (BYTE *)o->sg->fpVidMem + y * o->sg->lPitch, w * 4);
  for (int i = 0; i < 16; i++) if (!g_dc[i].dc) { g_dc[i].dc = dc; g_dc[i].bm = bm; g_dc[i].old = SelectObject(dc, bm); break; }
  lg("OsThunkDdGetDC %p %lux%lu bpp %lu -> %p", h, w, hh, bpp, dc);
  return dc;
}
#define IMPL_OsThunkDdReleaseDC
BOOL WINAPI OsThunkDdReleaseDC(HANDLE h) {
  for (int i = 0; i < 16; i++) if (g_dc[i].dc) { SelectObject(g_dc[i].dc, g_dc[i].old); DeleteObject(g_dc[i].bm); DeleteDC(g_dc[i].dc); g_dc[i].dc = NULL; break; }
  return TRUE;
}
#include "stubs.inc"
