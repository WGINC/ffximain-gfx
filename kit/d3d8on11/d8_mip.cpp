// SPDX-License-Identifier: GPL-3.0-only
//
// d8_mip.cpp -- mip levels for the game's textures that have none (mipmaps=1; 19 Nov). The owner's frame captures:
// every texture the game draws with has one level (148 of 148: DXT3, DXT1, A8R8G8B8), no draw uses a mip filter, and
// the names come from a 2048x4096 A8R8G8B8 font atlas drawn as 3D quads; minified with no levels, what is drawn
// skips texels: distant text breaks up and distant textures shimmer (docs/HISTORY.md, 13 Nov, 14th entry).
//
// What is made, from level 0 as the game wrote it:
//   * each level is the 2x2 box average of the one above (odd sizes: the last row or column counted twice), in the
//     texture's own values (no gamma conversion: what D3DX's box filter of the period does);
//   * cutout textures (every alpha 0 or full) keep their coverage: each level's alpha is scaled so that the share of
//     texels at or above 128 is level 0's (a box average makes cutouts thin out with distance). 128 is a choice: the
//     game's alpha reference per draw is not known when the levels are made. Textures with graded alpha are averaged;
//   * block formats are decoded, averaged and encoded again: DXT1 (BC1, its punch-through alpha kept), DXT3 (BC2,
//     4-bit alpha), DXT5 (BC3). The encoder fits each block's colours along their principal axis (as common BC1
//     encoders do); it is not an optimal encoder.
#include "d8_mip.h"
#include <string.h>
#include <math.h>
#include <algorithm>

bool mip_format_ok(D3DFORMAT f) {
  return f == D3DFMT_A8R8G8B8 || f == D3DFMT_X8R8G8B8 || f == D3DFMT_DXT1 || f == D3DFMT_DXT3 || f == D3DFMT_DXT5;
}
static bool is_block(D3DFORMAT f) { return f == D3DFMT_DXT1 || f == D3DFMT_DXT3 || f == D3DFMT_DXT5; }
static uint32_t bsize(D3DFORMAT f) { return f == D3DFMT_DXT1 ? 8 : 16; }

