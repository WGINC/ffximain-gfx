# FFXiMain.dll graphics reconstruction (build 2026-09-03)

Gate-verified C++ for the graphics code of the FFXI PC client. It is produced from IDA Pro / Hex-Rays
output (and, for functions IDA has not exported yet, from Ghidra). Each function is checked against the
original in an emulator.
Private work: nothing here is published, and no game bytes are in the repo.

## Layout

| Path | What |
|---|---|
| `tree/` | The deliverable: clean, class-shaped C++ (`include/`, `src/`, `build.sh`). Each function carries its `// GATE:` result |
| `d3dx/` | The statically linked DirectX 8.1 D3DX code: a manifest of byte-matched functions and a binding header (see *Coverage* for the manifest's limits) |
| `verification/` | Gate results (raw and clean), cases, first-divergence localisation, mutation test |
| `docs/` | Reports (`REPORT-2026-10-02.md` is current), `RESIDUE.md` (every function not in the verified tree, with the reason), `PLAN.md`, the name reports |
| `pipeline/` | Translator (`hx2cpp.py`, `hx2clean.py`), gate (`diffemu_gfx.py`, `gate_some.py`), IDA side (`ida_gfx.py`, `ida_apply_names.py`), Ghidra side (`ghidra/GfxExport.java`), and the tools below |
| `data/` | Inputs. The Hex-Rays export; facts read from the DLL (`fp_consts`, `ret_kinds`, `fp_callsites`, `fpret`, `purges`); names not yet in the IDB (`names_pending_idb.json`, `field_names.json`); hand corrections (`hx_patches/`, `type_fixes.json`, `ida_sp_fixes.json`); the gate's input model (`input_ranges.json`, `allocators.json`, `leaf_funcs.json`); the coverage list (`gfx_extension.json`); class vtables, prototypes, the D3D8 spec, DancingMad names |
| `incoming/` | The parallel session's renamed/organised tree: reviewed, merged (`docs/REVIEW-parallel.md`), kept for provenance |
| `runs/` | Scratch output of `run.sh` (ignored, like every `*.dll`) |

## Reproduce

    export FFXI_DLL=/path/to/FFXiMain_unpacked.dll   # never copied into the repo
    ./run.sh all data/exports/gfx_export_current.jsonl myrun

`run.sh` runs the whole pipeline under `runs/myrun/`:
1. reads its facts from the DLL;
2. translates the export and builds one unit per function;
3. gates every function;
4. generates the clean tree from the passing ones;
5. gates that tree as built and annotates it;
6. writes `RESIDUE.md`.

Steps can be run one at a time: `./run.sh facts|translate|cases|gate|clean|cleangate|residue`.

The gate runs as a work queue with one worker per core (`$JOBS`). A full gate takes about 8 minutes on 4 cores.
To gate only part of it, use `pipeline/gate_some.py RUN fail|notpass|changed:DIR|0xADDR...`.

**Requirements**
- Python 3 with `unicorn`, `pefile` and `capstone`.
- MinGW-w64 i686 (`i686-w64-mingw32-g++`).
- IDA Pro 9.x with Hex-Rays, for new exports.
- Optionally Ghidra 12 with JDK 21 (see *Coverage*).
- `gen_d3dx_bind.py` needs `DX81_INCLUDE` pointing at the DirectX 8.1 SDK headers.

## Status (2 Oct 2026, evening)

768 graphics functions, every one with a stated outcome (`docs/RESIDUE.md`):

| | Count |
|---|---|
| **Verified in the clean tree, as built** | **681**, 0 failures |
| Microsoft D3DX 8.1 | 47 |
| Constructs C++ cannot express | 9 |
| Gate FAIL (first divergence recorded) | 1 |
| Gate INCONCLUSIVE (the original never returns under fuzzed inputs) | 30 |
| Do not compile | 0 |

Placement of the 681 tree functions:
- **507 are class members:** 456 instance methods and 51 static methods.
- **174 are free functions,** each in the file of its source unit: 147 in units identified only by link order (`unit_*.cpp`) and 27 in named units.

The one FAIL, `CXiSkeletonActor_OnMove`, is an IDA stack-pointer error, not a translation error: IDA's SP is
4 bytes short between two indirect calls (0x100C6693..0x100C7902), so Hex-Rays gives one stack slot two
names. The correction is in `data/ida_sp_fixes.json`; apply it in IDA (`ida_apply_names.apply_sp()`) and
re-export the function.

**Latest raw-tree gates (dev7, after commit `60b62b3`)**, recorded in `verification/gate_raw_latest.jsonl` and
`verification/gate_live_latest.jsonl`. The clean tree has not been regenerated from these yet:
- **Fuzz gate:** 680 PASS, 30 INCONCLUSIVE, 2 FAIL. The second FAIL, `Dancer_InitDevice` (0x10251A80), is a
  regression from the last pipeline changes and is still to be traced.
- **Live gate** (inputs taken from a memory dump of the running game, `FFXI_LIVE`): 658 PASS, 51
  INCONCLUSIVE, 3 FAIL. The FAILs are `CXiSkeletonActor_OnMove` (IDA SP error, above),
  `CTsZoneMap_DrawProjectedShadow` (Hex-Rays names stack slots 12 bytes off in its later region; needs an
  IDA-side fix and re-export) and `Dancer_InitDevice` (the same regression). Most INCONCLUSIVE results are
  sqgx/sqsn functions, which are idle in the dump (`g_pDancerDevice` is null) and pass under fuzzing.

On 1 Oct the clean tree had 633 functions, with 43 FAILs and 3 that did not compile. The repairs since,
to the translator, the gate model and the data, are described in `docs/REPORT-2026-10-02.md`.

Names:
- **1 Oct:** 545 functions were named from the code (`docs/NAMES-2026-10-01.md`).
- **2 Oct:**
  - 195 globals and callees were named (`docs/NAMES-2026-10-02.md`);
  - 90 fields were named (`data/field_names.json`).
- These names, and the type and SP corrections in `data/`, are applied by the generator but are **not yet
  in the IDB**: run `pipeline/ida_apply_names.py` in IDA (it applies names, `type_fixes.json` and
  `ida_sp_fixes.json`), then re-export.
- Still unnamed in the tree: 198 distinct `sub_` names (functions and callees) and 233 `field_XXX` members.

## Coverage: is every graphical system here?

**Every function that touches a Direct3D device is in the set; not every graphics function is.** `pipeline/gfx_coverage.py` re-derives coverage from the DLL alone:

- **Direct device users: all covered.** Every function that reads or writes a device holder
  (`g_pD3DHolder`, `g_pCDx`, `g_pDancerDevice`, 0x10669418) is in the 768-function set.
- **Missing: 267 game functions that reach the device only through other graphics code** (`data/gfx_extension.json`, by tier):
  - **Tier 1 (43):** named graphics code, plus callers of D3DX resource code. This includes
    effect-element destructors (`CMo*Elem`), `Font_DrawChar`, `Cell_/Shape_Draw*`, `TexObj_*`,
    `CMoDX_*` matrix and texture helpers, `XiZone_GetAmbient`, the PS2-layer texture bakers
    (`sqsdGeneric/Sampling/Aniso/Glow_BakeOntoShape`, `sqshDraw_Submit`), the D3DX shader assembler
    caller and two texture loaders.
  - **Tier 2 (62):** unnamed functions that call two or more graphics functions.
  - **Tier 3 (162):** functions that call one graphics helper, mostly game logic.
- **Route for the missing ones:**
  - From IDA: `ida_gfx.select_extension('data/gfx_extension.json')`, then `export_step`.
  - From Ghidra, when IDA is not at hand: `pipeline/ghidra/GfxExport.java` decompiles a list headlessly into the same JSONL format. It applies the IDB's names and commits the inferred signatures first.
  - The translator adapts Ghidra's C (`xi_ghidra.hpp`; each callee's argument count comes from its real `ret N`).
  - First run, on the 105 tier-1/2 functions:
    - all 105 decompile and 71 compile;
    - **26 pass the gate**, for example `Font_DrawChar`, `XiZone_GetAmbient`, the `CMoDX` matrix accessors, `sqshDraw_Submit` and the four `sqsd*_BakeOntoShape`;
    - 34 fail and 11 are inconclusive.

    These results are in `runs/` only. They join the tree when the pipeline accepts Ghidra input end to end; until then the IDA export is the route.
