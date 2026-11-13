// d8_exec12.cpp -- d3d8on11's Direct3D 12 executor (d8_exec12.h says what it is and how it differs from d8_exec.cpp).
// The shaders are the Direct3D 11 executor's (gen_vs, gen_ps in d8_exec.cpp), compiled for shader model 5.0; what each
// record means is d8_proto.h's, and each case below does what d8_exec.cpp's does for it, in Direct3D 12's terms.
//
// Struct returns: MinGW-w64's headers declare COM methods that return a struct (GetCPUDescriptorHandleForHeapStart,
// GetDesc) with the hidden-pointer return of the COM ABI only under WIDL_EXPLICIT_AGGREGATE_RETURNS; without it GCC
// returns them in registers on x86 and reads garbage. Defined before every header here.
#define WIDL_EXPLICIT_AGGREGATE_RETURNS
#include "d8_exec12.h"
#include <d3dcompiler.h>
#include <d3d12sdklayers.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
static ID3D12Resource *readback_buffer12(ID3D12Device *dev, uint64_t bytes);

DXGI_FORMAT vdxgi(uint8_t f);                      // d8_exec.cpp
std::string gen_vs(const VSKey &k);
std::string gen_ps(const PSKey &k);

static double qpc12() {
  static LARGE_INTEGER f; if (!f.QuadPart) QueryPerformanceFrequency(&f);
  LARGE_INTEGER t; QueryPerformanceCounter(&t); return (double)t.QuadPart * 1000.0 / (double)f.QuadPart;
}
template <class T> static void rel(T *&p) { if (p) { p->Release(); p = nullptr; } }
// diagnostic switches (13 Nov; the owner's picture check on Parallels): each turns off one thing that could make a draw
// read the wrong inputs. D8ON11_D3D12_DIAG=list of: sm4 (shader model 4.0 code, as the Direct3D 11 executor's),
// smpdirect (samplers made in the shader heap), nocheck (no sampler check at the start), fresh (a new texture table and
// sampler table for every draw), sync (the CPU waits for the GPU after every draw)
static bool diag(const char *what) {
  static const char *e = getenv("D8ON11_D3D12_DIAG");
  return e && strstr(e, what) != nullptr;
}
static const char *VS_T() { return diag("sm4") ? "vs_4_0" : "vs_5_0"; }
static const char *PS_T() { return diag("sm4") ? "ps_4_0" : "ps_5_0"; }
static uint64_t fnv64(const void *p, size_t n, uint64_t h = 14695981039346656037ull) {
  const uint8_t *b = (const uint8_t *)p; for (size_t i = 0; i < n; i++) { h ^= b[i]; h *= 1099511628211ull; } return h;
}
// a static buffer's read state: vertex, index, and copy source (draw merging copies it into its slots)
static const D3D12_RESOURCE_STATES ST_VBIB = D3D12_RESOURCE_STATE_GENERIC_READ;
enum { RING_MB = 64, GV_CAP = 65536 };

// ---- the bridge to Exec (d8_exec.cpp includes no Direct3D 12 header of ours) -------------------------------------
Exec12 *exec12_new(Exec &o) { return new Exec12(o); }
void exec12_delete(Exec12 *x) { if (x) { x->shutdown(); delete x; } }
void exec12_run(Exec12 *x, uint16_t op, const uint8_t *p, uint32_t n, int32_t *hr, std::vector<uint8_t> *reply) { x->run(op, p, n, hr, reply); }

// ---- descriptors -------------------------------------------------------------------------------------------------------
D3D12_CPU_DESCRIPTOR_HANDLE Exec12::CpuHeap::at(uint32_t i) const {
  D3D12_CPU_DESCRIPTOR_HANDLE h = pages[i / PAGE]->GetCPUDescriptorHandleForHeapStart();
  h.ptr += (SIZE_T)(i % PAGE) * inc; return h;
}
uint32_t Exec12::CpuHeap::take(ID3D12Device *d) {
  if (!free_.empty()) { uint32_t i = free_.back(); free_.pop_back(); return i; }
  if (next == pages.size() * (size_t)PAGE) {
    D3D12_DESCRIPTOR_HEAP_DESC hd = {}; hd.Type = type; hd.NumDescriptors = PAGE;
    ID3D12DescriptorHeap *h = nullptr;
    if (FAILED(d->CreateDescriptorHeap(&hd, __uuidof(ID3D12DescriptorHeap), (void **)&h))) return ~0u;
    pages.push_back(h);
  }
  return next++;
}

// ---- fences, submissions, the ring ---------------------------------------------------------------------------------------
void Exec12::check_lost(const char *where, HRESULT h) {
  if (lost) return;
  HRESULT why = dev ? dev->GetDeviceRemovedReason() : S_OK;
  if (FAILED(h) || FAILED(why)) {
    lost = true;
    d8_log("D3D12: %s: 0x%08lX; the device's removal reason: 0x%08lX; frame %llu, %llu draws so far. Nothing more is drawn "
           "(the waits for the GPU end, so the game does not hang on them)", where, (unsigned long)h, (unsigned long)why,
           (unsigned long long)o.presents, (unsigned long long)o.draws);
  }
}
// until the GPU has passed v; 5 s at a time, with the device's state checked between: a lost device answers no fence,
// and an endless wait here is a hang of the game (docs/HISTORY.md, 12 Nov: the owner's hangs)
void Exec12::wait(uint64_t v) {
  if (lost || !fence || fence->GetCompletedValue() >= v) return;
  if (FAILED(fence->SetEventOnCompletion(v, fev))) { check_lost("SetEventOnCompletion", E_FAIL); return; }
  double t0 = qpc12(); bool said = false;
  while (WaitForSingleObject(fev, 5000) == WAIT_TIMEOUT) {
    check_lost("waiting for the GPU", S_OK);
    if (lost) return;
    if (!said) { d8_log("D3D12: the GPU has not finished a submission in 5 s (fence %llu, at %llu); still waiting", (unsigned long long)v, (unsigned long long)fence->GetCompletedValue()); said = true; }
  }
  if (said) d8_log("D3D12: the GPU finished it after %.0f ms", qpc12() - t0);
}
void Exec12::ring_retire() {
  if (!fence) return;
  uint64_t done = fence->GetCompletedValue();
  while (!ring_marks.empty() && ring_marks.front().fence <= done) { ring_tail = ring_marks.front().head; ring_marks.pop_front(); }
  while (!gv_marks.empty() && gv_marks.front().fence <= done) { gv_tail = gv_marks.front().head; gv_marks.pop_front(); }
}
void Exec12::dig() {
  if (!fence) return;
  uint64_t done = fence->GetCompletedValue();
  for (size_t i = 0; i < graves.size();) {
    if (graves[i].fence <= done || lost) { graves[i].r->Release(); graves[i] = graves.back(); graves.pop_back(); } else i++;
  }
}
void Exec12::begin_list() {
  if (FAILED(cl->Reset(alloc[fi], nullptr))) { check_lost("ID3D12GraphicsCommandList::Reset", E_FAIL); return; }
  ID3D12DescriptorHeap *hs[2] = {gv_srv, gv_smp}; cl->SetDescriptorHeaps(2, hs);
  cl->SetGraphicsRootSignature(rs);
  bd = Bound(); bd.valid = true; tgt_dirty = true;
}
void Exec12::submit() {
  barriers();
  HRESULT h = cl->Close();
  if (FAILED(h)) check_lost("ID3D12GraphicsCommandList::Close", h);
  if (!lost) { ID3D12CommandList *l = cl; q->ExecuteCommandLists(1, &l); }
  fval++; q->Signal(fence, fval);
  ring_marks.push_back({fval, ring_head}); gv_marks.push_back({fval, gv_head});
  alloc_fence[fi] = fval; submits++;
  begin_list();
}
bool Exec12::ring_alloc(uint64_t bytes, uint64_t align, uint8_t **cpu, D3D12_GPU_VIRTUAL_ADDRESS *gpu, uint64_t *off) {
  if (bytes > ring_cap / 2) return false;
  if (!bytes) bytes = 4;
  for (int tries = 0;; tries++) {
    ring_retire();
    uint64_t start = (ring_head + align - 1) / align * align, phys = start % ring_cap;
    if (phys + bytes > ring_cap) { start += ring_cap - phys; phys = 0; }
    if (start + bytes - ring_tail <= ring_cap) {
      ring_head = start + bytes; *cpu = ring_m + phys; *gpu = ring_va + phys; if (off) *off = phys; return true;
    }
    if (lost) { ring_head = ring_tail = 0; ring_marks.clear(); continue; }
    ring_waits++;
    if (ring_marks.empty()) submit();                      // all of it is this list's: send it, then wait for it
    wait(ring_marks.front().fence);
  }
}

// ---- objects ---------------------------------------------------------------------------------------------------------
void Exec12::to(ID3D12Resource *r, D3D12_RESOURCE_STATES &cur, D3D12_RESOURCE_STATES s) {
  if (!r || cur == s) return;
  D3D12_RESOURCE_BARRIER b = {}; b.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
  b.Transition.pResource = r; b.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
  b.Transition.StateBefore = cur; b.Transition.StateAfter = s;
  bars.push_back(b); cur = s;
}
void Exec12::free_res(Res &r, bool now) {
  for (auto &v : r.rtv) h_rtv.give(v.second);
  for (auto &v : r.srv_levels) h_srv.give(v.second);
  if (r.srv != ~0u) h_srv.give(r.srv);
  if (r.dsv != ~0u) h_dsv.give(r.dsv);
  r.rtv.clear(); r.srv_levels.clear(); r.srv = r.dsv = ~0u;
  if (r.kind != 4) {                                       // (the back buffers are the swap chain's)
    if (now) { rel(r.r); rel(r.snap); } else { bury(r.r); bury(r.snap); r.r = nullptr; r.snap = nullptr; }
  }
  r.r = nullptr;
}
static D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc(DXGI_FORMAT f, bool cube, UINT first, UINT count) {
  D3D12_SHADER_RESOURCE_VIEW_DESC v = {}; v.Format = f; v.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
  if (cube) { v.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE; v.TextureCube.MostDetailedMip = first; v.TextureCube.MipLevels = count; }
  else { v.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; v.Texture2D.MostDetailedMip = first; v.Texture2D.MipLevels = count; }
  return v;
}
D3D12_CPU_DESCRIPTOR_HANDLE Exec12::rtv_of(const XRef &ref, DXGI_FORMAT *fmt, uint32_t *w, uint32_t *h, Res **rr) {
  D3D12_CPU_DESCRIPTOR_HANDLE none = {};
  Res *x = res_of(ref.id);
  if (!x || !x->r || x->kind == XT_DEPTH || x->kind == 3) return none;
  uint32_t key = ref.level * 8 + ref.face;
  auto v = x->rtv.find(key);
  uint32_t idx;
  if (v != x->rtv.end()) idx = v->second;
  else {
    idx = h_rtv.take(dev); if (idx == ~0u) return none;
    D3D12_RENDER_TARGET_VIEW_DESC rd = {}; rd.Format = x->fmt;
    if (x->faces == 6) { rd.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY; rd.Texture2DArray.MipSlice = ref.level; rd.Texture2DArray.FirstArraySlice = ref.face; rd.Texture2DArray.ArraySize = 1; }
    else { rd.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D; rd.Texture2D.MipSlice = ref.level; }
    dev->CreateRenderTargetView(x->r, &rd, h_rtv.at(idx));
    x->rtv[key] = idx;
  }
  if (fmt) *fmt = x->fmt;
  if (w) *w = std::max(1u, x->w >> ref.level);
  if (h) *h = std::max(1u, x->h >> ref.level);
  if (rr) *rr = x;
  return h_rtv.at(idx);
}
// X_TARGETS, resolved now (the back buffer is whichever is current) and bound; each draw and clear also moves the targets
// into their states again (a copy may have moved them)
void Exec12::bind_targets() {
  if (tgt_dirty) {
    Res *rr = nullptr; has_rtv = false; rt_res = nullptr; rt_st = nullptr; rt_fmt = DXGI_FORMAT_UNKNOWN; rt_w = rt_h = 0;
    if (tgt_rt.id) {
      cur_rtv = rtv_of(tgt_rt, &rt_fmt, &rt_w, &rt_h, &rr);
      if (cur_rtv.ptr && rr) { has_rtv = true; rt_res = rr->r; rt_st = &rr->st; }
    }
    ds_res = tgt_ds ? res_of(tgt_ds) : nullptr; has_dsv = false; ds_fmt = DXGI_FORMAT_UNKNOWN;
    if (ds_res && ds_res->r && ds_res->dsv != ~0u) { cur_dsv = h_dsv.at(ds_res->dsv); has_dsv = true; ds_fmt = ds_res->fmt; if (!has_rtv) { rt_w = ds_res->w; rt_h = ds_res->h; } }
    else ds_res = nullptr;
    tgt_dirty = false;
    cl->OMSetRenderTargets(has_rtv ? 1 : 0, has_rtv ? &cur_rtv : nullptr, FALSE, has_dsv ? &cur_dsv : nullptr);
    ts_mark(tgt_key());                                      // the GPU's time from here on is this target's
  }
  if (rt_res && rt_st) to(rt_res, *rt_st, D3D12_RESOURCE_STATE_RENDER_TARGET);
  if (ds_res) to(*ds_res, D3D12_RESOURCE_STATE_DEPTH_WRITE);
}

