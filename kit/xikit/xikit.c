/* xikit.c -- runs the reconstructed graphics code inside the real game (docs/KIT.md).

   Injected into pol.exe like speedfix_hook and d3dprof (DllMain only starts a thread). Next to this DLL:
     gfx_recon_ingame.dll   the clean tree built with XI_INGAME (every game address rebased through xi_rel)
     xikit_manifest.txt     the functions that may be swapped and the bytes their entries must hold
     xikit.ini              the settings (written with the defaults if missing; re-read when it changes)
   It writes xikit.log, xikit_calls_<pid>.csv (call counts) and xikit_quarantine.txt (functions that raised an
   exception inside reconstructed code; left out from then on) next to itself, and d3dprof_<pid>.csv when the
   profiler is on.

   Safety rules (the owner's speedfix_hook rules, plus the hot-patch ones):
     * nothing happens unless FFXiMain.dll is the analysed build and the reconstruction DLL is the one the
       manifest was made from (size and CRC-32);
     * code is written only inside the device's Present, on the thread that renders, at a frame boundary, with
       every other thread of the process suspended; if any of them is stopped inside the 5 bytes being written,
       nothing is written that frame (retried on the next);
     * before a jmp is written the live bytes are compared with the analysed build's (relocations undone); a
       restore first checks that the kit's own jmp is still there. A mismatch refuses that function, logged;
     * only the 5 bytes are made writable, then their old protection is put back, and the instruction cache is
       flushed;
     * this DLL and the reconstruction DLL pin themselves: a restore only redirects future calls, so a
       reconstructed function still on some thread's stack returns into code that is still loaded.

   Build (32-bit, like the game): build.sh (MinGW) or CMakeLists.txt (MSVC). */
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define ANALYSED_BASE 0x10000000u
#define G_PCDX 0x1045666Cu             /* CDx*; the device is at +0xC (data/com_fields.json, accepted) */
#define CDX_DEVICE_OFF 0xC
#define SLOT_RESET 14
#define SLOT_PRESENT 15
#define NSLOTS 97
#define MAXFN 2048
#define JMP_LEN 5
#define STUB_LEN 12                    /* lock inc dword [count]; jmp target */

/* d3dprof.c, built in with XIKIT defined: d3dprof_attach gives the device its counting vtable copy */
void d3dprof_attach(void **dev, uint32_t rel, uint32_t lo, uint32_t hi, const char *csv);

typedef struct {
  uint32_t rva;                        /* entry - 0x10000000 */
  int flags;                           /* 1 SEH, 2 speedfix site */
  char exp[160], name[96];
  uint8_t expect[20];                  /* analysed bytes, entry-4 .. entry+16 */
  int nrel; int8_t rel[8];             /* relocated dwords touching that window (offsets -3..19) */
  uint8_t *site;                       /* live entry */
  void *target;                        /* the reconstruction's export */
  uint8_t *stub;                       /* counting stub, or NULL */
  uint8_t saved[JMP_LEN];              /* live bytes the jmp replaced */
  int patched, refused, quarantined, wanted;
  volatile LONG calls; LONG calls_reported;
} Fn;

typedef struct {
  int swap, count_calls, profile, respect_speedfix, include_flagged, report_every, hotkey, log_each;
  char only[4096], exclude[4096];
} Cfg;

static Fn g_fn[MAXFN]; static int g_nfn;
static Fn *g_by_target[MAXFN];         /* sorted by reconstruction address, for the exception handler */
static Cfg g_cfg;
static char g_dir[MAX_PATH], g_ini[MAX_PATH], g_logpath[MAX_PATH], g_quar[MAX_PATH];
static FILETIME g_ini_time;
static CRITICAL_SECTION g_loglock;
static uint32_t g_rel, g_game_lo, g_game_hi, g_recon_lo, g_recon_hi;
static uint8_t *g_stubs;
static void **g_orig_vt; static void *g_vt[NSLOTS];
static DWORD g_render_thread;
static uint32_t g_frame; static int g_master_off, g_hotkey_down, g_dirty = 1, g_speedfix_loaded;
static uint32_t g_game_ts, g_recon_size, g_recon_crc;

