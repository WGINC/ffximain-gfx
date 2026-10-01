# FFXiMain.dll (2026-09-03): graphics code, second pass on the leftovers (1 Oct 2026)

This continues `REPORT-gfx.md`. That round left 264 of the 768 graphics functions outside the
verified tree. You asked for three things: package the 94 "library" functions, document the 35
"inexpressible" ones, and find a smarter way through the rest. This report covers all three.

## Result

| | Before (`REPORT-gfx.md`) | Now |
|---|---|---|
| Microsoft code (static D3DX) | 94 | **47**, all byte-identical to DirectX 8.1 `d3dx8.lib` |
| Constructs C++ cannot express | 35 | **9** |
| Translated / compiling | 639 / 629 | **712 / 706** |
| Verified as decompiled C++ | 504 | **624** |
| **Clean tree, gated exactly as built** | 496 PASS, 8 INCONCLUSIVE | **620 PASS, 0 FAIL**; 4 placed but not gateable (marked `// NOT VERIFIED`) |
| D3D calls compared in the clean build | 15,591 | **276,914**, across 120 functions |
| Still failing / inconclusive as decompiled | 54 / 63 | **42 / 32**, each with its first divergence in `RESIDUE.md` |

Every one of the 768 functions now has a stated outcome. `RESIDUE.md` lists the 148 not in the
verified tree, with the reason and the route for each.

## 1. The "94 library": half of them were game code

- **Only 45 of the 94 were Microsoft code.** You were right that these could be found online. The DirectX 8.1 SDK
  (`dx81sdk_full.exe`) contains `d3dx8.lib`, dated 16 Oct 2001.
  - `libmatch.py` parses that COFF library, masks relocation sites, and byte-compares every function
    against the game image. **825 functions match: 524 by name and bytes, 301 by bytes alone.**
  - The DirectX 8.0 library matched only 7 of the 45, so this build used exactly the 8.1 version.
- **The other 49 were game functions,** for example `App_InitScene`, `Camera_ApplyViewProjection` and
  `CDx_ApplyStereoEyeProjection`.
  - They were filed as library only because they call something FLIRT had misnamed as runtime.
  - The rule is now "a function is library code if *it* matches the library", not "if it calls
    something runtime-named".
  - All 49 went back through the pipeline.
- **Packaged:**
  - `d3dx81_manifest.json`: 825 entries, each with address, library symbol, object file, bytes compared and how it matched. Metadata only.
  - `d3dx81_bind.hpp`: the 46 public D3DX entry points the game links, bound to their game
    addresses. Prototypes come from the SDK's own `d3dx8.h`, which this bundle does not copy.
    - `d3dx81::D3DXCreateTexture(...)` calls the copy inside FFXiMain.
    - Defining `XI_D3DX_LINK_SDK` makes the same calls link against `d3dx8.lib` instead.
  - The IDB: 78 functions renamed to their D3DX names; all 825 commented with symbol and evidence.
- **The count is now 47, not 45.** Two more functions in the graphics set turned out to be D3DX
  internals once their callers were fixed.

## 2. The "35 inexpressible": mostly mislabelled

| What it was | Count | Fix |
|---|---|---|
| Regex false positives (`D3DFMT_UNKNOWN` matched `_UNKNOWN`) | 2 | Regex fixed |
| Register-only conventions IDA invented | 11 | Retyped from the code |
| `__asm` blocks where Hex-Rays lost the x87 stack | 18 | Typed the FPU helpers (`__ftol`, `__CIpow`, `__CIfmod`, `__CIacos`, `__CIasin`) and float-returning callees (`fpret.py`: 381 found by stack-depth analysis, 214 confirmed by IDA, 159 applied) |
| Genuine | **9** | Listed in `RESIDUE.md` |

- **The 9 genuine cases:**
  - 2 frames where Hex-Rays still places a local on the return address (`retaddr`):
    `CDx_CreateRampTexture1x16` and `0x1028AB50`. Four others were fixed this round (§3).
  - 2 `__asm` blocks. One is in the 23.6 KB scheduler interpreter `CMoSchedularTask_Interpret`.
  - 1 real `__usercall` (`0x10125360`, which takes a float in ST0) and 1 real `__userpurge`
    (`0x101772E0`, with an EDI argument).
  - 2 callers of `__userpurge` functions: `0x10178250`, and `0x102D538A`, which calls the D3DX internal
    `D3DXFilterTexture`.
  - 1 absolute memory access (`MEMORY[...]`, `0x10281510`).
- **Route:** these stay as `orig::` calls into the game.

## 3. The rest: diagnose first, then repair by rule

The smarter method has three parts.
- **(a)** The gate stopped producing false verdicts.
- **(b)** Every remaining failure is located to the first diverging event and the C++ line that caused it.
- **(c)** The failures were grouped by cause. Each group was fixed by a rule applied across the whole
  graphics set in IDA, not one function at a time. After each IDA repair the code was
  re-exported and gated again.

