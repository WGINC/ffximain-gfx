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
static const float *g_follow_dec;   /* the camera follow timer's per-pass amount, in speedfix.dll */
static uint32_t rel;
/* an analysed address -> the mapped game's, as the kit translates it: on another build its manifest's !shift ranges
   (kit/xikit/manifest_for_build.py; code and, since build 6AE818B4, data) */
static int32_t g_shift[1 + 3 * 64];
static uintptr_t game_addr(uint32_t a) {
  int32_t d = 0;
  for (int i = 0; i < g_shift[0]; i++) if (a >= (uint32_t)g_shift[1 + 3 * i] && a < (uint32_t)g_shift[2 + 3 * i]) { d = g_shift[3 + 3 * i]; break; }
  return (uintptr_t)a + rel + d;
}
static void load_shifts(HMODULE g) {
  IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS *)((char *)g + ((IMAGE_DOS_HEADER *)g)->e_lfanew);
  char p[64], line[512]; snprintf(p, sizeof p, "xikit_manifest_%08lX.txt", (unsigned long)nt->FileHeader.TimeDateStamp);
  FILE *f = fopen(p, "r"); if (!f) return;
  while (fgets(line, sizeof line, f)) { unsigned lo, hi; int d;
    if (sscanf(line, "!shift %x %x %d", &lo, &hi, &d) == 3 && g_shift[0] < 64) { int i = g_shift[0]++; g_shift[1 + 3 * i] = (int32_t)lo; g_shift[2 + 3 * i] = (int32_t)hi; g_shift[3 + 3 * i] = d; } }
  fclose(f);
  printf("    this build (%08lX): %d shift ranges from %s\n", (unsigned long)nt->FileHeader.TimeDateStamp, g_shift[0], p);
}
#define GAME(a) game_addr((uint32_t)(a))
static int fails;
static void check(int c, const char *what) { printf("%s: %s\n", c ? "ok" : "FAIL", what); if (!c) fails++; }
static void task_dispatch_test(HMODULE sf);
static void occl_hold_test(HMODULE sf);
static void flare_test(void);
static void gpu_skin_test(void);

/* ---- the fake device: what reaches it is recorded ---- */
typedef struct { int slot; uint32_t a[4]; int nbytes; uint8_t bytes[256]; } Call;
static Call g_log[32768]; static volatile LONG g_n;
static void rec(int slot, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, const void *data, int n) {
  LONG i = InterlockedIncrement(&g_n) - 1;
  if (i >= (LONG)(sizeof g_log / sizeof g_log[0])) { if (i == (LONG)(sizeof g_log / sizeof g_log[0])) printf("FAIL: the test's call log is full (sf_host.c g_log): later checks cannot count calls\n"); return; }
  Call *c = &g_log[i]; c->slot = slot; c->a[0] = a0; c->a[1] = a1; c->a[2] = a2; c->a[3] = a3;
  c->nbytes = n > 256 ? 256 : n; if (data && n > 0) memcpy(c->bytes, data, c->nbytes);
}
static DWORD g_rs[256];
/* index buffers (skin_merge=2 joins a model's index ranges into one it creates): Lock, Unlock, GetDesc */
typedef struct { void **vt; DWORD fmt, usage, pool, size; uint16_t ix[512]; LONG refs; } FIb;
static void *g_ivt[14]; static FIb g_ib1, *g_made[64]; static int g_nmade;
static ULONG __stdcall i_addref(FIb *b) { return ++b->refs; }
static ULONG __stdcall i_release(FIb *b) { return --b->refs; }
static HRESULT __stdcall i_lock(FIb *b, UINT off, UINT n, BYTE **p, DWORD f) { *p = (BYTE *)b->ix + off; return 0; }
static HRESULT __stdcall i_unlock(FIb *b) { return 0; }
static HRESULT __stdcall i_getdesc(FIb *b, DWORD *d) { d[0] = b->fmt; d[1] = 7; d[2] = b->usage; d[3] = b->pool; d[4] = b->size; return 0; }
static HRESULT __stdcall d_createib(void *d, UINT len, DWORD us, DWORD fmt, DWORD pool, FIb **o) {
  FIb *b = (FIb *)calloc(1, sizeof *b); b->vt = g_ivt; b->fmt = fmt; b->usage = us; b->pool = pool; b->size = len; b->refs = 1;
  if (g_nmade < 64) g_made[g_nmade++] = b; rec(24, len, us, fmt, pool, 0, 0); *o = b; return 0; }
static HRESULT __stdcall d_setindices(void *d, void *ib, UINT base) { rec(85, (uint32_t)(uintptr_t)ib, base, 0, 0, 0, 0); return 0; }
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
static HRESULT __stdcall d_dip(void *d, DWORD t, UINT mi, UINT nv, UINT si, UINT n) { rec(71, t, mi, nv, n, &si, 4); return 0; }
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
static int g_surf_notyet; static LONG g_nNotYet;   /* locks with d3d8on11's do-not-wait flag (0x4000) are told "not yet" */
static HRESULT __stdcall s_lock(FSurf *s, FLock *lr, const RECT *r, DWORD f) {
  if ((f & 0x4000) && g_surf_notyet) { g_nNotYet++; return (HRESULT)0x8876021CL; }
  g_nLock++; lr->Pitch = 64; lr->pBits = s->px; return 0; }
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
static HRESULT __stdcall d_settransform(void *d, DWORD st, const float *m) { rec(37, st, 0, 0, 0, m, m ? 64 : 0); return 0; }
static HRESULT __stdcall d_setss(void *d, UINT n, void *vb, UINT stride) { rec(83, n, (uint32_t)(uintptr_t)vb, stride, 0, 0, 0); return 0; }
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
static void dipr(UINT mi, UINT nv, UINT si, UINT n) { DEV(71, HRESULT (__stdcall *)(void *, DWORD, UINT, UINT, UINT, UINT))(&g_dev, 4, mi, nv, si, n); }
static void setindices(void *ib) { DEV(85, HRESULT (__stdcall *)(void *, void *, UINT))(&g_dev, ib, 0); }
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
static int log_count(const char *what) {                  /* how many times xikit.log has `what` */
  FILE *f = fopen("xikit.log", "rb"); if (!f) return 0;
  static char b[1 << 20]; size_t n = fread(b, 1, sizeof b - 1, f); fclose(f); b[n] = 0;
  int c = 0; for (char *p = b; (p = strstr(p, what)); p++) c++;
  return c;
}
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
/* write a settings file so that speedfix sees it changed: speedfix re-reads when the file's write time differs from the
   one it read, so a second write inside the same tick of the file system's clock is written again until the time moves */
