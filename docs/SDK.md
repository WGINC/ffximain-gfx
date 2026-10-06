# XIKIT SDK: writing graphics mods

A mod is a 32-bit DLL that xikit loads into the game. It can **replace** any of the 774 graphics functions in
the kit's manifest, **call the game's own version** of the function it replaced, call any reconstructed function
through the game's entry, read and write the game's objects through the reconstruction's class layouts, and
**draw every frame** with the game's Direct3D 8 device.

Everything the SDK knows about a function comes from the reconstruction (`tree/`): its declaration, which is the
one the gate verified the reconstructed body with, and its body, which shows what the function does. Read the body
before you change a function.

## What you need

- **MinGW-w64, 32-bit (i686) g++.** On Windows: MSYS2's `mingw32` shell (`pacman -S mingw-w64-i686-gcc`) or a
  WinLibs i686 build. On Linux: `i686-w64-mingw32-g++`. MSVC cannot compile the headers: they use GCC attributes.
- **The kit** (`xikit.dll`, `gfx_recon_ingame.dll`, `xikit_manifest.txt`, `xikit.ini`) installed as `docs/KIT.md`
  describes.
- **This SDK** (`xikit_sdk.zip`, built by `sdk/package.sh`):

| Folder | What |
|---|---|
| `include/xikit_sdk.hpp` | The one header a mod includes: `XI_HOOK`, `xikit::fn`, `xikit::game`, `XIKIT_MOD_GLOBALS` |
| `include/xikit_mod.h` | The kit–mod interface in C: `XikitHost`, the exports a mod has, the result codes |
| `include/xikit_game.hpp` | Generated (`sdk/gen_sdk.py`). Includes every tree header and adds `xigame::Name()`, a typed pointer to each reconstructed function's game entry, and `xihook::Name`, the manifest flags of each hookable function |
| `tree_include/` | The reconstruction's headers. Classes have their field offsets checked by `static_assert`. Also free functions, `orig::` (game code by address), the game's globals, and Direct3D 8 |
| `tree_src/` | The reconstruction's gate-verified source, one place to read what each function does |
| `template/my_mod.cpp` | A starting point |
| `examples/overlay.cpp` | A pass-through hook plus a box drawn every frame |
| `build_mod.sh`, `build_mod.bat` | Build a mod |

## A mod in five steps

1. **Pick the function.** `xihook::` in `include/xikit_game.hpp` lists the ones a mod may hook, with their manifest
   flags. Find its declaration in `tree_include/`, and its body in `tree_src/` by searching for its name or its
   address (every body starts with `// FUNCTION: FFXIMAIN 0x...`).
2. **Write the replacement** with the same signature. A member function becomes a `__thiscall` function that takes
   the object first:

   ```cpp
   #include "xikit_sdk.hpp"
   XIKIT_MOD_GLOBALS                                   // once per mod

   static xikit::fn<decltype(&CDx::DrawFadeQuad)> prev_DrawFadeQuad;   // the game's own function
   static int __thiscall my_DrawFadeQuad(CDx *self, int a1, int a2) {
     /* before */
     int r = prev_DrawFadeQuad(self, a1, a2);         // leave this out to replace the function outright
     /* after */
     return r;
   }
   ```
3. **Hook it** in `xikit_mod_init`. Hooks can only be made there:

   ```cpp
   extern "C" __declspec(dllexport) int xikit_mod_init(const XikitHost *host) {
     return XI_HOOK(host, CDx::DrawFadeQuad, my_DrawFadeQuad, prev_DrawFadeQuad);   // 0 = hooked
   }
   ```
   `XI_HOOK` fails to compile when the replacement's type differs from the declaration's, including the calling
   convention, `this`, the parameters and the return type.
4. **Build** with `build_mod.sh my_mod.dll my_mod.cpp` (or `build_mod.bat`).
5. **Install.** Put the DLL next to `xikit.dll` and add it to `xikit.ini` as `mods=my_mod.dll` (comma-separated for
   several). Start the game. `xikit.log` shows each mod's load, every hook it made and why any hook was refused.

## The interface (`xikit_mod.h`)

A mod exports:

| Export | |
|---|---|
| `int xikit_mod_init(const XikitHost *host)` | Required. Called once, after the game has made its Direct3D device and before the kit's first frame. Return 0. |
| `void xikit_mod_frame(void *device)` | Optional. Called every frame on the render thread, just before Present. The game has already ended its scene by then, so wrap any drawing in `BeginScene`/`EndScene` (see the example). |
| `uintptr_t xi_rel` | Defined by `XIKIT_MOD_GLOBALS`. The kit sets it to the game's load delta before init, and the tree headers' `XI_GAME`, `orig::` and globals rebase through it. |

`XikitHost` gives the mod:

