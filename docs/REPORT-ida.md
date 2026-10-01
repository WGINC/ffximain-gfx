# FFXiMain.dll (2026-09-03): IDA cross-check and IDB application (30 Sep 2026)

This continues `ffximain-verified-recon` and `ffximain-cpp-tree`. IDA Pro 9.4, with Hex-Rays, was
driven live through the IDAssistMCP plugin against `FFXiMain_unpacked.dll` (MD5 `f6dbabef…904d`,
matching the grade-A value). IDA supplied the first **independent** analysis of the pipeline's
function discovery and calling conventions, and the pipeline's results were then written into an
IDB.

No game bytes left the machine. Only addresses, names, layouts and conventions went through the
exchange folder.

## 1. Where things are on the Windows machine (`C:\Users\wadeglover\Downloads\`)

| File | State |
|---|---|
| `FFXiMain_recon.i64` | **The working IDB.** Every change in §3 is applied and saved. |
| `FFXiMain_unpacked.dll_20260930180806.i64` | IDA snapshot taken before any change. Pristine. |
| `FFXiMain_unpacked.dll.i64` | The original. Its packed file was never saved, so it is pristine. It may have leftover unpacked files from a killed session: answer **No** if IDA offers to restore. |
| `ffxi_ida_exchange\` | Scripts, the plan, IDA exports and run logs (also in this bundle). |

## 2. Cross-check: the pipeline against IDA's own analysis

### 2.1 Function discovery

Before any change, IDA had 19,307 functions and the pipeline 21,628. **18,723 starts agreed.**
Every disagreement family was sampled in IDA's disassembly and given a verdict:

| Family | Count | Verdict |
|---|---|---|
| Pipeline start, no IDA function, code head, referenced from data (vtables, tables) | 574 | **IDA missed them.** Example: `0x10002760`, a textbook scalar deleting destructor. |
| Same, no reference at all (found by the pipeline's gap scan) | 1,179 | IDA decoded the code but made no function. Accepted as functions. |
| Same, referenced from code | 67 | IDA missed them. |
| Pipeline start inside an IDA function body, after `ret` + padding | ~266 | **IDA merged them.** It folds a tail-jumped-to function into the jumper. Examples: `0x10030DA0` inside `0x10030C60`; `0x1019CFF0`, a vtable target. |
| Pipeline start inside an IDA *library* function | ~460 | **FLIRT false positives.** IDA applied "VisualC 2-14/net runtime" signatures to a VC6 binary. `unknown_libname_82` spans 3.4 KB of game code (`0x100D9900–0x100DA68C`). |
| Tiny IDA library "functions" inside a pipeline function (`mov al,1; ret` named `std::codecvt_base::do_always_noconv_N`, …) | 80 | **FLIRT false positives.** They split real functions; `0x100600B0` is one of the report's 47 `add ecx,-N / … / mov al,1` adjustors. |
| Pipeline start is an IDA tail chunk (EH unwind funclets, shared tails) | 350 | Both readings defensible. IDA's reading was kept. |
| Pipeline "function" is really a switch jump table | **13** | **Pipeline bug** (`discover.py`), see §4. |
| IDA function that the pipeline starts *inside* (D3DX, CRT, import stubs) | 17 | **IDA right.** Examples: `_D3DXFillVolumeTexture@12`, `_signal`, `__open_osfhandle`. |
| IDA-only `jmp` stub + target pairs (`0x10010250` → `0x10010260`), import thunks, `nullsub` after a noreturn call | ~390 | Conventions only. Left as IDA had them. |

**After the apply (§3):** IDA has 21,410 functions. **20,906 starts agree** with the pipeline
(was 18,723) and **20,316 have identical total size** (was 18,050).

Of the 277 planned functions not present as functions:
- 186 are EH funclets that IDA's re-analysis re-attached to their parents;
- most of the other 91 are CRT hand-written assembly with entry points only bytes apart.

### 2.2 Stack cleanup: now independently confirmed

IDA's own purged-bytes value (from `ret N`) agrees with `prototypes.json` on **every one of the
17,441** functions both tools bound identically.

### 2.3 ECX and the calling convention: five faults in `cc.py`

On functions where the two tools disagree about whether ECX carries `this`, a path-sensitive ECX
read-before-write check was run inside IDA, and each family was sampled by hand.

| # | Fault | Evidence | Size |
|---|---|---|---|
| 1 | A `call` to a thiscall callee while ECX still holds the incoming value isn't counted as a read (pass-through `this`). | `0x1013A9B0`, `0x10014350` | ~305 |
| 2 | Tail-wrapper inheritance overrides the wrapper's own evidence. A wrapper that loads ECX itself (`mov ecx, offset g; jmp f`) is not thiscall; one that does `mov esi, ecx` is not cdecl. | `0x100088D0`, `0x10151100` | ~185 + some |
| 3 | A prologue `push ecx` (MSVC's 4-byte stack reservation) is counted as a read. | `0x100014A0`, `0x10034B80` | ~60 |
| 4 | `__alloca_probe` / `__EH_prolog` preserve ECX, but a call to them is treated as clobbering it. | `0x101B1D00`, `0x102DF3E0` | not counted |
| 5 | ESP tracking is lost after `__alloca_probe`, so locals are counted as stack arguments. | `0x10004060` gets **199,558** arguments; `0x100669D0` gets 24,673 | 60 entries above 16 args |

- **Fault 5 hangs IDA.** Parsing a 199,558-argument declaration never finishes. The toolkit's
  `apply_functions_ida.py` contains that entry, so running it hangs IDA. Do not use it; use §3's
  scripts.
- **The gate-verified code is unaffected.** The gate compares ECX at every call, so these faults
  touch only prototypes.
- IDA is wrong too: its `__fastcall` guesses on functions that never touch EDX (`0x10018B00`),
  and FLIRT-imposed `__thiscall` on `mov al,1; ret`.

### 2.4 DancingMad corrections: confirmed by IDA

- The only code storing vtable `0x1032AD20` is `0x10041032`, inside `0x10041020`.
- Slot 6 of `0x1032B3B8` is `0x10052ED0`.
- Slot 6 of `0x1032B2F8` is `0x10052DF0`.
- `0x10040F50` is not an instruction head.

## 3. What was written into `FFXiMain_recon.i64`

| Phase | Result |
|---|---|
| 1. DancingMad corrections | Stale label at `0x10052E00` removed. `CMoD3mSpecularElem_Ctor` → `0x10041020`, `CXi471011C8_VT_1032B3B8_Dtor` → `0x10052ED0`, new `CMoVuDist_ScalarDeletingDtor` at `0x10052DF0`. |
| 2. Remove bad IDA functions | 163 wrong merges and 80 tiny FLIRT fragments deleted (fragment labels cleared). IDA's own reading kept in the 17 + 350 cases above. The only function comments affected were 6 FLIRT tags. |
| 3. Create functions | 2,581 created with the pipeline's bounds, 32 tail chunks attached. 24 failed and 16 weren't code; both are listed in `apply_log_p1to5.json`. |
| 4. Extend fragment owners | 74 functions extended to the pipeline's end. |
| 5. Class structs | **242 structs** (pack 1, base class as first member). **All 242 sizes as computed by IDA match** `sizeof` from the binary's descriptors. The 3 existing user types were not touched; clashing tree classes were added as `CMoProcessor_recon`, `CXiSkeletonActor_recon` and `CXiSkeletonActorRes_recon`. Each struct's comment names its header. |
| 6. Prototypes for new functions | **2,107 typed.** 89 conventions were corrected by the ECX check: 49 cdecl→thiscall, 29 stdcall→thiscall, 10 thiscall→cdecl, 1 thiscall→stdcall. Class members get `Class *this`. 4 skipped for impossible argument counts; 55 noreturn and 13 ambiguous left untyped. **No existing IDA type was changed.** |
| 7. Names and comments | **480 dummy or FLIRT-false-positive names** replaced with reconstruction names (e.g. `sub_100013A0` → `CApp_ScalarDeletingDtor`). **All 4,254 gate-verified reconstructions are marked `[recon]`:** 4,129 as function comments, 125 as address comments where IDA doesn't treat the address as a function start. |

Hex-Rays check: new function `0x100252D0` now decompiles as
`CYyModelDt_VT_1032A430_ScalarDeletingDtor_2(CYyModelDt_VT_1032A430 *this, int a1)`, with its
provenance comment.

## 4. Fixes needed in the pipeline (not applied: the DLL isn't in this workspace to re-run them)

1. **`discover.py`: switch tables mistaken for functions.** 13 tables were not recognised and the
   gap scan seeded functions in them: `0x1003C670`, `0x1003C6B0`, `0x1005DE50`, `0x100AF4A0`,
   `0x100B1090`, `0x100BC960/70/80/90`, `0x100D6740`, `0x10205A40`, `0x1021ACE0`, `0x102A4F10`.
   One swallowed a real function, `0x1003C6C0`.
   - **Fix:** reject a gap-scan candidate whose first two dwords are `.text` addresses covered by
     the private relocation table.
   - None of the 13 is in the tree or the verified corpus.
2. **`cc.py`: faults 1–5 in §2.3.**
   - Count a call to a thiscall callee as an ECX use.
   - Apply tail-wrapper inheritance only when the wrapper neither reads nor writes ECX.
   - Ignore a first-instruction `push ecx` unless the slot is read before it is written.
   - Whitelist ECX-preserving helpers.
   - After `call __alloca_probe`, treat ESP as `ESP − EAX`, or mark the arity unknown.
   - Cap arity at 16 and mark anything above it ambiguous.
3. **Don't run `data/apply_functions_ida.py`** until it is regenerated from a fixed `cc.py`.

## 5. Reliability ratings, updated (Part 1 of the methods guide)

| # | Method | Was | Now | Why |
|---|---|---|---|---|
| — | Function discovery (`discover.py`) | not rated | **B** | 20,906 of 21,628 starts and 20,316 sizes agree with IDA after adjudication. One known bug family (13 switch tables). |
| — | Stack cleanup / arity from `ret N` (`cc.py`) | not rated | **A** | 17,441 of 17,441 agree with IDA's independent purge analysis. |
| — | ECX / thiscall decision (`cc.py`) | not rated | **C** | About 2.6% wrong from four identified faults. Gate-verified bodies unaffected. |
| — | cdecl arity (`cc.py`) | not rated | **D** above 16 args | ESP lost after `__alloca_probe`. |
| 14 | IDA scripts | U | `apply_recon_ida_v2.py` + `run_phase67_v2.py`: **B**. `apply_functions_ida.py`: **D** (hangs IDA). | The new scripts ran in IDA 9.4 and their result was re-exported and compared. The IDB already had 227 `_vtbl` names when this session started, consistent with `apply_classes_ida.py` having been run earlier, but that run was not observed. |
| — | DancingMad corrections | B | **A** | Confirmed against IDA's own cross-references and vtable reads. |

Still unverified: whether names placed "by address layout" (`kind: off`) belong to the class they
sit in, and anything needing a second build.

## 6. Operational notes for the next session

- **Two scripts hung IDA** before the cause was found (fault 5). In both cases the database was
  closed without saving and the work was redone from the snapshot and then a checkpoint.
- **Run long IDB changes as short, logged calls,** or as File → Script file with a progress file.
  The progress file pinned the hang to one function and one call.
- **File-sync caveat.** A pushed file once arrived stale on the device. Push under a new filename
  and verify its MD5 on the device before executing it.
- **IDA 9.4 renamed `PT_SILENT` to `PT_SIL`** in `ida_typeinf`.

## 7. Next steps, by value

1. Fix `cc.py` and `discover.py` (§4), re-run them against the DLL, and regenerate `prototypes.json` and the IDA script.
2. Use Hex-Rays microcode as the front end of a branch-capable lifter feeding the same gate. That is the route past the current 20% coverage.
3. Adjudicate the 21 larger IDA library functions that sit inside pipeline functions (`_DllMain@12`, `___setargv`, `__fcloseall`, …). Either IDA's FLIRT or the pipeline's merge is wrong there, and neither was changed.
4. A second build (2026-08-22) for anchor carrying, still the main open test.
