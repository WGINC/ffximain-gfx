/* probe11.c -- phase 0 of the D3D11 renderer (docs/D3D11.md, section 4): what D3D11 costs on the owner's machine,
   from an emulated x86 process and from a native ARM64 one, and whether two processes can share the output.
   Built twice from this file (build.sh): probe11_x86.exe and probe11_arm64.exe.

     probe11 caps  OUTDIR            adapter, feature level, the formats the port needs, the shader compiler
     probe11 bench OUTDIR            per-call costs of the calls a D3D8-on-D3D11 device makes, and a frame shaped
                                     like Port Jeuno's plaza (docs/HISTORY.md, session 5) as many small draws and as
                                     few large ones
     probe11 xproc OUTDIR CHILD.exe  this process makes a window; CHILD (e.g. the other architecture's build) is
                                     started with `xchild HWND` and tries (1) a swap chain on that window and (2) a
                                     shared texture this process then draws from; both are checked by reading pixels
     probe11 xchild HWND HANDLE DIR  (started by xproc)

   Every result is one line `key,value` in OUTDIR\probe11_<arch>.csv (xproc: probe11_xproc_<arch>.csv), so
   phase0_report.py can set the builds side by side. A step that fails writes its HRESULT and the run goes on. */
#define COBJMACROS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__aarch64__) || defined(_M_ARM64)
#define ARCH "arm64"
#elif defined(__x86_64__) || defined(_M_X64)
#define ARCH "x64"
#else
#define ARCH "x86"
#endif

static FILE *g_out; static const char *g_outdir = ".";
static void put(const char *k, const char *fmt, ...) {
  va_list a; va_start(a, fmt);
  fprintf(g_out, "%s,", k); vfprintf(g_out, fmt, a); fputc('\n', g_out); fflush(g_out);
  va_end(a);
  va_start(a, fmt); printf("%-40s ", k); vprintf(fmt, a); putchar('\n'); va_end(a);
}
static double g_qpf;
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

static void process_info(void) {
  typedef BOOL (WINAPI *IW2)(HANDLE, USHORT *, USHORT *);
  IW2 iw2 = (IW2)(void *)GetProcAddress(GetModuleHandleA("kernel32.dll"), "IsWow64Process2");
  USHORT pm = 0, nm = 0;
  if (iw2 && iw2(GetCurrentProcess(), &pm, &nm)) put("process", "build %s; native machine 0x%X; process machine 0x%X (%s)", ARCH, nm, pm, pm ? "emulated or WOW64" : "native");
  else put("process", "build %s; IsWow64Process2 not available", ARCH);
  SYSTEM_INFO si; GetNativeSystemInfo(&si); put("cpus", "%lu", (unsigned long)si.dwNumberOfProcessors);
}
static void modules_info(void) {              /* which files were loaded: a d3d11.dll beside the exe would be used first */
  static const char *const n[] = {"d3d11.dll", "dxgi.dll", "d3dcompiler_47.dll"};
  for (int i = 0; i < 3; i++) { HMODULE m = GetModuleHandleA(n[i]); char p[MAX_PATH] = "(not loaded)"; if (m) GetModuleFileNameA(m, p, sizeof p); put("module", "%s: %s", n[i], p); }
}

/* ---- window ---------------------------------------------------------------------------------------------------- */
static LRESULT CALLBACK wndproc(HWND h, UINT m, WPARAM w, LPARAM l) { return DefWindowProcA(h, m, w, l); }
static HWND make_window(const char *title, int w, int h) {
  WNDCLASSA wc; memset(&wc, 0, sizeof wc);
  wc.lpfnWndProc = wndproc; wc.hInstance = GetModuleHandleA(NULL); wc.lpszClassName = "probe11"; wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  RegisterClassA(&wc);
  RECT r = {0, 0, w, h}; AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);
  HWND hw = CreateWindowA("probe11", title, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 40, 40, r.right - r.left, r.bottom - r.top, NULL, NULL, wc.hInstance, NULL);
  return hw;
}
static void pump(void) { MSG m; while (PeekMessageA(&m, NULL, 0, 0, PM_REMOVE)) { TranslateMessage(&m); DispatchMessageA(&m); } }

