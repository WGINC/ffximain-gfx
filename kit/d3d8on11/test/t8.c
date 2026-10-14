/* t8.c -- d3d8on11's test program: the same D3D8 calls through whichever d3d8.dll Windows loads (Wine's own, or ours
   next to this program with WINEDLLOVERRIDES=d3d8=n). test_d3d8on11.py runs both and compares.

     t8 defaults    every render state and texture stage state of a fresh device, one per line ("rs N V", "tss S T V")
     t8 basic       the milestone-1 checks, one line each: "ok: ..." or "FAIL: ..." */
#define COBJMACROS
#include <windows.h>
#include <d3d8.h>
#include <stdio.h>
#include <string.h>

static IDirect3DDevice8 *d; static int fails;
static void check(int c, const char *what) { printf("%s: %s\n", c ? "ok" : "FAIL", what); if (!c) fails++; }
static LRESULT CALLBACK wp(HWND h, UINT m, WPARAM w, LPARAM l) { return DefWindowProcA(h, m, w, l); }

static HWND window(void) {
  WNDCLASSA wc; memset(&wc, 0, sizeof wc); wc.lpfnWndProc = wp; wc.hInstance = GetModuleHandleA(NULL); wc.lpszClassName = "t8"; RegisterClassA(&wc);
  return CreateWindowA("t8", "t8", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 320, 240, NULL, NULL, wc.hInstance, NULL);
}
static IDirect3D8 *g_d3d;
static HRESULT make(HWND w, D3DPRESENT_PARAMETERS *pp) {
  memset(pp, 0, sizeof *pp);
  pp->Windowed = TRUE; pp->SwapEffect = D3DSWAPEFFECT_DISCARD; pp->BackBufferFormat = D3DFMT_X8R8G8B8;
  pp->BackBufferWidth = 64; pp->BackBufferHeight = 64; pp->EnableAutoDepthStencil = TRUE; pp->AutoDepthStencilFormat = D3DFMT_D24S8;
  pp->hDeviceWindow = w;
  return IDirect3D8_CreateDevice(g_d3d, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, w, D3DCREATE_HARDWARE_VERTEXPROCESSING, pp, &d);
}
/* the back buffer's pixel at x,y, read through CopyRects into an image surface (as the game's code reads surfaces) */
static DWORD bb_pixel(int x, int y) {
  IDirect3DSurface8 *bb = NULL, *img = NULL; DWORD v = 0xDEADBEEF; D3DLOCKED_RECT lr;
  IDirect3DDevice8_GetBackBuffer(d, 0, D3DBACKBUFFER_TYPE_MONO, &bb);
  IDirect3DDevice8_CreateImageSurface(d, 64, 64, D3DFMT_X8R8G8B8, &img);
  if (bb && img && SUCCEEDED(IDirect3DDevice8_CopyRects(d, bb, NULL, 0, img, NULL)) &&
      SUCCEEDED(IDirect3DSurface8_LockRect(img, &lr, NULL, D3DLOCK_READONLY))) {
    v = ((DWORD *)((BYTE *)lr.pBits + y * lr.Pitch))[x] & 0xFFFFFF; IDirect3DSurface8_UnlockRect(img);
  }
  if (img) IDirect3DSurface8_Release(img); if (bb) IDirect3DSurface8_Release(bb);
  return v;
}

