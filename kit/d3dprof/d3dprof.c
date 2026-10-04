/* d3dprof.c -- a per-frame profile of the game's Direct3D 8 calls, taken inside the running game.

   Loaded into the game process (by whatever loader the owner uses; DllMain only starts a thread), it:
     1. checks that FFXiMain.dll is the analysed build (PE TimeDateStamp 0x6A995428, data/binary.json) and
        does nothing otherwise;
     2. waits for the device: [[g_pCDx]+0xC], g_pCDx at 0x1045666C in the analysed image (CDx.pDevice, the
        accepted declaration in data/com_fields.json; the four dumps agree with it), rebased to where the
        game's DLL is loaded;
     3. gives that one device object a copy of its vtable whose 97 entries (the D3D8 header's
        IDirect3DDevice8 order, data/com_spec.json) go through a counting thunk to the original methods.
        The driver's own vtable is not written.
   Every call is counted per (method, call site); the call site is the return address, written as the
   analysed build's address (0x10xxxxxx) so it matches data/render_map.json. Per frame (Present to
   Present) it also counts state sets that repeat the value last set (SetRenderState,
   SetTextureStageState, SetTexture, SetVertexShader, SetStreamSource, SetIndices), primitives drawn, and
   the frame time. Every 600 frames it rewrites d3dprof_<pid>.csv next to this DLL.

   Only calls from the thread that made the first call are profiled; any other thread's calls are passed
   through and counted in other_thread_calls (the header says nothing about D3D8 thread use here, so it
   is measured, not assumed).

   Build: i686-w64-mingw32-gcc -O2 -shared -o d3dprof.dll d3dprof.c -Wl,--kill-at  (kit/d3dprof/build.sh) */
#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#define ANALYSED_BASE 0x10000000u
#define ANALYSED_TIMESTAMP 0x6A995428u
#define G_PCDX 0x1045666Cu
#define CDX_DEVICE_OFF 0xC
#define NSLOTS 97
#define MAXSITES 8192
#define REPORT_EVERY 600

static const char *NAMES[NSLOTS] = {
  "QueryInterface", "AddRef", "Release", "TestCooperativeLevel", "GetAvailableTextureMem",
  "ResourceManagerDiscardBytes", "GetDirect3D", "GetDeviceCaps", "GetDisplayMode", "GetCreationParameters",
  "SetCursorProperties", "SetCursorPosition", "ShowCursor", "CreateAdditionalSwapChain", "Reset", "Present",
  "GetBackBuffer", "GetRasterStatus", "SetGammaRamp", "GetGammaRamp", "CreateTexture", "CreateVolumeTexture",
  "CreateCubeTexture", "CreateVertexBuffer", "CreateIndexBuffer", "CreateRenderTarget",
  "CreateDepthStencilSurface", "CreateImageSurface", "CopyRects", "UpdateTexture", "GetFrontBuffer",
  "SetRenderTarget", "GetRenderTarget", "GetDepthStencilSurface", "BeginScene", "EndScene", "Clear",
  "SetTransform", "GetTransform", "MultiplyTransform", "SetViewport", "GetViewport", "SetMaterial",
  "GetMaterial", "SetLight", "GetLight", "LightEnable", "GetLightEnable", "SetClipPlane", "GetClipPlane",
  "SetRenderState", "GetRenderState", "BeginStateBlock", "EndStateBlock", "ApplyStateBlock",
  "CaptureStateBlock", "DeleteStateBlock", "CreateStateBlock", "SetClipStatus", "GetClipStatus", "GetTexture",
  "SetTexture", "GetTextureStageState", "SetTextureStageState", "ValidateDevice", "GetInfo",
  "SetPaletteEntries", "GetPaletteEntries", "SetCurrentTexturePalette", "GetCurrentTexturePalette",
  "DrawPrimitive", "DrawIndexedPrimitive", "DrawPrimitiveUP", "DrawIndexedPrimitiveUP", "ProcessVertices",
  "CreateVertexShader", "SetVertexShader", "GetVertexShader", "DeleteVertexShader", "SetVertexShaderConstant",
  "GetVertexShaderConstant", "GetVertexShaderDeclaration", "GetVertexShaderFunction", "SetStreamSource",
  "GetStreamSource", "SetIndices", "GetIndices", "CreatePixelShader", "SetPixelShader", "GetPixelShader",
  "DeletePixelShader", "SetPixelShaderConstant", "GetPixelShaderConstant", "GetPixelShaderFunction",
  "DrawRectPatch", "DrawTriPatch", "DeletePatch"};
