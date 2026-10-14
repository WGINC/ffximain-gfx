// d8_device.cpp -- d3d8on11: IDirect3DDevice8, milestone 1 (docs/D3D11.md phase 1; kit/d3d8on11/README.md).
//
// Milestone 1 makes the device, its swap chain, resources and their uploads, Clear, Present, Reset, render targets,
// copies and read-backs, and keeps every state the game sets and reads back, state blocks included. Draws are
// recorded and counted but not executed: the fixed-function shaders that execute them are the next milestone.
#include "d8.h"
#include <string>

static DWORD fbits(float f) { DWORD d; memcpy(&d, &f, 4); return d; }
static void identity(D3DMATRIX *m) { memset(m, 0, sizeof *m); m->_11 = m->_22 = m->_33 = m->_44 = 1; }

// ---- creation, swap chain, defaults ----------------------------------------------------------------------------------
HRESULT Dev::init(D8 *o, HWND focus, DWORD beh, D3DPRESENT_PARAMETERS *p) {
  d3d = o; d3d->AddRef(); behavior = beh; pp = *p;
  wnd = pp.hDeviceWindow ? pp.hDeviceWindow : focus;
  D3D_FEATURE_LEVEL fl = D3D_FEATURE_LEVEL_11_0, got;
  HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, &fl, 1,
                                 D3D11_SDK_VERSION, &dev, &got, &ctx);
  if (FAILED(hr)) { d8_log("D3D11CreateDevice (feature level 11_0): 0x%08lX", (unsigned long)hr); return D3DERR_NOTAVAILABLE; }
  hr = make_swap();
  if (FAILED(hr)) return hr;
  vshaders.push_back(VShader{false, 0, {}, {}});            // index 0 unused: handles start at 3
  memset(&gamma, 0, sizeof gamma);
  for (int i = 0; i < 256; i++) gamma.red[i] = gamma.green[i] = gamma.blue[i] = (WORD)(i * 257);
  defaults();
  d8_log("device: %ux%u, back buffer format %lu, %s, depth %s (format %lu), behavior 0x%lX, window %p",
         pp.BackBufferWidth, pp.BackBufferHeight, (unsigned long)pp.BackBufferFormat, pp.Windowed ? "windowed" : "full screen asked: windowed",
         pp.EnableAutoDepthStencil ? "yes" : "no", (unsigned long)pp.AutoDepthStencilFormat, (unsigned long)beh, (void *)wnd);
  return D3D_OK;
}
HRESULT Dev::make_swap() {
  if (!pp.Windowed) d8_once("fullscreen", "full screen was asked: milestone 1 renders windowed");
  RECT cr; GetClientRect(wnd, &cr);
  if (!pp.BackBufferWidth) pp.BackBufferWidth = cr.right > 0 ? cr.right : 1;     // D3D8: 0 in windowed mode is the client area
  if (!pp.BackBufferHeight) pp.BackBufferHeight = cr.bottom > 0 ? cr.bottom : 1;
  if (pp.BackBufferFormat == D3DFMT_UNKNOWN) pp.BackBufferFormat = D3DFMT_X8R8G8B8;
  if (!pp.BackBufferCount) pp.BackBufferCount = 1;
  IDXGIDevice *xd = NULL; IDXGIAdapter *xa = NULL; IDXGIFactory *xf = NULL;
  dev->QueryInterface(__uuidof(IDXGIDevice), (void **)&xd); xd->GetAdapter(&xa); xa->GetParent(__uuidof(IDXGIFactory), (void **)&xf);
  DXGI_SWAP_CHAIN_DESC sd = {};
  sd.BufferDesc.Width = pp.BackBufferWidth; sd.BufferDesc.Height = pp.BackBufferHeight; sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  sd.SampleDesc.Count = 1; sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; sd.BufferCount = 1; sd.OutputWindow = wnd;
  sd.Windowed = TRUE; sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  HRESULT hr = xf->CreateSwapChain(dev, &sd, &swap);
  xf->MakeWindowAssociation(wnd, DXGI_MWA_NO_ALT_ENTER);
  xf->Release(); xa->Release(); xd->Release();
  if (FAILED(hr)) { d8_log("CreateSwapChain %ux%u: 0x%08lX", pp.BackBufferWidth, pp.BackBufferHeight, (unsigned long)hr); return D3DERR_NOTAVAILABLE; }
  backbuf = new Surf(this, SK_BACKBUFFER, pp.BackBufferWidth, pp.BackBufferHeight, pp.BackBufferFormat, D3DPOOL_DEFAULT, D3DUSAGE_RENDERTARGET);
  swap->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&backbuf->tex);
  backbuf->get_rtv(); backbuf->lockable = (pp.Flags & D3DPRESENTFLAG_LOCKABLE_BACKBUFFER) != 0;
  curRT = backbuf; curRT->AddRef();
  if (pp.EnableAutoDepthStencil) {
    IDirect3DSurface8 *s = NULL;
    if (SUCCEEDED(CreateDepthStencilSurface(pp.BackBufferWidth, pp.BackBufferHeight, pp.AutoDepthStencilFormat, D3DMULTISAMPLE_NONE, &s))) {
      autods = (Surf *)s; curDS = autods; curDS->AddRef();
    }
  }
  Cmd c; c.op = C_SET_TARGETS; c.targets.rt = curRT; c.targets.ds = curDS; push(c);
  return D3D_OK;
}
void Dev::release_swap() {
  flush();
  ctx->OMSetRenderTargets(0, NULL, NULL); ctx->ClearState(); ctx->Flush();
  if (curRT) curRT->Release(); if (curDS) curDS->Release(); curRT = curDS = nullptr;
  if (autods) autods->Release(); autods = nullptr;
  if (backbuf) backbuf->Release(); backbuf = nullptr;
  if (swap) swap->Release(); swap = nullptr;
}
// D3D8's default state (the DirectX 8 SDK reference, the D3DRENDERSTATETYPE and D3DTEXTURESTAGESTATETYPE pages).
// test/test_d3d8on11.py reads every render state and stage state back from a fresh device here and under Wine's own
// d3d8, and lists each difference.
void Dev::defaults() {
  for (int i = 0; i < 8; i++) if (st.tex[i]) st.tex[i]->Release();
  for (int i = 0; i < 16; i++) if (st.stream[i]) st.stream[i]->Release();
  if (st.ib) st.ib->Release();
  memset(st.rs, 0, sizeof st.rs); memset(st.tss, 0, sizeof st.tss); memset(st.tex, 0, sizeof st.tex);
  memset(st.stream, 0, sizeof st.stream); memset(st.stride, 0, sizeof st.stride); st.ib = nullptr; st.baseVertex = 0;
  DWORD *r = st.rs;
  r[D3DRS_ZENABLE] = pp.EnableAutoDepthStencil ? D3DZB_TRUE : D3DZB_FALSE;
  r[D3DRS_FILLMODE] = D3DFILL_SOLID; r[D3DRS_SHADEMODE] = D3DSHADE_GOURAUD; r[D3DRS_ZWRITEENABLE] = TRUE;
  r[D3DRS_LASTPIXEL] = TRUE; r[D3DRS_SRCBLEND] = D3DBLEND_ONE; r[D3DRS_DESTBLEND] = D3DBLEND_ZERO;
  r[D3DRS_CULLMODE] = D3DCULL_CCW; r[D3DRS_ZFUNC] = D3DCMP_LESSEQUAL; r[D3DRS_ALPHAFUNC] = D3DCMP_ALWAYS;
  r[D3DRS_FOGSTART] = fbits(0.0f); r[D3DRS_FOGEND] = fbits(1.0f); r[D3DRS_FOGDENSITY] = fbits(1.0f);
  r[D3DRS_STENCILFAIL] = r[D3DRS_STENCILZFAIL] = r[D3DRS_STENCILPASS] = D3DSTENCILOP_KEEP;
  r[D3DRS_STENCILFUNC] = D3DCMP_ALWAYS; r[D3DRS_STENCILMASK] = r[D3DRS_STENCILWRITEMASK] = 0xFFFFFFFF;
  r[D3DRS_TEXTUREFACTOR] = 0xFFFFFFFF; r[D3DRS_CLIPPING] = TRUE; r[D3DRS_LIGHTING] = TRUE;
  r[D3DRS_COLORVERTEX] = TRUE; r[D3DRS_LOCALVIEWER] = TRUE;
  r[D3DRS_DIFFUSEMATERIALSOURCE] = D3DMCS_COLOR1; r[D3DRS_SPECULARMATERIALSOURCE] = D3DMCS_COLOR2;
  r[D3DRS_AMBIENTMATERIALSOURCE] = D3DMCS_MATERIAL; r[D3DRS_EMISSIVEMATERIALSOURCE] = D3DMCS_MATERIAL;
  r[D3DRS_SOFTWAREVERTEXPROCESSING] = (behavior & D3DCREATE_SOFTWARE_VERTEXPROCESSING) ? TRUE : FALSE;
  r[D3DRS_POINTSIZE] = fbits(1.0f); r[D3DRS_POINTSIZE_MIN] = fbits(0.0f); r[D3DRS_POINTSCALE_A] = fbits(1.0f);
  r[D3DRS_POINTSCALE_B] = fbits(0.0f); r[D3DRS_POINTSCALE_C] = fbits(0.0f);
  r[D3DRS_MULTISAMPLEANTIALIAS] = TRUE; r[D3DRS_MULTISAMPLEMASK] = 0xFFFFFFFF; r[D3DRS_PATCHSEGMENTS] = fbits(1.0f);
  r[D3DRS_POINTSIZE_MAX] = fbits(64.0f); r[D3DRS_COLORWRITEENABLE] = 0xF; r[D3DRS_TWEENFACTOR] = fbits(0.0f);
  r[D3DRS_BLENDOP] = D3DBLENDOP_ADD;
  for (int s = 0; s < 8; s++) {
    DWORD *t = st.tss[s];
    t[D3DTSS_COLOROP] = s ? D3DTOP_DISABLE : D3DTOP_MODULATE; t[D3DTSS_ALPHAOP] = s ? D3DTOP_DISABLE : D3DTOP_SELECTARG1;
    t[D3DTSS_COLORARG1] = t[D3DTSS_ALPHAARG1] = D3DTA_TEXTURE; t[D3DTSS_COLORARG2] = t[D3DTSS_ALPHAARG2] = D3DTA_CURRENT;
    t[D3DTSS_TEXCOORDINDEX] = s;
    t[D3DTSS_ADDRESSU] = t[D3DTSS_ADDRESSV] = t[D3DTSS_ADDRESSW] = D3DTADDRESS_WRAP;
    t[D3DTSS_MAGFILTER] = t[D3DTSS_MINFILTER] = D3DTEXF_POINT; t[D3DTSS_MIPFILTER] = D3DTEXF_NONE;
    t[D3DTSS_MAXANISOTROPY] = 1; t[D3DTSS_COLORARG0] = t[D3DTSS_ALPHAARG0] = t[D3DTSS_RESULTARG] = D3DTA_CURRENT;
  }
  for (int i = 0; i < 512; i++) { identity(&st.xf[i]); st.xfSet[i] = false; }
  st.vp = {0, 0, pp.BackBufferWidth, pp.BackBufferHeight, 0.0f, 1.0f};
  memset(&st.mat, 0, sizeof st.mat); st.light.clear(); st.lightOn.clear(); st.vs = 0; st.ps = 0;
}

