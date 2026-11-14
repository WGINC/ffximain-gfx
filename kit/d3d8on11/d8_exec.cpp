// d8_exec.cpp -- d3d8on11's executor (d8_exec.h): Direct3D 11 objects by id, and the fixed-function pipeline as
// generated shaders. What each draw needs was decided on the D3D8 side (d8_ff.cpp: the keys, the constants, the state
// descriptions); here the keys become HLSL once, compiled with d3dcompiler_47's D3DCompile, cached, and drawn.
//
// What each behaviour should be comes from the DirectX 8 SDK reference (render states, texture stage states,
// lighting, fog, texture coordinate processing); where the reference leaves a choice open, from Wine 9.0's wined3d,
// named where used. The picture test (test/test_d3d8on11.py, mode scenes) compares with Wine's d3d8.
#include "d8_exec.h"
#include "d8_mip.h"
#include <algorithm>
#include <d3dcompiler.h>
#include <d3d11_1.h>
#include <d3d12.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
static float rhw_offset();   // (below; a test switch)
Exec12 *exec12_new(Exec &o); void exec12_delete(Exec12 *x);    // the Direct3D 12 executor (d8_exec12.cpp)
void exec12_run(Exec12 *x, uint16_t op, const uint8_t *p, uint32_t n, int32_t *hr, std::vector<uint8_t> *reply);

typedef HRESULT (WINAPI *PFN_Compile)(const void *, SIZE_T, const char *, const D3D_SHADER_MACRO *, ID3DInclude *,
                                       const char *, const char *, UINT, UINT, ID3DBlob **, ID3DBlob **);

DXGI_FORMAT vdxgi(uint8_t f) {
  switch (f) {
  case V_F1: return DXGI_FORMAT_R32_FLOAT; case V_F2: return DXGI_FORMAT_R32G32_FLOAT;
  case V_F3: return DXGI_FORMAT_R32G32B32_FLOAT; case V_F4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
  case V_COLOR: return DXGI_FORMAT_B8G8R8A8_UNORM; case V_UBYTE4: return DXGI_FORMAT_R8G8B8A8_UINT;
  case V_SHORT2: return DXGI_FORMAT_R16G16_SINT; default: return DXGI_FORMAT_R16G16B16A16_SINT;
  }
}

struct ExecFF {
  PFN_Compile compile = nullptr;
  std::unordered_map<std::string, ID3D11VertexShader *> vs; std::unordered_map<std::string, ID3D11InputLayout *> il;
  std::unordered_map<std::string, ID3D11PixelShader *> ps;
  std::unordered_map<std::string, ID3D11BlendState *> bs; std::unordered_map<std::string, ID3D11DepthStencilState *> ds;
  std::unordered_map<std::string, ID3D11RasterizerState *> rs; std::unordered_map<std::string, ID3D11SamplerState *> ss;
  ID3D11Buffer *vcb = nullptr, *pcb = nullptr, *upvb = nullptr, *upib = nullptr;
  UINT upvbPos = 0, upibPos = 0;
  enum { UPVB = 8 << 20, UPIB = 2 << 20 };
  // the batch's constant arenas (Exec::batch): a draw's constants at an offset that is a multiple of 16 constants
  // (VSSetConstantBuffers1: "pFirstConstant ... must be a multiple of 16 constants"; d3d11_1.h). Only where the
  // driver reports D3D11_FEATURE_DATA_D3D11_OPTIONS.ConstantBufferOffsetting; NOOVERWRITE on them only where it reports
  // MapNoOverwriteOnDynamicConstantBuffer (else each flush maps them with DISCARD from offset 0)
  ID3D11DeviceContext1 *ctx1 = nullptr; bool cbnoov = false;
  ID3D11Buffer *varena = nullptr, *parena = nullptr; UINT vaPos = 0, paPos = 0;
  enum { VSLOT = (sizeof(VSConst) + 255) & ~255u, PSLOT = (sizeof(PSConst) + 255) & ~255u, VAR = VSLOT * 4096, PAR = PSLOT * 4096 };
};
typedef ExecFF FF;
template <class K> static std::string kstr(const K &k) { return std::string((const char *)&k, sizeof k); }

// ---- HLSL: vertex shader --------------------------------------------------------------------------------------------------
static const char *VS_CB =
  "cbuffer C : register(b0) {\n"
  "  row_major float4x4 wvp, wv, wvn, proj; row_major float4x4 tex[8];\n"
  "  float4 vp, fix, ambient, mdiff, mamb, mspec, memis, mpow;\n"
  "  float4 ldiff[8], lspec[8], lamb[8], lpos[8], ldir[8], latt[8], lspot[8];\n"
  "  float4 fogp;\n"
  "};\n";
// the vertex shader's output, the pixel shader's input. Flat shading (D3DSHADE_FLAT): the colour and specular of the
// primitive's first vertex (the DirectX reference, D3DSHADEMODE); D3D11's nointerpolation takes the first vertex too.
static std::string io_struct(const char *name, bool flat) {
  const char *q = flat ? "nointerpolation " : "";
  // flat shading: "The color and specular component of the first vertex ... are used ... The specular alpha is
  // interpolated" (DirectX 8.1, D3DSHADEMODE), so the specular alpha also goes as an interpolated value of its own
  return std::string("struct ") + name + " { float4 pos : SV_Position; " + q + "float4 d0 : COLOR0; " + q +
         "float4 d1 : COLOR1; float d1a : SPECALPHA; float4 tc[8] : TEXCOORD0; float fog : FOG; };\n";
}
std::string gen_vs(const VSKey &k) {
  std::string s = VS_CB;
  s += "struct VI {\n";
  int has[8][8] = {};
  for (int i = 0; i < k.nel; i++) {
    const VElem &e = k.el[i]; const char *t[] = {"float", "float2", "float3", "float4", "float4", "uint4", "int2", "int4"};
    const char *sem[] = {"POSITION", "POSITIONT", "NORMAL", "COLOR", "TEXCOORD", "PSIZE", "BLENDWEIGHT", "SKIN"};
    if (e.sem == SE_SKIN) {                                  // gpu skinning: a source vertex (XSkinVtx)
      s += "  float4 ska : SKINA0; float4 skb : SKINB0; float3 skna : SKINNA0; float3 sknb : SKINNB0; uint2 ski : SKINI0;\n";
      has[SE_SKIN][0] = i + 1; continue;
    }
    char b[96]; snprintf(b, sizeof b, "  %s a%d : %s%d;\n", t[e.fmt], i, e.sem == SE_POST ? "POSITION" : sem[e.sem], e.sem == SE_POST ? 0 : e.idx);
    s += b; has[e.sem][e.idx] = i + 1;
  }
  s += "};\n" + io_struct("VO", k.flat);
  // gpu skinning (d8_skin.h): position = pa . bones[a] + pb . bones[b] with the w components on row 3, the normal the
  // same from rows 0 to 2: the sums Model_SkinVerticesCpu (0x1002F1D0) makes on the CPU
  const bool skin = has[SE_SKIN][0] != 0;
  if (skin) s = std::string("cbuffer B : register(b1) { row_major float4x4 bones[256]; };\n") + s;
  s += "VO main(VI v) {\n  VO o = (VO)0;\n";
  auto in = [&](int sem, int idx, const char *def) -> std::string {
    if (!has[sem][idx]) return def;
    const VElem &e = k.el[has[sem][idx] - 1]; std::string n = "v.a" + std::to_string(has[sem][idx] - 1);
    switch (e.fmt) {
    // "(value, 0.0, 0.0, 1.0)", "(value, value, 0.0, 1.0)", "(value, value, value, 1.0)"; SHORT2 as FLOAT2
    // (DirectX 8.1, D3DVSD_REG)
    case V_F1: return "float4(" + n + ", 0, 0, 1)"; case V_F2: case V_SHORT2: return "float4(" + n + ", 0, 1)";
    case V_F3: return "float4(" + n + ", 1)"; default: return "float4(" + n + ")";
    }
  };
  if (k.rhw) {
    // pre-transformed: screen x, y, z and 1/w; D3D8 puts pixel centres on integer coordinates, D3D11 on .5
    s += "  float4 p = " + in(SE_POST, 0, "float4(0,0,0,1)") + ";\n";
    s += "  float w = p.w != 0 ? 1.0 / p.w : 1.0;\n";
    // (a test only, D8ON11_TEST_RHW_OFFSET=x: another offset than 0.5, to compare with other implementations' pictures)
    { char off[32]; snprintf(off, sizeof off, "%.4f", rhw_offset());
      s += std::string("  float2 n = float2((p.x + ") + off + " - vp.x) / vp.z * 2 - 1, 1 - (p.y + " + off + " - vp.y) / vp.w * 2);\n"; }
    s += "  o.pos = float4(n * w, p.z * w, w);\n  float3 cp = float3(0, 0, p.z);\n";
  } else {
    if (skin) s += "  float4 p = float4(mul(v.ska, bones[v.ski.x]).xyz + mul(v.skb, bones[v.ski.y]).xyz, 1);\n";
    else s += "  float4 p = float4(" + in(SE_POS, 0, "float4(0,0,0,1)") + ".xyz, 1);\n";
    // tweening, before lighting: "POSITION = POSITION1 * (1.0 - TWEENFACTOR) + POSITION2 * TWEENFACTOR, and likewise for
    // normals" (DirectX 8.1, "Vertex Tweening"); fix.z holds D3DRS_TWEENFACTOR
    if (k.tween && has[SE_POS][1]) s += "  p.xyz = p.xyz * (1.0 - fix.z) + " + in(SE_POS, 1, "float4(0,0,0,1)") + ".xyz * fix.z;\n";
    s += "  o.pos = mul(p, wvp);\n  o.pos.xy += fix.xy * o.pos.w;\n";
    s += "  float3 cp = mul(p, wv).xyz;\n";
  }
  s += "  float4 vd = " + in(SE_COLOR, 0, "float4(1,1,1,1)") + "; float4 vs = " + in(SE_COLOR, 1, "float4(0,0,0,0)") + ";\n";
  bool needN = k.lighting || [&] { for (int i = 0; i < k.ntc; i++) if (k.tc[i].src == 1 || k.tc[i].src == 3) return true; return false; }();
  if (needN && !k.rhw) {
    std::string n0 = in(SE_NORMAL, 0, "float4(0,0,0,0)") + ".xyz";
    if (skin) n0 = "(mul(float4(v.skna, 0), bones[v.ski.x]).xyz + mul(float4(v.sknb, 0), bones[v.ski.y]).xyz)";
    if (k.tween && has[SE_NORMAL][1]) n0 = "(" + n0 + " * (1.0 - fix.z) + " + in(SE_NORMAL, 1, "float4(0,0,0,0)") + ".xyz * fix.z)";
    s += "  float3 nrm = mul(float4(" + n0 + ", 0), wvn).xyz;\n";
    // normals are normalized only with D3DRS_NORMALIZENORMALS, after the transform (DirectX 8.1, "Camera Space
    // Transformations"): a normal that is not unit length, or a scaled world matrix, changes the lighting otherwise
    s += k.normalize ? "  if (dot(nrm, nrm) > 0) nrm = normalize(nrm);\n" : "";
  }
  // Lighting, checked against the Microsoft DirectX 8.1 (C++) documentation, "Mathematics of Lighting" and its pages
  // (the owner's copy, 14 Oct): camera space, N = N*(wv^-1)^T and norm(N) only with NORMALIZENORMALS ("Camera Space
  // Transformations"); a directional light's Ldir = -norm(Ldir*vMatrix), a point or spot light's the normalized vector
  // to the light; Atten = 1/(att0 + att1*d + att2*d^2), and no effect beyond Range ("Attenuation and Spotlight
  // Terms"); Diffuse = sum[Vd*Ld*(N.Ldir)*Atten*Spot], Specular = Vs*sum[Ls*(N.H)^P*Atten*Spot] with
  // H = norm(norm(Cp - Vp) + Ldir) for LOCALVIEWER, else the viewer "infinitely distant on the z-axis", both clamped
  // after all lights; Emissive = Me; the material-source rules for each.
  //   - Ambient: DirectX 8.1 has Mc*[Ga + sum(Lai)], Direct3D 9 ("Ambient Lighting") Ca*[Ga + sum(Atten*Spot*Lai)];
  //     this takes Direct3D 9's. The two differ only for a point or spot light with an ambient colour: every SetLight
  //     in the game's traces (6080, 5180: 98,309 calls) has ambient 0, so the game's picture does not depend on it.
  //   - Not in either text: the clamp of each light's N.L at 0, the N.L > 0 and N.H > 0 conditions on specular,
  //     the spot factor's formula (in a "Spotlight Model" page the owner's copy lacks; the game's traces set no spot
  //     light). Those are Wine 9.0's.
  if (k.lighting && !k.rhw) {
    const char *src[] = {"", "vd", "vs"};
    std::string md = k.matsrc[0] ? src[k.matsrc[0]] : "mdiff", ms = k.matsrc[1] ? src[k.matsrc[1]] : "mspec";
    std::string ma = k.matsrc[2] ? src[k.matsrc[2]] : "mamb", me = k.matsrc[3] ? src[k.matsrc[3]] : "memis";
    s += "  float3 dsum = 0, asum = 0; float4 ssum = 0;\n";
    s += k.localviewer ? "  float3 V = -normalize(cp);\n" : "  float3 V = float3(0, 0, -1);\n";
    for (int i = 0; i < k.nlights; i++) {
      char b[1600];
      if (k.ltype[i] == D3DLIGHT_DIRECTIONAL)
        snprintf(b, sizeof b, "  { float3 L = -ldir[%d].xyz; float att = 1;\n", i);
      else
        snprintf(b, sizeof b,
                 "  { float3 dv = lpos[%d].xyz - cp; float d = length(dv); float3 L = dv / max(d, 1e-20);\n"
                 "    float att = d > latt[%d].x ? 0 : 1 / max(latt[%d].y + latt[%d].z * d + latt[%d].w * d * d, 1e-20);\n",
                 i, i, i, i, i);
      s += b;
      if (k.ltype[i] == D3DLIGHT_SPOT) {
        // lspot: falloff, cos(theta/2), cos(phi/2)
        snprintf(b, sizeof b,
                 "    float rho = dot(-L, normalize(ldir[%d].xyz));\n"
                 "    float sp = rho > lspot[%d].y ? 1 : rho <= lspot[%d].z ? 0 : pow(saturate((rho - lspot[%d].z) / max(lspot[%d].y - lspot[%d].z, 1e-20)), lspot[%d].x);\n"
                 "    att *= sp;\n", i, i, i, i, i, i, i);
        s += b;
      }
      // each light's N.L as the "Diffuse Lighting" equation has it (DirectX 8.1 and Direct3D 9 alike), sum[...*(N.Ldir)*...],
      // clamped "after all lights are processed": no clamp at 1 per light (Wine 9.0 clamps there, which shows with
      // normals longer than 1 and NORMALIZENORMALS off: test/random_cases.py, class L). The clamp at 0 per light is
      // not in that text; it is Wine's and keeps a light from taking light away. Windows' is not established
      snprintf(b, sizeof b,
               "    float nl = max(dot(nrm, L), 0);\n"
               "    asum += att * lamb[%d].rgb; dsum += att * nl * ldiff[%d].rgb;\n", i, i);
      s += b;
      if (k.specular) {
        // the specular term needs N.L > 0 and N.H > 0, and keeps the light's alpha (as Wine's glsl_shader.c lighting)
        snprintf(b, sizeof b, "    float3 H = normalize(L + V); float nh = dot(nrm, H); if (nl > 0 && nh > 0) ssum += att * pow(nh, mpow.x) * lspec[%d];\n", i);
        s += b;
      }
      s += "  }\n";
    }
    s += "  o.d0.rgb = " + me + ".rgb + " + ma + ".rgb * (ambient.rgb + asum) + " + md + ".rgb * dsum;\n";
    s += "  o.d0.a = " + md + ".a;\n";
    s += k.specular ? "  o.d1 = " + ms + " * ssum;\n" : "  o.d1 = float4(0,0,0,0);\n";
    s += "  o.d0 = saturate(o.d0); o.d1 = saturate(o.d1);\n";
  } else {
    s += "  o.d0 = vd; o.d1 = vs;\n";
  }
  // texture coordinates, per stage
  for (int i = 0; i < k.ntc; i++) {
    char b[400]; const auto &t = k.tc[i];
    std::string src;
    // Pre-transformed vertices: no coordinates are generated, the stage reads its set as given. The DirectX 8 era text
    // (Windows CE .NET 4.2 Direct3D, "Transformed and Lit Vertices") says Direct3D "does not modify transformed and lit
    // vertices at all"; it does not name the texture coordinate flags, so this is a reading of it, the one Wine's bug
    // 37313 also takes (Wine 9.0 generates from the screen position instead). The game was not seen combining the two
    // (its traces, 14 Oct). test/random_cases.py lists such cases apart.
    switch (k.rhw ? 0 : t.src) {
    case 1: src = "float4(nrm, 1)"; break;
    case 2: src = "float4(cp, 1)"; break;
    case 3: src = "float4(reflect(normalize(cp), nrm), 1)"; break;
    default: {
      // a coordinate set's missing components: the one after its last is 1, the rest 0 (so a 2D set's translation
      // is the matrix's third row, as the DirectX 8 reference describes texture transforms) PROVISIONAL
      int ix = has[SE_TEX][t.idx]; std::string n = "v.a" + std::to_string(ix - 1);
      if (!ix) src = "float4(0,0,0,0)";
      else switch (k.el[ix - 1].fmt) {
        case V_F1: src = "float4(" + n + ", 1, 0, 0)"; break; case V_F2: case V_SHORT2: src = "float4(" + n + ", 1, 0)"; break;
        case V_F3: src = "float4(" + n + ", 1)"; break; default: src = "float4(" + n + ")";
      }
    } }
    if (t.count) snprintf(b, sizeof b, "  o.tc[%d] = mul(%s, tex[%d]);\n", i, src.c_str(), i);
    else snprintf(b, sizeof b, "  o.tc[%d] = %s;\n", i, src.c_str());
    s += b;
  }
  switch (k.fogsrc) {
  case FOG_SPECA: s += "  o.fog = vs.a * 255;\n"; break;
  case FOG_RANGE: s += "  o.fog = length(cp);\n"; break;
  case FOG_EYEZ: s += "  o.fog = abs(cp.z);\n"; break;
  case FOG_RHWZ: s += "  o.fog = p.z;\n"; break;
  case FOG_CLIPZ: s += "  o.fog = o.pos.z;\n"; break;
  case FOG_RHWW: s += "  o.fog = w;\n"; break;                      // 1/RHW: "true w" (DirectX 8.1, "Pixel Fog")
  default: s += "  o.fog = 0;\n";
  }
  // vertex fog: the factor per vertex, clamped, as 255 * factor (fogp: start, end, density, 1 / (end - start))
  if (k.vfog == 1) s += "  o.fog = saturate((fogp.y - o.fog) * fogp.w) * 255;\n";
  else if (k.vfog == 2) s += "  o.fog = saturate(exp(-fogp.z * o.fog)) * 255;\n";
  else if (k.vfog == 3) s += "  o.fog = saturate(exp(-fogp.z * fogp.z * o.fog * o.fog)) * 255;\n";
  s += "  o.d1a = o.d1.a;\n  return o;\n}\n";
  return s;
}

