// d8.h -- d3d8on11: our own Direct3D 8 device on Direct3D 11 (docs/D3D11.md, design A, phase 1).
//
// Layout of the work:
//   - the D3D8 side (entry points): keeps the state the game can read back (Get*), owns the D3D8 objects, and records
//     what the GPU must do as commands (Cmd);
//   - the executor (Exec): plays commands into D3D11. Phase 1 runs it on the game's thread at each sync point (a draw,
//     Clear, Present, a copy, a lock of GPU data); phase 2 moves the same function to another thread or process.
// Anything not implemented logs once and fails (d8_gen.h), so what the game needs is found, not guessed.
#pragma once
#define WIN32_LEAN_AND_MEAN
#define COBJMACROS
#include <windows.h>
#include <d3d8.h>
#define D3DCOLORVALUE_DEFINED            // d3d8types.h has defined it; dxgitype.h would again
#include <d3d11.h>
#include <dxgi.h>
#include <stdint.h>
#include <string.h>
#include <vector>

// ---- log (d3d8on11.log next to the module; each message once unless counted) ------------------------------------
void d8_log(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
void d8_notimpl(const char *what);                  // once per name
void d8_once(const char *key, const char *fmt, ...) __attribute__((format(printf, 2, 3)));

// ---- formats ------------------------------------------------------------------------------------------------------
struct FmtInfo {
  DXGI_FORMAT dxgi;   // DXGI_FORMAT_UNKNOWN: not supported here
  int bpp;            // bits per pixel (0 for block formats)
  int block;          // bytes per 4x4 block (BC formats), else 0
  bool depth;
};
FmtInfo d8_fmt(D3DFORMAT f);
UINT d8_pitch(D3DFORMAT f, UINT w);                 // bytes per row (per row of blocks for BC)
UINT d8_rows(D3DFORMAT f, UINT h);                  // rows (rows of blocks for BC)

#include "d8_gen.h"

struct Dev;
struct Tex; struct Surf;

// ---- a surface: a texture level, a render target, a depth-stencil surface, an image surface or the back buffer --
enum SurfKind { SK_TEXLEVEL, SK_RT, SK_DS, SK_IMAGE, SK_BACKBUFFER };
struct Surf final : public IDirect3DSurface8_Stub {
  LONG ref = 1;
  Dev *dev; SurfKind kind; IUnknown *container = nullptr;   // the texture of a texture level (not referenced)
  UINT w, h, level = 0, face = 0; D3DFORMAT fmt; D3DPOOL pool; DWORD usage; bool lockable = false;
  ID3D11Texture2D *tex = nullptr;                // its own GPU image (RT, DS, back buffer); levels use the container's
  ID3D11RenderTargetView *rtv = nullptr; ID3D11DepthStencilView *dsv = nullptr;
  std::vector<uint8_t> cpu;                      // image surfaces, and what a lock reads back or writes
  bool locked = false;
  Surf(Dev *d, SurfKind k, UINT w_, UINT h_, D3DFORMAT f, D3DPOOL p, DWORD u);
  ~Surf();
  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, void **);
  ULONG STDMETHODCALLTYPE AddRef();
  ULONG STDMETHODCALLTYPE Release();
  HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice8 **);
  HRESULT STDMETHODCALLTYPE GetContainer(REFIID, void **);
  HRESULT STDMETHODCALLTYPE GetDesc(D3DSURFACE_DESC *);
  HRESULT STDMETHODCALLTYPE LockRect(D3DLOCKED_RECT *, const RECT *, DWORD);
  HRESULT STDMETHODCALLTYPE UnlockRect();
  ID3D11RenderTargetView *get_rtv();             // a texture level's view is made on first use
};