/* ------------------------------------------------------------------------------------------------ log */
static void klog(const char *fmt, ...) {
  char buf[1024]; va_list a; va_start(a, fmt); vsnprintf(buf, sizeof buf, fmt, a); va_end(a);
  EnterCriticalSection(&g_loglock);
  FILE *f = fopen(g_logpath, "a");
  if (f) { SYSTEMTIME t; GetLocalTime(&t); fprintf(f, "%02u:%02u:%02u.%03u %s\n", t.wHour, t.wMinute, t.wSecond, t.wMilliseconds, buf); fclose(f); }
  LeaveCriticalSection(&g_loglock);
}

/* ------------------------------------------------------------------------------------------------ config */
static const char DEFAULT_INI[] =
  "; xikit.ini -- re-read every 60 frames when it changes (docs/KIT.md)\r\n"
  "[xikit]\r\n"
  "; 1: run the reconstructed functions instead of the originals (all verified ones, minus the lists below)\r\n"
  "swap=0\r\n"
  "; only these (comma-separated RVAs like 0x6C530 / addresses like 0x1006C530 / names); empty = all\r\n"
  "only=\r\n"
  "; never these (same forms)\r\n"
  "exclude=\r\n"
  "; 1: also swap functions flagged SEH or speedfix-site (see the manifest)\r\n"
  "include_flagged=0\r\n"
  "; 1: leave out the functions speedfix_hook patches inside, when speedfix_hook.dll is loaded\r\n"
  "respect_speedfix=1\r\n"
  "; 1: count the calls of every swapped function (xikit_calls_<pid>.csv)\r\n"
  "count_calls=1\r\n"
  "; frames between call-count reports\r\n"
  "report_every=600\r\n"
  "; 1: Ctrl+Alt+F9 switches every swap off / back on, at the next frame\r\n"
  "hotkey=1\r\n"
  "; 1: log every function patched and restored (otherwise only totals)\r\n"
  "log_each=0\r\n"
  "; 1: d3dprof in the same process (d3dprof_<pid>.csv); takes effect at the next start\r\n"
  "profile=0\r\n";

static void read_cfg(void) {
  Cfg c; memset(&c, 0, sizeof c);
  c.swap = GetPrivateProfileIntA("xikit", "swap", 0, g_ini);
  c.count_calls = GetPrivateProfileIntA("xikit", "count_calls", 1, g_ini);
  c.profile = GetPrivateProfileIntA("xikit", "profile", 0, g_ini);
  c.respect_speedfix = GetPrivateProfileIntA("xikit", "respect_speedfix", 1, g_ini);
  c.include_flagged = GetPrivateProfileIntA("xikit", "include_flagged", 0, g_ini);
  c.report_every = GetPrivateProfileIntA("xikit", "report_every", 600, g_ini);
  c.hotkey = GetPrivateProfileIntA("xikit", "hotkey", 1, g_ini);
  c.log_each = GetPrivateProfileIntA("xikit", "log_each", 0, g_ini);
  GetPrivateProfileStringA("xikit", "only", "", c.only, sizeof c.only, g_ini);
  GetPrivateProfileStringA("xikit", "exclude", "", c.exclude, sizeof c.exclude, g_ini);
  if (c.report_every < 60) c.report_every = 60;
  g_cfg = c;
}

static int ini_changed(void) {
  WIN32_FILE_ATTRIBUTE_DATA d;
  if (!GetFileAttributesExA(g_ini, GetFileExInfoStandard, &d)) return 0;
  if (CompareFileTime(&d.ftLastWriteTime, &g_ini_time) == 0) return 0;
  g_ini_time = d.ftLastWriteTime; return 1;
}

/* does a comma-separated list name this function (RVA, absolute analysed address, or name)? */
static int listed(const char *list, const Fn *f) {
  char item[128]; const char *p = list;
  while (*p) {
    int n = 0;
    while (*p == ' ' || *p == ',' || *p == '\t') p++;
    while (*p && *p != ',' && n < (int)sizeof item - 1) item[n++] = *p++;
    while (n && (item[n - 1] == ' ' || item[n - 1] == '\t')) n--;
    item[n] = 0;
    if (!n) continue;
    if ((item[0] == '0' && (item[1] == 'x' || item[1] == 'X')) || (item[0] >= '0' && item[0] <= '9')) {
      uint32_t v = (uint32_t)strtoul(item, NULL, 16);
      if (v == f->rva || v == f->rva + ANALYSED_BASE) return 1;
    } else if (!strcmp(item, f->name) || !strcmp(item, f->exp)) return 1;
  }
  return 0;
}