// ---- init ----------------------------------------------------------------------------------------------------------------
typedef HRESULT (WINAPI *PFN_Serialize12)(const D3D12_ROOT_SIGNATURE_DESC *, D3D_ROOT_SIGNATURE_VERSION, ID3DBlob **, ID3DBlob **);
typedef HRESULT (WINAPI *PFN_DebugIf12)(REFIID, void **);
int32_t Exec12::init(const XInit &x, bool resize) {
  wnd = (HWND)(uintptr_t)x.hwnd; sync = x.interval <= 4 ? x.interval : 0; width = x.width; height = x.height;
  if (resize) {
    if (!swap) return D3DERR_NOTAVAILABLE;
    wait_idle();
    Res *b = res_of(X_BACKBUFFER); if (b) { free_res(*b, true); res.erase(X_BACKBUFFER); }
    for (int i = 0; i < NBB; i++) rel(bb[i]);
    tgt_dirty = true; rt_res = nullptr; rt_st = nullptr;
    HRESULT h = swap->ResizeBuffers(NBB, x.width, x.height, DXGI_FORMAT_B8G8R8A8_UNORM, 0);
    if (FAILED(h)) { d8_log("D3D12: ResizeBuffers %ux%u: 0x%08lX", x.width, x.height, (unsigned long)h); return D3DERR_NOTAVAILABLE; }
  } else {
    if (getenv("D8ON11_TEST_D3D12_FAIL")) { d8_log("D3D12: not started (D8ON11_TEST_D3D12_FAIL, a test)"); return D3DERR_NOTAVAILABLE; }
    HMODULE m = LoadLibraryA("d3d12.dll");
    PFN_D3D12_CREATE_DEVICE create = m ? (PFN_D3D12_CREATE_DEVICE)(void *)GetProcAddress(m, "D3D12CreateDevice") : nullptr;
    PFN_Serialize12 serialize = m ? (PFN_Serialize12)(void *)GetProcAddress(m, "D3D12SerializeRootSignature") : nullptr;
    if (!create || !serialize) { d8_log("D3D12: d3d12.dll %s", m ? "has no D3D12CreateDevice / D3D12SerializeRootSignature" : "not loaded"); return D3DERR_NOTAVAILABLE; }
    if (getenv("D8ON11_DEBUG_LAYER")) {
      PFN_DebugIf12 dbg = (PFN_DebugIf12)(void *)GetProcAddress(m, "D3D12GetDebugInterface"); ID3D12Debug *d = nullptr;
      if (dbg && SUCCEEDED(dbg(__uuidof(ID3D12Debug), (void **)&d))) { d->EnableDebugLayer(); d->Release(); d8_log("D3D12: debug layer on"); }
    }
    HRESULT h = CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void **)&fac);
    if (FAILED(h)) { d8_log("D3D12: CreateDXGIFactory1 (IDXGIFactory4): 0x%08lX", (unsigned long)h); return D3DERR_NOTAVAILABLE; }
    IUnknown *adapter = nullptr; IDXGIAdapter *warp = nullptr;
    const char *drv = getenv("D8ON11_DRIVER");
    if (drv && !_stricmp(drv, "warp") && SUCCEEDED(fac->EnumWarpAdapter(__uuidof(IDXGIAdapter), (void **)&warp))) adapter = warp;
    h = create(adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void **)&dev);
    rel(warp);
    if (FAILED(h) || !dev) { d8_log("D3D12: D3D12CreateDevice (feature level 11_0): 0x%08lX", (unsigned long)h); dev = nullptr; return D3DERR_NOTAVAILABLE; }
    D3D12_COMMAND_QUEUE_DESC qd = {}; qd.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    if (FAILED(h = dev->CreateCommandQueue(&qd, __uuidof(ID3D12CommandQueue), (void **)&q))) { d8_log("D3D12: CreateCommandQueue: 0x%08lX", (unsigned long)h); return D3DERR_NOTAVAILABLE; }
    if (FAILED(h = dev->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void **)&fence))) { d8_log("D3D12: CreateFence: 0x%08lX", (unsigned long)h); return D3DERR_NOTAVAILABLE; }
    fev = CreateEventA(NULL, FALSE, FALSE, NULL);
    for (int i = 0; i < NF; i++)
      if (FAILED(h = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void **)&alloc[i]))) { d8_log("D3D12: CreateCommandAllocator: 0x%08lX", (unsigned long)h); return D3DERR_NOTAVAILABLE; }
    if (FAILED(h = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, alloc[0], nullptr, __uuidof(ID3D12GraphicsCommandList), (void **)&cl))) { d8_log("D3D12: CreateCommandList: 0x%08lX", (unsigned long)h); return D3DERR_NOTAVAILABLE; }
    cl->Close();
    // the upload ring
    { D3D12_HEAP_PROPERTIES hp = {}; hp.Type = D3D12_HEAP_TYPE_UPLOAD;
      D3D12_RESOURCE_DESC rd = {}; rd.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER; rd.Width = (uint64_t)RING_MB << 20; rd.Height = 1; rd.DepthOrArraySize = 1;
      rd.MipLevels = 1; rd.SampleDesc.Count = 1; rd.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
      if (FAILED(h = dev->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, __uuidof(ID3D12Resource), (void **)&ring))) { d8_log("D3D12: the upload ring (%d MB): 0x%08lX", RING_MB, (unsigned long)h); return D3DERR_NOTAVAILABLE; }
      D3D12_RANGE none = {0, 0};
      if (FAILED(h = ring->Map(0, &none, (void **)&ring_m))) { d8_log("D3D12: Map of the upload ring: 0x%08lX", (unsigned long)h); return D3DERR_NOTAVAILABLE; }
      ring_va = ring->GetGPUVirtualAddress(); ring_cap = rd.Width; }
    // descriptor heaps
    h_srv.type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; h_rtv.type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; h_dsv.type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    h_smp.type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
    for (CpuHeap *c : {&h_srv, &h_rtv, &h_dsv, &h_smp}) c->inc = dev->GetDescriptorHandleIncrementSize(c->type);
    { D3D12_DESCRIPTOR_HEAP_DESC hd = {}; hd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; hd.NumDescriptors = GV_CAP; hd.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
      if (FAILED(h = dev->CreateDescriptorHeap(&hd, __uuidof(ID3D12DescriptorHeap), (void **)&gv_srv))) { d8_log("D3D12: the shader-visible descriptor heap: 0x%08lX", (unsigned long)h); return D3DERR_NOTAVAILABLE; }
      gv_inc = h_srv.inc; gv_cap = GV_CAP;
      hd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER; hd.NumDescriptors = SMP_CAP;
      if (FAILED(h = dev->CreateDescriptorHeap(&hd, __uuidof(ID3D12DescriptorHeap), (void **)&gv_smp))) { d8_log("D3D12: the sampler heap: 0x%08lX", (unsigned long)h); return D3DERR_NOTAVAILABLE; }
      smp_inc = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER); }
    // null views for the stages a draw does not use (resource binding tier 1 wants every descriptor of a table valid)
    null_srv2d = h_srv.take(dev); null_srvcube = h_srv.take(dev);
    { D3D12_SHADER_RESOURCE_VIEW_DESC v = srv_desc(DXGI_FORMAT_R8G8B8A8_UNORM, false, 0, 1); dev->CreateShaderResourceView(nullptr, &v, h_srv.at(null_srv2d));
      v = srv_desc(DXGI_FORMAT_R8G8B8A8_UNORM, true, 0, 1); dev->CreateShaderResourceView(nullptr, &v, h_srv.at(null_srvcube)); }
    // the root signature: b0 (VS constants), b1 (the bone palette), b0 (PS constants), t0..t7, s0..s7
    { D3D12_DESCRIPTOR_RANGE rt = {D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 8, 0, 0, 0}, rsm = {D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 8, 0, 0, 0};
      D3D12_ROOT_PARAMETER rp[5] = {};
      rp[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; rp[0].Descriptor.ShaderRegister = 0; rp[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
      rp[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; rp[1].Descriptor.ShaderRegister = 1; rp[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
      rp[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; rp[2].Descriptor.ShaderRegister = 0; rp[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
      rp[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; rp[3].DescriptorTable.NumDescriptorRanges = 1; rp[3].DescriptorTable.pDescriptorRanges = &rt; rp[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
      rp[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; rp[4].DescriptorTable.NumDescriptorRanges = 1; rp[4].DescriptorTable.pDescriptorRanges = &rsm; rp[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
      D3D12_ROOT_SIGNATURE_DESC rd = {}; rd.NumParameters = 5; rd.pParameters = rp; rd.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
      ID3DBlob *b = nullptr, *e = nullptr;
      h = serialize(&rd, D3D_ROOT_SIGNATURE_VERSION_1, &b, &e);
      if (FAILED(h)) { d8_log("D3D12: D3D12SerializeRootSignature: 0x%08lX %s", (unsigned long)h, e ? (const char *)e->GetBufferPointer() : ""); rel(e); return D3DERR_NOTAVAILABLE; }
      rel(e);
      h = dev->CreateRootSignature(0, b->GetBufferPointer(), b->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&rs); rel(b);
      if (FAILED(h)) { d8_log("D3D12: CreateRootSignature: 0x%08lX", (unsigned long)h); return D3DERR_NOTAVAILABLE; } }
    HMODULE cm = LoadLibraryA("d3dcompiler_47.dll");
    compile = cm ? (PFN_Compile)(void *)GetProcAddress(cm, "D3DCompile") : nullptr;
    create_blob = cm ? (PFN_CreateBlob)(void *)GetProcAddress(cm, "D3DCreateBlob") : nullptr;
    caches_load();
    if (!compile) d8_log("D3D12: d3dcompiler_47.dll not found: draws cannot be executed");
    // the swap chain: the flip model (the only one Direct3D 12 has)
    DXGI_SWAP_CHAIN_DESC1 sd = {}; sd.Width = x.width; sd.Height = x.height; sd.Format = DXGI_FORMAT_B8G8R8A8_UNORM; sd.SampleDesc.Count = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; sd.BufferCount = NBB; sd.Scaling = DXGI_SCALING_STRETCH; sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    IDXGISwapChain1 *s1 = nullptr;
    h = fac->CreateSwapChainForHwnd(q, wnd, &sd, nullptr, nullptr, &s1);
    if (FAILED(h) || FAILED(s1->QueryInterface(__uuidof(IDXGISwapChain3), (void **)&swap))) {
      d8_log("D3D12: CreateSwapChainForHwnd %ux%u (FLIP_DISCARD, %d buffers): 0x%08lX", x.width, x.height, NBB, (unsigned long)h); rel(s1); return D3DERR_NOTAVAILABLE;
    }
    rel(s1);
    fac->MakeWindowAssociation(wnd, DXGI_MWA_NO_ALT_ENTER);
    begin_list();
    // the sampler check (sampler_check): the copied descriptors first, then made in place; neither right: Direct3D 11
    { uint32_t a[2] = {0, 0}, b[2] = {0, 0};
      const bool force_wrong = getenv("D8ON11_TEST_D3D12_SAMPLERS") != nullptr;   // a test: both ways count as wrong
      if (getenv("D8ON11_D3D12_DIAG")) d8_log("D3D12 diagnostic switches: %s", getenv("D8ON11_D3D12_DIAG"));
      int ra, rb2;
      if (diag("nocheck") || diag("smpdirect")) { ra = rb2 = -1; smp_copy = !diag("smpdirect"); d8_log("D3D12 sampler check: not run (diagnostic); samplers %s", smp_copy ? "copied" : "made in the shader heap"); goto checked; }
      ra = sampler_check(true, a); rb2 = ra == 3 && !force_wrong ? 3 : sampler_check(false, b);
      if (force_wrong) { ra = rb2 = 0; d8_log("D3D12 sampler check: both ways treated as wrong whatever they draw (D8ON11_TEST_D3D12_SAMPLERS, a test)"); }
      d8_log("D3D12 sampler check: samplers copied into the shader heap: linear sample %u (128 is right), clamped sample %u (255 is right): %s%s",
             a[0], a[1], ra == 3 ? "right" : ra < 0 ? "not run" : "WRONG", ra == 3 ? "" : "");
      if (ra != 3) d8_log("D3D12 sampler check: samplers made in the shader heap: linear sample %u, clamped sample %u: %s", b[0], b[1], rb2 == 3 ? "right" : rb2 < 0 ? "not run" : "WRONG");
      if (ra == 3) smp_copy = true;
      else if (rb2 == 3) smp_copy = false;
      else { d8_log("D3D12: this driver does not sample as the stream asks by either way: Direct3D 11 runs the stream"); return D3DERR_NOTAVAILABLE; }
      checked:; }
    ts_init(); ts_begin();
    D3D12_FEATURE_DATA_D3D12_OPTIONS op = {}; dev->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &op, sizeof op);
    d8_log("D3D12 executor: device made; resource binding tier %d; upload ring %d MB; swap chain %ux%u, FLIP_DISCARD, %d buffers; up to %u frames in flight",
           (int)op.ResourceBindingTier, RING_MB, x.width, x.height, NBB, inflight);
  }
  for (int i = 0; i < NBB; i++) {
    if (FAILED(swap->GetBuffer(i, __uuidof(ID3D12Resource), (void **)&bb[i]))) { d8_log("D3D12: GetBuffer %d failed", i); return D3DERR_NOTAVAILABLE; }
    bb_st[i] = D3D12_RESOURCE_STATE_PRESENT;
  }
  Res &b = res[X_BACKBUFFER]; b = Res(); b.kind = 4; b.w = x.width; b.h = x.height; b.d3dfmt = D3DFMT_X8R8G8B8; b.fmt = DXGI_FORMAT_B8G8R8A8_UNORM;
  uint32_t cur = swap->GetCurrentBackBufferIndex(); b.r = bb[cur]; b.st = bb_st[cur];
  bbi = cur;
  return D3D_OK;
}

// ---- records -------------------------------------------------------------------------------------------------------------
void Exec12::run(uint16_t op, const uint8_t *p, uint32_t n, int32_t *hr, std::vector<uint8_t> *reply) {
  bool was_same = false;
  if (op == X_DRAW_SAME) {                                 // draw_delta: the last packet's head, then this tail
    if (n < XDRAW_TAIL || head.size() != XDRAW_HEAD) { d8_once("samenohead12", "stream error: X_DRAW_SAME with no draw before it (skipped)"); return; }
    same.resize(sizeof(XDraw)); memcpy(same.data(), head.data(), XDRAW_HEAD); memcpy(same.data() + XDRAW_HEAD, p, XDRAW_TAIL);
    p = same.data(); n = (uint32_t)sizeof(XDraw); op = X_DRAW; was_same = true;
  } else if (op == X_DRAW && n >= sizeof(XDraw)) head.assign(p, p + XDRAW_HEAD);
  // draw merging: a draw that continues the held run joins it; anything else draws the run first
  if (merge_on && dev && !lost) {
    if (op == X_DRAW && was_same && run_n) {
      std::vector<uint8_t> first(sizeof(XDraw)); memcpy(first.data(), run_head.data(), XDRAW_HEAD); memcpy(first.data() + XDRAW_HEAD, run_tails.data(), XDRAW_TAIL);
      if (same_run(*(const XDraw *)first.data(), *(const XDraw *)p)) { run_tails.insert(run_tails.end(), p + XDRAW_HEAD, p + sizeof(XDraw)); run_n++; return; }
    }
    if (run_n) flush_run();
    if (op == X_DRAW && n >= sizeof(XDraw) && mergeable(*(const XDraw *)p)) {
      run_head.assign(p, p + XDRAW_HEAD); run_tails.assign(p + XDRAW_HEAD, p + sizeof(XDraw)); run_n = 1; return;
    }
  }
  if (!dev && op != X_INIT && op != X_OPTION) return;
  if (lost && op != X_INIT && op != X_RESIZE && op != X_OPTION && op != X_READBACK && op != X_FINISH && op != X_PRESENT) {
    if (op == X_DRAW) o.draws++;
    return;
  }
  switch (op) {
  case X_INIT: case X_RESIZE: {
    const XInit &x = *(const XInit *)p;
    *hr = init(x, op == X_RESIZE);
    if (*hr >= 0 && x.ds_format) {
      Res *old = res_of(x.ds_id); if (old) { free_res(*old, false); res.erase(x.ds_id); }
      XTex t = {x.ds_id, XT_DEPTH, x.width, x.height, 1, 1, x.ds_format, D3DUSAGE_DEPTHSTENCIL};
      int32_t h2 = 0; run(X_TEX, (const uint8_t *)&t, sizeof t, &h2, nullptr);
    }
    break; }
  case X_TEX: {
    const XTex &t = *(const XTex *)p;
    if (res.count(t.id)) { d8_log("stream error: object %u created twice (the earlier one is replaced)", t.id); free_res(res[t.id], false); res.erase(t.id); }
    FmtInfo fi = d8_fmt((D3DFORMAT)t.d3dfmt);
    Res x; x.kind = t.kind; x.w = t.width; x.h = t.height; x.levels = t.levels; x.faces = t.faces; x.d3dfmt = t.d3dfmt; x.fmt = fi.dxgi;
    D3D12_HEAP_PROPERTIES hp = {}; hp.Type = D3D12_HEAP_TYPE_DEFAULT;
    D3D12_RESOURCE_DESC rd = {}; rd.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; rd.Width = t.width; rd.Height = t.height;
    rd.DepthOrArraySize = (UINT16)t.faces; rd.MipLevels = (UINT16)t.levels; rd.Format = fi.dxgi; rd.SampleDesc.Count = 1;
    D3D12_CLEAR_VALUE cv = {}; cv.Format = fi.dxgi; const D3D12_CLEAR_VALUE *pcv = nullptr;
    const bool rt = t.kind == XT_RENDERTARGET || (t.usage & D3DUSAGE_RENDERTARGET);
    if (t.kind == XT_DEPTH) { rd.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE; x.st = D3D12_RESOURCE_STATE_DEPTH_WRITE; cv.DepthStencil.Depth = 1; pcv = &cv; }
    else if (rt) { rd.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET; x.st = D3D12_RESOURCE_STATE_RENDER_TARGET; }
    else x.st = D3D12_RESOURCE_STATE_COPY_DEST;
    HRESULT h = dev->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, x.st, pcv, __uuidof(ID3D12Resource), (void **)&x.r);
    if (FAILED(h)) { d8_log("CreateTexture2D %ux%u format %u levels %u: 0x%08lX (D3D12)", t.width, t.height, t.d3dfmt, t.levels, (unsigned long)h); check_lost("CreateCommittedResource", S_OK); break; }
    if (t.kind == XT_DEPTH) {
      x.dsv = h_dsv.take(dev);
      D3D12_DEPTH_STENCIL_VIEW_DESC dd = {}; dd.Format = fi.dxgi; dd.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
      if (x.dsv != ~0u) dev->CreateDepthStencilView(x.r, &dd, h_dsv.at(x.dsv));
    } else {
      x.srv = h_srv.take(dev);
      D3D12_SHADER_RESOURCE_VIEW_DESC v = srv_desc(fi.dxgi, t.faces == 6, 0, t.levels);
      if (x.srv != ~0u) dev->CreateShaderResourceView(x.r, &v, h_srv.at(x.srv));
    }
    res[t.id] = std::move(x);
    break; }
  case X_BUF: {
    const XBuf &b = *(const XBuf *)p;
    if (res.count(b.id)) { d8_log("stream error: object %u created twice (the earlier one is replaced)", b.id); free_res(res[b.id], false); res.erase(b.id); }
    Res x; x.kind = 3; x.index = b.index; x.w = b.length; x.dyn = b.dyn != 0; x.serial = ++res_serial;
    uint32_t size = (b.length + 15) & ~15u;
    if (x.dyn) x.shadow.assign(size, 0);                    // its contents live in the ring (dyn_view); this is the CPU's copy
    else {
      if (merge_on && b.index && size <= (256u << 10)) x.shadow.assign(size, 0);   // draw merging rewrites its indices from this copy
      D3D12_HEAP_PROPERTIES hp = {}; hp.Type = D3D12_HEAP_TYPE_DEFAULT;
      D3D12_RESOURCE_DESC rd = {}; rd.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER; rd.Width = size; rd.Height = 1; rd.DepthOrArraySize = 1;
      rd.MipLevels = 1; rd.SampleDesc.Count = 1; rd.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
      x.st = D3D12_RESOURCE_STATE_COPY_DEST;
      if (FAILED(dev->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, x.st, nullptr, __uuidof(ID3D12Resource), (void **)&x.r))) {
        d8_log("CreateBuffer %u bytes failed (D3D12)", b.length); check_lost("CreateCommittedResource", S_OK); break;
      }
      x.va = x.r->GetGPUVirtualAddress();
    }
    res[b.id] = std::move(x);
    break; }
  case X_DESTROY: {
    uint32_t id = *(const uint32_t *)p;
    auto it = res.find(id);
    if (it == res.end()) { d8_once("destroyunknown12", "stream error: object %u destroyed but not held (its creation failed, or a stream error)", id); break; }
    for (size_t i = 0; i < snap_pend.size(); i++) if (snap_pend[i].id == id) { snap_pend.erase(snap_pend.begin() + i); break; }
    if (id == tgt_rt.id || id == tgt_ds) { tgt_dirty = true; rt_res = nullptr; rt_st = nullptr; ds_res = nullptr; }
    if (id == tgt_rt.id) tgt_rt = XRef(); if (id == tgt_ds) tgt_ds = 0;
    free_res(it->second, false); res.erase(it);
    last_srv_epoch = ~0ull;                                // a view the last table holds may be this object's
    break; }
  case X_UPLOAD_TEX: upload_tex(*(const XUpTex *)p, p + sizeof(XUpTex)); break;
  case X_UPLOAD_BUF: upload_buf(*(const XUpBuf *)p, p + sizeof(XUpBuf)); break;
  case X_SKIN: {
    const XSkin &x = *(const XSkin *)p;
    if (x.nbones) {
      last_skin_pal = std::make_shared<SkinPal>(); last_skin_pal->serial = ++pal_serial;
      last_skin_pal->d.assign(64u * SKIN_BONES, 0); memcpy(last_skin_pal->d.data(), p + sizeof x, 64u * std::min(x.nbones, (uint32_t)SKIN_BONES));
    }
    Res *ps = res_of(x.pos), *pn = res_of(x.nrm);
    if (!ps || !pn || !res_of(x.src) || !last_skin_pal) { d8_once("skinjob12", "stream error: X_SKIN names an object not held, or has no palette (skipped)"); break; }
    ps->skin_src = x.src; ps->pal = last_skin_pal; pn->skin_src = 0; pn->pal.reset();
    break; }
  case X_TARGETS: {
    const XTargets &t = *(const XTargets *)p;
    tgt_rt = t.rt; tgt_ds = t.ds; tgt_dirty = true;
    break; }
  case X_CLEAR: clear(*(const XClear *)p, (const XRect *)((const XClear *)p + 1)); break;
  case X_COPY: copy(*(const XCopy *)p); break;
  case X_DRAW: { const XDraw &d = *(const XDraw *)p; last_site = o.last_site = d.site; draw(d, p + sizeof(XDraw)); o.draws++; break; }
  case X_PRESENT: present(); break;
  case X_SNAP: snap(*(const XSnap *)p); break;
  case X_READBACK: *hr = readback(*(const XRef *)p, reply); break;
  case X_FINISH: *hr = D3D_OK; break;
  case X_OPTION: {
    const XOption &x = *(const XOption *)p;
    if (x.key == OPT_LATENCY) {
      // frame_latency=N: DXGI lets N frames queue for the GPU; here the CPU may be N+1 submissions of frames ahead (it
      // records the next while N wait), at most NF
      if (x.value) inflight = std::min<uint32_t>(x.value + 1, NF);
    } else if (x.key == OPT_MERGE) {
      if (run_n) flush_run();
      merge_on = (x.value & 1) != 0;
      d8_once("merge12", "draw_merge: the Direct3D 12 executor merges runs of delta draws (needs draw_delta=1)");
    } else if (x.key == OPT_SITES || x.key == OPT_D3D12) {
    } else if (x.key == OPT_FLIP) {
      if (!x.value) d8_once("flip12", "swap_flip=0: Direct3D 12 has only the flip model; it is used");
    } else if (x.value) {
      static const char *nm[] = {"", "exec_filter", "exec_batch", "draw_sites", "swap_flip", "gpu_time", "frame_latency", "buffer_arena", "draw_merge"};
      char k[32]; snprintf(k, sizeof k, "opt12_%u", x.key);
      d8_once(k, "%s=%u: a Direct3D 11 executor lever, not applied by the Direct3D 12 executor", x.key < 9 ? nm[x.key] : "?", x.value);
    }
    break; }
  }
}

// ---- uploads -------------------------------------------------------------------------------------------------------------
void Exec12::upload_tex(const XUpTex &u, const uint8_t *rows) {
  Res *x = res_of(u.id); if (!x || !x->r) return;
  uint32_t level = u.sub % x->levels;
  uint32_t lw = std::max(1u, x->w >> level), lh = std::max(1u, x->h >> level);
  uint32_t w = u.w ? u.w : lw, h = u.w ? u.h : lh;
  FmtInfo fi = d8_fmt((D3DFORMAT)x->d3dfmt);
  if (fi.block) { w = (w + 3) & ~3u; h = (h + 3) & ~3u; }  // a block format's footprint is whole blocks (a small level's too)
  uint64_t pitch = ((uint64_t)u.rowbytes + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1) & ~(uint64_t)(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1);
  uint64_t bytes = pitch * u.rows;
  uint8_t *cpu; D3D12_GPU_VIRTUAL_ADDRESS va; uint64_t off; ID3D12Resource *src = ring; ID3D12Resource *own = nullptr;
  if (!ring_alloc(bytes, D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT, &cpu, &va, &off)) {
    // larger than half the ring (the game's 32 MB texture, d8_exec.cpp's note on big uploads): a buffer of its own
    D3D12_HEAP_PROPERTIES hp = {}; hp.Type = D3D12_HEAP_TYPE_UPLOAD;
    D3D12_RESOURCE_DESC rd = {}; rd.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER; rd.Width = bytes; rd.Height = 1; rd.DepthOrArraySize = 1;
    rd.MipLevels = 1; rd.SampleDesc.Count = 1; rd.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    if (FAILED(dev->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, __uuidof(ID3D12Resource), (void **)&own)) ||
        FAILED(own->Map(0, nullptr, (void **)&cpu))) { d8_once("bigupload12", "D3D12: an upload of %llu bytes: no buffer for it (lost)", (unsigned long long)bytes); rel(own); return; }
    src = own; off = 0;
  }
  for (uint32_t y = 0; y < u.rows; y++) memcpy(cpu + y * pitch, rows + (size_t)y * u.rowbytes, u.rowbytes);
  if (own) { own->Unmap(0, nullptr); bury(own); }          // (released when the GPU is past this submission)
  to(*x, D3D12_RESOURCE_STATE_COPY_DEST); barriers();
  D3D12_TEXTURE_COPY_LOCATION dl = {}, sl = {};
  dl.pResource = x->r; dl.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX; dl.SubresourceIndex = u.sub;
  sl.pResource = src; sl.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT; sl.PlacedFootprint.Offset = off;
  sl.PlacedFootprint.Footprint.Format = x->fmt; sl.PlacedFootprint.Footprint.Width = w; sl.PlacedFootprint.Footprint.Height = h;
  sl.PlacedFootprint.Footprint.Depth = 1; sl.PlacedFootprint.Footprint.RowPitch = (UINT)pitch;
  cl->CopyTextureRegion(&dl, u.w ? u.x : 0, u.w ? u.y : 0, 0, &sl, nullptr);
  o.up_bytes[0] += (uint64_t)u.rowbytes * u.rows; o.up_n[0]++;
  if (x->up_frame != ~0ull && o.presents - x->up_frame <= 1) o.up_again[0] += (uint64_t)u.rowbytes * u.rows;
  x->up_frame = o.presents;
}
void Exec12::upload_buf(const XUpBuf &u, const uint8_t *data) {
  Res *x = res_of(u.id); if (!x) return;
  o.up_bytes[1] += u.length; o.up_n[1]++;
  if (x->up_frame != ~0ull && o.presents - x->up_frame <= 1) o.up_again[1] += u.length;
  x->up_frame = o.presents;
  x->skin_src = 0; x->pal.reset();                         // written: no longer a skinned mesh's stand-in
  x->ver++;
  if (!x->dyn && !x->shadow.empty() && (uint64_t)u.offset + u.length <= x->shadow.size()) memcpy(x->shadow.data() + u.offset, data, u.length);
  if (x->dyn) {
    if ((uint64_t)u.offset + u.length > x->shadow.size()) { d8_once("dynrange12", "a dynamic buffer upload past its end (skipped)"); return; }
    memcpy(x->shadow.data() + u.offset, data, u.length);
    const bool noov = (u.flags & 0x1000u) && !(u.flags & 0x2000u);
    // NOOVERWRITE: the range in place, where this submission's draws read the buffer (the game promises none of them
    // reads it); otherwise the buffer gets a new place at its next draw (a DISCARD's fresh memory)
    if (noov && x->epoch == submits && x->cpu) { memcpy(x->cpu + u.offset, data, u.length); if (!o.dyn_noov++) d8_log("dynamic buffers: the first NOOVERWRITE upload (D3D12: in place)"); }
    else { x->epoch = ~0ull; if (!o.dyn_discard++) d8_log("dynamic buffers: the first DISCARD upload (D3D12: a new place at its next draw)"); }
    return;
  }
  if (!x->r || (uint64_t)u.offset + u.length > ((x->w + 15) & ~15u)) return;
  uint8_t *cpu; D3D12_GPU_VIRTUAL_ADDRESS va; uint64_t off;
  if (!ring_alloc(u.length, 16, &cpu, &va, &off)) { d8_once("bufbig12", "D3D12: a buffer upload larger than half the ring (lost)"); return; }
  memcpy(cpu, data, u.length);
  to(*x, D3D12_RESOURCE_STATE_COPY_DEST); barriers();
  cl->CopyBufferRegion(x->r, u.offset, ring, off, u.length);
}
// a dynamic buffer's contents for this submission: copied from its shadow at its first draw in it
bool Exec12::dyn_view(Res &r, D3D12_GPU_VIRTUAL_ADDRESS *va) {
  if (r.epoch != submits || !r.cpu) {
    uint8_t *cpu; D3D12_GPU_VIRTUAL_ADDRESS g;
    if (!ring_alloc(r.shadow.size(), 256, &cpu, &g)) return false;
    memcpy(cpu, r.shadow.data(), r.shadow.size()); r.cpu = cpu; r.va = g; r.epoch = submits;
  }
  *va = r.va; return true;
}

// ---- shaders and pipelines -----------------------------------------------------------------------------------------------
// compiled once per HLSL text: from the disk cache when an earlier start compiled the same text for the same target
// (the key is the text's hash and length, so a changed generator never reads old code)
ID3DBlob *Exec12::compile_hlsl(const std::string &src, const char *entry, const char *target) {
  if (!compile) return nullptr;
  char k[96]; snprintf(k, sizeof k, "%s|%s|%016llx|%u", target, entry, (unsigned long long)fnv64(src.data(), src.size()), (unsigned)src.size());
  auto it = sh_disk.find(k);
  if (it != sh_disk.end() && create_blob) {
    ID3DBlob *b = nullptr;
    if (SUCCEEDED(create_blob(it->second.size(), &b)) && b) { memcpy(b->GetBufferPointer(), it->second.data(), it->second.size()); c_cmp_loaded++; return b; }
  }
  ID3DBlob *code = nullptr, *err = nullptr; double t0 = qpc12();
  HRESULT h = compile(src.data(), src.size(), "d3d8on11", NULL, NULL, entry, target, D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &code, &err);
  c_cmp_ms += qpc12() - t0; c_cmp_new++;
  if (FAILED(h)) { d8_log("shader compile failed (%s, 0x%08lX): %s\n----\n%s----", target, (unsigned long)h, err ? (const char *)err->GetBufferPointer() : "", src.c_str()); rel(err); return nullptr; }
  rel(err);
  const uint8_t *cb = (const uint8_t *)code->GetBufferPointer();
  sh_disk[k].assign(cb, cb + code->GetBufferSize()); sh_dirty = true;
  return code;
}
// a pipeline state: from the driver's pipeline library when an earlier start stored it under the same name, else made and
// stored. The name is a hash of everything the description holds by value and of the shaders' code and layout
ID3D12PipelineState *Exec12::make_pso(const D3D12_GRAPHICS_PIPELINE_STATE_DESC &g, uint64_t vh, uint64_t ph) {
  uint64_t h = fnv64(&g.BlendState, sizeof g.BlendState);
  h = fnv64(&g.SampleMask, sizeof g.SampleMask, h); h = fnv64(&g.RasterizerState, sizeof g.RasterizerState, h);
  h = fnv64(&g.DepthStencilState, sizeof g.DepthStencilState, h); h = fnv64(&g.PrimitiveTopologyType, sizeof g.PrimitiveTopologyType, h);
  h = fnv64(&g.NumRenderTargets, sizeof g.NumRenderTargets, h); h = fnv64(g.RTVFormats, sizeof g.RTVFormats, h);
  h = fnv64(&g.DSVFormat, sizeof g.DSVFormat, h); h = fnv64(&g.SampleDesc, sizeof g.SampleDesc, h);
  h = fnv64(&vh, sizeof vh, h); h = fnv64(&ph, sizeof ph, h);
  WCHAR name[40]; swprintf(name, 40, L"d8_%016llx", (unsigned long long)h);
  ID3D12PipelineState *pso = nullptr; double t0 = qpc12();
  if (plib && SUCCEEDED(plib->LoadGraphicsPipeline(name, &g, __uuidof(ID3D12PipelineState), (void **)&pso)) && pso) { c_pso_loaded++; c_pso_ms += qpc12() - t0; return pso; }
  pso = nullptr;
  HRESULT hr = dev->CreateGraphicsPipelineState(&g, __uuidof(ID3D12PipelineState), (void **)&pso);
  c_pso_ms += qpc12() - t0; c_pso_new++;
  if (FAILED(hr)) { d8_log("D3D12: CreateGraphicsPipelineState: 0x%08lX (render target format %u, depth format %u)", (unsigned long)hr, (unsigned)g.RTVFormats[0], (unsigned)g.DSVFormat); return nullptr; }
  if (plib && SUCCEEDED(plib->StorePipeline(name, pso))) plib_dirty = true;
  return pso;
}

// ---- the caches on disk ---------------------------------------------------------------------------------------------------
static bool read_file(const std::string &path, std::vector<uint8_t> *out, size_t limit) {
  HANDLE f = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
  if (f == INVALID_HANDLE_VALUE) return false;
  LARGE_INTEGER sz; bool ok = GetFileSizeEx(f, &sz) && sz.QuadPart > 0 && (uint64_t)sz.QuadPart <= limit;
  if (ok) { out->resize((size_t)sz.QuadPart); DWORD got = 0; ok = ReadFile(f, out->data(), (DWORD)out->size(), &got, NULL) && got == out->size(); }
  CloseHandle(f); return ok;
}
static bool write_file(const std::string &path, const void *p, size_t n) {   // a new file in place of the old, whole or not at all
  std::string tmp = path + ".new";
  HANDLE f = CreateFileA(tmp.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
  if (f == INVALID_HANDLE_VALUE) return false;
  DWORD put = 0; bool ok = WriteFile(f, p, (DWORD)n, &put, NULL) && put == n; CloseHandle(f);
  return ok && MoveFileExA(tmp.c_str(), path.c_str(), MOVEFILE_REPLACE_EXISTING);
}
void Exec12::caches_load() {
  { HMODULE hm = NULL; char path[MAX_PATH] = {};
    GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)(void *)&fnv64, &hm);
    GetModuleFileNameA(hm, path, sizeof path); char *sl = strrchr(path, '\\'); if (sl) sl[1] = 0; else path[0] = 0;
    cache_dir = path; }
  if (getenv("D8ON11_NO_DISK_CACHE")) { d8_log("D3D12 caches: off (D8ON11_NO_DISK_CACHE)"); return; }
  std::vector<uint8_t> b; size_t nsh = 0;
  if (read_file(cache_dir + "d3d8on11_d3d12_shaders.bin", &b, 64u << 20) && b.size() >= 12 && !memcmp(b.data(), "D8SH", 4)) {
    size_t at = 12;                                            // "D8SH", version 1, count; then key length, key, code length, code
    uint32_t ver, cnt; memcpy(&ver, b.data() + 4, 4); memcpy(&cnt, b.data() + 8, 4);
    for (uint32_t i = 0; ver == 1 && i < cnt; i++) {
      uint32_t kl, cl2;
      if (at + 4 > b.size()) break; memcpy(&kl, &b[at], 4); at += 4; if (at + kl + 4 > b.size()) break;
      std::string key((const char *)&b[at], kl); at += kl; memcpy(&cl2, &b[at], 4); at += 4; if (at + cl2 > b.size()) break;
      sh_disk[key].assign(b.begin() + at, b.begin() + at + cl2); at += cl2; nsh++;
    }
  }
  ID3D12Device1 *d1 = nullptr; HRESULT h = E_NOINTERFACE;
  if (SUCCEEDED(dev->QueryInterface(__uuidof(ID3D12Device1), (void **)&d1))) {
    if (read_file(cache_dir + "d3d8on11_d3d12_pipelines.bin", &plib_blob, 256u << 20))
      h = d1->CreatePipelineLibrary(plib_blob.data(), plib_blob.size(), IID_D8PipelineLibrary, (void **)&plib);
    if (FAILED(h) || !plib) {                                  // none yet, or made by another driver or device: a new one
      if (!plib_blob.empty()) d8_log("D3D12 caches: the stored pipeline library was not taken (0x%08lX: another driver, device or build); a new one", (unsigned long)h);
      plib_blob.clear(); plib = nullptr;
      h = d1->CreatePipelineLibrary(nullptr, 0, IID_D8PipelineLibrary, (void **)&plib);
      if (FAILED(h)) { plib = nullptr; d8_log("D3D12 caches: no pipeline library on this device (0x%08lX): pipelines are made each start", (unsigned long)h); }
    }
    d1->Release();
  } else d8_log("D3D12 caches: no ID3D12Device1: pipelines are made each start");
  d8_log("D3D12 caches: %u compiled shaders from the last starts; pipeline library %s (%u KB) in %s", (unsigned)nsh,
         plib ? (plib_blob.empty() ? "new" : "loaded") : "none", (unsigned)(plib_blob.size() >> 10), cache_dir.c_str());
}
void Exec12::caches_save() {
  if (getenv("D8ON11_NO_DISK_CACHE") || cache_dir.empty()) return;
  if (sh_dirty) {
    std::vector<uint8_t> b(12); memcpy(b.data(), "D8SH", 4); uint32_t ver = 1, cnt = (uint32_t)sh_disk.size(); memcpy(&b[4], &ver, 4); memcpy(&b[8], &cnt, 4);
    for (auto &e : sh_disk) {
      uint32_t kl = (uint32_t)e.first.size(), cl2 = (uint32_t)e.second.size();
      b.insert(b.end(), (uint8_t *)&kl, (uint8_t *)&kl + 4); b.insert(b.end(), e.first.begin(), e.first.end());
      b.insert(b.end(), (uint8_t *)&cl2, (uint8_t *)&cl2 + 4); b.insert(b.end(), e.second.begin(), e.second.end());
    }
    if (write_file(cache_dir + "d3d8on11_d3d12_shaders.bin", b.data(), b.size())) sh_dirty = false;
    else d8_once("shsave12", "D3D12 caches: d3d8on11_d3d12_shaders.bin could not be written in %s", cache_dir.c_str());
  }
  if (plib && plib_dirty) {
    std::vector<uint8_t> b(plib->GetSerializedSize());
    if (!b.empty() && SUCCEEDED(plib->Serialize(b.data(), b.size())) && write_file(cache_dir + "d3d8on11_d3d12_pipelines.bin", b.data(), b.size())) plib_dirty = false;
    else d8_once("plsave12", "D3D12 caches: d3d8on11_d3d12_pipelines.bin could not be written in %s", cache_dir.c_str());
  }
}

// ---- the GPU's time -------------------------------------------------------------------------------------------------------
void Exec12::ts_init() {
  UINT64 f = 0;
  if (FAILED(q->GetTimestampFrequency(&f)) || !f) { d8_log("D3D12 gpu time: the queue gives no timestamp frequency: not measured"); return; }
  tfreq = (double)f;
  D3D12_QUERY_HEAP_DESC qd = {}; qd.Type = D3D12_QUERY_HEAP_TYPE_TIMESTAMP; qd.Count = TQ_FR * TQ_N;
  if (FAILED(dev->CreateQueryHeap(&qd, __uuidof(ID3D12QueryHeap), (void **)&qh))) { d8_log("D3D12 gpu time: no timestamp query heap: not measured"); return; }
  qrb = readback_buffer12(dev, (uint64_t)TQ_FR * TQ_N * 8);
  void *m = nullptr;
  if (!qrb || FAILED(qrb->Map(0, nullptr, &m))) { d8_log("D3D12 gpu time: no readback buffer: not measured"); rel(qh); rel(qrb); return; }
  qrb_m = (const uint64_t *)m; tq_on = true;
}
void Exec12::ts_mark(uint32_t key) {
  if (!tq_on) return;
  TFrame &f = tq[tq_cur];
  if (f.n >= TQ_N - 1) return;                                 // (the last one is the frame's end)
  cl->EndQuery(qh, D3D12_QUERY_TYPE_TIMESTAMP, tq_cur * TQ_N + f.n); f.key[f.n++] = key;
}
void Exec12::ts_begin() {
  if (!tq_on) return;
  TFrame &f = tq[tq_cur];
  if (f.pending) { ts_collect(); if (f.pending) { f.pending = false; tq_bad++; } }   // (8 frames behind: not measured)
  f.n = 0; ts_mark(tgt_dirty ? 0 : tgt_key());
}
void Exec12::ts_end() {
  if (!tq_on) return;
  TFrame &f = tq[tq_cur];
  cl->EndQuery(qh, D3D12_QUERY_TYPE_TIMESTAMP, tq_cur * TQ_N + f.n); f.key[f.n++] = ~0u;
  cl->ResolveQueryData(qh, D3D12_QUERY_TYPE_TIMESTAMP, tq_cur * TQ_N, f.n, qrb, (UINT64)tq_cur * TQ_N * 8);
  f.fence = fval + 1; f.pending = true; tq_cur = (tq_cur + 1) % TQ_FR;
}
void Exec12::ts_collect() {
  if (!tq_on) return;
  uint64_t done = fence->GetCompletedValue();
  for (int i = 0; i < TQ_FR; i++) {
    TFrame &f = tq[i];
    if (!f.pending || f.fence > done) continue;
    f.pending = false;
    const uint64_t *v = qrb_m + (size_t)i * TQ_N;
    bool ok = f.n >= 2 && v[0] != 0;
    for (uint32_t j = 1; ok && j < f.n; j++) ok = v[j] >= v[j - 1];
    if (!ok) { tq_bad++; continue; }
    for (uint32_t j = 0; j + 1 < f.n; j++) { PassT &p = tq_pass[f.key[j]]; p.ms += (double)(v[j + 1] - v[j]) * 1000.0 / tfreq; p.n++; }
    tq_ms.push_back((float)((double)(v[f.n - 1] - v[0]) * 1000.0 / tfreq)); tq_frames++;
  }
}
void Exec12::ts_report() {
  if (!tq_on) return;
  if (tq_ms.empty()) { d8_log("D3D12 gpu time: %u frames not measured (timestamps zero or out of order)", tq_bad); tq_bad = 0; return; }
  std::vector<float> v = tq_ms; std::sort(v.begin(), v.end()); double sum = 0; for (float x : v) sum += x;
  std::vector<std::pair<double, uint32_t>> ps; for (auto &e : tq_pass) ps.push_back({e.second.ms, e.first});
  std::sort(ps.rbegin(), ps.rend());
  char by[512] = {}; size_t at = 0;
  for (size_t i = 0; i < ps.size() && i < 6 && at < sizeof by - 64; i++) {
    uint32_t k = ps[i].second; const PassT &p = tq_pass[k];
    if (k == 0) at += snprintf(by + at, sizeof by - at, "%s(before any target) %.2f ms", i ? "; " : "", p.ms / tq_frames);
    else at += snprintf(by + at, sizeof by - at, "%s%ux%u%s %.2f ms (%.1f passes)", i ? "; " : "", (k >> 16) & 0x7FFF, k & 0xFFFF, (k & 0x80000000u) ? " depth only" : "",
                        p.ms / tq_frames, (double)p.n / tq_frames);
  }
  d8_log("D3D12 gpu time, %u frames: a frame's GPU work %.3f ms on average, 95%% %.3f, longest %.3f (%u not measured); by render target: %s",
         tq_frames, sum / v.size(), v[(size_t)(v.size() * 0.95)], v.back(), tq_bad, by);
  tq_ms.clear(); tq_pass.clear(); tq_frames = 0; tq_bad = 0;
}

// ---- pipeline state descriptions from the stream's Direct3D 11 ones ------------------------------------------------
static D3D12_PRIMITIVE_TOPOLOGY_TYPE topo_type(uint32_t t) {
  switch (t) {
  case D3D_PRIMITIVE_TOPOLOGY_POINTLIST: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
  case D3D_PRIMITIVE_TOPOLOGY_LINELIST: case D3D_PRIMITIVE_TOPOLOGY_LINESTRIP: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
  default: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
  }
}
static D3D12_BLEND_DESC blend12(const D3D11_BLEND_DESC &b) {
  D3D12_BLEND_DESC r = {}; r.AlphaToCoverageEnable = b.AlphaToCoverageEnable; r.IndependentBlendEnable = b.IndependentBlendEnable;
  for (int i = 0; i < 8; i++) {
    const D3D11_RENDER_TARGET_BLEND_DESC &s = b.RenderTarget[i]; D3D12_RENDER_TARGET_BLEND_DESC &d = r.RenderTarget[i];
    d.BlendEnable = s.BlendEnable; d.LogicOpEnable = FALSE; d.SrcBlend = (D3D12_BLEND)s.SrcBlend; d.DestBlend = (D3D12_BLEND)s.DestBlend;
    d.BlendOp = (D3D12_BLEND_OP)s.BlendOp; d.SrcBlendAlpha = (D3D12_BLEND)s.SrcBlendAlpha; d.DestBlendAlpha = (D3D12_BLEND)s.DestBlendAlpha;
    d.BlendOpAlpha = (D3D12_BLEND_OP)s.BlendOpAlpha; d.LogicOp = D3D12_LOGIC_OP_NOOP; d.RenderTargetWriteMask = s.RenderTargetWriteMask;
    // a description the D3D8 side left zero (no blending) still needs valid factors in Direct3D 12
    if (!d.SrcBlend) d.SrcBlend = D3D12_BLEND_ONE; if (!d.DestBlend) d.DestBlend = D3D12_BLEND_ZERO; if (!d.BlendOp) d.BlendOp = D3D12_BLEND_OP_ADD;
    if (!d.SrcBlendAlpha) d.SrcBlendAlpha = D3D12_BLEND_ONE; if (!d.DestBlendAlpha) d.DestBlendAlpha = D3D12_BLEND_ZERO; if (!d.BlendOpAlpha) d.BlendOpAlpha = D3D12_BLEND_OP_ADD;
  }
  return r;
}
static D3D12_DEPTH_STENCILOP_DESC sop12(const D3D11_DEPTH_STENCILOP_DESC &s) {
  D3D12_DEPTH_STENCILOP_DESC d = {(D3D12_STENCIL_OP)s.StencilFailOp, (D3D12_STENCIL_OP)s.StencilDepthFailOp, (D3D12_STENCIL_OP)s.StencilPassOp, (D3D12_COMPARISON_FUNC)s.StencilFunc};
  if (!d.StencilFailOp) d.StencilFailOp = D3D12_STENCIL_OP_KEEP; if (!d.StencilDepthFailOp) d.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
  if (!d.StencilPassOp) d.StencilPassOp = D3D12_STENCIL_OP_KEEP; if (!d.StencilFunc) d.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
  return d;
}
static D3D12_DEPTH_STENCIL_DESC depth12(const D3D11_DEPTH_STENCIL_DESC &s, bool has_ds) {
  D3D12_DEPTH_STENCIL_DESC d = {};
  d.DepthEnable = has_ds && s.DepthEnable; d.DepthWriteMask = (D3D12_DEPTH_WRITE_MASK)s.DepthWriteMask; d.DepthFunc = (D3D12_COMPARISON_FUNC)s.DepthFunc;
  if (!d.DepthFunc) d.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
  d.StencilEnable = has_ds && s.StencilEnable; d.StencilReadMask = s.StencilReadMask; d.StencilWriteMask = s.StencilWriteMask;
  d.FrontFace = sop12(s.FrontFace); d.BackFace = sop12(s.BackFace);
  return d;
}
static D3D12_RASTERIZER_DESC raster12(const D3D11_RASTERIZER_DESC &s) {
  D3D12_RASTERIZER_DESC d = {};
  d.FillMode = s.FillMode ? (D3D12_FILL_MODE)s.FillMode : D3D12_FILL_MODE_SOLID; d.CullMode = s.CullMode ? (D3D12_CULL_MODE)s.CullMode : D3D12_CULL_MODE_NONE;
  d.FrontCounterClockwise = s.FrontCounterClockwise; d.DepthBias = s.DepthBias; d.DepthBiasClamp = s.DepthBiasClamp;
  d.SlopeScaledDepthBias = s.SlopeScaledDepthBias; d.DepthClipEnable = s.DepthClipEnable; d.MultisampleEnable = s.MultisampleEnable;
  d.AntialiasedLineEnable = s.AntialiasedLineEnable; d.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
  return d;   // (ScissorEnable has no place here: Direct3D 12 always scissors, and the scissor is the whole target; d8_ff.cpp never enables it)
}
static D3D12_SAMPLER_DESC sampler12(const D3D11_SAMPLER_DESC *s) {
  D3D12_SAMPLER_DESC d = {};
  if (!s || !s->AddressU) {                                // a stage without a sampler: a valid one that nothing reads
    d.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT; d.AddressU = d.AddressV = d.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    d.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; d.MaxLOD = FLT_MAX; return d;
  }
  d.Filter = (D3D12_FILTER)s->Filter; d.AddressU = (D3D12_TEXTURE_ADDRESS_MODE)s->AddressU; d.AddressV = (D3D12_TEXTURE_ADDRESS_MODE)s->AddressV;
  d.AddressW = (D3D12_TEXTURE_ADDRESS_MODE)s->AddressW; d.MipLODBias = s->MipLODBias; d.MaxAnisotropy = s->MaxAnisotropy;
  d.ComparisonFunc = s->ComparisonFunc ? (D3D12_COMPARISON_FUNC)s->ComparisonFunc : D3D12_COMPARISON_FUNC_NEVER;
  for (int i = 0; i < 4; i++) d.BorderColor[i] = s->BorderColor[i];
  d.MinLOD = s->MinLOD; d.MaxLOD = s->MaxLOD;
  return d;
}

// ---- samplers ------------------------------------------------------------------------------------------------------------
void Exec12::write_samplers(uint32_t table, const D3D12_SAMPLER_DESC *sd) {
  D3D12_CPU_DESCRIPTOR_HANDLE h = gv_smp->GetCPUDescriptorHandleForHeapStart(); h.ptr += (SIZE_T)table * smp_inc;
  for (uint32_t i = 0; i < 8; i++) {
    D3D12_CPU_DESCRIPTOR_HANDLE hi = h; hi.ptr += (SIZE_T)i * smp_inc;
    if (!smp_copy) { dev->CreateSampler(&sd[i], hi); continue; }
    std::string k((const char *)&sd[i], sizeof sd[i]);
    auto it = smp_cpu.find(k); uint32_t ci;
    if (it != smp_cpu.end()) ci = it->second;
    else { ci = h_smp.take(dev); if (ci == ~0u) { dev->CreateSampler(&sd[i], hi); continue; } dev->CreateSampler(&sd[i], h_smp.at(ci)); smp_cpu[k] = ci; }
    dev->CopyDescriptorsSimple(1, hi, h_smp.at(ci), D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
  }
}
// Two pixels drawn through the same root signature, descriptor tables and sampler path as a draw: pixel 0 samples a 2x2
// black-and-white checker at its centre with a LINEAR / WRAP sampler (s0): the four texels' blend, about 128; pixel 1
// samples it at u = 1.25 with a POINT / CLAMP sampler (s1): the right column's top texel (white, 255), where WRAP would
// give the left's (black). Red channels into px. Nothing it makes is kept.
int Exec12::sampler_check(bool copy, uint32_t px[2]) {
  if (!compile || lost) return -1;
  static const char *src =
    "Texture2D t : register(t0); SamplerState s0 : register(s0); SamplerState s1 : register(s1);\n"
    "float4 vs(uint i : SV_VertexID) : SV_Position { return float4(i == 1 ? 3 : -1, i == 2 ? -3 : 1, 0, 1); }\n"
    "float4 ps(float4 p : SV_Position) : SV_Target { return p.x < 1 ? t.Sample(s0, float2(0.5, 0.5)) : t.Sample(s1, float2(1.25, 0.25)); }\n";
  ID3DBlob *vb = compile_hlsl(src, "vs", VS_T()), *pb = compile_hlsl(src, "ps", PS_T());
  if (!vb || !pb) { rel(vb); rel(pb); return -1; }
  bool was = smp_copy; smp_copy = copy;
  D3D12_HEAP_PROPERTIES hp = {}; hp.Type = D3D12_HEAP_TYPE_DEFAULT;
  D3D12_RESOURCE_DESC rd = {}; rd.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; rd.Width = 2; rd.Height = 2; rd.DepthOrArraySize = 1;
  rd.MipLevels = 1; rd.Format = DXGI_FORMAT_R8G8B8A8_UNORM; rd.SampleDesc.Count = 1;
  ID3D12Resource *tex = nullptr, *rt = nullptr, *rb = nullptr; ID3D12PipelineState *pso = nullptr;
  D3D12_RESOURCE_STATES ts = D3D12_RESOURCE_STATE_COPY_DEST, rs_ = D3D12_RESOURCE_STATE_RENDER_TARGET;
  uint32_t si = ~0u, ri = ~0u; int result = -1;
  do {
    if (FAILED(dev->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, ts, nullptr, __uuidof(ID3D12Resource), (void **)&tex))) break;
    rd.Height = 1; rd.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    if (FAILED(dev->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, rs_, nullptr, __uuidof(ID3D12Resource), (void **)&rt))) break;
    rb = readback_buffer12(dev, 512); if (!rb) break;
    uint8_t *c; D3D12_GPU_VIRTUAL_ADDRESS g; uint64_t off;
    if (!ring_alloc(512, D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT, &c, &g, &off)) break;
    static const uint32_t texels[4] = {0xFF000000u, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFF000000u};   // black white / white black
    memcpy(c, texels, 8); memcpy(c + 256, texels + 2, 8);
    D3D12_TEXTURE_COPY_LOCATION dl = {}, sl = {};
    dl.pResource = tex; dl.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    sl.pResource = ring; sl.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT; sl.PlacedFootprint.Offset = off;
    sl.PlacedFootprint.Footprint = {DXGI_FORMAT_R8G8B8A8_UNORM, 2, 2, 1, 256};
    cl->CopyTextureRegion(&dl, 0, 0, 0, &sl, nullptr);
    to(tex, ts, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE); barriers();
    si = h_srv.take(dev); ri = h_rtv.take(dev); if (si == ~0u || ri == ~0u) break;
    D3D12_SHADER_RESOURCE_VIEW_DESC v = srv_desc(DXGI_FORMAT_R8G8B8A8_UNORM, false, 0, 1); dev->CreateShaderResourceView(tex, &v, h_srv.at(si));
    dev->CreateRenderTargetView(rt, nullptr, h_rtv.at(ri));
    D3D12_GRAPHICS_PIPELINE_STATE_DESC gd = {};
    gd.pRootSignature = rs; gd.VS = {vb->GetBufferPointer(), vb->GetBufferSize()}; gd.PS = {pb->GetBufferPointer(), pb->GetBufferSize()};
    D3D11_BLEND_DESC b11 = {}; b11.RenderTarget[0].RenderTargetWriteMask = 0xF; gd.BlendState = blend12(b11);
    D3D11_RASTERIZER_DESC r11 = {}; r11.FillMode = D3D11_FILL_SOLID; r11.CullMode = D3D11_CULL_NONE; gd.RasterizerState = raster12(r11);
    D3D11_DEPTH_STENCIL_DESC d11 = {}; gd.DepthStencilState = depth12(d11, false);
    gd.SampleMask = 0xFFFFFFFFu; gd.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; gd.NumRenderTargets = 1;
    gd.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; gd.SampleDesc.Count = 1;
    if (FAILED(dev->CreateGraphicsPipelineState(&gd, __uuidof(ID3D12PipelineState), (void **)&pso))) break;
    // the tables, as a draw makes them
    if (smp_next + 8 > SMP_CAP) break;
    uint32_t tab = smp_next; smp_next += 8;
    D3D12_SAMPLER_DESC sd[8];
    for (int i = 0; i < 8; i++) sd[i] = sampler12(nullptr);
    sd[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;                          // s0: LINEAR, WRAP
    sd[1].AddressU = sd[1].AddressV = sd[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;   // s1: POINT, CLAMP
    write_samplers(tab, sd);
    uint64_t start = gv_head, phys = start % gv_cap; if (phys + 8 > gv_cap) { start += gv_cap - phys; phys = 0; } gv_head = start + 8;
    D3D12_CPU_DESCRIPTOR_HANDLE t0 = gv_srv->GetCPUDescriptorHandleForHeapStart(); t0.ptr += (SIZE_T)phys * gv_inc;
    for (uint32_t i = 0; i < 8; i++) { D3D12_CPU_DESCRIPTOR_HANDLE di = t0; di.ptr += (SIZE_T)i * gv_inc; dev->CopyDescriptorsSimple(1, di, h_srv.at(i ? null_srv2d : si), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV); }
    D3D12_CPU_DESCRIPTOR_HANDLE rh = h_rtv.at(ri); cl->OMSetRenderTargets(1, &rh, FALSE, nullptr);
    D3D12_VIEWPORT vp = {0, 0, 2, 1, 0, 1}; D3D12_RECT sc = {0, 0, 2, 1};
    cl->RSSetViewports(1, &vp); cl->RSSetScissorRects(1, &sc); cl->SetPipelineState(pso);
    D3D12_GPU_DESCRIPTOR_HANDLE gt = gv_srv->GetGPUDescriptorHandleForHeapStart(); gt.ptr += phys * gv_inc; cl->SetGraphicsRootDescriptorTable(3, gt);
    D3D12_GPU_DESCRIPTOR_HANDLE gs = gv_smp->GetGPUDescriptorHandleForHeapStart(); gs.ptr += (UINT64)tab * smp_inc; cl->SetGraphicsRootDescriptorTable(4, gs);
    cl->SetGraphicsRootConstantBufferView(0, ring_va); cl->SetGraphicsRootConstantBufferView(1, ring_va); cl->SetGraphicsRootConstantBufferView(2, ring_va);
    cl->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    cl->DrawInstanced(3, 1, 0, 0);
    to(rt, rs_, D3D12_RESOURCE_STATE_COPY_SOURCE); barriers();
    D3D12_TEXTURE_COPY_LOCATION rl = {}, ss = {};
    rl.pResource = rb; rl.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT; rl.PlacedFootprint.Footprint = {DXGI_FORMAT_R8G8B8A8_UNORM, 2, 1, 1, 256};
    ss.pResource = rt; ss.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    cl->CopyTextureRegion(&rl, 0, 0, 0, &ss, nullptr);
    submit(); wait(fval);
    uint8_t *m = nullptr;
    if (lost || FAILED(rb->Map(0, nullptr, (void **)&m))) break;
    px[0] = m[0]; px[1] = m[4];
    D3D12_RANGE none = {0, 0}; rb->Unmap(0, &none);
    result = (px[0] >= 100 && px[0] <= 155 ? 1 : 0) | (px[1] >= 200 ? 2 : 0);
  } while (0);
  if (si != ~0u) h_srv.give(si); if (ri != ~0u) h_rtv.give(ri);
  bury(tex); bury(rt); bury(rb); bury(pso);
  rel(vb); rel(pb);
  smp_tables.clear(); smp_cpu.clear(); smp_next = 0; memo.smp_ok = false;   // (the check's samplers are not the stream's)
  if (result < 0) smp_copy = was;
  bd = Bound(); bd.valid = true; tgt_dirty = true;
  return result;
}

// ---- draw merging -----------------------------------------------------------------------------------------------------
Exec12::Res *Exec12::eff_stream(const XDraw &d, int s, uint32_t *stride) {
  if (!d.stream[s].buf) return nullptr;
  bool sk = false; for (int e = 0; e < d.vk.nel; e++) sk |= d.vk.el[e].sem == SE_SKIN && d.vk.el[e].stream == s;
  Res *r = res_of(d.stream[s].buf);
  if (!r) return nullptr;
  if (sk) { if (!r->skin_src || !r->pal) return nullptr; *stride = (uint32_t)sizeof(XSkinVtx); return res_of(r->skin_src); }
  *stride = d.stream[s].stride; return r;
}
bool Exec12::mergeable(const XDraw &d) {
  if (d.kind != XD_INDEXED || d.topology != (uint32_t)D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST || d.raster.FillMode != D3D11_FILL_SOLID || !d.count || !d.stream[0].buf) return false;
  for (int s = 0; s < 16; s++) {
    if (!d.stream[s].buf) continue;
    uint32_t st; Res *r = eff_stream(d, s, &st);
    if (s > 3 || !st || !r || r->kind != 3 || (!r->dyn && !r->r)) return false;
    if ((uint64_t)(r->dyn ? r->shadow.size() : r->w) > sa_cap() / 4) return false;
  }
  Res *ib = res_of(d.ib);
  if (!ib || !ib->index || ib->shadow.empty()) return false;
  return ((uint64_t)d.start + d.count) * (d.ib32 ? 4u : 2u) <= ib->w;
}
bool Exec12::same_run(const XDraw &first, const XDraw &d) {
  if (!mergeable(d) || d.ib32 != first.ib32) return false;
  for (int s = 0; s < 16; s++) if (!d.stream[s].buf != !first.stream[s].buf || d.stream[s].stride != first.stream[s].stride) return false;
  for (int e = 0; e < d.vk.nel; e++) if (d.vk.el[e].sem == SE_SKIN) {     // one palette for the run (b1 holds one)
    Res *a = res_of(first.stream[d.vk.el[e].stream].buf), *b = res_of(d.stream[d.vk.el[e].stream].buf);
    if (!a || !b || !a->pal || !b->pal || a->pal->serial != b->pal->serial) return false;
  }
  return true;
}
void Exec12::flush_run() {
  uint32_t n = run_n; run_n = 0;
  if (!n) return;
  std::vector<uint8_t> first(sizeof(XDraw)); memcpy(first.data(), run_head.data(), XDRAW_HEAD); memcpy(first.data() + XDRAW_HEAD, run_tails.data(), XDRAW_TAIL);
  const XDraw &d = *(const XDraw *)first.data();
  const XDraw &last = *(const XDraw *)(first.data());
  (void)last;
  MergeIn mi = {run_tails.data(), n};
  mg_fell = false;
  draw(d, nullptr, n > 1 ? &mi : nullptr);
  o.draws += n;
  if (n > 1 && !mg_fell) { mg_runs++; mg_saved += n - 1; mg_total_runs++; mg_total_saved += n - 1; }
  { XDraw tl; memcpy(&tl.stream, run_tails.data() + (size_t)(n - 1) * XDRAW_TAIL, XDRAW_TAIL); last_site = o.last_site = tl.site; }
}

// ---- a draw --------------------------------------------------------------------------------------------------------------
void Exec12::draw(const XDraw &d, const uint8_t *data, const MergeIn *mi) {
  if (!compile) return;
  // shaders (the Direct3D 11 executor's HLSL), compiled once per key
  // the last draw's shaders, pipeline state and sampler table, when this draw's keys equal its (most draws: no lookup)
  VSh *Vp = nullptr; PSh *Pp = nullptr;
  if (memo.vk_ok && !memcmp(&memo.vk, &d.vk, sizeof d.vk)) Vp = memo.v;
  if (memo.pk_ok && !memcmp(&memo.pk, &d.pk, sizeof d.pk)) Pp = memo.p;
  std::string vks, pks;
  if (!Vp) vks.assign((const char *)&d.vk, sizeof d.vk);
  if (!Pp) pks.assign((const char *)&d.pk, sizeof d.pk);
  auto vi = Vp ? vsh.end() : vsh.find(vks);
  if (!Vp && vi == vsh.end()) {
    VSh v; v.id = (uint32_t)vsh.size() + 1; v.code = compile_hlsl(gen_vs(d.vk), "main", VS_T());
    static const char *sn[] = {"POSITION", "POSITION", "NORMAL", "COLOR", "TEXCOORD", "PSIZE", "BLENDWEIGHT"};
    for (int i = 0; i < d.vk.nel; i++) {
      const VElem &e = d.vk.el[i];
      if (e.sem == SE_SKIN) {                              // gpu skinning: the source vertex's five parts (XSkinVtx)
        v.il.push_back({"SKINA", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, e.stream, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0});
        v.il.push_back({"SKINB", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, e.stream, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0});
        v.il.push_back({"SKINNA", 0, DXGI_FORMAT_R32G32B32_FLOAT, e.stream, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0});
        v.il.push_back({"SKINNB", 0, DXGI_FORMAT_R32G32B32_FLOAT, e.stream, 44, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0});
        v.il.push_back({"SKINI", 0, DXGI_FORMAT_R16G16_UINT, e.stream, 56, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0});
        continue;
      }
      v.il.push_back({sn[e.sem], e.sem == SE_POST ? 0u : e.idx, vdxgi(e.fmt), e.stream, e.off, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0});
    }
    vi = vsh.emplace(vks, std::move(v)).first;
  }
  if (!Vp) {
    Vp = &vi->second; memo.vk = d.vk; memo.v = Vp; memo.vk_ok = true;
    if (!Vp->h && Vp->code) {                                  // the code's and the input layout's hash (the pipeline library's names)
      uint64_t h = fnv64(Vp->code->GetBufferPointer(), Vp->code->GetBufferSize());
      for (auto &e : Vp->il) { h = fnv64(e.SemanticName, strlen(e.SemanticName), h); h = fnv64(&e.SemanticIndex, sizeof(UINT) * 4, h); h = fnv64(&e.Format, sizeof e.Format, h); }
      Vp->h = h | 1;
    }
  }
  if (!Pp) {
    auto pi = psh.find(pks);
    if (pi == psh.end()) { PSh s; s.id = (uint32_t)psh.size() + 1; s.code = compile_hlsl(gen_ps(d.pk), "main", PS_T()); pi = psh.emplace(pks, s).first; }
    Pp = &pi->second; memo.pk = d.pk; memo.p = Pp; memo.pk_ok = true;
    if (!Pp->h && Pp->code) Pp->h = fnv64(Pp->code->GetBufferPointer(), Pp->code->GetBufferSize()) | 1;
  }
  VSh &V = *Vp; PSh &P = *Pp;
  if (!V.code || !P.code) return;

  // ---- what may wait for the GPU comes first: the sampler table, then room in the ring and the descriptor ring. A wait
  // that submits re-opens the list, so everything this draw binds is bound after these
  uint32_t ns = std::min<uint32_t>(d.nstages, 8);
  D3D11_SAMPLER_DESC skd[8]; memset(skd, 0, sizeof skd);
  for (uint32_t i = 0; i < ns; i++) if (d.stage[i].tex) skd[i] = d.stage[i].sampler;
  uint32_t smp_tab;
  const bool fresh = diag("fresh");
  if (!fresh && memo.smp_ok && !memcmp(memo.skd, skd, sizeof skd)) smp_tab = memo.smp;
  else {
  std::string sk((const char *)skd, sizeof skd);
  auto st = fresh ? smp_tables.end() : smp_tables.find(sk);
  if (st != smp_tables.end()) smp_tab = st->second;
  else {
    if (smp_next + 8 > SMP_CAP) { wait_idle(); smp_tables.clear(); smp_next = 0; memo.smp_ok = false; d8_once("smpfull12", "D3D12: the sampler heap was full: waited for the GPU and started it again"); }
    smp_tab = smp_next; smp_next += 8;
    D3D12_SAMPLER_DESC sd[8];
    for (uint32_t i = 0; i < 8; i++) sd[i] = sampler12(i < ns && d.stage[i].tex ? &d.stage[i].sampler : nullptr);
    write_samplers(smp_tab, sd);
    smp_tables[sk] = smp_tab;
  }
  memcpy(memo.skd, skd, sizeof skd); memo.smp = smp_tab; memo.smp_ok = true;
  }
  int sks = -1; for (int e = 0; e < d.vk.nel; e++) if (d.vk.el[e].sem == SE_SKIN) sks = d.vk.el[e].stream;
  { uint64_t need = 2048 + 256 + 512;                      // the constants
    if (sks >= 0) need += 64u * SKIN_BONES + 256;
    if (d.kind == XD_UP || d.kind == XD_UP_INDEXED) need += d.up_vbytes + d.ibytes + 512;
    if (d.kind == XD_INLINE_INDEXED) need += d.ibytes + 256;
    for (UINT i = 0; i < 16; i++) if (d.stream[i].buf) { Res *r = res_of(d.stream[i].buf); if (r && r->dyn && r->epoch != submits) need += r->shadow.size() + 256; }
    if (d.kind == XD_INDEXED) { Res *r = res_of(d.ib); if (r && r->dyn && r->epoch != submits) need += r->shadow.size() + 256; }
    if (sks >= 0) { Res *r = res_of(d.stream[sks].buf); if (r && r->skin_src) { Res *s = res_of(r->skin_src); if (s && s->dyn) need += s->shadow.size() + 256; } }
    if (mi) {                                              // the run's indices, and its dynamic sources' copies
      XDraw t; memcpy(&t, &d, XDRAW_HEAD);
      for (uint32_t j = 0; j < mi->n; j++) {
        memcpy(&t.stream, mi->tails + (size_t)j * XDRAW_TAIL, XDRAW_TAIL); need += (uint64_t)t.count * 4 + 16;
        for (int s = 0; s < 4; s++) { uint32_t st; Res *r = eff_stream(t, s, &st); if (r && r->dyn && r->epoch != submits) need += r->shadow.size() + 256; }
      }
    }
    if (need > ring_cap / 4) { d8_once("drawbig12", "D3D12: a draw needs %llu bytes of the ring, more than a quarter of it (skipped)", (unsigned long long)need); return; }
    // room for all of it (twice, for the wrap): waits here, not between this draw's allocations
    for (;;) {
      ring_retire();
      if (ring_cap - (ring_head - ring_tail) >= 2 * need) break;
      if (lost) return;
      ring_waits++;
      if (ring_marks.empty()) submit();
      wait(ring_marks.front().fence);
    }
    for (;;) {
      ring_retire();
      if (gv_cap - (gv_head - gv_tail) >= 16) break;
      if (lost) return;
      if (gv_marks.empty()) submit();
      wait(gv_marks.front().fence);
    }
  }
  // ---- draw merging: each draw's vertex slot (its buffers copied there once per version); may wait for the GPU when the
  // slot buffers are full, so before anything is bound
  std::vector<uint32_t> mslot;
  if (mi) {
    if (!sa[0]) {
      D3D12_HEAP_PROPERTIES hp = {}; hp.Type = D3D12_HEAP_TYPE_DEFAULT;
      D3D12_RESOURCE_DESC rd = {}; rd.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER; rd.Width = sa_cap(); rd.Height = 1; rd.DepthOrArraySize = 1;
      rd.MipLevels = 1; rd.SampleDesc.Count = 1; rd.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
      for (int i = 0; i < 4; i++) {
        sa_st[i] = D3D12_RESOURCE_STATE_COPY_DEST;
        if (FAILED(dev->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, sa_st[i], nullptr, __uuidof(ID3D12Resource), (void **)&sa[i]))) {
          for (int k = 0; k < 4; k++) rel(sa[k]);
          d8_log("draw_merge (D3D12): the slot buffers (4 x %u KB) could not be made: off", (unsigned)(sa_cap() >> 10)); merge_on = false; break;
        }
      }
      if (sa[0]) d8_log("draw_merge (D3D12): four %u KB slot buffers for vertex streams 0 to 3", (unsigned)(sa_cap() >> 10));
    }
    const uint64_t cap = sa_cap();
    XDraw t; memcpy(&t, &d, XDRAW_HEAD);
    for (int attempt = 0; sa[0] && attempt < 2 && mslot.size() < mi->n; attempt++) {
      mslot.clear(); bool full = false;
      // a test (D8ON11_TEST_SLOT_WRAP=N): every Nth merged run finds the slots full, so they start again
      { static long tw = getenv("D8ON11_TEST_SLOT_WRAP") ? atol(getenv("D8ON11_TEST_SLOT_WRAP")) : 0; static long tc = 0;
        if (tw > 0 && attempt == 0 && ++tc % tw == 0) full = true; }
      std::vector<std::pair<int, std::pair<Res *, uint64_t>>> copies;   // (stream, (source, its slot's byte offset))
      for (uint32_t j = 0; j < mi->n && !full; j++) {
        memcpy(&t.stream, mi->tails + (size_t)j * XDRAW_TAIL, XDRAW_TAIL);
        SlotKey key; memset(&key, 0, sizeof key); Res *rs[4] = {}; uint32_t st[4] = {}; uint64_t L = 0, sz[4] = {};
        for (int s = 0; s < 4; s++) {
          rs[s] = eff_stream(t, s, &st[s]); if (!rs[s]) continue;
          key.v[s * 3] = rs[s]->serial; key.v[s * 3 + 1] = rs[s]->ver; key.v[s * 3 + 2] = st[s];
          sz[s] = rs[s]->dyn ? rs[s]->shadow.size() : rs[s]->w; L = std::max<uint64_t>(L, (sz[s] + st[s] - 1) / st[s]);
        }
        auto f = slots.find(key);
        if (f != slots.end()) { mslot.push_back(f->second); continue; }
        uint64_t v = 0;
        for (int s = 0; s < 4; s++) if (rs[s]) v = std::max<uint64_t>(v, (sa_pos[s] + st[s] - 1) / st[s]);
        for (int s = 0; s < 4; s++) if (rs[s] && (v + L) * st[s] > cap) full = true;
        if (full) break;
        for (int s = 0; s < 4; s++) if (rs[s]) { copies.push_back({s, {rs[s], v * st[s]}}); sa_pos[s] = (v + L) * st[s]; }
        slots[key] = (uint32_t)v; mslot.push_back((uint32_t)v);
      }
      if (full) {                                          // the slot buffers are full: start them again (once the GPU is done)
        wait_idle(); slots.clear(); for (auto &x : sa_pos) x = 0; mg_wraps++; mslot.clear(); continue;
      }
      if (!copies.empty()) {
        for (int s = 0; s < 4; s++) to(sa[s], sa_st[s], D3D12_RESOURCE_STATE_COPY_DEST);
        for (auto &c : copies) { Res *r = c.second.first; if (!r->dyn) to(*r, ST_VBIB); }
        barriers();
        for (auto &c : copies) {
          Res *r = c.second.first; uint64_t bytes = r->dyn ? r->shadow.size() : r->w;
          if (r->dyn) { D3D12_GPU_VIRTUAL_ADDRESS va; if (dyn_view(*r, &va)) cl->CopyBufferRegion(sa[c.first], c.second.second, ring, va - ring_va, bytes); }
          else cl->CopyBufferRegion(sa[c.first], c.second.second, r->r, 0, bytes);
          mg_bytes += bytes;
        }
      }
    }
    if (mslot.size() < mi->n) {                            // could not place the run: its draws one by one
      d8_once("mergeplace12", "draw_merge (D3D12): a run did not fit the slot buffers: drawn one by one");
      XDraw t2; memcpy(&t2, &d, XDRAW_HEAD);
      for (uint32_t j = 0; j < mi->n; j++) { memcpy(&t2.stream, mi->tails + (size_t)j * XDRAW_TAIL, XDRAW_TAIL); draw(t2, nullptr, nullptr); }
      mg_fell = true; return;
    }
    for (int s = 0; s < 4; s++) to(sa[s], sa_st[s], D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
  }
  bind_targets();

  // ---- the pipeline state, cached by everything it is made of
  struct PKey { uint32_t vs, ps; D3D11_BLEND_DESC b; D3D11_DEPTH_STENCIL_DESC ds; D3D11_RASTERIZER_DESC r; uint32_t tt, rtf, dsf, nrt; } k;
  static_assert(sizeof(PKey) <= sizeof(memo.pkey), "the pipeline key fits the memo");
  memset(&k, 0, sizeof k);
  k.vs = V.id; k.ps = P.id; k.b = d.blend; k.ds = d.depth; k.r = d.raster; k.tt = topo_type(d.topology); k.rtf = rt_fmt; k.dsf = ds_fmt; k.nrt = has_rtv;
  ID3D12PipelineState *pso;
  if (memo.pso_ok && !memcmp(&memo.pkey, &k, sizeof k)) pso = memo.pso;
  else {
  std::string pk((const char *)&k, sizeof k);
  auto pit = psos.find(pk);
  if (pit != psos.end()) pso = pit->second;
  else {
    D3D12_GRAPHICS_PIPELINE_STATE_DESC g = {};
    g.pRootSignature = rs; g.VS = {V.code->GetBufferPointer(), V.code->GetBufferSize()}; g.PS = {P.code->GetBufferPointer(), P.code->GetBufferSize()};
    g.BlendState = blend12(d.blend); g.SampleMask = 0xFFFFFFFFu; g.RasterizerState = raster12(d.raster);
    g.DepthStencilState = depth12(d.depth, has_dsv); g.InputLayout = {V.il.data(), (UINT)V.il.size()};
    g.PrimitiveTopologyType = (D3D12_PRIMITIVE_TOPOLOGY_TYPE)k.tt; g.NumRenderTargets = has_rtv ? 1 : 0;
    if (has_rtv) g.RTVFormats[0] = rt_fmt;
    g.DSVFormat = has_dsv ? ds_fmt : DXGI_FORMAT_UNKNOWN; g.SampleDesc.Count = 1;
    pso = make_pso(g, V.h, P.h);
    psos[pk] = pso; pso_made++;
    if (pso_made == 256 || pso_made == 1024 || pso_made == 4096) d8_log("D3D12: %llu pipeline states made so far", (unsigned long long)pso_made);
  }
  memcpy(memo.pkey, &k, sizeof k); memo.pso = pso; memo.pso_ok = true;
  }
  if (!pso) return;

  // ---- constants (equal to the last draw's in this submission: the same place)
  if (last_c_epoch != submits || memcmp(&last_vc, &d.vc, sizeof d.vc)) {
    uint8_t *c; D3D12_GPU_VIRTUAL_ADDRESS g; ring_alloc(2048, 256, &c, &g); memcpy(c, &d.vc, sizeof d.vc); last_vc = d.vc; last_vca = g;
  }
  if (last_c_epoch != submits || memcmp(&last_pc, &d.pc, sizeof d.pc)) {
    uint8_t *c; D3D12_GPU_VIRTUAL_ADDRESS g; ring_alloc(256, 256, &c, &g); memcpy(c, &d.pc, sizeof d.pc); last_pc = d.pc; last_pca = g;
  }
  last_c_epoch = submits;

  // ---- textures: each stage's view (or the null one), into a table of eight; the same views as the last draw's: its table
  uint32_t idx[8];
  for (uint32_t i = 0; i < 8; i++) {
    bool cube = i < ns && d.pk.s[i].tex == 2;
    idx[i] = cube ? null_srvcube : null_srv2d;
    if (i >= ns || !d.stage[i].tex) continue;
    Res *r = res_of(d.stage[i].tex);
    if (!r || !r->r || r->srv == ~0u) continue;
    if (r->r == rt_res) { d8_once("rtsample12", "a draw reads the texture it draws into: read as no texture (as Direct3D 11 does)"); continue; }
    uint32_t f0 = d.stage[i].first_level, c = d.stage[i].nlevels;
    if (f0 == 0 && (c == 0 || c >= r->levels)) idx[i] = r->srv;
    else if (f0 < r->levels) {
      if (c == 0 || f0 + c > r->levels) c = r->levels - f0;
      uint32_t &v = r->srv_levels[f0 * 256 + c];
      if (!v) {
        v = h_srv.take(dev);
        D3D12_SHADER_RESOURCE_VIEW_DESC vd = srv_desc(r->fmt, r->faces == 6, f0, c);
        if (v != ~0u) dev->CreateShaderResourceView(r->r, &vd, h_srv.at(v));
      }
      if (v != ~0u) idx[i] = v;
    }
    to(*r, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
  }
  uint64_t srv_tab;
  if (!diag("fresh") && last_srv_epoch == submits && !memcmp(idx, last_srv_ids, sizeof idx)) srv_tab = last_srv_tab;
  else {
    uint64_t start = gv_head, phys = start % gv_cap;
    if (phys + 8 > gv_cap) { start += gv_cap - phys; phys = 0; }
    gv_head = start + 8; srv_tab = phys;
    D3D12_CPU_DESCRIPTOR_HANDLE dst = gv_srv->GetCPUDescriptorHandleForHeapStart(); dst.ptr += (SIZE_T)phys * gv_inc;
    for (uint32_t i = 0; i < 8; i++) {
      D3D12_CPU_DESCRIPTOR_HANDLE di = dst; di.ptr += (SIZE_T)i * gv_inc;
      dev->CopyDescriptorsSimple(1, di, h_srv.at(idx[i]), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }
    memcpy(last_srv_ids, idx, sizeof idx); last_srv_tab = srv_tab; last_srv_epoch = submits;
  }

  // ---- buffers
  D3D12_VERTEX_BUFFER_VIEW vb[16] = {}; UINT nvb = 0;
  D3D12_INDEX_BUFFER_VIEW ib = {}; bool use_ib = false;
  auto view_of = [&](Res &r, D3D12_GPU_VIRTUAL_ADDRESS *va, UINT *size) -> bool {
    if (r.dyn) { *size = (UINT)r.shadow.size(); return dyn_view(r, va); }
    if (!r.r) return false;
    to(r, ST_VBIB); *va = r.va; *size = (r.w + 15) & ~15u; return true;
  };
  if (sks >= 0 && (d.kind == XD_UP || d.kind == XD_UP_INDEXED)) sks = -1;
  uint32_t mcount = 0;
  if (mi) {                                                // the run: the slot buffers, and its indices rewritten into the ring
    for (int s = 0; s < 4; s++) if (d.stream[s].buf) {
      vb[s] = {sa[s]->GetGPUVirtualAddress(), (UINT)sa_cap(), s == sks ? (UINT)sizeof(XSkinVtx) : d.stream[s].stride}; nvb = std::max<UINT>(nvb, s + 1);
    }
    XDraw t; memcpy(&t, &d, XDRAW_HEAD);
    for (uint32_t j = 0; j < mi->n; j++) { memcpy(&t.stream, mi->tails + (size_t)j * XDRAW_TAIL, XDRAW_TAIL); mcount += t.count; }
    uint8_t *c; D3D12_GPU_VIRTUAL_ADDRESS g;
    if (!ring_alloc((uint64_t)mcount * 4, 16, &c, &g)) return;
    uint32_t *out = (uint32_t *)c;
    for (uint32_t j = 0; j < mi->n; j++) {
      memcpy(&t.stream, mi->tails + (size_t)j * XDRAW_TAIL, XDRAW_TAIL);
      Res *ib2 = res_of(t.ib); if (!ib2) return;
      int32_t add = (int32_t)t.base + (int32_t)mslot[j];
      if (t.ib32) { const uint32_t *ix = (const uint32_t *)ib2->shadow.data() + t.start; for (uint32_t k = 0; k < t.count; k++) *out++ = ix[k] + add; }
      else { const uint16_t *ix = (const uint16_t *)ib2->shadow.data() + t.start; for (uint32_t k = 0; k < t.count; k++) *out++ = (uint32_t)(ix[k] + add); }
    }
    ib = {g, mcount * 4, DXGI_FORMAT_R32_UINT}; use_ib = true;
    if (sks >= 0) {                                        // the run's palette (one for all of it, same_run)
      Res *st2 = res_of(d.stream[sks].buf);
      if (!st2 || !st2->pal) return;
      if (last_pal_epoch != submits || last_pal != st2->pal->serial) {
        uint8_t *pc; D3D12_GPU_VIRTUAL_ADDRESS pg; ring_alloc(64u * SKIN_BONES, 256, &pc, &pg);
        memcpy(pc, st2->pal->d.data(), st2->pal->d.size()); last_pal = st2->pal->serial; last_pal_epoch = submits; last_pal_va = pg;
      }
    }
    sks = sks >= 0 ? sks : -1;
  }
  if (mi) {}
  else if (sks >= 0) {                                     // gpu skinning: stream sks reads the mesh's source; the palette in b1
    Res *st2 = res_of(d.stream[sks].buf); Res *src = st2 && st2->skin_src ? res_of(st2->skin_src) : nullptr;
    if (!src || !st2->pal) { d8_once("skindraw12", "a skinned draw whose buffer stands for no mesh (skipped)"); return; }
    D3D12_GPU_VIRTUAL_ADDRESS va; UINT size;
    if (!view_of(*src, &va, &size)) return;
    vb[sks] = {va, size, (UINT)sizeof(XSkinVtx)}; nvb = std::max<UINT>(nvb, sks + 1);
    if (last_pal_epoch != submits || last_pal != st2->pal->serial) {
      uint8_t *c; D3D12_GPU_VIRTUAL_ADDRESS g; ring_alloc(64u * SKIN_BONES, 256, &c, &g);
      memcpy(c, st2->pal->d.data(), st2->pal->d.size()); last_pal = st2->pal->serial; last_pal_epoch = submits; last_pal_va = g;
    }
  }
  if (mi) {}
  else if (d.kind == XD_UP || d.kind == XD_UP_INDEXED) {
    uint8_t *c; D3D12_GPU_VIRTUAL_ADDRESS g;
    if (!ring_alloc(d.up_vbytes, 16, &c, &g)) { d8_once("upbig12", "a ...UP draw larger than the ring: skipped"); return; }
    memcpy(c, data, d.up_vbytes);
    vb[0] = {g, d.up_vbytes, d.up_stride}; nvb = std::max<UINT>(nvb, 1);
    if (d.kind == XD_UP_INDEXED) {
      if (!ring_alloc(d.ibytes, 16, &c, &g)) return;
      memcpy(c, data + d.up_vbytes, d.ibytes);
      ib = {g, d.ibytes, d.i32 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT}; use_ib = true;
    }
  } else {
    for (UINT i = 0; i < 16; i++) {
      if (!d.stream[i].buf || (int)i == sks) continue;
      Res *r = res_of(d.stream[i].buf);
      D3D12_GPU_VIRTUAL_ADDRESS va; UINT size;
      if (!r || !view_of(*r, &va, &size)) { d8_once("nostream12", "a draw reads a stream whose buffer is gone (skipped)"); return; }
      vb[i] = {va, size, d.stream[i].stride}; nvb = std::max<UINT>(nvb, i + 1);
    }
    if (d.kind == XD_INLINE_INDEXED) {
      uint8_t *c; D3D12_GPU_VIRTUAL_ADDRESS g;
      if (!ring_alloc(d.ibytes, 16, &c, &g)) return;
      memcpy(c, data, d.ibytes);
      ib = {g, d.ibytes, d.i32 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT}; use_ib = true;
    } else if (d.kind == XD_INDEXED) {
      Res *r = res_of(d.ib); D3D12_GPU_VIRTUAL_ADDRESS va; UINT size;
      if (!r || !view_of(*r, &va, &size)) { d8_once("noib12", "an indexed draw whose index buffer is gone (skipped)"); return; }
      ib = {va, size, d.ib32 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT}; use_ib = true;
    }
  }

  // ---- bind what differs from what the list has, and draw
  barriers();
  c_draws++;
  if (bd.pso != pso) { cl->SetPipelineState(pso); bd.pso = pso; c_pso++; }
  if (bd.vcb != last_vca) { cl->SetGraphicsRootConstantBufferView(0, last_vca); bd.vcb = last_vca; }
  D3D12_GPU_VIRTUAL_ADDRESS bones = sks >= 0 ? last_pal_va : last_vca;   // (b1 is read only by a skinning shader)
  if (bd.bones != bones) { cl->SetGraphicsRootConstantBufferView(1, bones); bd.bones = bones; }
  if (bd.pcb != last_pca) { cl->SetGraphicsRootConstantBufferView(2, last_pca); bd.pcb = last_pca; }
  if (bd.srv_tab != srv_tab) { D3D12_GPU_DESCRIPTOR_HANDLE h = gv_srv->GetGPUDescriptorHandleForHeapStart(); h.ptr += srv_tab * gv_inc; cl->SetGraphicsRootDescriptorTable(3, h); bd.srv_tab = srv_tab; c_srvtab++; }
  if (bd.smp_tab != smp_tab) { D3D12_GPU_DESCRIPTOR_HANDLE h = gv_smp->GetGPUDescriptorHandleForHeapStart(); h.ptr += (UINT64)smp_tab * smp_inc; cl->SetGraphicsRootDescriptorTable(4, h); bd.smp_tab = smp_tab; c_smptab++; }
  D3D12_VIEWPORT vp; memcpy(&vp, &d.viewport, sizeof vp);
  if (memcmp(&bd.vp, &vp, sizeof vp)) { cl->RSSetViewports(1, &vp); bd.vp = vp; }
  D3D12_RECT sc = {0, 0, (LONG)(rt_w ? rt_w : width), (LONG)(rt_h ? rt_h : height)};
  if (memcmp(&bd.sc, &sc, sizeof sc)) { cl->RSSetScissorRects(1, &sc); bd.sc = sc; }
  if (bd.topo != d.topology) { cl->IASetPrimitiveTopology((D3D12_PRIMITIVE_TOPOLOGY)d.topology); bd.topo = d.topology; }
  if (bd.sref != d.stencil_ref) { cl->OMSetStencilRef(d.stencil_ref); bd.sref = d.stencil_ref; }
  for (UINT i = 0; i < nvb; i++) if (memcmp(&bd.vb[i], &vb[i], sizeof vb[i])) { cl->IASetVertexBuffers(i, 1, &vb[i]); bd.vb[i] = vb[i]; }
  if (use_ib && memcmp(&bd.ib, &ib, sizeof ib)) { cl->IASetIndexBuffer(&ib); bd.ib = ib; }
  if (mi) { cl->DrawIndexedInstanced(mcount, 1, 0, 0, 0); if (diag("sync")) wait_idle(); return; }
  struct SyncAfter { Exec12 *x; ~SyncAfter() { if (diag("sync")) x->wait_idle(); } } sync_after{this};
  switch (d.kind) {
  case XD_DRAW: cl->DrawInstanced(d.count, 1, d.start, 0); break;
  case XD_UP: cl->DrawInstanced(d.count, 1, 0, 0); break;
  case XD_UP_INDEXED: cl->DrawIndexedInstanced(d.count, 1, 0, 0, 0); break;
  case XD_INLINE_INDEXED: cl->DrawIndexedInstanced(d.count, 1, 0, (INT)d.base, 0); break;
  default: cl->DrawIndexedInstanced(d.count, 1, d.start, (INT)d.base, 0); break;
  }
}

// ---- clears and copies -----------------------------------------------------------------------------------------------------
// D3D8's Clear, rectangles or not: Direct3D 12's clears take the rectangles themselves (target pixels, as XRect)
void Exec12::clear(const XClear &c, const XRect *r) {
  bind_targets(); barriers();
  const D3D12_RECT *rects = c.nrects ? (const D3D12_RECT *)r : nullptr;
  static_assert(sizeof(D3D12_RECT) == sizeof(XRect), "XRect is a D3D12_RECT");
  if ((c.flags & D3DCLEAR_TARGET) && has_rtv) {
    float f[4] = {((c.color >> 16) & 255) / 255.0f, ((c.color >> 8) & 255) / 255.0f, (c.color & 255) / 255.0f, (c.color >> 24) / 255.0f};
    cl->ClearRenderTargetView(cur_rtv, f, c.nrects, rects);
  }
  UINT df = ((c.flags & D3DCLEAR_ZBUFFER) ? D3D12_CLEAR_FLAG_DEPTH : 0) | ((c.flags & D3DCLEAR_STENCIL) ? D3D12_CLEAR_FLAG_STENCIL : 0);
  if (df && has_dsv) cl->ClearDepthStencilView(cur_dsv, (D3D12_CLEAR_FLAGS)df, c.z, (UINT8)c.stencil, c.nrects, rects);
}
static UINT sub12(const Exec12::Res &x, const XRef &r) { return r.level + r.face * x.levels; }
void Exec12::copy(const XCopy &c) {
  Res *s = res_of(c.src.id), *d = res_of(c.dst.id);
  if (!s || !d || !s->r || !d->r) return;
  D3D12_BOX bx = {(UINT)c.left, (UINT)c.top, 0, (UINT)c.right, (UINT)c.bottom, 1};
  // CopyTextureRegion, like Direct3D 11's copy, needs formats of one group: only the back buffer (B8G8R8A8) and an
  // X8R8G8B8 surface (B8G8R8X8) differ (d8_exec.cpp, X_COPY), and go through a draw
  if (s->fmt != d->fmt) { copy_draw(*d, sub12(*d, c.dst), c.x, c.y, *s, sub12(*s, c.src), bx); return; }
  to(*s, D3D12_RESOURCE_STATE_COPY_SOURCE); to(*d, D3D12_RESOURCE_STATE_COPY_DEST); barriers();
  D3D12_TEXTURE_COPY_LOCATION dl = {}, sl = {};
  dl.pResource = d->r; dl.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX; dl.SubresourceIndex = sub12(*d, c.dst);
  sl.pResource = s->r; sl.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX; sl.SubresourceIndex = sub12(*s, c.src);
  cl->CopyTextureRegion(&dl, c.x, c.y, 0, &sl, &bx);
}
// the texels through two scratch textures (as d8_exec.cpp's copy_draw): the source's format, copied into and read with Load;
// the destination's, drawn into and copied out
void Exec12::copy_draw(Res &dst, UINT dsub, UINT x, UINT y, Res &src, UINT ssub, const D3D12_BOX &bx) {
  if (!compile) return;
  if (!cpy_vs) {
    const char *s =
      "Texture2D t : register(t0);\n"
      "float4 vs(uint id : SV_VertexID) : SV_Position { return float4((id & 1) ? 1 : -1, (id >> 1) ? -1 : 1, 0, 1); }\n"
      "float4 ps(float4 p : SV_Position) : SV_Target { return t.Load(int3(p.xy, 0)); }\n";
    cpy_vs = compile_hlsl(s, "vs", VS_T()); cpy_ps = compile_hlsl(s, "ps", PS_T());
  }
  if (!cpy_vs || !cpy_ps) return;
  UINT w = bx.right - bx.left, h = bx.bottom - bx.top;
  auto scratch = [&](uint32_t key, DXGI_FORMAT f, bool rt) -> Res * {
    Res &r = res[key];                                    // (ids from 0xFFFFFF00 up are this executor's; the D3D8 side's are smaller)
    if (r.r && r.w >= w && r.h >= h && r.fmt == f) return &r;
    free_res(r, false); r = Res(); r.kind = rt ? XT_RENDERTARGET : XT_TEXTURE; r.fmt = f; r.w = std::max(w, r.w); r.h = std::max(h, r.h); r.levels = 1; r.faces = 1;
    D3D12_HEAP_PROPERTIES hp = {}; hp.Type = D3D12_HEAP_TYPE_DEFAULT;
    D3D12_RESOURCE_DESC rd = {}; rd.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; rd.Width = r.w; rd.Height = r.h; rd.DepthOrArraySize = 1;
    rd.MipLevels = 1; rd.Format = f; rd.SampleDesc.Count = 1; rd.Flags = rt ? D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET : D3D12_RESOURCE_FLAG_NONE;
    r.st = D3D12_RESOURCE_STATE_COPY_DEST;
    if (FAILED(dev->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, r.st, nullptr, __uuidof(ID3D12Resource), (void **)&r.r))) { r.r = nullptr; return nullptr; }
    if (!rt) { r.srv = h_srv.take(dev); D3D12_SHADER_RESOURCE_VIEW_DESC v = srv_desc(f, false, 0, 1); dev->CreateShaderResourceView(r.r, &v, h_srv.at(r.srv)); }
    return &r;
  };
  Res *a = scratch(0xFFFFFF00u + 2 * (uint32_t)src.fmt % 128, src.fmt, false), *b = scratch(0xFFFFFF01u + 2 * (uint32_t)dst.fmt % 128, dst.fmt, true);
  if (!a || !b) { d8_once("copydraw12", "X_COPY between formats of different type groups: a scratch texture could not be made (not copied)"); return; }
  ID3D12PipelineState *&pso = cpy_psos[(uint32_t)dst.fmt];
  if (!pso) {
    D3D12_GRAPHICS_PIPELINE_STATE_DESC g = {};
    g.pRootSignature = rs; g.VS = {cpy_vs->GetBufferPointer(), cpy_vs->GetBufferSize()}; g.PS = {cpy_ps->GetBufferPointer(), cpy_ps->GetBufferSize()};
    D3D11_BLEND_DESC b11 = {}; b11.RenderTarget[0].RenderTargetWriteMask = 0xF; g.BlendState = blend12(b11);
    D3D11_RASTERIZER_DESC r11 = {}; r11.FillMode = D3D11_FILL_SOLID; r11.CullMode = D3D11_CULL_NONE; g.RasterizerState = raster12(r11);
    D3D11_DEPTH_STENCIL_DESC d11 = {}; g.DepthStencilState = depth12(d11, false);
    g.SampleMask = 0xFFFFFFFFu; g.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; g.NumRenderTargets = 1; g.RTVFormats[0] = dst.fmt; g.SampleDesc.Count = 1;
    if (FAILED(dev->CreateGraphicsPipelineState(&g, __uuidof(ID3D12PipelineState), (void **)&pso))) { pso = nullptr; d8_once("cpypso12", "D3D12: the copy pipeline could not be made (not copied)"); return; }
  }
  // room in the descriptor ring first (a wait submits and re-opens the list)
  for (;;) { ring_retire(); if (gv_cap - (gv_head - gv_tail) >= 16 || lost) break; if (gv_marks.empty()) submit(); wait(gv_marks.front().fence); }
  to(src, D3D12_RESOURCE_STATE_COPY_SOURCE); to(*a, D3D12_RESOURCE_STATE_COPY_DEST); barriers();
  D3D12_TEXTURE_COPY_LOCATION dl = {}, sl = {};
  dl.pResource = a->r; dl.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX; dl.SubresourceIndex = 0;
  sl.pResource = src.r; sl.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX; sl.SubresourceIndex = ssub;
  cl->CopyTextureRegion(&dl, 0, 0, 0, &sl, &bx);
  to(*a, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE); to(*b, D3D12_RESOURCE_STATE_RENDER_TARGET); barriers();
  XRef br = {0xFFFFFF01u + 2 * (uint32_t)dst.fmt % 128, 0, 0};
  D3D12_CPU_DESCRIPTOR_HANDLE rtv = rtv_of(br, nullptr, nullptr, nullptr, nullptr);
  cl->OMSetRenderTargets(1, &rtv, FALSE, nullptr);
  D3D12_VIEWPORT vp = {0, 0, (float)w, (float)h, 0, 1}; D3D12_RECT sc = {0, 0, (LONG)w, (LONG)h};
  cl->RSSetViewports(1, &vp); cl->RSSetScissorRects(1, &sc); cl->SetPipelineState(pso);
  cl->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
  uint64_t start = gv_head, phys = start % gv_cap; if (phys + 8 > gv_cap) { start += gv_cap - phys; phys = 0; } gv_head = start + 8;
  D3D12_CPU_DESCRIPTOR_HANDLE t0 = gv_srv->GetCPUDescriptorHandleForHeapStart(); t0.ptr += (SIZE_T)phys * gv_inc;
  for (uint32_t i = 0; i < 8; i++) { D3D12_CPU_DESCRIPTOR_HANDLE di = t0; di.ptr += (SIZE_T)i * gv_inc; dev->CopyDescriptorsSimple(1, di, h_srv.at(i ? null_srv2d : a->srv), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV); }
  D3D12_GPU_DESCRIPTOR_HANDLE gt = gv_srv->GetGPUDescriptorHandleForHeapStart(); gt.ptr += phys * gv_inc;
  cl->SetGraphicsRootDescriptorTable(3, gt);
  cl->DrawInstanced(4, 1, 0, 0);
  to(*b, D3D12_RESOURCE_STATE_COPY_SOURCE); to(dst, D3D12_RESOURCE_STATE_COPY_DEST); barriers();
  D3D12_BOX ob = {0, 0, 0, w, h, 1};
  dl.pResource = dst.r; dl.SubresourceIndex = dsub; sl.pResource = b->r; sl.SubresourceIndex = 0;
  cl->CopyTextureRegion(&dl, x, y, 0, &sl, &ob);
  bd = Bound(); bd.valid = true; tgt_dirty = true;         // the draw's binds are not the stream's: all bound again
}

// ---- readbacks and snapshots -------------------------------------------------------------------------------------------------
static ID3D12Resource *readback_buffer12(ID3D12Device *dev, uint64_t bytes) {
  D3D12_HEAP_PROPERTIES hp = {}; hp.Type = D3D12_HEAP_TYPE_READBACK;
  D3D12_RESOURCE_DESC rd = {}; rd.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER; rd.Width = bytes; rd.Height = 1; rd.DepthOrArraySize = 1;
  rd.MipLevels = 1; rd.SampleDesc.Count = 1; rd.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
  ID3D12Resource *b = nullptr;
  if (FAILED(dev->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, __uuidof(ID3D12Resource), (void **)&b))) return nullptr;
  return b;
}
int32_t Exec12::readback(const XRef &r, std::vector<uint8_t> *reply) {
  Res *x = res_of(r.id);
  if (!x || !x->r || x->kind == XT_DEPTH || x->kind == 3 || lost) return D3DERR_INVALIDCALL;
  UINT sub = sub12(*x, r), w = std::max(1u, x->w >> r.level), h = std::max(1u, x->h >> r.level);
  D3D12_RESOURCE_DESC rd = x->r->GetDesc();
  D3D12_PLACED_SUBRESOURCE_FOOTPRINT fp; UINT nrows; UINT64 rowsize, total;
  dev->GetCopyableFootprints(&rd, sub, 1, 0, &fp, &nrows, &rowsize, &total);
  ID3D12Resource *rb = readback_buffer12(dev, total);
  if (!rb) return D3DERR_INVALIDCALL;
  to(*x, D3D12_RESOURCE_STATE_COPY_SOURCE); barriers();
  D3D12_TEXTURE_COPY_LOCATION dl = {}, sl = {};
  dl.pResource = rb; dl.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT; dl.PlacedFootprint = fp;
  sl.pResource = x->r; sl.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX; sl.SubresourceIndex = sub;
  cl->CopyTextureRegion(&dl, 0, 0, 0, &sl, nullptr);
  submit(); wait(fval);
  int32_t hr = D3DERR_INVALIDCALL; uint8_t *m = nullptr;
  if (!lost && SUCCEEDED(rb->Map(0, nullptr, (void **)&m))) {
    UINT pitch = d8_pitch((D3DFORMAT)x->d3dfmt, w), rows = d8_rows((D3DFORMAT)x->d3dfmt, h);
    reply->resize((size_t)pitch * rows);
    for (UINT y = 0; y < rows && y < nrows; y++) memcpy(reply->data() + (size_t)y * pitch, m + fp.Offset + (size_t)y * fp.Footprint.RowPitch, pitch);
    D3D12_RANGE none = {0, 0}; rb->Unmap(0, &none); hr = D3D_OK;
  }
  rb->Release();
  return hr;
}
// X_SNAP: the copy goes into this object's own readback buffer with the next submission; a Present that finds the GPU past
// it fills the slot (as d8_exec.cpp's snap_poll, which never waits)
void Exec12::snap(const XSnap &s) {
  Res *x = res_of(s.dst.id);
  if (!o.snaps || s.slot >= SNAP_SLOTS || !x || !x->r || x->kind == XT_DEPTH || x->kind == 3) return;
  if ((size_t)d8_pitch((D3DFORMAT)x->d3dfmt, x->w) * d8_rows((D3DFORMAT)x->d3dfmt, x->h) > SNAP_BYTES) return;
  D3D12_RESOURCE_DESC rd = x->r->GetDesc();
  D3D12_PLACED_SUBRESOURCE_FOOTPRINT fp; UINT nrows; UINT64 rowsize, total;
  UINT sub = sub12(*x, s.dst);
  dev->GetCopyableFootprints(&rd, sub, 1, 0, &fp, &nrows, &rowsize, &total);
  if (!x->snap) { x->snap = readback_buffer12(dev, total); if (!x->snap) { d8_once("snapbuf12", "X_SNAP: a readback buffer could not be made (snapshots off for it)"); return; } }
  // a copy still pending for this object: the later one replaces it, but its buffer is in use; wait for that one first
  // (a copy of it not yet sent with a submission: send it first, then wait; waiting on a fence value not yet signalled
  // by any submission would never end)
  for (auto &e : snap_pend) if (e.id == s.dst.id && e.fence > fence->GetCompletedValue()) { if (e.fence > fval) submit(); wait(e.fence); break; }
  to(*x, D3D12_RESOURCE_STATE_COPY_SOURCE); barriers();
  D3D12_TEXTURE_COPY_LOCATION dl = {}, sl = {};
  dl.pResource = x->snap; dl.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT; dl.PlacedFootprint = fp;
  sl.pResource = x->r; sl.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX; sl.SubresourceIndex = sub;
  cl->CopyTextureRegion(&dl, 0, 0, 0, &sl, nullptr);
  x->snap_pitch = fp.Footprint.RowPitch;
  bool had = false;
  for (auto &e : snap_pend) if (e.id == s.dst.id) { e.slot = s.slot; e.wseq = s.wseq; e.fence = fval + 1; had = true; break; }
  if (!had) snap_pend.push_back({s.dst.id, s.slot, s.wseq, fval + 1});
}
void Exec12::snap_poll() {
  uint64_t done = fence->GetCompletedValue();
  for (size_t i = 0; i < snap_pend.size();) {
    SnapPend q2 = snap_pend[i];
    Res *x = res_of(q2.id);
    if (!x || !x->snap) { snap_pend.erase(snap_pend.begin() + i); continue; }
    if (q2.fence > done) { i++; continue; }
    snap_pend.erase(snap_pend.begin() + i);
    uint8_t *m = nullptr;
    if (FAILED(x->snap->Map(0, nullptr, (void **)&m))) { d8_once("snapmap12", "X_SNAP: Map of a readback buffer failed"); continue; }
    UINT pitch = d8_pitch((D3DFORMAT)x->d3dfmt, x->w), rows = d8_rows((D3DFORMAT)x->d3dfmt, x->h);
    SnapSlot &s = o.snaps[q2.slot];
    uint32_t v = __atomic_load_n(&s.ver, __ATOMIC_SEQ_CST);
    __atomic_store_n(&s.ver, v | 1u, __ATOMIC_SEQ_CST);
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
    s.id = q2.id; s.wseq = q2.wseq; s.bytes = pitch * rows;
    for (UINT y = 0; y < rows; y++) memcpy(s.data + (size_t)y * pitch, m + (size_t)y * x->snap_pitch, pitch);
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
    __atomic_store_n(&s.ver, (v | 1u) + 1u, __ATOMIC_SEQ_CST);
    D3D12_RANGE none = {0, 0}; x->snap->Unmap(0, &none);
    o.snaps_done++;
  }
}

// ---- Present ------------------------------------------------------------------------------------------------------------
void Exec12::present() {
  Res *b = res_of(X_BACKBUFFER);
  if (!swap || !b) return;
  to(*b, D3D12_RESOURCE_STATE_PRESENT);
  bb_st[bbi] = b->st;
  barriers();
  ts_end();
  HRESULT h = cl->Close();
  if (FAILED(h)) check_lost("ID3D12GraphicsCommandList::Close", h);
  if (!lost) { ID3D12CommandList *l = cl; q->ExecuteCommandLists(1, &l); }
  fval++; q->Signal(fence, fval);
  ring_marks.push_back({fval, ring_head}); gv_marks.push_back({fval, gv_head});
  alloc_fence[fi] = fval; submits++;
  if (!lost) {
    h = swap->Present(sync, 0);
    if (FAILED(h)) check_lost("IDXGISwapChain::Present", h);
  }
  o.presents++;
  // a test (D8ON11_TEST_D3D12_LOST=N): the device counts as lost at the Nth Present, as a driver fault leaves it
  { static long lost_at = getenv("D8ON11_TEST_D3D12_LOST") ? atol(getenv("D8ON11_TEST_D3D12_LOST")) : 0;
    if (lost_at > 0 && o.presents == (uint64_t)lost_at && !lost) check_lost("D8ON11_TEST_D3D12_LOST (a test)", DXGI_ERROR_DEVICE_REMOVED); }
  // frames in flight: the CPU goes on once the frame `inflight` Presents back is done
  // (after frame P is sent, frame P - inflight must be done: at most `inflight` frames are queued for the GPU)
  pres_f[o.presents % 8] = fval;
  if (o.presents > inflight) wait(pres_f[(o.presents - inflight) % 8]);
  // the next allocator, once the GPU is done with what it holds
  fi = (fi + 1) % NF; wait(alloc_fence[fi]);
  if (!lost && FAILED(alloc[fi]->Reset())) check_lost("ID3D12CommandAllocator::Reset", E_FAIL);
  ring_retire(); dig();
  // the back buffer is the next one now
  bbi = swap->GetCurrentBackBufferIndex();
  for (auto &v : b->rtv) h_rtv.give(v.second);
  b->rtv.clear(); b->r = bb[bbi]; b->st = bb_st[bbi];
  begin_list();
  ts_collect(); ts_begin();
  if (!snap_pend.empty()) snap_poll();
  if (++c_frames >= 600) {
    double f = c_frames;
    d8_log("D3D12 binds, %u frames: %.0f draws, %.0f pipeline switches, %.0f texture tables, %.0f sampler tables, %.0f barriers a frame; "
           "%u pipelines made (%u from the library), %u shaders compiled (%u from the disk cache): %.1f ms making them in all",
           c_frames, c_draws / f, c_pso / f, c_srvtab / f, c_smptab / f, c_bars / f, c_pso_new, c_pso_loaded, c_cmp_new, c_cmp_loaded, c_pso_ms + c_cmp_ms);
    if (merge_on) d8_log("draw_merge (D3D12), %u frames: %.1f merged draws a frame took in %.1f others; %.3f MB copied to the slots a frame; %llu times the slots started again",
                         c_frames, mg_runs / f, mg_saved / f, mg_bytes / f / 1048576.0, (unsigned long long)mg_wraps);
    mg_runs = mg_saved = mg_bytes = 0;
    ts_report();
    c_draws = c_pso = c_srvtab = c_smptab = c_bars = 0; c_frames = 0; c_pso_new = c_pso_loaded = c_cmp_new = c_cmp_loaded = 0; c_pso_ms = c_cmp_ms = 0;
  }
  // the game never releases its device at exit (the owner's 13 Nov logs: no shutdown line), so the caches are written
  // while it runs: news, and 1,800 Presents since the last write (or since the start); the write's time is logged
  if ((sh_dirty || plib_dirty) && o.presents - cache_saved_at >= 1800) {
    double t0 = qpc12(); caches_save(); cache_saved_at = o.presents;
    d8_log("D3D12 caches written (%u shaders, pipeline library %s): %.1f ms", (unsigned)sh_disk.size(), plib ? "yes" : "none", qpc12() - t0);
  }
}

void Exec12::shutdown() {
  // (only a started executor has an open list to send; one whose X_INIT failed hands the stream to Direct3D 11, whose
  // shutdown line is the one that counts)
  const bool started = dev && q && fence && swap;
  if (started && !lost) wait_idle();
  if (started) caches_save();
  if (plib) { plib->Release(); plib = nullptr; }
  for (auto *&a : sa) rel(a);
  if (qrb) { qrb->Unmap(0, nullptr); rel(qrb); } rel(qh);
  size_t n = 0; for (auto &r : res) n += r.second.kind != 4 && r.first < 0xFFFFFF00u;
  if (started)
    d8_log("executor shutdown: %u objects still held besides the back buffer (D3D12; %llu pipeline states made, %llu waits for room in the upload ring; "
           "draw_merge: %llu merged draws took in %llu others, the slots started again %llu times)", (unsigned)n, (unsigned long long)pso_made, (unsigned long long)ring_waits,
           (unsigned long long)mg_total_runs, (unsigned long long)mg_total_saved, (unsigned long long)mg_wraps);
  for (auto &r : res) free_res(r.second, true);
  res.clear();
  for (auto &g : graves) g.r->Release(); graves.clear();
  for (auto &p : psos) rel(p.second); psos.clear();
  for (auto &p : cpy_psos) rel(p.second); cpy_psos.clear();
  for (auto &v : vsh) rel(v.second.code); for (auto &v : psh) rel(v.second.code); vsh.clear(); psh.clear();
  rel(cpy_vs); rel(cpy_ps);
  for (int i = 0; i < NBB; i++) rel(bb[i]);
  rel(swap);
  for (CpuHeap *c : {&h_srv, &h_rtv, &h_dsv, &h_smp}) { for (auto *p : c->pages) p->Release(); c->pages.clear(); }
  rel(gv_srv); rel(gv_smp); rel(rs);
  if (ring) { ring->Unmap(0, nullptr); rel(ring); }
  rel(cl); for (int i = 0; i < NF; i++) rel(alloc[i]);
  rel(fence); if (fev) CloseHandle(fev); fev = NULL;
  rel(q); rel(fac); rel(dev);
}
