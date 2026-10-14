/* t8.c -- d3d8on11's test program: the same D3D8 calls through whichever d3d8.dll Windows loads (Wine's own, or ours
   next to this program with WINEDLLOVERRIDES=d3d8=n). test_d3d8on11.py runs both and compares.

     t8 defaults    every render state and texture stage state of a fresh device, one per line ("rs N V", "tss S T V")
     t8 basic       the milestone-1 checks, one line each: "ok: ..." or "FAIL: ..."
     t8 scenes      feature scenes (the vertex formats, primitives, states and stage setups the game uses, data/d3d8_scope.json),
                    each drawn on a 64x64 back buffer and read back whole: "scene NAME <hex RGB per pixel>"
     t8 random FILE the cases of FILE (random_cases.py: states drawn from the values the game sets), each drawn on a
                    64x64 back buffer and read back whole, as the scenes are
     t8 threads     a second thread creates, writes and releases textures and buffers while this one draws, presents and
                    reads back; then a textured draw must read back right. "ok:" / "FAIL:" lines
     t8 churn       5,000 rounds of creating and releasing every kind of object; then a textured draw must read back
                    right (ours logs at the end how many objects its executor still holds)
     t8 frames      600 frames of Clear and Present, with a back-buffer read-back from frame 30: "presented N" lines
     t8 orphan      (ours, remote mode) the same, ending every d8x*.exe process at frame 30: the device must end this
                    process (exit code 3), not hang */
#define COBJMACROS
#include <windows.h>
#include <d3d8.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <tlhelp32.h>

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

/* ---- scenes ------------------------------------------------------------------------------------------------------- */
typedef struct { float x, y, z, rhw; DWORD c; } VRhw;                       /* 0x44  XYZRHW|DIFFUSE */
typedef struct { float x, y, z, rhw; DWORD c; float u, v; } VRhwT;          /* 0x144 XYZRHW|DIFFUSE|TEX1 */
typedef struct { float x, y, z; DWORD c; } VXyz;                            /* 0x42  XYZ|DIFFUSE */
typedef struct { float x, y, z; DWORD c; float u, v; } VXyzT;               /* 0x142 XYZ|DIFFUSE|TEX1 */
typedef struct { float x, y, z, nx, ny, nz; DWORD c; float u, v; } VXyzN;   /* 0x152 XYZ|NORMAL|DIFFUSE|TEX1 */