static int wanted(const Fn *f) {
  if (!g_cfg.swap || g_master_off || f->refused || f->quarantined || !f->target) return 0;
  if ((f->flags & 1) && !g_cfg.include_flagged) return 0;
  if ((f->flags & 2) && g_cfg.respect_speedfix && g_speedfix_loaded && !g_cfg.include_flagged) return 0;
  if (g_cfg.only[0] && !listed(g_cfg.only, f)) return 0;
  if (g_cfg.exclude[0] && listed(g_cfg.exclude, f)) return 0;
  return 1;
}

/* ------------------------------------------------------------------------------------------------ manifest */
static int hexval(int c) { return c >= '0' && c <= '9' ? c - '0' : (c | 32) >= 'a' && (c | 32) <= 'f' ? (c | 32) - 'a' + 10 : -1; }

static int load_manifest(const char *path) {
  FILE *m = fopen(path, "r"); char line[1024];
  if (!m) { klog("no manifest at %s: nothing done", path); return 0; }
  while (fgets(line, sizeof line, m)) {
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;
    if (line[0] == '!') {
      char k[64]; unsigned v;
      if (sscanf(line + 1, "%63s %x", k, &v) == 2) {
        if (!strcmp(k, "game_timestamp")) g_game_ts = v;
        else if (!strcmp(k, "recon_crc32")) g_recon_crc = v;
      }
      if (sscanf(line + 1, "recon_size %u", &v) == 1) g_recon_size = v;
      continue;
    }
    if (g_nfn >= MAXFN) break;
    Fn *f = &g_fn[g_nfn]; memset(f, 0, sizeof *f);
    char hex[64], rels[64]; unsigned rva;
    if (sscanf(line, "%x %d %159s %63s %63s %95s", &rva, &f->flags, f->exp, hex, rels, f->name) != 6 || strlen(hex) != 40) {
      klog("manifest: bad line ignored: %.80s", line); continue;
    }
    f->rva = rva;
    for (int i = 0; i < 20; i++) f->expect[i] = (uint8_t)(hexval(hex[2 * i]) << 4 | hexval(hex[2 * i + 1]));
    if (strcmp(rels, "-")) for (char *p = rels; *p && f->nrel < 8;) { f->rel[f->nrel++] = (int8_t)strtol(p, &p, 10); if (*p == ',') p++; }
    g_nfn++;
  }
  fclose(m);
  return g_nfn;
}

static uint32_t crc32_file(const char *path, uint32_t *size) {
  static uint32_t tab[256]; static int init;
  if (!init) { for (uint32_t i = 0; i < 256; i++) { uint32_t c = i; for (int k = 0; k < 8; k++) c = c & 1 ? 0xEDB88320u ^ (c >> 1) : c >> 1; tab[i] = c; } init = 1; }
  FILE *f = fopen(path, "rb"); uint32_t crc = 0xFFFFFFFFu, n = 0; uint8_t buf[65536]; size_t got;
  if (!f) return 0;
  while ((got = fread(buf, 1, sizeof buf, f)) > 0) { for (size_t i = 0; i < got; i++) crc = tab[(crc ^ buf[i]) & 0xFF] ^ (crc >> 8); n += (uint32_t)got; }
  fclose(f); *size = n; return crc ^ 0xFFFFFFFFu;
}

/* ------------------------------------------------------------------------------------------------ checks */
/* the live 20-byte window, relocations undone, against the analysed bytes; returns -1 if equal, else the
   first differing offset (relative to the entry) */
static int check_bytes(const Fn *f) {
  uint8_t live[20], care[20];
  memcpy(live, f->site - 4, 20); memset(care, 1, 20);
  for (int i = 0; i < f->nrel; i++) {
    int p = f->rel[i];
    if (p >= 0 && p + 4 <= 20) { uint32_t v; memcpy(&v, live + p, 4); v -= g_rel; memcpy(live + p, &v, 4); }
    else for (int k = 0; k < 4; k++) if (p + k >= 0 && p + k < 20) care[p + k] = 0;   /* straddles the window: not compared */
  }
  for (int i = 0; i < 20; i++) if (care[i] && live[i] != f->expect[i]) return i - 4;
  return -1;
}

static void make_jmp(uint8_t *out, const uint8_t *at, const void *to) {
  int32_t d = (int32_t)((uintptr_t)to - ((uintptr_t)at + 5)); out[0] = 0xE9; memcpy(out + 1, &d, 4);
}

