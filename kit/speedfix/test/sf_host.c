/* sf_host.c -- speedfix (kit/speedfix) under Wine with the real FFXiMain.dll and the kit (test_speedfix.py runs it).
   Like kit/xikit/test/host.c: the game DLL is mapped without running it, the game gets a fake CDx whose device
   records what reaches it, xikit.dll loads speedfix.dll as its mod, and this program plays the render thread. Every
   check prints "ok: ..." or "FAIL: ...". */
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define G_PCDX 0x1045666Cu
static uint32_t rel;
#define GAME(a) ((uintptr_t)(a) + rel)
static int fails;
static void check(int c, const char *what) { printf("%s: %s\n", c ? "ok" : "FAIL", what); if (!c) fails++; }

/* ---- the fake device: what reaches it is recorded ---- */
typedef struct { int slot; uint32_t a[4]; int nbytes; uint8_t bytes[256]; } Call;
static Call g_log[4096]; static volatile LONG g_n;
static void rec(int slot, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, const void *data, int n) {
  LONG i = InterlockedIncrement(&g_n) - 1; if (i >= 4096) return;
  Call *c = &g_log[i]; c->slot = slot; c->a[0] = a0; c->a[1] = a1; c->a[2] = a2; c->a[3] = a3;
  c->nbytes = n > 256 ? 256 : n; if (data && n > 0) memcpy(c->bytes, data, c->nbytes);
}
static DWORD g_rs[256];
static HRESULT __stdcall d_present(void *d, const RECT *a, const RECT *b, HWND w, const void *r) { rec(15, 0, 0, 0, 0, 0, 0); return 0; }
static HRESULT __stdcall d_caps(void *d, void *caps) { return 0x8876086C; }       /* D3DERR_INVALIDCALL: speedfix keeps 65535 */
static HRESULT __stdcall d_begin(void *d) { rec(34, 0, 0, 0, 0, 0, 0); return 0; }
static HRESULT __stdcall d_end(void *d) { rec(35, 0, 0, 0, 0, 0, 0); return 0; }
static HRESULT __stdcall d_setrs(void *d, DWORD s, DWORD v) { rec(50, s, v, 0, 0, 0, 0); if (s < 256) g_rs[s] = v; return 0; }
static HRESULT __stdcall d_getrs(void *d, DWORD s, DWORD *v) { rec(51, s, 0, 0, 0, 0, 0); *v = s < 256 ? g_rs[s] : 0; return 0; }
static HRESULT __stdcall d_settex(void *d, DWORD st, void *t) { rec(61, st, (uint32_t)(uintptr_t)t, 0, 0, 0, 0); return 0; }
static HRESULT __stdcall d_settss(void *d, DWORD st, DWORD ty, DWORD v) { rec(63, st, ty, v, 0, 0, 0); return 0; }
static HRESULT __stdcall d_setvs(void *d, DWORD h) { rec(76, h, 0, 0, 0, 0, 0); return 0; }
static HRESULT __stdcall d_dpup(void *d, DWORD t, UINT n, const void *data, UINT stride) {
  int v = t == 4 ? (int)n * 3 : (int)n + 2;
  rec(72, t, n, stride, 0, data, v * (int)stride); return 0;
}
static HRESULT __stdcall d_dp(void *d, DWORD t, UINT start, UINT n) { rec(70, t, start, n, 0, 0, 0); return 0; }
static HRESULT __stdcall d_dip(void *d, DWORD t, UINT mi, UINT nv, UINT si, UINT n) { rec(71, t, mi, nv, n, 0, 0); return 0; }
static HRESULT __stdcall d_dipup(void *d, DWORD t, UINT mi, UINT nv, UINT n, const void *ix, DWORD f, const void *v, UINT st) {
  rec(73, t, nv, n, st, 0, 0); return 0; }
static HRESULT __stdcall d_clear(void *d, DWORD n, const void *r, DWORD f, DWORD c, float z, DWORD s) { rec(36, n, f, c, 0, 0, 0); return 0; }
static DWORD g_nvs = 0x101;                      /* shader handles: odd, as D3D8's are (bit 0 tells them from FVFs) */
static HRESULT __stdcall d_createvs(void *d, const DWORD *decl, const DWORD *fn, DWORD *h, DWORD us) {
  *h = g_nvs; g_nvs += 2; rec(75, *h, (uint32_t)(uintptr_t)fn, us, 0, 0, 0); return 0; }
static HRESULT __stdcall d_deletevs(void *d, DWORD h) { rec(78, h, 0, 0, 0, 0, 0); return 0; }
/* fake 16x16 surfaces for the occlusion readback (occlusion_late): CopyRects copies their pixels, LockRect gives them */
typedef struct { void **vt; uint32_t px[256]; DWORD pool; LONG refs; } FSurf;
static void *g_svt[11];
static FSurf g_boxSurf, g_gameSurf;             /* the box texture's level 0; the game's lockable target at [mng+0x18] */
static LONG g_nRT, g_nSurfRelease, g_nLock;
static ULONG __stdcall s_addref(FSurf *s) { return ++s->refs; }
static ULONG __stdcall s_release(FSurf *s) { if (s != &g_boxSurf && s != &g_gameSurf) g_nSurfRelease++; return --s->refs; }
static HRESULT __stdcall s_getdesc(FSurf *s, uint32_t *d) { memset(d, 0, 32); d[0] = 21; d[1] = 1; d[3] = s->pool; d[6] = 16; d[7] = 16; return 0; }
typedef struct { INT Pitch; void *pBits; } FLock;
static HRESULT __stdcall s_lock(FSurf *s, FLock *lr, const RECT *r, DWORD f) { g_nLock++; lr->Pitch = 64; lr->pBits = s->px; return 0; }
static HRESULT __stdcall s_unlock(FSurf *s) { return 0; }
static FSurf *new_surf(DWORD pool) { FSurf *s = (FSurf *)calloc(1, sizeof *s); s->vt = g_svt; s->pool = pool; s->refs = 1; return s; }
static HRESULT __stdcall d_creatert(void *d, UINT w, UINT h, DWORD f, DWORD ms, BOOL lk, FSurf **o) { g_nRT++; *o = new_surf(0); return 0; }
static HRESULT __stdcall d_reset(void *d, void *pp) { rec(14, 0, 0, 0, 0, 0, 0); return 0; }
static HRESULT __stdcall d_copyrects(void *d, void *s, const void *r, UINT n, void *t, const void *p) {
  rec(28, 0, 0, 0, 0, 0, 0);
  if (s && t && *(void ***)s == g_svt && *(void ***)t == g_svt) memcpy(((FSurf *)t)->px, ((FSurf *)s)->px, 1024);
  return 0;
}
static HRESULT __stdcall x_getsurflevel(void *t, UINT l, FSurf **o) { g_boxSurf.refs++; *o = &g_boxSurf; return 0; }
static void *g_xvt[17]; static struct { void **vt; } g_boxTex = { g_xvt };
static uint8_t g_mng[0x40], g_modx[0xF00];
static void picture(int kind) {                 /* 0: nothing drawn, 1: all drawn, 2: the top half drawn */
  for (int i = 0; i < 256; i++) g_boxSurf.px[i] = kind == 1 || (kind == 2 && i < 128) ? 0xFFFFFFFFu : 0xFF000000u;
}
/* state blocks: tokens counted up; what reaches the device is recorded like any other call */
static DWORD g_ntok = 1;
static HRESULT __stdcall d_beginsb(void *d) { rec(52, 0, 0, 0, 0, 0, 0); return 0; }
static HRESULT __stdcall d_endsb(void *d, DWORD *t) { *t = g_ntok++; rec(53, *t, 0, 0, 0, 0, 0); return 0; }
static HRESULT __stdcall d_applysb(void *d, DWORD t) { rec(54, t, 0, 0, 0, 0, 0); return 0; }
static HRESULT __stdcall d_capturesb(void *d, DWORD t) { rec(55, t, 0, 0, 0, 0, 0); return 0; }
static HRESULT __stdcall d_deletesb(void *d, DWORD t) { rec(56, t, 0, 0, 0, 0, 0); return 0; }
static HRESULT __stdcall d_createsb(void *d, DWORD ty, DWORD *t) { *t = g_ntok++; rec(57, *t, ty, 0, 0, 0, 0); return 0; }
static HRESULT __stdcall d_settransform(void *d, DWORD st, const float *m) { rec(37, st, 0, 0, 0, 0, 0); return 0; }
static void __stdcall d_other(void) { printf("FAIL: a device method the test does not provide was called\n"); ExitProcess(3); }
static void *g_vt[97];
static struct { void **vt; } g_dev = { g_vt };
static uint8_t g_cdx[0x1000];
/* a texture the game binds: with batching on speedfix asks its type (IDirect3DBaseTexture8::GetType, slot 10) to
   guard its LockRect; this one answers D3DRTYPE_SURFACE (1), so no vtable of it is changed */
