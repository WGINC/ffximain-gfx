/* host.c -- a stand-in for pol.exe to test xikit under Wine (test_xikit.py runs it).

   It maps the real FFXiMain.dll without running its DllMain (DONT_RESOLVE_DLL_REFERENCES; the loader still
   applies its relocations), optionally after occupying 0x10000000 so the DLL is loaded elsewhere, gives the game a
   fake CDx whose device has a fake vtable, loads xikit.dll and then plays the render thread: it calls the
   device's Present (which the kit hooks) and calls game functions through their original entries. Each check
   prints "ok: ..." or "FAIL: ...". */
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
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
static uint8_t g_cdx[0x1000];

static void check(int cond, const char *what) { printf("%s: %s\n", cond ? "ok" : "FAIL", what); if (!cond) fails++; }
static void present(int n) { for (int i = 0; i < n; i++) ((HRESULT (__stdcall *)(void *, const RECT *, const RECT *, HWND, const void *))g_dev.vt[15])(&g_dev, 0, 0, 0, 0); }
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
  int modrun = argc > 1 && (!strcmp(argv[1], "mod") || !strcmp(argv[1], "modrel"));
  int caprun = argc > 1 && !strcmp(argv[1], "capture");
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
  *(void **)(g_cdx + 0xC) = &g_dev; g_cdx[0x915] = 0x5A;
  *(void **)(uintptr_t)(G_PCDX + rel) = g_cdx;
  /* include_untested=1: these checks are about how the kit patches, so they use functions whatever their gate
     coverage; the default (untested functions left out) is checked below */
  uint8_t orig_frame[5]; memcpy(orig_frame, (void *)(uintptr_t)(FRAME_RENDER + rel), 5);
  touch_ini(ini, profile ? "[xikit]\nswap=1\ninclude_untested=1\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\nprofile=1\ntrace=1\ntrace_after=4\ntrace_count=1\n"
                 : modrun ? "[xikit]\nswap=1\ninclude_untested=1\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\nmods=mod_test.dll\n"
                 : caprun ? "[xikit]\nswap=0\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\ncapture=CXiSkeletonActor::GetColor,CXiSkeletonActor::SetColor\ncapture_count=1\ncapture_pages=4096\n"
                         : "[xikit]\nswap=1\ninclude_untested=1\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\n");
  if (!LoadLibraryA("xikit.dll")) { printf("FAIL: xikit.dll not loaded (%lu)\n", GetLastError()); return 2; }
  for (int t = 0; t < 400 && g_dev.vt == g_vtbl; t++) Sleep(50);
  check(g_dev.vt != g_vtbl, "the kit hooked the device (its own vtable copy)");
  check(g_vtbl[15] == (void *)fake_present, "the original vtable was not written");
  present(2);
  uint8_t *set = (uint8_t *)(uintptr_t)(SETCOLOR + rel), *gam = (uint8_t *)(uintptr_t)(GAMMA + rel);
  if (!caprun) check(set[0] == 0xE9 && gam[0] == 0xE9, "swap=1: the entries now jump to the reconstruction");
  check(presents == 2, "Present still reaches the device's own method");
  static uint8_t actor[0xA10];
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
    HMODULE m = GetModuleHandleA("mod_test.dll");
    check(m != NULL, "mods=mod_test.dll: the kit loaded the mod");
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
    touch_ini(ini, "[xikit]\nswap=0\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\nmods=mod_test.dll\n");
    present(130);
    check(!memcmp(set, orig_set, 5), "swap=0: SetColor has its original bytes again");
    check(get[0] == 0xE9 && ((GetColorFn)get)(actor) == 0x11223345u, "swap=0: the mod's hook stays");
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
  touch_ini(ini, "[xikit]\nswap=0\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\n");
  present(130);
  check(!memcmp(set, orig_set, 5) && !memcmp(gam, orig_gam, 5), "swap=0 (ini re-read): the original bytes are back");
  /* the default: a function the gate's compared trials did not fully cover (manifest flag 4) is left out.
     Frame_Render's trials ran 53 of its 1424 instructions */
  uint8_t *frame = (uint8_t *)(uintptr_t)(FRAME_RENDER + rel);
  touch_ini(ini, "[xikit]\nswap=1\ninclude_untested=1\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\n");
  present(130);
  check(frame[0] == 0xE9, "include_untested=1: Frame_Render (flag 4) is swapped");
  touch_ini(ini, "[xikit]\nswap=1\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\n");
  present(130);
  check(!memcmp(frame, orig_frame, 5), "the default (include_untested=0): Frame_Render, flag 4, has its original bytes");
  touch_ini(ini, "[xikit]\nswap=1\ninclude_untested=1\nexclude=Gfx_IsGammaSupported\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\n");
  present(130);
  check(set[0] == 0xE9 && !memcmp(gam, orig_gam, 5), "swap=1 with an exclude: SetColor swapped, Gfx_IsGammaSupported not");
  /* bisect: SetColor is the "culprit" (the screen is wrong while it is swapped); the host answers as a person would */
  if (!profile) {          /* (its extra frames would shift the profiler's per-window counts) */
  touch_ini(ini, "[xikit]\nswap=1\ninclude_untested=1\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\nbisect=1\n");
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
    touch_ini(ini, "[xikit]\nswap=1\ninclude_untested=1\nexclude=Gfx_IsGammaSupported\ncount_calls=1\nreport_every=60\nhotkey=0\nlog_each=1\n");
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
    csv = fopen(name, "r"); int head = 0, row = 0;
    if (csv) {
      while (fgets(line, sizeof line, csv)) {
        if (!strncmp(line, "# d3dprof v2: 600 frames", 24)) head = 1;
        /* the first window: a frame is Present to Present, so 601 Present calls over 600 frames = 1.002 */
        if (!strncmp(line, "Present,", 8) && strstr(line, ",other module,1.002,")) row = 1;
      }
      fclose(csv);
    }
    check(head, "profile=1: d3dprof wrote its report after 600 frames");
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
