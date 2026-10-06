# XIKIT SDK: writing graphics mods

A mod is a 32-bit DLL that xikit loads into the game. It can **replace** any of the 774 graphics functions in
the kit's manifest, **call the game's own version** of the function it replaced, **edit a reconstructed function**
as a diff against its gate-verified source (a fork), **intercept the Direct3D 8 device's methods**, call any
reconstructed function through the game's entry, read and write the game's objects through the reconstruction's
class layouts, and **draw every frame** with the game's device. With `mod_reload=1` a rebuilt mod is loaded again
while the game runs.

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
| `include/xikit_sdk.hpp` | The one header a mod includes: `XI_HOOK`, `XI_DEVICE_HOOK`, `XI_FORK`, `xikit::fn`, `xikit::game`, `XIKIT_MOD_GLOBALS` |
| `include/xikit_mod.h` | The kit–mod interface in C: `XikitHost`, the exports a mod has, the result codes |
| `include/xikit_game.hpp` | Generated (`sdk/gen_sdk.py`). Includes every tree header and adds `xigame::Name()`, a typed pointer to each reconstructed function's game entry, and `xihook::Name`, the manifest flags of each hookable function |
| `tree_include/` | The reconstruction's headers. Classes have their field offsets checked by `static_assert`. Also free functions, `orig::` (game code by address), the game's globals, and Direct3D 8 |
| `tree_src/` | The reconstruction's gate-verified source, one place to read what each function does |
| `template/my_mod.cpp` | A starting point |
| `examples/overlay.cpp` | A pass-through hook plus a box drawn every frame |
| `build_mod.sh`, `build_mod.bat` | Build a mod (a mod with forks: `build_mod.sh`) |
| `fork.py`, `fork_flags.txt`, `tree_placement.json` | Forks: `fork.py new` / `diff` (`gate` runs in the repository) |
| `gfx_recon_ingame.dll` | The kit's reconstruction, which a fork's calls to other reconstructed functions link against |

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
| `void xikit_mod_unload(void)` | Optional. Called before a reload replaces this build (`mod_reload=1`): release what the mod created (textures, threads). Its hooks are already off. |
| `uintptr_t xi_rel` | Defined by `XIKIT_MOD_GLOBALS`. The kit sets it to the game's load delta before init, and the tree headers' `XI_GAME`, `orig::` and globals rebase through it. |

`XikitHost` gives the mod:

| Field | |
|---|---|
| `version`, `size` | `XIKIT_API_VERSION` (2). Fields are only ever added at the end; a field exists when `size` reaches past it (`XI_DEVICE_HOOK` checks). |
| `game_base`, `rel` | Where `FFXiMain.dll` is loaded; `rel` is `game_base - 0x10000000` |
| `device` | The game's `IDirect3DDevice8` when the mod was loaded. If the game ever makes a new device, the kit hooks that one too: use the argument of `xikit_mod_frame`, which is always the current one |
| `recon`, `dir` | The reconstruction DLL's handle; the kit's folder |
| `log(fmt, ...)` | A line in `xikit.log`, prefixed with the mod's name |
| `hook(name, replacement, &previous)` | What `XI_HOOK` calls. The name is as the manifest gives it (`CDx::DrawFadeQuad`), its export symbol, or `"0x1000A820"`. |
| `reconstruction(name)` | The reconstruction's export of a manifest function |
| `device_hook(method, slot, replacement, &previous)` | Version 2. What `XI_DEVICE_HOOK` calls (below) |

`hook` results:

| Result | Meaning |
|---|---|
| `XIKIT_OK` | Hooked |
| `XIKIT_E_UNKNOWN` | Not in the manifest |
| `XIKIT_E_TAKEN` | Another mod hooks it |
| `XIKIT_E_REFUSED` | Its live entry differs from the analysed build, `speedfix_hook` patches it, or it has a register input (flag 8, below) |
| `XIKIT_E_LATE` | Called outside init |
| `XIKIT_E_NOPREV` | No way to reach its previous code |
| `XIKIT_E_OLDKIT` | The kit predates the call (`device_hook` on a version 1 kit) |

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

## Device hooks (`XI_DEVICE_HOOK`)

A mod can intercept any method of the game's `IDirect3DDevice8`: to filter or batch state, time calls, or change what
a draw asks for, without touching the game's functions.

```cpp
static XI_DEVICE_FN(SetRenderState) prev_SetRenderState;
static int __stdcall my_SetRenderState(IDirect3DDevice8 *dev, D3DRENDERSTATETYPE s, unsigned v) {
  /* ... */
  return prev_SetRenderState(dev, s, v);           // leave this out to keep the call from the device
}
// in xikit_mod_init:
XI_DEVICE_HOOK(host, SetRenderState, my_SetRenderState, prev_SetRenderState);
```

- **The type comes from `xi_d3d8.hpp`** (the tree's copy of d3d8.h's method order). `XI_DEVICE_FN(method)` is the
  vtable entry's type, `__stdcall` with the device first. `XI_DEVICE_HOOK` fails to compile when the replacement's
  type differs.
