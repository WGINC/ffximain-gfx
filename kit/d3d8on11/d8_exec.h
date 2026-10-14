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
  uint64_t draws = 0, presents = 0;
  int32_t fail_init = 0;                           // tests only (d8x.exe): X_INIT answers this when nonzero
  // one record; a sync record's answer goes to hr and reply
  void run(uint16_t op, const uint8_t *p, uint32_t n, int32_t *hr, std::vector<uint8_t> *reply);
  void shutdown();
private:
  int32_t init(const XInit &x, bool resize);
  void release_swap();
  ID3D11RenderTargetView *rtv_of(const XRef &r);
  void draw(const XDraw &d, const uint8_t *data);
  void ff_init();
};
