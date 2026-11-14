// d8_common.cpp -- d3d8on11: the log and the formats, shared by the D3D8 side (d3d8.dll) and the executor (in it, or in
// d8x.exe for the executor process).
#include "d8_common.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

const char *d8_log_name = "d3d8on11.log";
// ---- log --------------------------------------------------------------------------------------------------------------
static FILE *g_log; static CRITICAL_SECTION g_logLock; static bool g_logInit;
thread_local int d8_stats_depth; static bool g_logStats = true;
bool d8_log_stats_on() { return g_logStats; }
static void log_open() {
  if (g_logInit) return;
  g_logInit = true; InitializeCriticalSection(&g_logLock);
  char p[MAX_PATH]; HMODULE m = NULL;
  GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&log_open, &m);
  GetModuleFileNameA(m, p, sizeof p);
  char *s = strrchr(p, '\\');
  { char ini[MAX_PATH + 16]; snprintf(ini, sizeof ini, "%.*s\\d3d8on11.ini", s ? (int)(s - p) : 0, p);
    g_logStats = GetPrivateProfileIntA("d3d8on11", "log_stats", 1, ini) != 0;
    char e[8]; DWORD n = GetEnvironmentVariableA("D8ON11_LOG_STATS", e, sizeof e); if (n > 0 && n < sizeof e) g_logStats = atoi(e) != 0; }
  strcpy(s ? s + 1 : p, d8_log_name);
  // the last start's log is kept beside it (_prev): a game started again without start.bat wrote over it (21 Nov,
  // the owner's first adv11 start)
  { char q[MAX_PATH]; snprintf(q, sizeof q, "%s", p); char *e = strrchr(q, '.'); if (e && (size_t)(e - q) + 10 < sizeof q) { strcpy(e, "_prev.log"); MoveFileExA(p, q, MOVEFILE_REPLACE_EXISTING); } }
  g_log = fopen(p, "w");
}
void d8_log(const char *fmt, ...) {
  log_open(); if (!g_log) return;
  if (d8_stats_depth > 0 && !g_logStats) return;
  EnterCriticalSection(&g_logLock);
  va_list a; va_start(a, fmt); vfprintf(g_log, fmt, a); va_end(a); fputc('\n', g_log); fflush(g_log);
  LeaveCriticalSection(&g_logLock);
}
static std::set<std::string> g_seen;
void d8_once(const char *key, const char *fmt, ...) {
  log_open();
  EnterCriticalSection(&g_logLock);
  bool first = g_seen.insert(key).second;
  LeaveCriticalSection(&g_logLock);
  if (!first) return;
  char b[1024]; va_list a; va_start(a, fmt); vsnprintf(b, sizeof b, fmt, a); va_end(a);
  d8_log("%s", b);
}
const char *d8_caller(void *ra, char *buf, size_t n, char *mod, size_t nmod) {
  HMODULE m = NULL; char path[MAX_PATH] = "?";
  if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)ra, &m) && m)
    GetModuleFileNameA(m, path, sizeof path);
  const char *base = strrchr(path, '\\'); base = base ? base + 1 : path;
  snprintf(buf, n, "%s+0x%lX", base, (unsigned long)((uintptr_t)ra - (uintptr_t)m));
  if (mod && nmod) snprintf(mod, nmod, "%s", base);
  return buf;
}
void d8_notimpl_at(const char *what, void *ra) {
  char at[MAX_PATH + 16], mod[MAX_PATH]; d8_caller(ra, at, sizeof at, mod, sizeof mod);
  d8_once((std::string(what) + "@" + mod).c_str(), "not implemented: %s (failed), called from %s", what, at);
}

