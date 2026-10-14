/* host.c -- a stand-in for pol.exe to test xikit under Wine (test_xikit.py runs it).

   It maps the real FFXiMain.dll without running its DllMain (DONT_RESOLVE_DLL_REFERENCES; the loader still
   applies its relocations), optionally after occupying 0x10000000 so the DLL is loaded elsewhere, gives the game a
   fake CDx whose device has a fake vtable, loads xikit.dll and then plays the render thread: it calls the
   device's Present (which the kit hooks) and calls game functions through their original entries. Each check
   prints "ok: ..." or "FAIL: ...". */
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

#define G_PCDX 0x1045666Cu
#define SETCOLOR 0x100A4830u          /* CXiSkeletonActor::SetColor: this+0x660 = arg */
#define GETCOLOR 0x100A4840u          /* CXiSkeletonActor::GetColor: returns this+0x660 */
#define GAMMA    0x10002BE0u          /* Gfx_IsGammaSupported: [g_pCDx] ? byte [g_pCDx+0x915] : 0 */
#define PUSHRT1  0x1000A800u          /* CDx::PushRenderTargetRet1: calls the original CDx_PushRenderTarget, returns 1 */
#define FRAME_RENDER 0x10011A00u      /* Frame_Render: manifest flag 4 (its compared gate trials ran 53 of 1424 instructions) */

typedef void (__attribute__((thiscall)) *SetColorFn)(void *, uint32_t);
typedef uint32_t (__attribute__((thiscall)) *GetColorFn)(void *);
typedef uint8_t (*GammaFn)(void);
typedef char (__attribute__((thiscall)) *PushRt1Fn)(void *, int, int, int);

static int presents, fails;
static HRESULT __stdcall fake_present(void *d, const RECT *a, const RECT *b, HWND w, const void *r) { (void)d; (void)a; (void)b; (void)w; (void)r; presents++; return 0; }
static void __stdcall other_slot(void) { printf("FAIL: a device method other than Present was called\n"); ExitProcess(3); }
static void *g_vtbl[97];
static struct { void **vt; } g_dev = { g_vtbl };
/* trace pictures (profile mode): a 64x32 X8R8G8B8 back buffer the host paints each frame, read back by the kit
   through GetBackBuffer, CreateImageSurface, CopyRects and the surface's GetDesc/LockRect/UnlockRect */
#define PW 64
#define PH 32
typedef struct { void **vt; uint32_t *pix; } FakeSurf;
static uint32_t g_bbpix[PW * PH], g_imgpix[PW * PH]; static void *g_svt[11];
static FakeSurf g_bb = { g_svt, g_bbpix }, g_img = { g_svt, g_imgpix };
static ULONG __stdcall s_release(void *t) { (void)t; return 0; }
static HRESULT __stdcall s_desc(void *t, uint32_t *d) { (void)t; memset(d, 0, 32); d[0] = 22; d[6] = PW; d[7] = PH; return 0; }
static HRESULT __stdcall s_lock(void *t, int32_t *lr, const RECT *r, DWORD fl) { (void)r; (void)fl; lr[0] = PW * 4; lr[1] = (int32_t)(uintptr_t)((FakeSurf *)t)->pix; return 0; }
static HRESULT __stdcall s_unlock(void *t) { (void)t; return 0; }
static HRESULT __stdcall d_getbb(void *d, UINT i, UINT ty, void **pp) { (void)d; (void)i; (void)ty; *pp = &g_bb; return 0; }
static HRESULT __stdcall d_createimg(void *d, UINT w, UINT h, UINT f, void **pp) { (void)d; (void)w; (void)h; (void)f; *pp = &g_img; return 0; }
static HRESULT __stdcall d_copyrects(void *d, void *src, const RECT *r, UINT n, void *dst, const POINT *pt) { (void)d; (void)r; (void)n; (void)pt; memcpy(((FakeSurf *)dst)->pix, ((FakeSurf *)src)->pix, sizeof g_bbpix); return 0; }
static uint8_t *g_paint_site;
static int tcl_calls, bs_calls;     /* facts mode: the device methods CDx::TestCooperativeLevel and CDx::BeginScene call (slots 3, 34) */
static void __stdcall d_texmethod(void) {}
static void (__stdcall *const g_texvt[19])(void) = { d_texmethod, d_texmethod, d_texmethod, d_texmethod, d_texmethod, d_texmethod, d_texmethod,
  d_texmethod, d_texmethod, d_texmethod, d_texmethod, d_texmethod, d_texmethod, d_texmethod, d_texmethod, d_texmethod, d_texmethod, d_texmethod, d_texmethod };
static uint32_t g_faketex[4] = { 0 };   /* profile mode: resources created through the hooked device (d3dprof's wrappers log them); [0]: its vtable, set in main */
static HRESULT __stdcall d_createtex(void *d, UINT w, UINT h, UINT lv, DWORD us, DWORD fmt, DWORD pool, void **pp) { (void)d; (void)w; (void)h; (void)lv; (void)us; (void)fmt; (void)pool; *pp = g_faketex; return 0; }
static HRESULT __stdcall d_createvs(void *d, const DWORD *decl, const DWORD *fn, DWORD *h, DWORD usage) { (void)d; (void)decl; (void)fn; (void)usage; *h = 0x1234; return 0; }
static HRESULT __stdcall d_tcl(void *d) { (void)d; tcl_calls++; return 0; }
static HRESULT __stdcall d_beginscene(void *d) { (void)d; bs_calls++; return 0; }   /* GetColor's entry: the band it "draws" is bright while it is swapped */
static uint8_t g_cdx[0x1000];

static void check(int cond, const char *what) { printf("%s: %s\n", cond ? "ok" : "FAIL", what); if (!cond) fails++; }
static void paint(void) {        /* the top band animates smoothly (a triangle wave); the bottom band is GetColor's */
  int t = presents % 400; uint32_t a = (uint32_t)(t < 200 ? t : 400 - t), b = g_paint_site && g_paint_site[0] == 0xE9 ? 200 : 50;
  for (int y = 0; y < PH; y++) for (int x = 0; x < PW; x++) g_bbpix[y * PW + x] = y < PH / 2 ? a * 0x010101u : (b << 16);
}
static void present(int n) { for (int i = 0; i < n; i++) { paint(); ((HRESULT (__stdcall *)(void *, const RECT *, const RECT *, HWND, const void *))g_dev.vt[15])(&g_dev, 0, 0, 0, 0); } }
static void touch_ini(const char *ini, const char *body) {
  Sleep(20); FILE *f = fopen(ini, "w"); fputs(body, f); fclose(f);
}