// ---- a 2D texture (cube textures are Tex with six faces) -------------------------------------------------------------
struct TexCore {
  Dev *dev; UINT w, h, levels, faces; D3DFORMAT fmt; D3DPOOL pool; DWORD usage;
  ID3D11Texture2D *gpu = nullptr; ID3D11ShaderResourceView *srv = nullptr;
  std::vector<std::vector<uint8_t>> cpu;         // per face*levels+level: what the game writes through LockRect
  std::vector<Surf *> surfs;                     // per face*levels+level, made on first GetSurfaceLevel
  UINT lw(UINT l) const { UINT v = w >> l; return v ? v : 1; }
  UINT lh(UINT l) const { UINT v = h >> l; return v ? v : 1; }
  HRESULT init(Dev *d, UINT w_, UINT h_, UINT lv, UINT faces_, DWORD u, D3DFORMAT f, D3DPOOL p);
  void destroy();
  HRESULT lock(UINT face, UINT l, D3DLOCKED_RECT *lr, const RECT *r, DWORD flags);
  HRESULT unlock(UINT face, UINT l);
  Surf *surface(IUnknown *owner, UINT face, UINT l);
};
struct Tex final : public IDirect3DTexture8_Stub {
  LONG ref = 1; TexCore c;
  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, void **);
  ULONG STDMETHODCALLTYPE AddRef();
  ULONG STDMETHODCALLTYPE Release();
  HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice8 **);
  D3DRESOURCETYPE STDMETHODCALLTYPE GetType() { return D3DRTYPE_TEXTURE; }
  DWORD STDMETHODCALLTYPE GetLevelCount() { return c.levels; }
  DWORD STDMETHODCALLTYPE SetPriority(DWORD) { return 0; }
  DWORD STDMETHODCALLTYPE GetPriority() { return 0; }
  void STDMETHODCALLTYPE PreLoad() {}
  DWORD STDMETHODCALLTYPE SetLOD(DWORD) { return 0; }
  DWORD STDMETHODCALLTYPE GetLOD() { return 0; }
  HRESULT STDMETHODCALLTYPE GetLevelDesc(UINT, D3DSURFACE_DESC *);
  HRESULT STDMETHODCALLTYPE GetSurfaceLevel(UINT, IDirect3DSurface8 **);
  HRESULT STDMETHODCALLTYPE LockRect(UINT, D3DLOCKED_RECT *, const RECT *, DWORD);
  HRESULT STDMETHODCALLTYPE UnlockRect(UINT);
  HRESULT STDMETHODCALLTYPE AddDirtyRect(const RECT *) { return D3D_OK; }
};
struct CubeTex final : public IDirect3DCubeTexture8_Stub {
  LONG ref = 1; TexCore c;
  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, void **);
  ULONG STDMETHODCALLTYPE AddRef();
  ULONG STDMETHODCALLTYPE Release();
  HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice8 **);
  D3DRESOURCETYPE STDMETHODCALLTYPE GetType() { return D3DRTYPE_CUBETEXTURE; }
  DWORD STDMETHODCALLTYPE GetLevelCount() { return c.levels; }
  DWORD STDMETHODCALLTYPE SetPriority(DWORD) { return 0; }
  DWORD STDMETHODCALLTYPE GetPriority() { return 0; }
  void STDMETHODCALLTYPE PreLoad() {}
  DWORD STDMETHODCALLTYPE SetLOD(DWORD) { return 0; }
  DWORD STDMETHODCALLTYPE GetLOD() { return 0; }
  HRESULT STDMETHODCALLTYPE GetLevelDesc(UINT, D3DSURFACE_DESC *);
  HRESULT STDMETHODCALLTYPE GetCubeMapSurface(D3DCUBEMAP_FACES, UINT, IDirect3DSurface8 **);
  HRESULT STDMETHODCALLTYPE LockRect(D3DCUBEMAP_FACES, UINT, D3DLOCKED_RECT *, const RECT *, DWORD);
  HRESULT STDMETHODCALLTYPE UnlockRect(D3DCUBEMAP_FACES, UINT);
  HRESULT STDMETHODCALLTYPE AddDirtyRect(D3DCUBEMAP_FACES, const RECT *) { return D3D_OK; }
};