// ---- the executor -------------------------------------------------------------------------------------------------------
void Dev::flush() {
  if (q.empty()) return;
  std::vector<Cmd> run; run.swap(q);
  for (const Cmd &c : run) exec(c);
}
static void argb(D3DCOLOR c, float *f) {
  f[0] = ((c >> 16) & 255) / 255.0f; f[1] = ((c >> 8) & 255) / 255.0f; f[2] = (c & 255) / 255.0f; f[3] = (c >> 24) / 255.0f;
}
void Dev::exec(const Cmd &c) {
  switch (c.op) {
  case C_SET_TARGETS: {
    ID3D11RenderTargetView *rv = c.targets.rt ? c.targets.rt->get_rtv() : NULL;
    ctx->OMSetRenderTargets(1, &rv, c.targets.ds ? c.targets.ds->dsv : NULL);
    break; }
  case C_CLEAR: {
    if ((c.clear.flags & D3DCLEAR_TARGET) && curRT && curRT->get_rtv()) { float f[4]; argb(c.clear.color, f); ctx->ClearRenderTargetView(curRT->get_rtv(), f); }
    UINT df = ((c.clear.flags & D3DCLEAR_ZBUFFER) ? D3D11_CLEAR_DEPTH : 0) | ((c.clear.flags & D3DCLEAR_STENCIL) ? D3D11_CLEAR_STENCIL : 0);
    if (df && curDS && curDS->dsv) ctx->ClearDepthStencilView(curDS->dsv, df, c.clear.z, (UINT8)c.clear.stencil);
    break; }
  case C_PRESENT: {
    HRESULT hr = swap->Present(0, 0);
    if (FAILED(hr)) d8_once("present", "IDXGISwapChain::Present: 0x%08lX (first failure)", (unsigned long)hr);
    break; }
  case C_UPLOAD_TEX: {
    TexCore *t = c.uptex.t; UINT l = c.uptex.sub % t->levels, face = c.uptex.sub / t->levels;
    const std::vector<uint8_t> &b = t->cpu[c.uptex.sub];
    if (!b.empty() && t->gpu) ctx->UpdateSubresource(t->gpu, D3D11CalcSubresource(l, face, t->levels), NULL, b.data(), d8_pitch(t->fmt, t->lw(l)), 0);
    break; }
  case C_UPLOAD_BUF: {
    D3D11_BOX bx = {c.upbuf.lo, 0, 0, c.upbuf.hi, 1, 1};
    ctx->UpdateSubresource(c.upbuf.b->gpu, 0, &bx, c.upbuf.b->cpu.data() + c.upbuf.lo, 0, 0);
    break; }
  case C_COPY: {
    Surf *s = c.copy.src, *d = c.copy.dst;
    D3D11_TEXTURE2D_DESC sd, dd; s->tex->GetDesc(&sd); d->tex->GetDesc(&dd);
    UINT ss = s->kind == SK_TEXLEVEL ? D3D11CalcSubresource(s->level, s->face, sd.MipLevels) : 0;
    UINT ds = d->kind == SK_TEXLEVEL ? D3D11CalcSubresource(d->level, d->face, dd.MipLevels) : 0;
    D3D11_BOX bx = {(UINT)c.copy.r.left, (UINT)c.copy.r.top, 0, (UINT)c.copy.r.right, (UINT)c.copy.r.bottom, 1};
    ctx->CopySubresourceRegion(d->tex, ds, c.copy.p.x, c.copy.p.y, 0, s->tex, ss, &bx);
    break; }
  case C_DRAW:
    drawsNotExecuted++;
    break;
  }
}

