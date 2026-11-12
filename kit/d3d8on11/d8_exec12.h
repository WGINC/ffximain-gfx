// d8_exec12.h -- d3d8on11's Direct3D 12 executor (12 Nov): the same command stream as the Direct3D 11 executor
// (d8_proto.h), run on Direct3D 12. Chosen by d3d8on11.ini backend=d3d12 (X_OPTION OPT_BACKEND, before X_INIT); when
// a Direct3D 12 device or swap chain cannot be made, Exec runs the stream on Direct3D 11 as before (logged).
//
// Why: on the owner's machine (Parallels, kit/d3d11probe probe12, 12 Nov) 2,000 small draws took 6.1 ms on Direct3D
// 11's immediate context and 2.0 ms on Direct3D 12, from the game's x86 process (docs/HISTORY.md).
//
// What differs from the Direct3D 11 executor, by design:
//   - Memory the CPU writes for the GPU (draw constants, the bone palette, dynamic buffers' contents, ...UP vertices and
//     inline indices, texture and buffer uploads) comes from one upload ring, mapped once. A region is reused only after
//     the GPU has finished the submission that read it (a fence value per submission). A dynamic buffer's contents are
//     copied into the ring from its CPU copy (the shadow) at its first draw in each submission, so a draw never reads
//     ring memory the ring may already have given out again; within a submission DISCARD gives it a new place and
//     NOOVERWRITE writes its range in place, as a Direct3D 11 dynamic buffer's Map does.
//   - The pipeline state (shaders, input layout, blend, depth-stencil, rasterizer, topology type, target formats) is one
//     object, made once per combination and cached (Direct3D 12 has no separate state objects).
//   - Resources are in a state the command list says (render target, shader resource, copy source or destination);
//     each is moved to the state its next use needs, before that use.
//   - The swap chain is the flip model (DXGI_SWAP_EFFECT_FLIP_DISCARD, 2 buffers): Direct3D 12 has no other. OPT_FLIP
//     is therefore always on here; the D3D8 side's DISCARD swap effect leaves the back buffer undefined after a
//     Present either way.
//   - D3D11-only levers (OPT_BATCH, OPT_FILTER, OPT_BUFARENA, OPT_MERGE, OPT_GPUTIME) are not used: batching exists
//     to save Direct3D 11 Map calls, which this executor does not make per draw. They are logged as not applied.
#pragma once
#include "d8_exec.h"
#include <d3d12.h>
#include <dxgi1_4.h>
#include <deque>

struct Exec;
struct Exec12 {
  explicit Exec12(Exec &owner) : o(owner) {}
  Exec &o;                                         // the stats and the snapshot table are the owner's
  // run one record (the owner times it); X_INIT failing means: run this stream on Direct3D 11
  void run(uint16_t op, const uint8_t *p, uint32_t n, int32_t *hr, std::vector<uint8_t> *reply);
  int32_t init(const XInit &x, bool resize);
  void shutdown();
  bool ok() const { return dev != nullptr; }
  uint32_t last_site = ~0u;

  // ---- device, queue, swap chain ----
  ID3D12Device *dev = nullptr; ID3D12CommandQueue *q = nullptr; IDXGIFactory4 *fac = nullptr; IDXGISwapChain3 *swap = nullptr;
  enum { NBB = 2, NF = 3 };                        // back buffers; command allocators (frames the CPU may be ahead)
  ID3D12Resource *bb[NBB] = {}; D3D12_RESOURCE_STATES bb_st[NBB] = {}; uint32_t bbi = 0;   // bbi: the current back buffer
  uint64_t pres_f[8] = {};                         // the fence value of each of the last Presents
  ID3D12CommandAllocator *alloc[NF] = {}; uint64_t alloc_fence[NF] = {}; uint32_t fi = 0;
  ID3D12GraphicsCommandList *cl = nullptr;
  ID3D12Fence *fence = nullptr; HANDLE fev = NULL; uint64_t fval = 0;   // fval: the value the last submission signals
  uint32_t inflight = 2;                           // OPT_LATENCY: frames the GPU may be behind the CPU (1..NF)
  HWND wnd = NULL; uint32_t sync = 0, width = 0, height = 0;
  uint64_t submits = 0;                            // the submission count: ring data older than this one is re-copied
  void submit();                                   // close, execute, signal, reopen (the ring's marks with it)
  void wait(uint64_t v);                           // until the GPU has passed fence value v
  void wait_idle() { submit(); wait(fval); }
  void begin_list();                               // the list is open again: everything is bound again at the next draw

