/* sf_host.c -- speedfix (kit/speedfix) under Wine with the real FFXiMain.dll and the kit (test_speedfix.py runs it).
   Like kit/xikit/test/host.c: the game DLL is mapped without running it, the game gets a fake CDx whose device
   records what reaches it, xikit.dll loads speedfix.dll as its mod, and this program plays the render thread. Every
   check prints "ok: ..." or "FAIL: ...". */
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
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
static HRESULT __stdcall d_clear(void *d, DWORD n, const void *r, DWORD f, DWORD c, float z, DWORD s) { rec(36, n, f, c, 0, 0, 0); return 0; }
static DWORD g_nvs = 0x101;                      /* shader handles: odd, as D3D8's are (bit 0 tells them from FVFs) */
static HRESULT __stdcall d_createvs(void *d, const DWORD *decl, const DWORD *fn, DWORD *h, DWORD us) {
  *h = g_nvs; g_nvs += 2; rec(75, *h, (uint32_t)(uintptr_t)fn, us, 0, 0, 0); return 0; }
static HRESULT __stdcall d_deletevs(void *d, DWORD h) { rec(78, h, 0, 0, 0, 0, 0); return 0; }
/* fake 16x16 surfaces for the occlusion readback (occlusion_async): CopyRects copies their pixels, LockRect gives them */
typedef struct { void **vt; uint32_t px[256]; DWORD pool; LONG refs; } FSurf;
static void *g_svt[11];
static FSurf g_boxSurf, g_gameSurf;             /* the box texture's level 0; the game's lockable target at [mng+0x18] */
static LONG g_nImage, g_nRT, g_nSurfRelease, g_nLock;
static ULONG __stdcall s_addref(FSurf *s) { return ++s->refs; }
static ULONG __stdcall s_release(FSurf *s) { if (s != &g_boxSurf && s != &g_gameSurf) g_nSurfRelease++; return --s->refs; }
static HRESULT __stdcall s_getdesc(FSurf *s, uint32_t *d) { memset(d, 0, 32); d[0] = 21; d[1] = 1; d[3] = s->pool; d[6] = 16; d[7] = 16; return 0; }
static HRESULT __stdcall s_lock(FSurf *s, struct { INT Pitch; void *pBits; } *lr, const RECT *r, DWORD f) { g_nLock++; lr->Pitch = 64; lr->pBits = s->px; return 0; }
static HRESULT __stdcall s_unlock(FSurf *s) { return 0; }
static FSurf *new_surf(DWORD pool) { FSurf *s = (FSurf *)calloc(1, sizeof *s); s->vt = g_svt; s->pool = pool; s->refs = 1; return s; }
static HRESULT __stdcall d_creatert(void *d, UINT w, UINT h, DWORD f, DWORD ms, BOOL lk, FSurf **o) { g_nRT++; *o = new_surf(0); return 0; }
static HRESULT __stdcall d_createimage(void *d, UINT w, UINT h, DWORD f, FSurf **o) { g_nImage++; *o = new_surf(2); return 0; }
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
static void touch(const char *path, const char *body) { Sleep(30); FILE *f = fopen(path, "w"); fputs(body, f); fclose(f); }

/* the stagger's call: ESI = the actor (0x100CC584 `mov edi, [esi+0x9F0]`) */
extern int call_with_esi(void *fn, void *esi);
__asm__(".globl _call_with_esi\n_call_with_esi:\n  push %esi\n  mov 12(%esp), %esi\n  call *8(%esp)\n  pop %esi\n  ret\n");
static void *call_target(uint32_t site) { uint8_t *p = (uint8_t *)GAME(site); return p[0] == 0xE8 ? p + 5 + *(int32_t *)(p + 1) : NULL; }
static uint8_t g_cpu[0x24];        /* [0x104566E8]: the game's CPU object (0x1000D380); +9 its SSE flag (sse_skinning) */
static uint8_t g_cam[0x40];        /* [0x104568FC] -> +0x34: the frame counter (GetFrameCounter, 0x10014D50) */

static DWORD WINAPI other_thread(LPVOID u) { setrs(7, 1); return 0; }