/* ------------------------------------------------------------------------------------------------ patching */
typedef struct { HANDLE h; DWORD id; } Th;

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
    if (n < max) { th[n].h = h; th[n].id = te.th32ThreadID; n++; } else { ResumeThread(h); CloseHandle(h); }
  }
  CloseHandle(snap);
  return n;
}

static void resume_all(Th *th, int n) { for (int i = 0; i < n; i++) { ResumeThread(th[i].h); CloseHandle(th[i].h); } }

static int write5(uint8_t *at, const uint8_t *bytes) {
  DWORD old;
  if (!VirtualProtect(at, JMP_LEN, PAGE_EXECUTE_READWRITE, &old)) return 0;
  memcpy(at, bytes, JMP_LEN);
  VirtualProtect(at, JMP_LEN, old, &old);
  FlushInstructionCache(GetCurrentProcess(), at, JMP_LEN);
  return 1;
}

/* bring every function to its wanted state; called from Present on the render thread */
static void apply(void) {
  static Th th[1024]; static uint32_t eips[1024];
  int todo = 0;
  for (int i = 0; i < g_nfn; i++) { g_fn[i].wanted = wanted(&g_fn[i]); todo += g_fn[i].wanted != g_fn[i].patched; }
  if (!todo) { g_dirty = 0; return; }
  int n = suspend_others(th, 1024);
  if (n < 0) { klog("could not list threads: nothing written this frame"); return; }
  for (int i = 0; i < n; i++) {
    CONTEXT c; memset(&c, 0, sizeof c); c.ContextFlags = CONTEXT_CONTROL;
    eips[i] = GetThreadContext(th[i].h, &c) ? c.Eip : 0;
  }
  int patched = 0, restored = 0, busy = 0, refused = 0;
  /* messages are kept until the threads run again: logging takes locks another thread may hold */
  static char msgs[64][200]; int nmsg = 0;
  for (int i = 0; i < g_nfn; i++) {
    Fn *f = &g_fn[i];
    if (f->wanted == f->patched) continue;
    int inside = 0;
    for (int t = 0; t < n; t++) if (eips[t] >= (uint32_t)(uintptr_t)f->site && eips[t] < (uint32_t)(uintptr_t)f->site + JMP_LEN) inside = 1;
    if (inside) { busy++; continue; }
    uint8_t jmp[JMP_LEN];
    make_jmp(jmp, f->site, f->stub ? (void *)f->stub : f->target);
    if (f->wanted) {
      int d = check_bytes(f);
      if (d != -1) {
        f->refused = 1; refused++;
        if (nmsg < 64) snprintf(msgs[nmsg++], 200, "refused %s (0x%08X): live byte at entry%+d is not the analysed build's",
                                f->name, f->rva + ANALYSED_BASE, d);
        continue;
      }
      memcpy(f->saved, f->site, JMP_LEN);
      if (write5(f->site, jmp)) { f->patched = 1; patched++;
        if (g_cfg.log_each && nmsg < 64) snprintf(msgs[nmsg++], 200, "swapped %s (0x%08X)", f->name, f->rva + ANALYSED_BASE); }
    } else {
      if (memcmp(f->site, jmp, JMP_LEN)) {
        f->refused = 1; f->patched = 0; refused++;
        if (nmsg < 64) snprintf(msgs[nmsg++], 200, "not restored %s (0x%08X): its entry no longer holds the kit's jmp (another hook wrote there)",
                                f->name, f->rva + ANALYSED_BASE);
        continue;
      }
      if (write5(f->site, f->saved)) { f->patched = 0; restored++;
        if (g_cfg.log_each && nmsg < 64) snprintf(msgs[nmsg++], 200, "restored %s (0x%08X)", f->name, f->rva + ANALYSED_BASE); }
    }
  }
  resume_all(th, n);
  for (int i = 0; i < nmsg; i++) klog("%s", msgs[i]);
  int now = 0; for (int i = 0; i < g_nfn; i++) now += g_fn[i].patched;
  klog("frame %u: %d swapped, %d restored, %d refused, %d waiting (a thread was inside the entry); %d of %d now run reconstructed code",
       g_frame, patched, restored, refused, busy, now, g_nfn);
  g_dirty = busy > 0;
}

