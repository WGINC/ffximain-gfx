/* xisamp.c -- the native sampler (README.md): a 64-bit process of its own (ARM64 on Windows on ARM, x64 on an x64
   Windows) that samples one thread of a 32-bit (x86) process and records, for each sample, both what the x86 code
   was doing and what the machine was doing natively.

     xisamp_<arch>.exe <pid> <tid> <interval_ms> <window_s> <outdir> [<rel> [<lo:hi:shift>...] [frame=<hex>] [spike=<ms>]]

   xikit (sample=2) starts it with the game's render thread. Per sample, with the thread suspended:
     * the x86 context (Wow64GetThreadContext; NtQueryInformationThread ThreadWow64Context if that fails): Eip, Esp,
       and 16 KB of the x86 stack from Esp (one ReadProcessMemory);
     * the native context (GetThreadContext): ARM64 Pc, Lr, Fp, Sp (x64: Rip, Rsp) and up to 64 KB of the native
       stack from Sp.
   Then, the thread running again:
     * x86 chain: the x86 stack scanned from Esp for values inside an x86 image that follow a call instruction (the
       bytes before them, read from the target process); up to 24. A scan, as xikit's sample=1 is: a return address
       left in a dead slot of an older frame is found too;
     * native chain (ARM64 only): the frame-pointer chain from Fp (x29), which the Windows ARM64 ABI has every
       non-leaf function keep: [fp] = the caller's fp, [fp+8] = the return address; up to 24, while each fp is in the
       stack read and above the one before. Lr is recorded on its own (a leaf has not saved it). On x64 the native
       chain is left empty: x64 code keeps no frame-pointer chain to follow without the unwind tables.
   Samples go into a histogram keyed by the whole tuple (x86 eip, x86 chain, native pc, native lr, native chain);
   every window_s seconds it is written to <outdir>\xikit_native_<pid>.csv (appended; read.py reads it):
     w,<n>,<local start>,<local end>,<samples>,<missed>,<x86 ctx failed>,<native ctx failed>,<dropped>
     m,<base>,<size>,<machine>,<path>                 every image in the process (VirtualQueryEx, PE header)
     s,<count>,<eip>,<x86 chain a;b;..>,<pc>,<lr>,<native chain a;b;..>
     a,<address>,<module>,<rva>,<export>,<offset>,<alloc base>,<type>,<protect>
   An 'a' line for each address the window used: its image and RVA, the nearest export at or below it (a position,
   not a claim the code belongs to that export), or, outside any image, its allocation and memory type. Addresses
   are as the process had them; read.py turns FFXiMain.dll addresses into the analysed build's with <rel> and the
   shift table, which xikit passes on and this writes in the header. It stops when the process ends.

   Stutters (frame=<address> spike=<ms>, which xikit passes with sample=2 and spike_ms): <address> is xikit's frame
   counter (xikit.c g_frame) in the game's process, read with each sample while the thread is suspended. xikit adds 1
   to it in its Present hook right after taking the time that ends a frame, so a sample that reads v fell in the frame
   xikit_spikes_<pid>.csv numbers v + 1 (from the time Present v was entered to the time Present v + 1 was). When the
   value changes, a frame whose samples spanned spike ms or more (this process's clock, from its first sample to the
   first of the next frame: within one sample interval of the frame) is written to <outdir>\xikit_stutters_<pid>.csv,
   in the same form as a window, its w line numbering the frame:
     w,<frame>,<local start>,<local end>,<samples>,<missed>,<x86 ctx failed>,<native ctx failed>,<dropped>,<ms>
   m lines only when the process's images changed since the last block, a lines only for addresses no earlier
   block labelled (read.py carries both forward). Writing a block takes the sampler's time (the image table, the
   exports): no samples are taken meanwhile, and the next frame's span starts after it, so a frame right after a
   stutter can be missed or shortened. A window's write (every window_s) is left out of the frame in progress the same
   way, and the window's span starts after it. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <psapi.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAIN 24
#define X86STACK 16384
#define NSTACK 65536
#define HN 65536                                 /* distinct tuples per window */

