/* fcap.c -- frame capture: one frame of the game's Direct3D 8 calls with every piece of data they use, for the
   replayer (kit/fcap/replay.c), which plays it into any d3d8.dll: Wine's, ours (kit/d3d8on11), dgVoodoo. Built into
   d3dprof (and so into xikit.dll); xikit.ini fcap=1 turns it on (docs/KIT.md, "Frame capture"). File format:
   fcap_fmt.h.

   What it needs from the start: the contents of the vertex and index buffers in D3DPOOL_DEFAULT. Those are
   write-only to the game (D3DUSAGE_WRITEONLY; the game's dynamic buffers and its model index buffers, run 2's
   resource log), so they cannot be read back when the frame comes. Every buffer the device creates gets d3dprof's
   Lock/Unlock wrapper; at Unlock the written range is copied into a CPU copy of a DEFAULT-pool buffer, freed at its
   last Release. Managed textures and buffers are read back with D3DLOCK_READONLY when the frame first uses them;
   render-target textures through CopyRects into an image surface; depth surfaces are not read.

   The capture itself runs on the render thread from one Present to the next. Calls from other threads in that frame
   are counted and noted, not recorded. */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fcap_fmt.h"

typedef HRESULT (__stdcall *F_u)(void *);
typedef HRESULT (__stdcall *F_up)(void *, void *);
typedef HRESULT (__stdcall *F_uup)(void *, uint32_t, void *);
typedef HRESULT (__stdcall *F_uuup)(void *, uint32_t, uint32_t, void *);
typedef HRESULT (__stdcall *F_uppu)(void *, uint32_t, void *, void *, uint32_t);
typedef HRESULT (__stdcall *F_uuppu)(void *, uint32_t, uint32_t, void *, void *, uint32_t);
typedef HRESULT (__stdcall *F_uu)(void *, uint32_t, uint32_t);
typedef HRESULT (__stdcall *F_upp)(void *, void *, void *);
typedef HRESULT (__stdcall *F_pupp)(void *, void *, void *, uint32_t, void *, void *);
typedef HRESULT (__stdcall *F_uuuup)(void *, uint32_t, uint32_t, uint32_t, void *);
typedef HRESULT (__stdcall *F_lock)(void *, uint32_t, uint32_t, uint8_t **, uint32_t);
#define VT(o) (*(void ***)(o))

int fcap_enabled;                         /* xikit.ini fcap=1: buffer copies kept from the start */
static void **g_dvt;                      /* the device vtable calls go through (d3dprof's g_orig) */
static void *g_dev;
static char g_dir[MAX_PATH];
static int g_state;                       /* 0 off, 1 copies only, 2 armed (the next Present starts a frame), 3 capturing */
static int g_internal;                    /* fcap's own locks: the buffer hooks leave them alone */
static int g_inblock;                     /* between BeginStateBlock and EndStateBlock */
static FILE *g_f; static unsigned g_count; static uint32_t g_frame, g_game_ts;
static uint32_t g_notes, g_calls, g_bytes_written;
static void *g_bb, *g_autods;
static DWORD g_rt;                        /* the render thread (the one fcap_call runs on) */
static uint32_t g_other_writes;           /* buffers written by other threads during the frame */
static CRITICAL_SECTION g_lock; static int g_lock_init;
uint32_t fcap_other_thread_calls;         /* d3dprof counts device calls from other threads while capturing */
static uint32_t g_other_at_start;

/* ---- the file ----------------------------------------------------------------------------------------------------- */
static uint8_t *g_rec; static uint32_t g_rec_n, g_rec_cap;
static void rbytes(const void *p, uint32_t n) {
  if (g_rec_n + n + 4 > g_rec_cap) {
    uint32_t c = g_rec_cap ? g_rec_cap : 1 << 16;
    while (c < g_rec_n + n + 4) c *= 2;
    uint8_t *q = (uint8_t *)realloc(g_rec, c); if (!q) return;
    g_rec = q; g_rec_cap = c;
  }
  if (p) memcpy(g_rec + g_rec_n, p, n); else memset(g_rec + g_rec_n, 0, n);
  g_rec_n += n;
  while (g_rec_n & 3) g_rec[g_rec_n++] = 0;
}
static void r32(uint32_t v) { rbytes(&v, 4); }
static void rec_begin(void) { g_rec_n = 0; }
static void rec_end(uint32_t tag) {
  if (!g_f) return;
  uint32_t h[2] = {tag, g_rec_n};
  fwrite(h, 4, 2, g_f); if (g_rec_n) fwrite(g_rec, 1, g_rec_n, g_f);
  g_bytes_written += 8 + g_rec_n;
}
static void note(const char *fmt, ...) {
  char b[512]; va_list ap; va_start(ap, fmt); int n = vsnprintf(b, sizeof b, fmt, ap); va_end(ap);
  if (n < 0) return;
  if (n >= (int)sizeof b) n = sizeof b - 1;
  rec_begin(); rbytes(b, (uint32_t)n + 1); rec_end(FC_NOTE); g_notes++;
}

/* ---- buffer copies (from the start) ------------------------------------------------------------------------------ */
typedef struct { void *obj; uint8_t *mem; uint32_t len, usage, fvf_or_fmt, pool; uint8_t ib, used; } Buf;
#define NBUF 65536
static Buf *g_buf;
typedef struct { void *obj; uint8_t *p; uint32_t off, size; } Lk;
#define NLK 1024
static Lk g_lk[NLK];
static Buf *buf_find(void *obj, int add) {
  if (!g_buf) return NULL;
  uint32_t h = ((uint32_t)(uintptr_t)obj >> 4) & (NBUF - 1), free_at = NBUF;
  for (uint32_t n = 0; n < NBUF; n++, h = (h + 1) & (NBUF - 1)) {
    if (g_buf[h].obj == obj && g_buf[h].used) return &g_buf[h];
    if (!g_buf[h].used) { if (free_at == NBUF) free_at = h; if (!g_buf[h].obj) break; }   /* obj set, used 0: tombstone */
  }
  if (!add || free_at == NBUF) return NULL;
  Buf *b = &g_buf[free_at]; memset(b, 0, sizeof *b); b->obj = obj; b->used = 1;
  return b;
}
/* Lock, Unlock and Release of the buffer classes, patched in their vtables (shared by every object of the class): an
   object keeps its own vtable pointer (Wine's d3d8 checks it: dlls/d3d8/buffer.c asserts lpVtbl at SetStreamSource) */