// ---- HLSL: pixel shader ---------------------------------------------------------------------------------------------------
static std::string arg(int a, const char *tex) {
  std::string v;
  switch (a & D3DTA_SELECTMASK) {
  case D3DTA_DIFFUSE: v = "i.d0"; break; case D3DTA_CURRENT: v = "cur"; break; case D3DTA_TEXTURE: v = tex; break;
  case D3DTA_TFACTOR: v = "tfactor"; break; case D3DTA_SPECULAR: v = "float4(i.d1.rgb, i.d1a)"; break; case D3DTA_TEMP: v = "tmp"; break;
  default: v = "cur";
  }
  if (a & D3DTA_ALPHAREPLICATE) v = "(" + v + ").aaaa";
  if (a & D3DTA_COMPLEMENT) v = "(1 - " + v + ")";
  return v;
}
static std::string op(int o, const std::string &a1, const std::string &a2, const std::string &a0, const char *tex, const char *sw) {
  auto c = [&](const std::string &x) { return "(" + x + ")" + sw; };
  std::string A = c(a1), B = c(a2), Z = c(a0);
  switch (o) {
  case D3DTOP_SELECTARG1: return A;
  case D3DTOP_SELECTARG2: return B;
  case D3DTOP_MODULATE: return A + " * " + B;
  case D3DTOP_MODULATE2X: return "saturate(" + A + " * " + B + " * 2)";
  case D3DTOP_MODULATE4X: return "saturate(" + A + " * " + B + " * 4)";
  case D3DTOP_ADD: return "saturate(" + A + " + " + B + ")";
  case D3DTOP_ADDSIGNED: return "saturate(" + A + " + " + B + " - 0.5)";
  case D3DTOP_ADDSIGNED2X: return "saturate((" + A + " + " + B + " - 0.5) * 2)";
  case D3DTOP_SUBTRACT: return "saturate(" + A + " - " + B + ")";
  case D3DTOP_ADDSMOOTH: return "saturate(" + A + " + " + B + " - " + A + " * " + B + ")";
  case D3DTOP_BLENDDIFFUSEALPHA: return "lerp(" + B + ", " + A + ", i.d0.a)";
  case D3DTOP_BLENDTEXTUREALPHA: return std::string("lerp(") + B + ", " + A + ", (" + tex + ").a)";
  case D3DTOP_BLENDFACTORALPHA: return "lerp(" + B + ", " + A + ", tfactor.a)";
  case D3DTOP_BLENDTEXTUREALPHAPM: return std::string("saturate(") + A + " + " + B + " * (1 - (" + tex + ").a))";
  case D3DTOP_BLENDCURRENTALPHA: return "lerp(" + B + ", " + A + ", cur.a)";
  case D3DTOP_MODULATEALPHA_ADDCOLOR: return "saturate(" + A + " + (" + a1 + ").a * " + B + ")";
  case D3DTOP_MODULATECOLOR_ADDALPHA: return "saturate(" + A + " * " + B + " + (" + a1 + ").a)";
  case D3DTOP_MODULATEINVALPHA_ADDCOLOR: return "saturate((1 - (" + a1 + ").a) * " + B + " + " + A + ")";
  case D3DTOP_MODULATEINVCOLOR_ADDALPHA: return "saturate((1 - " + A + ") * " + B + " + (" + a1 + ").a)";
  case D3DTOP_DOTPRODUCT3: return "saturate(4 * dot(((" + a1 + ").rgb - 0.5), ((" + a2 + ").rgb - 0.5)))";
  case D3DTOP_MULTIPLYADD: return "saturate(" + Z + " + " + A + " * " + B + ")";
  case D3DTOP_LERP: return "lerp(" + B + ", " + A + ", " + Z + ")";   // arg0 * arg1 + (1 - arg0) * arg2
  }
  return "";
}
static float rhw_offset() { static float e = getenv("D8ON11_TEST_RHW_OFFSET") ? (float)atof(getenv("D8ON11_TEST_RHW_OFFSET")) : 0.5f; return e; }
static float texedge() { static float e = getenv("D8ON11_TEST_TEXEDGE") ? (float)atof(getenv("D8ON11_TEST_TEXEDGE")) : 0; return e; }
static float ties() { static float e = getenv("D8ON11_TEST_TIES") ? (float)atof(getenv("D8ON11_TEST_TIES")) : 0; return e > 0 ? e : 1.0f / 256; }
std::string gen_ps(const PSKey &k) {
  std::string s =
    "cbuffer C : register(b0) { float4 tfactor, fogcolor, fogp, aref; };\n"
    + io_struct("PI", k.flat);
  for (int i = 0; i < k.nstages; i++) {
    char b[160];
    if (k.s[i].tex == 1) snprintf(b, sizeof b, "Texture2D t%d : register(t%d); SamplerState s%d : register(s%d);\n", i, i, i, i);
    else if (k.s[i].tex == 2) snprintf(b, sizeof b, "TextureCube t%d : register(t%d); SamplerState s%d : register(s%d);\n", i, i, i, i);
    else b[0] = 0;
    s += b;
  }
  s += "float4 main(PI i) : SV_Target {\n  float4 cur = i.d0, tmp = 0; bool tie = false;\n";
  for (int i = 0; i < k.nstages; i++) {
    const auto &t = k.s[i]; char b[400], tex[16];
    snprintf(tex, sizeof tex, "x%d", i);
    if (t.tex == 1) {
      std::string uv = t.tcn ? "i.tc[" + std::to_string(i) + "].xy / i.tc[" + std::to_string(i) + "]." + "xyzw"[t.tcn - 1] : "i.tc[" + std::to_string(i) + "].xy";
      snprintf(b, sizeof b, "  float4 x%d = t%d.Sample(s%d, %s);\n", i, i, i, uv.c_str());
      if (texedge() > 0) {
        // a test only (D8ON11_TEST_TEXEDGE=eps): where the coordinate lies within eps texels of a texel boundary, read
        // the texel across the nearer boundary. Applied to every 2D stage, so meaningful only for point-filtered cases:
        // a pixel that then matches Microsoft's reference device differed from it only by the reference's point
        // sampling at a boundary (DirectX 8.1, "Nearest-Point Sampling"; README.md). The choice is made by a scalar
        // condition: Wine's d3dcompiler does not evaluate a float2 ?: per component (measured, 14 Oct)
        char e[600];
        snprintf(e, sizeof e, "  { uint w, h; t%d.GetDimensions(w, h); float2 z = float2(w, h), q = (%s) * z, f = frac(q), d = min(f, 1 - f); float2 m = "
                 "(d.x <= d.y ? float2(1, 0) : float2(0, 1)) * sign(f - 0.5); if (min(d.x, d.y) < %g) x%d = t%d.Sample(s%d, (floor(q) + m + 0.5) / z); }\n",
                 i, uv.c_str(), texedge(), i, i, i);
        s += b; s += e; b[0] = 0;
      }
      if ((k.tie_minpt | k.tie_magpt) >> i & 1) {
        // a test only (D8ON11_TEST_TIES=eps): mark the pixel where this stage point-samples within eps texels of a
        // texel boundary, where D3D11 lets the driver read either texel ("scaledU is converted to at least 16.8 Fixed
        // Point"; "The integer part of fxpScaledU is the chosen texel", D3D11.3 functional spec 7.18.7, with the 0.6 ULP
        // conversion tolerance of 3.2.4.1). Magnified or minified, and the level: the spec's isotropic LOD (7.18.11,
        // log2 of the longer of the two derivative vectors in texels; ours, not the driver's, which may differ near
        // LOD 0), a level nearest it (MIP POINT, 7.18.10) or the two around it (MIP LINEAR), within the stage's view
        // (one level for MIPFILTER NONE, d8_ff.cpp). MIPMAPLODBIAS is not applied (the game's traces never set it,
        // data/d3d8_scope.json); a boundary that address clamping removes is still counted
        char e[1400];
        snprintf(e, sizeof e,
                 "  { uint w, h, n; t%d.GetDimensions(0, w, h, n); float2 q = (%s) * float2(w, h), gx = ddx(q), gy = ddy(q);\n"
                 "    float lod = 0.5 * log2(max(dot(gx, gx), dot(gy, gy)));\n"
                 "    if (lod <= 0 ? %d : %d) {\n"
                 "      float l0 = lod <= 0 ? 0 : %s; uint la = (uint)min(l0, n - 1), lb = (uint)min(l0 + (%d && lod > 0 ? 1 : 0), n - 1);\n"
                 "      for (uint l = la; l <= lb; l++) { t%d.GetDimensions(l, w, h, n); float2 f = frac((%s) * float2(w, h)), d = min(f, 1 - f);\n"
                 "        if (min(d.x, d.y) < %g) tie = true; } } }\n",
                 i, uv.c_str(), k.tie_magpt >> i & 1, k.tie_minpt >> i & 1, (k.tie_miplin >> i & 1) ? "floor(lod)" : "floor(lod + 0.5)",
                 k.tie_miplin >> i & 1, i, uv.c_str(), ties());
        s += b; s += e; b[0] = 0;
      }
    } else if (t.tex == 2) snprintf(b, sizeof b, "  float4 x%d = t%d.Sample(s%d, i.tc[%d].xyz);\n", i, i, i, i);
    // any other read of a stage's texture when none is set (d8_ff.cpp: COLORARG1 D3DTA_TEXTURE ends the stages) reads
    // (0, 0, 0, 1): measured on the reference device (t8 random, 14 Oct: through ARG2, ARG0, the alpha args, with
    // COMPLEMENT and ALPHAREPLICATE); the DirectX 8 reference does not say
    else snprintf(b, sizeof b, "  float4 x%d = float4(0, 0, 0, 1);\n", i);
    s += b;
    std::string cr = op(t.cop, arg(t.ca1, tex), arg(t.ca2, tex), arg(t.ca0, tex), tex, ".rgb");
    std::string ar = t.aop == D3DTOP_DISABLE ? "cur.a" : op(t.aop, arg(t.aa1, tex), arg(t.aa2, tex), arg(t.aa0, tex), tex, ".a");
    if (t.cop == D3DTOP_DOTPRODUCT3) ar = cr;                                        // DOTPRODUCT3 goes to all four channels
    const char *dst = t.res == D3DTA_TEMP ? "tmp" : "cur";
    snprintf(b, sizeof b, "  { float3 c = %s; float a = %s; %s = float4(c, a); }\n", cr.c_str(), ar.c_str(), dst);
    s += b;
  }
  if (k.specadd) s += "  cur.rgb = saturate(cur.rgb + i.d1.rgb);\n";
  if (k.alphatest) {
    // D3DCMP: NEVER 1, LESS 2, EQUAL 3, LESSEQUAL 4, GREATER 5, NOTEQUAL 6, GREATEREQUAL 7, ALWAYS 8 (pass when alpha OP ref).
    // ALPHAREF "is an 8-bit value" and "the incoming alpha value is compared with" it (DirectX 8.1, D3DRENDERSTATETYPE);
    // the text does not say how the incoming alpha is made 8-bit. Measured on the reference device (t8 random, 14 Oct:
    // alpha T*T, a two-stage chain, ADDSIGNED's half steps, against each function): floor(alpha * 255), taken once
    // after the last stage, whatever the function. The 1/512 added first is ours: it keeps a value that is exactly
    // k/255 from falling a step under float rounding (a product of 8-bit values is m/255, 1/255 from the next step)
    static const char *c[] = {"", "false", "a < r", "a == r", "a <= r", "a > r", "a != r", "a >= r", "true"};
    s += std::string("  { float a = floor(cur.a * 255 + 1.0 / 512), r = round(aref.x * 255); if (!(") + c[k.alphatest] + ")) discard; }\n";
  }
  // fog: 1 linear, 2 exp, 3 exp2 (fogp: start, end, density, 1 / (end - start))
  if (k.fog == 1) s += "  cur.rgb = lerp(fogcolor.rgb, cur.rgb, saturate((fogp.y - i.fog) * fogp.w));\n";
  else if (k.fog == 2) s += "  cur.rgb = lerp(fogcolor.rgb, cur.rgb, saturate(exp(-fogp.z * i.fog)));\n";
  else if (k.fog == 3) s += "  cur.rgb = lerp(fogcolor.rgb, cur.rgb, saturate(exp(-fogp.z * fogp.z * i.fog * i.fog)));\n";
  if (k.tie_minpt | k.tie_magpt) s += "  if (tie) cur = float4(1, 0, 1, 1);\n";   // the tie marker (D8ON11_TEST_TIES)
  s += "  return cur;\n}\n";
  return s;
}

// ---- compile and cache --------------------------------------------------------------------------------------------------------
static ID3DBlob *compile(FF *f, const std::string &src, const char *target) {
  if (!f->compile) return nullptr;
  ID3DBlob *code = nullptr, *err = nullptr;
  HRESULT hr = f->compile(src.data(), src.size(), "d3d8on11", NULL, NULL, "main", target, D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &code, &err);
  if (FAILED(hr)) {
    d8_log("shader compile failed (%s, 0x%08lX): %s\n----\n%s----", target, (unsigned long)hr,
           err ? (const char *)err->GetBufferPointer() : "", src.c_str());
    if (err) err->Release();
    return nullptr;
  }
  if (err) err->Release();
  return code;
}

template <class D, class O, class Make> static O *cached(std::unordered_map<std::string, O *> &m, const D &d, Make make) {
  std::string k((const char *)&d, sizeof d);
  auto it = m.find(k);
  if (it != m.end()) return it->second;
  O *o = nullptr; make(&d, &o); m[k] = o;
  return o;
}
// ring writes into the UP buffers: NOOVERWRITE while it fits, DISCARD when it wraps
static bool ring(ID3D11DeviceContext *ctx, ID3D11Buffer *b, UINT cap, UINT &pos, const void *data, UINT bytes, UINT align, UINT *at) {
  if (bytes > cap) return false;
  pos = (pos + align - 1) / align * align;
  D3D11_MAP m = D3D11_MAP_WRITE_NO_OVERWRITE;
  if (pos + bytes > cap) { pos = 0; m = D3D11_MAP_WRITE_DISCARD; }
  if (pos == 0) m = D3D11_MAP_WRITE_DISCARD;
  D3D11_MAPPED_SUBRESOURCE ms;
  if (FAILED(ctx->Map(b, 0, m, 0, &ms))) return false;
  memcpy((uint8_t *)ms.pData + pos, data, bytes); ctx->Unmap(b, 0);
  *at = pos; pos += bytes;
  return true;
}


