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
// New levers (each off by default; kit/speedfix/README.md):
//   * lazy_state: SetRenderState / SetTextureStageState / SetTexture are held until a method that reads that state
//     runs (a draw, Clear, a Get, a state block, the scene and Present). D3D8 reads that state when it draws, not when
//     it is set, so the device sees the same state at every draw. A change undone before then (the game's
//     Cell_DrawQuadBlended sets ALPHABLENDENABLE 0 after each quad and 1 before the next) never reaches the device.
//     A held texture is AddRef'd until it is sent or the change is undone. What a held call returns is D3D_OK: a
//     failure when it is sent is not reported to the game (the shadow is then left unknown).
//   * vs_dedupe: CreateVertexShader with no function (the fixed-function pipeline, which a declaration and usage
//     define completely: DirectX 8 SDK, CreateVertexShader) of a declaration already made returns that shader's handle
//     again; DeleteVertexShader deletes it when the last of them is deleted. The owner's sessions: ~1,450 creations,
//     3 distinct shaders (d3dprof's shader catalogue).
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
  S_SetIndices = 85, S_CaptureSB = 55, S_DeleteSB = 56, S_CreateSB = 57, NSLOT = 97, ID_TEXLOCK = 116, ID_SURFLOCK = 109
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
static bool g_recOk;                    // the block being recorded is followed here (block_cache, below)
static DWORD g_curVS = 1;               // the bound shader or FVF (1: unknown), tracked whatever the switches
static DWORD g_fill, g_shade; static bool g_fillV, g_shadeV;   // FILLMODE (8), SHADEMODE (9), tracked always
static int g_frame, g_last_scene_frame = -1;
static bool g_motion;                   // motion_stats at the start (its hooks are installed then)
static bool g_sceneHeld; static void *g_heldDev; static volatile LONG g_mergedEnds;   // merge_scenes
volatile int sf_frame_no, sf_hooks_frame = -1;     // speedfix.h: the frame number, the last frame whose scene ran through these hooks

static void forget_all(void) {
  memset(g_rsV, 0, sizeof g_rsV); memset(g_tssV, 0, sizeof g_tssV); memset(g_texV, 0, sizeof g_texV); g_vsV = false;
  memset(g_xfV, 0, sizeof g_xfV); g_vpV = false; memset(g_lightV, 0, sizeof g_lightV); memset(g_leV, 0, sizeof g_leV);
  memset(g_ssV, 0, sizeof g_ssV); g_ibV = false; g_fillV = g_shadeV = false;
}

// ---- figures for the log --------------------------------------------------------------------------------------
static volatile LONG g_fwd, g_skip, g_upIn, g_upOut, g_upConv, g_foreign, g_forgot_gap;
static volatile LONG g_nullDrawn;      // null_draws (below): draw calls not sent

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
static void apply_pending(void);
// a call from another thread: whatever is pending goes out first, and nothing seen so far is trusted any more
static inline void foreign(void) { flush(); apply_pending(); forget_all(); g_recording = 0; g_recOk = false; g_foreign++; }

// skin_merge=2 (below, "grouping"): inside one SkinAndDrawV2, the calls are held and replayed through these hooks
enum { G_RS, G_TSS, G_TEX, G_VS, G_XF, G_MAT, G_LIGHT, G_LEN, G_SS, G_SI, G_VP, G_DIP };
static bool g_grp;                      // holding (render thread, between sf_group_begin and sf_group_end)
static bool grp_hold(int ty, DWORD a, DWORD b, DWORD c, DWORD d, DWORD e, const void *blob, int nb);
static void grp_order(void);            // what is held goes out as the game made it, in its order

