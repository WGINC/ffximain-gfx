// d8_res.cpp -- d3d8on11: textures, surfaces, vertex and index buffers.
//
// Lockable data lives in a CPU copy the game writes through Lock; Unlock records an upload of what changed (Cmd), so
// the executor is the only code that touches the D3D11 context. GPU images that the game reads back (a lockable
// render target, the back buffer) are copied out through a staging texture at LockRect: that read waits for the GPU,
// as the game's occlusion readback does under any D3D8 implementation.
#include "d8.h"
#include <string>

static const GUID *iid_res_base[] = {&IID_IUnknown, &IID_IDirect3DResource8, &IID_IDirect3DBaseTexture8};

// ---- textures -----------------------------------------------------------------------------------------------------------
HRESULT TexCore::init(Dev *d, UINT w_, UINT h_, UINT lv, UINT faces_, DWORD u, D3DFORMAT f, D3DPOOL p) {
  dev = d; w = w_; h = h_; faces = faces_; usage = u; fmt = f; pool = p;
  FmtInfo fi = d8_fmt(f);
  if (fi.dxgi == DXGI_FORMAT_UNKNOWN || fi.depth) {
    d8_once(("texfmt" + std::to_string(f)).c_str(), "CreateTexture: format %lu not supported (failed)", (unsigned long)f);
    return D3DERR_INVALIDCALL;
  }
  UINT full = 1; for (UINT m = w > h ? w : h; m > 1; m >>= 1) full++;
  levels = lv == 0 || lv > full ? full : lv;
  if (fi.block && ((w & 3) || (h & 3)))
    d8_once("bcsize", "texture %ux%u in a block format: D3D11 wants the top level in whole 4x4 blocks; created as is, "
            "behaviour to be checked (first case logged)", w, h);
  if (pool == D3DPOOL_SYSTEMMEM || pool == D3DPOOL_SCRATCH) {
    d8_once("syspool", "a texture in pool %d: kept on the CPU only (no GPU copy)", (int)pool);
  } else {
    D3D11_TEXTURE2D_DESC td = {}; td.Width = w; td.Height = h; td.MipLevels = levels; td.ArraySize = faces;
    td.Format = fi.dxgi; td.SampleDesc.Count = 1; td.Usage = D3D11_USAGE_DEFAULT;
    td.BindFlags = D3D11_BIND_SHADER_RESOURCE | ((usage & D3DUSAGE_RENDERTARGET) ? D3D11_BIND_RENDER_TARGET : 0);
    td.MiscFlags = faces == 6 ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0;
    HRESULT hr = d->dev->CreateTexture2D(&td, NULL, &gpu);
    if (FAILED(hr)) { d8_log("CreateTexture2D %ux%u fmt %lu levels %u: 0x%08lX", w, h, (unsigned long)f, levels, (unsigned long)hr); return D3DERR_OUTOFVIDEOMEMORY; }
    d->dev->CreateShaderResourceView(gpu, NULL, &srv);
  }
  cpu.resize(faces * levels); surfs.assign(faces * levels, nullptr);
  return D3D_OK;
}
void TexCore::destroy() {
  for (Surf *s : surfs) if (s) { s->container = nullptr; s->Release(); }
  if (srv) srv->Release(); if (gpu) gpu->Release();
  srv = nullptr; gpu = nullptr;
}
HRESULT TexCore::lock(UINT face, UINT l, D3DLOCKED_RECT *lr, const RECT *r, DWORD) {
  if (l >= levels || face >= faces || !lr) return D3DERR_INVALIDCALL;
  if (usage & D3DUSAGE_RENDERTARGET) { d8_notimpl("LockRect of a render-target texture"); return D3DERR_INVALIDCALL; }
  UINT sub = face * levels + l, pitch = d8_pitch(fmt, lw(l));
  std::vector<uint8_t> &b = cpu[sub];
  if (b.empty()) b.assign((size_t)pitch * d8_rows(fmt, lh(l)), 0);
  size_t off = 0;
  if (r) {
    FmtInfo fi = d8_fmt(fmt);
    off = fi.block ? (size_t)(r->top / 4) * pitch + (r->left / 4) * fi.block : (size_t)r->top * pitch + r->left * (fi.bpp / 8);
  }
  lr->pBits = b.data() + off; lr->Pitch = pitch;
  return D3D_OK;
}
HRESULT TexCore::unlock(UINT face, UINT l) {
  if (l >= levels || face >= faces) return D3DERR_INVALIDCALL;
  if (gpu) { Cmd c; c.op = C_UPLOAD_TEX; c.uptex.t = this; c.uptex.sub = face * levels + l; dev->push(c); }
  return D3D_OK;
}
Surf *TexCore::surface(IUnknown *owner, UINT face, UINT l) {
  UINT sub = face * levels + l;
  if (!surfs[sub]) {
    Surf *s = new Surf(dev, SK_TEXLEVEL, lw(l), lh(l), fmt, pool, usage);
    s->container = owner; s->level = l; s->face = face; s->tex = gpu; if (gpu) gpu->AddRef();
    surfs[sub] = s;
  }
  surfs[sub]->AddRef();
  return surfs[sub];
}