// ---- IUnknown and the simple queries ------------------------------------------------------------------------------------
HRESULT Dev::QueryInterface(REFIID r, void **o) {
  if (IsEqualGUID(r, IID_IUnknown) || IsEqualGUID(r, IID_IDirect3DDevice8)) { *o = this; AddRef(); return S_OK; }
  *o = NULL; return E_NOINTERFACE;
}
ULONG Dev::AddRef() { return InterlockedIncrement(&ref); }
ULONG Dev::Release() {
  LONG n = InterlockedDecrement(&ref);
  if (n) return n;
  d8_log("device released: %llu draws recorded, %llu of them not executed (milestone 1)", (unsigned long long)draws, (unsigned long long)drawsNotExecuted);
  for (Block *b : blocks) delete b;
  st.vs = 0; defaults();
  release_swap();
  if (ctx) ctx->Release(); if (dev) dev->Release();
  d3d->Release();
  delete this;
  return 0;
}
HRESULT Dev::TestCooperativeLevel() { return D3D_OK; }
UINT Dev::GetAvailableTextureMem() { return 512u << 20; }
HRESULT Dev::GetDirect3D(IDirect3D8 **o) { if (!o) return D3DERR_INVALIDCALL; *o = d3d; d3d->AddRef(); return D3D_OK; }
HRESULT Dev::GetDeviceCaps(D3DCAPS8 *c) { if (!c) return D3DERR_INVALIDCALL; d8_caps(c); return D3D_OK; }
HRESULT Dev::GetDisplayMode(D3DDISPLAYMODE *m) { return d3d->GetAdapterDisplayMode(0, m); }
HRESULT Dev::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *p) {
  if (!p) return D3DERR_INVALIDCALL;
  p->AdapterOrdinal = 0; p->DeviceType = D3DDEVTYPE_HAL; p->hFocusWindow = wnd; p->BehaviorFlags = behavior; return D3D_OK;
}
static BOOL g_cursorShown = TRUE;
WINBOOL Dev::ShowCursor(WINBOOL show) { BOOL was = g_cursorShown; g_cursorShown = show; d8_once("cursor", "ShowCursor: kept, the device draws no cursor in milestone 1"); return was; }
void Dev::SetGammaRamp(DWORD, const D3DGAMMARAMP *r) { if (r) gamma = *r; d8_once("gamma", "SetGammaRamp: kept, not applied (windowed)"); }
void Dev::GetGammaRamp(D3DGAMMARAMP *r) { if (r) *r = gamma; }

