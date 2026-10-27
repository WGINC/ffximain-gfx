# Matching round for tasks #118 and #119: the Direct3D-calling functions and their direct callers

Toolchain checked first: `match.py --compid` prints C 48 / C++ 49 build 9044; `match.py --all` reported every file in
recompile/ IDENTICAL before this round and after it.

## What the two tasks cover, and from where

- **#118, the Direct3D-calling functions.** `pipeline/frame_status.py` (verification/frame_set_status.json): 41
  frame-set functions with device calls, 36 in the tree, 6 already at the game's bytes (0x10009730, 0x10009740,
  0x1000A1D0, 0x1000AAB0, 0x1000CF90, 0x100464D0), 30 to do. docs/PLAN.md step 20 speaks of 57 device-calling
  functions: that is every function named by a site in data/com_calls.json (37 with proven sites, 20 with accepted
  ones); 16 of them are outside the frame set, 11 of those in the tree (0x1003D9A0, 0x1003F440, 0x10041AF0,
  0x100426F0, 0x100431B0, 0x10043720, 0x10054D70, 0x1006A020, 0x10075700, 0x10076030, 0x10076520).
- **#119, their direct callers.** Read from the game's bytes: every function (data/func_extents.json) holding a
  `call rel32` to one of the 57. 134 functions, 100 of them not themselves among the 57. By the sweep
  (verification/vc6_sweep.json at the branch head): 4 IDENTICAL, 33 not in the tree, 1 with no verdict, 62 to do
  (52 DIFFERS, 9 NO_COMPILE, 1 LEFT_OUT). No other worker's recompile/ file covers any of them.

## Result

**#118: 13 of the 30 frame-set functions at the game's bytes** (recompile/d3d_<addr>.cpp); none of the 11 outside
the frame set. **#119: 14 of the 62** (recompile/d3dcaller_<addr>.cpp). 27 functions in all, each IDENTICAL under
`match.py --all`. Every call target and global is compared (callees and globals are named by their addresses);
the only uncompared relocations are the float constants the compiler pools (`__real@...`), whose values were read
from the game at the addresses the game's bytes use (0x10329A08 = 0.5f, 0x103295D8 = 0.0f).

How each source is written: behaviour the tree's (tree/src, gate-verified); the class layouts are the tree's, cut
to the fields the function uses; the Direct3D interfaces are tree/include/xi_d3d8.hpp's, written in the SDK's C++
form (d3d8.h: `__stdcall` virtual methods in vtable order) instead of the C `lpVtbl` form; only the declarations a
file uses are copied into it (each file is self-contained, since match.py compiles it alone). Where the bytes
force a shape the tree does not show, it is listed below.

### #118, at the bytes

