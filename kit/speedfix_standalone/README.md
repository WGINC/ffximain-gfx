# speedfix, standalone

speedfix (kit/speedfix/) without the in-game kit: one DLL injected into pol.exe, with its files next to it. The
speedfix sources are linked in unchanged; `sfs_host.c` is the part of xikit they need (the manifest, the byte checks,
the build translation, trampoline hooks, the device layer, the per-frame call) and nothing else: no function
swapping, profiler, captures or fact recorder.

## What it does

Everything kit/speedfix/README.md lists, set in `speedfix.ini`. The shipped `speedfix.ini` turns on:
- `frame_step=1`: the game's time step answered in whole 1/60 s ticks with the fraction carried, so walking, the
  camera and animations follow real time at any frame rate (docs/RENDERER.md 3a and 3b: the speed that varied with the
  frame rate in the owner's runs of 26 Oct). Frame_Render, Camera_GetFrameScale and Camera_GetClampedField28 run
  speedfix's forks: verified tree functions with the step changed, every other difference none (kit/speedfix/fork/).
- the defaults: the redundant-state cache, the vertex-shader cache, the occlusion stagger (8), texture compression.
Every other lever is off; turning one on is as in kit/speedfix/README.md.

What it does not do: anything about the renderer itself (dgVoodoo, d3d8on11 or the system's d3d8.dll stays as it is),
or the 60 fps wait (Windower's frame divisor 0 already removes it; `uncap=1` does the same without Windower).

## Files (dist/, from build.sh)

| File | What |
|---|---|
| `speedfix_standalone.dll` | inject this one |
| `gfx_recon_ingame.dll` | the verified tree, in-game build: the forks call functions in it |
| `speedfix_manifest.txt`, `speedfix_manifest_6AC37846.txt` | which bytes each hooked entry must have, for the analysed build and for 6AC37846 (the build the owner runs; its moved code ranges) |
| `speedfix.ini` | the settings above (`speedfix.ini.example` keeps the shipped copy) |
| `VERSION.txt` | the source commit and each file's hash |

## Use

1. Put the files of dist/ in one folder.
2. Do not load xikit.dll (with `mods=speedfix.dll`) or speedfix_hook.dll in the same game: one speedfix at a time.
   speedfix_hook is detected and its functions are left alone; xikit is not.
3. Inject `speedfix_standalone.dll` into pol.exe like xikit or d3dprof (System Informer "Inject DLL", or any
   injector), before or after the game has started.
4. `speedfix_standalone.log` in the same folder says what happened: the build found, each hook made or refused (with
   the byte that differed), "function hooks: N written", and speedfix's own lines and minute summaries.

It does nothing, and says so in the log, when FFXiMain.dll is a build with no manifest, when gfx_recon_ingame.dll is
not the one the manifest was made from, or when a hooked entry's bytes are not the analysed build's.

## Build and test

    kit/xikit/build.sh                                       # first: the reconstruction DLL and the manifests
    FFXI_DLL=... FFXI_DLL_6AC37846=... kit/speedfix_standalone/build.sh
    FFXI_DLL=... python3 kit/speedfix_standalone/test/test_standalone.py

The test runs speedfix's own test host (kit/speedfix/test/sf_host.c: the real FFXiMain.dll mapped, a fake device that
records what reaches it, the render thread played) with this DLL in place of xikit, through speedfix's six test
modes. 27-28 Oct: all 113 checks pass under Wine on the analysed build. Not tested here: build 6AC37846 itself (the
host maps the analysed DLL; the translation is xikit's, tested in kit/xikit/test), and the real game.