  // ---- the upload ring ----
  ID3D12Resource *ring = nullptr; uint8_t *ring_m = nullptr; D3D12_GPU_VIRTUAL_ADDRESS ring_va = 0; uint64_t ring_cap = 0;
  uint64_t ring_head = 0, ring_tail = 0;           // virtual offsets (physical = mod cap); [tail, head) is in use
  struct Mark { uint64_t fence, head; }; std::deque<Mark> ring_marks;
  uint64_t ring_waits = 0;
  // `bytes` at `align`; its CPU pointer and GPU address. Never fails for bytes <= cap / 2: waits for the GPU instead
  bool ring_alloc(uint64_t bytes, uint64_t align, uint8_t **cpu, D3D12_GPU_VIRTUAL_ADDRESS *gpu, uint64_t *off = nullptr);
  void ring_retire();
  // an upload larger than half the ring: a buffer of its own, released when the GPU is done with it
  struct Grave { uint64_t fence; ID3D12Resource *r; };
  std::vector<Grave> graves;
  void bury(ID3D12Resource *r) { if (r) graves.push_back({fval + 1, r}); }
  void dig();

  // ---- descriptors ----
  struct CpuHeap {                                 // pages of non-shader-visible descriptors, with a free list
    D3D12_DESCRIPTOR_HEAP_TYPE type; UINT inc = 0; std::vector<ID3D12DescriptorHeap *> pages; std::vector<uint32_t> free_;
    uint32_t next = 0; enum { PAGE = 4096 };
    D3D12_CPU_DESCRIPTOR_HANDLE at(uint32_t i) const;
    uint32_t take(ID3D12Device *d);
    void give(uint32_t i) { free_.push_back(i); }
  } h_srv, h_rtv, h_dsv;
  // (a CPU descriptor is read when a command is recorded -- copied into gv_srv, or by OMSetRenderTargets -- so a destroyed
  // object's descriptors are free at once; its resource is buried until the GPU has passed the submission)
  ID3D12DescriptorHeap *gv_srv = nullptr; UINT gv_inc = 0; uint64_t gv_cap = 0, gv_head = 0, gv_tail = 0; std::deque<Mark> gv_marks;
  ID3D12DescriptorHeap *gv_smp = nullptr; UINT smp_inc = 0; uint32_t smp_next = 0; enum { SMP_CAP = 2048 };
  std::unordered_map<std::string, uint32_t> smp_tables;   // eight sampler descriptions -> their table in gv_smp
  uint32_t null_srv2d = ~0u, null_srvcube = ~0u;   // in h_srv
  ID3D12RootSignature *rs = nullptr;

  // ---- objects ----
  struct Res {
    uint32_t kind = 0;                             // XT_*, 3: a buffer
    ID3D12Resource *r = nullptr; D3D12_RESOURCE_STATES st = D3D12_RESOURCE_STATE_COMMON;
    DXGI_FORMAT fmt = DXGI_FORMAT_UNKNOWN; uint32_t w = 0, h = 0, levels = 1, faces = 1, d3dfmt = 0, index = 0;
    uint32_t srv = ~0u, dsv = ~0u; std::unordered_map<uint32_t, uint32_t> rtv;   // h_srv / h_dsv / h_rtv indices; rtv per level*8+face
    std::unordered_map<uint32_t, uint32_t> srv_levels;                          // first*256+count -> h_srv index
    // a buffer
    bool dyn = false; std::vector<uint8_t> shadow; D3D12_GPU_VIRTUAL_ADDRESS va = 0; uint8_t *cpu = nullptr; uint64_t epoch = ~0ull;
    uint32_t skin_src = 0; std::shared_ptr<SkinPal> pal;
    uint64_t up_frame = ~0ull;
    // X_SNAP: a readback buffer of its own and the fence value its copy is done at
    ID3D12Resource *snap = nullptr; uint32_t snap_pitch = 0;
  };
  std::unordered_map<uint32_t, Res> res;
  void free_res(Res &r, bool now);
  D3D12_CPU_DESCRIPTOR_HANDLE rtv_of(const XRef &r, DXGI_FORMAT *fmt, uint32_t *w, uint32_t *h, Res **rr);
  Res *res_of(uint32_t id) { auto it = res.find(id); return it == res.end() ? nullptr : &it->second; }
  std::vector<D3D12_RESOURCE_BARRIER> bars;
  void to(Res &r, D3D12_RESOURCE_STATES s) { to(r.r, r.st, s); }
  void to(ID3D12Resource *r, D3D12_RESOURCE_STATES &cur, D3D12_RESOURCE_STATES s);
  void barriers() { if (!bars.empty()) { cl->ResourceBarrier((UINT)bars.size(), bars.data()); bars.clear(); } }