// ---- lazy_state: changes held until something reads them --------------------------------------------------------
// Keys: 0..255 render states, PK_TSS + stage * 32 + type texture stage states, PK_TEX + stage textures. Each held key
// is in g_pl once (g_pos: where; an undone change leaves -1 there), in the order it was first held.
enum { PK_TSS = 256, PK_TEX = 512, NPK = 520, PLIST = 1024 };
static bool g_isP[NPK]; static DWORD g_pv[NPK]; static short g_pos[NPK];
static short g_pl[PLIST]; static int g_npl, g_live;
static int g_level;                     // the hooks at this start: 0 core, 1 core and every method that reads state, 2 all
static volatile LONG g_held, g_sent, g_dropped;
static inline bool lazy_on(void) { return sf_cfg.lazy_state && g_level >= 1 && !g_recording; }
static void pend(int k, DWORD v) {
  if (g_npl == PLIST) {                 // squeeze out undone entries (at most NPK are live)
    int j = 0;
    for (int i = 0; i < g_npl; i++) if (g_pl[i] >= 0) { g_pos[g_pl[i]] = (short)j; g_pl[j++] = g_pl[i]; }
    g_npl = j;
  }
  g_isP[k] = true; g_pv[k] = v; g_pos[k] = (short)g_npl; g_pl[g_npl++] = (short)k; g_live++; g_held++;
}
static void unpend(int k) {             // undone before anything read it
  g_isP[k] = false; g_pl[g_pos[k]] = -1; g_live--; g_dropped++;
  if (k >= PK_TEX && g_pv[k]) ((IUnknown *)(uintptr_t)g_pv[k])->Release();
}
static void drop_pending(void) {        // Reset: the device returns to its defaults, what was held is moot
  for (int i = 0; i < g_npl; i++) { int k = g_pl[i]; if (k >= 0 && g_isP[k]) unpend(k); }
  g_npl = 0; g_live = 0;
}

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
// Methods that neither read nor change render states, texture stage states or texture bindings (they set or get
// other state, or make resources): a held change need not go out before them. Every other method is taken to read
// them (Clear included, though which states it reads is not settled here). With lazy_state at the start, only these
// are left unhooked (they carry most of the calls: transforms, lights, streams, shader constants).
static bool other_state(int s) {
  switch (s) {
  case 20: case 21: case 22: case 23: case 24: case 25: case 26: case 27:          // Create* resources
  case 37: case 38: case 39: case 40: case 41: case 42: case 43: case 44: case 45: case 46: case 47: case 48: case 49:
  case 75: case 76: case 77: case 78: case 79: case 80: case 81: case 82:          // vertex shaders and their constants
  case 83: case 84: case 85: case 86:                                              // streams, indices
  case 87: case 88: case 89: case 90: case 91: case 92: case 93:                   // pixel shaders and their constants
    return true;
  }
  return false;
}
static void close_held(void);
static void mot_count(int id);
static void __cdecl stub_enter(int id, DWORD *sp) {      // sp[0] = return address, sp[1] = this, sp[2...] = arguments
  if (g_grp) grp_order();
  if (!render_thread()) { if (g_render_tid) foreign(); return; }
  flush();
  if (g_npl && id < NSLOT && !other_state(id)) apply_pending();
  if (id == 28 || id == 29 || id == 30) close_held();     // CopyRects, UpdateTexture, GetFrontBuffer: outside a merged scene
  if (g_motion) mot_count(id);
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

// ---- block_cache: what a recorded state block sets --------------------------------------------------------------
// Microsoft's reference for IDirect3DDevice8::BeginStateBlock: the setters called between BeginStateBlock and
// EndStateBlock are recorded in the block ("If the same state is specified multiple times in a state block, only the
// last value is used"); ApplyStateBlock applies it. The game records its blocks that way, once each, from fixed
// tables (StateBlock_ApplyTSS 0x1006AF90, 0x1006AF00; docs/HISTORY.md 11 Oct). Each block recorded on the render thread
// is followed here: its render states and texture stage states with their values, the texture stages it binds, and
// whether it records any other setter this layer sees (the shader; transforms, viewport, lights, streams and indices
// when they are hooked). With block_cache on, applying a followed block sets those states in the shadow, makes its
// texture stages and (if it had any) those other kinds unknown, and keeps the rest; any other block forgets
// everything, as before. Textures are not taken from a block: whether the block's reference keeps a texture alive
// is not established here, so a stage it binds is made unknown. A block is no longer followed after DeleteStateBlock,
// CaptureStateBlock (it takes new values), CreateStateBlock returning its token, or Reset (which loses all state).
enum { BLK_MAX = 64, BLK_N = 256 };
struct Blk { DWORD tok; bool used, other; int n; short key[BLK_N]; DWORD val[BLK_N]; };
static Blk g_blk[BLK_MAX], g_rec;
static volatile LONG g_blkKnown, g_blkForgot;
static void rec_add(int k, DWORD v) {
  if (!g_recOk) return;
  if (g_rec.n == BLK_N) { g_recOk = false; return; }
  g_rec.key[g_rec.n] = (short)k; g_rec.val[g_rec.n++] = v;
}
static inline void rec_other(void) { if (g_recording) g_rec.other = true; }
static Blk *blk_find(DWORD t) { for (int i = 0; i < BLK_MAX; i++) if (g_blk[i].used && g_blk[i].tok == t) return &g_blk[i]; return NULL; }
static void blk_drop(DWORD t) { Blk *b = blk_find(t); if (b) b->used = false; }
static void blk_drop_all(void) { for (int i = 0; i < BLK_MAX; i++) g_blk[i].used = false; }
static void blk_keep(DWORD t) {
  blk_drop(t);
  for (int i = 0; i < BLK_MAX; i++) if (!g_blk[i].used) { g_blk[i] = g_rec; g_blk[i].tok = t; g_blk[i].used = true; return; }
}

// ---- sending a set call on to the device, with the shadow updated (the setters and apply_pending) ---------------
static inline bool cache_on(void) { return sf_cfg.state_cache || sf_cfg.lazy_state; }
static HRESULT fwd_rs(DWORD st, DWORD v) {
  g_fwd++;
  HRESULT hr = PREV(PFN_D2, S_SetRenderState)(g_dev, st, v);
  if (st < 256 && !g_recording) {
    bool ok = SUCCEEDED(hr);
    g_rs[st] = v; g_rsV[st] = ok && cache_on();
    if (st == D3DRS_FILLMODE) { g_fill = v; g_fillV = ok; }
    if (st == D3DRS_SHADEMODE) { g_shade = v; g_shadeV = ok; }
  } else if (g_recording) { if (st < 256 && SUCCEEDED(hr)) rec_add((int)st, v); else g_recOk = false; }
  return hr;
}
static HRESULT fwd_tss(DWORD stage, DWORD ty, DWORD v) {
  g_fwd++;
  HRESULT hr = PREV(PFN_D3, S_SetTSS)(g_dev, stage, ty, v);
  if (stage < 8 && ty < 32 && !g_recording) { g_tss[stage][ty] = v; g_tssV[stage][ty] = SUCCEEDED(hr) && cache_on(); }
  else if (g_recording) { if (stage < 8 && ty < 32 && SUCCEEDED(hr)) rec_add(PK_TSS + (int)stage * 32 + (int)ty, v); else g_recOk = false; }
  return hr;
}
static HRESULT fwd_tex(DWORD stage, IDirect3DBaseTexture8 *t) {
  g_fwd++;
  HRESULT hr = PREV(PFN_DP, S_SetTexture)(g_dev, stage, t);
  if (stage < 8 && !g_recording) { g_tex[stage] = t; g_texV[stage] = SUCCEEDED(hr) && cache_on(); }
  else if (g_recording) { if (stage < 8) rec_add(PK_TEX + (int)stage, 0); else g_recOk = false; }
  return hr;
}
// every held change goes out, in the order first held (the batch, drawn under the state before them, goes first)
static void apply_pending(void) {
  if (!g_live) { g_npl = 0; return; }
  int n = g_npl; g_npl = 0;
  for (int i = 0; i < n; i++) {
    int k = g_pl[i];
    if (k < 0 || !g_isP[k]) continue;
    g_isP[k] = false; g_live--; g_sent++;
    if (k < PK_TSS) fwd_rs((DWORD)k, g_pv[k]);
    else if (k < PK_TEX) fwd_tss((DWORD)(k - PK_TSS) >> 5, (DWORD)(k - PK_TSS) & 31, g_pv[k]);
    else { IDirect3DBaseTexture8 *t = (IDirect3DBaseTexture8 *)(uintptr_t)g_pv[k]; fwd_tex((DWORD)(k - PK_TEX), t); if (t) t->Release(); }
  }
}
static inline void commit(void) { flush(); apply_pending(); }

// merge_scenes: the EndScene the game makes inside a frame is held; the BeginScene after it is then dropped (the scene
// is still open on the device), so the frame is one scene. A held EndScene goes out before Present and Reset, and
// before the methods that read or copy whole surfaces (CopyRects, UpdateTexture, GetFrontBuffer: stub_enter), which
// then run outside a scene as the game had them. Render thread only; hooks live only (the kit's A phases and F9 leave
// the game's scenes as they are).
static void close_held(void) {
  if (!g_sceneHeld) return;
  g_sceneHeld = false;
  PREV(PFN_Dev, S_EndScene)(g_heldDev);
}
static HRESULT __stdcall h_Reset(void *d, D3DPRESENT_PARAMETERS *pp) {
  if (g_grp) grp_order();
  close_held();
  flush(); drop_pending(); forget_all();
  sf_occl_reset();                              // occlusion_late's surfaces (default-pool ones must go before Reset)
  HRESULT hr = PREV(PFN_Reset, S_Reset)(d, pp);
  forget_all(); g_curVS = 1; g_recording = 0; g_recOk = false; blk_drop_all();   // Reset returns the device to its default state
  return hr;
}
static HRESULT __stdcall h_Present(void *d, const RECT *a, const RECT *b, HWND w, const RGNDATA *r) {
  if (g_grp) grp_order();
  if (!g_render_tid) { g_render_tid = GetCurrentThreadId(); H->log("speedfix: render thread %lu", (unsigned long)g_render_tid); }
  else if (!render_thread()) foreign();
  commit();
  close_held();
  return PREV(PFN_Present, S_Present)(d, a, b, w, r);
}
// Scene boundaries. State set by something that calls the device below this layer cannot be seen here; forgetting
// the shadow at each scene bounds such a difference to one scene (speedfix_hook's state_cache_reset_each_scene).
// Frames that passed without these hooks (the kit turns mod hooks off for F9 and a trace's unswapped frames) set
// state the shadow never saw: then it is forgotten whatever the switch. Held changes go out at each boundary.
static HRESULT __stdcall h_BeginScene(void *d) {
  if (g_grp) grp_order();
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_Dev, S_BeginScene)(d); }
  commit();
  if (g_last_scene_frame >= 0 && g_frame - g_last_scene_frame > 1) { drop_pending(); forget_all(); g_forgot_gap++; }
  g_last_scene_frame = g_frame;
  sf_hooks_frame = g_frame;
  if (g_sceneHeld) {                               // the scene the game ended is still open: this begin is dropped
    g_sceneHeld = false;
    if (sf_cfg.scene_reset) forget_all();
    return D3D_OK;
  }
  HRESULT hr = PREV(PFN_Dev, S_BeginScene)(d);
  if (sf_cfg.scene_reset) forget_all();
  return hr;
}
static HRESULT __stdcall h_EndScene(void *d) {
  if (g_grp) grp_order();
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_Dev, S_EndScene)(d); }
  commit();
  if (sf_cfg.merge_scenes && sf_hooks_live() && !g_sceneHeld) {
    g_sceneHeld = true; g_heldDev = d; g_mergedEnds++;
    if (sf_cfg.scene_reset) forget_all();
    return D3D_OK;
  }
  HRESULT hr = PREV(PFN_Dev, S_EndScene)(d);
  if (sf_cfg.scene_reset) forget_all();
  return hr;
}
// a state block records, applies or captures state: what is held goes out first, in its place in the order
static HRESULT __stdcall h_BeginSB(void *d) {
  if (g_grp) grp_order();
  bool mine = render_thread();
  if (!mine && g_render_tid) foreign(); else commit();
  HRESULT hr = PREV(PFN_Dev, S_BeginSB)(d);
  if (SUCCEEDED(hr)) { g_recording = 1; g_rec.n = 0; g_rec.other = false; g_recOk = mine; }
  return hr;
}
static HRESULT __stdcall h_EndSB(void *d, DWORD *tok) {
  if (g_grp) grp_order();
  bool mine = render_thread();
  if (!mine && g_render_tid) foreign(); else commit();
  bool follow = g_recOk && mine; g_recording = 0; g_recOk = false;
  HRESULT hr = PREV(PFN_DPtr, S_EndSB)(d, tok);
  if (SUCCEEDED(hr) && tok) { if (follow) blk_keep(*tok); else blk_drop(*tok); }
  return hr;
}
static HRESULT __stdcall h_ApplySB(void *d, DWORD tok) {
  if (g_grp) grp_order();
  bool mine = render_thread();
  if (!mine && g_render_tid) foreign(); else commit();
  HRESULT hr = PREV(PFN_D1, S_ApplySB)(d, tok);
  Blk *b = mine && SUCCEEDED(hr) && sf_cfg.block_cache && !g_recording ? blk_find(tok) : NULL;
  if (!b) { forget_all(); g_curVS = 1; g_blkForgot++; return hr; }   // a block may set any state, the shader too
  if (b->other) {
    g_vsV = false; g_curVS = 1; memset(g_xfV, 0, sizeof g_xfV); g_vpV = false; memset(g_lightV, 0, sizeof g_lightV);
    memset(g_leV, 0, sizeof g_leV); memset(g_ssV, 0, sizeof g_ssV); g_ibV = false;
  }
  bool c = cache_on();
  for (int i = 0; i < b->n; i++) {             // in recorded order: a state set twice ends at its last value
    int k = b->key[i]; DWORD v = b->val[i];
    if (k < PK_TSS) {
      g_rs[k] = v; g_rsV[k] = c;
      if (k == D3DRS_FILLMODE) { g_fill = v; g_fillV = true; }
      if (k == D3DRS_SHADEMODE) { g_shade = v; g_shadeV = true; }
    } else if (k < PK_TEX) { g_tss[(k - PK_TSS) >> 5][(k - PK_TSS) & 31] = v; g_tssV[(k - PK_TSS) >> 5][(k - PK_TSS) & 31] = c; }
    else g_texV[k - PK_TEX] = false;
  }
  g_blkKnown++;
  return hr;
}
typedef HRESULT (__stdcall *PFN_CreateSB)(void *, DWORD, DWORD *);
// these change no device state; what is held goes out first, as the flushing stub in their slots did
static HRESULT __stdcall h_CaptureSB(void *d, DWORD tok) {
  if (g_grp) grp_order();
  if (!render_thread() && g_render_tid) foreign(); else commit();
  blk_drop(tok); return PREV(PFN_D1, S_CaptureSB)(d, tok);
}
static HRESULT __stdcall h_DeleteSB(void *d, DWORD tok) {
  if (g_grp) grp_order();
  if (!render_thread() && g_render_tid) foreign(); else commit();
  blk_drop(tok); return PREV(PFN_D1, S_DeleteSB)(d, tok);
}
static HRESULT __stdcall h_CreateSB(void *d, DWORD ty, DWORD *tok) {
  if (g_grp) grp_order();
  if (!render_thread() && g_render_tid) foreign(); else commit();
  HRESULT hr = PREV(PFN_CreateSB, S_CreateSB)(d, ty, tok);
  if (SUCCEEDED(hr) && tok) blk_drop(*tok);
  return hr;
}

