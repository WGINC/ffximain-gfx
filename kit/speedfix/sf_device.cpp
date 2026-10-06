// SPDX-License-Identifier: GPL-3.0-only
//
// sf_device.cpp -- speedfix's Direct3D 8 device layer, on the kit's device hooks (xikit device_hook, docs/SDK.md).
// Rewritten from speedfix_hook.cpp (the owner's, GPL-3.0-only): the state cache, the vertex-shader cache, the
// extended cache and DrawPrimitiveUP batching keep that file's rules. What changed:
//   * The hooks are the kit's device hooks, not writes to the driver's vtable. d3dprof sits behind them, so its
//     counts are what speedfix lets through (call_time=1 measures what it saves), and the kit takes them off for
//     F9 and for a trace's unswapped frames (an A/B with nothing else changed).
//   * Calls from any thread but the render thread are never cached or batched: they flush what is pending and make
//     the whole shadow unknown (the game's other threads may set state behind it).
//   * MultiplyTransform makes that transform's shadow unknown (the old extended cache kept trusting it).
//   * A batch converts strips and fans to lists only while FILLMODE is SOLID and SHADEMODE is GOURAUD (read from the
//     device when not known): with point or wireframe fill the converted list repeats shared vertices and edges, and
//     with flat shading the colour vertex of each triangle depends on how D3D8 numbers it, which is not settled here.
//   * The unsafe worker-thread submit and the device-call census are gone (d3dprof's profile replaces the census).
//   * When the kit's hooks come back after frames without them, the shadow is forgotten.
#include <windows.h>
#include <d3d8.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "speedfix.h"

static const XikitHost *H;
static IDirect3DDevice8 *g_dev;
static DWORD g_render_tid;              // the thread of the first Present
static void *g_prev[97];                // per slot: the next layer (another mod, d3dprof, Direct3D)

enum {
  S_Reset = 14, S_Present = 15, S_SetRenderTarget = 31, S_BeginScene = 34, S_EndScene = 35, S_SetTransform = 37,
  S_MultiplyTransform = 39, S_SetViewport = 40, S_SetMaterial = 42, S_SetLight = 44, S_LightEnable = 46,
  S_SetRenderState = 50, S_GetRenderState = 51, S_BeginSB = 52, S_EndSB = 53, S_ApplySB = 54, S_SetTexture = 61,
  S_SetTSS = 63, S_DP = 70, S_DIP = 71, S_DPUP = 72, S_DIPUP = 73, S_SetVS = 76, S_DeleteVS = 78, S_SetStream = 83,
  S_SetIndices = 85, NSLOT = 97, ID_TEXLOCK = 116, ID_SURFLOCK = 109
};
static const char *const kName[NSLOT] = {
  "QueryInterface","AddRef","Release","TestCooperativeLevel","GetAvailableTextureMem","ResourceManagerDiscardBytes",
  "GetDirect3D","GetDeviceCaps","GetDisplayMode","GetCreationParameters","SetCursorProperties","SetCursorPosition",
  "ShowCursor","CreateAdditionalSwapChain","Reset","Present","GetBackBuffer","GetRasterStatus","SetGammaRamp",
  "GetGammaRamp","CreateTexture","CreateVolumeTexture","CreateCubeTexture","CreateVertexBuffer","CreateIndexBuffer",
  "CreateRenderTarget","CreateDepthStencilSurface","CreateImageSurface","CopyRects","UpdateTexture","GetFrontBuffer",
  "SetRenderTarget","GetRenderTarget","GetDepthStencilSurface","BeginScene","EndScene","Clear","SetTransform",
  "GetTransform","MultiplyTransform","SetViewport","GetViewport","SetMaterial","GetMaterial","SetLight","GetLight",
  "LightEnable","GetLightEnable","SetClipPlane","GetClipPlane","SetRenderState","GetRenderState","BeginStateBlock",
  "EndStateBlock","ApplyStateBlock","CaptureStateBlock","DeleteStateBlock","CreateStateBlock","SetClipStatus",
  "GetClipStatus","GetTexture","SetTexture","GetTextureStageState","SetTextureStageState","ValidateDevice","GetInfo",
  "SetPaletteEntries","GetPaletteEntries","SetCurrentTexturePalette","GetCurrentTexturePalette","DrawPrimitive",
  "DrawIndexedPrimitive","DrawPrimitiveUP","DrawIndexedPrimitiveUP","ProcessVertices","CreateVertexShader",
  "SetVertexShader","GetVertexShader","DeleteVertexShader","SetVertexShaderConstant","GetVertexShaderConstant",
  "GetVertexShaderDeclaration","GetVertexShaderFunction","SetStreamSource","GetStreamSource","SetIndices","GetIndices",
  "CreatePixelShader","SetPixelShader","GetPixelShader","DeletePixelShader","SetPixelShaderConstant",
  "GetPixelShaderConstant","GetPixelShaderFunction","DrawRectPatch","DrawTriPatch","DeletePatch"};