static bool is_iid(REFIID r, const GUID &own) {
  if (IsEqualGUID(r, own)) return true;
  for (const GUID *g : iid_res_base) if (IsEqualGUID(r, *g)) return true;
  return false;
}
HRESULT Tex::QueryInterface(REFIID r, void **o) { if (is_iid(r, IID_IDirect3DTexture8)) { *o = this; AddRef(); return S_OK; } *o = NULL; return E_NOINTERFACE; }
ULONG Tex::AddRef() { return InterlockedIncrement(&ref); }
ULONG Tex::Release() { LONG n = InterlockedDecrement(&ref); if (!n) { c.dev->flush(); c.destroy(); delete this; } return n; }
HRESULT Tex::GetDevice(IDirect3DDevice8 **d) { *d = c.dev; c.dev->AddRef(); return D3D_OK; }
HRESULT Tex::GetLevelDesc(UINT l, D3DSURFACE_DESC *d) {
  if (l >= c.levels || !d) return D3DERR_INVALIDCALL;
  d->Format = c.fmt; d->Type = D3DRTYPE_SURFACE; d->Usage = c.usage; d->Pool = c.pool; d->MultiSampleType = D3DMULTISAMPLE_NONE;
  d->Width = c.lw(l); d->Height = c.lh(l); d->Size = d8_pitch(c.fmt, d->Width) * d8_rows(c.fmt, d->Height);
  return D3D_OK;
}
HRESULT Tex::GetSurfaceLevel(UINT l, IDirect3DSurface8 **s) { if (l >= c.levels || !s) return D3DERR_INVALIDCALL; *s = c.surface(this, 0, l); return D3D_OK; }
HRESULT Tex::LockRect(UINT l, D3DLOCKED_RECT *lr, const RECT *r, DWORD f) { return c.lock(0, l, lr, r, f); }
HRESULT Tex::UnlockRect(UINT l) { return c.unlock(0, l); }

