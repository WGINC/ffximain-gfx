# The measurement session (4 Nov): where every millisecond of a frame goes, inside and outside the graphics

What this answers, with the source of each answer:

| Question | Measured by | Phase |
|---|---|---|
| How long each frame takes, its spread, the stutters | the kit's QPC time at each Present (`frames=1`), PresentMon | all |
| How much of a frame is the device (Present, the GPU) and how much is the game | the kit (inside / outside Present), PresentMon's GPU busy time | W1, W2 |
| What the game's own pacing does each frame (the 60 fps wait, the step's truncation) | the game's fields read raw each frame (docs/RENDERER.md 3) | all |
| Where the render thread is, by Frame_Render's call and one level down; what the other threads do | every thread sampled with its stack (`sample=3`) | W2, W5 |
| The exact time of each call in the main loop and Frame_Render (zone, actors, effects, UI, sound, ...) | each call timed through a stub (`callsites=2`; `4`: three levels down) | W2, W3, W5 |
| The game's Windows calls: waits (the Sleep in its pacing), file reads, messages, input, GDI text, network, heap | each call through its import slot timed (`imports=1`, `2`) | W2, W3, W5 |
| Direct3D: each call site's time | the profiler (`profile=1`, `call_time=1`) | W4 |
| What the draws cost the CPU (all of them), and the occlusion readbacks | speedfix's A/B: `null_draws`, `skip_all_occlusion_readbacks` | W6 |
| What is left with your best speedfix levers | W2's measurements with the levers on | W5 |
| Emulation (Windows on ARM): the emulator, the driver, Windows' own code | the native sampler (`sample=2`) | W2b |
| The machine: CPU of every thread, memory, page faults, disk reads | `sysstats=1` | W1, W2, W3, W5 |
| The setup: the game's settings, the display, timers (Sleep(1) as measured), what common calls cost here | `env=1` (xikit_env) | W1, W2, W5 |
| CrossOver: the same, and the Mac's own view (Wine's processes, wineserver, system calls, Rosetta, GPU power) | the kit in the bottle, and `mac/measure_mac.sh` | M1-M3 |

Everything in W phases runs the game as it is: nothing is swapped (`swap=0`). The kit only reads, except the call
timing, which points calls at a stub that calls the same function (each call costs a little more; the kit measures
how much at the start and writes it in xikit.log). W1 has none of it: it is the clean baseline the others are
compared with.

## Files

Unzip into the folder of `xikit.dll` (the PlayOnlineViewer folder), so that this folder (`measure_04nov`) sits inside
it, and copy the kit files from the zip's `kit` folder next to `xikit.dll` (replace the old ones): `xikit.dll`,
`gfx_recon_ingame.dll`, `xikit_manifest.txt`, `xikit_manifest_6AC37846.txt`, `xikit_measure.txt`, `xikit_facts.txt`,
`speedfix.dll`, `xiload.exe`, `xisamp_arm64.exe`, `xisamp_x64.exe`, `VERSION.txt`. Keep your own `xikit.ini` and
`speedfix.ini` aside: the phases overwrite them.

`xiload.exe` is new: it loads the kit into the game by itself (no System Informer). `start_phase.bat` starts it.

## Windows (Parallels): the phases

For each phase: open a Command Prompt **as administrator** (PresentMon needs it), then

    cd /d "<the PlayOnlineViewer folder>\measure_04nov"
    start_phase.bat W1_clean

and start the game as you always do (dgVoodoo, your usual settings). Play the steps below, close the game normally,
and go to the next phase. Each phase writes its own log folder.

In the order of what they are worth (W1, W2 and W5 are the ones that matter most):

