# The in-game kit (xikit)

Runs the reconstructed graphics code inside the real game, one function at a time or all of them, with a
switch to go back to the original at any frame. Built on what is verified (`tree/`, the gate verdicts) and on
the crash-safety rules the owner's `speedfix_hook` already follows.

## What it does

| Feature | What | Default |
|---|---|---|
| Build check | Does nothing unless FFXiMain.dll is the analysed build (PE TimeDateStamp 0x6A995428) and the reconstruction DLL is the one the manifest was made from | always |
| Swap | Writes a `jmp` at a verified function's entry to its reconstruction (`gfx_recon_ingame.dll`, every game address rebased through `xi_rel`) | off; `swap=1` |
| Selection | All verified functions, or a list (address or name); minus an exclude list (`exclude=`; shipped with `Gfx_DestroyD3DHolder`, which faults when swapped), the quarantine file, the functions flagged SEH or speedfix-site in the manifest (`include_flagged=1` swaps them too), and the functions `speedfix_hook` or the speedfix mod (kit/speedfix) patches inside when either is loaded (`respect_speedfix=0` keeps them) | all verified |
| Live toggle | Changing the ini (re-read every 60 frames) or the hotkey (Ctrl+Alt+F9: all swaps off/on) patches or restores at the next frame boundary | on |
| Call counts | A per-function stub counts how often each swapped function runs (`xikit_calls_<pid>.csv`): which reconstructed code the game actually used | on; `count_calls=0` turns it off |
| Crash capture | An exception whose address is in reconstructed code (or a stub) is logged with the function, registers and the top of the stack, and the function is added to `xikit_quarantine.txt`, so the next start leaves it out. An exception in game code (or another module) called from reconstructed code: the stack is searched for return addresses into reconstructions (each must follow a `call`), the chain is logged, and the innermost is quarantined as `suspected` | always |
| Untested functions | Functions whose gate trials did not run every instruction of the original (manifest flag 4) are left out | on; `include_untested=1` swaps them too |
| Profiler | d3dprof (per call site, per frame) in the same process, chained | off; `profile=1` |
| Log | `xikit.log` next to the DLL: what was checked, swapped, refused and why; the kit's build stamp (`VERSION.txt`) on the second line | always; `log_each=1` lists every function patched and restored, not only totals |

## Which functions may be swapped (decided at build time, from the binary: `kit/xikit/make_manifest.py`)

A function goes into the manifest only if all of these hold:
- **verified**: PASS on fuzz and all four dumps, in the clean tree as built (`verification/gate_clean_final.jsonl`);
- **its export is found**: the in-game DLL exports it under the symbol the gate called (`verification/cases_clean_final.json`);
- **its entry can hold the jump**: the first 5 bytes are inside the function;
- **nothing jumps into those 5 bytes**: no branch or call anywhere in the DLL's code (every function in
  `data/func_extents.json`, decoded) targets entry+1 .. entry+4. Such a jump would land inside the new `jmp`.

The manifest also carries flags that leave a function out by default, each with its reason:
- **SEH**: the original installs an exception frame (`mov eax, fs:[0]` with a handler pushed). The
  reconstruction has no `__try`, so on an exception it does not behave the same;
- **speedfix site**: `speedfix_hook` patches inside it (0x1006C4D7, 0x100CC688, 0x100CD011, 0x1006CDDC). Only
  left out when speedfix_hook.dll is loaded in the process.
- **untested** (flag 4): the compared trials of the fuzz and live gates together did not run every instruction
  of the original (`verification/coverage_clean.json`, `pipeline/combine_coverage.py`). A PASS says nothing
  about code no trial reached. The first in-game crash (5 Oct) was in such code: `Frame_Render` passed every
  gate with 53 of its 1424 instructions run, and its two calls of `CXiTimer::GetFps` lost the timer in ECX
  (pol.exe, c0000005 at FFXiMain.dll+0x19230). Also set when the PASS rests on fewer than 10
  trials in which the original returned, over all gates together (thin evidence: `sub_10016320` passed on one
  trial per gate). Left out unless `include_untested=1`.
- **register input** (flag 8): the original reads EDX at its entry, or ECX while the tree declares it neither
  `__thiscall` nor `__fastcall` (`data/edx_use.json`, `data/ecx_use.json`). Swapping is unaffected; a mod may not
  hook it (docs/SDK.md). One function: `Obj_104E1D4C::Cursor_Draw`.
- **T=** (the trampoline column, for mods): see docs/SDK.md, "What `previous` is".

## How a patch is written (the same rules as speedfix_hook, plus the classic hot-patch ones)

1. **On the game's own render thread, at a frame boundary.** Patches are written inside the kit's Present
   hook, so the thread that runs graphics code is not inside any of it.
2. **Every other thread is suspended** while bytes are written (Toolhelp snapshot). If any thread's EIP is in
   the 5 bytes being written, all threads are resumed and the write retried next frame.
3. **The live bytes are checked first.** The expected bytes come from the analysed build, with its
   relocations applied for where the DLL is loaded. A mismatch means someone else changed them (another
   hook, a different build): the function is refused and the reason logged. A restore checks that the
   kit's own `jmp` is still there.
4. **One page at a time**: `VirtualProtect` of the 5 bytes only (`PAGE_EXECUTE_READWRITE`, then the old
   protection back), and `FlushInstructionCache`.
5. **Nothing is ever unloaded.** The kit and the reconstruction DLL pin themselves. A restore only redirects
   future calls, so a reconstructed function still on some thread's stack returns into code that is still
   there.

