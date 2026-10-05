# History

The dated record of the reconstruction: what each round did, what it found, and the errors it fixed.
How the pipeline works now is in `METHOD.md`. The earlier reports this replaces, and the IDA exchange
folder of 30 Sep - 2 Oct, are in the git history (commit `3b1bb71` and before).

## Where the errors came from

Almost every error had one root: the binary does not record how much an indirect call pops or what type a
value has. IDA guessed: its stack pointer, Hex-Rays' argument counts, `_DWORD` with no sign. The gate
guessed too: pop counts by counting pushes, then a search for a count that "worked". Each wrong guess
surfaced somewhere else:
- a stack slot with two names;
- a call given the next call's arguments;
- a loop compare that went unsigned;
- a FAIL that was the gate's own model.

Since 3 Oct only definitive sources are used (`CLAUDE.md`, `METHOD.md` section 2). Two rules found in the
solver to be assumptions were removed, not tuned:
- a tail `jmp` does not always leave at depth 0;
- a switch table's length is the bound in front of it, not "while the entries look like code".

## Progress

| Date | Verified in the clean tree | Raw gate (PASS / FAIL / INCONCLUSIVE / UNRESOLVED) | D3DX | Inexpressible |
|---|---|---|---|---|
| 1 Oct, first pass | 496 (+8 not verified) | 504 / 54 / 63 / - | 94 (45 really D3DX) | 35 |
| 1 Oct, second pass | 620 | 624 / 42 / 32 / - | 47 | 9 |
| 1 Oct, evening (placement, names) | 633 | 633 / 43 / 33 / - | 47 | 9 |
| 2 Oct, evening | 681 | 681 / 1 / 30 / - | 47 | 9 |
| 3 Oct, definitive facts only (`def2`) | 700 | 700 / 2 / 1 / 11 | 48 | 5 |
| 4 Oct, third IDA pass (`r5`) | 706 | 706 / 2 / 1 / 6 | 48 | 5 |
| 4 Oct, fuzz and live combined | 708 | 708 / 2 / 1 / 4 | 48 | 5 |

## 30 Sep: IDA cross-check

IDA 9.4 was driven through an MCP bridge against the unpacked DLL. It gave the first independent check of
the earlier pipeline's function discovery and calling conventions. The pipeline's results were then written
into the IDB (`FFXiMain_recon.i64`).
- **Function starts:** 18,723 of IDA's starts agreed before adjudication, and 20,906 after it. Each
  disagreement family was sampled and settled:
  - IDA had missed data-referenced functions;
  - IDA had merged tail-jumped functions;
  - about 540 FLIRT false positives came from VS2010 runtime signatures in a VC6 binary;
  - 13 switch tables were mistaken for functions by the pipeline.
- **Stack cleanup** from `ret N` agreed with IDA's purge analysis on all 17,441 functions both bound.
- **ECX/thiscall:** five faults in the old `cc.py` were found and fixed:
  1. a pass-through `this` was not counted as a use;
  2. tail-wrapper inheritance overrode the wrapper's own evidence;
  3. a prologue `push ecx` was counted as a read;
  4. ECX-preserving helpers were treated as clobbering it;
  5. ESP was lost after `__alloca_probe`, giving one function 199,558 arguments. That entry hung IDA.
  The fix changed 1,001 `this` decisions; the gate-verified code was unaffected.
- **Written to the IDB:**
  - 2,581 functions created;
  - 242 class structs, every size matching the binary's descriptors;
  - 2,107 prototypes;
  - 480 dummy or false-positive names replaced.

## 1 Oct: branching graphics code

The earlier tree covered only branch-free functions, about 20% of the code. This round made Hex-Rays the
front end and the equivalence gate the judge.
- **The device:** `g_pD3DHolder` (D3D at +4, device at +8) and `g_pCDx` (0x1045666C, built in 0x100161B0).
- **D3D8 types in the IDB:** 12 COM interfaces and 11 enums from `d3d8.h`.
- **Conventions:** 100 register-only conventions IDA had invented were retyped.
- **New in the gate:** fake D3D8 objects, fuzzed stub returns replayed on both sides, width masks, and
  stack addresses compared as such.
- **Result:** 504 functions passed as decompiled; the clean tree passed 496 as built.
- **Mutation test:** 30 of 40 mutants rejected. The survivors were coverage limits.

**Second pass on the leftovers.**
- **D3DX:** only 45 of the 94 "library" functions were D3DX. They are byte-identical to `d3dx8.lib` of the
  DirectX 8.1 SDK (`libmatch.py`: 825 matches). The other 49 were game code, filed as library because they
  called something FLIRT had misnamed.
