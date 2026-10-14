// d8_main.cpp -- d3d8on11: the module, the log, formats, caps and the IDirect3D8 object.
#include "d8.h"
#include <stdio.h>
#include <stdarg.h>
#include <set>
#include <string>

// ---- log --------------------------------------------------------------------------------------------------------------
static FILE *g_log; static CRITICAL_SECTION g_logLock; static bool g_logInit;
static void log_open() {
  if (g_logInit) return;
  g_logInit = true; InitializeCriticalSection(&g_logLock);
  char p[MAX_PATH]; HMODULE m = NULL;
  GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&log_open, &m);
  GetModuleFileNameA(m, p, sizeof p);
  char *s = strrchr(p, '\\'); strcpy(s ? s + 1 : p, "d3d8on11.log");
  g_log = fopen(p, "w");
}
void d8_log(const char *fmt, ...) {
  log_open(); if (!g_log) return;
  EnterCriticalSection(&g_logLock);
  va_list a; va_start(a, fmt); vfprintf(g_log, fmt, a); va_end(a); fputc('\n', g_log); fflush(g_log);
  LeaveCriticalSection(&g_logLock);
}
static std::set<std::string> g_seen;
void d8_once(const char *key, const char *fmt, ...) {
  log_open();
  EnterCriticalSection(&g_logLock);
  bool first = g_seen.insert(key).second;
  LeaveCriticalSection(&g_logLock);
  if (!first) return;
  char b[1024]; va_list a; va_start(a, fmt); vsnprintf(b, sizeof b, fmt, a); va_end(a);
  d8_log("%s", b);
}
void d8_notimpl(const char *what) { d8_once(what, "not implemented: %s (failed)", what); }

// ---- formats (D3DFORMAT numbers from d3d8types.h; DXGI from dxgiformat.h) ------------------------------------------------
FmtInfo d8_fmt(D3DFORMAT f) {
  switch ((DWORD)f) {
  case D3DFMT_A8R8G8B8: return {DXGI_FORMAT_B8G8R8A8_UNORM, 32, 0, false};
  case D3DFMT_X8R8G8B8: return {DXGI_FORMAT_B8G8R8X8_UNORM, 32, 0, false};
  case D3DFMT_R5G6B5: return {DXGI_FORMAT_B5G6R5_UNORM, 16, 0, false};
  case D3DFMT_A1R5G5B5: return {DXGI_FORMAT_B5G5R5A1_UNORM, 16, 0, false};
  case D3DFMT_A4R4G4B4: return {DXGI_FORMAT_B4G4R4A4_UNORM, 16, 0, false};
  case D3DFMT_A8: return {DXGI_FORMAT_A8_UNORM, 8, 0, false};
  case D3DFMT_DXT1: return {DXGI_FORMAT_BC1_UNORM, 0, 8, false};
  case D3DFMT_DXT2: case D3DFMT_DXT3: return {DXGI_FORMAT_BC2_UNORM, 0, 16, false};
  case D3DFMT_DXT4: case D3DFMT_DXT5: return {DXGI_FORMAT_BC3_UNORM, 0, 16, false};
  case D3DFMT_D24S8: case D3DFMT_D24X8: return {DXGI_FORMAT_D24_UNORM_S8_UINT, 32, 0, true};
  case D3DFMT_D16: return {DXGI_FORMAT_D16_UNORM, 16, 0, true};
  case D3DFMT_D32: return {DXGI_FORMAT_D32_FLOAT, 32, 0, true};
  case D3DFMT_INDEX16: return {DXGI_FORMAT_R16_UINT, 16, 0, false};
  case D3DFMT_INDEX32: return {DXGI_FORMAT_R32_UINT, 32, 0, false};
  }
  return {DXGI_FORMAT_UNKNOWN, 0, 0, false};
}
UINT d8_pitch(D3DFORMAT f, UINT w) { FmtInfo i = d8_fmt(f); return i.block ? ((w + 3) / 4) * i.block : w * (i.bpp / 8); }
UINT d8_rows(D3DFORMAT f, UINT h) { return d8_fmt(f).block ? (h + 3) / 4 : h; }

// ---- caps ---------------------------------------------------------------------------------------------------------------
// The caps the game is given: what the owner's dgVoodoo answered on the owner's machine (probe8's caps.raw, phase 0
// run of 13 Oct; data/dgvoodoo_caps8.json decodes it). The game chooses its paths by caps, so it is told the same as
// under dgVoodoo and takes the paths its traces were recorded on. Where these promise something this device does not
// do yet (vertex blending, the game's own shaders), the call is logged when it comes, not hidden by smaller caps.
static const uint8_t kCaps[212] = {
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0A, 0x20, 0x20, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x80,
  0x03, 0x00, 0x00, 0x00, 0xF0, 0xBE, 0x1B, 0x01, 0xF2, 0x0E, 0x00, 0x00, 0x91, 0x61, 0x73, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00,
  0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x08, 0x42, 0x08, 0x00, 0xC5, 0xEC, 0x01, 0x00, 0x00, 0x07, 0x03, 0x07, 0x00, 0x07, 0x03, 0x07,
  0x00, 0x07, 0x03, 0x07, 0x3F, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00,
  0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0xB7, 0x43, 0xBA, 0x51, 0x80, 0x96, 0x18, 0xCB,
  0x80, 0x96, 0x18, 0xCB, 0x80, 0x96, 0x18, 0x4B, 0x80, 0x96, 0x18, 0x4B, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x08, 0x00, 0x18, 0x00,
  0xFF, 0xFF, 0xFE, 0x03, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x7B, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0x0F, 0x10, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0xFE, 0xFF, 0x00, 0x01, 0x00, 0x00, 0x04, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F,
};
void d8_caps(D3DCAPS8 *c) {
  static_assert(sizeof(D3DCAPS8) == sizeof kCaps, "D3DCAPS8 is 212 bytes in d3d8caps.h");
  memcpy(c, kCaps, sizeof kCaps);
}

