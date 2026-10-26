// d8_exec.cpp -- d3d8on11's executor (d8_exec.h): Direct3D 11 objects by id, and the fixed-function pipeline as
// generated shaders. What each draw needs was decided on the D3D8 side (d8_ff.cpp: the keys, the constants, the state
// descriptions); here the keys become HLSL once, compiled with d3dcompiler_47's D3DCompile, cached, and drawn.
//
// What each behaviour should be comes from the DirectX 8 SDK reference (render states, texture stage states,
// lighting, fog, texture coordinate processing); where the reference leaves a choice open, from Wine 9.0's wined3d,
// named where used. The picture test (test/test_d3d8on11.py, mode scenes) compares with Wine's d3d8.
#include "d8_exec.h"
#include <d3dcompiler.h>
#include <d3d11_1.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

typedef HRESULT (WINAPI *PFN_Compile)(const void *, SIZE_T, const char *, const D3D_SHADER_MACRO *, ID3DInclude *,
                                       const char *, const char *, UINT, UINT, ID3DBlob **, ID3DBlob **);

static DXGI_FORMAT vdxgi(uint8_t f) {
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
static std::string gen_vs(const VSKey &k) {
  std::string s = VS_CB;
  s += "struct VI {\n";
  int has[7][8] = {};
  for (int i = 0; i < k.nel; i++) {
    const VElem &e = k.el[i]; const char *t[] = {"float", "float2", "float3", "float4", "float4", "uint4", "int2", "int4"};
    const char *sem[] = {"POSITION", "POSITIONT", "NORMAL", "COLOR", "TEXCOORD", "PSIZE", "BLENDWEIGHT"};
    char b[96]; snprintf(b, sizeof b, "  %s a%d : %s%d;\n", t[e.fmt], i, e.sem == SE_POST ? "POSITION" : sem[e.sem], e.sem == SE_POST ? 0 : e.idx);
    s += b; has[e.sem][e.idx] = i + 1;
  }
  s += "};\n" + io_struct("VO", k.flat);
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
    s += "  float2 n = float2((p.x + 0.5 - vp.x) / vp.z * 2 - 1, 1 - (p.y + 0.5 - vp.y) / vp.w * 2);\n";
    s += "  o.pos = float4(n * w, p.z * w, w);\n  float3 cp = float3(0, 0, p.z);\n";
  } else {
    s += "  float4 p = float4(" + in(SE_POS, 0, "float4(0,0,0,1)") + ".xyz, 1);\n";
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
static float texedge() { static float e = getenv("D8ON11_TEST_TEXEDGE") ? (float)atof(getenv("D8ON11_TEST_TEXEDGE")) : 0; return e; }
static float ties() { static float e = getenv("D8ON11_TEST_TIES") ? (float)atof(getenv("D8ON11_TEST_TIES")) : 0; return e > 0 ? e : 1.0f / 256; }
static std::string gen_ps(const PSKey &k) {
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
  bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; bd.ByteWidth = FF::UPVB; dev->CreateBuffer(&bd, NULL, &ff->upvb);
  bd.BindFlags = D3D11_BIND_INDEX_BUFFER; bd.ByteWidth = FF::UPIB; dev->CreateBuffer(&bd, NULL, &ff->upib);
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
  IDXGIDevice *xd = NULL; IDXGIAdapter *xa = NULL; IDXGIFactory *xf = NULL;
  dev->QueryInterface(__uuidof(IDXGIDevice), (void **)&xd); xd->GetAdapter(&xa); xa->GetParent(__uuidof(IDXGIFactory), (void **)&xf);
  DXGI_SWAP_CHAIN_DESC sd = {};
  sd.BufferDesc.Width = x.width; sd.BufferDesc.Height = x.height; sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  sd.SampleDesc.Count = 1; sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; sd.BufferCount = 1; sd.OutputWindow = wnd;
  sd.Windowed = TRUE; sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  HRESULT hr = xf->CreateSwapChain(dev, &sd, &swap);
  xf->MakeWindowAssociation(wnd, DXGI_MWA_NO_ALT_ENTER);
  xf->Release(); xa->Release(); xd->Release();
  if (FAILED(hr)) { d8_log("CreateSwapChain %ux%u: 0x%08lX", x.width, x.height, (unsigned long)hr); return D3DERR_NOTAVAILABLE; }
  ExecRes &b = res[X_BACKBUFFER]; b.kind = 4; b.w = x.width; b.h = x.height; b.d3dfmt = D3DFMT_X8R8G8B8;
  swap->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&b.tex);
  return D3D_OK;
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
  if (clr_vs) clr_vs->Release(); if (clr_ps) clr_ps->Release(); if (clr_cb) clr_cb->Release(); clr_vs = nullptr; clr_ps = nullptr; clr_cb = nullptr;
  if (cpy_vs) cpy_vs->Release(); if (cpy_ps) cpy_ps->Release(); cpy_vs = nullptr; cpy_ps = nullptr;
  for (auto *m : {&cpy_src, &cpy_dst}) { for (auto &e : *m) { if (e.second.srv) e.second.srv->Release(); if (e.second.rtv) e.second.rtv->Release(); if (e.second.tex) e.second.tex->Release(); } m->clear(); }
  // what the D3D8 side never destroyed (the tests expect none but the back buffer once the device is released)
  { size_t n = 0; for (auto &r : res) n += r.second.kind != 4; d8_log("executor shutdown: %u objects still held besides the back buffer", (unsigned)n); }
  release_swap();
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
static bool holdable(uint16_t op) {
  return op != X_INIT && op != X_RESIZE && op != X_READBACK && op != X_FINISH && op != X_PRESENT && op != X_OPTION && op != X_QUIT && op != X_PAD;
}
enum { HOLD_MAX = 16384, HOLD_BYTES = 32 << 20 };          // a batch is flushed at this many records or copied bytes
void Exec::run(uint16_t op, const uint8_t *p, uint32_t n, int32_t *hr, std::vector<uint8_t> *reply) {
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
  size_t k = i;
  for (; k < N; k++) {
    Prep &q = prep[k]; q = {~0u, ~0u, ~0u, ~0u};
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
      memcpy(ar, save, sizeof ar); q = {~0u, ~0u, ~0u, ~0u};
      if (k == i) k++;                                         // not expected: a first record fits from offset 0
      break;
    }
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
  prep.resize(held.size());
  size_t i = 0, N = held.size();
  while (i < N) {
    double t0 = timed ? qpc_ms() : 0;
    size_t k = plan(i);
    double extra = timed ? qpc_ms() - t0 : 0;
    for (size_t j = i; j < k; j++) {
      const Held &h = held[j]; const uint8_t *p = h.p ? h.p : hold.data() + h.off;
      int32_t hr = 0; double t1 = timed ? qpc_ms() : 0;
      exec(h.op, p, h.n, &hr, nullptr, &prep[j]);
      if (timed) { double ms = qpc_ms() - t1; if (h.op == X_DRAW) { ms += extra; extra = 0; } timed(timed_ctx, h.op, ms); }
    }
    if (timed && extra > 0) timed(timed_ctx, X_DRAW, extra);   // a chunk without draws: charged as a draw's time
    i = k;
  }
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
    if (FAILED(dev->CreateBuffer(&bd, NULL, &x.buf))) { d8_log("CreateBuffer %u bytes failed", b.length); break; }
    res[b.id] = x;
    break; }
  case X_DESTROY: {
    auto it = res.find(*(const uint32_t *)p);
    if (it == res.end()) d8_once("destroyunknown", "stream error: object %u destroyed but not held (its creation failed, or a stream error)", *(const uint32_t *)p);
    if (it != res.end()) {
      for (size_t i = 0; i < snap_pend.size(); i++) if (snap_pend[i].id == it->first) { snap_pend.erase(snap_pend.begin() + i); break; }
      for (auto &v : it->second.rtv) if (v.second == cur_rtv) cur_rtv = nullptr;
      if (it->second.dsv == cur_dsv) cur_dsv = nullptr;
      free_res(it->second); res.erase(it);
      forget();                                            // a view or buffer the filter holds may be this object's
    }
    break; }
  case X_UPLOAD_TEX: {
    const XUpTex &u = *(const XUpTex *)p;
    auto it = res.find(u.id); if (it == res.end() || !it->second.tex) break;
    D3D11_BOX bx = {u.x, u.y, 0, u.x + u.w, u.y + u.h, 1};
    ctx->UpdateSubresource(it->second.tex, u.sub, u.w ? &bx : NULL, p + sizeof u, u.rowbytes, 0);
    break; }
  case X_UPLOAD_BUF: {
    const XUpBuf &u = *(const XUpBuf *)p;
    auto it = res.find(u.id); if (it == res.end() || !it->second.buf) break;
    D3D11_BOX bx = {u.offset, 0, 0, u.offset + u.length, 1, 1};
    ctx->UpdateSubresource(it->second.buf, 0, &bx, p + sizeof u, 0, 0);
    break; }
  case X_TARGETS: {
    const XTargets &t = *(const XTargets *)p;
    cur_rtv = t.rt.id ? rtv_of(t.rt) : nullptr;
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
  case X_DRAW: draw(*(const XDraw *)p, p + sizeof(XDraw), pp); draws++; break;
  case X_PRESENT: {
    drain_debug();
    HRESULT h = swap ? swap->Present(sync, 0) : E_FAIL;
    if (FAILED(h)) d8_once("present", "IDXGISwapChain::Present: 0x%08lX (first failure)", (unsigned long)h);
    presents++;
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
    memo.vc_ok = memo.pc_ok = false;                       // the constants may be in either place: written again
    break; }
  }
  (void)n;
}

// ---- a draw -------------------------------------------------------------------------------------------------------------------
void Exec::draw(const XDraw &d, const uint8_t *data, const Prep *pp) {
  if (!ff || !ff->compile) return;
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
        D3D11_INPUT_ELEMENT_DESC ie[16];
        static const char *sn[] = {"POSITION", "POSITION", "NORMAL", "COLOR", "TEXCOORD", "PSIZE", "BLENDWEIGHT"};
        for (int i = 0; i < d.vk.nel; i++)
          ie[i] = {sn[d.vk.el[i].sem], d.vk.el[i].sem == SE_POST ? 0u : d.vk.el[i].idx, vdxgi(d.vk.el[i].fmt), d.vk.el[i].stream, d.vk.el[i].off, D3D11_INPUT_PER_VERTEX_DATA, 0};
        if (FAILED(dev->CreateInputLayout(ie, d.vk.nel, b->GetBufferPointer(), b->GetBufferSize(), &lay))) d8_log("CreateInputLayout failed");
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
  auto set_vb = [&](UINT i, ID3D11Buffer *b, UINT stride) {
    if (f && bd.vb[i] == b && bd.stride[i] == stride && bd.voff[i] == 0) return;
    UINT off = 0; ctx->IASetVertexBuffers(i, 1, &b, &stride, &off); bd.vb[i] = b; bd.stride[i] = stride; bd.voff[i] = 0;
  };
  auto set_ib = [&](ID3D11Buffer *b, bool i32) {
    DXGI_FORMAT fm = i32 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
    if (f && bd.ib == b && bd.ifmt == fm) return;
    ctx->IASetIndexBuffer(b, fm, 0); bd.ib = b; bd.ifmt = fm;
  };
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
  for (UINT i = 0; i < 16; i++) {
    if (!d.stream[i].buf) continue;
    auto it = res.find(d.stream[i].buf);
    if (it == res.end() || !it->second.buf) { d8_once("nostream", "a draw reads a stream whose buffer is gone (skipped)"); return; }
    set_vb(i, it->second.buf, d.stream[i].stride);
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
  auto ib = res.find(d.ib);
  if (ib == res.end() || !ib->second.buf) { d8_once("noib", "an indexed draw whose index buffer is gone (skipped)"); return; }
  set_ib(ib->second.buf, d.ib32 != 0);
  ctx->DrawIndexed(d.count, d.start, (INT)d.base);
}
