# The 29 Nov session: the camera measured

One start of the game, about 8 minutes.

## What is new

- **The camera is in the frame log now:** its eye and the point it looks at, every frame (the fields the game builds
  the view from). The reader measures how evenly the camera moves and turns, as it does for your character.
- **Your 28 Nov run:** `precise_pacing` now works with the frame rate uncapped. The game's frame time is no longer
  whole milliseconds, and your character moves by exactly that time. It is on in the installer, with `pace_adaptive`.
- **The open question:** `step_exact` did nothing measurable for your character, because the character's movement
  does not read it. It does reach the camera's follow code. This start measures whether it changes the camera.

## Before you start

Unzip into the PlayOnlineViewer folder so that `d12_29nov` and `ffxi_kit` sit inside it; copy the zip's `kit` files
next to `xikit.dll`. If the kit is installed: `uninstall.bat` in `ffxi_kit` first. Command Prompt **as
administrator**, in `d12_29nov`.

## The start (`start.bat cam`)

Your installed settings, with `step_exact` on. Once you are on the street: `ab.bat`. For about 4 minutes speedfix
switches `step_exact` off and on by itself every 30 seconds. Walk, and **turn the camera a lot**: slow pans, quick
turns, the camera following you round corners. Stay in the zone this time. Note when the camera feels smooth and
roughly when (the Command Prompt shows the time). Close the game, `restore.bat`, `collect.bat`, and upload
`d12_29nov_results.zip`.

If the game crashes or hangs: wait about 40 seconds, close it, run `collect.bat` at once, and say at which step.
