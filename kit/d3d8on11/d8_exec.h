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
#include <vector>

struct ExecFF;
struct ExecRes {
  uint32_t kind = 0;                               // XT_*, or 3: a buffer, 4: the back buffer
  ID3D11Texture2D *tex = nullptr; ID3D11ShaderResourceView *srv = nullptr; ID3D11DepthStencilView *dsv = nullptr;
  ID3D11Buffer *buf = nullptr;
  ID3D11Texture2D *stage = nullptr;                // X_SNAP's staging copy (a small lockable render target)
  std::unordered_map<uint32_t, ID3D11RenderTargetView *> rtv;   // per level*8+face
  std::unordered_map<uint32_t, ID3D11ShaderResourceView *> srv_levels;   // views of some levels: first*256+count
  uint32_t w = 0, h = 0, levels = 1, faces = 1, d3dfmt = 0, index = 0;
};
struct Exec {
  ID3D11Device *dev = nullptr; ID3D11DeviceContext *ctx = nullptr; IDXGISwapChain *swap = nullptr;
  HWND wnd = NULL;
  std::unordered_map<uint32_t, ExecRes> res;
  ID3D11RenderTargetView *cur_rtv = nullptr; ID3D11DepthStencilView *cur_dsv = nullptr;
  ExecFF *ff = nullptr;
  ID3D11InfoQueue *iq = nullptr;                   // D8ON11_DEBUG_LAYER only: drained to the log at each Present
  void drain_debug();
  uint64_t draws = 0, presents = 0;
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
  void flush();
  // the time each record took to run, for the stats (d8_link.cpp); a flush's own work counts as the draws'
  void (*timed)(void *, uint16_t, double) = nullptr; void *timed_ctx = nullptr;
  uint64_t flushes = 0, arena_maps = 0, draw_maps = 0;   // for the stats: flushes, Maps of the arenas, Maps by a draw
  void shutdown();
private:
  struct Held { uint16_t op; uint32_t n; const uint8_t *p; size_t off; };
  std::vector<Held> held; std::vector<uint8_t> hold;
  struct Prep { uint32_t vc, pc, vb, ib; };                // byte offsets into the arenas; ~0u: none (the draw maps)
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
    ID3D11Buffer *ib = nullptr; DXGI_FORMAT ifmt = DXGI_FORMAT_UNKNOWN;
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
