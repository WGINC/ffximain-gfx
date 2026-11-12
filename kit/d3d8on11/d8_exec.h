// d8_exec.h -- d3d8on11's executor: the only code that uses Direct3D 11. It runs the command stream (d8_proto.h) in
// order: in the game's process (on its thread, or a thread of its own) or in a process of its own (d8x.exe, built for
// x86 or natively for ARM64). d8_link.cpp carries the stream to it.
#pragma once
#include "d8_common.h"
#include "d8_proto.h"
#include <d3d11.h>
#include <dxgi.h>
#include <string>
#include <unordered_map>
#include <map>
#include <memory>
#include <vector>

struct ExecFF;
struct ExecRes {
  uint32_t kind = 0;                               // XT_*, or 3: a buffer, 4: the back buffer
  ID3D11Texture2D *tex = nullptr; ID3D11ShaderResourceView *srv = nullptr; ID3D11DepthStencilView *dsv = nullptr;
  ID3D11Buffer *buf = nullptr;
  std::vector<uint8_t> shadow; bool dyn = false, mapped = false;
  // buffer_arena (Exec::BArena): where a dynamic buffer's contents are for a draw: in its own buffer, or in the arena
  // of generation agen at byte `at`
  bool in_arena = false; uint32_t at = 0, agen = 0;
  uint32_t ver = 0, ref_chunk = 0;
  // gpu skinning (X_SKIN): this buffer stands for source buffer skin_src skinned by palette pal (0: it does not)
  uint32_t skin_src = 0; std::shared_ptr<struct SkinPal> pal;                 // draw_merge: counts its uploads; buffer_arena: the chunk a draw last read it in
  uint64_t up_frame = ~0ull;                       // the Present count at its last upload (the stats' "again")   // a dynamic buffer: its whole contents, kept here
  ID3D11Texture2D *stage = nullptr;                // X_SNAP's staging copy (a small lockable render target)
  std::unordered_map<uint32_t, ID3D11RenderTargetView *> rtv;   // per level*8+face
  std::unordered_map<uint32_t, ID3D11ShaderResourceView *> srv_levels;   // views of some levels: first*256+count
  uint32_t w = 0, h = 0, levels = 1, faces = 1, d3dfmt = 0, index = 0;
};
struct SkinPal { uint64_t serial; std::vector<uint8_t> d; };   // a bone palette: 64 bytes a bone
struct Exec {
  ID3D11Device *dev = nullptr; ID3D11DeviceContext *ctx = nullptr; IDXGISwapChain *swap = nullptr;
  HWND wnd = NULL;
  std::unordered_map<uint32_t, ExecRes> res;
  ID3D11RenderTargetView *cur_rtv = nullptr; ID3D11DepthStencilView *cur_dsv = nullptr;
  ExecFF *ff = nullptr;
  ID3D11InfoQueue *iq = nullptr;                   // D8ON11_DEBUG_LAYER only: drained to the log at each Present
  void drain_debug();
  uint64_t draws = 0, presents = 0;
  // (12 Nov) what a second thread could take: the per-flush planning (Exec::plan), and a sample (1 draw in 32) of draw()
  // split at its first D3D11 bind into preparing (records, keys, cached objects) and submitting (the binds and the draw)
  double plan_ms = 0, split_prep = 0, split_sub = 0; uint64_t split_n = 0; uint32_t split_ctr = 0;
  uint32_t sync = 0;                               // Present's SyncInterval, from X_INIT / X_RESIZE (XInit.interval)
  int32_t fail_init = 0;                           // tests only (d8x.exe): X_INIT answers this when nonzero
  // X_OPTION OPT_FILTER: a draw skips the Direct3D 11 calls that would set what is already set (Bound, below)
  uint32_t filter = 0;
  // X_SNAP: the snapshot table (d8_link.cpp gives it) and the copies the GPU may not have finished yet, one per object
  // (a later X_SNAP of the same object replaces the earlier: its staging copy holds the later contents)
  SnapSlot *snaps = nullptr;
  struct SnapPend { uint32_t id, slot, wseq; };
  std::vector<SnapPend> snap_pend;
  uint64_t snaps_done = 0;
  void snap_poll();
  // one record; a sync record's answer goes to hr and reply
  void run(uint16_t op, const uint8_t *p, uint32_t n, int32_t *hr, std::vector<uint8_t> *reply);
  // X_OPTION OPT_BATCH (1: on): records other than X_INIT, X_RESIZE, X_READBACK, X_FINISH, X_PRESENT and X_OPTION are
  // held, not run, until one of those comes or the transport calls flush(). A flush writes the held draws' constants
  // and ...UP / inline index data into large dynamic buffers (the arenas) with one Map of each, then runs the records
  // in their order; a draw binds its constants at an offset (ID3D11DeviceContext1::VSSetConstantBuffers1, where the
  // driver reports ConstantBufferOffsetting) instead of mapping a buffer of its own. Direct3D 11 runs a context's
  // commands in the order they are made, so the records run in the same order as without it and the picture is the
  // same (test_d3d8on11.py compares). Why: on the owner's machine each Map is a Lock2 and an Unlock2 system call
  // (docs/HISTORY.md, 18 Oct, run 14204: the counts, and their cost, which is inferred).
  // A held record's bytes must stay where they are until the flush: the ring's reader does not free them before
  // (d8_link.cpp); copy_held (direct mode, whose buffer is reused) makes this executor copy them.
  uint32_t batch = 0; bool copy_held = false;
  bool holding() const { return !held.empty(); }
  // X_DRAW_SAME: the head of the last draw packet run or held. While its record is held it is read where it lies
  // (in the ring, or in `hold` at head_off); otherwise from head_copy
  const uint8_t *head_p = nullptr; size_t head_off = ~(size_t)0; std::vector<uint8_t> head_copy; std::vector<uint8_t> same_tmp;
  void head_keep();
  void flush();
  // the time each record took to run, for the stats (d8_link.cpp); a flush's own work counts as the draws'
  void (*timed)(void *, uint16_t, double) = nullptr; void *timed_ctx = nullptr;
  bool sites_on = false;                             // OPT_SITES
  // OPT_FLIP: the swap chain's model. 0: DXGI_SWAP_EFFECT_DISCARD, one buffer (the blt model, as before); 1:
  // DXGI_SWAP_EFFECT_FLIP_DISCARD, two buffers (the flip model). Set before the swap chain is made, for the whole start
  // (7 Nov: switched while running, it froze the owner's game); the flip model's targets are bound again after each
  // Present (last_rt, last_ds: what X_TARGETS set)
  uint32_t latency = 0;                            // OPT_LATENCY: the frames DXGI may queue (0: its default, 3)
  bool flip = false, flip_want = false; XInit last_init = {}; XRef last_rt = {}; uint32_t last_ds = 0;
  int32_t make_swap();
  // OPT_GPUTIME: the GPU's time for each frame's work, from timestamp queries around it (a disjoint query each); read
  // back NQ frames later without waiting (D3D11_ASYNC_GETDATA_DONOTFLUSH); a frame whose queries are not ready by then
  // is counted as not measured. Logged every 600 Presents: the mean, the 95th percentile, how many were measured.
  bool gpu_on = false; enum { NQ = 6 };
  ID3D11Query *q_dis[NQ] = {}, *q_beg[NQ] = {}, *q_end[NQ] = {}; bool q_used[NQ] = {}; int q_i = 0; bool q_open = false;
  std::vector<float> gpu_ms; uint32_t gpu_frames = 0, gpu_lost = 0;
  void gpu_frame_end(); void gpu_frame_begin(); void gpu_collect(int slot, bool wait_none);
  uint32_t last_site = ~0u;                          // XDraw::site of the draw run last (the stats charge its time to it)
  uint64_t flushes = 0, arena_maps = 0, draw_maps = 0;
  uint64_t up_bytes[2] = {}, up_n[2] = {}, dyn_discard = 0, dyn_noov = 0;
  uint64_t up_again[2] = {};                       // bytes to an object uploaded in this frame or the one before too   // uploads: [0] textures, [1] buffers; dynamic Maps   // for the stats: flushes, Maps of the arenas, Maps by a draw
  void shutdown();
private:
  struct Held { uint16_t op; uint32_t n; const uint8_t *p; size_t off; };
  std::vector<Held> held; std::vector<uint8_t> hold;
  // byte offsets into the arenas; ~0u: none (the draw maps). bufs: the index into prep_bufs of this draw's buffers as
  // the batch placed them (buffer_arena), ~0u: none; up: an upload the batch has already written (not run again)
  // merged (draw_merge): 1 the first draw of a merged run, which draws mcount indices from mfirst of the merged index
  // arena with the run's streams bound to the slot arenas; 2 a later draw of the run (not run)
  // skin_src (gpu skinning): the source buffer a skinned draw reads (0: not skinned, ~0u: its stand-in holds nothing),
  // skin_pal: 1 + the index of its palette in prep_pals
  struct Prep { uint32_t vc, pc, vb, ib, bufs; uint8_t up; uint32_t up_at; uint8_t merged; uint32_t mfirst, mcount; uint32_t skin_src, skin_pal; };
  std::vector<std::shared_ptr<SkinPal>> prep_pals;
  std::shared_ptr<SkinPal> last_pal; uint64_t pal_serial = 0; ID3D11Buffer *bones_cb = nullptr;
  void skin_apply(const XSkin &x, const uint8_t *data);
  struct PrepBufs { ID3D11Buffer *sb[16]; UINT so[16]; ID3D11Buffer *ib; UINT io; };
  std::vector<PrepBufs> prep_bufs;
  // buffer_arena (X_OPTION OPT_BUFARENA; d3d8on11.ini buffer_arena): the game's dynamic buffers' contents live in two
  // large dynamic buffers, [0] vertex data and [1] indices, instead of a Direct3D 11 buffer each. An upload that
  // replaces a buffer (DISCARD, or no flag) puts the buffer's whole contents at a new place in the arena; a
  // NOOVERWRITE upload writes its range where the buffer already is. A draw binds the arena at the buffer's offset.
  // A new place each time is what Map(DISCARD) of the buffer itself gives, so in a batch every upload can be written
  // before the batch's first draw, under one Map of the arena (Exec::plan). When the arena is full it starts again
  // with Map(DISCARD) and a new generation; a buffer still at a place of an older generation is written to its own
  // Direct3D 11 buffer when next drawn, and stays there until its next upload.
  struct BArena { ID3D11Buffer *b = nullptr; UINT cap = 0, pos = 0; uint32_t gen = 1; uint8_t *m = nullptr; bool fresh = true; } ba[2];
public:
  // draw_merge (X_OPTION OPT_MERGE; d3d8on11.ini draw_merge; needs buffer_arena and batches): consecutive indexed
  // triangle-list draws whose packets are equal in everything but their buffers and index range become one draw.
  // The game draws a character part from its own vertex buffers (three streams) and its own index buffer, so such
  // draws differ only there (docs/HISTORY.md, 6 Oct: 1,620 a frame in the plaza, 510-771 if the buffers were shared).
  // Each draw's stream buffers are copied whole, from the executor's copies, to one vertex slot v of the slot arenas
  // (stream s at byte v * its stride, so one vertex number reads every stream), and its indices are written to the
  // merged index arena as 32-bit numbers plus v and its base vertex. The run's first draw then draws all of them:
  // the same triangles, in the same order, with the same state. A buffer set already at a slot (its buffers' upload
  // counts unchanged) is not copied again. When an arena is full merging stops for the chunk and the arenas start
  // again at the next.
  uint32_t merge_on = 0, merge_cap = 0;
  uint64_t mg_runs = 0, mg_saved = 0, mg_bytes = 0, mg_wraps = 0; uint64_t mg_kept = 0;   // bytes of static buffers' copies held
  uint32_t arena_kb = 0;
  uint64_t ba_place = 0, ba_range = 0, ba_bytes = 0, ba_wraps = 0, ba_own = 0, ba_maps = 0;   // for the stats
private:
  bool arena_for(const ExecRes &r) const { return arena_kb && r.dyn && ba[r.index ? 1 : 0].b && r.shadow.size() <= ba[r.index ? 1 : 0].cap / 8; }
  void arena_make();
  bool arena_take(BArena &a, UINT bytes, bool may_wrap, UINT *at);
  void arena_own(ExecRes &r);
  bool arena_upload_now(ExecRes &r, const XUpBuf &u, const uint8_t *data);
  void arena_resolve(ExecRes &r, ID3D11Buffer **b, UINT *off);
  struct SArena { ID3D11Buffer *b = nullptr; UINT cap = 0, pos = 0; bool fresh = true, used = false; uint8_t *m = nullptr; } sa[5];   // [0..3] streams, [4] indices
  uint32_t sgen = 1, chunk_no = 0; bool swrap = false;
  struct SlotKey { uint32_t v[12]; bool operator<(const SlotKey &o) const { return memcmp(v, o.v, sizeof v) < 0; } };
  std::map<SlotKey, uint32_t> slots;                 // (each stream's buffer, upload count, stride) -> the vertex slot
  struct MOp { uint32_t j; uint8_t kind, s; uint32_t dst, id, a, b; int32_t add; };   // kind 0: copy buffer id to sa[s] at dst; 1: indices
  std::vector<MOp> mops;
  void merge_make();
  std::vector<Prep> prep;
  size_t plan(size_t i);
  void exec(uint16_t op, const uint8_t *p, uint32_t n, int32_t *hr, std::vector<uint8_t> *reply, const Prep *pp);
  int32_t init(const XInit &x, bool resize);
  void release_swap();
  ID3D11RenderTargetView *rtv_of(const XRef &r);
  void draw(const XDraw &d, const uint8_t *data, const Prep *pp);
  void clear_rects(const XClear &c, const XRect *r);
  // what this executor last set on the context, for the filter. Every member is what a draw sets; anything else that
  // sets context state (clear_rects, X_TARGETS, release_swap's ClearState) calls forget(). Also the last keys and
  // descriptions a draw looked up, so that a draw like the one before it does not build their keys again, and the
  // constant bytes last written to vcb / pcb (only draws write those buffers, so these outlive forget()).
  struct Bound {
    bool valid = false;
    ID3D11VertexShader *vs = nullptr; ID3D11PixelShader *ps = nullptr; ID3D11InputLayout *lay = nullptr;
    ID3D11BlendState *bs = nullptr; ID3D11DepthStencilState *dss = nullptr; UINT sref = 0; ID3D11RasterizerState *rs = nullptr;
    D3D11_VIEWPORT vp = {}; UINT topo = 0; bool cbs = false; UINT vco = ~0u, pco = ~0u;   // vco, pco: the arenas' offsets bound
    ID3D11ShaderResourceView *srv[8] = {}; ID3D11SamplerState *smp[8] = {};
    ID3D11Buffer *vb[16] = {}; UINT stride[16] = {}, voff[16] = {};
    ID3D11Buffer *ib = nullptr; DXGI_FORMAT ifmt = DXGI_FORMAT_UNKNOWN; UINT ioff = 0;
    uint64_t pal = 0;                                 // the palette in bones_cb (gpu skinning)
  } bd;
  struct Memo {
    bool vk_ok = false, pk_ok = false, vc_ok = false, pc_ok = false, bs_ok = false, ds_ok = false, rs_ok = false, ss_ok[8] = {};
    VSKey vk; PSKey pk; VSConst vc; PSConst pc;
    ID3D11VertexShader *vsh = nullptr; ID3D11InputLayout *lay = nullptr; ID3D11PixelShader *psh = nullptr;
    D3D11_BLEND_DESC bsd; ID3D11BlendState *bs = nullptr; D3D11_DEPTH_STENCIL_DESC dsd; ID3D11DepthStencilState *dss = nullptr;
    D3D11_RASTERIZER_DESC rsd; ID3D11RasterizerState *rs = nullptr; D3D11_SAMPLER_DESC ssd[8]; ID3D11SamplerState *ss[8] = {};
  } memo;
  void forget() { bd = Bound(); }
  ID3D11VertexShader *clr_vs = nullptr; ID3D11PixelShader *clr_ps = nullptr; ID3D11Buffer *clr_cb = nullptr;
  // X_COPY between formats Direct3D 8 counts as one and Direct3D 11 does not (copy_draw)
  ID3D11VertexShader *cpy_vs = nullptr; ID3D11PixelShader *cpy_ps = nullptr;
  struct Scratch { ID3D11Texture2D *tex = nullptr; ID3D11ShaderResourceView *srv = nullptr; ID3D11RenderTargetView *rtv = nullptr; UINT w = 0, h = 0; };
  std::unordered_map<uint32_t, Scratch> cpy_src, cpy_dst;   // per DXGI format
  Scratch *scratch(std::unordered_map<uint32_t, Scratch> &m, DXGI_FORMAT f, UINT w, UINT h, UINT bind);
  void copy_draw(ID3D11Texture2D *dt, UINT dsub, UINT x, UINT y, ID3D11Texture2D *st, UINT ssub, const D3D11_BOX &bx);
  void ff_init();
};