static void fcap_buffer_locked(void *obj, uint32_t off, uint32_t size, uint8_t *p);
static void fcap_buffer_unlocking(void *obj);
static void fcap_buffer_released(void *obj);
#define NCLS 8
static void **g_cls[NCLS]; static void *g_cls_orig[NCLS][3]; static uint32_t g_cls_type[NCLS]; static int g_ncls;   /* slots 2 (Release), 11 (Lock), 12 (Unlock) */
/* the class of an object: by its vtable, or, for an object given a copy of a patched vtable (the kit's buffer trace,
   d3dprof hook_buf), by its resource type (IDirect3DResource8::GetType, slot 10, which the copy keeps) */
static int cls_of(void *obj) {
  void **vt = VT(obj);
  for (int i = 0; i < g_ncls; i++) if (g_cls[i] == vt) return i;
  uint32_t t = ((F_u)vt[10])(obj);
  for (int i = 0; i < g_ncls; i++) if (g_cls_type[i] == t) return i;
  return 0;
}
static HRESULT __stdcall cls_lock(void *self, uint32_t off, uint32_t size, uint8_t **pp, uint32_t flags) {
  int i = cls_of(self);
  HRESULT hr = ((F_lock)g_cls_orig[i][1])(self, off, size, pp, flags);
  if (hr >= 0 && pp) fcap_buffer_locked(self, off, size, *pp);
  return hr;
}
static HRESULT __stdcall cls_unlock(void *self) {
  int i = cls_of(self);
  fcap_buffer_unlocking(self);
  return ((F_u)g_cls_orig[i][2])(self);
}
static ULONG __stdcall cls_release(void *self) {
  int i = cls_of(self);
  ULONG n = ((ULONG (__stdcall *)(void *))g_cls_orig[i][0])(self);
  if (n == 0) fcap_buffer_released(self);
  return n;
}
static void hook_class(void *obj) {
  void **vt = VT(obj);
  for (int k = 0; k < g_ncls; k++) if (g_cls[k] == vt) return;
  if (g_ncls >= NCLS) return;
  DWORD old;
  if (!VirtualProtect(vt, 13 * sizeof(void *), PAGE_READWRITE, &old)) return;
  int i = g_ncls;
  g_cls_orig[i][0] = vt[2]; g_cls_orig[i][1] = vt[11]; g_cls_orig[i][2] = vt[12]; g_cls_type[i] = ((F_u)vt[10])(obj);
  g_cls[i] = vt; g_ncls++;                                  /* (registered before the entries change: a call in between finds it) */
  vt[2] = (void *)cls_release; vt[11] = (void *)cls_lock; vt[12] = (void *)cls_unlock;
  VirtualProtect(vt, 13 * sizeof(void *), old, &old);
}
void fcap_buffer_created(void *obj, int ib, uint32_t len, uint32_t usage, uint32_t fvf_or_fmt, uint32_t pool) {
  if (!fcap_enabled || !obj) return;
  EnterCriticalSection(&g_lock); hook_class(obj); LeaveCriticalSection(&g_lock);
  EnterCriticalSection(&g_lock);
  Buf *b = buf_find(obj, 0);
  if (b) { free(b->mem); b->mem = NULL; b->used = 0; }      /* a new object at a freed one's address */
  b = buf_find(obj, 1);
  if (b) {
    b->len = len; b->usage = usage; b->fvf_or_fmt = fvf_or_fmt; b->pool = pool; b->ib = (uint8_t)ib;
    if (pool == 0 /* D3DPOOL_DEFAULT */ && len <= (64u << 20)) b->mem = (uint8_t *)calloc(1, len);
  }
  LeaveCriticalSection(&g_lock);
}
static int g_emitted_has(void *obj);
static void g_emitted_del(void *obj);
static void fcap_buffer_locked(void *obj, uint32_t off, uint32_t size, uint8_t *p) {
  if (!fcap_enabled || g_internal || !p) return;
  EnterCriticalSection(&g_lock);
  Buf *b = buf_find(obj, 0);
  if (b) {
    if (size == 0 || off + size > b->len) size = off < b->len ? b->len - off : 0;
    uint32_t h = ((uint32_t)(uintptr_t)obj >> 4) & (NLK - 1);
    for (int n = 0; n < NLK; n++, h = (h + 1) & (NLK - 1))
      if (!g_lk[h].obj || g_lk[h].obj == obj) { g_lk[h].obj = obj; g_lk[h].p = p; g_lk[h].off = off; g_lk[h].size = size; break; }
  }
  LeaveCriticalSection(&g_lock);
}
static void fcap_buffer_unlocking(void *obj) {
  if (!fcap_enabled || g_internal) return;
  EnterCriticalSection(&g_lock);
  uint32_t h = ((uint32_t)(uintptr_t)obj >> 4) & (NLK - 1);
  for (int n = 0; n < NLK; n++, h = (h + 1) & (NLK - 1)) {
    if (g_lk[h].obj != obj) { if (!g_lk[h].obj) break; continue; }
    Lk k = g_lk[h];
    /* remove, keeping the probe chain: re-insert the entries after it */
    g_lk[h].obj = NULL;
    for (uint32_t j = (h + 1) & (NLK - 1); g_lk[j].obj; j = (j + 1) & (NLK - 1)) {
      Lk m = g_lk[j]; g_lk[j].obj = NULL;
      uint32_t t = ((uint32_t)(uintptr_t)m.obj >> 4) & (NLK - 1);
      while (g_lk[t].obj) t = (t + 1) & (NLK - 1);
      g_lk[t] = m;
    }
    Buf *b = buf_find(obj, 0);
    if (b && k.size && !IsBadReadPtr(k.p, k.size)) {
      if (b->mem) memcpy(b->mem + k.off, k.p, k.size);
      if (g_state == 3 && GetCurrentThreadId() != g_rt && g_emitted_has(obj)) g_other_writes++;
      if (g_state == 3 && GetCurrentThreadId() == g_rt && g_emitted_has(obj)) {
        rec_begin(); r32((uint32_t)(uintptr_t)obj); r32(k.off); r32(k.size); rbytes(k.p, k.size); rec_end(FC_BUFDATA);
      }
    }
    break;
  }
  LeaveCriticalSection(&g_lock);
}
static void fcap_buffer_released(void *obj) {
  if (!fcap_enabled) return;
  EnterCriticalSection(&g_lock);
  Buf *b = buf_find(obj, 0);
  if (b) { free(b->mem); b->mem = NULL; b->used = 0; }
  LeaveCriticalSection(&g_lock);
  if (g_state == 3) g_emitted_del(obj);
}