// ---- the shadow: what the device holds, as far as this layer has seen it -------------------------------------
static DWORD g_rs[256]; static bool g_rsV[256];
static DWORD g_tss[8][32]; static bool g_tssV[8][32];
static void *g_tex[8]; static bool g_texV[8];
static DWORD g_vs; static bool g_vsV;
static D3DMATRIX g_xf[512]; static bool g_xfV[512];
static D3DVIEWPORT8 g_vp; static bool g_vpV;
static D3DLIGHT8 g_light[32]; static bool g_lightV[32];
static BOOL g_le[32]; static bool g_leV[32];
static void *g_ssVb[16]; static UINT g_ssStride[16]; static bool g_ssV[16];
static void *g_ib; static UINT g_ibBase; static bool g_ibV;
static int g_recording;                 // between BeginStateBlock and EndStateBlock: setters are recorded, not applied
static DWORD g_curVS = 1;               // the bound shader or FVF (1: unknown), tracked whatever the switches
static DWORD g_fill, g_shade; static bool g_fillV, g_shadeV;   // FILLMODE (8), SHADEMODE (9), tracked always
static int g_frame, g_last_scene_frame = -1;
volatile int sf_frame_no, sf_hooks_frame = -1;     // speedfix.h: the frame number, the last frame whose scene ran through these hooks

static void forget_all(void) {
  memset(g_rsV, 0, sizeof g_rsV); memset(g_tssV, 0, sizeof g_tssV); memset(g_texV, 0, sizeof g_texV); g_vsV = false;
  memset(g_xfV, 0, sizeof g_xfV); g_vpV = false; memset(g_lightV, 0, sizeof g_lightV); memset(g_leV, 0, sizeof g_leV);
  memset(g_ssV, 0, sizeof g_ssV); g_ibV = false; g_fillV = g_shadeV = false;
}

// ---- figures for the log --------------------------------------------------------------------------------------
static volatile LONG g_fwd, g_skip, g_upIn, g_upOut, g_upConv, g_foreign, g_forgot_gap;

// ---- DrawPrimitiveUP batching -----------------------------------------------------------------------------------
enum { UPBUF = 1 << 20 };
static BYTE g_up[UPBUF];
static int g_upBytes, g_upPrims; static DWORD g_upStride; static bool g_upRhw;
static DWORD g_maxPrims = 65535;
typedef HRESULT (__stdcall *PFN_DPUP)(void *, D3DPRIMITIVETYPE, UINT, const void *, UINT);

static void flush(void) {
  if (!g_upPrims) return;
  int prims = g_upPrims; g_upPrims = 0; g_upBytes = 0;          // cleared first: never sent twice
  ((PFN_DPUP)g_prev[S_DPUP])(g_dev, D3DPT_TRIANGLELIST, (UINT)prims, g_up, g_upStride);
  g_upOut++;
}

static inline bool render_thread(void) { return g_render_tid && GetCurrentThreadId() == g_render_tid; }
// a call from another thread: whatever is pending goes out first, and nothing seen so far is trusted any more
static inline void foreign(void) { flush(); forget_all(); g_recording = 0; g_foreign++; }

// D3D8 FVF codes never set bit 0 (D3DFVF_RESERVED0); shader handles do. XYZRHW: position bits 0x00E == 0x004.
static inline bool fvf_rhw(DWORD h) { return !(h & 1) && (h & 0x00E) == 0x004; }
// render states that feed only the fixed-function transform and lighting, which pre-transformed (XYZRHW)
// vertices skip (speedfix_hook's list): a change cannot alter a pending RHW batch
static inline bool tnl_only(DWORD st) {
  switch (st) { case 137: case 139: case 140: case 141: case 142: case 143: case 145: case 146: case 147: case 148: case 151: return true; }
  return false;
}

// ---- generic hooks: every slot without its own hook flushes first (one 32-byte stub per slot) ---------------
static BYTE *g_stubs;
static void *g_stubTarget[NSLOT + 16];
static void __cdecl stub_enter(int id, DWORD *sp) {      // sp[0] = return address, sp[1] = this, sp[2...] = arguments
  if (!render_thread()) { if (g_render_tid) foreign(); return; }
  flush();
  switch (id) {
  case S_SetRenderTarget: g_vpV = false; break;            // SetRenderTarget resets the viewport
  case S_MultiplyTransform: if (sp[2] < 512) g_xfV[sp[2]] = false; break;
  case S_DIPUP: g_ssV[0] = false; g_ibV = false; break;    // DrawIndexedPrimitiveUP leaves stream 0 and the indices unset
  }
}
static BYTE *make_stub(int i, int id, void *target) {
  BYTE *p = g_stubs + 32 * i;
  g_stubTarget[i] = target;
  // push esp; push id; call stub_enter; add esp, 8; jmp [g_stubTarget[i]]   (EAX/ECX/EDX are scratch at a
  // __stdcall method's entry; the arguments stay on the stack untouched)
  p[0] = 0x54; p[1] = 0x6A; p[2] = (BYTE)id;
  p[3] = 0xE8; *(DWORD *)(p + 4) = (DWORD)((BYTE *)(void *)&stub_enter - (p + 8));
  p[8] = 0x83; p[9] = 0xC4; p[10] = 0x08;
  p[11] = 0xFF; p[12] = 0x25; *(DWORD *)(p + 13) = (DWORD)(uintptr_t)&g_stubTarget[i];
  memset(p + 17, 0xCC, 15);
  return p;
}