enum { S_PRESENT = 15, S_SETRT = 31, S_BEGIN = 34, S_END = 35, S_RS = 50, S_STATEBLOCK_APPLY = 54,
       S_SETTEX = 61, S_TSS = 63, S_DP = 70, S_DIP = 71, S_DPUP = 72, S_DIPUP = 73, S_SETVS = 76,
       S_SSS = 83, S_SETIND = 85 };

static void **g_orig;                 /* the device's own vtable */
static void *g_vt[NSLOTS];            /* the copy the device is given */
static uint32_t g_rel;                /* load address - analysed address */
static uint32_t g_lo, g_hi;           /* the game DLL's address range as loaded */
static int g_recording;               /* between BeginStateBlock and EndStateBlock: Set* calls are recorded
                                         into the block, not applied to the device (D3D8 documentation) */
static DWORD g_thread;
static char g_out[MAX_PATH];

/* per (slot, site) totals over the report window */
typedef struct { uint32_t site; uint16_t slot; uint16_t used; uint32_t calls, redundant; uint64_t prims; } Site;
static Site g_sites[MAXSITES];
static uint32_t g_nsites, g_dropped_sites, g_other_thread;
/* the value each state was last set to (0xFFFFFFFF never set; reset by ApplyStateBlock, see below) */
static uint32_t g_rs[256], g_tss[8][32], g_vs, g_ib;
static void *g_tex[8], *g_vb[16];
static uint32_t g_vbstride[16];
/* frame totals */
static uint32_t g_frames;
static uint64_t g_calls_window, g_draws_window, g_prims_window, g_redundant_window;
static LARGE_INTEGER g_qpf, g_last_present;
static double g_ms_sum, g_ms_max, g_ms_min = 1e9;

static Site *site_of(uint32_t site, int slot) {
  uint32_t h = (site * 2654435761u ^ (uint32_t)slot * 40503u) & (MAXSITES - 1);
  for (uint32_t n = 0; n < MAXSITES; n++, h = (h + 1) & (MAXSITES - 1)) {
    Site *s = &g_sites[h];
    if (!s->used) {
      if (g_nsites >= MAXSITES * 3 / 4) { g_dropped_sites++; return NULL; }
      s->used = 1; s->site = site; s->slot = (uint16_t)slot; g_nsites++; return s;
    }
    if (s->site == site && s->slot == slot) return s;
  }
  return NULL;
}

static void forget_state(void) {
  memset(g_rs, 0xFF, sizeof g_rs); memset(g_tss, 0xFF, sizeof g_tss); memset(g_tex, 0xFF, sizeof g_tex);
  memset(g_vb, 0xFF, sizeof g_vb); g_vs = g_ib = 0xFFFFFFFFu;
}

static void report(void) {
  FILE *f = fopen(g_out, "w");
  if (!f) return;
  double n = g_frames ? (double)g_frames : 1.0;
  fprintf(f, "# d3dprof: %u frames; frame ms avg %.3f min %.3f max %.3f; per frame: calls %.1f draws %.1f "
             "primitives %.1f redundant_sets %.1f; other_thread_calls %u; sites %u (dropped %u)\n",
          g_frames, g_ms_sum / n, g_ms_min, g_ms_max, g_calls_window / n, g_draws_window / n, g_prims_window / n,
          g_redundant_window / n, g_other_thread, g_nsites, g_dropped_sites);
  fprintf(f, "method,call_site,caller,calls_per_frame,redundant_per_frame,primitives_per_frame\n");
  for (uint32_t i = 0; i < MAXSITES; i++) {
    Site *s = &g_sites[i];
    if (!s->used) continue;
    fprintf(f, "%s,0x%08X,%s,%.3f,%.3f,%.1f\n", NAMES[s->slot], s->site & ~1u, (s->site & 1) ? "other module" : "FFXiMain",
            s->calls / n, s->redundant / n, s->prims / n);
  }
  fclose(f);
  memset(g_sites, 0, sizeof g_sites); g_nsites = g_dropped_sites = 0;
  g_frames = 0; g_calls_window = g_draws_window = g_prims_window = g_redundant_window = 0;
  g_ms_sum = g_ms_max = 0; g_ms_min = 1e9;
}