// ---- frame ------------------------------------------------------------------------------------------------------------
HRESULT Dev::Reset(D3DPRESENT_PARAMETERS *p) {
  if (!p) return D3DERR_INVALIDCALL;
  release_swap();
  pp = *p; if (pp.hDeviceWindow) wnd = pp.hDeviceWindow;
  HRESULT hr = make_swap();
  if (FAILED(hr)) return D3DERR_DEVICELOST;
  defaults();
  d8_log("Reset: %ux%u", pp.BackBufferWidth, pp.BackBufferHeight);
  return D3D_OK;
}
HRESULT Dev::Present(const RECT *s, const RECT *d, HWND w, const RGNDATA *) {
  if (s || d || w) d8_once("presentargs", "Present with rectangles or another window: they are not used in milestone 1");
  Cmd c; c.op = C_PRESENT; push(c); flush();
  return D3D_OK;
}
HRESULT Dev::GetBackBuffer(UINT i, D3DBACKBUFFER_TYPE, IDirect3DSurface8 **o) {
  if (i || !o || !backbuf) return D3DERR_INVALIDCALL;
  *o = backbuf; backbuf->AddRef(); return D3D_OK;
}
HRESULT Dev::BeginScene() { if (inScene) return D3DERR_INVALIDCALL; inScene = true; return D3D_OK; }
HRESULT Dev::EndScene() { if (!inScene) return D3DERR_INVALIDCALL; inScene = false; return D3D_OK; }
HRESULT Dev::Clear(DWORD n, const D3DRECT *r, DWORD flags, D3DCOLOR color, float z, DWORD stencil) {
  // D3D8 clears the viewport (or rectangles in it); a D3D11 view clear covers the whole target, so only that case is done
  bool whole = st.vp.X == 0 && st.vp.Y == 0 && curRT && st.vp.Width >= curRT->w && st.vp.Height >= curRT->h;
  if (n || (r && n) || !whole) {
    d8_once("clearrect", "Clear of rectangles or of a viewport smaller than the target: not implemented (failed; first: %lu rects, viewport %lux%lu at %lu,%lu)",
            (unsigned long)n, (unsigned long)st.vp.Width, (unsigned long)st.vp.Height, (unsigned long)st.vp.X, (unsigned long)st.vp.Y);
    return D3DERR_INVALIDCALL;
  }
  Cmd c; c.op = C_CLEAR; c.clear.flags = flags; c.clear.color = color; c.clear.z = z; c.clear.stencil = stencil; push(c);
  return D3D_OK;
}

