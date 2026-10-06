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

## 5 Oct 2026 (later): the in-game DLL gated on its own, and what it found

- **The shipped DLL gated on fuzz** (`gfx_recon_ingame.dll`, `xi_rel = 0`) failed three functions the clean
  build had passed under the earlier seeding; the clean build fails them too under the current harness:
  - 0x1017DE20: the x87 status word was rebuilt where it is read, from the textually last compare; the two
    branches compare different operands, so a NaN on the other branch was missed (`fpu_status_words` now records
    `xi_fpu_unord` at each compare; 35 functions retranslated);
  - 0x10057FB0 (`CMoSchedularTask_Interpret`): a virtual call typed `__thiscall(float)` - GCC pushed `this`
    and left ECX stale (`thiscall_float_this`);
  - 0x1006B4C0: `nullsub_43` is a bare `ret`, the original returns its argument (hx_patch;
    `reg_kept_audit` now counts a `ret` as a read of EAX and finds no other site).
- **Fuzz inputs**: one draw in eight is a constant the function compares against (`_cmp_consts`), and every
  global read at a fixed address is fuzzed (`case_abs_reads.py`). Two more defects surfaced:
  - 0x100CBDE0: a game function's `bool` return tested together with another condition; GCC assumes 0/1,
    MSVC tests AL (`orig::` wrappers return `uint8_t`);
  - 0x10044270: Hex-Rays printed a writable global's initial bytes as a literal (hx_patch).
- **Result**: fuzz + 4 dumps, 775 PASS, 2 INCONCLUSIVE (0x1006B3B0, 0x10016320: the original never returns under
  the new inputs), 0 FAIL; 620 functions fully covered (was 594). The in-game DLL: 773 PASS on fuzz, 0 FAIL.
- **Kit**: flag 4 also marks a PASS on fewer than 10 compared trials; 620 functions swapped by default.
- **Gate time**: the full gate (fuzz + 4 dumps) is 37 minutes from cold and 5 minutes with the cache when one
  source file changed.
- **Known, not fixed**: verify_all `export-sp` (IDA's SP in 0x100412B0, upstream of calls whose pop is not
  known; the function is left out of the kit as untested); 44 virtual-call sites with unknown pop counts keep
  30 functions partly untested (Frame_Render among them).

## 6 Oct 2026: the second in-game test, and the register the analysis lost

- **What the test showed** (all 620 swapped): no crash at first, then the 3D scene a flat colour, the UI
  "moshing", the screen fading between white and a blue pinstripe pattern; one run crashed at once in
  `FFXiMain.dll+0x4401D` (analysed 0x1004401D), `ecx=0x10`, called from reconstructed `fn_100759C0`.
- **Cause**: `DynVB_Reserve` (0x10044010) and `DynVB_Unlock` (0x100441A0) read their entry ECX (the DynVB object)
  and the IDB types them without `this`; Hex-Rays dropped the object and the reconstruction called them with
  whatever ECX held. Reserve then writes through that pointer (`mov [edx+0x10], ecx` ...): a wild write, which
  would explain the flat colour and the corrupted UI too (not shown; it is the only defect of its kind found).
- **Why the analysis missed it**: `ecx_use`/`edx_use` counted every call as a write of ECX/EDX. The function
  reads its ECX only after `call 0x10043DE0`, which leaves ECX untouched. `reg_preserve.py` (the fixpoint of
  "leaves EAX/ECX/EDX untouched", taken out of `reg_kept_audit.py`) is now used there, and `push ecx` is
  decided exactly where the slot is overwritten first. Result: ECX has no "cannot tell" left; the only wrappers
  that read an ECX they are not given are the two DynVB functions (fixed) and `CreateProcessorInstance`
  (through a callee, known).
- **Fix**: both functions `__thiscall` (`data/type_fixes.json`), the 8 reconstructed callers pass the object the
  original passes (`data/hx_patches`, written from the disassembly: each loads ECX from a `g_pDynVB_*` global just
  before the call). All 8 pass fuzz and the four dumps; with the old tree, 7 of them fail the new ECX comparison.
- **Gate**: a stub whose callee reads its entry ECX (a direct read in `data/ecx_use.json`) compares ECX whatever its
  declared convention (`__fastcall` callees now too: 20 more cases, all PASS). Without this the gate was blind to
  a missing `this` on any callee typed without one.
- **Checks added**: `entry_regs.py` (EAX/EBX/ESI/EDI/EBP read at entry: none of the 1,165 wrappers reads one but
  with a prologue `push`, 13 undecided because of a switch table the analysis cannot bound), and verify_all
  `wrapper-regs`: a wrapper typed without a register it reads fails the build.
- **Kit**: bisect mode (`bisect=1`, Ctrl+Alt+F10/F11/F12), `docs/KIT.md`.
- **Not known**: whether this was the cause of the flat colour and the pinstripe fades. The bisect finds the
  function if it was not.