| Phase | What | Steps |
|---|---|---|
| `W1_clean` | the baseline: frame log, machine, environment | all of them (below). Optional, during step 1: `wpr_optional.bat` in a second administrator prompt (20 s of Windows' own trace; a large file) |
| `W2_where` | where the time goes: samples, the main loop's and Frame_Render's calls, the Windows calls | all |
| `W5_levers` | W2 with your best speedfix levers (`speedfix.ini` in its folder: sse_skinning, batching, lazy_state, the state caches) | all |
| `W6_ab` | the draws' cost: speedfix A/B of `null_draws` (**nothing is drawn during its B phases: the screen goes black or freezes, that is expected**) and of the occlusion readbacks | stand still in the plaza (step 1) for 7 minutes, until xikit.log says `every group of ab_levers has run` |
| `W3_deep` | three levels of calls below the main loop timed, and the hot Windows calls | steps 1, 3, 4, 6 |
| `W4_d3d` | the Direct3D profiler with each call's time | steps 1, 3, 6 |
| `W2b_native` | the native sampler (the emulator and the driver) | steps 1, 3, 6 |

### The steps (the same in every phase)

Press **Ctrl+Alt+M (or Ctrl+Alt+F7)** in the game at the start of each step; the frame log numbers the steps by it. Note anything
unusual (a long stutter, a black screen outside W6) with the step number.

1. Port Jeuno, the busy plaza: stand still, camera still, **60 s**.
2. Rotate the camera slowly all the way round, a few times: **30 s**.
3. Run around the plaza: **60 s**.
4. Open the main menu and the inventory, scroll through it: **30 s**, then close them.
5. Walk through a zone line (to Lower Jeuno or out of town), and stand **20 s** after you arrive.
6. Somewhere busy: a fight with effects, or a crowd: **60 s**.
7. Stand still anywhere: **30 s**. Close the game.

About 7 minutes a phase, plus logging in.

### Sending

After the last phase run `collect.bat` (from this folder; no administrator needed) and upload
`measure_04nov_results.zip` from the PlayOnlineViewer folder. If it is too large, send the W1, W2 and W5 folders
first.

## CrossOver on the Mac (M phases)

The same kit runs inside the bottle (it was tested under Wine). Copy the zip's `kit` files and this whole folder
next to `pol.exe` inside the bottle (CrossOver: the bottle's C: drive, `Program Files (x86)/PlayOnline/SquareEnix/
PlayOnlineViewer`; your path may differ). Then in Terminal:

    bash "<that folder>/measure_04nov/mac/measure_mac.sh" "<bottle name>" W1_clean wined3d

It starts xiload and the game in the bottle, asks you to press Enter when you reach step 1 and again at step 6, and
at the end puts a zip on your Desktop. During the run it records what macOS shows (every process's CPU, threads,
context switches and system calls each second, Wine's server included; 15-s stack samples of the game process and
of wineserver).

| Phase | Run it for |
|---|---|
| `M1` = `W1_clean` | each graphics setting you can pick in CrossOver (the default first; then D3DMetal or DXVK if your bottle offers them; then dgVoodoo's d3d8.dll in the game folder if you use it there). The third argument is your name for the setting |
| `M2` = `W2_where` | the default setting, and the one that ran best in M1 |
| `M3` = `W3_deep` | the default setting (optional) |

Options, set before the command: `HUD=1` (Metal's and DXVK's on-screen counters: take a photo of them in step 1),
`POWER=1` (asks for your password: CPU and GPU power and busy time each second, `powermetrics`), `NOSTART=1` (you
start the game from CrossOver yourself; the script waits for it). Upload the Desktop zips.

## What happens with the results

`kit/xikit/measure_read.py` turns each log folder into `report.md`: the frame times by step; the game's pacing; the
render thread's time by Frame_Render's calls and one level down; the exact times of the timed calls per frame and in
the stutters; the Windows calls; every thread's CPU; PresentMon's GPU times. From those, the work toward a stable
60 fps is sized by where the time is: in the game's frame (which calls), in the device and the GPU, in the pacing
itself, outside the frame (other threads, the system), or in the platform (the emulator, Wine, the driver).
