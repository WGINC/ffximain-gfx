# FFXiMain.dll graphics reconstruction (build 2026-09-03)

Gate-verified C++ for the graphics code of the FFXI PC client, produced from IDA Pro / Hex-Rays output.
Every function is checked against the original in an emulator. Every fact the pipeline relies on comes
from a definitive source, or the result says it is unknown (`CLAUDE.md`).
This is private work: nothing here is published, and no game bytes are in the repo.

## Layout

| Path | What |
|---|---|
| `tree/` | **The deliverable.** Clean, class-shaped C++ (`include/`, `src/`, `build.sh`); each function carries its `// GATE:` result. Generated; see `tree/README.md` |
| `verification/` | Gate results for the tree: fuzz raw (`gate_raw_final`), fuzz and live combined (`gate_raw_combined`, `gate_clean_final`), the live gates per dump and the dumps' verification (`live/`), cases, first divergences, the mutation test |
| `docs/` | `METHOD.md` (how it works), `HISTORY.md` (dated record), `HANDOFF.md` (state and next steps), `EFFICIENCY.md` (working cheaply), `PLAN.md` and `PLAN_TASKS.md` (the plan and its tasks), `FRAMESET_REMAINING.md` (the frame set outside the tree, by blocker), `NAMES.md` (naming evidence), `KIT.md` and `SDK.md` (the in-game tools), `ROADMAP.md`, `GAPS.md`, `RENDERER.md`, `UI.md`, `DEARTIFACT.md`, `LOCALS.md` |
| `pipeline/` | Everything that runs outside IDA: translator, gate, fact tools, tests (`pipeline/tests/`) |
| `ida/` | What runs inside IDA: `ffxi_ida_fix.py` (generated, self-contained), its template, the graphics-set exporter `ida_gfx.py`, `ida_regflow.py` |
| `kit/` | What runs in the game: `xikit` (hooks, measuring, capture, swaps of reconstructed functions; build translation), `speedfix` (the performance mod: levers, `uncap`, `frame_step`, Liminality), `d3d8on11` (our Direct3D 8 device on Direct3D 11), `d3dprof` and `fcap` (profiler, frame capture), `xiload`, `xisamp`, and `sessions/` (each owner run's package and instructions). `kit/README.md`, `docs/KIT.md` |
| `sdk/` | Mods for xikit: headers, template, examples, forks (`fork.py`). `docs/SDK.md` |
| `data/` | Inputs and facts: the Hex-Rays export (`exports/`), facts read from the DLL, hand corrections (`hx_patches/`, `type_fixes.json`), the gate's input model, the class model, D3D8/D3DX data (`d3dx/`) |
| `run.sh` | The pipeline, end to end |
| `runs/` | Scratch output of `run.sh` (ignored) |

## Where things stand (14 Nov 2026): shipped as Longthinx gobbiefix

- **What ships:** `kit/install` packages the renderer (d3d8on11 on Direct3D 12), the kit (xikit) and speedfix as
  **Longthinx gobbiefix**: one installer, nothing to start before the game, a public settings file (`gobbiefix.ini`,
  each setting written up in `kit/install/README.md`), quiet logs, and `gobbiefix_debug.bat` for the statistics.
- **Installed settings, each measured on the owner's M5 Mac (Parallels, Windows on ARM):** no frame cap (a 120 cap
  and vsync measured no better), `smooth_pacing` (frames over twice their neighbours 67 -> 19 a minute), 2 swap
  buffers (1 Dec, blind: Windows showed 94% of frames against 76% with 3; the owner felt no stutter), `precise_pacing`
  under `uncap` with `frame_step=3` (the character's step variation 15-19% -> 10-11%), mip levels with 16x anisotropy.
- **Known, not ours to fix from inside Windows:** frames Windows has shown are lost before the Mac's panel (phone at
  240 fps against PresentMon: the panel 58-86 new pictures a second while Windows showed 89-113; the same with the Mac
  fixed at 60 Hz). Where between Parallels and the panel is not known (docs/HISTORY.md, 33rd-35th entries).
- **Open, cause not known:** frames of 25-31 ms every few seconds in some starts (not in the renderer; the sampler was
  off); 25 frames where the game waited on the renderer, one burst of which PresentMon shows as the GPU busy ~100 ms a
  frame on ordinary frames; Windower's Config addon faulting at exit (Config.dll+0x41F67, 30 Nov log); Windows
  composing the game instead of flipping it directly; `mode=remote` (the ARM64 executor process) crashed once in
  Windower's Hook.dll (16 Nov).