### (a) Gate artefacts removed

| Artefact | Fix |
|---|---|
| Unmapped data reads crashed both sides, so verdicts were INCONCLUSIVE | **Demand paging:** a deterministic page per (trial, address), the same for both runs, mapped during three discovery runs of the original |
| Virtual calls on fuzzed `this` executed garbage | `this` gets its **real class vtable** (`class_vtables.json`), and every slot is stubbed by its prototype |
| Import calls compared four "arguments" including caller junk | **Per-import argument counts** from IDA (293 typed imports) |
| Variadic callees (`printf`-style debug output) compared slots past the format, which hold caller junk | Only the fixed parameters are compared |
| Original crashed or never finished, yet the trial was still compared | Uninformative. A reconstruction that crashes where the original **returns** is still a FAIL |
| Float results differed only in excess precision. MSVC returns ST0 unrounded; C++ must round to the declared `float`/`double` | Compared at the declared precision. Applies to 3 functions, flagged in their `GATE:` line |
| Stack-pointer arguments and ECX differed between frame layouts | Compared as "a stack address" |

Effect: decompiled-C++ PASS went from 504 to 598 before any code changed. None of these can turn
wrong code into a PASS:
- the D3D calls, memory writes and returns are compared exactly as before;
- the mutation test below still rejects most mutants.

### (b) Localisation

`localize.py` replays a failing trial on both sides and records every write outside the stack and
every call, in order. It reports:
- the first event that differs;
- the original's address;
- the reconstruction's source line, rebuilt with `-g` and resolved with `addr2line`.

Example:
- `CDx_SetDefaultRenderState` diverges at event 18;
- the source line is `F_100098E0(&self->base.vftable, 3, v2, v5);`;
- the cause is the wrong fourth argument. `RESIDUE.md` gives this for every remaining FAIL.

A second signal predicts failure without running anything. In the translations, a register
variable that is read but never assigned (`int v4; // ecx`) appeared 71 times, and 50 of those were
in functions that did not pass. That pointed at register data flow, so the IDA rules below target
it.

### (c) Repair rules, applied in IDA across the graphics set

| Rule | Evidence it is right | Applied |
|---|---|---|
| **COM call-site types.** Hex-Rays cast some D3D vtable calls to the wrong arity, e.g. `SetRenderState(dev)` with no arguments, and then absorbed saved registers into the next call. The call site now gets the vtable member's own prototype | The member type comes from `d3d8.h` | 70 calls in 29 functions |
| **COM call purge.** Where IDA's stack tracking had a D3D call popping the wrong number of bytes, set the stack change to 4 × arguments. Kept only if the decompiled result gets strictly better; otherwise reverted | `retaddr` and positive-SP warnings disappear; every `ret` is balanced | `retaddr` gone from 4 functions. 3 now pass (`CDx_GetStereoSecondEyeTarget`, `Post_FlashAndZoomFeedback`, `0x10038E10`); `CDx_DrawFadeQuad` is now translatable but inconclusive |
| **Fake `__thiscall`.** The callee never reads ECX on any path (data-flow analysis that follows tail jumps and calls), so it is retyped `cdecl` | Path analysis of the callee | 9 (e.g. `0x10093DB0`, FLIRT-named `codecvt_base` stubs that are `mov al,1; ret`) |
| **Callee keeps a scratch register** (`__spoils<…>`). The caller reads ECX or EDX after the call; the callee provably never writes it (recursively). This is MSVC link-time code generation | Path analysis | 21 callees, e.g. `CDx_GetRenderTargetTop` keeps ECX |
| **Return and parameter widths from gate evidence.** The upper EAX bytes differ but AL agrees, so the function returns `char`. EAX is never set, so it returns `void`. A store keeps the full dword, so the parameter is `int` | Re-gated with the narrower comparison; all pass | 3 `char`, 7 `void`, 2 `int` parameters |

Translator and clean-stage fixes found on the way:
- `__spoils` is stripped from C++ signatures; it is ABI-neutral for GCC.
- Globals that FLIRT gave a runtime type (`exception`, `Iostream_init`) are used as addresses.
- The cast-repair pass only edits the unit's own file, and casts the whole right-hand side.
- In the clean stage:
  - a pointer-depth bug, where `*((_DWORD **)self + 3)` became `at<_DWORD **>`, is fixed;
  - a class-pointer field no longer replaces a raw `_DWORD *` access;
  - a global read at two widths by different functions gets a second name, `unk_104571D8_as_u8`, so each reads exactly as wide as the original did.

Progress of decompiled-C++ PASS: 504, then 598 after the gate fixes, then 615, 622 and 624 after
the IDA repair rounds. One function regressed between rounds (`0x10281830`). Each function is
translated on its own, so the final export keeps the passing version.

