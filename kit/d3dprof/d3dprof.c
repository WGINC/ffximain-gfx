/* d3dprof.c -- a per-frame profile of the game's Direct3D 8 calls, taken inside the running game.

   Injected into pol.exe (System Informer "Inject DLL" or any injector, as speedfix_hook is; DllMain only
   starts a thread), it:
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

   Build (32-bit, like the game):
     MSVC:  cmake -B build -A Win32 && cmake --build build --config Release   (CMakeLists.txt)
            or, in an x86 Developer Command Prompt:  cl /O2 /LD d3dprof.c
     MinGW: build.sh */
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

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
/* kind: 0 = a call site in FFXiMain (the analysed address of the return address), 1 = a caller outside
   FFXiMain (the return address as it is), 2 = the caller of one of CDx's draw wrappers (the analysed
   address the wrapper returns to; see WRAP) */
enum { K_GAME, K_OTHER, K_WRAPPED };
static const char *KIND[] = {"FFXiMain", "other module", "caller of CDx draw wrapper"};
typedef struct { uint32_t site; uint16_t slot; uint8_t used, kind; uint32_t calls, redundant; uint64_t prims; } Site;
/* The draws of CDx_DrawPrimitive / CDx_DrawIndexedPrimitive / CDx_DrawPrimitiveUP (0x1000CB20, 0x1000CC00,
   0x1000CD00) are made inside those wrappers. Who asked for the draw is the wrapper's caller. Its return
   address is at a fixed offset from ESP at the method's entry: 4 + the exact stack depth at the call
   instruction (pipeline/sp_solve.py insn_depths). */
static const struct { uint32_t ret; uint32_t off; } WRAP[] = {
  {0x1000CB74, 44}, {0x1000CBB2, 44}, {0x1000CBF3, 32},     /* CDx_DrawPrimitive: depths 40, 40, 28 */
  {0x1000CC5E, 52}, {0x1000CCA6, 52}, {0x1000CCF1, 40},     /* CDx_DrawIndexedPrimitive: 48, 48, 36 */
  {0x1000CD59, 48}, {0x1000CD9C, 48}, {0x1000CDE2, 36}};    /* CDx_DrawPrimitiveUP: 44, 44, 32 */
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


/* ------------------------------------------------------------------------------------------------ call trace
   A record of every call the render thread makes through the device, for a few frames: slot, call site (as in the
   profile), the method's first parameters, and a hash of what a pointer parameter points at where the method reads
   a structure through it (transform 64 bytes, viewport 24, material 68, light 104, clip plane 16, shader constants,
   and the vertex data of the two ...UP draws). The kit records frames with its swaps off, then on, in one session
   (xikit.c: Ctrl+Alt+F8), and trace_diff.py compares the two by call site. Vertex buffers a draw uses are not
   hashed: their contents are written through a locked pointer, not through a device call. */
#define SLOT_VB_UNLOCK 97                /* buffer records (below): not device methods */
#define SLOT_IB_UNLOCK 98
static const uint8_t NPARAM[NSLOTS] = {2, 0, 0, 0, 0, 1, 1, 1, 1, 1, 3, 3, 1, 2, 1, 4, 3, 1, 2, 1, 7, 8, 6, 5, 5, 6, 5, 4, 5, 2, 1, 2, 1, 1, 0, 0, 6, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 0, 1, 1, 1, 1, 2, 1, 1, 2, 2, 3, 3, 1, 3, 2, 2, 1, 1, 3, 5, 4, 8, 5, 4, 1, 1, 1, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 3, 3, 3, 3, 3, 1};
typedef struct { uint8_t label, np; uint16_t slot; uint32_t frame, site, a[6], h; } Rec;
#define TRACE_MAX 600000
static Rec *g_trace; static uint32_t g_ntrace, g_trace_dropped, g_trace_frame; static int g_trace_label = -1;
/* the bytes themselves for the small structures a call reads (a matrix, a vertex list up to 128 bytes), so a difference
   can be read field by field: per record an offset into g_bytes and a length (0: none) */
#define TRACE_BYTES (24u << 20)
static uint8_t *g_bytes; static uint32_t g_nbytes, *g_boff; static uint8_t *g_blen;
static void keep_bytes(uint32_t rec, const void *p, uint32_t n) {
  g_blen[rec] = 0;
  if (!p || n == 0 || n > 128 || g_nbytes + n > TRACE_BYTES || IsBadReadPtr(p, n)) return;
  for (uint32_t k = 0; k < n; k++) g_bytes[g_nbytes + k] = ((const uint8_t *)p)[k];   /* (no CRT call: test_thunks.py) */
  g_boff[rec] = g_nbytes; g_blen[rec] = (uint8_t)n; g_nbytes += n;
}

