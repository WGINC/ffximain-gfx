/* sf_host.c -- speedfix (kit/speedfix) under Wine with the real FFXiMain.dll and the kit (test_speedfix.py runs it).
   Like kit/xikit/test/host.c: the game DLL is mapped without running it, the game gets a fake CDx whose device
   records what reaches it, xikit.dll loads speedfix.dll as its mod, and this program plays the render thread. Every
   check prints "ok: ..." or "FAIL: ...". */
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

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
static void __stdcall d_other(void) { printf("FAIL: a device method the test does not provide was called\n"); ExitProcess(3); }
static void *g_vt[97];
static struct { void **vt; } g_dev = { g_vt };
static uint8_t g_cdx[0x1000];
/* a texture the game binds: with batching on speedfix asks its type (IDirect3DBaseTexture8::GetType, slot 10) to
   guard its LockRect; this one answers D3DRTYPE_SURFACE (1), so no vtable of it is changed */
static DWORD __stdcall t_gettype(void *t) { return 1; }
static void *g_tvt[16]; static struct { void **vt; } g_tex1 = { g_tvt };

#define DEV(slot, T) ((T)g_dev.vt[slot])
static void present(void) { DEV(15, HRESULT (__stdcall *)(void *, void *, void *, void *, void *))(&g_dev, 0, 0, 0, 0); }
static void begin(void) { DEV(34, HRESULT (__stdcall *)(void *))(&g_dev); }
static void end(void) { DEV(35, HRESULT (__stdcall *)(void *))(&g_dev); }
static void setrs(DWORD s, DWORD v) { DEV(50, HRESULT (__stdcall *)(void *, DWORD, DWORD))(&g_dev, s, v); }
static void settss(DWORD st, DWORD ty, DWORD v) { DEV(63, HRESULT (__stdcall *)(void *, DWORD, DWORD, DWORD))(&g_dev, st, ty, v); }
static void settex(DWORD st, void *t) { DEV(61, HRESULT (__stdcall *)(void *, DWORD, void *))(&g_dev, st, t); }
static void setvs(DWORD h) { DEV(76, HRESULT (__stdcall *)(void *, DWORD))(&g_dev, h); }
static void clear(void) { DEV(36, HRESULT (__stdcall *)(void *, DWORD, const void *, DWORD, DWORD, float, DWORD))(&g_dev, 0, 0, 1, 0, 1.0f, 0); }
static void dpup(DWORD t, UINT n, const void *data, UINT stride) { DEV(72, HRESULT (__stdcall *)(void *, DWORD, UINT, const void *, UINT))(&g_dev, t, n, data, stride); }
static void frames(int k) { for (int i = 0; i < k; i++) { begin(); end(); present(); } }
static int count(int slot, LONG from) { int c = 0; for (LONG i = from; i < g_n; i++) c += g_log[i].slot == slot; return c; }
static Call *last(int slot) { for (LONG i = g_n - 1; i >= 0; i--) if (g_log[i].slot == slot) return &g_log[i]; return NULL; }
static void touch(const char *path, const char *body) { Sleep(30); FILE *f = fopen(path, "w"); fputs(body, f); fclose(f); }

/* the stagger's call: ESI = the actor (0x100CC584 `mov edi, [esi+0x9F0]`) */
extern int call_with_esi(void *fn, void *esi);
__asm__(".globl _call_with_esi\n_call_with_esi:\n  push %esi\n  mov 12(%esp), %esi\n  call *8(%esp)\n  pop %esi\n  ret\n");
static void *call_target(uint32_t site) { uint8_t *p = (uint8_t *)GAME(site); return p[0] == 0xE8 ? p + 5 + *(int32_t *)(p + 1) : NULL; }
static uint8_t g_cam[0x40];        /* [0x104568FC] -> +0x34: the frame counter (GetFrameCounter, 0x10014D50) */

static DWORD WINAPI other_thread(LPVOID u) { setrs(7, 1); return 0; }

int main(int argc, char **argv) {
  setvbuf(stdout, NULL, _IONBF, 0);
  int minimal = argc > 1 && !strcmp(argv[1], "minimal");
  HMODULE g = LoadLibraryExA("FFXiMain.dll", NULL, DONT_RESOLVE_DLL_REFERENCES);
  if (!g) { printf("FAIL: FFXiMain.dll not mapped\n"); return 2; }
  rel = (uint32_t)(uintptr_t)g - 0x10000000u;
  if (rel) { printf("FAIL: the test needs the game DLL at its analysed base\n"); return 2; }
  for (int k = 0; k < 97; k++) g_vt[k] = (void *)d_other;
  g_vt[7] = (void *)d_caps; g_vt[15] = (void *)d_present; g_vt[34] = (void *)d_begin; g_vt[35] = (void *)d_end; g_vt[36] = (void *)d_clear;
  g_vt[50] = (void *)d_setrs; g_vt[51] = (void *)d_getrs; g_vt[61] = (void *)d_settex; g_vt[63] = (void *)d_settss;
  g_vt[72] = (void *)d_dpup; g_vt[76] = (void *)d_setvs;
  for (int k = 0; k < 16; k++) g_tvt[k] = (void *)d_other;
  g_tvt[10] = (void *)t_gettype;
  g_rs[8] = 3; g_rs[9] = 2;                        /* the device's defaults: FILL_SOLID, SHADE_GOURAUD */
  *(void **)(g_cdx + 0xC) = &g_dev;
  DWORD old; VirtualProtect((void *)GAME(0x10000000u), 0x1000000, PAGE_EXECUTE_READWRITE, &old);
  *(void **)GAME(G_PCDX) = g_cdx;
  *(void **)GAME(0x104568FCu) = g_cam;
  uint32_t tex_before = *(uint32_t *)GAME(0x10456AD0u), tier_before = *(uint32_t *)GAME(0x1006CDDCu);
  touch("xikit.ini", "[xikit]\nswap=0\ncount_calls=1\nreport_every=60\nhotkey=0\nmods=speedfix.dll\n");
  DeleteFileA("speedfix.ini");
  /* the full run starts with batching on (every device method hooked); the minimal run with the defaults (only the
     cached setters and the methods around their state hooked) */
  if (!minimal) touch("speedfix.ini", "[speedfix]\nbatch_up_draws=1\n");
  if (!LoadLibraryA("xikit.dll")) { printf("FAIL: xikit.dll not loaded\n"); return 2; }
  for (int i = 0; i < 300 && g_dev.vt == g_vt; i++) Sleep(20);        /* the kit puts its vtable copy on the device */
  check(g_dev.vt != g_vt, "the kit hooked the device");
  frames(3);
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

  printf(fails ? "FAILED (%d)\n" : "ALL OK\n", fails);
  return fails ? 1 : 0;
}
