# Matching round 2 for tasks #118 and #119: the Direct3D-calling functions and their direct callers

Continues recompile/d3d_round_118.md (which defines the two sets and the rules 1-7 used here). Toolchain as there:
`match.py --compid` C 48 / C++ 49 build 9044. Every new file is IDENTICAL under `match.py --all`, and so are the 72
files that were there before. As in round 118, the only relocations match.py does not compare are the float
constants the compiler pools (`__real@...`); for every new file each one's value was read from the game at the
address the game's bytes use and is equal (a check run beside match.py: the bits named by the `__real@` symbol
against the 4 or 8 bytes at that address in FFXI_DLL).

`match.py --all` after this round: 85 files (72 before, 13 new), every one IDENTICAL (98 functions), exit 0.

## Result

**#118: 10 more at the game's bytes** (recompile/d3d_<addr>.cpp): 6 of the frame set (25 of its 36 device-calling
functions in the tree are now at the bytes: 6 before round 118, 13 in it, 6 here) and 4 of the 11 outside it:

| address | function | frame set | what decided the bytes |
|---|---|---|---|
| 0x1006AF00 | StateBlockHolder_ApplyRenderStates | yes | the global read into a local inside the loop (rule 8); was 2 lines off |
| 0x1006AF90 | StateBlock_ApplyTSS | yes | the same; was 2 lines off |
| 0x100472F0 | YmElem_RestoreDevice | yes | rule 8 on the SetRenderState return; the memcpy branch written first (was 4 lines off) |
| 0x1003DB60 | Sky_DrawDomeViewCentered | yes | rule 8 on the five SetRenderState calls; each matrix pop through a local copy of g_pProcessor (rule 9) |
| 0x1003E9A0 | YmElem_SetD3ADrawState | yes | rule 8 on the five SetRenderState calls; it also puts `this` in ESI and the result in EDI as the game does |
| 0x1003E910 | YmElem_DrawD3ASegment | yes | rule 8 on its one SetRenderState; it also gives the game's argument registers for the call after it |
| 0x10041AF0 | CMoElem_VT_1032ADE8::vf18 | no | rule 8 on the two `g_pResourceMng + 3528` reads and the SetRenderState |
| 0x1003D9A0 | Sky_DrawDome | no | as 0x1003DB60, and the colour loop indexed (rule 13) |
| 0x100431B0 | CMoDistModelElem::Draw | no | rule 8; the matrix pop by rule 9; the two float copies through inline accessors (rule 10) |
| 0x10076520 | DrawQuadDynVB | no | rule 8; each pair of equal vertex floats by one chained assignment (rule 3) |

**#119: 3 more** (recompile/d3dcaller_<addr>.cpp):

| address | function | what decided the bytes |
|---|---|---|
| 0x10025C40 | EnvMap_BeginPass | the `== 0` exit written first (rule 5); 0x100279A0 called on the matrix object (rule 7) |
| 0x1011BBD0 | Font_DrawGlyphQuad | float literals and float locals (rule 11); one statement order of three (rule 14); `v21[result++]` (rule 15) |
| 0x10033800 | Model_BeginOffscreenPass | the two 64-byte locals as plain arrays (not the decompiler's frame block); `< 4.0f`; the `== 0` branch first; 0x100279A0 on each object (rule 7) |

How each source was made: the tree's behaviour (every one of the 13 functions has `GATE: PASS` in tree/src); the
sweep's conversion of it (pipeline/vc6/sweep.py --calls) or round 118's closest source as the start; the globals as
declared address-named variables (rule 1); the Direct3D calls in the SDK's C++ form (the interface declarations
copied from tree/include/xi_d3d8.hpp, as in round 118); then the changes named above, each kept only where match.py
said IDENTICAL.

## Rules found in this round (each kept only where match.py said IDENTICAL)

