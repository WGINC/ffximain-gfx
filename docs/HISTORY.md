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
- **kit/d3d8on11, milestone 2: drawing** (`d8_ff.cpp`; README there).
  - Fixed-function state becomes vertex and pixel shader keys, then HLSL compiled with d3dcompiler_47, cached.
    D3D11 state objects are cached by description. Fans become lists; ...UP draws use a ring buffer.
  - Picture test (`t8 scenes`): 20 scenes through Wine's d3d8 and ours. The first run matched 2 of 13, and every
    difference had a cause found:
    - a stage reading an unbound texture: Wine passes CURRENT on (`is_invalid_op`); the DirectX 8 reference
      documents only ALPHAARG1's fallback to DIFFUSE;
    - no texture transform on pre-transformed vertices or with COUNT1 (Wine's `compute_texture_matrix`);
    - the fog model: a per-vertex coordinate and a per-pixel factor (Wine's `get_fog_start_end` and the GLSL fog
      code);
    - the half-pixel offset for transformed vertices had the wrong sign: geometry moves +0.5 px right and down;
    - fan flat shading takes the first non-spoke vertex (Khronos EXT_provoking_vertex, issue 7: Direct3D 9's
      behaviour).

    After those, all 20 match within 3 of 255.
  - Not drawn yet: the game's own vertex programs (milestone 2c), pixel shaders, vertex blending. Each is logged.
- **probe8** also dumps the whole D3DCAPS8 (`caps.raw`). The device's caps were provisional until they could answer
  as dgVoodoo's do on the owner's machine (done: see "13 Oct runs" below). The game chooses its paths by caps.

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

## 13 Oct runs (kit 0ff09cd, received 14 Oct): run 0 (probes) and run 1 (levers) of goal1_finish

Run 2 (the picture check, the time samples with the whole patch on, the readback call's pop) has not come in. The
owner notes the movement timing of run 1 may have been off.

**The machine, as the kit logs it.**
- Windows 11 on ARM64 (kernel 10.0 build 26200), the game emulated x86.
- "Virtual CPU" at 3.07 GHz with SSE to SSE4.2 and no AVX.
- Parallels Display Adapter (`prl_umdd10.dll`), 4,092 MB shared and no dedicated memory.
- d3d8.dll in the game folder: dgVoodoo ("dgVoodoo DX API Layer"), file version 4.8.2.134.

**Run 0, probes (phase0_20261013_214301).**
- State blocks on dgVoodoo:
  - stateblock.record PASS: between Begin and End, a set is recorded, not applied.
  - stateblock.apply PASS: applying sets what the block recorded (ALPHAREF at its last value, 0x61) and nothing
    else.
  - After Reset, the old token still applies (ALPHAREF 0x61); found, not judged.
  - block_cache's premise holds on this d3d8.dll.
- Caps (data/dgvoodoo_caps8.json decodes caps.raw):
  - vertex shader 1.1, 256 constants;
  - pixel shader 1.4;
  - 4 vertex blend matrices, MaxVertexBlendMatrixIndex 255, UBYTE4 bone indices in a vertex shader supported;
  - 16 streams, stride 65536, 8 lights, MaxPrimitiveCount 16,777,216.
  - For goal 2: a vs 1.1 program has 256 constants; 77 bones as 4x3 matrices take 231 of them. That this leaves
    room for the rest of a skinning shader is arithmetic, not a test.
- Occlusion readback on dgVoodoo, the game's pattern (median of 70 frames):
  - a query with nothing drawn: 0.54 ms;
  - one query after 1,600 draws: 3.40 ms;
  - eight queries after 1,600 draws: 6.26 ms;
  - eight probes answered a frame late (no lock): 0.18 ms.
  - A readback waits for the frame's GPU work before it; answering from the previous frame would avoid that wait.
- D3D11 against D3D8 per call and the synthetic frames repeat the earlier runs' picture: D3D8 7.55 ms against D3D11
  8.60 to 9.05 ms for the plaza-like frame, 0.17 to 0.31 ms merged. Cross-process present seen, shared texture not
  seen, as before.
- kit/d3d8on11 now answers GetDeviceCaps with these 212 bytes. The game takes the paths it takes under dgVoodoo.

**Run 1, levers (process 8948, new game build 6AC37846 through build translation).**
- Every lever on; A/B of each of the three, phases of 30 s (A B B A A B), the first 60 frames of each left out. Port
  Jeuno plaza.

  | Lever | A (off) | B (on) | B − A | Spread within a label |
  |---|---|---|---|---|
  | block_cache | 24.68 ms | 26.11 ms | +1.43 ms (+5.8%) | 3.94 ms (within it) |
  | skin_merge | 24.23 ms | 25.59 ms | +1.36 ms (+5.6%) | 5.29 ms (within it) |
  | extended_state_cache | 29.55 ms | 27.01 ms | −2.53 ms (−8.6%) | 4.25 ms (within it) |

- None is faster by more than the spread, so by the session's rule none becomes a default. All three stay off as
  they were.
- What this run cannot say: whether any of them helps or costs by less than about 4 to 5 ms. The phases of one
  label differed by 4 to 5 ms, as much as or more than any difference measured.
- The frame was 22.7 to 31.2 ms a phase. Session 5's frames (6 Oct, same plaza, older build) were 29.5 to 45.7 ms;
  the crowd and the machine's state are not controlled, so the two are not compared.
- block_cache at work: most applies had their contents known after the first minute, e.g. 2,579,458 known and
  138,980 not in one minute. The first minute (loading in) had 1,175,263 known and 1,075,114 not.
- lazy_state: about a third of held changes never sent (e.g. 6,098,785 of 21,719,067 in a minute).
- The cached setters skipped 51 to 61% of calls. batch_up_draws sent 86 to 90% fewer DrawPrimitiveUP calls.
- vs_dedupe: 3 vertex shaders made in the session; every later creation got one of them.
- The kit ran on build 6AC37846 through its translated manifest. Swapped functions: 2 at frame 1, none refused.

## 13 Oct run 2, check (process 6796, received 14 Oct)

Plaza, standing for the two traces, then walking through the crowd. Every lever on (sse_skinning, batch_up_draws,
lazy_state, skin_merge, block_cache, extended_state_cache, vs_dedupe), cpu_tier=1 (the SSE tier was in use). The
owner reports **exploding vertices during some parts of the run**.

**The pictures** (kit/d3dprof/pictures.py, two traces of 9 frames, kit and speedfix hooks off and on in alternate
frames):
- trace 1 (90 s): swapped frames 3.5 to 5.5% of pixels outside their neighbours' range; control 7.2 to 10.4%;
- trace 2 (210 s): swapped 0.47 to 0.53%; control 1.16 to 1.43%.
- Verdict for both: no swapped frame differs from its neighbours more than the control does. The marked pixels are
  the edges of animated characters, in swapped and control frames alike.
- The alternation switches skin_merge, sse_skinning and every device-hook lever per frame (sf_hooks_live), so in
  those frames the levers did not change the picture. No exploding vertices appear in these 18 frames. Both traces
  were taken standing; when the explosions happened is not in the data.

**The exploding vertices: what is established, what is not.**
- skin_merge cannot draw a held record's vertices from a rewritten buffer. In SkinAndDrawV2 (the fork, lines
  104-160) every Lock and Unlock of the model's vertex buffers, and Model_SkinVerticesCpu, come before the record
  loop. The merged draw passes the same vertex range (0, the model's count) as the game's own draw of each record.
- Not ruled out, from this data:
  - sse_skinning: the game's own SSE skinning loops, switched on live on a CPU whose vendor string the game never
    runs them for;
  - cpu_tier: the game's SSE math tier, chosen at load and not undone by Ctrl+Alt+F9;
  - the other levers;
  - the game itself on this machine (x86 emulated on ARM64, Parallels display driver, dgVoodoo).
- The way to settle it is a run where the owner switches things off while it happens: Ctrl+Alt+F9 (everything the kit
  and speedfix add, live), then the levers one at a time in speedfix.ini (re-read live), then cpu_tier=0 at a start.

**The readback call's pop** (fact_sites.py ingest; data/game_sp.json):
- 0x1006C8CD: 12 bytes in all 80,805 observed calls. The target is slot 15 of the vtable the kit logs with LockRect at
  slot 16 (a texture), i.e. IDirect3DTexture8::GetSurfaceLevel(this, level, ppSurface): three DWORDs in the D3D8
  header, 12 bytes.
- The other sites measured again agree with the pop counts already recorded.
- The readback function 0x1006C8B0 could not be re-gated at first: the tree does not reconstruct it (it calls the
  game's code), and no translation of it was in the repository. It has since been translated and gated: PASS
  ("14 Oct: the occlusion readback 0x1006C8B0 translated and gated").

**Where the time goes with the whole patch on** (kit/d3dprof/samples.py; the last 600 frames, 10,977 samples at
1 ms, during the walk):
- Time in other modules (Direct3D, driver, Windows), by the game function that called there:
  - the occlusion readback 0x1006C8B0, its LockRect (return 0x1006C91F): 26.8%;
  - SkinAndDrawV2 through 0x1002A000 (return 0x1002A034 is after its call to 0x1002DC10; the forked copy runs in
    speedfix): 12.1%;
  - CMoVuElem_Draw: 7.6%;
  - Gfx_SetStreamSourceCached 0x1002F130: 5.4%;
  - CTsZoneMap::DrawProjectedShadow: 4.0%;
  - CDx::SetTransform: 3.6%.
- The render thread itself: 68.8% of samples in memory of no module, as in session 5. (Corrected below: 66% are
  one address, 0x00010002.)
- The occlusion readback is the largest single item while walking: a quarter of the render thread. Phase 0
  measured what an answer a frame late costs on this machine (0.18 ms for eight, against 3.40 ms for one LockRect
  after 1,600 draws). That is the asynchronous query of goal 2 (docs/D3D11.md section 3).

## 14 Oct: the occlusion readback 0x1006C8B0 translated and gated (task 31)

- **Translated** from the committed Hex-Rays export (data/exports/gfx_export.jsonl) by the pipeline's own steps:
  hx2cpp.py, build_raw.sh, gfx_cases.py.
- **Gate results, one step at a time:**
  - The recorded verdict was UNRESOLVED: call 0x1006C8CD, pop count unknown.
  - With run 2's fact (12, data/game_sp.json) the gate went one call further. It stopped at 0x1006C907 (slot 2 of
    the object GetSurfaceLevel returned), because nothing in the case said what the objects are.
  - The objects are now declared for this function (data/global_com.json, read by gfx_cases.py), each with its
    source:
    - [[0x1047C004]+12] is an IDirect3DTexture8: run 2's facts, its call target is slot 15 of the vtable the kit
      captured as the texture's;
    - [[0x1047BFA8]+0xEBC] is the device: data/com_calls.json, site 0x1006C8FD, CopyRects;
    - this+4 is an IDirect3DSurface8: it is CopyRects' destination parameter in d3d8.h, and the same field is locked
      through slot 9.
  - With those: PASS, but 89 of 100 trials faulted reading address 0. The harness did not fill LockRect's
    D3DLOCKED_RECT, so pBits was 0.
- **Harness addition** (pipeline/diffemu_gfx.py, LOCKED_RECT_OUT):
  - LockRect on a fake surface, texture or cube texture now writes Pitch and a pBits into readable pool memory,
    parameter positions from d3d8.h.
  - Its values come from a stream of their own (trial seed, call number). Functions that never lock draw from the
    trial stream as before.
  - Checked against the old harness on every gated function with a call through +0x24 or +0x40 (18 besides this
    one): 17 identical; 0x1000C900 now returns in 58 trials instead of 32, 138 D3D calls compared instead of 68,
    PASS either way. Its line in verification/gate_raw_final.jsonl is the old harness's.
- **Result:** PASS, 100 trials, 82 returned (through the pixel-counting loop over the locked memory), 18 faulted the
  same way in the game's own code (a failed GetSurfaceLevel leaves the surface pointer unset and it is released), 0
  mismatches, 435 D3D calls compared, nothing unresolved.
  - Also PASS on the state of the one full dump in this container (runs/dump/pol_full_dump.dmp: 431 D3D calls).
  - verification/cases_raw_final.json and gate_raw_final.jsonl carry the new case and verdict.
- **Not in the clean tree:** the tree is promoted on fuzz and the four dumps (town, town2, combat, zone to inn), and
  those dumps are not in this container. The function stays a raw PASS until it is gated there.
- data/global_com.json declares the objects for this function only. The same device sits behind 0x1047BFA8 for 102
  other gated functions; declaring it for them would strengthen their gates and needs them re-gated.

## 14 Oct: corrections to what we understood (the owner asked for them before any more porting)

Each checked against the owner's own run files.

**1. The game draws with no vertex or pixel shader programs. Every draw seen is fixed-function.**
- Before: the scope listed the game's 5 vertex and 1 pixel shader programs (data/shader_catalogue.json) and "281
  shader handles". The device's README and D3D11.md read that as "the game's own programs, not drawn yet,
  milestone 2c", and task 32 was to translate them.
- What the runs show:
  - Every vertex shader the kit captured in the owner's sessions (2,253 xikit_vshader_*.bin, 6 to 13 Oct) has 0
    function tokens: a declaration only.
  - No CreatePixelShader appears in any resource log.
  - The only two captures with code in this container are test fixtures made here from the catalogue's tokens
    (scratchpad xikit_vshader_1_0000004{2,3}.bin), not the game's.
  - The 281 handles are these declarations.
- Run 2's three distinct declarations:
  - handle 3: stream 0 position, stream 1 normal, stream 2 texcoord. These are SkinAndDrawV2's three streams,
    strides 12, 12 and 8: the characters.
  - handle 5: stream 0 position; stream 1 normal, diffuse and texcoord.
  - handle 7: one stream with position, normal, diffuse and texcoord.
- The programs are built only by 0x10178250. Its one caller is a branch of 0x10178B30, a virtual method (vtable
  entry 0x10336768) that also calls Zone_RenderEnvironment. Which zone or state takes that branch is not
  established; no session reached it.
- Consequence: the port needs no shader translation for anything observed. Task 32 is closed as not needed. A
  program is logged if it ever comes.
- kit/d3d8on11's picture test now draws the character layout (three streams, indexed, lit, textured). It matches
  Wine. One case was found and kept out of the test: where a texture coordinate is exactly 1.0 between vertices,
  point sampling ties between two texels, and Wine and D3D11 break the tie differently. Wine nudges its GL
  geometry for D3D's fill convention (wined3d get_projection_matrix). How Windows' D3D8 breaks it is not
  established.

**2. "68.8% of the render thread in memory of no module" is one address, not unknown code.**
- 7,230 of run 2's 10,977 samples (66%) are at EIP 0x00010002. That is not a code region in any module or heap the
  kit knows.
- The process is x86 emulated on ARM64. What 0x00010002 means there is not established. One reading, not
  verified: the emulator shows that x86 context while the thread runs native code (the system's D3D11, the
  Parallels driver) or waits.
- The "elsewhere" view (time by the game function on the stack) is not affected: it reads return addresses, not
  EIP. So "the readback's LockRect is 26.8%" stands as time under that call, not as time in x86 code.

**3. The resource log's module column was wrong for calls through speedfix's device hooks.**
- Example: 18,283 CreateVertexBuffer at 0x1003B334 were labelled "no module". The call-site column already named
  the game's own return address (d3dprof_via_stk); the module column used the immediate return address, which is
  the kit's thunk memory.
- Fixed (kit/d3dprof/d3dprof.c caller_ra): both columns use the same caller. Earlier logs: read the call-site
  column, not the module column, for such rows.

**4. What stands, rechecked:**
- The machine: Windows 11 on ARM64 under Parallels, the game emulated x86. The GPU is the Parallels Display Adapter
  (shared memory only), reached through dgVoodoo 4.8.2.134, which is a D3D11 layer (D3D11.DLL and
  prl_umdd10.dll are loaded).
  - Every time measured so far, the readback stall above all, is on this one machine and virtual GPU. None of it
    is shown to hold on a native Windows PC.
- Hook.dll (Windower's) is in the process and patches three non-graphics game functions (METHOD.md, the dumps).

## 14 Oct: frame capture and replay (task 29)

- **The kit captures one whole frame** (`fcap=1`, docs/KIT.md "Frame capture"; kit/d3dprof/fcap.c). It holds every
  call with its data, every object the frame uses with its contents, the start state, state blocks written as the
  Set calls they amount to, and the picture the game's own d3d8.dll drew.
  - The default-pool buffers the game cannot read back are copied from creation on.
  - Buffers are wrapped by patching their class vtable, because Wine's d3d8 asserts that a buffer's vtable is its own.
- **The replayer** (kit/fcap/replay.c, dist/fcap_replay.exe) plays a capture into any d3d8.dll and writes both
  pictures; kit/fcap/compare.py compares them.
- **Tested under Wine** (kit/fcap/test/test_fcap.py): a game-like frame captured through Wine's d3d8.
  - Replayed through Wine's d3d8: identical to the captured picture.
  - Replayed through d3d8on11: within 1 of 255.
- **The first replay through d3d8on11 found three gaps, now fixed:**
  - normals were normalized whenever lighting was on. D3D8 normalizes only with D3DRS_NORMALIZENORMALS (the DirectX 8
    reference; Wine does the same). The 20 picture-test scenes had near-unit normals and an unscaled world matrix, so
    they did not show it;
  - SetClipPlane and GetClipPlane were missing. They are now kept, read back and recorded in blocks; drawing with
    CLIPPLANEENABLE set is logged as not implemented (the game's traces never set it);
  - CopyRects from an image surface into a GPU surface, and between image surfaces.
- **What it is for:** a real plaza frame from the owner's game, replayed through dgVoodoo and through d3d8on11 on the
  owner's machine and here, compared with the picture the game drew. That is phase 1's check (D3D11.md). No such
  capture exists yet.

## 14 Oct: the port's phase 2 and ARM mode (tasks 35, 36)

kit/d3d8on11 is split in two halves joined by a command stream, so the D3D11 work can run on the game's thread, on a
thread of its own (design A2), or in a process of its own built natively for ARM64 (design B). The owner asked for
"an ARM mode, or do what you think is right": both designs are in, chosen at start.
- The stream (`d8_proto.h`): fixed-width records with ids instead of pointers, the same layout in x86 and ARM64
  builds. A draw is one packet: the shader keys, the constants, the D3D11 state descriptions, the bound ids, and
  for ...UP draws and fans, the vertex and index data. Four records are answered; the game waits only for those,
  for room, and when it is two Presents ahead.
- The executor (`d8_exec.cpp`): the device, swap chain, resources by id, and the HLSL generator, moved unchanged.
- The transports (`d8_link.cpp`): `direct`, `thread` (default) and `remote` (d8x.exe or d8x_arm64.exe, started
  next to the DLL; on failure to start, the device falls back to `thread`; if it ends mid-game, the game is ended
  with a log line).
- Tested here, under Wine:
  - test_d3d8on11.py in all three modes: basic passes in each; the 20 scenes match Wine's d3d8 as before; `thread`
    and `remote` (x86 executor) draw the same pixels as `direct`;
  - test_fcap.py: the replay through ours matches its capture (worst channel difference 1);
  - the fallback with no executor beside the DLL;
  - the executor process ended at frame 30 (t8 orphan): the game ends with exit code 3 and the log line.
- Measured: a draw packet is 3,128 bytes in both builds (pinned by static_asserts), about 9.4 MB a frame at 3,000
  draws. What copying it costs is not measured.
- Not established:
  - d8x_arm64.exe runs: it is built (zig 0.16.0, imports only d3d11, kernel32 and the UCRT) but this container
    cannot run ARM64 code;
  - whether any mode is faster than `direct`, or than dgVoodoo, in the game: nothing here measures speed;
  - the fallback when the executor starts but cannot make its device (code exists, untested).

### Checks of the above (14 Oct, the same day)

- The rewritten draw path was compared with the old one, section by section: the keys, constants, state descriptions
  and sampler descriptions are the old code's, now filled into the packet; shader creation and binding moved to the
  executor unchanged.
- The draw packet's layout is the same under i686 MinGW-w64 g++ and zig 0.16.0's aarch64 clang (3,128 bytes; the
  offsets of its parts compared), and is now pinned by static_asserts, as is the ring header's (64 bytes).
- A 600-frame test in each mode found a hang that a change of that day had introduced (a stats counter named like the
  record size parameter, so every record header got a wrong size). It was fixed before any commit; the test stays.
- The ring's wrap (X_PAD) and the counters' wrap at 2^32 were never reached by the tests; now they are (64 KB ring,
  counters started at 0xFFFF0100). The ring's size must be a power of two for the second; the executor checks it.
- The executor process ending mid-game is tested (t8 orphan): exit code 3 and the log line.
- The kit on our device: xikit hooks it and writes its frame times (test_xikit `realdev_ours`; also `realdev` on
  Wine's d3d8). fcap captures through it; that capture replays through Wine's d3d8 to the same picture. Its call list
  differs from a capture through Wine's d3d8 only in the start state of the 30 render states known to differ (states
  D3D8 does not define, and 165), the vertex shader handle each device picks, and one undefined state (0xD3) Wine
  keeps in state blocks.
- test_xikit's `mod` run failed once and passed on the rerun, as it has before; its cause is not established.
- kit/sessions/port_first: the first runs in the game (README there).

### More checks, asked for ("have you tested everything you can?"), 14 Oct

- A randomized comparison with Wine's d3d8 (test/random_cases.py, the game's own state values): 13 of the first 200
  cases differed. Each was minimised automatically and traced:
  - a defect of ours: MIPFILTER NONE / MAXMIPLEVEL as an LOD clamp made a minified texture use MAGFILTER (fixed: a
    view of the levels instead; the Direct3D 9 reference's D3DTEXTUREFILTERTYPE);
  - a choice of ours with no source: pre-transformed vertices with generated coordinates read (0,0,0,1) (now the set
    as given, by Microsoft's "does not modify transformed and lit vertices");
  - four classes where Wine and the reference, or the reference and nothing, decide: A, C, S, L (kit/d3d8on11
    README). Each was checked, not assumed: the case changed only as its class says draws the same on both. S was
    confirmed with a one-colour cube (no difference) and a colour per face (differences only on face boundaries).
  - Seeds 1 to 5, 1,000 cases: no difference outside the classes.
- Threads: a second thread making objects while the first does too. With the old id counter the executor saw an id
  created twice in every run; the counter is atomic now. The pictures alone did not show it.
- Churn: 5,000 rounds of every kind of object; the executor holds nothing at the end.
- The remote executor failing to make its device: the device falls back to thread mode.
- Corrections of the day's own statements: "the game writes buffers from a second thread" was not established (the
  frame capture only guards against it); removed.

### Checked against Microsoft's documentation (14 Oct)

- The owner supplied a 549-page excerpt of the Microsoft DirectX 8.1 (C++) documentation (not in the repository).
  The lighting pages match d3d8on11's lighting term for term, with two exceptions and three gaps (kit/d3d8on11
  README, "Checked against Microsoft's documentation"):
  - The ambient term differs between DirectX 8.1 (no attenuation or spot factor) and Direct3D 9 (both); ours is
    Direct3D 9's. The game's 98,309 traced SetLight calls all have ambient 0, so it cannot show there.
  - The clamp of each light's N.L at 0, specular's N.L > 0 and N.H > 0 conditions, and the spot factor are in
    neither text; they are Wine's. The traces set no spot light.
- The excerpt has no render-state, stage-state, texture-operation, fog or rasterization pages, so those parts of the
  device are still checked only against Wine.

### Checked against the DirectX 8.1 SDK's own documentation (14 Oct)

- The owner supplied the DirectX 8.1 SDK (and the 8.1 extras, the DirectX 9 SDK and the June 2010 SDK). Its
  documentation (directx8_c.chm, 9,246 pages) and headers are the source now; they are not in the repository.
- Checked: render-state and stage-state defaults, texture operations and arguments, default vertex colours, texture
  coordinates and filtering, lighting, fog, flat shading, blending, alpha test, culling, Clear, state blocks,
  palettes (kit/d3d8on11 README, "Checked against Microsoft's documentation").
- Changed by it: three defaults (POSITIONORDER, NORMALORDER, AMBIENTMATERIALSOURCE); pixel fog's depth for
  pre-transformed vertices (1/RHW, not z); the specular alpha interpolated under flat shading; Clear of a viewport or
  rectangles (it failed; the game's own code clears right after SetRenderTarget, where ours resets the viewport to
  the target, so the change may not show in the game); D3DSBT_ALL without the index buffer and with the palette;
  palettes kept (both calls failed before; the game makes both).
- Where Microsoft's documents disagree, both are named: AMBIENTMATERIALSOURCE's default and the ambient term (8.1 and
  9 differ; the game's traced vertex formats and lights make both read the same), the specular default (two 8.1 pages
  against one).
- Not yet checked then (done below): CopyRects, LockRect, Reset, addressing and border colour, vertex declarations, Present.

### The rest of the DirectX 8.1 audit, and the June 2010 SDK's Direct3D 11 pages (14 Oct)

- Vertex declarations: registers 15 and 16 (D3DVSDE_POSITION2, D3DVSDE_NORMAL2) were not mapped (a draw through one
  was skipped). Now mapped, with tweening ("Vertex Tweening") when D3DRS_VERTEXBLEND is D3DVBF_TWEENING. The game
  was not seen needing it: its three captured declarations (above, 14 Oct) have neither register; v15 is only in its
  shader programs' declarations, which carry code and were never reached. SHORT2 elements did not compile; fixed to
  D3DVSD_REG's expansion.
- CopyRects validated as listed; a copy into a texture level updates the copy the game locks. Lock rules: DEFAULT-pool
  textures not lockable unless dynamic, READONLY sends nothing, a written render-target lock goes back to the target,
  a draw with a locked vertex buffer set fails (whether the game does that is not known: its traces record unlocks
  only). A failed Reset leaves the device "not reset" (TestCooperativeLevel and Present answer as the reference).
- The stricter CopyRects failed one call in kit/fcap's test host: the whole 32x32 level into the 16x16 target, which
  the reference says fails and Wine's d3d8 allows. The game's own call (0x1006C8FD) passes one rectangle, the constant
  {0, 0, 16, 16} at 0x1032C18C, into its 16x16 lockable target, whose format its constructor (0x1006C140) takes from
  level 0 of a texture (GetDesc); that this texture is the copy's source is not established. The host now makes the
  game's call.
- Addressing and border colour match; Present's arguments are not implemented, and the game passes NULL in all 34
  traced Presents.
- Not settled by any document here: where magnification turns to minification under MAXMIPLEVEL (8.1 and the
  Direct3D 11 pages are silent; the game was not seen setting MAXMIPLEVEL), and how a cube texture is filtered at
  face edges (the README's earlier "D3D11 always filters across faces" had no source and is withdrawn).


### Microsoft's own DirectX 8.1 runtime as a test reference (14 Oct)

The owner's copy of the DirectX 8.1 SDK holds the retail runtime (d3d8.dll), its reference device (d3dref8.dll) and
the runtime's kernel-thunk DLL (d3d8thk.dll) in dxnt.cab. Under Wine the runtime stops at the thunks, which call into
the NT kernel's DirectDraw. Our stand-in for d3d8thk.dll (kit/d3d8on11/test/msref/kthunk.c) emulates a display driver
with no 3D, using the structures in Microsoft's WDK 7.1 DDI headers (downloaded from Microsoft; not in the repository).
Each thunk's argument count comes from the real DLL's `ret N`. With it, `t8` gets a D3DDEVTYPE_REF device that draws
in software. The SDK's debug runtime (d3d8d.dll) runs the same way and reports its validation errors through
OutputDebugString. test_d3d8on11.py compares against it when D3D8REF_DIR and WDK_DDI are set.

What it showed:
- Vertex fog. Microsoft's runtime makes the factor per vertex, clamps it to [0, 1] and interpolates it. Ours (Wine's
  model) interpolated the distance and made the factor per pixel, so a floor whose near vertex is nearer than
  FOGSTART was unfogged where Microsoft's device fogs it slightly. On an untextured copy of the scene, rows 41-59
  differed by up to 6 levels. Worked out from the scene's geometry, the reference device's values are the clamped
  factor interpolated perspective-correctly. The DirectX 8.1 text says the same ("it applies fog calculations at
  each vertex in a polygon, and then interpolates the results"; "The system stores the fog factor in the alpha
  component of the specular color for a vertex"). Changed: VSKey.vfog; the draw packet grew by 4 bytes (the layout
  checks were re-measured on both compilers), and the ring version is now 2.
- Defaults. D3DRS_AMBIENTMATERIALSOURCE is D3DMCS_MATERIAL on Microsoft's runtime. The 8.1 text says COLOR2 in
  D3DRENDERSTATETYPE and MATERIAL in "Color Vertices", and ours had taken the first. D3DRS_DEBUGMONITORTOKEN answers
  0xBAADCAFE, where the text says D3DDMT_ENABLE. Both changed to the runtime's values. The other 288 states compared
  are equal.
- The debug runtime rejects fixed-function declarations the retail runtime draws: a FLOAT4 position ("Position
  register must be FLOAT3 for fixed-function pipeline") and SHORT2 texture coordinates ("Invalid data type set for
  texture register"). Retail drew the first without its colours and the second with one flat texel colour.
  declaration_fan now uses FLOAT3. decl_short2/4 check our expansion only and are not compared. No validation was
  added to ours: the game runs on the retail runtime.
- ZBIAS cannot be compared: the reference device does not report D3DPRASTERCAPS_ZBIAS (RasterCaps 0x00772199) and
  ignores it.
- Every other differing pixel is a tie, computed from each scene's geometry:
  - xyz_depth_cull: 7 pixels within 0.013 px of an edge;
  - fog: 13 pixels exactly on the floor's side edges; the other fog-scene differences came from the checker texture
    (on texel boundaries, or 0.004 px from one at the horizon), and the fog scenes are untextured now;
  - texture_transform_2d (301 pixels), stages_blend_alphatest (141), texgen_cameraspace_transform and
    v15_declaration (the centre row and column): point samples exactly on texel boundaries;
  - three_stream_declaration (19,50): v = 25.994 texels;
  - texgen_reflection_cube: three lookups within 0.006 texel of a checker boundary or within 0.2% of a face edge.
  Nudging our geometry by 1e-4 px did not flip those four, so they are ties at the 0.01 px scale, not 1e-4. The
  test's criterion takes a pixel as matching when a 3x3 neighbour matches both ways, and it names those four.
- D3D11.3 Functional Spec (microsoft.github.io/DirectX-Specs) 7.18.12 says Direct3D 11 filters cube textures across
  faces. That settles what ours does; what Direct3D 8 hardware did is still not established. 7.18.11: magnification
  against minification is decided after the sampler's LOD clamp from feature level 11.0, and undefined before it.

### The randomized cases against Microsoft's runtime; a run for the owner's machine (14 Oct)

Two faults in the randomized test itself, both fixed:
- The scope (data/d3d8_scope.json) lists at most 24 values for a state and ends a longer list with a "..." entry.
  random_cases.py drew that entry as a value; t8's sscanf skipped the line, so those states kept their starting
  value. The generator now leaves "..." out.
- Each case started from reset_state, which sets only some states. Whatever else a case set stayed set for the cases
  after it. Every implementation ran the cases in the same order, so the comparisons were like for like, but a case's
  text was not all of its state, and the classes (read from the text) and any bisection of a case could be wrong.
  Found when r0146 drew 829 pixels differently from Microsoft's reference in its file and 3,959 alone. t8 now reads
  each state a case sets before setting it and puts it back before the next case. Microsoft's runtime answers a real
  value for every state the generator sets (none of them is one it answers 0xBAADCAFE). Afterwards r0146 draws the
  same in its file and alone.

What the reference showed, case by case (bisected by taking out one line at a time and compared with the reference
criterion):
- Linear fog with FOGSTART >= FOGEND. Along a depth ramp (2.5 to 4.5) the reference device fogs fully where the depth
  is at or past FOGEND and not at all before it, whatever FOGSTART is: start 4/end 3, start 4.5/end 2.5, start 100/
  end 3.5, start 3.5/end 3.5 and start 3.5/end 0 all step at end. With start < end it is the formula, as ours. The
  DirectX 8.1 text ("Fog Formulas") calls start the distance where fog "begins" and end the distance where it "no
  longer increases", which contradict each other when start >= end, and says nothing more. Wine 9.0 and ours compute
  the formula. Every fogged draw in traces 5180, 6080 and 6796 has start < end (table linear, start 0 or about 8,
  end 750 to 3,242). Not changed; a class (F) of the randomized test.
- Table fog on pre-transformed vertices. Microsoft's runtime fogs by 1/RHW, as ours (the 8.1 "Pixel Fog" text:
  "Direct3D takes the reciprocal of the RHW element ... to reproduce true w"); Wine 9.0 by the vertex z (glsl_shader.c,
  for transformed vertices: ffp_varying_fogcoord = ec_pos.z). The game draws such vertices with table fog: 255 draws
  of trace 6796 (FVF 0x144, 0x44, 0x104; from 0x1000CDE2, 0x1000CBF3, 0x10009915, 0x100097A0), 6 of 5180. A new scene,
  fog_table_rhw, draws it: the reference and ours are equal (0 pixels), Wine differs (WINE_DIFFERS). A class (W) of
  the randomized test against Wine, checked by drawing the case with fog off. (In the vertex-shader trace column, odd
  values are shader handles, not FVF codes: D3D8 handles have bit 0 set, FVF codes never do.)
- r0179, with its class F changed: 57 pixels, all in column 22 but a few. A probe (the gradient texture selected
  straight through) showed the reference sampling texel 1 there and ours texel 2. From the case's geometry (pre-
  transformed, w 2.1 to 2.7 between the grid vertices at u 0.2 and 0.4, screen fraction 0.5909), the perspective-
  correct coordinate there is s = 0.12533, 16s = 2.0053: 0.005 texel past the boundary, a tie (ours takes the side
  the exact value is on). Two hypotheses taken on the way were wrong and are not used: that the column was the
  magnification/minification boundary (equal MAG and MIN filters left all 57 pixels), and that it depended on fog
  (it is there with fog off; fog with table mode NONE only covered it up).
- With the MS reference in the suite, a case that still differs from Wine after its classes' changes is drawn by the
  reference too: if that matches ours by the reference criterion, it is listed as Wine's alone. Seeds 1 to 3 now pass
  (0 cases outside the classes).

Not yet analysed: drawn by Microsoft's reference, 45 of the 600 cases of seeds 1 to 3 differ from ours with none of
the classes (21 of them by 60 pixels or fewer). Known causes that can be among them, not yet assigned case by case:
ZBIAS (the reference ignores it) and an alpha op DISABLE on a colour-enabled stage (r0052 is that, bisected). The
randomized test compares with Wine only; it does not yet compare with the reference.

ALPHAOP DISABLE on a colour-enabled stage: "causes undefined behavior" (D3DTEXTUREOP, 8.1). The reference device
makes alpha 1; ours and Wine pass the previous stage's alpha on. The game does it from 0x1000CBF3 (stage 1: colour
MODULATE4X of CURRENT and TFACTOR, alpha DISABLE, a texture bound; alpha test ALWAYS; blending SRCALPHA onto ONE or
INVSRCALPHA) in 8,715 of 34,011 draws of trace 6796, 1,132 of 5180 and 85 of 6080, so the alpha shows. Scenes
alphaop_disabled_stage0/1 draw it in that state; the reference test lists them as undefined, not failed.

dgVoodoo 2.87.5 (its D3D8.dll is file version 4.8.2.134, the owner's) was downloaded from its GitHub releases to run
here. It does not run under Wine: it is packed, and the unpacked code faults at RVA 0x5E683 on Wine's D3D11 and on DXVK
2.7.1 with lavapipe alike. So kit/d3d8on11/owner_run (run_t8.bat) runs t8 on the owner's machine through the game's
dgVoodoo d3d8.dll, Windows' own and ours (direct and remote), each in its own folder, and owner_compare.py compares
them. `t8 where` prints the d3d8.dll loaded, its version and the adapter. Tried here with a stand-in folder (the
reference in dgVoodoo's place, Wine in Windows'): the script reads all of it.

The 45 cases, analysed (14 Oct). Each finding was reduced to a minimal case and measured against the reference:
- a stage reading a texture it was not given: the DirectX 8.1 "Texture Blending" text defines it, Wine did otherwise;
  ours now follows the text (d8_ff.cpp). The game does it from 0x1000CBF3 (23 of 28 such draws in trace 6796). Class N.
- the alpha test: the reference compares floor(alpha x 255) with ALPHAREF; ours now does the same (d8_exec.cpp). Wine
  compares the float (wined3d_private.h:3117, glsl_shader.c:7529), so the Wine suite counts it as class T.
- a stage read with no texture set reads (0,0,0,1) on the reference, as on ours.
- not taken, reference artefacts (README, "Measured on Microsoft's reference device and not taken"): the lit vertex
  colour truncated to 8 bits (s2r0073, 934 px), and point sampling near a texel boundary. The test switch
  D8ON11_TEST_TEXEDGE flips the texel within eps texels of a boundary: at 0.05 it explains 856 of 1920 unmatched
  pixels over the 67 of 142 changed cases that still differ; with s2r0073, 130 pixels remain, at most 29 a case, not
  analysed.
Wine's d3dcompiler does not evaluate a float2 ?: per component (it took the first component's choice for both):
found writing the switch; the generated shaders' ?: conditions are all scalar (checked).
The randomized test's classes now come from random_cases.py (classes(), changes()) in both test_d3d8on11.py and
owner_compare.py, which also lists D, N and P.

## 14 Oct: the owner's t8run (20261014_193029), read (task 39)

The run: a Parallels VM; Windows' d3d8.dll 6.2.26100.9278 on the Parallels Display Adapter (prl_umdd.dll);
dgVoodoo 4.8.2.134; ours direct and remote. Its case files equal `random_cases.py 1/2/3 200`. Its ours d3d8.dll is
byte-identical to a fresh build of e42c097 here, so the build reproduces.

Windows against Microsoft's reference (the reference rerun here on the same scenes):
- the alpha test: Windows draws stages_blend_alphatest as the reference and today's ours do (the floor rule);
  dgVoodoo differs by 165 px there, as Wine's float compare does.
- decl_short2/4: Windows and the reference both draw one flat texel (ours differs by 682 px). Already undefined (the
  debug runtime rejects SHORT texcoords); the game declares FLOAT only.
- fog_vertex_linear: Windows differs from the reference by 624 px; the reference and ours agree (0).
- fog on with FOGTABLEMODE and FOGVERTEXMODE NONE and an FVF without specular (r0095 s3, r0041 s3, r0057 s2): the
  reference draws fully fogged, as ours does; Windows/Parallels draws the colours unfogged. The game's fogged draws
  all use table mode LINEAR (scope: rs 35 {3: 4240}, rs 140 {0: 4240}), so the case does not arise in the game.

Ours on the owner's driver against ours here (same DLL): 24 random cases differ for seed 1, 13 for seed 2, 20 for
seed 3; scenes stages_blend_alphatest 165 px and texture_transform_2d 50 px. Ruled out here:
- the shader compiler: Microsoft's D3DCompiler_43 (NuGet Microsoft.DXSDK.D3DX 9.29.952.8) in place of Wine's gives
  identical pictures;
- the D3D11 implementation: DXVK 2.7.1 on lavapipe gives pictures identical to Wine's d3d11 (0 differences, all seeds
  and scenes).
