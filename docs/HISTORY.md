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

## 6 Oct (night): the measuring tools of GAPS.md section 7, items 1 to 5

All tested under Wine with the real FFXiMain.dll (kit/xikit/test/test_xikit.py, kit/d3dprof/test_thunks.py); none
has been run in the game yet.
- **Input recorder** (`capture=`): the registers and the pages reachable at a function's entry, as `.xcap`;
  pipeline/xcap.py replays the call in the gate (the test capture: PASS, with coverage).
- **Timing** (`xikit_timing_<pid>.csv`: the wait inside Present against frame time) and the **sampler**
  (`sample=1`, kit/d3dprof/samples.py: self, elsewhere and inclusive views).
- **Pictures** (`trace_pictures=K`): the back buffer of the alternating trace's frames; kit/d3dprof/pictures.py
  compares each swapped frame with its neighbours, against a control.
- **Fact recorder** (`facts=1`): pop count and target at the call sites the gate leaves UNRESOLVED;
  pipeline/fact_sites.py prepares the sites and turns agreeing observations into data/game_sp.json.
- **State per draw, resources, shaders**: ` s=` per draw and `#S` per distinct state in the trace (known states
  only), named by trace_diff.py in a finding; every creation call in `xikit_resources_<pid>.csv`; every vertex
  shader saved as created and decoded by kit/d3dprof/vshader.py. Not built: the static catalogue mapping each
  shader to its `vs.1.1` source text in the binary.

## 6 Oct (night, later): GAPS.md section 7, item 6: forks, device hooks, mod reload

- **Forks** (`sdk/fork.py`, `XI_FORK`): `new` copies a function's block from the tree's source; `diff` shows the
  edit; `gate` builds the in-game tree with the function replaced by the fork (compiled with the flags a mod build
  uses: `sdk/fork_flags.txt`, generated from tree/build.sh) and gates it against the game's function. A fork's
  source is the verified source, but its compiled code is not assumed to be the verified code (GCC's code for a
  function depends on the rest of its file): the gate is what verifies it. Unedited `CDx::DrawFadeQuad`: PASS,
  208 of 208 instructions; with `D3DRS_FOGENABLE` 0 changed to 1: FAIL at stubbed call #7, the changed argument.
- **Device hooks** (`XI_DEVICE_HOOK`, kit API 2): a front vtable on the device; the profiler and trace sit behind
  it and name a call a hook passes on by the call it is handling.
- **Mod reload** (`mod_reload=1`): mods load from copies; a changed file is loaded again in the game; old builds
  stay loaded.
- Tested under Wine (test_xikit.py: the mod runs and a new reload run); not run in the game.
- The resource log of the previous entry is written only when d3dprof is attached (`profile=1` or `trace=1`);
  KIT.md said "always on" and is corrected.

## 6 Oct (night, later): GAPS.md section 7, item 7: the gate with declared differences

- **pipeline/declared.py, `fork.py gate --declare`:** a changed function against the original, where every
  difference must be declared: calls dropped, added or changed (Direct3D method or game function, argument
  conditions), memory, return value; the report counts each declaration seen and lists those never seen.
  `sdk/test_fork.py` (CDx::DrawFadeQuad): unedited PASS; FOGENABLE 0 -> 1 FAIL, declared PASS; LIGHTING replaced
  by AddRef: FAIL until both the drop and the add are declared; a declaration for another state does not cover it.
- In declared mode the gate answers each stubbed call from the call's own content (diffemu_gfx.py `_keyed`), so a
  dropped call does not shift later answers. Off by default: a normal gate run is byte-identical to the previous
  harness (checked on 0x10001520, 0x100091B0, 0x1000A820, 0x100A4830).
- **Found while checking that:** the current harness does not reproduce `verification/gate_clean_final.jsonl`
  exactly, with or without this change. The verdicts agree, but the counts differ. 0x10001520 has outcomes
  return 67 / import 33 now, against 75 / 25 recorded. 0x1000A820 has 1568 D3D calls compared, against 1552.
  0x100091B0 has coverage 282, against 274. The record predates later harness changes, and which change moved
  the numbers is not established here.

## 6 Oct (morning, tools): the tools brought up to date
- **Tests run here:** 32-bit Wine 9.0 and `numpy` installed; `test_xikit.py` 185 checks ok (it now builds the gate's
  tree DLL when the capture replay needs it, and checks the logged build stamp), `test_thunks.py`, `sdk/test_fork.py`,
  `run.sh test` all pass.
- **`pipeline/tool_drift.py`:** regenerates the IDA script and the SDK's generated files, rebuilds the kit (reproducible:
  d3dprof now links with `--no-insert-timestamp` too) and compares with `kit/BUILD.lock.json`; checks the ini keys
  xikit.c reads against its default ini and docs/KIT.md. First run found:
  - `ida/ffxi_ida_fix.py` lacked the 6 Oct DynVB fixes in `data/type_fixes.json` (2 conventions, 9 prototypes):
    regenerated;
  - the shipped `exclude=Gfx_DestroyD3DHolder` was in the hand-edited ini only, not in xikit.c's default: added;
  - four ini keys missing from docs/KIT.md: documented.
- **Build stamp:** `kit/xikit/dist/VERSION.txt` (source commit, date, tree gate, file hashes), logged by xikit.dll at
  start; the SDK zip carries its own `VERSION.txt`.
- **Docs:** README (layout rows for `kit/` and `sdk/`, Wine and numpy, `tool_drift.py`), kit/README.md counts, SDK.md's
  register note. A first edit there said the EDX analysis still counts every call as a write; that was wrong:
  `edx_inputs.py` runs `ecx_inputs`' fixpoint, which follows `reg_preserve.py` for EDX too (7a925bd). Regenerated,
  `data/edx_use.json` is identical to the committed file; task #64 is marked closed in SDK.md, GAPS.md and ROADMAP.md.
- **The gate counts that "did not reproduce" (6 Oct, night) do reproduce.** The harness had not changed: the
  check ran `diffemu_gfx.py` directly, whose `--seed` defaulted to 1, while every recorded gate runs with seed 11
  (`gate_some.py`, `mutate_clean.py`). With seed 11 the current harness gives the recorded numbers exactly
  (0x10001520 return 75 / import 25, 0x1000A820 1552 D3D calls compared, 0x100091B0 coverage 274); with seed 1 it
  gives the numbers that were reported as drift (67 / 33, 1568). `diffemu_gfx.py` now defaults to seed 11, so
  `fork.py gate` and any direct run use the gate's trials (`sdk/test_fork.py` passes).
- **Static shader catalogue** (`kit/d3dprof/shader_catalogue.py`, `data/shader_catalogue.json`; GAPS.md 7 item 5,
  the part not built): the 5 vertex shaders and 1 pixel shader 0x10178250 builds from source-line tables in `.data`,
  their text as 0x10177090 joins it (both `[CDx+0x920]` variants; none of the six has an optional `#` line), and
  the exact tokens from the game's own `D3DXAssembleShader` run in Unicorn (heap and critical sections answered;
  d3d8.dll's validation of the result not run, it writes no tokens). Decoded by `vshader.py` the tokens are the
  source lines (D3DX writes `sub` as `add` with a negated source). `vshader.py` names a captured shader by its
  token hash. The other 6 CreateVertexShader sites: 4 fixed-function (no code), 2 in functions nothing calls or
  points to (one assembles `c:\test.vsh`). `tool_drift.py` regenerates the catalogue too.
- **`fork.py gate-hook`** (GAPS.md 7 item 7, the part not built): a mod's own replacement hooked with `XI_HOOK`,
  gated against the game's function from the mod's sources (compiled as `build_mod.sh` does), with a generated
  gate host: the mod's `xikit_mod_init` runs once before the trials (a case's new `setup` export in
  `diffemu_gfx.py`, its writes kept; a normal gate run is byte-identical, checked on 3 functions), `previous` is the
  game's own entry. `sdk/test_fork.py`: 12 checks, all ok (4 new).
- **Per-call time** (GAPS.md 7 item 2, the part not built): `call_time=1` (xikit, with `profile=1`; standalone
  d3dprof: `d3dprof.ini`) times each Direct3D call on the render thread through a return stub and a 64-deep shadow
  stack; `us_per_call` and `us_per_frame` per call site, the counter's frequency and its read cost in the header.
  Tested in Unicorn (`test_thunks.py`: return, pop, EAX/EDX, one interval recorded) and under Wine (`test_xikit.py`
  profile run: 186 checks ok). Stale rows in GAPS.md and ROADMAP.md (timing, pictures "missing" / "to build") updated.

## 6 Oct (later): speedfix rewritten as a mod of the kit
- The owner gave speedfix_hook.cpp (2,091 lines, GPL-3.0-only) and its last build; `speedfix_patches.h` (its patch
  data) was not among them. Rewritten as `kit/speedfix/` (GPL-3.0-only kept): the device fixes on the kit's device
  hooks (d3dprof behind them measures what they save), the game-code changes re-derived from the binary.
- Found in the code while doing so:
  - the occlusion stagger can be one re-aimed call in the bucket test (0x100CC58A), keeping forced updates and allowing N
    to be lowered live;
  - skipping the readback alone (not the whole query) keeps Impl's restoring of ZENABLE/ZWRITEENABLE;
  - `frame_cap_fps` above 60 would make the game's time run fast (the step is max(step, 1) ticks; RENDERER.md 3a), and
    60/n caps keep game time exact;
  - the SSE tier is chosen in a load step after the device exists (0x10010D20), so a mod's init is usually early
    enough.
- Tested offline: `kit/speedfix/test/test_speedfix.py` (Wine host with the real DLL and the kit: 20 checks; the gate
  on the query hook: PASS, 38 of 38 instructions). The kit now applies `respect_speedfix` to the mod too. Not run in
  the game.

