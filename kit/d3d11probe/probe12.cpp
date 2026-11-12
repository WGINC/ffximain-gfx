/* probe12.cpp -- (12 Nov) can the work of a frame's draws be spread over threads on the owner's machine?
   Built twice from this file (build.sh): probe12_x86.exe (the game's architecture, emulated on the owner's Mac) and
   probe12_arm64.exe (native). Run outside the game: probe12 OUTDIR

   (The order run: D3D12 first, then D3D11. Before each timed test a `starting` line names it.)
   1. D3D12: is there a device (D3D12CreateDevice on adapter 0), its feature level, and does a command list run.
   2. One frame of N small draws (N = 2,000, the 19 Nov crowd windows had 950-1,700), each changing four constants
      and drawing one triangle into a 256x256 target, made in four ways and timed to the GPU's finish:
        d3d11_immediate      on the immediate context, one thread (what d3d8on11's executor does);
        d3d11_deferred_T     on T deferred contexts, one thread each, then executed on the immediate context;
        d3d12_T              on T D3D12 command lists, one thread each, then one ExecuteCommandLists;
      T = 1, 2, 4, 8. Each is 40 frames, the first 8 left out; the median of: the recording (wall time until every
      thread is done), the submission, the wait for the GPU, and the total.
   This is a model of the executor's work, not the game's frame: one constant change and one draw each, no textures or
   state changes. What it settles is whether more threads shorten such a frame on this machine, and by how much.

   Every result is one line `key,value` in OUTDIR\probe12_<arch>.csv. A step that fails writes its HRESULT and the
   run goes on with the next. */
#define WIN32_LEAN_AND_MEAN
// methods that return a struct (GetCPUDescriptorHandleForHeapStart) return it through a hidden pointer in the COM ABI;
// with this, MinGW's headers declare them so and wrap them (without it GCC returns the struct in registers on x86)
#define WIDL_EXPLICIT_AGGREGATE_RETURNS
#include <windows.h>
#include <d3d11.h>
#include <d3d12.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <vector>

#if defined(__aarch64__) || defined(_M_ARM64)
#define ARCH "arm64"
#elif defined(__x86_64__) || defined(_M_X64)
#define ARCH "x64"
#else
#define ARCH "x86"
#endif