What the pixels show: in stages_blend_alphatest the differing pixels (365, in all four quadrants) take another
texel's value (e.g. (5,2): 20e93c there, 20093c here and on Windows; the green comes from a different texture row),
not a different blend. The cause is not known. Unverified hypotheses: a point-sampling tie at an exact texel boundary
broken differently by that GPU; a Parallels D3D11 driver fault; behaviour of ours that D3D11 leaves undefined. The
next measurement needs the owner's machine: the same run under WARP and with the D3D11 debug layer.

Not started, on purpose: "losslessly improve Square's code and methods" (the owner, with Luna's "3D Game
Programming with DirectX 11"). Deferred until ours is shown correct on the owner's driver: a rewrite measured
against an implementation whose pictures are not yet explained cannot be shown lossless. ROADMAP lists it.

## 15 Oct: the owner's second t8run (20261014_195926): ours on WARP

The run of t8_compare_d37f032: the same as the first plus ours on WARP (`ours_warp`) and with the D3D11 debug layer
(`ours_debug`). The debug pass made no device: D3D11CreateDevice answered 0x887A002D (DXGI_ERROR_SDK_COMPONENT_MISSING:
Windows' Graphics Tools are not installed), as run_t8.bat warned. The same DLL was run here on the same case files.

Two groups of differences, by which implementations agree:
- the owner's GPU against WARP on the owner's machine, which agrees with Wine here: scenes stages_blend_alphatest
  (165 px unmatched) and texture_transform_2d (50); 16, 7 and 18 random cases (seeds 1 to 3).
- Windows' D3D11 (the GPU and WARP agree) against Wine and DXVK here: 19, 11 and 16 random cases.
Every pixel that differs in the two scenes, between any two of GPU, WARP and here (414, 305, 365 px in
stages_blend_alphatest; 573, 588, 257 in texture_transform_2d), samples a 16-texel texture with point filtering
exactly on a texel edge: D3D8 samples at integer pixel positions, and with the scenes' vertices the texel coordinate
there is an exact integer (computed in rationals from t8.c). 832 of the 3136 drawn pixels of each scene do so; the
others are the same everywhere. Where the edge is v = 0 with WRAP, one choice is the texture's last row, hence
unmatched pixels rather than shifted ones. Every random case of either group is in class C or N except seed 1 r0152
(class ST, 1 px). Class C reads a texture coordinate set the vertex lacks, which ours makes (0,0,0,0) (d8_exec.cpp
gen_vs): an exact texel edge at every pixel. So: on the same input two of Microsoft's own D3D11 implementations (the
Parallels driver and WARP) pick different texels at an exact edge with point sampling, and Wine and DXVK pick a third
way in some cases. Ours passes the same coordinates to all of them; what differs is the choice at the tie.

Not known yet: how many of the game's draws point-sample exactly on texel edges. The game sets point filtering
(MAGFILTER/MINFILTER 1) on stage 0 in 1,383 and 1,313 of 3,956 and 3,924 filter sets (data/d3d8_scope.json, traces
5180 and 6080); which draws, and at what coordinates, needs a frame capture (task 29's capture) to count. If the game
does, the pictures can differ by driver, and ours would need a rule for the tie taken from the reference.

dgVoodoo draws only the first 13 scenes in both of the owner's runs (no picture from declaration_fan on), and fails
basic's tweening check; not examined.

## 15 Oct: a counter for point samples at texel boundaries (task 42)

The rule, from the D3D11.3 functional spec (downloaded, not in the repo): point sampling scales the coordinate by the
texture size, converts it to "at least 16.8 Fixed Point" and takes the integer part as the texel (7.18.7); float to
fixed conversion may be off by 0.6 ULP (3.2.4.1); 8 bits of subtexel precision are required (7.18.16.1). So within
about 1/256 texel of a boundary either texel is D3D11 behaviour, which is what the owner's run showed.

- **D8ON11_TEST_TIES=eps** (kit/d3d8on11, a test only): magenta where a point-filtered 2D stage samples within eps
  texels of a boundary. Its filter bits ride in PSKey's spare bytes, set only with the switch, so normal runs make
  the same shaders as before (test_d3d8on11.py: ALL OK).
- **kit/fcap/ties.py** replays a capture with and without it and counts the pixels the marker reaches.
- **On t8 here:** texture_transform_2d 832 px marked; every pixel where Windows, ours on the owner's GPU, ours on
  WARP and the reference device differ from ours here is among them (546, 257, 588, 301 px). The same holds for
  texgen_cameraspace_transform (70, 69, 69, 70) and, except against the reference (53 of 129), v15_declaration. rhw_texture: all 1024 point-sampled pixels marked (drawn 1:1, so
  every coordinate is an exact integer), none differed anywhere. A marked pixel may differ; it need not.
- **On a capture:** fcap_host with its filters set to POINT: 886 of 12288 px; the 1:1 quad entirely. The test's own
  capture (linear filtering) marks none.

Not known: the game's count. It needs a frame capture from the game (`fcap=1`, docs/KIT.md); none exists yet.


## 15 Oct: ours ended the game at startup in `thread` mode (owner, kit 35fa886)

The owner's run with the thread folder's settings ended at startup: Windows logged exception 0x40000015 in Windower's
Hook.dll (offset 0x000c73aa), about a second after xikit hooked the device. d3d8on11.log was not in the upload. The
cause, found by replaying the same build's dgVoodoo capture (xikit_fcap_3956_0) through ours here:

- In `thread` mode the replay ended with exit code 3 and the log line "a record of 33554464 bytes does not fit the
  ring (33554432): the device stops" (d8_link.cpp, `ExitProcess(3)`). 33554464 = 2048 x 4096 x 4 + 32: an
  XUpTex header and one level of a 2048x4096 A8R8G8B8 texture.
- The game creates that texture at startup (0x102D3013, 800x1000 hex, levels 1 and 13, format 0x15, pool 1). It is
  the last call in the crashed run's resource log, and in the dgVoodoo run it is followed by more frames.
- TexCore::unlock sent the whole level as one record; the ring takes at most half its size.

Not established: that this exit is what Windows reported. ExitProcess(3) is not 0x40000015, and how Windower's
Hook.dll turns an exit into that exception is not known. The owner's d3d8on11.log would show the line if it was.

Fix: d8_link.h `upload_tex` sends a texture upload larger than a record may carry in bands of whole rows (each
with its box; a block format's band is whole block rows); `upload_buf` cuts a buffer's in byte ranges. Each
link says what it takes (`max_payload`: the ring half its size less the header; direct, no limit). Checked:

- the capture replays in `thread`, `remote` and `thread` with a 64 KB ring (every large upload in bands), each
  identical to `direct` pixel for pixel (0 of 1434888 differ);
- t8 has a new scene `big_texture` (that texture, a colour per row, point sampled half a texel from any edge);
  test_d3d8on11.py runs it in every mode and through the 64 KB ring.

Also from that capture (task 42, the game's first count): ties.py marks 8588 of 1434888 px (0.60%; the other two
captures 8582 and 8600), spread over the room's walls and floor. Against dgVoodoo's own picture of that frame, ours
differs in 8689 px by more than 8 in a channel. Only 34 of those are marked. 7755 are at y >= 730, in the chat
log and the UI text. So the boundary ties are not the main difference from dgVoodoo; what is has not been found.

Separately: xikit attaches to the device by polling for it every 200 ms (xikit.c), so its resource log misses the
creates made before that. In the crashed run the first 216 were missing, Windower's two textures among them.

## 15 Oct: what each state block type captures, from Microsoft's runtime

The owner's log (kit 35fa886, thread mode, under Windower) had "not implemented: CreateStateBlock of the pixel or
vertex state only". The game's own call (0x1017B5FE) asks for D3DSBT_ALL; which module asked for the other types is
not established (the next kit names the caller in such lines).

The DirectX 8.1 documentation does not settle what the pixel and vertex blocks hold: its D3DSTATEBLOCKTYPE page and
its "Creating Predefined State Blocks" page list different states. BLENDOP, COLORARG0, ALPHAARG0 and RESULTARG are on
the first only. MULTISAMPLEANTIALIAS and MULTISAMPLEMASK are vertex states on the first and pixel states on the
second. CULLMODE, FOGCOLOR, FOGENABLE and SPECULARENABLE are vertex states on the second only; the second also has
transforms and clip planes in the vertex block.

So it was measured on the runtime itself: Microsoft's retail DirectX 8.1 d3d8.dll with its reference device (the
msref stand-in, software vertex processing), new mode `t8 sbtypes`. Each state is set to A, a block of the type is
created, the state is set to B, the block is applied and the state read back: A means captured. 1326 rows, none
unmeasured. The same render state and texture stage state numbers are lists in that d3d8.dll's data (file offset
0x11045C on: the vertex texture stage states 11 and 24, then the vertex render states, then the pixel texture stage
states, the pixel render states and the full list).

- ALL: every render state d3d8types.h defines except D3DRS_ZVISIBLE (30) and D3DRS_DEBUGMONITORTOKEN (165); every
  texture stage state 1..28 but 12 (not defined); textures, transforms, the clip plane, the light and its enable, the
  material, the viewport, both shaders and their constants, the stream, the palette, and the index buffer. The
  documentation's list has no index buffer; ours had followed it, and now follows the runtime.
- PIXELSTATE: the 38 render states in d8_device.cpp SB_RS_PIXEL (the documentation's pixel lists, with BLENDOP and
  without FOG*, MULTISAMPLE*), every texture stage state, the pixel shader and its constants.
- VERTEXSTATE: the 40 render states in SB_RS_VERTEX (with CULLMODE, FOGENABLE, FOGCOLOR, SPECULARENABLE,
  MULTISAMPLE*, POSITIONORDER, NORMALORDER), TEXCOORDINDEX and TEXTURETRANSFORMFLAGS, the light and its enable, the
  vertex shader and its constants. No transform and no clip plane, though the guide page lists them.

Not established: whether the runtime takes the same lists on a HAL device or a pure device (only the reference device,
not pure, was measured; the lists are the runtime's data, not the driver's). Only light 0, transforms 2, 3, 16, 23,
256, 257, clip plane 0, texture stage 0's texture and stream 0 were tested of their kinds.

Ours now creates all three types from these lists. `t8 sbtypes` through ours, direct mode, against the runtime: every
row that both measure agrees; 9 rows are not measured on ours (its pixel shaders and shader constants are not
implemented; the game uses neither). test_d3d8on11.py stage 4 runs this comparison.

## 17 Oct: toward 60 fps: the game's limiter paced by the counter, and the executor filter measured offline

**The limiter, from the game's code.** Read by disassembly of FFXiMain.dll, with the imports named by pefile:
- The frame limiter is 0x10011A00, called from 0x10001B74. Its loop calls `Sleep(1)` (import 0x103290F8) while
  60 / (scale x rate) is below the tick count `[limiter+0x30]`.
- The rate is the timer's vtable 0x1032A118 slot 9, 0x10019550. It computes 1000 / (timeGetTime (import 0x10329400)
  - `[t+0x1C]` - `[t+0xC]`), counting 0 ms as 1, and stores the result at `[t+4]`.