// The cached setters. lazy_state: a value the device holds undoes a held change (or is skipped); another is held.
static HRESULT __stdcall h_SetRenderState(void *d, D3DRENDERSTATETYPE st, DWORD v) {
  if (g_grp && !g_recording && grp_hold(G_RS, (DWORD)st, v, 0, 0, 0, NULL, 0)) return D3D_OK;
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_D2, S_SetRenderState)(d, st, v); }
  if ((unsigned)st < 256 && lazy_on()) {
    int k = (int)st;
    if (g_rsV[st] && g_rs[st] == v) { if (g_isP[k]) unpend(k); else g_skip++; return D3D_OK; }
    if (g_isP[k]) { g_pv[k] = v; g_dropped++; } else pend(k, v);
    return D3D_OK;
  }
  bool use = sf_cfg.state_cache && !g_recording && (unsigned)st < 256;
  if (use && g_rsV[st] && g_rs[st] == v) { g_skip++; return D3D_OK; }
  if (!(g_upPrims && g_upRhw && tnl_only(st))) flush();      // a real change: what is pending goes out first
  return fwd_rs(st, v);
}
static void mot_texture(const void *t);
static HRESULT __stdcall h_SetTexture(void *d, DWORD stage, IDirect3DBaseTexture8 *t) {
  if (g_grp && !g_recording && grp_hold(G_TEX, stage, (DWORD)(uintptr_t)t, 0, 0, 0, NULL, 0)) return D3D_OK;
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_DP, S_SetTexture)(d, stage, t); }
  if (g_motion && t) mot_texture(t);
  if (stage < 8 && lazy_on()) {
    int k = PK_TEX + (int)stage;
    if (g_texV[stage] && g_tex[stage] == t) { if (g_isP[k]) unpend(k); else g_skip++; return D3D_OK; }
    if (sf_cfg.batch_up) note_texture(t);
    if (t) t->AddRef();                          // the game may release it before it is sent
    if (g_isP[k]) { IDirect3DBaseTexture8 *o = (IDirect3DBaseTexture8 *)(uintptr_t)g_pv[k]; g_pv[k] = (DWORD)(uintptr_t)t; if (o) o->Release(); g_dropped++; }
    else pend(k, (DWORD)(uintptr_t)t);
    return D3D_OK;
  }
  bool use = sf_cfg.state_cache && !g_recording && stage < 8;
  if (use && g_texV[stage] && g_tex[stage] == t) { g_skip++; return D3D_OK; }
  flush();
  if (sf_cfg.batch_up) note_texture(t);
  return fwd_tex(stage, t);
}
static HRESULT __stdcall h_SetTSS(void *d, DWORD stage, D3DTEXTURESTAGESTATETYPE ty, DWORD v) {
  if (g_grp && !g_recording && grp_hold(G_TSS, stage, (DWORD)ty, v, 0, 0, NULL, 0)) return D3D_OK;
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_D3, S_SetTSS)(d, stage, ty, v); }
  if (stage < 8 && (unsigned)ty < 32 && lazy_on()) {
    int k = PK_TSS + (int)stage * 32 + (int)ty;
    if (g_tssV[stage][ty] && g_tss[stage][ty] == v) { if (g_isP[k]) unpend(k); else g_skip++; return D3D_OK; }
    if (g_isP[k]) { g_pv[k] = v; g_dropped++; } else pend(k, v);
    return D3D_OK;
  }
  bool use = sf_cfg.state_cache && !g_recording && stage < 8 && (unsigned)ty < 32;
  if (use && g_tssV[stage][ty] && g_tss[stage][ty] == v) { g_skip++; return D3D_OK; }
  flush();
  return fwd_tss(stage, ty, v);
}
static HRESULT __stdcall h_SetVS(void *d, DWORD h) {
  if (g_grp && !g_recording && grp_hold(G_VS, h, 0, 0, 0, 0, NULL, 0)) return D3D_OK;
  if (!render_thread()) { if (g_render_tid) foreign(); HRESULT hr = PREV(PFN_D1, S_SetVS)(d, h); g_curVS = 1; return hr; }
  bool use = sf_cfg.vs_cache && !g_recording;
  if (use && g_vsV && g_vs == h) { g_skip++; return D3D_OK; }
  flush();
  g_fwd++;
  HRESULT hr = PREV(PFN_D1, S_SetVS)(d, h);
  if (!g_recording) { if (SUCCEEDED(hr)) g_curVS = h; g_vs = h; g_vsV = SUCCEEDED(hr) && sf_cfg.vs_cache; } else rec_other();
  return hr;
}