static uint32_t fnv(const void *p, uint32_t n) {
  uint32_t h = 2166136261u;
  if (!p || n == 0 || n > 0x1000000 || IsBadReadPtr(p, n)) return 0xDEAD0000u | (p ? 1u : 0u);
  for (uint32_t i = 0; i < n; i++) h = (h ^ ((const uint8_t *)p)[i]) * 16777619u;
  return h;
}
static uint32_t prim_verts(uint32_t type, uint32_t n) {     /* D3DPRIMITIVETYPE: POINTLIST 1 .. TRIANGLEFAN 6 */
  switch (type) { case 1: return n; case 2: return 2 * n; case 3: return n + 1; case 4: return 3 * n; case 5: case 6: return n + 2; }
  return 0;
}

static void pend_clear(void);
void d3dprof_trace_start(int label) {
  if (!g_trace) g_trace = (Rec *)VirtualAlloc(NULL, TRACE_MAX * sizeof(Rec), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  if (!g_bytes) {
    g_bytes = (uint8_t *)VirtualAlloc(NULL, TRACE_BYTES + TRACE_MAX * 5, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (g_bytes) { g_boff = (uint32_t *)(g_bytes + TRACE_BYTES); g_blen = (uint8_t *)(g_boff + TRACE_MAX); }
  }
  if (!g_trace || !g_bytes) return;
  if (label == 0) { g_ntrace = 0; g_trace_dropped = 0; g_nbytes = 0; }
  pend_clear();
  g_trace_label = label; g_trace_frame = 0;
}
void d3dprof_trace_stop(void) { g_trace_label = -1; }
/* the alternating trace (xikit.c): the label of the frame that starts now, without restarting the record */
void d3dprof_trace_relabel(int label) { if (g_trace_label >= 0) g_trace_label = label; }
void d3dprof_trace_frames(int *n_out) { if (n_out) *n_out = (int)g_trace_frame; }
int d3dprof_trace_write(const char *path, const char *note) {
  FILE *f = fopen(path, "w");
  if (!f || !g_trace) return 0;
  fprintf(f, "# d3d trace v1: %s\n# label frame slot method call_site nparams a2..a7 hash; call site = analysed address (0x10xxxxxx) or the raw return address\n"
             "# %u records, %u dropped\n", note, g_ntrace, g_trace_dropped);
  for (uint32_t i = 0; i < g_ntrace; i++) {
    Rec *r = &g_trace[i];
    fprintf(f, "%u %u %u %s 0x%08X %u", r->label, r->frame, r->slot,
            r->slot < NSLOTS ? NAMES[r->slot] : r->slot == SLOT_VB_UNLOCK ? "VB_Unlock" : "IB_Unlock", r->site, r->np);
    for (int k = 0; k < r->np && k < 6; k++) fprintf(f, " %08X", r->a[k]);
    fprintf(f, " %08X", r->h);
    if (g_blen && g_blen[i]) { fputs(" d=", f); for (uint32_t k = 0; k < g_blen[i]; k++) fprintf(f, "%02X", g_bytes[g_boff[i] + k]); }
    fputc('\n', f);
  }
  fclose(f); return 1;
}
/* Vertex and index buffer contents. The game writes them through the pointer IDirect3DVertexBuffer8::Lock /
   IDirect3DIndexBuffer8::Lock returns (slots 11 and 12 Unlock, 13 GetDesc in the D3D8 header for both), not
   through the device, so the device trace cannot see them. While a trace is armed, every buffer the game binds
   (SetStreamSource, SetIndices) gets a copy of its class's vtable with Lock and Unlock wrapped (that object only, as
   for the device); at Unlock the locked range is hashed and recorded as VB_Unlock / IB_Unlock with the call site of
   the Lock: the function that filled it. Also counted: dwords whose float exponent is all ones (inf/NaN, also what a
   D3DCOLOR with alpha 0xFF looks like) and dwords that are floats of magnitude 2^20 or more. */
#define NBUFCLS 16
#define BUFVT 14
typedef HRESULT (__stdcall *LockFn)(void *, UINT, UINT, BYTE **, DWORD);
typedef HRESULT (__stdcall *UnlockFn)(void *);
typedef HRESULT (__stdcall *DescFn)(void *, void *);
static void **g_bc_orig[NBUFCLS]; static void *g_bc_copy[NBUFCLS][BUFVT]; static uint8_t g_bc_kind[NBUFCLS]; static int g_nbc;
static int g_hookbufs;
typedef struct { void *obj; uint8_t *p; uint32_t off, size, site; } Pending;
#define NPEND 1024
static Pending g_pend[NPEND];
static int bc_of(void **vp) { for (int i = 0; i < g_nbc; i++) if (vp == (void **)g_bc_copy[i]) return i; return -1; }
static HRESULT __stdcall buf_lock(void *self, UINT off, UINT size, BYTE **pp, DWORD flags);
static HRESULT __stdcall buf_unlock(void *self);
static void hook_buf(void *obj, int kind) {
  if (!obj || IsBadReadPtr(obj, 4)) return;
  void **vp = *(void ***)obj;
  if (bc_of(vp) >= 0 || IsBadReadPtr(vp, BUFVT * 4)) return;
  int i = 0;
  while (i < g_nbc && g_bc_orig[i] != vp) i++;
  if (i == g_nbc) {
    if (g_nbc >= NBUFCLS) return;
    memcpy(g_bc_copy[i], vp, sizeof g_bc_copy[i]);
    g_bc_copy[i][11] = (void *)buf_lock; g_bc_copy[i][12] = (void *)buf_unlock;
    g_bc_orig[i] = vp; g_bc_kind[i] = (uint8_t)kind; g_nbc++;
  }
  *(void ***)obj = (void **)g_bc_copy[i];
}
void d3dprof_trace_hookbufs(int on) { g_hookbufs = on; }
static void pend_clear(void) { for (int i = 0; i < NPEND; i++) g_pend[i].obj = NULL; }   /* (no CRT call: test_thunks.py runs this) */
#ifdef _MSC_VER
#include <intrin.h>
#define RETADDR() ((uint32_t)(uintptr_t)_ReturnAddress())
#else
#define RETADDR() ((uint32_t)(uintptr_t)__builtin_return_address(0))
#endif
static HRESULT __stdcall buf_lock(void *self, UINT off, UINT size, BYTE **pp, DWORD flags) {
  uint32_t ra = RETADDR();
  int i = bc_of(*(void ***)self);
  if (i < 0) return E_FAIL;                         /* cannot happen: only objects given this vtable copy get here */
  HRESULT hr = ((LockFn)g_bc_orig[i][11])(self, off, size, pp, flags);
  if (g_trace_label >= 0 && hr >= 0 && pp && *pp && GetCurrentThreadId() == g_thread) {
    if (size == 0) {                                /* the whole buffer from off: the size is in the description */
      uint32_t d[8] = {0};
      if (((DescFn)g_bc_orig[i][13])(self, d) >= 0 && d[4] > off) size = d[4] - off;
    }
    uint32_t h = ((uint32_t)(uintptr_t)self >> 4) & (NPEND - 1);
    for (int n = 0; n < NPEND; n++, h = (h + 1) & (NPEND - 1))
      if (!g_pend[h].obj || g_pend[h].obj == (void *)1 || g_pend[h].obj == self) {
        g_pend[h].obj = self; g_pend[h].p = *pp; g_pend[h].off = off; g_pend[h].size = size;
        g_pend[h].site = ra >= g_lo && ra < g_hi ? ra - g_rel : ra; break;
      }
  }
  return hr;
}
static HRESULT __stdcall buf_unlock(void *self) {
  int i = bc_of(*(void ***)self);
  if (i < 0) return E_FAIL;
  uint32_t h = ((uint32_t)(uintptr_t)self >> 4) & (NPEND - 1);
  for (int n = 0; n < NPEND; n++, h = (h + 1) & (NPEND - 1)) {
    if (!g_pend[h].obj) break;
    if (g_pend[h].obj != self) continue;
    Pending q = g_pend[h]; g_pend[h].obj = (void *)1;    /* a tombstone keeps later entries of the probe reachable */
    if (g_trace_label >= 0 && g_trace && GetCurrentThreadId() == g_thread) {
      if (g_ntrace >= TRACE_MAX) { g_trace_dropped++; break; }
      uint32_t nonfinite = 0, huge = 0;
      if (q.size <= 0x1000000 && !IsBadReadPtr(q.p, q.size))
        for (uint32_t k = 0; k + 4 <= q.size; k += 4) {
          uint32_t w = *(const uint32_t *)(q.p + k), e = (w >> 23) & 0xFF;
          if (e == 0xFF) nonfinite++; else if (e >= 147) huge++;
        }
      Rec *r = &g_trace[g_ntrace++];
      r->label = (uint8_t)g_trace_label; r->frame = g_trace_frame; r->slot = g_bc_kind[i] ? SLOT_IB_UNLOCK : SLOT_VB_UNLOCK;
      r->site = q.site; r->np = 4; r->a[0] = q.off; r->a[1] = q.size; r->a[2] = nonfinite; r->a[3] = huge; r->a[4] = r->a[5] = 0;
      r->h = fnv(q.p, q.size); g_blen[g_ntrace - 1] = 0;
    }
    break;
  }
  return ((UnlockFn)g_bc_orig[i][12])(self);
}

static void trace_call(int slot, uint32_t *a, uint32_t site) {
  if (g_hookbufs && GetCurrentThreadId() == g_thread) {
    if (slot == S_SSS) hook_buf((void *)a[3], 0);
    else if (slot == S_SETIND) hook_buf((void *)a[2], 1);
  }
  if (g_trace_label < 0 || !g_trace) return;
  if (g_ntrace >= TRACE_MAX) { g_trace_dropped++; return; }
  Rec *r = &g_trace[g_ntrace++];
  r->label = (uint8_t)g_trace_label; r->frame = g_trace_frame; r->slot = (uint16_t)slot; r->site = site; r->np = NPARAM[slot];
  for (int k = 0; k < 6; k++) r->a[k] = k < r->np ? a[2 + k] : 0;
  uint32_t h = 0;
  switch (slot) {
  case 37: case 39: h = fnv((void *)a[3], 64); break;                       /* SetTransform, MultiplyTransform */
  case 40: h = fnv((void *)a[2], 24); break;                                /* SetViewport */
  case 42: h = fnv((void *)a[2], 68); break;                                /* SetMaterial */
  case 44: h = fnv((void *)a[3], 104); break;                               /* SetLight */
  case 48: h = fnv((void *)a[3], 16); break;                                /* SetClipPlane */
  case 79: case 91: h = fnv((void *)a[3], 16 * a[4]); break;                /* Set{Vertex,Pixel}ShaderConstant */
  case 72: h = fnv((void *)a[4], prim_verts(a[2], a[3]) * a[5]); break;     /* DrawPrimitiveUP(type, count, data, stride) */
  case 73: h = fnv((void *)a[8], (a[3] + a[4]) * a[9]); break;              /* DrawIndexedPrimitiveUP(type, min, num, count, idx, fmt, data, stride) */
  }
  r->h = h;
  switch (slot) {                                                           /* the bytes too, where small */
  case 37: case 39: keep_bytes(g_ntrace - 1, (void *)a[3], 64); break;
  case 72: keep_bytes(g_ntrace - 1, (void *)a[4], prim_verts(a[2], a[3]) * a[5]); break;
  default: g_blen[g_ntrace - 1] = 0;
  }
}

static Site *site_of(uint32_t site, int slot, int kind) {
  uint32_t h = (site * 2654435761u ^ (uint32_t)slot * 40503u ^ (uint32_t)kind * 97u) & (MAXSITES - 1);
  for (uint32_t n = 0; n < MAXSITES; n++, h = (h + 1) & (MAXSITES - 1)) {
    Site *s = &g_sites[h];
    if (!s->used) {
      if (g_nsites >= MAXSITES * 3 / 4) { g_dropped_sites++; return NULL; }
      s->used = 1; s->site = site; s->slot = (uint16_t)slot; s->kind = (uint8_t)kind; g_nsites++; return s;
    }
    if (s->site == site && s->slot == slot && s->kind == kind) return s;
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
  fprintf(f, "# d3dprof v2: %u frames; frame ms avg %.3f min %.3f max %.3f; per frame: calls %.1f draws %.1f "
             "primitives %.1f redundant_sets %.1f; other_thread_calls %u; sites %u (dropped %u)\n",
          g_frames, g_ms_sum / n, g_ms_min, g_ms_max, g_calls_window / n, g_draws_window / n, g_prims_window / n,
          g_redundant_window / n, g_other_thread, g_nsites, g_dropped_sites);
  fprintf(f, "method,call_site,caller,calls_per_frame,redundant_per_frame,primitives_per_frame\n");
  for (uint32_t i = 0; i < MAXSITES; i++) {
    Site *s = &g_sites[i];
    if (!s->used) continue;
    fprintf(f, "%s,0x%08X,%s,%.3f,%.3f,%.1f\n", NAMES[s->slot], s->site, KIND[s->kind],
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
  /* a call site in the game's DLL as the analysed address; any other caller as it is, marked by its kind */
  int game = a[0] >= g_lo && a[0] < g_hi;
  uint32_t ret = game ? a[0] - g_rel : a[0];
  int redundant = 0; uint32_t prims = 0;
  trace_call(slot, a, ret);
  if (slot == S_PRESENT && g_trace_label >= 0) g_trace_frame++;
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
  Site *s = site_of(ret, slot, game ? K_GAME : K_OTHER);
  if (s) { s->calls++; s->redundant += redundant; s->prims += prims; }
  if (game && slot >= S_DP && slot <= S_DPUP)
    for (unsigned w = 0; w < sizeof WRAP / sizeof WRAP[0]; w++)
      if (WRAP[w].ret == ret) {
        uint32_t c = a[WRAP[w].off / 4];
        if (c >= g_lo && c < g_hi && (s = site_of(c - g_rel, slot, K_WRAPPED)) != NULL) { s->calls++; s->prims += prims; }
        break;
      }
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
#ifdef _MSC_VER
/* MSVC (x86 only: inline assembly and naked functions do not exist for x64, and the game is 32-bit) */
static __declspec(naked) void d3dprof_common(void) {
  __asm {
    mov ecx, esp
    push ecx
    push eax
    call d3dprof_record
    add esp, 8
    jmp eax
  }
}
#define T(k) static __declspec(naked) void d3dprof_t##k(void) { __asm mov eax, k __asm jmp d3dprof_common }
#else
/* GCC / MinGW */
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
#endif
#define T10(d) T(d##0) T(d##1) T(d##2) T(d##3) T(d##4) T(d##5) T(d##6) T(d##7) T(d##8) T(d##9)
T(0) T(1) T(2) T(3) T(4) T(5) T(6) T(7) T(8) T(9)
T10(1) T10(2) T10(3) T10(4) T10(5) T10(6) T10(7) T10(8)
T(90) T(91) T(92) T(93) T(94) T(95) T(96)
#define P(k) (void *)d3dprof_t##k
#define P10(d) P(d##0), P(d##1), P(d##2), P(d##3), P(d##4), P(d##5), P(d##6), P(d##7), P(d##8), P(d##9)
static void *const THUNKS[NSLOTS] = {P(0), P(1), P(2), P(3), P(4), P(5), P(6), P(7), P(8), P(9),
  P10(1), P10(2), P10(3), P10(4), P10(5), P10(6), P10(7), P10(8),
  P(90), P(91), P(92), P(93), P(94), P(95), P(96)};

#ifdef XIKIT
/* built into the in-game kit (kit/xikit): no thread of its own; the kit finds the device and calls this. The
   device's current vtable (the kit's copy) becomes what every thunk calls on to, so the two chain. */
void d3dprof_attach(void **dev, uint32_t rel, uint32_t lo, uint32_t hi, const char *csv) {
  snprintf(g_out, sizeof g_out, "%s", csv);
  g_rel = rel; g_lo = lo; g_hi = hi;
  QueryPerformanceFrequency(&g_qpf); forget_state();
  g_orig = (void **)*dev;
  for (int k = 0; k < NSLOTS; k++) g_vt[k] = THUNKS[k];
  *(void ***)dev = g_vt;
}
#else
static void log_line(const char *msg) {
  FILE *f = fopen(g_out, "a");
  if (f) { fprintf(f, "# %s\n", msg); fclose(f); }
}

static DWORD WINAPI start(LPVOID self) {
  char path[MAX_PATH]; GetModuleFileNameA((HMODULE)self, path, MAX_PATH);
  char *slash = strrchr(path, '\\'); if (slash) slash[1] = 0;
  snprintf(g_out, sizeof g_out, "%sd3dprof_%lu.csv", path, (unsigned long)GetCurrentProcessId());
  /* injected into pol.exe (any injector), possibly long before the game DLL loads: wait without a limit */
  HMODULE game = NULL;
  while (!(game = GetModuleHandleA("FFXiMain.dll"))) Sleep(100);
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
#endif