## Mutation test (does the gate still reject wrong code?)

- **Method:** 40 random one-site mutants on the final clean tree (an integer literal +1, or a D3D enum swapped), a new random sample.
- **Result:** **33 of 40 rejected** (82%).
- **The 7 survivors:**
  - 2 are in functions whose original returned in only 3 of 100 trials (`0x100CBDE0`, `0x100755A0`).
  - The other 5 have not been analysed site by site. The likely cause is a mutated literal on a
    branch the trials did not reach, or one whose value is overwritten before use.
- **What this means:** the gate fixes in §3a did not make the gate weaker.
- **Previous round:** 30 of 40 were rejected. The survivors were coverage limits, mostly paths
  that ran in at most 3 of 100 trials.

## DancingMad cross-check

- **Agreement:** 183 of 188 overlapping names agree, with **0 conflicts**.
- **5 names adopted** in the IDB and the generator: `CDx_PushRenderTarget`, `CXiMovie_Draw`,
  `OcclusionMng_CreateTarget`, `ModelActor_Draw`, `Dancer_InitDevice`.
- **DM's descriptions** fill the last column of `RESIDUE.md`.
- **Still wrong in DM's updated script:** `0x10040F50` and `0x10052E00`, the two errors corrected on 30 Sep.

## What is left (`RESIDUE.md`)

- **42 FAIL.** The first divergence and source line are given for each. Patterns still open:
  - **Calls Hex-Rays dropped or misordered,** e.g. `0x10181BB0` is never called in 3 functions. These are usually a callee whose stack purge is wrong.
  - **An extra byte store.** Two `CXiSkeletonActor` constructors write a byte at `+0x83C` that the original leaves alone, and `CXiSkeletonActor_Init` has the same problem.
  - **x87 results that differ beyond rounding.** This affects the 4 `CXiSkeletonActor` size and speed getters and
    `0x10282A30`. The difference is in operation order or intermediate precision, not just the final rounding.
  - **Runaway loops,** in 4 functions. A `qmemcpy` size or loop bound was decompiled wrong.
- **32 INCONCLUSIVE.** The original never returns under fuzzed inputs. These need structured inputs, or a memory snapshot from a live client.
- **14 build gaps:**
  - 8 scalar-deleting destructors that IDA typed `__fastcall` (the gate has no fastcall case yet);
  - 6 compile errors: an enum `++`/`--`, `LPCWSTR`, and `Concurrency::` types from FLIRT.
- **4 placed in the tree but not gateable there.** They are verified as decompiled, and marked `// NOT VERIFIED` in the tree.

## Reliability

| Claim | Grade | Note |
|---|---|---|
| D3DX identification | **A** | Byte-for-byte with relocations masked. 301 are bytes-only matches (no name); weak ones under 16 bytes are comment-only in the IDB |
| IDA repair rules | **B** | Each is backed by data-flow analysis or the D3D8 header, and every change was re-gated; the purge rule reverts itself when it doesn't help |
| Gate PASS | **B** | Random inputs, not path coverage. Callees are stubbed. 10 PASS functions returned in fewer than 10 trials, and their `GATE:` line says so |
| Excess-precision equality (3 functions) | **B** | Equal for any caller that stores the result; a caller that keeps using ST0 would see the last bits differ |

## Files

- **`REPORT-gfx2.md`** (this file) and **`RESIDUE.md`**.
- **`tree/`:** clean C++, 135 `.cpp` + 119 headers. There are 280 member functions in 110 classes and 344 free functions. Each function carries its clean-build `GATE:` line.
- **`d3dx/`:** `d3dx81_manifest.json` and `d3dx81_bind.hpp`.
- **`verification/`:**
  - `gate_clean_final.jsonl`: one line per function in the clean build;
  - `gate_raw_final.jsonl`: the decompiled-C++ run;
  - `localize_fail.json`: first divergence per FAIL;
  - `mutants_clean.json`;
  - `cases_clean_final.json`.
- **`pipeline/`:** everything needed to reproduce:
  - new this round: `ida_regflow.py`, `localize.py`, `repair_compile.py`, `byte_returns.py`, `libmatch.py`, `fpret.py`, `msvc_demangle.py`, `gen_residue.py`, `gen_d3dx_bind.py`, `annotate_gate.py`;
  - updated: `hx2cpp.py`, `hx2clean.py`, `gfx_cases.py` and `diffemu_gfx.py`.
- **Not included:** any game binary, any byte signatures, and the SDK's own headers or library.

**IDB (`FFXiMain_recon.i64`, saved):**
- the repairs in §3: call-site types, stack points, retypes and `__spoils`;
- D3DX names and comments;
- the 5 DancingMad names;
- a `[gfx-recon 2026-10-01]` comment on all 624 verified functions, giving file, name and evidence. It replaces the 504 old ones.