## 6 Oct (later): whole-frame differential attempted; in-game A/B trace
- **`pipeline/frame_diff.py`**: runs `Frame_Render` from the town dump with the original code only, then again with the
  kit's swap set redirected to the reconstruction, nothing stubbed but other modules' calls. It cannot get through a
  frame: at 0x10009F44 the original calls a method of a real D3D8 object (the interface of that object is not given
  by any source: com_calls proves only the device) and the emulated answer writes no output, so the next
  instruction faults (call 7 of the frame). The callee's own `ret N` cannot be read from the dump (the D3D8.dll code
  pages hold no readable code). `--assume-pops` guesses unresolved pops from the pushes and is labelled exploratory;
  it is not a verdict source. What would make the frame run: the interface of every D3D object the frame touches and
  what each method writes through its out parameters.
- **A/B trace** (`kit/d3dprof` trace, xikit `trace=1`, Ctrl+Alt+F8, `docs/KIT.md`): the real game's Direct3D calls with
  and without the swaps, compared by call site (`trace_diff.py`). Tested under Wine and in the thunk test.


- **First A/B traces from the game** (7428, three traces): compared by call site every swapped function looked different (the
  call comes from reconstruction addresses): an artefact, fixed in `trace_diff.py` (content comparison). Compared by
  content, traces 1 and 2 (same scene) show the same Direct3D state and draw stream with 620 swapped as with none,
  within the scene's own change (about 21,000 calls per frame, a few dozen tokens differ). Trace 3 is a different scene
  (calls fall from 15,800 to 15,000 within the unswapped frames) and says nothing. Vertex-buffer and texture contents
  are not in the trace. `Gfx_DestroyD3DHolder` faults every run in xikit.dll's data area (g_vt region): left out of the
  shipped ini (`exclude=`), cause not found.
- **The owner's report on trace 7428**: the picture was right while the swaps were off for the trace and wrong again
  once they were back on. The traced device calls are the same in both halves, so the difference is in data the trace
  did not see. The trace now records vertex and index buffer contents at `Unlock`, by the function that locked them
  (`trace_buffers=1`), tested in `test_thunks.py` (Lock/Unlock pops, size from GetDesc, hash, call site).
- **Rig runs (6 Oct, owner)**: swapping only the 87 functions that ran at load gave a right picture, the 149 that run
  every frame a broken one. A bisect narrowed it to 7 `CDx` functions (GetRenderTargetTop, PushRenderTarget, SetTexture,
  DrawTexturedScreenQuad, PushRenderTargetRet1, DrawFadeQuad, SetTransform): broken with all 7, right with every group
  the bisect tried alone ({GetRenderTargetTop, PushRenderTarget, SetTexture}, {DrawTexturedScreenQuad,
  PushRenderTargetRet1}, {DrawFadeQuad}, {SetTransform}). So two or more break it only together; the one-culprit
  bisect could not find that. All 7 pass every gate with every instruction covered, also on the shipped in-game DLL and
  with the x87 stack depth compared at every return; no branch enters their first 5 bytes. The bisect is now ddmin with
  leave-one-out below 9 suspects (simulated over every 1-, 2- and 3-function culprit set of 7: each found exactly, at
  most 7 answers; Wine test: a single culprit and a pair).

## 6 Oct (late): the picture fault found and fixed; the gate compares the data behind pointers
- **Culprit**: the kit's new bisect (ddmin + leave-one-out) named `CDx::DrawTexturedScreenQuad` (0x1000A5A0). The
  original builds the full-screen quad's 4 vertices (112 bytes) as one block on its stack and passes its address to
  `CDx::DrawPrimitiveUP`; Hex-Rays split the block into 28 scalars, and GCC kept the first and dropped the other 27 as
  dead stores (seen in the compiled code of both the gate and the in-game DLL), so the quad was drawn from stack garbage.
- **Why the gate passed it**: it compared the pointer as "a stack address" only. It now compares the data the D3D8
  methods read through pointers (UP draws, transforms, viewport, material, light, clip plane, shader constants; sizes from
  the D3D8 header) and the data `CDx::DrawPrimitiveUP` (0x1000CD00) forwards to the device (its own code). With that, the
  promoted tree failed 7 functions this way (DrawTexturedScreenQuad, DrawScreenQuadAtDepth, Gfx_DrawScreenQuad256,
  ProjTex_CaptureToTarget, Mem_DrawHeapUsageBar, Gfx_DrawSolidRect2D, Gfx_DrawQuadOutline).