- **The 35 "inexpressible":**
  - 2 were regex false positives;
  - 11 were invented register conventions;
  - 18 were `__asm` where Hex-Rays had lost the x87 stack: the FPU helpers and float returners were typed
    (`fpret.py`);
  - 9 remained.
- **Gate artefacts removed:**
  - demand-paged data;
  - real class vtables for `this`;
  - per-import argument counts;
  - variadic callees compared on their fixed parameters only;
  - excess precision compared at the declared width.
  None of these can turn wrong code into a PASS.
- **Localisation:** `localize.py` gives each FAIL's first divergence and the source line.
- **Repair rules applied in IDA:**
  - COM call-site types from the vtable member;
  - COM call purges, kept only where the decompiled result got strictly better;
  - fake `__thiscall` removed;
  - `__spoils` where a callee keeps a scratch register;
  - return widths.
- **Result:** 624 PASS as decompiled, 620 in the clean tree. Mutation test: 33 of 40 rejected
  (`verification/mutants_clean.json`).

**Parallel session's tree, reviewed.** A parallel session had hand-edited the 504-function v1 tree. A
section-by-section comparison with relocations masked showed its edits were naming and placement only.
- **Names:** 295 of its 317 names were applied in the IDB. 22 describe a call rather than a meaning, so
  they were kept as comments.
- **Placement:** it was re-derived from evidence for all functions rather than copied (`METHOD.md`
  section 5). 29 of its 56 moves agree.
- **Weak evidence:** its 8 "operates on CTsZoneMap" moves were settled one by one. 3 have new evidence:
  their callers load `g_pZoneMap`, which the CTsZoneMap constructor creates.

**Names:** 545 functions were named from the code (262 high, 283 medium confidence; `NAMES.md`).
**Tree:** 633 functions, all PASS as built.

## 2 Oct: the gate gives callees only values they can produce

This round worked without IDA, from the uploaded IDA listing and the DLL.
- **Stubs:**
  - return kinds (`ret_kinds.py`);
  - x87 pushes only where the caller reads them (`fp_callsites.py`);
  - `fpret.py` corrected. 85 functions IDA typed as float returners are proven to return nothing on the
    x87 stack.
- **Translator rules:**
  - exact float constants;
  - the x87 status word;
  - unconsumed float callees;
  - `_DWORD` signedness;
  - exact stack frames;
  - in-place buffers;
  - type parsing (712 of 712 compile).
- **Hand corrections as data:** `hx_patches/` and `type_fixes.json`.
- **Evening, every FAIL to its root cause:**
  - **Hex-Rays and IDB errors** (9): misframed arguments, 16-bit parameters typed `int`, an untyped
    `D3DMATERIAL8`, a matrix returned in EAX but typed `float`, parity tests on never-assigned flags, split
    corner arrays, NaN compares, and IDA's SP 4 bytes short in `CXiSkeletonActor_OnMove`.
  - **The gate's model** (5): it compared stale cdecl slots, executed a fuzzed vtable, fuzzed a ring index
    past its wrap, could not perform a getter's write, and had an allocator return NULL. This led to
    `cdecl_args.py`, `input_ranges.json`, `leaf_funcs.py` (5,112 leaf functions run for real) and
    `allocators.json`.
  - **Hidden by stubs:** running leaves for real exposed three real translation bugs.
- **Night:**
  - `Dancer_InitDevice` fixed (`filled_past_array`);
  - `real_sp.py` measures stack depth on real code;
  - the first `ida/ffxi_ida_fix.py`;
  - the clean tree 681/681.
- **Live gate on the 7 Sep dump:** 659 PASS, 51 INCONCLUSIVE, 2 FAIL. That dump is of the 5 Aug build,
  mapped to the analysed build by `build_map.py`, partly by nearest match. Its result
  (`gate_live_latest.jsonl`, commit `3b1bb71`) was superseded by the same-build dumps of 3 Oct and
  removed.
- **Names:** 193 globals and callees, and 77 fields (`NAMES.md`, `data/field_names.json`).

## 3 Oct: first IDA run, then definitive facts only

**First IDA run.** It applied 185 names, 7 prototypes and the viewport types, and set SP points in 10
functions.
- **Now passing:** OnMove and `CTsZoneMap_DrawProjectedShadow` (fuzz and live), plus three functions that
  had not translated before.
