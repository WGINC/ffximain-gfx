# Sweep round for task #116: fewer NO_COMPILE

`pipeline/vc6/sweep.py` turns each tree function into a source for Visual C++ 6.0 SP6 with the Processor Pack's
code generator (48/49 build 9044; `match.py --compid`). NO_COMPILE means the converted source does not compile.
This round changes how the conversion spells what the tree's GCC build accepts and VC6 does not. Each change is
made for a whole group of functions, chosen by the group's first VC6 error. No function was edited by hand.

Run: `sweep.py OUT.json --calls --rules --jobs 2`, the same tree before and after (4,146 functions). The sweep now
records cl's first error for each NO_COMPILE function in the `error` field. The table below comes from that record.

## Counts

| verdict | `verification/vc6_sweep.json` as committed (older tree, 4,121) | before (this tree) | after |
|---|---|---|---|
| IDENTICAL | 1,457 | 1,461 | **1,610** |
| IDENTICAL_UNCOMPARED | 5 | 5 | 0 (the 5 are IDENTICAL now, see globals below) |
| DIFFERS | 1,656 | 1,668 | 2,088 |
| NO_COMPILE | 604 | 603 | **39** |
| LEFT_OUT | 399 | 409 | 409 |
| DIFFERS proven at the game's bytes by a rewrite rule (`rule_identical`) | 158 | 159 | 168 |
| at the game's bytes in all | 1,620 | 1,625 | **1,778** |

Movements from before to after: NO_COMPILE to IDENTICAL 143, NO_COMPILE to DIFFERS 421 (9 of these are IDENTICAL
by a rule), IDENTICAL_UNCOMPARED to IDENTICAL 5, DIFFERS to IDENTICAL 1 (memcpy linkage, below). Every function
that was IDENTICAL, IDENTICAL_UNCOMPARED or IDENTICAL by a rule before still is. `match.py --all` reports all 45
files in recompile/ IDENTICAL, before and after.

## The rules, by error group

Each rule changes how the code is spelled, never what it computes. The verdict still comes only from the byte
comparison. A wrong spelling can only give other bytes; it cannot make a source match the game.

**1. The implicit conversions GCC's `-fpermissive` accepts, written as casts** (`permissive`). This covered C2440 at
`=` / `return` / initialisation: 289 functions; C2664, an argument: 8.

The tree is built with `-fpermissive` (`tree/build.sh`). Under it GCC accepts an implicit conversion between two
pointer types, or between a pointer and an integer, and makes the conversion a cast would make. VC6's C++ refuses
it. After the first compile, each such conversion cl reports is written as a cast to the type cl names:
- the conversion is pointer to pointer, pointer to or from a 4-byte integer, or an integer to an enum;
- it sits at an assignment, an initialisation or a return written whole on one line, one per line;
- or it is an argument of a call cl names (by name, or the one function-pointer call on the line).

On this flat 32-bit target the cast gives the same value. The sweep repeats compile and cast while cl finds more
(2 rounds here). In all, 721 casts in 315 functions; the `casts` field records the number per function.
```
  return self_;                                  // C2440 'return' : 'void *' to 'unsigned int'
  return (unsigned int)(self_);                  // 0x1004E210: IDENTICAL
  *(uint8_t ***)self_ = &PTR_FUN_1032965c;       // C2440 '=' : 'void ** ' to 'unsigned char ** '
  *(uint8_t ***)self_ = (unsigned char **)(&PTR_FUN_1032965c);   // 0x100035F0: IDENTICAL
```
Result for the C2440 groups: 90 IDENTICAL, 6 more IDENTICAL by a rule, 3 still NO_COMPILE (the conversion sits
inside an `if` condition, or the statement spans two lines). C2664: 2 IDENTICAL, 5 DIFFERS, 1 still NO_COMPILE.

**2. A C source naming a Direct3D interface** (48 functions: cl gave no message of the source's own; the errors
were in the header).

`xi_d3d8.hpp` (the tree's header from the SDK's d3d8.h) is C++, and a `__cdecl` function is compiled as C. A C
source now includes `xi_d3d8_vc6c.h` (`d3d_c`). That is the same header with:
- a typedef for each struct and enum, which C needs;
- the interfaces' inline C++ wrappers and the enum `++`/`--` operators left out (C has those on an enum);
- the two `static const` formats written as macros of the same value.

The structs, fields and vtable slots are the header's own. A source that calls a wrapper (`dev->SetRenderState(...)`)
is compiled as C++, as the tree is. Forward declarations of structs now carry a typedef too
(`struct CDx; typedef struct CDx CDx;`), which C needs for `(CDx * *)`.
```
  return (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC));   // 0x1025D750: IDENTICAL
```
Result: 13 IDENTICAL, 35 DIFFERS.

