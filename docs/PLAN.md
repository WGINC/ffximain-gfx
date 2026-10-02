# Plan to completion (1 Oct 2026)

"Complete" means three things:
- Every one of the 768 graphics functions is either verified C++ in `tree/` or a documented
  `orig::` call with a stated reason.
- The tree is organised by source unit and uses real names.
- The whole thing regenerates from the IDB with `run.sh`.

## Where things stand

| | Count |
|---|---|
| Verified in the clean tree as built | 620 (+4 placed but not gateable) |
| Microsoft D3DX 8.1 | 47 |
| Not expressible in C++ | 9 |
| Gate FAIL | 42 |
| Gate INCONCLUSIVE | 32 |
| Build gaps (compile error / no gate case) | 14 |

## The parallel session's tree (`incoming/parallel-organized.zip`)

**What it is.** The parallel session hand-edited the *previous* tree, the v1 one with 504
functions. It did two things:
- **Names:** it replaced 316 placeholders and added named offset constants (`include/gfx_fields.hpp`).
- **Placement:** it moved 56 free functions into the source unit the binary places them in. It used
  asserts, names, constructor vtable stores, and "both neighbours from the same unit within 4 KB".

**Checked.** I built the v1 tree and theirs with `-ffunction-sections` and compared every function
section with relocation sites masked. All 636 v1 sections are present and identical, and the only
extra sections are the 11 empty `.text` sections of the new files. The edits are naming and
placement only, as claimed.

**Good:**
- The evidence is recorded per name (`RENAMES.csv`) and per move (`ORGANIZATION.md`).
- Unresolved items are listed rather than guessed (`UNRESOLVED.csv`, 333 entries).
- The placement rule is the right one for MSVC object order.
- They chose not to use vtable slots as evidence. That is correct, because a subclass vtable contains inherited methods.

**Needs work before it is adopted:**
1. **It is built on v1.** The current tree has 120 more functions and different placement, so the
   edits cannot be copied over file by file. They have to become *data the generator applies*
   (`renames.csv`, a unit map, the field constants), so they survive regeneration and cover the new
   functions.
2. **8 moves into `CTsZoneMap.cpp` are marked "hand: operates on CTsZoneMap"** with no other
   evidence. One of them, `0x101844C0`, is DancingMad's `ModelActor_Draw`, which argues against
   `CTsZoneMap`.
3. **15 names describe a call the function makes, not what it means**
   (`Dev_SetRenderState_10086210`). Treat them as provisional.
4. **`0x10184C90` was moved,** but in the current pipeline it is `__asm` and not in the tree.
5. **The IDB was not updated** (e.g. `g_pCurrentCamera` -> `g_pXiZone`). The IDB should stay the
   source of truth.

## Progress, 1 Oct 2026 (second pass)

- **Tasks 2–4 are done.** The parallel session's names were reviewed: 295 applied in the IDB and 22 kept
  as comments. Placement was re-derived from evidence for all functions (`docs/PLACEMENT.md`,
  `docs/REVIEW-parallel.md`). Free functions went from 344 to 156, and members from 280 to 477.
- **Part of task 7 is done:**
  - 8 destructors IDA typed `__fastcall` were retyped `__thiscall`; their code reads only ECX. All 8 pass.
  - 16 FLIRT false positives (VS2010 `Concurrency::` names in a 2002 binary) were removed from the IDB.
  - D3D enums can now be stepped with `++`/`--`.
  - All 633 tree functions can be gated.
- **Tree:** 633 functions, all PASS in the clean build.

## Progress, 2 Oct 2026

- **Placeholders in the tree:** 193 more were named. The tree went from 24 `sub_` and 283 data placeholders to 13 and 105 (`docs/NAMES-2026-10-02.md`).
- **The IDB has not caught up yet.** The IDA bridge was unreachable, so the names are in `data/names_pending_idb.json`:
  - the generator applies them;
  - `pipeline/ida_apply_names.py` pushes them into the IDB.
  - **Next at the desktop:** run it, re-export, and then empty the file.
- **The generator now gives each address one name across the whole export.** Records exported before a rename no longer leak old placeholders into the tree.
- **The tree is byte-identical,** so the 633 PASS results stand.

## Tasks (also in the task list)

1. **Tidy the repo.** Done: this layout, `run.sh`, repo-relative paths, reproduction checked.
2. **Port the parallel renames and unit placement into the generator.** Extend the placement to
   the new functions, then re-check object identity and re-gate the clean tree.
3. **Review the weak points listed above.** Mark each entry accept, provisional or reject.
4. **Apply the accepted names to the IDB.** Re-export, and confirm the tree picks them up without a
   rename pass.
5. **Repair the 42 FAILs by pattern:**
   - dropped calls (check non-COM callee stack purge);
   - wrong call arguments;
   - an extra byte store in two `CXiSkeletonActor` constructors and `Init`;
   - runaway loops;
   - x87 ordering;
   - crashes where the original returns.
6. **Resolve the 32 INCONCLUSIVE.** Build a layout-aware fuzzer for `this` objects, and/or support a
   memory snapshot from a live client.
7. **Close the build gaps:**
   - 8 `__fastcall`-typed destructors;
   - 6 compile errors;
   - 4 tree functions with no gateable export.
8. **The 9 inexpressible functions:**
   - asm thunks for the real register conventions;
   - fix 2 `retaddr` frames;
   - 2 `__asm` blocks;
   - 1 `MEMORY[]` access;
   - otherwise document them as `orig::`.
9. **Gate strength:**
   - analyse the 5 mutation survivors;
   - improve coverage for the 10 PASS functions with thin evidence;
   - give the fake device state;
   - run a larger mutation sample.
10. **Fields:**
    - name the `field_XXX` members, which also removes many of the 87 `at<>()` accesses;
    - settle the `CMoProcessor` size;
    - feed the names back to the IDB.
11. **Final pass:** full regenerate and full gate with no carried-over verdicts, then report and
    bundle.

**Order:** 2–4 first (cheap, and they change names everywhere else), then 5–8 (coverage), then
9–10 (strength and readability), then 11.

## Notes for running

- **Two cases are slow, not hung.** `CMoCamera_SetPathData` and `CMoGeneratorClone_InitCloneFromSource`
  take about 7 s per trial, because the original itself runs to the instruction limit in most
  trials (about 12 minutes each for 100 trials). A wall-clock cap was tried and removed: Unicorn's
  timeout made every run about 4x slower.
- **The gate resumes into an existing output file.** It skips cases already present there, so use a
  fresh run directory.
