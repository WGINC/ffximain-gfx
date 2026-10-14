// d8_res.cpp -- d3d8on11: textures, surfaces, vertex and index buffers.
//
// Lockable data lives in a CPU copy the game writes through Lock; Unlock sends what changed to the executor
// (d8_exec.cpp, through the link), which alone uses Direct3D 11. GPU images that the game reads back (a lockable
// render target, the back buffer) are asked for at LockRect (X_READBACK): that read waits for the executor and the
// GPU, as the game's occlusion readback does under any D3D8 implementation.
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
    id = d->new_id();
    XTex t = {id, XT_TEXTURE, w, h, levels, faces, (uint32_t)f, usage};
    d->link->put(X_TEX, &t, sizeof t);
  }
  cpu.resize(faces * levels); surfs.assign(faces * levels, nullptr);
  return D3D_OK;
}
void TexCore::destroy() {
  for (Surf *s : surfs) if (s) { s->container = nullptr; s->Release(); }
  dev->destroy_obj(id); id = 0;
}
HRESULT TexCore::lock(UINT face, UINT l, D3DLOCKED_RECT *lr, const RECT *r, DWORD flags) {
  if (l >= levels || face >= faces || !lr) return D3DERR_INVALIDCALL;
  // "Textures created in D3DPOOL_DEFAULT are not lockable" (DirectX 8.1, IDirect3DTexture8::LockRect), except dynamic
  // ones ("D3DLOCK_DISCARD ... is a valid flag only when using dynamic textures")
  if (pool == D3DPOOL_DEFAULT && !(usage & D3DUSAGE_DYNAMIC)) return D3DERR_INVALIDCALL;
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
  if (readonly.size() < cpu.size()) readonly.resize(cpu.size(), 0);
  readonly[sub] = (flags & D3DLOCK_READONLY) != 0;
  return D3D_OK;
}
HRESULT TexCore::unlock(UINT face, UINT l) {
  // D3DLOCK_READONLY: "The application will not write" (DirectX 8.1, LockRect): nothing to send
  { UINT sub = face * levels + l; if (sub < readonly.size() && readonly[sub]) { readonly[sub] = 0; return D3D_OK; } }
  if (l >= levels || face >= faces) return D3DERR_INVALIDCALL;
  if (id) {                                                  // the whole level, as the CPU copy holds it
    std::vector<uint8_t> &b = cpu[face * levels + l];
    UINT pitch = d8_pitch(fmt, lw(l)), rows = d8_rows(fmt, lh(l));
    if (b.size() < (size_t)pitch * rows) b.resize((size_t)pitch * rows, 0);
    XUpTex u = {id, D3D11CalcSubresource(l, face, levels), 0, 0, 0, 0, pitch, rows};
    dev->link->put(X_UPLOAD_TEX, &u, sizeof u, b.data(), pitch * rows);
  }
  return D3D_OK;
}
Surf *TexCore::surface(IUnknown *owner, UINT face, UINT l) {
  UINT sub = face * levels + l;
  if (!surfs[sub]) {
    Surf *s = new Surf(dev, SK_TEXLEVEL, lw(l), lh(l), fmt, pool, usage);
    s->container = owner; s->level = l; s->face = face; s->id = id;
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
ULONG Tex::Release() { LONG n = InterlockedDecrement(&ref); if (!n) { c.destroy(); delete this; } return n; }
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
ULONG CubeTex::Release() { LONG n = InterlockedDecrement(&ref); if (!n) { c.destroy(); delete this; } return n; }
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
Surf::~Surf() { if (kind == SK_RT || kind == SK_DS) dev->destroy_obj(id); }   // (the back buffer is the swap chain's)
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
// the GPU image read back into the CPU copy: the executor runs what came before, copies through a staging texture, answers
static bool read_back(Surf *s) {
  if (!s->id) return false;
  std::vector<uint8_t> out; XRef r = s->xref();
  if (s->dev->link->call(X_READBACK, &r, sizeof r, &out) < 0) return false;
  UINT pitch = d8_pitch(s->fmt, s->w), rows = d8_rows(s->fmt, s->h);
  s->cpu.assign((size_t)pitch * rows, 0);
  memcpy(s->cpu.data(), out.data(), out.size() < s->cpu.size() ? out.size() : s->cpu.size());
  return true;
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
  } else if (cpu.empty()) cpu.assign((size_t)d8_pitch(fmt, w) * d8_rows(fmt, h), 0);
  UINT pitch = d8_pitch(fmt, w); size_t off = 0;
  if (r) { FmtInfo fi = d8_fmt(fmt); off = fi.block ? (size_t)(r->top / 4) * pitch + (r->left / 4) * fi.block : (size_t)r->top * pitch + r->left * (fi.bpp / 8); }
  lr->pBits = cpu.data() + off; lr->Pitch = pitch; locked = true;
  lockFlags = flags; lockRect = r ? *r : RECT{0, 0, (LONG)w, (LONG)h};
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
  // a render target or the back buffer locked for writing: what was written goes back to it (its rows of the lock)
  if ((kind == SK_RT || kind == SK_BACKBUFFER) && id && !(lockFlags & D3DLOCK_READONLY)) {
    UINT pitch = d8_pitch(fmt, w), rb = d8_pitch(fmt, (UINT)(lockRect.right - lockRect.left)), rows = (UINT)(lockRect.bottom - lockRect.top);
    std::vector<uint8_t> out((size_t)rb * rows);
    for (UINT y = 0; y < rows; y++) memcpy(&out[(size_t)y * rb], &cpu[(size_t)(lockRect.top + y) * pitch + d8_pitch(fmt, (UINT)lockRect.left)], rb);
    XUpTex u = {id, 0, (uint32_t)lockRect.left, (uint32_t)lockRect.top, (uint32_t)(lockRect.right - lockRect.left), rows, rb, rows};
    dev->link->put(X_UPLOAD_TEX, &u, sizeof u, out.data(), (uint32_t)out.size());
  }
  return D3D_OK;
}

// ---- buffers ------------------------------------------------------------------------------------------------------------
HRESULT BufCore::init(Dev *d, UINT len_, DWORD u, DWORD fvf_, D3DFORMAT f, D3DPOOL p, bool idx) {
  dev = d; len = len_; usage = u; fvf = fvf_; fmt = f; pool = p; index = idx;
  if (!len) return D3DERR_INVALIDCALL;
  cpu.assign(len, 0);
  if (pool != D3DPOOL_SYSTEMMEM) { id = d->new_id(); XBuf b = {id, len, idx ? 1u : 0u}; d->link->put(X_BUF, &b, sizeof b); }
  return D3D_OK;
}
void BufCore::destroy() { dev->destroy_obj(id); id = 0; }
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
  if (id && hi > lo) { XUpBuf u = {id, lo, hi - lo}; dev->link->put(X_UPLOAD_BUF, &u, sizeof u, cpu.data() + lo, hi - lo); }
  return D3D_OK;
}
HRESULT VB::QueryInterface(REFIID r, void **o) { if (is_iid(r, IID_IDirect3DVertexBuffer8)) { *o = this; AddRef(); return S_OK; } *o = NULL; return E_NOINTERFACE; }
ULONG VB::AddRef() { return InterlockedIncrement(&ref); }
ULONG VB::Release() { LONG n = InterlockedDecrement(&ref); if (!n) { c.destroy(); delete this; } return n; }
HRESULT VB::GetDevice(IDirect3DDevice8 **d) { *d = c.dev; c.dev->AddRef(); return D3D_OK; }
HRESULT VB::GetDesc(D3DVERTEXBUFFER_DESC *d) {
  if (!d) return D3DERR_INVALIDCALL;
  d->Format = D3DFMT_VERTEXDATA; d->Type = D3DRTYPE_VERTEXBUFFER; d->Usage = c.usage; d->Pool = c.pool; d->Size = c.len; d->FVF = c.fvf;
  return D3D_OK;
}
HRESULT IB::QueryInterface(REFIID r, void **o) { if (is_iid(r, IID_IDirect3DIndexBuffer8)) { *o = this; AddRef(); return S_OK; } *o = NULL; return E_NOINTERFACE; }
ULONG IB::AddRef() { return InterlockedIncrement(&ref); }
ULONG IB::Release() { LONG n = InterlockedDecrement(&ref); if (!n) { c.destroy(); delete this; } return n; }
HRESULT IB::GetDevice(IDirect3DDevice8 **d) { *d = c.dev; c.dev->AddRef(); return D3D_OK; }
HRESULT IB::GetDesc(D3DINDEXBUFFER_DESC *d) {
  if (!d) return D3DERR_INVALIDCALL;
  d->Format = c.fmt; d->Type = D3DRTYPE_INDEXBUFFER; d->Usage = c.usage; d->Pool = c.pool; d->Size = c.len;
  return D3D_OK;
}
