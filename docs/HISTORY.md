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
