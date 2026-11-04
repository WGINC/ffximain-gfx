/* measure.c -- the kit's measurement session (docs/KIT.md, "Measuring"), #included by xikit.c (it uses the kit's
   state: g_out, g_rel, g_frame, g_render_thread, live_of, klog, ...). Everything here only reads the game, except the
   two kinds of call timing, which redirect calls and change nothing else:

   frames=1     xikit_frames_<pid>.csv, one line per frame (Present to Present, on the render thread): the frame's
                time, the time inside the device's Present, the kit's own time, the render thread's CPU cycles and
                time, the process's page faults, the mark (Ctrl+Alt+F7 adds one), and the game's own pacing fields
                read raw: Frame_Render's `this` = [0x104568FC] (docs/RENDERER.md 3, from the code), its dwords +0x28,
                +0x2C (the step and this frame's ticks), +0x30 (the minimum ticks), +0x38, +0x3C (whole ticks and
                the carried fraction), the ring 0x10456964..0x10456973, and the timer object's [[this+0x1C]+4] (the
                last measured rate). They are written as hex dwords: the reader (kit/xikit/measure_read.py) decodes
                them as RENDERER.md 3 says each is used.
   imports=1|2  every import slot of FFXiMain.dll listed in xikit_measure.txt (make_measure.py: the game's import
                table) whose function is in the list below is wrapped: each call is timed (inclusive, and exclusive
                of the wrapped calls made inside it), per thread kind (render thread or other), per call site (its
                return address), per frame on the render thread. 2 adds the hot ones (heap, critical sections,
                string and locale calls, IsBad*Ptr).
   callsites=N  every direct call listed in xikit_measure.txt at level N-1 or below (callsites=1: level 0, the main
                loop and App_RenderFrameMaskedFPU; 2: also level 1, Frame_Render and the game mode's update; 3, 4:
                the functions those call) gets its rel32 pointed at a timing stub, which calls the same function. A site whose live
                bytes are not that call to that function is left alone (logged). A site called more than
                callsite_max times in a frame is put back at the next window (its cost would distort the frame).
   sample=3     every thread of the process: where it is (EIP) and the return addresses on its stack, raw, with the
                QPC time and the kit's frame number, into xikit_raw_<pid>.bin (measure_read.py reads it): the render
                thread every sample_ms, the others every sample_other_ms when they used CPU since the last look.
   sysstats=1   xikit_sys_<pid>.csv each second (the process's and the machine's CPU time, memory, page faults, I/O)
                and xikit_threads_<pid>.csv every 2 s (each thread's start address, CPU cycles and times).
   env=1        (default) xikit_env_<pid>.txt: the game's settings in the registry, the display, timers (resolution,
                Sleep(1) as measured, timeGetTime's step), the cost of common calls (measured here), Wine if this is
                Wine, the modules, the environment; copies of dgVoodoo.conf, d3d8on11.ini, speedfix.ini, xikit.ini.

   The call timing's mechanism: a stub per wrapped function (push k; jmp m_enter). m_enter saves the caller's return
   address in the thread's own shadow stack (with the stack slot it came from), puts m_land in its place and jumps to
   the function, with every register as the caller left it. m_land takes the entry back (the one whose slot is the
   first below the stack pointer the function returned with: entries left by a call that never returned, unwound by an
   exception, are dropped), records the time and returns to the caller with EAX, EDX and every other register as the
   function left them. The thread's last-error value is kept as the function left it. Time is the TSC (rdtsc),
   converted with its rate against QueryPerformanceCounter, measured over the session (#cal lines). */

#include <psapi.h>

#define MP_MAX 4096
#define MP_HASH 32768
#define MP_DEPTH 192
enum { G_SITE, G_WAIT, G_FILE, G_MSG, G_INPUT, G_GDI, G_NET, G_TIME, G_HEAP, G_SYNC, G_MEM, G_OTHER, G_N };
static const char *const m_gname[G_N] = {"site", "wait", "file", "msg", "input", "gdi", "net", "time", "heap", "sync", "mem", "other"};

typedef struct {
  uint32_t orig;                    /* what the call reaches: the function */
  uint32_t at;                      /* the import slot or the call site (live address) */
  uint32_t ea, tea;                 /* analysed: slot/site, and the site's target */
  uint8_t group, level, kind, on, want;   /* kind: 'I' import, 'S' site; want: what m_apply_probes makes it */
  char name[64];
  uint8_t stub[12];
  /* the render thread's sums for the frame being measured (written by that thread only) */
  uint32_t f_calls; uint64_t f_incl, f_excl, f_max;
  uint32_t w_maxcalls;              /* most calls in one frame this window (callsite_max) */
} Probe;
static Probe *m_p; static int m_np;
static uint8_t *m_stubs;
static int m_frames, m_imports, m_callsites, m_sysstats, m_env = 1, m_callsite_max = 500, m_rawsample;
static volatile LONG m_on;          /* call timing records (the stubs always pass the call on) */
static DWORD m_tls = TLS_OUT_OF_INDEXES;
static volatile LONG m_stale, m_nomatch, m_deep; static double m_cost_ns = -1;

typedef struct { uint32_t ret, slot; uint64_t t0, child; int k; } SEnt;
typedef struct { DWORD tid; int depth, render; SEnt e[MP_DEPTH]; } SThr;
static SThr *m_thr[1024]; static volatile LONG m_nthr;

/* window totals since the start, per (probe, thread kind, return address) */
typedef struct { uint32_t key, ra; uint64_t calls, incl, excl, max; } HEnt;
static HEnt *m_h; static volatile LONG m_hlock, m_hfull;

static inline uint64_t m_tsc(void) { uint32_t lo, hi; __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi)); return ((uint64_t)hi << 32) | lo; }
static inline uint32_t m_get_le(void) { uint32_t v; __asm__ __volatile__("movl %%fs:0x34, %0" : "=r"(v)); return v; }
static inline void m_set_le(uint32_t v) { __asm__ __volatile__("movl %0, %%fs:0x34" : : "r"(v) : "memory"); }

/* TSC rate: pairs of (QPC, TSC) at the start and at each report */
static LARGE_INTEGER m_q0; static uint64_t m_t0; static double m_tsc_per_ms = 0;
static void m_cal(void) {
  LARGE_INTEGER q; QueryPerformanceCounter(&q); uint64_t t = m_tsc();
  double ms = (double)(q.QuadPart - m_q0.QuadPart) * 1000.0 / (double)g_qpf.QuadPart;
  if (ms > 50) m_tsc_per_ms = (double)(t - m_t0) / ms;
}
static double m_ms(uint64_t ticks) { return m_tsc_per_ms > 0 ? (double)ticks / m_tsc_per_ms : 0; }
static double m_qms(LARGE_INTEGER a, LARGE_INTEGER b) { return (double)(b.QuadPart - a.QuadPart) * 1000.0 / (double)g_qpf.QuadPart; }

/* ------------------------------------------------------------------------------------------------ buffered output
   Each stream: the producer appends under its lock; the writer thread swaps the buffer out every 250 ms and writes
   it. A full buffer drops what does not fit (counted, logged at the end of each window). */