typedef struct { uint32_t eip; uint8_t nx; uint8_t nn; uint32_t x[CHAIN]; uint64_t pc, lr, n[CHAIN]; uint32_t count; uint32_t used; } Ent;
typedef struct { Ent *h; uint32_t size, n, dropped; } Tab;      /* size: a power of two */
static Tab g_w, g_f;                             /* the window's; the current frame's (stutters) */

/* ---------------------------------------------------------------- the process's images (VirtualQueryEx) */
typedef struct { uint64_t base, size; uint16_t machine; char path[MAX_PATH]; uint32_t nexp; uint32_t *erva; char **ename; int exp_read; } Mod;
static Mod g_mod[1024]; static int g_nmod; static HANDLE g_proc, g_thr;
static int rd(uint64_t a, void *buf, size_t n) { SIZE_T got = 0; return ReadProcessMemory(g_proc, (LPCVOID)(uintptr_t)a, buf, n, &got) && got == n; }
static Mod g_old[1024];
static void scan_mods(void) {                    /* an image seen before keeps its exports, read once */
  if (WaitForSingleObject(g_proc, 0) == WAIT_OBJECT_0) return;   /* gone: the last table stands */
  int nold = g_nmod; memcpy(g_old, g_mod, sizeof(Mod) * nold); g_nmod = 0;
  uint64_t a = 0;
  for (;;) {
    MEMORY_BASIC_INFORMATION mb;
    if (!VirtualQueryEx(g_proc, (LPCVOID)(uintptr_t)a, &mb, sizeof mb)) break;
    uint64_t base = (uint64_t)(uintptr_t)mb.AllocationBase, next = (uint64_t)(uintptr_t)mb.BaseAddress + mb.RegionSize;
    if (mb.State == MEM_COMMIT && mb.Type == MEM_IMAGE && base == (uint64_t)(uintptr_t)mb.BaseAddress && g_nmod < 1024) {
      IMAGE_DOS_HEADER d; uint8_t nt[0x60];
      if (rd(base, &d, sizeof d) && d.e_magic == IMAGE_DOS_SIGNATURE && rd(base + d.e_lfanew, nt, sizeof nt) && !memcmp(nt, "PE\0\0", 4)) {
        Mod *m = &g_mod[g_nmod]; memset(m, 0, sizeof *m);
        m->base = base; memcpy(&m->machine, nt + 4, 2);
        uint32_t soi; memcpy(&soi, nt + 24 + 56, 4); m->size = soi;   /* SizeOfImage: the same offset in PE32 and PE32+ */
        if (!K32GetMappedFileNameA(g_proc, (LPVOID)(uintptr_t)base, m->path, sizeof m->path)) snprintf(m->path, sizeof m->path, "?");
        for (int i = 0; i < nold; i++)
          if (g_old[i].base == m->base && g_old[i].size == m->size && !strcmp(g_old[i].path, m->path)) { *m = g_old[i]; g_old[i].exp_read = 2; break; }
        g_nmod++;
      }
    }
    if (next <= a) break;
    a = next;
  }
  for (int i = 0; i < nold; i++) if (g_old[i].exp_read != 2) {
    for (uint32_t k = 0; k < g_old[i].nexp; k++) free(g_old[i].ename[k]);
    free(g_old[i].ename); free(g_old[i].erva);
  }
}
static Mod *mod_of(uint64_t a) { for (int i = 0; i < g_nmod; i++) if (a - g_mod[i].base < g_mod[i].size) return &g_mod[i]; return NULL; }
static void read_exports(Mod *m) {               /* the export directory, read from the process: (rva, name) sorted */
  m->exp_read = 1;
  IMAGE_DOS_HEADER d; uint32_t sig; IMAGE_FILE_HEADER fh; uint8_t opt[240];
  if (!rd(m->base, &d, sizeof d) || !rd(m->base + d.e_lfanew, &sig, 4) || !rd(m->base + d.e_lfanew + 4, &fh, sizeof fh)) return;
  if (!rd(m->base + d.e_lfanew + 24, opt, fh.SizeOfOptionalHeader < sizeof opt ? fh.SizeOfOptionalHeader : sizeof opt)) return;
  uint16_t magic; memcpy(&magic, opt, 2);
  int dd = magic == 0x20B ? 112 : 96; uint32_t erva, esz;
  memcpy(&erva, opt + dd, 4); memcpy(&esz, opt + dd + 4, 4);
  if (!erva || !esz) return;
  IMAGE_EXPORT_DIRECTORY ed; if (!rd(m->base + erva, &ed, sizeof ed)) return;
  uint32_t nn = ed.NumberOfNames, nf = ed.NumberOfFunctions; if (!nn || nn > 65536 || nf > 65536) return;
  uint32_t *fns = malloc(nf * 4), *names = malloc(nn * 4); uint16_t *ord = malloc(nn * 2);
  if (fns && names && ord && rd(m->base + ed.AddressOfFunctions, fns, nf * 4) && rd(m->base + ed.AddressOfNames, names, nn * 4) && rd(m->base + ed.AddressOfNameOrdinals, ord, nn * 2)) {
    m->erva = malloc(nn * 4); m->ename = calloc(nn, sizeof(char *));
    for (uint32_t i = 0; i < nn && m->erva; i++) {
      if (ord[i] >= nf) continue;
      uint32_t r = fns[ord[i]]; if (r >= erva && r < erva + esz) continue;   /* a forwarder, not code here */
      char nm[256] = ""; if (!rd(m->base + names[i], nm, sizeof nm - 1)) { /* the name may end near the image's end */
        for (int k = 0; k < 255 && rd(m->base + names[i] + k, nm + k, 1) && nm[k]; k++); }
      nm[255] = 0; m->erva[m->nexp] = r; m->ename[m->nexp] = _strdup(nm); m->nexp++;
    }
    for (uint32_t i = 1; i < m->nexp; i++)         /* insertion sort by rva (export tables are small) */
      for (uint32_t j = i; j > 0 && m->erva[j - 1] > m->erva[j]; j--) {
        uint32_t t = m->erva[j]; m->erva[j] = m->erva[j - 1]; m->erva[j - 1] = t;
        char *s = m->ename[j]; m->ename[j] = m->ename[j - 1]; m->ename[j - 1] = s;
      }
  }
  free(fns); free(names); free(ord);
}

