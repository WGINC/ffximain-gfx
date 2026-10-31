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
| `kit/` | The in-game kit: `xikit` (swaps reconstructed functions into the running game, traces, bisect, capture) and `d3dprof` (per-call-site Direct3D profiler). `kit/README.md`, `docs/KIT.md` |
| `sdk/` | Mods for xikit: headers, template, examples, forks (`fork.py`). `docs/SDK.md` |
| `data/` | Inputs and facts: the Hex-Rays export (`exports/`), facts read from the DLL, hand corrections (`hx_patches/`, `type_fixes.json`), the gate's input model, the class model, D3D8/D3DX data (`d3dx/`) |
| `run.sh` | The pipeline, end to end |
| `runs/` | Scratch output of `run.sh` (ignored) |

## Status (27 Oct 2026)

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
