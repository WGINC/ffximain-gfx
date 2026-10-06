# The in-game kit (xikit)

Runs the reconstructed graphics code inside the real game, one function at a time or all of them, with a
switch to go back to the original at any frame. Built on what is verified (`tree/`, the gate verdicts) and on
the crash-safety rules the owner's `speedfix_hook` already follows.

## What it does

| Feature | What | Default |
|---|---|---|
| Build check | Does nothing unless FFXiMain.dll is the analysed build (PE TimeDateStamp 0x6A995428) and the reconstruction DLL is the one the manifest was made from | always |
| Swap | Writes a `jmp` at a verified function's entry to its reconstruction (`gfx_recon_ingame.dll`, every game address rebased through `xi_rel`) | off; `swap=1` |
| Selection | All verified functions, or a list (address or name); minus an exclude list, the quarantine file, and the functions `speedfix_hook` patches inside when it is loaded | all verified |
| Live toggle | Changing the ini (re-read every 60 frames) or the hotkey (Ctrl+Alt+F9: all swaps off/on) patches or restores at the next frame boundary | on |
| Call counts | A per-function stub counts how often each swapped function runs (`xikit_calls_<pid>.csv`): which reconstructed code the game actually used | on |
| Crash capture | An exception whose address is in reconstructed code (or a stub) is logged with the function, registers and the top of the stack, and the function is added to `xikit_quarantine.txt`, so the next start leaves it out. An exception in game code (or another module) called from reconstructed code: the stack is searched for return addresses into reconstructions (each must follow a `call`), the chain is logged, and the innermost is quarantined as `suspected` | always |
| Untested functions | Functions whose gate trials did not run every instruction of the original (manifest flag 4) are left out | on; `include_untested=1` swaps them too |
| Profiler | d3dprof (per call site, per frame) in the same process, chained | off; `profile=1` |
| Log | `xikit.log` next to the DLL: what was checked, swapped, refused and why | always |

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
fake CDx and device, loads `xikit.dll` from `dist/` and plays the render thread. It runs five times (the last two
with a mod, `test/mod_test.cpp`, at the analysed base and relocated: docs/SDK.md, Limits); the first three:

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

  Samples estimate time shares; they do not time each call. Per-call time for Direct3D methods is this
  "elsewhere" view, statistically.
- **Tested** (Wine, mode `profile`): timing lines written; samples written; samples taken while the thread ran
  the reconstructed GetColor name it.

## Capturing real calls for the gate (`capture=`)

The gate's trials ran 63% of the reconstruction's instructions; the hot paths far less (docs/GAPS.md). With
`capture=Name1,Name2` (manifest names or addresses), the kit records the next `capture_count` calls (default 1)
of each, from `capture_after` seconds after the start (default 0):
- **What a capture is.** At the entry, with every other thread suspended, it takes the registers, the stack
  above the entry (up to 256 KB), FFXiMain.dll's writable data, and every page reachable by following pointers
  from those, breadth first, up to `capture_pages` pages (default 20,000, 80 MB). It writes them as
  `xikit_capture_<pid>_<RVA>_<n>.xcap`. Which pages a call needs is not known beforehand: what pointers reach is
  a preparation of the inputs, and a page the capture lacks is filled as the gate fills any unknown page, on
  both sides alike.
- **The game is not changed.** The call then continues into the game's own code (a trampoline; the
  reconstruction where the entry has none). After the last capture the entry is restored.
- **On the analysis side:**

      python3 pipeline/xcap.py xikit_capture_....xcap verification/cases_clean_final.json tree/build/gfx_recon.dll OUT_DIR

  runs the original and the reconstruction from that exact call (its `this` and arguments, on its memory) in
  the gate's live mode, with coverage, and prints the verdict and how many of the original's instructions ran.
  Each capture is a gate case on the game's real state.
- **Tested** (Wine, mode `capture`): GetColor and SetColor captured, the game's values returned, the entries
  restored, and both captures replayed through the gate (PASS, every instruction run, SetColor's argument read
  off the captured stack).

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