HRESULT CubeTex::QueryInterface(REFIID r, void **o) { if (is_iid(r, IID_IDirect3DCubeTexture8)) { *o = this; AddRef(); return S_OK; } *o = NULL; return E_NOINTERFACE; }
ULONG CubeTex::AddRef() { return InterlockedIncrement(&ref); }
ULONG CubeTex::Release() { LONG n = InterlockedDecrement(&ref); if (!n) { c.dev->flush(); c.destroy(); delete this; } return n; }
HRESULT CubeTex::GetDevice(IDirect3DDevice8 **d) { *d = c.dev; c.dev->AddRef(); return D3D_OK; }
HRESULT CubeTex::GetLevelDesc(UINT l, D3DSURFACE_DESC *d) {
  if (l >= c.levels || !d) return D3DERR_INVALIDCALL;
  d->Format = c.fmt; d->Type = D3DRTYPE_SURFACE; d->Usage = c.usage; d->Pool = c.pool; d->MultiSampleType = D3DMULTISAMPLE_NONE;
  d->Width = c.lw(l); d->Height = c.lh(l); d->Size = d8_pitch(c.fmt, d->Width) * d8_rows(c.fmt, d->Height);
  return D3D_OK;
}
HRESULT CubeTex::GetCubeMapSurface(D3DCUBEMAP_FACES f, UINT l, IDirect3DSurface8 **s) {
  if ((UINT)f >= 6 || l >= c.levels || !s) return D3DERR_INVALIDCALL;
  *s = c.surface(this, (UINT)f, l); return D3D_OK;
}
HRESULT CubeTex::LockRect(D3DCUBEMAP_FACES f, UINT l, D3DLOCKED_RECT *lr, const RECT *r, DWORD fl) { return c.lock((UINT)f, l, lr, r, fl); }
HRESULT CubeTex::UnlockRect(D3DCUBEMAP_FACES f, UINT l) { return c.unlock((UINT)f, l); }

// ---- surfaces -----------------------------------------------------------------------------------------------------------
Surf::Surf(Dev *d, SurfKind k, UINT w_, UINT h_, D3DFORMAT f, D3DPOOL p, DWORD u)
    : dev(d), kind(k), w(w_), h(h_), fmt(f), pool(p), usage(u) {}