// texture and surface LockRect: the other way a pending batch's inputs can change (a glyph texture written between
// two text draws). Their vtables (one per resource class of the driver) get a flushing stub in that slot, once.
static void **g_guardVt[16]; static int g_nguard;
static bool write_slot(void **slot, void *v) {
  DWORD old;
  if (!VirtualProtect(slot, sizeof(void *), PAGE_EXECUTE_READWRITE, &old)) return false;
  InterlockedExchangePointer((PVOID *)slot, v);
  VirtualProtect(slot, sizeof(void *), old, &old);
  return *slot == v;
}
static void guard(void **vt, int slot, int id) {
  for (int i = 0; i < g_nguard; i++) if (g_guardVt[i] == vt) return;
  if (g_nguard >= 16 || !vt[slot]) return;
  BYTE *s = make_stub(NSLOT + g_nguard, id, vt[slot]);
  FlushInstructionCache(GetCurrentProcess(), s, 32);
  if (write_slot(&vt[slot], s)) { g_guardVt[g_nguard++] = vt; H->log("speedfix: LockRect of vtable %p (slot %d) flushes pending draws first", (void *)vt, slot); }
}
static void note_texture(IDirect3DBaseTexture8 *t) {
  if (!t || g_nguard >= 16) return;
  void **vt = *(void ***)t;
  for (int i = 0; i < g_nguard; i++) if (g_guardVt[i] == vt) return;
  D3DRESOURCETYPE ty = t->GetType();
  if (ty != D3DRTYPE_TEXTURE && ty != D3DRTYPE_CUBETEXTURE && ty != D3DRTYPE_VOLUMETEXTURE) return;
  guard(vt, 16, ID_TEXLOCK);                                // LockRect / LockBox
  IDirect3DSurface8 *s = NULL;
  if (ty == D3DRTYPE_TEXTURE) ((IDirect3DTexture8 *)t)->GetSurfaceLevel(0, &s);
  else if (ty == D3DRTYPE_CUBETEXTURE) ((IDirect3DCubeTexture8 *)t)->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_X, 0, &s);
  if (s) { guard(*(void ***)s, 9, ID_SURFLOCK); s->Release(); }
}

// ---- hooks with their own logic ----------------------------------------------------------------------------------
typedef HRESULT (__stdcall *PFN_Reset)(void *, D3DPRESENT_PARAMETERS *);
typedef HRESULT (__stdcall *PFN_Present)(void *, const RECT *, const RECT *, HWND, const RGNDATA *);
typedef HRESULT (__stdcall *PFN_Dev)(void *);
typedef HRESULT (__stdcall *PFN_D1)(void *, DWORD);
typedef HRESULT (__stdcall *PFN_D2)(void *, DWORD, DWORD);
typedef HRESULT (__stdcall *PFN_D3)(void *, DWORD, DWORD, DWORD);
typedef HRESULT (__stdcall *PFN_DP)(void *, DWORD, const void *);
typedef HRESULT (__stdcall *PFN_P)(void *, const void *);
typedef HRESULT (__stdcall *PFN_DPtr)(void *, DWORD *);
typedef HRESULT (__stdcall *PFN_SS)(void *, UINT, void *, UINT);
typedef HRESULT (__stdcall *PFN_SI)(void *, void *, UINT);
typedef HRESULT (__stdcall *PFN_GetRS)(void *, DWORD, DWORD *);
typedef HRESULT (__stdcall *PFN_DPr4)(void *, D3DPRIMITIVETYPE, UINT, UINT);
typedef HRESULT (__stdcall *PFN_DIP6)(void *, D3DPRIMITIVETYPE, UINT, UINT, UINT, UINT);
#define PREV(T, s) ((T)g_prev[s])

