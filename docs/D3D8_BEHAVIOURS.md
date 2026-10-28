# D3D8 behaviours our device must reproduce: the SDK's text, two second opinions, ours (plan #139, #138)

docs/D3D11.md section 6 lists D3D8 behaviours "to be checked against the DirectX 8 SDK's documentation and the picture
comparison, not assumed". This file puts side by side, for each: (a) what Microsoft's DirectX 8.1 SDK documentation
says, (b) what DXVK's d3d8 frontend and d3d8to9 do (#138: second opinions, not authorities), (c) what kit/d3d8on11 does,
(d) where they differ. It records differences; it decides nothing and changes no code.

## Sources

- **DirectX 8.1 SDK documentation**: `directx8_c.chm` ("Microsoft DirectX 8.1 (C++)"), file date 2001-10-19 -- the same
  file and date kit/d3d8on11/README.md:272 names as the owner's copy. Taken from archive.org item `dx81sdk_full`,
  `dx81sdk_full.exe` (MD5 28533018267fa278bb1c603a67d86d2f, as archive.org lists it), at
  `DXF/DXSDK/doc/DirectX8/directx8_c.chm`, with the SDK's `include/d3d8caps.h` and `include/d3d8types.h` (2001-10-16).
  Pages are cited by title and by their path in the chm (under `directx_cpp/Graphics/`; PG = `ProgrammersGuide/`,
  Ref = `Reference/CPP/D3D/`). No online URL: Microsoft no longer hosts the DirectX 8 pages, and the archived MSDN
  copies are on web.archive.org, which this session's network refused (connection reset). None of the SDK files is
  in the repository.
