// d8_ff.cpp -- d3d8on11: a draw, decided. Direct3D 8's fixed-function state at the draw becomes the shader keys
// (VSKey: the vertex work -- layout, lighting, material sources, texture coordinates, the fog coordinate; PSKey: the
// pixel work -- the stages, alpha test, fog, specular), the constants, the D3D11 state descriptions and the bindings,
// in one draw packet (XDraw, d8_proto.h) that the executor (d8_exec.cpp) draws. Vertex and index data of ...UP draws,
// and the indices a triangle fan becomes, go with the packet, so the packet holds everything the draw reads but the
// buffers and textures, which the executor already has.
//
// What each behaviour should be comes from the DirectX 8 SDK reference (render states, texture stage states,
// lighting, fog, texture coordinate processing). Where the reference leaves a choice open, the choice is Wine 9.0's
// wined3d, named where used; the picture test (test/test_d3d8on11.py, mode scenes) compares with Wine's d3d8.
#include "d8.h"
#include <math.h>
#include <functional>
#include <string>

static int vsize(uint8_t f) { static const int n[] = {4, 8, 12, 16, 4, 4, 4, 8}; return n[f]; }

// ---- the vertex layout of the current shader (an FVF code, or a declaration made without code) --------------------------------
static bool layout_fvf(DWORD fvf, VSKey &k, UINT &stride) {
  int n = 0; UINT o = 0;
  auto add = [&](uint8_t sem, uint8_t idx, uint8_t fmt) { k.el[n] = {sem, idx, 0, fmt, (uint16_t)o}; o += vsize(fmt); n++; };
  switch (fvf & D3DFVF_POSITION_MASK) {
  case D3DFVF_XYZ: add(SE_POS, 0, V_F3); break;
  case D3DFVF_XYZRHW: add(SE_POST, 0, V_F4); k.rhw = 1; break;
  case D3DFVF_XYZB1: case D3DFVF_XYZB2: case D3DFVF_XYZB3: case D3DFVF_XYZB4: case D3DFVF_XYZB5:
    d8_once("fvfblend", "an FVF with blend weights (0x%lX): vertex blending not implemented (draw skipped)", (unsigned long)fvf); return false;
  default: return false;
  }
  if (fvf & D3DFVF_NORMAL) add(SE_NORMAL, 0, V_F3);
  if (fvf & D3DFVF_PSIZE) add(SE_PSIZE, 0, V_F1);
  if (fvf & D3DFVF_DIFFUSE) add(SE_COLOR, 0, V_COLOR);
  if (fvf & D3DFVF_SPECULAR) add(SE_COLOR, 1, V_COLOR);
  int ntex = (fvf & D3DFVF_TEXCOUNT_MASK) >> D3DFVF_TEXCOUNT_SHIFT;
  for (int t = 0; t < ntex; t++) {
    int sz = (fvf >> (16 + 2 * t)) & 3;                      // D3DFVF_TEXTUREFORMAT2 0, 3 1, 4 2, 1 3
    static const uint8_t f[] = {V_F2, V_F3, V_F4, V_F1};
    add(SE_TEX, (uint8_t)t, f[sz]);
  }
  k.nel = (uint8_t)n; stride = o;
  return true;
}
static bool layout_decl(const std::vector<DWORD> &d, VSKey &k) {
  int n = 0, stream = 0; UINT off[16] = {};
  for (size_t i = 0; i < d.size(); i++) {
    DWORD t = d[i], ty = (t & D3DVSD_TOKENTYPEMASK) >> D3DVSD_TOKENTYPESHIFT;
    if (t == D3DVSD_END()) break;
    if (ty == D3DVSD_TOKEN_STREAM) { stream = t & D3DVSD_STREAMNUMBERMASK; continue; }
    if (ty == D3DVSD_TOKEN_STREAMDATA) {
      if (t & D3DVSD_DATALOADTYPEMASK) {                     // a skip
        off[stream] += 4 * ((t & D3DVSD_SKIPCOUNTMASK) >> D3DVSD_SKIPCOUNTSHIFT); continue;
      }
      DWORD dt = (t & D3DVSD_DATATYPEMASK) >> D3DVSD_DATATYPESHIFT, reg = t & D3DVSD_VERTEXREGMASK;
      static const uint8_t f[] = {V_F1, V_F2, V_F3, V_F4, V_COLOR, V_UBYTE4, V_SHORT2, V_SHORT4};
      if (dt > 7 || n >= 16) return false;
      // the fixed-function register meanings (d3d8types.h D3DVSDE_*): 0 position, 1 blend weight, 3 normal, 4 point
      // size, 5 diffuse, 6 specular, 7..14 texture coordinates,
      uint8_t sem, idx = 0;
      // and 15 / 16 the second position and normal, read only when D3DRS_VERTEXBLEND is D3DVBF_TWEENING (DirectX 8.1,
      // "Vertex Tweening"). Every declaration the game was traced using maps register 15 (data/d3d8_scope.json)
      if (reg == 15) { sem = SE_POS; idx = 1; } else if (reg == 16) { sem = SE_NORMAL; idx = 1; } else
      if (reg == 0) sem = SE_POS; else if (reg == 1) sem = SE_WEIGHT; else if (reg == 3) sem = SE_NORMAL; else if (reg == 4) sem = SE_PSIZE;
      else if (reg == 5) sem = SE_COLOR; else if (reg == 6) { sem = SE_COLOR; idx = 1; } else if (reg >= 7 && reg <= 14) { sem = SE_TEX; idx = (uint8_t)(reg - 7); }
      else { d8_once("declreg", "a declaration register %lu without code: not implemented (draw skipped)", (unsigned long)reg); return false; }
      k.el[n++] = {sem, idx, (uint8_t)stream, f[dt], (uint16_t)off[stream]};
      off[stream] += vsize(f[dt]);
      continue;
    }
    if (ty == D3DVSD_TOKEN_CONSTMEM) { i += 4 * ((t & D3DVSD_CONSTCOUNTMASK) >> D3DVSD_CONSTCOUNTSHIFT); continue; }
    if (ty == D3DVSD_TOKEN_TESSELLATOR) { d8_once("tess", "a declaration with tessellation: not implemented (draw skipped)"); return false; }
    if (ty == D3DVSD_TOKEN_EXT) { i += (t & D3DVSD_EXTCOUNTMASK) >> D3DVSD_EXTCOUNTSHIFT; continue; }
  }
  k.nel = (uint8_t)n;
  return true;
}

