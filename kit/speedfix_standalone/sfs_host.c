/* SPDX-License-Identifier: GPL-3.0-only
   sfs_host.c -- speedfix on its own: the part of xikit speedfix needs, and nothing else (no function swapping, no
   profiler, captures or fact recorder). Injected into pol.exe like xikit.dll and speedfix_hook (DllMain only starts a
   thread). Next to this DLL: gfx_recon_ingame.dll (the forks call verified tree functions in it), the manifest
   speedfix_manifest.txt / speedfix_manifest_<TimeDateStamp>.txt (kit/xikit's, for the analysed build and each other
   build it translates), speedfix.ini. It writes speedfix_standalone.log there.

   What it does, as xikit does it (kit/xikit/xikit.c; the functions below are its, cut down):
   - waits for FFXiMain.dll, picks the manifest of its TimeDateStamp and refuses any other build;
   - checks gfx_recon_ingame.dll against the manifest (size, CRC-32) and sets its xi_rel / xi_shift, and speedfix's;
   - waits for the device ([[g_pCDx]+0xC]), calls speedfix's xikit_mod_init with a host of its own:
     hook / hook_keeping_registers: a manifest function's entry checked against the analysed build's bytes
     (check_bytes) and jumped to the replacement; the previous is a trampoline of its first whole instructions
     (manifest T=), never the reconstruction; device_hook: the device object gets a vtable copy of its own;
   - the function jumps are written on the render thread at the first Present, other threads suspended and none inside
     an entry (apply); each Present then runs speedfix's xikit_mod_frame before the frame is presented.
   Nothing is written at an entry whose live bytes differ from the analysed build's. */
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../../sdk/include/xikit_mod.h"

#define ANALYSED_BASE 0x10000000u
#define G_PCDX 0x1045666Cu             /* CDx*; the device is at +0xC (data/com_fields.json, accepted) */
#define CDX_DEVICE_OFF 0xC
#define SLOT_PRESENT 15
#define NSLOTS 97
#define MAXFN 12288
#define JMP_LEN 5
#define TEXT_LO 0x10001000u
#define TEXT_HI 0x10329000u

/* speedfix (sf_game.cpp, sf_main.cpp), linked in */
int  __cdecl xikit_mod_init(const XikitHost *host);
void __cdecl xikit_mod_frame(void *device);
extern uintptr_t xi_rel; extern int32_t xi_shift[25];

typedef struct {
  uint32_t rva; int flags; char exp[256], name[96];
  uint8_t expect[20]; int nrel; int8_t rel[8];
  uint8_t *site; int tlen, nfix; int8_t tfix[4];
  void *repl, *tramp; int patched, refused;
} Fn;

static Fn *g_fn; static int g_nfn;
static uint8_t *g_tramps;
static int32_t g_shift[25];
static uint32_t g_rel, g_game_ts, g_recon_size, g_recon_crc;
static char g_dir[MAX_PATH], g_log[MAX_PATH];
static CRITICAL_SECTION g_loglock;
static int g_open, g_dirty, g_hookspeedfix_loaded;
static void *g_below[NSLOTS], *g_fvt[NSLOTS], *g_mod_slot[NSLOTS];
static XikitHost g_host;

static void klog(const char *fmt, ...) {
  char buf[1200]; va_list a; va_start(a, fmt); vsnprintf(buf, sizeof buf, fmt, a); va_end(a);
  EnterCriticalSection(&g_loglock);
  FILE *f = fopen(g_log, "a");
  if (f) { SYSTEMTIME t; GetLocalTime(&t); fprintf(f, "%02u:%02u:%02u.%03u %s\r\n", t.wHour, t.wMinute, t.wSecond, t.wMilliseconds, buf); fclose(f); }
  LeaveCriticalSection(&g_loglock);
}
static void __cdecl host_log(const char *fmt, ...) {
  char buf[1000]; va_list a; va_start(a, fmt); vsnprintf(buf, sizeof buf, fmt, a); va_end(a); klog("speedfix: %s", buf);
}

