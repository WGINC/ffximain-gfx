# data/

Every input the pipeline reads, grouped by what it is. "From" names the tool in `pipeline/` that writes the
file (rerun it to regenerate); "carried over" means it was made before this repo or by hand, and nothing
here regenerates it.

## The export

| File | What | From |
|---|---|---|
| `exports/gfx_export.jsonl` | Hex-Rays export of the 768 graphics functions (third IDA pass, 3 Oct) | `ida/ffxi_ida_fix.py` |
| `exports/ida_fix_report.json`, `ida_fix_log.txt` | What that IDA run changed and checked | `ida/ffxi_ida_fix.py` |

## Facts read from the DLL (definitive sources, `docs/METHOD.md` section 2)

| File | What | From |
|---|---|---|
| `func_extents.json` | Function extents of the whole image (21,410, the IDB's after 30 Sep) | carried over (IDB) |
| `purges.json` | Each function's stack pop from its `ret N` | `purges.py` |
| `sp_solved.json` | Indirect calls' pops solved from the caller's stack balance | `sp_solve.py` |
| `vcalls.json` | Virtual calls whose every possible callee is proven | `vcall_resolve.py` |
| `ecx_use.json` | Functions that read their entry ECX, with the instruction | `vcall_resolve.py` |
| `edx_use.json` | Functions that read their entry EDX, with the instruction | `edx_inputs.py` |
| `ecx_this_sites.json` | Virtual calls where the original enters the callee with ECX = the object, and whether it is also pushed | `ecx_this_sites.py` |
| `reg_kept_audit.json` | EAX/ECX/EDX values the original keeps live across a call to a callee that leaves them untouched, and where it uses them | `reg_kept_audit.py` |
| `com_calls.json` | D3D8 calls on a proven object, with the header's pop | `com_calls.py` |
| `real_sp.json` | Stack depth measured running real code | `real_sp.py` |
| `game_sp.json` | What a callee pops at call sites the gate could not settle, measured in the running game (xikit `facts=1`); recorded only where every observed call agreed (not re-derivable: an observation) | `fact_sites.py ingest` |
| `callsite_args.json` | Each call's type as the IDB declares it (`typed`) or Hex-Rays made up (`cast`) | `callsites.py` |
| `callsite_audit.json` | All sources side by side for every indirect call | `callsite_audit.py` |
| `ret_kinds.json` | EAX at `ret`: 0/1, or AL 0/1 | `ret_kinds.py` |
| `fpret.json`, `fp_callsites.json`, `fp_consts.json` | x87 returns, results callers consume, exact constants | `fpret.py`, `fp_callsites.py`, `fpconst.py` |
| `cdecl_args.json` | Argument slots each cdecl function reads | `cdecl_args.py` |
| `leaf_funcs.json` | Small leaf functions the gate runs for real | `leaf_funcs.py` |
| `import_nargs.json` | Argument counts of 293 typed imports | carried over (IDB) |

## Proofs and hand corrections (each entry carries its evidence)

| File | What |
|---|---|
| `com_fields.json` | Access paths proven to hold a D3D8 interface; the checkable parts are recomputed by `com_calls.py` on every run |
| `hx_patches/0xADDR.json` | Exact-text corrections of Hex-Rays output, tied to the export text's SHA-1 |
| `type_fixes.json` | Prototype and data-type corrections (also pushed to the IDB by the IDA script) |
| `ida_sp_revert.json` | SP points an earlier IDA run set wrongly, for the IDA script to remove |
| `names_pending_idb.json`, `field_names.json` | Names applied by the generator (and pushed to the IDB); field names for classes with no IDB struct |

## The gate's input model

| File | What | From |
|---|---|---|
| `protos_fixed.json` | Prototypes of every function, for stubs and cases | carried over |
| `class_vtables.json`, `this_com.json` | A class's real vtable for a fuzzed `this`; `this` that is a D3D object | carried over |
| `input_ranges.json` | Value ranges the game keeps (table counts, record sizes), each with its evidence | by hand |
| `allocators.json` | Allocator functions: stubs return fresh zeroed blocks | by hand |
| `this_sources.json` | Where a method's `this` comes from (for live inputs) | carried over |

## Class model and placement (`docs/METHOD.md` section 5)

| File | What | From |
|---|---|---|
| `class_model_include/` | The class model's headers (PS2 DWARF and vtables) | carried over |
| `tree_manifest.json` | Class placement from the class model | carried over |
| `units.json` | Source unit of every function | `unit_place.py` |
| `unit_evidence.json`, `units_callers.json` | The anchors and caller evidence `unit_place.py` uses | carried over |
| `this_votes.json`, `global_objects.json` | Callers passing their own `this`; globals whose creating code shows their class | carried over |
| `ida_named_fields.json`, `gfx_types_game.h` | IDB struct field names; game types printed from the IDB | carried over (IDB) |
| `dm_names.json` | DancingMad names and descriptions | carried over |

## Direct3D and D3DX

| File | What | From |
|---|---|---|
| `com_spec.json` | D3D8 interfaces: each method's slot and parameter count, from the SDK header | carried over (header) |
| `d3d8_ida.h`, `d3d8_methods.json` | D3D8 declarations for the IDB; method order per interface | `gen_d3d8_decls.py` |
| `d3d_values.json` | D3D enum values for naming arguments in the clean tree | carried over (header) |
| `d3dx/d3dx81_manifest.json` | The 825 functions byte-identical to `d3dx8.lib` (DirectX 8.1 SDK) | `libmatch.py` |
| `d3dx/d3dx81_bind.hpp` | The 46 public D3DX entry points bound to their game addresses | `gen_d3dx_bind.py` |

## Coverage

| File | What | From |
|---|---|---|
| `gfx_extension.json` | The 267 graphics-related functions outside the set, by tier | `gfx_coverage.py` |
| `../verification/coverage_clean.json` | Per function, the original's instructions and calls no compared gate trial reached, over the fuzz and live gates | `combine_coverage.py` (gates run with `FFXI_COVERAGE=1`) |