// ---- resources --------------------------------------------------------------------------------------------------------
HRESULT Dev::CreateTexture(UINT w, UINT h, UINT l, DWORD u, D3DFORMAT f, D3DPOOL p, IDirect3DTexture8 **o) {
  if (!o || !w || !h) return D3DERR_INVALIDCALL;
  Tex *t = new Tex(); HRESULT hr = t->c.init(this, w, h, l, 1, u, f, p);
  if (FAILED(hr)) { delete t; *o = NULL; return hr; }
  *o = t; return D3D_OK;
}
HRESULT Dev::CreateCubeTexture(UINT e, UINT l, DWORD u, D3DFORMAT f, D3DPOOL p, IDirect3DCubeTexture8 **o) {
  if (!o || !e) return D3DERR_INVALIDCALL;
  CubeTex *t = new CubeTex(); HRESULT hr = t->c.init(this, e, e, l, 6, u, f, p);
  if (FAILED(hr)) { delete t; *o = NULL; return hr; }
  *o = t; return D3D_OK;
}
HRESULT Dev::CreateVertexBuffer(UINT len, DWORD u, DWORD fvf, D3DPOOL p, IDirect3DVertexBuffer8 **o) {
  if (!o) return D3DERR_INVALIDCALL;
  VB *b = new VB(); HRESULT hr = b->c.init(this, len, u, fvf, D3DFMT_VERTEXDATA, p, false);
  if (FAILED(hr)) { delete b; *o = NULL; return hr; }
  *o = b; return D3D_OK;
}
HRESULT Dev::CreateIndexBuffer(UINT len, DWORD u, D3DFORMAT f, D3DPOOL p, IDirect3DIndexBuffer8 **o) {
  if (!o || (f != D3DFMT_INDEX16 && f != D3DFMT_INDEX32)) return D3DERR_INVALIDCALL;
  IB *b = new IB(); HRESULT hr = b->c.init(this, len, u, 0, f, p, true);
  if (FAILED(hr)) { delete b; *o = NULL; return hr; }
  *o = b; return D3D_OK;
}
HRESULT Dev::CreateRenderTarget(UINT w, UINT h, D3DFORMAT f, D3DMULTISAMPLE_TYPE ms, WINBOOL lockable, IDirect3DSurface8 **o) {
  if (!o) return D3DERR_INVALIDCALL;
  FmtInfo fi = d8_fmt(f);
  if (ms != D3DMULTISAMPLE_NONE || fi.dxgi == DXGI_FORMAT_UNKNOWN || fi.depth || fi.block) { d8_notimpl("CreateRenderTarget of this format or multisampling"); return D3DERR_INVALIDCALL; }
  Surf *s = new Surf(this, SK_RT, w, h, f, D3DPOOL_DEFAULT, D3DUSAGE_RENDERTARGET); s->lockable = lockable != 0;
  D3D11_TEXTURE2D_DESC td = {}; td.Width = w; td.Height = h; td.MipLevels = 1; td.ArraySize = 1; td.Format = fi.dxgi;
  td.SampleDesc.Count = 1; td.Usage = D3D11_USAGE_DEFAULT; td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
  if (FAILED(dev->CreateTexture2D(&td, NULL, &s->tex))) { delete s; return D3DERR_OUTOFVIDEOMEMORY; }
  *o = s; return D3D_OK;
}
HRESULT Dev::CreateDepthStencilSurface(UINT w, UINT h, D3DFORMAT f, D3DMULTISAMPLE_TYPE ms, IDirect3DSurface8 **o) {
  if (!o) return D3DERR_INVALIDCALL;
  FmtInfo fi = d8_fmt(f);
  if (ms != D3DMULTISAMPLE_NONE || !fi.depth) { d8_notimpl("CreateDepthStencilSurface of this format or multisampling"); return D3DERR_INVALIDCALL; }
  Surf *s = new Surf(this, SK_DS, w, h, f, D3DPOOL_DEFAULT, D3DUSAGE_DEPTHSTENCIL);
  D3D11_TEXTURE2D_DESC td = {}; td.Width = w; td.Height = h; td.MipLevels = 1; td.ArraySize = 1; td.Format = fi.dxgi;
  td.SampleDesc.Count = 1; td.Usage = D3D11_USAGE_DEFAULT; td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  if (FAILED(dev->CreateTexture2D(&td, NULL, &s->tex)) || FAILED(dev->CreateDepthStencilView(s->tex, NULL, &s->dsv))) { delete s; return D3DERR_OUTOFVIDEOMEMORY; }
  *o = s; return D3D_OK;
}
HRESULT Dev::CreateImageSurface(UINT w, UINT h, D3DFORMAT f, IDirect3DSurface8 **o) {
  if (!o || d8_fmt(f).dxgi == DXGI_FORMAT_UNKNOWN) return D3DERR_INVALIDCALL;
  Surf *s = new Surf(this, SK_IMAGE, w, h, f, D3DPOOL_SYSTEMMEM, 0);
  s->cpu.assign((size_t)d8_pitch(f, w) * d8_rows(f, h), 0);
  *o = s; return D3D_OK;
}
HRESULT Dev::CopyRects(IDirect3DSurface8 *src, const RECT *rects, UINT n, IDirect3DSurface8 *dst, const POINT *pts) {
  Surf *s = (Surf *)src, *d = (Surf *)dst;
  if (!s || !d || s->fmt != d->fmt) return D3DERR_INVALIDCALL;
  RECT whole = {0, 0, (LONG)s->w, (LONG)s->h}; POINT zero = {0, 0};
  UINT cnt = rects && n ? n : 1;
  for (UINT i = 0; i < cnt; i++) {
    RECT r = rects && n ? rects[i] : whole; POINT p = pts ? pts[i] : POINT{r.left, r.top};
    if (!rects || !n) p = zero;
    if (s->tex && d->tex) {                                   // GPU to GPU
      Cmd c; c.op = C_COPY; c.copy.src = s; c.copy.dst = d; c.copy.r = r; c.copy.p = p; push(c); flush();
    } else if (s->tex && d->kind == SK_IMAGE) {               // GPU to system memory: a read back
      D3DLOCKED_RECT lr;
      if (FAILED(s->LockRect(&lr, NULL, D3DLOCK_READONLY))) return D3DERR_INVALIDCALL;
      UINT bpp = d8_fmt(s->fmt).bpp / 8;
      for (LONG y = r.top; y < r.bottom; y++)
        memcpy(&d->cpu[(size_t)(p.y + y - r.top) * d8_pitch(d->fmt, d->w) + (size_t)p.x * bpp], (uint8_t *)lr.pBits + (size_t)y * lr.Pitch + (size_t)r.left * bpp, (size_t)(r.right - r.left) * bpp);
      s->UnlockRect();
    } else {
      d8_notimpl("CopyRects between these kinds of surface");
      return D3DERR_INVALIDCALL;
    }
  }
  return D3D_OK;
}
HRESULT Dev::SetRenderTarget(IDirect3DSurface8 *rt, IDirect3DSurface8 *ds) {
  Surf *r = (Surf *)rt, *z = (Surf *)ds;
  if (r && r->kind != SK_RT && r->kind != SK_BACKBUFFER && !(r->kind == SK_TEXLEVEL && (r->usage & D3DUSAGE_RENDERTARGET))) return D3DERR_INVALIDCALL;
  if (z && z->kind != SK_DS) return D3DERR_INVALIDCALL;
  if (r) { r->AddRef(); if (curRT) curRT->Release(); curRT = r; }       // D3D8: a NULL target keeps the current one
  if (z) z->AddRef(); if (curDS) curDS->Release(); curDS = z;
  st.vp = {0, 0, curRT->w, curRT->h, 0.0f, 1.0f};                       // D3D8: setting a target resets the viewport
  Cmd c; c.op = C_SET_TARGETS; c.targets.rt = curRT; c.targets.ds = curDS; push(c);
  return D3D_OK;
}
HRESULT Dev::GetRenderTarget(IDirect3DSurface8 **o) { if (!o || !curRT) return D3DERR_INVALIDCALL; *o = curRT; curRT->AddRef(); return D3D_OK; }
HRESULT Dev::GetDepthStencilSurface(IDirect3DSurface8 **o) {
  if (!o) return D3DERR_INVALIDCALL;
  *o = curDS; if (!curDS) return D3DERR_NOTFOUND;
  curDS->AddRef(); return D3D_OK;
}