## What the build gives (`kit/xikit/build.sh`, this commit's tree)

- **Manifest**: 774 functions listed (every function PASS over the merged gates), none refused (each has its
  export, room for the jump, and no jump into its first 5 bytes). Flags: 0 SEH, 3 speedfix sites, 154 untested
  or thin evidence (left out by default), 1 register input (also untested); 620 are swapped in by default.
  721 have a trampoline for mods (`T=`), and the reconstruction DLL's 792 code symbols are listed (`!code`) so a
  fault inside it is attributed to the function it is in.
- **The in-game DLL is the verified code**: `gfx_recon_ingame.dll` (the tree built with `XI_INGAME`, every
  game address `XI_GAME(ea) = ea + xi_rel`) is gated on fuzz itself with `xi_rel = 0`. Its verdicts are the clean
  tree's fuzz verdicts, function for function (re-run 6 Oct on the shipped DLL, `runs/ingame`): 772 PASS; 1 FAIL,
  `Post_DrawRotatingGhostLayers` (0x10038E10), which fails in the tree too and is not in the manifest; and 4 not
  compared on fuzz (0x10003890 and 0x100CB4E0 UNRESOLVED, which pass on the dumps; 0x1006B3B0 and 0x10016320
  INCONCLUSIVE everywhere, not listed).
  This check found the 5 Oct translation defects the clean build had passed by chance (`docs/HISTORY.md`).
- **No game address is left unrebased**: a scan of the in-game DLL's code finds 4,118 rebased adds and no
  absolute game address used directly; its data sections hold no game pointers.

## Tests (`kit/xikit/test/test_xikit.py`, Wine, the real FFXiMain.dll)

`host.exe` (`test/host.c`) maps FFXiMain.dll without running it (`DONT_RESOLVE_DLL_REFERENCES`), gives it a
fake CDx and device, loads `xikit.dll` from `dist/` and plays the render thread. It runs twelve times: base,
relocated and profile (below); `ab` (the A/B test and the log folder); `mod` and `modrel`, with a mod (`test/mod_test.cpp`) at the analysed base and
relocated (docs/SDK.md, Limits); `reload` (`mod_reload=1`); `capture` (the capture replayed through the gate, which
needs the gate's build of the tree, `tree/build/gfx_recon.dll`: the test builds it when missing); `facts`
(`facts=1`); and `realdev` and `realdev_ours` (14 Oct): a real Direct3D 8 device in the fake one's place, Wine's d3d8
and then kit/d3d8on11's, in a virtual X server. The kit must hook it, present 200 frames through it and write
`xikit_timing_<pid>.csv`. And `native` (18 Oct): `sample=2` in a Wine prefix with 64-bit Windows. The kit must start
`xisamp_x64.exe` on the render thread, and its file must show the host's GetColor loop and the Sleep after it, each
under the host's calls. `MODES="..."` runs only the runs named. The picture comparison needs `numpy`. Last full run: 6 Oct, Wine 9.0, 194 checks, all ok. The first three:

- **base**: the game DLL at 0x10000000;
- **relocated**: 0x10000000 reserved first, so the DLL is mapped elsewhere (0x009A0000 under Wine). Wine does
  not apply relocations for this load mode, so the host applies them as the game has them: the whole
  `.reloc` chain from the section's start (92,563 entries). The file's header directory points at only the
  last 87 blocks, which do not cover `.text`; the chain is the one `pipeline/dump_verify.py` checks against
  every relocated dword in the live dumps;
- **profile**: at the base, with `profile=1` from the start, so d3dprof is attached in front of the kit's
  vtable copy.

All three pass every check:
- the device is hooked through its own vtable copy; the original vtable is not written; Present still
  reaches the device's method;
- `swap=1` (with `include_untested=1`): all 774 entries jump to the reconstruction. Reconstructed `SetColor` (0x100A4830) writes
  `this+0x660`, `GetColor` reads it back, and `Gfx_IsGammaSupported` reads `g_pCDx` at the rebased address;
- the call count of `SetColor` is in the report;
- `swap=0` written to the ini: re-read, all original bytes back;
- `swap=1` with an exclude: the excluded function keeps its original bytes;
- the default leaves `Frame_Render` (flag 4) unswapped; `include_untested=1` swaps it;
- an access violation inside the reconstruction (a null `this`) is passed on to the game's own handlers,
  the function is restored at the next frame and written to `xikit_quarantine.txt`;
- an access violation in the game's own code called from a reconstruction (`CDx::PushRenderTargetRet1` on a
  null CDx calls the reconstructed `CDx_PushRenderTarget`, which calls the original
  `CDx_PushRenderTargetTexture`, which faults): the innermost reconstructed caller, `CDx_PushRenderTarget`, is
  restored at the next frame and listed as `suspected`; its caller stays swapped;
- profile mode only: Present still reaches the device through both hooks; d3dprof writes its report at 600
  frames, and the report has the host's Present calls as `other module`, 601 calls in 600 frames (a frame is
  Present to Present, so the first window holds one more Present than frames).

Wine maps images the Windows loader refuses, so `test_xikit.py` also checks the Windows layout rules on both DLLs
(sections contiguous, `SizeOfImage`, raw data in the file). The first build sent to the owner failed in the game
with LoadLibrary error 193 (`ERROR_BAD_EXE_FORMAT`): the pinned section bases of `tree/build.sh` leave gaps
between sections. `close_gaps.py` now extends each section's virtual size to the next one; only 6 header bytes
change, every section's contents and address stay the same, so the code is the code the gate ran.

