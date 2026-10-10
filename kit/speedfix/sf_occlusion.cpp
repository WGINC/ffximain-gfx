// SPDX-License-Identifier: GPL-3.0-only
//
// sf_occlusion.cpp -- speedfix's occlusion_async: the occlusion readback answered from the query's previous probe,
// so the CPU does not wait for the GPU to finish the picture it has just drawn.
//
// What the game does (disassembled; docs/HISTORY.md, 10 Oct):
//   * CMoOcclusionMng's constructor (0x1006C140) makes a 16x16 lockable render target (CreateRenderTarget, Lockable 1,
//     so D3DPOOL_DEFAULT) in the scene texture's format, at [mng+0x18]; OcclusionMng_CreateTarget (0x1006C220) makes the
//     16x16 texture the probe boxes are drawn into, at [mng+0xC]. mng is [0x1047C004].
//   * Every query object (an actor's at +0x8F0, and the other four callers' own) holds that surface at +4
//     (OcclusionMng_Prepare 0x1006C9B0 AddRefs it there).
//   * The readback 0x1006C8B0 (__thiscall, one stack argument, ret 4; its one caller is 0x1006C4D7 with argument 0):
//     GetSurfaceLevel(0) of the box texture; CopyRects of rect (0,0,16,16) (0x1032C18C) from it into [q+4] on the
//     CMoDX device [[0x1047BFA8]+0xEBC]; Release; LockRect([q+4], rect 0x1032C17C, D3DLOCK_READONLY 0x10); the sum of
//     (dword & 0x80) over 8 dwords, 8 bytes apart, of every second row; UnlockRect; the sum >> 5 (0 to 256). A failed
//     LockRect answers 0. With argument 0 it writes nothing to the query object.
//   * The LockRect is where the CPU waits for the GPU (19.5% of the render thread in run 6156, 22.4% in session 1).
//
// What this does instead, per query object (the answer belongs to the object; one shared "previous answer" would
// give one actor another's):
//   * the picture drawn for this probe is copied (the same CopyRects) into a surface of the object's own, and read
//     at the object's next probe, so the copy has had a probe cycle (4 frames by the game's stagger) to finish;
//   * the answer returned is that previous picture's, counted exactly as the game counts;
//   * with no picture of its own yet, or one older than OA_MAX_AGE frames, the game's own readback answers (the same
//     frame's picture), and this probe's picture is copied for the next.
// Modes (speedfix.ini occlusion_async):
//   1  the object's surface is an image surface (CreateImageSurface; its pool is logged from GetDesc);
//   2  it is a lockable render target like the game's own (the delay alone);
//   3  measure only: the game's answer is returned; the delayed answer is computed beside it and how often they
//      differ is logged (no time saved: the game's lock still waits).
// Every surface is released before Reset and made again when next needed.
#include <windows.h>
#include <d3d8.h>
#include <stdint.h>
#include <string.h>
#include "speedfix.h"

static const XikitHost *H;
#define XI_GAME(a) ((uintptr_t)(a) + (uintptr_t)H->rel)

enum { OA_SLOTS = 1024, OA_MAX_AGE = 32 };   // OA_MAX_AGE: a chosen bound, 2x the longest stagger period (16 frames)
struct OaSlot { void *q; IDirect3DSurface8 *s; int kind; int frame; bool pending; };
static OaSlot g_slot[OA_SLOTS];
static int g_used;
static bool g_imageFailed;                    // mode 1 could not make or fill an image surface: mode 2's surface instead
static volatile LONG g_nAsync, g_nSync, g_nCopyFail, g_nNoSlot, g_nSame, g_nDiffer, g_nVisFlip;
// timing (QueryPerformanceCounter ticks), render thread only: the game's readback, this file's copy and its read;
// frames counted apart for the lever on and off, so each figure is also given per frame of its own label
static LONGLONG g_tGame, g_tCopy, g_tRead; static LONG g_nGame, g_nCopy, g_nRead, g_framesOn, g_framesOff;
static inline LONGLONG qpc(void) { LARGE_INTEGER t; QueryPerformanceCounter(&t); return t.QuadPart; }
typedef int (__thiscall *ReadbackFn)(void *, int);
// the game's readback, timed (sf_main.cpp calls it for every readback speedfix does not answer itself)
extern "C" int sf_occl_game(void *q, int flag) {
  LONGLONG t = qpc();
  int r = ((ReadbackFn)XI_GAME(0x1006C8B0u))(q, flag);
  g_tGame += qpc() - t; g_nGame++;
  return r;
}
extern "C" void sf_occl_frame(int on) { if (on) g_framesOn++; else g_framesOff++; }

