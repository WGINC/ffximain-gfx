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

- **Manifest**: 775 functions listed (every function PASS over the merged gates), none refused (each has its
  export, room for the jump, and no jump into its first 5 bytes). Flags: 0 SEH, 3 speedfix sites, 155 untested
  or thin evidence (left out by default); 620 are swapped in by default.
- **The in-game DLL is the verified code**: `gfx_recon_ingame.dll` (the tree built with `XI_INGAME`, every
  game address `XI_GAME(ea) = ea + xi_rel`) is gated on fuzz itself with `xi_rel = 0`. Its verdicts are the clean
  tree's fuzz verdicts, function for function: 773 PASS, 0 FAIL, and 4 not compared on fuzz (0x10003890 and
  0x100CB4E0 UNRESOLVED, which pass on the dumps; 0x1006B3B0 and 0x10016320 INCONCLUSIVE everywhere, not listed).
  This check found the 5 Oct translation defects the clean build had passed by chance (`docs/HISTORY.md`).
- **No game address is left unrebased**: a scan of the in-game DLL's code finds 4,118 rebased adds and no
  absolute game address used directly; its data sections hold no game pointers.

## Tests (`kit/xikit/test/test_xikit.py`, Wine, the real FFXiMain.dll)

`host.exe` (`test/host.c`) maps FFXiMain.dll without running it (`DONT_RESOLVE_DLL_REFERENCES`), gives it a
fake CDx and device, loads `xikit.dll` from `dist/` and plays the render thread. It runs three times:

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
- `swap=1`: all 777 entries jump to the reconstruction. Reconstructed `SetColor` (0x100A4830) writes
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
