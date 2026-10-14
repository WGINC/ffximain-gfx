# d3d8on11: our own Direct3D 8 device on Direct3D 11

Design A of docs/D3D11.md: a `d3d8.dll` that takes dgVoodoo's place in the game's folder and implements the part of
Direct3D 8 the game uses (`data/d3d8_scope.json`) on Direct3D 11.

## How it is built

- `d8_main.cpp`: the module (`Direct3DCreate8`), the log (`d3d8on11.log` next to the DLL), formats, caps, IDirect3D8.
- `d8_device.cpp`: IDirect3DDevice8: the state the game sets and reads back, state blocks, resources, frames.
- `d8_res.cpp`: textures, cube textures, surfaces, vertex and index buffers.
- `d8_ff.cpp`: draws, through Direct3D 8's fixed-function pipeline as generated shaders (milestone 2).
- `gen_stubs.py` → `d8_gen.h` (generated at build): every method of every interface, logging "not implemented" once and
  failing. The device's classes override what they implement. A method the game calls that is not implemented is
  found in the log, not guessed. The vtable order is the D3D8 header's: a method missing from the generator leaves a
  class abstract and the build fails.

**Recording and execution are separate from the start.** The entry points keep the state and record what the GPU
must do (`Cmd`). One function, `Dev::flush` → `Dev::exec`, plays the commands into D3D11. Today it runs on the game's
thread at each sync point: a draw, Clear, Present, a copy, a read-back, a resource release. Phase 2 moves that
function to a second thread (A2) or another process (B) without changing the entry points.

**Locked data.** Textures and buffers keep a CPU copy that the game writes through Lock. Unlock records an upload of
what changed. A lockable render target or the back buffer read back at LockRect waits for the GPU, as the game's
occlusion readback does on any D3D8.

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

Each key becomes HLSL once, is compiled with `D3DCompile` from `d3dcompiler_47.dll` (loaded at run time), and is cached.
Blend, depth-stencil, rasterizer and sampler states are D3D11 state objects, cached by description. Fans become lists.
...UP draws go through a dynamic ring buffer. Phase 1 executes a draw when it is made, after what was recorded
before it.

Where each behaviour comes from (named in the code where it is used):
- The DirectX 8 reference: the stage ops and arguments, ALPHAARG1's fallback to DIFFUSE with no texture, lighting,
  material sources, texture coordinate generation, D3DTTFF_PROJECTED, flat shading from the first vertex, ZBIAS
  ordering.
- Khronos EXT_provoking_vertex, issue 7: in a triangle fan, flat shading takes the first non-spoke vertex (Direct3D 9's
  behaviour).
- Wine 9.0's wined3d, where the reference leaves the behaviour open. Windows' own behaviour in these cases is not
  established:
  - a stage whose op reads a texture that is not bound passes CURRENT on (`is_invalid_op`);
  - no texture transform with COUNT1 or on pre-transformed vertices (`compute_texture_matrix`);
  - the fog model: one coordinate per vertex (specular alpha, eye distance or depth) and the factor per pixel, with the
    start == end case (`get_fog_start_end`);
  - ZBIAS as −ZBIAS units and −ZBIAS times the slope (d3d8 `WINED3D_LEGACY_DEPTH_BIAS`);
  - a vertex without a diffuse colour is white, and without a specular colour is 0;
  - the half-pixel offset (`get_projection_matrix`).

Not implemented, each logged and the draw skipped:
- vertex shaders with code. The game holds some (data/shader_catalogue.json), but none was created in the owner's
  sessions: all 2,253 captured were declarations only (docs/D3D11.md section 2). The 281 handles in the traces are
  those declarations, which this device draws;
- pixel shaders;
- vertex blending (FVF XYZB*);
- tessellation;
- BUMPENVMAP and PREMODULATE (drawn as SELECTARG1).

None of these appears in the game's fixed-function draws as far as the traces show.

Known gaps, each logged when hit:
- Clear of rectangles, or of a viewport smaller than the target;
- full screen (renders windowed);
- CreateStateBlock of the pixel or vertex state only;
- palettes;
- writing to a locked render target;
- textures in a block format whose top level is not whole 4x4 blocks: D3D11 wants whole blocks; behaviour to be
  checked;
- caps: the device answers with dgVoodoo's own D3DCAPS8 from the owner's machine (probe8's caps.raw, 13 Oct;
  data/dgvoodoo_caps8.json), so the game takes the paths it takes there. They promise features not implemented here
  yet (vertex blending, the game's own shaders); those are logged when used.

## Tests (`test/test_d3d8on11.py`, Wine on Mesa's software renderer in a virtual X server)

`t8.exe` makes the same D3D8 calls through Wine's own d3d8 and through ours:
- `basic`: 22 checks (Clear and read back, a lockable render target, textures, buffers, state blocks, transforms,
  lights, shader handles, Reset, release). All pass on both.
- `scenes`: 20 scenes, each drawn on a 64x64 back buffer and read back whole, compared pixel by pixel. They cover:
  - the six FVF codes the game uses (data/d3d8_scope.json) and a declaration;
  - strips, lists, fans, indexed and ...UP draws, base vertex;
  - depth and culling, ZBIAS 8;
  - lighting with material and vertex colour sources, specular, eight point and spot lights;
  - texture stages with TFACTOR, MODULATE2X and BLENDTEXTUREALPHA;
  - the blend modes and BLENDOP values the game sets, alpha test;
  - vertex, table linear and table exp fog;
  - texture transforms, camera-space position, normal and reflection texgen into a cube texture;
  - DXT1 with mips, flat shading.

  On 14 Oct all 20 match Wine's d3d8. The worst channel difference is 3 of 255 (lighting), and no pixel is beyond
  the test's tolerance of 8. The tolerance and the 1% limit are the test's own criteria. With KEEP=1 each scene is
  written as a PNG (Wine | ours | difference).
- `defaults`: every render state and stage state of a fresh device, read on both and compared. On 14 Oct: 480
  states each, 30 differing.
  - 28 of the 30 are numbers above D3DRS_BLENDOP (171), which the D3D8 header does not define. Wine's d3d8 answers
    for them because its implementation is shared with D3D9.
  - D3DRS_DEBUGMONITORTOKEN (165): Wine 0xBAADCAFE, ours 0.
  - D3DRS_POINTSIZE_MAX (166): Wine 256.0, ours 64.0.
  - Which of these Windows' own D3D8 gives is not established. Neither matters to the game as far as the scope
    shows: it sets neither.

    KEEP=1 python3 test/test_d3d8on11.py