static FILE *g_out;
static void put(const char *k, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
static void put(const char *k, const char *fmt, ...) {
  va_list a; va_start(a, fmt);
  if (g_out) { fprintf(g_out, "%s,", k); vfprintf(g_out, fmt, a); fputc('\n', g_out); fflush(g_out); }
  va_end(a);
  va_start(a, fmt); printf("%-34s ", k); vprintf(fmt, a); putchar('\n'); fflush(stdout); va_end(a);
}
static double now_ms() {
  static LARGE_INTEGER f; if (!f.QuadPart) QueryPerformanceFrequency(&f);
  LARGE_INTEGER t; QueryPerformanceCounter(&t); return (double)t.QuadPart * 1000.0 / (double)f.QuadPart;
}
template <class T> static void rel(T *&p) { if (p) { p->Release(); p = nullptr; } }

enum { N = 2000, FRAMES = 40, SKIP = 8, MAXT = 8, W = 256 };
static const int THREADS[] = {1, 2, 4, 8};

// ---- a small pool: MAXT workers, each runs job(i) when started -----------------------------------------------------
struct Pool {
  HANDLE go[MAXT] = {}, done[MAXT] = {}, th[MAXT] = {};
  void (*job)(void *, int) = nullptr; void *arg = nullptr; volatile LONG quit = 0;
  struct Slot { Pool *p; int i; } slot[MAXT];
  static DWORD WINAPI main_(LPVOID v) {
    Slot *s = (Slot *)v;
    for (;;) { WaitForSingleObject(s->p->go[s->i], INFINITE); if (s->p->quit) return 0; s->p->job(s->p->arg, s->i); SetEvent(s->p->done[s->i]); }
  }
  void start() { for (int i = 0; i < MAXT; i++) { go[i] = CreateEventA(0, FALSE, FALSE, 0); done[i] = CreateEventA(0, FALSE, FALSE, 0);
                 slot[i] = {this, i}; th[i] = CreateThread(0, 0, main_, &slot[i], 0, 0); } }
  void run(int t, void (*j)(void *, int), void *a) { job = j; arg = a; for (int i = 0; i < t; i++) SetEvent(go[i]); WaitForMultipleObjects(t, done, TRUE, INFINITE); }
  void stop() { quit = 1; for (int i = 0; i < MAXT; i++) SetEvent(go[i]); WaitForMultipleObjects(MAXT, th, TRUE, 5000); }
};
static Pool g_pool;

static double median(std::vector<double> v) { if (v.empty()) return 0; std::sort(v.begin(), v.end()); return v[v.size() / 2]; }
struct Times { std::vector<double> rec, sub, wait, tot; };
static void report(const char *name, Times &t) {
  put(name, "record %.3f ms, submit %.3f ms, GPU wait %.3f ms, total %.3f ms (median of %u frames, %d draws)",
      median(t.rec), median(t.sub), median(t.wait), median(t.tot), (unsigned)t.tot.size(), (int)N);
}

// ---- shaders: a triangle placed by its constants (no vertex buffer: SV_VertexID) -----------------------------------
static const char *HLSL =
  "cbuffer C : register(b0) { float4 k; };\n"
  "float4 vs(uint i : SV_VertexID) : SV_Position { float2 p = float2(i == 1 ? 0.02 : 0.0, i == 2 ? 0.02 : 0.0); return float4(p + k.xy, 0, 1); }\n"
  "float4 ps() : SV_Target { return float4(k.zw, 0.5, 1); }\n";
typedef HRESULT (WINAPI *PFN_Compile)(LPCVOID, SIZE_T, LPCSTR, const D3D_SHADER_MACRO *, ID3DInclude *, LPCSTR, LPCSTR, UINT, UINT, ID3DBlob **, ID3DBlob **);
static PFN_Compile g_compile;
static ID3DBlob *compile(const char *entry, const char *target) {
  ID3DBlob *b = nullptr, *e = nullptr;
  HRESULT h = g_compile ? g_compile(HLSL, strlen(HLSL), "probe12", 0, 0, entry, target, 0, 0, &b, &e) : E_FAIL;
  if (FAILED(h)) { put("shader", "%s %s: 0x%08lX %s", entry, target, (unsigned long)h, e ? (const char *)e->GetBufferPointer() : ""); rel(e); return nullptr; }
  rel(e); return b;
}
static void constants(float *k, int i) { k[0] = (float)(i % 40) / 20.0f - 1.0f; k[1] = (float)(i / 40 % 40) / 20.0f - 1.0f; k[2] = (float)(i & 255) / 255.0f; k[3] = 0.5f; }

// ---- D3D11 -----------------------------------------------------------------------------------------------------------
struct D11 {
  ID3D11Device *dev = nullptr; ID3D11DeviceContext *imm = nullptr; ID3D11RenderTargetView *rtv = nullptr;
  ID3D11VertexShader *vs = nullptr; ID3D11PixelShader *ps = nullptr; ID3D11Buffer *cb[MAXT] = {};
  ID3D11DeviceContext *def[MAXT] = {}; ID3D11CommandList *cl[MAXT] = {}; ID3D11Query *ev = nullptr; int t = 1;
  void state(ID3D11DeviceContext *c, int i) {
    D3D11_VIEWPORT vp = {0, 0, (float)W, (float)W, 0, 1};
    c->OMSetRenderTargets(1, &rtv, nullptr); c->RSSetViewports(1, &vp); c->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    c->IASetInputLayout(nullptr); c->VSSetShader(vs, 0, 0); c->PSSetShader(ps, 0, 0); c->VSSetConstantBuffers(0, 1, &cb[i]); c->PSSetConstantBuffers(0, 1, &cb[i]);
  }
  void draws(ID3D11DeviceContext *c, int i, int from, int to) {
    for (int d = from; d < to; d++) {
      D3D11_MAPPED_SUBRESOURCE m; if (FAILED(c->Map(cb[i], 0, D3D11_MAP_WRITE_DISCARD, 0, &m))) return;
      constants((float *)m.pData, d); c->Unmap(cb[i], 0); c->Draw(3, 0);
    }
  }
  // PROBE12_SERIAL=1: the threads record one at a time (test_probe.py under Wine, whose deferred contexts crashed in 2 of
  // 9 runs when two threads recorded at once, each on its own context and buffer, which D3D11 allows; 0 of 10 one at
  // a time). The output says when this is on: its numbers are not threaded ones
  static bool serial; static CRITICAL_SECTION cs;
  static void job(void *a, int i) {
    if (serial) EnterCriticalSection(&cs);
    D11 *x = (D11 *)a; int per = N / x->t;
    x->state(x->def[i], i); x->draws(x->def[i], i, i * per, i == x->t - 1 ? N : (i + 1) * per);
    x->def[i]->FinishCommandList(FALSE, &x->cl[i]);
    if (serial) LeaveCriticalSection(&cs);
  }
  double wait_gpu() { double t0 = now_ms(); imm->End(ev); while (imm->GetData(ev, nullptr, 0, 0) == S_FALSE) YieldProcessor(); return now_ms() - t0; }
};

bool D11::serial = false; CRITICAL_SECTION D11::cs;

static void run_d3d11(IDXGIAdapter *ad) {
  D11 x; D3D_FEATURE_LEVEL fl = (D3D_FEATURE_LEVEL)0;
  D3D_FEATURE_LEVEL want[] = {D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0};
  HRESULT h = D3D11CreateDevice(ad, ad ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE, 0, 0, want, 3, D3D11_SDK_VERSION, &x.dev, &fl, &x.imm);
  if (FAILED(h)) { put("d3d11_device", "0x%08lX", (unsigned long)h); return; }
  put("d3d11_device", "feature level 0x%X", (unsigned)fl);
  D3D11_FEATURE_DATA_THREADING th = {};
  if (SUCCEEDED(x.dev->CheckFeatureSupport(D3D11_FEATURE_THREADING, &th, sizeof th)))
    put("d3d11_threading", "driver concurrent creates %d, driver command lists %d (0: the runtime emulates them)", (int)th.DriverConcurrentCreates, (int)th.DriverCommandLists);
  ID3DBlob *vb = compile("vs", "vs_4_0"), *pb = compile("ps", "ps_4_0");
  if (!vb || !pb) { rel(vb); rel(pb); rel(x.imm); rel(x.dev); return; }
  x.dev->CreateVertexShader(vb->GetBufferPointer(), vb->GetBufferSize(), 0, &x.vs); x.dev->CreatePixelShader(pb->GetBufferPointer(), pb->GetBufferSize(), 0, &x.ps); rel(vb); rel(pb);
  D3D11_TEXTURE2D_DESC td = {}; td.Width = td.Height = W; td.MipLevels = td.ArraySize = 1; td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  td.SampleDesc.Count = 1; td.Usage = D3D11_USAGE_DEFAULT; td.BindFlags = D3D11_BIND_RENDER_TARGET;
  ID3D11Texture2D *rt = nullptr; x.dev->CreateTexture2D(&td, 0, &rt); if (rt) x.dev->CreateRenderTargetView(rt, 0, &x.rtv); rel(rt);
  D3D11_BUFFER_DESC bd = {}; bd.ByteWidth = 16; bd.Usage = D3D11_USAGE_DYNAMIC; bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  for (int i = 0; i < MAXT; i++) x.dev->CreateBuffer(&bd, 0, &x.cb[i]);
  D3D11_QUERY_DESC qd = {D3D11_QUERY_EVENT, 0}; x.dev->CreateQuery(&qd, &x.ev);
  if (!x.rtv || !x.cb[MAXT - 1] || !x.ev || !x.vs || !x.ps) { put("d3d11_setup", "failed"); goto out; }
  put("starting", "d3d11_immediate (if this is the last line, it crashed or hung here)");
  { Times t;                                                  // the immediate context, one thread
    for (int f = 0; f < FRAMES; f++) {
      double t0 = now_ms(); x.state(x.imm, 0); x.draws(x.imm, 0, 0, N); double t1 = now_ms(); x.imm->Flush(); double t2 = now_ms();
      double w = x.wait_gpu();
      if (f >= SKIP) { t.rec.push_back(t1 - t0); t.sub.push_back(t2 - t1); t.wait.push_back(w); t.tot.push_back(t2 - t0 + w); }
    }
    report("d3d11_immediate", t); }
  for (int i = 0; i < MAXT; i++) if (FAILED(h = x.dev->CreateDeferredContext(0, &x.def[i]))) { put("d3d11_deferred", "CreateDeferredContext 0x%08lX", (unsigned long)h); goto out; }
  { char v[8] = {}; GetEnvironmentVariableA("PROBE12_SERIAL", v, sizeof v); D11::serial = v[0] == '1'; InitializeCriticalSection(&D11::cs);
    if (D11::serial) put("d3d11_deferred_serial", "PROBE12_SERIAL=1: the deferred contexts record one thread at a time (not threaded numbers)"); }
  for (int T : THREADS) {                                     // deferred contexts on T threads
    Times t; x.t = T; char name[32]; snprintf(name, sizeof name, "d3d11_deferred_%d", T);
    put("starting", "%s (if this is the last line, it crashed or hung here)", name);
    for (int f = 0; f < FRAMES; f++) {
      double t0 = now_ms(); g_pool.run(T, D11::job, &x); double t1 = now_ms();
      for (int i = 0; i < T; i++) if (x.cl[i]) { x.imm->ExecuteCommandList(x.cl[i], FALSE); rel(x.cl[i]); }
      x.imm->Flush(); double t2 = now_ms(); double w = x.wait_gpu();
      if (f >= SKIP) { t.rec.push_back(t1 - t0); t.sub.push_back(t2 - t1); t.wait.push_back(w); t.tot.push_back(t2 - t0 + w); }
    }
    report(name, t);
  }
out:
  for (int i = 0; i < MAXT; i++) { rel(x.def[i]); rel(x.cb[i]); }
  rel(x.ev); rel(x.rtv); rel(x.vs); rel(x.ps); rel(x.imm); rel(x.dev);
}

// ---- D3D12 -----------------------------------------------------------------------------------------------------------
typedef HRESULT (WINAPI *PFN_Serialize)(const D3D12_ROOT_SIGNATURE_DESC *, D3D_ROOT_SIGNATURE_VERSION, ID3DBlob **, ID3DBlob **);
struct D12 {
  ID3D12Device *dev = nullptr; ID3D12CommandQueue *q = nullptr; ID3D12RootSignature *rs = nullptr; ID3D12PipelineState *pso = nullptr;
  ID3D12Resource *rt = nullptr; ID3D12DescriptorHeap *heap = nullptr; D3D12_CPU_DESCRIPTOR_HANDLE rtv = {};
  ID3D12CommandAllocator *al[MAXT] = {}; ID3D12GraphicsCommandList *cl[MAXT] = {}; ID3D12Fence *fence = nullptr; HANDLE ev = nullptr;
  UINT64 fv = 0; int t = 1;
  static void job(void *a, int i) {
    D12 *x = (D12 *)a; int per = N / x->t, from = i * per, to = i == x->t - 1 ? N : (i + 1) * per;
    ID3D12GraphicsCommandList *c = x->cl[i];
    x->al[i]->Reset(); c->Reset(x->al[i], x->pso);
    D3D12_VIEWPORT vp = {0, 0, (float)W, (float)W, 0, 1}; D3D12_RECT sc = {0, 0, W, W};
    c->SetGraphicsRootSignature(x->rs); c->OMSetRenderTargets(1, &x->rtv, FALSE, nullptr); c->RSSetViewports(1, &vp); c->RSSetScissorRects(1, &sc);
    c->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    for (int d = from; d < to; d++) { float k[4]; constants(k, d); c->SetGraphicsRoot32BitConstants(0, 4, k, 0); c->DrawInstanced(3, 1, 0, 0); }
    c->Close();
  }
  double wait_gpu() {
    double t0 = now_ms(); q->Signal(fence, ++fv);
    if (fence->GetCompletedValue() < fv) { fence->SetEventOnCompletion(fv, ev); if (WaitForSingleObject(ev, 5000) != WAIT_OBJECT_0) return -1; }
    return now_ms() - t0;
  }
};

static void run_d3d12(IDXGIAdapter *ad) {
  HMODULE m = LoadLibraryA("d3d12.dll");
  if (!m) { put("d3d12_dll", "not loaded (error %lu)", (unsigned long)GetLastError()); return; }
  PFN_D3D12_CREATE_DEVICE create = (PFN_D3D12_CREATE_DEVICE)(void *)GetProcAddress(m, "D3D12CreateDevice");
  PFN_Serialize serialize = (PFN_Serialize)(void *)GetProcAddress(m, "D3D12SerializeRootSignature");
  if (!create || !serialize) { put("d3d12_dll", "no D3D12CreateDevice / D3D12SerializeRootSignature"); return; }
  D12 x; double t0 = now_ms();
  HRESULT h = create(ad, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void **)&x.dev);
  if (FAILED(h) || !x.dev) { put("d3d12_device", "D3D12CreateDevice 0x%08lX: no D3D12 device", (unsigned long)h); return; }
  put("d3d12_device", "made (%.1f ms)", now_ms() - t0);
  { D3D_FEATURE_LEVEL want[] = {(D3D_FEATURE_LEVEL)0xc200, (D3D_FEATURE_LEVEL)0xc100, (D3D_FEATURE_LEVEL)0xc000, D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0};
    D3D12_FEATURE_DATA_FEATURE_LEVELS fl = {}; fl.NumFeatureLevels = 5; fl.pFeatureLevelsRequested = want;
    D3D12_FEATURE_DATA_D3D12_OPTIONS o = {};
    HRESULT hf = x.dev->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &fl, sizeof fl), ho = x.dev->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &o, sizeof o);
    put("d3d12_caps", "highest feature level 0x%X (0x%08lX); resource binding tier %d, heap tier %d (0x%08lX)",
        SUCCEEDED(hf) ? (unsigned)fl.MaxSupportedFeatureLevel : 0u, (unsigned long)hf, (int)o.ResourceBindingTier, (int)o.ResourceHeapTier, (unsigned long)ho); }
  const char *step = "";
  do {
    D3D12_COMMAND_QUEUE_DESC qd = {}; qd.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    step = "CreateCommandQueue"; if (FAILED(h = x.dev->CreateCommandQueue(&qd, __uuidof(ID3D12CommandQueue), (void **)&x.q))) break;
    step = "CreateFence"; if (FAILED(h = x.dev->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void **)&x.fence))) break;
    x.ev = CreateEventA(0, FALSE, FALSE, 0);
    D3D12_ROOT_PARAMETER rp = {}; rp.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS; rp.Constants.Num32BitValues = 4; rp.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    D3D12_ROOT_SIGNATURE_DESC rsd = {}; rsd.NumParameters = 1; rsd.pParameters = &rp;
    ID3DBlob *sig = nullptr, *err = nullptr;
    step = "D3D12SerializeRootSignature"; if (FAILED(h = serialize(&rsd, D3D_ROOT_SIGNATURE_VERSION_1, &sig, &err))) { rel(err); break; }
    rel(err);
    h = x.dev->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&x.rs); rel(sig);
    step = "CreateRootSignature"; if (FAILED(h)) break;
    ID3DBlob *vb = compile("vs", "vs_5_0"), *pb = compile("ps", "ps_5_0");
    step = "shaders"; if (!vb || !pb) { rel(vb); rel(pb); h = E_FAIL; break; }
    D3D12_GRAPHICS_PIPELINE_STATE_DESC pd = {};
    pd.pRootSignature = x.rs; pd.VS = {vb->GetBufferPointer(), vb->GetBufferSize()}; pd.PS = {pb->GetBufferPointer(), pb->GetBufferSize()};
    pd.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    pd.BlendState.RenderTarget[0].SrcBlend = pd.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    pd.BlendState.RenderTarget[0].DestBlend = pd.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    pd.BlendState.RenderTarget[0].BlendOp = pd.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    pd.BlendState.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
    pd.SampleMask = 0xFFFFFFFFu; pd.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; pd.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
    pd.RasterizerState.DepthClipEnable = TRUE; pd.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    pd.NumRenderTargets = 1; pd.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; pd.SampleDesc.Count = 1;
    h = x.dev->CreateGraphicsPipelineState(&pd, __uuidof(ID3D12PipelineState), (void **)&x.pso); rel(vb); rel(pb);
    step = "CreateGraphicsPipelineState"; if (FAILED(h)) break;
    D3D12_HEAP_PROPERTIES hp = {}; hp.Type = D3D12_HEAP_TYPE_DEFAULT;
    D3D12_RESOURCE_DESC rd = {}; rd.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; rd.Width = rd.Height = W; rd.DepthOrArraySize = rd.MipLevels = 1;
    rd.Format = DXGI_FORMAT_R8G8B8A8_UNORM; rd.SampleDesc.Count = 1; rd.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    step = "CreateCommittedResource"; if (FAILED(h = x.dev->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_RENDER_TARGET, nullptr, __uuidof(ID3D12Resource), (void **)&x.rt))) break;
    D3D12_DESCRIPTOR_HEAP_DESC hd = {}; hd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; hd.NumDescriptors = 1;
    step = "CreateDescriptorHeap"; if (FAILED(h = x.dev->CreateDescriptorHeap(&hd, __uuidof(ID3D12DescriptorHeap), (void **)&x.heap))) break;
    x.rtv = x.heap->GetCPUDescriptorHandleForHeapStart();   // (the header's wrapper: WIDL_EXPLICIT_AGGREGATE_RETURNS above)
    x.dev->CreateRenderTargetView(x.rt, nullptr, x.rtv);
    for (int i = 0; i < MAXT; i++) {
      step = "CreateCommandAllocator"; if (FAILED(h = x.dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void **)&x.al[i]))) break;
      step = "CreateCommandList"; if (FAILED(h = x.dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, x.al[i], x.pso, __uuidof(ID3D12GraphicsCommandList), (void **)&x.cl[i]))) break;
      x.cl[i]->Close();
    }
    if (FAILED(h)) break;
    step = nullptr;
  } while (0);
  if (step) { put("d3d12_setup", "%s: 0x%08lX (device removed reason 0x%08lX)", step, (unsigned long)h, (unsigned long)x.dev->GetDeviceRemovedReason()); }
  else {
    put("d3d12_setup", "ok");
    for (int T : THREADS) {
      Times t; x.t = T; char name[32]; snprintf(name, sizeof name, "d3d12_%d", T); bool lost = false;
      put("starting", "%s (if this is the last line, it crashed or hung here)", name);
      for (int f = 0; f < FRAMES && !lost; f++) {
        double a = now_ms(); g_pool.run(T, D12::job, &x); double b = now_ms();
        ID3D12CommandList *ls[MAXT]; for (int i = 0; i < T; i++) ls[i] = x.cl[i];
        x.q->ExecuteCommandLists(T, ls); double c = now_ms(); double w = x.wait_gpu();
        if (w < 0) { put(name, "the GPU did not finish a frame in 5 s (device removed reason 0x%08lX)", (unsigned long)x.dev->GetDeviceRemovedReason()); lost = true; break; }
        if (f >= SKIP) { t.rec.push_back(b - a); t.sub.push_back(c - b); t.wait.push_back(w); t.tot.push_back(c - a + w); }
      }
      if (lost) break;
      report(name, t);
    }
  }
  x.wait_gpu();
  for (int i = 0; i < MAXT; i++) { rel(x.cl[i]); rel(x.al[i]); }
  rel(x.heap); rel(x.rt); rel(x.pso); rel(x.rs); rel(x.fence); rel(x.q); rel(x.dev);
  if (x.ev) CloseHandle(x.ev);
}

