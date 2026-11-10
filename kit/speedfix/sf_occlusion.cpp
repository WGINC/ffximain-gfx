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
// Without a wait (10 Nov; speedfix.ini occlusion_nowait=1, the default): the owner's run of 6 Nov (start 3) had the late
// read at a mean of 1-2 ms a call, 2.3-5.8 ms a frame: d3d8on11's own thread runs up to two frames behind the game, so a
// lock made a frame after its copy found no snapshot and waited (d3d8on11's log: a quarter of the locks). When the
// d3d8.dll in use exports D8ON11_LockDoNotWait (kit/d3d8on11), the late read passes that flag: a lock whose snapshot is
// not there yet fails at once, and nothing waits. Each object then keeps up to OL_RING pictures in flight, so an object
// probed every frame still gets a new answer each probe; the answer returned is the newest finished picture's, or the
// last answer again when none has finished since. So the answer can be more than one probe late (a picture older than
// OL_MAX_AGE frames is dropped; with none in flight and none answered the game's own readback answers, as before).
// Modes (speedfix.ini occlusion_late): 1 the late answer; 2 measure only: the game's answer is returned, the late one
// is computed beside it and how often they differ is logged. Every surface is released before Reset.
#include <windows.h>
#include <d3d8.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include "speedfix.h"

static const XikitHost *H;
#define XI_GAME(a) sf_game_addr((uint32_t)(a))

enum { OL_SLOTS = 1024, OL_MAX_AGE = 32, OL_RING = 3 };   // OL_MAX_AGE, OL_RING: chosen bounds, not facts of the game
struct OlPic { IDirect3DSurface8 *s; int frame; bool pending; unsigned seq; };   // seq: the order the pictures were copied in
static unsigned g_seq;
// pic[0] is the one picture of the waiting path; last/held: occlusion_hold's; ans: the last answer given (-1 none)
struct OlSlot { void *q; OlPic pic[OL_RING]; int last; bool held; int ans; };
static DWORD g_nowait_flag; static bool g_nowait_asked;
static volatile LONG g_nNotYet, g_nAgain, g_nNoRoom, g_nFirst;
enum : HRESULT { OL_NOTYET = (HRESULT)0x8876021CL };          // kit/d3d8on11 d8.h D8ON11_NOTYET
// the flag for a lock that must not wait, or 0: asked of the d3d8.dll in use once (and again after a test sets it)
static DWORD nowait_flag(void) {
  if (!g_nowait_asked) {
    g_nowait_asked = true;
    HMODULE m = GetModuleHandleA("d3d8.dll");
    typedef unsigned (__stdcall *PFN)(void);
    PFN f = m ? (PFN)(void *)GetProcAddress(m, "D8ON11_LockDoNotWait") : NULL;
    g_nowait_flag = f ? f() : 0;
    H->log("speedfix: occlusion_late: %s", g_nowait_flag ? "the d3d8.dll in use exports D8ON11_LockDoNotWait: late reads do not wait (occlusion_nowait)"
                                                         : "the d3d8.dll in use has no D8ON11_LockDoNotWait: late reads may wait, as before");
  }
  return sf_cfg.occlusion_nowait ? g_nowait_flag : 0;
}
// for the host test (sf_host.c): the flag as a d3d8.dll with the export would give it
extern "C" __declspec(dllexport) void sf_occl_test_nowait(unsigned flag) { g_nowait_flag = flag; g_nowait_asked = true; }
static OlSlot g_slot[OL_SLOTS];
static int g_used;
static volatile LONG g_nLate, g_nSync, g_nCopyFail, g_nSame, g_nDiffer, g_nVisFlip, g_nHeld;
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
  for (OlPic &p : e->pic) { if (p.s) p.s->Release(); p.s = NULL; p.pending = false; }
}
extern "C" void sf_occl_reset(void) {         // before the device's Reset (sf_device.cpp, h_Reset): default-pool surfaces
  for (int i = 0; i < OL_SLOTS; i++) release_slot(&g_slot[i]);
}