**3. Virtual calls: more decompiler spellings as the stand-in class call** (`_vcalls`). This covered C4234 (`__thiscall`
in a function-pointer type, which VC6 cannot spell): 45 functions.

The call is still taken only where the slot is read from the vtable of the object passed as `this`. That object
must be free of side effects, because the decompiled form reads it twice and the call reads it once. The slot's
offset must be a multiple of 4. New forms:
- the vtable read through a cast, `*(int *)X`;
- a name declared (a local, a parameter or a global) as a pointer to a 4-byte integer;
- a loaded object pointer, `**(int **)(E)`;
- Hex-Rays' slot-address form `(*(R (__thiscall **)(T, P...))(*(_DWORD *)X + OFF))(X, ...)`.

`*X + OFF` is accepted only where X's declared type makes it a byte offset. Under the rule as it was, the old form
`*X + OFF` was taken for any X.
```
  ((int (__thiscall *)(void *, XI_ANY))((**(code **)(*(int *)self_ + 0x18))))((void *)(self_),1);
  ((XV6_7db304 *)(self_))->f(1);
```
Result: 9 IDENTICAL, 2 IDENTICAL by a rule, 26 DIFFERS, 8 still NO_COMPILE (see the remaining groups).

**4. The shim's own inline functions are not the function** (`SHIM_HELPER`). This covered "N functions in the object":
72 functions.

An instantiated `XI_ANY` constructor or `xi_castbits_` is emitted beside the function. The function is now the one
code symbol left once those are set aside. A call to a helper is a relocation whose symbol names no address, so it
is reported and never compared as equal. Result: all 72 DIFFERS.

**5. A method's own view of `this`.** This covered C2371, `self_` redefined: 25 functions.

A method whose body begins `T * self = reinterpret_cast<T *>(this);` keeps that line as its binding of `self_`. The
generated `void *self_ = this;` is no longer added before it. Result: 10 IDENTICAL, 14 DIFFERS, 1 still NO_COMPILE
(0x100CF030 now stops at a C4234 form of group 3, case (c) below).

**6. Globals holding a function pointer** (`FGLOB`, `glob_lines`). This covered undeclared `Sleep`, `CloseHandle`,
`EnterCriticalSection`, ...: 18 functions.

`gfx_globals.hpp`'s `typedef R (CC *NAME_t)(P); #define NAME XI_EA(NAME_t, ADDR)` (an import's IAT slot, a vtable
pointer) was not parsed. It is now declared at its address, as the other globals are: `#define Sleep (*(Sleep_t
*)0x103290F8u)`. The call is `call [0x103290F8]`, with the game's address in the instruction. The parameter types'
Windows typedefs come from the SDK, as `xi_compat_win.hpp` gives them (UINT, LPSTR, LPCSTR, LPSYSTEMTIME, ...). The
5 IDENTICAL_UNCOMPARED functions called `timeGetTime` through the shim's `dllimport` symbol, whose address was not
compared. Their call now names 0x10329400, which is compared, so they are IDENTICAL. Result: all 18 DIFFERS.

**7. Names a body uses that the shim lacked.**
- From `xi_ghidra.hpp` / `xi_decomp.hpp`, verbatim: CONCAT11/12/21/22/43/44, ZEXT/SEXT, LOBYTE ... SHIDWORD, sbyte,
  longlong, qword, xi_bool.
- Defined only in a source that uses them (`HELPERS`):
  - `builtin_strncpy` and `xi_k4` / `xi_k8`, as the tree defines them;
  - SUBnm with `xi_b32` / `xi_b64` and their float overload, which make the source C++, since C cannot take a
    float's encoding by overload.
- In C, `nullptr` is `((void *)0)`.
- `__builtin_isunordered(a, b)` is `(_isnan(a) || _isnan(b))`. FLOAT.H: `_isnan` is nonzero for a NaN; GCC: the
  builtin is true when either argument is a NaN.
- Windows types in a body: LPDWORD, PVOID, LPCRITICAL_SECTION (gfx_types.hpp / the SDK).
- `NAME_t`, a global's own typedef, when the body names it.
- A struct or class from tree/include that a body names through a pointer gets a forward declaration. Code that
  reads its fields still fails to compile.

Result for the undeclared-name groups (C2065 other than fsin, fpatan, memset and the group 6 imports): 15 IDENTICAL,
1 IDENTICAL by a rule, 31 DIFFERS, 4 still NO_COMPILE. Those 4 are two Vt_ structs whose size the code needs
(0x10062F90, 0x100630B0), 0x10003040 (D3DHolder, which now stops on its `vftable` call, group 3 case (a)) and
0x10185CA0 (atan2).

**8. The CRT's memcpy / memset / strcpy with C linkage in C++.**

