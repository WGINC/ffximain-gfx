// d8_common.h -- d3d8on11: the log and the formats, shared by the D3D8 side and the executor (d8_common.cpp).
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d8.h>
#define D3DCOLORVALUE_DEFINED            // d3d8types.h has defined it; dxgitype.h would again
#include <dxgiformat.h>
#include <stdint.h>

// ---- log (d8_log_name next to the module; each message once unless counted) ------------------------------------
extern const char *d8_log_name;
void d8_log(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
// once per name and calling module, naming the caller (module+offset): in a process with other Direct3D users (Windower's
// Hook.dll draws through the same device) the log says whose call it was. ra: the return address in the method itself;
// called from a helper of ours instead, it names d3d8.dll
void d8_notimpl_at(const char *what, void *ra);
#define d8_notimpl(what) d8_notimpl_at((what), __builtin_return_address(0))
const char *d8_caller(void *ra, char *buf, size_t n, char *mod = nullptr, size_t nmod = 0);
// (18 Nov) readbacks by the game's call site: the public lock and copy methods note their return address, each
// X_READBACK is counted under it with the time it waited; the top sites logged with the frame stats (d8_device.cpp)
extern thread_local void *d8_rb_site;
void d8_rb_count(double ms);
void d8_rb_log(unsigned frames);
static inline double qpc_now_ms() { LARGE_INTEGER t, f; QueryPerformanceCounter(&t); QueryPerformanceFrequency(&f); return t.QuadPart * 1000.0 / (double)f.QuadPart; }   // "Module.dll+0x1234"
void d8_once(const char *key, const char *fmt, ...) __attribute__((format(printf, 2, 3)));

// ---- formats ------------------------------------------------------------------------------------------------------
struct FmtInfo {
  DXGI_FORMAT dxgi;   // DXGI_FORMAT_UNKNOWN: not supported here
  int bpp;            // bits per pixel (0 for block formats)
  int block;          // bytes per 4x4 block (BC formats), else 0
  bool depth;
};
FmtInfo d8_fmt(D3DFORMAT f);
UINT d8_pitch(D3DFORMAT f, UINT w);                 // bytes per row (per row of blocks for BC)
UINT d8_rows(D3DFORMAT f, UINT h);                  // rows (rows of blocks for BC)
