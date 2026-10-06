// overlay.cpp -- an example mod (docs/SDK.md): it counts the calls of a hooked game function and draws a small box
// in the top-left corner each frame. Build:  sdk/build_mod.sh overlay.dll sdk/examples/overlay.cpp
//
// What it shows: a pass-through hook (CDx::DrawFadeQuad, which the game calls once a frame: the kit's call counts
// in town show 600 calls per 600 frames), the frame callback, and drawing with the game's Direct3D 8 device while
// leaving the device's state as the game had it.
#include "xikit_sdk.hpp"
XIKIT_MOD_GLOBALS

// values from the DirectX 8 SDK header d3d8types.h that xi_d3d8.hpp does not name
enum : unsigned { FVF_XYZRHW = 0x004, FVF_DIFFUSE = 0x040, TOP_SELECTARG1 = 2, TA_DIFFUSE = 0, BLEND_SRCALPHA = 5,
                  BLEND_INVSRCALPHA = 6, CULL_NONE = 1 };

static const XikitHost *g_host;
static volatile long g_fades, g_frames;

static xikit::fn<decltype(&CDx::DrawFadeQuad)> prev_DrawFadeQuad;
static int __thiscall my_DrawFadeQuad(CDx *self, int a1, int a2) {
  g_fades++;
  return prev_DrawFadeQuad(self, a1, a2);         // the game's own DrawFadeQuad, unchanged
}

struct Vtx { float x, y, z, rhw; unsigned color; };

extern "C" __declspec(dllexport) void xikit_mod_frame(void *device) {
  IDirect3DDevice8 *dev = static_cast<IDirect3DDevice8 *>(device);
  if (++g_frames % 600 == 0) g_host->log("overlay: %ld DrawFadeQuad calls in the last 600 frames", (long)g_fades), g_fades = 0;
  // the states this draw changes, saved and put back afterwards
  static const D3DRENDERSTATETYPE rs[] = { D3DRS_ZENABLE, D3DRS_ZWRITEENABLE, D3DRS_LIGHTING, D3DRS_FOGENABLE, D3DRS_CULLMODE,
                                           D3DRS_ALPHATESTENABLE, D3DRS_ALPHABLENDENABLE, D3DRS_SRCBLEND, D3DRS_DESTBLEND };
  static const D3DTEXTURESTAGESTATETYPE ts[] = { D3DTSS_COLOROP, D3DTSS_COLORARG1, D3DTSS_ALPHAOP, D3DTSS_ALPHAARG1 };
  unsigned rsv[sizeof rs / sizeof *rs], tsv[sizeof ts / sizeof *ts], vs = 0; IDirect3DBaseTexture8 *tex = nullptr;
  for (unsigned i = 0; i < sizeof rs / sizeof *rs; i++) dev->GetRenderState(rs[i], &rsv[i]);
  for (unsigned i = 0; i < sizeof ts / sizeof *ts; i++) dev->GetTextureStageState(0, ts[i], &tsv[i]);
  dev->GetVertexShader(&vs); dev->GetTexture(0, &tex);
  if (dev->BeginScene() >= 0) {
    const unsigned rsn[] = { 0, 0, 0, 0, CULL_NONE, 0, 1, BLEND_SRCALPHA, BLEND_INVSRCALPHA };
    for (unsigned i = 0; i < sizeof rs / sizeof *rs; i++) dev->SetRenderState(rs[i], rsn[i]);
    dev->SetTextureStageState(0, D3DTSS_COLOROP, TOP_SELECTARG1); dev->SetTextureStageState(0, D3DTSS_COLORARG1, TA_DIFFUSE);
    dev->SetTextureStageState(0, D3DTSS_ALPHAOP, TOP_SELECTARG1); dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, TA_DIFFUSE);
    dev->SetTexture(0, nullptr); dev->SetVertexShader(FVF_XYZRHW | FVF_DIFFUSE);
    unsigned c = 0x80000000u | ((g_frames / 2) % 2 ? 0x0040C040u : 0x00208020u);   // half-transparent green, blinking
    Vtx q[4] = { { 8, 8, 0, 1, c }, { 40, 8, 0, 1, c }, { 8, 24, 0, 1, c }, { 40, 24, 0, 1, c } };
    dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, q, sizeof(Vtx));
    dev->EndScene();
  }
  for (unsigned i = 0; i < sizeof rs / sizeof *rs; i++) dev->SetRenderState(rs[i], rsv[i]);
  for (unsigned i = 0; i < sizeof ts / sizeof *ts; i++) dev->SetTextureStageState(0, ts[i], tsv[i]);
  dev->SetVertexShader(vs); dev->SetTexture(0, tex);
  if (tex) tex->Release();                         // GetTexture added a reference
}

extern "C" __declspec(dllexport) int xikit_mod_init(const XikitHost *host) {
  g_host = host;
  return XI_HOOK(host, CDx::DrawFadeQuad, my_DrawFadeQuad, prev_DrawFadeQuad);
}