8. **A device call through a copy of the global in a named local.** The game reads g_pResourceMng afresh for each
   device call into the register that then holds the device (`mov eax, [g]; mov eax, [eax+0xebc]; mov ecx, [eax]`),
   in every call of a run. Written as the tree has it, this compiler gives the first call that form and the later
   ones `mov edx, [g]; mov eax, [edx+0xebc]`. Reading the global into a block-scoped local first gives the game's
   form for every call:
   ```
   DEV->SetRenderState(D3DRS_ZWRITEENABLE, 0);                                       // 2nd call on: mov edx,[g]; mov eax,[edx+0xebc]
   { int g = g_pResourceMng_1047BFA8; (*(IDirect3DDevice8 **)(g + 3772))->SetRenderState(D3DRS_ZWRITEENABLE, 0); }
                                                                                     // every call: mov eax,[g]; mov eax,[eax+0xebc]
   ```
   `int g; g = G; ...` statement by statement gives the same instructions as the block form (a test of five calls
   in a row); an inline accessor doing the same copy
   (`inline IDirect3DDevice8 *Dev() { int g = G; return ...; }`) does not (it compiles as the tree's form). The
   change also moves registers elsewhere in the function: in 0x1003E9A0 it puts `this` in ESI and the result in EDI,
   in 0x1003E910 it swaps the two argument registers of the next call, in 0x1006AF00/0x1006AF90 it gives the
   global's load in the loop to ECX. Where the game has the other form (`mov edx, [g]; mov eax, [edx+0xebc]`, as in
   0x100759C0 below) the call is left as the tree writes it.
9. **A matrix-stack pop through a local copy of the stack's owner.**
   ```
   MSTACK[--MTOP] = m;           // MSTACK/MTOP: (g_pProcessor + 0xA10/0xA90); g_pProcessor and the top reloaded after the store
   { char *pr = g_pProcessor_1047CF7C; ((float **)(pr + 0xA10))[--*(int *)(pr + 0xA90)] = m; }   // one load, as the game
   ```
   An inline member `void pop(float *m) { stack[--top] = m; }` called on the global gives the same bytes (0x1003DB60).
10. **A float read through an inline accessor goes through the FPU.** `v3[8] = this->field_120;` (float to float) is
    an integer move (rule 3); `v3[8] = gx();` with `float gx() { return field_120; }` inline gives the game's
    `fld [edi+0x120]; fstp [esi+0x20]` (0x100431B0). A `(float)` cast or a copy through `*(float *)&` does not;
   two float locals (`float x = field_120; ... v3[8] = x;`) give the FPU copies but store the second one after the
   next global load (6 lines apart).
11. **Float arithmetic in float.** The decompiler's double locals and constants (`double v7; v7 = (double)*a3 - 0.5;`)
    give qword FPU operands and other spills; float locals, `(float)` casts and `f` literals give the game's dword
    operands (0x1011BBD0: 30 lines to 6). The comparison of rule 4 (`< 4.0f`) is the same rule (0x10033800).
12. **The decompiler's frame block is not the frame.** Two 64-byte locals written as `unsigned char xi_frame[0x528]`
    with references into it give a 0x528-byte frame; as two plain arrays the compiler lays them out as the game
    (`sub esp, 0x80`, 0x10033800).
13. **An indexed loop keeps two pointers; a pointer walk gets one.** `for (i = 0; i < 8; i++) f(col + 4 * i, p + 32 + 4 * i);`
    gives the game's two induction registers (`lea ebx, [esp+0x14]; lea edi, [esi+0x20]; ... add edi, 4; add ebx, 4`);
    the same loop over two walking pointers (`d += 4; s += 4;`) is compiled with one pointer and a difference
    (`sub ebx, eax; lea ecx, [ebx+edi]`) (0x1003D9A0).
14. **The order of independent statements decides the schedule.** In 0x1011BBD0 the three statements `v22 = (float)(a2 / 64);`,
    `v12 = 0;`, `v13 = (unsigned int *)(self_ + 4);` were tried in all six orders; only that one gives the game's
    `lea ebp, [esi+0x10]` before `xor edi, edi` (each of the others: 4 lines apart).
15. **A post-increment in the index places the increment before the store**: `v19 = v21[result++]; *v17 = v19;`
    gives `mov edx, [esp+eax*4]; inc eax; mov [ecx], edx`; the `for (...; ++result)` form increments after the store
    (0x1011BBD0).

The rules of round 118 used again: 1 (every file), 3 (0x10076520), 4/11 (0x10033800), 5 (0x10025C40, 0x10033800,
0x100472F0), 7 (0x10025C40, 0x10033800).

## What this changes in round 118's observations

The first observation there (for runs of `SetRenderState(c, c)` the game reloads the global into the device's own
register each time, and no source form tried gave it) now has a source form that gives it under this compiler:
rule 8. It matched 0x1003DB60 and 0x1003E9A0, two of the three functions that observation named. That observation
therefore no longer points at another code generator. The second one (0x1006AA30 keeps EDX live across the call to
0x1006A560) is not examined further in this round: no source tried keeps a caller-saved register live across a call,
and the cause is not known.