static void touch(const char *path, const char *body) {
  WIN32_FILE_ATTRIBUTE_DATA a0, a1; int had = GetFileAttributesExA(path, GetFileExInfoStandard, &a0);
  for (int i = 0; i < 200; i++) {
    Sleep(30); FILE *f = fopen(path, "w"); fputs(body, f); fclose(f);
    if (!had || !GetFileAttributesExA(path, GetFileExInfoStandard, &a1) || CompareFileTime(&a0.ftLastWriteTime, &a1.ftLastWriteTime)) break;
  }
}

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
  /* (after the limiter's checks: these calls move precise_pacing's base outside the frame's own order) */
  { /* (28 Nov) the fork of Frame_Render makes the game's three rate calls from speedfix.dll and says which
       (xi_rate_site, fork/Frame_Render.fork.cpp): a call marked 3 sets the base, a call marked 1 then answers the
       counter's ms since it (not whole ms); unmarked, from here, the game's function */
    typedef float (__attribute__((thiscall)) *Rate)(void *);
    volatile int *site = (volatile int *)GetProcAddress(GetModuleHandleA("speedfix.dll"), "xi_rate_site");
    int frac = 0, close = 0, k; int32_t paused = *(int32_t *)(g_tim + 0x1C);
    touch("speedfix.ini", "[speedfix]\nprecise_pacing=1\nstats_every=1\n"); frames(70);   /* (on again; a frame through the hooks) */
    for (k = 0; k < 5 && site; k++) {
      *(int *)(g_tim + 0x1C) = 0;
      *site = 3; ((Rate)vt[9])(g_tim); *site = 0; double t0 = qms();
      Sleep(3 + k);
      double t1 = qms(); *site = 1; float r = ((Rate)vt[9])(g_tim); *site = 0;
      double ms = 1000.0 / r;
      if (fabs(ms - floor(ms + 0.5)) > 0.001) frac++;
      if (fabs(ms - (t1 - t0)) < 0.3) close++;
    }
    *(int32_t *)(g_tim + 0x1C) = paused;
    printf("    the fork's marked rate calls: %d of 5 answered in fractional ms, %d of 5 within 0.3 ms of the counter's time\n", frac, close);
    check(site && frac >= 4 && close >= 4, "precise_pacing=1: the timer's rate called as the fork's site 1 after its site 3 answers the counter's ms between them, not whole ms");
  }
  { /* pace_adaptive (27 Nov): frames whose own work takes 2 to 7 ms (a Sleep of 2 + (7i mod 6)); without it, the
       intervals between Presents follow the work; with it (pace_pct 90, changed while running), each frame is held to
       the 90th percentile of the last 120 frames' own times: the intervals' spread falls and their median comes near
       that percentile */
    double iv[2][200]; int nn[2] = {0, 0};
    for (int on = 0; on < 2; on++) {
      touch("speedfix.ini", on ? "[speedfix]\npace_adaptive=1\npace_pct=90\nstats_every=1\n" : "[speedfix]\npace_adaptive=0\nstats_every=1\n");
      for (int i = 0; i < 70; i++) { begin(); end(); Sleep(2 + (7 * i) % 6); present(); }
      double prev = qms();
      for (int i = 0; i < 200; i++) { begin(); end(); Sleep(2 + (7 * i) % 6); present(); double t = qms(); iv[on][nn[on]++] = t - prev; prev = t; }
    }
    double m[2], sd[2], med[2]; int nearmed[2];
    for (int on = 0; on < 2; on++) {
      double s = 0, s2 = 0, tmp[200]; for (int i = 0; i < nn[on]; i++) { s += iv[on][i]; s2 += iv[on][i] * iv[on][i]; tmp[i] = iv[on][i]; }
      m[on] = s / nn[on]; sd[on] = sqrt(s2 / nn[on] - m[on] * m[on]);
      for (int i = 1; i < nn[on]; i++) for (int j = i; j > 0 && tmp[j - 1] > tmp[j]; j--) { double x = tmp[j]; tmp[j] = tmp[j - 1]; tmp[j - 1] = x; }
      med[on] = tmp[nn[on] / 2];
      nearmed[on] = 0; for (int i = 0; i < nn[on]; i++) nearmed[on] += fabs(iv[on][i] - med[on]) <= 0.5;
    }
    printf("    pace_adaptive: off: intervals mean %.2f ms, sd %.2f, median %.2f, %d of %d within 0.5 ms of it; on: mean %.2f, sd %.2f, median %.2f, %d of %d within 0.5 ms\n",
           m[0], sd[0], med[0], nearmed[0], nn[0], m[1], sd[1], med[1], nearmed[1], nn[1]);
    /* what the pacer does: a frame done before the aim is let go at the aim, so most intervals sit at one value (those
       over the aim, about 1 in 10 at pace_pct 90, go at their own time) */
    check(nearmed[1] >= nn[1] * 3 / 4 && nearmed[1] > 2 * nearmed[0], "pace_adaptive=1: three in four intervals between Presents or more within 0.5 ms of their median, over twice as many as without it");
    check(m[1] >= m[0] && m[1] < m[0] * 1.6, "pace_adaptive=1: the frame rate falls to the 90th percentile's, not further (mean interval under 1.6 times)");
    Sleep(1100); for (int i = 0; i < 70; i++) { begin(); end(); present(); }   /* (stats_every=1: the next statistics line) */
    check(log_has("speedfix: pace_adaptive: ") && log_has(" frames paced, the interval aimed at "), "pace_adaptive: the statistics line says how many frames it paced and the interval it aimed at");
    touch("speedfix.ini", "[speedfix]\npace_adaptive=0\nstats_every=1\n");
    for (int i = 0; i < 70; i++) { begin(); end(); present(); }
  }
  { /* stats_every=0 (14 Nov, the installer's quiet logs): no statistics line however long; 1 again: they come back */
    touch("speedfix.ini", "[speedfix]\nstats_every=0\n"); for (int i = 0; i < 70; i++) { begin(); end(); present(); }
    Sleep(1100); for (int i = 0; i < 70; i++) { begin(); end(); present(); }
    int before = log_count("speedfix: frames ");
    Sleep(2100); for (int i = 0; i < 70; i++) { begin(); end(); present(); }
    check(log_count("speedfix: frames ") == before, "stats_every=0: no statistics line in 2 s");
    touch("speedfix.ini", "[speedfix]\nstats_every=1\n"); Sleep(1100); for (int i = 0; i < 70; i++) { begin(); end(); present(); }
    Sleep(1100); for (int i = 0; i < 70; i++) { begin(); end(); present(); }
    check(log_count("speedfix: frames ") > before, "stats_every=1 again: the statistics lines come back");
  }
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
  check(*(uint8_t *)GAME(0x10012C3Cu) == 0xE8 && ((uint8_t *)GAME(0x10012C3Cu) + 5 + *(int32_t *)GAME(0x10012C3Du) == (uint8_t *)GAME(0x10311C2Cu)
        || (uint8_t *)GAME(0x10012C3Cu) + 5 + *(int32_t *)GAME(0x10012C3Du) == (uint8_t *)GAME(0x10311C6Cu)),   /* _ftol: 6A995428, 6AC37846 */
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
  { /* frame_step=3 (changed while running): the five tick loops (data/frame_step_loops.json). The camera follow's
       getter call (0x100205B1) is called alone (a ret written after it, the game's byte put back after): it must get
       the fraction, and the camera loop's factor (its operand, now pointing into speedfix.dll) must be
       1 - 0.75^(s/ceil s) for that s; the game's constant 0.25 must not be written */
    touch("speedfix.ini", "[speedfix]\nuncap=1\nframe_step=3\nstats_every=1\n");
    step_frames(70, 1, 3);
    check(*g_xi_mode == 3, "uncap=1 frame_step=3 (changed while running): the switch follows");
    check(log_has("speedfix: frame_step=3: 24 of 24 loop changes in place"), "frame_step=3: the 4 factor operands, the 3 calls of 0x100272B0, OnMove's 4 compares, the walk/run rate's 3 operands, the camera's integer loops' 3 calls of 0x100272B0, and the camera follow's 2 fmul [0.125], its timer's fsub, its chase call, its 2 calls of 0x100272E0 and its counter's mov changed (24 of 24)");
    { const uint8_t *fc = (const uint8_t *)GAME(0x1001FD5Au), *hc = (const uint8_t *)GAME(0x1001FB19u), *tc = (const uint8_t *)GAME(0x1001FA55u);
      HMODULE h1 = NULL, h2 = NULL, h3 = NULL;
      GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)(fc + 5 + *(const int32_t *)(fc + 1)), &h1);
      GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)(hc + 5 + *(const int32_t *)(hc + 1)), &h2);
      GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, *(LPCSTR *)(tc + 2), &h3);
      check(fc[0] == 0xE8 && h1 == sf && hc[0] == 0xE8 && h2 == sf && !memcmp(hc + 5, "\x0F\x1F\x44\x00\x00", 5) && hc[10] == 0xEB && h3 == sf,
            "... the camera follow's chase call (0x1001FD5A) and its counter's set (0x1001FB19: a call and a 5-byte nop, the game's jmp after) go into speedfix.dll; its timer's 1.0 is read from speedfix.dll");
      g_follow_dec = *(const float *const *)(tc + 2); }
    uint8_t *op = (uint8_t *)GAME(0x100205CDu); const float *kf = *(const float **)(op + 2); HMODULE h = NULL;
    GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)kf, &h);
    check(h == sf && *(const uint32_t *)GAME(0x10329CE4u) == 0x3E800000u, "... the camera loop's factor is read from speedfix.dll; the game's 0.25 is not written");
    uint8_t *c = (uint8_t *)GAME(0x100C6D76u); h = NULL;
    GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)(c + 5 + *(int32_t *)(c + 1)), &h);
    check(c[0] == 0xE8 && h == sf, "... OnMove's turning loop (0x100C6D76) calls into speedfix.dll (sf_scale_k)");
    uint8_t keep = *(uint8_t *)GAME(0x100205B6u); *(uint8_t *)GAME(0x100205B6u) = 0xC3;
    Getter loop = (Getter)GAME(0x100205B1u);
    int wrong = 0, frac = 0, kbad = 0;
    for (int i = 0; i < 100; i++) {
      begin(); end(); present(); Sleep(3); run_limiter(g_lim); g_step_make(g_lim, ring_raw());
      float a = loop();
      if (a != ring_avg(0)) wrong++;
      if (a != floorf(a)) frac++;
      float want = a == ceilf(a) ? 0.25f : (float)(1.0 - pow(0.75, (double)a / ceil((double)a)));
      if (a > 0 && *kf != want) kbad++;
    }
    *(uint8_t *)GAME(0x100205B6u) = keep;
    printf("    frame_step=3, the camera loop's call: %d of 100 not the step, %d a fraction, %d with a factor not 1 - 0.75^(s/ceil s)\n", wrong, frac, kbad);
    check(wrong == 0 && frac > 50, "frame_step=3: the camera loop's getter call gets the fraction (the step, not truncated)");
    check(kbad == 0, "frame_step=3: its factor is 1 - 0.75^(s/ceil s) for the s it was given");
    { /* step_exact=1 (27 Nov, changed while running): the step is the frame's own time by the counter (since the call
         before), within one millisecond (0.06 ticks at scale 1) of the game's own whole-ms time for the frame (the
         ring's newest, 0x10456964 + 4 * [0x10456974]); frames alternating 2 and 9 ms */
      touch("speedfix.ini", "[speedfix]\nuncap=1\nframe_step=3\nstep_exact=1\nstats_every=1\n");
      step_frames(70, 1, 3);
      volatile int *ex = (volatile int *)GetProcAddress(sf, "xi_step_exact");
      check(ex && *ex == 1, "step_exact=1 (changed while running): the fork's switch follows");
      keep = *(uint8_t *)GAME(0x100205B6u); *(uint8_t *)GAME(0x100205B6u) = 0xC3;
      int near_qpc = 0, in_ms = 0, n = 0; double sum = 0, t_first = 0, t_last = 0, prev = 0;
      for (int i = 0; i < 120; i++) {
        begin(); end(); present(); Sleep(i & 1 ? 9 : 2); run_limiter(g_lim);
        double t = qms(); g_step_make(g_lim, ring_raw());
        float a = loop(), v = ((const float *)GAME(0x10456964u))[*(const uint8_t *)GAME(0x10456974u) & 3];
        if (i > 0) {
          double want = (t - prev) * 0.06; n++; sum += a; if (!t_first) t_first = prev; t_last = t;
          if (fabs(a - want) <= 0.012 || fabs(a - v) >= 0.0599) near_qpc++;      /* (clamped at v +- 1 ms when the two differ by more) */
          if (fabs(a - v) <= 0.0601) in_ms++;
        }
        prev = t;
      }
      *(uint8_t *)GAME(0x100205B6u) = keep;
      printf("    step_exact=1, frames of 2 and 9 ms: %d of %d within 0.2 ms of the counter's time (or at the 1 ms bound), %d within 1 ms of the game's whole-ms time; the steps add up to %.2f ticks, the counter %.2f\n",
             near_qpc, n, in_ms, sum, (t_last - t_first) * 0.06);
      check(near_qpc >= n * 9 / 10 && in_ms == n, "step_exact=1: each frame's step is its own time by the counter, kept within 1 ms of the game's whole-ms time");
      check(fabs(sum - (t_last - t_first) * 0.06) < 0.06 * 3, "step_exact=1: over 119 frames the steps add up to the counter's time (within 3 ms)");
      touch("speedfix.ini", "[speedfix]\nuncap=1\nframe_step=3\nstats_every=1\n");
      step_frames(70, 1, 3);
      check(ex && *ex == 0, "step_exact back to 0 (changed while running): the switch follows");
    }
    touch("speedfix.ini", "[speedfix]\nuncap=1\nframe_step=1\nstats_every=1\n");
    step_frames(70, 1, 3);
    check(*kf == 0.25f, "frame_step=1 again: the camera loop's factor is the game's 0.25");
    check(g_follow_dec && *g_follow_dec == 1.0f, "frame_step=1 again: the camera follow's timer takes the game's 1.0 a pass");
  }
  task_dispatch_test(sf);
  occl_hold_test(sf);
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

