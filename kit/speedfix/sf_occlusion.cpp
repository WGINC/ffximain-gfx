// SPDX-License-Identifier: GPL-3.0-only
//
// sf_occlusion.cpp -- speedfix's occlusion_late: the occlusion readback answered from the query's previous probe, so
// the CPU need not wait for the GPU to finish the picture it has just drawn.
//
// What the game does (disassembled; docs/HISTORY.md, 10 and 11 Oct):
//   * CMoOcclusionMng's constructor (0x1006C140) makes a 16x16 lockable render target (CreateRenderTarget, Lockable 1)
//     at [mng+0x18]; the 16x16 texture the probe boxes are drawn into is at [mng+0xC]. mng is [0x1047C004].
//   * The query objects are per owner (an actor's at +0x8F0).
//   * The readback 0x1006C8B0 (__thiscall, one stack argument, ret 4; its one caller is 0x1006C4D7 with argument 0):
//     GetSurfaceLevel(0) of the box texture; CopyRects of rect (0,0,16,16) (0x1032C18C) from it into [q+4] on the
//     CMoDX device [[0x1047BFA8]+0xEBC]; Release; LockRect([q+4], rect 0x1032C17C, D3DLOCK_READONLY 0x10); the sum of
//     (dword & 0x80) over 8 dwords, 8 bytes apart, of every second row; UnlockRect; the sum >> 5 (0 to 256). A failed
//     LockRect answers 0. With argument 0 it writes nothing to the query object.
//   * Its LockRect comes right after its CopyRects, so the lock waits for the GPU to draw the boxes and copy them.
//
// What this does instead, per query object (the answer belongs to the object: one shared "previous answer" would give
// one actor another's):
//   * this probe's picture is copied (the same CopyRects) into a 16x16 lockable render target of the object's own, and
//     read at the object's next probe, a later frame;
//   * the answer returned is that previous picture's, counted exactly as the game counts;
//   * with no picture of its own yet, or one older than OL_MAX_AGE frames, the game's own readback answers (this
//     frame's picture), and this probe's picture is copied for the next.
// So the answer is one probe late: an actor that comes into view is drawn from its next probe on (behaviour changes;
// mode 2 measures how often the late answer differs from the game's).
// Whether the late lock waits depends on the device, and is measured here (the read's time, per call):
//   * d3d8on11: no wait when the copy's snapshot is there (kit/d3d8on11 README, "Readback snapshots": the executor maps
//     the copy at a Present after it without waiting). Its log says how many locks were answered so.
//   * dgVoodoo and others: not known. The owner measured their own earlier async readback slower there (HISTORY,
//     11 Oct); which surface kind it used is not recorded here.
// Modes (speedfix.ini occlusion_late): 1 the late answer; 2 measure only: the game's answer is returned, the late one
// is computed beside it and how often they differ is logged. Every surface is released before Reset.
#include <windows.h>
#include <d3d8.h>
#include <stdint.h>
#include <string.h>
#include "speedfix.h"

static const XikitHost *H;
#define XI_GAME(a) sf_game_addr((uint32_t)(a))

enum { OL_SLOTS = 1024, OL_MAX_AGE = 32 };   // OL_MAX_AGE: a chosen bound, not a fact of the game
struct OlSlot { void *q; IDirect3DSurface8 *s; int frame; bool pending; };
static OlSlot g_slot[OL_SLOTS];
static int g_used;
static volatile LONG g_nLate, g_nSync, g_nCopyFail, g_nSame, g_nDiffer, g_nVisFlip;
// timing (QueryPerformanceCounter ticks), render thread only: the game's readback, this file's copy and its read;
// frames counted apart for the lever on and off, so each figure is also given per frame of its own label
static LONGLONG g_tGame, g_tCopy, g_tRead, g_tReadMax; static LONG g_nGame, g_nCopy, g_nRead, g_framesOn, g_framesOff, g_framesSkip;
static inline LONGLONG qpc(void) { LARGE_INTEGER t; QueryPerformanceCounter(&t); return t.QuadPart; }
typedef int (__thiscall *ReadbackFn)(void *, int);
// the game's readback, timed (sf_main.cpp calls it for every readback speedfix does not answer itself)
extern "C" int sf_occl_game(void *q, int flag) {
  LONGLONG t = qpc();
  int r = ((ReadbackFn)XI_GAME(0x1006C8B0u))(q, flag);
  g_tGame += qpc() - t; g_nGame++;
  return r;
}
// a frame with skip_all_occlusion_readbacks on runs no readback, so it is in neither per-frame figure
extern "C" void sf_occl_frame(int on) { if (on < 0) g_framesSkip++; else if (on) g_framesOn++; else g_framesOff++; }
extern "C" void sf_occl_init(const XikitHost *host) { H = host; }

