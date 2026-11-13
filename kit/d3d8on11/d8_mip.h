// SPDX-License-Identifier: GPL-3.0-only
// d8_mip.h -- mip levels for one-level textures (d8_mip.cpp)
#ifndef D8_MIP_H
#define D8_MIP_H
#include <stdint.h>
#include <vector>
#ifdef D8_MIP_STANDALONE
typedef unsigned UINT; typedef int D3DFORMAT;
enum { D3DFMT_A8R8G8B8 = 21, D3DFMT_X8R8G8B8 = 22, D3DFMT_DXT1 = 0x31545844, D3DFMT_DXT3 = 0x33545844, D3DFMT_DXT5 = 0x35545844 };
#else
#include <windows.h>
#include <d3d8.h>
#endif
bool mip_format_ok(D3DFORMAT f);
// levels 1 .. nlevels-1 of a texture whose level 0 is `level0` (packed rows), each in the texture's format, packed
// (block formats: whole 4x4 blocks); returns how many were made (0: the format is not handled)
int mip_build(D3DFORMAT f, UINT w, UINT h, const uint8_t *level0, UINT nlevels, std::vector<std::vector<uint8_t>> &out, bool *cutout = nullptr);
struct MipBox { UINT x0, y0, x1, y1; };   // texels [x0, x1) x [y0, y1)
// after a write to `box` of level 0: levels 1 .. (from an earlier mip_build of this texture, not a cutout) made again
// where the box reaches them, each equal to what a whole mip_build would make; changed[k]: the part of level k+1 made.
// A8R8G8B8 and X8R8G8B8 only; returns false (nothing done) otherwise
bool mip_update(D3DFORMAT f, UINT w, UINT h, const uint8_t *level0, UINT nlevels, MipBox box, std::vector<std::vector<uint8_t>> &levels, std::vector<MipBox> &changed);
void mip_decode(D3DFORMAT f, UINT w, UINT h, const uint8_t *src, std::vector<uint8_t> &o);
void mip_encode(D3DFORMAT f, UINT w, UINT h, const std::vector<uint8_t> &rgba, std::vector<uint8_t> &o);
#endif