// ---- the executor ------------------------------------------------------------------------------------------------------
void Exec::ff_init() {
  ff = new ExecFF();
  HMODULE m = LoadLibraryA("d3dcompiler_47.dll");
  ff->compile = m ? (PFN_Compile)(void *)GetProcAddress(m, "D3DCompile") : nullptr;
  if (!ff->compile) d8_log("d3dcompiler_47.dll not found: draws cannot be executed");
  D3D11_BUFFER_DESC bd = {}; bd.Usage = D3D11_USAGE_DYNAMIC; bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; bd.ByteWidth = (sizeof(VSConst) + 15) & ~15u; dev->CreateBuffer(&bd, NULL, &ff->vcb);
  bd.ByteWidth = (sizeof(PSConst) + 15) & ~15u; dev->CreateBuffer(&bd, NULL, &ff->pcb);
  bd.ByteWidth = 64u * SKIN_BONES; dev->CreateBuffer(&bd, NULL, &bones_cb);   // gpu skinning's palette
  bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; bd.ByteWidth = FF::UPVB; dev->CreateBuffer(&bd, NULL, &ff->upvb);
  bd.BindFlags = D3D11_BIND_INDEX_BUFFER; bd.ByteWidth = FF::UPIB; dev->CreateBuffer(&bd, NULL, &ff->upib);
  { D3D11_FEATURE_DATA_THREADING th = {};                 // (12 Nov) can the driver take work from several threads itself
    if (SUCCEEDED(dev->CheckFeatureSupport(D3D11_FEATURE_THREADING, &th, sizeof th)))
      d8_log("D3D11 threading: the driver's concurrent creates %d, the driver's command lists %d (0: the runtime emulates them)",
             (int)th.DriverConcurrentCreates, (int)th.DriverCommandLists); }
  D3D11_FEATURE_DATA_D3D11_OPTIONS o = {};
  if (SUCCEEDED(ctx->QueryInterface(__uuidof(ID3D11DeviceContext1), (void **)&ff->ctx1)) &&
      SUCCEEDED(dev->CheckFeatureSupport(D3D11_FEATURE_D3D11_OPTIONS, &o, sizeof o)) && o.ConstantBufferOffsetting) {
    ff->cbnoov = o.MapNoOverwriteOnDynamicConstantBuffer != 0;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; bd.ByteWidth = FF::VAR;
    if (FAILED(dev->CreateBuffer(&bd, NULL, &ff->varena))) ff->varena = nullptr;
    bd.ByteWidth = FF::PAR;
    if (FAILED(dev->CreateBuffer(&bd, NULL, &ff->parena))) { ff->parena = nullptr; if (ff->varena) ff->varena->Release(); ff->varena = nullptr; }
  }
  d8_log("constant buffer offsets: %s (Direct3D 11.1 context %s, ConstantBufferOffsetting %d, MapNoOverwriteOnDynamicConstantBuffer %d)",
         ff->varena ? "used by batches" : "not available: each draw maps its constants", ff->ctx1 ? "yes" : "no",
         (int)o.ConstantBufferOffsetting, (int)o.MapNoOverwriteOnDynamicConstantBuffer);
}
static void free_res(ExecRes &r) {
  for (auto &v : r.rtv) if (v.second) v.second->Release();
  r.rtv.clear();
  for (auto &v : r.srv_levels) if (v.second) v.second->Release();
  r.srv_levels.clear();
  if (r.srv) r.srv->Release(); if (r.dsv) r.dsv->Release(); if (r.tex) r.tex->Release(); if (r.buf) r.buf->Release();
  if (r.stage) r.stage->Release();
  r.srv = nullptr; r.dsv = nullptr; r.tex = nullptr; r.buf = nullptr; r.stage = nullptr;
}
void Exec::release_swap() {
  if (ctx) { ctx->OMSetRenderTargets(0, NULL, NULL); ctx->ClearState(); ctx->Flush(); }
  forget();
  cur_rtv = nullptr; cur_dsv = nullptr;
  auto it = res.find(X_BACKBUFFER);
  if (it != res.end()) { free_res(it->second); res.erase(it); }
  if (swap) swap->Release(); swap = nullptr;
}
void Exec::drain_debug() {
  if (!iq) return;
  UINT64 n = iq->GetNumStoredMessages();
  for (UINT64 i = 0; i < n; i++) {
    SIZE_T len = 0;
    if (FAILED(iq->GetMessage(i, NULL, &len)) || !len) continue;
    D3D11_MESSAGE *m = (D3D11_MESSAGE *)malloc(len);
    if (m && SUCCEEDED(iq->GetMessage(i, m, &len)))
      d8_log("D3D11 debug layer [frame %llu] sev %d id %d: %.*s", (unsigned long long)presents, (int)m->Severity, (int)m->ID,
             (int)m->DescriptionByteLength, m->pDescription);
    free(m);
  }
  iq->ClearStoredMessages();
}
int32_t Exec::init(const XInit &x, bool resize) {
  if (fail_init) { d8_log("X_INIT answered 0x%08X (D8ON11_TEST_EXEC_FAIL_INIT, a test)", (unsigned)fail_init); return fail_init; }
  wnd = (HWND)(uintptr_t)x.hwnd;
  sync = x.interval <= 4 ? x.interval : 0;                 // IDXGISwapChain::Present takes 0 to 4
  if (!resize) {
    D3D_FEATURE_LEVEL fl = D3D_FEATURE_LEVEL_11_0, got;
    /* Diagnostics (the owner's t8run, HISTORY 14 Oct): D8ON11_DRIVER=warp draws on Microsoft's software rasterizer
       instead of the GPU; D8ON11_DEBUG_LAYER=1 asks for the D3D11 debug layer (needs the Graphics Tools feature). */
    const char *drv = getenv("D8ON11_DRIVER");
    bool warp = drv && !_stricmp(drv, "warp");
    UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | (getenv("D8ON11_DEBUG_LAYER") ? D3D11_CREATE_DEVICE_DEBUG : 0);
    HRESULT hr = D3D11CreateDevice(NULL, warp ? D3D_DRIVER_TYPE_WARP : D3D_DRIVER_TYPE_HARDWARE, NULL, flags, &fl, 1,
                                   D3D11_SDK_VERSION, &dev, &got, &ctx);
    if (warp || (flags & D3D11_CREATE_DEVICE_DEBUG)) d8_log("D3D11 device: %s%s: 0x%08lX", warp ? "WARP" : "hardware",
                                                            (flags & D3D11_CREATE_DEVICE_DEBUG) ? ", debug layer" : "", (unsigned long)hr);
    if (SUCCEEDED(hr) && (flags & D3D11_CREATE_DEVICE_DEBUG)) dev->QueryInterface(__uuidof(ID3D11InfoQueue), (void **)&iq);
    if (FAILED(hr)) { d8_log("D3D11CreateDevice (feature level 11_0): 0x%08lX", (unsigned long)hr); return D3DERR_NOTAVAILABLE; }
    ff_init();
  } else release_swap();
  last_init = x;
  return make_swap();
}
// the swap chain for last_init, in the model `flip` names; the back buffer's resource (X_BACKBUFFER) with it. In the flip
// model Direct3D 11 keeps buffer 0 the current back buffer (its views follow it), and unbinds it at each Present
// (bound again after each, X_PRESENT). A flip-model swap chain that cannot be made falls back to the blt model, logged.
int32_t Exec::make_swap() {
  const XInit &x = last_init;
  IDXGIDevice *xd = NULL; IDXGIAdapter *xa = NULL; IDXGIFactory *xf = NULL;
  dev->QueryInterface(__uuidof(IDXGIDevice), (void **)&xd); xd->GetAdapter(&xa); xa->GetParent(__uuidof(IDXGIFactory), (void **)&xf);
  if (latency) {                                           // frame_latency: how many frames the driver may queue ahead
    IDXGIDevice1 *x1 = NULL;
    HRESULT lh = dev->QueryInterface(__uuidof(IDXGIDevice1), (void **)&x1);
    if (SUCCEEDED(lh)) { lh = x1->SetMaximumFrameLatency(latency); x1->Release(); }
    d8_log("frame_latency=%u: SetMaximumFrameLatency 0x%08lX", latency, (unsigned long)lh);
  }
  DXGI_SWAP_CHAIN_DESC sd = {};
  sd.BufferDesc.Width = x.width; sd.BufferDesc.Height = x.height; sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  sd.SampleDesc.Count = 1; sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; sd.OutputWindow = wnd; sd.Windowed = TRUE;
  sd.BufferCount = flip ? 2 : 1; sd.SwapEffect = flip ? DXGI_SWAP_EFFECT_FLIP_DISCARD : DXGI_SWAP_EFFECT_DISCARD;
  HRESULT hr = xf->CreateSwapChain(dev, &sd, &swap);
  if (FAILED(hr) && flip) {
    d8_log("CreateSwapChain, flip model (FLIP_DISCARD, 2 buffers) %ux%u: 0x%08lX; the blt model instead", x.width, x.height, (unsigned long)hr);
    flip = flip_want = false; sd.BufferCount = 1; sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    hr = xf->CreateSwapChain(dev, &sd, &swap);
  }
  xf->MakeWindowAssociation(wnd, DXGI_MWA_NO_ALT_ENTER);
  xf->Release(); xa->Release(); xd->Release();
  if (FAILED(hr)) { d8_log("CreateSwapChain %ux%u: 0x%08lX", x.width, x.height, (unsigned long)hr); return D3DERR_NOTAVAILABLE; }
  ExecRes &b = res[X_BACKBUFFER]; b.kind = 4; b.w = x.width; b.h = x.height; b.d3dfmt = D3DFMT_X8R8G8B8;
  swap->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&b.tex);
  return D3D_OK;
}
// ---- whether Direct3D 12 is there (OPT_D3D12, 12 Nov) ----------------------------------------------------------------------
// The owner asked whether D3D12 could spread the executor's work over threads. Whether the machine has it is not known
// from a feature level a D3D11 driver reports, so this makes one: on the D3D11 device's own adapter, in this process
// (32-bit x86 in thread mode, the executor's own architecture in remote mode), then a command queue, a command list,
// and a fence it waits on (2 s at most). Each step's HRESULT is logged; the first failure ends it. Nothing it makes is
// kept. d3d8on11.ini d3d12_probe=0 (or D8ON11_D3D12_PROBE=0) turns it off.
static double qpc_ms();
static void d3d12_wide(const WCHAR *w, char *out, int n) { out[0] = 0; WideCharToMultiByte(CP_UTF8, 0, w, -1, out, n, NULL, NULL); }
void Exec::d3d12_probe() {
  d8_log("D3D12 probe: starting (%d-bit process; d3d12_probe=0 in d3d8on11.ini turns it off)", (int)(sizeof(void *) * 8));
  HMODULE m = LoadLibraryA("d3d12.dll");
  if (!m) { d8_log("D3D12 probe: d3d12.dll not loaded (error %lu): no D3D12 in this process", (unsigned long)GetLastError()); return; }
  PFN_D3D12_CREATE_DEVICE create = (PFN_D3D12_CREATE_DEVICE)(void *)GetProcAddress(m, "D3D12CreateDevice");
  if (!create) { d8_log("D3D12 probe: d3d12.dll has no D3D12CreateDevice"); return; }
  IDXGIDevice *dd = nullptr; IDXGIAdapter *ad = nullptr;
  if (dev && SUCCEEDED(dev->QueryInterface(__uuidof(IDXGIDevice), (void **)&dd))) { dd->GetAdapter(&ad); dd->Release(); }
  if (ad) { DXGI_ADAPTER_DESC a = {}; ad->GetDesc(&a); char nm[256]; d3d12_wide(a.Description, nm, sizeof nm);
    d8_log("D3D12 probe: the D3D11 device's adapter: \"%s\" (vendor 0x%04X, device 0x%04X, %u MB dedicated video memory)",
           nm, a.VendorId, a.DeviceId, (unsigned)(a.DedicatedVideoMemory >> 20)); }
  ID3D12Device *d = nullptr; double t0 = qpc_ms();
  HRESULT h = create(ad, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void **)&d);
  if (ad) ad->Release();
  if (FAILED(h) || !d) { d8_log("D3D12 probe: D3D12CreateDevice on that adapter: 0x%08lX: no D3D12 device here", (unsigned long)h); return; }
  d8_log("D3D12 probe: a D3D12 device was made (%.1f ms)", qpc_ms() - t0);
  { D3D_FEATURE_LEVEL want[] = {(D3D_FEATURE_LEVEL)0xc200, (D3D_FEATURE_LEVEL)0xc100, (D3D_FEATURE_LEVEL)0xc000,
                                D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0};
    D3D12_FEATURE_DATA_FEATURE_LEVELS fl = {}; fl.NumFeatureLevels = 5; fl.pFeatureLevelsRequested = want;
    HRESULT hf = d->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &fl, sizeof fl);
    D3D12_FEATURE_DATA_D3D12_OPTIONS o = {}; HRESULT ho = d->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &o, sizeof o);
    d8_log("D3D12 probe: highest feature level 0x%X (0x%08lX); resource binding tier %d, resource heap tier %d (0x%08lX)",
           SUCCEEDED(hf) ? (unsigned)fl.MaxSupportedFeatureLevel : 0u, (unsigned long)hf,
           (int)o.ResourceBindingTier, (int)o.ResourceHeapTier, (unsigned long)ho); }
  ID3D12CommandQueue *q = nullptr; ID3D12CommandAllocator *al = nullptr; ID3D12GraphicsCommandList *cl = nullptr;
  ID3D12Fence *f = nullptr; HANDLE ev = nullptr; const char *step = "";
  do {
    D3D12_COMMAND_QUEUE_DESC qd = {}; qd.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    step = "CreateCommandQueue"; if (FAILED(h = d->CreateCommandQueue(&qd, __uuidof(ID3D12CommandQueue), (void **)&q))) break;
    step = "CreateCommandAllocator"; if (FAILED(h = d->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void **)&al))) break;
    step = "CreateCommandList"; if (FAILED(h = d->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, al, NULL, __uuidof(ID3D12GraphicsCommandList), (void **)&cl))) break;
    step = "Close"; if (FAILED(h = cl->Close())) break;
    step = "CreateFence"; if (FAILED(h = d->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void **)&f))) break;
    ev = CreateEventA(NULL, FALSE, FALSE, NULL);
    ID3D12CommandList *lists[] = {cl}; double t1 = qpc_ms();
    q->ExecuteCommandLists(1, lists);
    step = "Signal"; if (FAILED(h = q->Signal(f, 1))) break;
    step = "SetEventOnCompletion"; if (FAILED(h = f->SetEventOnCompletion(1, ev))) break;
    DWORD w = WaitForSingleObject(ev, 2000);
    if (w != WAIT_OBJECT_0) { d8_log("D3D12 probe: a command list was submitted but the GPU did not finish it in 2 s (wait %lu, fence %llu)",
                                     (unsigned long)w, (unsigned long long)f->GetCompletedValue()); step = nullptr; break; }
    d8_log("D3D12 probe: a command list ran on the GPU and its fence came back (%.2f ms): D3D12 works in this process", qpc_ms() - t1);
    step = nullptr;
  } while (0);
  if (step) d8_log("D3D12 probe: %s: 0x%08lX (device removed reason 0x%08lX)", step, (unsigned long)h, (unsigned long)d->GetDeviceRemovedReason());
  if (ev) CloseHandle(ev);
  if (f) f->Release(); if (cl) cl->Release(); if (al) al->Release(); if (q) q->Release();
  d->Release();
  d8_log("D3D12 probe: ended, everything it made released");
}