/* skin_merge=2: inside sf_group_begin / sf_group_end (as speedfix's hook of SkinAndDrawV2 calls them) a model's
   opaque triangle-list draws under the same state go out as one draw of a joined index list; the rest keep the order */
static int group_test(void) {
  HMODULE sf = GetModuleHandleA("speedfix.dll");
  void (*gb)(void) = (void (*)(void))GetProcAddress(sf, "sf_group_begin"), (*ge)(void) = (void (*)(void))GetProcAddress(sf, "sf_group_end");
  check(gb && ge, "skin_merge=2: sf_group_begin and sf_group_end found");
  if (!gb || !ge) { printf("FAILED (%d)\n", fails); return 1; }
  check(jumps_into(0x1002DC10u, sf), "skin_merge=2 at the start: SkinAndDrawV2's entry jumps into speedfix.dll");
  g_ib1.vt = g_ivt; g_ib1.fmt = 101; g_ib1.pool = 1; g_ib1.size = sizeof g_ib1.ix; g_ib1.refs = 1;
  for (int i = 0; i < 512; i++) g_ib1.ix[i] = (uint16_t)(i % 40);
  begin(); setrs(27, 0);                           /* ALPHABLENDENABLE off, known before the model */
  LONG from = g_n;
  gb();
  setindices(&g_ib1);
  settex(0, &g_tex1); dipr(0, 10, 0, 2);           /* A: indices 0..5 */
  settex(0, &g_tex2); dipr(10, 10, 6, 2);          /* B: 6..11 */
  settex(0, &g_tex1); dipr(20, 10, 12, 1);         /* A: 12..14 */
  settex(0, &g_tex2); dipr(30, 10, 18, 1);         /* B: 18..20 */
  check(count(71, from) == 0, "... nothing drawn while the model's calls are held");
  ge();
  check(count(71, from) == 2, "... four draws under two textures: two draws");
  Call *d1 = NULL, *d2 = NULL; for (LONG i = from; i < g_n; i++) if (g_log[i].slot == 71) { if (!d1) d1 = &g_log[i]; else d2 = &g_log[i]; }
  check(d1 && d2 && d1->a[3] == 3 && d2->a[3] == 3 && d1->a[1] == 0 && d1->a[2] == 30 && d2->a[1] == 10 && d2->a[2] == 30,
        "... each of 3 triangles, over the union of the vertex ranges");
  check(g_nmade == 2 && g_made[0]->pool == 1 && g_made[0]->size == 18 && g_made[0]->ix[6] == 12 % 40 && g_made[0]->ix[8] == 14 % 40
        && g_made[1]->ix[0] == 6 && g_made[1]->ix[6] == 18, "... from two index lists made of the draws' ranges (managed pool)");
  { int texA = 0; LONG d1i = (LONG)(d1 - g_log); for (LONG i = from; i < d1i; i++) if (g_log[i].slot == 61) texA = g_log[i].a[1] == (uint32_t)(uintptr_t)&g_tex1; check(texA, "... the first draw under texture A (its first draw came first)"); }
  Call *si = last(85), *tx = last(61);
  check(si && si->a[0] == (uint32_t)(uintptr_t)&g_ib1 && tx && tx->a[1] == (uint32_t)(uintptr_t)&g_tex2, "... and the device left with the game's index buffer and last texture");
  /* the same model again: the joined lists are reused, not made again */
  from = g_n; gb(); setindices(&g_ib1);
  settex(0, &g_tex1); dipr(0, 10, 0, 2); settex(0, &g_tex2); dipr(10, 10, 6, 2); settex(0, &g_tex1); dipr(20, 10, 12, 1); settex(0, &g_tex2); dipr(30, 10, 18, 1);
  ge();
  check(count(71, from) == 2 && g_nmade == 2, "... the same model again: two draws, the joined lists reused");
  /* a blended draw keeps its place: the opaque draws before it go out first, those after it after */
  from = g_n; gb(); setindices(&g_ib1);
  settex(0, &g_tex1); dipr(0, 10, 0, 2); setrs(27, 1); dipr(10, 10, 6, 2); setrs(27, 0); dipr(20, 10, 12, 1);
  ge();
  { int k = 0; UINT st[3] = {0}; for (LONG i = from; i < g_n; i++) if (g_log[i].slot == 71 && k < 3) st[k++] = *(UINT *)g_log[i].bytes;
    check(count(71, from) == 3 && st[0] == 0 && st[1] == 6 && st[2] == 12, "... a blended draw between two opaque ones: three draws in the game's order"); }
  check(last(50)->a[0] == 27 && last(50)->a[1] == 0, "... and alpha blending left off, as the game left it");
  /* another device call in the model: what was held goes out first, in the game's order */
  from = g_n; gb(); setindices(&g_ib1);
  settex(0, &g_tex1); dipr(0, 10, 0, 2); settex(0, &g_tex2); dipr(10, 10, 6, 2); dp(1); settex(0, &g_tex1); dipr(20, 10, 12, 1);
  ge();
  { LONG p = -1; for (LONG i = from; i < g_n; i++) if (g_log[i].slot == 70) p = i;
    check(p >= 0 && count(71, from) == 3 && count(71, p) == 1, "... a DrawPrimitive in the model: the two draws before it go out before it, as made"); }
  end(); present();
  Sleep(1100); frames(2);                          /* stats_every=1: logged after a second */
  check(log_has("skin_merge=2: "), "... the grouping counted in the log");
  printf(fails ? "FAILED (%d)\n" : "ALL OK\n", fails);
  return fails ? 1 : 0;
}


