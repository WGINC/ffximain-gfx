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

`t8.exe` makes the same D3D8 calls through Wine's own d3d8 and through ours, ours in each of the three modes (`remote`
with the x86 `d8x.exe`: Wine here runs x86 only):
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

  On 14 Oct all 20 match Wine's d3d8, and `thread` and `remote` draw exactly what `direct` draws (every pixel). The worst channel difference is 1 of 255 (lights_8), and no pixel is beyond
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
  the same on both (0 pixels in every case of seeds 1 to 5, 14 Oct: 1,000 cases):
  - A: pre-transformed vertices with a stage generating coordinates. Wine 9.0 generates them from the screen
    position; ours reads the set as given. Microsoft's text: "Direct3D does not modify transformed and lit vertices"
    (Direct3D 9, "Texture Coordinate Transformations"; the same in the Windows CE .NET 4.2 Direct3D text). It names
    transforms, not generation, so for generation this is a reading. Wine's own bug 37313 reads it the same way.
    Change: the stage stops generating.
  - C: a stage reading a coordinate set the vertex format does not have. Ours reads zeros; not in the reference.
    Change: the stage reads set 0 (or has no texture).
  - S: a cube texture filtered linearly. Wine's d3d8 filters each face apart (WINED3D_LEGACY_CUBEMAP_FILTERING,
    dlls/d3d8/d3d8_private.h); D3D11 always filters across faces. What Direct3D 8 hardware did is not established.
    Change: that stage point-filtered.
  - L: lighting with normals longer than 1, NORMALIZENORMALS off. Wine clamps each light's N.L at 1; the reference's
    equation, sum[Cd*Ld*(N.Ldir)*Atten*Spot] clamped after all lights ("Diffuse Lighting", Direct3D 9), does not;
    ours follows the equation. Change: NORMALIZENORMALS on.
  The game was seen using none of A and C in its traces (a trace's state at a draw is partial, so this is "not seen",
  not "never"). It does create cube textures and set linear filtering, and it sets NORMALIZENORMALS off, so S and L
  may show in the game.
- Not established and not tested apart: what the third component of a 2D coordinate set reads as when a cube texture
  is addressed with it (ours 1, by the PROVISIONAL rule in d8_exec.cpp; Wine 0). The rechecks avoid it.
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