// ---- formats (D3DFORMAT numbers from d3d8types.h; DXGI from dxgiformat.h) ------------------------------------------------
FmtInfo d8_fmt(D3DFORMAT f) {
  switch ((DWORD)f) {
  case D3DFMT_A8R8G8B8: return {DXGI_FORMAT_B8G8R8A8_UNORM, 32, 0, false};
  case D3DFMT_X8R8G8B8: return {DXGI_FORMAT_B8G8R8X8_UNORM, 32, 0, false};
  case D3DFMT_R5G6B5: return {DXGI_FORMAT_B5G6R5_UNORM, 16, 0, false};
  case D3DFMT_A1R5G5B5: return {DXGI_FORMAT_B5G5R5A1_UNORM, 16, 0, false};
  case D3DFMT_A4R4G4B4: return {DXGI_FORMAT_B4G4R4A4_UNORM, 16, 0, false};
  case D3DFMT_A8: return {DXGI_FORMAT_A8_UNORM, 8, 0, false};
  case D3DFMT_DXT1: return {DXGI_FORMAT_BC1_UNORM, 0, 8, false};
  case D3DFMT_DXT2: case D3DFMT_DXT3: return {DXGI_FORMAT_BC2_UNORM, 0, 16, false};
  case D3DFMT_DXT4: case D3DFMT_DXT5: return {DXGI_FORMAT_BC3_UNORM, 0, 16, false};
  case D3DFMT_D24S8: case D3DFMT_D24X8: return {DXGI_FORMAT_D24_UNORM_S8_UINT, 32, 0, true};
  case D3DFMT_D16: return {DXGI_FORMAT_D16_UNORM, 16, 0, true};
  case D3DFMT_D32: return {DXGI_FORMAT_D32_FLOAT, 32, 0, true};
  case D3DFMT_INDEX16: return {DXGI_FORMAT_R16_UINT, 16, 0, false};
  case D3DFMT_INDEX32: return {DXGI_FORMAT_R32_UINT, 32, 0, false};
  }
  return {DXGI_FORMAT_UNKNOWN, 0, 0, false};
}
UINT d8_pitch(D3DFORMAT f, UINT w) { FmtInfo i = d8_fmt(f); return i.block ? ((w + 3) / 4) * i.block : w * (i.bpp / 8); }
UINT d8_rows(D3DFORMAT f, UINT h) { return d8_fmt(f).block ? (h + 3) / 4 : h; }


// ---- readbacks by call site (18 Nov) ----------------------------------------------------------------------------------------
#include <map>
thread_local void *d8_rb_site;
static CRITICAL_SECTION g_rb_cs; static bool g_rb_init;
static std::map<void *, std::pair<unsigned long, double>> g_rb;
void d8_rb_count(double ms) {
  if (!g_rb_init) { InitializeCriticalSection(&g_rb_cs); g_rb_init = true; }
  EnterCriticalSection(&g_rb_cs); auto &e = g_rb[d8_rb_site]; e.first++; e.second += ms; LeaveCriticalSection(&g_rb_cs);
}
void d8_rb_log(unsigned frames) {
  if (!g_rb_init) return;
  EnterCriticalSection(&g_rb_cs);
  std::vector<std::pair<void *, std::pair<unsigned long, double>>> v(g_rb.begin(), g_rb.end()); g_rb.clear();
  LeaveCriticalSection(&g_rb_cs);
  if (v.empty()) return;
  std::sort(v.begin(), v.end(), [](const auto &a, const auto &b) { return a.second.second > b.second.second; });
  char line[1200]; size_t at = 0;
  for (size_t i = 0; i < v.size() && i < 6 && at < sizeof line - 160; i++) {
    char w[MAX_PATH + 16]; d8_caller(v[i].first, w, sizeof w);
    at += snprintf(line + at, sizeof line - at, "%s%s: %lu, %.1f ms", i ? "; " : "", v[i].first ? w : "(no site)", v[i].second.first, v[i].second.second);
  }
  d8_log("readbacks by call site, %u frames (each waits for the GPU): %s", frames, line);
}

