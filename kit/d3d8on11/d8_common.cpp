// d8_common.cpp -- d3d8on11: the log and the formats, shared by the D3D8 side (d3d8.dll) and the executor (in it, or in
// d8x.exe for the executor process).
#include "d8_common.h"
#include <stdio.h>
#include <stdarg.h>
#include <set>
#include <string>

const char *d8_log_name = "d3d8on11.log";
// ---- log --------------------------------------------------------------------------------------------------------------
static FILE *g_log; static CRITICAL_SECTION g_logLock; static bool g_logInit;
static void log_open() {
  if (g_logInit) return;
  g_logInit = true; InitializeCriticalSection(&g_logLock);
  char p[MAX_PATH]; HMODULE m = NULL;
  GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&log_open, &m);
  GetModuleFileNameA(m, p, sizeof p);
  char *s = strrchr(p, '\\'); strcpy(s ? s + 1 : p, d8_log_name);
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
const char *d8_caller(void *ra, char *buf, size_t n, char *mod, size_t nmod) {
  HMODULE m = NULL; char path[MAX_PATH] = "?";
  if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)ra, &m) && m)
    GetModuleFileNameA(m, path, sizeof path);
  const char *base = strrchr(path, '\\'); base = base ? base + 1 : path;
  snprintf(buf, n, "%s+0x%lX", base, (unsigned long)((uintptr_t)ra - (uintptr_t)m));
  if (mod && nmod) snprintf(mod, nmod, "%s", base);
  return buf;
}
void d8_notimpl_at(const char *what, void *ra) {
  char at[MAX_PATH + 16], mod[MAX_PATH]; d8_caller(ra, at, sizeof at, mod, sizeof mod);
  d8_once((std::string(what) + "@" + mod).c_str(), "not implemented: %s (failed), called from %s", what, at);
}

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