/* ---- device ---------------------------------------------------------------------------------------------------- */
static ID3D11Device *g_dev; static ID3D11DeviceContext *g_ctx; static IDXGISwapChain *g_sc; static ID3D11RenderTargetView *g_rtv;
static int make_device(HWND hw, int w, int h) {
  DXGI_SWAP_CHAIN_DESC sd; memset(&sd, 0, sizeof sd);
  sd.BufferDesc.Width = w; sd.BufferDesc.Height = h; sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.SampleDesc.Count = 1; sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; sd.BufferCount = 1; sd.OutputWindow = hw;
  sd.Windowed = TRUE; sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  D3D_FEATURE_LEVEL fl = 0;
  HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &sd, &g_sc, &g_dev, &fl, &g_ctx);
  put("device.create", "hr 0x%08lX feature level 0x%X", (unsigned long)hr, (unsigned)fl);
  if (FAILED(hr)) return 0;
  ID3D11Texture2D *bb = NULL;
  IDXGISwapChain_GetBuffer(g_sc, 0, &IID_ID3D11Texture2D, (void **)&bb);
  ID3D11Device_CreateRenderTargetView(g_dev, (ID3D11Resource *)bb, NULL, &g_rtv);
  ID3D11Texture2D_Release(bb);
  D3D11_VIEWPORT vp = {0, 0, (float)w, (float)h, 0, 1};
  ID3D11DeviceContext_RSSetViewports(g_ctx, 1, &vp);
  ID3D11DeviceContext_OMSetRenderTargets(g_ctx, 1, &g_rtv, NULL);
  return 1;
}

/* ---- the shader compiler (d3dcompiler_47.dll, loaded by name: whether it is there is one of the results) ------- */
typedef HRESULT (WINAPI *PFN_Compile)(LPCVOID, SIZE_T, LPCSTR, const D3D_SHADER_MACRO *, ID3DInclude *, LPCSTR, LPCSTR, UINT, UINT, ID3DBlob **, ID3DBlob **);
static PFN_Compile g_compile;
static ID3DBlob *compile(const char *src, const char *entry, const char *target) {
  if (!g_compile) return NULL;
  ID3DBlob *code = NULL, *err = NULL;
  HRESULT hr = g_compile(src, strlen(src), "probe", NULL, NULL, entry, target, D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &code, &err);
  if (FAILED(hr)) { put("compile.error", "%s %s hr 0x%08lX %s", entry, target, (unsigned long)hr, err ? (const char *)ID3D10Blob_GetBufferPointer(err) : ""); if (err) ID3D10Blob_Release(err); return NULL; }
  if (err) ID3D10Blob_Release(err);
  return code;
}

/* ---- caps -------------------------------------------------------------------------------------------------------- */
static void caps(void) {
  IDXGIFactory *f = NULL; IDXGIAdapter *a = NULL;
  if (SUCCEEDED(CreateDXGIFactory(&IID_IDXGIFactory, (void **)&f))) {
    for (UINT i = 0; IDXGIFactory_EnumAdapters(f, i, &a) == S_OK; i++) {
      DXGI_ADAPTER_DESC d; IDXGIAdapter_GetDesc(a, &d);
      char name[256]; WideCharToMultiByte(CP_UTF8, 0, d.Description, -1, name, sizeof name, NULL, NULL);
      put("adapter", "%u: %s; vendor 0x%04X device 0x%04X; dedicated video %lu MB, shared %lu MB", i, name, d.VendorId, d.DeviceId,
          (unsigned long)(d.DedicatedVideoMemory >> 20), (unsigned long)(d.SharedSystemMemory >> 20));
      IDXGIAdapter_Release(a);
    }
    IDXGIFactory_Release(f);
  }
  static const struct { DXGI_FORMAT f; const char *n; } fm[] = {
    {DXGI_FORMAT_BC1_UNORM, "BC1 (DXT1)"}, {DXGI_FORMAT_BC2_UNORM, "BC2 (DXT3)"}, {DXGI_FORMAT_BC3_UNORM, "BC3 (DXT5)"},
    {DXGI_FORMAT_B8G8R8A8_UNORM, "B8G8R8A8 (A8R8G8B8)"}, {DXGI_FORMAT_B8G8R8X8_UNORM, "B8G8R8X8 (X8R8G8B8)"},
    {DXGI_FORMAT_B5G6R5_UNORM, "B5G6R5 (R5G6B5)"}, {DXGI_FORMAT_B5G5R5A1_UNORM, "B5G5R5A1 (A1R5G5B5)"},
    {DXGI_FORMAT_B4G4R4A4_UNORM, "B4G4R4A4 (A4R4G4B4)"}, {DXGI_FORMAT_D24_UNORM_S8_UINT, "D24S8"}, {DXGI_FORMAT_R8_UNORM, "R8 (L8, P8 indices)"}};
  for (unsigned i = 0; i < sizeof fm / sizeof *fm; i++) {
    UINT s = 0; HRESULT hr = ID3D11Device_CheckFormatSupport(g_dev, fm[i].f, &s);
    put("format", "%s: hr 0x%08lX texture2d %u render target %u depth %u sample %u", fm[i].n, (unsigned long)hr,
        !!(s & D3D11_FORMAT_SUPPORT_TEXTURE2D), !!(s & D3D11_FORMAT_SUPPORT_RENDER_TARGET), !!(s & D3D11_FORMAT_SUPPORT_DEPTH_STENCIL),
        !!(s & D3D11_FORMAT_SUPPORT_SHADER_SAMPLE));
  }
  D3D11_FEATURE_DATA_THREADING th; memset(&th, 0, sizeof th);
  ID3D11Device_CheckFeatureSupport(g_dev, D3D11_FEATURE_THREADING, &th, sizeof th);
  put("threading", "concurrent creates %d, command lists %d", th.DriverConcurrentCreates, th.DriverCommandLists);
  D3D11_QUERY_DESC qd = {D3D11_QUERY_OCCLUSION, 0}; ID3D11Query *q = NULL;
  put("occlusion query", "hr 0x%08lX", (unsigned long)ID3D11Device_CreateQuery(g_dev, &qd, &q)); if (q) ID3D11Query_Release(q);
  put("shader compiler", "d3dcompiler_47.dll %s", g_compile ? "loaded" : "not found");
}

