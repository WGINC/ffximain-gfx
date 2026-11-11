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

The two sections below are as written on 5 Nov and 1 Nov; where they differ from the list above, the list is right.

## Performance on the owner's machine (5 Nov 2026)

The owner's machine: an M5 MacBook Pro, Windows 11 on ARM64 in Parallels (the game x86-emulated), Windower loaded;
CrossOver on macOS as well. All figures are the Port Jeuno plaza, standing; sources in docs/HISTORY.md.

| Device | Levers | Occlusion readbacks | Median frame | Measured |
|---|---|---|---|---|
| dgVoodoo | off | on | ~50 ms | 4 Nov (W1, W2) |
| dgVoodoo | speedfix's | on | ~31-34 ms | 4 Nov (W5); 17 Oct (perf60) |
| dgVoodoo | speedfix's | skipped | ~33.5 ms | 17 Oct (run 3464) |
| **d3d8on11** | speedfix's | skipped | **12.9-14.0 ms** (p95 ~18) | 26 Oct (T3) |
| d3d8on11, nothing drawn | speedfix's | skipped | 7.5 ms | 26 Oct (T3) |
| d3d8on11 | speedfix's, `uncap` `frame_step` | answered by d3d8on11 (snapshots not hit) | 40.6 ms (p95 65) | 5 Nov (final, kit timing on) |

- On this machine the time is the render thread's CPU, not the GPU (PresentMon: GPU busy 5-7 ms). Through dgVoodoo
  about half of it is kernel buffer locks taken through WOW64 (18 Oct; 6,245 a frame on 4 Nov); d3d8on11's batched
  executor avoids them.
- Of Frame_Render's own calls (4 Nov, timed exactly): the world call 0x10074C30 holds most of the frame (35 ms
  vanilla, 21 ms with the levers), the UI call 0x1015F6A0 6 ms (0.7 with the levers), EndScene about 6 ms.
- Above 60 the game's time runs fast unless the step is fixed (docs/RENDERER.md 3a, 3b): speedfix `frame_step`.
  Above 60, most motion still moves in whole 1/60 s ticks: smoother motion needs interpolation (ROADMAP P4, not built).
- The last session (5 Nov, `kit/sessions/final_05nov`, docs/HISTORY.md): d3d8on11 with the levers, `uncap` and
  `frame_step`, readbacks not skipped. Median frame per step: busy 7.5 ms, menus 5.5, zone 6.2, camera 8.1, stand
  11.3, run 31.7, plaza 40.6 (77 fps over the run). In the plaza's 40 ms windows d3d8on11's executor thread is
  busy 98% of the time (d3d8on11.log: readbacks 17 ms, draws 10, Present 8): it is the limit, and no readback found
  its snapshot. `occlusion_late=1` (answered every late lock offline, 18 Oct) is not measured in the game.
  The game's time follows real time from 31 to 184 fps (the character's distance per real second, 5.955-5.981).
  Characters' poses flickered while moving above 60 (owner); the cause is not established. The 2.4 s stalls in that
  run come with the kit's call timing (none in the runs without it).
- The kit's measuring (docs/KIT.md, "Measuring"), `xiload.exe` (loads the kit, Windows and CrossOver),
  `kit/xikit/measure_read.py` (the report).

## Status (1 Nov 2026)

Where to start: `docs/HANDOFF.md` (the state and the next steps), `docs/EFFICIENCY.md` (how to work on it cheaply),
`docs/PLAN.md` (the plan) and `docs/PLAN_TASKS.md` (every task of the plan and its state).

The tree is selected by the frame: every function the frame is known to run (`pipeline/frame_set.py`,
`data/frame_set.json`; it follows proven indirect targets and the targets recorded in the running game, so it is a
lower bound). The frame set has 5,430 functions: 219 are D3DX library code, 4,449 are in the tree
(`verification/frame_set_status.json`). What is left of it, by blocker: `docs/FRAMESET_REMAINING.md`.

