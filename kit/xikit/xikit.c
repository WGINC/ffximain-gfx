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
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../../sdk/include/xikit_mod.h"
#ifdef _MSC_VER
#include <intrin.h>
#else
#include <cpuid.h>
#endif

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
void d3dprof_set_shift(const int32_t *tab);   /* another build's moved code ranges: call sites reported in the analysed build's numbering */
void d3dprof_trace_start(int label); void d3dprof_trace_relabel(int label); void d3dprof_trace_hookbufs(int on); void d3dprof_trace_stop(void); void d3dprof_trace_frames(int *n);
int d3dprof_trace_write(const char *path, const char *note);
extern int d3dprof_call_time; extern uint32_t d3dprof_via_stk[64]; extern volatile uint32_t d3dprof_via_depth; const char *d3dprof_method(int slot);

typedef struct {
  uint32_t rva;                        /* entry - 0x10000000 */
  int flags;                           /* 1 SEH, 2 speedfix site, 4 untested, 8 register input (no mod hook) */
  char exp[160], name[96];
  uint8_t expect[20];                  /* analysed bytes, entry-4 .. entry+16 */
  int nrel; int8_t rel[8];             /* relocated dwords touching that window (offsets -3..19) */
  uint8_t *site;                       /* live entry */
  void *target;                        /* the reconstruction's export */
  uint8_t *stub;                       /* counting stub, or NULL */
  uint8_t saved[JMP_LEN];              /* live bytes the jmp replaced */
  int patched, refused, quarantined, wanted;
  int bs_susp, bs_test;                /* bisect: still a suspect / in the set being tested */
  volatile LONG calls; LONG calls_reported;
  int tlen, nfix; int8_t tfix[4];      /* trampoline (manifest T=): whole instructions copied from the entry, rel32 to re-aim */
  void *mod_target; int mod;           /* a mod's replacement (sdk/: xikit_mod.h), and which mod (index into g_mods + 1) */
  int mod_on_recon;                    /* the hook's previous is the reconstruction (no trampoline) */
  void *tramp;                         /* the trampoline, once built (from the untouched entry; a reload reuses it) */
  void *cap_stub; volatile LONG cap_done;  /* capture= (an entry snapshot, below): the stub while captures remain */
  void *jmp_to;                        /* where the jmp the kit wrote at the entry goes (while patched) */
} Fn;

typedef struct {
  int swap, count_calls, profile, respect_speedfix, include_flagged, include_untested, report_every, hotkey, log_each, bisect, trace;
  int ab, ab_after, ab_phase, ab_rounds, ab_settle, spike_ms;
  char only[4096], exclude[4096];
} Cfg;

static Fn g_fn[MAXFN]; static int g_nfn;
/* the reconstruction DLL's code symbols (manifest !code RVA SYMBOL): which function a fault inside it is in */
#define MAXCODE 4096
typedef struct { uint32_t rva; int fn; char sym[160]; } Code;   /* (sym as long as Fn.exp: a cut name would match nothing) */
static Code g_code[MAXCODE]; static int g_ncode;
static Fn *g_by_target[MAXFN];         /* sorted by reconstruction address, for the exception handler */
static Cfg g_cfg;
static char g_dir[MAX_PATH], g_ini[MAX_PATH], g_logpath[MAX_PATH], g_quar[MAX_PATH];
static char g_out[MAX_PATH];           /* where everything the kit writes goes (log_folder=1: xikit_logs\\<start>_<pid>); its inputs stay in g_dir */
static FILETIME g_ini_time;
static CRITICAL_SECTION g_loglock;
static CRITICAL_SECTION g_susplock;   /* one thread at a time suspends the others (apply, capture): two doing it at once could suspend each other */
static uint32_t g_rel, g_game_lo, g_game_hi, g_recon_lo, g_recon_hi;
static uint8_t *g_stubs;
static void **g_orig_vt; static void *g_vt[NSLOTS];
static DWORD g_render_thread;
static uint32_t g_frame; static int g_master_off, g_hotkey_down, g_dirty = 1, g_speedfix_loaded;
static uint32_t g_game_ts, g_recon_size, g_recon_crc;
/* Another build of the game (kit/xikit/builds/<stamp>.json, pipeline/build_translate.py): its manifest,
   xikit_manifest_<stamp>.txt, carries the analysed build's code ranges that moved there (!shift LO HI SHIFT).
   g_shift: count, then lo, hi, shift; the same table goes to the reconstruction DLL and the mods (xi_shift). The
   kit keeps analysed addresses everywhere (manifest RVAs, logs, profiles, traces) and translates at the boundary. */
static int32_t g_shift[25];
#define TEXT_LO 0x10001000u                    /* the analysed build's code: .text up to .rdata */
#define TEXT_HI 0x10329000u
static uint32_t live_of(uint32_t ea) {         /* an analysed address -> where it is in the running game */
  int32_t d = 0;
  if (ea >= TEXT_LO && ea < TEXT_HI)
    for (int i = 0; i < g_shift[0]; i++) if (ea >= (uint32_t)g_shift[1 + 3 * i] && ea < (uint32_t)g_shift[2 + 3 * i]) d = g_shift[3 + 3 * i];
  return ea + g_rel + d;
}
static uint32_t analysed_of(uint32_t a) {      /* an address in the running game -> the analysed build's */
  uint32_t ea = a - g_rel;
  for (int i = 0; i < g_shift[0]; i++) {
    uint32_t lo = (uint32_t)g_shift[1 + 3 * i], hi = (uint32_t)g_shift[2 + 3 * i]; int32_t d = g_shift[3 + 3 * i];
    if (ea >= lo + d && ea < hi + d) return ea - d;
  }
  return ea;
}
static int g_trace_attached, g_tr_state, g_tr_wait, g_tr_prev_off, g_tr_key, g_tr_n, g_tr_after, g_tr_every, g_tr_count, g_tr_state_started, g_tr_buffers, g_tr_alt;
static DWORD g_tr_next;
static int g_tr_pictures, g_tr_labels[32];       /* trace_pictures=K; the label each recorded frame got */
#define TR_FRAMES 4
#define TR_ALT_FRAMES 17           /* alternating trace: off, on, off, ... off: 8 swapped frames, each between two unswapped */

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
  "; never these (same forms). Gfx_DestroyD3DHolder faults when swapped, cause not found (docs/GAPS.md)\r\n"
  "exclude=Gfx_DestroyD3DHolder\r\n"
  "; 1: also swap functions flagged SEH or speedfix-site (see the manifest)\r\n"
  "include_flagged=0\r\n"
  "; 1: also swap functions whose gate runs did not reach all of the original's code (flag 4 in the manifest).\r\n"
  ";    Their PASS says nothing about the code no compared trial reached; only= / exclude= still apply\r\n"
  "include_untested=0\r\n"
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
  "; 1: bisect mode: with the screen broken press Ctrl+Alt+F10 or create xikit_bisect_bad.txt; the kit then swaps a part of\r\n"
  ";    the functions that have run, and you answer F10 / xikit_bisect_bad.txt if the screen is still broken or F11 /\r\n"
  ";    xikit_bisect_good.txt if it looks right. It narrows to the smallest set that breaks the screen (one function, or\r\n"
  ";    several that only break it together) and logs it (xikit.log, xikit_bisect.txt). F12 / xikit_bisect_stop.txt ends it\r\n"
  ";    and swaps everything again (culprits found included)\r\n"
  "bisect=0\r\n"
  "; 1: Ctrl+Alt+F8 records the game's Direct3D calls for 17 frames with the swaps flipped at every frame (off, on, off,\r\n"
  ";    ... off) and writes xikit_trace_<pid>_<n>.txt (kit/d3dprof/trace_diff.py compares each swapped frame with the\r\n"
  ";    unswapped frames either side of it: which replaced function makes the game draw differently). Stand still in a\r\n"
  ";    scene first; takes effect at the next start\r\n"
  "trace=0\r\n"
  "; 0: the older trace instead, 4 frames with every swap off, then 4 with the swaps on\r\n"
  "trace_alternate=1\r\n"
  "; the keys do not reach every game window: with trace=1, trace_after=SECONDS runs the trace by itself that many seconds after\r\n"
  ";    the start (then every trace_every seconds, trace_count times in all; each writes xikit_trace_<pid>_<n>.txt). Creating\r\n"
  ";    the file xikit_trace_now.txt next to xikit.dll starts one too, and with bisect=1 xikit_bisect_bad.txt / _good.txt /\r\n"
  ";    _stop.txt answer like F10 / F11 / F12\r\n"
  "trace_after=0\r\ntrace_every=120\r\ntrace_count=3\r\n"
  "; K > 0: an alternating trace also saves the picture of frames 8-2K .. 8+2K (K swapped frames each with both\r\n"
  ";    neighbours; about 8 MB a frame at 1920x1080); kit/d3dprof/pictures.py compares them\r\n"
  "trace_pictures=0\r\n"
  "; 1: the trace also hashes what the game writes into vertex and index buffers (their Lock/Unlock are wrapped)\r\n"
  "trace_buffers=1\r\n"
  "; 1: d3dprof in the same process (d3dprof_<pid>.csv); takes effect at the next start\r\n"
  "profile=0\r\n"
  "; 1 (with profile=1): also the time inside each Direct3D call, per call site, on the render thread (us_per_call in\r\n"
  ";    d3dprof_<pid>.csv); takes effect at the next start\r\n"
  "call_time=0\r\n"
  "; mod DLLs next to xikit.dll, comma-separated (sdk/, docs/SDK.md); loaded at the start; their hooks stay on with\r\n"
  ";    swap=0 and go off with the swaps for F9 and a trace's unswapped frames\r\n"
  "mods=\r\n"
  "; 1: load each mod from a copy and load it again when its file changes (a rebuilt mod, while the game runs; also\r\n"
  ";    on creating xikit_reload_now.txt); takes effect at the next start\r\n"
  "mod_reload=0\r\n"
  "; record real calls for the gate (docs/KIT.md): manifest names, comma-separated; each writes an .xcap\r\n"
  "; capture_follow: for the call's own pointers (registers, the first 8 arguments), also the pages after each, up\r\n"
  ";    to this many (an object or list longer than a page)\r\n"
  "capture=\r\ncapture_count=1\r\ncapture_after=0\r\ncapture_pages=20000\r\ncapture_follow=16\r\n"
  "; 1: sample the render thread every sample_ms (xikit_samples_<pid>.csv; kit/d3dprof/samples.py reads it); frame timing\r\n"
  ";    (xikit_timing_<pid>.csv) is always written, every report_every frames; takes effect at the next start\r\n"
  "sample=0\r\nsample_ms=1\r\n"
  "; 1: measure the call sites listed in xikit_facts.txt (pipeline/fact_sites.py): what each callee pops, where it\r\n"
  ";    goes; xikit_facts_<pid>.csv every report_every frames; takes effect at the next start\r\n"
  "facts=0\r\n"
  "; 1: everything the kit writes (this log, profiles, traces, timing, samples, captures, A/B results) goes into a new\r\n"
  ";    folder per start, xikit_logs\\<date>_<time>_<pid>; 0: next to xikit.dll as before. Read at the start\r\n"
  "log_folder=1\r\n"
  "; 1: an A/B test with no keys: ab_after seconds from now (or from the start), the kit switches everything it adds\r\n"
  ";    off (A: no swaps, no mod hooks, as F9) and on (B) in phases of ab_phase seconds, A B B A for each of ab_rounds\r\n"
  ";    rounds (ABBA cancels a steady drift in the scene). The first ab_settle frames of each phase are left out. Each\r\n"
  ";    phase's frame times and a summary go to xikit_ab_<pid>.csv and xikit.log. Stand still while it runs. Setting\r\n"
  ";    ab=0 stops it; set it to 0 and back to 1 to run another. Not affected: what a mod did once at its start\r\n"
  ";    (speedfix's cpu_tier, texture_compression)\r\n"
  "ab=0\r\nab_after=60\r\nab_phase=30\r\nab_rounds=4\r\nab_settle=60\r\n"
  "; every frame slower than this many ms (Present to Present) goes to xikit_spikes_<pid>.csv with its frame number (the\r\n"
  ";    resource log's frame numbers match, with profile=1: what was created in a stutter); 0 = off\r\n"
  "spike_ms=50\r\n";