- **Three of my mistakes, all fixed:**
  1. 10 of the 16 SP points were wrong. Their depths came from the emulator's fake objects and stubs, not
     the game. `real_sp.py` now trusts only real code, and the next run reverted those points.
  2. The D3DX CPU-dispatch thunks (`jmp [table]`) were mistyped. `purges.py` now resolves all 58 to their
     default implementation's `ret N`.
  3. Hex-Rays takes an untyped indirect call's arguments from the pushes in front of it.

**Definitive facts only.** This round brought in the owner's rule (`CLAUDE.md`):
- `sp_solve.py` and its self-test;
- `vcall_resolve.py`;
- `callsite_audit.py`;
- UNRESOLVED as a verdict;
- Win32 calls answered as documented;
- a thread information block at FS.
The push-counting fallback and the search for a count that "works" were removed. Full gate: 700 PASS,
11 UNRESOLVED, 2 FAIL, 1 INCONCLUSIVE. All 685 PASSes of the previous run still pass.

The tree was regenerated: 700 functions in 187 files, all PASS as built. Two pipeline faults were found and
fixed:
- a stage run against the wrong export (`run.sh` now checks the SHA-1);
- a type fix applied by halves (it now sets the convention and the parameter list).

## 3-4 Oct: third IDA pass, same-build dumps, proven D3D calls

- **Third IDA pass** (`data/exports/gfx_export.jsonl`):
  - 6923 calls typed in the IDB, 777 cast by Hex-Rays, 889 comparisons with their signedness;
  - all 3845 exact call effects already agreed with IDA's SP.
- **`callsite_args.json` had been empty,** so the `typed` source had never applied. It was rebuilt (7700
  sites).
- **Same-build dumps:**
  - four ProcDump dumps (town, town2, combat, zone to inn) of the analysed build;
  - `dump_verify.py` shows every code byte matching except 4 run-time-patched entries, none graphics;
  - the DLL's relocation directory covers only the last 87 of the 893 blocks, so the full chain is read
    from the section start;
  - live mode now uses only definitive pops, with a sound purge walk into other modules.
- **`com_calls.py`:** D3D8 calls whose object is a proven field (`com_fields.json`, checks recomputed every
  run). 217 calls are proven; the solver gains 31 agreements and 139 new calls.
- **`sp_solve.py`:** two-level MSVC switch tables, with label guards.
- **Fixes:**
  - `ProcessLoadState` (0x100D70A0): an unassigned ECX variable was passed as `this`;
  - `Gfx_InitCubeMapTextures` (0x10075280): GetDesc was cast `__cdecl`, so the stack slots were shifted;
  - 0x10070D80: a thiscall thunk, through the type fix;
  - `sub_1006C530`: four `fild dword` conversions are signed and Hex-Rays had typed the sum `_DWORD`;
  - a `self` name collision in four functions.
- **r5:** 706 PASS, 6 UNRESOLVED, 2 FAIL (`sub_1017DE20`, `sub_100548D0`), 1 INCONCLUSIVE
  (`sub_10016320`). The clean tree (188 files) passes all 706 as built and was promoted. Building it found
  a global the IDB names `C` that a newly passing function also uses as a parameter; globals are macros in
  the tree, so any global named like a parameter or local now gets a distinct name (`g_C_106631D4`).
- **Repo reorganised:**
  - one export;
  - the IDA-side scripts in `ida/`;
  - D3DX data in `data/d3dx/`;
  - the dated reports merged into `METHOD.md`, `HISTORY.md` and `NAMES.md`;
  - the IDA exchange archive, the parallel session's bundle and superseded data removed (in git history).
- **Live gates** on all four dumps, raw and clean (`verification/live/`):
  - Raw: 704 PASS per dump. The clean tree: 704 PASS and 4 INCONCLUSIVE per dump, 0 FAIL.
  - They found `CXiSkeletonActor_Slot244_Unknown` wrong for NaN inputs, after the fuzz gate had passed it.
    The original's `test ah, 5; jp` is taken for unordered compares; Hex-Rays' `a >= b` is not. It was
    patched, and `fcmp.py` now computes each such branch's outcomes from the code, for a rule over the
    next export.
  - Combined with the fuzz gate: 708 verified. `Actor_SelectNearestLights` and `sub_10003890` (3 returning
    trials per dump; marked thin) pass only on real state.
- **`vcall_resolve`** follows switch tables only where bounded. 180 functions earlier claimed never to read
  ECX are now unknown.

## 4 Oct 2026 (later): r8 -- 714 verified, no FAIL, no INCONCLUSIVE

