# Recompiling to the game's bytes (byte-identical)

The verified tree (`tree/`) is equivalent to the game's code: every function PASSes the gate. Byte-identical is a
further, separate goal: source that the game's own compiler turns into exactly the bytes in FFXiMain.dll. This page
says what is established, from where, and what is not.

## Which compiler built FFXiMain.dll

**Source: the DLL's Rich header** (written by the linker: one entry per tool and build, with the number of objects
each made) and **the @comp.id each compiler stamps into its objects** (a COFF symbol, `prodid << 16 | build`), read
from objects compiled here. The product numbers are matched by compiling with each candidate and reading its stamp,
not taken from a published table.

| Rich header entry (prodid, build: objects) | What makes it (tested here) |
|---|---|
| 48 / 49, 9044: 208 C, 799 C++ | Visual C++ 6.0 SP6 front ends (cl.exe 12.0.8804, c1.dll / c1xx.dll 12.0.9782) with the Processor Pack's code generator (vcpp5.exe: c2.dll 13.0.9044). `match.py --compid` prints C 48 / C++ 49 build 9044. |
| 10 / 11, 9782: 176 C, 13 C++ | **Not reproduced.** The CD's compiler (12.0.8168) stamps 10 / 11 build 8168. SP6's front ends refuse the CD's c2.dll ("C1900: Il mismatch between 'P1' version '19991026' and 'P2' version '19970710'"), and this archive's SP6 holds no c2.dll of its own. |
| 28 / 29, 9178: 5 + 77 objects; 25, 9210; 49, 8943; 11, 8047; 12, 7291; 14, 7299; 18, 8444; 19, 8022 / 8034 | Not tested: no tool in this archive stamps them. |
| 4, 8447 (the linker; the PE header says linker 6.00) | Not tested. |

So 1,007 of the DLL's objects come from the toolchain `setup_vc6.sh` assembles. Which objects hold which functions
is not in the Rich header; a function is known to be one of them only when its recompile is identical.

## The tools

- `pipeline/vc6/setup_vc6.sh`: assembles that toolchain from the owner's `en_vs6_ent.zip` into `$VC6` (outside the
  repository: the compiler, like the game DLL, never enters it). Runs under Wine.
- `pipeline/vc6/match.py SRC.cpp [--flags ...]`: compiles with it (`/O2` by default, plus `/Gy`, which only puts each
  function in its own section) and compares every function marked `// FUNCTION: FFXIMAIN 0xADDR` with the game's
  bytes: every byte outside a relocation equal, each relocation's target equal where the symbol names an address
  (reported otherwise), and the length the game function's up to alignment padding.

## Matched functions (`recompile/`)

`recompile/*.cpp` hold the sources that compile to the game's bytes; `match.py --all` checks every one (exit 1 unless
all are IDENTICAL). 25 Oct: 14 functions in `recompile/small_math.cpp` -- the identity matrix and row copy below,
Vec3/Vec4 copy, add, subtract and scale, a `return this`, an empty function, Mat4_SetTranslation, two clearing
methods, a counter reset and Camera_GetFrameScale (a global read through a DIR32 relocation, its 1.0f constant
reported, not compared). What the bytes showed beyond the behaviour: 0x10126350 begins `mov eax, ecx`, which is a
constructor returning `this` (as MSVC constructors do), its stores in the reverse order a chained assignment gives;
0x101BA5C0 ends `mov al, 1`, a `bool` return.

## The tree's own code, compiled as it is (`pipeline/vc6/sweep.py --calls`)

Each tree function in the `XI_API` form, and each class method, 4121 in all, is put in its own file with only mechanical changes: a header
of the tree's type names; `XI_API` dropped; `__cdecl`/`__stdcall` compiled as C (C++ when it calls a `__thiscall`
function), `__thiscall` as a member of an empty struct with `self_` bound to `this`; each global as `(*(type *)address)`
(the compiler writes the absolute address, the game's own relocated value at its preferred base); each `orig::` call
as a direct call of an external named `FUN_<address>` with the wrapper's convention and parameter types (in C an
`XI_ANY` parameter is `unsigned int`, in C++ a template parameter deduced from the argument, so the call passes the
argument's own type; a wrong declaration can only make the bytes differ, never match). Compiled at `/O2`, compared
byte for byte and each call by its target:

| verdict | functions |
|---|---|
| IDENTICAL | **1433** (every relocation compared: a call's target, a global's address; a function with one whose symbol names no address would be IDENTICAL_UNCOMPARED, and there are none) |
| DIFFERS | 1529 |
| NO_COMPILE | 582 (the converted source does not compile) |
| LEFT_OUT | 577 (132 XI_ANY in the body, 86 type the shim lacks, 62 variadic with XI_ANY in C++, 40 thiscall without self_ first) |

`verification/vc6_sweep.json` has every verdict. The largest identical self-contained one, 0x101200B0, is 61
instructions. Where the DIFFERS differ is below (`differs.py`).
Of the tree's 4,280 functions, 1433 (33%) compile to the game's bytes as they are. A method that uses its class's fields compiles against a struct of those fields (from tree/include, its methods and `static_assert`s left out); a global of a type the header lacks gets a forward declaration (a pointer) or a one-int placeholder (by value: reading its fields fails to compile, using its size gives other bytes); a variadic call (`sqDebugPrintf`) is a caller-cleanup `...` call as the game's. The tree's helpers: `xi_frame` locals with their alignment attribute dropped (compiled as C++, for the references); `XI_CODE(name)` the function's address as a number; `XI_PIECE` a sized piece through a cast; `XI_CALL_ST0(0x10311C2C)(x)` the cast `(__int64)x`, because 0x10311C2C is LIBC.LIB's `__ftol` byte for byte (the 39 bytes, compared at every sweep's start; its relocation is then compared against that address). Not converted: the 159 functions whose signatures use register conventions (`__attribute__((regparm))`, which VC6 cannot declare) and bodies that still use `XI_ANY`.