static void read_cfg(void) {
  Cfg c; memset(&c, 0, sizeof c);
  c.swap = GetPrivateProfileIntA("xikit", "swap", 0, g_ini);
  c.count_calls = GetPrivateProfileIntA("xikit", "count_calls", 1, g_ini);
  c.profile = GetPrivateProfileIntA("xikit", "profile", 0, g_ini);
  c.respect_speedfix = GetPrivateProfileIntA("xikit", "respect_speedfix", 1, g_ini);
  c.include_flagged = GetPrivateProfileIntA("xikit", "include_flagged", 0, g_ini);
  c.include_untested = GetPrivateProfileIntA("xikit", "include_untested", 0, g_ini);
  c.report_every = GetPrivateProfileIntA("xikit", "report_every", 600, g_ini);
  c.hotkey = GetPrivateProfileIntA("xikit", "hotkey", 1, g_ini);
  c.log_each = GetPrivateProfileIntA("xikit", "log_each", 0, g_ini);
  c.bisect = GetPrivateProfileIntA("xikit", "bisect", 0, g_ini);
  c.trace = GetPrivateProfileIntA("xikit", "trace", 0, g_ini);
  c.ab = GetPrivateProfileIntA("xikit", "ab", 0, g_ini);
  c.spike_ms = GetPrivateProfileIntA("xikit", "spike_ms", 50, g_ini);
  c.ab_after = GetPrivateProfileIntA("xikit", "ab_after", 60, g_ini);
  c.ab_phase = GetPrivateProfileIntA("xikit", "ab_phase", 30, g_ini);
  c.ab_rounds = GetPrivateProfileIntA("xikit", "ab_rounds", 4, g_ini);
  c.ab_settle = GetPrivateProfileIntA("xikit", "ab_settle", 60, g_ini);
  if (c.ab_after < 0) c.ab_after = 0;
  if (c.ab_phase < 1) c.ab_phase = 1;
  if (c.ab_rounds < 1) c.ab_rounds = 1;
  if (c.ab_rounds > 16) c.ab_rounds = 16;
  if (c.ab_settle < 0) c.ab_settle = 0;
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

static int g_bis_on;
static int g_bis_found[64], g_bis_nfound;           /* culprits found this session: kept on the original code */

static int base_wanted(const Fn *f) {
  if (f->mod_target || (f->cap_stub && !f->cap_done)) return 0;   /* a mod's hook or a capture: wanted() below; never a bisect suspect */
  if (!g_cfg.swap || g_master_off || f->refused || f->quarantined || !f->target) return 0;
  if ((f->flags & 1) && !g_cfg.include_flagged) return 0;
  if ((f->flags & 2) && g_cfg.respect_speedfix && g_speedfix_loaded && !g_cfg.include_flagged) return 0;
  if ((f->flags & 4) && !g_cfg.include_untested) return 0;
  if (g_cfg.only[0] && !listed(g_cfg.only, f)) return 0;
  if (g_cfg.exclude[0] && listed(g_cfg.exclude, f)) return 0;
  for (int k = 0; k < g_bis_nfound; k++) if (&g_fn[g_bis_found[k]] == f) return 0;
  return 1;
}

static int wanted(const Fn *f) {
  /* a mod's hook: swap=0 leaves it on, F9 and a trace's unswapped frames turn it off. The quarantine is about the
     reconstruction's code, which a hook with a trampoline never runs; a hook whose previous is the reconstruction is
     refused at hook() if that function is quarantined, and turned off if it is quarantined later (f->mod_on_recon) */
  if (f->mod_target) return !g_master_off && !f->refused && !(f->quarantined && f->mod_on_recon);
  if (f->cap_stub && !f->cap_done) return !f->refused;      /* a capture runs whatever the swaps say */
  int w = base_wanted(f);
  return g_bis_on ? (w && f->bs_test) : w;
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
      { unsigned lo, hi; int d;
        if (sscanf(line + 1, "shift %x %x %d", &lo, &hi, &d) == 3 && g_shift[0] < 8) {
          int i = g_shift[0]++; g_shift[1 + 3 * i] = (int32_t)lo; g_shift[2 + 3 * i] = (int32_t)hi; g_shift[3 + 3 * i] = d; } }
      if (sscanf(line + 1, "recon_size %u", &v) == 1) g_recon_size = v;
      if (g_ncode < MAXCODE && sscanf(line + 1, "code %x %159s", &v, g_code[g_ncode].sym) == 2) { g_code[g_ncode].rva = v; g_code[g_ncode].fn = -1; g_ncode++; }
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
    char *t = strstr(line, " T=");                  /* (a manifest from before the SDK has none: no trampolines) */
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

static int g_nsites; static int sites_pending(void); static void apply_sites(const uint32_t *eips, int n, char msgs[][200], int *nmsg); static void report_facts(void);
/* where the entry of f should jump now, or NULL (the game's own bytes): a mod's hook, a capture stub, the counting
   stub, or the reconstruction */
static void *desired(const Fn *f) {
  if (!wanted(f)) return NULL;
  if (f->mod_target) return f->mod_target;
  if (f->cap_stub && !f->cap_done) return f->cap_stub;
  return f->stub ? (void *)f->stub : f->target;
}

/* bring every function to its wanted state; called from Present on the render thread. A patched entry is
   compared with the jmp the kit wrote there (jmp_to) before it is restored or re-aimed */
static void apply(void) {
  static Th th[1024]; static uint32_t eips[1024]; static void *want[MAXFN];
  int todo = 0;
  for (int i = 0; i < g_nfn; i++) {
    Fn *f = &g_fn[i]; want[i] = desired(f); f->wanted = want[i] != NULL;
    todo += (f->patched ? f->jmp_to : NULL) != want[i];
  }
  if (!todo && !(g_nsites && sites_pending())) { g_dirty = 0; return; }
  EnterCriticalSection(&g_susplock);
  int n = suspend_others(th, 1024);
  if (n < 0) { LeaveCriticalSection(&g_susplock); klog("could not list threads: nothing written this frame"); return; }
  for (int i = 0; i < n; i++) {
    CONTEXT c; memset(&c, 0, sizeof c); c.ContextFlags = CONTEXT_CONTROL;
    eips[i] = GetThreadContext(th[i].h, &c) ? c.Eip : 0;
  }
  int patched = 0, restored = 0, busy = 0, refused = 0, retargeted = 0;
  /* messages are kept until the threads run again: logging takes locks another thread may hold */
  static char msgs[64][200]; int nmsg = 0;
  for (int i = 0; i < g_nfn; i++) {
    Fn *f = &g_fn[i];
    if ((f->patched ? f->jmp_to : NULL) == want[i]) continue;
    int inside = 0;
    for (int t = 0; t < n; t++) if (eips[t] >= (uint32_t)(uintptr_t)f->site && eips[t] < (uint32_t)(uintptr_t)f->site + JMP_LEN) inside = 1;
    if (inside) { busy++; continue; }
    uint8_t jmp[JMP_LEN], cur[JMP_LEN];
    if (!f->patched) {
      int d = check_bytes(f);
      if (d != -1) {
        f->refused = 1; refused++;
        if (nmsg < 64) snprintf(msgs[nmsg++], 200, "refused %s (0x%08X): live byte at entry%+d is not the analysed build's",
                                f->name, f->rva + ANALYSED_BASE, d);
        continue;
      }
      make_jmp(jmp, f->site, want[i]);
      memcpy(f->saved, f->site, JMP_LEN);
      if (write5(f->site, jmp)) { f->patched = 1; f->jmp_to = want[i]; patched++;
        if (g_cfg.log_each && nmsg < 64) snprintf(msgs[nmsg++], 200, "swapped %s (0x%08X)", f->name, f->rva + ANALYSED_BASE); }
      continue;
    }
    make_jmp(cur, f->site, f->jmp_to);
    if (memcmp(f->site, cur, JMP_LEN)) {
      f->refused = 1; f->patched = 0; f->jmp_to = NULL; refused++;
      if (nmsg < 64) snprintf(msgs[nmsg++], 200, "not restored %s (0x%08X): its entry no longer holds the kit's jmp (another hook wrote there)",
                              f->name, f->rva + ANALYSED_BASE);
      continue;
    }
    if (!want[i]) {
      if (write5(f->site, f->saved)) { f->patched = 0; f->jmp_to = NULL; restored++;
        if (g_cfg.log_each && nmsg < 64) snprintf(msgs[nmsg++], 200, "restored %s (0x%08X)", f->name, f->rva + ANALYSED_BASE); }
    } else {
      make_jmp(jmp, f->site, want[i]);
      if (write5(f->site, jmp)) { f->jmp_to = want[i]; retargeted++; }
    }
  }
  if (g_nsites) apply_sites(eips, n, msgs, &nmsg);
  resume_all(th, n);
  LeaveCriticalSection(&g_susplock);
  for (int i = 0; i < nmsg; i++) klog("%s", msgs[i]);
  int now = 0; for (int i = 0; i < g_nfn; i++) now += g_fn[i].patched && !g_fn[i].mod_target && !(g_fn[i].cap_stub && !g_fn[i].cap_done);
  klog("frame %u: %d swapped, %d restored, %d re-aimed, %d refused, %d waiting (a thread was inside the entry); %d of %d now run reconstructed code",
       g_frame, patched, restored, retargeted, refused, busy, now, g_nfn);
  g_dirty = busy > 0 || (g_nsites && sites_pending());
}


/* ------------------------------------------------------------------------------------------------ bisect */
/* The screen is wrong with everything swapped; which functions make it so? Candidates are the swapped functions that
   have run (a function that never ran cannot have drawn anything). No single culprit is assumed: on 6 Oct the picture
   broke with 7 functions swapped and was right with each of the halves a bisect tried, so it took two of them
   together. This is delta debugging (Zeller's ddmin): the suspects C (known to break the screen) are cut into n pieces;
   each piece is swapped alone, then each complement (C without that piece); a set that breaks the screen becomes the
   new C. When no piece and no complement breaks it, n doubles; when n has reached the size of C, C is minimal: taking
   any one function out of it gives a right screen. From 8 suspects down, each is left out in turn instead (a set
   that is still wrong without it drops it): at most 8 answers. The user answers each step: still wrong (F10 /
   xikit_bisect_bad.txt) or right (F11 / xikit_bisect_good.txt). */
static int g_bis_n, g_bis_step; static int g_bis_idx[MAXFN];     /* C: indices into g_fn */
static int g_bis_phase;                                          /* 0 off, 1 pieces, 2 complements, 3 leave one out */
static int g_bis_parts, g_bis_i;                                 /* n, and the piece being tested */
static int g_bis_t[MAXFN], g_bis_tn;                             /* the set being tested */

static void bis_note(const char *fmt, ...) {
  char buf[400]; va_list ap; va_start(ap, fmt); vsnprintf(buf, sizeof buf, fmt, ap); va_end(ap);
  klog("bisect: %s", buf);
  char path[MAX_PATH]; snprintf(path, sizeof path, "%s\\xikit_bisect.txt", g_out);
  FILE *f = fopen(path, "a"); if (f) { fprintf(f, "%s\n", buf); fclose(f); }
}

static void bis_list(const char *what, const int *idx, int n) {
  char buf[360]; int len = 0; buf[0] = 0;
  for (int i = 0; i < n && len < 300; i++) len += snprintf(buf + len, sizeof buf - len, "%s%s", i ? ", " : "", g_fn[idx[i]].name);
  bis_note("%s (%d): %s%s", what, n, buf, len >= 300 ? " ..." : "");
}

static void bis_test(const int *idx, int n) {
  for (int i = 0; i < g_nfn; i++) g_fn[i].bs_test = 0;
  for (int i = 0; i < n; i++) g_fn[idx[i]].bs_test = 1;
  g_dirty = 1;
}

static void bis_finish(void) {                 /* C is 1-minimal: report it, keep it on the original code */
  if (g_bis_n == 1)
    bis_note("CULPRIT: %s (0x%08X, %s) - the screen is wrong with it swapped and right without it. It stays on the original code for the rest of this run (put it in exclude= to keep it so)",
             g_fn[g_bis_idx[0]].name, g_fn[g_bis_idx[0]].rva + ANALYSED_BASE, g_fn[g_bis_idx[0]].exp);
  else {
    bis_note("CULPRITS TOGETHER: these %d functions swapped together make the screen wrong; with any one of them left out it is right. They stay on the original code for the rest of this run", g_bis_n);
    bis_list("culprits", g_bis_idx, g_bis_n);
  }
  for (int k = 0; k < g_bis_n && g_bis_nfound < 64; k++) g_bis_found[g_bis_nfound++] = g_bis_idx[k];
  g_bis_on = 0; g_bis_phase = 0; g_dirty = 1;
}

static void bis_next(void) {                    /* set up the next set to test from C, n and the piece index */
  for (;;) {
    if (g_bis_n == 1) { bis_finish(); return; }
    /* 8 suspects or fewer: each is left out in turn (at most 8 answers; ddmin would need about 20 for a pair of 7) */
    if (g_bis_n <= 8 && g_bis_phase != 3) { g_bis_phase = 3; g_bis_i = 0; }
    if (g_bis_phase == 3) { if (g_bis_i >= g_bis_n) { bis_finish(); return; } break; }
    if (g_bis_parts > g_bis_n) g_bis_parts = g_bis_n;
    if (g_bis_i >= g_bis_parts) {               /* every piece (or complement) tested and right */
      if (g_bis_phase == 1 && g_bis_parts > 2) { g_bis_phase = 2; g_bis_i = 0; continue; }
      if (g_bis_parts >= g_bis_n) { bis_finish(); return; }
      g_bis_parts = g_bis_parts * 2 > g_bis_n ? g_bis_n : g_bis_parts * 2; g_bis_phase = 1; g_bis_i = 0; continue;
    }
    break;
  }
  g_bis_tn = 0;
  if (g_bis_phase == 3) {
    for (int k = 0; k < g_bis_n; k++) if (k != g_bis_i) g_bis_t[g_bis_tn++] = g_bis_idx[k];
    g_bis_step++; bis_test(g_bis_t, g_bis_tn);
    bis_note("step %d: %d suspects; swapping all of them but %s. Screen still wrong: F10 / xikit_bisect_bad.txt; right: F11 / xikit_bisect_good.txt",
             g_bis_step, g_bis_n, g_fn[g_bis_idx[g_bis_i]].name);
    return;
  }
  int lo = g_bis_i * g_bis_n / g_bis_parts, hi = (g_bis_i + 1) * g_bis_n / g_bis_parts;
  for (int k = 0; k < g_bis_n; k++)
    if ((k >= lo && k < hi) == (g_bis_phase == 1)) g_bis_t[g_bis_tn++] = g_bis_idx[k];
  g_bis_step++; bis_test(g_bis_t, g_bis_tn);
  bis_note("step %d: %d suspects in %d pieces; swapping %s %d (%d functions; the rest run the original code). Screen still wrong: F10 / xikit_bisect_bad.txt; right: F11 / xikit_bisect_good.txt",
           g_bis_step, g_bis_n, g_bis_parts, g_bis_phase == 1 ? "piece" : "all but piece", g_bis_i + 1, g_bis_tn);
  if (g_bis_tn <= 12) bis_list("swapped now", g_bis_t, g_bis_tn);
}

static void bis_start(void) {
  g_bis_n = 0; g_bis_step = 0;
  for (int i = 0; i < g_nfn; i++) {
    Fn *f = &g_fn[i]; f->bs_test = 0;
    int w = base_wanted(f);
    if (w && f->stub && f->calls == 0) w = 0;
    if (w) g_bis_idx[g_bis_n++] = i;
  }
  if (g_bis_n == 0) { bis_note("no swapped function has run yet; nothing to bisect"); return; }
  g_bis_on = 1; bis_note("started: %d functions have run since they were swapped", g_bis_n);
  if (g_bis_n <= 12) bis_list("suspects", g_bis_idx, g_bis_n);
  g_bis_parts = 2; g_bis_phase = 1; g_bis_i = 0;
  bis_next();
}

static void bis_stop(void) {
  g_bis_on = 0; g_bis_phase = 0; g_dirty = 1;
  bis_note("ended: all functions swapped again%s", g_bis_nfound ? ", the culprits found included" : "");
  g_bis_nfound = 0;
}

static void bis_answer(int bad) {
  if (!g_bis_on) { if (bad) bis_start(); return; }
  if (bad) {                                    /* the tested set breaks the screen: it becomes C */
    memcpy(g_bis_idx, g_bis_t, g_bis_tn * sizeof(int));
    int was = g_bis_phase; g_bis_n = g_bis_tn;
    if (was != 3) {                             /* (leaving one out: the next suspect has moved into its place) */
      g_bis_parts = was == 1 ? 2 : (g_bis_parts - 1 < 2 ? 2 : g_bis_parts - 1);
      g_bis_phase = 1; g_bis_i = 0;
    }
  } else g_bis_i++;
  bis_next();
}

/* for the test host, which plays the render thread and cannot press keys */
__declspec(dllexport) void xikit_bisect_answer(int bad) { if (bad == 2) bis_stop(); else bis_answer(bad); }

/* ------------------------------------------------------------------------------------------------ reports */
static void report_calls(void) {
  char path[MAX_PATH]; snprintf(path, sizeof path, "%s\\xikit_calls_%lu.csv", g_out, (unsigned long)GetCurrentProcessId());
  FILE *f = fopen(path, "w"); if (!f) return;
  fprintf(f, "# frame %u; calls of each swapped function since it was swapped, and in the last window\n", g_frame);
  fprintf(f, "address,name,swapped,calls_total,calls_window\n");
  for (int i = 0; i < g_nfn; i++) {
    Fn *x = &g_fn[i]; LONG c = x->calls;
    if (!x->stub || x->mod_target || (!x->patched && !c)) continue;      /* (a mod's hook bypasses the counting stub: not listed) */
    fprintf(f, "0x%08X,%s,%d,%ld,%ld\n", x->rva + ANALYSED_BASE, x->name, x->patched, (long)c, (long)(c - x->calls_reported));
    x->calls_reported = c;
  }
  fclose(f);
}

/* ------------------------------------------------------------------------------------------------ Present */
/* A/B trace. trace_alternate=1 (the default): swaps off (wait until the restores are written), then 17 frames recorded
   with the swaps flipped at every Present (off, on, off, ... off), so each swapped frame lies between two unswapped
   ones a frame apart and what animates can be told from what the swaps change (trace_diff.py). trace_alternate=0:
   swaps off (wait), 4 frames recorded, swaps on (wait), 4 frames recorded. Either way the file is then
   written. Started by Ctrl+Alt+F8, by creating the file xikit_trace_now.txt next to xikit.dll (the kit deletes it), or by
   the timer (trace_after / trace_every / trace_count in xikit.ini: the keys do not reach every game window). */
static void trace_begin(const char *why) {
  if (g_tr_state) return;
  int hooks = 0; for (int i = 0; i < g_nfn; i++) hooks += g_fn[i].mod_target != NULL;
  if (!g_cfg.swap && !hooks) { klog("trace: swap=0 in xikit.ini and no mod hooks, so the two halves would be the same; set swap=1"); return; }
  if (g_tr_buffers) d3dprof_trace_hookbufs(1);     /* buffers bound from now on get Lock/Unlock wrapped (d3dprof.c) */
  g_tr_prev_off = g_master_off; g_master_off = 1; g_dirty = 1; g_tr_state = 1; g_tr_wait = 0;
  klog("trace %d (%s): swaps off, waiting for the restores", g_tr_n + 1, why);
}

static int g_tr_maxon;
static void trace_finish(const char *mode) {
  char p[MAX_PATH], note[260]; int now = 0; for (int i = 0; i < g_nfn; i++) now += g_fn[i].patched && !g_fn[i].mod_target;
  if (g_tr_maxon > now) now = g_tr_maxon;      /* (alternating: the last frame is an unswapped one) */
  d3dprof_trace_stop(); g_master_off = g_tr_prev_off; g_dirty = 1; g_tr_state = 0; g_tr_maxon = 0;
  g_tr_n++; g_tr_state_started = 0;
  snprintf(p, sizeof p, "%s\\xikit_trace_%lu_%d.txt", g_out, (unsigned long)GetCurrentProcessId(), g_tr_n);
  if (!strcmp(mode, "alternating"))
    snprintf(note, sizeof note, "alternating: label 0 = swaps off, 1 = swaps on (%d functions swapped), 2 = mixed; %d frames; FFXiMain.dll at +0x%X; reconstruction at 0x%08X", now, TR_ALT_FRAMES, g_rel, g_recon_lo);
  else
    snprintf(note, sizeof note, "label 0 = swaps off, label 1 = swaps on (%d functions swapped); %d frames each; FFXiMain.dll at +0x%X; reconstruction at 0x%08X", now, TR_FRAMES, g_rel, g_recon_lo);
  klog("trace: %s %s", d3dprof_trace_write(p, note) ? "written" : "could not write", p);
}

static void trace_step(void) {
  int n = 0;
  int down = (GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState(VK_MENU) & 0x8000) && (GetAsyncKeyState(VK_F8) & 0x8000);
  if (down && !g_tr_key) trace_begin("key");
  g_tr_key = down;
  if (g_frame % 30 == 0) {
    char p[MAX_PATH]; snprintf(p, sizeof p, "%s\\xikit_trace_now.txt", g_dir);
    if (DeleteFileA(p)) trace_begin("file");
  }
  if (g_tr_after > 0 && g_tr_n + g_tr_state_started < g_tr_count && !g_tr_state) {
    DWORD now = GetTickCount();
    if (!g_tr_next) g_tr_next = now + (DWORD)g_tr_after * 1000;
    if ((int)(now - g_tr_next) >= 0) { g_tr_state_started++; g_tr_next = now + (DWORD)g_tr_every * 1000; trace_begin("timer"); }
  }
  switch (g_tr_state) {
  case 1: if (++g_tr_wait >= 8 && !g_dirty) {
            d3dprof_trace_start(0);
            if (g_tr_alt) { g_tr_state = 5; klog("trace: recording %d frames, the swaps alternating (off, on, off, ...)", TR_ALT_FRAMES); }
            else { g_tr_state = 2; klog("trace: recording %d frames, swaps off", TR_FRAMES); }
          } break;
  case 5: d3dprof_trace_frames(&n);           /* (the frame that just ended was frame n-1; the label of frame n is set in trace_label) */
          if (n >= TR_ALT_FRAMES) { trace_finish("alternating"); break; }
          g_master_off = !g_master_off; g_dirty = 1;
          break;
  case 2: d3dprof_trace_frames(&n);
          if (n >= TR_FRAMES) { d3dprof_trace_stop(); g_master_off = 0; g_dirty = 1; g_tr_state = 3; g_tr_wait = 0; klog("trace: swaps on, waiting for the swaps"); }
          break;
  case 3: if (++g_tr_wait >= 8 && !g_dirty) { d3dprof_trace_start(1); g_tr_state = 4; klog("trace: recording %d frames, swaps on", TR_FRAMES); } break;
  case 4: d3dprof_trace_frames(&n);
          if (n >= TR_FRAMES) trace_finish("halves");
          break;
  }
}

/* the alternating trace: called after apply(), before the frame's own calls. A frame is labelled by what apply()
   achieved: 0 = every function restored, 1 = every one swapped, 2 = mixed (a thread was inside an entry, so some
   were left as they were; trace_diff.py leaves such a frame out). */
static void trace_label(void) {
  if (g_tr_state != 5) return;
  int on = 0; for (int i = 0; i < g_nfn; i++) on += g_fn[i].patched && !g_fn[i].mod_target;
  int lab = g_dirty ? 2 : g_master_off ? 0 : 1, fr = 0; d3dprof_trace_frames(&fr);
  d3dprof_trace_relabel(lab);
  if (fr >= 0 && fr < 32) g_tr_labels[fr] = lab;
  if (on > g_tr_maxon) g_tr_maxon = on;
}

/* pictures of an alternating trace's frames (trace_pictures=K, docs/KIT.md): the back buffer of frames 8-2K .. 8+2K,
   read at the start of Present (the frame is complete) into a system-memory surface (CopyRects, as the game reads
   its own occlusion target), written as xikit_trace_<pid>_<n>_f<frame>_l<label>.pic. The device's own methods are
   called through its original vtable, so the trace does not record them. kit/d3dprof/pictures.py compares each
   swapped frame with its two neighbours, pixel by pixel. */
static int g_pic_failed; static void *g_pic_img; static uint32_t g_pic_w, g_pic_h, g_pic_fmt;
typedef HRESULT (__stdcall *GetBBFn)(void *, UINT, UINT, void **);
typedef HRESULT (__stdcall *CreateImgFn)(void *, UINT, UINT, UINT, void **);
typedef HRESULT (__stdcall *CopyRectsFn)(void *, void *, const RECT *, UINT, void *, const POINT *);
typedef HRESULT (__stdcall *SurfDescFn)(void *, uint32_t *);
typedef HRESULT (__stdcall *SurfLockFn)(void *, int32_t *, const RECT *, DWORD);
typedef HRESULT (__stdcall *SurfFn)(void *);
#define VT(o) (*(void ***)(o))
static void pic_capture(void *dev, int frame, int label) {
  void *bb = NULL; uint32_t d[8] = {0}; int32_t lr[2] = {0}; HRESULT hr;
  if ((hr = ((GetBBFn)g_orig_vt[16])(dev, 0, 0, &bb)) < 0 || !bb) { if (!g_pic_failed++) klog("trace picture: GetBackBuffer failed (0x%08lX)", (unsigned long)hr); return; }
  ((SurfDescFn)VT(bb)[8])(bb, d);                                 /* D3DSURFACE_DESC: Format, ..., Width [6], Height [7] */
  if (!g_pic_img || g_pic_w != d[6] || g_pic_h != d[7] || g_pic_fmt != d[0]) {
    if (g_pic_img) ((SurfFn)VT(g_pic_img)[2])(g_pic_img);
    g_pic_img = NULL;
    if ((hr = ((CreateImgFn)g_orig_vt[27])(dev, d[6], d[7], d[0], &g_pic_img)) < 0) { if (!g_pic_failed++) klog("trace picture: CreateImageSurface %ux%u format %u failed (0x%08lX)", d[6], d[7], d[0], (unsigned long)hr); g_pic_img = NULL; }
    g_pic_w = d[6]; g_pic_h = d[7]; g_pic_fmt = d[0];
  }
  if (g_pic_img && (hr = ((CopyRectsFn)g_orig_vt[28])(dev, bb, NULL, 0, g_pic_img, NULL)) >= 0 &&
      (hr = ((SurfLockFn)VT(g_pic_img)[9])(g_pic_img, lr, NULL, 0x10 /* D3DLOCK_READONLY */)) >= 0) {
    int bpp = (d[0] == 21 || d[0] == 22) ? 4 : (d[0] >= 23 && d[0] <= 26) ? 2 : 0;   /* A8R8G8B8, X8R8G8B8; R5G6B5, X1R5G5B5, A1R5G5B5, A4R4G4B4 */
    char path[MAX_PATH]; snprintf(path, sizeof path, "%s\\xikit_trace_%lu_%d_f%02d_l%d.pic", g_out, (unsigned long)GetCurrentProcessId(), g_tr_n + 1, frame, label);
    FILE *f = bpp ? fopen(path, "wb") : NULL;
    if (f) {
      uint32_t head[8] = { 0x43495058u /* "XPIC" */, 1, d[6], d[7], d[0], (uint32_t)bpp, (uint32_t)frame, (uint32_t)label };
      fwrite(head, 4, 8, f);
      for (uint32_t y = 0; y < d[7]; y++) fwrite((uint8_t *)(uintptr_t)lr[1] + (size_t)y * lr[0], 1, (size_t)d[6] * bpp, f);
      fclose(f);
    } else if (!bpp && !g_pic_failed++) klog("trace picture: back buffer format %u is not one pictures.py reads", d[0]);
    ((SurfFn)VT(g_pic_img)[10])(g_pic_img);
  } else if (!g_pic_failed++) klog("trace picture: CopyRects or LockRect failed (0x%08lX)", (unsigned long)hr);
  ((SurfFn)VT(bb)[2])(bb);
}

static int g_nmods; static void mods_frame(void *dev); static void mods_watch(void);     /* (mods: below) */
static int after_call(uint32_t r);
static Fn *fn_at_sym(uint32_t a, const char **sym);

/* ------------------------------------------------------------------------------------------------ timing (sample=1)
   Where the render thread's time goes, measured (docs/KIT.md):
     * per frame: the time from one Present to the next, the time inside the device's Present, and the kit's own
       time before it (its checks, swaps, mods' frame callbacks); xikit_timing_<pid>.csv, one line per window;
     * a sampling profiler: every sample_ms the render thread is suspended for an instant and its EIP read, and its
       stack scanned for return addresses into the game or the reconstruction (each one the address after a call
       instruction). xikit_samples_<pid>.csv, rewritten every window: per address where the thread was (self),
       per innermost game/reconstruction return address when it was in another module (Direct3D, the driver,
       Windows: the call site that went there), and per return address anywhere on the stack (inclusive).
   Samples are an estimate of time shares, not a measurement of each call. The stack scan finds return addresses
   still on the stack; one left in a dead slot of an older frame is counted too (inclusive only). */
#define SAMPN 65536
typedef struct { uint32_t k[SAMPN]; uint32_t n[SAMPN]; } Hist;
static Hist *g_h_self, *g_h_caller, *g_h_incl;
static CRITICAL_SECTION g_samplock; static int g_sampling, g_sample_ms = 1; static volatile LONG g_samples, g_sample_miss;
static LARGE_INTEGER g_qpf, g_t_prev; static double g_tm_frame[4096], g_tm_present[4096], g_tm_kit[4096]; static int g_tm_n;
static void hist_add(Hist *h, uint32_t key) {
  uint32_t i = (key * 2654435761u) & (SAMPN - 1);
  for (int k = 0; k < SAMPN; k++, i = (i + 1) & (SAMPN - 1)) {
    if (h->k[i] == key) { h->n[i]++; return; }
    if (!h->k[i]) { h->k[i] = key; h->n[i] = 1; return; }
  }
}
static int in_ours(uint32_t a) { return (a >= g_game_lo && a < g_game_hi) || (a >= g_recon_lo && a < g_recon_hi); }
/* the readable parts of the two images, found once (VirtualQuery): the sampler checks a return address against
   them instead of IsBadReadPtr, which works by raising an access violation that every vectored handler sees */
static uint32_t g_rd_lo[64], g_rd_hi[64]; static int g_nrd;
static void readable_ranges(uint32_t lo, uint32_t hi) {
  for (uint32_t a = lo; a < hi && g_nrd < 64;) {
    MEMORY_BASIC_INFORMATION mb;
    if (!VirtualQuery((void *)(uintptr_t)a, &mb, sizeof mb)) break;
    uint32_t e = (uint32_t)(uintptr_t)mb.BaseAddress + (uint32_t)mb.RegionSize;
    if (mb.State == MEM_COMMIT && !(mb.Protect & (PAGE_GUARD | PAGE_NOACCESS)) && (mb.Protect & 0xEE)) { g_rd_lo[g_nrd] = a; g_rd_hi[g_nrd] = e < hi ? e : hi; g_nrd++; }
    if (e <= a) break;
    a = e;
  }
}
static int readable7(uint32_t r) { for (int i = 0; i < g_nrd; i++) if (r - 7 >= g_rd_lo[i] && r <= g_rd_hi[i]) return 1; return 0; }
static int after_call_quiet(uint32_t r) {             /* after_call, for addresses inside the two images, never faulting */
  const uint8_t *p = (const uint8_t *)(uintptr_t)r;
  if (!readable7(r)) return 0;
  if (p[-5] == 0xE8) return 1;
  if (p[-2] == 0xFF && ((p[-1] >> 3) & 7) == 2 && (p[-1] >> 6) == 3) return 1;
  if (p[-2] == 0xFF && ((p[-1] >> 3) & 7) == 2 && (p[-1] >> 6) == 0 && (p[-1] & 7) != 4 && (p[-1] & 7) != 5) return 1;
  if (p[-3] == 0xFF && ((p[-2] >> 3) & 7) == 2 && (p[-2] >> 6) == 1) return 1;
  if (p[-6] == 0xFF && ((p[-5] >> 3) & 7) == 2 && ((p[-5] >> 6) == 2 || (p[-5] & 0xC7) == 5)) return 1;
  if (p[-4] == 0xFF && ((p[-3] >> 3) & 7) == 2 && (p[-3] >> 6) == 1 && (p[-3] & 7) == 4) return 1;
  if (p[-7] == 0xFF && ((p[-6] >> 3) & 7) == 2 && (p[-6] >> 6) == 2 && (p[-6] & 7) == 4) return 1;
  return 0;
}
static DWORD WINAPI sampler(LPVOID unused) {
  (void)unused;
  HANDLE th = NULL; DWORD tid = 0;
  timeBeginPeriod(1);
  for (;;) {
    Sleep(g_sample_ms);
    if (!g_render_thread) continue;
    if (tid != g_render_thread) { if (th) CloseHandle(th); tid = g_render_thread; th = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT, FALSE, tid); }
    if (!th) continue;
    static uint32_t ras[256]; int nra = 0; uint32_t eip = 0;
    EnterCriticalSection(&g_susplock);
    if (SuspendThread(th) != (DWORD)-1) {
      CONTEXT c; memset(&c, 0, sizeof c); c.ContextFlags = CONTEXT_CONTROL;
      if (GetThreadContext(th, &c)) {
        eip = c.Eip; MEMORY_BASIC_INFORMATION mb;
        if (VirtualQuery((void *)(uintptr_t)c.Esp, &mb, sizeof mb) && mb.State == MEM_COMMIT) {
          uint32_t *q = (uint32_t *)(uintptr_t)c.Esp, *top = (uint32_t *)((uint8_t *)mb.BaseAddress + mb.RegionSize);
          for (int k = 0; q < top && k < 16384 && nra < 256; q++, k++)
            if (in_ours(*q) && after_call_quiet(*q)) ras[nra++] = *q;
        }
      }
      ResumeThread(th);
    }
    LeaveCriticalSection(&g_susplock);
    if (!eip) { InterlockedIncrement(&g_sample_miss); continue; }
    EnterCriticalSection(&g_samplock);
    hist_add(g_h_self, eip);
    if (!in_ours(eip) && nra) hist_add(g_h_caller, ras[0]);
    for (int i = 0; i < nra; i++) {               /* each address once per sample (recursion, stale copies) */
      int dup = 0; for (int j = 0; j < i; j++) if (ras[j] == ras[i]) { dup = 1; break; }
      if (!dup) hist_add(g_h_incl, ras[i]);
    }
    g_samples++;
    LeaveCriticalSection(&g_samplock);
  }
  return 0;
}
/* an address as the analysis reads it: game (analysed address), recon (symbol+offset), or module!offset */
static void addr_name(uint32_t a, char *out, size_t n) {
  if (a >= g_game_lo && a < g_game_hi) { snprintf(out, n, "game,0x%08X", analysed_of(a)); return; }
  if (a >= g_recon_lo && a < g_recon_hi) {
    const char *sym = "?"; Fn *f = fn_at_sym(a, &sym);
    snprintf(out, n, "recon,0x%08X %s%s%s", a - g_recon_lo, sym, f ? " " : "", f ? f->name : ""); return;
  }
  HMODULE m = NULL; char mn[MAX_PATH] = "?";
  if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)(uintptr_t)a, &m) && m) {
    GetModuleFileNameA(m, mn, sizeof mn); const char *b = strrchr(mn, '\\');
    snprintf(out, n, "module,%s+0x%lX", b ? b + 1 : mn, (unsigned long)(a - (uint32_t)(uintptr_t)m));
  } else snprintf(out, n, "memory,0x%08X", a);
}
static void where(uint32_t a, char *out, size_t n) { addr_name(a, out, n); for (char *c = out; *c; c++) if (*c == ',') *c = ':'; }   /* one CSV field */
static int cmp_double(const void *a, const void *b) { double x = *(const double *)a, y = *(const double *)b; return x < y ? -1 : x > y; }
static void report_timing(void) {
  char path[MAX_PATH]; int n = g_tm_n; g_tm_n = 0;
  if (n > 1) {
    static double sorted[4096]; double sf = 0, sp = 0, sk = 0, mf = 0, mp = 0;
    for (int i = 1; i < n; i++) { sorted[i - 1] = g_tm_frame[i]; sf += g_tm_frame[i]; if (g_tm_frame[i] > mf) mf = g_tm_frame[i]; }
    for (int i = 0; i < n; i++) { sp += g_tm_present[i]; sk += g_tm_kit[i]; if (g_tm_present[i] > mp) mp = g_tm_present[i]; }
    qsort(sorted, n - 1, sizeof sorted[0], cmp_double);
    snprintf(path, sizeof path, "%s\\xikit_timing_%lu.csv", g_out, (unsigned long)GetCurrentProcessId());
    int fresh = GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES;
    FILE *f = fopen(path, "a");
    if (f) {
      if (fresh) fprintf(f, "frame,frames,frame_ms_avg,frame_ms_p50,frame_ms_p95,frame_ms_max,present_ms_avg,present_ms_max,kit_ms_avg,swapped\n");
      int sw = 0; for (int i = 0; i < g_nfn; i++) sw += g_fn[i].patched && !g_fn[i].mod_target;
      fprintf(f, "%u,%d,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%d\n", g_frame, n - 1, sf / (n - 1), sorted[(n - 1) / 2], sorted[(int)((n - 1) * 0.95)], mf,
              sp / n, mp, sk / n, sw);
      fclose(f);
    }
  }
  if (!g_sampling) return;
  snprintf(path, sizeof path, "%s\\xikit_samples_%lu.csv", g_out, (unsigned long)GetCurrentProcessId());
  FILE *f = fopen(path, "w"); if (!f) return;
  EnterCriticalSection(&g_samplock);
  fprintf(f, "# xikit samples: %ld samples every %d ms of the render thread (%ld missed), over the last %d frames; FFXiMain.dll at +0x%X; reconstruction at 0x%08X\n"
             "# kind,where,address[ symbol],samples: self = where the thread was; caller = the innermost game/reconstruction return address when it was elsewhere;"
             " incl = a return address anywhere on the stack\n", (long)g_samples, g_sample_ms, (long)g_sample_miss, g_cfg.report_every, g_rel, g_recon_lo);
  const char *kinds[3] = {"self", "caller", "incl"}; Hist *hs[3] = {g_h_self, g_h_caller, g_h_incl};
  for (int t = 0; t < 3; t++)
    for (int i = 0; i < SAMPN; i++) if (hs[t]->k[i]) { char nm[300]; addr_name(hs[t]->k[i], nm, sizeof nm); fprintf(f, "%s,%s,%u\n", kinds[t], nm, hs[t]->n[i]); }
  for (int t = 0; t < 3; t++) memset(hs[t], 0, sizeof *hs[t]);
  g_samples = 0; g_sample_miss = 0;
  LeaveCriticalSection(&g_samplock);
  fclose(f);
}
static void start_sampler(void) {
  g_sampling = GetPrivateProfileIntA("xikit", "sample", 0, g_ini);
  g_sample_ms = GetPrivateProfileIntA("xikit", "sample_ms", 1, g_ini); if (g_sample_ms < 1) g_sample_ms = 1;
  QueryPerformanceFrequency(&g_qpf);
  if (!g_sampling) return;
  g_h_self = (Hist *)VirtualAlloc(NULL, 3 * sizeof(Hist), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  if (!g_h_self) { g_sampling = 0; klog("sample: could not reserve the tables"); return; }
  g_h_caller = g_h_self + 1; g_h_incl = g_h_self + 2;
  readable_ranges(g_game_lo, g_game_hi); readable_ranges(g_recon_lo, g_recon_hi);
  HANDLE h = CreateThread(NULL, 0, sampler, NULL, 0, NULL);
  if (h) { SetThreadPriority(h, THREAD_PRIORITY_TIME_CRITICAL); CloseHandle(h); }
  klog("sample: the render thread every %d ms (xikit_samples_<pid>.csv every %d frames)", g_sample_ms, g_cfg.report_every);
}
typedef HRESULT (__stdcall *PresentFn)(void *, const RECT *, const RECT *, HWND, const void *);

/* ------------------------------------------------------------------------------------------------ ab=1 (docs/KIT.md)
   An A/B test with no keys. ab_after seconds after it is switched on, phases of ab_phase seconds in the order
   A B B A, ab_rounds times: A = everything the kit adds off (g_master_off, as F9: no swaps, no mod hooks), B = on.
   ABBA puts each label's phases around the same mean time, so a scene that drifts steadily affects both alike. The
   first ab_settle frames of a phase are left out (the switch itself, and what it takes to settle), and so are frames
   while a trace runs. Per phase: frames used, mean, median, p95 and max frame time; at the end the two labels
   compared, with how much the phases of one label differ among themselves (the noise the difference must beat). */
#define AB_MAX 16384
static int g_ab_state;                  /* 0 idle, 1 waiting ab_after, 2 running, 3 done (ab=0 resets) */
static DWORD g_ab_t0; static int g_ab_i, g_ab_nph, g_ab_seen, g_ab_n, g_ab_after, g_ab_phase, g_ab_settle;
static double g_ab_ft[AB_MAX], g_ab_mean[32], g_ab_med[32]; static int g_ab_cnt[32];
static char g_ab_path[MAX_PATH];
static char ab_label(int i) { return ((i + 1) / 2) % 2 ? 'B' : 'A'; }
static void ab_set(int i) {
  g_ab_i = i; g_ab_t0 = GetTickCount(); g_ab_seen = 0; g_ab_n = 0;
  int off = ab_label(i) == 'A';
  if (g_master_off != off) { g_master_off = off; g_dirty = 1; }
  klog("ab: phase %d of %d: %c (%s)", i + 1, g_ab_nph, ab_label(i), off ? "everything the kit adds off" : "on");
}
static void ab_end_phase(void) {
  int i = g_ab_i, n = g_ab_n; double sum = 0, mx = 0;
  for (int k = 0; k < n; k++) { sum += g_ab_ft[k]; if (g_ab_ft[k] > mx) mx = g_ab_ft[k]; }
  qsort(g_ab_ft, n, sizeof g_ab_ft[0], cmp_double);
  g_ab_cnt[i] = n; g_ab_mean[i] = n ? sum / n : 0; g_ab_med[i] = n ? g_ab_ft[n / 2] : 0;
  FILE *f = fopen(g_ab_path, "a");
  if (f) { fprintf(f, "%d,%c,%d,%.3f,%.3f,%.3f,%.3f\n", i + 1, ab_label(i), n, g_ab_mean[i], g_ab_med[i], n ? g_ab_ft[(int)(n * 0.95)] : 0, mx); fclose(f); }
  klog("ab: phase %d (%c): %d frames, mean %.2f ms, median %.2f ms", i + 1, ab_label(i), n, g_ab_mean[i], g_ab_med[i]);
}
static void ab_finish(int stopped) {
  if (g_master_off) { g_master_off = 0; g_dirty = 1; }
  if (stopped) { klog("ab: stopped before the end (ab=0); everything back on"); return; }
  double s[2] = {0, 0}, m[2] = {0, 0}, lo[2] = {1e9, 1e9}, hi[2] = {0, 0}; int c[2] = {0, 0};
  for (int i = 0; i < g_ab_nph; i++) {
    if (!g_ab_cnt[i]) continue;
    int b = ab_label(i) == 'B'; s[b] += g_ab_mean[i]; m[b] += g_ab_med[i]; c[b]++;
    if (g_ab_mean[i] < lo[b]) lo[b] = g_ab_mean[i];
    if (g_ab_mean[i] > hi[b]) hi[b] = g_ab_mean[i];
  }
  if (!c[0] || !c[1]) { klog("ab: done, but a label has no frames: nothing to compare"); return; }
  double a = s[0] / c[0], b = s[1] / c[1], am = m[0] / c[0], bm = m[1] / c[1];
  double noise = hi[0] - lo[0] > hi[1] - lo[1] ? hi[0] - lo[0] : hi[1] - lo[1];
  FILE *f = fopen(g_ab_path, "a");
  if (f) {
    fprintf(f, "# A (off): mean of phase means %.3f ms, of medians %.3f; B (on): %.3f, %.3f; B - A: %+.3f ms (%+.1f%%), medians %+.3f ms\n",
            a, am, b, bm, b - a, a > 0 ? 100.0 * (b - a) / a : 0.0, bm - am);
    fprintf(f, "# phases of one label differ among themselves by up to %.3f ms: a B - A smaller than that is within the noise\n", noise);
    fclose(f);
  }
  klog("ab: done. A (off) %.2f ms, B (on) %.2f ms: B - A %+.2f ms (%+.1f%%), medians %+.2f ms; phases of one label differ by up to %.2f ms%s",
       a, b, b - a, a > 0 ? 100.0 * (b - a) / a : 0.0, bm - am, noise, fabs(b - a) <= noise ? " (the difference is within that)" : "");
}
static void ab_control(void) {          /* each frame on the render thread, before apply() */
  if (!g_cfg.ab) { if (g_ab_state == 1 || g_ab_state == 2) ab_finish(1); g_ab_state = 0; return; }
  if (g_ab_state == 0) {
    g_ab_state = 1; g_ab_t0 = GetTickCount();
    g_ab_after = g_cfg.ab_after; g_ab_phase = g_cfg.ab_phase; g_ab_settle = g_cfg.ab_settle; g_ab_nph = 2 * g_cfg.ab_rounds;
    for (int i = 0; i < 32; i++) g_ab_cnt[i] = 0;
    klog("ab: starts in %d s: %d phases of %d s (A B B A ...), the first %d frames of each left out; stand still", g_ab_after, g_ab_nph, g_ab_phase, g_ab_settle);
  }
  if (g_ab_state == 1 && GetTickCount() - g_ab_t0 >= (DWORD)g_ab_after * 1000) {
    static int run; run++;
    snprintf(g_ab_path, sizeof g_ab_path, "%s\\xikit_ab_%lu_%d.csv", g_out, (unsigned long)GetCurrentProcessId(), run);
    FILE *f = fopen(g_ab_path, "w");
    if (f) {
      fprintf(f, "# A/B test %d: A = everything the kit adds off (no swaps, no mod hooks), B = on (swap=%d, mods as loaded); phases of %d s, "
              "the first %d frames of each left out; frame time = Present to Present\nphase,label,frames,mean_ms,p50_ms,p95_ms,max_ms\n",
              run, g_cfg.swap, g_ab_phase, g_ab_settle);
      fclose(f);
    }
    g_ab_state = 2; ab_set(0);
  }
  if (g_ab_state == 2 && GetTickCount() - g_ab_t0 >= (DWORD)g_ab_phase * 1000) {
    ab_end_phase();
    if (g_ab_i + 1 < g_ab_nph) ab_set(g_ab_i + 1);
    else { ab_finish(0); g_ab_state = 3; klog("ab: set ab=0 and back to 1 for another run; results in %s", g_ab_path); }
  }
}
static void ab_frame(double ms) {       /* each frame's time, after Present */
  if (g_ab_state != 2 || g_dirty || g_tr_state || ms <= 0) return;
  if (g_ab_seen++ >= g_ab_settle && g_ab_n < AB_MAX) g_ab_ft[g_ab_n++] = ms;
}

static HRESULT __stdcall kit_present(void *dev, const RECT *a, const RECT *b, HWND w, const void *r) {
  if (!g_render_thread) g_render_thread = GetCurrentThreadId();
  LARGE_INTEGER t0, t1, t2; int mine = GetCurrentThreadId() == g_render_thread;
  QueryPerformanceCounter(&t0);
  if (mine) {
    g_frame++;
    if (g_tr_state == 5 && g_tr_pictures > 0) {     /* the frame that just ended (before the trace moves on) */
      int fr = 0; d3dprof_trace_frames(&fr); fr -= 1;
      if (fr >= 0 && fr < 32 && fr >= TR_ALT_FRAMES / 2 - 2 * g_tr_pictures && fr <= TR_ALT_FRAMES / 2 + 2 * g_tr_pictures) pic_capture(dev, fr, g_tr_labels[fr]);
    }
    if (g_cfg.hotkey) {
      int down = (GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState(VK_MENU) & 0x8000) && (GetAsyncKeyState(VK_F9) & 0x8000);
      if (down && !g_hotkey_down) { g_master_off = !g_master_off; g_dirty = 1; klog("hotkey: swaps %s", g_master_off ? "off" : "on"); }
      g_hotkey_down = down;
    }
    if (g_cfg.bisect) {
      static int k10, k11, k12;
      int mod = (GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState(VK_MENU) & 0x8000);
      int d10 = mod && (GetAsyncKeyState(VK_F10) & 0x8000), d11 = mod && (GetAsyncKeyState(VK_F11) & 0x8000), d12 = mod && (GetAsyncKeyState(VK_F12) & 0x8000);
      if (d10 && !k10) bis_answer(1);
      if (d11 && !k11) bis_answer(0);
      if (d12 && !k12) bis_stop();
      k10 = d10; k11 = d11; k12 = d12;
    }
    if (g_cfg.bisect && g_frame % 30 == 0) {      /* the same answers as files: create xikit_bisect_bad.txt / _good.txt / _stop.txt next to xikit.dll */
      static const char *const nm[3] = {"xikit_bisect_bad.txt", "xikit_bisect_good.txt", "xikit_bisect_stop.txt"};
      for (int i = 0; i < 3; i++) {
        char p[MAX_PATH]; snprintf(p, sizeof p, "%s\\%s", g_dir, nm[i]);
        if (DeleteFileA(p)) { klog("bisect answer from file %s", nm[i]); if (i == 0) bis_answer(1); else if (i == 1) bis_answer(0); else bis_stop(); }
      }
    }
    if (g_trace_attached) trace_step();
    if (g_frame % 60 == 0 && ini_changed()) { read_cfg(); g_dirty = 1; klog("xikit.ini changed: re-read"); }
    if (g_nmods) mods_watch();
    ab_control();
    if (g_dirty) apply();
    if (g_trace_attached) trace_label();
    if (g_cfg.count_calls && g_frame % (uint32_t)g_cfg.report_every == 0) report_calls();
    if (g_nmods) mods_frame(dev);
  }
  QueryPerformanceCounter(&t1);
  HRESULT hr = ((PresentFn)g_orig_vt[SLOT_PRESENT])(dev, a, b, w, r);
  if (mine && g_qpf.QuadPart) {
    QueryPerformanceCounter(&t2); double ms = 1000.0 / (double)g_qpf.QuadPart;
    if (g_tm_n < 4096) {
      g_tm_frame[g_tm_n] = g_t_prev.QuadPart ? (double)(t0.QuadPart - g_t_prev.QuadPart) * ms : 0;
      g_tm_present[g_tm_n] = (double)(t2.QuadPart - t1.QuadPart) * ms; g_tm_kit[g_tm_n] = (double)(t1.QuadPart - t0.QuadPart) * ms; g_tm_n++;
    }
    if (g_t_prev.QuadPart) {
      double fm = (double)(t0.QuadPart - g_t_prev.QuadPart) * ms;
      ab_frame(fm);
      if (g_cfg.spike_ms > 0 && fm > g_cfg.spike_ms) {           /* a stutter: which frame, how long, what the kit was doing */
        char sp[MAX_PATH]; snprintf(sp, sizeof sp, "%s\\xikit_spikes_%lu.csv", g_out, (unsigned long)GetCurrentProcessId());
        int fresh = GetFileAttributesA(sp) == INVALID_FILE_ATTRIBUTES;
        FILE *f = fopen(sp, "a");
        if (f) {
          if (fresh) fprintf(f, "# frames slower than spike_ms; frame = the kit's frame number (d3dprof's resource log counts the same Presents)\nframe,ms,present_ms_before,swaps_off\n");
          fprintf(f, "%u,%.1f,%.2f,%d\n", g_frame, fm, g_tm_n > 1 ? g_tm_present[g_tm_n - 2] : 0.0, g_master_off);
          fclose(f);
        }
      }
    }
    g_t_prev = t0;
    if (g_frame % (uint32_t)g_cfg.report_every == 0) { report_timing(); report_facts(); }
  }
  return hr;
}

/* ------------------------------------------------------------------------------------------------ crashes */
static int cmp_target(const void *a, const void *b) {
  uintptr_t x = (uintptr_t)(*(Fn *const *)a)->target, y = (uintptr_t)(*(Fn *const *)b)->target;
  return x < y ? -1 : x > y;
}

/* the manifest function whose reconstruction holds address a: the code symbol at or below a (the manifest's !code
   list, sorted, from the DLL's own symbol table). NULL when a is not in the reconstruction, or is in code that is no
   manifest function's (the runtime helpers, a function left out of the manifest): nothing is quarantined for it.
   *sym (if given) receives the symbol's name. A manifest without !code lines (older) falls back to the nearest
   reconstruction entry at or below a. */
static Fn *fn_at_sym(uint32_t a, const char **sym) {
  if (sym) *sym = "?";
  if (a < g_recon_lo || a >= g_recon_hi) return NULL;
  if (g_ncode) {
    uint32_t r = a - g_recon_lo; int lo = 0, hi = g_ncode - 1, k = -1;
    while (lo <= hi) { int m = (lo + hi) / 2; if (g_code[m].rva <= r) { k = m; lo = m + 1; } else hi = m - 1; }
    if (k < 0) return NULL;
    if (sym) *sym = g_code[k].sym;
    return g_code[k].fn >= 0 ? &g_fn[g_code[k].fn] : NULL;
  }
  Fn *f = NULL; int lo = 0, hi = g_nfn - 1;
  while (lo <= hi) { int m = (lo + hi) / 2; if ((uint32_t)(uintptr_t)g_by_target[m]->target <= a) { f = g_by_target[m]; lo = m + 1; } else hi = m - 1; }
  return f;
}
static Fn *fn_at(uint32_t a) { return fn_at_sym(a, NULL); }

static int after_call(uint32_t r) {          /* r follows a call instruction (rel32, or FF /2 through a register or memory) */
  const uint8_t *p = (const uint8_t *)(uintptr_t)r;
  if (IsBadReadPtr(p - 7, 7)) return 0;
  if (p[-5] == 0xE8) return 1;
  if (p[-2] == 0xFF && ((p[-1] >> 3) & 7) == 2 && (p[-1] >> 6) == 3) return 1;              /* call reg */
  if (p[-2] == 0xFF && ((p[-1] >> 3) & 7) == 2 && (p[-1] >> 6) == 0 && (p[-1] & 7) != 4 && (p[-1] & 7) != 5) return 1;
  if (p[-3] == 0xFF && ((p[-2] >> 3) & 7) == 2 && (p[-2] >> 6) == 1) return 1;              /* call [reg+d8] */
  if (p[-6] == 0xFF && ((p[-5] >> 3) & 7) == 2 && ((p[-5] >> 6) == 2 || (p[-5] & 0xC7) == 5)) return 1;   /* [reg+d32], [abs] */
  if (p[-4] == 0xFF && ((p[-3] >> 3) & 7) == 2 && (p[-3] >> 6) == 1 && (p[-3] & 7) == 4) return 1;     /* [sib+d8] */
  if (p[-7] == 0xFF && ((p[-6] >> 3) & 7) == 2 && (p[-6] >> 6) == 2 && (p[-6] & 7) == 4) return 1;     /* [sib+d32] */
  return 0;
}

/* an exception outside reconstructed code (in the game or another module). If a reconstruction called the code
   that faulted, its return address is on this thread's stack: the innermost such caller is quarantined as
   suspected (the original may have been given what the reconstruction passed it -- a wrong ECX crashed
   CXiTimer::GetFps this way). Only faults, not the exceptions programs raise on purpose. */
static LONG caller_fault(EXCEPTION_POINTERS *e) {
  DWORD code = e->ExceptionRecord->ExceptionCode;
  if (code != EXCEPTION_ACCESS_VIOLATION && code != EXCEPTION_ILLEGAL_INSTRUCTION && code != EXCEPTION_PRIV_INSTRUCTION
      && code != EXCEPTION_INT_DIVIDE_BY_ZERO && code != EXCEPTION_STACK_OVERFLOW && code != EXCEPTION_ARRAY_BOUNDS_EXCEEDED)
    return EXCEPTION_CONTINUE_SEARCH;
  CONTEXT *c = e->ContextRecord;
  uint32_t *sp = (uint32_t *)(uintptr_t)c->Esp, *top = sp;
  MEMORY_BASIC_INFORMATION mb;                   /* the committed stack above ESP (no fs: access, which GCC treats as UB) */
  if (VirtualQuery(sp, &mb, sizeof mb)) top = (uint32_t *)((uint8_t *)mb.BaseAddress + mb.RegionSize);
  Fn *chain[4]; uint32_t at[4]; int n = 0;
  for (uint32_t *q = sp; q < top && q < sp + 16384 && n < 4; q++) {
    if (IsBadReadPtr(q, 4)) break;
    Fn *f = fn_at(*q);
    if (f && after_call(*q)) { chain[n] = f; at[n] = *q; n++; }
  }
  if (!n) return EXCEPTION_CONTINUE_SEARCH;
  uint32_t eip = c->Eip; HMODULE mod = NULL; char mname[MAX_PATH] = "?";
  if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)(uintptr_t)eip, &mod))
    GetModuleFileNameA(mod, mname, sizeof mname);
  const char *base = strrchr(mname, '\\'); base = base ? base + 1 : mname;
  uint32_t game_ea = (eip >= g_game_lo && eip < g_game_hi) ? analysed_of(eip) : 0;
  klog("fault module base %p, offset 0x%lX (look it up with nm -n on the DLL: offset + its ImageBase)", (void *)mod, (unsigned long)(eip - (uint32_t)(uintptr_t)mod));
  klog("EXCEPTION 0x%08lX at 0x%08lX in %s (analysed 0x%08X), called from reconstructed %s (original 0x%08X, return 0x%08X)%s%s%s%s: "
       "eax=%08lX ebx=%08lX ecx=%08lX edx=%08lX esi=%08lX edi=%08lX ebp=%08lX esp=%08lX",
       code, (unsigned long)eip, base, game_ea, chain[0]->name, chain[0]->rva + ANALYSED_BASE, at[0],
       n > 1 ? " <- " : "", n > 1 ? chain[1]->name : "", n > 2 ? " <- " : "", n > 2 ? chain[2]->name : "",
       c->Eax, c->Ebx, c->Ecx, c->Edx, c->Esi, c->Edi, c->Ebp, c->Esp);
  Fn *f = chain[0];
  if (!f->quarantined) {
    f->quarantined = 1; g_dirty = 1;
    FILE *q = fopen(g_quar, "a");
    if (q) { fprintf(q, "0x%08X %s suspected: exception 0x%08lX in %s (analysed 0x%08X) it called\n", f->rva + ANALYSED_BASE, f->name, code, base, game_ea); fclose(q); }
    klog("quarantined %s (suspected: the fault is in code it called): restored at the next frame if the game survives, left out from the next start", f->name);
  }
  return EXCEPTION_CONTINUE_SEARCH;
}

