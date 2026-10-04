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
| `docs/` | `METHOD.md` (how it works), `HISTORY.md` (dated record), `RESIDUE.md` (every graphics function not in the tree, with the reason; generated), `NAMES.md` (naming evidence) |
| `pipeline/` | Everything that runs outside IDA: translator, gate, fact tools, tests (`pipeline/tests/`) |
| `ida/` | What runs inside IDA: `ffxi_ida_fix.py` (generated, self-contained), its template, the graphics-set exporter `ida_gfx.py`, `ida_regflow.py` |
| `data/` | Inputs and facts: the Hex-Rays export (`exports/`), facts read from the DLL, hand corrections (`hx_patches/`, `type_fixes.json`), the gate's input model, the class model, D3D8/D3DX data (`d3dx/`) |
| `run.sh` | The pipeline, end to end |
| `runs/` | Scratch output of `run.sh` (ignored) |

## Status (4 Oct 2026)

768 graphics functions, each with a stated outcome (`docs/RESIDUE.md`):

| | Count |
|---|---|
| **Verified in the clean tree, as built** (`tree/`, 189 files) | **708**, 0 failures |
| Microsoft D3DX 8.1 (statically linked) | 48 |
| Constructs C++ cannot express | 5 |
| Gate UNRESOLVED (a call with no definitive pop; the call site is named) | 4 |
| Gate FAIL (first divergence recorded) | 2 |
| Gate INCONCLUSIVE (the original never returns) | 1 |

Gates (`verification/`): the fuzz gate (100 random trials per function) and the live gate on each of four
memory dumps of the running game, verified as the analysed build (`verification/live/`). Raw and clean,
combined (`combine_gates.py`: a FAIL anywhere is a FAIL): 708 PASS. Each function's `// GATE:` line lists
every gate's verdict and returning trials.

The 12 open functions, each with a named cause:
- **FAIL (2):** `sub_1017DE20` (IDA's stack pointer is wrong in it, so one stack slot has two names);
  `sub_100548D0` (returns the x87 status word in AX).
- **Constructs (5):** `CDx_CreateTexture` and `sub_1017C9C0` (the same IDA stack-pointer cause, read as a
  local on the return address); `CMoSchedularTask_Interpret`, `sub_10125360`, `sub_10184C90` (`__asm`).
- **UNRESOLVED (4):** D3D calls through a field with several writers (0x10009F44, 0x1006C16F, 0x102811DC);
  a game-class call whose class has no instance in the dumps (0x10047623).
- **INCONCLUSIVE (1):** `sub_10016320`, the global shutdown.

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
- Python 3 with `unicorn`, `pefile`, `capstone` and `minidump` (for dumps);
- MinGW-w64 i686 (`i686-w64-mingw32-g++`);
- IDA Pro 9.x with Hex-Rays, for new exports.

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
