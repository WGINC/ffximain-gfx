# Port plan: other platforms, then 64-bit

Written 17 Oct 2026, from the owner's conversation of 16-17 Oct (the DirectXYZ idea: legacy-Direct3D games made
64-bit and cross-platform, with this repository as its first game). This is a plan, not a decision: each step says
what is known, from where, and what is not known. `CLAUDE.md`'s rules apply to all of it.

Two tracks:
- **Track A, the renderer on other platforms:** the game stays 32-bit; `kit/d3d8on11` runs it on Windows, Linux and
  macOS.
- **Track B, 64-bit:** the game itself rebuilt from the reconstruction as a 64-bit program.

Track A comes first and does not depend on B. B depends on A's device (section 3, B4).

## 1. Where things stand (as of 16 Oct)

| Piece | State | Source |
|---|---|---|
| Graphics reconstruction | 777 functions in `tree/`, 774 PASS on fuzz and four live dumps | README.md, `verification/` |
| The whole DLL | 21,410 functions; the graphics set is 867 of them | `data/func_extents.json`, docs/RESIDUE.md |
| Our D3D8 device | `kit/d3d8on11`: milestones 1 and 2 done (all states, resources, state blocks, fixed-function drawing), checked against Wine's d3d8 and Microsoft's 8.1 runtime with its reference device | kit/d3d8on11/README.md |
| Executor modes | `direct`, `thread` (default), `remote` (separate process; `d8x.exe` x86 or `d8x_arm64.exe`) | kit/d3d8on11/README.md, `d8_link.cpp` |
| In game | The `thread` startup exit (a 32 MB texture upload larger than the ring) is fixed by banded uploads (15 Oct); the owner's in-game run of the fix is still to come | docs/HISTORY.md, 15 Oct |
| Picture against dgVoodoo | 8,689 px of a captured frame differ by more than 8 in a channel, 7,755 of them in the chat log and UI text; the cause is not found | docs/HISTORY.md, 15 Oct |

What phase 0 measured, which this plan relies on (docs/D3D11.md section 4, runs of 6 Oct):
- a native ARM64 executor submits the plaza-shaped frame 1.03 to 1.13 times faster than x86 under emulation;
- the large gains on every path come from fewer draws (28 to 64 times less submit time for the same triangles).

So the platform work below is for **portability**, not speed. Speed comes from the game-aware paths in
docs/D3D11.md section 3 (GPU skinning, asynchronous occlusion, batched text and UI, zone mesh), which apply on
every platform once the device runs there.

## 2. Track A: one target, Direct3D 11

**Decision proposed on 16 Oct:** the executor keeps speaking Direct3D 11 on every platform. No Vulkan or Metal
executor is written now.

| Platform | How D3D11 reaches the GPU |
|---|---|
| Windows (x86, ARM64) | Microsoft's D3D11, as now |
| Linux | Wine, with DXVK's d3d11 |
| macOS | Wine (or CrossOver), with DXMT; Apple's D3DMetal as a user option |

Why D3D11, from what is known:
- DXMT (github.com/3Shain/dxmt, LGPL-2.1) is a Metal translation layer for D3D10 and D3D11 under Wine on macOS.
  It does not implement D3D8 or D3D9; our device turns D3D8 into D3D11 before it.