extern "C" void sf_occl_init(const XikitHost *host) { H = host; }

static void release_slot(OaSlot *e) {
  if (e->s) e->s->Release();
  e->s = NULL; e->pending = false; e->kind = 0;
}
extern "C" void sf_occl_reset(void) {         // before the device's Reset (sf_device.cpp, h_Reset)
  for (int i = 0; i < OA_SLOTS; i++) release_slot(&g_slot[i]);
}

static OaSlot *find_slot(void *q) {
  unsigned h = (unsigned)(((uintptr_t)q >> 2) * 2654435761u) % OA_SLOTS;
  for (int i = 0; i < OA_SLOTS; i++) {
    OaSlot *e = &g_slot[(h + i) % OA_SLOTS];
    if (e->q == q) return e;
    if (!e->q) { e->q = q; e->frame = sf_frame_no - OA_MAX_AGE - 1; g_used++; return e; }
  }
  // full: the slot probed longest ago is given to this object (its surface is kept: every one is alike)
  OaSlot *old = &g_slot[0];
  for (int i = 1; i < OA_SLOTS; i++) if (sf_frame_no - g_slot[i].frame > sf_frame_no - old->frame) old = &g_slot[i];
  old->q = q; old->pending = false; old->frame = sf_frame_no - OA_MAX_AGE - 1;
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

static IDirect3DSurface8 *make_surface(IDirect3DDevice8 *dev, IDirect3DSurface8 *game, int kind) {
  D3DSURFACE_DESC d;
  if (FAILED(game->GetDesc(&d))) return NULL;
  IDirect3DSurface8 *s = NULL;
  HRESULT hr = kind == 1 ? dev->CreateImageSurface(16, 16, d.Format, &s)
                         : dev->CreateRenderTarget(16, 16, d.Format, D3DMULTISAMPLE_NONE, TRUE, &s);
  static bool logged[3];
  if (FAILED(hr)) {
    if (!logged[kind]) { logged[kind] = true; H->log("speedfix: occlusion_async: %s(16, 16, format %u) failed: 0x%08lX", kind == 1 ? "CreateImageSurface" : "CreateRenderTarget", (unsigned)d.Format, (unsigned long)hr); }
    return NULL;
  }
  if (!logged[kind]) {
    logged[kind] = true;
    D3DSURFACE_DESC m;
    if (SUCCEEDED(s->GetDesc(&m)))
      H->log("speedfix: occlusion_async: its %s surfaces: format %u, pool %u (the game's own: format %u, pool %u)",
             kind == 1 ? "image" : "render-target", (unsigned)m.Format, (unsigned)m.Pool, (unsigned)d.Format, (unsigned)d.Pool);
  }
  return s;
}

// copy this probe's picture into the object's own surface; false if it could not be
static bool queue_copy(IDirect3DDevice8 *dev, IDirect3DTexture8 *tex, IDirect3DSurface8 *game, OaSlot *e, int kind) {
  if (e->s && e->kind != kind) release_slot(e);
  if (!e->s) { e->s = make_surface(dev, game, kind); e->kind = e->s ? kind : 0; }
  if (!e->s) return false;
  IDirect3DSurface8 *src = NULL;
  LONGLONG t = qpc();
  if (FAILED(tex->GetSurfaceLevel(0, &src)) || !src) return false;
  HRESULT hr = dev->CopyRects(src, &kRect16, 1, e->s, NULL);
  src->Release();
  g_tCopy += qpc() - t; g_nCopy++;
  if (FAILED(hr)) {
    static bool logged[3];
    if (!logged[kind]) { logged[kind] = true; H->log("speedfix: occlusion_async: CopyRects into its %s surface failed: 0x%08lX", kind == 1 ? "image" : "render-target", (unsigned long)hr); }
    if (kind == 1 && !g_imageFailed) { g_imageFailed = true; H->log("speedfix: occlusion_async: mode 1 falls back to mode 2's surface (a lockable render target)"); }
    release_slot(e);
    return false;
  }
  e->pending = true; e->frame = sf_frame_no;
  return true;
}

// read the object's previous picture; -1 if there is none young enough
static int read_previous(OaSlot *e) {
  if (!e->pending || !e->s) return -1;
  e->pending = false;
  if (sf_frame_no - e->frame > OA_MAX_AGE) return -1;
  D3DLOCKED_RECT lr;
  LONGLONG t = qpc();
  if (FAILED(e->s->LockRect(&lr, &kRect16, D3DLOCK_READONLY))) { g_tRead += qpc() - t; g_nRead++; return 0; }   // as the game answers a failed lock
  int n = count_locked(lr);
  e->s->UnlockRect();
  g_tRead += qpc() - t; g_nRead++;
  return n;
}

// called for argument 0 only, with occlusion_async on and the kit's hooks live (sf_main.cpp, sf_readback)
extern "C" int sf_occl_readback(void *q, int mode) {
  uintptr_t modx = *(uintptr_t *)XI_GAME(0x1047BFA8u);
  uint8_t *mng = *(uint8_t **)XI_GAME(0x1047C004u);
  IDirect3DDevice8 *dev = modx ? *(IDirect3DDevice8 **)(modx + 0xEBC) : NULL;
  IDirect3DTexture8 *tex = mng ? *(IDirect3DTexture8 **)(mng + 0xC) : NULL;
  IDirect3DSurface8 *gsurf = mng ? *(IDirect3DSurface8 **)(mng + 0x18) : NULL;
  if (!dev || !tex || !gsurf) { g_nSync++; return sf_occl_game(q, 0); }
  OaSlot *e = find_slot(q);
  if (!e) { g_nNoSlot++; return sf_occl_game(q, 0); }
  int kind = mode == 1 && !g_imageFailed ? 1 : 2;
  int prev = read_previous(e);
  int ans;
  if (mode == 3) {                            // measure only
    ans = sf_occl_game(q, 0);
    if (prev >= 0) { if (prev == ans) g_nSame++; else g_nDiffer++; if ((prev == 0) != (ans == 0)) g_nVisFlip++; }
  } else if (prev >= 0) { ans = prev; g_nAsync++; }
  else { ans = sf_occl_game(q, 0); g_nSync++; }
  if (!queue_copy(dev, tex, gsurf, e, kind)) g_nCopyFail++;
  return ans;
}

extern "C" void sf_occl_stats(void) {
  LARGE_INTEGER qf; QueryPerformanceFrequency(&qf);
  double us = 1e6 / (double)qf.QuadPart;
  if (g_nGame + g_nCopy + g_nRead)
    H->log("speedfix: readback time: the game's readback %ld calls, mean %.1f us, %.3f ms a frame with occlusion_async off (%ld frames); "
           "occlusion_async's copy %ld, mean %.1f us; its read %ld, mean %.1f us; the two %.3f ms a frame with it on (%ld frames)",
           g_nGame, g_nGame ? g_tGame * us / g_nGame : 0.0, g_framesOff ? g_tGame * us / 1000.0 / g_framesOff : 0.0, g_framesOff,
           g_nCopy, g_nCopy ? g_tCopy * us / g_nCopy : 0.0, g_nRead, g_nRead ? g_tRead * us / g_nRead : 0.0,
           g_framesOn ? (g_tCopy + g_tRead) * us / 1000.0 / g_framesOn : 0.0, g_framesOn);
  g_tGame = g_tCopy = g_tRead = 0; g_nGame = g_nCopy = g_nRead = g_framesOn = g_framesOff = 0;
  LONG a = InterlockedExchange(&g_nAsync, 0), s = InterlockedExchange(&g_nSync, 0), f = InterlockedExchange(&g_nCopyFail, 0);
  LONG n = InterlockedExchange(&g_nNoSlot, 0), same = InterlockedExchange(&g_nSame, 0), dif = InterlockedExchange(&g_nDiffer, 0);
  LONG flip = InterlockedExchange(&g_nVisFlip, 0);
  if (a + s + f + n) H->log("speedfix: occlusion_async: %ld answered from the previous probe, %ld by the game's readback (first or stale), "
                            "%ld copies failed, %ld without a slot; %d query objects seen", a, s, f, n, g_used);
  if (same + dif) H->log("speedfix: occlusion_async=3: of %ld probes with a previous picture, the delayed answer differed from the "
                         "game's in %ld (%.1f%%); visible/hidden differed in %ld (%.1f%%)", same + dif, dif,
                         100.0 * dif / (same + dif), flip, 100.0 * flip / (same + dif));
}
