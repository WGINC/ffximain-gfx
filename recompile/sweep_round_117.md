# Sweep round for task #117: fewer LEFT_OUT

LEFT_OUT means `pipeline/vc6/sweep.py` does not convert the function at all. The `why` field gives the reason. This
round teaches the conversion the forms behind the largest reasons, for whole groups. No function was edited by hand.
Each change alters only how the code is spelled, so that VC6 takes it. The tree's computation stays the same.

Run: `sweep.py OUT.json --calls --rules --jobs 2`, the same tree before and after (4,251 functions; the tree at
081ee53, newer than round 116's 4,146). Toolchain checked first: `match.py --compid` 48 / 49 build 9044.

## Counts

| verdict | `verification/vc6_sweep.json` as committed (older tree, 4,146) | before (this tree) | after |
|---|---|---|---|
| IDENTICAL | 1,610 | 1,638 | **1,650** |
| DIFFERS | 2,088 | 2,152 | 2,278 |
| NO_COMPILE | 39 | 42 | 71 |
| LEFT_OUT | 409 | 419 | **252** |
| DIFFERS proven at the game's bytes by a rewrite rule (`rule_identical`) | 168 | 169 | 169 |
| at the game's bytes in all | 1,778 | 1,807 | **1,819** |

Movements from before to after:
- LEFT_OUT to IDENTICAL: 12.
- LEFT_OUT to DIFFERS: 126. None of these is IDENTICAL by a rule.
- LEFT_OUT to NO_COMPILE: 29.
- Nothing else moved.

Every function that was IDENTICAL, or IDENTICAL by a rule, before still is. The 42 NO_COMPILE functions from before
are all still NO_COMPILE. `match.py --all` reports all 45 files in recompile/ IDENTICAL after the change.

LEFT_OUT by reason, before and after:

| reason (`why`) | before | after | where the before group went |
|---|---|---|---|
| no wrapper (a callee taking a register its convention lacks) | 249 | 252 | unchanged, plus 3 from "type the shim lacks"; see below |
| no wrapper (a variadic wrapper with typed fixed parameters) | 10 | 0 | 8 DIFFERS, 2 NO_COMPILE |
| type the shim lacks (a callee's parameter or return type) | 92 | 0 | 9 IDENTICAL, 60 DIFFERS, 20 NO_COMPILE, 3 now stop at a register-convention callee |
| thiscall without self_ first | 40 | 0 | 3 IDENTICAL, 35 DIFFERS, 2 NO_COMPILE |
| global type the shim lacks | 28 | 0 | 23 DIFFERS, 5 NO_COMPILE |
| **total** | **419** | **252** | (252 = 249 + the 3) |

## The rules

**1. A callee's types, declared as the tree declares them** (`call_type_decls`). This covered "type the shim lacks"
(92). Before, a call through a wrapper in `gfx_orig.hpp` was taken only when every type was the shim's own. Now each
type is taken when every name in it is one of these:
- Visual C++ 6.0's own `__int8`, `__int16`, `__int32`, `__int64`. The tree's GCC build defines the first three as
  `char`, `short` and `int` (`xi_decomp.hpp`), which are the same types in VC6. GCC has `__int64` itself.
- A Windows typedef, as the tree declares it. New ones: `HIMC` (`xi_compat_win.hpp`: `struct HIMC__ *`, imm.h
  DECLARE_HANDLE), `MMRESULT` (`unsigned int`), `MSG` (`tagMSG`), `LONG` (`int`, gfx_types.hpp:269).
- A Direct3D type. The source then includes the tree's d3d8 header (xi_d3d8.hpp, as round 116 converts it). This
  covers `_D3DFORMAT` (an enum) passed by value.
- A pointer to a struct that tree/include declares, or to one of xi_compat_win.hpp's opaque types: `Iostream_init`,
  `exception`, `type_info`, and `std::codecvt_base`, which makes the source C++. The pointer is passed as its 4
  bytes. A forward declaration is all it needs.

A type held by value that is none of these still leaves the function out. Its layout would be needed, and the sweep
has no layout for it.
```
  XI_ORIG CMoProcessor * Fn(void * a0, void * a1, int a2, int a3, void * a4)  // gfx_orig.hpp (__stdcall, 0x1006E970)
  struct CMoProcessor; typedef struct CMoProcessor CMoProcessor;
  extern "C" CMoProcessor * __stdcall FUN_1006E970(void *, void *, int, int, void *);   // 0x10070040: IDENTICAL
```
```
  struct HIMC__; typedef struct HIMC__ *HIMC;  typedef int LONG;
  template <class A0> HIMC __stdcall FUN_101AFF50(A0);                       // 0x101B3F70: IDENTICAL
```
The same rule also declares a `__thiscall` function's own signature types (rule 3). `struct_names` now also reads
`struct /*VFT*/ Name` (gfx_types.hpp's vtable structs). A global's function-pointer type now has its parameter names
dropped after a `*` as well (`const MSG *lpMsg`), so the parameter name is no longer declared as a type.

**2. A variadic wrapper with typed fixed parameters.** This covered 10 of "no wrapper for": sqDebugPrintf,
Unit10017C10_NoOpVarargsA / B.

`gfx_orig.hpp` has a second variadic form, `reinterpret_cast<R (__cdecl *)(void *, ...)>(XI_GAME(A))(a0, rest...)`.
The tree passes the extra arguments with their own types through a C `...`, which is exactly what a VC6 `...` call
does: caller cleanup, default promotions. `VORIG` now reads that form as well as the `decltype(XI_ANY(rest))...` one.
```
  extern "C" int __cdecl FUN_10017D20(void *, ...);
  result = FUN_10017D20(aDeviceLost);
```
Result: 8 DIFFERS, 2 NO_COMPILE.

**3. A `__thiscall` function whose first parameter is not `void *`.** This covered "thiscall without self_ first"
(40).

The tree's first parameter of a `__thiscall` function is ECX. The function is still compiled as a member of the empty
struct `XS`, and the parameter is bound to `this` with the parameter's own type: `T self_ = (T)this;`. That is the
same register's value, for a pointer type, or for a 4-byte integer (`int`, `_DWORD`, ...). One function's first
parameter is named `a1` (0x100472F0); it is bound under that name. A first parameter that is not `void *` and not one
of those types is still left out. None is left out for this reason now.
```
  XI_API int __thiscall Gfx_SetViewMatrixFromField10(int self_)        // tree
  int XS::Gfx_SetViewMatrixFromField10(void) { int self_ = (int)this; ... }   // 0x10044210: IDENTICAL
```
Result: 3 IDENTICAL (0x10044210, 0x10044230, 0x101792A0), 35 DIFFERS, 2 NO_COMPILE.

**4. A class method that uses its fields, naming a global of any type.** This covered "global type the shim lacks"
(28).

That path took only globals whose type was the shim's own. Its globals are now declared by `glob_lines`, as for every
other function. That means:
- a forward declaration for a pointer;
- the one-int placeholder for a type held by value: code reading its fields fails to compile, and code using its
  size gives other bytes, so neither can match by chance;
- `XI_PIECEA` for an array global (round 116, rule 10).

Result: 23 DIFFERS, 5 NO_COMPILE.

**5. A global of a Direct3D type held by value** gets the tree's d3d8 header (`type_decls`). Before, it got the
one-int placeholder, so its fields did not compile: `D3DVIEWPORT8 g_viewport_1B70` read as `.Y`, `.Height`. Its
layout is the header's (from the SDK's d3d8.h). This rule moved 2 of the functions above from NO_COMPILE to DIFFERS.

**6. Names the shim lacked, which the converted functions use.**
- From `<string.h>`, which the tree includes (xi_decomp.hpp): `strlen`, `strcat`, `strcmp`, `memcmp`. They are
  declared as VC6's STRING.H declares them for the static CRT, with C linkage, and made intrinsic as memcpy is.
- From xi_decomp.hpp, verbatim: `_QWORD`, `__PAIR64__`, `__SPAIR64__`.
- `COERCE_FLOAT(v)`, which is xi_decomp.hpp's `xi_bitcast<float>((uint32_t)(v))` written through the shim's
  `xi_castbits_`.

None of the functions that were IDENTICAL or NO_COMPILE before changed verdict through these.

## What is left out, and why

**All 252 functions still left out call a game function that takes an argument in EAX** (or, in one case, EDX alone). Those callees,
123 distinct, are listed in `gfx_orig.hpp` as asm wrappers (`hx2cpp.ghidra_reg_call`). The storage is the IDB's; the
pop count comes from `purges.json`.

Grouped by the wrapper's registers and stack (the 3 functions that moved here from "type the shim lacks" included):

| registers | stack | functions |
|---|---|---|
| EAX + ECX | callee pops | 113 |
| EAX | caller pops | 81 |
| EAX | none | 46 (22 of them return in ST0: Camera_GetFrameScale_v1) |
| EAX | callee pops | 7 |
| EAX + EDX | caller pops | 4 |
| EDX alone | caller pops | 1 |

Every group but the last has EAX. No Visual C++ 6.0 convention passes an argument in EAX:
- `__cdecl` and `__stdcall` pass none in registers;
- `__thiscall` uses ECX;
- `__fastcall` uses ECX and EDX, and its callee pops.

The last group (EDX alone, caller pops) is not a VC6 convention either. Writing the call with `__asm` would spell the
asm, not code the compiler generates. Whether the game's callers were compiled from such source is not known. The
sweep therefore keeps these out, as RECOMPILE.md already says.

**The 29 new NO_COMPILE**, by cl's first error. These are already the groups round 116 left:

| error | functions | why it is left |
|---|---|---|
| C4234: a virtual-call spelling `_vcalls` does not take | 12 | as round 116's C4234 group: a slot through an IDB vtable struct, pointer arithmetic, or a vtable loaded earlier into a local (a data-flow fact) |
| C2027 / C2036: a struct used by its layout or size | 10 | `D3DHolder` (3), `CDx_vtbl`, `Vt_1033BE90`, `Vt_1033C8F0`, `Vt_1032BAA8`, `tagRECT`, `tagCANDIDATEFORM`, `_iobuf` (FILE). Each needs gfx_types.hpp's (or the SDK's) layout in the shim, which this round does not carry. |
| C2039: a field through an IDB vtable struct | 1 | 0x10025260, `Vt_1032A460_vtbl` forward-declared only |
| C2440 / C2664: a conversion permissive does not take | 3 | 0x10010D20 (`unsigned int *` to `unsigned int` inside an expression), 0x10086210 (an array to `__int16 *`), 0x10007EF0 (two calls on the line) |
| C2065 `sin` | 2 | 0x10043720, 0x10075700. Round 116's x87 group: whether VC6's `sin` compiles to `fsin` or to `__CIsin` is not established. |
| C2101 | 1 | 0x100431B0, `&` on a constant |

## Not changed (no fact for the change)

A variadic wrapper of the `decltype(XI_ANY(rest))...` form passes each extra argument as 4 bytes (`XI_ANY`). The sweep
spells that call as `...`, where a `float` or `double` argument is pushed as 8 bytes. That gives other bytes, never a
false match. Spelling it as the tree does needs the argument's own conversion to `XI_ANY`, which is not a 4-byte cast
for a floating value; this round does not change it.