static DWORD __stdcall t_gettype(void *t) { return 1; }
static LONG g_addref, g_release;                 /* the texture's references taken and given back (lazy_state holds one) */
static ULONG __stdcall t_addref(void *t) { return ++g_addref; }
static ULONG __stdcall t_release(void *t) { return ++g_release; }
static void *g_tvt[16]; static struct { void **vt; } g_tex1 = { g_tvt }, g_tex2 = { g_tvt };

#define DEV(slot, T) ((T)g_dev.vt[slot])
static void present(void) { DEV(15, HRESULT (__stdcall *)(void *, void *, void *, void *, void *))(&g_dev, 0, 0, 0, 0); }
static void begin(void) { DEV(34, HRESULT (__stdcall *)(void *))(&g_dev); }
static void end(void) { DEV(35, HRESULT (__stdcall *)(void *))(&g_dev); }
static void setrs(DWORD s, DWORD v) { DEV(50, HRESULT (__stdcall *)(void *, DWORD, DWORD))(&g_dev, s, v); }
static void settss(DWORD st, DWORD ty, DWORD v) { DEV(63, HRESULT (__stdcall *)(void *, DWORD, DWORD, DWORD))(&g_dev, st, ty, v); }
static void settex(DWORD st, void *t) { DEV(61, HRESULT (__stdcall *)(void *, DWORD, void *))(&g_dev, st, t); }
static void setvs(DWORD h) { DEV(76, HRESULT (__stdcall *)(void *, DWORD))(&g_dev, h); }
static void clear(void) { DEV(36, HRESULT (__stdcall *)(void *, DWORD, const void *, DWORD, DWORD, float, DWORD))(&g_dev, 0, 0, 1, 0, 1.0f, 0); }
static void getrs(DWORD s, DWORD *v) { DEV(51, HRESULT (__stdcall *)(void *, DWORD, DWORD *))(&g_dev, s, v); }
static DWORD createvs(const DWORD *decl, const DWORD *fn) { DWORD h = 0; DEV(75, HRESULT (__stdcall *)(void *, const DWORD *, const DWORD *, DWORD *, DWORD))(&g_dev, decl, fn, &h, 0); return h; }
static void deletevs(DWORD h) { DEV(78, HRESULT (__stdcall *)(void *, DWORD))(&g_dev, h); }
static void dpup(DWORD t, UINT n, const void *data, UINT stride) { DEV(72, HRESULT (__stdcall *)(void *, DWORD, UINT, const void *, UINT))(&g_dev, t, n, data, stride); }
static void dp(UINT n) { DEV(70, HRESULT (__stdcall *)(void *, DWORD, UINT, UINT))(&g_dev, 4, 0, n); }
static void dip(UINT n) { DEV(71, HRESULT (__stdcall *)(void *, DWORD, UINT, UINT, UINT, UINT))(&g_dev, 4, 0, 3, 0, n); }
static void dipup(const void *ix, const void *v) {
  DEV(73, HRESULT (__stdcall *)(void *, DWORD, UINT, UINT, UINT, const void *, DWORD, const void *, UINT))(&g_dev, 4, 0, 3, 1, ix, 101 /* D3DFMT_INDEX16 */, v, 16); }
static void beginsb(void) { DEV(52, HRESULT (__stdcall *)(void *))(&g_dev); }
static DWORD endsb(void) { DWORD t = 0; DEV(53, HRESULT (__stdcall *)(void *, DWORD *))(&g_dev, &t); return t; }
static void applysb(DWORD t) { DEV(54, HRESULT (__stdcall *)(void *, DWORD))(&g_dev, t); }
static void capturesb(DWORD t) { DEV(55, HRESULT (__stdcall *)(void *, DWORD))(&g_dev, t); }
static void deletesb(DWORD t) { DEV(56, HRESULT (__stdcall *)(void *, DWORD))(&g_dev, t); }
static DWORD createsb(DWORD ty) { DWORD t = 0; DEV(57, HRESULT (__stdcall *)(void *, DWORD, DWORD *))(&g_dev, ty, &t); return t; }
static void frames(int k) { for (int i = 0; i < k; i++) { begin(); end(); present(); } }
static void copyrects(void) { DEV(28, HRESULT (__stdcall *)(void *, void *, const void *, UINT, void *, const void *))(&g_dev, 0, 0, 0, 0, 0); }
static void setview(float x) { float m[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, x,0,0,1}; DEV(37, HRESULT (__stdcall *)(void *, DWORD, const float *))(&g_dev, 2, m); }
static int count(int slot, LONG from) { int c = 0; for (LONG i = from; i < g_n; i++) c += g_log[i].slot == slot; return c; }
/* the device calls since `from`, as slot numbers, e.g. "50 72 72 35" */
static const char *seq(LONG from) { static char b[512]; int o = 0; b[0] = 0;
  for (LONG i = from; i < g_n && o < 500; i++) o += sprintf(b + o, o ? " %d" : "%d", g_log[i].slot); return b; }
static Call *last(int slot) { for (LONG i = g_n - 1; i >= 0; i--) if (g_log[i].slot == slot) return &g_log[i]; return NULL; }
static int log_has(const char *what) {
  FILE *f = fopen("xikit.log", "rb"); if (!f) return 0;
  static char b[1 << 20]; size_t n = fread(b, 1, sizeof b - 1, f); fclose(f); b[n] = 0;
  return strstr(b, what) != NULL;
}
/* how many "speedfix: readback time:" lines xikit.log has; the last one's frames with occlusion_late on and frames with
   every readback skipped go to *on and *skip */
static int rb_lines(long *on, long *skip) {
  FILE *f = fopen("xikit.log", "rb"); if (!f) return 0;
  static char b[1 << 20]; size_t n = fread(b, 1, sizeof b - 1, f); fclose(f); b[n] = 0;
  int c = 0; char *l = NULL, *p = b; while ((p = strstr(p, "speedfix: readback time:"))) { l = p++; c++; }
  if (l && (l = strstr(l, "with it on ("))) sscanf(l, "with it on (%ld frames); %ld frames with every readback skipped", on, skip);
  return c;
}
static void touch(const char *path, const char *body) { Sleep(30); FILE *f = fopen(path, "w"); fputs(body, f); fclose(f); }

/* the stagger's call: ESI = the actor (0x100CC584 `mov edi, [esi+0x9F0]`) */
extern int call_with_esi(void *fn, void *esi);
__asm__(".globl _call_with_esi\n_call_with_esi:\n  push %esi\n  mov 12(%esp), %esi\n  call *8(%esp)\n  pop %esi\n  ret\n");
static void *call_target(uint32_t site) { uint8_t *p = (uint8_t *)GAME(site); return p[0] == 0xE8 ? p + 5 + *(int32_t *)(p + 1) : NULL; }
static uint8_t g_cpu[0x24];        /* [0x104566E8]: the game's CPU object (0x1000D380); +9 its SSE flag (sse_skinning) */
static uint8_t g_cam[0x40];        /* [0x104568FC] -> +0x34: the frame counter (GetFrameCounter, 0x10014D50) */

static DWORD WINAPI other_thread(LPVOID u) { setrs(7, 1); return 0; }