- **Fourth-pass export** (IDA SP exact at all 3841 checked calls; `CDx_CreateTexture` settled through the
  accepted `CDx+0xC` declaration, tagged everywhere it is used).
- **Gate:** the fuzzer draws its pools in bulk (it was ~90% of gate time). NaN-exact x87 branches: `fcmp.py`
  exports each `test ah, N; jp/jnp` branch's taken outcomes from the code and `hx_patches.fcmp_exact`
  rewrites Hex-Rays' condition where it differs (96 conditions in 21 functions).
- **r8:** fuzz 712 PASS, 6 UNRESOLVED; with the live gates on four dumps 714 PASS, 4 UNRESOLVED. The clean
  tree (189 files) passes all 714 in every gate. `sub_1017DE20`, `sub_1017C9C0`, the three `__asm`
  fallbacks, `CDx_CreateTexture` and `sub_10016320` now pass. Promoted.
- **verify_all.py:** every committed fact re-derived from the binary and compared; rule tests and the
  solver self-test; the tree regenerated byte for byte from the promoted gate and re-gated. It found the
  committed `fpret.json` stale (0x10103BA0 returns through `_ftol` in EAX); replaced.
- **Leftover registers:** 25 reads of never-assigned ECX/EDX variables classified from the code
  (`reg_leftover_audit.py`, `edx_inputs.py`): 3 field writes, 13 harmless, 9 not proven (listed).
- **De-artifacting started** (`docs/DEARTIFACT.md`): class types pushed to the IDB (`make_ida_types.py`,
  119 classes, 354 proven fields); `artifact_count.py`; the IDA script reverts a typed `this` that makes a
  function's pseudocode worse (measured). Exact gate cache (`gate_cache.py`).

## 5 Oct 2026: the first in-game crash, and the Hex-Rays defects of its kind

- **Crash:** pol.exe, c0000005 at FFXiMain.dll+0x19230 (`CXiTimer::GetFps`, `fld [ecx+8]`) as soon as the
  kit swapped in `Frame_Render`. Its two calls of `GetFps` through the timer's vtable were typed by Hex-Rays as
  a cast without `this`, so the reconstruction called them with whatever ECX held. The gate had passed it: its
  compared trials ran 53 of the function's 1424 instructions.
- **Coverage** (`FFXI_COVERAGE=1`, `combine_coverage.py`): which of the original's instructions the compared
  trials ran. The kit leaves out by default every function not fully covered (manifest flag 4).
- **ECX at calls:** `ecx_this_sites.py` (the original enters a virtual method with ECX = the object) and
  ECX compared at direct calls to `type_fixes` `__thiscall` functions.
- **The same class across the tree**, each fixed as data with its disassembly evidence:
  - ECX lost at direct calls: `Frame_Render` -> 0x101C4CE0 (dereferences it) and 0x10014A20,
    `CApp_RunMainLoop` -> 0x10014C10;
  - a register the original keeps live across a call to a callee that leaves it alone
    (`reg_kept_audit.py`): the D3D device in EDX in 0x1006AA30 (2 calls), `this` in ECX in `DrawMesh`
    0x10040670;
  - x87 compare flags never assigned (`(c0 | c2) != 0`): four angle wraps in 0x10125360;
  - stack arguments the IDB's type leaves out, named as locals in the argument slots: `CXiMovie_DrawFrame`
    (a rectangle and a float pointer), and `Frame_Render`'s call of it;
  - a struct out-parameter Hex-Rays split into separate locals (`read_past_byref`): 0x10042020,
    0x1001D940, 0x1028AB50.
- **Fact tools corrected:** `ecx_use`/`edx_use` (`or r,-1`, byte-mask `and`), `ecx_this_sites` (control
  flow, joins), `cdecl_args` (callee pops, control flow, the EBP slot formula, `_chkstk`; lower bounds
  instead of dropping a function; 525 of the changed counts now agree with the IDB).
- **Gate:** stubs compare up to 16 arguments (was 4), cdecl argument counts from the code's reads, masks per
  stack slot. Fuzz: 774 PASS, 2 UNRESOLVED, 1 INCONCLUSIVE, unchanged.
- **Kit:** `include_untested`, and a fault in game code called from a reconstruction quarantines the innermost
  reconstructed caller as `suspected`.
- **Checked and not defects** (from an outside audit): the three `CMoSchedularTask_Interpret` sites
  (`push ecx; mov ecx, esp` reserves a slot the constructor 0x1005E110 zeroes) and `App_InitScene` (ECX
  left over, overwritten before use).