- **The game's builds:** 6A995428 (analysed), 6AC37846 and 6AE818B4 are known to the kit (manifests); a build it does
  not know runs with the renderer alone and a window says to update.

Earlier status sections of this file (11 Nov, 5 Nov, 1 Nov) are in docs/HISTORY.md, "11 Nov: README sections retired" and "14 Nov: the 11 Nov status retired".
Every document and what it is for: docs/README.md. The devkit's map: docs/DEVKIT.md.

## Reproduce

    export FFXI_DLL=/path/to/FFXiMain_unpacked.dll   # never copied into the repo
    ./run.sh all data/exports/gfx_export.jsonl myrun

Under `runs/myrun/`, this:
1. reads the facts from the DLL;
2. translates the export into one unit per function and builds them;
3. gates every function;
4. generates the clean tree from the passing ones;
5. gates that tree as built and annotates it;
6. writes `RESIDUE.md`.

Steps run alone too: `./run.sh facts|translate|cases|gate|clean|cleangate|residue`. `./run.sh test` runs
the rule tests and the stack solver's self-test. A full gate takes about 8 minutes on 4 cores.
`pipeline/gate_some.py RUN fail|notpass|changed:DIR|0xADDR...` gates a subset.

**Requirements:**
- Python 3 with `unicorn`, `pefile`, `capstone` and `minidump` (for dumps), and `numpy` (the kit's picture comparison);
- MinGW-w64 i686 (`i686-w64-mingw32-g++`);
- IDA Pro 9.x with Hex-Rays, for new exports;
- For kit/d3d11probe: PyPI `ziglang==0.16.0` (ARM64 Windows builds), and for its tests `xvfb` and i386 Mesa
  (`libgl1-mesa-dri:i386`).
- 32-bit Wine, for the kit's tests (`kit/xikit/test/test_xikit.py`; on Ubuntu: `dpkg --add-architecture i386`,
  then `wine` and `wine32:i386`).

## The IDA step

With the IDB open in IDA 9.4 (save it first, or work on a copy), run **File > Script file... >
`ida/ffxi_ida_fix.py`**. It runs unattended and writes `gfx_export.jsonl`, `ida_fix_report.json` and
`ida_fix_log.txt` next to the IDB. Put them in `data/exports/`, then run the pipeline.
`METHOD.md` section 6 lists what the script changes.

## Tools

| Tool | Use |
|---|---|
| `gate_some.py` | Gate any subset on a work queue; prints each verdict that changed from `verification/` |
| `localize.py`, `watch.py` | First divergence of a FAIL with the source line; who wrote a memory range on each side |
| `disasm.py` | Disassemble a function by address from the DLL (all chunks, with names) |
| `sp_solve.py`, `vcall_resolve.py`, `com_calls.py`, `real_sp.py`, `purges.py` | The definitive pop sources (`METHOD.md` section 2) |
| `callsite_audit.py` | Every indirect call: what each source says it pops, conflicts, and the calls none covers |
| `dump_verify.py`, `live_state.py` | Is a dump the analysed build; the gate's live inputs from it |
| `ida_sp_check.py`, `stack_depth.py` | Where IDA's SP differs from the exact depth, with the one correction for each; depth per instruction |
| `unset_reads.py`, `loop_bounds.py` | Locals Hex-Rays reads but never assigns; why an original runs to the instruction limit |
| `ida_listing.py`, `name_context.py` | Read an IDA `.asm` listing without IDA; the evidence for naming placeholders |
| `gfx_coverage.py`, `libmatch.py` | The coverage audit; D3DX identification |
| `same_code.py` | Are two builds the same code, function by function |
| `make_ida_fix.py` | Rebuild `ida/ffxi_ida_fix.py` from the template and `data/` |
| `bg.sh` | Run a long job in the background and stop it by process group |
| `unpack_pol.py` | The shipped FFXiMain.dll with its code filled in (its entry stub run in an emulator); for a new build |
| `build_translate.py` | Can the kit run on another build: the code ranges that moved, every manifest function proven the same code, every address we name settled (`kit/xikit/builds/<stamp>.json`; docs/KIT.md) |
| `tool_drift.py` | Are the tools current: regenerates the IDA script and the SDK's generated files, rebuilds the kit and compares with `kit/BUILD.lock.json`, checks the kit's ini keys against `docs/KIT.md` (`--fix` keeps the regenerated files) |
