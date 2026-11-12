# Phase 0 of the D3D11 renderer (docs/D3D11.md, section 4)

Measurements on the owner's machine that choose between the designs of docs/D3D11.md: our own Direct3D 8 device on
D3D11 in the game's emulated x86 process (A), the same with a second thread (A2), or a native ARM64 renderer process
(B). Nothing here touches the game.

## Run (owner)

1. Close the game.
2. Copy everything in `dist/` (in the kit: the `d3d11_phase0` folder) into the PlayOnlineViewer folder, where the
   game's `d3d8.dll` (dgVoodoo's) is. That makes `probe8` measure Direct3D 8 through the same `d3d8.dll` and
   `dgVoodoo.conf` the game uses.
3. Run `run_phase0.bat`. It takes about 5 minutes. Windows open and close by themselves; leave them alone.
4. Send the `phase0_<date>_<time>` folder it makes.

## What it measures

| Program | What |
|---|---|
| `probe11_x86.exe caps`, `probe11_arm64.exe caps` | The adapter, the D3D11 feature level, support for the formats the port needs (BC1/BC2 for the game's DXT1/DXT3, the 16-bit formats, D24S8), driver threading, occlusion queries, and whether d3dcompiler_47.dll is there. The loaded d3d11.dll, dxgi.dll and compiler are logged with their paths |
| `probe11_* bench` | ns per call for the calls a D3D8-on-D3D11 device makes (blend state, texture bind, vertex-buffer bind, Map of a constant buffer, a small indexed draw, a ring-buffer draw). Then a frame shaped like Port Jeuno's plaza: 1,600 character draws and 900 UI draws as D3D8 issues them, and the same triangles in 33 draws. Submit time and time including Present and a wait for the GPU, medians of 220 frames |
| `probe8_x86.exe bench` | The same calls and frames through Direct3D 8, through the `d3d8.dll` next to it (path and version logged). D3D8 has no event query, so its frame time includes Present but no GPU wait. Before the bench it checks how a recorded state block behaves through that `d3d8.dll` (`stateblock.record`, `stateblock.apply`: PASS or FAIL; `stateblock.after Reset`: as found); speedfix's `block_cache` rests on it |
| `probe11_* xproc` | A window in one process, and the other architecture's build started on it: (1) its swap chain on that window, which the parent checks by reading the window's pixels; (2) a texture the parent shares, which the child writes and the parent draws. Both directions, x86 parent with an ARM64 child and the reverse |

`phase0_report.py FOLDER` sets the results side by side: per call, D3D11 from x86 and from ARM64 (their ratio is what
emulation costs a call) and D3D8 through dgVoodoo (its ratio to D3D11 is what that layer adds); per frame, as D3D8
issues it against merged; and the two cross-process verdicts.

**Reading the per-call numbers:** an implementation may record a state call and do the work at the next draw, so a
setter can look nearly free while the draw after it carries the cost. The frame numbers include both, and decide.

## probe12 (12 Nov): D3D12, and whether more threads shorten a frame

The owner asked whether the executor's one queue could be split, with D3D12 or otherwise. `probe12_x86.exe` and
`probe12_arm64.exe OUTDIR` (`run_probe12.bat`; anywhere, the game closed; about 2 minutes, nothing opens; send the
`probe12_<date>` folder):

- D3D12 on adapter 0: is a device made (D3D12CreateDevice), its highest feature level and binding tiers, and does
  a frame of command lists run to its fence.
- One frame of 2,000 small draws (one four-constant change and one triangle each into a 256x256 target), timed to
  the GPU's finish, 40 frames with the first 8 left out, medians of recording, submitting, the GPU wait and the total:
  `d3d12_T` (T command lists recorded on T threads, one ExecuteCommandLists), `d3d11_immediate` (one thread, as the
  executor does now), `d3d11_deferred_T` (T deferred contexts on T threads, then executed on the immediate context);
  T = 1, 2, 4, 8. Plus the D3D11 driver's threading capabilities.
- What it settles: whether, on this machine, a frame of many draws gets shorter with more threads, under D3D12 or
  D3D11's deferred contexts, and from which architecture. It is a model of the executor's work (no textures, no
  state changes), not the game's frame.
- D3D12 runs first; a `starting` line before each timed test names the test if the program stops there.
- Under Wine (test_probe.py) the deferred contexts are recorded one thread at a time (`PROBE12_SERIAL=1`, logged):
  two threads recording at once, each on its own deferred context and buffer (which D3D11 allows), crashed Wine's
  d3d11 in 2 of 9 runs, writing into the memory its Map returned; 0 of 10 runs crashed one at a time. Which line of
  Wine is at fault is not established.

## Built and tested here

- `build.sh`: probe11 for x86 and ARM64 with zig cc (PyPI `ziglang`, pinned to 0.16.0; its mingw-w64 libraries
  cover ARM64 Windows, and this container cannot fetch llvm-mingw from GitHub), and probe8 with MinGW-w64.
  Reproducible: kit/BUILD.lock.json holds the hashes and pipeline/tool_drift.py rebuilds and compares them.
- probe12: x86 with MinGW-w64, ARM64 with zig's clang (`build.sh`).
- `test_probe.py`: the x86 builds under Wine on Mesa's software renderer (xvfb-run): every result line is written,
  the D3D11 device is made, the child of xproc runs on the parent's window and logs into the run folder, and the
  report reads the run (plus its selftest); the ARM64 build is checked to be an ARM64 image.
- Not testable here, so not tested: the ARM64 build running, any number for the owner's machine, and the
  cross-process verdicts (Wine's PrintWindow does not see OpenGL output, and Wine has no shared D3D11 textures).