## 6 Oct (later): the owner's baseline capture (kit 0de35c4, no speedfix, swap=0, profile=1 call_time=1)
- d3dprof's last 600-frame window (process 10016): 19.2 ms per frame; 13,701 Direct3D calls, 1,670 draws, 4,519
  sets that repeat the current value. The timed calls add up to about 15.9 ms per frame, timer reads included. Frame
  time varied between 18 and 46 ms over the session's windows (xikit_timing), so only a run ending in the same place
  compares with this window.
- Where the measured time is:
  - EndScene: 4.4 ms (3 per frame, 1.46 ms each);
  - SetRenderState: 3.9 ms, 3.6 ms of it at one site, 0x1011F962 in Cell_DrawQuadBlended (ALPHABLENDENABLE = 0 right
    after each quad's DrawPrimitiveUP; 540 per frame at 6.7 us; 10 of them repeats). The other sites average about
    0.1 us;
  - DrawPrimitiveUP: 3.4 ms (1,031 per frame);
  - DrawPrimitive: 2.7 ms.
  Not known: whether that state change is expensive itself, or whether the layers below (dgVoodoo) do the queued
  draw's work when blending changes. The trace cannot tell them apart. Either way the state cache cannot remove it
  (it is not a repeat), and batching cannot merge across it.
- SetVertexShader: 1,215 of 1,241 per frame repeat the bound shader (0.1 ms in all).
- Other-thread calls: 3,220 since the start. In the resource log, 1,970 creations were on another thread
  (CreateVertexBuffer, CreateTexture).
- Fixed in d3dprof's reports (no change to what is measured):
  - the resource log's method field held unquoted commas, which shifted the columns;
  - its frame column was the frame within the window;
  - `other_thread_calls` was a total since the start without saying so; it is now given per window and since the
    start;
  - the timer cost showed only the least of 1000 back-to-back read pairs, 0 at 24 MHz. It now shows the mean too.

## 6 Oct (later): the owner's speedfix run (kit 0de35c4, speedfix defaults, process 6080)
- Scene: combat test, a busy town, then the same combat test as the baseline. The last 600 frames do not match the
  baseline's: 25,766 calls, 4,888 draws and 808,715 primitives per frame (baseline 13,701, 1,670, 204,100). The
  difference is the zone mesh: sub_1017C9C0 asked for 3,851 DrawPrimitive per frame (baseline 557), 16.3 ms at 4.2 us
  each. speedfix does not touch it, so frame time (40.2 ms against 19.2) does not compare.
- Per call, compared: SetRenderState 0.87 -> 0.54 us on average (the repeats it removes were the cheap ones); the
  Cell_DrawQuadBlended blend site 6.7 -> 6.9 us (not a repeat: unchanged, as expected); EndScene 1.46 -> 1.29 ms.
  Repeated sets reaching the device: 4,519 -> 201 per frame. speedfix's log: 15-34% of the cached setters skipped
  per minute, varying with the scene. cpu_tier was too late (the tier was already chosen at injection).
- The alternating trace (17 frames, speedfix off/on each frame), with `trace_diff.py`:
  - 44 COUNT findings: fewer repeated state calls in the speedfix frames;
  - one OUTSIDE finding: SetLight of light 1 differs in Position, in every speedfix frame. Light 1 is directional
    (type 3), and D3D8 ignores Position for directional lights; it holds leftover stack values
    (`7184BA71 001AEAC0 0` off, `32 AB 1` on) that speedfix's code on the stack changes. Direction, colours and
    attenuation are identical.
  Nothing else differs.
- Not measured: the cost of speedfix's own hooks (d3dprof sits below them). With batching off, speedfix had still
  hooked all 94 methods, putting some 20,000 calls a frame through two layers for nothing. It now hooks only the 12 it
  needs unless batching or the extended cache is on at the start.
- **The kit, asked for by the owner (keys do not reach the game window):**
  - `ab=1`, an A/B test with no keys: ABBA phases switching off everything the kit adds, with per-phase frame times and
    a summary that sets B - A against the spread between phases of one label;
  - `log_folder=1` (the default): a folder per start for every file the kit writes.
  Wine test mode `ab`; 194 checks, all ok.
- **Loading and stutters, from the two captures' resource logs:**
  - about 14,450 resource creations per session; 12,460 of them on the render thread, which waits on each;
  - per-call times that were measured: CreateVertexShader 269 us, CreateTexture 148 us, CreateIndexBuffer 70 us;
  - CreateVertexShader was called about 1,450 times per session for only 3 distinct shaders (declarations without
    code; 471 and 397 handles saved, 3 distinct contents), so the same shaders are created again and again;
  - most creations came from one call site outside FFXiMain (module offset 0x9D8E), which the old logs could not
    name;
  - frame spikes reached 1.3-1.5 s.
  Kit changes so the next capture can tie stutters to their cause:
  - the resource log names the module of a caller outside FFXiMain (`module+0xOFFSET`), as does d3dprof's profile;
  - the resource log times each creation call;
  - `spike_ms=50` writes every slow frame with its frame number, which matches the resource log's.

## Levers for the owner to measure (speedfix)

The owner asked for the levers from the analysis, made testable. From the two sessions' logs (runs 10016 and 6080)
and the repo:
- **Steady frame time:** `Cell_DrawQuadBlended` turns ALPHABLENDENABLE off after each quad and on before the next
  (540 calls a frame of the off alone, 3.6 ms a frame in the baseline). The cache cannot skip a change. New lever
  `lazy_state`: render-state, texture-stage-state and texture changes are held until a method that reads them, and a
  change undone before then is never sent. With batching, quads around such a toggle merge. It hooks every
  state-reading method, a cost not yet measured against the saving.
- **Loading and stutter:** about 1,450 CreateVertexShader calls a session (269 us each) for 3 distinct declaration-only
  shaders. New lever `vs_dedupe`: one shader per declaration and usage, its handle given out again, deleted with its
  last holder.
- **Vertex and index buffers:** about 10,000 created a session (parsed from the resource logs: mostly DYNAMIC |
  WRITEONLY default-pool vertex buffers of 6,000, 4,000, 8,000 and 2,000 bytes; 6,500-6,800 of them from the unnamed
  module at offset 0x9D8E, 1,260-1,630 from FFXiMain 0x10170A61 and 0x101703C7). Not a lever yet: pooling needs the
  buffers' own Release, and the creating module is still to be named by the next capture.
- **The lever A/B:** speedfix's `ab_levers=` runs ABBA phases with the listed settings off (A) and as set (B), the rest
  of speedfix on in both, and logs per-phase frame times and B - A against the spread within a label.
- The default hook set grows from 12 to 13 methods (CreateVertexShader, so vs_dedupe can switch live).
- Tested offline (`test_speedfix.py`, three Wine runs plus the gate): every lazy_state, vs_dedupe and A/B check ok.
  Not tested: the real game.

## 6 Oct (later): the owner's Bedau run (kit 0de35c4, speedfix mod, profile=1 call_time=1, swap=0)

Process 3736, about 28 minutes, 28,200 frames. The run has no sampler output (sample=1 was not set).

**Frame time by section (xikit_timing):**
- Frames 600 to 6000: about 18 to 21 ms per frame.
- From about frame 6600: 64 to 79 ms per frame, steady to the end.

**d3dprof's last 600-frame window:**
- 67.9 ms per frame.
- About 30,000 Direct3D calls, 2,830 draws and 168,590 primitives per frame.
- 20 repeated sets per frame. speedfix's cache skipped the rest: 37 to 44% of the setter calls entering it, per its
  log.

**Where the time inside Direct3D calls goes.** Timed calls add up to 25.8 ms per frame; Present is 0.24 ms of it.

| Method | ms per frame | Calls per frame | µs per call |
|---|---|---|---|
| DrawIndexedPrimitive | 9.0 | 1,586 | 5.7 |
| DrawPrimitiveUP | 4.9 | 885 | 5.5 (2.5 primitives each) |
| DrawPrimitive | 1.6 | 359 | 4.5 |
| EndScene | 4.6 | 7.7 | 600 |
| SetRenderState | 2.8 | 11,228 | — |
| Other setters | under 0.6 each | | 0.08 to 0.45 |

- 1.5 ms of the SetRenderState time is one site, 0x1011F968 in Cell_DrawQuadBlended: 198 calls per frame at
  7.4 µs each.
- The other setters cost 0.08 to 0.45 µs per call.

**Outside any timed Direct3D call: about 42 ms per frame.** That covers the game's own code, the layers in front
of d3dprof and Parallels.
- The layers in front of d3dprof: speedfix on 94 methods, the kit's front thunk, and d3dprof's own record.
- How the 42 ms divides among these is not measured.
- In the baseline (no speedfix, a different place), only about 3.3 ms of 19.2 ms was outside the timed calls.

**Across this run's 27 one-minute speedfix reports:**
- Fit: frame ms ≈ 11.0 + 1.48 µs × setter calls entering speedfix (r = 0.970).
- That per-call figure is far above the 0.08 to 0.25 µs the setters take inside Direct3D.
- This is a correlation over one session's scenes, not a measured cost.

**Present stays at about 0.24 ms throughout:** the CPU does not wait for the GPU at Present.

**Resource log:**
- 15,367 creations in all:
  - 9,438 vertex buffers;
  - 2,525 textures;
  - 1,741 vertex shaders (498 distinct handles saved);
  - 1,649 index buffers;
  - 1,970 of them on another thread.
- Every row's call site was the same raw address, 0x71D39D8E. speedfix hooks the Create methods, and the resource
  wrappers took their return address, which lies in the hook layer, not in the game. Fixed:
  - The wrappers now name the game's call through the kit's caller stack, on the render thread.
  - Other threads keep the raw address.
  - Test: test_xikit.py, mod runs.
- This build wrote neither an absolute frame number nor the creation time, so churn per frame and stutter causes
  cannot be read from this log. Later builds record both.

## 6 Oct (later): the owner's Bedau run with the sampler and the A/B (kit b617642, speedfix defaults, profile=0)

Process 6156. Bedau frame times are the same as the profiled run before it (64 to 72 ms), with the profiler off and
speedfix on 13 methods, not 94. So the profiler and hook layers did not make those frames slow.

**A/B (xikit_ab_6156_1.csv).** The game was closed during phase 6, so 5 of 8 phases are complete.
- A (kit and speedfix off): 69.9, 70.9, 72.0 ms.
- B (on): 68.1, 66.4 ms.
- B − A ≈ −3.6 ms (about 5%). That exceeds the spread within each label, but rests on two B phases.

**Sampler (the last 600 frames, about 71 ms each; 21,595 samples of the render thread).**
- 62.5% of samples report EIP 0x00010002. That is no module's code, and every such sample has a game return address on
  the stack. Read as: the thread was in code outside the 32-bit context (the D3D11 runtime, the Parallels display
  driver prl_umdd10.dll, the kernel). That reading is not established.
- Time inside calls the game made out of its own code, by the game function that made them (68.9% in all):

  | Calls made by | Share of time | Decoded at the call site |
  |---|---|---|
  | The occlusion readback 0x1006C8B0 | 19.5% | IDirect3DSurface8::LockRect, vtable slot 9, D3DLOCK_READONLY, at 0x1006C91C |
  | CDx::DrawIndexedPrimitive | 16.0% | |
  | CDx::DrawPrimitiveUP | 10.3% | |
  | CYyModelDt::SkinAndDrawV2 | 7.6% | IDirect3DVertexBuffer8::Lock (slot 11) with D3DLOCK_DISCARD (0x2000), and Unlock; on a failed lock the game calls Sleep(0) and retries |
  | CDx::DrawPrimitive | 3.2% | |
  | Cell_DrawQuadBlended | 1.95% | SetRenderState(ALPHABLENDENABLE, 0) at 0x1011F962 |

- The game's own code: 31.1%.
  - 13.3% in 0x1002F1D0's x87 skinning loop (0x1002F200..0x100300AF, which reads g_boneWeightMatrixTable).
  - 0x1002F1D0 chooses between that loop and an SSE loop at 0x100300B0, whose helpers 0x100309B0 and 0x10030A40 are SSE
    code. The choice follows two bytes, [[0x104566E8]+9] and [[0x104566E8]+0xC], of an object set at start
    (0x10001339).
  - All 2,865 samples in that function were in the x87 loop.
  - What sets those two bytes is not traced here. Whether speedfix's cpu_tier changes them is not established:
    cpu_tier writes the SSE flag 0x1047CF78, which feeds the processor object made by 0x1006CB50.
- xikit.dll and speedfix.dll together: 26 samples (0.1%).

**Why call_time timed only 38%.** call_time timed only device methods. The readback's LockRect (a surface method) and
the skinning buffers' Lock and Unlock are not device methods, which accounts for most of the time call_time saw as
"outside Direct3D" in the run before.

kit/d3dprof/samples.py: the inclusive view's standard error failed on shares above 100% (a function on a stack more
than once); fixed.
- **The owner's machine is an M5 MacBook Pro** (stated by the owner). So Windows on ARM runs in Parallels and the game
  is x86-emulated.
