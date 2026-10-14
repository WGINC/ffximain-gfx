// d8_device.cpp -- d3d8on11: IDirect3DDevice8, milestone 1 (docs/D3D11.md phase 1; kit/d3d8on11/README.md).
//
// Milestone 1 makes the device, its swap chain, resources and their uploads, Clear, Present, Reset, render targets,
// copies and read-backs, and keeps every state the game sets and reads back, state blocks included. Milestone 2
// executes draws through generated shaders (d8_ff.cpp).
#include "d8.h"
#include <string>

static DWORD fbits(float f) { DWORD d; memcpy(&d, &f, 4); return d; }
static void identity(D3DMATRIX *m) { memset(m, 0, sizeof *m); m->_11 = m->_22 = m->_33 = m->_44 = 1; }

// ---- creation, swap chain, defaults ----------------------------------------------------------------------------------
HRESULT Dev::init(D8 *o, HWND focus, DWORD beh, D3DPRESENT_PARAMETERS *p) {
  d3d = o; d3d->AddRef(); behavior = beh; pp = *p;
  wnd = pp.hDeviceWindow ? pp.hDeviceWindow : focus;
  // the executor: on this thread (direct), on its own thread (thread, the default), or in its own process (remote: the
  // ARM64 build on an ARM64 machine). d3d8on11.ini next to the DLL, [d3d8on11] mode=; D8ON11_MODE overrides it
  char mode[32] = "thread", ini[MAX_PATH]; HMODULE m = NULL;
  GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&identity, &m);
  GetModuleFileNameA(m, ini, sizeof ini); char *sl = strrchr(ini, '\\'); strcpy(sl ? sl + 1 : ini, "d3d8on11.ini");
  GetPrivateProfileStringA("d3d8on11", "mode", "thread", mode, sizeof mode, ini);
  char env[32]; DWORD ne = GetEnvironmentVariableA("D8ON11_MODE", env, sizeof env);
  if (ne > 0 && ne < sizeof env) strcpy(mode, env);
  link = link_make(mode);
  if (!link && strcmp(mode, "direct")) { d8_log("mode %s could not start: thread instead", mode); link = link_make("thread"); }
  if (!link) link = link_make("direct");
  d8_log("executor: %s", link->name());
  HRESULT hr = make_swap(false);
  if (FAILED(hr) && strcmp(link->name(), "thread") && strcmp(link->name(), "direct")) {
    // the executor process could not make the device or the swap chain (d3d8on11_exec.log says why): in-process instead
    d8_log("the %s executor could not start the device: thread instead", link->name());
    delete link; link = link_make("thread"); if (!link) link = link_make("direct");
    d8_log("executor: %s", link->name());
    hr = make_swap(false);
  }
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
HRESULT Dev::make_swap(bool resize) {
  if (!pp.Windowed) d8_once("fullscreen", "full screen was asked: rendered windowed");
  RECT cr; GetClientRect(wnd, &cr);
  if (!pp.BackBufferWidth) pp.BackBufferWidth = cr.right > 0 ? cr.right : 1;     // D3D8: 0 in windowed mode is the client area
  if (!pp.BackBufferHeight) pp.BackBufferHeight = cr.bottom > 0 ? cr.bottom : 1;
  if (pp.BackBufferFormat == D3DFMT_UNKNOWN) pp.BackBufferFormat = D3DFMT_X8R8G8B8;
  if (!pp.BackBufferCount) pp.BackBufferCount = 1;
  uint32_t ds_id = pp.EnableAutoDepthStencil ? new_id() : 0;
  if (pp.EnableAutoDepthStencil && (!d8_fmt(pp.AutoDepthStencilFormat).depth)) { d8_notimpl("this automatic depth-stencil format"); ds_id = 0; }
  XInit x = {(uint64_t)(uintptr_t)wnd, pp.BackBufferWidth, pp.BackBufferHeight, ds_id ? (uint32_t)pp.AutoDepthStencilFormat : 0u, ds_id};
  int32_t hr = link->call(resize ? X_RESIZE : X_INIT, &x, sizeof x);
  if (hr < 0) return D3DERR_NOTAVAILABLE;
  backbuf = new Surf(this, SK_BACKBUFFER, pp.BackBufferWidth, pp.BackBufferHeight, pp.BackBufferFormat, D3DPOOL_DEFAULT, D3DUSAGE_RENDERTARGET);
  backbuf->id = X_BACKBUFFER; backbuf->lockable = (pp.Flags & D3DPRESENTFLAG_LOCKABLE_BACKBUFFER) != 0;
  curRT = backbuf; curRT->AddRef();
  if (ds_id) {
    autods = new Surf(this, SK_DS, pp.BackBufferWidth, pp.BackBufferHeight, pp.AutoDepthStencilFormat, D3DPOOL_DEFAULT, D3DUSAGE_DEPTHSTENCIL);
    autods->id = ds_id; curDS = autods; curDS->AddRef();
  }
  targets();
  return D3D_OK;
}
void Dev::targets() {
  XTargets t; memset(&t, 0, sizeof t);
  if (curRT) t.rt = curRT->xref();
  t.ds = curDS ? curDS->id : 0;
  link->put(X_TARGETS, &t, sizeof t);
}
void Dev::release_swap() {
  if (curRT) curRT->Release(); if (curDS) curDS->Release(); curRT = curDS = nullptr;
  if (autods) autods->Release(); autods = nullptr;
  if (backbuf) backbuf->Release(); backbuf = nullptr;
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
  // AMBIENTMATERIALSOURCE: D3DMCS_COLOR2 by the DirectX 8.1 reference (D3DRENDERSTATETYPE); the Direct3D 9 reference
  // gives D3DMCS_MATERIAL, as Wine does. The two differ only for a lit vertex with a specular colour: the game's traced
  // formats have none (FVF codes 0x42, 0x44, 0x104, 0x142, 0x144, 0x152; its declarations map no register 6), and
  // with no specular colour COLOR2 uses the material's ambient colour ("Ambient Lighting"), which is MATERIAL
  r[D3DRS_AMBIENTMATERIALSOURCE] = D3DMCS_COLOR2; r[D3DRS_EMISSIVEMATERIALSOURCE] = D3DMCS_MATERIAL;
  r[D3DRS_SOFTWAREVERTEXPROCESSING] = (behavior & D3DCREATE_SOFTWARE_VERTEXPROCESSING) ? TRUE : FALSE;
  r[D3DRS_POINTSIZE] = fbits(1.0f); r[D3DRS_POINTSIZE_MIN] = fbits(0.0f); r[D3DRS_POINTSCALE_A] = fbits(1.0f);
  r[D3DRS_POINTSCALE_B] = fbits(0.0f); r[D3DRS_POINTSCALE_C] = fbits(0.0f);
  r[D3DRS_MULTISAMPLEANTIALIAS] = TRUE; r[D3DRS_MULTISAMPLEMASK] = 0xFFFFFFFF; r[D3DRS_PATCHSEGMENTS] = fbits(1.0f);
  r[D3DRS_POINTSIZE_MAX] = fbits(64.0f); r[D3DRS_COLORWRITEENABLE] = 0xF; r[D3DRS_TWEENFACTOR] = fbits(0.0f);
  r[D3DRS_BLENDOP] = D3DBLENDOP_ADD;
  // N-patch interpolation orders: D3DORDER_CUBIC (3) and D3DORDER_LINEAR (1), the DirectX 8.1 reference's defaults
  // and d3d8types.h's comments; not drawn here (no N-patches), only answered
  r[D3DRS_POSITIONORDER] = 3; r[D3DRS_NORMALORDER] = 1;
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
  memset(&st.mat, 0, sizeof st.mat); st.light.clear(); st.lightOn.clear(); st.vs = 0; st.ps = 0; memset(st.clip, 0, sizeof st.clip);
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
  d8_log("device released: %llu draws, %llu of them not executed", (unsigned long long)draws, (unsigned long long)drawsNotExecuted);
  for (Block *b : blocks) delete b;
  st.vs = 0; defaults();
  release_swap();
  delete link; link = nullptr;
  d3d->Release();
  delete this;
  return 0;
}
// lost devices (DirectX 8.1, IDirect3DDevice8::Reset and TestCooperativeLevel): "If a call to Reset fails, the device
// will be placed in the 'lost' state"; a device that can be reset answers D3DERR_DEVICENOTRESET, and "Present will
// return D3DERR_DEVICELOST if the device is either 'lost' or 'not reset'". This device is windowed and loses nothing
// on its own (no full-screen focus to lose), so it is only ever "not reset", after a failed Reset
HRESULT Dev::TestCooperativeLevel() { return notReset ? D3DERR_DEVICENOTRESET : D3D_OK; }
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
  HRESULT hr = make_swap(true);
  if (FAILED(hr)) { notReset = true; return D3DERR_DEVICELOST; }
  notReset = false;
  defaults();
  d8_log("Reset: %ux%u", pp.BackBufferWidth, pp.BackBufferHeight);
  return D3D_OK;
}
HRESULT Dev::Present(const RECT *s, const RECT *d, HWND w, const RGNDATA *) {
  if (notReset) return D3DERR_DEVICELOST;
  if (s || d || w) d8_once("presentargs", "Present with rectangles or another window: they are not used in milestone 1");
  link->put(X_PRESENT, nullptr, 0);
  return D3D_OK;
}
HRESULT Dev::GetBackBuffer(UINT i, D3DBACKBUFFER_TYPE, IDirect3DSurface8 **o) {
  if (i || !o || !backbuf) return D3DERR_INVALIDCALL;
  *o = backbuf; backbuf->AddRef(); return D3D_OK;
}
HRESULT Dev::BeginScene() { if (inScene) return D3DERR_INVALIDCALL; inScene = true; return D3D_OK; }
HRESULT Dev::EndScene() { if (!inScene) return D3DERR_INVALIDCALL; inScene = false; return D3D_OK; }
HRESULT Dev::Clear(DWORD n, const D3DRECT *r, DWORD flags, D3DCOLOR color, float z, DWORD stencil) {
  // "Clears the viewport, or a set of rectangles in the viewport ... Coordinates are clipped to the bounds of the
  // viewport rectangle", and it "fails if you specify the D3DCLEAR_ZBUFFER or D3DCLEAR_STENCIL flags when the render
  // target does not have an attached depth buffer", or STENCIL when the depth format has no stencil (DirectX 8.1,
  // IDirect3DDevice8::Clear)
  if (!(flags & (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL)) || (n && !r) || !curRT) return D3DERR_INVALIDCALL;
  if ((flags & (D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL)) && !curDS) return D3DERR_INVALIDCALL;
  if ((flags & D3DCLEAR_STENCIL) && curDS->fmt != D3DFMT_D24S8 && curDS->fmt != D3DFMT_D15S1 && curDS->fmt != D3DFMT_D24X4S4)
    return D3DERR_INVALIDCALL;
  LONG vx1 = (LONG)st.vp.X, vy1 = (LONG)st.vp.Y;
  LONG vx2 = (LONG)(st.vp.X + st.vp.Width), vy2 = (LONG)(st.vp.Y + st.vp.Height);
  if (vx2 > (LONG)curRT->w) vx2 = curRT->w; if (vy2 > (LONG)curRT->h) vy2 = curRT->h;
  std::vector<XRect> rs;
  if (!r) { if (vx2 > vx1 && vy2 > vy1) rs.push_back({vx1, vy1, vx2, vy2}); }
  else for (DWORD i = 0; i < n; i++) {
    XRect q = {r[i].x1 > vx1 ? r[i].x1 : vx1, r[i].y1 > vy1 ? r[i].y1 : vy1, r[i].x2 < vx2 ? r[i].x2 : vx2, r[i].y2 < vy2 ? r[i].y2 : vy2};
    if (q.x2 > q.x1 && q.y2 > q.y1) rs.push_back(q);
  }
  if (rs.empty()) return D3D_OK;
  // the whole target in one rectangle, and a depth surface (if cleared) of the same size: a view clear does it all
  bool dsSame = !(flags & (D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL)) || (curDS->w == curRT->w && curDS->h == curRT->h);
  if (rs.size() == 1 && rs[0].x1 == 0 && rs[0].y1 == 0 && rs[0].x2 == (LONG)curRT->w && rs[0].y2 == (LONG)curRT->h && dsSame) rs.clear();
  XClear c = {flags, color, z, stencil, curRT->w, curRT->h, (uint32_t)rs.size()};
  link->put(X_CLEAR, &c, sizeof c, rs.data(), (uint32_t)(rs.size() * sizeof(XRect)));
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
  s->id = new_id(); XTex t = {s->id, XT_RENDERTARGET, w, h, 1, 1, (uint32_t)f, D3DUSAGE_RENDERTARGET}; link->put(X_TEX, &t, sizeof t);
  *o = s; return D3D_OK;
}
HRESULT Dev::CreateDepthStencilSurface(UINT w, UINT h, D3DFORMAT f, D3DMULTISAMPLE_TYPE ms, IDirect3DSurface8 **o) {
  if (!o) return D3DERR_INVALIDCALL;
  FmtInfo fi = d8_fmt(f);
  if (ms != D3DMULTISAMPLE_NONE || !fi.depth) { d8_notimpl("CreateDepthStencilSurface of this format or multisampling"); return D3DERR_INVALIDCALL; }
  Surf *s = new Surf(this, SK_DS, w, h, f, D3DPOOL_DEFAULT, D3DUSAGE_DEPTHSTENCIL);
  s->id = new_id(); XTex t = {s->id, XT_DEPTH, w, h, 1, 1, (uint32_t)f, D3DUSAGE_DEPTHSTENCIL}; link->put(X_TEX, &t, sizeof t);
  *o = s; return D3D_OK;
}
HRESULT Dev::CreateImageSurface(UINT w, UINT h, D3DFORMAT f, IDirect3DSurface8 **o) {
  if (!o || d8_fmt(f).dxgi == DXGI_FORMAT_UNKNOWN) return D3DERR_INVALIDCALL;
  Surf *s = new Surf(this, SK_IMAGE, w, h, f, D3DPOOL_SYSTEMMEM, 0);
  s->cpu.assign((size_t)d8_pitch(f, w) * d8_rows(f, h), 0);
  *o = s; return D3D_OK;
}
// the texture a texture level belongs to
static TexCore *core_of(Surf *s) {
  if (s->kind != SK_TEXLEVEL || !s->container) return nullptr;
  IDirect3DBaseTexture8 *t = (IDirect3DBaseTexture8 *)s->container;
  return t->GetType() == D3DRTYPE_CUBETEXTURE ? &((CubeTex *)(IDirect3DCubeTexture8 *)t)->c : &((Tex *)(IDirect3DTexture8 *)t)->c;
}
HRESULT Dev::CopyRects(IDirect3DSurface8 *src, const RECT *rects, UINT n, IDirect3DSurface8 *dst, const POINT *pts) {
  // DirectX 8.1, IDirect3DDevice8::CopyRects: source and destination "must point to a different surface"; "The format
  // of the two surfaces must match"; "this method will fail unless all the source rectangles and their corresponding
  // destination rectangles are completely contained within the source and destination surfaces"; it "cannot be
  // applied to surfaces whose formats are classified as depth stencil formats"; NULL points copy each rectangle "to
  // the same offset"; NULL rectangles copy "the entire surface"
  Surf *s = (Surf *)src, *d = (Surf *)dst;
  if (!s || !d || s == d || s->fmt != d->fmt || d8_fmt(s->fmt).depth) return D3DERR_INVALIDCALL;
  RECT whole = {0, 0, (LONG)s->w, (LONG)s->h}; POINT zero = {0, 0};
  UINT cnt = rects && n ? n : 1;
  for (UINT i = 0; i < cnt; i++) {
    RECT r = rects && n ? rects[i] : whole; POINT p = !(rects && n) ? zero : pts ? pts[i] : POINT{r.left, r.top};
    if (r.left < 0 || r.top < 0 || r.right <= r.left || r.bottom <= r.top || r.right > (LONG)s->w || r.bottom > (LONG)s->h ||
        p.x < 0 || p.y < 0 || p.x + (r.right - r.left) > (LONG)d->w || p.y + (r.bottom - r.top) > (LONG)d->h) return D3DERR_INVALIDCALL;
  }
  for (UINT i = 0; i < cnt; i++) {
    RECT r = rects && n ? rects[i] : whole; POINT p = pts ? pts[i] : POINT{r.left, r.top};
    if (!rects || !n) p = zero;
    bool sg = s->kind != SK_IMAGE && s->id, dg = d->kind != SK_IMAGE && d->id;
    if (sg && dg) {                                           // GPU to GPU
      XCopy c = {s->xref(), d->xref(), r.left, r.top, r.right, r.bottom, p.x, p.y}; link->put(X_COPY, &c, sizeof c);
      if (d->kind == SK_TEXLEVEL && d->pool != D3DPOOL_DEFAULT)
        d8_once("copystale", "CopyRects from a GPU surface into a lockable texture level: its CPU copy is not updated (a later LockRect reads the old texels)");
    } else if (sg && d->kind == SK_IMAGE) {                   // GPU to system memory: a read back
      std::vector<uint8_t> out; XRef ref = s->xref();
      if (link->call(X_READBACK, &ref, sizeof ref, &out) < 0) return D3DERR_INVALIDCALL;
      FmtInfo fi = d8_fmt(s->fmt);
      UINT bw = fi.block ? 4 : 1, sw = s->kind == SK_TEXLEVEL ? s->w : s->w;
      UINT sp = d8_pitch(s->fmt, sw), dp = d8_pitch(d->fmt, d->w), rowb = d8_pitch(s->fmt, (UINT)(r.right - r.left)), rows = d8_rows(s->fmt, (UINT)(r.bottom - r.top));
      for (UINT y = 0; y < rows; y++) {
        size_t so = (size_t)(r.top / bw + y) * sp + d8_pitch(s->fmt, (UINT)r.left), dof = (size_t)(p.y / bw + y) * dp + d8_pitch(d->fmt, (UINT)p.x);
        if (so + rowb <= out.size() && dof + rowb <= d->cpu.size()) memcpy(&d->cpu[dof], &out[so], rowb);
      }
    } else if (s->kind == SK_IMAGE && (dg || d->kind == SK_IMAGE)) {   // system memory to the GPU, or to system memory
      FmtInfo fi = d8_fmt(s->fmt);
      UINT bw = fi.block ? 4 : 1;                               // block formats copy whole 4x4 blocks
      if ((r.left | r.top | p.x | p.y) % bw) { d8_once("copyalign", "CopyRects of a block format not on block edges (refused)"); return D3DERR_INVALIDCALL; }
      UINT sp = d8_pitch(s->fmt, s->w), rowb = d8_pitch(s->fmt, (UINT)(r.right - r.left)), rows = d8_rows(s->fmt, (UINT)(r.bottom - r.top));
      const uint8_t *src = s->cpu.data() + (size_t)(r.top / bw) * sp + (size_t)d8_pitch(s->fmt, (UINT)r.left);
      if (d->kind == SK_IMAGE) {
        UINT dp = d8_pitch(d->fmt, d->w);
        for (UINT y = 0; y < rows; y++) memcpy(&d->cpu[(size_t)(p.y / bw + y) * dp + d8_pitch(d->fmt, (UINT)p.x)], src + (size_t)y * sp, rowb);
      } else {
        UINT levels = 1;
        if (d->kind == SK_TEXLEVEL && d->container)
          levels = ((IDirect3DBaseTexture8 *)d->container)->GetType() == D3DRTYPE_CUBETEXTURE ? ((CubeTex *)(IDirect3DCubeTexture8 *)d->container)->c.levels : ((Tex *)(IDirect3DTexture8 *)d->container)->c.levels;
        std::vector<uint8_t> rowsdata((size_t)rowb * rows);
        for (UINT y = 0; y < rows; y++) memcpy(&rowsdata[(size_t)y * rowb], src + (size_t)y * sp, rowb);
        XUpTex u = {d->id, d->kind == SK_TEXLEVEL ? D3D11CalcSubresource(d->level, d->face, levels) : 0u, (uint32_t)p.x, (uint32_t)p.y,
                    (uint32_t)(r.right - r.left), (uint32_t)(r.bottom - r.top), rowb, rows};
        link->put(X_UPLOAD_TEX, &u, sizeof u, rowsdata.data(), (uint32_t)rowsdata.size());
        // a texture level's CPU copy is what LockRect reads and UnlockRect sends whole: it gets the rows too
        if (TexCore *tc = d->kind == SK_TEXLEVEL ? core_of(d) : nullptr) {
          std::vector<uint8_t> &lv = tc->cpu[d->face * tc->levels + d->level];
          UINT dp = d8_pitch(d->fmt, d->w);
          if (lv.size() < (size_t)dp * d8_rows(d->fmt, d->h)) lv.resize((size_t)dp * d8_rows(d->fmt, d->h), 0);
          for (UINT y = 0; y < rows; y++) memcpy(&lv[(size_t)(p.y / bw + y) * dp + d8_pitch(d->fmt, (UINT)p.x)], &rowsdata[(size_t)y * rowb], rowb);
        }
      }
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
  targets();
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
// user clip planes: kept and read back; drawing with D3DRS_CLIPPLANEENABLE set is logged as not implemented (d8_ff.cpp).
// The game's traces never set CLIPPLANEENABLE (data/d3d8_scope.json)
HRESULT Dev::SetClipPlane(DWORD i, const float *p) {
  if (i >= 6 || !p) return D3DERR_INVALIDCALL;
  memcpy(TARGET.clip[i], p, 16); if (rec) rec->clip[i] = true;
  return D3D_OK;
}
HRESULT Dev::GetClipPlane(DWORD i, float *p) { if (i >= 6 || !p) return D3DERR_INVALIDCALL; memcpy(p, st.clip[i], 16); return D3D_OK; }
// texture palettes (DirectX 8.1, IDirect3DDevice8::SetPaletteEntries and SetCurrentTexturePalette): 256 entries each;
// "A single logical palette is associated with the device, and is shared by all texture stages"; without
// D3DPTEXTURECAPS_ALPHAPALETTE "every entry in the palette must have alpha set to 1.0 or this method will fail".
// Kept and read back only: no palettized texture format is implemented, and the game creates none (its formats:
// data/d3d8_scope.json, resources). The current palette's value before any set is not in the reference: 0 here
HRESULT Dev::SetPaletteEntries(UINT n, const PALETTEENTRY *e) {
  if (!e) return D3DERR_INVALIDCALL;
  D3DCAPS8 c; GetDeviceCaps(&c);
  if (!(c.TextureCaps & D3DPTEXTURECAPS_ALPHAPALETTE)) for (int i = 0; i < 256; i++) if (e[i].peFlags != 0xFF) return D3DERR_INVALIDCALL;
  palettes[n].assign(e, e + 256);
  return D3D_OK;
}
HRESULT Dev::GetPaletteEntries(UINT n, PALETTEENTRY *e) {
  auto it = palettes.find(n);
  if (!e || it == palettes.end()) return D3DERR_INVALIDCALL;
  memcpy(e, it->second.data(), 256 * sizeof(PALETTEENTRY));
  return D3D_OK;
}
// not among the methods a state block records (DirectX 8.1, BeginStateBlock), so it takes effect while one records;
// a D3DSBT_ALL block holds "the current palette"
HRESULT Dev::SetCurrentTexturePalette(UINT n) { st.palette = n; return D3D_OK; }
HRESULT Dev::GetCurrentTexturePalette(UINT *n) { if (!n) return D3DERR_INVALIDCALL; *n = st.palette; return D3D_OK; }
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
  for (int i = 0; i < 6; i++) if (b.clip[i]) memcpy(to.clip[i], from.clip[i], 16);
  if (b.palette) to.palette = from.palette;
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
  // D3DSBT_ALL: "all current render states, clipplanes, texture stage states, the current material, all current
  // textures, all current lights and enabled light parameters, the current palette, the current pixel shader, all
  // current streams, the pixel shader constants, the current viewport, the current vertex shader, all current
  // transforms, the vertex shader constants" (DirectX 8.1, D3DSTATEBLOCKTYPE): the index buffer is not in the list
  memset(b->xf, 1, sizeof b->xf); b->vp = b->mat = b->vs = b->ps = b->palette = true; b->ib = false; memset(b->stream, 1, sizeof b->stream);
  memset(b->clip, 1, sizeof b->clip);
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

// ---- draws: executed through generated shaders (d8_ff.cpp) ----------------------------------------------------------------
static HRESULT counted(Dev *d, HRESULT hr) { d->draws++; if (hr != D3D_OK) d->drawsNotExecuted++; return hr; }
HRESULT Dev::DrawPrimitive(D3DPRIMITIVETYPE t, UINT start, UINT n) {
  return counted(this, ff_draw(0, t, n, start, 0, 0, 0, NULL, 0, NULL, D3DFMT_UNKNOWN));
}
HRESULT Dev::DrawIndexedPrimitive(D3DPRIMITIVETYPE t, UINT minIndex, UINT numVerts, UINT startIndex, UINT n) {
  return counted(this, ff_draw(1, t, n, 0, minIndex, numVerts, startIndex, NULL, 0, NULL, D3DFMT_UNKNOWN));
}
HRESULT Dev::DrawPrimitiveUP(D3DPRIMITIVETYPE t, UINT n, const void *v, UINT stride) {
  if (!v || !stride) return D3DERR_INVALIDCALL;
  return counted(this, ff_draw(2, t, n, 0, 0, 0, 0, v, stride, NULL, D3DFMT_UNKNOWN));
}
HRESULT Dev::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE t, UINT minIndex, UINT numVerts, UINT n, const void *i, D3DFORMAT f, const void *v, UINT stride) {
  if (!v || !i || !stride) return D3DERR_INVALIDCALL;
  return counted(this, ff_draw(3, t, n, 0, minIndex, numVerts, 0, v, stride, i, f));
}
