# FFXiMain.dll (2026-09-03): graphics code as clean, gate-verified C++ (1 Oct 2026)

This continues `REPORT-ida.md` and `REPORT-ccfix.md`. The earlier tree covered only branch-free
functions, which is why it stopped at about 20%. This round extends the method to branching
graphics code: Hex-Rays (IDA 9.4) is the front end, and the equivalence gate decides what is kept.

## Result

| | |
|---|---|
| Graphics functions selected | **768**: 236 read device-bearing data, 489 are graphics class members, 80 have D3D-typed signatures (the groups overlap) |
| Microsoft code excluded (static D3DX library, C++ runtime) | 94 |
| Inexpressible in C++ (inline asm, register-only conventions, unknown data) | 35 |
| Translated to C++ | 639; 629 compile |
| Passed the equivalence gate as decompiled C++ (selection) | **504** |
| **Clean tree, gated exactly as built** | **496 PASS**, 8 INCONCLUSIVE, **0 FAIL** |
| D3D calls compared method-by-method with arguments | **15,591** across the 57 verified functions that make D3D calls |
| Clean source | 131 `.cpp` + 118 headers, 9,797 lines; 234 member functions in 109 classes, 270 free functions |

**How strong is a PASS?**
- The median verified function returned normally in 100 of 100 trials.
- 24 PASS functions returned in fewer than 10 trials. Their evidence is thin; each file says so.
- The 8 INCONCLUSIVE functions are in the tree, marked `// NOT VERIFIED`.

Sample (`src/CDx.cpp`):

```cpp
// FUNCTION: FFXIMAIN 0x1000AAB0  (placed by typed this)
// GATE: PASS -- 100 trials, 100 returned normally, 1500 D3D calls compared (diffemu_gfx, clean build)
int CDx::Post_DepthBlurComposite(int a1, int a2)
{
  orig::CDx_SetDefaultRenderState(this);
  this->pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
  this->pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
  this->pDevice->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
  this->pDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
  ...
```

## Method

1. **Find the device.**
   - `g_pD3DHolder` (`0x103D3CBC`): the D3D holder, with `IDirect3D8*` at `+4` and `IDirect3DDevice8*` at `+8`.
   - `g_pCDx` (`0x1045666C`, named this session, AUTHOR-INFERRED): the `CDx` singleton, built in
     `0x100161B0` from the holder's two pointers. Its `+8` and `+C` hold them.
   - `g_pDancerDevice` (`0x1066941C`) and `0x10669418`: copies used by the `sqgx` layer.
2. **Type D3D8 in the IDB.**
   - 12 COM interfaces and 11 enums were generated from MinGW-w64 `d3d8.h`. The method order is the
     vtable layout: 97 device methods, 388-byte vtable.
   - Every offset seen in the code matches, e.g. `Present` +60 and `SetRenderState` +200.
   - Hex-Rays now prints `pDevice->lpVtbl->SetRenderState(pDevice, D3DRS_ZWRITEENABLE, 0)`.
3. **Fix IDA's conventions.**
   - IDA had given 100 graphics functions register-only conventions (`__userpurge` with EBP/EDI
     arguments, which MSVC6 never generates).
   - They were retyped from the fixed `cc.py` (`REPORT-ccfix.md`); 100 of 100 applied.
4. **Translate (`hx2cpp.py`).**
   - Every global becomes an lvalue at its real address, and every callee a typed pointer to the
     original. The reconstruction therefore reads the same memory and calls the same code as the
     function it replaces.
   - String literals map back to the game's own strings.
5. **Gate (`diffemu_gfx.py`), new this round:**
   - **Fake Direct3D 8 objects.** Every method call is logged as interface, method, `this` and
     arguments, then compared. It returns a fuzzed `HRESULT`. Out-parameters that return interfaces
     (`CreateTexture`, `GetSurfaceLevel`, …) receive another fake object, so code that goes on to
     use them is still checked.
   - **Fuzzed stub returns,** with the same sequence replayed for both runs, so both branch
     directions get exercised.
   - **Per-argument width masks** for 8- and 16-bit parameters, and **16-bit return comparison**.
   - **Stack addresses passed as arguments compare as "a stack address".** A rebuilt frame
     legitimately lays out its locals differently.
   - **One fresh emulator per case, and globals restored after each case.**
6. **Clean up (`hx2clean.py`), then gate the result again as built:**
   - **Placement:** functions go into classes by the tree manifest, the IDB name, or a typed
     `this`. Nothing else is guessed.
   - **Members:** each function becomes a real member; `*((float *)self + 38)` becomes
     `this->field_98`. Layouts are inferred from every member of the class, with `static_assert`s
     on every offset and on the descriptor `sizeof`.
   - **Named fields:** your IDB's names are used (`CXiSkeletonActor::moveSpeed`, `color`,
     `boundRadius`; `CMoProcessor::ScratchMatrices[512]` …), but only where the access has the same
     width and kind.
   - **D3D calls** become `pDevice->SetRenderState(...)`, with render-state and texture-stage
     values named from the D3D enums.
   - **Calls into game code** are `orig::Name(...)`; a game function used as a value is
     `XI_CODE(Name)`.

