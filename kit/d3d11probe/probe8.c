/* probe8.c -- phase 0 of the D3D11 renderer (docs/D3D11.md): the calls and the frame of probe11's bench, through
   Direct3D 8 as the game makes them (fixed-function lighting, the FVF XYZ|NORMAL|DIFFUSE|TEX1, DrawPrimitiveUP for
   the UI). Run from the folder of the game's d3d8.dll (PlayOnlineViewer, where dgVoodoo's is), Windows loads that
   one first; which file was loaded is logged with its version.

     probe8 bench OUTDIR        -> OUTDIR\probe8_x86.csv

   Direct3D 8 has no event query, so a frame's time here includes Present but not a wait for the GPU. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d8.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static FILE *g_out; static double g_qpf;
static void put(const char *k, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
static void put(const char *k, const char *fmt, ...) {
  va_list a; va_start(a, fmt); fprintf(g_out, "%s,", k); vfprintf(g_out, fmt, a); fputc('\n', g_out); fflush(g_out); va_end(a);
  va_start(a, fmt); printf("%-40s ", k); vprintf(fmt, a); putchar('\n'); va_end(a);
}
static double now_us(void) { LARGE_INTEGER t; QueryPerformanceCounter(&t); return t.QuadPart * 1e6 / g_qpf; }
/* the clock's smallest observed step: in run 20261006_124338 probe8's times were all multiples of about 2.86 ms
   while probe11's, in the same folder, were not; the cause is not known, so each run measures and logs it */
static double timer_step_us(void) {
  double best = 1e9;
  for (int k = 0; k < 50; k++) {
    LARGE_INTEGER a, b; QueryPerformanceCounter(&a);
    do QueryPerformanceCounter(&b); while (b.QuadPart == a.QuadPart);
    double d = (b.QuadPart - a.QuadPart) * 1e6 / g_qpf; if (d < best) best = d;
  }
  return best;
}
static int cmp_d(const void *a, const void *b) { double x = *(const double *)a, y = *(const double *)b; return x < y ? -1 : x > y; }
static LRESULT CALLBACK wndproc(HWND h, UINT m, WPARAM w, LPARAM l) { return DefWindowProcA(h, m, w, l); }
static void pump(void) { MSG m; while (PeekMessageA(&m, NULL, 0, 0, PM_REMOVE)) { TranslateMessage(&m); DispatchMessageA(&m); } }

typedef struct { float x, y, z, nx, ny, nz; DWORD c; float u, v; } Vtx;
#define FVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
enum { NVB = 64, NTEX = 32, MODEL_IDX = 174, BIG = 50 };
static IDirect3DDevice8 *d; static IDirect3DVertexBuffer8 *g_vb[NVB], *g_dyn; static IDirect3DIndexBuffer8 *g_ib, *g_bigib;
static IDirect3DTexture8 *g_tex[NTEX]; static D3DMATRIX g_w; static D3DLIGHT8 g_light;