- D3DMetal (Apple's Game Porting Toolkit) also takes D3D11. Its licence does not clearly permit redistribution: a
  request to Apple to say so is open (Apple Developer Forums thread 841547, no Apple reply in it). Until that is
  settled, nothing here ships it; a user may select their own copy.
- D3D11 is DXVK's main path. DXVK on Windows is not supported by its developers (its wiki, "Driver support"), which
  does not matter here: on Windows our executor uses Microsoft's D3D11.
- A Vulkan executor through MoltenVK is not needed for any platform above.

### A0. Finish the device on Windows (the owner's machine)

Nothing on another platform is judged before Windows is settled.
1. The owner's in-game run of the banded uploads (`thread` and `remote`).
2. The 8,689 px difference from dgVoodoo: find the cause (the UI text first, where most of it is), or name what is
   not known. ties.py has already ruled out texel-boundary ties as the main cause (34 of 8,689 marked).
3. The known gaps that the game reaches (kit/d3d8on11/README.md, "Known gaps"): full screen first. Each other gap is
   taken only when a log shows the game hitting it.

### A1. An x64 executor

`remote` mode runs the executor as its own process, so it need not be 32-bit even though the game is.
1. Add `dist/d8x_x64.exe` to `build.sh` (MinGW-w64 x86_64, same sources), reproducible and locked in
   `kit/BUILD.lock.json` like the other two.
2. Pin the stream for x64: `d8_proto.h`'s static_asserts are stated for the x86 and ARM64 builds; build them for x64
   and confirm the same sizes and offsets (XDraw, VSKey, PSKey and the rest), or the build fails.
3. Executor choice in `d8_link.cpp` (now: `d8x_arm64.exe` when IsWow64Process2 reports 0xAA64 and the file is there,
   else `d8x.exe`): add x64 for an AMD64 native machine. What IsWow64Process2 answers under Wine on macOS (Rosetta)
   and on Linux is not known; measure it, do not assume it.
4. Tests: `test_d3d8on11.py` runs `remote` with each executor it can run here.

### A2. Linux

1. Wine (32-bit game, wow64) with DXVK's d3d11.dll for the executor, in each mode: `thread` needs DXVK's 32-bit
   d3d11; `remote` with `d8x_x64.exe` needs its 64-bit one.
2. Run t8's scenes and the frame captures (`kit/fcap`) through it, compared pixel for pixel with the same build in
   `direct` mode on the owner's Windows machine. The reference is Windows, not Wine.
3. A real GPU, not Mesa's software renderer only: which machine is not decided (owner).

### A3. macOS

1. Wine or CrossOver with DXMT, `remote` mode with `d8x_x64.exe` under Rosetta. Whether DXMT supports a 32-bit
   process is not stated in its README; `remote` with an x64 executor avoids the question, `thread` would need it
   answered.
2. Not known, to be measured first:
   - whether the shared mapping and its ring work between two Wine processes on macOS;
   - whether a swap chain on another process's window works there. Phase 0 saw it work on Windows under Parallels;
     a shared texture there opened but the parent never saw the written colour (docs/D3D11.md section 4).
3. Same comparison as A2.2.
4. D3DMetal: the same tests with the user's own copy selected; nothing about it is relied on.

### A4. Shader compilation off Windows (a known risk)

The executor compiles each key's HLSL at run time with `D3DCompile` from `d3dcompiler_47.dll` (`d8_exec.cpp`).
Under Wine that is Wine's own d3dcompiler unless a native one is supplied, and it already differs: it does not
evaluate a float2 `?:` per component (measured 14 Oct, the comment in `d8_exec.cpp`). Options, for the owner:
- generated HLSL that avoids each construct found to differ, each named in the code (as now);
- compiled shaders cached by key as DXBC, produced on Windows with Microsoft's compiler and shipped with the kit.
  The number of keys the game reaches is not known; the frame captures can count them.

### A5. What A delivers

The game, still 32-bit, running through our device on Windows, Linux and macOS, each checked against the Windows
reference picture; the game-aware speed paths then work on all three.

## 3. Track B: 64-bit

**The constraint:** 32-bit and 64-bit code cannot share a Windows process, so the game cannot become 64-bit one
function at a time the way xikit swaps functions now. The whole program changes at once. The stages below keep
each step checkable by the gate.

### B1. Reconstruct the whole DLL

- 21,410 functions (`data/func_extents.json`) against 867 graphics functions now. Everything outside graphics
  (game logic, networking, files, sound, input, UI) needs the same pipeline: export, translate, gate.
- Order by subsystem, so each one can be swapped in and run in game through xikit before the next.
- The pipeline's rules do not change. What it cannot express today (the 11 constructs in docs/RESIDUE.md:
  `__usercall`/`__userpurge` callees, `__asm`, the x87 status word) will recur across the DLL; how many is not known
  until the whole DLL is exported.
