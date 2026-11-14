# The 28 Nov session: the character moved by exact time

One start of the game, about 10 minutes.

## What your 27 Nov run showed

- **`pace_adaptive` works:** frames taking over twice as long as their neighbours fell from 67 to 19 a minute (alone)
  and from 77 to 22 (with `step_exact`). It is now on in the installer's settings.
- **The judder in movement has a cause:** your character's position is logged every frame. It moves each frame by
  exactly that frame's time as the game measures it (within 0.1-0.7%). That measure is in whole milliseconds, so at
  7 ms frames each step is up to 14% off the real time, and the walking speed jitters 15-20% from frame to frame.
  `step_exact` did not reach it: the movement reads the game's own frame time, not the step speedfix answers.
- **Why `precise_pacing` did not help before:** it makes the game's frame time exact (the precise counter instead of
  whole milliseconds), but it only recognised the game's own frame code. With `uncap`, speedfix's copy of that code
  runs instead, so it never applied. Now speedfix's copy says which timer call it is making, and `precise_pacing`
  answers it.

No cap: the 120 fps cap measured no better on screen (25 Nov), so there is none.

## Before you start

Unzip into the PlayOnlineViewer folder so that `d12_28nov` and `ffxi_kit` sit inside it; copy the zip's `kit` files
next to `xikit.dll`. If the kit is installed: `uninstall.bat` in `ffxi_kit` first. Command Prompt **as
administrator**, in `d12_28nov`. The same street and walk as before; walk in straight runs as well as turning.

## The start (`start.bat smooth2`)

`pace_adaptive`, `precise_pacing` and `step_exact` are on. Once you are on the street, walking: `ab.bat`. For about
6 minutes speedfix switches by itself, every 30 seconds, in three rounds of 2 minutes:
1. `precise_pacing` off and on;
2. `step_exact` off and on;
3. both off and both on.

Keep walking until `ab.bat` says done. **Does your character's and the camera's movement look smoother in some
stretches?** Note roughly when. Close the game, `restore.bat`, `collect.bat`, and upload `d12_28nov_results.zip`.

If the game crashes or hangs: wait about 40 seconds, close it, run `collect.bat` at once, and say at which step.