// ---- state (kept for Get*, recorded into a state block between BeginStateBlock and EndStateBlock) -------------------------
// While a block records, a set goes into the block only: the device's state does not change (probe8 checks that).
#define TARGET (rec ? rec->s : st)
HRESULT Dev::SetTransform(D3DTRANSFORMSTATETYPE t, const D3DMATRIX *m) {
  if ((UINT)t >= 512 || !m) return D3DERR_INVALIDCALL;
  TARGET.xf[t] = *m; if (rec) rec->xf[t] = true; else st.xfSet[t] = true;
  return D3D_OK;
}
HRESULT Dev::GetTransform(D3DTRANSFORMSTATETYPE t, D3DMATRIX *m) { if ((UINT)t >= 512 || !m) return D3DERR_INVALIDCALL; *m = st.xf[t]; return D3D_OK; }
HRESULT Dev::MultiplyTransform(D3DTRANSFORMSTATETYPE t, const D3DMATRIX *m) {
  if ((UINT)t >= 512 || !m) return D3DERR_INVALIDCALL;
  D3DMATRIX a = st.xf[t], o;                                  // D3D8: the new matrix is m times the current one
  for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) {
    float s = 0; for (int k = 0; k < 4; k++) s += m->m[i][k] * a.m[k][j];
    o.m[i][j] = s;
  }
  return SetTransform(t, &o);
}
HRESULT Dev::SetViewport(const D3DVIEWPORT8 *v) { if (!v) return D3DERR_INVALIDCALL; TARGET.vp = *v; if (rec) rec->vp = true; return D3D_OK; }
HRESULT Dev::GetViewport(D3DVIEWPORT8 *v) { if (!v) return D3DERR_INVALIDCALL; *v = st.vp; return D3D_OK; }
HRESULT Dev::SetMaterial(const D3DMATERIAL8 *m) { if (!m) return D3DERR_INVALIDCALL; TARGET.mat = *m; if (rec) rec->mat = true; return D3D_OK; }
HRESULT Dev::GetMaterial(D3DMATERIAL8 *m) { if (!m) return D3DERR_INVALIDCALL; *m = st.mat; return D3D_OK; }
static void grow(State &s, DWORD i) {
  if (s.light.size() <= i) {
    D3DLIGHT8 def = {}; def.Type = D3DLIGHT_DIRECTIONAL; def.Diffuse.r = def.Diffuse.g = def.Diffuse.b = 1.0f; def.Direction.z = 1.0f;
    s.light.resize(i + 1, def); s.lightOn.resize(i + 1, FALSE);       // D3D8: LightEnable on an unset light sets this default
  }
}
HRESULT Dev::SetLight(DWORD i, const D3DLIGHT8 *l) {
  if (!l || i > 0xFFFF) return D3DERR_INVALIDCALL;
  State &s = TARGET; grow(s, i); s.light[i] = *l; if (rec) rec->lights.push_back((int)i);
  return D3D_OK;
}
HRESULT Dev::GetLight(DWORD i, D3DLIGHT8 *l) { if (!l || i >= st.light.size()) return D3DERR_INVALIDCALL; *l = st.light[i]; return D3D_OK; }
HRESULT Dev::LightEnable(DWORD i, WINBOOL on) {
  if (i > 0xFFFF) return D3DERR_INVALIDCALL;
  State &s = TARGET; grow(s, i); s.lightOn[i] = on ? TRUE : FALSE; if (rec) rec->lights.push_back((int)i);
  return D3D_OK;
}
HRESULT Dev::GetLightEnable(DWORD i, WINBOOL *on) { if (!on || i >= st.lightOn.size()) return D3DERR_INVALIDCALL; *on = st.lightOn[i]; return D3D_OK; }
HRESULT Dev::SetRenderState(D3DRENDERSTATETYPE s, DWORD v) {
  if ((UINT)s >= 256) return D3DERR_INVALIDCALL;
  TARGET.rs[s] = v; if (rec) rec->rs[s] = true;
  return D3D_OK;
}
HRESULT Dev::GetRenderState(D3DRENDERSTATETYPE s, DWORD *v) { if ((UINT)s >= 256 || !v) return D3DERR_INVALIDCALL; *v = st.rs[s]; return D3D_OK; }
HRESULT Dev::GetTextureStageState(DWORD stg, D3DTEXTURESTAGESTATETYPE t, DWORD *v) {
  if (stg >= 8 || (UINT)t >= 32 || !v) return D3DERR_INVALIDCALL;
  *v = st.tss[stg][t]; return D3D_OK;
}
HRESULT Dev::SetTextureStageState(DWORD stg, D3DTEXTURESTAGESTATETYPE t, DWORD v) {
  if (stg >= 8 || (UINT)t >= 32) return D3DERR_INVALIDCALL;
  TARGET.tss[stg][t] = v; if (rec) rec->tss[stg][t] = true;
  return D3D_OK;
}
// bound objects hold a reference, as D3D8 does
template <class T> static void rebind(T *&slot, T *v) { if (v) v->AddRef(); if (slot) slot->Release(); slot = v; }
HRESULT Dev::SetTexture(DWORD stg, IDirect3DBaseTexture8 *t) {
  if (stg >= 8) return D3DERR_INVALIDCALL;
  if (rec) { rebind(rec->s.tex[stg], t); rec->tex[stg] = true; } else rebind(st.tex[stg], t);
  return D3D_OK;
}
HRESULT Dev::GetTexture(DWORD stg, IDirect3DBaseTexture8 **t) {
  if (stg >= 8 || !t) return D3DERR_INVALIDCALL;
  *t = st.tex[stg]; if (*t) (*t)->AddRef(); return D3D_OK;
}
HRESULT Dev::SetStreamSource(UINT n, IDirect3DVertexBuffer8 *b, UINT stride) {
  if (n >= 16) return D3DERR_INVALIDCALL;
  State &s = TARGET; VB *v = (VB *)b; rebind(s.stream[n], v); s.stride[n] = stride; if (rec) rec->stream[n] = true;
  return D3D_OK;
}
HRESULT Dev::GetStreamSource(UINT n, IDirect3DVertexBuffer8 **b, UINT *stride) {
  if (n >= 16 || !b || !stride) return D3DERR_INVALIDCALL;
  *b = st.stream[n]; if (*b) (*b)->AddRef(); *stride = st.stride[n]; return D3D_OK;
}
HRESULT Dev::SetIndices(IDirect3DIndexBuffer8 *b, UINT base) {
  State &s = TARGET; IB *i = (IB *)b; rebind(s.ib, i); s.baseVertex = base; if (rec) rec->ib = true;
  return D3D_OK;
}
HRESULT Dev::GetIndices(IDirect3DIndexBuffer8 **b, UINT *base) {
  if (!b || !base) return D3DERR_INVALIDCALL;
  *b = st.ib; if (*b) (*b)->AddRef(); *base = st.baseVertex; return D3D_OK;
}