- **The D3DX manifest overstates the statically linked library.** It lists 825 matched functions,
  but 16 of them lie inside game code and are not linked library code. They are D3DX's inline math
  helpers compiled into game objects from the SDK headers, and tiny constructors whose 13–46 bytes
  happen to match. None of them are among the 47 D3DX functions counted above, all of which are
  inside the library region and match on at least 38 bytes.

## Tools

| Tool | Use |
|---|---|
| `gate_some.py` | Gate any subset on a work queue; prints each verdict that changed from `verification/` |
| `localize.py` | First divergence of a FAIL, with the source line, and where a crashing reconstruction stopped |
| `disasm.py` | Disassemble a function by address from the DLL (all chunks, with names) |
| `ida_listing.py` | Read an IDA `.asm` listing without IDA: procedures, data and Shift-JIS strings, xrefs, address-to-name map |
| `name_context.py` | The evidence for naming placeholders and `field_XXX` members |
| `gfx_coverage.py` | The coverage audit above |
| `same_code.py` | Are two builds the same code, function by function (renames and moves must be) |
| `fpconst.py`, `ret_kinds.py`, `fp_callsites.py`, `fpret.py`, `purges.py`, `cdecl_args.py`, `leaf_funcs.py` | Facts from the DLL (`run.sh facts`) |
| `stack_depth.py`, `ida_sp_check.py` | The real stack depth at each instruction, and where IDA's own SP (read from its listing) differs: the cause of a stack variable with two names |
| `unset_reads.py` | Locals Hex-Rays reads but never assigns (a lost return value, a misframed argument, an unmodelled condition code) |
| `loop_bounds.py` | Why an original runs to the instruction limit: the loop and the fuzzed bound it compares against |

See `docs/REPORT-2026-10-02.md`, `docs/PLAN.md`, `docs/RESIDUE.md`, `docs/PLACEMENT.md` and the name reports.