static void dump(const char *name) {
  IDirect3DSurface8 *bb = NULL, *img = NULL; D3DLOCKED_RECT lr;
  IDirect3DDevice8_GetBackBuffer(d, 0, D3DBACKBUFFER_TYPE_MONO, &bb);
  IDirect3DDevice8_CreateImageSurface(d, 64, 64, D3DFMT_X8R8G8B8, &img);
  printf("scene %s ", name);
  if (bb && img && SUCCEEDED(IDirect3DDevice8_CopyRects(d, bb, NULL, 0, img, NULL)) &&
      SUCCEEDED(IDirect3DSurface8_LockRect(img, &lr, NULL, D3DLOCK_READONLY))) {
    for (int y = 0; y < 64; y++) for (int x = 0; x < 64; x++) printf("%06lX", (unsigned long)(((DWORD *)((BYTE *)lr.pBits + y * lr.Pitch))[x] & 0xFFFFFF));
    IDirect3DSurface8_UnlockRect(img);
  } else printf("none");
  printf("\n");
  if (img) IDirect3DSurface8_Release(img); if (bb) IDirect3DSurface8_Release(bb);
}
static void begin(DWORD clear) {
  IDirect3DDevice8_Clear(d, 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, clear, 1.0f, 0);
  IDirect3DDevice8_BeginScene(d);
}
static void end(const char *name) { IDirect3DDevice8_EndScene(d); dump(name); }
static void reset_state(void) {
  /* back to the state a scene starts from: the defaults the scenes rely on, set explicitly */
  D3DMATRIX id; memset(&id, 0, sizeof id); id._11 = id._22 = id._33 = id._44 = 1;
  IDirect3DDevice8_SetTransform(d, D3DTS_WORLD, &id); IDirect3DDevice8_SetTransform(d, D3DTS_VIEW, &id);
  IDirect3DDevice8_SetTransform(d, D3DTS_PROJECTION, &id); IDirect3DDevice8_SetTransform(d, D3DTS_TEXTURE0, &id);
  IDirect3DDevice8_SetRenderState(d, D3DRS_LIGHTING, FALSE); IDirect3DDevice8_SetRenderState(d, D3DRS_CULLMODE, D3DCULL_NONE);
  IDirect3DDevice8_SetRenderState(d, D3DRS_ZENABLE, D3DZB_FALSE); IDirect3DDevice8_SetRenderState(d, D3DRS_ZWRITEENABLE, TRUE);
  IDirect3DDevice8_SetRenderState(d, D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
  IDirect3DDevice8_SetRenderState(d, D3DRS_ALPHABLENDENABLE, FALSE); IDirect3DDevice8_SetRenderState(d, D3DRS_ALPHATESTENABLE, FALSE);
  IDirect3DDevice8_SetRenderState(d, D3DRS_FOGENABLE, FALSE); IDirect3DDevice8_SetRenderState(d, D3DRS_SPECULARENABLE, FALSE);
  IDirect3DDevice8_SetRenderState(d, D3DRS_FOGTABLEMODE, D3DFOG_NONE); IDirect3DDevice8_SetRenderState(d, D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
  for (int i = 0; i < 4; i++) {
    IDirect3DDevice8_SetTexture(d, i, NULL);
    IDirect3DDevice8_SetTextureStageState(d, i, D3DTSS_COLOROP, i ? D3DTOP_DISABLE : D3DTOP_MODULATE);
    IDirect3DDevice8_SetTextureStageState(d, i, D3DTSS_ALPHAOP, i ? D3DTOP_DISABLE : D3DTOP_SELECTARG1);
    IDirect3DDevice8_SetTextureStageState(d, i, D3DTSS_COLORARG1, D3DTA_TEXTURE); IDirect3DDevice8_SetTextureStageState(d, i, D3DTSS_COLORARG2, D3DTA_CURRENT);
    IDirect3DDevice8_SetTextureStageState(d, i, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); IDirect3DDevice8_SetTextureStageState(d, i, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
    IDirect3DDevice8_SetTextureStageState(d, i, D3DTSS_TEXCOORDINDEX, i);
    IDirect3DDevice8_SetTextureStageState(d, i, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
    IDirect3DDevice8_SetTextureStageState(d, i, D3DTSS_MAGFILTER, D3DTEXF_POINT); IDirect3DDevice8_SetTextureStageState(d, i, D3DTSS_MINFILTER, D3DTEXF_POINT);
    IDirect3DDevice8_SetTextureStageState(d, i, D3DTSS_MIPFILTER, D3DTEXF_NONE);
    IDirect3DDevice8_SetTextureStageState(d, i, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP); IDirect3DDevice8_SetTextureStageState(d, i, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
  }
  IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE); IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
}
/* a texture: w x h A8R8G8B8, each texel from f(x, y) */
static IDirect3DTexture8 *texture(int w, int h, DWORD (*f)(int, int)) {
  IDirect3DTexture8 *t = NULL; D3DLOCKED_RECT lr;
  if (FAILED(IDirect3DDevice8_CreateTexture(d, w, h, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &t))) return NULL;
  IDirect3DTexture8_LockRect(t, 0, &lr, NULL, 0);
  for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) ((DWORD *)((BYTE *)lr.pBits + y * lr.Pitch))[x] = f(x, y);
  IDirect3DTexture8_UnlockRect(t, 0);
  return t;
}
static DWORD tx_checker(int x, int y) { return ((x ^ y) & 1) ? 0xFFFFFFFF : 0xFF2040C0; }
static DWORD tx_grad(int x, int y) { return 0x80000000 | (x * 16) << 16 | (y * 16) << 8 | 0x40; }
static DWORD tx_alpha(int x, int y) { return (DWORD)((x * 255 / 15) << 24) | 0x00FF8000; }
static void persp(D3DMATRIX *m, float fov, float zn, float zf) {   /* the D3DX left-handed perspective, written out */
  float ys = 1.0f / (float)tan(fov / 2); memset(m, 0, sizeof *m);
  m->_11 = ys; m->_22 = ys; m->_33 = zf / (zf - zn); m->_34 = 1; m->_43 = -zn * zf / (zf - zn);
}
static void quad_rhw(float x0, float y0, float x1, float y1, float z, DWORD c0, DWORD c1, DWORD c2, DWORD c3) {
  VRhw v[4] = {{x0, y0, z, 1, c0}, {x1, y0, z, 1, c1}, {x0, y1, z, 1, c2}, {x1, y1, z, 1, c3}};
  IDirect3DDevice8_SetVertexShader(d, D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
  IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLESTRIP, 2, v, sizeof *v);
}
static void scenes(void) {
  IDirect3DTexture8 *chk = texture(16, 16, tx_checker), *grad = texture(16, 16, tx_grad), *alp = texture(16, 16, tx_alpha);
  D3DMATRIX proj, view; persp(&proj, 1.0f, 1.0f, 100.0f);
  memset(&view, 0, sizeof view); view._11 = view._22 = view._33 = view._44 = 1; view._43 = 5;    /* camera 5 units back */

  /* pre-transformed rectangles on whole and half pixel edges: where D3D8 puts pixel centres */
  reset_state(); begin(0xFF000000);
  quad_rhw(8, 8, 24, 20, 0.5f, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000);
  quad_rhw(30.5f, 8.5f, 40.5f, 20.5f, 0.5f, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00);
  quad_rhw(8, 30, 56, 56, 0.5f, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFFFFFFFF);
  end("rhw_rects");

  /* a 16x16 texture drawn 1:1 and stretched 2x, point filtered: where texel centres fall */
  reset_state(); begin(0xFF000000);
  IDirect3DDevice8_SetTexture(d, 0, (IDirect3DBaseTexture8 *)chk);
  IDirect3DDevice8_SetVertexShader(d, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
  { VRhwT v[4] = {{4, 4, 0.5f, 1, 0xFFFFFFFF, 0, 0}, {20, 4, 0.5f, 1, 0xFFFFFFFF, 1, 0}, {4, 20, 0.5f, 1, 0xFFFFFFFF, 0, 1}, {20, 20, 0.5f, 1, 0xFFFFFFFF, 1, 1}};
    IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLESTRIP, 2, v, sizeof *v); }
  IDirect3DDevice8_SetTexture(d, 0, (IDirect3DBaseTexture8 *)grad);
  { VRhwT v[4] = {{28, 4, 0.5f, 1, 0xFFFFFFFF, 0, 0}, {60, 4, 0.5f, 1, 0xFFFFFFFF, 1, 0}, {28, 36, 0.5f, 1, 0xFFFFFFFF, 0, 1}, {60, 36, 0.5f, 1, 0xFFFFFFFF, 1, 1}};
    IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLESTRIP, 2, v, sizeof *v); }
  IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR); IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
  IDirect3DDevice8_SetTexture(d, 0, (IDirect3DBaseTexture8 *)chk);
  { VRhwT v[4] = {{4, 40, 0.5f, 1, 0xFFFF8080, 0, 0}, {36, 40, 0.5f, 1, 0xFFFF8080, 1, 0}, {4, 60, 0.5f, 1, 0xFFFF8080, 0, 1}, {36, 60, 0.5f, 1, 0xFFFF8080, 1, 1}};
    IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLESTRIP, 2, v, sizeof *v); }
  end("rhw_texture");

  /* transformed triangles with a perspective camera, depth test and culling */
  reset_state(); begin(0xFF101010);
  IDirect3DDevice8_SetTransform(d, D3DTS_PROJECTION, &proj); IDirect3DDevice8_SetTransform(d, D3DTS_VIEW, &view);
  IDirect3DDevice8_SetRenderState(d, D3DRS_ZENABLE, D3DZB_TRUE); IDirect3DDevice8_SetRenderState(d, D3DRS_CULLMODE, D3DCULL_CCW);
  IDirect3DDevice8_SetVertexShader(d, D3DFVF_XYZ | D3DFVF_DIFFUSE);
  { VXyz a[3] = {{-2, -1.5f, 1, 0xFFFF0000}, {0, 2, 1, 0xFFFF0000}, {2, -1.5f, 1, 0xFFFF0000}};          /* clockwise: drawn */
    VXyz b[3] = {{-1.5f, 1.5f, 0, 0xFF00FF00}, {2.5f, 1, 2, 0xFF00FF00}, {-1, -2, 0, 0xFF00FF00}};       /* crosses a in depth */
    VXyz c[3] = {{-2, 2, 0, 0xFF0000FF}, {2, 2, 0, 0xFF0000FF}, {0, -2, 0, 0xFF0000FF}};                 /* clockwise? listed for cull */
    VXyz e[3] = {{-2, -2, -1, 0xFFFFFF00}, {0, -1, -1, 0xFFFFFF00}, {2, -2, -1, 0xFFFFFF00}};            /* counter-clockwise: culled */
    IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLELIST, 1, a, sizeof *a);
    IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLELIST, 1, b, sizeof *b);
    IDirect3DDevice8_SetRenderState(d, D3DRS_CULLMODE, D3DCULL_CW);
    IDirect3DDevice8_SetRenderState(d, D3DRS_ZFUNC, D3DCMP_GREATER);
    IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLELIST, 1, c, sizeof *c);
    IDirect3DDevice8_SetRenderState(d, D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    IDirect3DDevice8_SetRenderState(d, D3DRS_CULLMODE, D3DCULL_CCW);
    IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLELIST, 1, e, sizeof *e); }
  end("xyz_depth_cull");

  /* lighting: a directional and a point light on a grid of normals, material and vertex colour sources */
  reset_state(); begin(0xFF000000);
  IDirect3DDevice8_SetTransform(d, D3DTS_PROJECTION, &proj); IDirect3DDevice8_SetTransform(d, D3DTS_VIEW, &view);
  IDirect3DDevice8_SetRenderState(d, D3DRS_LIGHTING, TRUE); IDirect3DDevice8_SetRenderState(d, D3DRS_AMBIENT, 0xFF202020);
  { D3DMATERIAL8 m; memset(&m, 0, sizeof m); m.Diffuse.r = 1; m.Diffuse.g = 0.8f; m.Diffuse.b = 0.6f; m.Diffuse.a = 1;
    m.Ambient.r = m.Ambient.g = m.Ambient.b = 1; m.Specular.r = m.Specular.g = m.Specular.b = 1; m.Power = 8;
    IDirect3DDevice8_SetMaterial(d, &m);
    D3DLIGHT8 l; memset(&l, 0, sizeof l); l.Type = D3DLIGHT_DIRECTIONAL; l.Diffuse.r = 0.7f; l.Diffuse.g = 0.7f; l.Diffuse.b = 0.9f;
    l.Direction.x = 0.5f; l.Direction.y = -0.5f; l.Direction.z = 1;
    IDirect3DDevice8_SetLight(d, 0, &l); IDirect3DDevice8_LightEnable(d, 0, TRUE);
    memset(&l, 0, sizeof l); l.Type = D3DLIGHT_POINT; l.Diffuse.r = 1; l.Diffuse.g = 0.3f; l.Specular.r = l.Specular.g = l.Specular.b = 1;
    l.Position.x = -1.5f; l.Position.y = 1; l.Position.z = -1; l.Range = 10; l.Attenuation0 = 0.2f; l.Attenuation1 = 0.3f;
    IDirect3DDevice8_SetLight(d, 1, &l); IDirect3DDevice8_LightEnable(d, 1, TRUE); }
  IDirect3DDevice8_SetVertexShader(d, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);
  { VXyzN g[8 * 8]; WORD ix[7 * 7 * 6]; int n = 0;
    for (int y = 0; y < 8; y++) for (int x = 0; x < 8; x++) {
      float fx = x / 7.0f * 2 - 1, fy = y / 7.0f * 2 - 1, nz = -(float)sqrt(fmax(0.0, 1.0 - 0.5 * (fx * fx + fy * fy)));
      VXyzN v = {fx * 2.2f, -fy * 2.2f, 0, fx * 0.7f, -fy * 0.7f, nz, 0xFF40FF40, 0, 0}; g[y * 8 + x] = v; }
    for (int y = 0; y < 7; y++) for (int x = 0; x < 7; x++) {
      WORD a = (WORD)(y * 8 + x); ix[n++] = a; ix[n++] = a + 1; ix[n++] = a + 8; ix[n++] = a + 1; ix[n++] = a + 9; ix[n++] = a + 8; }
    IDirect3DDevice8_DrawIndexedPrimitiveUP(d, D3DPT_TRIANGLELIST, 0, 64, 7 * 7 * 2, ix, D3DFMT_INDEX16, g, sizeof *g);
    /* again with the diffuse colour from the vertex, and specular on, offset right */
    end("lit_material");
    reset_state(); begin(0xFF000000);
    IDirect3DDevice8_SetTransform(d, D3DTS_PROJECTION, &proj); IDirect3DDevice8_SetTransform(d, D3DTS_VIEW, &view);
    IDirect3DDevice8_SetRenderState(d, D3DRS_LIGHTING, TRUE); IDirect3DDevice8_SetRenderState(d, D3DRS_AMBIENT, 0xFF202020);
    IDirect3DDevice8_SetRenderState(d, D3DRS_COLORVERTEX, TRUE);
    IDirect3DDevice8_SetRenderState(d, D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
    IDirect3DDevice8_SetRenderState(d, D3DRS_SPECULARENABLE, TRUE);
    IDirect3DDevice8_SetVertexShader(d, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    IDirect3DDevice8_DrawIndexedPrimitiveUP(d, D3DPT_TRIANGLELIST, 0, 64, 7 * 7 * 2, ix, D3DFMT_INDEX16, g, sizeof *g); }
  end("lit_vertexcolor_specular");
  IDirect3DDevice8_LightEnable(d, 0, FALSE); IDirect3DDevice8_LightEnable(d, 1, FALSE);
  IDirect3DDevice8_SetRenderState(d, D3DRS_COLORVERTEX, TRUE); IDirect3DDevice8_SetRenderState(d, D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

  /* two texture stages: texture * diffuse, then + texture 2 by its alpha; TFACTOR; alpha blend and alpha test */
  reset_state(); begin(0xFF303030);
  IDirect3DDevice8_SetTexture(d, 0, (IDirect3DBaseTexture8 *)grad); IDirect3DDevice8_SetTexture(d, 1, (IDirect3DBaseTexture8 *)alp);
  IDirect3DDevice8_SetTextureStageState(d, 1, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);
  IDirect3DDevice8_SetTextureStageState(d, 1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
  IDirect3DDevice8_SetTextureStageState(d, 1, D3DTSS_TEXCOORDINDEX, 0);
  { VRhwT v[4] = {{2, 2, 0.5f, 1, 0xFFFFFFFF, 0, 0}, {30, 2, 0.5f, 1, 0xFFFFFFFF, 1, 0}, {2, 30, 0.5f, 1, 0xFF8080FF, 0, 1}, {30, 30, 0.5f, 1, 0xFF8080FF, 1, 1}};
    IDirect3DDevice8_SetVertexShader(d, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLESTRIP, 2, v, sizeof *v); }
  IDirect3DDevice8_SetTextureStageState(d, 1, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
  IDirect3DDevice8_SetTextureStageState(d, 1, D3DTSS_COLORARG1, D3DTA_TFACTOR);
  IDirect3DDevice8_SetRenderState(d, D3DRS_TEXTUREFACTOR, 0xFF60A040);
  { VRhwT v[4] = {{34, 2, 0.5f, 1, 0xFFFFFFFF, 0, 0}, {62, 2, 0.5f, 1, 0xFFFFFFFF, 1, 0}, {34, 30, 0.5f, 1, 0xFFFFFFFF, 0, 1}, {62, 30, 0.5f, 1, 0xFFFFFFFF, 1, 1}};
    IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLESTRIP, 2, v, sizeof *v); }
  IDirect3DDevice8_SetTexture(d, 1, NULL); IDirect3DDevice8_SetTextureStageState(d, 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
  IDirect3DDevice8_SetTexture(d, 0, (IDirect3DBaseTexture8 *)alp);
  IDirect3DDevice8_SetRenderState(d, D3DRS_ALPHABLENDENABLE, TRUE);
  IDirect3DDevice8_SetRenderState(d, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); IDirect3DDevice8_SetRenderState(d, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  { VRhwT v[4] = {{2, 34, 0.5f, 1, 0xFFFFFFFF, 0, 0}, {30, 34, 0.5f, 1, 0xFFFFFFFF, 1, 0}, {2, 62, 0.5f, 1, 0xFFFFFFFF, 0, 1}, {30, 62, 0.5f, 1, 0xFFFFFFFF, 1, 1}};
    IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLESTRIP, 2, v, sizeof *v); }
  IDirect3DDevice8_SetRenderState(d, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); IDirect3DDevice8_SetRenderState(d, D3DRS_DESTBLEND, D3DBLEND_ONE);
  IDirect3DDevice8_SetRenderState(d, D3DRS_ALPHATESTENABLE, TRUE);
  IDirect3DDevice8_SetRenderState(d, D3DRS_ALPHAREF, 0x80); IDirect3DDevice8_SetRenderState(d, D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
  { VRhwT v[4] = {{34, 34, 0.5f, 1, 0xFFFFFFFF, 0, 0}, {62, 34, 0.5f, 1, 0xFFFFFFFF, 1, 0}, {34, 62, 0.5f, 1, 0xFFFFFFFF, 0, 1}, {62, 62, 0.5f, 1, 0xFFFFFFFF, 1, 1}};
    IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLESTRIP, 2, v, sizeof *v); }
  end("stages_blend_alphatest");

  /* fog: vertex linear fog, then table (pixel) linear and exp fog, on a floor going into the distance */
  { static const DWORD tm[3][2] = {{D3DFOG_NONE, D3DFOG_LINEAR}, {D3DFOG_LINEAR, D3DFOG_NONE}, {D3DFOG_EXP, D3DFOG_NONE}};
    static const char *names[3] = {"fog_vertex_linear", "fog_table_linear", "fog_table_exp"};
    for (int k = 0; k < 3; k++) {
      reset_state(); begin(0xFF000000);
      IDirect3DDevice8_SetTransform(d, D3DTS_PROJECTION, &proj); IDirect3DDevice8_SetTransform(d, D3DTS_VIEW, &view);
      IDirect3DDevice8_SetRenderState(d, D3DRS_FOGENABLE, TRUE); IDirect3DDevice8_SetRenderState(d, D3DRS_FOGCOLOR, 0xFF8090A0);
      float fs = 5, fe = 30, fd = 0.08f;
      IDirect3DDevice8_SetRenderState(d, D3DRS_FOGSTART, *(DWORD *)&fs); IDirect3DDevice8_SetRenderState(d, D3DRS_FOGEND, *(DWORD *)&fe);
      IDirect3DDevice8_SetRenderState(d, D3DRS_FOGDENSITY, *(DWORD *)&fd);
      IDirect3DDevice8_SetRenderState(d, D3DRS_FOGTABLEMODE, tm[k][0]); IDirect3DDevice8_SetRenderState(d, D3DRS_FOGVERTEXMODE, tm[k][1]);
      IDirect3DDevice8_SetTexture(d, 0, (IDirect3DBaseTexture8 *)chk);
      IDirect3DDevice8_SetVertexShader(d, D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
      VXyzT v[4] = {{-6, -1.5f, -2, 0xFFFF4040, 0, 0}, {6, -1.5f, -2, 0xFFFF4040, 8, 0}, {-6, -1.5f, 40, 0xFFFF4040, 0, 8}, {6, -1.5f, 40, 0xFFFF4040, 8, 8}};
      IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLESTRIP, 2, v, sizeof *v);
      end(names[k]);
    } }

  /* the primitives from buffers: a vertex buffer and an index buffer with a base vertex; a strip, a fan, a list */
  reset_state(); begin(0xFF000000);
  { IDirect3DVertexBuffer8 *vb = NULL; IDirect3DIndexBuffer8 *ib = NULL; BYTE *p;
    IDirect3DDevice8_CreateVertexBuffer(d, 32 * sizeof(VRhw), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_MANAGED, &vb);
    IDirect3DDevice8_CreateIndexBuffer(d, 32 * 2, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &ib);
    IDirect3DVertexBuffer8_Lock(vb, 0, 0, &p, 0);
    VRhw *v = (VRhw *)p;
    VRhw strip[4] = {{4, 4, 0.5f, 1, 0xFFFF0000}, {28, 4, 0.5f, 1, 0xFF00FF00}, {4, 28, 0.5f, 1, 0xFF0000FF}, {28, 28, 0.5f, 1, 0xFFFFFF00}};
    VRhw fan[6] = {{46, 16, 0.5f, 1, 0xFFFFFFFF}, {46, 2, 0.5f, 1, 0xFFFF0000}, {60, 10, 0.5f, 1, 0xFF00FF00}, {60, 24, 0.5f, 1, 0xFF0000FF},
                   {46, 30, 0.5f, 1, 0xFFFF00FF}, {32, 22, 0.5f, 1, 0xFF00FFFF}};
    VRhw list[6] = {{4, 36, 0.5f, 1, 0xFFFF8000}, {28, 36, 0.5f, 1, 0xFFFF8000}, {4, 60, 0.5f, 1, 0xFF0080FF},
                    {36, 36, 0.5f, 1, 0xFF80FF80}, {60, 60, 0.5f, 1, 0xFF80FF80}, {36, 60, 0.5f, 1, 0xFF808080}};
    memcpy(v, strip, sizeof strip); memcpy(v + 4, fan, sizeof fan); memcpy(v + 10, list, sizeof list);
    IDirect3DVertexBuffer8_Unlock(vb);
    IDirect3DIndexBuffer8_Lock(ib, 0, 0, &p, 0);
    WORD ix[6] = {0, 1, 2, 3, 4, 5}; memcpy(p, ix, sizeof ix);
    IDirect3DIndexBuffer8_Unlock(ib);
    IDirect3DDevice8_SetVertexShader(d, D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    IDirect3DDevice8_SetStreamSource(d, 0, vb, sizeof(VRhw));
    IDirect3DDevice8_DrawPrimitive(d, D3DPT_TRIANGLESTRIP, 0, 2);
    IDirect3DDevice8_DrawPrimitive(d, D3DPT_TRIANGLEFAN, 4, 4);
    IDirect3DDevice8_SetIndices(d, ib, 10);                                 /* base vertex 10: the list */
    IDirect3DDevice8_DrawIndexedPrimitive(d, D3DPT_TRIANGLELIST, 0, 6, 0, 2);
    IDirect3DDevice8_SetStreamSource(d, 0, NULL, 0); IDirect3DDevice8_SetIndices(d, NULL, 0);
    IDirect3DVertexBuffer8_Release(vb); IDirect3DIndexBuffer8_Release(ib); }
  end("buffers_strip_fan_indexed");

  /* a declaration (no shader code), as the game makes its vertex shaders; and a fan through ...UP */
  reset_state(); begin(0xFF000000);
  { static const DWORD decl[] = {D3DVSD_STREAM(0), D3DVSD_REG(0, D3DVSDT_FLOAT4), D3DVSD_REG(5, D3DVSDT_D3DCOLOR), D3DVSD_END()};
    DWORD h = 0;
    IDirect3DDevice8_CreateVertexShader(d, decl, NULL, &h, 0);
    IDirect3DDevice8_SetTransform(d, D3DTS_PROJECTION, &proj); IDirect3DDevice8_SetTransform(d, D3DTS_VIEW, &view);
    IDirect3DDevice8_SetVertexShader(d, h);
    struct { float x, y, z, w; DWORD c; } v[5] = {{0, 0, 0, 1, 0xFFFFFFFF}, {0, 2, 0, 1, 0xFFFF0000}, {2, 0, 0, 1, 0xFF00FF00},
                                                   {0, -2, 0, 1, 0xFF0000FF}, {-2, 0, 0, 1, 0xFFFFFF00}};
    IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLEFAN, 3, v, sizeof v[0]);
    IDirect3DDevice8_SetVertexShader(d, D3DFVF_XYZ | D3DFVF_DIFFUSE);
    IDirect3DDevice8_DeleteVertexShader(d, h); }
  end("declaration_fan");

  /* texture coordinates generated from the camera-space position, through a texture transform */
  reset_state(); begin(0xFF000000);
  IDirect3DDevice8_SetTransform(d, D3DTS_PROJECTION, &proj); IDirect3DDevice8_SetTransform(d, D3DTS_VIEW, &view);
  IDirect3DDevice8_SetTexture(d, 0, (IDirect3DBaseTexture8 *)chk);
  IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
  IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
  { D3DMATRIX t; memset(&t, 0, sizeof t); t._11 = 0.5f; t._22 = -0.5f; t._33 = 1; t._44 = 1; t._41 = 0.25f; t._42 = 0.125f;
    IDirect3DDevice8_SetTransform(d, D3DTS_TEXTURE0, &t);
    VXyz v[4] = {{-2, 2, 0, 0xFFFFFFFF}, {2, 2, 0, 0xFFFFFFFF}, {-2, -2, 0, 0xFFFFFFFF}, {2, -2, 0, 0xFFFFFFFF}};
    IDirect3DDevice8_SetVertexShader(d, D3DFVF_XYZ | D3DFVF_DIFFUSE);
    IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLESTRIP, 2, v, sizeof *v); }
  end("texgen_cameraspace_transform");

  /* a 2D texture transform on the vertex's own coordinates (translation in the third row) */
  reset_state(); begin(0xFF000000);
  IDirect3DDevice8_SetTexture(d, 0, (IDirect3DBaseTexture8 *)chk);
  IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
  { D3DMATRIX t; memset(&t, 0, sizeof t); t._11 = 2; t._22 = 1; t._33 = 1; t._44 = 1; t._31 = 0.25f; t._32 = 0.5f;
    IDirect3DDevice8_SetTransform(d, D3DTS_TEXTURE0, &t);
    VRhwT v[4] = {{4, 4, 0.5f, 1, 0xFFFFFFFF, 0, 0}, {60, 4, 0.5f, 1, 0xFFFFFFFF, 1, 0}, {4, 60, 0.5f, 1, 0xFFFFFFFF, 0, 1}, {60, 60, 0.5f, 1, 0xFFFFFFFF, 1, 1}};
    IDirect3DDevice8_SetVertexShader(d, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLESTRIP, 2, v, sizeof *v); }
  end("texture_transform_2d");

  /* flat shading on a list, a strip and a fan (the colour of each triangle's first vertex) */
  reset_state(); begin(0xFF000000);
  IDirect3DDevice8_SetRenderState(d, D3DRS_SHADEMODE, D3DSHADE_FLAT);
  quad_rhw(4, 4, 30, 30, 0.5f, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFFFFFFFF);
  { VRhw l[6] = {{34, 4, 0.5f, 1, 0xFFFF8000}, {60, 4, 0.5f, 1, 0xFF00FF00}, {34, 30, 0.5f, 1, 0xFF0000FF},
                 {60, 8, 0.5f, 1, 0xFF00FFFF}, {60, 30, 0.5f, 1, 0xFFFF00FF}, {38, 30, 0.5f, 1, 0xFFFFFF00}};
    IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLELIST, 2, l, sizeof *l);
    VRhw f[5] = {{16, 46, 0.5f, 1, 0xFFFFFFFF}, {16, 34, 0.5f, 1, 0xFFFF0000}, {28, 46, 0.5f, 1, 0xFF00FF00},
                 {16, 60, 0.5f, 1, 0xFF0000FF}, {4, 46, 0.5f, 1, 0xFFFFFF00}};
    IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLEFAN, 3, f, sizeof *f); }
  IDirect3DDevice8_SetRenderState(d, D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
  end("flat_shading");

  /* ZBIAS: coplanar quads, the later one with ZBIAS 8 (as the game sets it) and ZFUNC LESS */
  reset_state(); begin(0xFF000000);
  IDirect3DDevice8_SetTransform(d, D3DTS_PROJECTION, &proj); IDirect3DDevice8_SetTransform(d, D3DTS_VIEW, &view);
  IDirect3DDevice8_SetRenderState(d, D3DRS_ZENABLE, D3DZB_TRUE); IDirect3DDevice8_SetRenderState(d, D3DRS_ZFUNC, D3DCMP_LESS);
  IDirect3DDevice8_SetVertexShader(d, D3DFVF_XYZ | D3DFVF_DIFFUSE);
  { VXyz a[4] = {{-2, 2, 3, 0xFF0000FF}, {2, 2, -1, 0xFF0000FF}, {-2, -2, 3, 0xFF0000FF}, {2, -2, -1, 0xFF0000FF}};
    VXyz b[4] = {{-2, 2, 3, 0xFFFFFF00}, {2, 2, -1, 0xFFFFFF00}, {-2, -2, 3, 0xFFFFFF00}, {2, -2, -1, 0xFFFFFF00}};
    IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLESTRIP, 2, a, sizeof *a);
    IDirect3DDevice8_SetRenderState(d, D3DRS_ZBIAS, 8);
    IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLESTRIP, 2, b, sizeof *b);
    IDirect3DDevice8_SetRenderState(d, D3DRS_ZBIAS, 0); }
  end("zbias");

  /* the blend modes the game sets: SRCBLEND 5/1/2 with DESTBLEND 2/6/4, BLENDOP ADD and REVSUBTRACT */
  reset_state(); begin(0xFF406080);
  IDirect3DDevice8_SetRenderState(d, D3DRS_ALPHABLENDENABLE, TRUE);
  { static const DWORD m[6][3] = {{D3DBLEND_SRCALPHA, D3DBLEND_ONE, D3DBLENDOP_ADD}, {D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA, D3DBLENDOP_ADD},
                                  {D3DBLEND_ZERO, D3DBLEND_SRCCOLOR, D3DBLENDOP_ADD}, {D3DBLEND_ONE, D3DBLEND_ONE, D3DBLENDOP_REVSUBTRACT},
                                  {D3DBLEND_SRCALPHA, D3DBLEND_ONE, D3DBLENDOP_REVSUBTRACT}, {D3DBLEND_ONE, D3DBLEND_ZERO, D3DBLENDOP_ADD}};
    for (int i = 0; i < 6; i++) {
      IDirect3DDevice8_SetRenderState(d, D3DRS_SRCBLEND, m[i][0]); IDirect3DDevice8_SetRenderState(d, D3DRS_DESTBLEND, m[i][1]);
      IDirect3DDevice8_SetRenderState(d, D3DRS_BLENDOP, m[i][2]);
      float x = 2.0f + (i % 3) * 20, y = 2.0f + (i / 3) * 30;
      quad_rhw(x, y, x + 18, y + 28, 0.5f, 0x80FF8040, 0x20FF8040, 0xC040A0FF, 0xFF40A0FF);
    }
    IDirect3DDevice8_SetRenderState(d, D3DRS_BLENDOP, D3DBLENDOP_ADD); }
  end("blend_modes");

  /* stage 1 from a cube texture through the camera-space reflection vector, COUNT3 (as the game sets stage 1) */
  reset_state(); begin(0xFF000000);
  { IDirect3DCubeTexture8 *cube = NULL;
    IDirect3DDevice8_CreateCubeTexture(d, 8, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &cube);
    static const DWORD fc[6] = {0xFFFF0000, 0xFF00FFFF, 0xFF00FF00, 0xFFFF00FF, 0xFF0000FF, 0xFFFFFF00};
    for (int f = 0; cube && f < 6; f++) { D3DLOCKED_RECT lr;
      IDirect3DCubeTexture8_LockRect(cube, (D3DCUBEMAP_FACES)f, 0, &lr, NULL, 0);
      for (int y = 0; y < 8; y++) for (int x = 0; x < 8; x++) ((DWORD *)((BYTE *)lr.pBits + y * lr.Pitch))[x] = ((x ^ y) & 2) ? fc[f] : (fc[f] & 0xFF7F7F7F);
      IDirect3DCubeTexture8_UnlockRect(cube, (D3DCUBEMAP_FACES)f, 0); }
    IDirect3DDevice8_SetTransform(d, D3DTS_PROJECTION, &proj); IDirect3DDevice8_SetTransform(d, D3DTS_VIEW, &view);
    IDirect3DDevice8_SetTexture(d, 0, (IDirect3DBaseTexture8 *)grad);
    IDirect3DDevice8_SetTexture(d, 1, (IDirect3DBaseTexture8 *)cube);
    IDirect3DDevice8_SetTextureStageState(d, 1, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
    IDirect3DDevice8_SetTextureStageState(d, 1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
    IDirect3DDevice8_SetTextureStageState(d, 1, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR | 1);
    IDirect3DDevice8_SetTextureStageState(d, 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT3);
    D3DMATRIX r; memset(&r, 0, sizeof r); r._11 = (float)cos(0.6); r._13 = (float)-sin(0.6); r._22 = 1; r._31 = (float)sin(0.6); r._33 = (float)cos(0.6); r._44 = 1;
    IDirect3DDevice8_SetTransform(d, D3DTS_TEXTURE1, &r);
    IDirect3DDevice8_SetVertexShader(d, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    VXyzN g[8 * 8]; WORD ix[7 * 7 * 6]; int n = 0;
    for (int y = 0; y < 8; y++) for (int x = 0; x < 8; x++) {
      float fx = x / 7.0f * 2 - 1, fy = y / 7.0f * 2 - 1, nz = -(float)sqrt(fmax(0.0, 1.0 - 0.5 * (fx * fx + fy * fy)));
      VXyzN v = {fx * 2.2f, -fy * 2.2f, 0, fx * 0.7f, -fy * 0.7f, nz, 0xFFFFFFFF, x / 7.0f, y / 7.0f}; g[y * 8 + x] = v; }
    for (int y = 0; y < 7; y++) for (int x = 0; x < 7; x++) {
      WORD a = (WORD)(y * 8 + x); ix[n++] = a; ix[n++] = a + 1; ix[n++] = a + 8; ix[n++] = a + 1; ix[n++] = a + 9; ix[n++] = a + 8; }
    IDirect3DDevice8_DrawIndexedPrimitiveUP(d, D3DPT_TRIANGLELIST, 0, 64, 7 * 7 * 2, ix, D3DFMT_INDEX16, g, sizeof *g);
    /* the camera-space normal as well, at the right with 0x10001 (seen in the game's traces) */
    IDirect3DDevice8_SetTextureStageState(d, 1, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL | 1);
    D3DMATRIX w; memset(&w, 0, sizeof w); w._11 = w._22 = w._33 = 0.4f; w._44 = 1; w._41 = 1.4f; w._42 = -1.4f;
    IDirect3DDevice8_SetTransform(d, D3DTS_WORLD, &w);
    IDirect3DDevice8_DrawIndexedPrimitiveUP(d, D3DPT_TRIANGLELIST, 0, 64, 7 * 7 * 2, ix, D3DFMT_INDEX16, g, sizeof *g);
    IDirect3DDevice8_SetTexture(d, 1, NULL);
    if (cube) IDirect3DCubeTexture8_Release(cube); }
  end("texgen_reflection_cube");

  /* a DXT1 texture (the game's textures are block compressed), linear filtered, with a mip chain */
  reset_state(); begin(0xFF000000);
  { IDirect3DTexture8 *t = NULL;
    IDirect3DDevice8_CreateTexture(d, 16, 16, 0, 0, D3DFMT_DXT1, D3DPOOL_MANAGED, &t);
    for (UINT l = 0; t && l < IDirect3DTexture8_GetLevelCount(t); l++) { D3DLOCKED_RECT lr; D3DSURFACE_DESC sd;
      IDirect3DTexture8_GetLevelDesc(t, l, &sd); IDirect3DTexture8_LockRect(t, l, &lr, NULL, 0);
      UINT bw = (sd.Width + 3) / 4, bh = (sd.Height + 3) / 4;
      for (UINT y = 0; y < bh; y++) for (UINT x = 0; x < bw; x++) {
        WORD *b = (WORD *)((BYTE *)lr.pBits + y * lr.Pitch + x * 8);
        b[0] = (WORD)(0xF800 >> (l * 5 % 11)) | 0x001F; b[1] = (WORD)(0x07E0 + x * 0x0841 + y * 0x1000);
        b[2] = 0x1B4E; b[3] = 0xE4B1; }
      IDirect3DTexture8_UnlockRect(t, l); }
    IDirect3DDevice8_SetTexture(d, 0, (IDirect3DBaseTexture8 *)t);
    IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR); IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
    IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_MIPFILTER, D3DTEXF_POINT);
    IDirect3DDevice8_SetVertexShader(d, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    { VRhwT v[4] = {{2, 2, 0.5f, 1, 0xFFFFFFFF, 0, 0}, {34, 2, 0.5f, 1, 0xFFFFFFFF, 1, 0}, {2, 34, 0.5f, 1, 0xFFFFFFFF, 0, 1}, {34, 34, 0.5f, 1, 0xFFFFFFFF, 1, 1}};
      IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLESTRIP, 2, v, sizeof *v); }
    { VRhwT v[4] = {{38, 38, 0.5f, 1, 0xFFFFFFFF, 0, 0}, {46, 38, 0.5f, 1, 0xFFFFFFFF, 1, 0}, {38, 46, 0.5f, 1, 0xFFFFFFFF, 0, 1}, {46, 46, 0.5f, 1, 0xFFFFFFFF, 1, 1}};
      IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLESTRIP, 2, v, sizeof *v); }
    /* FVF 0x104 (XYZRHW|TEX1, no colour: the diffuse reads as white) */
    { struct { float x, y, z, rhw, u, v; } q[4] = {{2, 38, 0.5f, 1, 0, 0}, {34, 38, 0.5f, 1, 1, 0}, {2, 62, 0.5f, 1, 0, 1}, {34, 62, 0.5f, 1, 1, 1}};
      IDirect3DDevice8_SetVertexShader(d, D3DFVF_XYZRHW | D3DFVF_TEX1);
      IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLESTRIP, 2, q, sizeof q[0]); }
    if (t) IDirect3DTexture8_Release(t); }
  end("dxt1_mips_fvf104");

  /* eight lights at once: point and spot lights around a grid, normals renormalized, ambient from the material */
  reset_state(); begin(0xFF000000);
  IDirect3DDevice8_SetTransform(d, D3DTS_PROJECTION, &proj); IDirect3DDevice8_SetTransform(d, D3DTS_VIEW, &view);
  IDirect3DDevice8_SetRenderState(d, D3DRS_LIGHTING, TRUE); IDirect3DDevice8_SetRenderState(d, D3DRS_AMBIENT, 0xFF42404C);
  IDirect3DDevice8_SetRenderState(d, D3DRS_COLORVERTEX, FALSE); IDirect3DDevice8_SetRenderState(d, D3DRS_NORMALIZENORMALS, TRUE);
  { D3DMATERIAL8 m; memset(&m, 0, sizeof m); m.Diffuse.r = m.Diffuse.g = m.Diffuse.b = m.Diffuse.a = 1; m.Ambient = m.Diffuse;
    IDirect3DDevice8_SetMaterial(d, &m);
    for (int i = 0; i < 8; i++) {
      D3DLIGHT8 l; memset(&l, 0, sizeof l); float a = i * 0.785f;
      l.Type = i % 3 == 2 ? D3DLIGHT_SPOT : D3DLIGHT_POINT;
      l.Diffuse.r = (i & 1) ? 0.6f : 0.1f; l.Diffuse.g = (i & 2) ? 0.6f : 0.1f; l.Diffuse.b = (i & 4) ? 0.6f : 0.2f;
      l.Ambient.r = l.Ambient.g = l.Ambient.b = 0.02f;
      l.Position.x = 2.2f * (float)cos(a); l.Position.y = 2.2f * (float)sin(a); l.Position.z = -1.2f;
      l.Direction.x = -l.Position.x; l.Direction.y = -l.Position.y; l.Direction.z = 1.5f;
      l.Range = 4; l.Attenuation0 = 0.5f; l.Attenuation1 = 0.4f; l.Falloff = 1; l.Theta = 0.4f; l.Phi = 1.0f;
      IDirect3DDevice8_SetLight(d, i, &l); IDirect3DDevice8_LightEnable(d, i, TRUE); }
    IDirect3DDevice8_SetVertexShader(d, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    VXyzN g[8 * 8]; WORD ix[7 * 7 * 6]; int n = 0;
    for (int y = 0; y < 8; y++) for (int x = 0; x < 8; x++) {
      float fx = x / 7.0f * 2 - 1, fy = y / 7.0f * 2 - 1;
      VXyzN v = {fx * 2.4f, -fy * 2.4f, 0, fx * 3, -fy * 3, -4, 0xFFFFFFFF, 0, 0}; g[y * 8 + x] = v; }
    for (int y = 0; y < 7; y++) for (int x = 0; x < 7; x++) {
      WORD a = (WORD)(y * 8 + x); ix[n++] = a; ix[n++] = a + 1; ix[n++] = a + 8; ix[n++] = a + 1; ix[n++] = a + 9; ix[n++] = a + 8; }
    IDirect3DDevice8_DrawIndexedPrimitiveUP(d, D3DPT_TRIANGLELIST, 0, 64, 7 * 7 * 2, ix, D3DFMT_INDEX16, g, sizeof *g);
    for (int i = 0; i < 8; i++) IDirect3DDevice8_LightEnable(d, i, FALSE); }
  end("lights_8");

  /* the game's character layout (run 2's captured declaration, handle 3): position, normal and texcoord each from
     its own stream (SkinAndDrawV2's streams 0, 1, 2, strides 12, 12, 8), indexed, lit, textured */
  reset_state(); begin(0xFF000000);
  { static const DWORD decl[] = {D3DVSD_STREAM(0), D3DVSD_REG(0, D3DVSDT_FLOAT3), D3DVSD_STREAM(1), D3DVSD_REG(3, D3DVSDT_FLOAT3),
                                 D3DVSD_STREAM(2), D3DVSD_REG(7, D3DVSDT_FLOAT2), D3DVSD_END()};
    DWORD h = 0; IDirect3DVertexBuffer8 *vp = NULL, *vn = NULL, *vt = NULL; IDirect3DIndexBuffer8 *ib = NULL; BYTE *p;
    IDirect3DDevice8_CreateVertexShader(d, decl, NULL, &h, 0);
    IDirect3DDevice8_CreateVertexBuffer(d, 64 * 12, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &vp);
    IDirect3DDevice8_CreateVertexBuffer(d, 64 * 12, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &vn);
    IDirect3DDevice8_CreateVertexBuffer(d, 64 * 8, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &vt);
    IDirect3DDevice8_CreateIndexBuffer(d, 7 * 7 * 6 * 2, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &ib);
    float *P, *N, *T; WORD *I;
    IDirect3DVertexBuffer8_Lock(vp, 0, 0, &p, 0); P = (float *)p;
    IDirect3DVertexBuffer8_Lock(vn, 0, 0, &p, 0); N = (float *)p;
    IDirect3DVertexBuffer8_Lock(vt, 0, 0, &p, 0); T = (float *)p;
    for (int y = 0; y < 8; y++) for (int x = 0; x < 8; x++) {
      int k = y * 8 + x; float fx = x / 7.0f * 2 - 1, fy = y / 7.0f * 2 - 1;
      P[3 * k] = fx * 2.2f; P[3 * k + 1] = -fy * 2.2f; P[3 * k + 2] = 0.3f * fx * fx;
      N[3 * k] = fx * 0.6f; N[3 * k + 1] = -fy * 0.6f; N[3 * k + 2] = -1;
      /* 1.9, not 2: at exactly u = 1.0 between vertices point sampling ties between two texels, and Wine (which nudges
         GL geometry for D3D's fill convention, wined3d get_projection_matrix) and D3D11 break the tie differently;
         which way Windows' D3D8 does is not established (docs/HISTORY.md, 14 Oct corrections) */
      T[2 * k] = x / 7.0f * 1.9f; T[2 * k + 1] = y / 7.0f * 1.9f; }
    IDirect3DVertexBuffer8_Unlock(vp); IDirect3DVertexBuffer8_Unlock(vn); IDirect3DVertexBuffer8_Unlock(vt);
    IDirect3DIndexBuffer8_Lock(ib, 0, 0, &p, 0); I = (WORD *)p; int n = 0;
    for (int y = 0; y < 7; y++) for (int x = 0; x < 7; x++) {
      WORD a = (WORD)(y * 8 + x); I[n++] = a; I[n++] = a + 1; I[n++] = a + 8; I[n++] = a + 1; I[n++] = a + 9; I[n++] = a + 8; }
    IDirect3DIndexBuffer8_Unlock(ib);
    IDirect3DDevice8_SetTransform(d, D3DTS_PROJECTION, &proj); IDirect3DDevice8_SetTransform(d, D3DTS_VIEW, &view);
    IDirect3DDevice8_SetRenderState(d, D3DRS_LIGHTING, TRUE); IDirect3DDevice8_SetRenderState(d, D3DRS_AMBIENT, 0xFF42404C);
    IDirect3DDevice8_SetRenderState(d, D3DRS_ZENABLE, D3DZB_TRUE);
    D3DMATERIAL8 m; memset(&m, 0, sizeof m); m.Diffuse.r = m.Diffuse.g = m.Diffuse.b = m.Diffuse.a = 1; m.Ambient = m.Diffuse;
    IDirect3DDevice8_SetMaterial(d, &m);
    D3DLIGHT8 l; memset(&l, 0, sizeof l); l.Type = D3DLIGHT_DIRECTIONAL; l.Diffuse.r = l.Diffuse.g = l.Diffuse.b = 0.8f;
    l.Direction.x = 0.4f; l.Direction.y = -0.6f; l.Direction.z = 1;
    IDirect3DDevice8_SetLight(d, 0, &l); IDirect3DDevice8_LightEnable(d, 0, TRUE);
    IDirect3DDevice8_SetTexture(d, 0, (IDirect3DBaseTexture8 *)grad);
    IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
    IDirect3DDevice8_SetVertexShader(d, h);
    IDirect3DDevice8_SetStreamSource(d, 0, vp, 12); IDirect3DDevice8_SetStreamSource(d, 1, vn, 12); IDirect3DDevice8_SetStreamSource(d, 2, vt, 8);
    IDirect3DDevice8_SetIndices(d, ib, 0);
    IDirect3DDevice8_DrawIndexedPrimitive(d, D3DPT_TRIANGLELIST, 0, 64, 0, 7 * 7 * 2);
    IDirect3DDevice8_LightEnable(d, 0, FALSE);
    for (int i = 0; i < 3; i++) IDirect3DDevice8_SetStreamSource(d, i, NULL, 0);
    IDirect3DDevice8_SetIndices(d, NULL, 0); IDirect3DDevice8_SetVertexShader(d, D3DFVF_XYZ | D3DFVF_DIFFUSE);
    IDirect3DDevice8_DeleteVertexShader(d, h);
    IDirect3DVertexBuffer8_Release(vp); IDirect3DVertexBuffer8_Release(vn); IDirect3DVertexBuffer8_Release(vt); IDirect3DIndexBuffer8_Release(ib); }
  end("three_stream_declaration");

  if (chk) IDirect3DTexture8_Release(chk); if (grad) IDirect3DTexture8_Release(grad); if (alp) IDirect3DTexture8_Release(alp);
}

