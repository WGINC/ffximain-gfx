# d3d8on11: our own Direct3D 8 device on Direct3D 11

Design A of docs/D3D11.md: a `d3d8.dll` that takes dgVoodoo's place in the game's folder and implements the part of
Direct3D 8 the game uses (`data/d3d8_scope.json`) on Direct3D 11.

## How it is built

Two halves joined by a command stream (`d8_proto.h`): the D3D8 side, which the game calls, and the executor, the only
code that uses Direct3D 11.

- D3D8 side, in `d3d8.dll`:
  - `d8_main.cpp`: the module (`Direct3DCreate8`), caps, IDirect3D8.
  - `d8_device.cpp`: IDirect3DDevice8: the state the game sets and reads back, state blocks, resources, frames.
  - `d8_res.cpp`: textures, cube textures, surfaces, vertex and index buffers, each with a CPU copy the game locks.
  - `d8_ff.cpp`: a draw, decided: the fixed-function state becomes the shader keys, the constants and the D3D11 state
    descriptions, in one draw packet (`XDraw`) with the ids of the buffers and textures it reads.
  - `gen_stubs.py` → `d8_gen.h` (generated at build): every method of every interface, logging "not implemented"
    once and failing. The device's classes override what they implement. A method the game calls that is not
    implemented is found in the log, not guessed. The vtable order is the D3D8 header's: a method missing from the
    generator leaves a class abstract and the build fails.
- The stream, `d8_proto.h`: fixed-width records with no pointers, laid out the same in an x86 and an ARM64 build
  (pinned by static_asserts). Objects are named by ids. Four records are answered (X_INIT, X_RESIZE, X_READBACK,
  X_FINISH); the D3D8 side waits only for those. Everything else runs later, in order.
- The executor, `d8_exec.cpp`: runs the records in order on Direct3D 11. It holds the device, the swap chain on the
  game's window, the resources by id, and the shader generator: each key becomes HLSL once, is compiled with
  `D3DCompile` from `d3dcompiler_47.dll` (loaded at run time), and is cached; state objects are cached by
  description.