// ---- vs_dedupe ---------------------------------------------------------------------------------------------------
// A declaration runs to D3DVSD_END(); D3DVSD_CONST and extension tokens carry their data after them (d3d8types.h),
// which may hold 0xFFFFFFFF, so the tokens are walked, not searched.
enum { VSD_MAX = 64, VSD_TOK = 128 };
struct VsShared { DWORD h, usage, refs; int n; DWORD decl[VSD_TOK]; };
static VsShared g_vsd[VSD_MAX]; static int g_nvsd; static CRITICAL_SECTION g_vsdLock;
static volatile LONG g_vsdMade, g_vsdShared, g_vsdKept;
static int decl_tokens(const DWORD *d) {
  for (int i = 0; i < VSD_TOK;) {
    DWORD t = d[i];
    if (t == D3DVSD_END()) return i + 1;
    DWORD ty = (t & D3DVSD_TOKENTYPEMASK) >> D3DVSD_TOKENTYPESHIFT;
    if (ty == D3DVSD_TOKEN_CONSTMEM) i += 1 + 4 * (int)((t & D3DVSD_CONSTCOUNTMASK) >> D3DVSD_CONSTCOUNTSHIFT);
    else if (ty == D3DVSD_TOKEN_EXT) i += 1 + (int)((t & D3DVSD_EXTCOUNTMASK) >> D3DVSD_EXTCOUNTSHIFT);
    else i++;
  }
  return -1;                                     // longer than kept here: made as the game asked
}
typedef HRESULT (__stdcall *PFN_CreateVS)(void *, const DWORD *, const DWORD *, DWORD *, DWORD);
// creation sets no state: no flush, and a call from another thread leaves the shadow alone
static HRESULT __stdcall h_CreateVS(void *d, const DWORD *decl, const DWORD *fn, DWORD *ph, DWORD usage) {
  if (g_grp) grp_order();
  if (g_motion && render_thread()) mot_count(75);
  int n = sf_cfg.vs_dedupe && decl && !fn && ph ? decl_tokens(decl) : -1;
  if (n > 0) {
    EnterCriticalSection(&g_vsdLock);
    for (int i = 0; i < g_nvsd; i++)
      if (g_vsd[i].usage == usage && g_vsd[i].n == n && !memcmp(g_vsd[i].decl, decl, n * sizeof(DWORD))) {
        g_vsd[i].refs++; *ph = g_vsd[i].h; LeaveCriticalSection(&g_vsdLock); g_vsdShared++; return D3D_OK;
      }
    LeaveCriticalSection(&g_vsdLock);
  }
  HRESULT hr = PREV(PFN_CreateVS, 75)(d, decl, fn, ph, usage);
  if (n > 0 && SUCCEEDED(hr)) {
    EnterCriticalSection(&g_vsdLock);
    if (g_nvsd < VSD_MAX) { VsShared &e = g_vsd[g_nvsd++]; e.h = *ph; e.usage = usage; e.refs = 1; e.n = n; memcpy(e.decl, decl, n * sizeof(DWORD)); }
    LeaveCriticalSection(&g_vsdLock);
    g_vsdMade++;
  }
  return hr;
}
// a shared handle is deleted when the last of its creations is (whether vs_dedupe is on now or not)
static bool vsd_keep(DWORD h) {
  bool keep = false;
  EnterCriticalSection(&g_vsdLock);
  for (int i = 0; i < g_nvsd; i++)
    if (g_vsd[i].h == h) {
      if (--g_vsd[i].refs > 0) keep = true;
      else g_vsd[i] = g_vsd[--g_nvsd];
      break;
    }
  LeaveCriticalSection(&g_vsdLock);
  return keep;
}

// a deleted shader's handle can be given out again by the next CreateVertexShader
static HRESULT __stdcall h_DeleteVS(void *d, DWORD h) {
  if (g_grp) grp_order();
  if (vsd_keep(h)) { g_vsdKept++; return D3D_OK; }     // others still use it: the shadow stays true
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
static void mot_view(const D3DMATRIX *m);
static HRESULT __stdcall h_SetTransform(void *d, D3DTRANSFORMSTATETYPE st, const D3DMATRIX *m) {
  if (g_grp && !g_recording && m && grp_hold(G_XF, (DWORD)st, 0, 0, 0, 0, m, sizeof *m)) return D3D_OK;
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_DP, S_SetTransform)(d, st, m); }
  if (g_motion && st == D3DTS_VIEW && m) mot_view(m);
  if (!g_recording && (unsigned)st < 512 && m) {
    if (g_xfV[st] && !memcmp(&g_xf[st], m, sizeof *m) && sf_cfg.ext_cache) { g_skip++; return D3D_OK; }
  }
  // view, projection and world matrices do not touch pre-transformed vertices; texture transforms do
  bool geom = st == D3DTS_VIEW || st == D3DTS_PROJECTION || ((unsigned)st >= 256 && (unsigned)st < 512);
  if (!(g_upPrims && g_upRhw && geom)) flush();
  g_fwd++;
  HRESULT hr = PREV(PFN_DP, S_SetTransform)(d, st, m);
  if (!g_recording && (unsigned)st < 512) { if (m && SUCCEEDED(hr)) { g_xf[st] = *m; g_xfV[st] = true; } else g_xfV[st] = false; }
  rec_other();
  return hr;
}
static HRESULT __stdcall h_SetViewport(void *d, const D3DVIEWPORT8 *v) {
  if (g_grp && !g_recording && v && grp_hold(G_VP, 0, 0, 0, 0, 0, v, sizeof *v)) return D3D_OK;
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_P, S_SetViewport)(d, v); }
  if (!g_recording && v && g_vpV && !memcmp(&g_vp, v, sizeof *v) && sf_cfg.ext_cache) { g_skip++; return D3D_OK; }
  flush(); g_fwd++;
  HRESULT hr = PREV(PFN_P, S_SetViewport)(d, v);
  if (!g_recording) { if (v && SUCCEEDED(hr)) { g_vp = *v; g_vpV = true; } else g_vpV = false; }
  rec_other();
  return hr;
}
static HRESULT __stdcall h_SetMaterial(void *d, const D3DMATERIAL8 *m) {
  if (g_grp && !g_recording && m && grp_hold(G_MAT, 0, 0, 0, 0, 0, m, sizeof *m)) return D3D_OK;
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_P, S_SetMaterial)(d, m); }
  if (!(g_upPrims && g_upRhw)) flush();                   // materials feed lighting only
  return PREV(PFN_P, S_SetMaterial)(d, m);
}
static HRESULT __stdcall h_SetLight(void *d, DWORD i, const D3DLIGHT8 *l) {
  if (g_grp && !g_recording && l && grp_hold(G_LIGHT, i, 0, 0, 0, 0, l, sizeof *l)) return D3D_OK;
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_DP, S_SetLight)(d, i, l); }
  if (!g_recording && i < 32 && l && g_lightV[i] && !memcmp(&g_light[i], l, sizeof *l) && sf_cfg.ext_cache) { g_skip++; return D3D_OK; }
  if (!(g_upPrims && g_upRhw)) flush();                   // lights never reach pre-transformed vertices
  g_fwd++;
  HRESULT hr = PREV(PFN_DP, S_SetLight)(d, i, l);
  if (!g_recording && i < 32) { if (l && SUCCEEDED(hr)) { g_light[i] = *l; g_lightV[i] = true; } else g_lightV[i] = false; }
  rec_other();
  return hr;
}
static HRESULT __stdcall h_LightEnable(void *d, DWORD i, BOOL on) {
  if (g_grp && !g_recording && grp_hold(G_LEN, i, (DWORD)on, 0, 0, 0, NULL, 0)) return D3D_OK;
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_D2, S_LightEnable)(d, i, on); }
  if (!g_recording && i < 32 && g_leV[i] && (g_le[i] != 0) == (on != 0) && sf_cfg.ext_cache) { g_skip++; return D3D_OK; }
  if (!(g_upPrims && g_upRhw)) flush();
  g_fwd++;
  HRESULT hr = PREV(PFN_D2, S_LightEnable)(d, i, on);
  if (!g_recording && i < 32) { g_le[i] = on; g_leV[i] = SUCCEEDED(hr); }
  rec_other();
  return hr;
}
// re-binding the buffer the device already holds a reference to
static HRESULT __stdcall h_SetStream(void *d, UINT n, IDirect3DVertexBuffer8 *vb, UINT stride) {
  if (g_grp && !g_recording && grp_hold(G_SS, n, (DWORD)(uintptr_t)vb, stride, 0, 0, NULL, 0)) return D3D_OK;
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_SS, S_SetStream)(d, n, vb, stride); }
  if (!g_recording && n < 16 && g_ssV[n] && g_ssVb[n] == vb && g_ssStride[n] == stride && sf_cfg.ext_cache) { g_skip++; return D3D_OK; }
  flush(); g_fwd++;
  HRESULT hr = PREV(PFN_SS, S_SetStream)(d, n, vb, stride);
  if (!g_recording && n < 16) { g_ssVb[n] = vb; g_ssStride[n] = stride; g_ssV[n] = SUCCEEDED(hr); }
  rec_other();
  return hr;
}
static HRESULT __stdcall h_SetIndices(void *d, IDirect3DIndexBuffer8 *ib, UINT base) {
  if (g_grp && !g_recording && grp_hold(G_SI, (DWORD)(uintptr_t)ib, base, 0, 0, 0, NULL, 0)) return D3D_OK;
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_SI, S_SetIndices)(d, ib, base); }
  if (!g_recording && g_ibV && g_ib == ib && g_ibBase == base && sf_cfg.ext_cache) { g_skip++; return D3D_OK; }
  flush(); g_fwd++;
  HRESULT hr = PREV(PFN_SI, S_SetIndices)(d, ib, base);
  if (!g_recording) { g_ib = ib; g_ibBase = base; g_ibV = SUCCEEDED(hr); }
  rec_other();
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
  if (g_grp) grp_order();
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_DPUP, S_DPUP)(d, t, prims, data, stride); }
  g_upIn++;
  if (g_motion) mot_count(S_DPUP);
  if (g_live) commit();                            // held changes: the batch so far goes out under the state before them
  else g_npl = 0;
  g_ssV[0] = false;                                // DrawPrimitiveUP leaves stream 0 unset (D3D8)
  DWORD maxP = sf_cfg.batch_max_prims > 0 && (DWORD)sf_cfg.batch_max_prims < g_maxPrims ? (DWORD)sf_cfg.batch_max_prims : g_maxPrims;
  int maxB = sf_cfg.batch_max_bytes > 0 && sf_cfg.batch_max_bytes < UPBUF ? sf_cfg.batch_max_bytes : UPBUF;
  int outBytes = (int)prims * 3 * (int)stride;
  bool ok = sf_cfg.batch_up && !g_recording && data && stride && stride <= 256 && prims && prims <= maxP &&
            verts_for(t, prims) > 0 && outBytes <= maxB && (t == D3DPT_TRIANGLELIST || may_convert());
  if (sf_cfg.null_draws) { flush(); g_nullDrawn++; return D3D_OK; }
  if (!ok) { flush(); g_upOut++; return PREV(PFN_DPUP, S_DPUP)(d, t, prims, data, stride); }
  if (g_upPrims && (stride != g_upStride || g_upBytes + outBytes > maxB || (DWORD)g_upPrims + prims > maxP)) flush();
  if (t != D3DPT_TRIANGLELIST) g_upConv++;
  expand(t, prims, (const BYTE *)data, stride, g_up + g_upBytes);  // the caller's data is only valid during the call
  if (!g_upPrims) g_upRhw = fvf_rhw(g_curVS);
  g_upBytes += outBytes; g_upPrims += (int)prims; g_upStride = stride;
  return D3D_OK;
}

