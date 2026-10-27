# Matching round 3 for tasks #118 and #119 (27-28 Oct)

Continues recompile/d3d_round_119.md (rules 1-15). Toolchain rebuilt in a new container from the owner's archive with
pipeline/vc6/setup_vc6.sh: `match.py --compid` C 48 / C++ 49 build 9044; `match.py --all` on the 98 functions of
round 119: all IDENTICAL before anything was added.

New tools: pipeline/vc6/mdiff.py (the unified diff of our instructions against the game's, the address in an operand
blanked, the rest of the operand kept; the counts of "lines" below are its counts), and `// SYMBOL: <decorated name> 0xADDR` lines in a source, which
match.py now uses to compare the relocations of symbols the compiler names itself.

## Result

**#118: 0x10054D70 BeginEffectOffscreenPass at the game's bytes** (recompile/d3d_10054D70.cpp; outside the frame
set). Every relocation is compared (the static object's four symbols through SYMBOL lines) except the pooled
`__real@40000000`, read from the game at the address its bytes use: 0x10329990 = 2.0f, equal.

This is a function tree r13 FAILs (docs/HISTORY.md 27 Oct): the tree passes two arguments to g_pProcessor's slot 9
at 0x10054F1B, the game pushes three, `(m, s, m)`. The source here is written from the game's bytes, so it passes
the three; that the callee takes 12 bytes is data/purges.json's (0x1006D270 `ret 12`).

From the sweep's conversion (91 lines) to 0, each step kept only where the bytes came closer:

| change | lines after |
|---|---|
| plain locals instead of the decompiler's 0x494-byte frame block (rule 12), the three-argument call | 77 |
| the viewport zeroed with `memset` (the game's `rep stosd` of 6 dwords) before its fields are set | 49 |
| eye and target as 3-float structs, `at = eye; at.y = eye.y - 2.0f` (the game writes at.y twice: the copy, then the result) | 21 |
| the eye read field by field from the matrix (`eye.x = m[12]; ...`), not as a struct through a pointer | 9 |
| the up vector as a function-local static object with an inline constructor and an empty inline destructor | 4 |
| the global read into a block-scoped local between the viewport's fields (rule 8's form, placed where the game loads it) | 0 |

**#119: 0x10040E70 RebuildBlendWeights at the game's bytes** (recompile/d3dcaller_10040E70.cpp), every relocation
compared. From the sweep's 131 lines to 0 in one rewrite: the game calls 0x10054D70 and 0x10054F80 as methods of one
local object (`lea ecx, [esp+0x10]` before each, the same frame slot), so they are a local `Pass` and its `Begin` /
`End` (SYMBOL lines give their game addresses, the calls' targets in the game's bytes); the 64-byte matrix copy a
struct assignment (`rep movsd`); the null-checked pointer at +0x104 read with the decompiler's ternary.
0x10054D70 itself does not read ECX: as a member that ignores `this` it compiles to the same bytes as the free
function recompile/d3d_10054D70.cpp declares.

**#119: 0x1000C6E0 CDx::CreateStereoResources and 0x10026990 CYyIcon::Draw at the game's bytes**
(recompile/d3dcaller_1000C6E0.cpp, d3dcaller_10026990.cpp), every relocation compared but 0x10026990's pooled 0.5f
(`__real@3f000000`, the game's 0x10329A08 = 0.5). 0x1000C6E0 from 53 lines in one rewrite: its callees as the tree's
CDx and CEnv methods (match.py now reads a decorated `?Method@Class@@` as the tree's Class::Method and compares it
through tree/placement.json), 0x1000D620 called on g_pEnv (the tree's text passes no ECX), and the two colours one
8-byte array written byte by byte. 0x10026990 from 60: float arithmetic in the decompiled order, both texture sizes
converted first, and rule 22.

**#119: 0x10002040 Gfx_CopySurfaceRectToTexture and 0x100266C0 Dbg_DrawAxesAt at the game's bytes**
(recompile/d3dcaller_10002040.cpp, d3dcaller_100266C0.cpp), every relocation compared. 0x10002040 from 63 lines in
one rewrite: one failure path reached by `goto` from each failing step, each object released there if held (the
decompiler's `&&` chain with two copies of the cleanup gives another layout). 0x100266C0 from 99: the three
matrices are stack objects with a constructor (0x10027990) and a destructor (0x100279A0), rule 23; the origin
vertex written once; for each axis the two colours, then a block-scoped read of g_pCDx (rule 8), then the far
vertex.

**Where step 20 stands** (pipeline/vc6/render_core_status.py, verification/render_core_status.json; the sets as
round 118 defines them): #118, 30 of the 57 Direct3D-calling functions at the game's bytes (25 of the 41 in the
frame set); #119, 26 of their 100 direct callers (20 of the 60 in the frame set).

pipeline/vc6/reorder.py searches the order of the statements in a marked region (swaps and moves, hill climbing on
mdiff's count) and writes a source only where match.py says IDENTICAL; a source at the game's bytes computes what
the game's code computes, whatever the order that got it there.

**Not in the tree, written from the game's bytes and Ghidra's text** (Ghidra is installed again; the 43 functions
of the two sets that are not in the tree exported for their text): 0x1006ADC0 (#118, recompile/d3d_1006ADC0.cpp),
and 0x10044250, 0x10009E60, 0x10075FF0, 0x10075690, 0x1006B090 (#119, recompile/d3dcaller_notree_r120.cpp), every
relocation compared. These are not gate-verified (no tree source); being at the game's bytes, each computes what
the game's function computes. 0x1006B090 is a destructor (the class's vtable stored first, the member at +4
destroyed last by a tail jump). 0x100764C0 is not: the game copies the same four parameters twice, re-reading each
from the stack; struct copies, by-value structs, inline constructors and memcpy give one read (28 lines).

## Rules found in this round (each kept only where match.py said IDENTICAL, or where noted the bytes came closer)

16. **A function-local static object is the compiler's guard, not one written by hand.** The game's
    `mov al, [guard]; test al, 1; jne; mov dl, al; ...; or dl, 1; mov [guard], dl; ...; push $E; call atexit` comes
    from `static T x(args);` with T having a constructor and a destructor; the hand-written guard
    (`if (!(g & 1)) { g |= 1; ...; atexit(...); }`, the decompiler's form) gives `or al, 1` and another order. An
    empty inline destructor still registers the `$E` function, which compiles to a bare `ret` (the game's 0x10054F70).
    0x10075700 has one too (constructor 0x1006AD90, `$E` 0x10075950).
17. **Struct copies are integer moves; a field computed from a copy is written twice.** 0x10054D70.
18. **A struct zeroed by `memset` gives `rep stosd`**; `= {0}` gives one store per dword. 0x10054D70.
19. **A constant the compiler would fold stays as two multiplies when written as a division**: `x / 65535.0f * 480.0f`
    gives the game's `fmul [1/65535]; fmul [480]` (0x10329E9C is 1/65535 as a float); `x * 1.5259022e-05f * 480.0f`
    is folded into one multiply. 0x10017310 (from 4 lines to 2).
20. **Floats read through an inline accessor go through the FPU, plain reads by integer moves** (rule 10, again):
    0x1016DDF0 copies x by integer and y, z through the FPU; `a->x`, `a->gy()`, `a->gz()` gives that (117 lines to 6).
21. **The identity matrix written element by element in order** gives the game's 16 immediate stores in order; the
    decompiler's `memset` runs give zero registers and another order (0x1016DDF0).

22. **Two variables holding the same pointer keep a field read twice.** The game tests the texture's two pointers
    and then reads the first again to pick one; with one variable the compiler reuses the test's load. Testing
    through a typed pointer to the object and picking through the plain one (two locals, the same value) gives the
    game's second read (0x10026990, and 0x10026C70's selection). 0x10041160's second null check came the same way
    (a one-member struct pointer in the condition).

23. **Objects the game constructs and destroys on the stack are class objects with a constructor and a destructor.**
    The game calls 0x10027990 (`mov eax, ecx; ret`, the IDB's `??0Iostream_init`) on each 64-byte matrix as the
    function starts and 0x100279A0 on each as it ends, in the reverse order: what the compiler emits for
    `Mat a, b, c;` with an out-of-line constructor and destructor. The decompiler's calls in the body give neither
    the order nor the frame (0x100266C0, 0x100263F0).

`/G6` (Pentium Pro scheduling) gives 0x1006A020's epilogue (its 2-line miss), but under `/O2 /G6` most of the 98
matched functions differ (the device loads move), so it is not the build's setting; noted, not used.

## Not matched (closest, lines; what is left)

| address | lines | what is left |
|---|---|---|
| 0x10017310 Unit10014350_Draw (#119) | 2 | as round 119: `mov edi, [esp+0xc]` before `xor eax, eax` in the game; 7 more placements of the index and the array tried |
| 0x1006A020 Gfx_CreateVertexBuffer (#118) | 2 | as round 119: the result loaded before the pops in the game; a separate buffer local in three forms, C and C++, `/O1`, `/Ox`, `/Oy-`, `/G3`-`/G5`, `/Gr` |
| 0x1016DDF0 Dbg_DrawTriangleLines (#119) | 6 (from 117) | the global is loaded before the 16 matrix stores in the game (into EDX), after them in ours; a block-local copy, an inline CDx method, an argument-filling inline and a constructor tried |
| 0x10026C70 CYyIcon::DrawSubRect (#119) | 79 (from 105) | the game spills y1 to a parameter slot and copies it by integer; which float lands in which dead parameter slot is not reproduced |
| 0x100263F0 Dbg_DrawViewAxisGizmo (#119) | 4 (from 123) | written as 0x100266C0; the game loads g_pCDx into EDX right after the previous call, before the first axis's stores; the plain form loads it after them, a block-scoped copy takes EAX (4 placements tried) |
| 0x1006B2B0 CMoDX::CreateAndInitSystem (#119) | 15 (from 19) | the file manager reached as `mov eax, [edx+0x40008]; mov eax, [eax]; mov ecx, eax` in the game, loaded straight into ECX in ours (a local, an inline accessor tried); the singleton creations written as `p ? ctor(p) : 0` stored then used |
| 0x10041160 BindTintedTexture (#119) | 3 (from 164) | the scratch colour loaded as 32 bits into EBX and its second byte taken by `xor eax, eax; mov al, bh` in the game; a 16-bit read gives `mov al, bh` but `mov bx` and an extra `and`, a 32-bit read gives EDI and shifts (15 lines); the second null check of the pointer at +0x1A8 comes from reading it through a one-member struct (`struct H { int *p; }`) in the condition |
| 0x10075700 TextureHolder_BindWarped (#118) | 128 (from 133) | a 12-byte frame in the game (8 in ours); the a4 == 0 return not merged with the last one in the game |

The sweep's conversion (pipeline/vc6/sweep.py) now compiles 8 of the functions round 119 listed as not compiling
(`sin`/`cos`/`atan2`/`ABS`/`fpatan`, and two virtual-call spellings): 0x1002A3B0 252 lines, 0x10043720 214,
0x10046070 212, 0x100467E0 925, 0x10075700 133, 0x10174EC0 630, 0x10185CA0 1480 (and 0x10054D70 above).
0x10003890 and 0x100426F0 still do not (a `__thiscall` pointer through another object; a local declared with a
`__thiscall` pointer type).

`match.py --all` after this round: 91 files, every function IDENTICAL: 106 marks, 104 addresses (0x10026E90 and 0x100272B0 are in two files each).