/* ---- vertex shader declarations (from the start) ----------------------------------------------------------------- */
typedef struct { uint32_t h, nd, nf; uint32_t *tok; } Vs;
static Vs g_vs[4096]; static int g_nvs;
void fcap_vs_created(uint32_t h, const uint32_t *decl, uint32_t nd, const uint32_t *func, uint32_t nf) {
  if (!fcap_enabled) return;
  EnterCriticalSection(&g_lock);
  int i = 0; while (i < g_nvs && g_vs[i].h != h) i++;
  if (i == g_nvs && g_nvs < 4096) g_nvs++;
  if (i < 4096) {
    free(g_vs[i].tok);
    g_vs[i].h = h; g_vs[i].nd = nd; g_vs[i].nf = nf; g_vs[i].tok = (uint32_t *)malloc(4 * (nd + nf + 1));
    if (g_vs[i].tok) { if (decl) memcpy(g_vs[i].tok, decl, 4 * nd); if (func) memcpy(g_vs[i].tok + nd, func, 4 * nf); }
  }
  LeaveCriticalSection(&g_lock);
}

/* ---- objects written in this frame -------------------------------------------------------------------------------- */
#define NEMIT 32768
static void *g_emit[NEMIT]; static uint32_t g_vs_emit[1024]; static int g_nvs_emit;
static int g_emitted_has(void *obj) {
  uint32_t h = ((uint32_t)(uintptr_t)obj >> 4) & (NEMIT - 1);
  for (int n = 0; n < NEMIT; n++, h = (h + 1) & (NEMIT - 1)) { if (g_emit[h] == obj) return 1; if (!g_emit[h]) return 0; }
  return 0;
}
static void g_emitted_add(void *obj) {
  uint32_t h = ((uint32_t)(uintptr_t)obj >> 4) & (NEMIT - 1);
  for (int n = 0; n < NEMIT; n++, h = (h + 1) & (NEMIT - 1)) if (!g_emit[h] || g_emit[h] == (void *)1) { g_emit[h] = obj; return; }
}
static void g_emitted_del(void *obj) {
  uint32_t h = ((uint32_t)(uintptr_t)obj >> 4) & (NEMIT - 1);
  for (int n = 0; n < NEMIT; n++, h = (h + 1) & (NEMIT - 1)) { if (g_emit[h] == obj) { g_emit[h] = (void *)1; return; } if (!g_emit[h]) return; }
}
void fcap_object_created(void *obj) { if (g_state == 3 && obj) g_emitted_del(obj); }   /* a new object at an old address */