/* task_draws (speedfix sf_fork.cpp sf_dispatch): fake tasks through the hooked CMoTask::Dispatch (0x10075000). Each
   slot records what it was given: ECX, the stack argument a jmp'd-to +0x24 sees at [esp+4], and speedfix's
   xi_task_vt / xi_task_slot while it runs */
static volatile void *const *g_tkvt; static volatile int *g_tksl;
static void *t_ecx20, *t_ecx24, *t_arg24, *t_vt20; static int t_sl20, t_sl24, t_n24;
static char __attribute__((thiscall)) t_f20_0(void *self) { t_ecx20 = self; t_vt20 = (void *)*g_tkvt; t_sl20 = *g_tksl; return 0; }
static char __attribute__((thiscall)) t_f20_1(void *self) { t_ecx20 = self; t_sl20 = *g_tksl; return 1; }
static void __attribute__((thiscall)) t_f24_pop(void *self, void *arg) { t_ecx24 = self; t_arg24 = arg; t_sl24 = *g_tksl; t_n24++; }   /* ret 4 */
static void __attribute__((thiscall)) t_f24_nopop(void *self) { t_ecx24 = self; t_sl24 = *g_tksl; t_n24++; }                        /* ret */
static void task_dispatch_test(HMODULE sf) {
  g_tkvt = (volatile void *const *)GetProcAddress(sf, "xi_task_vt"); g_tksl = (volatile int *)GetProcAddress(sf, "xi_task_slot");
  check(g_tkvt && g_tksl, "task_draws: speedfix exports xi_task_vt and xi_task_slot");
  check(log_has("task_draws: CMoTask::Dispatch hooked"), "task_draws=1 at the start: CMoTask::Dispatch hooked (the log says so)");
  if (!g_tkvt || !g_tksl) return;
  void (__cdecl *disp)(void *) = (void (__cdecl *)(void *))GAME(0x10075000u);
  static void *vtA[10], *vtB[10], *vtC[10]; static void *taskA[4], *taskB[4], *taskC[4];
  vtA[8] = (void *)t_f20_0; vtA[9] = (void *)t_f24_pop; taskA[0] = vtA;
  vtB[8] = (void *)t_f20_0; vtB[9] = (void *)t_f24_nopop; taskB[0] = vtB;
  vtC[8] = (void *)t_f20_1; vtC[9] = (void *)t_f24_pop; taskC[0] = vtC;
  volatile int guard = 0x5A5A5A5A; void *sp0, *sp1;
  __asm__ volatile("movl %%esp, %0" : "=r"(sp0));
  disp(taskA);
  __asm__ volatile("movl %%esp, %0" : "=r"(sp1));
  check(t_ecx20 == taskA && t_vt20 == vtA && t_sl20 == 1, "task_draws: +0x20 called with ECX = the task, xi_task_vt its vtable, slot 1");
  check(t_n24 == 1 && t_ecx24 == taskA && t_arg24 == taskA && t_sl24 == 2, "... +0x20 returning 0: +0x24 called with ECX = the task and [esp+4] = the task (as after the game's jmp), slot 2");
  check(sp0 == sp1 && guard == 0x5A5A5A5A, "... a +0x24 that pops 4 (ret 4): the stack as before");
  t_n24 = 0; disp(taskB);
  __asm__ volatile("movl %%esp, %0" : "=r"(sp1));
  check(t_n24 == 1 && t_ecx24 == taskB && sp0 == sp1, "... a +0x24 that pops nothing (ret): called, the stack as before");
  t_n24 = 0; disp(taskC);
  check(t_n24 == 0 && t_ecx20 == taskC && t_sl20 == 1, "... +0x20 returning 1: +0x24 not called");
  check(*g_tkvt == NULL && *g_tksl == 0, "... after each dispatch: no task running");
  /* Liminality (sf_main.cpp sf_task_skip, sf_task_post, lim_cam): on a frame of 0 ticks (forced: sf_lim_test_zero) a task
     that is not a drawing task is skipped; the game's three drawing tasks are not. An actor (vtable +0x20 the game's
     OnMove; never called here) keeps what OnMove left in +0xB0 +0xB2 +0x6A4 +0x8E8 after each of its ticks, gets them back
     when skipped, is drawn at the blend of its last two ticks' +0x5FC / +0x61C (the facing the short way round), and has
     the tick's values back before OnMove; the camera's eye and target are blended around Camera_ApplyViewProjection */
  volatile int *zero = (volatile int *)GetProcAddress(sf, "sf_lim_test_zero");
  volatile float *alpha = (volatile float *)GetProcAddress(sf, "sf_lim_test_alpha");
  int (__cdecl *skip)(void *) = (int (__cdecl *)(void *))GetProcAddress(sf, "sf_task_skip");
  void (__cdecl *post)(void *) = (void (__cdecl *)(void *))GetProcAddress(sf, "sf_task_post");
  void (*volatile *cam)(int) = (void (*volatile *)(int))GetProcAddress(sf, "xi_lim_cam");
  check(zero && alpha && skip && post && cam && *cam, "liminality: speedfix exports its test switches, sf_task_skip, sf_task_post and xi_lim_cam");
  if (!zero || !alpha || !skip || !post || !cam || !*cam) return;
  touch("speedfix.ini", "[speedfix]\nuncap=1\nframe_step=1\nliminality=1\ntask_draws=1\nstats_every=1\n");
  step_frames(70, 1, 3);
  check(log_has("speedfix: liminality: the three drawing tasks' vtables and CXiSkeletonActor's OnMove are the game's: on"), "liminality=1: the drawing tasks' vtables and OnMove checked against the game's (the log says so)");
  *zero = 1;
  t_ecx20 = NULL; t_n24 = 0; disp(taskA);
  int skipped = t_ecx20 == NULL && t_n24 == 0;
  static void *dA[1], *dB[1], *dC[1]; dA[0] = (void *)GAME(0x10329C44u); dB[0] = (void *)GAME(0x10336748u); dC[0] = (void *)GAME(0x10329C18u);
  int dvt = !skip(dA) && !skip(dB) && !skip(dC);
  check(skipped, "liminality: on a frame of 0 ticks a task that is not a drawing task is skipped (+0x20 and +0x24 not called)");
  check(dvt, "liminality: ... the game's three drawing tasks (CXiActorDraw, CTsZoneMap, CXiActorNameDraw) are not");
  { static void *fxvt[16], *fx[1]; fxvt[8] = (void *)GAME(0x10045A40u); fx[0] = fxvt;
    check(!skip(fx), "liminality: ... nor an effect element (+0x20 CMoElem_PrepareDraw): it puts itself in the ordering table CMoOt_Update draws and clears every frame"); }
  /* an actor: its last two ticks kept after each OnMove; drawn at their blend only inside a drawing window (here
     CXiActorDraw's +0x20: skip() before it, post() after it), the tick's values back after the window */
  static uint8_t act[0xA00] __attribute__((aligned(16))); static void *avt[16];
  avt[8] = (void *)GAME(0x100C63D0u); *(void **)act = avt;
  float *pos = (float *)(act + 0x5FC), *dir = (float *)(act + 0x61C);
  void **dlh = (void **)GAME(0x1047D578u), *keepdl = *dlh; *dlh = act; *(void **)(act + 0x54) = NULL;
  static void *dWin[1]; dWin[0] = (void *)GAME(0x10329C44u);
  *zero = 2; *alpha = 0.25f;
  pos[0] = 1; pos[1] = 2; pos[2] = 3; dir[0] = 0; dir[1] = 3.1f; dir[2] = 0;
  act[0xB0] = 0; act[0xB2] = 0; *(uint32_t *)(act + 0x6A4) = 0x10; act[0x8E8] = 1;
  check(skip(act) == 0, "liminality: an actor on a tick frame is not skipped (its OnMove runs)");
  post(act);
  check(pos[0] == 1 && pos[1] == 2 && pos[2] == 3, "... after its first OnMove: as it is (no earlier tick)");
  check(skip(act) == 0 && pos[0] == 1, "... the next tick: its OnMove runs");
  pos[0] = 5; pos[1] = 2; pos[2] = 3; dir[1] = -3.1f;      /* OnMove moved it 4 and turned it across +-pi */
  post(act);
  check(pos[0] == 5 && dir[1] == -3.1f, "... after its OnMove the game's logic sees the tick's own values (nothing blended outside the drawing)");
  skip(dWin);
  int blend1 = fabsf(pos[0] - 2.0f) < 1e-5f;                /* 1 + (5 - 1) * 0.25 */
  float wantd = 3.1f + 0.25f * (6.28318531f - 6.2f); if (wantd > 3.14159265f) wantd -= 6.28318531f;
  int dshort = fabsf(dir[1] - wantd) < 1e-4f;
  printf("    liminality: in the drawing window, at a quarter of a tick: x %.4f (want 2.0), facing %.4f (want %.4f, the short way)\n", pos[0], dir[1], wantd);
  post(dWin);
  check(blend1, "... drawn at the blend of its two ticks (x 2.0 at a quarter)");
  check(dshort, "... its facing blended the short way round across +-pi");
  check(pos[0] == 5 && dir[1] == -3.1f, "... and the tick's values back right after the drawing");
  act[0xB2] |= 1; act[0xB0] = 1; act[0x8E8] = 0;           /* what the drawing does (occluded probe; drawn) */
  *zero = 1; *alpha = 0.5f;
  int sk = skip(act);
  int flags = (act[0xB2] & 1) == 0 && act[0xB0] == 0 && act[0x8E8] == 1 && *(uint32_t *)(act + 0x6A4) == 0x10;
  check(sk == 1 && flags, "... on a frame of 0 ticks: skipped, and +0xB0 +0xB2 +0x6A4 +0x8E8 as its OnMove left them (the drawing's changes undone)");
  skip(dWin); int blend2 = fabsf(pos[0] - 3.0f) < 1e-5f; post(dWin);
  check(blend2 && pos[0] == 5, "... and drawn at that frame's blend (x 3.0 at a half), its own values back after");
  pos[0] = 6; skip(dWin); int moved = pos[0] == 6; post(dWin);
  check(moved && pos[0] == 6, "... one moved since its tick by other code is drawn where it is, and left there");
  pos[0] = 5;
  /* a warp: no blend */
  *zero = 2; skip(act); pos[0] = 500; post(act); skip(dWin); int warp = pos[0] == 500; post(dWin);
  check(warp, "... a move of 50 or more in a tick (a warp) is not blended");
  *dlh = keepdl;
  /* the weight: drawn one tick behind now at every span (12 Nov: c / span was two ticks behind at span 2) */
  { float (__cdecl *at)(float, double) = (float (__cdecl *)(float, double))GetProcAddress(sf, "sf_lim_alpha_test");
    float a1 = at ? at(1.0f, 0.25) : -1, a2 = at ? at(2.0f, 0.25) : -1, a3 = at ? at(2.0f, 0.0) : -1;
    printf("    liminality: the weight at span 1, carry 0.25: %.4f (want 0.25); span 2, carry 0.25: %.4f (want 0.625); span 2, carry 0: %.4f (want 0.5)\n", a1, a2, a3);
    check(fabsf(a1 - 0.25f) < 1e-6f && fabsf(a2 - 0.625f) < 1e-6f && fabsf(a3 - 0.5f) < 1e-6f, "liminality: the blend's weight is (span - 1 + carry) / span: one tick behind now at every span"); }
  /* the camera, around Camera_ApplyViewProjection */
  { static uint8_t app[0x400], camo[0x200]; void *keep = *(void **)GAME(0x104568FCu);
    *(void **)(app + 0x50) = camo; *(void **)GAME(0x104568FCu) = app;
    float *eye = (float *)(camo + 0x44), *tgt = (float *)(camo + 0x50);
    *zero = 2; *alpha = 0.5f;
    eye[0] = 0; tgt[0] = 10; (*cam)(0); (*cam)(1);          /* a tick: the first, no earlier one */
    eye[0] = 2; tgt[0] = 14; (*cam)(0);
    int cb = fabsf(eye[0] - 1.0f) < 1e-6f && fabsf(tgt[0] - 12.0f) < 1e-6f;
    (*cam)(1);
    int cr = eye[0] == 2 && tgt[0] == 14;
    *zero = 1; *alpha = 0.75f; (*cam)(0);
    int cz = fabsf(eye[0] - 1.5f) < 1e-6f; (*cam)(1);
    camo[0x88] = 1; *zero = 2; eye[0] = 9; (*cam)(0); int snap = eye[0] == 9; (*cam)(1); camo[0x88] = 0;
    *zero = 2; eye[0] = 9; tgt[0] = 20; (*cam)(0); (*cam)(1);       /* a tick at 9 */
    *zero = 1; eye[0] = 11; tgt[0] = 22; (*cam)(0); (*cam)(1);      /* no tick, but the camera moved (other code) */
    int keptmove = eye[0] == 11 && tgt[0] == 22;
    *(void **)GAME(0x104568FCu) = keep;
    check(cb && cr, "liminality: the camera's eye and target blended for the view (a half: 1, 12) and the tick's back after (2, 14)");
    check(cz, "... on a frame of 0 ticks blended by that frame's carry (three quarters: 1.5)");
    check(snap, "... with a snap request pending (+0x88), not blended");
    check(keptmove, "... a camera moved without a tick is never put back where the last tick left it (12 Nov: the camera stuck)"); }
  /* the ordering table (sf_main.cpp ot_*): an actor OnMove put in the table at the tick (see-through: node +0x8C8,
     callback 0x100CBD90, +0x8D4 = the actor) is put in again on a frame of 0 ticks by the game's own insert
     (0x100553C0), into a small table of the game's layout here; one that was not, is not */
  { static uint8_t a3[0xA00] __attribute__((aligned(16))), otb[0x80] __attribute__((aligned(16))), ot[0x20]; static void *a3vt[16];
    a3vt[8] = (void *)GAME(0x100C63D0u); *(void **)a3 = a3vt;
    *(void **)(ot + 4) = otb; *(int *)(ot + 8) = 3; *(float *)(ot + 0xC) = 0.0f; *(float *)(ot + 0x10) = 100.0f; *(float *)(ot + 0x14) = 0.03f;
    void **otg = (void **)GAME(0x1047BC94u), *keepot = *otg; *otg = ot;
    uint8_t *node = a3 + 0x8C8; *(float *)node = 10.0f; *(void **)(node + 8) = (void *)GAME(0x100CBD90u); *(void **)(node + 0xC) = a3;
    *(void **)(a3 + 0x8D4) = a3; *(int32_t *)(node + 0x14) = 1;      /* as OnMove leaves it on the table path */
    *zero = 2; skip(a3); post(a3);
    *(int32_t *)(node + 0x14) = 0; memset(otb, 0, sizeof otb);      /* the table drawn and cleared */
    *zero = 1; int sk = skip(a3);
    int again = *(void **)(otb + 0x20 + 4) == node && *(int32_t *)(node + 0x14) == 1;   /* bucket 1: round(10 x 0.03) + 1 (0x10055413..0x10055435) */
    check(sk == 1 && again, "ordering table: a see-through actor OnMove put in the table at the tick is put in again on a frame of 0 ticks (the game's insert)");
    *zero = 2; *(void **)(a3 + 0x8D4) = NULL; skip(a3); post(a3);   /* this tick: the draw-list path */
    *(int32_t *)(node + 0x14) = 0; memset(otb, 0, sizeof otb);
    *zero = 1; skip(a3);
    check(*(void **)(otb + 0x20 + 4) == NULL && *(int32_t *)(node + 0x14) == 0, "... one that was on the draw list at the tick is not");
    *otg = keepot; }
  /* the limbs (sf_main.cpp lim_limbs_*): a CYyMotionQue on a fake actor's model (+0x674 -> parts at +0x44, a part's
     motion lists at +0x24, queue +4 the element list), its frame +0x24 advanced as ComputeFrameAdvance does (step x
     res[+0x36] x 0.5 x speed +0x28), drawn at the blend of its last two ticks while CXiActorDraw runs, and its own
     values back after; a wrap is blended across the duration; a frame the formula does not give is not blended */
  { volatile int *tv = (volatile int *)GetProcAddress(sf, "sf_lim_test_valid");
    check(tv != NULL, "limbs: speedfix exports sf_lim_test_valid");
    static uint8_t a2[0xA00] __attribute__((aligned(16))), part[0x100], el[0x60], res[0x40]; static void *a2vt[16]; static void *hnd;
    a2vt[8] = (void *)GAME(0x100C63D0u); *(void **)a2 = a2vt;
    *(void **)(a2 + 0x674 + 0x44) = part; *(void **)(part + 4) = NULL;
    *(void **)(part + 0x24 + 0x78 + 4) = el;                /* the first queue ProcessAll walks */
    *(void **)el = (void *)GAME(0x1032A1C0u); *(void **)(el + 4) = NULL;
    hnd = res; *(void **)(el + 0x34) = &hnd; *(int16_t *)(res + 0x34) = 30; *(float *)(res + 0x36) = 2.0f;
    float *fr = (float *)(el + 0x24), *wt = (float *)(el + 0x10);
    *(float *)(el + 0x28) = 1.0f; *(int32_t *)(el + 0x2C) = 0; *(float *)(el + 0x38) = 0.0f; el[8] = 3; *(float *)(el + 0x18) = 0.1f;
    void **head = (void **)GAME(0x1047D578u), *keeph = *head; *head = a2; *(void **)(a2 + 0x54) = NULL;
    static void *dAct[1]; dAct[0] = (void *)GAME(0x10329C44u);
    *tv = 1; *zero = 2; *alpha = 0.25f;
    *fr = 10.0f; *wt = 0.2f; skip(a2); post(a2);
    *fr = 11.0f; *wt = 0.3f; skip(a2); post(a2);           /* a tick: +1 (1 x 2.0 x 0.5 x 1), the weight +0.1 */
    skip(dAct); float b1 = *fr, w1 = *wt; post(dAct); float r1 = *fr, rw1 = *wt;
    printf("    limbs: at a quarter: frame %.4f (want 10.25), weight %.4f (want 0.225); after the drawing %.4f, %.4f\n", b1, w1, r1, rw1);
    check(fabsf(b1 - 10.25f) < 1e-5f && fabsf(w1 - 0.225f) < 1e-5f, "limbs: a motion's frame and fade weight drawn at the blend of its two ticks");
    check(r1 == 11.0f && fabsf(rw1 - 0.3f) < 1e-6f, "... and the tick's own values back after CXiActorDraw");
    *alpha = 0.5f;
    *fr = 29.6f; *wt = 0.4f; skip(a2); post(a2);
    *fr = 0.6f; *wt = 0.5f; skip(a2); post(a2);             /* 29.6 + 1 wrapped at 30 */
    skip(dAct); float b2 = *fr; post(dAct);
    printf("    limbs: across the wrap at a half: frame %.4f (want 0.1)\n", b2);
    check(fabsf(b2 - 0.1f) < 1e-4f, "... a looping motion blended across its wrap (29.6 -> 0.6 at a half: 0.1)");
    *fr = 20.0f; skip(a2); post(a2);                       /* not what one tick gives: started elsewhere */
    skip(dAct); float b3 = *fr; post(dAct);
    check(b3 == 20.0f, "... a frame the game's advance does not give (a restart, a jump) is drawn as it is");
    *fr = 21.0f; skip(a2); post(a2); *fr = 99.0f;           /* changed after the tick by something else */
    skip(dAct); float b4 = *fr; post(dAct);
    check(b4 == 99.0f, "... and one changed since the tick is left alone");
    *head = keeph; *tv = 0; }
  /* step reads (sf_main.cpp step_ctx): a fraction site (sf_step_sites.h) reached in a drawing task is answered with the
     frame's fraction under Liminality; one reached outside them too gets whole ticks from then on; the getter's fork
     reports every answer */
  { int (*volatile *ctx)(uintptr_t, int) = (int (*volatile *)(uintptr_t, int))GetProcAddress(sf, "xi_step_ctx");
    volatile long *calls = (volatile long *)GetProcAddress(sf, "sf_sr_calls");
    check(ctx && *ctx && calls, "step reads: speedfix exports xi_step_ctx and sf_sr_calls");
    if (ctx && *ctx && calls) {
      volatile void **tv = (volatile void **)g_tkvt; volatile void *keepvt = *tv;
      uintptr_t zone = GAME(0x10178BD5u), other = GAME(0x1001A9B0u), plain = GAME(0x1002AD5Au);
      *tv = (void *)GAME(0x10336748u);                      /* CTsZoneMap's +0x20 running */
      int a1 = (*ctx)(zone, 1), a2 = (*ctx)(plain, 0), a3 = (*ctx)(other, 1);
      *tv = (void *)vtA; int a4 = (*ctx)(other, 1);
      *tv = (void *)GAME(0x10329C44u); int a5 = (*ctx)(other, 1), a6 = (*ctx)(zone, 1);
      *tv = NULL; int a7 = (*ctx)(plain, 0);
      check(a1 == 1 && a6 == 1, "step reads: liminality: a fraction site in a drawing task is answered with the fraction (the zone's 0x10178BD5)");
      check(a2 == 0 && a7 == 0, "... a site that is not a fraction site: whole ticks");
      check(a3 == 1 && a4 == 0 && a5 == 0, "... a fraction site reached in another task too: whole there, and whole from then on in the drawing tasks");
      check(log_has("runs outside the drawing tasks too: whole ticks there from now on"), "... the log names it");
      long c0 = *calls; uint32_t keepm = *g_xi_mode; *g_xi_mode = 1;
      ((long double (__cdecl *)(void))GAME(0x10014CF0u))(); ((long double (__cdecl *)(void))GAME(0x10014D20u))();
      *g_xi_mode = keepm;
      check(*calls - c0 == 2, "... both getters' forks report their answers (0x10014CF0, 0x10014D20)");
      *tv = keepvt; } }
  *zero = 0; *alpha = -1.0f;
  touch("speedfix.ini", "[speedfix]\nuncap=1\nframe_step=1\nliminality=0\ntask_draws=1\nstats_every=1\n");
  step_frames(70, 1, 3);
  *zero = 1; t_ecx20 = NULL; disp(taskA); *zero = 0;
  check(t_ecx20 == taskA, "liminality=0: nothing skipped, even on a frame of 0 ticks");
  check(log_has("speedfix: liminality: ") , "the stats line counts the skipped tasks");
  check(log_has("speedfix: step reads, "), "the stats line lists the step reads by where they ran");
}