typedef struct { CRITICAL_SECTION cs; char *b, *spare; size_t n, cap; FILE *f; volatile LONG dropped; char path[MAX_PATH]; } MStream;
static MStream m_sframes, m_sprobe, m_sraw, m_ssys, m_sthreads;
static int ms_open(MStream *s, const char *name, size_t cap, const char *head) {
  InitializeCriticalSection(&s->cs);
  snprintf(s->path, sizeof s->path, "%s\\%s_%lu.%s", g_out, name, (unsigned long)GetCurrentProcessId(), strcmp(name, "xikit_raw") ? "csv" : "bin");
  s->f = fopen(s->path, "wb"); if (!s->f) { klog("measure: could not write %s", s->path); return 0; }
  s->b = (char *)VirtualAlloc(NULL, cap, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  s->spare = (char *)VirtualAlloc(NULL, cap, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  if (!s->b || !s->spare) { fclose(s->f); s->f = NULL; return 0; }
  s->cap = cap;
  if (head) { fputs(head, s->f); fflush(s->f); }
  return 1;
}
static void ms_put(MStream *s, const void *p, size_t n) {
  if (!s->f) return;
  EnterCriticalSection(&s->cs);
  if (s->n + n <= s->cap) { memcpy(s->b + s->n, p, n); s->n += n; } else InterlockedIncrement(&s->dropped);
  LeaveCriticalSection(&s->cs);
}
static void ms_printf(MStream *s, const char *fmt, ...) {
  char buf[4096]; va_list a; va_start(a, fmt); int n = vsnprintf(buf, sizeof buf, fmt, a); va_end(a);
  if (n > 0) ms_put(s, buf, (size_t)n < sizeof buf ? (size_t)n : sizeof buf - 1);
}
static void ms_flush(MStream *s) {
  if (!s->f) return;
  EnterCriticalSection(&s->cs); char *b = s->b; size_t n = s->n; s->b = s->spare; s->spare = b; s->n = 0; LeaveCriticalSection(&s->cs);
  if (n) { fwrite(b, 1, n, s->f); fflush(s->f); }
}
static uint64_t m_raw_bytes, m_raw_max;
static volatile LONG m_dump_wanted;
static void m_dump_probes(void);
static DWORD WINAPI m_writer(LPVOID u) {
  (void)u;
  for (;;) {
    Sleep(250);
    ms_flush(&m_sframes); ms_flush(&m_sprobe); ms_flush(&m_ssys); ms_flush(&m_sthreads);
    size_t before = m_sraw.n; ms_flush(&m_sraw); m_raw_bytes += before;
    if (m_dump_wanted) { m_dump_wanted = 0; m_dump_probes(); }
  }
  return 0;
}

/* ------------------------------------------------------------------------------------------------ the stubs */
uint32_t m_enter_c(int k, uint32_t *slot) __attribute__((used));
uint32_t m_land_c(uint32_t cur) __attribute__((used));
void m_land(void);
void m_enter(void);
__asm__(
  ".text\n"
  ".globl _m_enter\n"
  "_m_enter:\n"                     /* [esp] = k (the stub's push), [esp+4] = the caller's return address */
  "  pushal\n"                      /* [esp+32] = k, [esp+36] = the return address */
  "  leal 36(%esp), %eax\n"
  "  pushl %eax\n"
  "  pushl 36(%esp)\n"              /* (k, one push further down) */
  "  call _m_enter_c\n"
  "  addl $8, %esp\n"
  "  movl %eax, 32(%esp)\n"         /* k's slot now holds the function */
  "  popal\n"                       /* every register as the caller left it */
  "  ret\n"                         /* to the function; it returns to whatever m_enter_c left in the slot */
  ".globl _m_land\n"
  "_m_land:\n"                      /* the function returned here: esp is where it left it */
  "  pushl $0\n"                    /* the slot for the caller's return address */
  "  pushal\n"
  "  leal 36(%esp), %eax\n"         /* the stack pointer the function returned with */
  "  pushl %eax\n"
  "  call _m_land_c\n"
  "  addl $4, %esp\n"
  "  movl %eax, 32(%esp)\n"
  "  popal\n"                       /* EAX, EDX, ... as the function left them */
  "  ret\n");

static SThr *m_thr_new(void) {
  SThr *t = (SThr *)VirtualAlloc(NULL, sizeof(SThr), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  if (!t) return NULL;
  t->tid = GetCurrentThreadId(); t->render = t->tid == g_render_thread;
  LONG i = InterlockedIncrement(&m_nthr) - 1;
  if (i < 1024) m_thr[i] = t;
  TlsSetValue(m_tls, t);
  return t;
}
static void m_record(int k, uint32_t ra, uint64_t incl, uint64_t excl, int render) {
  Probe *p = &m_p[k];
  if (render) { p->f_calls++; p->f_incl += incl; p->f_excl += excl; if (incl > p->f_max) p->f_max = incl; }
  uint32_t key = ((uint32_t)k << 1 | (uint32_t)render) + 1, i = ((key * 2654435761u) ^ (ra * 40503u)) & (MP_HASH - 1);
  while (InterlockedCompareExchange(&m_hlock, 1, 0)) YieldProcessor();
  for (int n = 0; n < MP_HASH; n++, i = (i + 1) & (MP_HASH - 1)) {
    HEnt *h = &m_h[i];
    if (h->key == key && h->ra == ra) { h->calls++; h->incl += incl; h->excl += excl; if (incl > h->max) h->max = incl; break; }
    if (!h->key) { h->key = key; h->ra = ra; h->calls = 1; h->incl = incl; h->excl = excl; h->max = incl; break; }
    if (n == MP_HASH - 1) m_hfull++;
  }
  m_hlock = 0;
}
uint32_t m_enter_c(int k, uint32_t *slot) {
  uint32_t le = m_get_le();
  uint32_t target = m_p[k].orig;
  if (m_on) {
    SThr *t = (SThr *)TlsGetValue(m_tls);
    if (!t) t = m_thr_new();
    if (t && !t->render && t->tid == g_render_thread) t->render = 1;
    if (t && t->depth < MP_DEPTH) {
      SEnt *e = &t->e[t->depth];
      e->ret = *slot; e->slot = (uint32_t)(uintptr_t)slot; e->k = k; e->child = 0;
      t->depth++;                    /* (before the slot is changed: a sampler that finds m_land finds its entry) */
      *slot = (uint32_t)(uintptr_t)m_land;
      e->t0 = m_tsc();
    } else if (t) InterlockedIncrement(&m_deep);
  }
  m_set_le(le);
  return target;
}
uint32_t m_land_c(uint32_t cur) {
  uint64_t t1 = m_tsc(); uint32_t le = m_get_le();
  SThr *t = (SThr *)TlsGetValue(m_tls);
  int i = 0;
  if (t) while (i < t->depth && t->e[i].slot >= cur) i++;   /* the outer calls' slots are above the returning one's */
  if (!t || i >= t->depth) {
    /* no entry: cannot happen while the shadow stack is intact (m_land is only ever written with an entry) */
    InterlockedIncrement(&m_nomatch);
    klog("measure: a wrapped call returned with no entry (stack pointer %08X): the process cannot continue safely", cur);
    m_set_le(le); ExitProcess(0xDEAD0001u);
  }
  SEnt *e = &t->e[i];
  uint32_t ret = e->ret; uint64_t incl = t1 - e->t0, excl = incl > e->child ? incl - e->child : 0;
  if (i + 1 < t->depth) InterlockedExchangeAdd(&m_stale, t->depth - i - 1);
  t->depth = i;
  if (i > 0) t->e[i - 1].child += incl;
  m_record(e->k, ret, incl, excl, t->render);
  m_set_le(le);
  return ret;
}

/* ------------------------------------------------------------------------------------------------ what is wrapped */
typedef struct { const char *name; uint8_t group, hot; } ImpSpec;
static const ImpSpec m_imp[] = {
  {"Sleep", G_WAIT}, {"WaitForSingleObject", G_WAIT}, {"WaitForMultipleObjects", G_WAIT}, {"MsgWaitForMultipleObjects", G_WAIT},
  {"WSAWaitForMultipleEvents", G_WAIT}, {"select", G_WAIT},
  {"CreateFileA", G_FILE}, {"CreateFileW", G_FILE}, {"ReadFile", G_FILE}, {"WriteFile", G_FILE}, {"SetFilePointer", G_FILE},
  {"GetFileSize", G_FILE}, {"CloseHandle", G_FILE}, {"FindFirstFileA", G_FILE}, {"FindNextFileA", G_FILE}, {"FindClose", G_FILE},
  {"GetFileAttributesA", G_FILE}, {"MapViewOfFile", G_FILE}, {"UnmapViewOfFile", G_FILE}, {"CreateFileMappingA", G_FILE},
  {"FlushFileBuffers", G_FILE}, {"DeleteFileA", G_FILE}, {"CopyFileA", G_FILE}, {"CreateDirectoryA", G_FILE}, {"GetFileType", G_FILE},
  {"SetEndOfFile", G_FILE}, {"SetFileAttributesA", G_FILE},
  {"PeekMessageA", G_MSG}, {"TranslateMessage", G_MSG}, {"DispatchMessageA", G_MSG}, {"GetQueueStatus", G_MSG},
  {"SendMessageA", G_MSG}, {"PostMessageA", G_MSG}, {"PostThreadMessageA", G_MSG}, {"DefWindowProcA", G_MSG}, {"PostQuitMessage", G_MSG},
  {"GetAsyncKeyState", G_INPUT}, {"GetKeyboardState", G_INPUT}, {"GetKeyboardLayout", G_INPUT}, {"MapVirtualKeyA", G_INPUT},
  {"SetCursor", G_INPUT}, {"SetCursorPos", G_INPUT}, {"keybd_event", G_INPUT}, {"ImmGetContext", G_INPUT}, {"ImmReleaseContext", G_INPUT},
  {"ImmGetOpenStatus", G_INPUT}, {"ImmGetConversionStatus", G_INPUT}, {"ImmGetCompositionStringA", G_INPUT}, {"ImmNotifyIME", G_INPUT},
  {"ImmSetCandidateWindow", G_INPUT}, {"ImmSetCompositionWindow", G_INPUT}, {"ImmGetVirtualKey", G_INPUT},
  {"GetForegroundWindow", G_INPUT}, {"SetForegroundWindow", G_INPUT}, {"GetWindowRect", G_INPUT}, {"GetSystemMetrics", G_INPUT},
  {"ShowWindow", G_INPUT}, {"MoveWindow", G_INPUT}, {"GetWindowPlacement", G_INPUT}, {"AdjustWindowRectEx", G_INPUT},
  {"GetWindowThreadProcessId", G_INPUT}, {"AttachThreadInput", G_INPUT}, {"SystemParametersInfoA", G_INPUT}, {"FindWindowA", G_INPUT},
  {"OpenClipboard", G_INPUT}, {"CloseClipboard", G_INPUT},
  {"DrawTextA", G_GDI}, {"DrawTextW", G_GDI}, {"CreateDIBSection", G_GDI}, {"CreateFontIndirectA", G_GDI}, {"SelectObject", G_GDI},
  {"SetTextColor", G_GDI}, {"SetBkColor", G_GDI}, {"SetBkMode", G_GDI}, {"CreateCompatibleDC", G_GDI}, {"DeleteDC", G_GDI},
  {"DeleteObject", G_GDI}, {"GetObjectA", G_GDI},
  {"recv", G_NET}, {"send", G_NET}, {"recvfrom", G_NET}, {"sendto", G_NET}, {"ioctlsocket", G_NET}, {"WSAEnumNetworkEvents", G_NET},
  {"WSAResetEvent", G_NET}, {"WSASetEvent", G_NET}, {"WSAGetLastError", G_NET}, {"__WSAFDIsSet", G_NET}, {"connect", G_NET},
  {"gethostbyname", G_NET}, {"socket", G_NET}, {"closesocket", G_NET}, {"setsockopt", G_NET}, {"getsockopt", G_NET},
  {"timeGetTime", G_TIME}, {"QueryPerformanceCounter", G_TIME}, {"GetTickCount", G_TIME}, {"GetLocalTime", G_TIME},
  {"GetSystemTime", G_TIME}, {"timeBeginPeriod", G_TIME}, {"timeEndPeriod", G_TIME}, {"timeSetEvent", G_TIME}, {"timeKillEvent", G_TIME},
  {"SetEvent", G_SYNC}, {"ResetEvent", G_SYNC}, {"ReleaseSemaphore", G_SYNC}, {"ReleaseMutex", G_SYNC}, {"CreateEventA", G_SYNC},
  {"CreateThread", G_SYNC}, {"ResumeThread", G_SYNC}, {"SetThreadPriority", G_SYNC}, {"GetExitCodeThread", G_SYNC},
  {"WSASetEvent", G_SYNC}, {"DuplicateHandle", G_SYNC},
  {"VirtualAlloc", G_MEM}, {"VirtualFree", G_MEM}, {"GlobalAlloc", G_MEM}, {"GlobalLock", G_MEM}, {"GlobalUnlock", G_MEM},
  {"HeapCreate", G_MEM}, {"GlobalMemoryStatus", G_MEM},
  {"RegOpenKeyExA", G_OTHER}, {"RegOpenKeyA", G_OTHER}, {"RegQueryValueExA", G_OTHER}, {"RegSetValueExA", G_OTHER},
  {"RegCloseKey", G_OTHER}, {"RegCreateKeyExA", G_OTHER}, {"RegEnumValueA", G_OTHER}, {"RegEnumKeyExA", G_OTHER},
  {"LoadLibraryA", G_OTHER}, {"LoadLibraryExA", G_OTHER}, {"GetProcAddress", G_OTHER}, {"FreeLibrary", G_OTHER},
  {"OutputDebugStringA", G_OTHER}, {"CoCreateInstance", G_OTHER}, {"GetModuleHandleA", G_OTHER}, {"GetModuleFileNameA", G_OTHER},
  {"FindResourceA", G_OTHER}, {"LoadResource", G_OTHER}, {"GetEnvironmentVariableA", G_OTHER}, {"GetCurrentDirectoryA", G_OTHER},
  {"SetCurrentDirectoryA", G_OTHER}, {"GetFullPathNameA", G_OTHER}, {"GetShortPathNameA", G_OTHER}, {"CreateProcessA", G_OTHER},
  /* imports=2 */
  {"HeapAlloc", G_HEAP, 1}, {"HeapFree", G_HEAP, 1}, {"HeapReAlloc", G_HEAP, 1}, {"HeapSize", G_HEAP, 1},
  {"EnterCriticalSection", G_SYNC, 1}, {"LeaveCriticalSection", G_SYNC, 1}, {"InterlockedIncrement", G_SYNC, 1},
  {"InterlockedDecrement", G_SYNC, 1}, {"InterlockedExchange", G_SYNC, 1},
  {"IsBadReadPtr", G_OTHER, 1}, {"IsBadWritePtr", G_OTHER, 1}, {"IsBadCodePtr", G_OTHER, 1},
  {"lstrcmpiA", G_OTHER, 1}, {"lstrlenA", G_OTHER, 1}, {"lstrcpyA", G_OTHER, 1}, {"lstrcpynA", G_OTHER, 1}, {"lstrcatA", G_OTHER, 1},
  {"lstrlenW", G_OTHER, 1}, {"CompareStringA", G_OTHER, 1}, {"CompareStringW", G_OTHER, 1}, {"LCMapStringA", G_OTHER, 1},
  {"LCMapStringW", G_OTHER, 1}, {"MultiByteToWideChar", G_OTHER, 1}, {"WideCharToMultiByte", G_OTHER, 1},
  {"GetStringTypeA", G_OTHER, 1}, {"GetStringTypeW", G_OTHER, 1}, {"IsDBCSLeadByte", G_OTHER, 1}, {"CharNextA", G_OTHER, 1},
  {"TlsGetValue", G_OTHER, 1}, {"TlsSetValue", G_OTHER, 1}, {"GetLastError", G_OTHER, 1}, {"SetLastError", G_OTHER, 1},
  {"GetCurrentThreadId", G_OTHER, 1}, {"GetCurrentThread", G_OTHER, 1}, {"GetCurrentProcess", G_OTHER, 1}, {"MulDiv", G_OTHER, 1},
  {"wvsprintfA", G_OTHER, 1},
};

static int m_addr_in_module(uint32_t a, HMODULE m) {
  if (!m) return 0;
  IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS *)((char *)m + ((IMAGE_DOS_HEADER *)m)->e_lfanew);
  return a >= (uint32_t)(uintptr_t)m && a < (uint32_t)(uintptr_t)m + nt->OptionalHeader.SizeOfImage;
}
static void m_make_stub(int k) {
  uint8_t *s = m_stubs + 12 * k; int32_t d = (int32_t)((uintptr_t)m_enter - ((uintptr_t)s + 10));
  s[0] = 0x68; memcpy(s + 1, &k, 4); s[5] = 0xE9; memcpy(s + 6, &d, 4); s[10] = 0xCC; s[11] = 0xCC;
}

/* reads xikit_measure.txt: the import slots and the call sites, checked against the live game */
static void m_load_table(void) {
  char path[MAX_PATH], line[512]; snprintf(path, sizeof path, "%s\\xikit_measure.txt", g_dir);
  FILE *f = fopen(path, "r");
  if (!f) { klog("measure: no xikit_measure.txt next to xikit.dll: imports= and callsites= do nothing"); return; }
  int ni = 0, ni_other = 0, ns = 0, ns_bad = 0, ns_level = 0;
  while (fgets(line, sizeof line, f) && m_np < MP_MAX) {
    unsigned a, t, lv; char dll[64], nm[128], caller[160], tname[160];
    if (line[0] == 'I' && m_imports && sscanf(line, "I %x %63s %127s", &a, dll, nm) == 3) {
      const ImpSpec *sp = NULL;
      for (unsigned i = 0; i < sizeof m_imp / sizeof *m_imp; i++) if (!strcmp(m_imp[i].name, nm)) { sp = &m_imp[i]; break; }
      if (!sp || (sp->hot && m_imports < 2)) continue;
      uint32_t slot = a + g_rel;                            /* the import table is data: it moves with the image only */
      HMODULE dm = GetModuleHandleA(dll);
      uint32_t want = dm ? (uint32_t)(uintptr_t)GetProcAddress(dm, nm) : 0, live = *(volatile uint32_t *)(uintptr_t)slot;
      if (!live || (live != want && !m_addr_in_module(live, dm))) {
        klog("measure: import slot %08X (%s!%s) holds %08X, not %s's function (%08X): left alone", a, dll, nm, live, dll, want); ni_other++; continue;
      }
      Probe *p = &m_p[m_np]; memset(p, 0, sizeof *p);
      p->kind = 'I'; p->at = slot; p->ea = a; p->orig = live; p->group = sp->group; snprintf(p->name, sizeof p->name, "%s", nm);
      m_make_stub(m_np); m_np++; ni++;
    } else if (line[0] == 'S' && m_callsites && sscanf(line, "S %x %x %u %159s %159s", &a, &t, &lv, caller, tname) == 5) {
      if ((int)lv > m_callsites - 1) { ns_level++; continue; }      /* callsites=1: level 0; =2: levels 0-1; ... */
      uint8_t *site = (uint8_t *)(uintptr_t)live_of(a); uint32_t tl = live_of(t);
      int32_t rel; memcpy(&rel, site + 1, 4);
      if (site[0] != 0xE8 || (uint32_t)(uintptr_t)site + 5 + (uint32_t)rel != tl) { ns_bad++; continue; }
      Probe *p = &m_p[m_np]; memset(p, 0, sizeof *p);
      p->kind = 'S'; p->at = (uint32_t)(uintptr_t)site; p->ea = a; p->tea = t; p->orig = tl; p->group = G_SITE; p->level = (uint8_t)lv;
      snprintf(p->name, sizeof p->name, "%s", tname);
      m_make_stub(m_np); m_np++; ns++;
    }
  }
  fclose(f);
  klog("measure: %d import slots to wrap (%d held another function: left alone); %d call sites to time (%d not the analysed call in the live code: left alone; %d above callsites=%d's level)",
       ni, ni_other, ns, ns_bad, ns_level, m_callsites);
}

/* every probe to its `want`: on the render thread inside Present. For call sites the other threads are suspended
   (once for the whole batch), and a site some thread is stopped inside is left for the next time */
static int m_apply_probes(void) {
  Th th[512]; int n = 0, sites = 0, left = 0;
  static uint32_t eips[512];
  for (int k = 0; k < m_np; k++) if (m_p[k].kind == 'S' && m_p[k].want != m_p[k].on) sites = 1;
  EnterCriticalSection(&g_susplock);
  if (sites) {
    n = suspend_others(th, 512); if (n < 0) n = 0;
    for (int i = 0; i < n; i++) {
      CONTEXT c; memset(&c, 0, sizeof c); c.ContextFlags = CONTEXT_CONTROL;
      eips[i] = GetThreadContext(th[i].h, &c) ? c.Eip : 0;
    }
  }
  for (int k = 0; k < m_np; k++) {
    Probe *p = &m_p[k];
    if (p->on == p->want) continue;
    uint32_t to = p->want ? (uint32_t)(uintptr_t)(m_stubs + 12 * k) : p->orig;
    DWORD old;
    if (p->kind == 'I') {
      if (!VirtualProtect((void *)(uintptr_t)p->at, 4, PAGE_EXECUTE_READWRITE, &old)) continue;   /* (keeps execute if the page had it) */
      InterlockedExchange((volatile LONG *)(uintptr_t)p->at, (LONG)to);
      VirtualProtect((void *)(uintptr_t)p->at, 4, old, &old);
    } else {
      int busy = 0;                  /* a thread stopped inside the call instruction: not this frame */
      for (int i = 0; i < n; i++) if (eips[i] > p->at && eips[i] < p->at + 5) busy = 1;
      if (busy) { left++; continue; }
      int32_t rel = (int32_t)(to - (p->at + 5));
      if (!VirtualProtect((void *)(uintptr_t)(p->at + 1), 4, PAGE_EXECUTE_READWRITE, &old)) continue;
      memcpy((void *)(uintptr_t)(p->at + 1), &rel, 4);
      VirtualProtect((void *)(uintptr_t)(p->at + 1), 4, old, &old);
      FlushInstructionCache(GetCurrentProcess(), (void *)(uintptr_t)p->at, 5);
    }
    p->on = p->want;
  }
  if (sites) resume_all(th, n);
  LeaveCriticalSection(&g_susplock);
  return left;
}

/* the cost of a wrapped call, measured here: a dummy function called through a stub of its own */
static uint32_t __stdcall m_dummy(uint32_t x) { return x + 1; }
static double m_probe_cost_ns(void) {
  if (m_np >= MP_MAX) return -1;
  int k = m_np; Probe *p = &m_p[k]; memset(p, 0, sizeof *p);
  p->kind = 'D'; p->orig = (uint32_t)(uintptr_t)m_dummy; snprintf(p->name, sizeof p->name, "(calibration)"); m_make_stub(k);
  typedef uint32_t (__stdcall *F)(uint32_t); F direct = m_dummy, via = (F)(void *)(m_stubs + 12 * k);
  LONG was = m_on; m_on = 1;
  volatile uint32_t s = 0; LARGE_INTEGER a, b, c;
  QueryPerformanceCounter(&a); for (int i = 0; i < 20000; i++) s += direct((uint32_t)i);
  QueryPerformanceCounter(&b); for (int i = 0; i < 20000; i++) s += via((uint32_t)i);
  QueryPerformanceCounter(&c);
  m_on = was;
  /* the calibration's records are not a probe of the game: cleared */
  while (InterlockedCompareExchange(&m_hlock, 1, 0)) YieldProcessor();
  for (int i = 0; i < MP_HASH; i++) if (m_h[i].key == (((uint32_t)k << 1) | 0) + 1 || m_h[i].key == (((uint32_t)k << 1) | 1) + 1) memset(&m_h[i], 0, sizeof m_h[i]);
  m_hlock = 0;
  p->f_calls = 0; p->f_incl = p->f_excl = p->f_max = 0;
  return ((double)(c.QuadPart - b.QuadPart) - (double)(b.QuadPart - a.QuadPart)) * 1e9 / (double)g_qpf.QuadPart / 20000.0;
}

/* ------------------------------------------------------------------------------------------------ per frame */
static int m_mark, m_markdown, m_started, m_pending;
static uint64_t m_rt_cyc; static FILETIME m_rt_k, m_rt_u; static DWORD m_pf; static HANDLE m_rt_h;
typedef BOOL (WINAPI *QTCT)(HANDLE, PULONG64);
static QTCT m_qtct;
static uint32_t m_obj_ok_for, m_tim_ok_for;
static int m_readable(uint32_t a, uint32_t n) {
  MEMORY_BASIC_INFORMATION mb;
  if (!a || !VirtualQuery((void *)(uintptr_t)a, &mb, sizeof mb)) return 0;
  if (mb.State != MEM_COMMIT || (mb.Protect & (PAGE_GUARD | PAGE_NOACCESS)) || !(mb.Protect & 0xEE)) return 0;
  return a + n <= (uint32_t)(uintptr_t)mb.BaseAddress + (uint32_t)mb.RegionSize;
}
static void m_frame_write(double frame_ms, double present_ms, double kit_ms, LARGE_INTEGER t0) {
  /* the render thread's CPU: cycles (QueryThreadCycleTime) and time (GetThreadTimes) since the last frame */
  uint64_t cyc = 0; FILETIME c_, e_, k, u; double cpu_ms = -1;
  if (!m_rt_h) m_rt_h = OpenThread(THREAD_QUERY_INFORMATION, FALSE, GetCurrentThreadId());
  if (m_qtct && m_rt_h) m_qtct(m_rt_h, &cyc);
  if (m_rt_h && GetThreadTimes(m_rt_h, &c_, &e_, &k, &u)) {
    uint64_t now = ((uint64_t)k.dwHighDateTime << 32 | k.dwLowDateTime) + ((uint64_t)u.dwHighDateTime << 32 | u.dwLowDateTime);
    uint64_t was = ((uint64_t)m_rt_k.dwHighDateTime << 32 | m_rt_k.dwLowDateTime) + ((uint64_t)m_rt_u.dwHighDateTime << 32 | m_rt_u.dwLowDateTime);
    if (m_rt_k.dwLowDateTime | m_rt_k.dwHighDateTime | m_rt_u.dwLowDateTime | m_rt_u.dwHighDateTime) cpu_ms = (double)(now - was) / 10000.0;
    m_rt_k = k; m_rt_u = u;
  }
  uint64_t dcyc = m_rt_cyc ? cyc - m_rt_cyc : 0; m_rt_cyc = cyc;
  PROCESS_MEMORY_COUNTERS pm; memset(&pm, 0, sizeof pm); pm.cb = sizeof pm;
  DWORD dpf = 0; if (K32GetProcessMemoryInfo(GetCurrentProcess(), &pm, sizeof pm)) { dpf = m_pf ? pm.PageFaultCount - m_pf : 0; m_pf = pm.PageFaultCount; }
  /* the game's pacing fields, raw (RENDERER.md 3): only from memory that is there */
  uint32_t obj = 0, f[5] = {0}, ring[4] = {0}, tf = 0; int have = 0, have_t = 0;
  uint32_t pobj = live_of(0x104568FCu), pring = live_of(0x10456964u);
  obj = *(volatile uint32_t *)(uintptr_t)pobj;
  if (obj && (obj == m_obj_ok_for || m_readable(obj, 0x40))) {
    m_obj_ok_for = obj; have = 1;
    static const int off[5] = {0x28, 0x2C, 0x30, 0x38, 0x3C};
    for (int i = 0; i < 5; i++) f[i] = *(volatile uint32_t *)(uintptr_t)(obj + off[i]);
    uint32_t tim = *(volatile uint32_t *)(uintptr_t)(obj + 0x1C);
    if (tim && (tim == m_tim_ok_for || m_readable(tim, 8))) { m_tim_ok_for = tim; tf = *(volatile uint32_t *)(uintptr_t)(tim + 4); have_t = 1; }
  }
  for (int i = 0; i < 4; i++) ring[i] = *(volatile uint32_t *)(uintptr_t)(pring + 4 * i);
  /* the render thread's wrapped calls this frame, by group (exclusive time), and the longest one */
  double gms[G_N] = {0}; uint32_t gcalls[G_N] = {0}; int longest = -1; uint64_t lmax = 0;
  for (int i = 0; i < m_np; i++) {
    Probe *p = &m_p[i]; if (!p->f_calls) continue;
    gms[p->group] += m_ms(p->f_excl); gcalls[p->group] += p->f_calls;
    if (p->kind == 'I' && p->f_max > lmax) { lmax = p->f_max; longest = i; }
  }
  char line[1024]; int n = snprintf(line, sizeof line, "%u,%lld,%.3f,%.3f,%.3f,%llu,%.2f,%lu,%d,%d,", g_frame, (long long)t0.QuadPart, frame_ms, present_ms, kit_ms,
                                    (unsigned long long)dcyc, cpu_ms, (unsigned long)dpf, m_mark, g_master_off);
  if (have) n += snprintf(line + n, sizeof line - n, "%08X,%08X,%08X,%08X,%08X,", f[0], f[1], f[2], f[3], f[4]); else n += snprintf(line + n, sizeof line - n, ",,,,,");
  n += snprintf(line + n, sizeof line - n, "%08X,%08X,%08X,%08X,", ring[0], ring[1], ring[2], ring[3]);
  n += have_t ? snprintf(line + n, sizeof line - n, "%08X", tf) : 0;
  if (m_imports || m_callsites) {
    for (int g = 0; g < G_N; g++) n += snprintf(line + n, sizeof line - n, ",%.3f,%u", gms[g], gcalls[g]);
    n += snprintf(line + n, sizeof line - n, ",%s,%.3f", longest >= 0 ? m_p[longest].name : "", m_ms(lmax));
  }
  n += snprintf(line + n, sizeof line - n, "\n");
  ms_put(&m_sframes, line, (size_t)n);
}
static void m_probe_frame(void) {          /* each probe the render thread called this frame: frame,k,calls,incl,excl,max (TSC ticks) */
  char buf[64];
  for (int i = 0; i < m_np; i++) {
    Probe *p = &m_p[i]; if (!p->f_calls) continue;
    int n = snprintf(buf, sizeof buf, "%u,%d,%u,%llu,%llu,%llu\n", g_frame, i, p->f_calls, (unsigned long long)p->f_incl, (unsigned long long)p->f_excl, (unsigned long long)p->f_max);
    ms_put(&m_sprobe, buf, (size_t)n);
    if (p->f_calls > p->w_maxcalls) p->w_maxcalls = p->f_calls;
    p->f_calls = 0; p->f_incl = p->f_excl = p->f_max = 0;
  }
}
static void m_report(void);
/* the frame boundary: from kit_present on the render thread, after the device's Present returned */
static void measure_frame(double frame_ms, double present_ms, double kit_ms, LARGE_INTEGER t0) {
  if (!m_started) return;
  if (m_np && !m_on) {                     /* the first frame: the probes go in */
    m_on = 1;
    for (int k = 0; k < m_np; k++) m_p[k].want = 1;
    LARGE_INTEGER a, b; QueryPerformanceCounter(&a); m_apply_probes(); QueryPerformanceCounter(&b);
    int on = 0; for (int k = 0; k < m_np; k++) on += m_p[k].on;
    klog("measure: %d of %d wrapped calls in place (%.1f ms; the rest at the next frames)", on, m_np, m_qms(a, b));
  } else if (m_np && m_pending) m_pending = m_apply_probes();
  int down = (GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState(VK_MENU) & 0x8000) && (GetAsyncKeyState(VK_F7) & 0x8000);
  if (down && !m_markdown) { m_mark++; klog("measure: mark %d at frame %u", m_mark, g_frame); }
  m_markdown = down;
  if (m_frames) m_frame_write(frame_ms, present_ms, kit_ms, t0);
  if (m_np) m_probe_frame();
  if (g_frame % 60 == 0) m_report();
}

/* every 60 frames on the render thread (inside Present): call sites called too often are put back; every
   report_every frames the TSC rate is measured again and the totals are dumped (by the writer thread, so the
   frame being measured does not carry the file's cost) */
static void m_report(void) {
  if (m_np) {
    int back = 0;
    for (int k = 0; k < m_np; k++) {
      Probe *p = &m_p[k];
      if (p->kind == 'S' && p->on && p->want && (int)p->w_maxcalls > m_callsite_max) {
        p->want = 0; back++;
        klog("measure: call site %08X (to %s) made %u calls in one frame (callsite_max=%d): put back", p->ea, p->name, p->w_maxcalls, m_callsite_max);
      }
      p->w_maxcalls = 0;
    }
    if (back) m_pending = m_apply_probes();
  }
  if (g_frame % (uint32_t)g_cfg.report_every) return;
  m_cal();
  if (m_np) {
    LARGE_INTEGER q; QueryPerformanceCounter(&q);
    ms_printf(&m_sprobe, "#cal,%lld,%llu,%.3f\n", (long long)q.QuadPart, (unsigned long long)m_tsc(), m_tsc_per_ms);
    m_dump_wanted = 1;
  }
  MStream *all[5] = {&m_sframes, &m_sprobe, &m_sraw, &m_ssys, &m_sthreads};
  for (int i = 0; i < 5; i++) if (all[i]->dropped) { klog("measure: %s: %ld writes dropped (its buffer was full)", all[i]->path, (long)all[i]->dropped); all[i]->dropped = 0; }
}
static void m_dump_probes(void) {
  char path[MAX_PATH]; snprintf(path, sizeof path, "%s\\xikit_probes_%lu.csv", g_out, (unsigned long)GetCurrentProcessId());
  FILE *f = fopen(path, "w");
  if (!f) return;
  fprintf(f, "# wrapped calls since the start: TSC %.3f ticks per ms; a wrapped call costs %.0f ns more than a plain one (measured at the start; inside each caller's time)\n"
             "# stale entries dropped %ld, calls past the depth limit %ld, hash full %ld\n"
             "k,kind,name,analysed,target,level,group,on,thread,return_to,calls,incl_ms,excl_ms,max_ms\n",
          m_tsc_per_ms, m_cost_ns, (long)m_stale, (long)m_deep, (long)m_hfull);
  static HEnt copy[MP_HASH];
  while (InterlockedCompareExchange(&m_hlock, 1, 0)) YieldProcessor();
  memcpy(copy, m_h, sizeof copy);
  m_hlock = 0;
  for (int i = 0; i < MP_HASH; i++) {
    HEnt *h = &copy[i]; if (!h->key) continue;
    int k = (int)((h->key - 1) >> 1), r = (int)((h->key - 1) & 1); Probe *p = &m_p[k];
    char ra[300]; where(h->ra, ra, sizeof ra);
    fprintf(f, "%d,%c,%s,%08X,%08X,%d,%s,%d,%s,%s,%llu,%.3f,%.3f,%.3f\n", k, p->kind, p->name, p->ea, p->tea, p->level, m_gname[p->group], p->on,
            r ? "render" : "other", ra, (unsigned long long)h->calls, m_ms(h->incl), m_ms(h->excl), m_ms(h->max));
  }
  fclose(f);
}

/* ------------------------------------------------------------------------------------------------ sample=3 */
typedef struct { uint32_t lo, hi; } Rg;
static Rg *m_rg; static int m_nrg;
static int cmp_rg(const void *a, const void *b) { uint32_t x = ((const Rg *)a)->lo, y = ((const Rg *)b)->lo; return x < y ? -1 : x > y; }
static int m_nmods_seen;
static void m_modules(int emit) {            /* every module's readable pages (for the return-address check); 'M' records */
  HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetCurrentProcessId());
  if (snap == INVALID_HANDLE_VALUE) return;
  MODULEENTRY32 me; me.dwSize = sizeof me; int n = 0, nm = 0;
  for (BOOL ok = Module32First(snap, &me); ok; ok = Module32Next(snap, &me)) nm++;
  if (nm == m_nmods_seen && m_nrg) { CloseHandle(snap); return; }
  m_nmods_seen = nm;
  for (BOOL ok = Module32First(snap, &me); ok; ok = Module32Next(snap, &me)) {
    uint32_t lo = (uint32_t)(uintptr_t)me.modBaseAddr, hi = lo + me.modBaseSize;
    if (emit) {
      uint8_t rec[12]; size_t ln = strlen(me.szExePath); uint16_t l16 = (uint16_t)(ln > 1000 ? 1000 : ln);
      rec[0] = 'M'; rec[1] = 0; memcpy(rec + 2, &l16, 2); memcpy(rec + 4, &lo, 4); memcpy(rec + 8, &me.modBaseSize, 4);
      ms_put(&m_sraw, rec, 12); ms_put(&m_sraw, me.szExePath, l16);
    }
    for (uint32_t a = lo; a < hi && n < 16384;) {
      MEMORY_BASIC_INFORMATION mb;
      if (!VirtualQuery((void *)(uintptr_t)a, &mb, sizeof mb)) break;
      uint32_t e = (uint32_t)(uintptr_t)mb.BaseAddress + (uint32_t)mb.RegionSize;
      if (mb.State == MEM_COMMIT && !(mb.Protect & (PAGE_GUARD | PAGE_NOACCESS)) && (mb.Protect & 0xF0)) { m_rg[n].lo = a; m_rg[n].hi = e < hi ? e : hi; n++; }
      if (e <= a) break;
      a = e;
    }
  }
  CloseHandle(snap);
  qsort(m_rg, n, sizeof *m_rg, cmp_rg); m_nrg = n;
}
static int m_code(uint32_t r) {               /* r-7 .. r inside one executable region of a module */
  int lo = 0, hi = m_nrg - 1;
  while (lo <= hi) {
    int mid = (lo + hi) / 2;
    if (r <= m_rg[mid].lo) hi = mid - 1;
    else if (r > m_rg[mid].hi) lo = mid + 1;
    else return r - 7 >= m_rg[mid].lo;
  }
  return 0;
}
static int m_after_call(uint32_t r) {        /* the bytes before r are a call instruction (as after_call_quiet) */
  if (!m_code(r)) return 0;
  const uint8_t *p = (const uint8_t *)(uintptr_t)r;
  if (p[-5] == 0xE8) return 1;
  if (p[-2] == 0xFF && ((p[-1] >> 3) & 7) == 2 && (p[-1] >> 6) == 3) return 1;
  if (p[-2] == 0xFF && ((p[-1] >> 3) & 7) == 2 && (p[-1] >> 6) == 0 && (p[-1] & 7) != 4 && (p[-1] & 7) != 5) return 1;
  if (p[-3] == 0xFF && ((p[-2] >> 3) & 7) == 2 && (p[-2] >> 6) == 1) return 1;
  if (p[-6] == 0xFF && ((p[-5] >> 3) & 7) == 2 && ((p[-5] >> 6) == 2 || (p[-5] & 0xC7) == 5)) return 1;
  if (p[-4] == 0xFF && ((p[-3] >> 3) & 7) == 2 && (p[-3] >> 6) == 1 && (p[-3] & 7) == 4) return 1;
  if (p[-7] == 0xFF && ((p[-6] >> 3) & 7) == 2 && (p[-6] >> 6) == 2 && (p[-6] & 7) == 4) return 1;
  return 0;
}
typedef struct { DWORD tid; HANDLE h; uint64_t cyc; uint32_t top, eip, esp, start; int alive, render; } RThr;
static RThr m_rt[512]; static int m_nrt; static DWORD m_own[8]; static int m_nown;
static int m_cycles_work = -1;               /* QueryThreadCycleTime moves for a running thread (-1: not known yet) */
static int m_sample_ms = 1, m_other_ms = 5, m_stack_rt = 16384, m_stack_other = 4096;
typedef LONG (WINAPI *NQIT)(HANDLE, int, PVOID, ULONG, PULONG);
static NQIT m_nqit;
static void m_threads(void) {
  for (int i = 0; i < m_nrt; i++) m_rt[i].alive = 0;
  HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); if (snap == INVALID_HANDLE_VALUE) return;
  THREADENTRY32 te; te.dwSize = sizeof te; DWORD pid = GetCurrentProcessId();
  for (BOOL ok = Thread32First(snap, &te); ok; ok = Thread32Next(snap, &te)) {
    if (te.th32OwnerProcessID != pid) continue;
    int own = 0; for (int i = 0; i < m_nown; i++) if (m_own[i] == te.th32ThreadID) own = 1;
    if (own) continue;
    int i = 0; while (i < m_nrt && m_rt[i].tid != te.th32ThreadID) i++;
    if (i == m_nrt) {
      if (m_nrt >= 512) continue;
      HANDLE h = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION, FALSE, te.th32ThreadID);
      if (!h) continue;
      RThr *t = &m_rt[m_nrt++]; memset(t, 0, sizeof *t); t->tid = te.th32ThreadID; t->h = h;
      if (m_nqit) m_nqit(h, 9 /* ThreadQuerySetWin32StartAddress */, &t->start, sizeof t->start, NULL);
      uint8_t rec[20]; LARGE_INTEGER q; QueryPerformanceCounter(&q);
      rec[0] = 'T'; rec[1] = 0; rec[2] = rec[3] = 0; memcpy(rec + 4, &t->tid, 4); memcpy(rec + 8, &t->start, 4); memcpy(rec + 12, &q.QuadPart, 8);
      ms_put(&m_sraw, rec, 20);
    }
    m_rt[i].alive = 1; m_rt[i].render = m_rt[i].tid == g_render_thread;
  }
  CloseHandle(snap);
  for (int i = 0; i < m_nrt;) {
    if (!m_rt[i].alive) { CloseHandle(m_rt[i].h); m_rt[i] = m_rt[--m_nrt]; } else i++;
  }
}
static void m_sample_one(RThr *t, LARGE_INTEGER q) {
  static uint32_t ras[160]; int nra = 0, cap = t->render ? 128 : 48, lim = t->render ? m_stack_rt : m_stack_other;
  CONTEXT c; memset(&c, 0, sizeof c); c.ContextFlags = CONTEXT_CONTROL;
  if (SuspendThread(t->h) == (DWORD)-1) return;
  int got = GetThreadContext(t->h, &c), need_top = 0;
  if (got) {
    /* the stack's top, from an earlier look (VirtualQuery is never called while the thread is suspended) */
    if (!t->top || c.Esp >= t->top || c.Esp < t->top - 0x800000) { need_top = 1; t->top = 0; }
    if (c.Eip == t->eip && c.Esp == t->esp && !t->render) {          /* not moved since the last look: a short record */
      ResumeThread(t->h);
      uint8_t rec[20]; rec[0] = 'R'; rec[1] = 0; rec[2] = rec[3] = 0; memcpy(rec + 4, &t->tid, 4); uint32_t fr = g_frame; memcpy(rec + 8, &fr, 4); memcpy(rec + 12, &q.QuadPart, 8);
      ms_put(&m_sraw, rec, 20); return;
    }
    SThr *sh = NULL;                          /* the thread's shadow stack: a wrapped call's real return address */
    for (LONG i = 0; i < m_nthr && i < 1024; i++) if (m_thr[i] && m_thr[i]->tid == t->tid) { sh = m_thr[i]; break; }
    if (t->top) {
      uint32_t *p = (uint32_t *)(uintptr_t)c.Esp, *top = (uint32_t *)(uintptr_t)t->top;
      for (int k = 0; p < top && k < lim && nra < cap; p++, k++) {
        uint32_t v = *p;
        if (v == (uint32_t)(uintptr_t)m_land && sh) {
          for (int d = 0; d < sh->depth; d++) if (sh->e[d].slot == (uint32_t)(uintptr_t)p) { v = sh->e[d].ret; break; }
        }
        if (in_ours(v) ? after_call_quiet(v) : m_after_call(v)) ras[nra++] = v;
      }
    }
  }
  ResumeThread(t->h);
  if (!got) return;
  if (need_top) {
    MEMORY_BASIC_INFORMATION mb;
    t->top = VirtualQuery((void *)(uintptr_t)c.Esp, &mb, sizeof mb) && mb.State == MEM_COMMIT ? (uint32_t)(uintptr_t)mb.BaseAddress + (uint32_t)mb.RegionSize : 0;
  }
  t->eip = c.Eip; t->esp = c.Esp;
  uint8_t rec[24]; uint16_t n16 = (uint16_t)nra; uint32_t fr = g_frame;
  rec[0] = 'S'; rec[1] = (uint8_t)((t->render ? 1 : 0) | (nra == cap ? 4 : 0)); memcpy(rec + 2, &n16, 2); memcpy(rec + 4, &t->tid, 4);
  memcpy(rec + 8, &fr, 4); memcpy(rec + 12, &q.QuadPart, 8); memcpy(rec + 20, &c.Eip, 4);
  EnterCriticalSection(&m_sraw.cs);          /* the record and its addresses together */
  if (m_sraw.n + 24 + 4 * (size_t)nra <= m_sraw.cap) { memcpy(m_sraw.b + m_sraw.n, rec, 24); memcpy(m_sraw.b + m_sraw.n + 24, ras, 4 * (size_t)nra); m_sraw.n += 24 + 4 * (size_t)nra; }
  else InterlockedIncrement(&m_sraw.dropped);
  LeaveCriticalSection(&m_sraw.cs);
}
static DWORD WINAPI m_sampler(LPVOID u) {
  (void)u; timeBeginPeriod(1);
  DWORD last_refresh = 0, last_other = 0; int stopped = 0;
  for (;;) {
    Sleep(m_sample_ms);
    if (!g_render_thread) continue;
    if (m_raw_bytes > m_raw_max) { if (!stopped) klog("sample=3: xikit_raw reached raw_max_mb: no more samples"); stopped = 1; Sleep(1000); continue; }
    DWORD now = GetTickCount();
    if (now - last_refresh >= 1000) {
      m_threads(); m_modules(1); last_refresh = now;
      if (m_cycles_work < 0 && m_qtct) {         /* does the cycle count move for the render thread? */
        for (int i = 0; i < m_nrt; i++) if (m_rt[i].render) {
          uint64_t c = 0; static uint64_t first; static int seen;
          if (!m_qtct(m_rt[i].h, &c)) { m_cycles_work = 0; break; }
          if (!seen) { first = c; seen = 1; } else { m_cycles_work = c != first; klog("sample=3: QueryThreadCycleTime %s", m_cycles_work ? "moves: idle threads are skipped" : "does not move: every thread is looked at"); }
        }
      }
    }
    LARGE_INTEGER q; QueryPerformanceCounter(&q);
    int others = now - last_other >= (DWORD)m_other_ms; if (others) last_other = now;
    EnterCriticalSection(&g_susplock);
    for (int i = 0; i < m_nrt; i++) {
      RThr *t = &m_rt[i];
      if (!t->render && !others) continue;
      if (!t->render && m_cycles_work == 1) {
        uint64_t c = 0; m_qtct(t->h, &c);
        if (c == t->cyc) continue;               /* no CPU since the last look */
        t->cyc = c;
      }
      m_sample_one(t, q);
    }
    LeaveCriticalSection(&g_susplock);
  }
  return 0;
}