- **Why the game skins with x87:** its CPU detection (0x1000D400) sets the SSE flag `[0x104566E8]+9`, the one that
  selects the SSE skinning loop, only for a "GenuineTMx86" vendor or one containing "Intel". speedfix lever
  `sse_skinning` (off by default; A/B-able) writes the CPU's own SSE bit there. Wine-tested; not run in the game.
- **The owner's earlier speedfix_hook.cpp compared with kit/speedfix.**
  - Already carried over: cpu_tier, texture_compression, the caches, occlusion_stagger, both occlusion skips, the
    frame pacer (high-resolution timer plus spin), batching with the RHW rule, and the batch caps.
  - Dropped on purpose earlier: the device-call census (d3dprof's profile replaces it) and the unsafe worker-thread
    submit.
  - Left out: `occlusion_async_readback`. The owner measured it slower on dgVoodoo/D3D11/Parallels.
  - Taken now: its log of the math tier in use. g_pProcessor's vtable is checked against the three tier classes,
    0x1032C298 SSE, 0x1032C1C8 3DNow! and 0x1032C368 x87.
  - Run 6156's samples show the x87 tier in use. Its methods took 0.6% of the render thread, so the tier itself is
    not a lever there.
  - Not taken: its camera still/moving split of frame statistics, which needs a SetTransform hook.
- **speedfix additions after the Bedau runs:**
  - `merge_scenes` (lever): one Begin/EndScene pair a frame. The profiled run had 7.7 scenes a frame and 4.6 ms in
    EndScene.
  - `motion_stats`: speedfix_hook's still/moving split of frame statistics.
  - `stats_every`: seconds between the statistics lines.
  - `ab_levers` groups separated by `;`, run one after another. Several levers can be measured in one session
    without editing the ini between them.
  - All Wine-tested (test_speedfix.py); none run in the game yet.
- **Who issues the draws in Bedau** (run 3736's profile, by the caller of each CDx draw wrapper):
  - DrawIndexedPrimitive, 1,586 a frame: all from CYyModelDt::SkinAndDrawV2. It walks a model's command list and
    issues one triangle strip (type 5) per strip record, about 58 primitives each. [Corrected, 6 Oct night: the 5 is
    the trace's argument count; the type is the next field, and 97% are type 4 lists, from `0x54` records.]
  - DrawPrimitiveUP, 885 a frame: Font_DrawGlyphQuad 386, Cell_DrawQuadBlended 209, Actor_DrawWorldText 200 (2
    triangles each).
  - DrawPrimitive, 359 a frame: CMoVuElem_Draw 270; the zone mesh 0x1017C9C0 45, at about 714 primitives each.
- **New tools:**
  - the kit logs the CPU (CPUID), the machine and Windows build, and every graphics module with its version;
  - kit/d3dprof/compare_paths.py compares two profiles per method;
  - kit/d3dprof/merge_potential.py counts the draws merging would leave in a trace.
- **Fixed:** trace_diff.py read the `#S` state lines as comments, because the `#` test came first. Its state
  differences were therefore always empty.
- **The owner states the game always crashes without dgVoodoo** on their machine. Windows' own Direct3D 8 is
  therefore not a path to compare there. Why it crashes is not known; xikit.log's module list would show what
  loads up to the crash. Left to compare: dgVoodoo's own settings, such as its output API.

## 6 Oct (evening): the owner's three sessions (build f94640a)

**Environment** (the new log), the same in every session.
- CPUID vendor "Virtual CPU " (Virtual CPU @ 3.07GHz), SSE to SSE4.2, no AVX. The game's detection therefore
  withholds its SSE skinning.
- Native ARM64, the process x86-emulated. RtlGetVersion reports Windows 5.1 build 2600 (XP) inside pol.exe, which
  looks like a compatibility-mode version lie; not established.
- Graphics modules:
  - d3d8.dll 4.8.2.134 (dgVoodoo's, in PlayOnlineViewer);
  - DDRAW and D3DIM700;
  - Windows' D3D11, dxgi and dxcore (26100);
  - Parallels' prl_umdd10.dll 20.18.2640.57513.
  All x86.
- dgVoodoo's Direct3D 12 output shows texture stretching and flicker (owner). D3D11 stays.

**Session 1 (process 12764): xikit ab=1 with speedfix's four levers on**, all of them, against the kit off, in Bedau.
- A 74.1, 70.6, 70.6, 71.2 ms; B 40.2, 33.2, 34.6, 34.6 ms.
- B − A = −36.0 ms (−50.2%). The spread within a label was at most 7.0 ms.
- The per-lever A/B (ab_levers) was not run, so each lever's share is not known.
- speedfix's counts:
  - DrawPrimitiveUP 84 to 87% fewer;
  - lazy_state: about 7 million calls a minute never sent;
  - merge_scenes: 3,000 to 8,000 pairs a minute dropped;
  - every actor occlusion query skipped;
  - the math tier in use was SSE, because cpu_tier applied before the game chose.
- Sampler, last 600 frames with everything on (about 36 ms each):
  - SkinAndDrawV2's calls into Direct3D 26%. DrawIndexedPrimitive lists at 0x1002E746 13.6%; SetStreamSource
    right after the skinning buffers 4.9%.
  - The occlusion readback's LockRect 22.4%: the non-actor query users.
  - Present (Frame_Render 0x10012B00) 16.9%. merge_scenes moves the scene's end to just before it.
  - The x87 skinning loop no longer appears; the SSE loop 0x100303B0 is under 1%.
- motion_stats: no camera-moving frames during the A/B (as asked: standing).

**Session 2 (process 5180).**
- 3 captures of SkinAndDrawV2 (82 MB each; the page budget ran out) and a trace of 590,195 records.
- merge_potential.py on label 0 (no swaps, no mods), per frame:
  - DrawIndexedPrimitive 1,620 draws make 1,561 runs; merging in place leaves nearly all of them;
  - DrawPrimitive 225 make 216 runs;
  - DrawPrimitiveUP 1,221 make 185 runs.
- Between consecutive character draws:

  | What separates them | Share of pairs |
  |---|---|
  | Their vertex buffers differ (vb:0, vb:1), nothing else | 49% |
  | SetLight | 36% |
  | SetLight and SetTransform | 6.6% |
  | Nothing: mergeable | 3.6% |

- If every part were skinned into one shared buffer, the draws would make 771 runs. If, in addition, a light,
  transform, material or viewport set to the value it already has did not count as a change, they would make 510.
  That is the measured upper bound for SkinAndDrawV2 issuing fewer draws.
- Replaying capture 1 through the gate: UNRESOLVED at 0x1002DD00, the skinning buffer's IDirect3DVertexBuffer8::Lock,
  a call into dgVoodoo's d3d8.dll whose pop count no source settles. The fact recorder measures it: 110 sites
  prepared, the 57 indirect calls of SkinAndDrawV2 and the gate's other UNRESOLVED sites.

**Session 3 (process 2020): dgVoodoo D3D11, no mods.**
- 72.7 ms a frame; 43,659 calls, 3,213 draws a frame.
- No D3D12 profile was taken, because of the glitches above.

**Session 4 (process 12164): every speedfix lever on, facts=1, no profiler.**
- Where it was taken is not in the log. Two scenes by their draw counts:
  - the first 3 minutes: 819 draws a frame standing still; p50 17.5 to 19.4 ms, means 20 to 25 ms (spikes);
  - from then on (about 10 minutes, standing still): 3,450 to 3,640 draws a frame (1,250 to 1,270 of them
    DrawPrimitiveUP); in minutes without spike runs, mean 31.5 to 35 ms, p50 30 to 35 ms, p95 37 to 46 ms.
- Present itself (the kit's own timing of the call into dgVoodoo) 0.27 to 0.59 ms a frame. So session 1's
  "Present 16.9%" (the sampler at the Present call site) was not Present itself. merge_scenes sends the scene's
  held EndScene from inside its Present hook, before passing Present on, so EndScene's time is counted at that site.
- Occlusion readbacks: none ran (skip_all_occlusion_readbacks), CopyRects 0.0 a frame throughout. Every actor query
  was skipped.
- sse_skinning set the game's SSE flag (it was 0; the CPU reports SSE). The math tier stayed x87: cpu_tier applies
  only when the kit is injected before the game chooses.
- Spike runs: 24 runs of 5 to 71 consecutive frames at 88 to 123 ms in 12 minutes. Their cause was not measured
  (no sampler in this session).
- Windows reports itself as 5.1 build 2600 to the game (RtlGetVersion), and session 2's capture lists
  AcLayers.DLL and AcGenral.DLL. The game runs under a Windows compatibility layer. Its effect was not measured.
- ab_levers was set to "A", "B", "1" and "A B B A". None is a lever, so no A/B ran. The ini's text ("phases ... A B
  B A") read as if those were the values. It now names settings in an example, and the log says so when a name is
  not a lever.
- facts=1 measured 61 call sites, every observed call agreeing, into data/game_sp.json. 50 of them are in
  SkinAndDrawV2, including the skinning buffer's Lock at 0x1002DD00 (pops 20; vtable d3d8.dll+0x12A654, slot 11).
- The 3 captures of session 2 replayed through the gate with these facts:
  - Capture 2: PASS, 8 of 8 trials compared, 932 of SkinAndDrawV2's 1,596 instructions covered (the gate's fuzzing
    had reached 62). This needed two gate changes. `Sleep` is now answered like the other benign imports
    (VOID Sleep(DWORD) in the Windows SDK; the calls are still compared), because SkinAndDrawV2 sleeps and retries
    after a failed Lock. And xcap.py takes `--insn-limit` (5,000,000 here); the gate's default of 300,000 is less
    than skinning a whole model takes.
  - Captures 1 and 3: every trial runs out of instructions in the command-list walk. The model's list runs onto
    page 0x26714000, which the capture does not hold (its 20,000-page budget ran out). Filled with fuzzed bytes,
    the list holds a command word the walk never advances past. The kit now takes the `capture_follow` pages
    (default 16) after each of the call's own pointers.
- The kit now records which interface each dgVoodoo vtable is: the vtable of each object a Create method returned,
  in xikit_vtables_<pid>.csv (profile=1 or trace=1), ingested by pipeline/com_vtables.py. The gate uses a record only
  when a captured vtable's entries match it exactly. Whether these captures need it is not known: the
  reconstruction's Lock went unresolved only in trials where the game's side had stopped at Sleep.

**The owner, 6 Oct:**
- "Bedau", the name these records use for the measuring spot, is the busy plaza of Port Jeuno.
- No exe in the PlayOnline folder is set to run in compatibility mode, yet the game is told Windows 5.1 and the
  compatibility DLLs are loaded. Where the layer comes from is not known. The kit now logs the kernel's own version,
  `__COMPAT_LAYER` and the registry layers for the exe.

**Session 5 (6 Oct), Port Jeuno's plaza ("Bedau"), standing still, every speedfix lever on.**
- Part 1 (process 2264): speedfix's lever A/B. Phases of 30 s, A B B A, 3 rounds, about 3,100 to 3,400 draws a
  frame. A = the lever off, B = on, everything else on in both:

  | Lever | A (off) | B (on) | B − A | Spread within a label |
  |---|---|---|---|---|
  | sse_skinning | 45.73 ms | 31.22 ms | −14.51 ms (−31.7%) | 5.96 ms |
  | batch_up_draws + lazy_state | 39.02 ms | 30.86 ms | −8.16 ms (−20.9%) | 3.37 ms |
  | skip_all_occlusion_readbacks | 31.80 ms | 30.49 ms | −1.31 ms | 3.98 ms (within it) |
  | merge_scenes | 30.31 ms | 31.48 ms | +1.17 ms | 3.48 ms (within it) |
  | skip_actor_occlusion | 29.48 ms | 29.81 ms | +0.33 ms | 4.37 ms (within it) |

  Occlusion readbacks requested: 158 in the first minute (loading in), none later while standing still. The
  readback LockRect at 22% in session 1's samples is not explained by this.
  The math tier was x87 at this start.
- Part 2 (process 11952): profile=1, sample=1, 3 captures, facts=1.
  - The compatibility line: the kernel is Windows 10.0 build 26200; the game is told 5.1. __COMPAT_LAYER is
    "HighDpiAware" and the registry has no layers for pol.exe. So the 5.1 does not come from a layer set on the
    exe; where it comes from is not known.
  - The math tier was SSE at this start (x87 at part 1's). Which tier a start gets is not under the kit's control
    when the game chooses before the kit is injected.
  - The profiler (after speedfix): 17,478 calls and 1,982 draws a frame; SetRenderState 4,745, SetStreamSource
    3,071, SetTextureStageState 1,861, SetTransform 1,653, SetLight 1,140, DrawIndexedPrimitive 956.
  - The sampler, last 600 frames (31.5 ms each): 70% of the render thread's samples were in memory of no module.
    By the game call site under them: SkinAndDrawV2 27% (its DrawIndexedPrimitive wrapper call 0x1002E746
    15.3%, 0x1002F130 5.2%, the buffer Lock 1.6%); CDx::Present from Frame_Render (with merge_scenes' EndScene)
    11.6%; CTsZoneMap::DrawProjectedShadow's SetTransform calls 9.8%; fn 0x10021C10 (via 0x100279F0) 5.2%.
  - xikit_vtables: the 5 dgVoodoo vtables (vertex and index buffer, texture, cube texture, surface) into
    data/com_vtables.json. The vertex buffer's matches the session 2 capture's entries exactly. facts=1 added sites
    to data/game_sp.json.
  - The 3 captures, with capture_follow, replayed: all PASS (7, 8 and 8 trials compared; one trial stopped at
    0x1002DDCE, the vertex buffer's Unlock on its retry path, with no fact for it). Coverage 840, 765 and 233 of
    1,596. Together with session 2's capture 2: 937 of SkinAndDrawV2's 1,596 instructions have run in a compared
    trial and agreed.

## 6 Oct (evening): the render-target switches (task 18) and speedfix's skip_shadow_pass

From run 6080's trace (taken about a minute before the session ended; where is not in its log, and the owner says the session ended at the combat-test spot) (17 frames, kit e69ca62), each frame is three scenes:
- one that only sets a render target;
- the world (about 3,860 draws, the zone mesh's DrawPrimitive almost all of them);
- the last one, which holds 18 Push/Pop pairs of one off-screen target (surface 3E21D114, no depth) with 1 or 2 draws
  each, then the UI.

That is 38 or 42 SetRenderTarget a frame.
- The pairs are Model_BeginOffscreenPass / Model_EndOffscreenPassBindTextures. When g_actorShadowMode is 1, the pass
  draws meshes a second time into that target (14 distinct meshes, 28 draws a frame), from the light's view. The mode
  is set in CXiSkeletonActor's per-actor update; which actors' meshes the pass draws is not traced here.
- Across the 17 frames, SetTexture never bound any render-target texture the resource log saw created, except the
  scene's (3E02CB40, 3024 x 1898). That log began after the game started, so the shadow texture may not be in it. The
  only CopyRects were the occlusion readback's, from another surface. What reads the shadow target is not known.
- Checked afterwards against the game's instructions (IDA listing): all four `call Model_BeginOffscreenPass` are
  followed by `test al, al` / `jz` past the matching End call. speedfix now counts, in the game, SetTexture binds of
  the exact texture the pass draws into (Begin's `[self+8]`).
- speedfix lever `skip_shadow_pass` (off by default): it hooks the pair and, between them, sends no SetRenderTarget,
  Clear or draw; every state call is sent.
  - Tested in the Wine host, driving the game's own two functions.
  - The owner checks it by eye: the actor's shadow with it on.
- The other scene splits are what merge_scenes (the parallel session) already removes.

## 6 Oct (evening): speedfix up_ring (task 19), and a race in the speedfix host test

- `up_ring` (lever, off by default): DrawPrimitiveUP, merged or not, goes out as DrawPrimitive from speedfix's own
  2 MB dynamic vertex buffer (NOOVERWRITE at the cursor, DISCARD on wrap; released before Reset). Wine-tested; its
  gain on dgVoodoo is for the A/B to show.
- test_speedfix.py's minimal run failed after the kit gained its environment log. xikit.c puts its Present hook on
  the device, then logs the environment, and only then puts the mods' device hooks in front. The host began its
  checks at the first vtable change, so under Wine the first calls missed speedfix. The host now waits for the kit's
  "methods hooked by mods" line. In the game this window is the first frames after injection, which speedfix's gap
  rule already treats as frames without its hooks.

## 6 Oct (evening): merging within one model, and the zone mesh's draws (tasks 78 and 16)

- kit/d3dprof/merge_potential.py `--same-value`: a SetTransform, SetLight, SetMaterial, LightEnable or SetViewport
  that sets the value its slot already holds does not break a run. The vertex streams stay in the state hash, so this
  measures merging within one model's own buffers, with no shared skinning buffer. Selftest ok.
- The plaza trace it was written for (session 2, process 5180) was in the parallel session's container and is not
  here. On run 6080's trace, character DrawIndexedPrimitive goes from 79 to 78 runs a frame with
  --same-value. The plaza's figure is still to be measured; send the session-2 trace (or a new trace=1 run in the
  plaza).
- The zone mesh in that frame (sub_1017C9C0, the DrawPrimitive through CDx at 0x1000CBF3): 3,879 consecutive
  pairs, none mergeable. Every pair is separated by its own vertex buffer (3,851 SetStreamSource over 371 buffers)
  and a SetTexture (3,479 changes over 17 textures). The SetLight between them always repeats its value, which
  extended_state_cache already removes. Fewer zone draws therefore need the zone's geometry rebuilt into shared
  buffers, grouped by texture (17 textures). That is a load-time rebuild of the zone format (task 16), worth it for
  town scenes like 6080's (16.3 ms of zone draws). In the plaza (45 zone draws a frame) it does not pay.

## 6 Oct (evening): the plaza trace measured for merging within a model (task 78)

The owner sent sessions 1 to 3 ("mutil") and session 5 ("Gumba", processes 2264 and 11952) again. The parallel
session had already recorded them (above). New here, from session 2's trace (process 5180, label 0, 9 frames):
- merge_potential.py in place: DrawIndexedPrimitive 1,620 draws make 1,561 runs; with `--same-value`, 1,525. Merging
  strips where they stand barely pays. Within a model, consecutive draws alternate between two pairs of vertex
  buffers (SkinAndDrawV2 draws each strip a second time, from v10+80/+84, when field_34 bit 0 is set), and material
  records sit between strips.
- Reordering within a model: 326 models a frame, each identified by its index buffer (the SetIndices before its
  draws). Their draws use 764 distinct combinations of state hash (textures, render and stage states, streams, index
  buffer), light values and transforms. So drawing each model's strips grouped by those combinations, each group as one
  indexed list built once, would leave at most 764 draws, close to the 771 that one shared skinning buffer gives. This
  is an upper bound: reordering keeps the picture only for parts drawn without alpha blending, and the hash alone does
  not say which parts blend. The fork has to keep blended parts in their order.

## 6 Oct (evening): phase 0 of the D3D11 renderer, built (kit/d3d11probe)

The owner asked for what the D3D11 port needs, in the kit. Phase 0 (docs/D3D11.md) measures, on the owner's machine:
- D3D11 per call and per plaza-shaped frame, from x86 (emulated) and from ARM64 (native): probe11 bench;
- the same through Direct3D 8 via the PlayOnlineViewer folder's d3d8.dll (dgVoodoo): probe8 bench;
- formats, threading, queries and the shader compiler: probe11 caps;
- whether an ARM64 process can present into the x86 game's window or share a texture with it: probe11 xproc.

Toolchain:
- GitHub, and with it llvm-mingw, is outside this container's egress policy (403).
- PyPI's `ziglang` (0.16.0, pinned) builds ARM64 Windows executables against mingw-w64's d3d11, dxgi and
  d3dcompiler_47 import libraries.
- The builds are reproducible and in kit/BUILD.lock.json.

Testing:
- Wine runs D3D11 and D3D8 here on Mesa's llvmpipe inside xvfb-run (`apt-get install xvfb libgl1-mesa-dri:i386`).
  The x86 probes run end to end there (test_probe.py, 8 checks).
- That also gives the D3D11 device of phase 1 an offline test bed, for behaviour, not speed.
- Not testable here: the ARM64 run, any number for the owner's machine, and the cross-process verdicts.

## 6 Oct (evening): the owner's run 7388 (kit e746fc6); skip_shadow_pass and up_ring reverted; phase 0's first run

**Run 7388** (Port Jeuno's plaza, every lever on, the lever A/B on up_ring):
- The owner's screenshots: corrupted frames in game and on the title and character screens. They show full-screen
  smeared textures and large blocky shapes, and screens not cleared.
- skip_shadow_pass: speedfix's counter showed the shadow pass's texture bound by SetTexture 259 times in the first
  minute and 2,116 in the next. The texture is read; the claim that its output was unused was wrong. Skipping its
  Clear and draws leaves stale contents in a texture the scene samples; whether that made the blocky shapes is not
  established.
- up_ring: phase 1 (A, off) 20.28 ms mean, phase 2 (B, on) 25.55 ms. It left stream 0 bound to its ring on the
  assumption, with no source, that the game sets stream 0 before any draw that reads it. Which draw read the ring, if
  one did, is not known; the frames' corruption has not been traced to a cause.
  - One explanation was checked and is wrong: Gfx_SetStreamSourceCached (0x1002F130) records the buffer and stride
    and then calls SetStreamSource for any non-NULL buffer (Gfx_ApplyCachedStreamSource; it does nothing for NULL),
    so the game does not skip a set because of its record.
- Which of the two levers caused which artifact is not established; both were on in every frame shown.
- The log ends at 13:00:55, during phase 3, with no summary line; why it stops there is not recorded.
- Reverted at the owner's request: kit/speedfix's code is back to the parallel session's last commit (b70f4fb).
  Kept: the host test's wait for the mods' device hooks (not a lever). The speedfix suite passes.

**Phase 0, first run** (phase0_20261006_124338): the results are in docs/D3D11.md, phase 0.
- D3D11 from x86 costs 1.15 to 1.66 times what it costs from ARM64 per call.
- The plaza-shaped frame: 7.78 ms as 2,500 draws, 0.15 ms as 33.
- A NOOVERWRITE Map costs 4.7 us, which explains up_ring's loss.
- A child's swap chain presents into the parent's window in both directions. The shared texture was not seen.
- probe8's Direct3D 8 times are all multiples of about 2.86 ms, so they are not usable. probe8 now logs its clock's
  step and frame means, and runs each call 200,000 times.

## 6 Oct (evening): phase 0, second run (phase0_20261006_131825)

- probe8's clock: QueryPerformanceCounter steps 0.042 us there, before and after the D3D8 device is made. The
  previous entry's "its clock stepped coarsely" was therefore wrong. Its times are still whole multiples of about
  2.864 ms, so the D3D8 calls end on that grid; the cause is not known.
- With 200,000 calls per measurement the grid is at most about 14 ns a call. Frame means are not affected:
  - the plaza-shaped frame through dgVoodoo: 7.28 ms;
  - through D3D11 (medians): x86 8.91 ms, ARM64 8.66 ms;
  - the 33-draw version: 0.26 ms through dgVoodoo, 0.14 to 0.17 ms through D3D11.
- The first run's per-call "1.15 to 1.66 times" is not comparable with this one. A small draw costs 219 ns at 20,000
  draws a frame and 906 ns at 200,000, on both architectures, so per-call cost depends on the batch. The frame
  figures are the comparable ones: x86/ARM64 1.13 (run 1) and 1.03 (run 2).
- dgVoodoo's DrawPrimitiveUP: 157.5 ns a call. D3D11 NOOVERWRITE Map plus Draw: 2.8 to 4.7 us.
- docs/D3D11.md, phase 0, now says what this means for the designs. Fewer draws pays on every path, through dgVoodoo
  too, so per-call cost is not the case for the port. Asynchronous occlusion queries and GPU skinning are, and they
  are not measured.
- Fixed in the probes:
  - probe11's frame lines carried no means: its format string lacked the two numbers, and nothing checked it.
  - put() is now declared printf-like, so the compiler checks every format against its arguments.
  - phase0_report.py reports frame means where they exist.

## 6 Oct (evening): the ring buffer looked at again

- In the plaza trace (label 0, 9 frames) no DrawPrimitive or DrawIndexedPrimitive reads stream 0 after a
  DrawPrimitiveUP without the game setting stream 0 first. So "a later draw read the ring from stream 0" has nothing
  in these frames to support it.
- A bug in the reverted up_ring, reproduced in the Wine host with the reverted code (batching, lazy_state and up_ring
  on, as in run 7388):
  - The ring sent its draw through the device's front, which re-entered speedfix's hooks.
  - The DrawPrimitive hook sent the held state first, so the device saw SetTexture(A), SetStreamSource(ring),
    SetTexture(B), DrawPrimitive. A UI quad was drawn with the next quad's texture.
- Whether a ring can pay at all on this path:
  - through dgVoodoo, DrawPrimitiveUP costs 157.5 ns and a DISCARD lock 2,849 ns (probe8, run 2);
  - dgVoodoo's NOOVERWRITE lock and a whole ring draw are now measured by probe8 too, for the next run.
  - A ring is reconsidered only if that measurement favours it, with its calls made below speedfix's hooks.

## 6 Oct (night): skin_merge, the SkinAndDrawV2 merge, built and gated (task 78)

- **No index capture.** The plaza's character draws are type 4 lists, not strips (the trace's 5 was its argument
  count). Consecutive `0x54` records draw consecutive index ranges (`v9 += 3 * v26`). A run of them is one range of
  the game's own index buffer: nothing to read back or rebuild. docs/SKINMERGE.md says why.
- **The fork:** kit/speedfix/fork/CYyModelDt__SkinAndDrawV2.fork.cpp (fork.py new, then edited on the 0x54 branch).
  It holds a list record when no pass of it can draw, and draws the run at the first record that is not a held list.
- **Model_BeginOffscreenPass, from its code** (disassembled at 0x10033800 and 0x10033A4A): unless word 0x1045B77A is 1,
  float 0x1045B80C < 4.0 (0x1032A3A0) and [this+8] != 0, it calls only `mov eax, ecx / ret` and `ret` helpers and
  returns 0. So a held record skips the pass block.
  - A first version called Begin before the held draws and needed "add" and "drop" declared for it. Skipping the block
    leaves only a "drop", which is a narrower declaration.
- **The gate:**
  - merge off, no declarations: PASS;
  - merge on (`--declare fork/skinmerge.declare.json`): PASS on fuzz, but no fuzzed trial reaches a held record.
  - Replayed from the owner's captures with the stubbed Begin: capture 11952_2 FAILed. The stub answered non-zero,
    and the original drew a pass that Begin's own code cannot allow on that memory.
  - The same replays with Begin running as the game's code: 11952_1, 11952_2 (a merged draw seen), 11952_3 and 5180_2
    PASS; 5180_1 and 5180_3 INCONCLUSIVE (command lists past the captured pages).
  - All of this is kit/speedfix/test/test_skinmerge.py.
- **What it can save** (the plaza trace, label 0, mean of 9 frames):
  - 1,577 of the 1,620 DrawIndexedPrimitive are SkinAndDrawV2 list draws: 916 records, 656 drawn twice;
  - the fork's rule leaves at most 1,330. Script: SkinAndDrawV2 draws found as a type-4 DIP right after a SetIndices
    from 0x1002E71E; merged when only shader/stream/index calls are between them, with the same index buffer, minIndex
    and numVerts and a following start index.
- **speedfix:**
  - `skin_merge=0`. When it is 1 at the start, XI_FORK installs the fork (sf_fork.cpp); after that it switches live
    both ways, and it is an `ab_levers` name.
  - test_speedfix.py checks the fork is installed when on and absent when off.
- **Not known:** time saved, and the picture. Both need the game: `ab_levers=skin_merge`, and characters looked at with
  it on and off.

## 11 Oct: occlusion_async reverted

- The owner's further testing found the reported occlusion-readback fix wrong. At the owner's request, speedfix's
  `occlusion_async` (sf_occlusion.cpp), its readback timing and their tests and notes are reverted. kit/speedfix is
  back to 291b257 (skin_merge).
- Kept from that work, as facts read from the game's code and not tied to the lever:
  - CMoOcclusionMng's constructor (0x1006C140) makes a 16x16 lockable render target (D3DPOOL_DEFAULT) at [mng+0x18];
  - the readback 0x1006C8B0 copies (CopyRects) the box texture into it and locks it at once;
  - the query objects are per owner (an actor's at +0x8F0).

## 11 Oct: an outside write-up of the renderer, tested against this build

The owner received a write-up of the client's renderer (static analysis of an older build, sha256 392e847a...,
6,206,976 bytes; "nothing was measured", in its own words). Nothing from it enters this repository; its claims were
tested here against the analysed build and the owner's traces.
- **Its functions are ours.** Its byte signatures each match one place in our build:
  - 0x10022400 -> 0x100224D0 (PrepareRenderState); 0x10033730 / 0x100339A0 -> 0x10033800 / 0x10033A70 (the
    offscreen shadow pass);
  - 0x1006C1B0 / 0x1006C7E0 -> 0x1006C280 / 0x1006C8B0 (the occlusion query and readback); 0x100CBCA0 -> 0x100CBDE0;
  - 0x1002DB40 -> 0x1002DC10 (SkinAndDrawV2); 0x1000D400 (CPU detection) at the same address;
  - its door visibility test 0x100AC4C0 -> 0x100AC5A0.
- **Agrees with this build:**
  - the 30 fps default pacing (RENDERER.md 3);
  - CDx::Present (0x10009720) retries up to 100 times and only logs a lost device;
  - no device Reset: none in the reconstructed tree, and none of the binary's 52 calls through slot 0x38 loads a
    device pointer within 20 instructions before it (a scan, not a proof);
  - the vendor-gated SSE skinning;
  - the door test reaches the occlusion query (0x100AC894) with no frame-counter test on its way.
- **Measured in the owner's traces** (frame 2, label 0):
  - occlusion readbacks (CopyRects at 0x1006C900): plaza (5180) 9 every traced frame; run 6080 1 every fourth;
  - the type-1 mesh path (its "re-expanded every frame", rank 3): no device call from PrepareRenderState in either;
  - the offscreen shadow pass: 18 a frame in both, 33 to 38 device calls each (it says 53 to 56), 629 calls a frame
    in all (1.5% of the plaza's);
  - draws repeated with the same arguments, state, stream, transforms and lights (its rank 6, layer 0 drawn twice
    "when a sub-area is loaded"): 1 in the plaza frame, 0 in run 6080's. Whether either had a sub-area is not known.
- **Not settled:** its "world target defaults to 512 by 512" is about the persisted settings' defaults. The owner's
  run 6080 had a 3024 x 1898 scene target, so its "fill cost is small" does not hold there.

## 11 Oct (later): the outside write-up audited in full

Every one of its 769 function entries mapped to this build where possible:
- 245 by its own byte signature (each one match);
- 454 by the address drift of the nearest signature, kept only where our function's span (start to the next start)
  equals the size the entry states;
- 70 not mapped.
Its "size" is that span, padding included. Its 3,943 instruction citations ("`text` at 0xADDR") were each checked
against our disassembly at the mapped address:
- 3,212 match outright, of 3,318 that can be checked (96.8%);
- of the other 106: 26 are rep forms or C expressions; 29 match within 16 bytes and 18 elsewhere in the same function
  (the cited address points at a neighbouring instruction); 33 have no literal match. Of those 33, the hand-checked
  ones are the same facts at a neighbouring address (both offsets 0x39594/0x39598, 0xdc0/0xdc4, 0x16c/0x19c occur
  there), or call targets our drift map placed on another function of the same size. No invented instruction was
  found.

**Its inefficiency claims, tested** (code: this build's disassembly or gate-verified tree; trace: frame 2, label 0
of the plaza 5180 and run 6080 traces):
- Confirmed in code:
  - the main loop forces the window to the foreground every frame (9 user32 calls) in window mode 0, the default;
    and the "restore" of the foreground lock timeout passes the buffer's address, not the value (a bug);
  - the vertex-buffer pool's 200 warm-up buffers (4 sizes x 50, about 1.3 MB) are never reused: they are state 4,
    Acquire reuses only state 0 and ages only 1-2, Release acts only on state 3;
  - the projection's bounding box is overwritten by its only caller (0x10042D43, the one slot-0x84 call on the
    processor in the binary) before any read;
  - the zone's deferred-element count (+0x38D40) is zeroed once before the layer loop and each layer's drain starts
    at 0: with two layers, layer 0's deferred elements are drawn again (not seen in either traced frame);
  - the weather blend frees and reallocates its 0xAC-byte record on that path (Mem_FreeObject, Mem_AllocObject);
  - the camera's view/projection is applied only after Present (0x10012E1E, its one caller), a frame late;
  - the skinned mesh's second draw is a mirrored copy (SKINMERGE.md).
- Confirmed in the traces (repeats of the value the device holds):
  - SkinAndDrawV2 8,872 of 12,477 sets;
  - nameplate text 1,095 of 1,136;
  - the font page SetTexture 286 of 335;
  - the zone model draw 211 of 302;
  - the D3A model's ZWRITEENABLE 17 of 17;
  - SetLight 1,459 of 1,857;
  - SetTransform 132 of 1,037.
  In all, 16,769 of the plaza frame's 36,919 state calls (45%) repeat the held value.
- Confirmed with different numbers:
  - an occlusion probe is 28 traced device calls (they say about 40), with the four SetRenderTarget, three
    DrawPrimitiveUP and a Clear inside its own EndScene/BeginScene pair, as they say;
  - the shadow pass is 33 to 38 calls per record (they say 53 to 56).
- Not running in the traced scenes: the type-1 mesh path (their rank 3), the Dancer's draws, the subtractive-blend
  restore.
- Their open question "how often the per-record shadow pass fires": 18 a frame in both traces.
- Their own caveat stands: nothing in it is measured. Its ranking is work per call times proven frequency.

## 11 Oct (later still): their source compared with ours; the effect drain measured

- docs/DECOMP_COMPARISON.md: the outside decompilation against `tree/`. In brief:
  - 176 functions are in both;
  - both agree with the binary's `ret N` on stack arguments (169 of 169);
  - their `void` for 75 functions whose return ours keeps is right in practice: no direct caller reads EAX;
  - 110 of 176 make the same virtual calls; the rest that were read are differences of representation;
  - 227 of 280 sampled files of theirs compile; none can run in the game.
- **The effect drain** (their rank 5), in the plaza trace (frame 2, label 0): 203 queued elements drawn, 29 to 79
  device calls each from setup to restore (mean 37.3, the element's draw included). That is about 7,570 calls, 18% of
  the frame's 41,093, the largest block after the character draws.
- Still not tested: their 70 entries that could not be mapped, and the load-time claims (format checks per texture,
  zone entry).

## 11 Oct (last): the outside write-up's remaining entries and load-time claims

**The 70 unmapped entries.** 28 more were mapped (DECOMP_COMPARISON.md); one call-link mapping was rejected because
its first cited instruction does not match. The 42 left are almost all stubs of 1 to 64 bytes. With the 28, 3,341 of
3,453 citations match (96.7%).

**Claims in the newly mapped entries:** the distortion ring sends SetVertexShader(0x152) once per band, as they say:
our loop 0x1004394B-0x1004398B calls slot +0x130 with 0x152 before each band's DrawPrimitiveUP.

**Load-time claims, tested in this build's code.** Static, so "reachable" says the path exists, not how often it
runs.
- **Format search** (their 102d20dd, our 0x102D270D): confirmed.
  - The table at 0x10345730 has 43 records of 0x24 bytes; A8R8G8B8 is record 2, A1R5G5B5 8, DXT1 28, DXT3 30.
  - One CheckDeviceFormat per record until the requested format is found supported.
  - A texture made through TexObj_Create reaches the search twice (D3DXCheckTextureRequirements, then
    D3DXCreateTexture through TexObj_Upload): 56 calls for DXT1, 60 for DXT3. Their "62 to 64 per DXT texture" is not
    reproduced; other calls inside D3DX were not counted.
- **Texture pump** (their 1003aae0, our TexMgr_UploadPump 0x1003ABB0): confirmed. It stops after 21 creates
  (`cmp ebp, 0x14` / `jg`), counted only while `[[0x10456a28]]` is 0x60 and `this+0x10` is clear; re-uploads (state 2)
  are never counted.
- **Temporary texture** (their 10039d80, our TexObj_Upload 0x10039E50): confirmed. The temporary is made at
  0x10039EC9 (D3DXCreateTexture), decoded into with D3DXLoadSurfaceFromMemory, and copied at 0x1003A89F
  (D3DXLoadSurfaceFromSurface).
- **Startup cursors** (our gate-verified 0x100091B0): confirmed. 18 cursor files (table 0x1034FBB0, mousenor.ani to
  arw_nw.ani) are loaded with LoadCursorFromFileA whenever GetCurrentDirectoryA succeeds; no cursor-option test.
- **Character mesh task** (their 1002c8c0, our 0x1002C990): structure confirmed. State word +0x38 through the jump
  table at 0x1002CB60, sub-step +0x3C, states 0 to 8, the flag at +0x120. Their "at least 18 calls" was not counted.
- **Bone-use flags** (their 10029ff0, our 0x1002A0C0): confirmed. Each rebuild clears the block and re-marks every
  node, and it is called per node add (0x10029E50, 0x10029F70): k(k+1)/2 node passes for k adds.
- **Zone exit** (their 10073a50): confirmed. Each destroy (0x100721F0 -> 0x10073B80 -> 0x100720B0) searches the
  singly linked list from [0x1047D124] along +0x2C (0x10071FD0) for the predecessor: quadratic in the node count.
- **Zone entry** (their 10178c80, our 0x10178DF0): confirmed as a path. Direct calls reach the resource manager's
  blocking fetch (0x10072FD0), the file-event dispatch (0x1019F960), a 0x40000-byte chunk reader with Sleep
  (0x101BE590), CreateFileA, ReadFile, CloseHandle and WaitForSingleObject, with no hand-off to another thread on
  that path.
- **Dancer** (their 10250ed0, our 0x10251500): confirmed. 0x10251A80 on its open path pushes 0xB00010 bytes (11.0 MB).
- **Not tested:** the cube-texture build's "about 221 device calls"; "nothing evicts a texture"; the per-frame
  re-poll of resource requests.

## 11 Oct (done): the outside write-up, last claims

- **Mapping.** Six more of its entries were mapped by content, each checked against an independent fact
  (DECOMP_COMPARISON.md): 733 of 769 now, the 36 left stubs or functions that nothing tells apart.
  - TransformPointX87 is slot +0x50 of our x87 math-tier vtable (0x1006D720). The SSE and 3DNow! tables' slot +0x50 is
    where their two other variants had already been mapped by citations.
- **Cube textures** (their 100751b0, our Gfx_InitCubeMapTextures 0x10075280): confirmed.
  - 3 cubes (count stored at 0x10075320), edges 32 << n, faces from the table at 0x103546A0 with face 5 skipped:
    15 faces;
  - per face: the push (0x1006A070: 4 SetTexture, SetRenderTarget, SetViewport), SetVertexShader, the draw, the pop
    (0x1006A0E0: 4 SetTexture, SetRenderTarget, SetViewport): 14 calls;
  - with the 5 render states and 3 cube textures, 218 plus the depth surfaces: their "about 220".
- **Texture cache:** confirmed.
  - The unlink helper (0x1003AB10) has two callers, CYyTex_Destructor 0x10039970 and 0x100399D0: an entry leaves the
    cache only when its owner releases it; nothing evicts by size, age or use.
  - Appending (TexMgr_Enqueue 0x1003AB80 -> 0x1003AB40 -> 0x1003AAF0) walks the +0x38 links to the tail each time.
- **Resource requests** (their 100729a0, our 0x10072A70): partly confirmed.
  - State at +0x41, the started-request limit `cmp eax, 0x10` at 0x10072BC2.
  - The state-1 poll (0x101C4AD0) resolves the file name again through XiFileImpl_getFileName on each poll.
  - Their semaphore search of the request ring in the same poll was not traced.
- **Mesh build** (their 1002c8c0): confirmed.
  - CreateBuffersStep (our gate-verified 0x1002CCB0) returns false for steps 0 to 6 and true at 7: 8 calls.
  - A failed CreateIndexBuffer or AcquireVertexBuffer frees everything but keeps the step, so the next call retries:
    no failure exit.
  - Their minimum of 18 calls rests also on the index fill (0x1002CE20) taking 2, not counted here.
  - Their summary's "every mesh request creates its own buffer" is overstated. AcquireVertexBuffer, called with 0
    (pooled), reuses a released buffer of the same rounded size, usage and pool once it has aged to free; only the
    warm-up's 200 buffers (state 4) never are. Their entry's own wording (one usage, format and pool; sizes rounded up
    to 2,000 bytes) is right.
- **Format search, per texture: not settled.** Per search: DXT1 28 calls, DXT3 30. Two searches per texture made
  through TexObj_Create: 56 / 60. Their 62 to 64 would need further CheckDeviceFormat calls inside D3DX. Those cannot
  be told apart statically from other interfaces' slot 0x28, and counting them needs D3DX run against a device that
  answers like a real one.

## 11 Oct: the outside write-up as context (names, address table)

- **Names.** 38 functions shared with the write-up still had `sub_` names here. 36 of its names were checked against
  our disassembly and taken (NAMES.md, 11 Oct; `data/names_pending_idb.json`, `ida/ffxi_ida_fix.py` regenerated).
  Two were not: 0x1006B480 and 0x101D0B00, where nothing in the code supports the role given. Two of its entries
  were wrong in detail: 0x10183490 is called (from 0x100431B0), and 0x1003EB30 is a wrapper of 0x1003E910.
- **Address table.** `data/outside_writeup_map.json` and `pipeline/outside_lookup.py` (DECOMP_COMPARISON.md, "How to
  use theirs"): 733 of 769 entries mapped, 35 unmapped, 1 rejected.

## 11 Oct: the effect drain as a performance target (pipeline/effect_drain.py)

Plaza trace (session 2, process 5180), label 0, the nine frames 0 to 16. The figures are for frame 2; the other eight
are within a few percent.

- **What it is.** Each queued effect element runs YmElem_SetupDevice (0x100467E0), its draw, then YmElem_RestoreDevice
  (0x100472F0). That is 203 elements, 7,580 calls (18% of the frame's 41,093), 29 to 79 calls each.
  - Setup makes 4,400 of them (about 22 per element): mostly SetRenderState, then a texture-stage block.
  - Restore makes one SetTransform and applies a render-state block.
  - Between two elements there are only two calls: OnDraw's SetRenderState 0xAB and one SetTransform (201 of 202
    gaps).
- **Why speedfix's cache misses it.** The default cache skips 46 of the 7,580 calls, and the extended cache none
  more. 544 ApplyStateBlock calls fall inside the drain, 2.7 per element. Speedfix forgets everything at each one,
  since a block may set any state, so the sets after it are always sent.
- **What the blocks hold** (the game's own code):
  - 0x1006AF00 and StateBlock_ApplyTSS (0x1006AF90) record a block once (token -1): BeginStateBlock, one
    SetRenderState or SetTextureStageState per table entry up to 0x7FFFFFFF, then EndStateBlock. After that they
    only apply it.
  - Setup passes table 0x10353598 at 0x10047207: stages 0 to 3, each with TEXTURETRANSFORMFLAGS 0, TEXCOORDINDEX 0,
    MAGFILTER 2 and MINFILTER 2.
  - Restore passes table 0x10353668 at 0x10047371: 16 render states (LIGHTING 1, FOGENABLE 0, CULLMODE 1,
    ALPHABLENDENABLE 0, ALPHATESTENABLE 1, ALPHAFUNC 8, ALPHAREF 0x60, ZENABLE 1, ZWRITEENABLE 1, ZFUNC 4, ...;
    state names from d3d8types.h).
  - Both tables are in `.data`, and each is referenced once. That they are not written at run time is not proven.
  - In the trace, token 8 always follows a setup call, and token 0xA always comes just before restore's SetTransform
    (1,812 of 1,812 across the nine frames).
- **Simulated over the whole frame** (device calls that would reach the device; drain window in brackets):

  | Settings | Frame | Drain |
  |---|---|---|
  | none | 41,093 | 7,580 |
  | defaults | 27,688 | 7,534 |
  | + extended cache | 24,347 | 7,534 |
  | + extended cache, blocks known | 22,391 | 5,783 |
  | + lazy_state | 20,645 | 7,355 |
  | + lazy_state, blocks known | 18,423 | 5,138 |

  "Blocks known" means that applying a block sets speedfix's shadow to the block's values instead of forgetting.
  Only the drain's two blocks are modelled: tokens 8 and 0xA, of the 13 tokens seen in label 0. The others are still forgotten,
  so the frame figure is a lower bound for that change. Speedfix would learn every block by watching its recording,
  which it already marks (`g_recording`).
- **Not a fix: restoring once per run.** Applying the restore block only at the end of a run of elements (a game-side
  change) gives 18,927, more than the block-aware cache alone. Without the restore, each element starts from what
  the previous one left, and its setup changes more.
- **What it rests on, and what is not known:**
  - That a recorded block holds exactly the states set during the recording. This is the DirectX 8 SDK's
    documented behaviour for BeginStateBlock/EndStateBlock; the documentation is not in this repository, and it has
    not been checked on a device.
  - What happens to recorded blocks on Reset, and to token reuse after DeleteStateBlock. Speedfix would have to
    forget a block's contents on both.
  - Calls are counted, not time: applying a block still costs the driver its 16 states. The lever A/B decides.

## 11 Oct: block_cache (speedfix) and the state-block check (probe8)

- **The documentation.** Microsoft's reference for IDirect3DDevice8::BeginStateBlock (archived Windows CE .NET 4.2
  pages, learn.microsoft.com/previous-versions/ms889280; EndStateBlock ms889304; ApplyStateBlock ms889278):
  - the setters called between BeginStateBlock and EndStateBlock are recorded in the block (LightEnable,
    SetClipPlane, SetIndices, SetLight, SetMaterial, SetRenderState, SetStreamSource, SetTexture,
    SetTextureStageState, SetTransform, SetViewport, SetVertexShader);
  - "The ordering of state changes in a state block is not guaranteed. If the same state is specified multiple times
    in a state block, only the last value is used";
  - 0xFFFFFFFF is an invalid handle; a block cannot be applied while another is being recorded.

  The pages do not say outright that applying a block leaves every other state alone, or what Reset does to blocks.
  Wine's d3d8 tests (dlls/d3d8/tests/stateblock.c) check that apply does not overwrite constants the block did not
  set. Wine is a reimplementation, not the runtime the game uses.
- **The check on the real thing.** probe8 now records a block and applies it through the `d3d8.dll` next to it,
  before its bench (`stateblock.record`, `stateblock.apply`, `stateblock.after Reset`; the phase 0 report prints
  them). It checks four things: the sets are recorded, not applied; the block sets ALPHAREF to its last recorded value
  and MAGFILTER; ZFUNC and MINFILTER, set after recording, stay. Under Wine: PASS, and the token still applies its
  values after Reset. On the owner's machine that `d3d8.dll` is dgVoodoo's, so `run_phase0.bat` there answers it for
  the game.
- **block_cache** (speedfix, off by default, live both ways; kit/speedfix/README.md):
  - Speedfix follows every block recorded on the render thread: its render and stage states with their values, the
    texture stages it binds, and whether it records any other setter it sees.
  - With the lever on, applying a followed block sets those states in the shadow, makes the texture stages (and, if
    any, the other kinds) unknown, and keeps the rest.
  - A block stops being followed at DeleteStateBlock, CaptureStateBlock, CreateStateBlock with its token, and Reset.
    That last one is conservative: under Wine the block survives Reset, but on dgVoodoo that is not known yet.
  - CaptureStateBlock, DeleteStateBlock and CreateStateBlock now have their own hooks, so the minimal start hooks 16
    methods instead of 13.
  - Offline (test_speedfix.py, new mode `blocks`, fake device):
    - after a block, its values and the states it does not touch are cached;
    - a different value still goes out;
    - a block that binds a texture or sets the shader makes those unknown;
    - Capture and Create make a block unknown;
    - switching off and on while running works, and a block recorded while the lever was off is followed.

    All four modes pass, and test_probe.py passes.
  - Simulated effect on the plaza trace (above): 24,347 to 22,391 calls a frame with the extended cache, 20,645 to
    18,423 with lazy_state; more if other blocks behave the same.
  - Still to measure: frame time (`ab_levers=block_cache`), and probe8's answer on dgVoodoo.

## 14 Oct: the runs that finish goal 1's research (kit/sessions/goal1_finish)

What was still open for the performance patch, put into three runs for the owner (README there):
- **Run 0, probes.** probe8 now also logs, through the d3d8.dll beside it:
  - its vertex and pixel shader versions, MaxVertexShaderConst, the blending caps and UBYTE4 support (the limits GPU
    skinning would work within);
  - the cost of the game's occlusion readback pattern: a probe drawn into a 16x16 render-target texture,
    CopyRects into a lockable 16x16 target, LockRect(READONLY) at once (the pattern read from the game's code,
    11 Oct; imitated, not the game's code). It times one query in an empty frame, one and eight after 1,600 draws,
    and eight probes with no lock, which is what an answer a frame late would cost.

  Under Wine (llvmpipe, numbers not meaningful for the owner's machine): 256 constants, vs 1.1, ps 1.4. The first
  lock after 1,600 draws waits for all of them (68 ms there), later ones in the same frame add little, and no lock
  costs nothing. That is the shape of the owner's measurement of the game (4-5 ms for the first query, 1.2 ms for
  each further one).
- **Run 1, levers.** Every lever measured as a gain is on (sse_skinning, batch_up_draws with lazy_state). The A/B
  measures the three never measured in the game: block_cache, skin_merge, extended_state_cache. vs_dedupe is on
  (it acts at load: its counts and the spike log, not the A/B).
- **Run 2, check.** Two alternating traces with pictures (the hooks of the kit and of speedfix off and on in
  alternate frames, so the pictures compare speedfix's levers), the sampler and the profile with the whole patch
  on, and facts=1. facts=1 is there for the occlusion readback's call at 0x1006C8CD: it is in xikit_facts.txt, but
  no run so far reached it (the facts run of 6 Oct stood still, and readbacks stop when no character comes into
  view), so it is walked through the crowd.

## 14 Oct: groundwork of the port (tasks 26-29)

- **The scope as data** (`data/d3d8_scope.json`, pipeline/d3d8_scope.py):
  - the static call-site map (62 methods on 7 interfaces, 27 sites of interfaces not established);
  - the owner's traces 6080 and 5180: 33 methods, 25 render states and 35 stage states with their values, 6 FVF
    codes;
  - five resource logs (every creation's usage, format, pool, FVF);
  - the profiles' calls per frame.

  Each source is kept apart.
- **kit/d3d8on11, milestone 1:** our d3d8.dll on D3D11, everything but executing draws (D3D11.md, phase 1; README
  there).
  - `gen_stubs.py` makes every D3D8 method log "not implemented" and fail until it is written.
  - Commands are recorded and executed apart from the start.
  - Tested under Wine: the same program through Wine's d3d8 and ours, 22 checks each, all pass. A fresh device's
    480 states compared: 2 D3D8 states differ (DEBUGMONITORTOKEN, POINTSIZE_MAX); the other 28 are numbers D3D8
    does not define.
  - In tool_drift and BUILD.lock.json.
- **probe8** also dumps the whole D3DCAPS8 (`caps.raw`). The device's caps are provisional until they can answer as
  dgVoodoo's do on the owner's machine. The game chooses its paths by caps.

## 14 Oct: a new game build (5 Oct, TimeDateStamp 6AC37846), and build translation

- **Why the owner's runs wrote nothing:** the game updated. The kit checks the build stamp and did nothing on a build
  it had no manifest for.
- **The new build, unpacked** (pipeline/unpack_pol.py): the shipped DLL's entry stub decompresses POL1 into .text.
  Run in an emulator, it reaches .text at 0x1031676F; the 3 Sep build's entry was 0x1031672F.
- **What changed** (pipeline/build_translate.py; build_map.py matched 18,690 of 18,697 functions):
  - code moved +32 from 0x10232A90 and +64 from 0x10309EC0;
  - the first insertion is in two handler functions reached through pointers, 0x10232970 and 0x10232A30. Each now
    calls 0x10230930 with its ID minus 0x2000 and returns early when that is true. Not graphics code; what they
    handle is not established;
  - the second insertion is 32 bytes of int3 padding;
  - data does not move. The changed dwords in .rdata and .data are pointers into moved code, timestamps and the
    build-date string.
- **What it means for us:** all 774 manifest functions are proven the same code (56 of them moved by 32). Every
  address the reconstruction, speedfix and the SDK name is settled. Nothing needs re-analysing; the kit translates.
- **Build translation** (docs/KIT.md):
  - `kit/xikit/builds/<stamp>.json` and `xikit_manifest_<stamp>.txt` per build;
  - `xi_shift` beside `xi_rel` in the reconstruction and the mods (`XI_GAME` applies it; data accesses unchanged);
  - the kit and d3dprof report analysed addresses on any build.
- **Tests:** test_xikit.py and test_speedfix.py pass on both builds. On the new build, 774 functions swapped, none
  refused, and the reconstruction received both moved ranges. test_fork.py passes.
- **Two test checks fail intermittently on both builds,** and on the commit before this work too (a worktree build of
  a888779): profile mode's "samples taken while the thread ran the reconstructed GetColor name it", and the mod
  modes' device-hook checks. They passed on reruns. Not caused by this change; not yet explained.
