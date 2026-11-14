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
// (14 Nov) the periodic statistics lines (every 600 frames, and the like) are written only with log_stats=1 in
// d3d8on11.ini (default 1; the installer sets 0, its debug mode 1). Each such block holds a D8StatsScope: d8_log drops
// what that thread writes inside one while log_stats is 0. Read by each process when it opens its log.
extern thread_local int d8_stats_depth;
struct D8StatsScope { D8StatsScope() { d8_stats_depth++; } ~D8StatsScope() { d8_stats_depth--; } };
bool d8_log_stats_on();
void d8_crash_install();
// the records already written after the one the executor runs now (ring modes; none in direct mode): f is called for
// each in order until it returns nonzero; returns whether it did (d8_link.cpp, 22 Nov: discard_hints)
typedef int (*D8PeekFn)(uint16_t op, const uint8_t *p, uint32_t n, void *ctx);
bool d8_peek(D8PeekFn f, void *ctx);                         // exceptions of the fatal kinds to the log (d8_common.cpp)
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