// ---- state objects ------------------------------------------------------------------------------------------------------------
static D3D11_BLEND blend(DWORD b, bool alpha) {
  switch (b) {
  case D3DBLEND_ZERO: return D3D11_BLEND_ZERO; case D3DBLEND_ONE: return D3D11_BLEND_ONE;
  case D3DBLEND_SRCCOLOR: return alpha ? D3D11_BLEND_SRC_ALPHA : D3D11_BLEND_SRC_COLOR;
  case D3DBLEND_INVSRCCOLOR: return alpha ? D3D11_BLEND_INV_SRC_ALPHA : D3D11_BLEND_INV_SRC_COLOR;
  case D3DBLEND_SRCALPHA: return D3D11_BLEND_SRC_ALPHA; case D3DBLEND_INVSRCALPHA: return D3D11_BLEND_INV_SRC_ALPHA;
  case D3DBLEND_DESTALPHA: return D3D11_BLEND_DEST_ALPHA; case D3DBLEND_INVDESTALPHA: return D3D11_BLEND_INV_DEST_ALPHA;
  case D3DBLEND_DESTCOLOR: return alpha ? D3D11_BLEND_DEST_ALPHA : D3D11_BLEND_DEST_COLOR;
  case D3DBLEND_INVDESTCOLOR: return alpha ? D3D11_BLEND_INV_DEST_ALPHA : D3D11_BLEND_INV_DEST_COLOR;
  case D3DBLEND_SRCALPHASAT: return D3D11_BLEND_SRC_ALPHA_SAT;
  }
  return D3D11_BLEND_ONE;
}
static D3D11_COMPARISON_FUNC cmpf(DWORD c) { return c >= 1 && c <= 8 ? (D3D11_COMPARISON_FUNC)c : D3D11_COMPARISON_ALWAYS; }   // same numbering
static D3D11_STENCIL_OP sop(DWORD o) { return o >= 1 && o <= 8 ? (D3D11_STENCIL_OP)o : D3D11_STENCIL_OP_KEEP; }               // same numbering
static D3D11_TEXTURE_ADDRESS_MODE addr(DWORD a) {
  switch (a) { case D3DTADDRESS_MIRROR: return D3D11_TEXTURE_ADDRESS_MIRROR; case D3DTADDRESS_CLAMP: return D3D11_TEXTURE_ADDRESS_CLAMP;
  case D3DTADDRESS_BORDER: return D3D11_TEXTURE_ADDRESS_BORDER; case D3DTADDRESS_MIRRORONCE: return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE; }
  return D3D11_TEXTURE_ADDRESS_WRAP;
}
static void argb4(D3DCOLOR c, float *f) {
  f[0] = ((c >> 16) & 255) / 255.0f; f[1] = ((c >> 8) & 255) / 255.0f; f[2] = (c & 255) / 255.0f; f[3] = (c >> 24) / 255.0f;
}
static void mmul(const D3DMATRIX &a, const D3DMATRIX &b, float *o) {
  for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) { float s = 0; for (int k = 0; k < 4; k++) s += a.m[i][k] * b.m[k][j]; o[i * 4 + j] = s; }
}
static void inv_transpose3(const float *m, float *o) {      // the upper 3x3's inverse transpose, in a 4x4
  float a = m[0], b = m[1], c = m[2], d = m[4], e = m[5], f = m[6], g = m[8], h = m[9], i = m[10];
  float A = e * i - f * h, B = -(d * i - f * g), C = d * h - e * g, det = a * A + b * B + c * C;
  float r = det != 0 ? 1 / det : 0;
  float inv[9] = {A * r, -(b * i - c * h) * r, (b * f - c * e) * r, B * r, (a * i - c * g) * r, -(a * f - c * d) * r, C * r, -(a * h - b * g) * r, (a * e - b * d) * r};
  memset(o, 0, 64);
  for (int x = 0; x < 3; x++) for (int y = 0; y < 3; y++) o[x * 4 + y] = inv[y * 3 + x];   // transpose
  o[15] = 1;
}

