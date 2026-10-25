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