Not tested here: the real game (a real device, the game's threads, speedfix_hook in the same process). The
first run in the game is with `swap=0`, then `swap=1` with `only=` a few functions, with the log read after each.

## Files

- `kit/xikit/xikit.c`: the DLL. MinGW (`build.sh`) or MSVC (`CMakeLists.txt`), 32-bit. ISO C11 (clang
  `-std=c11 -pedantic` reports nothing); it has not been compiled with MSVC itself here.
- `kit/xikit/make_manifest.py`: `xikit_manifest.txt`, one line per function.
- `kit/xikit/build.sh`: builds the in-game reconstruction DLL from `tree/`, the manifest, the kit and the
  default `xikit.ini` into `kit/xikit/dist/`. Not committed: the manifest holds bytes of the game's code.
- `kit/xikit/close_gaps.py`: closes the layout gaps in the in-game DLL so Windows loads it (build.sh runs it).
- `kit/xikit/test/`: `host.c` and `test_xikit.py`, above.
- `xikit.ini`: written next to the DLL with every key documented when it is missing (`DEFAULT_INI` in xikit.c).

### Pictures (`trace_pictures=K`)

The trace compares calls, not what they draw. With `trace_pictures=K` an alternating trace also saves the finished
picture of frames 8-2K to 8+2K. That is K swapped frames, each with both neighbours, about 8 MB a frame at
1920x1080, as `xikit_trace_<pid>_<n>_f<frame>_l<label>.pic`.
- **How a picture is read.** The back buffer is read at the start of Present into a system-memory surface, with
  CopyRects, the way the game reads its own occlusion target. The device's own methods are called through its
  original vtable, so the trace does not record them.
- **The comparison:**

      python3 kit/d3dprof/pictures.py DIR_WITH_THE_PICS [--out prefix]

  checks each swapped frame pixel by pixel against the range its two neighbours span (`--tol`, default 2), with
  the same test on an unswapped frame as the control. It reports the share of pixels outside and where. With
  `--out` it also writes a picture with those pixels in red.
- **Tested** (Wine, mode `profile`): a host back buffer with one smoothly animated band and one band that
  changes only while a function is swapped. Exactly the swapped band is found on both swapped frames, nothing in
  the animated band, and nothing in the control.

## Where the time goes (`xikit_timing_<pid>.csv`, `sample=1`)

- **Frame timing, always on.** Every `report_every` frames the kit appends one line to
  `xikit_timing_<pid>.csv`:
  - the frame time (Present to Present: average, median, 95th percentile, maximum);
  - the time inside the device's Present;
  - the kit's own time before it (its checks and swaps, and mods' frame callbacks);
  - the number of functions swapped.

  Across many log folders,

      python3 pipeline/run_ledger.py <folder> [<folder> ...] > ledger.md

  lists every run's settings, per-minute frames, timing rows, A/B phases and summaries, and readback counts,
  each cited by file and line. It also lists the executor A/B of any `d3d8on11.log`.
- **The sampling profiler, `sample=1`.** Read at the start. Every `sample_ms` (default 1) a thread of the kit
  suspends the render thread for an instant and records:
  - its EIP;
  - the return addresses into the game or the reconstruction on its stack. A return address is an address that
    follows a call instruction; the readable parts of both images are checked first, so the scan never faults.

  `xikit_samples_<pid>.csv` is rewritten every window. Then

      python3 kit/d3dprof/samples.py xikit_samples_<pid>.csv

  shows, as shares of the samples with their standard error:
  - **self:** where the thread was (game function, reconstructed function, or module);
  - **elsewhere:** time in Direct3D, the driver and Windows, by the game or reconstructed function that called
    there, with the call sites;
  - **inclusive:** time under each function. This is an upper estimate: a stale return address left in an
    older frame is counted too.

  Samples estimate time shares; they do not time each call.