/* ------------------------------------------------------------------------------------------------ reports */
static void report_calls(void) {
  char path[MAX_PATH]; snprintf(path, sizeof path, "%s\\xikit_calls_%lu.csv", g_dir, (unsigned long)GetCurrentProcessId());
  FILE *f = fopen(path, "w"); if (!f) return;
  fprintf(f, "# frame %u; calls of each swapped function since it was swapped, and in the last window\n", g_frame);
  fprintf(f, "address,name,swapped,calls_total,calls_window\n");
  for (int i = 0; i < g_nfn; i++) {
    Fn *x = &g_fn[i]; LONG c = x->calls;
    if (!x->stub || (!x->patched && !c)) continue;
    fprintf(f, "0x%08X,%s,%d,%ld,%ld\n", x->rva + ANALYSED_BASE, x->name, x->patched, (long)c, (long)(c - x->calls_reported));
    x->calls_reported = c;
  }
  fclose(f);
}

/* ------------------------------------------------------------------------------------------------ Present */
typedef HRESULT (__stdcall *PresentFn)(void *, const RECT *, const RECT *, HWND, const void *);

static HRESULT __stdcall kit_present(void *dev, const RECT *a, const RECT *b, HWND w, const void *r) {
  if (!g_render_thread) g_render_thread = GetCurrentThreadId();
  if (GetCurrentThreadId() == g_render_thread) {
    g_frame++;
    if (g_cfg.hotkey) {
      int down = (GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState(VK_MENU) & 0x8000) && (GetAsyncKeyState(VK_F9) & 0x8000);
      if (down && !g_hotkey_down) { g_master_off = !g_master_off; g_dirty = 1; klog("hotkey: swaps %s", g_master_off ? "off" : "on"); }
      g_hotkey_down = down;
    }
    if (g_frame % 60 == 0 && ini_changed()) { read_cfg(); g_dirty = 1; klog("xikit.ini changed: re-read"); }
    if (g_dirty) apply();
    if (g_cfg.count_calls && g_frame % (uint32_t)g_cfg.report_every == 0) report_calls();
  }
  return ((PresentFn)g_orig_vt[SLOT_PRESENT])(dev, a, b, w, r);
}

/* ------------------------------------------------------------------------------------------------ crashes */
static int cmp_target(const void *a, const void *b) {
  uintptr_t x = (uintptr_t)(*(Fn *const *)a)->target, y = (uintptr_t)(*(Fn *const *)b)->target;
  return x < y ? -1 : x > y;
}

static LONG CALLBACK on_exception(EXCEPTION_POINTERS *e) {
  DWORD code = e->ExceptionRecord->ExceptionCode;
  if (code == EXCEPTION_BREAKPOINT || code == EXCEPTION_SINGLE_STEP || code == 0xE06D7363u || code == 0x406D1388u
      || code == DBG_PRINTEXCEPTION_C) return EXCEPTION_CONTINUE_SEARCH;
  uint32_t eip = e->ContextRecord->Eip;
  Fn *f = NULL;
  if (eip >= g_recon_lo && eip < g_recon_hi) {
    int lo = 0, hi = g_nfn - 1;
    while (lo <= hi) { int m = (lo + hi) / 2; if ((uint32_t)(uintptr_t)g_by_target[m]->target <= eip) { f = g_by_target[m]; lo = m + 1; } else hi = m - 1; }
  } else if (g_stubs && eip >= (uint32_t)(uintptr_t)g_stubs && eip < (uint32_t)(uintptr_t)g_stubs + MAXFN * STUB_LEN) {
    f = &g_fn[(eip - (uint32_t)(uintptr_t)g_stubs) / STUB_LEN];
  } else return EXCEPTION_CONTINUE_SEARCH;
  CONTEXT *c = e->ContextRecord; uint32_t *sp = (uint32_t *)(uintptr_t)c->Esp;
  klog("EXCEPTION 0x%08lX at 0x%08lX in reconstructed %s (original 0x%08X, +0x%lX into the reconstruction): "
       "eax=%08lX ebx=%08lX ecx=%08lX edx=%08lX esi=%08lX edi=%08lX ebp=%08lX esp=%08lX; [esp]=%08X %08X %08X %08X",
       code, (unsigned long)eip, f ? f->name : "?", f ? f->rva + ANALYSED_BASE : 0,
       f ? (unsigned long)(eip - (uint32_t)(uintptr_t)f->target) : 0ul, c->Eax, c->Ebx, c->Ecx, c->Edx, c->Esi, c->Edi, c->Ebp, c->Esp,
       IsBadReadPtr(sp, 16) ? 0 : sp[0], IsBadReadPtr(sp, 16) ? 0 : sp[1], IsBadReadPtr(sp, 16) ? 0 : sp[2], IsBadReadPtr(sp, 16) ? 0 : sp[3]);
  if (f && !f->quarantined) {
    f->quarantined = 1; g_dirty = 1;
    FILE *q = fopen(g_quar, "a");
    if (q) { fprintf(q, "0x%08X %s exception 0x%08lX at +0x%lX\n", f->rva + ANALYSED_BASE, f->name, code, (unsigned long)(eip - (uint32_t)(uintptr_t)f->target)); fclose(q); }
    klog("quarantined %s: it is restored at the next frame and left out from the next start (delete its line in xikit_quarantine.txt to try again)", f->name);
  }
  return EXCEPTION_CONTINUE_SEARCH;            /* the game's own handlers decide, as they would have */
}

