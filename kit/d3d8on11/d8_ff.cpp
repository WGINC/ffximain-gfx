// d8_ff.cpp -- d3d8on11 milestone 2: draws executed through generated shaders (Direct3D 8's fixed-function pipeline).
//
// At a draw, the state that decides the vertex work (vertex layout, lighting, material sources, texture coordinate
// sources and transforms, vertex fog) becomes a VSKey and the state that decides the pixel work (the texture stages,
// alpha test, fog, specular) a PSKey. Each key is turned into HLSL once (gen_vs / gen_ps), compiled with
// d3dcompiler_47's D3DCompile, and cached. Blend, depth-stencil, rasterizer and sampler states are D3D11 state
// objects cached by their descriptions. Constants (matrices, lights, material, fog, factors) go into one constant
// buffer per stage, written at each draw.
//
// What each behaviour should be comes from the DirectX 8 SDK reference (render states, texture stage states,
// lighting, fog, texture coordinate processing). Where the reference leaves a choice open, the choice is marked
// PROVISIONAL here and settled by the picture test (test/test_d3d8on11.py, mode scenes): the same scenes through
// Wine's d3d8, itself tested against Windows, and through this device.
//
// Phase 1 executes a draw on the game's thread when it is made (after what is recorded before it), so a ...UP draw's
// memory is still valid. Phase 2 (a second thread) will copy the state and the UP data into the command.
#include "d8.h"
#include <d3dcompiler.h>
#include <math.h>
#include <functional>
#include <string>
#include <unordered_map>

typedef HRESULT (WINAPI *PFN_Compile)(const void *, SIZE_T, const char *, const D3D_SHADER_MACRO *, ID3DInclude *,
                                       const char *, const char *, UINT, UINT, ID3DBlob **, ID3DBlob **);

// ---- vertex layouts ------------------------------------------------------------------------------------------------------
enum Sem : uint8_t { SE_POS, SE_POST, SE_NORMAL, SE_COLOR, SE_TEX, SE_PSIZE, SE_WEIGHT };
enum VFmt : uint8_t { V_F1, V_F2, V_F3, V_F4, V_COLOR, V_UBYTE4, V_SHORT2, V_SHORT4 };
struct VElem { uint8_t sem, idx, stream, fmt; uint16_t off; };
static DXGI_FORMAT vdxgi(uint8_t f) {
  switch (f) {
  case V_F1: return DXGI_FORMAT_R32_FLOAT; case V_F2: return DXGI_FORMAT_R32G32_FLOAT;
  case V_F3: return DXGI_FORMAT_R32G32B32_FLOAT; case V_F4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
  case V_COLOR: return DXGI_FORMAT_B8G8R8A8_UNORM; case V_UBYTE4: return DXGI_FORMAT_R8G8B8A8_UINT;
  case V_SHORT2: return DXGI_FORMAT_R16G16_SINT; default: return DXGI_FORMAT_R16G16B16A16_SINT;
  }
}
static int vsize(uint8_t f) { static const int n[] = {4, 8, 12, 16, 4, 4, 4, 8}; return n[f]; }

// ---- the keys -------------------------------------------------------------------------------------------------------------
struct VSKey {
  uint8_t nel; VElem el[16];
  uint8_t rhw, lighting, nlights, ltype[8];
  uint8_t matsrc[4];                      // diffuse, specular, ambient, emissive: 0 material, 1 vertex diffuse, 2 vertex specular
  uint8_t specular, localviewer, normalize;
  uint8_t fogsrc, flat, pad1;             // the fog coordinate (FOG_* below); flat shading
  uint8_t ntc; struct { uint8_t src, idx, count, proj; } tc[8];   // per stage: 0 passthru, 1 cs normal, 2 cs position, 3 cs reflection
};
// The fog model (Wine 9.0 wined3d: utils.c wined3d_ffp_get_vs_settings / wined3d_ffp_get_fs_settings /
// get_fog_start_end, glsl_shader.c shader_glsl_generate_fog_code): the vertex shader gives one coordinate, the pixel
// shader turns it into the factor with the table mode if set, else the vertex mode, linearly from FOGSTART/FOGEND or by
// FOGDENSITY. With no table mode and no vertex mode (or pre-transformed vertices) the coordinate is the specular alpha,
// read linearly with start 255 and end 0, so the factor is that alpha.
enum { FOG_OFF, FOG_SPECA, FOG_RANGE, FOG_EYEZ, FOG_RHWZ, FOG_CLIPZ };
struct PSKey {
  struct { uint8_t cop, ca1, ca2, ca0, aop, aa1, aa2, aa0, res, tex, tcn; } s[8];
  uint8_t nstages, alphatest, fog, specadd;   // fog: 0 none, 1 linear, 2 exp, 3 exp2
  uint8_t flat, pad[3];
};
template <class K> static std::string kstr(const K &k) { return std::string((const char *)&k, sizeof k); }