/* ------------------------------------------------------------------------------------------------ sysstats=1 */
static uint64_t ft64(FILETIME f) { return (uint64_t)f.dwHighDateTime << 32 | f.dwLowDateTime; }
static DWORD WINAPI m_sys(LPVOID u) {
  (void)u;
  FILETIME si0, sk0, su0, c, e, pk0, pu0; GetSystemTimes(&si0, &sk0, &su0); GetProcessTimes(GetCurrentProcess(), &c, &e, &pk0, &pu0);
  IO_COUNTERS io0; memset(&io0, 0, sizeof io0); GetProcessIoCounters(GetCurrentProcess(), &io0);
  DWORD pf0 = 0; uint32_t fr0 = g_frame; LARGE_INTEGER q0; QueryPerformanceCounter(&q0);
  typedef struct { DWORD tid; uint64_t cyc, k, u; } TP; static TP prev[512]; int np = 0;
  for (int tick = 1;; tick++) {
    Sleep(1000);
    LARGE_INTEGER q; QueryPerformanceCounter(&q);
    FILETIME si, sk, su, pk, pu; GetSystemTimes(&si, &sk, &su); GetProcessTimes(GetCurrentProcess(), &c, &e, &pk, &pu);
    double sidle = (double)(ft64(si) - ft64(si0)), skern = (double)(ft64(sk) - ft64(sk0)), suser = (double)(ft64(su) - ft64(su0));   /* kernel includes idle */
    double tot = skern + suser;
    PROCESS_MEMORY_COUNTERS_EX pm; memset(&pm, 0, sizeof pm); pm.cb = sizeof pm; K32GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&pm, sizeof pm);
    IO_COUNTERS io; memset(&io, 0, sizeof io); GetProcessIoCounters(GetCurrentProcess(), &io);
    DWORD hc = 0; GetProcessHandleCount(GetCurrentProcess(), &hc);
    ms_printf(&m_ssys, "%lld,%u,%u,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%lu,%llu,%.2f,%llu,%.2f,%llu,%.2f,%lu\n",
              (long long)q.QuadPart, g_frame, g_frame - fr0,
              (double)(ft64(pu) - ft64(pu0)) / 10000.0, (double)(ft64(pk) - ft64(pk0)) / 10000.0,
              tot > 0 ? 100.0 * (tot - sidle) / tot : -1, tot > 0 ? 100.0 * (skern - sidle) / tot : -1,
              (double)pm.WorkingSetSize / 1048576.0, (double)pm.PrivateUsage / 1048576.0,
              (unsigned long)(pf0 ? pm.PageFaultCount - pf0 : 0),
              (unsigned long long)(io.ReadOperationCount - io0.ReadOperationCount), (double)(io.ReadTransferCount - io0.ReadTransferCount) / 1048576.0,
              (unsigned long long)(io.WriteOperationCount - io0.WriteOperationCount), (double)(io.WriteTransferCount - io0.WriteTransferCount) / 1048576.0,
              (unsigned long long)(io.OtherOperationCount - io0.OtherOperationCount), (double)(io.OtherTransferCount - io0.OtherTransferCount) / 1048576.0,
              (unsigned long)hc);
    si0 = si; sk0 = sk; su0 = su; pk0 = pk; pu0 = pu; io0 = io; pf0 = pm.PageFaultCount; fr0 = g_frame; q0 = q;
    if (tick % 2) continue;
    /* each thread: start address, CPU cycles and times since the last line */
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); if (snap == INVALID_HANDLE_VALUE) continue;
    THREADENTRY32 te; te.dwSize = sizeof te; DWORD pid = GetCurrentProcessId();
    static TP cur[512]; int nc = 0;
    for (BOOL ok = Thread32First(snap, &te); ok && nc < 512; ok = Thread32Next(snap, &te)) {
      if (te.th32OwnerProcessID != pid) continue;
      HANDLE h = OpenThread(THREAD_QUERY_INFORMATION, FALSE, te.th32ThreadID); if (!h) continue;
      uint32_t start = 0; uint64_t cyc = 0; FILETIME tc, tx, tk, tu;
      if (m_nqit) m_nqit(h, 9, &start, sizeof start, NULL);
      if (m_qtct) m_qtct(h, &cyc);
      if (!GetThreadTimes(h, &tc, &tx, &tk, &tu)) { CloseHandle(h); continue; }
      int prio = GetThreadPriority(h); CloseHandle(h);
      TP *p = NULL; for (int i = 0; i < np; i++) if (prev[i].tid == te.th32ThreadID) p = &prev[i];
      char nm[300]; where(start, nm, sizeof nm);
      int own = 0; for (int i = 0; i < m_nown; i++) if (m_own[i] == te.th32ThreadID) own = 1;
      ms_printf(&m_sthreads, "%lld,%lu,%s,%s,%d,%.1f,%.1f,%.1f\n", (long long)q.QuadPart, (unsigned long)te.th32ThreadID, nm,
                te.th32ThreadID == g_render_thread ? "render" : own ? "kit" : "", prio,
                p ? (double)(cyc - p->cyc) / 1e6 : -1, p ? (double)(ft64(tu) - p->u) / 10000.0 : -1, p ? (double)(ft64(tk) - p->k) / 10000.0 : -1);
      cur[nc].tid = te.th32ThreadID; cur[nc].cyc = cyc; cur[nc].k = ft64(tk); cur[nc].u = ft64(tu); nc++;
    }
    CloseHandle(snap);
    memcpy(prev, cur, sizeof(TP) * nc); np = nc;
  }
  return 0;
}

