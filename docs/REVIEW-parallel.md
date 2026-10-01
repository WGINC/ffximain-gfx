# Review of the parallel session's tree (1 Oct 2026)

Source: `incoming/parallel-organized.zip`. It is a hand-edited copy of the v1 tree (504 functions).

## Code identity: confirmed

Both trees were built with `-ffunction-sections` and every function section was compared with
relocation sites masked:
- all 636 sections of the v1 tree are present and identical;
- the only extra sections are the 11 empty `.text` sections of the new files.

## Names (`RENAMES.csv`, 317 entries)

| Decision | Count | What |
|---|---|---|
| Applied to the IDB | 295 | Names with real meaning: PS2 names (`sqgxTextureAdd`, `sqgxPerspective`, ...), DancingMad names (`XiZone_GetAmbient`, `XiZone_GetFog`), zone-renderer names, dancer-module prefixes, `thunk_` names, and the global renames. Every entry gets an IDB comment `[name source: parallel session 2026-10-01] <name> -- <evidence>` |
| Provisional, comment only | 22 | Names that describe a call the function makes, a field read or a vtable reset rather than a meaning (`Dev_SetRenderState_10086210`, `GetField_44_1002B5A0`, `ResetVtbl_1032AA4C`, `GetGlobal_1047D114`). The IDB keeps `sub_`; the suggestion is recorded as a comment |

`g_pCurrentCamera` -> `g_pXiZone` is applied. The code supports it: `0x1018B130` stores the
result of a constructor that writes the `XiZone` vtable.

## Moves (`ORGANIZATION.md`, 56 functions)

These were re-derived from evidence rather than copied (`pipeline/unit_place.py` and the placement
rules in `hx2clean.py`), so they cover the 129 functions new since v1 and survive regeneration.

- **29 agree.**
- **15 differ.**
  - 4 tiny functions sit inside `CYySoundElem`'s code, but `CMoElem`'s vtable points at them. These look like
    identical-code copies, so either answer is defensible. The class model's `CMoElem` membership
    is kept.
  - 6 went to `CMoProcessor` in their tree, but the class model puts them in the unnamed subclasses
    `CMoProcessor_VT_1032C368`/`..._C298`.
  - 2 were placed by names ending `_variant16`/`_variant17`; those names are shared-accessor copies,
    so they are not evidence. Their placement is now rejected.
  - 3 others follow the class model.
- **12 not reproduced by unit evidence alone.** Of these:
  - **The 8 "hand: operates on CTsZoneMap" moves:**
    - `0x10182700`, `0x10184FE0` and `0x101844C0` are now `CTsZoneMap` methods on new evidence: their
      callers load `g_pZoneMap` into ECX, and `g_pZoneMap` is created by the `CTsZoneMap`
      constructor. `0x101844C0` is DancingMad's `ModelActor_Draw`; it is a `CTsZoneMap` method that
      draws model actors, so their move was right and the name stays.
    - `0x10178070`: callers pass their own `CTsZoneMap` `this` (method).
    - `0x10178B00`: called only from `CTsZoneMap` and adjacent to it (unit file).
    - `0x101791F0`, `0x101792A0` and `0x1017ADE0`: no evidence beyond adjacency to zone-renderer code.
      They stay in the unnamed unit file `unit_10178790_ZoneMap.cpp`.
    - `0x10184C90` is `__asm` in the current pipeline and not in the tree.
  - **`sqgxTexture` / `sqgxBlend` moves:** their PS2 names are now in the IDB and anchor the units.

## Field offsets (`include/gfx_fields.hpp`)

Not ported yet. They name raw offsets inside free functions by hand. Porting needs a generator rule
that knows which struct a pointer refers to. That is task "Name fields and settle layouts".
