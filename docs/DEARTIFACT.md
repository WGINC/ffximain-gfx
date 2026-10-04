# De-artifacting plan

Goal: the reconstructed C++ reads like source -- typed objects, fields and methods instead of `*((_DWORD *)x + 7)`,
`(*(int (__thiscall **)(...))(*p + 8))(p)`, `sub_1012AB40`, `dword_104E1B70` -- without giving up one bit of the
gate. Every type or name pushed comes from a definitive source (CLAUDE.md); where none exists the artifact stays
and is counted, never replaced by a plausible guess.

## Where the artifacts are (export 02c4c27f, 768 functions)

| artifact | count | what removes it |
|---|---|---|
| `_DWORD/_BYTE/_WORD *` casts on `this` | 712 | class types (phase 1) |
| ... on locals | 857 | points-to typing (phase 2) |
| ... on arguments | 312 | points-to typing (phase 2) |
| ... on globals | 249 | global typing (phase 2) |
| ... on other expressions (fields of fields, casts of casts) | 917 | field pointee types (phase 3) |
| raw virtual-call casts | 302 | phases 1-3 (the object's type) |
| `LOBYTE/HIWORD/BYTEn/...` | 735 | exact rewrite (phase 4) |
| `sub_XXXXXXXX` uses | 1655 | evidence-only naming (phase 5) |
| `dword_/unk_/off_/byte_/flt_` uses | 1069 | global types + naming (phases 2, 5) |
| `vN`/`aN` uses | 33570 | type-derived names (phase 6, optional) |

Text-level rewriting of the pseudocode is not used: the export has no text-to-address map, so a textual change
cannot be tied to the instruction that proves it. Types and names go into the IDB (ida/ffxi_ida_fix.py), Hex-Rays
re-decompiles, and the re-export is translated and gated like any other. The exceptions are rewrites that are
exact by C semantics alone (phase 4), done in hx2clean and gated.

## Every round

1. pipeline step derives the facts (data/*.json, each entry with its source);
2. make_ida_fix.py embeds them; the owner runs ida/ffxi_ida_fix.py and uploads the export, report and log;
3. translate, fuzz + live gate (rN), compare with the previous round;
4. accepted only if: 0 FAIL, no function goes from PASS to anything else, artifact counts do not rise
   (`pipeline/artifact_count.py`, phase 0); the IDA report's refusals are read and explained.

## Phase 0 -- measure (first)

`pipeline/artifact_count.py EXPORT [TREE]`: the table above per export and per tree, written to
verification/artifacts.json and checked by verify_all (counts recorded per round in HISTORY). Also: the
incremental gate cache (#31) -- every phase below costs a full gate per round; caching verdicts by
(object code, case, dump, harness) hash re-runs only the functions whose code changed.

## Phase 1 -- class types (`this`), in progress

Done: 119 classes with exactly one proven vtable (vcall_resolve), 354 fields at single-width offsets, `this`
typed on 325 methods; vtable structs with slot prototypes from the IDB.
Pending:
- fields inside the IDB's `_pad_` placeholders (script pushed; next IDA run);
- **base classes.** 98 IDB structs hold a `base` struct at offset 0. The base relation is proven by the code
  only when the derived constructor calls the base constructor on the same `this` before storing its own
  vtable (the base vtable stored first, then overwritten). Source: the constructors' instructions, followed
  by vcall_resolve's register flow. Where proven: base field accesses are typed on the base struct, the derived
  vtable struct extends the base's, and `this->base.vftable` gets the derived vtbl type in the derived
  methods. Where not proven, the IDB's `base` stays as it is and is counted;
- the 16 methods the IDB declares non-thiscall although they are methods: listed, left (their calling
  convention is the IDB's declared type; changing it needs a purges/sp_solve proof, already available for
  some).

## Phase 2 -- points-to: types for locals, arguments, globals

A value's type is set only when **every** definition reaching it has that type ("all agree", never majority;
one unknown definition and the variable stays untyped). Sources of a definition's type:
- D3D8 interfaces: the out-parameter of a header-declared creator (`CreateTexture(..., IDirect3DTexture8 **)`)
  and the objects com_calls.py already proves (D3D8 header; accepted declarations stay tagged);
- class objects: `operator new(size)` followed by the class's constructor on the result (the constructor
  stores the proven vtable);
- loads from a field or global whose type was proven earlier (fixpoint with phase 3);
- arguments: a parameter has type T when every call site of the function passes T and the function has no
  unknown caller (address taken, vtable slot, export). Callers from vcall_resolve and the call graph.
Pushed to the IDB as local variable types (ida_hexrays user lvar info, keyed by the variable's location, which
the export carries) and as global/prototype types. Hex-Rays then prints `Texture->lpVtbl->GetSurfaceLevel`
and `obj->field_1C`. Also closes #20 (4 UNRESOLVED need exactly this).

## Phase 3 -- field pointee types

A field `C::field_X` is `T *` when every store into it (across all methods of C and every place a proven C
object is written) stores a proven T. Iterated with phase 2 to a fixpoint. Removes the "other" casts
(`*(_DWORD *)(*(_DWORD *)(this + 4) + 28)`).

## Phase 4 -- partial-register macros, exact rewrite

`LOBYTE(x)` is `(uint8_t)x`, `HIWORD(x)` is `(uint16_t)((uint32_t)x >> 16)`, an assignment
`LOBYTE(x) = y` is `x = (x & ~0xFF) | (uint8_t)y`, and so on -- identities of C on two's-complement 32-bit
values, independent of the program. Done in hx2clean with unit tests per macro; gated. Where the macro hides
an x87 status word or a 64-bit split the construct stays (#23 lists them).

## Phase 5 -- names from evidence only

A name is pushed only when it states a fact:
- class + slot: a function that is slot k of exactly one class's vtable and of no other is `C::vf<k>`;
  a destructor shape is not used (it is a pattern, not a proof);
- a string in the binary naming the function (assert/log text containing the function's name, used only in
  that function): the name the string gives;
- library code identified by IDA's signatures is shown as such but marked "IDA signature" (not proof);
- globals: typed by phase 2 and named after the proven type and address (`g_IDirect3DTexture8_10621904`)
  -- a meaning name (`g_pSkyTexture`) is never invented.
What has no such evidence keeps its address name. Expected: most of the 1655 `sub_` uses stay; the count
of each kind is reported, not hidden.

## Phase 6 -- local names (optional, cosmetic)

Hex-Rays' `vN`/`aN` carry no information to recover; any name is invented. Only type-derived names
(`tex1`, `pModel2` from a proven type) or SDK parameter names (D3D8 prototypes) are allowed. Recommended
only after phases 1-5, and only if the owner wants them.

## Order

0 (counter + gate cache) -> 1 (pads, base classes) -> 2 (D3D locals first: largest and fully header-backed,
then classes, arguments, globals) -> 3 -> 4 -> 5 -> 6. Each phase is one or more IDA rounds; 1-3 interlock
and are run as one fixpoint once the pieces exist.

## What will remain

Anything without a definitive source: offsets accessed at several widths (unions the code does not
disambiguate), pointers whose definitions disagree or are unknown (callbacks, data from files), functions
with no naming evidence. These are listed per function in the residue, as the gate's UNRESOLVED calls are.