/* bytes per row and rows of a level (d3d8types.h formats; DXTn: 4x4 blocks) */
static int fmt_rows(uint32_t fmt, uint32_t w, uint32_t h, uint32_t *rowbytes, uint32_t *rows) {
  uint32_t bpp = 0, blk = 0;
  switch (fmt) {
  case 21: case 22: bpp = 4; break;                          /* A8R8G8B8, X8R8G8B8 */
  case 20: bpp = 3; break;                                   /* R8G8B8 */
  case 23: case 24: case 25: case 26: case 51: bpp = 2; break; /* R5G6B5, X1R5G5B5, A1R5G5B5, A4R4G4B4, A8L8 */
  case 28: case 50: case 41: bpp = 1; break;                 /* A8, L8, P8 */
  case 0x31545844: blk = 8; break;                           /* DXT1 */
  case 0x32545844: case 0x33545844: case 0x34545844: case 0x35545844: blk = 16; break;   /* DXT2..5 */
  default: return 0;
  }
  if (blk) { *rowbytes = ((w + 3) / 4) * blk; *rows = (h + 3) / 4; }
  else { *rowbytes = w * bpp; *rows = h; }
  return 1;
}
static void put_locked(const uint8_t *bits, int32_t pitch, uint32_t rowbytes, uint32_t rows) {
  r32(rowbytes * rows);
  for (uint32_t y = 0; y < rows; y++) rbytes(bits + (int32_t)y * pitch, rowbytes);   /* rbytes pads each row to 4 */
}
/* a surface's pixels through CopyRects into an image surface (render targets: D3DPOOL_DEFAULT, not lockable) */
static int put_by_copy(void *surf, uint32_t fmt, uint32_t w, uint32_t h, uint32_t rowbytes, uint32_t rows) {
  void *img = NULL; int ok = 0;
  if (((F_uuuup)g_dvt[27])(g_dev, w, h, fmt, &img) >= 0 && img) {
    if (((F_pupp)g_dvt[28])(g_dev, surf, NULL, 0, img, NULL) >= 0) {
      uint32_t lr[2];
      if (((HRESULT (__stdcall *)(void *, void *, void *, uint32_t))VT(img)[9])(img, lr, NULL, 0x10) >= 0) {
        put_locked((const uint8_t *)(uintptr_t)lr[1], (int32_t)lr[0], rowbytes, rows); ok = 1;
        ((F_u)VT(img)[10])(img);
      }
    }
    ((F_u)VT(img)[2])(img);
  }
  return ok;
}
/* row-padded size of a level as put_locked writes it */
static void emit_tex(void *t) {
  uint32_t type = ((F_u)VT(t)[10])(t);                       /* IDirect3DResource8::GetType */
  uint32_t levels = ((F_u)VT(t)[13])(t), d[8] = {0};         /* GetLevelCount; D3DSURFACE_DESC */
  int cube = type == 5 /* D3DRTYPE_CUBETEXTURE */;
  if (type != 3 && !cube) { note("object %p of resource type %u used as a texture: not captured", t, type); return; }
  if (((F_uup)VT(t)[14])(t, 0, d) < 0) { note("texture %p: GetLevelDesc failed", t); return; }
  rec_begin();
  r32((uint32_t)(uintptr_t)t);
  if (cube) { r32(d[6]); } else { r32(d[6]); r32(d[7]); }
  r32(levels); r32(d[2]); r32(d[0]); r32(d[3]);
  g_internal++;
  for (uint32_t face = 0; face < (cube ? 6u : 1u); face++)
    for (uint32_t l = 0; l < levels; l++) {
      uint32_t ld[8] = {0}, rowbytes = 0, rows = 0, lr[2] = {0};
      ((F_uup)VT(t)[14])(t, l, ld);
      if (!fmt_rows(ld[0], ld[6], ld[7], &rowbytes, &rows)) { if (!face && !l) note("texture %p: format %u not read", t, ld[0]); r32(0); continue; }
      if (ld[3] != 0) {                                      /* MANAGED or SYSTEMMEM: read back */
        HRESULT hr = cube ? ((HRESULT (__stdcall *)(void *, uint32_t, uint32_t, void *, void *, uint32_t))VT(t)[16])(t, face, l, lr, NULL, 0x10)
                          : ((HRESULT (__stdcall *)(void *, uint32_t, void *, void *, uint32_t))VT(t)[16])(t, l, lr, NULL, 0x10);
        if (hr >= 0) {
          put_locked((const uint8_t *)(uintptr_t)lr[1], (int32_t)lr[0], rowbytes, rows);
          if (cube) ((F_uu)VT(t)[17])(t, face, l); else ((F_up)VT(t)[17])(t, (void *)(uintptr_t)l);
        } else { r32(0); note("texture %p level %u: LockRect failed (0x%08lX)", t, l, (unsigned long)hr); }
      } else {                                               /* DEFAULT (a render target): through an image surface */
        void *s = NULL;
        HRESULT hr = cube ? ((F_uuup)VT(t)[15])(t, face, l, &s) : ((F_uup)VT(t)[15])(t, l, &s);
        if (hr < 0 || !s || !put_by_copy(s, ld[0], ld[6], ld[7], rowbytes, rows)) { r32(0); if (!face && !l) note("texture %p (D3DPOOL_DEFAULT) level %u: not read", t, l); }
        if (s) ((F_u)VT(s)[2])(s);
      }
    }
  g_internal--;
  rec_end(cube ? FC_CUBE : FC_TEX);
  g_emitted_add(t);
}
static void emit_buf(void *b, int ib) {
  uint32_t d[6] = {0};
  if (((F_up)VT(b)[13])(b, d) < 0) { note("buffer %p: GetDesc failed", b); return; }
  rec_begin();
  r32((uint32_t)(uintptr_t)b); r32(d[4]); r32(d[2]); r32(ib ? d[0] : d[5]); r32(d[3]);
  EnterCriticalSection(&g_lock);
  Buf *c = buf_find(b, 0);
  if (c && c->mem) { r32(1); rbytes(c->mem, d[4]); }
  else if (d[3] != 0) {                                      /* not DEFAULT: readable */
    uint8_t *p = NULL; g_internal++;
    if (((F_lock)VT(b)[11])(b, 0, 0, &p, 0x10) >= 0 && p) { r32(1); rbytes(p, d[4]); ((F_u)VT(b)[12])(b); }
    else { r32(0); note("buffer %p: Lock READONLY failed", b); }
    g_internal--;
  } else { r32(0); note("buffer %p (D3DPOOL_DEFAULT) has no copy: created before fcap was on, or on another path", b); }
  LeaveCriticalSection(&g_lock);
  rec_end(ib ? FC_IB : FC_VB);
  g_emitted_add(b);
}
static const GUID IID_Tex8 = {0xE4CDD575, 0x2866, 0x4F01, {0xB1, 0x2E, 0x7E, 0xEC, 0xE1, 0xEC, 0x93, 0x58}};
static const GUID IID_Cube8 = {0x3EE5B968, 0x2ACA, 0x4C34, {0x8B, 0xB5, 0x7E, 0x0C, 0x3D, 0x19, 0xB7, 0x50}};
static void emit_surf(void *s) {
  uint32_t d[8] = {0}, kind = 0, cont = 0, level = 0, face = 0;
  ((F_up)VT(s)[8])(s, d);                                    /* GetDesc */
  void *c = NULL;
  if (((F_upp)VT(s)[7])(s, (void *)&IID_Tex8, &c) >= 0 && c) {
    uint32_t n = ((F_u)VT(c)[13])(c);
    for (uint32_t l = 0; l < n && !kind; l++) { void *x = NULL; if (((F_uup)VT(c)[15])(c, l, &x) >= 0 && x) { if (x == s) { kind = FS_TEXLEVEL; level = l; } ((F_u)VT(x)[2])(x); } }
    if (kind && !g_emitted_has(c)) emit_tex(c);
    cont = (uint32_t)(uintptr_t)c; ((F_u)VT(c)[2])(c);
  } else if (((F_upp)VT(s)[7])(s, (void *)&IID_Cube8, &c) >= 0 && c) {
    uint32_t n = ((F_u)VT(c)[13])(c);
    for (uint32_t f = 0; f < 6 && !kind; f++)
      for (uint32_t l = 0; l < n && !kind; l++) { void *x = NULL; if (((F_uuup)VT(c)[15])(c, f, l, &x) >= 0 && x) { if (x == s) { kind = FS_CUBEFACE; level = l; face = f; } ((F_u)VT(x)[2])(x); } }
    if (kind && !g_emitted_has(c)) emit_tex(c);
    cont = (uint32_t)(uintptr_t)c; ((F_u)VT(c)[2])(c);
  }
  if (!kind) {
    if (s == g_bb) kind = FS_BACKBUFFER;
    else if (s == g_autods) kind = FS_AUTODEPTH;
    else if (d[2] & 1) kind = FS_RENDERTARGET;               /* D3DUSAGE_RENDERTARGET */
    else if (d[2] & 2) kind = FS_DEPTHSTENCIL;               /* D3DUSAGE_DEPTHSTENCIL */
    else kind = FS_IMAGE;
  }
  rec_begin();
  r32((uint32_t)(uintptr_t)s); r32(kind); r32(cont); r32(level); r32(face);
  r32(d[0]); r32(d[6]); r32(d[7]); r32(d[2]); r32(d[3]); r32(d[5]);
  uint32_t rowbytes, rows;
  if ((kind == FS_IMAGE || kind == FS_RENDERTARGET) && fmt_rows(d[0], d[6], d[7], &rowbytes, &rows)) {
    uint32_t lr[2] = {0}; int ok = 0; g_internal++;
    if (kind == FS_IMAGE && ((HRESULT (__stdcall *)(void *, void *, void *, uint32_t))VT(s)[9])(s, lr, NULL, 0x10) >= 0) {
      r32(1); put_locked((const uint8_t *)(uintptr_t)lr[1], (int32_t)lr[0], rowbytes, rows); ((F_u)VT(s)[10])(s); ok = 1;
    } else if (kind == FS_RENDERTARGET) {
      uint32_t at = g_rec_n; r32(1);
      if (put_by_copy(s, d[0], d[6], d[7], rowbytes, rows)) ok = 1; else g_rec_n = at;
    }
    if (!ok) r32(0);
    g_internal--;
  } else r32(0);
  rec_end(FC_SURF);
  g_emitted_add(s);
}
static void ensure(void *o, int what) {          /* what: 0 texture, 1 vb, 2 ib, 3 surface */
  if (!o || g_emitted_has(o)) return;
  if (IsBadReadPtr(o, 4) || IsBadReadPtr(VT(o), 4 * 18)) { note("object %p: not readable", o); return; }
  if (what == 0) emit_tex(o); else if (what == 3) emit_surf(o); else emit_buf(o, what == 2);
}
static void ensure_vs(uint32_t h) {
  if (!(h & 1)) return;                          /* an FVF code */
  for (int i = 0; i < g_nvs_emit; i++) if (g_vs_emit[i] == h) return;
  EnterCriticalSection(&g_lock);
  int i = 0; while (i < g_nvs && g_vs[i].h != h) i++;
  if (i < g_nvs && g_vs[i].tok) {
    rec_begin(); r32(h); r32(g_vs[i].nd); r32(g_vs[i].nf); rbytes(g_vs[i].tok, 4 * (g_vs[i].nd + g_vs[i].nf)); rec_end(FC_VSDECL);
  } else note("vertex shader %08X: its declaration was not seen (created before fcap was on)", h);
  LeaveCriticalSection(&g_lock);
  if (g_nvs_emit < 1024) g_vs_emit[g_nvs_emit++] = h;
}