static void release_slot(OlSlot *e) {
  if (e->s) e->s->Release();
  e->s = NULL; e->pending = false;
}
extern "C" void sf_occl_reset(void) {         // before the device's Reset (sf_device.cpp, h_Reset): default-pool surfaces
  for (int i = 0; i < OL_SLOTS; i++) release_slot(&g_slot[i]);
}

static OlSlot *find_slot(void *q) {
  unsigned h = (unsigned)(((uintptr_t)q >> 2) * 2654435761u) % OL_SLOTS;
  for (int i = 0; i < OL_SLOTS; i++) {
    OlSlot *e = &g_slot[(h + i) % OL_SLOTS];
    if (e->q == q) return e;
    if (!e->q) { e->q = q; e->frame = sf_frame_no - OL_MAX_AGE - 1; g_used++; return e; }
  }
  // full: the slot probed longest ago is given to this object (its surface is kept: every one is alike)
  OlSlot *old = &g_slot[0];
  for (int i = 1; i < OL_SLOTS; i++) if (sf_frame_no - g_slot[i].frame > sf_frame_no - old->frame) old = &g_slot[i];
  old->q = q; old->pending = false; old->frame = sf_frame_no - OL_MAX_AGE - 1;
  return old;
}

// the game's count (0x1006C923..0x1006C971) over a locked 16x16 rect
static int count_locked(const D3DLOCKED_RECT &lr) {
  int sum = 0;
  const uint8_t *row = (const uint8_t *)lr.pBits;
  for (int r = 0; r < 8; r++, row += 2 * lr.Pitch)
    for (int x = 0; x < 0x40; x += 8) sum += *(const uint32_t *)(row + x) & 0x80;
  return sum >> 5;
}

static const RECT kRect16 = {0, 0, 16, 16};

// a surface like the game's own [mng+0x18]: a 16x16 lockable render target in its format
static IDirect3DSurface8 *make_surface(IDirect3DDevice8 *dev, IDirect3DSurface8 *game) {
  D3DSURFACE_DESC d;
  if (FAILED(game->GetDesc(&d))) return NULL;
  IDirect3DSurface8 *s = NULL;
  HRESULT hr = dev->CreateRenderTarget(16, 16, d.Format, D3DMULTISAMPLE_NONE, TRUE, &s);
  static bool logged;
  if (FAILED(hr)) {
    if (!logged) { logged = true; H->log("speedfix: occlusion_late: CreateRenderTarget(16, 16, format %u, lockable) failed: 0x%08lX", (unsigned)d.Format, (unsigned long)hr); }
    return NULL;
  }
  if (!logged) { logged = true; H->log("speedfix: occlusion_late: its surfaces: 16x16 lockable render targets, format %u (the game's own: format %u, pool %u)", (unsigned)d.Format, (unsigned)d.Format, (unsigned)d.Pool); }
  return s;
}

// copy this probe's picture into the object's own surface; false if it could not be
static bool queue_copy(IDirect3DDevice8 *dev, IDirect3DTexture8 *tex, IDirect3DSurface8 *game, OlSlot *e) {
  if (!e->s) e->s = make_surface(dev, game);
  if (!e->s) return false;
  IDirect3DSurface8 *src = NULL;
  LONGLONG t = qpc();
  if (FAILED(tex->GetSurfaceLevel(0, &src)) || !src) return false;
  HRESULT hr = dev->CopyRects(src, &kRect16, 1, e->s, NULL);
  src->Release();
  g_tCopy += qpc() - t; g_nCopy++;
  if (FAILED(hr)) {
    static bool logged;
    if (!logged) { logged = true; H->log("speedfix: occlusion_late: CopyRects into its surface failed: 0x%08lX", (unsigned long)hr); }
    release_slot(e);
    return false;
  }
  e->pending = true; e->frame = sf_frame_no;
  return true;
}