/* called by the thunk with the method's slot and the caller's stack (a[0] = return address, a[1] = this,
   a[2...] = the method's parameters); returns the original method, which the thunk jumps to */
void *__cdecl d3dprof_record(int slot, uint32_t *a) {
  if (g_thread == 0) g_thread = GetCurrentThreadId();
  if (GetCurrentThreadId() != g_thread) { g_other_thread++; return g_orig[slot]; }
  /* a call site in the game's DLL as the analysed address; any other caller as it is, with bit 0 set (an x86
     call returns to an address the call instruction ends at, so the bit is free only as a marker for the CSV) */
  uint32_t ret = a[0] >= g_lo && a[0] < g_hi ? a[0] - g_rel : a[0] | 1u;
  int redundant = 0; uint32_t prims = 0;
  if (slot == 52) g_recording = 1;
  else if (slot == 53) g_recording = 0;
  if (!g_recording) switch (slot) {
  case S_RS: if (a[2] < 256) { redundant = g_rs[a[2]] == a[3]; g_rs[a[2]] = a[3]; } break;
  case S_TSS: if (a[2] < 8 && a[3] < 32) { redundant = g_tss[a[2]][a[3]] == a[4]; g_tss[a[2]][a[3]] = a[4]; } break;
  case S_SETTEX: if (a[2] < 8) { redundant = g_tex[a[2]] == (void *)a[3]; g_tex[a[2]] = (void *)a[3]; } break;
  case S_SETVS: redundant = g_vs == a[2]; g_vs = a[2]; break;
  case S_SETIND: redundant = g_ib == a[2]; g_ib = a[2]; break;
  case S_SSS: if (a[2] < 16) { redundant = g_vb[a[2]] == (void *)a[3] && g_vbstride[a[2]] == a[4];
                               g_vb[a[2]] = (void *)a[3]; g_vbstride[a[2]] = a[4]; } break;
  /* a state block, a Reset or a new render target may set any state: what was last set is no longer known */
  case S_STATEBLOCK_APPLY: case 14: forget_state(); break;
  case S_DP: prims = a[4]; break;               /* DrawPrimitive(type, start, count) */
  case S_DIP: prims = a[6]; break;              /* DrawIndexedPrimitive(type, min, num, start, count) */
  case S_DPUP: prims = a[3]; break;             /* DrawPrimitiveUP(type, count, data, stride) */
  case S_DIPUP: prims = a[5]; break;            /* DrawIndexedPrimitiveUP(type, min, num, count, ...) */
  }
  Site *s = site_of(ret, slot);
  if (s) { s->calls++; s->redundant += redundant; s->prims += prims; }
  g_calls_window++; g_redundant_window += redundant; g_prims_window += prims;
  if (slot >= S_DP && slot <= S_DIPUP) g_draws_window++;
  if (slot == S_PRESENT) {
    LARGE_INTEGER now; QueryPerformanceCounter(&now);
    if (g_last_present.QuadPart) {
      double ms = (now.QuadPart - g_last_present.QuadPart) * 1000.0 / g_qpf.QuadPart;
      g_ms_sum += ms; if (ms > g_ms_max) g_ms_max = ms; if (ms < g_ms_min) g_ms_min = ms;
      if (++g_frames >= REPORT_EVERY) report();
    }
    g_last_present = now;
  }
  return g_orig[slot];
}

/* one thunk per slot: EAX = slot (EAX is not preserved across a stdcall method anyway), then the common
   path, which leaves the stack exactly as the caller made it and jumps to the original method */