The shim declared memcpy without `extern "C"`, so in a C++ source `#pragma intrinsic(memcpy)` had no effect (cl
C4162). The source then called `?memcpy@@...` instead of the intrinsic. These are now `extern "C"` and intrinsic,
as the CRT's are. This rule fixed undeclared `memset` (9: 7 DIFFERS, 2 now stop at another error), and one DIFFERS
function became IDENTICAL.

**9. Literals.** This covered "bad suffix on number": 9 functions.
- `ull` / `ll` (C++) becomes `ui64` / `i64`: the same types in VC6.
- A hexadecimal floating literal (C++17; VC6 has none) becomes the decimal Python's `repr` gives for that double.
  At every sweep's start, the compiler itself is checked to read it back: all such decimals are compiled into one
  array, and the `.data` bytes are compared with the double (`hexf_check`). A literal that read back otherwise
  would leave its functions out. All 23 read back as written.

```
  0x1.0001000000000p-16   ->   1.5259021893143654e-05
```
Result: all 9 DIFFERS.

**10. A piece of an array global** (C2101, `&` on constant: 2 functions).

VC6 refuses `&` of `*(char (*)[1])0x1034FB65u` (tested). `XI_PIECE(G, o, s)` on an array global now takes the piece
from the array's first byte, `(char *)(G)`, which is the same address. Result: 2 DIFFERS.

The sweep also records each NO_COMPILE function's first error (`error`) and caches cl's messages with the object
cache.

## The remaining 39 NO_COMPILE, by group

Each group below either needs a fact this round does not have, or is too small or too irregular to have a whole-group
rule worth writing.

| group | functions | why it is left |
|---|---|---|
| `fsin` / `fpatan` / `cos` / `atan2` undeclared | 8 / 8 / 1 / 1 | These are the tree's x87 instructions on an 80-bit `long double` (`xi_ghidra.hpp`: inline `fsin`, `fpatan`). VC6 has no 80-bit type (its `long double` is 64-bit). Whether VC6's `sin` / `atan2` intrinsic compiles to the bare instruction or to the CRT's `__CIsin` / `__CIatan2` is not established here. No spelling is known to compute the same; it needs that fact. Addresses: fsin 0x10027B80, 0x10027BD0, 0x10027C20, 0x1006DCF0, 0x1006E380, 0x1006FAE0, 0x1006FC40, 0x100830C0; fpatan 0x10015490, 0x1005E9C0, 0x1005EEF0, 0x10159B10, 0x10177420, 0x1024F0B0, 0x10278E90; cos 0x10174EC0; atan2 0x10185CA0. |
| C4234: other virtual-call spellings | 11 | (a) Through the IDB's vtable structs: `(*(CMoTask_vtbl * *)this)[1].vf10_CMoTask_vf010` (0x1003EF90, 0x10052DE0, 0x10053140) or `g_pD3DHolder->vftable` (0x10003040). These need the structs' layouts from gfx_types.hpp, and their fields are `__thiscall` pointers too. (b) The slot as pointer arithmetic, `(T **)(*(void **)X) + 9` (0x10042020, 0x10054D70). (c) The vtable loaded into a local earlier, `(*(R (__thiscall **)(P))(v2 + 508))(self, ...)` (0x100C5D60, 0x100CE480, 0x100CF030, 0x100D04C0, 0x100D06F0). Whether that local still holds `*self` at the call is a data-flow fact, not a spelling. |
| C2664 through a function-pointer call with two such calls on the line | 2 | 0x100096B0 (the call spans lines), 0x1000A6E0 (more than one call through a cast on the line) |
| C2440 inside an `if` condition / over two lines | 2 | 0x10029F70, 0x101ACC30 |
| struct used by its size or fields | 3 | `Vt_1032BA60` / `Vt_1032B9D0` (pointer arithmetic: 0x10062F90, 0x100630B0); `D3DHolder->_rest` (0x10002F20). These need gfx_types.hpp's layouts. |
| single cases | 4 | 0x10016870 (`WINDOWPLACEMENT` by value: needs tagPOINT/tagRECT); 0x10160A50 (C2446, a comparison of two pointer types); 0x10189120 (a write to a 3-byte piece, which the tree does through `xi_u24`); 0x101A0770 (C2143 in the signature) |

## Found while doing this, not changed (no fact for the change)

- In C the shim's `bool` is `unsigned char`, so a value is not normalised to 0/1 as the tree's C++ `bool` is. Only
  one C source in this sweep names `bool`.
- The shim's `undefined4` is `unsigned int`. A float copied into it is converted, while the tree's `undefined4`
  (`xi_ghidra_shared.hpp`) keeps the float's bits. The conversion would make the compiler emit code the game's
  bytes lack, so it can only give DIFFERS; it is not a semantic match.
- A VC6 `long double` is 64-bit; the tree's is 80-bit. This affects the 556 sources that use one, but no verdict
  depends on it unless their bytes match.