/* ---------------------------------------------------------------- the x86 call check */
#define PC 4096                                   /* pages of x86 code read this window */
static uint32_t g_pk[PC]; static uint8_t g_pv[PC][4096]; static uint8_t g_pok[PC];
static const uint8_t *page(uint32_t a) {
  uint32_t p = a & ~0xFFFu, i = (p >> 12) * 2654435761u % PC;
  for (int k = 0; k < 8; k++, i = (i + 1) % PC) {
    if (g_pk[i] == p) return g_pok[i] ? g_pv[i] : NULL;
    if (!g_pk[i]) { g_pk[i] = p; g_pok[i] = rd(p, g_pv[i], 4096); return g_pok[i] ? g_pv[i] : NULL; }
  }
  return NULL;
}
static int byte_at(uint32_t a, uint8_t *b) { const uint8_t *p = page(a); if (!p) return 0; *b = p[a & 0xFFF]; return 1; }
static int after_call(uint32_t r) {               /* the same encodings as xikit.c's after_call_quiet */
  uint8_t p[8];                                   /* p[k] = byte at r - 8 + k; p[8 - n] = r[-n] */
  for (int k = 0; k < 8; k++) if (!byte_at(r - 8 + k, &p[k])) return 0;
#define B(n) p[8 - (n)]
  if (B(5) == 0xE8) return 1;
  if (B(2) == 0xFF && ((B(1) >> 3) & 7) == 2 && (B(1) >> 6) == 3) return 1;
  if (B(2) == 0xFF && ((B(1) >> 3) & 7) == 2 && (B(1) >> 6) == 0 && (B(1) & 7) != 4 && (B(1) & 7) != 5) return 1;
  if (B(3) == 0xFF && ((B(2) >> 3) & 7) == 2 && (B(2) >> 6) == 1) return 1;
  if (B(6) == 0xFF && ((B(5) >> 3) & 7) == 2 && ((B(5) >> 6) == 2 || (B(5) & 0xC7) == 5)) return 1;
  if (B(4) == 0xFF && ((B(3) >> 3) & 7) == 2 && (B(3) >> 6) == 1 && (B(3) & 7) == 4) return 1;
  if (B(7) == 0xFF && ((B(6) >> 3) & 7) == 2 && (B(6) >> 6) == 2 && (B(6) & 7) == 4) return 1;
#undef B
  return 0;
}
static int in_x86_image(uint32_t a) { Mod *m = mod_of(a); return m && m->machine == IMAGE_FILE_MACHINE_I386; }