static jmp_buf jb; static volatile int armed; static uintptr_t landing_sp;
static void landing(void) { longjmp(jb, 1); }
static LONG CALLBACK host_veh(EXCEPTION_POINTERS *e) {
  if (armed && e->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
    armed = 0; e->ContextRecord->Eip = (DWORD)(uintptr_t)landing; e->ContextRecord->Esp = (DWORD)landing_sp;
    return EXCEPTION_CONTINUE_EXECUTION;
  }
  return EXCEPTION_CONTINUE_SEARCH;
}

int main(int argc, char **argv) {
  int reloadrun = argc > 1 && !strcmp(argv[1], "reload");
  int modrun = argc > 1 && (!strcmp(argv[1], "mod") || !strcmp(argv[1], "modrel") || reloadrun);
  int caprun = argc > 1 && !strcmp(argv[1], "capture");
  int factrun = argc > 1 && !strcmp(argv[1], "facts");
  int abrun = argc > 1 && !strcmp(argv[1], "ab");
  g_faketex[0] = (uint32_t)(uintptr_t)g_texvt;
  int relocated = argc > 1 && (!strcmp(argv[1], "relocated") || !strcmp(argv[1], "modrel")), profile = argc > 1 && !strcmp(argv[1], "profile");
  const char *ini = "xikit.ini";
  setvbuf(stdout, NULL, _IONBF, 0);
  if (relocated && !VirtualAlloc((void *)0x10000000, 0x01000000, MEM_RESERVE, PAGE_NOACCESS)) printf("note: could not reserve 0x10000000\n");
  HMODULE g = LoadLibraryExA("FFXiMain.dll", NULL, DONT_RESOLVE_DLL_REFERENCES);
  if (!g) { printf("FAIL: FFXiMain.dll not mapped (%lu)\n", GetLastError()); return 2; }
  uint32_t rel = (uint32_t)(uintptr_t)g - 0x10000000u;
  /* Wine maps a DONT_RESOLVE_DLL_REFERENCES image without applying its base relocations, and the unpacked file's
     header directory covers only the last 87 blocks of its table (not .text). In the game every relocation of
     the whole chain is applied, as the dumps show (pipeline/dump_verify.py checks every relocated dword). Apply
     the same chain, read from the .reloc section's start, so the image is as the game would have it. */
  if (rel && *(uint32_t *)(uintptr_t)(GAMMA + rel + 1) == G_PCDX) {
    IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS *)((char *)g + ((IMAGE_DOS_HEADER *)g)->e_lfanew);
    IMAGE_SECTION_HEADER *sh = IMAGE_FIRST_SECTION(nt), *rs = NULL;
    for (int k = 0; k < nt->FileHeader.NumberOfSections; k++) if (!memcmp(sh[k].Name, ".reloc", 7)) rs = &sh[k];
    if (!rs) { printf("FAIL: no .reloc section\n"); return 2; }
    DWORD old; VirtualProtect(g, nt->OptionalHeader.SizeOfImage, PAGE_EXECUTE_READWRITE, &old);
    uint8_t *p = (uint8_t *)g + rs->VirtualAddress, *end = p + rs->Misc.VirtualSize; int n = 0;
    while (p + 8 <= end) {
      IMAGE_BASE_RELOCATION *b = (IMAGE_BASE_RELOCATION *)p;
      if (b->SizeOfBlock < 8 || b->SizeOfBlock % 2 || p + b->SizeOfBlock > end) break;
      WORD *e = (WORD *)(b + 1);
      for (DWORD k = 0; k < (b->SizeOfBlock - sizeof *b) / 2; k++)
        if ((e[k] >> 12) == IMAGE_REL_BASED_HIGHLOW) { *(uint32_t *)((uint8_t *)g + b->VirtualAddress + (e[k] & 0xFFF)) += rel; n++; }
      p += b->SizeOfBlock;
    }
    printf("note: applied %d base relocations (the loader's job; Wine skips it for this load mode); entry dword now %08x\n", n, *(uint32_t *)(uintptr_t)(GAMMA + rel + 1));
  }
  printf("FFXiMain.dll at %p (delta %+ld)\n", g, (long)rel);
  check(!relocated || rel != 0, relocated ? "the game DLL was loaded away from its analysed base" : "the game DLL at its analysed base");
  uint8_t orig_set[5], orig_gam[5];
  memcpy(orig_set, (void *)(uintptr_t)(SETCOLOR + rel), 5); memcpy(orig_gam, (void *)(uintptr_t)(GAMMA + rel), 5);
  uint8_t orig_ecb[9]; memcpy(orig_ecb, (void *)(uintptr_t)(0x100096B0u + rel), 9);     /* CDx::EndClearBegin (the mod run) */
  for (int k = 0; k < 97; k++) g_vtbl[k] = (void *)other_slot;
  g_vtbl[15] = (void *)fake_present;
  g_vtbl[3] = (void *)d_tcl; g_vtbl[34] = (void *)d_beginscene; g_vtbl[20] = (void *)d_createtex; g_vtbl[75] = (void *)d_createvs;
  g_vtbl[16] = (void *)d_getbb; g_vtbl[27] = (void *)d_createimg; g_vtbl[28] = (void *)d_copyrects;
  for (int k = 0; k < 11; k++) g_svt[k] = (void *)other_slot;
  g_svt[2] = (void *)s_release; g_svt[8] = (void *)s_desc; g_svt[9] = (void *)s_lock; g_svt[10] = (void *)s_unlock;
  g_paint_site = (uint8_t *)(uintptr_t)(GETCOLOR + rel);   /* (SetColor is quarantined by the crash checks before the trace) */
  *(void **)(g_cdx + 0xC) = &g_dev; g_cdx[0x915] = 0x5A;
  *(void **)(uintptr_t)(G_PCDX + rel) = g_cdx;
  /* include_untested=1: these checks are about how the kit patches, so they use functions whatever their gate
     coverage; the default (untested functions left out) is checked below */
  uint8_t orig_frame[5]; memcpy(orig_frame, (void *)(uintptr_t)(FRAME_RENDER + rel), 5);
  touch_ini(ini, abrun ? "[xikit]\nlog_folder=1\nswap=1\ninclude_untested=1\ncount_calls=1\nreport_every=60\nhotkey=0\nab=1\nab_after=0\nab_phase=1\nab_rounds=2\nab_settle=3\n"
                 : profile ? "[xikit]\nlog_folder=0\nswap=1\ninclude_untested=1\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\nprofile=1\ncall_time=1\ntrace=1\ntrace_after=4\ntrace_count=1\nsample=1\ntrace_pictures=1\n"
                 : reloadrun ? "[xikit]\nlog_folder=0\nswap=1\ninclude_untested=1\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\nmods=mod_test.dll\nmod_reload=1\n"
                 : modrun ? "[xikit]\nlog_folder=0\nswap=1\ninclude_untested=1\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\nprofile=1\nmods=mod_test.dll\n"
                 : factrun ? "[xikit]\nlog_folder=0\nswap=0\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\nfacts=1\n"
                 : caprun ? "[xikit]\nlog_folder=0\nswap=0\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\ncapture=CXiSkeletonActor::GetColor,CXiSkeletonActor::SetColor\ncapture_count=1\ncapture_pages=4096\n"
                         : "[xikit]\nlog_folder=0\nswap=1\ninclude_untested=1\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\n");
  if (!LoadLibraryA("xikit.dll")) { printf("FAIL: xikit.dll not loaded (%lu)\n", GetLastError()); return 2; }
  for (int t = 0; t < 400 && g_dev.vt == g_vtbl; t++) Sleep(50);
  check(g_dev.vt != g_vtbl, "the kit hooked the device (its own vtable copy)");
  check(g_vtbl[15] == (void *)fake_present, "the original vtable was not written");
  {   /* build translation: the moved code ranges the kit read from this build's manifest reach the reconstruction */
    int32_t *xs = (int32_t *)GetProcAddress(GetModuleHandleA("gfx_recon_ingame.dll"), "xi_shift");
    printf("xi_shift:");
    if (xs) for (int i = 0; i <= 3 * xs[0] && i < 25; i++) printf(" %ld", (long)xs[i]);
    printf("\n");
    check(xs != NULL, "the reconstruction exports xi_shift and the kit filled it (its ranges: the xi_shift line)");
  }
  present(2);
  uint8_t *set = (uint8_t *)(uintptr_t)(SETCOLOR + rel), *gam = (uint8_t *)(uintptr_t)(GAMMA + rel);
  if (abrun) {
    /* ab=1 with log_folder=1: 4 phases of 1 s, A B B A; the swapped entries follow the phases; everything the kit writes
       goes to one new folder under xikit_logs */
    char seq[64] = ""; int ns = 0; int last = -1; DWORD t0 = GetTickCount();
    while (GetTickCount() - t0 < 6500) {
      present(1); Sleep(5);
      int on = set[0] == 0xE9;
      if (on != last && ns < 60) { seq[ns++] = on ? 'B' : 'A'; last = on; }
    }
    seq[ns] = 0;
    check(!strncmp(seq, "BABAB", 5) || !strncmp(seq, "ABAB", 4), "ab: the swaps went off and on with the phases");
    printf("note: entry states seen in order: %s (A B B A: off, on, on, off, then on again after the test)\n", seq);
    check(GetFileAttributesA("xikit.log") == INVALID_FILE_ATTRIBUTES, "log_folder=1: no xikit.log next to xikit.dll");
    WIN32_FIND_DATAA fd; HANDLE h = FindFirstFileA("xikit_logs\\*_*", &fd); char dir[MAX_PATH] = "";
    if (h != INVALID_HANDLE_VALUE) { snprintf(dir, sizeof dir, "xikit_logs\\%s", fd.cFileName); FindClose(h); }
    check(dir[0] != 0, "log_folder=1: a folder xikit_logs\\<date>_<time>_<pid> was made");
    char p2[MAX_PATH], line[512]; int rows = 0, labels_ok = 1, summary = 0; const char *want = "ABBA";
    snprintf(p2, sizeof p2, "%s\\xikit_ab_%lu_1.csv", dir, GetCurrentProcessId());
    FILE *af = fopen(p2, "r");
    if (af) { while (fgets(line, sizeof line, af)) {
        int ph, n; char lab; double mean;
        if (sscanf(line, "%d,%c,%d,%lf", &ph, &lab, &n, &mean) == 4) { if (ph < 1 || ph > 4 || lab != want[ph - 1] || n < 1 || mean <= 0) labels_ok = 0; rows++; }
        if (!strncmp(line, "# A (off):", 10)) summary = 1; } fclose(af); }
    check(rows == 4 && labels_ok && summary, "ab: xikit_ab_<pid>_1.csv in that folder has 4 phases A B B A with frames, and the summary");
    snprintf(p2, sizeof p2, "%s\\xikit.log", dir); int done = 0; FILE *lf = fopen(p2, "r");
    if (lf) { while (fgets(line, sizeof line, lf)) if (strstr(line, "ab: done. A (off)")) done = 1; fclose(lf); }
    check(done, "ab: xikit.log (in that folder) has the comparison");
    present(1); Sleep(150); present(1);                     /* a stutter: one frame of 150 ms */
    snprintf(p2, sizeof p2, "%s\\xikit_spikes_%lu.csv", dir, GetCurrentProcessId()); int spike = 0; FILE *sf = fopen(p2, "r");
    if (sf) { while (fgets(line, sizeof line, sf)) { unsigned fr; double ms; if (sscanf(line, "%u,%lf", &fr, &ms) == 2 && ms > 140) spike = 1; } fclose(sf); }
    check(spike, "spike_ms=50: a 150 ms frame is in xikit_spikes_<pid>.csv with its frame number");
    printf(fails ? "FAILED (%d)\n" : "ALL OK\n", fails);
    ExitProcess(fails ? 1 : 0);
  }
  if (!caprun && !factrun) check(set[0] == 0xE9 && gam[0] == 0xE9, "swap=1: the entries now jump to the reconstruction");
  check(presents == 2, "Present still reaches the device's own method");
  static uint8_t actor[0xA10];
  if (factrun) {
    /* facts=1 with xikit_facts.txt listing the calls in CDx::TestCooperativeLevel (0x10009716, call [ecx+0xC], 3 bytes: the
       region starts 2 bytes earlier) and CDx::BeginScene (0x10009736, call [ecx+0x88]): the game's own code (swap=0) runs them */
    char line[256]; int k;
    typedef BOOL (__attribute__((thiscall)) *CdxFn)(void *);
    for (k = 0; k < 5; k++) { ((CdxFn)(uintptr_t)(0x10009710u + rel))(g_cdx); ((CdxFn)(uintptr_t)(0x10009730u + rel))(g_cdx); }
    check(tcl_calls == 5 && bs_calls == 5, "facts=1: the patched call sites still reach the device's methods");
    present(60);
    char name[96]; snprintf(name, sizeof name, "xikit_facts_%lu.csv", GetCurrentProcessId());
    FILE *fc = fopen(name, "r"); int a = 0, b = 0;
    if (fc) { while (fgets(line, sizeof line, fc)) {
                if (!strncmp(line, "0x10009716,4,5,", 15) && strstr(line, ",3,0x10009710")) a = 1;
                if (!strncmp(line, "0x10009736,4,5,", 15) && strstr(line, ",34,0x10009730")) b = 1; } fclose(fc); }
    check(a && b, "xikit_facts_<pid>.csv: both sites, 5 calls each, the callee popped 4, through vtable slots 3 and 34");
    printf("%s\n", fails ? "FAILED" : "ALL OK");
    ExitProcess(fails ? 1 : 0);
  }
  if (caprun) {
    /* capture=: the first call of the game's GetColor (swap=0: the game's code) is captured; the call still returns
       the game's value; after capture_count calls the entry has its own bytes again. test_xikit.py replays the
       .xcap through the gate (pipeline/xcap.py) */
    char line[256];
    uint8_t *get = (uint8_t *)(uintptr_t)(GETCOLOR + rel);
    check(get[0] == 0xE9, "capture=: GetColor's entry jumps to the capture stub");
    ((SetColorFn)(uintptr_t)(SETCOLOR + rel))(actor, 0x5566AABBu);           /* captured too: one argument */
    check(*(uint32_t *)(actor + 0x660) == 0x5566AABBu, "the captured SetColor ran the game's code");
    check(((GetColorFn)get)(actor) == 0x5566AABBu, "the captured call returned the game's value");
    present(4);
    check(get[0] != 0xE9, "after capture_count calls the entry has its own bytes again");
    char name[96]; snprintf(name, sizeof name, "xikit_capture_%lu_A4840_1.xcap", GetCurrentProcessId());
    FILE *x = fopen(name, "rb"); long size = 0; uint32_t head[16] = {0};
    if (x) { fread(head, 4, 16, x); fseek(x, 0, SEEK_END); size = ftell(x); fclose(x); }
    check(x && head[0] == 0x50414358u && head[2] == GETCOLOR && head[8] > 0 && size == 64 + 36 + 72L * head[7] + 4L * head[8] + 4096L * head[8],
          "the .xcap holds the entry, the modules and the pages, sized as its header says");
    /* capture_follow (16): the pages after `this` (ECX) are taken, while readable */
    { uint32_t want = ((uint32_t)(uintptr_t)actor & ~0xFFFu) + 0x1000u; MEMORY_BASIC_INFORMATION mb; int have = 0;
      int readable = VirtualQuery((void *)(uintptr_t)want, &mb, sizeof mb) && mb.State == MEM_COMMIT && (mb.Protect & 0xEE) && !(mb.Protect & (PAGE_GUARD | PAGE_NOACCESS));
      x = fopen(name, "rb");
      if (x && head[8]) { uint32_t *pg = malloc(4 * head[8]); fseek(x, 64 + 36 + 72L * head[7], SEEK_SET);
                          if (pg && fread(pg, 4, head[8], x) == head[8]) for (uint32_t i = 0; i < head[8]; i++) have |= pg[i] == want;
                          free(pg); }
      if (x) fclose(x);
      check(!readable || have, "capture_follow: the page after the captured call's `this` is in the .xcap"); }
    FILE *lg = fopen("xikit.log", "r"); int logged = 0;
    if (lg) { while (fgets(line, sizeof line, lg)) if (strstr(line, "capture 1 of CXiSkeletonActor::GetColor")) logged = 1; fclose(lg); }
    check(logged, "xikit.log has the capture");
    printf("%s\n", fails ? "FAILED" : "ALL OK");
    ExitProcess(fails ? 1 : 0);
  }
  if (modrun) {
    /* sdk/: mod_test.dll (mod_test.cpp) hooks CXiSkeletonActor::GetColor with "the game's value + 1", its previous
       being a trampoline to the game's own code; SetColor is swapped to the reconstruction as usual */
    char line[256];
    HMODULE m = GetModuleHandleA(reloadrun ? "mod_test.0.dll" : "mod_test.dll");
    check(m != NULL, reloadrun ? "mods=mod_test.dll, mod_reload=1: the kit loaded a copy (xikit_mods_loaded\\mod_test.0.dll)" : "mods=mod_test.dll: the kit loaded the mod");
    if (!m) { printf("FAILED\n"); ExitProcess(1); }
    volatile long *frames = (volatile long *)GetProcAddress(m, "mod_frames"), *unk = (volatile long *)GetProcAddress(m, "mod_unknown_result"),
                  *irel = (volatile long *)GetProcAddress(m, "mod_init_rel");
    void **prev = (void **)GetProcAddress(m, "mod_prev");
    check(*irel == (long)rel, "the mod's xi_rel was the game's load delta when its init ran");
    uint8_t *get = (uint8_t *)(uintptr_t)(GETCOLOR + rel);
    int32_t d; memcpy(&d, get + 1, 4); uintptr_t to = (uintptr_t)get + 5 + d;
    IMAGE_NT_HEADERS *mnt = (IMAGE_NT_HEADERS *)((char *)m + ((IMAGE_DOS_HEADER *)m)->e_lfanew);
    check(get[0] == 0xE9 && to >= (uintptr_t)m && to < (uintptr_t)m + mnt->OptionalHeader.SizeOfImage, "GetColor's entry jumps into the mod (not the reconstruction)");
    ((SetColorFn)set)(actor, 0x11223344u);
    check(((GetColorFn)get)(actor) == 0x11223345u, "the hook ran: the game's GetColor value + 1");
    check(*prev && ((GetColorFn)*prev)(actor) == 0x11223344u, "the hook's previous (a trampoline) runs the game's own GetColor");
    check(*prev < (void *)m || *prev >= (void *)((char *)m + mnt->OptionalHeader.SizeOfImage), "previous is outside the mod (the kit's trampoline)");
    check(*unk == -1, "hook() of a name not in the manifest returned XIKIT_E_UNKNOWN");
    /* CDx::EndClearBegin's trampoline (T=9:5): 9 bytes copied, the rel32 call at +4 re-aimed, then jmp entry+9 */
    uint8_t *ecb = (uint8_t *)(uintptr_t)(0x100096B0u + rel), *tr = *(uint8_t **)GetProcAddress(m, "mod_prev_ecb");
    int ok = tr != NULL;
    if (ok) {
      int32_t dg, dt, dj; memcpy(&dt, tr + 5, 4); memcpy(&dj, tr + 10, 4);
      memcpy(&dg, (uint8_t *)(uintptr_t)(0x100096B0u + rel + 5), 4);    /* (entry+5..8: after the kit's jmp, still the game's) */
      ok = tr[4] == 0xE8 && (uintptr_t)tr + 9 + dt == (uintptr_t)ecb + 9 + dg && tr[9] == 0xE9 && (uintptr_t)tr + 14 + dj == (uintptr_t)ecb + 9
           && !memcmp(tr, orig_ecb, 4) && ecb[0] == 0xE9;    /* the 4 bytes before the call as the game had them; the entry now the kit's jmp */
    }
    check(ok, "CDx::EndClearBegin's trampoline: its copied call reaches the game's own target, then it jumps back to entry+9");
    check(*frames >= 2, "xikit_mod_frame ran each frame");
    FILE *lg = fopen("xikit.log", "r"); int hooked = 0, refused = 0;
    if (lg) { while (fgets(line, sizeof line, lg)) {
                if (strstr(line, "mod mod_test.dll: hooked CXiSkeletonActor::GetColor") && strstr(line, "previous = the game's code (trampoline)")) hooked = 1;
                if (strstr(line, "hook(No::SuchFunction): not a function of the manifest")) refused = 1; } fclose(lg); }
    check(hooked && refused, "xikit.log has the hook and the refusal");
    /* device hooks: BeginScene passes on (the mod counts, the device's own method runs), TestCooperativeLevel
       answers 7 by itself; a hook whose slot does not match its name is refused */
    typedef HRESULT (__stdcall *Dev0Fn)(void *);
    volatile long *begins = (volatile long *)GetProcAddress(m, "mod_begins");
    long b0 = *begins; int bs0 = bs_calls, tc0 = tcl_calls;
    HRESULT h1 = ((Dev0Fn)g_dev.vt[34])(&g_dev), h2 = ((Dev0Fn)g_dev.vt[3])(&g_dev);
    check(*(volatile long *)GetProcAddress(m, "mod_dev_result") == 0, "device_hook of BeginScene and TestCooperativeLevel returned XIKIT_OK");
    check(*begins == b0 + 1 && bs_calls == bs0 + 1 && h1 == 0, "the BeginScene device hook ran, and its previous reached the device's own BeginScene");
    check(h2 == 7 && tcl_calls == tc0, "the TestCooperativeLevel device hook answered by itself (the device's method not called)");
    check(*(volatile long *)GetProcAddress(m, "mod_bad_slot_result") == -1, "device_hook(\"BeginScene\", slot 35) refused: the slot is EndScene's");
    { void *fc = *(void **)GetProcAddress(m, "mod_fork_code"); int32_t ds; memcpy(&ds, set + 1, 4);
      check(*(volatile long *)GetProcAddress(m, "mod_fork_result") == 0 && fc && set[0] == 0xE9 && (uintptr_t)set + 5 + ds == (uintptr_t)fc,
            "XI_FORK: SetColor's entry jumps to the mod's own SetColor (the fork)");
      ((SetColorFn)set)(actor, 0x0BADF00Du);
      check(*(uint32_t *)(actor + 0x660) == 0x0BADF00Du, "the fork ran: SetColor wrote this+0x660");
      ((SetColorFn)set)(actor, 0x11223344u); }
    if (reloadrun) {
      /* mod_reload=1: a rebuilt mod (mod_test2.dll: GetColor adds 2) written over mod_test.dll is loaded again, as
         xikit_mods_loaded\mod_test.1.dll: the old build's hooks off, its xikit_mod_unload called, the new ones on */
      check(CopyFileA("mod_test2.dll", "mod_test.dll", FALSE), "the rebuilt mod written over mod_test.dll (the loaded copy does not lock it)");
      present(100);
      HMODULE m1 = GetModuleHandleA("mod_test.1.dll");
      check(m1 != NULL, "the kit loaded the new build (mod_test.1.dll) within 100 frames");
      if (!m1) { printf("FAILED\n"); ExitProcess(1); }
      check(*(volatile long *)GetProcAddress(m, "mod_unloaded") == 1, "the old build's xikit_mod_unload ran");
      memcpy(&d, get + 1, 4); to = (uintptr_t)get + 5 + d;
      IMAGE_NT_HEADERS *n1 = (IMAGE_NT_HEADERS *)((char *)m1 + ((IMAGE_DOS_HEADER *)m1)->e_lfanew);
      check(get[0] == 0xE9 && to >= (uintptr_t)m1 && to < (uintptr_t)m1 + n1->OptionalHeader.SizeOfImage, "GetColor's entry now jumps into the new build");
      check(((GetColorFn)get)(actor) == 0x11223346u, "the new build's hook runs: the game's GetColor value + 2");
      check(*(void **)GetProcAddress(m1, "mod_prev") == *prev, "the new build's previous is the same trampoline (built once, from the untouched entry)");
      volatile long *b1 = (volatile long *)GetProcAddress(m1, "mod_begins"); long ob = *begins, nb = *b1; bs0 = bs_calls;
      ((Dev0Fn)g_dev.vt[34])(&g_dev);
      check(*b1 == nb + 2 && *begins == ob && bs_calls == bs0 + 1, "BeginScene now runs the new build's device hook only, then the device's own method");
      FILE *t = fopen("xikit_reload_now.txt", "w"); if (t) fclose(t);
      present(40);
      check(GetModuleHandleA("mod_test.2.dll") != NULL && ((GetColorFn)get)(actor) == 0x11223346u, "xikit_reload_now.txt: loaded again (mod_test.2.dll), hooks on");
      printf("%s\n", fails ? "FAILED" : "ALL OK");
      ExitProcess(fails ? 1 : 0);
    }
    touch_ini(ini, "[xikit]\nlog_folder=0\nswap=0\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\nprofile=1\nmods=mod_test.dll\n");
    present(130);
    check(!memcmp(gam, orig_gam, 5), "swap=0: Gfx_IsGammaSupported has its original bytes again");
    check(get[0] == 0xE9 && ((GetColorFn)get)(actor) == 0x11223345u && set[0] == 0xE9, "swap=0: the mod's hook and its fork stay");
    /* the profiler sits behind the device hooks: the BeginScene above reached it through the mod's previous, and is
       reported at the host's call site (the call the hook was handling), not at an address inside the mod */
    present(600);
    { char nm[64]; snprintf(nm, sizeof nm, "d3dprof_%lu.csv", GetCurrentProcessId()); FILE *pc = fopen(nm, "r");
      HMODULE hx = GetModuleHandleA(NULL); IMAGE_NT_HEADERS *hn = (IMAGE_NT_HEADERS *)((char *)hx + ((IMAGE_DOS_HEADER *)hx)->e_lfanew);
      int in_host = 0, in_mod = 0;
      if (pc) { while (fgets(line, sizeof line, pc)) if (!strncmp(line, "BeginScene,0x", 13)) {
                  uintptr_t a = (uintptr_t)strtoul(line + 11, NULL, 16);
                  if (a >= (uintptr_t)hx && a < (uintptr_t)hx + hn->OptionalHeader.SizeOfImage) in_host = 1;
                  if (a >= (uintptr_t)m && a < (uintptr_t)m + mnt->OptionalHeader.SizeOfImage) in_mod = 1; } fclose(pc); }
      check(in_host && !in_mod, "d3dprof reports the hooked BeginScene at the host's call site, none inside the mod");
      /* a creation through the mod's CreateTexture hook: the resource log names the host's call site too */
      void *tex = NULL; ((HRESULT (__stdcall *)(void *, UINT, UINT, UINT, DWORD, DWORD, DWORD, void **))g_dev.vt[20])(&g_dev, 64, 32, 1, 0, 21, 1, &tex);
      snprintf(nm, sizeof nm, "xikit_resources_%lu.csv", GetCurrentProcessId()); FILE *rf = fopen(nm, "r"); int r_host = 0, r_mod = 0;
      if (rf) { while (fgets(line, sizeof line, rf)) { char *q = strstr(line, "\"CreateTexture(");
                  if (!q || !(q = strstr(q, "\",0x"))) continue;
                  uintptr_t a = (uintptr_t)strtoul(q + 2, NULL, 16);
                  if (a >= (uintptr_t)hx && a < (uintptr_t)hx + hn->OptionalHeader.SizeOfImage) r_host = 1;
                  if (a >= (uintptr_t)m && a < (uintptr_t)m + mnt->OptionalHeader.SizeOfImage) r_mod = 1; } fclose(rf); }
      check(tex == g_faketex && r_host && !r_mod, "a CreateTexture through the mod's device hook: xikit_resources_<pid>.csv names the host's call site, not the mod"); }
    printf("%s\n", fails ? "FAILED" : "ALL OK");
    ExitProcess(fails ? 1 : 0);
  }
  ((SetColorFn)set)(actor, 0x11223344u);
  check(*(uint32_t *)(actor + 0x660) == 0x11223344u, "reconstructed SetColor wrote this+0x660");
  check(((GetColorFn)(uintptr_t)(GETCOLOR + rel))(actor) == 0x11223344u, "GetColor (swapped too) reads it back");
  check(((GammaFn)gam)() == 0x5A, "reconstructed Gfx_IsGammaSupported read g_pCDx at the rebased address");
  present(60);
  FILE *csv; char name[64]; snprintf(name, sizeof name, "xikit_calls_%lu.csv", GetCurrentProcessId());
  csv = fopen(name, "r"); int seen = 0; char line[256];
  if (csv) { while (fgets(line, sizeof line, csv)) if (strstr(line, "0x100A4830,") && strstr(line, ",1,1,1")) seen = 1; fclose(csv); }
  check(seen, "the call count of SetColor is in the report (1 call)");
  touch_ini(ini, "[xikit]\nlog_folder=0\nswap=0\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\n");
  present(130);
  check(!memcmp(set, orig_set, 5) && !memcmp(gam, orig_gam, 5), "swap=0 (ini re-read): the original bytes are back");
  /* the default: a function the gate's compared trials did not fully cover (manifest flag 4) is left out.
     Frame_Render's trials ran 53 of its 1424 instructions */
  uint8_t *frame = (uint8_t *)(uintptr_t)(FRAME_RENDER + rel);
  touch_ini(ini, "[xikit]\nlog_folder=0\nswap=1\ninclude_untested=1\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\n");
  present(130);
  check(frame[0] == 0xE9, "include_untested=1: Frame_Render (flag 4) is swapped");
  touch_ini(ini, "[xikit]\nlog_folder=0\nswap=1\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\n");
  present(130);
  check(!memcmp(frame, orig_frame, 5), "the default (include_untested=0): Frame_Render, flag 4, has its original bytes");
  touch_ini(ini, "[xikit]\nlog_folder=0\nswap=1\ninclude_untested=1\nexclude=Gfx_IsGammaSupported\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\n");
  present(130);
  check(set[0] == 0xE9 && !memcmp(gam, orig_gam, 5), "swap=1 with an exclude: SetColor swapped, Gfx_IsGammaSupported not");
  /* bisect: SetColor is the "culprit" (the screen is wrong while it is swapped); the host answers as a person would */
  if (!profile) {          /* (its extra frames would shift the profiler's per-window counts) */
  touch_ini(ini, "[xikit]\nlog_folder=0\nswap=1\ninclude_untested=1\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\nbisect=1\n");
    present(130);
    ((SetColorFn)set)(actor, 1); ((GetColorFn)(uintptr_t)(GETCOLOR + rel))(actor); ((GammaFn)gam)();
    present(2);
    { void (__cdecl *answer)(int) = (void (__cdecl *)(int))GetProcAddress(GetModuleHandleA("xikit.dll"), "xikit_bisect_answer");
      check(answer != NULL, "bisect: xikit_bisect_answer is exported");
      answer(1);                                   /* the screen is wrong: start */
      for (int step = 0; step < 40; step++) {
        present(2);
        int bad = set[0] == 0xE9;                  /* wrong exactly while SetColor is swapped */
        FILE *lg = fopen("xikit.log", "r"); int done = 0;
        if (lg) { while (fgets(line, sizeof line, lg)) if (strstr(line, "CULPRIT")) done = 1; fclose(lg); }
        if (done) break;
        answer(bad);
      }
      present(2);
      FILE *lg = fopen("xikit.log", "r"); int cul = 0;
      if (lg) { while (fgets(line, sizeof line, lg)) if (strstr(line, "CULPRIT: CXiSkeletonActor::SetColor")) cul = 1; fclose(lg); }
      check(cul, "bisect: narrowed to CXiSkeletonActor::SetColor and logged it");
      present(2);
      check(set[0] != 0xE9 && !memcmp(set, orig_set, 5), "bisect: the culprit stays on the original code after it is found");
      answer(2); present(2);
      check(set[0] == 0xE9, "bisect: F12 (stop) swaps everything again, the culprit included");
      /* two functions that break the screen only together (SetColor and GetColor swapped at once): no single culprit */
      uint8_t *gc = (uint8_t *)(uintptr_t)(GETCOLOR + rel);
      answer(1);
      for (int step = 0; step < 60; step++) {
        present(2);
        int bad = set[0] == 0xE9 && gc[0] == 0xE9;
        FILE *lg = fopen("xikit.log", "r"); int done = 0;
        if (lg) { while (fgets(line, sizeof line, lg)) if (strstr(line, "CULPRITS TOGETHER")) done = 1; fclose(lg); }
        if (done) break;
        answer(bad);
      }
      present(2);
      lg = fopen("xikit.log", "r"); int pair = 0, both = 0;
      if (lg) { while (fgets(line, sizeof line, lg)) {
                  if (strstr(line, "CULPRITS TOGETHER: these 2 functions")) pair = 1;
                  if (strstr(line, "culprits (2):") && strstr(line, "CXiSkeletonActor::SetColor") && strstr(line, "CXiSkeletonActor::GetColor")) both = 1; }
                fclose(lg); }
      check(pair && both, "bisect: two functions that break the screen only together are found as that pair");
      check(set[0] != 0xE9 && gc[0] != 0xE9, "bisect: both stay on the original code after they are found");
      answer(2); present(2); }
    touch_ini(ini, "[xikit]\nlog_folder=0\nswap=1\ninclude_untested=1\nexclude=Gfx_IsGammaSupported\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\n");
    present(130);
  }
  /* a crash inside reconstructed code: SetColor on a null this */
  AddVectoredExceptionHandler(0, host_veh);
  volatile char room[1024]; landing_sp = ((uintptr_t)(room + 512)) & ~15u; (void)room;
  if (!setjmp(jb)) { armed = 1; ((SetColorFn)set)(NULL, 1); printf("FAIL: no exception\n"); fails++; }
  else printf("ok: the access violation inside the reconstruction was raised and passed on to the game's handlers\n");
  present(2);
  check(!memcmp(set, orig_set, 5), "the quarantined function was restored at the next frame");
  FILE *q = fopen("xikit_quarantine.txt", "r"); int quar = 0;
  if (q) { while (fgets(line, sizeof line, q)) if (strstr(line, "0x100A4830")) quar = 1; fclose(q); }
  check(quar, "it is in xikit_quarantine.txt");
  /* a fault in the game's own code, called from a reconstruction: CDx::PushRenderTargetRet1 on a null CDx calls
     CDx_PushRenderTarget (0x10009E20, swapped too), whose reconstruction calls the original
     CDx_PushRenderTargetTexture, which faults reading the null CDx. The kit finds the reconstructions' return
     addresses on the stack and quarantines the innermost, CDx_PushRenderTarget, as suspected. */
  uint8_t *prt = (uint8_t *)(uintptr_t)(PUSHRT1 + rel);
  check(prt[0] == 0xE9, "CDx::PushRenderTargetRet1 is swapped");
  if (!setjmp(jb)) { armed = 1; ((PushRt1Fn)prt)(NULL, 0, 0, 0); printf("FAIL: no exception\n"); fails++; }
  else printf("ok: the access violation in the game's code it called was raised and passed on\n");
  present(2);
  uint8_t *pr = (uint8_t *)(uintptr_t)(0x10009E20u + rel);
  check(pr[0] != 0xE9 && prt[0] == 0xE9, "the innermost reconstructed caller (CDx_PushRenderTarget) was restored at the next frame, its caller not");
  q = fopen("xikit_quarantine.txt", "r"); quar = 0;
  if (q) { while (fgets(line, sizeof line, q)) if (strstr(line, "0x10009E20") && strstr(line, "suspected")) quar = 1; fclose(q); }
  check(quar, "it is in xikit_quarantine.txt as suspected");
  if (profile) {
    /* d3dprof sits in front of the kit's copy: Present goes thunk -> kit_present -> the device's own method.
       Its report is rewritten every 600 frames; the host's own Present calls come from outside FFXiMain.dll */
    int before = presents; present(600);
    check(presents == before + 600, "profile=1: Present still reaches the device's own method through both hooks");
    snprintf(name, sizeof name, "d3dprof_%lu.csv", GetCurrentProcessId());
    csv = fopen(name, "r"); int head = 0, row = 0, ctime = 0, crow = 0;
    if (csv) {
      while (fgets(line, sizeof line, csv)) {
        if (!strncmp(line, "# d3dprof v2: 600 frames", 24)) head = 1;
        if (!strncmp(line, "# call time: QueryPerformanceCounter", 36)) ctime++;
        if (strstr(line, ",primitives_per_frame,us_per_call,us_per_frame")) ctime++;
        /* call_time=1: the Present row carries the time per call and per frame (two numbers after the primitives) */
        if (!strncmp(line, "Present,", 8)) { double a, b, c, d, e; char *p = strstr(line, ",host.exe+0x"); if (p) p = strchr(p + 1, ',');   /* the caller: its module */
          if (p && sscanf(p + 1, "%lf,%lf,%lf,%lf,%lf", &a, &b, &c, &d, &e) == 5 && d >= 0 && e >= 0) crow = 1; }
        /* the first window: a frame is Present to Present, so 601 Present calls over 600 frames = 1.002 */
        if (!strncmp(line, "Present,", 8) && strstr(line, ",host.exe+0x") && strstr(line, ",1.002,")) row = 1;
      }
      fclose(csv);
    }
    check(head, "profile=1: d3dprof wrote its report after 600 frames");
    check(ctime == 2 && crow, "call_time=1: the report has the timer line, the two time columns, and Present's time per call and per frame");
    /* the sampler should find the render thread in game or reconstructed code when it is there: GetColor (swapped)
       called for about half a second, then a window of frames so the samples are written */
    { DWORD t = GetTickCount(); volatile uint32_t sink = 0;
      while (GetTickCount() - t < 500) for (int k = 0; k < 10000; k++) sink += ((GetColorFn)(uintptr_t)(GETCOLOR + rel))(actor);
      present(60); }
    /* resources: a texture and a vertex shader created through the device (as the game would), logged by d3dprof's
       wrappers; the shader saved as the game passed it: stream 0, v0 <- FLOAT3, v5 <- D3DCOLOR; m4x4 oPos, v0, c0; mov oD0, v5 */
    { void *tex = NULL; DWORD vsh = 0;
      static const DWORD decl[] = { 0x20000000u, 0x40020000u, 0x40040005u, 0xFFFFFFFFu };
      static const DWORD fn[] = { 0xFFFE0101u, 0x00000014u, 0xC00F0000u, 0x90E40000u, 0xA0E40000u, 0x00000001u, 0xD00F0000u, 0x90E40005u, 0x0000FFFFu };
      ((HRESULT (__stdcall *)(void *, UINT, UINT, UINT, DWORD, DWORD, DWORD, void **))g_dev.vt[20])(&g_dev, 256, 128, 1, 0, 21, 1, &tex);
      ((HRESULT (__stdcall *)(void *, const DWORD *, const DWORD *, DWORD *, DWORD))g_dev.vt[75])(&g_dev, decl, fn, &vsh, 0);
      check(tex == g_faketex && vsh == 0x1234, "CreateTexture and CreateVertexShader through the hooked device still return the device's results");
      snprintf(name, sizeof name, "xikit_resources_%lu.csv", GetCurrentProcessId());
      FILE *rf = fopen(name, "r"); int t1 = 0, v1 = 0;
      if (rf) { while (fgets(line, sizeof line, rf)) { if (strstr(line, ",\"CreateTexture(") && strstr(line, ",100,80,1,0,15,1")) t1 = 1;
                                                       if (strstr(line, ",\"CreateVertexShader(") && strstr(line, ",00001234,4,9,0")) v1 = 1; } fclose(rf); }
      check(t1 && v1, "xikit_resources_<pid>.csv logs both, with their parameters and the objects created");
      snprintf(name, sizeof name, "xikit_vtables_%lu.csv", GetCurrentProcessId());
      FILE *vf = fopen(name, "r"); int vrows = 0, vok = 0; static char vl[2048];
      if (vf) { while (fgets(vl, sizeof vl, vf)) if (!strncmp(vl, "IDirect3DTexture8,", 18)) {
                  int commas = 0; for (char *q = vl; *q; q++) commas += *q == ',';
                  vrows++; vok = commas == 20 && strstr(vl, ".exe+0x") != NULL; } fclose(vf); }
      check(vrows == 1 && vok, "xikit_vtables_<pid>.csv: the created texture's vtable once, as IDirect3DTexture8 with its 19 entries"); }
    /* timing (always) and the sampling profiler (sample=1): written every report_every (60) frames */
    snprintf(name, sizeof name, "xikit_timing_%lu.csv", GetCurrentProcessId());
    FILE *tm = fopen(name, "r"); int trows = 0;
    if (tm) { while (fgets(line, sizeof line, tm)) if (line[0] >= '0' && line[0] <= '9') trows++; fclose(tm); }
    check(trows >= 5, "xikit_timing_<pid>.csv has a line per window (frame, Present and kit times)");
    snprintf(name, sizeof name, "xikit_samples_%lu.csv", GetCurrentProcessId());
    FILE *sm = fopen(name, "r"); int shead = 0, srows = 0;
    if (sm) { while (fgets(line, sizeof line, sm)) { if (!strncmp(line, "# xikit samples:", 16)) shead = 1; if (!strncmp(line, "self,", 5)) srows++; } fclose(sm); }
    check(shead && srows > 0, "sample=1: xikit_samples_<pid>.csv holds samples of the render thread");
    sm = fopen(name, "r"); int ingame = 0;
    if (sm) { while (fgets(line, sizeof line, sm)) if (!strncmp(line, "self,recon,", 11) && strstr(line, "GetColor")) ingame = 1; fclose(sm); }
    check(ingame, "sample=1: samples taken while the thread ran the reconstructed GetColor name it");
    check(row, "profile=1: the report has Present (601 calls in 600 frames) from outside FFXiMain.dll");
  }
  if (profile) {
    /* trace_after=4 starts the trace four seconds after the first frame; trace_alternate defaults to 1: 17 frames with the
       swaps flipped at every Present (frame 0 off, 1 on, ..., 15 on, 16 off); the host's Present is the only call */
    Sleep(4500); present(100);
    snprintf(name, sizeof name, "xikit_trace_%lu_1.txt", GetCurrentProcessId());
    FILE *tr = fopen(name, "r"); int hdr = 0, l0 = 0, l1 = 0, l15 = 0, l16 = 0, bad = 0;
    if (tr) { while (fgets(line, sizeof line, tr)) { if (!strncmp(line, "# d3d trace v1: alternating", 27)) hdr = 1;
                unsigned lab, fr; if (line[0] != '#' && sscanf(line, "%u %u 15", &lab, &fr) == 2 && lab != (fr & 1)) bad = 1;
                if (!strncmp(line, "0 0 15 Present", 14)) l0 = 1;
                if (!strncmp(line, "1 1 15 Present", 14)) l1 = 1;
                if (!strncmp(line, "1 15 15 Present", 15)) l15 = 1;
                if (!strncmp(line, "0 16 15 Present", 15)) l16 = 1; } fclose(tr); }
    check(hdr && l0 && l1 && l15 && l16 && !bad, "trace: xikit_trace_<pid>_1.txt has 17 frames, the swaps off in the even ones (label 0) and on in the odd ones (label 1)");
    /* the file trigger: creating xikit_trace_now.txt starts another, and the kit deletes the file */
    FILE *tn = fopen("xikit_trace_now.txt", "w"); if (tn) fclose(tn);
    present(100);
    snprintf(name, sizeof name, "xikit_trace_%lu_2.txt", GetCurrentProcessId());
    tr = fopen(name, "r"); int two = tr != NULL; if (tr) fclose(tr);
    check(two && GetFileAttributesA("xikit_trace_now.txt") == INVALID_FILE_ATTRIBUTES, "trace: xikit_trace_now.txt started a second trace and was deleted");
  }
  printf("%s\n", fails ? "FAILED" : "ALL OK");
  ExitProcess(fails ? 1 : 0);
}
