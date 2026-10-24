# In-game kit

Tools that run inside the game process. Each one checks first that FFXiMain.dll is the analysed build
(PE TimeDateStamp 0x6A995428, `data/binary.json`), and does nothing if it isn't.

## d3dprof: a per-frame profile of the Direct3D 8 calls

Build it 32-bit, like the game:
- Visual Studio: `cmake -B build -A Win32` then `cmake --build build --config Release`
  (`build\Release\d3dprof.dll`). The static runtime is used, so no redistributable is needed.
- MinGW: `build.sh`.

The thunks are x86 inline assembly in both forms (MSVC naked functions, GCC top-level asm). Checks:
- the MinGW build is run in Unicorn by `test_thunks.py`;
- the MSVC form is compiled with clang in MSVC mode (`--target=i686-pc-windows-msvc -fasm-blocks`), and
  the thunks come out as the same instructions.

`d3dprof/d3dprof.dll` records every call on the
device. It finds the device through `[[g_pCDx]+0xC]` (0x1045666C in the analysed image, rebased to where the
game's DLL is loaded) and gives that one object a copy of its vtable that counts each call. The driver's
own vtable and the game's code are not written.

Every 600 frames it rewrites `d3dprof_<pid>.csv` next to the DLL:

- a header line: frame time (avg/min/max ms, Present to Present), and per frame the calls, draws,
  primitives, and *redundant* state sets: SetRenderState, SetTextureStageState, SetTexture,
  SetVertexShader, SetStreamSource or SetIndices with the value that state already has. State set inside
  a state block recording is not tracked, and ApplyStateBlock and Reset clear what is known;
- one row per (method, call site): calls, redundant sets and primitives per frame. A call site is the
  return address as the analysed build's address, so it matches `data/render_map.json`. Calls from
  outside FFXiMain.dll are marked `other module`.

It is a plain DLL injected into pol.exe, the way the owner's speedfix_hook is (System Informer "Inject
DLL" or any injector). DllMain starts a thread. The thread waits, with no time limit, for FFXiMain.dll,
then for the device. It works the same on x86 Windows and on Windows on ARM, where the game runs under
x86 emulation.

With speedfix_hook: speedfix_hook patches the slots of the driver's vtable, and d3dprof gives the device a
copy of whichever vtable it finds. Either order chains: a call goes through both hooks. When speedfix
caches or merges calls, d3dprof counts them on its side of speedfix:
- injected after speedfix, d3dprof sits in front of it and counts what the game asks for;
- injected before, it counts what reaches the driver.

`test_thunks.py` runs the thunks in Unicorn: a call through the copy reaches the original
method with the caller's stack unchanged, returns with the method's own pop, and is counted.

Time spent inside each D3D call: off by default; `call_time=1` (in xikit.ini with `profile=1`, or `[d3dprof]` in a
`d3dprof.ini` next to the standalone DLL) adds `us_per_call` and `us_per_frame` per call site (docs/KIT.md, "Where the
time goes"). Calls made on other threads are counted as `other_thread_calls`, not profiled.

Kernel calls: off by default; `kernel_calls=1` (or `2`: every win32u stub) counts the system calls the process makes
through win32u.dll's x86 stubs and charges D3DKMT Lock2/Unlock2/WaitForSynchronizationObjectFromCpu/Render to the
device call or buffer Lock/Unlock they happen in (docs/KIT.md).

## speedfix: the performance fixes, as a mod of the kit

`speedfix/` is speedfix_hook rewritten as an xikit mod: state cache, UP batching, occlusion stagger and skips, frame
pacing, texture compression, the SSE tier. Build with `speedfix/build.sh`, add `mods=speedfix.dll`. What changed,
the settings and the tests: `speedfix/README.md`.

## xikit: run the reconstructed graphics code in the game

`xikit.dll` swaps the game's functions for their reconstructions (`gfx_recon_ingame.dll`, the verified
tree built with every game address rebased to where FFXiMain.dll is loaded), one at a time or all 774 in the
manifest (620 by default: the other 154 are flagged untested), and
switches back at any frame. The design, the rules it writes patches by, and the test results are in
`docs/KIT.md`.

Build (MinGW, needs `$FFXI_DLL` for the manifest): `xikit/build.sh` writes `xikit/dist/`:
`xikit.dll`, `gfx_recon_ingame.dll`, `xikit_manifest.txt`, `xikit.ini`. `xikit.dll` alone also builds with
Visual Studio (`xikit/CMakeLists.txt`, `cmake -B build -A Win32`); the other three still come from build.sh.

Use: put the four files in one folder and inject `xikit.dll` into pol.exe like d3dprof. It starts with
`swap=0` (it hooks and logs, nothing more). Set `swap=1` in `xikit.ini` while the game runs; it is re-read
every 60 frames. Ctrl+Alt+F9 switches all swaps off and on. `xikit.log` says what was swapped and refused,
`xikit_calls_<pid>.csv` which swapped functions the game called. A function whose reconstruction raises an
exception is put back and listed in `xikit_quarantine.txt`, and left out at the next start. `profile=1`
runs d3dprof inside it.

### Recording fields

`[watch]` in `xikit.ini` records values from the game's memory to `xikit_watch.csv` (in the start's log folder),
for lining up what a value does with what you did. `enabled=0` (default) costs the frame nothing.

```
[watch]
enabled=1
every=1                       ; look every N frames
w1=NAME BASE CHAIN TYPE       ; up to w64
```

`BASE` is an analysed-build address (`0x10480AF0`, mapped to the running game like every address in the kit), or
`BASE[i]`: slot `i` (decimal) of an array of 4-byte pointers at `BASE`, or `BASE[@ADDR]`: the slot whose index is
the signed 16-bit word at the analysed address `ADDR`, read each time (the player's own entity: `0x10480AF0[@0x10485F7A]`,
see docs/MEASURE.md). `CHAIN` is comma-separated offsets:
`p = BASE` (with `[i]`: `p = *(uint32_t*)(BASE + 4*i)`); for each offset but the last, `p = *(uint32_t*)(p + off)`;
the field is at `p` + the last offset. `-` means the field is at `BASE` itself (not defined with `[i]`, refused).
`TYPE` is `u8 u16 u32 i32 f32 ptr`. Every read is checked with `VirtualQuery` first; an unreadable value is `?`.

The file has `#watch NAME BASE CHAIN TYPE` lines and a `frame,ms,NAME,...` header (written when recording starts
or the list changes), then `frame,ms,NAME=value,...` lines, one every `every` frames and only when a value
changed (the first look always). `ms` is `GetTickCount64`. Ctrl+Alt+F7 writes `frame,ms,#MARK n` (n counts up)
and logs it, to mark when an action begins.

Each build writes `dist/VERSION.txt`: the source commit, the date, the tree's gate result and every file's
SHA-256. `xikit.log` repeats its first line, so a log says which kit wrote it.

`xisamp/` is the native sampler (`sample=2` in xikit.ini; `xisamp/build.sh`, copied into `xikit/dist/` by its
build): a 64-bit process that samples the render thread's x86 and native (ARM64) positions together;
`xisamp/read.py` reads its `xikit_native_<pid>.csv`, `xisamp/test_xisamp.py` tests it under Wine (docs/KIT.md).

Test: `xikit/test/test_xikit.py` (Wine and `numpy`, the real FFXiMain.dll; twelve runs, docs/KIT.md, Tests).
Is the kit current? `pipeline/tool_drift.py` rebuilds it and compares with `kit/BUILD.lock.json`.