// ---- BC decoding (to B, G, R, A bytes, as A8R8G8B8 lies in memory) --------------------------------------------------------
static void c565(uint16_t c, uint8_t *o) { int r = c >> 11, g = (c >> 5) & 63, b = c & 31; o[0] = (uint8_t)(b << 3 | b >> 2); o[1] = (uint8_t)(g << 2 | g >> 4); o[2] = (uint8_t)(r << 3 | r >> 2); }
static void dec_color(const uint8_t *blk, bool bc1, uint8_t out[16][4]) {
  uint16_t c0 = (uint16_t)(blk[0] | blk[1] << 8), c1 = (uint16_t)(blk[2] | blk[3] << 8); uint8_t p[4][4];
  c565(c0, p[0]); c565(c1, p[1]); p[0][3] = p[1][3] = 255;
  if (!bc1 || c0 > c1) {
    for (int k = 0; k < 3; k++) { p[2][k] = (uint8_t)((2 * p[0][k] + p[1][k]) / 3); p[3][k] = (uint8_t)((p[0][k] + 2 * p[1][k]) / 3); }
    p[2][3] = p[3][3] = 255;
  } else {
    for (int k = 0; k < 3; k++) { p[2][k] = (uint8_t)((p[0][k] + p[1][k]) / 2); p[3][k] = 0; }
    p[2][3] = 255; p[3][3] = 0;
  }
  uint32_t idx = (uint32_t)(blk[4] | blk[5] << 8 | blk[6] << 16 | (uint32_t)blk[7] << 24);
  for (int i = 0; i < 16; i++) memcpy(out[i], p[(idx >> (2 * i)) & 3], 4);
}
static void dec_block(D3DFORMAT f, const uint8_t *b, uint8_t out[16][4]) {
  if (f == D3DFMT_DXT1) { dec_color(b, true, out); return; }
  dec_color(b + 8, false, out);
  if (f == D3DFMT_DXT3) { for (int i = 0; i < 16; i++) { int a = (b[i / 2] >> (4 * (i & 1))) & 15; out[i][3] = (uint8_t)(a * 17); } return; }
  int a0 = b[0], a1 = b[1], av[8] = {a0, a1};
  if (a0 > a1) for (int k = 1; k < 7; k++) av[k + 1] = ((7 - k) * a0 + k * a1) / 7;
  else { for (int k = 1; k < 5; k++) av[k + 1] = ((5 - k) * a0 + k * a1) / 5; av[6] = 0; av[7] = 255; }
  uint64_t bits = 0; for (int k = 0; k < 6; k++) bits |= (uint64_t)b[2 + k] << (8 * k);
  for (int i = 0; i < 16; i++) out[i][3] = (uint8_t)av[(bits >> (3 * i)) & 7];
}
// ---- BC encoding --------------------------------------------------------------------------------------------------------------
static uint16_t to565(const float *c) {   // c: B, G, R in 0..255
  int r = (int)lrintf(std::min(255.f, std::max(0.f, c[2])) * 31 / 255), g = (int)lrintf(std::min(255.f, std::max(0.f, c[1])) * 63 / 255),
      b = (int)lrintf(std::min(255.f, std::max(0.f, c[0])) * 31 / 255);
  return (uint16_t)(r << 11 | g << 5 | b);
}
static void enc_color(const uint8_t px[16][4], bool bc1, uint8_t *blk) {
  bool transp[16]; int n = 0; float mean[3] = {0, 0, 0};
  for (int i = 0; i < 16; i++) { transp[i] = bc1 && px[i][3] < 128; if (!transp[i]) { for (int k = 0; k < 3; k++) mean[k] += px[i][k]; n++; } }
  bool anyt = n < 16;
  if (!n) { memset(blk, 0, 8); blk[2] = 0; blk[0] = 0; for (int i = 4; i < 8; i++) blk[i] = 0xFF; return; }   // all transparent: c0 = c1 = 0 (3-colour mode), index 3
  for (int k = 0; k < 3; k++) mean[k] /= n;
  float cov[6] = {0, 0, 0, 0, 0, 0};
  for (int i = 0; i < 16; i++) if (!transp[i]) {
    float d[3] = {px[i][0] - mean[0], px[i][1] - mean[1], px[i][2] - mean[2]};
    cov[0] += d[0] * d[0]; cov[1] += d[0] * d[1]; cov[2] += d[0] * d[2]; cov[3] += d[1] * d[1]; cov[4] += d[1] * d[2]; cov[5] += d[2] * d[2];
  }
  float ax[3] = {1, 1, 1};
  for (int it = 0; it < 8; it++) {
    float v[3] = {cov[0] * ax[0] + cov[1] * ax[1] + cov[2] * ax[2], cov[1] * ax[0] + cov[3] * ax[1] + cov[4] * ax[2], cov[2] * ax[0] + cov[4] * ax[1] + cov[5] * ax[2]};
    float l = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]); if (l < 1e-6f) break; for (int k = 0; k < 3; k++) ax[k] = v[k] / l;
  }
  float tmin = 1e9f, tmax = -1e9f;
  for (int i = 0; i < 16; i++) if (!transp[i]) { float t = (px[i][0] - mean[0]) * ax[0] + (px[i][1] - mean[1]) * ax[1] + (px[i][2] - mean[2]) * ax[2]; tmin = std::min(tmin, t); tmax = std::max(tmax, t); }
  float e0[3], e1[3]; for (int k = 0; k < 3; k++) { e0[k] = mean[k] + ax[k] * tmax; e1[k] = mean[k] + ax[k] * tmin; }
  uint16_t c0 = to565(e0), c1 = to565(e1);
  bool four = !anyt;
  if (four) { if (c0 < c1) std::swap(c0, c1); }
  else if (c0 > c1) std::swap(c0, c1);                                        // 3-colour mode: c0 <= c1
  uint8_t pal[4][4]; c565(c0, pal[0]); c565(c1, pal[1]);
  int np;
  if (four && c0 != c1) { for (int k = 0; k < 3; k++) { pal[2][k] = (uint8_t)((2 * pal[0][k] + pal[1][k]) / 3); pal[3][k] = (uint8_t)((pal[0][k] + 2 * pal[1][k]) / 3); } np = 4; }
  else if (four) np = 1;
  else { for (int k = 0; k < 3; k++) pal[2][k] = (uint8_t)((pal[0][k] + pal[1][k]) / 2); np = 3; }
  uint32_t idx = 0;
  for (int i = 0; i < 16; i++) {
    int best = 0;
    if (transp[i]) best = 3;
    else { int bd = 1 << 30; for (int p = 0; p < np; p++) { int d = 0; for (int k = 0; k < 3; k++) { int e = px[i][k] - pal[p][k]; d += e * e; } if (d < bd) { bd = d; best = p; } } }
    idx |= (uint32_t)best << (2 * i);
  }
  blk[0] = (uint8_t)c0; blk[1] = (uint8_t)(c0 >> 8); blk[2] = (uint8_t)c1; blk[3] = (uint8_t)(c1 >> 8);
  blk[4] = (uint8_t)idx; blk[5] = (uint8_t)(idx >> 8); blk[6] = (uint8_t)(idx >> 16); blk[7] = (uint8_t)(idx >> 24);
}
static void enc_block(D3DFORMAT f, const uint8_t px[16][4], uint8_t *b) {
  if (f == D3DFMT_DXT1) { enc_color(px, true, b); return; }
  enc_color(px, false, b + 8);
  if (f == D3DFMT_DXT3) { memset(b, 0, 8); for (int i = 0; i < 16; i++) b[i / 2] |= (uint8_t)(((px[i][3] * 15 + 127) / 255) << (4 * (i & 1))); return; }
  int amin = 255, amax = 0; for (int i = 0; i < 16; i++) { amin = std::min(amin, (int)px[i][3]); amax = std::max(amax, (int)px[i][3]); }
  b[0] = (uint8_t)amax; b[1] = (uint8_t)amin; int av[8] = {amax, amin};
  for (int k = 1; k < 7; k++) av[k + 1] = ((7 - k) * amax + k * amin) / 7;
  uint64_t bits = 0;
  for (int i = 0; i < 16; i++) {
    int best = 0, bd = 1 << 30;
    if (amax != amin) for (int p = 0; p < 8; p++) { int d = abs(px[i][3] - av[p]); if (d < bd) { bd = d; best = p; } }
    bits |= (uint64_t)best << (3 * i);
  }
  for (int k = 0; k < 6; k++) b[2 + k] = (uint8_t)(bits >> (8 * k));
}
// ---- levels ------------------------------------------------------------------------------------------------------------------
static void to_rgba(D3DFORMAT f, UINT w, UINT h, const uint8_t *src, std::vector<uint8_t> &o) {
  o.assign((size_t)w * h * 4, 0);
  if (!is_block(f)) {
    memcpy(o.data(), src, o.size());
    if (f == D3DFMT_X8R8G8B8) for (size_t i = 3; i < o.size(); i += 4) o[i] = 255;
    return;
  }
  UINT bw = (w + 3) / 4, bh = (h + 3) / 4; uint8_t px[16][4];
  for (UINT by = 0; by < bh; by++) for (UINT bx = 0; bx < bw; bx++) {
    dec_block(f, src + ((size_t)by * bw + bx) * bsize(f), px);
    for (int i = 0; i < 16; i++) { UINT x = bx * 4 + (i & 3), y = by * 4 + (i >> 2); if (x < w && y < h) memcpy(&o[((size_t)y * w + x) * 4], px[i], 4); }
  }
}
static void from_rgba(D3DFORMAT f, UINT w, UINT h, const std::vector<uint8_t> &in, std::vector<uint8_t> &o) {
  if (!is_block(f)) { o = in; return; }
  UINT bw = (w + 3) / 4, bh = (h + 3) / 4; o.assign((size_t)bw * bh * bsize(f), 0); uint8_t px[16][4];
  for (UINT by = 0; by < bh; by++) for (UINT bx = 0; bx < bw; bx++) {
    for (int i = 0; i < 16; i++) {                       // texels past the edge repeat the last one (they are not sampled)
      UINT x = std::min(bx * 4 + (i & 3), w - 1), y = std::min(by * 4 + (i >> 2), h - 1); memcpy(px[i], &in[((size_t)y * w + x) * 4], 4);
    }
    enc_block(f, px, &o[((size_t)by * bw + bx) * bsize(f)]);
  }
}
static double coverage(const std::vector<uint8_t> &p, float scale) {
  size_t n = 0, c = 0; for (size_t i = 3; i < p.size(); i += 4) { n++; c += p[i] * scale >= 127.5f; } return n ? (double)c / n : 0;
}
// ---- the 2x2 box average of a level, rows of 4-byte texels (22 Nov: SSE2 on x86 when the CPU has it) ----------------
// mip_simd: -1 (the default) the CPU decides, 0 the plain loop, 1 SSE2 if the CPU has it. Both give the same bytes:
// (a + b + c + d + 2) / 4 per channel, in 16-bit lanes that cannot overflow (4 x 255 + 2) (mip_test checks them equal)
int mip_simd = -1;
static void box_plain(const uint8_t *cur, UINT cw, UINT ch, uint8_t *nxt, UINT nw, UINT nh) {
  for (UINT y = 0; y < nh; y++) for (UINT x = 0; x < nw; x++) {
    UINT x0 = std::min(2 * x, cw - 1), x1 = std::min(2 * x + 1, cw - 1), y0 = std::min(2 * y, ch - 1), y1 = std::min(2 * y + 1, ch - 1);
    for (int k = 0; k < 4; k++) {
      unsigned s = cur[((size_t)y0 * cw + x0) * 4 + k] + cur[((size_t)y0 * cw + x1) * 4 + k] + cur[((size_t)y1 * cw + x0) * 4 + k] + cur[((size_t)y1 * cw + x1) * 4 + k];
      nxt[((size_t)y * nw + x) * 4 + k] = (uint8_t)((s + 2) / 4);
    }
  }
}
#if defined(__i386__) || defined(__x86_64__)
#include <emmintrin.h>
__attribute__((target("sse2"))) static void box_sse2(const uint8_t *cur, UINT cw, UINT ch, uint8_t *nxt, UINT nw, UINT nh) {
  // with cw, ch >= 2 no texel is read twice (an odd last row or column is never reached: nw = cw / 2): two output
  // texels at a time from four input columns while 2x + 3 <= cw - 1, the rest by the plain rule
  const __m128i z = _mm_setzero_si128(), two = _mm_set1_epi16(2);
  for (UINT y = 0; y < nh; y++) {
    const uint8_t *r0 = cur + (size_t)(2 * y) * cw * 4, *r1 = cur + (size_t)(2 * y + 1) * cw * 4; uint8_t *o = nxt + (size_t)y * nw * 4;
    UINT x = 0;
    for (; x + 1 < nw; x += 2) {
      __m128i a = _mm_loadu_si128((const __m128i *)(r0 + 8 * x)), b = _mm_loadu_si128((const __m128i *)(r1 + 8 * x));
      __m128i lo = _mm_add_epi16(_mm_unpacklo_epi8(a, z), _mm_unpacklo_epi8(b, z));   // texels 2x, 2x+1 (rows added)
      __m128i hi = _mm_add_epi16(_mm_unpackhi_epi8(a, z), _mm_unpackhi_epi8(b, z));   // texels 2x+2, 2x+3
      lo = _mm_add_epi16(lo, _mm_srli_si128(lo, 8)); hi = _mm_add_epi16(hi, _mm_srli_si128(hi, 8));
      __m128i s = _mm_srli_epi16(_mm_add_epi16(_mm_unpacklo_epi64(lo, hi), two), 2);
      _mm_storel_epi64((__m128i *)(o + 4 * x), _mm_packus_epi16(s, z));
    }
    for (; x < nw; x++) for (int k = 0; k < 4; k++)
      o[4 * x + k] = (uint8_t)((r0[8 * x + k] + r0[8 * x + 4 + k] + r1[8 * x + k] + r1[8 * x + 4 + k] + 2) / 4);
  }
}
static bool have_sse2() { static int h = -1; if (h < 0) { __builtin_cpu_init(); h = __builtin_cpu_supports("sse2") ? 1 : 0; } return h != 0; }
#endif
const char *mip_simd_name() {
#if defined(__i386__) || defined(__x86_64__)
  return mip_simd != 0 && have_sse2() ? "SSE2 (the CPU has it)" : "the plain loop";
#else
  return "the plain loop (the compiler's own vectorising on this architecture)";
#endif
}
static void box(const uint8_t *cur, UINT cw, UINT ch, uint8_t *nxt, UINT nw, UINT nh) {
#if defined(__i386__) || defined(__x86_64__)
  if (mip_simd != 0 && cw >= 2 && ch >= 2 && have_sse2()) { box_sse2(cur, cw, ch, nxt, nw, nh); return; }
#endif
  box_plain(cur, cw, ch, nxt, nw, nh);
}
int mip_build(D3DFORMAT f, UINT w, UINT h, const uint8_t *level0, UINT nlevels, std::vector<std::vector<uint8_t>> &out, bool *cutout_out) {
  out.clear(); if (cutout_out) *cutout_out = false;
  if (!mip_format_ok(f) || nlevels < 2) return 0;
  // A8R8G8B8 is read in place (its level 0 is already B, G, R, A); the others are converted first. Each level is
  // made into its own buffer once (22 Nov: the copies cost more than the averaging: 51 ms for the 2048x4096 atlas)
  std::vector<uint8_t> conv, nxt;
  const uint8_t *cur = level0; size_t cur_n = (size_t)w * h * 4;
  if (f != D3DFMT_A8R8G8B8) { to_rgba(f, w, h, level0, conv); cur = conv.data(); }
  bool cutout = f != D3DFMT_X8R8G8B8, anyt = false;
  for (size_t i = 3; i < cur_n && cutout; i += 4) { if (cur[i] != 0 && cur[i] != 255) cutout = false; if (cur[i] != 255) anyt = true; }
  if (f == D3DFMT_DXT3) {                                   // 4-bit alpha: 0 and 15 only are a cutout
    cutout = true; anyt = false; for (size_t i = 3; i < cur_n; i += 4) { if (cur[i] != 0 && cur[i] != 255) { cutout = false; break; } if (cur[i] != 255) anyt = true; }
  }
  cutout = cutout && anyt; if (cutout_out) *cutout_out = cutout;
  double cov0 = 0;
  if (cutout) { std::vector<uint8_t> v(cur, cur + cur_n); cov0 = coverage(v, 1.0f); }
  bool plain = !is_block(f) && !cutout;                     // the averaged level is the level as stored
  std::vector<uint8_t> prev;                                // the averaged level before scaling or encoding
  UINT cw = w, ch = h;
  for (UINT l = 1; l < nlevels; l++) {
    UINT nw = std::max(1u, cw / 2), nh = std::max(1u, ch / 2);
    nxt.resize((size_t)nw * nh * 4);
    box(cur, cw, ch, nxt.data(), nw, nh);
    if (plain) { out.push_back(std::move(nxt)); nxt = std::vector<uint8_t>(); cur = out.back().data(); }
    else {
      std::vector<uint8_t> lvl = nxt;
      if (cutout) {                                         // the scale that gives level 0's share of alpha >= 128
        float lo = 0.0f, hi = 8.0f;
        for (int it = 0; it < 20; it++) { float m = (lo + hi) / 2; if (coverage(lvl, m) < cov0) lo = m; else hi = m; }
        // coverage is a step function of the scale: the step nearer level 0's share is taken (either side of it)
        float sc = fabs(coverage(lvl, lo) - cov0) < fabs(coverage(lvl, hi) - cov0) ? lo : hi;
        for (size_t i = 3; i < lvl.size(); i += 4) lvl[i] = (uint8_t)std::min(255.0f, lvl[i] * sc + 0.5f);
      }
      out.emplace_back(); from_rgba(f, nw, nh, lvl, out.back());
      prev.swap(nxt); cur = prev.data();                      // the next level averages the unscaled one
    }
    cw = nw; ch = nh;
  }
  return (int)out.size();
}
// a write to part of level 0 (19 Nov): the 2x2 box average is local, so for a texture that is not a cutout (whose
// levels are scaled by a share counted over the whole level) the texels a box reaches can be made alone. A texel x of
// level l reads texels min(2x, w-1) and min(2x+1, w-1) of level l-1, so the box at level l is [x0/2, (x1-1)/2]
bool mip_update(D3DFORMAT f, UINT w, UINT h, const uint8_t *level0, UINT nlevels, MipBox b, std::vector<std::vector<uint8_t>> &levels, std::vector<MipBox> &changed) {
  changed.clear();
  if ((f != D3DFMT_A8R8G8B8 && f != D3DFMT_X8R8G8B8) || nlevels < 2 || levels.size() != nlevels - 1) return false;
  b.x1 = std::min(b.x1, w); b.y1 = std::min(b.y1, h);
  const uint8_t *cur = level0; UINT cw = w, ch = h; bool x8 = f == D3DFMT_X8R8G8B8;
  for (UINT l = 1; l < nlevels; l++) {
    UINT nw = std::max(1u, cw / 2), nh = std::max(1u, ch / 2);
    if (levels[l - 1].size() != (size_t)nw * nh * 4) { changed.clear(); return false; }
    if (b.x0 >= b.x1 || b.y0 >= b.y1) break;
    MipBox n = {b.x0 / 2, b.y0 / 2, std::min(nw, (b.x1 - 1) / 2 + 1), std::min(nh, (b.y1 - 1) / 2 + 1)};
    if (n.x0 >= n.x1 || n.y0 >= n.y1) break;
    uint8_t *o = levels[l - 1].data();
    for (UINT y = n.y0; y < n.y1; y++) for (UINT x = n.x0; x < n.x1; x++) {
      UINT x0 = std::min(2 * x, cw - 1), x1 = std::min(2 * x + 1, cw - 1), y0 = std::min(2 * y, ch - 1), y1 = std::min(2 * y + 1, ch - 1);
      for (int k = 0; k < 4; k++) {
        if (k == 3 && x8 && l == 1) { o[((size_t)y * nw + x) * 4 + 3] = 255; continue; }   // level 0's X byte is not alpha
        unsigned s = cur[((size_t)y0 * cw + x0) * 4 + k] + cur[((size_t)y0 * cw + x1) * 4 + k] + cur[((size_t)y1 * cw + x0) * 4 + k] + cur[((size_t)y1 * cw + x1) * 4 + k];
        o[((size_t)y * nw + x) * 4 + k] = (uint8_t)((s + 2) / 4);
      }
    }
    changed.push_back(n); cur = o; cw = nw; ch = nh; b = n;
  }
  return true;
}
// a decode for the tests (d8_mip_test): level data back to B, G, R, A
void mip_decode(D3DFORMAT f, UINT w, UINT h, const uint8_t *src, std::vector<uint8_t> &o) { to_rgba(f, w, h, src, o); }
// an encode for the tests: B, G, R, A texels to the format (packed)
void mip_encode(D3DFORMAT f, UINT w, UINT h, const std::vector<uint8_t> &rgba, std::vector<uint8_t> &o) { from_rgba(f, w, h, rgba, o); }