- The transport, `d8_link.cpp`, one of three modes:
  - `direct`: each record runs as it is written, on the game's thread (phase 1);
  - `thread` (the default): a 32 MB ring in the game's process, read by an executor thread (phase 2, design A2).
    The game waits only for answered records, for room in the ring, and when it is two Presents ahead of the
    executor;
  - `remote`: the same ring in a shared mapping, read by an executor process started next to the DLL (design B, "ARM
    mode"): `d8x_arm64.exe`, built natively for ARM64, when Windows reports an ARM64 machine (IsWow64Process2) and
    it is there, else `d8x.exe` (x86). It presents into the game's window. Its log is `d3d8on11_exec.log`. If it
    cannot be started, or cannot make the device and swap chain, the device falls back to `thread` and logs why. If
    it ends while the game runs, the game is ended with a log line: the game's resources live in it.
  - Chosen by `d3d8on11.ini` next to the DLL (`[d3d8on11]` `mode=direct|thread|remote`), or the `D8ON11_MODE`
    environment variable, which wins. The log's second line names the mode in use.
  - Every 600 Presents the log gets `executor stats` (the executor's busy time a frame) and, in `thread` and `remote`,
    `ring stats`: how long the game thread waited for room, for pacing and for answers, and the MB it sent. In
    `remote` mode the executor's line is in `d3d8on11_exec.log`.
  - Diagnostics: `D8ON11_DRIVER=warp` makes the D3D11 device on WARP, Microsoft's software rasterizer, instead of the
    GPU; `D8ON11_DEBUG_LAYER=1` asks for the D3D11 debug layer and writes its messages to the log at each Present
    (Windows needs the optional Graphics Tools feature, or the device is not made). Both log a `D3D11 device:` line.
  - For the tests only: `D8ON11_RING_KB` (a power of two, 64 to 32768) makes the ring small so it wraps, and
    `D8ON11_RING_START` (hex) starts its byte counters near 2^32 so they wrap too. The ring's size must divide 2^32:
    positions are counter % size.
- `build.sh` builds `dist/d3d8.dll` and `dist/d8x.exe` with MinGW-w64, and `dist/d8x_arm64.exe` with zig's clang
  (PyPI `ziglang` 0.16.0, as kit/d3d11probe does). All three are reproducible and locked in kit/BUILD.lock.json.

**Locked data.** Textures and buffers keep a CPU copy that the game writes through Lock. Unlock sends what changed
(a buffer's range, a texture level). A lockable render target or the back buffer read back at LockRect waits for the
executor to reach it, as the game's occlusion readback does on any D3D8.

**Ring memory.** `thread` and `remote` take 48 MB of the game's address space (the 32 MB ring and a 16 MB reply
area, which bounds one read-back). A record larger than half the ring (16 MB) ends the game with a log line. How large the game's largest
record is (a ...UP draw carries its vertices) is not established.

**Bytes per draw.** A draw packet is 3,128 bytes before any ...UP or fan data (pinned in d8_proto.h for both builds),
so 3,000 draws make about 9.4 MB a frame through the ring, written by the game's thread and read by the executor.
What that copying costs, against `direct` (which builds the same packet but runs it in place), is not measured.

## Milestone 1: everything but drawing

Done and tested:
- device creation with its swap chain on the game's window (windowed), automatic depth buffer, Reset;
- textures (BC1/BC2/BC3, 32-bit, 16-bit, A8), cube textures, render targets, depth surfaces, image surfaces, vertex
  and index buffers, with Lock/Unlock uploads;
- Clear of the whole target, Present, SetRenderTarget (which resets the viewport, as D3D8 does), CopyRects (GPU to
  GPU, and GPU to an image surface), read-back through LockRect;
- every state the game sets and reads back: render states, stage states, transforms, viewport, material, lights,
  textures, streams, indices, vertex shader handles;
- state blocks recorded with Begin/End, applied, captured, deleted, and CreateStateBlock(ALL).

## Milestone 2: drawing (`d8_ff.cpp`)

At a draw, the state that decides the vertex work becomes a key (VSKey), and so does the state that decides the pixel
work (PSKey):
- vertex work: the layout from the FVF or the declaration, lighting, material sources, texture coordinate sources and
  transforms, the fog coordinate, flat shading;
- pixel work: the stages, alpha test, fog mode, specular, flat shading.

The keys, the constants and the state descriptions go to the executor in the draw packet. There each key becomes
HLSL once, compiled with `D3DCompile` and cached; blend, depth-stencil, rasterizer and sampler states are D3D11 state
objects, cached by description. Fans become indexed lists, their indices made on the D3D8 side. ...UP draws carry
their vertices and indices in the packet; the executor writes them into a dynamic ring buffer.

Where each behaviour comes from (named in the code where it is used): the DirectX 8.1 (C++) reference first (see
"Checked against Microsoft's documentation" below), then Microsoft's Direct3D 9 pages where the 8.1 reference is
silent, each named as such; Wine 9.0's wined3d only where neither says, and then named as Wine's. Windows' own
behaviour in those cases is not established. Still Wine's:
- the fog model's specular-alpha case and vertex linear fog's start == end case (`get_fog_start_end`);
- ZBIAS as −ZBIAS units and −ZBIAS times the slope (d3d8 `WINED3D_LEGACY_DEPTH_BIAS`): the reference says only that a
  higher value draws in front;
- the half-pixel offset's arithmetic (`get_projection_matrix`); that pixel centres are at integer coordinates is the
  reference's ("Triangle Rasterization Rules");
- the clamp of each light's N.L at 0, specular's N.L > 0 and N.H > 0, the spot factor;
- DOTPRODUCT3's scale of 4 (the reference: inputs "shifted down by half", no scale given).

Taken from Microsoft's own DirectX 8.1 runtime with its reference device (14 Oct; "Microsoft's reference device"
below), where Wine or the reference text said otherwise:
- vertex fog makes the fog factor per vertex, clamped to [0, 1], and interpolates it (Wine interpolates the distance
  and makes the factor per pixel). The reference says the same: "it applies fog calculations at each vertex in a
  polygon, and then interpolates the results" ("Vertex Fog");
- D3DRS_AMBIENTMATERIALSOURCE defaults to D3DMCS_MATERIAL, D3DRS_DEBUGMONITORTOKEN answers 0xBAADCAFE ("defaults").
- a stage's texture read when none is set (other than through COLORARG1, below) reads (0, 0, 0, 1): through ARG2,
  ARG0, the alpha args, with COMPLEMENT and ALPHAREPLICATE (the reference does not say; Wine passes CURRENT on, its
  `is_invalid_op`);
- the alpha test compares floor(alpha * 255), taken once after the last stage, with ALPHAREF, for every function
  (alpha T*T, a two-stage chain and ADDSIGNED's half steps against each). The reference says ALPHAREF "is an 8-bit
  value" and not how the incoming alpha becomes one. Ours compared the float for LESS and GREATER and within half a
  step for EQUAL and NOTEQUAL (Wine: the float), so it could pass EQUAL 145 and fail GREATEREQUAL 145 on one value.
  The 1/512 added before the floor is ours (a product of 8-bit values is m/255; float rounding must not drop it a
  step). The game alpha-tests nearly all its draws, with GREATER 0, GREATER 0x45 and GREATER 0x60 (traces 5180,
  6080, 6796).

From the DirectX 8.1 text, where Wine did otherwise:
- "When a texture stage has D3DTSS_COLORARG1 equal to D3DTA_TEXTURE and the texture pointer for the stage is NULL,
  this stage and all stages after it are not processed" ("Texture Blending"). Microsoft's reference device does so
  for every op (SELECTARG2 too) and for alpha; a stage 0 so skipped draws the diffuse colour and alpha, as DISABLE
  does; TEXTURE with a modifier is not "equal". Wine 9.0 passes CURRENT on and runs the later stages. The game draws
  so: 23 and 28 draws of traces 6796_1 and 6796_2, stage 0 from 0x1000CBF3 (colour MODULATE(TEXTURE, TFACTOR),
  alpha MODULATE4X), some with a later stage on; traces 5180 and 6080 have none.

Measured on Microsoft's reference device and not taken (each a reference-device artefact, not Direct3D's rule):
- it truncates the lit vertex colour to 8 bits per vertex before interpolating (ours interpolates the float): one
  step lower, which a stage squaring the colour makes four (random case s2r0073: 934 pixels differ, 0 with ours
  truncated). It does so with D3DCREATE_HARDWARE_VERTEXPROCESSING too. The game creates its device with
  HARDWARE_VERTEXPROCESSING (0x40) unless its setting at g_pAppSettings+88 is set or that fails, then SOFTWARE (0x20)
  (Gfx_CreateDevice 0x100034B0, Gfx_CreateD3DHolder's callers); what its machines' drivers do here is not known;
- point sampling near a texel boundary: "the reference rasterizer does not perform this operation with perfect
  accuracy" (DirectX 8.1, "Nearest-Point Sampling"). Measured (14 Oct) on the random cases that differ from it with no
  class: the unmatched pixels of the point-filtered perspective cases lie within 0.05 texels of a texel boundary
  (mostly within 0.006) by the exact texture coordinates; ours takes the texel the exact coordinate falls in, the
  reference device the neighbour (case m4: 17 of 17 pixels). The test switch D8ON11_TEST_TEXEDGE=eps makes ours read
  the texel across the nearer boundary wherever the coordinate is within eps texels of it (every 2D stage; a test
  only): at eps 0.05, 58 of the 59 unmatched pixels of the four reduced cases then match, and of the 1920 unmatched
  pixels of the 67 random cases (of 142) that still differ with their classes' changes, 856 match; of the 1064
  left, 934 are s2r0073 (the lit colour, above) and the rest (at most 29 a case, many differing by 4 or 5 where 3 is allowed) are not analysed: not
  explained.

Known gaps, each logged when hit:
- full screen (renders windowed);
- CreateStateBlock of the pixel or vertex state only (the game creates D3DSBT_ALL only: its two call sites);
- palettized texture formats (the palettes themselves are kept and read back; the game creates no palettized texture);
- textures in a block format whose top level is not whole 4x4 blocks: D3D11 wants whole blocks; behaviour to be
  checked;
- caps: the device answers with dgVoodoo's own D3DCAPS8 from the owner's machine (probe8's caps.raw, 13 Oct;
  data/dgvoodoo_caps8.json), so the game takes the paths it takes there. They promise features not implemented here
  yet (vertex blending, the game's own shaders); those are logged when used.

## Checked against Microsoft's documentation

Sources: the DirectX 8.1 SDK's own documentation (directx8_c.chm, 19 Oct 2001) and headers (d3d8types.h), from the
owner's copy of the SDK (14 Oct; neither is in the repository; the code quotes what it relies on), and Microsoft's
Direct3D 9 pages where named, and the Direct3D 11 pages of the June 2010 SDK (windows_graphics.chm) where named.
Checked, area by area:
- Render-state and stage-state defaults: all as the reference states (README "defaults" below), and as Microsoft's
  runtime answers them. Changed: POSITIONORDER, NORMALORDER; AMBIENTMATERIALSOURCE and DEBUGMONITORTOKEN, where the
  runtime answers otherwise than the reference text (the text contradicts itself on the first).
- Texture operations (D3DTEXTUREOP's formulas) and argument flags: as the reference. Not stated there: whether results
  are clamped (ours: each to [0, 1]); DOTPRODUCT3's scale.
- Default vertex colours: diffuse 0xFFFFFFFF, specular 0 ("Untransformed and Lit Vertices", "Transformed and Lit
  Vertices"; the "Texture Argument Flags" page says 0xFFFFFFFF for specular, against both).
- Texture coordinates: a missing set reads (0,0) (D3DTSS_TEXCOORDINDEX); a 2D set enters a transform with 1 as its
  third component, as the reference's own example needs ("Special Effects": translation in the matrix's third
  row); its fourth (ours 0) is not stated. No transform on pre-transformed vertices (Direct3D 9, "Texture Coordinate
  Transformations"). Filtering: MAGFILTER and MINFILTER by magnification, MIPFILTER NONE "disables mipmapping".
- Lighting: the "Mathematics of Lighting" pages, term for term; the ambient term differs between 8.1 (no attenuation)
  and 9 (with), ours is 9's, and every SetLight the game traced has ambient 0.
- Fog: the formulas (linear, exp, exp2, the blend), the depths (camera |z|, range), vertex fog's factor made per
  vertex and interpolated ("Vertex Fog"; changed 14 Oct, found against Microsoft's runtime), and pixel fog's depth: w on a
  device with D3DPRASTERCAPS_WFOG (ours: dgVoodoo's caps) unless the projection is affine, 1/RHW for pre-transformed
  vertices (changed: was z, as Wine; no test compares this case).
- Flat shading: colour and specular from the first vertex (fans: vertex i + 1), specular alpha interpolated (changed:
  it was held too).
- Blend factors and operations, culling, the fill convention: as the reference. The alpha test: as above (the
  reference does not say how the alpha is made 8-bit).
- Clear: the viewport, or rectangles clipped to it; fails for Z or stencil with no depth buffer or no stencil
  (changed: rectangles and smaller viewports failed; now drawn, test scene clear_viewport_rects, equal to Wine's).
- State blocks: D3DSBT_ALL holds what the reference lists (changed: it held the index buffer, which is not listed,
  and not the palette, which is); recorded blocks record the methods the reference lists.
- Palettes: SetPaletteEntries / SetCurrentTexturePalette as the reference (they failed before; the game calls both).
- CopyRects: fails as the reference lists (the same surface, formats that differ, depth formats, a rectangle not
  inside both surfaces; changed: none of these was checked). A copy into a texture level also updates the copy the
  game locks (changed: it did not); a copy from the GPU into a lockable level is logged (`copystale`): the CPU copy
  is not refreshed.
- Locks: "Textures created in D3DPOOL_DEFAULT are not lockable" unless dynamic (changed: they were; in run 2's resource
  log every DEFAULT-pool texture the game made is a render target, whose locks were already refused); READONLY sends
  nothing back; a render target or back buffer locked for writing gets the locked rows back (changed: they were
  dropped; basic check "a texel written through a rectangle lock"). A draw with a locked vertex buffer set fails, as
  IDirect3DVertexBuffer8::Lock says (changed: it drew; logged `drawlocked`). Whether the game ever does this is not
  known: its traces record vertex-buffer unlocks, not locks. The DISCARD / NOOVERWRITE / READONLY combinations the
  reference allows per pool are not enforced.
- Reset and lost devices: a failed Reset leaves the device "not reset": TestCooperativeLevel answers
  D3DERR_DEVICENOTRESET and Present D3DERR_DEVICELOST until a Reset succeeds (changed: both answered D3D_OK).
- Vertex declarations: registers as D3DVSDE_* (d3d8types.h); 15 and 16 (POSITION2, NORMAL2) were not mapped (a draw
  was skipped). Now mapped, and read when D3DRS_VERTEXBLEND is D3DVBF_TWEENING: "POSITION = POSITION1 * (1.0 -
  TWEENFACTOR) + POSITION2 * TWEENFACTOR", the same for normals ("Vertex Tweening"; dgVoodoo's caps report
  D3DVTXPCAPS_TWEENING). This changes nothing the game was seen doing: its 2,253 captured shaders are three
  declarations without register 15 or 16 (docs/HISTORY.md); v15 is only in the declarations of its shader programs,
  which have code and were never reached. Scene v15_declaration (that declaration without code, VERTEXBLEND 0)
  equals Wine's; the basic check "tweening at 0.5" runs on ours only (Wine's d3d8 does not report the cap). Other
  VERTEXBLEND values are logged (`vblend`). Types expand as D3DVSD_REG says ("(value, value, 0.0, 1.0)" and so on);
  SHORT2 did not compile (changed; scenes decl_short2, decl_short4).
- Texture addressing and border colour: D3DTEXTUREADDRESS's five modes have the same definitions, word for word, as
  D3D11_TEXTURE_ADDRESS_MODE's (June 2010 SDK); the border colour's default is 0, as D3DTSS_BORDERCOLOR says.
- Present: rectangles and another window are not implemented (logged `presentargs`). The game passes NULL for all
  four arguments in each of the 34 Presents of its two traces.
- MAXMIPLEVEL: neither the 8.1 reference (D3DTSS_MAXMIPLEVEL) nor the Direct3D 11 pages of the June 2010 SDK
  (D3D11_SAMPLER_DESC, D3D11_TEX2D_SRV, SetResourceMinLOD) say whether magnification and minification are decided
  against level 0 or against the most detailed level allowed. Ours decides against the latter (a view starting at
  that level). The game was not seen setting MAXMIPLEVEL or MIPMAPLODBIAS in its traces (MIPFILTER NONE then gives
  level 0, where both readings agree). For Direct3D 11 itself the functional specification says: "The selection of
  minification vs magnification occurs after LOD clamping. Prior to feature level 11.0, it was undefined whether
  magnification selection occurred before or after LOD clamping" (D3D11.3 Functional Spec 7.18.11); that is about
  the sampler's own clamp, which ours does not use for this.
- Not yet checked against the reference: the caps' use.

## Tests (`test/test_d3d8on11.py`, Wine on Mesa's software renderer in a virtual X server)

`t8.exe` makes the same D3D8 calls through Wine's own d3d8 and through ours, ours in each of the three modes (`remote`
with the x86 `d8x.exe`: Wine here runs x86 only):
- `basic`: 24 checks (Clear and read back, a lockable render target, textures, buffers, state blocks, transforms,
  lights, shader handles, Reset, release, a rectangle lock of the target, tweening). All pass on both (tweening on ours
  only: Wine's d3d8 does not report D3DVTXPCAPS_TWEENING).
- `scenes`: 27 scenes, each drawn on a 64x64 back buffer and read back whole, compared pixel by pixel. They cover:
  - the six FVF codes the game uses (data/d3d8_scope.json), a declaration with register 15, SHORT2 and SHORT4 elements
    (as texture coordinates, which Microsoft's runtime does not define for the fixed-function pipeline: the debug
    runtime rejects them; these two check our expansion against the D3DVSD_REG text only);
  - strips, lists, fans, indexed and ...UP draws, base vertex;
  - depth and culling, ZBIAS 8;
  - lighting with material and vertex colour sources, specular, eight point and spot lights;
  - texture stages with TFACTOR, MODULATE2X and BLENDTEXTUREALPHA;
  - the blend modes and BLENDOP values the game sets, alpha test;
  - vertex, table linear and table exp fog, table fog on pre-transformed vertices (untextured: a texture minified toward the horizon made those pictures
    turn on where each pixel samples);
  - texture transforms, camera-space position, normal and reflection texgen into a cube texture;
  - DXT1 with mips, flat shading;
  - Clear of a smaller viewport and of rectangles;
  - a stage whose colour op is on and whose alpha op is DISABLE, on stage 0 and on stage 1, in the game's state at
    its draws from 0x1000CBF3 (alphaop_disabled_stage0/1).

  On 14 Oct 25 of 27 match Wine's d3d8 (worst channel difference 1 of 255, lights_8; none beyond the test's
  tolerance of 8), and `thread` and `remote` draw exactly what `direct` draws (every pixel). Two differ from Wine as
  listed in the test (WINE_DIFFERS), and Microsoft's runtime draws both as ours does: fog_vertex_linear (Wine's
  vertex fog is not Microsoft's, below) and fog_table_rhw (table fog on pre-transformed vertices: the DirectX 8.1
  "Pixel Fog" text takes 1/RHW "to reproduce true w", as ours; Wine 9.0 reads the vertex z, glsl_shader.c). The game
  draws such vertices with table fog (trace 6796: 255 draws, from 0x1000CDE2 and 0x1000CBF3). The tolerance and the 1% limit
  are the test's own criteria. With KEEP=1 each scene is written as a PNG (Wine | ours | difference).
- Microsoft's reference device (with D3D8REF_DIR and WDK_DDI set; otherwise the test says it did not run): the
  DirectX 8.1 SDK's retail d3d8.dll and d3dref8.dll under Wine, with our stand-in for d3d8thk.dll, the runtime's calls
  into the NT kernel's DirectDraw (test/msref: kthunk.c emulates a display driver with no 3D; each thunk's argument
  count from the real d3d8thk.dll's `ret N`, gen_thunks.py). t8 makes a D3DDEVTYPE_REF device (T8_REF=1), and the
  scenes and defaults are compared with ours:
  - scenes: a pixel passes within 3 of the reference's pixel or, both ways, of one of its 3x3 neighbours. Every pixel
    beyond 3 on 14 Oct was computed to be a tie: a pixel centre on a triangle edge (or within 0.013 px of one), or a
    point sample on a texel boundary (within 0.006 texel), or a cube lookup within 0.2% of a face edge; the four the
    neighbourhood does not absorb are listed in the test with their numbers (REF_TIES). Not compared: zbias (the
    reference device does not report D3DPRASTERCAPS_ZBIAS; its RasterCaps are 0x00772199) and decl_short2/4 (above).
    Listed as undefined, not failed: alphaop_disabled_stage0/1 (below). All 22 others pass;
  - defaults: 290 states equal; left out are D3DRS_SOFTWAREVERTEXPROCESSING (it follows the creation flags), numbers
    above 173, and the 51 the runtime answers 0xBAADCAFE (its fill for a state never set), and Microsoft's runtime
    fails GetRenderState for the retired numbers (1-6, 11-13, ...) where ours answers 0.
  What it found: vertex fog (above), and the two defaults below. And one case the reference leaves undefined that the
  game relies on: a stage whose colour op is on and whose alpha op is D3DTOP_DISABLE ("Setting the alpha operation to
  D3DTOP_DISABLE when color blending is enabled causes undefined behavior", D3DTEXTUREOP). The reference device makes
  alpha 1 there; ours and Wine pass the previous stage's alpha on. The game does it from 0x1000CBF3, with SRCALPHA
  blending, in 8,715 of the 34,011 draws of trace 6796 (1,132 in 5180, 85 in 6080), and the alpha shows on screen.
  What the owner's dgVoodoo makes there is not known: the scenes alphaop_disabled_stage0/1 draw it, and `owner_run/`
  runs them on the owner's machine. Ours is unchanged until that is known. Microsoft's debug runtime (d3d8d.dll) checks more than
  the retail one: it rejects a fixed-function declaration whose position is not FLOAT3 or whose texture coordinates are
  not FLOAT1-4, among others; the retail runtime, which the game runs on, does not check, and neither does ours.
- `defaults`: every render state and stage state of a fresh device, read on both and compared, and (by hand, 14 Oct)
  with the defaults the DirectX 8.1 reference states (D3DRENDERSTATETYPE, D3DTEXTURESTAGESTATETYPE). Every one of the
  76 render states Microsoft's d3d8types.h defines, and every stage state, now has the reference's stated default:
  - two did not, and were changed: D3DRS_POSITIONORDER (172) to D3DORDER_CUBIC and D3DRS_NORMALORDER (173) to
    D3DORDER_LINEAR (both as Wine has them; ours had 0);
  - D3DRS_AMBIENTMATERIALSOURCE (147): the reference contradicts itself (D3DRENDERSTATETYPE: D3DMCS_COLOR2; "Color
    Vertices": D3DMCS_MATERIAL). Ours had COLOR2; Microsoft's runtime answers MATERIAL, as Wine and the Direct3D 9
    reference, and ours does now (no difference to the game's vertex formats: d8_device.cpp);
  - D3DRS_DEBUGMONITORTOKEN (165): the reference says it "defaults to D3DDMT_ENABLE" (0), which ours had; Microsoft's
    runtime answers 0xBAADCAFE, as Wine, and ours does now. D3DRS_POINTSIZE_MAX (166) is 64.0 on ours and on
    Microsoft's runtime with its reference device (Wine 256.0);
  - the other differences from Wine are numbers above 173, which d3d8types.h does not define (Wine's d3d8 shares
    its state with D3D9). An earlier version of this file put 172 and 173 among them; they are defined.
  - ALPHAREF, ZVISIBLE and WRAP0-7 have no default in the reference; ours are 0, as Wine's.

    KEEP=1 python3 test/test_d3d8on11.py

- `frames`, in each mode: 600 frames of Clear and Present, with a read-back each frame from frame 30, through a 64 KB
  ring. All must run, and the stats lines must be written. (This test exists because a change made while adding the
  stats wrote a wrong size into every record header; the scenes, which present a few times, had not shown it. It was
  found and fixed before any commit.)
- The scenes again in `thread` and `remote` through a 64 KB ring whose counters start at 0xFFFF0100: about 165 KB
  go through, so the ring wraps twice and the counters pass 2^32. The pictures must equal `direct`'s exactly.
- `orphan` (remote mode only): t8 ends its executor process at frame 30 and goes on presenting and reading back. The
  game must end with exit code 3 and the log line, not hang. It does.
- `random`: randomized cases (`test/random_cases.py`), each state drawn from the values the game was seen setting
  (data/d3d8_scope.json), over its six FVF codes, strips and lists, UP, buffer and indexed draws, with lights, texture
  transforms and cube textures. 200 per run (`RANDOM_SEED`, `RANDOM_CASES`); compared with Wine's d3d8 as the scenes
  are. A difference fails unless it falls in a class below and the case, changed only as its class says, then draws
  the same on both (0 pixels in every case of seeds 1 to 3, 14 Oct, after the two fixes below the classes). A case
  that still differs is drawn by Microsoft's runtime too when D3D8REF_DIR is set: where that matches ours by the
  reference criterion, the difference is listed as Wine's alone, not failed:
  - A: pre-transformed vertices with a stage generating coordinates. Wine 9.0 generates them from the screen
    position; ours reads the set as given. Microsoft's text: "Direct3D does not modify transformed and lit vertices"
    (Direct3D 9, "Texture Coordinate Transformations"; the same in the Windows CE .NET 4.2 Direct3D text). It names
    transforms, not generation, so for generation this is a reading. Wine's own bug 37313 reads it the same way.
    Change: the stage stops generating.
  - C: a stage reading a coordinate set the vertex format does not have. Ours reads zeros; not in the reference.
    Change: the stage reads set 0 (or, with no set, is turned off: COLOROP DISABLE; a stage left without its
    texture is drawn by the "Texture Blending" rule above, and by Wine otherwise).
  - S: a cube texture filtered linearly. Wine's d3d8 filters each face apart (WINED3D_LEGACY_CUBEMAP_FILTERING,
    dlls/d3d8/d3d8_private.h); ours leaves it to Direct3D 11, whose functional specification requires filtering
    across faces ("TextureCube filtering near Cube edges, where 2x2 (bilinear) filter taps would fall off a face are
    required to spill over by one texel row/column to the appropriate adjacent map", D3D11.3 Functional Spec 7.18.12).
    Neither the 8.1 reference nor the June 2010 SDK's Direct3D 11 pages say which a Direct3D 8 device does, and what
    Direct3D 8 hardware did is not established.
    Change: that stage point-filtered.
  - L: lighting with normals longer than 1, NORMALIZENORMALS off. Wine clamps each light's N.L at 1; the reference's
    equation, sum[Cd*Ld*(N.Ldir)*Atten*Spot] clamped after all lights ("Diffuse Lighting", Direct3D 9), does not;
    ours follows the equation. Change: NORMALIZENORMALS on.
  - F: linear fog with FOGSTART >= FOGEND. The DirectX 8.1 text defines start as where fog "begins" and end as where
    it "no longer increases" ("Fog Formulas"), which contradict each other then. Microsoft's reference device fogs
    fully from end on and not before (measured along a depth ramp, 14 Oct); Wine and ours compute the formula. The
    game never sets it (every fogged draw of traces 5180, 6080 and 6796 has start < end). Change: FOGEND above
    FOGSTART.
  - W: table fog on pre-transformed vertices: Wine fogs by z, Microsoft's runtime and ours by 1/RHW (scene
    fog_table_rhw, above). Change: fog off.
  - D: a stage with its colour op on and its alpha op DISABLE, which the DirectX 8.1 text calls undefined
    (D3DTEXTUREOP). The game does it (0x1000CBF3). Change: that alpha op SELECTARG1 of CURRENT.
  - N: a stage reading a texture it was not given: ours by the "Texture Blending" rule above, Wine otherwise. The
    game does it (0x1000CBF3). Change: that stage off.
  - T: the alpha test (a function other than NEVER or ALWAYS): Wine compares the float alpha with ALPHAREF / 255
    (wined3d_private.h:3117, glsl_shader.c:7529 of Wine 9.0), ours floor(alpha x 255) as Microsoft's reference device
    (above); they differ where alpha x 255 falls between two integers. Counted against Wine only. Change: the alpha
    test off.
  The game was seen using none of A and C in its traces (a trace's state at a draw is partial, so this is "not seen",
  not "never"). It does create cube textures and set linear filtering, and it sets NORMALIZENORMALS off, so S and L
  may show in the game.
- Not established and not tested apart: what the third component of a 2D coordinate set reads as when a cube texture
  is addressed with it (ours 1, by the PROVISIONAL rule in d8_exec.cpp; Wine 0). The rechecks avoid it.
- What was wrong with the randomized test itself (found 14 Oct, fixed):
  - the scope lists at most 24 values a state and ends a longer list with a "..." entry; the generator drew it as a
    value, and t8 skipped the line (sscanf). Those states were then left at their starting value.
  - each case started from reset_state, which sets only some states: the others a case set were left for the cases
    after it, so a case's text was not all of its state (and the classes, read from the text, could be wrong). t8 now
    reads each state a case sets before setting it and puts it back before the next case. The rest of the device's
    state is the same for every case.
- What the randomized test fixed (14 Oct):
  - MIPFILTER NONE and MAXMIPLEVEL were sent as the sampler's LOD clamp, and a clamp to one level can decide
    magnification against minification (on Mesa a minified texture was drawn with MAGFILTER). The reference applies
    MAGFILTER and MINFILTER when the texture is magnified or minified "respectively" and MIPFILTER NONE "disables
    mipmapping" (Direct3D 9, D3DTEXTUREFILTERTYPE). Now the executor binds a view of only the levels the stage may
    read and leaves the LOD range open.
  - Pre-transformed vertices with generated coordinates read the constant (0,0,0,1), which had no source; now the
    set as given (class A).
- `threads` and `churn` (above) are checked against the executor's own invariants too: an object created twice or
  destroyed unknown is logged as a stream error. Object ids were taken from a counter that was not atomic: with two
  threads creating objects, the log of each of two runs had stream errors (5 and 2 lines); the counter is atomic now and the test
  fails on any stream error.
- `where` prints the d3d8.dll t8 loaded (path and file version) and the adapter, for runs on other machines.
- On the owner's machine (`owner_run/`: run_t8.bat, README.md): where, basic, defaults, scenes and random (seeds 1 to
  3) through the game's dgVoodoo d3d8.dll, Windows' own and ours in direct and remote mode, each in a folder of its
  own; `owner_run/owner_compare.py` compares each with ours by the reference criterion and names, per random case
  that differs, its classes. dgVoodoo does not run under Wine here (packed; it faults at the same place on Wine's
  D3D11 and on DXVK), so this is the only way to see what it draws.
- Elsewhere: kit/xikit/test (`realdev_ours`) hooks this device with the kit as the game's device is hooked and gets
  its frame times; kit/fcap/test captures a frame through it and replays that through Wine's d3d8 to the same picture.

Not tested here:
- `d8x_arm64.exe` itself: this container runs no ARM64 Windows code.
- The fallback when the executor process starts but cannot make its device or swap chain (the fallback when it
  cannot be started is tested by hand: no d8x.exe beside the DLL gives `thread`).
- What is established for the ARM64 path comes from the owner's machine. Phase 0's xproc probe (docs/HISTORY.md,
  6 Oct) saw a child's swap chain present into the parent's window, with an x86 parent and an ARM64 child and the
  reverse. The probe's swap chain is the executor's kind (windowed, DISCARD, one buffer) but R8G8B8A8, not
  B8G8R8A8. The raw probe logs are not in the repository; HISTORY.md records the result.
- Speed in any mode: Wine on a software renderer says nothing about it.