/* precise_pacing: the game's own limiter code, 0x10012B05 to the lap call at 0x10012BF5, run on a limiter object of the
   test's (esi; [esi+0x1C] a timer with the game's vtable 0x1032A118). The DLL is mapped without its imports, so the
   two the code reaches are filled in (timeGetTime 0x10329400, Sleep 0x103290F8). The lap (slot 8, 0x1032A138) goes
   to lim_lap, which runs the game's lap (0x100194F0) and returns to the test instead of to 0x10012BF8. */
static uint8_t g_lim[0x40], g_tim[0x20];
uint32_t g_lim_sp, g_lim_entry, g_lap_orig;
extern void run_limiter(void *obj);
extern void lim_lap(void);
__asm__(".globl _run_limiter\n_run_limiter:\n  push %ebp\n  push %ebx\n  push %esi\n  push %edi\n  mov 20(%esp), %esi\n"
        "  mov %esp, _g_lim_sp\n  sub $0x40, %esp\n  xor %ebx, %ebx\n  jmp *_g_lim_entry\n"
        ".globl _lim_lap\n_lim_lap:\n  call *_g_lap_orig\n  mov _g_lim_sp, %esp\n  pop %edi\n  pop %esi\n  pop %ebx\n  pop %ebp\n  ret\n");
static LARGE_INTEGER g_qf;
static double qms(void) { LARGE_INTEGER t; QueryPerformanceCounter(&t); return t.QuadPart * 1000.0 / (double)g_qf.QuadPart; }
static int cmp_dbl(const void *a, const void *b) { double x = *(const double *)a, y = *(const double *)b; return x < y ? -1 : x > y; }
/* k frames of the limiter, each after a frame through the hooks (scene) or not; the periods between the limiter's
   returns, their median and mean, and the game's ticks over them ([esi+0x38] each frame, the fraction [esi+0x3C]).
   g_whole: the frames after the first whose time, as the rate at 0x10012BCE stored it ([t+4] = 1000 / ms), is a whole
   number of ms, 17 or more: the game's own timing (timeGetTime) and wait */
static int g_whole;
static double lim_frames(int k, int scene, double *mean, double *ticks, double *span) {
  static double d[512]; double t0 = qms(), prev = t0, sum = 0; float f0 = *(float *)(g_lim + 0x3C); int tk = 0; g_whole = 0;
  for (int i = 0; i < k && i < 512; i++) {
    if (scene) { begin(); end(); }
    present(); run_limiter(g_lim);
    double t = qms(); d[i] = t - prev; prev = t; sum += d[i]; tk += *(int *)(g_lim + 0x38);
    double e = 1000.0 / *(float *)(g_tim + 4); if (i && e > 16.5 && fabs(e - floor(e + 0.5)) < 1e-3) g_whole++;
  }
  *mean = sum / k; *ticks = tk + *(float *)(g_lim + 0x3C) - f0; *span = prev - t0;
  qsort(d, k, sizeof d[0], cmp_dbl); return d[k / 2];
}
static int pacing_test(void) {
  QueryPerformanceFrequency(&g_qf);
  uint8_t *code = (uint8_t *)GAME(0x10012B05u);
  void **vt = (void **)GAME(0x1032A118u);
  check(vt[9] != (void *)GAME(0x10019550u), "precise_pacing=1 at the start: the timer's rate entry (1032A13C) no longer holds 10019550");
  check(code[0] == 0xBD && code[5] == 0x90, "... and 10012B05 is mov ebp, imm32; nop (the wait thunk) instead of mov ebp, [Sleep]");
  check(log_has("precise_pacing: the frame limiter's wait (10012B05) and its timer's rate entry (1032A13C) now go to speedfix"), "... logged");
  /* another caller of the rate (this test, by its return address) gets the game's function: 1000 / whole ms */
  { typedef float (__attribute__((thiscall)) *Rate)(void *); *(int *)(g_tim + 0xC) = (int)timeGetTime() - 40;
    DWORD a = timeGetTime(); float r = ((Rate)vt[9])(g_tim); DWORD b = timeGetTime();
    int lo = (int)(a - *(DWORD *)(g_tim + 0xC)), hi = (int)(b - *(DWORD *)(g_tim + 0xC)); int ok = 0;
    for (int e = lo; e <= hi; e++) ok |= r == (float)(1000.0 / e) && *(float *)(g_tim + 4) == r;
    check(ok, "another caller of the timer's rate runs the game's function (1000 / whole ms since the lap, stored at +4)"); }
  double mean, ticks, span, med;
  lim_frames(10, 1, &mean, &ticks, &span);           /* the counter's first base */
  med = lim_frames(240, 1, &mean, &ticks, &span);
  printf("    on: median %.3f ms, mean %.3f ms; game ticks %.3f over %.1f ms (%.3f by the counter)\n", med, mean, ticks, span, span * 0.06);
  check(fabs(med - 1000.0 / 60) < 0.05 && fabs(mean - 1000.0 / 60) < 0.3, "precise_pacing=1: the game's limiter paces at 16.667 ms (median within 0.05, mean within 0.3)");
  check(fabs(ticks - span * 0.06) < 0.1, "precise_pacing=1: the game's ticks add up to the counter's time (within 0.1 tick over 240 frames)");
  check(g_whole < 5, "precise_pacing=1: the frames' times are the counter's, not whole ms (fewer than 5 of 240 whole by chance)");
  med = lim_frames(12, 0, &mean, &ticks, &span);
  printf("    on, frames without the hooks: median %.3f ms; %d of 11 timed in whole ms\n", med, g_whole);
  check(g_whole == 11, "precise_pacing=1, frames without the hooks (after the first, whose frame before went through them): the game's own whole-ms timing and wait");
  touch("speedfix.ini", "[speedfix]\nprecise_pacing=0\nstats_every=1\n");
  lim_frames(70, 1, &mean, &ticks, &span);
  med = lim_frames(120, 1, &mean, &ticks, &span);
  printf("    off: median %.3f ms, mean %.3f ms; game ticks %.3f over %.1f ms (%.3f by the counter)\n", med, mean, ticks, span, span * 0.06);
  check(g_whole == 119 && mean > 16.9, "precise_pacing=0 again: the game's own timing, every frame whole ms and 17 or more (58.8 fps)");
  check(fabs(ticks - span * 0.06) < 1.0, "precise_pacing=0: the game's ticks still add up to the time (within a tick)");
  check(log_has("precise_pacing: ") && log_has(" frames timed by the counter"), "the statistics line counts the frames the counter timed");
  check(log_has("its tick count [+0x30] 1, its timer's scale [+8] 1.0000"), "... and names the limiter's tick count and its timer's scale (this host's: 1, 1.0)");
  printf(fails ? "FAILED (%d)\n" : "ALL OK\n", fails);
  return fails ? 1 : 0;
}

/* uncap and frame_step: speedfix's forks (Frame_Render, the getters 0x10014CF0 and 0x10014D20; fork/*.fork.cpp, gated
   against the game's functions) are installed at the start: their entries jump into speedfix.dll. Frame_Render as a
   whole does not run here; the game's own frame code from the wait (0x10012B05) through the step and the frame counter
   (0x10012CB0) does, on a frame object of the test's that is also [g_pCamera], and the test then gives the game's ring
   average to the fork's step (xi_step_make, what the fork's Frame_Render calls there). The getters are called from the
   test (whole ticks) and from the call at 0x10227E58 (a call given the fraction: a ret written after it). The fork's
   wait with uncap (the floor read as 0) is not run here (the gate's); for short frames the test writes the floor 0
   itself, as that wait would read it. */