int main(int argc, char **argv) {
  setvbuf(stdout, NULL, _IONBF, 0);
  g_d3d = Direct3DCreate8(D3D_SDK_VERSION);
  if (!g_d3d) { printf("FAIL: Direct3DCreate8\n"); return 2; }
  HWND w = window(); D3DPRESENT_PARAMETERS pp;
  HRESULT hr = make(w, &pp);
  if (FAILED(hr)) { printf("FAIL: CreateDevice 0x%08lX\n", (unsigned long)hr); return 2; }
  if (argc > 1 && !strcmp(argv[1], "defaults")) {
    for (int s = 0; s < 256; s++) { DWORD v = 0; if (SUCCEEDED(IDirect3DDevice8_GetRenderState(d, (D3DRENDERSTATETYPE)s, &v))) printf("rs %d %08lX\n", s, (unsigned long)v); }
    for (int st = 0; st < 8; st++) for (int t = 1; t < 29; t++) { DWORD v = 0; if (SUCCEEDED(IDirect3DDevice8_GetTextureStageState(d, st, (D3DTEXTURESTAGESTATETYPE)t, &v))) printf("tss %d %d %08lX\n", st, t, (unsigned long)v); }
    IDirect3DDevice8_Release(d); IDirect3D8_Release(g_d3d);
    return 0;
  }

  /* Clear and read back */
  IDirect3DDevice8_Clear(d, 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFF0000, 1.0f, 0);
  check(bb_pixel(5, 5) == 0xFF0000, "Clear: the back buffer reads back red through CopyRects into an image surface");
  hr = IDirect3DDevice8_Present(d, NULL, NULL, NULL, NULL);
  check(hr == D3D_OK, "Present");

  /* a lockable render target: draw target, clear, read back (the occlusion readback's path) */
  { IDirect3DSurface8 *rt = NULL, *bb = NULL, *ds = NULL; D3DLOCKED_RECT lr; DWORD px = 0;
    IDirect3DDevice8_CreateRenderTarget(d, 16, 16, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, TRUE, &rt);
    IDirect3DDevice8_GetRenderTarget(d, &bb); IDirect3DDevice8_GetDepthStencilSurface(d, &ds);
    IDirect3DDevice8_SetRenderTarget(d, rt, NULL);
    D3DVIEWPORT8 vp; IDirect3DDevice8_GetViewport(d, &vp);
    check(vp.Width == 16 && vp.Height == 16, "SetRenderTarget sets the viewport to the new target");
    IDirect3DDevice8_Clear(d, 0, NULL, D3DCLEAR_TARGET, 0xFF0000FF, 1.0f, 0);
    if (rt && SUCCEEDED(IDirect3DSurface8_LockRect(rt, &lr, NULL, D3DLOCK_READONLY))) { px = ((DWORD *)lr.pBits)[17] & 0xFFFFFF; IDirect3DSurface8_UnlockRect(rt); }
    check(px == 0x0000FF, "a lockable render target: cleared blue, read back blue");
    IDirect3DDevice8_SetRenderTarget(d, bb, ds);
    if (bb) IDirect3DSurface8_Release(bb); if (ds) IDirect3DSurface8_Release(ds); if (rt) IDirect3DSurface8_Release(rt); }

  /* textures and buffers */
  { IDirect3DTexture8 *t = NULL; D3DLOCKED_RECT lr; D3DSURFACE_DESC sd;
    hr = IDirect3DDevice8_CreateTexture(d, 32, 32, 0, 0, D3DFMT_DXT1, D3DPOOL_MANAGED, &t);
    check(hr == D3D_OK && t && IDirect3DTexture8_GetLevelCount(t) == 6, "a 32x32 DXT1 texture with a full chain has 6 levels");
    if (t) {
      hr = IDirect3DTexture8_LockRect(t, 0, &lr, NULL, 0);
      check(hr == D3D_OK && lr.Pitch == 64, "DXT1 level 0: Pitch 64 (8 blocks of 8 bytes)");
      if (hr == D3D_OK) { memset(lr.pBits, 0x55, 64 * 8); IDirect3DTexture8_UnlockRect(t, 0); }
      IDirect3DTexture8_GetLevelDesc(t, 5, &sd);
      check(sd.Width == 1 && sd.Height == 1, "level 5 is 1x1");
      IDirect3DSurface8 *s = NULL; IDirect3DTexture8_GetSurfaceLevel(t, 1, &s);
      check(s != NULL, "GetSurfaceLevel");
      if (s) IDirect3DSurface8_Release(s);
      IDirect3DDevice8_SetTexture(d, 0, (IDirect3DBaseTexture8 *)t);
      IDirect3DBaseTexture8 *g = NULL; IDirect3DDevice8_GetTexture(d, 0, &g);
      check(g == (IDirect3DBaseTexture8 *)t, "GetTexture gives the bound texture");
      if (g) IDirect3DBaseTexture8_Release(g);
      IDirect3DDevice8_SetTexture(d, 0, NULL);
      IDirect3DTexture8_Release(t);
    } }
  { IDirect3DVertexBuffer8 *vb = NULL; BYTE *p = NULL;
    hr = IDirect3DDevice8_CreateVertexBuffer(d, 4096, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &vb);
    check(hr == D3D_OK, "a dynamic vertex buffer");
    if (vb) { check(SUCCEEDED(IDirect3DVertexBuffer8_Lock(vb, 0, 0, &p, D3DLOCK_DISCARD)) && p, "Lock DISCARD of the whole buffer");
      if (p) { memset(p, 1, 4096); IDirect3DVertexBuffer8_Unlock(vb); }
      IDirect3DDevice8_SetStreamSource(d, 0, vb, 24);
      IDirect3DVertexBuffer8 *g = NULL; UINT stride = 0; IDirect3DDevice8_GetStreamSource(d, 0, &g, &stride);
      check(g == vb && stride == 24, "GetStreamSource");
      if (g) IDirect3DVertexBuffer8_Release(g);
      IDirect3DDevice8_SetStreamSource(d, 0, NULL, 0);
      IDirect3DVertexBuffer8_Release(vb); } }

  /* state blocks: the same check as probe8's */
  { DWORD tok = 0, v1 = 0, v2 = 0;
    IDirect3DDevice8_SetRenderState(d, D3DRS_ALPHAREF, 0x10); IDirect3DDevice8_SetRenderState(d, D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    IDirect3DDevice8_BeginStateBlock(d);
    IDirect3DDevice8_SetRenderState(d, D3DRS_ALPHAREF, 0x60); IDirect3DDevice8_SetRenderState(d, D3DRS_ALPHAREF, 0x61);
    IDirect3DDevice8_EndStateBlock(d, &tok);
    IDirect3DDevice8_GetRenderState(d, D3DRS_ALPHAREF, &v1);
    check(v1 == 0x10, "a set while a block records is recorded, not applied");
    IDirect3DDevice8_SetRenderState(d, D3DRS_ZFUNC, D3DCMP_ALWAYS);
    IDirect3DDevice8_ApplyStateBlock(d, tok);
    IDirect3DDevice8_GetRenderState(d, D3DRS_ALPHAREF, &v1); IDirect3DDevice8_GetRenderState(d, D3DRS_ZFUNC, &v2);
    check(v1 == 0x61 && v2 == D3DCMP_ALWAYS, "applying it sets its last value and leaves the rest");
    IDirect3DDevice8_DeleteStateBlock(d, tok); }

  /* transforms and lights read back */
  { D3DMATRIX m, g; memset(&m, 0, sizeof m); m._11 = 2; m._22 = 3; m._33 = 4; m._44 = 1; m._41 = 7;
    IDirect3DDevice8_SetTransform(d, D3DTS_WORLD, &m); IDirect3DDevice8_GetTransform(d, D3DTS_WORLD, &g);
    check(!memcmp(&m, &g, sizeof m), "GetTransform gives the matrix set");
    D3DLIGHT8 l, gl; memset(&l, 0, sizeof l); l.Type = D3DLIGHT_POINT; l.Range = 5; l.Diffuse.g = 1;
    IDirect3DDevice8_SetLight(d, 3, &l); IDirect3DDevice8_LightEnable(d, 3, TRUE); BOOL on = FALSE;
    IDirect3DDevice8_GetLight(d, 3, &gl); IDirect3DDevice8_GetLightEnable(d, 3, &on);
    check(!memcmp(&l, &gl, sizeof l) && on, "GetLight and GetLightEnable give what was set"); }

  /* vertex shader handles: declarations only, as the game makes them */
  { static const DWORD decl[] = {D3DVSD_STREAM(0), D3DVSD_REG(0, D3DVSDT_FLOAT3), D3DVSD_REG(3, D3DVSDT_FLOAT3), D3DVSD_END()};
    DWORD h = 0, g = 0;
    hr = IDirect3DDevice8_CreateVertexShader(d, decl, NULL, &h, 0);
    check(hr == D3D_OK && (h & 1), "CreateVertexShader of a declaration: a handle with bit 0 set");
    IDirect3DDevice8_SetVertexShader(d, h); IDirect3DDevice8_GetVertexShader(d, &g);
    check(g == h, "GetVertexShader");
    IDirect3DDevice8_SetVertexShader(d, D3DFVF_XYZ | D3DFVF_DIFFUSE);
    check(IDirect3DDevice8_DeleteVertexShader(d, h) == D3D_OK, "DeleteVertexShader"); }

  /* Reset to another size */
  pp.BackBufferWidth = 96; pp.BackBufferHeight = 80;
  hr = IDirect3DDevice8_Reset(d, &pp);
  check(hr == D3D_OK, "Reset to 96x80");
  { IDirect3DSurface8 *bb = NULL; D3DSURFACE_DESC sd; memset(&sd, 0, sizeof sd);
    IDirect3DDevice8_GetBackBuffer(d, 0, D3DBACKBUFFER_TYPE_MONO, &bb);
    if (bb) { IDirect3DSurface8_GetDesc(bb, &sd); IDirect3DSurface8_Release(bb); }
    check(sd.Width == 96 && sd.Height == 80, "after Reset the back buffer is 96x80"); }

  ULONG left = IDirect3DDevice8_Release(d);
  check(left == 0, "the device's last reference released");
  IDirect3D8_Release(g_d3d);
  printf(fails ? "FAILED (%d)\n" : "ALL OK\n", fails);
  return fails ? 1 : 0;
}