/* ---- random cases (random_cases.py writes them) -------------------------------------------------------------------
   Lines: "case NAME FVF PRIM KIND" (FVF hex; PRIM 4 list, 5 strip; KIND 0 UP, 1 vertex buffer, 2 indexed), then
   "rs S V", "tss STAGE T V", "tex STAGE K" (0 none, 1 checker, 2 gradient, 3 alpha ramp, 4 cube, 5 cube of one
   colour, 6 cube with one colour per face), "light I ON",
   "tt STAGE m11 m12 m21 m22 m31 m32" (a texture transform), "mat R G B A", then "draw". Values hex, floats decimal. */
static IDirect3DCubeTexture8 *g_cube, *g_cube_flat, *g_cube_faces;
static IDirect3DCubeTexture8 *cube_of(DWORD (*f)(int fc, int x, int y)) {
  IDirect3DCubeTexture8 *c = NULL;
  if (FAILED(IDirect3DDevice8_CreateCubeTexture(d, 4, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &c))) return NULL;
  for (int fc = 0; fc < 6; fc++) { D3DLOCKED_RECT lr; IDirect3DCubeTexture8_LockRect(c, (D3DCUBEMAP_FACES)fc, 0, &lr, NULL, 0);
    for (int y = 0; y < 4; y++) for (int x = 0; x < 4; x++) ((DWORD *)((BYTE *)lr.pBits + y * lr.Pitch))[x] = f(fc, x, y);
    IDirect3DCubeTexture8_UnlockRect(c, (D3DCUBEMAP_FACES)fc, 0); }
  return c;
}
static DWORD cube_flat(int fc, int x, int y) { (void)fc; (void)x; (void)y; return 0xFF4080C0; }
static DWORD cube_faces(int fc, int x, int y) { static const DWORD c[6] = {0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFFFFFF00, 0xFF00FFFF, 0xFFFF00FF}; (void)x; (void)y; return c[fc]; }
static float frand(unsigned *s) { *s = *s * 1103515245u + 12345u; return (float)((*s >> 8) & 0xFFFF) / 65535.0f; }
static int vsize_of(DWORD fvf) {
  int n = (fvf & D3DFVF_POSITION_MASK) == D3DFVF_XYZRHW ? 16 : 12;
  if (fvf & D3DFVF_NORMAL) n += 12; if (fvf & D3DFVF_DIFFUSE) n += 4; if (fvf & D3DFVF_SPECULAR) n += 4;
  return n + 8 * (int)((fvf & D3DFVF_TEXCOUNT_MASK) >> D3DFVF_TEXCOUNT_SHIFT);
}
/* a vertex of a GxG grid layer: L 0 the far layer (whole view), 1 the near one (part of it, tilted) */
static void rvertex(BYTE *o, DWORD fvf, int L, int i, int j, int G) {
  float u = (float)i / (G - 1), v = (float)j / (G - 1); unsigned sd = (unsigned)(L * 977 + i * 131 + j * 31 + 7);
  float *f = (float *)o;
  if ((fvf & D3DFVF_POSITION_MASK) == D3DFVF_XYZRHW) {
    float x = L ? 8 + u * 44 : -2 + u * 68, y = L ? 12 + v * 44 : -2 + v * 68, z = L ? 0.3f + 0.3f * u : 0.7f + 0.2f * v;
    float w = L ? 1.5f + 3 * u : 6 + 3 * v;
    f[0] = x; f[1] = y; f[2] = z; f[3] = 1 / w; f += 4;
  } else {
    float x = L ? -1.2f + 2.4f * u : -6 + 12 * u, y = L ? -1.2f + 2.4f * v : -6 + 12 * v, z = L ? 2.5f + 2.0f * u : 7 + 2 * v;
    f[0] = x; f[1] = y; f[2] = z; f += 3;
  }
  if (fvf & D3DFVF_NORMAL) { float nx = (u - 0.5f) * (L ? 1.6f : 0.6f), ny = (v - 0.5f) * 1.2f, nz = -1.0f; f[0] = nx; f[1] = ny; f[2] = nz; f += 3; }
  DWORD *c = (DWORD *)f;
  if (fvf & D3DFVF_DIFFUSE) { DWORD a = (DWORD)(frand(&sd) * 255), r = (DWORD)(frand(&sd) * 255), g = (DWORD)(frand(&sd) * 255), b = (DWORD)(frand(&sd) * 255);
                              *c++ = a << 24 | r << 16 | g << 8 | b; }
  if (fvf & D3DFVF_SPECULAR) *c++ = 0x80404040;
  f = (float *)c;
  int nt = (int)((fvf & D3DFVF_TEXCOUNT_MASK) >> D3DFVF_TEXCOUNT_SHIFT);
  /* offset by a fraction of a texel, so that no texel boundary falls on a pixel centre (a tie each device breaks by its
     own rounding: 14 Oct, the scene centre lines) */
  for (int t = 0; t < nt; t++) { f[0] = -0.5f + 2.0f * u + 0.13f * t + 0.0137f; f[1] = -0.25f + 1.5f * v + 0.0071f; f += 2; }
}
static void rdraw(DWORD fvf, int prim, int kind) {
  enum { G = 6 }; int vs = vsize_of(fvf);
  static BYTE vb[2 * G * G * 64]; static WORD ib[2 * (G - 1) * (G - 1) * 6 + 64]; int nv = 0, ni = 0;
  for (int L = 0; L < 2; L++) for (int j = 0; j < G; j++) for (int i = 0; i < G; i++) rvertex(vb + (nv++) * vs, fvf, L, i, j, G);
  IDirect3DDevice8_SetVertexShader(d, fvf);
  IDirect3DVertexBuffer8 *b = NULL; IDirect3DIndexBuffer8 *x = NULL;
  for (int L = 0; L < 2; L++) {
    int base = L * G * G;
    ni = 0;
    if (prim == 4) {                                              /* a list: two triangles per cell */
      for (int j = 0; j < G - 1; j++) for (int i = 0; i < G - 1; i++) {
        WORD a = (WORD)(base + j * G + i), c = (WORD)(a + G);
        ib[ni++] = a; ib[ni++] = a + 1; ib[ni++] = c; ib[ni++] = c; ib[ni++] = a + 1; ib[ni++] = c + 1;
      }
    } else {                                                      /* a strip per row, joined by degenerate triangles */
      for (int j = 0; j < G - 1; j++) {
        if (j) { ib[ni] = ib[ni - 1]; ni++; ib[ni++] = (WORD)(base + j * G); }
        for (int i = 0; i < G; i++) { ib[ni++] = (WORD)(base + j * G + i); ib[ni++] = (WORD)(base + (j + 1) * G + i); }
      }
    }
    int prims = prim == 4 ? ni / 3 : ni - 2;
    if (kind == 0) {                                              /* DrawPrimitiveUP: the vertices in index order */
      static BYTE up[4096 * 64]; for (int k = 0; k < ni; k++) memcpy(up + k * vs, vb + ib[k] * vs, vs);
      IDirect3DDevice8_DrawPrimitiveUP(d, (D3DPRIMITIVETYPE)prim, prims, up, vs);
    } else if (kind == 1) {                                       /* DrawPrimitive from a vertex buffer, the same */
      if (FAILED(IDirect3DDevice8_CreateVertexBuffer(d, ni * vs, D3DUSAGE_WRITEONLY, fvf, D3DPOOL_MANAGED, &b))) continue;
      BYTE *p; IDirect3DVertexBuffer8_Lock(b, 0, 0, &p, 0); for (int k = 0; k < ni; k++) memcpy(p + k * vs, vb + ib[k] * vs, vs);
      IDirect3DVertexBuffer8_Unlock(b);
      IDirect3DDevice8_SetStreamSource(d, 0, b, vs); IDirect3DDevice8_DrawPrimitive(d, (D3DPRIMITIVETYPE)prim, 0, prims);
      IDirect3DVertexBuffer8_Release(b); b = NULL;
    } else {                                                      /* DrawIndexedPrimitive, the layer's vertices at a base */
      if (FAILED(IDirect3DDevice8_CreateVertexBuffer(d, nv * vs, D3DUSAGE_WRITEONLY, fvf, D3DPOOL_MANAGED, &b))) continue;
      if (FAILED(IDirect3DDevice8_CreateIndexBuffer(d, ni * 2, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &x))) { IDirect3DVertexBuffer8_Release(b); continue; }
      BYTE *p; IDirect3DVertexBuffer8_Lock(b, 0, 0, &p, 0); memcpy(p, vb, nv * vs); IDirect3DVertexBuffer8_Unlock(b);
      WORD *q; IDirect3DIndexBuffer8_Lock(x, 0, 0, (BYTE **)&q, 0); for (int k = 0; k < ni; k++) q[k] = (WORD)(ib[k] - base); IDirect3DIndexBuffer8_Unlock(x);
      IDirect3DDevice8_SetStreamSource(d, 0, b, vs); IDirect3DDevice8_SetIndices(d, x, (UINT)base);
      IDirect3DDevice8_DrawIndexedPrimitive(d, (D3DPRIMITIVETYPE)prim, 0, G * G, 0, prims);
      IDirect3DDevice8_SetStreamSource(d, 0, NULL, 0); IDirect3DDevice8_SetIndices(d, NULL, 0);
      IDirect3DVertexBuffer8_Release(b); IDirect3DIndexBuffer8_Release(x); b = NULL; x = NULL;
    }
  }
}
static DWORD tx_cube(int x, int y) { return 0xFF000000u | (DWORD)(x * 60) << 16 | (DWORD)(y * 60) << 8 | 0x80; }
static void random_cases(const char *path) {
  FILE *f = fopen(path, "r"); if (!f) { printf("FAIL: cannot open %s\n", path); return; }
  IDirect3DTexture8 *tx[4] = {NULL, texture(16, 16, tx_checker), texture(16, 16, tx_grad), texture(16, 16, tx_alpha)};
  if (SUCCEEDED(IDirect3DDevice8_CreateCubeTexture(d, 4, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &g_cube)))
    for (int fc = 0; fc < 6; fc++) { D3DLOCKED_RECT lr; IDirect3DCubeTexture8_LockRect(g_cube, (D3DCUBEMAP_FACES)fc, 0, &lr, NULL, 0);
      for (int y = 0; y < 4; y++) for (int x = 0; x < 4; x++) ((DWORD *)((BYTE *)lr.pBits + y * lr.Pitch))[x] = tx_cube(x, y) ^ (DWORD)(fc * 0x2A0000);
      IDirect3DCubeTexture8_UnlockRect(g_cube, (D3DCUBEMAP_FACES)fc, 0); }
  g_cube_flat = cube_of(cube_flat); g_cube_faces = cube_of(cube_faces);
  D3DLIGHT8 lt[3]; memset(lt, 0, sizeof lt);
  lt[0].Type = D3DLIGHT_DIRECTIONAL; lt[0].Direction.x = 0.3f; lt[0].Direction.y = -0.5f; lt[0].Direction.z = 1;
  lt[1].Type = D3DLIGHT_POINT; lt[1].Position.x = 1; lt[1].Position.y = 1; lt[1].Position.z = 1; lt[1].Range = 20; lt[1].Attenuation0 = 0.5f; lt[1].Attenuation1 = 0.1f;
  lt[2].Type = D3DLIGHT_SPOT; lt[2].Position.z = -1; lt[2].Direction.z = 1; lt[2].Range = 30; lt[2].Attenuation0 = 1; lt[2].Theta = 0.3f; lt[2].Phi = 0.9f; lt[2].Falloff = 1;
  for (int i = 0; i < 3; i++) {
    lt[i].Diffuse.r = 0.9f - 0.2f * i; lt[i].Diffuse.g = 0.5f + 0.2f * i; lt[i].Diffuse.b = 0.4f; lt[i].Diffuse.a = 1;
    lt[i].Ambient.r = lt[i].Ambient.g = lt[i].Ambient.b = 0.1f; IDirect3DDevice8_SetLight(d, i, &lt[i]);
  }
  char line[512], name[64] = ""; unsigned fvf = 0; int prim = 5, kind = 0;
  while (fgets(line, sizeof line, f)) {
    unsigned a, b, c; int i; float m[6];
    if (sscanf(line, "case %63s %x %d %d", name, &fvf, &prim, &kind) == 4) {
      reset_state();
      D3DMATRIX pr; persp(&pr, 1.2f, 1.0f, 20.0f); IDirect3DDevice8_SetTransform(d, D3DTS_PROJECTION, &pr);
      for (i = 0; i < 3; i++) IDirect3DDevice8_LightEnable(d, i, FALSE);
      D3DMATERIAL8 mt; memset(&mt, 0, sizeof mt); mt.Diffuse.r = mt.Diffuse.g = mt.Diffuse.b = mt.Diffuse.a = 1; mt.Ambient = mt.Diffuse;
      IDirect3DDevice8_SetMaterial(d, &mt);
      for (i = 0; i < 4; i++) { D3DMATRIX id; memset(&id, 0, sizeof id); id._11 = id._22 = id._33 = id._44 = 1; IDirect3DDevice8_SetTransform(d, (D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + i), &id); }
      IDirect3DDevice8_SetRenderState(d, D3DRS_ZENABLE, D3DZB_TRUE);
    } else if (sscanf(line, "rs %u %x", &a, &b) == 2) IDirect3DDevice8_SetRenderState(d, (D3DRENDERSTATETYPE)a, b);
    else if (sscanf(line, "tss %u %u %x", &a, &b, &c) == 3) IDirect3DDevice8_SetTextureStageState(d, a, (D3DTEXTURESTAGESTATETYPE)b, c);
    else if (sscanf(line, "tex %u %u", &a, &b) == 2) IDirect3DDevice8_SetTexture(d, a, b == 4 ? (IDirect3DBaseTexture8 *)g_cube : b == 5 ? (IDirect3DBaseTexture8 *)g_cube_flat :
                                                                     b == 6 ? (IDirect3DBaseTexture8 *)g_cube_faces : b < 4 ? (IDirect3DBaseTexture8 *)tx[b] : NULL);
    else if (sscanf(line, "light %u %u", &a, &b) == 2) IDirect3DDevice8_LightEnable(d, a, b);
    else if (sscanf(line, "tt %u %f %f %f %f %f %f", &a, &m[0], &m[1], &m[2], &m[3], &m[4], &m[5]) == 7) {
      D3DMATRIX t; memset(&t, 0, sizeof t); t._11 = m[0]; t._12 = m[1]; t._21 = m[2]; t._22 = m[3]; t._31 = m[4]; t._32 = m[5]; t._33 = 1; t._44 = 1;
      IDirect3DDevice8_SetTransform(d, (D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + a), &t);
    } else if (sscanf(line, "mat %f %f %f %f", &m[0], &m[1], &m[2], &m[3]) == 4) {
      D3DMATERIAL8 mt; memset(&mt, 0, sizeof mt); mt.Diffuse.r = m[0]; mt.Diffuse.g = m[1]; mt.Diffuse.b = m[2]; mt.Diffuse.a = m[3];
      mt.Ambient.r = mt.Ambient.g = mt.Ambient.b = 0.5f; mt.Ambient.a = 1; mt.Emissive.b = 0.1f; IDirect3DDevice8_SetMaterial(d, &mt);
    } else if (!strncmp(line, "draw", 4)) {
      begin(0xFF203040); rdraw(fvf, prim, kind); end(name);
    }
  }
  fclose(f);
  for (int i = 1; i < 4; i++) if (tx[i]) IDirect3DTexture8_Release(tx[i]);
  if (g_cube) IDirect3DCubeTexture8_Release(g_cube);
  if (g_cube_flat) IDirect3DCubeTexture8_Release(g_cube_flat); if (g_cube_faces) IDirect3DCubeTexture8_Release(g_cube_faces);
}