// ---- null_draws (a test, not a speedup): the draw calls return D3D_OK without reaching the device -----------------
// Everything else the game sends still goes through: held changes go out at the draw as they would (lazy_state),
// so the device sees the same state calls, locks, Clear and Present. Signatures: IDirect3DDevice8 in the DirectX 8
// headers (d3d8.h). What the game reads back from the frame (the occlusion readback) is wrong under this lever; the
// session that uses it skips those readbacks in both arms (kit/sessions/ceiling).
typedef HRESULT (__stdcall *PFN_DIPUP)(void *, D3DPRIMITIVETYPE, UINT, UINT, UINT, const void *, D3DFORMAT, const void *, UINT);
static inline void draw_enter(int id) {          // what stub_enter does for a draw slot
  flush();
  if (g_npl) apply_pending();
  if (g_motion) mot_count(id);
}
static HRESULT __stdcall h_DP(void *d, D3DPRIMITIVETYPE t, UINT start, UINT prims) {
  if (g_grp) grp_order();
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_DPr4, S_DP)(d, t, start, prims); }
  draw_enter(S_DP);
  if (sf_cfg.null_draws) { g_nullDrawn++; return D3D_OK; }
  return PREV(PFN_DPr4, S_DP)(d, t, start, prims);
}
static HRESULT __stdcall h_DIP(void *d, D3DPRIMITIVETYPE t, UINT minIdx, UINT nv, UINT startIdx, UINT prims) {
  if (g_grp && !g_recording && grp_hold(G_DIP, (DWORD)t, minIdx, nv, startIdx, prims, NULL, 0)) return D3D_OK;
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_DIP6, S_DIP)(d, t, minIdx, nv, startIdx, prims); }
  draw_enter(S_DIP);
  if (sf_cfg.null_draws) { g_nullDrawn++; return D3D_OK; }
  return PREV(PFN_DIP6, S_DIP)(d, t, minIdx, nv, startIdx, prims);
}
static HRESULT __stdcall h_DIPUP(void *d, D3DPRIMITIVETYPE t, UINT minIdx, UINT nv, UINT prims, const void *idx, D3DFORMAT fmt,
                                 const void *vtx, UINT stride) {
  if (g_grp) grp_order();
  if (!render_thread()) { if (g_render_tid) foreign(); return PREV(PFN_DIPUP, S_DIPUP)(d, t, minIdx, nv, prims, idx, fmt, vtx, stride); }
  draw_enter(S_DIPUP);
  g_ssV[0] = false; g_ibV = false;               // DrawIndexedPrimitiveUP leaves stream 0 and the indices unset
  if (sf_cfg.null_draws) { g_nullDrawn++; return D3D_OK; }
  return PREV(PFN_DIPUP, S_DIPUP)(d, t, minIdx, nv, prims, idx, fmt, vtx, stride);
}

// ---- skin_merge=2: grouping a model's draws (docs/SKINMERGE.md; HISTORY.md 6 Oct, "Reordering within a model") ---
// Inside one CYyModelDt::SkinAndDrawV2 (sf_group_begin / sf_group_end, from speedfix's hook of it: sf_fork.cpp) the
// device calls above are held here. At the end each triangle-list DrawIndexedPrimitive drawn without alpha blending is
// put with the others drawn under the same state (every state the call set, as it stood at that draw), and each such
// group goes out once: its state, then one DrawIndexedPrimitive of an index list made of its draws' index ranges, built
// once from the model's index buffer and kept. A draw with blending, any other primitive or any state not known keeps
// its place: it closes the groups before it (they go out first, in the order of their first draws), then goes out as
// the game made it. Groups go out in the order of their first draws, so a state first set after some draws is set
// after the groups drawn before it (their draws had the value from before). Any other device call, or more held than
// fits, sends everything held in the game's order first. What is changed against the game's own calls is the order of
// opaque triangle-list draws inside one model (the picture differs only where two of them meet at the same depth) and
// the index buffer they are drawn from; the state the device holds afterwards is the game's.
enum { GOPS = 2048, GKEYS = 160, GDRAWS = 512, GBLOB = 104, GCACHE = 4096 };
struct GOp { BYTE ty; DWORD a, b, c, d, e; BYTE blob[GBLOB]; BYTE nb; };
static GOp g_go[GOPS]; static int g_ngo;
static DWORD g_gkey[GKEYS]; static int g_ngkey;                      // the state keys the held calls set
static short g_gcur[GKEYS];                                          // per key: the held call that set it last (-1 none)
struct GDraw { int op; int snap; bool reord; };
static GDraw g_gd[GDRAWS]; static int g_ngd;
static short g_gsnap[GDRAWS * GKEYS];                                // each draw's g_gcur as it was at that draw
static volatile LONG g_gModels, g_gIn, g_gOut, g_gBarrier, g_gOrder, g_gBuilt, g_gHit, g_gNoCache;
static inline DWORD gkey(const GOp &o) {
  switch (o.ty) {
  case G_RS: return (G_RS << 16) | (o.a & 0xFFFF); case G_TSS: return (G_TSS << 16) | ((o.a & 7) << 5) | (o.b & 31);
  case G_TEX: case G_LIGHT: case G_LEN: case G_SS: case G_XF: return ((DWORD)o.ty << 16) | (o.a & 0xFFFF);
  default: return (DWORD)o.ty << 16;
  }
}
static int gslot(DWORD k) {
  for (int i = 0; i < g_ngkey; i++) if (g_gkey[i] == k) return i;
  if (g_ngkey == GKEYS) return -1;
  g_gkey[g_ngkey] = k; g_gcur[g_ngkey] = -1; return g_ngkey++;
}
static bool gsame(int i, int j) {             // two held calls set their key to the same value
  if (i == j) return true;
  if (i < 0 || j < 0) return false;
  const GOp &x = g_go[i], &y = g_go[j];
  return x.a == y.a && x.b == y.b && x.c == y.c && x.nb == y.nb && !memcmp(x.blob, y.blob, x.nb);
}
static void greplay(int i) {                  // one held call, through the hooks (g_grp is off)
  const GOp &o = g_go[i];
  switch (o.ty) {
  case G_RS: h_SetRenderState(g_dev, (D3DRENDERSTATETYPE)o.a, o.b); break;
  case G_TSS: h_SetTSS(g_dev, o.a, (D3DTEXTURESTAGESTATETYPE)o.b, o.c); break;
  case G_TEX: h_SetTexture(g_dev, o.a, (IDirect3DBaseTexture8 *)(uintptr_t)o.b); break;
  case G_VS: h_SetVS(g_dev, o.a); break;
  case G_XF: h_SetTransform(g_dev, (D3DTRANSFORMSTATETYPE)o.a, (const D3DMATRIX *)o.blob); break;
  case G_MAT: h_SetMaterial(g_dev, (const D3DMATERIAL8 *)o.blob); break;
  case G_LIGHT: h_SetLight(g_dev, o.a, (const D3DLIGHT8 *)o.blob); break;
  case G_LEN: h_LightEnable(g_dev, o.a, (BOOL)o.b); break;
  case G_SS: h_SetStream(g_dev, o.a, (IDirect3DVertexBuffer8 *)(uintptr_t)o.b, o.c); break;
  case G_SI: h_SetIndices(g_dev, (IDirect3DIndexBuffer8 *)(uintptr_t)o.a, o.b); break;
  case G_VP: h_SetViewport(g_dev, (const D3DVIEWPORT8 *)o.blob); break;
  case G_DIP: h_DIP(g_dev, (D3DPRIMITIVETYPE)o.a, o.b, o.c, o.d, o.e); break;
  }
}
static void grp_reset(void) { g_ngo = 0; g_ngkey = 0; g_ngd = 0; }
static void grp_order(void) {
  if (!g_ngo) return;
  bool was = g_grp; g_grp = false;
  for (int i = 0; i < g_ngo; i++) greplay(i);
  g_gOrder++; grp_reset(); g_grp = was;
}
static bool grp_hold(int ty, DWORD a, DWORD b, DWORD c, DWORD d, DWORD e, const void *blob, int nb) {
  if (g_ngo == GOPS || nb > GBLOB || (ty == G_DIP && g_ngd == GDRAWS)) grp_order();
  GOp &o = g_go[g_ngo]; o.ty = (BYTE)ty; o.a = a; o.b = b; o.c = c; o.d = d; o.e = e; o.nb = (BYTE)nb;
  if (nb) memcpy(o.blob, blob, nb);
  if (ty != G_DIP) {
    int k = gslot(gkey(o));
    if (k < 0) { grp_order(); return false; }  // more keys than fit: this call goes out after what was held
    g_gcur[k] = (short)g_ngo++;
    return true;
  }
  // a draw: reorderable when a triangle list, from an index buffer set here, with ALPHABLENDENABLE known to be 0
  int si = -1, ab = -1;
  for (int k = 0; k < g_ngkey; k++) { if (g_gkey[k] == ((DWORD)G_SI << 16)) si = g_gcur[k]; if (g_gkey[k] == ((DWORD)G_RS << 16 | D3DRS_ALPHABLENDENABLE)) ab = g_gcur[k]; }
  bool blendOff = ab >= 0 ? g_go[ab].b == 0 : (g_rsV[D3DRS_ALPHABLENDENABLE] && g_rs[D3DRS_ALPHABLENDENABLE] == 0 && !g_isP[D3DRS_ALPHABLENDENABLE]);
  GDraw &dr = g_gd[g_ngd];
  dr.op = g_ngo; dr.reord = a == D3DPT_TRIANGLELIST && si >= 0 && g_go[si].a != 0 && blendOff;
  dr.snap = g_ngd * GKEYS; memcpy(&g_gsnap[dr.snap], g_gcur, sizeof(short) * g_ngkey);
  for (int k = g_ngkey; k < GKEYS; k++) g_gsnap[dr.snap + k] = -1;
  g_ngd++; g_ngo++; g_gIn++;
  return true;
}