/* occlusion_hold (sf_occlusion.cpp sf_occl_hold): a visible object answered hidden once is kept at its last count for
   that probe and hidden at the second hidden answer in a row; answers that show it are passed straight through */
static void occl_hold_test(HMODULE sf) {
  void (*t)(const int *, int, int *) = (void (*)(const int *, int, int *))GetProcAddress(sf, "sf_occl_hold_test");
  check(t != NULL, "occlusion_hold: speedfix exports sf_occl_hold_test");
  if (!t) return;
  static const int in[] = {0, 40, 0, 40, 0, 0, 0, 12, 0, 0, 30, 0, 50};
  static const int want[] = {0, 40, 40, 40, 40, 0, 0, 12, 12, 0, 30, 30, 50};
  int out[13]; t(in, 13, out);
  int ok = 1; for (int i = 0; i < 13; i++) ok &= out[i] == want[i];
  printf("    occlusion_hold: in  0 40 0 40 0 0 0 12 0 0 30 0 50\n    occlusion_hold: out"); for (int i = 0; i < 13; i++) printf(" %d", out[i]); printf("\n");
  check(ok, "occlusion_hold: one hidden answer after a visible one is held at the last count; two in a row hide it; a hidden "
            "object shown at once; an object never seen stays hidden");
}
int main(int argc, char **argv) {
  setvbuf(stdout, NULL, _IONBF, 0);
  int minimal = argc > 1 && !strcmp(argv[1], "minimal"), lazy = argc > 1 && !strcmp(argv[1], "lazy"), blocks = argc > 1 && !strcmp(argv[1], "blocks");
  int pacing = argc > 1 && !strcmp(argv[1], "pacing"), step = argc > 1 && !strcmp(argv[1], "step"), group = argc > 1 && !strcmp(argv[1], "group");
  HMODULE g = LoadLibraryExA("FFXiMain.dll", NULL, DONT_RESOLVE_DLL_REFERENCES);
  if (!g) { printf("FAIL: FFXiMain.dll not mapped\n"); return 2; }
  rel = (uint32_t)(uintptr_t)g - 0x10000000u;
  load_shifts(g);
  if (rel) { printf("FAIL: the test needs the game DLL at its analysed base\n"); return 2; }
  for (int k = 0; k < 97; k++) g_vt[k] = (void *)d_other;
  g_vt[7] = (void *)d_caps; g_vt[15] = (void *)d_present; g_vt[34] = (void *)d_begin; g_vt[35] = (void *)d_end; g_vt[36] = (void *)d_clear;
  g_vt[28] = (void *)d_copyrects; g_vt[37] = (void *)d_settransform; g_vt[83] = (void *)d_setss;
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
  g_vt[24] = (void *)d_createib; g_vt[85] = (void *)d_setindices;
  for (int k = 0; k < 14; k++) g_ivt[k] = (void *)d_other;
  g_ivt[1] = (void *)i_addref; g_ivt[2] = (void *)i_release; g_ivt[11] = (void *)i_lock; g_ivt[12] = (void *)i_unlock; g_ivt[13] = (void *)i_getdesc;
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
  else if (step) touch("speedfix.ini", "[speedfix]\nframe_step=2\nstats_every=1\ntask_draws=1\n");
  else if (group) touch("speedfix.ini", "[speedfix]\nskin_merge=2\nstats_every=1\n");
  else if (getenv("SF_HOST_INI")) touch("speedfix.ini", getenv("SF_HOST_INI"));   /* a test's own settings (test_speedfix.py) */
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
    if (group) {}
    else if (!minimal && !lazy && !blocks && !pacing && !step) check(forked, "skin_merge=1 at the start: SkinAndDrawV2's entry jumps into speedfix.dll (the fork)");
    /* (14 Nov) sphere_shine (default 1) is drawn in the fork too: with skin_merge off it is still in, and every switch
       of it off draws as the game (test_skinmerge.py: the gate with them off PASSes) */
    else check(forked, "skin_merge off, sphere_shine on (the default) at the start: SkinAndDrawV2's entry jumps into speedfix.dll (the fork)");
  }
  if (pacing) { frames(1); return pacing_test(); }
  if (step) { frames(1); return step_test(); }
  if (group) { frames(1); return group_test(); }
  if (minimal) {
    flare_test();
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
    touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\nocclusion_stagger=8\nocclusion_late=1\nocclusion_hold=0\nstats_every=1\n"); frames(65);
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
    /* occlusion_nowait: with d3d8on11's do-not-wait flag the late read never waits: while no picture has finished the
       object's last answer is given again and up to three pictures are in flight; then the newest finished one answers */
    { void (*nw)(unsigned) = (void (*)(unsigned))GetProcAddress(GetModuleHandleA("speedfix.dll"), "sf_occl_test_nowait");
      check(nw != NULL, "occlusion_nowait: speedfix exports sf_occl_test_nowait");
      if (nw) {
        struct { short w[2]; void *s; } q3 = {{0, 0}, &g_gameSurf};
        nw(0x4000); g_surf_notyet = 1;
        LONG l0 = g_nLock, rt0 = g_nRT, ny0 = g_nNotYet;
        begin();
        picture(1); int n1 = rb(&q3, 0);         /* no answer yet: the game's readback (256); picture 1 in flight */
        LONG l1 = g_nLock;
        picture(0); int n2 = rb(&q3, 0);         /* not finished: 256 again; picture 0 in flight */
        picture(2); int n3 = rb(&q3, 0);         /* 256 again; the half picture in flight */
        picture(1); int n4 = rb(&q3, 0);         /* 256 again; all three in flight: this picture is not copied */
        LONG l4 = g_nLock, rt4 = g_nRT, ny4 = g_nNotYet;
        g_surf_notyet = 0;
        picture(0); int n5 = rb(&q3, 0);         /* all finished: the newest one's answer (the half picture) */
        picture(1); int n6 = rb(&q3, 0);         /* the picture copied at n5 (nothing drawn): 0 */
        end(); present();
        check(n1 == 256 && l1 - l0 == 1, "occlusion_nowait: a first probe is answered by the game's readback");
        check(n2 == 256 && n3 == 256 && n4 == 256 && l4 == l1 && ny4 - ny0 == 6 && rt4 - rt0 == 3,
              "... while no picture has finished: the last answer again, no lock waits (6 told not yet), three surfaces in flight and no fourth");
        check(n5 == truth[2] && n6 == 0, "... then the newest finished picture answers, counted as the game counts, and the next probe's follows");
        g_surf_notyet = 1; frames(40);            /* in flight longer than OL_MAX_AGE */
        begin(); picture(2); int n7 = rb(&q3, 0); end(); present();
        check(n7 == truth[2], "... a picture in flight for more than 32 frames is dropped: the game's answer");
        g_surf_notyet = 0; nw(0);
        touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\nocclusion_stagger=8\nocclusion_late=1\nocclusion_hold=0\nocclusion_nowait=0\nstats_every=1\n"); frames(65);
        nw(0x4000); g_surf_notyet = 1; LONG ny8 = g_nNotYet;
        begin(); picture(1); rb(&q3, 0); picture(0); int n9 = rb(&q3, 0); end(); present();
        check(g_nNotYet == ny8 && n9 == 256, "occlusion_nowait=0: the flag is not passed; the previous picture answers as before");
        g_surf_notyet = 0; nw(0);
        /* occlusion_first=2: a first probe is answered "visible" without the game's readback, then its own picture's */
        touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\nocclusion_stagger=8\nocclusion_late=1\nocclusion_hold=0\nocclusion_first=2\nstats_every=1\n"); frames(65);
        struct { short w[2]; void *s; } q4 = {{0, 0}, &g_gameSurf};
        LONG lf = g_nLock;
        begin(); picture(0); int f1 = rb(&q4, 0); LONG lf1 = g_nLock; picture(1); int f2 = rb(&q4, 0); end(); present();
        check(f1 == 256 && lf1 == lf && f2 == 0, "occlusion_first=2: a first probe is answered visible with no lock at all; the next probe answers its picture (nothing drawn: 0)");
        touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\nocclusion_stagger=8\nocclusion_late=1\nocclusion_hold=0\nstats_every=1\n"); frames(65);
      }
    }
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
  gpu_skin_test();
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
  /* the draw call sites speedfix hands d3d8on11 (9 Nov: in game they were the kit's pad): the kit's front thunk returns
     every hooked call through its pad and keeps the caller's address; xikit_device_caller gives it back, so every site
     speedfix saw on the render thread is resolved (sf_site_seen: seen, resolved, in FFXiMain.dll) */
  { volatile LONG *ss = (volatile LONG *)GetProcAddress(GetModuleHandleA("speedfix.dll"), "sf_site_seen");
    check(GetProcAddress(GetModuleHandleA("xikit.dll"), "xikit_device_caller") != NULL, "the kit exports xikit_device_caller");
    if (ss) printf("    draw call sites: %ld seen by speedfix, %ld resolved past the kit's pad, %ld in FFXiMain.dll\n", ss[0], ss[1], ss[2]);
    check(ss && ss[0] > 0 && ss[1] == ss[0], "every draw call site speedfix saw is the caller's, not the kit's pad (resolved through xikit_device_caller)"); }

  printf(fails ? "FAILED (%d)\n" : "ALL OK\n", fails);
  return fails ? 1 : 0;
}