// ---- state blocks -----------------------------------------------------------------------------------------------------
static void copy_block(State &to, const State &from, const Block &b) {
  for (int i = 0; i < 256; i++) if (b.rs[i]) to.rs[i] = from.rs[i];
  for (int s = 0; s < 8; s++) for (int t = 0; t < 32; t++) if (b.tss[s][t]) to.tss[s][t] = from.tss[s][t];
  for (int s = 0; s < 8; s++) if (b.tex[s]) rebind(to.tex[s], from.tex[s]);
  for (int i = 0; i < 512; i++) if (b.xf[i]) to.xf[i] = from.xf[i];
  if (b.vp) to.vp = from.vp;
  if (b.mat) to.mat = from.mat;
  for (int n = 0; n < 16; n++) if (b.stream[n]) { rebind(to.stream[n], from.stream[n]); to.stride[n] = from.stride[n]; }
  if (b.ib) { rebind(to.ib, from.ib); to.baseVertex = from.baseVertex; }
  if (b.vs) to.vs = from.vs;
  if (b.ps) to.ps = from.ps;
  for (int i : b.lights) {
    grow(to, i);
    if ((size_t)i < from.light.size()) { to.light[i] = from.light[i]; to.lightOn[i] = from.lightOn[i]; }
  }
}
static Block *new_block() {
  Block *b = new Block(); memset((void *)b, 0, offsetof(Block, lights));
  new (&b->lights) std::vector<int>(); new (&b->s) State();
  memset(b->s.tex, 0, sizeof b->s.tex); memset(b->s.stream, 0, sizeof b->s.stream); b->s.ib = nullptr;
  return b;
}
static void free_block(Block *b) {
  for (int i = 0; i < 8; i++) if (b->s.tex[i]) b->s.tex[i]->Release();
  for (int i = 0; i < 16; i++) if (b->s.stream[i]) b->s.stream[i]->Release();
  if (b->s.ib) b->s.ib->Release();
  delete b;
}
HRESULT Dev::BeginStateBlock() { if (rec) return D3DERR_INVALIDCALL; rec = new_block(); return D3D_OK; }
HRESULT Dev::EndStateBlock(DWORD *tok) {
  if (!rec || !tok) return D3DERR_INVALIDCALL;
  blocks.push_back(rec); rec = nullptr; *tok = (DWORD)blocks.size();
  return D3D_OK;
}
static Block *find(std::vector<Block *> &v, DWORD t) { return t && t <= v.size() ? v[t - 1] : nullptr; }
HRESULT Dev::ApplyStateBlock(DWORD t) {
  Block *b = find(blocks, t);
  if (!b || rec) return D3DERR_INVALIDCALL;                  // D3D8: no apply while another block records
  copy_block(st, b->s, *b);
  return D3D_OK;
}
HRESULT Dev::CaptureStateBlock(DWORD t) {
  Block *b = find(blocks, t);
  if (!b || rec) return D3DERR_INVALIDCALL;
  copy_block(b->s, st, *b);
  return D3D_OK;
}
HRESULT Dev::DeleteStateBlock(DWORD t) {
  Block *b = find(blocks, t);
  if (!b) return D3DERR_INVALIDCALL;
  free_block(b); blocks[t - 1] = nullptr;
  return D3D_OK;
}
HRESULT Dev::CreateStateBlock(D3DSTATEBLOCKTYPE type, DWORD *tok) {
  if (!tok || rec) return D3DERR_INVALIDCALL;
  if (type != D3DSBT_ALL) { d8_notimpl("CreateStateBlock of the pixel or vertex state only"); return D3DERR_INVALIDCALL; }
  Block *b = new_block();
  memset(b->rs, 1, sizeof b->rs); memset(b->tss, 1, sizeof b->tss); memset(b->tex, 1, sizeof b->tex);
  memset(b->xf, 1, sizeof b->xf); b->vp = b->mat = b->ib = b->vs = b->ps = true; memset(b->stream, 1, sizeof b->stream);
  for (size_t i = 0; i < st.light.size(); i++) b->lights.push_back((int)i);
  copy_block(b->s, st, *b);
  blocks.push_back(b); *tok = (DWORD)blocks.size();
  return D3D_OK;
}