| | Count | Source |
|---|---|---|
| **Functions in the tree** (`tree/`, tree r17) | **4,897** | `tree/placement.json`; exports in `tree/source.json` (IDA, then Ghidra fs23, fs21, ..., fs22, fs24) |
| **PASS** (fuzz gate, combined with the live gates on the four dumps) | **4,884** | `verification/gate_clean_final.jsonl` |
| Not verified (`// NOT VERIFIED`) | 13: 8 FAIL, 3 INCONCLUSIVE, 2 UNRESOLVED, each with its cause | same; docs/PROBLEMS.md P18 |
| Gate-passed, left out of the tree | 15, each with its reason | `tree/left_out.json` |
| **At the game's exact bytes** (Visual C++ 6.0 SP6, Processor Pack code generator) | **2,474 of the frame set's 5,430 (45.6%)**: the sweep's conversions (2,557 of 4,766 swept, as written or under a rule) and the 317 sources in `recompile/` (122 hand-written, 195 the sweep's sources of functions outside the tree or under /O1, `recompile/sweep/`) | `verification/vc6_sweep.json`, `verification/recompile_match.json`, `pipeline/vc6/match.py` |

What "PASS" rests on, and what it does not:
- Every function is gated on 100 fuzzed trials. The 16 not PASS on fuzz were also gated on the four memory dumps of
  the running game (27 Oct); `combine_gates.py` makes a function PASS where some gate compared it and passed and none
  failed. Three passed that way, two of them on few returning trials (0x10003890: 3 per dump; 0x100E08D0: 2); the
  counts are in each record's `gates` field.
- A function with a second case (a case from another export) is gated under both, and the stricter verdict stands.
- Open questions that can change verdicts are in `docs/PROBLEMS.md`. IDB call types of unrecorded origin are not used
  (P13); the owner can admit their own by marking `requests/review/02_user_types.md`.
- Function names come partly from naming sessions, measured at 61-63 of 100 known names correct (P16). A name says
  nothing about the code; the gate binds every object by address.

One result rests on a declaration the owner accepted rather than a proof: `CDx+0xC` holds the device
(`data/com_fields.json`, "accepted"); every call and stack balance that uses it carries the note.

### In the game, and the way to a source-level D3D11 renderer (1 Nov)

- **Swapped in the game: 580 functions** (the kit's default set). Their frames are the game's own: the emulated whole
  frame is identical through the world pass on three dumps (`pipeline/frame_diff.py`), and the owner's block traces
  show no drawing difference (docs/HISTORY.md, 1 Nov). The other verified functions are not swapped by default: 4,211
  have incomplete gate coverage (manifest flag 4), and 89 used game addresses without rebasing them.
- **Swapping the untested ones as well is not yet safe.** In the emulated frame it differed on three dumps, all from
  one function: `Env_ComputeFog` (0x10189120) kept a pointer's top byte as the fog colour's alpha. The cause is a
  translation rule: Ghidra's casts to 3-byte integers (`(uint3)x`, 2,172 of them in 719 functions) were dropped.
  Fixed in `hx2cpp.ghidra_odd_casts`; not yet in an installed tree.
- **Unrebased addresses:** fixed by rule from the relocation table (`pipeline/reloc_literals.py`: relocated literals in
  every function, call return addresses, comparison bounds one off a relocated value). On a scratch tree the probe's
  holders went from 95 to 22; the rest are being settled. Not yet in an installed tree.
- **Device interface (#136):** with `XI_BACKEND` every Direct3D 8 call in the tree goes through `xi_backend[]`
  (`pipeline/xi_d3d8_backend.hpp`, generated by `gen_d3d8_backend.py`); a null entry calls the object's own vtable.
  The whole tree builds with it, and the emulated frame is identical with no backend installed (town, combat).
- **Render path as one set (#132):** the manifest marks it (flag 16, `verification/render_core_status.json`), and
  `swap_set=render_path` in xikit.ini swaps exactly those, untested included. Built, not yet run in the game.
- **Not in the tree yet:** 10 of the 57 Direct3D-calling functions and 33 of their 100 direct callers. Each has a
  named cause (unknown pop counts needing a `facts=1` run, EAX facts, 4 FAILs, 14 never gated). One cause is settled:
  the D3DX dispatch table's targets (`pipeline/dispatch_slots.py`).

So a source-level D3D11 speedfix can be built on the 580 functions and d3d8on11 today, but not yet the whole render
path: that waits on the next tree round (the fixes above), a `facts=1` run, and the remaining missing functions.

`pipeline/verify_all.py` re-derives every committed fact from the binary, regenerates the tree from its recorded
sources and, without `--quick`, re-gates it (`verification/verify_all.json`). Its one known failing check is
`export-sp` (PROBLEMS P5).

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
