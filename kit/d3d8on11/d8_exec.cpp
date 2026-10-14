// d8_exec.cpp -- d3d8on11's executor (d8_exec.h): Direct3D 11 objects by id, and the fixed-function pipeline as
// generated shaders. What each draw needs was decided on the D3D8 side (d8_ff.cpp: the keys, the constants, the state
// descriptions); here the keys become HLSL once, compiled with d3dcompiler_47's D3DCompile, cached, and drawn.
//
// What each behaviour should be comes from the DirectX 8 SDK reference (render states, texture stage states,
// lighting, fog, texture coordinate processing); where the reference leaves a choice open, from Wine 9.0's wined3d,
// named where used. The picture test (test/test_d3d8on11.py, mode scenes) compares with Wine's d3d8.
#include "d8_exec.h"
#include <d3dcompiler.h>
#include <math.h>
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
  enum { UPVB = 4 << 20, UPIB = 1 << 20 };
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
  return std::string("struct ") + name + " { float4 pos : SV_Position; " + q + "float4 d0 : COLOR0; " + q +
         "float4 d1 : COLOR1; float4 tc[8] : TEXCOORD0; float fog : FOG; };\n";
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
    case V_F1: return "float4(" + n + ", 0, 0, 1)"; case V_F2: return "float4(" + n + ", 0, 1)";
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
    s += "  o.pos = mul(p, wvp);\n  o.pos.xy += fix.xy * o.pos.w;\n";
    s += "  float3 cp = mul(p, wv).xyz;\n";
  }
  s += "  float4 vd = " + in(SE_COLOR, 0, "float4(1,1,1,1)") + "; float4 vs = " + in(SE_COLOR, 1, "float4(0,0,0,0)") + ";\n";
  bool needN = k.lighting || [&] { for (int i = 0; i < k.ntc; i++) if (k.tc[i].src == 1 || k.tc[i].src == 3) return true; return false; }();
  if (needN && !k.rhw) {
    s += "  float3 nrm = mul(float4(" + in(SE_NORMAL, 0, "float4(0,0,0,0)") + ".xyz, 0), wvn).xyz;\n";
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
        case V_F1: src = "float4(" + n + ", 1, 0, 0)"; break; case V_F2: src = "float4(" + n + ", 1, 0)"; break;
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
  default: s += "  o.fog = 0;\n";
  }
  s += "  return o;\n}\n";
  return s;
}