Surf::~Surf() { if (rtv) rtv->Release(); if (dsv) dsv->Release(); if (tex) tex->Release(); }
HRESULT Surf::QueryInterface(REFIID r, void **o) {
  if (IsEqualGUID(r, IID_IUnknown) || IsEqualGUID(r, IID_IDirect3DSurface8)) { *o = this; AddRef(); return S_OK; }
  *o = NULL; return E_NOINTERFACE;
}
// a texture level's surface lives as long as its texture: its references count on the texture (D3D8 behaviour)
ULONG Surf::AddRef() { if (container) container->AddRef(); return InterlockedIncrement(&ref); }
ULONG Surf::Release() {
  IUnknown *c = container;
  LONG n = InterlockedDecrement(&ref);
  if (c) { c->Release(); return n; }
  if (!n) delete this;
  return n;
}
HRESULT Surf::GetDevice(IDirect3DDevice8 **d) { *d = dev; dev->AddRef(); return D3D_OK; }
HRESULT Surf::GetContainer(REFIID r, void **o) {
  if (container) return container->QueryInterface(r, o);
  return dev->QueryInterface(r, o);
}
HRESULT Surf::GetDesc(D3DSURFACE_DESC *d) {
  if (!d) return D3DERR_INVALIDCALL;
  d->Format = fmt; d->Type = D3DRTYPE_SURFACE; d->Usage = usage; d->Pool = pool; d->MultiSampleType = D3DMULTISAMPLE_NONE;
  d->Width = w; d->Height = h; d->Size = d8_pitch(fmt, w) * d8_rows(fmt, h);
  return D3D_OK;
}
ID3D11RenderTargetView *Surf::get_rtv() {
  if (rtv || !tex) return rtv;
  D3D11_RENDER_TARGET_VIEW_DESC rd = {}; rd.Format = d8_fmt(fmt).dxgi;
  if (kind == SK_TEXLEVEL && face == 0 && !(container && ((IDirect3DBaseTexture8 *)container)->GetType() == D3DRTYPE_CUBETEXTURE)) {
    rd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; rd.Texture2D.MipSlice = level;
  } else if (kind == SK_TEXLEVEL) {
    rd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY; rd.Texture2DArray.MipSlice = level;
    rd.Texture2DArray.FirstArraySlice = face; rd.Texture2DArray.ArraySize = 1;
  } else { rd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; }
  if (kind == SK_BACKBUFFER) rd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  dev->dev->CreateRenderTargetView(tex, &rd, &rtv);
  return rtv;
}
// The GPU image read back into the CPU copy: flushes what is recorded, copies through a staging texture, waits.
static bool read_back(Surf *s) {
  if (!s->tex) return false;
  s->dev->flush();
  D3D11_TEXTURE2D_DESC td; s->tex->GetDesc(&td);
  UINT sub = s->kind == SK_TEXLEVEL ? D3D11CalcSubresource(s->level, s->face, td.MipLevels) : 0;
  D3D11_TEXTURE2D_DESC sd = {}; sd.Width = s->w; sd.Height = s->h; sd.MipLevels = 1; sd.ArraySize = 1; sd.Format = td.Format;
  sd.SampleDesc.Count = 1; sd.Usage = D3D11_USAGE_STAGING; sd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
  ID3D11Texture2D *st = NULL;
  if (FAILED(s->dev->dev->CreateTexture2D(&sd, NULL, &st))) return false;
  s->dev->ctx->CopySubresourceRegion(st, 0, 0, 0, 0, s->tex, sub, NULL);
  D3D11_MAPPED_SUBRESOURCE m;
  bool ok = SUCCEEDED(s->dev->ctx->Map(st, 0, D3D11_MAP_READ, 0, &m));
  if (ok) {
    UINT pitch = d8_pitch(s->fmt, s->w), rows = d8_rows(s->fmt, s->h);
    s->cpu.resize((size_t)pitch * rows);
    for (UINT y = 0; y < rows; y++) memcpy(&s->cpu[(size_t)y * pitch], (uint8_t *)m.pData + (size_t)y * m.RowPitch, pitch);
    s->dev->ctx->Unmap(st, 0);
  }
  st->Release();
  return ok;
}
HRESULT Surf::LockRect(D3DLOCKED_RECT *lr, const RECT *r, DWORD flags) {
  if (!lr || locked) return D3DERR_INVALIDCALL;
  if (kind == SK_TEXLEVEL) {
    HRESULT hr;
    if (container && ((IDirect3DBaseTexture8 *)container)->GetType() == D3DRTYPE_CUBETEXTURE)
      hr = ((CubeTex *)(IDirect3DCubeTexture8 *)container)->c.lock(face, level, lr, r, flags);
    else hr = ((Tex *)(IDirect3DTexture8 *)container)->c.lock(0, level, lr, r, flags);
    if (SUCCEEDED(hr)) locked = true;
    return hr;
  }
  if (kind == SK_DS) { d8_notimpl("LockRect of a depth-stencil surface"); return D3DERR_INVALIDCALL; }
  if ((kind == SK_RT && !lockable)) { d8_notimpl("LockRect of a render target made not lockable"); return D3DERR_INVALIDCALL; }
  if (kind == SK_RT || kind == SK_BACKBUFFER) {
    if (!read_back(this)) return D3DERR_INVALIDCALL;
    if (!(flags & D3DLOCK_READONLY)) d8_once("rtwrite", "a render target locked for writing: what is written is not sent back yet");
  } else if (cpu.empty()) cpu.assign((size_t)d8_pitch(fmt, w) * d8_rows(fmt, h), 0);
  UINT pitch = d8_pitch(fmt, w); size_t off = 0;
  if (r) { FmtInfo fi = d8_fmt(fmt); off = fi.block ? (size_t)(r->top / 4) * pitch + (r->left / 4) * fi.block : (size_t)r->top * pitch + r->left * (fi.bpp / 8); }
  lr->pBits = cpu.data() + off; lr->Pitch = pitch; locked = true;
  return D3D_OK;
}
HRESULT Surf::UnlockRect() {
  if (!locked) return D3DERR_INVALIDCALL;
  locked = false;
  if (kind == SK_TEXLEVEL) {
    if (container && ((IDirect3DBaseTexture8 *)container)->GetType() == D3DRTYPE_CUBETEXTURE)
      return ((CubeTex *)(IDirect3DCubeTexture8 *)container)->c.unlock(face, level);
    return ((Tex *)(IDirect3DTexture8 *)container)->c.unlock(0, level);
  }
  return D3D_OK;
}