// ---- the GPU's time per frame (OPT_GPUTIME) ----
void Exec::gpu_collect(int j, bool) {
  if (!q_used[j]) return;
  D3D11_QUERY_DATA_TIMESTAMP_DISJOINT dj; UINT64 t0, t1;
  if (ctx->GetData(q_dis[j], &dj, sizeof dj, D3D11_ASYNC_GETDATA_DONOTFLUSH) != S_OK ||
      ctx->GetData(q_beg[j], &t0, sizeof t0, D3D11_ASYNC_GETDATA_DONOTFLUSH) != S_OK ||
      ctx->GetData(q_end[j], &t1, sizeof t1, D3D11_ASYNC_GETDATA_DONOTFLUSH) != S_OK) { gpu_lost++; q_used[j] = false; return; }
  q_used[j] = false;
  if (dj.Disjoint || !dj.Frequency || t1 < t0) { gpu_lost++; return; }
  gpu_ms.push_back((float)((double)(t1 - t0) * 1000.0 / (double)dj.Frequency));
}
void Exec::gpu_frame_begin() {                             // after a Present: the next frame's work starts here
  if (!gpu_on || !dev) return;
  if (!q_dis[0]) {
    D3D11_QUERY_DESC qd = {D3D11_QUERY_TIMESTAMP_DISJOINT, 0}, qt = {D3D11_QUERY_TIMESTAMP, 0};
    for (int i = 0; i < NQ; i++) { dev->CreateQuery(&qd, &q_dis[i]); dev->CreateQuery(&qt, &q_beg[i]); dev->CreateQuery(&qt, &q_end[i]); }
    if (!q_dis[NQ - 1] || !q_end[NQ - 1]) { d8_log("gpu_time: the timestamp queries could not be made; off"); gpu_on = false; return; }
  }
  q_i = (q_i + 1) % NQ;
  gpu_collect(q_i, true);                                  // this slot's last use, NQ frames ago: read now or counted as lost
  ctx->Begin(q_dis[q_i]); ctx->End(q_beg[q_i]); q_open = true;
}
void Exec::gpu_frame_end() {                               // before a Present: the frame's work ends here
  if (!gpu_on || !q_open) return;
  ctx->End(q_end[q_i]); ctx->End(q_dis[q_i]); q_used[q_i] = true; q_open = false; gpu_frames++;
  if (gpu_frames >= 600) { D8StatsScope q;
    std::vector<float> v = gpu_ms; std::sort(v.begin(), v.end());
    double sum = 0; for (float f : v) sum += f;
    if (!v.empty())
      d8_log("gpu_time, %u frames: the GPU's time for a frame's work %.3f ms on average, 95%% %.3f, longest %.3f (%u measured, %u not ready "
             "in %d frames or disjoint); swap chain: %s", gpu_frames, sum / v.size(), v[(size_t)(v.size() * 0.95)], v.back(), (unsigned)v.size(), gpu_lost, NQ,
             flip ? "flip model (FLIP_DISCARD, 2 buffers)" : "blt model (DISCARD, 1 buffer)");
    else d8_log("gpu_time, %u frames: none measured (%u not ready in %d frames or disjoint)", gpu_frames, gpu_lost, NQ);
    gpu_ms.clear(); gpu_frames = 0; gpu_lost = 0;
  }
}
// Clear of rectangles (part of the target, or a depth surface larger than it): each rectangle drawn with a shader
// that writes the colour, the depth (SV_Depth) and, by REPLACE, the stencil. Draws reset every state they use, so
// nothing is restored here.
void Exec::clear_rects(const XClear &c, const XRect *r) {
  if (!ff || !ff->compile) ff_init();
  if (!ff || !ff->compile || !c.width || !c.height) return;
  if (!clr_vs) {
    const char *src =
      "cbuffer C : register(b0) { float4 rect; float4 color; float4 zv; };\n"
      "struct O { float4 pos : SV_Position; };\n"
      "O vs(uint id : SV_VertexID) { O o; float2 t = float2(id & 1, id >> 1);\n"
      "  o.pos = float4(lerp(rect.xy, rect.zw, t), zv.x, 1); return o; }\n"
      "void ps(O i, out float4 col : SV_Target, out float dep : SV_Depth) { col = color; dep = zv.x; }\n";
    ID3DBlob *b = nullptr, *err = nullptr;
    if (SUCCEEDED(ff->compile(src, strlen(src), "clear", NULL, NULL, "vs", "vs_4_0", 0, 0, &b, &err))) { dev->CreateVertexShader(b->GetBufferPointer(), b->GetBufferSize(), NULL, &clr_vs); b->Release(); }
    if (err) { d8_log("clear shader: %s", (const char *)err->GetBufferPointer()); err->Release(); err = nullptr; }
    if (SUCCEEDED(ff->compile(src, strlen(src), "clear", NULL, NULL, "ps", "ps_4_0", 0, 0, &b, &err))) { dev->CreatePixelShader(b->GetBufferPointer(), b->GetBufferSize(), NULL, &clr_ps); b->Release(); }
    if (err) { d8_log("clear shader: %s", (const char *)err->GetBufferPointer()); err->Release(); }
    D3D11_BUFFER_DESC bd = {}; bd.ByteWidth = 48; bd.Usage = D3D11_USAGE_DYNAMIC; bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    dev->CreateBuffer(&bd, NULL, &clr_cb);
  }
  if (!clr_vs || !clr_ps || !clr_cb) return;
  forget();                                                // the binds below are not the filter's
  D3D11_BLEND_DESC bd = {}; bd.RenderTarget[0].RenderTargetWriteMask = (c.flags & D3DCLEAR_TARGET) ? 0xF : 0;
  D3D11_DEPTH_STENCIL_DESC dd = {};
  dd.DepthEnable = (c.flags & D3DCLEAR_ZBUFFER) ? TRUE : FALSE; dd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; dd.DepthFunc = D3D11_COMPARISON_ALWAYS;
  dd.StencilEnable = (c.flags & D3DCLEAR_STENCIL) ? TRUE : FALSE; dd.StencilReadMask = 0xFF; dd.StencilWriteMask = 0xFF;
  dd.FrontFace = {D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_REPLACE, D3D11_COMPARISON_ALWAYS}; dd.BackFace = dd.FrontFace;
  D3D11_RASTERIZER_DESC rd = {}; rd.FillMode = D3D11_FILL_SOLID; rd.CullMode = D3D11_CULL_NONE; rd.DepthClipEnable = FALSE;
  ctx->OMSetBlendState(cached(ff->bs, bd, [&](const D3D11_BLEND_DESC *x, ID3D11BlendState **o) { dev->CreateBlendState(x, o); }), NULL, 0xFFFFFFFF);
  ctx->OMSetDepthStencilState(cached(ff->ds, dd, [&](const D3D11_DEPTH_STENCIL_DESC *x, ID3D11DepthStencilState **o) { dev->CreateDepthStencilState(x, o); }), c.stencil & 0xFF);
  ctx->RSSetState(cached(ff->rs, rd, [&](const D3D11_RASTERIZER_DESC *x, ID3D11RasterizerState **o) { dev->CreateRasterizerState(x, o); }));
  D3D11_VIEWPORT v = {0, 0, (float)c.width, (float)c.height, 0, 1}; ctx->RSSetViewports(1, &v);
  ctx->VSSetShader(clr_vs, NULL, 0); ctx->PSSetShader(clr_ps, NULL, 0); ctx->IASetInputLayout(NULL);
  ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
  ctx->VSSetConstantBuffers(0, 1, &clr_cb); ctx->PSSetConstantBuffers(0, 1, &clr_cb);
  for (uint32_t i = 0; i < c.nrects; i++) {
    float k[12] = {r[i].x1 * 2.0f / c.width - 1, 1 - r[i].y1 * 2.0f / c.height, r[i].x2 * 2.0f / c.width - 1, 1 - r[i].y2 * 2.0f / c.height,
                   ((c.color >> 16) & 255) / 255.0f, ((c.color >> 8) & 255) / 255.0f, (c.color & 255) / 255.0f, (c.color >> 24) / 255.0f,
                   c.z, 0, 0, 0};
    D3D11_MAPPED_SUBRESOURCE m;
    if (SUCCEEDED(ctx->Map(clr_cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &m))) { memcpy(m.pData, k, sizeof k); ctx->Unmap(clr_cb, 0); }
    ctx->Draw(4, 0);
  }
}
// a scratch texture of format f at least w x h, bound as bind (shader input or render target), with its view
Exec::Scratch *Exec::scratch(std::unordered_map<uint32_t, Scratch> &m, DXGI_FORMAT f, UINT w, UINT h, UINT bind) {
  Scratch &s = m[(uint32_t)f];
  if (s.tex && s.w >= w && s.h >= h) return &s;
  if (s.srv) s.srv->Release(); if (s.rtv) s.rtv->Release(); if (s.tex) s.tex->Release(); s = Scratch();
  D3D11_TEXTURE2D_DESC td = {}; td.Width = w; td.Height = h; td.MipLevels = 1; td.ArraySize = 1; td.Format = f;
  td.SampleDesc.Count = 1; td.Usage = D3D11_USAGE_DEFAULT; td.BindFlags = bind;
  if (FAILED(dev->CreateTexture2D(&td, NULL, &s.tex))) { s.tex = nullptr; return nullptr; }
  if (bind & D3D11_BIND_SHADER_RESOURCE) dev->CreateShaderResourceView(s.tex, NULL, &s.srv);
  if (bind & D3D11_BIND_RENDER_TARGET) dev->CreateRenderTargetView(s.tex, NULL, &s.rtv);
  if ((bind & D3D11_BIND_SHADER_RESOURCE) ? !s.srv : !s.rtv) return nullptr;
  s.w = w; s.h = h;
  return &s;
}
// X_COPY when the two Direct3D 11 formats are not of one type group, so CopySubresourceRegion may not copy between them
// (D3D11, ID3D11DeviceContext::CopySubresourceRegion: the resources "must have compatible DXGI formats, which means the
// formats must be identical or at least from the same type group") while Direct3D 8 counts them as one format: our back
// buffer is B8G8R8A8 (DXGI swap chains have no B8G8R8X8) and an X8R8G8B8 surface is B8G8R8X8. The texels go through
// two scratch textures: the source's, copied into (same format) and read with Load (exact texels, no filtering), and the
// destination's, drawn into and copied out (same format). A B8G8R8X8 view reads alpha as 1; X8 ignores the alpha written.
void Exec::copy_draw(ID3D11Texture2D *dt, UINT dsub, UINT x, UINT y, ID3D11Texture2D *st, UINT ssub, const D3D11_BOX &bx) {
  if (!ff || !ff->compile) ff_init();
  if (!ff || !ff->compile) return;
  if (!cpy_vs) {
    const char *src =
      "Texture2D t : register(t0);\n"
      "float4 vs(uint id : SV_VertexID) : SV_Position { return float4((id & 1) ? 1 : -1, (id >> 1) ? -1 : 1, 0, 1); }\n"
      "float4 ps(float4 p : SV_Position) : SV_Target { return t.Load(int3(p.xy, 0)); }\n";
    ID3DBlob *b = nullptr, *err = nullptr;
    if (SUCCEEDED(ff->compile(src, strlen(src), "copy", NULL, NULL, "vs", "vs_4_0", 0, 0, &b, &err))) { dev->CreateVertexShader(b->GetBufferPointer(), b->GetBufferSize(), NULL, &cpy_vs); b->Release(); }
    if (err) { d8_log("copy shader: %s", (const char *)err->GetBufferPointer()); err->Release(); err = nullptr; }
    if (SUCCEEDED(ff->compile(src, strlen(src), "copy", NULL, NULL, "ps", "ps_4_0", 0, 0, &b, &err))) { dev->CreatePixelShader(b->GetBufferPointer(), b->GetBufferSize(), NULL, &cpy_ps); b->Release(); }
    if (err) { d8_log("copy shader: %s", (const char *)err->GetBufferPointer()); err->Release(); }
  }
  if (!cpy_vs || !cpy_ps) return;
  D3D11_TEXTURE2D_DESC sd, dd; st->GetDesc(&sd); dt->GetDesc(&dd);
  UINT w = bx.right - bx.left, h = bx.bottom - bx.top;
  Scratch *a = scratch(cpy_src, sd.Format, w, h, D3D11_BIND_SHADER_RESOURCE), *b = scratch(cpy_dst, dd.Format, w, h, D3D11_BIND_RENDER_TARGET);
  if (!a || !b) { d8_once("copydraw", "X_COPY between formats of different type groups: a scratch texture could not be made (not copied)"); return; }
  ctx->CopySubresourceRegion(a->tex, 0, 0, 0, 0, st, ssub, &bx);
  forget();                                                // the binds below are not the filter's
  D3D11_BLEND_DESC bd = {}; bd.RenderTarget[0].RenderTargetWriteMask = 0xF;
  D3D11_DEPTH_STENCIL_DESC ds = {};
  D3D11_RASTERIZER_DESC rd = {}; rd.FillMode = D3D11_FILL_SOLID; rd.CullMode = D3D11_CULL_NONE; rd.DepthClipEnable = FALSE;
  ctx->OMSetBlendState(cached(ff->bs, bd, [&](const D3D11_BLEND_DESC *q, ID3D11BlendState **o) { dev->CreateBlendState(q, o); }), NULL, 0xFFFFFFFF);
  ctx->OMSetDepthStencilState(cached(ff->ds, ds, [&](const D3D11_DEPTH_STENCIL_DESC *q, ID3D11DepthStencilState **o) { dev->CreateDepthStencilState(q, o); }), 0);
  ctx->RSSetState(cached(ff->rs, rd, [&](const D3D11_RASTERIZER_DESC *q, ID3D11RasterizerState **o) { dev->CreateRasterizerState(q, o); }));
  D3D11_VIEWPORT v = {0, 0, (float)w, (float)h, 0, 1}; ctx->RSSetViewports(1, &v);
  ctx->OMSetRenderTargets(1, &b->rtv, NULL);
  ctx->VSSetShader(cpy_vs, NULL, 0); ctx->PSSetShader(cpy_ps, NULL, 0); ctx->IASetInputLayout(NULL);
  ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
  ctx->PSSetShaderResources(0, 1, &a->srv);
  ctx->Draw(4, 0);
  ID3D11ShaderResourceView *none = nullptr; ctx->PSSetShaderResources(0, 1, &none);
  ctx->OMSetRenderTargets(cur_rtv ? 1 : 0, &cur_rtv, cur_dsv);   // the device's targets again (X_TARGETS set them)
  D3D11_BOX ob = {0, 0, 0, w, h, 1};
  ctx->CopySubresourceRegion(dt, dsub, x, y, 0, b->tex, 0, &ob);
}
// X_SNAP's copies the GPU has finished, into the snapshot table; never waits (D3D11_MAP_FLAG_DO_NOT_WAIT: a copy still
// running answers DXGI_ERROR_WAS_STILL_DRAWING and is tried at the next Present)
void Exec::snap_poll() {
  for (size_t i = 0; i < snap_pend.size();) {
    SnapPend q = snap_pend[i];
    auto it = res.find(q.id);
    if (it == res.end() || !it->second.stage) { snap_pend.erase(snap_pend.begin() + i); continue; }
    ExecRes &x = it->second;
    D3D11_MAPPED_SUBRESOURCE m;
    HRESULT h = ctx->Map(x.stage, 0, D3D11_MAP_READ, D3D11_MAP_FLAG_DO_NOT_WAIT, &m);
    if (h == DXGI_ERROR_WAS_STILL_DRAWING) { i++; continue; }
    snap_pend.erase(snap_pend.begin() + i);
    if (FAILED(h)) { d8_once("snapmap", "X_SNAP: Map of a staging copy failed: 0x%08lX (first failure)", (unsigned long)h); continue; }
    UINT pitch = d8_pitch((D3DFORMAT)x.d3dfmt, x.w), rows = d8_rows((D3DFORMAT)x.d3dfmt, x.h);
    SnapSlot &s = snaps[q.slot];
    // the sequence lock's writer: odd while the fields change; the fences keep the data between the two stores (ARM64)
    uint32_t v = __atomic_load_n(&s.ver, __ATOMIC_SEQ_CST);
    __atomic_store_n(&s.ver, v | 1u, __ATOMIC_SEQ_CST);
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
    s.id = q.id; s.wseq = q.wseq; s.bytes = pitch * rows;
    for (UINT y = 0; y < rows; y++) memcpy(s.data + (size_t)y * pitch, (uint8_t *)m.pData + (size_t)y * m.RowPitch, pitch);
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
    __atomic_store_n(&s.ver, (v | 1u) + 1u, __ATOMIC_SEQ_CST);
    ctx->Unmap(x.stage, 0);
    snaps_done++;
  }
}
void Exec::shutdown() {
  if (x12) { exec12_delete(x12); x12 = nullptr; return; }   // (this executor made no Direct3D 11 objects)
  for (int i = 0; i < NQ; i++) { if (q_dis[i]) q_dis[i]->Release(); if (q_beg[i]) q_beg[i]->Release(); if (q_end[i]) q_end[i]->Release(); q_dis[i] = q_beg[i] = q_end[i] = nullptr; }
  if (clr_vs) clr_vs->Release(); if (clr_ps) clr_ps->Release(); if (clr_cb) clr_cb->Release(); clr_vs = nullptr; clr_ps = nullptr; clr_cb = nullptr;
  if (cpy_vs) cpy_vs->Release(); if (cpy_ps) cpy_ps->Release(); cpy_vs = nullptr; cpy_ps = nullptr;
  for (auto *m : {&cpy_src, &cpy_dst}) { for (auto &e : *m) { if (e.second.srv) e.second.srv->Release(); if (e.second.rtv) e.second.rtv->Release(); if (e.second.tex) e.second.tex->Release(); } m->clear(); }
  // what the D3D8 side never destroyed (the tests expect none but the back buffer once the device is released)
  { size_t n = 0; for (auto &r : res) n += r.second.kind != 4; d8_log("executor shutdown: %u objects still held besides the back buffer", (unsigned)n); }
  release_swap();
  if (arena_kb) d8_log("buffer arena totals: %llu buffers placed, %llu ranges written in place, %llu Maps of an arena, %llu times an arena started again, %llu buffers back to their own",
                       (unsigned long long)ba_place, (unsigned long long)ba_range, (unsigned long long)ba_maps, (unsigned long long)ba_wraps, (unsigned long long)ba_own);
  if (merge_cap) d8_log("draw merge totals: %llu merged draws made, %llu draws they took in besides their own, %llu times the arenas started again",
                        (unsigned long long)mg_runs, (unsigned long long)mg_saved, (unsigned long long)mg_wraps);
  if (bones_cb) { bones_cb->Release(); bones_cb = nullptr; }
  for (BArena &a : ba) { if (a.b) a.b->Release(); a.b = nullptr; }
  for (SArena &a : sa) { if (a.b) a.b->Release(); a.b = nullptr; }
  for (auto &r : res) free_res(r.second);
  res.clear();
  if (ff) {
    for (auto &x : ff->vs) if (x.second) x.second->Release(); for (auto &x : ff->il) if (x.second) x.second->Release();
    for (auto &x : ff->ps) if (x.second) x.second->Release(); for (auto &x : ff->bs) if (x.second) x.second->Release();
    for (auto &x : ff->ds) if (x.second) x.second->Release(); for (auto &x : ff->rs) if (x.second) x.second->Release();
    for (auto &x : ff->ss) if (x.second) x.second->Release();
    if (ff->vcb) ff->vcb->Release(); if (ff->pcb) ff->pcb->Release(); if (ff->upvb) ff->upvb->Release(); if (ff->upib) ff->upib->Release();
    if (ff->varena) ff->varena->Release(); if (ff->parena) ff->parena->Release(); if (ff->ctx1) ff->ctx1->Release();
    delete ff; ff = nullptr;
  }
  if (ctx) ctx->Release(); if (dev) dev->Release(); ctx = nullptr; dev = nullptr;
}
// a render-target view of a surface: the back buffer, a render target, or a texture's level and face
ID3D11RenderTargetView *Exec::rtv_of(const XRef &r) {
  auto it = res.find(r.id);
  if (it == res.end() || !it->second.tex) return nullptr;
  ExecRes &x = it->second;
  uint32_t key = r.level * 8 + r.face;
  auto v = x.rtv.find(key);
  if (v != x.rtv.end()) return v->second;
  D3D11_RENDER_TARGET_VIEW_DESC rd = {}; rd.Format = x.kind == 4 ? DXGI_FORMAT_B8G8R8A8_UNORM : d8_fmt((D3DFORMAT)x.d3dfmt).dxgi;
  if (x.faces == 6) { rd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY; rd.Texture2DArray.MipSlice = r.level; rd.Texture2DArray.FirstArraySlice = r.face; rd.Texture2DArray.ArraySize = 1; }
  else { rd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; rd.Texture2D.MipSlice = r.level; }
  ID3D11RenderTargetView *view = nullptr;
  dev->CreateRenderTargetView(x.tex, &rd, &view);
  x.rtv[key] = view;
  return view;
}
static UINT sub_of(const ExecRes &x, const XRef &r) { return D3D11CalcSubresource(r.level, r.face, x.levels); }