// ---- vertex and index buffers: a CPU copy the game locks, uploaded to the GPU buffer at Unlock ------------------------
struct BufCore {
  Dev *dev; UINT len; DWORD usage, fvf; D3DFORMAT fmt; D3DPOOL pool; bool index;
  std::vector<uint8_t> cpu; ID3D11Buffer *gpu = nullptr;
  UINT lo = 0, hi = 0; int locks = 0;            // the range written since the last upload
  HRESULT init(Dev *d, UINT len_, DWORD u, DWORD fvf_, D3DFORMAT f, D3DPOOL p, bool idx);
  void destroy();
  HRESULT lock(UINT off, UINT size, BYTE **p, DWORD flags);
  HRESULT unlock();
};
struct VB final : public IDirect3DVertexBuffer8_Stub {
  LONG ref = 1; BufCore c;
  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, void **);
  ULONG STDMETHODCALLTYPE AddRef();
  ULONG STDMETHODCALLTYPE Release();
  HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice8 **);
  D3DRESOURCETYPE STDMETHODCALLTYPE GetType() { return D3DRTYPE_VERTEXBUFFER; }
  DWORD STDMETHODCALLTYPE SetPriority(DWORD) { return 0; }
  DWORD STDMETHODCALLTYPE GetPriority() { return 0; }
  void STDMETHODCALLTYPE PreLoad() {}
  HRESULT STDMETHODCALLTYPE Lock(UINT o, UINT s, BYTE **p, DWORD f) { return c.lock(o, s, p, f); }
  HRESULT STDMETHODCALLTYPE Unlock() { return c.unlock(); }
  HRESULT STDMETHODCALLTYPE GetDesc(D3DVERTEXBUFFER_DESC *);
};
struct IB final : public IDirect3DIndexBuffer8_Stub {
  LONG ref = 1; BufCore c;
  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, void **);
  ULONG STDMETHODCALLTYPE AddRef();
  ULONG STDMETHODCALLTYPE Release();
  HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice8 **);
  D3DRESOURCETYPE STDMETHODCALLTYPE GetType() { return D3DRTYPE_INDEXBUFFER; }
  DWORD STDMETHODCALLTYPE SetPriority(DWORD) { return 0; }
  DWORD STDMETHODCALLTYPE GetPriority() { return 0; }
  void STDMETHODCALLTYPE PreLoad() {}
  HRESULT STDMETHODCALLTYPE Lock(UINT o, UINT s, BYTE **p, DWORD f) { return c.lock(o, s, p, f); }
  HRESULT STDMETHODCALLTYPE Unlock() { return c.unlock(); }
  HRESULT STDMETHODCALLTYPE GetDesc(D3DINDEXBUFFER_DESC *);
};

// ---- the state the game can set and read back (D3D8 side) ---------------------------------------------------------------
struct State {
  DWORD rs[256]; DWORD tss[8][32];
  IDirect3DBaseTexture8 *tex[8];
  D3DMATRIX xf[512]; bool xfSet[512];
  D3DVIEWPORT8 vp; D3DMATERIAL8 mat;
  std::vector<D3DLIGHT8> light; std::vector<BOOL> lightOn;
  VB *stream[16]; UINT stride[16]; IB *ib; UINT baseVertex;
  DWORD vs, ps;
};

// recorded state blocks: which pieces the block holds, with their values
struct Block {
  bool rs[256], tss[8][32], tex[8], xf[512], vp, mat, stream[16], ib, vs, ps;
  std::vector<int> lights;                      // indices set or enabled
  State s;
};