static HRESULT __stdcall h_Reset(void *d, D3DPRESENT_PARAMETERS *pp) {
  flush(); forget_all();
  HRESULT hr = PREV(PFN_Reset, S_Reset)(d, pp);
  forget_all(); g_curVS = 1; g_recording = 0;   // Reset returns the device to its default state
  return hr;
}
static HRESULT __stdcall h_Present(void *d, const RECT *a, const RECT *b, HWND w, const RGNDATA *r) {
  if (!g_render_tid) { g_render_tid = GetCurrentThreadId(); H->log("speedfix: render thread %lu", (unsigned long)g_render_tid); }
  else if (!render_thread()) foreign();
  flush();
  return PREV(PFN_Present, S_Present)(d, a, b, w, r);
}
// Scene boundaries. State set by something that calls the device below this layer cannot be seen here; forgetting
// the shadow at each scene bounds such a difference to one scene (speedfix_hook's state_cache_reset_each_scene).
// Frames that passed without these hooks (the kit turns mod hooks off for F9 and a trace's unswapped frames) set
// state the shadow never saw: then it is forgotten whatever the switch.
static HRESULT __stdcall h_BeginScene(void *d) {
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_Dev, S_BeginScene)(d); }
  flush();
  if (g_last_scene_frame >= 0 && g_frame - g_last_scene_frame > 1) { forget_all(); g_forgot_gap++; }
  g_last_scene_frame = g_frame;
  sf_hooks_frame = g_frame;
  HRESULT hr = PREV(PFN_Dev, S_BeginScene)(d);
  if (sf_cfg.scene_reset) forget_all();
  return hr;
}
static HRESULT __stdcall h_EndScene(void *d) {
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_Dev, S_EndScene)(d); }
  flush();
  HRESULT hr = PREV(PFN_Dev, S_EndScene)(d);
  if (sf_cfg.scene_reset) forget_all();
  return hr;
}
static HRESULT __stdcall h_BeginSB(void *d) {
  if (!render_thread() && g_render_tid) foreign(); else flush();
  HRESULT hr = PREV(PFN_Dev, S_BeginSB)(d); if (SUCCEEDED(hr)) g_recording = 1; return hr;
}
static HRESULT __stdcall h_EndSB(void *d, DWORD *tok) {
  if (!render_thread() && g_render_tid) foreign(); else flush();
  g_recording = 0; return PREV(PFN_DPtr, S_EndSB)(d, tok);
}
static HRESULT __stdcall h_ApplySB(void *d, DWORD tok) {
  if (!render_thread() && g_render_tid) foreign(); else flush();
  HRESULT hr = PREV(PFN_D1, S_ApplySB)(d, tok);
  forget_all(); g_curVS = 1;                     // a block may set any state, the shader too
  return hr;
}

static HRESULT __stdcall h_SetRenderState(void *d, D3DRENDERSTATETYPE st, DWORD v) {
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_D2, S_SetRenderState)(d, st, v); }
  bool use = sf_cfg.state_cache && !g_recording && (unsigned)st < 256;
  if (use && g_rsV[st] && g_rs[st] == v) { g_skip++; return D3D_OK; }
  if (!(g_upPrims && g_upRhw && tnl_only(st))) flush();      // a real change: what is pending goes out first
  g_fwd++;
  HRESULT hr = PREV(PFN_D2, S_SetRenderState)(d, st, v);
  if ((unsigned)st < 256 && !g_recording) {
    bool ok = SUCCEEDED(hr);
    g_rs[st] = v; g_rsV[st] = ok && sf_cfg.state_cache;
    if (st == D3DRS_FILLMODE) { g_fill = v; g_fillV = ok; }
    if (st == D3DRS_SHADEMODE) { g_shade = v; g_shadeV = ok; }
  }
  return hr;
}
static HRESULT __stdcall h_SetTexture(void *d, DWORD stage, IDirect3DBaseTexture8 *t) {
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_DP, S_SetTexture)(d, stage, t); }
  bool use = sf_cfg.state_cache && !g_recording && stage < 8;
  if (use && g_texV[stage] && g_tex[stage] == t) { g_skip++; return D3D_OK; }
  flush();
  if (sf_cfg.batch_up) note_texture(t);
  g_fwd++;
  HRESULT hr = PREV(PFN_DP, S_SetTexture)(d, stage, t);
  if (stage < 8 && !g_recording) { g_tex[stage] = t; g_texV[stage] = SUCCEEDED(hr) && sf_cfg.state_cache; }
  return hr;
}
static HRESULT __stdcall h_SetTSS(void *d, DWORD stage, D3DTEXTURESTAGESTATETYPE ty, DWORD v) {
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_D3, S_SetTSS)(d, stage, ty, v); }
  bool use = sf_cfg.state_cache && !g_recording && stage < 8 && (unsigned)ty < 32;
  if (use && g_tssV[stage][ty] && g_tss[stage][ty] == v) { g_skip++; return D3D_OK; }
  flush();
  g_fwd++;
  HRESULT hr = PREV(PFN_D3, S_SetTSS)(d, stage, ty, v);
  if (stage < 8 && (unsigned)ty < 32 && !g_recording) { g_tss[stage][ty] = v; g_tssV[stage][ty] = SUCCEEDED(hr) && sf_cfg.state_cache; }
  return hr;
}
static HRESULT __stdcall h_SetVS(void *d, DWORD h) {
  if (!render_thread()) { if (g_render_tid) foreign(); HRESULT hr = PREV(PFN_D1, S_SetVS)(d, h); g_curVS = 1; return hr; }
  bool use = sf_cfg.vs_cache && !g_recording;
  if (use && g_vsV && g_vs == h) { g_skip++; return D3D_OK; }
  flush();
  g_fwd++;
  HRESULT hr = PREV(PFN_D1, S_SetVS)(d, h);
  if (!g_recording) { if (SUCCEEDED(hr)) g_curVS = h; g_vs = h; g_vsV = SUCCEEDED(hr) && sf_cfg.vs_cache; }
  return hr;
}
// a deleted shader's handle can be given out again by the next CreateVertexShader
static HRESULT __stdcall h_DeleteVS(void *d, DWORD h) {
  if (!render_thread()) { if (g_render_tid) foreign(); }
  else flush();
  if (g_vs == h) g_vsV = false;
  HRESULT hr = PREV(PFN_D1, S_DeleteVS)(d, h);
  if (g_vs == h) g_vsV = false;
  if (g_curVS == h) g_curVS = 1;
  return hr;
}

