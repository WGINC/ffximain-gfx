# The graphics devkit: what it is, what exists, what is left (11 Nov 2026)

The devkit is for changing how FFXiMain.dll draws, and nothing else: its Direct3D use, the code that reaches it,
and the graphical functions that sit inside other systems. This page is the map. How to write a mod is
`docs/SDK.md`; how the kit is installed and what each setting does is `docs/KIT.md`.

## The parts

| Part | Where | State |
|---|---|---|
| The kit: loads into the game, swaps reconstructed functions in, loads mods, measures | `kit/xikit`, `kit/xiload` | Runs in the owner's game. Three game builds known (build translation, `docs/KIT.md`) |
| The reconstruction: the game's graphics functions as C++ that passes the gate | `tree/` (4,826 functions in the kit's manifest) | Gate-verified; 580 swapped in by default in the owner's game |
| The mod interface: hook a function, call the game's own, hook a device method, draw each frame, reload | `sdk/include`, `kit/xikit` (API version 3) | Tested under Wine in the kit's suite (`kit/xikit/test`) |
| Forks: a reconstructed function edited as a diff against its verified source | `sdk/fork.py` | `new`, `diff`, `gate` work; speedfix's forks are built this way |
| **The seam map: which functions are the graphics system** | `sdk/seams.py` -> `sdk/seams.json` | New 11 Nov. Below |
| **FFXiMain.pdb: the reconstruction's names, sizes and class types for a debugger** | `sdk/pdb/` | New 11 Nov. Analysed build and translated builds; read back by llvm-pdbutil and by dbghelp (Wine's). Below |
| **Mods with their own PDB** (source-level debugging in Visual Studio / WinDbg) | `sdk/build_mod.sh`, `.bat` with `XIKIT_PDB=1` | New 11 Nov. clang + lld; built and loaded in the kit's suite under Wine. Not run on Windows |
| **Visual Studio and VS Code projects** | `sdk/template/my_mod.sln`, `.vcxproj`, `.vscode/` | New 11 Nov. Files parse; the build command they run is the tested one. Not opened in either editor (no Windows machine here) |
| The Direct3D 11 device | `kit/d3d8on11` | Runs the owner's game; its own suite |
| Speedfix: the performance levers, as a mod | `kit/speedfix` | Runs in the owner's game |
| Data formats and readers | `docs/FORMATS.md` | Model, skin, texture, effect, scene, shader |
| Capture and replay of a function's inputs; picture capture and compare | `kit/xikit` (xcap), `kit/fcap` | Work under Wine and in the owner's runs |

## The seam

"Graphics" is not decided by a name or a guess. `sdk/seams.py` reads it from the game:

