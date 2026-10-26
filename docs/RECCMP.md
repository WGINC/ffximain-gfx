# reccmp against match.py (#114)

[reccmp](https://github.com/isledecomp/reccmp) (the LEGO Island decompilation's recompilation compare, version 0.1.7
from a source checkout) compares a linked binary and its PDB with the original, function by function, from
`// FUNCTION: <module> 0x...` annotations, the same marker form `recompile/` uses. This page says what it did with
our Visual C++ 6.0 output, where it agrees with `pipeline/vc6/match.py`, where not and why, and whether it can replace
or check match.py. Every number below comes from a run recorded here; what was not tested is said to be untested.

## Result in short

- reccmp sees all 58 annotated functions (56 addresses; 0x10026E90 and 0x100272B0 are annotated in both
  `small_math.cpp` and a `perm_*.c` file, compared in two DLLs). match.py: all 58 IDENTICAL.
- reccmp gives 100% for 57 of 58. **Camera_GetFrameScale (0x10014CF0) is 80%**: its `1.0f` constant is a `1.0
  (FLOAT)` in our DLL and `<OFFSET2>` in the game's, because reccmp reads **no base relocations** from
  FFXiMain_unpacked.dll (its relocation directory begins inside `.reloc`, not at the section start), and reccmp
  recognises a float constant only at a relocated operand.
- reccmp's 100% is weaker than match.py's IDENTICAL, shown by tests here: the source changed to read the **wrong
  global** (0x104E0AFC for 0x104E0AF8) is still "100% match" in reccmp, while match.py says DIFFERS at the byte. A wrong
  call target is caught by both.
- So reccmp **cannot replace match.py** on this binary. It can run beside it as a second view (HTML/JSON diffs,
  progress over time); a disagreement it reports is worth reading, but its agreement adds nothing match.py has not
  already established.

## Setup

The original: reccmp takes the target name from the original's file name (`--paths`: `original.stem.upper()`,
reccmp/project/detect.py), so `OUT_DIR/orig/FFXIMAIN.dll` is a symlink to `$FFXI_DLL` outside the repository. The
recompiled DLLs and PDBs are built by `pipeline/vc6/reccmp_link.py` (new), outside the repository too:

```
export FFXI_DLL=.../FFXiMain_unpacked.dll VC6=.../vc6tc WINEPREFIX=.../wpfx_worker_reccmp
python3 pipeline/vc6/match.py --all                          # 45 files, 58 functions, every one IDENTICAL (exit 0)
python3 pipeline/vc6/match.py --all --flags "/O2 /Z7"        # the same with debug info: every one IDENTICAL (exit 0)
python3 pipeline/vc6/reccmp_link.py OUT_DIR --reccmp VENV/bin --match            # build, reccmp --paths, match.py
python3 pipeline/vc6/reccmp_link.py OUT_DIR --skip-build --sizes --reccmp VENV/bin   # reccmp with the game's sizes
```

What reccmp_link.py does, and why each step (the details are in its docstring):

- **Compile**: match.py's own command (`cl /nologo /c /Gy` + `/O2`) plus `/Z7` (CodeView in each object). The second
  match.py run above shows `/Z7` changes no code byte of any of the 58 functions. One compile at a time.
- **Two DLLs**, `recompile/*.c` and `recompile/*.cpp`: `small_math.cpp` and `perm_10026E90.c` / `perm_100272B0.c`
  define the same `extern "C"` symbols (`_Vec4_Copy`, `_Vec3_ScaleInPlace`), which one link would refuse.