// ---- buffers ------------------------------------------------------------------------------------------------------------
HRESULT BufCore::init(Dev *d, UINT len_, DWORD u, DWORD fvf_, D3DFORMAT f, D3DPOOL p, bool idx) {
  dev = d; len = len_; usage = u; fvf = fvf_; fmt = f; pool = p; index = idx;
  if (!len) return D3DERR_INVALIDCALL;
  cpu.assign(len, 0);
  if (pool != D3DPOOL_SYSTEMMEM) {
    D3D11_BUFFER_DESC bd = {}; bd.ByteWidth = (len + 15) & ~15u; bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = idx ? D3D11_BIND_INDEX_BUFFER : D3D11_BIND_VERTEX_BUFFER;
    if (FAILED(d->dev->CreateBuffer(&bd, NULL, &gpu))) return D3DERR_OUTOFVIDEOMEMORY;
  }
  return D3D_OK;
}
void BufCore::destroy() { if (gpu) gpu->Release(); gpu = nullptr; }
HRESULT BufCore::lock(UINT off, UINT size, BYTE **p, DWORD) {
  if (!p || off > len) return D3DERR_INVALIDCALL;
  if (!size || off + size > len) size = len - off;               // D3D8: size 0 locks the whole buffer
  if (!locks++) { lo = off; hi = off + size; } else { if (off < lo) lo = off; if (off + size > hi) hi = off + size; }
  *p = cpu.data() + off;
  return D3D_OK;
}
HRESULT BufCore::unlock() {
  if (!locks) return D3DERR_INVALIDCALL;
  if (--locks) return D3D_OK;
  if (gpu && hi > lo) { Cmd c; c.op = C_UPLOAD_BUF; c.upbuf.b = this; c.upbuf.lo = lo; c.upbuf.hi = hi; dev->push(c); }
  return D3D_OK;
}
HRESULT VB::QueryInterface(REFIID r, void **o) { if (is_iid(r, IID_IDirect3DVertexBuffer8)) { *o = this; AddRef(); return S_OK; } *o = NULL; return E_NOINTERFACE; }
ULONG VB::AddRef() { return InterlockedIncrement(&ref); }
ULONG VB::Release() { LONG n = InterlockedDecrement(&ref); if (!n) { c.dev->flush(); c.destroy(); delete this; } return n; }
HRESULT VB::GetDevice(IDirect3DDevice8 **d) { *d = c.dev; c.dev->AddRef(); return D3D_OK; }
HRESULT VB::GetDesc(D3DVERTEXBUFFER_DESC *d) {
  if (!d) return D3DERR_INVALIDCALL;
  d->Format = D3DFMT_VERTEXDATA; d->Type = D3DRTYPE_VERTEXBUFFER; d->Usage = c.usage; d->Pool = c.pool; d->Size = c.len; d->FVF = c.fvf;
  return D3D_OK;
}
HRESULT IB::QueryInterface(REFIID r, void **o) { if (is_iid(r, IID_IDirect3DIndexBuffer8)) { *o = this; AddRef(); return S_OK; } *o = NULL; return E_NOINTERFACE; }
ULONG IB::AddRef() { return InterlockedIncrement(&ref); }
ULONG IB::Release() { LONG n = InterlockedDecrement(&ref); if (!n) { c.dev->flush(); c.destroy(); delete this; } return n; }
HRESULT IB::GetDevice(IDirect3DDevice8 **d) { *d = c.dev; c.dev->AddRef(); return D3D_OK; }
HRESULT IB::GetDesc(D3DINDEXBUFFER_DESC *d) {
  if (!d) return D3DERR_INVALIDCALL;
  d->Format = c.fmt; d->Type = D3DRTYPE_INDEXBUFFER; d->Usage = c.usage; d->Pool = c.pool; d->Size = c.len;
  return D3D_OK;
}