/* gpu_skin (sf_skin.cpp, kit/d3d8on11/d8_skin.h): the job speedfix hands the device, checked against the game's own
   skinning. A mesh object is laid out here as Model_SkinVerticesCpu (0x1002F1D0) reads it: 20 one-bone and 20 two-bone
   vertices, 3 duplicates, a bone list, a mirrored source. The game's routine skins it into four arrays. Then
   sf_gpu_skin is called with a function that keeps its jobs, and each job is skinned here by the sums d3d8on11's
   vertex shader makes from the 64-byte source d3d8on11 builds (Dev::skin): positions and normals must equal the game's
   within float rounding, for the plain and the mirrored set, with the game's x87 loop and with its SSE loop. */
typedef struct { unsigned size; void *pos, *nrm, *key; unsigned variant; const float *src; unsigned count1, count2;
                 const unsigned short *words; unsigned shift; const unsigned short *bonelist; const unsigned short *dups; unsigned ndups;
                 const float *matrices; } GsJob;
static GsJob g_gsjob[4]; static int g_gsn;
static int __stdcall gs_take(const GsJob *j) { if (g_gsn < 4) g_gsjob[g_gsn] = *j; g_gsn++; return 1; }
static void gs_skin(const GsJob *j, float *P, float *N) {
  unsigned n = j->count1 + j->count2 + j->ndups;
  for (unsigned k = 0; k < n; k++) {
    unsigned q = k < j->count1 + j->count2 ? k : j->dups[k - j->count1 - j->count2];
    float pa[4] = {0, 0, 0, 0}, pb[4] = {0, 0, 0, 0}, na[3] = {0, 0, 0}, nb[3] = {0, 0, 0}; unsigned ba, bb;
    #define GSB(w) (j->bonelist ? j->bonelist[((w) >> j->shift) & 0x7F] : (((w) >> j->shift) & 0x7F))
    if (q < j->count1) { const float *f = j->src + 6 * q; pa[0] = f[0]; pa[1] = f[1]; pa[2] = f[2]; pa[3] = 1; na[0] = f[3]; na[1] = f[4]; na[2] = f[5]; ba = bb = GSB(j->words[2 * q]); }
    else { const float *f = j->src + 6 * j->count1 + 14 * (q - j->count1);
      pa[0] = f[0]; pa[1] = f[2]; pa[2] = f[4]; pa[3] = f[6]; pb[0] = f[1]; pb[1] = f[3]; pb[2] = f[5]; pb[3] = f[7];
      na[0] = f[8]; na[1] = f[10]; na[2] = f[12]; nb[0] = f[9]; nb[1] = f[11]; nb[2] = f[13];
      ba = GSB(j->words[2 * q]); bb = GSB(j->words[2 * q + 1]); }
    #undef GSB
    const float *A = j->matrices + 16 * ba, *B = j->matrices + 16 * bb;
    for (int c = 0; c < 3; c++) {
      P[3 * k + c] = pa[0] * A[c] + pa[1] * A[4 + c] + pa[2] * A[8 + c] + pa[3] * A[12 + c] + pb[0] * B[c] + pb[1] * B[4 + c] + pb[2] * B[8 + c] + pb[3] * B[12 + c];
      N[3 * k + c] = na[0] * A[c] + na[1] * A[4 + c] + na[2] * A[8 + c] + nb[0] * B[c] + nb[1] * B[4 + c] + nb[2] * B[8 + c];
    }
  }
}
static void gpu_skin_test(void) {
  enum { C1 = 20, C2 = 20, ND = 3, NV = C1 + C2 + ND };
  HMODULE sf = GetModuleHandleA("speedfix.dll");
  int (*job)(void *, int) = (int (*)(void *, int))GetProcAddress(sf, "sf_gpu_skin");
  void (*settest)(void *) = (void (*)(void *))GetProcAddress(sf, "sf_gpu_skin_test");
  check(job && settest, "gpu_skin: speedfix exports sf_gpu_skin and sf_gpu_skin_test");
  if (!job || !settest) return;
  static uint8_t self[0x80], v10[0xA0]; static float src[C1 * 6 + C2 * 14 + 16], msrc[C1 * 6 + C2 * 14 + 16], mats[40 * 16];
  static uint16_t words[2 * (C1 + C2)], blist[8] = {3, 7, 12, 20, 31, 5, 9, 1}, dups[ND] = {2, 25, 39}, counts[2] = {C1, C2};
  static float out[4][NV * 3 + 8], ref[2][NV * 3];
  unsigned seed = 12345;
  #define RNDF() (seed = seed * 1103515245u + 12345u, (float)((seed >> 8) & 0xFFFF) / 32768.0f - 1.0f)
  for (unsigned i = 0; i < sizeof src / 4; i++) { src[i] = RNDF(); msrc[i] = RNDF(); }
  for (unsigned i = 0; i < sizeof mats / 4; i++) mats[i] = RNDF();
  for (int i = 0; i < 2 * (C1 + C2); i++) { seed = seed * 1103515245u + 12345u; words[i] = (uint16_t)(((seed >> 9) & 7) | (((seed >> 13) & 7) << 7)); }
  memset(self, 0, sizeof self); memset(v10, 0, sizeof v10);
  *(uint16_t *)(self + 0x32) = 0x80; self[0x34] = 1;                    /* a bone list; mirrored */
  *(void **)(self + 0x3C) = blist; *(void **)(self + 0x42) = counts; *(void **)(self + 0x48) = words; *(void **)(self + 0x5E) = v10;
  *(void **)(v10 + 0x6E) = src; *(void **)(v10 + 0x76) = msrc; *(void **)(v10 + 0x5C) = dups; *(uint32_t *)(v10 + 0x60) = ND;
  *(void **)(v10 + 0x8E) = out[0]; *(void **)(v10 + 0x92) = out[1]; *(void **)(v10 + 0x96) = out[2]; *(void **)(v10 + 0x9A) = out[3];
  *(void **)(v10 + 0x40) = (void *)0x1111; *(void **)(v10 + 0x44) = (void *)0x2222; *(void **)(v10 + 0x48) = (void *)0x3333;
  *(void **)(v10 + 0x50) = (void *)0x4444; *(void **)(v10 + 0x54) = (void *)0x5555;
  void *old774 = *(void **)GAME(0x1045B774u); uint32_t old618 = *(uint32_t *)GAME(0x1045B618u); void *old550 = *(void **)GAME(0x10457550u);
  *(void **)GAME(0x1045B774u) = NULL; *(uint32_t *)GAME(0x1045B618u) = C1 | (C2 << 16); *(void **)GAME(0x10457550u) = mats;
  typedef void (__attribute__((thiscall)) *SkinFn)(void *);
  settest((void *)gs_take);
  for (int sse = 0; sse < 2; sse++) {
    uint8_t keep = g_cpu[9]; g_cpu[9] = (uint8_t)sse;
    memset(out, 0, sizeof out);
    ((SkinFn)GAME(0x1002F1D0u))(self);
    g_cpu[9] = keep;
    g_gsn = 0; int took = job(self, (int)(uintptr_t)v10);
    double worst = 0; int okjobs = took == 1 && g_gsn == 2;
    for (int v = 0; okjobs && v < 2; v++) {
      const GsJob *j = &g_gsjob[v];
      okjobs = j->variant == (unsigned)v && j->pos == (void *)(uintptr_t)(v ? 0x4444 : 0x2222) && j->nrm == (void *)(uintptr_t)(v ? 0x5555 : 0x3333) &&
               j->key == (void *)0x1111 && j->count1 == C1 && j->count2 == C2 && j->ndups == ND;
      if (!okjobs) break;
      gs_skin(j, ref[0], ref[1]);
      for (int k = 0; k < NV * 3; k++) { double e1 = fabs((double)ref[0][k] - out[v * 2][k]), e2 = fabs((double)ref[1][k] - out[v * 2 + 1][k]); if (e1 > worst) worst = e1; if (e2 > worst) worst = e2; }
    }
    printf("    gpu_skin: the game's %s loop against the job's sums: jobs taken %d (%d handed), worst difference %.3g over %d vertices x 2 sets\n",
           sse ? "SSE" : "x87", took, g_gsn, worst, NV);
    char what[200]; snprintf(what, sizeof what, "gpu_skin: the job equals the game's own skinning (0x1002F1D0, its %s loop): positions and normals of both sets within 1e-4", sse ? "SSE" : "x87");
    check(okjobs && worst < 1e-4, what);
  }
  /* the second route ([0x1045B774] set) and counts that do not agree are left to the game */
  *(void **)GAME(0x1045B774u) = (void *)self; g_gsn = 0; int t1 = job(self, (int)(uintptr_t)v10);
  *(void **)GAME(0x1045B774u) = NULL; counts[0] = C1 + 1; int t2 = job(self, (int)(uintptr_t)v10); counts[0] = C1;
  check(t1 == 0 && t2 == 0 && g_gsn == 0, "gpu_skin: the routine's second route, and counts that do not agree, are left to the game (no job)");
  settest(NULL);
  *(void **)GAME(0x1045B774u) = old774; *(uint32_t *)GAME(0x1045B618u) = old618; *(void **)GAME(0x10457550u) = old550;
  #undef RNDF
}

