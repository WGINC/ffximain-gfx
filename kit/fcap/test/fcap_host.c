/* fcap_host.c -- the frame capture's test program: draws game-like frames through a real Direct3D 8 device (Wine's
   d3d8 in the test), with d3dprof and fcap attached to that device as the kit attaches them to the game's, and
   captures one frame. test_fcap.py then replays the capture through Wine's d3d8 and through kit/d3d8on11 and compares
   the pictures with the one the capture took.

   What the frames hold, each a path the game's frames take (data/d3d8_scope.json, run 2's resource log):
     - a DEFAULT-pool WRITEONLY index buffer filled once at creation, and DEFAULT-pool DYNAMIC vertex buffers filled in
       the first frame only: their contents must come from the copies kept since creation;
     - a dynamic vertex buffer rewritten with DISCARD every frame (a lock inside the captured frame);
     - managed DXT1 and A8R8G8B8 textures, a cube texture;
     - the three-stream character declaration, lit; an FVF draw; ...UP draws (a strip and a fan);
     - a state block recorded in the first frame and applied in the captured one;
     - a render-to-texture pass whose texture is then drawn;
     - fog, alpha blending, ZBIAS, a CopyRects into a lockable render target (the occlusion readback's path). */
#define COBJMACROS
#include <windows.h>
#include <d3d8.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

void d3dprof_attach(void **dev, uint32_t rel, uint32_t lo, uint32_t hi, const char *csv);
extern int fcap_enabled; void fcap_arm(void); int fcap_busy(void); unsigned fcap_written(void);

static IDirect3DDevice8 *d;
static LRESULT CALLBACK wp(HWND h, UINT m, WPARAM w, LPARAM l) { return DefWindowProcA(h, m, w, l); }
typedef struct { float x, y, z, rhw; DWORD c; float u, v; } VRhwT;

static IDirect3DTexture8 *tex_argb(int w, int h, int seed) {
  IDirect3DTexture8 *t = NULL; D3DLOCKED_RECT lr;
  IDirect3DDevice8_CreateTexture(d, w, h, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &t);
  IDirect3DTexture8_LockRect(t, 0, &lr, NULL, 0);
  for (int y = 0; y < h; y++) for (int x = 0; x < w; x++)
    ((DWORD *)((BYTE *)lr.pBits + y * lr.Pitch))[x] = 0xFF000000 | ((x * 16 + seed) & 255) << 16 | ((y * 16) & 255) << 8 | (((x ^ y) & 4) ? 0xE0 : 0x30);
  IDirect3DTexture8_UnlockRect(t, 0);
  return t;
}
static IDirect3DTexture8 *tex_dxt1(void) {
  IDirect3DTexture8 *t = NULL;
  IDirect3DDevice8_CreateTexture(d, 16, 16, 0, 0, D3DFMT_DXT1, D3DPOOL_MANAGED, &t);
  for (UINT l = 0; l < IDirect3DTexture8_GetLevelCount(t); l++) {
    D3DLOCKED_RECT lr; D3DSURFACE_DESC sd; IDirect3DTexture8_GetLevelDesc(t, l, &sd); IDirect3DTexture8_LockRect(t, l, &lr, NULL, 0);
    for (UINT y = 0; y < (sd.Height + 3) / 4; y++) for (UINT x = 0; x < (sd.Width + 3) / 4; x++) {
      WORD *b = (WORD *)((BYTE *)lr.pBits + y * lr.Pitch + x * 8);
      b[0] = (WORD)(0xF800 >> (l * 3)) | 0x1F; b[1] = (WORD)(0x07E0 + x * 0x0841 + y * 0x1000); b[2] = 0x1B4E; b[3] = 0xE4B1; }
    IDirect3DTexture8_UnlockRect(t, l); }
  return t;
}

