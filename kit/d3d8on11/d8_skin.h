// SPDX-License-Identifier: GPL-3.0-only
//
// d8_skin.h -- GPU skinning: what a mod hands d3d8on11 instead of locking two vertex buffers and writing skinned
// positions and normals into them (kit/speedfix: its fork of the game's SkinAndDrawV2). The layout is the game's own
// (docs/FORMATS.md 2.4, read from Model_SkinVerticesCpu 0x1002F1D0):
//   * `src`: count1 one-bone vertices of 6 floats (position, normal), then count2 two-bone vertices of 14 floats
//     (xa xb ya yb za zb wa wb nxa nxb nya nyb nza nzb);
//   * `words`: 4 bytes a vertex, two u16; a one-bone vertex uses the first, a two-bone vertex both. The bone number
//     is (word >> shift) & 0x7F (shift 0, or 7 for the mirrored copy), then bonelist[number] when bonelist is given;
//   * `dups`: output vertex count1 + count2 + k is source vertex dups[k] again;
//   * `matrices`: 64 bytes a bone; position = xyz1 . M (rows 0..2 by x, y, z, row 3 once), normal = xyz . M's rows 0..2;
//     a two-bone vertex adds both bones' terms, its w components multiplying each row 3.
// The answer 1 means the two buffers now hold that skinned mesh for every draw that reads them as the game's
// character declaration does (stream 0 positions, stream 1 normals), until either is locked and written again.
// 0 means nothing was changed: the caller skins as before.
#ifndef D8_SKIN_H
#define D8_SKIN_H
#include <stdint.h>
struct D8Skin {
  uint32_t size;                      // sizeof(D8Skin)
  void *pos, *nrm;                    // IDirect3DVertexBuffer8: the buffers the skinned positions and normals would go to
  void *key;                          // IDirect3DIndexBuffer8 of the same mesh: what the static source is kept with
  uint32_t variant;                   // 0, or 1 for the mirrored copy (its own source and bone numbers)
  const float *src; uint32_t count1, count2;
  const uint16_t *words; uint32_t shift;
  const uint16_t *bonelist;           // or NULL
  const uint16_t *dups; uint32_t ndups;
  const float *matrices;
};
// exported by d3d8.dll (d3d8on11): int __stdcall D8ON11_Skin(const D8Skin *)
#endif