// the extended cache (ext_cache): the shadow is kept always on the render thread, a repeat is skipped only with
// the switch on
static HRESULT __stdcall h_SetTransform(void *d, D3DTRANSFORMSTATETYPE st, const D3DMATRIX *m) {
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_DP, S_SetTransform)(d, st, m); }
  if (!g_recording && (unsigned)st < 512 && m) {
    if (g_xfV[st] && !memcmp(&g_xf[st], m, sizeof *m) && sf_cfg.ext_cache) { g_skip++; return D3D_OK; }
  }
  // view, projection and world matrices do not touch pre-transformed vertices; texture transforms do
  bool geom = st == D3DTS_VIEW || st == D3DTS_PROJECTION || ((unsigned)st >= 256 && (unsigned)st < 512);
  if (!(g_upPrims && g_upRhw && geom)) flush();
  g_fwd++;
  HRESULT hr = PREV(PFN_DP, S_SetTransform)(d, st, m);
  if (!g_recording && (unsigned)st < 512) { if (m && SUCCEEDED(hr)) { g_xf[st] = *m; g_xfV[st] = true; } else g_xfV[st] = false; }
  return hr;
}
static HRESULT __stdcall h_SetViewport(void *d, const D3DVIEWPORT8 *v) {
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_P, S_SetViewport)(d, v); }
  if (!g_recording && v && g_vpV && !memcmp(&g_vp, v, sizeof *v) && sf_cfg.ext_cache) { g_skip++; return D3D_OK; }
  flush(); g_fwd++;
  HRESULT hr = PREV(PFN_P, S_SetViewport)(d, v);
  if (!g_recording) { if (v && SUCCEEDED(hr)) { g_vp = *v; g_vpV = true; } else g_vpV = false; }
  return hr;
}
static HRESULT __stdcall h_SetMaterial(void *d, const D3DMATERIAL8 *m) {
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_P, S_SetMaterial)(d, m); }
  if (!(g_upPrims && g_upRhw)) flush();                   // materials feed lighting only
  return PREV(PFN_P, S_SetMaterial)(d, m);
}
static HRESULT __stdcall h_SetLight(void *d, DWORD i, const D3DLIGHT8 *l) {
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_DP, S_SetLight)(d, i, l); }
  if (!g_recording && i < 32 && l && g_lightV[i] && !memcmp(&g_light[i], l, sizeof *l) && sf_cfg.ext_cache) { g_skip++; return D3D_OK; }
  if (!(g_upPrims && g_upRhw)) flush();                   // lights never reach pre-transformed vertices
  g_fwd++;
  HRESULT hr = PREV(PFN_DP, S_SetLight)(d, i, l);
  if (!g_recording && i < 32) { if (l && SUCCEEDED(hr)) { g_light[i] = *l; g_lightV[i] = true; } else g_lightV[i] = false; }
  return hr;
}
static HRESULT __stdcall h_LightEnable(void *d, DWORD i, BOOL on) {
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_D2, S_LightEnable)(d, i, on); }
  if (!g_recording && i < 32 && g_leV[i] && (g_le[i] != 0) == (on != 0) && sf_cfg.ext_cache) { g_skip++; return D3D_OK; }
  if (!(g_upPrims && g_upRhw)) flush();
  g_fwd++;
  HRESULT hr = PREV(PFN_D2, S_LightEnable)(d, i, on);
  if (!g_recording && i < 32) { g_le[i] = on; g_leV[i] = SUCCEEDED(hr); }
  return hr;
}
// re-binding the buffer the device already holds a reference to
static HRESULT __stdcall h_SetStream(void *d, UINT n, IDirect3DVertexBuffer8 *vb, UINT stride) {
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_SS, S_SetStream)(d, n, vb, stride); }
  if (!g_recording && n < 16 && g_ssV[n] && g_ssVb[n] == vb && g_ssStride[n] == stride && sf_cfg.ext_cache) { g_skip++; return D3D_OK; }
  flush(); g_fwd++;
  HRESULT hr = PREV(PFN_SS, S_SetStream)(d, n, vb, stride);
  if (!g_recording && n < 16) { g_ssVb[n] = vb; g_ssStride[n] = stride; g_ssV[n] = SUCCEEDED(hr); }
  return hr;
}
static HRESULT __stdcall h_SetIndices(void *d, IDirect3DIndexBuffer8 *ib, UINT base) {
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_SI, S_SetIndices)(d, ib, base); }
  if (!g_recording && g_ibV && g_ib == ib && g_ibBase == base && sf_cfg.ext_cache) { g_skip++; return D3D_OK; }
  flush(); g_fwd++;
  HRESULT hr = PREV(PFN_SI, S_SetIndices)(d, ib, base);
  if (!g_recording) { g_ib = ib; g_ibBase = base; g_ibV = SUCCEEDED(hr); }
  return hr;
}

