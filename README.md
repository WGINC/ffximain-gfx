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
| `docs/` | `METHOD.md` (how it works), `HISTORY.md` (dated record), `RESIDUE.md` (every graphics function not in the tree, with the reason; generated), `NAMES.md` (naming evidence), `KIT.md` and `SDK.md` (the in-game tools), `ROADMAP.md`, `GAPS.md`, `RENDERER.md`, `UI.md`, `DEARTIFACT.md`, `LOCALS.md` |
| `pipeline/` | Everything that runs outside IDA: translator, gate, fact tools, tests (`pipeline/tests/`) |
| `ida/` | What runs inside IDA: `ffxi_ida_fix.py` (generated, self-contained), its template, the graphics-set exporter `ida_gfx.py`, `ida_regflow.py` |
| `kit/` | The in-game kit: `xikit` (swaps reconstructed functions into the running game, traces, bisect, capture) and `d3dprof` (per-call-site Direct3D profiler). `kit/README.md`, `docs/KIT.md` |
| `sdk/` | Mods for xikit: headers, template, examples, forks (`fork.py`). `docs/SDK.md` |
| `data/` | Inputs and facts: the Hex-Rays export (`exports/`), facts read from the DLL, hand corrections (`hx_patches/`, `type_fixes.json`), the gate's input model, the class model, D3D8/D3DX data (`d3dx/`) |
| `run.sh` | The pipeline, end to end |
| `runs/` | Scratch output of `run.sh` (ignored) |

## Status (25 Oct 2026)

The tree is selected by the frame, not the device: every function the frame is known to run (`pipeline/frame_set.py`,
`data/frame_set.json`; a lower bound, since unproven indirect calls are not followed), from five exports
(`tree/source.json`: IDA and Ghidra). Of the frame set's 4,765 game-code functions, 3,748 are in the tree and 1,017
are not (`data/frame_set.json` `_summary`).

| | Count | Source |
|---|---|---|
| **Functions in the tree** (`tree/`) | **4,206** | `tree/placement.json` |
| **PASS on the fuzz gate** | **4,205** | `verification/gate_clean_final.jsonl`, under the gate rules of 22 Oct (P10, P13 in `docs/PROBLEMS.md`) |
| Not verified (`// NOT VERIFIED`) | 1: 0x10003890, INCONCLUSIVE (the original faults on every fuzzed input through objects in its own `this`) | same |
| Gate-passed, left out of the tree | 9: each calls a game function that may read a register its wrapper does not pass | `tree/left_out.json` |
| **Compiled by the game's own compiler to the game's exact bytes** | **1,313** of the 22 Oct tree's 4,205 functions; 1,281 of them are in the 24 Oct tree. Not rerun on the current tree's text | `verification/msvc_match_tree.json` (Visual C++ 6.0, Processor Pack code generator, `/O2`; relocations by target) |

What "PASS" rests on, and what it does not:
- The fuzz gate only: 100 random trials per function. The live gates on the four memory dumps of the running game
  (`verification/live/`) were run on the 4 Oct tree (774 functions) and not since: the dumps are not in this
  environment (PROBLEMS P1).
- Not every verdict was re-gated on the current build. A function whose compiled code, case, call-site facts and
  fixed inputs are unchanged since the build it was gated on keeps that verdict (`pipeline/code_same.py`;
  docs/HISTORY.md, 22 Oct evening).
- 0x10016320 passes only under the fixed inputs `data/case_inputs.json` states; its GATE line says so.
- Open questions that can change verdicts are in `docs/PROBLEMS.md`. IDB call types of unrecorded origin are not
  used (P13): an IDB type counts only where it is the SDK's, and the 912 indirect sites that rested on one alone are
  UNRESOLVED wherever the gate reaches them. The owner can admit their own types by marking
  `requests/review/02_user_types.md` (224 of 372 have no recorded origin).
- Function names: many come from naming sessions, measured at 61-63 of 100 known names correct and 1-2 wrong
  (PROBLEMS P16). A name says nothing about the code; the gate binds every object by address.

One result rests on a declaration the owner accepted rather than a proof: `CDx+0xC` holds the device
(`data/com_fields.json`, "accepted"); every call and stack balance that uses it carries the note.

`pipeline/verify_all.py` re-derives every committed fact from the binary, regenerates the tree from its recorded
sources and, without `--quick`, re-gates it (`verification/verify_all.json`). It fails on one known check,
`export-sp` (PROBLEMS P5): IDA's stack depth in the epilogue of 0x100412B0 is 4 more than the real depth from
0x100419F4, on a path that never ran and has an unsolved call, so no correction can be named.

`docs/RESIDUE.md` still describes the 6 Oct graphics set (867 functions, before the frame set) and has not been
regenerated for the current tree.

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