static void read_quarantine(void) {
  FILE *q = fopen(g_quar, "r"); char line[256]; int n = 0;
  if (!q) return;
  while (fgets(line, sizeof line, q)) {
    uint32_t a = (uint32_t)strtoul(line, NULL, 16);
    for (int i = 0; i < g_nfn; i++) if (g_fn[i].rva + ANALYSED_BASE == a) { g_fn[i].quarantined = 1; n++; }
  }
  fclose(q);
  if (n) klog("%d functions quarantined by xikit_quarantine.txt", n);
}

/* ------------------------------------------------------------------------------------------------ start */
static DWORD WINAPI start(LPVOID self) {
  char path[MAX_PATH], recon[MAX_PATH];
  GetModuleFileNameA((HMODULE)self, g_dir, MAX_PATH);
  char *slash = strrchr(g_dir, '\\'); if (slash) *slash = 0;
  snprintf(g_logpath, sizeof g_logpath, "%s\\xikit.log", g_dir);
  snprintf(g_ini, sizeof g_ini, "%s\\xikit.ini", g_dir);
  snprintf(g_quar, sizeof g_quar, "%s\\xikit_quarantine.txt", g_dir);
  klog("xikit started in process %lu", (unsigned long)GetCurrentProcessId());
  if (GetFileAttributesA(g_ini) == INVALID_FILE_ATTRIBUTES) {
    FILE *f = fopen(g_ini, "wb"); if (f) { fwrite(DEFAULT_INI, 1, sizeof DEFAULT_INI - 1, f); fclose(f); }
  }
  ini_changed(); read_cfg();
  snprintf(path, sizeof path, "%s\\xikit_manifest.txt", g_dir);
  if (!load_manifest(path)) return 0;
  snprintf(recon, sizeof recon, "%s\\gfx_recon_ingame.dll", g_dir);
  uint32_t size = 0, crc = crc32_file(recon, &size);
  if (!size || size != g_recon_size || crc != g_recon_crc) {
    klog("gfx_recon_ingame.dll is not the one the manifest was made from (size %u, CRC-32 %08X; manifest %u, %08X): nothing done",
         size, crc, g_recon_size, g_recon_crc);
    return 0;
  }
  HMODULE game;
  while (!(game = GetModuleHandleA("FFXiMain.dll"))) Sleep(100);   /* injected possibly long before it loads */
  IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS *)((char *)game + ((IMAGE_DOS_HEADER *)game)->e_lfanew);
  if (nt->FileHeader.TimeDateStamp != g_game_ts) {
    klog("FFXiMain.dll is not the analysed build (TimeDateStamp %08lX, manifest %08X): nothing done", nt->FileHeader.TimeDateStamp, g_game_ts);
    return 0;
  }
  g_rel = (uint32_t)(uintptr_t)game - ANALYSED_BASE;
  g_game_lo = (uint32_t)(uintptr_t)game; g_game_hi = g_game_lo + nt->OptionalHeader.SizeOfImage;
  HMODULE rec = LoadLibraryA(recon);
  if (!rec) { klog("could not load %s (error %lu): nothing done", recon, GetLastError()); return 0; }
  HMODULE pin;
  GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_PIN | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)start, &pin);
  GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_PIN, recon, &pin);
  uintptr_t *xi_rel = (uintptr_t *)GetProcAddress(rec, "xi_rel");
  if (!xi_rel) { klog("the reconstruction DLL exports no xi_rel (not the XI_INGAME build): nothing done"); return 0; }
  *xi_rel = g_rel;
  IMAGE_NT_HEADERS *rnt = (IMAGE_NT_HEADERS *)((char *)rec + ((IMAGE_DOS_HEADER *)rec)->e_lfanew);
  g_recon_lo = (uint32_t)(uintptr_t)rec; g_recon_hi = g_recon_lo + rnt->OptionalHeader.SizeOfImage;
  g_speedfix_loaded = GetModuleHandleA("speedfix_hook.dll") != NULL;
  int missing = 0;
  for (int i = 0; i < g_nfn; i++) {
    Fn *f = &g_fn[i];
    f->site = (uint8_t *)(uintptr_t)(f->rva + ANALYSED_BASE + g_rel);
    f->target = (void *)GetProcAddress(rec, f->exp);
    if (!f->target) { missing++; f->refused = 1; }
    g_by_target[i] = f;
  }
  qsort(g_by_target, g_nfn, sizeof g_by_target[0], cmp_target);
  if (g_cfg.count_calls) {
    g_stubs = (uint8_t *)VirtualAlloc(NULL, MAXFN * STUB_LEN, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (g_stubs) for (int i = 0; i < g_nfn; i++) {
      Fn *f = &g_fn[i]; uint8_t *s = g_stubs + i * STUB_LEN;
      if (!f->target) continue;
      uint32_t cnt = (uint32_t)(uintptr_t)&f->calls;
      s[0] = 0xF0; s[1] = 0xFF; s[2] = 0x05; memcpy(s + 3, &cnt, 4);          /* lock inc dword [&calls] */
      make_jmp(s + 7, s + 7, f->target);                                      /* jmp reconstruction */
      f->stub = s;
    }
    if (g_stubs) { DWORD old; VirtualProtect(g_stubs, MAXFN * STUB_LEN, PAGE_EXECUTE_READ, &old); FlushInstructionCache(GetCurrentProcess(), g_stubs, MAXFN * STUB_LEN); }
  }
  read_quarantine();
  AddVectoredExceptionHandler(1, on_exception);
  klog("FFXiMain.dll at %p (delta %+ld), reconstruction at %p; %d functions in the manifest (%d without an export); speedfix_hook %s",
       game, (long)g_rel, rec, g_nfn, missing, g_speedfix_loaded ? "loaded" : "not loaded");
  /* the device: [[g_pCDx]+0xC], once the game has made it */
  void ***cdx = (void ***)(uintptr_t)(G_PCDX + g_rel); void **dev = NULL;
  for (;;) { void **c = (void **)*cdx; if (c && (dev = (void **)c[CDX_DEVICE_OFF / 4]) != NULL) break; Sleep(200); }
  g_orig_vt = (void **)*dev;
  memcpy(g_vt, g_orig_vt, sizeof g_vt);
  g_vt[SLOT_PRESENT] = (void *)kit_present;
  *(void ***)dev = g_vt;                      /* this object only: the driver's own vtable is not written */
  klog("device %p hooked (Present); swaps are applied at the next frame (swap=%d)", dev, g_cfg.swap);
  if (g_cfg.profile) {
    char csv[MAX_PATH]; snprintf(csv, sizeof csv, "%s\\d3dprof_%lu.csv", g_dir, (unsigned long)GetCurrentProcessId());
    d3dprof_attach(dev, g_rel, g_game_lo, g_game_hi, csv);
    klog("profiler attached: %s", csv);
  }
  /* watchdog: if the game ever makes a new device object, give that one the hook too (Reset keeps the object) */
  for (void **last = dev;;) {
    Sleep(1000);
    void **c = (void **)*cdx; void **d = c ? (void **)c[CDX_DEVICE_OFF / 4] : NULL;
    if (!d || d == last) continue;
    void **vt = (void **)*d;
    if (vt != g_vt) {
      g_orig_vt = vt; memcpy(g_vt, vt, sizeof g_vt); g_vt[SLOT_PRESENT] = (void *)kit_present;
      *(void ***)d = g_vt;
      klog("a new device %p: hooked (Present)%s", d, g_cfg.profile ? "; the profiler stays on the old one" : "");
    }
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