- The live gate's dumps cover what the game was doing when they were taken. Gate coverage outside graphics needs
  dumps of those subsystems at work; which scenes those are is the owner's to choose.

### B2. A whole 32-bit build from source

The milestone that everything after depends on: an FFXiMain.dll built entirely from the reconstruction, loaded by
pol.exe in place of the original, running a session with no original function left.
- The 48 statically linked D3DX 8.1 functions: linked as Microsoft's (as the original does) at this stage.
- pol.exe and the other PlayOnline modules are outside this repository's analysis. What they hold, and what the
  game calls in them, is not established.

### B3. 64-bit with the 32-bit memory model

Build the B2 source as 64-bit, with the game's memory kept as a 4 GB block and every game pointer kept as a 32-bit
offset into it.
- The game's data layout does not change, so the gate's comparisons with the original still hold.
- Host objects cannot live in 32 bits: COM interface pointers (`CDx+0xC` holds the device; data/com_fields.json),
  HANDLEs, HWNDs, and anything else the OS or a runtime returns. Each becomes a 32-bit id in the game's memory
  and a host pointer in a table. Which fields hold such values is in the class model in part; the full list is not
  established.
- Globals at fixed addresses move into the block at their original offsets.

### B4. What 64-bit needs from outside the game

- **Direct3D 8:** Microsoft ships no 64-bit d3d8.dll. Our device, built 64-bit and in process, is the only path; A0
  is a prerequisite.
- **D3DX 8.1:** no 64-bit version exists. The 48 functions are rewritten and gated against Microsoft's 32-bit ones.
- **x87:** 64-bit code computes floats in SSE, so results can differ from the original's x87 code (10 of the 13
  current FAILs are x87 comparisons). Per function: emulate x87 exactly, or accept a stated tolerance. Which is the
  owner's decision; no tolerance is applied without it.
- **The C runtime and Windows imports:** the 64-bit build links its own; every import the original makes is listed
  in its import table and is checked call by call.
- **pol.exe:** a 64-bit FFXiMain.dll needs a 64-bit host. Replacing the PlayOnline host includes its login path.
  Whether that is done at all is the owner's decision (section 4).
- **Windower and its addons** hook the 32-bit client. A 64-bit client breaks them. To be raised with the Fenestra
  developers before B3 starts.

### B5. Native 64-bit pointers

Convert the game's own pointers to native ones, type by type, driven by the type and naming work
(docs/DEARTIFACT.md).
- The gate then compares two different layouts and needs a comparator that maps each struct between them.
- Structs whose layout is fixed by the DAT files or the network packets stay explicitly 32-bit, field by field.
  Which structs those are is established from where each is read or sent, not from its name.

### B6. What B delivers

A 64-bit Windows build. After B5, the same source built for ARM64 natively, and for macOS and Linux with the
Windows imports shimmed (SDL3 for windows, input and audio; the device for graphics). That last step is not
planned here.

## 4. The owner's decisions

1. Publishing: this repository stays private (README.md). The public research stays in DancingMad, whose README
   says MIT while its LICENSE file is GPL-3.0; one of the two is to be corrected.
2. Whether to begin B1, the whole DLL, once A0 is done.
3. The x87 policy (B4).
4. Whether the PlayOnline host is replaced (B4), and how the game's terms of service bear on any of B.
5. The Linux GPU machine (A2.3), and whether D3DMetal is tested at all (A3.4).
6. The shader option (A4).

## 5. The next tasks, in order

1. A0.1: package the banded-upload kit for the owner's in-game run.
2. A0.2: localise the UI text difference from dgVoodoo on capture xikit_fcap_3956_0.
3. A1: the x64 executor, its static_asserts, its selection, its tests.
4. A2.1-A2.2 under Wine and DXVK here, with the Windows run as the reference.
5. A4: count the shader keys the captures reach; report, do not choose.

Each task ends with its HISTORY.md entry: what was measured, from where, and what is still not known.