static uint32_t live_of(uint32_t ea) {          /* an analysed address -> where it is in the running game */
  int32_t d = 0;
  if (ea >= TEXT_LO && ea < TEXT_HI)
    for (int i = 0; i < g_shift[0]; i++) if (ea >= (uint32_t)g_shift[1 + 3 * i] && ea < (uint32_t)g_shift[2 + 3 * i]) d = g_shift[3 + 3 * i];
  return ea + g_rel + d;
}
static int hexval(int c) { return c >= '0' && c <= '9' ? c - '0' : (c | 32) >= 'a' && (c | 32) <= 'f' ? (c | 32) - 'a' + 10 : -1; }

static int load_manifest(const char *path) {      /* kit/xikit's manifest format (make_manifest.py) */
  FILE *m = fopen(path, "r"); char line[1024];
  if (!m) return 0;
  while (fgets(line, sizeof line, m)) {
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;
    if (line[0] == '!') {
      char k[64]; unsigned v, lo, hi; int d;
      if (sscanf(line + 1, "%63s %x", k, &v) == 2) {
        if (!strcmp(k, "game_timestamp")) g_game_ts = v;
        else if (!strcmp(k, "recon_crc32")) g_recon_crc = v;
      }
      if (sscanf(line + 1, "shift %x %x %d", &lo, &hi, &d) == 3 && g_shift[0] < 8) {
        int i = g_shift[0]++; g_shift[1 + 3 * i] = (int32_t)lo; g_shift[2 + 3 * i] = (int32_t)hi; g_shift[3 + 3 * i] = d; }
      if (sscanf(line + 1, "recon_size %u", &v) == 1) g_recon_size = v;
      continue;
    }
    if (g_nfn >= MAXFN) break;
    Fn *f = &g_fn[g_nfn]; memset(f, 0, sizeof *f);
    char hex[64], rels[64]; unsigned rva;
    if (sscanf(line, "%x %d %255s %63s %63s %95s", &rva, &f->flags, f->exp, hex, rels, f->name) != 6 || strlen(hex) != 40) continue;
    f->rva = rva;
    for (int i = 0; i < 20; i++) f->expect[i] = (uint8_t)(hexval(hex[2 * i]) << 4 | hexval(hex[2 * i + 1]));
    if (strcmp(rels, "-")) for (char *p = rels; *p && f->nrel < 8;) { f->rel[f->nrel++] = (int8_t)strtol(p, &p, 10); if (*p == ',') p++; }
    char *t = strstr(line, " T=");
    if (t && t[3] >= '0' && t[3] <= '9') {
      char *q = t + 3; f->tlen = (int)strtol(q, &q, 10);
      if (*q == ':') for (q++; *q >= '0' && *q <= '9' && f->nfix < 4;) { f->tfix[f->nfix++] = (int8_t)strtol(q, &q, 10); if (*q == ',') q++; }
      if (f->tlen < JMP_LEN || f->tlen > 16) f->tlen = 0;
    }
    g_nfn++;
  }
  fclose(m);
  return g_nfn;
}

static uint32_t crc32_file(const char *path, uint32_t *size) {
  static uint32_t tab[256]; static int init;
  if (!init) { for (uint32_t i = 0; i < 256; i++) { uint32_t c = i; for (int k = 0; k < 8; k++) c = c & 1 ? 0xEDB88320u ^ (c >> 1) : c >> 1; tab[i] = c; } init = 1; }
  FILE *f = fopen(path, "rb"); uint32_t crc = 0xFFFFFFFFu, n = 0; static uint8_t buf[65536]; size_t got;
  if (!f) return 0;
  while ((got = fread(buf, 1, sizeof buf, f)) > 0) { for (size_t i = 0; i < got; i++) crc = tab[(crc ^ buf[i]) & 0xFF] ^ (crc >> 8); n += (uint32_t)got; }
  fclose(f); *size = n; return crc ^ 0xFFFFFFFFu;
}