static void module_info(void) {
  HMODULE m = GetModuleHandleA("d3d8.dll"); char p[MAX_PATH] = "?"; if (m) GetModuleFileNameA(m, p, sizeof p);
  char fv[64] = "?"; DWORD h = 0, sz = GetFileVersionInfoSizeA(p, &h);
  void *buf = sz ? malloc(sz) : NULL; VS_FIXEDFILEINFO *fi = NULL; UINT fl = 0;
  if (buf && GetFileVersionInfoA(p, 0, sz, buf) && VerQueryValueA(buf, "\\", (LPVOID *)&fi, &fl) && fi)
    snprintf(fv, sizeof fv, "%u.%u.%u.%u", (unsigned)(fi->dwFileVersionMS >> 16), (unsigned)(fi->dwFileVersionMS & 0xFFFF), (unsigned)(fi->dwFileVersionLS >> 16), (unsigned)(fi->dwFileVersionLS & 0xFFFF));
  free(buf);
  put("d3d8.dll", "%s, version %s", p, fv);
}
static void set_world(int i) { memset(&g_w, 0, sizeof g_w); g_w._11 = g_w._22 = g_w._33 = g_w._44 = 1; g_w._41 = (i % 40) * 0.025f - 0.5f; g_w._42 = (i / 40 % 40) * 0.02f - 0.4f; IDirect3DDevice8_SetTransform(d, D3DTS_WORLD, &g_w); }
static void set_light(int i) { g_light.Diffuse.r = 0.5f + (i & 7) * 0.05f; IDirect3DDevice8_SetLight(d, 0, &g_light); }
static void up_draw(void) {
  static Vtx q[6]; for (int i = 0; i < 6; i++) { q[i].x = 0.6f + (i % 3) * 0.01f; q[i].y = 0.6f; q[i].z = 0.1f; q[i].nz = -1; q[i].c = 0xFFFFFFFF; }
  IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLELIST, 2, q, sizeof(Vtx));
}
static void micro(const char *name, int kind) {
  enum { K = 200000, FR = 7 }; double ns[FR];
  for (int f = 0; f < FR; f++) {
    IDirect3DDevice8_Clear(d, 0, NULL, D3DCLEAR_TARGET, 0xFF202040, 1, 0); IDirect3DDevice8_BeginScene(d);
    double t0 = now_us();
    for (int i = 0; i < K; i++) switch (kind) {
      case 0: IDirect3DDevice8_SetRenderState(d, D3DRS_ALPHABLENDENABLE, i & 1); break;
      case 1: IDirect3DDevice8_SetTexture(d, 0, (IDirect3DBaseTexture8 *)g_tex[i & 1]); break;
      case 2: IDirect3DDevice8_SetStreamSource(d, 0, g_vb[i & 1], sizeof(Vtx)); break;
      case 3: set_world(i); break;
      case 4: set_light(i); break;
      case 5: IDirect3DDevice8_DrawIndexedPrimitive(d, D3DPT_TRIANGLELIST, 0, 60, 0, 2); break;
      case 6: up_draw(); break;
      case 7: { BYTE *p; if (SUCCEEDED(IDirect3DVertexBuffer8_Lock(g_dyn, 0, 6 * sizeof(Vtx), &p, D3DLOCK_DISCARD))) IDirect3DVertexBuffer8_Unlock(g_dyn); } break;
      case 8: IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_COLOROP, (i & 1) ? D3DTOP_MODULATE : D3DTOP_SELECTARG1); break;
      case 9: { BYTE *p; if (SUCCEEDED(IDirect3DVertexBuffer8_Lock(g_dyn, 0, 6 * sizeof(Vtx), &p, D3DLOCK_NOOVERWRITE))) IDirect3DVertexBuffer8_Unlock(g_dyn); } break;
      case 10: {                                  /* a ring draw: what speedfix's up_ring did for each batch */
        static UINT pos; UINT bytes = 6 * sizeof(Vtx); DWORD fl = D3DLOCK_NOOVERWRITE; BYTE *p;
        if (pos + bytes > 4096 * sizeof(Vtx)) { pos = 0; fl = D3DLOCK_DISCARD; }
        if (SUCCEEDED(IDirect3DVertexBuffer8_Lock(g_dyn, pos, bytes, &p, fl))) { memset(p, 0, bytes); IDirect3DVertexBuffer8_Unlock(g_dyn); }
        IDirect3DDevice8_SetStreamSource(d, 0, g_dyn, sizeof(Vtx));
        IDirect3DDevice8_DrawPrimitive(d, D3DPT_TRIANGLELIST, pos / sizeof(Vtx), 2); pos += bytes; } break;
    }
    double t1 = now_us();
    IDirect3DDevice8_EndScene(d); IDirect3DDevice8_Present(d, NULL, NULL, NULL, NULL); pump();
    ns[f] = (t1 - t0) * 1000.0 / K;
    IDirect3DDevice8_SetStreamSource(d, 0, g_vb[0], sizeof(Vtx));
  }
  qsort(ns, FR, sizeof ns[0], cmp_d);
  put(name, "%.1f ns a call (median of %d frames of %d calls)", ns[FR / 2], FR, K);
  IDirect3DDevice8_SetRenderState(d, D3DRS_ALPHABLENDENABLE, FALSE);
}
static void frame_bench(const char *name, int merged) {
  enum { FR = 240 }; static double cpu[FR], tot[FR];
  for (int f = 0; f < FR; f++) {
    double t0 = now_us();
    IDirect3DDevice8_Clear(d, 0, NULL, D3DCLEAR_TARGET, 0xFF202040, 1, 0); IDirect3DDevice8_BeginScene(d);
    if (!merged) {
      IDirect3DDevice8_SetIndices(d, g_ib, 0);
      for (int i = 0; i < 1600; i++) {
        IDirect3DDevice8_SetStreamSource(d, 0, g_vb[i % NVB], sizeof(Vtx)); set_world(i); set_light(i);
        if (i % 3 == 0) IDirect3DDevice8_SetTexture(d, 0, (IDirect3DBaseTexture8 *)g_tex[i % NTEX]);
        if (i % 4 == 0) IDirect3DDevice8_SetRenderState(d, D3DRS_ALPHABLENDENABLE, (i / 4) & 1);
        IDirect3DDevice8_DrawIndexedPrimitive(d, D3DPT_TRIANGLELIST, 0, 60, 0, MODEL_IDX / 3);
      }
      IDirect3DDevice8_SetRenderState(d, D3DRS_ALPHABLENDENABLE, TRUE);
      for (int i = 0; i < 900; i++) up_draw();
    } else {
      IDirect3DDevice8_SetIndices(d, g_bigib, 0);
      for (int i = 0; i < 32; i++) {
        IDirect3DDevice8_SetStreamSource(d, 0, g_vb[i % NVB], sizeof(Vtx)); set_world(i); set_light(i);
        IDirect3DDevice8_SetTexture(d, 0, (IDirect3DBaseTexture8 *)g_tex[i % NTEX]);
        IDirect3DDevice8_DrawIndexedPrimitive(d, D3DPT_TRIANGLELIST, 0, 60, 0, MODEL_IDX / 3 * BIG);
      }
      IDirect3DDevice8_SetRenderState(d, D3DRS_ALPHABLENDENABLE, TRUE);
      static Vtx q[900 * 6]; for (int i = 0; i < 900 * 6; i++) { q[i].x = 0.6f + (i % 3) * 0.01f; q[i].y = 0.6f; q[i].z = 0.1f; q[i].nz = -1; q[i].c = 0xFFFFFFFF; }
      IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLELIST, 1800, q, sizeof(Vtx));
    }
    IDirect3DDevice8_SetRenderState(d, D3DRS_ALPHABLENDENABLE, FALSE);
    IDirect3DDevice8_EndScene(d);
    double t1 = now_us();
    IDirect3DDevice8_Present(d, NULL, NULL, NULL, NULL);
    double t2 = now_us(); pump();
    cpu[f] = (t1 - t0) / 1000.0; tot[f] = (t2 - t0) / 1000.0;
  }
  double ms = 0, mt = 0; for (int k = 20; k < FR; k++) { ms += cpu[k]; mt += tot[k]; }   /* means: exact whatever the clock's step */
  ms /= FR - 20; mt /= FR - 20;
  qsort(cpu + 20, FR - 20, sizeof cpu[0], cmp_d); qsort(tot + 20, FR - 20, sizeof tot[0], cmp_d);
  int n = FR - 20;
  put(name, "submit %.2f ms (p95 %.2f), with Present %.2f ms (p95 %.2f); median of %d frames; means %.2f / %.2f ms", cpu[20 + n / 2], cpu[20 + n * 95 / 100], tot[20 + n / 2], tot[20 + n * 95 / 100], n, ms, mt);
}