/* ---- calls ---------------------------------------------------------------------------------------------------------- */
static void emit_call(int slot, uint32_t flags, const uint32_t *a, int n) {
  const void *p[2]; uint32_t sz[2];
  int nd = fc_call_data(slot, a, p, sz);
  rec_begin(); r32((uint32_t)slot | flags); r32((uint32_t)n);
  for (int k = 0; k < n; k++) r32(a[k]);
  r32((uint32_t)nd);
  for (int k = 0; k < nd; k++) {
    if (sz[k] && p[k] && !IsBadReadPtr(p[k], sz[k])) { r32(sz[k]); rbytes(p[k], sz[k]); }
    else r32(0);
  }
  rec_end(FC_CALL); g_calls++;
}
static void synth(int slot, uint32_t a0, uint32_t a1, uint32_t a2) { uint32_t a[3] = {a0, a1, a2}; emit_call(slot, FC_SYNTH, a, 3); }

/* ---- the device's state: read back (Get*), kept up to date by the calls, written as Set calls where it changed ---- */
typedef struct {
  uint32_t rs[256]; uint8_t rs_ok[256];
  uint32_t tss[8][29]; uint8_t tss_ok[8][29];
  uint32_t tex[8];
  float xf[512][16]; uint8_t xf_ok[512];
  uint32_t vp[6]; uint8_t vp_ok;
  uint32_t mat[17]; uint8_t mat_ok;
  uint32_t light[256][26]; uint8_t light_ok[256]; uint32_t len[256]; uint8_t len_ok[256];
  float clip[6][4]; uint8_t clip_ok[6];
  uint32_t vs; uint32_t vb[16], stride[16]; uint32_t ib, base;
  uint32_t rt, ds;
} St;
static St *g_cur, *g_new;
static void get_state(St *s, int full) {
  memset(s, 0, sizeof *s);
  for (uint32_t i = 0; i < 256; i++) s->rs_ok[i] = ((F_uup)g_dvt[51])(g_dev, i, &s->rs[i]) >= 0;
  for (uint32_t st = 0; st < 8; st++) for (uint32_t t = 1; t < 29; t++) s->tss_ok[st][t] = ((F_uuup)g_dvt[62])(g_dev, st, t, &s->tss[st][t]) >= 0;
  for (uint32_t st = 0; st < 8; st++) { void *t = NULL; if (((F_uup)g_dvt[60])(g_dev, st, &t) >= 0 && t) { s->tex[st] = (uint32_t)(uintptr_t)t; ((F_u)VT(t)[2])(t); } }
  /* the transform states D3D8 defines (d3d8types.h): VIEW 2, PROJECTION 3, TEXTURE0..7 16..23, WORLDMATRIX(n) 256+n */
  for (uint32_t i = 0; i < 512; i++)
    if (i == 2 || i == 3 || (i >= 16 && i <= 23) || i >= 256) s->xf_ok[i] = ((F_uup)g_dvt[38])(g_dev, i, s->xf[i]) >= 0;
  s->vp_ok = ((F_up)g_dvt[41])(g_dev, s->vp) >= 0;
  s->mat_ok = ((F_up)g_dvt[43])(g_dev, s->mat) >= 0;
  for (uint32_t i = 0; i < 256; i++) {
    s->light_ok[i] = ((F_uup)g_dvt[45])(g_dev, i, s->light[i]) >= 0;
    s->len_ok[i] = s->light_ok[i] && ((F_uup)g_dvt[47])(g_dev, i, &s->len[i]) >= 0;
  }
  for (uint32_t i = 0; i < 6; i++) s->clip_ok[i] = ((F_uup)g_dvt[49])(g_dev, i, s->clip[i]) >= 0;
  ((F_up)g_dvt[77])(g_dev, &s->vs);
  for (uint32_t i = 0; i < 16; i++) { void *b = NULL; if (((HRESULT (__stdcall *)(void *, uint32_t, void *, void *))g_dvt[84])(g_dev, i, &b, &s->stride[i]) >= 0 && b) { s->vb[i] = (uint32_t)(uintptr_t)b; ((F_u)VT(b)[2])(b); } }
  { void *b = NULL; if (((F_upp)g_dvt[86])(g_dev, &b, &s->base) >= 0 && b) { s->ib = (uint32_t)(uintptr_t)b; ((F_u)VT(b)[2])(b); } }
  if (full) {
    void *x = NULL;
    if (((F_up)g_dvt[32])(g_dev, &x) >= 0 && x) { s->rt = (uint32_t)(uintptr_t)x; ((F_u)VT(x)[2])(x); }
    x = NULL;
    if (((F_up)g_dvt[33])(g_dev, &x) >= 0 && x) { s->ds = (uint32_t)(uintptr_t)x; ((F_u)VT(x)[2])(x); }
  }
}
/* the Set calls that take the device from o (what the replay has) to s; o = NULL: everything s holds */
static void emit_diff(const St *o, const St *s, int full) {
  if (full) {
    ensure((void *)(uintptr_t)s->rt, 3); ensure((void *)(uintptr_t)s->ds, 3);
    synth(FS_SETRT, s->rt, s->ds, 0);                        /* (it resets the viewport: the viewport follows) */
  }
  if (s->vp_ok && (!o || full || !o->vp_ok || memcmp(o->vp, s->vp, 24))) { uint32_t a[1] = {(uint32_t)(uintptr_t)s->vp}; emit_call(FS_SETVIEWPORT, FC_SYNTH, a, 1); }
  for (int i = 0; i < 512; i++)
    if (s->xf_ok[i] && (!o || !o->xf_ok[i] || memcmp(o->xf[i], s->xf[i], 64))) { uint32_t a[2] = {(uint32_t)i, (uint32_t)(uintptr_t)s->xf[i]}; emit_call(FS_SETTRANSFORM, FC_SYNTH, a, 2); }
  for (int i = 0; i < 256; i++)
    if (s->rs_ok[i] && (!o || !o->rs_ok[i] || o->rs[i] != s->rs[i])) synth(FS_SETRS, (uint32_t)i, s->rs[i], 0);
  for (int st = 0; st < 8; st++) for (int t = 1; t < 29; t++)
    if (s->tss_ok[st][t] && (!o || !o->tss_ok[st][t] || o->tss[st][t] != s->tss[st][t])) synth(FS_SETTSS, (uint32_t)st, (uint32_t)t, s->tss[st][t]);
  for (int st = 0; st < 8; st++)
    if (!o || o->tex[st] != s->tex[st]) { ensure((void *)(uintptr_t)s->tex[st], 0); synth(FS_SETTEXTURE, (uint32_t)st, s->tex[st], 0); }
  if (s->mat_ok && (!o || !o->mat_ok || memcmp(o->mat, s->mat, 68))) { uint32_t a[1] = {(uint32_t)(uintptr_t)s->mat}; emit_call(FS_SETMATERIAL, FC_SYNTH, a, 1); }
  for (int i = 0; i < 256; i++) {
    if (s->light_ok[i] && (!o || !o->light_ok[i] || memcmp(o->light[i], s->light[i], 104))) { uint32_t a[2] = {(uint32_t)i, (uint32_t)(uintptr_t)s->light[i]}; emit_call(FS_SETLIGHT, FC_SYNTH, a, 2); }
    if (s->len_ok[i] && (!o || !o->len_ok[i] || o->len[i] != s->len[i])) synth(FS_LIGHTENABLE, (uint32_t)i, s->len[i], 0);
  }
  for (int i = 0; i < 6; i++)
    if (s->clip_ok[i] && (!o || !o->clip_ok[i] || memcmp(o->clip[i], s->clip[i], 16))) { uint32_t a[2] = {(uint32_t)i, (uint32_t)(uintptr_t)s->clip[i]}; emit_call(FS_SETCLIPPLANE, FC_SYNTH, a, 2); }
  if (!o || o->vs != s->vs) { ensure_vs(s->vs); synth(FS_SETVS, s->vs, 0, 0); }
  for (int i = 0; i < 16; i++)
    if (!o || o->vb[i] != s->vb[i] || o->stride[i] != s->stride[i]) { ensure((void *)(uintptr_t)s->vb[i], 1); synth(FS_SSS, (uint32_t)i, s->vb[i], s->stride[i]); }
  if (!o || o->ib != s->ib || o->base != s->base) { ensure((void *)(uintptr_t)s->ib, 2); synth(FS_SETINDICES, s->ib, s->base, 0); }
}
/* a call's effect on the kept state (so a state block's effect is written relative to what the replay has) */
static void track(int slot, const uint32_t *a) {
  St *s = g_cur;
  switch (slot) {
  case FS_SETRS: if (a[0] < 256) { s->rs[a[0]] = a[1]; s->rs_ok[a[0]] = 1; } break;
  case FS_SETTSS: if (a[0] < 8 && a[1] < 29) { s->tss[a[0]][a[1]] = a[2]; s->tss_ok[a[0]][a[1]] = 1; } break;
  case FS_SETTEXTURE: if (a[0] < 8) s->tex[a[0]] = a[1]; break;
  case FS_SETTRANSFORM: if (a[0] < 512 && a[1] && !IsBadReadPtr((void *)(uintptr_t)a[1], 64)) { memcpy(s->xf[a[0]], (void *)(uintptr_t)a[1], 64); s->xf_ok[a[0]] = 1; } break;
  case FS_MULTIPLYTRANSFORM: if (a[0] < 512) s->xf_ok[a[0]] = 0; break;     /* not computed here: written again at the next diff */
  case FS_SETVIEWPORT: if (a[0] && !IsBadReadPtr((void *)(uintptr_t)a[0], 24)) { memcpy(s->vp, (void *)(uintptr_t)a[0], 24); s->vp_ok = 1; } break;
  case FS_SETMATERIAL: if (a[0] && !IsBadReadPtr((void *)(uintptr_t)a[0], 68)) { memcpy(s->mat, (void *)(uintptr_t)a[0], 68); s->mat_ok = 1; } break;
  case FS_SETLIGHT: if (a[0] < 256 && a[1] && !IsBadReadPtr((void *)(uintptr_t)a[1], 104)) { memcpy(s->light[a[0]], (void *)(uintptr_t)a[1], 104); s->light_ok[a[0]] = 1; } break;
  case FS_LIGHTENABLE: if (a[0] < 256) { s->len[a[0]] = a[1]; s->len_ok[a[0]] = 1; } else note("light index %u: past the 256 the capture reads back", a[0]); break;
  case FS_SETCLIPPLANE: if (a[0] < 6 && a[1] && !IsBadReadPtr((void *)(uintptr_t)a[1], 16)) { memcpy(s->clip[a[0]], (void *)(uintptr_t)a[1], 16); s->clip_ok[a[0]] = 1; } break;
  case FS_SETVS: s->vs = a[0]; break;
  case FS_SSS: if (a[0] < 16) { s->vb[a[0]] = a[1]; s->stride[a[0]] = a[2]; } break;
  case FS_SETINDICES: s->ib = a[0]; s->base = a[1]; break;
  case FS_SETRT: s->vp_ok = 0; break;                      /* SetRenderTarget sets the viewport to the target */
  }
}

