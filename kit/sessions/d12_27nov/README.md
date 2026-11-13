# The 27 Nov session: each frame moved by its own time, and an adaptive pace

One start of the game, about 10 minutes.

## What is new

- **`step_exact`:** the game moves the camera, characters and effects each frame by a time step. It measures that
  step in whole milliseconds and uses the average of the last 4 frames, so after a slow frame things move too little,
  then catch up. In your walks the step was 7-13% off each frame's real time. With `step_exact` speedfix gives the
  game this frame's own time from the precise counter, kept within 1 ms of the game's own count so its clock stays
  right. It changes no frame and caps nothing.
- **`pace_adaptive`:** speedfix holds each frame just long enough that frames go out at an even interval: the 90th
  percentile of the last 120 frames' own time (about a second). It follows the scene up and down; there is no fixed
  number and no waiting for the display. It costs a little average frame rate for even spacing.

## Before you start

Unzip into the PlayOnlineViewer folder so that `d12_27nov` and `ffxi_kit` sit inside it; copy the zip's `kit` files
next to `xikit.dll`. If the kit is installed: `uninstall.bat` in `ffxi_kit` first. Command Prompt **as
administrator**, in `d12_27nov`. The same street and walk as before.

## The start (`start.bat smooth`)

Both are on from the start. Once you are on the street, walking: `ab.bat`. For about 6 minutes speedfix switches
by itself, every 30 seconds, in three rounds of 2 minutes:
1. `step_exact` off and on (the pace on throughout);
2. `pace_adaptive` off and on (`step_exact` on throughout);
3. both off and both on.

Keep walking and turning the camera along your usual route until `ab.bat` says done. **When does the camera feel
smooth, and can you tell the switches apart?** Note what you notice and roughly when (the Command Prompt shows the
time). Close the game, `restore.bat`, `collect.bat`, and upload `d12_27nov_results.zip`.

If the game crashes or hangs: wait about 40 seconds, close it, run `collect.bat` at once, and say at which step.