## Not matched

The closest source tried; the number of differing instruction lines (a unified diff of the two instruction
streams, operands relocated by match.py blanked); the first difference.

| address | lines | difference (ours / the game's) | tried |
|---|---|---|---|
| 0x1006A020 Gfx_CreateVertexBuffer (#118) | 2 | the returned buffer is loaded after `pop edi; pop esi` in ours (`mov eax, [esp+4]`), before them in the game (`mov eax, [esp+0xc]`) | rule 8 (it gave the game's ECX and EDI before); the buffer as a separate local (put in the dead first parameter's slot, as the game), as the parameter, through a pointer local, as `unsigned int` or the interface type, a result variable, an inner block: 17 forms |
| 0x10017310 Unit10014350_Draw (#119) | 2 | `mov edi, [esp+0xc]` (the vertex array) after `xor eax, eax` in ours, before it in the game | float literals and parenthesized products, a plain `for` over the index, the array as a parameter type or a local pointer, the index declared in the loop: 16 forms; from 27 lines |
| 0x100759C0 TextureHolder_BindTinted (#118) | 46 | the `*this == 0` return is placed at the end in the game, inline after the test in ours; the game's three SetTexture returns use three register forms | rule 8 on every call; not re-written by hand |
| 0x10075280 Gfx_InitCubeMapTextures (#118) | 113 | the zero kept in EBX in ours, ESI in the game; ours keeps `32 << v0` in ESI across the call to 0x1000AD70, the game computes it again after it | rule 8 on the five SetRenderState; the frame as plain locals; the four vertex floats by one chain; 9 placements of the edge length |
| 0x10076030 (#118) | 277 | sweep form with rule 8 | not re-written by hand |
| 0x1006C300, 0x1006C530 (#118) | 160, 156 | sweep form with rule 8 / float literals | not re-written by hand |
| 0x10040220 (#118) | 510 | sweep form | not re-written by hand |
| 0x1003F440 (#118) | 374 | sweep form with rule 8 | not re-written by hand |
| 0x10046070, 0x100467E0, 0x100426F0, 0x10043720, 0x10054D70, 0x10075700 (#118) | | the sweep's source does not compile (`__thiscall` on a non-member, `ABS` / `fpatan` / `sin` / `cos` undeclared) | not converted by hand |
| 0x100091B0, 0x1000A3B0, 0x1000A6E0, 0x1000C0B0 (#118) | | as round 118 | not re-attempted |

#119, by the sweep's source with rule 1, rule 8 and the float forms of rule 11 applied mechanically (the smallest of
those variants), none re-written by hand: 0x1016DDF0 93 (the game copies the y and z of each vertex through the FPU
and the x by integer move, ours all by integer move; the identity matrix stored as immediates in the game),
0x10040E70 105, 0x1001DD00 108, 0x10026C70 112, 0x1006AA30 116 (the register live across a call, above),
0x100263F0 123, 0x10041160 125, 0x100266C0 131, 0x10163400 131, 0x10038720 132, 0x10026AD0 137, 0x10184C90 154,
0x1001D3B0 183, 0x10003D70 196, 0x100295A0 210, 0x10025870 216, 0x10012F40 245, 0x1000A820 268, 0x10038A80 295,
0x10040670 322, 0x10182F10 335, 0x10033A70 357, 0x1021D4C0 367, 0x100132B0 476, 0x10183DA0 525, 0x1002B6E0 628,
0x1011F300 757, 0x10182700 824, 0x10184FE0 1253, 0x1002DC10 2456. The sweep's source does not compile for 0x10001520
(`tagMSG` incomplete), 0x10003890 and 0x1002A3B0 (`__thiscall` on a non-member), 0x10010D20 and 0x10086210 (C2440
conversions), 0x10011A00 (`_SYSTEMTIME` incomplete), 0x10025260 (a vtable member the shim's struct lacks),
0x10174EC0 and 0x10185CA0 (`sin`, `cos`, `atan2` undeclared); 0x10038E10 is left out by the sweep (no wrapper for
Camera_GetFrameScale_v1). Already attempted in round 118 and not re-attempted: 0x1000C6E0, 0x10002040, 0x1006B2B0,
0x10026990.