// ---- IDirect3D8 ---------------------------------------------------------------------------------------------------------
HRESULT D8::QueryInterface(REFIID r, void **o) {
  if (IsEqualGUID(r, IID_IUnknown) || IsEqualGUID(r, IID_IDirect3D8)) { *o = this; AddRef(); return S_OK; }
  *o = NULL; return E_NOINTERFACE;
}
ULONG D8::AddRef() { return InterlockedIncrement(&ref); }
ULONG D8::Release() { LONG n = InterlockedDecrement(&ref); if (!n) delete this; return n; }
static void current_mode(D3DDISPLAYMODE *m) {
  DEVMODEA dm; memset(&dm, 0, sizeof dm); dm.dmSize = sizeof dm;
  EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &dm);
  m->Width = dm.dmPelsWidth; m->Height = dm.dmPelsHeight; m->RefreshRate = dm.dmDisplayFrequency;
  m->Format = D3DFMT_X8R8G8B8;
}
HRESULT D8::GetAdapterIdentifier(UINT a, DWORD, D3DADAPTER_IDENTIFIER8 *id) {
  if (a || !id) return D3DERR_INVALIDCALL;
  memset(id, 0, sizeof *id); strcpy(id->Driver, "d3d8on11.dll"); strcpy(id->Description, "d3d8on11 (Direct3D 8 on Direct3D 11)");
  return D3D_OK;
}
// modes: the current one only (windowed rendering); the game's mode list, if it shows one, has that mode
UINT D8::GetAdapterModeCount(UINT a) { return a ? 0 : 1; }
HRESULT D8::EnumAdapterModes(UINT a, UINT i, D3DDISPLAYMODE *m) { if (a || i || !m) return D3DERR_INVALIDCALL; current_mode(m); return D3D_OK; }
HRESULT D8::GetAdapterDisplayMode(UINT a, D3DDISPLAYMODE *m) { if (a || !m) return D3DERR_INVALIDCALL; current_mode(m); return D3D_OK; }
HRESULT D8::CheckDeviceType(UINT a, D3DDEVTYPE t, D3DFORMAT, D3DFORMAT, WINBOOL) {
  return !a && t == D3DDEVTYPE_HAL ? D3D_OK : D3DERR_NOTAVAILABLE;
}
HRESULT D8::CheckDeviceFormat(UINT a, D3DDEVTYPE t, D3DFORMAT, DWORD usage, D3DRESOURCETYPE rt, D3DFORMAT f) {
  if (a || t != D3DDEVTYPE_HAL) return D3DERR_NOTAVAILABLE;
  FmtInfo i = d8_fmt(f);
  bool ok = i.dxgi != DXGI_FORMAT_UNKNOWN && (!(usage & D3DUSAGE_DEPTHSTENCIL) || i.depth) && rt != D3DRTYPE_VOLUMETEXTURE;
  if (!ok) d8_once((std::string("cdf") + std::to_string(f) + "/" + std::to_string(usage)).c_str(),
                   "CheckDeviceFormat: format %lu usage 0x%lX type %d answered not available", (unsigned long)f, (unsigned long)usage, (int)rt);
  return ok ? D3D_OK : D3DERR_NOTAVAILABLE;
}
HRESULT D8::CheckDeviceMultiSampleType(UINT a, D3DDEVTYPE, D3DFORMAT, WINBOOL, D3DMULTISAMPLE_TYPE m) {
  return !a && m == D3DMULTISAMPLE_NONE ? D3D_OK : D3DERR_NOTAVAILABLE;
}
HRESULT D8::CheckDepthStencilMatch(UINT a, D3DDEVTYPE, D3DFORMAT, D3DFORMAT, D3DFORMAT ds) {
  return !a && d8_fmt(ds).depth ? D3D_OK : D3DERR_NOTAVAILABLE;
}
HRESULT D8::GetDeviceCaps(UINT a, D3DDEVTYPE t, D3DCAPS8 *c) {
  if (a || t != D3DDEVTYPE_HAL || !c) return D3DERR_INVALIDCALL;
  d8_caps(c); return D3D_OK;
}
HMONITOR D8::GetAdapterMonitor(UINT a) { POINT p = {0, 0}; return a ? NULL : MonitorFromPoint(p, MONITOR_DEFAULTTOPRIMARY); }
HRESULT D8::CreateDevice(UINT a, D3DDEVTYPE t, HWND focus, DWORD beh, D3DPRESENT_PARAMETERS *pp, IDirect3DDevice8 **out) {
  if (!out) return D3DERR_INVALIDCALL;
  *out = NULL;
  if (a || t != D3DDEVTYPE_HAL || !pp) return D3DERR_INVALIDCALL;
  Dev *d = new Dev();
  HRESULT hr = d->init(this, focus, beh, pp);
  if (FAILED(hr)) { d8_log("CreateDevice failed: 0x%08lX", (unsigned long)hr); d->Release(); return hr; }
  *out = d; return D3D_OK;
}

extern "C" __declspec(dllexport) IDirect3D8 *WINAPI Direct3DCreate8(UINT sdk) {
  d8_log("d3d8on11: Direct3DCreate8(%u)", sdk);
  return new D8();
}