int main(int argc, char **argv) {
  setvbuf(stdout, NULL, _IONBF, 0);
  int minimal = argc > 1 && !strcmp(argv[1], "minimal"), lazy = argc > 1 && !strcmp(argv[1], "lazy");
  HMODULE g = LoadLibraryExA("FFXiMain.dll", NULL, DONT_RESOLVE_DLL_REFERENCES);
  if (!g) { printf("FAIL: FFXiMain.dll not mapped\n"); return 2; }
  rel = (uint32_t)(uintptr_t)g - 0x10000000u;
  if (rel) { printf("FAIL: the test needs the game DLL at its analysed base\n"); return 2; }
  for (int k = 0; k < 97; k++) g_vt[k] = (void *)d_other;
  g_vt[7] = (void *)d_caps; g_vt[15] = (void *)d_present; g_vt[34] = (void *)d_begin; g_vt[35] = (void *)d_end; g_vt[36] = (void *)d_clear;
  g_vt[28] = (void *)d_copyrects; g_vt[37] = (void *)d_settransform;
  g_vt[14] = (void *)d_reset; g_vt[25] = (void *)d_creatert; g_vt[27] = (void *)d_createimage;
  for (int k = 0; k < 11; k++) g_svt[k] = (void *)d_other;
  g_svt[1] = (void *)s_addref; g_svt[2] = (void *)s_release; g_svt[8] = (void *)s_getdesc; g_svt[9] = (void *)s_lock; g_svt[10] = (void *)s_unlock;
  for (int k = 0; k < 17; k++) g_xvt[k] = (void *)d_other;
  g_xvt[15] = (void *)x_getsurflevel;
  g_boxSurf.vt = g_gameSurf.vt = g_svt; g_boxSurf.refs = g_gameSurf.refs = 1;
  *(void **)(g_mng + 0xC) = &g_boxTex; *(void **)(g_mng + 0x18) = &g_gameSurf;
  *(void **)(g_modx + 0xEBC) = &g_dev;
  g_vt[50] = (void *)d_setrs; g_vt[51] = (void *)d_getrs; g_vt[61] = (void *)d_settex; g_vt[63] = (void *)d_settss;
  g_vt[72] = (void *)d_dpup; g_vt[76] = (void *)d_setvs; g_vt[75] = (void *)d_createvs; g_vt[78] = (void *)d_deletevs;
  for (int k = 0; k < 16; k++) g_tvt[k] = (void *)d_other;
  g_tvt[10] = (void *)t_gettype; g_tvt[1] = (void *)t_addref; g_tvt[2] = (void *)t_release;
  g_rs[8] = 3; g_rs[9] = 2;                        /* the device's defaults: FILL_SOLID, SHADE_GOURAUD */
  *(void **)(g_cdx + 0xC) = &g_dev;
  DWORD old; VirtualProtect((void *)GAME(0x10000000u), 0x1000000, PAGE_EXECUTE_READWRITE, &old);
  *(void **)GAME(G_PCDX) = g_cdx;
  *(void **)GAME(0x104568FCu) = g_cam;
  *(void **)GAME(0x104566E8u) = g_cpu;                /* +9 = 0: the game found no SSE (as on the owner's machine) */
  uint32_t tex_before = *(uint32_t *)GAME(0x10456AD0u), tier_before = *(uint32_t *)GAME(0x1006CDDCu);
  touch("xikit.ini", "[xikit]\nlog_folder=0\nswap=0\ncount_calls=1\nreport_every=60\nhotkey=0\nmods=speedfix.dll\n");
  DeleteFileA("speedfix.ini");
  /* the full run starts with batching on (every device method hooked); the minimal run with the defaults (only the
     cached setters and the methods around their state hooked) */
  if (lazy) touch("speedfix.ini", "[speedfix]\nlazy_state=1\nvs_dedupe=1\n");
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
    if (!minimal && !lazy) check(forked, "skin_merge=1 at the start: SkinAndDrawV2's entry jumps into speedfix.dll (the fork)");
    else check(!forked, "skin_merge off at the start: SkinAndDrawV2 is the game's code");
  }
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
  /* occlusion_async: the game's own readback (0x1006C8B0, run here on fake surfaces) gives the truth for each
     picture; the delayed path must give the previous probe's, per query object, counted the same way */
  { typedef int (__attribute__((thiscall)) *Rb)(void *, int); Rb rb = (Rb)call_target(0x1006C4D7u);
    *(void **)GAME(0x1047C004u) = g_mng; *(void **)GAME(0x1047BFA8u) = g_modx;
    struct { short w[2]; void *s; } q1 = {{0, 0}, &g_gameSurf}, q2 = {{0, 0}, &g_gameSurf};
    int truth[3];
    touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\nocclusion_stagger=8\noccl_test=1\n"); frames(65);
    begin(); for (int k = 0; k < 3; k++) { picture(k); truth[k] = rb(&q1, 0); } end(); present();
    printf("info: the game's readback answers %d (nothing drawn), %d (all), %d (top half)\n", truth[0], truth[1], truth[2]);
    check(truth[0] == 0 && truth[1] == 256 && truth[2] > 0 && truth[2] < 256, "the game's own readback runs on the fake surfaces: 0, 256 and a part");
    touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\nocclusion_stagger=8\nocclusion_async=1\nstats_every=1\n"); frames(65);
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
    check(a1 == 256 && locks1 - locks0 == 1 + 0, "occlusion_async=1: a first probe is answered by the game's readback (its LockRect)");
    check(a2 == 256 && a3 == 0 && a4 == truth[2], "... later probes answer the previous probe's picture, counted exactly as the game counts (the half picture: the game's value)");
    check(b1 == 0 && a5 == 256, "... per query object: a second object starts with the game's answer, and the first keeps its own previous picture");
    check(g_nImage == 2 && g_nRT == 0, "... each object's copy goes to its own image surface (CreateImageSurface, two objects)");
    frames(40);                                  /* older than OA_MAX_AGE (32 frames) */
    begin(); picture(2); int s1 = rb(&q1, 0); end(); present();
    check(s1 == truth[2], "... a previous picture older than 32 frames is not used: the game's answer for this frame's picture");
    LONG rel0 = g_nSurfRelease;
    DEV(14, HRESULT (__stdcall *)(void *, void *))(&g_dev, 0);
    check(g_nSurfRelease - rel0 == 2, "... Reset releases its surfaces first (both)");
    begin(); picture(1); int r1 = rb(&q1, 0); end(); present();
    check(r1 == 256 && g_nImage == 3, "... after Reset: the game's answer, and a new surface");
    touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\nocclusion_stagger=8\nocclusion_async=2\nstats_every=1\n"); frames(65);
    begin(); picture(0); int m1 = rb(&q1, 0); picture(1); int m2 = rb(&q1, 0); end(); present();
    check(g_nRT >= 1 && m2 == 0 && (m1 == 0 || m1 == 256), "occlusion_async=2: a lockable render target of its own (CreateRenderTarget), the delayed answer");
    touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\nocclusion_stagger=8\nocclusion_async=3\nstats_every=1\n"); frames(65);
    begin(); picture(1); int t1 = rb(&q1, 0); picture(0); int t2 = rb(&q1, 0); picture(2); int t3 = rb(&q1, 0); end(); present();
    check(t1 == 256 && t2 == 0 && t3 == truth[2], "occlusion_async=3 (measure only): every answer is the game's");
    for (int i = 0; i < 300 && !log_has("occlusion_async=3: of"); i++) { frames(1); Sleep(10); }   /* stats_every=1: once a second */
    check(log_has("occlusion_async=3: of") && log_has("occlusion_async: its image surfaces: format 21, pool 2"), "... the differences are logged; the image surface's pool is logged from GetDesc (2, system memory, as this fake says)");
    check(log_has("speedfix: readback time: the game's readback") && log_has("occlusion_async's copy"), "... the time in the game's readback and in its copy and read is logged");
    touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\nocclusion_stagger=8\noccl_test=0\n"); frames(65);
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
