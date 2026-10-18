/* replay.c -- plays a frame capture (kit/d3dprof/fcap.c, format fcap_fmt.h) into whichever d3d8.dll Windows loads
   next to it (Wine's, ours from kit/d3d8on11, dgVoodoo), and saves the picture it draws.

     fcap_replay.exe CAPTURE.fcap OUT_PREFIX
       OUT_PREFIX_replay.pic   the back buffer after the frame's calls (the kit's picture format, "XPIC")
       OUT_PREFIX_capture.pic  the picture the capture took in the game (what the game's own d3d8.dll drew)
       stdout                  one line per note of the capture and per call that failed here
     fcap_replay.exe CAPTURE.fcap OUT_PREFIX N
       as above, then the frame's calls (and the buffer updates between them) N more times with a Present after each,
       timed with QueryPerformanceCounter: a benchmark of the device the calls go to. The figures are of this machine
       and this device (under Wine, of Wine and its renderer), not of the game's machine

   Objects are created as the capture describes them; every call goes through the device's vtable with the
   argument words as recorded (so a float such as Clear's Z keeps its bits), object pointers and vertex shader handles
   replaced by this run's, data pointers by the capture's data. */
#define COBJMACROS
#include <windows.h>
#include <d3d8.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../d3dprof/fcap_fmt.h"

static IDirect3DDevice8 *d;
typedef struct { uint32_t id; void *obj; } Map;
static Map *g_map; static uint32_t g_nmap = 1 << 16;
static void map_put(uint32_t id, void *obj) {
  uint32_t h = (id >> 4) & (g_nmap - 1);
  while (g_map[h].id && g_map[h].id != id) h = (h + 1) & (g_nmap - 1);
  g_map[h].id = id; g_map[h].obj = obj;
}
static void *map_get(uint32_t id) {
  if (!id) return NULL;
  uint32_t h = (id >> 4) & (g_nmap - 1);
  while (g_map[h].id) { if (g_map[h].id == id) return g_map[h].obj; h = (h + 1) & (g_nmap - 1); }
  return NULL;
}
static uint32_t g_vsmap[4096][2]; static int g_nvs;
static uint32_t vs_get(uint32_t h) {
  if (!(h & 1)) return h;
  for (int i = 0; i < g_nvs; i++) if (g_vsmap[i][0] == h) return g_vsmap[i][1];
  return h;
}
static int g_fail;
static void fail(const char *what, HRESULT hr) { if (g_fail++ < 200) printf("replay: %s: 0x%08lX\n", what, (unsigned long)hr); }

static int fmt_rows(uint32_t fmt, uint32_t w, uint32_t h, uint32_t *rowbytes, uint32_t *rows) {
  uint32_t bpp = 0, blk = 0;
  switch (fmt) {
  case 21: case 22: bpp = 4; break; case 20: bpp = 3; break;
  case 23: case 24: case 25: case 26: case 51: bpp = 2; break; case 28: case 50: case 41: bpp = 1; break;
  case 0x31545844: blk = 8; break; case 0x32545844: case 0x33545844: case 0x34545844: case 0x35545844: blk = 16; break;
  default: return 0;
  }
  if (blk) { *rowbytes = ((w + 3) / 4) * blk; *rows = (h + 3) / 4; } else { *rowbytes = w * bpp; *rows = h; }
  return 1;
}
static uint32_t pad4(uint32_t n) { return (n + 3) & ~3u; }
/* rows as the capture wrote them (each padded to 4 bytes) into a locked rectangle */
static void put_rows(const uint8_t *src, uint8_t *dst, int pitch, uint32_t rowbytes, uint32_t rows) {
  for (uint32_t y = 0; y < rows; y++) memcpy(dst + (int)y * pitch, src + y * pad4(rowbytes), rowbytes);
}
/* a surface's pixels through an image surface (render targets cannot be locked) */
static void fill_surface(IDirect3DSurface8 *s, uint32_t fmt, uint32_t w, uint32_t h, const uint8_t *data, int lockable) {
  uint32_t rb, rows; if (!fmt_rows(fmt, w, h, &rb, &rows)) return;
  D3DLOCKED_RECT lr;
  if (lockable && SUCCEEDED(IDirect3DSurface8_LockRect(s, &lr, NULL, 0))) { put_rows(data, lr.pBits, lr.Pitch, rb, rows); IDirect3DSurface8_UnlockRect(s); return; }
  IDirect3DSurface8 *img = NULL;
  HRESULT hr = IDirect3DDevice8_CreateImageSurface(d, w, h, (D3DFORMAT)fmt, &img);
  if (FAILED(hr)) { fail("CreateImageSurface for a render target's contents", hr); return; }
  if (SUCCEEDED(IDirect3DSurface8_LockRect(img, &lr, NULL, 0))) { put_rows(data, lr.pBits, lr.Pitch, rb, rows); IDirect3DSurface8_UnlockRect(img); }
  if (FAILED(hr = IDirect3DDevice8_CopyRects(d, img, NULL, 0, s, NULL))) fail("CopyRects into a render target", hr);
  IDirect3DSurface8_Release(img);
}