- **The native sampler, `sample=2`** (kit/xisamp). Read at the start. `sample=1` runs inside the game: on an
  x86-on-ARM64 machine it sees only the emulated x86 state, and on the owner's machine 64-67% of its samples read
  EIP 0x00010002, which is in no module (task 34; what that value is has not been established). `sample=2`
  instead starts `xisamp_arm64.exe` (native ARM64) or `xisamp_x64.exe` (native x64) from the kit folder, chosen by
  `IsWow64Process2`'s native machine, with the first frame's thread. It is a 64-bit process of its own. Every
  `sample_ms` it suspends the thread and reads:
  - the x86 context (`Wow64GetThreadContext`) and 16 KB of the x86 stack;
  - the native context (`GetThreadContext`: ARM64 Pc, Lr, Fp, Sp; x64 Rip, Rsp) and up to 64 KB of the native
    stack.

  With the thread running again, it builds:
  - **the x86 chain:** a scan, as `sample=1`'s, of values in any x86 image that follow a call instruction;
  - **the native chain** (ARM64): the x29 frame-pointer chain, which the Windows ARM64 ABI requires.

  Each sample is one tuple: (x86 eip, x86 chain, native pc, native lr, native chain). Every `sample_window`
  seconds (default 30), `xikit_native_<pid>.csv` gets:
  - the window's local start and end, which line up with A/B phases;
  - its tuples with counts;
  - every image in the process, with its machine;
  - a label for each address: its image and RVA, and the nearest export at or below it. The export is a position
    only: a module's internal functions have no export.

      python3 kit/xisamp/read.py xikit_native_<pid>.csv [--from HH:MM:SS --to HH:MM:SS] [--focus 0x1002A034]

  shows, as shares of the samples:
  - the x86 module;
  - the native module;
  - the two together;
  - for x86 eips in no image: their values and their native pc and frames;
  - time outside the game, by call site (as `sample=1`'s elsewhere);
  - the system-call stub: the innermost x86 return address into win32u.dll or ntdll.dll, named by its export (the
    stub the x86 code called), paired with the native pc (a stub's samples should sit in one wow64 thunk), and by
    call site, counting only samples whose native pc is in wow64win.dll or ntdll.dll;
  - for each `--focus` call site: its samples split by x86 module!export, native module!export and stub.

  Game addresses are the analysed build's: xikit passes `rel` and the shift table.
  - **Tested** (`kit/xisamp/test_xisamp.py`, x64 build under 64-bit Wine, a test x86 process with a known chain):
    every busy sample's x86 chain begins with the three return addresses its disassembly gives;
    the waits in Windows are found under the right caller; exports label kernel32 addresses; `read.py` reads the
    file.
  - **Not tested here:** the ARM64 build. It builds, and the test checks it is an ARM64 image, but there is no ARM64
    Windows here. Its native frame chain is therefore untested; under Wine the native pc is the x86 eip.
- **Time inside each Direct3D call, `call_time=1`** (with `profile=1`; read at the start). On the render thread,
  d3dprof's thunk keeps the call's return address on a shadow stack and points it at a return stub, reading
  `QueryPerformanceCounter` last thing before the method runs; the stub reads it first thing after the method
  returns, adds the difference to the call site and jumps back to the caller with EAX and EDX as the method left
  them (integer code only; the x87 stack is not touched). `d3dprof_<pid>.csv` then has `us_per_call` and
  `us_per_frame` per (method, call site), and a header line with the counter's frequency and the cost of two counter
  reads back to back (measured at the start, not subtracted). What is timed includes the layers behind d3dprof
  (speedfix_hook, dgVoodoo, the driver). Calls nested deeper than 64 are counted, not timed; calls on other threads
  are not profiled. Standalone d3dprof: `d3dprof.ini` next to it, `[d3dprof]` `call_time=1`.
- **Tested** (Wine, mode `profile`): timing lines written; samples written; samples taken while the thread ran
  the reconstructed GetColor name it; with `call_time=1`, the report's time columns and Present's times.
  `test_thunks.py` (Unicorn): a timed call returns to its caller through the stub with the method's pop, EAX and EDX,
  and records exactly one counter interval at its site.

## What the game runs on (always, in xikit.log)

Once the device exists the kit logs:
- **The CPU as CPUID reports it:** the vendor string, the brand, and SSE through AVX. The game's own detection turns
  its SSE code on only for an Intel or Transmeta vendor; speedfix's `sse_skinning` works around that.
- **The machine:** the native machine (x86, x64 or ARM64; an x86 process on ARM64 Windows is emulated), and the
  Windows build.
- **Compatibility layers:** the kernel's own version (KUSER_SHARED_DATA, which no shim changes) beside the version
  the game is told, the `__COMPAT_LAYER` the process started with, and the layers the registry sets for this exe
  (AppCompatFlags\Layers, user and machine). The owner's game is told Windows 5.1, with no compatibility mode set on
  the PlayOnline exes; this line names where the layer comes from.
- **Every graphics module loaded** (d3d8, d3d9, d3d10, d3d11, d3d12, dxgi, the D3DxOn12 layers, dgVoodoo, Parallels'
  prl_ drivers, vendor drivers), each with its path, file version and PE machine type: which Direct3D 8
  implementation the game's calls go through, and what lies under it.

### Comparing Direct3D 8 paths (kit/d3dprof/compare_paths.py)

The cost of each call depends on the path under the game: a d3dprof profile (`profile=1 call_time=1`) of one path
and one of another, taken standing at the same spot, compare per method (`compare_paths.py A.csv B.csv --names A B`):
frame time, time inside Direct3D calls, calls a frame and the time per call. Frame time decides. Surface and buffer
locks are not device methods and are not in the per-call times (the readback's LockRect, the skinning buffers'
Lock); the frame time includes them.

### How many draws could be merged (kit/d3dprof/merge_potential.py)

A trace (`trace=1`) read for runs of consecutive draws under the same state hash with nothing between them that
changes what a draw sees (transforms, materials, lights, constants, a refilled buffer, another base index, scene
boundaries). Per frame and label: the draws of each method and the runs they form, which is what merging in place
would leave (an upper bound; `--selftest`).

## Where the kit writes (`log_folder=1`)

Every start gets a folder of its own, `xikit_logs\<date>_<time>_<pid>` next to `xikit.dll`, and everything the
kit writes goes there: `xikit.log`, d3dprof's profile and resource log, the vertex shaders, timing, samples, call
counts, traces and their pictures, captures, facts and A/B results. What the kit reads stays next to `xikit.dll`:
`xikit.ini`, the quarantine, the manifest, the mods, and the trigger files (`xikit_trace_now.txt`, the bisect
answers, `xikit_reload_now.txt`). `log_folder=0` writes next to `xikit.dll` as before. The setting is read at the start.

## Stutters (`spike_ms=50`)

Every frame slower than `spike_ms` (Present to Present; 0 = off) is written to `xikit_spikes_<pid>.csv` with its frame
number. With `profile=1` the resource log (`xikit_resources_<pid>.csv`) has the same frame numbers, the time of each
creation call and the module that asked for it, so a stutter can be matched with what was created in it.

## An A/B test with no keys (`ab=1`)

`ab=1` measures what the kit adds, the swaps and every mod (speedfix included), against the game without it, in one
session and one scene:
- `ab_after` seconds after `ab=1` is read (60 by default; at the start, or within 60 frames of saving the ini),
  phases of `ab_phase` seconds (30) run in the order A B B A, `ab_rounds` times (4: 8 phases, 4 minutes).
- A = everything the kit adds is off, as with F9: no swaps and no mod hooks. B = on.
- ABBA puts each label's phases around the same mean time, so a scene that drifts steadily affects both alike.
- The first `ab_settle` frames of a phase (60) are left out, and so are frames while a trace runs.
- Results go to `xikit_ab_<pid>_<n>.csv`: per phase, the frames used and the mean, median, p95 and maximum frame
  time (Present to Present). The summary compares the two labels: B - A in ms and %, and how much the phases of one
  label differ among themselves. A difference smaller than that spread is within the noise. `xikit.log` has the same
  lines.
- `ab=0` stops a run and switches everything back on. Setting it to 0 and back to 1 starts another run.
- Stand still while it runs.
- Not switched: what a mod did once at its start (speedfix's `cpu_tier` and `texture_compression`).
- One speedfix setting against the rest of speedfix: speedfix's own `ab_levers` (kit/speedfix/README.md, "The lever
  A/B"), with `ab=0` here.
- Tested: Wine, mode `ab`: 4 phases of 1 s; the swapped entries follow the phases; the folder, the CSV and the
  summary are checked.

## Capturing real calls for the gate (`capture=`)

The gate's trials ran 63% of the reconstruction's instructions; the hot paths far less (docs/GAPS.md). With
`capture=Name1,Name2` (manifest names or addresses), the kit records the next `capture_count` calls (default 1)
of each, from `capture_after` seconds after the start (default 0):
- **What a capture is.** At the entry, with every other thread suspended, it takes the registers, the stack
  above the entry (up to 256 KB), FFXiMain.dll's writable data, and every page reachable by following pointers
  from those, breadth first, up to `capture_pages` pages (default 20,000, 80 MB). For the call's own pointers
  (the registers and the first 8 arguments) it first takes the `capture_follow` pages after each (default 16): an
  object or command list longer than a page. Without that, SkinAndDrawV2's command list ran onto a page the
  budget-bound walk never reached, and the gate's replay looped on fuzzed bytes. It writes them as
  `xikit_capture_<pid>_<RVA>_<n>.xcap`. Which pages a call needs is not known beforehand: what pointers reach is
  a preparation of the inputs, and a page the capture lacks is filled as the gate fills any unknown page, on
  both sides alike.
- **The game is not changed.** The call then continues into the game's own code (a trampoline; the
  reconstruction where the entry has none). After the last capture the entry is restored.
- **On the analysis side:**

      python3 pipeline/xcap.py xikit_capture_....xcap verification/cases_clean_final.json tree/build/gfx_recon.dll OUT_DIR [--insn-limit N]

  runs the original and the reconstruction from that exact call (its `this` and arguments, on its memory) in
  the gate's live mode, with coverage, and prints the verdict and how many of the original's instructions ran.
  Each capture is a gate case on the game's real state.
- **Tested** (Wine, mode `capture`): GetColor and SetColor captured, the game's values returned, the entries
  restored, and both captures replayed through the gate (PASS, every instruction run, SetColor's argument read
  off the captured stack).

## Another build of the game (build translation)

Square Enix updates FFXiMain.dll. The kit, the reconstruction and the mods all name the game in the analysed build's
numbering (3 Sep 2026, TimeDateStamp 6A995428). On a build it has no manifest for, the kit logs `FFXiMain.dll is a
build the kit has no manifest for` and does nothing. A new build is added like this (game binaries stay out of the
repository):

1. `python3 pipeline/unpack_pol.py SHIPPED_FFXiMain.dll OUT_unpacked.dll`. The shipped DLL's code is compressed in
   its POL1 section and unpacked by its entry stub at load. This runs the stub in an emulator up to the first
   instruction in .text and writes the image as the stub leaves it.
2. `python3 pipeline/build_translate.py $FFXI_DLL OUT_unpacked.dll` writes `kit/xikit/builds/<stamp>.json` only when
   everything the kit relies on is settled (its docstring has the rules):
   - the code ranges that moved, from build_map's matches;
   - every manifest function proven the same code at its range's shift;
   - every address the reconstruction, speedfix and the SDK name either lies in an unchanged range (code), or is
     referenced by the same operand there (data).
   Otherwise it names each unsettled function or address.
3. `FFXI_DLL_<stamp>=OUT_unpacked.dll kit/xikit/build.sh` adds `dist/xikit_manifest_<stamp>.txt`
   (manifest_for_build.py): the same functions, with that build's entry bytes and relocations, and `!shift LO HI
   SHIFT` lines.

At start the kit reads the running game's TimeDateStamp and loads `xikit_manifest_<stamp>.txt` when there is one.
It writes its jumps at the translated addresses and passes the moved ranges to the reconstruction DLL and the mods
(`xi_shift`, beside `xi_rel`; `XI_GAME` applies them). It keeps reporting the analysed build's addresses everywhere:
logs, profiles, traces, samples, faults. Data addresses never move in a build it accepts, so for a constant address
`XI_GAME` folds to what it was; a code address costs a search of at most 8 ranges. A mod built before this exports
no `xi_shift` and is refused on a translated build; the SDK's `XIKIT_MOD_GLOBALS` defines it.

`facts=1` keeps the analysed build's bytes for its sites. A site whose bytes differ in the running build (a call into
code that moved) is refused, as on any mismatch.

**Build 6AC37846 (5 Oct 2026), checked 14 Oct:**
- The code moved in two places: +32 from 0x10232A90, +64 from 0x10309EC0.
- Two handler functions (0x10232970, 0x10232A30, reached through pointers) gained a check and return early on it.
  The other insertion is 32 bytes of int3 padding before `_a_atan2`.
- All 774 manifest functions are proven the same code (718 unmoved, 56 at +32). The 1,893 code and 1,138 data
  addresses the reconstruction, speedfix and the SDK name are settled.
- Every changed dword in .rdata/.data is a pointer into moved code, a build timestamp or the build-date string.
- Two copy routines with jump tables inside (0x103149D0, 0x103169A0) are not shown the same by the
  instruction-by-instruction check. Nothing of ours names them.
- test_xikit.py and test_speedfix.py pass on both builds. On the new build the kit swapped all 774 functions and
  refused none, and the reconstruction received the two ranges.

## Measuring what the gate cannot settle (`facts=1`)

8 exported functions are UNRESOLVED (the occlusion readback among them), and 2 more in the in-game DLL's fuzz
gate. Each is blocked at a call whose pop count has no definitive source. `xikit_facts.txt`
(`pipeline/fact_sites.py sites --from-gates`, built with the kit) lists the 54 such call sites in the committed
gate records, each with a patch region that:
- ends at the call's return address, so a thread inside the callee returns to the game's own bytes;
- is the call plus the plain instructions just before it, at least 5 and at most 16 bytes;
- holds no other call or branch;
- is jumped into by nothing in the game.

With `facts=1` the kit patches those regions to run from a copy that records ESP just before the call and just
after it returns, plus where the call went: the target, and for `call [reg+disp]` the vtable and slot.
`xikit_facts_<pid>.csv` is rewritten every window. Then

    python3 pipeline/fact_sites.py ingest xikit_facts_<pid>.csv

records each site's pop count in `data/game_sp.json`, only when every observed call agreed. The gate reads that
file as one more definitive source (CLAUDE.md: the original running real game code); the site then needs a
re-gate. A site never reached in a session stays unmeasured.

**Tested** (Wine, mode `facts`): two CDx call sites, one a 3-byte `call [ecx+0xC]` whose region starts 2 bytes
earlier. Both still reach their methods, are measured popping 4 through vtable slots 3 and 34, and are ingested.

## Mods (`mods=`)

`mods=name.dll,...` loads mod DLLs from the kit's folder at the start; they can replace manifest functions, call
the game's own version through a trampoline, and draw each frame. See docs/SDK.md.

**Device hooks** (`device_hook`, docs/SDK.md) sit in a vtable copy of their own in front of the others on the device
object: the game's call reaches a mod's hook first, then the profiler and trace (d3dprof), then the kit's Present hook,
then Direct3D. So the profile and the trace record what the mods let through, which is what the device receives. A
call that a hook passes on, or makes itself while handling a call, is recorded with the call site of the call it is
handling (the kit keeps that return address while the hook runs, on the render thread). Device hooks follow the master
switch like function hooks: F9 and a trace's unswapped frames send every call past them.

**Reloading** (`mod_reload=1`, read at the start): the kit loads each mod from a copy,
`xikit_mods_loaded\NAME.N.dll`, so a build can write the mod's file while the game runs. Every 30 frames it looks at
the files; one that has changed, and not changed again since the last look, is loaded again: the new build is loaded
first (if that fails the old one stays), the old build's `xikit_mod_unload` runs, its function and device hooks are
taken off, and the new build's `xikit_mod_init` hooks again; the entries are re-aimed at that frame's `apply()`, with
the other threads suspended. Creating `xikit_reload_now.txt` reloads every mod. No reload starts during a trace. Old
builds stay loaded: a thread may still be inside one.

## Comparing what the game draws with and without the swaps (`trace=1`)

Bisect needs your eyes. The trace needs only a still scene. With `trace=1` in `xikit.ini` (takes effect at the next
start; d3dprof is built in), standing still, press **Ctrl+Alt+F8**. Where keys do not reach the game window (this client), use the timer or a file instead: `trace_after=60` runs the trace 60 seconds after the start, then every `trace_every` seconds, `trace_count` times in all; creating `xikit_trace_now.txt` next to `xikit.dll` starts one (the kit deletes it). With `bisect=1`, the files `xikit_bisect_bad.txt`, `_good.txt` and `_stop.txt` answer like F10, F11, F12. The kit restores every original, then records the
Direct3D calls the render thread makes for 17 frames with the swaps flipped at every frame (off, on, off, ... off;
`trace_alternate=1`, the default), and writes `xikit_trace_<pid>_<n>.txt` (progress in `xikit.log`). A frame where a
thread was inside a function's entry, so not every swap could be flipped, is labelled 2 (mixed) and left out.
`trace_alternate=0` gives the older trace: 4 frames all off, then 4 all on. Per call: the method, the call site (analysed address), the
method's first parameters, and a hash of what a pointer parameter points at where the method reads a structure through
it (transform, viewport, material, light, clip plane, shader constants, vertex data of the two `...UP` draws).
With `trace_buffers=1` (default) every vertex and index buffer the game binds during a trace gets its `Lock`/`Unlock` wrapped (a vtable copy for that object, as for the device): at `Unlock` the locked range is hashed and recorded with the call site of the `Lock`, so `trace_diff.py` lists buffer contents by the function that filled them (reconstruction addresses are named through the reconstruction's exports and the load address in the trace header). Texture contents are not traced.

    python3 kit/d3dprof/trace_diff.py xikit_trace_<pid>.txt --manifest xikit_manifest.txt

For an alternating trace, each swapped frame is compared with the unswapped frames just before and after it. Calls are
paired by method, the state they set (render state, stage and type, transform, light index, ...) and their order in
the frame; a group is compared only where all three frames make the same number of calls. A call is reported as
STABLE (both neighbours agree with each other and the swapped frame does not), OUTSIDE (a kept matrix or vertex
float lies outside the range its two neighbours span, so it is not in between the frame before and the frame
after) or COUNT (the neighbours make the same number of calls and the swapped frame another). The same test is run
on unswapped frames against the unswapped frames two either side, as a control: a finding the control shows as
often is the scene's own. Where the neighbours differ and only a hash was kept (a light), the call is counted as
undecided. The function named with a finding is the swapped frame's call site through the reconstruction's exports,
a naming aid.

For a trace with `trace_alternate=0` it compares the halves by call content (method, parameters, the hash of the data a pointer parameter covers; the pointer itself is left out, it differs between runs). It does not compare call sites by default: with the swaps on a call is made from inside the reconstruction, whose addresses are not the game's, so by-site (`--by-site`) shows every swapped function as different. The scene's own change (the 4 frames without swaps against
each other) is subtracted; what is left changed when the swaps went on, and `--manifest` names the swapped function
nearest below each call site. It says where the two differ, not which side is right.

### The state each draw ran with (in every trace)

d3dprof shadows the render states, texture-stage states, textures, vertex shader, index buffer and vertex streams
the game sets (for its redundancy count). In a trace, each draw record carries ` s=XXXXXXXX`: a hash of that shadow
at the draw. After the records, one `#S HASH ...` line per distinct state lists it: `rs:STATE=VALUE`,
`tss:STAGE.TYPE=VALUE`, `tex:STAGE=OBJECT`, `vs=`, `ib=`, `vb:STREAM=OBJECT/STRIDE` (values in hex, state numbers
as the D3D8 header numbers them). Only what is known is written: a state the game has not set since the kit
attached, or since a state block was applied or the device reset, is unknown to the shadow and left out, not
guessed. At most 4096 distinct states are kept per trace; the header line counts hash collisions with a different
state (expected 0). `trace_diff.py` uses them: a STABLE finding on a draw names the states that differ between the
swapped frame's draw and its neighbours' (`| states: rs:7 FFFF->1, ...`).

### Every resource the game creates (`xikit_resources_<pid>.csv`, with `profile=1` or `trace=1`)

The device's creation methods (CreateTexture, CreateVolumeTexture, CreateCubeTexture, CreateVertexBuffer,
CreateIndexBuffer, CreateRenderTarget, CreateDepthStencilSurface, CreateImageSurface, CreateVertexShader) are
wrapped from the start when d3dprof is attached (`profile=1` or `trace=1`), on every thread: each call is appended to `xikit_resources_<pid>.csv` with the call site
(analysed address, or the raw return address outside FFXiMain), the parameters named in the method column, the
object it got and the HRESULT. The object pointers are the ones the trace's `tex:`, `vb:`, `ib=` and the
`SetTexture`/`SetStreamSource` parameters show, so a state can be read back to who created what, with which format
and size.

The first time an object a creation method returned has a vtable not seen before, `xikit_vtables_<pid>.csv` gets
a line: the interface the D3D8 header declares for that method's out parameter, the vtable, and its entries (as
many as the interface has), each as `module+offset`. `pipeline/com_vtables.py ingest` turns it into
`data/com_vtables.json`; the gate uses a record to know which interface a captured object is, but only when the
captured vtable's entries are exactly the recorded ones.