/* ---- bench ------------------------------------------------------------------------------------------------------- */
static const char *kHlsl =
  "cbuffer C : register(b0) { float4x4 wvp; float4 col; };\n"
  "struct V { float3 p : POSITION; float4 c : COLOR; float2 t : TEXCOORD; };\n"
  "struct O { float4 p : SV_Position; float4 c : COLOR; float2 t : TEXCOORD; };\n"
  "Texture2D tx : register(t0); SamplerState ss : register(s0);\n"
  "O vs(V v) { O o; o.p = mul(float4(v.p, 1), wvp); o.c = v.c * col; o.t = v.t; return o; }\n"
  "float4 ps(O o) : SV_Target { return tx.Sample(ss, o.t) * o.c; }\n";
typedef struct { float x, y, z; DWORD c; float u, v; } Vtx;
enum { NVB = 64, NTEX = 32, MODEL_IDX = 174, RING_BYTES = 4 << 20 };
static ID3D11Buffer *g_vb[NVB], *g_ib, *g_cb, *g_ring; static ID3D11ShaderResourceView *g_srv[NTEX];
static ID3D11BlendState *g_bs[2]; static ID3D11InputLayout *g_il; static ID3D11VertexShader *g_vs; static ID3D11PixelShader *g_ps;
static ID3D11SamplerState *g_samp; static UINT g_ringPos;