typedef HRESULT (__stdcall *G)(void *, ...);
static HRESULT call(int slot, const uint32_t *a, int n) {
  void **vt = *(void ***)d;
  switch (n) {
  case 0: return ((HRESULT (__stdcall *)(void *))vt[slot])(d);
  case 1: return ((HRESULT (__stdcall *)(void *, uint32_t))vt[slot])(d, a[0]);
  case 2: return ((HRESULT (__stdcall *)(void *, uint32_t, uint32_t))vt[slot])(d, a[0], a[1]);
  case 3: return ((HRESULT (__stdcall *)(void *, uint32_t, uint32_t, uint32_t))vt[slot])(d, a[0], a[1], a[2]);
  case 4: return ((HRESULT (__stdcall *)(void *, uint32_t, uint32_t, uint32_t, uint32_t))vt[slot])(d, a[0], a[1], a[2], a[3]);
  case 5: return ((HRESULT (__stdcall *)(void *, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t))vt[slot])(d, a[0], a[1], a[2], a[3], a[4]);
  case 6: return ((HRESULT (__stdcall *)(void *, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t))vt[slot])(d, a[0], a[1], a[2], a[3], a[4], a[5]);
  case 7: return ((HRESULT (__stdcall *)(void *, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t))vt[slot])(d, a[0], a[1], a[2], a[3], a[4], a[5], a[6]);
  case 8: return ((HRESULT (__stdcall *)(void *, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t))vt[slot])(d, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]);
  }
  return E_FAIL;
}
static const char *slot_name(int s) {
  static char b[16];
  switch (s) {
  case FS_COPYRECTS: return "CopyRects"; case FS_SETRT: return "SetRenderTarget"; case FS_CLEAR: return "Clear";
  case FS_SETTRANSFORM: return "SetTransform"; case FS_SETVIEWPORT: return "SetViewport"; case FS_SETRS: return "SetRenderState";
  case FS_SETTEXTURE: return "SetTexture"; case FS_SETTSS: return "SetTextureStageState"; case FS_DP: return "DrawPrimitive";
  case FS_DIP: return "DrawIndexedPrimitive"; case FS_DPUP: return "DrawPrimitiveUP"; case FS_DIPUP: return "DrawIndexedPrimitiveUP";
  case FS_SETVS: return "SetVertexShader"; case FS_SSS: return "SetStreamSource"; case FS_SETINDICES: return "SetIndices";
  case FS_SETLIGHT: return "SetLight"; case FS_LIGHTENABLE: return "LightEnable"; case FS_SETMATERIAL: return "SetMaterial";
  }
  snprintf(b, sizeof b, "slot %d", s); return b;
}