/* ---- threads and churn ---------------------------------------------------------------------------------------------- */
static volatile LONG g_side_done, g_side_fail;
static DWORD tx_solid(int x, int y) { (void)x; (void)y; return 0xFF30C050; }
static void churn_round(int i) {
  IDirect3DTexture8 *t = NULL; IDirect3DVertexBuffer8 *v = NULL; IDirect3DIndexBuffer8 *x = NULL; D3DLOCKED_RECT lr; BYTE *p;
  if (SUCCEEDED(IDirect3DDevice8_CreateTexture(d, 8 + (i & 7) * 4, 8, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &t)) &&
      SUCCEEDED(IDirect3DTexture8_LockRect(t, 0, &lr, NULL, 0))) { memset(lr.pBits, i & 255, lr.Pitch * 8); IDirect3DTexture8_UnlockRect(t, 0); }
  else InterlockedIncrement(&g_side_fail);
  if (SUCCEEDED(IDirect3DDevice8_CreateVertexBuffer(d, 256 + (i & 15) * 16, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &v)) &&
      SUCCEEDED(IDirect3DVertexBuffer8_Lock(v, 0, 0, &p, D3DLOCK_DISCARD))) { memset(p, i & 255, 256); IDirect3DVertexBuffer8_Unlock(v); }
  else InterlockedIncrement(&g_side_fail);
  if (SUCCEEDED(IDirect3DDevice8_CreateIndexBuffer(d, 64, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &x)) &&
      SUCCEEDED(IDirect3DIndexBuffer8_Lock(x, 0, 0, &p, 0))) { memset(p, 0, 64); IDirect3DIndexBuffer8_Unlock(x); }
  else InterlockedIncrement(&g_side_fail);
  if (t) IDirect3DTexture8_Release(t); if (v) IDirect3DVertexBuffer8_Release(v); if (x) IDirect3DIndexBuffer8_Release(x);
}
static DWORD WINAPI side_thread(LPVOID n) { for (int i = 0; i < (int)(INT_PTR)n; i++) churn_round(i); InterlockedExchange(&g_side_done, 1); return 0; }
static void textured_check(const char *what) {
  IDirect3DTexture8 *t = texture(4, 4, tx_solid);
  reset_state(); IDirect3DDevice8_SetTexture(d, 0, (IDirect3DBaseTexture8 *)t);
  IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  begin(0xFF000000);
  VRhwT q[4] = {{0, 0, 0.5f, 1, 0xFFFFFFFF, 0, 0}, {64, 0, 0.5f, 1, 0xFFFFFFFF, 1, 0}, {0, 64, 0.5f, 1, 0xFFFFFFFF, 0, 1}, {64, 64, 0.5f, 1, 0xFFFFFFFF, 1, 1}};
  IDirect3DDevice8_SetVertexShader(d, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
  IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLESTRIP, 2, q, sizeof *q);
  IDirect3DDevice8_EndScene(d);
  DWORD px = bb_pixel(20, 30);
  char m[160]; snprintf(m, sizeof m, "%s: a textured draw afterwards reads back the texture's colour (%06lX)", what, (unsigned long)px);
  check(px == 0x30C050, m);
  IDirect3DDevice8_SetTexture(d, 0, NULL); IDirect3DTexture8_Release(t);
}
static void threads_test(void) {
  HANDLE h = CreateThread(NULL, 0, side_thread, (LPVOID)(INT_PTR)2000, 0, NULL);
  int frames = 0;
  while (!g_side_done && frames < 100000) {
    IDirect3DDevice8_Clear(d, 0, NULL, D3DCLEAR_TARGET, 0xFF000000 | (DWORD)frames, 1.0f, 0);
    begin(0xFF102030); quad_rhw(4, 4, 60, 60, 0.5f, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFFFFFFFF); IDirect3DDevice8_EndScene(d);
    IDirect3DDevice8_Present(d, NULL, NULL, NULL, NULL);
    if (frames % 10 == 0) bb_pixel(1, 1);
    churn_round(frames);                                  /* objects made here too, at the same time as there */
    frames++;
  }
  WaitForSingleObject(h, 60000); CloseHandle(h);
  char m[120]; snprintf(m, sizeof m, "threads: 2,000 rounds on the second thread, every create and lock succeeded, while %d frames ran here", frames);
  check(g_side_done && !g_side_fail, m);
  textured_check("threads");
}
static void churn_test(void) {
  for (int i = 0; i < 5000; i++) {
    churn_round(i);
    IDirect3DSurface8 *rt = NULL, *ds = NULL, *img = NULL;
    if (FAILED(IDirect3DDevice8_CreateRenderTarget(d, 16, 16, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, i & 1, &rt))) g_side_fail++;
    if (FAILED(IDirect3DDevice8_CreateDepthStencilSurface(d, 16, 16, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, &ds))) g_side_fail++;
    if (FAILED(IDirect3DDevice8_CreateImageSurface(d, 16, 16, D3DFMT_A8R8G8B8, &img))) g_side_fail++;
    if (rt) IDirect3DSurface8_Release(rt); if (ds) IDirect3DSurface8_Release(ds); if (img) IDirect3DSurface8_Release(img);
    if (i % 500 == 0) IDirect3DDevice8_Present(d, NULL, NULL, NULL, NULL);
  }
  check(!g_side_fail, "churn: 5,000 rounds of textures, buffers and surfaces, every create and lock succeeded");
  textured_check("churn");
}

