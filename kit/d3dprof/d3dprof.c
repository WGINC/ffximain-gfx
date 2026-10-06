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
static void *g_next[NSLOTS];          /* where each thunk goes on to: g_orig's method, or a logging wrapper (resources, below) */
static void *g_vt[NSLOTS];            /* the copy the device is given */
static uint32_t g_rel;                /* load address - analysed address */
static uint32_t g_lo, g_hi;           /* the game DLL's address range as loaded */
static int g_recording;               /* between BeginStateBlock and EndStateBlock: Set* calls are recorded
                                         into the block, not applied to the device (D3D8 documentation) */
static DWORD g_thread;
static char g_out[MAX_PATH];
int d3dprof_call_time;                 /* per-call time (below, d3dprof_return); xikit sets it from call_time= */
static double g_ct_overhead_us;       /* two counter reads back to back, measured at attach */

/* per (slot, site) totals over the report window */
/* kind: 0 = a call site in FFXiMain (the analysed address of the return address), 1 = a caller outside
   FFXiMain (the return address as it is), 2 = the caller of one of CDx's draw wrappers (the analysed
   address the wrapper returns to; see WRAP) */
enum { K_GAME, K_OTHER, K_WRAPPED };
static const char *KIND[] = {"FFXiMain", "other module", "caller of CDx draw wrapper"};
typedef struct { uint32_t site; uint16_t slot; uint8_t used, kind; uint32_t calls, redundant; uint64_t prims, ticks; uint32_t timed; } Site;
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
#define TRACE_MAX 1500000           /* records (40 bytes each); a busy scene makes 35000 calls a frame. Fewer if the address space has no room: */
#define TRACE_MIN 400000
static uint32_t g_trace_cap;
static Rec *g_trace; static uint32_t g_ntrace, g_trace_dropped, g_trace_frame; static int g_trace_label = -1;
/* the bytes themselves for the small structures a call reads (a matrix, a vertex list up to 128 bytes), so a difference
   can be read field by field: per record an offset into g_bytes and a length (0: none) */
#define TRACE_BYTES (24u << 20)
static uint8_t *g_bytes; static uint32_t g_nbytes, *g_boff; static uint8_t *g_blen;
/* the device state each draw ran with (docs/KIT.md): per draw record a hash of what the shadow below knows, and per
   distinct hash the state itself, written with the trace. A state the shadow does not know (never set since the
   kit attached, or since a state block was applied or the device reset: forget_state) is written as unknown */
static uint32_t *g_st;
typedef struct { uint32_t rs[256], tss[8][32], tex[8], vs, ib, vb[16], stride[16]; } Snap;
#define MAXSNAP 4096
static Snap *g_snap; static uint32_t g_snaph[MAXSNAP], g_nsnap, g_snap_collide;
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