/* ------------------------------------------------------------------------------------------------ env=1 */
static FILE *m_envf;
static void envp(const char *fmt, ...) {
  if (!m_envf) return;
  va_list a; va_start(a, fmt); vfprintf(m_envf, fmt, a); va_end(a); fputc('\n', m_envf); fflush(m_envf);
}
static void env_regkey(HKEY root, const char *rname, const char *key, int depth) {
  HKEY h; if (RegOpenKeyExA(root, key, 0, KEY_READ, &h) != ERROR_SUCCESS) return;
  envp("[registry %s\\%s]", rname, key);
  for (DWORD i = 0;; i++) {
    char name[256]; BYTE data[1024]; DWORD nl = sizeof name, dl = sizeof data, t = 0;
    LONG r = RegEnumValueA(h, i, name, &nl, NULL, &t, data, &dl);
    if (r == ERROR_NO_MORE_ITEMS) break;
    if (r != ERROR_SUCCESS && r != ERROR_MORE_DATA) break;
    if (t == REG_DWORD && dl >= 4) { DWORD v; memcpy(&v, data, 4); envp("  %s = dword %lu (0x%08lX)", name[0] ? name : "(default)", (unsigned long)v, (unsigned long)v); }
    else if ((t == REG_SZ || t == REG_EXPAND_SZ) && r == ERROR_SUCCESS) { data[dl < sizeof data ? dl : sizeof data - 1] = 0; envp("  %s = \"%s\"", name[0] ? name : "(default)", (char *)data); }
    else { char hx[200]; int k = 0; for (DWORD j = 0; j < dl && j < 64 && k < 190; j++) k += snprintf(hx + k, sizeof hx - k, "%02X", data[j]); envp("  %s = type %lu, %lu bytes: %s", name[0] ? name : "(default)", (unsigned long)t, (unsigned long)dl, hx); }
  }
  if (depth > 0) for (DWORD i = 0;; i++) {
    char sub[256], full[600]; DWORD sl = sizeof sub;
    if (RegEnumKeyExA(h, i, sub, &sl, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) break;
    snprintf(full, sizeof full, "%s\\%s", key, sub); env_regkey(root, rname, full, depth - 1);
  }
  RegCloseKey(h);
}
static int cmp_d(const void *a, const void *b) { double x = *(const double *)a, y = *(const double *)b; return x < y ? -1 : x > y; }
static void env_timers(const char *when) {
  envp("[timers: %s, frame %u]", when, g_frame);
  typedef LONG (WINAPI *NQTR)(PULONG, PULONG, PULONG);
  NQTR nq = (NQTR)(void *)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryTimerResolution");
  ULONG mn = 0, mx = 0, cu = 0; if (nq && nq(&mn, &mx, &cu) == 0) envp("  timer resolution (NtQueryTimerResolution): current %.4f ms, coarsest %.4f ms, finest %.4f ms", cu / 10000.0, mn / 10000.0, mx / 10000.0);
  else envp("  timer resolution: NtQueryTimerResolution not available");
  TIMECAPS tc; if (timeGetDevCaps(&tc, sizeof tc) == 0) envp("  timeGetDevCaps: period %u..%u ms", tc.wPeriodMin, tc.wPeriodMax);
  envp("  QueryPerformanceFrequency %lld; TSC %.3f ticks per ms", (long long)g_qpf.QuadPart, m_tsc_per_ms);
  /* Sleep(1), as the game's wait loop calls it (0x10012B12..0x10012B64), measured: 200 calls */
  static double d[200]; LARGE_INTEGER a, b;
  for (int i = 0; i < 200; i++) { QueryPerformanceCounter(&a); Sleep(1); QueryPerformanceCounter(&b); d[i] = m_qms(a, b); }
  qsort(d, 200, sizeof d[0], cmp_d);
  envp("  Sleep(1), 200 calls: min %.3f, median %.3f, 95%% %.3f, max %.3f ms", d[0], d[100], d[190], d[199]);
  /* timeGetTime's and GetTickCount's steps (the game's frame timer reads timeGetTime: RENDERER.md 3), over 100 ms */
  int steps[64] = {0}, tsteps[64] = {0}; DWORD t0 = timeGetTime(), g0 = GetTickCount(); QueryPerformanceCounter(&a);
  do {
    DWORD t = timeGetTime(), g = GetTickCount();
    if (t != t0) { DWORD s = t - t0; steps[s < 63 ? s : 63]++; t0 = t; }
    if (g != g0) { DWORD s = g - g0; tsteps[s < 63 ? s : 63]++; g0 = g; }
    QueryPerformanceCounter(&b);
  } while (m_qms(a, b) < 100);
  char l1[400] = "", l2[400] = ""; int k1 = 0, k2 = 0;
  for (int i = 1; i < 64; i++) { if (steps[i]) k1 += snprintf(l1 + k1, sizeof l1 - k1, " %d ms x%d", i, steps[i]); if (tsteps[i]) k2 += snprintf(l2 + k2, sizeof l2 - k2, " %d ms x%d", i, tsteps[i]); }
  envp("  timeGetTime steps over 100 ms:%s", l1); envp("  GetTickCount steps over 100 ms:%s", l2);
}
#define BENCH(label, n, stmt) do { LARGE_INTEGER a_, b_; QueryPerformanceCounter(&a_); for (int i_ = 0; i_ < (n); i_++) { stmt; } QueryPerformanceCounter(&b_); \
    envp("  %-40s %9.0f ns a call (%d calls)", label, m_qms(a_, b_) * 1e6 / (n), (n)); } while (0)