// the joined index lists, kept: by index buffer and the ranges joined
struct GIb { IDirect3DIndexBuffer8 *src, *ib; DWORD hash; int n; DWORD *rng; UINT minI, nvI, prims; int frame; bool nocache; };
static GIb g_gib[GCACHE]; static int g_ngib;
typedef HRESULT (__stdcall *PFN_CreateIB)(void *, UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DIndexBuffer8 **);
static GIb *gib_get(IDirect3DIndexBuffer8 *src, const DWORD *rng, int n) {   // rng: start, prims, min, nv per draw
  DWORD h = 2166136261u ^ (DWORD)(uintptr_t)src;
  for (int i = 0; i < 4 * n; i++) h = (h ^ rng[i]) * 16777619u;
  for (int i = 0; i < g_ngib; i++) {
    GIb &c = g_gib[i];
    if (c.hash == h && c.src == src && c.n == n && !memcmp(c.rng, rng, 16 * n)) { c.frame = g_frame; g_gHit++; return &c; }
  }
  int slot = g_ngib;
  if (g_ngib == GCACHE) {                     // full: the one used longest ago goes
    slot = 0; for (int i = 1; i < GCACHE; i++) if (g_gib[i].frame < g_gib[slot].frame) slot = i;
    GIb &o = g_gib[slot]; if (o.ib) o.ib->Release(); if (o.src) o.src->Release(); free(o.rng); memset(&o, 0, sizeof o);
  } else g_ngib++;
  GIb &c = g_gib[slot]; memset(&c, 0, sizeof c);
  c.src = src; src->AddRef(); c.hash = h; c.n = n; c.frame = g_frame;
  c.rng = (DWORD *)malloc(16 * n); memcpy(c.rng, rng, 16 * n);
  UINT lo = 0xFFFFFFFFu, hi = 0, prims = 0;
  for (int i = 0; i < n; i++) { prims += rng[4 * i + 1]; if (rng[4 * i + 2] < lo) lo = rng[4 * i + 2]; if (rng[4 * i + 2] + rng[4 * i + 3] > hi) hi = rng[4 * i + 2] + rng[4 * i + 3]; }
  c.minI = lo; c.nvI = hi - lo; c.prims = prims;
  D3DINDEXBUFFER_DESC desc;
  // a buffer the game may rewrite (dynamic) or one that cannot be read back (default pool) is not joined
  if (prims > g_maxPrims || FAILED(src->GetDesc(&desc)) || (desc.Usage & D3DUSAGE_DYNAMIC) || desc.Pool == D3DPOOL_DEFAULT || !g_prev[24]) { c.nocache = true; g_gNoCache++; return &c; }
  UINT isz = desc.Format == D3DFMT_INDEX32 ? 4 : 2;
  BYTE *sp = NULL, *dp = NULL; IDirect3DIndexBuffer8 *ib = NULL;
  if (FAILED(src->Lock(0, 0, &sp, D3DLOCK_READONLY))) { c.nocache = true; g_gNoCache++; return &c; }
  bool ok = SUCCEEDED(PREV(PFN_CreateIB, 24)(g_dev, prims * 3 * isz, D3DUSAGE_WRITEONLY, desc.Format, D3DPOOL_MANAGED, &ib)) && ib
         && SUCCEEDED(ib->Lock(0, 0, &dp, 0));
  if (ok) {
    UINT at = 0;
    for (int i = 0; i < n; i++) {
      UINT st = rng[4 * i], cnt = rng[4 * i + 1] * 3;
      if ((st + cnt) * isz > desc.Size) { ok = false; break; }
      memcpy(dp + at * isz, sp + st * isz, cnt * isz); at += cnt;
    }
    ib->Unlock();
  }
  src->Unlock();
  if (!ok) { if (ib) ib->Release(); c.nocache = true; g_gNoCache++; return &c; }
  c.ib = ib; g_gBuilt++;
  return &c;
}