// ---- HLSL: pixel shader ---------------------------------------------------------------------------------------------------
static std::string arg(int a, const char *tex) {
  std::string v;
  switch (a & D3DTA_SELECTMASK) {
  case D3DTA_DIFFUSE: v = "i.d0"; break; case D3DTA_CURRENT: v = "cur"; break; case D3DTA_TEXTURE: v = tex; break;
  case D3DTA_TFACTOR: v = "tfactor"; break; case D3DTA_SPECULAR: v = "i.d1"; break; case D3DTA_TEMP: v = "tmp"; break;
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
  s += "float4 main(PI i) : SV_Target {\n  float4 cur = i.d0, tmp = 0;\n";
  for (int i = 0; i < k.nstages; i++) {
    const auto &t = k.s[i]; char b[400], tex[16];
    snprintf(tex, sizeof tex, "x%d", i);
    if (t.tex == 1) {
      std::string uv = t.tcn ? "i.tc[" + std::to_string(i) + "].xy / i.tc[" + std::to_string(i) + "]." + "xyzw"[t.tcn - 1] : "i.tc[" + std::to_string(i) + "].xy";
      snprintf(b, sizeof b, "  float4 x%d = t%d.Sample(s%d, %s);\n", i, i, i, uv.c_str());
    } else if (t.tex == 2) snprintf(b, sizeof b, "  float4 x%d = t%d.Sample(s%d, i.tc[%d].xyz);\n", i, i, i, i);
    else snprintf(b, sizeof b, "  float4 x%d = float4(0, 0, 0, 1);\n", i);    // no texture bound PROVISIONAL
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
    // D3DCMP: NEVER 1, LESS 2, EQUAL 3, LESSEQUAL 4, GREATER 5, NOTEQUAL 6, GREATEREQUAL 7, ALWAYS 8 (pass when alpha OP ref)
    static const char *c[] = {"", "false", "a < r", "abs(a - r) < 0.5 / 255", "a <= r", "a > r", "abs(a - r) >= 0.5 / 255", "a >= r", "true"};
    s += std::string("  { float a = cur.a, r = aref.x; if (!(") + c[k.alphatest] + ")) discard; }\n";
  }
  // fog: 1 linear, 2 exp, 3 exp2 (fogp: start, end, density, 1 / (end - start))
  if (k.fog == 1) s += "  cur.rgb = lerp(fogcolor.rgb, cur.rgb, saturate((fogp.y - i.fog) * fogp.w));\n";
  else if (k.fog == 2) s += "  cur.rgb = lerp(fogcolor.rgb, cur.rgb, saturate(exp(-fogp.z * i.fog)));\n";
  else if (k.fog == 3) s += "  cur.rgb = lerp(fogcolor.rgb, cur.rgb, saturate(exp(-fogp.z * fogp.z * i.fog * i.fog)));\n";
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
}
static void free_res(ExecRes &r) {
  for (auto &v : r.rtv) if (v.second) v.second->Release();
  r.rtv.clear();
  for (auto &v : r.srv_levels) if (v.second) v.second->Release();
  r.srv_levels.clear();
  if (r.srv) r.srv->Release(); if (r.dsv) r.dsv->Release(); if (r.tex) r.tex->Release(); if (r.buf) r.buf->Release();
  r.srv = nullptr; r.dsv = nullptr; r.tex = nullptr; r.buf = nullptr;
}
void Exec::release_swap() {
  if (ctx) { ctx->OMSetRenderTargets(0, NULL, NULL); ctx->ClearState(); ctx->Flush(); }
  cur_rtv = nullptr; cur_dsv = nullptr;
  auto it = res.find(X_BACKBUFFER);
  if (it != res.end()) { free_res(it->second); res.erase(it); }
  if (swap) swap->Release(); swap = nullptr;
}
int32_t Exec::init(const XInit &x, bool resize) {
  if (fail_init) { d8_log("X_INIT answered 0x%08X (D8ON11_TEST_EXEC_FAIL_INIT, a test)", (unsigned)fail_init); return fail_init; }
  wnd = (HWND)(uintptr_t)x.hwnd;
  if (!resize) {
    D3D_FEATURE_LEVEL fl = D3D_FEATURE_LEVEL_11_0, got;
    HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, &fl, 1,
                                   D3D11_SDK_VERSION, &dev, &got, &ctx);
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
void Exec::shutdown() {
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

void Exec::run(uint16_t op, const uint8_t *p, uint32_t n, int32_t *hr, std::vector<uint8_t> *reply) {
  switch (op) {
  case X_INIT: case X_RESIZE: {
    const XInit &x = *(const XInit *)p;
    *hr = init(x, op == X_RESIZE);
    if (*hr >= 0 && x.ds_format) {                          // the automatic depth surface
      auto old = res.find(x.ds_id); if (old != res.end()) { free_res(old->second); res.erase(old); }
      XTex t = {x.ds_id, XT_DEPTH, x.width, x.height, 1, 1, x.ds_format, D3DUSAGE_DEPTHSTENCIL};
      int32_t h2 = 0; run(X_TEX, (const uint8_t *)&t, sizeof t, &h2, nullptr);
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
      for (auto &v : it->second.rtv) if (v.second == cur_rtv) cur_rtv = nullptr;
      if (it->second.dsv == cur_dsv) cur_dsv = nullptr;
      free_res(it->second); res.erase(it);
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
    break; }
  case X_CLEAR: {
    const XClear &c = *(const XClear *)p;
    if ((c.flags & D3DCLEAR_TARGET) && cur_rtv) {
      float f[4] = {((c.color >> 16) & 255) / 255.0f, ((c.color >> 8) & 255) / 255.0f, (c.color & 255) / 255.0f, (c.color >> 24) / 255.0f};
      ctx->ClearRenderTargetView(cur_rtv, f);
    }
    UINT df = ((c.flags & D3DCLEAR_ZBUFFER) ? D3D11_CLEAR_DEPTH : 0) | ((c.flags & D3DCLEAR_STENCIL) ? D3D11_CLEAR_STENCIL : 0);
    if (df && cur_dsv) ctx->ClearDepthStencilView(cur_dsv, df, c.z, (UINT8)c.stencil);
    break; }
  case X_COPY: {
    const XCopy &c = *(const XCopy *)p;
    auto s = res.find(c.src.id), d = res.find(c.dst.id);
    if (s == res.end() || d == res.end() || !s->second.tex || !d->second.tex) break;
    D3D11_BOX bx = {(UINT)c.left, (UINT)c.top, 0, (UINT)c.right, (UINT)c.bottom, 1};
    ctx->CopySubresourceRegion(d->second.tex, sub_of(d->second, c.dst), c.x, c.y, 0, s->second.tex, sub_of(s->second, c.src), &bx);
    break; }
  case X_DRAW: draw(*(const XDraw *)p, p + sizeof(XDraw)); draws++; break;
  case X_PRESENT: {
    HRESULT h = swap ? swap->Present(0, 0) : E_FAIL;
    if (FAILED(h)) d8_once("present", "IDXGISwapChain::Present: 0x%08lX (first failure)", (unsigned long)h);
    presents++;
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
  }
  (void)n;
}

// ---- a draw -------------------------------------------------------------------------------------------------------------------
void Exec::draw(const XDraw &d, const uint8_t *data) {
  if (!ff || !ff->compile) return;
  std::string vks = kstr(d.vk);
  ID3D11VertexShader *vsh = nullptr; ID3D11InputLayout *lay = nullptr;
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
  std::string pks = kstr(d.pk);
  ID3D11PixelShader *psh = nullptr;
  auto pi = ff->ps.find(pks);
  if (pi == ff->ps.end()) {
    ID3DBlob *b = compile(ff, gen_ps(d.pk), "ps_4_0");
    if (b) { dev->CreatePixelShader(b->GetBufferPointer(), b->GetBufferSize(), NULL, &psh); b->Release(); }
    ff->ps[pks] = psh;
  } else psh = pi->second;
  if (!vsh || !psh || !lay) return;
  D3D11_MAPPED_SUBRESOURCE m;
  if (SUCCEEDED(ctx->Map(ff->vcb, 0, D3D11_MAP_WRITE_DISCARD, 0, &m))) { memcpy(m.pData, &d.vc, sizeof d.vc); ctx->Unmap(ff->vcb, 0); }
  if (SUCCEEDED(ctx->Map(ff->pcb, 0, D3D11_MAP_WRITE_DISCARD, 0, &m))) { memcpy(m.pData, &d.pc, sizeof d.pc); ctx->Unmap(ff->pcb, 0); }
  ctx->OMSetBlendState(cached(ff->bs, d.blend, [&](const D3D11_BLEND_DESC *x, ID3D11BlendState **o) { dev->CreateBlendState(x, o); }), NULL, 0xFFFFFFFF);
  ctx->OMSetDepthStencilState(cached(ff->ds, d.depth, [&](const D3D11_DEPTH_STENCIL_DESC *x, ID3D11DepthStencilState **o) { dev->CreateDepthStencilState(x, o); }), d.stencil_ref);
  ctx->RSSetState(cached(ff->rs, d.raster, [&](const D3D11_RASTERIZER_DESC *x, ID3D11RasterizerState **o) { dev->CreateRasterizerState(x, o); }));
  ctx->RSSetViewports(1, &d.viewport);
  for (UINT i = 0; i < d.nstages && i < 8; i++) {
    ID3D11ShaderResourceView *srv = nullptr; ID3D11SamplerState *smp = nullptr;
    if (d.stage[i].tex) {
      auto it = res.find(d.stage[i].tex);
      if (it != res.end()) {
        ExecRes &r = it->second; uint32_t f = d.stage[i].first_level, c = d.stage[i].nlevels;
        if (f == 0 && (c == 0 || c >= r.levels)) srv = r.srv;
        else if (r.tex && f < r.levels) {                        // a view of only the levels the stage may read
          if (c == 0 || f + c > r.levels) c = r.levels - f;
          ID3D11ShaderResourceView *&v = r.srv_levels[f * 256 + c];
          if (!v) {
            D3D11_SHADER_RESOURCE_VIEW_DESC vd = {}; D3D11_TEXTURE2D_DESC td; r.tex->GetDesc(&td); vd.Format = td.Format;
            if (r.faces == 6) { vd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE; vd.TextureCube.MostDetailedMip = f; vd.TextureCube.MipLevels = c; }
            else { vd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D; vd.Texture2D.MostDetailedMip = f; vd.Texture2D.MipLevels = c; }
            if (FAILED(dev->CreateShaderResourceView(r.tex, &vd, &v))) d8_log("a view of levels %u..%u of object %u failed", f, f + c - 1, d.stage[i].tex);
          }
          srv = v;
        }
      }
      smp = cached(ff->ss, d.stage[i].sampler, [&](const D3D11_SAMPLER_DESC *x, ID3D11SamplerState **o) { dev->CreateSamplerState(x, o); });
    }
    ctx->PSSetShaderResources(i, 1, &srv); ctx->PSSetSamplers(i, 1, &smp);
  }
  ctx->VSSetShader(vsh, NULL, 0); ctx->PSSetShader(psh, NULL, 0); ctx->IASetInputLayout(lay);
  ctx->VSSetConstantBuffers(0, 1, &ff->vcb); ctx->PSSetConstantBuffers(0, 1, &ff->pcb);
  ctx->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)d.topology);
  if (d.kind == XD_UP || d.kind == XD_UP_INDEXED) {
    UINT at = 0, stride = d.up_stride, off = 0;
    if (!ring(ctx, ff->upvb, FF::UPVB, ff->upvbPos, data, d.up_vbytes, stride, &at)) { d8_once("upbig", "a ...UP draw larger than the ring: skipped"); return; }
    ctx->IASetVertexBuffers(0, 1, &ff->upvb, &stride, &off);
    UINT first = at / stride;
    if (d.kind == XD_UP) { ctx->Draw(d.count, first); return; }
    UINT isz = d.i32 ? 4 : 2, ia;
    if (!ring(ctx, ff->upib, FF::UPIB, ff->upibPos, data + d.up_vbytes, d.ibytes, isz, &ia)) return;
    ctx->IASetIndexBuffer(ff->upib, d.i32 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT, 0);
    ctx->DrawIndexed(d.count, ia / isz, (INT)first);
    return;
  }
  for (UINT i = 0; i < 16; i++) {
    if (!d.stream[i].buf) continue;
    auto it = res.find(d.stream[i].buf);
    if (it == res.end() || !it->second.buf) { d8_once("nostream", "a draw reads a stream whose buffer is gone (skipped)"); return; }
    UINT stride = d.stream[i].stride, off = 0; ctx->IASetVertexBuffers(i, 1, &it->second.buf, &stride, &off);
  }
  if (d.kind == XD_DRAW) { ctx->Draw(d.count, d.start); return; }
  if (d.kind == XD_INLINE_INDEXED) {
    UINT isz = d.i32 ? 4 : 2, ia;
    if (!ring(ctx, ff->upib, FF::UPIB, ff->upibPos, data, d.ibytes, isz, &ia)) return;
    ctx->IASetIndexBuffer(ff->upib, d.i32 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT, 0);
    ctx->DrawIndexed(d.count, ia / isz, (INT)d.base);
    return;
  }
  auto ib = res.find(d.ib);
  if (ib == res.end() || !ib->second.buf) { d8_once("noib", "an indexed draw whose index buffer is gone (skipped)"); return; }
  ctx->IASetIndexBuffer(ib->second.buf, d.ib32 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT, 0);
  ctx->DrawIndexed(d.count, d.start, (INT)d.base);
}