// ---- shaders ------------------------------------------------------------------------------------------------------------
static size_t decl_len(const DWORD *d) {                    // tokens up to and with D3DVSD_END
  size_t i = 0;
  for (;;) {
    DWORD t = d[i];
    if (t == D3DVSD_END()) return i + 1;
    DWORD ty = (t & D3DVSD_TOKENTYPEMASK) >> D3DVSD_TOKENTYPESHIFT;
    if (ty == D3DVSD_TOKEN_CONSTMEM) i += 1 + 4 * ((t & D3DVSD_CONSTCOUNTMASK) >> D3DVSD_CONSTCOUNTSHIFT);
    else if (ty == D3DVSD_TOKEN_EXT) i += 1 + ((t & D3DVSD_EXTCOUNTMASK) >> D3DVSD_EXTCOUNTSHIFT);
    else i++;
  }
}
static size_t code_len(const DWORD *c) { size_t i = 0; while (c[i] != 0x0000FFFF) i++; return i + 1; }
HRESULT Dev::CreateVertexShader(const DWORD *decl, const DWORD *code, DWORD *h, DWORD) {
  if (!decl || !h) return D3DERR_INVALIDCALL;
  VShader v; v.used = true; v.fvf = 0;
  v.decl.assign(decl, decl + decl_len(decl));
  if (code) v.code.assign(code, code + code_len(code));
  vshaders.push_back(v);
  *h = (DWORD)((vshaders.size() - 1) << 1) | 1;              // D3D8 handles have bit 0 set; FVF codes never do
  return D3D_OK;
}
HRESULT Dev::SetVertexShader(DWORD h) {
  if (h & 1) { size_t i = h >> 1; if (i >= vshaders.size() || !vshaders[i].used) return D3DERR_INVALIDCALL; }
  TARGET.vs = h; if (rec) rec->vs = true;
  return D3D_OK;
}
HRESULT Dev::GetVertexShader(DWORD *h) { if (!h) return D3DERR_INVALIDCALL; *h = st.vs; return D3D_OK; }
HRESULT Dev::DeleteVertexShader(DWORD h) {
  size_t i = h >> 1;
  if (!(h & 1) || i >= vshaders.size() || !vshaders[i].used) return D3DERR_INVALIDCALL;
  vshaders[i].used = false; vshaders[i].decl.clear(); vshaders[i].code.clear();
  if (st.vs == h) st.vs = 0;
  return D3D_OK;
}

// ---- draws: recorded and counted; not executed in milestone 1 ----------------------------------------------------------------
static HRESULT draw(Dev *d) {
  d->draws++;
  d8_once("draws", "draws are recorded and counted, not executed (milestone 1: the fixed-function shaders come next)");
  Cmd c; c.op = C_DRAW; c.draw.kind = 0; d->push(c);
  return D3D_OK;
}
HRESULT Dev::DrawPrimitive(D3DPRIMITIVETYPE, UINT, UINT) { return draw(this); }
HRESULT Dev::DrawIndexedPrimitive(D3DPRIMITIVETYPE, UINT, UINT, UINT, UINT) { return draw(this); }
HRESULT Dev::DrawPrimitiveUP(D3DPRIMITIVETYPE, UINT, const void *, UINT) { return draw(this); }
HRESULT Dev::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE, UINT, UINT, UINT, const void *, D3DFORMAT, const void *, UINT) { return draw(this); }