static short g_gem[GKEYS];                    // per key: the held call whose value the device holds now (-1: the value before)
static bool g_gemOurs;                        // the device's indices are a joined list's
static void gem_to(const short *snap) {       // the device to the state of a draw's snapshot
  for (int k = 0; k < g_ngkey; k++) {
    int want = snap[k];
    if (want < 0) continue;                    // not set before this draw: the device still has the value before
    bool si = g_gkey[k] == ((DWORD)G_SI << 16);
    if ((si && g_gemOurs) || !gsame(g_gem[k], want)) { greplay(want); g_gem[k] = (short)want; if (si) g_gemOurs = false; }
  }
}
static void grp_emit_group(const int *ds, int n) {   // draws (indices into g_gd) of one group
  const short *snap = &g_gsnap[g_gd[ds[0]].snap];
  gem_to(snap);
  if (n > 1) {
    int sik = -1; for (int k = 0; k < g_ngkey; k++) if (g_gkey[k] == ((DWORD)G_SI << 16)) sik = k;
    const GOp &si = g_go[snap[sik]];
    static DWORD rng[4 * GDRAWS];
    for (int i = 0; i < n; i++) { const GOp &o = g_go[g_gd[ds[i]].op]; rng[4 * i] = o.d; rng[4 * i + 1] = o.e; rng[4 * i + 2] = o.b; rng[4 * i + 3] = o.c; }
    GIb *c = gib_get((IDirect3DIndexBuffer8 *)(uintptr_t)si.a, rng, n);
    if (c && c->ib) {
      h_SetIndices(g_dev, c->ib, si.b); g_gemOurs = true;
      h_DIP(g_dev, D3DPT_TRIANGLELIST, c->minI, c->nvI, 0, c->prims); g_gOut++;
      return;
    }
  }
  for (int i = 0; i < n; i++) { greplay(g_gd[ds[i]].op); g_gOut++; }
}
static void grp_emit_segment(int from, int to) {      // reorderable draws from..to-1: grouped, first-seen order
  static int gid[GDRAWS], order[GDRAWS], members[GDRAWS];
  int ng = 0;
  for (int i = from; i < to; i++) {
    const short *s = &g_gsnap[g_gd[i].snap]; int g = -1;
    for (int j = 0; j < ng && g < 0; j++) {
      const short *t = &g_gsnap[g_gd[order[j]].snap]; bool eq = true;
      for (int k = 0; k < g_ngkey && eq; k++) eq = gsame(s[k], t[k]) || (s[k] < 0 && t[k] < 0);
      if (eq) g = j;
    }
    if (g < 0) { g = ng; order[ng++] = i; }
    gid[i] = g;
  }
  for (int g = 0; g < ng; g++) {
    int n = 0; for (int i = from; i < to; i++) if (gid[i] == g) members[n++] = i;
    grp_emit_group(members, n);
  }
}
static int g_gdepth;
volatile int sf_group_on;
extern "C" __declspec(dllexport) void sf_group_begin(void) {
  if (g_gdepth++ || !sf_group_on || !render_thread() || g_recording) return;
  grp_reset(); g_grp = true; g_gModels++;
}
extern "C" __declspec(dllexport) void sf_group_end(void) {
  if (--g_gdepth || !g_grp) return;
  g_grp = false;
  for (int k = 0; k < g_ngkey; k++) g_gem[k] = -1;
  g_gemOurs = false;
  int seg = 0;
  for (int i = 0; i < g_ngd; i++) {
    if (g_gd[i].reord) continue;
    grp_emit_segment(seg, i);                    // what comes before a draw that keeps its place
    gem_to(&g_gsnap[g_gd[i].snap]); greplay(g_gd[i].op); g_gOut++; g_gBarrier++;
    seg = i + 1;
  }
  grp_emit_segment(seg, g_ngd);
  // the device to the state the game left it in: every key's last held value
  for (int k = 0; k < g_ngkey; k++) {
    bool si = g_gkey[k] == ((DWORD)G_SI << 16);
    if (g_gcur[k] >= 0 && ((si && g_gemOurs) || !gsame(g_gem[k], g_gcur[k]))) greplay(g_gcur[k]);
  }
  grp_reset();
}

// with batch_up_draws, extended_state_cache and lazy_state off at the start, only these are hooked (the draws for
// null_draws, which switches live)
static bool core_slot(int s) {
  switch (s) {
  case S_Reset: case S_Present: case S_BeginScene: case S_EndScene: case S_BeginSB: case S_EndSB: case S_ApplySB:
  case S_CaptureSB: case S_DeleteSB: case S_CreateSB:
  case S_SetRenderState: case S_SetTexture: case S_SetTSS: case S_SetVS: case S_DeleteVS: case 75 /* CreateVertexShader */: return true;
  case S_DP: case S_DIP: case S_DPUP: case S_DIPUP: return true;
  }
  return false;
}

// ---- installation --------------------------------------------------------------------------------------------
static void *hook_for(int s) {
  switch (s) {
  case S_Reset: return (void *)h_Reset; case S_Present: return (void *)h_Present;
  case S_BeginScene: return (void *)h_BeginScene; case S_EndScene: return (void *)h_EndScene;
  case S_BeginSB: return (void *)h_BeginSB; case S_EndSB: return (void *)h_EndSB; case S_ApplySB: return (void *)h_ApplySB;
  case S_CaptureSB: return (void *)h_CaptureSB; case S_DeleteSB: return (void *)h_DeleteSB; case S_CreateSB: return (void *)h_CreateSB;
  case S_SetRenderState: return (void *)h_SetRenderState; case S_SetTexture: return (void *)h_SetTexture;
  case S_SetTSS: return (void *)h_SetTSS; case S_SetVS: return (void *)h_SetVS; case S_DeleteVS: return (void *)h_DeleteVS;
  case S_SetTransform: return (void *)h_SetTransform; case S_SetViewport: return (void *)h_SetViewport;
  case S_SetMaterial: return (void *)h_SetMaterial; case S_SetLight: return (void *)h_SetLight;
  case S_LightEnable: return (void *)h_LightEnable; case S_SetStream: return (void *)h_SetStream;
  case S_SetIndices: return (void *)h_SetIndices; case S_DPUP: return (void *)h_DPUP; case S_DP: return (void *)h_DP;
  case S_DIP: return (void *)h_DIP; case S_DIPUP: return (void *)h_DIPUP; case 75: return (void *)h_CreateVS;
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
  InitializeCriticalSection(&g_vsdLock);
  int hooked = 0, stubs = 0;
  // Every hooked method costs a pass through the kit's front layer and this one on each call, which d3dprof (behind
  // them) cannot see. Batching needs every method (each flushes what is pending first) and the extended cache needs
  // its setters; with both off at the start only the cached setters and the methods that change or bound their
  // state are hooked (the owner's first capture: some 20,000 calls a frame were otherwise passed through for nothing).
  // lazy_state needs every method that reads the state it holds (other_state: the rest)
  g_level = sf_cfg.batch_up || sf_cfg.ext_cache || sf_cfg.skin_merge >= 2 ? 2 : sf_cfg.lazy_state ? 1 : 0;
  g_motion = sf_cfg.motion_stats != 0;
  // 0-2 (QueryInterface, AddRef, Release) do not render. Every other method: its own hook, or a flushing stub
  for (int s = 3; s < NSLOT; s++) {
    bool scene_slot = s == 28 || s == 29 || s == 30;                     // merge_scenes closes a held scene before them
    bool motion_slot = g_motion && ((s >= 20 && s <= 28) || s == S_SetTransform || (s >= S_DP && s <= S_DIPUP));
    if (!(g_level == 2 || core_slot(s) || scene_slot || motion_slot || (g_level == 1 && !other_state(s)))) continue;
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
            g_level == 2 ? "" : g_level == 1 ? " (batching and the extended cache were off at the start: turning either on takes effect at the next start)"
            : " (batching, the extended cache and lazy_state were off at the start: turning one on takes effect at the next start)");
  return 0;
}

// a cache turned on starts from an empty shadow: nothing recorded while it was off is trusted
extern "C" int sf_device_allows(const char *key) {
  if (!strcmp(key, "batch_up_draws") || !strcmp(key, "extended_state_cache")) return g_level == 2;
  if (!strcmp(key, "lazy_state")) return g_level >= 1;
  return 1;
}
extern "C" void sf_device_config_changed(const SfConfig *b) {
  if (g_level < 2 && (sf_cfg.batch_up || sf_cfg.ext_cache)) {
    if (sf_cfg.batch_up != b->batch_up || sf_cfg.ext_cache != b->ext_cache)
      H->log("speedfix: batch_up_draws / extended_state_cache take effect at the next start (they were off at this one, so their hooks are not in place)");
    sf_cfg.batch_up = 0; sf_cfg.ext_cache = 0;
  }
  if (g_level < 1 && sf_cfg.lazy_state) {
    if (!b->lazy_state) H->log("speedfix: lazy_state takes effect at the next start (it was off at this one, so its hooks are not in place)");
    sf_cfg.lazy_state = 0;
  }
  if (!sf_cfg.lazy_state && b->lazy_state) commit();     // nothing stays held
  if (!sf_cfg.state_cache && !sf_cfg.lazy_state && (b->state_cache || b->lazy_state)) {   // the shadow is no longer kept true
    memset(g_rsV, 0, sizeof g_rsV); memset(g_tssV, 0, sizeof g_tssV); memset(g_texV, 0, sizeof g_texV);
  }
  if (cache_on() && !(b->state_cache || b->lazy_state)) { memset(g_rsV, 0, sizeof g_rsV); memset(g_tssV, 0, sizeof g_tssV); memset(g_texV, 0, sizeof g_texV); }
  if (sf_cfg.vs_cache && !b->vs_cache) g_vsV = false;
  if (!sf_cfg.batch_up && b->batch_up) flush();
}

// ---- motion_stats (speedfix_hook's still/moving split) ----------------------------------------------------------
// The camera counts as moving in a frame when the first view transform of that frame differs from the previous
// frame's first one (any element by more than 1e-4). Per frame: draws (DrawPrimitive, DrawIndexedPrimitive,
// DrawPrimitiveUP, DrawIndexedPrimitiveUP), UP draws, CopyRects (the occlusion readback copies its target with one),
// textures bound for the first time this session (a table of 32,768 pointers, cleared when three quarters full), and
// resource creations (the Create methods and CreateVertexShader). Render thread; frames whose scene went through the
// hooks only. Logged each minute with sf_device_stats.
static LONG g_mfDraws, g_mfUp, g_mfCopy, g_mfNewTex, g_mfCreates; static bool g_mfViewTaken, g_mfMoving;
static float g_prevView[16]; static bool g_prevViewV;
struct MotionAcc { LONG n; double ms, draws, up, copies, newTex, creates; float samp[4096]; int ns; };
static MotionAcc g_mot[2];
static const void *g_texSeen[32768]; static int g_texSeenN; static LONG g_texSeenResets;
static void mot_count(int id) {
  if (id >= S_DP && id <= S_DIPUP) { g_mfDraws++; if (id == S_DPUP) g_mfUp++; }
  else if (id == 28) g_mfCopy++;
  else if ((id >= 20 && id <= 27) || id == 75) g_mfCreates++;
}
static void mot_view(const D3DMATRIX *m) {
  if (g_mfViewTaken) return;
  g_mfViewTaken = true;
  const float *f = (const float *)m;
  if (g_prevViewV) for (int i = 0; i < 16; i++) if (fabsf(f[i] - g_prevView[i]) > 1e-4f) { g_mfMoving = true; break; }
  memcpy(g_prevView, f, sizeof g_prevView); g_prevViewV = true;
}
static void mot_texture(const void *t) {
  unsigned h = (unsigned)(((uintptr_t)t >> 4) * 2654435761u) & 32767;
  while (g_texSeen[h]) { if (g_texSeen[h] == t) return; h = (h + 1) & 32767; }
  if (g_texSeenN >= 24576) { memset(g_texSeen, 0, sizeof g_texSeen); g_texSeenN = 0; g_texSeenResets++; mot_texture(t); return; }
  g_texSeen[h] = t; g_texSeenN++; g_mfNewTex++;
}
static void mot_frame(double iv) {
  MotionAcc &a = g_mot[g_mfMoving ? 1 : 0];
  a.n++; a.ms += iv; a.draws += g_mfDraws; a.up += g_mfUp; a.copies += g_mfCopy; a.newTex += g_mfNewTex; a.creates += g_mfCreates;
  if (a.ns < 4096) a.samp[a.ns++] = (float)iv;
}
static void mot_reset_frame(void) { g_mfDraws = g_mfUp = g_mfCopy = g_mfNewTex = g_mfCreates = 0; g_mfViewTaken = g_mfMoving = false; }
static int cmp_f(const void *a, const void *b) { float x = *(const float *)a, y = *(const float *)b; return (x > y) - (x < y); }
static void mot_log(void) {
  static const char *const nm[2] = {"camera still ", "camera moving"};
  for (int k = 0; k < 2; k++) {
    MotionAcc &a = g_mot[k];
    if (!a.n) { H->log("speedfix: motion: %s: no frames", nm[k]); continue; }
    qsort(a.samp, a.ns, sizeof a.samp[0], cmp_f);
    H->log("speedfix: motion: %s: %ld frames, mean %.1f ms, p50 %.1f, p95 %.1f | per frame: draws %.0f (UP %.0f), CopyRects %.1f, "
           "first-time texture binds %.2f, creations %.2f", nm[k], a.n, a.ms / a.n, a.samp[a.ns / 2], a.samp[a.ns * 95 / 100],
           a.draws / a.n, a.up / a.n, a.copies / a.n, a.newTex / a.n, a.creates / a.n);
    memset(&a, 0, sizeof a);
  }
  if (g_texSeenResets) { H->log("speedfix: motion: the first-bind table was cleared %ld times (counts high in those frames)", g_texSeenResets); g_texSeenResets = 0; }
}

// ---- frame pacing (render thread, before Present) ---------------------------------------------------------------
static LARGE_INTEGER g_qpf; static LONGLONG g_next; static HANDLE g_timer; static bool g_timerTried; static LONGLONG g_lastQpc;
static LONG g_pcN, g_pcLate; static double g_pcSum, g_pcSum2, g_pcMax;
extern "C" void sf_device_frame(void) {
  // Held changes never outlive a frame: the kit may take the hooks off from the next one (F9, a trace, its A/B), and
  // the game would then set state past them. This runs below the hooks (g_prev), whether they are on or not.
  if (g_npl && render_thread()) commit();
  bool live = sf_hooks_live();                     // (the frame that just ended went through the hooks; read before the number moves on)
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
    if (g_motion && live) mot_frame(iv);
  }
  if (g_motion) mot_reset_frame();
  g_lastQpc = now.QuadPart;
}