| Field | |
|---|---|
| `version`, `size` | `XIKIT_API_VERSION` (1). Fields are only ever added at the end. |
| `game_base`, `rel` | Where `FFXiMain.dll` is loaded; `rel` is `game_base - 0x10000000` |
| `device` | The game's `IDirect3DDevice8` when the mod was loaded. If the game ever makes a new device, the kit hooks that one too: use the argument of `xikit_mod_frame`, which is always the current one |
| `recon`, `dir` | The reconstruction DLL's handle; the kit's folder |
| `log(fmt, ...)` | A line in `xikit.log`, prefixed with the mod's name |
| `hook(name, replacement, &previous)` | What `XI_HOOK` calls. The name is as the manifest gives it (`CDx::DrawFadeQuad`), its export symbol, or `"0x1000A820"`. |
| `reconstruction(name)` | The reconstruction's export of a manifest function |

`hook` results:

| Result | Meaning |
|---|---|
| `XIKIT_OK` | Hooked |
| `XIKIT_E_UNKNOWN` | Not in the manifest |
| `XIKIT_E_TAKEN` | Another mod hooks it |
| `XIKIT_E_REFUSED` | Its live entry differs from the analysed build, `speedfix_hook` patches it, or it has a register input (flag 8, below) |
| `XIKIT_E_LATE` | Called outside init |
| `XIKIT_E_NOPREV` | No way to reach its previous code |

## What `previous` is

- **A trampoline to the game's own code, for 721 of the 774 functions.** The kit copies the first whole instructions
  of the function, at least the 5 bytes its `jmp` overwrites and at most 16, out of the live entry, after checking
  them against the analysed build. It re-aims any `call`/`jmp rel32` among them and appends a jump back to the rest
  of the function. `make_manifest.py` decides per function, from the game's code, whether this is safe (`T=` in the
  manifest), and says no when:
  - one of the first instructions branches relatively or returns;
  - the copied bytes pass the end of the function;
  - any code in the game jumps into them.
- **The reconstruction's export, for the other 53.** These are mostly functions that return within their first 5
  bytes. That is the gate-verified C++ of the same function. The log says which one a hook got, and
  `xihook::Name_TRAMP` says it at compile time.

## Calling other game code

| Call | What it runs |
|---|---|
| `xigame::CDx__SetTexture()(cdx, stage, tex)` | Whatever is at the game's entry: the game's code, the kit's swap, or another mod's hook. The type comes from the tree's declaration. |
| `orig::Name(...)` (`tree_include/gfx_orig.hpp`) | The same, for the game functions the reconstruction itself calls |
| A tree member called directly (`cdx->SetTexture(...)`) | The reconstruction's code, bypassing any hook. Link the mod against the reconstruction: `build_mod.sh out.dll src.cpp -- gfx_recon_ingame.dll`. The kit has loaded it by the time any mod runs. |
| Game globals (`tree_include/gfx_globals.hpp`) and objects | Use the class layouts; `xikit::game<T>(0x1045666C)` turns any analysed address into a pointer |

## How hooks behave with the kit's other switches

- **Hooks are on whatever `swap` says.** `swap=0` turns off the reconstruction swaps, not the mods.
- **Hooks follow the master switch.** F9 and the unswapped frames of a trace turn them off along with the swaps, so
  an A/B trace with a mod loaded compares the game with and without the mod. Frame callbacks always run.
- **A hooked function is never a bisect suspect**, and the call counter does not count it.
- **The bisect, profiler, trace and crash quarantine work as before.** A crash inside the reconstruction quarantines
  the function it is in (the kit knows each function's code from the DLL's symbol table, `!code` in the manifest).
  - A hook whose previous is a trampoline is not affected: it never runs that reconstruction.
  - A hook whose previous is the reconstruction goes off when that function is quarantined.
  - `hook()` refuses such a hook if the function is already in `xikit_quarantine.txt`.
- **A trace with `swap=0`** compares the game with and without the mods' hooks.

## Limits

- **Only manifest functions can be hooked:** the 774 the kit has verified are safe to patch at the entry. Other game
  code can be called (`orig::`, `xigame::`) but not replaced.
- **How much the types are worth depends on the gate.** For the 154 functions with flag 4 (incomplete gate coverage)
  the declaration is backed by less evidence.
- **The register-input data has a known blind spot (task #64).** Flag 8 marks a function whose game code reads a
  register its declaration does not carry (data/edx_use.json, data/ecx_use.json). The kit refuses to hook it,
  because the replacement could neither see the register nor pass it on. One manifest function has it:
  `Obj_104E1D4C::Cursor_Draw` (EDX). The data behind this flag can miss callees that leave a register untouched, so
  other functions may be affected without being flagged.
- **What is tested and what is not.** The Wine tests (`kit/xikit/test`, modes `mod` and `modrel`) cover:
  - loading a mod;
  - `xi_rel`;
  - a hook replacing a function;
  - its trampoline running the game's own code, with the game at its base and loaded elsewhere;
  - a trampoline whose copied `call` is re-aimed;
  - a refused name;
  - the frame callback;
  - hooks staying on with `swap=0`.

  The example's drawing is only compiled: the test host's fake device implements Present alone. Nothing here has
  been run in the game yet.
- **The overlay draws into whatever render target is bound at Present.** That is normally the back buffer, but this
  has not been checked against the game.
- **A mod's code is not quarantined.** A crash in it is a crash of the game.
