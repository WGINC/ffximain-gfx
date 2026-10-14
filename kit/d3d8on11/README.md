# d3d8on11: our own Direct3D 8 device on Direct3D 11

Design A of docs/D3D11.md: a `d3d8.dll` that takes dgVoodoo's place in the game's folder and implements the part of
Direct3D 8 the game uses (`data/d3d8_scope.json`) on Direct3D 11.

## How it is built

- `d8_main.cpp`: the module (`Direct3DCreate8`), the log (`d3d8on11.log` next to the DLL), formats, caps, IDirect3D8.
- `d8_device.cpp`: IDirect3DDevice8: the state the game sets and reads back, state blocks, resources, frames.
- `d8_res.cpp`: textures, cube textures, surfaces, vertex and index buffers.
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

## Milestone 1 (this): everything but drawing

Done and tested:
- device creation with its swap chain on the game's window (windowed), automatic depth buffer, Reset;
- textures (BC1/BC2/BC3, 32-bit, 16-bit, A8), cube textures, render targets, depth surfaces, image surfaces, vertex
  and index buffers, with Lock/Unlock uploads;
- Clear of the whole target, Present, SetRenderTarget (which resets the viewport, as D3D8 does), CopyRects (GPU to
  GPU, and GPU to an image surface), read-back through LockRect;
- every state the game sets and reads back: render states, stage states, transforms, viewport, material, lights,
  textures, streams, indices, vertex shader handles;
- state blocks recorded with Begin/End, applied, captured, deleted, and CreateStateBlock(ALL).

Draws are recorded and counted but **not executed**. Drawing needs the fixed-function pipeline as shaders: milestone 2.

Known gaps, each logged when hit:
- Clear of rectangles, or of a viewport smaller than the target;
- full screen (renders windowed);
- CreateStateBlock of the pixel or vertex state only;
- palettes;
- writing to a locked render target;
- textures in a block format whose top level is not whole 4x4 blocks: D3D11 wants whole blocks; behaviour to be
  checked;
- caps: provisional. The game chooses paths by them, so they should answer as dgVoodoo does on the owner's machine.
  That needs a dump of dgVoodoo's caps, which probe8 does not make yet.

## Tests (`test/test_d3d8on11.py`, Wine on Mesa's software renderer in a virtual X server)

`t8.exe` makes the same D3D8 calls through Wine's own d3d8 and through ours:
- `basic`: 22 checks (Clear and read back, a lockable render target, textures, buffers, state blocks, transforms,
  lights, shader handles, Reset, release). All pass on both.
- `defaults`: every render state and stage state of a fresh device, read on both and compared. On 14 Oct: 480
  states each, 30 differing.
  - 28 of the 30 are numbers above D3DRS_BLENDOP (171), which the D3D8 header does not define. Wine's d3d8 answers
    for them because its implementation is shared with D3D9.
  - D3DRS_DEBUGMONITORTOKEN (165): Wine 0xBAADCAFE, ours 0.
  - D3DRS_POINTSIZE_MAX (166): Wine 256.0, ours 64.0.
  - Which of these Windows' own D3D8 gives is not established. Neither matters to the game as far as the scope
    shows: it sets neither.

    KEEP=1 python3 test/test_d3d8on11.py