/* ---------------------------------------------------------------- the histogram */
static uint64_t mix(uint64_t h, uint64_t v) { h ^= v + 0x9E3779B97F4A7C15ull + (h << 6) + (h >> 2); return h; }
static void add(Tab *t, const Ent *e) {
  uint64_t h = mix(mix(mix(e->eip, e->pc), e->lr), e->nx * 64 + e->nn);
  for (int i = 0; i < e->nx; i++) h = mix(h, e->x[i]);
  for (int i = 0; i < e->nn; i++) h = mix(h, e->n[i]);
  uint32_t m = t->size - 1, i = (uint32_t)h & m;
  for (uint32_t k = 0; k < t->size; k++, i = (i + 1) & m) {
    Ent *s = &t->h[i];
    if (!s->used) { if (t->n >= t->size * 3 / 4) break; *s = *e; s->used = 1; s->count = 1; t->n++; return; }
    if (s->eip == e->eip && s->pc == e->pc && s->lr == e->lr && s->nx == e->nx && s->nn == e->nn &&
        !memcmp(s->x, e->x, e->nx * 4) && !memcmp(s->n, e->n, e->nn * 8)) { s->count++; return; }
  }
  t->dropped++;
}
static void clear(Tab *t) { memset(t->h, 0, sizeof(Ent) * t->size); t->n = 0; t->dropped = 0; }

/* ---------------------------------------------------------------- the addresses' labels */
static uint64_t *g_addr; static uint32_t g_naddr, g_caddr;
static void note(uint64_t a) { if (g_naddr == g_caddr) { g_caddr = g_caddr ? g_caddr * 2 : 65536; g_addr = realloc(g_addr, g_caddr * 8); } g_addr[g_naddr++] = a; }
static int cmpu64(const void *x, const void *y) { uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y; return a < b ? -1 : a > b; }
static void label(FILE *f, uint64_t a) {
  Mod *m = mod_of(a);
  if (m) {
    if (!m->exp_read) read_exports(m);
    uint32_t rva = (uint32_t)(a - m->base); const char *b = strrchr(m->path, '\\'); b = b ? b + 1 : m->path;
    int lo = 0, hi = (int)m->nexp - 1, best = -1;
    while (lo <= hi) { int mid = (lo + hi) / 2; if (m->erva[mid] <= rva) { best = mid; lo = mid + 1; } else hi = mid - 1; }
    if (best >= 0) fprintf(f, "a,0x%llX,%s,0x%X,%s,0x%X,,,\n", (unsigned long long)a, b, rva, m->ename[best], rva - m->erva[best]);
    else fprintf(f, "a,0x%llX,%s,0x%X,,,,,\n", (unsigned long long)a, b, rva);
    return;
  }
  MEMORY_BASIC_INFORMATION mb;
  if (a && VirtualQueryEx(g_proc, (LPCVOID)(uintptr_t)a, &mb, sizeof mb))
    fprintf(f, "a,0x%llX,,,,,0x%llX,%s,0x%lX\n", (unsigned long long)a, (unsigned long long)(uintptr_t)mb.AllocationBase,
            mb.State != MEM_COMMIT ? "uncommitted" : mb.Type == MEM_PRIVATE ? "private" : mb.Type == MEM_MAPPED ? "mapped" : "image", (unsigned long)mb.Protect);
  else fprintf(f, "a,0x%llX,,,,,,unknown,\n", (unsigned long long)a);
}