static OlSlot *find_slot(void *q) {
  unsigned h = (unsigned)(((uintptr_t)q >> 2) * 2654435761u) % OL_SLOTS;
  for (int i = 0; i < OL_SLOTS; i++) {
    OlSlot *e = &g_slot[(h + i) % OL_SLOTS];
    if (e->q == q) return e;
    if (!e->q) { e->q = q; e->last = -1; e->held = false; e->ans = -1; g_used++; return e; }
  }
  // full: the slot probed longest ago is given to this object (its surfaces are kept: every one is alike)
  auto newest = [](const OlSlot *x) { int f = INT_MIN; for (const OlPic &p : x->pic) if (p.s && p.frame > f) f = p.frame; return f; };
  OlSlot *old = &g_slot[0];
  for (int i = 1; i < OL_SLOTS; i++) if (newest(&g_slot[i]) < newest(old)) old = &g_slot[i];
  old->q = q; for (OlPic &p : old->pic) p.pending = false; old->last = -1; old->held = false; old->ans = -1;
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

// copy this probe's picture into one of the object's own surfaces; false if it could not be
static bool queue_copy(IDirect3DDevice8 *dev, IDirect3DTexture8 *tex, IDirect3DSurface8 *game, OlPic *p) {
  if (!p->s) p->s = make_surface(dev, game);
  if (!p->s) return false;
  IDirect3DSurface8 *src = NULL;
  LONGLONG t = qpc();
  if (FAILED(tex->GetSurfaceLevel(0, &src)) || !src) return false;
  HRESULT hr = dev->CopyRects(src, &kRect16, 1, p->s, NULL);
  src->Release();
  g_tCopy += qpc() - t; g_nCopy++;
  if (FAILED(hr)) {
    static bool logged;
    if (!logged) { logged = true; H->log("speedfix: occlusion_late: CopyRects into its surface failed: 0x%08lX", (unsigned long)hr); }
    p->s->Release(); p->s = NULL; p->pending = false;
    return false;
  }
  p->pending = true; p->frame = sf_frame_no; p->seq = ++g_seq;
  return true;
}

// read one picture; its count, -1 if there is none young enough, -2 if it is not finished (a lock that does not wait)
static int read_pic(OlPic *p, DWORD flag) {
  if (!p->pending || !p->s) return -1;
  if (sf_frame_no - p->frame > OL_MAX_AGE) { p->pending = false; return -1; }
  D3DLOCKED_RECT lr; int n = 0;
  LONGLONG t = qpc();
  HRESULT hr = p->s->LockRect(&lr, &kRect16, D3DLOCK_READONLY | flag);
  if (flag && hr == OL_NOTYET) { g_tRead += qpc() - t; return -2; }
  p->pending = false;
  if (SUCCEEDED(hr)) { n = count_locked(lr); p->s->UnlockRect(); }   // a failed lock: 0, as the game answers
  LONGLONG dt = qpc() - t;
  g_tRead += dt; g_nRead++; if (dt > g_tReadMax) g_tReadMax = dt;
  return n;
}
// the object's late answer: -1 if it has none (the game's readback must answer). Without the flag: the one previous
// picture, waited for. With it: the newest finished picture's count, else the last answer again while one is in flight.
static int read_previous(OlSlot *e, DWORD flag) {
  if (!flag) return read_pic(&e->pic[0], 0);
  int got = -1; unsigned gotseq = 0; bool flying = false;
  for (OlPic &p : e->pic) {
    unsigned q = p.seq; int n = read_pic(&p, flag);
    if (n == -2) { flying = true; g_nNotYet++; }
    else if (n >= 0 && (got < 0 || (int)(q - gotseq) > 0)) { got = n; gotseq = q; }
  }
  if (got >= 0) return got;
  if (flying && e->ans >= 0) { g_nAgain++; return e->ans; }
  return -1;
}

// occlusion_hold: the answer to return given this probe's answer `ans` and the object's history. The late answer shows an
// object's visibility one probe late, and on 3-4% of probes (9 Nov, occlusion_late=2) visible and hidden differ: an
// object seen at the last probe and answered hidden now is kept at its last count for this one probe; it is hidden at
// the second hidden answer in a row. A chosen rule (it draws a hidden object one probe longer), not the game's.
extern "C" int sf_occl_hold(int *last, bool *held, int ans, int hold) {
  int r = ans;
  if (hold && ans == 0 && *last > 0 && !*held) { r = *last; *held = true; g_nHeld++; }
  else { *held = false; *last = ans; }
  return r;
}
// the rule run on a sequence of answers (the host test, sf_host.c): out[i] = what speedfix returns for in[i]
extern "C" __declspec(dllexport) void sf_occl_hold_test(const int *in, int n, int *out) {
  int last = -1; bool held = false;
  for (int i = 0; i < n; i++) out[i] = sf_occl_hold(&last, &held, in[i], 1);
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
  DWORD flag = nowait_flag();
  int prev = read_previous(e, flag);
  int ans;
  if (mode == 2) {                            // measure only
    ans = sf_occl_game(q, 0);
    if (prev >= 0) { if (prev == ans) g_nSame++; else g_nDiffer++; if ((prev == 0) != (ans == 0)) g_nVisFlip++; }
  } else if (prev >= 0) { ans = prev; g_nLate++; }
  else if (mode == 1 && sf_cfg.occlusion_first) {
    // occlusion_first (a chosen rule, not the game's; off by default): an object with no finished picture of its own
    // is not read back with a wait. Its last answer is given again if it has one; else 2 answers "all visible" (256:
    // what is drawn is still hidden by the depth test where it is behind something, but a count used as a brightness
    // is full for that probe) and 1 answers "hidden" (0: the object appears one probe later).
    ans = e->ans >= 0 ? e->ans : sf_cfg.occlusion_first == 2 ? 256 : 0; g_nFirst++;
  }
  else { ans = sf_occl_game(q, 0); g_nSync++; }
  e->ans = ans;
  if (mode == 1) ans = sf_occl_hold(&e->last, &e->held, ans, sf_cfg.occlusion_hold);
  // this probe's picture: into the one surface (waiting path), or into one not in flight (a copy into a surface in
  // flight would make its snapshot stale: kit/d3d8on11, the write count)
  OlPic *to = &e->pic[0];
  if (flag) { to = NULL; for (OlPic &p : e->pic) if (!p.pending) { to = &p; break; } }
  if (!to) g_nNoRoom++;
  else if (!queue_copy(dev, tex, gsurf, to)) g_nCopyFail++;
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
  LONG hd = InterlockedExchange(&g_nHeld, 0);
  LONG ny = InterlockedExchange(&g_nNotYet, 0), ag = InterlockedExchange(&g_nAgain, 0), nr = InterlockedExchange(&g_nNoRoom, 0);
  LONG fi = InterlockedExchange(&g_nFirst, 0);
  if (fi) H->log("speedfix: occlusion_first=%d: %ld probes with no finished picture answered without the game's readback", sf_cfg.occlusion_first, fi);
  if (ny + ag + nr) H->log("speedfix: occlusion_nowait: %ld locks told not yet (no wait), %ld probes answered with the object's last answer again, "
                           "%ld probes whose picture was not copied (all %d of the object's in flight)", ny, ag, nr, (int)OL_RING);
  if (a + s + f) H->log("speedfix: occlusion_late: %ld answered from the previous probe, %ld by the game's readback (first or stale), "
                        "%ld copies failed; %d query objects seen; occlusion_hold kept %ld visible one probe longer", a, s, f, g_used, hd);
  if (same + dif) H->log("speedfix: occlusion_late=2: of %ld probes with a previous picture, the late answer differed from the "
                         "game's in %ld (%.1f%%); visible/hidden differed in %ld (%.1f%%)", same + dif, dif,
                         100.0 * dif / (same + dif), flip, 100.0 * flip / (same + dif));
}
