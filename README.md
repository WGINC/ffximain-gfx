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

## Where things stand (11 Nov 2026)

- **The game's builds:** three are known to the kit: 6A995428 (analysed), 6AC37846 and 6AE818B4 (2 Nov; translated,
  docs/KIT.md). On 6AE818B4 speedfix and the kit's measuring work; the reconstruction is not swapped in (61 of its
  addresses are not proven there).
- **Frame time, Port Jeuno plaza, d3d8on11 with speedfix (the owner's run of 16 Nov):** median frame 14.5 ms standing
  and 12.5 running with the late occlusion read that never waits (`occlusion_nowait`, the default) and the buffer
  arena (`buffer_arena=1`); 16.9 and 19.0 without them. 0.7 to 4.2% of frames are over 33 ms.
- **What limits it:** the number of draws. d3d8on11's worker spends about 7.7 us on each (Present's time is 4.9 us a
  draw) and the game's thread 2.3 us, at 1,500 to 1,700 draws a frame.
- **Run once in the game (the owner's 18 Nov session), not yet measured:** draw merging (`draw_merge`), delta draws
  (`draw_delta`) and GPU skinning (`gpu_skin`). GPU skinning hid every character but the owner's: d3d8on11 dropped
  the game's occlusion rectangles while a skinned buffer was bound. Fixed and tested on 11 Nov; not yet run again
  (`kit/sessions/ab_19nov`). In that run the graphics device was lost once (0x887A0005) and the game crashed; the
  cause is not known, and the build now logs the device's reason.
- **The long frames while running (that run's samples, 191 frames of 33 ms or more in 145 s):** 97 were the game's
  thread waiting inside Windower's Lua runtime, entered from Windower's hook on a packet from the server; 84 were
  the game's waiting readback for a first occlusion test. Both counts are from the start with the defect above;
  ab_19nov repeats them and switches each cause off in turn.
- **Not explained:** shimmering in the distance and nameplates jittering while the camera moves (both also with the
  new pieces off).
- **Above 60 fps:** the game's time follows real time (`uncap`, `frame_step=3`). Liminality (`liminality=1`) is built
  and passes its offline tests; its last version has not run in the game (`kit/sessions/ab_15nov`).
- **Not working:** d3d8on11's worker as a native ARM64 process (`mode=remote`) crashed in Windower's Hook.dll in the
  owner's game on 16 Nov; cause not established.

Earlier status sections of this file (5 Nov, 1 Nov) are in docs/HISTORY.md, "11 Nov: README sections retired".
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