## What the gate caught along the way (all fixed and re-gated)

**In my tooling:**
- 16-bit arguments were compared with upper-half junk included.
- Strings were typed as pointers instead of arrays.
- Objects with no known layout were sized at 256 bytes. The mutation test exposed this: reads past
  the end saw zeros on both sides.

**In the gate:**
- **Batch leaks.** A stub placed inside another case's function body, plus globals that were never
  restored, gave false FAIL/INCONCLUSIVE results. They could never give a false PASS.
- **Effect:** running unbatched recovered 100 functions the batched run had rejected.

**In the clean stage:**
- **`orig::nullsub_44` passed as a callback.** It took the address of my wrapper, not the game's.
- **A named field used at the wrong width.** A 1-byte store became a 4-byte one, at
  `CXiSkeletonActor+0xA04`.

**Genuine decompiler errors (excluded):**
- 54 functions fail as decompiled; 63 are inconclusive.
- `0x101772E0` and `0x10281DB0` passed with one seed and failed with another.

## Mutation test (gate sensitivity on this corpus)

- **Clean tree:** 40 random one-site mutants (an integer literal +1, or a D3D enum swapped).
  **30 were rejected.**
- **The 10 survivors are coverage limits:**
  - 6 are on paths that ran in at most 3 of 100 trials;
  - 2 index past the 16 KB object cap, so both sides read zeros;
  - 1 is probably equivalent (`v8 = 0` overwritten);
  - 1 is an indexed store not fully explained.
- **Earlier, on the raw translation:** 24 of 30 were rejected before the sizing fix.

## Size conflict to resolve

**`CMoProcessor`.** The class model's descriptor-only record says `sizeof 0x10`, but the methods
placed there read up to `+0xB20`, and your IDB struct is `0xBB0`. The header uses `0xBB0` and says
so. Either that descriptor isn't this class's, or the class-model name for it is wrong.

## Reliability

| Method | Grade | Note |
|---|---|---|
| D3D8 interface layout (method order / vtable offsets) | **A** | From `d3d8.h`; every offset seen in the code matches. |
| Device location (`g_pD3DHolder`/`g_pCDx` fields) | **B** | Read from the code that stores them; consistent across 6 users. |
| Hex-Rays → C++, gated per function | **B** | 75% mutant detection on this corpus. Random inputs, not path coverage. Callees are stubbed, so callee semantics aren't covered. |
| Fake-COM checking of D3D calls | **B** | Methods, arguments and order are compared. The fake device has no state: a call's effect on later D3D behaviour isn't modelled. |
| Clean class placement | **C** for "IDB name" / "typed this", **B** for tree manifest | The code is verified; the class it sits in is inferred. |
| Field names | your IDB names where width and kind match; otherwise `field_XXX` | |
| `orig::` call signatures | **C** | Pointer parameters are `void *` and return types come from IDA. ABI-exact, but not semantic types. |

## Files

- `tree/`: the clean C++.
  - `include/` and `src/`, plus `build.sh` (MinGW i686, freestanding, MSVC6 ABI).
  - `placement.json` (why each function sits where it does) and `size_conflicts.json`.
- `verification/`
  - `gate_clean.jsonl`: one line per function; trials, returns, D3D calls compared.
  - `cases_clean.json`, `gate_select.jsonl` (the selection run), `mutants_clean.json`, `mutants_raw.json`.
- `pipeline/`: everything needed to reproduce.
  - IDA side: `ida_gfx.py`, `gen_d3d8_decls.py`, `d3d8_ida.h`.
  - `hx2cpp.py`, `hx2clean.py`, `gfx_cases.py`, `diffemu_gfx.py` (+ `diffemu_fixed.py`),
    `map_clean_cases.py`, `mutate_clean.py`, and the headers.
  - `data/gfx_export3.jsonl`: the Hex-Rays export; decompiled pseudocode, no machine code.

**IDB (`FFXiMain_recon.i64`, saved):**
- D3D8 types; `D3DHolder` and `CDx` structs;
- typed device globals, with `g_pCDx` renamed;
- 16 `CDx *this` types (10 by name, 6 propagated from call sites), 100 convention fixes;
- a `[gfx-recon]` comment on all 504 functions, giving file, class::method and evidence.

## Next

1. **Raise coverage of the passed functions.**
   - 24 have under 10 returning trials.
   - A struct-aware fuzzer that sizes and points `this` fields from the inferred layouts would lift
     both these and the 63 inconclusive ones.
2. **Give the fake device state** (current render states, bound textures), so later calls see
   earlier ones.
3. **Name fields.** Most layouts are `field_XXX`. Behaviour-based names, plus the PS2 DWARF member
   order, are the next readability step.
4. **Settle the `CMoProcessor` size conflict.**