static void env_bench(void) {
  envp("[the cost of common calls, measured on a kit thread]");
  volatile uint64_t s = 0; LARGE_INTEGER q; MSG msg; POINT pt; BYTE kb[256];
  BENCH("rdtsc", 100000, s += m_tsc());
  BENCH("QueryPerformanceCounter", 20000, (QueryPerformanceCounter(&q), s += q.QuadPart));
  BENCH("timeGetTime", 20000, s += timeGetTime());
  BENCH("GetTickCount", 20000, s += GetTickCount());
  BENCH("GetAsyncKeyState", 5000, s += (uint64_t)GetAsyncKeyState(VK_SHIFT));
  BENCH("GetKeyboardState", 2000, s += GetKeyboardState(kb));
  BENCH("GetForegroundWindow", 5000, s += (uint64_t)(uintptr_t)GetForegroundWindow());
  BENCH("GetCursorPos", 5000, s += GetCursorPos(&pt));
  BENCH("PeekMessageA (this thread, empty queue)", 5000, s += PeekMessageA(&msg, NULL, 0, 0, PM_NOREMOVE));
  BENCH("Sleep(0)", 2000, Sleep(0));
  BENCH("SwitchToThread", 2000, s += SwitchToThread());
  HANDLE ev = CreateEventA(NULL, FALSE, FALSE, NULL);
  BENCH("SetEvent + WaitForSingleObject", 5000, (SetEvent(ev), s += WaitForSingleObject(ev, 0)));
  BENCH("WaitForSingleObject (not signaled, 0)", 5000, s += WaitForSingleObject(ev, 0));
  CloseHandle(ev);
  CRITICAL_SECTION cs; InitializeCriticalSection(&cs);
  BENCH("EnterCriticalSection + Leave", 100000, (EnterCriticalSection(&cs), LeaveCriticalSection(&cs)));
  DeleteCriticalSection(&cs);
  HANDLE hp = GetProcessHeap();
  BENCH("HeapAlloc + HeapFree (64 bytes)", 50000, HeapFree(hp, 0, HeapAlloc(hp, 0, 64)));
  MEMORY_BASIC_INFORMATION mb; BENCH("VirtualQuery", 5000, s += VirtualQuery(&mb, &mb, sizeof mb));
  BENCH("IsBadReadPtr (valid)", 5000, s += IsBadReadPtr(&mb, 4));
  char tmp[MAX_PATH]; snprintf(tmp, sizeof tmp, "%s\\xikit_bench.tmp", g_out);
  HANDLE fh = CreateFileA(tmp, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE, NULL);
  if (fh != INVALID_HANDLE_VALUE) {
    static char blk[65536]; DWORD w; for (int i = 0; i < 16; i++) WriteFile(fh, blk, sizeof blk, &w, NULL);
    BENCH("SetFilePointer + ReadFile (4 KB, cached)", 2000, (SetFilePointer(fh, (i_ % 256) * 4096, NULL, FILE_BEGIN), ReadFile(fh, blk, 4096, &w, NULL)));
    BENCH("ReadFile (64 KB, cached)", 500, (SetFilePointer(fh, 0, NULL, FILE_BEGIN), ReadFile(fh, blk, 65536, &w, NULL)));
    CloseHandle(fh);
  }
  BENCH("CreateFileA + CloseHandle (this folder's ini)", 200, CloseHandle(CreateFileA(g_ini, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL)));
  HDC dc = CreateCompatibleDC(NULL);
  if (dc) {
    BITMAPINFO bi; memset(&bi, 0, sizeof bi); bi.bmiHeader.biSize = sizeof bi.bmiHeader; bi.bmiHeader.biWidth = 256; bi.bmiHeader.biHeight = -64; bi.bmiHeader.biPlanes = 1; bi.bmiHeader.biBitCount = 32;
    void *bits; HBITMAP bm = CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &bits, NULL, 0);
    if (bm) {
      HGDIOBJ old = SelectObject(dc, bm); RECT r = {0, 0, 256, 64};
      BENCH("DrawTextA (12 characters, into a DIB)", 1000, s += DrawTextA(dc, "Port Jeuno 1", 12, &r, DT_LEFT | DT_TOP | DT_SINGLELINE));
      SelectObject(dc, old); DeleteObject(bm);
    }
    DeleteDC(dc);
  }
  (void)s;
}
static void env_copy(const char *from_dir, const char *name) {
  char a[MAX_PATH], b[MAX_PATH]; snprintf(a, sizeof a, "%s\\%s", from_dir, name); snprintf(b, sizeof b, "%s\\copy_%s", g_out, name);
  if (GetFileAttributesA(a) != INVALID_FILE_ATTRIBUTES && CopyFileA(a, b, FALSE)) envp("  copied %s", a);
}
static void env_modules(void) {
  HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetCurrentProcessId());
  if (snap == INVALID_HANDLE_VALUE) return;
  MODULEENTRY32 me; me.dwSize = sizeof me;
  for (BOOL ok = Module32First(snap, &me); ok; ok = Module32Next(snap, &me)) {
    IMAGE_DOS_HEADER *dh = (IMAGE_DOS_HEADER *)me.modBaseAddr; uint32_t ts = 0;
    if (dh && dh->e_magic == IMAGE_DOS_SIGNATURE) ts = ((IMAGE_NT_HEADERS *)(me.modBaseAddr + dh->e_lfanew))->FileHeader.TimeDateStamp;
    envp("  %08X %8lX  stamp %08X  %s", (uint32_t)(uintptr_t)me.modBaseAddr, (unsigned long)me.modBaseSize, ts, me.szExePath);
  }
  CloseHandle(snap);
}
static BOOL CALLBACK env_win(HWND w, LPARAM lp) {
  DWORD pid = 0; GetWindowThreadProcessId(w, &pid);
  if (pid != GetCurrentProcessId() || !IsWindowVisible(w)) return TRUE;
  char cls[128] = "", title[128] = ""; GetClassNameA(w, cls, sizeof cls); GetWindowTextA(w, title, sizeof title);
  RECT r, c; GetWindowRect(w, &r); GetClientRect(w, &c);
  envp("  window %p class \"%s\" title \"%s\": %ldx%ld at %ld,%ld, client %ldx%ld, style %08lX exstyle %08lX", (void *)w, cls, title,
       r.right - r.left, r.bottom - r.top, r.left, r.top, c.right, c.bottom, (unsigned long)GetWindowLongA(w, GWL_STYLE), (unsigned long)GetWindowLongA(w, GWL_EXSTYLE));
  (void)lp; return TRUE;
}
static DWORD WINAPI m_envthread(LPVOID u) {
  (void)u;
  char p[MAX_PATH]; snprintf(p, sizeof p, "%s\\xikit_env_%lu.txt", g_out, (unsigned long)GetCurrentProcessId());
  m_envf = fopen(p, "w"); if (!m_envf) return 0;
  SYSTEMTIME st; GetLocalTime(&st);
  envp("# xikit environment, %04u-%02u-%02u %02u:%02u:%02u, process %lu", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, (unsigned long)GetCurrentProcessId());
  HMODULE nt = GetModuleHandleA("ntdll.dll");
  typedef const char *(CDECL *WGV)(void); typedef void (CDECL *WGHV)(const char **, const char **);
  WGV wv = (WGV)(void *)GetProcAddress(nt, "wine_get_version"), wb = (WGV)(void *)GetProcAddress(nt, "wine_get_build_id");
  WGHV wh = (WGHV)(void *)GetProcAddress(nt, "wine_get_host_version");
  if (wv) { const char *sys = "?", *rel = "?"; if (wh) wh(&sys, &rel); envp("[wine] version %s, build %s, host %s %s", wv(), wb ? wb() : "?", sys, rel); }
  else envp("[wine] not Wine (ntdll has no wine_get_version)");
  SYSTEM_INFO si; GetNativeSystemInfo(&si);
  envp("[machine] %lu logical processors (native), arch %u; this process's affinity:", (unsigned long)si.dwNumberOfProcessors, si.wProcessorArchitecture);
  DWORD_PTR pa = 0, sa = 0; GetProcessAffinityMask(GetCurrentProcess(), &pa, &sa);
  envp("  process %08lX, system %08lX; priority class %08lX", (unsigned long)pa, (unsigned long)sa, (unsigned long)GetPriorityClass(GetCurrentProcess()));
  MEMORYSTATUSEX mx; mx.dwLength = sizeof mx; if (GlobalMemoryStatusEx(&mx)) envp("  memory %.0f MB total, %.0f MB free; this process's address space %.0f MB free", mx.ullTotalPhys / 1048576.0, mx.ullAvailPhys / 1048576.0, mx.ullAvailVirtual / 1048576.0);
  DEVMODEA dm; memset(&dm, 0, sizeof dm); dm.dmSize = sizeof dm;
  if (EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &dm)) envp("[display] %lux%lu, %lu bits, %lu Hz", (unsigned long)dm.dmPelsWidth, (unsigned long)dm.dmPelsHeight, (unsigned long)dm.dmBitsPerPel, (unsigned long)dm.dmDisplayFrequency);
  DISPLAY_DEVICEA dd; dd.cb = sizeof dd;
  for (DWORD i = 0; EnumDisplayDevicesA(NULL, i, &dd, 0); i++) envp("  adapter %lu: \"%s\" (%s), flags %08lX", (unsigned long)i, dd.DeviceString, dd.DeviceName, (unsigned long)dd.StateFlags);
  EnumWindows(env_win, 0);
  /* the game's settings: PlayOnline's keys (the 32-bit view: this process is 32-bit) */
  static const char *const keys[] = {"SOFTWARE\\PlayOnlineUS\\SquareEnix\\FinalFantasyXI", "SOFTWARE\\PlayOnlineEU\\SquareEnix\\FinalFantasyXI",
                                     "SOFTWARE\\PlayOnline\\SquareEnix\\FinalFantasyXI", "SOFTWARE\\PlayOnlineUS\\InstallFolder", "SOFTWARE\\PlayOnlineUS\\SquareEnix\\PlayOnlineViewer\\Settings"};
  for (unsigned i = 0; i < sizeof keys / sizeof *keys; i++) { env_regkey(HKEY_LOCAL_MACHINE, "HKLM", keys[i], 1); env_regkey(HKEY_CURRENT_USER, "HKCU", keys[i], 1); }
  /* Wine's own settings, where they are */
  static const char *const wk[] = {"Software\\Wine\\Direct3D", "Software\\Wine\\DllOverrides", "Software\\Wine\\Mac Driver", "Software\\Wine\\X11 Driver", "Software\\Wine\\AppDefaults\\pol.exe\\Direct3D", "Software\\Wine\\AppDefaults\\pol.exe\\DllOverrides"};
  for (unsigned i = 0; i < sizeof wk / sizeof *wk; i++) env_regkey(HKEY_CURRENT_USER, "HKCU", wk[i], 0);
  envp("[environment variables that name Wine, CrossOver, DXVK, Metal, Vulkan, MoltenVK, dgVoodoo, Rosetta]");
  char *envs = GetEnvironmentStringsA();
  if (envs) {
    for (char *e = envs; *e; e += strlen(e) + 1) {
      char up[64]; int i = 0; for (; e[i] && e[i] != '=' && i < 63; i++) up[i] = (char)(e[i] >= 'a' && e[i] <= 'z' ? e[i] - 32 : e[i]); up[i] = 0;
      if (strstr(up, "WINE") || strstr(up, "CX_") || strstr(up, "DXVK") || strstr(up, "MTL") || strstr(up, "VK_") || strstr(up, "MVK") ||
          strstr(up, "D3DM") || strstr(up, "DGVOODOO") || strstr(up, "ROSETTA") || strstr(up, "__COMPAT") || strstr(up, "PROCESSOR") || strstr(up, "NUMBER_OF"))
        envp("  %s", e);
    }
    FreeEnvironmentStringsA(envs);
  }
  envp("[files copied into this folder]");
  char exe[MAX_PATH]; GetModuleFileNameA(NULL, exe, sizeof exe); char *s = strrchr(exe, '\\'); if (s) *s = 0;
  char d8[MAX_PATH] = ""; HMODULE hd8 = GetModuleHandleA("d3d8.dll"); if (hd8) { GetModuleFileNameA(hd8, d8, sizeof d8); s = strrchr(d8, '\\'); if (s) *s = 0; envp("  d3d8.dll is %s\\d3d8.dll", d8); }
  const char *dirs[3] = {exe, d8, g_dir};
  static const char *const files[] = {"dgVoodoo.conf", "d3d8on11.ini", "speedfix.ini", "d3dprof.ini", "xikit.ini", "dxvk.conf", "xikit_phase.txt"};
  for (int i = 0; i < 3; i++) if (dirs[i][0] && (i == 0 || strcmp(dirs[i], dirs[0])) && (i < 2 || (strcmp(dirs[i], dirs[1]))))
    for (unsigned k = 0; k < sizeof files / sizeof *files; k++) env_copy(dirs[i], files[k]);
  envp("[modules at the start]"); env_modules();
  Sleep(2000);                                /* the call costs below go through system-call stubs: after the kit's own setup has settled */
  env_timers("at the start");
  env_bench();
  Sleep(60000); env_timers("60 s in");
  envp("[modules 60 s in]"); env_modules();
  Sleep(240000); env_timers("300 s in");
  return 0;
}

