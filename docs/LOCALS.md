# Local variables: what their instructions say (4 Oct 2026)

Hex-Rays declares every local with a type it picks itself. Where it cannot tell, it uses a placeholder
(`_DWORD *`, `_BYTE[64]`) or a plain `int` that the code then reads through casts (`*(float *)&v7`). This round
checks those types against the machine code, using the map from each variable to where it lives, which the owner
exported with `ida/ida_lvars.py` (`data/exports/gfx_lvars.jsonl`).

## Inputs and what was not used

- **867 records**: 839 decompile to exactly the export's text (same sha1), so their variable names are the
  export's. **28 do not**: the IDB changed between the export and the lvars run, and their v-numbers differ.
  They are listed in `data/local_types.json` and were not used.
- **Where a stack variable is**: IDA's frame offset. The address is entry ESP + `stk_ida` − frsize − frregs.
  The formula was checked on the export: in all 464 usable functions with stack arguments, the lowest argument
  lands at entry ESP + 4.
- **Which instructions touch it**: every instruction in the function whose stack operand overlaps the variable,
  not only the ones Hex-Rays attributes uses to. Each operand's address comes from the exact stack depth at that
  instruction:
  - from `sp_solve.insn_depths`, where every pop on the way comes from a definitive source;
  - otherwise from depths observed running the original (`data/real_sp.json`, paths where every earlier call
    was real game code).

  The two sources agree at every address where both exist. Not counted:
  - observed depths in a function that aligns ESP (`and esp, -N`), because the run's padding is in them and
    IDA's frame is not;
  - an instruction with neither source. This leaves 60,270 of 83,153 instructions with a known depth.
- **Functions not used for stack variables**: one function (0x100412B0) where IDA's SP differs from the exact
  depth. There, Hex-Rays' variables are not the slots they claim to be.
- **Register variables**: only the instructions Hex-Rays attributes their uses to, and only where the register
  actually appears. Their live ranges are not in the map, so their accesses are not all known and none is
  decided.

## Decision rule

A variable gets a new type only when its instructions leave exactly one. That happens in one case. A 4- or
8-byte integer (`int`, `unsigned int`, `_DWORD`, `__int64`) becomes `float` or `double` when both of these hold:

- every instruction that touches any of its bytes is either:
  - an x87 float operation of its whole width, or
  - a whole-width copy (`mov`/`push`/`pop`);
- at least one of those instructions is an x87 float operation.

Anything else is left alone, with the reason:

| Verdict | Count | Why it is not decided |
|---|---|---|
| `register` | 5,735 | Register variable; its accesses are not all known |
| `not_integer` | 1,644 | Already a float, pointer or array; listed with its accesses only |
| `blind` | 883 | Some stack operand in the function has no known depth, so it could be this variable |
| `no_float` | 822 | Integer accesses only: the declared integer stands. A sign is not decided here (copies do not show one) |
| `address_taken` | 44 | Its address, or an overlapping variable's, is taken, so accesses through it are not seen |
| `mixed` | 10 | Both integer and float accesses |
| `sp_differs` | 4 | In 0x100412B0 (above) |
| **`decided`** | **29** | **Integer whose bytes are only ever handled as a float** |

## The 29 decided

- **18 are locals.** Each was rewritten in a copy of the export:
  - the declaration becomes `float`;
  - each `*(float *)&v` becomes `v`;
  - each other use becomes `*(int *)&v`. Those are the same bytes read as the declared integer, so the
    behaviour is unchanged by construction.

  ```cpp
  // before                                         // after
  int v7;                                           float v7;
  *(float *)&v7 = *(float *)&a2 + 16.0;             v7 = *(float *)&a2 + 16.0;
  return Gfx_DrawScreenQuad256(a1, v7);             return Gfx_DrawScreenQuad256(a1, *(int *)&v7);
  ```

  The 13 functions they are in were translated again and gated on fuzz plus the four dumps. The verdicts are
  identical to the untyped translation: 11 PASS, plus 2 that already FAIL for other reasons (0x10056850,
  0x100600F0). This is `runs/r11t`, a test translation; the verified tree is still built from the export as
  Hex-Rays printed it.
- **11 are arguments**: `Gfx_DrawScreenQuad256` a1/a2, `CYyIcon_Draw` a3, `CXiMovie_Draw` a3,
  `Mem_DrawHeapUsageBar` a2/a4, `CXiSkeletonActor_GetElem` a2, `CXiSkeletonActor_GetElemLocal` a3,
  `Cell_DrawStretchedWrapped` a6, `sub_10056850` a4, `Tex_DrawNamedTexturePreview` a2.
  - The callee reads each one only as a float, so its prototype says `int` where the code means `float`.
  - They are not rewritten, because an argument's type is the prototype and its callers see it.
  - The fix belongs in the IDB, where Hex-Rays then prints the callers right as well. The example above shows
    why: `Gfx_DrawScreenQuad256(a1, *(int *)&v7)` is a float passed through an `int` parameter.

## The placeholder types (`_DWORD *`, `_BYTE[N]`, ...)

None of the 785 placeholder-typed locals is decided. The instructions do not pick a type for them:

| Shape | Accesses seen through it | Count |
|---|---|---|
| Pointer (`_DWORD *`, `_BYTE *`, `_WORD *`, `_DWORD **`) | None seen (register variables whose attributed instruction is not the dereference) | 390 |
| | Whole-width copies only | 183 |
| | Integer instructions only | 36 |
| | Floats at some offsets and copies at others: a structure, not a `float *` | 6 |
| | Floats and integers | 6 |
| Array (`_DWORD[16]`, `_BYTE[64]`, ...) | Copies only, or address taken and filled by a callee | 131 |
| | Other | 9 |
| Function pointer through an object (`int (__thiscall ***)(_DWORD)`) | Various | 24 |

Within its own function, a `_DWORD *` is usually only loaded, stored and passed on, so "a pointer to 4-byte
values" is all those instructions say. Its real type is fixed by where the pointer comes from or goes, in
another function. Deciding it needs that interprocedural step: callee parameters read by the callee's own
instructions, and producers' return values. It is not attempted here.

## Files

- `pipeline/local_evidence.py`: every access of every variable, from the instructions
  (`runs/r11/local_evidence.json`).
- `pipeline/local_types.py`: the verdicts (`data/local_types.json`, re-derived by `verify_all.py`) and the
  typed copy of the export.
- `ida/ida_lvars.py`: the IDA script that writes `gfx_lvars.jsonl`.