A vertex shader's declaration and code are saved exactly as the game passed them, as
`xikit_vshader_<pid>_<handle>.bin`; the handle is the one `SetVertexShader` and the `#S` lines' `vs=` show.

    python3 kit/d3dprof/vshader.py xikit_vshader_*.bin

decodes them by the D3D8 SDK's token formats (d3d8types.h): which stream and register each input comes from, with
its type, and the vs.1.x instructions. An opcode or register type outside that table is printed raw. This is the
runtime catalogue: the shaders as created. Each is also named against the static catalogue (`catalogue: vs2, source
table 0x103770D0 ...`, or "not one of the shaders the game assembles from its own source text").

**The static catalogue** (`kit/d3dprof/shader_catalogue.py`, `data/shader_catalogue.json`): every shader the game
assembles, from the binary. 0x10178250 builds 5 vertex shaders and 1 pixel shader from tables of source-line
pointers in `.data` (19, 33, 37, 41 and 45 lines; the pixel shader 9). 0x10177090 joins the lines; a line starting
with `#` is dropped when `[CDx+0x920]` is set and kept otherwise (none of the six has one, so both variants
assemble the same). The tokens come from running the game's own `D3DXAssembleShader` (0x102DAA0D) in Unicorn
with the game's flags, and are read back through the `ID3DXBuffer` methods. D3DX's check of the result by
d3d8.dll's `ValidateVertexShader`/`ValidatePixelShader` is not run (no d3d8.dll here): it only sets the error flag
and does not write the tokens. The vertex declaration is the one 0x10178250 writes on its stack: stream 0, v0
FLOAT3, v3 FLOAT3, v5 D3DCOLOR, v7 FLOAT2, v15 FLOAT3. The committed JSON holds hashes, counts and addresses;
the source text and tokens are written to `runs/shaders/` (game content, not committed). Decoded with
`vshader.py`, the tokens match the source lines except notation: D3DX writes `sub a, b, c` as `add a, b, -c`,
and a short swizzle repeats its last component (`.xy` is `.xyyy`).