/* ---------------------------------------------------------------- the files */
static void write_mods(FILE *f) {
  for (int i = 0; i < g_nmod; i++) fprintf(f, "m,0x%llX,0x%llX,0x%X,%s\n", (unsigned long long)g_mod[i].base, (unsigned long long)g_mod[i].size, g_mod[i].machine, g_mod[i].path);
}
static uint64_t *g_seen; static uint32_t g_nseen;   /* the stutter file's labelled addresses (open addressing, 2^20) */
#define SEENN (1u << 20)
static int seen_before(uint64_t a) {             /* 1: labelled already (or the set is full: not labelled again) */
  uint64_t k = a + 1; uint32_t i = (uint32_t)(k * 0x9E3779B97F4A7C15ull >> 44) & (SEENN - 1);
  for (;; i = (i + 1) & (SEENN - 1)) {
    if (g_seen[i] == k) return 1;
    if (!g_seen[i]) { if (g_nseen >= SEENN * 3 / 4) return 1; g_seen[i] = k; g_nseen++; return 0; }
  }
}
static void write_rows(FILE *f, Tab *t, int once) {   /* once: a lines only for addresses not labelled before */
  g_naddr = 0;
  for (uint32_t i = 0; i < t->size; i++) {
    Ent *e = &t->h[i]; if (!e->used) continue;
    fprintf(f, "s,%u,0x%X,", e->count, e->eip); note(e->eip);
    for (int k = 0; k < e->nx; k++) { fprintf(f, k ? ";0x%X" : "0x%X", e->x[k]); note(e->x[k]); }
    fprintf(f, ",0x%llX,0x%llX,", (unsigned long long)e->pc, (unsigned long long)e->lr); note(e->pc); note(e->lr);
    for (int k = 0; k < e->nn; k++) { fprintf(f, k ? ";0x%llX" : "0x%llX", (unsigned long long)e->n[k]); note(e->n[k]); }
    fputc('\n', f);
  }
  qsort(g_addr, g_naddr, 8, cmpu64);
  for (uint32_t i = 0; i < g_naddr; i++) if ((!i || g_addr[i] != g_addr[i - 1]) && !(once && seen_before(g_addr[i]))) label(f, g_addr[i]);
}

static char g_path[MAX_PATH]; static SYSTEMTIME g_t0; static long g_samples, g_missed, g_xfail, g_nfail; static int g_win;
static void stamp(char *o, size_t n, const SYSTEMTIME *t) { snprintf(o, n, "%04u-%02u-%02u %02u:%02u:%02u.%03u", t->wYear, t->wMonth, t->wDay, t->wHour, t->wMinute, t->wSecond, t->wMilliseconds); }
static void flush_window(void) {
  SYSTEMTIME t1; GetLocalTime(&t1); char s0[40], s1[40]; stamp(s0, sizeof s0, &g_t0); stamp(s1, sizeof s1, &t1);
  scan_mods();
  FILE *f = fopen(g_path, "a");
  if (f) {
    fprintf(f, "w,%d,%s,%s,%ld,%ld,%ld,%ld,%u\n", g_win, s0, s1, g_samples, g_missed, g_xfail, g_nfail, g_w.dropped);
    write_mods(f);
    write_rows(f, &g_w, 0);
    fclose(f);
  }
  clear(&g_w);
  memset(g_pk, 0, sizeof g_pk);                   /* code can be patched (hooks): read the pages again */
  g_samples = g_missed = g_xfail = g_nfail = 0; g_win++; g_t0 = t1;
}

