// mip_test.cpp -- d8_mip.cpp on its own (native build, run by test_d3d8on11.py): solid colours stay solid; A8R8G8B8
// levels are the 2x2 box average exactly; the block formats' levels stay near the A8R8G8B8 path's levels made from the
// same decoded level 0; a cutout keeps its coverage; DXT1's transparent texels stay transparent
#include "../d8_mip.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
static int bad = 0;
#define CHECK(c, ...) do { bool ok_ = (c); printf("%s: ", ok_ ? "ok" : "FAIL"); printf(__VA_ARGS__); printf("\n"); bad += !ok_; } while (0)
static const char *fname(D3DFORMAT f) { return f == D3DFMT_DXT1 ? "DXT1" : f == D3DFMT_DXT3 ? "DXT3" : f == D3DFMT_DXT5 ? "DXT5" : "A8R8G8B8"; }
static double cover(const std::vector<uint8_t> &p) { size_t n = 0, c = 0; for (size_t i = 3; i < p.size(); i += 4) { n++; c += p[i] >= 128; } return (double)c / n; }
int main() {
  std::vector<std::vector<uint8_t>> out, rg; std::vector<uint8_t> dec, l0, d0;
  const D3DFORMAT bfs[3] = {D3DFMT_DXT1, D3DFMT_DXT3, D3DFMT_DXT5};
  // 1 solid colour, each format: every level decodes to level 0's colour
  for (D3DFORMAT f : bfs) {
    std::vector<uint8_t> rgba(64 * 64 * 4); for (size_t i = 0; i < rgba.size(); i += 4) { rgba[i] = 90; rgba[i + 1] = 160; rgba[i + 2] = 40; rgba[i + 3] = 255; }
    mip_encode(f, 64, 64, rgba, l0); mip_decode(f, 64, 64, l0.data(), d0);
    int n = mip_build(f, 64, 64, l0.data(), 7, out); bool same = n == 6;
    for (int l = 0; l < n && same; l++) { UINT s = 64 >> (l + 1); mip_decode(f, s, s, out[l].data(), dec); for (size_t i = 0; i < dec.size(); i++) same = same && dec[i] == d0[i % 4]; }
    CHECK(same, "solid %s 64x64: 6 levels, each level 0's colour", fname(f));
  }
  // 2 A8R8G8B8 random 37x20 (odd sizes): each level the box average of the one above, exactly
  { srand(7); std::vector<uint8_t> r0(37 * 20 * 4); for (auto &b : r0) b = (uint8_t)(rand() & 255);
    int n = mip_build(D3DFMT_A8R8G8B8, 37, 20, r0.data(), 6, out); std::vector<uint8_t> cur = r0; UINT cw = 37, ch = 20; bool exact = n == 5;
    for (int l = 0; l < n && exact; l++) {
      UINT nw = cw / 2 ? cw / 2 : 1, nh = ch / 2 ? ch / 2 : 1; std::vector<uint8_t> nx(nw * nh * 4);
      for (UINT y = 0; y < nh; y++) for (UINT x = 0; x < nw; x++) for (int k = 0; k < 4; k++) {
        UINT x0 = 2 * x < cw ? 2 * x : cw - 1, x1 = 2 * x + 1 < cw ? 2 * x + 1 : cw - 1, y0 = 2 * y < ch ? 2 * y : ch - 1, y1 = 2 * y + 1 < ch ? 2 * y + 1 : ch - 1;
        nx[(y * nw + x) * 4 + k] = (uint8_t)((cur[(y0 * cw + x0) * 4 + k] + cur[(y0 * cw + x1) * 4 + k] + cur[(y1 * cw + x0) * 4 + k] + cur[(y1 * cw + x1) * 4 + k] + 2) / 4); }
      exact = exact && out[l] == nx; cur = nx; cw = nw; ch = nh; }
    CHECK(exact, "A8R8G8B8 37x20, graded alpha: 5 levels, each the 2x2 box average exactly"); }
  // 3 the block formats' levels are the encoder's output for the A8R8G8B8 path's levels from the same decoded level 0,
  //   byte for byte (no coverage scaling: opaque, or graded alpha); and on a field varying along one direction per
  //   block (what a BC block can hold) the decoded levels are near the A8R8G8B8 levels
  for (D3DFORMAT f : bfs) {
    std::vector<uint8_t> rgba(128 * 128 * 4);
    for (int y = 0; y < 128; y++) for (int x = 0; x < 128; x++) { uint8_t *p = &rgba[(y * 128 + x) * 4]; p[0] = (uint8_t)(x * 2); p[1] = (uint8_t)(255 - x); p[2] = (uint8_t)(40 + x); p[3] = f == D3DFMT_DXT1 ? 255 : (uint8_t)(255 - x); }
    mip_encode(f, 128, 128, rgba, l0); mip_decode(f, 128, 128, l0.data(), d0);
    int n = mip_build(f, 128, 128, l0.data(), 6, out); mip_build(D3DFMT_A8R8G8B8, 128, 128, d0.data(), 6, rg);
    bool same = n == 5; double worst = 0; int maxd = 0;
    for (int l = 0; l < n; l++) { UINT s = 128 >> (l + 1); std::vector<uint8_t> e; mip_encode(f, s, s, rg[l], e); same = same && e == out[l];
      mip_decode(f, s, s, out[l].data(), dec); double m = 0; for (size_t i = 0; i < dec.size(); i++) { int d = abs((int)dec[i] - rg[l][i]); m += d; if (d > maxd) maxd = d; } m /= dec.size(); if (m > worst) worst = m; }
    CHECK(same, "%s 128x128: 5 levels, each the encoder's output for the A8R8G8B8 path's level, byte for byte", fname(f));
    CHECK(worst < 3.0, "%s, a field varying along x: mean channel difference from the A8R8G8B8 levels at most %.2f (bound 3), largest %d", fname(f), worst, maxd);
  }
  // 4 cutouts (DXT1 punch-through, alpha 0 or 255): level 0's transparent texels decode transparent; for round blobs
  //   (foliage-like) each level's coverage stays near level 0's; 1-texel bars (the worst case) are printed only
  for (int kind = 0; kind < 2; kind++) {
    std::vector<uint8_t> rgba(64 * 64 * 4);
    for (int y = 0; y < 64; y++) for (int x = 0; x < 64; x++) { uint8_t *p = &rgba[(y * 64 + x) * 4];
      bool on = kind == 0 ? ((x - 16) * (x - 16) + (y - 20) * (y - 20) < 120 || (x - 44) * (x - 44) + (y - 40) * (y - 40) < 200 || (x - 20) * (x - 20) + (y - 50) * (y - 50) < 60)
                          : ((x % 4 == 0) || (y % 6 == 0));
      p[0] = 40; p[1] = 120; p[2] = 30; p[3] = on ? 255 : 0; }
    mip_encode(D3DFMT_DXT1, 64, 64, rgba, l0); mip_decode(D3DFMT_DXT1, 64, 64, l0.data(), d0);
    bool tk = true; for (size_t i = 3; i < d0.size(); i += 4) tk = tk && d0[i] == rgba[i];
    double c0 = cover(d0), worst = 0, plainw = 0; int n = mip_build(D3DFMT_DXT1, 64, 64, l0.data(), 4, out);
    std::vector<std::vector<uint8_t>> plain; mip_build(D3DFMT_X8R8G8B8, 64, 64, d0.data(), 4, plain);   // (X8: no scaling; alpha replaced: not used)
    for (int l = 0; l < n; l++) { UINT s = 64 >> (l + 1); mip_decode(D3DFMT_DXT1, s, s, out[l].data(), dec); double c = cover(dec); if (fabs(c - c0) > worst) worst = fabs(c - c0); }
    (void)plainw;
    if (kind == 0) {
      CHECK(tk, "DXT1 cutout: level 0's transparent texels decode transparent");
      CHECK(n == 3 && worst < 0.05, "DXT1 cutout, round blobs (coverage %.3f): levels 1-3 within %.3f of it (bound 0.05)", c0, worst);
    } else printf("note: DXT1 cutout, 1-texel bars (coverage %.3f): levels 1-3 within %.3f of it (the worst case: half-size steps)\n", c0, worst);
  }
  // 5 a write to part of level 0 (mip_update): after random boxes are rewritten, the levels made again in part equal
  //   a whole build of the new level 0, byte for byte; odd and even sizes, boxes at the edges, A8R8G8B8 and X8R8G8B8
  //   (whose X byte is random here: the levels' alpha must still be 255)
  { srand(11); bool same = true; int runs = 0;
    const UINT sz[4][2] = {{37, 20}, {64, 64}, {256, 32}, {5, 3}};
    for (D3DFORMAT f : {D3DFMT_A8R8G8B8, D3DFMT_X8R8G8B8}) for (auto &d : sz) {
      UINT w = d[0], h = d[1], full = 1; for (UINT m = w > h ? w : h; m > 1; m >>= 1) full++;
      std::vector<uint8_t> t(w * h * 4); for (auto &b : t) b = (uint8_t)(rand() & 255);
      std::vector<std::vector<uint8_t>> lv, whole; std::vector<MipBox> ch; bool cut = true;
      mip_build(f, w, h, t.data(), full, lv, &cut); same = same && !cut;
      for (int k = 0; k < 40; k++) {
        UINT x0 = rand() % w, y0 = rand() % h, x1 = x0 + 1 + rand() % (w - x0), y1 = y0 + 1 + rand() % (h - y0);
        if (k % 10 == 0) { x0 = w - 1; x1 = w; } if (k % 10 == 1) { y0 = h - 1; y1 = h; }
        for (UINT y = y0; y < y1; y++) for (UINT x = x0; x < x1; x++) for (int c = 0; c < 4; c++) t[(y * w + x) * 4 + c] = (uint8_t)(rand() & 255);
        bool ok = mip_update(f, w, h, t.data(), full, MipBox{x0, y0, x1, y1}, lv, ch);
        mip_build(f, w, h, t.data(), full, whole); same = same && ok && lv == whole; runs++;
      }
    }
    CHECK(same, "mip_update: %d writes to part of level 0, the levels equal a whole build's byte for byte", runs);
    std::vector<std::vector<uint8_t>> lv; std::vector<MipBox> ch; std::vector<uint8_t> t(64 * 64 / 2);
    mip_build(D3DFMT_DXT1, 64, 64, t.data(), 7, lv);
    CHECK(!mip_update(D3DFMT_DXT1, 64, 64, t.data(), 7, MipBox{0, 0, 4, 4}, lv, ch), "mip_update refuses a block format (the caller builds whole)"); }
  printf("%s\n", bad ? "FAILED" : "ALL OK");
  return bad ? 1 : 0;
}