static int bench_setup(void) {
  ID3DBlob *vsb = compile(kHlsl, "vs", "vs_4_0"), *psb = compile(kHlsl, "ps", "ps_4_0");
  if (!vsb || !psb) { put("bench", "skipped: no shader compiler"); return 0; }
  ID3D11Device_CreateVertexShader(g_dev, ID3D10Blob_GetBufferPointer(vsb), ID3D10Blob_GetBufferSize(vsb), NULL, &g_vs);
  ID3D11Device_CreatePixelShader(g_dev, ID3D10Blob_GetBufferPointer(psb), ID3D10Blob_GetBufferSize(psb), NULL, &g_ps);
  D3D11_INPUT_ELEMENT_DESC el[3] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"COLOR", 0, DXGI_FORMAT_B8G8R8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}};
  ID3D11Device_CreateInputLayout(g_dev, el, 3, ID3D10Blob_GetBufferPointer(vsb), ID3D10Blob_GetBufferSize(vsb), &g_il);
  ID3D10Blob_Release(vsb); ID3D10Blob_Release(psb);
  /* model buffers: a strip-like grid of 60 vertices; 174 indices = 58 triangles (the plaza's strips: about 58) */
  Vtx v[60]; for (int i = 0; i < 60; i++) { v[i].x = (i % 2) * 0.02f + (i / 2) * 0.001f - 0.5f; v[i].y = (i / 2) * 0.01f - 0.2f; v[i].z = 0.5f; v[i].c = 0xFFFFFFFF; v[i].u = (float)(i % 2); v[i].v = (float)(i / 2) / 30; }
  D3D11_BUFFER_DESC bd = {sizeof v, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0}; D3D11_SUBRESOURCE_DATA sr = {v, 0, 0};
  for (int i = 0; i < NVB; i++) ID3D11Device_CreateBuffer(g_dev, &bd, &sr, &g_vb[i]);
  WORD ix[MODEL_IDX]; for (int t = 0; t < MODEL_IDX / 3; t++) { ix[3 * t] = (WORD)t; ix[3 * t + 1] = (WORD)(t + 1); ix[3 * t + 2] = (WORD)(t + 2); }
  D3D11_BUFFER_DESC ib = {sizeof ix, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0, 0, 0}; D3D11_SUBRESOURCE_DATA isr = {ix, 0, 0};
  ID3D11Device_CreateBuffer(g_dev, &ib, &isr, &g_ib);
  D3D11_BUFFER_DESC cb = {80, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0};
  ID3D11Device_CreateBuffer(g_dev, &cb, NULL, &g_cb);
  D3D11_BUFFER_DESC rb = {RING_BYTES, D3D11_USAGE_DYNAMIC, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0};
  ID3D11Device_CreateBuffer(g_dev, &rb, NULL, &g_ring);
  DWORD px[64 * 64]; for (int i = 0; i < 64 * 64; i++) px[i] = 0xFF808080u ^ (DWORD)(i * 2654435761u);
  D3D11_TEXTURE2D_DESC td = {64, 64, 1, 1, DXGI_FORMAT_B8G8R8A8_UNORM, {1, 0}, D3D11_USAGE_IMMUTABLE, D3D11_BIND_SHADER_RESOURCE, 0, 0};
  D3D11_SUBRESOURCE_DATA tsr = {px, 64 * 4, 0};
  for (int i = 0; i < NTEX; i++) { ID3D11Texture2D *t = NULL; ID3D11Device_CreateTexture2D(g_dev, &td, &tsr, &t); ID3D11Device_CreateShaderResourceView(g_dev, (ID3D11Resource *)t, NULL, &g_srv[i]); ID3D11Texture2D_Release(t); }
  D3D11_BLEND_DESC bl; memset(&bl, 0, sizeof bl); bl.RenderTarget[0].RenderTargetWriteMask = 0xF;
  ID3D11Device_CreateBlendState(g_dev, &bl, &g_bs[0]);
  bl.RenderTarget[0].BlendEnable = TRUE; bl.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; bl.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  bl.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; bl.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; bl.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; bl.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  ID3D11Device_CreateBlendState(g_dev, &bl, &g_bs[1]);
  D3D11_SAMPLER_DESC sd; memset(&sd, 0, sizeof sd); sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; sd.AddressU = sd.AddressV = sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; sd.MaxLOD = D3D11_FLOAT32_MAX;
  ID3D11Device_CreateSamplerState(g_dev, &sd, &g_samp);
  ID3D11DeviceContext_IASetInputLayout(g_ctx, g_il); ID3D11DeviceContext_IASetPrimitiveTopology(g_ctx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  ID3D11DeviceContext_IASetIndexBuffer(g_ctx, g_ib, DXGI_FORMAT_R16_UINT, 0);
  ID3D11DeviceContext_VSSetShader(g_ctx, g_vs, NULL, 0); ID3D11DeviceContext_PSSetShader(g_ctx, g_ps, NULL, 0);
  ID3D11DeviceContext_VSSetConstantBuffers(g_ctx, 0, 1, &g_cb); ID3D11DeviceContext_PSSetSamplers(g_ctx, 0, 1, &g_samp);
  UINT st = sizeof(Vtx), of = 0; ID3D11DeviceContext_IASetVertexBuffers(g_ctx, 0, 1, &g_vb[0], &st, &of);
  ID3D11DeviceContext_PSSetShaderResources(g_ctx, 0, 1, &g_srv[0]);
  return 1;
}
static void set_cb(int i) {
  D3D11_MAPPED_SUBRESOURCE m;
  if (SUCCEEDED(ID3D11DeviceContext_Map(g_ctx, (ID3D11Resource *)g_cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &m))) {
    float *f = (float *)m.pData; memset(f, 0, 80); f[0] = f[5] = f[10] = f[15] = 1; f[12] = (i % 40) * 0.025f - 0.5f; f[13] = (i / 40 % 40) * 0.02f - 0.4f;
    f[16] = f[17] = f[18] = f[19] = 1; ID3D11DeviceContext_Unmap(g_ctx, (ID3D11Resource *)g_cb, 0);
  }
}
static void ring_draw(int verts) {          /* the up_ring pattern: NOOVERWRITE at the cursor, DISCARD on wrap */
  UINT bytes = verts * sizeof(Vtx); D3D11_MAP mt = D3D11_MAP_WRITE_NO_OVERWRITE;
  if (g_ringPos + bytes > RING_BYTES) { g_ringPos = 0; mt = D3D11_MAP_WRITE_DISCARD; }
  D3D11_MAPPED_SUBRESOURCE m;
  if (FAILED(ID3D11DeviceContext_Map(g_ctx, (ID3D11Resource *)g_ring, 0, mt, 0, &m))) return;
  Vtx *v = (Vtx *)((BYTE *)m.pData + g_ringPos);
  for (int i = 0; i < verts; i++) { v[i].x = 0.6f + (i % 3) * 0.01f; v[i].y = 0.6f + (i / 3) * 0.001f; v[i].z = 0.1f; v[i].c = 0xFFFFFFFF; v[i].u = v[i].v = 0; }
  ID3D11DeviceContext_Unmap(g_ctx, (ID3D11Resource *)g_ring, 0);
  UINT st = sizeof(Vtx), of = 0; ID3D11DeviceContext_IASetVertexBuffers(g_ctx, 0, 1, &g_ring, &st, &of);
  ID3D11DeviceContext_Draw(g_ctx, verts, g_ringPos / sizeof(Vtx));
  g_ringPos += bytes;
}
static void gpu_wait(void) {                 /* the frame's GPU work finished: an event query, spun on */
  D3D11_QUERY_DESC qd = {D3D11_QUERY_EVENT, 0}; ID3D11Query *q = NULL;
  if (FAILED(ID3D11Device_CreateQuery(g_dev, &qd, &q))) return;
  ID3D11DeviceContext_End(g_ctx, (ID3D11Asynchronous *)q);
  BOOL done = FALSE; double t0 = now_us();
  while (ID3D11DeviceContext_GetData(g_ctx, (ID3D11Asynchronous *)q, &done, sizeof done, 0) == S_FALSE && now_us() - t0 < 2e6) YieldProcessor();
  ID3D11Query_Release(q);
}
/* one micro-benchmark: K calls of one kind inside a frame, 7 frames, the median ns per call */
static void micro(const char *name, int kind) {
  enum { K = 200000, FR = 7 }; double ns[FR]; UINT st = sizeof(Vtx), of = 0;
  for (int f = 0; f < FR; f++) {
    float c[4] = {0.1f, 0.1f, 0.2f, 1}; ID3D11DeviceContext_ClearRenderTargetView(g_ctx, g_rtv, c);
    double t0 = now_us();
    for (int i = 0; i < K; i++) switch (kind) {
      case 0: ID3D11DeviceContext_OMSetBlendState(g_ctx, g_bs[i & 1], NULL, 0xFFFFFFFF); break;
      case 1: ID3D11DeviceContext_PSSetShaderResources(g_ctx, 0, 1, &g_srv[i & 1]); break;
      case 2: ID3D11DeviceContext_IASetVertexBuffers(g_ctx, 0, 1, &g_vb[i & 1], &st, &of); break;
      case 3: set_cb(i); break;
      case 4: ID3D11DeviceContext_DrawIndexed(g_ctx, 6, 0, 0); break;
      case 5: ring_draw(6); break;
    }
    double t1 = now_us();
    IDXGISwapChain_Present(g_sc, 0, 0); gpu_wait(); pump();
    ns[f] = (t1 - t0) * 1000.0 / K;
  }
  qsort(ns, FR, sizeof ns[0], cmp_d);
  put(name, "%.1f ns a call (median of %d frames of %d calls)", ns[FR / 2], FR, K);
  ID3D11DeviceContext_OMSetBlendState(g_ctx, g_bs[0], NULL, 0xFFFFFFFF);
  ID3D11DeviceContext_IASetVertexBuffers(g_ctx, 0, 1, &g_vb[0], &st, &of);
}
/* the plaza frame: 1,600 character draws (a vertex buffer, a constant buffer, a texture every third), 900 UI draws
   of 2 triangles from the ring, 500 blend changes. `merged`: the same triangles in 32 + 1 draws. */