/* ---- the picture: the back buffer at Present ----------------------------------------------------------------------- */
static void emit_picture(void) {
  void *bb = NULL; uint32_t d[8] = {0};
  if (((F_uuup)g_dvt[16])(g_dev, 0, 0, &bb) < 0 || !bb) { note("picture: GetBackBuffer failed"); return; }
  ((F_up)VT(bb)[8])(bb, d);
  uint32_t rowbytes, rows;
  if (fmt_rows(d[0], d[6], d[7], &rowbytes, &rows)) {
    rec_begin(); r32(d[0]); r32(d[6]); r32(d[7]); r32(rowbytes / (d[6] ? d[6] : 1));
    uint32_t at = g_rec_n;
    if (put_by_copy(bb, d[0], d[6], d[7], rowbytes, rows)) {
      /* put_by_copy wrote a byte count first: drop it, the header says the size */
      memmove(g_rec + at, g_rec + at + 4, g_rec_n - at - 4); g_rec_n -= 4;
      rec_end(FC_PICTURE);
    } else note("picture: the back buffer could not be read");
  } else note("picture: back buffer format %u not read", d[0]);
  ((F_u)VT(bb)[2])(bb);
}

/* ---- the frame ------------------------------------------------------------------------------------------------------ */
static void begin_frame(void) {
  char path[MAX_PATH];
  snprintf(path, sizeof path, "%s\\xikit_fcap_%lu_%u.fcap", g_dir, (unsigned long)GetCurrentProcessId(), g_count);
  g_f = fopen(path, "wb");
  if (!g_f) { g_state = 1; return; }
  setvbuf(g_f, NULL, _IOFBF, 1 << 20);
  memset(g_emit, 0, sizeof g_emit); g_nvs_emit = 0; g_calls = g_notes = 0; g_bytes_written = 0; g_inblock = 0;
  g_bb = NULL; g_autods = NULL;
  ((F_uuup)g_dvt[16])(g_dev, 0, 0, &g_bb); if (g_bb) ((F_u)VT(g_bb)[2])(g_bb);   /* identity only */
  ((F_up)g_dvt[33])(g_dev, &g_autods); if (g_autods) ((F_u)VT(g_autods)[2])(g_autods);
  FcHeader h; memset(&h, 0, sizeof h);
  h.magic = FC_MAGIC; h.version = FC_VERSION; h.game_timestamp = g_game_ts; h.frame = g_frame;
  uint32_t d[8] = {0};
  if (g_bb) { ((F_up)VT(g_bb)[8])(g_bb, d); h.bb_format = d[0]; h.bb_width = d[6]; h.bb_height = d[7]; }
  if (g_autods) { memset(d, 0, sizeof d); ((F_up)VT(g_autods)[8])(g_autods, d); h.ds_format = d[0]; h.ds_width = d[6]; h.ds_height = d[7]; }
  fwrite(&h, sizeof h, 1, g_f);
  get_state(g_cur, 1);
  emit_diff(NULL, g_cur, 1);
  g_other_at_start = fcap_other_thread_calls; g_other_writes = 0;
  g_state = 3;
}
static void end_frame(void) {
  emit_picture();
  if (g_other_writes) note("%u buffer writes from other threads to buffers this frame uses: not recorded", g_other_writes);
  if (fcap_other_thread_calls != g_other_at_start)
    note("%u device calls from other threads during this frame: not recorded", fcap_other_thread_calls - g_other_at_start);
  rec_begin(); rec_end(FC_END);
  fclose(g_f); g_f = NULL; g_count++;
  g_state = 1;
}
void fcap_arm(void) { if (fcap_enabled && g_state == 1 && g_dvt) g_state = 2; }
int fcap_busy(void) { return g_state >= 2; }
unsigned fcap_written(void) { return g_count; }