// read the object's previous picture; -1 if there is none young enough
static int read_previous(OlSlot *e) {
  if (!e->pending || !e->s) return -1;
  e->pending = false;
  if (sf_frame_no - e->frame > OL_MAX_AGE) return -1;
  D3DLOCKED_RECT lr; int n = 0;
  LONGLONG t = qpc();
  if (SUCCEEDED(e->s->LockRect(&lr, &kRect16, D3DLOCK_READONLY))) { n = count_locked(lr); e->s->UnlockRect(); }   // a failed lock: 0, as the game answers
  LONGLONG dt = qpc() - t;
  g_tRead += dt; g_nRead++; if (dt > g_tReadMax) g_tReadMax = dt;
  return n;
}

// called for argument 0 only, with occlusion_late on and the kit's hooks live (sf_main.cpp, sf_readback)
extern "C" int sf_occl_readback(void *q, int mode) {
  uintptr_t modx = *(uintptr_t *)XI_GAME(0x1047BFA8u);
  uint8_t *mng = *(uint8_t **)XI_GAME(0x1047C004u);
  IDirect3DDevice8 *dev = modx ? *(IDirect3DDevice8 **)(modx + 0xEBC) : NULL;
  IDirect3DTexture8 *tex = mng ? *(IDirect3DTexture8 **)(mng + 0xC) : NULL;
  IDirect3DSurface8 *gsurf = mng ? *(IDirect3DSurface8 **)(mng + 0x18) : NULL;
  if (!dev || !tex || !gsurf) { g_nSync++; return sf_occl_game(q, 0); }
  OlSlot *e = find_slot(q);
  int prev = read_previous(e);
  int ans;
  if (mode == 2) {                            // measure only
    ans = sf_occl_game(q, 0);
    if (prev >= 0) { if (prev == ans) g_nSame++; else g_nDiffer++; if ((prev == 0) != (ans == 0)) g_nVisFlip++; }
  } else if (prev >= 0) { ans = prev; g_nLate++; }
  else { ans = sf_occl_game(q, 0); g_nSync++; }
  if (!queue_copy(dev, tex, gsurf, e)) g_nCopyFail++;
  return ans;
}

extern "C" void sf_occl_stats(void) {
  LARGE_INTEGER qf; QueryPerformanceFrequency(&qf);
  double us = 1e6 / (double)qf.QuadPart;
  if (g_nGame + g_nCopy + g_nRead)
    H->log("speedfix: readback time: the game's readback %ld calls, mean %.1f us, %.3f ms a frame with occlusion_late off (%ld frames); "
           "occlusion_late's copy %ld, mean %.1f us; its late read %ld, mean %.1f us, longest %.1f us; the two %.3f ms a frame "
           "with it on (%ld frames); %ld frames with every readback skipped, in neither",
           g_nGame, g_nGame ? g_tGame * us / g_nGame : 0.0, g_framesOff ? g_tGame * us / 1000.0 / g_framesOff : 0.0, g_framesOff,
           g_nCopy, g_nCopy ? g_tCopy * us / g_nCopy : 0.0, g_nRead, g_nRead ? g_tRead * us / g_nRead : 0.0, g_tReadMax * us,
           g_framesOn ? (g_tCopy + g_tRead) * us / 1000.0 / g_framesOn : 0.0, g_framesOn, g_framesSkip);
  g_tGame = g_tCopy = g_tRead = g_tReadMax = 0; g_nGame = g_nCopy = g_nRead = g_framesOn = g_framesOff = g_framesSkip = 0;
  LONG a = InterlockedExchange(&g_nLate, 0), s = InterlockedExchange(&g_nSync, 0), f = InterlockedExchange(&g_nCopyFail, 0);
  LONG same = InterlockedExchange(&g_nSame, 0), dif = InterlockedExchange(&g_nDiffer, 0), flip = InterlockedExchange(&g_nVisFlip, 0);
  if (a + s + f) H->log("speedfix: occlusion_late: %ld answered from the previous probe, %ld by the game's readback (first or stale), "
                        "%ld copies failed; %d query objects seen", a, s, f, g_used);
  if (same + dif) H->log("speedfix: occlusion_late=2: of %ld probes with a previous picture, the late answer differed from the "
                         "game's in %ld (%.1f%%); visible/hidden differed in %ld (%.1f%%)", same + dif, dif,
                         100.0 * dif / (same + dif), flip, 100.0 * flip / (same + dif));
}