int main(int argc, char **argv) {
  const char *dir = argc > 1 ? argv[1] : ".";
  char path[MAX_PATH]; snprintf(path, sizeof path, "%s\\probe12_%s.csv", dir, ARCH);
  g_out = fopen(path, "w");
  put("arch", "%s", ARCH);
  SYSTEM_INFO si; GetNativeSystemInfo(&si); put("cpus", "%lu logical processors", (unsigned long)si.dwNumberOfProcessors);
  HMODULE dc = LoadLibraryA("d3dcompiler_47.dll"); g_compile = dc ? (PFN_Compile)(void *)GetProcAddress(dc, "D3DCompile") : nullptr;
  if (!g_compile) put("compiler", "d3dcompiler_47.dll not found: no draws can be timed");
  IDXGIFactory1 *fac = nullptr; IDXGIAdapter1 *ad = nullptr;
  if (SUCCEEDED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void **)&fac))) fac->EnumAdapters1(0, &ad);
  if (ad) { DXGI_ADAPTER_DESC1 a = {}; ad->GetDesc1(&a); char nm[256] = {}; WideCharToMultiByte(CP_UTF8, 0, a.Description, -1, nm, sizeof nm, 0, 0);
            put("adapter", "\"%s\" vendor 0x%04X device 0x%04X", nm, a.VendorId, a.DeviceId); }
  g_pool.start();
  run_d3d12(ad);                                              // first: a crash in the D3D11 part cannot lose its answer
  run_d3d11(ad);
  g_pool.stop();
  if (ad) ad->Release();
  if (fac) fac->Release();
  put("done", "%s", path);
  if (g_out) fclose(g_out);
  return 0;
}