static double qpc_ms() {
  static LARGE_INTEGER f; if (!f.QuadPart) QueryPerformanceFrequency(&f);
  LARGE_INTEGER t; QueryPerformanceCounter(&t); return (double)t.QuadPart * 1000.0 / (double)f.QuadPart;
}
// ---- buffer_arena (d8_exec.h, BArena) ---------------------------------------------------------------------------------
void Exec::arena_make() {
  if (!arena_kb || ba[0].b || !dev) return;
  D3D11_BUFFER_DESC bd = {}; bd.Usage = D3D11_USAGE_DYNAMIC; bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  UINT cap[2] = {arena_kb * 1024u, arena_kb * 256u};          // indices: a quarter of the vertex arena
  for (int i = 0; i < 2; i++) {
    bd.BindFlags = i ? D3D11_BIND_INDEX_BUFFER : D3D11_BIND_VERTEX_BUFFER; bd.ByteWidth = cap[i];
    if (FAILED(dev->CreateBuffer(&bd, NULL, &ba[i].b))) { ba[i].b = nullptr; d8_log("buffer_arena: the %s arena (%u KB) could not be made: those buffers stay their own", i ? "index" : "vertex", cap[i] / 1024); continue; }
    ba[i].cap = cap[i]; ba[i].pos = 0; ba[i].gen = 1; ba[i].fresh = true;
  }
  d8_log("buffer_arena: the game's dynamic buffers live in a %u KB vertex arena and a %u KB index arena (buffers up to an eighth of it)", cap[0] / 1024, cap[1] / 1024);
}
// room for `bytes`; when the arena is full it starts again (a new generation, its next Map a DISCARD) if may_wrap
bool Exec::arena_take(BArena &a, UINT bytes, bool may_wrap, UINT *at) {
  UINT p = (a.pos + 15) & ~15u;
  if (p + bytes > a.cap) {
    if (!may_wrap) return false;
    p = 0; a.fresh = true; a.gen++; ba_wraps++;
  }
  *at = p; a.pos = p + bytes; return true;
}
// a buffer whose place is of an older generation (or that must leave the arena): its contents into its own buffer
void Exec::arena_own(ExecRes &r) {
  D3D11_MAPPED_SUBRESOURCE m;
  if (SUCCEEDED(ctx->Map(r.buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &m))) { memcpy(m.pData, r.shadow.data(), r.shadow.size()); ctx->Unmap(r.buf, 0); r.mapped = true; }
  else d8_once("arenaown", "buffer_arena: Map of a buffer leaving the arena failed (its contents lost)");
  r.in_arena = false; ba_own++;
}
// an upload run by itself (no batch): the shadow already holds it
bool Exec::arena_upload_now(ExecRes &r, const XUpBuf &u, const uint8_t *data) {
  BArena &a = ba[r.index ? 1 : 0];
  const bool noov = (u.flags & 0x1000u) && !(u.flags & 0x2000u);
  D3D11_MAPPED_SUBRESOURCE m;
  if (noov && r.in_arena && r.agen == a.gen) {
    if (FAILED(ctx->Map(a.b, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &m))) return false;
    memcpy((uint8_t *)m.pData + r.at + u.offset, data, u.length); ctx->Unmap(a.b, 0); ba_range++; ba_maps++; ba_bytes += u.length;
    return true;
  }
  UINT at; arena_take(a, (UINT)r.shadow.size(), true, &at);
  if (FAILED(ctx->Map(a.b, 0, a.fresh ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE_NO_OVERWRITE, 0, &m))) return false;
  a.fresh = false;
  memcpy((uint8_t *)m.pData + at, r.shadow.data(), r.shadow.size()); ctx->Unmap(a.b, 0);
  r.in_arena = true; r.at = at; r.agen = a.gen; ba_place++; ba_maps++; ba_bytes += r.shadow.size();
  return true;
}
// gpu skinning: buffers pos and nrm stand for the source skinned by a palette (this record's, or the last one's)
void Exec::skin_apply(const XSkin &x, const uint8_t *data) {
  if (x.nbones) {
    last_pal = std::make_shared<SkinPal>(); last_pal->serial = ++pal_serial;
    last_pal->d.assign(64u * SKIN_BONES, 0); memcpy(last_pal->d.data(), data, 64u * (x.nbones < SKIN_BONES ? x.nbones : (uint32_t)SKIN_BONES));
  }
  auto p = res.find(x.pos), n = res.find(x.nrm);
  if (p == res.end() || n == res.end() || !res.count(x.src) || !last_pal) { d8_once("skinjob", "stream error: X_SKIN names an object not held, or has no palette (skipped)"); return; }
  p->second.skin_src = x.src; p->second.pal = last_pal; n->second.skin_src = 0; n->second.pal.reset();
}
// draw_merge's arenas: one per stream 0..3 and one of 32-bit indices, each a quarter of the buffer arena's size
void Exec::merge_make() {
  if (!merge_cap || !arena_kb || sa[0].b || !dev) return;
  D3D11_BUFFER_DESC bd = {}; bd.Usage = D3D11_USAGE_DYNAMIC; bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; bd.ByteWidth = arena_kb * 256u;
  for (int i = 0; i < 5; i++) {
    bd.BindFlags = i == 4 ? D3D11_BIND_INDEX_BUFFER : D3D11_BIND_VERTEX_BUFFER;
    if (FAILED(dev->CreateBuffer(&bd, NULL, &sa[i].b))) {
      d8_log("draw_merge: an arena (%u KB) could not be made: off", bd.ByteWidth / 1024);
      for (SArena &a : sa) { if (a.b) a.b->Release(); a.b = nullptr; }
      return;
    }
    sa[i].cap = bd.ByteWidth; sa[i].pos = 0; sa[i].fresh = true;
  }
  d8_log("draw_merge: four %u KB slot arenas for vertex streams 0 to 3 and one for 32-bit indices", bd.ByteWidth / 1024);
}
// where a draw reads a dynamic buffer
void Exec::arena_resolve(ExecRes &r, ID3D11Buffer **b, UINT *off) {
  if (r.in_arena) {
    BArena &a = ba[r.index ? 1 : 0];
    if (r.agen == a.gen) { *b = a.b; *off = r.at; return; }
    arena_own(r);
  }
  *b = r.buf; *off = 0;
}
static bool holdable(uint16_t op) {
  return op != X_INIT && op != X_RESIZE && op != X_READBACK && op != X_FINISH && op != X_PRESENT && op != X_OPTION && op != X_QUIT && op != X_PAD && op != X_MIPS;
}
enum { HOLD_MAX = 16384, HOLD_BYTES = 32 << 20 };          // a batch is flushed at this many records or copied bytes
// the last draw packet's head, copied before its record's bytes go (a flush frees the held records)
void Exec::head_keep() {
  const uint8_t *src = head_off != ~(size_t)0 ? hold.data() + head_off : head_p;
  if (src && src != head_copy.data()) { head_copy.resize(XDRAW_HEAD); memcpy(head_copy.data(), src, XDRAW_HEAD); }
  if (src) { head_p = head_copy.data(); head_off = ~(size_t)0; }
}
void Exec::run(uint16_t op, const uint8_t *p, uint32_t n, int32_t *hr, std::vector<uint8_t> *reply) {
  // ---- the Direct3D 12 executor (OPT_BACKEND), when chosen and started
  if (op == X_OPTION && n >= sizeof(XOption) && ((const XOption *)p)->key == OPT_BACKEND) {
    want12 = ((const XOption *)p)->value == 12;
    if (want12 && !x12) d8_log("backend=d3d12: the Direct3D 12 executor runs the stream (d8_exec12.h); Direct3D 11 if it cannot start");
    return;
  }
  if (want12 && !x12 && op == X_INIT) {
    x12 = exec12_new(*this);
    for (auto &o2 : opts12) { int32_t h2 = 0; exec12_run(x12, X_OPTION, o2.data(), (uint32_t)o2.size(), &h2, nullptr); }
    double t0 = timed ? qpc_ms() : 0;
    exec12_run(x12, op, p, n, hr, reply);
    if (*hr >= 0) { if (timed) timed(timed_ctx, op, qpc_ms() - t0); return; }
    d8_log("the Direct3D 12 executor could not start (0x%08X): Direct3D 11 runs the stream", (unsigned)*hr);
    exec12_delete(x12); x12 = nullptr; want12 = false;
  }
  if (x12) {
    double t0 = timed ? qpc_ms() : 0;
    exec12_run(x12, op, p, n, hr, reply);
    // (an X_DRAW_SAME is a draw: counted as one, as the Direct3D 11 path's flush counts it)
    if (timed) timed(timed_ctx, op == X_DRAW_SAME ? X_DRAW : op, qpc_ms() - t0);
    return;
  }
  if (want12 && op == X_OPTION) opts12.emplace_back(p, p + n);   // for Exec12 at X_INIT (applied here as well)
  if (op == X_DRAW_SAME) {                                 // draw_delta: the last packet's head, then this tail
    if (n < XDRAW_TAIL || (!head_p && head_off == ~(size_t)0)) { d8_once("samenohead", "stream error: X_DRAW_SAME with no draw before it (skipped)"); return; }
    if (batch) {
      size_t off = hold.size(); hold.resize(off + sizeof(XDraw));
      const uint8_t *src = head_off != ~(size_t)0 ? hold.data() + head_off : head_p;
      memcpy(hold.data() + off, src, XDRAW_HEAD); memcpy(hold.data() + off + XDRAW_HEAD, p, XDRAW_TAIL);
      Held h = {X_DRAW, (uint32_t)sizeof(XDraw), nullptr, off};
      held.push_back(h); head_off = off; head_p = nullptr;
      if (held.size() >= HOLD_MAX || hold.size() >= HOLD_BYTES) flush();
      return;
    }
    same_tmp.resize(sizeof(XDraw));
    memcpy(same_tmp.data(), head_p, XDRAW_HEAD); memcpy(same_tmp.data() + XDRAW_HEAD, p, XDRAW_TAIL);
    p = same_tmp.data(); n = (uint32_t)sizeof(XDraw); op = X_DRAW;
    if (!held.empty()) flush();
    double t0 = timed ? qpc_ms() : 0;
    exec(op, p, n, hr, reply, nullptr);
    if (timed) timed(timed_ctx, op, qpc_ms() - t0);
    return;                                                // head_p stays: the head has not changed
  }
  if (op == X_DRAW && n >= sizeof(XDraw)) {                // remember this packet's head for an X_DRAW_SAME
    if (batch && copy_held) { head_off = hold.size(); head_p = nullptr; }
    else if (batch) { head_p = p; head_off = ~(size_t)0; }
    else { head_copy.resize(XDRAW_HEAD); memcpy(head_copy.data(), p, XDRAW_HEAD); head_p = head_copy.data(); head_off = ~(size_t)0; }
  }
  if (batch && holdable(op)) {
    Held h = {op, n, p, 0};
    if (copy_held) { h.p = nullptr; h.off = hold.size(); hold.insert(hold.end(), p, p + n); }
    held.push_back(h);
    if (held.size() >= HOLD_MAX || hold.size() >= HOLD_BYTES) flush();
    return;
  }
  if (!held.empty()) flush();
  double t0 = timed ? qpc_ms() : 0;
  exec(op, p, n, hr, reply, nullptr);
  if (timed) timed(timed_ctx, op, qpc_ms() - t0);
}
// the arenas' room for held[i...]: offsets for each draw's constants and ...UP / inline index data, as many records as
// fit; then one Map of each arena used, the bytes written, one Unmap. Returns the end of the records planned. A record
// that does not fit an arena from offset 0 either (a ...UP draw larger than it) gets no offsets: it maps as before.
size_t Exec::plan(size_t i) {
  FF *f = ff; size_t N = held.size();
  const bool cb = f->varena && f->ctx1;
  struct Ar { ID3D11Buffer *b; UINT cap, *pos; UINT at; bool disc, used; uint8_t *m; } ar[4] = {
    {f->varena, FF::VAR, &f->vaPos, 0, false, false, nullptr}, {f->parena, FF::PAR, &f->paPos, 0, false, false, nullptr},
    {f->upvb, FF::UPVB, &f->upvbPos, 0, false, false, nullptr}, {f->upib, FF::UPIB, &f->upibPos, 0, false, false, nullptr}};
  for (Ar &a : ar) { a.at = *a.pos; if (a.b == f->varena || a.b == f->parena) { if (!f->cbnoov) { a.at = 0; a.disc = true; } } }
  // a slot of `bytes` aligned to `align`; ~0u when it does not fit (and the arena cannot start again: it already has
  // this chunk's data)
  auto take = [&](Ar &a, UINT bytes, UINT align, bool first) -> UINT {
    if (bytes > a.cap) return ~0u - 1;
    UINT at = (a.at + align - 1) / align * align;
    if (at + bytes > a.cap || at == 0) {
      if (a.used && at + bytes > a.cap) return ~0u;          // the chunk ends here
      at = 0; a.disc = true;
    }
    a.at = at + bytes; a.used = true; return at;
  };
  const VSConst *lv = nullptr; const PSConst *lp = nullptr; UINT lvo = 0, lpo = 0;
  bool ba_used[2] = {false, false}, ba_refd[2] = {false, false};
  // draw_merge: the run being built (its first draw's record, -1 none), how many draws it has, and whether an arena
  // was full (no more merging in this chunk)
  const bool mrg = merge_on && merge_cap && arena_kb && sa[0].b;
  chunk_no++; mops.clear();
  if (swrap) { for (SArena &a : sa) { a.pos = 0; a.fresh = true; } sgen++; slots.clear(); swrap = false; mg_wraps++; }
  for (SArena &a : sa) a.used = false;
  long run_head = -1; int run_n = 0; const XDraw *hd = nullptr; bool mfull = false;
  uint32_t hv[4] = {};                                       // the first draw's buffers' upload counts when it was planned
  // the buffer and stride stream si of a draw reads: for a skinned draw's stand-in, the mesh's source (64 bytes)
  auto m_eff = [&](const XDraw &d, const Prep &q, UINT si, uint32_t *stride) -> ExecRes * {
    if (!d.stream[si].buf) return nullptr;
    bool sk = false; if (q.skin_src) for (int e = 0; e < d.vk.nel; e++) sk |= d.vk.el[e].sem == SE_SKIN && d.vk.el[e].stream == si;
    auto it = res.find(sk ? q.skin_src : d.stream[si].buf);
    *stride = sk ? (uint32_t)sizeof(XSkinVtx) : d.stream[si].stride;
    return it == res.end() ? nullptr : &it->second;
  };
  auto m_cand = [&](const XDraw &d, const Prep &q) -> bool {
    if (d.kind != XD_INDEXED || d.topology != (uint32_t)D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST || d.raster.FillMode != D3D11_FILL_SOLID || !d.count || !d.stream[0].buf) return false;
    for (UINT si = 0; si < 16; si++) {
      if (!d.stream[si].buf) continue;
      if (si > 3 || !d.stream[si].stride) return false;
      uint32_t st2; ExecRes *r2 = m_eff(d, q, si, &st2);
      if (!r2 || r2->shadow.empty() || r2->index) return false;
    }
    auto ib = res.find(d.ib);
    if (ib == res.end() || ib->second.shadow.empty() || !ib->second.index) return false;
    return ((uint64_t)d.start + d.count) * (d.ib32 ? 4u : 2u) <= ib->second.w;
  };
  auto m_same = [&](const XDraw &d, const XDraw &h) -> bool {
    if (memcmp(&d, &h, offsetof(XDraw, stream)) || d.ib32 != h.ib32) return false;
    for (UINT si = 0; si < 16; si++) if (d.stream[si].stride != h.stream[si].stride || !d.stream[si].buf != !h.stream[si].buf) return false;
    return true;
  };
  // put draw j of a run in the arenas: its streams at a vertex slot, its indices after the run's; false: no room
  auto m_put = [&](size_t j, const XDraw &d, bool head, const uint32_t *vers) -> bool {
    SlotKey key; memset(&key, 0, sizeof key); ExecRes *rs[4] = {}; UINT L = 0;
    uint32_t strd[4] = {}, ids[4] = {};
    for (UINT si = 0; si < 4; si++) {
      if (!d.stream[si].buf) continue;
      rs[si] = m_eff(d, prep[j], si, &strd[si]);
      ids[si] = strd[si] == sizeof(XSkinVtx) && prep[j].skin_src ? prep[j].skin_src : d.stream[si].buf;
      { bool sk = false; for (int e = 0; e < d.vk.nel; e++) sk |= d.vk.el[e].sem == SE_SKIN && d.vk.el[e].stream == si; if (!sk) ids[si] = d.stream[si].buf; }
      key.v[si * 3] = ids[si]; key.v[si * 3 + 1] = vers ? vers[si] : rs[si]->ver; key.v[si * 3 + 2] = strd[si];
      UINT n = ((UINT)rs[si]->shadow.size() + strd[si] - 1) / strd[si]; if (n > L) L = n;
      if (rs[si]->shadow.size() > sa[si].cap / 2) return false;          // too large for the arenas: never merged
    }
    if ((uint64_t)d.count * 4 > sa[4].cap / 2) return false;
    if ((uint64_t)sa[4].pos + (uint64_t)d.count * 4 > sa[4].cap) { swrap = true; return false; }
    uint32_t v;
    auto f = slots.find(key);
    if (f != slots.end()) v = f->second;
    else {
      v = 0;
      for (UINT si = 0; si < 4; si++) if (rs[si]) { UINT n = (sa[si].pos + strd[si] - 1) / strd[si]; if (n > v) v = n; }
      for (UINT si = 0; si < 4; si++) if (rs[si] && (uint64_t)(v + L) * strd[si] > sa[si].cap) { swrap = true; return false; }
      for (UINT si = 0; si < 4; si++) if (rs[si]) {
        MOp o = {(uint32_t)j, 0, (uint8_t)si, v * strd[si], ids[si], (uint32_t)rs[si]->shadow.size(), 0, 0}; mops.push_back(o);
        sa[si].pos = (v + L) * strd[si]; sa[si].used = true;
      }
      slots[key] = v;
    }
    MOp o = {(uint32_t)j, (uint8_t)(d.ib32 ? 2 : 1), 4, sa[4].pos, d.ib, d.start, d.count, (int32_t)d.base + (int32_t)v}; mops.push_back(o);
    if (head) { prep[j].merged = 1; prep[j].mfirst = sa[4].pos / 4; prep[j].mcount = d.count; }
    sa[4].pos += d.count * 4; sa[4].used = true;
    return true;
  };
  size_t k = i;
  for (; k < N; k++) {
    Prep &q = prep[k]; q = {~0u, ~0u, ~0u, ~0u, ~0u, 0, 0, 0, 0, 0};
    if (held[k].op != X_DRAW) {
      const uint8_t *p = held[k].p ? held[k].p : hold.data() + held[k].off;
      // a buffer made or freed: the records after it are planned once it has run (the arena's uploads and a skinning
      // job's source are looked up here, before anything runs)
      if (held[k].op == X_BUF) { k++; break; }
      if (held[k].op == X_DESTROY) { auto it = res.find(*(const uint32_t *)p); if (it != res.end() && it->second.kind == 3) { k++; break; } }
      if (held[k].op == X_SKIN) { skin_apply(*(const XSkin *)p, p + sizeof(XSkin)); q.up = 4; }   // in the records' order; not run again
      if (held[k].op == X_UPLOAD_BUF) {
        const XUpBuf &u = *(const XUpBuf *)p;
        auto it = res.find(u.id);
        if (it != res.end()) { it->second.skin_src = 0; it->second.pal.reset(); }   // written: no longer a skinned mesh's stand-in
        if (it != res.end() && it->second.buf && arena_for(it->second) && (uint64_t)u.offset + u.length <= it->second.shadow.size()) {
          ExecRes &r = it->second; int ai = r.index ? 1 : 0; BArena &a = ba[ai];
          const bool noov = (u.flags & 0x1000u) && !(u.flags & 0x2000u);
          // a NOOVERWRITE range into a buffer a draw of this chunk reads: the chunk ends first, so that draw is made
          // before the write, as it is without the arena (the game promises not to write what a draw still reads; the
          // batch would otherwise put the write before the draw whatever the game wrote)
          if (noov && r.in_arena && r.agen == a.gen && r.ref_chunk == chunk_no && k > i) break;
          r.ver++;
          if (noov && r.in_arena && r.agen == a.gen) { q.up = 2; q.up_at = r.at + u.offset; ba_used[ai] = true; }
          else {
            UINT at;
            // the arena may start again only before this chunk has put or read anything in it
            if (!arena_take(a, (UINT)r.shadow.size(), !ba_used[ai] && !ba_refd[ai], &at)) break;
            q.up = 1; q.up_at = at; r.in_arena = true; r.at = at; r.agen = a.gen; ba_used[ai] = true;
          }
        } else if (it != res.end() && it->second.buf && !it->second.dyn && !it->second.shadow.empty() &&
                   (uint64_t)u.offset + u.length <= it->second.shadow.size()) { q.up = 3; it->second.ver++; }   // draw_merge's copy of a static buffer
      }
      if (!q.up) run_head = -1;                                // anything but a buffer upload between two draws ends a run
    }
    if (held[k].op != X_DRAW) continue;
    const uint8_t *p = held[k].p ? held[k].p : hold.data() + held[k].off;
    const XDraw &d = *(const XDraw *)p;
    Ar save[4]; memcpy(save, ar, sizeof ar);
    bool stop = false;
    if (cb) {
      if (filter && lv && !memcmp(lv, &d.vc, sizeof d.vc)) q.vc = lvo;
      else { UINT o = take(ar[0], FF::VSLOT, 256, k == i); if (o == ~0u) stop = true; else { q.vc = o; lv = &d.vc; lvo = o; } }
      if (!stop) {
        if (filter && lp && !memcmp(lp, &d.pc, sizeof d.pc)) q.pc = lpo;
        else { UINT o = take(ar[1], FF::PSLOT, 256, k == i); if (o == ~0u) stop = true; else { q.pc = o; lp = &d.pc; lpo = o; } }
      }
    }
    if (!stop && (d.kind == XD_UP || d.kind == XD_UP_INDEXED)) {
      UINT o = take(ar[2], d.up_vbytes, d.up_stride ? d.up_stride : 1, k == i);
      if (o == ~0u) stop = true; else if (o != ~0u - 1) q.vb = o;
    }
    if (!stop && (d.kind == XD_UP_INDEXED || d.kind == XD_INLINE_INDEXED) && (d.kind != XD_UP_INDEXED || q.vb != ~0u)) {
      UINT o = take(ar[3], d.ibytes, d.i32 ? 4 : 2, k == i);
      if (o == ~0u) stop = true; else if (o != ~0u - 1) q.ib = o;
    }
    if (stop) {                                                // this record starts the next chunk
      memcpy(ar, save, sizeof ar); q = {~0u, ~0u, ~0u, ~0u, ~0u, 0, 0, 0, 0, 0}; run_head = -1;
      if (k == i) k++;                                         // not expected: a first record fits from offset 0
      break;
    }
    if (arena_kb && d.kind != XD_UP && d.kind != XD_UP_INDEXED) {   // where this draw reads its buffers, as of this record
      PrepBufs pb = {};
      for (UINT si = 0; si < 16; si++) {
        if (!d.stream[si].buf) continue;
        auto it = res.find(d.stream[si].buf);
        if (it == res.end() || !it->second.buf) continue;
        if (it->second.dyn) { arena_resolve(it->second, &pb.sb[si], &pb.so[si]); if (pb.sb[si] == ba[0].b) { ba_refd[0] = true; it->second.ref_chunk = chunk_no; } }
        else pb.sb[si] = it->second.buf;
      }
      if (d.kind == XD_INDEXED) {
        auto it = res.find(d.ib);
        if (it != res.end() && it->second.buf) {
          if (it->second.dyn) { arena_resolve(it->second, &pb.ib, &pb.io); if (pb.ib == ba[1].b) { ba_refd[1] = true; it->second.ref_chunk = chunk_no; } }
          else pb.ib = it->second.buf;
        }
      }
      q.bufs = (uint32_t)prep_bufs.size(); prep_bufs.push_back(pb);
    }
    // gpu skinning: the source and palette this draw's stand-in buffer has as of this record
    int sks = -1; for (int e = 0; e < d.vk.nel; e++) if (d.vk.el[e].sem == SE_SKIN) sks = d.vk.el[e].stream;
    if (sks >= 0) {
      auto it = res.find(d.stream[sks].buf);
      if (it == res.end() || !it->second.skin_src || !it->second.pal || !res.count(it->second.skin_src)) q.skin_src = ~0u;
      else { q.skin_src = it->second.skin_src; prep_pals.push_back(it->second.pal); q.skin_pal = (uint32_t)prep_pals.size(); }
    }
    // draw_merge: this draw joins the run when its packet equals the run's first in all but buffers and range
    if (mrg && !mfull && q.skin_src != ~0u && m_cand(d, q)) {
      if (run_head >= 0 && m_same(d, *hd) && (!q.skin_src || prep_pals[q.skin_pal - 1]->serial == prep_pals[prep[run_head].skin_pal - 1]->serial)) {
        bool ok = run_n > 1 || m_put((size_t)run_head, *hd, true, hv);
        if (ok && m_put(k, d, false, nullptr)) { q.merged = 2; prep[run_head].mcount += d.count; run_n++; mg_saved++; }
        else { mfull = swrap; run_head = -1; }
      } else {
        run_head = (long)k; hd = &d; run_n = 1;
        for (UINT si = 0; si < 4; si++) { uint32_t st2; ExecRes *r2 = m_eff(d, q, si, &st2); hv[si] = r2 ? r2->ver : 0; }
      }
    } else run_head = -1;
  }
  // the buffer arenas this chunk writes: one Map each, every upload's bytes in the records' order
  if (arena_kb) {
    for (int ai = 0; ai < 2; ai++) {
      BArena &a = ba[ai]; a.m = nullptr;
      if (!ba_used[ai]) continue;
      D3D11_MAPPED_SUBRESOURCE ms;
      if (FAILED(ctx->Map(a.b, 0, a.fresh ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE_NO_OVERWRITE, 0, &ms))) { d8_once("arenamap", "buffer_arena: Map of an arena failed (those uploads lost)"); continue; }
      a.fresh = false; a.m = (uint8_t *)ms.pData; ba_maps++;
    }
    for (SArena &a : sa) {
      a.m = nullptr;
      if (!a.used) continue;
      D3D11_MAPPED_SUBRESOURCE ms;
      if (FAILED(ctx->Map(a.b, 0, a.fresh ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE_NO_OVERWRITE, 0, &ms))) { d8_once("mergemap", "draw_merge: Map of an arena failed (those draws lost)"); continue; }
      a.fresh = false; a.m = (uint8_t *)ms.pData; ba_maps++;
    }
    std::stable_sort(mops.begin(), mops.end(), [](const MOp &x, const MOp &y) { return x.j < y.j; });
    size_t mi = 0;
    for (size_t j = i; j < k; j++) {
      const Prep &q = prep[j];
      if (q.up && q.up != 4) {
        const uint8_t *p = held[j].p ? held[j].p : hold.data() + held[j].off;
        const XUpBuf &u = *(const XUpBuf *)p;
        ExecRes &r = res.find(u.id)->second; BArena &a = ba[r.index ? 1 : 0];
        memcpy(r.shadow.data() + u.offset, p + sizeof u, u.length);
        if (q.up == 3 || !a.m) {}
        else if (q.up == 1) { memcpy(a.m + q.up_at, r.shadow.data(), r.shadow.size()); ba_place++; ba_bytes += r.shadow.size(); }
        else { memcpy(a.m + q.up_at, p + sizeof u, u.length); ba_range++; ba_bytes += u.length; }
      }
      // draw_merge: this draw's buffers and indices, from the executor's copies as they are at this record
      for (; mi < mops.size() && mops[mi].j == j; mi++) {
        const MOp &o = mops[mi]; SArena &a = sa[o.s];
        if (!a.m) continue;
        ExecRes &r = res.find(o.id)->second;
        if (o.kind == 0) { memcpy(a.m + o.dst, r.shadow.data(), o.a); mg_bytes += o.a; }
        else {
          uint32_t *out = (uint32_t *)(a.m + o.dst);
          if (o.kind == 1) { const uint16_t *in = (const uint16_t *)r.shadow.data() + o.a; for (uint32_t t = 0; t < o.b; t++) out[t] = (uint32_t)((int32_t)in[t] + o.add); }
          else { const uint32_t *in = (const uint32_t *)r.shadow.data() + o.a; for (uint32_t t = 0; t < o.b; t++) out[t] = (uint32_t)((int32_t)in[t] + o.add); }
          mg_bytes += o.b * 4;
        }
      }
    }
    for (int ai = 0; ai < 2; ai++) if (ba[ai].m) { ctx->Unmap(ba[ai].b, 0); ba[ai].m = nullptr; }
    for (SArena &a : sa) if (a.m) { ctx->Unmap(a.b, 0); a.m = nullptr; }
  }
  // map each arena used once
  for (Ar &a : ar) {
    if (!a.used) continue;
    D3D11_MAPPED_SUBRESOURCE ms;
    if (FAILED(ctx->Map(a.b, 0, a.disc ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE_NO_OVERWRITE, 0, &ms))) { a.m = nullptr; continue; }
    a.m = (uint8_t *)ms.pData; arena_maps++;
  }
  UINT wv = ~0u, wp = ~0u;
  for (size_t j = i; j < k; j++) {
    Prep &q = prep[j];
    if (held[j].op != X_DRAW) continue;
    const uint8_t *p = held[j].p ? held[j].p : hold.data() + held[j].off;
    const XDraw &d = *(const XDraw *)p; const uint8_t *data = p + sizeof(XDraw);
    if (q.vc != ~0u && !(ar[0].m && ar[1].m)) q.vc = q.pc = ~0u;
    if (q.vc != ~0u && q.vc != wv) { memcpy(ar[0].m + q.vc, &d.vc, sizeof d.vc); wv = q.vc; }
    if (q.pc != ~0u && q.pc != wp) { memcpy(ar[1].m + q.pc, &d.pc, sizeof d.pc); wp = q.pc; }
    if (q.vb != ~0u) { if (ar[2].m) memcpy(ar[2].m + q.vb, data, d.up_vbytes); else q.vb = ~0u; }
    if (q.ib != ~0u) {
      if (ar[3].m) memcpy(ar[3].m + q.ib, d.kind == XD_INLINE_INDEXED ? data : data + d.up_vbytes, d.ibytes); else q.ib = ~0u;
    }
    if (d.kind == XD_UP_INDEXED && (q.vb == ~0u) != (q.ib == ~0u)) q.vb = q.ib = ~0u;   // both or neither
  }
  for (Ar &a : ar) if (a.m) { ctx->Unmap(a.b, 0); }
  for (Ar &a : ar) if (a.used) *a.pos = a.at;
  return k;
}
void Exec::flush() {
  if (held.empty()) return;
  flushes++;
  prep.resize(held.size()); prep_bufs.clear(); prep_pals.clear();
  size_t i = 0, N = held.size();
  while (i < N) {
    double t0 = timed ? qpc_ms() : 0;
    size_t k = plan(i);
    double extra = timed ? qpc_ms() - t0 : 0; plan_ms += extra;
    for (size_t j = i; j < k; j++) {
      const Held &h = held[j]; const uint8_t *p = h.p ? h.p : hold.data() + h.off;
      int32_t hr = 0; double t1 = timed ? qpc_ms() : 0;
      exec(h.op, p, h.n, &hr, nullptr, &prep[j]);
      if (timed) { double ms = qpc_ms() - t1; if (h.op == X_DRAW) { ms += extra; extra = 0; } timed(timed_ctx, h.op, ms); }
    }
    if (timed && extra > 0) timed(timed_ctx, X_DRAW, extra);   // a chunk without draws: charged as a draw's time
    i = k;
  }
  head_keep();                                             // before the held records' bytes go
  held.clear(); hold.clear();
}
void Exec::exec(uint16_t op, const uint8_t *p, uint32_t n, int32_t *hr, std::vector<uint8_t> *reply, const Prep *pp) {
  switch (op) {
  case X_INIT: case X_RESIZE: {
    const XInit &x = *(const XInit *)p;
    *hr = init(x, op == X_RESIZE);
    if (*hr >= 0 && x.ds_format) {                          // the automatic depth surface
      auto old = res.find(x.ds_id); if (old != res.end()) { free_res(old->second); res.erase(old); }
      XTex t = {x.ds_id, XT_DEPTH, x.width, x.height, 1, 1, x.ds_format, D3DUSAGE_DEPTHSTENCIL};
      int32_t h2 = 0; exec(X_TEX, (const uint8_t *)&t, sizeof t, &h2, nullptr, nullptr);
    }
    break; }
  case X_TEX: {
    const XTex &t = *(const XTex *)p;
    if (res.count(t.id)) { d8_log("stream error: object %u created twice (the earlier one is replaced)", t.id); free_res(res[t.id]); res.erase(t.id); }
    FmtInfo fi = d8_fmt((D3DFORMAT)t.d3dfmt);
    ExecRes x; x.kind = t.kind; x.w = t.width; x.h = t.height; x.levels = t.levels; x.faces = t.faces; x.d3dfmt = t.d3dfmt;
    D3D11_TEXTURE2D_DESC td = {}; td.Width = t.width; td.Height = t.height; td.MipLevels = t.levels; td.ArraySize = t.faces;
    td.Format = fi.dxgi; td.SampleDesc.Count = 1; td.Usage = D3D11_USAGE_DEFAULT;
    if (t.kind == XT_DEPTH) td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    else td.BindFlags = D3D11_BIND_SHADER_RESOURCE | ((t.kind == XT_RENDERTARGET || (t.usage & D3DUSAGE_RENDERTARGET)) ? D3D11_BIND_RENDER_TARGET : 0);
    td.MiscFlags = t.faces == 6 ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0;
    HRESULT h = dev->CreateTexture2D(&td, NULL, &x.tex);
    if (FAILED(h)) { d8_log("CreateTexture2D %ux%u format %u levels %u: 0x%08lX", t.width, t.height, t.d3dfmt, t.levels, (unsigned long)h); break; }
    if (t.kind == XT_DEPTH) dev->CreateDepthStencilView(x.tex, NULL, &x.dsv);
    else dev->CreateShaderResourceView(x.tex, NULL, &x.srv);
    res[t.id] = x;
    break; }
  case X_BUF: {
    const XBuf &b = *(const XBuf *)p;
    if (res.count(b.id)) { d8_log("stream error: object %u created twice (the earlier one is replaced)", b.id); free_res(res[b.id]); res.erase(b.id); }
    ExecRes x; x.kind = 3; x.index = b.index; x.w = b.length;
    D3D11_BUFFER_DESC bd = {}; bd.ByteWidth = (b.length + 15) & ~15u; bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = b.index ? D3D11_BIND_INDEX_BUFFER : D3D11_BIND_VERTEX_BUFFER;
    if (b.dyn) { bd.Usage = D3D11_USAGE_DYNAMIC; bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; x.dyn = true; x.shadow.assign(bd.ByteWidth, 0); }
    if (FAILED(dev->CreateBuffer(&bd, NULL, &x.buf))) { d8_log("CreateBuffer %u bytes failed", b.length); break; }
    if (b.dyn) arena_make();
    // draw_merge reads a draw's buffers from the executor's own copies: one for every buffer up to 256 KB (a chosen
    // bound: in thread mode the copies are in the game's 32-bit address space; mg_kept counts them)
    if (merge_cap && !b.dyn && b.length <= (256u << 10)) { x.shadow.assign(bd.ByteWidth, 0); mg_kept += bd.ByteWidth; }
    if (merge_cap) merge_make();
    res[b.id] = x;
    break; }
  case X_DESTROY: {
    auto it = res.find(*(const uint32_t *)p);
    if (it == res.end()) d8_once("destroyunknown", "stream error: object %u destroyed but not held (its creation failed, or a stream error)", *(const uint32_t *)p);
    if (it != res.end()) {
      for (size_t i = 0; i < snap_pend.size(); i++) if (snap_pend[i].id == it->first) { snap_pend.erase(snap_pend.begin() + i); break; }
      for (auto &v : it->second.rtv) if (v.second == cur_rtv) cur_rtv = nullptr;
      if (it->second.dsv == cur_dsv) cur_dsv = nullptr;
      if (!it->second.dyn && !it->second.shadow.empty()) mg_kept -= it->second.shadow.size();
      free_res(it->second); res.erase(it);
      forget();                                            // a view or buffer the filter holds may be this object's
    }
    break; }
  case X_UPLOAD_TEX: {
    const XUpTex &u = *(const XUpTex *)p;
    auto it = res.find(u.id); if (it == res.end() || !it->second.tex) break;
    D3D11_BOX bx = {u.x, u.y, 0, u.x + u.w, u.y + u.h, 1};
    ctx->UpdateSubresource(it->second.tex, u.sub, u.w ? &bx : NULL, p + sizeof u, u.rowbytes, 0);
    up_bytes[0] += (uint64_t)u.rowbytes * u.rows; up_n[0]++;
    if (it->second.up_frame != ~0ull && presents - it->second.up_frame <= 1) up_again[0] += (uint64_t)u.rowbytes * u.rows;
    it->second.up_frame = presents;
    break; }
  case X_MIPS: {                                           // mip_exec: levels 1 .. from level 0 (d8_mip.cpp)
    const XMips &m = *(const XMips *)p;
    auto it = res.find(m.id); if (it == res.end() || !it->second.tex || m.levels < 2) break;
    std::vector<std::vector<uint8_t>> lv;
    int k = mip_build((D3DFORMAT)m.fmt, m.w, m.h, p + sizeof m, m.levels, lv);
    for (int i = 0; i < k; i++) {
      UINT l = (UINT)i + 1, lw = m.w >> l ? m.w >> l : 1;
      ctx->UpdateSubresource(it->second.tex, D3D11CalcSubresource(l, 0, m.levels), NULL, lv[i].data(), d8_pitch((D3DFORMAT)m.fmt, lw), 0);
    }
    break; }
  case X_UPLOAD_BUF: {
    const XUpBuf &u = *(const XUpBuf *)p;
    auto it = res.find(u.id); if (it == res.end() || !it->second.buf) break;
    up_bytes[1] += u.length; up_n[1]++;
    ExecRes &r = it->second;
    if (r.up_frame != ~0ull && presents - r.up_frame <= 1) up_again[1] += u.length;
    r.up_frame = presents;
    if (!pp) { r.skin_src = 0; r.pal.reset(); }             // written: no longer a skinned mesh's stand-in (a batch did this in Exec::plan)
    if (pp && (pp->up == 1 || pp->up == 2)) break;         // buffer_arena: the batch has written it (Exec::plan)
    if (!r.dyn && !r.shadow.empty() && !(pp && pp->up == 3) && (uint64_t)u.offset + u.length <= r.shadow.size()) {
      memcpy(r.shadow.data() + u.offset, p + sizeof u, u.length); r.ver++;   // draw_merge's copy (a batch has done this)
    }
    if (!r.dyn) { D3D11_BOX bx = {u.offset, 0, 0, u.offset + u.length, 1, 1}; ctx->UpdateSubresource(r.buf, 0, &bx, p + sizeof u, 0, 0); break; }
    // a dynamic buffer (dynamic_buffers): the shadow holds what the game has written; NOOVERWRITE (every lock had it,
    // and the buffer has been mapped): the range beside what the GPU reads; otherwise DISCARD, fresh memory, and the
    // whole shadow into it, so the buffer holds what UpdateSubresource would have left in it
    if ((uint64_t)u.offset + u.length > r.shadow.size()) { d8_once("dynrange", "a dynamic buffer upload past its end (skipped)"); break; }
    memcpy(r.shadow.data() + u.offset, p + sizeof u, u.length); r.ver++;
    if (arena_for(r) && arena_upload_now(r, u, p + sizeof u)) break;
    if (r.in_arena) r.in_arena = false;                    // the arena could not take it: its own buffer, whole
    bool noov = (u.flags & 0x1000u) && !(u.flags & 0x2000u) && r.mapped;
    D3D11_MAPPED_SUBRESOURCE m;
    if (FAILED(ctx->Map(r.buf, 0, noov ? D3D11_MAP_WRITE_NO_OVERWRITE : D3D11_MAP_WRITE_DISCARD, 0, &m))) { d8_once("dynmap", "Map of a dynamic buffer failed (that upload lost)"); break; }
    if (noov) { memcpy((uint8_t *)m.pData + u.offset, r.shadow.data() + u.offset, u.length); if (!dyn_noov++) d8_log("dynamic buffers: the first NOOVERWRITE Map"); }
    else { memcpy(m.pData, r.shadow.data(), r.shadow.size()); if (!dyn_discard++) d8_log("dynamic buffers: the first DISCARD Map"); }
    ctx->Unmap(r.buf, 0); r.mapped = true;
    break; }
  case X_SKIN: if (!pp) skin_apply(*(const XSkin *)p, p + sizeof(XSkin)); break;   // (a batch applied it in Exec::plan)
  case X_TARGETS: {
    const XTargets &t = *(const XTargets *)p;
    cur_rtv = t.rt.id ? rtv_of(t.rt) : nullptr; last_rt = t.rt; last_ds = t.ds;
    auto d = t.ds ? res.find(t.ds) : res.end();
    cur_dsv = d != res.end() ? d->second.dsv : nullptr;
    ctx->OMSetRenderTargets(1, &cur_rtv, cur_dsv);
    forget();                                              // a render target bound as a texture is unbound by D3D11
    break; }
  case X_CLEAR: {
    const XClear &c = *(const XClear *)p;
    if (!c.nrects) {
      if ((c.flags & D3DCLEAR_TARGET) && cur_rtv) {
        float f[4] = {((c.color >> 16) & 255) / 255.0f, ((c.color >> 8) & 255) / 255.0f, (c.color & 255) / 255.0f, (c.color >> 24) / 255.0f};
        ctx->ClearRenderTargetView(cur_rtv, f);
      }
      UINT df = ((c.flags & D3DCLEAR_ZBUFFER) ? D3D11_CLEAR_DEPTH : 0) | ((c.flags & D3DCLEAR_STENCIL) ? D3D11_CLEAR_STENCIL : 0);
      if (df && cur_dsv) ctx->ClearDepthStencilView(cur_dsv, df, c.z, (UINT8)c.stencil);
    } else clear_rects(c, (const XRect *)(&c + 1));
    break; }
  case X_COPY: {
    const XCopy &c = *(const XCopy *)p;
    auto s = res.find(c.src.id), d = res.find(c.dst.id);
    if (s == res.end() || d == res.end() || !s->second.tex || !d->second.tex) break;
    D3D11_BOX bx = {(UINT)c.left, (UINT)c.top, 0, (UINT)c.right, (UINT)c.bottom, 1};
    D3D11_TEXTURE2D_DESC sd, dd; s->second.tex->GetDesc(&sd); d->second.tex->GetDesc(&dd);
    // CopyRects requires one D3D8 format and d8_fmt gives each its one DXGI format, so only the back buffer (always
    // B8G8R8A8, X_INIT) can differ: from an X8R8G8B8 surface's B8G8R8X8, which is not of its type group
    if (sd.Format != dd.Format)
      copy_draw(d->second.tex, sub_of(d->second, c.dst), c.x, c.y, s->second.tex, sub_of(s->second, c.src), bx);
    else ctx->CopySubresourceRegion(d->second.tex, sub_of(d->second, c.dst), c.x, c.y, 0, s->second.tex, sub_of(s->second, c.src), &bx);
    break; }
  case X_DRAW: if (pp && pp->merged == 2) { draws++; break; }   // draw_merge: drawn by its run's first draw
    last_site = ((const XDraw *)p)->site; draw(*(const XDraw *)p, p + sizeof(XDraw), pp); draws++; break;
  case X_PRESENT: {
    drain_debug();
    gpu_frame_end();
    HRESULT h = swap ? swap->Present(sync, 0) : E_FAIL;
    // (the removal reason is the device's own answer: 0x887A0006 the GPU hung on what it was sent, 0x887A0007 a reset,
    // 0x887A0020 a driver fault, 0x887A002B the device was taken away by the system, e.g. the session or the VM's adapter)
    if (FAILED(h)) d8_once("present", "IDXGISwapChain::Present: 0x%08lX (first failure); the device's removal reason: 0x%08lX; frame %llu, %llu draws so far",
                           (unsigned long)h, dev ? (unsigned long)dev->GetDeviceRemovedReason() : 0ul, (unsigned long long)presents, (unsigned long long)draws);
    presents++;
    if (d3d12_want && !d3d12_done && presents == 1) { d3d12_done = true; d3d12_probe(); }
    if (flip && swap) {                                    // the flip model unbinds the back buffer at each Present: bound again
      cur_rtv = last_rt.id ? rtv_of(last_rt) : nullptr;
      auto d = last_ds ? res.find(last_ds) : res.end(); cur_dsv = d != res.end() ? d->second.dsv : nullptr;
      ctx->OMSetRenderTargets(1, &cur_rtv, cur_dsv); forget();
    }
    gpu_frame_begin();
    if (!snap_pend.empty()) snap_poll();
    break; }
  case X_SNAP: {
    const XSnap &q = *(const XSnap *)p;
    auto it = res.find(q.dst.id);
    if (!snaps || q.slot >= SNAP_SLOTS || it == res.end() || !it->second.tex) break;
    ExecRes &x = it->second;
    if ((size_t)d8_pitch((D3DFORMAT)x.d3dfmt, x.w) * d8_rows((D3DFORMAT)x.d3dfmt, x.h) > SNAP_BYTES) break;
    if (!x.stage) {
      D3D11_TEXTURE2D_DESC td; x.tex->GetDesc(&td);
      D3D11_TEXTURE2D_DESC sd = {}; sd.Width = x.w; sd.Height = x.h; sd.MipLevels = 1; sd.ArraySize = 1; sd.Format = td.Format;
      sd.SampleDesc.Count = 1; sd.Usage = D3D11_USAGE_STAGING; sd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
      if (FAILED(dev->CreateTexture2D(&sd, NULL, &x.stage))) { x.stage = nullptr; d8_once("snapstage", "X_SNAP: a staging texture could not be made (snapshots off for it)"); break; }
    }
    ctx->CopySubresourceRegion(x.stage, 0, 0, 0, 0, x.tex, sub_of(x, q.dst), NULL);
    bool had = false;
    for (auto &e : snap_pend) if (e.id == q.dst.id) { e.slot = q.slot; e.wseq = q.wseq; had = true; break; }
    if (!had) snap_pend.push_back({q.dst.id, q.slot, q.wseq});
    break; }
  case X_READBACK: {
    const XRef &r = *(const XRef *)p;
    auto it = res.find(r.id);
    *hr = D3DERR_INVALIDCALL;
    if (it == res.end() || !it->second.tex) break;
    ExecRes &x = it->second;
    UINT w = x.w >> r.level ? x.w >> r.level : 1, h = x.h >> r.level ? x.h >> r.level : 1;
    D3D11_TEXTURE2D_DESC td; x.tex->GetDesc(&td);
    D3D11_TEXTURE2D_DESC sd = {}; sd.Width = w; sd.Height = h; sd.MipLevels = 1; sd.ArraySize = 1; sd.Format = td.Format;
    sd.SampleDesc.Count = 1; sd.Usage = D3D11_USAGE_STAGING; sd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    ID3D11Texture2D *st = NULL;
    if (FAILED(dev->CreateTexture2D(&sd, NULL, &st))) break;
    ctx->CopySubresourceRegion(st, 0, 0, 0, 0, x.tex, sub_of(x, r), NULL);
    D3D11_MAPPED_SUBRESOURCE m;
    if (SUCCEEDED(ctx->Map(st, 0, D3D11_MAP_READ, 0, &m))) {
      UINT pitch = d8_pitch((D3DFORMAT)x.d3dfmt, w), rows = d8_rows((D3DFORMAT)x.d3dfmt, h);
      reply->resize((size_t)pitch * rows);
      for (UINT y = 0; y < rows; y++) memcpy(reply->data() + (size_t)y * pitch, (uint8_t *)m.pData + (size_t)y * m.RowPitch, pitch);
      ctx->Unmap(st, 0); *hr = D3D_OK;
    }
    st->Release();
    break; }
  case X_FINISH: *hr = D3D_OK; break;
  case X_OPTION: {
    const XOption &o = *(const XOption *)p;
    if (o.key == OPT_FILTER) { filter = o.value; forget(); }
    if (o.key == OPT_BATCH) { batch = o.value; forget(); }
    if (o.key == OPT_SITES) sites_on = o.value != 0;
    if (o.key == OPT_BUFARENA && !ba[0].b) { arena_kb = o.value; arena_make(); }   // a start setting
    if (o.key == OPT_MERGE) {                              // bit 1 at the start: keep copies; bit 0: merge now (switched live)
      if ((o.value & 2) && res.size() <= 2) merge_cap = 1;
      merge_on = (o.value & 1) && merge_cap; merge_make(); forget();
    }
    if (o.key == OPT_FLIP) {                               // the model the swap chain is made in: before it exists only
      if (!swap) flip = flip_want = o.value != 0;
      else if ((o.value != 0) != flip) d8_once("flip_live", "swap_flip=%u after the swap chain was made: not applied (the model is set for the whole start)", o.value);
    }
    if (o.key == OPT_GPUTIME) gpu_on = o.value != 0;
    if (o.key == OPT_D3D12) d3d12_want = o.value != 0;
    if (o.key == OPT_LATENCY && !swap) latency = o.value;   // a start setting: applied when the swap chain is made
    memo.vc_ok = memo.pc_ok = false;                       // the constants may be in either place: written again
    break; }
  }
  (void)n;
}