// ---- the constants (one buffer per stage; float4 rows) -----------------------------------------------------------------
struct VSConst {
  float wvp[16], wv[16], wvn[16], proj[16];   // world*view*proj, world*view, its inverse transpose (normals), projection
  float tex[8][16];
  float vp[4];                                // viewport x, y, w, h (pre-transformed vertices)
  float fix[4];                               // the D3D8 to D3D11 pixel offset in clip space (x, y)
  float ambient[4], mdiff[4], mamb[4], mspec[4], memis[4], mpow[4];
  float ldiff[8][4], lspec[8][4], lamb[8][4], lpos[8][4], ldir[8][4], latt[8][4], lspot[8][4];
  float fog[4];                               // start, end, density, 1/(end-start)
};
struct PSConst { float tfactor[4], fogcolor[4], fog[4], aref[4]; };

struct FF {
  PFN_Compile compile = nullptr;
  std::unordered_map<std::string, ID3D11VertexShader *> vs; std::unordered_map<std::string, ID3D11InputLayout *> il;
  std::unordered_map<std::string, ID3D11PixelShader *> ps;
  std::unordered_map<std::string, ID3D11BlendState *> bs; std::unordered_map<std::string, ID3D11DepthStencilState *> ds;
  std::unordered_map<std::string, ID3D11RasterizerState *> rs; std::unordered_map<std::string, ID3D11SamplerState *> ss;
  ID3D11Buffer *vcb = nullptr, *pcb = nullptr, *upvb = nullptr, *upib = nullptr;
  UINT upvbPos = 0, upibPos = 0;
  enum { UPVB = 4 << 20, UPIB = 1 << 20 };
  bool failed = false;
};

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
    s += k.normalize || k.lighting ? "  if (dot(nrm, nrm) > 0) nrm = normalize(nrm);\n" : "";
  }
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
    switch (t.src) {
    case 1: src = k.rhw ? "float4(0,0,0,1)" : "float4(nrm, 1)"; break;
    case 2: src = "float4(cp, 1)"; break;
    case 3: src = k.rhw ? "float4(0,0,0,1)" : "float4(reflect(normalize(cp), nrm), 1)"; break;
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

void Dev::ff_init() {
  ff = new FF();
  HMODULE m = LoadLibraryA("d3dcompiler_47.dll");
  ff->compile = m ? (PFN_Compile)(void *)GetProcAddress(m, "D3DCompile") : nullptr;
  if (!ff->compile) d8_log("d3dcompiler_47.dll not found: draws cannot be executed");
  D3D11_BUFFER_DESC bd = {}; bd.Usage = D3D11_USAGE_DYNAMIC; bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; bd.ByteWidth = (sizeof(VSConst) + 15) & ~15u; dev->CreateBuffer(&bd, NULL, &ff->vcb);
  bd.ByteWidth = (sizeof(PSConst) + 15) & ~15u; dev->CreateBuffer(&bd, NULL, &ff->pcb);
  bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; bd.ByteWidth = FF::UPVB; dev->CreateBuffer(&bd, NULL, &ff->upvb);
  bd.BindFlags = D3D11_BIND_INDEX_BUFFER; bd.ByteWidth = FF::UPIB; dev->CreateBuffer(&bd, NULL, &ff->upib);
}
void Dev::ff_free() {
  if (!ff) return;
  for (auto &x : ff->vs) x.second->Release(); for (auto &x : ff->il) if (x.second) x.second->Release();
  for (auto &x : ff->ps) x.second->Release(); for (auto &x : ff->bs) x.second->Release();
  for (auto &x : ff->ds) x.second->Release(); for (auto &x : ff->rs) x.second->Release(); for (auto &x : ff->ss) x.second->Release();
  if (ff->vcb) ff->vcb->Release(); if (ff->pcb) ff->pcb->Release(); if (ff->upvb) ff->upvb->Release(); if (ff->upib) ff->upib->Release();
  delete ff; ff = nullptr;
}

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
      // size, 5 diffuse, 6 specular, 7..14 texture coordinates
      uint8_t sem, idx = 0;
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
template <class D, class O, class Make> static O *cached(std::unordered_map<std::string, O *> &m, const D &d, Make make) {
  std::string k((const char *)&d, sizeof d);
  auto it = m.find(k);
  if (it != m.end()) return it->second;
  O *o = nullptr; make(&d, &o); m[k] = o;
  return o;
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

HRESULT Dev::ff_draw(int kind, D3DPRIMITIVETYPE type, UINT prims, UINT startVertex, UINT minIndex, UINT numVerts, UINT startIndex,
                     const void *upv, UINT upStride, const void *upi, D3DFORMAT upiFmt) {
  if (!prims) return D3D_OK;
  if (!ff || !ff->compile) return D3DERR_INVALIDCALL;
  flush();                                                   // what was recorded before this draw goes first
  VSKey vk; memset(&vk, 0, sizeof vk); PSKey pk; memset(&pk, 0, sizeof pk);
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
  if (st.ps) { d8_once("psprog", "a pixel shader (the game's own program): not translated yet (draw skipped)"); return D3D_OK; }
  const DWORD *r = st.rs;
  // ---- vertex key
  bool hasN = false, hasD = false, hasS = false;
  for (int i = 0; i < vk.nel; i++) { hasN |= vk.el[i].sem == SE_NORMAL; hasD |= vk.el[i].sem == SE_COLOR && vk.el[i].idx == 0; hasS |= vk.el[i].sem == SE_COLOR && vk.el[i].idx == 1; }
  vk.lighting = !vk.rhw && r[D3DRS_LIGHTING];
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
  else if (tmode != D3DFOG_NONE)                             // table fog: depth (an orthographic projection: clip z)
    vk.fogsrc = vk.rhw ? FOG_RHWZ : P._14 == 0 && P._24 == 0 && P._34 == 0 && P._44 == 1 ? FOG_CLIPZ : FOG_EYEZ;
  else vk.fogsrc = r[D3DRS_RANGEFOGENABLE] ? FOG_RANGE : FOG_EYEZ;
  if (fog) { DWORD m = coordfog ? D3DFOG_LINEAR : tmode != D3DFOG_NONE ? tmode : vmode;
    pk.fog = m == D3DFOG_EXP ? 2 : m == D3DFOG_EXP2 ? 3 : m == D3DFOG_LINEAR ? 1 : 0; }
  (void)hasN;
  // ---- the stages: textures, coordinates, combiners
  int ns = 0;
  for (; ns < 8 && st.tss[ns][D3DTSS_COLOROP] != D3DTOP_DISABLE; ns++) {
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
    // a stage whose op reads a texture that is not bound passes the current colour (alpha) on: SELECTARG1(CURRENT).
    // The DirectX 8 reference documents only ALPHAARG1 (D3DTA_TEXTURE with no texture reads as D3DTA_DIFFUSE, the same
    // as CURRENT in stage 0); the rule for the rest is Wine's (9.0 utils.c is_invalid_op). Windows' is not established.
    if (!s.tex) {
      auto invalid = [](uint8_t op, uint8_t a1, uint8_t a2, uint8_t a0) {
        if (op == D3DTOP_DISABLE) return false;
        if ((a1 & D3DTA_SELECTMASK) == D3DTA_TEXTURE && op != D3DTOP_SELECTARG2) return true;
        if ((a2 & D3DTA_SELECTMASK) == D3DTA_TEXTURE && op != D3DTOP_SELECTARG1) return true;
        return (a0 & D3DTA_SELECTMASK) == D3DTA_TEXTURE && (op == D3DTOP_MULTIPLYADD || op == D3DTOP_LERP);
      };
      if (invalid(s.cop, s.ca1, s.ca2, s.ca0)) { s.cop = D3DTOP_SELECTARG1; s.ca1 = D3DTA_CURRENT; }
      if (s.cop != D3DTOP_DOTPRODUCT3 && invalid(s.aop, s.aa1, s.aa2, s.aa0)) { s.aop = D3DTOP_SELECTARG1; s.aa1 = D3DTA_CURRENT; }
    }
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
  // ---- shaders
  std::string vks = kstr(vk);
  ID3D11VertexShader *vsh = nullptr; ID3D11InputLayout *lay = nullptr;
  auto vi = ff->vs.find(vks);
  if (vi == ff->vs.end()) {
    std::string src = gen_vs(vk);
    ID3DBlob *b = compile(ff, src, "vs_4_0");
    if (b) {
      dev->CreateVertexShader(b->GetBufferPointer(), b->GetBufferSize(), NULL, &vsh);
      D3D11_INPUT_ELEMENT_DESC ie[16];
      static const char *sn[] = {"POSITION", "POSITION", "NORMAL", "COLOR", "TEXCOORD", "PSIZE", "BLENDWEIGHT"};
      for (int i = 0; i < vk.nel; i++) {
        ie[i] = {sn[vk.el[i].sem], vk.el[i].sem == SE_POST ? 0u : vk.el[i].idx, vdxgi(vk.el[i].fmt), vk.el[i].stream, vk.el[i].off, D3D11_INPUT_PER_VERTEX_DATA, 0};
      }
      if (FAILED(dev->CreateInputLayout(ie, vk.nel, b->GetBufferPointer(), b->GetBufferSize(), &lay))) d8_log("CreateInputLayout failed");
      b->Release();
    }
    ff->vs[vks] = vsh; ff->il[vks] = lay;
  } else { vsh = vi->second; lay = ff->il[vks]; }
  std::string pks = kstr(pk);
  ID3D11PixelShader *psh = nullptr;
  auto pi = ff->ps.find(pks);
  if (pi == ff->ps.end()) {
    ID3DBlob *b = compile(ff, gen_ps(pk), "ps_4_0");
    if (b) { dev->CreatePixelShader(b->GetBufferPointer(), b->GetBufferSize(), NULL, &psh); b->Release(); }
    ff->ps[pks] = psh;
  } else psh = pi->second;
  if (!vsh || !psh || !lay) return D3DERR_INVALIDCALL;
  // ---- constants
  {
    VSConst c; memset(&c, 0, sizeof c);
    D3DMATRIX wv; float t[16];
    mmul(st.xf[D3DTS_WORLD], st.xf[D3DTS_VIEW], t); memcpy(&wv, t, 64);
    mmul(wv, st.xf[D3DTS_PROJECTION], c.wvp); memcpy(c.wv, t, 64); inv_transpose3(t, c.wvn); memcpy(c.proj, &st.xf[D3DTS_PROJECTION], 64);
    for (int i = 0; i < 8; i++) memcpy(c.tex[i], &st.xf[D3DTS_TEXTURE0 + i], 64);
    c.vp[0] = (float)st.vp.X; c.vp[1] = (float)st.vp.Y; c.vp[2] = (float)st.vp.Width; c.vp[3] = (float)st.vp.Height;
    // D3D8 puts pixel centres on integer screen coordinates, D3D11 on .5: geometry moves half a pixel right and down
    // (Wine 9.0 utils.c get_projection_matrix does the same for GL; the picture test compares the result)
    c.fix[0] = 1.0f / (float)st.vp.Width; c.fix[1] = -1.0f / (float)st.vp.Height;
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
    D3D11_MAPPED_SUBRESOURCE m;
    if (SUCCEEDED(ctx->Map(ff->vcb, 0, D3D11_MAP_WRITE_DISCARD, 0, &m))) { memcpy(m.pData, &c, sizeof c); ctx->Unmap(ff->vcb, 0); }
    PSConst p; memset(&p, 0, sizeof p);
    argb4(r[D3DRS_TEXTUREFACTOR], p.tfactor); argb4(r[D3DRS_FOGCOLOR], p.fogcolor);
    // start, end, density, scale = 1 / (end - start), as Wine's get_fog_start_end and shader_glsl_load_fog_uniform:
    // the specular-alpha coordinate reads with start 255, end 0; vertex fog with start == end fogs everything
    { float s0 = fs, e0 = fe;
      if (coordfog) { s0 = 255; e0 = 0; }
      else if (tmode == D3DFOG_NONE && fs == fe) { s0 = -INFINITY; e0 = 0; }
      p.fog[0] = s0; p.fog[1] = e0; p.fog[2] = fdn; p.fog[3] = 1.0f / (e0 - s0); }
    p.aref[0] = (r[D3DRS_ALPHAREF] & 255) / 255.0f;
    if (SUCCEEDED(ctx->Map(ff->pcb, 0, D3D11_MAP_WRITE_DISCARD, 0, &m))) { memcpy(m.pData, &p, sizeof p); ctx->Unmap(ff->pcb, 0); }
  }
  // ---- fixed state objects
  {
    D3D11_BLEND_DESC bd = {}; auto &t = bd.RenderTarget[0];
    t.BlendEnable = r[D3DRS_ALPHABLENDENABLE] ? TRUE : FALSE;
    DWORD sb = r[D3DRS_SRCBLEND], db = r[D3DRS_DESTBLEND];
    if (sb == D3DBLEND_BOTHSRCALPHA) { sb = D3DBLEND_SRCALPHA; db = D3DBLEND_INVSRCALPHA; }
    if (sb == D3DBLEND_BOTHINVSRCALPHA) { sb = D3DBLEND_INVSRCALPHA; db = D3DBLEND_SRCALPHA; }
    t.SrcBlend = blend(sb, false); t.DestBlend = blend(db, false); t.SrcBlendAlpha = blend(sb, true); t.DestBlendAlpha = blend(db, true);
    DWORD bo = r[D3DRS_BLENDOP]; t.BlendOp = t.BlendOpAlpha = bo >= 1 && bo <= 5 ? (D3D11_BLEND_OP)bo : D3D11_BLEND_OP_ADD;
    t.RenderTargetWriteMask = (UINT8)(r[D3DRS_COLORWRITEENABLE] & 0xF);
    ID3D11BlendState *b = cached(ff->bs, bd, [&](const D3D11_BLEND_DESC *d, ID3D11BlendState **o) { dev->CreateBlendState(d, o); });
    ctx->OMSetBlendState(b, NULL, 0xFFFFFFFF);
    D3D11_DEPTH_STENCIL_DESC dd = {};
    dd.DepthEnable = r[D3DRS_ZENABLE] && curDS ? TRUE : FALSE;
    dd.DepthWriteMask = r[D3DRS_ZWRITEENABLE] ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
    dd.DepthFunc = cmpf(r[D3DRS_ZFUNC]);
    dd.StencilEnable = r[D3DRS_STENCILENABLE] && curDS ? TRUE : FALSE;
    dd.StencilReadMask = (UINT8)r[D3DRS_STENCILMASK]; dd.StencilWriteMask = (UINT8)r[D3DRS_STENCILWRITEMASK];
    dd.FrontFace = {sop(r[D3DRS_STENCILFAIL]), sop(r[D3DRS_STENCILZFAIL]), sop(r[D3DRS_STENCILPASS]), cmpf(r[D3DRS_STENCILFUNC])};
    dd.BackFace = dd.FrontFace;
    ID3D11DepthStencilState *z = cached(ff->ds, dd, [&](const D3D11_DEPTH_STENCIL_DESC *d, ID3D11DepthStencilState **o) { dev->CreateDepthStencilState(d, o); });
    ctx->OMSetDepthStencilState(z, r[D3DRS_STENCILREF] & 255);
    D3D11_RASTERIZER_DESC rd = {};
    rd.FillMode = r[D3DRS_FILLMODE] == D3DFILL_WIREFRAME ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
    // D3D8 culls by winding: CCW (the default) culls counter-clockwise triangles; D3D11's default front is clockwise
    rd.CullMode = r[D3DRS_CULLMODE] == D3DCULL_NONE ? D3D11_CULL_NONE : r[D3DRS_CULLMODE] == D3DCULL_CW ? D3D11_CULL_FRONT : D3D11_CULL_BACK;
    rd.DepthClipEnable = TRUE;
    // D3D8 ZBIAS (0..16): a higher value draws in front (the DirectX 8 reference); by how much is the driver's. This
    // takes Wine's d3d8 (9.0: directx.c WINED3D_LEGACY_DEPTH_BIAS, state.c depthbias): -ZBIAS in units of the depth
    // buffer's resolution and -ZBIAS times the depth slope. What the owner's driver under dgVoodoo does is not known.
    rd.DepthBias = -(INT)r[D3DRS_ZBIAS]; rd.SlopeScaledDepthBias = -(float)r[D3DRS_ZBIAS];
    ID3D11RasterizerState *rs = cached(ff->rs, rd, [&](const D3D11_RASTERIZER_DESC *d, ID3D11RasterizerState **o) { dev->CreateRasterizerState(d, o); });
    ctx->RSSetState(rs);
    D3D11_VIEWPORT v = {(float)st.vp.X, (float)st.vp.Y, (float)st.vp.Width, (float)st.vp.Height, st.vp.MinZ, st.vp.MaxZ};
    ctx->RSSetViewports(1, &v);
    for (int i = 0; i < ns; i++) {
      ID3D11ShaderResourceView *srv = nullptr; ID3D11SamplerState *smp = nullptr;
      if (pk.s[i].tex) {
        IDirect3DBaseTexture8 *tx = st.tex[i];
        srv = tx->GetType() == D3DRTYPE_CUBETEXTURE ? ((CubeTex *)(IDirect3DCubeTexture8 *)tx)->c.srv : ((Tex *)(IDirect3DTexture8 *)tx)->c.srv;
        const DWORD *t = st.tss[i];
        D3D11_SAMPLER_DESC sd = {};
        bool aniso = t[D3DTSS_MINFILTER] == D3DTEXF_ANISOTROPIC || t[D3DTSS_MAGFILTER] == D3DTEXF_ANISOTROPIC;
        int mn = t[D3DTSS_MINFILTER] >= D3DTEXF_LINEAR ? 1 : 0, mg = t[D3DTSS_MAGFILTER] >= D3DTEXF_LINEAR ? 1 : 0, mp = t[D3DTSS_MIPFILTER] >= D3DTEXF_LINEAR ? 1 : 0;
        sd.Filter = aniso ? D3D11_FILTER_ANISOTROPIC : (D3D11_FILTER)((mn ? 0x10 : 0) | (mg ? 0x4 : 0) | (mp ? 0x1 : 0));
        sd.AddressU = addr(t[D3DTSS_ADDRESSU]); sd.AddressV = addr(t[D3DTSS_ADDRESSV]); sd.AddressW = addr(t[D3DTSS_ADDRESSW]);
        float bc[4]; argb4(t[D3DTSS_BORDERCOLOR], bc); memcpy(sd.BorderColor, bc, 16);
        sd.MaxAnisotropy = t[D3DTSS_MAXANISOTROPY] ? (t[D3DTSS_MAXANISOTROPY] > 16 ? 16 : t[D3DTSS_MAXANISOTROPY]) : 1;
        memcpy(&sd.MipLODBias, &t[D3DTSS_MIPMAPLODBIAS], 4);
        sd.MinLOD = (float)t[D3DTSS_MAXMIPLEVEL]; sd.MaxLOD = t[D3DTSS_MIPFILTER] == D3DTEXF_NONE ? sd.MinLOD : D3D11_FLOAT32_MAX;
        sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
        smp = cached(ff->ss, sd, [&](const D3D11_SAMPLER_DESC *d, ID3D11SamplerState **o) { dev->CreateSamplerState(d, o); });
      }
      ctx->PSSetShaderResources(i, 1, &srv); ctx->PSSetSamplers(i, 1, &smp);
    }
  }
  ctx->VSSetShader(vsh, NULL, 0); ctx->PSSetShader(psh, NULL, 0); ctx->IASetInputLayout(lay);
  ctx->VSSetConstantBuffers(0, 1, &ff->vcb); ctx->PSSetConstantBuffers(0, 1, &ff->pcb);
  // ---- vertices and indices
  UINT nv = verts_for(type, prims);
  bool fan = type == D3DPT_TRIANGLEFAN;
  ctx->IASetPrimitiveTopology(topo(type));
  std::vector<uint32_t> fanIdx;
  auto fan_list = [&](UINT first, UINT n, const std::function<uint32_t(UINT)> &ix) {
    fanIdx.resize(3 * n);
    // each triangle starts at its first non-spoke vertex, so flat shading takes that vertex's colour as Direct3D 9 does
    // for fans (Khronos EXT_provoking_vertex, issue 7); a rotation keeps the winding
    for (UINT k = 0; k < n; k++) { fanIdx[3 * k] = ix(first + k + 1); fanIdx[3 * k + 1] = ix(first + k + 2); fanIdx[3 * k + 2] = ix(first); }
  };
  if (kind == 2 || kind == 3) {                              // from the caller's memory
    UINT vbytes = (kind == 2 ? nv : numVerts + minIndex) * upStride, at = 0;
    const uint8_t *src = (const uint8_t *)upv;
    if (!ring(ctx, ff->upvb, FF::UPVB, ff->upvbPos, src, vbytes, upStride, &at)) { d8_once("upbig", "a ...UP draw larger than the ring: skipped"); return D3D_OK; }
    UINT off = 0; ctx->IASetVertexBuffers(0, 1, &ff->upvb, &upStride, &off);
    UINT first = at / upStride;
    if (kind == 2) {
      if (fan) { fan_list(0, prims, [](UINT i) { return (uint32_t)i; }); UINT ia;
        if (!ring(ctx, ff->upib, FF::UPIB, ff->upibPos, fanIdx.data(), (UINT)fanIdx.size() * 4, 4, &ia)) return D3D_OK;
        ctx->IASetIndexBuffer(ff->upib, DXGI_FORMAT_R32_UINT, 0); ctx->DrawIndexed((UINT)fanIdx.size(), ia / 4, (INT)first); }
      else ctx->Draw(nv, first);
    } else {
      UINT isz = upiFmt == D3DFMT_INDEX32 ? 4 : 2, ia;
      if (fan) {
        fan_list(0, prims, [&](UINT i) { return isz == 4 ? ((const uint32_t *)upi)[i] : (uint32_t)((const uint16_t *)upi)[i]; });
        if (!ring(ctx, ff->upib, FF::UPIB, ff->upibPos, fanIdx.data(), (UINT)fanIdx.size() * 4, 4, &ia)) return D3D_OK;
        ctx->IASetIndexBuffer(ff->upib, DXGI_FORMAT_R32_UINT, 0); ctx->DrawIndexed((UINT)fanIdx.size(), ia / 4, (INT)first);
      } else {
        if (!ring(ctx, ff->upib, FF::UPIB, ff->upibPos, upi, nv * isz, isz, &ia)) return D3D_OK;
        ctx->IASetIndexBuffer(ff->upib, isz == 4 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT, 0);
        ctx->DrawIndexed(nv, ia / isz, (INT)first);
      }
    }
    // D3D8: after a ...UP draw, stream 0 and the indices are unset
    if (st.stream[0]) { st.stream[0]->Release(); st.stream[0] = nullptr; } st.stride[0] = 0;
    if (kind == 3 && st.ib) { st.ib->Release(); st.ib = nullptr; }
    return D3D_OK;
  }
  for (int i = 0; i < 16; i++) {
    bool used = false; for (int e = 0; e < vk.nel; e++) used |= vk.el[e].stream == i;
    if (!used) continue;
    VB *b = st.stream[i];
    if (!b || !b->c.gpu) { d8_once("nostream", "a draw reads a stream with no vertex buffer (skipped)"); return D3D_OK; }
    UINT stride = st.stride[i], off = 0; ctx->IASetVertexBuffers(i, 1, &b->c.gpu, &stride, &off);
  }
  if (kind == 0) {
    if (fan) { fan_list(startVertex, prims, [](UINT i) { return (uint32_t)i; }); UINT ia;
      if (!ring(ctx, ff->upib, FF::UPIB, ff->upibPos, fanIdx.data(), (UINT)fanIdx.size() * 4, 4, &ia)) return D3D_OK;
      ctx->IASetIndexBuffer(ff->upib, DXGI_FORMAT_R32_UINT, 0); ctx->DrawIndexed((UINT)fanIdx.size(), ia / 4, 0); }
    else ctx->Draw(nv, startVertex);
  } else {
    IB *ib = st.ib;
    if (!ib || !ib->c.gpu) { d8_once("noib", "an indexed draw with no index buffer (skipped)"); return D3D_OK; }
    UINT isz = ib->c.fmt == D3DFMT_INDEX32 ? 4 : 2;
    if (fan) {
      const uint8_t *ic = ib->c.cpu.data();
      fan_list(startIndex, prims, [&](UINT i) { return isz == 4 ? ((const uint32_t *)ic)[i] : (uint32_t)((const uint16_t *)ic)[i]; });
      UINT ia; if (!ring(ctx, ff->upib, FF::UPIB, ff->upibPos, fanIdx.data(), (UINT)fanIdx.size() * 4, 4, &ia)) return D3D_OK;
      ctx->IASetIndexBuffer(ff->upib, DXGI_FORMAT_R32_UINT, 0); ctx->DrawIndexed((UINT)fanIdx.size(), ia / 4, (INT)st.baseVertex);
    } else {
      ctx->IASetIndexBuffer(ib->c.gpu, isz == 4 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT, 0);
      ctx->DrawIndexed(nv, startIndex, (INT)st.baseVertex);
    }
  }
  (void)minIndex; (void)fvfStride;
  return D3D_OK;
}