// ---- the commands the executor plays into D3D11 -------------------------------------------------------------------------
enum CmdOp { C_CLEAR, C_PRESENT, C_UPLOAD_TEX, C_UPLOAD_BUF, C_SET_TARGETS, C_DRAW, C_COPY };
struct Cmd {
  CmdOp op;
  union {
    struct { DWORD flags; D3DCOLOR color; float z; DWORD stencil; } clear;
    struct { TexCore *t; UINT sub; } uptex;
    struct { BufCore *b; UINT lo, hi; } upbuf;
    struct { Surf *rt, *ds; } targets;
    struct { Surf *src, *dst; RECT r; POINT p; } copy;
    struct { int kind; } draw;
  };
};

struct D8;
struct Dev final : public IDirect3DDevice8_Stub {
  LONG ref = 1; D8 *d3d; HWND wnd; DWORD behavior; D3DPRESENT_PARAMETERS pp;
  ID3D11Device *dev = nullptr; ID3D11DeviceContext *ctx = nullptr; IDXGISwapChain *swap = nullptr;
  Surf *backbuf = nullptr, *autods = nullptr, *curRT = nullptr, *curDS = nullptr;
  State st;
  std::vector<Block *> blocks; Block *rec = nullptr;
  struct VShader { bool used; DWORD fvf; std::vector<DWORD> decl, code; };
  std::vector<VShader> vshaders;                // handle = (index << 1) | 1, as D3D8 handles have bit 0 set
  std::vector<Cmd> q;                            // recorded, not yet executed
  D3DGAMMARAMP gamma; bool inScene = false; UINT64 draws = 0, drawsNotExecuted = 0;
  struct FF *ff = nullptr;                       // the fixed-function pipeline as generated shaders (d8_ff.cpp)
  void ff_init();
  void ff_free();
  HRESULT ff_draw(int kind, D3DPRIMITIVETYPE type, UINT prims, UINT startVertex, UINT minIndex, UINT numVerts, UINT startIndex,
                  const void *upv, UINT upStride, const void *upi, D3DFORMAT upiFmt);
  HRESULT init(D8 *o, HWND focus, DWORD beh, D3DPRESENT_PARAMETERS *p);
  HRESULT make_swap();
  void release_swap();
  void defaults();
  void flush();                                  // the executor: play q into D3D11
  void exec(const Cmd &c);
  void push(const Cmd &c) { q.push_back(c); }

  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, void **);
  ULONG STDMETHODCALLTYPE AddRef();
  ULONG STDMETHODCALLTYPE Release();
  HRESULT STDMETHODCALLTYPE TestCooperativeLevel();
  UINT STDMETHODCALLTYPE GetAvailableTextureMem();
  HRESULT STDMETHODCALLTYPE GetDirect3D(IDirect3D8 **);
  HRESULT STDMETHODCALLTYPE GetDeviceCaps(D3DCAPS8 *);
  HRESULT STDMETHODCALLTYPE GetDisplayMode(D3DDISPLAYMODE *);
  HRESULT STDMETHODCALLTYPE GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *);
  void STDMETHODCALLTYPE SetCursorPosition(UINT, UINT, DWORD) {}
  WINBOOL STDMETHODCALLTYPE ShowCursor(WINBOOL);
  HRESULT STDMETHODCALLTYPE Reset(D3DPRESENT_PARAMETERS *);
  HRESULT STDMETHODCALLTYPE Present(const RECT *, const RECT *, HWND, const RGNDATA *);
  HRESULT STDMETHODCALLTYPE GetBackBuffer(UINT, D3DBACKBUFFER_TYPE, IDirect3DSurface8 **);
  void STDMETHODCALLTYPE SetGammaRamp(DWORD, const D3DGAMMARAMP *);
  void STDMETHODCALLTYPE GetGammaRamp(D3DGAMMARAMP *);
  HRESULT STDMETHODCALLTYPE CreateTexture(UINT, UINT, UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DTexture8 **);
  HRESULT STDMETHODCALLTYPE CreateCubeTexture(UINT, UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DCubeTexture8 **);
  HRESULT STDMETHODCALLTYPE CreateVertexBuffer(UINT, DWORD, DWORD, D3DPOOL, IDirect3DVertexBuffer8 **);
  HRESULT STDMETHODCALLTYPE CreateIndexBuffer(UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DIndexBuffer8 **);
  HRESULT STDMETHODCALLTYPE CreateRenderTarget(UINT, UINT, D3DFORMAT, D3DMULTISAMPLE_TYPE, WINBOOL, IDirect3DSurface8 **);
  HRESULT STDMETHODCALLTYPE CreateDepthStencilSurface(UINT, UINT, D3DFORMAT, D3DMULTISAMPLE_TYPE, IDirect3DSurface8 **);
  HRESULT STDMETHODCALLTYPE CreateImageSurface(UINT, UINT, D3DFORMAT, IDirect3DSurface8 **);
  HRESULT STDMETHODCALLTYPE CopyRects(IDirect3DSurface8 *, const RECT *, UINT, IDirect3DSurface8 *, const POINT *);
  HRESULT STDMETHODCALLTYPE SetRenderTarget(IDirect3DSurface8 *, IDirect3DSurface8 *);
  HRESULT STDMETHODCALLTYPE GetRenderTarget(IDirect3DSurface8 **);
  HRESULT STDMETHODCALLTYPE GetDepthStencilSurface(IDirect3DSurface8 **);
  HRESULT STDMETHODCALLTYPE BeginScene();
  HRESULT STDMETHODCALLTYPE EndScene();
  HRESULT STDMETHODCALLTYPE Clear(DWORD, const D3DRECT *, DWORD, D3DCOLOR, float, DWORD);
  HRESULT STDMETHODCALLTYPE SetTransform(D3DTRANSFORMSTATETYPE, const D3DMATRIX *);
  HRESULT STDMETHODCALLTYPE GetTransform(D3DTRANSFORMSTATETYPE, D3DMATRIX *);
  HRESULT STDMETHODCALLTYPE MultiplyTransform(D3DTRANSFORMSTATETYPE, const D3DMATRIX *);
  HRESULT STDMETHODCALLTYPE SetViewport(const D3DVIEWPORT8 *);
  HRESULT STDMETHODCALLTYPE GetViewport(D3DVIEWPORT8 *);
  HRESULT STDMETHODCALLTYPE SetMaterial(const D3DMATERIAL8 *);
  HRESULT STDMETHODCALLTYPE GetMaterial(D3DMATERIAL8 *);
  HRESULT STDMETHODCALLTYPE SetLight(DWORD, const D3DLIGHT8 *);
  HRESULT STDMETHODCALLTYPE GetLight(DWORD, D3DLIGHT8 *);
  HRESULT STDMETHODCALLTYPE LightEnable(DWORD, WINBOOL);
  HRESULT STDMETHODCALLTYPE GetLightEnable(DWORD, WINBOOL *);
  HRESULT STDMETHODCALLTYPE SetRenderState(D3DRENDERSTATETYPE, DWORD);
  HRESULT STDMETHODCALLTYPE GetRenderState(D3DRENDERSTATETYPE, DWORD *);
  HRESULT STDMETHODCALLTYPE BeginStateBlock();
  HRESULT STDMETHODCALLTYPE EndStateBlock(DWORD *);
  HRESULT STDMETHODCALLTYPE ApplyStateBlock(DWORD);
  HRESULT STDMETHODCALLTYPE CaptureStateBlock(DWORD);
  HRESULT STDMETHODCALLTYPE DeleteStateBlock(DWORD);
  HRESULT STDMETHODCALLTYPE CreateStateBlock(D3DSTATEBLOCKTYPE, DWORD *);
  HRESULT STDMETHODCALLTYPE GetTexture(DWORD, IDirect3DBaseTexture8 **);
  HRESULT STDMETHODCALLTYPE SetTexture(DWORD, IDirect3DBaseTexture8 *);
  HRESULT STDMETHODCALLTYPE GetTextureStageState(DWORD, D3DTEXTURESTAGESTATETYPE, DWORD *);
  HRESULT STDMETHODCALLTYPE SetTextureStageState(DWORD, D3DTEXTURESTAGESTATETYPE, DWORD);
  HRESULT STDMETHODCALLTYPE DrawPrimitive(D3DPRIMITIVETYPE, UINT, UINT);
  HRESULT STDMETHODCALLTYPE DrawIndexedPrimitive(D3DPRIMITIVETYPE, UINT, UINT, UINT, UINT);
  HRESULT STDMETHODCALLTYPE DrawPrimitiveUP(D3DPRIMITIVETYPE, UINT, const void *, UINT);
  HRESULT STDMETHODCALLTYPE DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE, UINT, UINT, UINT, const void *, D3DFORMAT, const void *, UINT);
  HRESULT STDMETHODCALLTYPE CreateVertexShader(const DWORD *, const DWORD *, DWORD *, DWORD);
  HRESULT STDMETHODCALLTYPE SetVertexShader(DWORD);
  HRESULT STDMETHODCALLTYPE GetVertexShader(DWORD *);
  HRESULT STDMETHODCALLTYPE DeleteVertexShader(DWORD);
  HRESULT STDMETHODCALLTYPE SetStreamSource(UINT, IDirect3DVertexBuffer8 *, UINT);
  HRESULT STDMETHODCALLTYPE GetStreamSource(UINT, IDirect3DVertexBuffer8 **, UINT *);
  HRESULT STDMETHODCALLTYPE SetIndices(IDirect3DIndexBuffer8 *, UINT);
  HRESULT STDMETHODCALLTYPE GetIndices(IDirect3DIndexBuffer8 **, UINT *);
};