extern void lim_end(void);
__asm__(".globl _lim_end\n_lim_end:\n  mov _g_lim_sp, %esp\n  pop %edi\n  pop %esi\n  pop %ebx\n  pop %ebp\n  ret\n");
typedef float (*Getter)(void);
static float ring_raw(void) {          /* 0x10012C19..0x10012C39: the sum in floats, then * 0.25 */
  float a = 0.0f; const float *r = (const float *)GAME(0x10456964u);
  for (int i = 0; i < 4; i++) { volatile float t = r[i] + a; a = t; }
  return a * 0.25f;
}
static float ring_avg(int fl) {        /* floor and cap as 0x10012C4C.. */
  float f = ring_raw();
  if (!(f >= (float)fl)) f = (float)fl;
  return f > 20.0f ? 20.0f : f;
}
typedef void (*StepMake)(void *, double);
static StepMake g_step_make; static volatile int *g_xi_uncap, *g_xi_mode;
typedef struct { double whole, frac, game_ticks, span; int n, zero, nonint, wrong_frac, game_like; } StepRun;
static StepRun step_frames(int k, int scene, int nap) {
  StepRun r = {0}; Getter g0 = (Getter)GAME(0x10014CF0u), g1 = (Getter)GAME(0x10014D20u), site = (Getter)GAME(0x10227E58u);
  float f0 = *(float *)(g_lim + 0x3C); double t0 = qms();
  for (int i = 0; i < k; i++) {
    if (scene) { begin(); end(); }
    present(); if (nap) Sleep(nap); run_limiter(g_lim);
    if (*g_xi_mode) g_step_make(g_lim, ring_raw());
    float w = g0(), w1 = g1(), fr = site();
    float game = *(float *)(g_lim + 0x28) < 1.0f ? 1.0f : *(float *)(g_lim + 0x28);
    r.whole += w; r.frac += fr; r.game_ticks += *(int *)(g_lim + 0x38); r.n++;
    if (w == 0.0f) r.zero++;
    if (w != floorf(w) || w1 != w) r.nonint++;
    if (fr != ring_avg(*g_xi_uncap ? 0 : *(int *)(g_lim + 0x30))) r.wrong_frac++;
    if (w == game && w1 == game && fr == game) r.game_like++;
  }
  r.game_ticks += *(float *)(g_lim + 0x3C) - f0; r.span = qms() - t0;
  return r;
}
static void step_print(const char *what, StepRun r) {
  printf("    %s: %d frames, %.1f ms (%.2f ticks by the counter, %.2f the game's [+0x38]); whole %.0f (%d of 0), fraction %.2f; "
         "%d frames as the game's\n", what, r.n, r.span, r.span * 0.06, r.game_ticks, r.whole, r.zero, r.frac, r.game_like);
}
/* a call of 0x10014CF0 as its callers make it, with EAX and EDX holding known values: what it leaves in EAX, ECX, EDX */
typedef struct { uint32_t eax, ecx, edx; float st0; } Regs;
static Regs call_cf0(uint32_t eax_in, uint32_t edx_in) {
  Regs r; uint32_t a = eax_in, c, d = edx_in; float v; void *fn = (void *)GAME(0x10014CF0u);
  __asm__ volatile("call *%[f]" : "+a"(a), "=c"(c), "+d"(d), "=t"(v) : [f] "S"(fn) : "memory", "cc", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)");
  r.eax = a; r.ecx = c; r.edx = d; r.st0 = v; return r;
}
static int jumps_into(uint32_t ea, HMODULE m) {
  uint8_t *e = (uint8_t *)GAME(ea); HMODULE h = NULL;
  void *to = e[0] == 0xE9 ? e + 5 + *(int32_t *)(e + 1) : NULL;
  if (to) GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)to, &h);
  return h && h == m;
}
static int step_test(void) {
  QueryPerformanceFrequency(&g_qf);
  HMODULE sf = GetModuleHandleA("speedfix.dll");
  check(jumps_into(0x10011A00u, sf) && jumps_into(0x10014CF0u, sf) && jumps_into(0x10014D20u, sf),
        "frame_step=2 at the start: Frame_Render and the getters 10014CF0, 10014D20 jump into speedfix.dll (the forks)");
  check(*(uint8_t *)GAME(0x10012C3Cu) == 0xE8 && (uint8_t *)GAME(0x10012C3Cu) + 5 + *(int32_t *)GAME(0x10012C3Du) == (uint8_t *)GAME(0x10311C2Cu),
        "... and the game's own code is not patched (the call of _ftol at 10012C3C is the game's)");
  check(log_has("frame_step / uncap: Frame_Render (0x10011A00), Camera_GetFrameScale (0x10014CF0) and Camera_GetClampedField28 (0x10014D20) run speedfix's gated forks"), "... logged");
  g_step_make = (StepMake)GetProcAddress(sf, "xi_step_make"); g_xi_uncap = (volatile int *)GetProcAddress(sf, "xi_uncap"); g_xi_mode = (volatile int *)GetProcAddress(sf, "xi_step_mode");
  check(g_step_make && g_xi_uncap && g_xi_mode, "the fork's step (xi_step_make) and its switches are found");
  if (!g_step_make || !g_xi_uncap || !g_xi_mode) { printf("FAILED (%d)\n", fails); return 1; }
  *(uint8_t *)GAME(0x10227E5Du) = 0xC3;
  step_frames(10, 1, 0);
  check(*g_xi_mode == 2 && *g_xi_uncap == 0, "frame_step=2 uncap=0: the switches speedfix sets each frame");
  StepRun r = step_frames(120, 1, 0);
  step_print("frame_step=2, uncap=0 (the game's 60 fps wait)", r);
  check(r.nonint == 0, "frame_step=2: the getters answer whole ticks, the same");
  check(r.wrong_frac == 0, "frame_step=2: the call at 10227E58 gets the game's average of its last 4 frames, not truncated (floor and cap as the game's)");
  check(fabs(r.whole - r.game_ticks) < 3.0 && fabs(r.frac - r.game_ticks) < 3.0, "frame_step=2: whole ticks and the fraction add up to the game's own ticks [+0x38] (within 3 over 120 frames)");
  { /* 0x10014CF0 hands bytes 2-3 of its entry EAX back to callers that read them (manifest flag 8): through speedfix's
       wrapper, EAX, ECX and EDX are the game's code's, switches on as off; only ST0 is the fork's answer */
    int m = *g_xi_mode;
    *g_xi_mode = 0; Regs off = call_cf0(0x5A5A1234u, 0x77665544u);
    *g_xi_mode = m; Regs on = call_cf0(0x5A5A1234u, 0x77665544u);
    printf("    0x10014CF0 with EAX 5A5A1234, EDX 77665544: off EAX %08X ECX %08X EDX %08X ST0 %g; on EAX %08X ECX %08X EDX %08X ST0 %g\n",
           off.eax, off.ecx, off.edx, off.st0, on.eax, on.ecx, on.edx, on.st0);
    check((off.eax >> 16) == 0x5A5A && off.ecx == *(uint32_t *)GAME(0x104568FCu) && off.edx == 0x77665544u,
          "0x10014CF0, switches off: the game's code keeps EAX bytes 2-3 and EDX, and leaves ECX = [g_pCamera]");
    check(on.eax == off.eax && on.ecx == off.ecx && on.edx == off.edx,
          "0x10014CF0, switches on: EAX, ECX and EDX as the game's code leaves them (hook_keeping_registers)");
  }
  touch("speedfix.ini", "[speedfix]\nuncap=1\nframe_step=1\nstats_every=1\n");
  step_frames(70, 1, 3);
  check(*g_xi_uncap == 1 && *g_xi_mode == 1, "uncap=1 frame_step=1 (changed while running): the switches follow");
  check(*(int *)(g_lim + 0x30) == 1, "uncap=1: the game's tick count [+0x30] is not written (the fork reads its floor as 0)");
  *(int *)(g_lim + 0x30) = 0;                         /* the floor the fork's wait reads under uncap (see above) */
  r = step_frames(200, 1, 3);
  step_print("frame_step=1, floor 0, 3 ms frames", r);
  check(r.span < 200 * 9.0, "with the floor 0, frames shorter than 1/60 s");
  check(r.nonint == 0 && r.zero > 20, "frame_step=1: whole ticks, frames of 0 among them");
  check(fabs(r.whole - r.game_ticks) < 3.0, "frame_step=1: the whole ticks add up to the game's own ticks [+0x38] (within 3 over 200 frames)");
  check(r.frac == r.whole, "frame_step=1: the call at 10227E58 gets whole ticks too");
  touch("speedfix.ini", "[speedfix]\nuncap=1\nframe_step=0\nstats_every=1\n");
  step_frames(70, 1, 3);
  r = step_frames(100, 1, 3);
  step_print("frame_step=0, floor 0 (the game's step with no wait)", r);
  check(r.game_like == 100 && r.whole == 100, "frame_step=0: the game's answer, one tick every frame");
  check(log_has("uncap=1 with frame_step=0: the game advances one tick of 1/60 s every frame"), "... and the log says so");
  *(int *)(g_lim + 0x30) = 1;
  touch("speedfix.ini", "[speedfix]\nuncap=0\nframe_step=0\nstats_every=1\n");
  step_frames(70, 1, 0);
  check(*g_xi_uncap == 0 && *g_xi_mode == 0, "uncap=0 frame_step=0: both switches off (the forks are the game's functions)");
  check(log_has("speedfix: frame_step=1 uncap=1: ") && log_has("(what frame_step 1 and 2 answer) added up to: whole "), "the statistics line compares the step's sum with the counter's time");
  printf(fails ? "FAILED (%d)\n" : "ALL OK\n", fails);
  return fails ? 1 : 0;
}