/* speedfix's block_cache rests on how a recorded state block behaves (kit/speedfix/sf_device.cpp; docs/HISTORY.md
   11 Oct): the sets between BeginStateBlock and EndStateBlock are recorded, not applied; applying the block sets
   what it recorded, a state set twice at its last value, and leaves every other state as it was. Checked here
   through this folder's d3d8.dll (the one the game uses), before any resource is made so that Reset can run:
   whether the block is still usable after Reset is logged as found (the game re-records only if its token is -1). */
static DWORD rs(D3DRENDERSTATETYPE s) { DWORD v = 0xDEADBEEF; IDirect3DDevice8_GetRenderState(d, s, &v); return v; }
static DWORD tss(DWORD st, D3DTEXTURESTAGESTATETYPE t) { DWORD v = 0xDEADBEEF; IDirect3DDevice8_GetTextureStageState(d, st, t, &v); return v; }
static void stateblock_check(D3DPRESENT_PARAMETERS *pp) {
  IDirect3DDevice8_SetRenderState(d, D3DRS_ALPHAREF, 0x10); IDirect3DDevice8_SetRenderState(d, D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
  IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_MINFILTER, D3DTEXF_POINT); IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_MAGFILTER, D3DTEXF_POINT);
  DWORD tok = 0xFFFFFFFF;
  HRESULT h1 = IDirect3DDevice8_BeginStateBlock(d);
  IDirect3DDevice8_SetRenderState(d, D3DRS_ALPHAREF, 0x60); IDirect3DDevice8_SetRenderState(d, D3DRS_ALPHAREF, 0x61);
  IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
  HRESULT h2 = IDirect3DDevice8_EndStateBlock(d, &tok);
  DWORD rec_ar = rs(D3DRS_ALPHAREF), rec_mag = tss(0, D3DTSS_MAGFILTER);
  put("stateblock.record", "Begin hr 0x%08lX, End hr 0x%08lX, token 0x%lX; after recording ALPHAREF 0x%lX MAGFILTER %lu (%s)",
      (unsigned long)h1, (unsigned long)h2, (unsigned long)tok, (unsigned long)rec_ar, (unsigned long)rec_mag,
      rec_ar == 0x10 && rec_mag == D3DTEXF_POINT ? "PASS: recorded, not applied" : "FAIL: the recording changed the device");
  IDirect3DDevice8_SetRenderState(d, D3DRS_ALPHAREF, 0x20); IDirect3DDevice8_SetRenderState(d, D3DRS_ZFUNC, D3DCMP_ALWAYS);
  IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
  HRESULT h3 = IDirect3DDevice8_ApplyStateBlock(d, tok);
  DWORD ar = rs(D3DRS_ALPHAREF), zf = rs(D3DRS_ZFUNC), mn = tss(0, D3DTSS_MINFILTER), mg = tss(0, D3DTSS_MAGFILTER);
  int ok = ar == 0x61 && mg == D3DTEXF_LINEAR && zf == D3DCMP_ALWAYS && mn == D3DTEXF_LINEAR;
  put("stateblock.apply", "hr 0x%08lX; ALPHAREF 0x%lX (block: 0x61, its last value), MAGFILTER %lu (block: 2); not in the block: ZFUNC %lu (set 8), MINFILTER %lu (set 2) (%s)",
      (unsigned long)h3, (unsigned long)ar, (unsigned long)mg, (unsigned long)zf, (unsigned long)mn,
      ok ? "PASS: the block set what it recorded and nothing else" : "FAIL");
  D3DPRESENT_PARAMETERS q = *pp;
  HRESULT h4 = IDirect3DDevice8_Reset(d, &q);
  IDirect3DDevice8_SetRenderState(d, D3DRS_ALPHAREF, 0x20);
  HRESULT h5 = IDirect3DDevice8_ApplyStateBlock(d, tok);
  put("stateblock.after Reset", "Reset hr 0x%08lX; ApplyStateBlock of the old token hr 0x%08lX, ALPHAREF then 0x%lX (found, not judged)",
      (unsigned long)h4, (unsigned long)h5, (unsigned long)rs(D3DRS_ALPHAREF));
  IDirect3DDevice8_DeleteStateBlock(d, tok);
}
/* What goal 2 needs to know from this d3d8.dll (docs/D3D11.md): the shader limits GPU skinning would work within,
   and what the game's occlusion readback pattern costs here. The pattern is the one the game's code shows
   (docs/HISTORY.md, 11 Oct): a probe drawn into a 16x16 render-target texture, CopyRects into a lockable 16x16 render
   target, LockRect(READONLY) at once. This imitates it; it is not the game's code. */