// the state a strip or fan may be converted under (see the head of the file); read from the device when unknown
static bool known_rs(D3DRENDERSTATETYPE st, DWORD *v, bool *valid) {
  if (!*valid) {
    DWORD x = 0;
    if (FAILED(PREV(PFN_GetRS, S_GetRenderState)(g_dev, st, &x))) return false;
    *v = x; *valid = true;
  }
  return true;
}
static bool may_convert(void) {
  return known_rs(D3DRS_FILLMODE, &g_fill, &g_fillV) && g_fill == D3DFILL_SOLID &&
         known_rs(D3DRS_SHADEMODE, &g_shade, &g_shadeV) && g_shade == D3DSHADE_GOURAUD;
}
static int verts_for(D3DPRIMITIVETYPE t, UINT n) {
  switch (t) { case D3DPT_TRIANGLELIST: return (int)n * 3; case D3DPT_TRIANGLESTRIP: case D3DPT_TRIANGLEFAN: return (int)n + 2; default: return -1; }
}
// a strip's odd triangles are wound the other way (D3D8 flips them); (i, i+2, i+1) is that same winding
static void expand(D3DPRIMITIVETYPE t, UINT n, const BYTE *src, UINT stride, BYTE *dst) {
  if (t == D3DPT_TRIANGLELIST) { memcpy(dst, src, (size_t)n * 3 * stride); return; }
  for (UINT i = 0; i < n; i++) {
    UINT a, b, c;
    if (t == D3DPT_TRIANGLESTRIP) { a = i; b = (i & 1) ? i + 2 : i + 1; c = (i & 1) ? i + 1 : i + 2; }
    else { a = 0; b = i + 1; c = i + 2; }
    memcpy(dst, src + a * stride, stride); memcpy(dst + stride, src + b * stride, stride); memcpy(dst + 2 * stride, src + c * stride, stride);
    dst += 3 * stride;
  }
}
static HRESULT __stdcall h_DPUP(void *d, D3DPRIMITIVETYPE t, UINT prims, const void *data, UINT stride) {
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_DPUP, S_DPUP)(d, t, prims, data, stride); }
  g_upIn++;
  g_ssV[0] = false;                                // DrawPrimitiveUP leaves stream 0 unset (D3D8)
  DWORD maxP = sf_cfg.batch_max_prims > 0 && (DWORD)sf_cfg.batch_max_prims < g_maxPrims ? (DWORD)sf_cfg.batch_max_prims : g_maxPrims;
  int maxB = sf_cfg.batch_max_bytes > 0 && sf_cfg.batch_max_bytes < UPBUF ? sf_cfg.batch_max_bytes : UPBUF;
  int outBytes = (int)prims * 3 * (int)stride;
  bool ok = sf_cfg.batch_up && !g_recording && data && stride && stride <= 256 && prims && prims <= maxP &&
            verts_for(t, prims) > 0 && outBytes <= maxB && (t == D3DPT_TRIANGLELIST || may_convert());
  if (!ok) { flush(); g_upOut++; return PREV(PFN_DPUP, S_DPUP)(d, t, prims, data, stride); }
  if (g_upPrims && (stride != g_upStride || g_upBytes + outBytes > maxB || (DWORD)g_upPrims + prims > maxP)) flush();
  if (t != D3DPT_TRIANGLELIST) g_upConv++;
  expand(t, prims, (const BYTE *)data, stride, g_up + g_upBytes);  // the caller's data is only valid during the call
  if (!g_upPrims) g_upRhw = fvf_rhw(g_curVS);
  g_upBytes += outBytes; g_upPrims += (int)prims; g_upStride = stride;
  return D3D_OK;
}