- **The slot is checked twice.** The macro computes it from the entry's place in `IDirect3DDevice8Vtbl`, and the kit
  refuses the hook when that slot is not the method it names in its own table (d3dprof's, from d3d8.h).
- **Several mods may hook one method.** The one hooked last runs first; each one's `previous` goes on to the next,
  then to the profiler and trace, then to Direct3D.
- **What the profile and trace record is what the hooks let through**, which is what the device gets. A call a
  hook passes on, or makes itself while handling one, is recorded at the call site of the call it is handling.
- **They follow the master switch** like function hooks. An alternating trace with a device-hook mod loaded compares
  frames with and without the mod at the device.
- **Calls from every thread run the hook.** A hook that keeps state must allow for calls from more than one thread
  (d3dprof counts device calls from threads other than the render thread separately).
- **A method no mod hooks costs nothing.** Its entry in the front vtable is the layer below's.

## Forks (`fork.py`, `XI_FORK`)

To change a reconstructed function a little, start from its gate-verified source rather than rewriting it:

```
python3 fork.py new CDx::DrawFadeQuad mymod/      ->  mymod/CDx__DrawFadeQuad.fork.cpp
  (edit it)
python3 fork.py diff mymod/CDx__DrawFadeQuad.fork.cpp     the edit, as a diff against the tree's source
python3 fork.py gate mymod/CDx__DrawFadeQuad.fork.cpp     (in the repository) the edit run against the game's code
```

```cpp
// in xikit_mod_init: the game's entry jumps to the mod's own definition of the function
XI_FORK(host, CDx::DrawFadeQuad);
```

Build it like any source of the mod: `build_mod.sh my_mod.dll my_mod.cpp mymod/CDx__DrawFadeQuad.fork.cpp`.

- **`new` copies the function's block from the tree's source verbatim**, with the source file's includes. The fork
  defines the function itself (`int CDx::DrawFadeQuad(int a1, int a2) { ... }`). `XI_FORK` hooks the function's
  entry with that definition's address; it refuses a virtual member. Tree classes have none.
- **`build_mod.sh` compiles a `.fork.cpp` with the tree's own flags** (`fork_flags.txt`, generated from
  tree/build.sh) and `XI_INGAME`. It links the mod against `gfx_recon_ingame.dll`, where the fork's calls to other
  reconstructed functions go.
- **The source is verified, the copy is not until it is gated.** A function's machine code can depend on the rest
  of its source file: GCC inlines and allocates registers across the functions it sees. So `fork.py` does not
  claim the fork compiles to the gated code. `gate` builds the tree's in-game build with this one function replaced
  by the fork, compiled exactly as the mod compiles it. It then runs the gate (`diffemu_gfx.py`, with coverage)
  against the game's original function: on fuzz, or on a live dump with `--live`.
  - An unedited fork must PASS as the tree does.
  - An edited one shows where it now differs from the game. For example, `D3DRS_FOGENABLE` changed from 0 to 1
    gives `stubbed call #7 differs: ... 0x0000001C, 0x00000000 / ... 0x0000001C, 0x00000001`.
- **C library calls differ by place:** in the game, a fork's `memcpy` and similar calls are the mod's (msvcrt); in
  `gate` they are the tree's `rt.c`.

## Reloading (`mod_reload=1`)

With `mod_reload=1` in `xikit.ini` (read at the start), the kit loads each mod from a copy in `xikit_mods_loaded\`.
The build can then write `my_mod.dll` while the game runs.

- **When:** within 60 frames of the build. The kit looks every 30 frames, and loads a changed file only once it is
  unchanged at the next look. Creating `xikit_reload_now.txt` reloads every mod.
- **What happens:** the old build's `xikit_mod_unload` runs, all its function and device hooks are taken off, and
  the new build's `xikit_mod_init` runs and hooks again. If the new build cannot be loaded, the old one stays.
- **The new build starts with fresh statics.** Nothing carries over except what the mod keeps outside itself.
- **Old builds stay loaded but are never called again.** A thread may still be returning through one. Each
  reload costs one copy of the DLL in memory.
- **Trampolines are built once,** from the untouched entry, so a reloaded hook's `previous` is the same code.

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
  - hooks staying on with `swap=0`;
  - device hooks:
    - one passing on to the device;
    - one answering by itself;
    - a slot that does not match its name, refused;
    - the profiler naming a hooked call by its caller;
  - a fork: `fork.py new` unedited, built into the mod, hooked with `XI_FORK` and run. `fork.py gate` PASSes it
    against the game's SetColor. The edited-fork failure above was run by hand;
  - a reload: the rebuilt file loaded, the old build's unload called, its hooks replaced by the new build's, and
    `xikit_reload_now.txt`.

  The example's drawing is only compiled: the test host's fake device implements Present alone. Nothing here has
  been run in the game yet.
- **The overlay draws into whatever render target is bound at Present.** That is normally the back buffer, but this
  has not been checked against the game.
- **A mod's code is not quarantined.** A crash in it is a crash of the game.