- **Fix**: `byref_blocks.py` finds a BYREF local whose block (to the end of the frame, from the export's [esp+X] [ebp-Y])
  holds a write-only local or undeclared bytes; that is a fifth trigger of `exact_frame` (locals at their original
  offsets). 71 functions in the export, 10 of the tree changed; all pass. (A first attempt as packed structs broke a
  function whose locals alias one slot; dropped.)
- **x87 intrinsics** on `long double` keep 80 bits (overloads in xi_decomp.hpp).
- **Gate of the promoted tree** (fuzz + four dumps, data compared): 774 PASS, 2 INCONCLUSIVE (as before), 1 FAIL:
  `Post_DrawRotatingGhostLayers` (0x10038E10, flag 4, never swapped by default), whose vertices carry -0.0 where the
  reconstruction writes +0.0; cause not found. 620 fully covered. Kit: 774 listed.
- **Trace** records the bytes of matrices and vertex lists up to 128 bytes; `trace_diff.py` prints them for calls that
  differ. The owner's trace with the interim kit (5304): 8 call variants change with swaps on against 66 between
  unswapped frames; one world transform (from a matrix at another address) and one UI quad differ every frame.

- **Traces 9176_1 and _2** (the final kit, one scene, 30 s after start): every call that differs with swaps on is the
  scene's own: an 8-dot ring indicator a quarter-second further in its animation, one world matrix whose translation
  moves (it also moves between swapped frames), light 1 (differs between every unswapped frame too), the fog end
  (drifts in the unswapped frames too), and stack addresses of `GetTransform`/`SetLight` data. Skinned vertex buffers
  cannot be compared this way (animated every frame).
- **Alternating trace** (`trace_alternate=1`, default): 17 frames with the swaps flipped at every Present, labels set
  after `apply()` (2 = a mixed frame, left out); `trace_diff.py` tests each swapped frame against its two unswapped
  neighbours (STABLE / OUTSIDE / COUNT) with the same test on unswapped frames as a control. Same-frame replay is not
  possible in game: a frame advances the game's own state.
- **Alternating traces 11000_1..4** (owner's session; scene not stated; 8 swapped frames each, 5 in trace 2): no call of a swapped frame
  differs from its two unswapped neighbours more often than the same test fires among unswapped frames, except in
  trace 4: five render-state/transform groups one call longer in frame 9 and one shorter in frame 15 (one object
  more, then one fewer: opposite directions, the scene's object count). Buffer creations (CreateTexture from
  0x102D3013, CreateIndexBuffer) fell in swapped frames 3 times, CreateVertexBuffer in unswapped frames twice: loading,
  too few to tell. Trace 2 hit the 600000-record limit (a busy scene, about 35000 calls a frame). Undecided calls were
  almost all `SetLight` (hash only).
- **Trace**: up to 1500000 records (falls back to fewer if the address space has no room); `SetLight` keeps its 104
  bytes, so an animated light is checked as between its neighbours; `trace_diff.py` lists findings by how much more
  often they fire in swapped frames than in the control.
- **SDK (mods)**: xikit loads mod DLLs (`mods=` in xikit.ini; `sdk/`, docs/SDK.md). A mod hooks any of the 774
  manifest functions from `xikit_mod_init`; `XI_HOOK` checks at compile time that the replacement has the tree's
  declared type (calling convention included: a static_assert, since the tree's headers need -fpermissive).
  `previous` is a trampoline into the game's own code for 721 functions (manifest `T=`: whole instructions from the
  entry, 5..16 bytes, rel32 re-aimed, refused where a branch or return is among them, the bytes pass the function's
  end, or code jumps into them) and the reconstruction's export for 53. Manifest flag 8 (register input the
  declaration lacks; `Obj_104E1D4C::Cursor_Draw`) refuses a hook. `sdk/gen_sdk.py` makes `xigame::` typed entry
  pointers for all 777 reconstructed functions and `xihook::` flags. Wine: mod runs at base and relocated (hook,
  trampoline to the game's code, a re-aimed call in a trampoline, refusal, frame callback, swap=0). The example's
  drawing is compiled only; nothing has run in the game yet.
- **Roadmap** (docs/ROADMAP.md): the three goals charted from the research. Over 8 captures, frame time follows
  the call count (frame ms ~ 6.6 + 0.00168 x calls, r = 0.988; a correlation, not a cost measurement). In capture
  11000 the heaviest caller is CYyModelDt::SkinAndDrawV2 (5,330 calls per frame, 72% redundant); every hot
  function there is hookable, most with flag 4. The next step is measuring where the time goes (P0).
- **trace_diff.py** read the manifest's last column as the name; since the `T=` column it is column 6 (fixed).
- **Gaps** (docs/GAPS.md): what blocks changing any part of the pipeline on knowledge. The gate's trials ran 63.3%
  of the tree's instructions; the hot paths least (SkinAndDrawV2 62 of 1,596, Frame_Render 53 of 1,424, the zone
  mesh 462 of 1,403). The occlusion readback is in the export but UNRESOLVED at one call (0x1006C8CD), not outside
  it: ROADMAP.md and RENDERER.md said otherwise and are corrected; every device user is in the export. The
  in-game DLL's own fuzz gate, re-run: 772 PASS, 1 FAIL (Post_DrawRotatingGhostLayers, not in the manifest), 4 not
  compared. Ordered closers: in-game input recorder, timing, picture comparison, in-game fact recorder.