// with batch_up_draws and extended_state_cache both off at the start, only these are hooked
static int g_full;
static bool core_slot(int s) {
  switch (s) {
  case S_Reset: case S_Present: case S_BeginScene: case S_EndScene: case S_BeginSB: case S_EndSB: case S_ApplySB:
  case S_SetRenderState: case S_SetTexture: case S_SetTSS: case S_SetVS: case S_DeleteVS: return true;
  }
  return false;
}

// ---- installation --------------------------------------------------------------------------------------------
static void *hook_for(int s) {
  switch (s) {
  case S_Reset: return (void *)h_Reset; case S_Present: return (void *)h_Present;
  case S_BeginScene: return (void *)h_BeginScene; case S_EndScene: return (void *)h_EndScene;
  case S_BeginSB: return (void *)h_BeginSB; case S_EndSB: return (void *)h_EndSB; case S_ApplySB: return (void *)h_ApplySB;
  case S_SetRenderState: return (void *)h_SetRenderState; case S_SetTexture: return (void *)h_SetTexture;
  case S_SetTSS: return (void *)h_SetTSS; case S_SetVS: return (void *)h_SetVS; case S_DeleteVS: return (void *)h_DeleteVS;
  case S_SetTransform: return (void *)h_SetTransform; case S_SetViewport: return (void *)h_SetViewport;
  case S_SetMaterial: return (void *)h_SetMaterial; case S_SetLight: return (void *)h_SetLight;
  case S_LightEnable: return (void *)h_LightEnable; case S_SetStream: return (void *)h_SetStream;
  case S_SetIndices: return (void *)h_SetIndices; case S_DPUP: return (void *)h_DPUP;
  }
  return NULL;
}

