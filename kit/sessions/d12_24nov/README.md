# The 24 Nov session: the game-thread savings measured, the ARM crash narrowed, the native sampler

Four starts of the game and the installer, about 20 minutes.

## What your 23 Nov run showed

- **Where the game's thread spends its time** (start prof, 2,256 samples). Of my own code on that thread:
  - a test-only setting read on every draw: 6.1% of the samples;
  - state blocks copied by checking all 1,024 flags each time: 2.8%;
  - the kit asking Windows which thread is calling, on every hooked device call: 2.0%;
  - the four blend translations done on every draw: 1.95%.

  Each is fixed with the same result as before. Two more like them (speedfix's thread check, the renderer's caller
  lookup per draw) are fixed too, without a share of their own in the profile. Together about 12% of the samples;
  start fast measures what that is in frame time.
- **A quarter of the samples (24%) sat on one address outside any module.** On the Mac's emulator that is how the
  x86 view shows a thread that is in native code (the emulator, Windows, the driver): the 18 Oct native sampler runs
  found the same placeholder. Start native looks inside it.
- **The arm crash, again on the kit's environment thread,** in Windower's Hook.dll, through Windows' USER32. That
  thread no longer calls USER32 unless asked (`env_windows=1`). Starts arm0 and arm1 split the question.
- **The installer run:** by the logs it ran faster, not slower: 143, 132 and 125 frames a second per minute, against
  127, 110, 105 and 99 on 22 Nov. But xiload was still open from the session and had loaded the kit as well. The
  installer start records no frame-by-frame times, so smoothness itself was not measured. `restore.bat` now closes xiload.

## Before you start

Unzip into the PlayOnlineViewer folder so that `d12_24nov` and `ffxi_kit` sit inside it; copy the zip's `kit` files
next to `xikit.dll`. If the kit is installed: `uninstall.bat` in your old `ffxi_kit` first. Command Prompt **as
administrator**, in `d12_24nov`.

Use the same town street for every start, and walk the same way each time.

## 1. The savings (`start.bat fast`)

The installed settings. `mark walk`, 2 minutes walking; `mark stand`, 1 minute standing among people. Close the game,
`restore.bat`.

Optional: if you have Windower addons that draw text on screen, unload them for one more minute: `mark notext`, walk
1 minute, then load them again. Windows' text libraries and Windower's Hook.dll were about 8% of the game thread's
samples; this says how much of that is the addons.

## 2. The ARM64 executor without the environment thread (`start.bat arm0`)

`mark walk`, 2 minutes walking. If it crashes: wait, close it, `collect.bat` at once, and say "arm0 crashed".
Close the game, `restore.bat`.

## 3. The ARM64 executor with the environment thread (`start.bat arm1`)

Only if arm0 ran its 2 minutes. `mark walk`, 2 minutes walking. If it crashes, as above ("arm1 crashed"). Close the
game, `restore.bat`.

What the two say:
- arm0 and arm1 both run: fixed. The crash was the environment thread's USER32 queries.
- arm0 runs, arm1 crashes: the environment thread crashes even without those queries. It stays off on ARM.
- arm0 crashes: the environment thread is not the cause. The crash log names the place.

## 4. The native sampler (`start.bat native`)

As start 1: `mark walk`, 2 minutes walking. A second program samples the game's thread from outside: its frame rate
is not one to compare. Close the game, `restore.bat`.

## 5. The installer

Check that no xiload window is open (`restore.bat` closes it). In `ffxi_kit`: `install.bat /defaults`, start the game
as usual, walk the same street a minute, change zone once. Close the game. Keep it installed or `uninstall.bat`.

Then `collect.bat` in `d12_24nov`, and upload `d12_24nov_results.zip`.

If the game crashes or hangs: wait about 40 seconds, close it, run `collect.bat` at once, and say at which step.