// ---- a draw -------------------------------------------------------------------------------------------------------------------
void Exec::draw(const XDraw &d, const uint8_t *data, const Prep *pp) {
  if (!ff || !ff->compile) return;
  struct Split { Exec *x; double t0 = 0, t1 = 0; bool on;
    Split(Exec *e) : x(e), on((++e->split_ctr & 31) == 0) { if (on) t0 = qpc_ms(); }
    void bind() { if (on && !t1) t1 = qpc_ms(); }
    ~Split() { if (on && t1) { double t2 = qpc_ms(); x->split_prep += t1 - t0; x->split_sub += t2 - t1; x->split_n++; } } } split(this);
  // the filter (X_OPTION OPT_FILTER): a key, description or constant block equal to the last draw's reuses what that
  // draw found or wrote, and a bind equal to what this executor last set (Bound) is not made again. Direct3D 11 keeps
  // a context's state until it is set again, and a dynamic buffer's contents until it is mapped again, so the context
  // ends each draw in the same state either way; test_d3d8on11.py compares the pictures with the filter on and off.
  const bool f = filter != 0;
  if (f && !bd.valid) { forget(); bd.valid = true; }
  ID3D11VertexShader *vsh = nullptr; ID3D11InputLayout *lay = nullptr;
  if (f && memo.vk_ok && !memcmp(&memo.vk, &d.vk, sizeof d.vk)) { vsh = memo.vsh; lay = memo.lay; }
  else {
    std::string vks = kstr(d.vk);
    auto vi = ff->vs.find(vks);
    if (vi == ff->vs.end()) {
      ID3DBlob *b = compile(ff, gen_vs(d.vk), "vs_4_0");
      if (b) {
        dev->CreateVertexShader(b->GetBufferPointer(), b->GetBufferSize(), NULL, &vsh);
        D3D11_INPUT_ELEMENT_DESC ie[24]; UINT nie = 0;
        static const char *sn[] = {"POSITION", "POSITION", "NORMAL", "COLOR", "TEXCOORD", "PSIZE", "BLENDWEIGHT"};
        for (int i = 0; i < d.vk.nel; i++) {
          const VElem &e = d.vk.el[i];
          if (e.sem == SE_SKIN) {                            // gpu skinning: the source vertex's five parts (XSkinVtx)
            ie[nie++] = {"SKINA", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, e.stream, 0, D3D11_INPUT_PER_VERTEX_DATA, 0};
            ie[nie++] = {"SKINB", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, e.stream, 16, D3D11_INPUT_PER_VERTEX_DATA, 0};
            ie[nie++] = {"SKINNA", 0, DXGI_FORMAT_R32G32B32_FLOAT, e.stream, 32, D3D11_INPUT_PER_VERTEX_DATA, 0};
            ie[nie++] = {"SKINNB", 0, DXGI_FORMAT_R32G32B32_FLOAT, e.stream, 44, D3D11_INPUT_PER_VERTEX_DATA, 0};
            ie[nie++] = {"SKINI", 0, DXGI_FORMAT_R16G16_UINT, e.stream, 56, D3D11_INPUT_PER_VERTEX_DATA, 0};
            continue;
          }
          ie[nie++] = {sn[e.sem], e.sem == SE_POST ? 0u : e.idx, vdxgi(e.fmt), e.stream, e.off, D3D11_INPUT_PER_VERTEX_DATA, 0};
        }
        if (FAILED(dev->CreateInputLayout(ie, nie, b->GetBufferPointer(), b->GetBufferSize(), &lay))) d8_log("CreateInputLayout failed");
        b->Release();
      }
      ff->vs[vks] = vsh; ff->il[vks] = lay;
    } else { vsh = vi->second; lay = ff->il[vks]; }
    memo.vk = d.vk; memo.vsh = vsh; memo.lay = lay; memo.vk_ok = true;
  }
  ID3D11PixelShader *psh = nullptr;
  if (f && memo.pk_ok && !memcmp(&memo.pk, &d.pk, sizeof d.pk)) psh = memo.psh;
  else {
    std::string pks = kstr(d.pk);
    auto pi = ff->ps.find(pks);
    if (pi == ff->ps.end()) {
      ID3DBlob *b = compile(ff, gen_ps(d.pk), "ps_4_0");
      if (b) { dev->CreatePixelShader(b->GetBufferPointer(), b->GetBufferSize(), NULL, &psh); b->Release(); }
      ff->ps[pks] = psh;
    } else psh = pi->second;
    memo.pk = d.pk; memo.psh = psh; memo.pk_ok = true;
  }
  if (!vsh || !psh || !lay) return;
  D3D11_MAPPED_SUBRESOURCE m;
  const bool arena = pp && pp->vc != ~0u;
  if (arena) {
    // the batch wrote this draw's constants into the arenas (Exec::plan); vcb and pcb keep what they had, so memo stays
    if (!f || pp->vc != bd.vco || pp->pc != bd.pco || bd.cbs) {
      UINT fv = pp->vc / 16, nv = FF::VSLOT / 16, fp = pp->pc / 16, np = FF::PSLOT / 16;
      ff->ctx1->VSSetConstantBuffers1(0, 1, &ff->varena, &fv, &nv); ff->ctx1->PSSetConstantBuffers1(0, 1, &ff->parena, &fp, &np);
      bd.vco = pp->vc; bd.pco = pp->pc; bd.cbs = false;
    }
  } else if (!f || !memo.vc_ok || memcmp(&memo.vc, &d.vc, sizeof d.vc)) {
    memo.vc_ok = false;
    if (SUCCEEDED(ctx->Map(ff->vcb, 0, D3D11_MAP_WRITE_DISCARD, 0, &m))) { memcpy(m.pData, &d.vc, sizeof d.vc); ctx->Unmap(ff->vcb, 0); memo.vc = d.vc; memo.vc_ok = true; draw_maps++; }
  }
  if (!arena && (!f || !memo.pc_ok || memcmp(&memo.pc, &d.pc, sizeof d.pc))) {
    memo.pc_ok = false;
    if (SUCCEEDED(ctx->Map(ff->pcb, 0, D3D11_MAP_WRITE_DISCARD, 0, &m))) { memcpy(m.pData, &d.pc, sizeof d.pc); ctx->Unmap(ff->pcb, 0); memo.pc = d.pc; memo.pc_ok = true; draw_maps++; }
  }
  ID3D11BlendState *bs; ID3D11DepthStencilState *dss; ID3D11RasterizerState *rs;
  if (f && memo.bs_ok && !memcmp(&memo.bsd, &d.blend, sizeof d.blend)) bs = memo.bs;
  else { bs = cached(ff->bs, d.blend, [&](const D3D11_BLEND_DESC *x, ID3D11BlendState **o) { dev->CreateBlendState(x, o); }); memo.bsd = d.blend; memo.bs = bs; memo.bs_ok = true; }
  if (f && memo.ds_ok && !memcmp(&memo.dsd, &d.depth, sizeof d.depth)) dss = memo.dss;
  else { dss = cached(ff->ds, d.depth, [&](const D3D11_DEPTH_STENCIL_DESC *x, ID3D11DepthStencilState **o) { dev->CreateDepthStencilState(x, o); }); memo.dsd = d.depth; memo.dss = dss; memo.ds_ok = true; }
  if (f && memo.rs_ok && !memcmp(&memo.rsd, &d.raster, sizeof d.raster)) rs = memo.rs;
  else { rs = cached(ff->rs, d.raster, [&](const D3D11_RASTERIZER_DESC *x, ID3D11RasterizerState **o) { dev->CreateRasterizerState(x, o); }); memo.rsd = d.raster; memo.rs = rs; memo.rs_ok = true; }
  split.bind();                                            // (the split's boundary: the first bind)
  if (!f || bs != bd.bs) { ctx->OMSetBlendState(bs, NULL, 0xFFFFFFFF); bd.bs = bs; }
  if (!f || dss != bd.dss || d.stencil_ref != bd.sref) { ctx->OMSetDepthStencilState(dss, d.stencil_ref); bd.dss = dss; bd.sref = d.stencil_ref; }
  if (!f || rs != bd.rs) { ctx->RSSetState(rs); bd.rs = rs; }
  if (!f || memcmp(&d.viewport, &bd.vp, sizeof bd.vp)) { ctx->RSSetViewports(1, &d.viewport); bd.vp = d.viewport; }
  for (UINT i = 0; i < d.nstages && i < 8; i++) {
    ID3D11ShaderResourceView *srv = nullptr; ID3D11SamplerState *smp = nullptr;
    if (d.stage[i].tex) {
      auto it = res.find(d.stage[i].tex);
      if (it != res.end()) {
        ExecRes &r = it->second; uint32_t f0 = d.stage[i].first_level, c = d.stage[i].nlevels;
        if (f0 == 0 && (c == 0 || c >= r.levels)) srv = r.srv;
        else if (r.tex && f0 < r.levels) {                       // a view of only the levels the stage may read
          if (c == 0 || f0 + c > r.levels) c = r.levels - f0;
          ID3D11ShaderResourceView *&v = r.srv_levels[f0 * 256 + c];
          if (!v) {
            D3D11_SHADER_RESOURCE_VIEW_DESC vd = {}; D3D11_TEXTURE2D_DESC td; r.tex->GetDesc(&td); vd.Format = td.Format;
            if (r.faces == 6) { vd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE; vd.TextureCube.MostDetailedMip = f0; vd.TextureCube.MipLevels = c; }
            else { vd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D; vd.Texture2D.MostDetailedMip = f0; vd.Texture2D.MipLevels = c; }
            if (FAILED(dev->CreateShaderResourceView(r.tex, &vd, &v))) d8_log("a view of levels %u..%u of object %u failed", f0, f0 + c - 1, d.stage[i].tex);
          }
          srv = v;
        }
      }
      if (f && memo.ss_ok[i] && !memcmp(&memo.ssd[i], &d.stage[i].sampler, sizeof memo.ssd[i])) smp = memo.ss[i];
      else {
        smp = cached(ff->ss, d.stage[i].sampler, [&](const D3D11_SAMPLER_DESC *x, ID3D11SamplerState **o) { dev->CreateSamplerState(x, o); });
        memo.ssd[i] = d.stage[i].sampler; memo.ss[i] = smp; memo.ss_ok[i] = true;
      }
    }
    if (!f || srv != bd.srv[i]) { ctx->PSSetShaderResources(i, 1, &srv); bd.srv[i] = srv; }
    if (!f || smp != bd.smp[i]) { ctx->PSSetSamplers(i, 1, &smp); bd.smp[i] = smp; }
  }
  if (!f || vsh != bd.vs) { ctx->VSSetShader(vsh, NULL, 0); bd.vs = vsh; }
  if (!f || psh != bd.ps) { ctx->PSSetShader(psh, NULL, 0); bd.ps = psh; }
  if (!f || lay != bd.lay) { ctx->IASetInputLayout(lay); bd.lay = lay; }
  if (!arena && (!f || !bd.cbs)) { ctx->VSSetConstantBuffers(0, 1, &ff->vcb); ctx->PSSetConstantBuffers(0, 1, &ff->pcb); bd.cbs = true; bd.vco = bd.pco = ~0u; }
  if (!f || d.topology != bd.topo) { ctx->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)d.topology); bd.topo = d.topology; }
  auto set_vb = [&](UINT i, ID3D11Buffer *b, UINT stride, UINT off = 0) {
    if (f && bd.vb[i] == b && bd.stride[i] == stride && bd.voff[i] == off) return;
    ctx->IASetVertexBuffers(i, 1, &b, &stride, &off); bd.vb[i] = b; bd.stride[i] = stride; bd.voff[i] = off;
  };
  auto set_ib = [&](ID3D11Buffer *b, bool i32, UINT off = 0) {
    DXGI_FORMAT fm = i32 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
    if (f && bd.ib == b && bd.ifmt == fm && bd.ioff == off) return;
    ctx->IASetIndexBuffer(b, fm, off); bd.ib = b; bd.ifmt = fm; bd.ioff = off;
  };
  // buffer_arena: the batch's places for this draw's buffers (Exec::plan), or, run by itself, where they are now
  const PrepBufs *pb = pp && pp->bufs != ~0u ? &prep_bufs[pp->bufs] : nullptr;
  if (d.kind == XD_UP || d.kind == XD_UP_INDEXED) {
    UINT at = 0, stride = d.up_stride;
    if (pp && pp->vb != ~0u) at = pp->vb;
    else if (!ring(ctx, ff->upvb, FF::UPVB, ff->upvbPos, data, d.up_vbytes, stride, &at)) { d8_once("upbig", "a ...UP draw larger than the ring: skipped"); return; }
    else draw_maps++;
    set_vb(0, ff->upvb, stride);
    UINT first = at / stride;
    if (d.kind == XD_UP) { ctx->Draw(d.count, first); return; }
    UINT isz = d.i32 ? 4 : 2, ia;
    if (pp && pp->ib != ~0u) ia = pp->ib;
    else if (!ring(ctx, ff->upib, FF::UPIB, ff->upibPos, data + d.up_vbytes, d.ibytes, isz, &ia)) return;
    else draw_maps++;
    set_ib(ff->upib, d.i32);
    ctx->DrawIndexed(d.count, ia / isz, (INT)first);
    return;
  }
  // gpu skinning: this draw's stream `sks` reads a mesh's source, skinned by a palette in constant buffer 1
  int sks = -1; for (int e = 0; e < d.vk.nel; e++) if (d.vk.el[e].sem == SE_SKIN) sks = d.vk.el[e].stream;
  ID3D11Buffer *skbuf = nullptr;
  if (sks >= 0) {
    std::shared_ptr<SkinPal> pal; uint32_t src = 0;
    if (pp) { if (pp->skin_src && pp->skin_src != ~0u) { src = pp->skin_src; pal = prep_pals[pp->skin_pal - 1]; } }
    else { auto it = res.find(d.stream[sks].buf); if (it != res.end()) { src = it->second.skin_src; pal = it->second.pal; } }
    auto sr = src ? res.find(src) : res.end();
    if (sr == res.end() || !sr->second.buf || !pal || !bones_cb) { d8_once("skindraw", "a skinned draw whose buffer stands for no mesh (skipped)"); return; }
    skbuf = sr->second.buf;
    if (!f || bd.pal != pal->serial) {
      if (SUCCEEDED(ctx->Map(bones_cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &m))) { memcpy(m.pData, pal->d.data(), pal->d.size()); ctx->Unmap(bones_cb, 0); draw_maps++; }
      ctx->VSSetConstantBuffers(1, 1, &bones_cb); bd.pal = pal->serial;
    }
  }
  if (pp && pp->merged == 1) {                             // draw_merge: the run, from the slot arenas (Exec::plan)
    for (UINT i = 0; i < 4; i++) if (d.stream[i].buf) set_vb(i, sa[i].b, (int)i == sks ? (UINT)sizeof(XSkinVtx) : d.stream[i].stride, 0);
    set_ib(sa[4].b, true, 0);
    ctx->DrawIndexed(pp->mcount, pp->mfirst, 0); mg_runs++;
    return;
  }
  for (UINT i = 0; i < 16; i++) {
    if (!d.stream[i].buf) continue;
    if ((int)i == sks) { set_vb(i, skbuf, (UINT)sizeof(XSkinVtx), 0); continue; }
    if (pb) {
      if (!pb->sb[i]) { d8_once("nostream", "a draw reads a stream whose buffer is gone (skipped)"); return; }
      set_vb(i, pb->sb[i], d.stream[i].stride, pb->so[i]); continue;
    }
    auto it = res.find(d.stream[i].buf);
    if (it == res.end() || !it->second.buf) { d8_once("nostream", "a draw reads a stream whose buffer is gone (skipped)"); return; }
    ID3D11Buffer *vbuf = it->second.buf; UINT voff = 0;
    if (arena_kb && it->second.dyn) arena_resolve(it->second, &vbuf, &voff);
    set_vb(i, vbuf, d.stream[i].stride, voff);
  }
  if (d.kind == XD_DRAW) { ctx->Draw(d.count, d.start); return; }
  if (d.kind == XD_INLINE_INDEXED) {
    UINT isz = d.i32 ? 4 : 2, ia;
    if (pp && pp->ib != ~0u) ia = pp->ib;
    else if (!ring(ctx, ff->upib, FF::UPIB, ff->upibPos, data, d.ibytes, isz, &ia)) return;
    else draw_maps++;
    set_ib(ff->upib, d.i32);
    ctx->DrawIndexed(d.count, ia / isz, (INT)d.base);
    return;
  }
  if (pb) {
    if (!pb->ib) { d8_once("noib", "an indexed draw whose index buffer is gone (skipped)"); return; }
    set_ib(pb->ib, d.ib32 != 0, pb->io);
  } else {
    auto ib = res.find(d.ib);
    if (ib == res.end() || !ib->second.buf) { d8_once("noib", "an indexed draw whose index buffer is gone (skipped)"); return; }
    ID3D11Buffer *ibuf = ib->second.buf; UINT ioff = 0;
    if (arena_kb && ib->second.dyn) arena_resolve(ib->second, &ibuf, &ioff);
    set_ib(ibuf, d.ib32 != 0, ioff);
  }
  ctx->DrawIndexed(d.count, d.start, (INT)d.base);
}