static void frame_bench(const char *name, int merged) {
  enum { FR = 240 }; static double cpu[FR], tot[FR]; UINT st = sizeof(Vtx), of = 0;
  for (int f = 0; f < FR; f++) {
    double t0 = now_us();
    float c[4] = {0.1f, 0.1f, 0.2f, 1}; ID3D11DeviceContext_ClearRenderTargetView(g_ctx, g_rtv, c);
    if (!merged) {
      for (int i = 0; i < 1600; i++) {
        ID3D11DeviceContext_IASetVertexBuffers(g_ctx, 0, 1, &g_vb[i % NVB], &st, &of);
        set_cb(i);
        if (i % 3 == 0) ID3D11DeviceContext_PSSetShaderResources(g_ctx, 0, 1, &g_srv[i % NTEX]);
        if (i % 4 == 0) ID3D11DeviceContext_OMSetBlendState(g_ctx, g_bs[(i / 4) & 1], NULL, 0xFFFFFFFF);
        ID3D11DeviceContext_DrawIndexed(g_ctx, MODEL_IDX, 0, 0);
      }
      ID3D11DeviceContext_OMSetBlendState(g_ctx, g_bs[1], NULL, 0xFFFFFFFF);
      for (int i = 0; i < 900; i++) ring_draw(6);
    } else {
      for (int i = 0; i < 32; i++) {        /* 1,600 / 32 = 50 instances' worth of indices per draw, as one list */
        ID3D11DeviceContext_IASetVertexBuffers(g_ctx, 0, 1, &g_vb[i % NVB], &st, &of);
        set_cb(i); ID3D11DeviceContext_PSSetShaderResources(g_ctx, 0, 1, &g_srv[i % NTEX]);
        ID3D11DeviceContext_DrawIndexedInstanced(g_ctx, MODEL_IDX, 50, 0, 0, 0);
      }
      ID3D11DeviceContext_OMSetBlendState(g_ctx, g_bs[1], NULL, 0xFFFFFFFF);
      ring_draw(900 * 6);
    }
    ID3D11DeviceContext_OMSetBlendState(g_ctx, g_bs[0], NULL, 0xFFFFFFFF);
    double t1 = now_us();
    IDXGISwapChain_Present(g_sc, 0, 0); gpu_wait();
    double t2 = now_us(); pump();
    cpu[f] = (t1 - t0) / 1000.0; tot[f] = (t2 - t0) / 1000.0;
  }
  double ms = 0, mt = 0; for (int k = 20; k < FR; k++) { ms += cpu[k]; mt += tot[k]; }   /* means: exact whatever the clock's step */
  ms /= FR - 20; mt /= FR - 20;
  qsort(cpu + 20, FR - 20, sizeof cpu[0], cmp_d); qsort(tot + 20, FR - 20, sizeof tot[0], cmp_d);   /* the first 20 left out */
  int n = FR - 20;
  put(name, "submit %.2f ms (p95 %.2f), with Present and the GPU %.2f ms (p95 %.2f); median of %d frames",
      cpu[20 + n / 2], cpu[20 + n * 95 / 100], tot[20 + n / 2], tot[20 + n * 95 / 100], n, ms, mt);
}