int main(int argc, char **argv) {
  setvbuf(stdout, NULL, _IONBF, 0);
  const char *out = argc > 1 ? argv[1] : "fcap_host_d3dprof.csv";
  WNDCLASSA wc; memset(&wc, 0, sizeof wc); wc.lpfnWndProc = wp; wc.hInstance = GetModuleHandleA(NULL); wc.lpszClassName = "fh"; RegisterClassA(&wc);
  HWND w = CreateWindowA("fh", "fcap host", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 200, 200, NULL, NULL, wc.hInstance, NULL);
  IDirect3D8 *d3d = Direct3DCreate8(D3D_SDK_VERSION);
  D3DPRESENT_PARAMETERS pp; memset(&pp, 0, sizeof pp);
  pp.Windowed = TRUE; pp.SwapEffect = D3DSWAPEFFECT_DISCARD; pp.BackBufferFormat = D3DFMT_X8R8G8B8; pp.BackBufferWidth = 128; pp.BackBufferHeight = 96;
  pp.EnableAutoDepthStencil = TRUE; pp.AutoDepthStencilFormat = D3DFMT_D24S8; pp.hDeviceWindow = w;
  if (FAILED(IDirect3D8_CreateDevice(d3d, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, w, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &d))) { printf("FAIL: CreateDevice\n"); return 2; }
  /* d3dprof and fcap on this device, as the kit attaches them to the game's (this program stands for the game) */
  fcap_enabled = 1;
  HMODULE self = GetModuleHandleA(NULL); IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS *)((BYTE *)self + ((IMAGE_DOS_HEADER *)self)->e_lfanew);
  uint32_t lo = (uint32_t)(uintptr_t)self, hi = lo + nt->OptionalHeader.SizeOfImage;
  d3dprof_attach((void **)d, lo - 0x10000000u, lo, hi, out);

  /* resources, as the game makes them */
  IDirect3DTexture8 *ta = tex_argb(16, 16, 0), *tb = tex_dxt1(), *rtt = NULL;
  IDirect3DDevice8_CreateTexture(d, 32, 32, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &rtt);
  IDirect3DSurface8 *occl = NULL; IDirect3DDevice8_CreateRenderTarget(d, 16, 16, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, TRUE, &occl);
  IDirect3DVertexBuffer8 *vp = NULL, *vn = NULL, *vt = NULL, *dyn = NULL; IDirect3DIndexBuffer8 *ib = NULL; BYTE *p;
  IDirect3DDevice8_CreateVertexBuffer(d, 64 * 12, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &vp);
  IDirect3DDevice8_CreateVertexBuffer(d, 64 * 12, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &vn);
  IDirect3DDevice8_CreateVertexBuffer(d, 64 * 8, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &vt);
  IDirect3DDevice8_CreateVertexBuffer(d, 4 * sizeof(VRhwT), D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1, D3DPOOL_DEFAULT, &dyn);
  IDirect3DDevice8_CreateIndexBuffer(d, 7 * 7 * 6 * 2, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &ib);
  IDirect3DIndexBuffer8_Lock(ib, 0, 0, &p, 0);
  { WORD *I = (WORD *)p; int n = 0;
    for (int y = 0; y < 7; y++) for (int x = 0; x < 7; x++) { WORD a = (WORD)(y * 8 + x); I[n++] = a; I[n++] = a + 1; I[n++] = a + 8; I[n++] = a + 1; I[n++] = a + 9; I[n++] = a + 8; } }
  IDirect3DIndexBuffer8_Unlock(ib);
  static const DWORD decl[] = {D3DVSD_STREAM(0), D3DVSD_REG(0, D3DVSDT_FLOAT3), D3DVSD_STREAM(1), D3DVSD_REG(3, D3DVSDT_FLOAT3),
                               D3DVSD_STREAM(2), D3DVSD_REG(7, D3DVSDT_FLOAT2), D3DVSD_END()};
  DWORD vsh = 0; IDirect3DDevice8_CreateVertexShader(d, decl, NULL, &vsh, 0);
  D3DMATRIX proj, view; memset(&proj, 0, sizeof proj); memset(&view, 0, sizeof view);
  { float ys = 1.0f / tanf(0.5f), zn = 1, zf = 100; proj._11 = ys * 96 / 128; proj._22 = ys; proj._33 = zf / (zf - zn); proj._34 = 1; proj._43 = -zn * zf / (zf - zn); }
  view._11 = view._22 = view._33 = view._44 = 1; view._43 = 5;
  DWORD sb = 0;

  for (int frame = 0; frame < 6; frame++) {
    if (frame == 0) {                           /* the skinned buffers: written once, drawn in every frame */
      float *P, *N, *T;
      IDirect3DVertexBuffer8_Lock(vp, 0, 0, &p, D3DLOCK_DISCARD); P = (float *)p;
      IDirect3DVertexBuffer8_Lock(vn, 0, 0, &p, D3DLOCK_DISCARD); N = (float *)p;
      IDirect3DVertexBuffer8_Lock(vt, 0, 0, &p, 0); T = (float *)p;
      for (int y = 0; y < 8; y++) for (int x = 0; x < 8; x++) {
        int k = y * 8 + x; float fx = x / 7.0f * 2 - 1, fy = y / 7.0f * 2 - 1;
        P[3 * k] = fx * 1.6f - 0.8f; P[3 * k + 1] = -fy * 1.6f; P[3 * k + 2] = 0.3f * fx * fx;
        N[3 * k] = fx * 0.6f; N[3 * k + 1] = -fy * 0.6f; N[3 * k + 2] = -1;
        T[2 * k] = x / 7.0f * 1.9f; T[2 * k + 1] = y / 7.0f * 1.9f; }
      IDirect3DVertexBuffer8_Unlock(vp); IDirect3DVertexBuffer8_Unlock(vn); IDirect3DVertexBuffer8_Unlock(vt);
      /* a state block, applied later (the game's effects apply blocks recorded once) */
      IDirect3DDevice8_BeginStateBlock(d);
      IDirect3DDevice8_SetRenderState(d, D3DRS_ALPHABLENDENABLE, TRUE);
      IDirect3DDevice8_SetRenderState(d, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); IDirect3DDevice8_SetRenderState(d, D3DRS_DESTBLEND, D3DBLEND_ONE);
      IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
      IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
      IDirect3DDevice8_EndStateBlock(d, &sb);
    }
    if (frame == 2) fcap_arm();                /* the next Present (frame 2's) starts the capture: frame 3 is captured */
    /* render to a texture */
    IDirect3DSurface8 *bb = NULL, *ds = NULL, *lv = NULL;
    IDirect3DDevice8_GetRenderTarget(d, &bb); IDirect3DDevice8_GetDepthStencilSurface(d, &ds); IDirect3DTexture8_GetSurfaceLevel(rtt, 0, &lv);
    IDirect3DDevice8_SetRenderTarget(d, lv, NULL);
    IDirect3DDevice8_Clear(d, 0, NULL, D3DCLEAR_TARGET, 0xFF203060 + frame * 0x10, 1.0f, 0);
    IDirect3DDevice8_BeginScene(d);
    IDirect3DDevice8_SetRenderState(d, D3DRS_ZENABLE, FALSE); IDirect3DDevice8_SetRenderState(d, D3DRS_LIGHTING, FALSE);
    IDirect3DDevice8_SetRenderState(d, D3DRS_CULLMODE, D3DCULL_NONE);
    IDirect3DDevice8_SetTexture(d, 0, (IDirect3DBaseTexture8 *)ta);
    IDirect3DDevice8_SetVertexShader(d, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    { VRhwT f[5] = {{16, 16, 0.5f, 1, 0xFFFFFFFF, 0.5f, 0.5f}, {16, 2, 0.5f, 1, 0xFFFF8080, 0.5f, 0}, {30, 16, 0.5f, 1, 0xFF80FF80, 1, 0.5f},
                    {16, 30, 0.5f, 1, 0xFF8080FF, 0.5f, 1}, {2, 16, 0.5f, 1, 0xFFFFFF80, 0, 0.5f}};
      IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLEFAN, 3, f, sizeof *f); }
    IDirect3DDevice8_EndScene(d);
    /* the occlusion readback's path: CopyRects of a render target into a lockable one, then LockRect. As the game's
       call (0x1006C8FD): one rectangle, the constant {0, 0, 16, 16} at 0x1032C18C, and no points; the whole 32x32
       level would not fit the 16x16 target, which the DirectX 8.1 reference says fails (IDirect3DDevice8::CopyRects) */
    { static const RECT occl_rect = {0, 0, 16, 16}; IDirect3DDevice8_CopyRects(d, lv, &occl_rect, 1, occl, NULL); }
    { D3DLOCKED_RECT lr; if (SUCCEEDED(IDirect3DSurface8_LockRect(occl, &lr, NULL, D3DLOCK_READONLY))) IDirect3DSurface8_UnlockRect(occl); }
    IDirect3DDevice8_SetRenderTarget(d, bb, ds);
    IDirect3DSurface8_Release(lv); IDirect3DSurface8_Release(bb); IDirect3DSurface8_Release(ds);
    /* the main scene */
    IDirect3DDevice8_Clear(d, 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF101820, 1.0f, 0);
    IDirect3DDevice8_BeginScene(d);
    IDirect3DDevice8_SetTransform(d, D3DTS_PROJECTION, &proj); IDirect3DDevice8_SetTransform(d, D3DTS_VIEW, &view);
    IDirect3DDevice8_SetRenderState(d, D3DRS_ZENABLE, TRUE); IDirect3DDevice8_SetRenderState(d, D3DRS_LIGHTING, TRUE);
    IDirect3DDevice8_SetRenderState(d, D3DRS_AMBIENT, 0xFF42404C);
    IDirect3DDevice8_SetRenderState(d, D3DRS_FOGENABLE, TRUE); IDirect3DDevice8_SetRenderState(d, D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
    { float fs = 4, fe = 9; DWORD dfs, dfe; memcpy(&dfs, &fs, 4); memcpy(&dfe, &fe, 4); IDirect3DDevice8_SetRenderState(d, D3DRS_FOGSTART, dfs); IDirect3DDevice8_SetRenderState(d, D3DRS_FOGEND, dfe); }
    IDirect3DDevice8_SetRenderState(d, D3DRS_FOGCOLOR, 0xFF676968);
    { D3DMATERIAL8 m; memset(&m, 0, sizeof m); m.Diffuse.r = m.Diffuse.g = m.Diffuse.b = m.Diffuse.a = 1; m.Ambient = m.Diffuse; IDirect3DDevice8_SetMaterial(d, &m);
      D3DLIGHT8 l; memset(&l, 0, sizeof l); l.Type = D3DLIGHT_DIRECTIONAL; l.Diffuse.r = l.Diffuse.g = l.Diffuse.b = 0.8f;
      l.Direction.x = 0.4f; l.Direction.y = -0.6f; l.Direction.z = 1; IDirect3DDevice8_SetLight(d, 0, &l); IDirect3DDevice8_LightEnable(d, 0, TRUE); }
    IDirect3DDevice8_SetTexture(d, 0, (IDirect3DBaseTexture8 *)tb);
    IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR); IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
    IDirect3DDevice8_SetVertexShader(d, vsh);
    IDirect3DDevice8_SetStreamSource(d, 0, vp, 12); IDirect3DDevice8_SetStreamSource(d, 1, vn, 12); IDirect3DDevice8_SetStreamSource(d, 2, vt, 8);
    IDirect3DDevice8_SetIndices(d, ib, 0);
    IDirect3DDevice8_DrawIndexedPrimitive(d, D3DPT_TRIANGLELIST, 0, 64, 0, 7 * 7 * 2);
    IDirect3DDevice8_SetRenderState(d, D3DRS_ZBIAS, 8);
    { D3DMATRIX wm; memset(&wm, 0, sizeof wm); wm._11 = wm._22 = wm._33 = 0.5f; wm._44 = 1; wm._41 = 1.6f; wm._42 = 0.4f;
      IDirect3DDevice8_SetTransform(d, D3DTS_WORLD, &wm);
      IDirect3DDevice8_DrawIndexedPrimitive(d, D3DPT_TRIANGLELIST, 0, 64, 0, 7 * 7 * 2);
      wm._11 = wm._22 = wm._33 = 1; wm._41 = wm._42 = 0; IDirect3DDevice8_SetTransform(d, D3DTS_WORLD, &wm); }
    IDirect3DDevice8_SetRenderState(d, D3DRS_ZBIAS, 0);
    IDirect3DDevice8_LightEnable(d, 0, FALSE); IDirect3DDevice8_SetRenderState(d, D3DRS_LIGHTING, FALSE); IDirect3DDevice8_SetRenderState(d, D3DRS_FOGENABLE, FALSE);
    /* a dynamic buffer rewritten every frame, drawn with the render-target texture and the recorded state block */
    { VRhwT *q; IDirect3DVertexBuffer8_Lock(dyn, 0, 0, (BYTE **)&q, D3DLOCK_DISCARD);
      float x0 = 84 + frame, y0 = 54;
      VRhwT v[4] = {{x0, y0, 0.1f, 1, 0x80FFFFFF, 0, 0}, {x0 + 36, y0, 0.1f, 1, 0x80FFFFFF, 1, 0}, {x0, y0 + 36, 0.1f, 1, 0xC0FFFFFF, 0, 1}, {x0 + 36, y0 + 36, 0.1f, 1, 0xC0FFFFFF, 1, 1}};
      memcpy(q, v, sizeof v); IDirect3DVertexBuffer8_Unlock(dyn); }
    IDirect3DDevice8_ApplyStateBlock(d, sb);
    IDirect3DDevice8_SetTexture(d, 0, (IDirect3DBaseTexture8 *)rtt);
    IDirect3DDevice8_SetVertexShader(d, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    IDirect3DDevice8_SetStreamSource(d, 0, dyn, sizeof(VRhwT));
    IDirect3DDevice8_DrawPrimitive(d, D3DPT_TRIANGLESTRIP, 0, 2);
    IDirect3DDevice8_SetRenderState(d, D3DRS_ALPHABLENDENABLE, FALSE);
    IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_COLOROP, D3DTOP_MODULATE); IDirect3DDevice8_SetTextureStageState(d, 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    /* a UI quad through ...UP */
    IDirect3DDevice8_SetTexture(d, 0, (IDirect3DBaseTexture8 *)ta);
    { VRhwT v[4] = {{4, 4, 0, 1, 0xFFFFFFFF, 0, 0}, {28, 4, 0, 1, 0xFFFFFFFF, 1, 0}, {4, 20, 0, 1, 0xFFFFFFFF, 0, 1}, {28, 20, 0, 1, 0xFFFFFFFF, 1, 1}};
      IDirect3DDevice8_DrawPrimitiveUP(d, D3DPT_TRIANGLESTRIP, 2, v, sizeof *v); }
    IDirect3DDevice8_EndScene(d);
    IDirect3DDevice8_Present(d, NULL, NULL, NULL, NULL);
  }
  printf("captures written: %u%s\n", fcap_written(), fcap_busy() ? " (one still open)" : "");
  IDirect3DDevice8_DeleteStateBlock(d, sb); IDirect3DDevice8_DeleteVertexShader(d, vsh);
  IDirect3DVertexBuffer8_Release(vp); IDirect3DVertexBuffer8_Release(vn); IDirect3DVertexBuffer8_Release(vt); IDirect3DVertexBuffer8_Release(dyn);
  IDirect3DIndexBuffer8_Release(ib); IDirect3DSurface8_Release(occl);
  IDirect3DTexture8_Release(ta); IDirect3DTexture8_Release(tb); IDirect3DTexture8_Release(rtt);
  IDirect3DDevice8_SetTexture(d, 0, NULL); IDirect3DDevice8_SetStreamSource(d, 0, NULL, 0); IDirect3DDevice8_SetStreamSource(d, 1, NULL, 0);
  IDirect3DDevice8_SetStreamSource(d, 2, NULL, 0); IDirect3DDevice8_SetIndices(d, NULL, 0);
  IDirect3DDevice8_Release(d); IDirect3D8_Release(d3d); DestroyWindow(w);
  return fcap_written() == 1 ? 0 : 1;
}