static char g_spath[MAX_PATH]; static uint32_t g_faddr; static int g_spike_ms;
static uint64_t g_lastmods;                      /* a hash of the image table last written to the stutter file */
static long g_fs, g_fmiss, g_fxf, g_fnf; static SYSTEMTIME g_ft0;
static int flush_frame(uint32_t v, double fms) {    /* the frame whose samples read v: frame v + 1 (see above); 1: written */
  SYSTEMTIME t1; GetLocalTime(&t1); int wrote = 0;
  if (fms >= g_spike_ms && g_fs) {
    wrote = 1;
    char s0[40], s1[40]; stamp(s0, sizeof s0, &g_ft0); stamp(s1, sizeof s1, &t1);
    scan_mods();
    uint64_t mh = 0; for (int i = 0; i < g_nmod; i++) mh = mix(mix(mh, g_mod[i].base), g_mod[i].size);
    FILE *f = fopen(g_spath, "a");
    if (f) {
      fprintf(f, "w,%u,%s,%s,%ld,%ld,%ld,%ld,%u,%.1f\n", v + 1, s0, s1, g_fs, g_fmiss, g_fxf, g_fnf, g_f.dropped, fms);
      if (mh != g_lastmods) { write_mods(f); g_lastmods = mh; }
      write_rows(f, &g_f, 1);
      fclose(f);
    }
  }
  clear(&g_f); g_fs = g_fmiss = g_fxf = g_fnf = 0;
  if (wrote) GetLocalTime(&t1);
  g_ft0 = t1;
  return wrote;
}

typedef BOOL (WINAPI *W64GTC)(HANDLE, PWOW64_CONTEXT);
typedef LONG (NTAPI *NQIT)(HANDLE, ULONG, PVOID, ULONG, PULONG);