static int g_swvp;
static void caps_log(void) {
  D3DCAPS8 c; if (FAILED(IDirect3DDevice8_GetDeviceCaps(d, &c))) { put("caps", "GetDeviceCaps failed"); return; }
  put("caps.vertex shader", "version %lu.%lu, MaxVertexShaderConst %lu; device made with %s vertex processing",
      (unsigned long)((c.VertexShaderVersion >> 8) & 0xFF), (unsigned long)(c.VertexShaderVersion & 0xFF), (unsigned long)c.MaxVertexShaderConst,
      g_swvp ? "SOFTWARE" : "HARDWARE");
  put("caps.pixel shader", "version %lu.%lu", (unsigned long)((c.PixelShaderVersion >> 8) & 0xFF), (unsigned long)(c.PixelShaderVersion & 0xFF));
  put("caps.vertex blending", "MaxVertexBlendMatrices %lu, MaxVertexBlendMatrixIndex %lu, MaxStreams %lu, bone indices as UBYTE4 in a vertex shader: %s",
      (unsigned long)c.MaxVertexBlendMatrices, (unsigned long)c.MaxVertexBlendMatrixIndex, (unsigned long)c.MaxStreams,
      (c.VertexProcessingCaps & D3DVTXPCAPS_NO_VSDT_UBYTE4) ? "not supported (D3DVTXPCAPS_NO_VSDT_UBYTE4)" : "supported");
  { char hx[sizeof c * 2 + 1]; const unsigned char *b = (const unsigned char *)&c;   /* the whole D3DCAPS8, for d3d8on11 to answer the same */
    for (size_t i = 0; i < sizeof c; i++) sprintf(hx + 2 * i, "%02X", b[i]);
    put("caps.raw", "%u bytes: %s", (unsigned)sizeof c, hx); }
  put("caps.limits", "MaxPrimitiveCount %lu, MaxVertexIndex %lu, MaxActiveLights %lu, VertexProcessingCaps 0x%lX, DevCaps 0x%lX",
      (unsigned long)c.MaxPrimitiveCount, (unsigned long)c.MaxVertexIndex, (unsigned long)c.MaxActiveLights,
      (unsigned long)c.VertexProcessingCaps, (unsigned long)c.DevCaps);
}
static void plaza_draws(void) {                  /* the character part of frame_bench's D3D8 frame */
  IDirect3DDevice8_SetIndices(d, g_ib, 0);
  for (int i = 0; i < 1600; i++) {
    IDirect3DDevice8_SetStreamSource(d, 0, g_vb[i % NVB], sizeof(Vtx)); set_world(i);
    if (i % 3 == 0) IDirect3DDevice8_SetTexture(d, 0, (IDirect3DBaseTexture8 *)g_tex[i % NTEX]);
    IDirect3DDevice8_DrawIndexedPrimitive(d, D3DPT_TRIANGLELIST, 0, 60, 0, MODEL_IDX / 3);
  }
}
static void readback_bench(void) {
  IDirect3DTexture8 *box = NULL; IDirect3DSurface8 *boxs = NULL, *lk = NULL, *bb = NULL, *ds = NULL;
  D3DDISPLAYMODE dm; IDirect3DDevice8_GetDisplayMode(d, &dm);
  HRESULT h1 = IDirect3DDevice8_CreateTexture(d, 16, 16, 1, D3DUSAGE_RENDERTARGET, dm.Format, D3DPOOL_DEFAULT, &box);
  if (box) IDirect3DTexture8_GetSurfaceLevel(box, 0, &boxs);
  HRESULT h2 = IDirect3DDevice8_CreateRenderTarget(d, 16, 16, dm.Format, D3DMULTISAMPLE_NONE, TRUE, &lk);
  if (!boxs || !lk) { put("readback", "not run: CreateTexture hr 0x%08lX, CreateRenderTarget hr 0x%08lX", (unsigned long)h1, (unsigned long)h2); goto out; }
  IDirect3DDevice8_GetRenderTarget(d, &bb); IDirect3DDevice8_GetDepthStencilSurface(d, &ds);
  static const struct { const char *name; int plaza, n, lock; } cs[] = {
    {"readback.1 query, empty frame", 0, 1, 1},
    {"readback.1 query after 1600 draws", 1, 1, 1},
    {"readback.8 queries after 1600 draws", 1, 8, 1},
    {"readback.8 probes after 1600 draws, no lock (what an answer a frame late costs)", 1, 8, 0},
  };
  for (int k = 0; k < 4; k++) {
    enum { FR = 80 }; double ms[FR]; int bad = 0;
    for (int f = 0; f < FR; f++) {
      IDirect3DDevice8_Clear(d, 0, NULL, D3DCLEAR_TARGET, 0xFF202040, 1, 0); IDirect3DDevice8_BeginScene(d);
      if (cs[k].plaza) plaza_draws();
      double t0 = now_us();
      for (int q = 0; q < cs[k].n; q++) {
        IDirect3DDevice8_SetRenderTarget(d, boxs, NULL);
        IDirect3DDevice8_Clear(d, 0, NULL, D3DCLEAR_TARGET, 0, 1, 0);
        up_draw();
        IDirect3DDevice8_SetRenderTarget(d, bb, ds);
        if (cs[k].lock) {
          D3DLOCKED_RECT lr;
          if (FAILED(IDirect3DDevice8_CopyRects(d, boxs, NULL, 0, lk, NULL))) bad++;
          if (SUCCEEDED(IDirect3DSurface8_LockRect(lk, &lr, NULL, D3DLOCK_READONLY))) { volatile DWORD x = *(DWORD *)lr.pBits; (void)x; IDirect3DSurface8_UnlockRect(lk); }
          else bad++;
        }
      }
      double t1 = now_us();
      IDirect3DDevice8_EndScene(d); IDirect3DDevice8_Present(d, NULL, NULL, NULL, NULL); pump();
      ms[f] = (t1 - t0) / 1000.0;
    }
    double mean = 0; for (int f = 10; f < FR; f++) mean += ms[f]; mean /= FR - 10;
    qsort(ms + 10, FR - 10, sizeof ms[0], cmp_d);
    put(cs[k].name, "%.3f ms (median of %d frames; p95 %.3f; mean %.3f)%s", ms[10 + (FR - 10) / 2], FR - 10, ms[10 + (FR - 10) * 95 / 100], mean,
        bad ? "; some CopyRects or LockRect failed" : "");
  }
out:
  if (bb) IDirect3DSurface8_Release(bb);
  if (ds) IDirect3DSurface8_Release(ds);
  if (boxs) IDirect3DSurface8_Release(boxs);
  if (box) IDirect3DTexture8_Release(box);
  if (lk) IDirect3DSurface8_Release(lk);
}
int main(int argc, char **argv) {
  LARGE_INTEGER f; QueryPerformanceFrequency(&f); g_qpf = (double)f.QuadPart;
  if (argc < 3 || strcmp(argv[1], "bench")) { printf("usage: probe8 bench OUTDIR\n"); return 2; }
  CreateDirectoryA(argv[2], NULL);
  char path[MAX_PATH]; snprintf(path, sizeof path, "%s\\probe8_x86.csv", argv[2]);
  g_out = fopen(path, "a"); if (!g_out) { printf("cannot write %s\n", path); return 2; }
  put("timer", "QueryPerformanceFrequency %.0f Hz; smallest step seen %.3f us (before Direct3D 8 is loaded)", g_qpf, timer_step_us());
  IDirect3D8 *d3d = Direct3DCreate8(D3D_SDK_VERSION);
  module_info();
  if (!d3d) { put("Direct3DCreate8", "failed"); return 1; }
  WNDCLASSA wc; memset(&wc, 0, sizeof wc); wc.lpfnWndProc = wndproc; wc.hInstance = GetModuleHandleA(NULL); wc.lpszClassName = "probe8"; RegisterClassA(&wc);
  RECT r = {0, 0, 1280, 720}; AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);
  HWND hw = CreateWindowA("probe8", "probe8 (d3d8)", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 40, 40, r.right - r.left, r.bottom - r.top, NULL, NULL, wc.hInstance, NULL); pump();
  D3DDISPLAYMODE dm; IDirect3D8_GetAdapterDisplayMode(d3d, D3DADAPTER_DEFAULT, &dm);
  D3DPRESENT_PARAMETERS pp; memset(&pp, 0, sizeof pp);
  pp.Windowed = TRUE; pp.SwapEffect = D3DSWAPEFFECT_DISCARD; pp.BackBufferFormat = dm.Format; pp.hDeviceWindow = hw;
  HRESULT hr = IDirect3D8_CreateDevice(d3d, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hw, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &d);
  if (FAILED(hr)) { g_swvp = 1; hr = IDirect3D8_CreateDevice(d3d, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hw, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &pp, &d); }
  put("device.create", "hr 0x%08lX", (unsigned long)hr);
  if (FAILED(hr)) return 1;
  D3DADAPTER_IDENTIFIER8 id; if (SUCCEEDED(IDirect3D8_GetAdapterIdentifier(d3d, D3DADAPTER_DEFAULT, 0, &id))) put("adapter", "%s (%s)", id.Description, id.Driver);
  stateblock_check(&pp);
  caps_log();
  /* resources: as probe11's */
  Vtx v[60]; for (int i = 0; i < 60; i++) { memset(&v[i], 0, sizeof v[i]); v[i].x = (i % 2) * 0.02f + (i / 2) * 0.001f - 0.5f; v[i].y = (i / 2) * 0.01f - 0.2f; v[i].z = 0.5f; v[i].nz = -1; v[i].c = 0xFFFFFFFF; v[i].u = (float)(i % 2); v[i].v = (float)(i / 2) / 30; }
  for (int i = 0; i < NVB; i++) { BYTE *p; IDirect3DDevice8_CreateVertexBuffer(d, sizeof v, D3DUSAGE_WRITEONLY, FVF, D3DPOOL_MANAGED, &g_vb[i]);
    if (g_vb[i] && SUCCEEDED(IDirect3DVertexBuffer8_Lock(g_vb[i], 0, 0, &p, 0))) { memcpy(p, v, sizeof v); IDirect3DVertexBuffer8_Unlock(g_vb[i]); } }
  IDirect3DDevice8_CreateVertexBuffer(d, 4096 * sizeof(Vtx), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, FVF, D3DPOOL_DEFAULT, &g_dyn);
  { BYTE *p; IDirect3DDevice8_CreateIndexBuffer(d, MODEL_IDX * 2, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_ib);
    if (SUCCEEDED(IDirect3DIndexBuffer8_Lock(g_ib, 0, 0, &p, 0))) { WORD *w = (WORD *)p; for (int t = 0; t < MODEL_IDX / 3; t++) { w[3 * t] = (WORD)t; w[3 * t + 1] = (WORD)(t + 1); w[3 * t + 2] = (WORD)(t + 2); } IDirect3DIndexBuffer8_Unlock(g_ib); }
    IDirect3DDevice8_CreateIndexBuffer(d, MODEL_IDX * 2 * BIG, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_bigib);
    if (SUCCEEDED(IDirect3DIndexBuffer8_Lock(g_bigib, 0, 0, &p, 0))) { WORD *w = (WORD *)p; for (int k = 0; k < BIG; k++) for (int t = 0; t < MODEL_IDX / 3; t++) { WORD *o = w + k * MODEL_IDX + 3 * t; o[0] = (WORD)t; o[1] = (WORD)(t + 1); o[2] = (WORD)(t + 2); } IDirect3DIndexBuffer8_Unlock(g_bigib); } }
  for (int i = 0; i < NTEX; i++) { D3DLOCKED_RECT lr; IDirect3DDevice8_CreateTexture(d, 64, 64, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &g_tex[i]);
    if (g_tex[i] && SUCCEEDED(IDirect3DTexture8_LockRect(g_tex[i], 0, &lr, NULL, 0))) { for (int y = 0; y < 64; y++) for (int x = 0; x < 64; x++) ((DWORD *)((BYTE *)lr.pBits + y * lr.Pitch))[x] = 0xFF808080u ^ (DWORD)((y * 64 + x) * 2654435761u); IDirect3DTexture8_UnlockRect(g_tex[i], 0); } }
  memset(&g_light, 0, sizeof g_light); g_light.Type = D3DLIGHT_DIRECTIONAL; g_light.Diffuse.r = g_light.Diffuse.g = g_light.Diffuse.b = g_light.Diffuse.a = 1; g_light.Direction.z = 1;
  IDirect3DDevice8_SetLight(d, 0, &g_light); IDirect3DDevice8_LightEnable(d, 0, TRUE);
  IDirect3DDevice8_SetRenderState(d, D3DRS_LIGHTING, TRUE); IDirect3DDevice8_SetRenderState(d, D3DRS_CULLMODE, D3DCULL_NONE);
  IDirect3DDevice8_SetRenderState(d, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); IDirect3DDevice8_SetRenderState(d, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  IDirect3DDevice8_SetVertexShader(d, FVF); IDirect3DDevice8_SetStreamSource(d, 0, g_vb[0], sizeof(Vtx)); IDirect3DDevice8_SetIndices(d, g_ib, 0);
  IDirect3DDevice8_SetTexture(d, 0, (IDirect3DBaseTexture8 *)g_tex[0]);
  put("timer", "smallest step seen %.3f us (with the device made)", timer_step_us());
  micro("call.SetRenderState", 0); micro("call.SetTexture", 1); micro("call.SetStreamSource", 2); micro("call.SetTransform (world)", 3);
  micro("call.SetLight", 4); micro("call.DrawIndexedPrimitive (2 triangles)", 5); micro("call.DrawPrimitiveUP (2 triangles)", 6);
  micro("call.VB Lock DISCARD + Unlock", 7); micro("call.SetTextureStageState", 8);
  micro("call.VB Lock NOOVERWRITE + Unlock", 9); micro("call.ring draw (Lock NOOVERWRITE + SetStreamSource + DrawPrimitive)", 10);
  frame_bench("frame.plaza-like as D3D8 issues it (1600+900 draws)", 0);
  frame_bench("frame.plaza-like merged (33 draws)", 1);
  readback_bench();
  return 0;
}