static void state_now(Snap *s) {
  memcpy(s->rs, g_rs, sizeof s->rs); memcpy(s->tss, g_tss, sizeof s->tss);
  for (int i = 0; i < 8; i++) s->tex[i] = (uint32_t)(uintptr_t)g_tex[i];
  s->vs = g_vs; s->ib = g_ib;
  for (int i = 0; i < 16; i++) { s->vb[i] = (uint32_t)(uintptr_t)g_vb[i]; s->stride[i] = g_vbstride[i]; }
}
static uint32_t state_hash(void) {        /* the current state's hash; its first occurrence in the trace is kept */
  static Snap cur; state_now(&cur);
  uint32_t h = 2166136261u; const uint8_t *b = (const uint8_t *)&cur;
  for (uint32_t i = 0; i < sizeof cur; i++) h = (h ^ b[i]) * 16777619u;
  if (!g_snap) g_snap = (Snap *)VirtualAlloc(NULL, MAXSNAP * sizeof(Snap), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  for (uint32_t k = 0; k < g_nsnap; k++) if (g_snaph[k] == h) { if (memcmp(&g_snap[k], &cur, sizeof cur)) g_snap_collide++; return h; }
  if (g_snap && g_nsnap < MAXSNAP) { g_snaph[g_nsnap] = h; g_snap[g_nsnap++] = cur; }
  return h;
}

static void pend_clear(void);
void d3dprof_trace_start(int label) {
  for (uint32_t cap = TRACE_MAX; !g_trace && cap >= TRACE_MIN; cap /= 2) {
    g_trace = (Rec *)VirtualAlloc(NULL, cap * sizeof(Rec), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    g_bytes = g_trace ? (uint8_t *)VirtualAlloc(NULL, TRACE_BYTES + cap * 9, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE) : NULL;
    if (g_trace && !g_bytes) { VirtualFree(g_trace, 0, MEM_RELEASE); g_trace = NULL; }
    if (g_bytes) { g_trace_cap = cap; g_boff = (uint32_t *)(g_bytes + TRACE_BYTES); g_st = g_boff + cap; g_blen = (uint8_t *)(g_st + cap); }
  }
  if (!g_trace || !g_bytes) return;
  if (label == 0) { g_ntrace = 0; g_trace_dropped = 0; g_nbytes = 0; g_nsnap = 0; g_snap_collide = 0; }
  pend_clear();
  g_trace_label = label; g_trace_frame = 0;
}
void d3dprof_trace_stop(void) { g_trace_label = -1; }
/* the alternating trace (xikit.c): the label of the frame that starts now, without restarting the record */
void d3dprof_trace_relabel(int label) { if (g_trace_label >= 0) g_trace_label = label; }
void d3dprof_trace_frames(int *n_out) { if (n_out) *n_out = (int)g_trace_frame; }
int d3dprof_trace_write(const char *path, const char *note) {
  if (!g_trace) return 0;
  FILE *f = fopen(path, "w");
  if (!f) return 0;
  fprintf(f, "# d3d trace v1: %s\n# label frame slot method call_site nparams a2..a7 hash; call site = analysed address (0x10xxxxxx) or the raw return address\n"
             "# %u records, %u dropped\n", note, g_ntrace, g_trace_dropped);
  for (uint32_t i = 0; i < g_ntrace; i++) {
    Rec *r = &g_trace[i];
    fprintf(f, "%u %u %u %s 0x%08X %u", r->label, r->frame, r->slot,
            r->slot < NSLOTS ? NAMES[r->slot] : r->slot == SLOT_VB_UNLOCK ? "VB_Unlock" : "IB_Unlock", r->site, r->np);
    for (int k = 0; k < r->np && k < 6; k++) fprintf(f, " %08X", r->a[k]);
    fprintf(f, " %08X", r->h);
    if (g_blen && g_blen[i]) { fputs(" d=", f); for (uint32_t k = 0; k < g_blen[i]; k++) fprintf(f, "%02X", g_bytes[g_boff[i] + k]); }
    if (g_st && g_st[i]) fprintf(f, " s=%08X", g_st[i]);
    fputc('\n', f);
  }
  /* the states the draws ran with: only what is known (0xFFFFFFFF: not known, written as nothing) */
  fprintf(f, "# states: %u distinct (hash collisions with a different state: %u); per state: rs:STATE=VALUE tss:STAGE.TYPE=VALUE tex:STAGE=OBJECT vs ib vb:STREAM=OBJECT/STRIDE\n", g_nsnap, g_snap_collide);
  for (uint32_t k = 0; k < g_nsnap; k++) {
    Snap *sn = &g_snap[k]; fprintf(f, "#S %08X", g_snaph[k]);
    for (int i = 0; i < 256; i++) if (sn->rs[i] != 0xFFFFFFFFu) fprintf(f, " rs:%d=%X", i, sn->rs[i]);
    for (int st = 0; st < 8; st++) for (int i = 0; i < 32; i++) if (sn->tss[st][i] != 0xFFFFFFFFu) fprintf(f, " tss:%d.%d=%X", st, i, sn->tss[st][i]);
    for (int i = 0; i < 8; i++) if (sn->tex[i] != 0xFFFFFFFFu) fprintf(f, " tex:%d=%X", i, sn->tex[i]);
    if (sn->vs != 0xFFFFFFFFu) fprintf(f, " vs=%X", sn->vs);
    if (sn->ib != 0xFFFFFFFFu) fprintf(f, " ib=%X", sn->ib);
    for (int i = 0; i < 16; i++) if (sn->vb[i] != 0xFFFFFFFFu) fprintf(f, " vb:%d=%X/%u", i, sn->vb[i], sn->stride[i]);
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
      if (g_ntrace >= g_trace_cap) { g_trace_dropped++; break; }
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
  if (g_ntrace >= g_trace_cap) { g_trace_dropped++; return; }
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
  case 44: keep_bytes(g_ntrace - 1, (void *)a[3], 104); break;           /* a light: animated lights are told apart by value */
  case 72: keep_bytes(g_ntrace - 1, (void *)a[4], prim_verts(a[2], a[3]) * a[5]); break;
  default: g_blen[g_ntrace - 1] = 0;
  }
  g_st[g_ntrace - 1] = (slot >= S_DP && slot <= S_DIPUP) ? state_hash() : 0;
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
  if (d3dprof_call_time)
    fprintf(f, "# call time: QueryPerformanceCounter at %.0f Hz around each call on the render thread; timer overhead per "
               "call (two counter reads, measured at start, not subtracted) %.3f us\n", (double)g_qpf.QuadPart, g_ct_overhead_us);
  fprintf(f, "method,call_site,caller,calls_per_frame,redundant_per_frame,primitives_per_frame%s\n",
          d3dprof_call_time ? ",us_per_call,us_per_frame" : "");
  for (uint32_t i = 0; i < MAXSITES; i++) {
    Site *s = &g_sites[i];
    if (!s->used) continue;
    fprintf(f, "%s,0x%08X,%s,%.3f,%.3f,%.1f", NAMES[s->slot], s->site, KIND[s->kind],
            s->calls / n, s->redundant / n, s->prims / n);
    if (d3dprof_call_time) {
      double us = g_qpf.QuadPart ? s->ticks * 1e6 / (double)g_qpf.QuadPart : 0;
      if (s->timed) fprintf(f, ",%.3f,%.3f", us / s->timed, us / n); else fputs(",,", f);
    }
    fputc('\n', f);
  }
  fclose(f);
  memset(g_sites, 0, sizeof g_sites); g_nsites = g_dropped_sites = 0;
  g_frames = 0; g_calls_window = g_draws_window = g_prims_window = g_redundant_window = 0;
  g_ms_sum = g_ms_max = 0; g_ms_min = 1e9;
}

/* called by the thunk with the method's slot and the caller's stack (a[0] = return address, a[1] = this,
   a[2...] = the method's parameters); returns the original method, which the thunk jumps to */
/* xikit's device hooks for mods (xikit.c, device_hook) sit in front of these thunks. When one runs on the render
   thread, the kit pushes the return address of the call it intercepted here (and pops it when that call returns),
   so a call the mod's hook passes on, or makes itself, is recorded with the site of the call it is handling, not
   with an address inside the mod */
uint32_t d3dprof_via_stk[64]; volatile uint32_t d3dprof_via_depth;

/* Per-call time (d3dprof_call_time, xikit.ini call_time=1): on the render thread, the call's return address on the
   caller's stack is replaced by d3dprof_ret_stub, the real one kept on a shadow stack with the counter read last
   thing before the method runs; the stub reads the counter first thing after it returns, adds the difference to
   the call's site and jumps to the real return address with EAX and EDX as the method left them. Nothing else of
   the caller's stack changes, and the method's own `ret N` has already popped its arguments. The return path is
   integer code only (the x87 stack is not touched). The site is looked up again at return (report() may have
   started a new window in between). Calls deeper than 64 on the shadow stack are counted but not timed. */
typedef struct { uint32_t ret, site; uint16_t slot; uint8_t kind; LARGE_INTEGER t0; } Pending_call;
static Pending_call g_ct[64]; static uint32_t g_ct_depth, g_ct_untimed;
extern void d3dprof_ret_stub(void);
uint32_t __cdecl d3dprof_return(void) {
  LARGE_INTEGER now; QueryPerformanceCounter(&now);
  Pending_call *c = &g_ct[--g_ct_depth];
  Site *s = site_of(c->site, c->slot, c->kind);
  if (s) { s->ticks += (uint64_t)(now.QuadPart - c->t0.QuadPart); s->timed++; }
  return c->ret;
}
const char *d3dprof_method(int slot) { return slot >= 0 && slot < NSLOTS ? NAMES[slot] : NULL; }

void *__cdecl d3dprof_record(int slot, uint32_t *a) {
  if (g_thread == 0) g_thread = GetCurrentThreadId();
  if (GetCurrentThreadId() != g_thread) { g_other_thread++; return g_next[slot]; }
  /* a call site in the game's DLL as the analysed address; any other caller as it is, marked by its kind */
  uint32_t ra = a[0];
  if (d3dprof_via_depth && d3dprof_via_depth <= 64 && !(ra >= g_lo && ra < g_hi)) ra = d3dprof_via_stk[d3dprof_via_depth - 1];
  int game = ra >= g_lo && ra < g_hi;
  uint32_t ret = game ? ra - g_rel : ra;
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
  if (d3dprof_call_time) {
    if (g_ct_depth < 64) {
      Pending_call *c = &g_ct[g_ct_depth++];
      c->ret = a[0]; c->site = ret; c->slot = (uint16_t)slot; c->kind = (uint8_t)(game ? K_GAME : K_OTHER);
      a[0] = (uint32_t)(uintptr_t)d3dprof_ret_stub;
      QueryPerformanceCounter(&c->t0);
    } else g_ct_untimed++;
  }
  return g_next[slot];
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
__declspec(naked) void d3dprof_ret_stub(void) {
  __asm {
    push eax
    push edx
    call d3dprof_return
    mov ecx, eax
    pop edx
    pop eax
    jmp ecx
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
  ".globl _d3dprof_ret_stub\n"
  "_d3dprof_ret_stub:\n"
  "  push eax\n"
  "  push edx\n"
  "  call _d3dprof_return\n"
  "  mov ecx, eax\n"
  "  pop edx\n"
  "  pop eax\n"
  "  jmp ecx\n"
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


/* ------------------------------------------------------------------------------------------------ resources
   Every texture, buffer, surface and vertex shader the device creates (any thread), with the call site that asked
   for it, its parameters and the object it got: <dir>/xikit_resources_<pid>.csv, appended as they happen. A
   vertex shader's declaration and function tokens are saved as xikit_vshader_<pid>_<handle>.bin, exactly as the
   game passed them (kit/d3dprof/vshader.py reads them). The wrappers sit after the thunk (g_next) and call the
   device's own method, so the counts and the trace see the call as before. */
static CRITICAL_SECTION g_reslock; static FILE *g_res; static char g_resdir[MAX_PATH]; static int g_res_init;
static uint32_t site_of_ra(uint32_t ra) { return ra >= g_lo && ra < g_hi ? ra - g_rel : ra; }
static void res_log(const char *method, uint32_t ra, const uint32_t *p, int np, uint32_t obj, HRESULT hr) {
  if (!g_res_init) return;
  EnterCriticalSection(&g_reslock);
  if (!g_res) {
    char path[MAX_PATH]; snprintf(path, sizeof path, "%s\\xikit_resources_%lu.csv", g_resdir, (unsigned long)GetCurrentProcessId());
    g_res = fopen(path, "a");
    if (g_res) fprintf(g_res, "# resources created through the device: frame,thread,method,call_site,result,object,parameters... (call site: analysed address in FFXiMain, or the raw return address)\n");
  }
  if (g_res) {
    fprintf(g_res, "%u,%s,%s,0x%08X,%08lX,%08X", g_frames, GetCurrentThreadId() == g_thread ? "render" : "other", method, site_of_ra(ra), (unsigned long)hr, obj);
    for (int i = 0; i < np; i++) fprintf(g_res, ",%X", p[i]);
    fputc('\n', g_res); fflush(g_res);
  }
  LeaveCriticalSection(&g_reslock);
}
#define RA() ((uint32_t)(uintptr_t)__builtin_return_address(0))
#define OBJ(pp) ((hr >= 0 && (pp)) ? (uint32_t)(uintptr_t)*(pp) : 0)
static HRESULT __stdcall w_tex(void *d, UINT w, UINT h, UINT lv, DWORD us, DWORD fmt, DWORD pool, void **pp) {
  HRESULT hr = ((HRESULT (__stdcall *)(void *, UINT, UINT, UINT, DWORD, DWORD, DWORD, void **))g_orig[20])(d, w, h, lv, us, fmt, pool, pp);
  uint32_t a[6] = {w, h, lv, us, fmt, pool}; res_log("CreateTexture(w,h,levels,usage,format,pool)", RA(), a, 6, OBJ(pp), hr); return hr; }
static HRESULT __stdcall w_vol(void *d, UINT w, UINT h, UINT dp, UINT lv, DWORD us, DWORD fmt, DWORD pool, void **pp) {
  HRESULT hr = ((HRESULT (__stdcall *)(void *, UINT, UINT, UINT, UINT, DWORD, DWORD, DWORD, void **))g_orig[21])(d, w, h, dp, lv, us, fmt, pool, pp);
  uint32_t a[7] = {w, h, dp, lv, us, fmt, pool}; res_log("CreateVolumeTexture(w,h,depth,levels,usage,format,pool)", RA(), a, 7, OBJ(pp), hr); return hr; }
static HRESULT __stdcall w_cube(void *d, UINT e, UINT lv, DWORD us, DWORD fmt, DWORD pool, void **pp) {
  HRESULT hr = ((HRESULT (__stdcall *)(void *, UINT, UINT, DWORD, DWORD, DWORD, void **))g_orig[22])(d, e, lv, us, fmt, pool, pp);
  uint32_t a[5] = {e, lv, us, fmt, pool}; res_log("CreateCubeTexture(edge,levels,usage,format,pool)", RA(), a, 5, OBJ(pp), hr); return hr; }
static HRESULT __stdcall w_vb(void *d, UINT len, DWORD us, DWORD fvf, DWORD pool, void **pp) {
  HRESULT hr = ((HRESULT (__stdcall *)(void *, UINT, DWORD, DWORD, DWORD, void **))g_orig[23])(d, len, us, fvf, pool, pp);
  uint32_t a[4] = {len, us, fvf, pool}; res_log("CreateVertexBuffer(length,usage,fvf,pool)", RA(), a, 4, OBJ(pp), hr); return hr; }
static HRESULT __stdcall w_ib(void *d, UINT len, DWORD us, DWORD fmt, DWORD pool, void **pp) {
  HRESULT hr = ((HRESULT (__stdcall *)(void *, UINT, DWORD, DWORD, DWORD, void **))g_orig[24])(d, len, us, fmt, pool, pp);
  uint32_t a[4] = {len, us, fmt, pool}; res_log("CreateIndexBuffer(length,usage,format,pool)", RA(), a, 4, OBJ(pp), hr); return hr; }
static HRESULT __stdcall w_rt(void *d, UINT w, UINT h, DWORD fmt, DWORD ms, BOOL lk, void **pp) {
  HRESULT hr = ((HRESULT (__stdcall *)(void *, UINT, UINT, DWORD, DWORD, BOOL, void **))g_orig[25])(d, w, h, fmt, ms, lk, pp);
  uint32_t a[5] = {w, h, fmt, ms, (uint32_t)lk}; res_log("CreateRenderTarget(w,h,format,multisample,lockable)", RA(), a, 5, OBJ(pp), hr); return hr; }
static HRESULT __stdcall w_ds(void *d, UINT w, UINT h, DWORD fmt, DWORD ms, void **pp) {
  HRESULT hr = ((HRESULT (__stdcall *)(void *, UINT, UINT, DWORD, DWORD, void **))g_orig[26])(d, w, h, fmt, ms, pp);
  uint32_t a[4] = {w, h, fmt, ms}; res_log("CreateDepthStencilSurface(w,h,format,multisample)", RA(), a, 4, OBJ(pp), hr); return hr; }
static HRESULT __stdcall w_img(void *d, UINT w, UINT h, DWORD fmt, void **pp) {
  HRESULT hr = ((HRESULT (__stdcall *)(void *, UINT, UINT, DWORD, void **))g_orig[27])(d, w, h, fmt, pp);
  uint32_t a[3] = {w, h, fmt}; res_log("CreateImageSurface(w,h,format)", RA(), a, 3, OBJ(pp), hr); return hr; }
static HRESULT __stdcall w_vs(void *d, const DWORD *decl, const DWORD *func, DWORD *handle, DWORD usage) {
  HRESULT hr = ((HRESULT (__stdcall *)(void *, const DWORD *, const DWORD *, DWORD *, DWORD))g_orig[75])(d, decl, func, handle, usage);
  uint32_t h = (hr >= 0 && handle) ? *handle : 0, nd = 0, nf = 0;
  if (decl) while (nd < 512 && decl[nd] != 0xFFFFFFFFu) nd++;          /* D3DVSD_END() */
  if (func) while (nf < 65536 && func[nf] != 0x0000FFFFu) nf++;         /* the end token */
  uint32_t a[3] = {nd + (decl ? 1 : 0), nf + (func ? 1 : 0), usage};
  res_log("CreateVertexShader(declaration tokens,function tokens,usage)", RA(), a, 3, h, hr);
  if (hr >= 0 && g_res_init) {
    char path[MAX_PATH]; snprintf(path, sizeof path, "%s\\xikit_vshader_%lu_%08X.bin", g_resdir, (unsigned long)GetCurrentProcessId(), h);
    FILE *f = fopen(path, "wb");
    if (f) { uint32_t head[4] = {0x48535658u /* "XVSH" */, h, a[0], a[1]}; fwrite(head, 4, 4, f);
             if (decl) fwrite(decl, 4, a[0], f);
             if (func) fwrite(func, 4, a[1], f);
             fclose(f); }
  }
  return hr;
}
static void resources_init(void) {
  InitializeCriticalSection(&g_reslock);
  snprintf(g_resdir, sizeof g_resdir, "%s", g_out); char *sl = strrchr(g_resdir, '\\'); if (sl) *sl = 0; else snprintf(g_resdir, sizeof g_resdir, ".");
  g_res_init = 1;
}
static void measure_overhead(void) {      /* two counter reads back to back: the least of 1000, in microseconds */
  LONGLONG best = -1;
  for (int i = 0; i < 1000; i++) {
    LARGE_INTEGER x, y; QueryPerformanceCounter(&x); QueryPerformanceCounter(&y);
    if (best < 0 || y.QuadPart - x.QuadPart < best) best = y.QuadPart - x.QuadPart;
  }
  g_ct_overhead_us = g_qpf.QuadPart ? best * 1e6 / (double)g_qpf.QuadPart : 0;
}

static void set_next(void) {
  for (int k = 0; k < NSLOTS; k++) g_next[k] = g_orig[k];
  g_next[20] = (void *)w_tex; g_next[21] = (void *)w_vol; g_next[22] = (void *)w_cube; g_next[23] = (void *)w_vb; g_next[24] = (void *)w_ib;
  g_next[25] = (void *)w_rt; g_next[26] = (void *)w_ds; g_next[27] = (void *)w_img; g_next[75] = (void *)w_vs;
}

#ifdef XIKIT
/* built into the in-game kit (kit/xikit): no thread of its own; the kit finds the device and calls this. The
   device's current vtable (the kit's copy) becomes what every thunk calls on to, so the two chain. */
void d3dprof_attach(void **dev, uint32_t rel, uint32_t lo, uint32_t hi, const char *csv) {
  snprintf(g_out, sizeof g_out, "%s", csv);
  g_rel = rel; g_lo = lo; g_hi = hi;
  QueryPerformanceFrequency(&g_qpf); forget_state(); if (d3dprof_call_time) measure_overhead();
  g_orig = (void **)*dev; set_next(); resources_init();
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
  /* d3dprof.ini next to the DLL, [d3dprof] call_time=1: time each call (xikit sets it from its own ini) */
  char ini[MAX_PATH]; snprintf(ini, sizeof ini, "%sd3dprof.ini", path);
  d3dprof_call_time = GetPrivateProfileIntA("d3dprof", "call_time", 0, ini);
  QueryPerformanceFrequency(&g_qpf); forget_state(); if (d3dprof_call_time) measure_overhead();
  g_orig = (void **)*dev; set_next(); resources_init();
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