__asm__(
  ".intel_syntax noprefix\n"
  "d3dprof_common:\n"
  "  mov ecx, esp\n"
  "  push ecx\n"
  "  push eax\n"
  "  call _d3dprof_record\n"
  "  add esp, 8\n"
  "  jmp eax\n"
  ".att_syntax\n");
#define T(k) __asm__(".intel_syntax noprefix\n.globl _d3dprof_t" #k "\n_d3dprof_t" #k ":\n  mov eax, " #k "\n  jmp d3dprof_common\n.att_syntax\n"); \
             extern void d3dprof_t##k(void);
#define T10(d) T(d##0) T(d##1) T(d##2) T(d##3) T(d##4) T(d##5) T(d##6) T(d##7) T(d##8) T(d##9)
T(0) T(1) T(2) T(3) T(4) T(5) T(6) T(7) T(8) T(9)
T10(1) T10(2) T10(3) T10(4) T10(5) T10(6) T10(7) T10(8)
T(90) T(91) T(92) T(93) T(94) T(95) T(96)
#define P(k) (void *)d3dprof_t##k
#define P10(d) P(d##0), P(d##1), P(d##2), P(d##3), P(d##4), P(d##5), P(d##6), P(d##7), P(d##8), P(d##9)
static void *const THUNKS[NSLOTS] = {P(0), P(1), P(2), P(3), P(4), P(5), P(6), P(7), P(8), P(9),
  P10(1), P10(2), P10(3), P10(4), P10(5), P10(6), P10(7), P10(8),
  P(90), P(91), P(92), P(93), P(94), P(95), P(96)};

static void log_line(const char *msg) {
  FILE *f = fopen(g_out, "a");
  if (f) { fprintf(f, "# %s\n", msg); fclose(f); }
}

static DWORD WINAPI start(LPVOID self) {
  char path[MAX_PATH]; GetModuleFileNameA((HMODULE)self, path, MAX_PATH);
  char *slash = strrchr(path, '\\'); if (slash) slash[1] = 0;
  snprintf(g_out, sizeof g_out, "%sd3dprof_%lu.csv", path, (unsigned long)GetCurrentProcessId());
  HMODULE game = NULL;
  for (int i = 0; i < 600 && !(game = GetModuleHandleA("FFXiMain.dll")); i++) Sleep(100);
  if (!game) { log_line("FFXiMain.dll not loaded within 60 s: nothing done"); return 0; }
  IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS *)((char *)game + ((IMAGE_DOS_HEADER *)game)->e_lfanew);
  if (nt->FileHeader.TimeDateStamp != ANALYSED_TIMESTAMP) {
    log_line("FFXiMain.dll is not the analysed build (TimeDateStamp differs): nothing done"); return 0;
  }
  g_rel = (uint32_t)(uintptr_t)game - ANALYSED_BASE;
  g_lo = (uint32_t)(uintptr_t)game; g_hi = g_lo + nt->OptionalHeader.SizeOfImage;
  void ***cdx = (void ***)(uintptr_t)(G_PCDX + g_rel);
  void **dev = NULL;
  for (;;) {                                         /* the device exists once the game has made it */
    void **c = (void **)*cdx;
    if (c && (dev = (void **)c[CDX_DEVICE_OFF / 4]) != NULL) break;
    Sleep(200);
  }
  QueryPerformanceFrequency(&g_qpf); forget_state();
  g_orig = (void **)*dev;
  for (int k = 0; k < NSLOTS; k++) g_vt[k] = THUNKS[k];
  *(void ***)dev = g_vt;                             /* this object only; the driver's vtable is untouched */
  char msg[160]; snprintf(msg, sizeof msg, "hooked: FFXiMain.dll at %p, device %p, its vtable %p", game, dev, g_orig);
  log_line(msg);
  return 0;
}

BOOL WINAPI DllMain(HINSTANCE h, DWORD why, LPVOID r) {
  if (why == DLL_PROCESS_ATTACH) { DisableThreadLibraryCalls(h); CloseHandle(CreateThread(NULL, 0, start, h, 0, NULL)); }
  return TRUE;
}