- The lap is slot 8, 0x100194F0. The scale is `[t+8]` (0x10019230).
- The limiter's second loop runs only on a NaN.
- With whole milliseconds, a tick count of 1 and a scale of 1.0, no frame is shorter than 17 ms: 58.8 fps. The tick
  count and scale in the owner's game are not established.

**speedfix `precise_pacing`** (off by default):
- The rate entry, for the limiter's three calls (by return address), uses QueryPerformanceCounter from the last
  frame's start.
- The wait at 0x10012B05 sleeps on a high-resolution waitable timer and then spins to the moment.
- Other callers of the rate, and frames not through the hooks, get the game's code.
- Which thread runs the limiter, and where it sits relative to Present, are not established. The log names the
  threads, and the tick count and scale, as the last wait read them.
- Tested in sf_host (Wine) on the game's own limiter code (0x10012B05 to the lap call):
  - On: median 16.674 ms, ticks adding up to the counter's time.
  - Off: every frame whole ms and 17 or more.
- It does not go above 60. The step is `max(step,1)` ticks, so the game's time would run fast. docs/ROADMAP.md P4
  says what going above 60 needs. Nothing here explains the hundreds of fps reported under Vulkan on Linux.

**The executor filter (d3d8on11 `exec_filter`), offline.**
- Setup: the owner's three 16 Oct frame captures (`2026-10-16_201622_3056`), each replayed 20 times with a
  Present after each.
- Configurations: through Wine's d3d8 and through ours (`direct` and `thread`, filter off and on), two rounds in
  reverse order.
- Machine: this container, Wine with Mesa llvmpipe. These are not figures for the owner's machine. Part of capture
  0's runs overlapped the speedfix tests.
- The executor's stats line is now also written at shutdown, so short runs get one.
- Results, executor draw time a frame (mean of the six runs per configuration):

| Mode | filter=0 | filter=1 |
|---|---|---|
| direct | 24.9 ms | 17.1 ms |
| thread | 31.6 ms | 20.9 ms |

- Filter on was lower in all 12 paired runs.
- Present dominates the executor's time here (286 to 354 ms of llvmpipe rasterising), so whole-frame times
  (ours 302 to 380 ms, Wine's 243 to 308 ms) say nothing about the filter. They also say nothing about a GPU.

**Not known yet:**
- Whether either measure moves the owner's frame rate.
- `kit/sessions/perf60` measures both in the game (run 1 on dgVoodoo, `precise_pacing`'s A/B; run 2 on ours,
  `exec_ab`, then `precise_pacing`'s A/B), with render-thread samples throughout.

## 17 Oct: the owner's perf60 runs (16 Oct, received 17 Oct)

Two runs at the Port Jeuno plaza, standing. Both used kit 35fa886 with the perf60 speedfix, on build 6AC37846.
- Run 1, process 2420: dgVoodoo 4.8.2.134.
- Run 2, process 3128: ours, in `thread` mode.

The machine is a native ARM64 CPU with the game emulated as x86, under the Parallels display driver (prl_umdd10).
Two things differed between the runs:
- The cpu tier: run 1 had the SSE tier. In run 2 the game had already chosen x87 before speedfix loaded.
- The crowd: it was not controlled.

So the runs are not compared with each other. Each A/B compares phases within one run.

**The divisor (tick count) and scale:** `[limiter+0x30]` was 1 and `[timer+8]` 1.0 in every minute of both runs.
The limiter and the frame callback ran on the same thread (4056; 1716).

**`precise_pacing`: no measurable effect, and why.**
- Run 1: A 34.09 ms, B 34.36 ms (+0.8%). Phases of one label differ by up to 3.52 ms.
- Run 2: A 35.00 ms, B 35.88 ms (+2.5%). Phases of one label differ by up to 3.49 ms.
- The cause, from the log: the frames never reached the limiter's wait. After the first minute (the login), the
  limiter waited 0 ms in every minute of run 1. In run 2 it waited 0 to 1.3 s a minute, 140 to 226 waits among
  about 1,850 frames.
- Frames were 31 to 36 ms, so the 16.7 ms limit never applied. The lever stays off by default and waits until
  frames are shorter than 16.7 ms.

**`exec_filter` (ours): faster in every phase.** 16 phases of 30 s, 8 each way, the first 60 frames of each left
out (d3d8on11.log, `executor ab:`):

| | filter off (8 phases) | filter on (8 phases) | on − off |
|---|---|---|---|
| The game's frame, mean of phases | 34.54 ms (32.93 to 36.92) | 29.07 ms (26.78 to 31.02) | −5.46 ms (−15.8%) |
| Executor busy a frame | 33.52 ms (31.98 to 35.60) | 26.83 ms (24.87 to 28.50) | −6.69 ms (−20.0%) |
| of which draws | 15.78 | 9.54 | −6.24 |
| uploads | 3.68 | 3.70 | |
| Present | 0.12 | 0.12 | |
| other | 13.90 | 13.40 | |

- The slowest phase with the filter on was faster than the fastest phase with it off, for both measures.
- The difference exceeds the spread within either label (4.24 ms and 3.63 ms at most), so it meets the rule set
  before the run (kit/sessions/perf60/README.md). `exec_filter` now defaults to 1 (d8_device.cpp). The tests run the
  unfiltered baseline with `D8ON11_EXEC_FILTER=0`, and check that the filter is on when nothing sets it.
- Switching the filter cannot leave stale state: an X_OPTION makes the executor forget what it set
  (d8_exec.cpp:639).

**Where ours spends its frame** (ring and executor stats, 600-frame lines):
- The executor is busy 92 to 97% of the time. It is the bottleneck on this machine.
- The game thread waits 15 to 24 ms a frame "for answers": the synchronous calls (X_READBACK, X_FINISH,
  X_INIT, X_RESIZE). It waits 0 ms for room and 0 ms for pacing.
- The render-thread samples agree. 66.5% of them are under the occlusion readback 0x1006C8B0, at its LockRect
  (return address 0x1006C91F, IDirect3DSurface8::LockRect, vtable +0x24).
- The executor's "other" (13 to 15 ms a frame) is every record that is not a draw, an upload or Present. That
  includes X_READBACK, which waits for the GPU. How much of it is the readback is not established. The stats now
  give readbacks their own figure (d8_link.cpp, `readbacks`).

**Where dgVoodoo's frame goes** (run 1, samples of the last 600 frames, 10,463 at 1 ms; the time spent in other
modules, by the game function that called there):
- the occlusion readback's LockRect (0x1006C91F): 25.0%;
- SkinAndDrawV2's call (0x1002A034, the forked copy in speedfix): 18.6%;
- EndScene: 16.6%. This is Frame_Render's call to 0x10009740 at return 0x10012AFA, which calls device vtable
  +0x8C, slot 35, IDirect3DDevice8::EndScene;
- Gfx_SetStreamSourceCached (0x1002F150): 3.5%;
- Cell_DrawQuadBlended: 2.9%.

As in the 13 Oct run, 74 to 77% of samples report EIP 0x00010002 (task 34, not established).

**The exploding vertices on new actors:**
- The owner reports them "after a bit, not at the beginning". The logs do not record when, or in which run.
- The A/Bs switched only:
  - `precise_pacing`: the limiter's wait and rate entry. These do not touch vertices.
  - `exec_filter`, in run 2 only. Switching it resets its shadow.
- `vs_dedupe` (read again): it shares only fixed-function shaders (no function) whose declaration tokens and usage
  are identical.
- Not ruled out, as on 13 Oct:
  - sse_skinning, which was switched on live in both runs ("the game's SSE flag is 0");
  - cpu_tier: SSE in run 1, x87 in run 2;
  - skin_merge and the other levers;
  - the machine.
- Which run showed them decides one of these. If run 2 did, the SSE math tier is ruled out, because run 2 ran x87.

## 17 Oct: re-evaluation from scratch (docs/REEVALUATION.md)

The owner asked for everything to be re-read with nothing assumed. Corrections that came out of it:
- "Frames are flat at 30–35 ms whatever the levers" (said in conversation) is wrong. The plaza ran at about 70 ms
  without speedfix. The levers took it to about 34 ms (session 1); `sse_skinning` and `batch_up_draws`+`lazy_state`
  were measured singly at −14.5 and −8.2 ms.
- Session 5's `skip_all_occlusion_readbacks` A/B did not test the readback: its own count says none were requested
  after the first minute. "Removing the readback moves the wait elsewhere" (said in conversation) has no source and
  is withdrawn. On 16 Oct the readback was under 25% (dgVoodoo) and 66.5% (ours) of render-thread samples.
- No scene in any run has been measured at or below 16.7 ms. The lightest, 819 draws with every lever on, ran at
  17.5–19.4 ms (p50).
Never measured, and deciding where 60 can come from: the GPU's time a frame, the game's own CPU time without
rendering, and the frame with the readbacks really removed. Tests T1–T3 in that document measure them.

## 17 Oct: null_draws and the ceiling session (T1–T3)

- **`null_draws`, a new speedfix lever, for T3.** It is a test, not a speedup.
  - DrawPrimitive, DrawIndexedPrimitive and their UP forms return D3D_OK without reaching the device. Their
    signatures come from d3d8.h.
  - Every other call still reaches the device. Changes held by lazy_state go out at the draw, as before.
  - It switches live both ways, so `ab_levers` can switch it.
  - The four draw methods are now always hooked. Even the minimal start hooks 20 methods (was 16).
  - Tested offline in sf_host's lazy mode: no draw reaches the device, the held state does, the draws come back
    when it is off, and the statistics line counts the draws not sent.
- **kit/sessions/ceiling:** two starts on dgVoodoo in the plaza.
  - Start 1, usual resolution: T2, then T3 (the owner swaps the ini mid-run).
  - Start 2, low resolution: T2's ini again. Its A phases against start 1's give T1.
- **Known limit of T3:** the game's own frame limiter (16.67 ms with precise_pacing) is a floor. T3 can show
  whether the frame without draws reaches it, not how far below it the game would go.

## 17 Oct: the run ledger, and what it corrects

`pipeline/run_ledger.py DIR ...` reads every `xikit.log`, timing, spike and A/B CSV and `d3d8on11.log` under the
given folders. It writes each run's settings, minutes, phases, A/B summaries and readback counts, each cited by file
and line. It was run over every log folder the owner has sent. docs/REEVALUATION.md sections 3 and 4 are now built
from it. Four runs there have no log here and are marked as HISTORY only: 3736, 6156, 12164, and 10016's d3dprof
figures.

Corrections against this file:
- **Readbacks in session 5** (above, "158 in the first minute… none later while standing still") is wrong.
  - Part 1 (2264) had 5,062, 2,005 and 7,165 in minutes 16 to 18 (2264/xikit.log:301,314,327). Those were the A
    phases of the skip_actor_occlusion A/B, when actor queries ran.
  - Part 2 (11952) had 83 to 2,602 in five later minutes with every actor query skipped
    (11952/xikit.log:192–236).
  - What makes the game request readbacks is not known.
  - The readback A/B itself still did not test the readback: its phases fell in minutes 7 to 9, which have none.
- **The house runs of 16 Oct (logs_up1)** were in no entry.
  - Their modes come from the owner's message: direct, dgVoodoo, remote, thread, DXVK.
  - In the house, without speedfix, dgVoodoo (7608), ours `remote` (5584) and ours `thread` (12120) held p50
    17.1–17.6 ms. That is the game's limiter floor of 17 ms (above, "The limiter, from the game's code").
  - Ours `direct` (5560) ran at 33–35 ms there.
  - DXVK (2340) crashed after 5 log lines.
  - So "no scene at or below 16.7 ms" holds only because the floor is 17 ms.
  - The `thread` attribution of logs_up1's d3d8on11.log is confirmed by its line 2, `executor: thread`.
- **exec_filter's −5.46 ms** is a mean of the 16 phase means computed from d3d8on11.log; the device prints no
  summary. The "4.24 ms and 3.63 ms" spreads are the largest within a label for the frame and for executor busy.
- **Session 5's phases** ran 27.65–46.40 ms (2264/xikit.log:292,278). The 29.5–45.7 ms said before were the A/B
  label means.

## 18 Oct: d3d8on11 readback snapshots (X_SNAP), and CopyRects from the back buffer

The owner asked whether anything helps the frames lost to movement and actor loading. Their choice was to answer the
occlusion readback a frame late (task 33), done in two parts. This entry is the first: the device side, in ours.

**Why ours needed it.** From our code (d8_device.cpp CopyRects, d8_res.cpp LockRect):
- a lock of a lockable render target staged and mapped at lock time;
- a copy from the GPU into an image surface was a synchronous X_READBACK.

Either way the game waited for the executor and the GPU. So a lock made a frame later would have waited all the same.
The reverted occlusion_async (mode 1, image surfaces) could not have saved anything in ours.

**What was built.** README, "Readback snapshots":
- After each CopyRects into a small lockable render target that is never a render target itself, the executor copies
  the target to a staging texture.
- At each Present it maps the finished copies without waiting (D3D11_MAP_FLAG_DO_NOT_WAIT) into a shared slot table.
- LockRect answers from the slot only when the surface's write count matches the slot's. It is exact, not a heuristic:
  the only D3D8 methods that write such a surface are CopyRects, being the bound target, and an unlock after a writing
  lock. The first is counted, the second is excluded (`drawn`), and the third is counted.
- RING_VERSION 4.

**A bug the new test found.** In ours, CopyRects from the back buffer into an X8R8G8B8 surface copied nothing.
- The back buffer is B8G8R8A8; an X8R8G8B8 surface is B8G8R8X8.
- D3D11's CopySubresourceRegion copies only within a type group. D3D8 counts the two as one format.
- Wine's d3d8 passed the same check.
- Such a copy is now drawn through two scratch textures (d8_exec.cpp `copy_draw`).
- The game's occlusion copy does not take this path. Its source is level 0 of the box texture `[[0x1047C004]+0xC]`
  (GetSurfaceLevel, vtable +0x3C, at 0x1006C8CD), not the back buffer; `[[0x1047BFA8]+0xEBC]` is the device
  (CopyRects, vtable +0x70, at 0x1006C8FD). (Corrected on the same day: this entry first named that pointer as the
  source.) Whether the game copies from its back buffer anywhere else was not checked.

**Tests.** test_d3d8on11.py passes in all three modes:
- `basic`: a copied target reads the copy, then what a lock wrote after it.
- `frames`: 599 frame-late locks, all reading the frame before's colour. From the snapshot: 599 in `direct`, 569 in
  `thread` and `remote`. The 30 that read back are frames 1–30, before anything makes the game wait for the executor.

**Not yet known.**
- Whether the game's lock waits on dgVoodoo when made a frame later. That needs the per-call timing of the speedfix
  lever, in the owner's run.
- The lever itself (speedfix `occlusion_late`) is the second part.

## 18 Oct: speedfix occlusion_late (the occlusion readback answered a frame late)

**From the game's code.** 0x1006C8B0 (`__thiscall(q, flag)`, `ret 4`; its only caller 0x1006C4D7 passes 0):
- GetSurfaceLevel(0) of the box texture `[[0x1047C004]+0xC]` (0x1006C8CD);
- CopyRects of it into `[q+4]` on the device `[[0x1047BFA8]+0xEBC]` (0x1006C8FD);
- LockRect READONLY of `[q+4]` at once (0x1006C91C), and a count of `(dword & 0x80) >> 5` over its 16x16 pixels.
The lock straight after the copy is what makes the CPU wait for the GPU.

**What was built.** speedfix `occlusion_late` (README, "occlusion_late"), off by default:
- 1: each query object gets a 16x16 lockable render target of its own, in the format of the game's `[q+4]`. Each probe
  copies the box picture into it and answers with the count of the picture copied at the object's previous probe,
  counted exactly as the game counts. A first probe, or a previous picture older than 32 frames, gets the game's own
  readback. 32 is a chosen bound, not a fact of the game. Reset releases the surfaces first.
- 2: measure only. Every answer is the game's; the log says how often the late answer would have differed, and how
  often visible/hidden would have flipped.
- Either way, each statistics line gives the time in the game's readback, in the copy, and in the late read
  (mean and longest).

**What it changes in the picture.** An actor that comes into or goes out of view is shown or hidden one probe late.
With occlusion_stagger=8 a probe is up to 8 frames apart. That is the price; mode 2 measures how often it happens.

**Tests (sf_host.c, Wine).** The game's own readback, run on fake surfaces, answers 0, 256 and 128 for an empty, a full
and a half-drawn picture. With the lever on: a first probe is the game's (its LockRect); later probes give the
previous picture's count; the answers are per object; one render target per object; a picture 40 frames old is not
used; Reset releases both surfaces; mode 2 answers as the game and logs the comparison and the times.

**Not known.**
- Whether a lock made a probe later waits on dgVoodoo. If the driver stalls on any lock of a GPU-written surface, the
  late read costs what the game's read cost. The longest late read in the log answers that in the owner's run.
- In ours (d3d8on11), the readback snapshots answer such a lock without waiting (test_d3d8on11.py, 599 of 599 in
  `direct`).
- The time it saves, and whether the one-probe delay shows: `ab_levers=occlusion_late`, and a look while moving
  through crowds.

## 18 Oct: texture loading off the render thread? What the code says

The owner asked whether decoding could move off the render thread, ideally onto the GPU without blocking.

**What runs, from the game's code** (tree/src/unit_100399D0_TexObj.cpp, gate PASS; D3DX names from docs/RESIDUE.md,
matched by bytes in d3dx8tex.obj, the D3DX statically linked into FFXiMain):
- TexObj_Create (0x10039A80) makes the final texture with D3DXCreateTexture (0x102D2FB9), pool 1 (MANAGED). Its format:
  - DXT1 (16-bit source) or DXT3 (24/32-bit) when the texture's class `([+44] >> 4) & 0xF` is 0, or above
    g_cfgTextureCompression (0x10456AD0), or 10 with the map-compression setting 0; the format is first checked with
    D3DXCheckTextureRequirements (0x102D2F40);
  - otherwise A1R5G5B5 (16) or A8R8G8B8 (24, 32).
  - The game's value of the setting was 0 in the test host; speedfix's texture_compression sets 15, so with it only
    classes 0 and (map compression 0) 10 are made DXT.