/* ---- two processes ------------------------------------------------------------------------------------------- */
static DWORD read_center(HWND hw) {          /* the window's pixels as composed (PrintWindow, PW_RENDERFULLCONTENT) */
  RECT r; GetClientRect(hw, &r); HDC wdc = GetDC(hw), mdc = CreateCompatibleDC(wdc);
  HBITMAP bm = CreateCompatibleBitmap(wdc, r.right, r.bottom); HGDIOBJ old = SelectObject(mdc, bm);
  PrintWindow(hw, mdc, 2 /* PW_RENDERFULLCONTENT */ | 1 /* PW_CLIENTONLY */);
  COLORREF c = GetPixel(mdc, r.right / 2, r.bottom / 2);
  SelectObject(mdc, old); DeleteObject(bm); DeleteDC(mdc); ReleaseDC(hw, wdc);
  return (DWORD)c;
}
static int near_color(DWORD c, int r, int g, int b) { int dr = (int)GetRValue(c) - r, dg = (int)GetGValue(c) - g, db = (int)GetBValue(c) - b; return dr * dr + dg * dg + db * db < 30 * 30; }
static void xproc(const char *child) {
  HWND hw = make_window("probe11 xproc", 320, 240); pump();
  if (!make_device(hw, 320, 240)) return;
  /* the shared texture this process creates and the child writes: the legacy shared handle (D3D11_RESOURCE_MISC_SHARED) */
  D3D11_TEXTURE2D_DESC td = {64, 64, 1, 1, DXGI_FORMAT_B8G8R8A8_UNORM, {1, 0}, D3D11_USAGE_DEFAULT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, 0, D3D11_RESOURCE_MISC_SHARED};
  ID3D11Texture2D *sh = NULL; HANDLE h = NULL; IDXGIResource *dr = NULL;
  HRESULT hr = ID3D11Device_CreateTexture2D(g_dev, &td, NULL, &sh);
  if (SUCCEEDED(hr)) hr = ID3D11Texture2D_QueryInterface(sh, &IID_IDXGIResource, (void **)&dr);
  if (SUCCEEDED(hr)) hr = IDXGIResource_GetSharedHandle(dr, &h);
  put("xproc.shared texture", "created here: hr 0x%08lX handle %p", (unsigned long)hr, (void *)h);
  float c0[4] = {0, 0, 0, 1}; ID3D11DeviceContext_ClearRenderTargetView(g_ctx, g_rtv, c0); IDXGISwapChain_Present(g_sc, 0, 0); pump();
  /* (1) the child presents green into this window */
  char cmd[1024]; snprintf(cmd, sizeof cmd, "\"%s\" xchild %llu %llu \"%s\"", child, (unsigned long long)(uintptr_t)hw, (unsigned long long)(uintptr_t)h, g_outdir);
  STARTUPINFOA si; PROCESS_INFORMATION pi; memset(&si, 0, sizeof si); si.cb = sizeof si;
  if (!CreateProcessA(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) { put("xproc.child", "not started (%lu): %s", GetLastError(), child); return; }
  DWORD px = 0; int seen = 0;               /* sampled while the child runs: its frames exist only while it does */
  for (int i = 0; i < 600 && WaitForSingleObject(pi.hProcess, 20) == WAIT_TIMEOUT; i++) {
    pump(); DWORD p = read_center(hw); if (near_color(p, 0, 255, 0)) { seen = 1; px = p; } else if (!seen) px = p;
  }
  DWORD code = 0; GetExitCodeProcess(pi.hProcess, &code);
  put("xproc.child exit", "%lu (bit 0: its swap chain on this window was made and presented; bit 1: it wrote the shared texture)", (unsigned long)code);
  put("xproc.child present into this window", "%s (center pixel %06lX; the child cleared to green)", seen ? "SEEN" : "not seen", (unsigned long)px);
  /* (2) this process draws the shared texture (the child cleared it to blue) onto its own back buffer */
  if (!h) put("xproc.shared texture written by the child", "not tried (no shared texture could be made in this process)");
  else if (!(code & 2)) put("xproc.shared texture written by the child", "not tried by the child (see its log)");
  else if (sh) {
    ID3D11Texture2D *bb = NULL; IDXGISwapChain_GetBuffer(g_sc, 0, &IID_ID3D11Texture2D, (void **)&bb);
    D3D11_BOX box = {0, 0, 0, 64, 64, 1};
    ID3D11DeviceContext_CopySubresourceRegion(g_ctx, (ID3D11Resource *)bb, 0, 128, 88, 0, (ID3D11Resource *)sh, 0, &box);
    ID3D11Texture2D_Release(bb); IDXGISwapChain_Present(g_sc, 0, 0); pump(); Sleep(200); pump();
    DWORD p2 = read_center(hw);
    put("xproc.shared texture written by the child", "%s (center pixel %06lX; the child cleared it to blue)", near_color(p2, 0, 0, 255) ? "SEEN" : "not seen", (unsigned long)p2);
  }
  CloseHandle(pi.hProcess); CloseHandle(pi.hThread);
}
static int xchild(HWND hw, HANDLE h) {
  int code = 0; ID3D11Device *d = NULL; ID3D11DeviceContext *c = NULL; IDXGISwapChain *sc = NULL;
  RECT r; GetClientRect(hw, &r);
  DXGI_SWAP_CHAIN_DESC sd; memset(&sd, 0, sizeof sd);
  sd.BufferDesc.Width = r.right; sd.BufferDesc.Height = r.bottom; sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; sd.SampleDesc.Count = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; sd.BufferCount = 1; sd.OutputWindow = hw; sd.Windowed = TRUE; sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &sd, &sc, &d, NULL, &c);
  put("xchild.swap chain on the parent's window", "hr 0x%08lX", (unsigned long)hr);
  if (SUCCEEDED(hr)) {
    ID3D11Texture2D *bb = NULL; ID3D11RenderTargetView *rtv = NULL;
    IDXGISwapChain_GetBuffer(sc, 0, &IID_ID3D11Texture2D, (void **)&bb);
    ID3D11Device_CreateRenderTargetView(d, (ID3D11Resource *)bb, NULL, &rtv);
    float g[4] = {0, 1, 0, 1}; HRESULT ph = S_OK;
    for (int i = 0; i < 30; i++) { ID3D11DeviceContext_ClearRenderTargetView(c, rtv, g); ph = IDXGISwapChain_Present(sc, 0, 0); Sleep(10); }
    put("xchild.present", "hr 0x%08lX", (unsigned long)ph);
    if (SUCCEEDED(ph)) code |= 1;
    ID3D11RenderTargetView_Release(rtv); ID3D11Texture2D_Release(bb);
  } else {
    hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &d, NULL, &c);
    put("xchild.device", "hr 0x%08lX", (unsigned long)hr);
  }
  if (d && h) {
    ID3D11Texture2D *t = NULL; ID3D11RenderTargetView *rtv = NULL;
    hr = ID3D11Device_OpenSharedResource(d, h, &IID_ID3D11Texture2D, (void **)&t);
    put("xchild.open shared texture", "hr 0x%08lX", (unsigned long)hr);
    if (SUCCEEDED(hr) && SUCCEEDED(ID3D11Device_CreateRenderTargetView(d, (ID3D11Resource *)t, NULL, &rtv))) {
      float b[4] = {0, 0, 1, 1}; ID3D11DeviceContext_ClearRenderTargetView(c, rtv, b); ID3D11DeviceContext_Flush(c); Sleep(100);
      code |= 2; ID3D11RenderTargetView_Release(rtv);
    }
    if (t) ID3D11Texture2D_Release(t);
  }
  Sleep(2000);                               /* the parent samples the window while this one still exists */
  return code;
}

