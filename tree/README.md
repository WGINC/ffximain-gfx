# FFXiMain.dll graphics code: clean C++ (build 2026-09-03)

Reconstructed from Hex-Rays output and verified function by function against the original in an
emulator (see `../docs/METHOD.md`). Every function carries:

    // FUNCTION: FFXIMAIN 0xADDR  (placed by <tree manifest | IDB name | typed this>)
    // GATE: PASS -- N trials, R returned normally, D D3D calls compared

(777 functions: 774 PASS; `// NOT VERIFIED` marks the 3 that are not, with their verdict: 2 INCONCLUSIVE, 1 FAIL).

## Layout
- `include/<Class>.hpp`: one class per header, `#pragma pack(1)`, a `static_assert` on every field
  offset and on `sizeof`. `field_XXX` = not yet named (XXX = hex offset).
- `src/<Class>.cpp`: member functions, plus free functions of that source unit. `src/<module>.cpp`: dancer (C) modules. `src/unit_<addr>[_<prefix>].cpp`: free functions from an unidentified source unit, grouped in link order. See `../docs/METHOD.md` (Placement).
- `include/gfx_orig.hpp`: `orig::Name(...)` calls the game's own (not reconstructed) code by
  address; `XI_CODE(Name)` is a game function's address used as a value.
- `include/gfx_globals.hpp`: game globals by IDB name, bound to their addresses.
- `include/xi_d3d8.hpp`: Direct3D 8 interfaces (COM layout, `dev->SetRenderState(...)`) and enums.
- `include/xi_decomp.hpp`: the few decompiler helper macros the bodies still use (`LOBYTE`, `qmemcpy`, ...).
- `at<T>(this, off)`: an access at an offset with no member of that width/kind.

## Build
    ./build.sh      # MinGW-w64 i686 -> build/gfx_recon.dll (freestanding, imports nothing)

Flags that matter: `-fno-strict-aliasing -fwrapv -fno-delete-null-pointer-checks` (decompiled code
reinterprets memory and relies on x86 wrap-around), `-mfpmath=387 -fexcess-precision=standard`
(MSVC6 x87 behaviour), `-fpermissive` (C-style pointer conversions).

## These functions run in the game's address space
Globals and `orig::` calls use the game's absolute addresses (image base 0x10000000), so this code is
meant to be loaded into the client next to FFXiMain.dll (proxy / hook DLL), not linked standalone.