The 7 `CreateVertexShader` sites: 0x101772A7 (the 5 above); 4 with no code, fixed-function shaders from a
declaration only (0x1002D6DC, 0x101792BE, 0x101792DF, 0x10284A58); 2 in functions nothing calls or points to
(0x10017810; 0x101771B0, which assembles `c:\test.vsh`, a developer leftover). The one `CreatePixelShader` site
(0x10177323) makes the pixel shader above.

## Frame capture (`fcap=1`): one whole frame, replayed through any d3d8.dll

`fcap=1` keeps, from the start, a copy of what the game writes into its vertex and index buffers in D3DPOOL_DEFAULT.
The game cannot read those back: they are write-only (run 2's resource log: 18,704 dynamic vertex buffers, 3,856 model
index buffers). A capture then writes one frame, from one Present to the next, to `xikit_fcap_<pid>_<n>.fcap` in the
log folder (format: kit/d3dprof/fcap_fmt.h). It holds:
- the device's state at the start, as Set calls;
- every call that changes something, with the data it points to (matrices, lights, ...UP vertices, rectangles);
- every object the frame uses, at first use:
  - managed textures and buffers, read back;
  - default-pool buffers, from the kept copies;
  - render-target textures, through CopyRects into an image surface;
  - the vertex shader declarations;