/* the live 20-byte window (entry-4 .. entry+16), relocations undone, against the analysed bytes: -1 if equal, else
   the first differing offset from the entry */
static int check_bytes(const Fn *f) {
  uint8_t live[20], care[20];
  memcpy(live, f->site - 4, 20); memset(care, 1, 20);
  for (int i = 0; i < f->nrel; i++) {
    int p = f->rel[i];
    if (p >= 0 && p + 4 <= 20) { uint32_t v; memcpy(&v, live + p, 4); v -= g_rel; memcpy(live + p, &v, 4); }
    else for (int k = 0; k < 4; k++) if (p + k >= 0 && p + k < 20) care[p + k] = 0;
  }
  for (int i = 0; i < 20; i++) if (care[i] && live[i] != f->expect[i]) return i - 4;
  return -1;
}
static void make_jmp(uint8_t *out, const uint8_t *at, const void *to) {
  int32_t d = (int32_t)((uintptr_t)to - ((uintptr_t)at + 5)); out[0] = 0xE9; memcpy(out + 1, &d, 4);
}

static Fn *fn_named(const char *what) {
  for (int i = 0; i < g_nfn; i++) if (!strcmp(what, g_fn[i].name) || !strcmp(what, g_fn[i].exp)) return &g_fn[i];
  if (what[0] == '0' && (what[1] == 'x' || what[1] == 'X')) {
    uint32_t v = (uint32_t)strtoul(what, NULL, 16);
    for (int i = 0; i < g_nfn; i++) if (v == g_fn[i].rva + ANALYSED_BASE || v == g_fn[i].rva) return &g_fn[i];
  }
  return NULL;
}

/* the game's own code of f: its first tlen bytes (whole instructions, manifest T=) copied from the live entry, rel32
   re-aimed, then a jmp to entry + tlen. Built from the untouched entry, after check_bytes. */
static void *build_tramp(Fn *f) {
  if (f->tramp) return f->tramp;
  if (!f->tlen || !g_tramps || f->patched) return NULL;
  uint8_t *t = g_tramps + (f - g_fn) * 32;
  memcpy(t, f->site, f->tlen);
  for (int k = 0; k < f->nfix; k++) {
    int32_t d; memcpy(&d, t + f->tfix[k], 4);
    d += (int32_t)((uintptr_t)f->site - (uintptr_t)t); memcpy(t + f->tfix[k], &d, 4);
  }
  make_jmp(t + f->tlen, t + f->tlen, f->site + f->tlen);
  FlushInstructionCache(GetCurrentProcess(), t, 32);
  return f->tramp = t;
}

static int hook_impl(const char *what, void *repl, void **prev, int keeps_regs) {
  if (prev) *prev = NULL;
  if (!g_open) { host_log("hook(%s): only inside xikit_mod_init", what ? what : "?"); return XIKIT_E_LATE; }
  Fn *f = what ? fn_named(what) : NULL;
  if (!f || !repl) { host_log("hook(%s): not a function of the manifest", what ? what : "NULL"); return XIKIT_E_UNKNOWN; }
  if (f->repl) { host_log("hook(%s): already hooked", f->name); return XIKIT_E_TAKEN; }
  int d = check_bytes(f);
  if (d != -1) { host_log("hook(%s): the live entry differs from the analysed build at %+d", f->name, d); return XIKIT_E_REFUSED; }
  if ((f->flags & 2) && g_hookspeedfix_loaded) { host_log("hook(%s): speedfix_hook.dll patches inside it", f->name); return XIKIT_E_REFUSED; }
  if ((f->flags & 8) && !keeps_regs) { host_log("hook(%s): its entry reads a register its declaration does not carry (manifest flag 8)", f->name); return XIKIT_E_REFUSED; }
  void *p = build_tramp(f);
  if (!p) { host_log("hook(%s): no trampoline for its entry, so no way to run the game's own code", f->name); return XIKIT_E_NOPREV; }
  if (prev) *prev = p;
  f->repl = repl; g_dirty = 1;
  host_log("hooked %s (0x%08X)%s; previous = the game's code (trampoline)", f->name, f->rva + ANALYSED_BASE, (f->flags & 8) ? " [keeps the registers]" : "");
  return XIKIT_OK;
}
static int __cdecl host_hook(const char *w, void *r, void **p) { return hook_impl(w, r, p, 0); }
static int __cdecl host_hook_regs(const char *w, void *r, void **p) { return hook_impl(w, r, p, 1); }
static void *__cdecl host_recon(const char *w) { (void)w; return NULL; }   /* never the previous here */