struct D8 final : public IDirect3D8_Stub {
  LONG ref = 1;
  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, void **);
  ULONG STDMETHODCALLTYPE AddRef();
  ULONG STDMETHODCALLTYPE Release();
  UINT STDMETHODCALLTYPE GetAdapterCount() { return 1; }
  HRESULT STDMETHODCALLTYPE GetAdapterIdentifier(UINT, DWORD, D3DADAPTER_IDENTIFIER8 *);
  UINT STDMETHODCALLTYPE GetAdapterModeCount(UINT);
  HRESULT STDMETHODCALLTYPE EnumAdapterModes(UINT, UINT, D3DDISPLAYMODE *);
  HRESULT STDMETHODCALLTYPE GetAdapterDisplayMode(UINT, D3DDISPLAYMODE *);
  HRESULT STDMETHODCALLTYPE CheckDeviceType(UINT, D3DDEVTYPE, D3DFORMAT, D3DFORMAT, WINBOOL);
  HRESULT STDMETHODCALLTYPE CheckDeviceFormat(UINT, D3DDEVTYPE, D3DFORMAT, DWORD, D3DRESOURCETYPE, D3DFORMAT);
  HRESULT STDMETHODCALLTYPE CheckDeviceMultiSampleType(UINT, D3DDEVTYPE, D3DFORMAT, WINBOOL, D3DMULTISAMPLE_TYPE);
  HRESULT STDMETHODCALLTYPE CheckDepthStencilMatch(UINT, D3DDEVTYPE, D3DFORMAT, D3DFORMAT, D3DFORMAT);
  HRESULT STDMETHODCALLTYPE GetDeviceCaps(UINT, D3DDEVTYPE, D3DCAPS8 *);
  HMONITOR STDMETHODCALLTYPE GetAdapterMonitor(UINT);
  HRESULT STDMETHODCALLTYPE CreateDevice(UINT, D3DDEVTYPE, HWND, DWORD, D3DPRESENT_PARAMETERS *, IDirect3DDevice8 **);
};
void d8_caps(D3DCAPS8 *c);