int main(int argc, char **argv) {
  setvbuf(stdout, NULL, _IONBF, 0);
  int minimal = argc > 1 && !strcmp(argv[1], "minimal"), lazy = argc > 1 && !strcmp(argv[1], "lazy"), blocks = argc > 1 && !strcmp(argv[1], "blocks");
  int pacing = argc > 1 && !strcmp(argv[1], "pacing"), step = argc > 1 && !strcmp(argv[1], "step");
  HMODULE g = LoadLibraryExA("FFXiMain.dll", NULL, DONT_RESOLVE_DLL_REFERENCES);
  if (!g) { printf("FAIL: FFXiMain.dll not mapped\n"); return 2; }
  rel = (uint32_t)(uintptr_t)g - 0x10000000u;
  if (rel) { printf("FAIL: the test needs the game DLL at its analysed base\n"); return 2; }
  for (int k = 0; k < 97; k++) g_vt[k] = (void *)d_other;
  g_vt[7] = (void *)d_caps; g_vt[15] = (void *)d_present; g_vt[34] = (void *)d_begin; g_vt[35] = (void *)d_end; g_vt[36] = (void *)d_clear;
  g_vt[28] = (void *)d_copyrects; g_vt[37] = (void *)d_settransform;
  g_vt[14] = (void *)d_reset; g_vt[25] = (void *)d_creatert;
  for (int k = 0; k < 11; k++) g_svt[k] = (void *)d_other;
  g_svt[1] = (void *)s_addref; g_svt[2] = (void *)s_release; g_svt[8] = (void *)s_getdesc; g_svt[9] = (void *)s_lock; g_svt[10] = (void *)s_unlock;
  for (int k = 0; k < 17; k++) g_xvt[k] = (void *)d_other;
  g_xvt[15] = (void *)x_getsurflevel;
  g_boxSurf.vt = g_gameSurf.vt = g_svt; g_boxSurf.refs = g_gameSurf.refs = 1;
  *(void **)(g_mng + 0xC) = &g_boxTex; *(void **)(g_mng + 0x18) = &g_gameSurf;
  *(void **)(g_modx + 0xEBC) = &g_dev;
  g_vt[50] = (void *)d_setrs; g_vt[51] = (void *)d_getrs; g_vt[61] = (void *)d_settex; g_vt[63] = (void *)d_settss;
  g_vt[52] = (void *)d_beginsb; g_vt[53] = (void *)d_endsb; g_vt[54] = (void *)d_applysb; g_vt[55] = (void *)d_capturesb;
  g_vt[56] = (void *)d_deletesb; g_vt[57] = (void *)d_createsb;
  g_vt[70] = (void *)d_dp; g_vt[71] = (void *)d_dip; g_vt[73] = (void *)d_dipup; g_vt[72] = (void *)d_dpup; g_vt[76] = (void *)d_setvs; g_vt[75] = (void *)d_createvs; g_vt[78] = (void *)d_deletevs;
  for (int k = 0; k < 16; k++) g_tvt[k] = (void *)d_other;
  g_tvt[10] = (void *)t_gettype; g_tvt[1] = (void *)t_addref; g_tvt[2] = (void *)t_release;
  g_rs[8] = 3; g_rs[9] = 2;                        /* the device's defaults: FILL_SOLID, SHADE_GOURAUD */
  *(void **)(g_cdx + 0xC) = &g_dev;
  DWORD old; VirtualProtect((void *)GAME(0x10000000u), 0x1000000, PAGE_EXECUTE_READWRITE, &old);
  *(void **)GAME(G_PCDX) = g_cdx;
  *(void **)GAME(0x104568FCu) = g_cam;
  *(void **)GAME(0x104566E8u) = g_cpu;                /* +9 = 0: the game found no SSE (as on the owner's machine) */
  uint32_t tex_before = *(uint32_t *)GAME(0x10456AD0u), tier_before = *(uint32_t *)GAME(0x1006CDDCu);
  if (pacing || step) {                               /* pacing_test: the limiter's imports, its objects, its lap */
    VirtualProtect((void *)GAME(0x10329000u), 0x2000, PAGE_READWRITE, &old);   /* the imports and the vtable (.rdata) */
    *(void **)GAME(0x10329400u) = (void *)timeGetTime; *(void **)GAME(0x103290F8u) = (void *)Sleep;
    g_lim_entry = (uint32_t)GAME(0x10012B05u); g_lap_orig = (uint32_t)GAME(0x100194F0u);
    if (pacing) ((void **)GAME(0x1032A118u))[8] = (void *)lim_lap;
    else {                                            /* step_test: on to 0x10012CB8, then back (jmp lim_end); g_pCamera */
      uint8_t *e = (uint8_t *)GAME(0x10012CB8u); int32_t rl = (int32_t)((uint8_t *)lim_end - (e + 5)); e[0] = 0xE9; memcpy(e + 1, &rl, 4);
      *(void **)GAME(0x104568FCu) = g_lim;
    }
    *(void **)g_tim = (void *)GAME(0x1032A118u); *(float *)(g_tim + 8) = 1.0f; *(DWORD *)(g_tim + 0xC) = timeGetTime();
    *(void **)(g_lim + 0x1C) = g_tim; *(int *)(g_lim + 0x30) = 1;
  }
  touch("xikit.ini", "[xikit]\nlog_folder=0\nswap=0\ncount_calls=1\nreport_every=60\nhotkey=0\nmods=speedfix.dll\n");
  DeleteFileA("speedfix.ini");
  /* the full run starts with batching on (every device method hooked); the minimal run with the defaults (only the
     cached setters and the methods around their state hooked) */
  if (lazy) touch("speedfix.ini", "[speedfix]\nlazy_state=1\nvs_dedupe=1\n");
  else if (blocks) touch("speedfix.ini", "[speedfix]\nblock_cache=1\n");
  else if (pacing) touch("speedfix.ini", "[speedfix]\nprecise_pacing=1\nstats_every=1\n");
  else if (step) touch("speedfix.ini", "[speedfix]\nframe_step=2\nstats_every=1\n");
  else if (!minimal) touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\nmotion_stats=1\nskin_merge=1\n");
  if (!LoadLibraryA("xikit.dll")) { printf("FAIL: xikit.dll not loaded\n"); return 2; }
  for (int i = 0; i < 300 && g_dev.vt == g_vt; i++) Sleep(20);        /* the kit puts its vtable copy on the device */
  check(g_dev.vt != g_vt, "the kit hooked the device");
  /* the kit puts its Present hook on the device, logs the environment, and only then puts the mods' device hooks in
     front (xikit.c, start): wait for that, or the first calls miss speedfix */
  for (int i = 0; i < 500 && !log_has("methods hooked by mods"); i++) Sleep(20);
  check(log_has("methods hooked by mods"), "the kit put the mods' device hooks in front");
  frames(3);
  {   /* skin_merge (the kit writes code hooks at a Present): on at the start (the full run), SkinAndDrawV2's entry
         (0x1002DC10) goes to speedfix's fork; off (minimal, lazy), it is the game's code */
    uint8_t *e = (uint8_t *)GAME(0x1002DC10u); HMODULE m = NULL;
    void *to = e[0] == 0xE9 ? e + 5 + *(int32_t *)(e + 1) : NULL;
    if (to) GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)to, &m);
    int forked = m && m == GetModuleHandleA("speedfix.dll");
    if (!minimal && !lazy && !blocks && !pacing && !step) check(forked, "skin_merge=1 at the start: SkinAndDrawV2's entry jumps into speedfix.dll (the fork)");
    else check(!forked, "skin_merge off at the start: SkinAndDrawV2 is the game's code");
  }
  if (pacing) { frames(1); return pacing_test(); }
  if (step) { frames(1); return step_test(); }
  if (minimal) {
    check(GetFileAttributesA("speedfix.ini") != INVALID_FILE_ATTRIBUTES, "speedfix.ini written with the defaults");
    LONG m = g_n; begin(); setrs(7, 1); setrs(7, 1); settex(0, &g_tex1); settex(0, &g_tex1);
    float v[12] = {0}; dpup(4, 1, v, 16); dpup(4, 1, v, 16); end(); present();
    check(count(50, m) == 1 && count(61, m) == 1 && count(72, m) == 2, "minimal: the state cache works; DrawPrimitiveUP passes through");
    touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\n"); frames(65);
    m = g_n; begin(); dpup(4, 1, v, 16); dpup(4, 1, v, 16);
    check(count(72, m) == 2, "minimal: batch_up_draws=1 written while running is left for the next start (each draw still reaches the device)");
    end(); present();
    printf(fails ? "FAILED (%d)\n" : "ALL OK\n", fails);
    return fails ? 1 : 0;
  }

  if (blocks) {
    /* block_cache: a block recorded through the hooks; applying it sets what it holds in the shadow */
    begin(); setrs(7, 1); setrs(22, 3);
    beginsb(); setrs(15, 1); settss(0, 16, 2); setrs(15, 1); DWORD t = endsb();
    setrs(15, 0);
    LONG m = g_n; applysb(t); setrs(15, 1); settss(0, 16, 2); setrs(22, 3); setrs(7, 1);
    check(!strcmp(seq(m), "54"), "block_cache: after the block, its own values (render state, stage state) and the states it does not touch are cached");
    m = g_n; setrs(15, 0);
    check(!strcmp(seq(m), "50"), "a value other than the block's still reaches the device");
    /* a block that binds a texture or sets the shader: those are made unknown */
    settex(0, &g_tex1); setvs(0x111);
    beginsb(); settex(0, &g_tex1); setvs(0x111); DWORD t2 = endsb();
    m = g_n; applysb(t2); settex(0, &g_tex1); setvs(0x111); setrs(22, 3);
    check(!strcmp(seq(m), "54 61 76"), "a block binding a texture or setting the shader: those go out again after it; other states stay cached");
    /* CaptureStateBlock, DeleteStateBlock, CreateStateBlock: the block is no longer followed */
    capturesb(t); m = g_n; applysb(t); setrs(22, 3);
    check(!strcmp(seq(m), "54 50"), "after CaptureStateBlock the block's contents are unknown: applying it forgets the cache");
    DWORD t3 = createsb(1); m = g_n; applysb(t3); setrs(22, 3);
    check(!strcmp(seq(m), "54 50"), "a block made by CreateStateBlock is not followed");
    beginsb(); setrs(15, 1); DWORD t4 = endsb(); deletesb(t4);
    end(); present();
    /* switched off while running: every block forgets the cache again */
    touch("speedfix.ini", "[speedfix]\nblock_cache=0\n"); frames(65);
    begin(); setrs(22, 3);
    beginsb(); setrs(15, 1); DWORD t5 = endsb();
    m = g_n; applysb(t5); setrs(22, 3);
    check(!strcmp(seq(m), "54 50"), "block_cache=0 while running: applying a block forgets the cache, as before");
    end(); present();
    touch("speedfix.ini", "[speedfix]\nblock_cache=1\n"); frames(65);
    begin(); setrs(22, 3); m = g_n; applysb(t5); setrs(22, 3); setrs(15, 1);
    check(!strcmp(seq(m), "54"), "on again: a block recorded while it was off is followed (blocks are always followed)");
    end(); present();
    printf(fails ? "FAILED (%d)\n" : "ALL OK\n", fails);
    return fails ? 1 : 0;
  }

  if (lazy) {
    float v[12] = {0}; DWORD x = 0;
    /* the game's UI quad (Cell_DrawQuadBlended): blending on, draw, blending off; the next quad turns it on again */
    LONG m = g_n; begin(); m = g_n;
    setrs(27, 1); dpup(4, 1, v, 16); setrs(27, 0); setrs(27, 1); dpup(4, 1, v, 16); setrs(27, 0);
    check(!strcmp(seq(m), "50 72 72"), "lazy_state: on, draw, off, on, draw, off -> the device sees on, draw, draw (the off/on between never reaches it)");
    end();
    check(!strcmp(seq(m), "50 72 72 50 35") && last(50)->a[0] == 27 && last(50)->a[1] == 0, "... and the last off goes out before EndScene");
    present();
    /* a read of the state gets the held value: it goes out first */
    begin(); m = g_n; setrs(7, 5); getrs(7, &x);
    check(!strcmp(seq(m), "50 51") && x == 5, "GetRenderState: the held change goes out first, and the read gives it");
    m = g_n; settss(0, 1, 2); clear();
    check(!strcmp(seq(m), "63 36"), "Clear: a held change goes out first");
    /* a held texture is referenced until sent or undone */
    m = g_n; settex(0, &g_tex1); LONG ar = g_addref, rl = g_release; dpup(4, 1, v, 16);
    check(!strcmp(seq(m), "61 72") && ar == 1 && g_release == rl + 1, "SetTexture: held with a reference, sent at the draw, the reference given back");
    m = g_n; ar = g_addref; rl = g_release; settex(0, &g_tex2); settex(0, &g_tex1); dpup(4, 1, v, 16);
    check(!strcmp(seq(m), "72") && g_addref == ar + 1 && g_release == rl + 1, "a texture change undone before the draw: never sent, its reference given back");
    end(); present();
    /* vs_dedupe: declarations walked by their tokens (a constant's data may hold 0xFFFFFFFF) */
    { static const DWORD d1[] = {0x20000000, 0x40020000, 0x40020003, 0xFFFFFFFF};
      static const DWORD d2[] = {0x20000000, 0x90000000 /* D3DVSD_CONST(0, 1) */, 0xFFFFFFFF, 1, 2, 3, 0x40020000, 0xFFFFFFFF};
      static const DWORD d3[] = {0x20000000, 0x90000000, 0xFFFFFFFF, 1, 2, 3, 0x40020003, 0xFFFFFFFF};
      static const DWORD fn[] = {0xFFFE0101, 0x0000FFFF};
      m = g_n; DWORD a = createvs(d1, NULL), b = createvs(d1, NULL);
      check(count(75, m) == 1 && a == b && (a & 1), "vs_dedupe: the same declaration twice -> one shader made, its handle given twice");
      m = g_n; DWORD c2 = createvs(d2, NULL), c3 = createvs(d3, NULL);
      check(count(75, m) == 2 && c2 != c3, "declarations that differ after a constant holding 0xFFFFFFFF are two shaders");
      m = g_n; DWORD f1 = createvs(d1, fn);
      check(count(75, m) == 1 && f1 != a, "a shader with code is made as asked");
      m = g_n; deletevs(a);
      check(count(78, m) == 0, "deleting one of two holders of a shared handle keeps the shader");
      deletevs(b);
      check(count(78, m) == 1 && last(78)->a[0] == a, "deleting the last deletes it");
      m = g_n; DWORD e = createvs(d1, NULL);
      check(count(75, m) == 1, "after that, the declaration is made again"); deletevs(e); deletevs(c2); deletevs(c3); deletevs(f1); }
    /* turned off while running: what is held goes out, and nothing more is held */
    begin(); setrs(7, 9);
    touch("speedfix.ini", "[speedfix]\nlazy_state=0\nvs_dedupe=1\n");
    for (int i = 0; i < 65; i++) present();
    m = g_n; setrs(7, 10);
    check(count(50, m) == 1, "lazy_state=0 while running: a change reaches the device at once");
    end(); present();
    /* null_draws: the four draw methods return without reaching the device; what lazy_state holds still goes out at
       them, and Clear and Present still reach it. Off again, the draws reach the device. */
    { static const WORD ix[3] = {0, 1, 2};
      touch("speedfix.ini", "[speedfix]\nlazy_state=1\nvs_dedupe=1\nnull_draws=1\nstats_every=1\n");
      for (int i = 0; i < 65; i++) present();
      begin(); m = g_n;
      setrs(7, 3); dp(1); setrs(7, 4); dip(1); settss(0, 1, 4); dipup(ix, v); setrs(27, 1); dpup(4, 1, v, 16); clear();
      check(!strcmp(seq(m), "50 50 63 50 36"), "null_draws=1: no draw reaches the device; each held change goes out at its draw; Clear goes out");
      end(); present();
      for (int i = 0; i < 30; i++) { Sleep(50); begin(); dp(1); end(); present(); }   /* past a statistics line (each second) */
      check(log_has("null_draws=1"), "the settings line names null_draws");
      touch("speedfix.ini", "[speedfix]\nlazy_state=1\nvs_dedupe=1\nnull_draws=0\n");
      for (int i = 0; i < 65; i++) present();
      begin(); m = g_n; dp(1); dip(1); dipup(ix, v); dpup(4, 1, v, 16);
      check(!strcmp(seq(m), "70 71 73 72"), "null_draws=0 while running: the four draws reach the device again");
      end(); present();
      check(log_has("speedfix: null_draws: ") && log_has(" draw calls not sent to the device"), "the statistics line counts the draws not sent"); }
    /* the lever A/B: lazy_state off (A) and on (B), phases of 1 s */
    touch("speedfix.ini", "[speedfix]\nlazy_state=1\nvs_dedupe=1\nab_levers=lazy_state, bogus\nab_after=0\nab_phase=1\nab_rounds=1\nab_settle=2\n");
    for (int i = 0; i < 400; i++) { begin(); end(); present(); Sleep(8); }
    printf(fails ? "FAILED (%d)\n" : "ALL OK\n", fails);
    return fails ? 1 : 0;
  }

  /* game-code changes made at the start or at the first frames */
  check(tier_before == 0x1974C084 && *(uint32_t *)GAME(0x1006CDDCu) == 0x9090C084, "cpu_tier: 74 19 at 1006CDDE became 90 90 (g_pProcessor was 0)");
  check(*(uint32_t *)GAME(0x10456AD0u) == 15, "texture_compression: g_cfgTextureCompression is 15");
  void *bucket = call_target(0x100CC58Au);
  check(bucket && (uintptr_t)bucket != GAME(0x10014D50u), "occlusion_stagger=8: the call at 100CC58A goes to speedfix");
  check(call_target(0x1006C4D7u) == (void *)GAME(0x1006C8B0u), "skip_all_occlusion_readbacks=0: the readback call is the game's");
  (void)tex_before;

  /* state cache: a repeat is not forwarded; a change is; BeginScene forgets (state_cache_reset_each_scene=1) */
  LONG m = g_n; begin();
  setrs(7, 1); setrs(7, 1); setrs(7, 0); setrs(7, 0);
  settss(0, 1, 4); settss(0, 1, 4); settex(0, &g_tex1); settex(0, &g_tex1); setvs(0x142); setvs(0x142);
  check(count(50, m) == 2 && count(63, m) == 1 && count(61, m) == 1 && count(76, m) == 1,
        "state cache: SetRenderState x4 (two values) -> 2 reach the device; SetTextureStageState, SetTexture, SetVertexShader repeated -> 1 each");
  end(); present(); m = g_n; begin(); setrs(7, 0);
  check(count(50, m) == 1, "a new scene forgets the shadow: the same value is forwarded again");
  /* another thread: forwarded, and the render thread's shadow is forgotten */
  setrs(7, 1); m = g_n;
  HANDLE t = CreateThread(NULL, 0, other_thread, NULL, 0, NULL); WaitForSingleObject(t, 5000);
  setrs(7, 1);
  check(count(50, m) == 2, "a call from another thread is forwarded and the render thread's next repeat is too");
  end(); present();

  /* batching: switched off and on again through the ini (re-read within 60 frames) */
  touch("speedfix.ini", "[speedfix]\nbatch_up_draws=0\n"); frames(65);
  float va[3 * 4] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, vb[3 * 4] = {21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
  m = g_n; begin(); dpup(4, 1, va, 16); dpup(4, 1, vb, 16); end(); present();
  check(count(72, m) == 2, "batch_up_draws=0 (set while running): each DrawPrimitiveUP reaches the device");
  touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\nocclusion_stagger=8\nskip_all_occlusion_readbacks=1\n");
  frames(65);
  m = g_n; begin(); dpup(4, 1, va, 16); dpup(4, 1, vb, 16);
  check(count(72, m) == 0, "batch_up_draws=1: two lists of one stride are held");
  setrs(7, 0);
  Call *c = last(72);
  check(count(72, m) == 1 && c && c->a[0] == 4 && c->a[1] == 2 && c->a[2] == 16 && !memcmp(c->bytes, va, 48) && !memcmp(c->bytes + 48, vb, 48),
        "a state change sends them first, as one list of 2 triangles with both vertex sets in order");
  check(g_log[g_n - 1].slot == 50, "... and then the state change");
  /* a strip under solid fill and Gouraud shading becomes a list with the strip's winding */
  float s4[4 * 4] = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3};
  m = g_n; dpup(5, 2, s4, 16); clear();
  c = last(72);
  { const float *o = (const float *)c->bytes; int ok = c->a[0] == 4 && c->a[1] == 2;
    static const int want[6] = {0, 1, 2, 1, 3, 2}; for (int i = 0; i < 6; i++) ok = ok && o[4 * i] == (float)want[i];
    check(ok, "a strip of 2 triangles goes out as the list (0,1,2)(1,3,2), at the next device call (Clear)"); }
  setrs(9, 1); m = g_n; dpup(5, 2, s4, 16);
  check(count(72, m) == 1 && last(72)->a[0] == 5, "with flat shading a strip is not converted: it reaches the device as a strip");
  setrs(9, 2); end(); present();

  /* lazy_state with batching (every method hooked: allowed while running): the UI quads' blend toggles are undone
     before they reach the device, so the two quads become one draw */
  touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\nocclusion_stagger=8\nskip_all_occlusion_readbacks=1\nlazy_state=1\n");
  frames(65);
  m = g_n; begin(); m = g_n;
  setrs(27, 1); dpup(4, 1, va, 16); setrs(27, 0); setrs(27, 1); dpup(4, 1, vb, 16); setrs(27, 0); clear();
  check(!strcmp(seq(m), "50 72 50 36") && g_log[m + 1].a[1] == 2, "lazy_state + batching: on, quad, off, on, quad, off, Clear -> on, one draw of both, off, Clear");
  end(); present();
  touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\nocclusion_stagger=8\nskip_all_occlusion_readbacks=1\n"); frames(65);

  /* occlusion_stagger=8 through the re-aimed call: the bucket answered only on frame mod 8 == bucket + 4k */
  { uint8_t actor[0xA00]; int *frame = (int *)(g_cam + 0x34); int hits = 0, bad = 0, first = -1;
    *(int *)(actor + 0x9F0) = 2; begin();
    for (int f = 0; f < 32; f++) { *frame = f; int r = call_with_esi(bucket, actor); int q = (r % 4) == 2;
      if (q) { hits++; if (f % 4 != 2) bad++; if (first < 0) first = f; else if ((f - first) % 8) bad++; } }
    check(hits == 4 && !bad, "occlusion_stagger=8: an actor of bucket 2 is queried on 4 of 32 frames, 8 apart, all of them frames the game would query");
    end(); present(); *frame = 6; hits = 0;
    for (int f = 0; f < 8; f++) { *frame = f; hits += call_with_esi(bucket, actor) == f; }
    check(hits == 8, "in a frame without the kit's hooks (no BeginScene through them) the game's own test is used");
  }
  check(call_target(0x1006C4D7u) != (void *)GAME(0x1006C8B0u), "skip_all_occlusion_readbacks=1: the readback call goes to speedfix");
  { typedef int (__attribute__((thiscall)) *Rb)(void *, int); Rb rb = (Rb)call_target(0x1006C4D7u);
    begin(); check(rb((void *)0x1234, 0) == 256, "... and answers 256 (fully visible) without the game's readback"); end(); present(); }
  /* occlusion_late: the game's own readback (0x1006C8B0, run here on fake surfaces) gives the truth for each picture;
     the late path must give the previous probe's, per query object, counted the same way */
  { typedef int (__attribute__((thiscall)) *Rb)(void *, int); Rb rb = (Rb)call_target(0x1006C4D7u);
    *(void **)GAME(0x1047C004u) = g_mng; *(void **)GAME(0x1047BFA8u) = g_modx;
    struct { short w[2]; void *s; } q1 = {{0, 0}, &g_gameSurf}, q2 = {{0, 0}, &g_gameSurf};
    int truth[3];
    touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\nocclusion_stagger=8\n"); frames(65);
    begin(); for (int k = 0; k < 3; k++) { picture(k); truth[k] = rb(&q1, 0); } end(); present();
    printf("info: the game's readback answers %d (nothing drawn), %d (all), %d (top half)\n", truth[0], truth[1], truth[2]);
    check(truth[0] == 0 && truth[1] == 256 && truth[2] > 0 && truth[2] < 256, "the game's own readback runs on the fake surfaces: 0, 256 and a part");
    touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\nocclusion_stagger=8\nocclusion_late=1\nstats_every=1\n"); frames(65);
    LONG locks0 = g_nLock;
    begin();
    picture(1); int a1 = rb(&q1, 0);           /* no previous picture: the game's answer (256), this one queued */
    LONG locks1 = g_nLock;
    picture(0); int a2 = rb(&q1, 0);           /* the previous picture's answer (256) */
    picture(2); int a3 = rb(&q1, 0);           /* 0 */
    picture(1); int a4 = rb(&q1, 0);           /* the top half's, as the game counts it */
    picture(0); int b1 = rb(&q2, 0);           /* another object: no previous of its own: the game's 0 */
    picture(0); int a5 = rb(&q1, 0);           /* q1's previous (all drawn), not q2's */
    end(); present();
    check(a1 == 256 && locks1 - locks0 == 1, "occlusion_late=1: a first probe is answered by the game's readback (its LockRect)");
    check(a2 == 256 && a3 == 0 && a4 == truth[2], "... later probes answer the previous probe's picture, counted exactly as the game counts (the half picture: the game's value)");
    check(b1 == 0 && a5 == 256, "... per query object: a second object starts with the game's answer, and the first keeps its own previous picture");
    check(g_nRT == 2, "... each object's copy goes to a lockable render target of its own (CreateRenderTarget, two objects)");
    frames(40);                                  /* older than OL_MAX_AGE (32 frames) */
    begin(); picture(2); int s1 = rb(&q1, 0); end(); present();
    check(s1 == truth[2], "... a previous picture older than 32 frames is not used: the game's answer for this frame's picture");
    LONG rel0 = g_nSurfRelease;
    DEV(14, HRESULT (__stdcall *)(void *, void *))(&g_dev, 0);
    check(g_nSurfRelease - rel0 == 2, "... Reset releases its surfaces first (both)");
    begin(); picture(1); int r1 = rb(&q1, 0); end(); present();
    check(r1 == 256 && g_nRT == 3, "... after Reset: the game's answer, and a new surface");
    touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\nocclusion_stagger=8\nocclusion_late=2\nstats_every=1\n"); frames(65);
    begin(); picture(1); int t1 = rb(&q1, 0); picture(0); int t2 = rb(&q1, 0); picture(2); int t3 = rb(&q1, 0); end(); present();
    check(t1 == 256 && t2 == 0 && t3 == truth[2], "occlusion_late=2 (measure only): every answer is the game's");
    for (int i = 0; i < 300 && !log_has("occlusion_late=2: of"); i++) { frames(1); Sleep(10); }   /* stats_every=1: once a second */
    check(log_has("occlusion_late=2: of") && log_has("the late answer differed from the game's in"),
          "... how often the late answer would have differed from the game's is logged");
    check(log_has("speedfix: readback time: the game's readback") && log_has("its late read") && log_has("occlusion_late: its surfaces: 16x16 lockable render targets, format 21"),
          "... the time in the game's readback and in its copy and late read is logged, and its surfaces' format");
    /* a frame with every readback skipped is in neither per-frame figure (the owner's run 3464 divided by them) */
    long fon = 0, fskip = 0;
    for (int k = 0; k < 5 && fskip <= 0; k++) {   /* a second's statistics can end between the two settings: again */
      touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\nocclusion_stagger=8\nstats_every=1\n"); frames(65);
      long x, y; int before = rb_lines(&x, &y);
      begin(); rb(&q1, 0); end(); present();
      touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\nocclusion_stagger=8\nskip_all_occlusion_readbacks=1\nstats_every=1\n"); frames(65);
      for (int i = 0; i < 300 && rb_lines(&fon, &fskip) == before; i++) { frames(1); Sleep(10); }
    }
    check(fskip > 0, "... frames with every readback skipped are counted apart, not as frames that ran the game's readback");
    touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\nocclusion_stagger=8\nskip_all_occlusion_readbacks=1\n"); frames(65);
    *(void **)GAME(0x1047C004u) = 0; *(void **)GAME(0x1047BFA8u) = 0;
  }
  /* sse_skinning: off (the default) leaves the game's SSE flag; on sets it (the CPU under Wine reports SSE); off again
     puts the game's value back */
  check(g_cpu[9] == 0, "sse_skinning=0: the game's SSE flag (CPU object +9) left at 0");
  touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\nocclusion_stagger=8\nskip_all_occlusion_readbacks=1\nsse_skinning=1\n"); frames(65);
  check(g_cpu[9] == 1, "sse_skinning=1: the flag is 1, so 0x1002F1D0 takes the game's SSE loop");
  touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\nocclusion_stagger=8\nskip_all_occlusion_readbacks=1\nsse_skinning=0\n"); frames(65);
  check(g_cpu[9] == 0, "sse_skinning=0 again: the game's own value (0) is back");
  /* merge_scenes: inside a frame the game's EndScene and the BeginScene after it never reach the device; the scene
     ends once, before Present; a CopyRects closes it first (then the next BeginScene goes through) */
  touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\nocclusion_stagger=8\nskip_all_occlusion_readbacks=1\nmerge_scenes=1\nstats_every=1\n"); frames(65);
  { LONG m = g_n; begin(); end(); begin(); end(); begin(); end(); present();
    check(count(34, m) == 1 && count(35, m) == 1 && last(35) && last(15) && last(35) < last(15),
          "merge_scenes=1: three scenes in a frame reach the device as one BeginScene and one EndScene, before Present");
    m = g_n; begin(); end(); copyrects(); begin(); end(); present();
    check(count(34, m) == 2 && count(35, m) == 2 && !strcmp(seq(m), "34 35 28 34 35 15"),
          "... a CopyRects between scenes ends the held scene first, and the next BeginScene goes through");
    touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\nocclusion_stagger=8\nskip_all_occlusion_readbacks=1\nmerge_scenes=0\nstats_every=1\n"); frames(65);
    m = g_n; begin(); end(); begin(); end(); present();
    check(count(34, m) == 2 && count(35, m) == 2, "merge_scenes=0 again: every scene reaches the device"); }
  /* motion_stats (on at this start): 30 frames with the same view, then 30 with the view changing each frame; the log
     line each second splits them (test_speedfix.py reads it) */
  for (int i = 0; i < 30; i++) { begin(); setview(0); end(); present(); }
  for (int i = 0; i < 30; i++) { begin(); setview((float)i); end(); present(); }
  Sleep(1100); frames(2);
  /* the tier the game chose, logged once it has chosen: a processor object of the x87 class (test_speedfix.py reads the log) */
  { static uint32_t proc[4]; proc[0] = (uint32_t)GAME(0x1032C368u); *(void **)GAME(0x1047CF7Cu) = proc; frames(3); }

  printf(fails ? "FAILED (%d)\n" : "ALL OK\n", fails);
  return fails ? 1 : 0;
}