- TexObj_Upload (0x10039E50), for a texture not stored compressed:
  - makes a temporary texture (D3DXCreateTexture, all levels, MANAGED: A1R5G5B5, R8G8B8 or A8R8G8B8);
  - locks it and decodes the file's pixels into it in the game's own loop: 4- and 8-bit palette lookups, 16/24/32-bit
    repacking, rows flipped;
  - copies level 0 into the final texture with D3DXLoadSurfaceFromSurface (0x102D4515), filter 1 (D3DX_FILTER_NONE);
    into a DXT texture that copy is D3DX's DXT compression, on the CPU;
  - makes each further level from the one before with D3DXLoadSurfaceFromSurface, filter 5 (D3DX_FILTER_BOX); for a
    DXT texture each level is decompressed, filtered and compressed again, on the CPU.
- A texture stored compressed (`[+41]` bit 0x80) is loaded with D3DXLoadSurfaceFromMemory (0x102D2868) and its FOURCC
  from 0x10351C94; its levels are then made as above.
- TexMgr_UploadPump (0x1003ABB0) makes at most 21 a frame (HISTORY, 10 Oct).

**What is measured:** CreateTexture 148 us a call through dgVoodoo; 12,460 of 14,450 creations a session on the render
thread (the owner's two captures). **Not measured:** the time in the decode loop, in D3DX's conversions and DXT
compression, in file reading, and in model setup, during a loading frame. Without that split, no part of it can be
named as the cause of the loading stutter.

**What can and cannot move, and where:**
- Under dgVoodoo: the D3D8 calls must stay on the device's thread (the game's device is used from the render thread;
  whether it was made with D3DCREATE_MULTITHREADED was not checked). Direct3D 8 has no compute shaders, and a DXT
  texture cannot be drawn into, so a GPU decode there would need the texture changed to a DEFAULT-pool render target,
  which Reset loses. Not proposed.
- The CPU work (the decode loop and D3DX) could run on a worker thread only if the game asked for textures ahead of
  needing them. It does not: the pump calls TexObj_Create inline and uses `[+64]` after it.
- In ours (d3d8on11, `thread` mode): CreateTexture and each level's upload are already ring records run by the executor
  thread (d8_res.cpp: X_TEX, `upload_tex`), so the GPU-side creation does not wait. Two exact (lossless) changes are
  possible there:
  - the temporary texture is only read back by D3DX on the CPU and never drawn, yet ours makes it on the GPU and
    uploads it; a managed texture's GPU copy could be made at its first use instead;
  - the palette lookup and repacking are integer table lookups, so a GPU version (a compute shader in the executor)
    gives the same bytes. D3DX's DXT encoder and box filter are not reproduced exactly by a GPU encoder, so moving
    those would change the picture; keeping such textures uncompressed (texture_compression) avoids them instead.

**Next, before any of these:** time TexObj_Upload, its D3DX calls and the pump in loading frames (speedfix, with
spike_ms), so the cost is known per part.

## 18 Oct: the owner's runs 8980 and 8452 (17 Oct, kit 35fa886), skip_all_occlusion_readbacks A/B

Two starts, by the owner's note 1512x949 then 1024x768 (the log does not record the size). Settings in both: every
lever on as before except merge_scenes and null_draws; `ab_levers=skip_all_occlusion_readbacks`, 8 phases of 30 s.
- **8980:** A (readbacks run) 46.51 ms, B (skipped) 36.05 ms: -10.47 ms (-22.5%), medians -11.88, p95 -6.05; phases of
  one label differ by up to 5.80 ms.
- **8452:** A 43.32 ms, B 32.54 ms: -10.78 ms (-24.9%), medians -10.42, p95 -16.48; spread up to 7.21 ms.
- In both the difference is larger than the spread between phases of one label, so the readbacks' cost is measured:
  about 10.5 ms a frame. The A minutes ran 2,734 to 3,223 readbacks in about 541 to 616 frames, about 5 a frame, so
  about 2 ms a readback (a ratio of the two counts, not a per-call timing).
- The smaller size was about 3 ms faster in each label; most of the frame time does not scale with the pixel count.
- Samples (render thread, 1 ms): 64% and 67% at EIP 0x00010002 (task 34, not established). The innermost game return
  address 0x1002A034 (29% and 31%) is the return from the call to SkinAndDrawV2 (0x1002A02F), which skin_merge replaces
  with speedfix's fork, so that time is in the fork and what it calls.
- **What this says about occlusion_late:** it is the reverted occlusion_async's mode 2. Its best case is
  skip_all_occlusion_readbacks' saving less the copies, and only if a lock made a probe later does not wait on
  dgVoodoo, which is not known. Its per-call timing (`readback time:`, the longest late read) answers that.


## 18 Oct: where the 64-67% and the 29-31% go: what sample=1 can and cannot say, and a native sampler

The question: of the render thread's samples in runs 8452 and 8980 (entry above), what is behind EIP 0x00010002
(67.4%, 64.2%) and behind the innermost game return address 0x1002A034 (31.1%, 28.7%)?

**Which speedfix ran (from the samples themselves):** every speedfix offset sampled falls on an instruction boundary
of `ceiling/speedfix.dll` (commit a1cd637, the null_draws ceiling session): 434 of 434 in 8452, 573 of 573 in 8980.
Kit 35fa886's speedfix.dll matches only 111 of 434. So the speedfix in those runs is the ceiling build, not the kit's.

**Run 8452 (10,088 samples) and 8980 (11,763), by sample=1's self and caller (separate counts, no joint data):**
- self 0x00010002: 6,798 (67.4%) and 7,552 (64.2%); 0x00010000: 83 in 8452.
- caller 0x1002A034 (the return from the call to SkinAndDrawV2 at 0x1002A02F in fn 0x1002A000, which skin_merge
  replaces with speedfix's fork): 3,141 (31.1%) and 3,375 (28.7%).
- caller 0x10012AFA (Frame_Render, the return from `call 0x10009740`, CDx::EndScene): 1,650 (16.4%) and 1,505 (12.8%);
  Frame_Render in all 17.65% and 16.36%.
- fn 0x1002F130 (caller 0x1002F150): 668 and 694. CMoVuElem_Draw (0x10044002): 230 and 772. Cell_DrawQuadBlended
  2.98% and 4.18%; fn 0x100303B0 2.77%; CDx::SetLight 2.21% (8452).
- self in speedfix.dll: 434 (4.3%) and 573 (4.9%). By function (8452/8980): ApplySB 53/74, SetTransform 46/66, the fork
  41/65, xm_flush 13/36, the rest smaller. The fork and xm_flush themselves are about 0.5-1% of the samples, so the
  29-31% under 0x1002A034 is in what they call (the device: dgVoodoo and below), not in their own code.
- self USER32 390 and 532; win32u 169; Hook.dll (Windower) 125 (8452).

**What is known and from where:**
- 0x1002A034 and 0x10012AFA are call sites in the game's code (the analysed build's disassembly, with the kit's
  rel/shift table).
- swap=0 in 8452 (0 of 774 reconstructed functions swapped; the 2 swapped are mod targets), so the game's own
  CDx::EndScene ran. Its own return address inside CDx::EndScene, 0x1000974C, is never found on the stack in the
  samples whose caller is 0x10012AFA. Why is not established.