| address | function | what decided the bytes |
|---|---|---|
| 0x100096B0 | CDx::EndClearBegin | the flag is or-ed into the parameter itself (`a4 \|= 4`), not into a copy |
| 0x10009710 | CDx::TestCooperativeLevel | `if (hr != 0) return false; return true;` (test/sete); `return hr == 0;` gives neg/sbb/inc |
| 0x10009F20 | CDx::PushRenderTargetTexture | the surface is a separate local, which the compiler puts in the dead first parameter's slot as the game does; the return value is the depth read for the last store |
| 0x1000A5A0 | CDx::DrawTexturedScreenQuad | vertices written vertex by vertex, `(float)r[i] - 0.5f` |
| 0x1000BB90 | CDx::DrawStereoOddFrameOverlay | the local static's guard and quad as the address-named globals they are |
| 0x1000BF30 | CDx::SetLight | a `switch` on the light type; `< 0.0f` (the game compares with a float constant; the tree's `0.0` is a double) |
| 0x1000CB20, 0x1000CC00, 0x1000CD00 | CDx::DrawPrimitive, DrawIndexedPrimitive, DrawPrimitiveUP | the stereo branch written first, the plain call as the fall-through return |
| 0x1000CF20 | CDx::ApplyStereoEyeProjection | a 64-byte matrix local; the eye selected by `switch` (sub eax, 0 / dec eax); 0x10027990 (`mov eax, ecx; ret`) and 0x100279A0 (`ret`) called on it with ECX set, as the bytes show (the tree declares 0x100279A0 `__cdecl` without arguments) |
| 0x10043F40 | VertexBatch_BindAndDraw | the device read once into a local around the stride call; the null path has no `return` (the game returns EAX as the test left it: 0, the tree's `result`) |
| 0x1006C8B0 | Occlusion_ReadbackCount | the source surface in an inner block (its slot then shared with the loop counter); `sum >>= 5`; `sum = 0` in the countdown branch |
| 0x10075960 | TexRef_SetTexture | the non-null branch written first |

### #119, at the bytes

| address | function | what decided the bytes |
|---|---|---|
| 0x10009EA0, 0x10009EE0 | CDx::PushRenderTargetTexture_Flag1, _Flag0 | the offset is two shorts zeroed one by one (two word stores), not one dword |
| 0x1000A550 | CDx::DrawTexturedScreenQuadUnit | as the tree |
| 0x1001D0A0 | Model_CreateTranslucentTargets | as the tree |
| 0x1001D240 | Model_BeginTranslucentPass | the guard as a `bool` tested once; the pass-not-begun branch written first; the depth surface as `depth == 0 ? default : stack[depth]` |
| 0x1001D350 | Model_TranslucentUnbindTexture | the guard as a `bool` tested once |
| 0x10033710 | Unit100310A0_Construct | create if absent, then AddRef if present (two statements) |
| 0x10038930 | Post_CreateScreenEffectTargets | as the tree |
| 0x1003EB30 | CMoD3aElem::vf18 | the pointer at +0x104 read through an inline accessor that tests it again (the game tests it twice) |
| 0x1006C220 | CMoOcclusionMng::OcclusionMng_CreateTarget | as the tree |
| 0x1006C280 | Occlusion_QueryScreenRect | 0x1000BB10 and 0x1000BB20 called on g_pCDx (the bytes set ECX; the tree declares them `__cdecl` without arguments) |
| 0x10177B40 | Gfx_DrawQuadOutline | the closing vertex by chained assignment (`v[0].x = v[4].x = q[0].x`), the five colours by one chain |
| 0x10177C50 | Gfx_DrawLine3D | the colour as the tree's byte expression (D3DCOLOR_ARGB's form gives other bytes) |
| 0x1018B860 | XiZone::DrawScene | one call `f(p ? *p : 0)`, which the compiler duplicates into both branches (`xor eax, eax` for the 0); 0x1018B8B0 called on `this` (the bytes set ECX; the tree declares it `__stdcall`) |

## Not matched

The closest source tried, its difference from the game (the number of differing instruction lines, then the
first differences), and what was tried.

| address | lines | difference (ours / the game's) | tried |
|---|---|---|---|
| 0x100091B0 CDx::Initialize | 33 | the constants 1 and 16 swap registers (`mov edi, 1` / `mov ebx, 0x10`; the game `mov ebx, 1` / `mov edi, 0x10`, so the game stores `bl` to +0x908 where ours stores an immediate); and the `switch`'s jump table follows the code in our object, which match.py's length rule counts against it | declaration orders, the +0x908 field's type, branch forms; the CheckDeviceFormat flags as `if (hr < 0) f = 0; else f = 1;` and the separator as `strcat(v8, "/")` (both now as the game) |
| 0x1000A3B0 CDx::DrawScreenQuadAtDepth | 12 | the two integer temporaries (width, height) sit below the vertex array in ours, above it (with gaps) in the game | separate int locals, declaration orders, `(float)` inline |
| 0x1000A6E0 CDx::CreateTexture | 15 | the success return is placed after the failure return in ours, before it in the game | 12 forms of the five-format chain (`&&`, `\|\|`, nested ifs, goto, a result variable, the texture in the parameter's slot) |
| 0x1000C0B0 CDx::CompositeSceneToBackBuffer | 12 | the four UV stores come before the first SetTextureStageState call's vtable load in ours, after it in the game; the stereo counter in EDX in ours, ECX in the game | UV before and after the first calls, an initializer, inline wrappers, four forms of the counter |
| 0x1000C6E0 CDx::CreateStereoResources (#119) | 15 | the address arguments go to EAX/ECX in ours, ECX/EDX in the game | argument spellings, declaration order, the return variable |
| 0x10002040 Gfx_CopySurfaceRectToTexture (#119) | 62 | the surface pointer goes in a dead parameter slot in ours, a separate local in the game | first form only |
| 0x1006B2B0 CMoDX::CreateAndInitSystem (#119) | 6 | `mov eax, [eax]; mov ecx, eax` in the game where ours loads ECX directly | inline accessors, a local |
| 0x10026990 CYyIcon::Draw (#119) | 14 | one float copied through the FPU in ours, through EAX/ECX in the game | float locals, statement orders |
| 0x10041AF0 CMoElem_VT_1032ADE8::vf18 | 25 | after a call the game reuses EAX for g_pResourceMng, ours takes EDX | global forms, accessor forms |
| 0x1006AF00, 0x1006AF90 StateBlock apply | 2, 2 | one register: the global's load in the loop is ECX in the game, EDX in ours | 20 loop and global forms |
| 0x1003E910 YmElem_DrawD3ASegment | 9 | g_pResourceMng in EAX in the game, ECX in ours; two argument registers swapped | 30 forms |
| 0x1003E9A0 YmElem_SetD3ADrawState | 34 | `this` and the result swap ESI and EDI; the five SetRenderState calls' registers (below) | the bit test now as the game (`(unsigned char)` of the bit field); 243 call forms |
| 0x1003DB60 Sky_DrawDomeViewCentered | 28 | the five SetRenderState calls' registers (below) | the colour loop, the float temporary and the matrix pops now as the game |
| 0x100472F0 YmElem_RestoreDevice | 4 | one SetRenderState call's registers (below) | the guard and the branch order now as the game |
| 0x1006A020 Gfx_CreateVertexBuffer | 2 | the returned buffer is loaded after the pops in ours, before them in the game | 10 forms |
| 0x10040220, 0x10046070, 0x100467E0, 0x1006C300, 0x1006C530, 0x10075280, 0x100759C0, and outside the frame set 0x1003D9A0, 0x1003F440, 0x100426F0, 0x100431B0, 0x10043720, 0x10054D70, 0x10075700, 0x10076030, 0x10076520 | | | not attempted in this round |
| #119: the other 44 | | | not attempted in this round |

## Rules found (each a change of how the code is written, kept only where match.py said IDENTICAL)

1. **A global as a declared variable, not a cast address.** `(*(int *)0x1047BFA8)` makes the compiler reload the
   global for the object and again for the vtable; `extern int g_pResourceMng_1047BFA8;` loads it once, as the game
   (0x1006AF00 went from 50 bytes longer to one register apart). The symbol carries the address, so match.py still
   compares it.
   ```
   (*(IDirect3DDevice8 **)((*(int *)0x1047BFA8) + 3772))->BeginStateBlock();  // [0x1047BFA8] read twice
   extern int g; (*(IDirect3DDevice8 **)(g + 3772))->BeginStateBlock();         // mov eax,[g]; mov eax,[eax+0xebc]
   ```
2. **The frame is laid out by use, not by declaration order** (seven declaration orders of 0x10041AF0's locals gave
   the same frame; in small tests the more-referenced of two locals got the lower address, and splitting one int
   used twice into two (0x10041AF0's width and height) moved it above the float it shared a frame with); a block-scoped local can share a slot
   with another whose scope has ended (0x1006C8B0); a local whose address is taken may be put in a dead parameter's
   slot (0x10009F20).
3. **Floats:** a plain float copy is an integer move; a chained assignment (`a = b = x`) goes through the FPU and
   stores right to left (0x10177B40); an `(float)int` repeated in one block is kept on the FPU stack (0x1000C0B0).
4. **A comparison's form decides the code.** `return x == 0;` (bool) gives neg/sbb/inc, `if (x) return false;
   return true;` gives test/sete (0x10009710); `x < 0.0` compares with a double, `x < 0.0f` with a float (0x1000BF30).
5. **Branch placement follows the source:** the `if` body falls through; a condition kept in a `bool` and tested
   once puts the early `return 0` first (0x1001D350, 0x1001D240).
6. **`f(p ? *p : 0)` is one call that the compiler duplicates** into both arms, with `xor eax, eax` for the 0; the
   decompiler writes it as two calls (0x1018B860).
7. **An out-of-line function the caller gives ECX is called on the object** (0x100279A0, 0x1000BB10, 0x1000BB20,
   0x1018B8B0), whatever convention the tree declares for it.

## What the unmatched ones show (observations from the bytes; their cause is not known)

- **Consecutive device calls with constant arguments.** For five `SetRenderState(c, c)` through g_pResourceMng in a
  row, the game (0x1003DB60, 0x1003E9A0, 0x10075280) loads the global into the device's own register (EAX) each
  time and alternates the vtable register (ECX, EDX, ECX, ...). This compiler, for every source form tried (C
  `lpVtbl`, the C++ interface, the tree's cast, a struct-typed global, an inline accessor, a local), gives the first
  call the game's form and every later one `mov edx, [g]; mov eax, [edx+0xebc]; mov ecx, [eax]`. Counted over the
  whole DLL (COM-style calls with only immediate arguments that follow an earlier indirect call in the same
  function): the game has both forms, 175 of the first and 629 of the second; 23 functions hold only the first,
  most of them between 0x1003D9A0 and 0x10076520; the sweep's IDENTICAL functions hold 26 of the second and 1 of
  the first.
- **A register live across a call.** 0x1006AA30 (#119) loads the device into EDX, calls 0x1006A560 and pushes EDX
  after the call, which assumes the callee leaves EDX alone; neither `__cdecl` nor `__stdcall` promises that. No
  source compiled in this round keeps a caller-saved register live across a call.
- Both are consistent with those functions coming from a code generator other than the one setup_vc6.sh assembles
  (docs/RECOMPILE.md: the Rich header's 189 objects of build 9782 are not reproduced), but nothing here shows which
  objects those are; it is reported as what the bytes show.