- **Link**: `link /NOLOGO /DLL /NOENTRY /NODEFAULTLIB /DEBUG /INCREMENTAL:NO /OPT:NOREF /OUT:FFXIMAIN.DLL
  /PDB:FFXIMAIN.pdb`. No `/FORCE`. The unresolved externals are taken from the linker's own LNK2001 list and given
  stub definitions in a generated `stubs.c` / `stubs.cpp`:
  - 22 functions the C sources call (`_FUN_XXXXXXXX`): an empty function under `// STUB: FFXIMAIN 0xXXXXXXXX` (the
    address the name carries); reccmp matches a STUB by address and does not compare it;
  - one C++ global, `?DAT_104568fc@@3PAUCam@@A` (`Cam *DAT_104568fc`, Camera_GetFrameScale's): `struct Cam;` then
    `// GLOBAL: FFXIMAIN 0x104568FC` and `Cam *DAT_104568fc;`;
  - `__fltused` (each group): `int _fltused;`, not annotated. Linking LIBC.LIB for it instead pulls in LIBC's
    floating-point start-up and then `crt0.obj`, which needs `_main` (seen with `/VERBOSE`).
- **reccmp**: `reccmp-reccmp --paths OUT_DIR/orig/FFXIMAIN.dll DLL PDB SRC --json ...` per DLL; with `--sizes`, from
  generated project files (`reccmp-project.yml` with a CSV data source `address|size` from data/func_extents.json,
  `reccmp-user.yml`, `reccmp-build.yml`) as `reccmp-reccmp --target FFXIMAIN`. reccmp runs the bundled cvdump.exe under
  Wine on the PDB (Visual C++ 6.0's PDB format is read without complaint). One reccmp run takes about 4 s.

Two settings were found by reccmp's own output, not chosen in advance:

- With `/DEBUG` alone the link is incremental, and every call goes through a `jmp` thunk (our call at 0x10001590 in
  the C DLL went to 0x10001096, `e9 ...`). reccmp then showed `call <OFFSET1>` on our side against `call FUN_100DC130
  (FUNCTION)` on the game's, and the 11 functions that call a stub were 60-93%. `/INCREMENTAL:NO` makes them 100%.
- reccmp names a GLOBAL from the line after the marker with a regular expression (reccmp/parser/util.py
  `get_variable_name`): on `struct Cam; Cam *DAT_104568fc;` it reads `Cam`, so the game's side was named `Cam` and
  ours `DAT_104568fc` (70%). The struct declaration is therefore written on the line before the marker.

## Results

match.py: `match.py SRC --json` per file (the verdict is IDENTICAL for each). reccmp: the `matching` field of its JSON
report, with `--paths` and with the game's sizes from a CSV (`--sizes`).

| address | function | source | match.py | reccmp | reccmp, game sizes |
|---|---|---|---|---|---|
| 0x1000D100 | Unit1000D0F0_Swap16 | perm_1000D100.c | IDENTICAL | 100.00% | 100.00% |
| 0x10014CF0 | Camera_GetFrameScale | small_math.cpp | IDENTICAL | 80.00% | 80.00% |
| 0x10014E60 | Camera_SetField2FC | perm_10014E60.c | IDENTICAL | 100.00% | 100.00% |
| 0x10014E80 | Camera_SetField300 | perm_10014E80.c | IDENTICAL | 100.00% | 100.00% |
| 0x10014EB0 | Camera_SetField314 | perm_10014EB0.c | IDENTICAL | 100.00% | 100.00% |
| 0x10014ED0 | Camera_SetField318 | perm_10014ED0.c | IDENTICAL | 100.00% | 100.00% |
| 0x10026E90 | Vec4_Copy | perm_10026E90.c | IDENTICAL | 100.00% | 100.00% |
| 0x10026E90 | Vec4_Copy | small_math.cpp | IDENTICAL | 100.00% | 100.00% |
| 0x10026EB0 | Vec3_Copy | small_math.cpp | IDENTICAL | 100.00% | 100.00% |
| 0x10026F20 | Vec3_AddInPlace | small_math.cpp | IDENTICAL | 100.00% | 100.00% |
| 0x10027120 | Vec3_Sub | small_math.cpp | IDENTICAL | 100.00% | 100.00% |
| 0x100272B0 | Vec3_ScaleInPlace | perm_100272B0.c | IDENTICAL | 100.00% | 100.00% |
| 0x100272B0 | Vec3_ScaleInPlace | small_math.cpp | IDENTICAL | 100.00% | 100.00% |
| 0x10027990 | T::ReturnThis | small_math.cpp | IDENTICAL | 100.00% | 100.00% |
| 0x100279A0 | Unit10026C70_Nop | small_math.cpp | IDENTICAL | 100.00% | 100.00% |
| 0x100279B0 | M::Identity | small_math.cpp | IDENTICAL | 100.00% | 100.00% |
| 0x10027CD0 | N::SetRow3 | small_math.cpp | IDENTICAL | 100.00% | 100.00% |
| 0x10027CF0 | T::SetTranslation | small_math.cpp | IDENTICAL | 100.00% | 100.00% |
| 0x10036050 | Unit10035B00_SetScaled10351704 | perm_10036050.c | IDENTICAL | 100.00% | 100.00% |
| 0x100360A0 | Unit10035B00_SetScaled10351700 | perm_100360A0.c | IDENTICAL | 100.00% | 100.00% |
| 0x1008A9E0 | Unit10086E90_SetFlagBit2 | perm_1008A9E0.c | IDENTICAL | 100.00% | 100.00% |
| 0x100E1B50 | Unit100D7420_CountActive | perm_100E1B50.c | IDENTICAL | 100.00% | 100.00% |
| 0x100E6880 | Unit100D7420_SetDuration138 | perm_100E6880.c | IDENTICAL | 100.00% | 100.00% |
| 0x100EF6F0 | Unit100D7420_UpdateThenAcquire | perm_100EF6F0.c | IDENTICAL | 100.00% | 100.00% |
| 0x100F9DA0 | Unit100D7420_ClearSessionGlobals | perm_100F9DA0.c | IDENTICAL | 100.00% | 100.00% |
| 0x100FC510 | Unit100D7420_GetIdPair | perm_100FC510.c | IDENTICAL | 100.00% | 100.00% |
| 0x101039E0 | ntGameTimeGet | perm_101039E0.c | IDENTICAL | 100.00% | 100.00% |
| 0x10103A90 | Time_ModDiv12FC00Div19500 | perm_10103A90.c | IDENTICAL | 100.00% | 100.00% |
| 0x10103B10 | Time_ModDivD80Div90 | perm_10103B10.c | IDENTICAL | 100.00% | 100.00% |
| 0x10108590 | Unit100D7420_AllocEntry | perm_10108590.c | IDENTICAL | 100.00% | 100.00% |
| 0x10122E20 | Unit100D7420_Call10122E30With1 | perm_10122E20.c | IDENTICAL | 100.00% | 100.00% |
| 0x10123650 | Cell_SetByte1036E0F0 | perm_10123650.c | IDENTICAL | 100.00% | 100.00% |
| 0x10123680 | Cell_SetWord1036D6B0 | perm_10123680.c | IDENTICAL | 100.00% | 100.00% |
| 0x10126320 | W::ClearWords | small_math.cpp | IDENTICAL | 100.00% | 100.00% |
| 0x10126350 | N7::N7 | small_math.cpp | IDENTICAL | 100.00% | 100.00% |
| 0x10161FF0 | Unit1012AC80_SetTopByte | perm_10161FF0.c | IDENTICAL | 100.00% | 100.00% |
| 0x101938B0 | Unit1018BA60_GetSettingMapped | perm_101938B0.c | IDENTICAL | 100.00% | 100.00% |
| 0x101BA5C0 | RC::ResetCounters | small_math.cpp | IDENTICAL | 100.00% | 100.00% |
| 0x101CC510 | Unit101C72E0_GetCC4F0ActorByIndexAt1FA | perm_101CC510.c | IDENTICAL | 100.00% | 100.00% |
| 0x101CC550 | Unit101C72E0_GetCC4F0ActorAt2A0 | perm_101CC550.c | IDENTICAL | 100.00% | 100.00% |
| 0x1022B290 | Unit1021D930_TickCounters | perm_1022B290.c | IDENTICAL | 100.00% | 100.00% |
| 0x102577C0 | Unit10253A20_GetDataOrDefault | perm_102577C0.c | IDENTICAL | 100.00% | 100.00% |
| 0x10257800 | Unit10253A20_GetDwordEntry | perm_10257800.c | IDENTICAL | 100.00% | 100.00% |
| 0x1025C0F0 | Unit10253A20_WrapSjisAt | perm_1025C0F0.c | IDENTICAL | 100.00% | 100.00% |
| 0x1025E210 | FontGlyphCache_AddToCounter | perm_1025E210.c | IDENTICAL | 100.00% | 100.00% |
| 0x10271D70 | StAvatar_SelectEntry | perm_10271D70.c | IDENTICAL | 100.00% | 100.00% |
| 0x102778C0 | Shader_SetFlagBit0 | perm_102778C0.c | IDENTICAL | 100.00% | 100.00% |
| 0x10277A50 | SqmdModel_Validate | perm_10277A50.c | IDENTICAL | 100.00% | 100.00% |
| 0x1027A720 | Unit1027A4B0_Set | perm_1027A720.c | IDENTICAL | 100.00% | 100.00% |
| 0x10281B60 | sqgxTextureGetSize | perm_10281B60.c | IDENTICAL | 100.00% | 100.00% |
| 0x10283260 | SqgxContext_SetCurrentIndex | perm_10283260.c | IDENTICAL | 100.00% | 100.00% |
| 0x10283380 | sqgxAmbient | perm_10283380.c | IDENTICAL | 100.00% | 100.00% |
| 0x102833A0 | SqgxContext_SetFloat76C | perm_102833A0.c | IDENTICAL | 100.00% | 100.00% |
| 0x102838A0 | sqgxPixelSize | perm_102838A0.c | IDENTICAL | 100.00% | 100.00% |
| 0x10291F10 | SqhiNode_AppendChild | perm_10291F10.c | IDENTICAL | 100.00% | 100.00% |
| 0x1029BD40 | Sqgx_SetFloat18 | perm_1029BD40.c | IDENTICAL | 100.00% | 100.00% |
| 0x1029C460 | Sqgx_SetContextFieldE24 | perm_1029C460.c | IDENTICAL | 100.00% | 100.00% |
| 0x102A3540 | Sqgx_SetParam9cc | perm_102A3540.c | IDENTICAL | 100.00% | 100.00% |

reccmp's summary lines: C DLL "Implemented 66.67% (44 / 66), Accuracy 100.00%" (the 22 stubs count as functions not
implemented); C++ DLL "Implemented 100.00% (14 / 14), Accuracy 98.57%".

## The disagreement: Camera_GetFrameScale (0x10014CF0), 80%

`reccmp-reccmp --target FFXIMAIN --verbose 0x10014CF0` (C++ DLL):

```
0x10014cf0 / 0x10001180 : mov ecx, dword ptr [DAT_104568fc (DATA)]
0x10014cf6 / 0x10001186 : fld dword ptr [ecx + 0x28]
0x10014cf9 /            : -fcomp dword ptr [<OFFSET2>]
           / 0x10001189 : +fcomp dword ptr [1.0 (FLOAT)]
...
0x10014d06 /            : -fld dword ptr [<OFFSET2>]
           / 0x10001196 : +fld dword ptr [1.0 (FLOAT)]
```

The game's operand is 0x1032961C in `.rdata`, which holds 1.0f; match.py compares the bytes around it and reports the
DIR32 target (the 1.0f constant's symbol carries no address) as not compared. reccmp names a float constant only when
the operand's address is in the image's relocation table (reccmp/analysis/float_const.py `find_float_consts`: `if
inst.address + 2 not in image.relocations: continue`). For the game's DLL that table is empty in reccmp:

- reccmp reads relocations only from sections whose start lies inside the base relocation directory
  (reccmp/formats/pe.py `get_sections_in_data_directory`, used by `relocations`);
- FFXiMain_unpacked.dll's directory is at RVA 0xC0836C, size 0xBF30, and its `.reloc` section starts at RVA 0xBE5000
  (size 0x2F29C), so no section qualifies: `detect_image(...).relocations` has 0 entries (run here).
- The table itself is there: parsed from the start of `.reloc` it has 92,563 HIGHLOW entries, from page 0x1000 to
  page 0xBE4000, including 0x10014CFB (this operand); the 24,088 inside the directory's range cover pages 0x329000 to
  0xBE4000 only (no `.text` page). Why the unpacked file's directory starts where it does is not known here.

**Experiment** (reccmp patched at run time, not in the repository: `relocations` read from the whole section that
holds the directory, from its start). The C++ DLL is then 14 of 14 at 100% (Camera_GetFrameScale included). But 9
of the 44 C functions drop: 0x1008A9E0 77.78%, 0x100EF6F0 90.00%, 0x100FC510 90.91%, 0x10123650 80.00%, 0x10123680
80.00%, 0x1022B290 85.19%, 0x10271D70 78.26%, 0x10277A50 76.92%, 0x10281B60 80.00%. Each uses a global written as a
literal address (`*((undefined1 *) 0x1036E0F0u)`), which the compiler emits as a plain number with no relocation; the
game's operand is relocated, so reccmp now gives it a placeholder and ours stays a number:

```
0x1012365c /            : -mov byte ptr [eax + <OFFSET1>], cl
           / 0x1000129c : +mov byte ptr [eax + 0x1036e0f0], cl
```

So with the relocations reccmp fails 9 functions match.py proves identical, and without them it fails 1. No setting
gives agreement with match.py on all 58 as the sources are written.

## What reccmp's 100% does and does not check (tests here)

Sensitivity tests on perm_101039E0.c (`ntGameTimeGet`: `return FUN_100DC130() + *(int *)0x104E0AF8;`), each a
modified copy built and compared outside the repository:

| change | reccmp | match.py |
|---|---|---|
| calls FUN_100DC140 instead, its stub annotated `STUB 0x100DC140` | 66.67% (`-call <OFFSET1>` / `+call FUN_100DC140`) | DIFFERS: REL32 targets 0x100DC140, the game 0x100DC130 |
| calls FUN_100DC140, stub not annotated | 66.67% (same diff) | (same source as above) |
| calls FUN_100DC130 (right), stub not annotated | 66.67% (`-call <OFFSET1>` / `+call FUN_100DC130`) | (not run; the bytes are those of the IDENTICAL source) |
| reads 0x104E0AFC instead of 0x104E0AF8 | **"100% match"** | DIFFERS: `+7: 0xFC here, 0xF8 in the game` |
| the CSV size of Vec3\_Copy set to 99 (it is 25) | 31.58% (the game read on past the function) | (match.py reads data/func_extents.json) |

From these and the code they exercise:
- reccmp compares disassembly text (capstone), with every absolute memory operand `[addr]` replaced by a name or a
  numbered placeholder (reccmp/compare/asm/parse.py `hex_replace_always`). Placeholders are numbered in order of
  first use within the function, so a different address in the same place gets the same placeholder: a wrong
  unnamed global is invisible. (With the global annotated as a GLOBAL it would be named on one side only; not
  tested.) match.py compares the bytes, and a literal address is bytes.
- A call matches only when the callee is annotated (game side) and named (our side, from the PDB); an unannotated
  callee is a mismatch even when right. Hence the stubs above.
- Without a size for the game's function, reccmp reads it over our function's length (reccmp/compare/functions.py:
  `orig_size = recomp_size` when the original's size is unknown), so a recompile that is a correct prefix of a longer
  game function would score 100%. The CSV of data/func_extents.json sizes closes that; with it the results are the same
  (table, last column).
- Read from the code, not tested: since it compares instruction text, two encodings of the same instruction (e.g.
  `mov eax, ecx` as 8B C1 or 89 C8) would compare equal in reccmp and differ in match.py.

## What reccmp offers that match.py does not

- **More entity kinds**: `VTABLE`, `GLOBAL`, `STRING`, `LINE`, `TEMPLATE`, `SYNTHETIC`, `LIBRARY` annotations;
  `reccmp-vtable` (vtable contents), `reccmp-datacmp` (globals' initial data and types from the PDB),
  `reccmp-stackcmp` (stack layout of a near-match), `reccmp-roadmap`, `reccmp-verexp` (exports). Run here:
  - `reccmp-datacmp --target FFXIMAIN` (C++ DLL): "Variables: 1. Issues: 0" -- the one global is our uninitialised
    stub, so this says nothing about the game's data;
  - `reccmp-vtable`: "Vtables found: 0. 100% match." and then `AssertionError` (reccmp/tools/vtable.py, `assert
    fun_match.name is not None`). The nameless function is the entry-point pair reccmp always makes
    (reccmp/compare/analyze.py `match_entry`): the game's entry 0x1031672F with ours, which is the image base because
    of `/NOENTRY` (AddressOfEntryPoint 0). A DLL with an entry point would avoid it (untested);
  - `reccmp-stackcmp --target FFXIMAIN 0x10014CF0`: an empty listing (the function has no stack variables);
  - `reccmp-decomplint --target FFXIMAIN recompile`: 47 `not_strict_format` warnings (the strict form wants lower-case
    hex, `0x100279b0`), `function_out_of_order` errors in small_math.cpp (its functions are not in address order)
    and `duplicate_offset` warnings for the two addresses annotated twice. Exit status 1 (the 3 errors).
- **Whole-binary bookkeeping**: progress and accuracy over all annotated functions, `--json` reports and `--diff`
  against an earlier report, an HTML report (`--html`), a Ghidra import of names and types from the PDB.
- **Speed of a re-check**: once linked, a reccmp run is about 4 s for either DLL; match.py --all recompiles every
  file (about 3 minutes for these 45 under Wine). The build reccmp needs costs the same compiles plus a link.

match.py has, and reccmp lacks on this binary: a byte-for-byte comparison (literal addresses, encodings); each
relocation's target compared by the address its symbol names, with uncompared ones listed; the game's function
length; no link step, no stubs, no PDB.

## What using it in the pipeline would need (as found here)

1. Build with reccmp_link.py's link (two or more DLLs while sources define a symbol twice; `/INCREMENTAL:NO`; stubs for
   every external, each callee and global annotated with its game address).
2. The game's function sizes as a CSV data source (`--sizes` writes it from data/func_extents.json).
3. The relocation table: with reccmp as it is, none is read from FFXiMain_unpacked.dll. Reading it (a change to
   reccmp's `relocations`, or a copy of the DLL whose directory covers the whole table) fixes float constants but makes
   reccmp fail every literal-address global the sources use, until those globals are written as annotated symbols
   (`// GLOBAL: FFXIMAIN 0x...`). Neither was done; the second would change recompile/ sources, which match.py proves
   as they are.
4. Even then, the wrong-global test above shows reccmp's agreement is not a byte-identical proof. It would stay a
   second view; the verdict stays match.py's.
