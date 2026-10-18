// d8_proto.h -- d3d8on11's command stream: what the D3D8 side (in the game) sends the executor (d8_exec.cpp), which
// alone uses Direct3D 11. The executor runs in the game's process (on the game's thread, or on a thread of its own:
// docs/D3D11.md phase 1 and 2, design A) or in a process of its own, built natively for ARM64 (design B, "ARM mode").
// So everything here is fixed-width with no pointers, and lays out the same in an x86 and an ARM64 build (natural
// alignment, Windows ABI on both); the static_asserts at the end pin the sizes.
//
// A record: XHdr, then its payload, padded to 8 bytes. Objects are named by ids the D3D8 side gives them; the back
// buffer is X_BACKBUFFER. A record marked "sync" is answered: the D3D8 side waits for the executor to reach it and
// reads the reply (XReply). Everything else is executed in order, later.
#pragma once
#include <stddef.h>
#include <stdint.h>
#include <d3d11.h>

enum : uint16_t {
  X_PAD = 0,         // the rest of the ring until it wraps (the transport's)
  X_INIT,            // sync: XInit -> hr
  X_RESIZE,          // sync: XInit (the window and sizes again; Reset) -> hr
  X_TEX,             // XTex: a texture (or cube texture, render-target texture), a render target, a depth surface
  X_BUF,             // XBuf
  X_DESTROY,         // uint32 id
  X_UPLOAD_TEX,      // XUpTex, then the rows
  X_UPLOAD_BUF,      // XUpBuf, then the bytes
  X_TARGETS,         // XTargets
  X_CLEAR,           // XClear, then its rectangles
  X_COPY,            // XCopy
  X_DRAW,            // XDraw, then UP vertex bytes, then index bytes
  X_PRESENT,         // (no payload)
  X_READBACK,        // sync: XRef -> the subresource's rows (pitch packed)
  X_FINISH,          // sync: nothing; answered when everything before it has run
  X_QUIT,            // the executor process ends
  X_OPTION,          // XOption: an executor switch (the A/B of d3d8on11.ini's exec_ab, d8_device.cpp)
  X_SNAP             // XSnap: a small lockable render target copied now to a staging texture of its own; its rows go
                     // to the snapshot table (SnapSlot) when a Present finds the GPU done with that copy
};
#define X_BACKBUFFER 1u

struct XHdr { uint16_t op, pad; uint32_t bytes; };              // bytes: the payload's, before padding
struct XReply { int32_t hr; uint32_t bytes; };                  // then the data
struct XRef { uint32_t id, level, face; };                      // a surface: a texture's level and face, or a whole surface

struct XInit {
  uint64_t hwnd;                                                // the game's window (a handle is valid across processes)
  uint32_t width, height, ds_format;                            // the back buffer; the automatic depth surface (0: none)
  uint32_t ds_id;                                               // its id
};
enum : uint32_t { XT_TEXTURE = 0, XT_RENDERTARGET = 1, XT_DEPTH = 2 };
struct XTex { uint32_t id, kind, width, height, levels, faces, d3dfmt, usage; };
struct XBuf { uint32_t id, length, index; };
struct XUpTex { uint32_t id, sub, x, y, w, h, rowbytes, rows; };  // a rectangle of subresource sub (D3D11 numbering)
struct XUpBuf { uint32_t id, offset, length; };
struct XTargets { XRef rt; uint32_t ds; };                          // ds 0: none
struct XClear { uint32_t flags, color; float z; uint32_t stencil; uint32_t width, height, nrects; };  // then nrects XRect
struct XRect { int32_t x1, y1, x2, y2; };   // nrects 0: the whole target (and depth surface); else these, in target pixels
enum : uint32_t { OPT_FILTER = 1 };
struct XOption { uint32_t key, value, settle; };   // settle: frames after it that the executor's phase figures leave out
struct XCopy { XRef src, dst; int32_t left, top, right, bottom, x, y; };
// The snapshot table: shared memory after the reply area (thread and remote; direct mode has its own). A lockable
// render target that only CopyRects writes gets a slot; each CopyRects into it is followed by X_SNAP with the
// surface's write count. The executor fills the slot when Map with D3D11_MAP_FLAG_DO_NOT_WAIT succeeds on the staging
// copy ("DXGI_ERROR_WAS_STILL_DRAWING" while the GPU is busy with it: D3D11_MAP_FLAG, Direct3D 11 reference), so it
// never waits. A LockRect whose surface's write count equals the slot's reads the slot instead of X_READBACK: the same
// bytes, since nothing has written the surface since that copy (d8_res.cpp, Surf::LockRect). A sequence lock: ver is
// odd while the executor writes.
struct XSnap { XRef dst; uint32_t slot, wseq; };
enum : uint32_t { SNAP_SLOTS = 1024, SNAP_BYTES = 1024 };    // 16x16 at 32 bits: the occlusion target (HISTORY, 11 Oct)
struct SnapSlot { volatile uint32_t ver, id, wseq, bytes; uint8_t data[SNAP_BYTES]; };