  // ---- targets ----
  XRef tgt_rt = {}; uint32_t tgt_ds = 0; bool tgt_dirty = true;
  D3D12_CPU_DESCRIPTOR_HANDLE cur_rtv = {}, cur_dsv = {}; bool has_rtv = false, has_dsv = false;
  DXGI_FORMAT rt_fmt = DXGI_FORMAT_UNKNOWN, ds_fmt = DXGI_FORMAT_UNKNOWN; uint32_t rt_w = 0, rt_h = 0;
  ID3D12Resource *rt_res = nullptr; D3D12_RESOURCE_STATES *rt_st = nullptr; Res *ds_res = nullptr;
  void bind_targets();                              // resolves X_TARGETS (the back buffer is the current one) and binds

  // ---- shaders and pipelines ----
  typedef HRESULT (WINAPI *PFN_Compile)(const void *, SIZE_T, const char *, const D3D_SHADER_MACRO *, ID3DInclude *,
                                         const char *, const char *, UINT, UINT, ID3DBlob **, ID3DBlob **);
  PFN_Compile compile = nullptr;
  struct VSh { ID3DBlob *code = nullptr; std::vector<D3D12_INPUT_ELEMENT_DESC> il; uint32_t id = 0; };
  struct PSh { ID3DBlob *code = nullptr; uint32_t id = 0; };
  std::unordered_map<std::string, VSh> vsh; std::unordered_map<std::string, PSh> psh;
  std::unordered_map<std::string, ID3D12PipelineState *> psos;
  ID3DBlob *compile_hlsl(const std::string &src, const char *entry, const char *target);
  ID3DBlob *cpy_vs = nullptr, *cpy_ps = nullptr;
  std::unordered_map<uint32_t, ID3D12PipelineState *> cpy_psos;   // per destination format
  uint64_t pso_made = 0;

  // ---- what is bound on the list (all of it again after begin_list) ----
  struct Bound {
    bool valid = false; ID3D12PipelineState *pso = nullptr; UINT topo = ~0u, sref = ~0u; D3D12_VIEWPORT vp = {}; D3D12_RECT sc = {};
    D3D12_GPU_VIRTUAL_ADDRESS vcb = 0, pcb = 0, bones = 0; uint64_t srv_tab = ~0ull; uint32_t smp_tab = ~0u;
    D3D12_VERTEX_BUFFER_VIEW vb[16] = {}; D3D12_INDEX_BUFFER_VIEW ib = {};
  } bd;
  // the last constants written, and where (valid within one submission)
  VSConst last_vc; PSConst last_pc; D3D12_GPU_VIRTUAL_ADDRESS last_vca = 0, last_pca = 0; uint64_t last_c_epoch = ~0ull;
  uint64_t last_pal = 0, last_pal_epoch = ~0ull; D3D12_GPU_VIRTUAL_ADDRESS last_pal_va = 0;
  uint32_t last_srv_ids[8] = {}; uint64_t last_srv_tab = ~0ull, last_srv_epoch = ~0ull;
  std::vector<uint8_t> head, same;                  // X_DRAW_SAME: the last packet's head; the packet made from it
  std::shared_ptr<SkinPal> last_skin_pal; uint64_t pal_serial = 0;

  // the last draw's keys and what they gave (the draws of a frame mostly repeat them)
  struct Memo {
    bool vk_ok = false, pk_ok = false, smp_ok = false, pso_ok = false;
    VSKey vk; PSKey pk; VSh *v = nullptr; PSh *p = nullptr;
    D3D11_SAMPLER_DESC skd[8]; uint32_t smp = 0;
    uint8_t pkey[512]; ID3D12PipelineState *pso = nullptr;
  } memo;
  void draw(const XDraw &d, const uint8_t *data);
  bool dyn_view(Res &r, D3D12_GPU_VIRTUAL_ADDRESS *va);   // a dynamic buffer's contents for this submission
  void upload_tex(const XUpTex &u, const uint8_t *rows);
  void upload_buf(const XUpBuf &u, const uint8_t *data);
  void clear(const XClear &c, const XRect *r);
  void copy(const XCopy &c);
  void copy_draw(Res &dst, UINT dsub, UINT x, UINT y, Res &src, UINT ssub, const D3D12_BOX &bx);
  int32_t readback(const XRef &r, std::vector<uint8_t> *reply);
  void present();
  void snap(const XSnap &s);
  struct SnapPend { uint32_t id, slot, wseq; uint64_t fence; };
  std::vector<SnapPend> snap_pend;
  void snap_poll();
  bool lost = false; void check_lost(const char *where, HRESULT h);
};
