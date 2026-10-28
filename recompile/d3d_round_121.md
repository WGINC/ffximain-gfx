# Round 121: render-core functions (#118, #119)

Each source below is at the game's bytes by pipeline/vc6/match.py (`/O2`, every relocation and call target compared);
`match.py --all` exits 0. A source at the game's bytes computes what the game's function computes; nothing about the
original source follows from it.

| address | task | file | what decided it |
|---|---|---|---|
| 0x1000A6E0 CDx::CreateTexture | #118 | d3d_1000A6E0.cpp | one result variable through the five CreateTexture attempts, then `if (hr >= 0) return tex; return 0;`: the success block first, as the game (round 118 had the attempts chained in one condition: 15 lines) |
| 0x10025260 | #119 | d3dcaller_r121.cpp | a constructor (vtable 0x1032A460 by a SYMBOL line); the texture and surface each through a local reloaded after AddRef, as the sweep's text; the sweep could not compile it (no AddRef slot in its stand-in vtable) |
| 0x10033A70 Model_EndOffscreenPassBindTextures | #119 | d3dcaller_10033A70.cpp | from round 120's 5-line form: the mode test's failure jumps into the second `return 0` (one block in the game): a `goto` to that return gives it |
| 0x10163400 Gfx_DrawSolidRect2D | #119 | d3dcaller_10163400.cpp | the decompiler's 20 frame locals as four 20-byte vertices (FVF 0x44), set one field group at a time; the group order searched (6 of 120 orders give the bytes); the closing call made on the object the game passes in ECX |
| 0x100764C0 | #119 | d3dcaller_r121.cpp | the four copied arguments as floats: VC6 copies floats through integer registers without reusing the loads, so each argument is read twice, as in the game (round 120: integers, structs and memcpy read once) |

**Rule 24. A float copied twice is read twice.** VC6 `/O2` moves a `float` through EAX/EDX when it is only copied, and
does not reuse the load for a second copy. Two reads of the same stack argument in the game, with no store between
them, point at a float. (0x100764C0)

**Rule 25. Where the game's success return comes before the failure return, the test is on a result variable after
the chain** (`hr = f(); if (hr < 0) hr = g(); ... if (hr >= 0) return x; return 0;`): VC6 threads each earlier
`jge` to the success block. (0x1000A6E0)

**Rule 26. One failure block reached from two tests is a `goto` to the second.** Where the game's first test jumps
forward into a `return` that a later test reaches by falling through, VC6 gives that only for a `goto` to the later
return; an early return is placed first, and a wrapping `if` merges both at the end. (0x10033A70)

**Rule 27. A run of frame locals filled with the same few values is an array of structs, filled a field group at a
time.** Hex-Rays splits a stack vertex array into one local per field; written back as the array, the compiler's
scheduling of the stores depends on which group comes first, which a search over the group orders settles. (0x10163400)

cpp_permuter (pipeline/vc6/cpp_perm.py) on 17 further in-tree misses: scores stay far (0x10026AD0 159 to 100,
0x10038720 144 to 133 after 400 compiles); 0x10001520 and 0x100295A0 did not compile from the old sweep sources
(MSG, before the WINT layouts). Not used further on these. Its candidates must be read before use: some best scores come from deleting statements
or changing a type (0x10163400's 49 deleted the vertex stores; 0x10043590's 13 made a pointer a short).