// ---- draws: the fixed-function pipeline as generated shaders (d8_ff.cpp decides, d8_exec.cpp draws) ------------------
enum Sem : uint8_t { SE_POS, SE_POST, SE_NORMAL, SE_COLOR, SE_TEX, SE_PSIZE, SE_WEIGHT };
enum VFmt : uint8_t { V_F1, V_F2, V_F3, V_F4, V_COLOR, V_UBYTE4, V_SHORT2, V_SHORT4 };
struct VElem { uint8_t sem, idx, stream, fmt; uint16_t off; };
struct VSKey {
  uint8_t nel; VElem el[16];
  uint8_t rhw, lighting, nlights, ltype[8];
  uint8_t matsrc[4];                      // diffuse, specular, ambient, emissive: 0 material, 1 vertex diffuse, 2 vertex specular
  uint8_t specular, localviewer, normalize;
  uint8_t fogsrc, flat, tween;            // the fog coordinate (FOG_* below); flat shading; D3DVBF_TWEENING
  uint8_t vfog;                           // vertex fog: 0 none, else the factor made per vertex (1 linear, 2 exp, 3 exp2)
  uint8_t ntc; struct { uint8_t src, idx, count, proj; } tc[8];   // per stage: 0 passthru, 1 cs normal, 2 cs position, 3 cs reflection
};
// The fog model (Wine 9.0 wined3d: utils.c wined3d_ffp_get_vs_settings / wined3d_ffp_get_fs_settings /
// get_fog_start_end, glsl_shader.c shader_glsl_generate_fog_code): the vertex shader gives one coordinate, the pixel
// shader turns it into the factor with the table mode if set, else the vertex mode, linearly from FOGSTART/FOGEND or by
// FOGDENSITY. With no table mode and no vertex mode (or pre-transformed vertices) the coordinate is the specular alpha,
// read linearly with start 255 and end 0, so the factor is that alpha.
// Vertex fog differs from Wine: the factor itself is made per vertex, clamped to [0, 1] and interpolated (VSKey.vfog),
// as the DirectX 8.1 reference says ("Vertex Fog": "it applies fog calculations at each vertex in a polygon, and then
// interpolates the results"; "The system stores the fog factor in the alpha component of the specular color for a
// vertex") and as Microsoft's own runtime with its reference device draws it (t8 fog_vertex_linear, T8_REF).
enum { FOG_OFF, FOG_SPECA, FOG_RANGE, FOG_EYEZ, FOG_RHWZ, FOG_CLIPZ, FOG_RHWW };
struct PSKey {
  struct { uint8_t cop, ca1, ca2, ca0, aop, aa1, aa2, aa0, res, tex, tcn; } s[8];
  uint8_t nstages, alphatest, fog, specadd;   // fog: 0 none, 1 linear, 2 exp, 3 exp2
  uint8_t flat;
  // a test only (D8ON11_TEST_TIES; zero otherwise): bit i set when stage i's MINFILTER / MAGFILTER is POINT, its
  // MIPFILTER LINEAR
  uint8_t tie_minpt, tie_magpt, tie_miplin;
};
// the constants (one buffer per stage; float4 rows)
struct VSConst {
  float wvp[16], wv[16], wvn[16], proj[16];   // world*view*proj, world*view, its inverse transpose (normals), projection
  float tex[8][16];
  float vp[4];                                // viewport x, y, w, h (pre-transformed vertices)
  float fix[4];                               // the D3D8 to D3D11 pixel offset in clip space (x, y); the tween factor (z)
  float ambient[4], mdiff[4], mamb[4], mspec[4], memis[4], mpow[4];
  float ldiff[8][4], lspec[8][4], lamb[8][4], lpos[8][4], ldir[8][4], latt[8][4], lspot[8][4];
  float fog[4];                               // start, end, density, 1/(end-start)
};
struct PSConst { float tfactor[4], fogcolor[4], fog[4], aref[4]; };

enum : uint32_t { XD_DRAW = 0, XD_INDEXED = 1, XD_UP = 2, XD_UP_INDEXED = 3, XD_INLINE_INDEXED = 4 };
struct XDraw {
  VSKey vk; PSKey pk;
  VSConst vc; PSConst pc;
  D3D11_BLEND_DESC blend; D3D11_DEPTH_STENCIL_DESC depth; D3D11_RASTERIZER_DESC raster; D3D11_VIEWPORT viewport;
  uint32_t stencil_ref;
  uint32_t nstages; struct { uint32_t tex; D3D11_SAMPLER_DESC sampler; uint32_t first_level, nlevels; } stage[8];   // tex: an id, 0 none; the levels it may read
  struct { uint32_t buf, stride; } stream[16];                                       // buf: an id, 0 unbound
  uint32_t ib, ib32;                                                                 // the index buffer (XD_INDEXED)
  uint32_t topology;                                                                 // D3D11_PRIMITIVE_TOPOLOGY
  uint32_t kind;                                                                     // XD_*
  uint32_t count, start, base;          // vertices or indices to draw; the first; the base vertex (indexed)
  uint32_t up_vbytes, up_stride;        // XD_UP*: vertex bytes that follow, their stride (stream 0)
  uint32_t ibytes, i32;                 // XD_UP_INDEXED / XD_INLINE_INDEXED: index bytes that follow the vertices; 32-bit
};

static_assert(sizeof(XHdr) == 8 && sizeof(XInit) == 24 && sizeof(XTex) == 32 && sizeof(XUpTex) == 32 && sizeof(XOption) == 12 &&
              sizeof(XSnap) == 20 && sizeof(SnapSlot) == 16 + SNAP_BYTES, "the stream's layout");
static_assert(sizeof(VElem) == 6 && sizeof(VSKey) == 154 && sizeof(PSKey) == 96, "the keys' layout");
// the draw packet, as both builds lay it out (checked with i686 MinGW-w64 g++ and zig 0.16.0 clang for aarch64-windows)
static_assert(sizeof(XDraw) == 3196 && offsetof(XDraw, vc) == 252 && offsetof(XDraw, blend) == 2124 &&
              offsetof(XDraw, stage) == 2512 && offsetof(XDraw, stream) == 3024, "the draw packet's layout");