// ---- crash reporting (21 Nov; the owner's two Direct3D 11 starts ended with nothing in the log) ----------------------
// A vectored handler sees every exception first, also those the code that raised them handles (the game, Windows,
// the drivers may use some on purpose): each fatal-kind code is logged as "first chance (it may be handled)", once per
// address, at most 24 lines. An unhandled-exception filter, chained to the one before it, logs "unhandled". With the
// address: its module and offset, the thread, the registers, and the values on the stack that point into a module's
// code (a scan of 1 KB of stack, not an unwound call stack: some are stale). Each line is flushed at once.
static LONG g_crash_lines; static LPTOP_LEVEL_EXCEPTION_FILTER g_prev_filter;
static bool crash_kind(DWORD c) {
  return c == EXCEPTION_ACCESS_VIOLATION || c == EXCEPTION_STACK_OVERFLOW || c == EXCEPTION_ILLEGAL_INSTRUCTION ||
         c == EXCEPTION_PRIV_INSTRUCTION || c == EXCEPTION_INT_DIVIDE_BY_ZERO || c == 0xC0000409u || c == 0xC0000374u ||
         c == EXCEPTION_ARRAY_BOUNDS_EXCEEDED || c == EXCEPTION_IN_PAGE_ERROR;
}
static bool in_code(uintptr_t v) {
  HMODULE m = NULL; MEMORY_BASIC_INFORMATION mb;
  if (v < 0x10000 || !VirtualQuery((void *)v, &mb, sizeof mb) || mb.State != MEM_COMMIT || !(mb.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))) return false;
  return GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)v, &m) && m;
}
static void crash_report(EXCEPTION_POINTERS *e, const char *kind) {
  static uintptr_t seen[24]; static LONG nseen;
  uintptr_t at = (uintptr_t)e->ExceptionRecord->ExceptionAddress;
  for (LONG i = 0; i < nseen && i < 24; i++) if (seen[i] == at && kind[0] == 'f') return;
  LONG n = InterlockedIncrement(&g_crash_lines); if (n > 24) return;
  if (nseen < 24) seen[nseen++] = at;
  char where[MAX_PATH + 16]; d8_caller((void *)at, where, sizeof where);
  const EXCEPTION_RECORD *r = e->ExceptionRecord;
  char extra[64] = "";
  if (r->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && r->NumberParameters >= 2)
    snprintf(extra, sizeof extra, " (%s 0x%08lX)", r->ExceptionInformation[0] == 0 ? "reading" : r->ExceptionInformation[0] == 8 ? "executing" : "writing", (unsigned long)r->ExceptionInformation[1]);
#if defined(__i386__)
  const CONTEXT *c = e->ContextRecord;
  d8_log("exception 0x%08lX%s at %s, thread %lu, %s; eax %08lX ebx %08lX ecx %08lX edx %08lX esi %08lX edi %08lX ebp %08lX esp %08lX",
         (unsigned long)r->ExceptionCode, extra, where, (unsigned long)GetCurrentThreadId(), kind, c->Eax, c->Ebx, c->Ecx, c->Edx, c->Esi, c->Edi, c->Ebp, c->Esp);
  uintptr_t sp = c->Esp;
#else
  d8_log("exception 0x%08lX%s at %s, thread %lu, %s", (unsigned long)r->ExceptionCode, extra, where, (unsigned long)GetCurrentThreadId(), kind);
#if defined(__x86_64__)
  uintptr_t sp = (uintptr_t)e->ContextRecord->Rsp;
#elif defined(__aarch64__)
  uintptr_t sp = (uintptr_t)e->ContextRecord->Sp;
#else
  uintptr_t sp = 0;
#endif
#endif
  if (r->ExceptionCode == EXCEPTION_STACK_OVERFLOW || !sp) return;      // (little stack left)
  char line[640]; size_t L = 0; int k = 0;
  L += snprintf(line + L, sizeof line - L, "  stack values pointing into code (a scan, not an unwound stack):");
  for (int i = 0; i < 256 && k < 12 && L < sizeof line - 64; i++) {
    uintptr_t v; MEMORY_BASIC_INFORMATION mb;
    uintptr_t a = sp + (uintptr_t)i * sizeof(void *);
    if (!VirtualQuery((void *)a, &mb, sizeof mb) || mb.State != MEM_COMMIT || (mb.Protect & (PAGE_NOACCESS | PAGE_GUARD))) break;
    v = *(uintptr_t *)a;
    if (in_code(v)) { char w[MAX_PATH + 16]; d8_caller((void *)v, w, sizeof w); L += snprintf(line + L, sizeof line - L, " %s", w); k++; }
  }
  d8_log("%s", line);
}
static LONG CALLBACK crash_vectored(EXCEPTION_POINTERS *e) {
  if (e && e->ExceptionRecord && crash_kind(e->ExceptionRecord->ExceptionCode)) crash_report(e, "first chance (it may be handled)");
  return EXCEPTION_CONTINUE_SEARCH;
}
static LONG WINAPI crash_filter(EXCEPTION_POINTERS *e) {
  if (e && e->ExceptionRecord) crash_report(e, "unhandled: the process ends here unless an earlier filter handles it");
  return g_prev_filter ? g_prev_filter(e) : EXCEPTION_CONTINUE_SEARCH;
}
void d8_crash_install() {
  static bool done; if (done) return; done = true;
  AddVectoredExceptionHandler(0, crash_vectored);
  g_prev_filter = SetUnhandledExceptionFilter(crash_filter);
  d8_log("crash reporting: exceptions of the fatal kinds logged with their module and offset (first chance, and unhandled)");
}