static void write_pic(const char *path, uint32_t fmt, uint32_t w, uint32_t h, uint32_t bpp, const uint8_t *rows, uint32_t rowpad) {
  FILE *f = fopen(path, "wb"); if (!f) return;
  uint32_t head[8] = {0x43495058u, 1, w, h, fmt, bpp, 0, 1};
  fwrite(head, 4, 8, f);
  for (uint32_t y = 0; y < h; y++) fwrite(rows + y * rowpad, 1, w * bpp, f);
  fclose(f);
}
static LRESULT CALLBACK wp(HWND h, UINT m, WPARAM w, LPARAM l) { return DefWindowProcA(h, m, w, l); }

static void save_picture(const char *prefix) {
  /* the picture this device drew */
  IDirect3DSurface8 *bb = NULL, *img = NULL; D3DSURFACE_DESC sd; D3DLOCKED_RECT lr;
  if (SUCCEEDED(IDirect3DDevice8_GetBackBuffer(d, 0, D3DBACKBUFFER_TYPE_MONO, &bb)) && SUCCEEDED(IDirect3DSurface8_GetDesc(bb, &sd)) &&
      SUCCEEDED(IDirect3DDevice8_CreateImageSurface(d, sd.Width, sd.Height, sd.Format, &img)) &&
      SUCCEEDED(IDirect3DDevice8_CopyRects(d, bb, NULL, 0, img, NULL)) && SUCCEEDED(IDirect3DSurface8_LockRect(img, &lr, NULL, D3DLOCK_READONLY))) {
    uint32_t rb, rows; fmt_rows(sd.Format, sd.Width, sd.Height, &rb, &rows);
    uint8_t *tmp = malloc(rb * rows);
    for (uint32_t y = 0; y < rows; y++) memcpy(tmp + y * rb, (uint8_t *)lr.pBits + (int)y * lr.Pitch, rb);
    char path[MAX_PATH]; snprintf(path, sizeof path, "%s_replay.pic", prefix);
    write_pic(path, sd.Format, sd.Width, sd.Height, rb / sd.Width, tmp, rb);
    free(tmp); IDirect3DSurface8_UnlockRect(img);
  } else printf("replay: the back buffer could not be read\n");
  if (img) IDirect3DSurface8_Release(img); if (bb) IDirect3DSurface8_Release(bb);
}
int main(int argc, char **argv) {
  setvbuf(stdout, NULL, _IONBF, 0);
  if (argc < 3) { printf("usage: fcap_replay CAPTURE.fcap OUT_PREFIX\n"); return 2; }
  FILE *f = fopen(argv[1], "rb"); if (!f) { printf("replay: cannot open %s\n", argv[1]); return 2; }
  fseek(f, 0, SEEK_END); long size = ftell(f); fseek(f, 0, SEEK_SET);
  uint8_t *buf = malloc(size); if (!buf || fread(buf, 1, size, f) != (size_t)size) { printf("replay: cannot read\n"); return 2; }
  fclose(f);
  FcHeader *hd = (FcHeader *)buf;
  if (size < (long)sizeof *hd || hd->magic != FC_MAGIC || hd->version != FC_VERSION) { printf("replay: not a version %u capture\n", FC_VERSION); return 2; }
  g_map = calloc(g_nmap, sizeof(Map));
  printf("capture: frame %u, back buffer %ux%u format %u, depth %ux%u format %u, game TimeDateStamp %08X\n",
         hd->frame, hd->bb_width, hd->bb_height, hd->bb_format, hd->ds_width, hd->ds_height, hd->ds_format, hd->game_timestamp);

  WNDCLASSA wc; memset(&wc, 0, sizeof wc); wc.lpfnWndProc = wp; wc.hInstance = GetModuleHandleA(NULL); wc.lpszClassName = "fcap"; RegisterClassA(&wc);
  RECT r = {0, 0, (LONG)hd->bb_width, (LONG)hd->bb_height}; AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);
  HWND w = CreateWindowA("fcap", "fcap replay", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, r.right - r.left, r.bottom - r.top, NULL, NULL, wc.hInstance, NULL);
  IDirect3D8 *d3d = Direct3DCreate8(D3D_SDK_VERSION);
  if (!d3d) { printf("replay: Direct3DCreate8 failed\n"); return 2; }
  D3DPRESENT_PARAMETERS pp; memset(&pp, 0, sizeof pp);
  pp.Windowed = TRUE; pp.SwapEffect = D3DSWAPEFFECT_DISCARD; pp.hDeviceWindow = w;
  pp.BackBufferWidth = hd->bb_width; pp.BackBufferHeight = hd->bb_height; pp.BackBufferFormat = (D3DFORMAT)hd->bb_format;
  int sep_ds = hd->ds_format && (hd->ds_width != hd->bb_width || hd->ds_height != hd->bb_height);
  if (hd->ds_format && !sep_ds) { pp.EnableAutoDepthStencil = TRUE; pp.AutoDepthStencilFormat = (D3DFORMAT)hd->ds_format; }
  HRESULT hr = IDirect3D8_CreateDevice(d3d, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, w, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &d);
  if (FAILED(hr)) { pp.BackBufferFormat = D3DFMT_X8R8G8B8; hr = IDirect3D8_CreateDevice(d3d, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, w, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &d); }
  if (FAILED(hr)) { printf("replay: CreateDevice failed: 0x%08lX\n", (unsigned long)hr); return 2; }
  IDirect3DSurface8 *sepds = NULL;
  if (sep_ds && SUCCEEDED(IDirect3DDevice8_CreateDepthStencilSurface(d, hd->ds_width, hd->ds_height, (D3DFORMAT)hd->ds_format, D3DMULTISAMPLE_NONE, &sepds))) {}

  uint32_t ncalls = 0, nobj = 0; int ended = 0;
  int reps = argc > 3 ? atoi(argv[3]) : 0, pass = 0;
  uint8_t *p = buf + sizeof *hd, *end = buf + size, *first_call = NULL;
  LARGE_INTEGER qf, t0, t1; QueryPerformanceFrequency(&qf);
  double tsum = 0, tmax = 0, tmin = 1e30;
  for (;;) {
  if (pass > 0) QueryPerformanceCounter(&t0);
  while (p + 8 <= end && !ended) {
    uint32_t tag = ((uint32_t *)p)[0], len = ((uint32_t *)p)[1];
    uint8_t *q = p + 8; p = q + len;
    if (p > end) { printf("replay: the capture is cut short\n"); break; }
    uint32_t *u = (uint32_t *)q;
    if (tag == FC_CALL && !first_call) first_call = q - 8;
    /* a repeat: only the calls and the buffer updates between them (the objects exist) */
    if (pass > 0 && tag != FC_CALL && tag != FC_BUFDATA && tag != FC_END) continue;
    switch (tag) {
    case FC_TEX: case FC_CUBE: {
      int cube = tag == FC_CUBE;
      uint32_t id = u[0], wdt = u[1], hgt = cube ? u[1] : u[2], *x = u + (cube ? 2 : 3);
      uint32_t levels = x[0], usage = x[1], fmt = x[2], pool = x[3];
      uint8_t *dp = (uint8_t *)(x + 4);
      IDirect3DBaseTexture8 *t = NULL;
      hr = cube ? IDirect3DDevice8_CreateCubeTexture(d, wdt, levels, usage, (D3DFORMAT)fmt, (D3DPOOL)pool, (IDirect3DCubeTexture8 **)&t)
                : IDirect3DDevice8_CreateTexture(d, wdt, hgt, levels, usage, (D3DFORMAT)fmt, (D3DPOOL)pool, (IDirect3DTexture8 **)&t);
      if (FAILED(hr)) { fail(cube ? "CreateCubeTexture" : "CreateTexture", hr); break; }
      map_put(id, t); nobj++;
      for (uint32_t face = 0; face < (cube ? 6u : 1u); face++)
        for (uint32_t l = 0; l < levels; l++) {
          uint32_t n = *(uint32_t *)dp; dp += 4;
          uint32_t lw = wdt >> l ? wdt >> l : 1, lh = hgt >> l ? hgt >> l : 1;
          if (n) {
            uint32_t rb, rows;
            if (fmt_rows(fmt, lw, lh, &rb, &rows)) {
              D3DLOCKED_RECT lr;
              if (pool != D3DPOOL_DEFAULT) {
                hr = cube ? IDirect3DCubeTexture8_LockRect((IDirect3DCubeTexture8 *)t, (D3DCUBEMAP_FACES)face, l, &lr, NULL, 0)
                          : IDirect3DTexture8_LockRect((IDirect3DTexture8 *)t, l, &lr, NULL, 0);
                if (SUCCEEDED(hr)) {
                  put_rows(dp, lr.pBits, lr.Pitch, rb, rows);
                  if (cube) IDirect3DCubeTexture8_UnlockRect((IDirect3DCubeTexture8 *)t, (D3DCUBEMAP_FACES)face, l); else IDirect3DTexture8_UnlockRect((IDirect3DTexture8 *)t, l);
                } else fail("LockRect of a texture level", hr);
              } else {
                IDirect3DSurface8 *s = NULL;
                hr = cube ? IDirect3DCubeTexture8_GetCubeMapSurface((IDirect3DCubeTexture8 *)t, (D3DCUBEMAP_FACES)face, l, &s)
                          : IDirect3DTexture8_GetSurfaceLevel((IDirect3DTexture8 *)t, l, &s);
                if (SUCCEEDED(hr)) { fill_surface(s, fmt, lw, lh, dp, 0); IDirect3DSurface8_Release(s); }
              }
            }
          }
          dp += pad4(n);
        }
      break; }
    case FC_VB: case FC_IB: {
      uint32_t id = u[0], length = u[1], usage = u[2], ff = u[3], pool = u[4], have = u[5];
      void *b = NULL;
      hr = tag == FC_VB ? IDirect3DDevice8_CreateVertexBuffer(d, length, usage, ff, (D3DPOOL)pool, (IDirect3DVertexBuffer8 **)&b)
                        : IDirect3DDevice8_CreateIndexBuffer(d, length, usage, (D3DFORMAT)ff, (D3DPOOL)pool, (IDirect3DIndexBuffer8 **)&b);
      if (FAILED(hr)) { fail(tag == FC_VB ? "CreateVertexBuffer" : "CreateIndexBuffer", hr); break; }
      map_put(id, b); nobj++;
      if (have) {
        BYTE *m = NULL; DWORD fl = (usage & D3DUSAGE_DYNAMIC) ? D3DLOCK_DISCARD : 0;
        hr = tag == FC_VB ? IDirect3DVertexBuffer8_Lock((IDirect3DVertexBuffer8 *)b, 0, 0, &m, fl) : IDirect3DIndexBuffer8_Lock((IDirect3DIndexBuffer8 *)b, 0, 0, &m, fl);
        if (SUCCEEDED(hr)) { memcpy(m, u + 6, length); if (tag == FC_VB) IDirect3DVertexBuffer8_Unlock((IDirect3DVertexBuffer8 *)b); else IDirect3DIndexBuffer8_Unlock((IDirect3DIndexBuffer8 *)b); }
        else fail("Lock of a buffer", hr);
      }
      break; }
    case FC_SURF: {
      uint32_t id = u[0], kind = u[1], cont = u[2], level = u[3], face = u[4], fmt = u[5], wd = u[6], ht = u[7], ms = u[10], have = u[11];
      IDirect3DSurface8 *s = NULL; hr = E_FAIL;
      switch (kind) {
      case FS_TEXLEVEL: { void *t = map_get(cont); if (t) hr = IDirect3DTexture8_GetSurfaceLevel((IDirect3DTexture8 *)t, level, &s); break; }
      case FS_CUBEFACE: { void *t = map_get(cont); if (t) hr = IDirect3DCubeTexture8_GetCubeMapSurface((IDirect3DCubeTexture8 *)t, (D3DCUBEMAP_FACES)face, level, &s); break; }
      case FS_BACKBUFFER: hr = IDirect3DDevice8_GetBackBuffer(d, 0, D3DBACKBUFFER_TYPE_MONO, &s); break;
      case FS_AUTODEPTH: if (sepds) { s = sepds; IDirect3DSurface8_AddRef(s); hr = D3D_OK; } else hr = IDirect3DDevice8_GetDepthStencilSurface(d, &s); break;
      case FS_RENDERTARGET: hr = IDirect3DDevice8_CreateRenderTarget(d, wd, ht, (D3DFORMAT)fmt, (D3DMULTISAMPLE_TYPE)ms, TRUE, &s); break;
      case FS_DEPTHSTENCIL: hr = IDirect3DDevice8_CreateDepthStencilSurface(d, wd, ht, (D3DFORMAT)fmt, (D3DMULTISAMPLE_TYPE)ms, &s); break;
      case FS_IMAGE: hr = IDirect3DDevice8_CreateImageSurface(d, wd, ht, (D3DFORMAT)fmt, &s); break;
      }
      if (FAILED(hr) || !s) { fail("a surface", hr); break; }
      map_put(id, s); nobj++;
      if (have) fill_surface(s, fmt, wd, ht, (uint8_t *)(u + 13), kind == FS_IMAGE);
      break; }
    case FC_VSDECL: {
      uint32_t h = u[0], nd = u[1], nf = u[2], nh = 0;
      hr = IDirect3DDevice8_CreateVertexShader(d, (DWORD *)(u + 3), nf ? (DWORD *)(u + 3 + nd) : NULL, (DWORD *)&nh, 0);
      if (FAILED(hr)) { fail("CreateVertexShader", hr); break; }
      if (g_nvs < 4096) { g_vsmap[g_nvs][0] = h; g_vsmap[g_nvs][1] = nh; g_nvs++; }
      break; }
    case FC_BUFDATA: {
      void *b = map_get(u[0]); BYTE *m = NULL;
      if (!b) { fail("buffer data for an unknown buffer", 0); break; }
      D3DRESOURCETYPE ty = IDirect3DResource8_GetType((IDirect3DResource8 *)b);
      hr = ty == D3DRTYPE_VERTEXBUFFER ? IDirect3DVertexBuffer8_Lock((IDirect3DVertexBuffer8 *)b, u[1], u[2], &m, D3DLOCK_NOOVERWRITE)
                                       : IDirect3DIndexBuffer8_Lock((IDirect3DIndexBuffer8 *)b, u[1], u[2], &m, D3DLOCK_NOOVERWRITE);
      if (SUCCEEDED(hr)) { memcpy(m, u + 3, u[2]); if (ty == D3DRTYPE_VERTEXBUFFER) IDirect3DVertexBuffer8_Unlock((IDirect3DVertexBuffer8 *)b); else IDirect3DIndexBuffer8_Unlock((IDirect3DIndexBuffer8 *)b); }
      else fail("Lock for buffer data", hr);
      break; }
    case FC_CALL: {
      int slot = (int)(u[0] & 0xFFFF); uint32_t n = u[1], a[8] = {0};
      if (n > 8) { fail("a call with more than 8 arguments", 0); break; }
      memcpy(a, u + 2, 4 * n);
      uint32_t *dq = u + 2 + n, nd = *dq++; uint8_t *dat[2] = {0}; uint32_t dl[2] = {0};
      for (uint32_t k = 0; k < nd && k < 2; k++) { dl[k] = *dq++; dat[k] = (uint8_t *)dq; dq += pad4(dl[k]) / 4; }
      /* objects and handles of this run; data in the capture */
      switch (slot) {
      case FS_SETTEXTURE: a[1] = (uint32_t)(uintptr_t)map_get(a[1]); break;
      case FS_SSS: a[1] = (uint32_t)(uintptr_t)map_get(a[1]); break;
      case FS_SETINDICES: a[0] = (uint32_t)(uintptr_t)map_get(a[0]); break;
      case FS_SETRT: a[0] = (uint32_t)(uintptr_t)map_get(a[0]); a[1] = (uint32_t)(uintptr_t)map_get(a[1]); break;
      case FS_COPYRECTS: a[0] = (uint32_t)(uintptr_t)map_get(a[0]); a[3] = (uint32_t)(uintptr_t)map_get(a[3]);
                         a[1] = dl[0] ? (uint32_t)(uintptr_t)dat[0] : 0; a[4] = dl[1] ? (uint32_t)(uintptr_t)dat[1] : 0; break;
      case FS_UPDATETEXTURE: a[0] = (uint32_t)(uintptr_t)map_get(a[0]); a[1] = (uint32_t)(uintptr_t)map_get(a[1]); break;
      case FS_SETVS: a[0] = vs_get(a[0]); break;
      case FS_SETTRANSFORM: case FS_MULTIPLYTRANSFORM: case FS_SETLIGHT: case FS_SETCLIPPLANE: case FS_SETPALETTE:
      case FS_SETVSCONST: case FS_SETPSCONST: a[1] = (uint32_t)(uintptr_t)dat[0]; break;
      case FS_SETVIEWPORT: case FS_SETMATERIAL: case FS_SETCLIPSTATUS: a[0] = (uint32_t)(uintptr_t)dat[0]; break;
      case FS_CLEAR: a[1] = dl[0] ? (uint32_t)(uintptr_t)dat[0] : 0; break;
      case FS_DPUP: a[2] = (uint32_t)(uintptr_t)dat[0]; break;
      case FS_DIPUP: a[4] = (uint32_t)(uintptr_t)dat[0]; a[6] = (uint32_t)(uintptr_t)dat[1]; break;
      case FS_PROCESSVERTICES: case FS_SETPS: case FS_CREATEPS: continue;   /* noted by the capture */
      }
      hr = call(slot, a, (int)n);
      ncalls++;
      if (FAILED(hr) && slot != FS_SETRS && slot != FS_SETTSS) { char b[96]; snprintf(b, sizeof b, "%s%s", slot_name(slot), (u[0] & FC_SYNTH) ? " (start state / state block)" : ""); fail(b, hr); }
      break; }
    case FC_PICTURE: {
      uint32_t fmt = u[0], wd = u[1], ht = u[2], bpp = u[3];
      char path[MAX_PATH]; snprintf(path, sizeof path, "%s_capture.pic", argv[2]);
      write_pic(path, fmt, wd, ht, bpp, (uint8_t *)(u + 4), pad4(wd * bpp));
      break; }
    case FC_NOTE: printf("capture note: %s\n", (char *)q); break;
    case FC_END: ended = 1; break;
    default: printf("replay: unknown record %u\n", tag);
    }
  }
  if (pass == 0) save_picture(argv[2]);
  else {
    IDirect3DDevice8_Present(d, NULL, NULL, NULL, NULL);
    QueryPerformanceCounter(&t1);
    double ms = (double)(t1.QuadPart - t0.QuadPart) * 1000.0 / (double)qf.QuadPart;
    tsum += ms; if (ms > tmax) tmax = ms; if (ms < tmin) tmin = ms;
  }
  if (!ended || !first_call || pass >= reps) break;
  if (pass == 0) { IDirect3DDevice8_Present(d, NULL, NULL, NULL, NULL); printf("replay: %u objects, %u calls, %d failures\n", nobj, ncalls, g_fail); }
  pass++; p = first_call; ended = 0;
  }
  if (reps > 0 && pass > 0)
    printf("replay: %d repeats of the frame's calls with a Present after each: %.3f ms a frame on average, %.3f at least, "
           "%.3f at most (this machine and device)\n", pass, tsum / pass, tmin, tmax);
  printf("replay: %u objects, %u calls, %d failures%s\n", nobj, ncalls, g_fail, ended ? "" : " (no end record)");
  IDirect3DDevice8_Present(d, NULL, NULL, NULL, NULL);
  IDirect3DDevice8_Release(d); IDirect3D8_Release(d3d);
  return ended ? 0 : 1;
}
