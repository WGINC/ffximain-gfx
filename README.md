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

## Status (6 Oct 2026, export 30cfb26)

867 graphics functions (the owner's IDA run of 4 Oct after `ida/ffxi_ida_fix.py`: 558 names applied, IDA's SP
corrected in 4 functions), each with a stated outcome (`docs/RESIDUE.md`):

| | Count |
|---|---|
| **Verified in the clean tree, as built** (`tree/`, 419 files) | **4,198** of the tree's 4,205, on the fuzz gate run on this build (22 Oct). The four dumps' live gates (`verification/live/`) were run on the 4 Oct tree and have not been rerun on this build (the dumps are not in this environment) |
| In the tree, not verified (`// NOT VERIFIED`) | 7: 0x10040220, 0x1006F0A0 and 0x1006F800 FAIL (as before); 0x1003C7D0 and 0x100CD540 FAIL in the clean build only: both were the gate's errors, since fixed and being re-gated (`docs/PROBLEMS.md` R1, R2); 0x10003890 UNRESOLVED (pop count of the call at 0x10003A50 unknown); 0x10016320 INCONCLUSIVE (the original never returns under the gate's inputs). `tree/left_out.json` lists 51 gate-passed functions left out: each calls a game function that may read a register its wrapper does not pass (49 EAX, 1 ECX, 1 EDX) |
| **Compiled by the game's own compiler to the game's exact bytes** | **1,313** of the tree's 4,205 (22 Oct): the function's tree text, rewritten only as `pipeline/msvc_tu.py` states for Visual C++ 6.0, compiles with the Processor Pack code generator and `/O2` to every byte the DLL holds, relocations by target (`verification/msvc_match_tree.json`). 943 compile to different bytes, 499 do not compile yet, 1,450 are left out with reasons (floating point, address-taken frames, calls through pointers, ...) |
| Microsoft D3DX 8.1 (statically linked) | 48 |
| Constructs C++ cannot express (`__userpurge`/`__usercall` callees, an `__asm` block, the x87 status word) | 11 |
| Did not compile (8), or compiles with no gate case (1) | 9 |
| Gate FAIL | 13 |
| Gate UNRESOLVED (a call with no definitive pop; the call site is named) | 8 |
| Gate INCONCLUSIVE | 1 |

Gates (`verification/`): the fuzz gate (100 random trials per function) and the live gate on each of four
memory dumps of the running game, verified as the analysed build (`verification/live/`). Raw and clean,
combined (`combine_gates.py`: a FAIL anywhere is a FAIL): 774 PASS. Since 6 Oct the gate also compares the data
the Direct3D methods read through pointers (vertex lists, matrices, lights...); that found 7 functions drawing from
uninitialised stack data (fixed) and leaves `Post_DrawRotatingGhostLayers` failing on the sign of a zero. Each function's `// GATE:` line lists
every gate's verdict and returning trials. `pipeline/verify_all.py` re-derives every committed fact from the
binary and re-gates the tree as committed (`verification/verify_all.json`, 6 Oct). It fails on exactly two known
checks: `tree-gate`, on the 3 functions above, and `export-sp`, IDA's SP in the epilogue of 0x100412B0: the real depth (exact, and observed running the original) is 4 less than IDA's from
0x100419F4. IDA's error is on a path that never ran and has an unsolved call, so no correction can be named.

The open functions, each with a named cause (`docs/RESIDUE.md`):
- **FAIL (13):** all among the newly exported device users, none a function that passed before. 10 have an
  x87 comparison Hex-Rays left as condition variables it never assigns (`(c0 | c2) != 0`): the original stores
  the value between `fcom` and `fnstsw` (`fcom; fst; fnstsw ax; test ah, 5; jp`). Which C value each compares
  is not in the export.
- **UNRESOLVED (8):** calls with no definitive pop count (named per call site in `docs/RESIDUE.md`).
- **Constructs (11) and compile failures (9):** listed with the construct or the compiler's message.

One result rests on a declaration the owner accepted rather than a proof: `CDx+0xC` holds the device
(`data/com_fields.json`, "accepted"); every call and stack balance that uses it carries the note.

De-artifacting (types and names from evidence, so the tree reads like source) is planned in
`docs/DEARTIFACT.md`; `pipeline/artifact_count.py` measures it per round.

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
