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
void d8_notimpl(const char *what);                  // once per name
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