extern "C" void sf_device_stats(void) {
  LONG f = InterlockedExchange(&g_fwd, 0), s = InterlockedExchange(&g_skip, 0);
  LONG in = InterlockedExchange(&g_upIn, 0), out = InterlockedExchange(&g_upOut, 0), cv = InterlockedExchange(&g_upConv, 0);
  LONG fo = InterlockedExchange(&g_foreign, 0), gap = InterlockedExchange(&g_forgot_gap, 0);
  LONG lh = InterlockedExchange(&g_held, 0), ls = InterlockedExchange(&g_sent, 0), ld = InterlockedExchange(&g_dropped, 0);
  LONG vm = InterlockedExchange(&g_vsdMade, 0), vsh = InterlockedExchange(&g_vsdShared, 0), vk = InterlockedExchange(&g_vsdKept, 0);
  if (lh + ld) H->log("speedfix: lazy_state: %ld changes held, %ld sent, %ld calls never sent (undone or replaced before a read)", lh, ls, ld);
  if (vm + vsh + vk) H->log("speedfix: vs_dedupe: %ld vertex shaders made, %ld creations given an existing one, %ld deletions of a shared one kept it", vm, vsh, vk);
  if (f + s) H->log("speedfix: cached setters: %ld forwarded, %ld skipped (%.1f%%)", f, s, 100.0 * s / (double)(f + s));
  if (in) H->log("speedfix: DrawPrimitiveUP: %ld from the game, %ld sent (%.1f%% fewer), %ld strips/fans converted%s",
                 in, out, 100.0 * (in - out) / (double)in, cv, sf_cfg.batch_up ? "" : " [batching off]");
  if (fo) H->log("speedfix: %ld device calls from other threads (each made the shadow unknown)", fo);
  if (gap) H->log("speedfix: shadow forgotten %ld times after frames without the hooks (F9, a trace)", gap);
  LONG bk = InterlockedExchange(&g_blkKnown, 0), bf = InterlockedExchange(&g_blkForgot, 0);
  if (sf_cfg.block_cache && bk + bf) H->log("speedfix: block_cache: %ld state blocks applied with their contents known, %ld not (the shadow forgotten)", bk, bf);
  LONG nd = InterlockedExchange(&g_nullDrawn, 0);
  if (nd || sf_cfg.null_draws) H->log("speedfix: null_draws: %ld draw calls not sent to the device", nd);
  LONG gm = InterlockedExchange(&g_gModels, 0), gi = InterlockedExchange(&g_gIn, 0), go = InterlockedExchange(&g_gOut, 0);
  LONG gb = InterlockedExchange(&g_gBarrier, 0), gor = InterlockedExchange(&g_gOrder, 0), gbu = InterlockedExchange(&g_gBuilt, 0);
  LONG gh = InterlockedExchange(&g_gHit, 0), gn = InterlockedExchange(&g_gNoCache, 0);
  if (gm) H->log("speedfix: skin_merge=2: %ld model draws grouped: %ld draws held, %ld sent (%.1f%% fewer), %ld kept in place (blended or not a list), %ld times sent in the game's order (another call between); index lists: %ld built, %ld reused, %ld not joinable (%d kept)",
                 gm, gi, go, gi ? 100.0 * (gi - go) / (double)gi : 0.0, gb, gor, gbu, gh, gn, g_ngib);
  LONG me = InterlockedExchange(&g_mergedEnds, 0);
  if (me) H->log("speedfix: merge_scenes: %ld EndScene/BeginScene pairs inside frames dropped", me);
  if (g_motion) mot_log();
  if (g_pcN > 1) {
    double mean = g_pcSum / g_pcN, var = g_pcSum2 / g_pcN - mean * mean;
    H->log("speedfix: frames %ld, mean %.2f ms, sd %.2f, max %.1f%s", g_pcN, mean, var > 0 ? sqrt(var) : 0.0, g_pcMax,
           sf_cfg.frame_cap_fps ? "" : " (no cap)");
    if (sf_cfg.frame_cap_fps) H->log("speedfix: cap %d fps: %ld frames late (over 1.5 periods)", sf_cfg.frame_cap_fps, g_pcLate);
  }
  g_pcN = g_pcLate = 0; g_pcSum = g_pcSum2 = g_pcMax = 0;
}
