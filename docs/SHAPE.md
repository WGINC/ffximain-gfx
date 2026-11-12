# Source-shaping: making the tree read like source, without changing what it is (11 Nov 2026)

The owner asked for the reconstruction to be shaped into source-like code with every class, constant, struct and
object named, procedurally. This page says what that takes, what is done, and what each remaining pass needs. It is
the place for this subject; `docs/NAMES.md` stays the place for how a name is given.

## The rule that makes it safe

(12 Nov) The passes are rules of `pipeline/source_shape.py`, which `regen_clean.sh` runs after the compile repairs
and which proves each rule per function by its compiled code (its `objcode` proof), putting back any function where
the proof fails. They were first written as a hook in `hx2clean.py`, before the repairs; that was moved, because
`source_shape.py` already existed for exactly this and runs at the right point. In the first round with them (r20), `source_shape.py` put the members rule back in 31 functions and d3d_values in
15. Reproduced on CDx.cpp: the rewrite alone, and over the whole file, gives identical code. The cause was the
proof's baseline: an `objcode` rule was compared with the file's code before any rule ran, so a code change made
earlier by a rule proven another way (undefined4's probe; GCC's register choice can follow a type's spelling) failed
every later `objcode` rule in that file. The baseline is now the code just before the rule (r21).
`pipeline/shape.py` holds the two
rules' text rewrites and two checks: `check` (the whole tree before and after the rewrites, per file) and `same OLD
NEW` (two trees, per function).

A shaping pass changes how the source reads and not what it compiles to. That is checked, not argued:

    python3 pipeline/shape.py check        # every file of tree/src, compiled before and after: the objects' code,
                                           # data and relocations must be byte for byte the same

A pass lives in `pipeline/shape.py` and is applied by `hx2clean.py` to every file it writes, so the tree still
regenerates from its sources. Because the objects are the same, the gate's verdicts stand (its cache reuses the
verdict of identical code), and `tree_round.py` counts a live-gate verdict for a function whose text only a
shaping pass changed.

A pass that needs a fact (which class a pointer is, whether an integer is a float's bits) takes it from a named
source or leaves the code as it is. A name that is a description and not a recovered name is marked as one.

## Where the tree stands (tree/src: 4,897 functions, 159,000 lines)

| What does not read like source | Count | What decides it |
|---|---|---|
| Direct3D state values as numbers | 744 named on 11 Nov; the rest are not enumerations (references, colours, indices) | The SDK |
| `*(T *)(p + N)` on a parameter or local | 5,945 | The class of `p` in that function: needs a proof per function |
| `*(T *)(g_x + N)` on a named global object | 868 (275 on `g_pResourceMng`, 125 on `g_pSqgxContext`, 95 on `g_pApp`, 93 on `g_pSceneState`) | The global's class: one layout each |
| `*(T *)(expr + N)` on an expression | 530 | As the first row |
| Integer literals in the range of float bit patterns (`1065353216` = 1.0f) | 341 | Whether the destination is a float: the instruction that reads it |
| `goto` | 1,132 | Structure only: no fact needed |
| Functions in a unit without a real name (`unit_10048000`) | 3,660 | The unit's source file name: PS2 symbols, runtime class tables |
| Globals named by address | 2,236 (696 named) | Evidence per global (docs/NAMES.md) |
| Class members named `field_XX` | 269 (194 named) | Evidence per member |

## The passes, in order

1. **Direct3D state values (done, 11 Nov).** `SetRenderState` / `SetTextureStageState` with a literal state and a
   literal value: the value is written as the SDK's name (`D3DCMP_GREATER`, `D3DTOP_MODULATE`,
   `D3DTA_TEXTURE | D3DTA_ALPHAREPLICATE`, `TRUE`). 744 places in 33 files; 0 objects changed. Which enumeration a
   state takes is the SDK documentation's, written out in `shape.py` (not machine-read from a file here; a wrong row
   could only mislabel, since the name is the same integer).
2. **Offset reads to members (done, 12 Nov).** `*(T *)((char *)B + N)` and `*(T *)&(*(unsigned __int8 *)((char *)B + N))`
   where B's class has a member starting at +N: `B->m` when T is the member's type, `*(T *)&B->m` otherwise (the
   same address and the same type read). B is `g_pCDx` or `g_pProcessor` (declared class pointers) or `this` in a
   member function. 2,088 places in 74 files; 0 objects changed. Reads at an offset where no member starts stay.
2b. **Layouts for the other global objects.** 868 reads go through eight or so named globals. Each is one object: its
   layout is the set of (offset, width) the tree reads and writes through that global, which the generator already
   knows how to turn into a class (it does so for `this`). Members start as `field_XX`. Object-identical by
   construction; needs the generator to treat `g_x` as a typed pointer.
3. **Layouts for parameters and locals.** The same, where the pointer's class is proven for that function:
   `data/this_class.json`, `class_flow.json` and the typed prototypes already prove many; the 5,945 that remain are
   the ones without a proof. Each needs type flow from a proven source (a caller passing a typed object, a vtable
   call on it). Where there is none the read stays raw and is listed.
4. **Float constants.** Measured 12 Nov: of the stores `*(_DWORD *)&X = <float bits>` into a member, none goes
   into a member declared `float`: the members they write are still placeholder bytes. So this needs the member's
   type set from its float reads first (a generator change to the layout), then the constant follows from the type. An integer that is a float's bits is written as the float only where the game's code reads
   that location with a floating-point instruction (`data/fp_consts.json`, `fp_callsites.json` hold such facts for
   call arguments). Not by the look of the number: `1065353216` is also a valid integer.
5. **Real names.** Classes and units from the game's runtime class tables and the PS2 build's debug symbols
   (`data/ps2_matches.json`; dancingmad's parsed DWARF: 2,758 types, 19,944 symbols). Members from the PS2 types
   only where a matched pair of functions reads the member at the corresponding place: PS2 and PC layouts differ, so
   an offset alone proves nothing.
6. **Control flow.** `goto`s removed where the flow graph reduces to loops and conditionals; the rest stay.
7. **Descriptive names.** Where no name is recoverable, a name that says what the code shows the thing is used for,
   marked as a description, with the evidence. Last, because every earlier pass reduces how many are needed.

## Naming round 6 (12 Nov)

Four rounds, each one model working from `pipeline/name_context.py`'s list of every use, one file per class, every
name with its evidence and marked `"kind": "descriptive"`; Opus for CXiSkeletonActor and the CDx group, Sonnet for
the smaller classes and the globals. Reviewed before merging into `data/field_names.json` and
`data/names_pending_idb.json`:

- members: 107 proposed, 106 taken (211 member names now); `CMoResource*::poolNode` (5) and `CTsZoneMap::cellGrid`
  left out (their evidence does not show the name); CXiSkeletonActor's "dir" members renamed `rot*` (the code wraps
  them into [-pi, pi] and feeds them to Mat4_PostRotateX/Y/Z: angles, not a direction); `CDx::isAtiAdapter`'s
  source named (VendorId 0x1002 is ATI's in the PCI-SIG registry);
- globals: 7 named, 72 left unnamed with what is known (mostly write-only, or set and never read in the tree);
- left unnamed in all: about 170 members, each with a sentence saying what is known.

A round's agent noted that `name_context.py fields` lists only `this->field_X` uses, not raw-offset uses through
other pointers; those were searched by hand for CXiSkeletonActor only.

## What this will not be

The original source. Most original names of locals, members and globals survive nowhere that is known. The result
is code that compiles to what the gate verified, reads like source, and says for each name whether it was recovered
or given.