/* device_hook: the slot of the device's own vtable copy; the previous is the method below (the driver's, or the
   d3d8.dll in front of it). One hook a method (speedfix is the only client). */
static int __cdecl host_device_hook(const char *method, int slot, void *repl, void **prev) {
  if (prev) *prev = NULL;
  if (!g_open) { host_log("device_hook(%s): only inside xikit_mod_init", method ? method : "?"); return XIKIT_E_LATE; }
  if (!repl || slot < 0 || slot >= NSLOTS) { host_log("device_hook(%s, slot %d): no such slot", method ? method : "NULL", slot); return XIKIT_E_UNKNOWN; }
  if (g_mod_slot[slot]) { host_log("device_hook(%s): already hooked", method); return XIKIT_E_TAKEN; }
  g_mod_slot[slot] = repl; if (prev) *prev = g_below[slot];
  host_log("device hook %s (slot %d)", method ? method : "?", slot);
  return XIKIT_OK;
}

/* ---- writing the jumps: other threads suspended, none inside an entry (xikit.c apply) */
typedef struct { HANDLE h; } Th;
static int suspend_others(Th *th, int max) {
  int n = 0; DWORD me = GetCurrentThreadId(), pid = GetCurrentProcessId();
  HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
  if (snap == INVALID_HANDLE_VALUE) return -1;
  THREADENTRY32 te; te.dwSize = sizeof te;
  for (BOOL ok = Thread32First(snap, &te); ok; ok = Thread32Next(snap, &te)) {
    if (te.th32OwnerProcessID != pid || te.th32ThreadID == me) continue;
    HANDLE h = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT, FALSE, te.th32ThreadID);
    if (!h) continue;
    if (SuspendThread(h) == (DWORD)-1) { CloseHandle(h); continue; }
    if (n < max) th[n++].h = h; else { ResumeThread(h); CloseHandle(h); }
  }
  CloseHandle(snap); return n;
}
static int write5(uint8_t *at, const uint8_t *bytes) {
  DWORD old;
  if (!VirtualProtect(at, JMP_LEN, PAGE_EXECUTE_READWRITE, &old)) return 0;
  memcpy(at, bytes, JMP_LEN); VirtualProtect(at, JMP_LEN, old, &old);
  FlushInstructionCache(GetCurrentProcess(), at, JMP_LEN); return 1;
}
static void apply(void) {
  static Th th[1024]; static uint32_t eips[1024];
  int n = suspend_others(th, 1024), done = 0, busy = 0, refused = 0;
  if (n < 0) { klog("could not list threads: nothing written this frame"); return; }
  for (int i = 0; i < n; i++) { CONTEXT c; memset(&c, 0, sizeof c); c.ContextFlags = CONTEXT_CONTROL; eips[i] = GetThreadContext(th[i].h, &c) ? c.Eip : 0; }
  static char msgs[64][200]; int nmsg = 0;
  for (int i = 0; i < g_nfn; i++) {
    Fn *f = &g_fn[i];
    if (!f->repl || f->patched || f->refused) continue;
    int inside = 0;
    for (int t = 0; t < n; t++) if (eips[t] >= (uint32_t)(uintptr_t)f->site && eips[t] < (uint32_t)(uintptr_t)f->site + JMP_LEN) inside = 1;
    if (inside) { busy++; continue; }
    if (check_bytes(f) != -1) { f->refused = 1; refused++; if (nmsg < 64) snprintf(msgs[nmsg++], 200, "refused %s: its entry changed since the hook was asked", f->name); continue; }
    uint8_t jmp[JMP_LEN]; make_jmp(jmp, f->site, f->repl);
    if (write5(f->site, jmp)) { f->patched = 1; done++; }
  }
  for (int i = 0; i < n; i++) { ResumeThread(th[i].h); CloseHandle(th[i].h); }
  for (int i = 0; i < nmsg; i++) klog("%s", msgs[i]);
  klog("function hooks: %d written, %d refused, %d waiting (a thread was inside the entry)", done, refused, busy);
  g_dirty = busy > 0;
}