/* every device call on the render thread, before it runs (d3dprof_record); a = its arguments after `this` */
void fcap_call(int slot, void *dev, const uint32_t *a, int n) {
  if (g_state < 2) return;
  g_dev = dev; g_rt = GetCurrentThreadId();
  if (slot == FS_PRESENT) {
    g_frame++;
    if (g_state == 3) end_frame();
    else if (g_state == 2) begin_frame();
    return;
  }
  if (g_state != 3) return;
  if (slot == 14) { note("Reset during the frame: the capture stops here"); end_frame(); return; }
  if (slot == FS_BEGINSB) { g_inblock = 1; return; }
  if (slot == FS_ENDSB) { g_inblock = 0; return; }
  if (g_inblock) return;                                   /* recorded into a block, not applied */
  if (slot == FS_APPLYSB) {
    /* apply it now (it is applied again by the call itself: a block only sets values), read the state, write what changed */
    ((F_up)g_dvt[54])(dev, (void *)(uintptr_t)a[0]);
    get_state(g_new, 0); g_new->rt = g_cur->rt; g_new->ds = g_cur->ds;
    emit_diff(g_cur, g_new, 0);
    St *t = g_cur; g_cur = g_new; g_new = t;
    return;
  }
  switch (slot) {
  case FS_SETTEXTURE: ensure((void *)(uintptr_t)a[1], 0); break;
  case FS_SSS: ensure((void *)(uintptr_t)a[1], 1); break;
  case FS_SETINDICES: ensure((void *)(uintptr_t)a[0], 2); break;
  case FS_SETRT: ensure((void *)(uintptr_t)a[0], 3); ensure((void *)(uintptr_t)a[1], 3); break;
  case FS_COPYRECTS: ensure((void *)(uintptr_t)a[0], 3); ensure((void *)(uintptr_t)a[3], 3); break;
  case FS_UPDATETEXTURE: ensure((void *)(uintptr_t)a[0], 0); ensure((void *)(uintptr_t)a[1], 0); break;
  case FS_SETVS: ensure_vs(a[0]); break;
  case FS_PROCESSVERTICES: case FS_CREATEPS: note("%s during the frame: recorded, not replayed", slot == FS_PROCESSVERTICES ? "ProcessVertices" : "CreatePixelShader"); break;
  case FS_SETPS: if (a[0]) note("SetPixelShader(%08X) during the frame: pixel shaders are not replayed", a[0]); break;
  }
  switch (slot) {
  case FS_COPYRECTS: case FS_UPDATETEXTURE: case FS_SETRT: case FS_BEGINSCENE: case FS_ENDSCENE: case FS_CLEAR:
  case FS_SETTRANSFORM: case FS_MULTIPLYTRANSFORM: case FS_SETVIEWPORT: case FS_SETMATERIAL: case FS_SETLIGHT:
  case FS_LIGHTENABLE: case FS_SETCLIPPLANE: case FS_SETRS: case FS_SETCLIPSTATUS: case FS_SETTEXTURE: case FS_SETTSS:
  case FS_SETPALETTE: case FS_SETCURPALETTE: case FS_DP: case FS_DIP: case FS_DPUP: case FS_DIPUP: case FS_PROCESSVERTICES:
  case FS_SETVS: case FS_SETVSCONST: case FS_SSS: case FS_SETINDICES: case FS_SETPS: case FS_SETPSCONST:
    emit_call(slot, 0, a, n); track(slot, a);
  }
  if (slot == FS_DPUP || slot == FS_DIPUP) {               /* D3D8: after a ...UP draw, stream 0 and (indexed) the indices are unset */
    g_cur->vb[0] = 0; g_cur->stride[0] = 0;
    if (slot == FS_DIPUP) g_cur->ib = 0;
  }
}

/* Not recorded (version 1): a texture written through LockRect after the frame first used it (its contents are read
   once, at first use). The kit does not wrap texture locks. */
void fcap_attach(void **dev_vt, const char *dir, uint32_t game_ts) {
  if (!g_lock_init) { InitializeCriticalSection(&g_lock); g_lock_init = 1; }
  g_dvt = dev_vt; g_game_ts = game_ts;
  snprintf(g_dir, sizeof g_dir, "%s", dir);
  if (!fcap_enabled) return;
  if (!g_buf) g_buf = (Buf *)calloc(NBUF, sizeof(Buf));
  if (!g_cur) g_cur = (St *)calloc(1, sizeof(St));
  if (!g_new) g_new = (St *)calloc(1, sizeof(St));
  if (!g_buf || !g_cur || !g_new) { fcap_enabled = 0; return; }
  g_state = 1;
}