static LONG CALLBACK on_exception(EXCEPTION_POINTERS *e) {
  DWORD code = e->ExceptionRecord->ExceptionCode;
  if (code == EXCEPTION_BREAKPOINT || code == EXCEPTION_SINGLE_STEP || code == 0xE06D7363u || code == 0x406D1388u
      || code == DBG_PRINTEXCEPTION_C) return EXCEPTION_CONTINUE_SEARCH;
  uint32_t eip = e->ContextRecord->Eip;
  Fn *f = NULL; const char *sym = "?";
  if (eip >= g_recon_lo && eip < g_recon_hi) {
    f = fn_at_sym(eip, &sym);
  } else if (g_stubs && eip >= (uint32_t)(uintptr_t)g_stubs && eip < (uint32_t)(uintptr_t)g_stubs + MAXFN * STUB_LEN) {
    f = &g_fn[(eip - (uint32_t)(uintptr_t)g_stubs) / STUB_LEN];
  } else return caller_fault(e);
  CONTEXT *c = e->ContextRecord; uint32_t *sp = (uint32_t *)(uintptr_t)c->Esp;
  klog("EXCEPTION 0x%08lX at 0x%08lX in reconstructed %s (original 0x%08X, +0x%lX into the reconstruction; symbol %s): "
       "eax=%08lX ebx=%08lX ecx=%08lX edx=%08lX esi=%08lX edi=%08lX ebp=%08lX esp=%08lX; [esp]=%08X %08X %08X %08X",
       code, (unsigned long)eip, f ? f->name : "?", f ? f->rva + ANALYSED_BASE : 0,
       f ? (unsigned long)(eip - (uint32_t)(uintptr_t)f->target) : 0ul, sym, c->Eax, c->Ebx, c->Ecx, c->Edx, c->Esi, c->Edi, c->Ebp, c->Esp,
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

/* ------------------------------------------------------------------------------------------------ mods (sdk/, docs/SDK.md) */
#define MAXMODS 16
typedef struct {
  char name[64], path[MAX_PATH];     /* as listed in mods=, and the file in the kit's folder */
  HMODULE h; xikit_mod_frame_fn frame; xikit_mod_unload_fn unload;
  FILETIME ft, seen; int gen, seen_change;   /* mod_reload: the file's time when loaded, the build count, a change seen (and its time) */
} Mod;
static Mod g_mods[MAXMODS]; static int g_mod_cur = -1, g_mods_open, g_mod_reload;
static uint8_t *g_tramps;                       /* MAXFN slots of 32 bytes: copied entry instructions + jmp back */
static XikitHost g_host;

static void __cdecl host_log(const char *fmt, ...) {
  char buf[900]; va_list a; va_start(a, fmt); vsnprintf(buf, sizeof buf, fmt, a); va_end(a);
  klog("mod %s: %s", g_mod_cur >= 0 ? g_mods[g_mod_cur].name : "?", buf);
}

static Fn *fn_named(const char *what) {
  for (int i = 0; i < g_nfn; i++) {
    Fn *f = &g_fn[i];
    if (!strcmp(what, f->name) || !strcmp(what, f->exp)) return f;
  }
  if (what[0] == '0' && (what[1] == 'x' || what[1] == 'X')) {
    uint32_t v = (uint32_t)strtoul(what, NULL, 16);
    for (int i = 0; i < g_nfn; i++) if (v == g_fn[i].rva + ANALYSED_BASE || v == g_fn[i].rva) return &g_fn[i];
  }
  return NULL;
}

/* the game's own code of f, reached from outside: its first tlen bytes (whole instructions, manifest T=) copied
   from the live entry, rel32 call/jmp re-aimed, then a jmp to entry + tlen. Built before the kit writes anything
   at the entry (mods are initialised before the first frame); the live bytes are checked against the analysed
   ones first (check_bytes: entry-4 .. entry+16, which covers tlen <= 16). */
static void *build_tramp(Fn *f) {
  if (f->tramp) return f->tramp;
  if (!f->tlen || !g_tramps || f->patched) return NULL;
  uint8_t *t = g_tramps + (f - g_fn) * 32;
  memcpy(t, f->site, f->tlen);
  for (int k = 0; k < f->nfix; k++) {
    int32_t d; memcpy(&d, t + f->tfix[k], 4);
    d += (int32_t)((uintptr_t)f->site - (uintptr_t)t);       /* same target, now from the copy */
    memcpy(t + f->tfix[k], &d, 4);
  }
  make_jmp(t + f->tlen, t + f->tlen, f->site + f->tlen);
  FlushInstructionCache(GetCurrentProcess(), t, 32);
  return f->tramp = t;
}

static int __cdecl host_hook(const char *what, void *repl, void **prev) {
  if (prev) *prev = NULL;
  if (!g_mods_open) { host_log("hook(%s): only inside xikit_mod_init", what ? what : "?"); return XIKIT_E_LATE; }
  Fn *f = what ? fn_named(what) : NULL;
  if (!f || !repl) { host_log("hook(%s): not a function of the manifest", what ? what : "NULL"); return XIKIT_E_UNKNOWN; }
  if (f->mod_target) { host_log("hook(%s): already hooked by %s", f->name, g_mods[f->mod - 1].name); return XIKIT_E_TAKEN; }
  int d = f->patched ? -1 : check_bytes(f);       /* (patched: a reload; the bytes were checked before the kit wrote its jmp) */
  if (d != -1) { host_log("hook(%s): the live entry differs from the analysed build at %+d", f->name, d); return XIKIT_E_REFUSED; }
  if ((f->flags & 2) && g_cfg.respect_speedfix && g_speedfix_loaded) { host_log("hook(%s): speedfix (speedfix_hook or the speedfix mod) patches inside it (respect_speedfix=1)", f->name); return XIKIT_E_REFUSED; }
  if (f->flags & 8) { host_log("hook(%s): the game's code reads a register at its entry that its declaration does not carry (manifest flag 8)", f->name); return XIKIT_E_REFUSED; }
  void *p = build_tramp(f); const char *via = "the game's code (trampoline)";
  if (!p) { p = f->target; via = "the reconstruction (no trampoline for this entry)"; }
  if (!p) { host_log("hook(%s): no way to reach its previous code", f->name); return XIKIT_E_NOPREV; }
  if (p == f->target && f->quarantined) { host_log("hook(%s): its previous would be the reconstruction, which is quarantined (xikit_quarantine.txt)", f->name); return XIKIT_E_REFUSED; }
  if (prev) *prev = p;
  f->mod_target = repl; f->mod = g_mod_cur + 1; f->mod_on_recon = p == f->target; f->refused = 0; g_dirty = 1;
  host_log("hooked %s (0x%08X)%s; previous = %s%s", f->name, f->rva + ANALYSED_BASE, f->flags & 4 ? " [gate coverage incomplete]" : "",
           via, (f->flags & 4) && p == f->target ? " [its gate coverage is incomplete]" : "");
  return XIKIT_OK;
}

static void *__cdecl host_recon(const char *what) { Fn *f = what ? fn_named(what) : NULL; return f ? f->target : NULL; }

/* ---- device hooks (device_hook). The device object gets one more vtable copy, in front of the others (the
   profiler's, the kit's): g_fvt. A method no mod hooks keeps the entry of the layer below (g_below), so it costs
   nothing; a hooked one gets a front thunk (generated code, below) that calls kit_front, which picks the hook to run.
   Each hook is an entry of g_dh: its replacement, and `next`, what its previous goes on to (the hook below it on the
   same method, or g_below). The previous a mod gets is the entry's stub, `jmp [&next]`, so taking a hook out (a
   reload) re-links the one above it without the mod's pointer changing. Hooks follow the master switch like the
   function hooks: F9 and a trace's unswapped frames send every call straight to g_below. */
#define MAXDH 256
typedef struct { void *fn; void *volatile next; int slot, mod, below, live; } DevHook;
static DevHook g_dh[MAXDH]; static int g_ndh, g_dh_top[NSLOTS];
static void *g_fvt[NSLOTS], *g_below[NSLOTS];
static uint8_t *g_dcode;                /* generated: MAXDH stubs of 8 bytes, NSLOTS thunks of 24, the return pad */
#define DH_STUB(i) (g_dcode + 8 * (i))
#define DH_THUNK(k) (g_dcode + 8 * MAXDH + 24 * (k))
#define DH_PAD (g_dcode + 8 * MAXDH + 24 * NSLOTS)
static int g_front_on;                  /* g_fvt is on the device */

/* the front thunk's choice: the top hook of the method, or (switch off, or none) the layer below. On the render
   thread the intercepted call's return address is kept on d3dprof's stack and the call made to return through the
   pad, which takes it off again (the profiler names calls made inside the hook by it: d3dprof.c) */
static void *__cdecl kit_front(int slot, uint32_t *a) {
  int i = g_dh_top[slot];
  if (i < 0 || g_master_off) return g_below[slot];
  if (GetCurrentThreadId() == g_render_thread && d3dprof_via_depth < 64) {
    d3dprof_via_stk[d3dprof_via_depth] = a[0]; d3dprof_via_depth++;
    a[0] = (uint32_t)(uintptr_t)DH_PAD;
  }
  return g_dh[i].fn;
}

static int make_dcode(void) {
  if (g_dcode) return 1;
  g_dcode = (uint8_t *)VirtualAlloc(NULL, 8 * MAXDH + 24 * NSLOTS + 32, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
  if (!g_dcode) return 0;
  for (int i = 0; i < MAXDH; i++) {          /* jmp dword [&g_dh[i].next] */
    uint8_t *c = DH_STUB(i); uint32_t at = (uint32_t)(uintptr_t)&g_dh[i].next;
    c[0] = 0xFF; c[1] = 0x25; memcpy(c + 2, &at, 4); c[6] = c[7] = 0xCC;
  }
  for (int k = 0; k < NSLOTS; k++) {         /* mov eax, k; mov ecx, esp; push ecx; push eax; call kit_front; add esp, 8; jmp eax */
    uint8_t *c = DH_THUNK(k); uint32_t v = (uint32_t)k;
    c[0] = 0xB8; memcpy(c + 1, &v, 4); c[5] = 0x89; c[6] = 0xE1; c[7] = 0x51; c[8] = 0x50;
    c[9] = 0xE8; int32_t d = (int32_t)((uintptr_t)kit_front - (uintptr_t)(c + 14)); memcpy(c + 10, &d, 4);
    c[14] = 0x83; c[15] = 0xC4; c[16] = 0x08; c[17] = 0xFF; c[18] = 0xE0;
    for (int j = 19; j < 24; j++) c[j] = 0xCC;
  }
  {                                          /* the pad: mov ecx, [depth]; dec ecx; mov [depth], ecx; push [stk + ecx*4]; ret */
    uint8_t *c = DH_PAD; uint32_t dp = (uint32_t)(uintptr_t)&d3dprof_via_depth, sp = (uint32_t)(uintptr_t)d3dprof_via_stk;
    c[0] = 0x8B; c[1] = 0x0D; memcpy(c + 2, &dp, 4); c[6] = 0x49; c[7] = 0x89; c[8] = 0x0D; memcpy(c + 9, &dp, 4);
    c[13] = 0xFF; c[14] = 0x34; c[15] = 0x8D; memcpy(c + 16, &sp, 4); c[20] = 0xC3;
  }
  FlushInstructionCache(GetCurrentProcess(), g_dcode, 8 * MAXDH + 24 * NSLOTS + 32);
  for (int k = 0; k < NSLOTS; k++) g_dh_top[k] = -1;
  return 1;
}

static int __cdecl host_device_hook(const char *method, int slot, void *repl, void **prev) {
  if (prev) *prev = NULL;
  if (!g_mods_open) { host_log("device_hook(%s): only inside xikit_mod_init", method ? method : "?"); return XIKIT_E_LATE; }
  const char *known = d3dprof_method(slot);
  if (!method || !repl || !known || strcmp(known, method)) {
    host_log("device_hook(%s, slot %d): not that IDirect3DDevice8 method in the kit's table (slot %d is %s)", method ? method : "NULL",
             slot, slot, known ? known : "out of range");
    return XIKIT_E_UNKNOWN;
  }
  if (!make_dcode() || g_ndh >= MAXDH) { host_log("device_hook(%s): no room for another device hook", method); return XIKIT_E_REFUSED; }
  int i = g_ndh++; DevHook *e = &g_dh[i];
  e->fn = repl; e->slot = slot; e->mod = g_mod_cur + 1; e->below = g_dh_top[slot]; e->live = 1;
  e->next = e->below >= 0 ? g_dh[e->below].fn : g_below[slot];   /* (g_below is filled when the front goes on; set again then) */
  g_dh_top[slot] = i;
  if (g_front_on) g_fvt[slot] = DH_THUNK(slot);
  if (prev) *prev = DH_STUB(i);
  host_log("device hook %s (slot %d)%s", method, slot, e->below >= 0 ? "; another mod's hook of it runs after this one" : "");
  return XIKIT_OK;
}

/* every hook of mod m out of its chain (a reload); the entries stay allocated: a thread may still be in a stub */
static void dh_remove_mod(int m) {
  for (int i = 0; i < g_ndh; i++) {
    DevHook *e = &g_dh[i];
    if (!e->live || e->mod != m) continue;
    if (g_dh_top[e->slot] == i) g_dh_top[e->slot] = e->below;
    else for (int u = 0; u < g_ndh; u++)
      if (g_dh[u].live && g_dh[u].slot == e->slot && g_dh[u].below == i) { g_dh[u].below = e->below; g_dh[u].next = e->next; }
    e->live = 0;
  }
}

/* the front layer onto a device whose vtable is now `below` (at the start, and on a new device) */
static void front_install(void **dev) {
  void **below = (void **)*dev;
  if (below == g_fvt) return;
  memcpy(g_below, below, sizeof g_below);
  for (int i = 0; i < g_ndh; i++) if (g_dh[i].live && g_dh[i].below < 0) g_dh[i].next = g_below[g_dh[i].slot];
  for (int k = 0; k < NSLOTS; k++) g_fvt[k] = g_dh_top[k] >= 0 ? (void *)DH_THUNK(k) : g_below[k];
  *(void ***)dev = g_fvt; g_front_on = 1;
}

/* ---- loading. With mod_reload=1 the kit loads a copy (xikit_mods_loaded\NAME.N.dll), so the build can write the
   mod's file while the game runs. Every loaded build stays loaded (pinned): a thread may still be in it. */
static HMODULE load_build(Mod *m, char *loaded, size_t n) {
  WIN32_FILE_ATTRIBUTE_DATA d;
  if (GetFileAttributesExA(m->path, GetFileExInfoStandard, &d)) m->ft = d.ftLastWriteTime;
  snprintf(loaded, n, "%s", m->path);
  if (g_mod_reload) {
    char dir[MAX_PATH], stem[64]; snprintf(dir, sizeof dir, "%s\\xikit_mods_loaded", g_dir); CreateDirectoryA(dir, NULL);
    snprintf(stem, sizeof stem, "%s", m->name); char *dot = strrchr(stem, '.'); if (dot) *dot = 0;
    snprintf(loaded, n, "%s\\%s.%d.dll", dir, stem, m->gen);
    if (!CopyFileA(m->path, loaded, FALSE)) { klog("mod %s: could not copy %s to %s (error %lu)", m->name, m->path, loaded, GetLastError()); return NULL; }
  }
  HMODULE h = LoadLibraryA(loaded);
  if (!h) klog("mod %s: could not load %s (error %lu)", m->name, loaded, GetLastError());
  return h;
}

static int start_mod(int i, HMODULE h, const char *loaded) {
  Mod *m = &g_mods[i];
  xikit_mod_init_fn init = (xikit_mod_init_fn)GetProcAddress(h, "xikit_mod_init");
  if (!init) { klog("mod %s: %s exports no xikit_mod_init; not used", m->name, loaded); return 0; }
  m->h = h; m->frame = (xikit_mod_frame_fn)GetProcAddress(h, "xikit_mod_frame"); m->unload = (xikit_mod_unload_fn)GetProcAddress(h, "xikit_mod_unload");
  uintptr_t *mr = (uintptr_t *)GetProcAddress(h, "xi_rel"); if (mr) *mr = g_rel;
  int32_t *ms = (int32_t *)GetProcAddress(h, "xi_shift");
  if (ms) memcpy(ms, g_shift, sizeof g_shift);
  else if (g_shift[0]) klog("mod %s exports no xi_shift (built with an SDK older than build translation): its game addresses in moved code would be wrong; not used", m->name);
  if (!ms && g_shift[0]) return 0;
  HMODULE pin; GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_PIN, loaded, &pin);
  g_mod_cur = i; g_mods_open = 1;
  int r = init(&g_host);
  g_mods_open = 0;
  klog("mod %s: %s at %p, init returned %d%s", m->name, m->gen ? "reloaded" : "loaded", h, r, m->frame ? "; it draws each frame (xikit_mod_frame)" : "");
  if (r != 0) m->frame = NULL;              /* (its hooks stay: they were checked and the mod asked for them) */
  g_mod_cur = -1; g_dirty = 1;
  return 1;
}

/* mods= (read once, at the start): load each, give it xi_rel, call its init */
static void load_mods(void *dev, HMODULE rec) {
  char list[1024]; GetPrivateProfileStringA("xikit", "mods", "", list, sizeof list, g_ini);
  if (!list[0]) return;
  g_mod_reload = GetPrivateProfileIntA("xikit", "mod_reload", 0, g_ini);
  if (!g_tramps) g_tramps = (uint8_t *)VirtualAlloc(NULL, MAXFN * 32, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
  make_dcode();
  g_host.version = XIKIT_API_VERSION; g_host.size = sizeof g_host;
  g_host.game_base = g_game_lo; g_host.rel = (intptr_t)g_rel; g_host.device = dev; g_host.recon = rec; g_host.dir = g_dir;
  g_host.log = host_log; g_host.hook = host_hook; g_host.reconstruction = host_recon; g_host.device_hook = host_device_hook;
  for (char *p = list; *p && g_nmods < MAXMODS;) {
    char nm[64]; int n = 0;
    while (*p == ' ' || *p == ',' || *p == '\t') p++;
    while (*p && *p != ',' && n < 63) nm[n++] = *p++;
    while (n && (nm[n - 1] == ' ' || nm[n - 1] == '\t')) n--;
    nm[n] = 0; if (!n) continue;
    Mod *m = &g_mods[g_nmods]; memset(m, 0, sizeof *m);
    snprintf(m->name, sizeof m->name, "%s", nm); snprintf(m->path, sizeof m->path, "%s\\%s", g_dir, nm);
    char loaded[MAX_PATH]; HMODULE h = load_build(m, loaded, sizeof loaded);
    if (!h) continue;
    int i = g_nmods++;
    if (!start_mod(i, h, loaded)) { g_nmods--; continue; }
  }
  if (g_mod_reload) klog("mod_reload=1: a mod's file that changes is loaded again (also: create xikit_reload_now.txt)");
  /* the speedfix mod (kit/speedfix) re-aims calls inside two speedfix-site functions, as speedfix_hook did: their
     reconstructions would bypass that, so respect_speedfix applies to it too */
  if (!g_speedfix_loaded && GetModuleHandleA("speedfix.dll")) {
    g_speedfix_loaded = 1;
    klog("speedfix.dll (the speedfix mod) is loaded: functions it changes inside (manifest flag 2) are left to the game's code (respect_speedfix=%d)", g_cfg.respect_speedfix);
  }
}

/* a reload, on the render thread inside Present (kit_present calls apply() right after, which re-aims the entries):
   the new build is loaded first; if it cannot be, the old one stays as it was */
static void reload_mod(int i) {
  Mod *m = &g_mods[i]; m->gen++;
  char loaded[MAX_PATH]; HMODULE h = load_build(m, loaded, sizeof loaded);
  if (!h) { m->gen--; return; }
  if (!GetProcAddress(h, "xikit_mod_init")) { klog("mod %s: the new build exports no xikit_mod_init; the old one stays", m->name); m->gen--; FreeLibrary(h); return; }
  g_mod_cur = i;
  if (m->unload) m->unload();
  g_mod_cur = -1;
  int nf = 0, nd = 0;
  for (int k = 0; k < g_nfn; k++) if (g_fn[k].mod_target && g_fn[k].mod == i + 1) { g_fn[k].mod_target = NULL; g_fn[k].mod = 0; g_fn[k].mod_on_recon = 0; nf++; }
  for (int k = 0; k < g_ndh; k++) nd += g_dh[k].live && g_dh[k].mod == i + 1;
  dh_remove_mod(i + 1);
  m->frame = NULL; m->unload = NULL;
  klog("mod %s: build %d: %d function hooks and %d device hooks of the old build taken off", m->name, m->gen, nf, nd);
  start_mod(i, h, loaded);
}

/* every 30 frames: a mod file newer than its loaded build, unchanged for one more check (the build has finished
   writing it), is reloaded; xikit_reload_now.txt reloads them all */
static void mods_watch(void) {
  if (!g_mod_reload || g_frame % 30 || g_tr_state) return;     /* (not in the middle of a trace) */
  char p[MAX_PATH]; snprintf(p, sizeof p, "%s\\xikit_reload_now.txt", g_dir);
  int all = DeleteFileA(p) != 0;
  for (int i = 0; i < g_nmods; i++) {
    Mod *m = &g_mods[i]; WIN32_FILE_ATTRIBUTE_DATA d;
    int changed = GetFileAttributesExA(m->path, GetFileExInfoStandard, &d) && CompareFileTime(&d.ftLastWriteTime, &m->ft) != 0;
    if (all || (changed && m->seen_change && CompareFileTime(&d.ftLastWriteTime, &m->seen) == 0)) { m->seen_change = 0; reload_mod(i); }
    else { m->seen_change = changed; if (changed) m->seen = d.ftLastWriteTime; }
  }
}

static void mods_frame(void *dev) {
  for (int i = 0; i < g_nmods; i++) if (g_mods[i].frame) { g_mod_cur = i; g_mods[i].frame(dev); }
  g_mod_cur = -1;
}


/* ------------------------------------------------------------------------------------------------ capture= (docs/KIT.md)
   An entry snapshot: when the game calls a chosen manifest function, the kit records the registers at its entry and
   the memory the call can reach, and writes them as xikit_capture_<pid>_<RVA>_<n>.xcap. pipeline/xcap.py turns a
   capture into a gate case that runs the original and the reconstruction from exactly that call (the gate's live
   mode), so code fuzzing and the four dumps never reach is compared on real inputs.

   What is recorded, all while every other thread of the process is suspended (nothing here allocates or logs
   until they run again):
     * the stack from the entry ESP up (the arguments and the callers' frames), at most 256 KB;
     * FFXiMain.dll's writable data, whole;
     * every committed, readable page a pointer reaches: from the registers and the stack, then from each page
       taken, breadth first, until capture_pages pages; the page after one is taken too when the pointer is in
       its last quarter (an object crossing the boundary). For the call's own pointers (the registers and the first
       8 arguments) the capture_follow pages after each are taken first, while readable: an object or command
       list longer than a page (SkinAndDrawV2's model, whose list ran into a page a budget-bound walk missed).
       FFXiMain's own image is not taken (the gate has it);
       a page of another module's code is, with the three after it (the gate reads a foreign callee's code to
       find what it pops).
   Which pages a call needs is not known beforehand: what pointers reach is a preparation of the inputs, not a
   fact a verdict rests on. Both sides run on the same memory; a page the capture lacks is filled as the gate
   fills any unknown page. */
#define MAXCAP 64
typedef struct { Fn *f; int count; volatile LONG done; uint8_t *stub; } Cap;
static Cap g_caps[MAXCAP]; static int g_ncap, g_cap_pages_max = 20000, g_cap_follow = 16; static volatile LONG g_cap_busy;
static DWORD g_cap_from;                       /* GetTickCount at which captures may start (capture_after) */
static uint32_t *g_cp_addr; static uint8_t *g_cp_data; static uint32_t *g_cp_set; static uint32_t g_cp_setn, g_cp_n;
static uint8_t *g_capstubs;

static int cp_seen(uint32_t page) {               /* open addressing set of page numbers; 1 if it was there */
  uint32_t k = page >> 12, h = (k * 2654435761u) & (g_cp_setn - 1);
  for (;;) {
    if (g_cp_set[h] == 0) { g_cp_set[h] = k + 1; return 0; }
    if (g_cp_set[h] == k + 1) return 1;
    h = (h + 1) & (g_cp_setn - 1);
  }
}
static int cp_readable(uint32_t a, int *exec) {
  static MEMORY_BASIC_INFORMATION mb; static uint32_t lo, hi;
  if (!(a >= lo && a < hi)) {
    if (!VirtualQuery((void *)(uintptr_t)a, &mb, sizeof mb)) { lo = hi = 0; return 0; }
    lo = (uint32_t)(uintptr_t)mb.BaseAddress; hi = lo + (uint32_t)mb.RegionSize;
  }
  DWORD p = mb.Protect;
  if (mb.State != MEM_COMMIT || (p & (PAGE_GUARD | PAGE_NOACCESS)) || !(p & 0xEE)) return 0;
  *exec = (p & 0xF0) != 0;
  return 1;
}
static void cp_take(uint32_t page) {               /* the page, if readable, not seen, and the budget allows */
  int exec = 0;
  if ((int)g_cp_n >= g_cap_pages_max || page < 0x10000) return;
  if (page >= g_game_lo && page < g_game_hi && !(page >= ANALYSED_BASE + 0x34F000u + g_rel && page < ANALYSED_BASE + 0x9FB000u + g_rel)) return;
  if (!cp_readable(page, &exec) || cp_seen(page)) return;
  g_cp_addr[g_cp_n] = page; memcpy(g_cp_data + (size_t)g_cp_n * 4096, (void *)(uintptr_t)page, 4096); g_cp_n++;
  if (exec) for (int k = 1; k <= 3; k++) cp_take(page + 4096u * k);
}
static void cp_ptr(uint32_t v) {
  cp_take(v & ~0xFFFu);
  if ((v & 0xFFF) >= 0xC00) cp_take((v & ~0xFFFu) + 0x1000);
}

static void cp_follow(uint32_t v) {               /* the pages after v's, up to capture_follow, while readable */
  int exec;
  if (v < 0x10000) return;
  for (int k = 1; k <= g_cap_follow; k++) {
    uint32_t p = (v & ~0xFFFu) + 4096u * (uint32_t)k;
    if (p < (v & ~0xFFFu) || !cp_readable(p, &exec)) break;
    cp_take(p);
  }
}

/* the stub jumps here with idx and the block pushad/pushfd left: edi esi ebp esp(+4 = the entry ESP) ebx edx ecx eax eflags */
static void __cdecl capture_c(int idx, uint32_t *r) {
  Cap *c = &g_caps[idx];
  if (c->done >= c->count || (int)(GetTickCount() - g_cap_from) < 0) return;
  if (InterlockedCompareExchange(&g_cap_busy, 1, 0)) return;
  if (c->done >= c->count) { g_cap_busy = 0; return; }
  uint32_t esp = r[3] + 4, regs[9] = { r[7], r[6], r[5], r[4], esp, r[2], r[1], r[0], r[8] };   /* eax ecx edx ebx esp ebp esi edi eflags */
  static Th th[1024];
  EnterCriticalSection(&g_susplock);
  int n = suspend_others(th, 1024);
  g_cp_n = 0; memset(g_cp_set, 0, g_cp_setn * 4);
  int exec;
  for (uint32_t a = esp & ~0xFFFu, k = 0; k < 64 && cp_readable(a, &exec); a += 4096, k++) cp_take(a);
  for (uint32_t a = ANALYSED_BASE + 0x34F000u + g_rel; a < ANALYSED_BASE + 0x9FB000u + g_rel; a += 4096) cp_take(a);
  for (int k = 0; k < 8; k++) if (k != 4) { cp_ptr(regs[k]); cp_follow(regs[k]); }
  for (int k = 1; k <= 8; k++) {                     /* the arguments: the dwords above the return address */
    uint32_t a = esp + 4u * (uint32_t)k;
    if (cp_readable(a & ~0xFFFu, &exec)) { uint32_t w = *(const uint32_t *)(uintptr_t)a; cp_ptr(w); cp_follow(w); }
  }
  for (uint32_t i = 0; i < g_cp_n; i++) {            /* breadth first: pages taken are scanned in the order taken */
    const uint32_t *w = (const uint32_t *)(g_cp_data + (size_t)i * 4096);
    for (int k = 0; k < 1024 && (int)g_cp_n < g_cap_pages_max; k++) if (w[k] >= 0x10000) cp_ptr(w[k]);
  }
  if (n > 0) resume_all(th, n);
  LeaveCriticalSection(&g_susplock);
  /* written with the threads running again */
  char path[MAX_PATH]; LONG nth = InterlockedIncrement(&c->done);
  snprintf(path, sizeof path, "%s\\xikit_capture_%lu_%05X_%ld.xcap", g_out, (unsigned long)GetCurrentProcessId(), c->f->rva, (long)nth);
  FILE *o = fopen(path, "wb"); uint32_t nmods = 0;
  if (o) {
    MODULEENTRY32 me; me.dwSize = sizeof me; HANDLE ms = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
    struct { uint32_t base, size; char name[64]; } mods[256];
    if (ms != INVALID_HANDLE_VALUE) {
      for (BOOL ok = Module32First(ms, &me); ok && nmods < 256; ok = Module32Next(ms, &me)) {
        mods[nmods].base = (uint32_t)(uintptr_t)me.modBaseAddr; mods[nmods].size = me.modBaseSize;
        memset(mods[nmods].name, 0, 64); snprintf(mods[nmods].name, 64, "%s", me.szModule); nmods++;
      }
      CloseHandle(ms);
    }
    uint32_t head[16] = { 0x50414358u /* "XCAP" */, 1, c->f->rva + ANALYSED_BASE, g_game_lo, g_game_hi - g_game_lo,
                          GetCurrentThreadId(), g_frame, nmods, g_cp_n, 0, 0, 0, 0, 0, 0, 0 };
    fwrite(head, 4, 16, o); fwrite(regs, 4, 9, o); fwrite(mods, sizeof mods[0], nmods, o);
    fwrite(g_cp_addr, 4, g_cp_n, o); fwrite(g_cp_data, 4096, g_cp_n, o); fclose(o);
  }
  if (c->done >= c->count) { c->f->cap_done = 1; g_dirty = 1; }
  g_cap_busy = 0;
  klog("capture %ld of %s (0x%08X) on thread %lu: %u pages, %u modules%s: %s", (long)nth, c->f->name, c->f->rva + ANALYSED_BASE,
       GetCurrentThreadId(), g_cp_n, nmods, (int)g_cp_n >= g_cap_pages_max ? " (the page budget ran out: capture_pages)" : "",
       o ? path : "could not write");
}

/* capture= (read once, at the start): a stub per function, entered from the game's entry; it continues into the
   game's own code (a trampoline), or the reconstruction where the entry has none */
static void setup_captures(void) {
  char list[2048]; GetPrivateProfileStringA("xikit", "capture", "", list, sizeof list, g_ini);
  if (!list[0]) return;
  int count = GetPrivateProfileIntA("xikit", "capture_count", 1, g_ini), after = GetPrivateProfileIntA("xikit", "capture_after", 0, g_ini);
  g_cap_pages_max = GetPrivateProfileIntA("xikit", "capture_pages", 20000, g_ini);
  if (g_cap_pages_max < 2048) g_cap_pages_max = 2048;
  g_cap_follow = GetPrivateProfileIntA("xikit", "capture_follow", 16, g_ini);
  if (g_cap_follow < 0) g_cap_follow = 0;
  if (g_cap_follow > 256) g_cap_follow = 256;
  g_cap_from = GetTickCount() + (DWORD)(after > 0 ? after : 0) * 1000;
  for (g_cp_setn = 1; g_cp_setn < (uint32_t)g_cap_pages_max * 4; g_cp_setn <<= 1) {}
  g_cp_addr = (uint32_t *)VirtualAlloc(NULL, (size_t)g_cap_pages_max * 4, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  g_cp_data = (uint8_t *)VirtualAlloc(NULL, (size_t)g_cap_pages_max * 4096, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  g_cp_set = (uint32_t *)VirtualAlloc(NULL, (size_t)g_cp_setn * 4, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  g_capstubs = (uint8_t *)VirtualAlloc(NULL, MAXCAP * 32, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
  if (!g_tramps) g_tramps = (uint8_t *)VirtualAlloc(NULL, MAXFN * 32, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
  if (!g_cp_addr || !g_cp_data || !g_cp_set || !g_capstubs) { klog("capture: could not reserve %d pages; no captures", g_cap_pages_max); return; }
  for (char *p = list; *p && g_ncap < MAXCAP;) {
    char nm[128]; int n = 0;
    while (*p == ' ' || *p == ',' || *p == '\t') p++;
    while (*p && *p != ',' && n < 127) nm[n++] = *p++;
    while (n && (nm[n - 1] == ' ' || nm[n - 1] == '\t')) n--;
    nm[n] = 0; if (!n) continue;
    Fn *f = fn_named(nm);
    if (!f) { klog("capture %s: not a function of the manifest", nm); continue; }
    if (f->mod_target) { klog("capture %s: a mod hooks it; not captured", nm); continue; }
    int d = check_bytes(f);
    if (d != -1) { klog("capture %s: the live entry differs from the analysed build at %+d", nm, d); continue; }
    void *prev = build_tramp(f); if (!prev) prev = f->target;
    if (!prev) { klog("capture %s: no way to continue into its code", nm); continue; }
    Cap *c = &g_caps[g_ncap]; uint8_t *st = g_capstubs + 32 * g_ncap;
    st[0] = 0x9C; st[1] = 0x60; st[2] = 0x54;                                   /* pushfd; pushad; push esp */
    st[3] = 0x68; uint32_t idx = (uint32_t)g_ncap; memcpy(st + 4, &idx, 4);     /* push idx */
    make_jmp(st + 8, st + 8, (void *)capture_c); st[8] = 0xE8;                  /* call capture_c */
    st[13] = 0x83; st[14] = 0xC4; st[15] = 0x08;                                /* add esp, 8 */
    st[16] = 0x61; st[17] = 0x9D;                                               /* popad; popfd */
    make_jmp(st + 18, st + 18, prev);                                           /* jmp to its code */
    c->f = f; c->count = count > 0 ? count : 1; c->stub = st; f->cap_stub = st; g_ncap++;
    klog("capture %s (0x%08X): %d call(s), from %d s after the start, up to %d pages each; continues into %s", f->name,
         f->rva + ANALYSED_BASE, c->count, after, g_cap_pages_max, prev == f->target ? "the reconstruction (no trampoline)" : "the game's code");
  }
  if (g_ncap) { g_dirty = 1; FlushInstructionCache(GetCurrentProcess(), g_capstubs, MAXCAP * 32); }
}


/* ------------------------------------------------------------------------------------------------ facts=1 (docs/KIT.md)
   The call sites the gate cannot settle (pipeline/fact_sites.py: each with a patch region that ends at the call's
   return address and that nothing jumps into), measured in the running game: the region runs from a copy that
   records ESP just before the call and just after it returns (their difference is what the callee popped), and
   where it went (the call's target, read as the call reads it; for `call [reg+disp]` also reg, the vtable, and
   disp/4, the slot). xikit_facts_<pid>.csv every window; fact_sites.py ingest turns agreeing observations into
   data/game_sp.json, a source the gate reads. */
#define MAXSITE 256
#define SITE_OBS 8
typedef struct { uint32_t pops, target, vt, slot; volatile LONG n; } SiteObs;
typedef struct {
  uint32_t site, start, len, calloff, func; int fix, nrel; int8_t rel[4]; uint8_t expect[16];
  uint8_t *at, *stub, saved[JMP_LEN]; int patched, refused;
  SiteObs obs[SITE_OBS]; volatile LONG nobs, overflow;
} Site;
static Site g_sites[MAXSITE]; static int g_facts; static DWORD g_fact_tls = TLS_OUT_OF_INDEXES;
static volatile LONG g_fact_lock;
typedef struct { int depth; struct { int idx; uint32_t esp, target, vt, slot; } e[16]; } FactThread;
static FactThread g_fact_pool[128]; static volatile LONG g_fact_npool;

static int site_bytes_ok(Site *t) {
  uint8_t live[16], care[16]; memcpy(live, t->at, t->len); memset(care, 1, sizeof care);
  for (int i = 0; i < t->nrel; i++) { int p = t->rel[i]; uint32_t v; memcpy(&v, live + p, 4); v -= g_rel; memcpy(live + p, &v, 4); (void)care; }
  return !memcmp(live, t->expect, t->len);
}
static FactThread *fact_thread(void) {
  FactThread *ft = (FactThread *)TlsGetValue(g_fact_tls);
  if (!ft) { LONG k = InterlockedIncrement(&g_fact_npool) - 1; if (k >= 128) return NULL; ft = &g_fact_pool[k]; TlsSetValue(g_fact_tls, ft); }
  return ft;
}
/* the stubs call these with the site index and the pushad/pushfd block: edi esi ebp esp(+4 = ESP at the call) ebx edx ecx eax eflags */
static void __cdecl fact_pre(int idx, uint32_t *r) {
  Site *t = &g_sites[idx]; FactThread *ft = fact_thread();
  if (!ft || ft->depth >= 16) return;
  uint32_t reg[8] = { r[7], r[6], r[5], r[4], r[3] + 4, r[2], r[1], r[0] };   /* eax ecx edx ebx esp ebp esi edi */
  const uint8_t *c = t->expect + t->calloff; uint32_t target = 0, vt = 0, slot = 0xFFFFFFFFu;
  if (c[0] == 0xE8) { int32_t d; memcpy(&d, c + 1, 4); target = t->site + 5 + d + g_rel; }
  else if (c[0] == 0xFF && ((c[1] >> 3) & 7) == 2) {
    int mod = c[1] >> 6, rm = c[1] & 7; uint32_t ea = 0; const uint8_t *q = c + 2; int base = rm;
    if (mod == 3) target = reg[rm];
    else {
      if (rm == 4) { uint8_t sib = *q++; int sc = sib >> 6, ix = (sib >> 3) & 7; base = sib & 7; if (ix != 4) ea += reg[ix] << sc; }
      if (mod == 0 && base == 5) { int32_t d; memcpy(&d, q, 4); ea += (uint32_t)d + g_rel; base = -1; }   /* [disp32]: an absolute address, relocated */
      else ea += reg[base];
      if (mod == 1) { ea += (uint32_t)(int32_t)(int8_t)*q; slot = (uint32_t)(int32_t)(int8_t)*q; }
      else if (mod == 2) { int32_t d; memcpy(&d, q, 4); ea += (uint32_t)d; slot = (uint32_t)d; }
      else slot = 0;
      if (base >= 0 && rm != 4) { vt = reg[base]; slot = slot / 4; } else slot = 0xFFFFFFFFu;
      target = *(const uint32_t *)(uintptr_t)ea;                 /* the read the call itself makes next */
    }
  }
  int d = ft->depth++; ft->e[d].idx = idx; ft->e[d].esp = r[3] + 4; ft->e[d].target = target; ft->e[d].vt = vt; ft->e[d].slot = slot;
}
static void __cdecl fact_post(int idx, uint32_t *r) {
  FactThread *ft = fact_thread(); Site *t = &g_sites[idx];
  if (!ft) return;
  while (ft->depth > 0 && ft->e[ft->depth - 1].idx != idx) ft->depth--;      /* (a callee that never returned here) */
  if (ft->depth <= 0) return;
  int d = --ft->depth; uint32_t pops = r[3] + 4 - ft->e[d].esp;
  while (InterlockedCompareExchange(&g_fact_lock, 1, 0)) {}
  int k = 0;
  for (; k < t->nobs; k++) { SiteObs *o = &t->obs[k]; if (o->pops == pops && o->target == ft->e[d].target && o->vt == ft->e[d].vt && o->slot == ft->e[d].slot) break; }
  if (k == t->nobs) { if (k < SITE_OBS) { t->obs[k].pops = pops; t->obs[k].target = ft->e[d].target; t->obs[k].vt = ft->e[d].vt; t->obs[k].slot = ft->e[d].slot; t->obs[k].n = 0; t->nobs++; } else { t->overflow++; k = -1; } }
  if (k >= 0) t->obs[k].n++;
  g_fact_lock = 0;
}
static uint8_t *emit_hook(uint8_t *p, int idx, void *fn) {
  p[0] = 0x9C; p[1] = 0x60; p[2] = 0x54; p[3] = 0x68; uint32_t v = (uint32_t)idx; memcpy(p + 4, &v, 4);
  make_jmp(p + 8, p + 8, fn); p[8] = 0xE8; p[13] = 0x83; p[14] = 0xC4; p[15] = 0x08; p[16] = 0x61; p[17] = 0x9D;
  return p + 18;
}
static void setup_facts(void) {
  g_facts = GetPrivateProfileIntA("xikit", "facts", 0, g_ini);
  if (!g_facts) return;
  char path[MAX_PATH], line[256]; snprintf(path, sizeof path, "%s\\xikit_facts.txt", g_dir);
  FILE *f = fopen(path, "r");
  if (!f) { klog("facts=1: no xikit_facts.txt next to xikit.dll (pipeline/fact_sites.py sites)"); g_facts = 0; return; }
  uint8_t *stubs = (uint8_t *)VirtualAlloc(NULL, MAXSITE * 96, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
  g_fact_tls = TlsAlloc();
  while (stubs && g_fact_tls != TLS_OUT_OF_INDEXES && fgets(line, sizeof line, f) && g_nsites < MAXSITE) {
    if (line[0] == '#' || line[0] == '\n') continue;
    Site *t = &g_sites[g_nsites]; char hex[64], rels[64];
    if (sscanf(line, "%x %x %u %u %d %63s %63s %x", &t->site, &t->start, &t->len, &t->calloff, &t->fix, hex, rels, &t->func) != 8
        || t->len < 5 || t->len > 16 || strlen(hex) != 2 * t->len) { klog("facts: bad line ignored: %.80s", line); continue; }
    for (unsigned i = 0; i < t->len; i++) t->expect[i] = (uint8_t)(hexval(hex[2 * i]) << 4 | hexval(hex[2 * i + 1]));
    t->nrel = 0;
    if (strcmp(rels, "-")) for (char *p = rels; *p && t->nrel < 4;) { t->rel[t->nrel++] = (int8_t)strtol(p, &p, 10); if (*p == ',') p++; }
    t->at = (uint8_t *)(uintptr_t)live_of(t->start);
    /* the stub: the instructions before the call (copied), the pre hook, the call (a direct one re-aimed), the post hook, back */
    uint8_t *p = t->stub = stubs + 96 * g_nsites;
    memcpy(p, t->at, t->calloff); p += t->calloff;
    p = emit_hook(p, g_nsites, (void *)fact_pre);
    unsigned clen = t->len - t->calloff; memcpy(p, t->at + t->calloff, clen);
    if (t->fix >= 0) { int32_t d; memcpy(&d, t->at + t->fix, 4); d += (int32_t)((uintptr_t)t->at - (uintptr_t)(t->stub + (p - t->stub) - t->calloff)); memcpy(p + (t->fix - t->calloff), &d, 4); }
    p += clen;
    p = emit_hook(p, g_nsites, (void *)fact_post);
    make_jmp(p, p, t->at + t->len);
    g_nsites++;
  }
  fclose(f);
  if (stubs) FlushInstructionCache(GetCurrentProcess(), stubs, MAXSITE * 96);
  klog("facts=1: %d call sites to measure (xikit_facts_<pid>.csv every %d frames)", g_nsites, g_cfg.report_every);
  g_dirty = 1;
}
/* inside apply's suspended section: each site patched once (it stays while the kit runs) */
static void apply_sites(const uint32_t *eips, int n, char msgs[][200], int *nmsg) {
  for (int i = 0; i < g_nsites; i++) {
    Site *t = &g_sites[i];
    if (t->patched || t->refused) continue;
    int inside = 0;
    for (int k = 0; k < n; k++) if (eips[k] >= (uint32_t)(uintptr_t)t->at && eips[k] < (uint32_t)(uintptr_t)t->at + t->len) inside = 1;
    if (inside) continue;
    if (!site_bytes_ok(t)) { t->refused = 1; if (*nmsg < 64) snprintf(msgs[(*nmsg)++], 200, "facts: site 0x%08X refused: its live bytes are not the analysed build's", t->site); continue; }
    uint8_t jmp[JMP_LEN]; make_jmp(jmp, t->at, t->stub); memcpy(t->saved, t->at, JMP_LEN);
    if (write5(t->at, jmp)) t->patched = 1;
  }
}
static int sites_pending(void) { for (int i = 0; i < g_nsites; i++) if (!g_sites[i].patched && !g_sites[i].refused) return 1; return 0; }
static void where(uint32_t a, char *out, size_t n);
static void report_facts(void) {
  if (!g_nsites) return;
  char path[MAX_PATH]; snprintf(path, sizeof path, "%s\\xikit_facts_%lu.csv", g_out, (unsigned long)GetCurrentProcessId());
  FILE *f = fopen(path, "w"); if (!f) return;
  fprintf(f, "# xikit facts: per call site (analysed), what the callee popped (ESP after the call minus ESP before it), how often, where it went;"
             " frame %u; FFXiMain.dll at +0x%X\nsite,pops,calls,target,vtable,slot,function\n", g_frame, g_rel);
  for (int i = 0; i < g_nsites; i++) {
    Site *t = &g_sites[i]; char tw[300], vw[300];
    for (int k = 0; k < t->nobs; k++) {
      where(t->obs[k].target, tw, sizeof tw); if (t->obs[k].vt) where(t->obs[k].vt, vw, sizeof vw); else snprintf(vw, sizeof vw, "-");
      fprintf(f, "0x%08X,%u,%ld,%s,%s,%ld,0x%08X\n", t->site, t->obs[k].pops, (long)t->obs[k].n, tw, vw, t->obs[k].slot == 0xFFFFFFFFu ? -1L : (long)t->obs[k].slot, t->func);
    }
    if (t->refused) fprintf(f, "# 0x%08X refused (bytes)\n", t->site);
    if (t->overflow) fprintf(f, "# 0x%08X: %ld calls with an observation beyond the %d kept\n", t->site, (long)t->overflow, SITE_OBS);
  }
  fclose(f);
}

/* ------------------------------------------------------------------------------------------------ start */
/* ------------------------------------------------------------------------------------------------ environment
   What the game runs on, logged once the device exists: the CPU as CPUID reports it (the game's own detection,
   0x1000D400, chooses its SSE code by the vendor string), the machine and Windows build (an x86 process on ARM64
   Windows is emulated), and every graphics module loaded, with its path, file version and PE machine type: which
   Direct3D 8 implementation the calls go through (a d3d8.dll next to the game, such as dgVoodoo's, or Windows' own)
   and what lies under it. */
static void cpuid_regs(unsigned leaf, unsigned r[4]) {
#ifdef _MSC_VER
  int x[4]; __cpuid(x, (int)leaf); for (int i = 0; i < 4; i++) r[i] = (unsigned)x[i];
#else
  if (!__get_cpuid(leaf, &r[0], &r[1], &r[2], &r[3])) r[0] = r[1] = r[2] = r[3] = 0;
#endif
}
static const char *machine_name(unsigned m) {
  return m == 0x14c ? "x86" : m == 0x8664 ? "x64" : m == 0xAA64 ? "ARM64" : m == 0x1c4 ? "ARMNT" : m == 0xA641 ? "ARM64EC" : m == 0x3A64 ? "CHPE x86" : "other";
}
static void log_environment(void) {
  unsigned r[4]; char vendor[13] = {0}, brand[49] = {0};
  cpuid_regs(0, r); memcpy(vendor, &r[1], 4); memcpy(vendor + 4, &r[3], 4); memcpy(vendor + 8, &r[2], 4);
  cpuid_regs(0x80000000u, r);
  if (r[0] >= 0x80000004u) for (unsigned l = 0; l < 3; l++) { cpuid_regs(0x80000002u + l, r); memcpy(brand + 16 * l, r, 16); }
  cpuid_regs(1, r);
  klog("cpu: CPUID vendor \"%s\", brand \"%s\"; SSE %u SSE2 %u SSE3 %u SSSE3 %u SSE4.1 %u SSE4.2 %u AVX %u (the game's own detection turns its SSE code on only for an Intel or Transmeta vendor)",
       vendor, brand, r[3] >> 25 & 1, r[3] >> 26 & 1, r[2] & 1, r[2] >> 9 & 1, r[2] >> 19 & 1, r[2] >> 20 & 1, r[2] >> 28 & 1);
  typedef BOOL (WINAPI *IW2)(HANDLE, USHORT *, USHORT *);
  IW2 iw2 = (IW2)(void *)GetProcAddress(GetModuleHandleA("kernel32.dll"), "IsWow64Process2");
  USHORT pm = 0, nm = 0;
  typedef LONG (WINAPI *RGV)(OSVERSIONINFOW *);
  RGV rgv = (RGV)(void *)GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlGetVersion");
  OSVERSIONINFOW ov; memset(&ov, 0, sizeof ov); ov.dwOSVersionInfoSize = sizeof ov;
  if (rgv) rgv(&ov);
  if (iw2 && iw2(GetCurrentProcess(), &pm, &nm))
    klog("machine: native %s (0x%X), this process %s; Windows %lu.%lu build %lu", machine_name(nm), nm, pm ? "emulated or WOW64" : "native",
         (unsigned long)ov.dwMajorVersion, (unsigned long)ov.dwMinorVersion, (unsigned long)ov.dwBuildNumber);
  else klog("machine: IsWow64Process2 not available; Windows %lu.%lu build %lu", (unsigned long)ov.dwMajorVersion, (unsigned long)ov.dwMinorVersion, (unsigned long)ov.dwBuildNumber);
  /* compatibility layers: the kernel's own version (KUSER_SHARED_DATA at 0x7FFE0000: NtMajorVersion +0x26C,
     NtMinorVersion +0x270, NtBuildNumber +0x260 from Windows 10 on; no shim changes it), the layers the process was
     started with (__COMPAT_LAYER) and the layers the registry sets for this exe (AppCompatFlags\Layers) */
  { volatile const uint8_t *kusd = (volatile const uint8_t *)(uintptr_t)0x7FFE0000u;
    char exe[MAX_PATH] = "", layer[512] = "", hku[512] = "", hkl[512] = "";
    GetModuleFileNameA(NULL, exe, sizeof exe);
    if (!GetEnvironmentVariableA("__COMPAT_LAYER", layer, sizeof layer)) snprintf(layer, sizeof layer, "(not set)");
    const char *key = "Software\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Layers";
    HKEY roots[2] = { HKEY_CURRENT_USER, HKEY_LOCAL_MACHINE }; char *outs[2] = { hku, hkl };
    for (int k = 0; k < 2; k++) {
      HKEY h; DWORD n = 511, t = 0;
      if (RegOpenKeyExA(roots[k], key, 0, KEY_QUERY_VALUE, &h) == ERROR_SUCCESS) {
        if (RegQueryValueExA(h, exe, NULL, &t, (BYTE *)outs[k], &n) != ERROR_SUCCESS || t != REG_SZ) snprintf(outs[k], 512, "(none)");
        else outs[k][n < 512 ? n : 511] = 0;
        RegCloseKey(h);
      } else snprintf(outs[k], 512, "(none)");
    }
    klog("compatibility: kernel version %lu.%lu build %lu (KUSER_SHARED_DATA; the game is told %lu.%lu); __COMPAT_LAYER %s; "
         "registry layers for %s: user %s, machine %s",
         (unsigned long)*(volatile const uint32_t *)(kusd + 0x26C), (unsigned long)*(volatile const uint32_t *)(kusd + 0x270),
         (unsigned long)(*(volatile const uint32_t *)(kusd + 0x260) & 0xFFFF), (unsigned long)ov.dwMajorVersion, (unsigned long)ov.dwMinorVersion,
         layer, exe, hku, hkl); }
  /* the graphics modules */
  HMODULE ver = LoadLibraryA("version.dll");
  typedef DWORD (WINAPI *GFVIS)(LPCSTR, LPDWORD); typedef BOOL (WINAPI *GFVI)(LPCSTR, DWORD, DWORD, LPVOID); typedef BOOL (WINAPI *VQV)(LPCVOID, LPCSTR, LPVOID *, PUINT);
  GFVIS gfvis = ver ? (GFVIS)(void *)GetProcAddress(ver, "GetFileVersionInfoSizeA") : NULL;
  GFVI gfvi = ver ? (GFVI)(void *)GetProcAddress(ver, "GetFileVersionInfoA") : NULL;
  VQV vqv = ver ? (VQV)(void *)GetProcAddress(ver, "VerQueryValueA") : NULL;
  static const char *const pat[] = {"d3d8", "d3d9", "d3d10", "d3d11", "d3d12", "dxgi", "dxcore", "d3dim", "ddraw", "dgvoodoo", "prl_", "opengl32", "vulkan", "atidxx", "nvd3d", "igd", "d3d8thk", "dxvk", "d3dcompiler", "d3dx"};
  HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetCurrentProcessId());
  if (snap == INVALID_HANDLE_VALUE) { klog("graphics modules: the module list could not be read (%lu)", GetLastError()); return; }
  MODULEENTRY32 me; me.dwSize = sizeof me; int n = 0;
  for (BOOL ok = Module32First(snap, &me); ok; ok = Module32Next(snap, &me)) {
    char low[MAX_PATH]; size_t i = 0;
    for (; me.szModule[i] && i < sizeof low - 1; i++) low[i] = (char)(me.szModule[i] >= 'A' && me.szModule[i] <= 'Z' ? me.szModule[i] + 32 : me.szModule[i]);
    low[i] = 0;
    int hit = 0; for (unsigned k = 0; k < sizeof pat / sizeof *pat; k++) if (strstr(low, pat[k])) hit = 1;
    if (!hit) continue;
    char fv[64] = "?";
    if (gfvis && gfvi && vqv) {
      DWORD h = 0, sz = gfvis(me.szExePath, &h);
      void *buf = sz ? malloc(sz) : NULL; VS_FIXEDFILEINFO *fi = NULL; UINT fl = 0;
      if (buf && gfvi(me.szExePath, 0, sz, buf) && vqv(buf, "\\", (LPVOID *)&fi, &fl) && fi && fl >= sizeof *fi)
        snprintf(fv, sizeof fv, "%u.%u.%u.%u", (unsigned)(fi->dwFileVersionMS >> 16), (unsigned)(fi->dwFileVersionMS & 0xFFFF),
                 (unsigned)(fi->dwFileVersionLS >> 16), (unsigned)(fi->dwFileVersionLS & 0xFFFF));
      free(buf);
    }
    unsigned mach = 0;
    IMAGE_DOS_HEADER *dh = (IMAGE_DOS_HEADER *)me.modBaseAddr;
    if (dh && dh->e_magic == IMAGE_DOS_SIGNATURE) mach = ((IMAGE_NT_HEADERS *)(me.modBaseAddr + dh->e_lfanew))->FileHeader.Machine;
    klog("graphics module: %s at %p, version %s, PE machine %s: %s", me.szModule, (void *)me.modBaseAddr, fv, machine_name(mach), me.szExePath);
    n++;
  }
  CloseHandle(snap);
  if (!n) klog("graphics modules: none of the known names loaded");
}

static DWORD WINAPI start(LPVOID self) {
  char path[MAX_PATH], recon[MAX_PATH];
  GetModuleFileNameA((HMODULE)self, g_dir, MAX_PATH);
  char *slash = strrchr(g_dir, '\\'); if (slash) *slash = 0;
  snprintf(g_ini, sizeof g_ini, "%s\\xikit.ini", g_dir);
  if (GetFileAttributesA(g_ini) == INVALID_FILE_ATTRIBUTES) {
    FILE *f = fopen(g_ini, "wb"); if (f) { fwrite(DEFAULT_INI, 1, sizeof DEFAULT_INI - 1, f); fclose(f); }
  }
  snprintf(g_out, sizeof g_out, "%s", g_dir);
  if (GetPrivateProfileIntA("xikit", "log_folder", 1, g_ini)) {   /* a folder of its own for this start's files */
    char base[MAX_PATH], d[MAX_PATH]; SYSTEMTIME st; GetLocalTime(&st);
    snprintf(base, sizeof base, "%s\\xikit_logs", g_dir); CreateDirectoryA(base, NULL);
    snprintf(d, sizeof d, "%s\\%04u-%02u-%02u_%02u%02u%02u_%lu", base, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,
             (unsigned long)GetCurrentProcessId());
    if (CreateDirectoryA(d, NULL) || GetLastError() == ERROR_ALREADY_EXISTS) snprintf(g_out, sizeof g_out, "%s", d);
  }
  snprintf(g_logpath, sizeof g_logpath, "%s\\xikit.log", g_out);
  snprintf(g_quar, sizeof g_quar, "%s\\xikit_quarantine.txt", g_dir);
  klog("xikit started in process %lu", (unsigned long)GetCurrentProcessId());
  {                                     /* the kit's build stamp (build.sh writes VERSION.txt): which source this is */
    char vp[MAX_PATH], line[256] = ""; snprintf(vp, sizeof vp, "%s\\VERSION.txt", g_dir);
    FILE *vf = fopen(vp, "r");
    if (vf) { if (fgets(line, sizeof line, vf)) line[strcspn(line, "\r\n")] = 0; fclose(vf); }
    klog("kit build: %s", line[0] ? line : "no VERSION.txt next to xikit.dll");
  }
  if (strcmp(g_out, g_dir)) klog("this start's files go to %s (log_folder=1)", g_out);
  ini_changed(); read_cfg();
  HMODULE game;
  while (!(game = GetModuleHandleA("FFXiMain.dll"))) Sleep(100);   /* injected possibly long before it loads */
  IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS *)((char *)game + ((IMAGE_DOS_HEADER *)game)->e_lfanew);
  /* the manifest made for this build: the analysed build's, or another's (build translation) */
  snprintf(path, sizeof path, "%s\\xikit_manifest_%08lX.txt", g_dir, (unsigned long)nt->FileHeader.TimeDateStamp);
  if (GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES) snprintf(path, sizeof path, "%s\\xikit_manifest.txt", g_dir);
  if (!load_manifest(path)) return 0;
  if (g_shift[0]) {
    char b[256]; int o = 0;
    for (int i = 0; i < g_shift[0] && o < 200; i++) o += snprintf(b + o, sizeof b - o, " %08X-%08X%+d", g_shift[1 + 3 * i], g_shift[2 + 3 * i], g_shift[3 + 3 * i]);
    klog("FFXiMain.dll build %08lX: its manifest %s translates the analysed build's code ranges:%s", (unsigned long)nt->FileHeader.TimeDateStamp, path, b);
  }
  snprintf(recon, sizeof recon, "%s\\gfx_recon_ingame.dll", g_dir);
  uint32_t size = 0, crc = crc32_file(recon, &size);
  if (!size || size != g_recon_size || crc != g_recon_crc) {
    klog("gfx_recon_ingame.dll is not the one the manifest was made from (size %u, CRC-32 %08X; manifest %u, %08X): nothing done",
         size, crc, g_recon_size, g_recon_crc);
    return 0;
  }
  if (nt->FileHeader.TimeDateStamp != g_game_ts) {
    klog("FFXiMain.dll is a build the kit has no manifest for (TimeDateStamp %08lX; the manifest is for %08X): nothing done. A new build is added with pipeline/unpack_pol.py and pipeline/build_translate.py (docs/KIT.md)", nt->FileHeader.TimeDateStamp, g_game_ts);
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
  int32_t *xs = (int32_t *)GetProcAddress(rec, "xi_shift");
  if (xs) memcpy(xs, g_shift, sizeof g_shift);
  else if (g_shift[0]) { klog("the reconstruction DLL exports no xi_shift (built before build translation): nothing done on this build"); return 0; }
  IMAGE_NT_HEADERS *rnt = (IMAGE_NT_HEADERS *)((char *)rec + ((IMAGE_DOS_HEADER *)rec)->e_lfanew);
  g_recon_lo = (uint32_t)(uintptr_t)rec; g_recon_hi = g_recon_lo + rnt->OptionalHeader.SizeOfImage;
  g_speedfix_loaded = GetModuleHandleA("speedfix_hook.dll") != NULL;
  int missing = 0;
  for (int i = 0; i < g_nfn; i++) {
    Fn *f = &g_fn[i];
    f->site = (uint8_t *)(uintptr_t)live_of(f->rva + ANALYSED_BASE);
    f->target = (void *)GetProcAddress(rec, f->exp);
    if (!f->target) { missing++; f->refused = 1; }
    g_by_target[i] = f;
  }
  for (int k = 0; k < g_ncode; k++)                 /* each code symbol to its manifest function, by export name */
    for (int i = 0; i < g_nfn; i++) if (!strcmp(g_code[k].sym, g_fn[i].exp)) { g_code[k].fn = i; break; }
  for (int k = 1; k < g_ncode; k++) if (g_code[k].rva < g_code[k - 1].rva) { klog("manifest: !code lines not in address order; faults attributed by the nearest entry instead"); g_ncode = 0; break; }
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
  load_mods(dev, rec);                        /* before the Present hook: nothing is written at any entry yet */
  setup_captures();                           /* (likewise: trampolines are built from the untouched entries) */
  start_sampler();
  setup_facts();
  if (g_tramps && !g_mod_reload) { DWORD old; VirtualProtect(g_tramps, MAXFN * 32, PAGE_EXECUTE_READ, &old); }
  g_orig_vt = (void **)*dev;
  memcpy(g_vt, g_orig_vt, sizeof g_vt);
  g_vt[SLOT_PRESENT] = (void *)kit_present;
  *(void ***)dev = g_vt;                      /* this object only: the driver's own vtable is not written */
  klog("device %p hooked (Present); swaps are applied at the next frame (swap=%d)", dev, g_cfg.swap);
  log_environment();
  if (g_cfg.profile || g_cfg.trace) {
    char csv[MAX_PATH]; snprintf(csv, sizeof csv, "%s\\d3dprof_%lu.csv", g_out, (unsigned long)GetCurrentProcessId());
    d3dprof_call_time = GetPrivateProfileIntA("xikit", "call_time", 0, g_ini);
    d3dprof_set_shift(g_shift);
    d3dprof_attach(dev, g_rel, g_game_lo, g_game_hi, csv);
    if (d3dprof_call_time) klog("call_time=1: d3dprof times each Direct3D call on the render thread");
    g_trace_attached = g_cfg.trace;      /* (like profile, read at the start only) */
    g_tr_after = GetPrivateProfileIntA("xikit", "trace_after", 0, g_ini); g_tr_every = GetPrivateProfileIntA("xikit", "trace_every", 120, g_ini);
    g_tr_count = GetPrivateProfileIntA("xikit", "trace_count", 3, g_ini);
    g_tr_buffers = GetPrivateProfileIntA("xikit", "trace_buffers", 1, g_ini); g_tr_alt = GetPrivateProfileIntA("xikit", "trace_alternate", 1, g_ini); g_tr_pictures = GetPrivateProfileIntA("xikit", "trace_pictures", 0, g_ini); if (g_tr_every < 20) g_tr_every = 20;
    klog("profiler attached: %s%s", csv, g_cfg.trace ? " (Ctrl+Alt+F8: A/B trace)" : "");
  }
  if (g_nmods) {                              /* the mods' device hooks, in front of everything (device_hook) */
    front_install(dev);
    int n = 0; for (int k = 0; k < NSLOTS; k++) n += g_dh_top[k] >= 0;
    klog("device hooks: %d methods hooked by mods", n);
  }
  /* watchdog: if the game ever makes a new device object, give that one the hook too (Reset keeps the object) */
  for (void **last = dev;;) {
    Sleep(1000);
    void **c = (void **)*cdx; void **d = c ? (void **)c[CDX_DEVICE_OFF / 4] : NULL;
    if (!d || d == last) continue;
    void **vt = (void **)*d;
    if (vt != g_vt && vt != (void **)g_fvt) {
      g_orig_vt = vt; memcpy(g_vt, vt, sizeof g_vt); g_vt[SLOT_PRESENT] = (void *)kit_present;
      *(void ***)d = g_vt;
      if (g_front_on) front_install(d);
      klog("a new device %p: hooked (Present%s)%s", d, g_front_on ? ", the mods' device hooks" : "", g_cfg.profile ? "; the profiler stays on the old one" : "");
    }
    last = d;
  }
}

BOOL WINAPI DllMain(HINSTANCE h, DWORD why, LPVOID r) {
  (void)r;
  if (why == DLL_PROCESS_ATTACH) {
    DisableThreadLibraryCalls(h); InitializeCriticalSection(&g_loglock); InitializeCriticalSection(&g_susplock); InitializeCriticalSection(&g_samplock);
    HANDLE t = CreateThread(NULL, 0, start, h, 0, NULL); if (t) CloseHandle(t);
  }
  return TRUE;
}