int main(int argc, char **argv) {
  LARGE_INTEGER f; QueryPerformanceFrequency(&f); g_qpf = (double)f.QuadPart;
  if (argc < 3) { printf("usage: probe11 caps|bench|xproc OUTDIR [CHILD.exe] | xchild HWND HANDLE OUTDIR\n"); return 2; }
  HMODULE dc = LoadLibraryA("d3dcompiler_47.dll"); if (dc) g_compile = (PFN_Compile)(void *)GetProcAddress(dc, "D3DCompile");
  char path[MAX_PATH];
  if (!strcmp(argv[1], "xchild")) {
    snprintf(path, sizeof path, "%s\\probe11_xchild_%s.csv", argc > 4 ? argv[4] : ".", ARCH);
    g_out = fopen(path, "w"); if (!g_out) g_out = stdout;
    process_info();
    return xchild((HWND)(uintptr_t)_strtoui64(argv[2], NULL, 10), argc > 3 ? (HANDLE)(uintptr_t)_strtoui64(argv[3], NULL, 10) : NULL);
  }
  CreateDirectoryA(argv[2], NULL); g_outdir = argv[2];
  snprintf(path, sizeof path, "%s\\probe11_%s%s.csv", argv[2], !strcmp(argv[1], "xproc") ? "xproc_" : "", ARCH);
  g_out = fopen(path, !strcmp(argv[1], "xproc") ? "w" : "a"); if (!g_out) { printf("cannot write %s\n", path); return 2; }
  process_info(); modules_info();
  put("timer", "QueryPerformanceFrequency %.0f Hz; smallest step seen %.3f us", g_qpf, timer_step_us());
  if (!strcmp(argv[1], "xproc")) { if (argc < 4) { put("xproc", "no child given"); return 2; } xproc(argv[3]); return 0; }
  HWND hw = make_window("probe11 (" ARCH ")", 1280, 720); pump();
  if (!make_device(hw, 1280, 720)) return 1;
  if (!strcmp(argv[1], "caps")) { caps(); return 0; }
  if (!strcmp(argv[1], "bench")) {
    if (!bench_setup()) return 1;
    micro("call.OMSetBlendState", 0); micro("call.PSSetShaderResources", 1); micro("call.IASetVertexBuffers", 2);
    micro("call.Map constant buffer (DISCARD)", 3); micro("call.DrawIndexed (2 triangles)", 4); micro("call.ring Map NOOVERWRITE + Draw", 5);
    frame_bench("frame.plaza-like as D3D8 issues it (1600+900 draws)", 0);
    frame_bench("frame.plaza-like merged (33 draws)", 1);
    return 0;
  }
  printf("unknown command %s\n", argv[1]); return 2;
}