- **Microsoft's Direct3D 10/11 pages** (online, fetched 28 Oct 2026), only for what D3D11 does with our output:
  "Coordinate Systems (Direct3D 10)"
  (https://learn.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-coordinates),
  "Primitive Topologies"
  (https://learn.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-primitive-topologies).
- **DXVK** tag `v3.1.1`, commit b1a1c99ab52b687cf950d62c88bc2fa316b41663 (https://github.com/doitsujin/dxvk/tree/v3.1.1).
  Its d3d8 frontend (src/d3d8) forwards almost every state to its own d3d9 device: `D3D8Device::SetRenderState`
  (src/d3d8/d3d8_device.cpp:1700-1769) maps a few states (ZBIAS, EDGEANTIALIAS, ...) and passes ZENABLE, SHADEMODE and
  every fog state unchanged (`State9 = d3d9::D3DRENDERSTATETYPE(State)`, :1703; `return GetD3D9()->SetRenderState(State9,
  Value)`, :1769). So the behaviour below is in src/d3d9.
- **Vulkan specification**, KhronosGroup/Vulkan-Docs commit 70d3045 (70304594e73a065c7da6485165a59ff3d928e719),
  chapters/drawing.adoc and chapters/vertexpostproc.adoc, only for what DXVK's flat shading resolves to. (The
  rendered spec at registry.khronos.org answered 403 to this session; the source on GitHub was used.)
- **d3d8to9** tag `v1.16.0`, commit 255338f698c8270b537f0a91a13f795f4f988250 (https://github.com/crosire/d3d8to9).
  It forwards every state involved here to the system's Direct3D 9 unchanged: `SetRenderState`
  (source/d3d8to9_device.cpp:736-765, `default: return ProxyInterface->SetRenderState(State, Value);` :763-764),
  `SetTextureStageState` (:991-1018, the colour and alpha ops through `default`, :1017-1018), `SetViewport` (:661-675,
  a bounds check, then the viewport unchanged), and the caps are the D3D9 driver's, copied (`ConvertCaps`,
  source/d3d8types.cpp:111-113, `CopyMemory(&Output, &Input, sizeof(Output))`). So for every behaviour below
  d3d8to9's result is whatever the machine's D3D9 runtime and driver do. Its source does not establish that, and it is
  not known here.
- **Ours**: kit/d3d8on11 on this branch (base 2026-10-28). **The game's use**: data/d3d8_scope.json `traces` (the
  owner's traces xikit_trace_5180_1 and xikit_trace_6080_1, 34 frames). These cover what those runs reached and
  nothing more.

## Summary

| Behaviour | SDK 8.1 says | DXVK v3.1.1 | d3d8to9 v1.16.0 | Ours | Differs? |
|---|---|---|---|---|---|
| 1. Pixel centres / XYZRHW offset | pixel centres at integer coordinates; XYZRHW passed to the driver unmodified | viewport moved +0.5 px for all geometry | D3D9's own (not established) | +0.5 px on XYZRHW x, y and on transformed geometry | no difference found |
| 2a. Both fog modes set | "applies only pixel fog" | pixel only | D3D9's | pixel only | no |
| 2b. Vertex fog | factor made per vertex, interpolated | same | D3D9's | same | no |
| 2c. XYZRHW, no table fog | the app's factor in specular alpha; absent specular is 0x0 | specular alpha; **1.0 (no fog) if the vertex has no specular** | D3D9's | specular alpha; absent specular reads 0 (full fog) | ours vs DXVK on absent specular |
| 2d. Range fog | vertex fog with D3D's T&L only | same | D3D9's | same | no |
| 2e. Table fog depth, transformed | w if WFOG and projection not affine (two different "affine" tests in the text) | 1/gl_FragCoord.w (clip w) unless its computed wNear = wFar = 1, then z | D3D9's | eye z unless 4th column is [0,0,0,1], then z | test differs from DXVK; ours uses eye z where DXVK uses clip w |
| 2f. Table fog depth, XYZRHW | 1/RHW; projection matrix used "even if" no D3D transform; affine -> z | always 1/RHW ("PositionT also implies W fog") | D3D9's | 1/RHW unless projection affine, then z | ours vs DXVK with an affine projection |
| 3. W-buffering (D3DZB_USEW) | enables w-buffering; a device without the cap may fall back to z or disable depth | no WBUFFER cap; USEW does z | D3D9 driver's caps and behaviour | no WBUFFER cap (dgVoodoo's caps); USEW does z | no difference from DXVK; not w-buffering |
| 4. Flat shading colour vertex | list 3i, strip i, fan i + 1; specular alpha interpolated | strip i, fan i + 1 (Vulkan first-vertex); **specular alpha flat** | D3D9's | strip: D3D11 leading vertex; fan rotated to i + 1; specular alpha interpolated | DXVK's specular alpha |
| 5. ALPHAOP DISABLE, COLOROP on | "undefined behavior" | alpha unchanged (temp's if result is TEMP; stage 0 to TEMP: diffuse alpha) | D3D9's | the current register's alpha, also when result is TEMP | ours vs DXVK when RESULTARG is TEMP |

## 1. The half-pixel offset of pre-transformed (XYZRHW) vertices

**(a) SDK.** The SDK has no "half-pixel offset" as such. What it states is where D3D8's pixel centres are, and that
it does not touch pre-transformed vertices:
- "Triangle Rasterization Rules" (PG/UsingDirect3D/VertexData/Coordinates/TriangleRasterizationRules.htm): "In
  Direct3D, the center of the pixel is the decisive point. If the center is inside a triangle, the pixel is part of
  the triangle. Pixel centers are at integer coordinates." And: "This description of triangle-rasterization rules
  used by Direct3D does not necessarily apply to all available hardware."
- "Transformed and Lit Vertices" (PG/UsingDirect3D/VertexData/FixedFunction/VertexFormats/TransformedAndLitVertices.htm):
  "In short, Direct3D does not modify transformed and lit vertices at all. It passes them directly to the driver to be
  rasterized." ... "The x and y values must be in screen coordinates".
- "Directly Mapping Texels to Pixels" (PG/UsingDirect3D/Textures/Coordinates/MappingTexelsToPixels.htm) works an
  example, but its numbers contradict each other: "draw a four-sided polygon ... that has screen-space coordinates of
  0.5 to 3.5", then "Because 0.0 is one-half pixel away from the first vertex, at -0.5". It is not used here.

D3D11, for the other side (Microsoft, "Coordinate Systems (Direct3D 10)"): "Pixel centers are offset by
(0.5f,0.5f) from integer locations", and "In Direct3D 9, pixel centers are at integer locations."

**(b) DXVK.** Every draw, transformed or not, goes through one viewport moved by half a pixel
(src/d3d9/d3d9_device.cpp:6843-6844, 6857-6858):
```
    // Correctness Factor for 1/2 texel offset
    constexpr float cf = 0.5f;
    state.viewport = VkViewport{
      float(vp.X)     + cf,    float(vp.Height + vp.Y) + cf,
```
Pre-transformed positions are made into clip space against the unshifted viewport
(src/d3d9/shaders/d3d9_fixed_function_vert.vert:459-466; inverse extent and offset from vp.X, vp.Y,
d3d9_device.cpp:8147-8159), so the +0.5 applies to them as well. **d3d8to9**: nothing of its own; D3D9's convention
is D3D8's (the Microsoft page above).

**(c) Ours.** Pre-transformed: kit/d3d8on11/d8_exec.cpp:86-91, `n = float2((p.x + 0.5 - vp.x) / vp.z * 2 - 1, 1 -
(p.y + 0.5 - vp.y) / vp.w * 2)` (comment: "D3D8 puts pixel centres on integer coordinates, D3D11 on .5"). Transformed:
d8_ff.cpp:225-227 sets `fix = (1/Width, -1/Height)` and d8_exec.cpp:97 adds `fix.xy * o.pos.w` (half a pixel right and
down; the comment names Wine 9.0's get_projection_matrix as the source of the arithmetic). The picture test has a scene
for it, `rhw_rects` (test/t8.c:138-143: rectangles on whole- and half-pixel edges), which README.md:384-388 lists among
the 22 scenes that pass against Microsoft's reference device.

**(d) Differences.** None found: the SDK's integer pixel centres moved to D3D11's .5 centres is +0.5 in x and y, which
is what ours and DXVK both add, to both kinds of geometry. The SDK's own caveat ("does not necessarily apply to all
available hardware") means what dgVoodoo or a given D3D8 driver does is not established by the SDK.

## 2. Fog

**(a) SDK.**
- Both modes set -- "Fog State" (PG/Appendix/DeviceStates/RenderStates/FogState.htm): "Setting either render state to
  D3DFOG_NONE disables pixel or vertex fog, respectively. If both render states are set to valid modes, the system
  applies only pixel fog effects."
- Vertex fog -- "Vertex Fog" (PG/UsingDirect3D/Rendering/FogAlphaDepth/Fog/VertexFog.htm): "it applies fog
  calculations at each vertex in a polygon, and then interpolates the results across the face of the polygon during
  rasterization." D3DCAPS8 (Ref/Structures/D3DCAPS8.htm), D3DPRASTERCAPS_FOGVERTEX: "Device calculates the fog value
  during the lighting operation, and interpolates the fog value during rasterization."
- Pre-transformed vertices, vertex fog -- "Vertex Fog": "If your application does not use Direct3D for transformation
  and lighting, it must perform fog calculations on its own. In this case, your application can place the fog factor
  that it computes in the alpha component of the specular color for each vertex. ... Direct3D uses the supplied fog
  factor to interpolate across the face of each polygon." "Transformed and Lit Vertices": "You can specify a diffuse
  or specular color. If you don't, the system uses 0x0 for specular color and 0xFFFFFFFF for diffuse color." (The SDK
  does not say in the fog pages what an absent specular does to fog.)
- Range fog -- "Vertex Fog": "Direct3D uses range-based fog calculations only when using vertex fog with the Direct3D
  transformation and lighting engine." D3DRENDERSTATETYPE (Ref/Enums/D3DRENDERSTATETYPE.htm), D3DRS_RANGEFOGENABLE:
  "range correction is offered only for vertex fog."
- Pixel (table) fog's depth -- "Pixel Fog" (PG/.../Fog/PixelFog.htm), "Eye-Relative vs. Z-Based Depth": "Eye-relative
  depth is essentially the w element from a homogeneous coordinate set. Microsoft® Direct3D® takes the reciprocal of
  the RHW element from a device space coordinate set to reproduce true w. If a device supports eye-relative fog, it sets
  the D3DPRASTERCAPS_WFOG flag"; "Note With the exception of the reference rasterizer, software devices always use z to
  calculate pixel fog effects."; "Direct3D uses the currently set projection matrix in its w-based depth calculations.
  As a result, an application must set a compliant projection matrix to receive the desired w-based features, even if
  it does not use the Direct3D transformation pipeline."; "Direct3D checks the fourth column of the projection matrix.
  If the coefficients are [0,0,0,1] (for an affine projection) the system will use z-based depth values for fog."
  D3DCAPS8, D3DPRASTERCAPS_WFOG, states the test differently: "W-based fog is used when a perspective projection
  matrix is specified, but affine projections still use z-based fog. The system considers a projection matrix that
  contains a nonzero value in the [3][4] element to be a perspective projection matrix." And "A W-Friendly Projection
  Matrix" (PG/UsingDirect3D/Transforms/ProjectionTransformation/WFriendlyProjectionMatrix.htm): "Computations such as
  these require that your projection matrix normalize W to be equivalent to world-space Z ... If you don't provide a
  compliant matrix, fog effects and depth buffering are not applied correctly." (What is drawn then is not stated.)
- Units -- "Fog Parameters" (PG/.../Fog/FogParameters.htm): Pixel (Z): "Device space [0.0,1.0]"; Pixel (W): "Camera
  space"; Vertex: "Camera space".

**(b) DXVK.**
- Both set: `fogModeVertex = enable && !pixelFog ? vertexFog : D3DFOG_NONE` (src/d3d9/d3d9_state.h:198-202).
- Vertex fog: the factor per vertex (d3d9_fixed_function_vert.vert:268-323, clamped at :304 for LINEAR), output
  `out_Fog` (:727), and with no pixel mode the fragment shader uses it as is (`case D3DFOG_NONE: fogFactor = in_Fog;`,
  d3d9_fixed_function_frag.glsl:222-223).
- Pre-transformed: `if (vertexHasPositionT()) { fogFactor = hasSpecular ? specular.w : 1.0; }` (vert:291-292): without
  a specular colour, factor 1, i.e. no fog.
- Range: `depth = length(pos3)` when `rangeFog()` (vert:284-286), the vertex's view-space position (`vtx.coord`,
  :727; world-view transform at :447); RANGEFOGENABLE feeds it (d3d9_device.cpp:8221).
- Pixel fog depth: `float depth = fogState.useZ ? gl_FragCoord.z : (1.0 / gl_FragCoord.w);` (frag:218), i.e. clip w.
  `useZ` (d3d9_device.cpp:6616-6628): "PositionT also implies W fog, some D3D6 jank apparently relies on that." For
  transformed vertices `fogUseZ = wNear == 1.0f && wFar == 1.0f`, from `ComputeWNearFar()` (:6656-6666), which
  computes the projection's w at the near and far planes (returning 1, 1 when `m[2][2] == 0` or `m[2][2] == m[2][3]`).
  DXVK reports D3DPRASTERCAPS_WFOG and ZFOG (d3d9_adapter.cpp:498-499).

**(c) Ours.** kit/d3d8on11/d8_ff.cpp:161-182 picks the fog coordinate:
- `coordfog = tmode == D3DFOG_NONE && (vk.rhw || vmode == D3DFOG_NONE)` (:164): pre-transformed vertices without
  table fog, and any vertex with both modes NONE, take specular alpha (`FOG_SPECA`, :167; `o.fog = vs.a * 255`,
  d8_exec.cpp:197). An absent specular reads `float4(0,0,0,0)` (d8_exec.cpp:100), so factor 0: full fog colour.
- Table fog first (`else if (tmode != D3DFOG_NONE)`, :174), so with both set only pixel fog is made.
- Table fog depth (:174-177): `affine = P._14 == 0 && P._24 == 0 && P._34 == 0 && P._44 == 1` (the "fourth column"
  test of "Pixel Fog"); pre-transformed: 1/RHW (`FOG_RHWW`, d8_exec.cpp:202 `o.fog = w`, with `w = 1/p.w`, :89), or
  the vertex z if affine (`FOG_RHWZ`, :200); transformed: eye-space |z| (`FOG_EYEZ`, :199), or clip z if affine
  (`FOG_CLIPZ`, :201). The comment (d8_ff.cpp:168-173) quotes "Pixel Fog" and notes that Wine 9.0 uses z for
  pre-transformed vertices.
- Vertex fog (:178-179): range (`FOG_RANGE`, `length(cp)`, d8_exec.cpp:198) or eye |z|; the factor per vertex,
  clamped (d8_exec.cpp:205-208), blended in the pixel shader (d8_exec.cpp:331-334).
- Our caps (dgVoodoo's, d8_main.cpp:11-21, RasterCaps 0x00736191 at byte 36) include WFOG (0x00100000), ZFOG
  (0x00200000) and FOGRANGE (0x00010000) (values: SDK d3d8caps.h:206, 209, 210).
- Tested: fog_vertex_linear, fog_table_linear, fog_table_exp, fog_table_rhw (test/t8.c:291, 319); README.md:373-376
  says Microsoft's runtime draws fog_vertex_linear and fog_table_rhw as ours does, and :384-388 that the scenes pass
  against the reference device.

The game, in traces 5180 and 6080: FOGTABLEMODE only 3 (LINEAR, 4,240 sets), FOGVERTEXMODE only 0 (4,240),
RANGEFOGENABLE only 0 (5,159). README.md:376 adds trace 6796's 255 table-fogged pre-transformed draws.

**(d) Differences.**
1. Pre-transformed vertices with no table fog and no specular colour: ours fogs fully (factor 0), DXVK not at all
   (factor 1). The SDK states the absent specular is 0x0 but not, in words, the fog that results. Not covered by a
   picture test as far as the scene list shows.
2. Table fog on pre-transformed vertices with an affine projection: ours uses the vertex z (following "Pixel Fog":
   the set projection matrix is used "even if it does not use the Direct3D transformation pipeline", and an affine one
   means z), DXVK always 1/RHW. With a perspective projection both use 1/RHW.
3. Table fog's z-or-w test for transformed vertices: three different tests -- the SDK's "Pixel Fog" (fourth column
   [0,0,0,1]; ours), the SDK's D3DCAPS8 text (nonzero [3][4]), and DXVK's computed wNear = wFar = 1. Which matrices
   they disagree on is not worked out here.
4. Table fog's w for transformed vertices: ours uses eye-space |z|, DXVK clip w. These are equal when the projection
   makes w equal to eye z, which is what "A W-Friendly Projection Matrix" asks for ("normalize W to be equivalent to
   world-space Z"); for a non-compliant matrix the SDK says only that fog is "not applied correctly". Whether the game's projection matrices
   are W-friendly is not checked here.
5. The SDK's "software devices always use z" (except the reference rasterizer): which kind of device dgVoodoo's
   behaviour corresponds to is not known.

## 3. W-buffering (D3DRS_ZENABLE = D3DZB_USEW)

**(a) SDK.**
- D3DZBUFFERTYPE (Ref/Enums/D3DZBUFFERTYPE.htm): "D3DZB_USEW Enable w-buffering." D3DRENDERSTATETYPE, D3DRS_ZENABLE:
  "Set this state to D3DZB_TRUE to enable z-buffering, D3DZB_USEW to enable w-buffering, or D3DZB_FALSE to disable
  depth buffering." d3d8types.h:285 `D3DZB_USEW = 2, // W buffering`.
- "Enabling Depth Buffering" (PG/.../DepthBuffers/EnableDepthBuffer.htm): "To use w-buffering, your application must
  set a compliant projection matrix even if it doesn't use the Microsoft® Direct3D® transformation pipeline."
- "Depth Buffers" (PG/.../DepthBuffers/ovwDepthBuffers.htm): "The values in the depth buffer can be a point's
  z-coordinate or its homogeneous w-coordinate ... one that uses w values is called a w-buffer." "Note The actual
  interpretation of a depth value is specific to the 3-D renderer."
- "Querying for Depth Buffer Support" (PG/.../DepthBuffers/QueryDepthBufferSupport.htm): "Although most drivers support
  z-based depth buffering, not all will support w-based depth buffering. Drivers do not fail if you attempt to enable
  an unsupported scheme. They fall back on another depth buffering method instead, or sometimes disable depth
  buffering altogether".
- D3DCAPS8: "D3DPRASTERCAPS_WBUFFER Device supports depth buffering using w." (d3d8caps.h:208, 0x00040000.)
- The SDK does not give the w-buffer's mapping of w to stored depth values.

**(b) DXVK.** The cap is not reported: `/* | D3DPRASTERCAPS_WBUFFER */` (src/d3d9/d3d9_adapter.cpp:497); d3d8's caps
conversion does not add it (src/d3d8/d3d8_util.h:41, 67 touch other bits). The depth test is on for any nonzero
ZENABLE: `state.setDepthTest(rs[D3DRS_ZENABLE]);` (d3d9_device.cpp:7053). So USEW draws as a z-buffer.
**d3d8to9**: USEW goes to D3D9 unchanged, and the caps are the D3D9 driver's; what follows is not known.

**(c) Ours.** `dd.DepthEnable = r[D3DRS_ZENABLE] && curDS ? TRUE : FALSE;` (kit/d3d8on11/d8_ff.cpp:268): USEW draws as
a z-buffer. Our RasterCaps 0x00736191 (dgVoodoo's, d8_main.cpp:11-21) do not have 0x00040000, so the game is told
there is no w-buffering. (Microsoft's reference device reports RasterCaps 0x00772199, README.md:387, which has it.)
No picture test sets USEW (no match in test/t8.c).

The game, in traces 5180 and 6080: ZENABLE only 1 (5,353 sets) and 0 (748); USEW not seen. The static part of
data/d3d8_scope.json was not searched for it here.

**(d) Differences.** Ours and DXVK do the same (no cap; USEW is z-buffering). The SDK allows a device without the cap
to "fall back on another depth buffering method", which is what both do; it does not say which fallback. Neither
does w-buffering. docs/D3D11.md section 6 says dgVoodoo reproduces W-buffering; what the owner's dgVoodoo does with
USEW, given that its caps (the ones ours copies) do not report WBUFFER, is not known.

## 4. Which vertex colours a flat-shaded primitive (D3DSHADE_FLAT)

**(a) SDK.** D3DSHADEMODE (Ref/Enums/D3DSHADEMODE.htm): "D3DSHADE_FLAT Flat shading mode. The color and specular
component of the first vertex in the triangle are used to determine the color and specular component of the face.
These colors remain constant across the triangle; that is, they are not interpolated. The specular alpha is
interpolated." Remarks: "The first vertex of a triangle for flat shading mode is defined in the following manner.
For a triangle list, the first vertex of the triangle i is i * 3. For a triangle strip, the first vertex of the
triangle i is vertex i. For a triangle fan, the first vertex of the triangle i is vertex i + 1." The SDK does not
state the rule for lines or for indexed draws separately.

**(b) DXVK.** Flat shading marks the fixed-function COLOR0 and COLOR1 inputs flat, whole
(src/d3d9/d3d9_fixed_function.cpp:99-101, `flatShadingMask` from `eColor, 0u` and `eColor, 1u`; enabled by
`state.setFlatShading(m_state.renderStates[D3DRS_SHADEMODE] == D3DSHADE_FLAT)`, d3d9_device.cpp:7103). The specular
alpha is in COLOR1, so it is flat too. DXVK sets no provoking-vertex mode (no "provoking" in src/ at v3.1.1), so
Vulkan's default applies: "If this structure is not provided when creating the pipeline, the pipeline will use the
VK_PROVOKING_VERTEX_MODE_FIRST_VERTEX_EXT mode" (vertexpostproc.adoc:800-801); then for a strip "the provoking vertex
for p_i is v_i" (drawing.adoc, Triangle Strips) and for a fan, whose triangle is `p_i = {v_i+1, v_i+2, v_0}`, "the
provoking vertex for p_i is v_i+1" (drawing.adoc, Triangle Fans). **d3d8to9**: D3D9's; not known.

**(c) Ours.** kit/d3d8on11/d8_exec.cpp:55-61: with flat shading, COLOR0 and COLOR1 are `nointerpolation`, and the
specular alpha goes as a separate interpolated value (`float d1a : SPECALPHA`), quoting D3DSHADEMODE's "The specular
alpha is interpolated". The vertex D3D11 takes is its "leading vertex": Microsoft, "Primitive Topologies": "a leading
vertex is the first non-adjacent vertex in a primitive ... For a triangle strip with adjacency, the leading vertices
are 0, 2, 4, 6, and so on." In words, Microsoft states the leading vertices only for strips with adjacency and lists;
for a plain triangle strip the page shows them in its diagram only. Lists and strips go to D3D11 as lists and strips.
Fans are rewritten as lists (d8_ff.cpp:322-327) with each triangle rotated to start at its "first non-spoke vertex",
`(i+1, i+2, 0)`, quoting D3DSHADEMODE's fan rule. The picture test `flat_shading` (test/t8.c:388-399: a two-triangle
list, a two-triangle strip with four different corner colours, a three-triangle fan) passes against Microsoft's
reference device (README.md:384-388).

The game, in traces 5180 and 6080: SHADEMODE 1 (FLAT) set 612 times (GOURAUD 1,681); primitive types only
TRIANGLESTRIP (102,979) and TRIANGLELIST (30,234): no fans drawn in those traces.

**(d) Differences.**
1. Specular alpha under flat shading: the SDK and ours interpolate it; DXVK holds it flat (COLOR1 whole).
2. The strip and fan colour vertex: ours, DXVK and the SDK agree on vertex i (strip) and i + 1 (fan). For ours on a
   plain strip, the D3D11 side rests on Microsoft's diagram and on its adjacency-strip sentence, and on the
   reference-device comparison of a two-triangle strip; a longer strip is not in the test.

## 5. ALPHAOP = D3DTOP_DISABLE with COLOROP on (kit/d3d8on11/README.md:392-398)

**(a) SDK.** D3DTEXTUREOP (Ref/Enums/D3DTEXTUREOP.htm), D3DTOP_DISABLE: "Disables output from this texture stage and
all stages with a higher index. To disable texture mapping, set this as the color operation for the first texture
stage (stage 0). Alpha operations cannot be disabled when color operations are enabled. Setting the alpha operation
to D3DTOP_DISABLE when color blending is enabled causes undefined behavior." The SDK says nothing more. Microsoft's
reference device makes alpha 1 there (README.md:395, measured 14 Oct).

**(b) DXVK.** A stage whose alpha op is DISABLE leaves alpha as it was in the register the stage writes
(d3d9_fixed_function_frag.glsl:559-560, 578-582: `vec4 prev = info.storeToTemp ? state.temp : state.current; vec4 next
= prev;` ... `if (info.alphaOp != D3DTOP_DISABLE) { ... next.a = ...; }`). Stage 0 writing TEMP is special-cased to
diffuse alpha: "Initialize first alpha stage with diffuse alpha if left undefined." (d3d9_device.cpp:8419-8425,
`if (!i && resultIsTemp && colorOp != D3DTOP_DISABLE && alphaOp == D3DTOP_DISABLE) { alphaOp = D3DTOP_SELECTARG1; ...
alphaArg1 = D3DTA_DIFFUSE;`). The current register starts as the diffuse colour (`state.current = in_Color0;`,
frag:599). **d3d8to9**: D3D9's; not known.

**(c) Ours.** `std::string ar = t.aop == D3DTOP_DISABLE ? "cur.a" : op(...)` (kit/d3d8on11/d8_exec.cpp:314), written to
the stage's result register (`tmp` or `cur`, :316-317); `cur` starts as the diffuse colour (`float4 cur = i.d0`, :267).
README.md:392-398: "ours and Wine pass the previous stage's alpha on", the reference device makes 1, "Ours is unchanged
until that is known" (what dgVoodoo makes; scenes alphaop_disabled_stage0/1, run on the owner's machine via
`owner_run/`).

The game, in traces 5180 and 6080: D3DTSS_RESULTARG set only on stage 0, only to 1 (D3DTA_CURRENT, 51 sets).

**(d) Differences.**
1. With the result going to CURRENT (the game's only traced case), ours and DXVK both keep the current alpha (the
   diffuse alpha at stage 0). Microsoft's reference device makes 1. The SDK says undefined.
2. With RESULTARG = TEMP: ours writes the current register's alpha into TEMP; DXVK keeps TEMP's alpha (stage 0: the
   diffuse alpha). Not reached in the traces above.