/* Present: the jumps (once), speedfix's frame, then the frame presented through speedfix's Present hook if it has one */
typedef HRESULT (__stdcall *PresentFn)(void *, const RECT *, const RECT *, HWND, const void *);
static HRESULT __stdcall sfs_present(void *dev, const RECT *a, const RECT *b, HWND w, const void *r) {
  if (g_dirty) apply();
  xikit_mod_frame(dev);
  PresentFn p = (PresentFn)(g_mod_slot[SLOT_PRESENT] ? g_mod_slot[SLOT_PRESENT] : g_below[SLOT_PRESENT]);
  return p(dev, a, b, w, r);
}
static void front_install(void **dev) {
  void **below = (void **)*dev;
  if (below == g_fvt) return;
  memcpy(g_below, below, sizeof g_below);
  for (int k = 0; k < NSLOTS; k++) g_fvt[k] = g_mod_slot[k] ? g_mod_slot[k] : g_below[k];
  g_fvt[SLOT_PRESENT] = (void *)sfs_present;
  *(void ***)dev = g_fvt;                     /* this object only: the driver's own vtable is not written */
}

static DWORD WINAPI start(LPVOID self) {
  char path[MAX_PATH];
  GetModuleFileNameA((HMODULE)self, g_dir, MAX_PATH);
  char *slash = strrchr(g_dir, '\\'); if (slash) *slash = 0;
  snprintf(g_log, sizeof g_log, "%s\\speedfix_standalone.log", g_dir);
  klog("speedfix (standalone) started in process %lu", (unsigned long)GetCurrentProcessId());
  HMODULE game;
  while (!(game = GetModuleHandleA("FFXiMain.dll"))) Sleep(100);
  IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS *)((char *)game + ((IMAGE_DOS_HEADER *)game)->e_lfanew);
  unsigned long ts = nt->FileHeader.TimeDateStamp;
  g_fn = (Fn *)calloc(MAXFN, sizeof(Fn));
  g_tramps = (uint8_t *)VirtualAlloc(NULL, MAXFN * 32, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
  if (!g_fn || !g_tramps) { klog("out of memory: nothing done"); return 0; }
  snprintf(path, sizeof path, "%s\\speedfix_manifest_%08lX.txt", g_dir, ts);
  if (GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES) snprintf(path, sizeof path, "%s\\speedfix_manifest.txt", g_dir);
  if (!load_manifest(path)) { klog("no manifest at %s: nothing done", path); return 0; }
  if (ts != g_game_ts) { klog("FFXiMain.dll is build %08lX; the manifest is for %08X: nothing done", ts, g_game_ts); return 0; }
  char recon[MAX_PATH]; snprintf(recon, sizeof recon, "%s\\gfx_recon_ingame.dll", g_dir);
  uint32_t size = 0, crc = crc32_file(recon, &size);
  if (!size || size != g_recon_size || crc != g_recon_crc) {
    klog("gfx_recon_ingame.dll is not the one the manifest was made from (size %u, CRC-32 %08X; manifest %u, %08X): nothing done", size, crc, g_recon_size, g_recon_crc);
    return 0;
  }
  HMODULE rec = GetModuleHandleA("gfx_recon_ingame.dll");     /* an import of this DLL (the forks call into it) */
  if (!rec) rec = LoadLibraryA(recon);
  if (!rec) { klog("could not load %s: nothing done", recon); return 0; }
  g_rel = (uint32_t)(uintptr_t)game - ANALYSED_BASE;
  uintptr_t *rr = (uintptr_t *)GetProcAddress(rec, "xi_rel"); int32_t *rs = (int32_t *)GetProcAddress(rec, "xi_shift");
  if (!rr || (g_shift[0] && !rs)) { klog("gfx_recon_ingame.dll exports no xi_rel / xi_shift: nothing done"); return 0; }
  *rr = g_rel; if (rs) memcpy(rs, g_shift, sizeof g_shift);
  xi_rel = g_rel; memcpy(xi_shift, g_shift, sizeof g_shift);
  HMODULE pin;
  GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_PIN | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)start, &pin);
  GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_PIN, recon, &pin);
  g_hookspeedfix_loaded = GetModuleHandleA("speedfix_hook.dll") != NULL;
  for (int i = 0; i < g_nfn; i++) g_fn[i].site = (uint8_t *)(uintptr_t)live_of(g_fn[i].rva + ANALYSED_BASE);
  klog("FFXiMain.dll build %08lX at %p (delta %+ld); %d functions in %s; %d moved code ranges", ts, game, (long)g_rel, g_nfn, path, g_shift[0]);
  void ***cdx = (void ***)(uintptr_t)(G_PCDX + g_rel); void **dev = NULL;
  for (;;) { void **c = (void **)*cdx; if (c && (dev = (void **)c[CDX_DEVICE_OFF / 4]) != NULL) break; Sleep(200); }
  memcpy(g_below, *dev, sizeof g_below);
  g_host.version = XIKIT_API_VERSION; g_host.size = sizeof g_host; g_host.game_base = (uintptr_t)game; g_host.rel = (intptr_t)g_rel;
  g_host.device = dev; g_host.recon = rec; g_host.dir = g_dir; g_host.log = host_log; g_host.hook = host_hook;
  g_host.reconstruction = host_recon; g_host.device_hook = host_device_hook; g_host.hook_keeping_registers = host_hook_regs;
  g_open = 1; int r = xikit_mod_init(&g_host); g_open = 0;
  klog("speedfix init returned %d", r);
  if (r) { klog("speedfix declined: nothing written"); return 0; }
  g_dirty = 1;
  front_install(dev);
  { int nh = 0; for (int k = 0; k < NSLOTS; k++) nh += g_mod_slot[k] != NULL;
    klog("device %p: speedfix's layer on, %d methods hooked by speedfix; the function hooks are written at the next frame", dev, nh); }
  for (void **last = dev;;) {                 /* a new device object gets the layer too (Reset keeps the object) */
    Sleep(1000);
    void **c = (void **)*cdx; void **d = c ? (void **)c[CDX_DEVICE_OFF / 4] : NULL;
    if (!d || d == last) continue;
    if ((void **)*d != g_fvt) { front_install(d); klog("a new device %p: speedfix's layer on", d); }
    last = d;
  }
}

BOOL WINAPI DllMain(HINSTANCE h, DWORD why, LPVOID r) {
  (void)r;
  if (why == DLL_PROCESS_ATTACH) {
    DisableThreadLibraryCalls(h); InitializeCriticalSection(&g_loglock);
    HANDLE t = CreateThread(NULL, 0, start, h, 0, NULL); if (t) CloseHandle(t);
  }
  return TRUE;
}