/* ------------------------------------------------------------------------------------------------ start */
static void measure_start(void) {
  m_frames = GetPrivateProfileIntA("xikit", "frames", 0, g_ini);
  m_imports = GetPrivateProfileIntA("xikit", "imports", 0, g_ini);
  m_callsites = GetPrivateProfileIntA("xikit", "callsites", 0, g_ini);
  m_callsite_max = GetPrivateProfileIntA("xikit", "callsite_max", 500, g_ini);
  m_sysstats = GetPrivateProfileIntA("xikit", "sysstats", 0, g_ini);
  m_env = GetPrivateProfileIntA("xikit", "env", 1, g_ini);
  m_rawsample = GetPrivateProfileIntA("xikit", "sample", 0, g_ini) == 3;
  m_sample_ms = GetPrivateProfileIntA("xikit", "sample_ms", 1, g_ini); if (m_sample_ms < 1) m_sample_ms = 1;
  m_other_ms = GetPrivateProfileIntA("xikit", "sample_other_ms", 5, g_ini); if (m_other_ms < 1) m_other_ms = 1;
  m_stack_rt = GetPrivateProfileIntA("xikit", "sample_stack", 16384, g_ini);
  m_stack_other = GetPrivateProfileIntA("xikit", "sample_other_stack", 4096, g_ini);
  m_raw_max = (uint64_t)GetPrivateProfileIntA("xikit", "raw_max_mb", 3000, g_ini) << 20;
  m_qtct = (QTCT)(void *)GetProcAddress(GetModuleHandleA("kernel32.dll"), "QueryThreadCycleTime");
  m_nqit = (NQIT)(void *)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationThread");
  if (!g_qpf.QuadPart) QueryPerformanceFrequency(&g_qpf);
  QueryPerformanceCounter(&m_q0); m_t0 = m_tsc();
  { LARGE_INTEGER a, b; QueryPerformanceCounter(&a); do QueryPerformanceCounter(&b); while (m_qms(a, b) < 100); m_cal(); }
  int any = m_frames || m_imports || m_callsites || m_sysstats || m_rawsample;
  if (m_env) { HANDLE h = CreateThread(NULL, 0, m_envthread, NULL, 0, NULL); if (h) CloseHandle(h); }
  if (!any) return;
  if (!g_nrd) { readable_ranges(g_game_lo, g_game_hi); readable_ranges(g_recon_lo, g_recon_hi); }
  if (m_frames) {
    char head[2048]; int n = snprintf(head, sizeof head,
      "# xikit frames: one line per Present on the render thread. qpc = QueryPerformanceCounter at the kit's Present (frequency %lld);"
      " frame_ms = Present to Present; present_ms = inside the device's Present; kit_ms = the kit's own time before it; rt_cycles and rt_cpu_ms = the render"
      " thread's CPU since the last frame (QueryThreadCycleTime, GetThreadTimes); pf = page faults of the process; mark = Ctrl+Alt+F7 count;"
      " p28..p3c = Frame_Render's this+0x28, +0x2C, +0x30, +0x38, +0x3C raw; ring0..3 = 0x10456964.. raw; tfps = [[this+0x1C]+4] raw (docs/RENDERER.md 3)%s\n"
      "frame,qpc,frame_ms,present_ms,kit_ms,rt_cycles,rt_cpu_ms,pf,mark,swaps_off,p28,p2c,p30,p38,p3c,ring0,ring1,ring2,ring3,tfps",
      (long long)g_qpf.QuadPart, m_imports || m_callsites ? "; then per group of wrapped calls on the render thread this frame: exclusive ms and calls; the longest wrapped import call" : "");
    if (m_imports || m_callsites) { for (int g = 0; g < G_N; g++) n += snprintf(head + n, sizeof head - n, ",%s_ms,%s_calls", m_gname[g], m_gname[g]); n += snprintf(head + n, sizeof head - n, ",longest_import,longest_ms"); }
    snprintf(head + n, sizeof head - n, "\n");
    ms_open(&m_sframes, "xikit_frames", 4 << 20, head);
  }
  if (m_imports || m_callsites) {
    m_p = (Probe *)VirtualAlloc(NULL, sizeof(Probe) * (MP_MAX + 1), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    m_h = (HEnt *)VirtualAlloc(NULL, sizeof(HEnt) * MP_HASH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    m_stubs = (uint8_t *)VirtualAlloc(NULL, 12 * (MP_MAX + 1), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    m_tls = TlsAlloc();
    if (!m_p || !m_h || !m_stubs || m_tls == TLS_OUT_OF_INDEXES) { klog("measure: could not reserve the call timing's tables"); m_imports = m_callsites = 0; }
    else {
      m_load_table();
      double cost = m_cost_ns = m_probe_cost_ns();
      DWORD old; VirtualProtect(m_stubs, 12 * (MP_MAX + 1), PAGE_EXECUTE_READ, &old); FlushInstructionCache(GetCurrentProcess(), m_stubs, 12 * (MP_MAX + 1));
      klog("measure: a wrapped call costs %.0f ns more than a plain call (20,000 calls of a dummy through a stub, on this thread); TSC %.3f ticks per ms", cost, m_tsc_per_ms);
      char head[512]; snprintf(head, sizeof head, "# xikit probe frames: frame,k,calls,incl,excl,max of each wrapped call the render thread made in that frame, in TSC ticks"
                                                   " (#cal lines: qpc,tsc,ticks per ms); k is the k of xikit_probes_<pid>.csv\n");
      ms_open(&m_sprobe, "xikit_probe_frames", 8 << 20, head);
      char lp[MAX_PATH]; snprintf(lp, sizeof lp, "%s\\xikit_probe_list_%lu.csv", g_out, (unsigned long)GetCurrentProcessId());
      FILE *f = fopen(lp, "w");
      if (f) { fprintf(f, "k,kind,name,analysed,target,level,group\n"); for (int k = 0; k < m_np; k++) fprintf(f, "%d,%c,%s,%08X,%08X,%d,%s\n", k, m_p[k].kind, m_p[k].name, m_p[k].ea, m_p[k].tea, m_p[k].level, m_gname[m_p[k].group]); fclose(f); }
    }
  }
  if (m_sysstats) {
    ms_open(&m_ssys, "xikit_sys", 1 << 20, "# xikit sys: each second. proc_user_ms, proc_kernel_ms = this process's CPU in that second (all threads); machine_busy_pct, machine_kernel_pct = every"
            " processor (GetSystemTimes); ws_mb, private_mb; pf = page faults; I/O operations and MB (GetProcessIoCounters)\n"
            "qpc,frame,frames,proc_user_ms,proc_kernel_ms,machine_busy_pct,machine_kernel_pct,ws_mb,private_mb,pf,read_ops,read_mb,write_ops,write_mb,other_ops,other_mb,handles\n");
    ms_open(&m_sthreads, "xikit_threads", 1 << 20, "# xikit threads: every 2 s, each thread: its start address, priority, CPU in the last 2 s (cycles in millions; user and kernel ms)\n"
            "qpc,tid,start,role,priority,mcycles,user_ms,kernel_ms\n");
  }
  if (m_rawsample) {
    m_rg = (Rg *)VirtualAlloc(NULL, sizeof(Rg) * 16384, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (m_rg && ms_open(&m_sraw, "xikit_raw", 16 << 20, NULL)) {
      uint8_t h[128]; memset(h, 0, sizeof h); memcpy(h, "XIRAW1\0\0", 8);
      uint32_t v[8] = {1, (uint32_t)GetCurrentProcessId(), g_rel, g_game_lo, g_game_hi, g_recon_lo, g_recon_hi, (uint32_t)(uintptr_t)m_land};
      memcpy(h + 8, v, 32); memcpy(h + 40, &g_qpf.QuadPart, 8); memcpy(h + 48, g_shift, sizeof g_shift < 80 ? sizeof g_shift : 80);
      fwrite(h, 1, 128, m_sraw.f); fflush(m_sraw.f);
    } else m_rawsample = 0;
  }
  HANDLE hw = CreateThread(NULL, 0, m_writer, NULL, 0, &m_own[m_nown]); if (hw) { m_nown++; CloseHandle(hw); }
  if (m_sysstats) { HANDLE h = CreateThread(NULL, 0, m_sys, NULL, 0, &m_own[m_nown]); if (h) { m_nown++; CloseHandle(h); } }
  if (m_rawsample) {
    HANDLE h = CreateThread(NULL, 0, m_sampler, NULL, CREATE_SUSPENDED, &m_own[m_nown]);
    if (h) { m_nown++; SetThreadPriority(h, THREAD_PRIORITY_TIME_CRITICAL); ResumeThread(h); CloseHandle(h); }
    klog("sample=3: every thread of the process, the render thread every %d ms, the others every %d ms when they ran (xikit_raw_<pid>.bin; kit/xikit/measure_read.py)", m_sample_ms, m_other_ms);
  }
  klog("measure: frames=%d imports=%d callsites=%d sysstats=%d sample=%d env=%d", m_frames, m_imports, m_callsites, m_sysstats, m_rawsample ? 3 : 0, m_env);
  m_started = 1;
}