int main(int argc, char **argv) {
  if (argc < 6) { fprintf(stderr, "usage: xisamp <pid> <tid> <interval_ms> <window_s> <outdir> [<rel> [<lo:hi:shift>...]]\n"); return 2; }
  DWORD pid = strtoul(argv[1], 0, 0), tid = strtoul(argv[2], 0, 0); int ms = atoi(argv[3]), win_s = atoi(argv[4]);
  if (ms < 1) ms = 1; if (win_s < 1) win_s = 30;
  g_proc = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION | SYNCHRONIZE, FALSE, pid);
  g_thr = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION, FALSE, tid);
  snprintf(g_path, sizeof g_path, "%s\\xikit_native_%lu.csv", argv[5], (unsigned long)pid);
  FILE *f = fopen(g_path, "a");
  if (!f) return 3;
  USHORT pm = 0, nm = 0;
  typedef BOOL (WINAPI *IW2)(HANDLE, USHORT *, USHORT *);
  IW2 iw2 = (IW2)(void *)GetProcAddress(GetModuleHandleA("kernel32.dll"), "IsWow64Process2");
  if (iw2) iw2(g_proc ? g_proc : GetCurrentProcess(), &pm, &nm);
  fprintf(f, "# xisamp: pid %lu tid %lu every %d ms, a window every %d s; this build for %s; process machine 0x%X, native 0x%X; open process %s, thread %s\n",
          (unsigned long)pid, (unsigned long)tid, ms, win_s,
#if defined(__aarch64__)
          "ARM64",
#else
          "x64",
#endif
          pm, nm, g_proc ? "ok" : "failed", g_thr ? "ok" : "failed");
  fprintf(f, "# rel %s; shift", argc > 6 ? argv[6] : "0");
  for (int i = 7; i < argc; i++) {
    if (!strncmp(argv[i], "frame=", 6)) g_faddr = strtoul(argv[i] + 6, 0, 16);
    else if (!strncmp(argv[i], "spike=", 6)) g_spike_ms = atoi(argv[i] + 6);
    else fprintf(f, " %s", argv[i]);
  }
  fprintf(f, "\n# w,n,start,end,samples,missed,x86_ctx_failed,native_ctx_failed,dropped | m,base,size,machine,path | s,count,eip,x86_chain,pc,lr,native_chain | a,address,module,rva,export,offset,alloc_base,type,protect\n");
  int stut = g_faddr && g_spike_ms > 0;
  if (stut) fprintf(f, "# stutters: frame counter at 0x%X, frames of %d ms or more to xikit_stutters_%lu.csv\n", g_faddr, g_spike_ms, (unsigned long)pid);
  fclose(f);
  if (!g_proc || !g_thr) return 4;
  g_w.size = HN; g_w.h = calloc(HN, sizeof(Ent)); if (!g_w.h) return 5;
  if (stut) {
    snprintf(g_spath, sizeof g_spath, "%s\\xikit_stutters_%lu.csv", argv[5], (unsigned long)pid);
    g_f.size = 8192; g_f.h = calloc(g_f.size, sizeof(Ent)); g_seen = calloc(SEENN, 8);
    if (!g_f.h || !g_seen) stut = 0;
    else if ((f = fopen(g_spath, "a"))) {
      fprintf(f, "# xisamp stutters: pid %lu tid %lu every %d ms; frames of %d ms or more; w,frame,start,end,samples,missed,x86_ctx_failed,native_ctx_failed,dropped,ms (frame as in xikit_spikes)\n",
              (unsigned long)pid, (unsigned long)tid, ms, g_spike_ms);
      fprintf(f, "# rel %s; shift", argc > 6 ? argv[6] : "0");
      for (int i = 7; i < argc; i++) if (strncmp(argv[i], "frame=", 6) && strncmp(argv[i], "spike=", 6)) fprintf(f, " %s", argv[i]);
      fputc('\n', f); fclose(f);
    }
  }
  LARGE_INTEGER qf, qn; QueryPerformanceFrequency(&qf);
  uint32_t fv = 0; int fv_ok = 0; LARGE_INTEGER fq0; fq0.QuadPart = 0; GetLocalTime(&g_ft0);
  W64GTC w64gtc = (W64GTC)(void *)GetProcAddress(GetModuleHandleA("kernel32.dll"), "Wow64GetThreadContext");
  NQIT nqit = (NQIT)(void *)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationThread");
  static uint8_t xs[X86STACK], ns[NSTACK];
  timeBeginPeriod(1);
  SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
  scan_mods(); GetLocalTime(&g_t0);
  uint64_t xlo = 0, xhi = 0, nlo = 0, nhi = 0;    /* the stacks' committed regions, queried when Esp/Sp leaves them */
  DWORD tw = GetTickCount();
  for (;;) {
    Sleep(ms);
    if (WaitForSingleObject(g_proc, 0) == WAIT_OBJECT_0) break;
    if (GetTickCount() - tw >= (DWORD)win_s * 1000) {   /* the write's time is left out of the frame in progress too */
      LARGE_INTEGER a0, a1; QueryPerformanceCounter(&a0); flush_window(); tw = GetTickCount();
      QueryPerformanceCounter(&a1); fq0.QuadPart += a1.QuadPart - a0.QuadPart;
    }
    Ent e; memset(&e, 0, sizeof e);
    WOW64_CONTEXT wc; CONTEXT nc; int xok = 0, nok = 0; uint32_t xn = 0; uint32_t nn = 0; uint64_t sp = 0, fp = 0;
    if (SuspendThread(g_thr) == (DWORD)-1) { g_missed++; g_fmiss++; continue; }
    memset(&wc, 0, sizeof wc); wc.ContextFlags = WOW64_CONTEXT_CONTROL | WOW64_CONTEXT_INTEGER;
    xok = w64gtc && w64gtc(g_thr, &wc);
    if (!xok && nqit) { memset(&wc, 0, sizeof wc); wc.ContextFlags = WOW64_CONTEXT_CONTROL | WOW64_CONTEXT_INTEGER; xok = nqit(g_thr, 29 /* ThreadWow64Context */, &wc, sizeof wc, NULL) >= 0; }
    memset(&nc, 0, sizeof nc); nc.ContextFlags = CONTEXT_CONTROL | CONTEXT_INTEGER;
    nok = GetThreadContext(g_thr, &nc);
    uint32_t v = 0; int vok = stut && rd(g_faddr, &v, 4);   /* the frame this sample is in, with the thread stopped */
    if (xok) {
      uint64_t esp = wc.Esp;
      if (esp < xlo || esp >= xhi) { MEMORY_BASIC_INFORMATION mb; xlo = xhi = 0;
        if (VirtualQueryEx(g_proc, (LPCVOID)(uintptr_t)esp, &mb, sizeof mb) && mb.State == MEM_COMMIT) { xlo = (uint64_t)(uintptr_t)mb.BaseAddress; xhi = xlo + mb.RegionSize; } }
      if (esp >= xlo && esp < xhi) { xn = (uint32_t)(xhi - esp < X86STACK ? xhi - esp : X86STACK); if (!rd(esp, xs, xn)) xn = 0; }
    }
    if (nok) {
#if defined(__aarch64__)
      e.pc = nc.Pc; e.lr = nc.Lr; sp = nc.Sp; fp = nc.Fp;
#else
      e.pc = nc.Rip; sp = nc.Rsp; fp = 0;
#endif
      if (sp < nlo || sp >= nhi) { MEMORY_BASIC_INFORMATION mb; nlo = nhi = 0;
        if (VirtualQueryEx(g_proc, (LPCVOID)(uintptr_t)sp, &mb, sizeof mb) && mb.State == MEM_COMMIT) { nlo = (uint64_t)(uintptr_t)mb.BaseAddress; nhi = nlo + mb.RegionSize; } }
      if (fp && sp >= nlo && sp < nhi) { nn = (uint32_t)(nhi - sp < NSTACK ? nhi - sp : NSTACK); if (!rd(sp, ns, nn)) nn = 0; }
    }
    ResumeThread(g_thr);
    if (vok) {
      QueryPerformanceCounter(&qn);
      if (!fv_ok) { fv = v; fv_ok = 1; fq0 = qn; clear(&g_f); g_fs = g_fmiss = g_fxf = g_fnf = 0; GetLocalTime(&g_ft0); }
      else if (v != fv) {     /* the next frame's time starts after the write: the sampler took no samples during it */
        int w = flush_frame(fv, (double)(qn.QuadPart - fq0.QuadPart) * 1000.0 / (double)qf.QuadPart); fv = v;
        if (w) QueryPerformanceCounter(&qn);
        fq0 = qn;
      }
    }
    if (!xok) { g_xfail++; g_fxf++; }
    if (!nok) { g_nfail++; g_fnf++; }
    if (!xok && !nok) { g_missed++; g_fmiss++; continue; }
    if (xok) {
      e.eip = wc.Eip;
      for (uint32_t o = 0; o + 4 <= xn && e.nx < CHAIN; o += 4) {
        uint32_t v; memcpy(&v, xs + o, 4);
        if (in_x86_image(v) && after_call(v)) e.x[e.nx++] = v;
      }
    }
    for (uint64_t prev = sp; nn && e.nn < CHAIN && fp >= prev && fp + 16 <= sp + nn && !(fp & 7);) {   /* x29 chain */
      uint64_t up, ra; memcpy(&up, ns + (fp - sp), 8); memcpy(&ra, ns + (fp - sp) + 8, 8);
      e.n[e.nn++] = ra; prev = fp + 16;
      if (up <= fp) break;
      fp = up;
    }
    add(&g_w, &e); g_samples++;
    if (vok) { add(&g_f, &e); g_fs++; }
  }
  if (fv_ok) { QueryPerformanceCounter(&qn); flush_frame(fv, (double)(qn.QuadPart - fq0.QuadPart) * 1000.0 / (double)qf.QuadPart); }
  flush_window();
  return 0;
}
