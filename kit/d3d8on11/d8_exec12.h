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

// ID3D12PipelineLibrary as d3d12.h declares it (the Windows SDK's; zig's mingw-w64 headers have it, the i686 MinGW-w64
// headers here do not): its IID and its methods after ID3D12DeviceChild's, in that order
struct D8PipelineLibrary : public ID3D12DeviceChild {
  virtual HRESULT STDMETHODCALLTYPE StorePipeline(const WCHAR *name, ID3D12PipelineState *pipeline) = 0;
  virtual HRESULT STDMETHODCALLTYPE LoadGraphicsPipeline(const WCHAR *name, const D3D12_GRAPHICS_PIPELINE_STATE_DESC *desc, REFIID riid, void **pso) = 0;
  virtual HRESULT STDMETHODCALLTYPE LoadComputePipeline(const WCHAR *name, const D3D12_COMPUTE_PIPELINE_STATE_DESC *desc, REFIID riid, void **pso) = 0;
  virtual SIZE_T STDMETHODCALLTYPE GetSerializedSize() = 0;
  virtual HRESULT STDMETHODCALLTYPE Serialize(void *data, SIZE_T bytes) = 0;
};
static const GUID IID_D8PipelineLibrary = {0xc64226a8, 0x9201, 0x46af, {0xb4, 0xcc, 0x53, 0xfb, 0x9f, 0xf7, 0x41, 0x4f}};

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
  struct Grave { uint64_t fence; IUnknown *r; };
  std::vector<Grave> graves;
  void bury(IUnknown *r) { if (r) graves.push_back({fval + 1, r}); }
  void dig();

  // ---- descriptors ----
  struct CpuHeap {                                 // pages of non-shader-visible descriptors, with a free list
    D3D12_DESCRIPTOR_HEAP_TYPE type; UINT inc = 0; std::vector<ID3D12DescriptorHeap *> pages; std::vector<uint32_t> free_;
    uint32_t next = 0; enum { PAGE = 4096 };
    D3D12_CPU_DESCRIPTOR_HANDLE at(uint32_t i) const;
    uint32_t take(ID3D12Device *d);
    void give(uint32_t i) { free_.push_back(i); }
  } h_srv, h_rtv, h_dsv, h_smp;
  // (a CPU descriptor is read when a command is recorded -- copied into gv_srv, or by OMSetRenderTargets -- so a destroyed
  // object's descriptors are free at once; its resource is buried until the GPU has passed the submission)
  ID3D12DescriptorHeap *gv_srv = nullptr; UINT gv_inc = 0; uint64_t gv_cap = 0, gv_head = 0, gv_tail = 0; std::deque<Mark> gv_marks;
  ID3D12DescriptorHeap *gv_smp = nullptr; UINT smp_inc = 0; uint32_t smp_next = 0; enum { SMP_CAP = 2048 };
  std::unordered_map<std::string, uint32_t> smp_tables;   // eight sampler descriptions -> their table in gv_smp
  // how a table's samplers are written (13 Nov): copied from CPU-only descriptors (h_smp, smp_cpu) into gv_smp, or made in
  // gv_smp directly. The owner's Parallels run drew as if no sampler state applied (point filtering, wrap addressing);
  // init's sampler check draws through each way and keeps the first that samples right (sampler_check)
  bool smp_copy = true; std::unordered_map<std::string, uint32_t> smp_cpu;
  void write_samplers(uint32_t table, const D3D12_SAMPLER_DESC *s);   // eight descriptions into gv_smp at `table`
  void write_samplers_at(D3D12_CPU_DESCRIPTOR_HANDLE h, const D3D12_SAMPLER_DESC *s);
  // (13 Nov) sampler tables each in a heap of their own, at its start: the owner's Parallels reads a sampler table only at
  // its heap's start, whatever place the table's handle names (the place check: right at sampler 0, POINT's samplers read
  // at 8, 64, 1024, 2040; D8ON11_D3D12_DIAG=smp0 drew every scene as Direct3D 11). Chosen at the start, only where the
  // place check is wrong and the heap check right. smp_tables then maps to an index in smp_hv; a change of table sets
  // the descriptor heaps again
  bool smp_heaps = false; std::vector<ID3D12DescriptorHeap *> smp_hv; enum { SMP_HEAPS_CAP = 1024 };
  ID3D12DescriptorHeap *smp_heap_new(const D3D12_SAMPLER_DESC *sd);
  uint64_t c_heapsw = 0;
  int sampler_check(bool copy, uint32_t px[2]);
  // (13 Nov) the owner's picture check on Parallels: some textures drawn wrong although sampler_check passes; these say
  // what the driver supports for each format the D3D8 side uses, and draw test pixels through those formats
  void format_report();
  // a texture of `fmt` (levels: each level's rows, packed, `pitch` bytes a row of texels or blocks), sampled in four
  // pixels: mode 0 Sample(s0 LINEAR, centre) in pixel 0; mode 1 SampleLevel(s0, centre, i) in pixel i. RGBA of px[0..3]
  bool tex_probe(DXGI_FORMAT fmt, UINT w, UINT h, const std::vector<std::vector<uint8_t>> &levels, const std::vector<UINT> &pitch,
                 const std::vector<UINT> &rows, int mode, uint32_t px[4], D3D12_FILTER filt = D3D12_FILTER_MIN_MAG_MIP_LINEAR,
                 UINT aniso = 0, uint32_t smp_at = 0, int heapsw = 0);
  bool filter_checks();                            // the place check's result: true where a table's place is honoured
  void tex_checks();   // bit 0: the linear sample blends; bit 1: the clamped sample clamps; -1: not run
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
    uint64_t up_frame = ~0ull; uint32_t ver = 0;   // ver: its uploads, counted (draw merging's slots are per version)
    uint32_t serial = 0;                           // a number no other object of this executor has had (draw merging's slot keys)
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
  void barriers() { if (!bars.empty()) { cl->ResourceBarrier((UINT)bars.size(), bars.data()); c_bars += bars.size(); bars.clear(); } }

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
  struct VSh { ID3DBlob *code = nullptr; std::vector<D3D12_INPUT_ELEMENT_DESC> il; uint32_t id = 0; uint64_t h = 0; };   // h: the bytecode's and layout's hash
  struct PSh { ID3DBlob *code = nullptr; uint32_t id = 0; uint64_t h = 0; };
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
  // ---- draw merging (OPT_MERGE, d3d8on11.ini draw_merge=1; 13 Nov): a run of X_DRAW_SAME records -- packets equal in all
  // but buffers and range, as the D3D8 side sends them with draw_delta=1 -- of indexed triangle lists becomes one draw,
  // as the Direct3D 11 executor's draw_merge does (d8_exec.h). Each draw's vertex buffers are copied by the GPU, whole
  // and once per version, to a vertex slot v of four slot buffers (stream s at v * its stride); its indices are written
  // to the ring as 32-bit numbers plus its base vertex and v. The run's head draws them all: the same triangles, in the
  // same order, with the same state. A run is held until a record that is not part of it comes.
  bool merge_on = false; std::vector<uint8_t> run_head, run_tails; uint32_t run_n = 0;
  // each slot buffer's size: 16 MB (GPU memory: not in the game's address space); D8ON11_TEST_SLOT_KB (a test) makes
  // them small so that they fill and start again
  static uint64_t sa_cap() { static uint64_t c = getenv("D8ON11_TEST_SLOT_KB") && atoi(getenv("D8ON11_TEST_SLOT_KB")) > 0 ? (uint64_t)atoi(getenv("D8ON11_TEST_SLOT_KB")) << 10 : 16ull << 20; return c; }
  ID3D12Resource *sa[4] = {}; D3D12_RESOURCE_STATES sa_st[4] = {}; uint64_t sa_pos[4] = {};
  struct SlotKey { uint32_t v[12]; bool operator<(const SlotKey &o2) const { return memcmp(v, o2.v, sizeof v) < 0; } };
  std::map<SlotKey, uint32_t> slots;
  uint64_t mg_runs = 0, mg_saved = 0, mg_bytes = 0, mg_wraps = 0; uint32_t res_serial = 0; bool mg_fell = false; uint64_t mg_total_runs = 0, mg_total_saved = 0;
  Res *eff_stream(const XDraw &d, int s, uint32_t *stride);   // the buffer stream s reads (a skinned stand-in's source)
  bool mergeable(const XDraw &d);
  bool same_run(const XDraw &first, const XDraw &d);
  void flush_run();
  struct MergeIn { const uint8_t *tails; uint32_t n; };
  void draw(const XDraw &d, const uint8_t *data, const MergeIn *mi = nullptr);
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

  // ---- the GPU's time (13 Nov): timestamps at a frame's start, at each change of render target, and before its Present,
  // resolved with the frame and read once its fence has passed (never waited for); logged every 600 Presents by
  // render-target size: what the game's double-size scene pass costs against the rest
  ID3D12QueryHeap *qh = nullptr; ID3D12Resource *qrb = nullptr; const uint64_t *qrb_m = nullptr; double tfreq = 0;
  enum { TQ_FR = 8, TQ_N = 64 };
  struct TFrame { uint32_t n = 0; uint64_t fence = 0; bool pending = false; uint32_t key[TQ_N]; } tq[TQ_FR];
  uint32_t tq_cur = 0; bool tq_on = false;
  struct PassT { double ms = 0; uint32_t n = 0; };
  std::map<uint32_t, PassT> tq_pass; std::vector<float> tq_ms; uint32_t tq_frames = 0, tq_bad = 0;
  void ts_init(); void ts_mark(uint32_t key); void ts_begin(); void ts_end(); void ts_collect(); void ts_report();
  uint32_t tgt_key() const { return has_rtv ? (rt_w << 16 | (rt_h & 0xFFFF)) : (0x80000000u | rt_w << 16 | (rt_h & 0xFFFF)); }
  // what the list did, every 600 Presents: pipeline switches, table changes, barriers; pipelines and shaders made
  // (and the time it took: a first use of a state combination costs the frame it happens in)
  uint64_t c_pso = 0, c_srvtab = 0, c_smptab = 0, c_bars = 0, c_draws = 0; uint32_t c_frames = 0;
  double c_pso_ms = 0, c_cmp_ms = 0; uint32_t c_pso_new = 0, c_pso_loaded = 0, c_cmp_new = 0, c_cmp_loaded = 0;
  // ---- caches on disk, next to the executor's module (13 Nov): compiled shaders by their HLSL text's hash
  // (d3d8on11_d3d12_shaders.bin) and the driver's own pipeline library (d3d8on11_d3d12_pipelines.bin), so a state
  // combination met in an earlier start is not compiled again. Written at shutdown and every 1,800 Presents with news
  D8PipelineLibrary *plib = nullptr; std::vector<uint8_t> plib_blob; bool plib_dirty = false;
  std::unordered_map<std::string, std::vector<uint8_t>> sh_disk; bool sh_dirty = false; std::string cache_dir;
  typedef HRESULT (WINAPI *PFN_CreateBlob)(SIZE_T, ID3DBlob **); PFN_CreateBlob create_blob = nullptr;
  void caches_load(); void caches_save(); uint64_t cache_saved_at = 0;
  ID3D12PipelineState *make_pso(const D3D12_GRAPHICS_PIPELINE_STATE_DESC &g, uint64_t vh, uint64_t ph);
};
