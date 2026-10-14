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
// PROVISIONAL. The game reads caps to choose its paths (Gfx_ProbeDeviceCaps and others), so ours should answer as the
// device the game runs on today does: dgVoodoo's on the owner's machine. Those values are not known yet (probe8's
// caps.raw, next run); until then these are what this device can do, chosen here, and every field is listed.
void d8_caps(D3DCAPS8 *c) {
  memset(c, 0, sizeof *c);
  c->DeviceType = D3DDEVTYPE_HAL; c->AdapterOrdinal = 0;
  c->Caps2 = D3DCAPS2_FULLSCREENGAMMA | D3DCAPS2_CANRENDERWINDOWED | D3DCAPS2_DYNAMICTEXTURES;
  c->Caps3 = D3DCAPS3_ALPHA_FULLSCREEN_FLIP_OR_DISCARD;
  c->PresentationIntervals = D3DPRESENT_INTERVAL_IMMEDIATE | D3DPRESENT_INTERVAL_ONE;
  c->CursorCaps = D3DCURSORCAPS_COLOR;
  c->DevCaps = D3DDEVCAPS_EXECUTESYSTEMMEMORY | D3DDEVCAPS_TLVERTEXSYSTEMMEMORY | D3DDEVCAPS_TLVERTEXVIDEOMEMORY |
               D3DDEVCAPS_TEXTUREVIDEOMEMORY | D3DDEVCAPS_DRAWPRIMTLVERTEX | D3DDEVCAPS_CANRENDERAFTERFLIP |
               D3DDEVCAPS_DRAWPRIMITIVES2 | D3DDEVCAPS_DRAWPRIMITIVES2EX | D3DDEVCAPS_HWTRANSFORMANDLIGHT |
               D3DDEVCAPS_HWRASTERIZATION | D3DDEVCAPS_PUREDEVICE;
  c->PrimitiveMiscCaps = D3DPMISCCAPS_MASKZ | D3DPMISCCAPS_CULLNONE | D3DPMISCCAPS_CULLCW | D3DPMISCCAPS_CULLCCW |
                         D3DPMISCCAPS_COLORWRITEENABLE | D3DPMISCCAPS_BLENDOP | D3DPMISCCAPS_TSSARGTEMP;
  c->RasterCaps = D3DPRASTERCAPS_DITHER | D3DPRASTERCAPS_ZTEST | D3DPRASTERCAPS_FOGVERTEX | D3DPRASTERCAPS_FOGTABLE |
                  D3DPRASTERCAPS_MIPMAPLODBIAS | D3DPRASTERCAPS_ZBIAS | D3DPRASTERCAPS_FOGRANGE |
                  D3DPRASTERCAPS_ANISOTROPY | D3DPRASTERCAPS_WFOG | D3DPRASTERCAPS_ZFOG;
  c->ZCmpCaps = c->AlphaCmpCaps = 0xFF;                       // every D3DPCMPCAPS_*
  c->SrcBlendCaps = c->DestBlendCaps = 0x1FFF;                // every D3DPBLENDCAPS_*
  c->ShadeCaps = D3DPSHADECAPS_COLORGOURAUDRGB | D3DPSHADECAPS_SPECULARGOURAUDRGB | D3DPSHADECAPS_ALPHAGOURAUDBLEND |
                 D3DPSHADECAPS_FOGGOURAUD;
  c->TextureCaps = D3DPTEXTURECAPS_PERSPECTIVE | D3DPTEXTURECAPS_ALPHA | D3DPTEXTURECAPS_CUBEMAP |
                   D3DPTEXTURECAPS_MIPMAP | D3DPTEXTURECAPS_MIPCUBEMAP | D3DPTEXTURECAPS_PROJECTED;
  c->TextureFilterCaps = c->CubeTextureFilterCaps = D3DPTFILTERCAPS_MINFPOINT | D3DPTFILTERCAPS_MINFLINEAR |
      D3DPTFILTERCAPS_MINFANISOTROPIC | D3DPTFILTERCAPS_MIPFPOINT | D3DPTFILTERCAPS_MIPFLINEAR |
      D3DPTFILTERCAPS_MAGFPOINT | D3DPTFILTERCAPS_MAGFLINEAR | D3DPTFILTERCAPS_MAGFANISOTROPIC;
  c->TextureAddressCaps = D3DPTADDRESSCAPS_WRAP | D3DPTADDRESSCAPS_MIRROR | D3DPTADDRESSCAPS_CLAMP |
                          D3DPTADDRESSCAPS_BORDER | D3DPTADDRESSCAPS_INDEPENDENTUV | D3DPTADDRESSCAPS_MIRRORONCE;
  c->LineCaps = D3DLINECAPS_TEXTURE | D3DLINECAPS_ZTEST | D3DLINECAPS_BLEND | D3DLINECAPS_ALPHACMP | D3DLINECAPS_FOG;
  c->MaxTextureWidth = c->MaxTextureHeight = 8192; c->MaxTextureRepeat = 8192; c->MaxTextureAspectRatio = 8192;
  c->MaxAnisotropy = 16; c->MaxVertexW = 1e10f;
  c->GuardBandLeft = -32768; c->GuardBandTop = -32768; c->GuardBandRight = 32768; c->GuardBandBottom = 32768;
  c->StencilCaps = 0xFF;                                      // every D3DSTENCILCAPS_*
  c->FVFCaps = 8 | D3DFVFCAPS_PSIZE;                          // 8 texture coordinate sets
  c->TextureOpCaps = 0x03FFFFFF;                              // every D3DTEXOPCAPS_* up to LERP
  c->MaxTextureBlendStages = 8; c->MaxSimultaneousTextures = 8;
  c->VertexProcessingCaps = D3DVTXPCAPS_TEXGEN | D3DVTXPCAPS_MATERIALSOURCE7 | D3DVTXPCAPS_DIRECTIONALLIGHTS |
                            D3DVTXPCAPS_POSITIONALLIGHTS | D3DVTXPCAPS_LOCALVIEWER;
  c->MaxActiveLights = 8; c->MaxUserClipPlanes = 6; c->MaxVertexBlendMatrices = 4; c->MaxVertexBlendMatrixIndex = 0;
  c->MaxPointSize = 256; c->MaxPrimitiveCount = 0xFFFFF; c->MaxVertexIndex = 0xFFFFFF; c->MaxStreams = 16;
  c->MaxStreamStride = 255; c->VertexShaderVersion = D3DVS_VERSION(1, 1); c->MaxVertexShaderConst = 256;
  c->PixelShaderVersion = D3DPS_VERSION(1, 4); c->MaxPixelShaderValue = 8.0f;
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