- buffer locks inside the frame;
- the back buffer as the game finished it: the picture the game's own d3d8.dll (dgVoodoo) drew.

D3D8 cannot read a state block's contents. At each ApplyStateBlock the capture applies the block itself (a block only
sets values, so applying it twice changes nothing), reads the whole state back, and writes what changed as Set calls.
Calls recorded into a block (between BeginStateBlock and EndStateBlock) are left out.

When: `fcap_after` seconds after the start (then every `fcap_every` seconds, `fcap_count` in all), or when the file
`xikit_fcap_now.txt` is created next to xikit.dll (the kit deletes it). The capture frame is slow (it reads the state
back at every ApplyStateBlock and every texture the frame uses); a plaza frame's file is expected to be tens of MB.

Not recorded (version 1), each said in the file as a note where it happens:
- calls from other threads during the frame;
- a texture written through LockRect after the frame first used it;
- objects or vertex shaders made before the kit attached;
- depth surfaces' contents;
- pixel shaders and ProcessVertices.

Buffers are wrapped by patching Lock, Unlock and Release in their class's vtable, not by giving objects a copy: Wine's
d3d8 checks that a buffer's vtable is its own (it asserts in dlls/d3d8/buffer.c).

**Replay** (`kit/fcap/replay.c` → `fcap_replay.exe CAPTURE.fcap OUT`): plays the capture into whichever d3d8.dll sits
next to it, and writes `OUT_replay.pic` (what that device drew) and `OUT_capture.pic` (what the game's drew).
`kit/fcap/compare.py A.pic B.pic --png OUT.png` compares two pictures.
`kit/fcap/ties.py CAPTURE.fcap --png OUT.png` replays a capture through kit/d3d8on11 under Wine twice, the second time
with D8ON11_TEST_TIES, and counts the pixels where the finished picture shows a point sample within 1/256 texel of a
texel boundary: where D3D11 drivers may pick different texels (kit/d3d8on11/README.md). An upper bound.

**Tested** (`kit/fcap/test/test_fcap.py`, Wine):
- a program draws game-like frames through Wine's d3d8 with d3dprof and fcap attached as the kit attaches them, and
  captures one. The frames hold write-only buffers filled before the captured frame, a dynamic buffer rewritten in it,
  managed DXT1 and A8R8G8B8 textures, the three-stream character declaration, a state block applied, a render-to-texture
  pass, fog, ZBIAS, ...UP draws and the readback's CopyRects;
- replayed through Wine's d3d8: the picture equals the captured one exactly;
- replayed through kit/d3d8on11: equal within 1 of 255.
- The first replay through d3d8on11 found three of its gaps: normals normalized without D3DRS_NORMALIZENORMALS,
  SetClipPlane, and CopyRects from an image surface. All three are fixed.

## Finding which functions break the picture (`bisect=1`)

With `bisect=1`, and the screen wrong with everything swapped: Ctrl+Alt+F10, or create `xikit_bisect_bad.txt` next to
`xikit.dll` (the keys do not reach every game window; the kit deletes the file once read). The candidates are the
swapped functions that have run. The kit then swaps a part of them only and logs which (`xikit.log`,
`xikit_bisect.txt`); answer each step with F10 / `xikit_bisect_bad.txt` (still wrong) or F11 /
`xikit_bisect_good.txt` (right). F12 / `xikit_bisect_stop.txt` ends it and swaps everything again.

No single culprit is assumed (on 6 Oct the picture broke with 7 functions swapped and was right with every group a
one-culprit bisect tried). The search is delta debugging (Zeller's ddmin) while more than 8 suspects remain, then each
suspect is left out in turn: at most 8 answers from there. It ends with `CULPRIT: <name>`, or `CULPRITS TOGETHER` and
the list when several functions break the picture only together; the set it reports is minimal (with any one of them
left out the screen is right). The culprits stay on the original code for the rest of the run.