## Where the DIFFERS differ, and the first rewrite rules (25 Oct)

`pipeline/vc6/differs.py SWEEP.json KEEP_DIR OUT.json` disassembles both sides of each DIFFERS function (ours from the
object, the game's over its extent) and puts it in one class, from the bytes alone:

| class | functions | what the two instruction streams share |
|---|---|---|
| OPERANDS | 126 | the same mnemonics in the same order; registers, displacements or immediates differ |
| ORDER | 14 | the same instructions (targets and relocated operands blanked), in another order |
| MNEMONICS | 38 | the same mnemonics in another order, operands differing too |
| SHAPE | 1,323 | different instructions |
| UNREAD | 28 | the extent is in pieces, or no object |

The frame (`push ebp; mov ebp, esp`) differs in 3. Among SHAPE, ours most often has an `or`/`and` the game lacks
(318 / 262 functions), and the game a `test` where ours has a `cmp`. The smallest of each, read side by side:
- **A register piece.** Ghidra writes what a `setne al` or `mov ax, ...` leaves in EAX as
  `(HIGH << 8) | (uint8_t)(LOW)`, HIGH being what the register held. Compiled as written, that rebuilds EAX
  (`and`, `xor cl, cl`, `or`). The game only sets the low piece: 0x1000D660 is `mov eax, [ecx+0x18]; test eax, eax;
  setne al; ret`.
- **A comparison turned round.** Ghidra writes `x >= 0` as `-1 < x`. VC6 compiles that to `cmp eax, -1; setg`, and
  the game has `test eax, eax; setge` (0x1019FA90).

So the sweep has rewrite rules (`--rules`). Each is a mechanical change of how the code is written, not of what it
computes, tried on each DIFFERS function. A result is recorded (`rule_identical` in the sweep's output) only where
match.py's comparison says IDENTICAL; the verdict of the tree's code as written is left as it is.
- `ret8` / `ret16`: every `return (HIGH << 8|16) | (uintN_t)(LOW);` becomes `return LOW;`, and the function
  returns `unsigned char` / `unsigned short`. What the game's bytes then show is that a source returning only that
  piece compiles to them; the register's upper part is whatever the compiler left there, as in the game.
- `ge0`: `-1 < X` becomes `X >= 0`.
- `cmpk`: `(C < X)` becomes `(X >= C+1)` and `(X < C)` becomes `(X <= C-1)`, for an integer literal C. VC6 compiles
  each form to a different compare-and-jump (0x10257FF0: the game has `cmp al, 2; jl`, the decompiler's `'\x01' <
  *p` gives `cmp al, 1; jle`). Each site is tried both ways, every combination up to four sites.
- `retthis`: a `__thiscall` function written as returning void becomes one returning `this`. The game's
  0x1006AD90 ends with EAX = ECX (`mov eax, ecx`), which a setter that returns `*this` compiles to.

| rule | DIFFERS it applies to | IDENTICAL with it |
|---|---|---|
| ret8 | 197 | 69 |
| ret16 | 21 | 14 |
| ge0 | 87 | 18 |
| cmpk | 416 | 27 |
| ret8 with cmpk | 35 | 5 |
| retthis | 219 | 25 |
| other combinations | 20 | 0 |

That makes 158 more functions proven at the game's bytes: 1,591 of 4,121 swept. The rest of the classes are
next: each rule is found the same way, from the smallest function of a class, and kept only by the comparison.

## First results (25 Oct)

Two functions written by hand, compiled with `/O2`, identical to the game's bytes:

- 0x100279B0 (55 bytes): a 4x4 identity matrix, `this` in ECX. The stores' order follows the source's statement
  order: one assignment per element in index order matches; a chained `a = b = c = 0.0f` stores in the reverse
  order and does not.
- 0x10027CD0 (30 bytes): four floats copied from a pointer argument to `this+0x30`. Four member assignments match;
  a struct assignment makes the compiler compute `this+0x30` once (`add ecx, 48`) and does not.

The flags are established only as far as these show: `/O2` reproduces both. Other functions may need others (the
listing names its flags: `/Ogty` for `/O2`).

## What it would take for the tree

The tree's C++ is written for GCC (`tree/build.sh`): `static_assert`, `<stdint.h>`, `constexpr`, GCC attributes and
the `xi_decomp.hpp` helpers are not Visual C++ 6.0. A function's source for matching is therefore its own file,
written in the C++ that compiler takes, and kept only when `match.py` says IDENTICAL. That is the usual matching
workflow (one function at a time, the compiler deciding), and it is open-ended: each function's source must be found,
not derived, because the same behaviour has many sources and only one gives these bytes.
