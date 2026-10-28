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

## The Mac profile (`speedfix_mac_parallels.ini`)

For the M5 MacBook Pro in Parallels with dgVoodoo (28 Oct: the shipped settings gave 35-82 ms frames there, the
machine's baseline; `frame_step=1` made motion run at real time below 60 fps, faster than the game's own 83-90%, and
in bursts when a fast frame gets no tick). Rename it to `speedfix.ini`. It turns `frame_step` off and the levers that
cut frame time or stutter on that machine on (skin_merge, batch_up_draws, merge_scenes, lazy_state, block_cache,
extended_state_cache, vs_dedupe, sse_skinning, skip_actor_occlusion), and runs the in-session A/B of skip_actor_occlusion:
the log's `ab ...: A (off) X ms, B (on) Y ms` line says what it gains there. occlusion_late is off: on dgVoodoo its late
read waits as long as the game's (3 to 5 ms each; 15.6 to 21.5 ms a frame in a crowd, owner's run of 28 Oct).

## Files (dist/, from build.sh)

| File | What |
|---|---|
| `speedfix_standalone.dll` | inject this one |
| `gfx_recon_ingame.dll` | the verified tree, in-game build: the forks call functions in it |
| `speedfix_manifest.txt`, `speedfix_manifest_6AC37846.txt` | which bytes each hooked entry must have, for the analysed build and for 6AC37846 (the build the owner runs; its moved code ranges) |
| `speedfix.ini` | the settings above (`speedfix.ini.example` keeps the shipped copy) |
| `speedfix_mac_parallels.ini` | the profile for the Mac in Parallels (above) |
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