int main(int argc, char **argv) {
  setvbuf(stdout, NULL, _IONBF, 0);
  g_d3d = Direct3DCreate8(D3D_SDK_VERSION);
  if (!g_d3d) { printf("FAIL: Direct3DCreate8\n"); return 2; }
  HWND w = window(); D3DPRESENT_PARAMETERS pp;
  HRESULT hr = make(w, &pp);
  if (FAILED(hr)) { printf("FAIL: CreateDevice 0x%08lX\n", (unsigned long)hr); return 2; }
  if (argc > 1 && !strcmp(argv[1], "scenes")) {
    scenes();
    IDirect3DDevice8_Release(d); IDirect3D8_Release(g_d3d);
    return 0;
  }
  if (argc > 1 && (!strcmp(argv[1], "threads") || !strcmp(argv[1], "churn"))) {
    if (!strcmp(argv[1], "threads")) threads_test(); else churn_test();
    IDirect3DDevice8_Release(d); IDirect3D8_Release(g_d3d);
    printf("%s\n", fails ? "FAILED" : "ALL OK");
    return fails != 0;
  }
  if (argc > 2 && !strcmp(argv[1], "random")) {
    random_cases(argv[2]);
    IDirect3DDevice8_Release(d); IDirect3D8_Release(g_d3d);
    return 0;
  }
  if (argc > 1 && (!strcmp(argv[1], "frames") || !strcmp(argv[1], "orphan"))) {
    int kill = !strcmp(argv[1], "orphan");
    for (int f = 0; f < 600; f++) {
      if (f == 30 && kill) {
        int n = 0; HANDLE sn = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); PROCESSENTRY32 pe; pe.dwSize = sizeof pe;
        for (BOOL more = Process32First(sn, &pe); more; more = Process32Next(sn, &pe))
          if (!_strnicmp(pe.szExeFile, "d8x", 3)) {
            HANDLE p = OpenProcess(PROCESS_TERMINATE | SYNCHRONIZE, FALSE, pe.th32ProcessID);
            if (p) { TerminateProcess(p, 7); WaitForSingleObject(p, 5000); CloseHandle(p); n++; }
          }
        CloseHandle(sn);
        printf("ended %d executor process(es)\n", n);
      }
      IDirect3DDevice8_Clear(d, 0, NULL, D3DCLEAR_TARGET, 0xFF000000 | (f * 4), 1.0f, 0);
      IDirect3DDevice8_Present(d, NULL, NULL, NULL, NULL);
      if (f >= 30) bb_pixel(1, 1);                 /* a read-back: waits for an answer that cannot come */
      printf("presented %d\n", f);
    }
    printf("still running after 600 frames\n");
    return 0;
  }
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