/* flare_fix (14 Nov, sf_device.cpp): a DrawPrimitive with the glow's signature (stride 36, at most 8 vertices, depth
   test on, no depth write, SRCALPHA / ONE) reaches the device between a WORLD moved -0.4 along its local z (row 3 minus
   0.4 x row 2) and the game's WORLD again; one with another blend reaches it with no transform around it */
/* (d3d8.h values: ZENABLE 7, ZWRITEENABLE 14, ALPHABLENDENABLE 27, SRCBLEND 19, DESTBLEND 20; SRCALPHA 5, INVSRCALPHA 6, ONE 2; WORLD 256; TRIANGLELIST 4) */
static void flare_test(void) {
  typedef HRESULT (__stdcall *SS)(void *, UINT, void *, UINT); typedef HRESULT (__stdcall *XF)(void *, DWORD, const float *);
  typedef HRESULT (__stdcall *DP)(void *, DWORD, UINT, UINT);
  float w[16] = {2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 5, 0, 10, 20, 30, 1};
  begin();
  setrs(7, 1); setrs(14, 0); setrs(27, 1); setrs(19, 5);
  setrs(20, 2);
  DEV(83, SS)(&g_dev, 0, (void *)0x1234, 36); DEV(37, XF)(&g_dev, 256, w);
  LONG from = g_n; DEV(70, DP)(&g_dev, 4, 0, 2);
  int seen = 0, ok = 0;
  for (LONG i = from; i < g_n; i++) {
    const Call *c = &g_log[i];
    if (seen == 0 && c->slot == 37 && c->a[0] == 256 && c->nbytes == 64) {
      float m[16]; memcpy(m, c->bytes, 64);
      ok = m[12] == 10 && m[13] == 20 && m[14] == 30 - 0.4f * 5 && m[0] == 2 && m[10] == 5; seen = 1;
    } else if (seen == 1 && c->slot == 70) seen = 2;
    else if (seen == 2 && c->slot == 37 && c->a[0] == 256) { float m[16]; memcpy(m, c->bytes, 64); if (!memcmp(m, w, 64)) seen = 3; }
  }
  check(seen == 3 && ok, "flare_fix: the glow draw goes out under WORLD moved -0.4 along its local z, then the game's WORLD again");
  setrs(20, 6);
  from = g_n; DEV(70, DP)(&g_dev, 4, 0, 2);
  check(count(37, from) == 0 && count(70, from) == 1, "flare_fix: a draw with another blend is drawn as the game draws it (no transform around it)");
  end(); present();
}