// ---- a draw -------------------------------------------------------------------------------------------------------------------
static UINT verts_for(D3DPRIMITIVETYPE t, UINT n) {
  switch (t) { case D3DPT_POINTLIST: return n; case D3DPT_LINELIST: return 2 * n; case D3DPT_LINESTRIP: return n + 1;
  case D3DPT_TRIANGLELIST: return 3 * n; default: return n + 2; }
}
static D3D11_PRIMITIVE_TOPOLOGY topo(D3DPRIMITIVETYPE t) {
  switch (t) { case D3DPT_POINTLIST: return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST; case D3DPT_LINELIST: return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
  case D3DPT_LINESTRIP: return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP; case D3DPT_TRIANGLESTRIP: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
  default: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; }   // lists, and fans converted to lists
}

HRESULT Dev::ff_draw(int kind, D3DPRIMITIVETYPE type, UINT prims, UINT startVertex, UINT minIndex, UINT numVerts, UINT startIndex,
                     const void *upv, UINT upStride, const void *upi, D3DFORMAT upiFmt) {
  if (!prims) return D3D_OK;
  XDraw D; memset(&D, 0, sizeof D);
  VSKey &vk = D.vk; PSKey &pk = D.pk;
  UINT fvfStride = 0;
  if (st.vs & 1) {
    size_t i = st.vs >> 1;
    if (i >= vshaders.size() || !vshaders[i].used) return D3DERR_INVALIDCALL;
    if (!vshaders[i].code.empty()) { d8_once("vsprog", "a vertex shader with code: not translated (draw skipped); the game was never seen creating one"); return D3D_OK; }
    if (!layout_decl(vshaders[i].decl, vk)) return D3D_OK;
  } else if (!layout_fvf(st.vs, vk, fvfStride)) {
    d8_once(("fvf" + std::to_string(st.vs)).c_str(), "FVF 0x%lX: not implemented (draw skipped)", (unsigned long)st.vs);
    return D3D_OK;
  }
  if (st.rs[D3DRS_CLIPPLANEENABLE]) d8_once("clipplanes", "D3DRS_CLIPPLANEENABLE 0x%lX: user clip planes are not applied", (unsigned long)st.rs[D3DRS_CLIPPLANEENABLE]);
  if (st.ps) { d8_once("psprog", "a pixel shader (the game's own program): not translated yet (draw skipped)"); return D3D_OK; }
  const DWORD *r = st.rs;
  // ---- vertex key
  bool hasN = false, hasD = false, hasS = false;
  for (int i = 0; i < vk.nel; i++) { hasN |= vk.el[i].sem == SE_NORMAL; hasD |= vk.el[i].sem == SE_COLOR && vk.el[i].idx == 0; hasS |= vk.el[i].sem == SE_COLOR && vk.el[i].idx == 1; }
  vk.lighting = !vk.rhw && r[D3DRS_LIGHTING];
  // tweening: D3DRS_VERTEXBLEND = D3DVBF_TWEENING (255) with a second position or normal in the declaration
  { bool p2 = false; for (int e = 0; e < vk.nel; e++) p2 |= (vk.el[e].sem == SE_POS || vk.el[e].sem == SE_NORMAL) && vk.el[e].idx == 1;
    vk.tween = !vk.rhw && r[D3DRS_VERTEXBLEND] == 255 && p2;
    if (r[D3DRS_VERTEXBLEND] && r[D3DRS_VERTEXBLEND] != 255 && !vk.rhw)
      d8_once("vblend", "D3DRS_VERTEXBLEND %lu (vertex blending by weights): not implemented, drawn unblended", (unsigned long)r[D3DRS_VERTEXBLEND]); }
  int lidx[8], nl = 0;
  if (vk.lighting) for (size_t i = 0; i < st.light.size() && nl < 8; i++) if (st.lightOn[i]) { lidx[nl] = (int)i; vk.ltype[nl] = (uint8_t)st.light[i].Type; nl++; }
  vk.nlights = (uint8_t)nl;
  if (vk.lighting && r[D3DRS_COLORVERTEX]) {
    const D3DRENDERSTATETYPE ms[4] = {D3DRS_DIFFUSEMATERIALSOURCE, D3DRS_SPECULARMATERIALSOURCE, D3DRS_AMBIENTMATERIALSOURCE, D3DRS_EMISSIVEMATERIALSOURCE};
    for (int j = 0; j < 4; j++) {
      DWORD v = r[ms[j]];                                  // a source the vertex lacks falls back to the material
      vk.matsrc[j] = v == D3DMCS_COLOR1 && hasD ? 1 : v == D3DMCS_COLOR2 && hasS ? 2 : 0;
    }
  }
  vk.specular = vk.lighting && r[D3DRS_SPECULARENABLE]; vk.localviewer = r[D3DRS_LOCALVIEWER] != 0; vk.normalize = r[D3DRS_NORMALIZENORMALS] != 0;
  bool fog = r[D3DRS_FOGENABLE] != 0;
  DWORD tmode = r[D3DRS_FOGTABLEMODE], vmode = r[D3DRS_FOGVERTEXMODE];
  bool coordfog = tmode == D3DFOG_NONE && (vk.rhw || vmode == D3DFOG_NONE);
  const D3DMATRIX &P = st.xf[D3DTS_PROJECTION];
  if (!fog) vk.fogsrc = FOG_OFF;
  else if (coordfog) vk.fogsrc = FOG_SPECA;
  // Table (pixel) fog's depth, by the DirectX 8.1 reference ("Pixel Fog"): eye-relative depth, w, on a device with
  // D3DPRASTERCAPS_WFOG (ours: dgVoodoo's caps, RasterCaps 0x00736191, have it), unless the projection matrix's
  // fourth column is [0,0,0,1], then device z. "Direct3D takes the reciprocal of the RHW element ... to reproduce true
  // w", and uses the set projection matrix "even if it does not use the Direct3D transformation pipeline", so
  // pre-transformed vertices follow the same rule (Wine 9.0 uses z for them). For transformed vertices w is camera z
  // with a "W-friendly" projection, the only kind the reference says fog works with.
  else if (tmode != D3DFOG_NONE) {
    bool affine = P._14 == 0 && P._24 == 0 && P._34 == 0 && P._44 == 1;
    vk.fogsrc = vk.rhw ? (affine ? FOG_RHWZ : FOG_RHWW) : affine ? FOG_CLIPZ : FOG_EYEZ;
  }
  else { vk.fogsrc = r[D3DRS_RANGEFOGENABLE] ? FOG_RANGE : FOG_EYEZ;
    vk.vfog = vmode == D3DFOG_EXP ? 2 : vmode == D3DFOG_EXP2 ? 3 : vmode == D3DFOG_LINEAR ? 1 : 0; }
  // vertex fog's factor arrives made, as 255 * factor: read like the specular-alpha coordinate
  if (fog) { DWORD m = coordfog || vk.vfog ? D3DFOG_LINEAR : tmode != D3DFOG_NONE ? tmode : vmode;
    pk.fog = m == D3DFOG_EXP ? 2 : m == D3DFOG_EXP2 ? 3 : m == D3DFOG_LINEAR ? 1 : 0; }
  (void)hasN;
  // ---- the stages: textures, coordinates, combiners
  int ns = 0;
  for (; ns < 8 && st.tss[ns][D3DTSS_COLOROP] != D3DTOP_DISABLE; ns++) {
    // "When a texture stage has D3DTSS_COLORARG1 equal to D3DTA_TEXTURE and the texture pointer for the stage is NULL,
    // this stage and all stages after it are not processed" (DirectX 8.1, Texture Blending). Measured on the reference
    // device (t8 random, 14 Oct): whatever the op (SELECTARG2 too), alpha included, stage 0 so skipped draws the diffuse
    // colour and alpha (as DISABLE does); D3DTA_TEXTURE with a modifier (COMPLEMENT, ALPHAREPLICATE) is not "equal"
    if (!st.tex[ns] && st.tss[ns][D3DTSS_COLORARG1] == D3DTA_TEXTURE) break;
    const DWORD *t = st.tss[ns]; auto &s = pk.s[ns];
    s.cop = (uint8_t)t[D3DTSS_COLOROP]; s.ca1 = (uint8_t)t[D3DTSS_COLORARG1]; s.ca2 = (uint8_t)t[D3DTSS_COLORARG2]; s.ca0 = (uint8_t)t[D3DTSS_COLORARG0];
    s.aop = (uint8_t)t[D3DTSS_ALPHAOP]; s.aa1 = (uint8_t)t[D3DTSS_ALPHAARG1]; s.aa2 = (uint8_t)t[D3DTSS_ALPHAARG2]; s.aa0 = (uint8_t)t[D3DTSS_ALPHAARG0];
    s.res = (uint8_t)(t[D3DTSS_RESULTARG] == D3DTA_TEMP ? D3DTA_TEMP : D3DTA_CURRENT);
    IDirect3DBaseTexture8 *tx = st.tex[ns];
    s.tex = !tx ? 0 : tx->GetType() == D3DRTYPE_CUBETEXTURE ? 2 : 1;
    DWORD tci = t[D3DTSS_TEXCOORDINDEX], tf = t[D3DTSS_TEXTURETRANSFORMFLAGS];
    auto &c = vk.tc[ns];
    c.src = (uint8_t)(tci >> 16 & 3); c.idx = (uint8_t)(tci & 7); c.count = (uint8_t)(tf & 7); c.proj = (tf & D3DTTFF_PROJECTED) ? 1 : 0;
    // no texture transform with COUNT1 or on pre-transformed vertices (Wine 9.0 utils.c compute_texture_matrix)
    if (c.count <= 1 || vk.rhw) c.count = 0;
    // projected: the coordinates divided by the last one the count gives (the DirectX 8 reference, D3DTTFF_PROJECTED);
    // the game's traces never set it (data/d3d8_scope.json)
    s.tcn = c.proj && c.count ? c.count : 0;
    if (c.proj) d8_once("ttffproj", "D3DTTFF_PROJECTED: executed as the reference describes; not compared with Wine (the game was not seen using it)");
    if (s.cop == D3DTOP_BUMPENVMAP || s.cop == D3DTOP_BUMPENVMAPLUMINANCE || s.cop == D3DTOP_PREMODULATE) {
      d8_once("tssop", "texture stage op %u: not implemented (treated as SELECTARG1)", s.cop); s.cop = D3DTOP_SELECTARG1;
    }
  }
  pk.nstages = (uint8_t)ns; vk.ntc = (uint8_t)ns;
  if (ns == 0) { pk.nstages = 0; }
  pk.specadd = r[D3DRS_SPECULARENABLE] != 0;
  pk.flat = vk.flat = r[D3DRS_SHADEMODE] == D3DSHADE_FLAT;
  pk.alphatest = r[D3DRS_ALPHATESTENABLE] ? (uint8_t)r[D3DRS_ALPHAFUNC] : 0;
  if (pk.alphatest == D3DCMP_ALWAYS) pk.alphatest = 0;
  // ---- constants
  {
    VSConst &c = D.vc;
    D3DMATRIX wv; float t[16];
    mmul(st.xf[D3DTS_WORLD], st.xf[D3DTS_VIEW], t); memcpy(&wv, t, 64);
    mmul(wv, st.xf[D3DTS_PROJECTION], c.wvp); memcpy(c.wv, t, 64); inv_transpose3(t, c.wvn); memcpy(c.proj, &st.xf[D3DTS_PROJECTION], 64);
    for (int i = 0; i < 8; i++) memcpy(c.tex[i], &st.xf[D3DTS_TEXTURE0 + i], 64);
    c.vp[0] = (float)st.vp.X; c.vp[1] = (float)st.vp.Y; c.vp[2] = (float)st.vp.Width; c.vp[3] = (float)st.vp.Height;
    // D3D8 puts pixel centres on integer screen coordinates, D3D11 on .5: geometry moves half a pixel right and down
    // (Wine 9.0 utils.c get_projection_matrix does the same for GL; the picture test compares the result)
    c.fix[0] = 1.0f / (float)st.vp.Width; c.fix[1] = -1.0f / (float)st.vp.Height;
    memcpy(&c.fix[2], &r[D3DRS_TWEENFACTOR], 4);
    argb4(r[D3DRS_AMBIENT], c.ambient);
    memcpy(c.mdiff, &st.mat.Diffuse, 16); memcpy(c.mamb, &st.mat.Ambient, 16); memcpy(c.mspec, &st.mat.Specular, 16);
    memcpy(c.memis, &st.mat.Emissive, 16); c.mpow[0] = st.mat.Power;
    const D3DMATRIX &V = st.xf[D3DTS_VIEW];
    for (int i = 0; i < nl; i++) {
      const D3DLIGHT8 &L = st.light[lidx[i]];
      memcpy(c.ldiff[i], &L.Diffuse, 16); memcpy(c.lspec[i], &L.Specular, 16); memcpy(c.lamb[i], &L.Ambient, 16);
      for (int j = 0; j < 3; j++) {                          // lights are given in world space; the shader works in eye space
        c.lpos[i][j] = L.Position.x * V.m[0][j] + L.Position.y * V.m[1][j] + L.Position.z * V.m[2][j] + V.m[3][j];
        c.ldir[i][j] = L.Direction.x * V.m[0][j] + L.Direction.y * V.m[1][j] + L.Direction.z * V.m[2][j];
      }
      float dl = sqrtf(c.ldir[i][0] * c.ldir[i][0] + c.ldir[i][1] * c.ldir[i][1] + c.ldir[i][2] * c.ldir[i][2]);
      if (dl > 0) for (int j = 0; j < 3; j++) c.ldir[i][j] /= dl;
      c.latt[i][0] = L.Range; c.latt[i][1] = L.Attenuation0; c.latt[i][2] = L.Attenuation1; c.latt[i][3] = L.Attenuation2;
      c.lspot[i][0] = L.Falloff; c.lspot[i][1] = cosf(L.Theta / 2); c.lspot[i][2] = cosf(L.Phi / 2);
    }
    float fs, fe, fdn; memcpy(&fs, &r[D3DRS_FOGSTART], 4); memcpy(&fe, &r[D3DRS_FOGEND], 4); memcpy(&fdn, &r[D3DRS_FOGDENSITY], 4);
    c.fog[0] = fs; c.fog[1] = fe; c.fog[2] = fdn; c.fog[3] = fe != fs ? 1 / (fe - fs) : 0;
    PSConst &p = D.pc;
    argb4(r[D3DRS_TEXTUREFACTOR], p.tfactor); argb4(r[D3DRS_FOGCOLOR], p.fogcolor);
    // start, end, density, scale = 1 / (end - start), as Wine's get_fog_start_end and shader_glsl_load_fog_uniform:
    // the specular-alpha coordinate (and vertex fog's factor) reads with start 255, end 0. Vertex linear fog with
    // start == end fogs everything (Wine's choice, kept: the vertex shader's scale is then 0, so the factor is 0)
    { float s0 = fs, e0 = fe;
      if (coordfog || vk.vfog) { s0 = 255; e0 = 0; }
      p.fog[0] = s0; p.fog[1] = e0; p.fog[2] = fdn; p.fog[3] = 1.0f / (e0 - s0); }
    p.aref[0] = (r[D3DRS_ALPHAREF] & 255) / 255.0f;
  }
  // ---- fixed state: descriptions (the executor keeps the D3D11 objects, cached by them)
  {
    D3D11_BLEND_DESC &bd = D.blend; auto &t = bd.RenderTarget[0];
    t.BlendEnable = r[D3DRS_ALPHABLENDENABLE] ? TRUE : FALSE;
    DWORD sb = r[D3DRS_SRCBLEND], db = r[D3DRS_DESTBLEND];
    if (sb == D3DBLEND_BOTHSRCALPHA) { sb = D3DBLEND_SRCALPHA; db = D3DBLEND_INVSRCALPHA; }
    if (sb == D3DBLEND_BOTHINVSRCALPHA) { sb = D3DBLEND_INVSRCALPHA; db = D3DBLEND_SRCALPHA; }
    t.SrcBlend = blend(sb, false); t.DestBlend = blend(db, false); t.SrcBlendAlpha = blend(sb, true); t.DestBlendAlpha = blend(db, true);
    DWORD bo = r[D3DRS_BLENDOP]; t.BlendOp = t.BlendOpAlpha = bo >= 1 && bo <= 5 ? (D3D11_BLEND_OP)bo : D3D11_BLEND_OP_ADD;
    t.RenderTargetWriteMask = (UINT8)(r[D3DRS_COLORWRITEENABLE] & 0xF);
    D3D11_DEPTH_STENCIL_DESC &dd = D.depth;
    dd.DepthEnable = r[D3DRS_ZENABLE] && curDS ? TRUE : FALSE;
    dd.DepthWriteMask = r[D3DRS_ZWRITEENABLE] ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
    dd.DepthFunc = cmpf(r[D3DRS_ZFUNC]);
    dd.StencilEnable = r[D3DRS_STENCILENABLE] && curDS ? TRUE : FALSE;
    dd.StencilReadMask = (UINT8)r[D3DRS_STENCILMASK]; dd.StencilWriteMask = (UINT8)r[D3DRS_STENCILWRITEMASK];
    dd.FrontFace = {sop(r[D3DRS_STENCILFAIL]), sop(r[D3DRS_STENCILZFAIL]), sop(r[D3DRS_STENCILPASS]), cmpf(r[D3DRS_STENCILFUNC])};
    dd.BackFace = dd.FrontFace;
    D.stencil_ref = r[D3DRS_STENCILREF] & 255;
    D3D11_RASTERIZER_DESC &rd = D.raster;
    rd.FillMode = r[D3DRS_FILLMODE] == D3DFILL_WIREFRAME ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
    // D3D8 culls by winding: CCW (the default) culls counter-clockwise triangles; D3D11's default front is clockwise
    rd.CullMode = r[D3DRS_CULLMODE] == D3DCULL_NONE ? D3D11_CULL_NONE : r[D3DRS_CULLMODE] == D3DCULL_CW ? D3D11_CULL_FRONT : D3D11_CULL_BACK;
    rd.DepthClipEnable = TRUE;
    // D3D8 ZBIAS (0..16): a higher value draws in front (the DirectX 8 reference); by how much is the driver's. This
    // takes Wine's d3d8 (9.0: directx.c WINED3D_LEGACY_DEPTH_BIAS, state.c depthbias): -ZBIAS in units of the depth
    // buffer's resolution and -ZBIAS times the depth slope. What the owner's driver under dgVoodoo does is not known.
    rd.DepthBias = -(INT)r[D3DRS_ZBIAS]; rd.SlopeScaledDepthBias = -(float)r[D3DRS_ZBIAS];
    D.viewport = {(float)st.vp.X, (float)st.vp.Y, (float)st.vp.Width, (float)st.vp.Height, st.vp.MinZ, st.vp.MaxZ};
    D.nstages = (uint32_t)ns;
    for (int i = 0; i < ns; i++) {
      if (!pk.s[i].tex) continue;
      IDirect3DBaseTexture8 *tx = st.tex[i];
      const TexCore &tc = tx->GetType() == D3DRTYPE_CUBETEXTURE ? ((CubeTex *)(IDirect3DCubeTexture8 *)tx)->c : ((Tex *)(IDirect3DTexture8 *)tx)->c;
      D.stage[i].tex = tc.id;
        const DWORD *t = st.tss[i];
        D3D11_SAMPLER_DESC &sd = D.stage[i].sampler;
        bool aniso = t[D3DTSS_MINFILTER] == D3DTEXF_ANISOTROPIC || t[D3DTSS_MAGFILTER] == D3DTEXF_ANISOTROPIC;
        int mn = t[D3DTSS_MINFILTER] >= D3DTEXF_LINEAR ? 1 : 0, mg = t[D3DTSS_MAGFILTER] >= D3DTEXF_LINEAR ? 1 : 0, mp = t[D3DTSS_MIPFILTER] >= D3DTEXF_LINEAR ? 1 : 0;
        sd.Filter = aniso ? D3D11_FILTER_ANISOTROPIC : (D3D11_FILTER)((mn ? 0x10 : 0) | (mg ? 0x4 : 0) | (mp ? 0x1 : 0));
        sd.AddressU = addr(t[D3DTSS_ADDRESSU]); sd.AddressV = addr(t[D3DTSS_ADDRESSV]); sd.AddressW = addr(t[D3DTSS_ADDRESSW]);
        float bc[4]; argb4(t[D3DTSS_BORDERCOLOR], bc); memcpy(sd.BorderColor, bc, 16);
        sd.MaxAnisotropy = t[D3DTSS_MAXANISOTROPY] ? (t[D3DTSS_MAXANISOTROPY] > 16 ? 16 : t[D3DTSS_MAXANISOTROPY]) : 1;
        memcpy(&sd.MipLODBias, &t[D3DTSS_MIPMAPLODBIAS], 4);
        // The levels a stage may read: from MAXMIPLEVEL (the most detailed level used) down, or only that level with
        // MIPFILTER NONE ("disables mipmapping"; the Direct3D 9 reference, D3DTEXTUREFILTERTYPE, which keeps D3D8's
        // states). They go to the executor as a view of those levels, not as the sampler's LOD clamp: MAGFILTER and
        // MINFILTER apply when the texture is magnified or minified "respectively" (same page), and a clamp to one
        // level can decide that instead (it did on Mesa: a minified texture was drawn with MAGFILTER; test/random_cases.py)
        UINT first = t[D3DTSS_MAXMIPLEVEL] < tc.levels ? t[D3DTSS_MAXMIPLEVEL] : tc.levels - 1;
        D.stage[i].first_level = first; D.stage[i].nlevels = t[D3DTSS_MIPFILTER] == D3DTEXF_NONE ? 1 : tc.levels - first;
        sd.MinLOD = 0; sd.MaxLOD = D3D11_FLOAT32_MAX;
        sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
    }
  }
  // ---- vertices and indices
  UINT nv = verts_for(type, prims);
  bool fan = type == D3DPT_TRIANGLEFAN;
  D.topology = topo(type);
  std::vector<uint32_t> fanIdx;
  auto fan_list = [&](UINT first, UINT n, const std::function<uint32_t(UINT)> &ix) {
    fanIdx.resize(3 * n);
    // each triangle starts at its first non-spoke vertex, so flat shading takes that vertex's colour: "For a triangle
    // fan, the first vertex of the triangle i is vertex i + 1" (DirectX 8.1, D3DSHADEMODE); a rotation keeps the winding
    for (UINT k = 0; k < n; k++) { fanIdx[3 * k] = ix(first + k + 1); fanIdx[3 * k + 1] = ix(first + k + 2); fanIdx[3 * k + 2] = ix(first); }
  };
  const void *idata = nullptr;
  if (kind == 2 || kind == 3) {                              // from the caller's memory: it goes with the packet
    D.up_stride = upStride; D.up_vbytes = (kind == 2 ? nv : numVerts + minIndex) * upStride;
    if (kind == 2) {
      if (fan) { fan_list(0, prims, [](UINT i) { return (uint32_t)i; }); D.kind = XD_UP_INDEXED; D.i32 = 1; D.count = (UINT)fanIdx.size(); }
      else { D.kind = XD_UP; D.count = nv; }
    } else {
      UINT isz = upiFmt == D3DFMT_INDEX32 ? 4 : 2;
      D.kind = XD_UP_INDEXED;
      if (fan) { fan_list(0, prims, [&](UINT i) { return isz == 4 ? ((const uint32_t *)upi)[i] : (uint32_t)((const uint16_t *)upi)[i]; }); D.i32 = 1; D.count = (UINT)fanIdx.size(); }
      else { idata = upi; D.ibytes = nv * isz; D.i32 = isz == 4; D.count = nv; }
    }
    if (fan) { idata = fanIdx.data(); D.ibytes = (UINT)fanIdx.size() * 4; }
    uint8_t *w = link->begin(X_DRAW, sizeof D + D.up_vbytes + D.ibytes);
    memcpy(w, &D, sizeof D); memcpy(w + sizeof D, upv, D.up_vbytes); if (D.ibytes) memcpy(w + sizeof D + D.up_vbytes, idata, D.ibytes);
    link->end();
    // D3D8: after a ...UP draw, stream 0 and the indices are unset
    if (st.stream[0]) { st.stream[0]->Release(); st.stream[0] = nullptr; } st.stride[0] = 0;
    if (kind == 3 && st.ib) { st.ib->Release(); st.ib = nullptr; }
    return D3D_OK;
  }
  for (int i = 0; i < 16; i++) {
    bool used = false; for (int e = 0; e < vk.nel; e++) used |= vk.el[e].stream == i;
    if (!used) continue;
    VB *b = st.stream[i];
    if (!b || !b->c.id) { d8_once("nostream", "a draw reads a stream with no vertex buffer (skipped)"); return D3D_OK; }
    // "DrawPrimitive calls will not succeed with any outstanding lock count on any currently set vertex buffer"
    // (DirectX 8.1, IDirect3DVertexBuffer8::Lock)
    if (b->c.locks) { d8_once("drawlocked", "a draw with a locked vertex buffer set: failed, as the reference says"); return D3DERR_INVALIDCALL; }
    D.stream[i].buf = b->c.id; D.stream[i].stride = st.stride[i];
  }
  if (kind == 0) {
    if (fan) { fan_list(startVertex, prims, [](UINT i) { return (uint32_t)i; }); D.kind = XD_INLINE_INDEXED; D.i32 = 1; D.count = (UINT)fanIdx.size(); D.base = 0; }
    else { D.kind = XD_DRAW; D.count = nv; D.start = startVertex; }
  } else {
    IB *ib = st.ib;
    if (!ib || !ib->c.id) { d8_once("noib", "an indexed draw with no index buffer (skipped)"); return D3D_OK; }
    UINT isz = ib->c.fmt == D3DFMT_INDEX32 ? 4 : 2;
    D.base = st.baseVertex;
    if (fan) {
      const uint8_t *ic = ib->c.cpu.data();
      fan_list(startIndex, prims, [&](UINT i) { return isz == 4 ? ((const uint32_t *)ic)[i] : (uint32_t)((const uint16_t *)ic)[i]; });
      D.kind = XD_INLINE_INDEXED; D.i32 = 1; D.count = (UINT)fanIdx.size();
    } else { D.kind = XD_INDEXED; D.ib = ib->c.id; D.ib32 = isz == 4; D.count = nv; D.start = startIndex; }
  }
  if (fan) D.ibytes = (UINT)fanIdx.size() * 4;
  link->put(X_DRAW, &D, sizeof D, fan ? fanIdx.data() : nullptr, D.ibytes);
  (void)minIndex; (void)fvfStride;
  return D3D_OK;
}