- **depth 0:** the function contains a Direct3D call site proven in the game (`data/render_map.json`): 215 functions.
- **depth 1:** it calls a depth-0 function directly (`call rel32` read from the code inside the function's extents):
  177 more.
- **depth n:** the fewest direct calls to the device. 1,150 of the game's 21,410 functions reach the device by direct
  calls. Calls through pointers and vtables are not followed, so "no depth" does not prove a function never draws.

`sdk/seams.json` has every function's unit, depth, whether it is in the frame set, in the tree, and hookable. Of the
depth-0 functions 185 are hookable, of depth 0 and 1 together 281. The units with the most (the full table: run
`sdk/seams.py`):

| Unit | Functions | Depth 0 | Depth 1 | In the tree | Hookable |
|---|---|---|---|---|---|
| CDx (the game's device wrapper) | 81 | 26 | 13 | 57 | 54 |
| unit_102822A0 | 68 | 21 | 0 | 44 | 44 |
| unit_10178790_ZoneMap | 170 | 14 | 5 | 75 | 72 |
| unit_100D7420 (2,763 functions, mostly not graphics) | 2763 | 12 | 16 | 690 | 652 |
| unit_10074CA0 | 74 | 8 | 3 | 32 | 32 |
| unit_100252F0 | 25 | 8 | 0 | 10 | 10 |
| sqgxTexture | 27 | 7 | 1 | 14 | 14 |
| unit_1012AC80 (2,008 functions, the menus) | 2008 | 4 | 9 | 415 | 376 |
| CXiSkeletonActor | 194 | 1 | 5 | 124 | 86 |

What this gives a mod author: a mod that stays at depth 0 and 1 and at the device's methods (`XI_DEVICE_HOOK`)
changes drawing and cannot reach game logic through the functions it replaces. A function of another system with a
graphical part (a menu that draws, an actor that tests occlusion) is in the map by its own depth, not by its unit.

## FFXiMain.pdb

`sdk/pdb/make_pdb.py OUT_DIR [kit/xikit/builds/<stamp>.json]` writes `FFXiMain.pdb` and a JSON list of every symbol
with the source of its name. It holds no game code or data.

- **Contents (analysed build 6A995428):** 21,404 functions with their sizes (4,826 named by the tree, 429 by other
  recorded evidence, 16,149 as `sub_<address>`), 4,368 named globals, 604 class/struct/union types from the tree's
  headers (CDx: 2,640 bytes, as the header asserts).
- **Build 6AE818B4 (2 Nov):** 25,585 of the 25,602 symbols carried over by the kit's build translation; 267
  functions keep their name with size 1 (the build file does not show them the same). An unnamed function is moved
  by its range's shift, which is proven only for the functions the build map matched.
- **Build 6AC37846:** 21,410 code symbols carried over; its build file has no data ranges, so no global is.
- **How it is made:** an empty image with the game's section layout and a label at each address, linked by lld,
  which writes the PDB; the types come from compiling the tree's headers with clang. No PDB bytes are written by
  hand.
- **Checked:** every symbol is read back at its address (llvm-pdbutil); dbghelp (Wine's) loads it and answers
  address-to-function, function-by-name, global-by-address and the size of CDx (`sdk/pdb/test_pdb.py`).
- **Loading it:** the game's DLL has no usable debug record (its debug directory entry is 80 bytes that are neither
  an NB10 nor an RSDS record), so no debugger matches the PDB to it by itself.
  - WinDbg: `.reload /i FFXiMain.dll` (its documented switch for a PDB that does not match), with the PDB's
    folder on the symbol path. Not tried here.
  - IDA and Ghidra import a PDB file for a module by hand. x64dbg: not checked.
  - Visual Studio: it loads a PDB only when the module's debug record names it. Whether it can be made to load this
    one is **not known**; it has not been tried on Windows. A mod's own PDB and the device's (`d3d8.dll`) are not
    affected: those modules carry their records.

## Mods in Visual Studio and VS Code

- Visual Studio's compiler cannot compile the reconstruction's headers (GCC attributes for the game's register
  calling conventions). clang can, in its MinGW mode, after two changes made on 11 Nov: the IDB's `wchar_t` typedef
  is now C-only in `tree/include/gfx_types.hpp` (g++ had ignored it), and `XIKIT_MOD_GLOBALS` exports its three
  variables by a linker directive under clang.
- `XIKIT_PDB=1 build_mod.sh out.dll src.cpp` (or `build_mod.bat`): clang compiles with CodeView debug info, lld
  writes `out.pdb`. On Windows this needs the llvm-mingw distribution on PATH. Forks in the mod are still compiled
  by g++, as the gate compiled them.
- `sdk/template/` has `my_mod.sln` / `my_mod.vcxproj` (a Makefile project that runs that build) and `.vscode/`
  (build tasks, attach configurations for Visual Studio's debugger and for gdb, IntelliSense paths).

## What is left, in order

1. **On Windows, once:** open the template in Visual Studio and VS Code; build with llvm-mingw; attach to the game;
   set a breakpoint in the mod. Try loading FFXiMain.pdb in Visual Studio and in WinDbg. (Owner or anyone with
   Windows; nothing here can run it.)
2. **PDB:** function prototypes and typed globals (now: names, sizes, class types; a function's type is empty);
   line numbers for the reconstruction (needs the reconstruction DLL built by clang, which would need its own gate
   run: the gate verified g++'s build).
3. **Seam:** indirect calls (vtables, pointers) in the depth; a per-unit list of what crosses the seam outward
   (the game functions graphics code calls that are not graphics): plan step "one listed boundary" (PLAN.md 40%).
4. **One definition set with the UI kit** (PLAN.md 50%): the names and types here against the teammate's; not
   started, needs their repository.
5. **The devkit as a release** (PLAN.md 55%): one zip with the kit, the SDK, the PDBs for the known builds and
   d3d8on11; a first-run check on Windows, Wine and the owner's Mac path.
6. **The source-side Direct3D 11 port** (PLAN.md 60 to 75%): the tree's device calls already go through
   `XI_BACKEND` (frame identical in two scenes); open: every device-calling function through it in the kit build,
   then a D3D11 backend beside the D3D8 one, starting with the functions at depth 0.