**What is not known:**
- What 0x00010002 is. It is in no image. It equals CONTEXT_i386|CONTEXT_INTEGER, which is an observation, not an
  explanation. Under x86 emulation on ARM64, a thread's x86 context read from inside the process may not be its
  position while native code (the emulator, the system's ARM64 code, the driver) runs. That is a possibility only.
- How the 29-31% and the EndScene share split between dgVoodoo, D3D11, the driver, the emulator and waiting. sample=1
  runs inside the emulated process and sees only the x86 state, so it cannot answer this.

**The native sampler (kit/xisamp, `sample=2`):** a native 64-bit process (ARM64 on the owner's machine) that xikit
starts at the first Present. Each sample it suspends the render thread and reads, at the same instant:
- the x86 context (Wow64GetThreadContext) and the x86 return addresses on its stack;
- the native context (pc, lr and the ARM64 frame-pointer chain).

It writes histograms per window to `xikit_native_<pid>.csv`. `kit/xisamp/read.py` gives:
- x86 module and native module, each on its own and jointly;
- the no-image eips with their native pc and frames, which is the 0x00010002 question;
- the samples by innermost game call site;
- for `--focus 0x1002A034 0x10012AFA`, the x86 and native positions under those sites.

What has been tested: the x64 build under 64-bit Wine (`test_xisamp.py`, 11 checks; `test_xikit.py`'s native run).
The ARM64 build has not run on ARM64. Its native chain is untested.

**Next:** the owner runs the same A/B with `sample=2` (`sample_ms=1`, `sample_window=30`). The two
`xisamp_*.exe` go next to xikit.dll. The owner sends `xikit_native_<pid>.csv` with xikit.log.


## 18 Oct: the owner's sample=2 runs 3464 and 14180 (17 Oct, kit 736e74a): the time is in three kernel calls

Two runs in the plaza, standing still, `ab_levers=skip_all_occlusion_readbacks`:
- **3464:** the kit as sent: skin_merge=1, occlusion_late=0, 8 phases. The tier in use was SSE.
- **14180:** skin_merge=0, occlusion_late=1, 2 phases (ab_rounds=1, so no measure of the noise). The tier in use was
  x87: cpu_tier's log line says "the game has already chosen its math tier".

**Frame times (speedfix's A/B lines):**
- 3464: A 43.51 ms, B 33.48 ms, B − A −10.03 ms. The phases of one label differ by up to 2.54 ms. 8452 (no
  sampler) gave 43.32/32.54, so the sampler's own cost does not show in the frame time.
- 14180: A 44.78 ms, B 25.78 ms, −19.00 ms. A ran occlusion_late (skip off), B skipped every readback.

**The sampler (xikit_native_<pid>.csv):**
- 132,515 samples in 3464's A/B (23:04:19–23:08:19) and 32,728 in 14180's. No failed reads.
- The rate was about 550 a second, not the 1,000 asked for.

**0x00010002 is not a position.**
- Wow64GetThreadContext, read from the other process at the same instant as the native context, gives eip 0x10002 in
  99.8–99.9% of the samples (and 0x10000 in the rest) in every window of both runs.
- So under xtajit the x86 EIP field does not say where the thread is. The x86 return addresses on its stack (ESP is
  valid) and the native pc do. sample=1's 64–67% at 0x00010002 was the whole thread, not one place.

**Which kernel calls (from the samples, with a check):**
- The innermost x86 return address into win32u.dll is the system-call stub the x86 code called. It is named by
  win32u's export table, read on the owner's machine (the file's `a` records).
- The native pc for each stub sits in one wow64win.dll thunk:
  - NtGdiDdDDILock2 at wow64win+0x3004;
  - NtGdiDdDDIUnlock2 at +0x35D4;
  - NtGdiDdDDIWaitForSynchronizationObjectFromCpu at +0x3634.
- Native module machines (`m` records):
  - wow64win.dll, wow64.dll and xtajit.dll are ARM64 (0xAA64), in System32.
  - d3d11, prl_umdd10, gdi32 and ntdll are loaded from SyChpe32 (CHPE).
  - D3D8.dll is dgVoodoo's x86 build, in PlayOnlineViewer.
- `kit/xisamp/read.py` now prints this view (the `syscalls` tables). Its call-site table counts a sample only when
  its native pc is in wow64win.dll or ntdll.dll, so a stale return address on the stack is not counted.

**Shares of the render thread's samples, native pc inside the call (read.py, by window):**

| window | frame (ms) | WaitForSync…FromCpu | Lock2 | Unlock2 |
|---|---|---|---|---|
| 3464 A, phases 4–5 (23:05:49–23:06:49) | ~43.8 | 26.52% | 22.07% | 18.39% |
| 3464 B, phases 6–7 (23:06:49–23:07:49) | ~33.7 | ~0 | 29.63% | 24.89% |
| 14180 A (occlusion_late) | 44.78 | 27.66% | 20.34% | 16.98% |
| 14180 B (skipped) | 25.78 | ~0 | 26.52% | 23.04% |

**The sync wait is the readback, and its cost per frame now agrees two ways:**
- In 3464 A, the wait's samples are under the game's readback LockRect (return 0x1006C91F): 26.54%.
- 26.5% × 43.8 ms is about 11.6 ms a frame.
- speedfix's own timing (QPC around the call) agrees. The minute 23:04:19–23:05:19 had 3,360 readbacks at a mean of
  2,374 µs, which is 7.98 s, in the A phase's 707 frames (647 counted plus the 60 left out). That is 11.3 ms per A
  frame.
- So the "4.909 ms a frame" in that log line was wrong. It divided by every frame of the minute, including the
  B phase's 918, which ran no readback. That is fixed here: a frame with every readback skipped is now counted apart
  (sf_occlusion.cpp, sf_occl_frame; test: sf_host.c).
- The A − B of about 10 ms is therefore the readback's own wait (about 4.75 calls a frame, about 2.4 ms each), not
  twice it as it seemed.
- These are two independent measures, the sampler's share and speedfix's timing. Their agreement is also the one
  check that the sampler's shares are proportional to time. Suspending a thread at a kernel call and at translated
  code may not take the same time, and the file has no per-sample time to test that otherwise.

**occlusion_late does not remove the wait (14180):**
- Its late read, the LockRect of a copy made a probe earlier, waits in the same kernel call. 27.66% × 44.78 ms is
  about 12.4 ms a frame.
- speedfix's timing agrees: 3,730 late reads at a mean of 2,154.9 µs is 8.04 s in A's 661 frames, 12.2 ms a frame.
  The log line's "4.513 ms a frame (1830 frames)" had the same divisor error.
- Those samples' x86 stacks run win32u <- speedfix.dll <- ... <- 0x1006C4DC, the call speedfix redirects. Their
  innermost game address, 0x1000A258 (fn 0x1000A230, which calls slot 61 of the object at [this+0xC] with (0, 0), SetTexture(0, NULL) if that is the device (MinGW d3d8.h), then
  0x1000A2B0), is under speedfix's frames. It is the readback's caller chain or a stale slot, not the waiting call.
- Through dgVoodoo on this machine, a lock of a render-target copy waits about as long a probe later as at once.
  Whether that wait is for the GPU or for the driver is not established.

**Lock2 + Unlock2: about 18 ms a frame in 3464, A and B alike:**
- A: (22.07 + 18.39)% × 43.8 ≈ 17.7 ms. B: (29.63 + 24.89)% × 33.7 ≈ 18.4 ms.
- Removing the readbacks leaves them unchanged.
- In 14180 B: (26.52 + 23.04)% × 25.78 ≈ 12.8 ms.
- By call site in 3464 B (native pc in the call):
  - 0x1002A034 (skin_merge's fork of SkinAndDrawV2): Lock2 13.14%, Unlock2 8.56%;
  - 0x10012AFA (Frame_Render's call to CDx::EndScene): 5.98% and 6.78%;
  - 0x1002F150: 2.73% and 2.22%;
  - CMoVuElem_Draw: 2.46% and 1.64%;
  - 0x10030831: Unlock2 1.92%;
  - CDx::SetLight: Lock2 1.05%;
  - the rest under 1% each.
- In 14180 (no fork) the game's own SkinAndDrawV2 sites (0x1002E74B, 0x1002DD03, 0x1002DD38, 0x1002DD7F, 0x1002DDB4,
  0x1002E78A) sum to about 14% in B.

**What is known:**
- D3DKMTLock2 and D3DKMTUnlock2 (the D3DKMT interface in the WDK headers) lock and unlock a video-memory
  allocation through the kernel.
- On this machine the D3D11 path under dgVoodoo makes them often enough to take about half of the render thread's
  time. The sampler cannot count calls.

**What is not known:**
- Which Direct3D 8 calls lead to them. Lock/Unlock of the game's dynamic vertex and index buffers is the obvious
  candidate, but that is not established. The x86 frames between the game's call site and win32u are in D3D8.dll
  (dgVoodoo), d3d11.dll and prl_umdd10.dll, and are positions only (no symbols).
- How many there are a frame.
- Why SetLight and EndScene sites show them.
- Whether the fork makes more of them than the game's own SkinAndDrawV2. 3464 (fork, SSE) and 14180 (no fork, x87)
  differ in two levers and in the phase count.

**The exploding vertices:**
- The owner turned skin_merge off for 14180 and saw none.
- 14180 also ran the x87 tier, where 3464 ran SSE (the cpu_tier log line). From this run alone they cannot be put on
  skin_merge rather than the SSE math tier.
- To settle it: a run with cpu_tier as in 3464 (the game choosing SSE) and skin_merge=0. Or, if they show again,
  skin_merge switched alone. skin_merge takes effect at the next start, so that means two starts with only that line
  changed.

**Next:**
- Count the D3D8 calls that lead to Lock2/Unlock2: d3dprof `call_time=1` gives the time and count per call site
  and method.
- Name the fork's inner call sites: read.py takes the game's and the reconstruction's addresses as call sites, not
  speedfix.dll's.

## 18 Oct: a kernel-call counter (d3dprof `kernel_calls=`), to count the D3D8 calls behind Lock2/Unlock2

Run 3464 B put about 20.7 ms of the 33.66 ms frame in kernel calls the sampler named (Lock2 10.2, Unlock2 8.6, waits
~1.1, others ~0.8). The sampler gives positions, not counts, and cannot say which Direct3D 8 call led to them. The
kit can now count them directly:

- **What it does:** with `kernel_calls=1` (or `2`), d3dprof patches win32u.dll's x86 system-call stubs in the game's
  process (exact byte form only; anything else is refused and named in the report) and counts each entry, render
  thread and other threads. Lock2, Unlock2, WaitForSynchronizationObjectFromCpu and Render are charged to the device
  method or the vertex/index buffer Lock/Unlock they happen in, per call site (docs/KIT.md).
- **Known from the tests:** the counting thunk is exact on Wine (3 PeekMessage calls a frame read as 3.00); the
  attribution is exact under Unicorn, nested calls included.
- **Not known yet:** whether the owner's win32u.dll (ARM64 Windows 26200, x86 under xtajit) has these stubs in this
  form. The report's first `# kernel calls:` line says, with the bytes of any it refused.
- **What it will settle:** how many Lock2/Unlock2 a frame there are, and which game call sites produce them. The
  13 Oct trace counted about 475-568 VB Unlocks a frame (the largest at 0x1002DD38 / 0x1002DD7F / 0x1002DDB4 in
  CYyModelDt_SkinAndDrawV2, CPU skinning). Whether those are what makes the Lock2/Unlock2 is what the run measures.

## 18 Oct: run 14204 (dgVoodoo, `kernel_calls=1`): where the Lock2/Unlock2 come from; the stutters

**Measured** (d3dprof_14204.csv, the last 600-frame window: frame 35.1 ms mean, 29.3 min, 376.1 max):
- 2,102.3 `NtGdiDdDDILock2` and as many `Unlock2` a frame, all on the render thread: 1,730.1 inside device calls,
  372.0 inside the game's vertex-buffer Lock/Unlock, 0.2 elsewhere. No `WaitForSynchronizationObjectFromCpu`.
- By method, Lock2 a frame (calls a frame): DrawPrimitive 809.5 (2,377.6), DrawIndexedPrimitive 507.1 (1,164.3),
  VB_Lock 372.0 (372.0), SetTexture 175.9 (1,139.5), DrawPrimitiveUP 100.1 (155.2), SetRenderState 88.5 (2,136.7).
  The largest buffer pair is the skinning VB at 0x1002DD7F / 0x1002DE3F, 84.5 a frame each.
- Not measured: the time of each kernel call. Run 3464 put about 18.8 ms a frame in Lock2+Unlock2 by sampling; with
  about 4,200 calls that is about 4.5 µs each, an inference from two different runs. That a dgVoodoo draw's Lock2
  is a D3D11 Map of a dynamic buffer (its constants or UP data) is also inference: the frames between are in
  dgVoodoo's D3D8.dll, d3d11.dll and prl_umdd10.dll, without symbols.

**What was done about it (ours):** the batched executor (`exec_batch`, kit/d3d8on11/README.md): one Map per arena per
batch for the shader constants and the UP vertex and index data of every draw in it, the draws binding their
constants at an offset. In ours, the 17 Oct thread-mode runs had the executor busy 92-97% of the frame. Whether the
batches cut that, and the Lock2 count, is what the owner's next run measures (`exec_ab_lever=batch`, with
`kernel_calls=1`).

**The stutters** (xikit_spikes_14204.csv, xikit_timing_14204.csv): 1,689 of about 9,600 frames over 50 ms; 74 over
150 ms; the worst 3,078.8 ms (frame 1460, with 1459 and 1461 at 482.8 and 547.7) and 1,496.7 ms (frame 76).
- Resource creation is not what they are: 59 of the 74 frames over 150 ms create nothing; the 15 that do spent
  23.4 ms in all in creation calls. The largest creations are at frame 0 (a 2048x4096 texture, 234 ms; xikit's own).
- What they are is not known. The native sampler aggregates 30 s windows, so it cannot be put on a frame. Most of
  the frames over 50 ms (1,425 of the 1,689 are 50-80 ms) are the slow stretches the timing windows show (p50 of
  16.7 to 69.8 ms by window), not single hitches.
- The speedfix A/B (group 1, skip_all_occlusion_readbacks) is confounded: the phases' means run from 29.2 to 80.8 ms
  with no order by lever, so the owner moved during it. The game's readback took 3.4-4.3 ms a call, 4.0-8.1 ms a
  frame when run.

**Next:** a stutter needs a per-frame record of where the time went (the game's frame split, not the sampler's
windows); the batches need the owner's run.

## 18 Oct: the stutters get their own samples (`xikit_stutters_<pid>.csv`)

Run 14204's 74 frames over 150 ms could not be put on anything: the native sampler wrote 30 s windows only.
- Now each sample also reads the kit's frame counter (`g_frame`) while the render thread is stopped. The kit counts
  a frame in Present right after taking its start time (xikit.c, `kit_present`), so a sample that reads `v` belongs
  to frame `v + 1`, numbered as `xikit_spikes_<pid>.csv` numbers it. No clock is shared between the two processes.
- A frame whose samples span `spike_ms` or more gets its own block (docs/KIT.md, "Stutters"); `read.py` lists each
  by game function, or by module and export with the innermost game call site.
- Tested under Wine x64: kit/xisamp/test (a 400 ms stall in frame 41: one block, 399 ms, every sample in the
  stall) and test_xikit.py `native` (the host's 2.5 s Sleep: one block, 1,399 of its 1,400 samples in the Sleep).
  Not tested: the ARM64 build on ARM64 Windows.
- Cost: the read takes samples. The `native` test's GetColor loop got 175 to 250 samples with it and 278 and 336
  with the build before it (the test's bound, 300, failed once on that build too; it is now 100).

## 18 Oct: past 60, with the game's time tied to real time (speedfix `uncap`, `frame_step`)

The game waits for at least `[g_pCamera+0x30]` ticks of 1/60 s a frame, and hands its code the step as whole ticks,
truncated, at least 1 (docs/RENDERER.md 3, 3a). With the minimum at 0 it does not wait, and every frame is a tick.
- Read from the code for this: the step's computation 0x10012B0B..0x10012CB5 (the ring of 4, its `* 0.25`, the
  `_ftol` call at 0x10012C3C, the floor `[+0x30]` and cap 20), the getters 0x10014CF0 and 0x10014D20, and
  Frame_Render's own copy at 0x10011A24.
- `pipeline/frame_step_users.py` follows the returned value from each of the 218 call sites of the getters: 41 only
  compare, drop or accumulate it, 177 convert it to an integer, store it, pass it on, or reach code the script does
  not model. No site divides by it. A function gets the fraction only if all its sites do: 14 sites in 13 functions.
- speedfix: `uncap` (the minimum to 0, the old value logged and restored), `frame_step=1` (the truncation replaced
  by whole ticks with the fraction carried), `frame_step=2` (and the untruncated value at the 14 sites).
- Tested in sf_host under Wine on both builds, the game's limiter and step code running: at 60 fps the whole ticks
  came to 123 against 123.3 by the counter; at about 300 fps (3 ms frames) speedfix's step added up to 100.0-100.7% of
  real time and the game's own to 500-502%.
- The owner's build needed `dist/xikit_manifest_6AC37846.txt` in the test (`FFXI_DLL_6AC37846=... kit/xikit/build.sh`);
  without it the kit does nothing there and every mode fails.
- Not tested in the running game. Not known: per-frame code that ignores the step (0x1024DE20 is one), what a step of 0
  does at the 204 whole sites, and what the server makes of it.

## 18 Oct: the tree's selection by the frame; frame_step and uncap as edits to verified code

The owner's direction: the tree's selection should follow the frame, not only the device; and a change like
frame_step should be made in the C++ as an edit to verified code, so the gate confirms that everything not meant to
change still matches the game.
- `pipeline/frame_set.py` (`data/frame_set.json`): the functions a frame is known to run, each with its evidence.
  - Static reach from CApp::RunMainLoop (0x10001520) gives at least 4,984 functions (4,765 game code, 219 D3DX),
    270 of them in the tree. It follows direct calls, tail jumps and the proven virtual calls.
  - 1,929 indirect calls in 484 functions have no proof and are not followed, so this is a lower bound.
  - SkinAndDrawV2 is found only through the samples. Static reach finds only 60 of the 113 functions with
    frame_step sites.
- The Ghidra route now goes end to end:
  - `pipeline/ghidra/export.sh` runs the export, and `hx2clean.py` takes comma lists of exports and gates.
    Ghidra records share the translator's preparation (`hx2cpp.ghidra_prepare`), and units with a Ghidra
    function include `xi_ghidra.hpp`.
  - `tree/source.json` names `data/exports/ghidra_export.jsonl` by SHA-1, with the raw gate
    `verification/gate_raw_ghidra.jsonl`. `verify_all.py` regenerates the tree from both exports:
    `tree-regen` differing [], build `failed: none`.
  - Regenerating found the committed tree out of step with its sources. Callees had been renamed in the tree
    only, and `xi_shift`/`xi_code` lived only in `tree/include`. The fix copies them back into
    `clean_support/` (`xi_decomp.hpp`, `rt.c`). `same_code.py --ignore-names`: 778 identical, 0 different.
  - The first two functions added by this route are the step getters 0x10014CF0 (Camera_GetFrameScale) and
    0x10014D20: raw PASS 100/100, clean PASS 100 with 10 of 10 instructions covered.
  - They are fuzz only: the four dumps are not in this container.
  - The tree has 779 functions, 776 PASS.
- speedfix's `uncap` and `frame_step` are now gated forks of three tree functions, and the patches (asm thunks,
  writes to `[+0x30]`) are removed:
  - The forks are `kit/speedfix/fork/Frame_Render.fork.cpp`, `Camera_GetFrameScale.fork.cpp` and
    `fn_10014D20.fork.cpp`.
  - In Frame_Render's fork, uncap reads the floor `[this+0x30]` as 0 at its four reads; the field is not written.
  - With frame_step on, the fork makes the step untruncated (floor and cap of 20 as the game's), carries the
    fraction, and answers the whole part. It does so at its own copy (0x10011A24, passed to 0x1000D3F0) and in
    the getters' forks; frame_step=2 also answers the fraction at the 14 sites.
  - Switches off: each fork PASSes the gate with no declarations (Frame_Render 68 returned, 24 UNRESOLVED,
    8 faults; the getters 100).
  - Switches on: each PASSes with every difference declared. Frame_Render's declared changed call of 0x1000D3F0
    was seen in 50 trials, the getters' declared return in 100.
  - The declarations may now name an import (`"call": "Sleep"`, `declared.bind_imports`): with the floor at 0
    the wait's Sleep is not called.
  - Test: `kit/speedfix/test/test_framestep.py` ALL OK; `test_speedfix.py` ALL OK in all 6 modes.
    - At 60 fps, the step made from the game's ring average after the game's own wait and step code added up
      to 123 whole ticks, against the game's 123.24.
    - With the floor at 0 and 3 ms frames: 39 against 37.92 ticks of real time, 161 frames of 0.
- Not known yet: the gate's fuzzed trials stop UNRESOLVED at the wait's first timer call (0x10012B28). That call
  and 0x10012B86, 0x10012B91, 0x10012BCB and 0x10012BF5 are virtual calls on the timer at `[this+0x1c]` (vtable
  0x1032A118) with no measured facts. So the gate has not run the step code; sf_host has. Needed: a `facts=1`
  session for those sites, and a `capture=Frame_Render` session to give the gate real inputs
  (`test_framestep.py CAPTURES=dir`).
- `kit/BUILD.lock.json` was rewritten. speedfix, the in-game DLL and its manifest changed with their sources.
  probe11 (x86, arm64), d8x_arm64 and xisamp (x64, arm64) had no source change and still differ from the lock.
  Built twice here at the same path they are identical (only the PDB path enters the timestamp and GUID). The
  hashes in the lock came from earlier containers, and those binaries are not in the repository, so the cause
  is not known.

## 20-21 Oct: the Ghidra route's rules, measured on 311 functions, then run on the frame set

Each rule below comes from a fact in the game's code or in Ghidra's own p-code, and each was kept only if the
311-function sample lost no PASS against every earlier run. The sample went from 178 to 205 PASS.
- **Stack layout.** In a function with a stack array, every stack local is a reference into one buffer at its
  Ghidra storage offset, reaching up to the entry ESP. Ghidra splits one buffer into pieces the code indexes
  across: Mat4_PostMultiply's 64 bytes are local_40[4] and local_30[12], and 0x101D7AC0 zeroes 0x15 dwords
  from a local with 24 bytes declared.
- **Bit-level operations.** Ghidra's `undefined4` keeps a float's bits (p-code COPY), and SUBnm takes bytes
  (SUBPIECE).
  - A piece `x._O_S_` has the sign of its own varnode's type (`/*XI_PS*/`).
  - A variable read or written through p-code CAST between a float type and a non-float type keeps its bits
    (`/*XI_BC:1*/`, `/*XI_BC:2*/`). Ghidra prints CAST like a converting C cast. An example is a stack slot the
    code uses as a float and as a loop counter (0x1008F740, 0x101EF0A0).
  - A CAST of a whole expression is not marked yet. Ghidra ties no op to a cast's tokens (0x1006CA20).
- **Arguments.** A cdecl function's stack arguments are the slots its code reads (`cargs`, cdecl_args.json,
  exact counts only). Variadic functions get their fixed parameters from the IDB's declared prototypes.
  - Parameter lists are rebuilt slot by slot, keeping the decompiler's typed parameter where one starts at the
    slot.
  - `replaceParameters` must use FORMAL_PARAMS. With ALL_PARAMS, Ghidra took a `__thiscall` function's first
    listed parameter for `this` and dropped it.
- **Returns.** Return widening keys on the p-code, a register output of the call the caller reads, not on the
  `extraout_` name. 0x1015B580 passes Mem_AllocDefault's EAX on as `this_01`.
- **Functions.** 23 of the 311 had no instructions: the export had made functions on bytes never disassembled,
  so their calls had no references and their callees no conventions. They are now disassembled first. A direct
  call target gets a function only where conv.txt has its convention: one made without it got a convention
  Ghidra inferred, and a wrong pop moved the caller's stack depths.
- **SSE.** A function whose own code does SSE float arithmetic and no x87 arithmetic is compiled with
  `target("sse,fpmath=sse")` (0x1006F0A0).
- **Gate.** A callee's `ret N` comes before the decompiler's pop count, which Ghidra reports as -1 for an
  unresolved function. Ghidra's 1- and 2-byte return names map like IDA's.
- **Hand corrections.** hx_patches (corrections to Hex-Rays text and IDA prototypes) no longer apply to Ghidra
  records.

The frame set (4,765 game-code functions of data/frame_set.json) as one export: 24 minutes of Ghidra, 4,764
decompiled.
- 3,911 were translated and 3,600 compile; 461 were skipped and 16 are untranslatable.
- Of the 460 skipped for an input Ghidra could not tie to a parameter, 147 are `in_EAX` alone. In 0x10001E70
  this is real: the NULL path returns before `xor eax, eax`, so the upper bytes of its byte result are the
  caller's EAX. A caller's EAX passed in cascades through callers, as export 15 showed, so these wait on a
  rule.

Not known, and needed from a run:
- What `[0x1066941C]` holds (set in Dancer_InitDevice from `[[0x103D3CBC]+8]`, CDx). Every call through it
  (0x10282470, 0x10297EC0, 0x10290B60's callee) has no proven pop or register effect. A `facts=1` session at
  those sites would establish them.
- The tree gate measured here: 772 PASS of 779 on fuzz, with 0x100CBDE0 and 0x100412B0 FAIL and 0x10003890
  and 0x100CB4E0 UNRESOLVED. That is besides the three tree/README.md lists. A tree built from 79ded91, before
  this work, gives the same result, so the README's 776 is not reproduced in this container; why is not
  known.

## 21 Oct: frameset4, and the class vtables' slot lists ended where the code ends them

**frameset4.** This run has cdecl_args, spops, ret_width and address-taken frames, but not XI_BC:3 or XI_BA
yet.
- Totals: 3,610 gated; 3,421 PASS, 140 FAIL, 38 INCONCLUSIVE, 11 UNRESOLVED.
- Against frameset3: 31 FAIL became PASS and 17 newly gated functions PASS.
- 0x101B9BC0 is now UNRESOLVED. The rule for a read x87 result of a virtual call on a fuzzed object stops it.
- 0x1025E100 and 0x1025E170 were not gated this time.

**Class vtables.** `data/class_vtables.json`'s slot lists came from a scan that read on while dwords looked
like code addresses, up to 400 slots. Checking DancingMad's §12b vtable lengths showed that 124 of the 226 lists
run into the next object. Examples:
- `CXiAtelActor`'s 283 slots run 29 past 0x1032E7C0, which 0x100A9416 stores as a vtable;
- many lists stop only at the cap of 400.

`pipeline/vtable_bounds.py` now ends each list at the first address the code refers to, or the first dword that
is not a function start. Every one of the 124 ends at a code-referenced address.

Only gfx_cases reads the slot lists: it stubs each slot's function for the fuzzed `this`. Rebuilding
frameset4's cases changed 28 of them, all by dropping stubs for functions of the next table. The 26 that compile
were re-gated, and none changed verdict.

## 21 Oct: frameset4's FAIL classes, each from the code or the decompiler's own p-code

frameset4's 188 functions that did not pass were re-exported and gated after each rule below. These were the
fs4fix runs. They went from 0 PASS to 96 PASS of 198, with the 10 frameset5 lost added. None that passed was
lost, and all now compile.

**New facts, each from the game's code:**
- **`caller_reads.py`** (data/caller_reads.json): the bytes of EAX that a function's callers read after it
  returns. A function counts only when every reference to it is a direct call or tail jump, with no address
  taken anywhere. Calls inside the caller are followed with each callee's own entry reads and kept bytes, and
  `ret` passes to the caller's callers; all of this runs to a fixpoint.
  - Results: 12,219 functions have all callers known. Of these, 4,857 have results no caller reads, 604 have only
    AL read, and 46 only AX.
  - Frame_Render comes out as 1 (AL), which agrees with the earlier reading by hand.
  - The gate compares only those bytes. When none are read, EAX is not compared.
- **`arg_reads.py`** (data/arg_reads.json): the bytes of each stack argument a function reads, from sp_solve's
  exact depths.
  - A function is left out when an argument's address is taken, ESP or EBP is copied, a depth is not exact, or it
    leaves by a jump to another function.
  - Stub argument masks use it. A caller that pushes a byte argument in a dword with leftover upper bytes then
    compares equal (0xDDDDDD79 against 0x79).

**Translator and export:**
- **Thunks.** A function that is one `jmp T` (5 bytes, from the image) becomes a call to T with its own
  parameters. Ghidra inlines T's body there. 275 functions in the image are thunks; in the frame set, 3 FAIL
  became PASS and 6 newly PASS.
- **ST0 returns where no convention is decided** (conv_facts `fret`, from fpret.json). For example, 0x10122E20
  pushes 1, calls a float function and returns its ST0. Ghidra had made it void.
- **PTRADD element sizes** (`XI_BA:N` before `+`, `XI_PA:N` before `[`). Ghidra prints PTRADD(base, i, N) as
  `base + i` or `base[i]`, whatever base points to. Most often the base is `&DAT_x` typed undefined4, with N = 1:
  C++ then scaled the index by 4.
  - Facts that settled it (a debug token dump):
    - the base is an implicit PTRSUB of the global's address, whose own high type is `undefined *`;
    - the export types the global from defined data only, and GhidraScript.getDataAt gives null where nothing is
      defined;
    - in a subscript with an expression index, no token carries the PTRADD; it is found from the base's PTRSUB
      output.
  - In this set, 44 FAIL became PASS from the byte offsets alone.
- **XI_BC:3, implied casts.** A p-code floating op reading a non-floating varnode reads its bits; there is no
  CAST op (0x100817F0: `fld dword ptr [eax+0x8c]` under FLOAT_LESSEQUAL).
- **XI_BC:3, one CAST printed several times.** An implicit output is printed at each use, and through chains of
  implicit outputs: `NAN(x)` and the comparison both print the cast. The first count let marks land on the wrong
  casts, which frameset5 showed as 10 functions no longer compiling. The rule is now:
  - the operand search stops at calls;
  - with another printed cast between this one and the operand, this one is the CAST's only where the CAST's input
    is a conversion's output.
- **XI_BC:2** with a right side XI_BC:3 has already made a bit copy is the same CAST. This fixed 8 functions that
  were untranslatable.

**Other fixes:**
- **entry_regs:** a switch's targets come from vcall_resolve.succs. 0x1004E720 and 0x1004A980 had been taken to
  read the registers they save, since their pops sit behind a switch. That made them "usercall", so the callers'
  ECX (`this`) was never set.
- **The kit:** `facts_frame` (kit/sessions/facts_frame). Sent as facts_frame_73c9ed9.zip.

**frameset5** (4,765 functions, every rule up to the per-use count): 3,490 PASS (frameset4: 3,422), 65 FAIL (140),
38 INCONCLUSIVE, 10 UNRESOLVED. The 10 that passed in frameset4 and did not compile here are fixed above.

**What the remaining FAILs and INCONCLUSIVEs are, as far as known:**
- **Out-parameters written by a stubbed callee.** 0x101A0680 passes `&local_4` to 0x101BE470, then passes
  local_4 on. A stub does not write it, so each side reads leftover stack: the original its entry ECX (the `push
  ecx` that made the slot), the reconstruction 0. The fuzz gate cannot decide these; the callee would have to run.
- **The original reaches an import on every trial** (11 INCONCLUSIVE). The gate ends a run at an import.
  Answering imports would need each one's convention, which data/import_nargs.json does not record.
- **The original faults on every fuzzed trial** (most other INCONCLUSIVEs). These need captured inputs, and
  `capture=` takes only tree functions.
- **The rest:** individual cases not yet read.

## 21 Oct (later): frameset6, `in_EAX` that no code sees, and the tree with the frame set

**Runs:**
- **frameset6** (the per-use XI_BC:3 rule): 3,507 PASS, 63 FAIL.
- **frameset6b** (frameset6's export, translated again with every translator rule): 3,552 PASS, 64 FAIL,
  38 INCONCLUSIVE, 10 UNRESOLVED.
  - Against frameset5, one function was lost: 0x10086900, where the "cast between" rule now leaves
    `(float)((int)fVar6 + 2)` unmarked.
  - A wrongly placed marker costs a PASS or a compile, never a false PASS: the gate judges every result.

**`in_EAX`:**
- data/eax_entry_unseen.json comes from caller_reads.py and lists 10,882 functions. In each, the function and its
  callees read none of EAX's entry value, and no caller reads the bytes it can hand back unchanged.
- hx2cpp starts `in_EAX` at 0 there. Of frameset5's 145 skips for `in_EAX` alone, 50 qualify and 44 pass.
- The other 95 read entry EAX, or hand it to a caller that reads it. The C++ cannot express that without a
  parameter the game's code does not declare: a design decision for the owner.

**The tree with the frame set:**
- The earlier two-hour builds were a header-order failure: every file stopped at its first include and wrote
  hundreds of thousands of errors. After the fixes in 521470e, the tree builds from:
  - the committed IDA export and Ghidra export, with their gates;
  - frameset6's export, with its gate.
- The result is 4,109 functions in 419 files, which compile in about 30 seconds. The committed tree regenerates
  unchanged but for three headers (xi_ghidra_shared.hpp is new).
- Its clean gate needs about 18 s a function on 4 workers, about 5 hours; the raw gate's cache does not apply to
  a new DLL. It runs detached (scratchpad runner, gate_rest.sh), round after round over what has no verdict yet.

**Compile failures found in frameset6** (fixed for the next export):
- 3-byte pieces (`XI_PIECE(x, 1, 3)`);
- x87 `fsin`/`fcos`;
- Ghidra goto labels (`LAB_...`) bound as globals;
- `__fastcall` callees with one register parameter, whose callers were decompiled passing only ECX.

A callee with no `ret N` (no definitive argument count) whose call Ghidra printed with fewer arguments stays a
compile failure: there is no source for the count.

## 21 Oct (night): the frame set promoted into the tree

**The tree** now holds 4,150 functions in 419 files (it held 779), generated from three sources:
- the committed IDA export;
- the Ghidra export;
- frameset6's export (`data/exports/ghidra_frameset.jsonl`, with its raw gate `verification/gate_raw_frameset.jsonl`).

**Verdicts:** the clean gate, run on the new build, gives 4,138 PASS. `// NOT VERIFIED` marks 12: 7 FAIL,
3 UNRESOLVED, 2 INCONCLUSIVE. Three of those were PASS in the 779-function tree, and the new run's verdicts replace
the old records:
- 0x100412B0: FAIL, 3 of 100 trials differ. verify_all's export-sp check already showed that IDA's stack depths in
  this function are wrong (0x100419F4 and on).
- 0x10003890 and 0x100CB4E0: UNRESOLVED. The pop counts of the calls at 0x10003A50 and 0x100CB8CB are now unknown.
The other 776 agree between the two runs.

**A rule found by the promotion** (verify_all step 2b, wrapper-regs): three game functions the tree called as
`__cdecl` read EDX in their own code.
- **0x10313520 `_strncat`** was the tool's mistake. vcall_resolve.ecx_inputs met two paths by the names they
  wrote: `mov dl, ..` on one path and `mov edx, ..` on the other met as nothing written. They now meet as the
  parts both write, so DL counts as written. This changed 4 entries in edx_use.json (`_strncat` among them) and
  none in ecx_use.json or vcalls.json. Test: `EcxUse.test_join_keeps_the_parts_both_paths_write`.
- **0x100A19B0 and 0x1011D990** really read entry EDX bits after a partial write. The tree's wrapper passes
  only stack arguments, and the gate compares a stubbed call's arguments, not EDX.
  - hx2clean.py now leaves out a function that calls a game function reading a register its wrapper's
    convention does not pass (ecx_use / edx_use / entry_regs, an instruction named). The game's code runs in
    its place. `tree/left_out.json` lists the three left out: 0x100A1AB0, 0x101271C0 and 0x10227BF0 (which
    calls 0x10227450, which reads ECX).
  - 0x100A19B0's read is masked away (`shr edx, 3; and edx, 3`), but no tool shows that, so its caller stays
    out.
- verify_all's wrapper pattern skipped `void` wrappers (no `return`), which is how 0x10227450 went unseen. It now
  checks all 3,910 wrappers.

`data/reg_kept_audit.json` covers the tree's gated functions (`gate_clean_final.jsonl`), so it now lists 262 sites
in 67 functions; before, it listed 28 sites in 3. 64 of those functions are new to the tree, and all are from the
Ghidra route. Ghidra follows a register kept across a call itself: 0x1021BC30 uses `self_` where the original
reuses EDX after `call 0x1021BC20`. Their gate PASS judges each translation as printed.

**Names kept stable across exports** (found when the speedfix forks stopped compiling against the promoted tree):
- A global's name and plain type now come from the records of the first export that uses it. Before, they came
  from the most common type over all records, so `g_pCamera` turned from a pointer into `int` and `_as_s32`
  vanished.
- A callee's `_vN` wrapper variants are numbered first by that same export, then by count; before, `sub_1025B560`
  became `sub_1025B560_v1`.
- `canonicalise_names` prefers a name the IDA records use over the Ghidra project's.
- The placeholder pattern now covers a leading underscore (`_dword_X`).

Of the 779 functions the tree held before, three gained a `// NOT VERIFIED` line. Six others changed, and only
in that `operator_delete` is now spelt `operator_delete_10311BB0`, because 0x101907D0 carries the name too.

## 21 Oct: the owner's facts + capture session (build 6AC37846)

**Facts (`facts=1`):** 2,240 sites asked for.
- 64 were refused by the kit, because their live bytes are not the analysed build's.
- 991 were reached; 1,185 were never reached.
- Every reached site gave one pop count over all its calls, so `fact_sites.py ingest` added all 991 to
  `data/game_sp.json`. That is 924 more than the 67 it held.

**Captures:** two calls of Frame_Render, each stopped at the 20,000-page budget (`capture_pages`). Both replay to
PASS with the frame-step switches on (`test_framestep.py`, CAPTURES=...):
- 5 of 8 trials return and are compared;
- 3 are UNRESOLVED at the virtual call returning to 0x10012CC8, on an object the capture did not hold.

The replay runs the analysed build's code on 6AC37846's memory. That is sound here because the builds' data
sections do not move: the kit's manifest maps only code ranges, 0x10232A90-0x10329000, inside `.text`.
Frame_Render lies outside them.

## 21 Oct: names from the PS2 build

The owner supplied INSTALL.ELF, a PS2 executable that is not stripped (Metrowerks MIPS C 2.4.1.01):
- 19,013 code symbols with mangled signatures, and its relocation table;
- no DWARF: `ps2_types.json` in DancingMad still has the types.

They also supplied 23 stripped PS2 builds, from 2002 to 2016, which carry no names. Neither the ELF nor the
builds are in either repository; point `PS2_ELF` at a local copy.

**Reading the ELF.** `pipeline/ps2_elf.py` reads the functions and the relocations:
- `R_MIPS_26`: every direct call;
- `HI16`/`LO16` pairs: every data address;
- `R_MIPS_32`: pointers stored in data;
- `R_MIPS_LITERAL`: literal-pool constants, relative to `$gp` (taken from `.reginfo`).

**Matching.** `pipeline/ps2_match.py` matches a PC function to a PS2 one only on evidence both binaries hold, and
writes `data/ps2_matches.json`. Each match records its evidence and DancingMad's confidence word:
- a literal unique to one function on each side, or a whole literal set: `confirmed`;
- unique float constants, or call-graph position: `candidate`.

The inferred kinds never pair C runtime code: the two runtimes are different code that share only names. The
result is 195 matches.
- **The frame.** Little of the PC frame is shared code. The PC draws through Direct3D and the PS2 through GS and
  VU.
- **Agreement.** Nine matched functions already carried the PS2 symbol's exact name in `runs/ghidra/names.json`.
  Where that list got them is not recorded.
- **Disagreements.** Six DancingMad names differ from their PS2 match on a confirmed literal. They are listed for
  review in DancingMad's `docs/CROSSCHECK.md` §7.

**Class names.** Both builds keep runtime-class descriptors, `{name, size, base}`. The PC's descriptors hold its
own class names (`CMoSchedularTask` where the PS2 has `YmSchedulerTask`), so the tree's class names already come
from the binary. A PS2 class whose name, with a leading `C`, is a PC descriptor name is spelt as the PC spells
it: 7 such pairs (`XiActor` → `CXiActor`).

**In the tree.** `hx2clean.py` names a function from its PS2 match only where neither the IDB, DancingMad nor
this repository's review names it. The same applies to the names of the callees it refers to.

**The 23 stripped PS2 builds (2002–2016) were measured as a bridge and left out.** Of the 3,860 literals unique to
one PC function, 370 appear anywhere in INSTALL.ELF. A stripped build adds:
- 4 to 7 more for the 2002–2003 builds;
- up to 140 for the 2016 build. These lead to 84 PC functions still unmatched, 82 of them with placeholder names.

The 84 are an upper bound, and a loose one. A literal INSTALL.ELF lacks means the function is newer than it,
with no named counterpart, or that it changed since. In the second case the step between the two PS2 builds is
code likeness across years of changes. A name that rests on that is weaker than `candidate`. An unstripped later
build would change this.

## 21 Oct (evening): source shape, phase 1, and the PS2 debug data

**`pipeline/source_shape.py`** runs as the last step of `regen_clean.sh`. It rewrites the clean tree's decompiler
idioms as source code, function by function. Each rule is kept only with the proof that fits it, and where the
proof fails the original text is put back. Nothing here relies on the gate. On the current tree:

| Rule | Kept | Put back | Proof |
|---|---|---|---|
| Ghidra type names as their standard types (`uint`, `float10`, `undefined1`, …) | 2,101 | 0 | A typedef name is its type; each pair is checked with `static_assert(std::is_same)` under the tree's compiler and flags |
| `xi_k4`/`xi_k8` bit patterns as decimal literals | 222 | 0 | The literal reads back to the same bits |
| `CONCAT`/`ZEXT`/`SEXT` as casts and shifts | 411 | 0 | The preprocessed tokens are identical |
| `undefined4` locals as `uint32_t` | 1,496 | 227 | The original compiles with a probe type identical to `undefined4` but without its float conversion, the only place the two differ |
| Parameters named from the PS2 DWARF | 12 | 3 | A renaming only: the preprocessed tokens, mapped back, are identical |

Identical machine code was tried as the proof first and dropped as the general test. GCC's register choices
change with a type's spelling alone (`float10` against `long double`), so identical code is stricter than equal
meaning.

**The owner's `SCUS_972.66`** is the debug build: 14.8 MB of DWARF v1 plus `.line`. It is not in either
repository; point `PS2_DEBUG_ELF` at a local copy.
- `pipeline/ps2_dwarf.py` uses DancingMad's parser to write `data/ps2_dwarf_funcs.json`: for each of the 195
  matched functions, its source file, its parameters and its locals, with their types.
- Matching against `SCUS_972.66` gives the same 195 matches, to the same symbols, as matching against INSTALL.ELF.

**The gate, from frameset8:**
- A callee whose return is 8 bytes now draws EDX as part of its result. Without that, 26 functions failed on an EDX
  the stub had left as each side happened to have it.
- `conv_facts.py` gives EAX as an input to the 312 functions in `eax_inputs.json` whose calling convention is not
  decided.

**Source shape, the `fcmp` rule** (`pipeline/fcmp_simplify.py`). The translation spells an x87 compare as NAN() tests
combined with `<`, `==`, `!=`, `&&` and `||`, which is exact but long. A C comparison of two floats is already
exact on NaN. So an expression over NAN(a), NAN(b) and comparisons of a with b is a function of six states: less,
equal, greater, a NaN, b NaN, both NaN. Where its truth table equals that of one comparison, or of its negation,
it is that comparison.
- **Proof:** the truth table is computed for each rewrite. A rule test also evaluates the forms in compiled C over
  the six states.
- **Purity:** the operands must have no side effects, since the original evaluates them more than once.
- **Result:** 60 rewrites, for example `(!NAN(a) && !NAN(b)) && a < b != (a == b)` becomes `a <= b`.
- **What remains:** most of the 377 NAN() uses left build the FPU status word itself, in functions that return it
  in EAX.

**Two facts for class placement:**
- **`pipeline/reg_saved.py`** (`data/reg_saved.json`): which of EBX, ESI, EDI and EBP each function hands back as
  it found them. Registers and stack slots are tracked symbolically, keyed by `sp_solve`'s exact depths, and the
  facts are solved to a greatest fixpoint over calls. Imports keep all four, as Microsoft's documented x86
  conventions require. 14,460 functions are decided, 8,921 of them restoring all four; 6,950 are undecided.
- **`pipeline/this_flow.py`** (`data/this_flow.json`): ECX at every direct call, relative to the caller's own entry
  ECX (`own`, `own+K`, or `global G`). It covers 12,966 call sites into 4,021 callees.

**What they settle for the tree is small.** Of the 1,194 free `__thiscall` functions with raw `this + offset`
accesses:
- 759 have no call site whose ECX is known: they are reached through vtables or pointers;
- most of the rest get the `this` of callers that have no class either;
- run to a fixpoint, the rule "every caller hands on its own `this`, and every caller is a method of X" places about
  21 more.

Their class has no definitive source. Link order would suggest one, but a `.cpp` file can define several classes.

## 21 Oct: the game's compiler, and the first byte-identical functions

The owner supplied Visual C++ 6.0 toolsets. The stamps were measured: the same files were compiled with each
toolset and the `@comp.id` in each object read.
- **The game's own objects:** the Processor Pack code generator (`c2.dll` 13.0.9044) writes prodid 48 (C) and
  49 (C++) with build 9044. These are exactly the Rich header's rows for the game's 208 C and 799 C++ objects.
- **The stamp comes from the code generator.** An SP5 front end (`C1XX` 12.0.8964) and an SP6 front end (12.0.9782)
  each write 49/9044 when used with it.
- **The 9782 rows:** the Rich header's prodid 10/11 build-9782 objects (176 C, 13 C++) carry exactly the stamps of
  SP6's runtime libraries, LIBC and LIBCMT. So the DLL links SP6's C runtime, and those objects are Microsoft's,
  not the game's.
- **The `vc6.7z` package:** its own compiler writes prodid 21/22.

`pipeline/msvc_match.py` compiles a function's source under wine and compares it with the DLL's bytes, comparing
relocated fields by their target. Two functions match byte for byte under `/O2`, with either front end; they are
in `pipeline/tests/msvc/`.
- **0x100106C0:** 64 bytes and 6 relocations.
- **`CDx::StepFadeCounter` (0x1000AE40):** 96 bytes. Its `fld st(0); fcomp` came only from an assignment inside the
  condition, `if ((d = a * b) <= 0.0f)`. No option and no other spelling of the comparison gave it.

The compilers stay out of the repository.

**Does the front end matter for matching? Measured, on this corpus: no.** The SP6 CRT's own sources were each
compiled with SP5's and SP6's front end. Both used the Processor Pack's `c2.dll`, the same headers and the same
options. Every code section of the two objects was compared byte for byte, relocations included:
- `/O2`: 1,234 functions, 0 differ;
- `/O2 /GX` (the C++ files, exception handling on): 752 functions, 0 differ;
- every file one front end compiled, the other compiled too.

What this shows is that corpus. It does not cover every construct the game's source may use.

## 22 Oct: entry EAX bit by bit, and the tree regenerated (4,205 functions)

**Entry EAX.** `pipeline/byte_flow.py` follows each entry bit of EAX through the registers (`docs/IN_EAX.md`, "Reads
seen and reads assumed"). `data/eax_inputs.json` now has 381 functions that may read their entry EAX and 22 that are
seen to (before: 444 and 16). The gate compares a stubbed callee's entry EAX only in its seen bytes. Where runs differ
only in bytes a callee is assumed to read, the verdict is UNRESOLVED (`combine_gates.py`'s NEWER keeps it so).
Imports' declared returns come from Microsoft's headers (`pipeline/sdk_protos.py`).

**Raw gates rerun** with those facts: the IDA route 770 PASS, the frame set (frameset9) 3,626 PASS, and the previous
frame-set export (`ghidra_frameset_prev`, for functions frameset9 no longer passes) 39 of 50.

**The tree** is regenerated from four exports (IDA, Ghidra, frameset9, frameset_prev; `tree/source.json`): 4,205
functions in 419 files, 51 left out (`tree/left_out.json`: a callee may read a register the wrapper does not pass).

**Clean gate, on this build:** 4,198 PASS. `// NOT VERIFIED` marks 7:
- 0x10040220, 0x1006F0A0, 0x1006F800: FAIL, as before.
- 0x1003C7D0 and 0x100CD540: FAIL in the clean build, PASS in the raw one. The cause is not found yet.
- 0x10003890: UNRESOLVED (the call at 0x10003A50 has no known pop count).
- 0x10016320: INCONCLUSIVE.

One function, 0x10257FF0, had no result because its worker process exited. A rerun on its own gave PASS.

Of the 12 not verified before, 5 now PASS on this build: 0x10038E10, 0x100CB4E0, 0x10053A00, 0x101B9BC0, 0x1021DD20.
0x10038E10's code now comes from the frame-set export (the IDA route's translation FAILs the raw gate). 0x100412B0
is no longer in the tree (raw FAIL on the IDA route), and 0x1006B3B0 is left out.

**Live gates not carried over.** `verification/gate_clean_final.jsonl` now holds only this build's fuzz verdicts.
The four dumps' gates (`verification/live/gate_clean_*.jsonl`) were run on the 4 Oct tree. Some functions' code has
changed since then under the same symbol (0x10038E10 among them), so those verdicts do not verify this build. The
dumps are not in this environment, so the gates were not rerun. `combine_gates.py` would have lifted 0x10003890 and
0x10016320 to PASS on those old runs, and that was not done.

Consequence for the kit: `kit/xikit/make_manifest.py` takes the PASSes of `gate_clean_final.jsonl` and its docstring
says "fuzz and the four dumps". The kit is not rebuilt from this file until the dumps' gates are rerun on this build
or the owner decides that fuzz alone is enough.

`data/reg_kept_audit.json` is regenerated over the new gated set: 247 sites in 71 functions (230 used, 17 overwritten).

## 22 Oct (later): the game's compiler over the whole tree, and two gate errors

**Byte matching.** `pipeline/msvc_tu.py` writes each tree function as a Visual C++ 6.0 translation unit:
- types as MSVC6 spells them;
- `at<T>` as a plain cast;
- a `__thiscall` free function as a member of a struct of its own;
- globals declared from `gfx_globals.hpp`, and game calls from `gfx_orig.hpp`, each checked by address;
- XI_ANY arguments through `xi_any()` overloads that give the same dword.

`msvc_match.py` then compiles each unit with the Processor Pack code generator and `/O2`, and compares it with the
DLL. Of 4,205 functions:
- 1,313 compile to every byte the DLL holds;
- 943 compile to different bytes;
- 499 do not compile yet, mostly implicit pointer conversions GCC's `-fpermissive` accepts;
- 1,450 are left out with reasons: 446 address-taken frames, 360 floating point, 191 calls through a pointer, 150
  GCC attributes, 106 untyped game calls, 69 entry-EAX reads, 59 `static __thiscall` members, and fewer others.

Negative control: in each of 6 sampled matches, a constant changed by 4, or every relocation moved by 16 bytes, gives
DIFFER. Results: `verification/msvc_match_tree.json`.

**Gate errors found through the two clean-only FAILs** (`docs/PROBLEMS.md` R1, R2):
- the clean cases mapped overloads to the first export of their name;
- a fuzzed function pointer into the reconstruction DLL's padding was executed in the original's run.

**A verification hole** (P10): the gate compared EAX only as wide as the decompiler's prototype. 309 Ghidra-route and
73 IDA-route `void` functions, and 92 `bool` ones, have callers that may read more. Those bytes are now compared, and a
difference there gives UNRESOLVED. The four raw gates and the clean gate are being rerun with it.

**Source shape, what the compiler shows** (P11): Ghidra's `CONCAT22(param >> 16, K)` returns are, in the game's code,
`mov ax, K` with the argument's top half left in EAX. Nine spellings tried (a write through the parameter's address,
4-byte structs and unions by value, local copies, and/or masks) do not give that code. The source construct is not
found yet.

## 22 Oct (evening): the tree's last functions, names applied, the community's names

**The tree: 4,049 of 4,050 PASS** (was 4,028 of 4,034). What changed and why:
- 0x1006F0A0, 0x1006F800 (FAIL by one ulp): the export's `sse` flag (float arithmetic in SSE single precision,
  each operation rounded) reached the raw translation but not the clean tree; hx2clean now carries it.
- 0x10045800, 0x101911A0 (UNRESOLVED, EAX past `void`): each caller reads EAX only at a virtual call nothing settles.
  The original's EAX at `ret` is definite (4, the inner loop's counter, on every path; the two values 0x101911A0
  computes), so the reconstruction returns it (data/hx_patches/, now applicable to Ghidra records by text sha1,
  with record fields set alongside).
- sp_solve: every pop is >= 0 (x86 `ret imm16`), so calls in one loop whose pops only their sum fixes, at 0, are each
  0. Self-test 35,480 hidden `ret N` solved, 1 wrong (the same miss as before, 20,751 solved; it reaches no output).
  399 more call sites solved, none lost; arg_reads, reg_saved, this_flow, local_types, cdecl_args, callsite_audit
  regenerated (each only gains).
- data/case_inputs.json: inputs the gate holds fixed, with why, carried into the verdict and the GATE line.
  0x10016320 (App shutdown: virtual destructors through a pointer table) PASSes with the table and g_pApp, g_pCDx
  empty, and says so. 0x10003890 stays INCONCLUSIVE: the original faults through objects in its own `this`.
- 16 of the 25 left out are back (pipeline/gate_sees_eax.py): a kept-EAX call the gate runs as the game does (the
  callee not stubbed, or stubbed handing back all of EAX where the callee keeps EAX on every path) is the gate's to
  judge; verify_all checks the same rule. 9 stay out.
- The re-gate after the new pops found a real defect: Ghidra prints a virtual call it has no convention for without
  its object, and 0x10159A20 called slot 111 of another object with its own `this`. hx2cpp.ghidra_vcall_this passes
  the object where every call site through that slot is entered with it in ECX (ecx_this_sites) and pops a definitive
  count (0x10159A20 and 0x10083F20 now PASS).
- Reviewed names are applied to the tree. Two collided with names the IDB/Ghidra project already gives other
  functions (check_names now refuses that); recursive calls kept their old name (fixed); an object Hex-Rays prints
  another way (`??0X@@QAE@XZ` as `X::X`) is not renamed.

**How the verdicts were obtained.** The container is reclaimed while the session is idle, so a full gate (8 h) does
not finish. `pipeline/code_same.py` compares each function's compiled code across two builds, every address into the
reconstruction resolved to what it names (a function by its FFXIMAIN address, a helper by name, data by its bytes);
the tree's functions call only game code at fixed addresses and the tree's helpers, so the same code with the same
case and the same call-site facts is the same gate run. promo12 -> promo13: 3,824 of 4,048 the same; 328 re-gated
(code, case, facts or fixed inputs changed). promo13 -> promo14: 4,041 the same; 10 re-gated. verification/
gate_clean_final.jsonl holds each function's verdict from the build it was last gated on.

**The community's names** (docs/COMMUNITY.md, owner's rule: trust it where it is documented, measure it where it is
not or conflicts): Ashita v4 and Windower 5 located in the build (37 functions, 32 globals, unique matches only), their
layouts (Ashita by offsetof compiled for i686), reconciled with ours. The entity is the 684-byte object 0x1008A120
builds (Ashita asserts 684); the IDB's "CXiActor" for it is not the runtime class CXiActor (196 bytes). Of the 161
entity fields our code uses, 103 named at the offset, 51 inside a named member, 7 undocumented. The measuring:
kit/xikit `[watch]` and docs/MEASURE.md.

**Naming accuracy measured** (PROBLEMS P16): 63 and 61 correct of 100 known names (a smaller and a larger model); the loss is vagueness
from facts the code does not hold, which field dossiers (pipeline/field_batch.py) did not recover; class_flow.py and
obj_flow.py settle classes from vtables and calls (983 functions).

## 24 Oct: captures of a translated build: pointers into moved code mapped back (`xcap.py --build`)

The 21 Oct entry calls replaying 6AC37846's captures on the analysed build's code sound because the data sections
do not move. That holds for data addresses. A capture also holds values pointing into code, and in 6AC37846 two
code ranges moved (+32 from 0x10232A90, +64 from 0x10309EC0). One capture of Frame_Render (the owner's 18 Oct
session) holds 1,336 such dwords. They were replayed as they were, so a call through one ran other code than
the game's, on both sides alike. That does not make a verdict wrong (input preparation, the same for both
sides), but the input was less real than stated.
- `pipeline/xcap.py --build kit/xikit/builds/<stamp>.json`: a value into the image that lies in one of that
  build's moved ranges is moved back by the range's shift. The capture holds no header page, so the build is the
  one xikit.log names at the start. Values in inserted code (no analysed counterpart) stay as they are.
- Checked on that capture: exactly the 1,336 values change, each now inside the analysed build's moved ranges;
  the other 20,478,664 values are unchanged.
- `test_framestep.py`: `CAPTURE_BUILD=...` passes it on. The captures replayed on 21 Oct have not been
  replayed again with it.

## 25 Oct: speedfix builds again; frame_step through a register-keeping hook; the kit's 2,048-function limit

**speedfix had not built since the naming rounds.** Its forks named callees the tree had renamed, and the getter
0x10014D20 is now `Camera_GetClampedField28`.
- The three frame_step forks were remade with `fork.py new` from the current tree and their edits reapplied
  exactly. `fork.py diff` shows each differs from the tree only by its edits.
- Callees are mapped by address. Four addresses with two prototypes in the tree take the one matching the old
  declaration.
- `sdk/include/xikit_game.hpp` was regenerated.

**frame_step and uncap did nothing in the game from 22 Oct.** The entry-EAX facts gave 0x10014CF0 manifest flag 8.
- On every path the getter writes only AX (`fnstsw ax`) and ECX, so bytes 2-3 of its entry EAX leave unchanged.
  13 call sites read them back (`data/eax_kept_sites.json`; 0x1006A000 is the last call before 0x10069F90 returns).
- So the kit refused the hook, correctly, and speedfix logged that it was not forked.
- Kit API version 3 adds `hook_keeping_registers`. A mod uses it to state that it keeps the registers itself, and the
  kit then requires a trampoline to the game's own code.
- speedfix's `sf_cf0_wrap` runs the game's getter first, keeps its EAX, ECX, EDX and flags, and loads the fork's
  answer into ST0. The x87 condition bits are undefined after FLD (Intel SDM), the game's last instruction on both
  paths.
- sf_host calls the getter with EAX 5A5A1234 and EDX 77665544. Switches off and on, it leaves EAX 5A5A4020,
  ECX = [g_pCamera] and EDX unchanged.

**The kit held 2,048 manifest functions** (`MAXFN`), and the loader stopped there without a word.
- Since the tree passed 2,048 functions (21 Oct), the second half of the manifest could not be swapped, hooked or
  captured.
- test_xikit found it: its swap check names `FontGlyphCache_GetDevice` (0x1025D750), which sat past the limit.
  `Gfx_IsGammaSupported`, which the test used before, left the tree when its raw gate became UNRESOLVED under the
  22 Oct EAX rule.
- The limits are now 12,288 functions and 12,288 `!code` lines, logged when exceeded. The kit loads 4,045 of the
  manifest's 4,046. The one left out, `fn_101A0730`, has an export name longer than the name field; it is logged.
- The quarantine test now excludes `CDx::PushRenderTargetTexture`, which is in the tree too, so the fault stays in
  the game's own code as the test intends.

**Tests:**
- test_xikit: all modes pass. The native sampler mode needs 64-bit Wine, which this container lacked; `wine64` was
  installed.
- test_speedfix: all six modes pass. test_framestep: ALL OK, the owner's three 18 Oct captures replaying PASS through
  `xcap.py --build`.
- tool_drift: the ini-key check now reads keys per section, so `[watch]`'s `enabled=` and `every=` are no longer
  reported as unread.

## 25 Oct (later): 156 more frame-set functions in the tree; naming round 2 done

**The tree: 4,206 functions, 4,205 PASS** (was 4,050 / 4,049; 0x10003890 stays INCONCLUSIVE). Of the frame set's 4,765
game-code functions, 3,748 are now in the tree and 1,017 are not (`data/frame_set.json`, tree flags recomputed against
`tree/placement.json`; the evidence is unchanged).

Where the 156 come from: 522 frame-set functions the Ghidra exports held but whose translation was never gated were
exported again with the current conv facts (`data/exports/ghidra_frameset11.jsonl`, raw gate
`verification/gate_raw_frameset11.jsonl`, listed last in `tree/source.json`). Translator fixes, each from a definite
source:
- A variadic `__cdecl` callee (its IDB prototype ends in `...`, it pops nothing) was given the largest call site's slot
  count, so every shorter call failed to compile (126 of 134 "too few arguments"). Each call now passes the arguments
  Ghidra reads at that site, each a 4-byte slot (`xi_ghidra.hpp` `xi_va_call`; hx2clean's wrapper the same).
- An object Ghidra names `default` (0x103169E8) was declared and replaced the switches' `default:` labels (57 functions).
- String labels keep `<` and `>` of their text (`s_<lastst>_10357e7c`); the auto-name pattern now takes them.
- `CONCAT21`, `CONCAT43` (Ghidra's piece concatenation, sizes in bytes) and `fptan` (x87 FPTAN pushes 1.0 after
  tan(ST0), Intel SDM; popped) added; the SDK layouts of `_WIN32_FIND_DATAA`, `WINDOWPLACEMENT`, `tagCOMPOSITIONFORM`,
  `timecaps_tag`, `LARGE_INTEGER` and four handle/pointer typedefs added to `xi_compat_win.hpp`.
- `export.sh` takes `GHIDRA_PROGRAM` when the project's program is not named like `FFXI_DLL`.
Raw gate of the re-export: 226 gated, 156 PASS, 44 FAIL, 16 UNRESOLVED, 10 INCONCLUSIVE. 181 are still skipped (a
register or x87 input Ghidra could not tie: `extraout_ST0` 114 sites, `extraout_ECX` 62, `in_EDX` 29, ...), 78 do not
compile, 15 call winsock ordinals with no SDK prototype.

How the tree's verdicts were obtained: `code_same.py` against the 24 Oct build: 3,967 functions compile to the same
code; 237 (the 156 new and 81 whose code the translator fixes changed) were gated on this build: 237 PASS. A last
regeneration with the newest names compiles to the same code for all 4,204 it compares. The tree builds (392 files)
byte-identical to that build. speedfix's Frame_Render fork took 14 renamed `orig::` names (by address).

**Naming round 2 is complete:** 132 of 132 batches; `data/names/` holds 2,831 reviewed names, applied by the
regeneration. Round 3 (`batches/round3/`, from this tree): 385 frame-set functions still with placeholder names in 61
batches.

**Naming round 3 is complete:** 61 of 61 batches, 385 names (167 high, 101 medium, 117 low confidence); `data/names/`
now holds 3,212. Names come from the game's own strings where it has them (sqMemCheck, sqRealloc, sqmdDMBResolve*,
sqmdModel*, the `.MRK`, `.log` and `cnf.dat` file names, the window class's `lpfnWndProc`, checked in the DLL at
0x10016630), from the SDK where a call fixes the meaning (RegisterClassA, ReleaseSemaphore, the MD5 constants of a
second, count-first MD5 context at 0x100DDE90), and otherwise from what the body does, said as such: a low name
describes the visible mechanics (`Unit10086E90_UpdateByKind170NotGroup38`) and its summary says what is not
established.

The regeneration with the round-3 names: 4,163 functions compile to the same code as before; 41 changed, because
a `Class_Method` name places a function in its class (the generator's existing rule: `CMoAttachments::GetCaster`,
`CMoTask_CtorByMode` into `CMoTask.cpp`, which empties `unit_100353E0.cpp`). The 41 were gated on the new build: 41
PASS. The tree is 4,206 functions, 4,205 PASS, 1 unverified; it builds from 391 files, failed none. speedfix's
Frame_Render fork took 5 more renamed `orig::` names, by address.

**The largest x87 skip, read:** 18 of the 114 `extraout_ST0` sites follow a call to 0x103138BA, which is
`mov edx, 0x103CFFE0; jmp 0x1031DD30`. The table at 0x103CFFE0 begins with the length-prefixed name `fmod` (DLL
bytes); the dispatcher classifies two operands (`fxam` on ST0 and ST1) and jumps through the table's 16 entries;
the normal-operand entry 0x103138C4 is `fxch st(1); fprem` (repeated while C2 is set) `; fstp st(1); ret`: two values
in on the x87 stack, one out (Intel SDM for FPREM/FSTP). fpret.json has no verdict for it (the stub jumps out of
its own extent). Ghidra prints these calls as `FUN_103138ba();` with no operands, so the two values the caller
loaded are not in its text: the sites cannot be translated from Ghidra's output alone, and stay skipped until
the operands are recovered from the instructions before each call. Measured: every one of the 24 call sites
(9 functions) has the same shape, `fld dword [esp+X]; fld qword [0x1032ABC8]` (2.0, DLL bytes) and the call, so the
operands are recoverable with the stack depth at each site; but none of the 43 skipped functions with an
`extraout_ST0` is blocked by fmod alone (each has other untied x87 values or register inputs), so that fix by
itself brings no function into the tree.


## 25 Oct (evening): the x87 stack across calls, read from the code; compile and gate failures

**x87_effects.py** (new, `--selftest`): for every function, what it does to its caller's x87 register stack --
how many values it takes (k), how many it leaves (r), how deep it pushes (m) -- followed along every path from the
game's instructions (Intel SDM push/pop and register-operand rules), callees by their own verdicts to a fixpoint.
Rules, each from a named source and in the file's doc: a call whose target the code does not fix (import, indirect,
pointer tail jump) is taken only at an empty x87 stack (Microsoft x86 conventions: empty on entry and return but a
float return in ST0), and what it returns is established from the caller's own code (vcalls.json `st0`; the caller
reading below its own pushes; a tail jump's return fixed by the function's other returns, ISO C 6.9.1), never
assumed; switch tables are followed for the index values every path into the jump establishes (bound checks,
borrow `sub; jb`, `and`, `mov`, `neg`, `add`/`sub` imm); code past IDA's extents is decoded by control flow;
`fnstenv`/`fldenv` pairs that keep the saved stack top are followed through (the 32-bit environment layout).
Result: 20,539 of 21,410 functions with a verdict; 871 refused, each with its root cause (614 of them are the fmod
intrinsic's error path, which calls RaiseException with the two operands on the x87 stack). The selftest checks the
two instruction-level readings (__ftol: 1 in, 0 out; fmod's normal entry 0x103138C4: 2 in, 1 out) and every
disagreement with fpret.json against fpret's two blind spots (a tail jump to a value-popping function; a call to a
float-returning function it has no verdict for); none is unexplained.

**Applied in the Ghidra export:** conv_facts.py takes float returns from x87_effects.json (fpret.json where it has
none) and an indirect call's ST0 return; a function leaving the x87 stack as it found it, called with values on
it, gets a prototype model with ST0..ST7 unaffected and no ST0 output (keepModels `st`), so the caller's values live
across the call (0x10027B80: `fcos` kept across 0x100279B0). GfxExport exports, per record, which call left each
`extraout_ST0` (`x87_rets`, Ghidra's INDIRECT op on ST0); hx2cpp assigns the value from that call where
x87_effects proves it returns one float at an empty stack. An indirect call's override returns `double` (a
`float10` override is returned through a hidden pointer, an argument the call does not have); the C++ call reads all
of ST0 (`float10`, from the record's icalls). A virtual call through a slot every site of which returns a float in
ST0 is typed float10 (ghidra_vcall_this).

Of the 60 skipped frame-set functions with an x87 register Ghidra could not tie, 16 now translate; gated: 11 PASS,
3 UNRESOLVED (two virtual calls whose x87 result the gate cannot know; an EAX no fact says no caller reads). The rest
stay refused, each named: a callee's net x87 effect that is not zero with values below its operands (Ghidra's fixed
ST registers cannot shift), x87 inputs of the function itself, other registers Ghidra could not tie.

## 25 Oct (late): fs13 FAIL classes, and where the promotion stands

The 51 fs13 gate FAILs (records not in the tree), grouped by their first mismatch:

* 25 stubbed-call argument differences. About 15 are the same byte with different upper bits (`0xE996FE03` vs
  `0x03`) in an argument the callee reads in full (`arg_reads.json` mask 0xFFFFFFFF). These are real: the
  reconstruction cuts a dword. In the cases traced (0x101609F0, the 0x100EExxx family), the dword is the result of
  a callee Ghidra prints as returning `undefined` (1 byte, its unlocked placeholder, not a declared type), such as
  0x10152EF0. That function's code sets all of EAX (`mov eax, [edi*4+0x10578548]`).
* 15 reconstruction faults on low addresses (0xFF, 0xA0, 0): the same cut result used as a pointer
  (`(char *)F_10152EF0(8,0x16)`).
* 9 memory and 2 EAX differences, most with an untyped-return callee in the function.

Tools, from the code (committed):

* `ret_width.py` writes `data/ret_full.json`: the functions whose code sets all four bytes of EAX on every path
  (7,962). A tail jump to code that is no function start in the extents is now decoded by control flow
  (`add_jump_targets`), which adds 88. `ret_width.json` is unchanged.
* hx2cpp widens a callee's `undefined`/`undefined1`/`undefined2` return to `undefined4` only where
  `ret_full.json` lists the callee. Re-translated and gated (run fs14, same export): among the records not in the
  tree, FAIL 51 → 23, PASS 48 → 74 (26 FAIL → PASS, 2 FAIL → UNRESOLVED, no PASS lost); tool_drift on the
  installed tree: 0 differences. verification/gate_raw_frameset13.jsonl is now the fs14 gate. Callees whose code cannot
  be followed (0x100F1920: `jmp [eax+0x2F8]` through a pointer table) stay as Ghidra prints them; nothing is
  known about their width.
* hx2clean: a Ghidra record's global is declared exactly as its raw translation declares it
  (`hx2cpp.ghidra_global_type`, shared). A `code *` global or parameter the text calls is spelt `xi_code *`
  (gfx_common.hpp), so it can be called. Without this, four units of the promo17 tree failed to compile, and
  `repair_compile.py` skipped them because not all their errors were pointer conversions.

Promotion (installed, promo17): the fs13 export (raw gate from fs14) is registered in tree/source.json. The tree
builds 4,278 functions with no failed file (code_same against the previous tree: 4,129 same, 149 changed). The
clean gate on the 74 new and 75 changed functions gives 149 PASS; 4,279 functions are annotated PASS, and one,
0x10003890, stays INCONCLUSIVE as before. The kit tests pass (speedfix framestep, skinmerge, speedfix;
sdk/test_fork.py). The called-pointer type was renamed `xi_codefn`: `xi_code` is already xi_decomp.hpp's address
function in the in-game build. data/this_flow.json was regenerated (one more site, 0x10321430 in 0x1031A00D):
it had not been regenerated after purges.json gained 64 entries, and nothing but verify_all reads it.

verify_all --quick on the promoted tree: tree-regen PASS (392 files, none failed), all tree sources PASS. FAIL:
reg_kept_audit and export-sp (0x100419F4) as recorded before (9a507d2); and three facts stale against their
inputs since purges.json gained 64 entries (38734c7): sp_solved.json (12 items), cdecl_args.json (9),
this_flow.json (regenerated in this commit). hx2cpp and gfx_cases read sp_solved/cdecl_args, so regenerating them
can change translations and cases: that is the next round (regenerate, re-translate, gate), not part of this one.

## 25 Oct (evening): stale facts, the kit's two new recordings, the game's compiler, naming round 4

**Facts regenerated.** sp_solved.json and cdecl_args.json, stale since purges.json's 64 new entries (38734c7):
cdecl_args gains an exact count for 8 functions from their new `ret N`, sp_solved loses the 11 call sites into
them (now known from purges). sp_solve --selftest: the one known miss (0x10318674), reaching no output. verify_all
--quick: both PASS. reg_saved.json, which reads cdecl_args, then changed for 73 functions (ESI now proven restored);
`pipeline/regen_facts.py` (new) re-derives every fact in verify_all's order, installing each, until a pass changes
nothing, so the facts that read each other settle together. reg_kept_audit.json, a report nothing reads, was
committed from its first pass (the verify_all FAIL recorded since 9a507d2); the rest follow when the run settles.

**A soundness fix with no effect.** x87_effects.py's index_values and flag_setter took capstone's operand access for
a `call`, which lists only ESP as written: a value or the flags were carried across a call as if the callee kept
them. A call now writes every register but ESP (an EBX/ESI/EDI/EBP its callee restores, data/reg_saved.json, is
kept) and ends the flag walk. x87_effects.json is unchanged (no switch was decided through a call); selftest OK.

**Two FAIL classes read, not fixed:**
* 0x101A04E0 / 0x101A03D0 pass `local_4` to a callee after `File_FindByKind`, whose output it is. The stub writes
  nothing, so the original passes what MSVC's `push ecx` left in the slot (the entry ECX, 0xCCCCCCCC in the gate)
  and the reconstruction its own stack. ecx_use.json leaves both undecided for that reason. Whether
  File_FindByKind always writes its output on success is a fact about its code no tool derives: the kit's new
  `out` recording measures it (below). A gate change that fills unwritten frame slots was tried and dropped: the
  original's `push ecx` is itself a write, so the slot is not unwritten.
* 0x102110C0's callee 0x102112B0 reads only the low word of its argument, but arg_reads.json left it out: IDA
  gives it a second chunk (0x1012CA90) that reads the slot as a dword after `mov [esp+4], imm` overwrote it, and a
  tail jump. A must-store analysis (bytes stored on every path are not the caller's) and following tail jumps at
  depth 0 decide 105 more functions, 0x102112B0 as [0xFFFF]; tested in a scratch copy, not committed yet.

**Switch tables.** 179 `jmp [r*4 + T]` in the DLL are not bounded by sp_solve.switch_targets, mostly because flag-
neutral instructions sit between the `cmp` and the `ja`. A generalised proof (walk back from the jump: modelled
index transforms, the Intel SDM's "Flags Affected: None" instructions between cmp and ja, a register bound set by
`mov reg, imm` in the run, no jump landing after the first instruction used) bounds 104 more; on the 74 of them
x87_effects.index_values also bounds, the targets agree exactly. Not committed yet: every fact that follows control
flow changes with it (its own round, through regen_facts.py). 0x100DA750, the callee behind fs14's 0x100DAF30
FAIL, needs the register case across joins (`mov ebp, 0xCB` before a join) and stays unbounded.

**The kit records output slots and switch jumps** (`facts=1`, kinds `out` and `jmp`, docs/KIT.md): 3,895 sites,
1,618 of them the calls in the FAIL functions the tree does not hold, 179 the unbounded switches. Measured values
go to data/game_outs.json and data/game_switch.json through `fact_sites.py ingest`; a measurement shows what the
game did, never that something cannot happen. The kit also read manifest exports into 160 bytes and dropped two
functions with longer mangled names (0x101A0730, 0x101A0770); now 256. All 12 Wine test modes pass. Delivered as
xikit_measure.zip with MEASURE.md's Block C.

**The game's compiler** (docs/RECOMPILE.md). The DLL's Rich header: 1,007 objects stamped 48/49 build 9044. The
owner's Visual Studio 6 archive, assembled by pipeline/vc6/setup_vc6.sh (outside the repo, under Wine): SP6's front
ends with the Processor Pack's c2.dll stamp exactly that. The 189 objects of 10/11 build 9782 are not reproduced
(SP6's front ends refuse the CD's c2.dll; this SP6 holds none). pipeline/vc6/match.py compiles a source and
compares each marked function with the game's bytes: 0x100279B0 (55 bytes) and 0x10027CD0 (30 bytes), written by
hand, are identical at /O2. Statement order is what decides them (one assignment per element in index order; four
member assignments rather than a struct copy).

**Naming round 4:** 66 of 66 (23 batches, `batches/round4/`): 4 high, 26 medium, 36 low. Not yet applied: the
regeneration that applies them waits for regen_facts.py to settle (it reads data/).

## 25 Oct (night): the plan, faster tools, and two things the gate could not see

**The plan.** docs/PLAN.md orders the owner's chain (source, devkit, D3D11, performance, final devkit and speedfix)
in 5% steps, each with a check. It takes the steps of ROADMAP, D3D11, GAPS, REEVALUATION and RECOMPILE, and is
corrected for runs already done: d3d8on11 runs the game (perf60 run 2, the house runs), and T1 and T2 are measured
(runs 8980 and 8452), T3 is not. docs/WORKERS.md describes batches for worker sessions.

**Round-4 tree installed** (6875e2b): 66 names, the 151 functions whose code changed all PASS. Kit tests
(framestep, skinmerge, speedfix, sdk fork) all OK.

**The VC6 sweep** (4c1a60e):
- compiles in parallel with an object cache: 22 s cold, 3 s cached, verdicts unchanged;
- `differs.py` classes the DIFFERS from both instruction streams;
- five rewrite rules, each kept only where match.py says IDENTICAL: 158 more functions at the game's bytes
  (1,591 of 4,121).

**regen_facts.py runs only what changed.** An audit hook (pipeline/regen_reads/sitecustomize.py) records every
file each tool's Python reads, and the directories it lists. A directory the import system lists counts only by
the entries an import could resolve to. A tool runs again when any of that changed, its command changed, or the
hook or the interpreter changed. Tools run concurrently by their recorded dependencies and within 80% of the
memory available: caller_reads.py peaks at about 5 GB, and four copies at once were killed. The first full
recording pass took 34 min and changed nothing.

**What the sweep showed the gate cannot see (docs/PROBLEMS.md P17).** At 0x100F1870 the game pushes six
arguments to the dispatch thunk 0x10310067, and the tree passes none; the gate PASSes it, because a caller-cleanup
stub compares only the slots the callee is known to read. `call_pushes.py` settles what the caller pushes for
each caller-cleanup call, from its own code and exact depths: 11,570 calls, 2,376 callees with one count. 74 of
the tree's wrappers declare fewer than that. The gate now compares the pushed slots where no exact read set says
they go unread (gfx_cases.py).

**An assumption in sp_solve, removed.** A direct call to a known function with no `ret N` record was taken never
to return. 1,709 functions end in a jump instead, the dispatch thunks among them. Their pops are now unknowns solved
from each caller's balance:
- the self-test passes (0 misses reach the output);
- solved calls go from 4,717 to 8,537, none lost or changed;
- with the generalised switch bound on top: 8,693.

**The fact round and the re-gate (26 Oct).** The facts above, committed at their fixpoint (0f13298). The gate then:
- stubs log the slots callers push past those compared (`args_may`, verification/cases_raw_final.json via
  `case_pushes.py`: 181 stubs in 166 cases); a difference there is UNRESOLVED;
- the stack starts filled with 0xA5A5A5A5, not zeros.
The whole tree, gated again (34 min): 4,229 PASS, 42 UNRESOLVED (P17), 5 FAIL and 4 INCONCLUSIVE (P18, 0x10003890
among them). The tree's GATE comments and frame_set_status.json follow; the kit's manifest is not rebuilt yet.

## 26 Oct: the fs15 export into the tree

**The export.** The 943 frame-set functions not in the tree, with the 51 tree functions not PASS, exported by Ghidra
after the fact round (data/exports/ghidra_frameset15.jsonl, 993 records). Its raw gate (verification/
gate_raw_frameset15.jsonl): 43 PASS, 61 FAIL, 69 INCONCLUSIVE, 205 UNRESOLVED. 154 of the UNRESOLVED are void
functions whose EAX differs at return, with no fact on whether a caller reads it (docs/PROBLEMS.md P20).

**A function that does not compile is left out, not fixed by hand.** The first regeneration placed 33 new
functions, but one unit failed: 0x10018850 compares a global the headers declare `undefined4` with a pointer.
regen_clean.sh now maps each failing unit's first error to its function (pipeline/drop_failed.py) and generates the
tree again without it (`XI_DROP`), at most three times. The game's own code runs in its place, and left_out.json
gives the compiler's message.

**The clean gate.** 32 new functions and 2 changed:
- 31 PASS; 0x10119BC0 and 0x100180D0 FAIL; 0x100289F0 INCONCLUSIVE.
- 11 had no case in cases_raw_final.json. fs15's cases for them were added.
- 17 had an older case that differs from fs15's own. Each was gated under both, and the stricter verdict stands. The 3
  not PASS pass under fs15's case only: the older case compares EDX, which no fact says callers ignore (P9).

Tree: 4,312 functions, 4,258 PASS. Frame set: 3,854 of 4,984 in the tree, 3,800 PASS.

## 26 Oct (later): raw verdicts under the current rules; argument bytes followed through registers

**Older raw verdicts, given again.** hx2clean.py builds a function from the first export whose raw gate passed it,
and the older exports' raw verdicts predate the 0xA5 stack fill and `args_may`. For the tree's non-PASS functions
that another export also passes raw (10), every older-export record was gated again under the current rules: 21
records, verification/gate_raw_regate.jsonl, read through `XI_REGATE` (tree/source.json `raw_regate`; verify_all
passes it on). None still passes. So:
- 0x100542F0 (FAIL), 0x100157B0, 0x101033C0 and 0x1027CCF0 (UNRESOLVED) are now built from fs15's translation, and
  all four PASS the clean gate, under both their cases;
- 0x10073D80, 0x100D4330, 0x100EEE90, 0x101C09B0, 0x101C0A40 and 0x101C3A90 pass raw in no export now, and left the
  tree (none passed the clean gate);
- 63 other functions changed only in how a global is spelled, which follows the records chosen; all PASS.
Tree: 4,306 functions, 4,262 PASS. Frame set: 3,848 in the tree, 3,804 PASS.

**arg_reads.py follows a loaded argument.** A whole-dword load of an argument into a register used to count all
four bytes as read. Now the register is followed (byte_flow.py), with direct and import calls handled as
caller_reads.py handles them. At a `ret`, ECX/EDX count only where some caller reads the register after the call
(every caller known; a fixpoint over callers that pass it on). 2,928 functions now read less than whole dwords of
some argument (2,677 before), and no mask is wider. In stubs, a full code mask falls back to the declared type's
mask per argument, as an unlisted function always did (gfx_cases.stub_masks; case_pushes.py applies it to
existing cases: 404 stubs in 388 cases).
- **Result:** no function gained a PASS. Nothing in the tree flipped, and fs15's raw gate gave the same 43 PASS with
  18 of its cases narrowed. 0x1021C380, the case that prompted the tool, stays FAIL: the leftover bytes of its
  argument reach ECX at the return of callers whose callers are not all known.

## 26 Oct (night): returns widened, pushed slots passed; three workers

**P20: the reconstruction hands back what the original leaves in EAX.** No sound caller set exists for a virtual
method, so instead fs15's 155 functions UNRESOLVED on EAX past a declared void were exported with a 4-byte EAX
return (export.sh `XI_RETW`, GfxExport.java `retw`) and gated on all of EAX: 131 raw PASS (fs16), 128 promoted.

**P17: callers pass the slots they push.** A callee of undecided convention that pops 0 now takes the bytes every
settled caller pushes (conv_facts.py `spops N pushed`). fs17, the 45 functions held there: 38 raw PASS. In the tree,
32 held functions became PASS after their older records were re-gated under the current rules, and 6 are new.

**P9:** data/regs_after.json (pipeline/regs_after.py) records whether any caller reads the ECX/EDX a function
leaves; some caller of 0x10119BC0 may read its EDX, so its FAIL stands. gfx_cases.py compares only EAX of an 8-byte
return no caller reads EDX of.

**Workers (each in its own worktree; the integrator merged):**
- docs/FORMATS.md (#128, #129): resource chunks, Os2 skinned mesh, Sk2 skeleton, Mo2 motion, Bm2 texture, zone
  mesh, dancer layer, all CreateVertexShader declarations; pipeline/field_reads.py and pipeline/vs_decl.py.
- docs/RECCMP.md (#114): reccmp sees all 58 annotated functions and scores 57 at 100%, but its 100% is weaker than
  match.py's IDENTICAL (a wrong unnamed global still scores 100%); a second view, not a verdict.
- VC6 sweep (#116): NO_COMPILE 603 to 39; at the game's bytes 1,625 to 1,778 (recompile/sweep_round_116.md).
  match.py --all: every file in recompile/ still IDENTICAL.

Tree: 4,442 functions, 4,428 PASS (from 4,262). Frame set: 3,984 in the tree, 3,970 PASS; 1,491 (29.9%) at the
game's bytes.

## 27 Oct: the rest of the frame set, imports stubbed, a name clash

**What was left.** Of the 1,000 frame-set functions outside the tree, 595 are statically linked library code, which the
translator marks `library`: D3DX 8.1 (matched by symbol: hx2cpp.py D3DX81) and the D3DX / C++ runtime (hx2cpp.py
LIBRARY). 219 of them are the frame set's `d3dx` entries, exported as fs18. Those are no reconstruction target. The other 405 were exported again with every current fact (fs19; `retw` for the 23
UNRESOLVED on EAX past a declared void or bool, which now also widens a 1-byte return).

**Imports, stubbed like game callees.** 34 functions were INCONCLUSIVE because every trial stopped at a Win32
import. data/import_pops.json (pipeline/import_pops.py) gives the bytes each import's call pops, from the game's
solved call sites (199 imports, none disagreeing). A case with `"imports": "stub"` answers such an import like a
stubbed game callee: the call and its arguments compared, EAX from the trial's stream, nothing written
(diffemu_gfx.py). New cases carry it (gfx_cases.py); existing cases keep their behaviour, so recorded verdicts stay
reproducible. 16 of the 34 then PASS raw; in the tree each also PASSes under its older case with the stub added.

**A name clash broke every unit.** The game's 0x10009770 is named SetCursorPos, and an fs19 record reads the import
table slot of that name: the slot's macro rewrote the function's declaration in a shared header, no unit compiled,
and the leave-out rule then dropped 141 tree functions. Two fixes:
- a global whose name is a function of the tree's gets `g_<name>_<addr>` (hx2clean.py);
- drop_failed.py refuses to drop anything when an error lies in a shared header, and regen_clean.sh stops there.

**Workers.** The VC6 sweep's LEFT_OUT went from 419 to 252 (recompile/sweep_round_117.md); the 252 left all call a
function that takes an argument in EAX, which no VC6 convention passes.

Tree: 4,463 functions, 4,449 PASS. Frame set: 4,005 in the tree, 3,991 PASS; 1,523 (30.6%) at the game's bytes.

## 27 Oct (later): the owner's in-game captures replayed

The run's eight captures (four functions, two each; build 6AC37846, mapped back by the kit manifest) failed to load
with UC_ERR_MAP: live_state.sandbox put the replay's scratch region at 0x5F800000, which the grown reconstruction DLL
(at 0x60000000) now overlaps. sandbox() now takes the ranges to avoid (the capture's own pages, the reconstruction's
image: live_state.pe_range, and 0x6E000000-0x80000000). Verdicts against tree_r11:

| Function | Capture 1 | Capture 2 |
|---|---|---|
| Frame_Render 0x10011A00 | PASS (41 of 1,424 instructions) | PASS (41) |
| Cell_DrawQuadBlended 0x1011F300 | PASS (93 of 597) | PASS (93) |
| CYyModelDt_SkinAndDrawV2 0x1002DC10 | PASS (47 of 1,596) | UNRESOLVED (638) |
| TexObj_Upload 0x10039E50 | PASS (134 of 1,077) | PASS (134) |

The UNRESOLVED: 3 trials return with EAX differing above the declared 2-byte return, and no fact says whether a
caller reads it (caller_reads.json). Not settled; the `retw` export (P20) of 0x1002DC10 would make the gate compare
all of EAX.

## 27 Oct (night): indirect call targets by source; the frame set follows the recorded ones (#106)

`pipeline/indirect_targets.py` (`data/indirect_targets.json`) lists every indirect call and jump with its targets,
keeping each source apart. Import slots (1,535 sites) and bounded switches (869 sites) are not listed. That leaves
8,934 sites: 8,466 calls and 468 jumps.
- `static` (585 sites): vcall_resolve's complete sets. 561 of them are in vcalls.json. The other 24 are calls whose
  callees the vtables settle but whose pop counts disagree or are unknown, so vcalls.json leaves them out.
  `vcall_resolve.resolve()` now also gives each site its reason; vcalls.json and ecx_use.json come out byte-identical.
  No jump has a static set: an indirect jump that is not a bounded switch stops the this-flow for its whole function.
- `com` (217) and `com_accepted` (153, the CDx+0xC declaration, kept marked unproven): com_calls.json.
- `recorded` (716 sites with no static or com source; 16 more that have one): game_sp.json / game_switch.json. The
  kit writes game addresses at the analysed build's addresses (xikit.c `analysed_of`, using the manifest's `!shift`
  ranges), so the 6AC37846 recordings need no mapping here. Checks: every recorded game target is a function start,
  and at the 16 sites with both a recording and a proof, the recording lies inside the proven set.
  **Observed, not proven complete.** 332 of the 716 have a game target. The other 384 are recorded only into
  Hook.dll, d3d8.dll, other modules or allocated memory, so where a third-party hook stands in, the target the
  game's code would reach is not seen.
- Unresolved (7,263), by reason:

  | Reason | Sites |
  |---|---|
  | pointer not loaded from `this`'s [+0], and not a proven D3D8 object | 6,222 |
  | through a register | 631 |
  | `this` may be any object | 428 |
  | indirect jump | 393 |
  | function has an unfollowed jump | 146 |
  | unbounded switch | 75 |
  | function pointer in .data | 60 |
  | other | 24 |

  The first kind includes the device-shaped calls through `[[0x1045666C]+0xC]` and `[0x1066941C]`, whose
  interface no proof gives. Each unresolved site records whether facts_wanted.json asked for it.
- `frame_set.py` now reads this file. It follows the static sets, including the 24 the old file left out, and the
  recorded game targets under a new evidence kind, `recorded`. It now also counts `call [abs]` through .data,
  which it had taken for import calls, and unproven indirect jumps.
  - The regenerated frame_set.json grows from 4,984 to 5,433 functions: 449 added, none removed, none D3DX, 10 of
    them in the tree.
  - `static` goes from 4,465 to 4,466 (0x10082460, through the pop-unsettled vcall at 0x10081BD9).
  - `recorded` holds 494 functions. 46 of them had been in the set only through the sampler.
  - Unproven indirect calls in static functions: 1,768 in 472 functions (it was 1,756 in 466). That is +13 `[abs]`
    calls through .data and -1 settled. Of the 1,768, 206 are recorded. Unproven jumps: 121.
  - The recorded-reach functions hold 83 more unproven calls. So the set stays a lower bound.

## 27 Oct (night): the remaining FAILs read (P18, plan #170 and #108)

Every FAIL and INCONCLUSIVE of the tree's clean gate, P18's list and fs14's 20 open FAILs read to the first
mismatch's cause (docs/PROBLEMS.md P18, a table per function). Four were defects with a definitive fix:
- GfxExport: a Ghidra thunk's convention, signature commit and EAX width are its target's (`sigOwner`); a thunk
  used as a value is bound to its own address where the function's instructions hold it (`valueAddress`);
- vcall_resolve: `lea ecx, [ecx]` is no read of ECX (ecx_use.json, 4 entries; _stat's callers now pass its two
  arguments; regen_facts.py changed nothing else);
- hx2cpp: a float stored into an `undefined4` global is a bit copy; a virtual call through an object expression
  passes the object (the ecx_this_sites rule, unchanged).

Export fs23 (data/exports/ghidra_frameset23.jsonl, 29 records, not promoted) and its raw gate
(verification/gate_raw_frameset23.jsonl: 14 PASS, 14 FAIL, 1 UNRESOLVED): 0x1013A100, 0x101BE6C0,
0x1015B410, 0x10221030, 0x10221680, 0x100F7790, 0x100F78C0, 0x100F7960 and 0x102710B0 PASS. Regression check:
133 functions exported with the old and the new GfxExport, 99 records differ, 6 FAIL -> PASS and none worse; the 5
tree records hx2cpp now prepares differently all PASS raw. The rest stay as they were, each with the fact that
would settle it (EDX past a narrow return, stack nothing wrote, stub outputs, pop counts, P19).

## 27 Oct (late): tree r14, and the live gate on the four dumps

**Tree r14** (pipeline/tree_round.py, `--last fs24`): 4,897 functions, 4,881 PASS on fuzz. fs24's raw PASSes entered;
the three switch cases (0x10223B4E, 0x102262EE, 0x1022635E) were left out with the reason. Against r13's records: 11
FAIL -> PASS (hx2clean's regparm rule, 36639d3), none worse.

**The live gate** (P1), first run on the current tree: the four dumps' states rebuilt with live_state.py (the dumps
are of the analysed build), and the 16 functions not PASS on fuzz gated on each. combine_gates.py's rule (PASS where
some gate compared and passed and none failed; a FAIL anywhere is a FAIL):

| Function | Fuzz | Live, each of the four dumps (returning trials) | Combined |
|---|---|---|---|
| 0x10003890 | INCONCLUSIVE (every trial faults) | PASS (3 of 100) | PASS |
| 0x100E08D0 | INCONCLUSIVE | PASS (2 of 100) | PASS |
| 0x10010D20 | UNRESOLVED (argument bytes pushed to 0x10028FD0) | PASS (100) | PASS |
| 0x100180D0, 0x10119BC0, 0x101BE6C0 | FAIL | PASS | FAIL (a FAIL anywhere) |
| the other 10 | as on fuzz | as on fuzz | unchanged |

The two INCONCLUSIVE -> PASS rest on 12 and 8 returning trials in all: the rest fault in the original code as well
(state the dump does not hold). Recorded as the rule gives them, with the counts in each record's `gates` field
(verification/gate_clean_final.jsonl; per dump verification/gate_clean_live14_*.jsonl).

Tree: 4,897 functions, 4,884 PASS. Frame set: 4,449 in the tree, 4,437 PASS; 1,612 (29.7%) at the game's bytes.

**r14's left-out switch cases, carried into verify_all.** `tree_round.py regen --drop --why` wrote the drops to
OUT.given_drop.json, but install did not record them and verify_all.py did not pass them on, so a verify_all regen
would have put 0x10223B4E, 0x102262EE and 0x1022635E back. install now keeps the file in tree/source.json
(`given_drop`) and verify_all.py hands it to regen_clean.sh as XI_DROP. r14's `given_drop` was rebuilt from
tree/left_out.json (each entry `name: why`, hx2clean.py's form for a given reason). install also gives an export entry
without `raw_gate` the default, as order() reads it. Not yet run: verify_all on a machine with the DLL and VC6.

## 28 Oct: two hand-found rules in the sweep (#163)

`pipeline/vc6/sweep.py` RULES gains two rules from the hand matches, each kept only where match.py's comparison says
IDENTICAL (`--calls --rules`, the whole tree r14, 4,664 functions):
- `globvar` (recompile/d3d_round_118.md rule 1): a global the sweep reads through its cast address,
  `(*(int *)0x1047BFA8u)`, declared as a variable `extern int xi_g_1047BFA8;` (the name carries the address, so
  match.py still compares it). Applied to 1,390 DIFFERS: IDENTICAL in 72 alone, 11 more with ret8 / cmpk.
- `devlocal` (d3d_round_119.md rule 8): a call through an object an integer global points to, the global read once
  into a block-scoped local; only where the statement holds no other call and no assignment. Applied to 24, IDENTICAL
  in 1 (0x1006AF00, with globvar: the function the rule was found on, now from the tree's own source).

verification/vc6_sweep.json: 2,071 functions at the game's bytes as written or by a rule (was 1,819 of 4,251 on
r13; 84 by the two new rules); none at the bytes before is lost. frame_set_status: 1,841 of the frame set (33.9%).

Two more rules, the same way:
- `frame` (d3d_round_119.md rule 12): the decompiler's `xi_frame` block as plain locals, each where its reference was
  declared; only where nothing else names the block and no two locals overlap. Applied to 385, IDENTICAL in 1.
- `float` (d3d_round_119.md rule 11): in the body only, `double` as `float` with `f` literals, or the literals
  alone. Applied to 368, IDENTICAL in 4, and 1 more with globvar.
The block and the double type are not what keeps most of those functions from the game's bytes. vc6_sweep.json: 2,077
at the game's bytes, none lost; frame set 1,844 (34.0%).

**ret8 / ret16 on C-linkage functions.** The rules' return-type rewrite looked for the type at the start of a line;
a function defined `extern "C" undefined4 __cdecl F(...)` (the sweep's form for a C-linkage function) never got it,
so the rule was never tried there. Now it is, and the low piece of `V & 0xffffff00` (the decompiler's cleared byte)
is read as 0. 26 more functions IDENTICAL (ret8 25, ret8_globvar 1): 2,103 at the game's bytes, none lost.
pipeline/vc6/differs.py skips the shim's helper functions in an object, as sweep.py does (it stopped on them).
Found by grouping the DIFFERS functions by their few-instruction difference (differs.py): `mov al, 1` in the game
where ours has `and al, ..; or al, ..` in 66 functions.

**Leads from differs.py, not rules (28 Oct).** Grouping the DIFFERS functions no rule matches by their small
difference (at most 4 instructions, 630 functions):
- 28 this-adjusting thunks: the game's `sub ecx, 0x54; jmp`, ours `add ecx, -0x54` with the argument reloaded. A
  `char` parameter passed on as `int` removes the reload (0x10042690); `(_DWORD *)this - 21`, `(char *)this - 0x54`,
  `(int)this - 0x54` and `(unsigned int)this - 0x54` all give `add ecx, -0x54`. The game's form is that of the
  compiler's own adjustor thunks (multiple inheritance); whether these are such is not established.
- 67 where the game has one `mov` more; about 40 of them the game setting ECX just before a call. The callees there
  (data/ecx_use.json): 42 read ECX, 24 do not, 7 not recorded. No single source form; not a rule.
- 0x10002C80 and others with a 0x40C-byte `xi_frame` block: the `frame` rule gives the game's frame size; what is left
  is the order of a call's argument loads (rule 14, for reorder.py).

**28 Oct, #118/#119: sources the sweep could not compile.** Conversions added to pipeline/vc6/sweep.py (each a
spelling VC6 takes for what the decompiled code says; a function counts only where match.py says IDENTICAL):
- a __thiscall call through a slot of one object's vtable on another object, and one through a function pointer
  read from memory: a call through a member-function pointer loaded from the slot (`mov ecx, X; call [slot]`);
- a field or local declared as a pointer to __thiscall functions (C4234): the keyword dropped (a layout only);
- RECT, FILE, MSG, SYSTEMTIME with their layouts from VC6's WINDEF.H, STDIO.H, WINUSER.H, WINBASE.H;
- rule `frametight`: the decompiler's frame block kept whole (a vertex array passed by address stays one array) but
  cut to the span its locals cover. 0x1000A3B0: frame 0x49C to 0x88, the game's 0x90; the rest is statement order.
NO_COMPILE 91 to 80; none of the eleven is IDENTICAL yet; 2,103 at the game's bytes, none lost. Left: 0x10011A00
uses `_mm_pause`, which VC6 lacks (only inline assembly would give `pause`); 0x1000A6E0, 0x10010D20, 0x10086210 fail
on a conversion inside a statement spanning lines; 0x10025260 on a vtable member the shim's class lacks.