extern "C" int sf_device_install(const XikitHost *host) {
  H = host; g_dev = (IDirect3DDevice8 *)host->device;
  if (host->size < offsetof(XikitHost, device_hook) + sizeof(void *)) { host->log("speedfix: the kit is older than API 2 (no device hooks)"); return 1; }
  g_stubs = (BYTE *)VirtualAlloc(NULL, 32 * (NSLOT + 16), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
  if (!g_stubs) { host->log("speedfix: no memory for its stubs"); return 1; }
  D3DCAPS8 caps;
  if (g_dev && SUCCEEDED(g_dev->GetDeviceCaps(&caps)) && caps.MaxPrimitiveCount)
    g_maxPrims = caps.MaxPrimitiveCount < 65535 ? caps.MaxPrimitiveCount : 65535;
  forget_all();
  int hooked = 0, stubs = 0;
  // Every hooked method costs a pass through the kit's front layer and this one on each call, which d3dprof (behind
  // them) cannot see. Batching needs every method (each flushes what is pending first) and the extended cache needs
  // its setters; with both off at the start only the cached setters and the methods that change or bound their
  // state are hooked (the owner's first capture: some 20,000 calls a frame were otherwise passed through for nothing)
  g_full = sf_cfg.batch_up || sf_cfg.ext_cache;
  // 0-2 (QueryInterface, AddRef, Release) do not render. Every other method: its own hook, or a flushing stub
  for (int s = 3; s < NSLOT; s++) {
    if (!g_full && !core_slot(s)) continue;
    void *h = hook_for(s), *prev = NULL;
    if (h) {
      if (host->device_hook(kName[s], s, h, &prev) == XIKIT_OK) { g_prev[s] = prev; hooked++; }
      else { host->log("speedfix: %s not hooked; the device layer stays off", kName[s]); return 1; }
    } else {
      // the stub jumps on through its own target slot, which device_hook fills with the next layer
      BYTE *st = make_stub(s, s, NULL);
      if (host->device_hook(kName[s], s, st, &g_stubTarget[s]) == XIKIT_OK) { g_prev[s] = g_stubTarget[s]; stubs++; }
      else { host->log("speedfix: %s not hooked; the device layer stays off", kName[s]); return 1; }
    }
  }
  FlushInstructionCache(GetCurrentProcess(), g_stubs, 32 * (NSLOT + 16));
  host->log("speedfix: %d device methods hooked, %d flush guards; MaxPrimitiveCount %lu%s", hooked, stubs, (unsigned long)g_maxPrims,
            g_full ? "" : " (batching and the extended cache were off at the start: turning either on takes effect at the next start)");
  return 0;
}

// a cache turned on starts from an empty shadow: nothing recorded while it was off is trusted
extern "C" void sf_device_config_changed(const SfConfig *b) {
  if (!g_full && (sf_cfg.batch_up || sf_cfg.ext_cache)) {
    if (sf_cfg.batch_up != b->batch_up || sf_cfg.ext_cache != b->ext_cache)
      H->log("speedfix: batch_up_draws / extended_state_cache take effect at the next start (they were off at this one, so their hooks are not in place)");
    sf_cfg.batch_up = 0; sf_cfg.ext_cache = 0;
  }
  if (sf_cfg.state_cache && !b->state_cache) { memset(g_rsV, 0, sizeof g_rsV); memset(g_tssV, 0, sizeof g_tssV); memset(g_texV, 0, sizeof g_texV); }
  if (sf_cfg.vs_cache && !b->vs_cache) g_vsV = false;
  if (!sf_cfg.batch_up && b->batch_up) flush();
}

// ---- frame pacing (render thread, before Present) ---------------------------------------------------------------
static LARGE_INTEGER g_qpf; static LONGLONG g_next; static HANDLE g_timer; static bool g_timerTried; static LONGLONG g_lastQpc;
static LONG g_pcN, g_pcLate; static double g_pcSum, g_pcSum2, g_pcMax;
extern "C" void sf_device_frame(void) {
  g_frame++; sf_frame_no = g_frame;
  if (!g_qpf.QuadPart) QueryPerformanceFrequency(&g_qpf);
  LARGE_INTEGER now; QueryPerformanceCounter(&now);
  int fps = sf_cfg.frame_cap_fps;
  if (fps > 0) {
    LONGLONG period = g_qpf.QuadPart / fps;
    if (!g_next || now.QuadPart - g_next > period) g_next = now.QuadPart;   // late by over a frame: re-anchor
    LONGLONG remain = g_next - now.QuadPart;
    if (remain > 0) {
      double ms = remain * 1000.0 / (double)g_qpf.QuadPart;
      if (!g_timerTried) {
        g_timerTried = true;
        typedef HANDLE (WINAPI *CWTEx)(LPSECURITY_ATTRIBUTES, LPCWSTR, DWORD, DWORD);
        CWTEx f = (CWTEx)(void *)GetProcAddress(GetModuleHandleA("kernel32.dll"), "CreateWaitableTimerExW");
        g_timer = f ? f(NULL, NULL, 0x2 /* CREATE_WAITABLE_TIMER_HIGH_RESOLUTION */, TIMER_ALL_ACCESS) : NULL;
      }
      if (ms > 1.5 && g_timer) {                 // sleep most of it on a high-resolution timer, spin the last ~1 ms
        LARGE_INTEGER due; due.QuadPart = -(LONGLONG)((ms - 1.0) * 10000.0);
        if (SetWaitableTimer(g_timer, &due, 0, NULL, NULL, FALSE)) WaitForSingleObject(g_timer, 200);
      }
      LARGE_INTEGER t;
      do { YieldProcessor(); QueryPerformanceCounter(&t); } while (t.QuadPart < g_next);
      now = t;
    }
    g_next += period;
  } else g_next = 0;
  if (g_lastQpc) {
    double iv = (now.QuadPart - g_lastQpc) * 1000.0 / (double)g_qpf.QuadPart;
    g_pcN++; g_pcSum += iv; g_pcSum2 += iv * iv; if (iv > g_pcMax) g_pcMax = iv;
    if (fps > 0 && iv > 1500.0 / fps) g_pcLate++;
  }
  g_lastQpc = now.QuadPart;
}

extern "C" void sf_device_stats(void) {
  LONG f = InterlockedExchange(&g_fwd, 0), s = InterlockedExchange(&g_skip, 0);
  LONG in = InterlockedExchange(&g_upIn, 0), out = InterlockedExchange(&g_upOut, 0), cv = InterlockedExchange(&g_upConv, 0);
  LONG fo = InterlockedExchange(&g_foreign, 0), gap = InterlockedExchange(&g_forgot_gap, 0);
  if (f + s) H->log("speedfix: cached setters: %ld forwarded, %ld skipped (%.1f%%)", f, s, 100.0 * s / (double)(f + s));
  if (in) H->log("speedfix: DrawPrimitiveUP: %ld from the game, %ld sent (%.1f%% fewer), %ld strips/fans converted%s",
                 in, out, 100.0 * (in - out) / (double)in, cv, sf_cfg.batch_up ? "" : " [batching off]");
  if (fo) H->log("speedfix: %ld device calls from other threads (each made the shadow unknown)", fo);
  if (gap) H->log("speedfix: shadow forgotten %ld times after frames without the hooks (F9, a trace)", gap);
  if (g_pcN > 1) {
    double mean = g_pcSum / g_pcN, var = g_pcSum2 / g_pcN - mean * mean;
    H->log("speedfix: frames %ld, mean %.2f ms, sd %.2f, max %.1f%s", g_pcN, mean, var > 0 ? sqrt(var) : 0.0, g_pcMax,
           sf_cfg.frame_cap_fps ? "" : " (no cap)");
    if (sf_cfg.frame_cap_fps) H->log("speedfix: cap %d fps: %ld frames late (over 1.5 periods)", sf_cfg.frame_cap_fps, g_pcLate);
  }
  g_pcN = g_pcLate = 0; g_pcSum = g_pcSum2 = g_pcMax = 0;
}
